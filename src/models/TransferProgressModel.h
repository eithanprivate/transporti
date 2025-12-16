#ifndef TRANSFERPROGRESSMODEL_H
#define TRANSFERPROGRESSMODEL_H

#include <QObject>

class TransferProgressModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(qint64 bytesSent READ bytesSent NOTIFY progressChanged)
    Q_PROPERTY(qint64 totalBytes READ totalBytes NOTIFY progressChanged)
    Q_PROPERTY(double progress READ progress NOTIFY progressChanged)
    Q_PROPERTY(double speed READ speed NOTIFY speedChanged)
    Q_PROPERTY(QString currentFile READ currentFile NOTIFY currentFileChanged)
    Q_PROPERTY(int eta READ eta NOTIFY etaChanged)

public:
    explicit TransferProgressModel(QObject *parent = nullptr);

    qint64 bytesSent() const { return m_bytesSent; }
    qint64 totalBytes() const { return m_totalBytes; }
    double progress() const;
    double speed() const { return m_speed; }
    QString currentFile() const { return m_currentFile; }
    int eta() const { return m_eta; }

    void updateProgress(qint64 bytesSent, qint64 totalBytes);
    void setCurrentFile(const QString &filename);
    void reset();

signals:
    void progressChanged();
    void speedChanged();
    void currentFileChanged();
    void etaChanged();

private:
    void calculateSpeed();

    qint64 m_bytesSent = 0;
    qint64 m_totalBytes = 0;
    double m_speed = 0.0;  // MB/s
    QString m_currentFile;
    int m_eta = 0;  // seconds
    qint64 m_lastBytes = 0;
    qint64 m_lastTime = 0;
};

#endif // TRANSFERPROGRESSMODEL_H
