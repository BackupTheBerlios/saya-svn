/**************************************************************************************
 * Name:      sigslot.h
 * Purpose:   Implementation of a platform-independent signal and slot library.
 * Original
 *   author:  Sarah Thompson, 2002.
 * Modified
 *   by:      Rick Garcia
 * Created:   2010-11-28
 * Modified:  2010-12-05
 * Copyright: (c) 2010, Rick Garcia
 * License:   LGPL Licence version 3.0 or later
 * Notes:
 *              Original code available at http://sigslot.sourceforge.net/
 *              Modified by Rick Garcia 05/12/2010:
 *              - Completely discarded Miguel A. Gavidia's modifications (sorry, found a better way)
 *              - Moved the emitting code to a _signal_call_base class to get rid of many templates
 *              - Replaced multiple template class definitions with CPP macros
 *              - Added signal chaining support through the _connection_base class
 *              - Replaced thread policies with a special shared mutex template.
 **************************************************************************************/

#ifndef SIGSLOT_H__
#define SIGSLOT_H__

#undef emit

namespace sigslot {

class has_slots;
class _signal_call_base;
class _connection_base;

/** @class _signal_base
 *  @brief This is the basis for our signal classes.
 *
 *  Connections are added and removed via the insert_connection and remove_connection.
 *  To remove a given connection, you create a temporary connection to a given object. _signal_base::remove_connection
 *  will then remove all matching connections.
 *  @see sigslot_full.h
 */

class _signal_base {
    friend class has_slots;
    public:
        /** Constructor. */
        _signal_base();

        /** Destructor. */
        virtual ~_signal_base();

        /** Copy constructor. */
        _signal_base(const _signal_base& s);

        /** Disconnects the signal from a given object. */
        void disconnect(has_slots* target);

        /** Disconnects all the slots. */
        void disconnect_all();

        /** Emits a signal to all the connected slots. */
        void emit_base(const _signal_call_base* pcall);

    protected:

        /** Adds a connection to the signal. Used by the signal template classes.
         *  @note The connection is copied and stored internally. */
        void add_connection(const _connection_base &conn);

        /** Removes a connection from the signal. Used by the signal template classes. */
        void remove_connection(const _connection_base &conn);

    private:

        /** Duplicates the current connections to a source object into a destination object. Used by has_slots copy constructor. */
        void slot_duplicate(const has_slots* oldtarget, has_slots* newtarget);

        /** Used by has_slots::disconnect_all */
        void kill_connection(has_slots* target);

        class Data;
        friend class Data;
        Data* m_Data;
};

/** @class has_slots
 *  @brief All classes containing chained signals and/or slots must be subclasses of has_slots.
 */
class has_slots {
    public:
        /** Constructor. */
        has_slots();

        /** Copy constructor. */
        has_slots(const has_slots& hs);

        /** Virtual dſestructor. */
        virtual ~has_slots();

        /** Disconnects all slots. */
        void disconnect_all();

    private:
        friend class _signal_base;

        /** Connects itself into a signal. */
        void connect_signal(_signal_base* signal);

        /** Disconnects from a signal. */
        void disconnect_signal(_signal_base* signal);

        class Data;
        friend class Data;
        Data* m_Data;
};

/** @class _connection_base
 *  @brief Implements a functionoid pointing to a class member function. Its parameters are of type signal_call_base.
 *
 *  To be able to work with signals and slots, all its functions are pure virtuals, so that we can implement
 *  The appropriate connections using templates.
 *  @see sigslot_full.h
 */

class _connection_base {
    public:

        /** Default constructor */
        _connection_base() : m_pobject(0) {}

        /** Virtual destructor */
        virtual ~_connection_base() {}

        /** @return the destination object. */
        has_slots* getdest() const { return m_pobject; }

        /** Clones the current connection. */
        virtual _connection_base* clone() = 0;

        /** Duplicates the current connections to another object. */
        virtual _connection_base* duplicate(has_slots* pnewdest) = 0;

        /** Emits a signal to the connected slots. */
        virtual void emit(const _signal_call_base* pcall) = 0;

