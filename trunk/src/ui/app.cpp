/***************************************************************
 * Name:      ui/app.cpp
 * Purpose:   Code for Application Class
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-04-30
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 **************************************************************/

#include <qapplication.h>
#include <qmainwindow.h>
#include <qstatusbar.h>
#include <qmessagebox.h>
#include <qfiledialog.h>
#include <qstringlist.h>
#include <qtextcodec.h>
#include <qresource.h>

#include <saya/core/iocommon.h>
#include <saya/core/sentryfuncs.h>
#include <saya/core/systring.h>
#include <saya/core/intl.h>
#include <saya/core/config.h>
#include <saya/core/dialogs.h>
#include <saya/core/eventqueue.h>
#include <saya/core/avdevice.h>
#include <saya/playbackmanager.h>
#include <saya/projectmanager.h>

#include "app.h"
#include "resources.h"
#include "debuglog.h"
#include "config.h"

int idsyEventpassed = QEvent::registerEventType();

class qsyEvent : public QEvent {
    public:
        qsyEvent(QEvent::Type type) : QEvent(type) {}
        Type type() const { return static_cast<Type>(idsyEventpassed); }
        virtual ~qsyEvent();
};

class QMainWindow;
extern QMainWindow* CreateMainFrame();

class qMyApp : public QApplication {
    public:
        qMyApp(int & argc, char ** argv ) : QApplication(argc, argv) {}
    protected:
        virtual void customEvent(QEvent *event);
};

void qMyApp::customEvent(QEvent *event) {
    if(event && static_cast<int>(event->type()) == idsyEventpassed) {
        syEvtQueue::ProcessNextEvent();
    }
}

const char* APP_NAME = "SayaVideoEditor";
const char* APP_VENDOR = "Rick Garcia";
const char* APP_SHOWNAME = "Saya";
const char* APP_SHOWOFFNAME = "SayaVE Ain't Yet Another Video Editor";
syString APP_DIR;
syString APP_FILENAME;
// ---------------------
// begin qsyApp::Data
// ---------------------

class qsyApp::Data {
    public:
        Data();
        ~Data();
        bool LoadResources();
        bool LoadResource(const syString& filename);
        syDebugLog* m_DebugLog;
        void* m_TopWindow;
        qMyApp* m_App;
        volatile bool m_MainLoopRunning;
};

qsyApp::Data::Data() :
m_DebugLog(0),
m_TopWindow(0),
m_App(0),
m_MainLoopRunning(false)
{
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    QCoreApplication::setOrganizationName(APP_VENDOR);
    QCoreApplication::setApplicationName(APP_NAME);
}

qsyApp::Data::~Data() {
    PlaybackManager::Unload();
    ProjectManager::Unload();
    delete m_App;
    m_App = 0;
}

bool qsyApp::Data::LoadResource(const syString& filename) {
    syString fullresource = ResourcesPath + filename;
    if(!QResource::registerResource(fullresource)) {
        ioCommon::Print(syString::Format("Error loading resource: '%s'. Aborting...\n",filename.c_str()));
        return false;
    }
    return true;
}

bool qsyApp::Data::LoadResources() {
    bool result = false;
    do {
        if(!LoadResource("welcomedlg.rcc")) break;
        if(!LoadResource("playbackcontrol.rcc")) break;
        result = true;
    }while(false);
    return result;
}

// -------------------
// end qsyApp::Data
// -------------------

// ---------------
// begin qsyApp
// ---------------

qsyApp::qsyApp(int argc, char** argv) :
syApp(argc, argv),
m_Data(new Data)
{
    m_Data->m_App = new qMyApp(m_argc, m_argv);
}

qsyApp::~qsyApp() {
    delete m_Data;
}

const char* qsyApp::GetApplicationName() const {
    return APP_NAME;
}

const char* qsyApp::GetApplicationDisplayName() const {
    return APP_SHOWNAME;
}

const char* qsyApp::GetApplicationVendor() const {
    return APP_VENDOR;
}

