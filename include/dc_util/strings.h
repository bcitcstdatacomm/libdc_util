#ifndef LIBDC_UTIL_STRINGS_H
#define LIBDC_UTIL_STRINGS_H


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


#include <dc_env/env.h>
#include <sys/types.h>


#ifdef __cplusplus
extern "C" {
#endif


/**
 *
 * @param env
 * @param str
 * @return
 */
char *dc_str_left_trim(const struct dc_env *env, char *str);

/**
 *
 * @param env
 * @param str
 * @return
 */
char *dc_str_right_trim(const struct dc_env *env, char *str);

/**
 *
 * @param env
 * @param str
 * @return
 */
char *dc_str_trim(const struct dc_env *env, char *str);

/**
 *
 * @param env
 * @param str
 * @param c
 * @return
 */
ssize_t dc_str_find_last(const struct dc_env *env, const char *str,
                         int c);

/**
 *
 * @param env
 * @param err
 * @param n
 * @param ...
 * @return
 */
char **dc_strs_to_array(const struct dc_env *env, struct dc_error *err,
                        size_t n, ...);

/**
 *
 * @param env
 * @param n
 * @param parray
 */
void dc_strs_destroy_array(const struct dc_env *env, size_t n,
                           char **parray);

/**
 *
 * @param env
 * @param str
 * @param c
 * @return
 */
size_t dc_str_find_all(const struct dc_env *env, const char *str, int c);


#ifdef __cplusplus
}
#endif


#endif // LIBDC_UTIL_STRINGS_H
