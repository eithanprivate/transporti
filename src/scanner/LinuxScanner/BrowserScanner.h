#ifndef BROWSERSCANNER_H
#define BROWSERSCANNER_H

#include "../ScannerBase.h"
#include <QAtomicInt>
#include <QStringList>

class BrowserScanner : public ScannerBase
{
    Q_OBJECT
public:
    explicit BrowserScanner(QObject *parent = nullptr);
    void startScan() override;
    void cancelScan() override;

private:
    void scanFirefox(QList<FileInfo> &files);
    void scanChrome(QList<FileInfo> &files);
    void scanChromium(QList<FileInfo> &files);
    void scanBrowserDirectory(const QString &path, const QString &browserName, QList<FileInfo> &files);

    QAtomicInt m_cancelled;
};

#endif
