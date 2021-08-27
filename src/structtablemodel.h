#ifndef STRUCTTABLEMODEL_H
#define STRUCTTABLEMODEL_H

#include <QAbstractTableModel>
#include <QDebug>

template <class H, class T>
struct MetaMember {
    T H::*      access;
    char const* name;
    bool        editable;

    constexpr MetaMember(T H::*_access, char const* _name, bool _editable)
        : access(_access), name(_name), editable(_editable) { }
};

template <class Tuple, class Function>
auto tuple_for_each(Tuple&& t, Function&& f) {
    std::apply([&](auto&... x) { (..., f(x)); }, t);
}

template <class Tuple, int I, class Function>
struct _get_by_idx {
    static_assert(I >= 1);
    static void check(Tuple&& t, int i, Function&& f) {
        if (I == i) { return f(std::get<I>(t)); }

        _get_by_idx<Tuple, I - 1, Function>::check(t, i, std::move(f));
    }
};

template <class Tuple, class Function>
struct _get_by_idx<Tuple, 0, Function> {
    static void check(Tuple&& t, int i, Function&& f) {
        if (0 == i) { return f(std::get<0>(t)); }

        throw std::runtime_error("Bad index");
    }
};


template <class Tuple, class Function>
void tuple_get(Tuple&& t, int index, Function&& f) {
    using LT = std::remove_cvref_t<Tuple>;

    constexpr static auto S = std::tuple_size_v<LT>;

    _get_by_idx<Tuple, S - 1, Function>::check(t, index, std::move(f));
}


template <class Record>
QStringList get_header() {
    QStringList ret;
    tuple_for_each(Record::meta, [&ret](auto const& m) { ret << m.name; });
    return ret;
}

template <class Record>
QHash<int, QByteArray> const& get_name_map() {
    static QHash<int, QByteArray> ret = []() {
        QHash<int, QByteArray> build;

        int i = 0;

        tuple_for_each(Record::meta, [&build, &i](auto const& thing) {
            build[Qt::UserRole + i] = thing.name;
            i++;
        });
        return build;
    }();
    return ret;
}

template <class Record>
QVariant record_runtime_get(Record const& r, int i) {
    QVariant ret;
    tuple_get(
        Record::meta, i, [&r, &ret](auto const& a) { ret = r.*a.access; });
    return ret;
}

template <class Record>
bool record_runtime_set(Record& r, int i, QVariant const& v) {
    bool ret = false;

    tuple_get(Record::meta, i, [&v, &ret, &r](auto& _value) {
        if (_value.editable) {
            ret = true;

            using LT = std::remove_cvref_t<decltype(r.*_value.access)>;
            (r.*_value.access) = v.value<LT>();
        }
    });

    return ret;
}

template <class Record>
class StructTableModel : public QAbstractTableModel {
    QList<Record> m_records;

    QStringList const m_header;

public:
    explicit StructTableModel(QObject* parent = nullptr)
        : QAbstractTableModel(parent), m_header(get_header<Record>()) { }

    // Header:
    QVariant headerData(int             section,
                        Qt::Orientation orientation,
                        int             role = Qt::DisplayRole) const override {
        if (orientation != Qt::Orientation::Horizontal) return {};
        if (role != Qt::DisplayRole) return {};

        return m_header.value(section);
    }

    int rowCount(QModelIndex const& parent = QModelIndex()) const override {
        if (parent.isValid()) return 0;
        return m_records.size();
    }

    int columnCount(QModelIndex const& parent = QModelIndex()) const override {
        if (parent.isValid()) return 0;
        return m_header.size();
    }

    QVariant data(QModelIndex const& index,
                  int                role = Qt::DisplayRole) const override {

        if (!index.isValid()) return {};
        if (index.row() >= m_records.size()) return {};

        auto const& item = m_records[index.row()];

        if (role == Qt::DisplayRole or role == Qt::EditRole) {
            return record_runtime_get(item, index.column());
        }

        if (role >= Qt::UserRole) {
            auto local_role = role - Qt::UserRole;

            assert(local_role >= 0);

            if (local_role >= m_header.size()) return {};

            return record_runtime_get(item, local_role);
        }

        return {};
    }


    bool setData(QModelIndex const& index,
                 QVariant const&    value,
                 int                role = Qt::EditRole) override {

        qDebug() << Q_FUNC_INFO << index << value << role;

        if (data(index, role) == value) return false;

        auto& item = m_records[index.row()];

        int location = -1;

        if (role >= Qt::UserRole) {
            location = role - Qt::UserRole;
        } else {
            location = index.column();
        }

        if (location >= m_header.size()) return false;

        bool ok = record_runtime_set(item, location, value);

        if (!ok) return false;

        emit dataChanged(index, index, QList<int>() << role);
        return true;
    }

    Qt::ItemFlags flags(QModelIndex const& index) const override {
        if (!index.isValid()) return Qt::NoItemFlags;

        bool can_edit = false;

        tuple_get(Record::meta, index.column(), [&can_edit](auto const& v) {
            can_edit = v.editable;
        });

        if (!can_edit) return Qt::NoItemFlags;

        return Qt::ItemIsEditable;
    }

    QHash<int, QByteArray> roleNames() const override {
        static const auto roles = get_name_map<Record>();

        return roles;
    }

    void reset(QList<Record> new_records = {}) {
        beginResetModel();
        m_records = new_records;
        endResetModel();
    }

    Record* get_at(int i) {
        if (i < 0) return nullptr;
        if (i >= m_records.size()) return nullptr;
        return &m_records[i];
    }

    auto const& vector() const { return m_records; }
};

#endif // STRUCTTABLEMODEL_H
