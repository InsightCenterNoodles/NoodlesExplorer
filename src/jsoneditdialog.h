#ifndef JSONEDITDIALOG_H
#define JSONEDITDIALOG_H

#include <QCborValue>
#include <QDialog>

namespace Ui {
class JSONEditDialog;
}

class JSONEditDialog : public QDialog {
    Q_OBJECT
    Ui::JSONEditDialog* ui;


public:
    static QCborValue parse_any(QString const& s, bool* ok = nullptr);

    explicit JSONEditDialog(QWidget* parent = nullptr);
    ~JSONEditDialog();

    void    set_text(QString);
    QString text() const;

    QCborValue get_as_any(bool* ok) const;

private slots:
    bool evaluate_text() const;
};

#endif // JSONEDITDIALOG_H
