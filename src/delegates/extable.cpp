#include "extable.h"

#include "tabledata.h"

#include <noo_common.h>

#include <QDebug>


// =============================================================================

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

void ExTable::on_update(nooc::TableUpdate const& /*md*/) {
    // nothing atm
}

QAbstractTableModel* ExTable::table_data() const {
    return m_data.get();
}
QAbstractTableModel* ExTable::selections_model() const {
    if (m_data) return m_data->selections();
    return nullptr;
}

void ExTable::on_table_subscribed(nooc::TableDataInit const& init) {

    QStringList all_cols;
    for (auto const& info : init.names) {
        all_cols << info.name;
    }

    qDebug() << Q_FUNC_INFO << all_cols;

    if (init.names.size() == 0) return;

    m_data = std::make_unique<RemoteTableData>();

    m_data->on_table_subscribed(init);

    m_subscribed = true;

    connect(m_data.get(),
            &RemoteTableData::ask_update_row,
            this,
            [this](int64_t key, QCborArray& l) {
                this->request_row_update(key, std::move(l));
            });

    emit fetch_new_remote_table_data();
}

void ExTable::on_table_reset(nooc::TableDataInit const& init) {
    qDebug() << Q_FUNC_INFO;
    if (m_data) m_data->on_table_reset(init);
}

void ExTable::on_table_rows_updated(QVector<int64_t> keys, QCborArray rows) {
    qDebug() << Q_FUNC_INFO;
    if (m_data) m_data->on_table_rows_updated(keys, rows);
}

void ExTable::on_table_rows_removed(QVector<int64_t> keys) {
    qDebug() << Q_FUNC_INFO;
    if (m_data) m_data->on_table_rows_removed(keys);
}
void ExTable::on_table_selection_updated(noo::Selection const& r) {
    qDebug() << Q_FUNC_INFO;
    if (m_data) m_data->on_table_selection_updated(r);
}
