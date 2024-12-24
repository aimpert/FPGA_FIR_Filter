#include <stddef.h>
#include <iostream>
#include <vector>
#include <fstream>

const std::string ROOT_DIR = "../../../../../"; // Vitis HLS
// const std::string ROOT_DIR = "./"; // CMake

const int N=11;

typedef int coef_t;
typedef int data_t;
typedef int	acc_t;

void fir(data_t *y, data_t x);