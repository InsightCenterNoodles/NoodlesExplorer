pragma Singleton

import QtQuick 2.0

Item {
    readonly property color primary_text: "white"
    readonly property color secondary_text: grey
    readonly property color disabled_text: grey4

    readonly property real icon_text_size: 16

    readonly property color button_background_highlight: grey2
    readonly property color button_background: grey3

    readonly property color disabled_color: grey5

    function rgb(r, g, b) {
        return Qt.rgba(r / 255.0, g / 255.0, b / 255.0, 1)
    }

    readonly property color red: rgb(255, 69, 58)
    readonly property color orange: rgb(255, 159, 10)
    readonly property color yellow: rgb(255, 214, 10)
    readonly property color green: rgb(48, 209, 88)

    readonly property color dimgrey: rgb(180,180,180)

    readonly property color grey: rgb(142, 142, 147)
    readonly property color grey2: rgb(99, 99, 102)
    readonly property color grey3: rgb(72, 72, 74)
    readonly property color grey4: rgb(58, 58, 60)
    readonly property color grey5: rgb(44, 44, 46)
    readonly property color grey6: rgb(28, 28, 30)

    function set_alpha(color, alpha) {
        return Qt.hsla(color.hslHue, color.hslSaturation,
                       color.hslLightness, alpha)
    }
}
