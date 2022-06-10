import QtQuick
import QtQuick.Controls

Popup {
    enter: Transition {
        ParallelAnimation {
            //            PropertyAnimation {
            //                target: root
            //                property: "y"
            //                from: root.height / 2
            //                to: Math.round((parent.height - height) / 2)
            //                duration: 300
            //                easing.type: Easing.OutQuad
            //            }
            PropertyAnimation {
                target: root
                property: "opacity"
                from: 0
                to: 1
                duration: 250
                easing.type: Easing.Linear
            }
        }
    }

    exit: Transition {
        ParallelAnimation {
            PropertyAnimation {
                target: root
                property: "opacity"
                from: 1
                to: 0
                duration: 250
                easing.type: Easing.Linear
            }
            //            PropertyAnimation {
            //                target: root
            //                property: "y"
            //                to: root.height / 2
            //                from: Math.round((parent.height - height) / 2)
            //                duration: 300
            //                easing.type: Easing.OutQuad
            //            }
        }
    }
}
