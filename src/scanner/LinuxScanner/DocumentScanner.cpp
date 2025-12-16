#include "DocumentScanner.h"
#include <QDir>
#include <QDirIterator>
#include <QStandardPaths>
#include <QFileInfo>
#include <QtConcurrent>
#include <QDebug>

DocumentScanner::DocumentScanner(QObject *parent)
    : ScannerBase(parent)
    , m_cancelled(0)
    , m_totalFiles(0)
    , m_scannedFiles(0)
{
    // Common document extensions
    m_documentExtensions << "pdf" << "doc" << "docx" << "odt" << "txt" << "rtf"
                         << "xls" << "xlsx" << "ods" << "csv"
                         << "ppt" << "pptx" << "odp"
                         << "jpg" << "jpeg" << "png" << "gif" << "bmp" << "svg" << "webp"
                         << "mp4" << "avi" << "mkv" << "mov" << "wmv" << "flv" << "webm"
                         << "mp3" << "wav" << "flac" << "ogg" << "m4a" << "aac"
                         << "zip" << "rar" << "7z" << "tar" << "gz" << "bz2"
                         << "html" << "xml" << "json" << "md" << "tex";
}

void DocumentScanner::startScan()
{
    qDebug() << "DocumentScanner: Starting scan...";
    m_cancelled.store(0);
    m_scannedFiles = 0;
    m_totalFiles = 0;

    // Run scan in separate thread
    QtConcurrent::run([this]() {
        QList<FileInfo> allFiles;
        qint64 totalSize = 0;

        QStringList directories = getDocumentDirectories();

        // First pass: count total files for progress reporting
        for (const QString &dir : directories) {
            if (m_cancelled.load()) {
                return;
            }

            QDirIterator it(dir, QDir::Files | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
            while (it.hasNext()) {
                it.next();
                if (isDocumentFile(it.fileName())) {
                    m_totalFiles++;
                }
                if (m_cancelled.load()) {
                    return;
                }
            }
        }

        qDebug() << "DocumentScanner: Found" << m_totalFiles << "potential files";

        // Second pass: collect file info
        for (const QString &dir : directories) {
            if (m_cancelled.load()) {
                emit scanError("Scan cancelled by user");
                return;
            }

            scanDirectory(dir, allFiles);
        }

        if (!m_cancelled.load()) {
            // Calculate total size
            for (const FileInfo &file : allFiles) {
                totalSize += file.size;
            }

            qDebug() << "DocumentScanner: Scan complete -" << allFiles.size() << "files," << totalSize << "bytes";
            emit scanComplete(allFiles, totalSize);
        }
    });
}

void DocumentScanner::cancelScan()
{
    qDebug() << "DocumentScanner: Cancelling scan...";
    m_cancelled.store(1);
}

void DocumentScanner::scanDirectory(const QString &path, QList<FileInfo> &files)
{
    QDirIterator it(path, QDir::Files | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);

    while (it.hasNext()) {
        if (m_cancelled.load()) {
            return;
        }

        QString filePath = it.next();
        QFileInfo fileInfo(filePath);

        if (isDocumentFile(fileInfo.fileName())) {
            FileInfo info;
            info.path = filePath;
            info.size = fileInfo.size();
            info.category = "Documents";
            files.append(info);

            m_scannedFiles++;

            // Emit progress every 10 files
            if (m_scannedFiles % 10 == 0 && m_totalFiles > 0) {
                emit scanProgress(m_scannedFiles, m_totalFiles);
            }
        }
    }
}

bool DocumentScanner::isDocumentFile(const QString &fileName) const
{
    QString suffix = QFileInfo(fileName).suffix().toLower();
    return m_documentExtensions.contains(suffix);
}

QStringList DocumentScanner::getDocumentDirectories() const
{
    QStringList dirs;

    // Standard user directories
    QString home = QDir::homePath();

    QStringList standardDirs;
    standardDirs << "Documents" << "Downloads" << "Desktop"
                 << "Pictures" << "Videos" << "Music";

    for (const QString &dirName : standardDirs) {
        QString dirPath = home + "/" + dirName;
        if (QDir(dirPath).exists()) {
            dirs << dirPath;
            qDebug() << "DocumentScanner: Will scan" << dirPath;
        }
    }

    // Also check XDG standard paths
    QStringList xdgDocs = QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation);
    QStringList xdgDownloads = QStandardPaths::standardLocations(QStandardPaths::DownloadLocation);
    QStringList xdgPictures = QStandardPaths::standardLocations(QStandardPaths::PicturesLocation);
    QStringList xdgVideos = QStandardPaths::standardLocations(QStandardPaths::MoviesLocation);
    QStringList xdgMusic = QStandardPaths::standardLocations(QStandardPaths::MusicLocation);

    for (const QString &dir : xdgDocs + xdgDownloads + xdgPictures + xdgVideos + xdgMusic) {
        if (!dirs.contains(dir) && QDir(dir).exists()) {
            dirs << dir;
            qDebug() << "DocumentScanner: Will scan (XDG)" << dir;
        }
    }

    return dirs;
}
