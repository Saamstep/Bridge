#ifndef BRIDGE2_H
#define BRIDGE2_H

#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include "downSampleAudio.h"
#include "downSampleAudio_emxutil.h"
#include "downSampleAudio_types.h"
#include "./codegen/lib/analyzeAudio/analyzeAudio.h"
#include "./codegen/lib/analyzeAudio/FFTImplementationCallback.h"
#include "./codegen/lib/analyzeAudio/analyzeAudio_emxutil.h"
#include "./codegen/lib/analyzeAudio/analyzeAudio_types.h"
#include "./codegen/lib/analyzeAudio/rt_nonfinite.h"
#include "./codegen/lib/analyzeAudio/rt_nonfinite.h"

#define MAX_DURATION 360
#define DOWNFACTOR 8
#define SAMPLES_BIN 6000
#define OVERLAP 0.25
#define MAX_SAMPLES 1728000
#define DS_SAMPLE_RATE 6000
#define RESOLUTION 8
#define DS_SAMPLES_BIN 750
#define DFT_SIZE 1500
#define DATA_ANALYSIS_RATE 187.5

int audioProcess(double *audioFile, double*freq, double *intensity);

#endif


