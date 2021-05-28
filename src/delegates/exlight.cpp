#include "exlight.h"

#include <QEntity>
#include <QPointLight>

QStringList ExLight::header() {
    return { "ID", "Name", "Color", "Intensity" };
}

ExLight::ExLight(noo::LightID                        id,
                 nooc::LightData const&              md,
                 std::shared_ptr<ComponentListModel> list,
                 Qt3DCore::QEntity*                  scene_root)
    : nooc::LightDelegate(id, md), ComponentListItem(list), m_data(md) {

    auto* p = new Qt3DRender::QPointLight(scene_root);

    auto c = QColor::fromRgbF(md.color.r, md.color.g, md.color.b);

    qDebug() << Q_FUNC_INFO << id.to_qstring() << c;

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
