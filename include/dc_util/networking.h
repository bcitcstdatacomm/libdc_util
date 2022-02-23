#ifndef LIBDC_UTIL_NETWORKING_H
#define LIBDC_UTIL_NETWORKING_H

#include <dc_posix/arpa/dc_inet.h>
#include <dc_posix/dc_posix_env.h>

char *dc_inet_ntop_compat(const struct dc_posix_env *env, struct dc_error *err, const struct sockaddr_storage *sockaddr);
in_port_t dc_inet_get_port(const struct dc_posix_env *env, struct dc_error *err, const struct sockaddr_storage *sockaddr);

#endif //LIBDC_UTIL_NETWORKING_H
