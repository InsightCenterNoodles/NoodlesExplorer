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
        anchors.fill: parent
        anchors.margins: 0

        Rectangle {
            width: 1
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.left: parent.right

            color: "#000000"
            border.width: 0
        }

        color: Style.set_alpha(Style.grey3, .75)

        property bool has_mouse: hover_area.hovered

        HoverHandler {
            id: hover_area
        }

        RowLayout {
            id: primary_button_layout
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.leftMargin: 5

            height: 35

            NSRoundedButton {
                id: start_conn_button
                text: {
                    let s = app_state.connection_state
                    switch (s) {
                    case -1:
                        return "\ue560"
                    case 0:
                        return "\uf1e6"
                    case 1:
                        return "\ue55b"
                    }
                }

                onClicked: app_state.disconnect()
            }

//            NSRoundedButton {
//                text: "\uf188"

//                onClicked: {
//                    app_state.exec_debug()
//                }
//            }

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

        DrawerContent {
            id: drawer_content
            anchors.top: primary_button_layout.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.margins: 6
        }
    }
}
