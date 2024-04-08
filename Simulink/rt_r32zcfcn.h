//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// File: rt_r32zcfcn.h
//
// Code generated for Simulink model 'Ankle_v1'.
//
// Model version                  : 4.1
// Simulink Coder version         : 9.7 (R2022a) 13-Nov-2021
// C/C++ source code generated on : Fri Sep  9 13:53:19 2022
//
#ifndef RTW_HEADER_rt_r32zcfcn_h_
#define RTW_HEADER_rt_r32zcfcn_h_
#include "rtwtypes.h"
#include "zero_crossing_types.h"
#include "solver_zc.h"
#ifndef slZcHadEvent
#define slZcHadEvent(ev, zcsDir)       (((ev) & (zcsDir)) != 0x00 )
#endif

#ifndef slZcUnAliasEvents
#define slZcUnAliasEvents(evL, evR)    ((((slZcHadEvent((evL), (SL_ZCS_EVENT_N2Z)) && slZcHadEvent((evR), (SL_ZCS_EVENT_Z2P))) || (slZcHadEvent((evL), (SL_ZCS_EVENT_P2Z)) && slZcHadEvent((evR), (SL_ZCS_EVENT_Z2N)))) ? (SL_ZCS_EVENT_NUL) : (evR)))
#endif

extern "C" {
  extern ZCEventType rt_R32ZCFcn(ZCDirection zcDir, ZCSigState *prevZc, real32_T
    currValue);
}                                      // extern "C"
#endif                                 // RTW_HEADER_rt_r32zcfcn_h_

//
// File trailer for generated code.
//
// [EOF]
//