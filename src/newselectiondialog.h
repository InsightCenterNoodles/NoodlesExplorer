#ifndef NEWSELECTIONDIALOG_H
#define NEWSELECTIONDIALOG_H

#include <QDialog>

namespace noo {
class Selection;
}

namespace Ui {
class NewSelectionDialog;
}

class QRegExpValidator;

class NewSelectionDialog : public QDialog {
    Q_OBJECT

    QRegExpValidator* m_validator;

public:
    explicit NewSelectionDialog(QString info, QWidget* parent = nullptr);
    ~NewSelectionDialog();

    noo::Selection get_selection() const;

private:
    Ui::NewSelectionDialog* ui;
};

#endif // NEWSELECTIONDIALOG_H
