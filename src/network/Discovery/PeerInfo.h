#ifndef PEERINFO_H
#define PEERINFO_H

#include <QString>
#include <QMetaType>
#include <QJsonObject>

struct PeerInfo {
    QString id;
    QString name;
    QString os;
    QString ip;
    quint16 port;
    qint64 lastSeen;

    PeerInfo() : port(0), lastSeen(0) {}

    PeerInfo(const QString &id, const QString &name, const QString &os,
             const QString &ip, quint16 port)
        : id(id), name(name), os(os), ip(ip), port(port), lastSeen(0) {}

    QJsonObject toJson() const;
    static PeerInfo fromJson(const QJsonObject &json);
};

Q_DECLARE_METATYPE(PeerInfo)

#endif // PEERINFO_H
