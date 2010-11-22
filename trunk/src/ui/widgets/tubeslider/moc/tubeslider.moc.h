/****************************************************************************
** Meta object code from reading C++ file 'tubeslider.h'
**
** Created: Sun Nov 21 19:15:17 2010
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'tubeslider.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_TubeSlider[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // signals: signature, parameters, type, tag, flags
      17,   12,   11,   11, 0x05,

 // slots: signature, parameters, type, tag, flags
      50,   37,   11,   11, 0x0a,
      78,   12,   11,   11, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_TubeSlider[] = {
    "TubeSlider\0\0time\0TimeChanged(double)\0"
    "start,finish\0setTimeRange(double,double)\0"
    "setCurrentTime(double)\0"
};

const QMetaObject TubeSlider::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_TubeSlider,
      qt_meta_data_TubeSlider, 0 }
};

const QMetaObject *TubeSlider::metaObject() const
{
    return &staticMetaObject;
}

void *TubeSlider::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_TubeSlider))
        return static_cast<void*>(const_cast< TubeSlider*>(this));
    return QWidget::qt_metacast(_clname);
}

int TubeSlider::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: TimeChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 1: setTimeRange((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        case 2: setCurrentTime((*reinterpret_cast< double(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void TubeSlider::TimeChanged(double _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
