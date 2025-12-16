import QtQuick 2.15
import QtQuick3D 1.15

Model {
    id: cube

    source: "#Cube"
    scale: Qt.vector3d(0.4, 0.4, 0.4)

    materials: [ PrincipledMaterial {
        baseColor: Qt.rgba(0.3, 0.6, 1.0, 0.7)
        metalness: 0.2
        roughness: 0.1
        opacity: 0.7
        alphaMode: PrincipledMaterial.Blend
    }]

    // Upward movement animation
    NumberAnimation on y {
        from: cube.y
        to: 200
        duration: 2000
        easing.type: Easing.OutQuad
    }

    // Fade out animation
    NumberAnimation on opacity {
        from: 1.0
        to: 0.0
        duration: 2000
    }

    // Rotation animation
    SequentialAnimation on eulerRotation.y {
        running: true
        loops: 1
        NumberAnimation { from: 0; to: 360; duration: 3000 }
    }

    SequentialAnimation on eulerRotation.x {
        running: true
        loops: 1
        NumberAnimation { from: 0; to: 180; duration: 3000 }
    }

    // Self-destruct after animation
    Timer {
        interval: 2500
        running: true
        repeat: false
        onTriggered: {
            cube.destroy()
        }
    }
}
