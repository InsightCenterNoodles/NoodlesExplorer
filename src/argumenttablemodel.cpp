#include "argumenttablemodel.h"

#include <QDebug>

ArgumentTableModel::ArgumentTableModel(QObject* parent)
    : StructTableModel<Argument>(parent) { }

void ArgumentTableModel::set_method_name(QString method_name) {
    if (m_method_name == method_name) return;

    m_method_name = method_name;
    emit method_name_changed(m_method_name);
}
void ArgumentTableModel::set_method_documentation(
    QString method_documentation) {
    if (m_method_documentation == method_documentation) return;

    m_method_documentation = method_documentation;
    emit method_documentation_changed(m_method_documentation);
}
