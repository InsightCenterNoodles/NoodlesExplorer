#include "exmesh.h"

#include "exbuffer.h"

#include <QAttribute>
#include <QEntity>
#include <QGeometry>
#include <QGeometryRenderer>
#include <Qt3DCore/QBuffer>

template <class Function>
void copy_attribute(nooc::Attribute const& ref,
                    size_t                 copy_start_offset,
                    size_t                 vertex_stride,
                    std::span<char>        dest,
                    Function               transformer) {
    auto* buff   = static_cast<ExBufferView*>(ref.view.get());
    auto  source = buff->data();

    if (source.empty()) return;

    char const* source_ptr = source.data() + ref.offset;
    char*       dest_ptr   = dest.data() + copy_start_offset;

    size_t count_elem = source.size_bytes() / ref.stride;

    for (size_t i = 0; i < count_elem; i++) {
        assert(source_ptr < (source.data() + source.size()));
        assert(dest_ptr < (dest.data() + dest.size()));

        transformer(source_ptr, dest_ptr);

        source_ptr += ref.stride;
        dest_ptr += vertex_stride;
    }
}

void vec3_tf(char const* src, char* dest) {
    using T     = glm::vec3;
    *((T*)dest) = *(T*)src;
}

void tex_tf(char const* src, char* dest) {
    using T     = glm::u16vec2;
    *((T*)dest) = *(T*)src;
}

void color_tf(char const* src, char* dest) {
    auto         from = *(glm::u8vec4*)src;
    glm::u16vec4 v(from);
    v *= 65536;
    v /= 255;
    *((glm::u16vec4*)dest) = v;
}

void copy_to(nooc::Attribute const& ref, std::span<char> dest) {
    ExBufferView* buff   = static_cast<ExBufferView*>(ref.view.get());
    auto          source = buff->data();

    char const* source_ptr = source.data() + ref.offset;

    size_t copy_size = std::min(dest.size(), source.size());

    std::memcpy(dest.data(), source_ptr, copy_size);
}

ExMeshGeometry::ExMeshGeometry(nooc::MeshInit const* d) : m_data(d) {
    QByteArray new_buffer;

    // lets do the repack dance

    //    size_t num_vertex = d.positions->size / d.positions->stride;

    //    qDebug() << Q_FUNC_INFO << num_vertex << "verts";

    //    size_t vertex_stride = sizeof(glm::vec3);
    //    if (d.normals) vertex_stride += sizeof(glm::vec3);
    //    if (d.textures) vertex_stride += sizeof(glm::u16vec2);
    //    if (d.colors) vertex_stride += sizeof(glm::u8vec4);

    //    size_t vertex_byte_count = num_vertex * vertex_stride;
    //    size_t prim_byte_count =
    //        (d.lines ? d.lines->size : 0) + (d.triangles ? d.triangles->size :
    //        0);

    //    new_buffer.resize(vertex_byte_count + prim_byte_count);

    //    std::span<char> dest_ref(new_buffer.data(), new_buffer.size());
    //    auto            prim_dest_ref = dest_ref.subspan(vertex_byte_count);

    //    size_t cursor = 0;

    //    copy_attribute(*d.positions, cursor, vertex_stride, dest_ref,
    //    vec3_tf); addAttribute(Attribute::PositionSemantic, cursor,
    //    Attribute::F32Type); cursor += sizeof(glm::vec3);

    //    if (d.normals) {
    //        copy_attribute(*d.normals, cursor, vertex_stride, dest_ref,
    //        vec3_tf); addAttribute(Attribute::NormalSemantic, cursor,
    //        Attribute::F32Type); cursor += sizeof(glm::vec3);
    //    }

    //    if (d.textures) {
    //        copy_attribute(*d.textures, cursor, vertex_stride, dest_ref,
    //        tex_tf); addAttribute(Attribute::TexCoord0Semantic, cursor,
    //        Attribute::U16Type); cursor += sizeof(glm::u16vec2);
    //    }

    //    if (d.colors) {
    //        copy_attribute(*d.colors, cursor, vertex_stride, dest_ref,
    //        color_tf); addAttribute(Attribute::ColorSemantic, cursor,
    //        Attribute::U16Type); cursor += sizeof(glm::u16vec4);
    //    }

    //    if (d.lines) {
    //        copy_to(*d.lines, prim_dest_ref);
    //        setPrimitiveType(PrimitiveType::Lines);
    //    } else if (d.triangles) {
    //        copy_to(*d.triangles, prim_dest_ref);
    //        setPrimitiveType(PrimitiveType::Triangles);
    //    }


    //    setVertexData(new_buffer);
    //    setIndexData(vertex_byte_count, new_buffer);
    //    setStride(vertex_stride);


    update();
}


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
    return { "ID", "Name", "# Patches" };
}

