#include "extexture.h"

QStringList ExTexture::header() {
    return { "ID", "Name", "Buffer", "Size" };
}

ExTexture::ExTexture(noo::TextureID                      id,
                     nooc::TextureData const&            md,
                     std::shared_ptr<ComponentListModel> list)
    : nooc::TextureDelegate(id, md), ComponentListItem(list), m_data(md) { }

ExTexture::~ExTexture() = default;

int ExTexture::get_id() const {
    return this->id().id_slot;
}
int ExTexture::get_id_gen() const {
    return this->id().id_gen;
}
QString ExTexture::get_name() const {
    return id().to_qstring();
}

QVariant ExTexture::get_column(int c) const {
    switch (c) {
    case 0: return get_id();
    case 1: return get_name();
    case 2: return ptr_to_id(m_data.buffer);
    case 3: return m_data.buffer ? (int)m_data.size : 0;
    }
    return {};
}

void ExTexture::on_update(nooc::TextureData const& md) {
    m_data = md;
}
