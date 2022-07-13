#include "exobject.h"

#include "attachedmethodlistmodel.h"
#include "exlight.h"
#include "exmaterial.h"
#include "exmesh.h"
#include "methodcalldialog.h"
#include "variant_tools.h"

#include <noo_common.h>

#include <glm/gtx/matrix_decompose.hpp>

// =============================================================================

EntityChangeNotifier::EntityChangeNotifier(QObject* parent)
    : ChangeNotifierBase(parent) { }

EntityChangeNotifier::~EntityChangeNotifier() {
    qDebug() << "Destroying entity notifier";
}

// =============================================================================

RepresentationPart::RepresentationPart(QObject* p) : QObject(p) { }
RepresentationPart::~RepresentationPart() = default;

QString TextPart::info_string() const {
    return "Text Part";
}

QString WebPart::info_string() const {
    return "Web Part";
}

// =============================================================================

QVector3D convert(glm::vec3 v) {
    return QVector3D(v.x, v.y, v.z);
}
QQuaternion convert_r(glm::vec4 v) {
    return QQuaternion(v.w, v.x, v.y, v.z);
}
QColor convert_c(glm::vec4 c) {
    return QColor::fromRgbF(c.r, c.g, c.b, c.a);
}

QMLInstanceTable::QMLInstanceTable(nooc::InstanceSource const& src) {
    if (!src.view) return;

    if (!src.view->is_data_ready()) {
        connect(src.view,
                &nooc::BufferViewDelegate::data_ready,
                this,
                &QMLInstanceTable::buffer_ready);
    } else {
        buffer_ready(src.view->get_sub_range());
    }
}

QMLInstanceTable::~QMLInstanceTable() {
    qDebug() << Q_FUNC_INFO;
}

QByteArray QMLInstanceTable::getInstanceBuffer(int* instanceCount) {
    qDebug() << "Fetch instance data" << m_ready_instances
             << m_instance_data.size() << instanceCountOverride();

    if (instanceCount) *instanceCount = m_ready_instances;
    return m_instance_data;
}

void QMLInstanceTable::buffer_ready(QByteArray array) {
    auto mat_array = std::span((glm::mat4 const*)array.data(),
                               array.size() / sizeof(glm::mat4));

    m_instance_data.resize(sizeof(InstanceTableEntry) * mat_array.size());

    auto* table_arr = (InstanceTableEntry*)m_instance_data.data();

    for (int i = 0; i < mat_array.size(); i++) {

        auto const& m = mat_array[i];

        qDebug() << "P" << convert(glm::vec3(m[0]));
        qDebug() << "S" << convert(glm::vec3(m[3]));
        qDebug() << "R" << convert_r(m[2]);
        qDebug() << "C" << convert_c(m[1]);

        table_arr[i] = calculateTableEntry(convert(glm::vec3(m[0])),
                                           convert(glm::vec3(m[3])),
                                           convert_r(m[2]).toEulerAngles(),
                                           convert_c(m[1]));
    }

    m_ready_instances = mat_array.size();

    markAllDirty();

    qDebug() << "Created table for" << m_ready_instances << "instances";
}

RenderSubObject::RenderSubObject(EntityChangeNotifier* n,
                                 int32_t               parent_id,
                                 nooc::EntityRenderableDefinition const& def,
                                 ExMeshGeometry&                         geom,
                                 ExObject* cpp_obj)
    : m_notifier(n) {

    qDebug() << Q_FUNC_INFO;

    m_id = n->new_id();

    if (def.instances) {
        m_table = new QMLInstanceTable(def.instances.value());
    }

    int32_t mat_id = -1;

    if (geom.patch_info()->material) {
        auto* m = qobject_cast<ExMaterial*>(geom.patch_info()->material.get());
        if (m) mat_id = m->qt_mat_id();
    }


    emit n->ask_create(m_id, cpp_obj, parent_id, mat_id, &geom, m_table);
}
RenderSubObject::~RenderSubObject() {
    if (m_notifier) {
        emit m_notifier->ask_delete(m_id);
        m_notifier->return_id(m_id);
    }
}

RenderPart::RenderPart(EntityChangeNotifier*                   n,
                       nooc::EntityRenderableDefinition const& def,
                       ExObject*                               parent)
    : RepresentationPart(parent),
      m_notifier(n),
      m_def(def),
      m_parent_exobject(parent) {

    // for each patch, make a sub object (for now)

    qDebug() << "new render part" << parent->get_name() << parent;

    auto mesh_delegate = dynamic_cast<ExMesh*>(def.mesh.get());

    if (!mesh_delegate) return;

    connect(mesh_delegate, &ExMesh::ready, this, &RenderPart::redo_subs);

    if (mesh_delegate->is_complete()) { redo_subs(); }
}

