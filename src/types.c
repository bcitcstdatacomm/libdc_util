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


#include <math.h>
#include "types.h"


__attribute__ ((unused)) off_t dc_max_off_t(void)
{
    static size_t bits = sizeof(off_t) * 8;
    long double   largest_signed;
    long double   largest_unsigned;
    off_t         max;

    largest_signed   = powl(2, bits);
    largest_unsigned = largest_signed / 2;
    max              = (off_t)(largest_unsigned - 1);

    return max;
}


