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

#ifndef TIME_SERIES_ARRAY_H
#define TIME_SERIES_ARRAY_H

#include "timeseriesarraytypes.h"
#include "timeseriesdatacontainer.h"
#include "timeseriesdataiterator.h"
#include "timeseriesdatarange.h"

namespace TimeSeries
{

template <int BlockSize = 8192> class TimeSeriesArray
{
public:
    TimeSeriesArray(time_s64 sizeMillis) : m_container(sizeMillis)
    {
    }

    virtual ~TimeSeriesArray()
    {
    }

    void append(time_s64 time, value_double value)
    {
        m_container.append(time, value);
    }

    TimeSeriesDataIterator<BlockSize> iter() const
    {
        return TimeSeriesDataIterator<BlockSize>(&m_container);
    }

    TimeSeriesDataRange<BlockSize> range(time_s64 beginTime = 0, time_s64 endTime = -1) const
    {
        return TimeSeriesDataRange<BlockSize>(&m_container, beginTime, endTime);
    }

    size_t dataSize() const
    {
        return m_container.dataSize();
    }

private:
    TimeSeriesDataContainer<BlockSize> m_container;
};

} // namespace TimeSeries

#endif // TIME_SERIES_ARRAY_H
