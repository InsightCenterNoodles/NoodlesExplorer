import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import "jsconsole.js" as Util

Rectangle {
    color: Style.set_alpha(Style.grey3, .75)

    property var current_function: Util.make_console_scope()

    function jsCall(exp) {
        const data = Util.call(exp, current_function)

        output_model.append(data)

        output_view.positionViewAtEnd()
    }

    function jsComplete(exp) {
        if (!exp.length) return

        const replacement = Util.autocomplete(exp)

        console.log(replacement)
    }

    ColumnLayout {
        anchors.fill: parent

        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true

            ListView {
                id: output_view

                clip: true

                anchors.fill: parent

                model: ListModel {
                    id: output_model
                }

                delegate: ColumnLayout {
                    id: delegate
                    required property var model
                    width: ListView.view.width

                    Label {
                        Layout.fillWidth: true
                        color: "green"
                        text: "> " + delegate.model.expression
                    }

                    Label {
                        Layout.fillWidth: true
                        color: delegate.model.error === "" ? "green" : "red"
                        text: delegate.model.error === "" ? "" + delegate.model.result : delegate.model.error
                    }
                    Rectangle {
                        height: 1
                        Layout.fillWidth: true
                        color: Style.grey5
                        opacity: .2
                    }
                }
            }
        }

        RowLayout {
            Layout.fillWidth: true
            NSTextField {
                id: input
                Layout.fillWidth: true
                focus: true

                placeholderText: "Enter command ('help' for listing)"

                onAccepted:  {
                    jsCall(input.text);
                    input.text = ""
                }
                Keys.onTabPressed: {
                    jsComplete(input.text)
                }
                Keys.onUpPressed: {
                    if (!output_model.count) return

                    input.text = output_model.get(output_model.count - 1).expression
                }
            }
        }
    }
}
