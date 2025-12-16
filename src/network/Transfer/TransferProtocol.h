#ifndef TRANSFERPROTOCOL_H
#define TRANSFERPROTOCOL_H

#include <QByteArray>
#include <QtGlobal>

class TransferProtocol
{
public:
    enum PacketType {
        HANDSHAKE = 0x01,
        PAIRING_REQUEST = 0x02,
        PAIRING_RESPONSE = 0x03,
        MANIFEST = 0x04,
        DATA_BLOCK = 0x05,
        ACK = 0x06,
        ERROR = 0x07,
        COMPLETE = 0x08
    };

    static const quint32 MAGIC = 0x54524E53;  // "TRNS"
    static const quint32 PROTOCOL_VERSION = 1;

    struct PacketHeader {
        quint32 magic;
        quint32 version;
        quint32 type;
        quint64 packetId;
        quint64 payloadSize;
        quint32 checksum;
    };

    static QByteArray createPacket(PacketType type, quint64 packetId, const QByteArray &payload);
    static bool parsePacket(const QByteArray &data, PacketHeader &header, QByteArray &payload);
    static quint32 calculateChecksum(const QByteArray &data);
};

#endif // TRANSFERPROTOCOL_H
