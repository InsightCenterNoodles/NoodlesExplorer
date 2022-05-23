#ifndef EXOBJECT_H
#define EXOBJECT_H

#include "componentlistmodel.h"
#include "delegates.h"
#include "exmesh.h"

#include <noo_client_interface.h>

#include <QMatrix4x4>

class ExMaterial;
class ExMesh;
class ExLight;
class AttachedMethodListModel;


// =============================================================================

class EntityChangeNotifier : public QObject {
    Q_OBJECT

public:
    explicit EntityChangeNotifier(QObject* parent = nullptr);
    ~EntityChangeNotifier();

signals:
    void ask_delete(int32_t);
    void ask_create(int32_t);
    void ask_set_parent(int32_t child, int32_t parent);
    void ask_set_tf(int32_t, QMatrix4x4 transform);
    void
    ask_set_render(int32_t, int32_t material, int32_t mesh, int32_t instances);
};

// =============================================================================

class RepresentationPart : public QObject {

public:
    RepresentationPart(QObject*);
    ~RepresentationPart() override;

    virtual QString info_string() const = 0;
};

class TextPart : public RepresentationPart {
public:
    QString info_string() const override;
};

class WebPart : public RepresentationPart {
public:
    QString info_string() const override;
};

class RenderPart : public RepresentationPart {
    Q_OBJECT
    AttachmentPoint<ExMaterial> m_material;
    AttachmentPoint<ExMesh>     m_mesh;
    std::vector<glm::mat4>      m_instances;

    UniqueQPtr<Qt3DCore::QEntity> m_3d_entity;

    //    std::optional<QtGeomInfo> m_att_mesh_details;
    //    void                      remake_mesh_attachment();

public:
    RenderPart(Qt3DCore::QEntity* p_entity,
               ExMaterial*,
               ExMesh*,
               std::span<glm::mat4 const>,
               QObject*);

    ~RenderPart();

    QString info_string() const override;

private slots:
    void material_changed();
    void mesh_changed();
};

// =============================================================================

class ExObject : public nooc::EntityDelegate {
    Q_OBJECT

    //    Qt3DCore::QEntity*            m_3d_root;
    //    UniqueQPtr<Qt3DCore::QEntity> m_3d_entity;
    //    Qt3DCore::QTransform*         m_3d_transform;

    //    QString            m_name;
    //    QPointer<ExObject> m_parent;
    //    glm::mat4          m_transform;

    QPointer<RepresentationPart> m_attached_part;

    AttachmentVector<ExLight>             m_lights;
    AttachmentVector<nooc::TableDelegate> m_tables;

    QStringList m_tags;

    AttachedMethodListModel* m_attached_methods;

    void update_from(nooc::EntityUpdateData const& md);

public:
    static QStringList header();

    ExObject(noo::EntityID           id,
             nooc::EntityInit const& md,
             EntityChangeNotifier*   notifier);

    ~ExObject();

    int      get_id() const;
    int      get_id_gen() const;
    QString  get_name() const;
    QVariant get_column(int c) const;

    AttachedMethodListModel* attached_method_list() const;

    void on_update(nooc::EntityUpdateData const&) override;

    //    Qt3DCore::QEntity* entity();

signals:
    void ask_delete(int32_t);
    void ask_create(int32_t);
    void ask_set_parent(int32_t child, int32_t parent);
    void ask_set_tf(int32_t, QMatrix4x4 transform);
    void
    ask_set_render(int32_t, int32_t material, int32_t mesh, int32_t instances);
};

// =============================================================================

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
