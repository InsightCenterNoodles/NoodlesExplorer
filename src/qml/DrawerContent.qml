import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import Qt.labs.qmlmodels 1.0

ColumnLayout {
    RowLayout {
        Layout.fillWidth: true
        Layout.fillHeight: true
        NSRoundedButton {
            text: "\uf6ff"

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
        NSRoundedButton {
            text: "\uf188"

            onClicked: {
                //renderer.showDebugOverlay = true
                app_state.exec_debug()
            }
        }

        NSRoundedButton {
            text: "\uf1de"

            onClicked: settings_pop.open()

            SettingsPopup {
                id: settings_pop
            }
        }

        Item {
            Layout.fillWidth: true
        }
        NSRoundedButton {
            id: pin_button
            text: "\uf08d"
            checkable: true
            //onToggled: window.allow_mouse = !checked

            //Component.onCompleted: window.allow_mouse = !checked
        }

        NSRoundedButton {
            text: "\uf054"
            //enabled: !pin_button.checked
            //            onClicked: if (!pin_button.checked)
            //                           drawer.close()
        }
    }

    ObjectBox {
        Layout.fillHeight: true
        Layout.fillWidth: true
    }

    NSGroupBox {
        title: "Tables"

        Layout.fillWidth: true
        Layout.preferredHeight: 200

        Item {
            //Material.elevation: 3
            anchors.fill: parent
            anchors.margins: 6

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
                        //color: Material.frameColor
                    }

                    Label {
                        z: parent.z + 1
                        visible: table_delegate_mouse_area.containsMouse
                        anchors.right: table_label.left
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom
                        anchors.margins: 3
                        width: height

                        text: "\uf080"
                        font.family: icon_solid.name
                        font.styleName: "Solid"

                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                app_state.launch_chart_view(index)
                            }
                        }
                    }

                    Label {
                        id: table_label
                        z: parent.z + 1
                        visible: table_delegate_mouse_area.containsMouse
                        anchors.right: parent.right
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom
                        anchors.margins: 3
                        width: height

                        text: "\uf0ce"
                        font.family: icon_solid.name
                        font.styleName: "Solid"

                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                app_state.launch_table_view(index)
                            }
                        }
                    }

                    MouseArea {
                        id: table_delegate_mouse_area
                        hoverEnabled: true
                        anchors.fill: parent
                    }
                }
            }
        }
    }

    NSGroupBox {
        Layout.fillWidth: true
        Layout.preferredHeight: 200

        title: "Methods"

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
                    //color: Material.frameColor
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
                        document_method_view.model.ask_call(index)
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

