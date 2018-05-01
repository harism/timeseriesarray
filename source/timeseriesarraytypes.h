// MIT License
//
// Copyright (c) 2018 Harri Smatt
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#ifndef TIME_SERIES_ARRAY_TYPES_H
#define TIME_SERIES_ARRAY_TYPES_H

namespace TimeSeries {

typedef signed long long time_s64;
typedef unsigned long long time_u64;
typedef unsigned int time_u32;

typedef unsigned long long value_u64;
typedef unsigned char value_u8;
typedef double value_double;

static_assert(sizeof(time_s64) == 8, "sizeof(time_s64) != 8");
static_assert(sizeof(time_u64) == 8, "sizeof(time_u64) != 8");
static_assert(sizeof(time_u32) == 4, "sizeof(time_u32) != 4");

static_assert(sizeof(value_u64) == 8, "sizeof(value_u64) != 8");
static_assert(sizeof(value_u8) == 1, "sizeof(value_u8) != 1");
static_assert(sizeof(value_double) == 8, "sizeof(value_double) != 8");

} // namespace TimeSeries

#endif // TIME_SERIES_ARRAY_TYPES_H