        /** Compares with another connection.
         *  @return true if both connections are of the same type and point to the same objects and slots; false otherwise.
         */
        virtual bool equals(const _connection_base* other) const = 0;
    protected:
        has_slots* m_pobject;
};

class _signal_call_base {
    public:
        _signal_call_base() {}
        virtual ~_signal_call_base() {}
};

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

#define SIGSLOT_CALL_MEMBERS0
#define SIGSLOT_CALL_MEMBERS1 arg1_type m_a1
#define SIGSLOT_CALL_MEMBERS2 SIGSLOT_CALL_MEMBERS1 ;arg2_type m_a2
#define SIGSLOT_CALL_MEMBERS3 SIGSLOT_CALL_MEMBERS2 ;arg3_type m_a3
#define SIGSLOT_CALL_MEMBERS4 SIGSLOT_CALL_MEMBERS3 ;arg4_type m_a4
#define SIGSLOT_CALL_MEMBERS5 SIGSLOT_CALL_MEMBERS4 ;arg5_type m_a5
#define SIGSLOT_CALL_MEMBERS6 SIGSLOT_CALL_MEMBERS5 ;arg6_type m_a6
#define SIGSLOT_CALL_MEMBERS7 SIGSLOT_CALL_MEMBERS6 ;arg7_type m_a7
#define SIGSLOT_CALL_MEMBERS8 SIGSLOT_CALL_MEMBERS7 ;arg8_type m_a8

#define SIGSLOT_CALL_ASSIGN0
#define SIGSLOT_CALL_ASSIGN1 : m_a1(a1)
#define SIGSLOT_CALL_ASSIGN2 SIGSLOT_CALL_ASSIGN1 ,m_a2(a2)
#define SIGSLOT_CALL_ASSIGN3 SIGSLOT_CALL_ASSIGN2 ,m_a3(a3)
#define SIGSLOT_CALL_ASSIGN4 SIGSLOT_CALL_ASSIGN3 ,m_a4(a4)
#define SIGSLOT_CALL_ASSIGN5 SIGSLOT_CALL_ASSIGN4 ,m_a5(a5)
#define SIGSLOT_CALL_ASSIGN6 SIGSLOT_CALL_ASSIGN5 ,m_a6(a6)
#define SIGSLOT_CALL_ASSIGN7 SIGSLOT_CALL_ASSIGN6 ,m_a7(a7)
#define SIGSLOT_CALL_ASSIGN8 SIGSLOT_CALL_ASSIGN7 ,m_a8(a8)

#define SIGSLOT_CALL_EMIT0
#define SIGSLOT_CALL_EMIT1 call->m_a1
#define SIGSLOT_CALL_EMIT2 SIGSLOT_CALL_EMIT1 ,call->m_a2
#define SIGSLOT_CALL_EMIT3 SIGSLOT_CALL_EMIT2 ,call->m_a3
#define SIGSLOT_CALL_EMIT4 SIGSLOT_CALL_EMIT3 ,call->m_a4
#define SIGSLOT_CALL_EMIT5 SIGSLOT_CALL_EMIT4 ,call->m_a5
#define SIGSLOT_CALL_EMIT6 SIGSLOT_CALL_EMIT5 ,call->m_a6
#define SIGSLOT_CALL_EMIT7 SIGSLOT_CALL_EMIT6 ,call->m_a7
#define SIGSLOT_CALL_EMIT8 SIGSLOT_CALL_EMIT7 ,call->m_a8


#define SIGSLOT_PARAMLIST0 0
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
#define SIGSLOT_CALL_VMEMBERS(x) SIGSLOT_CALL_MEMBERS ## x
#define SIGSLOT_CALL_VASSIGN(x) SIGSLOT_CALL_ASSIGN ## x
#define SIGSLOT_CALL_VEMIT(x) SIGSLOT_CALL_EMIT ## x
#define SIGSLOT_VPARAMLIST(x) SIGSLOT_PARAMLIST ## x

#define SIGSLOT_TEMPLATE_CALL(x) \
template<SIGSLOT_VCLASSLIST(x)> class _signal_call_ ## x : public _signal_call_base { \
    public: \
        SIGSLOT_CALL_VMEMBERS(x); \
        _signal_call_ ## x(SIGSLOT_VNAMEDARGLIST(x)) SIGSLOT_CALL_VASSIGN(x) {} \
        virtual ~_signal_call_ ## x() {} \
}

