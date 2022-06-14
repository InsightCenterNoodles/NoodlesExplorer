import QtQuick
import QtQuick.Controls
import Qt5Compat.GraphicalEffects

NSPopup {
    id: popover
    property var caller: null
    property int side: Qt.AlignBottom
    property int offset: 0

    function openAt(widget, x, y) {
        if (!widget)
            throw "Caller cannot be undefined!"

        caller = widget
        popover.parent = widget

        var position = widget.mapToItem(popover.parent, x, y)
        popover.x = position.x - popover.width / 2

        if (position.y + popover.height + 5 + padding > parent) {
            side = Qt.AlignTop
            popover.y = position.y - popover.height - 5 - padding - widget.height
            if (position.y - popover.height - 5 - widget.height - padding < units.gu(
                        1.5)) {
                popover.y = 5 + padding
                side = Qt.AlignVCenter
            }
        } else {
            side = Qt.AlignBottom
            popover.y = position.y + 5 + padding
        }

        if (popover.x < 1) {
            popover.offset = popover.x - 1
            popover.x = 1
        } else if (popover.x + popover.width > popover.parent.width - 1) {
            popover.offset = popover.x + popover.width - (popover.parent.width - 1)
            popover.x = popover.parent.width - 1 - popover.width
        } else {
            popover.offset = 0
        }
        open()
    }

    function openOn(widget) {
        openAt(widget, widget.width / 2, widget.height)
    }

    Rectangle {
        //border.color: borderColor
        //color: background
        width: 15
        radius: 5
        height: width
        visible: side != Qt.AlignVCenter

        rotation: 45

        anchors {
            horizontalCenter: parent.horizontalCenter
            verticalCenter: side == Qt.AlignBottom ? parent.top : parent.bottom
            verticalCenterOffset: side == Qt.AlignBottom ? 1 : -1
            horizontalCenterOffset: offset

            Behavior on verticalCenterOffset {
                NumberAnimation {
                    duration: 200
                }
            }
        }
    }
}
