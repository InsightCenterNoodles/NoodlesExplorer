#include "tabledata.h"

#include "variant_tools.h"

#include <QDebug>

void SelectionsTableData::new_selection(noo::Selection const& ref) {
    auto const location = m_selections.size();
    beginInsertRows(QModelIndex(), location, location);

    auto sel = m_selections.emplace_back(std::make_shared<SelectionSlot>());

    sel->selection = ref;
    sel->index     = location;

    m_string_map[ref.name] = sel;

    endInsertRows();
}

void SelectionsTableData::update_selection(noo::Selection const& ref) {
    if (!m_string_map.contains(ref.name)) return;

    auto& place = m_string_map[ref.name];

    place->selection = ref;

    auto qindex = index(place->index, 0);
    dataChanged(qindex, qindex);
}

void SelectionsTableData::del_selection(QString const& name) {
    if (!m_string_map.contains(name)) return;

    auto place = m_string_map[name]->index;
    beginRemoveRows(QModelIndex(), place, place);

    m_string_map.remove(name);
    m_selections.erase(m_selections.begin() + place);
    endRemoveRows();
}

SelectionsTableData::SelectionsTableData(QObject* parent)
    : QAbstractTableModel(parent) { }


void SelectionsTableData::set(QVector<noo::Selection> sel_list) {
    beginResetModel();

    m_selections.clear();
    m_string_map.clear();

    for (auto const& sel : sel_list) {
        auto ptr = std::make_shared<SelectionSlot>();

        ptr->selection = sel;
        ptr->index     = m_selections.size();

        m_selections << ptr;

        m_string_map[sel.name] = ptr;
    }

    endResetModel();
}

void SelectionsTableData::on_table_selection_updated(
    noo::Selection const& ref) {

    qDebug() << Q_FUNC_INFO << ref.name << ref.rows;

    // is this an update?
    auto iter = m_string_map.find(ref.name);

    if (iter == m_string_map.end()) {
        // this is an add
        return new_selection(ref);
    }

    // this is an update. if empty, its a delete
    if (ref.rows.size() == 0 and ref.row_ranges.empty()) {
        return del_selection(ref.name);
    }

    update_selection(ref);
}

SelectionsTableData::SelectionSlot const*
SelectionsTableData::slot_at(size_t i) const {
    if ((qint64)i >= m_selections.size()) return nullptr;

    return m_selections[i].get();
}

void SelectionsTableData::clear() {
    beginResetModel();
    m_selections.clear();
    m_string_map.clear();
    endResetModel();
}

QVariant SelectionsTableData::headerData(int             section,
                                         Qt::Orientation orientation,
                                         int             role) const {
    if (role != Qt::DisplayRole) return {};

    if (orientation == Qt::Orientation::Vertical) { return {}; }

    switch (section) {
    case 0: return QString("Name");
    }

    return "Unknown";
}


int SelectionsTableData::rowCount(QModelIndex const& parent) const {
    if (parent.isValid()) return 0;
    return m_selections.size();
}

int SelectionsTableData::columnCount(QModelIndex const& parent) const {
    if (parent.isValid()) return 0;
    return 1;
}

QVariant SelectionsTableData::data(QModelIndex const& index, int role) const {
    if (!index.isValid()) return {};
    if (index.row() >= m_selections.size()) return {};

    if (role != Qt::DisplayRole and role != Qt::EditRole) return {};

    return m_selections.at(index.row())->selection.name;
}

Qt::ItemFlags SelectionsTableData::flags(QModelIndex const& index) const {
    if (!index.isValid()) return Qt::NoItemFlags;

    return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}


// =============================================================================

RemoteTableData::RemoteTableData(QObject* parent)
    : QAbstractTableModel(parent) {

    m_selections = new SelectionsTableData(this);
}

void RemoteTableData::on_table_subscribed(nooc::TableDataInit const& init) {

    qDebug() << Q_FUNC_INFO;

    on_table_reset(init);
}

void RemoteTableData::on_table_reset(nooc::TableDataInit const& init) {
    qDebug() << Q_FUNC_INFO;

    beginResetModel();

    m_header.clear();
    m_rows.clear();
    m_row_to_key_map.clear();
    m_key_to_row_map.clear();

    m_selections->deleteLater();
    m_selections = new SelectionsTableData(this);

    if (init.names.size() == 0) {
        endResetModel();
        return;
    }

    for (auto const& ci : init.names) {
        m_header << ci.name;
    }

    for (int i = 0; i < std::min(init.rows.size(), init.keys.size()); i++) {
        auto const& row   = init.rows[i];
        auto        key   = init.keys[i];
        auto        place = m_rows.size();
        m_rows << row;
        m_key_to_row_map[key]   = place;
        m_row_to_key_map[place] = key;
    }

    qDebug() << "Loaded data" << m_rows.size();

    m_selections->set(init.selections);

    qDebug() << "Loaded selections";

    endResetModel();
}

using CellType = std::variant<double, QString>;

static CellType get_item_single(QCborValue v) {
    if (v.isInteger() or v.isDouble()) {
        return v.toDouble();
    } else {
        return v.toString();
    }
}

inline CellType get_cell_from_array(QCborValue lv, int i) {

    auto a = lv.toArray();

    auto v = a.at(i);

    return get_item_single(v);
}

