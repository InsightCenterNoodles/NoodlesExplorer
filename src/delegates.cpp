#include "delegates.h"

#include "instance_material/qinstancedmetalroughmaterial.h"
#include "tabledata.h"

#include <glm/gtc/type_ptr.hpp>

#include <QColor>
#include <QDebug>
#include <QTreeWidgetItem>

#include <QAbstractLight>
#include <QAttribute>
#include <QEntity>
#include <QGeometry>
#include <QGeometryRenderer>
#include <QMaterial>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QPointLight>
#include <Qt3DCore/QTransform>
#include <Qt3DExtras/QMetalRoughMaterial>

#include <QFile>

static QString to_qstring(std::string_view v) {
    return QString(v.data());
}

static QStringList to_qstringlist(std::vector<std::string_view> v) {
    QStringList ret;

    for (auto s : v) {
        ret << to_qstring(s);
    }

    return ret;
}

template <class T>
QString ptr_to_id(std::shared_ptr<T> const& ptr) {
    if (!ptr) return "None";
    return ptr->id().to_qstring();
}


template <class T>
QStringList build_id_list(std::vector<T> const& methods) {
    QStringList ret;

    for (auto const& ptr : methods) {
        ret << ptr->id().to_qstring();
    }

    return ret;
}

// =============================================================================

QStringList ExMethod::header() {
    return {
        "ID", "Name", "Documentation", "Return Documentation", "Arguments"
    };
}

ExMethod::ExMethod(noo::MethodID                       id,
                   nooc::MethodData const&             md,
                   std::shared_ptr<ComponentListModel> list)
    : nooc::MethodDelegate(id, md), ComponentListItem(list) {

    m_documentation        = to_qstring(md.documentation);
    m_return_documentation = to_qstring(md.return_documentation);

    for (auto a : md.argument_documentation) {
        m_argument_names << to_qstring(a.name);
        m_argument_details << to_qstring(a.doc);
    }
}

ExMethod::~ExMethod() = default;

int ExMethod::get_id() const {
    return this->id().id_slot;
}
int ExMethod::get_id_gen() const {
    return this->id().id_gen;
}
QString ExMethod::get_name() const {
    return to_qstring(this->name());
}

QVariant ExMethod::get_column(int c) const {
    switch (c) {
    case 0: return get_id();
    case 1: return get_name();
    case 2: return m_documentation;
    case 3: return m_return_documentation;
    case 4: return m_argument_names.join(", ");
    }
    return {};
}

// =============================================================================

QStringList ExSignal::header() {
    return { "ID", "Name", "Documentation", "Arguments" };
}

ExSignal::ExSignal(noo::SignalID                       id,
                   nooc::SignalData const&             md,
                   std::shared_ptr<ComponentListModel> list)
    : nooc::SignalDelegate(id, md), ComponentListItem(list) {

    m_documentation = to_qstring(md.documentation);

    for (auto a : md.argument_documentation) {
        m_argument_documentation << to_qstring(a.name);
    }
}

ExSignal::~ExSignal() = default;

int ExSignal::get_id() const {
    return this->id().id_slot;
}
int ExSignal::get_id_gen() const {
    return this->id().id_gen;
}
QString ExSignal::get_name() const {
    return to_qstring(this->name());
}

QVariant ExSignal::get_column(int c) const {
    switch (c) {
    case 0: return get_id();
    case 1: return get_name();
    case 2: return m_documentation;
    case 3: return m_argument_documentation;
    }
    return {};
}

// =============================================================================

QStringList ExBuffer::header() {
    return { "ID", "Name", "Size", "Ready" };
}

ExBuffer::ExBuffer(noo::BufferID                       id,
                   nooc::BufferData const&             md,
                   std::shared_ptr<ComponentListModel> list,
                   Qt3DCore::QEntity*                  scene_root)
    : nooc::BufferDelegate(id, md),
      ComponentListItem(list),
      m_scene_root(scene_root) {

    if (!md.data.empty()) {
        m_array = QByteArray((const char*)md.data.data(), md.data.size());
        m_ready = true;
    } else if (md.url.isValid()) {
        m_array.fill('\0', md.url_size);

        auto* nam = new QNetworkAccessManager(this);

        connect(nam,
                &QNetworkAccessManager::finished,
                this,
                &ExBuffer::url_fetched);

        connect(nam,
                &QNetworkAccessManager::finished,
                nam,
                &QNetworkAccessManager::deleteLater);

        nam->get(QNetworkRequest(md.url));
    }
}

