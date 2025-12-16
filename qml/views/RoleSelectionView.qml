import QtQuick 2.15
import QtQuick.Layouts 1.15
import "../components"
import "../styles"

Item {
    id: roleSelectionView

    signal roleSelected(int role)  // 1 for Source, 2 for Destination

    ColumnLayout {
        anchors.centerIn: parent
        spacing: AquaTheme.spacingLarge
        width: parent.width * 0.7

        // Title
        Text {
            Layout.fillWidth: true
            text: "Select Your Role"
            font.family: AquaTheme.fontFamily
            font.pixelSize: AquaTheme.fontSizeTitle
            font.weight: Font.Bold
            color: AquaTheme.textPrimary
            horizontalAlignment: Text.AlignHCenter
        }

        Text {
            Layout.fillWidth: true
            text: "Choose whether this computer is sending or receiving data"
            font.family: AquaTheme.fontFamily
            font.pixelSize: AquaTheme.fontSizeNormal
            color: AquaTheme.textSecondary
            horizontalAlignment: Text.AlignHCenter
            wrapMode: Text.WordWrap
        }

        Item { Layout.preferredHeight: AquaTheme.spacingLarge }

        // Role cards
        RowLayout {
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignHCenter
            spacing: AquaTheme.spacingLarge

            // Source (Linux) card
            Rectangle {
                Layout.preferredWidth: 250
                Layout.preferredHeight: 300
                radius: AquaTheme.borderRadius
                color: "white"
                border.color: AquaTheme.buttonBorder
                border.width: 1

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: AquaTheme.cardPadding
                    spacing: AquaTheme.spacing

                    Text {
                        text: "💻"
                        font.pixelSize: 60
                        Layout.alignment: Qt.AlignHCenter
                    }

                    Text {
                        Layout.fillWidth: true
                        text: "Source (Linux)"
                        font.family: AquaTheme.fontFamily
                        font.pixelSize: AquaTheme.fontSizeLarge
                        font.weight: Font.Bold
                        color: AquaTheme.textPrimary
                        horizontalAlignment: Text.AlignHCenter
                    }

                    Text {
                        Layout.fillWidth: true
                        text: "This computer will send data to the destination"
                        font.family: AquaTheme.fontFamily
                        font.pixelSize: AquaTheme.fontSizeSmall
                        color: AquaTheme.textSecondary
                        horizontalAlignment: Text.AlignHCenter
                        wrapMode: Text.WordWrap
                    }

                    Item { Layout.fillHeight: true }

                    AquaButton {
                        Layout.fillWidth: true
                        text: "I'm the Source"
                        onClicked: roleSelectionView.roleSelected(1)  // Source
                    }
                }
            }

            // Destination (Windows) card
            Rectangle {
                Layout.preferredWidth: 250
                Layout.preferredHeight: 300
                radius: AquaTheme.borderRadius
                color: "white"
                border.color: AquaTheme.buttonBorder
                border.width: 1

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: AquaTheme.cardPadding
                    spacing: AquaTheme.spacing

                    Text {
                        text: "🖥️"
                        font.pixelSize: 60
                        Layout.alignment: Qt.AlignHCenter
                    }

                    Text {
                        Layout.fillWidth: true
                        text: "Destination (Windows)"
                        font.family: AquaTheme.fontFamily
                        font.pixelSize: AquaTheme.fontSizeLarge
                        font.weight: Font.Bold
                        color: AquaTheme.textPrimary
                        horizontalAlignment: Text.AlignHCenter
                    }

                    Text {
                        Layout.fillWidth: true
                        text: "This computer will receive data from the source"
                        font.family: AquaTheme.fontFamily
                        font.pixelSize: AquaTheme.fontSizeSmall
                        color: AquaTheme.textSecondary
                        horizontalAlignment: Text.AlignHCenter
                        wrapMode: Text.WordWrap
                    }

                    Item { Layout.fillHeight: true }

                    AquaButton {
                        Layout.fillWidth: true
                        text: "I'm the Destination"
                        onClicked: roleSelectionView.roleSelected(2)  // Destination
                    }
                }
            }
        }
    }
}
