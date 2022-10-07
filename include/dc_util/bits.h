#ifndef LIBDC_UTIL_BITS_H
#define LIBDC_UTIL_BITS_H

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

#include <dc_posix/dc_posix_env.h>
#include <stdbool.h>
#include <stdint.h>


#define DC_8_BITS 8
#define DC_16_BITS 16
#define DC_32_BITS 32


/**
 *
 * @param env
 * @param byte
 * @param bits
 */
void dc_to_binary8(const struct dc_posix_env *env, uint8_t byte,
                   bool bits[static DC_8_BITS]);

/**
 *
 * @param env
 * @param bytes
 * @param bits
 */
void dc_to_binary16(const struct dc_posix_env *env, uint16_t bytes,
                    bool bits[static DC_16_BITS]);


/**
 *
 * @param env
 * @param bytes
 * @param bits
 */
void dc_to_binary32(const struct dc_posix_env *env, uint32_t bytes,
                    bool bits[static DC_32_BITS]);


/**
 *
 * @param env
 * @param bits
 * @param val
 */
void dc_from_binary8(const struct dc_posix_env *env, const bool bits[static DC_8_BITS],
                     uint8_t *val);

/**
 *
 * @param env
 * @param bits
 * @param val
 */
void dc_from_binary16(const struct dc_posix_env *env,
                      const bool bits[static DC_16_BITS], uint16_t *val);

/**
 *
 * @param env
 * @param bits
 * @param val
 */
void dc_from_binary32(const struct dc_posix_env *env,
                      const bool bits[static DC_32_BITS], uint32_t *val);


/**
 *
 * @param env
 * @param bits
 * @param printable
 */
void dc_to_printable_binary8(const struct dc_posix_env *env,
                             const bool bits[static DC_8_BITS],
                             char printable[static DC_8_BITS + 1]);

/**
 *
 * @param env
 * @param bits
 * @param printable
 */
void dc_to_printable_binary16(const struct dc_posix_env *env,
                              const bool bits[static DC_16_BITS],
                              char printable[static DC_16_BITS + 1]);

/**
 *
 * @param env
 * @param bits
 * @param printable
 */
void dc_to_printable_binary32(const struct dc_posix_env *env,
                              const bool bits[static DC_32_BITS],
                              char printable[static DC_32_BITS + 1]);


/**
 *
 * @param env
 * @param printable
 * @param bits
 */
void dc_from_printable_binary8(const struct dc_posix_env *env,
                               const char printable[static DC_8_BITS + 1],
                               bool bits[static DC_8_BITS]);

/**
 *
 * @param env
 * @param printable
 * @param bits
 */
void dc_from_printable_binary16(const struct dc_posix_env *env,
                                const char printable[static DC_16_BITS + 1],
                                bool bits[static DC_16_BITS]);

/**
 *
 * @param env
 * @param printable
 * @param bits
 */
void dc_from_printable_binary32(const struct dc_posix_env *env,
                                const char printable[static DC_32_BITS + 1],
                                bool bits[static DC_32_BITS]);


#endif // LIBDC_UTIL_BITS_H
