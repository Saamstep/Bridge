/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: _coder_analyzeAudio_api.h
 *
 * MATLAB Coder version            : 5.6
 * C/C++ source code generated on  : 14-Mar-2024 00:00:41
 */

#ifndef _CODER_ANALYZEAUDIO_API_H
#define _CODER_ANALYZEAUDIO_API_H

/* Include Files */
#include "emlrt.h"
#include "tmwtypes.h"
#include <string.h>

/* Variable Declarations */
extern emlrtCTX emlrtRootTLSGlobal;
extern emlrtContext emlrtContextGlobal;

#ifdef __cplusplus
extern "C" {
#endif

/* Function Declarations */
void analyzeAudio(real_T audioSeg[242555], real_T segLength, real_T binsize,
                  real_T *freq, real_T *intensity);

void analyzeAudio_api(const mxArray *const prhs[3], int32_T nlhs,
                      const mxArray *plhs[2]);

void analyzeAudio_atexit(void);

void analyzeAudio_initialize(void);

void analyzeAudio_terminate(void);

void analyzeAudio_xil_shutdown(void);

void analyzeAudio_xil_terminate(void);

#ifdef __cplusplus
}
#endif

#endif
/*
 * File trailer for _coder_analyzeAudio_api.h
 *
 * [EOF]
 */
