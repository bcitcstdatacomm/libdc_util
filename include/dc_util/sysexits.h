#ifndef LIBDC_UTIL_SYSEXITS_H
#define LIBDC_UTIL_SYSEXITS_H


#include <signal.h>


/*
 * Copyright 2021-2021 D'Arcy Smith.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


// https://krueger.report/exit-status-codes/
// https://pubs.opengroup.org/onlinepubs/009695299/utilities/trap.html

#define EX_OK EXIT_SUCCES            /* successful termination */
#define EX_UNSPECIFIED	EXIT_FAILURE /* unsuccessful termination */

#define EX_NOEXEC 126               /* command not executable */
#define EX_NOCOMMAND 127            /* command not found */
#define EX_BADCODE 128              /* invalid exit code */
#define EX_SIGABRT 128 + SIGABRT
#define EX_SIGALRM 128 + SIGALRM
#define EX_SIGBUS 128 + SIGBUS
#define EX_SIGCHLD 128 + SIGCHLD
#define EX_SIGCONT 128 + SIGCONT
#define EX_SIGFPE 128 + SIGFPE
#define EX_SIGHUP 128 + SIGHUP
#define EX_SIGILL 128 + SIGILL
#define EX_SIGINT 128 + SIGINT
#define EX_SIGKILL 128 + SIGKILL
#define EX_SIGPIPE 128 + SIGPIPE
#define EX_SIGQUIT 128 + SIGQUIT
#define EX_SIGSEGV 128 + SIGSEGV
#define EX_SIGSTOP 128 + SIGSTOP
#define EX_SIGTERM 128 + SIGTERM
#define EX_SIGTSTP 128 + SIGTSTP
#define EX_SIGTTIN 128 + SIGTTIN
#define EX_SIGTTOU 128 + SIGTTOU
#define EX_SIGUSR1 128 + SIGUSR1
#define EX_SIGUSR2 128 + SIGUSR2
#define EX_SIGSYS 128 + SIGSYS
#define EX_SIGTRAP 128 + SIGTRAP
#define EX_SIGURG 128 + SIGURG
#define EX_SIGVTALRM 128 + SIGVTALRM
#define EX_SIGXCPU 128 + SIGXCPU
#define EX_SIGXFSZ 128 + SIGXFSZ

#define EX_E2BIG 3
#define EX_EACCES 4
#define EX_EADDRINUSE 5
#define EX_EADDRNOTAVAIL 6
#define EX_EAFNOSUPPORT 7
#define EX_EAGAIN 8
#define EX_EALREADY 9
#define EX_EBADF 10
#define EX_EBADMSG 11
#define EX_EBUSY 12
#define EX_ECANCELED 13
#define EX_ECHILD 14
#define EX_ECONNABORTED 15
#define EX_ECONNREFUSED 16
#define EX_ECONNRESET 17
#define EX_EDEADLK 18
#define EX_EDESTADDRREQ 19
#define EX_EDOM 20
#define EX_EDQUOT 21
#define EX_EEXIST 22
#define EX_EFAULT 23
#define EX_EFBIG 24
#define EX_EHOSTUNREACH 25
#define EX_EIDRM 26
#define EX_EILSEQ 27
#define EX_EINPROGRESS 28
#define EX_EINTR 29
#define EX_EINVAL 30
#define EX_EIO 31
#define EX_EISCONN 32
#define EX_EISDIR 33
#define EX_ELOOP 34
#define EX_EMFILE 35
#define EX_EMLINK 36
#define EX_EMSGSIZE 37
#define EX_EMULTIHOP 38
#define EX_ENAMETOOLONG 39
#define EX_ENETDOWN 40
#define EX_ENETRESET 41
#define EX_ENETUNREACH 42
#define EX_ENFILE 43
#define EX_ENOBUFS 44
#define EX_ENODEV 45
#define EX_ENOENT 46
#define EX_ENOEXEC 47
#define EX_ENOLCK 48
#define EX_ENOLINK 49
#define EX_ENOMEM 50
#define EX_ENOMSG 51
#define EX_ENOPROTOOPT 52
#define EX_ENOSPC 53
#define EX_ENOSYS 54
#define EX_ENOTCONN 55
#define EX_ENOTDIR 56
#define EX_ENOTEMPTY 57
#define EX_ENOTRECOVERABLE 58
#define EX_ENOTSOCK 59
#define EX_ENOTSUP 60
#define EX_ENOTTY 61
#define EX_ENXIO 62
#define EX_EOPNOTSUPP 63
#define EX_EOVERFLOW 79
#define EX_EOWNERDEAD 80
#define EX_EPERM 81
#define EX_EPIPE 82
#define EX_EPROTO 83
#define EX_EPROTONOSUPPORT 84
#define EX_EPROTOTYPE 85
#define EX_ERANGE 86
#define EX_EROFS 87
#define EX_ESPIPE 88
#define EX_ESRCH 89
#define EX_ESTALE 90
#define EX_ETIMEDOUT 91
#define EX_ETXTBSY 92
#define EX_EWOULDBLOCK 93
#define EX_EXDEV 94


#endif // LIBDC_UTIL_SYSEXITS_H
