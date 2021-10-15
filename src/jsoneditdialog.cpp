#include "jsoneditdialog.h"
#include "ui_jsoneditdialog.h"

#include "variant_tools.h"

#include <noo_any.h>
#include <noo_common.h>

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>

// Forward
static noo::AnyVar from_json(QJsonValue const& s);

static noo::AnyVar from_json(QJsonArray const& arr) {
    // TODO: we can do better
    bool all_reals = std::all_of(
        arr.begin(), arr.end(), [](auto const& v) { return v.isDouble(); });

    if (all_reals) {
        std::vector<double> reals;
        reals.reserve(arr.size());

        for (auto const& v : arr) {
            reals.push_back(v.toDouble());
        }

        return noo::AnyVar(std::move(reals));
    }

    noo::AnyVarList ret;

    ret.reserve(arr.size());

    for (auto const& v : arr) {
        ret.push_back(from_json(v));
    }

    return std::move(ret);
}

static noo::AnyVar from_json(QJsonObject const& obj) {
    noo::AnyVarMap ret;

    for (auto iter = obj.begin(); iter != obj.end(); ++iter) {
        ret.try_emplace(iter.key().toStdString(), from_json(iter.value()));
    }

    return ret;
}

static noo::AnyVar from_json(QJsonValue const& s) {
    switch (s.type()) {
    case QJsonValue::Null: return std::monostate();
    case QJsonValue::Bool: return (int64_t)s.toBool();
    case QJsonValue::Double: return s.toDouble();
    case QJsonValue::String: return s.toString().toStdString();
    case QJsonValue::Array: return from_json(s.toArray());
    case QJsonValue::Object: return from_json(s.toObject());
    case QJsonValue::Undefined: return std::monostate();
    }

    return std::monostate();
}

static std::variant<noo::AnyVar, QString> from_raw_string(QString const& s) {

    auto ls = QString("[ %1 ]").arg(s);

    // to save us effort, we are going to hijack the document reader

    QJsonParseError error;

    auto doc = QJsonDocument::fromJson(ls.toLocal8Bit(), &error);

    if (error.error == QJsonParseError::NoError) {
        return from_json(doc.array().at(0));
    }

    int offset = error.offset - 2; // as we added the bracket and space...

    auto ss = s.mid(std::max(offset - 5, 0), 10);

    return QString("%1: %2").arg(ss).arg(error.errorString());
}

// =============================================================================

noo::AnyVar JSONEditDialog::parse_any(QString const& s, bool* ok) {
    auto parse_result = from_raw_string(s);

    noo::AnyVar a = VMATCH(
        parse_result,
        VCASE(noo::AnyVar & av) {
            if (ok) *ok = true;
            return std::move(av);
        },
        VCASE(QString) {
            if (ok) *ok = false;
            return noo::AnyVar();
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

noo::AnyVar JSONEditDialog::get_as_any(bool* ok) const {
    return parse_any(text(), ok);
}

bool JSONEditDialog::evaluate_text() const {
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

    ui->feedbackLabel->setText(ok ? "OK" : str);
    ui->feedbackLabel->setStyleSheet(sheet);

    return ok;
}
