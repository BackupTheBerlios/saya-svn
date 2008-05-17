/***************************************************************
 * Name:      undohistory.cpp
 * Purpose:   Implementation of a generic Undo History class
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-04-30
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 **************************************************************/

#include "undohistory.h"



UndoHistoryClass::UndoHistoryClass(unsigned long maxsize) {
    m_MaxSize = maxsize;
}

void UndoHistoryClass::Clear() {
    m_queue.clear();
    m_State = 0;
    m_UsedSize = 0;
}

unsigned int UndoHistoryClass::CurState() {
    return m_State;
}

bool UndoHistoryClass::IsEof() {
    if(m_State > m_queue.size()) {
        m_State = m_queue.size();
    }
    return (m_State == m_queue.size());
}

bool UndoHistoryClass::IsNextEof() {
    if(m_State > m_queue.size()) {
        m_State = m_queue.size();
    }
    return ((m_State + 1) >= m_queue.size());
}

bool UndoHistoryClass::RestoreFromCurrentSlot(wxString& data) {
    if(IsEof()) return false;
    data = m_queue[m_State].data;
    return true;
}

void UndoHistoryClass::SaveIntoCurrentSlot(const wxString Opname, const wxString& data) {
    UndoState tmpstate(Opname,data);
    if(!IsEof()) {
        m_queue[m_State] = tmpstate;
    } else {
        m_queue.push_back(tmpstate);
        m_UsedSize +=  data.Len()* sizeof(wxChar);
    }
}

void UndoHistoryClass::PopBack() {
    unsigned long freedmemory;
    freedmemory = m_queue[m_queue.size() - 1].data.Len() * sizeof(wxChar);
    m_queue.pop_back();
    if(m_UsedSize <= freedmemory) {
        m_UsedSize = 0;
    } else {
        m_UsedSize -= freedmemory;
    }
}

void UndoHistoryClass::PopFront() {
    if(!m_queue.size())
        return;
    unsigned long freedmemory;
    freedmemory = m_queue[0].data.Len() * sizeof(wxChar);
    m_queue.pop_front();
    if(m_State) {
        m_State--; // Shift state index, too!
    }
    if(m_UsedSize <= freedmemory) {
        m_UsedSize = 0;
    } else {
        m_UsedSize -= freedmemory;
    }
}

const wxString UndoHistoryClass::GetOpname(unsigned int idx) {
    if(idx < m_queue.size()) {
        return m_queue[idx].nextOp;
    }
    return wxEmptyString;
}

bool UndoHistoryClass::Undo(wxString& data, const wxString& curstatedata) {
    if(!m_queue.size() || !m_State) {
        return false;
    }

//    1. if(IsEof()) Save state in current slot with an empty transition name;
    if(IsEof()) {
        SaveIntoCurrentSlot(wxEmptyString, curstatedata);
    }
//    2. if(curpos) { curpos--; restore state; }
    bool result = false;
    if(m_State) {
        m_State--;
        result = RestoreFromCurrentSlot(data);
    }
    return result;
}

bool UndoHistoryClass::Redo(wxString& data) {

//    1. if(!IsNextEof()) { curpos++; restore state; }
    bool result = false;
    if(!IsNextEof()) {
        m_State++;
        result = RestoreFromCurrentSlot(data);
    }
    return result;
}

void UndoHistoryClass::PushUndo(const wxString Opname,const wxString& data) {

//    1. while(!IsNextEof()) delete the topmost state.
    while(!IsNextEof()) {
        PopBack();
    }

    if(IsEof()) {
//    2. if(IsEof()) Save state in current slot with the transition to be done;
        SaveIntoCurrentSlot(Opname, data);
    } else {
//      else just rename current slot's transition.
        m_queue[m_State].nextOp = Opname;
    }

//    3. ++curpos;
    m_State++;

    CheckMemUsage();
}

void UndoHistoryClass::CheckMemUsage() {
    // Check memory usage, and wipe as many entries from the beginning as necessary.

    while(m_queue.size() > 2 && m_UsedSize >= m_MaxSize) {
        PopFront();
    }
}

const wxString UndoHistoryClass::GetUndoOpname() {
    if(m_State && m_queue.size() >= m_State) {
        return m_queue[m_State - 1].nextOp;
    }
    return wxEmptyString;
}

const wxString UndoHistoryClass::GetRedoOpname() {
    if(!IsNextEof()) {
        return m_queue[m_State].nextOp;
    }
    return wxEmptyString;
}
