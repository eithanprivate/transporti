import QtQuick 2.15
import QtQuick.Layouts 1.15
import "../components"
import "../styles"

Item {
    id: discoveryView

    ColumnLayout {
        anchors.fill: parent
        spacing: AquaTheme.spacingLarge

        // Title
        Text {
            Layout.fillWidth: true
            text: "Looking for devices..."
            font.family: AquaTheme.fontFamily
            font.pixelSize: AquaTheme.fontSizeTitle
            font.weight: Font.Bold
            color: AquaTheme.textPrimary
        }

        Text {
            Layout.fillWidth: true
            text: "Make sure both computers are connected to the same network"
            font.family: AquaTheme.fontFamily
            font.pixelSize: AquaTheme.fontSizeNormal
            color: AquaTheme.textSecondary
            wrapMode: Text.WordWrap
        }

        // Animated searching indicator
        Row {
            Layout.alignment: Qt.AlignHCenter
            spacing: 8

            Repeater {
                model: 3
                Rectangle {
                    width: 12
                    height: 12
                    radius: 6
                    color: AquaTheme.primaryBlue

                    SequentialAnimation on opacity {
                        loops: Animation.Infinite
                        NumberAnimation {
                            from: 0.3
                            to: 1.0
                            duration: 600
                        }
                        NumberAnimation {
                            from: 1.0
                            to: 0.3
                            duration: 600
                        }
                        PauseAnimation { duration: index * 200 }
                    }
                }
            }
        }

        // Peer list
        ListView {
            Layout.fillWidth: true
            Layout.fillHeight: true
            model: peerModel
            spacing: AquaTheme.spacing

            delegate: Rectangle {
                width: ListView.view.width
                height: 80
                radius: AquaTheme.borderRadius
                color: "white"
                border.color: AquaTheme.buttonBorder
                border.width: 1

                RowLayout {
                    anchors.fill: parent
                    anchors.margins: AquaTheme.spacing
                    spacing: AquaTheme.spacing

                    Text {
                        text: model.deviceOS === "linux" ? "💻" : "🖥️"
                        font.pixelSize: 40
                    }

                    ColumnLayout {
                        Layout.fillWidth: true
                        spacing: 4

                        Text {
                            text: model.deviceName
                            font.family: AquaTheme.fontFamily
                            font.pixelSize: AquaTheme.fontSizeMedium
                            font.weight: Font.DemiBold
                            color: AquaTheme.textPrimary
                        }

                        Text {
                            text: model.deviceIP + " • " + (model.deviceOS === "linux" ? "Linux" : "Windows")
                            font.family: AquaTheme.fontFamily
                            font.pixelSize: AquaTheme.fontSizeSmall
                            color: AquaTheme.textSecondary
                        }
                    }

                    AquaButton {
                        text: "Connect"
                        implicitWidth: 120
                        implicitHeight: AquaTheme.buttonHeightSmall
                        onClicked: {
                            appCore.selectPeer(model.deviceId)
                        }
                    }
                }
            }

            // Empty state
            Text {
                anchors.centerIn: parent
                visible: parent.count === 0
                text: "No devices found yet...\nSearching..."
                font.family: AquaTheme.fontFamily
                font.pixelSize: AquaTheme.fontSizeNormal
                color: AquaTheme.textSecondary
                horizontalAlignment: Text.AlignHCenter
            }
        }
    }
}
