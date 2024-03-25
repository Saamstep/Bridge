/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: analyzeAudio.c
 *
 * MATLAB Coder version            : 5.6
 * C/C++ source code generated on  : 14-Mar-2024 00:00:41
 */

/* Include Files */
#include "analyzeAudio.h"
#include "FFTImplementationCallback.h"
#include "analyzeAudio_emxutil.h"
#include "analyzeAudio_types.h"
#include "rt_nonfinite.h"
#include "rt_nonfinite.h"
#include <math.h>

/* Function Declarations */
static double rt_hypotd_snf(double u0, double u1);

/* Function Definitions */
/*
 * Arguments    : double u0
 *                double u1
 * Return Type  : double
 */
static double rt_hypotd_snf(double u0, double u1)
{
  double a;
  double b;
  double y;
  a = fabs(u0);
  b = fabs(u1);
  if (a < b) {
    a /= b;
    y = b * sqrt(a * a + 1.0);
  } else if (a > b) {
    b /= a;
    y = a * sqrt(b * b + 1.0);
  } else if (rtIsNaN(b)) {
    y = rtNaN;
  } else {
    y = a * 1.4142135623730951;
  }
  return y;
}

/*
 * Arguments    : const double audioSeg[242555]
 *                double segLength
 *                double binsize
 *                double *freq
 *                double *intensity
 * Return Type  : void
 */
