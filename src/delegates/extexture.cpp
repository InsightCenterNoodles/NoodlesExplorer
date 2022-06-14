#include "extexture.h"

#include <magic_enum.hpp>

QStringList ExTexture::header() {
    return { "ID", "Name", "Image", "Sampler" };
}

ExTexture::ExTexture(noo::TextureID id, nooc::TextureInit const& md)
    : nooc::TextureDelegate(id, md) { }

ExTexture::~ExTexture() = default;

int ExTexture::get_id() const {
    return this->id().id_slot;
}
int ExTexture::get_id_gen() const {
    return this->id().id_gen;
}
QString ExTexture::get_name() const {
    return info().name;
}

QVariant ExTexture::get_column(int c) const {
    switch (c) {
    case 0: return get_id();
    case 1: return get_name();
    case 2: return ptr_to_id(info().image);
    case 3: return ptr_to_id(info().sampler);
    }
    return {};
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
