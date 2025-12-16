#include "TransferServer.h"
#include <QDebug>

TransferServer::TransferServer(QObject *parent)
    : QObject(parent)
    , m_server(new QTcpServer(this))
{
}

TransferServer::~TransferServer()
{
}

void TransferServer::startListening(quint16 port)
{
    qDebug() << "Starting transfer server on port" << port;
    m_server->listen(QHostAddress::Any, port);
}

void TransferServer::stopListening()
{
    qDebug() << "Stopping transfer server";
    m_server->close();
}
