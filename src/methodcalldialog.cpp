#include "methodcalldialog.h"
#include "ui_methodcalldialog.h"

#include "argumenttablemodel.h"
#include "variant_tools.h"

#include "delegates/exdoc.h"
#include "delegates/exmethod.h"
#include "delegates/exobject.h"
#include "jsoneditdialog.h"

#include <noo_common.h>

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QPushButton>


MethodCallDialog::MethodCallDialog(T                  mc,
                                   QPointer<ExMethod> method,
                                   QWidget*           parent)
    : QDialog(parent),
      m_ui(new Ui::MethodCallDialog),
      m_context(mc),
      m_method(method) {
    m_ui->setupUi(this);

    qDebug() << Q_FUNC_INFO << this;

    this->setAttribute(Qt::WA_DeleteOnClose);

    m_model = new ArgumentTableModel(this);


    if (m_method) {
        QStringList arg_names = m_method->argument_names();
        QStringList arg_deets = m_method->argument_details();

        QList<Argument> args;

        for (int i = 0; i < std::min(arg_names.size(), arg_deets.size()); i++) {
            args << Argument { arg_names[i], arg_deets[i], "null" };
        }

        m_model->reset(args);
    }

    m_ui->tableView->setModel(m_model);

    connect(m_ui->invokeButton,
            &QPushButton::clicked,
            this,
            &MethodCallDialog::execute_method);

    connect(
        m_ui->cancelButton, &QPushButton::clicked, [this]() { this->close(); });

    connect(m_ui->tableView,
            &QTableView::doubleClicked,
            this,
            &MethodCallDialog::double_clicked);
}

MethodCallDialog::~MethodCallDialog() {
    delete m_ui;
    qDebug() << Q_FUNC_INFO;
}

void MethodCallDialog::double_clicked(QModelIndex const& index) {

    auto json_index = m_model->index(index.row(), 2);

    auto text = m_model->data(json_index, Qt::DisplayRole).toString();

    JSONEditDialog dialog;

    dialog.set_text(text);

    auto ret = dialog.exec();

    if (ret == QDialog::DialogCode::Accepted) {
        m_model->setData(json_index, dialog.text());
    }
}

void MethodCallDialog::execute_method() {
    qDebug() << Q_FUNC_INFO;
    if (!m_method) return;

    noo::AnyVarList avlist;

    for (auto const& d : m_model->vector()) {
        avlist.push_back(JSONEditDialog::parse_any(d.current_value));
    }


    auto const* att_m_list = VMATCH(
        m_context,
        VCASE(QPointer<ExDoc> ptr) {
            return ptr ? &ptr->attached_methods() : nullptr;
        },
        VCASE(QPointer<ExObject> ptr) {
            return ptr ? &ptr->attached_methods() : nullptr;
        });

    if (!att_m_list) return;

    auto* r = att_m_list->new_call_by_delegate<NormalizeStringReply>(m_method);

    if (!r) return;

    connect(
        r, &NormalizeStringReply::recv, this, &MethodCallDialog::method_reply);

    connect(r,
            &NormalizeStringReply::recv_fail,
            this,
            &MethodCallDialog::method_failed);


    r->call_direct(std::move(avlist));
}

void MethodCallDialog::method_reply(QString str) {
    qDebug() << Q_FUNC_INFO << str;
    m_ui->resultText->setText("Success: " + str);
}

void MethodCallDialog::method_failed(QString str) {
    qDebug() << Q_FUNC_INFO << str;
    m_ui->resultText->setText("ERROR: " + str);
}

// =============================================================================

void NormalizeStringReply::interpret() {
    auto str = m_var.dump_string();

    emit recv(noo::to_qstring(str));
}

// =============================================================================
#if 0
JSONEditDelegate::JSONEditDelegate(QObject* parent)
    : QStyledItemDelegate(parent) { }

QWidget* JSONEditDelegate::createEditor(QWidget* parent,
                                        QStyleOptionViewItem const& /*option*/,
                                        QModelIndex const& /*index*/) const {

    auto p = new JSONEditor(parent);

    connect(p, &JSONEditor::editing_finished, this, &JSONEditDelegate::commit);
    connect(p, &JSONEditor::editing_cancelled, this, &JSONEditDelegate::cancel);

    return p;
}

void JSONEditDelegate::setEditorData(QWidget*           editor,
                                     QModelIndex const& index) const {
    auto string = index.model()->data(index, Qt::EditRole).toString();
    static_cast<JSONEditor*>(editor)->set_text(string);
}
void JSONEditDelegate::setModelData(QWidget*            editor,
                                    QAbstractItemModel* model,
                                    QModelIndex const&  index) const {
    auto string = static_cast<JSONEditor*>(editor)->text();

    model->setData(index, string, Qt::EditRole);
}

void JSONEditDelegate::updateEditorGeometry(
    QWidget*                    editor,
    QStyleOptionViewItem const& option,
    QModelIndex const& /*index*/) const {
    auto orig_rect = option.rect;
    orig_rect.setSize(QSize(orig_rect.width(), 200));
    editor->setGeometry(orig_rect);
}


QSize JSONEditDelegate::sizeHint(QStyleOptionViewItem const& option,
                                 QModelIndex const&          index) const {
    auto orig_rect = option.rect;
    return QSize(orig_rect.width(), 200);
}

void JSONEditDelegate::commit() {
    JSONEditor* editor = dynamic_cast<JSONEditor*>(sender());
    emit        commitData(editor);
    emit        closeEditor(editor);
}

void JSONEditDelegate::cancel() {
    JSONEditor* editor = dynamic_cast<JSONEditor*>(sender());
    emit        closeEditor(editor);
}

// =============================================================================

JSONEditor::JSONEditor(QWidget* parent) : QWidget(parent) {
    auto* vl = new QVBoxLayout();
    auto* hl = new QHBoxLayout();

    vl->setContentsMargins(0, 0, 0, 0);
    hl->setContentsMargins(0, 0, 0, 0);

    m_text_edit = new QPlainTextEdit();
    m_feedback  = new QLabel("Feedback");

    vl->addWidget(m_text_edit, 1);
    vl->addWidget(m_feedback);
    vl->addLayout(hl, 0);

    auto* nope = new QPushButton("Cancel");
    auto* okie = new QPushButton("Save");
    hl->addWidget(nope);
    hl->addWidget(okie);


    setLayout(vl);
    // m_text_edit->setStyleSheet("{color:red}");

    connect(
        nope, &QPushButton::clicked, [this]() { emit editing_cancelled(); });

    connect(okie, &QPushButton::clicked, [this]() {
        bool ok = evaluate_text();
        if (ok) emit editing_finished();
    });

    connect(m_text_edit,
            &QPlainTextEdit::textChanged,
            this,
            &JSONEditor::evaluate_text);
}

void JSONEditor::set_text(QString str) {
    m_text_edit->setPlainText(str);
    evaluate_text();
}

QString JSONEditor::text() const {
    return m_text_edit->toPlainText();
}

bool JSONEditor::evaluate_text() const {
    auto parse_result = from_raw_string(text());

    auto str = VMATCH(
        parse_result,
        VCASE(noo::AnyVar&) { return QString(); },
        VCASE(QString a) { return a; });

    bool ok = str.isEmpty();

    QColor c = QWidget::palette().color(QWidget::backgroundRole());

    auto sheet = QString("color: %1; background-color: %2")
                     .arg(ok ? "green" : "red")
                     .arg(c.name());

    m_feedback->setText(ok ? "OK" : str);
    m_feedback->setStyleSheet(sheet);

    return ok;
}
#endif