ExBuffer::~ExBuffer() = default;

int ExBuffer::get_id() const {
    return this->id().id_slot;
}
int ExBuffer::get_id_gen() const {
    return this->id().id_gen;
}
QString ExBuffer::get_name() const {
    return id().to_qstring();
}

QVariant ExBuffer::get_column(int c) const {
    switch (c) {
    case 0: return get_id();
    case 1: return get_name();
    case 2: return (qint64)size();
    case 3: return m_ready;
    }
    return {};
}

size_t ExBuffer::size() const {
    return m_array.size();
}

Qt3DRender::QBuffer* ExBuffer::entity() {
    if (!m_3d_entity) {
        m_3d_entity = new Qt3DRender::QBuffer(m_scene_root);

        // doesnt matter if this ready or not;
        m_3d_entity->setData(m_array);
    }

    return m_3d_entity;
}

void ExBuffer::url_fetched(QNetworkReply* reply) {
    reply->deleteLater();

    if (reply->error() != QNetworkReply::NoError) {
        qWarning() << "Unable to fetch resource"
                   << reply->request().url().toString() << " : "
                   << reply->errorString();
        return;
    }

    auto buff = reply->readAll();

    if (buff.size() < m_array.size()) {
        // bad data!
        qWarning() << "Unable to fetch resource"
                   << reply->request().url().toString()
                   << ": Data size does not match given byte count:"
                   << buff.size() << "versus" << m_array.size();
        return;
    }

    m_array = buff;

    m_3d_entity->setData(buff);
}

// =============================================================================

QStringList ExTexture::header() {
    return { "ID", "Name", "Buffer", "Size" };
}

ExTexture::ExTexture(noo::TextureID                      id,
                     nooc::TextureData const&            md,
                     std::shared_ptr<ComponentListModel> list)
    : nooc::TextureDelegate(id, md), ComponentListItem(list), m_data(md) { }

ExTexture::~ExTexture() = default;

int ExTexture::get_id() const {
    return this->id().id_slot;
}
int ExTexture::get_id_gen() const {
    return this->id().id_gen;
}
QString ExTexture::get_name() const {
    return id().to_qstring();
}

QVariant ExTexture::get_column(int c) const {
    switch (c) {
    case 0: return get_id();
    case 1: return get_name();
    case 2: return ptr_to_id(m_data.buffer);
    case 3: return m_data.buffer ? (int)m_data.size : 0;
    }
    return {};
}

void ExTexture::on_update(nooc::TextureData const& md) {
    m_data = md;
}

// =============================================================================

QStringList ExMaterial::header() {
    return { "ID", "Name", "Color", "Surface", "Texture" };
}

ExMaterial::ExMaterial(noo::MaterialID                     id,
                       nooc::MaterialData const&           md,
                       std::shared_ptr<ComponentListModel> list,
                       Qt3DCore::QEntity*                  scene_root)
    : nooc::MaterialDelegate(id, md),
      ComponentListItem(list),
      m_data(md),
      m_3d_entity(new QInstancedMetalRoughMaterial(scene_root)) {
    // m_3d_entity(new Qt3DExtras::QMetalRoughMaterial(scene_root)) {


    m_3d_entity->setBaseColor(
        QColor::fromRgbF(md.color.r, md.color.g, md.color.b));
    m_3d_entity->setMetalness(md.metallic);
    m_3d_entity->setRoughness(md.roughness);

    m_3d_entity->setObjectName(get_name());
}

ExMaterial::~ExMaterial() = default;

int ExMaterial::get_id() const {
    return this->id().id_slot;
}
int ExMaterial::get_id_gen() const {
    return this->id().id_gen;
}
QString ExMaterial::get_name() const {
    return id().to_qstring();
}

QVariant ExMaterial::get_column(int c) const {
    switch (c) {
    case 0: return get_id();
    case 1: return get_name();
    case 2:
        return QColor::fromRgbF(m_data.color.r, m_data.color.g, m_data.color.b);
    case 3:
        return QString("M: %1, R: %2, B: %3")
            .arg(m_data.metallic)
            .arg(m_data.roughness)
            .arg(m_data.use_blending);
    case 4: return ptr_to_id(m_data.texture);
    }
    return {};
}

