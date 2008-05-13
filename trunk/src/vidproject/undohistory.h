#ifndef UNDOHISTORY_H
#define UNDOHISTORY_H

#include <wx/string.h>
#include <deque>

class UndoState {
    public:
        UndoState(const wxString theOp,const wxString &undodata) {
            prevOp = theOp;
            data = undodata;
        }
        wxString prevOp; // previous operation that led to come to this state
        wxString data; // project's serialized state (tracks,clip pieces, transitions and effects)
        ~UndoState() {};
};

typedef std::deque<UndoState*>  UndoHistoryQueue;

class UndoHistoryClass {
    public:

        UndoHistoryClass(unsigned long maxsize = 16*1048576);
        // 16 MBytes of undo info ought to be enough for anyone ;-)
        void Clear();
        bool Undo(wxString& data);
        bool Redo(wxString& data);
        void PushUndo(const wxString Opname,const wxString& data);
        const wxString GetRedoOpname();
        const wxString GetUndoOpname();
        unsigned int GetStateIdx();
        const wxString GetOpname(unsigned int idx);
        ~UndoHistoryClass() {};
    private:
        unsigned int m_State;
        UndoHistoryQueue m_queue;
        unsigned long m_MaxSize;
        unsigned long m_UsedSize;
};

#endif
