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


#include "path.h"
#include <wordexp.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


void expand_path(char **expanded_path, const char *path)
{
    wordexp_t  expanded;
    char      *root;
    size_t     root_length;

    wordexp(path, &expanded, 0);
    root           = *expanded.we_wordv;
    root_length    = strlen(root);
    *expanded_path = malloc((root_length + 1) * sizeof(char));
    sprintf(*expanded_path, "%s", root);
    wordfree(&expanded);
}
