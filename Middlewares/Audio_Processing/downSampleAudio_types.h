/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: downSampleAudio_types.h
 *
 * MATLAB Coder version            : 5.3
 * C/C++ source code generated on  : 13-Mar-2024 22:17:54
 */

#ifndef DOWNSAMPLEAUDIO_TYPES_H
#define DOWNSAMPLEAUDIO_TYPES_H

/* Include Files */
#include "./codegen/lib/analyzeAudio/rtwtypesfft.h"

/* Type Definitions */
#ifndef struct_emxArray_real_T
#define struct_emxArray_real_T
struct emxArray_real_T {
  double *data;
  int *size;
  int allocatedSize;
  int numDimensions;
  boolean_T canFreeData;
};
#endif /* struct_emxArray_real_T */
#ifndef typedef_emxArray_real_T
#define typedef_emxArray_real_T
typedef struct emxArray_real_T emxArray_real_T;
#endif /* typedef_emxArray_real_T */

#endif
/*
 * File trailer for downSampleAudio_types.h
 *
 * [EOF]
 */
