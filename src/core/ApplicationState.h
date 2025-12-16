#ifndef APPLICATIONSTATE_H
#define APPLICATIONSTATE_H

#include <QObject>
#include <QString>

class ApplicationState : public QObject
{
    Q_OBJECT
    Q_PROPERTY(AppRole role READ role WRITE setRole NOTIFY roleChanged)
    Q_PROPERTY(AppStage stage READ stage WRITE setStage NOTIFY stageChanged)
    Q_PROPERTY(QString selectedPeerId READ selectedPeerId WRITE setSelectedPeerId NOTIFY selectedPeerIdChanged)

public:
    enum AppRole {
        RoleUnknown,
        RoleSource,      // Linux sender
        RoleDestination  // Windows receiver
    };
    Q_ENUM(AppRole)

    enum AppStage {
        StageWelcome,
        StageRoleSelection,
        StageDiscovery,
        StagePairing,
        StageScanning,
        StageCategorySelection,
        StageTransfer,
        StageComplete
    };
    Q_ENUM(AppStage)

    explicit ApplicationState(QObject *parent = nullptr);

    AppRole role() const { return m_role; }
    void setRole(AppRole role);

    AppStage stage() const { return m_stage; }
    void setStage(AppStage stage);

    QString selectedPeerId() const { return m_selectedPeerId; }
    void setSelectedPeerId(const QString &peerId);

signals:
    void roleChanged();
    void stageChanged();
    void selectedPeerIdChanged();

private:
    AppRole m_role = RoleUnknown;
    AppStage m_stage = StageWelcome;
    QString m_selectedPeerId;
};

#endif // APPLICATIONSTATE_H
