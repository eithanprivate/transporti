#include "LinuxDataScanner.h"
#include <QDebug>

LinuxDataScanner::LinuxDataScanner(QObject *parent)
    : ScannerBase(parent)
    , m_cancelled(false)
{
}

void LinuxDataScanner::startScan()
{
    qDebug() << "Starting Linux data scan...";
    m_cancelled = false;

    // Placeholder - actual scanning logic will be implemented later
    QList<FileInfo> files;
    emit scanComplete();
}

void LinuxDataScanner::cancelScan()
{
    qDebug() << "Cancelling scan...";
    m_cancelled = true;
}
