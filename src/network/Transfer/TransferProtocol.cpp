#include "TransferProtocol.h"
#include <QDataStream>

QByteArray TransferProtocol::createPacket(PacketType type, quint64 packetId, const QByteArray &payload)
{
    QByteArray packet;
    QDataStream stream(&packet, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::BigEndian);

    quint32 checksum = calculateChecksum(payload);

    stream << MAGIC;
    stream << PROTOCOL_VERSION;
    stream << static_cast<quint32>(type);
    stream << packetId;
    stream << static_cast<quint64>(payload.size());
    stream << checksum;
    packet.append(payload);

    return packet;
}

bool TransferProtocol::parsePacket(const QByteArray &data, PacketHeader &header, QByteArray &payload)
{
    if (data.size() < static_cast<int>(sizeof(PacketHeader)))
        return false;

    QDataStream stream(data);
    stream.setByteOrder(QDataStream::BigEndian);

    stream >> header.magic;
    stream >> header.version;
    stream >> header.type;
    stream >> header.packetId;
    stream >> header.payloadSize;
    stream >> header.checksum;

    if (header.magic != MAGIC)
        return false;

    payload = data.mid(sizeof(PacketHeader), header.payloadSize);

    return calculateChecksum(payload) == header.checksum;
}

quint32 TransferProtocol::calculateChecksum(const QByteArray &data)
{
    // Simple CRC32 implementation (placeholder)
    quint32 crc = 0;
    for (char byte : data) {
        crc ^= static_cast<quint8>(byte);
    }
    return crc;
}
