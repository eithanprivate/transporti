import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import "../components"
import "../styles"

Item {
    id: root

    signal scanComplete()

    ColumnLayout {
        anchors.centerIn: parent
        spacing: AquaTheme.spacing * 2
        width: parent.width * 0.6

        // Title
        Text {
            Layout.alignment: Qt.AlignHCenter
            text: "Scanning Your Data..."
            font.family: AquaTheme.fontFamily
            font.pixelSize: AquaTheme.fontSizeLarge
            font.weight: Font.Bold
            color: AquaTheme.textPrimary
        }

        // Animated scanner icon
        Rectangle {
            Layout.alignment: Qt.AlignHCenter
            width: 120
            height: 120
            radius: 60
            color: AquaTheme.accentColor
            opacity: 0.2

            Rectangle {
                id: scannerDot
                anchors.centerIn: parent
                width: 20
                height: 20
                radius: 10
                color: AquaTheme.accentColor

                SequentialAnimation on scale {
                    running: true
                    loops: Animation.Infinite
                    NumberAnimation { from: 1.0; to: 1.5; duration: 600; easing.type: Easing.InOutQuad }
                    NumberAnimation { from: 1.5; to: 1.0; duration: 600; easing.type: Easing.InOutQuad }
                }

                SequentialAnimation on opacity {
                    running: true
                    loops: Animation.Infinite
                    NumberAnimation { from: 1.0; to: 0.3; duration: 600 }
                    NumberAnimation { from: 0.3; to: 1.0; duration: 600 }
                }
            }
        }

        // Current scanner status
        Text {
            id: statusText
            Layout.alignment: Qt.AlignHCenter
            text: "Scanning documents..."
            font.family: AquaTheme.fontFamily
            font.pixelSize: AquaTheme.fontSizeNormal
            color: AquaTheme.textSecondary
        }

        // Progress information
        ColumnLayout {
            Layout.fillWidth: true
            spacing: AquaTheme.spacing

            // Categories being scanned
            Repeater {
                model: ListModel {
                    ListElement { name: "Documents"; status: "scanning"; files: 0 }
                    ListElement { name: "Configurations"; status: "pending"; files: 0 }
                    ListElement { name: "Applications"; status: "pending"; files: 0 }
                    ListElement { name: "Email"; status: "pending"; files: 0 }
                    ListElement { name: "Browser Data"; status: "pending"; files: 0 }
                }

                delegate: RowLayout {
                    Layout.fillWidth: true
                    spacing: AquaTheme.spacing

                    Rectangle {
                        width: 16
                        height: 16
                        radius: 8
                        color: {
                            if (status === "completed") return AquaTheme.successColor
                            if (status === "scanning") return AquaTheme.accentColor
                            return AquaTheme.borderColor
                        }

                        // Animated scanning indicator
                        SequentialAnimation on opacity {
                            running: status === "scanning"
                            loops: Animation.Infinite
                            NumberAnimation { from: 1.0; to: 0.3; duration: 500 }
                            NumberAnimation { from: 0.3; to: 1.0; duration: 500 }
                        }
                    }

                    Text {
                        Layout.fillWidth: true
                        text: name
                        font.family: AquaTheme.fontFamily
                        font.pixelSize: AquaTheme.fontSizeNormal
                        color: status === "completed" ? AquaTheme.textPrimary : AquaTheme.textSecondary
                    }

                    Text {
                        text: files > 0 ? files + " files" : ""
                        font.family: AquaTheme.fontFamily
                        font.pixelSize: AquaTheme.fontSizeSmall
                        color: AquaTheme.textSecondary
                    }
                }
            }
        }

        // Cancel button
        AquaButton {
            Layout.alignment: Qt.AlignHCenter
            text: "Cancel"
            implicitWidth: 120
            onClicked: {
                // TODO: Implement cancel logic
                console.log("Scan cancelled")
            }
        }
    }

    // Simulate scanning progress (will be replaced with real scanner signals)
    Timer {
        id: scanTimer
        interval: 2000
        repeat: false
        onTriggered: {
            console.log("Scan simulation complete")
            root.scanComplete()
        }
    }

    Component.onCompleted: {
        console.log("ScanningView initialized")
        scanTimer.start()
    }
}
