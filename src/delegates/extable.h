#ifndef EXTABLE_H
#define EXTABLE_H

#include "componentlistmodel.h"
#include "delegates.h"

#include <noo_client_interface.h>

class RemoteTableData;

class ExTable : public nooc::TableDelegate, public ComponentListItem {
    Q_OBJECT

    QString m_name;

    std::vector<nooc::MethodDelegatePtr> m_methods;

    std::shared_ptr<RemoteTableData> m_data;

    void set_from(nooc::TableData const& md);

    bool m_subscribed = false;

public:
    static QStringList header();

    ExTable(noo::TableID                        id,
            nooc::TableData const&              md,
            std::shared_ptr<ComponentListModel> list);

    ~ExTable();

    void prepare_delete() override { unregister(); }

    int      get_id() const override;
    int      get_id_gen() const override;
    QString  get_name() const override;
    QVariant get_column(int c) const override;

    void on_update(nooc::TableData const&) override;

    std::shared_ptr<RemoteTableData> table_data() const { return m_data; }

    bool is_subscribed() const { return m_subscribed; }

public:
    void on_table_initialize(noo::AnyVarListRef const& names,
                             noo::AnyVarRef            keys,
                             noo::AnyVarListRef const& data_cols,
                             noo::AnyVarListRef const& selections) override;

    void on_table_reset() override;
    void on_table_updated(noo::AnyVarRef keys, noo::AnyVarRef columns) override;
    void on_table_rows_removed(noo::AnyVarRef keys) override;
    void on_table_selection_updated(std::string_view,
                                    noo::SelectionRef const&) override;

signals:
    void fetch_new_remote_table_data();
};
#endif // EXTABLE_H
