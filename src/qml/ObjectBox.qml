import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import Qt.labs.qmlmodels 1.0

NSGroupBox {

    //title: "Objects"
    StackView {
        id: object_view_stack
        anchors.fill: parent
        initialItem: object_tree_component

        clip: true
    }

    Component {
        id: object_tree_component

        ColumnLayout {
            NSTextField {
                id: filter_obj_input
                Layout.fillWidth: true

                placeholderText: "Name or #tag"

                Binding {
                    target: filtered_object_list
                    property: "filter"
                    value: filter_obj_input.text
                }
            }

            ListView {
                id: document_obj_view

                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.margins: 0

                model: filtered_object_list

                clip: true

                delegate: Item {
                    id: obj_view_delegate

                    height: obj_name_label.implicitHeight * 1.5
                    width: document_table_view.width
                    Label {
                        id: obj_name_label
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
                        color: Style.grey4
                    }

                    MouseArea {
                        id: obj_delegate_mouse_area
                        hoverEnabled: true
                        anchors.fill: parent

                        onClicked: {
                            object_view_stack.push(object_method_pane, {
                                                       "text": name,
                                                       "model": methods
                                                   })
                        }
                    }
                }
            }
        }
    }

    Component {
        id: object_method_pane

        ColumnLayout {

            property alias text: current_obj_label.text
            property alias model: object_method_view.model

            Row {
                Layout.fillWidth: true

                NSRoundedButton {
                    text: "\uf104"
                    onClicked: object_view_stack.pop()
                }

                Label {
                    id: current_obj_label
                    verticalAlignment: Text.AlignVCenter
                    height: parent.height
                }
            }

            ListView {
                id: object_method_view
                Layout.fillWidth: true
                Layout.fillHeight: true

                //model:
                delegate: Item {
                    id: method_delegate

                    height: method_name_label.implicitHeight * 1.5
                    width: object_method_view.width
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
                        color: Style.grey4
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
                            object_method_view.model.ask_call(index)
                        }
                    }
                }
            }
        }
    }
}
