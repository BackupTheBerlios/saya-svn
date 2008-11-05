The header file wrap.h define various macros of the form

WRAP<n>[V]([<result_type>], <argument 1 type>, ...,
	   <fn>, <lib>, <pre>, <post>, <alt>)

The macro invocation will define a function named <fn> which "wraps" an
existing function by the same name.  We assume that a function named <fn>
already exists.  The digit <n> is the number of arguments to <fn>.
Currently <n> must be between 0 and 5.  The optional "V" suffix in the
macro name indicates that <fn> returns void, and no result type is specified.

If WRAP<n> is defined as above then every call to <fn>(arg, ...) will
instead perform the logical actions:

	<pre>(arg, ...);
	result = <fn>(arg, ...);
	<post>(result);
	return result;

It is acceptable for <pre> and <post> to actually be defined as a macro
and for <pre> to declare variables used in <post>.

Two other arguments are required in that they are needed by some
implementations:

<lib> is the library in which <fn> is defined.  We assume that we are wrapping
the previously visible definition of <fn>, but it is sometimes easier for the
implementation to obtain access to it if it knows the name of the library
in which it is defined.

<alt>(arg, ...) is invoked instead of <fn> if <fn> is invoked before we
have completed the wrapping operation.  This may happen if standard C library
calls are wrapped, and those calls are needed, for example, to look up
the original function.  The function <alt> should complete correctly without
calling <fn>.  (In many cases it will be acceptable to supply a function
which simply aborts.  For a function like "malloc", something more
sophisticated is necessary.)

Function wrapping is currently accomplished in one of two ways:

WRAP_LD:  If the WRAP_LD macro is defined, the wrapper function will be
called __wrap_<fn>, and it will refer to __real_<fn>.  This is normally
used with the "--wrap <fn>" GNU ld option (or the -Wl,--wrap -Wl,<fn>
gcc option).  This will cause <fn> to be properly wrapped for code that
is linked at the same time as the wrapper function.  The <lib> and
<alt> arguments are ignored in this case.

WRAP_PRELOAD (the default): The wrapper function is called <fn> and it looks
up the original function using dlsym().  This allows the wrapper to be placed
into e.g. a Linux shared library, and preloaded via the LD_PRELOAD
environment variable.  This can be used to wrap symbols in dynamically-
linked executables without relinking.  On Linux systems, it is recommended
that the file including wrap.h be compiled with _GNU_SOURCE defined.

EXAMPLE: See malloc_trace.c in the qprof distribution.

BUGS: This has only been tested to the extent it is used by the rest of
qprof.  That means many lines of code have never even been compiled.
But most of the undiscovered bugs are probably easy to fix.

	
