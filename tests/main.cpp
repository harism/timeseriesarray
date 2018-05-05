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

#include <chrono>
#include <cmath>
#include <iostream>
#include <sstream>
#include <timeseriesarray.h>

using TimeSeries::TimeSeriesArray;

namespace {

enum class DataType
{
    BOOL,
    S8,
    U8,
    S16,
    U16,
    S32,
    U32,
    S64,
    U64,
    FLOAT,
    DOUBLE
};

struct TestData
{
    DataType dataType;
    int timeStep;
    const double *values;
    int valueCount;
};

struct TestResult
{
    bool isSuccess;
    double durationWrite;
    double durationRead;
    double compressedRatio;
    double durationReadRange;
    std::string error;
};

double convert(DataType dataType, double value)
{
    switch (dataType)
    {
    case DataType::BOOL:
        return (double)(((unsigned long long)(value)) & 0x01);
    case DataType::S8:
        return (double)((char)((long long)(value)));
    case DataType::U8:
        return (double)((unsigned char)((unsigned long long)(value)));
    case DataType::S16:
        return (double)((short)((long long)(value)));
    case DataType::U16:
        return (double)((unsigned short)((unsigned long long)(value)));
    case DataType::S32:
        return (double)((int)((long long)(value)));
    case DataType::U32:
        return (double)((unsigned int)((unsigned long long)(value)));
    case DataType::S64:
        return (double)((long long)(value));
    case DataType::U64:
        return (double)((unsigned long long)(abs(value)));
    case DataType::FLOAT:
        return (double)((float)(value));
    case DataType::DOUBLE:
    default:
        return value;
    }
}

template<bool Compress>
TestResult testReadAndWrite(TestData &data)
{
    TestResult result;
    result.isSuccess = true;

    const TimeSeries::time_s64 timeStart = 55556666;
    const TimeSeries::time_s64 timeStep = data.timeStep;
    TimeSeriesArray<65536, Compress> array(timeStep * data.valueCount);

    // Test writing timeseries data
    {
        const auto durationStart = std::chrono::steady_clock::now();
        TimeSeries::time_s64 time = timeStart;

        for (int index = 0; index < data.valueCount; ++index)
        {
            double value = convert(data.dataType, data.values[index]);
            array.append(time, value);
            time += timeStep;
        }

        result.durationWrite = std::chrono::duration<double>(
                               std::chrono::steady_clock::now() - durationStart).count();
    }

    // Test reading timeseries data
    {
        const auto durationStart = std::chrono::steady_clock::now();
        TimeSeries::time_s64 time = timeStart;
        auto iter = array.iter();

        for (int index = 0; index < data.valueCount; ++index)
        {
            const double value = iter.value();
            const double expectedValue = convert(data.dataType, data.values[index]);

            /* if (!iter.isValid())
            {
                std::ostringstream error;
                error << "Iterator is not valid at index=" << index;
                result.error = error.str();
                result.isSuccess = false;
                break;
            }
            else */ if (iter.time() != time)
            {
                std::ostringstream error;
                error << "Time mismatch at index=" << index << "  " << iter.time()
                      << "!=" << time;
                result.error = error.str();
                result.isSuccess = false;
                break;
            }
            else if (value != expectedValue)
            {
                std::ostringstream error;
                error << "Value mismatch at index=" << index << "  " << value
                      << "!=" << expectedValue;
                result.error = error.str();
                result.isSuccess = false;
                break;
            }

            time += timeStep;
            iter.next();
        }

        if (result.isSuccess && iter.isValid())
        {

            iter.next();

            result.error = "Iterator over-indexing";
            result.isSuccess = false;
        }

        result.durationRead = std::chrono::duration<double>(
                              std::chrono::steady_clock::now() - durationStart).count();
        result.compressedRatio = array.dataSize() / (16.0 * data.valueCount);
    }

    // Test reading timeseries data
    {
        const auto durationStart = std::chrono::steady_clock::now();
        TimeSeries::time_s64 time = timeStart;
        const auto range = array.range();
        int index = 0;

        for (const auto& iter : range)
        {
            const double value = iter.value();
            const double expectedValue = convert(data.dataType, data.values[index]);

            /* if (!iter.isValid())
            {
                std::ostringstream error;
                error << "Iterator is not valid at index=" << index;
                result.error = error.str();
                result.isSuccess = false;
                break;
            }
            else */ if (iter.time() != time)
            {
                std::ostringstream error;
                error << "Time mismatch at index=" << index << "  " << iter.time()
                      << "!=" << time;
                result.error = error.str();
                result.isSuccess = false;
                break;
            }
            else if (value != expectedValue)
            {
                std::ostringstream error;
                error << "Value mismatch at index=" << index << "  " << value
                      << "!=" << expectedValue;
                result.error = error.str();
                result.isSuccess = false;
                break;
            }

            time += timeStep;
            index++;
        }

        if (result.isSuccess && index != data.valueCount) {
            result.error = "Iterator invalid indexing";
            result.isSuccess = false;
        }

        result.durationReadRange = std::chrono::duration<double>(
                                   std::chrono::steady_clock::now() - durationStart).count();
    }

    return result;
}

template<bool Compress>
bool test(DataType dataType, const double *values, int valueCount)
{
    TestData testData;
    testData.dataType = dataType;
    testData.timeStep = 155;
    testData.values = values;
    testData.valueCount = valueCount;

    const double timeScale = (valueCount * 16.0) / (1024 * 1024);
    std::cout << "Compress        : " << (Compress ? "true" : "false") << std::endl;
    auto result = testReadAndWrite<Compress>(testData);

    std::cout
        << "Time write      : " << result.durationWrite << "s   Speed : "
        << (timeScale * (1.0 / result.durationWrite)) << "MB/s"
        << std::endl

        << "Time read       : " << result.durationRead
        << "s   Speed : " << (timeScale * (1.0 / result.durationRead)) << "MB/s"
        << std::endl

        << "Time read range : " << result.durationReadRange
        << "s   Speed : " << (timeScale * (1.0 / result.durationReadRange)) << "MB/s"
        << std::endl

        << "Compressed size : " << (result.compressedRatio * 100.0) << "% of original data"
        << std::endl;

    if (!result.isSuccess) {
        std::cout << "Failed: " << result.error << std::endl;
    }

    return result.isSuccess;
}

} // Unnamed namespace

