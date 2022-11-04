#ifndef LIBDC_UTIL_STREAMS_H
#define LIBDC_UTIL_STREAMS_H


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
#include <stdint.h>


#ifdef __cplusplus
extern "C" {
#endif


struct dc_stream_copy_info;


/**
 *
 * @param env
 * @param err
 * @param data
 * @param count
 * @param test
 */
void dc_stream_filter_uint8_t(const struct dc_env *env,
                              struct dc_error *err, uint8_t *data,
                              size_t *count,
                              bool (*test)(const struct dc_env *env,
                                           struct dc_error *err, uint8_t data));

/**
 *
 * @param env
 * @param err
 * @param data
 * @param count
 * @param position
 * @param apply
 * @param arg
 */
void dc_stream_uint8_t(const struct dc_env *env, struct dc_error *err,
                       const uint8_t *data, size_t count, size_t position,
                       void (*apply)(const struct dc_env *env,
                                     struct dc_error *err, const uint8_t *data,
                                     size_t line_count, size_t file_position,
                                     void *arg),
                       void *arg);

/**
 *
 * @param env
 * @param err
 * @param data
 * @param count
 * @param position
 * @param apply
 * @param arg
 */
void dc_stream_for_each_uint8_t(
    const struct dc_env *env, struct dc_error *err, const uint8_t *data,
    size_t count, size_t position,
    void (*apply)(const struct dc_env *env, struct dc_error *err,
                  uint8_t item, size_t line_position, size_t line_count,
                  size_t file_position, void *data),
    void *arg);


/**
 *
 * @param env
 * @param err
 * @param fd_in
 * @param fd_out
 * @param buffer_size
 * @param info
 * @return
 */
bool dc_stream_copy(const struct dc_env *env, struct dc_error *err,
                    int fd_in, int fd_out, size_t buffer_size,
                    struct dc_stream_copy_info *info);


/**
 *
 * @param env
 * @param err
 * @param filter
 * @param in_consumer
 * @param in_data
 * @param out_consumer
 * @param out_data
 * @return
 */
struct dc_stream_copy_info *dc_stream_copy_info_create(
    const struct dc_env *env, struct dc_error *err,
    bool (*filter)(const struct dc_env *env, struct dc_error *err,
                   uint8_t data),
    void (*in_consumer)(const struct dc_env *env, struct dc_error *err,
                        const uint8_t *data, size_t line_count,
                        size_t file_position, void *arg),
    void *in_data,
    void (*out_consumer)(const struct dc_env *env, struct dc_error *err,
                         const uint8_t *data, size_t line_count,
                         size_t file_position, void *arg),
    void *out_data);

/**
 *
 * @param env
 * @param info
 */
void dc_stream_copy_info_destroy(const struct dc_env *env,
                                 struct dc_stream_copy_info **info);


#ifdef __cplusplus
}
#endif


#endif // LIBDC_UTIL_STREAMS_H