void analyzeAudio(double *audioSeg, double segLength,
                  double binsize, double *freq, double *intensity)
{
  emxArray_creal_T *fftseg;
  emxArray_real_T *costab;
  emxArray_real_T *costab1q;
  emxArray_real_T *magsq;
  emxArray_real_T *sintab;
  emxArray_real_T *sintabinv;
  creal_T *fftseg_data;
  double temp_im;
  double temp_re;
  double tempmax;
  double *costab1q_data;
  double *costab_data;
  double *sintab_data;
  double *sintabinv_data;
  int i;
  int iDelta;
  int ju;
  int k;
  int pmax;
  int pmin;
  int pow2p;
  unsigned int tempindex;
  tempmax = segLength * 2.0;
  emxInit_creal_T(&fftseg);
  if ((int)tempmax == 0) {
    pmax = (int)tempmax;
    pow2p = fftseg->size[0];
    fftseg->size[0] = (int)tempmax;
    emxEnsureCapacity_creal_T(fftseg, pow2p);
    fftseg_data = fftseg->data;
    for (pow2p = 0; pow2p < pmax; pow2p++) {
      fftseg_data[pow2p].re = 0.0;
      fftseg_data[pow2p].im = 0.0;
    }
  } else {
    bool useRadix2;
    useRadix2 =
        (((int)tempmax > 0) && (((int)tempmax & ((int)tempmax - 1)) == 0));
    pmin = 1;
    if (useRadix2) {
      pmax = (int)tempmax;
    } else {
      if ((int)tempmax > 0) {
        iDelta = ((int)tempmax + (int)tempmax) - 1;
        pmax = 31;
        if (iDelta <= 1) {
          pmax = 0;
        } else {
          bool exitg1;
          pmin = 0;
          exitg1 = false;
          while ((!exitg1) && (pmax - pmin > 1)) {
            k = (pmin + pmax) >> 1;
            pow2p = 1 << k;
            if (pow2p == iDelta) {
              pmax = k;
              exitg1 = true;
            } else if (pow2p > iDelta) {
              pmax = k;
            } else {
              pmin = k;
            }
          }
        }
        pmin = 1 << pmax;
      }
      pmax = pmin;
    }
    temp_im = 6.2831853071795862 / (double)pmax;
    iDelta = pmax / 2 / 2;
    emxInit_real_T(&costab1q, 2);
    pow2p = costab1q->size[0] * costab1q->size[1];
    costab1q->size[0] = 1;
    costab1q->size[1] = iDelta + 1;
    emxEnsureCapacity_real_T(costab1q, pow2p);
    costab1q_data = costab1q->data;
    costab1q_data[0] = 1.0;
    pmax = iDelta / 2 - 1;
    for (k = 0; k <= pmax; k++) {
      costab1q_data[k + 1] = cos(temp_im * ((double)k + 1.0));
    }
    pow2p = pmax + 2;
    pmax = iDelta - 1;
    for (k = pow2p; k <= pmax; k++) {
      costab1q_data[k] = sin(temp_im * (double)(iDelta - k));
    }
    costab1q_data[iDelta] = 0.0;
    emxInit_real_T(&costab, 2);
    emxInit_real_T(&sintab, 2);
    emxInit_real_T(&sintabinv, 2);
    if (!useRadix2) {
      iDelta = costab1q->size[1] - 1;
      pmax = (costab1q->size[1] - 1) << 1;
      pow2p = costab->size[0] * costab->size[1];
      costab->size[0] = 1;
      costab->size[1] = pmax + 1;
      emxEnsureCapacity_real_T(costab, pow2p);
      costab_data = costab->data;
      pow2p = sintab->size[0] * sintab->size[1];
      sintab->size[0] = 1;
      sintab->size[1] = pmax + 1;
      emxEnsureCapacity_real_T(sintab, pow2p);
      sintab_data = sintab->data;
      costab_data[0] = 1.0;
      sintab_data[0] = 0.0;
      pow2p = sintabinv->size[0] * sintabinv->size[1];
      sintabinv->size[0] = 1;
      sintabinv->size[1] = pmax + 1;
      emxEnsureCapacity_real_T(sintabinv, pow2p);
      sintabinv_data = sintabinv->data;
      for (k = 0; k < iDelta; k++) {
        sintabinv_data[k + 1] = costab1q_data[(iDelta - k) - 1];
      }
      pow2p = costab1q->size[1];
      for (k = pow2p; k <= pmax; k++) {
        sintabinv_data[k] = costab1q_data[k - iDelta];
      }
      for (k = 0; k < iDelta; k++) {
        costab_data[k + 1] = costab1q_data[k + 1];
        sintab_data[k + 1] = -costab1q_data[(iDelta - k) - 1];
      }
      pow2p = costab1q->size[1];
      for (k = pow2p; k <= pmax; k++) {
        costab_data[k] = -costab1q_data[pmax - k];
        sintab_data[k] = -costab1q_data[k - iDelta];
      }
    } else {
      iDelta = costab1q->size[1] - 1;
      pmax = (costab1q->size[1] - 1) << 1;
      pow2p = costab->size[0] * costab->size[1];
      costab->size[0] = 1;
      costab->size[1] = pmax + 1;
      emxEnsureCapacity_real_T(costab, pow2p);
      costab_data = costab->data;
      pow2p = sintab->size[0] * sintab->size[1];
      sintab->size[0] = 1;
      sintab->size[1] = pmax + 1;
      emxEnsureCapacity_real_T(sintab, pow2p);
      sintab_data = sintab->data;
      costab_data[0] = 1.0;
      sintab_data[0] = 0.0;
      for (k = 0; k < iDelta; k++) {
        costab_data[k + 1] = costab1q_data[k + 1];
        sintab_data[k + 1] = -costab1q_data[(iDelta - k) - 1];
      }
      pow2p = costab1q->size[1];
      for (k = pow2p; k <= pmax; k++) {
        costab_data[k] = -costab1q_data[pmax - k];
        sintab_data[k] = -costab1q_data[k - iDelta];
      }
      sintabinv->size[0] = 1;
      sintabinv->size[1] = 0;
    }
    emxFree_real_T(&costab1q);
    if (useRadix2) {
      pmax = (int)tempmax;
      pow2p = fftseg->size[0];
      fftseg->size[0] = (int)tempmax;
      emxEnsureCapacity_creal_T(fftseg, pow2p);
      fftseg_data = fftseg->data;
      if ((int)tempmax > 242555) {
        pow2p = fftseg->size[0];
        fftseg->size[0] = (int)tempmax;
        emxEnsureCapacity_creal_T(fftseg, pow2p);
        fftseg_data = fftseg->data;
        for (pow2p = 0; pow2p < pmax; pow2p++) {
          fftseg_data[pow2p].re = 0.0;
          fftseg_data[pow2p].im = 0.0;
        }
      }
      if ((int)tempmax != 1) {
        c_FFTImplementationCallback_doH(audioSeg, fftseg, (int)tempmax, costab,
                                        sintab);
        fftseg_data = fftseg->data;
      } else {
        double temp_re_tmp;
        int iDelta2;
        int iheight;
        int nRowsD2;
        if ((int)tempmax >= 242555) {
          pmin = 242553;
        } else {
          pmin = (int)tempmax - 2;
        }
        pow2p = (int)tempmax - 2;
        nRowsD2 = (int)tempmax / 2;
        k = nRowsD2 / 2;
        pmax = 0;
        ju = 0;
        for (i = 0; i <= pmin; i++) {
          fftseg_data[pmax].re = audioSeg[i];
          fftseg_data[pmax].im = 0.0;
          iDelta = (int)tempmax;
          useRadix2 = true;
          while (useRadix2) {
            iDelta >>= 1;
            ju ^= iDelta;
            useRadix2 = ((ju & iDelta) == 0);
          }
          pmax = ju;
        }
        if (pmin < 0) {
          pmin = 0;
        } else {
          pmin++;
        }
        fftseg_data[pmax].re = audioSeg[pmin];
        fftseg_data[pmax].im = 0.0;
        if ((int)tempmax > 1) {
          for (i = 0; i <= pow2p; i += 2) {
            temp_re_tmp = fftseg_data[i + 1].re;
            tempmax = fftseg_data[i + 1].im;
            temp_re = fftseg_data[i].re;
            temp_im = fftseg_data[i].im;
            fftseg_data[i + 1].re = temp_re - temp_re_tmp;
            fftseg_data[i + 1].im = temp_im - tempmax;
            fftseg_data[i].re = temp_re + temp_re_tmp;
            fftseg_data[i].im = temp_im + tempmax;
          }
        }
        iDelta = 2;
        iDelta2 = 4;
        iheight = ((k - 1) << 2) + 1;
        while (k > 0) {
          for (i = 0; i < iheight; i += iDelta2) {
            pmax = i + iDelta;
            temp_re = fftseg_data[pmax].re;
            temp_im = fftseg_data[pmax].im;
            fftseg_data[pmax].re = fftseg_data[i].re - temp_re;
            fftseg_data[pmax].im = fftseg_data[i].im - temp_im;
            fftseg_data[i].re += temp_re;
            fftseg_data[i].im += temp_im;
          }
          pmax = 1;
          for (pmin = k; pmin < nRowsD2; pmin += k) {
            double twid_im;
            double twid_re;
            twid_re = costab_data[pmin];
            twid_im = sintab_data[pmin];
            i = pmax;
            pow2p = pmax + iheight;
            while (i < pow2p) {
              ju = i + iDelta;
              temp_re_tmp = fftseg_data[ju].im;
              tempmax = fftseg_data[ju].re;
              temp_re = twid_re * tempmax - twid_im * temp_re_tmp;
              temp_im = twid_re * temp_re_tmp + twid_im * tempmax;
              fftseg_data[ju].re = fftseg_data[i].re - temp_re;
              fftseg_data[ju].im = fftseg_data[i].im - temp_im;
              fftseg_data[i].re += temp_re;
              fftseg_data[i].im += temp_im;
              i += iDelta2;
            }
            pmax++;
          }
          k /= 2;
          iDelta = iDelta2;
          iDelta2 += iDelta2;
          iheight -= iDelta;
        }
      }
    } else {
      c_FFTImplementationCallback_dob(audioSeg, pmin, (int)tempmax, costab,
                                      sintab, sintabinv, fftseg);
      fftseg_data = fftseg->data;
    }
    emxFree_real_T(&sintabinv);
    emxFree_real_T(&sintab);
    emxFree_real_T(&costab);
  }
  pmax = fftseg->size[0];
  emxInit_real_T(&magsq, 1);
  pow2p = magsq->size[0];
  magsq->size[0] = fftseg->size[0];
  emxEnsureCapacity_real_T(magsq, pow2p);
  costab1q_data = magsq->data;
  for (k = 0; k < pmax; k++) {
    costab1q_data[k] = rt_hypotd_snf(fftseg_data[k].re, fftseg_data[k].im);
  }
  emxFree_creal_T(&fftseg);
  pmax = magsq->size[0];
  for (pow2p = 0; pow2p < pmax; pow2p++) {
    tempmax = costab1q_data[pow2p];
    costab1q_data[pow2p] = tempmax * tempmax;
  }
  tempmax = 0.0;
  tempindex = 0U;
  pow2p = (int)(0.5 * (segLength * 2.0) + 1.0);
  for (i = 0; i < pow2p; i++) {
    if (costab1q_data[i] > tempmax) {
      tempmax = costab1q_data[i];
      tempindex = (unsigned int)i + 1U;
    }
  }
  if (magsq->size[0] == 0) {
    temp_im = 0.0;
  } else {
    if (magsq->size[0] <= 1024) {
      pmax = magsq->size[0];
      ju = 0;
      pow2p = 1;
    } else {
      pmax = 1024;
      pow2p = (int)((unsigned int)magsq->size[0] >> 10);
      ju = magsq->size[0] - (pow2p << 10);
      if (ju > 0) {
        pow2p++;
      } else {
        ju = 1024;
      }
    }
    temp_im = costab1q_data[0];
    for (k = 2; k <= pmax; k++) {
      temp_im += costab1q_data[k - 1];
    }
    for (iDelta = 2; iDelta <= pow2p; iDelta++) {
      pmax = (iDelta - 1) << 10;
      temp_re = costab1q_data[pmax];
      if (iDelta == pow2p) {
        pmin = ju;
      } else {
        pmin = 1024;
      }
      for (k = 2; k <= pmin; k++) {
        temp_re += costab1q_data[(pmax + k) - 1];
      }
      temp_im += temp_re;
    }
  }
  emxFree_real_T(&magsq);
  *intensity = tempmax / temp_im * 100.0;
  *freq = (double)tempindex * binsize / 2.0;
}

/*
 * File trailer for analyzeAudio.c
 *
 * [EOF]
 */
