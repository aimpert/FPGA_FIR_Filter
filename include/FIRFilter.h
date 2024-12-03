#include <stddef.h>
#include <iostream>
#include <vector>
#include <fstream>

const std::string ROOT_DIR = "../../../../../";

enum FIRData {
    input,
    output,
    output_gold,
    all
};


void convolution(const float *x, size_t inputLength, const float *h, size_t filterLength);

class FIRFilter {
    private:
        const float *x;
        size_t inputLength;
        const float *h;
        size_t filterLength;
        float *y;
        size_t outputLength;
    public:
    FIRFilter(const float *inputSignal, size_t inputLength, const float *filterSignal, size_t filterLength);
    //~FIRFilter();

    void applyFilter();
    float* getOutput();
    void writeData(FIRData type);
};