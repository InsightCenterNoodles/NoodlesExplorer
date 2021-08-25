#ifndef EXMETHOD_H
#define EXMETHOD_H

#include "componentlistmodel.h"

#include <noo_client_interface.h>

class ExMethod : public nooc::MethodDelegate {
    QString     m_documentation;
    QString     m_return_documentation;
    QStringList m_argument_names;
    QStringList m_argument_details;

public:
    static QStringList header();

    ExMethod(noo::MethodID id, nooc::MethodData const& md);

    ~ExMethod();

    int      get_id() const;
    int      get_id_gen() const;
    QString  get_name() const;
    QVariant get_column(int c) const;


    QString     documentation() const { return m_documentation; }
    QString     return_documentation() const { return m_return_documentation; }
    QStringList argument_names() const { return m_argument_names; }
    QStringList argument_details() const { return m_argument_details; }

    // void prepare_delete() override { unregister(); }
};

#endif // EXMETHOD_H
