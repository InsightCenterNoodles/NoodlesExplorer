#ifndef METHODCALLDIALOG_H
#define METHODCALLDIALOG_H

#include <noo_client_interface.h>

#include <QDialog>
#include <QPlainTextEdit>
#include <QPointer>
#include <QStyledItemDelegate>

class QLabel;

namespace Ui {
class MethodCallDialog;
}

class ExDoc;
class ExObject;
class ExMethod;
class ArgumentTableModel;

class MethodCallDialog : public QDialog {
    Q_OBJECT

    using T = std::variant<QPointer<ExDoc>, QPointer<ExObject>>;

    Ui::MethodCallDialog* m_ui;
    ArgumentTableModel*   m_model;

    T                  m_context;
    QPointer<ExMethod> m_method;

public:
    explicit MethodCallDialog(T                  mc,
                              QPointer<ExMethod> method,
                              QWidget*           parent = nullptr);
    ~MethodCallDialog();

private slots:
    void double_clicked(QModelIndex const&);
    void execute_method();

    void method_reply(QString);
    void method_failed(QString);
};

class NormalizeStringReply : public nooc::PendingMethodReply {
    Q_OBJECT

public:
    using PendingMethodReply::PendingMethodReply;

    void interpret() override;
signals:
    void recv(QString);
};

#endif // METHODCALLDIALOG_H
