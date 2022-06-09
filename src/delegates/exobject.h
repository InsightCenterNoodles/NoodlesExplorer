#ifndef EXOBJECT_H
#define EXOBJECT_H

#include "componentlistmodel.h"
#include "delegates.h"
#include "exmesh.h"

#include <noo_client_interface.h>

#include <QMatrix4x4>
#include <QQuick3DInstancing>

class ExMaterial;
class ExMesh;
class ExLight;
class ExObject;
class AttachedMethodListModel;


// =============================================================================

// we will use a sub-render object to handle multiples and such.
// thus, these are internal ids

class EntityChangeNotifier : public QObject {
    Q_OBJECT

    int32_t              m_next = 0;
    std::vector<int32_t> m_free_list;

public:
    explicit EntityChangeNotifier(QObject* parent = nullptr);
    ~EntityChangeNotifier();

    int32_t new_id() {
        if (m_free_list.empty()) {
            int32_t ret = m_next;
            m_next++;
            return ret;
        }
        int32_t ret = m_free_list.back();
        m_free_list.pop_back();
        return ret;
    }

    void return_id(int32_t id) { m_free_list.push_back(id); }

signals:
    void ask_delete(int32_t);
    void ask_create(int32_t             new_id,
                    int32_t             parent_id = -1,
                    QObject*            material  = nullptr,
                    QQuick3DGeometry*   mesh      = nullptr,
                    QQuick3DInstancing* instances = nullptr);
    void ask_set_tf(int32_t, QMatrix4x4 transform);
    void ask_set_parent(int32_t new_id, int32_t parent_id);
};

// =============================================================================

class RepresentationPart : public QObject {

public:
    RepresentationPart(QObject*);
    ~RepresentationPart() override;

    virtual QString info_string() const = 0;
};

// =============================================================================

class TextPart : public RepresentationPart {
public:
    QString info_string() const override;
};

// =============================================================================

class WebPart : public RepresentationPart {
public:
    QString info_string() const override;
};

// =============================================================================

class QMLInstanceTable : public QQuick3DInstancing {
    Q_OBJECT

    int        m_ready_instances = 0;
    QByteArray m_instance_data;

protected:
    QByteArray getInstanceBuffer(int* instanceCount) override;

public:
    QMLInstanceTable(nooc::InstanceSource const& src);
    ~QMLInstanceTable();

private slots:
    void buffer_ready(QByteArray);
};


class RenderSubObject {
    QPointer<EntityChangeNotifier> m_notifier;

    UniqueQPtr<QMLInstanceTable> m_table;

    int32_t m_id;

public:
    RenderSubObject(EntityChangeNotifier*                   n,
                    int32_t                                 parent_id,
                    nooc::EntityRenderableDefinition const& def,
                    ExMeshGeometry&                         geom);
    ~RenderSubObject();
};

class RenderPart : public RepresentationPart {
    Q_OBJECT

    QPointer<EntityChangeNotifier> m_notifier;

    QPointer<ExMesh>       m_mesh;
    std::vector<glm::mat4> m_instances;

    std::vector<std::unique_ptr<RenderSubObject>> m_sub_ids;

public:
    RenderPart(EntityChangeNotifier*,
               nooc::EntityRenderableDefinition const& def,
               ExObject*);

    ~RenderPart();

    QString info_string() const override;

private slots:
    void material_changed();
    void mesh_changed();
};

// =============================================================================

class ExObject : public nooc::EntityDelegate {
    Q_OBJECT

    UniqueQPtr<RepresentationPart> m_attached_part;
    QPointer<EntityChangeNotifier> m_notifier;

    AttachmentVector<ExLight>             m_lights;
    AttachmentVector<nooc::TableDelegate> m_tables;

    AttachedMethodListModel* m_attached_methods;

    void    rebuild(bool representation, bool methods);
    int32_t m_root = -1;

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

    int32_t internal_root() const { return m_root; }

    void on_complete() override;
    void on_update(nooc::EntityUpdateData const&) override;
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
