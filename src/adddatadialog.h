#ifndef ADDDATADIALOG_H
#define ADDDATADIALOG_H

#include <QDialog>

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

    std::shared_ptr<ExTable> m_table;

public:
    explicit AddDataDialog(std::shared_ptr<ExTable>, QWidget* parent = nullptr);
    ~AddDataDialog();

    noo::AnyVarList get_data() const;

private slots:
    void setup();
    void double_clicked(QTableWidgetItem*);
};

#endif // ADDDATADIALOG_H
