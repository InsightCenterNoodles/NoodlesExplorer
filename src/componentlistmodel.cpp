#include "componentlistmodel.h"

#include <QDebug>

// void ComponentListItem::unregister() {
//    m_host->unregister_item(this);
//}

ComponentListItem::ComponentListItem(std::shared_ptr<ComponentListModel> host)
    : m_host(host) {
    m_host->register_item(this);
}

ComponentListItem::~ComponentListItem() {
    m_host->unregister_item(this);
}

void ComponentListItem::notify() {
    m_host->item_in_list_updated(this);
}

// =============================================================================

ComponentListModel::ComponentListModel(QStringList header, QObject* parent)
    : QAbstractTableModel(parent), m_header(header) { }

QVariant ComponentListModel::headerData(int             section,
                                        Qt::Orientation orientation,
                                        int             role) const {
    // only section is 0

    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        return m_header.value(section);
    }

    return {};
}

int ComponentListModel::rowCount(QModelIndex const& parent) const {
    if (parent.isValid()) return 0;

    return m_items.size();
}

int ComponentListModel::columnCount(QModelIndex const& parent) const {
    if (parent.isValid()) return 0;

    return m_header.size();
}

QVariant ComponentListModel::data(QModelIndex const& index, int role) const {
    if (!index.isValid()) return {};

    auto const& item = m_items.at(index.row());

    Q_ASSERT(item);

    if (role == Qt::DisplayRole or role == Qt::EditRole) {
        return item->get_column(index.column());
    }

    if (role >= Qt::UserRole) {
        int index = role - Qt::UserRole;

        return item->get_column(index);
    }

    return QVariant();
}

QHash<int, QByteArray> ComponentListModel::roleNames() const {
    static const auto roles = [&]() {
        QHash<int, QByteArray> ret;

        for (int i = 0; i < m_header.size(); i++) {
            ret[Qt::UserRole + i] = m_header[i].toLower().toLocal8Bit();
        }

        return ret;
    }();

    return roles;
}

ComponentListItem* ComponentListModel::get_item(int index) const {
    return m_items.value(index);
}

void ComponentListModel::register_item(ComponentListItem* item) {
    Q_ASSERT(item);

    auto place = m_items.size();

    beginInsertRows({}, place, place);

    m_items.push_back(item);
    m_item_place_map[item] = place;

    endInsertRows();
}

void ComponentListModel::unregister_item(ComponentListItem* item) {

    auto iter = m_item_place_map.find(item);

    Q_ASSERT(iter != m_item_place_map.end());

    auto place = iter.value();

    beginRemoveRows({}, place, place);

    m_items.remove(place);

    // rebuild place map (terrible, but ok for now)
    m_item_place_map.clear();

    for (int i = 0; i < m_items.size(); i++) {
        m_item_place_map[m_items[i]] = i;
    }

    endRemoveRows();
}

void ComponentListModel::item_in_list_updated(ComponentListItem* item) {
    auto iter = m_item_place_map.find(item);

    Q_ASSERT(iter != m_item_place_map.end());

    auto place = iter.value();

    auto tl = index(place, 0);
    auto br = index(place, columnCount());

    emit dataChanged(tl, br);
}

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
