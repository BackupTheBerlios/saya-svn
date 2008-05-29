#ifndef PROJECTMANAGER_H
#define PROJECTMANAGER_H

/***************************************************************
 * Name:      projectmanager.h
 * Purpose:   Definition for the Project Manager class
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-04-30
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 **************************************************************/

#include "vidproject.h"
#include <deque>
#include <wx/wx.h>
using namespace std;

/** Tells whether the application is shutting down. */
bool IsAppShuttingDown();

/** Sets the flag that forbids a new Project Manager instance to be created. */
void ShutDownApp();

extern const wxString APP_NAME; /** The application's name. */
extern const wxString APP_VENDOR; /** The application vendor's name (me) */
extern const wxString APP_SHOWNAME; /** The application's official name */
extern const wxString APP_SHOWOFFNAME; /** The application's name and tagline for showing off */

extern int idProjectStatusChanged; /** Used to send an UpdateUI event to the mainframe, to change the frame's title. */


/** @class ProjectManager
  * @brief The main class that handles project loading and saving.
  *
  * ProjectManager is the main class that handles projects and opening of files.
  * It creates, loads, saves project, and is the main interface between the editing
  * framework and the User interface.
  *
  */
class ProjectManager
{
    private:
        /** @brief Standard constructor.
          *
          * The constructor is private and can only be called by the static function "Get".
          */
        ProjectManager();

    public:

        /** Standard destructor. */
        virtual ~ProjectManager();

        bool LoadProject(const wxString filename);
        /** @brief Loads a project from disk.
          *
          * @param filename The project's filename.
          * @return True on success, false otherwise.
          */
        bool LoadRecentProject(int fileno);

        /** @brief Saves the project.
          * @return True on success, false otherwise.
          */
        bool SaveProject();

        /** @brief Saves the project under a new filename.
          *
          * @param filename The New filename for the project.
          * @return True on success, false otherwise.
          */
        bool SaveProjectAs(const wxString filename);

        /** @brief Saves a copy of the project under the specified filename.
          *
          * @param filename the filename under which the project is to be saved.
          * @return True on success, false otherwise.
          * @note Save Project copy does not alter the project's unsaved status!
          */
        bool SaveProjectCopy(const wxString filename);

        /** @brief Saves file, or shows the "Save As..." dialog to the user if the project is new.
          * @note All errors are handled and if the file cannot be saved, options are given to the user.
          */
        bool InteractiveSaveProject();

        /**  @brief Shows the "Save As..." dialog to the user and saves the file.
          *
          * @note All errors are handled and if the file cannot be saved, options are given to the user.
          */
        bool InteractiveSaveProjectAs();

        /** @brief Shows the "Save Project Copy" dialog to the user.
          * @note All errors are handled and if the file cannot be saved, options are given to the user.
          */
        bool InteractiveSaveProjectCopy();

        /** @brief Closes the current project.
          *
          * @param force Tells whether the action must be done; set to false to allow user to cancel the operation.
          * @return True on success; false otherwise.
          * If force = false, the user will be asked to save the project in a Yes/No/Cancel dialog.
          * If user chooses Cancel, the project will NOT be closed.
          */
        bool CloseProject(bool force = false);

        /** @brief Loads configuration from disk.
          *
          * @return True on success; false otherwise.
          */
        bool LoadConfig();

        /** Saves configuration on disk.
          *
          * @return True on success; false otherwise.
          */
        bool SaveConfig();

        /** Gets the value of the "Clear Undo History on Save" flag. */
        bool GetClearUndoHistoryOnSave();

        /** Sets the value of the "Clear Undo History on Save" flag. */
        void SetClearUndoHistoryOnSave(bool flag);

        /** @brief sets the pointer for the program's main frame (window)
          *
          * Call this from your main program to specify which frame will
          * receive the events.
          */
        void SetMainFrame(wxFrame* frame);

        /** Gets the last used project directory */
        const wxString GetLastProjectDir();

        // Recent Projects and Imported files

        /** @brief Adds a filename to the Recent Projects list.
          *
          * @param s The filename to be added to the list.
          * @param fromthebeginning Tells whether to prepend the file at the beginning, or append it to the end.
          */
        void AddToRecentFiles(const wxString& s,bool fromthebeginning = true);

        /** @brief Adds a filename to the Recently Imported files list.
          *
          * @param s The filename to be added to the list.
          * @param fromthebeginning Tells whether to prepend the file at the beginning, or append it to the end.
          */
        void AddToRecentImports(const wxString& s,bool fromthebeginning = true);

        /** @brief Gets the filename for the nth recently opened project.
          *
          * @param fileno The index of the filename to retrieve (from 1 to 9)
          * @return The filename corresponding to the nth entry in the Recent Projects list.
          */
        const wxString GetRecentProjectName(int fileno);

        /** @brief Gets an offline project's Title
          * A proxy for VidProject::GetOfflineProjectTitle
          * @param filename The filename of the project
          * @return The project's title
          */
        const wxString GetOfflineProjectTitle(const wxString& filename);

        /** @brief Gets the filename for the nth recently imported file.
          *
          * @param fileno The index of the filename to retrieve (from 1 to 9)
          * @return The filename corresponding to the nth entry in the Recently Imported files list.
          */
        wxString GetRecentImportName(int filenmo);

        /** Clears the Recent Projects file list. */
        void ClearRecentFiles();

        /** Clears the Recent Imports file list. */
        void ClearRecentImports();

        /** @brief Tells whenever the Recent Opened projects list has been modified.
          *
          * Used and reset by the main frame to update the recent projects submenu.
          */
        bool m_recentfilesmodified;

        /** @brief Tells whenever the Recent Imported files list has been modified.
          *
          * Used and reset by the main frame to update the recent imports submenu.
          */
        bool m_recentimportsmodified;

        /** A list of the most recently opened project files. */
        deque<wxString> m_recentfiles;

        /** A list of the most recently imported clips. */
        deque<wxString> m_recentimports;

        /** @brief Gets the current project manager instance, or creates one.
          *
          * ProjectManager is a singleton, it means only one instance can be
          * active at a time. The static functions Get() and Unload() deal with
          * the creation and destruction of the instance, respectively.
          */
        static ProjectManager* Get();

        /** @brief Unloads the Project Manager from memory.
          * @warning This must be done ONLY when the program's about to exit!
          */
        static void Unload();

        /** Gets the pointer for the active project. */
        VidProject* GetProject();

        /** Tells whether there's an active project or not. */
        bool HasProject();

        /** Stores the last program error */
        wxString m_lasterror;

        /** Tells whether to clear the Undo History after the project's successfully saved. */
        bool m_clearundohistoryonsave;

        /** Called whenever the project's status or filename have changed. */
        void OnProjectStatusModified();

    protected:
    private:
        /** The currently opened project */
        VidProject* m_project;

        /** The last used project directory */
        wxString m_LastProjectDir;

        /** A pointer to the program's main frame */
        wxFrame* m_MainFrame;
};

#endif // PROJECTMANAGER_H
