#include "adddatadialog.h"
#include "ui_adddatadialog.h"

#include "delegates/extable.h"
#include "tabledata.h"

static QStringList make_column_names(QAbstractTableModel* model) {
    QStringList ret;

    for (auto i = 0; i < model->columnCount(); i++) {
        ret << model->headerData(i, Qt::Orientation::Horizontal).toString();
    }

    return ret;
}

void AddDataDialog::setup() {
    // m_ui_root->dataView->setModel(m_table->table_data().get());

    auto names = make_column_names(m_table->table_data());

    ui->tableWidget->setColumnCount(names.size());

    ui->tableWidget->setHorizontalHeaderLabels(names);

    ui->tableWidget->setSpan(
        new_row_item->row(), new_row_item->column(), 1, names.size());
}

void AddDataDialog::double_clicked(QTableWidgetItem* item) {
    if (item != new_row_item) return;

    ui->tableWidget->setSpan(new_row_item->row(), new_row_item->column(), 1, 1);

    ui->tableWidget->insertRow(new_row_item->row());

    auto names = make_column_names(m_table->table_data());

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

QCborArray AddDataDialog::get_data() const {
    // should be returned as a list of columns

    QCborArray l;

    for (int ci = 0; ci < ui->tableWidget->columnCount(); ci++) {
        QCborArray this_col;

        for (int ri = 0; ri < ui->tableWidget->rowCount() - 1; ri++) {
            auto* item = ui->tableWidget->item(ri, ci);

            auto d = item->data(Qt::EditRole);

            bool ok = false;

            auto value = d.toDouble(&ok);

            if (!ok) {
                this_col << d.toString();
            } else {
                this_col << value;
            }
        }

        l << std::move(this_col);
    }

    return l;
}
