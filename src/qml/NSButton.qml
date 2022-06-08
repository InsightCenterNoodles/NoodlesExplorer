import QtQuick
import QtQuick.Controls 2.15

Button {
    id: control

    padding: 6
    horizontalPadding: padding + 2
    spacing: 6

    icon.width: 24
    icon.height: 24
    icon.color: control.checked
                || control.highlighted ? control.palette.brightText : control.flat
                                         && !control.down ? (control.visualFocus ? control.palette.highlight : control.palette.windowText) : control.palette.buttonText

    contentItem: Label {
        text: control.text
        font: control.font
        color: control.checked
               || control.highlighted ? control.palette.brightText : control.flat
                                        && !control.down ? (control.visualFocus ? control.palette.highlight : control.palette.windowText) : control.palette.buttonText
    }

    background: Rectangle {
        implicitWidth: 100
        implicitHeight: 40
        visible: !control.flat || control.down || control.checked
                 || control.highlighted

        /*
        color: Color.blend(
                   control.checked
                   || control.highlighted ? Style.button_background_highlight : Style.button_background,
                   control.palette.mid, control.down ? 0.5 : 0.0)
                   */
        color: Style.button_background
    }
}
