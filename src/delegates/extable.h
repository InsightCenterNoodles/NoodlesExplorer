#ifndef EXTABLE_H
#define EXTABLE_H

#include "componentlistmodel.h"
#include "delegates.h"

#include <noo_client_interface.h>

class RemoteTableData;

class ExTable : public nooc::TableDelegate {
    Q_OBJECT

    std::shared_ptr<RemoteTableData> m_data;

    bool m_subscribed = false;

public:
    static QStringList header();

    ExTable(noo::TableID id, nooc::TableInit const& md);

    ~ExTable();

    // void prepare_delete() override { unregister(); }

    int      get_id() const;
    int      get_id_gen() const;
    QString  get_name() const;
    QVariant get_column(int c) const;

    void on_update(nooc::TableUpdate const&) override;

    std::shared_ptr<RemoteTableData> table_data() const { return m_data; }

    bool is_subscribed() const { return m_subscribed; }

public:
    void on_table_initialize(QCborArray const& names,
                             QCborValue        keys,
                             QCborArray const& data_cols,
                             QCborArray const& selections) override;

    void on_table_reset() override;
    void on_table_updated(QCborValue keys, QCborValue columns) override;
    void on_table_rows_removed(QCborValue keys) override;
    void on_table_selection_updated(QString, noo::Selection const&) override;

signals:
    void fetch_new_remote_table_data();
};
#endif // EXTABLE_H
