/****************************************************************************
** Meta object code from reading C++ file 'Editor.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../Editor.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Editor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Editor_t {
    QByteArrayData data[17];
    char stringdata0[186];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Editor_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Editor_t qt_meta_stringdata_Editor = {
    {
QT_MOC_LITERAL(0, 0, 6), // "Editor"
QT_MOC_LITERAL(1, 7, 15), // "slotItemPressed"
QT_MOC_LITERAL(2, 23, 0), // ""
QT_MOC_LITERAL(3, 24, 16), // "QListWidgetItem*"
QT_MOC_LITERAL(4, 41, 4), // "item"
QT_MOC_LITERAL(5, 46, 15), // "slotItemClicked"
QT_MOC_LITERAL(6, 62, 16), // "QTreeWidgetItem*"
QT_MOC_LITERAL(7, 79, 6), // "column"
QT_MOC_LITERAL(8, 86, 16), // "slotValueChanged"
QT_MOC_LITERAL(9, 103, 11), // "QtProperty*"
QT_MOC_LITERAL(10, 115, 4), // "prop"
QT_MOC_LITERAL(11, 120, 3), // "val"
QT_MOC_LITERAL(12, 124, 13), // "slotOpenScene"
QT_MOC_LITERAL(13, 138, 13), // "slotSaveScene"
QT_MOC_LITERAL(14, 152, 11), // "renderPoint"
QT_MOC_LITERAL(15, 164, 10), // "renderLine"
QT_MOC_LITERAL(16, 175, 10) // "renderFace"

    },
    "Editor\0slotItemPressed\0\0QListWidgetItem*\0"
    "item\0slotItemClicked\0QTreeWidgetItem*\0"
    "column\0slotValueChanged\0QtProperty*\0"
    "prop\0val\0slotOpenScene\0slotSaveScene\0"
    "renderPoint\0renderLine\0renderFace"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Editor[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   54,    2, 0x0a /* Public */,
       5,    2,   57,    2, 0x0a /* Public */,
       8,    2,   62,    2, 0x0a /* Public */,
      12,    0,   67,    2, 0x0a /* Public */,
      13,    0,   68,    2, 0x0a /* Public */,
      14,    0,   69,    2, 0x0a /* Public */,
      15,    0,   70,    2, 0x0a /* Public */,
      16,    0,   71,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 6, QMetaType::Int,    4,    7,
    QMetaType::Void, 0x80000000 | 9, QMetaType::QVariant,   10,   11,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void Editor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Editor *_t = static_cast<Editor *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->slotItemPressed((*reinterpret_cast< QListWidgetItem*(*)>(_a[1]))); break;
        case 1: _t->slotItemClicked((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 2: _t->slotValueChanged((*reinterpret_cast< QtProperty*(*)>(_a[1])),(*reinterpret_cast< const QVariant(*)>(_a[2]))); break;
        case 3: _t->slotOpenScene(); break;
        case 4: _t->slotSaveScene(); break;
        case 5: _t->renderPoint(); break;
        case 6: _t->renderLine(); break;
        case 7: _t->renderFace(); break;
        default: ;
        }
    }
}

const QMetaObject Editor::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_Editor.data,
      qt_meta_data_Editor,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *Editor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Editor::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Editor.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int Editor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 8;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
