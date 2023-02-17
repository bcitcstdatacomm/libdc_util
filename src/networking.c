/*
 * Copyright 2022-2022 D'Arcy Smith.
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


#include "dc_util/networking.h"
#include <dc_c/dc_stdlib.h>
#include <dc_c/dc_string.h>
#include <dc_posix/arpa/dc_inet.h>
#include <dc_posix/dc_netdb.h>
#include <dc_posix/dc_string.h>
#include <dc_posix/sys/dc_socket.h>
#include <dc_unix/dc_ifaddrs.h>
#include <net/if.h>


static int getsockopt_int(const struct dc_env *env, struct dc_error *err, int socket_fd, int level, int option);
static struct linger getsockopt_linger(const struct dc_env *env, struct dc_error *err, int socket_fd, int level, int option);
static struct timeval getsockopt_timeval(const struct dc_env *env, struct dc_error *err, int socket_fd, int level, int option);
static int getsockopt_socket_int(const struct dc_env *env, struct dc_error *err, int socket_fd, int option);
static bool getsockopt_socket_bool(const struct dc_env *env, struct dc_error *err, int socket_fd, bool option);
static struct linger getsockopt_socket_linger(const struct dc_env *env, struct dc_error *err, int socket_fd, int option);
static struct timeval getsockopt_socket_timeval(const struct dc_env *env, struct dc_error *err, int socket_fd, int option);
static void setsockopt_int(const struct dc_env *env, struct dc_error *err, int socket_fd, int level, int option, int value);
static void setsockopt_linger(const struct dc_env *env, struct dc_error *err, int socket_fd, int level, int option, int on, int seconds);
static void setsockopt_timeval(const struct dc_env *env, struct dc_error *err, int socket_fd, int level, int option, time_t seconds, long useconds);
static void setsockopt_socket_int(const struct dc_env *env, struct dc_error *err, int socket_fd, int option, int value);
static void setsockopt_socket_linger(const struct dc_env *env, struct dc_error *err, int socket_fd, int option, int on, int seconds);
static void setsockopt_socket_timeval(const struct dc_env *env, struct dc_error *err, int socket_fd, int option, time_t seconds, long useconds);


/*
struct sockaddr *foo(const struct dc_env *env, struct dc_error *err, sa_family_t family)
{
    struct ifaddrs *ifap;

    dc_getifaddrs(env, err, &ifap);

    for(struct ifaddrs *tmp = ifap; tmp != NULL; tmp = tmp->ifa_next)
    {
        struct sockaddr *addr;
        char addressOutputBuffer[INET6_ADDRSTRLEN];

        if(tmp->ifa_addr->sa_family == AF_INET)
        {
            addr = (struct sockaddr *)&((struct sockaddr_in *)tmp->ifa_addr)->sin_addr;
        }
        else if(tmp->ifa_addr->sa_family == AF_INET6)
        {
            addr = (struct sockaddr *)&((struct sockaddr_in6 *)tmp->ifa_addr)->sin6_addr;
        }
        else
        {
            addr = NULL;
        }

        if(addr)
        {
            dc_inet_ntop(env, err, tmp->ifa_addr->sa_family,
                         addr,
                         addressOutputBuffer,
                         sizeof(addressOutputBuffer));
            printf("%s\n", tmp->ifa_name);
            printf("%s\n", addressOutputBuffer);
        }
    }

    dc_freeifaddrs(env, ifap);
}
*/

char *dc_inet_ntop_compat(const struct dc_env *env, struct dc_error *err, const struct sockaddr_storage *sockaddr)
{
    const void *addr;
    socklen_t addr_str_len;
    char *addr_str;

    DC_TRACE(env);

    if(sockaddr->ss_family == AF_INET)
    {
        addr = &((const struct sockaddr_in *)sockaddr)->sin_addr;
        addr_str_len = INET_ADDRSTRLEN;
    }
    else if(sockaddr->ss_family == AF_INET6)
    {
        addr = &((const struct sockaddr_in6 *)sockaddr)->sin6_addr;
        addr_str_len = INET6_ADDRSTRLEN;
    }
    else
    {
        DC_ERROR_RAISE_USER(err, "", 1);
        addr_str_len = 0;
        addr = NULL;
    }

    addr_str = NULL;

    if(dc_error_has_no_error(err))
    {
        addr_str = dc_malloc(env, err, addr_str_len + 1);
        dc_inet_ntop(env, err, sockaddr->ss_family, addr, addr_str, addr_str_len);
    }

    return addr_str;
}

