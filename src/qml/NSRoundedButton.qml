import QtQuick 2.0
import QtQuick.Controls

RoundButton {
    id: control

    //    FontLoader {
    //        id: icon_solid
    //        source: "qrc:/fa-solid-900.ttf"
    //    }
    font.family: icon_solid.name
    font.styleName: "Solid"
    font.pointSize: Style.icon_text_size

    flat: true

    contentItem: Text {
        text: control.text
        font: control.font
        opacity: enabled ? 1.0 : 0.3
        color: control.down ? Style.secondary_text : Style.primary_text
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }
}
