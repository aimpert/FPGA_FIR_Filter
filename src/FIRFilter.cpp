#include "../include/FIRFilter.h"

#define MAX_INPUT_LENGTH 1846172
#define MAX_FILTER_LENGTH 11

void convolution(const float *x, const float *h, float *y) {
    #pragma HLS INTERFACE m_axi port=x depth=MAX_INPUT_LENGTH
    #pragma HLS INTERFACE m_axi port=h depth=MAX_FILTER_LENGTH
    #pragma HLS INTERFACE m_axi port=y depth=MAX_INPUT_LENGTH
    #pragma HLS INTERFACE s_axilite port=return

    #pragma HLS ARRAY_PARTITION variable=x complete
    #pragma HLS ARRAY_PARTITION variable=h complete
    #pragma HLS ARRAY_PARTITION variable=y complete

    for (int n = 0; n < MAX_INPUT_LENGTH; ++n) {
        #pragma HLS PIPELINE II=1
        y[n] = 0;
        for (int k = 0; k < MAX_FILTER_LENGTH; ++k) {
        #pragma HLS UNROLL factor=2
            int index = n - k;
            if (index >= 0) {
                y[n] += h[k] * x[index];
            }
        }
    }
}

void convolution_sw(const float *x, unsigned long inputLength, const float *h, unsigned long filterLength, float *y) {

    for (unsigned long n = 0; n < inputLength; ++n) {
        y[n] = 0;
        for (unsigned long k = 0; k <  filterLength; ++k) {
            if (n >= k) { 
                y[n] += h[k] * x[n - k];
           }
        }
    }
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