void RemoteTableData::on_table_rows_updated(QVector<int64_t> keys,
                                            QCborArray       rows) {

    qDebug() << Q_FUNC_INFO << keys
             << rows.toCborValue().toDiagnosticNotation();

    // TODO: optimize reset
    beginResetModel();

    // doing this split like this sucks. we have to check if all are new. if
    // they are, we can append them fast. if not, we should go line by line.
    // TODO: add fast append

    auto append_row = [&](int64_t key, QCborArray row) {
        qDebug() << "APPEND" << key << row;
        auto place = rowCount();

        m_row_to_key_map[place] = key;
        m_key_to_row_map[key]   = place;

        m_rows << row;
    };

    auto update_row = [&](int64_t key, QCborArray row) {
        qDebug() << "UPDATE" << key << row;

        auto place = m_key_to_row_map.at(key);

        m_rows[place] = row;
    };

    for (size_t key_i = 0; key_i < keys.size(); key_i++) {
        auto key  = keys[key_i];
        auto iter = m_key_to_row_map.find(key);
        if (iter == m_key_to_row_map.end()) {
            append_row(key, rows[key_i].toArray());
            continue;
        }

        // update
        update_row(key, rows[key_i].toArray());
    }

    endResetModel();
}

void RemoteTableData::on_table_rows_removed(QVector<int64_t> keys) {
    beginResetModel();

    // to delete we are finding the rows to remove, and then deleting the rows
    // from highest to lowest. this means we dont break index validity
    // this is all garbage
    // should be replaced with a real database thing.

    std::vector<size_t> places_to_delete;

    for (auto k : keys) {
        if (m_key_to_row_map.count(k) != 1) continue;

        places_to_delete.push_back(m_key_to_row_map.at(k));

        m_key_to_row_map.erase(k);
    }

    std::sort(places_to_delete.begin(),
              places_to_delete.end(),
              std::greater<size_t>());

    std::vector<size_t> key_list(m_rows.size());

    for (auto const& [r, k] : m_row_to_key_map) {
        key_list[r] = k;
    }

    for (auto place : places_to_delete) {
        m_rows.erase(m_rows.begin() + place);
        key_list.erase(key_list.begin() + place);
    }

    m_row_to_key_map.clear();
    m_key_to_row_map.clear();

    for (size_t row = 0; row < m_rows.size(); row++) {
        m_row_to_key_map[row]           = key_list[row];
        m_key_to_row_map[key_list[row]] = row;
    }

    endResetModel();
}

void RemoteTableData::on_table_selection_updated(noo::Selection const& sel) {
    m_selections->on_table_selection_updated(sel);
}

QStringList RemoteTableData::column_names() const {
    return m_header;
}

QVector<double> RemoteTableData::get_column(size_t i) const {
    QVector<double> ret;

    for (auto const& row : m_rows) {
        ret << row.at(i).toDouble();
    }

    return ret;
}

QVariant RemoteTableData::headerData(int             section,
                                     Qt::Orientation orientation,
                                     int             role) const {
    if (role != Qt::DisplayRole) return {};

    if (orientation == Qt::Orientation::Vertical) {
        return m_row_to_key_map.at(section);
    }

    return m_header.at(section);
}


int RemoteTableData::rowCount(QModelIndex const& parent) const {
    if (parent.isValid()) return 0;
    return m_rows.size();
}

int RemoteTableData::columnCount(QModelIndex const& parent) const {
    if (parent.isValid()) return 0;
    return m_header.size();
}

int64_t RemoteTableData::key_for_row(size_t i) const {
    return m_row_to_key_map.at(i);
}

int64_t RemoteTableData::row_for_key(size_t i) const {
    auto iter = m_key_to_row_map.find(i);
    if (iter == m_key_to_row_map.end()) return -1;
    return iter->second;
}

QVariant RemoteTableData::data(QModelIndex const& index, int role) const {
    // qDebug() << index << m_columns.size();
    if (!index.isValid()) return {};
    if (index.row() >= m_row_to_key_map.size()) return {};

    if (role != Qt::DisplayRole and role != Qt::EditRole) return {};

    return m_rows.at(index.row()).at(index.column()).toVariant();
}


bool RemoteTableData::setData(QModelIndex const& index,
                              QVariant const&    value,
                              int                role) {
    if (data(index, role) == value) return false;

    if (role != Qt::EditRole) return false;

    // we are only making SUGGESTIONS to the server to edit.
    // Thus, always return false.
    // This is bad form, but we have to send the whole row if we demand an
    // update so we get the current row, then we replace the new value in the
    // list, and send that along

    auto new_value = QCborValue::fromVariant(value);

    if (new_value.isUndefined()) return false;

    QCborArray row = m_rows[index.row()];

    row[index.column()] = new_value;

    auto key = m_row_to_key_map[index.row()];

    qDebug() << "Ask to update" << key << "with"
             << row.toCborValue().toDiagnosticNotation();

    emit ask_update_row(key, row);

    return false;
}

Qt::ItemFlags RemoteTableData::flags(QModelIndex const& index) const {
    if (!index.isValid()) return Qt::NoItemFlags;

    return Qt::ItemIsEditable | Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}
