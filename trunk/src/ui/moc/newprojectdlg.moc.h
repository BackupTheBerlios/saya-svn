/****************************************************************************
** Meta object code from reading C++ file 'newprojectdlg.h'
**
** Created: Sat Apr 25 15:31:27 2009
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../newprojectdlg.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'newprojectdlg.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_NewProjectDlg[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      15,   14,   14,   14, 0x0a,
      40,   38,   14,   14, 0x0a,
      65,   14,   14,   14, 0x0a,
      94,   14,   14,   14, 0x0a,
     108,   14,   14,   14, 0x0a,
     120,   14,   14,   14, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_NewProjectDlg[] = {
    "NewProjectDlg\0\0OnAVSettingsUpdateUI()\0"
    "i\0OnPrjPresetsChanged(int)\0"
    "OnPrjSaveSettingsAsClicked()\0OnBrowseDir()\0"
    "OnPressOk()\0OnEditingFinished()\0"
};

const QMetaObject NewProjectDlg::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_NewProjectDlg,
      qt_meta_data_NewProjectDlg, 0 }
};

const QMetaObject *NewProjectDlg::metaObject() const
{
    return &staticMetaObject;
}

void *NewProjectDlg::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_NewProjectDlg))
        return static_cast<void*>(const_cast< NewProjectDlg*>(this));
    if (!strcmp(_clname, "Ui::new_project_dialog"))
        return static_cast< Ui::new_project_dialog*>(const_cast< NewProjectDlg*>(this));
    return QDialog::qt_metacast(_clname);
}

int NewProjectDlg::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: OnAVSettingsUpdateUI(); break;
        case 1: OnPrjPresetsChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: OnPrjSaveSettingsAsClicked(); break;
        case 3: OnBrowseDir(); break;
        case 4: OnPressOk(); break;
        case 5: OnEditingFinished(); break;
        }
        _id -= 6;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
