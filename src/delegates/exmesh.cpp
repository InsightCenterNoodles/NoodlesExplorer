#include "exmesh.h"

#include "exbuffer.h"

#include <noo_common.h>

#include <QAttribute>
#include <QEntity>
#include <QGeometry>
#include <QGeometryRenderer>
#include <Qt3DCore/QBuffer>

template <class Function>
void copy_attribute(std::span<char const> src,
                    size_t                src_stride,
                    char*                 dest,
                    size_t                dest_stride,
                    size_t                vertex_count,
                    Function              transformer) {

    qDebug() << Q_FUNC_INFO;
    qDebug() << (void*)src.data() << src_stride << (void*)dest << dest_stride;

    if (src.empty()) return;

    char const* source_ptr = src.data();

    for (size_t i = 0; i < vertex_count; i++) {
        assert(source_ptr < (src.data() + src.size()));

        transformer(source_ptr, dest);

        source_ptr += src_stride;
        dest += dest_stride;
    }
}


template <class Function>
void compute_min_max(QVector3D&            min_p,
                     QVector3D&            max_p,
                     std::span<char const> src,
                     size_t                src_stride,
                     size_t                vertex_count,
                     Function              transformer) {

    if (src.empty()) return;

    char const* source_ptr = src.data();

    glm::vec3 lmin(std::numeric_limits<float>::max());
    glm::vec3 lmax(std::numeric_limits<float>::lowest());

    for (size_t i = 0; i < vertex_count; i++) {
        assert(source_ptr < (src.data() + src.size()));

        glm::vec4 dest;

        transformer(source_ptr, (char*)&dest);

        source_ptr += src_stride;

        lmin = glm::min(lmin, glm::vec3(dest));
        lmax = glm::max(lmax, glm::vec3(dest));
    }

    min_p = QVector3D(lmin.x, lmin.y, lmin.z);
    max_p = QVector3D(lmax.x, lmax.y, lmax.z);
}


void tf_position(char const* source_ptr, char* dest_ptr) {
    // we are assuming VEC3
    *((glm::vec3*)dest_ptr) = *((glm::vec3 const*)source_ptr);
}

void tf_texture_vec2(char const* source_ptr, char* dest_ptr) {
    *((glm::vec2*)dest_ptr) = *((glm::vec2 const*)source_ptr);
}

void tf_texture_u16vec2(char const* source_ptr, char* dest_ptr) {
    glm::vec2 v = *((glm::u16vec2 const*)source_ptr);
    v /= std::numeric_limits<uint16_t>::max();
    *((glm::vec2*)dest_ptr) = v;
}

void tf_color_u8vec4(char const* source_ptr, char* dest_ptr) {
    glm::vec4 v = *((glm::u8vec4 const*)source_ptr);
    v /= std::numeric_limits<uint8_t>::max();
    *((glm::vec4*)dest_ptr) = v;
}

void tf_color_vec4(char const* source_ptr, char* dest_ptr) {
    *((glm::vec4*)dest_ptr) = *((glm::vec4 const*)source_ptr);
}

void tf_index_u8(char const* source_ptr, char* dest_ptr) {
    *((glm::u32*)dest_ptr) = *((glm::u8 const*)source_ptr);
}
void tf_index_u16(char const* source_ptr, char* dest_ptr) {
    *((glm::u32*)dest_ptr) = *((glm::u16 const*)source_ptr);
}
void tf_index_u32(char const* source_ptr, char* dest_ptr) {
    *((glm::u32*)dest_ptr) = *((glm::u32 const*)source_ptr);
}

void copy_to(nooc::Attribute const& ref, std::span<char> dest) {
    ExBufferView* buff   = static_cast<ExBufferView*>(ref.view.get());
    auto          source = buff->data();

    char const* source_ptr = source.data() + ref.offset;

    size_t copy_size = std::min(dest.size(), source.size());

    std::memcpy(dest.data(), source_ptr, copy_size);
}

ExMeshGeometry::ExMeshGeometry(nooc::MeshPatch const* d) : m_data(d) {
    if (m_data->is_ready()) {
        update_data();
    } else {
        connect(m_data,
                &nooc::MeshPatch::ready,
                this,
                &ExMeshGeometry::update_data);
    }
}