in_port_t dc_inet_get_port(const struct dc_env *env, struct dc_error *err, const struct sockaddr_storage *sockaddr)
{
    in_port_t port;

    DC_TRACE(env);

    if(sockaddr->ss_family == AF_INET)
    {
        port = ((const struct sockaddr_in *)sockaddr)->sin_port;
    }
    else if(sockaddr->ss_family == AF_INET6)
    {
        port = ((const struct sockaddr_in6 *)sockaddr)->sin6_port;
    }
    else
    {
        DC_ERROR_RAISE_USER(err, "", 1);
        port = 0;
    }

    return port;
}

void dc_print_sockopts(const struct dc_env *env, struct dc_error *err, int socket_fd, FILE *stream)
{
    bool acceptconn;
    bool broadcast;
    bool debug;
    bool dontdoute;
    bool keepalive;
//    struct linger linger;
    bool oobinline;
    int rcvbuf;
    int dcvlowat;
    struct timeval rvtimeo;
    bool reuseaddr;
    int sndbuf;
    int sndlowat;
    struct timeval sndtimeo;
//    int maxconn;
    int type;

    acceptconn = dc_getsockopt_socket_ACCEPTCONN(env, err, socket_fd);
    broadcast  = dc_getsockopt_socket_BROADCAST(env, err, socket_fd);
    debug      = dc_getsockopt_socket_DEBUG(env, err, socket_fd);
    dontdoute  = dc_getsockopt_socket_DONTROUTE(env, err, socket_fd);
    keepalive  = dc_getsockopt_socket_KEEPALIVE(env, err, socket_fd);
//    linger     = dc_getsockopt_socket_LINGER(env, err, socket_fd);
    oobinline  = dc_getsockopt_socket_OOBINLINE(env, err, socket_fd);
    rcvbuf     = dc_getsockopt_socket_RCVBUF(env, err, socket_fd);
    dcvlowat   = dc_getsockopt_socket_RCVLOWAT(env, err, socket_fd);
    rvtimeo    = dc_getsockopt_socket_RCVTIMEO(env, err, socket_fd);
    reuseaddr  = dc_getsockopt_socket_REUSEADDR(env, err, socket_fd);
    sndbuf     = dc_getsockopt_socket_SNDBUF(env, err, socket_fd);
    sndlowat   = dc_getsockopt_socket_SNDLOWAT(env, err, socket_fd);
    sndtimeo   = dc_getsockopt_socket_SNDTIMEO(env, err, socket_fd);
//    maxconn    = dc_getsockopt_socket_MAXCONN(env, err, socket_fd);
    type       = dc_getsockopt_socket_TYPE(env, err, socket_fd);

    // NOLINTBEGIN(cert-err33-c)
    fprintf(stream, "socket (%d):\n", socket_fd);
    fprintf(stream, "\tTYPE:         %d\n", type);
    fprintf(stream, "\tACCEPTCONN:   %d\n", acceptconn);
    fprintf(stream, "\tBROADCAST:    %d\n", broadcast);
    fprintf(stream, "\tDEBUG:        %d\n", debug);
    fprintf(stream, "\tDONTROUTE:    %d\n", dontdoute);
    fprintf(stream, "\tKEEPALIVE:    %d\n", keepalive);
//    fprintf(stream, "\tLINGER:\n");
//    fprintf(stream, "\t\ton:       %d\n", linger.l_linger);
//    fprintf(stream, "\t\tseconds:  %d\n", linger.l_linger);
    fprintf(stream, "\tOOBINLINE:    %d\n", oobinline);
    fprintf(stream, "\tRCVBUF:       %d\n", rcvbuf);
    fprintf(stream, "\tRCVLOWAT :    %d\n", dcvlowat);
    fprintf(stream, "\tRCVTIMEO:\n");
    fprintf(stream, "\t\tseconds:  %ld\n", rvtimeo.tv_sec);
    fprintf(stream, "\t\tuseconds: %ld\n", rvtimeo.tv_usec);
    fprintf(stream, "\tREUSEADDR:    %d\n", reuseaddr);
    fprintf(stream, "\tSNDBUF:       %d\n", sndbuf);
    fprintf(stream, "\tSNDLOWAT:     %d\n", sndlowat);
    fprintf(stream, "\tSNDTIMEO:\n");
    fprintf(stream, "\t\tseconds : %ld\n", sndtimeo.tv_sec);
    fprintf(stream, "\t\tuseconds: %ld\n", sndtimeo.tv_usec);
//    fprintf(stream, "\tMAXCONN:      %d\n", maxconn);
    // NOLINTEND(cert-err33-c)
}

