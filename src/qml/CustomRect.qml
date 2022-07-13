import QtQuick

Item {
    id: root

    property color color: Qt.white

    onColorChanged: canvas.requestPaint()

    property int radius: 5
    onRadiusChanged: canvas.requestPaint()

    property bool topLeftCorner: true
    property bool topRightCorner: false
    property bool bottomRightCorner: true
    property bool bottomLeftCorner: false

    onTopLeftCornerChanged: canvas.requestPaint()
    onTopRightCornerChanged: canvas.requestPaint()
    onBottomRightCornerChanged: canvas.requestPaint()
    onBottomLeftCornerChanged: canvas.requestPaint()

    property color borderColor: "black"
    onBorderColorChanged: canvas.requestPaint()

    property int borderWidth: 0

    Canvas {
        id: canvas
        anchors.fill: parent

        onPaint: {
            var context = getContext("2d")
            context.reset()
            context.beginPath()

            //Start position
            context.moveTo(0, height / 2)

            //topLeftCorner
            if (topLeftCorner) {
                context.lineTo(0, radius)
                context.arcTo(0, 0, radius, 0, radius)
            } else {
                context.lineTo(0, 0)
            }

            //topRightCorner
            if (topRightCorner) {
                context.lineTo(width - radius, 0)
                context.arcTo(width, 0, width, radius, radius)
            } else {
                context.lineTo(width, 0)
            }

            //bottomRightCorner
            if (bottomRightCorner) {
                context.lineTo(width, height - radius)
                context.arcTo(width, height, width - radius, height, radius)
            } else {
                context.lineTo(width, height)
            }

            //bottomLeftCorner
            if (bottomLeftCorner) {
                context.lineTo(radius, height)
                context.arcTo(0, height, 0, height - radius, radius)
            } else {
                context.lineTo(0, height)
            }

            //Close path
            context.lineTo(height / 2)
            context.closePath()

            //Draw border
            if (borderWidth > 0) {
                context.lineWidth = borderWidth
                context.strokeStyle = borderColor
                context.stroke()
            }

            //Draw background
            context.fillStyle = color
            context.fill()
        }
    }
}
