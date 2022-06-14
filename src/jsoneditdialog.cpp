#include "jsoneditdialog.h"
#include "ui_jsoneditdialog.h"

#include "variant_tools.h"

#include <noo_common.h>

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>

// Forward

static std::variant<QCborValue, QString> from_raw_string(QString const& s) {

    auto ls = QString("[ %1 ]").arg(s);

    // to save us effort, we are going to hijack the document reader

    QJsonParseError error;

    auto doc = QJsonDocument::fromJson(ls.toLocal8Bit(), &error);

    if (error.error == QJsonParseError::NoError) {
        return QCborValue::fromJsonValue(doc.array().at(0));
    }

    int offset = error.offset - 2; // as we added the bracket and space...

    auto ss = s.mid(std::max(offset - 5, 0), 10);

    return QString("%1: %2").arg(ss).arg(error.errorString());
}

// =============================================================================

QCborValue JSONEditDialog::parse_any(QString const& s, bool* ok) {
    auto parse_result = from_raw_string(s);

    QCborValue a = VMATCH(
        parse_result,
        VCASE(QCborValue & av) {
            if (ok) *ok = true;
            return std::move(av);
        },
        VCASE(QString) {
            if (ok) *ok = false;
            return QCborValue();
        });

    return a;
}

JSONEditDialog::JSONEditDialog(QWidget* parent)
    : QDialog(parent), ui(new Ui::JSONEditDialog) {
    ui->setupUi(this);

    connect(ui->JSONBox,
            &QPlainTextEdit::textChanged,
            this,
            &JSONEditDialog::evaluate_text);
}

JSONEditDialog::~JSONEditDialog() {
    delete ui;
}

void JSONEditDialog::set_text(QString str) {
    ui->JSONBox->setPlainText(str);
    evaluate_text();
}

QString JSONEditDialog::text() const {
    return ui->JSONBox->toPlainText();
}

QCborValue JSONEditDialog::get_as_any(bool* ok) const {
    return parse_any(text(), ok);
}

bool JSONEditDialog::evaluate_text() const {
    auto parse_result = from_raw_string(text());

    auto str = VMATCH(
        parse_result,
        VCASE(QCborValue&) { return QString(); },
        VCASE(QString a) { return a; });

    bool ok = str.isEmpty();

    QColor c = QWidget::palette().color(QWidget::backgroundRole());

    auto sheet = QString("color: %1; background-color: %2")
                     .arg(ok ? "green" : "red")
                     .arg(c.name());

    ui->feedbackLabel->setText(ok ? "OK" : str);
    ui->feedbackLabel->setStyleSheet(sheet);

    return ok;
}
