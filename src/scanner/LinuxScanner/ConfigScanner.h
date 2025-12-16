#ifndef CONFIGSCANNER_H
#define CONFIGSCANNER_H

#include "../ScannerBase.h"
#include <QStringList>
#include <QAtomicInt>

class ConfigScanner : public ScannerBase
{
    Q_OBJECT
public:
    explicit ConfigScanner(QObject *parent = nullptr);
    void startScan() override;
    void cancelScan() override;

private:
    void scanConfigDirectory(const QString &basePath, QList<FileInfo> &files);
    bool isValidConfigDirectory(const QString &dirName) const;
    QStringList getConfigDirectories() const;

    QAtomicInt m_cancelled;
    QStringList m_whitelistedApps;
    int m_totalFiles;
    int m_scannedFiles;
};

#endif
