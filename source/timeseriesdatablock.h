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

#ifndef TIME_SERIES_DATA_BLOCK_H
#define TIME_SERIES_DATA_BLOCK_H

#include "timeseriesarraytypes.h"

namespace TimeSeries {

template <int BlockSize, bool Compressed>
class TimeSeriesDataBlock;

template <int BlockSize>
class TimeSeriesDataBlock<BlockSize, true>
{
public:
    TimeSeriesDataBlock(time_s64 time, value_u64 value) :
        m_dataSize(0),
        m_beginTime(time),
        m_endTime(time),
        m_beginValue(value),
        m_endValue(value)
    {
    }

    ~TimeSeriesDataBlock() = default;

    time_s64 beginTime() const
    {
        return m_beginTime;
    }

    time_s64 endTime() const
    {
        return m_endTime;
    }

    value_u64 beginValue() const
    {
        return m_beginValue;
    }

    value_u64 endValue() const
    {
        return m_endValue;
    }

    int size() const
    {
        return m_dataSize;
    }

    size_t dataSize() const
    {
        return 16 + m_dataSize;
    }

    bool append(time_s64 time, value_u64 value)
    {
        if (time <= m_endTime)
        {
            return true;
        }

        const time_u32 timeDiff = static_cast<time_u32>(time - m_endTime);
        const int timeDiffSize = 0x03 ^ (countLeadingZeroBits(timeDiff) >> 3);

        value_u64 valueOut = value ^ m_endValue;
        const int valueOutSizeTrailing = countTrailingZeroBits(valueOut) >> 3;
        valueOut >>= valueOutSizeTrailing << 3;

        const int valueOutSize = 8 - valueOutSizeTrailing;
        const int needsBytes = timeDiffSize + valueOutSize + 2;

        if (m_dataSize + needsBytes + (8 - valueOutSize) > BlockSize)
        {
            return false;
        }

        value_u8* output = m_data + m_dataSize;
        output[0] = (timeDiffSize << 6) | valueOutSize;

        *reinterpret_cast<time_u32*>(output + 1) = timeDiff;
        *reinterpret_cast<value_u64*>(output + timeDiffSize + 2) = valueOut;

        m_endTime = time;
        m_endValue = value;
        m_dataSize += needsBytes;

        return true;
    }

    int readAtOffset(int byteOffset, time_s64& time, value_u64& value) const
    {
        const value_u8 *input = m_data + byteOffset;
        const value_u8 infoByte = input[0];
        const int timeDiffSize = (infoByte >> 6) & 0x03;
        const int valueDiffIndex = timeDiffSize + 2;
        const int valueDiffSize = infoByte & 0x0F;

        value_u64 dataValue = *reinterpret_cast<const value_u64*>(input + 1);
        time += dataValue & ((1ULL << ((timeDiffSize + 1) << 3)) - 1);

        const int zeroBits = (8 - valueDiffSize) << 3;
        dataValue = *reinterpret_cast<const value_u64*>(input + valueDiffIndex);
        dataValue &= valueDiffSize ? (-1ULL >> zeroBits) : 0;
        dataValue <<= zeroBits;
        value ^= dataValue;

        return valueDiffIndex + valueDiffSize;
    }

    int read(time_s64* times, value_u64* values) const
    {
        int count = 0;
        const value_u8* input = m_data;
        const value_u8* inputEnd = m_data + m_dataSize;

        time_s64 time = times[count] = m_beginTime;
        value_u64 value = values[count++] = m_beginValue;

        value_u8 infoByte = 0;
        value_u64 dataValue = 0;

        value_u64 timeDiffSize = 0;
        value_u64 valueDiffSize = 0;
        value_u64 zeroBitCount = 0;

        while (input < inputEnd)
        {
            infoByte = *(input++);
            timeDiffSize = (infoByte >> 6) + 1;
            valueDiffSize = infoByte & 0x0F;

            dataValue = *reinterpret_cast<const value_u64*>(input);
            time += dataValue & ((1ULL << (timeDiffSize << 3)) - 1);
            input += timeDiffSize;

            zeroBitCount = (8 - valueDiffSize) << 3;
            dataValue = *reinterpret_cast<const value_u64*>(input);
            value ^= (dataValue & (-(valueDiffSize != 0))) << zeroBitCount;
            input += valueDiffSize;

            times[count] = time;
            values[count++] = value;
        }

        return count;
    }

private:
    int countLeadingZeroBits(const time_u32 u32) const
    {
        return u32 ? __builtin_clz(u32) : 64;
    }

    int countLeadingZeroBits(const value_u64 u64) const
    {
        return u64 ? __builtin_clzll(u64) : 64;
    };

    int countTrailingZeroBits(const value_u64 u64) const
    {
        return u64 ? __builtin_ctzll(u64) : 64;
    };

    int m_dataSize;
    time_s64 m_beginTime;
    time_s64 m_endTime;
    value_u64 m_beginValue;
    value_u64 m_endValue;
    value_u8 m_data[BlockSize];
};

template <int BlockSize>
class TimeSeriesDataBlock<BlockSize, false>
{
public:
    TimeSeriesDataBlock(time_s64 time, value_u64 value) :
        m_index(0)
    {
        m_times[0] = time;
        m_values[0] = value;
    }

    ~TimeSeriesDataBlock() = default;

    time_s64 beginTime() const
    {
        return m_times[0];
    }

    time_s64 endTime() const
    {
        return m_times[m_index];
    }

    value_u64 beginValue() const
    {
        return m_values[0];
    }

    value_u64 endValue() const
    {
        return m_values[m_index];
    }

    int size() const
    {
        return m_index;
    }

    size_t dataSize() const
    {
        return (m_index + 1) * 16;
    }

    bool append(time_s64 time, value_u64 value)
    {
        if (time <= endTime())
        {
            return true;
        }

        if (m_index >= (BlockSize / 16))
        {
            return false;
        }

        m_index++;
        m_times[m_index] = time;
        m_values[m_index] = value;

        return true;
    }

    int readAtOffset(int offset, time_s64& time, value_u64& value) const
    {
        time = m_times[offset + 1];
        value = m_values[offset + 1];
        return 1;
    }

    int read(time_s64*& times, value_u64*& values) const
    {
        times = m_times;
        values = m_values;
        return m_index + 1;
    }

private:
    int m_index;
    mutable time_s64 m_times[BlockSize / 16 + 1];
    mutable value_u64 m_values[BlockSize / 16 + 1];
};

} // namespace TimeSeries

#endif // TIME_SERIES_DATA_BLOCK_H
