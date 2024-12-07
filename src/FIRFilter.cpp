#include "../include/FIRFilter.h"


float* convolution(const float *x, unsigned long inputLength, const float *h, unsigned long filterLength) {
    float* y = new float[inputLength]();
    for (unsigned long n = 0; n < inputLength; ++n) {
        y[n] = 0;
        for (unsigned long k = 0; k < filterLength; ++k) {
            if (n >= k) { // Ensure we're not accessing out-of-bounds
                y[n] += h[k] * x[n - k];
            }
        }
    }
    return y;
}


// Function to write input and output datas to a .dat file for Vitis HLS
void writeData(option type, float* data, size_t length) {
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

    if (type == option::input || type == option::all) {
        writeArrayToFile(ROOT_DIR + "input.dat", data, length);
    }

    if (type == option::output || type == option::all) {
        writeArrayToFile(ROOT_DIR + "output.dat", data, length);
    }

    if (type == option::output_gold || type == option::all) {
        writeArrayToFile(ROOT_DIR + "output.gold.dat", data, length);
    }

}