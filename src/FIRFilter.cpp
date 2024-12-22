#include "../include/FIRFilter.h"


void fir(data_t *y, data_t x) {
    coef_t c[N] = {0.01444,0.03044,0.07242,0.12450,0.16675,0.18291,0.16675,0.12450,0.07242,0.03044,0.01444};
    static data_t shift_reg[N];
    acc_t acc = 0;
    int i;

    #pragma HLS array_partition variable=shift_reg complete
    #pragma HLS array_partition variable=c complete

    Shift_Reg_Loop:
    for (i = N - 1; i > 1; i = i - 2) {
        shift_reg[i] = shift_reg[i - 1];
        shift_reg[i - 1] = shift_reg[i - 2];
            
    }

    if (i == 1) shift_reg[1] = shift_reg[0];
    shift_reg[0] = x; 

    Convolution:
    for (i = N - 1; i >= 0; i--) {
        #pragma HLS unroll factor=2
        acc += shift_reg[i] * c[i];
    }   
    
    *y = acc;
}

// Original Implementation
// void fir(data_t *y, data_t x) {
//     coef_t c[N] = {0.01444,0.03044,0.07242,0.12450,0.16675,0.18291,0.16675,0.12450,0.07242,0.03044,0.01444};
//     static data_t shift_reg[N];
//     acc_t acc;
//     int i;

//     acc = 0;
//     Shift_Accum_Loop:
//     for (i = N - 1; i >= 0; i--) {
//         if (i == 0) {
//             acc += x * c[0];
//             shift_reg[0] = x;
//         } 
//         else {
//             shift_reg[i] = shift_reg[i - 1];
//             acc += shift_reg[i] * c[i];
//         }
//     }
//     // Shift_Accum_Loop:
//     // for (i = N - 1; i >= 1; i--) {
//     //         shift_reg[i] = shift_reg[i - 1];
//     //         acc += shift_reg[i] * c[i];
//     // }

//     // acc += x * c[0];
//     // shift_reg[0] = x;    
    
//     *y = acc;
// }