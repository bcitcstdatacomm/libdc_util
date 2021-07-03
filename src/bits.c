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


#include "bits.h"
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>


static void to_binary(const struct dc_posix_env *env, uint32_t val, bool *bits, const uint32_t *masks, size_t count);
static void to_printable_binary(const struct dc_posix_env *env, size_t count, const bool bits[static count], char printable[static count + 1]);
static void from_printable_binary(const struct dc_posix_env *env, size_t count, const char printable[static count + 1], bool bits[static count]);

static bool get_bit32(const struct dc_posix_env *env, uint32_t val, uint32_t mask);
static bool get_bit64(const struct dc_posix_env *env, uint64_t val, uint64_t mask);
static void set_bit(const struct dc_posix_env *env, size_t position, const char printable[static position], bool bits[static position]);


#define MASK_00000000_00000000_00000000_00000001 0x00000001
#define MASK_00000000_00000000_00000000_00000010 0x00000002
#define MASK_00000000_00000000_00000000_00000100 0x00000004
#define MASK_00000000_00000000_00000000_00001000 0x00000008
#define MASK_00000000_00000000_00000000_00010000 0x00000010
#define MASK_00000000_00000000_00000000_00100000 0x00000020
#define MASK_00000000_00000000_00000000_01000000 0x00000040
#define MASK_00000000_00000000_00000000_10000000 0x00000080
#define MASK_00000000_00000000_00000001_00000000 0x00000100
#define MASK_00000000_00000000_00000010_00000000 0x00000200
#define MASK_00000000_00000000_00000100_00000000 0x00000400
#define MASK_00000000_00000000_00001000_00000000 0x00000800
#define MASK_00000000_00000000_00010000_00000000 0x00001000
#define MASK_00000000_00000000_00100000_00000000 0x00002000
#define MASK_00000000_00000000_01000000_00000000 0x00004000
#define MASK_00000000_00000000_10000000_00000000 0x00008000
#define MASK_00000000_00000001_00000000_00000000 0x00010000
#define MASK_00000000_00000010_00000000_00000000 0x00020000
#define MASK_00000000_00000100_00000000_00000000 0x00040000
#define MASK_00000000_00001000_00000000_00000000 0x00080000
#define MASK_00000000_00010000_00000000_00000000 0x00100000
#define MASK_00000000_00100000_00000000_00000000 0x00200000
#define MASK_00000000_01000000_00000000_00000000 0x00400000
#define MASK_00000000_10000000_00000000_00000000 0x00800000
#define MASK_00000001_00000000_00000000_00000000 0x01000000
#define MASK_00000010_00000000_00000000_00000000 0x02000000
#define MASK_00000100_00000000_00000000_00000000 0x04000000
#define MASK_00001000_00000000_00000000_00000000 0x08000000
#define MASK_00010000_00000000_00000000_00000000 0x10000000
#define MASK_00100000_00000000_00000000_00000000 0x20000000
#define MASK_01000000_00000000_00000000_00000000 0x40000000
#define MASK_10000000_00000000_00000000_00000000 0x80000000
#define MASK_00000000_00000000_00000000_00000001_00000000_00000000_00000000_00000000 0x0000000100000000
#define MASK_00000000_00000000_00000000_00000010_00000000_00000000_00000000_00000000 0x0000000200000000
#define MASK_00000000_00000000_00000000_00000100_00000000_00000000_00000000_00000000 0x0000000400000000
#define MASK_00000000_00000000_00000000_00001000_00000000_00000000_00000000_00000000 0x0000000800000000
#define MASK_00000000_00000000_00000000_00010000_00000000_00000000_00000000_00000000 0x0000001000000000
#define MASK_00000000_00000000_00000000_00100000_00000000_00000000_00000000_00000000 0x0000002000000000
#define MASK_00000000_00000000_00000000_01000000_00000000_00000000_00000000_00000000 0x0000004000000000
#define MASK_00000000_00000000_00000000_10000000_00000000_00000000_00000000_00000000 0x0000008000000000
#define MASK_00000000_00000000_00000001_00000000_00000000_00000000_00000000_00000000 0x0000010000000000
#define MASK_00000000_00000000_00000010_00000000_00000000_00000000_00000000_00000000 0x0000020000000000
#define MASK_00000000_00000000_00000100_00000000_00000000_00000000_00000000_00000000 0x0000040000000000
#define MASK_00000000_00000000_00001000_00000000_00000000_00000000_00000000_00000000 0x0000080000000000
#define MASK_00000000_00000000_00010000_00000000_00000000_00000000_00000000_00000000 0x0000100000000000
#define MASK_00000000_00000000_00100000_00000000_00000000_00000000_00000000_00000000 0x0000200000000000
#define MASK_00000000_00000000_01000000_00000000_00000000_00000000_00000000_00000000 0x0000400000000000
#define MASK_00000000_00000000_10000000_00000000_00000000_00000000_00000000_00000000 0x0000800000000000
#define MASK_00000000_00000001_00000000_00000000_00000000_00000000_00000000_00000000 0x0001000000000000
#define MASK_00000000_00000010_00000000_00000000_00000000_00000000_00000000_00000000 0x0002000000000000
#define MASK_00000000_00000100_00000000_00000000_00000000_00000000_00000000_00000000 0x0004000000000000
#define MASK_00000000_00001000_00000000_00000000_00000000_00000000_00000000_00000000 0x0008000000000000
#define MASK_00000000_00010000_00000000_00000000_00000000_00000000_00000000_00000000 0x0010000000000000
#define MASK_00000000_00100000_00000000_00000000_00000000_00000000_00000000_00000000 0x0020000000000000
#define MASK_00000000_01000000_00000000_00000000_00000000_00000000_00000000_00000000 0x0040000000000000
#define MASK_00000000_10000000_00000000_00000000_00000000_00000000_00000000_00000000 0x0080000000000000
#define MASK_00000001_00000000_00000000_00000000_00000000_00000000_00000000_00000000 0x0100000000000000
#define MASK_00000010_00000000_00000000_00000000_00000000_00000000_00000000_00000000 0x0200000000000000
#define MASK_00000100_00000000_00000000_00000000_00000000_00000000_00000000_00000000 0x0400000000000000
#define MASK_00001000_00000000_00000000_00000000_00000000_00000000_00000000_00000000 0x0800000000000000
#define MASK_00010000_00000000_00000000_00000000_00000000_00000000_00000000_00000000 0x1000000000000000
#define MASK_00100000_00000000_00000000_00000000_00000000_00000000_00000000_00000000 0x2000000000000000
#define MASK_01000000_00000000_00000000_00000000_00000000_00000000_00000000_00000000 0x4000000000000000
#define MASK_10000000_00000000_00000000_00000000_00000000_00000000_00000000_00000000 0x8000000000000000

