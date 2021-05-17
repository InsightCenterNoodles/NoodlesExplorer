#ifndef EXMATERIAL_H
#define EXMATERIAL_H

#include "componentlistmodel.h"
#include "delegates.h"

#include <noo_client_interface.h>

class ExMaterial : public nooc::MaterialDelegate, public ComponentListItem {
    nooc::MaterialData m_data;

    UniqueQPtr<QInstancedMetalRoughMaterial> m_3d_entity;

public:
    static QStringList header();

    ExMaterial(noo::MaterialID                     id,
               nooc::MaterialData const&           md,
               std::shared_ptr<ComponentListModel> list,
               Qt3DCore::QEntity*                  scene_root);

    ~ExMaterial();

    void prepare_delete() override { unregister(); }

    int      get_id() const override;
    int      get_id_gen() const override;
    QString  get_name() const override;
    QVariant get_column(int c) const override;
    void     on_update(nooc::MaterialData const&) override;

    QInstancedMetalRoughMaterial* entity();
};

#endif // EXMATERIAL_H
