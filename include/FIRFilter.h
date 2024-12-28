#include <stddef.h>
#include <iostream>
#include <vector>
#include <fstream>
#include "ap_int.h"

const std::string ROOT_DIR = "../../../../../";

const int N=11;

typedef ap_int<10> coef_t;
typedef int data_t;
typedef int	acc_t;

void fir(data_t *y, data_t x);