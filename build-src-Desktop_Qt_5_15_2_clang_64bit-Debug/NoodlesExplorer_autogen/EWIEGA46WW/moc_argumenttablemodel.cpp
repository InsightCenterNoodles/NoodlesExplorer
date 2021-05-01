/****************************************************************************
** Meta object code from reading C++ file 'argumenttablemodel.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../src/argumenttablemodel.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'argumenttablemodel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ArgumentTableModel_t {
    QByteArrayData data[8];
    char stringdata0[143];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ArgumentTableModel_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ArgumentTableModel_t qt_meta_stringdata_ArgumentTableModel = {
    {
QT_MOC_LITERAL(0, 0, 18), // "ArgumentTableModel"
QT_MOC_LITERAL(1, 19, 19), // "method_name_changed"
QT_MOC_LITERAL(2, 39, 0), // ""
QT_MOC_LITERAL(3, 40, 11), // "method_name"
QT_MOC_LITERAL(4, 52, 28), // "method_documentation_changed"
QT_MOC_LITERAL(5, 81, 20), // "method_documentation"
QT_MOC_LITERAL(6, 102, 15), // "set_method_name"
QT_MOC_LITERAL(7, 118, 24) // "set_method_documentation"

    },
    "ArgumentTableModel\0method_name_changed\0"
    "\0method_name\0method_documentation_changed\0"
    "method_documentation\0set_method_name\0"
    "set_method_documentation"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ArgumentTableModel[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       2,   46, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   34,    2, 0x06 /* Public */,
       4,    1,   37,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    1,   40,    2, 0x0a /* Public */,
       7,    1,   43,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    5,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    5,

 // properties: name, type, flags
       3, QMetaType::QString, 0x00495003,
       5, QMetaType::QString, 0x00495003,

 // properties: notify_signal_id
       0,
       1,

       0        // eod
};

void ArgumentTableModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ArgumentTableModel *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->method_name_changed((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->method_documentation_changed((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->set_method_name((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->set_method_documentation((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (ArgumentTableModel::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ArgumentTableModel::method_name_changed)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (ArgumentTableModel::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ArgumentTableModel::method_documentation_changed)) {
                *result = 1;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        auto *_t = static_cast<ArgumentTableModel *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QString*>(_v) = _t->method_name(); break;
        case 1: *reinterpret_cast< QString*>(_v) = _t->method_documentation(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        auto *_t = static_cast<ArgumentTableModel *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->set_method_name(*reinterpret_cast< QString*>(_v)); break;
        case 1: _t->set_method_documentation(*reinterpret_cast< QString*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

QT_INIT_METAOBJECT const QMetaObject ArgumentTableModel::staticMetaObject = { {
    QMetaObject::SuperData::link<StructTableModel<Argument>::staticMetaObject>(),
    qt_meta_stringdata_ArgumentTableModel.data,
    qt_meta_data_ArgumentTableModel,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *ArgumentTableModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ArgumentTableModel::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ArgumentTableModel.stringdata0))
        return static_cast<void*>(this);
    return StructTableModel<Argument>::qt_metacast(_clname);
}

int ArgumentTableModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = StructTableModel<Argument>::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 2;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void ArgumentTableModel::method_name_changed(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void ArgumentTableModel::method_documentation_changed(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
