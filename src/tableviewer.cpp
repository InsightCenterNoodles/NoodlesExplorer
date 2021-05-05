#include "tableviewer.h"

#include "delegates.h"
#include "tabledata.h"

#include <QDebug>
#include <QHBoxLayout>
#include <QPushButton>
#include <QTabWidget>
#include <QTableView>
#include <QVBoxLayout>

TableViewer::TableViewer(std::shared_ptr<ExTable> t, QObject* parent)
    : QObject(parent), m_attached_table(t) {

    qDebug() << Q_FUNC_INFO;

    m_widget = new QWidget;
    m_widget->setAttribute(Qt::WA_DeleteOnClose);

    auto layout = new QVBoxLayout(m_widget.data());

    auto buttonbar = new QHBoxLayout();

    auto sub_button = new QPushButton("Subscribe");

    buttonbar->addWidget(sub_button);

    auto* view = new QTableView();

    layout->addLayout(buttonbar);
    layout->addWidget(view);


    connect(m_widget.data(), &QWidget::destroyed, [this](QObject*) {
        this->deleteLater();
    });

    connect(t.get(), &ExTable::fetch_new_remote_table_data, [this, view]() {
        view->setModel(m_attached_table->table_data().get());
    });

    connect(sub_button, &QPushButton::clicked, [this]() {
        qDebug() << "SUBSCRIBE";
        m_attached_table->subscribe();
    });

    view->setModel(m_attached_table->table_data().get());

    m_widget->setVisible(true);
}
