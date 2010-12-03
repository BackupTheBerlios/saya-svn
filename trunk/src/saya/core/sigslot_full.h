/**************************************************************************************
 * Name:      sigslot_full.h
 * Purpose:   Implementation of a threaded signal and slot library.
 * Original Author:
 *            Sarah Thompson
 * Modified by:
 *            - Miguel A. Gavidia,
 *            - Rick Garcia
 * Created:   2010-11-28
 * Modified:  2010-11-28
 * Copyright: Miguel A. Gavidia, Rick Garcia
 * License:   LGPL Licence version 3.0 or later
 * Notes:
 *              Modified by juvinious adding in chaining support 01/01/2007:
 *              - Modded to add in specific slot disconnections 01/24/2007
 *              - Added in stuff to be inclusive to saggui
 *              Modified by Rick Garcia 28/11/2010:
 *              - Removed saggui stuff that we don't need
 *              - Moved some classes to an external .cpp file
 *              - Made multithreaded by default using a special shared mutex template.
 *              - Removed now deprecated thread policies.
 *              - Replaced multiple template class definitions with CPP macros
 *              03/12/2010:
 *              - Separated has_signals and has_slots from the rest of the classes to minimize compile dependencies.
 *              - Signals are only triggered if a thread's not exiting (for safety).
 *              - WARNING: You should NEVER rely on a signal to call a destructor!
 *                If you're in a separate thread, and the thread's exiting, the destructor won't be called.
 **************************************************************************************/

/*
SAGGUI (Simplified All Gaming Graphical User Interface)
Copyright (c) 2007 Miguel A. Gavidia
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:

    * Redistributions of source code must retain the above
       copyright notice, this list of conditions and the following
       disclaimer.
    * Redistributions in binary form must reproduce the above
       copyright notice, this list of conditions and the following
       disclaimer in the documentation and/or other materials
       provided with the distribution.
    * Neither the name of the "SAGGUI" nor the names of its
       contributors may be used to endorse or promote products
       derived from this software without specific prior written
       permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
THE POSSIBILITY OF SUCH DAMAGE.
*/

/*  sigslot.h: Signal/Slot classes
 *
 *  Written by Sarah Thompson (sarah@telergy.com) 2002.
 *
 *  License: Public domain. You are free to use this code however you like, with the proviso that
 *           the author takes on no responsibility or liability for any use.
 *
 *   USING THE LIBRARY
 *
 *    See the full documentation at http://sigslot.sourceforge.net/
 *
 *
 */


#ifndef SIGSLOT_FULL_H__
#define SIGSLOT_FULL_H__

#include "sigslot.h"

#include <list>

// ------------------------------
// Begin variable argument macros
// ------------------------------

#define SIGSLOT_ARGLIST0 void*
#define SIGSLOT_ARGLIST1 arg1_type
#define SIGSLOT_ARGLIST2 SIGSLOT_ARGLIST1, arg2_type
#define SIGSLOT_ARGLIST3 SIGSLOT_ARGLIST2, arg3_type
#define SIGSLOT_ARGLIST4 SIGSLOT_ARGLIST3, arg4_type
#define SIGSLOT_ARGLIST5 SIGSLOT_ARGLIST4, arg5_type
#define SIGSLOT_ARGLIST6 SIGSLOT_ARGLIST5, arg6_type
#define SIGSLOT_ARGLIST7 SIGSLOT_ARGLIST6, arg7_type
#define SIGSLOT_ARGLIST8 SIGSLOT_ARGLIST7, arg8_type

