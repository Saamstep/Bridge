/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: downSampleAudio.h
 *
 * MATLAB Coder version            : 5.3
 * C/C++ source code generated on  : 13-Mar-2024 22:17:54
 */

#ifndef DOWNSAMPLEAUDIO_H
#define DOWNSAMPLEAUDIO_H

/* Include Files */
#include "downSampleAudio_types.h"
#include "./codegen/lib/analyzeAudio/rtwtypesfft.h"
#include <stddef.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Function Declarations */
extern void downSampleAudio(double *audioData, double downFactor,
                            emxArray_real_T *downAudio);

#ifdef __cplusplus
}
#endif

#endif
/*
 * File trailer for downSampleAudio.h
 *
 * [EOF]
 */