# Description

TimeSeriesArray is simple in-memory storage model for time series data consisting of 64bit unsigned
integer timestamp and corresponding 64bit double precision value. The compression itself is secondary
target for this implementation and main goal is to provide some compression without losing much of read
accessing speed. That is this implementation is targeted at efficient realtime rendering of stored
data instead. This comes with assumption compression provides some benefits over raw data storing
at this precision.

Same applies to writing speed, too, which is not paid similarly much attention to optimize as reading
speed.

### Current status

The implementation reaches roughly following speeds on MacBook Pro 2013 model:
- Write speed roughly 800-950MB/second
- Read speed roughly 2-2.5GB/second

These are measured with the included test application.

The application and time series array is written with Clang and XCode but should be relatively
easy to port for other platforms, too.

### Example output

The test application outputs something similar to following after it compiles and executes successfully.

```
Generate test data : 55556666 x (time_64bits + double_64bits) = 847.727MB

Execute tests..

Data type : DOUBLE
Time write : 0.944601s   Speed : 897.445MB/s
Time read  : 0.299999s   Speed : 2825.77MB/s
Time read_ : 0.312636s   Speed : 2711.55MB/s
Compressed size: 62.5212% of original data

Data type : FLOAT
Time write : 0.801985s   Speed : 1057.04MB/s
Time read  : 0.302999s   Speed : 2797.79MB/s
Time read_ : 0.374636s   Speed : 2262.8MB/s
Compressed size: 43.0132% of original data

Data type : S64
Time write : 0.897102s   Speed : 944.962MB/s
Time read  : 0.320827s   Speed : 2642.32MB/s
Time read_ : 0.36293s   Speed : 2335.79MB/s
Compressed size: 56.2299% of original data

Data type : U64
Time write : 0.985574s   Speed : 860.136MB/s
Time read  : 0.384763s   Speed : 2203.25MB/s
Time read_ : 0.422893s   Speed : 2004.59MB/s
Compressed size: 56.2299% of original data

Data type : S32
Time write : 0.841339s   Speed : 1007.59MB/s
Time read  : 0.324235s   Speed : 2614.54MB/s
Time read_ : 0.35266s   Speed : 2403.81MB/s
Compressed size: 47.8906% of original data

Data type : U32
Time write : 0.899349s   Speed : 942.601MB/s
Time read  : 0.392436s   Speed : 2160.17MB/s
Time read_ : 0.422737s   Speed : 2005.33MB/s
Compressed size: 48.9222% of original data

Data type : S16
Time write : 0.787976s   Speed : 1075.83MB/s
Time read  : 0.338238s   Speed : 2506.3MB/s
Time read_ : 0.349752s   Speed : 2423.79MB/s
Compressed size: 35.3875% of original data

Data type : U16
Time write : 0.828928s   Speed : 1022.68MB/s
Time read  : 0.370559s   Speed : 2287.7MB/s
Time read_ : 0.390991s   Speed : 2168.15MB/s
Compressed size: 36.4181% of original data

Data type : S8
Time write : 0.723918s   Speed : 1171.03MB/s
Time read  : 0.340901s   Speed : 2486.72MB/s
Time read_ : 0.352235s   Speed : 2406.71MB/s
Compressed size: 29.0688% of original data

Data type : U8
Time write : 0.814962s   Speed : 1040.21MB/s
Time read  : 0.384705s   Speed : 2203.58MB/s
Time read_ : 0.402153s   Speed : 2107.97MB/s
Compressed size: 30.1191% of original data

Data type : BOOL
Time write : 0.934157s   Speed : 907.479MB/s
Time read  : 0.355885s   Speed : 2382.03MB/s
Time read_ : 0.397336s   Speed : 2133.53MB/s
Compressed size: 18.7794% of original data
Program ended with exit code: 0
```

The compressed size is calculated against 16 bytes non-compressed data would occupy memory per time
stamped value.

# License
```
MIT License

Copyright (c) 2018 Harri Smatt

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```
