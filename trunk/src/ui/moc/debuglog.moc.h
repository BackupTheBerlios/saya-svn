/****************************************************************************
** Meta object code from reading C++ file 'debuglog.cpp'
**
** Created: Sun Nov 7 00:46:52 2010
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'debuglog.cpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_AppDebugLog__Data[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
      19,   18,   18,   18, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_AppDebugLog__Data[] = {
    "AppDebugLog::Data\0\0OnIdle()\0"
};

const QMetaObject AppDebugLog::Data::staticMetaObject = {
    { &QTextEdit::staticMetaObject, qt_meta_stringdata_AppDebugLog__Data,
      qt_meta_data_AppDebugLog__Data, 0 }
};

const QMetaObject *AppDebugLog::Data::metaObject() const
{
    return &staticMetaObject;
}

void *AppDebugLog::Data::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_AppDebugLog__Data))
        return static_cast<void*>(const_cast< Data*>(this));
    return QTextEdit::qt_metacast(_clname);
}

int AppDebugLog::Data::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTextEdit::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: OnIdle(); break;
        default: ;
        }
        _id -= 1;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
