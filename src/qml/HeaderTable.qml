import QtQuick
import QtQuick.Controls
//import QtQuick.Controls.Material
import QtQuick.Layouts

import Qt.labs.qmlmodels 1.0
import Qt.labs.settings 1.1

ColumnLayout {
    id: root

    property var model

    property int columnSpacing: 6
    property var relWidths: []
    property int defaultColumnWidth: 25

    function refresh_layout() {
        table_view.forceLayout()
    }

    onDefaultColumnWidthChanged: refresh_layout()
    onRelWidthsChanged: {
        refresh_layout()

        let sum = 0
        relWidths.forEach(function (x) {
            sum += x
        })

        frac_widths = relWidths.map(function (x) {
            return x / sum
        })
    }

    property var frac_widths: []

    Rectangle {
        Layout.fillWidth: true
        height: 18

        //color: Material.backgroundColor

        Row {
            anchors.fill: parent
            spacing: root.columnSpacing
            z: 2
            Repeater {
                model: table_view.columns > 0 ? table_view.columns : 1
                Label {
                    width: table_view.columnWidthProvider(modelData)
                    height: 12

                    text: root.model.headerData(modelData, Qt.Horizontal)

                    elide: Label.ElideRight

                    font.bold: true
                    font.italic: true
                }
            }
        }
    }

    TableView {
        id: table_view
        Layout.fillHeight: true
        Layout.fillWidth: true

        onWidthChanged: forceLayout()

        clip: true

        columnSpacing: root.columnSpacing

        columnWidthProvider: function (column) {
            if (column >= relWidths.length) {
                return defaultColumnWidth
            }

            let rel_w = frac_widths[column]

            let ac_w = Math.round(
                    rel_w * (width - (columnSpacing * table_view.columns)))

            return ac_w
        }

        //        rowHeightProvider: function (row) {
        //            return 12
        //        }
        model: root.model

        delegate: Label {
            text: String(display).length ? String(display) : " "
            elide: Label.ElideRight
        }
    }
}
