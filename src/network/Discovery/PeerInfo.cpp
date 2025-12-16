#include "PeerInfo.h"

QJsonObject PeerInfo::toJson() const
{
    QJsonObject json;
    json["id"] = id;
    json["name"] = name;
    json["os"] = os;
    json["ip"] = ip;
    json["port"] = port;
    return json;
}

PeerInfo PeerInfo::fromJson(const QJsonObject &json)
{
    PeerInfo peer;
    peer.id = json["id"].toString();
    peer.name = json["name"].toString();
    peer.os = json["os"].toString();
    peer.ip = json["ip"].toString();
    peer.port = json["port"].toInt();
    return peer;
}
