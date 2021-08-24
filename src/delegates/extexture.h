#ifndef EXTEXTURE_H
#define EXTEXTURE_H

#include "componentlistmodel.h"
#include "delegates.h"

#include <noo_client_interface.h>

class ExTexture : public nooc::TextureDelegate, public ComponentListItem {
    nooc::TextureData m_data;

public:
    static QStringList header();

    ExTexture(noo::TextureID                      id,
              nooc::TextureData const&            md,
              std::shared_ptr<ComponentListModel> list);

    ~ExTexture();

    // void prepare_delete() override { unregister(); }

    int      get_id() const override;
    int      get_id_gen() const override;
    QString  get_name() const override;
    QVariant get_column(int c) const override;
    void     on_update(nooc::TextureData const&) override;
};

#endif // EXTEXTURE_H
