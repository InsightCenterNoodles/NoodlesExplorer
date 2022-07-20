#include "attachedmethodlistmodel.h"

AttachedMethodListModel::AttachedMethodListModel(QObject* parent)
    : QAbstractTableModel(parent) { }

void AttachedMethodListModel::set(QVector<nooc::MethodDelegate*> const& l) {
    beginResetModel();

    m_list.clear();

    for (auto const& s : l) {
        m_list.emplace_back(dynamic_cast<ExMethod*>(s));
    }

    endResetModel();
}

void AttachedMethodListModel::clear() {
    beginResetModel();

    m_list.clear();

    endResetModel();
}

ExMethod* AttachedMethodListModel::get_method_row(int i) const {
    if (i < 0) return {};
    if ((size_t)i >= m_list.size()) return {};

    return m_list[i];
}

QVariant AttachedMethodListModel::headerData(int             section,
                                             Qt::Orientation orientation,
                                             int             role) const {

    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        switch (section) {
        case 0: return "Name";
        case 1: return "Slot";
        case 2: return "Generation";
        }
    }

    return {};
}

int AttachedMethodListModel::rowCount(const QModelIndex& parent) const {
    if (parent.isValid()) return 0;

    return m_list.size();
}

int AttachedMethodListModel::columnCount(const QModelIndex& parent) const {
    if (parent.isValid()) return 0;

    return 3;
}

QVariant AttachedMethodListModel::data(const QModelIndex& index,
                                       int                role) const {
    if (!index.isValid()) return {};

    auto const& item = m_list.at(index.row());

    Q_ASSERT(item);

    if (role == Qt::DisplayRole or role == Qt::EditRole) {
        return item->get_column(1);
    }

    switch (role) {
    case name: return item->name();
    case slot: return item->id().id_slot;
    case gen: return item->id().id_gen;
    }

    return QVariant();
}

QHash<int, QByteArray> AttachedMethodListModel::roleNames() const {
    static const QHash<int, QByteArray> r {
        { Roles::name, "name" },
        { Roles::slot, "slot" },
        { Roles::gen, "gen" },
    };

    return r;
}

void AttachedMethodListModel::ask_call(int index) {
    auto p = get_method_row(index);
    if (p) {
        // qDebug() << Q_FUNC_INFO << index << rowCount() << p;
        emit wishes_to_call(p);
    }
}