int main(int argc, char **argv) {
    bool testFailed = false;
    const int valueCount = 155556666;
    double* values = new double[valueCount];

    std::cout
        << "Generate test data : " << valueCount
        << " x (time_64bits + double_64bits) = " << ((16.0 * valueCount) / (1024.0 * 1024.0)) << "MB"
        << std::endl;

    for (int index = 0; index < valueCount; index++)
    {
        double value = sin((4.882341 * index) / valueCount);
        value += cos((5234.17181 * index) / valueCount) * 11222.1262;
        value += sin((222343.152223 * index) / valueCount) * 2234439941.73215246;
        value += cos((52223434.123642523 * index) / valueCount) * 9933234954858.88513341;
        values[index] = value;
    }

    std::cout << std::endl << "Execute tests.." << std::endl;

    const DataType dataTypes[] =
    {
        DataType::DOUBLE,
        DataType::FLOAT,
        DataType::S64,
        DataType::U64,
        DataType::S32,
        DataType::U32,
        DataType::S16,
        DataType::U16,
        DataType::S8,
        DataType::U8,
        DataType::BOOL
    };

    for (const auto dataType : dataTypes) {
        std::cout << std::endl;
        std::cout << "Data type : ";

        switch (dataType)
        {
        case DataType::BOOL:
            std::cout << "BOOL";
            break;
        case DataType::U8:
            std::cout << "U8";
            break;
        case DataType::S8:
            std::cout << "S8";
            break;
        case DataType::U16:
            std::cout << "U16";
            break;
        case DataType::S16:
            std::cout << "S16";
            break;
        case DataType::U32:
            std::cout << "U32";
            break;
        case DataType::S32:
            std::cout << "S32";
            break;
        case DataType::U64:
            std::cout << "U64";
            break;
        case DataType::S64:
            std::cout << "S64";
            break;
        case DataType::FLOAT:
            std::cout << "FLOAT";
            break;
        case DataType::DOUBLE:
            std::cout << "DOUBLE";
            break;
        }

        std::cout << std::endl;
        testFailed |= !test<true>(dataType, values, valueCount);
        std::cout << std::endl;
        testFailed |= !test<false>(dataType, values, valueCount);
    }

    delete[] values;
    return testFailed;
}
