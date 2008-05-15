#ifndef UNDOHISTORY_H
#define UNDOHISTORY_H

#include <wx/string.h>
#include <deque>

class UndoState {
    public:
        UndoState() {};
        UndoState(wxString theOp,wxString thedata) {
            nextOp = theOp;
            data = thedata;
        }
        wxString nextOp; // transition to the NEXT state
        wxString data; // project's serialized state (tracks,clip pieces, transitions and effects)
        ~UndoState() {};
};

typedef std::deque<UndoState>  UndoHistoryQueue;

class UndoHistoryClass {
    public:

        UndoHistoryClass(unsigned long maxsize = 16*1048576);
        // 16 MBytes of undo info ought to be enough for anyone ;-)
        void Clear();
        bool Undo(wxString& data,const wxString& curstatedata);
        bool Redo(wxString& data);
        void PushUndo(const wxString Opname,const wxString& data);
        bool IsEof();
        bool IsNextEof();
        bool RestoreFromCurrentSlot(wxString& data);
        void SaveIntoCurrentSlot(const wxString Opname, const wxString& data);

        const wxString GetRedoOpname();
        const wxString GetUndoOpname();
        unsigned int CurState();
        const wxString GetOpname(unsigned int idx);
        ~UndoHistoryClass() {};
    private:
        void PopBack();
        void PopFront();
        void CheckMemUsage();
        unsigned int m_State;
        UndoHistoryQueue m_queue;
        unsigned long m_MaxSize;
        unsigned long m_UsedSize;
};

#endif
