#ifndef PEERLISTMODEL_H
#define PEERLISTMODEL_H

#include <QAbstractListModel>
#include "../network/Discovery/PeerInfo.h"

class PeerListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum PeerRoles {
        DeviceIdRole = Qt::UserRole + 1,
        DeviceNameRole,
        DeviceOSRole,
        DeviceIPRole
    };

    explicit PeerListModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

public slots:
    void addPeer(const PeerInfo &peer);
    void removePeer(const QString &peerId);
    void clear();

private:
    QList<PeerInfo> m_peers;
};

#endif // PEERLISTMODEL_H
