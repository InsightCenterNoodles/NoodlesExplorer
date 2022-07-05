import QtQuick
import QtQuick.Controls

Item {
    id: control

    height: 25
    width: 65

    Row {
        id: button_row
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        anchors.leftMargin: 5

        spacing: 5

        Rectangle {
            color: maximize_area.pressed ? Qt.lighter(Style.red, 2) : Style.red

            width: 14
            height: 14
            radius: 7

            MouseArea {
                id: close_area
                anchors.fill: parent
                onClicked: Qt.quit()
            }
        }

        Rectangle {
            color: Style.yellow

            width: 14
            height: 14
            radius: 7

            MouseArea {
                id: minimize_area
                anchors.fill: parent
                onClicked: window.visibility = Window.Minimized
            }
        }

        Rectangle {
            color: Style.green

            width: 14
            height: 14
            radius: 7

            MouseArea {
                id: maximize_area
                anchors.fill: parent
                onClicked: window.visibility = window.visibility
                           === Window.FullScreen ? Window.AutomaticVisibility : Window.FullScreen
            }
        }
    }
}
