import QtQuick 2.15
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import QtQuick.Layouts 1.12

import Qt.labs.qmlmodels 1.0
import Qt.labs.settings 1.1

ApplicationWindow {
    id: window
    width: 800
    height: 600
    visible: true
    title: qsTr("Noodles Viewer")

    Component.onCompleted: {
        app_state.onOpen_call_method_page.connect(function () {
            call_method_pop.open()
        })

        app_state.onMethod_result_recieved.connect(function (s) {
            message_recv_dialog.text = s
            message_recv_dialog.open()
        })

        app_state.onMethod_error_recieved.connect(function (s) {
            message_recv_dialog.text = "Error! " + s
            message_recv_dialog.open()
        })
    }

    Settings {
        id: settings
        property alias x: window.x
        property alias y: window.y
        property alias width: window.width
        property alias height: window.height

        property string last_user: app_state.get_hostname()
        property string last_server: "ws://localhost:50000"

        property bool orbit_cam: true
        property color clear_color: "#111111"
    }

    FontLoader {
        id: icon_regular
        source: "qrc:/fa-regular-400.ttf"
    }
    FontLoader {
        id: icon_solid
        source: "qrc:/fa-solid-900.ttf"
    }

    SideDrawer {
        id: drawer
    }

    Popup {
        id: connection_pop

        modal: true
        dim: true

        x: Math.round((parent.width - width) / 2)
        y: Math.round((parent.height - height) / 2)

        width: parent.width * .75

        closePolicy: Popup.NoAutoClose

        ColumnLayout {
            anchors.right: parent.right
            anchors.left: parent.left
            GridLayout {
                rows: 2
                columns: 2

                Layout.fillWidth: true

                Label {
                    text: "Client Name:"
                    Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                }

                TextField {
                    id: username_box
                    placeholderText: "username"
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                    Layout.fillWidth: true

                    text: settings.last_user
                }

                Label {
                    text: "Server Address:"
                    Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                }

                TextField {
                    id: hostname_box
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                    Layout.fillWidth: true

                    text: settings.last_server

                    validator: RegExpValidator {
                        regExp: /^(ws|wss):\/\/[^ "]+$/
                    }
                }
            }
            RowLayout {
                Layout.fillWidth: true
                Item {
                    Layout.fillWidth: true
                }

                Button {
                    text: "Cancel"

                    Material.background: Material.Pink
                    onClicked: connection_pop.close()
                }
                Button {
                    text: "Connect"

                    onClicked: {

                        if (!hostname_box.acceptableInput) {
                            return
                        }

                        console.log(username_box.text, hostname_box.text)

                        if (app_state.start_connection(username_box.text,
                                                       hostname_box.text)) {
                            connection_pop.close()
                        }

                        settings.last_user = username_box.text
                        settings.last_server = hostname_box.text
                    }
                }
            }
        }
    }

    Popup {
        id: call_method_pop
        modal: true

        x: Math.round((parent.width - width) / 2)
        y: Math.round((parent.height - height) / 2)
        width: window.width * .9

        //height: window.height * .9
        MethodCallUI {
            anchors.fill: parent
        }
    }

    Dialog {
        id: message_recv_dialog
        title: "Message Result"
        standardButtons: Dialog.Ok

        parent: Overlay.overlay

        x: Math.round((parent.width - width) / 2)
        y: Math.round((parent.height - height) / 2)

        property alias text: result_label.text

        Label {
            id: result_label
            text: "Result"
        }
    }

    InspectorPopup {
        id: inspector_popup
        parent: Overlay.overlay
    }

    Item {
        id: main_content
        anchors.fill: parent

        anchors.rightMargin: drawer.pinned ? drawer.width : 0

        SceneViewComponent {
            anchors.fill: parent
        }

        ToolButton {
            text: "\uf013"
            font.family: icon_solid.name
            font.styleName: "Solid"

            visible: !drawer.visible

            onClicked: drawer.visible ? drawer.close() : drawer.open()

            anchors.top: parent.top
            anchors.right: parent.right

            Material.foreground: settings.clear_color.hslLightness > .5 ? "black" : "white"
        }

        //        Image {
        //            anchors.fill: parent
        //            source: "qrc:/img/test.png"
        //            mipmap: true
        //        }
    }
}