static uint32_t masks_8[] =
        {
                MASK_00000000_00000000_00000000_10000000,
                MASK_00000000_00000000_00000000_01000000,
                MASK_00000000_00000000_00000000_00100000,
                MASK_00000000_00000000_00000000_00010000,
                MASK_00000000_00000000_00000000_00001000,
                MASK_00000000_00000000_00000000_00000100,
                MASK_00000000_00000000_00000000_00000010,
                MASK_00000000_00000000_00000000_00000001,
        };

static uint32_t masks_16[] =
        {
                MASK_00000000_10000000_00000000_00000000,
                MASK_00000000_01000000_00000000_00000000,
                MASK_00000000_00100000_00000000_00000000,
                MASK_00000000_00010000_00000000_00000000,
                MASK_00000000_00001000_00000000_00000000,
                MASK_00000000_00000100_00000000_00000000,
                MASK_00000000_00000010_00000000_00000000,
                MASK_00000000_00000001_00000000_00000000,
                MASK_00000000_00000000_00000000_10000000,
                MASK_00000000_00000000_00000000_01000000,
                MASK_00000000_00000000_00000000_00100000,
                MASK_00000000_00000000_00000000_00010000,
                MASK_00000000_00000000_00000000_00001000,
                MASK_00000000_00000000_00000000_00000100,
                MASK_00000000_00000000_00000000_00000010,
                MASK_00000000_00000000_00000000_00000001,
        };

