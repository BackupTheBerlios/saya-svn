/****************************************************************************
** Meta object code from reading C++ file 'playbackcontrol.h'
**
** Created: Tue Feb 24 21:09:44 2009
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../playbackcontrol.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'playbackcontrol.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_PlaybackControl[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      17,   16,   16,   16, 0x05,
      38,   16,   16,   16, 0x05,
      59,   16,   16,   16, 0x05,
      83,   16,   16,   16, 0x05,
      98,   16,   16,   16, 0x05,
     118,   16,   16,   16, 0x05,
     140,   16,   16,   16, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_PlaybackControl[] = {
    "PlaybackControl\0\0playbackFirstFrame()\0"
    "playbackFastRewind()\0playbackPreviousFrame()\0"
    "playbackPlay()\0playbackNextFrame()\0"
    "playbackFastForward()\0playbackLastFrame()\0"
};

const QMetaObject PlaybackControl::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_PlaybackControl,
      qt_meta_data_PlaybackControl, 0 }
};

const QMetaObject *PlaybackControl::metaObject() const
{
    return &staticMetaObject;
}

void *PlaybackControl::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_PlaybackControl))
        return static_cast<void*>(const_cast< PlaybackControl*>(this));
    return QWidget::qt_metacast(_clname);
}

int PlaybackControl::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: playbackFirstFrame(); break;
        case 1: playbackFastRewind(); break;
        case 2: playbackPreviousFrame(); break;
        case 3: playbackPlay(); break;
        case 4: playbackNextFrame(); break;
        case 5: playbackFastForward(); break;
        case 6: playbackLastFrame(); break;
        }
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void PlaybackControl::playbackFirstFrame()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void PlaybackControl::playbackFastRewind()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void PlaybackControl::playbackPreviousFrame()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void PlaybackControl::playbackPlay()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}

// SIGNAL 4
void PlaybackControl::playbackNextFrame()
{
    QMetaObject::activate(this, &staticMetaObject, 4, 0);
}

// SIGNAL 5
void PlaybackControl::playbackFastForward()
{
    QMetaObject::activate(this, &staticMetaObject, 5, 0);
}

// SIGNAL 6
void PlaybackControl::playbackLastFrame()
{
    QMetaObject::activate(this, &staticMetaObject, 6, 0);
}
QT_END_MOC_NAMESPACE
