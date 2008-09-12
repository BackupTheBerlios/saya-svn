/***************************************************************
 * Name:      projectmanager.h
 * Purpose:   Definition for the Project Manager class
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-04-30
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 **************************************************************/

#ifndef PROJECTMANAGER_H
#define PROJECTMANAGER_H

#include <deque>
#include <list>
#include <map>
#include <string>

class VidProject;
class SayaConfigProvider;
class sayaEvtHandler;
class sayaDebugLogger;

/** Tells whether the application is shutting down. */
bool IsAppShuttingDown();

/** Sets the flag that forbids a new Project Manager instance to be created. */
void ShutDownApp();

extern const std::string APP_NAME; /** The application's name. */
extern const std::string APP_VENDOR; /** The application vendor's name (that would be moi) */
extern const std::string APP_SHOWNAME; /** The application's official name */
extern const std::string APP_SHOWOFFNAME; /** The application's name and tagline for showing off */

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

        /** @brief sets the pointer for the program's debug logger
          *
          * Call this from your main program to specify an object which will do the
          * debug logging.
          */
        void SetDebugLogger(sayaDebugLogger* logger);

        /** Sets the Configuration provider.
          *
          * @note You MUST call this method from your program after creating the project manager.
          */
        void SetConfigProvider(SayaConfigProvider* provider);

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

        /** @brief Counter telling the revision of the Recent Opened projects list.
          *
          * Whenever the Recent Opened Files list changes, the number is increased by one.
          */
        unsigned int GetRecentFilesModCounter();

        /** @brief Counter telling the revision of the recently imported files list.
          *
          * Whenever the Recent Imported Files list changes, the number is increased by one.
          */
        unsigned int GetRecentImportsModCounter();

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

        /** Gets (only names) all predefined settings saved by user (not include custom)  */
        std::list<std::string> GetPresets();

        /** Returns the info for the specified preset. The map contains the id of the component and the value.
        The value is a string, but it is well formed for every value type of every widget */
        std::map<std::string, std::string> GetPresetData(std::string preset);

        /** Save new predefined setting */
        bool SaveNewPreset(std::string preset, std::map<std::string, std::string>);

    protected:

    private:

        /** The currently opened project */
        VidProject* m_project;

        /** The last used project directory */
        std::string m_lastprojectdir;

        /** A pointer to the program's event handler */
        sayaEvtHandler* m_evthandler;

        /** A pointer to the program's debug logger */
        sayaDebugLogger* m_logger;

        /** A pointer to the program's config provider */
        SayaConfigProvider* m_configprovider;

        /** Counter for the Recent Projects List revision */
        unsigned int m_recentfilesmodcounter;

        /** Counter for the Recent Imports List revision */
        unsigned int m_recentimportsmodcounter;

};

#endif // PROJECTMANAGER_H
