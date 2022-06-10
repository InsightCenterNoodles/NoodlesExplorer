import QtQuick
import QtQuick3D
import QtQuick3D.Helpers

Model {

    property QtObject hosting_object: null

    materials: [
        PrincipledMaterial {
            baseColor: Qt.rgba(0.8, 0.8, 0.8, 1.0)
            metalness: 0.1
            roughness: 0.1
        }
    ]
}