static uint32_t masks_32[] =
        {
                MASK_10000000_00000000_00000000_00000000,
                MASK_01000000_00000000_00000000_00000000,
                MASK_00100000_00000000_00000000_00000000,
                MASK_00010000_00000000_00000000_00000000,
                MASK_00001000_00000000_00000000_00000000,
                MASK_00000100_00000000_00000000_00000000,
                MASK_00000010_00000000_00000000_00000000,
                MASK_00000001_00000000_00000000_00000000,
                MASK_00000000_10000000_00000000_00000000,
                MASK_00000000_01000000_00000000_00000000,
                MASK_00000000_00100000_00000000_00000000,
                MASK_00000000_00010000_00000000_00000000,
                MASK_00000000_00001000_00000000_00000000,
                MASK_00000000_00000100_00000000_00000000,
                MASK_00000000_00000010_00000000_00000000,
                MASK_00000000_00000001_00000000_00000000,
                MASK_00000000_00000000_10000000_00000000,
                MASK_00000000_00000000_01000000_00000000,
                MASK_00000000_00000000_00100000_00000000,
                MASK_00000000_00000000_00010000_00000000,
                MASK_00000000_00000000_00001000_00000000,
                MASK_00000000_00000000_00000100_00000000,
                MASK_00000000_00000000_00000010_00000000,
                MASK_00000000_00000000_00000001_00000000,
                MASK_00000000_00000000_00000000_10000000,
                MASK_00000000_00000000_00000000_01000000,
                MASK_00000000_00000000_00000000_00100000,
                MASK_00000000_00000000_00000000_00010000,
                MASK_00000000_00000000_00000000_00001000,
                MASK_00000000_00000000_00000000_00000100,
                MASK_00000000_00000000_00000000_00000010,
                MASK_00000000_00000000_00000000_00000001,
        };

static uint64_t masks_64[] =
        {
            MASK_10000000_00000000_00000000_00000000_00000000_00000000_00000000_00000000,
            MASK_01000000_00000000_00000000_00000000_00000000_00000000_00000000_00000000,
            MASK_00100000_00000000_00000000_00000000_00000000_00000000_00000000_00000000,
            MASK_00010000_00000000_00000000_00000000_00000000_00000000_00000000_00000000,
            MASK_00001000_00000000_00000000_00000000_00000000_00000000_00000000_00000000,
            MASK_00000100_00000000_00000000_00000000_00000000_00000000_00000000_00000000,
            MASK_00000010_00000000_00000000_00000000_00000000_00000000_00000000_00000000,
            MASK_00000001_00000000_00000000_00000000_00000000_00000000_00000000_00000000,
            MASK_00000000_10000000_00000000_00000000_00000000_00000000_00000000_00000000,
            MASK_00000000_01000000_00000000_00000000_00000000_00000000_00000000_00000000,
            MASK_00000000_00100000_00000000_00000000_00000000_00000000_00000000_00000000,
            MASK_00000000_00010000_00000000_00000000_00000000_00000000_00000000_00000000,
            MASK_00000000_00001000_00000000_00000000_00000000_00000000_00000000_00000000,
            MASK_00000000_00000100_00000000_00000000_00000000_00000000_00000000_00000000,
            MASK_00000000_00000010_00000000_00000000_00000000_00000000_00000000_00000000,
            MASK_00000000_00000001_00000000_00000000_00000000_00000000_00000000_00000000,
            MASK_00000000_00000000_10000000_00000000_00000000_00000000_00000000_00000000,
            MASK_00000000_00000000_01000000_00000000_00000000_00000000_00000000_00000000,
            MASK_00000000_00000000_00100000_00000000_00000000_00000000_00000000_00000000,
            MASK_00000000_00000000_00010000_00000000_00000000_00000000_00000000_00000000,
            MASK_00000000_00000000_00001000_00000000_00000000_00000000_00000000_00000000,
            MASK_00000000_00000000_00000100_00000000_00000000_00000000_00000000_00000000,
            MASK_00000000_00000000_00000010_00000000_00000000_00000000_00000000_00000000,
            MASK_00000000_00000000_00000001_00000000_00000000_00000000_00000000_00000000,
            MASK_00000000_00000000_00000000_10000000_00000000_00000000_00000000_00000000,
            MASK_00000000_00000000_00000000_01000000_00000000_00000000_00000000_00000000,
            MASK_00000000_00000000_00000000_00100000_00000000_00000000_00000000_00000000,
            MASK_00000000_00000000_00000000_00010000_00000000_00000000_00000000_00000000,
            MASK_00000000_00000000_00000000_00001000_00000000_00000000_00000000_00000000,
            MASK_00000000_00000000_00000000_00000100_00000000_00000000_00000000_00000000,
            MASK_00000000_00000000_00000000_00000010_00000000_00000000_00000000_00000000,
            MASK_00000000_00000000_00000000_00000001_00000000_00000000_00000000_00000000,
                                                MASK_10000000_00000000_00000000_00000000,
                                                MASK_01000000_00000000_00000000_00000000,
                                                MASK_00100000_00000000_00000000_00000000,
                                                MASK_00010000_00000000_00000000_00000000,
                                                MASK_00001000_00000000_00000000_00000000,
                                                MASK_00000100_00000000_00000000_00000000,
                                                MASK_00000010_00000000_00000000_00000000,
                                                MASK_00000001_00000000_00000000_00000000,
                                                MASK_00000000_10000000_00000000_00000000,
                                                MASK_00000000_01000000_00000000_00000000,
                                                MASK_00000000_00100000_00000000_00000000,
                                                MASK_00000000_00010000_00000000_00000000,
                                                MASK_00000000_00001000_00000000_00000000,
                                                MASK_00000000_00000100_00000000_00000000,
                                                MASK_00000000_00000010_00000000_00000000,
                                                MASK_00000000_00000001_00000000_00000000,
                                                MASK_00000000_00000000_10000000_00000000,
                                                MASK_00000000_00000000_01000000_00000000,
                                                MASK_00000000_00000000_00100000_00000000,
                                                MASK_00000000_00000000_00010000_00000000,
                                                MASK_00000000_00000000_00001000_00000000,
                                                MASK_00000000_00000000_00000100_00000000,
                                                MASK_00000000_00000000_00000010_00000000,
                                                MASK_00000000_00000000_00000001_00000000,
                                                MASK_00000000_00000000_00000000_10000000,
                                                MASK_00000000_00000000_00000000_01000000,
                                                MASK_00000000_00000000_00000000_00100000,
                                                MASK_00000000_00000000_00000000_00010000,
                                                MASK_00000000_00000000_00000000_00001000,
                                                MASK_00000000_00000000_00000000_00000100,
                                                MASK_00000000_00000000_00000000_00000010,
                                                MASK_00000000_00000000_00000000_00000001,
        };