// template <class T>
// struct VertexTypeTrait;

// template <>
// struct VertexTypeTrait<glm::vec2> {
//    static constexpr auto element_type  = Qt3DCore::QAttribute::Float;
//    static constexpr auto element_count = 2;
//    static inline auto    blank         = glm::vec2(1);
//};

// template <>
// struct VertexTypeTrait<glm::vec3> {
//    static constexpr auto element_type  = Qt3DCore::QAttribute::Float;
//    static constexpr auto element_count = 3;
//    static inline auto    blank         = glm::vec3(1);
//};

// template <>
// struct VertexTypeTrait<glm::u8vec4> {
//    static constexpr auto element_type  = Qt3DCore::QAttribute::UnsignedByte;
//    static constexpr auto element_count = 4;
//    static inline auto    blank         = glm::u8vec4(255);
//};

// template <>
// struct VertexTypeTrait<glm::u16vec2> {
//    static constexpr auto element_type  = Qt3DCore::QAttribute::UnsignedShort;
//    static constexpr auto element_count = 2;
//    static inline auto blank =
//    glm::u16vec2(std::numeric_limits<short>::max());
//};

// template <>
// struct VertexTypeTrait<glm::u16vec3> {
//    static constexpr auto element_type  = Qt3DCore::QAttribute::UnsignedShort;
//    static constexpr auto element_count = 3;
//    static inline auto blank =
//    glm::u16vec3(std::numeric_limits<short>::max());
//};

// template <class VertexType>
// void debug_attribute(ExBuffer* data, nooc::ComponentRef const& ref) {
//    qDebug() << "Attribute Values" << typeid(VertexType).name();

//    auto* buffer_start = data->byte_array().data();

//    buffer_start += ref.start;
//    auto buffer_end = buffer_start + ref.size;

//    for (; buffer_start < buffer_end; buffer_start += ref.stride) {
//        auto const& vt = *(VertexType*)buffer_start;
//        qDebug() << vt;
//    }
//}


// template <class VertexType>
// Qt3DCore::QAttribute*
// attrib_from_ref(QString                                  name,
//                Qt3DCore::QGeometry*                     node,
//                std::optional<nooc::ComponentRef> const& oref,
//                bool                                     permit_blank) {
//    qDebug() << "New Attrib" << typeid(VertexType).name() << name << node
//             << (bool)oref;

//    if (!oref and permit_blank) {
//        qDebug() << "Creating blank";
//        // synthesize a blank buffer

//        using Tr = VertexTypeTrait<VertexType>;

//        QByteArray array((const char*)&Tr::blank, sizeof(VertexType));

//        auto* b = new Qt3DCore::QBuffer(node);

//        b->setData(array);

//        auto* p =
//            new Qt3DCore::QAttribute(b,
//                                     name,
//                                     VertexTypeTrait<VertexType>::element_type,
//                                     VertexTypeTrait<VertexType>::element_count,
//                                     1,
//                                     0,
//                                     sizeof(VertexType),
//                                     node);

//        p->setAttributeType(Qt3DCore::QAttribute::VertexAttribute);
//        p->setDivisor(1'000'000'000); // just make it huge so it never
//        advances

//        return p;
//    }

//    if (!oref) return nullptr;

//    assert(oref.has_value());

//    auto const& ref = *oref;

//    auto buff = dynamic_cast<ExBuffer*>(ref.buffer);

//    if (!buff) return nullptr;

//    int count = ref.size / ref.stride;

//    // qDebug() << "Attrib has" << count << ref.start << ref.size <<
//    ref.stride;

//    if (ref.size >= buff->size()) {
//        qWarning() << "Reference is out side of buffer bounds!";
//        return nullptr;
//    }

//    // debug_attribute<VertexType>(buff.get(), ref);

//    auto* p =
//        new Qt3DCore::QAttribute(buff->entity(),
//                                 name,
//                                 VertexTypeTrait<VertexType>::element_type,
//                                 VertexTypeTrait<VertexType>::element_count,
//                                 count,
//                                 ref.start,
//                                 ref.stride,
//                                 node);