size_t format_byte_size(noo::Format f) {
    switch (f) {
    case noo::Format::U8: return 1;
    case noo::Format::U16: return 2;
    case noo::Format::U32: return 4;
    case noo::Format::U8VEC4: return 4;
    case noo::Format::U16VEC2: return 4;
    case noo::Format::VEC2: return 4 * 2;
    case noo::Format::VEC3: return 4 * 3;
    case noo::Format::VEC4: return 4 * 4;
    case noo::Format::MAT3: return 4 * 9;
    case noo::Format::MAT4: return 4 * 16;
    }
}

size_t format_dest_byte_size(noo::Format f) {
    switch (f) {
    case noo::Format::U8: return 4;
    case noo::Format::U16: return 4;
    case noo::Format::U32: return 4;
    case noo::Format::U8VEC4: return 4 * 4;
    case noo::Format::U16VEC2: return 4 * 2;
    case noo::Format::VEC2: return 4 * 2;
    case noo::Format::VEC3: return 4 * 3;
    case noo::Format::VEC4: return 4 * 4;
    case noo::Format::MAT3: return 4 * 9;
    case noo::Format::MAT4: return 4 * 16;
    }
}

auto convert_attrib(noo::AttributeSemantic a) {
    switch (a) {
    case noo::AttributeSemantic::POSITION:
        return QQuick3DGeometry::Attribute::PositionSemantic;
    case noo::AttributeSemantic::NORMAL:
        return QQuick3DGeometry::Attribute::NormalSemantic;
    case noo::AttributeSemantic::TANGENT:
        return QQuick3DGeometry::Attribute::TangentSemantic;
    case noo::AttributeSemantic::TEXTURE:
        return QQuick3DGeometry::Attribute::TexCoordSemantic;
    case noo::AttributeSemantic::COLOR:
        return QQuick3DGeometry::Attribute::ColorSemantic;
    }
}

auto convert_prim_type(noo::PrimitiveType p) {
    switch (p) {
    case noo::PrimitiveType::POINTS:
        return QQuick3DGeometry::PrimitiveType::Points;
    case noo::PrimitiveType::LINES:
        return QQuick3DGeometry::PrimitiveType::Lines;
    case noo::PrimitiveType::LINE_LOOP:
        qFatal("Line loops not yet implemented");
    case noo::PrimitiveType::LINE_STRIP:
        return QQuick3DGeometry::PrimitiveType::LineStrip;
    case noo::PrimitiveType::TRIANGLES:
        return QQuick3DGeometry::PrimitiveType::Triangles;
    case noo::PrimitiveType::TRIANGLE_STRIP:
        return QQuick3DGeometry::PrimitiveType::TriangleStrip;
    case noo::PrimitiveType::TRIANGLE_FAN:
        return QQuick3DGeometry::PrimitiveType::TriangleFan;
    }
}

