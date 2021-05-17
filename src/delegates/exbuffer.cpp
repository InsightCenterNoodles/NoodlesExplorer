#include "exbuffer.h"

#include <QEntity>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <Qt3DRender/QBuffer>

QStringList ExBuffer::header() {
    return { "ID", "Name", "Size", "Ready" };
}

ExBuffer::ExBuffer(noo::BufferID                       id,
                   nooc::BufferData const&             md,
                   std::shared_ptr<ComponentListModel> list,
                   Qt3DCore::QEntity*                  scene_root)
    : nooc::BufferDelegate(id, md),
      ComponentListItem(list),
      m_scene_root(scene_root) {

    if (!md.data.empty()) {
        m_array = QByteArray((const char*)md.data.data(), md.data.size());
        m_ready = true;
    } else if (md.url.isValid()) {
        m_array.fill('\0', md.url_size);

        auto* nam = new QNetworkAccessManager(this);

        connect(nam,
                &QNetworkAccessManager::finished,
                this,
                &ExBuffer::url_fetched);

        connect(nam,
                &QNetworkAccessManager::finished,
                nam,
                &QNetworkAccessManager::deleteLater);

        nam->get(QNetworkRequest(md.url));
    }
}

ExBuffer::~ExBuffer() = default;

int ExBuffer::get_id() const {
    return this->id().id_slot;
}
int ExBuffer::get_id_gen() const {
    return this->id().id_gen;
}
QString ExBuffer::get_name() const {
    return id().to_qstring();
}

QVariant ExBuffer::get_column(int c) const {
    switch (c) {
    case 0: return get_id();
    case 1: return get_name();
    case 2: return (qint64)size();
    case 3: return m_ready;
    }
    return {};
}

size_t ExBuffer::size() const {
    return m_array.size();
}

Qt3DRender::QBuffer* ExBuffer::entity() {
    if (!m_3d_entity) {
        m_3d_entity = new Qt3DRender::QBuffer(m_scene_root);

        // doesnt matter if this ready or not;
        m_3d_entity->setData(m_array);
    }

    return m_3d_entity;
}

void ExBuffer::url_fetched(QNetworkReply* reply) {
    reply->deleteLater();

    if (reply->error() != QNetworkReply::NoError) {
        qWarning() << "Unable to fetch resource"
                   << reply->request().url().toString() << " : "
                   << reply->errorString();
        return;
    }

    auto buff = reply->readAll();

    if (buff.size() < m_array.size()) {
        // bad data!
        qWarning() << "Unable to fetch resource"
                   << reply->request().url().toString()
                   << ": Data size does not match given byte count:"
                   << buff.size() << "versus" << m_array.size();
        return;
    }

    m_array = buff;

    m_3d_entity->setData(buff);
}
