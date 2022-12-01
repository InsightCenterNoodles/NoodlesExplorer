#ifndef EXTEXTURE_H
#define EXTEXTURE_H

#include "componentlistmodel.h"
#include "delegates.h"

#include <noo_client_interface.h>

#include <QQuick3DTextureData>

class TextureChangeNotifier : public ChangeNotifierBase {
    Q_OBJECT

public:
    explicit TextureChangeNotifier(QObject* parent = nullptr);
    ~TextureChangeNotifier();

signals:
    void ask_delete(int32_t);
    void
    ask_create(int32_t new_id, QQuick3DTextureData* data, QVariantMap extra);
};


class ExTexture : public nooc::TextureDelegate {
    Q_OBJECT

    QPointer<TextureChangeNotifier> m_notifier;

    QImage m_texture_image;

    UniqueQPtr<QQuick3DTextureData> m_texture_data;

    int32_t m_qt_tex_id = -1;

public:
    static QStringList header();

    ExTexture(noo::TextureID           id,
              nooc::TextureInit const& md,
              TextureChangeNotifier*   notifier);

    ~ExTexture();

    int      get_id() const;
    int      get_id_gen() const;
    QString  get_name() const;
    QVariant get_column(int c) const;

    int32_t qt_tex_id() const { return m_qt_tex_id; }

signals:
    void updated();

private slots:
    void image_ready(QImage);
};

class ExSampler : public nooc::SamplerDelegate {
    Q_OBJECT
public:
    static QStringList header();

    ExSampler(noo::SamplerID id, nooc::SamplerInit const& md);

    ~ExSampler();

    int      get_id() const;
    int      get_id_gen() const;
    QString  get_name() const;
    QVariant get_column(int c) const;

signals:
    void updated();
};

class ExImage : public nooc::ImageDelegate {
    Q_OBJECT
public:
    static QStringList header();

    ExImage(noo::ImageID id, nooc::ImageInit const& md);

    ~ExImage();

    int      get_id() const;
    int      get_id_gen() const;
    QString  get_name() const;
    QVariant get_column(int c) const;
signals:
    void updated();
};

#endif // EXTEXTURE_H