void ExMaterial::on_update(nooc::MaterialData const& md) {
    m_data = md;

    m_3d_entity->setBaseColor(
        QColor::fromRgbF(md.color.r, md.color.g, md.color.b));
    m_3d_entity->setMetalness(md.metallic);
    m_3d_entity->setRoughness(md.roughness);
}

QInstancedMetalRoughMaterial* ExMaterial::entity() {
    // Qt3DExtras::QMetalRoughMaterial* ExMaterial::entity() {
    return m_3d_entity;
}

// =============================================================================

QStringList ExLight::header() {
    return { "ID", "Name", "Color", "Intensity" };
}

ExLight::ExLight(noo::LightID                        id,
                 nooc::LightData const&              md,
                 std::shared_ptr<ComponentListModel> list,
                 Qt3DCore::QEntity*                  scene_root)
    : nooc::LightDelegate(id, md), ComponentListItem(list), m_data(md) {

    auto* p = new Qt3DRender::QPointLight(scene_root);

    p->setColor(QColor::fromRgbF(md.color.r, md.color.g, md.color.b));
    p->setIntensity(md.intensity);

    m_3d_entity.reset(p);
}

ExLight::~ExLight() = default;

int ExLight::get_id() const {
    return this->id().id_slot;
}
int ExLight::get_id_gen() const {
    return this->id().id_gen;
}
QString ExLight::get_name() const {
    return id().to_qstring();
}

QVariant ExLight::get_column(int c) const {
    switch (c) {
    case 0: return get_id();
    case 1: return get_name();
    case 2: return QColor(m_data.color.r, m_data.color.g, m_data.color.b);
    case 3: return m_data.intensity;
    }
    return {};
}

void ExLight::on_update(nooc::LightData const& md) {
    m_data = md;
}

Qt3DRender::QAbstractLight* ExLight::entity() {
    return m_3d_entity;
}

// =============================================================================

QDebug operator<<(QDebug debug, glm::vec3 const& c) {
    QDebugStateSaver saver(debug);
    debug.nospace() << '(' << c.x << ", " << c.y << ", " << c.z << ')';

    return debug;
}

QStringList ExMesh::header() {
    return { "ID", "Name", "# Vertex", "# Prim", "Components" };
}

template <class T>
struct VertexTypeTrait;

template <>
struct VertexTypeTrait<glm::vec2> {
    static constexpr auto element_type  = Qt3DRender::QAttribute::Float;
    static constexpr auto element_count = 2;
};

template <>
struct VertexTypeTrait<glm::vec3> {
    static constexpr auto element_type  = Qt3DRender::QAttribute::Float;
    static constexpr auto element_count = 3;
};

template <>
struct VertexTypeTrait<glm::u8vec4> {
    static constexpr auto element_type  = Qt3DRender::QAttribute::UnsignedByte;
    static constexpr auto element_count = 4;
};

template <>
struct VertexTypeTrait<glm::u16vec2> {
    static constexpr auto element_type  = Qt3DRender::QAttribute::UnsignedShort;
    static constexpr auto element_count = 2;
};

template <>
struct VertexTypeTrait<glm::u16vec3> {
    static constexpr auto element_type  = Qt3DRender::QAttribute::UnsignedShort;
    static constexpr auto element_count = 3;
};


template <class VertexType>
Qt3DRender::QAttribute*
attrib_from_ref(QString                                  name,
                Qt3DRender::QGeometry*                   node,
                std::optional<nooc::ComponentRef> const& oref) {
    qDebug() << "New Attrib" << typeid(VertexType).name() << name << node
             << (bool)oref;

    if (!oref) return nullptr;

    auto const& ref = *oref;

    auto buff = std::dynamic_pointer_cast<ExBuffer>(ref.buffer);

    if (!buff) return nullptr;

    int count = ref.size / ref.stride;

    qDebug() << "Attrib has" << count << ref.start << ref.size << ref.stride;

    if (ref.size >= buff->size()) {
        qWarning() << "Reference is out side of buffer bounds!";
        return nullptr;
    }

    auto* p =
        new Qt3DRender::QAttribute(buff->entity(),
                                   name,
                                   VertexTypeTrait<VertexType>::element_type,
                                   VertexTypeTrait<VertexType>::element_count,
                                   count,
                                   ref.start,
                                   ref.stride,
                                   node);

    p->setAttributeType(Qt3DRender::QAttribute::VertexAttribute);

    qDebug() << p->name() << p->count() << p->attributeType()
             << p->vertexBaseType() << p->vertexSize() << p->buffer()
             << p->byteOffset() << p->byteStride();

    return p;
}

