#include "extable.h"

#include "tabledata.h"

#include <noo_common.h>

#include <QDebug>

QStringList ExTable::header() {
    return { "ID", "Name" };
}

ExTable::ExTable(noo::TableID id, nooc::TableInit const& md)
    : nooc::TableDelegate(id, md) { }

ExTable::~ExTable() = default;

int ExTable::get_id() const {
    return this->id().id_slot;
}
int ExTable::get_id_gen() const {
    return this->id().id_gen;
}
QString ExTable::get_name() const {
    return info().name;
}

QVariant ExTable::get_column(int c) const {
    switch (c) {
    case 0: return get_id();
    case 1: return get_name();
    }
    return {};
}

void ExTable::on_update(nooc::TableUpdate const& md) {
    // nothing atm
}

void ExTable::on_table_initialize(QCborArray const& names,
                                  QCborValue        keys,
                                  QCborArray const& data_cols,
                                  QCborArray const& selections) {

    m_subscribed = true;

    qDebug() << Q_FUNC_INFO;


    if (names.size() != data_cols.size()) return;
    if (names.size() == 0) return;

    m_data = std::make_shared<RemoteTableData>();

    m_data->on_table_initialize(names, keys, data_cols, selections);

    connect(m_data.get(),
            &RemoteTableData::ask_update_row,
            [this](int64_t key, QCborArray& l) {
                this->request_row_update(key, std::move(l));
            });

    emit fetch_new_remote_table_data();
}

void ExTable::on_table_reset() {
    qDebug() << Q_FUNC_INFO;
    if (m_data) m_data->on_table_reset();
}

void ExTable::on_table_updated(QCborValue keys, QCborValue columns) {
    qDebug() << Q_FUNC_INFO;
    if (m_data) m_data->on_table_updated(keys, columns);
}

void ExTable::on_table_rows_removed(QCborValue keys) {
    qDebug() << Q_FUNC_INFO;
    if (m_data) m_data->on_table_rows_removed(keys);
}
void ExTable::on_table_selection_updated(QString s, noo::Selection const& r) {
    qDebug() << Q_FUNC_INFO;
    if (m_data) m_data->on_table_selection_updated(s, r);
}
