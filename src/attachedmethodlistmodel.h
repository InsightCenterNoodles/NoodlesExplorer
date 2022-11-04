#ifndef ATTACHEDMETHODLISTMODEL_H
#define ATTACHEDMETHODLISTMODEL_H

#include "delegates/exmethod.h"

#include <QAbstractTableModel>

#include <deque>

class AttachedMethodListModel : public QAbstractTableModel {
    Q_OBJECT

    std::deque<QPointer<ExMethod>> m_list;

public:
    enum Roles {
        name = Qt::UserRole,
        slot,
        gen,
    };

    explicit AttachedMethodListModel(QObject* parent = nullptr);

    void set(QVector<nooc::MethodDelegate*> const&);
    void clear();

    ExMethod* get_method_row(int) const;

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

    bool has(QString name) const;

public slots:
    void ask_call(int index);


signals:
    void wishes_to_call(ExMethod*);
};

#endif // ATTACHEDMETHODLISTMODEL_H
