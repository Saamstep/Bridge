/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: FFTImplementationCallback.c
 *
 * MATLAB Coder version            : 5.6
 * C/C++ source code generated on  : 14-Mar-2024 00:00:41
 */

/* Include Files */
#include "FFTImplementationCallback.h"
#include "analyzeAudio_emxutil.h"
#include "analyzeAudio_types.h"
#include "rt_nonfinite.h"
#include <math.h>

/* Function Declarations */
static void c_FFTImplementationCallback_r2b(const emxArray_creal_T *x,
                                            int unsigned_nRows,
                                            const emxArray_real_T *costab,
                                            const emxArray_real_T *sintab,
                                            emxArray_creal_T *y);

static void d_FFTImplementationCallback_doH(
    const double x[242555], emxArray_creal_T *y, int nRows, int nfft,
    const emxArray_creal_T *wwc, const emxArray_real_T *costab,
    const emxArray_real_T *sintab, const emxArray_real_T *costabinv,
    const emxArray_real_T *sintabinv);

/* Function Definitions */
/*
 * Arguments    : const emxArray_creal_T *x
 *                int unsigned_nRows
 *                const emxArray_real_T *costab
 *                const emxArray_real_T *sintab
 *                emxArray_creal_T *y
 * Return Type  : void
 */
static void c_FFTImplementationCallback_r2b(const emxArray_creal_T *x,
                                            int unsigned_nRows,
                                            const emxArray_real_T *costab,
                                            const emxArray_real_T *sintab,
                                            emxArray_creal_T *y)
{
  const creal_T *x_data;
  creal_T *y_data;
  const double *costab_data;
  const double *sintab_data;
  double temp_im;
  double temp_re;
  double temp_re_tmp;
  double twid_re;
  int i;
  int iDelta;
  int iDelta2;
  int iheight;
  int ihi;
  int iy;
  int j;
  int ju;
  int k;
  int nRowsD2;
  sintab_data = sintab->data;
  costab_data = costab->data;
  x_data = x->data;
  iy = y->size[0];
  y->size[0] = unsigned_nRows;
  emxEnsureCapacity_creal_T(y, iy);
  y_data = y->data;
  if (unsigned_nRows > x->size[0]) {
    iy = y->size[0];
    y->size[0] = unsigned_nRows;
    emxEnsureCapacity_creal_T(y, iy);
    y_data = y->data;
    for (iy = 0; iy < unsigned_nRows; iy++) {
      y_data[iy].re = 0.0;
      y_data[iy].im = 0.0;
    }
  }
  j = x->size[0];
  if (j > unsigned_nRows) {
    j = unsigned_nRows;
  }
  ihi = unsigned_nRows - 2;
  nRowsD2 = unsigned_nRows / 2;
  k = nRowsD2 / 2;
  iy = 0;
  ju = 0;
  for (i = 0; i <= j - 2; i++) {
    bool tst;
    y_data[iy] = x_data[i];
    iy = unsigned_nRows;
    tst = true;
    while (tst) {
      iy >>= 1;
      ju ^= iy;
      tst = ((ju & iy) == 0);
    }
    iy = ju;
  }
  if (j - 2 < 0) {
    j = 0;
  } else {
    j--;
  }
  y_data[iy] = x_data[j];
  if (unsigned_nRows > 1) {
    for (i = 0; i <= ihi; i += 2) {
      temp_re_tmp = y_data[i + 1].re;
      temp_im = y_data[i + 1].im;
      temp_re = y_data[i].re;
      twid_re = y_data[i].im;
      y_data[i + 1].re = temp_re - temp_re_tmp;
      y_data[i + 1].im = twid_re - temp_im;
      y_data[i].re = temp_re + temp_re_tmp;
      y_data[i].im = twid_re + temp_im;
    }
  }
  iDelta = 2;
  iDelta2 = 4;
  iheight = ((k - 1) << 2) + 1;
  while (k > 0) {
    for (i = 0; i < iheight; i += iDelta2) {
      iy = i + iDelta;
      temp_re = y_data[iy].re;
      temp_im = y_data[iy].im;
      y_data[iy].re = y_data[i].re - temp_re;
      y_data[iy].im = y_data[i].im - temp_im;
      y_data[i].re += temp_re;
      y_data[i].im += temp_im;
    }
    iy = 1;
    for (j = k; j < nRowsD2; j += k) {
      double twid_im;
      twid_re = costab_data[j];
      twid_im = sintab_data[j];
      i = iy;
      ihi = iy + iheight;
      while (i < ihi) {
        ju = i + iDelta;
        temp_re_tmp = y_data[ju].im;
        temp_im = y_data[ju].re;
        temp_re = twid_re * temp_im - twid_im * temp_re_tmp;
        temp_im = twid_re * temp_re_tmp + twid_im * temp_im;
        y_data[ju].re = y_data[i].re - temp_re;
        y_data[ju].im = y_data[i].im - temp_im;
        y_data[i].re += temp_re;
        y_data[i].im += temp_im;
        i += iDelta2;
      }
      iy++;
    }
    k /= 2;
    iDelta = iDelta2;
    iDelta2 += iDelta2;
    iheight -= iDelta;
  }
}

/*
 * Arguments    : const double x[242555]
 *                emxArray_creal_T *y
 *                int nRows
 *                int nfft
 *                const emxArray_creal_T *wwc
 *                const emxArray_real_T *costab
 *                const emxArray_real_T *sintab
 *                const emxArray_real_T *costabinv
 *                const emxArray_real_T *sintabinv
 * Return Type  : void
 */
