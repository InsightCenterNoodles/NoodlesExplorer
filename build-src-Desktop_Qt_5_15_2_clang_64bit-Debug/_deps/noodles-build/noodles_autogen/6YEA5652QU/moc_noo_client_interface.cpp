/****************************************************************************
** Meta object code from reading C++ file 'noo_client_interface.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../noodles-src/include/noo_client_interface.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'noo_client_interface.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_nooc__PendingMethodReply_t {
    QByteArrayData data[6];
    char stringdata0[70];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_nooc__PendingMethodReply_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_nooc__PendingMethodReply_t qt_meta_stringdata_nooc__PendingMethodReply = {
    {
QT_MOC_LITERAL(0, 0, 24), // "nooc::PendingMethodReply"
QT_MOC_LITERAL(1, 25, 9), // "recv_data"
QT_MOC_LITERAL(2, 35, 0), // ""
QT_MOC_LITERAL(3, 36, 14), // "noo::AnyVarRef"
QT_MOC_LITERAL(4, 51, 9), // "recv_fail"
QT_MOC_LITERAL(5, 61, 8) // "complete"

    },
    "nooc::PendingMethodReply\0recv_data\0\0"
    "noo::AnyVarRef\0recv_fail\0complete"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_nooc__PendingMethodReply[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   29,    2, 0x06 /* Public */,
       4,    1,   32,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    2,   35,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    2,
    QMetaType::Void, QMetaType::QString,    2,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3, QMetaType::QString,    2,    2,

       0        // eod
};

