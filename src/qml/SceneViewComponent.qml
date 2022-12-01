import QtQuick
import QtQuick3D
import QtQuick.Controls
import QtQuick3D.Helpers

import Qt.labs.settings 1.1

Item {

    property var entity_list: ({})

    property var material_list: ({})

    property var texture_list: ({})

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
            color: Qt.rgba(1, 1, 1.0, 1.0)
            brightness: 5
            rotation: Quaternion.lookAt(Qt.vector3d(1,1,1), Qt.vector3d(0,0,0))
            castsShadow: true
        }

        DirectionalLight {
            //visible: settings.override_lights
            visible: false
            color: Qt.rgba(.85, 1, .85, 1.0)
            brightness: 3
            rotation: Quaternion.lookAt(Qt.vector3d(0,-1,0), Qt.vector3d(0,0,0))
            //castsShadow: true
        }

        DirectionalLight {
            //visible: settings.override_lights
            visible: false
            color: Qt.rgba(.8, .8, 1, 1.0)
            brightness: 1
            rotation: Quaternion.lookAt(Qt.vector3d(0,1,0), Qt.vector3d(0,0,0))
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

        Model {
            id: translation_item

            pickable: true

            source: "#Sphere"
            scale: Qt.vector3d(.001, .001, .001)

            materials: [
                PrincipledMaterial {
                    baseColor: Style.red
                    metalness: 0.0
                    roughness: 0.1
                }
            ]

            visible: scene_3d.is_moving_thing


        }
    }

    property var entity_maker: Qt.createComponent("RenderableEntity.qml")
    property var material_maker: Qt.createComponent("RenderableMaterial.qml")
    property var texture_maker: Qt.createComponent("RenderableTexture.qml")

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
                "pickable": true,
                "hosting_object": pickable
            }

            console.log("Hooking to parent", pid, pickable)

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
                let material_props = material_list[material];
                //console.log("Creating new material:",
                //            JSON.stringify(material_props))
                var mat_ent = material_maker.createObject(new_ent,
                                                          material_props)

                new_ent.materials[0] = mat_ent
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

        function onStart_move(p) {
            console.log("Starting move, putting cursor at", p)
            translation_item.position = p;
            scene_3d.is_moving_thing = true;
        }
    }

    Connections {
        target: material_notifier

        function onAsk_delete(oid) {
            console.log("Deleting QML material", oid)
            delete material_list[oid]
        }
        function onAsk_create(oid, color, color_tex_id, metal, rough, double_sided) {
            console.log("Creating QML material", oid)

            let init_props = {
                "baseColor": color,
                "metalness": metal,
                "roughness": rough,
            }

            //console.log(PrincipledMaterial.BackFaceCulling, PrincipledMaterial.NoCulling)

            init_props["cullMode"] = double_sided ? PrincipledMaterial.NoCulling  : RenderableMaterial.BackFaceCulling

            //console.log(init_props["cullMode"])
            console.log("Contents", JSON.stringify(init_props), double_sided)

            if (color_tex_id >= 0) {
                init_props["baseColorMap"] = texture_list[color_tex_id]
            }

            material_list[oid] = init_props
        }
    }

    Connections {
        target: texture_notifier

        function onAsk_delete(oid) {
            console.log("Deleting QML texture", oid)
            delete texture_list[oid]
        }
        function onAsk_create(oid, data, extra_opts) {
            console.log("Creating QML texture", oid)

            console.log(JSON.stringify(extra_opts))

            extra_opts["textureData"] = data

            extra_opts["minFilter"] = extra_opts["minFilter"] ? Texture.Linear : Texture.Nearest;
            extra_opts["magFilter"] = extra_opts["magFilter"] ? Texture.Linear : Texture.Nearest;



            texture_list[oid] = texture_maker.createObject(root_node, extra_opts)
        }
    }

    View3D {
        id: scene_3d
        anchors.fill: parent

        focus: true

        importScene: root_node

        camera: main_cam

        property bool is_moving_thing: false
        property bool is_dragging_moving_thing: false

        environment: SceneEnvironment {
            antialiasingMode: SceneEnvironment.MSAA
            clearColor: settings.clear_color
            //backgroundMode: SceneEnvironment.Color
            backgroundMode: SceneEnvironment.SkyBox
            lightProbe: Texture {
                textureData: ProceduralSkyTextureData {
                    sunEnergy: 10
                }
            }
        }
    }

    MouseArea {
        anchors.fill: scene_3d
        anchors.margins: 20
        acceptedButtons: Qt.AllButtons

        hoverEnabled: !wasd_button.checked

        onPositionChanged: function(mouse) {
            if (scene_3d.is_dragging_moving_thing) {
                // get and keep current depth
                let scene_pos = scene_3d.mapFrom3DScene(translation_item.position)

                let z = scene_pos.z

                translation_item.position = scene_3d.mapTo3DScene(Qt.vector3d(mouse.x, mouse.y, z))
            }
        }

        onPressed: function(mouse) {
            console.log("Pressed", scene_3d.is_moving_thing, mouse.button)

            if (scene_3d.is_moving_thing && (mouse.button & Qt.LeftButton)) {

                var result_list = scene_3d.pickAll(mouse.x, mouse.y)

                for (let result of result_list) {
                    console.log("Checking", result.objectHit)
                    //var result = scene_3d.pick(mouse.x, mouse.y)

                    if (result.objectHit === translation_item) {
                        scene_3d.is_dragging_moving_thing = true
                        break;
                    }
                }

            }

            if (mouse.button & Qt.LeftButton) {
                // this is
            }
        }

        onReleased: function(mouse) {
            if (scene_3d.is_moving_thing &&  scene_3d.is_dragging_moving_thing) {
                scene_3d.is_dragging_moving_thing = false
                scene_3d.is_moving_thing = false

                let last_pos = root_node.mapPositionFromNode(translation_item, Qt.vector3d(0,0,0))

                entity_notifier.move_completed(last_pos)
            }
        }

        /*
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
        */
    }

    FirstPersonController {
        controlledObject: main_cam
        speed: .01
        shiftSpeed: .05

        xInvert: true
        yInvert: false

        enabled: wasd_button.checked
    }

    NSRoundedButton {
        id: wasd_button

        text: "\uf072"

        checkable: true

        anchors.right: parent.right
        anchors.bottom: parent.bottom

        Shortcut {
            sequence: "F"
            onActivated: wasd_button.checked = !wasd_button.checked
        }
    }
}
