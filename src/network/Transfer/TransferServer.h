#ifndef TRANSFERSERVER_H
#define TRANSFERSERVER_H

#include <QObject>
#include <QTcpServer>

class TransferServer : public QObject
{
    Q_OBJECT

public:
    explicit TransferServer(QObject *parent = nullptr);
    ~TransferServer();

    void startListening(quint16 port = 47624);
    void stopListening();

signals:
    void transferStarted();
    void transferProgress(qint64 bytesReceived, qint64 totalBytes);
    void transferComplete();
    void error(const QString &message);

private:
    QTcpServer *m_server;
};

#endif // TRANSFERSERVER_H
