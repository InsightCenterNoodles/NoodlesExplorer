#ifndef EXMESH_H
#define EXMESH_H

#include "componentlistmodel.h"
#include "delegates.h"

#include <noo_client_interface.h>

#include <QQuick3DGeometry>

// struct QtGeomInfo {
//    UniqueQPtr<Qt3DCore::QGeometry>           geom;
//    UniqueQPtr<Qt3DRender::QGeometryRenderer> renderer;

//    QByteArray                       instance_data;
//    UniqueQPtr<Qt3DCore::QBuffer>    instance_buffer;
//    UniqueQPtr<Qt3DCore::QAttribute> instance_attribute;

//    bool is_2d = false;

//    QtGeomInfo();
//    ~QtGeomInfo();

//    QtGeomInfo(QtGeomInfo&&);
//    QtGeomInfo& operator=(QtGeomInfo&&);
//};

class ExMeshChangeNotifier;

class ExMeshGeometry : public QQuick3DGeometry {
    Q_OBJECT
    QPointer<nooc::MeshPatch const> m_data;

public:
    ExMeshGeometry(nooc::MeshPatch const*);

public slots:
    void update_data();
};

class ExMesh : public nooc::MeshDelegate {
    Q_OBJECT

    // nooc::MeshData             m_data;
    size_t                                  m_iteration = -1;
    std::vector<UniqueQPtr<ExMeshGeometry>> m_geometry;

    size_t m_views_waiting = 0;

public:
    static QStringList header();

    ExMesh(noo::GeometryID       id,
           nooc::MeshInit const& md,
           ExMeshChangeNotifier* notifier);

    ~ExMesh();

    // void prepare_delete() override { unregister(); }

    int      get_id() const;
    int      get_id_gen() const;
    QString  get_name() const;
    QVariant get_column(int c) const;

    QStringList get_sub_info_header() const;
    QStringList get_sub_info(int);

    ExMeshGeometry* qt_geom(int i) const { return m_geometry.at(i); }
    int             qt_geom_count() const { return m_geometry.size(); }

    void on_complete() override;

signals:
    void updated();
};

// =============================================================================

class ExMeshChangeNotifier : public QObject {
    Q_OBJECT

public:
    explicit ExMeshChangeNotifier(QObject* parent = nullptr)
        : QObject(parent) { }
    ~ExMeshChangeNotifier() = default;

signals:
    void ask_recreate(int64_t old_id, int64_t new_id, ExMeshGeometry* data);
};


#endif // EXMESH_H