SIGSLOT_TEMPLATE_CALL(0);
SIGSLOT_TEMPLATE_CALL(1);
SIGSLOT_TEMPLATE_CALL(2);
SIGSLOT_TEMPLATE_CALL(3);
SIGSLOT_TEMPLATE_CALL(4);
SIGSLOT_TEMPLATE_CALL(5);
SIGSLOT_TEMPLATE_CALL(6);
SIGSLOT_TEMPLATE_CALL(7);
SIGSLOT_TEMPLATE_CALL(8);

class signal0;

template<class dest_type, class arg0_type = void*> class _connection0 : public _connection_base {
    public:
        _connection0() : m_psignal(0),m_pmemfun(0) {}
        _connection0(dest_type* pobject, void (dest_type::*pmemfun)()) : m_psignal(0), m_pmemfun(pmemfun) { m_pobject = pobject; }
        _connection0(dest_type* pobject, signal0 dest_type::*psignal) : m_psignal(psignal), m_pmemfun(0) { m_pobject = pobject; }
        _connection0(dest_type* pobject, signal0 dest_type::*psignal, void (dest_type::*pmemfun)()) : m_psignal(psignal), m_pmemfun(pmemfun) {  m_pobject = pobject; }
        virtual ~_connection0() {}
        virtual _connection_base* clone() { return new _connection0<dest_type, arg0_type>(*this); }
        virtual _connection_base* duplicate(has_slots* pnewdest) {
            return new _connection0<dest_type, arg0_type>((dest_type *)pnewdest, m_psignal, m_pmemfun);
        }
        virtual void emit(const _signal_call_base* pcall) {
            const _signal_call_0<void*>* call = dynamic_cast<const _signal_call_0<void*>* >(pcall);
            dest_type* callee = dynamic_cast<dest_type*>(m_pobject);
            if(call && callee) {
                if(m_pmemfun) {
                    (callee->*m_pmemfun)();
                } else if(m_psignal) {
                    (callee->*m_psignal).emit_base(pcall);
                }
            }
        }
        virtual bool equals(const _connection_base *other) const {
            const _connection0<dest_type, arg0_type> *pconn = dynamic_cast<const _connection0<dest_type, arg0_type> * >(other);
            if( !pconn || (m_pobject != pconn->m_pobject) || (m_pmemfun != pconn->m_pmemfun) || (m_psignal != pconn->m_psignal) ) { return false; }
            return true;
        }
        signal0 dest_type::* m_psignal;
        void (dest_type::* m_pmemfun)();
};


