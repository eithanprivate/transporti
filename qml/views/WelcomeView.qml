import QtQuick 2.15
import QtQuick.Layouts 1.15
import "../components"
import "../styles"

Item {
    id: welcomeView

    signal startClicked()

    ColumnLayout {
        anchors.centerIn: parent
        spacing: AquaTheme.spacingLarge
        width: parent.width * 0.6

        // Welcome icon/image
        Text {
            Layout.alignment: Qt.AlignHCenter
            text: "🚀"
            font.pixelSize: 80
        }

        // Title
        Text {
            Layout.alignment: Qt.AlignHCenter
            text: "Welcome to Transporti"
            font.family: AquaTheme.fontFamily
            font.pixelSize: AquaTheme.fontSizeHuge
            font.weight: Font.Bold
            color: AquaTheme.textPrimary
        }

        // Description
        Text {
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignHCenter
            text: "Seamlessly migrate your data from Linux to Windows with ease."
            font.family: AquaTheme.fontFamily
            font.pixelSize: AquaTheme.fontSizeMedium
            color: AquaTheme.textSecondary
            horizontalAlignment: Text.AlignHCenter
            wrapMode: Text.WordWrap
        }

        Item { Layout.preferredHeight: AquaTheme.spacing }

        // Start button
        AquaButton {
            Layout.alignment: Qt.AlignHCenter
            text: "Get Started"
            implicitWidth: 200
            primary: true
            onClicked: welcomeView.startClicked()
        }
    }
}
