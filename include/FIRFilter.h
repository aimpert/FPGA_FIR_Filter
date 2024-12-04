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


void convolution(const float *x, const float *h, unsigned long filterLength, float *y, unsigned long outputLength);

class FIRFilter {
    private:
        const float *x;
        unsigned long inputLength;
        const float *h;
        unsigned long filterLength;
        float *y;
        unsigned long outputLength;
    public:
    FIRFilter(const float *inputSignal, unsigned long inputLength, const float *filterSignal, unsigned long filterLength);
    //~FIRFilter();

    void applyFilter();
    float* getOutput();
    void writeData(FIRData type);
};