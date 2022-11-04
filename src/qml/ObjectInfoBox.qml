import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    radius: 10

    width: 250
    height: 250

    color: Style.set_alpha(Style.grey4, .75)

    Rectangle {
        id: title_bar
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right

        radius: 10

        color: Style.set_alpha(Style.grey6, .75)

        height: info_name.implicitHeight

        Label {
            id: info_name
            anchors.fill: parent
            anchors.leftMargin: 5
            text: window.current_object.item_info.name
        }
    }

    ColumnLayout {
        anchors.top: title_bar.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom


        NSRoundedButton {
            id: move_button

            visible: window.current_object.item_info.movable

            text: "\uf256"

            //checkable: true

            onClicked: window.current_object.item_info.model.pointer.start_move()

            Shortcut {
                sequence: "G"
                onActivated: window.current_object.item_info.model.pointer.start_move()
            }
        }

        ListView {
            id: object_method_view

            Layout.fillHeight: true
            Layout.fillWidth: true


            model: window.current_object.item_info.methods
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
