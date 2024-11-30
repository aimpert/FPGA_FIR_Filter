#include "FIRFilter.h"

FIRFilter::FIRFilter(const float *inputSignal, size_t inputLength, const float *filterSignal, size_t filterLength) {
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
    for (size_t n = 0; n < outputLength; ++n) {
        y[n] = 0;
        for (size_t k = 0; k < filterLength; ++k) {
            if (n >= k) { // Ensure we're not accessing out-of-bounds
                y[n] += h[k] * x[n - k];
            }
        }
    }
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
        writeArrayToFile("input.dat", this->x, inputLength);
    }

    if (type == FIRData::output || type == FIRData::all) {
        writeArrayToFile("output.dat", this->y, outputLength);
    }
}