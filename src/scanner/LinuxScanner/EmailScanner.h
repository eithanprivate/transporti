#ifndef EMAILSCANNER_H
#define EMAILSCANNER_H

#include "../ScannerBase.h"

class EmailScanner : public ScannerBase
{
    Q_OBJECT
public:
    explicit EmailScanner(QObject *parent = nullptr);
    void startScan() override;
    void cancelScan() override;
};

#endif
