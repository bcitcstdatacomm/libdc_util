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


#include "dc_util/types.h"
#include <dc_c/dc_inttypes.h>
#include <dc_c/dc_math.h>
#include <dc_c/dc_stdlib.h>
#include <float.h>
#include <inttypes.h>
#include <limits.h>
#include <stdio.h>


inline off_t dc_max_off_t(const struct dc_env *env, struct dc_error *err)
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
        bits              = sizeof(off_t) * CHAR_BIT;
        largest_signed    = dc_powl(env, err, 2, bits);
        largest_unsignedA = (uintmax_t)(largest_signed / 2);
        largest_unsignedB = largest_unsignedA - 1;
        max               = (off_t)largest_unsignedB;
    }

    return max;
}

uint16_t dc_uint8_from_str(const struct dc_env *env, struct dc_error *err, const char *str, int base)
{
    char     *endptr;
    uintmax_t value;

    DC_TRACE(env);
    value = dc_strtoumax(env, err, str, &endptr, base);

    if(dc_error_has_no_error(err))
    {
        if(value > UINT8_MAX)
        {
            // TODO: we should not assume 64 bits...
            //                            19          + 6 + 5 + '\0'
            static const char  *format = "%" PRIuMAX " is greater than %" PRIu8;
            static const size_t size   = (19 + 6 + 5 + 1) * sizeof(char);
            struct dc_error     *local_err;
            bool                 reporting;
            char                *msg;

            reporting = dc_error_is_reporting(err);
            local_err = dc_error_create(reporting);
            msg = dc_malloc(env, local_err, size);

            if(dc_error_has_no_error(local_err))
            {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-nonliteral"
                sprintf(msg, format, value, UINT8_MAX);    // NOLINT(cert-err33-c)
#pragma GCC diagnostic pop
                DC_ERROR_RAISE_SYSTEM(err, msg, ERANGE);
                dc_free(env, msg);
            }
            else
            {
                DC_ERROR_RAISE_SYSTEM(err, "Out of memory", ENOMEM);
            }

            value = 0;
        }
    }
    else
    {
        value = 0;
    }

    return (uint8_t)value;
}

uint16_t dc_uint16_from_str(const struct dc_env *env, struct dc_error *err, const char *str, int base)
{
    char     *endptr;
    uintmax_t value;

    DC_TRACE(env);
    value = dc_strtoumax(env, err, str, &endptr, base);

    if(dc_error_has_no_error(err))
    {
        if(value > UINT16_MAX)
        {
            // TODO: we should not assume 64 bits...
            //                            19          + 6 + 5 + '\0'
            static const char  *format = "%" PRIuMAX " is greater than %" PRIu16;
            static const size_t size   = (19 + 6 + 5 + 1) * sizeof(char);
            struct dc_error     *local_err;
            bool                 reporting;
            char                *msg;

            reporting = dc_error_is_reporting(err);
            local_err = dc_error_create(reporting);
            msg = dc_malloc(env, local_err, size);

            if(dc_error_has_no_error(local_err))
            {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-nonliteral"
                sprintf(msg, format, value, UINT16_MAX);    // NOLINT(cert-err33-c)
#pragma GCC diagnostic pop
                DC_ERROR_RAISE_SYSTEM(err, msg, ERANGE);
                dc_free(env, msg);
            }
            else
            {
                DC_ERROR_RAISE_SYSTEM(err, "Out of memory", ENOMEM);
            }

            value = 0;
        }
    }
    else
    {
        value = 0;
    }

    return (uint16_t)value;
}

long dc_long_from_str(const struct dc_env *env, struct dc_error *err, const char *str, int base)
{
    char     *endptr;
    uintmax_t value;

    DC_TRACE(env);
    value = dc_strtoumax(env, err, str, &endptr, base);

    if(dc_error_has_no_error(err))
    {
        if(value > LONG_MAX || value < LONG_MIN)
        {
            // TODO: we should not assume 64 bits...
            //                            19          + 6 + 5 + '\0'
            static const char  *format = "%" PRIuMAX " is greater than %" PRIu16;
            static const size_t size   = (19 + 6 + 5 + 1) * sizeof(char);
            struct dc_error     *local_err;
            bool                 reporting;
            char                *msg;

            reporting = dc_error_is_reporting(err);
            local_err = dc_error_create(reporting);
            msg = dc_malloc(env, local_err, size);

            if(dc_error_has_no_error(local_err))
            {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-nonliteral"
                sprintf(msg, format, value, UINT16_MAX);    // NOLINT(cert-err33-c)
#pragma GCC diagnostic pop
                DC_ERROR_RAISE_SYSTEM(err, msg, ERANGE);
                dc_free(env, msg);
            }
            else
            {
                DC_ERROR_RAISE_SYSTEM(err, "Out of memory", ENOMEM);
            }

            value = 0;
        }
    }
    else
    {
        value = 0;
    }

    return (uint16_t)value;
}

#define TIME_MIN(type) _Generic((type){0}, \
  float: FLT_MIN, \
  double: DBL_MIN, \
  long double: LDBL_MIN, \
  signed char: SCHAR_MIN, \
  signed short: SHRT_MIN, \
  signed int: INT_MIN, \
  signed long: LONG_MIN, \
  signed long long: LLONG_MIN, \
  unsigned char: 0, \
  unsigned short: 0, \
  unsigned int: 0, \
  unsigned long: 0, \
  unsigned long long: 0 \
)

#define TIME_MAX(type) _Generic((type){0}, \
  float: FLT_MAX, \
  double: DBL_MAX, \
  long double: LDBL_MAX, \
  signed char: SCHAR_MAX, \
  signed short: SHRT_MAX, \
  signed int: INT_MAX, \
  signed long: LONG_MAX, \
  signed long long: LLONG_MAX, \
  unsigned char: UCHAR_MAX, \
  unsigned short: USHRT_MAX, \
  unsigned int: UINT_MAX, \
  unsigned long: ULONG_MAX, \
  unsigned long long: ULLONG_MAX \
)

time_t dc_time_t_from_str(const struct dc_env *const env, struct dc_error *err, const char *str)
{
    char *endptr;
    intmax_t value = dc_strtoimax(env, err, str, &endptr, 10);
    time_t min;
    time_t max;

    min = TIME_MIN(time_t);
    max = TIME_MAX(time_t);

    if(dc_error_has_no_error(err))
    {
        if(value > max || value < min)
        {
            static const char *format = "Error: value %" PRIdMAX " is outside the range of time_t (%ld, %ld)";
            static const size_t size = (7 + 19 + 4 + 7 + 6 + 4 + 3 + 7 + 1) * sizeof(char);
            struct dc_error *local_err;
            bool reporting;
            char *msg;

            reporting = dc_error_is_reporting(err);
            local_err = dc_error_create(reporting);
            msg = dc_malloc(env, local_err, size);

            if(dc_error_has_no_error(local_err))
            {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-nonliteral"
                sprintf(msg, format, value, (long) max, (long) min);
#pragma GCC diagnostic pop
                DC_ERROR_RAISE_SYSTEM(err, msg, ERANGE);
                dc_free(env, msg);
            }
            else
            {
                DC_ERROR_RAISE_SYSTEM(err, "Error: Out of memory", ENOMEM);
            }

            value = 0;
        }
    } else {
        value = 0;
    }

    return (time_t) value;
}

in_port_t dc_in_port_t_from_str(const struct dc_env *env, struct dc_error *err, const char *str, int base)
{
    in_port_t value;

    value = dc_uint16_from_str(env, err, str, base);

    return value;
}
