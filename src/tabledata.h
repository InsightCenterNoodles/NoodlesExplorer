#ifndef TABLEDATA_H
#define TABLEDATA_H

#include <QAbstractTableModel>

#include <noo_server_interface.h> //steal the table info here

class SelectionsTableData : public QAbstractTableModel {
    Q_OBJECT

    struct SelectionSlot {
        std::string    name;
        noo::Selection selection;
        size_t         index;
    };

    using SelectionSlotPtr = std::shared_ptr<SelectionSlot>;

    std::vector<SelectionSlotPtr>                     m_selections;
    std::unordered_map<std::string, SelectionSlotPtr> m_string_map;

    void new_selection(std::string const&, noo::SelectionRef const&);
    void update_selection(std::string const&, noo::SelectionRef const&);
    void del_selection(std::string const&);

public:
    explicit SelectionsTableData(QObject* parent = nullptr);

    void on_table_selection_updated(std::string_view, noo::SelectionRef const&);

    SelectionSlot const* slot_at(size_t) const;

    void clear();

public:
    QVariant headerData(int             section,
                        Qt::Orientation orientation,
                        int             role = Qt::DisplayRole) const override;

    int rowCount(QModelIndex const& parent = QModelIndex()) const override;
    int columnCount(QModelIndex const& parent = QModelIndex()) const override;

    QVariant data(QModelIndex const& index,
                  int                role = Qt::DisplayRole) const override;

    Qt::ItemFlags flags(QModelIndex const& index) const override;
};

// =============================================================================

class RemoteTableData : public QAbstractTableModel {
    Q_OBJECT

    std::vector<noo::TableColumn> m_columns;

    std::vector<int64_t>                m_row_to_key_map;
    std::unordered_map<size_t, int64_t> m_key_to_row_map;

    SelectionsTableData* m_selections = nullptr;

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
    auto const& column(int i) const { return m_columns.at(i); }
    auto        column_count() const { return m_columns.size(); }

    int64_t key_for_row(size_t i) const;
    int64_t row_for_key(size_t i) const;

    SelectionsTableData* selections() const { return m_selections; }

public:
    QVariant headerData(int             section,
                        Qt::Orientation orientation,
                        int             role = Qt::DisplayRole) const override;

    int rowCount(QModelIndex const& parent = QModelIndex()) const override;
    int columnCount(QModelIndex const& parent = QModelIndex()) const override;

    QVariant data(QModelIndex const& index,
                  int                role = Qt::DisplayRole) const override;


    bool setData(QModelIndex const& index,
                 QVariant const&    value,
                 int                role = Qt::EditRole) override;

    Qt::ItemFlags flags(QModelIndex const& index) const override;

signals:
    void ask_update_row(int64_t key, noo::AnyVarList&);
};

#endif // TABLEDATA_H
