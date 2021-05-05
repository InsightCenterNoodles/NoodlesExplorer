#ifndef TABLEVIEWER_H
#define TABLEVIEWER_H

#include <QPointer>
#include <QWidget>

class ExTable;

class TableViewer : public QObject {
    Q_OBJECT

    std::shared_ptr<ExTable> m_attached_table;
    QPointer<QWidget>        m_widget;

public:
    explicit TableViewer(std::shared_ptr<ExTable>, QObject* parent);

signals:
};

#endif // TABLEVIEWER_H
