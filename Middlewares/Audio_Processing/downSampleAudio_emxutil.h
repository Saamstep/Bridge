/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: downSampleAudio_emxutil.h
 *
 * MATLAB Coder version            : 5.3
 * C/C++ source code generated on  : 13-Mar-2024 22:17:54
 */

#ifndef DOWNSAMPLEAUDIO_EMXUTIL_H
#define DOWNSAMPLEAUDIO_EMXUTIL_H

/* Include Files */
#include "downSampleAudio_types.h"
#include "./codegen/lib/analyzeAudio/rtwtypesfft.h"
#include <stddef.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Function Declarations */
extern void emxEnsureCapacity_real_T_Down(emxArray_real_T *emxArray, int oldNumel);

extern void emxFree_real_T_Down(emxArray_real_T **pEmxArray);

extern void emxInit_real_T_Down(emxArray_real_T **pEmxArray, int numDimensions);

#ifdef __cplusplus
}
#endif

#endif
/*
 * File trailer for downSampleAudio_emxutil.h
 *
 * [EOF]
 */
