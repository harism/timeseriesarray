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
Generate test data : 155556666 x (time_64bits + double_64bits) = 2373.61MB

Execute tests..

Data type : DOUBLE
Compress        : true
Time write      : 2.69366s   Speed : 881.182MB/s
Time read       : 0.861677s   Speed : 2754.64MB/s
Time read range : 1.00326s   Speed : 2365.89MB/s
Compressed size : 62.4812% of original data

Compress        : false
Time write      : 2.34296s   Speed : 1013.08MB/s
Time read       : 0.396095s   Speed : 5992.51MB/s
Time read range : 0.321013s   Speed : 7394.12MB/s
Compressed size : 100% of original data

Data type : FLOAT
Compress        : true
Time write      : 2.37374s   Speed : 999.942MB/s
Time read       : 0.886119s   Speed : 2678.65MB/s
Time read range : 1.05369s   Speed : 2252.65MB/s
Compressed size : 42.9719% of original data

Compress        : false
Time write      : 2.43934s   Speed : 973.052MB/s
Time read       : 0.399476s   Speed : 5941.8MB/s
Time read range : 0.323725s   Speed : 7332.18MB/s
Compressed size : 100% of original data

Data type : S64
Compress        : true
Time write      : 2.57208s   Speed : 922.834MB/s
Time read       : 0.979487s   Speed : 2423.32MB/s
Time read range : 0.961297s   Speed : 2469.17MB/s
Compressed size : 56.1533% of original data

Compress        : false
Time write      : 2.35582s   Speed : 1007.55MB/s
Time read       : 0.483373s   Speed : 4910.51MB/s
Time read range : 0.302164s   Speed : 7855.35MB/s
Compressed size : 100% of original data

Data type : U64
Compress        : true
Time write      : 2.71878s   Speed : 873.042MB/s
Time read       : 1.00389s   Speed : 2364.42MB/s
Time read range : 1.10781s   Speed : 2142.6MB/s
Compressed size : 56.1533% of original data

Compress        : false
Time write      : 2.49525s   Speed : 951.251MB/s
Time read       : 0.572871s   Speed : 4143.35MB/s
Time read range : 0.566828s   Speed : 4187.53MB/s
Compressed size : 100% of original data

Data type : S32
Compress        : true
Time write      : 2.3869s   Speed : 994.431MB/s
Time read       : 0.969648s   Speed : 2447.91MB/s
Time read range : 0.940186s   Speed : 2524.61MB/s
Compressed size : 47.8511% of original data

Compress        : false
Time write      : 2.39357s   Speed : 991.659MB/s
Time read       : 0.491102s   Speed : 4833.22MB/s
Time read range : 0.382008s   Speed : 6213.51MB/s
Compressed size : 100% of original data

Data type : U32
Compress        : true
Time write      : 2.58089s   Speed : 919.684MB/s
Time read       : 1.09107s   Speed : 2175.49MB/s
Time read range : 1.10344s   Speed : 2151.1MB/s
Compressed size : 48.8806% of original data

Compress        : false
Time write      : 2.55391s   Speed : 929.402MB/s
Time read       : 0.580389s   Speed : 4089.68MB/s
Time read range : 0.511225s   Speed : 4642.98MB/s
Compressed size : 100% of original data

Data type : S16
Compress        : true
Time write      : 2.22403s   Speed : 1067.25MB/s
Time read       : 0.937205s   Speed : 2532.64MB/s
Time read range : 0.92901s   Speed : 2554.99MB/s
Compressed size : 35.3477% of original data

Compress        : false
Time write      : 2.40595s   Speed : 986.556MB/s
Time read       : 0.469802s   Speed : 5052.36MB/s
Time read range : 0.414084s   Speed : 5732.18MB/s
Compressed size : 100% of original data

Data type : U16
Compress        : true
Time write      : 2.38289s   Speed : 996.105MB/s
Time read       : 1.0575s   Speed : 2244.54MB/s
Time read range : 1.10975s   Speed : 2138.86MB/s
Compressed size : 36.3779% of original data

Compress        : false
Time write      : 2.50454s   Speed : 947.723MB/s
Time read       : 0.586004s   Speed : 4050.5MB/s
Time read range : 0.493896s   Speed : 4805.88MB/s
Compressed size : 100% of original data

Data type : S8
Compress        : true
Time write      : 2.0439s   Speed : 1161.31MB/s
Time read       : 0.951222s   Speed : 2495.32MB/s
Time read range : 0.938082s   Speed : 2530.28MB/s
Compressed size : 29.033% of original data

Compress        : false
Time write      : 2.34259s   Speed : 1013.24MB/s
Time read       : 0.478495s   Speed : 4960.56MB/s
Time read range : 0.357927s   Speed : 6631.53MB/s
Compressed size : 100% of original data

Data type : U8
Compress        : true
Time write      : 2.30649s   Speed : 1029.1MB/s
Time read       : 1.04339s   Speed : 2274.9MB/s
Time read range : 1.07992s   Speed : 2197.95MB/s
Compressed size : 30.0827% of original data

Compress        : false
Time write      : 2.59745s   Speed : 913.822MB/s
Time read       : 0.576688s   Speed : 4115.93MB/s
Time read range : 0.52077s   Speed : 4557.88MB/s
Compressed size : 100% of original data

Data type : BOOL
Compress        : true
Time write      : 2.74557s   Speed : 864.523MB/s
Time read       : 0.971069s   Speed : 2444.32MB/s
Time read range : 1.09421s   Speed : 2169.24MB/s
Compressed size : 18.7544% of original data

Compress        : false
Time write      : 2.48342s   Speed : 955.782MB/s
Time read       : 0.515342s   Speed : 4605.88MB/s
Time read range : 0.549878s   Speed : 4316.6MB/s
Compressed size : 100% of original data
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
