import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    id: drawer

    radius: 5

    opacity: hover_area.hovered ? 1 : .75

    color: Style.set_alpha(Style.grey3, .75)

    property bool has_mouse: hover_area.hovered

    readonly property int optimal_drawer_size: 250

    width: {
        if (window.width < optimal_drawer_size) {
            return window.width * .33
        }

        return optimal_drawer_size
    }

    HoverHandler {
        id: hover_area
    }

    DrawerContent {
        id: drawer_content
        anchors.fill: parent
        anchors.margins: 6
    }
}
