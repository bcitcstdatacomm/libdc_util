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


#include "dc_util/io.h"
#include <dc_c/dc_string.h>
#include <dc_posix/dc_unistd.h>
#include <stdint.h>


ssize_t dc_write_fully(struct dc_env *env, struct dc_error *err, int fd, const void *buffer, size_t len)
{
    ssize_t total_bytes_write = 0;

    while(total_bytes_write < (ssize_t)len)
    {
        ssize_t bytes_written;

        bytes_written = dc_write(env, err, fd, (const uint8_t *)buffer + total_bytes_write, len - total_bytes_write);

        if(dc_error_has_error(err))
        {
            break;
        }

        total_bytes_write += bytes_written;
    }

    return total_bytes_write;
}

ssize_t dc_read_fully(struct dc_env *env, struct dc_error *err, int fd, void *buffer, size_t len)
{
    ssize_t total_bytes_read = 0;

    while (total_bytes_read < (ssize_t)len)
    {
        ssize_t bytes_read;

        bytes_read = dc_read(env, err, fd, (uint8_t *)buffer + total_bytes_read, len - total_bytes_read);

        if(dc_error_has_error(err))
        {
            break;
        }

        total_bytes_read += bytes_read;
    }

    return total_bytes_read;
}

