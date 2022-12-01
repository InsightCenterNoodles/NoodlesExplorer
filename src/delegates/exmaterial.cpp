#include "exmaterial.h"

#include "extexture.h"

#include <QEntity>

MaterialChangeNotifier::MaterialChangeNotifier(QObject* parent)
    : ChangeNotifierBase(parent) { }
MaterialChangeNotifier::~MaterialChangeNotifier() { }


// =============================================================================

QStringList ExMaterial::header() {
    return { "ID", "Name", "PBR", "Light", "Misc" };
}

ExMaterial::ExMaterial(noo::MaterialID           id,
                       nooc::MaterialInit const& md,
                       MaterialChangeNotifier*   notifier)
    : nooc::MaterialDelegate(id, md), m_notifier(notifier) {

    m_qt_mat_id = notifier->new_id();
}

ExMaterial::~ExMaterial() {
    if (m_notifier) {
        emit m_notifier->ask_delete(m_qt_mat_id);
        m_notifier->return_id(m_qt_mat_id);
    }
}

int ExMaterial::get_id() const {
    return this->id().id_slot;
}
int ExMaterial::get_id_gen() const {
    return this->id().id_gen;
}
QString ExMaterial::get_name() const {
    return id().to_qstring();
}

static QString ptr_to_id(std::optional<nooc::TextureRef> const& c) {
    if (!c) return "None";
    return ptr_to_id(c->texture);
}

static auto get_pbr(std::optional<nooc::PBRInfo> const& md) {

    QColor  base_color = Qt::white;
    QString base_color_tex;
    float   metallic  = 0;
    float   roughness = 1;
    QString rough_tex;

    if (md) {
        base_color     = md->base_color;
        base_color_tex = ptr_to_id(md->base_color_texture);
        metallic       = md->metallic;
        roughness      = md->roughness;
        rough_tex      = ptr_to_id(md->metal_rough_texture);
    }

    return QString("%1 %2 %3 %4 %5")
        .arg(base_color.name())
        .arg(base_color_tex)
        .arg(metallic)
        .arg(roughness)
        .arg(rough_tex);
}

static QString get_light(nooc::MaterialInit const& d) {
    return QString("%1 %2 %3")
        .arg(ptr_to_id(d.normal_texture))
        .arg(ptr_to_id(d.emissive_texture))
        .arg(ptr_to_id(d.occlusion_texture));
}

static QString get_misc(nooc::MaterialInit const& d) {
    return QString("%1 %2 %3")
        .arg(d.use_alpha)
        .arg(d.alpha_cutoff)
        .arg(d.double_sided);
}

QVariant ExMaterial::get_column(int c) const {
    switch (c) {
    case 0: return get_id();
    case 1: return get_name();
    case 2: return get_pbr(info().pbr_info);
    case 3: return get_light(info());
    case 4: return get_misc(info());
    }
    return {};
}

void ExMaterial::on_update(nooc::MaterialUpdate const& md) {
    //    // qDebug() << Q_FUNC_INFO << md.color.r << md.color.g << md.color.b
    //    //         << md.color.a;
    //    m_data = md;

    //    auto c = get_color(m_data.color);

    //    m_3d_entity->setBaseColor(c);
    //    m_3d_entity->setMetalness(md.metallic.value_or(0));
    //    m_3d_entity->setRoughness(md.roughness.value_or(0));

    //    m_2d_material->setDiffuse(c);
    //    // m_2d_material->setMetalness(md.metallic);
    //    m_2d_material->setShininess(1.0 - md.roughness.value_or(0));
}

// Qt3DExtras::QPhongMaterial* ExMaterial::get_2d_material() {
//    return m_2d_material;
//}

// QInstancedMetalRoughMaterial* ExMaterial::get_3d_material() {
//     return m_3d_entity;
// }

void ExMaterial::on_complete() {
    if (m_notifier and m_qt_mat_id >= 0) {
        emit m_notifier->ask_delete(m_qt_mat_id);
        m_notifier->return_id(m_qt_mat_id);
    }

    QColor base_color = Qt::white;
    float  metallic   = 0;
    float  roughness  = 1;

    int32_t tex_id = -1;

    if (info().pbr_info) {
        base_color = info().pbr_info->base_color;
        metallic   = info().pbr_info->metallic;
        roughness  = info().pbr_info->roughness;
    }

    if (info().pbr_info->base_color_texture) {
        auto* p = dynamic_cast<ExTexture*>(
            info().pbr_info->base_color_texture->texture.get());

        if (p) { tex_id = p->qt_tex_id(); }
    }

    emit m_notifier->ask_create(m_qt_mat_id,
                                base_color,
                                tex_id,
                                metallic,
                                roughness,
                                info().double_sided);
}
