/****************************************************************************
** Meta object code from reading C++ file 'delegates.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../src/delegates.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'delegates.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ExBuffer_t {
    QByteArrayData data[4];
    char stringdata0[37];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ExBuffer_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ExBuffer_t qt_meta_stringdata_ExBuffer = {
    {
QT_MOC_LITERAL(0, 0, 8), // "ExBuffer"
QT_MOC_LITERAL(1, 9, 11), // "url_fetched"
QT_MOC_LITERAL(2, 21, 0), // ""
QT_MOC_LITERAL(3, 22, 14) // "QNetworkReply*"

    },
    "ExBuffer\0url_fetched\0\0QNetworkReply*"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ExBuffer[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   19,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    2,

       0        // eod
};

void ExBuffer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ExBuffer *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->url_fetched((*reinterpret_cast< QNetworkReply*(*)>(_a[1]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject ExBuffer::staticMetaObject = { {
    QMetaObject::SuperData::link<nooc::BufferDelegate::staticMetaObject>(),
    qt_meta_stringdata_ExBuffer.data,
    qt_meta_data_ExBuffer,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *ExBuffer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ExBuffer::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ExBuffer.stringdata0))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "ComponentListItem"))
        return static_cast< ComponentListItem*>(this);
    return nooc::BufferDelegate::qt_metacast(_clname);
}

int ExBuffer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = nooc::BufferDelegate::qt_metacall(_c, _id, _a);
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
struct qt_meta_stringdata_ExDoc_t {
    QByteArrayData data[1];
    char stringdata0[6];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ExDoc_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ExDoc_t qt_meta_stringdata_ExDoc = {
    {
QT_MOC_LITERAL(0, 0, 5) // "ExDoc"

    },
    "ExDoc"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ExDoc[] = {

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

void ExDoc::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject ExDoc::staticMetaObject = { {
    QMetaObject::SuperData::link<nooc::DocumentDelegate::staticMetaObject>(),
    qt_meta_stringdata_ExDoc.data,
    qt_meta_data_ExDoc,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *ExDoc::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ExDoc::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ExDoc.stringdata0))
        return static_cast<void*>(this);
    return nooc::DocumentDelegate::qt_metacast(_clname);
}

int ExDoc::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = nooc::DocumentDelegate::qt_metacall(_c, _id, _a);
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
