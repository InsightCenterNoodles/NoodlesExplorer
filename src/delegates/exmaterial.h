#ifndef EXMATERIAL_H
#define EXMATERIAL_H

#include "componentlistmodel.h"
#include "delegates.h"

#include <noo_client_interface.h>

class MaterialChangeNotifier : public ChangeNotifierBase {
    Q_OBJECT

public:
    explicit MaterialChangeNotifier(QObject* parent = nullptr);
    ~MaterialChangeNotifier();

signals:
    void ask_delete(int32_t);
    void
    ask_create(int32_t new_id, QColor base_color, float metal, float rough);
};

class ExMaterial : public nooc::MaterialDelegate {
    Q_OBJECT

    QPointer<MaterialChangeNotifier> m_notifier;
    int32_t                          m_qt_mat_id = -1;

public:
    static QStringList header();

    ExMaterial(noo::MaterialID           id,
               nooc::MaterialInit const& md,
               MaterialChangeNotifier*   notifier);

    ~ExMaterial();

    int      get_id() const;
    int      get_id_gen() const;
    QString  get_name() const;
    QVariant get_column(int c) const;

    void on_update(nooc::MaterialUpdate const&) override;

    int32_t qt_mat_id() const { return m_qt_mat_id; }
};

#endif // EXMATERIAL_H
