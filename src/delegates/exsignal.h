#ifndef EXSIGNAL_H
#define EXSIGNAL_H

#include "componentlistmodel.h"
#include "delegates.h"

#include <noo_client_interface.h>

class ExSignal : public nooc::SignalDelegate, public ComponentListItem {
    QString     m_documentation;
    QStringList m_argument_documentation;

public:
    static QStringList header();

    ExSignal(noo::SignalID                       id,
             nooc::SignalData const&             md,
             std::shared_ptr<ComponentListModel> list);

    ~ExSignal();

    int      get_id() const override;
    int      get_id_gen() const override;
    QString  get_name() const override;
    QVariant get_column(int c) const override;

    // void prepare_delete() override { unregister(); }
};

#endif // EXSIGNAL_H
