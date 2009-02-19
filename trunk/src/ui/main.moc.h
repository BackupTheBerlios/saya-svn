/****************************************************************************
** Meta object code from reading C++ file 'main.cpp'
**
** Created: Wed Feb 18 19:59:15 2009
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
      39,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      16,   15,   15,   15, 0x0a,
      29,   15,   15,   15, 0x0a,
      43,   15,   15,   15, 0x0a,
      70,   15,   15,   15, 0x0a,
      96,   15,   15,   15, 0x0a,
     116,   15,   15,   15, 0x0a,
     136,   15,   15,   15, 0x0a,
     156,   15,   15,   15, 0x0a,
     176,   15,   15,   15, 0x0a,
     196,   15,   15,   15, 0x0a,
     216,   15,   15,   15, 0x0a,
     236,   15,   15,   15, 0x0a,
     256,   15,   15,   15, 0x0a,
     276,   15,   15,   15, 0x0a,
     289,   15,   15,   15, 0x0a,
     304,   15,   15,   15, 0x0a,
     321,   15,   15,   15, 0x0a,
     336,   15,   15,   15, 0x0a,
     351,   15,   15,   15, 0x0a,
     367,   15,   15,   15, 0x0a,
     388,   15,   15,   15, 0x0a,
     397,   15,   15,   15, 0x0a,
     407,   15,   15,   15, 0x0a,
     427,   15,   15,   15, 0x0a,
     449,   15,   15,   15, 0x0a,
     477,   15,   15,   15, 0x0a,
     498,   15,   15,   15, 0x0a,
     526,   15,   15,   15, 0x0a,
     556,   15,   15,   15, 0x0a,
     577,   15,   15,   15, 0x0a,
     601,   15,   15,   15, 0x0a,
     622,   15,   15,   15, 0x0a,
     647,   15,   15,   15, 0x0a,
     670,   15,   15,   15, 0x0a,
     693,   15,   15,   15, 0x0a,
     723,  715,   15,   15, 0x0a,
     762,  752,  747,   15, 0x0a,
     786,   15,  747,   15, 0x2a,
     806,   15,   15,   15, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_AppFrame__Data[] = {
    "AppFrame::Data\0\0OnFileOpen()\0OnFileClose()\0"
    "OnClearRecentProjectList()\0"
    "OnClearRecentImportList()\0OnOpenRecentFile1()\0"
    "OnOpenRecentFile2()\0OnOpenRecentFile3()\0"
    "OnOpenRecentFile4()\0OnOpenRecentFile5()\0"
    "OnOpenRecentFile6()\0OnOpenRecentFile7()\0"
    "OnOpenRecentFile8()\0OnOpenRecentFile9()\0"
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
    "LoadAndSetFrameSize()\0showmsg\0"
    "SaveDefaultLayout(bool)\0bool\0firsttime\0"
    "LoadDefaultLayout(bool)\0LoadDefaultLayout()\0"
    "DoUpdateAppTitle()\0"
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
        case 4: OnOpenRecentFile1(); break;
        case 5: OnOpenRecentFile2(); break;
        case 6: OnOpenRecentFile3(); break;
        case 7: OnOpenRecentFile4(); break;
        case 8: OnOpenRecentFile5(); break;
        case 9: OnOpenRecentFile6(); break;
        case 10: OnOpenRecentFile7(); break;
        case 11: OnOpenRecentFile8(); break;
        case 12: OnOpenRecentFile9(); break;
        case 13: OnFileSave(); break;
        case 14: OnFileSaveAs(); break;
        case 15: OnFileSaveCopy(); break;
        case 16: OnNewProject(); break;
        case 17: OnFileRevert(); break;
        case 18: OnFileCapture(); break;
        case 19: OnFileBatchCapture(); break;
        case 20: OnQuit(); break;
        case 21: OnAbout(); break;
        case 22: OnSaveFrameLayout(); break;
        case 23: OnLoadDefaultLayout(); break;
        case 24: OnWorkspaceFactoryDefault(); break;
        case 25: OnFileMenuUpdateUI(); break;
        case 26: OnRecentFilesMenuUpdateUI(); break;
        case 27: OnRecentImportsMenuUpdateUI(); break;
        case 28: OnEditMenuUpdateUI(); break;
        case 29: OnProjectMenuUpdateUI(); break;
        case 30: OnClipMenuUpdateUI(); break;
        case 31: OnSequenceMenuUpdateUI(); break;
        case 32: OnMarkerMenuUpdateUI(); break;
        case 33: OnWindowMenuUpdateUI(); break;
        case 34: LoadAndSetFrameSize(); break;
        case 35: SaveDefaultLayout((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 36: { bool _r = LoadDefaultLayout((*reinterpret_cast< bool(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 37: { bool _r = LoadDefaultLayout();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 38: DoUpdateAppTitle(); break;
        }
        _id -= 39;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
