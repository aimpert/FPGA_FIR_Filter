#include "../include/FIRFilter.h"


void convolution(const float *x, const float *h, unsigned long filterLength, float *y, unsigned long outputLength) {

    for (unsigned long n = 0; n < outputLength; ++n) {
        y[n] = 0;
        for (unsigned long k = 0; k < filterLength; ++k) {
            if (n >= k) { // Ensure we're not accessing out-of-bounds
                y[n] += h[k] * x[n - k];
            }
        }
    }
}

FIRFilter::FIRFilter(const float *inputSignal, unsigned long inputLength, const float *filterSignal, unsigned long filterLength) {
    // input params
    x = inputSignal;
    this->inputLength = inputLength;

    // filter params
    h = filterSignal;
    this->filterLength = filterLength;

    // output params for now
    outputLength = inputLength;
    y = new float[inputLength]();
}

void FIRFilter::applyFilter() {
    convolution(x, h, filterLength, y, outputLength);
}

float* FIRFilter::getOutput() {
    return this->y;
}

// Function to write input and output datas to a .dat file for Vitis HLS
void FIRFilter::writeData(FIRData type) {
    auto writeArrayToFile = [](const std::string& filename, const float* data, size_t length) {
        std::ofstream myfile(filename);
        if (!myfile.is_open()) {
            std::cerr << "File failed to open." << std::endl;
        }
        else {
            for (int i = 0; i < length; i++) {
                myfile << data[i] << std::endl;
            }
            myfile.close();
        }
    };

    if (type == FIRData::input || type == FIRData::all) {
        writeArrayToFile(ROOT_DIR + "input.dat", this->x, inputLength);
    }

    if (type == FIRData::output || type == FIRData::all) {
        writeArrayToFile(ROOT_DIR + "output.dat", this->y, outputLength);
    }

    if (type == FIRData::output_gold || type == FIRData::all) {
        writeArrayToFile(ROOT_DIR + "output.gold.dat", this->y, outputLength);
    }

}