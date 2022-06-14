import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    //NSPopOver {
    id: root

    //x: Math.round((parent.width - width) / 2)
    //y: Math.round((parent.height - height) / 2)

    //width: parent.width * .75
    ColumnLayout {
        anchors.fill: parent

        GridLayout {
            rows: 2
            columns: 2

            Layout.fillWidth: true

            Label {
                text: "Name:"
                Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
            }

            NSTextField {
                id: username_box
                placeholderText: "username"
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                Layout.fillWidth: true

                text: settings.last_user
            }

            Label {
                text: "Address:"
                Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
            }

            NSTextField {
                id: hostname_box
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                Layout.fillWidth: true

                text: settings.last_server

                validator: RegularExpressionValidator {
                    regularExpression: /^(ws|wss):\/\/[^ "]+$/
                }
            }
        }
        RowLayout {
            Layout.fillWidth: true
            Item {
                Layout.fillWidth: true
            }
            NSButton {
                text: "Connect"

                onClicked: {

                    if (!hostname_box.acceptableInput) {
                        return
                    }

                    console.log(username_box.text, hostname_box.text)

                    if (app_state.start_connection(username_box.text,
                                                   hostname_box.text)) {
                        root.close()
                    }

                    settings.last_user = username_box.text
                    settings.last_server = hostname_box.text
                }
            }
        }
    }
}
