import QtQuick 2.15
import "../styles"

Rectangle {
    anchors.fill: parent

    gradient: Gradient {
        orientation: Gradient.Vertical
        GradientStop { position: 0.0; color: AquaTheme.backgroundTop }
        GradientStop { position: 1.0; color: AquaTheme.backgroundBottom }
    }

    // Subtle texture overlay (optional)
    // Uncomment when texture image is available
    /*
    Image {
        anchors.fill: parent
        source: "qrc:/resources/images/backgrounds/aqua-texture.png"
        fillMode: Image.Tile
        opacity: 0.03
    }
    */
}
