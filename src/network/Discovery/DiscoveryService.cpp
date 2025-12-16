#include "DiscoveryService.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QHostInfo>
#include <QDateTime>
#include <QUuid>
#include <QNetworkInterface>
#include <QDebug>

DiscoveryService::DiscoveryService(Role role, QObject *parent)
    : QObject(parent)
    , m_udpSocket(new QUdpSocket(this))
    , m_broadcastTimer(new QTimer(this))
    , m_timeoutTimer(new QTimer(this))
    , m_deviceRole(role)
    , m_deviceId(QUuid::createUuid().toString())
{
    // Bind to discovery port
    m_udpSocket->bind(QHostAddress::AnyIPv4, DISCOVERY_PORT, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);

    connect(m_udpSocket, &QUdpSocket::readyRead, this, &DiscoveryService::processDatagram);
    connect(m_broadcastTimer, &QTimer::timeout, this, &DiscoveryService::sendBroadcast);
    connect(m_timeoutTimer, &QTimer::timeout, this, &DiscoveryService::checkPeerTimeouts);

    qDebug() << "DiscoveryService initialized with ID:" << m_deviceId;
}

DiscoveryService::~DiscoveryService()
{
    stopDiscovery();
}

void DiscoveryService::startDiscovery()
{
    qDebug() << "Starting discovery service...";
    m_broadcastTimer->start(BROADCAST_INTERVAL);
    m_timeoutTimer->start(PEER_TIMEOUT / 2);
    sendBroadcast();  // Send immediately
}

void DiscoveryService::stopDiscovery()
{
    qDebug() << "Stopping discovery service...";
    m_broadcastTimer->stop();
    m_timeoutTimer->stop();
}

void DiscoveryService::sendBroadcast()
{
    QJsonObject message;
    message["type"] = "TRANSPORTI_DISCOVERY";
    message["version"] = "1.0";
    message["role"] = (m_deviceRole == Source) ? "source" : "destination";

    QJsonObject device;
    device["id"] = m_deviceId;
    device["name"] = QHostInfo::localHostName();
#ifdef PLATFORM_LINUX
    device["os"] = "linux";
#elif defined(PLATFORM_WINDOWS)
    device["os"] = "windows";
#else
    device["os"] = "unknown";
#endif

    // Get local IP
    const QList<QHostAddress> addresses = QNetworkInterface::allAddresses();
    for (const QHostAddress &address : addresses) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol && !address.isLoopback()) {
            device["ip"] = address.toString();
            break;
        }
    }
    device["port"] = 47624;

    message["device"] = device;
    message["timestamp"] = QDateTime::currentMSecsSinceEpoch();

    QJsonDocument doc(message);
    QByteArray data = doc.toJson(QJsonDocument::Compact);

    m_udpSocket->writeDatagram(data, QHostAddress::Broadcast, DISCOVERY_PORT);
    qDebug() << "Broadcast sent";
}

void DiscoveryService::processDatagram()
{
    while (m_udpSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(m_udpSocket->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;

        m_udpSocket->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);

        QJsonDocument doc = QJsonDocument::fromJson(datagram);
        if (!doc.isObject())
            continue;

        QJsonObject message = doc.object();
        QString messageType = message["type"].toString();

        if (messageType == "PAIRING_REQUEST") {
            // Handle pairing request
            QJsonObject deviceObj = message["device"].toObject();
            QString peerId = deviceObj["id"].toString();
            QString peerName = deviceObj["name"].toString();
            QString pin = message["pin"].toString();

            qDebug() << "Pairing request received from:" << peerName << "PIN:" << pin;
            emit pairingRequestReceived(peerId, peerName, pin);
            continue;
        }

        if (messageType != "TRANSPORTI_DISCOVERY")
            continue;

        QJsonObject deviceObj = message["device"].toObject();
        QString peerId = deviceObj["id"].toString();

        // Ignore our own broadcasts
        if (peerId == m_deviceId)
            continue;

        // Check if roles are compatible (source finds destination, destination finds source)
        QString peerRole = message["role"].toString();
        if ((m_deviceRole == Source && peerRole != "destination") ||
            (m_deviceRole == Destination && peerRole != "source")) {
            continue;
        }

        PeerInfo peer;
        peer.id = peerId;
        peer.name = deviceObj["name"].toString();
        peer.os = deviceObj["os"].toString();
        peer.ip = deviceObj["ip"].toString();
        peer.port = deviceObj["port"].toInt();
        peer.lastSeen = QDateTime::currentMSecsSinceEpoch();

        bool isNewPeer = !m_discoveredPeers.contains(peerId);
        m_discoveredPeers[peerId] = peer;

        if (isNewPeer) {
            qDebug() << "New peer discovered:" << peer.name << "(" << peer.os << ")" << peer.ip;
            emit peerDiscovered(peer);
        } else {
            qDebug() << "Peer updated:" << peer.name;
        }
    }
}

void DiscoveryService::checkPeerTimeouts()
{
    qint64 currentTime = QDateTime::currentMSecsSinceEpoch();
    QList<QString> timedOutPeers;

    for (auto it = m_discoveredPeers.begin(); it != m_discoveredPeers.end(); ++it) {
        if (currentTime - it->lastSeen > PEER_TIMEOUT) {
            timedOutPeers.append(it.key());
        }
    }

    for (const QString &peerId : timedOutPeers) {
        qDebug() << "Peer timed out:" << m_discoveredPeers[peerId].name;
        m_discoveredPeers.remove(peerId);
        emit peerLost(peerId);
    }
}

void DiscoveryService::sendPairingRequest(const QString &peerId, const QString &peerIp, const QString &pin)
{
    qDebug() << "Sending pairing request to" << peerId << "at" << peerIp << "with PIN:" << pin;

    QJsonObject message;
    message["type"] = "PAIRING_REQUEST";
    message["version"] = "1.0";
    message["pin"] = pin;

    QJsonObject device;
    device["id"] = m_deviceId;
    device["name"] = QHostInfo::localHostName();
#ifdef PLATFORM_LINUX
    device["os"] = "linux";
#elif defined(PLATFORM_WINDOWS)
    device["os"] = "windows";
#else
    device["os"] = "unknown";
#endif

    message["device"] = device;
    message["timestamp"] = QDateTime::currentMSecsSinceEpoch();

    QJsonDocument doc(message);
    QByteArray data = doc.toJson(QJsonDocument::Compact);

    // Send to specific peer IP
    QHostAddress peerAddress(peerIp);
    m_udpSocket->writeDatagram(data, peerAddress, DISCOVERY_PORT);

    qDebug() << "Pairing request sent to" << peerIp;
}
