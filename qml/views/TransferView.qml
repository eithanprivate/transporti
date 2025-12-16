import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtGraphicalEffects 1.15
import "../components"
import "../styles"

Item {
    id: root

    property int bytesTransferred: 0
    property int totalBytes: 100000000
    property string currentFile: ""
    property real transferSpeed: 0.0
    property string eta: "Calculating..."

    signal transferCancelled()

    // Background gradient
    Rectangle {
        anchors.fill: parent
        gradient: Gradient {
            GradientStop { position: 0.0; color: "#0A0E1A" }
            GradientStop { position: 1.0; color: "#1A1F2E" }
        }
    }

    // 2D Animation Area (instead of 3D)
    Item {
        id: animationArea
        anchors.fill: parent
        anchors.bottomMargin: 200
        clip: true

        // Container for animated data blocks
        Item {
            id: blocksContainer
            anchors.fill: parent
        }
    }

    // Data block spawner timer
    Timer {
        id: blockSpawner
        interval: 200
        running: true
        repeat: true
        onTriggered: {
            // Limit concurrent blocks to 30
            if (blocksContainer.children.length < 30) {
                spawnDataBlock()
            }
        }
    }

    // Bottom overlay with progress info
    Rectangle {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        height: 200
        color: "#CC000000"

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: AquaTheme.spacing * 2
            spacing: AquaTheme.spacing

            // Progress bar
            ColumnLayout {
                Layout.fillWidth: true
                spacing: 4

                RowLayout {
                    Layout.fillWidth: true

                    Text {
                        text: "Transferring..."
                        font.family: AquaTheme.fontFamily
                        font.pixelSize: AquaTheme.fontSizeNormal
                        font.weight: Font.Medium
                        color: "white"
                    }

                    Item { Layout.fillWidth: true }

                    Text {
                        text: Math.round((root.bytesTransferred / root.totalBytes) * 100) + "%"
                        font.family: AquaTheme.fontFamily
                        font.pixelSize: AquaTheme.fontSizeNormal
                        font.weight: Font.Bold
                        color: AquaTheme.accentColor
                    }
                }

                Rectangle {
                    Layout.fillWidth: true
                    height: 8
                    radius: 4
                    color: "#33FFFFFF"

                    Rectangle {
                        width: parent.width * (root.bytesTransferred / root.totalBytes)
                        height: parent.height
                        radius: parent.radius
                        color: AquaTheme.accentColor

                        Behavior on width {
                            NumberAnimation { duration: 300; easing.type: Easing.OutQuad }
                        }
                    }
                }
            }

            // Transfer stats
            GridLayout {
                Layout.fillWidth: true
                columns: 3
                rowSpacing: 4
                columnSpacing: AquaTheme.spacing * 2

                // Current file
                Text {
                    text: "Current File:"
                    font.family: AquaTheme.fontFamily
                    font.pixelSize: AquaTheme.fontSizeSmall
                    color: "#AAAAAA"
                }
                Text {
                    Layout.fillWidth: true
                    Layout.columnSpan: 2
                    text: root.currentFile || "Preparing..."
                    font.family: AquaTheme.fontFamily
                    font.pixelSize: AquaTheme.fontSizeSmall
                    color: "white"
                    elide: Text.ElideMiddle
                }

                // Speed
                Text {
                    text: "Speed:"
                    font.family: AquaTheme.fontFamily
                    font.pixelSize: AquaTheme.fontSizeSmall
                    color: "#AAAAAA"
                }
                Text {
                    text: root.transferSpeed.toFixed(1) + " MB/s"
                    font.family: AquaTheme.fontFamily
                    font.pixelSize: AquaTheme.fontSizeSmall
                    color: "white"
                }

                // ETA
                Text {
                    text: "ETA:"
                    font.family: AquaTheme.fontFamily
                    font.pixelSize: AquaTheme.fontSizeSmall
                    color: "#AAAAAA"
                }
                Text {
                    text: root.eta
                    font.family: AquaTheme.fontFamily
                    font.pixelSize: AquaTheme.fontSizeSmall
                    color: "white"
                }

                // Transferred / Total
                Text {
                    text: "Transferred:"
                    font.family: AquaTheme.fontFamily
                    font.pixelSize: AquaTheme.fontSizeSmall
                    color: "#AAAAAA"
                }
                Text {
                    Layout.columnSpan: 2
                    text: formatBytes(root.bytesTransferred) + " / " + formatBytes(root.totalBytes)
                    font.family: AquaTheme.fontFamily
                    font.pixelSize: AquaTheme.fontSizeSmall
                    color: "white"
                }
            }

            // Cancel button
            AquaButton {
                Layout.alignment: Qt.AlignHCenter
                text: "Cancel Transfer"
                implicitWidth: 150
                onClicked: {
                    console.log("Transfer cancelled")
                    root.transferCancelled()
                }
            }
        }
    }

    // Component for animated data blocks
    Component {
        id: dataBlockComponent
        Rectangle {
            id: block
            width: 40
            height: 40
            radius: 8
            color: Qt.rgba(0.3, 0.6, 1.0, 0.7)
            border.color: Qt.rgba(0.5, 0.8, 1.0, 0.9)
            border.width: 2

            // Glow effect
            layer.enabled: true
            layer.effect: Glow {
                radius: 8
                samples: 16
                color: Qt.rgba(0.3, 0.6, 1.0, 0.5)
                spread: 0.3
            }

            // Upward movement animation
            NumberAnimation on y {
                from: block.y
                to: -100
                duration: 3000
                easing.type: Easing.OutQuad
            }

            // Fade out animation
            NumberAnimation on opacity {
                from: 1.0
                to: 0.0
                duration: 3000
            }

            // Rotation animation
            RotationAnimator on rotation {
                from: 0
                to: 360
                duration: 3000
                running: true
            }

            // Scale animation
            SequentialAnimation on scale {
                running: true
                NumberAnimation { from: 0.0; to: 1.0; duration: 300; easing.type: Easing.OutBack }
                NumberAnimation { from: 1.0; to: 0.8; duration: 2700 }
            }

            // Self-destruct after animation
            Timer {
                interval: 3200
                running: true
                repeat: false
                onTriggered: {
                    block.destroy()
                }
            }
        }
    }

    // Functions
    function spawnDataBlock() {
        var block = dataBlockComponent.createObject(blocksContainer, {
            "x": Math.random() * (animationArea.width - 40),
            "y": animationArea.height + 20
        })
    }

    function formatBytes(bytes) {
        if (bytes < 1024) return bytes + " B"
        if (bytes < 1024 * 1024) return (bytes / 1024).toFixed(1) + " KB"
        if (bytes < 1024 * 1024 * 1024) return (bytes / (1024 * 1024)).toFixed(1) + " MB"
        return (bytes / (1024 * 1024 * 1024)).toFixed(2) + " GB"
    }

    // Simulate transfer progress
    Timer {
        interval: 100
        running: true
        repeat: true
        onTriggered: {
            if (root.bytesTransferred < root.totalBytes) {
                root.bytesTransferred += Math.random() * 1000000
                root.transferSpeed = 45.2 + Math.random() * 10
                root.currentFile = "/home/user/Documents/file_" + Math.floor(Math.random() * 1000) + ".pdf"

                var remaining = (root.totalBytes - root.bytesTransferred) / (root.transferSpeed * 1024 * 1024)
                var minutes = Math.floor(remaining / 60)
                var seconds = Math.floor(remaining % 60)
                root.eta = minutes + "m " + seconds + "s"
            }
        }
    }

    Component.onCompleted: {
        console.log("TransferView initialized (2D version)")
    }
}
