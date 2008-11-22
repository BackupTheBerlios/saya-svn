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

class VidProject;
class SayaConfigProvider;
class sayaEvtHandler;
class sayaDebugLogger;
class RecentFilesList;
class PresetManager;
class AVController;
class ProjectManagerData;

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
    friend class ProjectManagerData;
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
        void SetConfigProvider(SayaConfigProvider* provider);

        /** Gets the last used project directory */
        const std::string GetLastProjectDir();

        // Recent Projects and Imported files

        const std::string GetOfflineProjectTitle(const std::string& filename);

        const std::string GetOfflineProjectTitle(const char* filename);

        /** A list of the most recently opened project files. */
        RecentFilesList* m_RecentFiles;

        /** A list of the most recently imported clips. */
        RecentFilesList* m_RecentImports;

        /** The object to access the current video presets */
        PresetManager* m_Presets;

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
        bool m_ClearUndoHistoryOnSave;

        /** Called whenever the project's status or filename have changed. */
        void OnProjectStatusModified();

    protected:

    private:
        ProjectManagerData* m_Data;
};

#endif // PROJECTMANAGER_H
