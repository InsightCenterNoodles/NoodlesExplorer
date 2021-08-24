#ifndef EXBUFFER_H
#define EXBUFFER_H

#include "componentlistmodel.h"
#include "delegates.h"

#include <noo_client_interface.h>

class QNetworkReply;

class ExBuffer : public nooc::BufferDelegate, public ComponentListItem {
    Q_OBJECT

    QByteArray m_array;

    bool m_ready = false;

    Qt3DCore::QEntity*              m_scene_root = nullptr;
    UniqueQPtr<Qt3DRender::QBuffer> m_3d_entity;

public:
    static QStringList header();

    ExBuffer(noo::BufferID                       id,
             nooc::BufferData const&             md,
             std::shared_ptr<ComponentListModel> list,
             Qt3DCore::QEntity*                  scene_root);

    ~ExBuffer();

    // void prepare_delete() override { unregister(); }

    int      get_id() const override;
    int      get_id_gen() const override;
    QString  get_name() const override;
    QVariant get_column(int c) const override;

    size_t            size() const;
    QByteArray const& byte_array() const { return m_array; }

    Qt3DRender::QBuffer* entity();


public slots:
    void url_fetched(QNetworkReply*);
};

#endif // EXBUFFER_H
