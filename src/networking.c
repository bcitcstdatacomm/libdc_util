#include "networking.h"
#include <dc_posix/dc_stdlib.h>
#include <dc_posix/sys/dc_socket.h>
#include <netinet/in.h>


static int getsockopt_int(const struct dc_posix_env *env, struct dc_error *err, int socket_fd, int level, int option);
static struct linger getsockopt_linger(const struct dc_posix_env *env, struct dc_error *err, int socket_fd, int level, int option);
static struct timeval getsockopt_timeval(const struct dc_posix_env *env, struct dc_error *err, int socket_fd, int level, int option);
static int getsockopt_socket_int(const struct dc_posix_env *env, struct dc_error *err, int socket_fd, int option);
static bool getsockopt_socket_bool(const struct dc_posix_env *env, struct dc_error *err, int socket_fd, bool option);
static struct linger getsockopt_socket_linger(const struct dc_posix_env *env, struct dc_error *err, int socket_fd, int option);
static struct timeval getsockopt_socket_timeval(const struct dc_posix_env *env, struct dc_error *err, int socket_fd, int option);
static void setsockopt_int(const struct dc_posix_env *env, struct dc_error *err, int socket_fd, int level, int option, int value);
static void setsockopt_linger(const struct dc_posix_env *env, struct dc_error *err, int socket_fd, int level, int option, int on, int seconds);
static void setsockopt_timeval(const struct dc_posix_env *env, struct dc_error *err, int socket_fd, int level, int option, time_t seconds, long useconds);
static void setsockopt_socket_int(const struct dc_posix_env *env, struct dc_error *err, int socket_fd, int option, int value);
static void setsockopt_socket_linger(const struct dc_posix_env *env, struct dc_error *err, int socket_fd, int option, int on, int seconds);
static void setsockopt_socket_timeval(const struct dc_posix_env *env, struct dc_error *err, int socket_fd, int option, time_t seconds, long useconds);


char *dc_inet_ntop_compat(const struct dc_posix_env *env, struct dc_error *err, const struct sockaddr_storage *sockaddr)
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

in_port_t dc_inet_get_port(const struct dc_posix_env *env, struct dc_error *err, const struct sockaddr_storage *sockaddr)
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

static inline int getsockopt_int(const struct dc_posix_env *env, struct dc_error *err, int socket_fd, int level, int option)
{
    int value;
    socklen_t len;

    DC_TRACE(env);
    dc_getsockopt(env, err, socket_fd, level, option, &value, &len);

    return value;
}

static inline struct linger getsockopt_linger(const struct dc_posix_env *env, struct dc_error *err, int socket_fd, int level, int option)
{
    struct linger value;
    socklen_t len;

    DC_TRACE(env);
    dc_getsockopt(env, err, socket_fd, level, option, &value, &len);

    return value;
}

static inline struct timeval getsockopt_timeval(const struct dc_posix_env *env, struct dc_error *err, int socket_fd, int level, int option)
{
    struct timeval value;
    socklen_t len;

    DC_TRACE(env);
    dc_getsockopt(env, err, socket_fd, level, option, &value, &len);

    return value;
}

static inline int getsockopt_socket_int(const struct dc_posix_env *env, struct dc_error *err, int socket_fd, int option)
{
    int value;

    DC_TRACE(env);
    value = getsockopt_int(env, err, socket_fd, SOL_SOCKET, option);

    return value;
}

static inline bool getsockopt_socket_bool(const struct dc_posix_env *env, struct dc_error *err, int socket_fd, bool option)
{
    int value;

    DC_TRACE(env);
    value = getsockopt_socket_int(env, err, socket_fd, option);

    return value;
}

static inline struct linger getsockopt_socket_linger(const struct dc_posix_env *env, struct dc_error *err, int socket_fd, int option)
{
    struct linger value;

    DC_TRACE(env);
    value = getsockopt_linger(env, err, socket_fd, SOL_SOCKET, option);

    return value;
}

static inline struct timeval getsockopt_socket_timeval(const struct dc_posix_env *env, struct dc_error *err, int socket_fd, int option)
{
    struct timeval value;

    DC_TRACE(env);
    value = getsockopt_timeval(env, err, socket_fd, SOL_SOCKET, option);

    return value;
}

bool dc_getsockopt_socket_ACCEPTCONN(const struct dc_posix_env *env, struct dc_error *err, int socket_fd)
{
    bool value;

    DC_TRACE(env);
    value = getsockopt_socket_bool(env, err, socket_fd, SO_ACCEPTCONN);

    return value;
}

bool dc_getsockopt_socket_BROADCAST(const struct dc_posix_env *env, struct dc_error *err, int socket_fd)
{
    bool value;

    DC_TRACE(env);
    value = getsockopt_socket_bool(env, err, socket_fd, SO_BROADCAST);

    return value;
}

