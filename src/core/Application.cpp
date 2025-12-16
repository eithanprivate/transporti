#include "Application.h"
#include "../models/PeerListModel.h"
#include "../models/DataCategoryModel.h"
#include "../models/TransferProgressModel.h"
#include "../network/NetworkManager.h"
#include "../network/Discovery/DiscoveryService.h"
#include "../network/Transfer/TransferClient.h"
#include "../network/Transfer/TransferServer.h"
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

        m_discoveryService->startDiscovery();
    }

    // Move to discovery stage
    m_appState->setStage(ApplicationState::StageDiscovery);
}

void Application::selectPeer(const QString &peerId)
{
    qDebug() << "Peer selected:" << peerId;
    m_appState->setSelectedPeerId(peerId);
    m_appState->setStage(ApplicationState::StagePairing);
    // Pairing logic will be implemented in PairingManager
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
