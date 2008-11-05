/***************************************************************
 * Name:      vidproject.h
 * Purpose:   Definition of the VidProject class
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-04-30
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 **************************************************************/

#ifndef VIDPROJECT_H
#define VIDPROJECT_H

#include "serializable.h"

// TODO: Simplify the vidproject API by using private classes and forward declarations

class VidProject;
class AVSettings;
class UndoHistoryClass;
class AVTimeline;
class AVResources;

/** @class VidProject
  * @brief Defines a video project and all objects contained within.
  *
  * This is perhaps the most important class. A video project contains
  * resources (imported video/audio clips, images),
  * sequences, tracks,clips, effects, everything needed to edit and
  * render a video. The only thing missing is the actual data, which
  * is in the video and audio files.
  *
  */
class VidProject:public serializable
{
    public:
        /** Standard constructor. */
        VidProject();
        /** Standard destructor. */
        virtual ~VidProject();

        /** @brief Loads the project from XML data.
          * @see serializable::unserialize
          */
        virtual bool unserialize(const std::string& src);

        /** @brief Saves the project into XML data.
          * @see serializable::serialize
          */
        virtual std::string serialize();

        /** @brief Loads a project from disk.
          *
          * Loads a project from a given filename. If the file can't be opened / parsed,
          * the error is stored in errortext.
          * @param filename The file to open.
          * @param errortext The text of the error (if any).
          * @return a pointer to the newly created project.
          */
        static VidProject* Load(const std::string filename,std::string &errortext);

        /** @brief Saves the project under its current filename.
          * @return True on success; false otherwise.
          */
        bool Save();

        /** @brief Saves the project under a new filename.
          *
          * The project will adopt the new filename if successful.
          * @return True on success; false otherwise.
          */
        bool SaveAs(const std::string filename);

        /** @brief Saves a copy of the project with another filename.
          *
          * The project's unsaved status will remain untouched.
          * @return True on success; false otherwise.
          */
        bool SaveCopy(const std::string filename);

        /** @brief Reverts the project to the original state after it was loaded from disk.
          *
          */
        void Revert();

        /** @brief Reloads the project from disk, discarding any changes.
          *
          * @return True on success; false otherwise.
          * @note The project's state is saved on a temporary variable before loading,
          * so that in case of failure, the project returns to its previous state.
          */
        bool Reload();


        /** Clears all data in the project, leaving it as new. */
        void Clear();

        /** @brief Gets an offline project's Title
          * @param filename The filename of the project
          * @return The project's title
          */
        static const std::string GetOfflineProjectTitle(const std::string& filename);

        // Undo History management functions

        /** Clears the Undo History */
        void ClearUndoHistory();

        /** Can the last operation be undone? */
        bool CanUndo();

        /** Can the last undoed operation be redone? */
        bool CanRedo();

        /** Undoes the last operation. */
        void Undo();

        /** Redoes the last undone operation. */
        void Redo();

        /** Saves the current state in an undo slot, and stores the operation's name. */
        void PushUndo(const std::string OpName);

        /** Gets the name for the last done operation. */
        const std::string GetUndoOpname();

        /** Gets the name for the last undone operation. */
        const std::string GetRedoOpname();

        /** Gets the name for the operation done after state idx. */
        const std::string GetUndoHistoryOpName(unsigned int idx);

        /** Gets the current undo slot. */
        unsigned int GetUndoIdx();

        /** Is the project modified? */
        bool IsModified();

        /** Sets the project's modified state to true. */
        void SetModified();

        /** Sets the project's modified state to false. */
        void ResetModified();

        /** Specifies if the project's export settings have been set. */
        bool m_NeedsExportSettings;

        /** Project's Title */
        std::string m_Title;

        /** Project's filename */
        std::string m_Filename;

        /** Is the project new (it hasn't been saved yet)? */
        bool IsNew();

        /** The export settings for the current project. @see AVSettings */
        AVSettings* m_ExportSettings;

    private:
        /** Project's modified flag */
        bool m_IsModified;
        /** Saves the project to a given filename. */
        bool SaveToFile(const std::string &filename);

        /** Project's original state as loaded from disk. */
        std::string m_OriginalXML;

        /** Undo History Stack. @see UndoHistoryClass */
        UndoHistoryClass* m_UndoHistory;

        /** Saves project's current state. */
        void SaveState(std::string& data);
        /** Loads specified state from the project. */
        bool LoadState(const std::string& data);

        /** The data for the sequences and clips in the project. */
        AVTimeline* m_Timeline; // the data for all clips

        /** The data for the resources used in the project. */
        AVResources* m_Resources;
};

#endif // VIDPROJECT_H
