#include "tabledata.h"

#include <QDebug>

RemoteTableData::RemoteTableData(QObject* parent)
    : QAbstractTableModel(parent) { }

void RemoteTableData::on_table_initialize(
    noo::AnyVarListRef const& names,
    noo::AnyVarRef            keys,
    noo::AnyVarListRef const& data_cols,
    noo::AnyVarListRef const& selections) {

    qDebug() << Q_FUNC_INFO;

    on_table_reset();

    beginResetModel();
    if (names.size() != data_cols.size()) return;
    if (names.size() == 0) return;

    for (size_t c_i = 0; c_i < names.size(); c_i++) {
        auto& new_c = m_columns.emplace_back();

        auto data_col = data_cols[c_i];

        if (data_col.has_real_list()) {
            new_c      = noo::span_to_vector(data_col.to_real_list());
            new_c.name = names[c_i].to_string();
            continue;
        }

        if (!data_col.has_list()) { continue; }

        // probe first

        auto list = data_col.to_vector();

        if (list.size() == 0) continue;

        auto first = list[0];

        if (first.has_real()) {
            auto coerced = data_col.coerce_real_list();

            auto* p = std::get_if<std::vector<double>>(&coerced.storage);

            assert(p);

            new_c      = std::move(*p);
            new_c.name = names[c_i].to_string();
            continue;
        }

        if (!first.has_string()) continue;

        std::vector<std::string> strs(list.size());

        for (size_t r_i = 0; r_i < list.size(); r_i++) {
            strs[r_i] = list[r_i].to_string();
        }

        new_c      = std::move(strs);
        new_c.name = names[c_i].to_string();
    }

    qDebug() << "Loaded data";

    {
        auto lk = keys.coerce_int_list();

        m_row_to_key_map.insert(m_row_to_key_map.end(), lk.begin(), lk.end());

        for (size_t ri = 0; ri < m_row_to_key_map.size(); ri++) {
            m_key_to_row_map[m_row_to_key_map[ri]] = ri;
        }
    }

    qDebug() << "Loaded keys" << m_row_to_key_map.size();

    for (size_t si = 0; si < selections.size(); si++) {
        auto pair = selections[si].to_vector();

        if (pair.size() < 2) continue;

        auto key       = pair[0].to_string();
        auto selection = noo::SelectionRef(pair[1]);

        if (key.empty()) continue;

        m_selections[std::string(key)] = selection.to_selection();
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
    m_selections.clear();

    endResetModel();
}

void RemoteTableData::on_table_updated(noo::AnyVarRef keys,
                                       noo::AnyVarRef columns) {
    // TODO: optimize reset
    beginResetModel();

    auto actual_keys = keys.coerce_int_list();

    auto l = columns.to_vector();

    if (l.size() != m_columns.size()) return;

    l.for_each([&](auto i, noo::AnyVarRef a) {
        if (a.has_real_list()) {
            auto real_list = a.coerce_real_list();
            m_columns[i].append(real_list.span());
        } else {
            m_columns[i].append(a.to_vector());
        }
    });

    auto current_row = rowCount();

    for (auto k : actual_keys) {
        m_row_to_key_map.push_back(k);
        m_key_to_row_map[k] = current_row;
        current_row++;
    }

    endResetModel();
}

void RemoteTableData::on_table_rows_removed(noo::AnyVarRef keys) {
    beginResetModel();
    auto actual_keys = keys.coerce_int_list();

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

    for (size_t row = 0; row < m_row_to_key_map.size();) {
        auto k              = m_row_to_key_map[row];
        m_key_to_row_map[k] = row;
    }

    endResetModel();
}

void RemoteTableData::on_table_selection_updated(std::string_view         key,
                                                 noo::SelectionRef const& sel) {
    m_selections[std::string(key)] = sel.to_selection();
}

QStringList RemoteTableData::column_names() const {
    QStringList ret;

    for (auto const& m : m_columns) {
        ret << QString::fromStdString(m.name);
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

    return QString::fromStdString(m_columns.at(section).name);
}


int RemoteTableData::rowCount(QModelIndex const& parent) const {
    if (parent.isValid()) return 0;
    return m_row_to_key_map.size();
}

int RemoteTableData::columnCount(QModelIndex const& parent) const {
    if (parent.isValid()) return 0;
    return m_columns.size();
}

QVariant RemoteTableData::data(QModelIndex const& index, int role) const {
    // qDebug() << index << m_columns.size();
    if (!index.isValid()) return {};
    if (index.row() >= m_row_to_key_map.size()) return {};

    if (role != Qt::DisplayRole and role != Qt::EditRole) return {};

    auto const& column_data = m_columns.at(index.column());

    if (column_data.is_string()) {
        auto const& item = column_data.as_string()[index.row()];
        return QString::fromStdString(item);
    } else {
        auto item = column_data.as_doubles()[index.row()];
        return item;
    }
}


// bool TableData::setData(QModelIndex const& index,
//                        QVariant const&    value,
//                        int                role) {
//    if (data(index, role) == value) return false;

//    auto& item = m_records[index.row()];

//    int location = -1;

//    if (role >= Qt::UserRole) {
//        location = role - Qt::UserRole;
//    } else {
//        location = index.column();
//    }

//    if (location >= m_header.size()) return false;

//    bool ok = record_runtime_set(item, location, value);

//    if (!ok) return false;

//    emit dataChanged(index, index, QVector<int>() << role);
//    return true;
//}

// Qt::ItemFlags TableData::flags(QModelIndex const& index) const {
//    if (!index.isValid()) return Qt::NoItemFlags;

//    bool can_edit = false;

//    tuple_get(Record::meta, index.column(), [&can_edit](auto const& v) {
//        can_edit = v.editable;
//    });

//    if (!can_edit) return Qt::NoItemFlags;

//    return Qt::ItemIsEditable;
//}
