#ifndef MUSICAL_BRIDGE_H
#define MUSICAL_BRIDGE_H

#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <fftw3.h>
#include "waveReader.h"

#define MAX_DURATION 360
#define DOWNFACTOR 8
#define SAMPLES_BIN 6000
#define OVERLAP 0.25
#define MAX_SAMPLES 1728000
#define SAMPLE_RATE 48000
#define DS_SAMPLE_RATE 6000
#define RESOLUTION 8
#define DS_SAMPLES_BIN 750 
#define DFT_SIZE 1500
#define DATA_ANALYSIS_RATE 187.5
#define MAX_UINT8_VAL 255.0

typedef struct
{
  uint32_t   ChunkID;       /* 0 */
  uint32_t   FileSize;      /* 4 */
  uint32_t   FileFormat;    /* 8 */
  uint32_t   SubChunk1ID;   /* 12 */
  uint32_t   SubChunk1Size; /* 16*/
  uint16_t   AudioFormat;   /* 20 */
  uint16_t   NbrChannels;   /* 22 */
  uint32_t   SampleRate;    /* 24 */

  uint32_t   ByteRate;      /* 28 */
  uint16_t   BlockAlign;    /* 32 */
  uint16_t   BitPerSample;  /* 34 */
  uint32_t   SubChunk2ID;   /* 36 */
  uint32_t   SubChunk2Size; /* 40 */

}WAV_HeaderTypeDef;

// Function prototypes
void wavAudioRead (char *filename, double *signal);
uint32_t downsampleAudio(double *signal);
void analyzeAudio(double *signal, uint32_t numSamples);
#endif