import QtQuick
import QtQuick.Controls
//import QtQuick.Controls.Material
import QtQuick.Layouts

import Qt.labs.qmlmodels 1.0
import Qt.labs.settings 1.1

CustomRect {
    id: title_dragger
    radius: 5

    topLeftCorner: false
    bottomLeftCorner: false
    topRightCorner: true
    bottomRightCorner: false

    color: Style.set_alpha(Style.grey3, .75)

    height: 35

    property bool has_mouse: window_drag_area.containsMouse
                             || window_drag_area.pressed

    MouseArea {
        id: window_drag_area
        anchors.fill: parent

        property int dx
        property int dy
        onPressed: {
            dx = mouseX
            dy = mouseY
        }

        onPositionChanged: {
            window.x += mouseX - dx
            window.y += mouseY - dy
        }
    }

    WindowControls {
        id: collapsed_controls
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        visible: drawer.state === "hidden"

        width: drawer.state === "hidden" ? 65 : 0
    }

    NSRoundedButton {
        id: close_sidebar_button
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: collapsed_controls.right

        text: drawer.state === "hidden" ? "\uf054" : "\uf053"

        onClicked: {
            console.log("HI")
            if (drawer.state)
                drawer.state = ""
            else
                drawer.state = "hidden"
        }
    }

    Label {
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: close_sidebar_button.right
        text: window.title

        font.bold: true
    }

    Row {
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.right

        NSRoundedButton {
            text: "\uf05a"

            onToggled: object_info_box.state = checked ? "" : "hidden"

            checkable: true
        }

        NSRoundedButton {
            text: "\uf2c2"

            onClicked: {
                inspector_popup.open()
            }
        }

        NSRoundedButton {

            text: "\uf1de"

            onClicked: settings_pop.open()

            SettingsPopup {
                id: settings_pop
            }
        }
    }
}