const char* qsyApp::GetApplicationShowOffName() const {
    return APP_SHOWOFFNAME;
}

const char* qsyApp::GetApplicationPath() const {
    return APP_DIR.c_str();
}

const char* qsyApp::GetApplicationFilename() const {
    return APP_FILENAME.c_str();
}

syConfig* qsyApp::CreateConfig() const {
    return new QsyConfig(GetApplicationName());
}

syDebugLog* qsyApp::CreateDebugLog() const {
    syDebugLog* log = m_Data->m_DebugLog = new AppDebugLog;
    return log;
}

bool qsyApp::OnInit() {
    bool result = false;
    APP_DIR = QCoreApplication::applicationDirPath();
    APP_FILENAME = QCoreApplication::applicationFilePath();

    do {
        // Init Project Manager and Playback Manager.
        DebugLog(_("Initializing Playback Manager..."));
        PlaybackManager::Get();

        DebugLog(_("Initializing Project Manager..."));
        ProjectManager::Get();

        DebugLog(_("Initializing Resources path..."));
        syInitResourcesPaths();

        if(!ProjectManager::Get()->LoadConfig()) {
            ErrorMessageBox(_("WARNING: Could not read configuration!"));
        }


//        DebugLog(_("Initializing File system handlers..."));
//        wxFileSystem::AddHandler(new wxZipFSHandler);
//        wxFileSystem::AddHandler(new wxMemoryFSHandler);
//        wxImage::AddHandler(new wxPNGHandler);
//        DebugLog(_("Initializing XML Resource handlers..."));
//        wxXmlResource::Get()->InitAllHandlers();

//        DebugLog(_("Initializing Image handlers..."));
//        wxInitAllImageHandlers();

        DebugLog(_("Loading resources..."));
        if(!m_Data->LoadResources()) {
            break;
        }

        DebugLog(_("Creating main frame..."));
        CreateMainFrame();

        DebugLog(_("Initialization finished."));
        result = true;
    }while(false);
	return result;
}

/** Exits the main loop on the next iteration. */
void qsyApp::Exit(bool now) {
    AVDevice::ShutDownAll();
    if(qApp) {
        if(now) {
            QCoreApplication::removePostedEvents (qApp);
            DebugLog(_("Good bye."));
        }
        qApp->exit();
    }
}

void qsyApp::OnExit() {
    ProjectManager::Get()->SaveConfig();
    ProjectManager::Get()->CloseProject(true);
    // The debug log is a QWidget, and it must be deleted before ~QApplication() is called. So we do it here.
    delete m_Data->m_DebugLog;
    m_Data->m_DebugLog = 0;
}

void qsyApp::Run() {
    syBoolSetter setter(m_Data->m_MainLoopRunning, true);
    Result = m_Data->m_App->exec();
}

int qsyApp::MessageBox(const syString& message, const syString& caption,unsigned int flags,void* parent) const {
    int qtbuttonflags = 0;
    QMessageBox::StandardButton defaultbutton = QMessageBox::Ok;

    // Sets the button flags
    if(flags & syOK) {
        qtbuttonflags |= QMessageBox::Ok;
        defaultbutton = QMessageBox::Ok;
    }
    if(flags & syCANCEL) { qtbuttonflags |= QMessageBox::Cancel; }
    if(flags & syYES) {
        qtbuttonflags |= QMessageBox::Yes;
        if(!defaultbutton) defaultbutton = QMessageBox::Yes;
    }
    if(flags & syNO) { qtbuttonflags |= QMessageBox::No; }

    // Sets the default button temporary variable
    if((flags & syNO_DEFAULT) && (flags & syNO) && !(flags & syCANCEL)) {
        defaultbutton = QMessageBox::No;
    } else if((flags & syCANCEL_DEFAULT) && (flags & syCANCEL)) {
            defaultbutton = QMessageBox::Cancel;
    }

    QMessageBox::Icon theicon = QMessageBox::NoIcon;
    switch(flags & syICON_MASK) {
        case syICON_EXCLAMATION:
            theicon = QMessageBox::Warning;
        break;
        case syICON_ERROR:
            theicon = QMessageBox::Critical;
        break;
        case syICON_QUESTION:
            theicon = QMessageBox::Question;
        break;
        case syICON_INFORMATION:
            theicon = QMessageBox::Information;
        break;
        default:;
    }

    // Create the message box
    QMessageBox msgbox(theicon, caption.c_str(), message.c_str(), (QMessageBox::StandardButtons)qtbuttonflags,
        static_cast<QWidget*>(parent));
    msgbox.setDefaultButton(defaultbutton);

    // And run it.
    int qret = msgbox.exec();
    int ret = syOK;

    switch(qret) {
        case QMessageBox::Ok:
            ret = syOK;
            break;
        case QMessageBox::Cancel:
            ret = syCANCEL;
            break;
        case QMessageBox::Yes:
            ret = syYES;
            break;
        case QMessageBox::No:
            ret = syNO;
            break;
    }

    return ret;
}

