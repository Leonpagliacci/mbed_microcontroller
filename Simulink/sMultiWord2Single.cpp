//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// File: sMultiWord2Single.cpp
//
// Code generated for Simulink model 'ankle_ds_gyems_f446'.
//
// Model version                  : 4.69
// Simulink Coder version         : 9.7 (R2022a) 13-Nov-2021
// C/C++ source code generated on : Thu Feb  9 17:24:16 2023
//
#include "sMultiWord2Single.h"
#include <cmath>
#include "rtwtypes.h"

real32_T sMultiWord2Single(const uint32_T u1[], int32_T n1, int32_T e1)
{
  int32_T exp_0;
  real32_T y;
  y = 0.0F;
  exp_0 = e1;
  if ((u1[n1 - 1] & 2147483648U) != 0U) {
    uint32_T cb;
    cb = 1U;
    for (int32_T i = 0; i < n1; i++) {
      uint32_T u1i;
      u1i = ~u1[i];
      cb += u1i;
      y -= static_cast<real32_T>(std::ldexp(static_cast<real_T>(static_cast<
        real32_T>(cb)), exp_0));
      cb = (cb < u1i);
      exp_0 += 32;
    }
  } else {
    for (int32_T i = 0; i < n1; i++) {
      y += static_cast<real32_T>(std::ldexp(static_cast<real_T>(static_cast<
        real32_T>(u1[i])), exp_0));
      exp_0 += 32;
    }
  }

  return y;
}

//
// File trailer for generated code.
//
// [EOF]
//
