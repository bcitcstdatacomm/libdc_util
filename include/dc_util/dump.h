#ifndef LIBDC_UTIL_DUMP_H
#define LIBDC_UTIL_DUMP_H

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

#include <dc_posix/dc_stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <sys/types.h>

struct dc_dump_info;


/**
 *
 * @param env
 * @param err
 * @param fd
 * @param file_size
 * @return
 */
struct dc_dump_info *dc_dump_info_create(const struct dc_posix_env *env,
                                         struct dc_error *err, int fd,
                                         off_t file_size);

/**
 *
 * @param env
 * @param pinfo
 */
void dc_dump_info_destroy(const struct dc_posix_env *env,
                          struct dc_dump_info **pinfo);

/**
 *
 * @param env
 * @param err
 * @param data
 * @param count
 * @param file_position
 * @param arg
 */
void dc_dump_dumper(const struct dc_posix_env *env, struct dc_error *err,
                    const uint8_t *data, size_t count, size_t file_position,
                    void *arg);

#endif // LIBDC_UTIL_DUMP_H
