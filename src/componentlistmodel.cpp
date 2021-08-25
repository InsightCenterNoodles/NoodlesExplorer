#include "componentlistmodel.h"

#include <QDebug>


// ComponentListItem::ComponentListItem() {
//    // calling register in the constructor is a bad idea.
//    // we arent fully constructed yet, and other qt code will react to this
//    // addition to the list and try to read data from it...resulting in a fun
//    // pure virtual call error.
//}

// ComponentListItem::~ComponentListItem() {
//    // m_host->unregister_item(this);
//}

// void ComponentListItem::notify() {
//    m_host->item_in_list_updated(this);
//}

// =============================================================================


// void ComponentListModelBase::unregister_item_bounce() {
//    return unregister_item(sender());
//}

// void ComponentListModelBase::item_in_list_updated_bounce() {
//    return item_in_list_updated(sender());
//}

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
