#ifndef DELEGATES_H
#define DELEGATES_H

#include "componentlistmodel.h"

#include <noo_client_interface.h>

#include <QPointer>
#include <Qt3DExtras/QMetalRoughMaterial>
#include <Qt3DRender/QBuffer>

namespace Qt3DCore {
class QEntity;
class QTransform;
} // namespace Qt3DCore

namespace Qt3DRender {
class QMaterial;
class QGeometry;
class QGeometryRenderer;
class QAbstractLight;
class QAttribute;
} // namespace Qt3DRender

class QTreeWidget;
class QTreeWidgetItem;

class QInstancedMetalRoughMaterial;


template <class T>
struct UniqueQPtr {
    QPointer<T> pointer;

    UniqueQPtr() = default;

    UniqueQPtr(T* t) : pointer(t) { }

    ~UniqueQPtr() {
        if (pointer) { pointer->deleteLater(); }
    }

    UniqueQPtr(UniqueQPtr const&) = delete;
    UniqueQPtr& operator=(UniqueQPtr const&) = delete;

    UniqueQPtr(UniqueQPtr&& other) {
        pointer = other.pointer;

        other.pointer.clear();
    }

    UniqueQPtr& operator=(UniqueQPtr&& other) {
        if (&other == this) return *this;
        reset();

        pointer = other.pointer;

        other.pointer.clear();

        return *this;
    }

    void reset(T* lt = nullptr) {
        if (pointer) {
            pointer->deleteLater();
            pointer.clear();
        }

        pointer = lt;
    }

    T* data() { return pointer.data(); }

    operator T*() const { return pointer; }
    operator bool() const { return (bool)pointer; }

    T* operator->() const { return pointer; }
};

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
    QString  get_name() const override;
    QVariant get_column(int c) const override;


    QString     documentation() const { return m_documentation; }
    QString     return_documentation() const { return m_return_documentation; }
    QStringList argument_names() const { return m_argument_names; }
    QStringList argument_details() const { return m_argument_details; }

    void prepare_delete() override { unregister(); }
};

// =============================================================================

class ExSignal : public nooc::SignalDelegate, public ComponentListItem {
    QString     m_documentation;
    QStringList m_argument_documentation;

public:
    static QStringList header();

    ExSignal(noo::SignalID                       id,
             nooc::SignalData const&             md,
             std::shared_ptr<ComponentListModel> list);

    ~ExSignal();

    int      get_id() const override;
    QString  get_name() const override;
    QVariant get_column(int c) const override;

    void prepare_delete() override { unregister(); }
};

// =============================================================================

class QNetworkReply;

class ExBuffer : public nooc::BufferDelegate, public ComponentListItem {
    Q_OBJECT

    QByteArray m_array;

    bool m_ready = false;

    Qt3DCore::QEntity*              m_scene_root = nullptr;
    UniqueQPtr<Qt3DRender::QBuffer> m_3d_entity;

public:
    static QStringList header();

    ExBuffer(noo::BufferID                       id,
             nooc::BufferData const&             md,
             std::shared_ptr<ComponentListModel> list,
             Qt3DCore::QEntity*                  scene_root);

    ~ExBuffer();

    void prepare_delete() override { unregister(); }

    int      get_id() const override;
    QString  get_name() const override;
    QVariant get_column(int c) const override;

    size_t            size() const;
    QByteArray const& byte_array() const { return m_array; }

    Qt3DRender::QBuffer* entity();


public slots:
    void url_fetched(QNetworkReply*);
};

// =============================================================================

class ExTexture : public nooc::TextureDelegate, public ComponentListItem {
    nooc::TextureData m_data;

public:
    static QStringList header();

    ExTexture(noo::TextureID                      id,
              nooc::TextureData const&            md,
              std::shared_ptr<ComponentListModel> list);

    ~ExTexture();

    void prepare_delete() override { unregister(); }

    int      get_id() const override;
    QString  get_name() const override;
    QVariant get_column(int c) const override;
    void     on_update(nooc::TextureData const&) override;
};

// =============================================================================

class ExMaterial : public nooc::MaterialDelegate, public ComponentListItem {
    nooc::MaterialData m_data;

    UniqueQPtr<QInstancedMetalRoughMaterial> m_3d_entity;

public:
    static QStringList header();

    ExMaterial(noo::MaterialID                     id,
               nooc::MaterialData const&           md,
               std::shared_ptr<ComponentListModel> list,
               Qt3DCore::QEntity*                  scene_root);

    ~ExMaterial();

    void prepare_delete() override { unregister(); }

