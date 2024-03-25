/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: downSampleAudio_emxAPI.h
 *
 * MATLAB Coder version            : 5.3
 * C/C++ source code generated on  : 13-Mar-2024 22:17:54
 */

#ifndef DOWNSAMPLEAUDIO_EMXAPI_H
#define DOWNSAMPLEAUDIO_EMXAPI_H

/* Include Files */
#include "downSampleAudio_types.h"
#include "./codegen/lib/analyzeAudio/rtwtypesfft.h"
#include <stddef.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Function Declarations */
extern emxArray_real_T *emxCreateND_real_T(int numDimensions, const int *size);

extern emxArray_real_T *
emxCreateWrapperND_real_T(double *data, int numDimensions, const int *size);

extern emxArray_real_T *emxCreateWrapper_real_T(double *data, int rows,
                                                int cols);

extern emxArray_real_T *emxCreate_real_T(int rows, int cols);

extern void emxDestroyArray_real_T(emxArray_real_T *emxArray);

extern void emxInitArray_real_T(emxArray_real_T **pEmxArray, int numDimensions);

#ifdef __cplusplus
}
#endif

#endif
/*
 * File trailer for downSampleAudio_emxAPI.h
 *
 * [EOF]
 */
