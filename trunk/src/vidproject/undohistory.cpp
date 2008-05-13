#include "undohistory.h"

UndoHistoryClass::UndoHistoryClass(unsigned long maxsize) {
    m_MaxSize = maxsize;
}

void UndoHistoryClass::Clear() {
    m_queue.clear();
    m_State = 0;
    m_UsedSize = 0;
}

unsigned int UndoHistoryClass::GetStateIdx() {
    return m_State;
}

const wxString UndoHistoryClass::GetOpname(unsigned int idx) {
    if(idx < m_queue.size()) {
        return m_queue[idx].prevOp;
    }
    return wxEmptyString;
}

bool UndoHistoryClass::Undo(wxString& data) {
    if(!m_queue.size()) {
        return false;
    }
    if(!m_State) {
        return false;
    }
    m_State--;
    data = m_queue[m_State].data;
    return true;
}

bool UndoHistoryClass::Redo(wxString& data) {
    if(m_State >= m_queue.size()) {
        return false;
    }
    data = m_queue[m_State].data;
    m_State++;
    return true;
}

void UndoHistoryClass::PushUndo(const wxString Opname,const wxString& data) {

    size_t cursize;
    // 1. Wipe history entries with index >= current state #.

    while(m_queue.size() > m_State) {
        cursize = (m_queue.back().data.Len() + m_queue.back().prevOp.Len() + 4096)* sizeof(wxChar);
        // We add 4Kbytes to take into account the wasted space after each string
        m_queue.pop_back();
        if(m_UsedSize < cursize) {
            m_UsedSize = 0;
        } else {
            m_UsedSize -= cursize;
        }
    }

    // 2. Check memory usage, and wipe as many entries from the beginning as necessary.
    while(m_State && m_queue.size() && m_UsedSize >= m_MaxSize) {
        cursize = (m_queue.front().data.Len() + m_queue.front().prevOp.Len() + 4096) * sizeof(wxChar);
        m_queue.pop_front();
        if(m_UsedSize < cursize) {
            m_UsedSize = 0;
        } else {
            m_UsedSize -= cursize;
        }
        m_State--; // Decrement the state, because the 'array' was shifted one place.
    }
    if(!m_State) {
        m_UsedSize = 0; // Fix any miscalculations when the queue gets empty
    }

    // 3. Push back the state, and increase the usage counter and the state index.

    cursize = (Opname.Len() + data.Len() + 4096) * sizeof(wxChar);
    UndoState tmpstate(Opname,data);
    m_queue.push_back(tmpstate);

    m_UsedSize += cursize;
    m_State = m_queue.size();
}

const wxString UndoHistoryClass::GetUndoOpname() {
    if(m_State && m_queue.size() >= m_State) {
        return m_queue[m_State - 1].prevOp;
    }
    return wxEmptyString;
}

const wxString UndoHistoryClass::GetRedoOpname() {
    if(m_queue.size() > m_State) {
        return m_queue[m_State].prevOp;
    }
    return wxEmptyString;
}
