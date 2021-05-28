#ifndef EXMETHOD_H
#define EXMETHOD_H

#include "componentlistmodel.h"

#include <noo_client_interface.h>

class ExMethod : public nooc::MethodDelegate, public ComponentListItem {
    QString     m_documentation;
    QString     m_return_documentation;
    QStringList m_argument_names;
    QStringList m_argument_details;

public:
    static QStringList header();

    ExMethod(noo::MethodID                       id,
             nooc::MethodData const&             md,
             std::shared_ptr<ComponentListModel> list);

    ~ExMethod();

    int      get_id() const override;
    int      get_id_gen() const override;
    QString  get_name() const override;
    QVariant get_column(int c) const override;


    QString     documentation() const { return m_documentation; }
    QString     return_documentation() const { return m_return_documentation; }
    QStringList argument_names() const { return m_argument_names; }
    QStringList argument_details() const { return m_argument_details; }

    void prepare_delete() override { unregister(); }
};

#endif // EXMETHOD_H
