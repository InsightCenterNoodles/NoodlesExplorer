#include "extable.h"

#include "tabledata.h"

#include <noo_common.h>

#include <QDebug>

void ExTable::set_from(nooc::TableData const& md) {
    if (md.name) {
        m_name = noo::to_qstring(*md.name);
    } else {
        m_name = id().to_qstring();
    }

    if (md.method_list) m_methods = *md.method_list;
}

QStringList ExTable::header() {
    return { "ID", "Name" };
}

ExTable::ExTable(noo::TableID                        id,
                 nooc::TableData const&              md,
                 std::shared_ptr<ComponentListModel> list)
    : nooc::TableDelegate(id, md), ComponentListItem(list) {

    set_from(md);
}

ExTable::~ExTable() = default;

int ExTable::get_id() const {
    return this->id().id_slot;
}
int ExTable::get_id_gen() const {
    return this->id().id_gen;
}
QString ExTable::get_name() const {
    return m_name;
}

QVariant ExTable::get_column(int c) const {
    switch (c) {
    case 0: return get_id();
    case 1: return get_name();
    }
    return {};
}

void ExTable::on_update(nooc::TableData const& md) {
    set_from(md);
}

void ExTable::on_table_initialize(noo::AnyVarListRef const& names,
                                  noo::AnyVarRef            keys,
                                  noo::AnyVarListRef const& data_cols,
                                  noo::AnyVarListRef const& selections) {

    m_subscribed = true;

    qDebug() << Q_FUNC_INFO;


    if (names.size() != data_cols.size()) return;
    if (names.size() == 0) return;

    m_data = std::make_shared<RemoteTableData>();

    m_data->on_table_initialize(names, keys, data_cols, selections);

    connect(m_data.get(),
            &RemoteTableData::ask_update_row,
            [this](int64_t key, noo::AnyVarList& l) {
                this->request_row_update(key, std::move(l));
            });

    emit fetch_new_remote_table_data();
}

void ExTable::on_table_reset() {
    qDebug() << Q_FUNC_INFO;
    if (m_data) m_data->on_table_reset();
}

void ExTable::on_table_updated(noo::AnyVarRef keys, noo::AnyVarRef columns) {
    qDebug() << Q_FUNC_INFO;
    if (m_data) m_data->on_table_updated(keys, columns);
}

void ExTable::on_table_rows_removed(noo::AnyVarRef keys) {
    qDebug() << Q_FUNC_INFO;
    if (m_data) m_data->on_table_rows_removed(keys);
}
void ExTable::on_table_selection_updated(std::string_view         s,
                                         noo::SelectionRef const& r) {
    qDebug() << Q_FUNC_INFO;
    if (m_data) m_data->on_table_selection_updated(s, r);
}