static inline int getsockopt_int(const struct dc_env *env, struct dc_error *err, int socket_fd, int level, int option)
{
    int value;
    socklen_t len;

    DC_TRACE(env);
    dc_getsockopt(env, err, socket_fd, level, option, &value, &len);

    return value;
}

static inline struct linger getsockopt_linger(const struct dc_env *env, struct dc_error *err, int socket_fd, int level, int option)
{
    struct linger value;
    socklen_t len;

    DC_TRACE(env);
    dc_getsockopt(env, err, socket_fd, level, option, &value, &len);

    return value;
}

static inline struct timeval getsockopt_timeval(const struct dc_env *env, struct dc_error *err, int socket_fd, int level, int option)
{
    struct timeval value;
    socklen_t len;

    DC_TRACE(env);
    dc_getsockopt(env, err, socket_fd, level, option, &value, &len);

    return value;
}

static inline int getsockopt_socket_int(const struct dc_env *env, struct dc_error *err, int socket_fd, int option)
{
    int value;

    DC_TRACE(env);
    value = getsockopt_int(env, err, socket_fd, SOL_SOCKET, option);

    return value;
}

static inline bool getsockopt_socket_bool(const struct dc_env *env, struct dc_error *err, int socket_fd, bool option)
{
    int value;

    DC_TRACE(env);
    value = getsockopt_socket_int(env, err, socket_fd, option);

    return value;
}

static inline struct linger getsockopt_socket_linger(const struct dc_env *env, struct dc_error *err, int socket_fd, int option)
{
    struct linger value;

    DC_TRACE(env);
    value = getsockopt_linger(env, err, socket_fd, SOL_SOCKET, option);

    return value;
}

static inline struct timeval getsockopt_socket_timeval(const struct dc_env *env, struct dc_error *err, int socket_fd, int option)
{
    struct timeval value;

    DC_TRACE(env);
    value = getsockopt_timeval(env, err, socket_fd, SOL_SOCKET, option);

    return value;
}

bool dc_getsockopt_socket_ACCEPTCONN(const struct dc_env *env, struct dc_error *err, int socket_fd)
{
    bool value;

    DC_TRACE(env);
    value = getsockopt_socket_bool(env, err, socket_fd, SO_ACCEPTCONN);

    return value;
}

bool dc_getsockopt_socket_BROADCAST(const struct dc_env *env, struct dc_error *err, int socket_fd)
{
    bool value;

    DC_TRACE(env);
    value = getsockopt_socket_bool(env, err, socket_fd, SO_BROADCAST);

    return value;
}

bool dc_getsockopt_socket_DEBUG(const struct dc_env *env, struct dc_error *err, int socket_fd)
{
    bool value;

    DC_TRACE(env);
    value = getsockopt_socket_bool(env, err, socket_fd, SO_DEBUG);

    return value;
}

bool dc_getsockopt_socket_DONTROUTE(const struct dc_env *env, struct dc_error *err, int socket_fd)
{
    bool value;

    DC_TRACE(env);
    value = getsockopt_socket_bool(env, err, socket_fd, SO_DONTROUTE);

    return value;
}

int dc_getsockopt_socket_ERROR(const struct dc_env *env, struct dc_error *err, int socket_fd)
{
    int value;

    DC_TRACE(env);
    value = getsockopt_socket_int(env, err, socket_fd, SO_ERROR);

    return value;
}

bool dc_getsockopt_socket_KEEPALIVE(const struct dc_env *env, struct dc_error *err, int socket_fd)
{
    bool value;

    DC_TRACE(env);
    value = getsockopt_socket_bool(env, err, socket_fd, SO_KEEPALIVE);

    return value;
}

