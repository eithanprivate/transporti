#ifndef TRANSFERCLIENT_H
#define TRANSFERCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include "../../models/DataCategoryModel.h"

class TransferClient : public QObject
{
    Q_OBJECT

public:
    explicit TransferClient(QObject *parent = nullptr);
    ~TransferClient();

    void connectToPeer(const QString &ip, quint16 port);
    void startTransfer(const QList<FileInfo> &files);
    void cancelTransfer();

signals:
    void connected();
    void transferProgress(qint64 bytesSent, qint64 totalBytes);
    void transferComplete();
    void error(const QString &message);

private:
    QTcpSocket *m_socket;
    bool m_transferActive;
};

#endif // TRANSFERCLIENT_H