Qt3DRender::QAttribute* instance_attrib(Qt3DRender::QGeometry* node,
                                        Qt3DRender::QBuffer*   array) {
    qDebug() << "New Attrib" << typeid(glm::mat4).name() << node;

    if (array->data().isEmpty()) return nullptr;

    int count = array->data().size() / sizeof(glm::mat4);

    qDebug() << "Index attrib has" << count;

    auto* p = new Qt3DRender::QAttribute(array,
                                         "raw_instance",
                                         Qt3DRender::QAttribute::Float,
                                         16,
                                         count,
                                         0,
                                         0,
                                         node);

    p->setAttributeType(Qt3DRender::QAttribute::VertexAttribute);
    p->setDivisor(1);

    qDebug() << p->name() << p->count() << p->attributeType()
             << p->vertexBaseType() << p->vertexSize() << p->buffer()
             << p->byteOffset() << p->byteStride();

    return p;
}

Qt3DRender::QAttribute* instance_fake_pos_attrib(Qt3DRender::QGeometry* node,
                                                 Qt3DRender::QBuffer*   array) {
    qDebug() << "New Attrib" << typeid(glm::mat4).name() << node;

    if (array->data().isEmpty()) return nullptr;

    int count = array->data().size() / sizeof(glm::mat4);

    qDebug() << "Index attrib has" << count;

    auto* p = new Qt3DRender::QAttribute(array,
                                         "instance_positions",
                                         Qt3DRender::QAttribute::Float,
                                         4,
                                         count,
                                         0,
                                         sizeof(glm::mat4),
                                         node);

    p->setAttributeType(Qt3DRender::QAttribute::VertexAttribute);

    qDebug() << p->name() << p->count() << p->attributeType()
             << p->vertexBaseType() << p->vertexSize() << p->buffer()
             << p->byteOffset() << p->byteStride();

    return p;
}


template <class IndexType>
Qt3DRender::QAttribute*
attrib_from_idx_ref(Qt3DRender::QGeometry*                   node,
                    std::optional<nooc::ComponentRef> const& oref) {

    if (!oref) return nullptr;

    auto const& ref = *oref;

    auto buff = std::dynamic_pointer_cast<ExBuffer>(ref.buffer);

    if (!buff) return nullptr;

    int count = ref.size / sizeof(IndexType);

    qDebug() << Q_FUNC_INFO << ref.size << count;

    auto* p = new Qt3DRender::QAttribute(node);
    p->setBuffer(buff->entity());
    p->setAttributeType(Qt3DRender::QAttribute::IndexAttribute);

    p->setVertexBaseType(VertexTypeTrait<IndexType>::element_type);
    // p->setVertexSize(VertexTypeTrait<IndexType>::element_count);
    p->setCount(VertexTypeTrait<IndexType>::element_count * count);
    p->setByteOffset(ref.start);

    qDebug() << p->name() << p->count() << p->attributeType()
             << p->vertexBaseType() << p->vertexSize() << p->buffer()
             << p->byteOffset() << p->byteStride();

    qDebug() << "BF" << buff->entity()->data().size();

    return p;
}

ExMesh::ExMesh(noo::MeshID                         id,
               nooc::MeshData const&               md,
               std::shared_ptr<ComponentListModel> list,
               Qt3DCore::QEntity*                  scene_root)
    : nooc::MeshDelegate(id, md),
      ComponentListItem(list),
      m_data(md),
      m_scene_root(scene_root) { }

ExMesh::~ExMesh() = default;

int ExMesh::get_id() const {
    return this->id().id_slot;
}
int ExMesh::get_id_gen() const {
    return this->id().id_gen;
}
QString ExMesh::get_name() const {
    return id().to_qstring();
}

