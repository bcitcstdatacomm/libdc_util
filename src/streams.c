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

#include "streams.h"
#include <dc_posix/dc_stdlib.h>
#include <dc_posix/dc_string.h>
#include <dc_posix/dc_unistd.h>

struct dc_stream_copy_info
{
    size_t in_position;
    size_t out_position;

    bool (*filter)(const struct dc_posix_env *env, struct dc_error *err, uint8_t data);

    void (*in_consumer)(const struct dc_posix_env *env,
                        struct dc_error           *err,
                        const uint8_t             *data,
                        size_t                     line_count,
                        size_t                     file_position,
                        void                      *arg);

    void *in_data;

    void (*out_consumer)(const struct dc_posix_env *env,
                         struct dc_error           *err,
                         const uint8_t             *data,
                         size_t                     line_count,
                         size_t                     file_position,
                         void                      *arg);

    void *out_data;
};

void dc_stream_filter_uint8_t(const struct dc_posix_env *env,
                              struct dc_error           *err,
                              uint8_t                   *data,
                              size_t                    *count,
                              bool (*test)(const struct dc_posix_env *env, struct dc_error *err, uint8_t data))
{
    DC_TRACE(env);

    if(test)
    {
        size_t new_count;

        new_count = 0;

        for(size_t i = 0; i < *count; i++)
        {
            // TODO: what to do if there is an error?
            if(test(env, err, data[i]))
            {
                data[new_count] = data[i];
                new_count++;
            }
        }

        *count = new_count;
    }
}

void dc_stream_uint8_t(const struct dc_posix_env *env,
                       struct dc_error           *err,
                       const uint8_t             *data,
                       size_t                     count,
                       size_t                     position,
                       void (*apply)(const struct dc_posix_env *env,
                                     struct dc_error           *err,
                                     const uint8_t             *data,
                                     size_t                     line_count,
                                     size_t                     file_position,
                                     void                      *arg),
                       void *arg)
{
    DC_TRACE(env);

    if(apply)
    {
        // TODO: what to do if there is an error?
        apply(env, err, data, count, position, arg);
    }
}

void dc_stream_for_each_uint8_t(const struct dc_posix_env *env,
                                struct dc_error           *err,
                                const uint8_t             *data,
                                size_t                     count,
                                size_t                     position,
                                void (*apply)(const struct dc_posix_env *env,
                                              struct dc_error           *err,
                                              uint8_t                    item,
                                              size_t                     line_position,
                                              size_t                     line_count,
                                              size_t                     file_position,
                                              void                      *data),
                                void *arg)
{
    DC_TRACE(env);

    if(apply)
    {
        for(size_t line_position = 0; line_position < count; line_position++)
        {
            // TODO: what to do if there is an error?
            apply(env, err, data[line_position], line_position, count, position, arg);
            position++;
        }
    }
}

struct dc_stream_copy_info *
dc_stream_copy_info_create(const struct dc_posix_env *env,
                           struct dc_error           *err,
                           bool (*filter)(const struct dc_posix_env *env, struct dc_error *err, uint8_t data),
                           void (*in_consumer)(const struct dc_posix_env *env,
                                               struct dc_error           *err,
                                               const uint8_t             *data,
                                               size_t                     line_count,
                                               size_t                     file_position,
                                               void                      *arg),
                           void *in_data,
                           void (*out_consumer)(const struct dc_posix_env *env,
                                                struct dc_error           *err,
                                                const uint8_t             *data,
                                                size_t                     line_count,
                                                size_t                     file_position,
                                                void                      *arg),
                           void *out_data)
{
    struct dc_stream_copy_info *info;

    DC_TRACE(env);
    info = dc_malloc(env, err, sizeof(struct dc_stream_copy_info));

    if(dc_error_has_no_error(err))
    {
        info->in_position  = 0;
        info->out_position = 0;
        info->filter       = filter;
        info->in_consumer  = in_consumer;
        info->in_data      = in_data;
        info->out_consumer = out_consumer;
        info->out_data     = out_data;
    }

    return info;
}

void dc_stream_copy_info_destroy(const struct dc_posix_env *env, struct dc_stream_copy_info **pinfo)
{
    DC_TRACE(env);

    dc_free(env, *pinfo, sizeof(struct dc_stream_copy_info));
    *pinfo = NULL;
}

bool dc_stream_copy(const struct dc_posix_env  *env,
                    struct dc_error            *err,
                    int                         fd_in,
                    int                         fd_out,
                    size_t                      buffer_size,
                    struct dc_stream_copy_info *info)
{
    uint8_t *buffer;
    bool     ret_val;

    DC_TRACE(env);
    buffer = dc_malloc(env, err, buffer_size);

    if(dc_error_has_no_error(err))
    {
        ssize_t read_len;

        // TODO: what about all the possible errors?
        while((read_len = dc_read(env, err, fd_in, buffer, buffer_size)) > 0)
        {
            size_t len;

            len = (size_t)read_len;

            // observe what was read in
            dc_stream_uint8_t(env, err, buffer, len, info->in_position, info->in_consumer, info->in_data);
            info->in_position += (size_t)read_len;

            // filter, could change the size
            dc_stream_filter_uint8_t(env, err, buffer, &len, info->filter);

            // observe what will be written
            dc_stream_uint8_t(env, err, buffer, len, info->out_position, info->out_consumer, info->out_data);
            info->out_position += len;

            // write to the destination
            dc_write(env, err, fd_out, buffer, len);

            if(dc_error_has_error(err))
            {
                break;
            }
        }

        dc_free(env, buffer, buffer_size);
    }

    ret_val = dc_error_has_no_error(err);

    return ret_val;
}
