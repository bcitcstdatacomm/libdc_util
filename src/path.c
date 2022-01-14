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

#include "path.h"
#include <dc_posix/dc_stdlib.h>
#include <dc_posix/dc_string.h>
#include <dc_posix/dc_wordexp.h>

void dc_expand_path(const struct dc_posix_env *env, struct dc_error *err, char **expanded_path, const char *path)
{
    wordexp_t expanded;

    DC_TRACE(env);
    dc_wordexp(env, err, path, &expanded, 0);

    if(dc_error_has_no_error(err))
    {
        char *root;
        size_t root_length;

        root = *expanded.we_wordv;
        root_length = dc_strlen(env, root);
        *expanded_path = dc_malloc(env, err, (root_length + 1) * sizeof(char));

        if(dc_error_has_no_error(err))
        {
            dc_strcpy(env, *expanded_path, root);
        }
    }

    wordfree(&expanded);
}
