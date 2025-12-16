pragma Singleton
import QtQuick 2.15

QtObject {
    // Colors - Mac OS X Aqua inspired
    readonly property color backgroundTop: "#FFFFFF"
    readonly property color backgroundBottom: "#E8F2FF"
    readonly property color sidebarBackground: "#F0F5FA"
    readonly property color primaryBlue: "#007AFF"
    readonly property color accentBlue: "#5AC8FA"
    readonly property color textPrimary: "#1D1D1F"
    readonly property color textSecondary: "#86868B"
    readonly property color textTertiary: "#C7C7CC"

    // Aqua button gradient colors
    readonly property color buttonTop: "#FFFFFF"
    readonly property color buttonMiddle: "#E0EFFF"
    readonly property color buttonBottom: "#C0DFFF"
    readonly property color buttonBorder: "#8BB8E8"

    // Button pressed state
    readonly property color buttonPressedTop: "#D0E8FF"
    readonly property color buttonPressedMiddle: "#B0D8FF"
    readonly property color buttonPressedBottom: "#A0C8EF"

    // Shadows
    readonly property color shadowColor: "#40000000"
    readonly property color innerShadow: "#20000000"

    // Dimensions
    readonly property int sidebarWidth: 220
    readonly property int buttonHeight: 44
    readonly property int buttonHeightSmall: 32
    readonly property int borderRadius: 8
    readonly property int cardPadding: 24
    readonly property int spacing: 16
    readonly property int spacingSmall: 8
    readonly property int spacingLarge: 32

    // Fonts
    readonly property string fontFamily: "SF Pro Display"  // Fallback to system
    readonly property int fontSizeSmall: 13
    readonly property int fontSizeNormal: 15
    readonly property int fontSizeMedium: 17
    readonly property int fontSizeLarge: 20
    readonly property int fontSizeTitle: 28
    readonly property int fontSizeHuge: 34

    // Animation durations (ms)
    readonly property int animationFast: 150
    readonly property int animationNormal: 250
    readonly property int animationSlow: 400
}
