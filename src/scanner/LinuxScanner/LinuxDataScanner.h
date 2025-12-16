#ifndef LINUXDATASCANNER_H
#define LINUXDATASCANNER_H

#include "../ScannerBase.h"

class LinuxDataScanner : public ScannerBase
{
    Q_OBJECT

public:
    explicit LinuxDataScanner(QObject *parent = nullptr);

    void startScan() override;
    void cancelScan() override;

signals:
    void scanComplete();

private:
    bool m_cancelled;
};

#endif // LINUXDATASCANNER_H