//    p->setAttributeType(Qt3DCore::QAttribute::VertexAttribute);

//    //    qDebug() << p->name() << p->count() << p->attributeType()
//    //             << p->vertexBaseType() << p->vertexSize() << p->buffer()
//    //             << p->byteOffset() << p->byteStride();

//    return p;
//}

// Qt3DCore::QAttribute* instance_attrib(Qt3DCore::QGeometry* node,
//                                      Qt3DCore::QBuffer*   array) {
//    //    qDebug() << "New Attrib" << typeid(glm::mat4).name() << node;

//    if (array->data().isEmpty()) return nullptr;

//    int count = array->data().size() / sizeof(glm::mat4);

//    //    qDebug() << "Index attrib has" << count;

//    auto* p = new Qt3DCore::QAttribute(array,
//                                       "raw_instance",
//                                       Qt3DCore::QAttribute::Float,
//                                       16,
//                                       count,
//                                       0,
//                                       0,
//                                       node);

//    p->setAttributeType(Qt3DCore::QAttribute::VertexAttribute);
//    p->setDivisor(1);

//    //    qDebug() << p->name() << p->count() << p->attributeType()
//    //             << p->vertexBaseType() << p->vertexSize() << p->buffer()
//    //             << p->byteOffset() << p->byteStride();

//    return p;
//}

// Qt3DCore::QAttribute* instance_fake_pos_attrib(Qt3DCore::QGeometry* node,
//                                               Qt3DCore::QBuffer*   array) {
//    //    qDebug() << "New Attrib" << typeid(glm::mat4).name() << node;

//    if (array->data().isEmpty()) return nullptr;

//    int count = array->data().size() / sizeof(glm::mat4);

//    //    qDebug() << "Index attrib has" << count;

//    auto* p = new Qt3DCore::QAttribute(array,
//                                       "instance_positions",
//                                       Qt3DCore::QAttribute::Float,
//                                       4,
//                                       count,
//                                       0,
//                                       sizeof(glm::mat4),
//                                       node);

//    p->setAttributeType(Qt3DCore::QAttribute::VertexAttribute);

//    //    qDebug() << p->name() << p->count() << p->attributeType()
//    //             << p->vertexBaseType() << p->vertexSize() << p->buffer()
//    //             << p->byteOffset() << p->byteStride();

//    return p;
//}


// template <class IndexType>
// Qt3DCore::QAttribute*
// attrib_from_idx_ref(Qt3DCore::QGeometry*                     node,
//                    std::optional<nooc::ComponentRef> const& oref) {

//    if (!oref) return nullptr;

//    auto const& ref = *oref;

//    auto buff = dynamic_cast<ExBuffer*>(ref.buffer);

//    if (!buff) return nullptr;

//    int count = ref.size / sizeof(IndexType);

//    qDebug() << Q_FUNC_INFO << ref.size << count;

//    auto* p = new Qt3DCore::QAttribute(node);
//    p->setBuffer(buff->entity());
//    p->setAttributeType(Qt3DCore::QAttribute::IndexAttribute);

//    p->setVertexBaseType(VertexTypeTrait<IndexType>::element_type);
//    // p->setVertexSize(VertexTypeTrait<IndexType>::element_count);
//    p->setCount(VertexTypeTrait<IndexType>::element_count * count);
//    p->setByteOffset(ref.start);

//    qDebug() << p->name() << p->count() << p->attributeType()
//             << p->vertexBaseType() << p->vertexSize() << p->buffer()
//             << p->byteOffset() << p->byteStride();

//    qDebug() << "BF" << buff->entity()->data().size();

//    return p;
//}

static IterationCounter exmesh_object_counter;

ExMesh::ExMesh(noo::GeometryID       id,
               nooc::MeshInit const& md,
               ExMeshChangeNotifier* notifier)
    : nooc::MeshDelegate(id, md) {
    m_geometry.reset(new ExMeshGeometry(&md));

    connect(this,
            &ExMesh::ask_recreate,
            notifier,
            &ExMeshChangeNotifier::ask_recreate);

    auto old    = m_iteration;
    m_iteration = exmesh_object_counter.next();

    {
        for (auto const& patch : info()->patches) {
            for (auto const& attrib : patch->attributes) {
                m_views_waiting += !attrib.view->is_data_ready();
            }

            if (patch->indicies) {
                m_views_waiting += !patch->indicies->view->is_data_ready();
            }
        }
    }

    emit ask_recreate(old, m_iteration, m_geometry);
}

