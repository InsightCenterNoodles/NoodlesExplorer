#include "exlight.h"

#include <QDirectionalLight>
#include <QEntity>
#include <QPointLight>

QStringList ExLight::header() {
    return { "ID", "Name", "Type", "Color", "Intensity" };
}

static QColor get_color(std::optional<glm::vec3> const& c) {
    auto vec = c.value_or(glm::vec3(1));

    return QColor::fromRgbF(vec.r, vec.g, vec.b);
}

ExLight::ExLight(noo::LightID           id,
                 nooc::LightData const& md,
                 Qt3DCore::QEntity*     scene_root)
    : nooc::LightDelegate(id, md), m_data(md) {

    qDebug() << "CREATE LIGHT" << id.to_qstring();
#if 0

    switch (md.type.value_or(nooc::LightType::POINT)) {
    case nooc::LightType::POINT: {
        auto* p = new Qt3DRender::QPointLight(scene_root);

        p->setColor(get_color(md.color));
        p->setIntensity(md.intensity.value_or(0));

        m_3d_entity.reset(p);
        qDebug() << "HERE" << __LINE__;
    } break;
    case nooc::LightType::SUN: {
        auto* p = new Qt3DRender::QDirectionalLight(scene_root);

        p->setColor(get_color(md.color));
        p->setIntensity(md.intensity.value_or(0));

        glm::vec3 dir = md.spatial.value_or(glm::vec4());

        p->setWorldDirection({ dir.x, dir.y, dir.z });

        m_3d_entity.reset(p);
        qDebug() << "HERE" << __LINE__;
    } break;
    }

    qDebug() << "HERE" << __LINE__;

    assert(m_3d_entity->isShareable());

    m_3d_entity->setColor(QColor("red"));
    m_3d_entity->setIntensity(100);
#endif
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
    case 2: return (int)m_data.type.value_or(nooc::LightType::POINT);
    case 3: return get_color(m_data.color);
    case 4: return m_data.intensity.value_or(0);
    }
    return {};
}

void ExLight::on_update(nooc::LightData const& md) {
    m_data = md;
}

Qt3DRender::QAbstractLight* ExLight::entity() {
    return m_3d_entity;
}
