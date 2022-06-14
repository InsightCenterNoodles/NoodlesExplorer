#include "argumenttablemodel.h"

#include "jsoneditdialog.h"

#include <QDebug>

ArgumentTableModel::ArgumentTableModel(QObject* parent)
    : StructTableModel<Argument>(parent) { }


QVariant ArgumentTableModel::data(const QModelIndex& index, int role) const {
    if (role == Qt::BackgroundRole and index.column() == 2) {
        auto text =
            StructTableModel<Argument>::data(index, Qt::DisplayRole).toString();

        bool ok = false;

        auto parse_result = JSONEditDialog::parse_any(text, &ok);

        return ok ? QColor("green") : QColor("red");
    }

    return StructTableModel<Argument>::data(index, role);
}

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