bool dc_getsockopt_socket_DEBUG(const struct dc_posix_env *env, struct dc_error *err, int socket_fd)
{
    bool value;

    DC_TRACE(env);
    value = getsockopt_socket_bool(env, err, socket_fd, SO_DEBUG);

    return value;
}

bool dc_getsockopt_socket_DONTROUTE(const struct dc_posix_env *env, struct dc_error *err, int socket_fd)
{
    bool value;

    DC_TRACE(env);
    value = getsockopt_socket_bool(env, err, socket_fd, SO_DONTROUTE);

    return value;
}

int dc_getsockopt_socket_ERROR(const struct dc_posix_env *env, struct dc_error *err, int socket_fd)
{
    int value;

    DC_TRACE(env);
    value = getsockopt_socket_int(env, err, socket_fd, SO_ERROR);

    return value;
}

bool dc_getsockopt_socket_KEEPALIVE(const struct dc_posix_env *env, struct dc_error *err, int socket_fd)
{
    bool value;

    DC_TRACE(env);
    value = getsockopt_socket_bool(env, err, socket_fd, SO_KEEPALIVE);

    return value;
}

struct linger dc_getsockopt_socket_LINGER(const struct dc_posix_env *env, struct dc_error *err, int socket_fd)
{
    struct linger value;

    DC_TRACE(env);
    value = getsockopt_socket_linger(env, err, socket_fd, SO_LINGER);

    return value;
}

bool dc_getsockopt_socket_OOBINLINE(const struct dc_posix_env *env, struct dc_error *err, int socket_fd)
{
    bool value;

    DC_TRACE(env);
    value = getsockopt_socket_bool(env, err, socket_fd, SO_OOBINLINE);

    return value;
}

int dc_getsockopt_socket_RCVBUF(const struct dc_posix_env *env, struct dc_error *err, int socket_fd)
{
    int value;

    DC_TRACE(env);
    value = getsockopt_socket_int(env, err, socket_fd, SO_RCVBUF);

    return value;
}

int dc_getsockopt_socket_RCVLOWAT(const struct dc_posix_env *env, struct dc_error *err, int socket_fd)
{
    int value;

    DC_TRACE(env);
    value = getsockopt_socket_int(env, err, socket_fd, SO_RCVLOWAT);

    return value;
}

struct timeval dc_getsockopt_socket_RCVTIMEO(const struct dc_posix_env *env, struct dc_error *err, int socket_fd)
{
    struct timeval value;

    DC_TRACE(env);
    value = getsockopt_socket_timeval(env, err, socket_fd, SO_RCVTIMEO);

    return value;
}

bool dc_getsockopt_socket_REUSEADDR(const struct dc_posix_env *env, struct dc_error *err, int socket_fd)
{
    bool value;

    DC_TRACE(env);
    value = getsockopt_socket_bool(env, err, socket_fd, SO_REUSEADDR);

    return value;
}

int dc_getsockopt_socket_SNDBUF(const struct dc_posix_env *env, struct dc_error *err, int socket_fd)
{
    int value;

    DC_TRACE(env);
    value = getsockopt_socket_int(env, err, socket_fd, SO_SNDBUF);

    return value;
}

int dc_getsockopt_socket_SNDLOWAT(const struct dc_posix_env *env, struct dc_error *err, int socket_fd)
{
    int value;

    DC_TRACE(env);
    value = getsockopt_socket_int(env, err, socket_fd, SO_SNDLOWAT);

    return value;
}

struct timeval dc_getsockopt_socket_SNDTIMEO(const struct dc_posix_env *env, struct dc_error *err, int socket_fd)
{
    struct timeval value;

    DC_TRACE(env);
    value = getsockopt_socket_timeval(env, err, socket_fd, SO_SNDTIMEO);

    return value;
}

int dc_getsockopt_socket_TYPE(const struct dc_posix_env *env, struct dc_error *err, int socket_fd)
{
    int value;

    DC_TRACE(env);
    value = getsockopt_socket_int(env, err, socket_fd, SO_TYPE);

    return value;
}

int dc_getsockopt_socket_SOMAXCONN(const struct dc_posix_env *env, struct dc_error *err, int socket_fd)
{
    int value;

    DC_TRACE(env);
    value = getsockopt_socket_int(env, err, socket_fd, SOMAXCONN);

    return value;
}

static inline void setsockopt_int(const struct dc_posix_env *env, struct dc_error *err, int socket_fd, int level, int option, int value)
{
    DC_TRACE(env);
    dc_setsockopt(env, err, socket_fd, level, option, &value, sizeof(value));
}

static inline void setsockopt_linger(const struct dc_posix_env *env, struct dc_error *err, int socket_fd, int level, int option, int on, int seconds)
{
    struct linger value;

    DC_TRACE(env);
    value.l_onoff = on;
    value.l_linger = seconds;
    dc_setsockopt(env, err, socket_fd, level, option, &value, sizeof(value));
}

