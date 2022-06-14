#include "exmethod.h"

#include <noo_common.h>

QStringList ExMethod::header() {
    return {
        "ID", "Name", "Documentation", "Return Documentation", "Arguments"
    };
}

ExMethod::ExMethod(noo::MethodID id, nooc::MethodInit const& md)
    : nooc::MethodDelegate(id, md) {

    for (auto a : md.argument_documentation) {
        m_argument_names << a.name;
        m_argument_details << a.doc;
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
    return this->name();
}

QVariant ExMethod::get_column(int c) const {
    switch (c) {
    case 0: return get_id();
    case 1: return get_name();
    case 2: return documentation();
    case 3: return return_documentation();
    case 4: return m_argument_names.join(", ");
    }
    return {};
}
