#ifndef LIBDC_UTIL_PATH_H
#define LIBDC_UTIL_PATH_H


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


/**
 *
 * @param env
 * @param expanded_path
 * @param path
 */
void expand_path(const struct dc_posix_env *env, struct dc_error *err, char **expanded_path, const char *path);


#endif // LIBDC_UTIL_PATH_H
