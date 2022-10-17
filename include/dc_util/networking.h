#ifndef LIBDC_UTIL_NETWORKING_H
#define LIBDC_UTIL_NETWORKING_H


#include <dc_posix/arpa/dc_inet.h>
#include <dc_posix/dc_posix_env.h>
#include <netdb.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>


char *dc_inet_ntop_compat(const struct dc_posix_env *env, struct dc_error *err, const struct sockaddr_storage *sockaddr);
in_port_t dc_inet_get_port(const struct dc_posix_env *env, struct dc_error *err, const struct sockaddr_storage *sockaddr);
void dc_print_sockopts(const struct dc_posix_env *env, struct dc_error *err, int socket_fd, FILE *stream);
bool dc_getsockopt_socket_ACCEPTCONN(const struct dc_posix_env *env, struct dc_error *err, int socket_fd);
bool dc_getsockopt_socket_BROADCAST(const struct dc_posix_env *env, struct dc_error *err, int socket_fd);
bool dc_getsockopt_socket_DEBUG(const struct dc_posix_env *env, struct dc_error *err, int socket_fd);
bool dc_getsockopt_socket_DONTROUTE(const struct dc_posix_env *env, struct dc_error *err, int socket_fd);
int dc_getsockopt_socket_ERROR(const struct dc_posix_env *env, struct dc_error *err, int socket_fd);
bool dc_getsockopt_socket_KEEPALIVE(const struct dc_posix_env *env, struct dc_error *err, int socket_fd);
struct linger dc_getsockopt_socket_LINGER(const struct dc_posix_env *env, struct dc_error *err, int socket_fd);
bool dc_getsockopt_socket_OOBINLINE(const struct dc_posix_env *env, struct dc_error *err, int socket_fd);
int dc_getsockopt_socket_RCVBUF(const struct dc_posix_env *env, struct dc_error *err, int socket_fd);
int dc_getsockopt_socket_RCVLOWAT(const struct dc_posix_env *env, struct dc_error *err, int socket_fd);
struct timeval dc_getsockopt_socket_RCVTIMEO(const struct dc_posix_env *env, struct dc_error *err, int socket_fd);
bool dc_getsockopt_socket_REUSEADDR(const struct dc_posix_env *env, struct dc_error *err, int socket_fd);
int dc_getsockopt_socket_SNDBUF(const struct dc_posix_env *env, struct dc_error *err, int socket_fd);
int dc_getsockopt_socket_SNDLOWAT(const struct dc_posix_env *env, struct dc_error *err, int socket_fd);
struct timeval dc_getsockopt_socket_SNDTIMEO(const struct dc_posix_env *env, struct dc_error *err, int socket_fd);
int dc_getsockopt_socket_TYPE(const struct dc_posix_env *env, struct dc_error *err, int socket_fd);
int dc_getsockopt_socket_MAXCONN(const struct dc_posix_env *env, struct dc_error *err, int socket_fd);
void dc_setsockopt_socket_BROADCAST(const struct dc_posix_env *env, struct dc_error *err, int socket_fd, bool value);
void dc_setsockopt_socket_DEBUG(const struct dc_posix_env *env, struct dc_error *err, int socket_fd, bool value);
void dc_setsockopt_socket_DONTROUTE(const struct dc_posix_env *env, struct dc_error *err, int socket_fd, bool value);
void dc_setsockopt_socket_KEEPALIVE(const struct dc_posix_env *env, struct dc_error *err, int socket_fd, bool value);
void dc_setsockopt_socket_LINGER(const struct dc_posix_env *env, struct dc_error *err, int socket_fd, bool on, int seconds);
void dc_setsockopt_socket_OOBINLINE(const struct dc_posix_env *env, struct dc_error *err, int socket_fd, bool value);
void dc_setsockopt_socket_RCVBUF(const struct dc_posix_env *env, struct dc_error *err, int socket_fd, int value);
void dc_setsockopt_socket_RCVLOWAT(const struct dc_posix_env *env, struct dc_error *err, int socket_fd, int value);
void dc_setsockopt_socket_RCVTIMEO(const struct dc_posix_env *env, struct dc_error *err, int socket_fd, time_t seconds, long useconds);
void dc_setsockopt_socket_REUSEADDR(const struct dc_posix_env *env, struct dc_error *err, int socket_fd, bool value);
void dc_setsockopt_socket_SNDBUF(const struct dc_posix_env *env, struct dc_error *err, int socket_fd, int value);
void dc_setsockopt_socket_SNDLOWAT(const struct dc_posix_env *env, struct dc_error *err, int socket_fd, int value);
void dc_setsockopt_socket_SNDTIMEO(const struct dc_posix_env *env, struct dc_error *err, int socket_fd, time_t seconds, long useconds);
void dc_setsockopt_socket_MAXCONN(const struct dc_posix_env *env, struct dc_error *err, int socket_fd, int value);

#endif //LIBDC_UTIL_NETWORKING_H
