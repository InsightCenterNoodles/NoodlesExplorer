#ifndef EXTEXTURE_H
#define EXTEXTURE_H

#include "componentlistmodel.h"
#include "delegates.h"

#include <noo_client_interface.h>

class ExTexture : public nooc::TextureDelegate {
    nooc::TextureData m_data;

public:
    static QStringList header();

    ExTexture(noo::TextureID id, nooc::TextureData const& md);

    ~ExTexture();

    // void prepare_delete() override { unregister(); }

    int      get_id() const;
    int      get_id_gen() const;
    QString  get_name() const;
    QVariant get_column(int c) const;
    void     on_update(nooc::TextureData const&) override;
};

#endif // EXTEXTURE_H
