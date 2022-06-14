#ifndef EXSIGNAL_H
#define EXSIGNAL_H

#include "componentlistmodel.h"
#include "delegates.h"

#include <noo_client_interface.h>

class ExSignal : public nooc::SignalDelegate {
    Q_OBJECT

    QStringList m_cached_args;

public:
    static QStringList header();

    ExSignal(noo::SignalID id, nooc::SignalInit const& md);

    ~ExSignal();

    int      get_id() const;
    int      get_id_gen() const;
    QString  get_name() const;
    QVariant get_column(int c) const;

    // void prepare_delete() override { unregister(); }
signals:
    void updated();
};

#endif // EXSIGNAL_H
