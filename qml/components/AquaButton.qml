import QtQuick 2.15
import QtQuick.Controls 2.15
import QtGraphicalEffects 1.15
import "../styles"

Button {
    id: control

    implicitWidth: 160
    implicitHeight: AquaTheme.buttonHeight

    property bool primary: false

    background: Rectangle {
        id: buttonBackground
        radius: AquaTheme.borderRadius

        gradient: Gradient {
            GradientStop {
                position: 0.0
                color: control.pressed ? AquaTheme.buttonPressedTop : AquaTheme.buttonTop
            }
            GradientStop {
                position: 0.5
                color: control.pressed ? AquaTheme.buttonPressedMiddle : AquaTheme.buttonMiddle
            }
            GradientStop {
                position: 1.0
                color: control.pressed ? AquaTheme.buttonPressedBottom : AquaTheme.buttonBottom
            }
        }

        border.color: AquaTheme.buttonBorder
        border.width: 1

        // Glossy highlight on top half
        Rectangle {
            anchors.fill: parent
            anchors.margins: 1
            anchors.bottomMargin: parent.height / 2
            radius: parent.radius
            gradient: Gradient {
                GradientStop { position: 0.0; color: "#60FFFFFF" }
                GradientStop { position: 1.0; color: "#00FFFFFF" }
            }
        }

        // Drop shadow layer
        layer.enabled: true
        layer.effect: DropShadow {
            horizontalOffset: 0
            verticalOffset: 2
            radius: 8
            samples: 16
            color: AquaTheme.shadowColor
            transparentBorder: true
        }

        // Hover effect
        Behavior on border.color {
            ColorAnimation { duration: AquaTheme.animationFast }
        }
    }

    contentItem: Text {
        text: control.text
        font.family: AquaTheme.fontFamily
        font.pixelSize: AquaTheme.fontSizeNormal
        font.weight: Font.Medium
        color: AquaTheme.textPrimary
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
    }

    // Smooth press animation
    scale: pressed ? 0.98 : 1.0
    Behavior on scale {
        NumberAnimation { duration: AquaTheme.animationFast }
    }
}
