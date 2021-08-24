#ifndef EXLIGHT_H
#define EXLIGHT_H

#include "componentlistmodel.h"
#include "delegates.h"

#include <noo_client_interface.h>

class ExLight : public nooc::LightDelegate, public ComponentListItem {
    nooc::LightData m_data;

    UniqueQPtr<Qt3DRender::QAbstractLight> m_3d_entity;

public:
    static QStringList header();

    ExLight(noo::LightID                        id,
            nooc::LightData const&              md,
            std::shared_ptr<ComponentListModel> list,
            Qt3DCore::QEntity*                  scene_root);

    ~ExLight();

    // void prepare_delete() override { unregister(); }

    int      get_id() const override;
    int      get_id_gen() const override;
    QString  get_name() const override;
    QVariant get_column(int c) const override;
    void     on_update(nooc::LightData const&) override;

    Qt3DRender::QAbstractLight* entity();
};

#endif // EXLIGHT_H