#define SIGSLOT_FUNCARGLIST0
#define SIGSLOT_FUNCARGLIST1 arg1_type
#define SIGSLOT_FUNCARGLIST2 SIGSLOT_FUNCARGLIST1, arg2_type
#define SIGSLOT_FUNCARGLIST3 SIGSLOT_FUNCARGLIST2, arg3_type
#define SIGSLOT_FUNCARGLIST4 SIGSLOT_FUNCARGLIST3, arg4_type
#define SIGSLOT_FUNCARGLIST5 SIGSLOT_FUNCARGLIST4, arg5_type
#define SIGSLOT_FUNCARGLIST6 SIGSLOT_FUNCARGLIST5, arg6_type
#define SIGSLOT_FUNCARGLIST7 SIGSLOT_FUNCARGLIST6, arg7_type
#define SIGSLOT_FUNCARGLIST8 SIGSLOT_FUNCARGLIST7, arg8_type


#define SIGSLOT_CLASSLIST0 class arg0_type = void*
#define SIGSLOT_CLASSLIST1 class arg1_type
#define SIGSLOT_CLASSLIST2 SIGSLOT_CLASSLIST1, class arg2_type
#define SIGSLOT_CLASSLIST3 SIGSLOT_CLASSLIST2, class arg3_type
#define SIGSLOT_CLASSLIST4 SIGSLOT_CLASSLIST3, class arg4_type
#define SIGSLOT_CLASSLIST5 SIGSLOT_CLASSLIST4, class arg5_type
#define SIGSLOT_CLASSLIST6 SIGSLOT_CLASSLIST5, class arg6_type
#define SIGSLOT_CLASSLIST7 SIGSLOT_CLASSLIST6, class arg7_type
#define SIGSLOT_CLASSLIST8 SIGSLOT_CLASSLIST7, class arg8_type

#define SIGSLOT_NAMEDARGLIST0
#define SIGSLOT_NAMEDARGLIST1 arg1_type a1
#define SIGSLOT_NAMEDARGLIST2 SIGSLOT_NAMEDARGLIST1, arg2_type a2
#define SIGSLOT_NAMEDARGLIST3 SIGSLOT_NAMEDARGLIST2, arg2_type a3
#define SIGSLOT_NAMEDARGLIST4 SIGSLOT_NAMEDARGLIST3, arg2_type a4
#define SIGSLOT_NAMEDARGLIST5 SIGSLOT_NAMEDARGLIST4, arg2_type a5
#define SIGSLOT_NAMEDARGLIST6 SIGSLOT_NAMEDARGLIST5, arg2_type a6
#define SIGSLOT_NAMEDARGLIST7 SIGSLOT_NAMEDARGLIST6, arg2_type a7
#define SIGSLOT_NAMEDARGLIST8 SIGSLOT_NAMEDARGLIST7, arg2_type a8

#define SIGSLOT_PARAMLIST0
#define SIGSLOT_PARAMLIST1 a1
#define SIGSLOT_PARAMLIST2 SIGSLOT_PARAMLIST1, a2
#define SIGSLOT_PARAMLIST3 SIGSLOT_PARAMLIST2, a3
#define SIGSLOT_PARAMLIST4 SIGSLOT_PARAMLIST3, a4
#define SIGSLOT_PARAMLIST5 SIGSLOT_PARAMLIST4, a5
#define SIGSLOT_PARAMLIST6 SIGSLOT_PARAMLIST5, a6
#define SIGSLOT_PARAMLIST7 SIGSLOT_PARAMLIST6, a7
#define SIGSLOT_PARAMLIST8 SIGSLOT_PARAMLIST7, a8

#define SIGSLOT_VARGLIST(x) SIGSLOT_ARGLIST ## x
#define SIGSLOT_VFUNCARGLIST(x) SIGSLOT_FUNCARGLIST ## x
#define SIGSLOT_VCARGLIST(x) , SIGSLOT_ARGLIST ## x
#define SIGSLOT_VCLASSLIST(x) SIGSLOT_CLASSLIST ## x
#define SIGSLOT_VCCLASSLIST(x) , SIGSLOT_CLASSLIST ## x
#define SIGSLOT_VNAMEDARGLIST(x) SIGSLOT_NAMEDARGLIST ## x
#define SIGSLOT_VPARAMLIST(x) SIGSLOT_PARAMLIST ## x

