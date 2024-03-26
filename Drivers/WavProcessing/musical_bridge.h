#ifndef MUSICAL_BRIDGE_H
#define MUSICAL_BRIDGE_H

#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include "arm_math.h"

#define MAX_DURATION 360
#define DOWNFACTOR 8
#define SAMPLES_BIN 6000
#define OVERLAP 0.25
#define MAX_SAMPLES 1728000
#define SAMPLE_RATE 48000
#define DS_SAMPLE_RATE 6000
#define RESOLUTION 8
#define DS_SAMPLES_BIN 512 
#define DFT_SIZE 1024
#define DATA_ANALYSIS_RATE 187.5

// Function prototypes
float32_t divideAndMax (float32_t *data, int start, int end);
uint32_t downsampleAudio(float32_t *signal, uint32_t numSamples);
void intensityInit(void);
void analyzeAudio(float32_t *signal, float32_t *intensity);
#endif
