#ifndef COMPONENTLISTMODEL_H
#define COMPONENTLISTMODEL_H

#include <QAbstractTableModel>
#include <QSet>
#include <QSortFilterProxyModel>

class ComponentListModel;

// be careful not to double inherit from QObject...

class ComponentListItem {
    std::shared_ptr<ComponentListModel> m_host;

protected:
    void notify();

    void unregister();

public:
    ComponentListItem(std::shared_ptr<ComponentListModel> host);
    ~ComponentListItem();

    // column 0 is the id
    // column 1 is the name

    virtual int     get_id() const     = 0;
    virtual int     get_id_gen() const = 0;
    virtual QString get_name() const   = 0;

    virtual QVariant get_column(int) const = 0;
};


class ComponentListModel : public QAbstractTableModel {
    Q_OBJECT

    QStringList m_header;

    QVector<ComponentListItem*>    m_items;
    QHash<ComponentListItem*, int> m_item_place_map;

public:
    explicit ComponentListModel(QStringList header, QObject* parent = nullptr);

    // Header:
    QVariant headerData(int             section,
                        Qt::Orientation orientation,
                        int             role = Qt::DisplayRole) const override;

    // Basic functionality:
    int rowCount(QModelIndex const& parent = QModelIndex()) const override;
    int columnCount(QModelIndex const& parent = QModelIndex()) const override;

    QVariant data(QModelIndex const& index,
                  int                role = Qt::DisplayRole) const override;


    QHash<int, QByteArray> roleNames() const override;

    ComponentListItem* get_item(int index) const;

public: // For ComponentListItem
    void register_item(ComponentListItem*);
    void unregister_item(ComponentListItem*);

    void item_in_list_updated(ComponentListItem*);
};

class ComponentListFilter : public QSortFilterProxyModel {
    Q_OBJECT

    QSet<int> m_accept_columns;

public:
    explicit ComponentListFilter(QSet<int> columns, QObject* p = nullptr);

protected:
    bool filterAcceptsColumn(int                source_column,
                             QModelIndex const& source_parent) const override;
};


#endif // COMPONENTLISTMODEL_H
