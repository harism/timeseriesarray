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

#ifndef TIME_SERIES_POINTER_BUFFER_H
#define TIME_SERIES_POINTER_BUFFER_H

namespace TimeSeries
{

template <class PointerType> class TimeSeriesPointerBuffer
{
public:
    TimeSeriesPointerBuffer() :
        m_size(0),
        m_offset(0),
        m_allocationSize(256),
        m_allocationSizeMask(m_allocationSize - 1),
        m_pointerBuffer(new PointerType*[m_allocationSize])
    {
    }

    virtual ~TimeSeriesPointerBuffer()
    {
        for (int index = 0; index < m_size; ++index)
        {
            delete m_pointerBuffer[(m_offset + index) & m_allocationSizeMask];
        }

        delete[] m_pointerBuffer;
    }

    int size() const
    {
        return m_size;
    }

    PointerType* at(int index) const
    {
        return m_pointerBuffer[(m_offset + index) & m_allocationSizeMask];
    }

    PointerType* first() const
    {
        return m_pointerBuffer[m_offset];
    }

    PointerType* last() const
    {
        return m_pointerBuffer[(m_offset + m_size - 1) & m_allocationSizeMask];
    }

    void append(PointerType *pPointer)
    {
        if (m_size == m_allocationSize)
        {
            m_allocationSize <<= 1;
            auto newPointerBuffer = new PointerType*[m_allocationSize];

            const int itemCountHead = (m_offset + m_size) & m_allocationSizeMask;
            const int itemCountTail = m_size - itemCountHead;
            std::memcpy(newPointerBuffer, m_pointerBuffer + m_offset, sizeof(PointerType*) * itemCountTail);
            std::memcpy(newPointerBuffer + itemCountTail, m_pointerBuffer, sizeof(PointerType*) * itemCountHead);

            delete[] m_pointerBuffer;
            m_pointerBuffer = newPointerBuffer;
            m_allocationSizeMask = m_allocationSize - 1;
            m_offset = 0;
        }

        m_pointerBuffer[(m_offset + m_size) & m_allocationSizeMask] = pPointer;
        m_size++;
    }

    void removeFirst()
    {
        delete m_pointerBuffer[m_offset];
        m_offset = (m_offset + 1) & m_allocationSizeMask;
        m_size--;
    }

private:
    int m_size;
    int m_offset;
    int m_allocationSize;
    int m_allocationSizeMask;
    PointerType** m_pointerBuffer;
};

} // namespace TimeSeries

#endif // TIME_SERIES_POINTER_BUFFER_H
