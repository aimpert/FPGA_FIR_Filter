# FPGA Accelerated FIR Filter

Just one of the basic examples of FPGA Acceleration for a common use-case (following the Paralell Programming for HLS book).

My FIR implementation (FIRFilter.cpp) has three versions:

## Baseline
```
void fir(data_t *y, data_t x) {
    coef_t c[N] = {53, 0, -91, 0, 313, 500, 313, 0, -91, 0,53};
    static data_t shift_reg[N];
    acc_t acc;
    int i;
    acc = 0;
    Shift_Accum_Loop:
    for (i = N - 1; i >= 0; i--) {
        #pragma hls pipeline off
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
```
For those unfamiliar an FIR Filter works by performing convolution on a signal with a set of coefficients to produce a new output. The outer loop is outside of fir(), and so this function is simply taking the current x and its past values in the shift register and summing its individual products with each of the coefficients.

### Synthesis Report
```
    +---------------------+------+------+---------+---------+----------+---------+------+----------+------+---------+-----------+-----------+-----+
    |       Modules       | Issue|      | Latency | Latency | Iteration|         | Trip |          |      |         |           |           |     |
    |       & Loops       | Type | Slack| (cycles)|   (ns)  |  Latency | Interval| Count| Pipelined| BRAM |   DSP   |     FF    |    LUT    | URAM|
    +---------------------+------+------+---------+---------+----------+---------+------+----------+------+---------+-----------+-----------+-----+
    |+ fir                |     -|  0.38|       56|  560.000|         -|       57|     -|        no|     -|  2 (~0%)|  426 (~0%)|  346 (~0%)|    -|
    | o Shift_Accum_Loop  |     -|  7.30|       55|  550.000|         5|        -|    11|        no|     -|        -|          -|          -|    -|
    +---------------------+------+------+---------+---------+----------+---------+------+----------+------+---------+-----------+-----------+-----+
```

With a targetted clock frequency of 10ns, the baseline implementation is running at 56 cycles. We know our algorithm is performing 4 instructions (two reads, an add, and a multiplication), so it is a reasonable assumption to say this can be optimized significantly.

## Pipelined

```
void fir(data_t *y, data_t x) {
    coef_t c[N] = {53, 0, -91, 0, 313, 500, 313, 0, -91, 0,53};
    static data_t shift_reg[N];
    acc_t acc = 0;
    int i;

    #pragma HLS array_partition variable=shift_reg complete
    #pragma HLS array_partition variable=c complete

    Shift_Reg_Loop:
    for (i = N - 1; i > 0; i--) {
        #pragma HLS UNROLL skip_exit_check factor=11
        shift_reg[i] = shift_reg[i - 1];
            
    }
    shift_reg[0] = x; 

    Convolution_4:
    for (i = N - 1; i >= 3; i -= 4) {
        #pragma HLS pipeline II=1
        acc += shift_reg[i] * c[i] + shift_reg[i - 1] * c[i - 1] +
            shift_reg[i - 2] * c[i - 2] + shift_reg[i - 3] * c[i - 3];
    }
    Convolution:
    for (; i >= 0; i--) {
        acc += shift_reg[i] * c[i];
    }

    *y = acc;
}
```
My first set of optimizations was declaring shift_reg and c for **array partitioning**. This breaks up the arrays into smaller chunks for better parallelism on an FPGA.

Another technique was **code hoisting**. On an FPGA, the conditional statements (if/else) would mean more logical hardware implementations, possibly slowing down the algorithm with all these checks.

I then applied **loop fissioning** so I could work on both operations (shift register shifting and convolution) seperately in an attempt to optimize each one differently.

The Shift_Reg_Loop has a full unroll with no pipelining (since it would be all done at the same time), and the Convolution_4 loop has manual unrolling with a pipeline factor = 1.

### Synthesis Report
```
+-------------------------------+------+------+---------+---------+----------+---------+------+----------+------+---------+------------+-----------+-----+
    |            Modules            | Issue|      | Latency | Latency | Iteration|         | Trip |          |      |         |            |           |     |
    |            & Loops            | Type | Slack| (cycles)|   (ns)  |  Latency | Interval| Count| Pipelined| BRAM |   DSP   |     FF     |    LUT    | URAM|
    +-------------------------------+------+------+---------+---------+----------+---------+------+----------+------+---------+------------+-----------+-----+
    |+ fir                          |     -|  0.39|       18|  180.000|         -|       19|     -|        no|     -|  10 (2%)|   2048 (1%)|  755 (~0%)|    -|
    | + fir_Pipeline_Convolution_4  |     -|  0.39|        7|   70.000|         -|        7|     -|        no|     -|   8 (2%)|  1120 (~0%)|  559 (~0%)|    -|
    |  o Convolution_4              |     -|  7.30|        5|   50.000|         5|        1|     2|       yes|     -|        -|           -|          -|    -|
    | + fir_Pipeline_Convolution    |     -|  0.39|        7|   70.000|         -|        7|     -|        no|     -|  2 (~0%)|   281 (~0%)|  165 (~0%)|    -|
    |  o Convolution                |     -|  7.30|        5|   50.000|         4|        1|     3|       yes|     -|        -|           -|          -|    -|
    +-------------------------------+------+------+---------+---------+----------+---------+------+----------+------+---------+------------+-----------+-----+
```
This implementation ran in a projected 18 cycles. As expected there was a significant increase in resource utilization due to the overhead of pipelining as well as loop fission.

## Full Unroll

```
void fir(data_t *y, data_t x) {
    coef_t c[N] = {53, 0, -91, 0, 313, 500, 313, 0, -91, 0,53};
    static data_t shift_reg[N];
    acc_t acc = 0;
    int i;

    #pragma HLS array_partition variable=shift_reg complete
    #pragma HLS array_partition variable=c complete

    Shift_And_Convolution: for (int i = N-1; i > 0; i--) {
        #pragma HLS UNROLL
        shift_reg[i] = shift_reg[i-1];
        acc += shift_reg[i] * c[i];
    }

    shift_reg[0] = x; 
    acc += shift_reg[0] * c[0];

    *y = acc;
}
```
The best optimization for this filter is simply a full unroll. We only have 11 coefficients making this an 11 tap filter. We can see that the hardware can feasibly perform all operations in parallel since it won't be taking many resources. With no pipelining there is no overhead, and the control logic is much simpler. If there were siginificantly more coefficients, it would likely be that the pipelined version would be more scalable.

### Synthesis Report

```
    +--------+------+------+---------+--------+----------+---------+------+----------+------+---------+-----------+-----------+-----+
    | Modules| Issue|      | Latency | Latency| Iteration|         | Trip |          |      |         |           |           |     |
    | & Loops| Type | Slack| (cycles)|  (ns)  |  Latency | Interval| Count| Pipelined| BRAM |   DSP   |     FF    |    LUT    | URAM|
    +--------+------+------+---------+--------+----------+---------+------+----------+------+---------+-----------+-----------+-----+
    |+ fir   |     -|  0.38|        4|  40.000|         -|        5|     -|        no|     -|  2 (~0%)|  810 (~0%)|  557 (~0%)|    -|
    +--------+------+------+---------+--------+----------+---------+------+----------+------+---------+-----------+-----------+-----+
```

As such, the implementation only takes 4 cycles. As mentioned earlier we had 4 instructions to take care of, and we achieved it by performing 4 cycles parallelizing all 11 iterations.
