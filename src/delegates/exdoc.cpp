#include "exdoc.h"

#include "delegates.h"

ExDoc::ExDoc() { }
ExDoc::~ExDoc() = default;

void ExDoc::on_update(nooc::DocumentData const& data) {
    m_methods = data.method_list;
}
void ExDoc::on_clear() {
    m_methods.clear();
}

QStringList ExDoc::get_ab_method_ids() const {
    return build_id_list(m_methods);
}
