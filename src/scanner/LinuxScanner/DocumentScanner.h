#ifndef DOCUMENTSCANNER_H
#define DOCUMENTSCANNER_H

#include "../ScannerBase.h"
#include <QStringList>
#include <QAtomicInt>

class DocumentScanner : public ScannerBase
{
    Q_OBJECT
public:
    explicit DocumentScanner(QObject *parent = nullptr);
    void startScan() override;
    void cancelScan() override;

private:
    void scanDirectory(const QString &path, QList<FileInfo> &files);
    bool isDocumentFile(const QString &fileName) const;
    QStringList getDocumentDirectories() const;

    QAtomicInt m_cancelled;
    QStringList m_documentExtensions;
    int m_totalFiles;
    int m_scannedFiles;
};

#endif
