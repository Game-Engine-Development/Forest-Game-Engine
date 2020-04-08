#include "Headers/Engine/Math/MathUtils.h"

float mathRound(float value) {
    return std::round(value * 1000.0f) / 1000.0f;
}

bool isIntegral(char num) {
    static constexpr int ZERO = 48;
    static constexpr int NINE = 57;

    //has an ascii code in between (and including) the literals 0 & 9
    return ZERO <= num && num <= NINE;
}

void reverse(unsigned char *start, unsigned char *end, size_t block_size, size_t numOfBlocks) {
    for(int x1 = 0; x1 < numOfBlocks/2; ++x1) {
        for(int x2 = 0; x2 < block_size; ++x2) {
            unsigned char temp; //will optimize away later

            temp = *(start+(x1*block_size + x2));
            *(start+(x1*block_size + x2)) = *(end+(-(x1*block_size) - (block_size-1) + x2));
            *(end+(-(x1*block_size) - (block_size-1) + x2)) = temp;
        }
    }
}

void printArray(unsigned char *start, size_t size) {
    for(int x1 = 0; x1 < size; ++x1) {
        std::cout << static_cast<int>(start[x1]) << " ";
    }
    std::cout << std::endl;
}
