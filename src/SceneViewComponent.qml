import QtQuick
import Qt3D.Core
import Qt3D.Render
import Qt3D.Input
import Qt3D.Extras
import QtQuick.Scene3D

import Qt.labs.settings 1.1

import EntityShim 1.0

Item {
    Connections {
        target: entity_notifier

        function onAsk_recreate(oid, nid, tf, mat, mesh, inst) {
            console.log("REQUEST NEW GFX", oid, nid, tf, mat, mesh, inst)
        }
    }

    Scene3D {
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
