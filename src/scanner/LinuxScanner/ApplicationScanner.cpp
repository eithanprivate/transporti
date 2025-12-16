#include "ApplicationScanner.h"
#include <QFile>
#include <QTextStream>
#include <QStandardPaths>
#include <QDir>
#include <QtConcurrent>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

ApplicationScanner::ApplicationScanner(QObject *parent)
    : ScannerBase(parent)
    , m_cancelled(0)
{
}

void ApplicationScanner::startScan()
{
    qDebug() << "ApplicationScanner: Starting scan...";
    m_cancelled.storeRelaxed(0);

    // Run scan in separate thread
    QtConcurrent::run([this]() {
        QList<FileInfo> files;

        // Detect package manager
        PackageManager pm = detectPackageManager();
        QStringList packages;
        QString managerName;

        if (m_cancelled.loadRelaxed()) {
            return;
        }

        switch (pm) {
        case Dpkg:
            qDebug() << "ApplicationScanner: Detected dpkg (Debian/Ubuntu)";
            managerName = "dpkg";
            packages = scanDpkgPackages();
            break;
        case Rpm:
            qDebug() << "ApplicationScanner: Detected rpm (Fedora/RedHat)";
            managerName = "rpm";
            packages = scanRpmPackages();
            break;
        case Pacman:
            qDebug() << "ApplicationScanner: Detected pacman (Arch Linux)";
            managerName = "pacman";
            packages = scanPacmanPackages();
            break;
        default:
            qDebug() << "ApplicationScanner: No supported package manager found";
            emit scanError("No supported package manager found (dpkg, rpm, or pacman)");
            return;
        }

        if (m_cancelled.loadRelaxed()) {
            emit scanError("Scan cancelled by user");
            return;
        }

        if (packages.isEmpty()) {
            qDebug() << "ApplicationScanner: No packages found";
            emit scanError("No packages found");
            return;
        }

        qDebug() << "ApplicationScanner: Found" << packages.size() << "installed packages";

        // Create package list file
        QString filePath = createPackageListFile(packages, managerName);

        if (!filePath.isEmpty()) {
            FileInfo info;
            info.path = filePath;
            info.size = QFileInfo(filePath).size();
            info.category = "Applications";
            files.append(info);

            qDebug() << "ApplicationScanner: Created package list at" << filePath;
            emit scanComplete(files, info.size);
        } else {
            emit scanError("Failed to create package list file");
        }
    });
}

void ApplicationScanner::cancelScan()
{
    qDebug() << "ApplicationScanner: Cancelling scan...";
    m_cancelled.storeRelaxed(1);
}

ApplicationScanner::PackageManager ApplicationScanner::detectPackageManager()
{
    // Check for dpkg (Debian/Ubuntu)
    QProcess dpkg;
    dpkg.start("which", QStringList() << "dpkg");
    dpkg.waitForFinished(1000);
    if (dpkg.exitCode() == 0) {
        return Dpkg;
    }

    // Check for rpm (Fedora/RedHat)
    QProcess rpm;
    rpm.start("which", QStringList() << "rpm");
    rpm.waitForFinished(1000);
    if (rpm.exitCode() == 0) {
        return Rpm;
    }

    // Check for pacman (Arch)
    QProcess pacman;
    pacman.start("which", QStringList() << "pacman");
    pacman.waitForFinished(1000);
    if (pacman.exitCode() == 0) {
        return Pacman;
    }

    return Unknown;
}

QStringList ApplicationScanner::scanDpkgPackages()
{
    QStringList packages;
    QProcess process;

    process.start("dpkg", QStringList() << "--get-selections");
    process.waitForFinished(10000);

    if (process.exitCode() != 0) {
        qDebug() << "ApplicationScanner: dpkg failed:" << process.errorString();
        return packages;
    }

    QString output = process.readAllStandardOutput();
    QStringList lines = output.split('\n', Qt::SkipEmptyParts);

    for (const QString &line : lines) {
        if (m_cancelled.loadRelaxed()) {
            break;
        }

        QStringList parts = line.split('\t', Qt::SkipEmptyParts);
        if (parts.size() >= 2 && parts[1] == "install") {
            packages.append(parts[0]);
        }
    }

    return packages;
}

QStringList ApplicationScanner::scanRpmPackages()
{
    QStringList packages;
    QProcess process;

    process.start("rpm", QStringList() << "-qa");
    process.waitForFinished(10000);

    if (process.exitCode() != 0) {
        qDebug() << "ApplicationScanner: rpm failed:" << process.errorString();
        return packages;
    }

    QString output = process.readAllStandardOutput();
    packages = output.split('\n', Qt::SkipEmptyParts);

    return packages;
}

QStringList ApplicationScanner::scanPacmanPackages()
{
    QStringList packages;
    QProcess process;

    process.start("pacman", QStringList() << "-Q");
    process.waitForFinished(10000);

    if (process.exitCode() != 0) {
        qDebug() << "ApplicationScanner: pacman failed:" << process.errorString();
        return packages;
    }

    QString output = process.readAllStandardOutput();
    QStringList lines = output.split('\n', Qt::SkipEmptyParts);

    for (const QString &line : lines) {
        if (m_cancelled.loadRelaxed()) {
            break;
        }

        QStringList parts = line.split(' ', Qt::SkipEmptyParts);
        if (!parts.isEmpty()) {
            packages.append(parts[0]);
        }
    }

    return packages;
}

QString ApplicationScanner::createPackageListFile(const QStringList &packages, const QString &managerName)
{
    // Create temporary directory
    QString tempDir = QStandardPaths::writableLocation(QStandardPaths::TempLocation);
    QString fileName = tempDir + "/transporti_installed_packages.json";

    // Create JSON structure
    QJsonObject root;
    root["package_manager"] = managerName;
    root["package_count"] = packages.size();
    root["scan_date"] = QDateTime::currentDateTime().toString(Qt::ISODate);

    QJsonArray packageArray;
    for (const QString &pkg : packages) {
        packageArray.append(pkg);
    }
    root["packages"] = packageArray;

    // Write to file
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "ApplicationScanner: Failed to create file:" << fileName;
        return QString();
    }

    QJsonDocument doc(root);
    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();

    return fileName;
}
