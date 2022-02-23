#include "networking.h"
#include <dc_posix/dc_stdlib.h>
#include <netinet/in.h>


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
