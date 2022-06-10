import QtQuick
import QtQuick.Controls 2.15

Button {
    id: control

    padding: 6
    horizontalPadding: padding + 2
    spacing: 6

    icon.width: 24
    icon.height: 24

    background: Rectangle {
        implicitWidth: contentItem.implicitWidth
        implicitHeight: contentItem.implicitHeight
        visible: !control.flat || control.down || control.checked
                 || control.highlighted

        color: {
            if (control.down)
                return Style.grey
            if (control.hovered)
                return Style.button_background_highlight
            return Style.button_background
        }

        radius: 5
    }
}
