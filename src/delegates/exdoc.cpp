#include "exdoc.h"

#include "delegates.h"

ExDoc::ExDoc(AttachedMethodListModel* p) : m_attached_methods(p) {
    // m_attached_methods = new AttachedMethodListModel(this);

    //    connect(m_attached_methods,
    //            &AttachedMethodListModel::wishes_to_call,
    //            [this](ExMethod* ptr) {
    //                auto* dialog = new MethodCallDialog(this, ptr);

    //                dialog->setVisible(true);
    //            });
}
ExDoc::~ExDoc() = default;

void ExDoc::on_update(nooc::DocumentData const& data) {
    qDebug() << "Updating document methods";
    if (data.methods_list) {
        m_attached_methods->set(data.methods_list.value());
    }
}
void ExDoc::on_clear() { }

QStringList ExDoc::get_ab_method_ids() const {
    return build_id_list(attached_methods().list());
}
