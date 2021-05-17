#include "exmaterial.h"

#include "instance_material/qinstancedmetalroughmaterial.h"

#include <QEntity>

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
