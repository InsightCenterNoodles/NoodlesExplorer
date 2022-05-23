#include "exsignal.h"

#include "noo_common.h"

QStringList ExSignal::header() {
    return { "ID", "Name", "Documentation", "Arguments" };
}

ExSignal::ExSignal(noo::SignalID id, nooc::SignalInit const& md)
    : nooc::SignalDelegate(id, md) {
    for (auto const& a : info().argument_documentation) {
        m_cached_args << a.name;
    }
}

ExSignal::~ExSignal() = default;

int ExSignal::get_id() const {
    return this->id().id_slot;
}
int ExSignal::get_id_gen() const {
    return this->id().id_gen;
}
QString ExSignal::get_name() const {
    return this->name();
}

QVariant ExSignal::get_column(int c) const {
    switch (c) {
    case 0: return get_id();
    case 1: return get_name();
    case 2: return info().documentation;
    case 3: return m_cached_args;
    }
    return {};
}
