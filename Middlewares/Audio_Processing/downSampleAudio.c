/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: downSampleAudio.c
 *
 * MATLAB Coder version            : 5.3
 * C/C++ source code generated on  : 13-Mar-2024 22:17:54
 */

/* Include Files */
#include "downSampleAudio.h"
#include "downSampleAudio_emxutil.h"
#include "downSampleAudio_types.h"
#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

/* Function Declarations */
static int div_s32_floor(int numerator, int denominator);

/* Function Definitions */
/*
 * Arguments    : int numerator
 *                int denominator
 * Return Type  : int
 */
static int div_s32_floor(int numerator, int denominator)
{
  unsigned int absDenominator;
  unsigned int absNumerator;
  int quotient;
  unsigned int tempAbsQuotient;
  boolean_T quotientNeedsNegation;
  if (denominator == 0) {
    if (numerator >= 0) {
      quotient = MAX_int32_T;
    } else {
      quotient = MIN_int32_T;
    }
  } else {
    if (numerator < 0) {
      absNumerator = ~(unsigned int)numerator + 1U;
    } else {
      absNumerator = (unsigned int)numerator;
    }
    if (denominator < 0) {
      absDenominator = ~(unsigned int)denominator + 1U;
    } else {
      absDenominator = (unsigned int)denominator;
    }
    quotientNeedsNegation = ((numerator < 0) != (denominator < 0));
    tempAbsQuotient = absNumerator / absDenominator;
    if (quotientNeedsNegation) {
      absNumerator %= absDenominator;
      if (absNumerator > 0U) {
        tempAbsQuotient++;
      }
      quotient = -(int)tempAbsQuotient;
    } else {
      quotient = (int)tempAbsQuotient;
    }
  }
  return quotient;
}

/*
 * Arguments    : const double audioData[476064]
 *                double downFactor
 *                emxArray_real_T *downAudio
 * Return Type  : void
 */
void downSampleAudio(double *audioData, double downFactor,
                     emxArray_real_T *downAudio)
{
  double *downAudio_data;
  int i;
  int i1;
  int loop_ub;
  if ((downFactor == 0.0) || (0.0 > downFactor)) {
    i = 1;
    i1 = -1;
  } else {
    i = (int)downFactor;
    i1 = sizeof(audioData)/sizeof(audioData[0]);
  }
  loop_ub = div_s32_floor(i1, i);
  i1 = downAudio->size[0];
  downAudio->size[0] = loop_ub + 1;
  emxEnsureCapacity_real_T_Down(downAudio, i1);
  downAudio_data = downAudio->data;
  for (i1 = 0; i1 <= loop_ub; i1++) {
    downAudio_data[i1] = audioData[i * i1];
  }
}

/*
 * File trailer for downSampleAudio.c
 *
 * [EOF]
 */
