import QtQuick 2.15
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import QtQuick.Layouts 1.12

Popup {
    id: settings_pop

    margins: 0

    GridLayout {
        columns: 2

        Label {
            text: "Orbit:"

            Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
        }

        Switch {
            id: orbit_setting

            Binding {
                target: orbit_setting
                property: "checked"
                value: settings.orbit_cam
            }

            onCheckedChanged: settings.orbit_cam = checked
        }

        Label {
            text: "Background:"
        }

        Rectangle {
            id: color_well
            color: settings.clear_color
            border.width: 1
            border.color: Material.primaryColor

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

