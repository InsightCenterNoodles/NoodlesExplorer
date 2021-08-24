#include "exmesh.h"

#include "exbuffer.h"

#include <QAttribute>
#include <QEntity>
#include <QGeometry>
#include <QGeometryRenderer>
#include <Qt3DRender/QBuffer>


QtGeomInfo::QtGeomInfo()  = default;
QtGeomInfo::~QtGeomInfo() = default;

QtGeomInfo::QtGeomInfo(QtGeomInfo&&) = default;
QtGeomInfo& QtGeomInfo::operator=(QtGeomInfo&&) = default;

QDebug operator<<(QDebug debug, glm::vec2 const& c) {
    QDebugStateSaver saver(debug);
    debug.nospace() << '(' << c.x << ", " << c.y << ')';

    return debug;
}

QDebug operator<<(QDebug debug, glm::vec3 const& c) {
    QDebugStateSaver saver(debug);
    debug.nospace() << '(' << c.x << ", " << c.y << ", " << c.z << ')';

    return debug;
}

QDebug operator<<(QDebug debug, glm::u8vec4 const& c) {
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
    static inline auto    blank         = glm::vec2(1);
};

template <>
struct VertexTypeTrait<glm::vec3> {
    static constexpr auto element_type  = Qt3DRender::QAttribute::Float;
    static constexpr auto element_count = 3;
    static inline auto    blank         = glm::vec3(1);
};

template <>
struct VertexTypeTrait<glm::u8vec4> {
    static constexpr auto element_type  = Qt3DRender::QAttribute::UnsignedByte;
    static constexpr auto element_count = 4;
    static inline auto    blank         = glm::u8vec4(255);
};

template <>
struct VertexTypeTrait<glm::u16vec2> {
    static constexpr auto element_type  = Qt3DRender::QAttribute::UnsignedShort;
    static constexpr auto element_count = 2;
    static inline auto blank = glm::u16vec2(std::numeric_limits<short>::max());
};

template <>
struct VertexTypeTrait<glm::u16vec3> {
    static constexpr auto element_type  = Qt3DRender::QAttribute::UnsignedShort;
    static constexpr auto element_count = 3;
    static inline auto blank = glm::u16vec3(std::numeric_limits<short>::max());
};

template <class VertexType>
void debug_attribute(ExBuffer* data, nooc::ComponentRef const& ref) {
    qDebug() << "Attribute Values" << typeid(VertexType).name();

    auto* buffer_start = data->byte_array().data();

    buffer_start += ref.start;
    auto buffer_end = buffer_start + ref.size;

    for (; buffer_start < buffer_end; buffer_start += ref.stride) {
        auto const& vt = *(VertexType*)buffer_start;
        qDebug() << vt;
    }
}


