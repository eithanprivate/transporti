#ifndef APPLICATIONSCANNER_H
#define APPLICATIONSCANNER_H

#include "../ScannerBase.h"

class ApplicationScanner : public ScannerBase
{
    Q_OBJECT
public:
    explicit ApplicationScanner(QObject *parent = nullptr);
    void startScan() override;
    void cancelScan() override;
};

#endif
