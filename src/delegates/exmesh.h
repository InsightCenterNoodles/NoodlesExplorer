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

    QtGeomInfo();
    ~QtGeomInfo();

    QtGeomInfo(QtGeomInfo&&);
    QtGeomInfo& operator=(QtGeomInfo&&);
};

class ExMesh : public nooc::MeshDelegate, public ComponentListItem {
    nooc::MeshData     m_data;
    Qt3DCore::QEntity* m_scene_root;

public:
    static QStringList header();

    ExMesh(noo::MeshID                         id,
           nooc::MeshData const&               md,
           std::shared_ptr<ComponentListModel> list,
           Qt3DCore::QEntity*                  scene_root);

    ~ExMesh();

    void prepare_delete() override { unregister(); }

    int      get_id() const override;
    int      get_id_gen() const override;
    QString  get_name() const override;
    QVariant get_column(int c) const override;

    QtGeomInfo make_new_info(std::span<glm::mat4> instances);
};


#endif // EXMESH_H
