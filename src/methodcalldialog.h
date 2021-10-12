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

class JSONEditDelegate : public QStyledItemDelegate {
    Q_OBJECT

public:
    JSONEditDelegate(QObject* parent = nullptr);

    QWidget* createEditor(QWidget*                    parent,
                          QStyleOptionViewItem const& option,
                          QModelIndex const&          index) const override;

    void setEditorData(QWidget*           editor,
                       QModelIndex const& index) const override;
    void setModelData(QWidget*            editor,
                      QAbstractItemModel* model,
                      QModelIndex const&  index) const override;

    void updateEditorGeometry(QWidget*                    editor,
                              QStyleOptionViewItem const& option,
                              QModelIndex const&          index) const override;

    // QAbstractItemDelegate interface
public:
    QSize sizeHint(QStyleOptionViewItem const& option,
                   QModelIndex const&          index) const override;

private slots:
    void commit();
    void cancel();
};


class JSONEditor : public QWidget {
    Q_OBJECT

    QPlainTextEdit* m_text_edit;
    QLabel*         m_feedback;

public:
    JSONEditor(QWidget* parent = nullptr);

    void    set_text(QString);
    QString text() const;

private slots:
    bool evaluate_text() const;

signals:
    void editing_finished();
    void editing_cancelled();
};

#endif // METHODCALLDIALOG_H
