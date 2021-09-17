#ifndef LIBDC_UTIL_EXIT_CODES_H
#define LIBDC_UTIL_EXIT_CODES_H


#include <signal.h>


// __has_include is NOT portable, but should work on gcc and clang
#if __has_include(<sysexits.h>)
    #include <sysexits.h>
#else
    #include <stdlib.h>
    #define EX_OK		    EXIT_SUCCESS /* successful termination */
    #define EX_UNSPECIFIED	EXIT_FAILURE	/* successful termination */
    #define EX__BASE	    64
    #define EX_USAGE	    64
    #define EX_DATAERR	    65
    #define EX_NOINPUT	    66
    #define EX_NOUSER	    67
    #define EX_NOHOST	    68
    #define EX_UNAVAILABLE	69
    #define EX_SOFTWARE	    70
    #define EX_OSERR	    71
    #define EX_OSFILE	    72
    #define EX_CANTCREAT    73
    #define EX_IOERR	    74
    #define EX_TEMPFAIL     75
    #define EX_PROTOCOL	    76
    #define EX_NOPERM	    77
    #define EX_CONFIG	    78
    #define EX__MAX 	    78
#endif


// https://krueger.report/programming/exit-status-codes/
// https://pubs.opengroup.org/onlinepubs/009695299/utilities/trap.html
#define EX_NOEXEC 126
#define EX_NOCOMMAND 127
#define EX_BADCODE 128
#define EX_SIGHUP 128 + SIGHUP
#define EX_SIGINT 128 + SIGINT
#define EX_SIGQUIT 128 + SIGQUIT
#define EX_SIGABRT 128 + SIGABRT
#define EX_SIGKILL 128 + SIGKILL
#define EX_SIGALRM 128 + SIGALRM
#define EX_SIGTERM 128 + SIGTERM


#endif // LIBDC_UTIL_EXIT_CODES_H
