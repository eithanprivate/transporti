#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QObject>

class NetworkManager : public QObject
{
    Q_OBJECT

public:
    explicit NetworkManager(QObject *parent = nullptr);
    ~NetworkManager();

    QString getLocalIPAddress();
    quint16 getAvailablePort();

private:
    // Network utilities
};

#endif // NETWORKMANAGER_H