#define SIGSLOT_TEMPLATE_CONNECTION(x) \
template<SIGSLOT_VCLASSLIST(x)> class signal## x; \
template<class dest_type SIGSLOT_VCCLASSLIST(x)> class _connection ## x : public _connection_base { \
    public: \
        _connection ## x() : m_psignal(0),m_pmemfun(0) {} \
        _connection ## x(dest_type* pobject, void (dest_type::*pmemfun)(SIGSLOT_VFUNCARGLIST(x))) : m_psignal(0), m_pmemfun(pmemfun) { m_pobject = pobject; } \
        _connection ## x(dest_type* pobject, signal## x<SIGSLOT_VARGLIST(x)> dest_type::*psignal) : m_psignal(psignal), m_pmemfun(0) { m_pobject = pobject; } \
        _connection ## x(dest_type* pobject, signal## x<SIGSLOT_VARGLIST(x)> dest_type::*psignal, void (dest_type::*pmemfun)(SIGSLOT_VFUNCARGLIST(x))) : m_psignal(psignal), m_pmemfun(pmemfun) {  m_pobject = pobject; } \
        virtual ~_connection ## x() {} \
        virtual _connection_base* clone() { return new _connection ## x<dest_type SIGSLOT_VCARGLIST(x)>(*this); } \
        virtual _connection_base* duplicate(has_slots* pnewdest) { \
            return new _connection ## x<dest_type SIGSLOT_VCARGLIST(x)>((dest_type *)pnewdest, m_psignal, m_pmemfun); \
        } \
        virtual void emit(const _signal_call_base* pcall) {  \
            const _signal_call_ ## x<SIGSLOT_VARGLIST(x)>* call = dynamic_cast<const _signal_call_ ## x<SIGSLOT_VARGLIST(x)>* >(pcall); \
            dest_type* callee = dynamic_cast<dest_type*>(m_pobject); \
            if(call && callee) { \
                if(m_pmemfun) { \
                    (callee->*m_pmemfun)(SIGSLOT_CALL_VEMIT(x)); \
                } else if(m_psignal) { \
                    (callee->*m_psignal).emit_base(pcall); \
                } \
            } \
        } \
        virtual bool equals(const _connection_base *other) const { \
            const _connection ## x<dest_type SIGSLOT_VCARGLIST(x)> *pconn = dynamic_cast<const _connection ## x<dest_type SIGSLOT_VCARGLIST(x)> * >(other); \
            if( !pconn || (m_pobject != pconn->m_pobject) || (m_pmemfun != pconn->m_pmemfun) || (m_psignal != pconn->m_psignal) ) { return false; } \
            return true; \
        } \
        signal## x<SIGSLOT_VARGLIST(x)> dest_type::* m_psignal; \
        void (dest_type::* m_pmemfun)(SIGSLOT_VFUNCARGLIST(x)); \
}

SIGSLOT_TEMPLATE_CONNECTION(1);
SIGSLOT_TEMPLATE_CONNECTION(2);
SIGSLOT_TEMPLATE_CONNECTION(3);
SIGSLOT_TEMPLATE_CONNECTION(4);
SIGSLOT_TEMPLATE_CONNECTION(5);
SIGSLOT_TEMPLATE_CONNECTION(6);
SIGSLOT_TEMPLATE_CONNECTION(7);
SIGSLOT_TEMPLATE_CONNECTION(8);

class signal0 : public _signal_base {
    public:
        signal0() {}
        signal0(const signal0 & s) : _signal_base(s) {}
        virtual ~signal0() {}

        template<class desttype> void connect(desttype* pobj, void (desttype::*pmemfun)()) {
            _connection0<desttype> conn(pobj, pmemfun);
            this->_signal_base::add_connection(conn);
        }
        template<class desttype> void connect(desttype* pobj, signal0 desttype::* psignal) {
            _connection0<desttype> conn(pobj, psignal);
            this->_signal_base::add_connection(conn);
        }

        template<class desttype> void disconnect(desttype* pobj, void (desttype::*pmemfun)()) {
            _connection0<desttype> conn(pobj, pmemfun);
            this->_signal_base::remove_connection(conn);
        }
        template<class desttype> void disconnect(desttype* pobj, signal0 desttype::* psignal) {
            _connection0<desttype> conn(pobj, psignal);
            this->_signal_base::remove_connection(conn);
        }

        inline void emit() {
            _signal_call_0<void*> call;
            this->_signal_base::emit_base(&call);
        }

        inline void operator()() {
            _signal_call_0<void*> call;
            this->_signal_base::emit_base(&call);
        }
};

