#include "sortutils.hpp"
#include <algorithm>
#include <random>

int main() {
    // Create and initialize random array
    SourceArray random{};  // Zero-initialize array
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(-10'000'000, 10'000'000);
    
    for (std::size_t i = 0; i < HOW_MANY_ELEMENTS; i++) {
        random[i] = dist(gen);
    }

    // Create sorted array
    SourceArray sorted = random;
    std::sort(sorted.begin(), sorted.end());

    // Create reversed array
    SourceArray reversed = sorted;
    std::reverse(reversed.begin(), reversed.end());

    // Create organ pipe array
    SourceArray organPipe = sorted;
    organPipeStdArray(organPipe);

    // Create rotated array
    SourceArray rotated = sorted;
    std::rotate(rotated.begin(), rotated.begin() + 1, rotated.end());

    // Evaluate performance for each array type
    evaluateRawArray(random, sorted, reversed, organPipe, rotated);
    evaluateStdArray(random, sorted, reversed, organPipe, rotated);
    evaluateStdVector(random, sorted, reversed, organPipe, rotated);

    return 0;
}