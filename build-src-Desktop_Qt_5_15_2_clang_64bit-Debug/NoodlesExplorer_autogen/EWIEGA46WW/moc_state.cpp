/****************************************************************************
** Meta object code from reading C++ file 'state.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../src/state.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'state.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_State_t {
    QByteArrayData data[22];
    char stringdata0[332];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_State_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_State_t qt_meta_stringdata_State = {
    {
QT_MOC_LITERAL(0, 0, 5), // "State"
QT_MOC_LITERAL(1, 6, 24), // "connection_state_changed"
QT_MOC_LITERAL(2, 31, 0), // ""
QT_MOC_LITERAL(3, 32, 16), // "connection_state"
QT_MOC_LITERAL(4, 49, 21), // "open_call_method_page"
QT_MOC_LITERAL(5, 71, 22), // "method_result_recieved"
QT_MOC_LITERAL(6, 94, 21), // "method_error_recieved"
QT_MOC_LITERAL(7, 116, 10), // "debug_tree"
QT_MOC_LITERAL(8, 127, 16), // "start_connection"
QT_MOC_LITERAL(9, 144, 4), // "name"
QT_MOC_LITERAL(10, 149, 3), // "url"
QT_MOC_LITERAL(11, 153, 20), // "set_connection_state"
QT_MOC_LITERAL(12, 174, 12), // "get_hostname"
QT_MOC_LITERAL(13, 187, 22), // "ask_to_call_doc_method"
QT_MOC_LITERAL(14, 210, 5), // "index"
QT_MOC_LITERAL(15, 216, 17), // "invoke_doc_method"
QT_MOC_LITERAL(16, 234, 10), // "scene_root"
QT_MOC_LITERAL(17, 245, 18), // "Qt3DCore::QEntity*"
QT_MOC_LITERAL(18, 264, 10), // "exec_debug"
QT_MOC_LITERAL(19, 275, 14), // "handle_connect"
QT_MOC_LITERAL(20, 290, 17), // "handle_disconnect"
QT_MOC_LITERAL(21, 308, 23) // "handle_document_updated"

    },
    "State\0connection_state_changed\0\0"
    "connection_state\0open_call_method_page\0"
    "method_result_recieved\0method_error_recieved\0"
    "debug_tree\0start_connection\0name\0url\0"
    "set_connection_state\0get_hostname\0"
    "ask_to_call_doc_method\0index\0"
    "invoke_doc_method\0scene_root\0"
    "Qt3DCore::QEntity*\0exec_debug\0"
    "handle_connect\0handle_disconnect\0"
    "handle_document_updated"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_State[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      15,   14, // methods
       1,  118, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   89,    2, 0x06 /* Public */,
       4,    0,   92,    2, 0x06 /* Public */,
       5,    1,   93,    2, 0x06 /* Public */,
       6,    1,   96,    2, 0x06 /* Public */,
       7,    0,   99,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       8,    2,  100,    2, 0x0a /* Public */,
      11,    1,  105,    2, 0x0a /* Public */,
      12,    0,  108,    2, 0x0a /* Public */,
      13,    1,  109,    2, 0x0a /* Public */,
      15,    0,  112,    2, 0x0a /* Public */,
      16,    0,  113,    2, 0x0a /* Public */,
      18,    0,  114,    2, 0x0a /* Public */,
      19,    0,  115,    2, 0x08 /* Private */,
      20,    0,  116,    2, 0x08 /* Private */,
      21,    0,  117,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Bool, QMetaType::QString, QMetaType::QString,    9,   10,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::QString,
    QMetaType::Void, QMetaType::Int,   14,
    QMetaType::Void,
    0x80000000 | 17,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // properties: name, type, flags
       3, QMetaType::Int, 0x00495003,

 // properties: notify_signal_id
       0,

       0        // eod
};

void State::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<State *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->connection_state_changed((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->open_call_method_page(); break;
        case 2: _t->method_result_recieved((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->method_error_recieved((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: _t->debug_tree(); break;
        case 5: { bool _r = _t->start_connection((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 6: _t->set_connection_state((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: { QString _r = _t->get_hostname();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 8: _t->ask_to_call_doc_method((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: _t->invoke_doc_method(); break;
        case 10: { Qt3DCore::QEntity* _r = _t->scene_root();
            if (_a[0]) *reinterpret_cast< Qt3DCore::QEntity**>(_a[0]) = std::move(_r); }  break;
        case 11: _t->exec_debug(); break;
        case 12: _t->handle_connect(); break;
        case 13: _t->handle_disconnect(); break;
        case 14: _t->handle_document_updated(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (State::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&State::connection_state_changed)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (State::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&State::open_call_method_page)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (State::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&State::method_result_recieved)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (State::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&State::method_error_recieved)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (State::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&State::debug_tree)) {
                *result = 4;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        auto *_t = static_cast<State *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< int*>(_v) = _t->connection_state(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        auto *_t = static_cast<State *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->set_connection_state(*reinterpret_cast< int*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

QT_INIT_METAOBJECT const QMetaObject State::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_State.data,
    qt_meta_data_State,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *State::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *State::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_State.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int State::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 15)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 15;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 15)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 15;
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 1;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void State::connection_state_changed(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void State::open_call_method_page()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void State::method_result_recieved(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void State::method_error_recieved(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void State::debug_tree()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
