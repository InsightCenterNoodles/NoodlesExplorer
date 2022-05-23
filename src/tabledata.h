#ifndef TABLEDATA_H
#define TABLEDATA_H

#include <noo_client_interface.h>

#include <QAbstractTableModel>

#include <span>


class TableColumn : public std::variant<QVector<double>, QStringList> {
public:
    QString name;

    using variant::variant;

    size_t size() const;
    bool   is_string() const;

    std::span<double const> as_doubles() const;
    QStringList const&      as_string() const;

    void append(std::span<double const>);
    void append(QStringList const&);
    void append(QCborArray const&);
    void append(double);
    void append(QString);

    void set(size_t row, double);
    void set(size_t row, QCborValue);
    void set(size_t row, QString);

    void erase(size_t row);

    void clear();
};


class SelectionsTableData : public QAbstractTableModel {
    Q_OBJECT

    struct SelectionSlot {
        QString        name;
        noo::Selection selection;
        size_t         index;
    };

    using SelectionSlotPtr = std::shared_ptr<SelectionSlot>;

    std::vector<SelectionSlotPtr>    m_selections;
    QHash<QString, SelectionSlotPtr> m_string_map;

    void new_selection(QString const&, noo::Selection const&);
    void update_selection(QString const&, noo::Selection const&);
    void del_selection(QString const&);

public:
    explicit SelectionsTableData(QObject* parent = nullptr);

    void on_table_selection_updated(QString, noo::Selection const&);

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

    QVector<TableColumn> m_columns;

    QVector<int64_t>                    m_row_to_key_map;
    std::unordered_map<size_t, int64_t> m_key_to_row_map;

    SelectionsTableData* m_selections = nullptr;

public:
    explicit RemoteTableData(QObject* parent = nullptr);

public:
    void on_table_initialize(QCborArray const& names,
                             QCborValue        keys,
                             QCborArray const& data_cols,
                             QCborArray const& selections);

    void on_table_reset();
    void on_table_updated(QCborValue keys, QCborValue columns);
    void on_table_rows_removed(QCborValue keys);
    void on_table_selection_updated(QString, noo::Selection const&);


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
    void ask_update_row(int64_t key, QCborArray&);
};

#endif // TABLEDATA_H
