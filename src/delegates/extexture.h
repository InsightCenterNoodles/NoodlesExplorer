#ifndef EXTEXTURE_H
#define EXTEXTURE_H

#include "componentlistmodel.h"
#include "delegates.h"

#include <noo_client_interface.h>

class ExTexture : public nooc::TextureDelegate {
public:
    static QStringList header();

    ExTexture(noo::TextureID id, nooc::TextureInit const& md);

    ~ExTexture();

    int      get_id() const;
    int      get_id_gen() const;
    QString  get_name() const;
    QVariant get_column(int c) const;
};

class ExSampler : public nooc::SamplerDelegate {
public:
    static QStringList header();

    ExSampler(noo::SamplerID id, nooc::SamplerInit const& md);

    ~ExSampler();

    int      get_id() const;
    int      get_id_gen() const;
    QString  get_name() const;
    QVariant get_column(int c) const;
};

class ExImage : public nooc::ImageDelegate {
public:
    static QStringList header();

    ExImage(noo::ImageID id, nooc::ImageInit const& md);

    ~ExImage();

    int      get_id() const;
    int      get_id_gen() const;
    QString  get_name() const;
    QVariant get_column(int c) const;
};

#endif // EXTEXTURE_H
