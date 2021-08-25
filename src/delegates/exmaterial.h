#ifndef EXMATERIAL_H
#define EXMATERIAL_H

#include "componentlistmodel.h"
#include "delegates.h"

#include <noo_client_interface.h>

#include <QPhongMaterial>

class ExMaterial : public nooc::MaterialDelegate {
    nooc::MaterialData m_data;

    UniqueQPtr<QInstancedMetalRoughMaterial> m_3d_entity;
    UniqueQPtr<Qt3DExtras::QPhongMaterial>   m_2d_material;

public:
    static QStringList header();

    ExMaterial(noo::MaterialID           id,
               nooc::MaterialData const& md,
               Qt3DCore::QEntity*        scene_root);

    ~ExMaterial();

    // void prepare_delete() override { unregister(); }

    int      get_id() const;
    int      get_id_gen() const;
    QString  get_name() const;
    QVariant get_column(int c) const;
    void     on_update(nooc::MaterialData const&) override;

    // Qt3DExtras::QPhongMaterial*   get_2d_material();
    QInstancedMetalRoughMaterial* get_3d_material();
};

#endif // EXMATERIAL_H