    int      get_id() const override;
    QString  get_name() const override;
    QVariant get_column(int c) const override;
    void     on_update(nooc::MaterialData const&) override;

    QInstancedMetalRoughMaterial* entity();
};

// =============================================================================

class ExLight : public nooc::LightDelegate, public ComponentListItem {
    nooc::LightData m_data;

    UniqueQPtr<Qt3DRender::QAbstractLight> m_3d_entity;

public:
    static QStringList header();

    ExLight(noo::LightID                        id,
            nooc::LightData const&              md,
            std::shared_ptr<ComponentListModel> list,
            Qt3DCore::QEntity*                  scene_root);

    ~ExLight();

    void prepare_delete() override { unregister(); }

    int      get_id() const override;
    QString  get_name() const override;
    QVariant get_column(int c) const override;
    void     on_update(nooc::LightData const&) override;

    Qt3DRender::QAbstractLight* entity();
};

// =============================================================================

struct QtGeomInfo {
    UniqueQPtr<Qt3DRender::QGeometry>         geom;
    UniqueQPtr<Qt3DRender::QGeometryRenderer> renderer;

    QByteArray                         instance_data;
    UniqueQPtr<Qt3DRender::QBuffer>    instance_buffer;
    UniqueQPtr<Qt3DRender::QAttribute> instance_attribute;
};

class ExMesh : public nooc::MeshDelegate, public ComponentListItem {
    nooc::MeshData     m_data;
    Qt3DCore::QEntity* m_scene_root;

public:
    static QStringList header();

    ExMesh(noo::MeshID                         id,
           nooc::MeshData const&               md,
           std::shared_ptr<ComponentListModel> list,
           Qt3DCore::QEntity*                  scene_root);

    ~ExMesh();

    void prepare_delete() override { unregister(); }

    int      get_id() const override;
    QString  get_name() const override;
    QVariant get_column(int c) const override;

    QtGeomInfo make_new_info(std::span<glm::mat4> instances);
};

// =============================================================================

class ExObject : public nooc::ObjectDelegate, public ComponentListItem {
    Qt3DCore::QEntity*            m_3d_root;
    UniqueQPtr<Qt3DCore::QEntity> m_3d_entity;
    Qt3DCore::QTransform*         m_3d_transform;


    std::optional<QtGeomInfo> m_att_mesh_details;
    void                      remake_mesh_attachment();


    QString                               m_name;
    std::shared_ptr<ExObject>             m_parent;
    glm::mat4                             m_transform;
    std::shared_ptr<ExMaterial>           m_material;
    std::shared_ptr<ExMesh>               m_mesh;
    std::vector<std::shared_ptr<ExLight>> m_lights;
    std::vector<nooc::TableDelegatePtr>   m_tables;
    std::vector<glm::mat4>                m_instances;
    QStringList                           m_tags;
    std::vector<nooc::MethodDelegatePtr>  m_method_list;
    std::vector<nooc::SignalDelegatePtr>  m_signal_list;
    std::optional<nooc::ObjectText>       m_text;

    void update_from(nooc::ObjectUpdateData const& md);

public:
    static QStringList header();

    ExObject(noo::ObjectID                       id,
             nooc::ObjectUpdateData const&       md,
             std::shared_ptr<ComponentListModel> list,
             Qt3DCore::QEntity*                  scene_root);

    ~ExObject();

    int      get_id() const override;
    QString  get_name() const override;
    QVariant get_column(int c) const override;

    void on_update(nooc::ObjectUpdateData const&) override;

    Qt3DCore::QEntity* entity();
};

// =============================================================================

class ExTable : public nooc::TableDelegate, public ComponentListItem {
    QString m_name;

    std::vector<nooc::MethodDelegatePtr> m_methods;

    void set_from(nooc::TableData const& md);

public:
    static QStringList header();

    ExTable(noo::TableID                        id,
            nooc::TableData const&              md,
            std::shared_ptr<ComponentListModel> list);

    ~ExTable();

    void prepare_delete() override { unregister(); }

    int      get_id() const override;
    QString  get_name() const override;
    QVariant get_column(int c) const override;

    void on_update(nooc::TableData const&) override;
};

// =============================================================================

class ExDoc : public nooc::DocumentDelegate {
    Q_OBJECT
    std::vector<nooc::MethodDelegatePtr> m_methods;

public:
    ExDoc();
    ~ExDoc();

    void on_update(nooc::DocumentData const&) override;
    void on_clear() override;

    QStringList get_ab_method_ids() const;
};


#endif // DELEGATES_H
