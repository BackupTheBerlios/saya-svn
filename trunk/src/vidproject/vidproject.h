#ifndef VIDPROJECT_H
#define VIDPROJECT_H

/***************************************************************
 * Name:      vidproject.h
 * Purpose:   Definition of the VidProject class
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-04-30
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 **************************************************************/

#include <wx/string.h>
#include <map>
#include <deque>
#include "avclip.h"
#include "undohistory.h"

class VidProject;
class ExportSettings;

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
        virtual bool unserialize(const wxString& data);

        /** @brief Saves the project into XML data.
          * @see serializable::serialize
          */
        virtual wxString serialize();

        /** @brief Loads a project from disk.
          *
          * Loads a project from a given filename. If the file can't be opened / parsed,
          * the error is stored in errortext.
          * @param filename The file to open.
          * @param errortext The text of the error (if any).
          * @return a pointer to the newly created project.
          */
        static VidProject* Load(const wxString filename,wxString &errortext);

        /** @brief Saves the project under its current filename.
          * @return True on success; false otherwise.
          */
        bool Save();

        /** @brief Saves the project under a new filename.
          *
          * The project will adopt the new filename if successful.
          * @return True on success; false otherwise.
          */
        bool SaveAs(const wxString filename);

        /** @brief Saves a copy of the project with another filename.
          *
          * The project's unsaved status will remain untouched.
          * @return True on success; false otherwise.
          */
        bool SaveCopy(const wxString filename);

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
        void PushUndo(const wxString OpName);

        /** Gets the name for the last done operation. */
        const wxString GetUndoOpname();

        /** Gets the name for the last undone operation. */
        const wxString GetRedoOpname();

        /** Gets the name for the operation done after state idx. */
        const wxString GetUndoHistoryOpName(unsigned int idx);

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
        wxString m_Title;

        /** Project's filename */
        wxString m_Filename;

        /** Is the project new (it hasn't been saved yet)? */
        bool IsNew();

        /** The export settings for the current project. @see VideoSettings */
        VideoSettings m_ExportSettings;

    private:
        /** Project's modified flag */
        bool m_IsModified;
        /** Saves the project to a given filename. */
        bool SaveToFile(const wxString &filename);

        /** Project's original state as loaded from disk. */
        wxString m_OriginalXML;

        /** Undo History Stack. @see UndoHistoryClass */
        UndoHistoryClass m_UndoHistory;

        /** Saves project's current state. */
        void SaveState(wxString& data);
        /** Loads specified state from the project. */
        bool LoadState(const wxString& data);

        /** The data for the sequences and clips in the project. */
        AVTimeline m_Timeline; // the data for all clips

        /** The data for the resources used in the project. */
        AVResources m_Resources;
};

#endif // VIDPROJECT_H