struct linger dc_getsockopt_socket_LINGER(const struct dc_env *env, struct dc_error *err, int socket_fd)
{
    struct linger value;

    DC_TRACE(env);
    value = getsockopt_socket_linger(env, err, socket_fd, SO_LINGER);

    return value;
}

bool dc_getsockopt_socket_OOBINLINE(const struct dc_env *env, struct dc_error *err, int socket_fd)
{
    bool value;

    DC_TRACE(env);
    value = getsockopt_socket_bool(env, err, socket_fd, SO_OOBINLINE);

    return value;
}

int dc_getsockopt_socket_RCVBUF(const struct dc_env *env, struct dc_error *err, int socket_fd)
{
    int value;

    DC_TRACE(env);
    value = getsockopt_socket_int(env, err, socket_fd, SO_RCVBUF);

    return value;
}

int dc_getsockopt_socket_RCVLOWAT(const struct dc_env *env, struct dc_error *err, int socket_fd)
{
    int value;

    DC_TRACE(env);
    value = getsockopt_socket_int(env, err, socket_fd, SO_RCVLOWAT);

    return value;
}

struct timeval dc_getsockopt_socket_RCVTIMEO(const struct dc_env *env, struct dc_error *err, int socket_fd)
{
    struct timeval value;

    DC_TRACE(env);
    value = getsockopt_socket_timeval(env, err, socket_fd, SO_RCVTIMEO);

    return value;
}

bool dc_getsockopt_socket_REUSEADDR(const struct dc_env *env, struct dc_error *err, int socket_fd)
{
    bool value;

    DC_TRACE(env);
    value = getsockopt_socket_bool(env, err, socket_fd, SO_REUSEADDR);

    return value;
}

int dc_getsockopt_socket_SNDBUF(const struct dc_env *env, struct dc_error *err, int socket_fd)
{
    int value;

    DC_TRACE(env);
    value = getsockopt_socket_int(env, err, socket_fd, SO_SNDBUF);

    return value;
}

int dc_getsockopt_socket_SNDLOWAT(const struct dc_env *env, struct dc_error *err, int socket_fd)
{
    int value;

    DC_TRACE(env);
    value = getsockopt_socket_int(env, err, socket_fd, SO_SNDLOWAT);

    return value;
}

struct timeval dc_getsockopt_socket_SNDTIMEO(const struct dc_env *env, struct dc_error *err, int socket_fd)
{
    struct timeval value;

    DC_TRACE(env);
    value = getsockopt_socket_timeval(env, err, socket_fd, SO_SNDTIMEO);

    return value;
}

int dc_getsockopt_socket_TYPE(const struct dc_env *env, struct dc_error *err, int socket_fd)
{
    int value;

    DC_TRACE(env);
    value = getsockopt_socket_int(env, err, socket_fd, SO_TYPE);

    return value;
}

int dc_getsockopt_socket_MAXCONN(const struct dc_env *env, struct dc_error *err, int socket_fd)
{
    int value;

    DC_TRACE(env);
    value = getsockopt_socket_int(env, err, socket_fd, SOMAXCONN);

    return value;
}

static inline void setsockopt_int(const struct dc_env *env, struct dc_error *err, int socket_fd, int level, int option, int value)
{
    DC_TRACE(env);
    dc_setsockopt(env, err, socket_fd, level, option, &value, sizeof(value));
}

static inline void setsockopt_linger(const struct dc_env *env, struct dc_error *err, int socket_fd, int level, int option, int on, int seconds)
{
    struct linger value;

    DC_TRACE(env);
    value.l_onoff = on;
    value.l_linger = seconds;
    dc_setsockopt(env, err, socket_fd, level, option, &value, sizeof(value));
}

static inline void setsockopt_timeval(const struct dc_env *env, struct dc_error *err, int socket_fd, int level, int option, time_t seconds, long useconds)
{
    struct timeval value;

    DC_TRACE(env);
    value.tv_sec = seconds;
    value.tv_usec = useconds;
    dc_setsockopt(env, err, socket_fd, level, option, &value, sizeof(value));
}

static inline void setsockopt_socket_int(const struct dc_env *env, struct dc_error *err, int socket_fd, int option, int value)
{
    DC_TRACE(env);
    setsockopt_int(env, err, socket_fd, SOL_SOCKET, option, value);
}

