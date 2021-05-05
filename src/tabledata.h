#ifndef TABLEDATA_H
#define TABLEDATA_H

#include <QAbstractTableModel>

#include <noo_server_interface.h> //steal the table info here

class RemoteTableData : public QAbstractTableModel {
    Q_OBJECT

    std::vector<noo::TableColumn> m_columns;

    std::vector<int64_t>                m_row_to_key_map;
    std::unordered_map<size_t, int64_t> m_key_to_row_map;

    std::unordered_map<std::string, noo::Selection> m_selections;

public:
    explicit RemoteTableData(QObject* parent = nullptr);

public:
    void on_table_initialize(noo::AnyVarListRef const& names,
                             noo::AnyVarRef            keys,
                             noo::AnyVarListRef const& data_cols,
                             noo::AnyVarListRef const& selections);

    void on_table_reset();
    void on_table_updated(noo::AnyVarRef keys, noo::AnyVarRef columns);
    void on_table_rows_removed(noo::AnyVarRef keys);
    void on_table_selection_updated(std::string_view, noo::SelectionRef const&);


public:
    QStringList column_names() const;

public:
    QVariant headerData(int             section,
                        Qt::Orientation orientation,
                        int             role = Qt::DisplayRole) const override;

    int rowCount(QModelIndex const& parent = QModelIndex()) const override;
    int columnCount(QModelIndex const& parent = QModelIndex()) const override;

    QVariant data(QModelIndex const& index,
                  int                role = Qt::DisplayRole) const override;


    //    bool setData(QModelIndex const& index,
    //                 QVariant const&    value,
    //                 int                role = Qt::EditRole) override;

    //    Qt::ItemFlags flags(QModelIndex const& index) const override;
};

#endif // TABLEDATA_H
