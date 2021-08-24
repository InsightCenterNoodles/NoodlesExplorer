#ifndef ADDDATADIALOG_H
#define ADDDATADIALOG_H

#include <QDialog>
#include <QPointer>

class ExTable;
class QTableWidgetItem;

namespace Ui {
class AddDataDialog;
}

namespace noo {
class AnyVar;

using AnyVarList = std::vector<AnyVar>;
} // namespace noo

class AddDataDialog : public QDialog {
    Q_OBJECT

    Ui::AddDataDialog* ui;

    QTableWidgetItem* new_row_item;

    QPointer<ExTable> m_table;

public:
    explicit AddDataDialog(QPointer<ExTable>, QWidget* parent = nullptr);
    ~AddDataDialog();

    noo::AnyVarList get_data() const;

private slots:
    void setup();
    void double_clicked(QTableWidgetItem*);
};

#endif // ADDDATADIALOG_H
