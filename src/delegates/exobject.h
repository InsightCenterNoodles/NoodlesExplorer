#ifndef EXOBJECT_H
#define EXOBJECT_H

#include "componentlistmodel.h"
#include "delegates.h"
#include "exmesh.h"

#include <noo_client_interface.h>

class ExMaterial;
class ExMesh;
class ExLight;


class ExObject : public nooc::ObjectDelegate {
    Q_OBJECT

    Qt3DCore::QEntity*            m_3d_root;
    UniqueQPtr<Qt3DCore::QEntity> m_3d_entity;
    Qt3DCore::QTransform*         m_3d_transform;


    std::optional<QtGeomInfo> m_att_mesh_details;
    void                      remake_mesh_attachment();


    QString                               m_name;
    QPointer<ExObject>                    m_parent;
    glm::mat4                             m_transform;
    AttachmentPoint<ExMaterial>           m_material;
    AttachmentPoint<ExMesh>               m_mesh;
    AttachmentVector<ExLight>             m_lights;
    AttachmentVector<nooc::TableDelegate> m_tables;
    std::vector<glm::mat4>                m_instances;
    QStringList                           m_tags;
    // std::vector<QPointer<nooc::MethodDelegate>> m_method_list;
    // std::vector<QPointer<nooc::SignalDelegate>> m_signal_list;
    std::optional<nooc::ObjectText> m_text;

    void update_from(nooc::ObjectUpdateData const& md);

public:
    static QStringList header();

    ExObject(noo::ObjectID                 id,
             nooc::ObjectUpdateData const& md,
             Qt3DCore::QEntity*            scene_root);

    ~ExObject();

    int      get_id() const;
    int      get_id_gen() const;
    QString  get_name() const;
    QVariant get_column(int c) const;

    void on_update(nooc::ObjectUpdateData const&) override;

    Qt3DCore::QEntity* entity();

private slots:
    void material_changed();
    void mesh_changed();
};

class TaggedNameObjectFilter : public QSortFilterProxyModel {
    Q_OBJECT

    QString m_filter;

    Q_PROPERTY(
        QString filter READ filter WRITE set_filter NOTIFY filter_changed)

    QStringList m_names;
    QStringList m_tags;

public:
    explicit TaggedNameObjectFilter(QObject* p = nullptr);

    bool filterAcceptsRow(int                source_row,
                          QModelIndex const& source_parent) const override;

    QString const& filter() const;
    void           set_filter(const QString& new_filter);
signals:
    void filter_changed();
};

#endif // EXOBJECT_H
