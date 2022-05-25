import QtQuick
import Qt3D.Core
import Qt3D.Render
import Qt3D.Input
import Qt3D.Extras
import QtQuick.Scene3D

import Qt.labs.settings 1.1

import EntityShim 1.0

Item {

    property var entity_list: ({})

    Connections {
        target: entity_notifier

        function onAsk_delete(oid) {
            entity_list[oid].destroy()
            delete entity_list[oid]
        }
        function onAsk_create(oid) {
            var comp = Qt.createComponent("RenderableEntity.qml")
            var new_ent = comp.createObject(scene_3d, {})
            entity_list[oid] = new_ent
        }
        function onAsk_set_parent(oid, pid) {// HOW DO WE DO THIS??
        }
        function onAsk_set_tf(oid, mat) {
            entity_list[oid].transform.matrix = mat
        }
        function onAsk_set_render(oid) {
            delete entity_list[oid]
        }
    }

    Scene3D {
        id: scene_3d
        anchors.fill: parent

        focus: true

        aspects: ["render", "logic", "input"]

        Entity {
            components: {
                input_settings
                render_settings
            }

            InputSettings {
                id: input_settings
            }

            RenderSettings {
                id: render_settings

                activeFrameGraph: ForwardRenderer {
                    id: renderer
                    camera: camera
                    clearColor: settings.clear_color

                    RenderStateSet {
                        renderStates: [
                            FrontFace {
                                direction: FrontFace.CounterClockWise
                            }
                        ]
                    }
                }
            }

            Camera {
                id: camera
                projectionType: CameraLens.PerspectiveProjection
                fieldOfView: 45
                nearPlane: 0.1
                farPlane: 500.0
                position: Qt.vector3d(0.0, 0.0, 10.0)
                viewCenter: Qt.vector3d(0.0, 0.0, 0.0)
                upVector: Qt.vector3d(0.0, 1.0, 0.0)
            }

            OrbitCameraController {
                id: orbit_cam_controller
                //enabled: settings.orbit_cam
                camera: camera
            }

            //            FirstPersonCameraController {
            //                id: fp_cam_controller
            //                enabled: !settings.orbit_cam
            //                camera: camera
            //            }
            EntityShim {}
        }
    }
}
