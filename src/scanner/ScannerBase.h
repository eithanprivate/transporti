#ifndef SCANNERBASE_H
#define SCANNERBASE_H

#include <QObject>
#include "../models/DataCategoryModel.h"

class ScannerBase : public QObject
{
    Q_OBJECT

public:
    explicit ScannerBase(QObject *parent = nullptr);
    virtual ~ScannerBase() {}

    virtual void startScan() = 0;
    virtual void cancelScan() = 0;

signals:
    void scanProgress(int current, int total);
    void scanComplete(QList<FileInfo> files, qint64 totalSize);
    void scanError(const QString &error);
};

#endif // SCANNERBASE_H
