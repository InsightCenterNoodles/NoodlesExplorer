import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts

ColumnLayout {
    Label {
        text: argument_model.method_documentation
        Layout.fillWidth: true
    }

    RowLayout {
        property string method_name: "load_table"
        property string method_doc: "This loads a table"

        Label {
            id: method_name_label
            text: argument_model.method_name + "("
            font.pointSize: 18
        }

        Row {
            spacing: 12
            Repeater {
                id: arg_view

                model: argument_model

                Row {
                    height: method_name_label.height + 6
                    width: arg_label.implicitWidth + arg_spacer.implicitWidth + 6

                    Rectangle {
                        height: parent.height
                        width: parent.width

                        color: Material.backgroundColor

                        border.width: 1
                        border.color: edited ? Material.color(
                                                   Material.Green) : Material.color(
                                                   Material.Red)

                        radius: 5

                        Label {
                            id: arg_label
                            text: edited ? name + ": " + clean_text(
                                               current_value) : name

                            function clean_text(t) {
                                return t.substring(0, 8).replace(/[\n\r]/g, '')
                            }

                            font.pointSize: 16
                            anchors.horizontalCenter: parent.horizontalCenter
                            anchors.verticalCenter: parent.verticalCenter
                        }

                        MouseArea {
                            anchors.fill: parent

                            onClicked: edit_arg_pop.open()
                        }

                        Dialog {
                            id: edit_arg_pop
                            modal: true
                            standardButtons: arg_editor_fld.valid_json ? Dialog.Save | Dialog.Cancel : Dialog.Cancel
                            title: "Edit Argument <i>" + name + "</i>"

                            parent: Overlay.overlay

                            x: Math.round((parent.width - width) / 2)
                            y: Math.round((parent.height - height) / 2)

                            width: window.width * .6
                            height: window.height * .6

                            onAccepted: {
                                current_value = arg_editor_fld.text
                                edited = true
                            }

                            Row {
                                anchors.fill: parent
                                spacing: 6

                                GroupBox {
                                    title: "Argument Documentation"

                                    width: parent.width / 2
                                    anchors.top: parent.top
                                    anchors.bottom: parent.bottom

                                    TextArea {
                                        readOnly: true

                                        text: documentation

                                        anchors.fill: parent
                                        background: Item {}

                                        wrapMode: TextEdit.WordWrap
                                    }
                                }

                                ColumnLayout {
                                    width: parent.width / 2

                                    anchors.top: parent.top
                                    anchors.bottom: parent.bottom

                                    GroupBox {
                                        title: "Argument Editor"

                                        Layout.fillWidth: true
                                        Layout.fillHeight: true
                                        TextArea {
                                            id: arg_editor_fld

                                            text: current_value
                                            anchors.fill: parent
                                            background: Item {}

                                            selectByKeyboard: true
                                            selectByMouse: true

                                            onTextChanged: {
                                                try {
                                                    let obj = JSON.parse(text)
                                                    valid_json = true
                                                } catch (e) {
                                                    valid_json = false
                                                }
                                            }

                                            property bool valid_json: current_value
                                        }
                                    }

                                    Label {
                                        id: json_edit_label
                                        text: arg_editor_fld.valid_json ? "OK" : "Parse Error"
                                        font.bold: !arg_editor_fld
                                        Material.foreground: arg_editor_fld.valid_json ? Material.color(Material.Green) : Material.color(Material.Red)
                                    }
                                }
                            }
                        }
                    }

                    Label {
                        id: arg_spacer
                        text: ","
                        font.pointSize: 16
                        visible: index !== argument_model.rowCount() - 1
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }
            }
        }

        Label {
            text: ");"
            font.pointSize: 18
            Layout.fillWidth: true
        }

        Button {
            text: "Invoke"
            onClicked: {
                app_state.invoke_doc_method()
                call_method_pop.close()
            }
        }
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:400;width:600}
}
##^##*/

