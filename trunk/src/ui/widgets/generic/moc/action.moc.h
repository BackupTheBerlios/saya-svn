/****************************************************************************
** Meta object code from reading C++ file 'action.h'
**
** Created: Wed Dec 8 08:32:37 2010
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'action.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_syAction[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
      10,    9,    9,    9, 0x0a,
      23,    9,    9,    9, 0x0a,
      44,   36,    9,    9, 0x0a,
      61,    9,    9,    9, 0x0a,
      76,   36,    9,    9, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_syAction[] = {
    "syAction\0\0sigchanged()\0sighovered()\0"
    "checked\0sigtoggled(bool)\0sigtriggered()\0"
    "sigtriggeredcheckable(bool)\0"
};

const QMetaObject syAction::staticMetaObject = {
    { &QAction::staticMetaObject, qt_meta_stringdata_syAction,
      qt_meta_data_syAction, 0 }
};

const QMetaObject *syAction::metaObject() const
{
    return &staticMetaObject;
}

void *syAction::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_syAction))
        return static_cast<void*>(const_cast< syAction*>(this));
    if (!strcmp(_clname, "has_slots"))
        return static_cast< has_slots*>(const_cast< syAction*>(this));
    return QAction::qt_metacast(_clname);
}

int syAction::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QAction::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: sigchanged(); break;
        case 1: sighovered(); break;
        case 2: sigtoggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: sigtriggered(); break;
        case 4: sigtriggeredcheckable((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 5;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
