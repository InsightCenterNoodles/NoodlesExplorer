import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Qt5Compat.GraphicalEffects

import Qt.labs.qmlmodels 1.0
import Qt.labs.settings 1.1

ApplicationWindow {
    id: window
    width: 800
    height: 600
    visible: true
    title: qsTr("Noodles Explorer")
    color: "transparent"

    //flags: Qt.FramelessWindowHint

    // for now
    property var current_object : null

    Settings {
        id: settings
        property alias x: window.x
        property alias y: window.y
        property alias width: window.width
        property alias height: window.height

        property string last_user: app_state.get_hostname()
        property string last_server: "ws://localhost:50000"

        property bool show_grid: true
        property color clear_color: "#111111"
        property bool override_lights: true
    }
    FontLoader {
        id: icon_solid
        source: "qrc:/fa_solid_6.otf"
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

    SceneViewComponent {
        id: main_content
        anchors.fill: parent
        //anchors.margins: 5
    }

    SideDrawer {
        id: drawer

        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: parent.left

        anchors.leftMargin: 0

        states: State {
            name: "hidden"
            PropertyChanges {
                target: drawer
                anchors.leftMargin: -drawer.width
                opacity: 0
            }
        }

        transitions: Transition {
            from: ""
            to: "hidden"
            reversible: true
            ParallelAnimation {
                NumberAnimation {
                    properties: "anchors.leftMargin,opacity"
                    duration: 500
                    easing.type: Easing.InOutQuad
                }
            }
        }
    }

    Console {
        id: main_console
        visible: false

        anchors.right: parent.right
        anchors.left: drawer.right
        anchors.bottom: parent.bottom

        height: 200
    }

    ObjectInfoBox {
        id: object_info_box
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.margins: 5

        state: "hidden"

        states: [
            State {
                name: "visible"
                PropertyChanges {
                    target: object_info_box
                    opacity: 1
                }
                PropertyChanges {
                    target: object_info_box
                    visible: true
                }
            },

            State {
                name: "hidden"
                PropertyChanges {
                    target: object_info_box
                    opacity: 0
                }
                PropertyChanges {
                    target: object_info_box
                    visible: false
                }
            }
        ]
        transitions: [
            Transition {
                from: "visible"
                to: "hidden"
                reversible: true
                SequentialAnimation {
                    NumberAnimation {
                        target: object_info_box
                        properties: "opacity"
                        duration: 500
                        easing.type: Easing.InOutQuad
                    }
                    NumberAnimation {
                        target: object_info_box
                        properties: "visible"
                        duration: 0
                    }
                }
            },
            Transition {
                from: "hidden"
                to: "visible"
                reversible: true
                SequentialAnimation {
                    NumberAnimation {
                        target: object_info_box
                        properties: "visible"
                        duration: 0
                    }
                    NumberAnimation {
                        target: object_info_box
                        properties: "opacity"
                        duration: 500
                        easing.type: Easing.InOutQuad
                    }
                }
            }
        ]
    }
}
