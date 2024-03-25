#ifndef MUSICAL_BRIDGE_H
#define MUSICAL_BRIDGE_H

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "waveReader.h"

typedef struct { 
    float Re; 
    float Im; 
} complex;

typedef struct {
	uint32_t samples;
	float *data;
} audio;


// Function prototypes
bool wavAudioRead (char *filename, audio *signal);
uint32_t downsampleAudio(float *signal);
void analyzeAudio(float *signal, uint32_t numSamples);
void fft(complex *signal, int N, complex *output);
void controlLED(float freq, float intensity);
#endif