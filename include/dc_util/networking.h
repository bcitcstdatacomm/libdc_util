#ifndef LIBDC_UTIL_NETWORKING_H
#define LIBDC_UTIL_NETWORKING_H


/*
 * Copyright 2022-2021 D'Arcy Smith.
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


#include <dc_env/env.h>
#include <netinet/in.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>


#ifdef __cplusplus
extern "C" {
#endif


char *dc_inet_ntop_compat(const struct dc_env *env, struct dc_error *err, const struct sockaddr_storage *sockaddr);
in_port_t dc_inet_get_port(const struct dc_env *env, struct dc_error *err, const struct sockaddr_storage *sockaddr);
void dc_print_sockopts(const struct dc_env *env, struct dc_error *err, int socket_fd, FILE *stream);
bool dc_getsockopt_socket_ACCEPTCONN(const struct dc_env *env, struct dc_error *err, int socket_fd);
bool dc_getsockopt_socket_BROADCAST(const struct dc_env *env, struct dc_error *err, int socket_fd);
bool dc_getsockopt_socket_DEBUG(const struct dc_env *env, struct dc_error *err, int socket_fd);
bool dc_getsockopt_socket_DONTROUTE(const struct dc_env *env, struct dc_error *err, int socket_fd);
int dc_getsockopt_socket_ERROR(const struct dc_env *env, struct dc_error *err, int socket_fd);
bool dc_getsockopt_socket_KEEPALIVE(const struct dc_env *env, struct dc_error *err, int socket_fd);
struct linger dc_getsockopt_socket_LINGER(const struct dc_env *env, struct dc_error *err, int socket_fd);
bool dc_getsockopt_socket_OOBINLINE(const struct dc_env *env, struct dc_error *err, int socket_fd);
int dc_getsockopt_socket_RCVBUF(const struct dc_env *env, struct dc_error *err, int socket_fd);
int dc_getsockopt_socket_RCVLOWAT(const struct dc_env *env, struct dc_error *err, int socket_fd);
struct timeval dc_getsockopt_socket_RCVTIMEO(const struct dc_env *env, struct dc_error *err, int socket_fd);
bool dc_getsockopt_socket_REUSEADDR(const struct dc_env *env, struct dc_error *err, int socket_fd);
int dc_getsockopt_socket_SNDBUF(const struct dc_env *env, struct dc_error *err, int socket_fd);
int dc_getsockopt_socket_SNDLOWAT(const struct dc_env *env, struct dc_error *err, int socket_fd);
struct timeval dc_getsockopt_socket_SNDTIMEO(const struct dc_env *env, struct dc_error *err, int socket_fd);
int dc_getsockopt_socket_TYPE(const struct dc_env *env, struct dc_error *err, int socket_fd);
int dc_getsockopt_socket_MAXCONN(const struct dc_env *env, struct dc_error *err, int socket_fd);
void dc_setsockopt_socket_BROADCAST(const struct dc_env *env, struct dc_error *err, int socket_fd, bool value);
void dc_setsockopt_socket_DEBUG(const struct dc_env *env, struct dc_error *err, int socket_fd, bool value);
void dc_setsockopt_socket_DONTROUTE(const struct dc_env *env, struct dc_error *err, int socket_fd, bool value);
void dc_setsockopt_socket_KEEPALIVE(const struct dc_env *env, struct dc_error *err, int socket_fd, bool value);
void dc_setsockopt_socket_LINGER(const struct dc_env *env, struct dc_error *err, int socket_fd, bool on, int seconds);
void dc_setsockopt_socket_OOBINLINE(const struct dc_env *env, struct dc_error *err, int socket_fd, bool value);
void dc_setsockopt_socket_RCVBUF(const struct dc_env *env, struct dc_error *err, int socket_fd, int value);
void dc_setsockopt_socket_RCVLOWAT(const struct dc_env *env, struct dc_error *err, int socket_fd, int value);
void dc_setsockopt_socket_RCVTIMEO(const struct dc_env *env, struct dc_error *err, int socket_fd, time_t seconds, long useconds);
void dc_setsockopt_socket_REUSEADDR(const struct dc_env *env, struct dc_error *err, int socket_fd, bool value);
void dc_setsockopt_socket_SNDBUF(const struct dc_env *env, struct dc_error *err, int socket_fd, int value);
void dc_setsockopt_socket_SNDLOWAT(const struct dc_env *env, struct dc_error *err, int socket_fd, int value);
void dc_setsockopt_socket_SNDTIMEO(const struct dc_env *env, struct dc_error *err, int socket_fd, time_t seconds, long useconds);
void dc_setsockopt_socket_MAXCONN(const struct dc_env *env, struct dc_error *err, int socket_fd, int value);
char *dc_get_ip_addresses_by_interface(struct dc_env *env, struct dc_error *err, const char *interface_name, int family);
char *dc_get_default_interface(struct dc_env *env, struct dc_error *err, int family);


#ifdef __cplusplus
}
#endif


#endif //LIBDC_UTIL_NETWORKING_H