void nooc::PendingMethodReply::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<PendingMethodReply *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->recv_data((*reinterpret_cast< noo::AnyVarRef(*)>(_a[1]))); break;
        case 1: _t->recv_fail((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->complete((*reinterpret_cast< noo::AnyVarRef(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (PendingMethodReply::*)(noo::AnyVarRef );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&PendingMethodReply::recv_data)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (PendingMethodReply::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&PendingMethodReply::recv_fail)) {
                *result = 1;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject nooc::PendingMethodReply::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_nooc__PendingMethodReply.data,
    qt_meta_data_nooc__PendingMethodReply,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *nooc::PendingMethodReply::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *nooc::PendingMethodReply::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_nooc__PendingMethodReply.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int nooc::PendingMethodReply::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void nooc::PendingMethodReply::recv_data(noo::AnyVarRef _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void nooc::PendingMethodReply::recv_fail(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
struct qt_meta_stringdata_nooc__translators__GetIntegerReply_t {
    QByteArrayData data[4];
    char stringdata0[49];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_nooc__translators__GetIntegerReply_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_nooc__translators__GetIntegerReply_t qt_meta_stringdata_nooc__translators__GetIntegerReply = {
    {
QT_MOC_LITERAL(0, 0, 34), // "nooc::translators::GetInteger..."
QT_MOC_LITERAL(1, 35, 4), // "recv"
QT_MOC_LITERAL(2, 40, 0), // ""
QT_MOC_LITERAL(3, 41, 7) // "int64_t"

    },
    "nooc::translators::GetIntegerReply\0"
    "recv\0\0int64_t"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_nooc__translators__GetIntegerReply[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   19,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    2,

       0        // eod
};

void nooc::translators::GetIntegerReply::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<GetIntegerReply *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->recv((*reinterpret_cast< int64_t(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (GetIntegerReply::*)(int64_t );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&GetIntegerReply::recv)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject nooc::translators::GetIntegerReply::staticMetaObject = { {
    QMetaObject::SuperData::link<PendingMethodReply::staticMetaObject>(),
    qt_meta_stringdata_nooc__translators__GetIntegerReply.data,
    qt_meta_data_nooc__translators__GetIntegerReply,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *nooc::translators::GetIntegerReply::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *nooc::translators::GetIntegerReply::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_nooc__translators__GetIntegerReply.stringdata0))
        return static_cast<void*>(this);
    return PendingMethodReply::qt_metacast(_clname);
}

int nooc::translators::GetIntegerReply::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = PendingMethodReply::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void nooc::translators::GetIntegerReply::recv(int64_t _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
struct qt_meta_stringdata_nooc__translators__GetStringReply_t {
    QByteArrayData data[3];
    char stringdata0[40];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_nooc__translators__GetStringReply_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_nooc__translators__GetStringReply_t qt_meta_stringdata_nooc__translators__GetStringReply = {
    {
QT_MOC_LITERAL(0, 0, 33), // "nooc::translators::GetStringR..."
QT_MOC_LITERAL(1, 34, 4), // "recv"
QT_MOC_LITERAL(2, 39, 0) // ""

    },
    "nooc::translators::GetStringReply\0"
    "recv\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_nooc__translators__GetStringReply[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   19,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    2,

       0        // eod
};

void nooc::translators::GetStringReply::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<GetStringReply *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->recv((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (GetStringReply::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&GetStringReply::recv)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject nooc::translators::GetStringReply::staticMetaObject = { {
    QMetaObject::SuperData::link<PendingMethodReply::staticMetaObject>(),
    qt_meta_stringdata_nooc__translators__GetStringReply.data,
    qt_meta_data_nooc__translators__GetStringReply,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *nooc::translators::GetStringReply::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *nooc::translators::GetStringReply::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_nooc__translators__GetStringReply.stringdata0))
        return static_cast<void*>(this);
    return PendingMethodReply::qt_metacast(_clname);
}

int nooc::translators::GetStringReply::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = PendingMethodReply::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void nooc::translators::GetStringReply::recv(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
struct qt_meta_stringdata_nooc__translators__GetStringListReply_t {
    QByteArrayData data[3];
    char stringdata0[44];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_nooc__translators__GetStringListReply_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_nooc__translators__GetStringListReply_t qt_meta_stringdata_nooc__translators__GetStringListReply = {
    {
QT_MOC_LITERAL(0, 0, 37), // "nooc::translators::GetStringL..."
QT_MOC_LITERAL(1, 38, 4), // "recv"
QT_MOC_LITERAL(2, 43, 0) // ""

    },
    "nooc::translators::GetStringListReply\0"
    "recv\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_nooc__translators__GetStringListReply[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   19,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QStringList,    2,

       0        // eod
};

void nooc::translators::GetStringListReply::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<GetStringListReply *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->recv((*reinterpret_cast< QStringList(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (GetStringListReply::*)(QStringList );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&GetStringListReply::recv)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject nooc::translators::GetStringListReply::staticMetaObject = { {
    QMetaObject::SuperData::link<PendingMethodReply::staticMetaObject>(),
    qt_meta_stringdata_nooc__translators__GetStringListReply.data,
    qt_meta_data_nooc__translators__GetStringListReply,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *nooc::translators::GetStringListReply::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *nooc::translators::GetStringListReply::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_nooc__translators__GetStringListReply.stringdata0))
        return static_cast<void*>(this);
    return PendingMethodReply::qt_metacast(_clname);
}

int nooc::translators::GetStringListReply::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = PendingMethodReply::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void nooc::translators::GetStringListReply::recv(QStringList _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
struct qt_meta_stringdata_nooc__translators__GetRealsReply_t {
    QByteArrayData data[4];
    char stringdata0[76];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_nooc__translators__GetRealsReply_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_nooc__translators__GetRealsReply_t qt_meta_stringdata_nooc__translators__GetRealsReply = {
    {
QT_MOC_LITERAL(0, 0, 32), // "nooc::translators::GetRealsReply"
QT_MOC_LITERAL(1, 33, 4), // "recv"
QT_MOC_LITERAL(2, 38, 0), // ""
QT_MOC_LITERAL(3, 39, 36) // "noo::PossiblyOwnedView<const ..."

    },
    "nooc::translators::GetRealsReply\0recv\0"
    "\0noo::PossiblyOwnedView<const double>"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_nooc__translators__GetRealsReply[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   19,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    2,

       0        // eod
};

void nooc::translators::GetRealsReply::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<GetRealsReply *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->recv((*reinterpret_cast< noo::PossiblyOwnedView<const double>(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (GetRealsReply::*)(noo::PossiblyOwnedView<double const> const & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&GetRealsReply::recv)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject nooc::translators::GetRealsReply::staticMetaObject = { {
    QMetaObject::SuperData::link<PendingMethodReply::staticMetaObject>(),
    qt_meta_stringdata_nooc__translators__GetRealsReply.data,
    qt_meta_data_nooc__translators__GetRealsReply,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *nooc::translators::GetRealsReply::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *nooc::translators::GetRealsReply::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_nooc__translators__GetRealsReply.stringdata0))
        return static_cast<void*>(this);
    return PendingMethodReply::qt_metacast(_clname);
}

int nooc::translators::GetRealsReply::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = PendingMethodReply::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void nooc::translators::GetRealsReply::recv(noo::PossiblyOwnedView<double const> const & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
struct qt_meta_stringdata_nooc__translators__GetSelectionReply_t {
    QByteArrayData data[4];
    char stringdata0[61];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_nooc__translators__GetSelectionReply_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_nooc__translators__GetSelectionReply_t qt_meta_stringdata_nooc__translators__GetSelectionReply = {
    {
QT_MOC_LITERAL(0, 0, 36), // "nooc::translators::GetSelecti..."
QT_MOC_LITERAL(1, 37, 4), // "recv"
QT_MOC_LITERAL(2, 42, 0), // ""
QT_MOC_LITERAL(3, 43, 17) // "noo::SelectionRef"

    },
    "nooc::translators::GetSelectionReply\0"
    "recv\0\0noo::SelectionRef"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_nooc__translators__GetSelectionReply[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   19,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, 0x80000000 | 3,    2,    2,

       0        // eod
};

void nooc::translators::GetSelectionReply::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<GetSelectionReply *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->recv((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< const noo::SelectionRef(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (GetSelectionReply::*)(QString , noo::SelectionRef const & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&GetSelectionReply::recv)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject nooc::translators::GetSelectionReply::staticMetaObject = { {
    QMetaObject::SuperData::link<PendingMethodReply::staticMetaObject>(),
    qt_meta_stringdata_nooc__translators__GetSelectionReply.data,
    qt_meta_data_nooc__translators__GetSelectionReply,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *nooc::translators::GetSelectionReply::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *nooc::translators::GetSelectionReply::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_nooc__translators__GetSelectionReply.stringdata0))
        return static_cast<void*>(this);
    return PendingMethodReply::qt_metacast(_clname);
}

int nooc::translators::GetSelectionReply::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = PendingMethodReply::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void nooc::translators::GetSelectionReply::recv(QString _t1, noo::SelectionRef const & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
struct qt_meta_stringdata_nooc__AttachedSignal_t {
    QByteArrayData data[4];
    char stringdata0[40];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_nooc__AttachedSignal_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_nooc__AttachedSignal_t qt_meta_stringdata_nooc__AttachedSignal = {
    {
QT_MOC_LITERAL(0, 0, 20), // "nooc::AttachedSignal"
QT_MOC_LITERAL(1, 21, 5), // "fired"
QT_MOC_LITERAL(2, 27, 0), // ""
QT_MOC_LITERAL(3, 28, 11) // "noo::AnyVar"

    },
    "nooc::AttachedSignal\0fired\0\0noo::AnyVar"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_nooc__AttachedSignal[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   19,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    2,

       0        // eod
};

void nooc::AttachedSignal::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<AttachedSignal *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->fired((*reinterpret_cast< const noo::AnyVar(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (AttachedSignal::*)(noo::AnyVar const & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AttachedSignal::fired)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject nooc::AttachedSignal::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_nooc__AttachedSignal.data,
    qt_meta_data_nooc__AttachedSignal,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *nooc::AttachedSignal::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *nooc::AttachedSignal::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_nooc__AttachedSignal.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int nooc::AttachedSignal::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void nooc::AttachedSignal::fired(noo::AnyVar const & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
struct qt_meta_stringdata_nooc__MethodDelegate_t {
    QByteArrayData data[7];
    char stringdata0[93];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_nooc__MethodDelegate_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_nooc__MethodDelegate_t qt_meta_stringdata_nooc__MethodDelegate = {
    {
QT_MOC_LITERAL(0, 0, 20), // "nooc::MethodDelegate"
QT_MOC_LITERAL(1, 21, 6), // "invoke"
QT_MOC_LITERAL(2, 28, 0), // ""
QT_MOC_LITERAL(3, 29, 13), // "noo::MethodID"
QT_MOC_LITERAL(4, 43, 13), // "MethodContext"
QT_MOC_LITERAL(5, 57, 15), // "noo::AnyVarList"
QT_MOC_LITERAL(6, 73, 19) // "PendingMethodReply*"

    },
    "nooc::MethodDelegate\0invoke\0\0noo::MethodID\0"
    "MethodContext\0noo::AnyVarList\0"
    "PendingMethodReply*"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_nooc__MethodDelegate[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    4,   19,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 4, 0x80000000 | 5, 0x80000000 | 6,    2,    2,    2,    2,

       0        // eod
};

void nooc::MethodDelegate::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MethodDelegate *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->invoke((*reinterpret_cast< noo::MethodID(*)>(_a[1])),(*reinterpret_cast< MethodContext(*)>(_a[2])),(*reinterpret_cast< const noo::AnyVarList(*)>(_a[3])),(*reinterpret_cast< PendingMethodReply*(*)>(_a[4]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 3:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< PendingMethodReply* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (MethodDelegate::*)(noo::MethodID , MethodContext , noo::AnyVarList const & , PendingMethodReply * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MethodDelegate::invoke)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject nooc::MethodDelegate::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_nooc__MethodDelegate.data,
    qt_meta_data_nooc__MethodDelegate,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *nooc::MethodDelegate::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *nooc::MethodDelegate::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_nooc__MethodDelegate.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int nooc::MethodDelegate::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void nooc::MethodDelegate::invoke(noo::MethodID _t1, MethodContext _t2, noo::AnyVarList const & _t3, PendingMethodReply * _t4)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t4))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
struct qt_meta_stringdata_nooc__SignalDelegate_t {
    QByteArrayData data[5];
    char stringdata0[58];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_nooc__SignalDelegate_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_nooc__SignalDelegate_t qt_meta_stringdata_nooc__SignalDelegate = {
    {
QT_MOC_LITERAL(0, 0, 20), // "nooc::SignalDelegate"
QT_MOC_LITERAL(1, 21, 5), // "fired"
QT_MOC_LITERAL(2, 27, 0), // ""
QT_MOC_LITERAL(3, 28, 13), // "MethodContext"
QT_MOC_LITERAL(4, 42, 15) // "noo::AnyVarList"

    },
    "nooc::SignalDelegate\0fired\0\0MethodContext\0"
    "noo::AnyVarList"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_nooc__SignalDelegate[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   19,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 4,    2,    2,

       0        // eod
};

void nooc::SignalDelegate::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<SignalDelegate *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->fired((*reinterpret_cast< MethodContext(*)>(_a[1])),(*reinterpret_cast< noo::AnyVarList(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (SignalDelegate::*)(MethodContext , noo::AnyVarList );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SignalDelegate::fired)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject nooc::SignalDelegate::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_nooc__SignalDelegate.data,
    qt_meta_data_nooc__SignalDelegate,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *nooc::SignalDelegate::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *nooc::SignalDelegate::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_nooc__SignalDelegate.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int nooc::SignalDelegate::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void nooc::SignalDelegate::fired(MethodContext _t1, noo::AnyVarList _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
struct qt_meta_stringdata_nooc__BufferDelegate_t {
    QByteArrayData data[1];
    char stringdata0[21];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_nooc__BufferDelegate_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_nooc__BufferDelegate_t qt_meta_stringdata_nooc__BufferDelegate = {
    {
QT_MOC_LITERAL(0, 0, 20) // "nooc::BufferDelegate"

    },
    "nooc::BufferDelegate"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_nooc__BufferDelegate[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

void nooc::BufferDelegate::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject nooc::BufferDelegate::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_nooc__BufferDelegate.data,
    qt_meta_data_nooc__BufferDelegate,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *nooc::BufferDelegate::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *nooc::BufferDelegate::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_nooc__BufferDelegate.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int nooc::BufferDelegate::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    return _id;
}
struct qt_meta_stringdata_nooc__TextureDelegate_t {
    QByteArrayData data[3];
    char stringdata0[31];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_nooc__TextureDelegate_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_nooc__TextureDelegate_t qt_meta_stringdata_nooc__TextureDelegate = {
    {
QT_MOC_LITERAL(0, 0, 21), // "nooc::TextureDelegate"
QT_MOC_LITERAL(1, 22, 7), // "updated"
QT_MOC_LITERAL(2, 30, 0) // ""

    },
    "nooc::TextureDelegate\0updated\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_nooc__TextureDelegate[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   19,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void,

       0        // eod
};

void nooc::TextureDelegate::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<TextureDelegate *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->updated(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (TextureDelegate::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TextureDelegate::updated)) {
                *result = 0;
                return;
            }
        }
    }
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject nooc::TextureDelegate::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_nooc__TextureDelegate.data,
    qt_meta_data_nooc__TextureDelegate,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *nooc::TextureDelegate::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *nooc::TextureDelegate::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_nooc__TextureDelegate.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int nooc::TextureDelegate::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void nooc::TextureDelegate::updated()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
struct qt_meta_stringdata_nooc__MaterialDelegate_t {
    QByteArrayData data[3];
    char stringdata0[32];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_nooc__MaterialDelegate_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_nooc__MaterialDelegate_t qt_meta_stringdata_nooc__MaterialDelegate = {
    {
QT_MOC_LITERAL(0, 0, 22), // "nooc::MaterialDelegate"
QT_MOC_LITERAL(1, 23, 7), // "updated"
QT_MOC_LITERAL(2, 31, 0) // ""

    },
    "nooc::MaterialDelegate\0updated\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_nooc__MaterialDelegate[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   19,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void,

       0        // eod
};

void nooc::MaterialDelegate::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MaterialDelegate *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->updated(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (MaterialDelegate::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MaterialDelegate::updated)) {
                *result = 0;
                return;
            }
        }
    }
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject nooc::MaterialDelegate::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_nooc__MaterialDelegate.data,
    qt_meta_data_nooc__MaterialDelegate,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *nooc::MaterialDelegate::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *nooc::MaterialDelegate::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_nooc__MaterialDelegate.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int nooc::MaterialDelegate::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void nooc::MaterialDelegate::updated()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
struct qt_meta_stringdata_nooc__LightDelegate_t {
    QByteArrayData data[3];
    char stringdata0[29];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_nooc__LightDelegate_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_nooc__LightDelegate_t qt_meta_stringdata_nooc__LightDelegate = {
    {
QT_MOC_LITERAL(0, 0, 19), // "nooc::LightDelegate"
QT_MOC_LITERAL(1, 20, 7), // "updated"
QT_MOC_LITERAL(2, 28, 0) // ""

    },
    "nooc::LightDelegate\0updated\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_nooc__LightDelegate[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   19,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void,

       0        // eod
};

void nooc::LightDelegate::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<LightDelegate *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->updated(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (LightDelegate::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&LightDelegate::updated)) {
                *result = 0;
                return;
            }
        }
    }
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject nooc::LightDelegate::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_nooc__LightDelegate.data,
    qt_meta_data_nooc__LightDelegate,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *nooc::LightDelegate::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *nooc::LightDelegate::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_nooc__LightDelegate.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int nooc::LightDelegate::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void nooc::LightDelegate::updated()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
struct qt_meta_stringdata_nooc__MeshDelegate_t {
    QByteArrayData data[1];
    char stringdata0[19];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_nooc__MeshDelegate_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_nooc__MeshDelegate_t qt_meta_stringdata_nooc__MeshDelegate = {
    {
QT_MOC_LITERAL(0, 0, 18) // "nooc::MeshDelegate"

    },
    "nooc::MeshDelegate"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_nooc__MeshDelegate[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

void nooc::MeshDelegate::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject nooc::MeshDelegate::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_nooc__MeshDelegate.data,
    qt_meta_data_nooc__MeshDelegate,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *nooc::MeshDelegate::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *nooc::MeshDelegate::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_nooc__MeshDelegate.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int nooc::MeshDelegate::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    return _id;
}
struct qt_meta_stringdata_nooc__ObjectDelegate_t {
    QByteArrayData data[3];
    char stringdata0[30];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_nooc__ObjectDelegate_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_nooc__ObjectDelegate_t qt_meta_stringdata_nooc__ObjectDelegate = {
    {
QT_MOC_LITERAL(0, 0, 20), // "nooc::ObjectDelegate"
QT_MOC_LITERAL(1, 21, 7), // "updated"
QT_MOC_LITERAL(2, 29, 0) // ""

    },
    "nooc::ObjectDelegate\0updated\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_nooc__ObjectDelegate[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   19,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void,

       0        // eod
};

void nooc::ObjectDelegate::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ObjectDelegate *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->updated(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (ObjectDelegate::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ObjectDelegate::updated)) {
                *result = 0;
                return;
            }
        }
    }
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject nooc::ObjectDelegate::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_nooc__ObjectDelegate.data,
    qt_meta_data_nooc__ObjectDelegate,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *nooc::ObjectDelegate::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *nooc::ObjectDelegate::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_nooc__ObjectDelegate.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int nooc::ObjectDelegate::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void nooc::ObjectDelegate::updated()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
struct qt_meta_stringdata_nooc__TableDelegate_t {
    QByteArrayData data[8];
    char stringdata0[125];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_nooc__TableDelegate_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_nooc__TableDelegate_t qt_meta_stringdata_nooc__TableDelegate = {
    {
QT_MOC_LITERAL(0, 0, 19), // "nooc::TableDelegate"
QT_MOC_LITERAL(1, 20, 7), // "updated"
QT_MOC_LITERAL(2, 28, 0), // ""
QT_MOC_LITERAL(3, 29, 14), // "on_get_columns"
QT_MOC_LITERAL(4, 44, 15), // "on_get_num_rows"
QT_MOC_LITERAL(5, 60, 20), // "on_request_selection"
QT_MOC_LITERAL(6, 81, 17), // "noo::SelectionRef"
QT_MOC_LITERAL(7, 99, 25) // "on_request_all_selections"

    },
    "nooc::TableDelegate\0updated\0\0"
    "on_get_columns\0on_get_num_rows\0"
    "on_request_selection\0noo::SelectionRef\0"
    "on_request_all_selections"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_nooc__TableDelegate[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   39,    2, 0x06 /* Public */,
       3,    1,   40,    2, 0x06 /* Public */,
       4,    1,   43,    2, 0x06 /* Public */,
       5,    2,   46,    2, 0x06 /* Public */,
       7,    0,   51,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QStringList,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::QString, 0x80000000 | 6,    2,    2,
    QMetaType::Void,

       0        // eod
};

void nooc::TableDelegate::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<TableDelegate *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->updated(); break;
        case 1: _t->on_get_columns((*reinterpret_cast< QStringList(*)>(_a[1]))); break;
        case 2: _t->on_get_num_rows((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->on_request_selection((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< const noo::SelectionRef(*)>(_a[2]))); break;
        case 4: _t->on_request_all_selections(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (TableDelegate::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TableDelegate::updated)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (TableDelegate::*)(QStringList );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TableDelegate::on_get_columns)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (TableDelegate::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TableDelegate::on_get_num_rows)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (TableDelegate::*)(QString , noo::SelectionRef const & ) const;
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TableDelegate::on_request_selection)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (TableDelegate::*)() const;
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TableDelegate::on_request_all_selections)) {
                *result = 4;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject nooc::TableDelegate::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_nooc__TableDelegate.data,
    qt_meta_data_nooc__TableDelegate,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *nooc::TableDelegate::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *nooc::TableDelegate::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_nooc__TableDelegate.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int nooc::TableDelegate::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void nooc::TableDelegate::updated()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void nooc::TableDelegate::on_get_columns(QStringList _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void nooc::TableDelegate::on_get_num_rows(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void nooc::TableDelegate::on_request_selection(QString _t1, noo::SelectionRef const & _t2)const
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(const_cast< nooc::TableDelegate *>(this), &staticMetaObject, 3, _a);
}

// SIGNAL 4
void nooc::TableDelegate::on_request_all_selections()const
{
    QMetaObject::activate(const_cast< nooc::TableDelegate *>(this), &staticMetaObject, 4, nullptr);
}
struct qt_meta_stringdata_nooc__DocumentDelegate_t {
    QByteArrayData data[3];
    char stringdata0[32];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_nooc__DocumentDelegate_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_nooc__DocumentDelegate_t qt_meta_stringdata_nooc__DocumentDelegate = {
    {
QT_MOC_LITERAL(0, 0, 22), // "nooc::DocumentDelegate"
QT_MOC_LITERAL(1, 23, 7), // "updated"
QT_MOC_LITERAL(2, 31, 0) // ""

    },
    "nooc::DocumentDelegate\0updated\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_nooc__DocumentDelegate[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   19,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void,

       0        // eod
};

void nooc::DocumentDelegate::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<DocumentDelegate *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->updated(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (DocumentDelegate::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DocumentDelegate::updated)) {
                *result = 0;
                return;
            }
        }
    }
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject nooc::DocumentDelegate::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_nooc__DocumentDelegate.data,
    qt_meta_data_nooc__DocumentDelegate,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *nooc::DocumentDelegate::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *nooc::DocumentDelegate::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_nooc__DocumentDelegate.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int nooc::DocumentDelegate::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void nooc::DocumentDelegate::updated()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
struct qt_meta_stringdata_nooc__ClientConnection_t {
    QByteArrayData data[5];
    char stringdata0[60];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_nooc__ClientConnection_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_nooc__ClientConnection_t qt_meta_stringdata_nooc__ClientConnection = {
    {
QT_MOC_LITERAL(0, 0, 22), // "nooc::ClientConnection"
QT_MOC_LITERAL(1, 23, 12), // "socket_error"
QT_MOC_LITERAL(2, 36, 0), // ""
QT_MOC_LITERAL(3, 37, 9), // "connected"
QT_MOC_LITERAL(4, 47, 12) // "disconnected"

    },
    "nooc::ClientConnection\0socket_error\0"
    "\0connected\0disconnected"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_nooc__ClientConnection[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   29,    2, 0x06 /* Public */,
       3,    0,   32,    2, 0x06 /* Public */,
       4,    0,   33,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void nooc::ClientConnection::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ClientConnection *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->socket_error((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->connected(); break;
        case 2: _t->disconnected(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (ClientConnection::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ClientConnection::socket_error)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (ClientConnection::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ClientConnection::connected)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (ClientConnection::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ClientConnection::disconnected)) {
                *result = 2;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject nooc::ClientConnection::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_nooc__ClientConnection.data,
    qt_meta_data_nooc__ClientConnection,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *nooc::ClientConnection::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *nooc::ClientConnection::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_nooc__ClientConnection.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int nooc::ClientConnection::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void nooc::ClientConnection::socket_error(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void nooc::ClientConnection::connected()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void nooc::ClientConnection::disconnected()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
