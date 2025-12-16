import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import "../components"
import "../styles"

Item {
    id: root

    property int filesTransferred: 0
    property string totalSize: "0 GB"
    property string timeTaken: "0m 0s"

    signal finished()

    ColumnLayout {
        anchors.centerIn: parent
        spacing: AquaTheme.spacing * 3
        width: parent.width * 0.6

        // Success icon
        Rectangle {
            Layout.alignment: Qt.AlignHCenter
            width: 120
            height: 120
            radius: 60
            color: AquaTheme.successColor
            opacity: 0.2

            Rectangle {
                anchors.centerIn: parent
                width: 80
                height: 80
                radius: 40
                color: AquaTheme.successColor

                // Checkmark
                Text {
                    anchors.centerIn: parent
                    text: "✓"
                    font.pixelSize: 56
                    font.weight: Font.Bold
                    color: "white"
                }

                // Success animation
                SequentialAnimation on scale {
                    running: true
                    NumberAnimation { from: 0.0; to: 1.2; duration: 400; easing.type: Easing.OutBack }
                    NumberAnimation { from: 1.2; to: 1.0; duration: 200; easing.type: Easing.InOutQuad }
                }
            }
        }

        // Title
        Text {
            Layout.alignment: Qt.AlignHCenter
            text: "Transfer Complete!"
            font.family: AquaTheme.fontFamily
            font.pixelSize: AquaTheme.fontSizeLarge
            font.weight: Font.Bold
            color: AquaTheme.textPrimary
        }

        // Success message
        Text {
            Layout.alignment: Qt.AlignHCenter
            Layout.fillWidth: true
            text: "Your data has been successfully transferred to the new computer"
            font.family: AquaTheme.fontFamily
            font.pixelSize: AquaTheme.fontSizeNormal
            color: AquaTheme.textSecondary
            wrapMode: Text.WordWrap
            horizontalAlignment: Text.AlignHCenter
        }

        // Statistics card
        Rectangle {
            Layout.alignment: Qt.AlignHCenter
            Layout.preferredWidth: 500
            Layout.preferredHeight: 200
            radius: AquaTheme.borderRadius * 2
            color: AquaTheme.cardBackground
            border.color: AquaTheme.borderColor
            border.width: 1

            gradient: Gradient {
                GradientStop { position: 0.0; color: "#FFFFFF" }
                GradientStop { position: 1.0; color: "#F8FBFF" }
            }

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: AquaTheme.spacing * 2
                spacing: AquaTheme.spacing * 2

                // Title
                Text {
                    text: "Transfer Summary"
                    font.family: AquaTheme.fontFamily
                    font.pixelSize: AquaTheme.fontSizeNormal
                    font.weight: Font.Medium
                    color: AquaTheme.textPrimary
                }

                // Stats grid
                GridLayout {
                    Layout.fillWidth: true
                    columns: 2
                    rowSpacing: AquaTheme.spacing
                    columnSpacing: AquaTheme.spacing * 2

                    // Files transferred
                    Text {
                        text: "Files Transferred:"
                        font.family: AquaTheme.fontFamily
                        font.pixelSize: AquaTheme.fontSizeNormal
                        color: AquaTheme.textSecondary
                    }
                    Text {
                        text: root.filesTransferred.toString()
                        font.family: AquaTheme.fontFamily
                        font.pixelSize: AquaTheme.fontSizeNormal
                        font.weight: Font.Medium
                        color: AquaTheme.textPrimary
                    }

                    // Total size
                    Text {
                        text: "Total Size:"
                        font.family: AquaTheme.fontFamily
                        font.pixelSize: AquaTheme.fontSizeNormal
                        color: AquaTheme.textSecondary
                    }
                    Text {
                        text: root.totalSize
                        font.family: AquaTheme.fontFamily
                        font.pixelSize: AquaTheme.fontSizeNormal
                        font.weight: Font.Medium
                        color: AquaTheme.textPrimary
                    }

                    // Time taken
                    Text {
                        text: "Time Taken:"
                        font.family: AquaTheme.fontFamily
                        font.pixelSize: AquaTheme.fontSizeNormal
                        color: AquaTheme.textSecondary
                    }
                    Text {
                        text: root.timeTaken
                        font.family: AquaTheme.fontFamily
                        font.pixelSize: AquaTheme.fontSizeNormal
                        font.weight: Font.Medium
                        color: AquaTheme.textPrimary
                    }

                    // Average speed
                    Text {
                        text: "Average Speed:"
                        font.family: AquaTheme.fontFamily
                        font.pixelSize: AquaTheme.fontSizeNormal
                        color: AquaTheme.textSecondary
                    }
                    Text {
                        text: "45.2 MB/s"
                        font.family: AquaTheme.fontFamily
                        font.pixelSize: AquaTheme.fontSizeNormal
                        font.weight: Font.Medium
                        color: AquaTheme.textPrimary
                    }
                }
            }
        }

        // Next steps info
        Rectangle {
            Layout.alignment: Qt.AlignHCenter
            Layout.preferredWidth: 500
            Layout.preferredHeight: 100
            radius: AquaTheme.borderRadius
            color: "#E8F5E9"
            border.color: "#81C784"
            border.width: 1

            RowLayout {
                anchors.fill: parent
                anchors.margins: AquaTheme.spacing
                spacing: AquaTheme.spacing

                Text {
                    text: "ℹ️"
                    font.pixelSize: 24
                }

                ColumnLayout {
                    Layout.fillWidth: true
                    spacing: 4

                    Text {
                        text: "What's Next?"
                        font.family: AquaTheme.fontFamily
                        font.pixelSize: AquaTheme.fontSizeNormal
                        font.weight: Font.Medium
                        color: "#2E7D32"
                    }

                    Text {
                        Layout.fillWidth: true
                        text: "Your files are now on the destination computer. You can verify them in the corresponding folders."
                        font.family: AquaTheme.fontFamily
                        font.pixelSize: AquaTheme.fontSizeSmall
                        color: "#388E3C"
                        wrapMode: Text.WordWrap
                    }
                }
            }
        }

        // Action buttons
        RowLayout {
            Layout.alignment: Qt.AlignHCenter
            spacing: AquaTheme.spacing

            AquaButton {
                text: "View Log"
                implicitWidth: 120
                onClicked: {
                    console.log("View transfer log")
                    // TODO: Show transfer log
                }
            }

            AquaButton {
                text: "Finish"
                implicitWidth: 120
                primary: true
                onClicked: {
                    console.log("Transfer finished")
                    root.finished()
                }
            }
        }
    }

    Component.onCompleted: {
        console.log("CompletionView initialized")
    }
}
