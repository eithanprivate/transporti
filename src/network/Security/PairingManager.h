#ifndef PAIRINGMANAGER_H
#define PAIRINGMANAGER_H

#include <QObject>

class PairingManager : public QObject
{
    Q_OBJECT

public:
    explicit PairingManager(QObject *parent = nullptr);

    QString generatePIN();
    void verifyPIN(const QString &pin);

signals:
    void pairingAccepted();
    void pairingRejected();

private:
    QString m_currentPIN;
};

#endif // PAIRINGMANAGER_H
