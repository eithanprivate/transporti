#include "EmailScanner.h"
#include <QDir>
#include <QDirIterator>
#include <QFileInfo>
#include <QtConcurrent>
#include <QDebug>

EmailScanner::EmailScanner(QObject *parent)
    : ScannerBase(parent)
    , m_cancelled(0)
    , m_totalFiles(0)
    , m_scannedFiles(0)
{
}

void EmailScanner::startScan()
{
    qDebug() << "EmailScanner: Starting scan...";
    m_cancelled.storeRelaxed(0);
    m_scannedFiles = 0;
    m_totalFiles = 0;

    // Run scan in separate thread
    QtConcurrent::run([this]() {
        QList<FileInfo> allFiles;
        qint64 totalSize = 0;

        // Scan Thunderbird
        scanThunderbird(allFiles);

        if (m_cancelled.loadRelaxed()) {
            emit scanError("Scan cancelled by user");
            return;
        }

        // Scan Evolution
        scanEvolution(allFiles);

        if (m_cancelled.loadRelaxed()) {
            emit scanError("Scan cancelled by user");
            return;
        }

        // Calculate total size
        for (const FileInfo &file : allFiles) {
            totalSize += file.size;
        }

        qDebug() << "EmailScanner: Scan complete -" << allFiles.size() << "files," << totalSize << "bytes";
        emit scanComplete(allFiles, totalSize);
    });
}

void EmailScanner::cancelScan()
{
    qDebug() << "EmailScanner: Cancelling scan...";
    m_cancelled.storeRelaxed(1);
}

void EmailScanner::scanThunderbird(QList<FileInfo> &files)
{
    QString home = QDir::homePath();
    QString thunderbirdPath = home + "/.thunderbird";

    if (QDir(thunderbirdPath).exists()) {
        qDebug() << "EmailScanner: Scanning Thunderbird at" << thunderbirdPath;
        scanDirectory(thunderbirdPath, "Email", files);
    } else {
        qDebug() << "EmailScanner: Thunderbird directory not found";
    }
}

void EmailScanner::scanEvolution(QList<FileInfo> &files)
{
    QString home = QDir::homePath();
    QString evolutionPath = home + "/.local/share/evolution";

    if (QDir(evolutionPath).exists()) {
        qDebug() << "EmailScanner: Scanning Evolution at" << evolutionPath;
        scanDirectory(evolutionPath, "Email", files);
    } else {
        qDebug() << "EmailScanner: Evolution directory not found";
    }
}

void EmailScanner::scanDirectory(const QString &path, const QString &category, QList<FileInfo> &files)
{
    QDirIterator it(path, QDir::Files | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);

    while (it.hasNext()) {
        if (m_cancelled.loadRelaxed()) {
            return;
        }

        QString filePath = it.next();

        // Skip cache directories
        if (filePath.contains("/cache/", Qt::CaseInsensitive) ||
            filePath.contains("/Cache/", Qt::CaseInsensitive)) {
            continue;
        }

        QFileInfo fileInfo(filePath);

        FileInfo info;
        info.path = filePath;
        info.size = fileInfo.size();
        info.category = category;
        files.append(info);

        m_scannedFiles++;

        // Emit progress every 10 files
        if (m_scannedFiles % 10 == 0) {
            emit scanProgress(m_scannedFiles, m_totalFiles > 0 ? m_totalFiles : m_scannedFiles);
        }
    }
}