QVariant ExMesh::get_column(int c) const {
    switch (c) {
    case 0: return get_id();
    case 1: return get_name();
    case 2: {
        if (!m_data.positions) return 0;
        auto p = m_data.positions.value();

        return quint64(p.size / p.stride);
    }
    case 3: {
        if (m_data.lines) {
            return quint64(m_data.lines->size / m_data.lines->stride);
        }

        if (m_data.triangles) {
            return quint64(m_data.triangles->size / m_data.triangles->stride);
        }

        return 0;
    }
    case 4: {
        return QString("P: %1, N: %2, C: %3, T: %4")
            .arg(m_data.positions.has_value())
            .arg(m_data.normals.has_value())
            .arg(m_data.colors.has_value())
            .arg(m_data.textures.has_value());
    }
    }
    return {};
}

QtGeomInfo ExMesh::make_new_info(std::span<glm::mat4> instances) {
    using namespace Qt3DRender;

    QtGeomInfo ret;

    qDebug() << "New geometry" << m_data.extent_min << m_data.extent_max;

    if (!m_data.positions) return ret;
    if (!m_data.triangles and !m_data.lines) return ret;

    // set up geometry
    ret.geom = new Qt3DRender::QGeometry(m_scene_root);

    ret.geom->setObjectName("Geom " + get_name());

    ret.renderer = new Qt3DRender::QGeometryRenderer(m_scene_root);

    ret.renderer->setObjectName("GeomRenderer " + get_name());

    auto* pa = attrib_from_ref<glm::vec3>(
        QAttribute::defaultPositionAttributeName(), ret.geom, m_data.positions);

    auto* na = attrib_from_ref<glm::vec3>(
        QAttribute::defaultNormalAttributeName(), ret.geom, m_data.normals);

    auto* ta = attrib_from_ref<glm::u16vec2>(
        QAttribute::defaultTextureCoordinateAttributeName(),
        ret.geom,
        m_data.textures);

    auto* ca = attrib_from_ref<glm::u8vec4>(
        QAttribute::defaultColorAttributeName(), ret.geom, m_data.colors);

    QAttribute* idx_p = nullptr;

    if (m_data.triangles) {
        idx_p = attrib_from_idx_ref<glm::u16vec3>(ret.geom, m_data.triangles);

        ret.renderer->setPrimitiveType(QGeometryRenderer::Triangles);

    } else if (m_data.lines) {
        idx_p = attrib_from_idx_ref<glm::u16vec2>(ret.geom, m_data.lines);

        ret.renderer->setPrimitiveType(QGeometryRenderer::Lines);
    }


    if (pa) ret.geom->addAttribute(pa);
    if (na) ret.geom->addAttribute(na);
    if (ta) ret.geom->addAttribute(ta);
    if (ca) ret.geom->addAttribute(ca);
    if (idx_p) ret.geom->addAttribute(idx_p);

    static glm::mat4 ident = []() {
        glm::mat4 id;
        id[0] = { 0, 0, 0, 1 };
        id[1] = { 1, 1, 1, 1 };
        id[2] = { 0, 0, 0, 1 };
        id[3] = { 1, 1, 1, 1 };
        return id;
    }();

    if (instances.empty()) { instances = std::span(&ident, 1); }

    //    for (auto const& m : instances) {
    //        qDebug() << m[0].x << m[0].y << m[0].z << m[0].w;
    //    }

    ret.instance_data.resize(instances.size_bytes());

    std::memcpy(
        ret.instance_data.data(), instances.data(), instances.size_bytes());

    ret.instance_buffer = new Qt3DRender::QBuffer(m_scene_root);

    ret.instance_buffer->setData(ret.instance_data);

    ret.instance_attribute.reset(
        instance_attrib(ret.geom, ret.instance_buffer));

    ret.geom->addAttribute(ret.instance_attribute);

    // now we need another attribute to get proper cull bounds from instances
    auto* fake_attrib = instance_fake_pos_attrib(ret.geom, ret.instance_buffer);

    ret.geom->addAttribute(fake_attrib);
    ret.geom->setBoundingVolumePositionAttribute(fake_attrib);

    ret.renderer->setGeometry(ret.geom);

    if (!instances.empty()) {
        ret.renderer->setInstanceCount(instances.size());
    }

    return ret;
}

