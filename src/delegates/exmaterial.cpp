#include "exmaterial.h"

#include "instance_material/qinstancedmetalroughmaterial.h"

#include <QEntity>

QStringList ExMaterial::header() {
    return { "ID", "Name", "PBR", "Light", "Misc" };
}

ExMaterial::ExMaterial(noo::MaterialID id, nooc::MaterialInit const& md)
    : nooc::MaterialDelegate(id, md) {

    // m_3d_entity->setObjectName(get_name());
    // m_2d_material->setObjectName(get_name());

    // on_update(md);
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

static QString ptr_to_id(std::optional<nooc::TextureRef> const& c) {
    if (!c) return "None";
    return ptr_to_id(c->texture);
}

static auto get_pbr(nooc::PBRInfo const& md) {
    return QString("%1 %2 %3 %4 %5 %6")
        .arg(md.base_color.name())
        .arg(ptr_to_id(md.base_color_texture))
        .arg(md.metallic)
        .arg(md.roughness)
        .arg(ptr_to_id(md.metal_rough_texture));
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
