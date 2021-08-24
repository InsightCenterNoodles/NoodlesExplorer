#include "newselectiondialog.h"
#include "ui_newselectiondialog.h"

#include "noo_client_interface.h"

#include <QRegExpValidator>

NewSelectionDialog::NewSelectionDialog(QString info, QWidget* parent)
    : QDialog(parent), ui(new Ui::NewSelectionDialog) {
    ui->setupUi(this);

    ui->label->setText(info);

    m_validator = new QRegExpValidator(QRegExp("[\\d,-]"), ui->keysTextEdit);
}

NewSelectionDialog::~NewSelectionDialog() {
    delete ui;
}

noo::Selection NewSelectionDialog::get_selection() const {
    noo::Selection sel;

    auto t = ui->keysTextEdit->toPlainText();

    if (t.isEmpty()) return sel;

    int  pos   = 0;
    auto valid = m_validator->validate(t, pos);

    if (valid != QValidator::Acceptable) { return sel; }

    auto refs = t.splitRef(',');

    for (auto ref : refs) {
        if (ref.isEmpty()) continue;

        if (ref.contains('-')) {
            auto parts = ref.split('-');

            if (parts.size() != 2) continue;

            auto start = parts.value(0);
            auto end   = parts.value(1);

            bool ok     = false;
            auto index1 = start.toLong(&ok);
            if (!ok) continue;

            auto index2 = end.toLong(&ok);
            if (!ok) continue;

            if (index2 < index1) { std::swap(index1, index2); }

            sel.row_ranges.emplace_back(index1, index2);

        } else {
            bool ok    = false;
            auto index = ref.toLong(&ok);

            if (!ok) continue;

            sel.rows.push_back(index);
        }
    }

    return sel;
}
