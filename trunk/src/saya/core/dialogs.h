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

class syString;

enum syDIALOG_BUTTON_TYPES {
    syYES_DEFAULT       = 0x00,
    syYES               = 0x02,
    syOK                = 0x04,
    syNO                = 0x08,
    syYES_NO            = 0x0A,
    syCANCEL            = 0x10,
    syOK_CANCEL         = 0x14,
    syYES_NO_CANCEL     = 0x1A,
    syNO_DEFAULT        = 0x80
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
 *  @return The code corresponding to the button pressed. */
int syMessageBox(const syString& message, const syString& caption,unsigned int flags,void* parent);

/** Pops up an aggressive message box. For use with errors when starting up the application. */
void syErrorMessageBox(const syString& message);

/** Changes the statusbar of the main frame, showing a message. */
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
    private:
        class Data;
        Data* m_Data;
};

#endif
