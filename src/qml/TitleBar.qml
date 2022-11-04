import QtQuick
import QtQuick.Controls
//import QtQuick.Controls.Material
import QtQuick.Layouts

import Qt.labs.qmlmodels 1.0
import Qt.labs.settings 1.1

Rectangle {
    id: title_dragger

    color: Style.set_alpha(Style.grey3, .75)

    height: 35

    NSRoundedButton {
        id: close_sidebar_button
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.leftMargin: 5

        text: drawer.state === "hidden" ? "\uf054" : "\uf053"

        onClicked: {
            if (drawer.state)
                drawer.state = "visible"
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
