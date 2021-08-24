#include "tableviewer.h"

#include "delegates/extable.h"
#include "newselectiondialog.h"
#include "tabledata.h"

#include "adddatadialog.h"

#include <QDateTime>
#include <QDebug>
#include <QFont>
#include <QHBoxLayout>
#include <QInputDialog>
#include <QPushButton>
#include <QTabWidget>
#include <QTableView>
#include <QVBoxLayout>

TableViewer::TableViewer(QPointer<ExTable> t, QObject* parent)
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

    connect(m_ui_root->addRows,
            &QToolButton::clicked,
            this,
            &TableViewer::add_rows);

    connect(m_ui_root->delRows,
            &QToolButton::clicked,
            this,
            &TableViewer::delete_selected_rows);

    m_ui_root->addSelection->setFont(font);
    m_ui_root->addSelection->setText(QChar(0xf067));

    m_ui_root->delSelection->setFont(font);
    m_ui_root->delSelection->setText(QChar(0xf068));


    connect(m_widget.data(), &QWidget::destroyed, [this](QObject*) {
        this->deleteLater();
    });

    connect(t, &ExTable::fetch_new_remote_table_data, [this]() {
        m_ui_root->dataView->setModel(m_attached_table->table_data().get());
        m_ui_root->selectionListView->setModel(
            m_attached_table->table_data()->selections());
    });

    if (!m_attached_table->is_subscribed()) m_attached_table->subscribe();

    m_ui_root->dataView->setModel(m_attached_table->table_data().get());

    if (m_attached_table and m_attached_table->table_data() and
        m_attached_table->table_data()->selections()) {
        m_ui_root->selectionListView->setModel(
            m_attached_table->table_data()->selections());
    }


    connect(m_ui_root->selectionListView,
            &QListView::doubleClicked,
            this,
            &TableViewer::selection_double_clicked);


    connect(m_ui_root->addSelection,
            &QPushButton::clicked,
            this,
            &TableViewer::add_selection);

    connect(m_ui_root->updateSelection,
            &QPushButton::clicked,
            this,
            &TableViewer::update_selection);

    connect(m_ui_root->delSelection,
            &QPushButton::clicked,
            this,
            &TableViewer::del_selection);

    m_widget->setVisible(true);
}

void TableViewer::add_rows() {
    AddDataDialog add_dialog(m_attached_table, m_widget);

    auto ret = add_dialog.exec();

    if (ret == QDialog::Rejected) return;

    auto data = add_dialog.get_data();

    if (data.empty()) return;

    qDebug() << Q_FUNC_INFO;
    for (auto const& d : data) {
        qDebug() << "COL" << QString::fromStdString(d.dump_string());
    }

    m_attached_table->request_rows_insert(std::move(data));
}

std::vector<int64_t> TableViewer::get_keys_from_selection() {
    auto rows = m_ui_root->dataView->selectionModel()->selectedRows();

    std::vector<int64_t> keys;
    keys.reserve(rows.size());

    for (auto row : rows) {
        auto key = m_attached_table->table_data()->key_for_row(row.row());
        keys.push_back(key);
    }

    return keys;
}

void TableViewer::delete_selected_rows() {
    qDebug() << Q_FUNC_INFO;
    // get the selected keys

    auto keys = get_keys_from_selection();

    auto p = m_attached_table->request_deletion(keys);

    assert(p);

    m_ui_root->delRows->setEnabled(false);

    connect(p, &QObject::destroyed, [this]() {
        m_ui_root->delRows->setEnabled(true);
    });
}

void TableViewer::add_selection() {
    auto name = QInputDialog::getText(m_widget,
                                      "New selection name",
                                      "New selection name:",
                                      QLineEdit::Normal);

    noo::Selection sel;

    sel.rows = get_keys_from_selection();

    if (sel.rows.empty()) {
        auto dialog = NewSelectionDialog(
            "Enter a selection. You can also select items in the table view "
            "and press the Add Selection button instead.",
            m_widget);

        auto dialog_ret = dialog.exec();

        if (dialog_ret != QDialog::Accepted) {
            // we dont want to add an empty one
            return;
        }

        sel = dialog.get_selection();
    }

    qDebug() << "Keys selected" << QVector<int64_t>::fromStdVector(sel.rows);

    auto local_str = name.toStdString();

    if (name.isEmpty()) { name = QDateTime::currentDateTime().toString(); }

    m_attached_table->request_selection_update(local_str, std::move(sel));
}

void TableViewer::del_selection() {
    auto rows = m_ui_root->selectionListView->selectionModel()->selectedRows();

    if (rows.empty()) return;

    auto row = rows.value(0);

    auto name =
        m_attached_table->table_data()->selections()->slot_at(row.row())->name;

    m_attached_table->request_selection_update(name, {});
}

void TableViewer::update_selection() {
    auto rows = m_ui_root->selectionListView->selectionModel()->selectedRows();

    if (rows.empty()) return;

    auto row = rows.value(0);

    auto name =
        m_attached_table->table_data()->selections()->slot_at(row.row())->name;

    if (name.empty()) return;

    noo::Selection sel;

    sel.rows = get_keys_from_selection();

    if (sel.rows.empty()) {
        auto dialog = NewSelectionDialog(
            "Enter a selection. You can also select items in the table view "
            "and press the Add Selection button instead.",
            m_widget);

        auto dialog_ret = dialog.exec();

        if (dialog_ret != QDialog::Accepted) {
            // we dont want to add an empty one
            return;
        }

        sel = dialog.get_selection();
    }

    m_attached_table->request_selection_update(name, std::move(sel));
}

void TableViewer::selection_double_clicked(QModelIndex const& index) {
    qDebug() << Q_FUNC_INFO << index;
    auto row = index.row();

    auto slot = m_attached_table->table_data()->selections()->slot_at(row);

    qDebug() << slot;

    if (!slot) return;

    auto const& selection = slot->selection;

    QItemSelection new_selection;

    auto* model = m_attached_table->table_data().get();

    auto cols = std::max(model->columnCount() - 1, 0);

    qDebug() << QVector<int64_t>(selection.rows.begin(), selection.rows.end());


    for (auto key : selection.rows) {
        auto row = model->row_for_key(key);

        qDebug() << key << row;

        if (row < 0) continue;

        auto index_s = model->index(row, 0);
        auto index_e = model->index(row, cols);

        qDebug() << index_s << index_e;

        new_selection.select(index_s, index_e);
    }

    for (auto const& range : selection.row_ranges) {
        // this is horrible, but for the moment it will work.

        for (auto key = range.first; key < range.second; key++) {
            auto row = model->row_for_key(key);

            qDebug() << key << row;

            if (row < 0) continue;

            auto index = model->index(row, cols);

            new_selection.select(index, index);
        }
    }

    qDebug() << new_selection;


    auto* selection_model = m_ui_root->dataView->selectionModel();

    selection_model->select(new_selection, QItemSelectionModel::Select);
}
