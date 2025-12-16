#include "TransferProgressModel.h"
#include <QDateTime>
#include <QtMath>

TransferProgressModel::TransferProgressModel(QObject *parent)
    : QObject(parent)
{
}

double TransferProgressModel::progress() const
{
    if (m_totalBytes == 0)
        return 0.0;
    return static_cast<double>(m_bytesSent) / static_cast<double>(m_totalBytes);
}

void TransferProgressModel::updateProgress(qint64 bytesSent, qint64 totalBytes)
{
    m_bytesSent = bytesSent;
    m_totalBytes = totalBytes;

    calculateSpeed();

    emit progressChanged();
}

void TransferProgressModel::setCurrentFile(const QString &filename)
{
    if (m_currentFile != filename) {
        m_currentFile = filename;
        emit currentFileChanged();
    }
}

void TransferProgressModel::reset()
{
    m_bytesSent = 0;
    m_totalBytes = 0;
    m_speed = 0.0;
    m_currentFile.clear();
    m_eta = 0;
    m_lastBytes = 0;
    m_lastTime = 0;

    emit progressChanged();
    emit speedChanged();
    emit currentFileChanged();
    emit etaChanged();
}

void TransferProgressModel::calculateSpeed()
{
    qint64 currentTime = QDateTime::currentMSecsSinceEpoch();

    if (m_lastTime > 0) {
        qint64 timeDiff = currentTime - m_lastTime;
        if (timeDiff > 0) {
            qint64 bytesDiff = m_bytesSent - m_lastBytes;
            // Speed in MB/s
            m_speed = (bytesDiff / 1024.0 / 1024.0) / (timeDiff / 1000.0);

            // Calculate ETA
            if (m_speed > 0) {
                qint64 remainingBytes = m_totalBytes - m_bytesSent;
                m_eta = qCeil((remainingBytes / 1024.0 / 1024.0) / m_speed);
                emit etaChanged();
            }

            emit speedChanged();
        }
    }

    m_lastBytes = m_bytesSent;
    m_lastTime = currentTime;
}
