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
#include <dc_c/dc_stdlib.h>
#include <dc_c/dc_string.h>
#include <dc_posix/dc_unistd.h>
#include <stdint.h>


size_t dc_read_fully(const struct dc_env *env, struct dc_error *err, int fd, void *buf, size_t nbytes)
{
    size_t bytes_read;
    uint8_t *bytes;

    DC_TRACE(env);
    bytes_read = 0;
    bytes = dc_malloc(env, err, nbytes);

    if(dc_error_has_no_error(err))
    {
        while(bytes_read < nbytes)
        {
            ssize_t nread;

            nread = dc_read(env, err, fd, &bytes[bytes_read], nbytes - bytes_read);

            if(dc_error_has_error(err))
            {
                break;
            }

            bytes_read += nread;
        }

        if(dc_error_has_no_error(err))
        {
            dc_memcpy(env, buf, bytes, bytes_read);
        }
    }

    return bytes_read;
}