RenderPart::~RenderPart() { }

QString RenderPart::info_string() const {
    return QString("Render: Mesh %1, Inst %2")
        .arg(ptr_to_id(m_mesh.get()))
        .arg(m_instances.size());
}

void RenderPart::redo_subs() {
    qDebug() << "REDO SUBS";

    m_sub_ids.clear();

    auto mesh_delegate = dynamic_cast<ExMesh*>(m_def.mesh.get());

    connect(mesh_delegate, &ExMesh::ready, this, &RenderPart::redo_subs);


    bool pickable =
        !(m_parent_exobject->info().tags.contains(noo::names::tag_user_hidden));

    qDebug() << "redoing render subs" << m_parent_exobject->get_name()
             << m_parent_exobject << pickable;

    for (int i = 0; i < mesh_delegate->qt_geom_count(); i++) {
        auto* geom = mesh_delegate->qt_geom(i);
        m_sub_ids.emplace_back(std::make_unique<RenderSubObject>(
            m_notifier,
            m_parent_exobject->internal_root(),
            m_def,
            *geom,
            pickable ? m_parent_exobject : nullptr));
    }
}

// =============================================================================

void ExObject::rebuild(bool representation, bool methods) {


    /*
    qDebug() << Q_FUNC_INFO << !!md.name << !!md.parent << !!md.transform
             << !!md.material << !!md.mesh << !!md.lights << !!md.tables
             << !!md.instances << !!md.tags << !!md.method_list
             << !!md.signal_list << !!md.text;
             */

    //    if (info().parent) {
    //        qDebug() << "Reparenting";

    //        // set
    //        auto parent = dynamic_cast<ExObject*>(info().parent.get());

    //        emit m_notifier->ask_set_parent(m_root, parent->internal_root());

    //        // qDebug() << this << "setting parent" << m_parent.get();
    //    }


    if (representation) {
        VMATCH(
            info().definition,
            VCASE(std::monostate) { m_attached_part.reset(); },
            VCASE(nooc::EntityTextDefinition const&) {
                m_attached_part.reset();
            },
            VCASE(nooc::EntityWebpageDefinition const&) {
                m_attached_part.reset();
            },
            VCASE(nooc::EntityRenderableDefinition const& def) {
                auto* ptr = new RenderPart(m_notifier, def, this);

                m_attached_part = ptr;
            });
    }


    {
        glm::vec3 scale, translation, skew;
        glm::quat orientation;
        glm::vec4 persp;

        bool ok = glm::decompose(
            info().transform, scale, orientation, translation, skew, persp);

        if (!ok) {
            qCritical() << "Unable to decompose matrix";
            translation = glm::vec3(0);
            scale       = glm::vec3(1);
            orientation = glm::quat(1, glm::vec3(0));
        }

        auto qtranslate =
            QVector3D(translation.x, translation.y, translation.z);

        auto qscale = QVector3D(scale.x, scale.y, scale.z);

        auto qquat = QQuaternion(
            orientation.w, orientation.x, orientation.y, orientation.z);

        emit m_notifier->ask_set_tf(m_root, qtranslate, qquat, qscale);
    }


#if 0

    //    if (md.lights) {
    //        for (auto& ptr : m_lights) {
    //            if (ptr) { m_3d_entity->removeComponent(ptr->entity()); }
    //        }

    //        m_lights = *md.lights;

    //        for (auto& ptr : m_lights) {
    //            if (ptr) { m_3d_entity->addComponent(ptr->entity()); }
    //        }
    //    }

    if (md.tables) { m_tables = *md.tables; }

    m_tags = to_qstringlist(md.tags.value_or(std::vector<std::string_view> {}));
    //    m_method_list =
    //        md.method_list.value_or(std::vector<nooc::MethodDelegatePtr> {});
    //    m_signal_list =
    //        md.signal_list.value_or(std::vector<nooc::SignalDelegatePtr> {});

    if (md.method_list) {
        qDebug() << "update method list" << md.method_list->size();
        m_attached_methods->set(*md.method_list);
    }
#endif

    if (methods) { m_attached_methods->set(info().methods_list); }
}

QStringList ExObject::header() {
    // be careful about changing these. consider the filter below.
    return { "ID",     "Name", "Parent", "Representation",
             "Lights", "Tags", "Methods" };
}