void qsyApp::ErrorMessageBox(const syString& message) const {
    qsyApp::MessageBox(message.c_str(), "ERROR",syOK | syICON_ERROR, 0);
}

void qsyApp::LogStatus(const syString& message) const {
    QWidget* w = static_cast<QWidget*>(GetTopWindow());
    QMainWindow* mw = dynamic_cast<QMainWindow*>(w);
    if(mw) {
        QStatusBar* sb = mw->statusBar();
        if(sb) {
            sb->showMessage(message);
        }
    }
}


syFileDialogResult qsyApp::FileSelector(
    const syString& message,
    const syString& default_path,
    const syString& default_filename,
    const syString& default_extension,
    const syString& wildcard,
    int flags,
    void* parent,
    int x,
    int y) const
{
    syFileDialogResult result;
    QFileDialog mydialog(0, message.c_str(), default_path.c_str(), wildcard.c_str());
    mydialog.setDefaultSuffix(default_extension.c_str());

    // Sets accept mode
    QFileDialog::AcceptMode acceptmode = QFileDialog::AcceptOpen;
    if(flags & syFD_SAVE) {
        acceptmode = QFileDialog::AcceptSave;
    }

    // Sets file mode
    QFileDialog::FileMode filemode = QFileDialog::AnyFile;
    if(flags & syFD_CHANGE_DIR) {
        filemode = QFileDialog::Directory;
    } else if(flags & syFD_FILE_MUST_EXIST) {
        if(flags & syFD_MULTIPLE) {
            filemode = QFileDialog::ExistingFiles;
        } else {
            filemode = QFileDialog::ExistingFile;
        }
    }

    mydialog.setAcceptMode(acceptmode);
    mydialog.setFileMode(filemode);
    if(mydialog.exec()) {
        QStringList files = mydialog.selectedFiles();

        for(QStringList::iterator i = files.begin(); i != files.end(); ++i) {
            result.AddFile(i->toUtf8().data ());
        }
    }
    return result;
}

void qsyApp::SetTopWindow(void* window) {
    m_Data->m_TopWindow = window;
}

void* qsyApp::GetTopWindow() const {
    return m_Data->m_TopWindow;
}

bool qsyApp::IsMainLoopRunning() const {
    return m_Data->m_MainLoopRunning;
}

void qsyApp::PostEvent(syEvtHandler* handler, syEvent& event) {
    if(!handler || IsAppShuttingDown()) return;
    syEvtQueue::PostEvent(handler, event);
    QEvent* tmpevent = new QEvent(static_cast<QEvent::Type>(idsyEventpassed));
    QCoreApplication::postEvent(qApp, tmpevent);
}

/** Wakes up the main thread to begin event processing. */
void qsyApp::WakeUpIdle() {
    // Qt doesn't need to wake up the main thread.
}

// -------------
// end qsyApp
// -------------
