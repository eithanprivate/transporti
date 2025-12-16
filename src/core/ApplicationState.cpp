#include "ApplicationState.h"

ApplicationState::ApplicationState(QObject *parent)
    : QObject(parent)
{
}

void ApplicationState::setRole(AppRole role)
{
    if (m_role != role) {
        m_role = role;
        emit roleChanged();
    }
}

void ApplicationState::setStage(AppStage stage)
{
    if (m_stage != stage) {
        m_stage = stage;
        emit stageChanged();
    }
}

void ApplicationState::setSelectedPeerId(const QString &peerId)
{
    if (m_selectedPeerId != peerId) {
        m_selectedPeerId = peerId;
        emit selectedPeerIdChanged();
    }
}
