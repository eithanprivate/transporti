#include "BrowserScanner.h"
#include <QDir>
#include <QDirIterator>
#include <QFileInfo>
#include <QtConcurrent>
#include <QDebug>

BrowserScanner::BrowserScanner(QObject *parent)
    : ScannerBase(parent)
    , m_cancelled(0)
{
}

void BrowserScanner::startScan()
{
    qDebug() << "BrowserScanner: Starting scan...";
    m_cancelled.storeRelaxed(0);

    // Run scan in separate thread
    QtConcurrent::run([this]() {
        QList<FileInfo> allFiles;
        qint64 totalSize = 0;

        // Scan Firefox
        scanFirefox(allFiles);

        if (m_cancelled.loadRelaxed()) {
            emit scanError("Scan cancelled by user");
            return;
        }

        // Scan Chrome
        scanChrome(allFiles);

        if (m_cancelled.loadRelaxed()) {
            emit scanError("Scan cancelled by user");
            return;
        }

        // Scan Chromium
        scanChromium(allFiles);

        if (m_cancelled.loadRelaxed()) {
            emit scanError("Scan cancelled by user");
            return;
        }

        // Calculate total size
        for (const FileInfo &file : allFiles) {
            totalSize += file.size;
        }

        qDebug() << "BrowserScanner: Scan complete -" << allFiles.size() << "files," << totalSize << "bytes";
        emit scanComplete(allFiles, totalSize);
    });
}

void BrowserScanner::cancelScan()
{
    qDebug() << "BrowserScanner: Cancelling scan...";
    m_cancelled.storeRelaxed(1);
}

void BrowserScanner::scanFirefox(QList<FileInfo> &files)
{
    QString home = QDir::homePath();
    QString firefoxPath = home + "/.mozilla/firefox";

    if (QDir(firefoxPath).exists()) {
        qDebug() << "BrowserScanner: Scanning Firefox at" << firefoxPath;
        scanBrowserDirectory(firefoxPath, "Browser", files);
    } else {
        qDebug() << "BrowserScanner: Firefox directory not found";
    }
}

void BrowserScanner::scanChrome(QList<FileInfo> &files)
{
    QString home = QDir::homePath();
    QString chromePath = home + "/.config/google-chrome";

    if (QDir(chromePath).exists()) {
        qDebug() << "BrowserScanner: Scanning Chrome at" << chromePath;
        scanBrowserDirectory(chromePath, "Browser", files);
    } else {
        qDebug() << "BrowserScanner: Chrome directory not found";
    }
}

void BrowserScanner::scanChromium(QList<FileInfo> &files)
{
    QString home = QDir::homePath();
    QString chromiumPath = home + "/.config/chromium";

    if (QDir(chromiumPath).exists()) {
        qDebug() << "BrowserScanner: Scanning Chromium at" << chromiumPath;
        scanBrowserDirectory(chromiumPath, "Browser", files);
    } else {
        qDebug() << "BrowserScanner: Chromium directory not found";
    }
}

void BrowserScanner::scanBrowserDirectory(const QString &path, const QString &browserName, QList<FileInfo> &files)
{
    QDirIterator it(path, QDir::Files | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);

    int count = 0;
    while (it.hasNext()) {
        if (m_cancelled.loadRelaxed()) {
            return;
        }

        QString filePath = it.next();

        // Skip cache and temp directories
        if (filePath.contains("/Cache/", Qt::CaseInsensitive) ||
            filePath.contains("/cache/", Qt::CaseInsensitive) ||
            filePath.contains("/GPUCache/", Qt::CaseInsensitive)) {
            continue;
        }

        // Focus on important files: bookmarks, history, passwords, etc.
        QString fileName = QFileInfo(filePath).fileName();
        if (fileName.contains("Bookmarks", Qt::CaseInsensitive) ||
            fileName.contains("History", Qt::CaseInsensitive) ||
            fileName.contains("places.sqlite", Qt::CaseInsensitive) ||
            fileName.contains("cookies", Qt::CaseInsensitive) ||
            fileName.contains("logins", Qt::CaseInsensitive) ||
            fileName.contains("key", Qt::CaseInsensitive)) {

            QFileInfo fileInfo(filePath);

            FileInfo info;
            info.path = filePath;
            info.size = fileInfo.size();
            info.category = browserName;
            files.append(info);

            count++;
            if (count % 10 == 0) {
                emit scanProgress(count, count);
            }
        }
    }

    qDebug() << "BrowserScanner: Found" << count << "browser files in" << path;
}