static void d_FFTImplementationCallback_doH(
    const double x[242555], emxArray_creal_T *y, int nRows, int nfft,
    const emxArray_creal_T *wwc, const emxArray_real_T *costab,
    const emxArray_real_T *sintab, const emxArray_real_T *costabinv,
    const emxArray_real_T *sintabinv)
{
  emxArray_creal_T *fv;
  emxArray_creal_T *fy;
  emxArray_creal_T *reconVar1;
  emxArray_creal_T *reconVar2;
  emxArray_creal_T *ytmp;
  emxArray_int32_T *wrapIndex;
  emxArray_real_T *b_costab;
  emxArray_real_T *b_sintab;
  emxArray_real_T *costab1q;
  emxArray_real_T *hcostabinv;
  emxArray_real_T *hsintab;
  emxArray_real_T *hsintabinv;
  const creal_T *wwc_data;
  creal_T *fv_data;
  creal_T *fy_data;
  creal_T *reconVar1_data;
  creal_T *reconVar2_data;
  creal_T *y_data;
  creal_T *ytmp_data;
  const double *costab_data;
  const double *costabinv_data;
  const double *sintab_data;
  const double *sintabinv_data;
  double re_tmp;
  double temp_im;
  double temp_re;
  double temp_re_tmp;
  double twid_im;
  double twid_re;
  double z_tmp;
  double *b_costab_data;
  double *b_sintab_data;
  double *costab1q_data;
  double *hcostabinv_data;
  double *hsintab_data;
  double *hsintabinv_data;
  int hnRows;
  int hszCostab;
  int i;
  int iDelta;
  int iDelta2;
  int iheight;
  int ju;
  int k;
  int nRowsD2;
  int nd2;
  int u1;
  int *wrapIndex_data;
  bool tst;
  sintabinv_data = sintabinv->data;
  costabinv_data = costabinv->data;
  sintab_data = sintab->data;
  costab_data = costab->data;
  wwc_data = wwc->data;
  y_data = y->data;
  hnRows = nRows / 2;
  emxInit_creal_T(&ytmp);
  u1 = ytmp->size[0];
  ytmp->size[0] = hnRows;
  emxEnsureCapacity_creal_T(ytmp, u1);
  ytmp_data = ytmp->data;
  if (hnRows > 242555) {
    u1 = ytmp->size[0];
    ytmp->size[0] = hnRows;
    emxEnsureCapacity_creal_T(ytmp, u1);
    ytmp_data = ytmp->data;
    for (u1 = 0; u1 < hnRows; u1++) {
      ytmp_data[u1].re = 0.0;
      ytmp_data[u1].im = 0.0;
    }
  }
  if (nRows <= 242555) {
    tst = true;
    ju = nRows;
  } else {
    tst = false;
    ju = 242554;
  }
  if (ju > nRows) {
    ju = nRows;
  }
  nd2 = nRows << 1;
  temp_im = 6.2831853071795862 / (double)nd2;
  iDelta = nd2 / 2 / 2;
  emxInit_real_T(&costab1q, 2);
  u1 = costab1q->size[0] * costab1q->size[1];
  costab1q->size[0] = 1;
  costab1q->size[1] = iDelta + 1;
  emxEnsureCapacity_real_T(costab1q, u1);
  costab1q_data = costab1q->data;
  costab1q_data[0] = 1.0;
  nd2 = iDelta / 2 - 1;
  for (k = 0; k <= nd2; k++) {
    costab1q_data[k + 1] = cos(temp_im * ((double)k + 1.0));
  }
  u1 = nd2 + 2;
  nd2 = iDelta - 1;
  for (k = u1; k <= nd2; k++) {
    costab1q_data[k] = sin(temp_im * (double)(iDelta - k));
  }
  costab1q_data[iDelta] = 0.0;
  iDelta = costab1q->size[1] - 1;
  nd2 = (costab1q->size[1] - 1) << 1;
  emxInit_real_T(&b_costab, 2);
  u1 = b_costab->size[0] * b_costab->size[1];
  b_costab->size[0] = 1;
  b_costab->size[1] = nd2 + 1;
  emxEnsureCapacity_real_T(b_costab, u1);
  b_costab_data = b_costab->data;
  emxInit_real_T(&b_sintab, 2);
  u1 = b_sintab->size[0] * b_sintab->size[1];
  b_sintab->size[0] = 1;
  b_sintab->size[1] = nd2 + 1;
  emxEnsureCapacity_real_T(b_sintab, u1);
  b_sintab_data = b_sintab->data;
  b_costab_data[0] = 1.0;
  b_sintab_data[0] = 0.0;
  for (k = 0; k < iDelta; k++) {
    b_costab_data[k + 1] = costab1q_data[k + 1];
    b_sintab_data[k + 1] = -costab1q_data[(iDelta - k) - 1];
  }
  u1 = costab1q->size[1];
  for (k = u1; k <= nd2; k++) {
    b_costab_data[k] = -costab1q_data[nd2 - k];
    b_sintab_data[k] = -costab1q_data[k - iDelta];
  }
  hszCostab = (int)((unsigned int)costab->size[1] >> 1);
  u1 = costab1q->size[0] * costab1q->size[1];
  costab1q->size[0] = 1;
  costab1q->size[1] = hszCostab;
  emxEnsureCapacity_real_T(costab1q, u1);
  costab1q_data = costab1q->data;
  emxInit_real_T(&hsintab, 2);
  u1 = hsintab->size[0] * hsintab->size[1];
  hsintab->size[0] = 1;
  hsintab->size[1] = hszCostab;
  emxEnsureCapacity_real_T(hsintab, u1);
  hsintab_data = hsintab->data;
  emxInit_real_T(&hcostabinv, 2);
  u1 = hcostabinv->size[0] * hcostabinv->size[1];
  hcostabinv->size[0] = 1;
  hcostabinv->size[1] = hszCostab;
  emxEnsureCapacity_real_T(hcostabinv, u1);
  hcostabinv_data = hcostabinv->data;
  emxInit_real_T(&hsintabinv, 2);
  u1 = hsintabinv->size[0] * hsintabinv->size[1];
  hsintabinv->size[0] = 1;
  hsintabinv->size[1] = hszCostab;
  emxEnsureCapacity_real_T(hsintabinv, u1);
  hsintabinv_data = hsintabinv->data;
  for (i = 0; i < hszCostab; i++) {
    nd2 = ((i + 1) << 1) - 2;
    costab1q_data[i] = costab_data[nd2];
    hsintab_data[i] = sintab_data[nd2];
    hcostabinv_data[i] = costabinv_data[nd2];
    hsintabinv_data[i] = sintabinv_data[nd2];
  }
  emxInit_creal_T(&reconVar1);
  u1 = reconVar1->size[0];
  reconVar1->size[0] = hnRows;
  emxEnsureCapacity_creal_T(reconVar1, u1);
  reconVar1_data = reconVar1->data;
  emxInit_creal_T(&reconVar2);
  u1 = reconVar2->size[0];
  reconVar2->size[0] = hnRows;
  emxEnsureCapacity_creal_T(reconVar2, u1);
  reconVar2_data = reconVar2->data;
  emxInit_int32_T(&wrapIndex, 2);
  u1 = wrapIndex->size[0] * wrapIndex->size[1];
  wrapIndex->size[0] = 1;
  wrapIndex->size[1] = hnRows;
  emxEnsureCapacity_int32_T(wrapIndex, u1);
  wrapIndex_data = wrapIndex->data;
  for (i = 0; i < hnRows; i++) {
    u1 = i << 1;
    temp_im = b_sintab_data[u1];
    temp_re = b_costab_data[u1];
    reconVar1_data[i].re = temp_im + 1.0;
    reconVar1_data[i].im = -temp_re;
    reconVar2_data[i].re = 1.0 - temp_im;
    reconVar2_data[i].im = temp_re;
    if (i + 1 != 1) {
      wrapIndex_data[i] = (hnRows - i) + 1;
    } else {
      wrapIndex_data[0] = 1;
    }
  }
  emxFree_real_T(&b_sintab);
  emxFree_real_T(&b_costab);
  z_tmp = (double)ju / 2.0;
  u1 = (int)z_tmp;
  for (hszCostab = 0; hszCostab < u1; hszCostab++) {
    nd2 = (hnRows + hszCostab) - 1;
    temp_re = wwc_data[nd2].re;
    temp_im = wwc_data[nd2].im;
    nd2 = hszCostab << 1;
    twid_re = x[nd2];
    twid_im = x[nd2 + 1];
    ytmp_data[hszCostab].re = temp_re * twid_re + temp_im * twid_im;
    ytmp_data[hszCostab].im = temp_re * twid_im - temp_im * twid_re;
  }
  if (!tst) {
    nd2 = (hnRows + (int)z_tmp) - 1;
    temp_re = wwc_data[nd2].re;
    temp_im = wwc_data[nd2].im;
    if ((int)z_tmp - 1 < 0) {
      iDelta = 0;
    } else {
      iDelta = (int)z_tmp << 1;
    }
    twid_re = x[iDelta];
    ytmp_data[(int)((double)ju / 2.0)].re = temp_re * twid_re + temp_im * 0.0;
    ytmp_data[(int)((double)ju / 2.0)].im = temp_re * 0.0 - temp_im * twid_re;
    if ((int)z_tmp + 2 <= hnRows) {
      u1 = (int)z_tmp + 2;
      for (i = u1; i <= hnRows; i++) {
        ytmp_data[i - 1].re = 0.0;
        ytmp_data[i - 1].im = 0.0;
      }
    }
  } else if ((int)z_tmp + 1 <= hnRows) {
    u1 = (int)z_tmp + 1;
    for (i = u1; i <= hnRows; i++) {
      ytmp_data[i - 1].re = 0.0;
      ytmp_data[i - 1].im = 0.0;
    }
  }
  z_tmp = (double)nfft / 2.0;
  emxInit_creal_T(&fy);
  nd2 = (int)z_tmp;
  u1 = fy->size[0];
  fy->size[0] = (int)z_tmp;
  emxEnsureCapacity_creal_T(fy, u1);
  fy_data = fy->data;
  if ((int)z_tmp > ytmp->size[0]) {
    u1 = fy->size[0];
    fy->size[0] = (int)z_tmp;
    emxEnsureCapacity_creal_T(fy, u1);
    fy_data = fy->data;
    for (u1 = 0; u1 < nd2; u1++) {
      fy_data[u1].re = 0.0;
      fy_data[u1].im = 0.0;
    }
  }
  ju = ytmp->size[0];
  u1 = (int)z_tmp;
  if (ju <= u1) {
    u1 = ju;
  }
  hszCostab = (int)z_tmp - 2;
  nRowsD2 = (int)z_tmp / 2;
  k = nRowsD2 / 2;
  nd2 = 0;
  ju = 0;
  for (i = 0; i <= u1 - 2; i++) {
    fy_data[nd2] = ytmp_data[i];
    iDelta = (int)z_tmp;
    tst = true;
    while (tst) {
      iDelta >>= 1;
      ju ^= iDelta;
      tst = ((ju & iDelta) == 0);
    }
    nd2 = ju;
  }
  if (u1 - 2 < 0) {
    iDelta = 0;
  } else {
    iDelta = u1 - 1;
  }
  fy_data[nd2] = ytmp_data[iDelta];
  if ((int)z_tmp > 1) {
    for (i = 0; i <= hszCostab; i += 2) {
      temp_re_tmp = fy_data[i + 1].re;
      temp_re = fy_data[i + 1].im;
      re_tmp = fy_data[i].re;
      twid_re = fy_data[i].im;
      fy_data[i + 1].re = re_tmp - temp_re_tmp;
      fy_data[i + 1].im = twid_re - temp_re;
      fy_data[i].re = re_tmp + temp_re_tmp;
      fy_data[i].im = twid_re + temp_re;
    }
  }
  iDelta = 2;
  iDelta2 = 4;
  iheight = ((k - 1) << 2) + 1;
  while (k > 0) {
    for (i = 0; i < iheight; i += iDelta2) {
      nd2 = i + iDelta;
      temp_re = fy_data[nd2].re;
      temp_im = fy_data[nd2].im;
      fy_data[nd2].re = fy_data[i].re - temp_re;
      fy_data[nd2].im = fy_data[i].im - temp_im;
      fy_data[i].re += temp_re;
      fy_data[i].im += temp_im;
    }
    nd2 = 1;
    for (hszCostab = k; hszCostab < nRowsD2; hszCostab += k) {
      twid_re = costab1q_data[hszCostab];
      twid_im = hsintab_data[hszCostab];
      i = nd2;
      ju = nd2 + iheight;
      while (i < ju) {
        u1 = i + iDelta;
        temp_re_tmp = fy_data[u1].im;
        temp_im = fy_data[u1].re;
        temp_re = twid_re * temp_im - twid_im * temp_re_tmp;
        temp_im = twid_re * temp_re_tmp + twid_im * temp_im;
        fy_data[u1].re = fy_data[i].re - temp_re;
        fy_data[u1].im = fy_data[i].im - temp_im;
        fy_data[i].re += temp_re;
        fy_data[i].im += temp_im;
        i += iDelta2;
      }
      nd2++;
    }
    k /= 2;
    iDelta = iDelta2;
    iDelta2 += iDelta2;
    iheight -= iDelta;
  }
  emxInit_creal_T(&fv);
  c_FFTImplementationCallback_r2b(wwc, (int)z_tmp, costab1q, hsintab, fv);
  fv_data = fv->data;
  emxFree_real_T(&costab1q);
  emxFree_real_T(&hsintab);
  nd2 = fy->size[0];
  for (u1 = 0; u1 < nd2; u1++) {
    re_tmp = fy_data[u1].re;
    twid_im = fv_data[u1].im;
    temp_im = fy_data[u1].im;
    temp_re = fv_data[u1].re;
    fy_data[u1].re = re_tmp * temp_re - temp_im * twid_im;
    fy_data[u1].im = re_tmp * twid_im + temp_im * temp_re;
  }
  c_FFTImplementationCallback_r2b(fy, (int)z_tmp, hcostabinv, hsintabinv, fv);
  fv_data = fv->data;
  emxFree_creal_T(&fy);
  emxFree_real_T(&hsintabinv);
  emxFree_real_T(&hcostabinv);
  if (fv->size[0] > 1) {
    temp_im = 1.0 / (double)fv->size[0];
    nd2 = fv->size[0];
    for (u1 = 0; u1 < nd2; u1++) {
      fv_data[u1].re *= temp_im;
      fv_data[u1].im *= temp_im;
    }
  }
  u1 = wwc->size[0];
  for (k = hnRows; k <= u1; k++) {
    temp_im = wwc_data[k - 1].re;
    temp_re = fv_data[k - 1].im;
    twid_re = wwc_data[k - 1].im;
    twid_im = fv_data[k - 1].re;
    nd2 = k - hnRows;
    ytmp_data[nd2].re = temp_im * twid_im + twid_re * temp_re;
    ytmp_data[nd2].im = temp_im * temp_re - twid_re * twid_im;
  }
  emxFree_creal_T(&fv);
  for (i = 0; i < hnRows; i++) {
    double ytmp_re_tmp;
    u1 = wrapIndex_data[i];
    temp_im = ytmp_data[i].re;
    temp_re = reconVar1_data[i].im;
    twid_re = ytmp_data[i].im;
    twid_im = reconVar1_data[i].re;
    re_tmp = ytmp_data[u1 - 1].re;
    temp_re_tmp = -ytmp_data[u1 - 1].im;
    z_tmp = reconVar2_data[i].im;
    ytmp_re_tmp = reconVar2_data[i].re;
    y_data[i].re = 0.5 * ((temp_im * twid_im - twid_re * temp_re) +
                          (re_tmp * ytmp_re_tmp - temp_re_tmp * z_tmp));
    y_data[i].im = 0.5 * ((temp_im * temp_re + twid_re * twid_im) +
                          (re_tmp * z_tmp + temp_re_tmp * ytmp_re_tmp));
    u1 = hnRows + i;
    y_data[u1].re = 0.5 * ((temp_im * ytmp_re_tmp - twid_re * z_tmp) +
                           (re_tmp * twid_im - temp_re_tmp * temp_re));
    y_data[u1].im = 0.5 * ((temp_im * z_tmp + twid_re * ytmp_re_tmp) +
                           (re_tmp * temp_re + temp_re_tmp * twid_im));
  }
  emxFree_creal_T(&reconVar2);
  emxFree_creal_T(&reconVar1);
  emxFree_int32_T(&wrapIndex);
  emxFree_creal_T(&ytmp);
}

