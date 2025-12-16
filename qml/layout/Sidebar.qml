import QtQuick 2.15
import QtQuick.Layouts 1.15
import "../components"
import "../styles"

Rectangle {
    id: sidebar
    width: AquaTheme.sidebarWidth
    color: AquaTheme.sidebarBackground

    property var stagesModel

    // Inner shadow effect on right edge
    Rectangle {
        anchors.fill: parent
        anchors.leftMargin: parent.width - 1
        width: 1
        gradient: Gradient {
            orientation: Gradient.Horizontal
            GradientStop { position: 0.0; color: "#20000000" }
            GradientStop { position: 1.0; color: "#00000000" }
        }
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 12

        // Logo/Title
        Item {
            Layout.fillWidth: true
            Layout.preferredHeight: 80

            Column {
                anchors.centerIn: parent
                spacing: 4

                Text {
                    anchors.horizontalCenter: parent.horizontalCenter
                    text: "Transporti"
                    font.family: AquaTheme.fontFamily
                    font.pixelSize: AquaTheme.fontSizeLarge
                    font.weight: Font.Bold
                    color: AquaTheme.primaryBlue
                }

                Text {
                    anchors.horizontalCenter: parent.horizontalCenter
                    text: "PC Migration"
                    font.family: AquaTheme.fontFamily
                    font.pixelSize: AquaTheme.fontSizeSmall
                    color: AquaTheme.textSecondary
                }
            }
        }

        Item { Layout.preferredHeight: 20 }

        // Progress stages
        Repeater {
            model: sidebar.stagesModel
            delegate: SidebarItem {
                Layout.fillWidth: true
                stageName: model.name
                stageIcon: model.icon || ""
                completed: model.completed || false
                current: model.current || false
            }
        }

        Item { Layout.fillHeight: true }

        // Version info
        Text {
            Layout.fillWidth: true
            text: "Version 1.0.0"
            font.family: AquaTheme.fontFamily
            font.pixelSize: AquaTheme.fontSizeSmall - 2
            color: AquaTheme.textTertiary
            horizontalAlignment: Text.AlignHCenter
        }
    }
}
