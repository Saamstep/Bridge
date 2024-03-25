
#include "musical_bridge.h"

static FIL waveFile; 

static uint32_t fileLength;
#define AUDIO_BUFFER_SIZE  4096*1
static uint8_t audioBuffer[AUDIO_BUFFER_SIZE];
static uint32_t audioRemainSize;

//WAV Player
static uint32_t samplingFreq;
static uint32_t playerReadBytes;
static bool isFinished=0;

static int numSamples; 



//FFT Function: https://www.math.wustl.edu/~victor/mfmm/fourier/fft.c


void wavAudioRead (char *filename, double *signal){
    WAV_HeaderTypeDef wavHeader;
    uint32_t readBytes = 0;
    //Open WAV fileFR
    if(f_open(&wavFile, filename, FA_READ) != FR_OK){
        return;
    }
    //Read WAV file Header
    f_read(&wavFile, &wavHeader, sizeof(wavHeader), &readBytes);
    //Get audio data size
    fileLength = wavHeader.FileSize;
    //Play the WAV file with frequency specified in header
    samplingFreq = wavHeader.SampleRate;

    f_lseek(&wavFile, 0);
    f_read (&wavFile, &audioBuffer[0], AUDIO_BUFFER_SIZE, &playerReadBytes);
    audioRemainSize = fileLength - playerReadBytes;
    for (size_t i = 0; i < playerReadBytes; i++) {
        signal[i] = (double)audioBuffer[i] / MAX_UINT8_VAL;
    }
    numSamples = playerReadBytes / 2;

}

//Function to downsample filtered audio based on preset downsampling factor
uint32_t downsampleAudio(double *signal){

    uint32_t dsNumSamples = numSamples / DOWNFACTOR;
    //printf("Down Sample Number: %d\n", dsNumSamples);
    //printf("Size of signal : %d\n",(int) (sizeof(&signal) / sizeof(&signal[0])));
    double *tempSignal = (double *)malloc(dsNumSamples * sizeof(double));
    if (tempSignal == NULL) {
        printf("Error: Memory allocation failed in downsampleAudio\n");
        return 0;
    }
    for (int i = 0; i < dsNumSamples; i++) {
        tempSignal[i] = signal[i * DOWNFACTOR];
        //printf("iteration %d complete\n", i);
    }
    for (int i = 0; i < dsNumSamples; i++) {
        signal[i] = tempSignal[i];
    }
    free(tempSignal);
    return dsNumSamples;
}

//Function to segment downsampled audio into smaller 
void analyzeAudio(double *signal, uint32_t numSamples) {
    //printf("Entering audio analysis\n");
    uint32_t numSeg = floor((numSamples / DS_SAMPLES_BIN) / OVERLAP);
    double freq, intensity;
    double totalFreq[numSeg], totalIntensity[numSeg];
    int totalIndex[numSeg];
    //printf("Entering first loop\n");
    for(int i = 0; i < numSeg; i++) {
        int start = 0;
        int end = start + DS_SAMPLES_BIN - 1; 
        if (end <= numSamples) {
            //printf("creating fftw variables\n");
            fftw_complex *in, *out;
            fftw_plan plan;

            in = (fftw_complex *)malloc(DFT_SIZE * sizeof(fftw_complex));
            out = (fftw_complex *)malloc(DFT_SIZE * sizeof(fftw_complex));
            plan = fftw_plan_dft_1d(DFT_SIZE, in, out, FFTW_FORWARD, FFTW_ESTIMATE);

            //printf("Assigning values from signal to fft\n");
            for (int j = 0; j < DS_SAMPLES_BIN; j++){
                in[j][0] = signal[start + j];
                in[j][1] = 0.0;
                //printf("iteration %d complete\n",j);
            }
            //printf("Run FFT\n");
            fftw_execute(plan);

            double mag;
            double sum;
            double max = -1;
            int maxindex;
            //printf("Calculating magnitude\n");
            for (int k = 0; k < DS_SAMPLES_BIN; k++) {
                //mag[k] = sqrtf(((fftOut[k].Re) * (fftOut[k].Re)) + ((fftOut[k].Im) * (fftOut[k].Im)));
                mag = sqrt((out[k][0] * out[k][0]) + (out[k][1] * out[i][1]));
                sum += mag;
                if (mag > max) {
                    max = mag;
                    maxindex = k;
                }
            }
            //if (max != 0) printf("Max = %f and Index = %d\n", max, maxindex);
            freq = maxindex * RESOLUTION;
            intensity = ((max / sum) * 100);
            totalFreq[i] = freq;
            totalIntensity[i] = intensity;
            totalIndex[i] = maxindex;
            //controlLED(freq, intensity);
            start += DATA_ANALYSIS_RATE;
            fftw_destroy_plan(plan);
            fftw_free(in);
            fftw_free(out);
        } else {
            printf("Error Out of Bounds\n");
            return;
        }
        printf("values of freq: %f, intensity: %f, index: %d\n", totalFreq[300], totalIntensity[300], totalIndex[300]);
    }
}


int main() {
    audioRemainSize = 0;
    playerReadBytes = 0;
    char *filename = "Just-Earworms-Outro.wav";
    double *signal = (double *)malloc(MAX_SAMPLES * sizeof(double));
    if (signal == NULL) return 1;
    wavAudioRead(filename, signal);
    printf("all constant values: %d, %d, %d, %d, %f\n", DS_SAMPLE_RATE, RESOLUTION, DS_SAMPLES_BIN, DFT_SIZE, DATA_ANALYSIS_RATE);

    uint32_t dsNumSamples = downsampleAudio(signal);
    if (dsNumSamples == 0) {
          free(signal);
          return 1;
    }
    analyzeAudio(signal, dsNumSamples);

    // printf("Numsamples = %d\n", numSamples);
    // printf("sample rate = %d\n", SAMPLE_RATE);
    
    free(signal);
    return 0;
}

