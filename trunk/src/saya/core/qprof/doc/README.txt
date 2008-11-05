Copyright (c) 2002-2003 by Hewlett-Packard Company.

This is a collection of simple profiling utilities for Linux, which we'll call
qprof version 0.4.  It is still in a state of flux.

See the file LICENSING for the terms under which this package is licensed.

Major Makefile targets:
-----------------------

qprof.so:
---------

Builds a dynamic library qprof.so, which can be prelinked into an executable
to generate a profile.  The executable usually does not need to be changed.
But the output is usually more informative if the executable has some
debug information.  Build and use instructions can be found in
using_qprof.html.

The following is a quick overview of the usage model.  (Using_qprof.html
suggests some aliases to make this a little simpler but less transparent.)

$ cat dumb_test.c
int main()
{
    long i;
    double sum;

    for (i = 1; i < 4000000; ++i) sum += 1.0/(double)i;
    for (; i < 8000000; ++i) sum += 1.0/(double)i;
    for (; i < 10000000; ++i) sum += 1.0/(double)i;
    exit(sum > 100000.0);
}
$ gcc -g -O2 dumb_test.c
$ setenv LD_PRELOAD ~/prelink/qprof.so
$ ./a.out
qprof: /home/hboehm/prelink/a.out: 150 total samples
main:dumb_test.c:6                                                     59
main:dumb_test.c:7                                                     61
main:dumb_test.c:8                                                     30
$ 

This should work with pthread-multithreaded and with dynamically linked
executables.  Qprof.so intercepts pthread_create().

Note that every command you type after this will be profiled, until
you "unsetenv LD_PRELOAD".  (A few commands, such as "vim", may get
confused by the profiling signals and fail.  We try to work around this by
intercepting and ignoring sigaction calls that affect handling of the
profiling signal. Applications that try to handle profiling signals
themselves will never end up seeing any such signals.)

malloc_trace.so:
---------------

Invoked like qprof.so.  Prints a malloc/free trace.  This is really
more of a test of wrap.h than a useful application.

check:
------

Builds and runs a test of the portable atomic operation routines and a
profiling test.  Currently the (brief) output needs to be checked manually,
since it's nondeterministic.

list_atomic.i:
------

Generates a preprocessed C file containing expansions for various
atomic operations exported by atomic_ops.h.  Useful for manually checking
that an atomic_ops.h port produces the intended expansions.


Important headers
-----------------

atomic_ops.h:
-------------

See README.atomic_ops.

handler_safe_data.h:
--------------------

Some primitives to support data structures manipulated from signal handlers.
Relies on atomic_ops.h.  Currently provides a very simple malloc
implementation, and a hash-table-of-counters data structure.  Further
additions are planned.

wrap.h:
-------

Defines an interface (and some hairy macros implementing it)
for intercepting arbitrary library calls.

prof_utils.h:
-------------

The interfaces for the profiling routines.


