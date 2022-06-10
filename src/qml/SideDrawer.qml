import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import Qt5Compat.GraphicalEffects

Item {
    id: root
    readonly property int optimal_drawer_size: 250

    width: {
        if (window.width < optimal_drawer_size) {
            return window.width * .33
        }

        return optimal_drawer_size
    }

    Rectangle {
        id: drawer

        anchors.fill: parent
        anchors.margins: 5

        radius: 5

        opacity: 1

        color: Style.set_alpha(Style.grey3, .6)

        property bool has_mouse: hover_area.hovered

        HoverHandler {
            id: hover_area
        }

        DrawerContent {
            id: drawer_content
            anchors.fill: parent
            anchors.margins: 6
        }
    }

    //    DropShadow {
    //        enabled: hover_area.hovered
    //        anchors.fill: drawer
    //        radius: 10
    //        source: drawer
    //        //color: "red"
    //        //cached: true
    //        spread: .2
    //        horizontalOffset: 0
    //        verticalOffset: 0
    //        transparentBorder: true
    //        clip: true
    //        opacity: drawer.opacity
    //    }
}