template <class VertexType>
Qt3DRender::QAttribute*
attrib_from_ref(QString                                  name,
                Qt3DRender::QGeometry*                   node,
                std::optional<nooc::ComponentRef> const& oref,
                bool                                     permit_blank) {
    qDebug() << "New Attrib" << typeid(VertexType).name() << name << node
             << (bool)oref;

    if (!oref and permit_blank) {
        qDebug() << "Creating blank";
        // synthesize a blank buffer

        using Tr = VertexTypeTrait<VertexType>;

        QByteArray array((const char*)&Tr::blank, sizeof(VertexType));

        auto* b = new Qt3DRender::QBuffer(node);

        b->setData(array);

        auto* p = new Qt3DRender::QAttribute(
            b,
            name,
            VertexTypeTrait<VertexType>::element_type,
            VertexTypeTrait<VertexType>::element_count,
            1,
            0,
            sizeof(VertexType),
            node);

        p->setAttributeType(Qt3DRender::QAttribute::VertexAttribute);
        p->setDivisor(1'000'000'000); // just make it huge so it never advances

        return p;
    }

    if (!oref) return nullptr;

    assert(oref.has_value());

    auto const& ref = *oref;

    auto buff = dynamic_cast<ExBuffer*>(ref.buffer);

    if (!buff) return nullptr;

    int count = ref.size / ref.stride;

    // qDebug() << "Attrib has" << count << ref.start << ref.size << ref.stride;

    if (ref.size >= buff->size()) {
        qWarning() << "Reference is out side of buffer bounds!";
        return nullptr;
    }

    // debug_attribute<VertexType>(buff.get(), ref);

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

    //    qDebug() << p->name() << p->count() << p->attributeType()
    //             << p->vertexBaseType() << p->vertexSize() << p->buffer()
    //             << p->byteOffset() << p->byteStride();

    return p;
}

Qt3DRender::QAttribute* instance_attrib(Qt3DRender::QGeometry* node,
                                        Qt3DRender::QBuffer*   array) {
    //    qDebug() << "New Attrib" << typeid(glm::mat4).name() << node;

    if (array->data().isEmpty()) return nullptr;

    int count = array->data().size() / sizeof(glm::mat4);

    //    qDebug() << "Index attrib has" << count;

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

    //    qDebug() << p->name() << p->count() << p->attributeType()
    //             << p->vertexBaseType() << p->vertexSize() << p->buffer()
    //             << p->byteOffset() << p->byteStride();

    return p;
}

Qt3DRender::QAttribute* instance_fake_pos_attrib(Qt3DRender::QGeometry* node,
                                                 Qt3DRender::QBuffer*   array) {
    //    qDebug() << "New Attrib" << typeid(glm::mat4).name() << node;

    if (array->data().isEmpty()) return nullptr;

    int count = array->data().size() / sizeof(glm::mat4);

    //    qDebug() << "Index attrib has" << count;

    auto* p = new Qt3DRender::QAttribute(array,
                                         "instance_positions",
                                         Qt3DRender::QAttribute::Float,
                                         4,
                                         count,
                                         0,
                                         sizeof(glm::mat4),
                                         node);

    p->setAttributeType(Qt3DRender::QAttribute::VertexAttribute);

    //    qDebug() << p->name() << p->count() << p->attributeType()
    //             << p->vertexBaseType() << p->vertexSize() << p->buffer()
    //             << p->byteOffset() << p->byteStride();

    return p;
}


template <class IndexType>
Qt3DRender::QAttribute*
attrib_from_idx_ref(Qt3DRender::QGeometry*                   node,
                    std::optional<nooc::ComponentRef> const& oref) {

    if (!oref) return nullptr;

    auto const& ref = *oref;

    auto buff = dynamic_cast<ExBuffer*>(ref.buffer);

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

    auto* pa =
        attrib_from_ref<glm::vec3>(QAttribute::defaultPositionAttributeName(),
                                   ret.geom,
                                   m_data.positions,
                                   false);

    auto* na =
        attrib_from_ref<glm::vec3>(QAttribute::defaultNormalAttributeName(),
                                   ret.geom,
                                   m_data.normals,
                                   false);

    auto* ta = attrib_from_ref<glm::u16vec2>(
        QAttribute::defaultTextureCoordinateAttributeName(),
        ret.geom,
        m_data.textures,
        true);

    auto* ca = attrib_from_ref<glm::u8vec4>(
        QAttribute::defaultColorAttributeName(), ret.geom, m_data.colors, true);

    QAttribute* idx_p = nullptr;

    if (m_data.triangles) {
        idx_p = attrib_from_idx_ref<glm::u16vec3>(ret.geom, m_data.triangles);

        ret.renderer->setPrimitiveType(QGeometryRenderer::Triangles);

        ret.is_2d = false;

    } else if (m_data.lines) {
        idx_p = attrib_from_idx_ref<glm::u16vec2>(ret.geom, m_data.lines);

        ret.renderer->setPrimitiveType(QGeometryRenderer::Lines);

        ret.is_2d = true;
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

    for (auto const& m : instances) {
        qDebug() << m[1].x << m[1].y << m[1].z << m[1].w;
    }

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
