#include <stddef.h>

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

#include "filesystem.h"
#include <dc_posix/dc_posix_env.h>
#include <dc_posix/dc_stdlib.h>
#include <dc_posix/dc_unistd.h>
#include <sys/unistd.h>

char *dc_get_working_dir(const struct dc_posix_env *env, struct dc_error *err)
{
    long path_max;
    size_t size;
    char *buf;
    char *ptr;

    DC_TRACE(env);

    path_max = dc_pathconf(env, err, ".", _PC_PATH_MAX);

    if(path_max == -1)
    {
        size = 1024;
    }
    else
    {
        size = (size_t)path_max;
    }

    for(buf = ptr = NULL; ptr == NULL; size *= 2)
    {
        buf = dc_realloc(env, err, buf, size);

        if(dc_error_has_no_error(err))
        {
            ptr = dc_getcwd(env, err, buf, size);

            if(dc_error_has_error(err))
            {
                if(err->errno_code == ERANGE)
                {
                    dc_error_reset(err);
                }
                else
                {
                    break;
                }
            }
        }
    }

    return buf;
}
