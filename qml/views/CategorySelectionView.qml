import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtGraphicalEffects 1.15
import "../components"
import "../styles"

Item {
    id: root

    signal categoriesSelected()

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: AquaTheme.cardPadding
        spacing: AquaTheme.spacing * 2

        // Header
        ColumnLayout {
            Layout.fillWidth: true
            spacing: AquaTheme.spacing

            Text {
                text: "Select Data to Transfer"
                font.family: AquaTheme.fontFamily
                font.pixelSize: AquaTheme.fontSizeLarge
                font.weight: Font.Bold
                color: AquaTheme.textPrimary
            }

            Text {
                text: "Choose which categories of data you want to migrate to your new PC"
                font.family: AquaTheme.fontFamily
                font.pixelSize: AquaTheme.fontSizeNormal
                color: AquaTheme.textSecondary
                wrapMode: Text.WordWrap
                Layout.fillWidth: true
            }
        }

        // Selection controls
        RowLayout {
            Layout.fillWidth: true
            spacing: AquaTheme.spacing

            AquaButton {
                text: "Select All"
                implicitWidth: 120
                onClicked: {
                    for (var i = 0; i < categoriesModel.count; i++) {
                        categoriesModel.setProperty(i, "selected", true)
                    }
                }
            }

            AquaButton {
                text: "Deselect All"
                implicitWidth: 120
                onClicked: {
                    for (var i = 0; i < categoriesModel.count; i++) {
                        categoriesModel.setProperty(i, "selected", false)
                    }
                }
            }

            Item { Layout.fillWidth: true }

            // Total size display
            Text {
                text: calculateTotalSize()
                font.family: AquaTheme.fontFamily
                font.pixelSize: AquaTheme.fontSizeNormal
                font.weight: Font.Medium
                color: AquaTheme.textPrimary
            }
        }

        // Categories list
        ListView {
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: AquaTheme.spacing
            clip: true

            model: ListModel {
                id: categoriesModel
                ListElement {
                    name: "Documents"
                    description: "PDFs, Word docs, images, videos, music"
                    fileCount: 1547
                    size: 12589440000
                    selected: true
                }
                ListElement {
                    name: "Configurations"
                    description: "Application settings and preferences"
                    fileCount: 234
                    size: 45678900
                    selected: true
                }
                ListElement {
                    name: "Applications"
                    description: "List of installed software packages"
                    fileCount: 1
                    size: 12450
                    selected: true
                }
                ListElement {
                    name: "Email"
                    description: "Thunderbird and Evolution mailboxes"
                    fileCount: 89
                    size: 234567800
                    selected: false
                }
                ListElement {
                    name: "Browser Data"
                    description: "Bookmarks, history, and saved passwords"
                    fileCount: 12
                    size: 567890
                    selected: true
                }
            }

            delegate: Rectangle {
                width: ListView.view.width
                height: 100
                radius: AquaTheme.borderRadius
                color: selected ? AquaTheme.cardBackgroundSelected : AquaTheme.cardBackground
                border.color: AquaTheme.borderColor
                border.width: 1

                // Drop shadow
                layer.enabled: true
                layer.effect: DropShadow {
                    horizontalOffset: 0
                    verticalOffset: 2
                    radius: 4
                    samples: 8
                    color: AquaTheme.shadowColor
                    transparentBorder: true
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        categoriesModel.setProperty(index, "selected", !selected)
                    }
                }

                RowLayout {
                    anchors.fill: parent
                    anchors.margins: AquaTheme.spacing
                    spacing: AquaTheme.spacing

                    // Checkbox
                    Rectangle {
                        width: 24
                        height: 24
                        radius: 4
                        border.color: AquaTheme.accentColor
                        border.width: 2
                        color: selected ? AquaTheme.accentColor : "transparent"

                        Text {
                            anchors.centerIn: parent
                            text: "✓"
                            font.pixelSize: 16
                            font.weight: Font.Bold
                            color: "white"
                            visible: selected
                        }
                    }

                    // Info
                    ColumnLayout {
                        Layout.fillWidth: true
                        spacing: 4

                        Text {
                            text: name
                            font.family: AquaTheme.fontFamily
                            font.pixelSize: AquaTheme.fontSizeNormal
                            font.weight: Font.Medium
                            color: AquaTheme.textPrimary
                        }

                        Text {
                            text: description
                            font.family: AquaTheme.fontFamily
                            font.pixelSize: AquaTheme.fontSizeSmall
                            color: AquaTheme.textSecondary
                        }

                        Text {
                            text: fileCount + " files · " + formatBytes(size)
                            font.family: AquaTheme.fontFamily
                            font.pixelSize: AquaTheme.fontSizeSmall
                            color: AquaTheme.textSecondary
                        }
                    }
                }

                Behavior on color {
                    ColorAnimation { duration: AquaTheme.animationFast }
                }
            }
        }

        // Action buttons
        RowLayout {
            Layout.fillWidth: true
            spacing: AquaTheme.spacing

            Item { Layout.fillWidth: true }

            AquaButton {
                text: "Back"
                implicitWidth: 120
                onClicked: {
                    // TODO: Go back to previous view
                    console.log("Going back...")
                }
            }

            AquaButton {
                text: "Continue"
                implicitWidth: 120
                primary: true
                onClicked: {
                    console.log("Categories selected, starting transfer...")
                    root.categoriesSelected()
                }
            }
        }
    }

    // Helper functions
    function formatBytes(bytes) {
        if (bytes < 1024) return bytes + " B"
        if (bytes < 1024 * 1024) return (bytes / 1024).toFixed(1) + " KB"
        if (bytes < 1024 * 1024 * 1024) return (bytes / (1024 * 1024)).toFixed(1) + " MB"
        return (bytes / (1024 * 1024 * 1024)).toFixed(2) + " GB"
    }

    function calculateTotalSize() {
        var total = 0
        for (var i = 0; i < categoriesModel.count; i++) {
            if (categoriesModel.get(i).selected) {
                total += categoriesModel.get(i).size
            }
        }
        return "Total: " + formatBytes(total)
    }

    Component.onCompleted: {
        console.log("CategorySelectionView initialized")
    }
}
