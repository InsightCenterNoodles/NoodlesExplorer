#include "tabledata.h"

#include "variant_tools.h"

#include <QDebug>

void SelectionsTableData::new_selection(std::string const&       name,
                                        noo::SelectionRef const& ref) {
    auto const location = m_selections.size();
    beginInsertRows(QModelIndex(), location, location);

    auto sel = m_selections.emplace_back(std::make_shared<SelectionSlot>());

    sel->name      = name;
    sel->selection = ref.to_selection();
    sel->index     = location;

    m_string_map[name] = sel;

    endInsertRows();
}

void SelectionsTableData::update_selection(std::string const&       name,
                                           noo::SelectionRef const& ref) {
    auto& place = m_string_map.at(name);

    place->selection = ref.to_selection();

    auto qindex = index(place->index, 0);
    dataChanged(qindex, qindex);
}

void SelectionsTableData::del_selection(std::string const& name) {
    auto place = m_string_map.at(name)->index;
    beginRemoveRows(QModelIndex(), place, place);

    m_string_map.erase(name);
    m_selections.erase(m_selections.begin() + place);
    endRemoveRows();
}

SelectionsTableData::SelectionsTableData(QObject* parent)
    : QAbstractTableModel(parent) { }

void SelectionsTableData::on_table_selection_updated(
    std::string_view         name,
    noo::SelectionRef const& ref) {
    auto ns = std::string(name);

    // is this an update?
    auto iter = m_string_map.find(ns);

    if (iter == m_string_map.end()) {
        // this is an add
        return new_selection(ns, ref);
    }

    // this is an update. if empty, its a delete
    if (ref.rows.size() == 0 and ref.row_ranges.empty()) {
        return del_selection(ns);
    }

    update_selection(ns, ref);
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

    return QString::fromStdString(m_selections.at(index.row())->name);
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

using CellType = std::variant<double, std::string_view>;

static CellType get_item_single(noo::AnyVarRef v) {
    qDebug() << Q_FUNC_INFO;
    return VMATCH_W(
        noo::visit,
        v,
        VCASE(int64_t sp) {
            qDebug() << "INT";
            return CellType((double)sp);
        },
        VCASE(double sp) {
            qDebug() << "DBL";
            return CellType(sp);
        },
        VCASE(std::string_view sp) {
            qDebug() << "STR";
            return CellType(sp);
        },
        VCASE(auto const& t) {
            qDebug() << (typeid(t).name());
            return CellType(0.0);
        })
}

inline CellType get_cell_from_array(noo::AnyVarRef v, int i) {
    qDebug() << Q_FUNC_INFO << i;
    return VMATCH_W(
        noo::visit,
        v,
        VCASE(std::span<int64_t const> sp) {
            qDebug() << "INT";
            return CellType((double)noo::get_or_default(sp, i));
        },
        VCASE(std::span<double const> sp) {
            qDebug() << "DBL";
            return CellType(noo::get_or_default(sp, i));
        },
        VCASE(noo::AnyVarListRef sp) {
            qDebug() << "VLIST";
            if (i < 0 or i >= sp.size()) { return CellType(0.0); }

            auto c = sp[i];
            return get_item_single(c);
        },
        VCASE(auto const& t) {
            qDebug() << (typeid(t).name());
            return CellType(0.0);
        })
}

void RemoteTableData::on_table_updated(noo::AnyVarRef keys,
                                       noo::AnyVarRef columns) {
    qDebug() << Q_FUNC_INFO << QString::fromStdString(keys.dump_string())
             << QString::fromStdString(columns.dump_string());
    // TODO: optimize reset
    beginResetModel();

    auto       actual_keys      = keys.coerce_int_list();
    auto const actual_keys_span = actual_keys.span();

    auto l = columns.to_vector();
    if (l.size() != m_columns.size()) return;


    // doing this split like this sucks. we have to check if all are new. if
    // they are, we can append them fast. if not, we should go line by line.
    // TODO: add fast append

    auto append_row = [&](int64_t ki) {
        qDebug() << "APPEND" << ki;
        auto key = actual_keys_span[ki];

        auto current_row = rowCount();

        m_row_to_key_map.push_back(key);
        m_key_to_row_map[key] = current_row;


        l.for_each([&](auto i, noo::AnyVarRef a) {
            auto cell = get_cell_from_array(a, ki);

            std::visit([&](auto const& c) { m_columns.at(i).append(c); }, cell);
        });
    };

    auto update_row = [&](int64_t ki) {
        qDebug() << "UPDATE" << ki;
        auto key = actual_keys_span[ki];

        auto row = m_key_to_row_map.at(key);

        l.for_each([&](auto i, noo::AnyVarRef a) {
            auto cell = get_cell_from_array(a, ki);

            std::visit([&](auto const& c) { m_columns.at(i).set(row, c); },
                       cell);
        });
    };

    for (size_t key_i = 0; key_i < actual_keys_span.size(); key_i++) {
        auto key  = actual_keys_span[key_i];
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

    for (size_t row = 0; row < m_row_to_key_map.size(); row++) {
        auto k              = m_row_to_key_map[row];
        m_key_to_row_map[k] = row;
    }

    endResetModel();
}

void RemoteTableData::on_table_selection_updated(std::string_view         key,
                                                 noo::SelectionRef const& sel) {
    m_selections->on_table_selection_updated(key, sel);
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
        return QString::fromStdString(item);
    } else {
        auto item = column_data.as_doubles()[index.row()];
        return item;
    }
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

    noo::AnyVarList row;

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
        row[index.column()] = value.toString().toStdString();
    }

    auto key = m_row_to_key_map[index.row()];

    emit ask_update_row(key, row);

    return false;
}

Qt::ItemFlags RemoteTableData::flags(QModelIndex const& index) const {
    if (!index.isValid()) return Qt::NoItemFlags;

    return Qt::ItemIsEditable | Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}
