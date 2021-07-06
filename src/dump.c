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


#include "dump.h"
#include "bits.h"
#include <dc_posix/string.h>
#include <dc_posix/unistd.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdio.h>
#include <math.h>


static const char *lookup_control(const struct dc_posix_env *env, uint8_t c);


#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"
struct dc_dump_info
{
    int dump_fd;
    size_t max_position;
    size_t line_number;
    size_t line_position;
    size_t line_format_size;
    size_t line_buffer_size;
    char *line_format;
    char *line_buffer;
};
#pragma GCC diagnostic pop



struct dc_dump_info *dc_dump_info_create(const struct dc_posix_env *env, struct dc_error *err, int fd, off_t file_size)
{
    struct dc_dump_info *info;

    DC_TRACE(env);
    info = dc_calloc(env, err, 1, sizeof(struct dc_dump_info));

    if(DC_HAS_NO_ERROR(err))
    {
        const char *format;

        info->dump_fd       = fd;
        info->line_number   = 1;
        info->line_position = 1;

        // TODO - breaks for 999999999999999997 - 999999999999999999
        info->max_position = (size_t)(log10l(file_size) + 1.0l);

        // NOTE: this should be controlled by a parameter in the future
        // file pos line # line pos : binary : octal : decimal : hex : ascii or
        // max_digits max_digits max_digits : 11111111 : 0377 : 255 : 0xFF : ????
        format = "%*d %*d %*d : %08s : 0%03o : %03d : 0x%02X : %-4s";
        info->line_format_size = dc_strlen(env, format) + 1;
        info->line_format = dc_malloc(env, err, info->line_format_size);

        if(DC_HAS_ERROR(err))
        {
            dc_free(env, info, sizeof(struct dc_dump_info));
            info = NULL;
        }
        else
        {
            dc_strcpy(env, info->line_format, format);

            // 3 * "%*d " where * is info->max_position
            // ": 11111111 " for binary (11)
            // ": 0### " for octal (7)
            // ": ### " for decimal (6)
            // ": 0x### " for hex (8)
            // ": ????" for the ASCII value (6)
            // '\0' + 1
            info->line_buffer_size = (3 * (info->max_position + 1)) + 11 + 7 + 6 + 8 + 6 + 1;
            info->line_buffer      = dc_malloc(env, err, info->line_buffer_size);

            if(DC_HAS_ERROR(err))
            {
                dc_free(env, info->line_format, info->line_format_size);
                dc_free(env, info, sizeof(struct dc_dump_info));
                info = NULL;
            }
        }
    }

    return info;
}

void dc_dump_info_destroy(const struct dc_posix_env *env, struct dc_dump_info **pinfo)
{
    struct dc_dump_info *info;

    DC_TRACE(env);
    info = *pinfo;
    dc_free(env, info->line_format, info->line_format_size);
    dc_free(env, info->line_buffer, info->line_buffer_size);
    dc_free(env, info, sizeof(struct dc_dump_info));

    if(env->null_free)
    {
        *pinfo = NULL;
    }
}

void dc_dump_dumper(const struct dc_posix_env      *env,
                    struct dc_error                *err,
                    const uint8_t                  *data,
                    size_t                          count,
                    size_t                          file_position,
                    void                           *arg)
{
    struct dc_dump_info *info;

    DC_TRACE(env);
    info = arg;

    for(size_t i = 0; i < count; i++)
    {
        uint8_t item;
        bool    bits[8];
        char    binary[9];
        char    printable[5];

        item = data[i];
        dc_to_binary8(env, item, bits);
        dc_to_printable_binary8(env, bits, binary);

        if(isprint(item))
        {
            printable[0] = (char)item;
            printable[1] = '\0';
        }
        else if(iscntrl(item))
        {
            const char *temp;

            temp = lookup_control(env, item);
            dc_strcpy(env, printable, temp);
        }
        else
        {
            printable[0] = '\0';
        }

    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wformat-nonliteral"
        sprintf(info->line_buffer, info->line_format,
                info->max_position, file_position,
                info->max_position, info->line_number,
                info->max_position, info->line_position,
                binary, item, item, item, printable);
    #pragma GCC diagnostic pop
        dc_write(env, err, info->dump_fd, info->line_buffer, dc_strlen(env, info->line_buffer));

        if(DC_HAS_NO_ERROR(err))
        {
            file_position++;
            dc_write(env, err, info->dump_fd, "\n", 1);

            if(DC_HAS_NO_ERROR(err))
            {
                if(item == '\n')
                {
                    info->line_number++;
                    info->line_position = 1;
                }
                else
                {
                    info->line_position++;
                }
            }
        }
    }
}


static const char *lookup_control(const struct dc_posix_env *env, uint8_t c)
{
    // https://en.wikipedia.org/wiki/List_of_Unicode_characters#Control_codes
    static const char *LOW_VALUES[] =
            {
                    "NUL",  // 0
                    "SOH",  // 1
                    "STX",  // 2
                    "ETX",  // 3
                    "EOT",  // 4
                    "ENQ",  // 5
                    "ACK",  // 6
                    "BEL",  // 7
                    "BS",   // 8
                    "\\t",  // 9
                    "\\n",  // 10
                    "VT",   // 11
                    "FF",   // 12
                    "\\r",  // 13
                    "SO",   // 14
                    "SI",   // 15
                    "DLE",  // 16
                    "DC1",  // 17
                    "DC2",  // 18
                    "DC3",  // 19
                    "DC4",  // 20
                    "NAK",  // 21
                    "SYN",  // 22
                    "ETB",  // 23
                    "CAN",  // 24
                    "EM",   // 25
                    "SUB",  // 26
                    "ESC",  // 27
                    "FS",   // 28
                    "GS",   // 29
                    "RS",   // 30
                    "US",   // 31
            };
    static const char *HIGH_VALUES[] =
            {
                    "DEL",  // 127
                    "PAD",  // 128
                    "HOP",  // 129
                    "BPH",  // 130
                    "NBH",  // 131
                    "IND",  // 132
                    "NEL",  // 133
                    "SSA",  // 134
                    "ESA",  // 135
                    "HTS",  // 136
                    "HTJ",  // 137
                    "VTS",  // 138
                    "PLD",  // 139
                    "PLU",  // 140
                    "RI",   // 141
                    "SS2",  // 142
                    "SS3",  // 143
                    "DCS",  // 144
                    "PU1",  // 145
                    "PU2",  // 146
                    "STS",  // 147
                    "CCH",  // 148
                    "MW",   // 159
                    "SPA",  // 150
                    "EPA",  // 151
                    "SOS",  // 152
                    "SGCI", // 153
                    "SCI",  // 154
                    "CSI",  // 155
                    "ST",   // 156
                    "OCS",  // 157
                    "PM",   // 158
                    "APC",  // 159
            };
    const char *value;

    DC_TRACE(env);

    if(c <= 31)
    {
        value = LOW_VALUES[c];
    }
    else if(c >= 127 && c <= 159)
    {
        value = HIGH_VALUES[c - 127];
    }
    else
    {
        value = "????";
    }

    return value;
}
