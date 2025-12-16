import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick3D 1.15
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

    // 3D Scene
    View3D {
        id: view3D
        anchors.fill: parent
        anchors.bottomMargin: 200

        environment: SceneEnvironment {
            clearColor: "transparent"
            backgroundMode: SceneEnvironment.Transparent
            antialiasingMode: SceneEnvironment.MSAA
            antialiasingQuality: SceneEnvironment.High
        }

        // Camera
        PerspectiveCamera {
            id: camera
            position: Qt.vector3d(0, 150, 400)
            eulerRotation.x: -20

            // Subtle camera movement
            SequentialAnimation on eulerRotation.y {
                running: true
                loops: Animation.Infinite
                NumberAnimation { from: -10; to: 10; duration: 8000; easing.type: Easing.InOutQuad }
                NumberAnimation { from: 10; to: -10; duration: 8000; easing.type: Easing.InOutQuad }
            }
        }

        // Main directional light
        DirectionalLight {
            eulerRotation.x: -30
            eulerRotation.y: -45
            brightness: 1.0
            color: "#FFFFFF"
        }

        // Fill light
        DirectionalLight {
            eulerRotation.x: 30
            eulerRotation.y: 135
            brightness: 0.5
            color: "#8AAAFF"
        }

        // Ambient light
        DirectionalLight {
            eulerRotation.x: 0
            eulerRotation.y: 0
            brightness: 0.3
            color: "#6688DD"
        }

        // Container for cubes
        Node {
            id: cubesContainer
        }
    }

    // Cube spawner timer
    Timer {
        id: cubeSpawner
        interval: 300
        running: true
        repeat: true
        onTriggered: {
            // Limit concurrent cubes to 50
            if (cubesContainer.children.length < 50) {
                spawnCube()
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

    // Functions
    function spawnCube() {
        var component = Qt.createComponent("DataCube.qml")
        if (component.status === Component.Ready) {
            var cube = component.createObject(cubesContainer, {
                "x": Math.random() * 400 - 200,
                "y": -150,
                "z": Math.random() * 200 - 100
            })
        }
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
        console.log("TransferView initialized")
    }
}
