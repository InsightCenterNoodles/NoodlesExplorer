#include "instancetable.h"

void InstanceTable::rebuild() {
    m_instance_data.resize(0);
    m_num_instances = 0;

    if (!m_source) return;

    for (auto const& mat : *m_source) {

        QColor c = QColor::fromRgbF(mat[1].r, mat[1].g, mat[1].b, mat[1].a);

        auto entry = calculateTableEntryFromQuaternion(
            { mat[0].x, mat[0].y, mat[0].z },
            { mat[3].x, mat[3].y, mat[3].z },
            QQuaternion(mat[2].w, mat[2].x, mat[2].y, mat[2].z),
            c);
        m_instance_data.append(reinterpret_cast<const char*>(&entry),
                               sizeof(entry));

        m_num_instances++;
    }

    m_dirty = false;
}

QByteArray InstanceTable::getInstanceBuffer(int* instanceCount) {
    if (m_dirty) { rebuild(); }

    if (instanceCount) *instanceCount = m_num_instances;
    return m_instance_data;
}

InstanceTable::InstanceTable(QQuick3DObject* parent)
    : QQuick3DInstancing(parent) { }

InstanceTable::~InstanceTable() = default;

void InstanceTable::set_from_list(std::shared_ptr<std::vector<glm::mat4>> s) {
    m_source = s;
    m_dirty  = true;
}
