import QtQuick 2.15
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import QtQuick.Layouts 1.12

import Qt.labs.qmlmodels 1.0

Drawer {
    id: drawer

    readonly property int optimal_drawer_size: 250

    property bool pinned: false

    interactive: pinned ? false : true
    modal: pinned ? false : true

    width: {
        if (window.width < optimal_drawer_size) {
            return window.width * .33
        }

        return optimal_drawer_size
    }

    height: window.height
    edge: Qt.RightEdge

    dim: false

    background: Rectangle {
        color: Material.backgroundColor
        opacity: .75
        Rectangle {
            x: -1
            width: 1
            height: parent.height
            color: Material.frameColor
        }
    }

    DrawerContent {
        id: drawer_content
        anchors.fill: parent
        anchors.margins: 6
    }
}
