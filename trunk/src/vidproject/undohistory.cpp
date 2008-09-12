/***************************************************************
 * Name:      undohistory.cpp
 * Purpose:   Implementation of a generic Undo History class
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-04-30
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 **************************************************************/

#include "undohistory.h"
#include "serializable.h"
#include <deque>

/** @class UndoState
 *
 *  Undo states store your classes' serialized data. Together they form a history stack.
 */
class UndoState {
    public:
        UndoState() {};
        UndoState(std::string theOp,std::string thedata) {
            nextOp = theOp;
            data = thedata;
        }
        /** Brief description of the transition to the next state */
        std::string nextOp; // transition to the NEXT state
        /** The project's serialized data */
        std::string data;
        ~UndoState() {};
};


class UndoHistoryQueue : public serializable {

    public:

        /** Standard constructor */
        UndoHistoryQueue();

        /** Standard destructor */
        virtual ~UndoHistoryQueue();

        /** @see serializable::unserialize */
        virtual bool unserialize(const std::string& src);

        /** @see serializable::serialize */
        virtual std::string serialize();

        std::deque<UndoState> data;
};

UndoHistoryClass::UndoHistoryClass(unsigned long maxsize) {
    m_MaxSize = maxsize;
    m_queue = new UndoHistoryQueue;
}

void UndoHistoryClass::Clear() {
    delete m_queue;
}

unsigned int UndoHistoryClass::CurState() {
    return m_State;
}

bool UndoHistoryClass::CanUndo() {
    return m_State > 0;
}

bool UndoHistoryClass::CanRedo() {
    return !IsNextEof();
}

bool UndoHistoryClass::IsEof() {
    if(m_State > m_queue->data.size()) {
        m_State = m_queue->data.size();
    }
    return (m_State == m_queue->data.size());
}

bool UndoHistoryClass::IsNextEof() {
    if(m_State > m_queue->data.size()) {
        m_State = m_queue->data.size();
    }
    return ((m_State + 1) >= m_queue->data.size());
}

bool UndoHistoryClass::RestoreFromCurrentSlot(std::string& data) {
    if(IsEof()) return false;
    data = m_queue->data[m_State].data;
    return true;
}

void UndoHistoryClass::SaveIntoCurrentSlot(const std::string Opname, const std::string& data) {
    UndoState tmpstate(Opname,data);
    if(!IsEof()) {
        m_queue->data[m_State] = tmpstate;
    } else {
        m_queue->data.push_back(tmpstate);
        m_UsedSize +=  data.length()*2*sizeof(char);
    }
}

void UndoHistoryClass::PopBack() {
    unsigned long freedmemory;
    freedmemory = m_queue->data[m_queue->data.size() - 1].data.length()*2*sizeof(char);
    m_queue->data.pop_back();
    if(m_UsedSize <= freedmemory) {
        m_UsedSize = 0;
    } else {
        m_UsedSize -= freedmemory;
    }
}

void UndoHistoryClass::PopFront() {
    if(!m_queue->data.size())
        return;
    unsigned long freedmemory;
    freedmemory = m_queue->data[0].data.length() * 2*sizeof(char);
    m_queue->data.pop_front();
    if(m_State) {
        m_State--; // Shift state index, too!
    }
    if(m_UsedSize <= freedmemory) {
        m_UsedSize = 0;
    } else {
        m_UsedSize -= freedmemory;
    }
}

const std::string UndoHistoryClass::GetOpname(unsigned int idx) {
    if(idx < m_queue->data.size()) {
        return m_queue->data[idx].nextOp;
    }
    return "";
}

bool UndoHistoryClass::Undo(std::string& data, const std::string& curstatedata) {
    if(!m_queue->data.size() || !m_State) {
        return false;
    }

//    1. if(IsEof()) Save state in current slot with an empty transition name;
    if(IsEof()) {
        SaveIntoCurrentSlot("", curstatedata);
    }
//    2. if(curpos) { curpos--; restore state; }
    bool result = false;
    if(m_State) {
        m_State--;
        result = RestoreFromCurrentSlot(data);
    }
    return result;
}

bool UndoHistoryClass::Redo(std::string& data) {

//    1. if(!IsNextEof()) { curpos++; restore state; }
    bool result = false;
    if(!IsNextEof()) {
        m_State++;
        result = RestoreFromCurrentSlot(data);
    }
    return result;
}

void UndoHistoryClass::PushUndo(const std::string Opname,const std::string& data) {

//    1. while(!IsNextEof()) delete the topmost state.
    while(!IsNextEof()) {
        PopBack();
    }

    if(IsEof()) {
//    2. if(IsEof()) Save state in current slot with the transition to be done;
        SaveIntoCurrentSlot(Opname, data);
    } else {
//      else just rename current slot's transition.
        m_queue->data[m_State].nextOp = Opname;
    }

//    3. ++curpos;
    m_State++;

    CheckMemUsage();
}

void UndoHistoryClass::CheckMemUsage() {
    // Check memory usage, and wipe as many entries from the beginning as necessary.

    while(m_queue->data.size() > 2 && m_UsedSize >= m_MaxSize) {
        PopFront();
    }
}

const std::string UndoHistoryClass::GetUndoOpname() {
    if(m_State && m_queue->data.size() >= m_State) {
        return m_queue->data[m_State - 1].nextOp;
    }
    return "";
}

const std::string UndoHistoryClass::GetRedoOpname() {
    if(!IsNextEof()) {
        return m_queue->data[m_State].nextOp;
    }
    return "";
}

UndoHistoryQueue::UndoHistoryQueue() {
}

UndoHistoryQueue::~UndoHistoryQueue() {
}

bool UndoHistoryQueue::unserialize(const std::string& src) {
    //TODO: Implement UndoHistoryQueue::unserialize
    return false;
}

std::string UndoHistoryQueue::serialize() {
    //TODO: Implement UndoHistoryQueue::serialize
    return "";
}
