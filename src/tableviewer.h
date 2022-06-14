#ifndef TABLEVIEWER_H
#define TABLEVIEWER_H

#include "ui_tableviewer.h"

#include <QPointer>
#include <QWidget>

class ExTable;

class TableViewer : public QObject {
    Q_OBJECT

    QPointer<QWidget>         m_widget;
    std::unique_ptr<Ui::Root> m_ui_root;

    QPointer<ExTable> m_attached_table;

    QVector<int64_t> get_keys_from_selection();

public:
    explicit TableViewer(QPointer<ExTable>, QObject* parent);

private slots:

    void add_rows();
    void delete_selected_rows();


    void add_selection();
    void del_selection();
    void update_selection();

    // void selection_double_clicked(QModelIndex const&);
signals:
};

#endif // TABLEVIEWER_H
