import QtQuick
import QtQuick.Controls

Rectangle {
    id: control
    radius: 5

    opacity: hover_area.hovered ? 1 : .75

    color: Style.set_alpha(Style.grey3, .6)

    height: 25
    width: 65

    property bool has_mouse: hover_area.hovered

    HoverHandler {
        id: hover_area
    }

    Row {
        id: button_row
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        anchors.leftMargin: 5

        spacing: 5

        Rectangle {
            color: Style.red

            width: 14
            height: 14
            radius: 7

            MouseArea {
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
                anchors.fill: parent
                onClicked: window.visibility = window.visibility
                           === Window.FullScreen ? Window.AutomaticVisibility : Window.FullScreen
            }
        }
    }
}
