#ifndef APPLICATIONSCANNER_H
#define APPLICATIONSCANNER_H

#include "../ScannerBase.h"
#include <QAtomicInt>
#include <QProcess>

class ApplicationScanner : public ScannerBase
{
    Q_OBJECT
public:
    explicit ApplicationScanner(QObject *parent = nullptr);
    void startScan() override;
    void cancelScan() override;

private:
    enum PackageManager {
        Unknown,
        Dpkg,      // Debian/Ubuntu
        Rpm,       // Fedora/RedHat/CentOS
        Pacman     // Arch Linux
    };

    PackageManager detectPackageManager();
    QStringList scanDpkgPackages();
    QStringList scanRpmPackages();
    QStringList scanPacmanPackages();
    QString createPackageListFile(const QStringList &packages, const QString &managerName);

    QAtomicInt m_cancelled;
};

#endif