static void to_binary(const struct dc_posix_env *env, uint32_t val, bool *bits, const uint32_t *masks, size_t count)
{
    DC_TRACE(env);

    for(size_t i = 0; i < count; i++)
    {
        uint32_t mask;

        mask    = masks[i];
        bits[i] = get_bit32(env, val, mask);
    }
}

__attribute__ ((unused)) void dc_to_binary8(const struct dc_posix_env *env, uint8_t val, bool bits[static 8])
{
    DC_TRACE(env);
    to_binary(env, val, bits, masks_8, 8);
}

__attribute__ ((unused)) void dc_to_binary16(const struct dc_posix_env *env, uint16_t val, bool bits[static 16])
{
    DC_TRACE(env);
    to_binary(env, val, bits, masks_16, 16);
}

__attribute__ ((unused)) void dc_to_binary32(const struct dc_posix_env *env, uint32_t val, bool bits[static 32])
{
    DC_TRACE(env);
    to_binary(env, val, bits, masks_32, 32);
}

__attribute__ ((unused)) static void to_binary64(const struct dc_posix_env *env, uint64_t val, bool bits[static 64])
{
    DC_TRACE(env);

    for(int i = 0; i < 64; i++)
    {
        uint64_t mask;

        mask    = masks_64[i];
        bits[i] = get_bit64(env, val, mask);
    }
}

static bool get_bit32(const struct dc_posix_env *env, uint32_t val, uint32_t mask)
{
    uint32_t masked;
    bool bit;

    DC_TRACE(env);
    masked = val & mask;

    if(masked > 0)
    {
        bit = true;
    }
    else
    {
        bit = false;
    }

    return bit;
}

static bool get_bit64(const struct dc_posix_env *env, uint64_t val, uint64_t mask)
{
    uint64_t masked;
    bool bit;

    DC_TRACE(env);
    masked = val & mask;

    if(masked > 0)
    {
        bit = true;
    }
    else
    {
        bit = false;
    }

    return bit;
}


static void to_printable_binary(const struct dc_posix_env *env, size_t count, const bool bits[count], char printable[count + 1])
{
    DC_TRACE(env);

    for(size_t i = 0; i < count; i++)
    {
        char bit;

        if(bits[i])
        {
            bit = '1';
        }
        else
        {
            bit = '0';
        }

        printable[i] = bit;
    }

    printable[count] = '\0';
}