ExObject::ExObject(noo::EntityID           id,
                   nooc::EntityInit const& md,
                   // Qt3DCore::QEntity*            scene_root,
                   EntityChangeNotifier* notifier)
    : nooc::EntityDelegate(id, md), m_notifier(notifier) {

    m_attached_methods = new AttachedMethodListModel(this);


    qDebug() << "New object" << id.id_slot;

    connect(m_attached_methods,
            &AttachedMethodListModel::wishes_to_call,
            this,
            [this](ExMethod* ptr) {
                auto* dialog = new MethodCallDialog(this, ptr);

                dialog->setVisible(true);
            });

    m_root = notifier->new_id();

    ExObject* new_parent = dynamic_cast<ExObject*>(info().parent.get());

    emit m_notifier->ask_create(
        m_root, nullptr, new_parent ? new_parent->internal_root() : -1);
}

ExObject::~ExObject() {
    qDebug() << Q_FUNC_INFO << this;

    if (m_notifier) {
        emit m_notifier->ask_delete(m_root);
        m_notifier->return_id(m_root);
    }
}

int ExObject::get_id() const {
    return this->id().id_slot;
}
int ExObject::get_id_gen() const {
    return this->id().id_gen;
}
QString ExObject::get_name() const {
    return info().name;
}

QVariant ExObject::get_column(int c) const {
    switch (c) {
    case 0: return get_id();
    case 1: return get_name().isEmpty() ? id().to_qstring() : get_name();
    case 2: return ptr_to_id(info().parent);

    case 3: return m_attached_part ? m_attached_part->info_string() : "None";
    case 4: return build_id_list(m_lights.get());

    case 5: return info().tags;
    case 6: return QVariant::fromValue(m_attached_methods);
    }
    return {};
}

void ExObject::on_complete() {
    rebuild(true, true);
}

void ExObject::on_update(nooc::EntityUpdateData const& md) {
    if (md.parent) { qFatal("UNABLE TO REPARENT AT THIS TIME"); }
    rebuild(md.definition.has_value(), md.methods_list.has_value());
}

// Qt3DCore::QEntity* ExObject::entity() {
//     assert(m_3d_entity);
//     return m_3d_entity.data();
// }

// =============================================================================

TaggedNameObjectFilter::TaggedNameObjectFilter(QObject* p)
    : QSortFilterProxyModel(p) { }

static int max_h_size = ExObject::header().size();
static int name_idx   = ExObject::header().indexOf("Name");
static int tag_idx    = ExObject::header().indexOf("Tags");

bool TaggedNameObjectFilter::filterAcceptsRow(
    int                source_row,
    QModelIndex const& source_parent) const {

    qDebug() << Q_FUNC_INFO << source_row;

    // see if this an object table...
    auto* src = sourceModel();

    if (!src) return true;

    if (src->columnCount() < max_h_size) return true;

    auto name = src->data(src->index(source_row, name_idx, source_parent))
                    .value<QString>()
                    .toLower();

    auto tags = src->data(src->index(source_row, tag_idx, source_parent))
                    .value<QStringList>();

    // ick
    static QString hidden_tag = noo::names::tag_user_hidden;

    if (tags.contains(hidden_tag)) {
        qDebug() << Q_FUNC_INFO << "Hidden!";
        return false;
    }

    if (m_names.isEmpty() and m_tags.isEmpty()) {
        qDebug() << Q_FUNC_INFO << "NO FILTER!";
        return true;
    }


    bool accept_name = false;

    for (auto const& n : m_names) {
        if (name.contains(n)) {
            accept_name = true;
            break;
        }
    }

    if (accept_name) return true;

    bool accept_tags = false;

    for (auto const& t : m_tags) {
        for (auto const& ot : tags) {
            if (ot.contains(t)) {
                accept_tags = true;
                break;
            }
        }
    }

    return accept_tags;
}

QString const& TaggedNameObjectFilter::filter() const {
    return m_filter;
}

void TaggedNameObjectFilter::set_filter(QString const& new_filter) {
    qDebug() << Q_FUNC_INFO << new_filter;

    auto l_new_filter = new_filter.toLower();

    if (m_filter == l_new_filter) return;
    m_filter = l_new_filter;

    m_tags.clear();
    m_names.clear();

    auto parts = m_filter.split(":");

    for (auto part : parts) {
        if (part.startsWith("#")) {
            m_tags << part.mid(1);
        } else {
            m_names << part;
        }
    }

    invalidateFilter();

    emit filter_changed();
}
