#ifndef EXTEXTURE_H
#define EXTEXTURE_H

#include "componentlistmodel.h"
#include "delegates.h"

#include <noo_client_interface.h>

class ExTexture : public nooc::TextureDelegate {
    Q_OBJECT
public:
    static QStringList header();

    ExTexture(noo::TextureID id, nooc::TextureInit const& md);

    ~ExTexture();

    int      get_id() const;
    int      get_id_gen() const;
    QString  get_name() const;
    QVariant get_column(int c) const;

signals:
    void updated();
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
