#ifndef ARGUMENTTABLEMODEL_H
#define ARGUMENTTABLEMODEL_H

#include "structtablemodel.h"

struct Argument {
    QString name;
    QString documentation;
    QString current_value;

    static constexpr auto meta =
        std::tuple(MetaMember(&Argument::name, "name", false),
                   MetaMember(&Argument::documentation, "documentation", false),
                   MetaMember(&Argument::current_value, "current_value", true));
};

class ArgumentTableModel : public StructTableModel<Argument> {
    Q_OBJECT

    Q_PROPERTY(QString method_name READ method_name WRITE set_method_name NOTIFY
                   method_name_changed)

    Q_PROPERTY(QString method_documentation READ method_documentation WRITE
                   set_method_documentation NOTIFY method_documentation_changed)

    QString m_method_name;
    QString m_method_documentation;

public:
    explicit ArgumentTableModel(QObject* parent = nullptr);

    QString method_name() const { return m_method_name; }
    QString method_documentation() const { return m_method_documentation; }

    QVariant data(const QModelIndex& index, int role) const override;

public slots:
    void set_method_name(QString method_name);
    void set_method_documentation(QString method_documentation);

signals:
    void method_name_changed(QString method_name);
    void method_documentation_changed(QString method_documentation);
};

#endif // ARGUMENTTABLEMODEL_H
