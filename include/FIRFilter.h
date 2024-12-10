#include <stddef.h>
#include <iostream>
#include <vector>
#include <fstream>

// const std::string ROOT_DIR = "../../../../../"; // Vitis HLS
const std::string ROOT_DIR = "./"; // CMake

enum option {
    input,
    output,
    output_gold,
    all
};


void convolution(const float *x, const float *h, float *y);
void convolution_sw(const float *x, unsigned long inputLength, const float *h, unsigned long filterLength, float *y);

void writeData(option type, float* data, size_t length);