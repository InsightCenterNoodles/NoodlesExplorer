import QtQuick
import QtQuick.Controls
//import QtQuick.Controls.Material
import QtQuick.Layouts

import Qt.labs.qmlmodels 1.0
import Qt.labs.settings 1.1

ApplicationWindow {
    id: window
    width: 800
    height: 600
    visible: true
    title: qsTr("Noodles Viewer")
    color: "transparent"

    // hacky, but needed to deal with bugs in WASDcontroller
    property bool allow_wasd_mouse: !(title_dragger.has_mouse
                                      || drawer.has_mouse)

    flags: Qt.FramelessWindowHint

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

    Rectangle {
        id: window_bezel

        radius: 5
        color: settings.clear_color

        anchors.fill: parent
        anchors.margins: 5

        SceneViewComponent {
            id: main_content
            anchors.fill: parent
            anchors.margins: 5
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

        TitleBar {
            id: title_dragger
            anchors.left: drawer.right
            anchors.leftMargin: drawer.state === "hidden" ? 0 : 1
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.topMargin: 0

            topLeftCorner: drawer.state === "hidden"
        }

        ObjectInfoBox {
            id: object_info_box
            anchors.right: parent.right
            anchors.top: title_dragger.bottom
            anchors.margins: 5

            state: "hidden"

            states: State {
                name: "hidden"
                PropertyChanges {
                    target: object_info_box
                    //anchors.leftMargin: -drawer.width
                    opacity: 0
                }
            }

            transitions: Transition {
                from: ""
                to: "hidden"
                reversible: true
                ParallelAnimation {
                    NumberAnimation {
                        //properties: "anchors.leftMargin,opacity"
                        properties: "opacity"
                        duration: 500
                        easing.type: Easing.InOutQuad
                    }
                }
            }
        }
    }

    MouseArea {
        anchors.right: parent.right
        anchors.bottom: parent.bottom

        width: 16
        height: width

        onPressed: {
            let resize = window.startSystemResize(Qt.RightEdge | Qt.BottomEdge)
            console.log(resize)
        }
    }
}
