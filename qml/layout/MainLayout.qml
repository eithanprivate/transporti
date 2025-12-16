import QtQuick 2.15
import QtQuick.Layouts 1.15
import "../components"
import "../styles"

Item {
    id: mainLayout

    property alias sidebar: sidebarLoader.sourceComponent
    property alias contentArea: contentLoader.sourceComponent

    RowLayout {
        anchors.fill: parent
        spacing: 0

        // Sidebar on the left
        Loader {
            id: sidebarLoader
            Layout.preferredWidth: AquaTheme.sidebarWidth
            Layout.fillHeight: true
        }

        // Main content area on the right
        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true

            // Gradient background
            GradientBackground {
                anchors.fill: parent
            }

            // Content loader
            Loader {
                id: contentLoader
                anchors.fill: parent
                anchors.margins: AquaTheme.cardPadding
            }
        }
    }
}
