#include <stddef.h>
#include <iostream>
#include <vector>
#include <fstream>

const std::string ROOT_DIR = "../../../../../"; // Vitis HLS
// const std::string ROOT_DIR = "./"; // CMake

enum option {
    input,
    output,
    output_gold,
    all
};


float* convolution(const float *x, unsigned long inputLength, const float *h, unsigned long filterLength);

void writeData(option type, float* data, size_t length);

// class FIRFilter {
//     private:
//         const float *x;
//         unsigned long inputLength;
//         const float *h;
//         unsigned long filterLength;
//         float *y;
//         unsigned long outputLength;
//     public:
//     FIRFilter(const float *inputSignal, unsigned long inputLength, const float *filterSignal, unsigned long filterLength);
//     //~FIRFilter();

//     void applyFilter();
//     float* getOutput();
// };