#define SIGSLOT_TEMPLATE_SIGNAL(x) \
template <SIGSLOT_VCLASSLIST(x)> class signal## x : public _signal_base { \
    public: \
        signal## x() {} \
        signal## x(const signal## x<SIGSLOT_VARGLIST(x)> & s) : _signal_base(s) {} \
        virtual ~signal## x() {} \
 \
        template<class desttype> void connect(desttype* pobj, void (desttype::*pmemfun)(SIGSLOT_VFUNCARGLIST(x))) { \
            _connection## x< desttype SIGSLOT_VCARGLIST(x)> conn(pobj, pmemfun); \
            this->_signal_base::add_connection(conn); \
        } \
        template<class desttype> void connect(desttype* pobj, signal## x<SIGSLOT_VARGLIST(x)> desttype::* psignal) { \
            _connection## x< desttype SIGSLOT_VCARGLIST(x)> conn(pobj, psignal); \
            this->_signal_base::add_connection(conn); \
        } \
 \
        template<class desttype> void disconnect(desttype* pobj, void (desttype::*pmemfun)(SIGSLOT_VFUNCARGLIST(x))) { \
            _connection## x< desttype SIGSLOT_VCARGLIST(x)> conn(pobj, pmemfun); \
            this->_signal_base::remove_connection(conn); \
        } \
        template<class desttype> void disconnect(desttype* pobj, signal## x<SIGSLOT_VARGLIST(x)> desttype::* psignal) { \
            _connection## x< desttype SIGSLOT_VCARGLIST(x)> conn(pobj, psignal); \
            this->_signal_base::remove_connection(conn); \
        } \
 \
        inline void emit(SIGSLOT_VNAMEDARGLIST(x)) { \
            _signal_call_ ## x<SIGSLOT_VARGLIST(x)> call(SIGSLOT_VPARAMLIST(x)); \
            emit_base(&call); \
        } \
 \
        inline void operator()(SIGSLOT_VNAMEDARGLIST(x)) { \
            _signal_call_ ## x<SIGSLOT_VARGLIST(x)> call(SIGSLOT_VPARAMLIST(x)); \
            emit_base(&call); \
        } \
}

SIGSLOT_TEMPLATE_SIGNAL(1);
SIGSLOT_TEMPLATE_SIGNAL(2);
SIGSLOT_TEMPLATE_SIGNAL(3);
SIGSLOT_TEMPLATE_SIGNAL(4);
SIGSLOT_TEMPLATE_SIGNAL(5);
SIGSLOT_TEMPLATE_SIGNAL(6);
SIGSLOT_TEMPLATE_SIGNAL(7);
SIGSLOT_TEMPLATE_SIGNAL(8);

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

#undef SIGSLOT_CALL_MEMBERS0
#undef SIGSLOT_CALL_MEMBERS1
#undef SIGSLOT_CALL_MEMBERS2
#undef SIGSLOT_CALL_MEMBERS3
#undef SIGSLOT_CALL_MEMBERS4
#undef SIGSLOT_CALL_MEMBERS5
#undef SIGSLOT_CALL_MEMBERS6
#undef SIGSLOT_CALL_MEMBERS7
#undef SIGSLOT_CALL_MEMBERS8

#undef SIGSLOT_CALL_ASSIGN0
#undef SIGSLOT_CALL_ASSIGN1
#undef SIGSLOT_CALL_ASSIGN2
#undef SIGSLOT_CALL_ASSIGN3
#undef SIGSLOT_CALL_ASSIGN4
#undef SIGSLOT_CALL_ASSIGN5
#undef SIGSLOT_CALL_ASSIGN6
#undef SIGSLOT_CALL_ASSIGN7
#undef SIGSLOT_CALL_ASSIGN8

#undef SIGSLOT_CALL_EMIT0
#undef SIGSLOT_CALL_EMIT1
#undef SIGSLOT_CALL_EMIT2
#undef SIGSLOT_CALL_EMIT3
#undef SIGSLOT_CALL_EMIT4
#undef SIGSLOT_CALL_EMIT5
#undef SIGSLOT_CALL_EMIT6
#undef SIGSLOT_CALL_EMIT7
#undef SIGSLOT_CALL_EMIT8


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
#undef SIGSLOT_CALL_VMEMBERS
#undef SIGSLOT_CALL_VASSIGN
#undef SIGSLOT_CALL_VEMIT
#undef SIGSLOT_VPARAMLIST

// -----------------------
// End macros undefinition
// -----------------------

}; // namespace sig_slot

#endif // SIGSLOT_H__
