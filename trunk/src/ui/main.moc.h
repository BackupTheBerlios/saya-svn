/****************************************************************************
** Meta object code from reading C++ file 'main.cpp'
**
** Created: Wed Jan 14 19:24:55 2009
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'main.cpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_AppFrame__Data[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
      26,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      16,   15,   15,   15, 0x0a,
      29,   15,   15,   15, 0x0a,
      43,   15,   15,   15, 0x0a,
      70,   15,   15,   15, 0x0a,
      96,   15,   15,   15, 0x0a,
     115,   15,   15,   15, 0x0a,
     128,   15,   15,   15, 0x0a,
     143,   15,   15,   15, 0x0a,
     160,   15,   15,   15, 0x0a,
     175,   15,   15,   15, 0x0a,
     190,   15,   15,   15, 0x0a,
     206,   15,   15,   15, 0x0a,
     227,   15,   15,   15, 0x0a,
     236,   15,   15,   15, 0x0a,
     246,   15,   15,   15, 0x0a,
     266,   15,   15,   15, 0x0a,
     288,   15,   15,   15, 0x0a,
     316,   15,   15,   15, 0x0a,
     337,   15,   15,   15, 0x0a,
     365,   15,   15,   15, 0x0a,
     395,   15,   15,   15, 0x0a,
     416,   15,   15,   15, 0x0a,
     440,   15,   15,   15, 0x0a,
     461,   15,   15,   15, 0x0a,
     486,   15,   15,   15, 0x0a,
     509,   15,   15,   15, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_AppFrame__Data[] = {
    "AppFrame::Data\0\0OnFileOpen()\0OnFileClose()\0"
    "OnClearRecentProjectList()\0"
    "OnClearRecentImportList()\0OnOpenRecentFile()\0"
    "OnFileSave()\0OnFileSaveAs()\0"
    "OnFileSaveCopy()\0OnNewProject()\0"
    "OnFileRevert()\0OnFileCapture()\0"
    "OnFileBatchCapture()\0OnQuit()\0OnAbout()\0"
    "OnSaveFrameLayout()\0OnLoadDefaultLayout()\0"
    "OnWorkspaceFactoryDefault()\0"
    "OnFileMenuUpdateUI()\0OnRecentFilesMenuUpdateUI()\0"
    "OnRecentImportsMenuUpdateUI()\0"
    "OnEditMenuUpdateUI()\0OnProjectMenuUpdateUI()\0"
    "OnClipMenuUpdateUI()\0OnSequenceMenuUpdateUI()\0"
    "OnMarkerMenuUpdateUI()\0OnWindowMenuUpdateUI()\0"
};

const QMetaObject AppFrame::Data::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_AppFrame__Data,
      qt_meta_data_AppFrame__Data, 0 }
};

const QMetaObject *AppFrame::Data::metaObject() const
{
    return &staticMetaObject;
}

void *AppFrame::Data::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_AppFrame__Data))
        return static_cast<void*>(const_cast< Data*>(this));
    if (!strcmp(_clname, "syEvtHandler"))
        return static_cast< syEvtHandler*>(const_cast< Data*>(this));
    return QObject::qt_metacast(_clname);
}

int AppFrame::Data::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: OnFileOpen(); break;
        case 1: OnFileClose(); break;
        case 2: OnClearRecentProjectList(); break;
        case 3: OnClearRecentImportList(); break;
        case 4: OnOpenRecentFile(); break;
        case 5: OnFileSave(); break;
        case 6: OnFileSaveAs(); break;
        case 7: OnFileSaveCopy(); break;
        case 8: OnNewProject(); break;
        case 9: OnFileRevert(); break;
        case 10: OnFileCapture(); break;
        case 11: OnFileBatchCapture(); break;
        case 12: OnQuit(); break;
        case 13: OnAbout(); break;
        case 14: OnSaveFrameLayout(); break;
        case 15: OnLoadDefaultLayout(); break;
        case 16: OnWorkspaceFactoryDefault(); break;
        case 17: OnFileMenuUpdateUI(); break;
        case 18: OnRecentFilesMenuUpdateUI(); break;
        case 19: OnRecentImportsMenuUpdateUI(); break;
        case 20: OnEditMenuUpdateUI(); break;
        case 21: OnProjectMenuUpdateUI(); break;
        case 22: OnClipMenuUpdateUI(); break;
        case 23: OnSequenceMenuUpdateUI(); break;
        case 24: OnMarkerMenuUpdateUI(); break;
        case 25: OnWindowMenuUpdateUI(); break;
        }
        _id -= 26;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
