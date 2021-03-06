import QtQuick
import QtQuick3D
import QtQuick.Controls
import QtQuick3D.Helpers

import Qt.labs.settings 1.1

Item {

    property var entity_list: ({})

    property var material_list: ({})

    Node {
        id: root_node

        Node {
            PerspectiveCamera {
                id: main_cam
                x: -4
                y: 1
                clipFar: 1000
                clipNear: .1

                Component.onCompleted: {
                    lookAt(root_node)
                }
            }
        }

        DirectionalLight {
            visible: settings.override_lights
            color: Qt.rgba(1.0, 1.0, 1.0, 1.0)
            brightness: .25
            rotation: Quaternion.lookAt(Qt.vector3d(-1,0,0), Qt.vector3d(0,0,0))
            castsShadow: true
        }

        DirectionalLight {
            visible: settings.override_lights
            color: Qt.rgba(1.0, 1.0, 1.0, 1.0)
            brightness: .1
            rotation: Quaternion.lookAt(Qt.vector3d(0,0,-1), Qt.vector3d(0,0,0))
            //castsShadow: true
        }

        DirectionalLight {
            visible: settings.override_lights
            color: Qt.rgba(1, 1, 1, 1.0)
            brightness: 1
            rotation: Quaternion.lookAt(Qt.vector3d(1,1,1), Qt.vector3d(0,0,0))
            //castsShadow: true
        }

        Model {
            geometry: GridGeometry {
                horizontalLines: 20
                verticalLines: 20
            }

            scale: Qt.vector3d(5, 5, 5)
            eulerRotation.x: -90
            materials: [
                DefaultMaterial {
                    diffuseColor: Qt.rgba(0.8, 0.8, 0.8, 1.0)
                    lighting: DefaultMaterial.NoLighting
                    opacity: .25
                }
            ]

            visible: settings.show_grid
        }

        Model {
            id: cursor

            source: "#Sphere"
            scale: Qt.vector3d(.001, .001, .001)

            materials: [
                PrincipledMaterial {
                    baseColor: Style.yellow
                    metalness: 0.0
                    roughness: 0.1
                }
            ]

            visible: false
        }
    }

    property var entity_maker: Qt.createComponent("RenderableEntity.qml")
    property var material_maker: Qt.createComponent("RenderableMaterial.qml")

    function seek_parent(pid) {
        if (pid >= 0) {
            return entity_list[pid]
        }
        return root_node
    }

    Connections {
        target: entity_notifier

        function onAsk_delete(oid) {
            entity_list[oid].destroy()
            delete entity_list[oid]
            console.log("Deleting", oid)
        }
        function onAsk_create(oid, pickable, pid, material, mesh, instances) {
            console.log("Creating", oid)
            let init_props = {
                "pickable": !!pickable,
                "hosting_object": pickable
            }

            console.log("Hooking to parent", pid)

            if (mesh) {
                init_props["geometry"] = mesh
            }

            if (instances) {
                init_props["instanceRoot"] = seek_parent(pid)
                init_props["instancing"] = instances
            }

            var new_ent = entity_maker.createObject(seek_parent(pid),
                                                    init_props)

            if (material >= 0) {
                console.log("Creating new material for object",
                            material_list[material])
                var mat_ent = material_maker.createObject(
                            new_ent, material_list[material])

                new_ent.materials.push(new_ent)
            }

            console.log(new_ent.bounds.minimum, new_ent.bounds.maximum)

            entity_list[oid] = new_ent
            console.log(oid, pid, material, mesh, instances)
            //onAsk_set_parent(oid, pid)
        }
        function onAsk_set_tf(oid, translate, quat, scale) {
            console.log("UPDATE TF", oid, translate, quat, scale)
            let e = entity_list[oid]
            e.position = translate
            e.rotation = quat
            e.scale = scale

            console.log("UPDATE TF", oid, e, e.sceneTransform, e.scenePosition)
        }
    }

    Connections {
        target: material_notifier

        function onAsk_delete(oid) {
            delete material_list[oid]
        }
        function onAsk_create(oid, color, metal, rough) {
            var comp = Qt.createComponent("RenderableMaterial.qml")

            let init_props = {
                "baseColor": color,
                "metalness": metal,
                "roughness": rough
            }
            material_list[oid] = init_props
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
            clearColor: settings.clear_color
            backgroundMode: SceneEnvironment.Color
        }
    }

    MouseArea {
        anchors.fill: scene_3d
        anchors.margins: 20
        acceptedButtons: Qt.RightButton

        onClicked: function (mouse) {
            console.log("Click at", mouse.x, mouse.y)
            var result_list = scene_3d.pickAll(mouse.x, mouse.y)

            for (var i = 0; i < result_list.length; i++) {

                result = result_list[i]

                console.log(result.distance, result.normal, result.objectHit,
                            result.position)
                if (result.objectHit) {
                    console.log("Hit!")
                    var obj = result.objectHit
                    console.log(obj.hosting_object)
                    if (obj.hosting_object) {
                        entity_notifier.on_pick(obj.hosting_object)
                    }

                    let new_pos = root_node.mapPositionFromScene(
                            result.scenePosition)

                    console.log("Moving cursor", new_pos)

                    cursor.position = new_pos
                    cursor.visible = true

                    return
                }

                console.log("No hit")
            }
        }
    }

    FirstPersonController {
        controlledObject: main_cam
        speed: .01
        shiftSpeed: .05

        xInvert: true
        yInvert: false

        enabled: window.allow_wasd_mouse
    }
}
