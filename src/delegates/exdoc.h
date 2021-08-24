#ifndef EXDOC_H
#define EXDOC_H

#include <noo_client_interface.h>

class ExDoc : public nooc::DocumentDelegate {
    Q_OBJECT

public:
    ExDoc();
    ~ExDoc();

    void on_update(nooc::DocumentData const&) override;
    void on_clear() override;

    QStringList get_ab_method_ids() const;
};

#endif // EXDOC_H
