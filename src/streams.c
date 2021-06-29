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
#include <dc_posix/stdlib.h>
#include <dc_posix/unistd.h>


void dc_stream_filter_uint8_t(uint8_t *data, size_t *count, uint8_t_filter_func test)
{
    if(test)
    {
        size_t new_count;

        new_count = 0;

        for(size_t i = 0; i < *count; i++)
        {
            if(test(data[i]))
            {
                data[new_count] = data[i];
                new_count++;
            }
        }

        *count = new_count;
    }
}


void dc_stream_for_each_uint8_t(const uint8_t *data, size_t count, size_t position, uint8_t_consumer_func apply, void *arg)
{
    if(apply)
    {
        for(size_t line_position = 0; line_position < count; line_position++)
        {
            apply(data[line_position], line_position, count, position, arg);
            position++;
        }
    }
}

struct dc_stream_copy_info
{
    size_t in_position;
    size_t out_position;
    uint8_t_filter_func filter;
    uint8_t_consumer_func in_consumer;
    void *in_data;
    uint8_t_consumer_func out_consumer;
    void *out_data;
};

struct dc_stream_copy_info *dc_stream_copy_info_create(const struct dc_posix_env *env, uint8_t_filter_func filter, uint8_t_consumer_func in_consumer, void *in_data, uint8_t_consumer_func out_consumer, void *out_data)
{
    struct dc_stream_copy_info *info;
    int err;

    info = dc_malloc(env, &err, sizeof(struct dc_stream_copy_info));

    if(info == NULL)
    {
    }

    info->in_position  = 0;
    info->out_position = 0;
    info->filter       = filter;
    info->in_consumer  = in_consumer;
    info->in_data      = in_data;
    info->out_consumer = out_consumer;
    info->out_data     = out_data;

    return info;
}

void dc_stream_copy_info_destroy(const struct dc_posix_env *env, struct dc_stream_copy_info **info)
{
    dc_free(env, *info);
    *info = NULL;
}

void dc_stream_copy(const struct dc_posix_env *env, int fd_in, int fd_out, size_t buffer_size, struct dc_stream_copy_info *info)
{
    uint8_t *buffer;
    ssize_t  len;
    int      err;

    buffer = dc_malloc(env, &err, buffer_size);

    while((len = dc_read(env, &err, fd_in, buffer, buffer_size)) > 0)
    {
        // TODO: what if len is -1?

        size_t len2;

        len2 = (size_t)len;
        dc_stream_for_each_uint8_t(buffer, len2, info->in_position, info->in_consumer, info->in_data);
        info->in_position += (size_t)len;
        dc_stream_filter_uint8_t(buffer, &len2, info->filter);
        dc_stream_for_each_uint8_t(buffer, len2, info->out_position, info->out_consumer, info->out_data);
        info->out_position += len2;
        write(fd_out, buffer, len2);
    }

    dc_free(env, buffer);
}