void ExMeshGeometry::update_data() {
    clear();

    // at the moment we unconditionally recreate a buffer. this is silly,
    // but makes life simple. later we can optimze the hell out of this

    size_t       dest_vertex_stride = 0;
    size_t const num_vertex         = m_data->count;
    {
        for (auto const& attrib : m_data->attributes) {
            dest_vertex_stride += format_dest_byte_size(attrib.format);
        }
    }
    size_t vertex_total_bytes = num_vertex * dest_vertex_stride;

    size_t num_index = 0;
    if (m_data->indices) {
        auto& ind = *m_data->indices;

        num_index = ind.count;
    }

    // allocate bytes
    auto new_bytes =
        QByteArray(vertex_total_bytes, Qt::Initialization::Uninitialized);

    qDebug() << "Num v/f" << num_vertex << num_index;

    qDebug() << "Vertex stride" << dest_vertex_stride;

    qDebug() << "Allocated new geom buffer" << new_bytes.size()
             << "vertex bytes" << vertex_total_bytes;

    // copy bytes

    char* cursor = new_bytes.data();

    QVector3D min_p, max_p;

    for (auto const& attrib : m_data->attributes) {

        qDebug() << "Copying attribute" << (int)attrib.semantic;

        auto* src_buff = attrib.view->source_buffer();

        auto& view_info = attrib.view->info();

        auto src_span = noo::safe_subspan(std::span(src_buff->data()),
                                          view_info.offset + attrib.offset,
                                          view_info.length);

        size_t bytes_to_copy = format_dest_byte_size(attrib.format);

        switch (attrib.semantic) {
        case noo::AttributeSemantic::POSITION:
            copy_attribute(src_span,
                           attrib.stride,
                           cursor,
                           dest_vertex_stride,
                           num_vertex,
                           tf_position);

            compute_min_max(
                min_p, max_p, src_span, attrib.stride, num_vertex, tf_position);

            break;
        case noo::AttributeSemantic::NORMAL:
            copy_attribute(src_span,
                           attrib.stride,
                           cursor,
                           dest_vertex_stride,
                           num_vertex,
                           tf_position);
            break;
        case noo::AttributeSemantic::TANGENT:
            copy_attribute(src_span,
                           attrib.stride,
                           cursor,
                           dest_vertex_stride,
                           num_vertex,
                           tf_position);
            break;
        case noo::AttributeSemantic::TEXTURE:
            if (attrib.format == noo::Format::VEC2) {
                copy_attribute(src_span,
                               attrib.stride,
                               cursor,
                               dest_vertex_stride,
                               num_vertex,
                               tf_texture_vec2);
            } else if (attrib.format == noo::Format::U16VEC2 and
                       attrib.normalized) {
                copy_attribute(src_span,
                               attrib.stride,
                               cursor,
                               dest_vertex_stride,
                               num_vertex,
                               tf_texture_u16vec2);
            }
            break;
        case noo::AttributeSemantic::COLOR:
            if (attrib.format == noo::Format::VEC4) {
                copy_attribute(src_span,
                               attrib.stride,
                               cursor,
                               dest_vertex_stride,
                               num_vertex,
                               tf_color_vec4);
            } else if (attrib.format == noo::Format::U8VEC4 and
                       attrib.normalized) {
                copy_attribute(src_span,
                               attrib.stride,
                               cursor,
                               dest_vertex_stride,
                               num_vertex,
                               tf_color_u8vec4);
            }
            break;
        }

        addAttribute(convert_attrib(attrib.semantic),
                     cursor - new_bytes.data(),
                     QQuick3DGeometry::Attribute::ComponentType::F32Type);

        cursor += bytes_to_copy;
    }

    qDebug() << "Check copy " << num_index << "indices";

    // copy index, if it exists
    if (num_index) {
        QByteArray new_index_bytes =
            QByteArray(4 * num_index, Qt::Initialization::Uninitialized);

        cursor = new_index_bytes.data();

        auto* view = m_data->indices->view.get();

        auto& view_info = view->info();

        auto* src_buff = view->source_buffer();

        auto span =
            noo::safe_subspan(std::span(src_buff->data()),
                              view_info.offset + m_data->indices->offset,
                              view_info.length);

        size_t bytes_to_copy = format_byte_size(m_data->indices->format);

        switch (m_data->indices->format) {
        case noo::Format::U8:
            copy_attribute(
                span, bytes_to_copy, cursor, 4, num_index, tf_index_u8);
            break;
        case noo::Format::U16:
            copy_attribute(
                span, bytes_to_copy, cursor, 4, num_index, tf_index_u16);
            break;
        case noo::Format::U32:
            copy_attribute(
                span, bytes_to_copy, cursor, 4, num_index, tf_index_u32);
            break;
        default: break;
        }
        addAttribute(Attribute::Semantic::IndexSemantic,
                     0,
                     QQuick3DGeometry::Attribute::ComponentType::U32Type);

        setIndexData(new_index_bytes);
    }

    setVertexData(new_bytes);
    setStride(dest_vertex_stride);

    setPrimitiveType(convert_prim_type(m_data->type));

    setBounds(min_p, max_p);

    qDebug() << "PRIM TYPE" << (int)m_data->type << (int)primitiveType();

    update();
}

// =============================================================================


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


ExMesh::ExMesh(noo::GeometryID       id,
               nooc::MeshInit const& md,
               ExMeshChangeNotifier* notifier)
    : nooc::MeshDelegate(id, md) {

    // emit ask_recreate(old, m_iteration, m_geometry);
}

ExMesh::~ExMesh() {
    // emit ask_recreate(m_iteration, -1, nullptr);
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

void ExMesh::on_complete() {
    qDebug() << "EXMESH complete!";
    m_geometry.clear();
    for (auto const& p : info()->patches) {
        qDebug() << "NEW EXMESHGEOM";
        m_geometry.push_back(new ExMeshGeometry(p));
    }

    emit updated();
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
