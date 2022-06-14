#ifndef COMPONENTLISTMODEL_H
#define COMPONENTLISTMODEL_H

#include <QAbstractTableModel>
#include <QSet>
#include <QSortFilterProxyModel>

// some jungle gym work for templated QObjects...
class ComponentListModelBase : public QAbstractTableModel {
    Q_OBJECT

public:
    using QAbstractTableModel::QAbstractTableModel;

public slots:
    virtual void unregister_item() = 0;

    virtual void item_in_list_updated() = 0;
};

template <class T>
class ComponentListModel : public ComponentListModelBase {

    QStringList m_header;

    QVector<T*>    m_items;
    QHash<T*, int> m_item_place_map;

    void register_item(T* item) {
        Q_ASSERT(item);

        auto place = m_items.size();

        beginInsertRows({}, place, place);

        m_items.push_back(item);
        m_item_place_map[item] = place;

        endInsertRows();
    }

public:
    explicit ComponentListModel(QObject* parent = nullptr)
        : ComponentListModelBase(parent), m_header(T::header()) { }

    // Header:
    QVariant headerData(int             section,
                        Qt::Orientation orientation,
                        int             role = Qt::DisplayRole) const override {
        // only section is 0

        if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
            return m_header.value(section);
        }

        return {};
    }

    // Basic functionality:
    int rowCount(QModelIndex const& parent = QModelIndex()) const override {
        if (parent.isValid()) return 0;

        return m_items.size();
    }
    int columnCount(QModelIndex const& parent = QModelIndex()) const override {
        if (parent.isValid()) return 0;

        return m_header.size();
    }

    QVariant data(QModelIndex const& index,
                  int                role = Qt::DisplayRole) const override {
        if (!index.isValid()) return {};

        auto item = m_items.at(index.row());

        Q_ASSERT(item);

        if (role == Qt::DisplayRole or role == Qt::EditRole) {
            return item->get_column(index.column());
        }

        if (role >= Qt::UserRole) {
            int index = role - Qt::UserRole;

            return item->get_column(index);
        }

        return QVariant();
    }


    QHash<int, QByteArray> roleNames() const override {
        static const auto roles = [&]() {
            QHash<int, QByteArray> ret;

            for (int i = 0; i < m_header.size(); i++) {
                ret[Qt::UserRole + i] = m_header[i].toLower().toLocal8Bit();
            }

            ret[Qt::DisplayRole] = "display";

            return ret;
        }();

        return roles;
    }

    T* get_item(int index) const { return m_items.value(index); }

public: // For ComponentListItem
    template <class... Args>
    std::unique_ptr<T> add_item(Args&&... args) {
        auto ptr = std::make_unique<T>(std::forward<Args>(args)...);
        register_item(ptr.get());
        connect(ptr.get(),
                &QObject::destroyed,
                this,
                &ComponentListModel::unregister_item);

        connect(ptr.get(),
                &T::updated,
                this,
                &ComponentListModel::item_in_list_updated);

        return ptr;
    }

    void unregister_item() override {
        auto item = static_cast<T*>(sender());

        Q_ASSERT(item);

        auto iter = m_item_place_map.find(item);

        Q_ASSERT(iter != m_item_place_map.end());

        auto place = iter.value();

        beginRemoveRows({}, place, place);

        m_items.remove(place);

        // rebuild place map (terrible, but ok for now)
        m_item_place_map.clear();

        for (int i = 0; i < m_items.size(); i++) {
            m_item_place_map[m_items[i]] = i;
        }

        endRemoveRows();
    }

    void item_in_list_updated() override {
        auto item = dynamic_cast<T*>(sender());

        auto iter = m_item_place_map.find(item);

        Q_ASSERT(iter != m_item_place_map.end());

        auto place = iter.value();

        auto tl = index(place, 0);
        auto br = index(place, columnCount());

        emit dataChanged(tl, br);
    }
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
