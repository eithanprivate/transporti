import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import Transporti.Core 1.0
import "components"
import "views"
import "layout"
import "styles"

ApplicationWindow {
    id: window
    width: 1200
    height: 800
    visible: true
    title: "Transporti - PC Migration"
    minimumWidth: 1000
    minimumHeight: 700

    // Stages model for sidebar
    ListModel {
        id: stagesModel
        ListElement { name: "Welcome"; icon: ""; completed: false; current: true }
        ListElement { name: "Role"; icon: ""; completed: false; current: false }
        ListElement { name: "Discover"; icon: ""; completed: false; current: false }
        ListElement { name: "Pair"; icon: ""; completed: false; current: false }
        ListElement { name: "Scan"; icon: ""; completed: false; current: false }
        ListElement { name: "Select"; icon: ""; completed: false; current: false }
        ListElement { name: "Transfer"; icon: ""; completed: false; current: false }
        ListElement { name: "Complete"; icon: ""; completed: false; current: false }
    }

    // Update stages based on appState
    Connections {
        target: appState
        function onStageChanged() {
            updateStages()
        }
    }

    function updateStages() {
        // Mark stages as current based on app state
        for (var i = 0; i < stagesModel.count; i++) {
            stagesModel.setProperty(i, "current", false)
            stagesModel.setProperty(i, "completed", false)
        }

        var currentStage = appState.stage
        if (currentStage >= 0 && currentStage < stagesModel.count) {
            stagesModel.setProperty(currentStage, "current", true)
            // Mark previous stages as completed
            for (var j = 0; j < currentStage; j++) {
                stagesModel.setProperty(j, "completed", true)
            }
        }
    }

    // Main layout
    RowLayout {
        anchors.fill: parent
        spacing: 0

        // Sidebar
        Sidebar {
            Layout.preferredWidth: AquaTheme.sidebarWidth
            Layout.fillHeight: true
            stagesModel: stagesModel
        }

        // Main content area
        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true

            // Gradient background
            GradientBackground {
                anchors.fill: parent
            }

            // Content stack view
            StackView {
                id: stackView
                anchors.fill: parent
                anchors.margins: AquaTheme.cardPadding
                initialItem: welcomeView

                // Smooth transitions
                pushEnter: Transition {
                    PropertyAnimation {
                        property: "opacity"
                        from: 0
                        to: 1
                        duration: AquaTheme.animationNormal
                    }
                }
                pushExit: Transition {
                    PropertyAnimation {
                        property: "opacity"
                        from: 1
                        to: 0
                        duration: AquaTheme.animationNormal
                    }
                }
            }
        }
    }

    // View components
    Component {
        id: welcomeView
        WelcomeView {
            onStartClicked: {
                stackView.push(roleSelectionView)
                stagesModel.setProperty(1, "current", true)
                stagesModel.setProperty(0, "completed", true)
                stagesModel.setProperty(0, "current", false)
            }
        }
    }

    Component {
        id: roleSelectionView
        RoleSelectionView {
            onRoleSelected: function(role) {
                console.log("Role selected:", role)
                // Convert int to AppRole enum
                if (role === 1) {
                    appCore.setRole(ApplicationState.RoleSource)
                } else if (role === 2) {
                    appCore.setRole(ApplicationState.RoleDestination)
                }
                stackView.push(discoveryView)
                updateStages()
            }
        }
    }

    Component {
        id: discoveryView
        DiscoveryView {
            // Discovery logic handled in C++ Application
        }
    }

    Component {
        id: pairingView
        PairingView {
            onPairingConfirmed: {
                console.log("Pairing confirmed")
                appCore.onPairingCompleted()
                updateStages()
            }
            onPairingCancelled: {
                console.log("Pairing cancelled")
                stackView.pop()
                updateStages()
            }
        }
    }

    Component {
        id: scanningView
        ScanningView {
            onScanComplete: {
                console.log("Scanning complete")
                updateStages()
            }
        }
    }

    Component {
        id: categorySelectionView
        CategorySelectionView {
            onCategoriesSelected: {
                console.log("Categories selected")
                appCore.startTransfer()
                updateStages()
            }
        }
    }

    Component {
        id: transferView
        TransferView {
            onTransferCancelled: {
                console.log("Transfer cancelled")
                appCore.cancelTransfer()
            }
        }
    }

    Component {
        id: completionView
        CompletionView {
            onFinished: {
                console.log("Finish clicked")
                Qt.quit()
            }
        }
    }

    // Listen for stage changes from C++
    Connections {
        target: appState
        function onStageChanged() {
            console.log("Stage changed to:", appState.stage)
            updateStages()

            // Auto-navigate to the appropriate view
            switch(appState.stage) {
                case 3: // StagePairing
                    console.log("Navigating to PairingView")
                    stackView.push(pairingView)
                    break
                case 4: // StageScanning
                    console.log("Navigating to ScanningView")
                    stackView.push(scanningView)
                    appCore.startScanning()
                    break
                case 5: // StageCategorySelection
                    console.log("Navigating to CategorySelectionView")
                    stackView.push(categorySelectionView)
                    break
                case 6: // StageTransfer
                    console.log("Navigating to TransferView")
                    stackView.push(transferView)
                    break
                case 7: // StageComplete
                    console.log("Navigating to CompletionView")
                    stackView.push(completionView)
                    break
            }
        }
    }

    // Listen for transfer completion
    Connections {
        target: appCore
        function onTransferCompleted() {
            console.log("Transfer completed!")
            appState.setStage(7) // StageComplete
        }
    }

    // Initialize
    Component.onCompleted: {
        console.log("Transporti UI initialized")
        updateStages()
    }
}
