#ifndef CONFIGSCANNER_H
#define CONFIGSCANNER_H

#include "../ScannerBase.h"

class ConfigScanner : public ScannerBase
{
    Q_OBJECT
public:
    explicit ConfigScanner(QObject *parent = nullptr);
    void startScan() override;
    void cancelScan() override;
};

#endif
