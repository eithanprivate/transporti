#ifndef DISCOVERYSERVICE_H
#define DISCOVERYSERVICE_H

#include <QObject>
#include <QUdpSocket>
#include <QTimer>
#include <QMap>
#include "PeerInfo.h"

class DiscoveryService : public QObject
{
    Q_OBJECT

public:
    enum Role { Source, Destination };

    explicit DiscoveryService(Role role, QObject *parent = nullptr);
    ~DiscoveryService();

    void startDiscovery();
    void stopDiscovery();
    void sendPairingRequest(const QString &peerId, const QString &peerIp, const QString &pin);

signals:
    void peerDiscovered(const PeerInfo &peer);
    void peerLost(const QString &peerId);
    void pairingRequestReceived(const QString &peerId, const QString &peerName, const QString &pin);

private slots:
    void sendBroadcast();
    void processDatagram();
    void checkPeerTimeouts();

private:
    QUdpSocket *m_udpSocket;
    QTimer *m_broadcastTimer;
    QTimer *m_timeoutTimer;
    Role m_deviceRole;
    QString m_deviceId;
    QMap<QString, PeerInfo> m_discoveredPeers;

    static const quint16 DISCOVERY_PORT = 47623;
    static const int BROADCAST_INTERVAL = 2000;  // 2 seconds
    static const int PEER_TIMEOUT = 10000;       // 10 seconds
};

#endif // DISCOVERYSERVICE_H
