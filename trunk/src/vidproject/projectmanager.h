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

/** Tells whether the application is shutting down. */
bool IsAppShuttingDown();

/** Sets the flag that forbids a new Project Manager instance to be created. */
void ShutDownApp();

extern const std::string APP_NAME; /** The application's name. */
extern const std::string APP_VENDOR; /** The application vendor's name (that would be moi) */
extern const std::string APP_SHOWNAME; /** The application's official name */
extern const std::string APP_SHOWOFFNAME; /** The application's name and tagline for showing off */

/** Used for UI-toolkit-independent event handling */
enum sayaEventType {
    sayaevt_DoNothing = 0,
    sayaevt_ProjectStatusChanged
};

/** Used for Yes/No/Cancel dialogs */
enum sayaYesNoCancel {
    sayaYes = 0,
    sayaNo,
    sayaCancel
};

/** @brief Abstract UI-toolkit-independent Event Handler.
  *
  * In an effort to make Saya as independent as possible regarding UI Toolkits, we're using
  * a wrapper class for your main event handler to deal with user interaction.
  * All strings passed and received are ANSI strings in multibyte format.
  * @note To receive project-related events, you must call ProjectMananger::SetEventHandler.
  */
class sayaEvtHandler {
    public:
        /** Standard constructor */
        sayaEvtHandler() {}

        /** Standard destructor */
        virtual ~sayaEvtHandler() {}

        /** Event processing function */
        virtual void ProcessSayaEvent(sayaEventType id, void* data = NULL) = 0;

        /** Shows an Error message box */
        virtual void ErrorMessageBox(const char* msg,const char* caption) = 0;

        /** Shows a yes/no message box and returns the answer (true = yes, false = no) */
        virtual bool YesNoMessageBox(const char* msg,const char* caption,bool exclamation) = 0;

        /** Shows a yes/no/cancel message box and returns the answer */
        virtual sayaYesNoCancel YesNoCancelMessageBox(const char* msg,const char* caption,bool exclamation) = 0;

        /** Shows the "Save Project As" dialog */
        virtual std::string ShowDialogSaveProjectAs() = 0;

        /** Shows the "Save Project Copy As" dialog */
        virtual std::string ShowDialogSaveProjectCopyAs() = 0;

};

/** Abstract Cross-platform Configuration class.
  *
  * @note All strings passed and received are ANSI strings in multibyte format.
  */
class sayaConfig {
    public:

        /** Standard constructor */
        sayaConfig(std::string application_name) {}

        /** Standard destructor */
        virtual ~sayaConfig() {};

        /** @brief Reads a string configuration value.
          *
          * @param key The key to read the configuration value from.
          * @param defaultvalue The default value to return, if the value is not found.
          * @return The value of the read configuration.
          */
        virtual std::string Read(const std::string& key, const std::string& defaultvalue) = 0;

        /** @brief Writes a string configuration value.
          *
          * @param key The key to write the configuration value into.
          * @param value The value to write.
          * @return true on success; false otherwise.
          */
        virtual bool Write(const std::string& key, const std::string& value) = 0;

        /** @brief Checks if a configuration key exists.
          *
          * @param key The key to search in the config.
          * @return true if the key exists; false otherwise.
          */
        virtual bool Exists(const std::string& key) = 0;
};

/** Abstract Cross-platform Configuration Provider class.
  *
  * This class creates a Configuration object for you to use.
  * @note All strings passed and received are ANSI strings in multibyte format.
  */
class sayaConfigProvider {
    public:
        /** Standard Constructor */
        sayaConfigProvider() {};

        /** Standard Destructor */
        virtual ~sayaConfigProvider() {};

        virtual sayaConfig* Create(const std::string application_name) = 0;
};


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

        bool LoadProject(const std::string filename);
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
        bool SaveProjectAs(const std::string filename);

        /** @brief Saves a copy of the project under the specified filename.
          *
          * @param filename the filename under which the project is to be saved.
          * @return True on success, false otherwise.
          * @note Save Project copy does not alter the project's unsaved status!
          */
        bool SaveProjectCopy(const std::string filename);

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
        void SetEventHandler(sayaEvtHandler* handler);

        /** Sets the Configuration provider.
          *
          * @note You MUST call this method from your program after creating the project manager.
          */
        void SetConfigProvider(sayaConfigProvider* provider);

        /** Gets the last used project directory */
        const std::string GetLastProjectDir();

        // Recent Projects and Imported files

        /** @brief Adds a filename to the Recent Projects list.
          *
          * @param s The filename to be added to the list.
          * @param fromthebeginning Tells whether to prepend the file at the beginning, or append it to the end.
          */
        void AddToRecentFiles(const std::string& s,bool fromthebeginning = true);

        /** @brief Adds a filename to the Recently Imported files list.
          *
          * @param s The filename to be added to the list.
          * @param fromthebeginning Tells whether to prepend the file at the beginning, or append it to the end.
          */
        void AddToRecentImports(const std::string& s,bool fromthebeginning = true);

        /** @brief Gets the filename for the nth recently opened project.
          *
          * @param fileno The index of the filename to retrieve (from 1 to 9)
          * @return The filename corresponding to the nth entry in the Recent Projects list.
          */
        const std::string GetRecentProjectName(int fileno);

        /** @brief Gets an offline project's Title
          * A proxy for VidProject::GetOfflineProjectTitle
          * @param filename The filename of the project
          * @return The project's title
          */
        const std::string GetOfflineProjectTitle(const std::string& filename);

        const std::string GetOfflineProjectTitle(const char* filename);

        /** @brief Gets the filename for the nth recently imported file.
          *
          * @param fileno The index of the filename to retrieve (from 1 to 9)
          * @return The filename corresponding to the nth entry in the Recently Imported files list.
          */
        std::string GetRecentImportName(int filenmo);

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
        std::deque<std::string> m_recentfiles;

        /** A list of the most recently imported clips. */
        std::deque<std::string> m_recentimports;

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
        std::string m_lasterror;

        /** Tells whether to clear the Undo History after the project's successfully saved. */
        bool m_clearundohistoryonsave;

        /** Called whenever the project's status or filename have changed. */
        void OnProjectStatusModified();

    protected:
    private:
        /** The currently opened project */
        VidProject* m_project;

        /** The last used project directory */
        std::string m_lastprojectdir;

        /** A pointer to the program's event handler */
        sayaEvtHandler* m_evthandler;

        /** A pointer to the program's config provider */
        sayaConfigProvider* m_configprovider;
};

#endif // PROJECTMANAGER_H
