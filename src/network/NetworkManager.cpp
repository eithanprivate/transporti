#include "NetworkManager.h"
#include <QNetworkInterface>
#include <QHostInfo>

NetworkManager::NetworkManager(QObject *parent)
    : QObject(parent)
{
}

NetworkManager::~NetworkManager()
{
}

QString NetworkManager::getLocalIPAddress()
{
    // Get local IP address (prefer non-loopback IPv4)
    const QList<QHostAddress> addresses = QNetworkInterface::allAddresses();
    for (const QHostAddress &address : addresses) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol &&
            !address.isLoopback()) {
            return address.toString();
        }
    }
    return "127.0.0.1";
}

quint16 NetworkManager::getAvailablePort()
{
    // For now, return default transfer port
    return 47624;
}