/*
 * Arguments    : const double x[242555]
 *                emxArray_creal_T *y
 *                int unsigned_nRows
 *                const emxArray_real_T *costab
 *                const emxArray_real_T *sintab
 * Return Type  : void
 */
void c_FFTImplementationCallback_doH(const double x[242555],
                                     emxArray_creal_T *y, int unsigned_nRows,
                                     const emxArray_real_T *costab,
                                     const emxArray_real_T *sintab)
{
  emxArray_creal_T *reconVar1;
  emxArray_creal_T *reconVar2;
  emxArray_int32_T *bitrevIndex;
  emxArray_int32_T *wrapIndex;
  emxArray_real_T *hcostab;
  emxArray_real_T *hsintab;
  creal_T *reconVar1_data;
  creal_T *reconVar2_data;
  creal_T *y_data;
  const double *costab_data;
  const double *sintab_data;
  double b_y_re_tmp;
  double c_y_re_tmp;
  double d_y_re_tmp;
  double im;
  double re;
  double temp2_im;
  double temp2_re;
  double temp_im;
  double temp_im_tmp;
  double temp_re;
  double temp_re_tmp;
  double y_re_tmp;
  double z_tmp;
  double *hcostab_data;
  double *hsintab_data;
  int hszCostab;
  int i;
  int iDelta2;
  int iheight;
  int iterVar;
  int ju;
  int k;
  int nRows;
  int nRowsD2;
  int u0;
  int *bitrevIndex_data;
  int *wrapIndex_data;
  bool tst;
  sintab_data = sintab->data;
  costab_data = costab->data;
  y_data = y->data;
  nRows = unsigned_nRows / 2;
  u0 = y->size[0];
  if (u0 > nRows) {
    u0 = nRows;
  }
  iDelta2 = nRows - 2;
  nRowsD2 = nRows / 2;
  k = nRowsD2 / 2;
  hszCostab = (int)((unsigned int)costab->size[1] >> 1);
  emxInit_real_T(&hcostab, 2);
  iheight = hcostab->size[0] * hcostab->size[1];
  hcostab->size[0] = 1;
  hcostab->size[1] = hszCostab;
  emxEnsureCapacity_real_T(hcostab, iheight);
  hcostab_data = hcostab->data;
  emxInit_real_T(&hsintab, 2);
  iheight = hsintab->size[0] * hsintab->size[1];
  hsintab->size[0] = 1;
  hsintab->size[1] = hszCostab;
  emxEnsureCapacity_real_T(hsintab, iheight);
  hsintab_data = hsintab->data;
  for (i = 0; i < hszCostab; i++) {
    iterVar = ((i + 1) << 1) - 2;
    hcostab_data[i] = costab_data[iterVar];
    hsintab_data[i] = sintab_data[iterVar];
  }
  emxInit_creal_T(&reconVar1);
  iheight = reconVar1->size[0];
  reconVar1->size[0] = nRows;
  emxEnsureCapacity_creal_T(reconVar1, iheight);
  reconVar1_data = reconVar1->data;
  emxInit_creal_T(&reconVar2);
  iheight = reconVar2->size[0];
  reconVar2->size[0] = nRows;
  emxEnsureCapacity_creal_T(reconVar2, iheight);
  reconVar2_data = reconVar2->data;
  emxInit_int32_T(&wrapIndex, 2);
  iheight = wrapIndex->size[0] * wrapIndex->size[1];
  wrapIndex->size[0] = 1;
  wrapIndex->size[1] = nRows;
  emxEnsureCapacity_int32_T(wrapIndex, iheight);
  wrapIndex_data = wrapIndex->data;
  for (i = 0; i < nRows; i++) {
    temp2_re = sintab_data[i];
    temp2_im = costab_data[i];
    reconVar1_data[i].re = temp2_re + 1.0;
    reconVar1_data[i].im = -temp2_im;
    reconVar2_data[i].re = 1.0 - temp2_re;
    reconVar2_data[i].im = temp2_im;
    if (i + 1 != 1) {
      wrapIndex_data[i] = (nRows - i) + 1;
    } else {
      wrapIndex_data[0] = 1;
    }
  }
  z_tmp = (double)unsigned_nRows / 2.0;
  ju = 0;
  hszCostab = 1;
  emxInit_int32_T(&bitrevIndex, 1);
  iterVar = (int)((double)unsigned_nRows / 2.0);
  iheight = bitrevIndex->size[0];
  bitrevIndex->size[0] = (int)z_tmp;
  emxEnsureCapacity_int32_T(bitrevIndex, iheight);
  bitrevIndex_data = bitrevIndex->data;
  for (iheight = 0; iheight < iterVar; iheight++) {
    bitrevIndex_data[iheight] = 0;
  }
  for (iheight = 0; iheight <= u0 - 2; iheight++) {
    bitrevIndex_data[iheight] = hszCostab;
    iterVar = (int)z_tmp;
    tst = true;
    while (tst) {
      iterVar >>= 1;
      ju ^= iterVar;
      tst = ((ju & iterVar) == 0);
    }
    hszCostab = ju + 1;
  }
  bitrevIndex_data[u0 - 1] = hszCostab;
  if (unsigned_nRows <= 242555) {
    tst = true;
    u0 = unsigned_nRows;
  } else {
    tst = false;
    u0 = 242554;
  }
  if (u0 > unsigned_nRows) {
    u0 = unsigned_nRows;
  }
  temp2_re = (double)u0 / 2.0;
  iheight = (int)temp2_re;
  for (i = 0; i < iheight; i++) {
    hszCostab = i << 1;
    y_data[bitrevIndex_data[i] - 1].re = x[hszCostab];
    y_data[bitrevIndex_data[i] - 1].im = x[hszCostab + 1];
  }
  if (!tst) {
    iheight = bitrevIndex_data[(int)temp2_re] - 1;
    if ((int)temp2_re - 1 < 0) {
      u0 = 0;
    } else {
      u0 = (int)temp2_re << 1;
    }
    y_data[iheight].re = x[u0];
    y_data[iheight].im = 0.0;
  }
  emxFree_int32_T(&bitrevIndex);
  if (nRows > 1) {
    for (i = 0; i <= iDelta2; i += 2) {
      temp2_re = y_data[i + 1].re;
      temp2_im = y_data[i + 1].im;
      temp_re = temp2_re;
      temp_im = temp2_im;
      re = y_data[i].re;
      im = y_data[i].im;
      temp2_re = re - temp2_re;
      temp2_im = im - temp2_im;
      y_data[i + 1].re = temp2_re;
      y_data[i + 1].im = temp2_im;
      re += temp_re;
      im += temp_im;
      y_data[i].re = re;
      y_data[i].im = im;
    }
  }
  nRows = 2;
  iDelta2 = 4;
  iheight = ((k - 1) << 2) + 1;
  while (k > 0) {
    for (i = 0; i < iheight; i += iDelta2) {
      iterVar = i + nRows;
      temp_re = y_data[iterVar].re;
      temp_im = y_data[iterVar].im;
      y_data[iterVar].re = y_data[i].re - temp_re;
      y_data[iterVar].im = y_data[i].im - temp_im;
      y_data[i].re += temp_re;
      y_data[i].im += temp_im;
    }
    iterVar = 1;
    for (hszCostab = k; hszCostab < nRowsD2; hszCostab += k) {
      temp2_re = hcostab_data[hszCostab];
      temp2_im = hsintab_data[hszCostab];
      i = iterVar;
      ju = iterVar + iheight;
      while (i < ju) {
        u0 = i + nRows;
        temp_re_tmp = y_data[u0].im;
        temp_im = y_data[u0].re;
        temp_re = temp2_re * temp_im - temp2_im * temp_re_tmp;
        temp_im = temp2_re * temp_re_tmp + temp2_im * temp_im;
        y_data[u0].re = y_data[i].re - temp_re;
        y_data[u0].im = y_data[i].im - temp_im;
        y_data[i].re += temp_re;
        y_data[i].im += temp_im;
        i += iDelta2;
      }
      iterVar++;
    }
    k /= 2;
    nRows = iDelta2;
    iDelta2 += iDelta2;
    iheight -= nRows;
  }
  emxFree_real_T(&hsintab);
  emxFree_real_T(&hcostab);
  iterVar = (int)z_tmp / 2;
  temp2_im = y_data[0].re;
  temp_im_tmp = y_data[0].im;
  y_re_tmp = temp2_im * reconVar1_data[0].re;
  temp_im = temp2_im * reconVar1_data[0].im;
  b_y_re_tmp = temp2_im * reconVar2_data[0].re;
  re = temp2_im * reconVar2_data[0].im;
  y_data[0].re = 0.5 * ((y_re_tmp - temp_im_tmp * reconVar1_data[0].im) +
                        (b_y_re_tmp - -temp_im_tmp * reconVar2_data[0].im));
  y_data[0].im = 0.5 * ((temp_im + temp_im_tmp * reconVar1_data[0].re) +
                        (re + -temp_im_tmp * reconVar2_data[0].re));
  y_data[(int)z_tmp].re =
      0.5 * ((b_y_re_tmp - temp_im_tmp * reconVar2_data[0].im) +
             (y_re_tmp - -temp_im_tmp * reconVar1_data[0].im));
  y_data[(int)z_tmp].im =
      0.5 * ((re + temp_im_tmp * reconVar2_data[0].re) +
             (temp_im + -temp_im_tmp * reconVar1_data[0].re));
  for (i = 2; i <= iterVar; i++) {
    temp_re_tmp = y_data[i - 1].re;
    temp_im_tmp = y_data[i - 1].im;
    iheight = wrapIndex_data[i - 1];
    temp2_im = y_data[iheight - 1].re;
    temp_re = y_data[iheight - 1].im;
    y_re_tmp = reconVar1_data[i - 1].im;
    b_y_re_tmp = reconVar1_data[i - 1].re;
    c_y_re_tmp = reconVar2_data[i - 1].im;
    d_y_re_tmp = reconVar2_data[i - 1].re;
    y_data[i - 1].re =
        0.5 * ((temp_re_tmp * b_y_re_tmp - temp_im_tmp * y_re_tmp) +
               (temp2_im * d_y_re_tmp - -temp_re * c_y_re_tmp));
    y_data[i - 1].im =
        0.5 * ((temp_re_tmp * y_re_tmp + temp_im_tmp * b_y_re_tmp) +
               (temp2_im * c_y_re_tmp + -temp_re * d_y_re_tmp));
    hszCostab = ((int)z_tmp + i) - 1;
    y_data[hszCostab].re =
        0.5 * ((temp_re_tmp * d_y_re_tmp - temp_im_tmp * c_y_re_tmp) +
               (temp2_im * b_y_re_tmp - -temp_re * y_re_tmp));
    y_data[hszCostab].im =
        0.5 * ((temp_re_tmp * c_y_re_tmp + temp_im_tmp * d_y_re_tmp) +
               (temp2_im * y_re_tmp + -temp_re * b_y_re_tmp));
    re = reconVar1_data[iheight - 1].im;
    im = reconVar1_data[iheight - 1].re;
    temp_im = reconVar2_data[iheight - 1].im;
    temp2_re = reconVar2_data[iheight - 1].re;
    y_data[iheight - 1].re =
        0.5 * ((temp2_im * im - temp_re * re) +
               (temp_re_tmp * temp2_re - -temp_im_tmp * temp_im));
    y_data[iheight - 1].im =
        0.5 * ((temp2_im * re + temp_re * im) +
               (temp_re_tmp * temp_im + -temp_im_tmp * temp2_re));
    iheight = (iheight + (int)z_tmp) - 1;
    y_data[iheight].re = 0.5 * ((temp2_im * temp2_re - temp_re * temp_im) +
                                (temp_re_tmp * im - -temp_im_tmp * re));
    y_data[iheight].im = 0.5 * ((temp2_im * temp_im + temp_re * temp2_re) +
                                (temp_re_tmp * re + -temp_im_tmp * im));
  }
  emxFree_int32_T(&wrapIndex);
  if (iterVar != 0) {
    temp_re_tmp = y_data[iterVar].re;
    temp_im_tmp = y_data[iterVar].im;
    y_re_tmp = reconVar1_data[iterVar].im;
    b_y_re_tmp = reconVar1_data[iterVar].re;
    c_y_re_tmp = temp_re_tmp * b_y_re_tmp;
    temp_im = temp_re_tmp * y_re_tmp;
    d_y_re_tmp = reconVar2_data[iterVar].im;
    im = reconVar2_data[iterVar].re;
    temp2_re = temp_re_tmp * im;
    re = temp_re_tmp * d_y_re_tmp;
    y_data[iterVar].re = 0.5 * ((c_y_re_tmp - temp_im_tmp * y_re_tmp) +
                                (temp2_re - -temp_im_tmp * d_y_re_tmp));
    y_data[iterVar].im =
        0.5 * ((temp_im + temp_im_tmp * b_y_re_tmp) + (re + -temp_im_tmp * im));
    iheight = (int)z_tmp + iterVar;
    y_data[iheight].re = 0.5 * ((temp2_re - temp_im_tmp * d_y_re_tmp) +
                                (c_y_re_tmp - -temp_im_tmp * y_re_tmp));
    y_data[iheight].im =
        0.5 * ((re + temp_im_tmp * im) + (temp_im + -temp_im_tmp * b_y_re_tmp));
  }
  emxFree_creal_T(&reconVar2);
  emxFree_creal_T(&reconVar1);
}

