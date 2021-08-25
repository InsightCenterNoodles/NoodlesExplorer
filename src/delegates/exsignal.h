#ifndef EXSIGNAL_H
#define EXSIGNAL_H

#include "componentlistmodel.h"
#include "delegates.h"

#include <noo_client_interface.h>

class ExSignal : public nooc::SignalDelegate {
    QString     m_documentation;
    QStringList m_argument_documentation;

public:
    static QStringList header();

    ExSignal(noo::SignalID id, nooc::SignalData const& md);

    ~ExSignal();

    int      get_id() const;
    int      get_id_gen() const;
    QString  get_name() const;
    QVariant get_column(int c) const;

    // void prepare_delete() override { unregister(); }
};

#endif // EXSIGNAL_H
