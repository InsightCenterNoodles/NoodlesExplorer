#ifndef EXMESH_H
#define EXMESH_H

#include "componentlistmodel.h"
#include "delegates.h"

#include <noo_client_interface.h>

struct QtGeomInfo {
    UniqueQPtr<Qt3DRender::QGeometry>         geom;
    UniqueQPtr<Qt3DRender::QGeometryRenderer> renderer;

    QByteArray                         instance_data;
    UniqueQPtr<Qt3DRender::QBuffer>    instance_buffer;
    UniqueQPtr<Qt3DRender::QAttribute> instance_attribute;

    bool is_2d = false;

    QtGeomInfo();
    ~QtGeomInfo();

    QtGeomInfo(QtGeomInfo&&);
    QtGeomInfo& operator=(QtGeomInfo&&);
};

class ExMesh : public nooc::MeshDelegate {
    nooc::MeshData     m_data;
    Qt3DCore::QEntity* m_scene_root;

public:
    static QStringList header();

    ExMesh(noo::MeshID           id,
           nooc::MeshData const& md,
           Qt3DCore::QEntity*    scene_root);

    ~ExMesh();

    // void prepare_delete() override { unregister(); }

    int      get_id() const;
    int      get_id_gen() const;
    QString  get_name() const;
    QVariant get_column(int c) const;

    QtGeomInfo make_new_info(std::span<glm::mat4> instances);
};


#endif // EXMESH_H
