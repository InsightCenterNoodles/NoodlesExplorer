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

class IterationCounter {
    size_t m_counter = 0;

public:
    size_t next() {
        m_counter++;
        return m_counter;
    }
};


template <class T>
struct UniqueQPtr {
    QPointer<T> pointer;

    UniqueQPtr() = default;

    UniqueQPtr(T* t) : pointer(t) { }

    ~UniqueQPtr() {
        if (pointer) { pointer->deleteLater(); }
    }

    UniqueQPtr(UniqueQPtr const&)            = delete;
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
QString ptr_to_id(std::optional<T> const& ptr) {
    if (!ptr) return "None";
    return ptr_to_id(*ptr);
}


template <class Container>
QStringList build_id_list(Container const& things) {
    QStringList ret;

    for (auto const& ptr : things) {
        ret << ptr->id().to_qstring();
    }

    return ret;
}

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

// =============================================================================

class ChangeNotifierBase : public QObject {
    Q_OBJECT

    int32_t              m_next = 0;
    std::vector<int32_t> m_free_list;

public:
    explicit ChangeNotifierBase(QObject* parent = nullptr) : QObject(parent) { }
    ~ChangeNotifierBase() = default;

    int32_t new_id() {
        if (m_free_list.empty()) {
            int32_t ret = m_next;
            m_next++;
            return ret;
        }
        int32_t ret = m_free_list.back();
        m_free_list.pop_back();
        return ret;
    }

    void return_id(int32_t id) { m_free_list.push_back(id); }
};


#endif // DELEGATES_H
