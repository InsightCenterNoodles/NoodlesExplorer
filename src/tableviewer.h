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

    std::shared_ptr<ExTable> m_attached_table;

public:
    explicit TableViewer(std::shared_ptr<ExTable>, QObject* parent);

private slots:

    void add_rows();
    void delete_selection();

signals:
};

#endif // TABLEVIEWER_H
