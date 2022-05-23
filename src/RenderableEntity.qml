import QtQuick
import Qt3D.Core
import Qt3D.Render
import Qt3D.Input
import Qt3D.Extras

Entity {

    //    ObjectPicker {
    //        id: picker
    //        hoverEnabled: true
    //        onClicked: {
    //            if (selected)
    //                selected = false
    //            else
    //                selected = true
    //        }
    //    }
    MetalRoughMaterial {
        id: material
        roughness: .1
        metalness: 1
    }

    Mesh {
        id: mesh
        source: url
    }

    Transform {
        id: transform
        scale3D: Qt.vector3d(1, 1, 1)
        translation: Qt.vector3d(0, 0, 0)
        rotation: fromAxisAndAngle(Qt.vector3d(1, 0, 0), 0)
    }

    components: [mesh, material, transform, picker]
}