void dc_to_printable_binary8(const struct dc_posix_env *env, const bool bits[static 8], char printable[static 9])
{
    DC_TRACE(env);
    to_printable_binary(env, 8, bits, printable);
}

void dc_to_printable_binary16(const struct dc_posix_env *env, const bool bits[static 16], char printable[static 17])
{
    DC_TRACE(env);
    to_printable_binary(env, 16, bits, printable);
}

void dc_to_printable_binary32(const struct dc_posix_env *env, const bool bits[static 32], char printable[static 33])
{
    DC_TRACE(env);
    to_printable_binary(env, 32, bits, printable);
}

void dc_to_printable_binary64(const struct dc_posix_env *env, const bool bits[static 64], char printable[static 65])
{
    DC_TRACE(env);
    to_printable_binary(env, 32, bits, printable);
}

void dc_from_printable_binary8(const struct dc_posix_env *env, const char printable[9], bool bits[8])
{
    DC_TRACE(env);

    for(size_t i = 0; i < 8; i++)
    {
        set_bit(env, i, printable, bits);
    }
}

void dc_from_printable_binary16(const struct dc_posix_env *env, const char printable[static 17], bool bits[static 16])
{
    DC_TRACE(env);

    for(size_t i = 0; i < 16; i++)
    {
        set_bit(env, i, printable, bits);
    }
}

void dc_from_printable_binary32(const struct dc_posix_env *env, const char printable[static 33], bool bits[static 32])
{
    DC_TRACE(env);

    for(size_t i = 0; i < 32; i++)
    {
        set_bit(env, i, printable, bits);
    }
}

void dc_from_printable_binary64(const struct dc_posix_env *env, const char printable[static 65], bool bits[static 64])
{
    DC_TRACE(env);

    for(size_t i = 0; i < 64; i++)
    {
        set_bit(env, i, printable, bits);
    }
}

__attribute__ ((unused)) static void from_printable_binary(const struct dc_posix_env *env, size_t count, const char printable[static count + 1], bool bits[static count])
{
    DC_TRACE(env);

    for(size_t i = 0; i < count; i++)
    {
        set_bit(env, i, printable, bits);
    }
}

static void set_bit(const struct dc_posix_env *env, size_t position, const char printable[static position], bool bits[static position])
{
    char c;
    bool bit;

    DC_TRACE(env);
    c = printable[position];

    if(c == '0')
    {
        bit = false;
    }
    else
    {
        bit = true;
    }

    bits[position] = bit;
}

__attribute__ ((unused)) void dc_from_binary8(const struct dc_posix_env *env, const bool bits[static 8], uint8_t *val)
{
    uint8_t value;

    DC_TRACE(env);
    value = 0;

    for(size_t i = 0; i < 8; i++)
    {
        if(bits[i])
        {
            // https://stackoverflow.com/questions/47981/how-do-you-set-clear-and-toggle-a-single-bit
            value |= 1 << (7 - i);
        }
    }

    *val = value;
}

__attribute__ ((unused)) void dc_from_binary16(const struct dc_posix_env *env, const bool bits[static 16], uint16_t *val)
{
    uint8_t value;

    DC_TRACE(env);
    value = 0;

    for(size_t i = 0; i < 16; i++)
    {
        if(bits[i])
        {
            // https://stackoverflow.com/questions/47981/how-do-you-set-clear-and-toggle-a-single-bit
            value |= 1 << (15 - i);
        }
    }

    *val = value;
}


__attribute__ ((unused)) void dc_from_binary32(const struct dc_posix_env *env, const bool bits[static 32], uint32_t *val)
{
    uint32_t value;

    DC_TRACE(env);
    value = 0;

    for(size_t i = 0; i < 32; i++)
    {
        if(bits[i])
        {
            // https://stackoverflow.com/questions/47981/how-do-you-set-clear-and-toggle-a-single-bit
            value |= 1 << (31 - i);
        }
    }

    *val = value;
}

__attribute__ ((unused)) void dc_from_binary64(const struct dc_posix_env *env, const bool bits[static 64], uint64_t *val)
{
    uint64_t value;

    DC_TRACE(env);
    value = 0;

    for(size_t i = 0; i < 64; i++)
    {
        if(bits[i])
        {
            // https://stackoverflow.com/questions/47981/how-do-you-set-clear-and-toggle-a-single-bit
            value |= 1 << (63 - i);
        }
    }

    *val = value;
}
