import QtQuick 2.15
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import QtQuick.Layouts 1.12

import Qt.labs.qmlmodels 1.0

ColumnLayout {
    RowLayout {
        Layout.fillWidth: true
        Layout.fillHeight: true
        ToolButton {
            text: "\uf6ff"
            font.family: icon_solid.name
            font.styleName: "Solid"

            onClicked: conn_menu.open()

            Menu {
                id: conn_menu
                Action {
                    text: "&Connect"
                    onTriggered: connection_pop.open()
                }
                Action {
                    text: "&Inspect"
                    onTriggered: inspector_popup.open()
                }
                Action {
                    text: "&Exit"
                }
            }

            Rectangle {
                id: conn_indicator

                width: 10
                height: 10
                radius: 5

                anchors.right: parent.right
                anchors.bottom: parent.bottom
                anchors.margins: 5

                color: {
                    let s = app_state.connection_state
                    switch (s) {
                    case -1:
                        return "red"
                    case 0:
                        return "grey"
                    case 1:
                        return "green"
                    }
                }
            }
        }
        ToolButton {
            text: "\uf188"
            font.family: icon_solid.name
            font.styleName: "Solid"

            onClicked: {
                renderer.showDebugOverlay = true
                app_state.exec_debug()
            }
        }

        ToolButton {
            text: "\uf1de"
            font.family: icon_solid.name
            font.styleName: "Solid"

            onClicked: settings_pop.open()

            SettingsPopup {
                id: settings_pop
            }
        }

        Item {
            Layout.fillWidth: true
        }
        ToolButton {
            id: pin_button
            text: "\uf08d"
            font.family: icon_solid.name
            font.styleName: "Solid"
            checkable: true
            onToggled: drawer.pinned = checked
        }
        ToolButton {
            text: "\uf054"
            font.family: icon_solid.name
            font.styleName: "Solid"
            enabled: !pin_button.checked
            onClicked: if (!pin_button.checked)
                           drawer.close()
        }
    }

    Item {
        Layout.fillWidth: true
        Layout.fillHeight: true
    }

    Rectangle {
        Layout.fillWidth: true
        Layout.preferredHeight: 200
        Layout.rightMargin: 6

        color: Material.backgroundColor
        opacity: .8
        border.color: Material.frameColor
        border.width: 1
        radius: 5

        GroupBox {
            title: "Tables"

            Material.elevation: 3

            anchors.fill: parent
            anchors.margins: 6

            background: Item {}

            ListView {
                id: document_table_view
                anchors.fill: parent

                model: table_list

                delegate: Item {
                    id: table_view_delegate

                    height: table_name_label.implicitHeight * 1.5
                    width: document_table_view.width
                    Label {
                        id: table_name_label
                        text: name
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.left: parent.left
                        anchors.leftMargin: 12
                        anchors.right: parent.right
                        anchors.rightMargin: 6
                    }

                    Rectangle {
                        height: 1
                        anchors.right: parent.right
                        anchors.rightMargin: 6
                        anchors.bottom: parent.bottom
                        width: parent.width - 12
                        color: Material.frameColor
                    }

                    Label {
                        z: parent.z + 1
                        visible: table_delegate_mouse_area.containsMouse
                        anchors.right: parent.right
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom
                        anchors.margins: 3
                        width: height

                        text: "\uf054"
                        font.family: icon_solid.name
                        font.styleName: "Solid"
                    }

                    MouseArea {
                        id: table_delegate_mouse_area
                        hoverEnabled: true
                        anchors.fill: parent

                        onClicked: {
                            app_state.launch_table_view(index)
                            //call_method_pop.close()
                            //app_state.ask_to_call_doc_method(index)
                        }
                    }
                }
            }
        }
    }

    Rectangle {
        Layout.fillWidth: true
        Layout.preferredHeight: 300
        Layout.rightMargin: 6

        color: Material.backgroundColor
        opacity: .8
        border.color: Material.frameColor
        border.width: 1
        radius: 5

        GroupBox {
            title: "Methods"

            Material.elevation: 3

            anchors.fill: parent
            anchors.margins: 6

            background: Item {}

            ListView {
                id: document_method_view
                anchors.fill: parent

                model: document_methods

                delegate: Item {
                    id: method_delegate

                    height: method_name_label.implicitHeight * 1.5
                    width: document_method_view.width
                    Label {
                        id: method_name_label
                        text: name
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.left: parent.left
                        anchors.leftMargin: 12
                        anchors.right: parent.right
                        anchors.rightMargin: 6
                    }

                    Rectangle {
                        height: 1
                        anchors.right: parent.right
                        anchors.rightMargin: 6
                        anchors.bottom: parent.bottom
                        width: parent.width - 12
                        color: Material.frameColor
                    }

                    Label {
                        z: parent.z + 1
                        visible: delegate_mouse_area.containsMouse
                        anchors.right: parent.right
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom
                        anchors.margins: 3
                        width: height

                        text: "\uf054"
                        font.family: icon_solid.name
                        font.styleName: "Solid"
                    }

                    MouseArea {
                        id: delegate_mouse_area
                        hoverEnabled: true
                        anchors.fill: parent

                        onClicked: {
                            call_method_pop.close()
                            app_state.ask_to_call_doc_method(index)
                        }
                    }
                }
            }
        }
    }
}

/*##^##
Designer {
    D{i:0;height:600;width:300}
}
##^##*/

