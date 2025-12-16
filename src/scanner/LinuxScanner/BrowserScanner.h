#ifndef BROWSERSCANNER_H
#define BROWSERSCANNER_H

#include "../ScannerBase.h"

class BrowserScanner : public ScannerBase
{
    Q_OBJECT
public:
    explicit BrowserScanner(QObject *parent = nullptr);
    void startScan() override;
    void cancelScan() override;
};

#endif
