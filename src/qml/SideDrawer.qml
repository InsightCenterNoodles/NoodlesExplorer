import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    id: drawer

    radius: 5
    opacity: .75

    color: Style.grey4

    readonly property int optimal_drawer_size: 250

    property bool pinned: false

    //interactive: pinned ? false : true
    //modal: pinned ? false : true
    width: {
        if (window.width < optimal_drawer_size) {
            return window.width * .33
        }

        return optimal_drawer_size
    }

    //height: window.height
    //edge: Qt.RightEdge

    //dim: false

    //    background: Rectangle {
    //        //color: Material.backgroundColor
    //        opacity: .75
    //        Rectangle {
    //            x: -1
    //            width: 1
    //            height: parent.height
    //            //color: Material.frameColor
    //        }
    //    }
    //    DrawerContent {
    //        id: drawer_content
    //        anchors.fill: parent
    //        anchors.margins: 6
    //    }
}