// =============================================================================

void ExObject::remake_mesh_attachment() {
    if (m_att_mesh_details) {
        m_3d_entity->removeComponent(m_att_mesh_details->renderer);
    }

    qDebug() << "Num instances" << m_instances.size();

    if (!m_mesh) return;

    m_att_mesh_details.emplace(m_mesh->make_new_info(m_instances));

    if (m_att_mesh_details) {
        m_3d_entity->addComponent(m_att_mesh_details->renderer);
    }
}

void ExObject::update_from(nooc::ObjectUpdateData const& md) {
    qDebug() << Q_FUNC_INFO << !!md.name << !!md.parent << !!md.transform
             << !!md.material << !!md.mesh << !!md.lights << !!md.tables
             << !!md.instances << !!md.tags << !!md.method_list
             << !!md.signal_list << !!md.text;


    bool post_instance_rebuild = false;

    if (md.name) {
        m_name = to_qstring(*md.name);
        if (m_name.isEmpty()) { m_name = id().to_qstring(); }
    }

    if (md.parent) {
        qDebug() << "Reparenting";

        // set
        m_parent = std::dynamic_pointer_cast<ExObject>(*md.parent);

        // set up the parent for the 3d scene
        m_3d_entity->setParent(m_parent ? m_parent->entity() : m_3d_root);

        qDebug() << this << "setting parent" << m_parent.get();
    }

    if (md.transform) {
        m_transform = *md.transform;

        m_3d_transform->setMatrix(QMatrix4x4(glm::value_ptr(m_transform)));
    }

    if (md.material) {
        if (m_material) { m_3d_entity->removeComponent(m_material->entity()); }

        m_material = std::dynamic_pointer_cast<ExMaterial>(*md.material);

        if (m_material) { m_3d_entity->addComponent(m_material->entity()); }

        qDebug() << this << "setting material" << m_material.get();

        if (!m_instances.empty()) post_instance_rebuild = true;
    }

    if (md.mesh) {
        m_mesh = std::dynamic_pointer_cast<ExMesh>(*md.mesh);

        qDebug() << this << "setting mesh" << m_mesh.get();

        if (!m_instances.empty()) post_instance_rebuild = true;
    }

    if (md.lights) {
        for (auto& ptr : m_lights) {
            if (ptr) { m_3d_entity->removeComponent(ptr->entity()); }
        }

        m_lights.clear();

        for (auto& ptr : *md.lights) {
            auto& p =
                m_lights.emplace_back(std::dynamic_pointer_cast<ExLight>(ptr));

            qDebug() << this << "setting light" << p.get();
        }

        for (auto& ptr : m_lights) {
            if (ptr) { m_3d_entity->addComponent(ptr->entity()); }
        }
    }

    m_tables = md.tables.value_or(std::vector<nooc::TableDelegatePtr> {});

    if (md.instances) {
        auto src = *md.instances;
        m_instances.clear();
        m_instances.insert(m_instances.end(), src.begin(), src.end());

        post_instance_rebuild = true;
    }

    if (post_instance_rebuild) remake_mesh_attachment();

    m_tags = to_qstringlist(md.tags.value_or(std::vector<std::string_view> {}));
    m_method_list =
        md.method_list.value_or(std::vector<nooc::MethodDelegatePtr> {});
    m_signal_list =
        md.signal_list.value_or(std::vector<nooc::SignalDelegatePtr> {});

    m_text = md.text;
}

QStringList ExObject::header() {
    return {
        "ID",       "Name",    "Parent",

        "Material", "Mesh",    "Lights",

        "Tags",     "Methods", "Signals",
    };
}

ExObject::ExObject(noo::ObjectID                       id,
                   nooc::ObjectUpdateData const&       md,
                   std::shared_ptr<ComponentListModel> list,
                   Qt3DCore::QEntity*                  scene_root)
    : nooc::ObjectDelegate(id, md),
      ComponentListItem(list),
      m_3d_root(scene_root) {

    m_3d_entity    = new Qt3DCore::QEntity(scene_root);
    m_3d_transform = new Qt3DCore::QTransform(m_3d_entity.data());

    m_3d_entity->addComponent(m_3d_transform);

    m_name = id.to_qstring();


    // temp add to scene root
    m_3d_entity->setParent(scene_root);
    m_3d_entity->setObjectName(m_name);

    qDebug() << "New object" << id.id_slot;

    update_from(md);
}

