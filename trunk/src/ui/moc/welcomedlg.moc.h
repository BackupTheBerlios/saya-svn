/****************************************************************************
** Meta object code from reading C++ file 'welcomedlg.cpp'
**
** Created: Sun Nov 7 00:46:52 2010
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'welcomedlg.cpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_WelcomeDialog__Data[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
      21,   20,   20,   20, 0x0a,
      36,   20,   20,   20, 0x0a,
      52,   20,   20,   20, 0x0a,
      72,   67,   20,   20, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_WelcomeDialog__Data[] = {
    "WelcomeDialog::Data\0\0OnNewProject()\0"
    "OnOpenProject()\0OnQuitButton()\0link\0"
    "OnLinkClicked(QUrl)\0"
};

const QMetaObject WelcomeDialog::Data::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_WelcomeDialog__Data,
      qt_meta_data_WelcomeDialog__Data, 0 }
};

const QMetaObject *WelcomeDialog::Data::metaObject() const
{
    return &staticMetaObject;
}

void *WelcomeDialog::Data::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_WelcomeDialog__Data))
        return static_cast<void*>(const_cast< Data*>(this));
    return QObject::qt_metacast(_clname);
}

int WelcomeDialog::Data::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: OnNewProject(); break;
        case 1: OnOpenProject(); break;
        case 2: OnQuitButton(); break;
        case 3: OnLinkClicked((*reinterpret_cast< const QUrl(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 4;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
