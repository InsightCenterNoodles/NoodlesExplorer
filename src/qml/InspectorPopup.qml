import QtQuick
import QtQuick.Controls
//import QtQuick.Controls.Material
import QtQuick.Layouts

import Qt.labs.qmlmodels 1.0
import Qt.labs.settings 1.1

Popup {
    id: inspector_popup
    modal: true

    x: Math.round((parent.width - width) / 2)
    y: Math.round((parent.height - height) / 2)
    width: window.width * .9
    height: window.height * .9

    ColumnLayout {
        anchors.fill: parent

        TabBar {
            id: component_tab_bar

            Layout.fillWidth: true

            TabButton {
                text: "\uf086" //Communication

                font.family: icon_solid.name
                font.styleName: "Solid"
            }

            TabButton {
                text: "\uf1c0" // buffer

                font.family: icon_solid.name
                font.styleName: "Solid"
            }

            TabButton {
                text: "\uf5ee" // mesh

                font.family: icon_solid.name
                font.styleName: "Solid"
            }

            TabButton {
                text: "\uf1fc" //Material

                font.family: icon_solid.name
                font.styleName: "Solid"
            }

            TabButton {
                text: "\uf03e" //texture

                font.family: icon_solid.name
                font.styleName: "Solid"
            }

            TabButton {
                text: "\uf0eb" //lights

                font.family: icon_solid.name
                font.styleName: "Solid"
            }

            TabButton {
                text: "\uf61f" //objects

                font.family: icon_solid.name
                font.styleName: "Solid"
            }

            TabButton {
                text: "\uf0ce" //table

                font.family: icon_solid.name
                font.styleName: "Solid"
            }
        }

        StackLayout {
            Layout.fillHeight: true
            Layout.fillWidth: true

            currentIndex: component_tab_bar.currentIndex

            clip: true

            Item {
                id: comm_tab

                HeaderTable {
                    anchors.fill: parent

                    model: method_list

                    relWidths: [1, 5, 10, 5, 5]
                }
            }

            Item {
                id: buffer_tab

                HeaderTable {
                    anchors.fill: parent

                    model: buffer_list

                    relWidths: [1, 5, 10, 1]
                }
            }

            Item {
                id: mesh_tab

                HeaderTable {
                    anchors.fill: parent

                    model: mesh_list

                    relWidths: [1, 5, 5, 5, 10]
                }
            }

            Item {
                id: mat_tab

                HeaderTable {
                    anchors.fill: parent

                    model: material_list

                    relWidths: [1, 5, 10, 5, 5]
                }
            }

            Item {
                id: tex_tab

                HeaderTable {
                    anchors.fill: parent

                    model: texture_list

                    relWidths: [1, 5, 5, 5, 5, 1]
                }
            }

            Item {
                id: light_tab

                HeaderTable {
                    anchors.fill: parent

                    model: light_list

                    relWidths: [1, 5, 1, 5, 1]
                }
            }

            Item {
                id: obj_tab

                HeaderTable {
                    anchors.fill: parent

                    model: object_list
                    relWidths: [1, 5, 5, 5, 5, 5, 5, 5, 5]
                }
            }

            Item {
                id: table_tab

                HeaderTable {
                    anchors.fill: parent

                    model: table_list

                    relWidths: [1, 5]
                }
            }
        }
    }
}