ExObject::~ExObject() {
    qDebug() << Q_FUNC_INFO << this;
}

int ExObject::get_id() const {
    return this->id().id_slot;
}
int ExObject::get_id_gen() const {
    return this->id().id_gen;
}
QString ExObject::get_name() const {
    return m_name;
}

QVariant ExObject::get_column(int c) const {
    switch (c) {
    case 0: return get_id();
    case 1: return get_name();
    case 2: return ptr_to_id(m_parent);

    case 3: return ptr_to_id(m_material);
    case 4: return ptr_to_id(m_mesh);
    case 5: return build_id_list(m_lights);

    case 6: return m_tags;
    case 7: return build_id_list(m_method_list);
    case 8: return build_id_list(m_signal_list);
    }
    return {};
}

void ExObject::on_update(nooc::ObjectUpdateData const& md) {
    update_from(md);
}

Qt3DCore::QEntity* ExObject::entity() {
    assert(m_3d_entity);
    return m_3d_entity.data();
}

// =============================================================================


void ExTable::set_from(nooc::TableData const& md) {
    if (md.name) {
        m_name = to_qstring(*md.name);
    } else {
        m_name = id().to_qstring();
    }

    if (md.method_list) m_methods = *md.method_list;
}

QStringList ExTable::header() {
    return { "ID", "Name" };
}

ExTable::ExTable(noo::TableID                        id,
                 nooc::TableData const&              md,
                 std::shared_ptr<ComponentListModel> list)
    : nooc::TableDelegate(id, md), ComponentListItem(list) {

    set_from(md);
}

ExTable::~ExTable() = default;

int ExTable::get_id() const {
    return this->id().id_slot;
}
int ExTable::get_id_gen() const {
    return this->id().id_gen;
}
QString ExTable::get_name() const {
    return m_name;
}

QVariant ExTable::get_column(int c) const {
    switch (c) {
    case 0: return get_id();
    case 1: return get_name();
    }
    return {};
}

void ExTable::on_update(nooc::TableData const& md) {
    set_from(md);
}

void ExTable::on_table_initialize(noo::AnyVarListRef const& names,
                                  noo::AnyVarRef            keys,
                                  noo::AnyVarListRef const& data_cols,
                                  noo::AnyVarListRef const& selections) {

    m_subscribed = true;

    qDebug() << Q_FUNC_INFO;


    if (names.size() != data_cols.size()) return;
    if (names.size() == 0) return;

    m_data = std::make_shared<RemoteTableData>();

    m_data->on_table_initialize(names, keys, data_cols, selections);

    connect(m_data.get(),
            &RemoteTableData::ask_update_row,
            [this](int64_t key, noo::AnyVarList& l) {
                this->request_row_update(key, std::move(l));
            });

    emit fetch_new_remote_table_data();
}

void ExTable::on_table_reset() {
    qDebug() << Q_FUNC_INFO;
    if (m_data) m_data->on_table_reset();
}

void ExTable::on_table_updated(noo::AnyVarRef keys, noo::AnyVarRef columns) {
    qDebug() << Q_FUNC_INFO;
    if (m_data) m_data->on_table_updated(keys, columns);
}

void ExTable::on_table_rows_removed(noo::AnyVarRef keys) {
    qDebug() << Q_FUNC_INFO;
    if (m_data) m_data->on_table_rows_removed(keys);
}
void ExTable::on_table_selection_updated(std::string_view         s,
                                         noo::SelectionRef const& r) {
    qDebug() << Q_FUNC_INFO;
    if (m_data) m_data->on_table_selection_updated(s, r);
}

// =============================================================================

ExDoc::ExDoc() { }
ExDoc::~ExDoc() = default;

void ExDoc::on_update(nooc::DocumentData const& data) {
    m_methods = data.method_list;
}
void ExDoc::on_clear() {
    m_methods.clear();
}

QStringList ExDoc::get_ab_method_ids() const {
    return build_id_list(m_methods);
}
