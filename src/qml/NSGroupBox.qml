import QtQuick
import QtQuick.Controls

GroupBox {
    id: control

    topInset: title.length === 0 ? 0 : 20
    topPadding: title.length === 0 ? 0 : 20

    background: Rectangle {
        color: "transparent"

        y: control.topPadding - control.padding
        width: parent.width
        height: parent.height - control.topPadding + control.padding

        radius: 5
    }

    label: Label {
        x: control.leftPadding
        width: control.availableWidth
        text: control.title
        elide: Text.ElideRight
        color: Style.dimgrey
    }
}
