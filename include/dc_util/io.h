#ifndef LIBDC_UTIL_IO_H
#define LIBDC_UTIL_IO_H

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


#include <dc_env/env.h>
#include <stddef.h>
#include <sys/types.h>


/**
 *
 * @param env
 * @param err
 * @param fd
 * @param buf
 * @param nbytes
 * @return
 */
size_t dc_read_fully(const struct dc_env *env, struct dc_error *err, int fd, void *buf, size_t nbytes);


#endif // LIBDC_UTIL_IO_H
