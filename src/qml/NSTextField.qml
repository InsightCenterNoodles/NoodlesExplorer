import QtQuick 2.0

import QtQuick.Controls

TextField {
    id: control

    background: Rectangle {
        implicitWidth: 200
        implicitHeight: 25
        radius: 5
        color: control.enabled ? Style.set_alpha(Style.grey3,
                                                 .75) : Style.set_alpha(
                                     Style.disabled_color, .75)
        border.color: Style.grey2
        border.width: 1
    }
}
