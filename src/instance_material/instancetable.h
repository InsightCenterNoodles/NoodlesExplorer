#ifndef INSTANCETABLE_H
#define INSTANCETABLE_H

#include <noo_include_glm.h>

#include <QtQuick3D/QQuick3DInstancing>

class InstanceTable : public QQuick3DInstancing {
    Q_OBJECT

    bool                                          m_dirty = true;
    std::shared_ptr<std::vector<glm::mat4> const> m_source;
    size_t                                        m_num_instances = 0;
    QByteArray                                    m_instance_data;

    void rebuild();

protected:
    QByteArray getInstanceBuffer(int* instanceCount) override;

public:
    InstanceTable(QQuick3DObject* parent = nullptr);
    ~InstanceTable() override;

    void set_from_list(std::shared_ptr<std::vector<glm::mat4>>);
};

#endif // INSTANCETABLE_H
