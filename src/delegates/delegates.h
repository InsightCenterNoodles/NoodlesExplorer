#ifndef DELEGATES_H
#define DELEGATES_H

#include <QPointer>

namespace Qt3DCore {
class QEntity;
class QTransform;
} // namespace Qt3DCore

namespace Qt3DRender {
class QMaterial;
class QGeometry;
class QGeometryRenderer;
class QAbstractLight;
class QAttribute;
class QBuffer;
} // namespace Qt3DRender

class QTreeWidget;
class QTreeWidgetItem;

class QInstancedMetalRoughMaterial;


template <class T>
struct UniqueQPtr {
    QPointer<T> pointer;

    UniqueQPtr() = default;

    UniqueQPtr(T* t) : pointer(t) { }

    ~UniqueQPtr() {
        if (pointer) { pointer->deleteLater(); }
    }

    UniqueQPtr(UniqueQPtr const&) = delete;
    UniqueQPtr& operator=(UniqueQPtr const&) = delete;

    UniqueQPtr(UniqueQPtr&& other) {
        pointer = other.pointer;

        other.pointer.clear();
    }

    UniqueQPtr& operator=(UniqueQPtr&& other) {
        if (&other == this) return *this;
        reset();

        pointer = other.pointer;

        other.pointer.clear();

        return *this;
    }

    void reset(T* lt = nullptr) {
        if (pointer) {
            pointer->deleteLater();
            pointer.clear();
        }

        pointer = lt;
    }

    T* data() { return pointer.data(); }

    operator T*() const { return pointer; }
    operator bool() const { return (bool)pointer; }

    T* operator->() const { return pointer; }
};

QStringList to_qstringlist(std::vector<std::string_view> v);

template <class T>
QString ptr_to_id(std::shared_ptr<T> const& ptr) {
    if (!ptr) return "None";
    return ptr->id().to_qstring();
}

template <class T>
QString ptr_to_id(T* ptr) {
    if (!ptr) return "None";
    return ptr->id().to_qstring();
}


template <class T>
QStringList build_id_list(std::vector<T> const& methods) {
    QStringList ret;

    for (auto const& ptr : methods) {
        ret << ptr->id().to_qstring();
    }

    return ret;
}

class AttachmentBase : public QObject {
    Q_OBJECT

signals:
    void attachment_changed(bool different_ptr = false);
};

template <class T>
class AttachmentPoint : public AttachmentBase {
    using Ptr = std::shared_ptr<T>;
    Ptr m_attachment;

    QMetaObject::Connection m_updated_link;

public:
    void set(Ptr p) {
        if (m_updated_link) disconnect(m_updated_link);

        m_attachment = p;
        emit attachment_changed(true);

        if constexpr (T::CAN_UPDATE) {
            m_updated_link = connect(p.get(), &T::updated, [this]() {
                emit attachment_changed(false);
            });
        }
    }

    void operator=(Ptr p) { set(p); }

    T* get() const { return m_attachment.get(); }
    T* get() { return m_attachment.get(); }

    operator bool() const { return !!m_attachment; }

    T& operator*() { return *m_attachment; }
    T* operator->() { return m_attachment.get(); }
};


#endif // DELEGATES_H
