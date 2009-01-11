/***************************************************************
 * Name:      welcomedlg.cpp
 * Purpose:   Implements the Welcome Dialog
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-05-26
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 **************************************************************/


#include "../saya/core/app.h"
#include "../saya/core/systring.h"
#include "../saya/core/events.h"
#include "../saya/core/intl.h"
#include "../saya/recentfileslist.h"
#include "../saya/projectmanager.h"

//
#include "welcomedlg.h"
#include <qtextbrowser.h>
#include <qpushbutton.h>
#include <qevent.h>
#include <qapplication.h>
#include <qdesktopwidget.h>

// These were the wxWidget events that we called whenever we choose an action.
// But how do we pass them to the main window in Qt?

extern int idFileOpen;
extern int idNewProject;
extern int syID_FILE1;
class WelcomeDialog::Data : public QObject {
    Q_OBJECT
    public:
        Data(WelcomeDialog* parent, syEvtHandler* eventhandler);
        void ConnectSignalsAndSlots();
        virtual ~Data();
        QWidget* m_Panel;
        void RefreshRecentFilesList();
        WelcomeDialog* m_Parent;
        syEvtHandler* m_EventHandler;

        // Objects in the parent widget
        QTextBrowser* m_Browser;
        QPushButton* m_NewButton;
        QPushButton* m_OpenButton;
        QPushButton* m_QuitButton;

    public slots:
        void OnNewProject();  // Called when the "New Project" button is clicked.
        void OnOpenProject(); // Called when the "Open Project" button is clicked.
        void OnQuitButton(); // Called when the "Quit" button is clicked.
        void OnLinkClicked(const QUrl& link);
};

WelcomeDialog::Data::Data(WelcomeDialog* parent, syEvtHandler* eventhandler) :
m_Parent(parent),
m_EventHandler(eventhandler),
m_NewButton(0),
m_OpenButton(0),
m_QuitButton(0)
{
}

void WelcomeDialog::Data::ConnectSignalsAndSlots() {
    if(m_NewButton)
        connect(m_NewButton, SIGNAL(clicked()), this, SLOT(OnNewProject()) );
    if(m_OpenButton)
        connect(m_OpenButton, SIGNAL(clicked()), this, SLOT(OnOpenProject()) );
    if(m_QuitButton)
        connect(m_QuitButton, SIGNAL(clicked()), this, SLOT(OnQuitButton()) );
    if(m_Browser) {
        m_Browser->setOpenLinks(false);
        connect(m_Browser, SIGNAL(anchorClicked(const QUrl &)), this, SLOT(OnLinkClicked(const QUrl &)));
    }
}

void WelcomeDialog::Data::RefreshRecentFilesList() {
    if(IsAppShuttingDown()) return;
    syString tmps;
    syString curfile;

    RecentFilesList& items = *(ProjectManager::Get()->GetRecentFiles());
    size_t i;
    for(i = 1; i <=9 && (i<= items.size()); ++i) {
        curfile = items.item(i);
        if(!curfile.empty()) {
            syString prjtitle = ProjectManager::Get()->GetOfflineProjectTitle(curfile);
            if(prjtitle.empty()) {
                prjtitle = _("(Untitled)");
            }
            tmps += syString::Format("<li><a href=\"sayarecent://%d\">%s</a><br /><small>%s</small></li>",i,prjtitle.c_str(),curfile.c_str());
        }
    }
    if(!tmps.empty()) {
        tmps = "<ol>" + tmps + "</ol>";
    }

    if(m_Browser) {
        m_Browser->setHtml(tmps);
    }
}

void WelcomeDialog::Data::OnNewProject() {
    if(IsAppShuttingDown()) return;
    syActionEvent evt(idNewProject);
    m_EventHandler->ProcessEvent(evt);
}

void WelcomeDialog::Data::OnOpenProject() {
    if(IsAppShuttingDown()) return;
    syActionEvent evt(idFileOpen);
    m_EventHandler->ProcessEvent(evt);
}

void WelcomeDialog::Data::OnQuitButton() {
    syApp::Get()->Exit(false);
}

void WelcomeDialog::Data::OnLinkClicked(const QUrl& link) {
    QString href = link.toString();
    int fileno = 1;
    if(href.startsWith("sayarecent://")) {
        href = href.mid(13,1);
        bool isok = false;
        fileno = href.toInt(&isok);
        if(isok) {
            syActionEvent tmpevent(syID_FILE1 + (fileno - 1));
            m_EventHandler->ProcessEvent(tmpevent);
        }
    }
}

WelcomeDialog::Data::~Data()
{
    m_Parent->m_Data = 0;
}

WelcomeDialog::WelcomeDialog(QWidget *parent) :
m_Data(new Data(this, dynamic_cast<syEvtHandler*>(parent)))
{
	setWindowFlags(Qt::Window);
	setAttribute(Qt::WA_QuitOnClose, false);
	setAttribute(Qt::WA_DeleteOnClose, false);
	setWindowTitle("Welcome to Saya!");

	#warning TODO: Set m_Data's members to point to WelcomeDialog's push buttons and HTML window.

    m_Data->ConnectSignalsAndSlots();
}

void WelcomeDialog::closeEvent(QCloseEvent *event) {
    event->accept();
    syApp::Get()->Exit(false);
}

WelcomeDialog::~WelcomeDialog() {
    delete m_Data;
    m_Data = 0;
}

void WelcomeDialog::showEvent(QShowEvent * event) {
    m_Data->RefreshRecentFilesList();

    // Center on screen
    QRect qRect(QApplication::desktop()->screenGeometry());
    int iXpos=(qRect.width()  - width()) /2;
    int iYpos=(qRect.height() - height()) /2;
    this->move(iXpos,iYpos);
}

//
//void WelcomeDialog::OnLinkClicked(wxHtmlLinkEvent& event) {
//    wxHtmlLinkInfo linkinfo = event.GetLinkInfo();
//    wxString href = linkinfo.GetHref();
//    long fileno = 1;
//    if(href.StartsWith(_T("sayarecent://"))) {
//        href = href.SubString(13,1);
//        if(href.ToLong(&fileno)) {
//            wxCommandEvent tmpevent(wxEVT_COMMAND_MENU_SELECTED,wxID_FILE1 + (fileno - 1));
//            m_Parent->ProcessEvent(tmpevent);
//        }
//    }
//}

#include "welcomedlg.moc.h"
