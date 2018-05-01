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

#ifndef TIME_SERIES_DATA_RANGE_H
#define TIME_SERIES_DATA_RANGE_H

#include "timeseriesarraytypes.h"
#include "timeseriesdatablock.h"
#include "timeseriesdatacontainer.h"

namespace TimeSeries {

template <int BlockSize>
class TimeSeriesDataRange
{
public:
    TimeSeriesDataRange(const TimeSeriesDataContainer<BlockSize>* container,
                        time_s64 beginTime, time_s64 endTime) :
        m_beginTime(beginTime),
        m_endTime(endTime),
        m_container(container)
    {
    }

    ~TimeSeriesDataRange() = default;

    class Iterator
    {
    public:
        Iterator(const TimeSeriesDataContainer<BlockSize>* container,
                 time_s64 beginTime,
                 time_s64 endTime) :
            m_beginTime(beginTime),
            m_endTime(endTime),
            m_count(0),
            m_index(0),
            m_times(nullptr),
            m_values(nullptr),
            m_blockIndex(0),
            m_container(container)
        {
            if (m_beginTime < 0 && m_endTime < 0)
            {
                m_container = nullptr;
            }

            while (m_container && m_blockIndex < m_container->blockCount())
            {
                const auto block = m_container->block(m_blockIndex);

                if (block->endTime() >= m_endTime)
                {
                    m_times = new time_s64[BlockSize / 2 + 1];
                    m_values = new value_double[BlockSize / 2 + 1];
                    m_count = block->read(m_times, reinterpret_cast<value_u64*>(m_values));

                    while (m_index + 1 < m_count && m_times[m_index + 1] <= m_beginTime)
                    {
                        ++m_index;
                    }

                    break;
                }

                m_blockIndex++;
            }

            if (m_container && m_blockIndex >= m_container->blockCount())
            {
                m_container = nullptr;
            }
        }

        ~Iterator()
        {
            delete[] m_times;
            delete[] m_values;
        }

        time_s64 time() const
        {
            return m_times[m_index];
        }

        value_double value() const
        {
            return m_values[m_index];
        }

        Iterator& operator++()
        {
            if (m_times[m_index] >= static_cast<time_u64>(m_endTime))
            {
                m_container = nullptr;
            }
            else if (++m_index >= m_count)
            {
                if (++m_blockIndex >= m_container->blockCount())
                {
                    m_container = nullptr;
                }
                else
                {
                    const auto block = m_container->block(m_blockIndex);
                    m_count = block->read(m_times, reinterpret_cast<value_u64*>(m_values));
                    m_index = 0;
                }
            }

            return *this;
        }

        const Iterator& operator*() const
        {
            return *this;
        }

        bool operator!=(const Iterator& other) const
        {
            return m_container != other.m_container;
        }

    private:
        time_s64 m_beginTime;
        time_s64 m_endTime;

        int m_count;
        int m_index;
        time_s64* m_times;
        value_double* m_values;

        int m_blockIndex;
        const TimeSeriesDataContainer<BlockSize>* m_container;
    };

    const Iterator begin() const
    {
        return Iterator(m_container, m_beginTime, m_endTime);
    }

    const Iterator end() const
    {
        return Iterator(m_container, -1, -1);
    }

private:
    time_s64 m_beginTime;
    time_s64 m_endTime;
    const TimeSeriesDataContainer<BlockSize>* m_container;
};

} // namespace TimeSeries

#endif // TIME_SERIES_DATA_RANGE_H