// ----------------------------
// End variable argument macros
// ----------------------------

namespace sigslot {

#define SIGSLOT_TEMPLATE_CONNECTION_BASE(x) \
template<SIGSLOT_VCLASSLIST(x)> \
class _connection_base ## x \
{ \
    public: \
        virtual ~_connection_base ## x() {} \
        virtual has_slots* getdest() const = 0; \
        virtual void emit(SIGSLOT_VFUNCARGLIST(x)) = 0; \
        virtual _connection_base ## x<SIGSLOT_VARGLIST(x)>* clone() = 0; \
        virtual _connection_base ## x<SIGSLOT_VARGLIST(x)>* duplicate(has_slots* pnewdest) = 0; \
}

SIGSLOT_TEMPLATE_CONNECTION_BASE(0);
SIGSLOT_TEMPLATE_CONNECTION_BASE(1);
SIGSLOT_TEMPLATE_CONNECTION_BASE(2);
SIGSLOT_TEMPLATE_CONNECTION_BASE(3);
SIGSLOT_TEMPLATE_CONNECTION_BASE(4);
SIGSLOT_TEMPLATE_CONNECTION_BASE(5);
SIGSLOT_TEMPLATE_CONNECTION_BASE(6);
SIGSLOT_TEMPLATE_CONNECTION_BASE(7);
SIGSLOT_TEMPLATE_CONNECTION_BASE(8);

/** @class _signal_mp
 *  @brief A thread-safe class designed for use with signal classes.
 *  @warning This class' internal mutex is shared by all its subclasses. DO NOT USE except for signals!
 */

class _signal_mp {
    friend class sig_locker;
    public:
        _signal_mp();
        virtual ~_signal_mp();

    protected:
        /** Locks the signal mutex. @return false if the current thread is closing. */
        bool sig_lock();

        /** @return true if the mutex is currently locked and owned by the current thread. */
        bool sig_locked();

        /** Unlocks the mutex. */
        void sig_unlock();

        /** @return true if the current thread is not exiting; false otherwise. */
        static bool sig_safe();

    private:
        class Data;
        friend class Data;
        Data* m_Data;
};

class sig_locker {
    public:
        /** Constructor */
        sig_locker(_signal_mp& sig);

        /** Destructor */
        ~sig_locker();

        /** (re)Locks the mutex */
        bool Lock();

        /** Unlocks the mutex */
        void Unlock();

        /** @return true if the mutex is currently locked and owned by the current thread. */
        bool IsLocked();
    private:
        _signal_mp& m_sig;
};

#define SIGSLOT_TEMPLATE_SIGNAL_BASE(x) \
template<SIGSLOT_VCLASSLIST(x)> \
class _signal_base ## x : public _signal_base, public _signal_mp \
{ \
    public: \
        typedef typename std::list<_connection_base ## x<SIGSLOT_VARGLIST(x)> *>  connections_list; \
        typedef typename connections_list::const_iterator const_iterator; \
        typedef typename connections_list::iterator iterator; \
        _signal_base ## x() {} \
        ~_signal_base ## x() { disconnect_all(); } \
        _signal_base ## x(const _signal_base ## x<SIGSLOT_VARGLIST(x)>& s) : _signal_base(s) {  \
            sig_locker lock(*this); if(lock.IsLocked()) { \
                const_iterator  it = s.m_connected_slots.begin(); const_iterator itEnd = s.m_connected_slots.end(); \
                while (it != itEnd) { (*it)->getdest()->signal_connect(this); m_connected_slots.push_back((*it)->clone()); ++it; } \
            } \
        } \
        void slot_duplicate(const has_slots* oldtarget, has_slots* newtarget) { \
            sig_locker lock(*this); if(lock.IsLocked()) { \
            iterator it = m_connected_slots.begin();iterator itEnd = m_connected_slots.end(); \
            while (it != itEnd) { if ((*it)->getdest() == oldtarget) { m_connected_slots.push_back((*it)->duplicate(newtarget)); } ++it; } \
            } \
        } \
        void disconnect_all() { \
            sig_locker lock(*this); if(lock.IsLocked()) { \
                const_iterator it  = m_connected_slots.begin(); const_iterator itEnd = m_connected_slots.end(); \
                while (it != itEnd) { (*it)->getdest()->signal_disconnect(this); delete *it; ++it; } \
                m_connected_slots.erase(m_connected_slots.begin(), m_connected_slots.end()); \
            } \
        } \
        void disconnect(has_slots* pclass) { \
            sig_locker lock(*this); if(lock.IsLocked()) { \
                iterator it = m_connected_slots.begin(); iterator itEnd = m_connected_slots.end(); \
                while (it != itEnd) { if ((*it)->getdest() == pclass) { delete *it; m_connected_slots.erase(it); pclass->signal_disconnect(this); return; } ++it; } \
            } \
        } \
        void slot_disconnect(has_slots* pslot) { \
            sig_locker lock(*this); if(lock.IsLocked()) { \
                iterator it = m_connected_slots.begin(); iterator itEnd = m_connected_slots.end(); \
                while (it != itEnd) { iterator itNext = it; ++itNext; if ((*it)->getdest() == pslot) { delete *it; m_connected_slots.erase(it); } it = itNext; } \
            } \
        } \
    protected: \
        connections_list m_connected_slots; \
}

