#ifndef ATTACHEDMETHODLISTMODEL_H
#define ATTACHEDMETHODLISTMODEL_H

#include "delegates.h"

#include <QAbstractTableModel>

class AttachedMethodListModel : public QAbstractTableModel {
    Q_OBJECT

    std::vector<std::shared_ptr<ExMethod>> m_list;

public:
    enum Roles {
        name = Qt::UserRole,
        slot,
        gen,
    };

    explicit AttachedMethodListModel(QObject* parent = nullptr);

    void set(std::vector<nooc::MethodDelegatePtr> const&);
    void clear();

    std::shared_ptr<ExMethod> get_method_row(int) const;

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
};

#endif // ATTACHEDMETHODLISTMODEL_H
