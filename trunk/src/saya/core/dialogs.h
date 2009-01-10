/**************************************************************
 * Name:      core/dialogs.h
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Purpose:   Defines classes for GUI Dialogs boxes.
 *            The idea is to define a toolkit-agnostic wrapper
 *            for dialog boxes (which will be implemented in
 *            the ui/ path),minimizing toolkit-dependency.
 * Note:      Some (if not all) of the constants defined here
 *            have exactly the same values as their wxWidgets
 *            counterparts to simplify the UI code. This might
 *            change if/when the UI is reimplemented uising
 *            another toolkit, like QT.
 * Created:   2008-12-14
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   wxWindows License
 **************************************************************/

#ifndef core_dialogs_h
#define core_dialogs_h

#include "systring.h"

enum syDIALOG_BUTTON_TYPES {
    syYES_DEFAULT       = 0x00,
    syYES               = 0x02,
    syOK                = 0x04,
    syNO                = 0x08,
    syYES_NO            = 0x0A,
    syCANCEL            = 0x10,
    syOK_CANCEL         = 0x14,
    syYES_NO_CANCEL     = 0x1A,
    syNO_DEFAULT        = 0x80,
    syCANCEL_DEFAULT    = 0x80,
};

enum syDIALOG_ICON_TYPES {
    syICON_EXCLAMATION  = 0x100,
    syICON_WARNING      = 0x100,
    syICON_HAND         = 0x200,
    syICON_ERROR        = 0x200,
    syICON_STOP         = 0x200,
    syICON_QUESTION     = 0x400,
    syICON_INFORMATION  = 0x800,
    syICON_ASTERISK     = 0x800,
    syICON_MASK         = 0xF00
};

enum syFILEDIALOG_FLAGS {
    syFD_OPEN              = 0x001,
    syFD_SAVE              = 0x002,
    syFD_OVERWRITE_PROMPT  = 0x004,
    syFD_FILE_MUST_EXIST   = 0x010,
    syFD_MULTIPLE          = 0x020,
    syFD_CHANGE_DIR        = 0x080,
    syFD_PREVIEW           = 0x100
};

/** @brief Pops up a message dialog, waiting for user input.
 *  @return The code corresponding to the button pressed.
 *  @note This is a wrapper for syApp::MessageBox. @see syApp::MessageBox.
 */
int syMessageBox(const syString& message, const syString& caption = "MessageBox",unsigned int flags = syOK,void* parent = 0);

/** @brief Shows an plain, OS-friendly error message box. Use this in case you messed up with your UI-toolkit.
 *  @note This is a wrapper for syApp::ErrorMessageBox. @see syApp::ErrorMessageBox.
 */
void syErrorMessageBox(const syString& message);

/** @brief Changes the statusbar of the main frame, showing a message.
 *  @note This is a wrapper for syApp::LogStatus. @see syApp::LogStatus.
 */
void syLogStatus(const syString& message);

/** This is the class that returns from a File Dialog call. */
class syFileDialogResult {
    public:
        /** Constructor. */
        syFileDialogResult();

        /** Copy constructor. */
        syFileDialogResult(const syFileDialogResult& copy);

        /** Assignment operator. */
        syFileDialogResult& operator=(const syFileDialogResult& copy);

        /** Destructor. */
        ~syFileDialogResult();

        /** Result of the query. True for OK, false for Cancel.*/
        bool GetOKResult();

        /** Number of files chosen (can be more than one in case of multiple file dialogs). */
        unsigned int GetFileCount();

        /** Adds a file to the list. No files will be duplicated. */
        void AddFile(const syString& filename);

        /** Clears the file list. */
        void Clear();

        /** Returns the file corresponding to the given index number (zero-based). */
        const syString operator[](unsigned int idx);

        operator const syString() const;
    private:
        class Data;
        Data* m_Data;
};

/** @brief Shows a file selector dialog. This is a wrapper for syApp::FileSelector.
 *  @param message The message to show in the dialog.
 *  @param default_path The default path to choose the files from.
 *  @param default_filename The filename to choose by default.
 *  @param default_extension The extension that the filename will have by default.
 *  @param wildcard The filter used to show the files in the dialog.
 *  @param flags A combination of the flags listed in the syFILEDIALOG_FLAGS enum.
 *  @param parent The parent window of the dialog. void* type used for extensibility.
 *  @param x The dialog's starting x position. -1 = center.
 *  @param y The dialog's starting y position.
 */
syFileDialogResult syFileSelector(
    const syString& message,
    const syString& default_path = "",
    const syString& default_filename = "",
    const syString& default_extension = "",
    const syString& wildcard = "*.*",
    int flags = 0,
    void* parent = 0,
    int x = -1,
    int y = -1);

#endif