SIGSLOT_TEMPLATE_SIGNAL_BASE(0);
SIGSLOT_TEMPLATE_SIGNAL_BASE(1);
SIGSLOT_TEMPLATE_SIGNAL_BASE(2);
SIGSLOT_TEMPLATE_SIGNAL_BASE(3);
SIGSLOT_TEMPLATE_SIGNAL_BASE(4);
SIGSLOT_TEMPLATE_SIGNAL_BASE(5);
SIGSLOT_TEMPLATE_SIGNAL_BASE(6);
SIGSLOT_TEMPLATE_SIGNAL_BASE(7);
SIGSLOT_TEMPLATE_SIGNAL_BASE(8);

typedef void slot;

#define SIGSLOT_TEMPLATE_CONNECTION(x) \
template<class dest_type SIGSLOT_VCCLASSLIST(x)> \
class _connection ## x : public _connection_base ## x<SIGSLOT_VARGLIST(x)> \
{ \
    public: \
        _connection ## x() { this->pobject = NULL; this->pmemfun = NULL; } \
        _connection ## x(dest_type* pobject, slot (dest_type::*pmemfun)(SIGSLOT_VARGLIST(x))) { m_pobject = pobject; m_pmemfun = pmemfun; } \
        virtual ~_connection ## x() {} \
        virtual _connection_base ## x<SIGSLOT_VARGLIST(x)>* clone() { return new _connection ## x<dest_type SIGSLOT_VCARGLIST(x)>(*this); } \
        virtual _connection_base ## x<SIGSLOT_VARGLIST(x)>* duplicate(has_slots* pnewdest) { return new _connection ## x<dest_type SIGSLOT_VCARGLIST(x)>((dest_type *)pnewdest, m_pmemfun); } \
        virtual void emit(SIGSLOT_VNAMEDARGLIST(x)) { (m_pobject->*m_pmemfun)(SIGSLOT_VPARAMLIST(x)); } \
        virtual has_slots* getdest() const { return m_pobject; } \
        dest_type* m_pobject; \
        void (dest_type::* m_pmemfun)(SIGSLOT_VARGLIST(x)); \
};

SIGSLOT_TEMPLATE_CONNECTION(0)
SIGSLOT_TEMPLATE_CONNECTION(1)
SIGSLOT_TEMPLATE_CONNECTION(2)
SIGSLOT_TEMPLATE_CONNECTION(3)
SIGSLOT_TEMPLATE_CONNECTION(4)
SIGSLOT_TEMPLATE_CONNECTION(5)
SIGSLOT_TEMPLATE_CONNECTION(6)
SIGSLOT_TEMPLATE_CONNECTION(7)
SIGSLOT_TEMPLATE_CONNECTION(8)