/*
 * Arguments    : const double x[242555]
 *                int n2blue
 *                int nfft
 *                const emxArray_real_T *costab
 *                const emxArray_real_T *sintab
 *                const emxArray_real_T *sintabinv
 *                emxArray_creal_T *y
 * Return Type  : void
 */
void c_FFTImplementationCallback_dob(const double x[242555], int n2blue,
                                     int nfft, const emxArray_real_T *costab,
                                     const emxArray_real_T *sintab,
                                     const emxArray_real_T *sintabinv,
                                     emxArray_creal_T *y)
{
  emxArray_creal_T *fv;
  emxArray_creal_T *fy;
  emxArray_creal_T *wwc;
  creal_T *fv_data;
  creal_T *fy_data;
  creal_T *wwc_data;
  creal_T *y_data;
  const double *costab_data;
  const double *sintab_data;
  double nt_im;
  int i;
  int iDelta;
  int iDelta2;
  int k;
  int minNrowsNx;
  int nInt2;
  int nInt2m1;
  int rt;
  sintab_data = sintab->data;
  costab_data = costab->data;
  emxInit_creal_T(&wwc);
  if ((nfft != 1) && ((nfft & 1) == 0)) {
    iDelta = nfft / 2;
    nInt2m1 = (iDelta + iDelta) - 1;
    iDelta2 = wwc->size[0];
    wwc->size[0] = nInt2m1;
    emxEnsureCapacity_creal_T(wwc, iDelta2);
    wwc_data = wwc->data;
    rt = 0;
    wwc_data[iDelta - 1].re = 1.0;
    wwc_data[iDelta - 1].im = 0.0;
    nInt2 = iDelta << 1;
    for (k = 0; k <= iDelta - 2; k++) {
      minNrowsNx = ((k + 1) << 1) - 1;
      if (nInt2 - rt <= minNrowsNx) {
        rt += minNrowsNx - nInt2;
      } else {
        rt += minNrowsNx;
      }
      nt_im = -3.1415926535897931 * (double)rt / (double)iDelta;
      iDelta2 = (iDelta - k) - 2;
      wwc_data[iDelta2].re = cos(nt_im);
      wwc_data[iDelta2].im = -sin(nt_im);
    }
    iDelta2 = nInt2m1 - 1;
    for (k = iDelta2; k >= iDelta; k--) {
      wwc_data[k] = wwc_data[(nInt2m1 - k) - 1];
    }
  } else {
    nInt2m1 = (nfft + nfft) - 1;
    iDelta2 = wwc->size[0];
    wwc->size[0] = nInt2m1;
    emxEnsureCapacity_creal_T(wwc, iDelta2);
    wwc_data = wwc->data;
    rt = 0;
    wwc_data[nfft - 1].re = 1.0;
    wwc_data[nfft - 1].im = 0.0;
    nInt2 = nfft << 1;
    for (k = 0; k <= nfft - 2; k++) {
      minNrowsNx = ((k + 1) << 1) - 1;
      if (nInt2 - rt <= minNrowsNx) {
        rt += minNrowsNx - nInt2;
      } else {
        rt += minNrowsNx;
      }
      nt_im = -3.1415926535897931 * (double)rt / (double)nfft;
      iDelta2 = (nfft - k) - 2;
      wwc_data[iDelta2].re = cos(nt_im);
      wwc_data[iDelta2].im = -sin(nt_im);
    }
    iDelta2 = nInt2m1 - 1;
    for (k = iDelta2; k >= nfft; k--) {
      wwc_data[k] = wwc_data[(nInt2m1 - k) - 1];
    }
  }
  iDelta2 = y->size[0];
  y->size[0] = nfft;
  emxEnsureCapacity_creal_T(y, iDelta2);
  y_data = y->data;
  if (nfft > 242555) {
    iDelta2 = y->size[0];
    y->size[0] = nfft;
    emxEnsureCapacity_creal_T(y, iDelta2);
    y_data = y->data;
    for (iDelta2 = 0; iDelta2 < nfft; iDelta2++) {
      y_data[iDelta2].re = 0.0;
      y_data[iDelta2].im = 0.0;
    }
  }
  emxInit_creal_T(&fy);
  emxInit_creal_T(&fv);
  if ((n2blue != 1) && ((nfft & 1) == 0)) {
    d_FFTImplementationCallback_doH(x, y, nfft, n2blue, wwc, costab, sintab,
                                    costab, sintabinv);
  } else {
    double nt_re;
    double nt_re_tmp;
    double twid_im;
    double twid_re;
    int iheight;
    int nRowsD2;
    if (nfft <= 242555) {
      minNrowsNx = nfft - 1;
    } else {
      minNrowsNx = 242554;
    }
    for (k = 0; k <= minNrowsNx; k++) {
      nInt2m1 = (nfft + k) - 1;
      nt_im = x[k];
      y_data[k].re = wwc_data[nInt2m1].re * nt_im;
      y_data[k].im = wwc_data[nInt2m1].im * -nt_im;
    }
    iDelta2 = minNrowsNx + 2;
    for (k = iDelta2; k <= nfft; k++) {
      y_data[k - 1].re = 0.0;
      y_data[k - 1].im = 0.0;
    }
    iDelta2 = fy->size[0];
    fy->size[0] = n2blue;
    emxEnsureCapacity_creal_T(fy, iDelta2);
    fy_data = fy->data;
    if (n2blue > y->size[0]) {
      iDelta2 = fy->size[0];
      fy->size[0] = n2blue;
      emxEnsureCapacity_creal_T(fy, iDelta2);
      fy_data = fy->data;
      for (iDelta2 = 0; iDelta2 < n2blue; iDelta2++) {
        fy_data[iDelta2].re = 0.0;
        fy_data[iDelta2].im = 0.0;
      }
    }
    nInt2m1 = y->size[0];
    if (nInt2m1 > n2blue) {
      nInt2m1 = n2blue;
    }
    rt = n2blue - 2;
    nRowsD2 = n2blue / 2;
    k = nRowsD2 / 2;
    minNrowsNx = 0;
    nInt2 = 0;
    for (i = 0; i <= nInt2m1 - 2; i++) {
      bool tst;
      fy_data[minNrowsNx] = y_data[i];
      minNrowsNx = n2blue;
      tst = true;
      while (tst) {
        minNrowsNx >>= 1;
        nInt2 ^= minNrowsNx;
        tst = ((nInt2 & minNrowsNx) == 0);
      }
      minNrowsNx = nInt2;
    }
    if (nInt2m1 - 2 < 0) {
      nInt2m1 = 0;
    } else {
      nInt2m1--;
    }
    fy_data[minNrowsNx] = y_data[nInt2m1];
    if (n2blue > 1) {
      for (i = 0; i <= rt; i += 2) {
        nt_re_tmp = fy_data[i + 1].re;
        nt_im = fy_data[i + 1].im;
        twid_im = fy_data[i].re;
        nt_re = fy_data[i].im;
        fy_data[i + 1].re = twid_im - nt_re_tmp;
        fy_data[i + 1].im = nt_re - nt_im;
        fy_data[i].re = twid_im + nt_re_tmp;
        fy_data[i].im = nt_re + nt_im;
      }
    }
    iDelta = 2;
    iDelta2 = 4;
    iheight = ((k - 1) << 2) + 1;
    while (k > 0) {
      for (i = 0; i < iheight; i += iDelta2) {
        nInt2m1 = i + iDelta;
        nt_re = fy_data[nInt2m1].re;
        nt_im = fy_data[nInt2m1].im;
        fy_data[nInt2m1].re = fy_data[i].re - nt_re;
        fy_data[nInt2m1].im = fy_data[i].im - nt_im;
        fy_data[i].re += nt_re;
        fy_data[i].im += nt_im;
      }
      minNrowsNx = 1;
      for (nInt2m1 = k; nInt2m1 < nRowsD2; nInt2m1 += k) {
        twid_re = costab_data[nInt2m1];
        twid_im = sintab_data[nInt2m1];
        i = minNrowsNx;
        rt = minNrowsNx + iheight;
        while (i < rt) {
          nInt2 = i + iDelta;
          nt_re_tmp = fy_data[nInt2].im;
          nt_im = fy_data[nInt2].re;
          nt_re = twid_re * nt_im - twid_im * nt_re_tmp;
          nt_im = twid_re * nt_re_tmp + twid_im * nt_im;
          fy_data[nInt2].re = fy_data[i].re - nt_re;
          fy_data[nInt2].im = fy_data[i].im - nt_im;
          fy_data[i].re += nt_re;
          fy_data[i].im += nt_im;
          i += iDelta2;
        }
        minNrowsNx++;
      }
      k /= 2;
      iDelta = iDelta2;
      iDelta2 += iDelta2;
      iheight -= iDelta;
    }
    c_FFTImplementationCallback_r2b(wwc, n2blue, costab, sintab, fv);
    fv_data = fv->data;
    minNrowsNx = fy->size[0];
    for (iDelta2 = 0; iDelta2 < minNrowsNx; iDelta2++) {
      twid_im = fy_data[iDelta2].re;
      nt_im = fv_data[iDelta2].im;
      nt_re = fy_data[iDelta2].im;
      twid_re = fv_data[iDelta2].re;
      fy_data[iDelta2].re = twid_im * twid_re - nt_re * nt_im;
      fy_data[iDelta2].im = twid_im * nt_im + nt_re * twid_re;
    }
    c_FFTImplementationCallback_r2b(fy, n2blue, costab, sintabinv, fv);
    fv_data = fv->data;
    if (fv->size[0] > 1) {
      nt_im = 1.0 / (double)fv->size[0];
      minNrowsNx = fv->size[0];
      for (iDelta2 = 0; iDelta2 < minNrowsNx; iDelta2++) {
        fv_data[iDelta2].re *= nt_im;
        fv_data[iDelta2].im *= nt_im;
      }
    }
    iDelta2 = wwc->size[0];
    for (k = nfft; k <= iDelta2; k++) {
      nt_im = wwc_data[k - 1].re;
      nt_re = fv_data[k - 1].im;
      twid_re = wwc_data[k - 1].im;
      twid_im = fv_data[k - 1].re;
      minNrowsNx = k - nfft;
      y_data[minNrowsNx].re = nt_im * twid_im + twid_re * nt_re;
      y_data[minNrowsNx].im = nt_im * nt_re - twid_re * twid_im;
    }
  }
  emxFree_creal_T(&fv);
  emxFree_creal_T(&fy);
  emxFree_creal_T(&wwc);
}

/*
 * File trailer for FFTImplementationCallback.c
 *
 * [EOF]
 */
