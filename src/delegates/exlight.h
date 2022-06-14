#ifndef EXLIGHT_H
#define EXLIGHT_H

#include "componentlistmodel.h"
#include "delegates.h"

#include <noo_client_interface.h>

class ExLight : public nooc::LightDelegate {
public:
    static QStringList header();

    ExLight(noo::LightID id, nooc::LightInit const& md);

    ~ExLight();

    // void prepare_delete() override { unregister(); }

    int      get_id() const;
    int      get_id_gen() const;
    QString  get_name() const;
    QVariant get_column(int c) const;
    void     on_update(nooc::LightUpdate const&) override;

    //    Qt3DRender::QAbstractLight* entity();
};

#endif // EXLIGHT_H
