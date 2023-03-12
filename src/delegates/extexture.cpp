#include "extexture.h"

#include <magic_enum.hpp>

#include <QImageWriter>

TextureChangeNotifier::TextureChangeNotifier(QObject* parent)
    : ChangeNotifierBase(parent) { }
TextureChangeNotifier::~TextureChangeNotifier() { }

// =============================================================================

QStringList ExTexture::header() {
    return { "ID", "Name", "Info", "Image", "Sampler", "Ready" };
}

ExTexture::ExTexture(noo::TextureID           id,
                     nooc::TextureInit const& md,
                     TextureChangeNotifier*   notifier)
    : nooc::TextureDelegate(id, md), m_notifier(notifier) {
    connect(this,
            &nooc::TextureDelegate::data_ready,
            this,
            &ExTexture::image_ready);
}

ExTexture::~ExTexture() {
    if (m_notifier and m_qt_tex_id >= 0) {
        emit m_notifier->ask_delete(m_qt_tex_id);
        m_notifier->return_id(m_qt_tex_id);
    }
}

int ExTexture::get_id() const {
    return this->id().id_slot;
}
int ExTexture::get_id_gen() const {
    return this->id().id_gen;
}
QString ExTexture::get_name() const {
    return info().name;
}

static QString get_image_info(QImage const& img) {
    if (img.isNull()) return "Null";
    return QString("%1 x %2 : %3")
        .arg(img.width())
        .arg(img.height())
        .arg(magic_enum::enum_name(img.format()).data());
}

QVariant ExTexture::get_column(int c) const {
    switch (c) {
    case 0: return get_id();
    case 1: return get_name();
    case 2: return get_image_info(m_texture_image);
    case 3: return ptr_to_id(info().image);
    case 4: return ptr_to_id(info().sampler);
    case 5: return is_data_ready();
    }
    return {};
}

void ExTexture::image_ready(QImage img) {
    qDebug() << Q_FUNC_INFO << img;
    m_texture_image = img;

    qDebug() << m_texture_image;

    m_texture_image = m_texture_image.convertToFormat(QImage::Format_RGBA8888);

    qDebug() << m_texture_image;

    // m_texture_image.mirror(true, true);

    // QImageWriter writer("dump.png", "png");
    // writer.write(m_texture_image);

    if (!m_texture_data) { m_texture_data = new QQuick3DTextureData(); }

    auto bytes = QByteArray((char*)m_texture_image.bits(),
                            m_texture_image.sizeInBytes());

    m_texture_data->setTextureData(bytes);
    m_texture_data->setFormat(QQuick3DTextureData::RGBA8);
    m_texture_data->setSize(m_texture_image.size());

    m_qt_tex_id = m_notifier->new_id();

    QVariantMap extra;

    extra["minFilter"] = 1;
    extra["magFilter"] = 1;

    if (info().sampler) {
        auto const& sinfo = info().sampler->info();

        //__builtin_dump_struct(&sinfo, &printf);

        if (sinfo.mag_filter == nooc::MagFilter::NEAREST) {
            extra["magFilter"] = 0;
        }

        if (sinfo.min_filter == nooc::MinFilter::NEAREST) {
            extra["minFilter"] = 0;
        }
    }

    emit m_notifier->ask_create(m_qt_tex_id, m_texture_data, extra);
}

// =============================================================================

QStringList ExSampler::header() {
    return { "ID", "Name", "Filters", "Modes" };
}

ExSampler::ExSampler(noo::SamplerID id, nooc::SamplerInit const& md)
    : nooc::SamplerDelegate(id, md) { }

ExSampler::~ExSampler() = default;

int ExSampler::get_id() const {
    return this->id().id_slot;
}
int ExSampler::get_id_gen() const {
    return this->id().id_gen;
}
QString ExSampler::get_name() const {
    return info().name;
}
QVariant ExSampler::get_column(int c) const {
    switch (c) {
    case 0: return get_id();
    case 1: return get_name();
    case 2: {
        auto min_filter = magic_enum::enum_name(info().min_filter);
        auto mag_filter = magic_enum::enum_name(info().mag_filter);
        return QString("%1 / %2").arg(min_filter.data()).arg(mag_filter.data());
    }
    case 3: {
        auto ws = magic_enum::enum_name(info().wrap_s);
        auto wt = magic_enum::enum_name(info().wrap_t);
        return QString("S:%1  T:%2").arg(ws.data()).arg(wt.data());
    }
    }
    return {};
}

// =============================================================================

QStringList ExImage::header() {
    return { "ID", "Name", "Source", "Ready" };
}

ExImage::ExImage(noo::ImageID id, nooc::ImageInit const& md)
    : nooc::ImageDelegate(id, md) { }

ExImage::~ExImage() = default;

int ExImage::get_id() const {
    return this->id().id_slot;
}
int ExImage::get_id_gen() const {
    return this->id().id_gen;
}
QString ExImage::get_name() const {
    return info().name;
}
QVariant ExImage::get_column(int c) const {
    switch (c) {
    case 0: return get_id();
    case 1: return get_name();
    case 2: {
        if (info().local_image) { return ptr_to_id(info().local_image); }
        return info().remote_image;
    }
    case 3: return this->is_data_ready();
    }
    return {};
}
