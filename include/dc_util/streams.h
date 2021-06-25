#ifndef UNTITLED5_DC_STREAMS_H
#define UNTITLED5_DC_STREAMS_H


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


#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>


struct dc_stream_copy_info;

typedef bool (*uint8_t_filter_func)(uint8_t data);
typedef void (*uint8_t_consumer_func)(uint8_t item, size_t line_position, size_t line_count, size_t file_position, void *data);

void dc_stream_filter_uint8_t(uint8_t *data, size_t *count, uint8_t_filter_func test);
void dc_stream_for_each_uint8_t(const uint8_t *data, size_t count, size_t position, uint8_t_consumer_func apply, void *arg);
void dc_stream_copy(int fd_in, int fd_out, size_t buffer_size, struct dc_stream_copy_info *info);
struct dc_stream_copy_info *dc_stream_copy_info_create(uint8_t_filter_func filter, uint8_t_consumer_func in_consumer, void *in_data, uint8_t_consumer_func out_consumer, void *out_data);
void dc_stream_copy_info_destroy(struct dc_stream_copy_info **info);


#endif

