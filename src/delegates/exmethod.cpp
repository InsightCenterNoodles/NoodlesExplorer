#include "exmethod.h"

#include <noo_common.h>

QStringList ExMethod::header() {
    return {
        "ID", "Name", "Documentation", "Return Documentation", "Arguments"
    };
}

ExMethod::ExMethod(noo::MethodID                       id,
                   nooc::MethodData const&             md,
                   std::shared_ptr<ComponentListModel> list)
    : nooc::MethodDelegate(id, md), ComponentListItem(list) {

    m_documentation        = noo::to_qstring(md.documentation);
    m_return_documentation = noo::to_qstring(md.return_documentation);

    for (auto a : md.argument_documentation) {
        m_argument_names << noo::to_qstring(a.name);
        m_argument_details << noo::to_qstring(a.doc);
    }
}

ExMethod::~ExMethod() = default;

int ExMethod::get_id() const {
    return this->id().id_slot;
}
int ExMethod::get_id_gen() const {
    return this->id().id_gen;
}
QString ExMethod::get_name() const {
    return noo::to_qstring(this->name());
}

QVariant ExMethod::get_column(int c) const {
    switch (c) {
    case 0: return get_id();
    case 1: return get_name();
    case 2: return m_documentation;
    case 3: return m_return_documentation;
    case 4: return m_argument_names.join(", ");
    }
    return {};
}
