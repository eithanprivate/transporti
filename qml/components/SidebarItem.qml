import QtQuick 2.15
import "../styles"

Item {
    id: root
    height: 60

    property string stageName: ""
    property string stageIcon: ""
    property bool completed: false
    property bool current: false

    Rectangle {
        anchors.fill: parent
        anchors.margins: 4
        radius: 6
        color: current ? "#20" + AquaTheme.primaryBlue.toString().substring(1) : "transparent"

        Row {
            anchors.fill: parent
            anchors.margins: 12
            spacing: 12

            // Status indicator
            Rectangle {
                width: 24
                height: 24
                radius: 12
                anchors.verticalCenter: parent.verticalCenter
                color: completed ? AquaTheme.primaryBlue : (current ? AquaTheme.accentBlue : AquaTheme.textTertiary)
                border.color: completed || current ? "transparent" : AquaTheme.textSecondary
                border.width: 2

                // Checkmark for completed
                Text {
                    anchors.centerIn: parent
                    text: completed ? "✓" : ""
                    color: "white"
                    font.pixelSize: 14
                    font.bold: true
                }
            }

            // Stage name
            Text {
                anchors.verticalCenter: parent.verticalCenter
                text: root.stageName
                font.family: AquaTheme.fontFamily
                font.pixelSize: AquaTheme.fontSizeNormal
                font.weight: current ? Font.DemiBold : Font.Normal
                color: current ? AquaTheme.textPrimary : AquaTheme.textSecondary
            }
        }
    }
}
