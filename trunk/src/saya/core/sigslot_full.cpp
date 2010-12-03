/**************************************************************************************
 * Name:      sigslot_full.cpp
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

#include "sigslot_full.h"
#include "sythread.h"

namespace sigslot {

// ----------------
// begin _signal_mp
// ----------------

class _signal_mp::Data {
    public:
        sySharedMutex<_signal_mp> m_signal_mutex;
};

_signal_mp::_signal_mp() : m_Data(new Data) {}
_signal_mp::~_signal_mp() {
    delete m_Data;
    m_Data = 0;
}

bool _signal_mp::sig_lock() {
    return m_Data->m_signal_mutex()->SafeLock();
}

bool _signal_mp::sig_locked() {
    return !(m_Data->m_signal_mutex()->IsUnlocked());
}

void _signal_mp::sig_unlock() {
    m_Data->m_signal_mutex()->Unlock();
}

bool _signal_mp::sig_safe() {
    return !syThread::MustAbort();
}
// --------------
// end _signal_mp
// --------------

// ----------------
// begin sig_locker
// ----------------

sig_locker::sig_locker(_signal_mp& sig) : m_sig(sig) {
    Lock();
}

/** Destructor */
sig_locker::~sig_locker() {
    Unlock();
}

/** (re)Locks the mutex */
bool sig_locker::Lock() {
    return m_sig.sig_lock();
}

/** Unlocks the mutex */
void sig_locker::Unlock() {
    m_sig.sig_unlock();
}

/** @return true if the mutex is currently locked and owned by the current thread. */
bool sig_locker::IsLocked() {
    return m_sig.sig_locked();
}

// --------------
// end sig_locker
// --------------

}; // namespace sigslot