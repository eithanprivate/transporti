#include "PairingManager.h"
#include <QRandomGenerator>

PairingManager::PairingManager(QObject *parent)
    : QObject(parent)
{
}

QString PairingManager::generatePIN()
{
    int pin = QRandomGenerator::global()->bounded(100000, 999999);
    m_currentPIN = QString::number(pin);
    return m_currentPIN;
}

void PairingManager::verifyPIN(const QString &pin)
{
    if (pin == m_currentPIN) {
        emit pairingAccepted();
    } else {
        emit pairingRejected();
    }
}
