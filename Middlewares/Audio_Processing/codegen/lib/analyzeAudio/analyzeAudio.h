/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: analyzeAudio.h
 *
 * MATLAB Coder version            : 5.6
 * C/C++ source code generated on  : 14-Mar-2024 00:00:41
 */

#ifndef ANALYZEAUDIO_H
#define ANALYZEAUDIO_H

/* Include Files */
#include "rtwtypesfft.h"
#include <stddef.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Function Declarations */
extern void analyzeAudio(double *audioSeg, double segLength,
                         double binsize, double *freq, double *intensity);

#ifdef __cplusplus
}
#endif

#endif
/*
 * File trailer for analyzeAudio.h
 *
 * [EOF]
 */
