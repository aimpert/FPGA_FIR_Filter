#include "../include/AudioFile.h"
#include "../include/FIRFilter.h"
#include <chrono>

using namespace std;

int main() {
    AudioFile<float> audioFile;
    audioFile.load (ROOT_DIR + "sampleAudio.wav");
    audioFile.printSummary();
    FILE *fp;
    data_t signal = 0.0 , output = 0.0;

    const int SAMPLES = audioFile.getNumSamplesPerChannel();


    fp=fopen("output.dat","w");
    auto totalTime = std::chrono::microseconds::zero();

    for (int i = 0; i < SAMPLES; i++) {
    auto start = chrono::high_resolution_clock::now();
      fir(&output, audioFile.samples[0].data()[i]);
      auto end = std::chrono::high_resolution_clock::now();
      //audioFile.samples[0][i] = output;
      fprintf(fp,"%f\n",output);
      //printf("%i %f %f\n",i,signal,output);
      totalTime += std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    }

    double avgTime = totalTime.count() / static_cast<double>(SAMPLES);
    std::cout << "Average Execution Time (C-sim): " << avgTime << " microseconds" << std::endl;
    
    fclose(fp);

    // convolution(audioFile.samples[0].data(), coefficients, output);
    // convolution_sw(audioFile.samples[0].data(), numSamples, coefficients, 11, output);

    // if we want to hear the audio!
    // for (int i = 0; i < numSamples; i++)
    // {   
	//     audioFile.samples[0][i] = filteredSamples[i];
    // }
    //audioFile.save("audioFile.wav");

    //writeData(option::output, output, numSamples); // write to output.dat

    string command = "diff -w " + ROOT_DIR + "output.dat " + ROOT_DIR + "output.gold.dat";
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
