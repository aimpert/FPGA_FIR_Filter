#include "../include/FIRFilter.h"

void fir(data_t *y, data_t x) {
    coef_t c[N] = {0.01444,0.03044,0.07242,0.12450,0.16675,0.18291,0.16675,0.12450,0.07242,0.03044,0.01444};
    static data_t shift_reg[N];
    acc_t acc;
    int i;

    acc = 0;
    Shift_Accum_Loop:
    for (i = N - 1; i >= 0; i--) {
        if (i == 0) {
            acc += x * c[0];
            shift_reg[0] = x;
        } 
        else {
            shift_reg[i] = shift_reg[i - 1];
            acc += shift_reg[i] * c[i];
        }
    }
    *y = acc;
}