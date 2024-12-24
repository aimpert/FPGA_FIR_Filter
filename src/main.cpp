/*
	Filename: fir_test.h
		FIR lab wirtten for WES/CSE237C class at UCSD.
		Testbench file
		Calls fir() function from fir.cpp
		Compares the output from fir() with out.gold.dat
*/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "../include/FIRFilter.h"

using namespace std;

int main () {
    const int   SAMPLES=600;
    FILE    *fp, *fin;

    data_t signal, output;
    int i;
    signal = 0;

    string out = ROOT_DIR + "output.dat";
    string in = ROOT_DIR + "input.dat";
    fin=fopen(in.c_str(),"r");
    fp=fopen(out.c_str(),"w");

    for (i=0;i<SAMPLES;i++) {
        fscanf(fin,"%d",&signal);
        //Call the HLS block
        fir(&output,signal);
        // Save the results.
        fprintf(fp,"%d\n",output);
        printf("%i %d %d\n",i,signal,output);
    }

    fclose(fp);
    fclose(fin);
    string command = "diff -w " + ROOT_DIR + "output.dat " + ROOT_DIR + "out.gold.dat";
    //Comparing results with the golden output.
    printf ("Comparing against output data \n");
    if (system(command.c_str())) {
        fprintf(stdout, "*******************************************\n");
        fprintf(stdout, "FAIL: Output DOES NOT match the golden output\n");
        fprintf(stdout, "*******************************************\n");
        return 1;
    } else {
        fprintf(stdout, "*******************************************\n");
        fprintf(stdout, "PASS: The output matches the golden output!\n");
        fprintf(stdout, "*******************************************\n");
        return 0;
    }

}

