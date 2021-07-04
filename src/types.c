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


#include "types.h"
#include <dc_posix/inttypes.h>
#include <math.h>


__attribute__ ((unused)) inline off_t dc_max_off_t(const struct dc_posix_env *env)
{
    static off_t max = 0;

    DC_TRACE(env);

    if(max == 0)
    {
        size_t      bits;
        long double largest_signed;
        uintmax_t   largest_unsignedA;
        uintmax_t   largest_unsignedB;

        // TODO: check for errors
        bits              = sizeof(off_t) * 8;
        largest_signed    = powl(2, bits);
        largest_unsignedA = (uintmax_t)(largest_signed / 2);
        largest_unsignedB = largest_unsignedA - 1;
        max               = (off_t)largest_unsignedB;
    }

    return max;
}


uint16_t dc_uint16_from_str(const struct dc_posix_env *env, struct dc_error *err, const char *str, int base)
{
    char      *endptr;
    uintmax_t  value;

    DC_TRACE(env);
    value = dc_strtoumax(env, err, str, &endptr, base);

    if(DC_HAS_NO_ERROR(err))
    {
        if(value > UINT16_MAX)
        {
            DC_REPORT_ERRNO(env, err, ERANGE);
            value = 0;
        }
    }
    else
    {
        value = 0;
    }

    return (uint16_t)value;
}
