#include "bridge2.h"

//void controlLED(double *freq, double *intensity) {
//    printf("Frequency of Most Dominant Note in current segment: %.2lf\n", *freq);
//    printf("Percent of LEDs lit up in LED strip: %.2lf\n", *intensity);
//    if (*intensity <= 33 && *intensity > 0) {
//        printf("Color of LEDs in strip: RED\n");
//    } 
//    else if (*intensity > 67) {
//        printf("Color of LEDs in strip: PURPLE\n");
//    } else {
//        printf("Color of LEDs in strip: BLUE\n");
//    }
//}

int audioProcess(double *audioFile, double *freq, double *intensity) {
    //char *filename = "Just-Earworms-Outro.wav";
    //audio signal;

    //float *signal = (float *)malloc(MAX_SAMPLES * sizeof(float));
    emxArray_real_T *downAudio;
    downSampleAudio(audioFile, DOWNFACTOR, downAudio);

    int nbeg = 0;

    while ((nbeg + DS_SAMPLES_BIN) < sizeof(downAudio->data)){
        double *segment = NULL;
        for (int i = nbeg; i < nbeg + DS_SAMPLES_BIN; i++){
            segment[i] = downAudio->data[i];
        }
        analyzeAudio(segment, DS_SAMPLES_BIN, RESOLUTION, freq, intensity);
        nbeg += DATA_ANALYSIS_RATE;
    }
    return 0;
}
