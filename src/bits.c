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


static void    to_printable_binary(const struct dc_env *env,
                                   size_t                     count,
                                   bool const                 bits[static count],
                                   char                       printable[static count + 1]);

static void    set_bit(const struct dc_env *env,
                       size_t                     position,
                       char const                 printable[static position],
                       bool                       bits[static position]);

const uint8_t  MASK_00000001                            = UINT8_C(0x00000001);
const uint8_t  MASK_00000010                            = UINT8_C(0x00000002);
const uint8_t  MASK_00000100                            = UINT8_C(0x00000004);
const uint8_t  MASK_00001000                            = UINT8_C(0x00000008);
const uint8_t  MASK_00010000                            = UINT8_C(0x00000010);
const uint8_t  MASK_00100000                            = UINT8_C(0x00000020);
const uint8_t  MASK_01000000                            = UINT8_C(0x00000040);
const uint8_t  MASK_10000000                            = UINT8_C(0x00000080);
const uint16_t MASK_00000001_00000000                   = UINT16_C(0x00000100);
const uint16_t MASK_00000010_00000000                   = UINT16_C(0x00000200);
const uint16_t MASK_00000100_00000000                   = UINT16_C(0x00000400);
const uint16_t MASK_00001000_00000000                   = UINT16_C(0x00000800);
const uint16_t MASK_00010000_00000000                   = UINT16_C(0x00001000);
const uint16_t MASK_00100000_00000000                   = UINT16_C(0x00002000);
const uint16_t MASK_01000000_00000000                   = UINT16_C(0x00004000);
const uint16_t MASK_10000000_00000000                   = UINT16_C(0x00008000);
const uint32_t MASK_00000000_00000001_00000000_00000000 = UINT32_C(0x00010000);
const uint32_t MASK_00000000_00000010_00000000_00000000 = UINT32_C(0x00020000);
const uint32_t MASK_00000000_00000100_00000000_00000000 = UINT32_C(0x00040000);
const uint32_t MASK_00000000_00001000_00000000_00000000 = UINT32_C(0x00080000);
const uint32_t MASK_00000000_00010000_00000000_00000000 = UINT32_C(0x00100000);
const uint32_t MASK_00000000_00100000_00000000_00000000 = UINT32_C(0x00200000);
const uint32_t MASK_00000000_01000000_00000000_00000000 = UINT32_C(0x00400000);
const uint32_t MASK_00000000_10000000_00000000_00000000 = UINT32_C(0x00800000);
const uint32_t MASK_00000001_00000000_00000000_00000000 = UINT32_C(0x01000000);
const uint32_t MASK_00000010_00000000_00000000_00000000 = UINT32_C(0x02000000);
const uint32_t MASK_00000100_00000000_00000000_00000000 = UINT32_C(0x04000000);
const uint32_t MASK_00001000_00000000_00000000_00000000 = UINT32_C(0x08000000);
const uint32_t MASK_00010000_00000000_00000000_00000000 = UINT32_C(0x10000000);
const uint32_t MASK_00100000_00000000_00000000_00000000 = UINT32_C(0x20000000);
const uint32_t MASK_01000000_00000000_00000000_00000000 = UINT32_C(0x40000000);
const uint32_t MASK_10000000_00000000_00000000_00000000 = UINT32_C(0x80000000);

static const uint8_t masks_8[] = {
    MASK_10000000,
    MASK_01000000,
    MASK_00100000,
    MASK_00010000,
    MASK_00001000,
    MASK_00000100,
    MASK_00000010,
    MASK_00000001,
};

static const uint16_t masks_16[] = {
    MASK_10000000_00000000,
    MASK_01000000_00000000,
    MASK_00100000_00000000,
    MASK_00010000_00000000,
    MASK_00001000_00000000,
    MASK_00000100_00000000,
    MASK_00000010_00000000,
    MASK_00000001_00000000,
    MASK_10000000,
    MASK_01000000,
    MASK_00100000,
    MASK_00010000,
    MASK_00001000,
    MASK_00000100,
    MASK_00000010,
    MASK_00000001,
};

static const uint32_t masks_32[] = {
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
    MASK_10000000_00000000,
    MASK_01000000_00000000,
    MASK_00100000_00000000,
    MASK_00010000_00000000,
    MASK_00001000_00000000,
    MASK_00000100_00000000,
    MASK_00000010_00000000,
    MASK_00000001_00000000,
    MASK_10000000,
    MASK_01000000,
    MASK_00100000,
    MASK_00010000,
    MASK_00001000,
    MASK_00000100,
    MASK_00000010,
    MASK_00000001,
};

