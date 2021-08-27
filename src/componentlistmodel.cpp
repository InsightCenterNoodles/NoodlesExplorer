#include "componentlistmodel.h"

#include <QDebug>

// =============================================================================

ComponentListFilter::ComponentListFilter(QSet<int> columns, QObject* p)
    : QSortFilterProxyModel(p), m_accept_columns(columns) { }

bool ComponentListFilter::filterAcceptsColumn(
    int                source_column,
    QModelIndex const& source_parent) const {
    Q_UNUSED(source_parent);

    if (m_accept_columns.contains(source_column)) { return true; }

    return false;
}
