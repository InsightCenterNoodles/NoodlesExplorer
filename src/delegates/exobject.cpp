#include "exobject.h"

#include "attachedmethodlistmodel.h"
#include "exlight.h"
#include "exmaterial.h"
#include "exmesh.h"
#include "instance_material/qinstancedmetalroughmaterial.h"
#include "methodcalldialog.h"
#include "variant_tools.h"

#include <noo_common.h>

#include <QAbstractLight>
#include <QEntity>
#include <QGeometry>
#include <QGeometryRenderer>
#include <QMaterial>
#include <Qt3DCore/QTransform>
#include <Qt3DRender/QFrontFace>

RepresentationPart::RepresentationPart(QObject* p) : QObject(p) { }
RepresentationPart::~RepresentationPart() = default;

QString TextPart::info_string() const {
    return "Text Part";
}

QString WebPart::info_string() const {
    return "Web Part";
}

void RenderPart::remake_mesh_attachment() {
    qDebug() << Q_FUNC_INFO << "START" << !!m_mesh << !!m_material;
    // if (!m_mesh or !m_material) return;

    if (m_att_mesh_details) {
        m_3d_entity->removeComponent(m_att_mesh_details->renderer);
    }

    if (m_material) {
        m_3d_entity->removeComponent(m_material->get_3d_material());
    }

    if (!m_mesh or !m_material) return;

    m_att_mesh_details.emplace(m_mesh->make_new_info(m_instances));

    if (m_att_mesh_details) {
        m_3d_entity->addComponent(m_att_mesh_details->renderer);
    }

    if (m_material) {
        m_3d_entity->addComponent(m_material->get_3d_material());
    }

    qDebug() << Q_FUNC_INFO << "END" << !!m_att_mesh_details << !!m_material;
}

RenderPart::RenderPart(Qt3DCore::QEntity*         p_entity,
                       ExMaterial*                ma,
                       ExMesh*                    me,
                       std::span<glm::mat4 const> inst,
                       QObject*                   parent)
    : RepresentationPart(parent), m_instances(inst.begin(), inst.end()) {

    m_3d_entity = new Qt3DCore::QEntity();
    m_3d_entity->setParent(p_entity);

    qDebug() << Q_FUNC_INFO << p_entity << m_3d_entity.data();

    m_material.set(ma);
    m_mesh.set(me);

    connect(&m_material,
            &AttachmentBase::attachment_changed,
            this,
            &RenderPart::material_changed);

    connect(&m_material,
            &AttachmentBase::attachment_updated,
            this,
            &RenderPart::material_changed);

    connect(&m_mesh,
            &AttachmentBase::attachment_changed,
            this,
            &RenderPart::mesh_changed);

    connect(&m_mesh,
            &AttachmentBase::attachment_updated,
            this,
            &RenderPart::mesh_changed);


    remake_mesh_attachment();
}

RenderPart::~RenderPart() { }

QString RenderPart::info_string() const {
    return QString("Render: Mesh %1, Mat %2, Inst %3")
        .arg(ptr_to_id(m_material.get()))
        .arg(ptr_to_id(m_mesh.get()))
        .arg(m_instances.size());
}

void RenderPart::material_changed() {
    remake_mesh_attachment();
}

void RenderPart::mesh_changed() {
    remake_mesh_attachment();
}

// =============================================================================

void ExObject::update_from(nooc::ObjectUpdateData const& md) {
    /*
    qDebug() << Q_FUNC_INFO << !!md.name << !!md.parent << !!md.transform
             << !!md.material << !!md.mesh << !!md.lights << !!md.tables
             << !!md.instances << !!md.tags << !!md.method_list
             << !!md.signal_list << !!md.text;
             */


    if (md.name) {
        m_name = noo::to_qstring(*md.name);
        if (m_name.isEmpty()) { m_name = id().to_qstring(); }
    }

    if (md.parent) {
        // qDebug() << "Reparenting";

        // set
        m_parent = dynamic_cast<ExObject*>(*md.parent);

        // set up the parent for the 3d scene
        m_3d_entity->setParent(m_parent ? m_parent->entity() : m_3d_root);

        // qDebug() << this << "setting parent" << m_parent.get();
    }

    if (md.transform) {
        m_transform = *md.transform;

        auto tf = QMatrix4x4(glm::value_ptr(m_transform));


        m_3d_transform->setMatrix(tf.transposed());
    }

    if (md.definition) {
        if (m_attached_part) {
            delete m_attached_part;
            m_attached_part = {};
        }

        VMATCH(
            *md.definition,
            VCASE(std::monostate) {},
            VCASE(nooc::ObjectTextDefinition const&) {},
            VCASE(nooc::ObjectWebpageDefinition const&) {},
            VCASE(nooc::ObjectRenderableDefinition const& def) {
                auto new_material = dynamic_cast<ExMaterial*>(def.material);
                auto new_mesh     = dynamic_cast<ExMesh*>(def.mesh);

                m_attached_part = new RenderPart(
                    m_3d_entity, new_material, new_mesh, def.instances, this);
            });
    }

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
}

QStringList ExObject::header() {
    // be careful about changing these. consider the filter below.
    return { "ID",     "Name", "Parent", "Representation",
             "Lights", "Tags", "Methods" };
}

ExObject::ExObject(noo::ObjectID                 id,
                   nooc::ObjectUpdateData const& md,
                   Qt3DCore::QEntity*            scene_root)
    : nooc::ObjectDelegate(id, md), m_3d_root(scene_root) {

    m_3d_entity    = new Qt3DCore::QEntity(scene_root);
    m_3d_transform = new Qt3DCore::QTransform(m_3d_entity.data());

    m_attached_methods = new AttachedMethodListModel(this);

    m_3d_entity->addComponent(m_3d_transform);

    m_name = id.to_qstring();


    // temp add to scene root
    m_3d_entity->setParent(scene_root);
    m_3d_entity->setObjectName(m_name);

    qDebug() << "New object" << id.id_slot;

    connect(m_attached_methods,
            &AttachedMethodListModel::wishes_to_call,
            [this](ExMethod* ptr) {
                auto* dialog = new MethodCallDialog(this, ptr);

                dialog->setVisible(true);
            });

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

    case 3: return m_attached_part ? m_attached_part->info_string() : "None";
    case 4: return build_id_list(m_lights.get());

    case 5: return m_tags;
    case 6: return QVariant::fromValue(m_attached_methods);
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
                    .value<QString>();

    auto tags = src->data(src->index(source_row, tag_idx, source_parent))
                    .value<QStringList>();

    // ick
    static QString hidden_tag =
        QString::fromStdString(std::string(noo::names::tag_noo_user_hidden));

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

    if (m_filter == new_filter) return;
    m_filter = new_filter;

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
