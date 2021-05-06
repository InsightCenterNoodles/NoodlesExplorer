#include "tableviewer.h"

#include "chartviewer.h"
#include "delegates.h"
#include "tabledata.h"

#include <QDebug>
#include <QFont>
#include <QHBoxLayout>
#include <QPushButton>
#include <QTabWidget>
#include <QTableView>
#include <QVBoxLayout>

TableViewer::TableViewer(std::shared_ptr<ExTable> t, QObject* parent)
    : QObject(parent), m_attached_table(t) {
    m_widget = new QWidget;
    m_widget->setAttribute(Qt::WA_DeleteOnClose);

    m_ui_root = std::make_unique<Ui::Root>();
    m_ui_root->setupUi(m_widget);

    m_widget->setWindowTitle(t->get_name());

    auto font = QFont("Font Awesome 5 Free", 14);
    font.setStyleName("Solid");

    m_ui_root->addRows->setFont(font);
    m_ui_root->addRows->setText(QChar(0xf067));

    m_ui_root->delRows->setFont(font);
    m_ui_root->delRows->setText(QChar(0xf068));

    m_ui_root->addSelection->setFont(font);
    m_ui_root->addSelection->setText(QChar(0xf067));

    m_ui_root->delSelection->setFont(font);
    m_ui_root->delSelection->setText(QChar(0xf068));

    m_ui_root->newChart->setFont(font);
    m_ui_root->newChart->setText(QChar(0xf080));

    connect(m_ui_root->newChart, &QToolButton::clicked, [this]() {
        auto* this_p = this->parent();
        new ChartViewer(m_attached_table, this_p);
    });

    connect(m_widget.data(), &QWidget::destroyed, [this](QObject*) {
        this->deleteLater();
    });

    connect(t.get(), &ExTable::fetch_new_remote_table_data, [this]() {
        m_ui_root->dataView->setModel(m_attached_table->table_data().get());
    });

    m_attached_table->subscribe();

    m_ui_root->dataView->setModel(m_attached_table->table_data().get());

    m_widget->setVisible(true);
}
