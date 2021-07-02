#ifndef LIBDC_UTIL_TYPES_H
#define LIBDC_UTIL_TYPES_H


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
#include <sys/types.h>
#include <stdint.h>


/**
 *
 * @return
 */
off_t dc_max_off_t(void);
uint16_t dc_uint16_from_str(const struct dc_posix_env *env, int *err, const char *str, int base);


#endif //LIBDC_UTIL_TYPES_H
