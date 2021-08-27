#ifndef DELEGATES_H
#define DELEGATES_H

#include <QPointer>

#include <span>
#include <vector>

namespace Qt3DCore {
class QEntity;
class QTransform;
class QGeometry;
class QAttribute;
class QBuffer;
} // namespace Qt3DCore

namespace Qt3DRender {
class QMaterial;
class QGeometryRenderer;
class QAbstractLight;
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
QString ptr_to_id(std::unique_ptr<T> const& ptr) {
    if (!ptr) return "None";
    return ptr->id().to_qstring();
}

template <class T>
QString ptr_to_id(QPointer<T> const& ptr) {
    if (!ptr) return "None";
    return ptr->id().to_qstring();
}

template <class T>
QString ptr_to_id(T* ptr) {
    if (!ptr) return "None";
    return ptr->id().to_qstring();
}


template <class T>
QStringList build_id_list(std::vector<T*> const& methods) {
    QStringList ret;

    for (auto const& ptr : methods) {
        ret << ptr->id().to_qstring();
    }

    return ret;
}

template <class T>
QStringList build_id_list(std::vector<QPointer<T>> const& methods) {
    QStringList ret;

    for (auto const& ptr : methods) {
        ret << ptr->id().to_qstring();
    }

    return ret;
}

// =============================================================================

class AttachmentBase : public QObject {
    Q_OBJECT

signals:

    // issued when the attached component is updated
    void attachment_updated();

    // issued when the attachment changes (swapped to a new component)
    void attachment_changed();
};

template <class T>
class AttachmentPoint : public AttachmentBase {
    QPointer<T> m_attachment;

    QMetaObject::Connection m_updated_link;

public:
    void set(T* p) {
        if (m_updated_link) disconnect(m_updated_link);

        m_attachment = p;
        emit attachment_changed();

        if constexpr (T::CAN_UPDATE) {
            m_updated_link = connect(
                p, &T::updated, this, &AttachmentBase::attachment_updated);
        }
    }

    void operator=(T* p) { set(p); }

    T* get() const { return m_attachment; }
    T* get() { return m_attachment; }

    operator bool() const { return !!m_attachment; }

    T& operator*() { return *m_attachment; }
    T* operator->() { return m_attachment; }
};

// =============================================================================

class AttachmentVectorBase : public QObject {
    Q_OBJECT

signals:
    void attachment_updated();

    void attachment_changed();
};

template <class T>
class AttachmentVector : public AttachmentVectorBase {
    std::vector<QPointer<T>> m_attachment;

    std::vector<QMetaObject::Connection> m_updated_links;

public:
    template <class U>
    void set(std::vector<U*> const& ptrs) {

        // static_assert(std::is_base_of_v<T, U> or std::is_same_v<T, U>);

        for (auto l : m_updated_links) {
            disconnect(l);
        }

        m_updated_links.clear();

        m_attachment.clear();

        for (auto p : ptrs) {
            if constexpr (std::is_same_v<T, U>) {
                m_attachment.emplace_back(p);
            } else {
                m_attachment.emplace_back(dynamic_cast<T*>(p));
            }

            if constexpr (T::CAN_UPDATE) {
                m_updated_links.push_back(
                    connect(p,
                            &T::updated,
                            this,
                            &AttachmentVector::attachment_updated));
            }
        }

        emit attachment_changed();
    }
    template <class U>
    void operator=(std::vector<U*> const& ptrs) {
        set(ptrs);
    }

    auto const& get() const { return m_attachment; }
    auto        get() { return m_attachment; }

    auto& operator*() { return get(); }
    auto* operator->() { return &m_attachment; }

    auto begin() const { return m_attachment.begin(); }
    auto end() const { return m_attachment.end(); }
};

#endif // DELEGATES_H