static inline void setsockopt_socket_linger(const struct dc_env *env, struct dc_error *err, int socket_fd, int option, int on, int seconds)
{
    DC_TRACE(env);
    setsockopt_linger(env, err, socket_fd, SOL_SOCKET, option, on, seconds);
}

static inline void setsockopt_socket_timeval(const struct dc_env *env, struct dc_error *err, int socket_fd, int option, time_t seconds, long useconds)
{
    DC_TRACE(env);
    setsockopt_timeval(env, err, socket_fd, SOL_SOCKET, option, seconds, useconds);
}

void dc_setsockopt_socket_BROADCAST(const struct dc_env *env, struct dc_error *err, int socket_fd, bool value)
{
    DC_TRACE(env);
    setsockopt_socket_int(env, err, socket_fd, SO_BROADCAST, value);
}

void dc_setsockopt_socket_DEBUG(const struct dc_env *env, struct dc_error *err, int socket_fd, bool value)
{
    DC_TRACE(env);
    setsockopt_socket_int(env, err, socket_fd, SO_DEBUG, value);
}

void dc_setsockopt_socket_DONTROUTE(const struct dc_env *env, struct dc_error *err, int socket_fd, bool value)
{
    DC_TRACE(env);
    setsockopt_socket_int(env, err, socket_fd, SO_DONTROUTE, value);
}

void dc_setsockopt_socket_KEEPALIVE(const struct dc_env *env, struct dc_error *err, int socket_fd, bool value)
{
    DC_TRACE(env);
    setsockopt_socket_int(env, err, socket_fd, SO_KEEPALIVE, value);
}

void dc_setsockopt_socket_LINGER(const struct dc_env *env, struct dc_error *err, int socket_fd, bool on, int seconds)
{
    DC_TRACE(env);
    setsockopt_socket_linger(env, err, socket_fd, SO_LINGER, on, seconds);
}

void dc_setsockopt_socket_OOBINLINE(const struct dc_env *env, struct dc_error *err, int socket_fd, bool value)
{
    DC_TRACE(env);
    setsockopt_socket_int(env, err, socket_fd, SO_OOBINLINE, value);
}

void dc_setsockopt_socket_RCVBUF(const struct dc_env *env, struct dc_error *err, int socket_fd, int value)
{
    DC_TRACE(env);
    setsockopt_socket_int(env, err, socket_fd, SO_RCVBUF, value);
}

void dc_setsockopt_socket_RCVLOWAT(const struct dc_env *env, struct dc_error *err, int socket_fd, int value)
{
    DC_TRACE(env);
    setsockopt_socket_int(env, err, socket_fd, SO_RCVLOWAT, value);
}

void dc_setsockopt_socket_RCVTIMEO(const struct dc_env *env, struct dc_error *err, int socket_fd, time_t seconds, long useconds)
{
    DC_TRACE(env);
    setsockopt_socket_timeval(env, err, socket_fd, SO_RCVTIMEO, seconds, useconds);
}

void dc_setsockopt_socket_REUSEADDR(const struct dc_env *env, struct dc_error *err, int socket_fd, bool value)
{
    DC_TRACE(env);
    setsockopt_socket_int(env, err, socket_fd, SO_REUSEADDR, value);
}

void dc_setsockopt_socket_SNDBUF(const struct dc_env *env, struct dc_error *err, int socket_fd, int value)
{
    DC_TRACE(env);
    setsockopt_socket_int(env, err, socket_fd, SO_SNDBUF, value);
}

void dc_setsockopt_socket_SNDLOWAT(const struct dc_env *env, struct dc_error *err, int socket_fd, int value)
{
    DC_TRACE(env);
    setsockopt_socket_int(env, err, socket_fd, SO_SNDLOWAT, value);
}

void dc_setsockopt_socket_SNDTIMEO(const struct dc_env *env, struct dc_error *err, int socket_fd, time_t seconds, long useconds)
{
    DC_TRACE(env);
    setsockopt_socket_timeval(env, err, socket_fd, SO_SNDTIMEO, seconds, useconds);
}

void dc_setsockopt_socket_MAXCONN(const struct dc_env *env, struct dc_error *err, int socket_fd, int value)
{
    DC_TRACE(env);
    setsockopt_socket_int(env, err, socket_fd, SOMAXCONN, value);
}

