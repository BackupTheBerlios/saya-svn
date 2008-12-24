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

#include "timeline/serializable.h"

class VidProject;
class VidProjectData;
class AVSettings;
class syString;

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
        bool unserialize(const syString& src);

        /** @brief Saves the project into XML data.
          * @see serializable::serialize
          */
        syString serialize();

        /** @brief Loads a project from disk.
          *
          * Loads a project from a given filename. If the file can't be opened / parsed,
          * the error is stored in errortext.
          * @param filename The file to open.
          * @param errortext The text of the error (if any).
          * @return a pointer to the newly created project.
          */
        static VidProject* Load(const char* filename,syString &errortext);

        /** @brief Loads a project from disk.
          *
          * Loads a project from a given filename. If the file can't be opened / parsed,
          * the error is stored in errortext.
          * @param filename The file to open.
          * @param errortext The text of the error (if any).
          * @return a pointer to the newly created project.
          */
        static VidProject* Load(const syString& filename,syString &errortext);

        /** @brief Saves the project under its current filename.
          * @return True on success; false otherwise.
          */
        bool Save();

        /** @brief Saves the project under a new filename.
          *
          * The project will adopt the new filename if successful.
          * @return True on success; false otherwise.
          */
        bool SaveAs(const char* filename);

        /** @brief Saves the project under a new filename.
          *
          * The project will adopt the new filename if successful.
          * @return True on success; false otherwise.
          */
        bool SaveAs(const syString& filename);

        /** @brief Saves a copy of the project with another filename.
          *
          * The project's unsaved status will remain untouched.
          * @return True on success; false otherwise.
          */
        bool SaveCopy(const char* filename);

        /** @brief Saves a copy of the project with another filename.
          *
          * The project's unsaved status will remain untouched.
          * @return True on success; false otherwise.
          */
        bool SaveCopy(const syString& filename);

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
        static const syString GetOfflineProjectTitle(const char* filename);

        // Undo History management functions

        /** Clears the Undo History */
        void ClearUndoHistory();

        /** Can the last operation be undone? */
        bool CanUndo() const;

        /** Can the last undoed operation be redone? */
        bool CanRedo() const;

        /** Undoes the last operation. */
        void Undo();

        /** Redoes the last undone operation. */
        void Redo();

        /** Saves the current state in an undo slot, and stores the operation's name. */
        void PushUndo(const char* OpName);

        /** Gets the name for the last done operation. */
        const syString GetUndoOpname() const;

        /** Gets the name for the last undone operation. */
        const syString GetRedoOpname() const;

        /** Gets the name for the operation done after state idx. */
        const syString GetUndoHistoryOpName(unsigned int idx) const;

        /** Gets the current undo slot. */
        unsigned int GetUndoIdx() const;

        /** Is the project modified? */
        bool IsModified() const;

        /** Sets the project's modified state to true. */
        void SetModified();

        /** Sets the project's modified state to false. */
        void ResetModified();

        /** Specifies if the project's export settings have been set. */
        bool m_NeedsExportSettings;

        /** Project's Title */
        const char* GetTitle() const;

        const char* GetFilename() const;

        void SetTitle(const char* newtitle);

        /** Is the project new (it hasn't been saved yet)? */
        bool IsNew() const;

        /** The export settings for the current project. @see AVSettings */
        AVSettings* m_ExportSettings;

    private:
        VidProjectData* m_Data;
};

#endif // VIDPROJECT_H
