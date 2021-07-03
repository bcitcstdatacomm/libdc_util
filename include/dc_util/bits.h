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


#include <dc_posix/posix_env.h>
#include <stdint.h>
#include <stdbool.h>


/**
 *
 * @param env
 * @param byte
 * @param bits
 */
void dc_to_binary8(const struct dc_posix_env *env, uint8_t byte, bool bits[static 8]);

/**
 *
 * @param env
 * @param byte
 * @param bits
 */
void dc_to_binary16(const struct dc_posix_env *env, uint16_t byte, bool bits[static 16]);

/**
 *
 * @param env
 * @param byte
 * @param bits
 */
void dc_to_binary32(const struct dc_posix_env *env, uint32_t byte, bool bits[static 32]);

/**
 *
 * @param env
 * @param byte
 * @param bits
 */
void dc_to_binary64(const struct dc_posix_env *env, uint64_t byte, bool bits[static 64]);

/**
 *
 * @param env
 * @param bits
 * @param val
 */
void dc_from_binary8(const struct dc_posix_env *env, const bool bits[static 8], uint8_t *val);

/**
 *
 * @param env
 * @param bits
 * @param val
 */
void dc_from_binary16(const struct dc_posix_env *env, const bool bits[static 16], uint16_t *val);

/**
 *
 * @param env
 * @param bits
 * @param val
 */
void dc_from_binary32(const struct dc_posix_env *env, const bool bits[static 32], uint32_t *val);

/**
 *
 * @param env
 * @param bits
 * @param val
 */
void dc_from_binary64(const struct dc_posix_env *env, const bool bits[static 64], uint64_t *val);

/**
 *
 * @param env
 * @param bits
 * @param printable
 */
void dc_to_printable_binary8(const struct dc_posix_env *env, const bool bits[static 8], char printable[static 9]);

/**
 *
 * @param env
 * @param bits
 * @param printable
 */
void dc_to_printable_binary16(const struct dc_posix_env *env, const bool bits[static 16], char printable[static 17]);

/**
 *
 * @param env
 * @param bits
 * @param printable
 */
void dc_to_printable_binary32(const struct dc_posix_env *env, const bool bits[static 32], char printable[static 33]);

/**
 *
 * @param env
 * @param bits
 * @param printable
 */
void dc_to_printable_binary64(const struct dc_posix_env *env, const bool bits[static 64], char printable[static 65]);

/**
 *
 * @param env
 * @param printable
 * @param bits
 */
void dc_from_printable_binary8(const struct dc_posix_env *env, const char printable[static 9], bool bits[static 8]);

/**
 *
 * @param env
 * @param printable
 * @param bits
 */
void dc_from_printable_binary16(const struct dc_posix_env *env, const char printable[static 17], bool bits[static 16]);

/**
 *
 * @param env
 * @param printable
 * @param bits
 */
void dc_from_printable_binary32(const struct dc_posix_env *env, const char printable[static 33], bool bits[static 32]);

/**
 *
 * @param env
 * @param printable
 * @param bits
 */
void dc_from_printable_binary64(const struct dc_posix_env *env, const char printable[static 65], bool bits[static 64]);


#endif // LIBDC_UTIL_BITS_H
