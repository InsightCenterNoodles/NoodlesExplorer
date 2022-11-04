import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import Qt.labs.qmlmodels 1.0

NSGroupBox {
    //title: "Objects"

    ListView {
        id: document_obj_view

        anchors.fill: parent

        model: filtered_object_list

        clip: true

        Component {
            id: header_search_item
            NSTextField {
                id: filter_obj_input

                width: document_table_view.width

                placeholderText: "Name or #tag"

                Binding {
                    target: filtered_object_list
                    property: "filter"
                    value: filter_obj_input.text
                }
            }
        }

        header: header_search_item

        highlight: Rectangle {
            radius: 5
            color: "#22FFFFFF"
        }

        Binding {
            target: window
            property: "current_object"
            value: document_obj_view.currentItem
        }

        delegate: Item {
            id: obj_view_delegate

            height: obj_name_label.implicitHeight * 1.5
            width: document_table_view.width

            property var item_info: model

            Label {
                id: obj_name_label
                text: name
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 12
                anchors.right: parent.right
                anchors.rightMargin: 6
            }

//            Rectangle {
//                height: 1
//                anchors.right: parent.right
//                anchors.rightMargin: 6
//                anchors.bottom: parent.bottom
//                width: parent.width - 12
//                color: Style.grey4
//            }

            MouseArea {
                anchors.fill: parent
                onClicked: document_obj_view.currentIndex = index
            }
        }
    }
}
