#include "../include/FIRFilter.h"


void fir(data_t *y, data_t x) {
    coef_t c[N] = {53, 0, -91, 0, 313, 500, 313, 0, -91, 0,53};
    static data_t shift_reg[N];
    acc_t acc = 0;
    int i;

    #pragma HLS array_partition variable=shift_reg complete
    #pragma HLS array_partition variable=c complete

    Shift_Reg_Loop:
    for (i = N - 1; i > 1; i = i - 2) {
        #pragma HLS pipeline II=1
        shift_reg[i] = shift_reg[i - 1];
        shift_reg[i - 1] = shift_reg[i - 2];
            
    }
    shift_reg[0] = x; 

    // Convolution:
    // for (i = N - 1; i >= 0; i--) {
    //     #pragma HLS pipeline II=1
    //     #pragma HLS unroll factor=4          
    //     acc = acc + shift_reg[i] * c[i];
    // }
    for (i = N - 1; i >= 3; i -= 4) {
        #pragma HLS pipeline II=1
        acc += shift_reg[i] * c[i] + shift_reg[i - 1] * c[i - 1] +
            shift_reg[i - 2] * c[i - 2] + shift_reg[i - 3] * c[i - 3];
    }
    for (; i >= 0; i--) {
        acc += shift_reg[i] * c[i];
    }

    *y = acc;
}


// void fir(data_t *y, data_t x) {
//     coef_t c[N] = {53, 0, -91, 0, 313, 500, 313, 0, -91, 0,53};
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