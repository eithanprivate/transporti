import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtGraphicalEffects 1.15
import "../components"
import "../styles"

Item {
    id: root

    property string pin: appCore.currentPin || "------"
    property bool isSource: appState.role === 1  // 1 = source (Linux), 2 = destination (Windows)

    signal pairingConfirmed()
    signal pairingCancelled()

    // Update PIN when it changes
    Connections {
        target: appCore
        function onCurrentPinChanged() {
            console.log("PIN updated:", appCore.currentPin)
        }
    }

    ColumnLayout {
        anchors.centerIn: parent
        spacing: AquaTheme.spacing * 3
        width: parent.width * 0.6

        // Title
        Text {
            Layout.alignment: Qt.AlignHCenter
            text: isSource ? "Pairing Code" : "Verify Pairing Code"
            font.family: AquaTheme.fontFamily
            font.pixelSize: AquaTheme.fontSizeLarge
            font.weight: Font.Bold
            color: AquaTheme.textPrimary
        }

        // Instructions
        Text {
            Layout.alignment: Qt.AlignHCenter
            Layout.fillWidth: true
            text: isSource
                ? "Share this code with the destination device to establish a secure connection"
                : "Verify this code matches the one displayed on the source device"
            font.family: AquaTheme.fontFamily
            font.pixelSize: AquaTheme.fontSizeNormal
            color: AquaTheme.textSecondary
            wrapMode: Text.WordWrap
            horizontalAlignment: Text.AlignHCenter
        }

        // PIN Display Card
        Rectangle {
            Layout.alignment: Qt.AlignHCenter
            width: 400
            height: 120
            radius: AquaTheme.borderRadius * 2
            color: AquaTheme.cardBackground
            border.color: AquaTheme.accentColor
            border.width: 2

            // Gradient overlay
            gradient: Gradient {
                GradientStop { position: 0.0; color: "#FFFFFF" }
                GradientStop { position: 1.0; color: "#F0F5FA" }
            }

            // PIN digits
            RowLayout {
                anchors.centerIn: parent
                spacing: 12

                Repeater {
                    model: 6

                    Rectangle {
                        width: 50
                        height: 70
                        radius: 8
                        color: "white"
                        border.color: AquaTheme.borderColor
                        border.width: 1

                        // Digit text
                        Text {
                            anchors.centerIn: parent
                            text: root.pin.charAt(index)
                            font.family: AquaTheme.fontFamily
                            font.pixelSize: 36
                            font.weight: Font.Bold
                            color: AquaTheme.accentColor
                        }

                        // Subtle shadow
                        layer.enabled: true
                        layer.effect: DropShadow {
                            horizontalOffset: 0
                            verticalOffset: 2
                            radius: 4
                            samples: 8
                            color: "#20000000"
                            transparentBorder: true
                        }
                    }
                }
            }
        }

        // Status indicator
        RowLayout {
            Layout.alignment: Qt.AlignHCenter
            spacing: AquaTheme.spacing

            Rectangle {
                id: statusIndicator
                width: 16
                height: 16
                radius: 8
                color: isSource ? AquaTheme.warningColor : AquaTheme.accentColor

                // Pulsing animation for waiting state
                SequentialAnimation on opacity {
                    running: isSource
                    loops: Animation.Infinite
                    NumberAnimation { from: 1.0; to: 0.3; duration: 800 }
                    NumberAnimation { from: 0.3; to: 1.0; duration: 800 }
                }
            }

            Text {
                text: isSource ? "Waiting for confirmation..." : "Ready to confirm"
                font.family: AquaTheme.fontFamily
                font.pixelSize: AquaTheme.fontSizeSmall
                color: AquaTheme.textSecondary
            }
        }

        // Connection info
        Rectangle {
            Layout.alignment: Qt.AlignHCenter
            Layout.preferredWidth: 450
            Layout.preferredHeight: 80
            radius: AquaTheme.borderRadius
            color: "#F8FBFF"
            border.color: AquaTheme.borderColor
            border.width: 1

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: AquaTheme.spacing
                spacing: 4

                Text {
                    text: "Connection Details"
                    font.family: AquaTheme.fontFamily
                    font.pixelSize: AquaTheme.fontSizeSmall
                    font.weight: Font.Medium
                    color: AquaTheme.textPrimary
                }

                Text {
                    text: "• Encrypted connection using AES-256"
                    font.family: AquaTheme.fontFamily
                    font.pixelSize: AquaTheme.fontSizeSmall
                    color: AquaTheme.textSecondary
                }

                Text {
                    text: "• Direct peer-to-peer transfer on local network"
                    font.family: AquaTheme.fontFamily
                    font.pixelSize: AquaTheme.fontSizeSmall
                    color: AquaTheme.textSecondary
                }
            }
        }

        // Action buttons
        RowLayout {
            Layout.alignment: Qt.AlignHCenter
            spacing: AquaTheme.spacing

            AquaButton {
                text: "Cancel"
                implicitWidth: 120
                onClicked: {
                    console.log("Pairing cancelled")
                    root.pairingCancelled()
                }
            }

            // Show confirm button only for destination
            AquaButton {
                visible: !isSource
                text: "Confirm"
                implicitWidth: 120
                primary: true
                onClicked: {
                    console.log("Pairing confirmed")
                    root.pairingConfirmed()
                }
            }
        }
    }

    // Generate random PIN on creation
    Component.onCompleted: {
        console.log("PairingView initialized")
        // Generate random 6-digit PIN
        var randomPin = ""
        for (var i = 0; i < 6; i++) {
            randomPin += Math.floor(Math.random() * 10)
        }
        root.pin = randomPin
        console.log("Generated PIN:", root.pin)
    }
}
