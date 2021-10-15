#ifndef JSONEDITDIALOG_H
#define JSONEDITDIALOG_H

#include <QDialog>

namespace noo {
struct AnyVar;
}

namespace Ui {
class JSONEditDialog;
}

class JSONEditDialog : public QDialog {
    Q_OBJECT
    Ui::JSONEditDialog* ui;


public:
    static noo::AnyVar parse_any(QString const& s, bool* ok = nullptr);

    explicit JSONEditDialog(QWidget* parent = nullptr);
    ~JSONEditDialog();

    void    set_text(QString);
    QString text() const;

    noo::AnyVar get_as_any(bool* ok) const;

private slots:
    bool evaluate_text() const;
};

#endif // JSONEDITDIALOG_H
