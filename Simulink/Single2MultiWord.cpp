//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// File: Single2MultiWord.cpp
//
// Code generated for Simulink model 'ankle_ds_gyems_f446'.
//
// Model version                  : 4.69
// Simulink Coder version         : 9.7 (R2022a) 13-Nov-2021
// C/C++ source code generated on : Thu Feb  9 17:24:16 2023
//
#include "Single2MultiWord.h"
#include <cmath>
#include "rtwtypes.h"

void Single2MultiWord(real32_T u1, uint32_T y[], int32_T n)
{
  int32_T currExp;
  int32_T msl;
  int32_T prevExp;
  real32_T yn;
  uint32_T cb;
  boolean_T isNegative;
  isNegative = (u1 < 0.0F);
  yn = std::frexp(u1, &currExp);
  msl = currExp <= 0 ? -1 : (currExp - 1) / 32;
  cb = 1U;
  for (int32_T i = msl + 1; i < n; i++) {
    y[i] = 0U;
  }

  yn = isNegative ? -yn : yn;
  prevExp = msl << 5;
  for (int32_T i = msl; i >= 0; i--) {
    real32_T yd;
    yn = std::ldexp(yn, currExp - prevExp);
    yd = std::floor(yn);
    yn -= yd;
    if (i < n) {
      y[i] = static_cast<uint32_T>(yd);
    }

    currExp = prevExp;
    prevExp -= 32;
  }

  if (isNegative) {
    for (int32_T i = 0; i < n; i++) {
      uint32_T u1i;
      u1i = ~y[i];
      cb += u1i;
      y[i] = cb;
      cb = (cb < u1i);
    }
  }
}

//
// File trailer for generated code.
//
// [EOF]
//
