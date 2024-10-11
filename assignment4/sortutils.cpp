#include "sortutils.hpp"
#include <algorithm>
#include <chrono>
#include <iostream>
#include <vector>

void initializeRawArrayFromStdArray(const SourceArray& source, int dest[]) {
    std::copy(source.begin(), source.end(), dest);
}

void organPipeStdArray(SourceArray& data) {
    const std::size_t mid = HOW_MANY_ELEMENTS / 2;
    for (std::size_t i = mid + 1; i < HOW_MANY_ELEMENTS; i++) {
        data[i] = data[HOW_MANY_ELEMENTS - 1 - i];
    }
}

namespace {
    template<typename Array>
    double measureSortTime(const Array& source, [[maybe_unused]] bool isParallel) {
        Array arr = source;  // Make a copy
        const auto start = std::chrono::high_resolution_clock::now();
        std::sort(std::begin(arr), std::end(arr));
        const auto end = std::chrono::high_resolution_clock::now();
        return std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    }

    double measureVectorSortTime(const SourceArray& source, [[maybe_unused]] bool isParallel) {
        std::vector<int> vec(source.begin(), source.end());
        const auto start = std::chrono::high_resolution_clock::now();
        std::sort(vec.begin(), vec.end());
        const auto end = std::chrono::high_resolution_clock::now();
        return std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    }

    double measureRawArraySortTime(const SourceArray& source, [[maybe_unused]] bool isParallel) {
        int arr[HOW_MANY_ELEMENTS];
        std::copy(source.begin(), source.end(), arr);
        const auto start = std::chrono::high_resolution_clock::now();
        std::sort(arr, arr + HOW_MANY_ELEMENTS);
        const auto end = std::chrono::high_resolution_clock::now();
        return std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    }

    template<typename MeasureFunc>
    void evaluatePerformance(const std::string& title,
                            const SourceArray& random,
                            const SourceArray& sorted,
                            const SourceArray& reversed,
                            const SourceArray& organPipe,
                            const SourceArray& rotated,
                            MeasureFunc measureFunction) {
        std::cout << "\n --- " << title << " ---\n\n";

        for (int isParallel = 0; isParallel <= 1; isParallel++) {
            std::cout << (isParallel ? "Parallel\n" : "Sequential\n");

            double randomTime = 0.0, sortedTime = 0.0, reversedTime = 0.0;
            double organPipeTime = 0.0, rotatedTime = 0.0;

            for (std::uint8_t i = 0; i < HOW_MANY_TIMES; i++) {
                randomTime += measureFunction(random, isParallel != 0);
                sortedTime += measureFunction(sorted, isParallel != 0);
                reversedTime += measureFunction(reversed, isParallel != 0);
                organPipeTime += measureFunction(organPipe, isParallel != 0);
                rotatedTime += measureFunction(rotated, isParallel != 0);
            }

            std::cout << "\tRandom Time     : " << static_cast<int>(randomTime / 1000.0) << " ms\n";
            std::cout << "\tSorted Time     : " << static_cast<int>(sortedTime / 1000.0) << " ms\n";
            std::cout << "\tReversed Time   : " << static_cast<int>(reversedTime / 1000.0) << " ms\n";
            std::cout << "\tOrgan Pipe Time : " << static_cast<int>(organPipeTime / 1000.0) << " ms\n";
            std::cout << "\tRotated Time    : " << static_cast<int>(rotatedTime / 1000.0) << " ms\n\n";
        }
    }
}

void evaluateRawArray(const SourceArray& random, const SourceArray& sorted, const SourceArray& reversed,
                     const SourceArray& organPipe, const SourceArray& rotated) {
    evaluatePerformance("Raw Array Performance", random, sorted, reversed, organPipe, rotated,
                       measureRawArraySortTime);
}

void evaluateStdArray(const SourceArray& random, const SourceArray& sorted, const SourceArray& reversed,
                     const SourceArray& organPipe, const SourceArray& rotated) {
    evaluatePerformance("std::array Performance", random, sorted, reversed, organPipe, rotated,
                       measureSortTime<SourceArray>);
}

void evaluateStdVector(const SourceArray& random, const SourceArray& sorted, const SourceArray& reversed,
                      const SourceArray& organPipe, const SourceArray& rotated) {
    evaluatePerformance("std::vector Performance", random, sorted, reversed, organPipe, rotated,
                       measureVectorSortTime);
}