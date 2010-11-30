/**************************************************************************************
 * Name:      sigslot.h
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

#define SIGSLOT_H__
#warning sigslot.h: UNDER CONSTRUCTION
// REMOVE THIS WHEN THE CODE'S FINISHED IMPLEMENTING

#ifndef SIGSLOT_H__
#define SIGSLOT_H__

#include <list>
#include <algorithm>
#include "sythread.h"


// --------------------------------------
// Begin preprocessor arguments expansion
// --------------------------------------

#define SIGSLOT_ARGLIST0
#define SIGSLOT_ARGLIST1 arg1_type
#define SIGSLOT_ARGLIST2 SIGSLOT_ARGLIST1, arg2_type
#define SIGSLOT_ARGLIST3 SIGSLOT_ARGLIST2, arg3_type
#define SIGSLOT_ARGLIST4 SIGSLOT_ARGLIST3, arg4_type
#define SIGSLOT_ARGLIST5 SIGSLOT_ARGLIST4, arg5_type
#define SIGSLOT_ARGLIST6 SIGSLOT_ARGLIST5, arg6_type
#define SIGSLOT_ARGLIST7 SIGSLOT_ARGLIST6, arg7_type
#define SIGSLOT_ARGLIST8 SIGSLOT_ARGLIST7, arg8_type

// For these we add a leading comma
#define SIGSLOT_CARGLIST0
#define SIGSLOT_CARGLIST1 , arg1_type
#define SIGSLOT_CARGLIST2 SIGSLOT_CARGLIST1, arg2_type
#define SIGSLOT_CARGLIST3 SIGSLOT_CARGLIST2, arg3_type
#define SIGSLOT_CARGLIST4 SIGSLOT_CARGLIST3, arg4_type
#define SIGSLOT_CARGLIST5 SIGSLOT_CARGLIST4, arg5_type
#define SIGSLOT_CARGLIST6 SIGSLOT_CARGLIST5, arg6_type
#define SIGSLOT_CARGLIST7 SIGSLOT_CARGLIST6, arg7_type
#define SIGSLOT_CARGLIST8 SIGSLOT_CARGLIST7, arg8_type

#define SIGSLOT_CLASSLIST0
#define SIGSLOT_CLASSLIST1 class arg1_type
#define SIGSLOT_CLASSLIST2 SIGSLOT_CLASSLIST1, class arg2_type
#define SIGSLOT_CLASSLIST3 SIGSLOT_CLASSLIST2, class arg3_type
#define SIGSLOT_CLASSLIST4 SIGSLOT_CLASSLIST3, class arg4_type
#define SIGSLOT_CLASSLIST5 SIGSLOT_CLASSLIST4, class arg5_type
#define SIGSLOT_CLASSLIST6 SIGSLOT_CLASSLIST5, class arg6_type
#define SIGSLOT_CLASSLIST7 SIGSLOT_CLASSLIST6, class arg7_type
#define SIGSLOT_CLASSLIST8 SIGSLOT_CLASSLIST7, class arg8_type

// For these we add a leading comma
#define SIGSLOT_CCLASSLIST0
#define SIGSLOT_CCLASSLIST1 ,class arg1_type
#define SIGSLOT_CCLASSLIST2 SIGSLOT_CCLASSLIST1, class arg2_type
#define SIGSLOT_CCLASSLIST3 SIGSLOT_CCLASSLIST2, class arg3_type
#define SIGSLOT_CCLASSLIST4 SIGSLOT_CCLASSLIST3, class arg4_type
#define SIGSLOT_CCLASSLIST5 SIGSLOT_CCLASSLIST4, class arg5_type
#define SIGSLOT_CCLASSLIST6 SIGSLOT_CCLASSLIST5, class arg6_type
#define SIGSLOT_CCLASSLIST7 SIGSLOT_CCLASSLIST6, class arg7_type
#define SIGSLOT_CCLASSLIST8 SIGSLOT_CCLASSLIST7, class arg8_type


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
#define SIGSLOT_VCARGLIST(x) SIGSLOT_CARGLIST ## x
#define SIGSLOT_VCLASSLIST(x) SIGSLOT_CLASSLIST ## x
#define SIGSLOT_VCCLASSLIST(x) SIGSLOT_CCLASSLIST ## x
#define SIGSLOT_VNAMEDARGLIST(x) SIGSLOT_NAMEDARGLIST ## x
#define SIGSLOT_VPARAMLIST(x) SIGSLOT_PARAMLIST ## x

// ------------------------------------
// End preprocessor arguments expansion
// ------------------------------------

namespace sigslot {

class has_slots;

#define SIGSLOT_TEMPLATE_CONNECTION_BASE(x) \
template<SIGSLOT_VCLASSLIST(x)> \
class _connection_base ## x \
{ \
    public: \
        virtual ~_connection_base ## x() {} \
        virtual has_slots* getdest() const = 0; \
        virtual void emit(SIGSLOT_VARGLIST(x)) = 0; \
        virtual _connection_base ## x<SIGSLOT_VARGLIST(x)>* clone() = 0; \
        virtual _connection_base ## x<SIGSLOT_VARGLIST(x)>* duplicate(has_slots* pnewdest) = 0; \
};

SIGSLOT_TEMPLATE_CONNECTION_BASE(0)
SIGSLOT_TEMPLATE_CONNECTION_BASE(1)
SIGSLOT_TEMPLATE_CONNECTION_BASE(2)
SIGSLOT_TEMPLATE_CONNECTION_BASE(3)
SIGSLOT_TEMPLATE_CONNECTION_BASE(4)
SIGSLOT_TEMPLATE_CONNECTION_BASE(5)
SIGSLOT_TEMPLATE_CONNECTION_BASE(6)
SIGSLOT_TEMPLATE_CONNECTION_BASE(7)
SIGSLOT_TEMPLATE_CONNECTION_BASE(8)


class _signal_base {
    public:
        virtual void slot_disconnect(has_slots* pslot) = 0;
        virtual void slot_duplicate(const has_slots* poldslot, has_slots* pnewslot) = 0;
    protected:
        sySharedMutex<_signal_base> m_signal_mutex;
};

class has_slots {
    private:
        class Data;
        friend class Data;
        Data* m_Data;

    public:
        has_slots();
        has_slots(const has_slots& hs);
        void signal_connect(_signal_base* sender);
        void signal_disconnect(_signal_base* sender);
        virtual ~has_slots();
        void disconnect_all_slots();
};

#define SIGSLOT_TEMPLATE_SIGNAL_BASE(x) \
template<SIGSLOT_VCLASSLIST(x)> \
class _signal_base ## x : public _signal_base \
{ \
    public: \
        typedef typename std::list<_connection_base ## x<SIGSLOT_VARGLIST(x)> *>  connections_list; \
        typedef typename connections_list::const_iterator const_iterator; \
        typedef typename connections_list::iterator iterator; \
        _signal_base ## x() {} \
        ~_signal_base ## x() { disconnect_all(); } \
        _signal_base ## x(const _signal_base ## x<SIGSLOT_VARGLIST(x)>& s) : _signal_base(s) {  \
            sySafeMutexLocker lock(m_Data->m_Mutex()); \
            const_iterator  it = s.m_connected_slots.begin(); const_iterator itEnd = s.m_connected_slots.end(); \
            while (it != itEnd) { (*it)->getdest()->signal_connect(this); m_connected_slots.push_back((*it)->clone()); ++it; } \
        } \
        void slot_duplicate(const has_slots* oldtarget, has_slots* newtarget) { \
            sySafeMutexLocker lock(m_Data->m_Mutex()); \
            iterator it = m_connected_slots.begin();iterator itEnd = m_connected_slots.end(); \
            while (it != itEnd) { if ((*it)->getdest() == oldtarget) { m_connected_slots.push_back((*it)->duplicate(newtarget)); } ++it; } \
        } \
        void disconnect_all() { \
            sySafeMutexLocker lock(m_Data->m_Mutex()); \
            const_iterator it  = m_connected_slots.begin(); const_iterator itEnd = m_connected_slots.end(); \
            while (it != itEnd) { (*it)->getdest()->signal_disconnect(this); delete *it; ++it; } \
            m_connected_slots.erase(m_connected_slots.begin(), m_connected_slots.end()); \
        } \
        void disconnect(has_slots* pclass) { \
            sySafeMutexLocker lock(m_Data->m_Mutex()); \
            iterator it = m_connected_slots.begin(); iterator itEnd = m_connected_slots.end(); \
            while (it != itEnd) { if ((*it)->getdest() == pclass) { delete *it; m_connected_slots.erase(it); pclass->signal_disconnect(this); return; } ++it; } \
        } \
        void slot_disconnect(has_slots* pslot) { \
            sySafeMutexLocker lock(m_Data->m_Mutex()); \
            iterator it = m_connected_slots.begin(); iterator itEnd = m_connected_slots.end(); \
            while (it != itEnd) { iterator itNext = it; ++itNext; if ((*it)->getdest() == pslot) { delete *it; m_connected_slots.erase(it); } it = itNext; } \
        } \
    protected: \
        connections_list m_connected_slots; \
};

SIGSLOT_TEMPLATE_SIGNAL_BASE(0)
SIGSLOT_TEMPLATE_SIGNAL_BASE(1)
SIGSLOT_TEMPLATE_SIGNAL_BASE(2)
SIGSLOT_TEMPLATE_SIGNAL_BASE(3)
SIGSLOT_TEMPLATE_SIGNAL_BASE(4)
SIGSLOT_TEMPLATE_SIGNAL_BASE(5)
SIGSLOT_TEMPLATE_SIGNAL_BASE(6)
SIGSLOT_TEMPLATE_SIGNAL_BASE(7)
SIGSLOT_TEMPLATE_SIGNAL_BASE(8)

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

class has_signals {
    public:
        has_signals();
        ~has_signals();

        void bind_signal(has_signals* chainsig);

        void connect_signal(has_signals* chainsig):
        void disconnect_connected(has_signals* chainsig);
        void disconnect_bound(has_signals* chainsig);
        void remove_connections();

    protected:
        std::list<has_signals*> connected_signals;
        std::list<has_signals*> bound_signals;
        typedef typename std::list<has_signals*>::iterator sig_iterator;
};

#warning TODO: Fix from this line to below
class signal0 : public _signal_base0, has_signals
{
    public:
        typedef typename _signal_base0::connections_list::iterator iterator;
        typedef typename _signal_base0::connections_list::const_iterator const_iterator;
        typedef typename std::list<has_signals*>::iterator sig_iterator;
        signal0() {}
        signal0(const signal0& s) : _signal_base0(s) {}
        virtual ~signal0() {}

        template<class desttype>
        void connect(desttype* pclass, void (desttype::*pmemfun)())
        {
            lock_block lock(this);
            _connection0<desttype>* conn =
                new _connection0<desttype>(pclass, pmemfun);
            this->m_connected_slots.push_back(conn);
            pclass->signal_connect(this);
        }

        template<class desttype>
        void disconnect_slot(desttype* pclass, void (desttype::*pmemfun)())
        {
            lock_block lock(this);
            iterator itNext, it = this->m_connected_slots.begin();
            iterator itEnd = this->m_connected_slots.end();


            while (it != itEnd)
            {
                itNext = it;
                ++itNext;
                _connection0<desttype>* temp = dynamic_cast<_connection0<desttype> *>((*it));
                if (temp->m_pobject == pclass && temp->m_pmemfun == pmemfun)
                {
                    delete (*it);
                    this->m_connected_slots.erase(it);
                    pclass->signal_disconnect(this);
                    break;
                }

                it = itNext;
            }
        }

        void connect(signal0* chainsig)
        {
            lock_block lock(this);
            this->connect_signal(chainsig);
        }

        void disconnect_signal(signal0* chainsig)
        {
            lock_block lock(this);
            this->disconnect_connected(chainsig);
        }

        void emit()
        {
            lock_block lock(this);
            const_iterator itNext, it = this->m_connected_slots.begin();
            const_iterator itEnd = this->m_connected_slots.end();

            while (it != itEnd)
            {
                itNext = it;
                ++itNext;

                (*it)->emit();

                it = itNext;
            }

            sig_iterator siNext, si = this->connected_signals.begin();
            sig_iterator siEnd = this->connected_signals.end();

            while (si != siEnd)
            {
                siNext = si;
                ++siNext;

                static_cast<signal0*>(*si)->emit();

                si = siNext;
            }
        }

        void operator()()
        {
            lock_block lock(this);
            const_iterator itNext, it = this->m_connected_slots.begin();
            const_iterator itEnd = this->m_connected_slots.end();

            while (it != itEnd)
            {
                itNext = it;
                ++itNext;

                (*it)->emit();

                it = itNext;
            }

            sig_iterator siNext, si = this->connected_signals.begin();
            sig_iterator siEnd = this->connected_signals.end();

            while (si != siEnd)
            {
                siNext = si;
                ++siNext;

                static_cast<signal0*>(*si)->emit();

                si = siNext;
            }
        }
};

template < class arg1_type >
class signal1 : public _signal_base1<arg1_type>, has_signals
{
    public:
        typedef typename std::list<has_signals*>::iterator sig_iterator;
        typedef typename _signal_base1<arg1_type>::connections_list::iterator iterator;
        typedef typename _signal_base1<arg1_type>::connections_list::const_iterator const_iterator;
        signal1()
        {
            ;
        }

        signal1(const signal1<arg1_type>& s)
                : _signal_base1<arg1_type>(s)
        {
            ;
        }

        virtual ~signal1()
        {
            ;
        }

        template<class desttype>
        void connect(desttype* pclass, void (desttype::*pmemfun)(arg1_type))
        {
            lock_block lock(this);
            _connection1<desttype, arg1_type>* conn =
                new _connection1<desttype, arg1_type>(pclass, pmemfun);
            this->m_connected_slots.push_back(conn);
            pclass->signal_connect(this);
        }

        template<class desttype>
        void disconnect_slot(desttype* pclass, void (desttype::*pmemfun)(arg1_type))
        {
            lock_block lock(this);
            iterator itNext, it = this->m_connected_slots.begin();
            iterator itEnd = this->m_connected_slots.end();


            while (it != itEnd)
            {
                itNext = it;
                ++itNext;
                _connection1<desttype, arg1_type>* temp = dynamic_cast<_connection1<desttype, arg1_type> *>((*it));
                if (temp->m_pobject == pclass && temp->m_pmemfun == pmemfun)
                {
                    delete (*it);
                    this->m_connected_slots.erase(it);
                    pclass->signal_disconnect(this);
                    break;
                }

                it = itNext;
            }
        }

        void bind_signal(signal1<arg1_type>* chainsig)
        {
            lock_block lock(this);
            this->bound_signals.push_back(chainsig);
        }

        void connect(signal1<arg1_type>* chainsig)
        {
            lock_block lock(this);
            this->connect_signal(chainsig);
        }

        void disconnect_signal(signal1<arg1_type>* chainsig)
        {
            lock_block lock(this);
            this->disconnect_connected(chainsig);
        }

        void emit(arg1_type a1)
        {
            lock_block lock(this);
            const_iterator itNext, it = this->m_connected_slots.begin();
            const_iterator itEnd = this->m_connected_slots.end();

            while (it != itEnd)
            {
                itNext = it;
                ++itNext;

                (*it)->emit(a1);

                it = itNext;
            }

            sig_iterator siNext, si = this->connected_signals.begin();
            sig_iterator siEnd = this->connected_signals.end();

            while (si != siEnd)
            {
                siNext = si;
                ++siNext;

                static_cast<signal1<arg1_type>*>(*si)->emit(a1);

                si = siNext;
            }
        }

        void operator()(arg1_type a1)
        {
            lock_block lock(this);
            const_iterator itNext, it = this->m_connected_slots.begin();
            const_iterator itEnd = this->m_connected_slots.end();

            while (it != itEnd)
            {
                itNext = it;
                ++itNext;

                (*it)->emit(a1);

                it = itNext;
            }

            sig_iterator siNext, si = this->connected_signals.begin();
            sig_iterator siEnd = this->connected_signals.end();

            while (si != siEnd)
            {
                siNext = si;
                ++siNext;

                static_cast<signal1<arg1_type>*>(*si)->emit(a1);

                si = siNext;
            }
        }
};

template < class arg1_type, typename arg2_type, typename mt_policy = SIGSLOT_DEFAULT_MT_POLICY >
class signal2 : public _signal_base2<arg1_type, arg2_type>, has_signals
{
    public:
        typedef typename std::list<has_signals*>::iterator sig_iterator;
        typedef typename _signal_base2<arg1_type, arg2_type>::connections_list::const_iterator const_iterator;
        typedef typename _signal_base2<arg1_type, arg2_type>::connections_list::iterator iterator;
        signal2()
        {
            ;
        }

        signal2(const signal2<arg1_type, arg2_type>& s)
                : _signal_base2<arg1_type, arg2_type>(s)
        {
            ;
        }

        virtual ~signal2()
        {
            ;
        }

        template<class desttype>
        void connect(desttype* pclass, void (desttype::*pmemfun)(arg1_type,
                     arg2_type))
        {
            lock_block lock(this);
            _connection2<desttype, arg1_type, arg2_type>* conn = new
            _connection2<desttype, arg1_type, arg2_type>(pclass, pmemfun);
            this->m_connected_slots.push_back(conn);
            pclass->signal_connect(this);
        }

        template<class desttype>
        void disconnect_slot(desttype* pclass, void (desttype::*pmemfun)(arg1_type,
                             arg2_type))
        {
            lock_block lock(this);
            iterator itNext, it = this->m_connected_slots.begin();
            iterator itEnd = this->m_connected_slots.end();


            while (it != itEnd)
            {
                itNext = it;
                ++itNext;
                _connection2<desttype, arg1_type, arg2_type>* temp = dynamic_cast<_connection2<desttype, arg1_type, arg2_type> *>((*it));
                if (temp->m_pobject == pclass && temp->m_pmemfun == pmemfun)
                {
                    delete (*it);
                    this->m_connected_slots.erase(it);
                    pclass->signal_disconnect(this);
                    break;
                }

                it = itNext;
            }
        }

        void connect(signal2<arg1_type, arg2_type>* chainsig)
        {
            lock_block lock(this);
            this->connect_signal(chainsig);
        }

        void disconnect_signal(signal2<arg1_type, arg2_type>* chainsig)
        {
            lock_block lock(this);
            this->disconnect_connected(chainsig);
        }

        void emit(arg1_type a1, arg2_type a2)
        {
            lock_block lock(this);
            const_iterator itNext, it = this->m_connected_slots.begin();
            const_iterator itEnd = this->m_connected_slots.end();

            while (it != itEnd)
            {
                itNext = it;
                ++itNext;

                (*it)->emit(a1, a2);

                it = itNext;
            }

            sig_iterator siNext, si = this->connected_signals.begin();
            sig_iterator siEnd = this->connected_signals.end();

            while (si != siEnd)
            {
                siNext = si;
                ++siNext;

                static_cast<signal2<arg1_type, arg2_type>*>(*si)->emit(a1, a2);

                si = siNext;
            }
        }

        void operator()(arg1_type a1, arg2_type a2)
        {
            lock_block lock(this);
            const_iterator itNext, it = this->m_connected_slots.begin();
            const_iterator itEnd = this->m_connected_slots.end();

            while (it != itEnd)
            {
                itNext = it;
                ++itNext;

                (*it)->emit(a1, a2);

                it = itNext;
            }

            sig_iterator siNext, si = this->connected_signals.begin();
            sig_iterator siEnd = this->connected_signals.end();

            while (si != siEnd)
            {
                siNext = si;
                ++siNext;

                static_cast<signal2<arg1_type, arg2_type>*>(*si)->emit(a1, a2);

                si = siNext;
            }
        }
};

template < class arg1_type, typename arg2_type, typename arg3_type, typename mt_policy = SIGSLOT_DEFAULT_MT_POLICY >
class signal3 : public _signal_base3<arg1_type, arg2_type, arg3_type>, has_signals
{
    public:
        typedef typename std::list<has_signals*>::iterator sig_iterator;
        typedef typename _signal_base3<arg1_type, arg2_type, arg3_type>::connections_list::const_iterator const_iterator;
        typedef typename _signal_base3<arg1_type, arg2_type, arg3_type>::connections_list::iterator iterator;
        signal3()
        {
            ;
        }

        signal3(const signal3<arg1_type, arg2_type, arg3_type>& s)
                : _signal_base3<arg1_type, arg2_type, arg3_type>(s)
        {
            ;
        }

        virtual ~signal3()
        {
            ;
        }

        template<class desttype>
        void connect(desttype* pclass, void (desttype::*pmemfun)(arg1_type,
                     arg2_type, arg3_type))
        {
            lock_block lock(this);
            _connection3<desttype, arg1_type, arg2_type, arg3_type>* conn =
                new _connection3<desttype, arg1_type, arg2_type, arg3_type>(pclass,
                        pmemfun);
            this->m_connected_slots.push_back(conn);
            pclass->signal_connect(this);
        }

        template<class desttype>
        void disconnect_slot(desttype* pclass, void (desttype::*pmemfun)(arg1_type,
                             arg2_type, arg3_type))
        {
            lock_block lock(this);
            iterator itNext, it = this->m_connected_slots.begin();
            iterator itEnd = this->m_connected_slots.end();


            while (it != itEnd)
            {
                itNext = it;
                ++itNext;
                _connection3<desttype, arg1_type, arg2_type, arg3_type>* temp = dynamic_cast<_connection3<desttype, arg1_type, arg2_type, arg3_type> *>((*it));
                if (temp->m_pobject == pclass && temp->m_pmemfun == pmemfun)
                {
                    delete (*it);
                    this->m_connected_slots.erase(it);
                    pclass->signal_disconnect(this);
                    break;
                }

                it = itNext;
            }
        }

        void connect(signal3<arg1_type, arg2_type, arg3_type>* chainsig)
        {
            lock_block lock(this);
            this->connect_signal(chainsig);
        }

        void disconnect_signal(signal3<arg1_type, arg2_type, arg3_type>* chainsig)
        {
            lock_block lock(this);
            this->disconnect_connected(chainsig);
        }

        void emit(arg1_type a1, arg2_type a2, arg3_type a3)
        {
            lock_block lock(this);
            const_iterator itNext, it = this->m_connected_slots.begin();
            const_iterator itEnd = this->m_connected_slots.end();

            while (it != itEnd)
            {
                itNext = it;
                ++itNext;

                (*it)->emit(a1, a2, a3);

                it = itNext;
            }

            sig_iterator siNext, si = this->connected_signals.begin();
            sig_iterator siEnd = this->connected_signals.end();

            while (si != siEnd)
            {
                siNext = si;
                ++siNext;

                static_cast<signal3<arg1_type, arg2_type, arg3_type>*>(*si)->emit(a1, a2, a3);

                si = siNext;
            }
        }

        void operator()(arg1_type a1, arg2_type a2, arg3_type a3)
        {
            lock_block lock(this);
            const_iterator itNext, it = this->m_connected_slots.begin();
            const_iterator itEnd = this->m_connected_slots.end();

            while (it != itEnd)
            {
                itNext = it;
                ++itNext;

                (*it)->emit(a1, a2, a3);

                it = itNext;
            }

            sig_iterator siNext, si = this->connected_signals.begin();
            sig_iterator siEnd = this->connected_signals.end();

            while (si != siEnd)
            {
                siNext = si;
                ++siNext;

                static_cast<signal3<arg1_type, arg2_type, arg3_type>*>(*si)->emit(a1, a2, a3);

                si = siNext;
            }
        }
};

template < class arg1_type, class arg2_type, class arg3_type, class arg4_type >
class signal4 : public _signal_base4 < arg1_type, arg2_type, arg3_type,
            arg4_type > , has_signals
{
    public:
        typedef typename std::list<has_signals*>::iterator sig_iterator;
        typedef typename _signal_base4<arg1_type, arg2_type, arg3_type, arg4_type>::connections_list::const_iterator const_iterator;
        typedef typename _signal_base4<arg1_type, arg2_type, arg3_type, arg4_type>::connections_list::iterator iterator;
        signal4()
        {
            ;
        }

        signal4(const signal4<arg1_type, arg2_type, arg3_type, arg4_type>& s)
                : _signal_base4<arg1_type, arg2_type, arg3_type, arg4_type>(s)
        {
            ;
        }

        virtual ~signal4()
        {
            ;
        }

        template<class desttype>
        void connect(desttype* pclass, void (desttype::*pmemfun)(arg1_type,
                     arg2_type, arg3_type, arg4_type))
        {
            lock_block lock(this);
            _connection4<desttype, arg1_type, arg2_type, arg3_type, arg4_type>*
            conn = new _connection4 < desttype, arg1_type, arg2_type, arg3_type,
            arg4_type > (pclass, pmemfun);
            this->m_connected_slots.push_back(conn);
            pclass->signal_connect(this);
        }

        template<class desttype>
        void disconnect_slot(desttype* pclass, void (desttype::*pmemfun)(arg1_type,
                             arg2_type, arg3_type, arg4_type))
        {
            lock_block lock(this);
            iterator itNext, it = this->m_connected_slots.begin();
            iterator itEnd = this->m_connected_slots.end();


            while (it != itEnd)
            {
                itNext = it;
                ++itNext;
                _connection4<desttype, arg1_type, arg2_type, arg3_type, arg4_type>* temp = dynamic_cast<_connection4<desttype, arg1_type, arg2_type, arg3_type, arg4_type> *>((*it));
                if (temp->m_pobject == pclass && temp->m_pmemfun == pmemfun)
                {
                    delete (*it);
                    this->m_connected_slots.erase(it);
                    pclass->signal_disconnect(this);
                    break;
                }

                it = itNext;
            }
        }

        void connect(signal4<arg1_type, arg2_type, arg3_type, arg4_type>* chainsig)
        {
            lock_block lock(this);
            this->connect_signal(chainsig);
        }

        void disconnect_signal(signal4<arg1_type, arg2_type, arg3_type, arg4_type>* chainsig)
        {
            lock_block lock(this);
            this->disconnect_connected(chainsig);
        }

        void emit(arg1_type a1, arg2_type a2, arg3_type a3, arg4_type a4)
        {
            lock_block lock(this);
            const_iterator itNext, it = this->m_connected_slots.begin();
            const_iterator itEnd = this->m_connected_slots.end();

            while (it != itEnd)
            {
                itNext = it;
                ++itNext;

                (*it)->emit(a1, a2, a3, a4);

                it = itNext;
            }

            sig_iterator siNext, si = this->connected_signals.begin();
            sig_iterator siEnd = this->connected_signals.end();

            while (si != siEnd)
            {
                siNext = si;
                ++siNext;

                static_cast<signal4<arg1_type, arg2_type, arg3_type, arg4_type>*>(*si)->emit(a1, a2, a3, a4);

                si = siNext;
            }
        }

        void operator()(arg1_type a1, arg2_type a2, arg3_type a3, arg4_type a4)
        {
            lock_block lock(this);
            const_iterator itNext, it = this->m_connected_slots.begin();
            const_iterator itEnd = this->m_connected_slots.end();

            while (it != itEnd)
            {
                itNext = it;
                ++itNext;

                (*it)->emit(a1, a2, a3, a4);

                it = itNext;
            }

            sig_iterator siNext, si = this->connected_signals.begin();
            sig_iterator siEnd = this->connected_signals.end();

            while (si != siEnd)
            {
                siNext = si;
                ++siNext;

                static_cast<signal4<arg1_type, arg2_type, arg3_type, arg4_type>*>(*si)->emit(a1, a2, a3, a4);

                si = siNext;
            }
        }
};

template < class arg1_type, class arg2_type, class arg3_type, class arg4_type,
class arg5_type >
class signal5 : public _signal_base5 < arg1_type, arg2_type, arg3_type,
            arg4_type, arg5_type > , has_signals
{
    public:
        typedef typename std::list<has_signals*>::iterator sig_iterator;
        typedef typename _signal_base5<arg1_type, arg2_type, arg3_type, arg4_type, arg5_type>::connections_list::const_iterator const_iterator;
        typedef typename _signal_base5<arg1_type, arg2_type, arg3_type, arg4_type, arg5_type>::connections_list::iterator iterator;
        signal5()
        {
            ;
        }

        signal5(const signal5 < arg1_type, arg2_type, arg3_type, arg4_type,
                arg5_type > & s)
                : _signal_base5 < arg1_type, arg2_type, arg3_type, arg4_type,
                arg5_type > (s)
        {
            ;
        }

        virtual ~signal5()
        {
            ;
        }

        template<class desttype>
        void connect(desttype* pclass, void (desttype::*pmemfun)(arg1_type,
                     arg2_type, arg3_type, arg4_type, arg5_type))
        {
            lock_block lock(this);
            _connection5 < desttype, arg1_type, arg2_type, arg3_type, arg4_type,
            arg5_type > * conn = new _connection5 < desttype, arg1_type, arg2_type,
            arg3_type, arg4_type, arg5_type > (pclass, pmemfun);
            this->m_connected_slots.push_back(conn);
            pclass->signal_connect(this);
        }

        template<class desttype>
        void disconnect_slot(desttype* pclass, void (desttype::*pmemfun)(arg1_type,
                             arg2_type, arg3_type, arg4_type, arg5_type))
        {
            lock_block lock(this);
            iterator itNext, it = this->m_connected_slots.begin();
            iterator itEnd = this->m_connected_slots.end();


            while (it != itEnd)
            {
                itNext = it;
                ++itNext;
                _connection5<desttype, arg1_type, arg2_type, arg3_type, arg4_type, arg5_type>* temp = dynamic_cast<_connection5<desttype, arg1_type, arg2_type, arg3_type, arg4_type, arg5_type> *>((*it));
                if (temp->m_pobject == pclass && temp->m_pmemfun == pmemfun)
                {
                    delete (*it);
                    this->m_connected_slots.erase(it);
                    pclass->signal_disconnect(this);
                    break;
                }

                it = itNext;
            }
        }

        void connect(signal5<arg1_type, arg2_type, arg3_type, arg4_type, arg5_type>* chainsig)
        {
            lock_block lock(this);
            this->connect_signal(chainsig);
        }

        void disconnect_signal(signal5<arg1_type, arg2_type, arg3_type, arg4_type, arg5_type>* chainsig)
        {
            lock_block lock(this);
            this->disconnect_connected(chainsig);
        }

        void emit(arg1_type a1, arg2_type a2, arg3_type a3, arg4_type a4,
                  arg5_type a5)
        {
            lock_block lock(this);
            const_iterator itNext, it = this->m_connected_slots.begin();
            const_iterator itEnd = this->m_connected_slots.end();

            while (it != itEnd)
            {
                itNext = it;
                ++itNext;

                (*it)->emit(a1, a2, a3, a4, a5);

                it = itNext;
            }

            sig_iterator siNext, si = this->connected_signals.begin();
            sig_iterator siEnd = this->connected_signals.end();

            while (si != siEnd)
            {
                siNext = si;
                ++siNext;

                static_cast<signal5<arg1_type, arg2_type, arg3_type, arg4_type, arg5_type>*>(*si)->emit(a1, a2, a3, a4, a5);

                si = siNext;
            }
        }

        void operator()(arg1_type a1, arg2_type a2, arg3_type a3, arg4_type a4,
                        arg5_type a5)
        {
            lock_block lock(this);
            const_iterator itNext, it = this->m_connected_slots.begin();
            const_iterator itEnd = this->m_connected_slots.end();

            while (it != itEnd)
            {
                itNext = it;
                ++itNext;

                (*it)->emit(a1, a2, a3, a4, a5);

                it = itNext;
            }

            sig_iterator siNext, si = this->connected_signals.begin();
            sig_iterator siEnd = this->connected_signals.end();

            while (si != siEnd)
            {
                siNext = si;
                ++siNext;

                static_cast<signal5<arg1_type, arg2_type, arg3_type, arg4_type, arg5_type>*>(*si)->emit(a1, a2, a3, a4, a5);

                si = siNext;
            }
        }
};


template < class arg1_type, class arg2_type, class arg3_type, class arg4_type,
class arg5_type, class arg6_type >
class signal6 : public _signal_base6 < arg1_type, arg2_type, arg3_type,
            arg4_type, arg5_type, arg6_type > , has_signals
{
    public:
        typedef typename std::list<has_signals*>::iterator sig_iterator;
        typedef typename _signal_base6 < arg1_type, arg2_type, arg3_type,
        arg4_type, arg5_type, arg6_type >::connections_list::const_iterator const_iterator;
        typedef typename _signal_base6 < arg1_type, arg2_type, arg3_type,
        arg4_type, arg5_type, arg6_type >::connections_list::iterator iterator;
        signal6()
        {
            ;
        }

        signal6(const signal6 < arg1_type, arg2_type, arg3_type, arg4_type,
                arg5_type, arg6_type > & s)
                : _signal_base6 < arg1_type, arg2_type, arg3_type, arg4_type,
                arg5_type, arg6_type > (s)
        {
            ;
        }

        virtual ~signal6()
        {
            ;
        }

        template<class desttype>
        void connect(desttype* pclass, void (desttype::*pmemfun)(arg1_type,
                     arg2_type, arg3_type, arg4_type, arg5_type, arg6_type))
        {
            lock_block lock(this);
            _connection6 < desttype, arg1_type, arg2_type, arg3_type, arg4_type,
            arg5_type, arg6_type > * conn =
                new _connection6 < desttype, arg1_type, arg2_type, arg3_type,
            arg4_type, arg5_type, arg6_type > (pclass, pmemfun);
            this->m_connected_slots.push_back(conn);
            pclass->signal_connect(this);
        }

        template<class desttype>
        void disconnect_slot(desttype* pclass, void (desttype::*pmemfun)(arg1_type,
                             arg2_type, arg3_type, arg4_type, arg5_type, arg6_type))
        {
            lock_block lock(this);
            iterator itNext, it = this->m_connected_slots.begin();
            iterator itEnd = this->m_connected_slots.end();


            while (it != itEnd)
            {
                itNext = it;
                ++itNext;
                _connection6<desttype, arg1_type, arg2_type, arg3_type, arg4_type, arg5_type, arg6_type>* temp = dynamic_cast<_connection6<desttype, arg1_type, arg2_type, arg3_type, arg4_type, arg5_type, arg6_type> *>((*it));
                if (temp->m_pobject == pclass && temp->m_pmemfun == pmemfun)
                {
                    delete (*it);
                    this->m_connected_slots.erase(it);
                    pclass->signal_disconnect(this);
                    break;
                }

                it = itNext;
            }
        }

        void connect(signal6<arg1_type, arg2_type, arg3_type, arg4_type, arg5_type, arg6_type>* chainsig)
        {
            lock_block lock(this);
            this->connect_signal(chainsig);
        }

        void disconnect_signal(signal6<arg1_type, arg2_type, arg3_type, arg4_type, arg5_type, arg6_type>* chainsig)
        {
            lock_block lock(this);
            this->disconnect_connected(chainsig);
        }

        void emit(arg1_type a1, arg2_type a2, arg3_type a3, arg4_type a4,
                  arg5_type a5, arg6_type a6)
        {
            lock_block lock(this);
            const_iterator itNext, it = this->m_connected_slots.begin();
            const_iterator itEnd = this->m_connected_slots.end();

            while (it != itEnd)
            {
                itNext = it;
                ++itNext;

                (*it)->emit(a1, a2, a3, a4, a5, a6);

                it = itNext;
            }

            sig_iterator siNext, si = this->connected_signals.begin();
            sig_iterator siEnd = this->connected_signals.end();

            while (si != siEnd)
            {
                siNext = si;
                ++siNext;

                static_cast<signal6<arg1_type, arg2_type, arg3_type, arg4_type, arg5_type, arg6_type>*>(*si)->emit(a1, a2, a3, a4, a5, a6);

                si = siNext;
            }
        }

        void operator()(arg1_type a1, arg2_type a2, arg3_type a3, arg4_type a4,
                        arg5_type a5, arg6_type a6)
        {
            lock_block lock(this);
            const_iterator itNext, it = this->m_connected_slots.begin();
            const_iterator itEnd = this->m_connected_slots.end();

            while (it != itEnd)
            {
                itNext = it;
                ++itNext;

                (*it)->emit(a1, a2, a3, a4, a5, a6);

                it = itNext;
            }

            sig_iterator siNext, si = this->connected_signals.begin();
            sig_iterator siEnd = this->connected_signals.end();

            while (si != siEnd)
            {
                siNext = si;
                ++siNext;

                static_cast<signal6<arg1_type, arg2_type, arg3_type, arg4_type, arg5_type, arg6_type>*>(*si)->emit(a1, a2, a3, a4, a5, a6);

                si = siNext;
            }
        }
};

template < class arg1_type, class arg2_type, class arg3_type, class arg4_type,
class arg5_type, class arg6_type, class arg7_type >
class signal7 : public _signal_base7 < arg1_type, arg2_type, arg3_type,
            arg4_type, arg5_type, arg6_type, arg7_type > , has_signals
{
    public:
        typedef typename std::list<has_signals*>::iterator sig_iterator;
        typedef typename _signal_base7 < arg1_type, arg2_type, arg3_type,
        arg4_type, arg5_type, arg6_type, arg7_type >::connections_list::const_iterator const_iterator;
        typedef typename _signal_base7 < arg1_type, arg2_type, arg3_type,
        arg4_type, arg5_type, arg6_type, arg7_type >::connections_list::iterator iterator;
        signal7()
        {
            ;
        }

        signal7(const signal7 < arg1_type, arg2_type, arg3_type, arg4_type,
                arg5_type, arg6_type, arg7_type > & s)
                : _signal_base7 < arg1_type, arg2_type, arg3_type, arg4_type,
                arg5_type, arg6_type, arg7_type > (s)
        {
            ;
        }

        virtual ~signal7()
        {
            ;
        }

        template<class desttype>
        void connect(desttype* pclass, void (desttype::*pmemfun)(arg1_type,
                     arg2_type, arg3_type, arg4_type, arg5_type, arg6_type,
                     arg7_type))
        {
            lock_block lock(this);
            _connection7 < desttype, arg1_type, arg2_type, arg3_type, arg4_type,
            arg5_type, arg6_type, arg7_type > * conn =
                new _connection7 < desttype, arg1_type, arg2_type, arg3_type,
            arg4_type, arg5_type, arg6_type, arg7_type > (pclass, pmemfun);
            this->m_connected_slots.push_back(conn);
            pclass->signal_connect(this);
        }

        template<class desttype>
        void disconnect_slot(desttype* pclass, void (desttype::*pmemfun)(arg1_type,
                             arg2_type, arg3_type, arg4_type, arg5_type, arg6_type, arg7_type))
        {
            lock_block lock(this);
            iterator itNext, it = this->m_connected_slots.begin();
            iterator itEnd = this->m_connected_slots.end();


            while (it != itEnd)
            {
                itNext = it;
                ++itNext;
                _connection7<desttype, arg1_type, arg2_type, arg3_type, arg4_type, arg5_type, arg6_type, arg7_type>* temp = dynamic_cast<_connection7<desttype, arg1_type, arg2_type, arg3_type, arg4_type, arg5_type, arg6_type, arg7_type> *>((*it));
                if (temp->m_pobject == pclass && temp->m_pmemfun == pmemfun)
                {
                    delete (*it);
                    this->m_connected_slots.erase(it);
                    pclass->signal_disconnect(this);
                    break;
                }

                it = itNext;
            }
        }

        void connect(signal7<arg1_type, arg2_type, arg3_type, arg4_type, arg5_type, arg6_type, arg7_type>* chainsig)
        {
            lock_block lock(this);
            this->connect_signal(chainsig);
        }

        void disconnect_signal(signal7<arg1_type, arg2_type, arg3_type, arg4_type, arg5_type, arg6_type, arg7_type>* chainsig)
        {
            lock_block lock(this);
            this->disconnect_connected(chainsig);
        }

        void emit(arg1_type a1, arg2_type a2, arg3_type a3, arg4_type a4,
                  arg5_type a5, arg6_type a6, arg7_type a7)
        {
            lock_block lock(this);
            const_iterator itNext, it = this->m_connected_slots.begin();
            const_iterator itEnd = this->m_connected_slots.end();

            while (it != itEnd)
            {
                itNext = it;
                ++itNext;

                (*it)->emit(a1, a2, a3, a4, a5, a6, a7);

                it = itNext;
            }

            sig_iterator siNext, si = this->connected_signals.begin();
            sig_iterator siEnd = this->connected_signals.end();

            while (si != siEnd)
            {
                siNext = si;
                ++siNext;

                static_cast<signal7<arg1_type, arg2_type, arg3_type, arg4_type, arg5_type, arg6_type, arg7_type>*>(*si)->emit(a1, a2, a3, a4, a5, a6, a7);

                si = siNext;
            }
        }

        void operator()(arg1_type a1, arg2_type a2, arg3_type a3, arg4_type a4,
                        arg5_type a5, arg6_type a6, arg7_type a7)
        {
            lock_block lock(this);
            const_iterator itNext, it = this->m_connected_slots.begin();
            const_iterator itEnd = this->m_connected_slots.end();

            while (it != itEnd)
            {
                itNext = it;
                ++itNext;

                (*it)->emit(a1, a2, a3, a4, a5, a6, a7);

                it = itNext;
            }

            sig_iterator siNext, si = this->connected_signals.begin();
            sig_iterator siEnd = this->connected_signals.end();

            while (si != siEnd)
            {
                siNext = si;
                ++siNext;

                static_cast<signal7<arg1_type, arg2_type, arg3_type, arg4_type, arg5_type, arg6_type, arg7_type>*>(*si)->emit(a1, a2, a3, a4, a5, a6, a7);

                si = siNext;
            }
        }
};

template < class arg1_type, class arg2_type, class arg3_type, class arg4_type,
class arg5_type, class arg6_type, class arg7_type, class arg8_type >
class signal8 : public _signal_base8 < arg1_type, arg2_type, arg3_type,
            arg4_type, arg5_type, arg6_type, arg7_type, arg8_type > , has_signals
{
    public:
        typedef typename std::list<has_signals*>::iterator sig_iterator;
        typedef typename _signal_base8 < arg1_type, arg2_type, arg3_type,
        arg4_type, arg5_type, arg6_type, arg7_type, arg8_type >::connections_list::const_iterator const_iterator;
        typedef typename _signal_base8 < arg1_type, arg2_type, arg3_type,
        arg4_type, arg5_type, arg6_type, arg7_type, arg8_type >::connections_list::iterator iterator;
        signal8()
        {
            ;
        }

        signal8(const signal8 < arg1_type, arg2_type, arg3_type, arg4_type,
                arg5_type, arg6_type, arg7_type, arg8_type > & s)
                : _signal_base8 < arg1_type, arg2_type, arg3_type, arg4_type,
                arg5_type, arg6_type, arg7_type, arg8_type > (s)
        {
            ;
        }

        virtual ~signal8()
        {
            ;
        }

        template<class desttype>
        void connect(desttype* pclass, void (desttype::*pmemfun)(arg1_type,
                     arg2_type, arg3_type, arg4_type, arg5_type, arg6_type,
                     arg7_type, arg8_type))
        {
            lock_block lock(this);
            _connection8 < desttype, arg1_type, arg2_type, arg3_type, arg4_type,
            arg5_type, arg6_type, arg7_type, arg8_type > * conn =
                new _connection8 < desttype, arg1_type, arg2_type, arg3_type,
            arg4_type, arg5_type, arg6_type, arg7_type,
            arg8_type > (pclass, pmemfun);
            this->m_connected_slots.push_back(conn);
            pclass->signal_connect(this);
        }

        template<class desttype>
        void disconnect_slot(desttype* pclass, void (desttype::*pmemfun)(arg1_type,
                             arg2_type, arg3_type, arg4_type, arg5_type, arg6_type, arg7_type, arg8_type))
        {
            lock_block lock(this);
            iterator itNext, it = this->m_connected_slots.begin();
            iterator itEnd = this->m_connected_slots.end();


            while (it != itEnd)
            {
                itNext = it;
                ++itNext;
                _connection8<desttype, arg1_type, arg2_type, arg3_type, arg4_type, arg5_type, arg6_type, arg7_type, arg8_type>* temp = dynamic_cast<_connection8<desttype, arg1_type, arg2_type, arg3_type, arg4_type, arg5_type, arg6_type, arg7_type, arg8_type> *>((*it));
                if (temp->m_pobject == pclass && temp->m_pmemfun == pmemfun)
                {
                    delete (*it);
                    this->m_connected_slots.erase(it);
                    pclass->signal_disconnect(this);
                    break;
                }

                it = itNext;
            }
        }

        void connect(signal8<arg1_type, arg2_type, arg3_type, arg4_type, arg5_type, arg6_type, arg7_type, arg8_type>* chainsig)
        {
            lock_block lock(this);
            this->connect_signal(chainsig);
        }

        void disconnect_signal(signal8<arg1_type, arg2_type, arg3_type, arg4_type, arg5_type, arg6_type, arg7_type, arg8_type>* chainsig)
        {
            lock_block lock(this);
            this->disconnect_connected(chainsig);
        }

        void emit(arg1_type a1, arg2_type a2, arg3_type a3, arg4_type a4,
                  arg5_type a5, arg6_type a6, arg7_type a7, arg8_type a8)
        {
            lock_block lock(this);
            const_iterator itNext, it = this->m_connected_slots.begin();
            const_iterator itEnd = this->m_connected_slots.end();

            while (it != itEnd)
            {
                itNext = it;
                ++itNext;

                (*it)->emit(a1, a2, a3, a4, a5, a6, a7, a8);

                it = itNext;
            }

            sig_iterator siNext, si = this->connected_signals.begin();
            sig_iterator siEnd = this->connected_signals.end();

            while (si != siEnd)
            {
                siNext = si;
                ++siNext;

                static_cast<signal8<arg1_type, arg2_type, arg3_type, arg4_type, arg5_type, arg6_type, arg7_type, arg8_type>*>(*si)->emit(a1, a2, a3, a4, a5, a6, a7, a8);

                si = siNext;
            }
        }

        void operator()(arg1_type a1, arg2_type a2, arg3_type a3, arg4_type a4,
                        arg5_type a5, arg6_type a6, arg7_type a7, arg8_type a8)
        {
            lock_block lock(this);
            const_iterator itNext, it = this->m_connected_slots.begin();
            const_iterator itEnd = this->m_connected_slots.end();

            while (it != itEnd)
            {
                itNext = it;
                ++itNext;

                (*it)->emit(a1, a2, a3, a4, a5, a6, a7, a8);

                it = itNext;
            }

            sig_iterator siNext, si = this->connected_signals.begin();
            sig_iterator siEnd = this->connected_signals.end();

            while (si != siEnd)
            {
                siNext = si;
                ++siNext;

                static_cast<signal8<arg1_type, arg2_type, arg3_type, arg4_type, arg5_type, arg6_type, arg7_type, arg8_type>*>(*si)->emit(a1, a2, a3, a4, a5, a6, a7, a8);

                si = siNext;
            }
        }
};

}; // namespace sigslot

#endif // SIGSLOT_H__



