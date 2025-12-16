#ifndef APPLICATION_H
#define APPLICATION_H

#include <QObject>
#include <QQmlEngine>
#include "ApplicationState.h"

// Forward declarations
class PeerListModel;
class DataCategoryModel;
class TransferProgressModel;
class NetworkManager;
class DiscoveryService;
class TransferClient;
class TransferServer;
class LinuxDataScanner;

class Application : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(ApplicationState* appState READ appState CONSTANT)

public:
    explicit Application(QObject *parent = nullptr);
    ~Application();

    ApplicationState* appState() const { return m_appState; }
    PeerListModel* peerModel() const { return m_peerModel; }
    DataCategoryModel* categoryModel() const { return m_categoryModel; }
    TransferProgressModel* progressModel() const { return m_progressModel; }

    Q_INVOKABLE void setRole(ApplicationState::AppRole role);
    Q_INVOKABLE void selectPeer(const QString &peerId);
    Q_INVOKABLE void startScanning();
    Q_INVOKABLE void startTransfer();
    Q_INVOKABLE void cancelTransfer();

signals:
    void transferStarted();
    void transferCompleted();
    void error(const QString &message);

private slots:
    void onPeerDiscovered();
    void onPairingCompleted();
    void onScanningCompleted();
    void onTransferProgress(qint64 bytesSent, qint64 totalBytes);

private:
    ApplicationState *m_appState;
    PeerListModel *m_peerModel;
    DataCategoryModel *m_categoryModel;
    TransferProgressModel *m_progressModel;

    NetworkManager *m_networkManager;
    DiscoveryService *m_discoveryService;
    TransferClient *m_transferClient;
    TransferServer *m_transferServer;
    LinuxDataScanner *m_scanner;
};

#endif // APPLICATION_H