#define SIGSLOT_TEMPLATE_SIGNAL(x) \
template <SIGSLOT_VCLASSLIST(x)> class signal## x : public _signal_base## x <SIGSLOT_VARGLIST(x)> ,has_signals { \
    public: \
        typedef typename std::list<has_signals*>::iterator sig_iterator; \
        typedef typename _signal_base## x <SIGSLOT_VARGLIST(x) >::connections_list::const_iterator const_iterator; \
        typedef typename _signal_base## x <SIGSLOT_VARGLIST(x) >::connections_list::iterator iterator; \
        signal## x() {} \
        signal## x(const signal## x <SIGSLOT_VARGLIST(x)> & s) : _signal_base## x <SIGSLOT_VARGLIST(x)> (s) {} \
        virtual ~signal## x() {} \
 \
        template<class desttype> void connect(desttype* pclass, void (desttype::*pmemfun)(SIGSLOT_VARGLIST(x))) { \
            sig_locker lock(*this); if(lock.IsLocked()) { \
                _connection## x <desttype SIGSLOT_VCARGLIST(x)> * conn = new _connection## x< desttype SIGSLOT_VCARGLIST(x)> (pclass, pmemfun); \
                this->m_connected_slots.push_back(conn); \
                pclass->signal_connect(this); \
            } \
        } \
 \
        template<class desttype> void disconnect_slot(desttype* pclass, void (desttype::*pmemfun)(SIGSLOT_VARGLIST(x))) { \
            sig_locker lock(*this); if(lock.IsLocked()) { \
                iterator itNext, it = this->m_connected_slots.begin(); \
                iterator itEnd = this->m_connected_slots.end(); \
                while (it != itEnd) { \
                    itNext = it; \
                    ++itNext; \
                    _connection## x<desttype SIGSLOT_VCARGLIST(x)>* temp = dynamic_cast<_connection## x<desttype SIGSLOT_VCARGLIST(x)> *>((*it)); \
                    if (temp->m_pobject == pclass && temp->m_pmemfun == pmemfun) { \
                        delete (*it); \
                        this->m_connected_slots.erase(it); \
                        pclass->signal_disconnect(this); \
                        break; \
                    } \
                    it = itNext; \
                } \
            } \
        } \
 \
        void connect(signal## x<SIGSLOT_VARGLIST(x)>* chainsig) { \
            sig_locker lock(*this); if(lock.IsLocked()) { \
                this->connect_signal(chainsig); \
            } \
        } \
 \
        void disconnect_signal(signal## x<SIGSLOT_VARGLIST(x)>* chainsig) { \
            sig_locker lock(*this); if(lock.IsLocked()) { \
                this->disconnect_connected(chainsig); \
            } \
        } \
 \
        void emit(SIGSLOT_VNAMEDARGLIST(x)) { \
            sig_locker lock(*this); if(lock.IsLocked()) { \
                const_iterator itNext, it = this->m_connected_slots.begin(); \
                const_iterator itEnd = this->m_connected_slots.end(); \
     \
                while (it != itEnd) { \
                    itNext = it; \
                    ++itNext; \
                    (*it)->emit(SIGSLOT_VPARAMLIST(x)); \
                    it = itNext; \
                } \
     \
                sig_iterator siNext, si = this->connected_signals.begin(); \
                sig_iterator siEnd = this->connected_signals.end(); \
     \
                while (si != siEnd) \
                { \
                    siNext = si; \
                    ++siNext; \
                    static_cast<signal## x<SIGSLOT_VARGLIST(x)>*>(*si)->emit(SIGSLOT_VPARAMLIST(x)); \
                    si = siNext; \
                } \
            } \
        } \
 \
        inline void operator()(SIGSLOT_VNAMEDARGLIST(x)) { \
            if(_signal_mp::sig_safe()) { emit(SIGSLOT_VPARAMLIST(x)); } \
        } \
}