void dc_to_binary8(const struct dc_env *env, uint8_t byte, bool bits[static DC_8_BITS])
{
    DC_TRACE(env);

    for(size_t i = 0; i < DC_8_BITS; i++)
    {
        uint8_t mask;
        uint8_t masked;
        bool    bit;

        mask    = masks_8[i];
        masked  = byte & mask;
        bit     = masked > 0;
        bits[i] = bit;
    }
}

void dc_to_binary16(const struct dc_env *env, uint16_t bytes, bool bits[static DC_16_BITS])
{
    DC_TRACE(env);

    for(size_t i = 0; i < DC_16_BITS; i++)
    {
        uint16_t mask;
        uint16_t masked;
        bool     bit;

        mask    = masks_16[i];
        masked  = bytes & mask;
        bit     = masked > 0;
        bits[i] = bit;
    }
}

void dc_to_binary32(const struct dc_env *env, uint32_t bytes, bool bits[static DC_32_BITS])
{
    DC_TRACE(env);

    for(size_t i = 0; i < DC_32_BITS; i++)
    {
        uint32_t mask;
        uint32_t masked;
        bool     bit;

        mask    = masks_32[i];
        masked  = bytes & mask;
        bit     = masked > 0;
        bits[i] = bit;
    }
}

static void
to_printable_binary(const struct dc_env *env, size_t count, bool const bits[count], char printable[count + 1])
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

void dc_to_printable_binary8(const struct dc_env *env, bool const bits[static DC_8_BITS], char printable[static DC_8_BITS + 1])
{
    DC_TRACE(env);
    to_printable_binary(env, DC_8_BITS, bits, printable);
}

void dc_to_printable_binary16(const struct dc_env *env, bool const bits[static DC_16_BITS], char printable[static DC_16_BITS + 1])
{
    DC_TRACE(env);
    to_printable_binary(env, DC_16_BITS, bits, printable);
}

void dc_to_printable_binary32(const struct dc_env *env, bool const bits[static DC_32_BITS], char printable[static DC_32_BITS + 1])
{
    DC_TRACE(env);
    to_printable_binary(env, DC_32_BITS, bits, printable);
}

void dc_from_printable_binary8(const struct dc_env *env, char const printable[DC_8_BITS + 1], bool bits[DC_8_BITS])
{
    DC_TRACE(env);

    for(size_t i = 0; i < DC_8_BITS; i++)
    {
        set_bit(env, i, printable, bits);
    }
}

void dc_from_printable_binary16(const struct dc_env *env, char const printable[static DC_16_BITS + 1], bool bits[static DC_16_BITS])
{
    DC_TRACE(env);

    for(size_t i = 0; i < DC_16_BITS; i++)
    {
        set_bit(env, i, printable, bits);
    }
}

void dc_from_printable_binary32(const struct dc_env *env, char const printable[static DC_32_BITS + 1], bool bits[static DC_32_BITS])
{
    DC_TRACE(env);

    for(size_t i = 0; i < DC_32_BITS; i++)
    {
        set_bit(env, i, printable, bits);
    }
}

static void set_bit(const struct dc_env *env,
                    size_t                     position,
                    char const                 printable[static position],
                    bool                       bits[static position])
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

void dc_from_binary8(const struct dc_env *env, bool const bits[static DC_8_BITS], uint8_t *val)
{
    uint32_t value;

    DC_TRACE(env);
    value = 0;

    for(uint32_t i = 0; i < DC_8_BITS; i++)
    {
        if(bits[i])
        {
            // https://stackoverflow.com/questions/47981/how-do-you-set-clear-and-toggle-a-single-bit
            value |= 1U << (7U - i);
        }
    }

    *val = (uint8_t)value;
}

void dc_from_binary16(const struct dc_env *env, bool const bits[static DC_16_BITS], uint16_t *val)
{
    uint32_t value;

    DC_TRACE(env);
    value = 0;

    for(uint32_t i = 0; i < DC_16_BITS; i++)
    {
        if(bits[i])
        {
            // https://stackoverflow.com/questions/47981/how-do-you-set-clear-and-toggle-a-single-bit
            value |= 1U << (15U - i);
        }
    }

    *val = (uint16_t)value;
}

void dc_from_binary32(const struct dc_env *env, bool const bits[static DC_32_BITS], uint32_t *val)
{
    uint32_t value;

    DC_TRACE(env);
    value = 0;

    for(uint32_t i = 0; i < DC_32_BITS; i++)
    {
        if(bits[i])
        {
            // https://stackoverflow.com/questions/47981/how-do-you-set-clear-and-toggle-a-single-bit
            value |= 1U << (31U - i);
        }
    }

    *val = value;
}