ExMesh::~ExMesh() {
    emit ask_recreate(m_iteration, -1, nullptr);
}

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
        return info()->patches.size();
    }
    }
    return {};
}

QStringList ExMesh::get_sub_info_header() const {
    static const QStringList s = { "Attributes", "Index", "Material" };
    return s;
}
QStringList ExMesh::get_sub_info(int i) {
    if (i < 0 or i >= info()->patches.size()) return {};
    info()->patches.at(i);
    return QStringList() << "TBD";
}

// QtGeomInfo ExMesh::make_new_info(std::span<glm::mat4> instances) {
//    using namespace Qt3DRender;

//    QtGeomInfo ret;

//    // qDebug() << "New geometry" << m_data.extent_min << m_data.extent_max;

//    if (!m_data.positions) return ret;
//    if (!m_data.triangles and !m_data.lines) return ret;

//    // set up geometry
//    ret.geom = new Qt3DCore::QGeometry(m_scene_root);

//    ret.geom->setObjectName("Geom " + get_name());

//    ret.renderer = new Qt3DRender::QGeometryRenderer(m_scene_root);

//    ret.renderer->setObjectName("GeomRenderer " + get_name());

//    auto* pa = attrib_from_ref<glm::vec3>(
//        Qt3DCore::QAttribute::defaultPositionAttributeName(),
//        ret.geom,
//        m_data.positions,
//        false);

//    auto* na = attrib_from_ref<glm::vec3>(
//        Qt3DCore::QAttribute::defaultNormalAttributeName(),
//        ret.geom,
//        m_data.normals,
//        false);

//    auto* ta = attrib_from_ref<glm::u16vec2>(
//        Qt3DCore::QAttribute::defaultTextureCoordinateAttributeName(),
//        ret.geom,
//        m_data.textures,
//        true);

//    auto* ca = attrib_from_ref<glm::u8vec4>(
//        Qt3DCore::QAttribute::defaultColorAttributeName(),
//        ret.geom,
//        m_data.colors,
//        true);

//    Qt3DCore::QAttribute* idx_p = nullptr;

//    if (m_data.triangles) {
//        idx_p = attrib_from_idx_ref<glm::u16vec3>(ret.geom, m_data.triangles);

//        ret.renderer->setPrimitiveType(QGeometryRenderer::Triangles);

//        ret.is_2d = false;

//    } else if (m_data.lines) {
//        idx_p = attrib_from_idx_ref<glm::u16vec2>(ret.geom, m_data.lines);

//        ret.renderer->setPrimitiveType(QGeometryRenderer::Lines);

//        ret.is_2d = true;
//    }


//    if (pa) ret.geom->addAttribute(pa);
//    if (na) ret.geom->addAttribute(na);
//    if (ta) ret.geom->addAttribute(ta);
//    if (ca) ret.geom->addAttribute(ca);
//    if (idx_p) ret.geom->addAttribute(idx_p);

//    static glm::mat4 ident = []() {
//        glm::mat4 id;
//        id[0] = { 0, 0, 0, 1 };
//        id[1] = { 1, 1, 1, 1 };
//        id[2] = { 0, 0, 0, 1 };
//        id[3] = { 1, 1, 1, 1 };
//        return id;
//    }();

//    if (instances.empty()) { instances = std::span(&ident, 1); }

//    for (auto const& m : instances) {
//        qDebug() << m[1].x << m[1].y << m[1].z << m[1].w;
//    }

//    ret.instance_data.resize(instances.size_bytes());

//    std::memcpy(
//        ret.instance_data.data(), instances.data(), instances.size_bytes());

//    ret.instance_buffer = new Qt3DCore::QBuffer(m_scene_root);

//    ret.instance_buffer->setData(ret.instance_data);

//    ret.instance_attribute.reset(
//        instance_attrib(ret.geom, ret.instance_buffer));

//    ret.geom->addAttribute(ret.instance_attribute);

//    // now we need another attribute to get proper cull bounds from instances
//    auto* fake_attrib = instance_fake_pos_attrib(ret.geom,
//    ret.instance_buffer);

//    ret.geom->addAttribute(fake_attrib);
//    ret.geom->setBoundingVolumePositionAttribute(fake_attrib);

//    ret.renderer->setGeometry(ret.geom);

//    if (!instances.empty()) {
//        ret.renderer->setInstanceCount(instances.size());
//    }

//    return ret;
//}
