#ifndef EMAILSCANNER_H
#define EMAILSCANNER_H

#include "../ScannerBase.h"
#include <QAtomicInt>
#include <QStringList>

class EmailScanner : public ScannerBase
{
    Q_OBJECT
public:
    explicit EmailScanner(QObject *parent = nullptr);
    void startScan() override;
    void cancelScan() override;

private:
    void scanThunderbird(QList<FileInfo> &files);
    void scanEvolution(QList<FileInfo> &files);
    void scanDirectory(const QString &path, const QString &category, QList<FileInfo> &files);

    QAtomicInt m_cancelled;
    int m_totalFiles;
    int m_scannedFiles;
};

#endif
