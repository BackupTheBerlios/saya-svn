/**************************************************************************************
 * Name:      textbrowser.cpp
 * Purpose:   Implementation of widgets with new signal implementations.
 *            These widgets will allow us to declare new signals and slots
 *            without having to rely on Qt's MOC.
 * Authors:   Ricardo Garcia
 * Created:   2010-11-28
 * Modified:  2010-11-28
 * Copyright: Ricardo Garcia
 * License:   LGPL Licence version 3.0 or later
 **************************************************************************************/

#include "textbrowser.h"
#include "moc/textbrowser.moc.h"

syTextBrowser::syTextBrowser ( QWidget * parent) : QTextBrowser(parent) { connectFakeSlots(); }

void syTextBrowser::connectFakeSlots() {

    connect(this,SIGNAL(anchorClicked(const QUrl &)),SLOT(siganchorClicked (const QUrl &)));
    connect(this,SIGNAL(backwardAvailable (bool)),SLOT(sigbackwardAvailable (bool)));
    connect(this,SIGNAL(forwardAvailable (bool)),SLOT(sigforwardAvailable (bool)));
    connect(this,SIGNAL(highlighted(const QUrl &)),SLOT(sigurlhighlighted (const QUrl &)));
    connect(this,SIGNAL(highlighted(const QString &)),SLOT(sigstringhighlighted (const QString &)));
    connect(this,SIGNAL(historyChanged ()),SLOT(sighistoryChanged ()));
    connect(this,SIGNAL(sourceChanged (const QUrl &)),SLOT(sigsourceChanged (const QUrl &)));
    connect(this,SIGNAL(copyAvailable (bool)),SLOT(sigcopyAvailable (bool)));
    connect(this,SIGNAL(currentCharFormatChanged (const QTextCharFormat &)),SLOT(sigcurrentCharFormatChanged (const QTextCharFormat &)));
    connect(this,SIGNAL(cursorPositionChanged ()),SLOT(sigcursorPositionChanged ()));
    connect(this,SIGNAL(redoAvailable (bool)),SLOT(sigredoAvailable (bool)));
    connect(this,SIGNAL(selectionChanged ()),SLOT(sigselectionChanged ()));
    connect(this,SIGNAL(textChanged ()),SLOT(sigtextChanged ()));
    connect(this,SIGNAL(undoAvailable (bool)),SLOT(sigundoAvailable (bool)));
    connect(this,SIGNAL(customContextMenuRequested(const QPoint &)),SLOT(sigcustomContextMenuRequested(const QPoint &)), Qt::DirectConnection);
    connect(this,SIGNAL(destroyed(QObject*)),SLOT(sigobjdestroyed(QObject*)), Qt::DirectConnection);
    connect(this,SIGNAL(destroyed()),SLOT(sigdestroyed()), Qt::DirectConnection);
}
