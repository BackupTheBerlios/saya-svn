/***************************************************************
 * Name:      undohistory.h
 * Purpose:   Definition of a generic Undo History class
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-04-30
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 **************************************************************/

#ifndef UNDOHISTORY_H
#define UNDOHISTORY_H

#include <string>

class UndoHistoryQueue;

class UndoHistoryClass {
    public:
        /** As default, the maximum size reserved for history is 16 megabytes */
        UndoHistoryClass(unsigned long maxsize = 16*1048576);
        /** Clears the history stack */
        void Clear();

        /** @brief Determines if the user can undo an operation in the current state.
          *
          * @return True if the user can undo; false otherwise.
          */
        bool CanUndo();

        /** @brief Determines if the user can redo an operation in the current state.
          *
          * @return True if the user can undo; false otherwise.
          */
        bool CanRedo();

        /** @brief Undoes an operation.
          * @param data Where the resulting state will be stored.
          * @param curstatedata The current serialized state
          * @return True if the operation was successful.
          */
        bool Undo(std::string& data,const std::string& curstatedata);

        /** @brief Redoes an operation.
          * @param data Where the resulting state will be stored.
          * @return True if the operation was successful.
          */
        bool Redo(std::string& data);

        /** @brief Does an operation, saving the current state.
          * @param Opname The name of the operation being performed.
          * @param data The current serialized state.
          */
        void PushUndo(const std::string Opname,const std::string& data);

        /** @brief Determines if the undo pointer is past the last saved state.
          *
          * @return True if the pointer is beyond the last saved state; false otherwise
          */
        bool IsEof();

        /** @brief Determines if the undo pointer plus one is past the last saved state.
          *
          * @return True if the pointer plus one is beyond the last saved state; false otherwise
          */
        bool IsNextEof();

        /** @brief Restores the data saved at the current undo slot.
          * @param data The string where the saved state is going to be recovered into.
          * @return True if successful; false otherwise.
          */
        bool RestoreFromCurrentSlot(std::string& data);

        /** @brief Saves the state into the current undo slot.
          * @param Opname The name of the operation for the next state.
          * @param data The saved state to be stored.
          */
        void SaveIntoCurrentSlot(const std::string Opname, const std::string& data);

        /** @brief Gets the operation to be "redone".
          * @return The "Redo" operation name.
          */
        const std::string GetRedoOpname();

        /** @brief Gets the operation to be "undone".
          * @return The "Undo" operation name.
          */
        const std::string GetUndoOpname();

        /** @brief Gets the current state pointer.
          * @return The current undo slot #.
          */
        unsigned int CurState();

        /** @brief Gets the name of the operation between saved state idx and idx+1.
          * @return The requested operation's name.
          */
        const std::string GetOpname(unsigned int idx);

        /** Standard destructor. */
        ~UndoHistoryClass() {};
    private:

        /** @brief Removes a saved state from the bottom of the undo stack.
          *
          * Called to conserve memory.
          */
        void PopBack();

        /** @brief Removes a saved state from the top of the undo stack.
          *
          * Used to remove Redo's.
          */
        void PopFront();

        /** Checks undo stack memory usage and acts accordingly. */
        void CheckMemUsage();

        /** The current undo state slot. */
        unsigned int m_State;

        /** The Undo queue */
        UndoHistoryQueue* m_queue;

        /** The Maximum size allowed (in bytes) for the undo stack. */
        unsigned long m_MaxSize;

        /** The memory currently used by the undo stack. */
        unsigned long m_UsedSize;
};

#endif
