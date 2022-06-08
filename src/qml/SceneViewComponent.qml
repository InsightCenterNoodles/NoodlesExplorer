import QtQuick
import QtQuick3D
import QtQuick.Controls
import QtQuick3D.Helpers

import Qt.labs.settings 1.1

Item {

    property var entity_list: ({})

    Node {
        id: root_node

        Node {
            PerspectiveCamera {
                id: main_cam
                x: 5
                y: 1
                clipFar: 1000
                clipNear: .1
                eulerRotation.y: 90
            }
        }

        DirectionalLight {
            ambientColor: Qt.rgba(0.5, 0.5, 0.5, 1.0)
            brightness: 1.0
            eulerRotation.x: -25
        }

        Model {
            geometry: GridGeometry {
                horizontalLines: 20
                verticalLines: 20
            }

            scale: Qt.vector3d(10, 10, 10)
            eulerRotation.x: -90
            materials: [
                PrincipledMaterial {
                    baseColor: Qt.rgba(0.8, 0.8, 0.8, 1.0)
                    metalness: 0.1
                    roughness: 0.1
                    opacity: .25
                }
            ]
        }
    }

    Connections {
        target: entity_notifier

        function seek_parent(pid) {
            if (pid >= 0) {
                return entity_list[pid]
            }
            return root_node
        }

        function onAsk_delete(oid) {
            entity_list[oid].destroy()
            delete entity_list[oid]
        }
        function onAsk_create(oid, pid, material, mesh, instances) {
            var comp = Qt.createComponent("RenderableEntity.qml")

            let init_props = {
                "parent": seek_parent(pid)
            }

            if (mesh) {
                init_props["geometry"] = mesh
            }

            var new_ent = comp.createObject(root_node, init_props)
            entity_list[oid] = new_ent
            console.log(oid, pid, material, mesh, instances)
            onAsk_set_parent(oid, pid)
        }
        function onAsk_set_tf(oid, mat) {
            entity_list[oid].transform.matrix = mat
        }
        function onAsk_set_parent(oid, pid) {
            entity_list[oid].parent = seek_parent(entity_list[pid])
        }
    }

    View3D {
        id: scene_3d
        anchors.fill: parent

        focus: true

        importScene: root_node

        camera: main_cam

        environment: SceneEnvironment {
            antialiasingMode: SceneEnvironment.MSAA
            clearColor: "black"
            backgroundMode: SceneEnvironment.Color
        }
    }

    WasdController {
        controlledObject: main_cam
        speed: .01
    }
}
