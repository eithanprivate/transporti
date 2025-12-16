#include "ConfigScanner.h"
#include <QDir>
#include <QDirIterator>
#include <QFileInfo>
#include <QtConcurrent>
#include <QDebug>

ConfigScanner::ConfigScanner(QObject *parent)
    : ScannerBase(parent)
    , m_cancelled(0)
    , m_totalFiles(0)
    , m_scannedFiles(0)
{
    // Whitelist of common applications to preserve configs
    m_whitelistedApps << "firefox" << "mozilla" << "google-chrome" << "chromium"
                      << "thunderbird" << "evolution" << "libreoffice"
                      << "Code" << "VSCodium" << "sublime-text" << "gedit"
                      << "kate" << "nautilus" << "dolphin" << "plasma"
                      << "gtk-3.0" << "gtk-4.0" << "dconf" << "gconf"
                      << "systemd" << "pulse" << "pipewire" << "fontconfig";
}

void ConfigScanner::startScan()
{
    qDebug() << "ConfigScanner: Starting scan...";
    m_cancelled.storeRelaxed(0);
    m_scannedFiles = 0;
    m_totalFiles = 0;

    // Run scan in separate thread
    QtConcurrent::run([this]() {
        QList<FileInfo> allFiles;
        qint64 totalSize = 0;

        QStringList configDirs = getConfigDirectories();

        // First pass: count files
        for (const QString &dir : configDirs) {
            if (m_cancelled.loadRelaxed()) {
                return;
            }

            QDirIterator it(dir, QDir::Files | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
            while (it.hasNext()) {
                it.next();
                QString filePath = it.filePath();

                // Skip cache and temporary directories
                if (filePath.contains("/cache/", Qt::CaseInsensitive) ||
                    filePath.contains("/Cache/", Qt::CaseInsensitive) ||
                    filePath.contains("/tmp/", Qt::CaseInsensitive) ||
                    filePath.contains("/.cache/", Qt::CaseInsensitive)) {
                    continue;
                }

                m_totalFiles++;

                if (m_cancelled.loadRelaxed()) {
                    return;
                }
            }
        }

        qDebug() << "ConfigScanner: Found" << m_totalFiles << "config files";

        // Second pass: collect file info
        for (const QString &dir : configDirs) {
            if (m_cancelled.loadRelaxed()) {
                emit scanError("Scan cancelled by user");
                return;
            }

            scanConfigDirectory(dir, allFiles);
        }

        if (!m_cancelled.loadRelaxed()) {
            // Calculate total size
            for (const FileInfo &file : allFiles) {
                totalSize += file.size;
            }

            qDebug() << "ConfigScanner: Scan complete -" << allFiles.size() << "files," << totalSize << "bytes";
            emit scanComplete(allFiles, totalSize);
        }
    });
}

void ConfigScanner::cancelScan()
{
    qDebug() << "ConfigScanner: Cancelling scan...";
    m_cancelled.storeRelaxed(1);
}

void ConfigScanner::scanConfigDirectory(const QString &basePath, QList<FileInfo> &files)
{
    QDirIterator it(basePath, QDir::Files | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);

    while (it.hasNext()) {
        if (m_cancelled.loadRelaxed()) {
            return;
        }

        QString filePath = it.next();

        // Skip cache and temporary files
        if (filePath.contains("/cache/", Qt::CaseInsensitive) ||
            filePath.contains("/Cache/", Qt::CaseInsensitive) ||
            filePath.contains("/tmp/", Qt::CaseInsensitive) ||
            filePath.contains("/.cache/", Qt::CaseInsensitive)) {
            continue;
        }

        QFileInfo fileInfo(filePath);

        FileInfo info;
        info.path = filePath;
        info.size = fileInfo.size();
        info.category = "Configurations";
        files.append(info);

        m_scannedFiles++;

        // Emit progress every 10 files
        if (m_scannedFiles % 10 == 0 && m_totalFiles > 0) {
            emit scanProgress(m_scannedFiles, m_totalFiles);
        }
    }
}

bool ConfigScanner::isValidConfigDirectory(const QString &dirName) const
{
    return m_whitelistedApps.contains(dirName, Qt::CaseInsensitive);
}

QStringList ConfigScanner::getConfigDirectories() const
{
    QStringList dirs;
    QString home = QDir::homePath();
    QString configPath = home + "/.config";

    // Check if .config exists
    if (!QDir(configPath).exists()) {
        qDebug() << "ConfigScanner: .config directory not found";
        return dirs;
    }

    // Scan for whitelisted app directories
    QDir configDir(configPath);
    QStringList entries = configDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);

    for (const QString &entry : entries) {
        if (isValidConfigDirectory(entry)) {
            QString fullPath = configPath + "/" + entry;
            dirs << fullPath;
            qDebug() << "ConfigScanner: Will scan" << fullPath;
        }
    }

    // Also check for ~/.mozilla (Firefox)
    QString mozillaPath = home + "/.mozilla";
    if (QDir(mozillaPath).exists()) {
        dirs << mozillaPath;
        qDebug() << "ConfigScanner: Will scan" << mozillaPath;
    }

    // Check for ~/.thunderbird
    QString thunderbirdPath = home + "/.thunderbird";
    if (QDir(thunderbirdPath).exists()) {
        dirs << thunderbirdPath;
        qDebug() << "ConfigScanner: Will scan" << thunderbirdPath;
    }

    return dirs;
}
