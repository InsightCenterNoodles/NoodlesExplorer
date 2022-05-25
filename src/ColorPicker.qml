import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts

Dialog {
    id: color_pick_pop
    property color picked_color: {
        return Qt.rgba(r_slider.value / 255., g_slider.value / 255.,
                       b_slider.value / 255., 1)
    }

    property var on_ok: function () {}

    function issue(call_f, c) {
        on_ok = call_f
        set_color(c)
        open()
    }

    function set_color(c) {
        //console.log(c.r, c.g, c.b)
        r_slider.value = c.r * 255
        g_slider.value = c.g * 255
        b_slider.value = c.b * 255
    }

    onAccepted: on_ok(picked_color)

    standardButtons: Dialog.Ok | Dialog.Cancel

    title: "Pick Color"

    GridLayout {
        columns: 3

        Rectangle {
            color: color_pick_pop.picked_color
            border.width: 1
            border.color: Material.primaryColor

            //Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.preferredHeight: 20

            //Layout.preferredWidth: 50
            Layout.columnSpan: 3
            radius: 5
        }

        Label {
            text: "R:"
        }
        Slider {
            id: r_slider
            from: 0
            to: 255
            value: 255
            stepSize: 1
        }
        Label {
            text: r_slider.value.toPrecision(3)
        }

        Label {
            text: "G:"
        }
        Slider {
            id: g_slider
            from: 0
            to: 255
            value: 255
            stepSize: 1
        }
        Label {
            text: g_slider.value.toPrecision(3)
        }

        Label {
            text: "B:"
        }
        Slider {
            id: b_slider
            from: 0
            to: 255
            value: 255
            stepSize: 1
        }
        Label {
            text: b_slider.value.toPrecision(3)
        }

        Item {
            Layout.fillWidth: true
            Layout.preferredHeight: 20
            Layout.columnSpan: 3

            RowLayout {
                anchors.fill: parent

                Rectangle {
                    Layout.fillHeight: true
                    Layout.fillWidth: true

                    color: "black"

                    radius: 5

                    MouseArea {
                        anchors.fill: parent
                        onClicked: set_color(Qt.rgba(0, 0, 0))
                    }
                }

                Rectangle {
                    Layout.fillHeight: true
                    Layout.fillWidth: true

                    color: Qt.rgba(.5, .5, .5)

                    radius: 5

                    MouseArea {
                        anchors.fill: parent
                        onClicked: set_color(Qt.rgba(.5, .5, .5))
                    }
                }

                Rectangle {
                    Layout.fillHeight: true
                    Layout.fillWidth: true

                    color: "white"

                    radius: 5

                    MouseArea {
                        anchors.fill: parent
                        onClicked: set_color(Qt.rgba(1, 1, 1))
                    }
                }
            }
        }
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/

