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

#ifndef TIME_SERIES_DATA_CONTAINER_H
#define TIME_SERIES_DATA_CONTAINER_H

#include "timeseriesarraytypes.h"
#include "timeseriesdatablock.h"
#include "timeseriespointerbuffer.h"

namespace TimeSeries
{

template <int BlockSize> class TimeSeriesDataContainer
{
public:
    TimeSeriesDataContainer(time_s64 sizeMillis) : m_sizeMillis(sizeMillis)
    {
    }

    ~TimeSeriesDataContainer() = default;

    void append(time_s64 time, value_double value)
    {
        const time_s64 MIN_TIME = time - m_sizeMillis;
        const value_u64 valueIn= *reinterpret_cast<const value_u64*>(&value);
        while (m_blocks.size() > 1 && m_blocks.at(1)->beginTime() <= MIN_TIME)
        {
            m_blocks.removeFirst();
        }
        if (blockCount() == 0 || !m_blocks.last()->append(time, valueIn))
        {
            m_blocks.append(new TimeSeriesDataBlock<BlockSize>(time, valueIn));
        }
    }

    int blockCount() const
    {
        return m_blocks.size();
    }

    const TimeSeriesDataBlock<BlockSize>* block(int index) const
    {
        return m_blocks.at(index);
    }

    size_t dataSize() const
    {
        size_t size = 0;
        for (int index = 0; index < m_blocks.size(); ++index) {
            size += m_blocks.at(index)->dataSize();
        }
        return size;
    }

private:
    time_s64 m_sizeMillis;
    TimeSeriesPointerBuffer<TimeSeriesDataBlock<BlockSize>> m_blocks;
};

} // namespace TimeSeries

#endif // TIME_SERIES_DATA_CONTAINER_H
