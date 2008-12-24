/***************************************************************
 * Name:      undohistory.cpp
 * Purpose:   Implementation of a generic Undo History class
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-04-30
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 **************************************************************/

#include "core/systring.h"
#include "undohistory.h"
#include "timeline/serializable.h"
#include "timeline/serialized.h"
#include <deque>

/** @class UndoState
 *
 *  Undo states store your classes' serialized data. Together they form a history stack.
 */
class UndoState {
    public:
        UndoState() {};
        UndoState(const char* theOp,syString thedata) {
            nextOp = theOp;
            data = thedata;
        }
        /** Brief description of the transition to the next state */
        syString nextOp; // transition to the NEXT state
        /** The project's serialized data */
        syString data;
        ~UndoState() {};
};

class UndoHistoryQueue : public serializable {

    public:

        /** Standard constructor */
        UndoHistoryQueue();

        /** Standard destructor */
        virtual ~UndoHistoryQueue();

        /** @see serializable::unserialize */
        virtual bool unserialize(const char* src);

        /** @see serializable::serialize */
        virtual void serialize(serialized& dest) const;

        std::deque<UndoState> data;
};

// ----------------------------
// begin UndoHistoryClass::Data
// ----------------------------

class UndoHistoryClass::Data {
    public:

        /** Standard constructor. */
        Data(UndoHistoryClass* parent, unsigned long maxsize);

        /** Standard destructor. */
        ~Data();

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
        UndoHistoryQueue m_queue;

        /** The Maximum size allowed (in bytes) for the undo stack. */
        unsigned long m_MaxSize;

        /** The memory currently used by the undo stack. */
        unsigned long m_UsedSize;

        UndoHistoryClass* m_Parent;
};

UndoHistoryClass::Data::Data(UndoHistoryClass* parent, unsigned long maxsize) :
m_State(0),
m_MaxSize(maxsize),
m_UsedSize(0),
m_Parent(parent)
{
}

UndoHistoryClass::Data::~Data() {
}

void UndoHistoryClass::Data::CheckMemUsage() {
    // Check memory usage, and wipe as many entries from the beginning as necessary.

    while(m_queue.data.size() > 2 && m_UsedSize >= m_MaxSize) {
        PopFront();
    }
}

void UndoHistoryClass::Data::PopBack() {
    unsigned long freedmemory;
    freedmemory = m_queue.data[m_queue.data.size() - 1].data.size()*2*sizeof(char);
    m_queue.data.pop_back();
    if(m_UsedSize <= freedmemory) {
        m_UsedSize = 0;
    } else {
        m_UsedSize -= freedmemory;
    }
}

void UndoHistoryClass::Data::PopFront() {
    if(!m_queue.data.size())
        return;
    unsigned long freedmemory;
    freedmemory = m_queue.data[0].data.size() * 2*sizeof(char);
    m_queue.data.pop_front();
    if(m_State) {
        m_State--; // Shift state index, too!
    }
    if(m_UsedSize <= freedmemory) {
        m_UsedSize = 0;
    } else {
        m_UsedSize -= freedmemory;
    }
}

// --------------------------
// end UndoHistoryClass::Data
// --------------------------





UndoHistoryClass::UndoHistoryClass(unsigned long maxsize) {
    m_Data = new UndoHistoryClass::Data(this, maxsize);
}

void UndoHistoryClass::Clear() {
    delete m_Data;
}

unsigned int UndoHistoryClass::CurState() const {
    return m_Data->m_State;
}

bool UndoHistoryClass::CanUndo() const {
    return m_Data->m_State > 0;
}

bool UndoHistoryClass::CanRedo() const {
    return !IsNextEof();
}

bool UndoHistoryClass::IsEof() const {
    if(m_Data->m_State > m_Data->m_queue.data.size()) {
        m_Data->m_State = m_Data->m_queue.data.size();
    }
    return (m_Data->m_State == m_Data->m_queue.data.size());
}

bool UndoHistoryClass::IsNextEof() const {
    if(m_Data->m_State > m_Data->m_queue.data.size()) {
        m_Data->m_State = m_Data->m_queue.data.size();
    }
    return ((m_Data->m_State + 1) >= m_Data->m_queue.data.size());
}

bool UndoHistoryClass::RestoreFromCurrentSlot(syString& data) {
    if(IsEof()) return false;
    data = m_Data->m_queue.data[m_Data->m_State].data;
    return true;
}

void UndoHistoryClass::SaveIntoCurrentSlot(const char* Opname, const syString& data) {
    UndoState tmpstate(Opname,data);
    if(!IsEof()) {
        m_Data->m_queue.data[m_Data->m_State] = tmpstate;
    } else {
        m_Data->m_queue.data.push_back(tmpstate);
        m_Data->m_UsedSize +=  data.size()*2*sizeof(char);
    }
}

const syString UndoHistoryClass::GetOpname(unsigned int idx) const {
    if(idx < m_Data->m_queue.data.size()) {
        return m_Data->m_queue.data[idx].nextOp;
    }
    return "";
}

bool UndoHistoryClass::Undo(syString& data, const syString& curstatedata) {
    if(!m_Data->m_queue.data.size() || !m_Data->m_State) {
        return false;
    }

//    1. if(IsEof()) Save state in current slot with an empty transition name;
    if(IsEof()) {
        SaveIntoCurrentSlot("", curstatedata);
    }
//    2. if(curpos) { curpos--; restore state; }
    bool result = false;
    if(m_Data->m_State) {
        m_Data->m_State--;
        result = RestoreFromCurrentSlot(data);
    }
    return result;
}

bool UndoHistoryClass::Redo(syString& data) {

//    1. if(!IsNextEof()) { curpos++; restore state; }
    bool result = false;
    if(!IsNextEof()) {
        m_Data->m_State++;
        result = RestoreFromCurrentSlot(data);
    }
    return result;
}

void UndoHistoryClass::PushUndo(const char* Opname,const syString& data) {

//    1. while(!IsNextEof()) delete the topmost state.
    while(!IsNextEof()) {
        m_Data->PopBack();
    }

    if(IsEof()) {
//    2. if(IsEof()) Save state in current slot with the transition to be done;
        SaveIntoCurrentSlot(Opname, data);
    } else {
//      else just rename current slot's transition.
        m_Data->m_queue.data[m_Data->m_State].nextOp = Opname;
    }

//    3. ++curpos;
    m_Data->m_State++;

    m_Data->CheckMemUsage();
}


const syString UndoHistoryClass::GetUndoOpname() const {
    if(m_Data->m_State && m_Data->m_queue.data.size() >= m_Data->m_State) {
        return m_Data->m_queue.data[m_Data->m_State - 1].nextOp;
    }
    return "";
}

const syString UndoHistoryClass::GetRedoOpname() const {
    if(!IsNextEof()) {
        return m_Data->m_queue.data[m_Data->m_State].nextOp;
    }
    return "";
}

UndoHistoryQueue::UndoHistoryQueue() {
}

UndoHistoryQueue::~UndoHistoryQueue() {
}

bool UndoHistoryQueue::unserialize(const char* src) {
    //TODO: Implement UndoHistoryQueue::unserialize
    return false;
}

void UndoHistoryQueue::serialize(serialized& dest) const {
    //TODO: Implement UndoHistoryQueue::serialize
}
