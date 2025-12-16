#include "PeerListModel.h"

PeerListModel::PeerListModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

int PeerListModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return m_peers.count();
}

QVariant PeerListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_peers.count())
        return QVariant();

    const PeerInfo &peer = m_peers.at(index.row());

    switch (role) {
    case DeviceIdRole:
        return peer.id;
    case DeviceNameRole:
        return peer.name;
    case DeviceOSRole:
        return peer.os;
    case DeviceIPRole:
        return peer.ip;
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> PeerListModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[DeviceIdRole] = "deviceId";
    roles[DeviceNameRole] = "deviceName";
    roles[DeviceOSRole] = "deviceOS";
    roles[DeviceIPRole] = "deviceIP";
    return roles;
}

void PeerListModel::addPeer(const PeerInfo &peer)
{
    // Check if peer already exists
    for (int i = 0; i < m_peers.count(); ++i) {
        if (m_peers.at(i).id == peer.id) {
            // Update existing peer
            m_peers[i] = peer;
            emit dataChanged(index(i), index(i));
            return;
        }
    }

    // Add new peer
    beginInsertRows(QModelIndex(), m_peers.count(), m_peers.count());
    m_peers.append(peer);
    endInsertRows();
}

void PeerListModel::removePeer(const QString &peerId)
{
    for (int i = 0; i < m_peers.count(); ++i) {
        if (m_peers.at(i).id == peerId) {
            beginRemoveRows(QModelIndex(), i, i);
            m_peers.removeAt(i);
            endRemoveRows();
            return;
        }
    }
}

void PeerListModel::clear()
{
    beginResetModel();
    m_peers.clear();
    endResetModel();
}
