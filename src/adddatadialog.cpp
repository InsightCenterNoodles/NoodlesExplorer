#include "adddatadialog.h"
#include "ui_adddatadialog.h"

#include "delegates/extable.h"
#include "tabledata.h"

#include <noo_any.h>

void AddDataDialog::setup() {
    // m_ui_root->dataView->setModel(m_table->table_data().get());

    auto names = m_table->table_data()->column_names();

    ui->tableWidget->setColumnCount(names.size());

    ui->tableWidget->setHorizontalHeaderLabels(names);

    ui->tableWidget->setSpan(
        new_row_item->row(), new_row_item->column(), 1, names.size());
}

void AddDataDialog::double_clicked(QTableWidgetItem* item) {
    if (item != new_row_item) return;

    ui->tableWidget->setSpan(new_row_item->row(), new_row_item->column(), 1, 1);

    ui->tableWidget->insertRow(new_row_item->row());

    auto names = m_table->table_data()->column_names();

    ui->tableWidget->setSpan(
        new_row_item->row(), new_row_item->column(), 1, names.size());
}

AddDataDialog::AddDataDialog(QPointer<ExTable> tbl, QWidget* parent)
    : QDialog(parent), ui(new Ui::AddDataDialog), m_table(tbl) {
    ui->setupUi(this);

    connect(m_table,
            &ExTable::fetch_new_remote_table_data,
            this,
            &AddDataDialog::setup);

    ui->tableWidget->setColumnCount(1);
    ui->tableWidget->setRowCount(1);

    new_row_item = new QTableWidgetItem("<double click to add a row>");

    ui->tableWidget->setItem(0, 0, new_row_item);

    setup();

    connect(ui->tableWidget,
            &QTableWidget::itemDoubleClicked,
            this,
            &AddDataDialog::double_clicked);

    // connect(ui->buttonBox, &QDialogButtonBox::accepted, this,
    // &QDialog::accept); connect(ui->buttonBox, &QDialogButtonBox::rejected,
    // this, &QDialog::reject);
}

AddDataDialog::~AddDataDialog() {
    delete ui;
}

noo::AnyVarList AddDataDialog::get_data() const {
    // should be returned as a list of columns

    noo::AnyVarList l;

    for (int ci = 0; ci < ui->tableWidget->columnCount(); ci++) {
        noo::AnyVarList this_col;

        for (int ri = 0; ri < ui->tableWidget->rowCount() - 1; ri++) {
            auto* item = ui->tableWidget->item(ri, ci);

            auto d = item->data(Qt::EditRole);

            bool ok = false;

            auto value = d.toDouble(&ok);

            if (!ok) {
                this_col.emplace_back(d.toString().toStdString());
            } else {
                this_col.emplace_back(value);
            }
        }

        l.emplace_back(std::move(this_col));
    }

    return l;
}
