#include "tabledata.h"

#include "variant_tools.h"

#include <QDebug>

void SelectionsTableData::new_selection(QString const&        name,
                                        noo::Selection const& ref) {
    auto const location = m_selections.size();
    beginInsertRows(QModelIndex(), location, location);

    auto sel = m_selections.emplace_back(std::make_shared<SelectionSlot>());

    sel->name      = name;
    sel->selection = ref;
    sel->index     = location;

    m_string_map[name] = sel;

    endInsertRows();
}

void SelectionsTableData::update_selection(QString const&        name,
                                           noo::Selection const& ref) {
    auto& place = m_string_map[name];

    place->selection = ref;

    auto qindex = index(place->index, 0);
    dataChanged(qindex, qindex);
}

void SelectionsTableData::del_selection(QString const& name) {
    auto place = m_string_map[name]->index;
    beginRemoveRows(QModelIndex(), place, place);

    m_string_map.remove(name);
    m_selections.erase(m_selections.begin() + place);
    endRemoveRows();
}

SelectionsTableData::SelectionsTableData(QObject* parent)
    : QAbstractTableModel(parent) { }

void SelectionsTableData::on_table_selection_updated(
    QString               name,
    noo::Selection const& ref) {

    // is this an update?
    auto iter = m_string_map.find(name);

    if (iter == m_string_map.end()) {
        // this is an add
        return new_selection(name, ref);
    }

    // this is an update. if empty, its a delete
    if (ref.rows.size() == 0 and ref.row_ranges.empty()) {
        return del_selection(name);
    }

    update_selection(name, ref);
}

