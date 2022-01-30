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

#include "strings.h"
#include <ctype.h>
#include <dc_posix/dc_string.h>
#include <stdarg.h>
#include <dc_posix/dc_stdlib.h>

char *dc_str_left_trim(const struct dc_posix_env *env, char *str)
{
    DC_TRACE(env);

    if(str != NULL)
    {
        size_t i;

        i = 0;

        while(isspace(str[i]))
        {
            i++;
        }

        if(i > 0)
        {
            size_t length;

            length = dc_strlen(env, str);
            dc_memmove(env, str, &str[i], length + 1 - i);
        }
    }

    return str;
}

char *dc_str_right_trim(const struct dc_posix_env *env, char *str)
{
    DC_TRACE(env);

    if(str != NULL)
    {
        size_t length;
        size_t i;

        length = dc_strlen(env, str);
        i = length - 1;

        while(isspace(str[i]))
        {
            i--;
        }

        str[i + 1] = '\0';
    }

    return str;
}

char *dc_str_trim(const struct dc_posix_env *env, char *str)
{
    DC_TRACE(env);

    if(str != NULL && str[0])
    {
        str = dc_str_left_trim(env, str);

        if(str[0])
        {
            str = dc_str_right_trim(env, str);
        }
    }

    return str;
}

ssize_t dc_str_find_last(const struct dc_posix_env *env, const char *str, int c)
{
    size_t index;

    DC_TRACE(env);
    index = (size_t)dc_strlen(env, str) - 1;

    while(index >= 0)
    {
        if(str[index] == c)
        {
            return (ssize_t)index;
        }

        index--;
    }

    return -1;
}

char **dc_strs_to_array(const struct dc_posix_env *env, struct dc_error *err, size_t n, ...)
{
    char **array;
    va_list args;

    DC_TRACE(env);
    array = dc_calloc(env, err, n, sizeof(char *));

    if(dc_error_has_error(err))
    {
        array = NULL;
    }
    else
    {
        va_start(args, n);

        for(size_t i = 0; i < n; i++)
        {
            char *str;

            str = va_arg(args, char *);

            if(str)
            {
                array[i] = dc_strdup(env, err, str);

                if(dc_error_has_error(err))
                {
                    // TODO: clean up app memory allocated so far
                    array = NULL;
                    break;
                }
            }
        }

        va_end(args);
    }

    return array;
}

void dc_strs_destroy_array(const struct dc_posix_env *env, size_t n, char **parray)
{
    for(size_t i = 0; i < n; i++)
    {
        if(parray[i])
        {
            dc_free(env, parray[i], dc_strlen(env, parray[i]));
            parray[i] = NULL;
        }
    }
}

size_t dc_str_find_all(const struct dc_posix_env *env, const char *str, int c)
{
    size_t num;

    DC_TRACE(env);
    num = 0;

    for(const char *tmp = str; *tmp; tmp++)
    {
        if(*tmp == c)
        {
            num++;
        }
    }

    return num;
}
