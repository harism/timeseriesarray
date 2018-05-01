# Description

TimeSeriesArray is simple in-memory storage model for time series data consisting of 64bit unsigned
integer timestamp and corresponding 64bit double value. The compression itself is secondary target
for this implementation and main goal is to provide some compression without losing much of read
accessing speed. That is this implementation is targeted at efficient realtime rendering of stored
instead.

Same applies to writing speed which is not tried to optimize same as reading speed.

### Current status

The implementation reaches roughly following speeds on MacBook Pro 2013 model:
- Write speed roughly 800-950MB/second
- Read speed roughly 2-2.5GB/second

These are measured with the included test application.

The application and time series array is written with Clang and XCode but should be relatively
easily portable for other platforms, too.

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