char *dc_get_ip_addresses_by_interface(const struct dc_env *env, struct dc_error *err, const char *interface_name, int family)
{
    struct ifaddrs *ifaddr;
    struct ifaddrs *ifa;
    char host[NI_MAXHOST];
    char *address;

    DC_TRACE(env);
    address = NULL;
    dc_getifaddrs(env, err, &ifaddr);

    if(dc_error_has_no_error(err))
    {
        for(ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next)
        {
            if(ifa->ifa_addr == NULL)
            {
                continue;
            }

            if(dc_strcmp(env, ifa->ifa_name, interface_name) == 0 && ifa->ifa_addr->sa_family == family)
            {
                dc_getnameinfo(env, err, ifa->ifa_addr, sizeof(struct sockaddr_in), host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);

                if(dc_error_has_no_error(err))
                {
                    address = dc_strdup(env, err, host);
                    break;
                }
            }
        }

        dc_freeifaddrs(env, ifaddr);
    }

    return address;
}

char *dc_get_default_interface(const struct dc_env *env, struct dc_error *err, int family)
{
    struct ifaddrs *ifaddr;
    struct ifaddrs *ifa;
    char *name;

    DC_TRACE(env);
    name = NULL;
    dc_getifaddrs(env, err, &ifaddr);

    if(dc_error_has_no_error(err))
    {
        for(ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next)
        {
            if(ifa->ifa_addr == NULL)
            {
                continue;
            }

            if((ifa->ifa_flags & (unsigned int)IFF_UP) &&
               (ifa->ifa_flags & (unsigned int)IFF_RUNNING) &&
               (!(ifa->ifa_flags & (unsigned int)IFF_LOOPBACK)) &&
               (ifa->ifa_addr->sa_family == family))
            {
                bool found;

                found = false;

                if(ifa->ifa_addr->sa_family == AF_INET)
                {
                    found = true;
                }
                else if(ifa->ifa_addr->sa_family == AF_INET6)
                {
                    found = true;
                }

                if(found)
                {
                    name = dc_strdup(env, err, ifa->ifa_name);
                    break;
                }
            }
        }

        dc_freeifaddrs(env, ifaddr);
    }

    return name;
}

ssize_t dc_sendmsg_fully(const struct dc_env *env, struct dc_error *err, int sockfd, const struct msghdr *msg, int flags)
{
    char   *buf;
    size_t  len;
    ssize_t total_sent;

    buf        = msg->msg_iov->iov_base;
    len        = msg->msg_iov->iov_len;
    total_sent = 0;

    while(total_sent < msg->msg_iovlen)
    {
        ssize_t sent;

        sent = dc_sendmsg(env, err, sockfd, msg, flags);

        if(dc_error_has_error(err))
        {
            if(dc_error_is_errno(err, EAGAIN) || dc_error_is_errno(err, EWOULDBLOCK))
            {
                continue;
            }
            else
            {
                break;
            }
        }
        else
        {
            total_sent += sent;
            buf += sent;
            len -= sent;
            msg->msg_iov->iov_base = buf;
            msg->msg_iov->iov_len = len;
        }
    }

    return total_sent;
}

ssize_t dc_recvmsg_fully(const struct dc_env *env, struct dc_error *err, int sockfd, struct msghdr *msg, int flags)
{
    ssize_t recv;
    size_t  total_received;
    char   *buf;
    size_t  len;

    total_received = 0;
    buf            = msg->msg_iov->iov_base;
    len            = msg->msg_iov->iov_len;

    while(total_received < len)
    {
        recv = dc_recvmsg(env, err, sockfd, msg, flags);

        if(dc_error_has_error(err))
        {
            if(dc_error_is_errno(err, EAGAIN) || dc_error_is_errno(err, EWOULDBLOCK))
            {
                break;
            }
        }
        else if (recv == 0)
        {
            DC_ERROR_RAISE_SYSTEM(err, "", 1);
            break;
        }
        else
        {
            total_received += (size_t)recv;
            buf += recv;
            len -= recv;
            msg->msg_iov->iov_base = buf;
            msg->msg_iov->iov_len = len;
        }
    }

    return (ssize_t)total_received;
}
