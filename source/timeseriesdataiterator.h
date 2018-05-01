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

#ifndef TIME_SERIES_DATA_ITERATOR_H
#define TIME_SERIES_DATA_ITERATOR_H

#include "timeseriesarraytypes.h"
#include "timeseriesdatablock.h"
#include "timeseriesdatacontainer.h"

namespace TimeSeries {

template <int BlockSize> class TimeSeriesDataIterator
{
public:
    TimeSeriesDataIterator(const TimeSeriesDataContainer<BlockSize>* container) :
        m_time(0),
        m_value(0),
        m_blockCount(0),
        m_blockIndex(0),
        m_blockReadIndex(0),
        m_block(nullptr),
        m_container(container)
    {
        m_blockCount = container->blockCount();

        if (m_blockCount > 0)
        {
            m_block = container->block(0);
            m_time = m_block->beginTime();
            m_value = m_block->beginValue();
        }
    }

    virtual ~TimeSeriesDataIterator()
    {
    }

    time_s64 time() const
    {
        return m_time;
    }

    value_double value() const
    {
        return *reinterpret_cast<const value_double*>(&m_value);
    }

    bool isValid() const
    {
        return m_container != nullptr;
    }

    void next()
    {
        if (m_blockReadIndex < m_block->size())
        {
            m_blockReadIndex += m_block->readAtOffset(m_blockReadIndex, m_time, m_value);
        }
        else if (++m_blockIndex >= m_container->blockCount())
        {
            m_container = nullptr;
        }
        else
        {
            m_blockReadIndex = 0;
            m_block = m_container->block(m_blockIndex);
            m_time = m_block->beginTime();
            m_value = m_block->beginValue();
        }
    }

private:
    time_s64 m_time;
    value_u64 m_value;

    int m_blockCount;
    int m_blockIndex;
    int m_blockReadIndex;
    const TimeSeriesDataBlock<BlockSize>* m_block;
    const TimeSeriesDataContainer<BlockSize>* m_container;
};

} // namespace TimeSeries

#endif // TIME_SERIES_DATA_ITERATOR_H