SelectionsTableData::SelectionSlot const*
SelectionsTableData::slot_at(size_t i) const {
    if (i >= m_selections.size()) return nullptr;

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

    return m_selections.at(index.row())->name;
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

void RemoteTableData::on_table_initialize(QCborArray const& names,
                                          QCborValue        keys,
                                          QCborArray const& data_cols,
                                          QCborArray const& selections) {

    qDebug() << Q_FUNC_INFO;

    on_table_reset();

    beginResetModel();
    if (names.size() != data_cols.size()) return;
    if (names.size() == 0) return;

    for (size_t c_i = 0; c_i < names.size(); c_i++) {
        auto& new_c = m_columns.emplace_back();

        auto data_col = data_cols[c_i];

        if (!data_col.isArray()) { continue; }

        // probe first

        auto list = data_col.toArray();

        if (list.size() == 0) continue;

        auto first = list[0];

        if (first.isInteger() or first.isDouble()) {
            auto coerced = noo::coerce_to_real_list(data_col);

            new_c      = coerced;
            new_c.name = names[c_i].toString(QString("Column %1").arg(c_i));
            continue;
        }

        if (!first.isString()) continue;

        QStringList strs(list.size());

        for (int r_i = 0; r_i < list.size(); r_i++) {
            strs[r_i] = list[r_i].toString();
        }

        new_c      = std::move(strs);
        new_c.name = names[c_i].toString();
    }

    qDebug() << "Loaded data";

    {
        auto lk = noo::coerce_to_int_list(keys);

        m_row_to_key_map << lk;

        for (size_t ri = 0; ri < m_row_to_key_map.size(); ri++) {
            m_key_to_row_map[m_row_to_key_map[ri]] = ri;
        }
    }

    qDebug() << "Loaded keys" << m_row_to_key_map.size();

    for (size_t si = 0; si < selections.size(); si++) {
        auto pair = selections[si].toArray();

        if (pair.size() < 2) continue;

        auto key       = pair[0].toString();
        auto selection = noo::Selection(pair[1].toMap());

        if (key.isEmpty()) continue;

        m_selections->on_table_selection_updated(key, selection);
    }

    qDebug() << "Loaded selections";

    endResetModel();
}

void RemoteTableData::on_table_reset() {
    beginResetModel();

    for (auto& c : m_columns) {
        c.clear();
    }

    m_row_to_key_map.clear();
    m_key_to_row_map.clear();
    m_selections->clear();

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

void RemoteTableData::on_table_updated(QCborValue keys, QCborValue columns) {
    qDebug() << Q_FUNC_INFO << keys.toDiagnosticNotation()
             << columns.toDiagnosticNotation();
    // TODO: optimize reset
    beginResetModel();

    auto const actual_keys = noo::coerce_to_int_list(keys);

    auto const l = columns.toArray();
    if (l.size() != m_columns.size()) return;


    // doing this split like this sucks. we have to check if all are new. if
    // they are, we can append them fast. if not, we should go line by line.
    // TODO: add fast append

    auto append_row = [&](int64_t ki) {
        qDebug() << "APPEND" << ki;
        auto key = actual_keys[ki];

        auto current_row = rowCount();

        m_row_to_key_map.push_back(key);
        m_key_to_row_map[key] = current_row;

        for (int i = 0; i < l.size(); i++) {
            auto a    = l[i];
            auto cell = get_cell_from_array(a, ki);

            std::visit([&](auto const& c) { m_columns[i].append(c); }, cell);
        }
    };

    auto update_row = [&](int64_t ki) {
        qDebug() << "UPDATE" << ki;
        auto key = actual_keys[ki];

        auto row = m_key_to_row_map.at(key);

        for (int i = 0; i < l.size(); i++) {
            auto a    = l[i];
            auto cell = get_cell_from_array(a, ki);

            std::visit([&](auto const& c) { m_columns[i].set(row, c); }, cell);
        }
    };

    for (size_t key_i = 0; key_i < actual_keys.size(); key_i++) {
        auto key  = actual_keys[key_i];
        auto iter = m_key_to_row_map.find(key);
        if (iter == m_key_to_row_map.end()) {
            append_row(key_i);
            continue;
        }

        // update
        update_row(key_i);
    }

    endResetModel();
}

void RemoteTableData::on_table_rows_removed(QCborValue keys) {
    beginResetModel();
    auto const actual_keys = noo::coerce_to_int_list(keys);

    // to delete we are finding the rows to remove, and then deleting the rows
    // from highest to lowest. this means we dont break index validity

    std::vector<size_t> rows_to_delete;

    for (auto k : actual_keys) {
        if (m_key_to_row_map.count(k) != 1) continue;

        rows_to_delete.push_back(m_key_to_row_map.at(k));

        m_key_to_row_map.erase(k);
    }

    std::sort(
        rows_to_delete.begin(), rows_to_delete.end(), std::greater<size_t>());

    for (auto row : rows_to_delete) {
        m_row_to_key_map.erase(m_row_to_key_map.begin() + row);
        for (auto& c : m_columns) {
            c.erase(row);
        }
    }

    for (size_t row = 0; row < m_row_to_key_map.size(); row++) {
        auto k              = m_row_to_key_map[row];
        m_key_to_row_map[k] = row;
    }

    endResetModel();
}

void RemoteTableData::on_table_selection_updated(QString               key,
                                                 noo::Selection const& sel) {
    m_selections->on_table_selection_updated(key, sel);
}

QStringList RemoteTableData::column_names() const {
    QStringList ret;

    for (auto const& m : m_columns) {
        ret << m.name;
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

    return m_columns.at(section).name;
}


int RemoteTableData::rowCount(QModelIndex const& parent) const {
    if (parent.isValid()) return 0;
    return m_row_to_key_map.size();
}

int RemoteTableData::columnCount(QModelIndex const& parent) const {
    if (parent.isValid()) return 0;
    return m_columns.size();
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

    auto const& column_data = m_columns.at(index.column());

    if (column_data.is_string()) {
        auto const& item = column_data.as_string()[index.row()];
        return item;
    }

    auto item = column_data.as_doubles()[index.row()];
    return item;
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

    QCborArray row;

    for (auto const& c : m_columns) {
        if (c.is_string()) {
            row.push_back(c.as_string()[index.row()]);
        } else {
            row.push_back(c.as_doubles()[index.row()]);
        }
    }

    bool ok          = false;
    auto clean_value = value.toDouble(&ok);

    if (ok) {
        row[index.column()] = clean_value;
    } else {
        row[index.column()] = value.toString();
    }

    auto key = m_row_to_key_map[index.row()];

    emit ask_update_row(key, row);

    return false;
}

Qt::ItemFlags RemoteTableData::flags(QModelIndex const& index) const {
    if (!index.isValid()) return Qt::NoItemFlags;

    return Qt::ItemIsEditable | Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}
