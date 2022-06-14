#ifndef EXBUFFER_H
#define EXBUFFER_H

#include "componentlistmodel.h"
#include "delegates.h"

#include <noo_client_interface.h>

class QNetworkReply;

class ExBuffer : public nooc::BufferDelegate {
    Q_OBJECT

    //    Qt3DCore::QEntity*            m_scene_root = nullptr;
    //    UniqueQPtr<Qt3DCore::QBuffer> m_3d_entity;

public:
    constexpr static bool CAN_UPDATE = true;
    static QStringList    header();

    ExBuffer(noo::BufferID id, nooc::BufferInit const& md);

    ~ExBuffer();

    // void prepare_delete() override { unregister(); }

    int      get_id() const;
    int      get_id_gen() const;
    QString  get_name() const;
    QVariant get_column(int c) const;

    size_t size() const;

    //    Qt3DCore::QBuffer* entity();

signals:
    void updated();
};

class ExBufferView : public nooc::BufferViewDelegate {
    Q_OBJECT

    //    Qt3DCore::QEntity*            m_scene_root = nullptr;
    //    UniqueQPtr<Qt3DCore::QBuffer> m_3d_entity;

public:
    static QStringList header();

    ExBufferView(noo::BufferViewID id, nooc::BufferViewInit const& md);

    ~ExBufferView();

    // void prepare_delete() override { unregister(); }

    int      get_id() const;
    int      get_id_gen() const;
    QString  get_name() const;
    QVariant get_column(int c) const;

    size_t size() const;

    std::span<char const> data() const;

    //    Qt3DCore::QBuffer* entity();

signals:
    void updated();
};

#endif // EXBUFFER_H