static inline void setsockopt_timeval(const struct dc_posix_env *env, struct dc_error *err, int socket_fd, int level, int option, time_t seconds, long useconds)
{
    struct timeval value;

    DC_TRACE(env);
    value.tv_sec = seconds;
    value.tv_usec = useconds;
    dc_setsockopt(env, err, socket_fd, level, option, &value, sizeof(value));
}

static inline void setsockopt_socket_int(const struct dc_posix_env *env, struct dc_error *err, int socket_fd, int option, int value)
{
    DC_TRACE(env);
    setsockopt_int(env, err, socket_fd, SOL_SOCKET, option, value);
}

static inline void setsockopt_socket_linger(const struct dc_posix_env *env, struct dc_error *err, int socket_fd, int option, int on, int seconds)
{
    DC_TRACE(env);
    setsockopt_linger(env, err, socket_fd, SOL_SOCKET, option, on, seconds);
}

static inline void setsockopt_socket_timeval(const struct dc_posix_env *env, struct dc_error *err, int socket_fd, int option, time_t seconds, long useconds)
{
    DC_TRACE(env);
    setsockopt_timeval(env, err, socket_fd, SOL_SOCKET, option, seconds, useconds);
}

void dc_setsockopt_socket_BROADCAST(const struct dc_posix_env *env, struct dc_error *err, int socket_fd, bool value)
{
    DC_TRACE(env);
    setsockopt_socket_int(env, err, socket_fd, SO_BROADCAST, value);
}

void dc_setsockopt_socket_DEBUG(const struct dc_posix_env *env, struct dc_error *err, int socket_fd, bool value)
{
    DC_TRACE(env);
    setsockopt_socket_int(env, err, socket_fd, SO_DEBUG, value);
}

void dc_setsockopt_socket_DONTROUTE(const struct dc_posix_env *env, struct dc_error *err, int socket_fd, bool value)
{
    DC_TRACE(env);
    setsockopt_socket_int(env, err, socket_fd, SO_DONTROUTE, value);
}

void dc_setsockopt_socket_KEEPALIVE(const struct dc_posix_env *env, struct dc_error *err, int socket_fd, bool value)
{
    DC_TRACE(env);
    setsockopt_socket_int(env, err, socket_fd, SO_KEEPALIVE, value);
}

void dc_setsockopt_socket_LINGER(const struct dc_posix_env *env, struct dc_error *err, int socket_fd, bool on, int seconds)
{
    DC_TRACE(env);
    setsockopt_socket_linger(env, err, socket_fd, SO_LINGER, on, seconds);
}

void dc_setsockopt_socket_OOBINLINE(const struct dc_posix_env *env, struct dc_error *err, int socket_fd, bool value)
{
    DC_TRACE(env);
    setsockopt_socket_int(env, err, socket_fd, SO_OOBINLINE, value);
}

void dc_setsockopt_socket_RCVBUF(const struct dc_posix_env *env, struct dc_error *err, int socket_fd, int value)
{
    DC_TRACE(env);
    setsockopt_socket_int(env, err, socket_fd, SO_RCVBUF, value);
}

void dc_setsockopt_socket_RCVLOWAT(const struct dc_posix_env *env, struct dc_error *err, int socket_fd, int value)
{
    DC_TRACE(env);
    setsockopt_socket_int(env, err, socket_fd, SO_RCVLOWAT, value);
}

void dc_setsockopt_socket_RCVTIMEO(const struct dc_posix_env *env, struct dc_error *err, int socket_fd, time_t seconds, long useconds)
{
    DC_TRACE(env);
    setsockopt_socket_timeval(env, err, socket_fd, SO_RCVTIMEO, seconds, useconds);
}

void dc_setsockopt_socket_REUSEADDR(const struct dc_posix_env *env, struct dc_error *err, int socket_fd, bool value)
{
    DC_TRACE(env);
    setsockopt_socket_int(env, err, socket_fd, SO_REUSEADDR, value);
}

void dc_setsockopt_socket_SNDBUF(const struct dc_posix_env *env, struct dc_error *err, int socket_fd, int value)
{
    DC_TRACE(env);
    setsockopt_socket_int(env, err, socket_fd, SO_SNDBUF, value);
}

void dc_setsockopt_socket_SNDLOWAT(const struct dc_posix_env *env, struct dc_error *err, int socket_fd, int value)
{
    DC_TRACE(env);
    setsockopt_socket_int(env, err, socket_fd, SO_SNDLOWAT, value);
}

void dc_setsockopt_socket_SNDTIMEO(const struct dc_posix_env *env, struct dc_error *err, int socket_fd, time_t seconds, long useconds)
{
    DC_TRACE(env);
    setsockopt_socket_timeval(env, err, socket_fd, SO_SNDTIMEO, seconds, useconds);
}

void dc_setsockopt_socket_SOMAXCONN(const struct dc_posix_env *env, struct dc_error *err, int socket_fd, int value)
{
    DC_TRACE(env);
    setsockopt_socket_int(env, err, socket_fd, SOMAXCONN, value);
}
