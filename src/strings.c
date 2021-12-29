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

char *trim(const struct dc_posix_env *env, char *str)
{
    DC_TRACE(env);

    if(str != NULL)
    {
        char *temp;
        size_t first;

        temp = str;
        first = 0;

        while(isspace(*temp))
        {
            temp++;
            first++;
        }

        if(!(*temp))
        {
            *str = '\0';
        }
        else
        {
            size_t length;
            char *dest;

            length = dc_strlen(env, temp);
            temp = &str[length - 1];

            while(isspace(*temp))
            {
                *temp = '\0';
                temp--;
            }

            dest = str;
            temp = &str[first];

            while(*temp)
            {
                *dest++ = *temp++;
            }

            *dest = '\0';
        }
    }

    return str;
}

ssize_t find_last(const struct dc_posix_env *env, const char *str, int c)
{
    size_t index;

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
