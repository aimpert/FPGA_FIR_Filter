#include "AudioFile.h"
#include "FIRFilter.h"

using namespace std;

int main() {
    AudioFile<float> audioFile;
    audioFile.load ("sampleAudio.wav");
    audioFile.printSummary();

    int numSamples = audioFile.getNumSamplesPerChannel();
    // coefficients generated with MatLab (okay generated by ChatGPT using MatLab)
    // these are (supposedly) representing a cut-off frequency of 400Hz
    float coefficients[11] = {0.01444,0.03044,0.07242,0.12450,0.16675,0.18291,0.16675,0.12450,0.07242,0.03044,0.01444};

    FIRFilter* lowPass = new FIRFilter(audioFile.samples[0].data(), numSamples, coefficients, 11);
    lowPass->applyFilter();

    float* filteredSamples = lowPass->getOutput();

    for (int i = 0; i < numSamples; i++)
    {   
	    audioFile.samples[0][i] = filteredSamples[i];
    }

    lowPass->writeData(FIRData::all);

    audioFile.save("audioFile.wav");

    delete lowPass;

    return 0;

}
