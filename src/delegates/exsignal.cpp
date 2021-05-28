#include "exsignal.h"

#include "noo_common.h"

QStringList ExSignal::header() {
    return { "ID", "Name", "Documentation", "Arguments" };
}

ExSignal::ExSignal(noo::SignalID                       id,
                   nooc::SignalData const&             md,
                   std::shared_ptr<ComponentListModel> list)
    : nooc::SignalDelegate(id, md), ComponentListItem(list) {

    m_documentation = noo::to_qstring(md.documentation);

    for (auto a : md.argument_documentation) {
        m_argument_documentation << noo::to_qstring(a.name);
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
    return noo::to_qstring(this->name());
}

QVariant ExSignal::get_column(int c) const {
    switch (c) {
    case 0: return get_id();
    case 1: return get_name();
    case 2: return m_documentation;
    case 3: return m_argument_documentation;
    }
    return {};
}
