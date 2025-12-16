#include "TransferClient.h"
#include <QDebug>

TransferClient::TransferClient(QObject *parent)
    : QObject(parent)
    , m_socket(new QTcpSocket(this))
    , m_transferActive(false)
{
}

TransferClient::~TransferClient()
{
}

void TransferClient::connectToPeer(const QString &ip, quint16 port)
{
    qDebug() << "Connecting to peer:" << ip << ":" << port;
    m_socket->connectToHost(ip, port);
}

void TransferClient::startTransfer(const QList<FileInfo> &files)
{
    qDebug() << "Starting transfer of" << files.count() << "files";
    m_transferActive = true;
    // Transfer logic will be implemented later
    emit transferComplete();
}

void TransferClient::cancelTransfer()
{
    qDebug() << "Cancelling transfer";
    m_transferActive = false;
}
