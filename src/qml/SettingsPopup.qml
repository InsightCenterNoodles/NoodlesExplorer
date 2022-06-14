import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

NSPopup {
    id: settings_pop

    modal: true
    dim: true

    parent: Overlay.overlay

    x: Math.round((parent.width - width) / 2)
    y: Math.round((parent.height - height) / 2)

    margins: 0

    GridLayout {
        columns: 2

        Label {
            Layout.columnSpan: 2
            text: "Settings"
            font.pointSize: 24
        }

        Label {
            text: "Show Grid:"

            Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
        }

        Switch {
            id: show_grid_setting

            Binding {
                target: show_grid_setting
                property: "checked"
                value: settings.show_grid
            }

            onCheckedChanged: settings.show_grid = checked
        }

        Label {
            text: "Background:"

            Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
        }

        Rectangle {
            id: color_well
            color: settings.clear_color
            border.width: 1

            Layout.fillHeight: true
            Layout.fillWidth: true

            radius: 2

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    color_pick_pop.issue(function (c) {
                        color_well.color = c
                        settings.clear_color = c
                    }, color_well.color)
                }
            }
        }

        ColorPicker {
            id: color_pick_pop
            margins: 0
        }
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/

