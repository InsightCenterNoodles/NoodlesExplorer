import QtQuick 2.12
import Qt3D.Core 2.12
import Qt3D.Render 2.12
import Qt3D.Input 2.12
import Qt3D.Extras 2.15
import QtQuick.Scene3D 2.15

import Qt.labs.settings 1.1

import EntityShim 1.0

Item {

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
            EntityShim {

                Entity {

                    enabled: false

                    MetalRoughMaterial {
                        id: redMat
                        baseColor: "red"
                    }
                    SphereMesh {
                        id: sphereMesh
                        radius: .1
                    }
                    Transform {
                        id: sphereTransform
                        translation: Qt.vector3d(0, 0, 0)
                    }

                    components: [redMat, sphereMesh, sphereTransform]
                }
            }
        }
    }
}