SIGSLOT_TEMPLATE_SIGNAL(0);
SIGSLOT_TEMPLATE_SIGNAL(1);
SIGSLOT_TEMPLATE_SIGNAL(2);
SIGSLOT_TEMPLATE_SIGNAL(3);
SIGSLOT_TEMPLATE_SIGNAL(4);
SIGSLOT_TEMPLATE_SIGNAL(5);
SIGSLOT_TEMPLATE_SIGNAL(6);
SIGSLOT_TEMPLATE_SIGNAL(7);
SIGSLOT_TEMPLATE_SIGNAL(8);

}; // namespace sigslot

// -------------------------
// Begin macros undefinition
// -------------------------

#undef SIGSLOT_ARGLIST0
#undef SIGSLOT_ARGLIST1
#undef SIGSLOT_ARGLIST2
#undef SIGSLOT_ARGLIST3
#undef SIGSLOT_ARGLIST4
#undef SIGSLOT_ARGLIST5
#undef SIGSLOT_ARGLIST6
#undef SIGSLOT_ARGLIST7
#undef SIGSLOT_ARGLIST8

#undef SIGSLOT_FUNCARGLIST0
#undef SIGSLOT_FUNCARGLIST1
#undef SIGSLOT_FUNCARGLIST2
#undef SIGSLOT_FUNCARGLIST3
#undef SIGSLOT_FUNCARGLIST4
#undef SIGSLOT_FUNCARGLIST5
#undef SIGSLOT_FUNCARGLIST6
#undef SIGSLOT_FUNCARGLIST7
#undef SIGSLOT_FUNCARGLIST8

#undef SIGSLOT_CLASSLIST0
#undef SIGSLOT_CLASSLIST1
#undef SIGSLOT_CLASSLIST2
#undef SIGSLOT_CLASSLIST3
#undef SIGSLOT_CLASSLIST4
#undef SIGSLOT_CLASSLIST5
#undef SIGSLOT_CLASSLIST6
#undef SIGSLOT_CLASSLIST7
#undef SIGSLOT_CLASSLIST8

#undef SIGSLOT_NAMEDARGLIST0
#undef SIGSLOT_NAMEDARGLIST1
#undef SIGSLOT_NAMEDARGLIST2
#undef SIGSLOT_NAMEDARGLIST3
#undef SIGSLOT_NAMEDARGLIST4
#undef SIGSLOT_NAMEDARGLIST5
#undef SIGSLOT_NAMEDARGLIST6
#undef SIGSLOT_NAMEDARGLIST7
#undef SIGSLOT_NAMEDARGLIST8

#undef SIGSLOT_PARAMLIST0
#undef SIGSLOT_PARAMLIST1
#undef SIGSLOT_PARAMLIST2
#undef SIGSLOT_PARAMLIST3
#undef SIGSLOT_PARAMLIST4
#undef SIGSLOT_PARAMLIST5
#undef SIGSLOT_PARAMLIST6
#undef SIGSLOT_PARAMLIST7
#undef SIGSLOT_PARAMLIST8

#undef SIGSLOT_VARGLIST
#undef SIGSLOT_VFUNCARGLIST
#undef SIGSLOT_VCARGLIST
#undef SIGSLOT_VCLASSLIST
#undef SIGSLOT_VCCLASSLIST
#undef SIGSLOT_VNAMEDARGLIST
#undef SIGSLOT_VPARAMLIST

#undef SIGSLOT_TEMPLATE_CONNECTION_BASE
#undef SIGSLOT_TEMPLATE_SIGNAL_BASE
#undef SIGSLOT_TEMPLATE_CONNECTION
#undef SIGSLOT_TEMPLATE_SIGNAL
// -----------------------
// End macros undefinition
// -----------------------

#endif

