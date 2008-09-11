/***************************************************************
 * Name:      atomic.h
 * Purpose:   Generic class for atomic operations
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-09-10
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   WxWindows License
 * Comments:  This is just a wrapper for some GCC atomic
              operations, but we could include some assembly
              code instead.
 **************************************************************/

#ifndef sy_atomic_h
#define sy_atomic_h
/** @brief Atomic primitives for multithreading. Available only for GCC.
 *
 *  Here we implement an interface to the atomic primitives for lock-free and wait-free programming.
 *  Compare-and-exchange (CAS) compares a pointer with an old value, and if it's the same,
 *  set it to a new value.
 *  @see http://gcc.gnu.org/onlinedocs/gcc-4.1.1/gcc/Atomic-Builtins.html#Atomic-Builtins
 *
 *  @warning lock-free programming is very difficult to debug; Use with care.
 *  @see http://en.wikipedia.org/wiki/Lock-free_and_wait-free_algorithms
 *  @see http://en.wikipedia.org/wiki/ABA_problem
 *
 */
class syAtomic {
    public:

        /** @brief Returns true on success. Curval obtains the current value of *ptr.
         *  We add an additional parameter to hold the old value because both the value and the return are boolean.
         */
        static bool bool_CAS(bool* ptr, bool oldval, bool newval);
        static bool bool_CAS(int* ptr, int oldval, int newval);
        static bool bool_CAS(unsigned int* ptr, unsigned int oldval, unsigned int newval);
        static bool bool_CAS(long* ptr, long oldval, long newval);
        static bool bool_CAS(unsigned long* ptr, unsigned long oldval, unsigned long newval);
        static bool bool_CAS(char* ptr, char oldval, char newval);
        static bool bool_CAS(unsigned char* ptr, unsigned char oldval, unsigned char newval);
        static bool bool_CAS(void** ptr, void* oldval, void* newval);

        static bool val_CAS(bool* ptr, bool oldval, bool newval);
        static int val_CAS(int* ptr, int oldval, int newval);
        static unsigned int val_CAS(unsigned int* ptr, unsigned int oldval, unsigned int newval);
        static long val_CAS(long* ptr, long oldval, long newval);
        static unsigned long val_CAS(unsigned long* ptr, unsigned long oldval, unsigned long newval);
        static char val_CAS(char* ptr, char oldval, char newval);
        static char val_CAS(unsigned char* ptr, unsigned char oldval, unsigned char newval);
        static void* val_CAS(void** ptr, void* oldval, void* newval);
};

#ifdef __GNUC__
inline bool syAtomic::bool_CAS(bool* ptr, bool oldval, bool newval) {
    return __sync_bool_compare_and_swap(ptr, oldval, newval);
}

inline bool syAtomic::bool_CAS(int* ptr, int oldval, int newval) {
    return __sync_bool_compare_and_swap(ptr, oldval, newval);
}

inline bool syAtomic::bool_CAS(unsigned int* ptr, unsigned int oldval, unsigned int newval) {
    return __sync_bool_compare_and_swap(ptr, oldval, newval);
}

inline bool syAtomic::bool_CAS(long* ptr, long oldval, long newval) {
    return __sync_bool_compare_and_swap(ptr, oldval, newval);
}

inline bool syAtomic::bool_CAS(unsigned long* ptr, unsigned long oldval, unsigned long newval) {
    return __sync_bool_compare_and_swap(ptr, oldval, newval);
}

inline bool syAtomic::bool_CAS(char* ptr, char oldval, char newval) {
    return __sync_bool_compare_and_swap(ptr, oldval, newval);
}

inline bool syAtomic::bool_CAS(unsigned char* ptr, unsigned char oldval, unsigned char newval) {
    return __sync_bool_compare_and_swap(ptr, oldval, newval);
}

inline bool syAtomic::bool_CAS(void** ptr, void* oldval, void* newval) {
    return __sync_bool_compare_and_swap(ptr, oldval, newval);
}

inline bool syAtomic::val_CAS(bool* ptr, bool oldval, bool newval) {
    return __sync_val_compare_and_swap(ptr, oldval, newval);
}

inline int syAtomic::val_CAS(int* ptr, int oldval, int newval) {
    return __sync_val_compare_and_swap(ptr, oldval, newval);
}

inline unsigned int syAtomic::val_CAS(unsigned int* ptr, unsigned int oldval, unsigned int newval) {
    return __sync_val_compare_and_swap(ptr, oldval, newval);
}

inline long syAtomic::val_CAS(long* ptr, long oldval, long newval) {
    return __sync_val_compare_and_swap(ptr, oldval, newval);
}

inline unsigned long syAtomic::val_CAS(unsigned long* ptr, unsigned long oldval, unsigned long newval) {
    return __sync_val_compare_and_swap(ptr, oldval, newval);
}

inline char syAtomic::val_CAS(char* ptr, char oldval, char newval) {
    return __sync_val_compare_and_swap(ptr, oldval, newval);
}

inline char syAtomic::val_CAS(unsigned char* ptr, unsigned char oldval, unsigned char newval) {
    return __sync_val_compare_and_swap(ptr, oldval, newval);
}

inline void* syAtomic::val_CAS(void** ptr, void* oldval, void* newval) {
    return __sync_val_compare_and_swap(ptr, oldval, newval);
}
#else
    #error sythread.h needs a GCC-compatible compiler; otherwise insert atomic operations here.
#endif

#endif
