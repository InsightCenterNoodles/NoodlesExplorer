#include "exdoc.h"

#include "delegates.h"

ExDoc::ExDoc() { }
ExDoc::~ExDoc() = default;

void ExDoc::on_update(nooc::DocumentData const& /*data*/) { }
void ExDoc::on_clear() { }

QStringList ExDoc::get_ab_method_ids() const {
    return build_id_list(attached_methods().list());
}
