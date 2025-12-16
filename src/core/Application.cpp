#include "Application.h"
#include "../models/PeerListModel.h"
#include "../models/DataCategoryModel.h"
#include "../models/TransferProgressModel.h"
#include "../network/NetworkManager.h"
#include "../network/Discovery/DiscoveryService.h"
#include "../network/Transfer/TransferClient.h"
#include "../network/Transfer/TransferServer.h"
#include "../network/Security/PairingManager.h"
#include "../scanner/LinuxScanner/LinuxDataScanner.h"
#include <QDebug>

Application::Application(QObject *parent)
    : QObject(parent)
    , m_appState(new ApplicationState(this))
    , m_peerModel(new PeerListModel(this))
    , m_categoryModel(new DataCategoryModel(this))
    , m_progressModel(new TransferProgressModel(this))
    , m_networkManager(new NetworkManager(this))
    , m_discoveryService(nullptr)
    , m_transferClient(nullptr)
    , m_transferServer(nullptr)
    , m_scanner(nullptr)
    , m_pairingManager(new PairingManager(this))
{
    qDebug() << "Transporti Application initialized";
}

Application::~Application()
{
}

void Application::setRole(ApplicationState::AppRole role)
{
    m_appState->setRole(role);

    // Initialize appropriate components based on role
    if (role == ApplicationState::RoleSource) {
        qDebug() << "Initializing as Source (Linux sender)";
        // Initialize discovery service to broadcast
        m_discoveryService = new DiscoveryService(DiscoveryService::Source, this);
        m_transferClient = new TransferClient(this);
        m_scanner = new LinuxDataScanner(this);

        // Connect signals
        connect(m_scanner, &LinuxDataScanner::scanComplete,
                this, &Application::onScanningCompleted);
        connect(m_transferClient, &TransferClient::transferProgress,
                this, &Application::onTransferProgress);
        connect(m_transferClient, &TransferClient::transferComplete,
                this, &Application::transferCompleted);

    } else if (role == ApplicationState::RoleDestination) {
        qDebug() << "Initializing as Destination (Windows receiver)";
        // Initialize discovery service to listen
        m_discoveryService = new DiscoveryService(DiscoveryService::Destination, this);
        m_transferServer = new TransferServer(this);
    }

    if (m_discoveryService) {
        connect(m_discoveryService, &DiscoveryService::peerDiscovered,
                m_peerModel, &PeerListModel::addPeer);
        connect(m_discoveryService, &DiscoveryService::peerLost,
                m_peerModel, &PeerListModel::removePeer);
        connect(m_discoveryService, &DiscoveryService::pairingRequestReceived,
                this, &Application::onPairingRequestReceived);

        m_discoveryService->startDiscovery();
    }

    // Move to discovery stage
    m_appState->setStage(ApplicationState::StageDiscovery);
}

void Application::selectPeer(const QString &peerId)
{
    qDebug() << "Peer selected:" << peerId;
    m_appState->setSelectedPeerId(peerId);

    // Generate PIN
    m_currentPin = m_pairingManager->generatePIN();
    qDebug() << "Generated PIN:" << m_currentPin;
    emit currentPinChanged();

    // Get peer info to send pairing request
    QString peerIp;
    for (int i = 0; i < m_peerModel->rowCount(); i++) {
        QModelIndex index = m_peerModel->index(i);
        QString id = m_peerModel->data(index, m_peerModel->roleNames().key("deviceId")).toString();
        if (id == peerId) {
            peerIp = m_peerModel->data(index, m_peerModel->roleNames().key("deviceIP")).toString();
            break;
        }
    }

    if (!peerIp.isEmpty() && m_discoveryService) {
        qDebug() << "Sending pairing request to" << peerIp;
        m_discoveryService->sendPairingRequest(peerId, peerIp, m_currentPin);
    }

    // Move to pairing stage
    m_appState->setStage(ApplicationState::StagePairing);
}

void Application::startScanning()
{
    qDebug() << "Starting data scanning...";
    m_appState->setStage(ApplicationState::StageScanning);

    if (m_scanner) {
        m_scanner->startScan();
    }
}

void Application::startTransfer()
{
    qDebug() << "Starting transfer...";
    m_appState->setStage(ApplicationState::StageTransfer);

    if (m_transferClient) {
        // Get selected categories from model
        auto selectedCategories = m_categoryModel->getSelectedCategories();
        m_transferClient->startTransfer(selectedCategories);
        emit transferStarted();
    }
}

void Application::cancelTransfer()
{
    qDebug() << "Cancelling transfer...";

    if (m_transferClient) {
        m_transferClient->cancelTransfer();
    }
}

void Application::onPeerDiscovered()
{
    qDebug() << "New peer discovered";
}

void Application::onPairingCompleted()
{
    qDebug() << "Pairing completed";
    m_appState->setStage(ApplicationState::StageScanning);
}

void Application::onScanningCompleted()
{
    qDebug() << "Scanning completed";
    m_appState->setStage(ApplicationState::StageCategorySelection);
}

void Application::onTransferProgress(qint64 bytesSent, qint64 totalBytes)
{
    m_progressModel->updateProgress(bytesSent, totalBytes);
}

void Application::onPairingRequestReceived(const QString &peerId, const QString &peerName, const QString &pin)
{
    qDebug() << "Pairing request received from" << peerName << "PIN:" << pin;

    // Store the PIN received from the other peer
    m_currentPin = pin;
    emit currentPinChanged();
    m_appState->setSelectedPeerId(peerId);

    // Move to pairing stage
    m_appState->setStage(ApplicationState::StagePairing);
}
