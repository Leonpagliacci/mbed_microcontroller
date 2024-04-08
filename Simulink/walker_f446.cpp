//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// File: walker_f446.cpp
//
// Code generated for Simulink model 'walker_f446'.
//
// Model version                  : 4.345
// Simulink Coder version         : 9.7 (R2022a) 13-Nov-2021
// C/C++ source code generated on : Fri Feb 17 16:20:19 2023
//
// Target selection: ert.tlc
// Embedded hardware selection: ARM Compatible->ARM Cortex-M
// Code generation objectives:
//    1. Execution efficiency
//    2. Traceability
// Validation result: Not run
//
#include "walker_f446.h"
#include <cmath>
#include "rtwtypes.h"
#include "walker_f446_capi.h"

// Private macros used by the generated code to access rtModel
#ifndef rtmIsMajorTimeStep
#define rtmIsMajorTimeStep(rtm)        (((rtm)->Timing.simTimeStep) == MAJOR_TIME_STEP)
#endif

#ifndef rtmIsMinorTimeStep
#define rtmIsMinorTimeStep(rtm)        (((rtm)->Timing.simTimeStep) == MINOR_TIME_STEP)
#endif

#ifndef rtmSetTPtr
#define rtmSetTPtr(rtm, val)           ((rtm)->Timing.t = (val))
#endif

// Model step function
void Model::step()
{
  // local block i/o variables
  real32_T rtb_FilterCoefficient;
  real32_T rtb_FilterCoefficient_e;
  real32_T rtb_NProdOut;
  real32_T rtb_IProdOut;

  {
    real_T lastSin_tmp;
    real_T rtb_Clock1;
    int32_T rtb_SineWave2;
    real32_T Gain1;
    real32_T Gain2;
    real32_T rtb_Filter_a;
    real32_T rtb_Filter_e;
    real32_T rtb_MultiportSwitch;

    // DiscretePulseGenerator: '<S4>/Discrete Pulse Generator'
    rtb_SineWave2 = ((rtDW.clockTickCounter < 50) && (rtDW.clockTickCounter >= 0));
    if (rtDW.clockTickCounter >= 99) {
      rtDW.clockTickCounter = 0;
    } else {
      rtDW.clockTickCounter++;
    }

    // End of DiscretePulseGenerator: '<S4>/Discrete Pulse Generator'

    // Outport: '<Root>/LED States' incorporates:
    //   DataTypeConversion: '<S4>/Data Type Conversion'

    rtY.LEDStates[0] = rtb_SineWave2;
    rtY.LEDStates[1] = rtb_SineWave2;

    // Clock: '<S5>/Clock1'
    rtb_Clock1 = (&rtM)->Timing.t[0];

    // Sin: '<S4>/Sine Wave3'
    if (rtDW.systemEnable != 0) {
      lastSin_tmp = 3.1415926535897931 * (((&rtM)->Timing.clockTick1) * 0.002);
      rtDW.lastSin = std::sin(lastSin_tmp);
      rtDW.lastCos = std::cos(lastSin_tmp);
      rtDW.systemEnable = 0;
    }

    // Sin: '<S4>/Sine Wave4'
    if (rtDW.systemEnable_d != 0) {
      lastSin_tmp = 6.2831853071795862 * (((&rtM)->Timing.clockTick1) * 0.002);
      rtDW.lastSin_a = std::sin(lastSin_tmp);
      rtDW.lastCos_f = std::cos(lastSin_tmp);
      rtDW.systemEnable_d = 0;
    }

    // Sin: '<S4>/Sine Wave2'
    if (rtDW.systemEnable_f != 0) {
      rtDW.lastSin_i = std::sin(3.1415926535897931 * (((&rtM)->Timing.clockTick1)
        * 0.002));
      rtDW.lastCos_n = std::cos(3.1415926535897931 * (((&rtM)->Timing.clockTick1)
        * 0.002));
      rtDW.systemEnable_f = 0;
    }

    // MultiPortSwitch: '<S4>/Multiport Switch' incorporates:
    //   Constant: '<S4>/Constant10'
    //   Constant: '<S4>/Constant2'
    //   Constant: '<S4>/Constant3'
    //   Constant: '<S4>/Constant4'
    //   Constant: '<S4>/Constant5'
    //   Constant: '<S4>/Constant6'
    //   Constant: '<S4>/Constant9'
    //   Constant: '<S5>/initialFreq'
    //   DataTypeConversion: '<S4>/Data Type Conversion4'
    //   DataTypeConversion: '<S4>/Data Type Conversion5'
    //   DataTypeConversion: '<S4>/Data Type Conversion7'
    //   DataTypeConversion: '<S4>/Data Type Conversion8'
    //   Gain: '<S4>/Gain13'
    //   Gain: '<S4>/Gain14'
    //   Gain: '<S4>/Gain4'
    //   Gain: '<S4>/Gain6'
    //   Gain: '<S4>/Gain7'
    //   Gain: '<S4>/Gain8'
    //   Gain: '<S4>/Gain9'
    //   Inport: '<Root>/BWS'
    //   Inport: '<Root>/Mode'
    //   Product: '<S5>/Product1'
    //   Product: '<S5>/Product2'
    //   Sin: '<S4>/Sine Wave2'
    //   Sin: '<S4>/Sine Wave3'
    //   Sin: '<S4>/Sine Wave4'
    //   Sum: '<S4>/Sum2'
    //   Sum: '<S4>/Sum3'
    //   Sum: '<S4>/Sum4'
    //   Sum: '<S4>/Sum5'
    //   Sum: '<S4>/Sum7'
    //   Sum: '<S4>/Sum8'
    //   Sum: '<S4>/Sum9'
    //   Sum: '<S5>/Sum'
    //   Trigonometry: '<S5>/Output'

    switch (rtU.Mode) {
     case 1:
      rtb_MultiportSwitch = static_cast<real32_T>((rtDW.lastSin *
        0.99998026085613712 + rtDW.lastCos * -0.0062831439655589511) *
        0.99998026085613712 + (rtDW.lastCos * 0.99998026085613712 - rtDW.lastSin
        * -0.0062831439655589511) * 0.0062831439655589511) * 250.0F;
      break;

     case 2:
      rtb_MultiportSwitch = (static_cast<real32_T>((rtDW.lastSin_a *
        0.99992104420381611 + rtDW.lastCos_f * -0.012566039883352607) *
        0.99992104420381611 + (rtDW.lastCos_f * 0.99992104420381611 -
        rtDW.lastSin_a * -0.012566039883352607) * 0.012566039883352607) * 0.5F +
        0.5F) * 9000.0F + 2000.0F;
      break;

     case 3:
      rtb_MultiportSwitch = (static_cast<real32_T>((rtDW.lastSin_i *
        0.99998026085613712 + rtDW.lastCos_n * -0.0062831439655589511) *
        0.99998026085613712 + (rtDW.lastCos_n * 0.99998026085613712 -
        rtDW.lastSin_i * -0.0062831439655589511) * 0.0062831439655589511) * 0.5F
        + 0.5F) * -3000.0F - 2000.0F;
      break;

     case 4:
      rtb_MultiportSwitch = (static_cast<real32_T>(std::sin((rtb_Clock1 *
        rtConstB.Gain + 0.62831853071795862) * rtb_Clock1)) * 0.5F + 0.5F) *
        9000.0F + -3000.0F;
      break;

     case 5:
      rtb_MultiportSwitch = rtU.BWS + 69.0F;
      break;

     default:
      rtb_MultiportSwitch = 69.0F;
      break;
    }

    // End of MultiPortSwitch: '<S4>/Multiport Switch'

    // Product: '<S4>/Product' incorporates:
    //   Inport: '<Root>/EnableAssist'

    rtb_MultiportSwitch *= static_cast<real32_T>(rtU.EnableAssist);

    // Gain: '<S4>/Gain1' incorporates:
    //   Constant: '<S4>/Offset'
    //   Inport: '<Root>/SpringEncoder1'
    //   Sum: '<S4>/Sum'

    Gain1 = (rtU.SpringEncoder1 - 1494.42798F) * 17.07F;

    // RateLimiter: '<S9>/Rate Limiter' incorporates:
    //   Inport: '<Root>/FwdBckAssist'

    rtb_Filter_e = rtU.FwdBckAssist - rtDW.PrevY;
    if (rtb_Filter_e > 0.02F) {
      rtb_Filter_e = rtDW.PrevY + 0.02F;
    } else if (rtb_Filter_e < -0.2F) {
      rtb_Filter_e = rtDW.PrevY + -0.2F;
    } else {
      rtb_Filter_e = rtU.FwdBckAssist;
    }

    rtDW.PrevY = rtb_Filter_e;

    // End of RateLimiter: '<S9>/Rate Limiter'

    // Sum: '<S9>/Sum2' incorporates:
    //   Gain: '<S9>/Gain'
    //   Inport: '<Root>/SpringEncoder2'

    rtDW.Ffwd = 0.002828F * rtU.SpringEncoder2 + rtb_Filter_e;

    // Gain: '<S9>/Gain1' incorporates:
    //   Inport: '<Root>/SpringEncoder3'

    rtDW.Fside = -0.002828F * rtU.SpringEncoder3;

    // Sum: '<S15>/Add1' incorporates:
    //   Constant: '<S15>/Filter_Constant'
    //   Constant: '<S15>/One'
    //   Inport: '<Root>/Left Wheel Velocity'
    //   Product: '<S15>/Product'
    //   Product: '<S15>/Product1'
    //   UnitDelay: '<S15>/Unit Delay'

    rtDW.UnitDelay_DSTATE = rtU.LeftWheelVelocity * 0.1F + 0.9F *
      rtDW.UnitDelay_DSTATE;

    // Sum: '<S12>/Add1' incorporates:
    //   Constant: '<S12>/Filter_Constant'
    //   Constant: '<S12>/One'
    //   Inport: '<Root>/Right Wheel Velocity'
    //   Product: '<S12>/Product'
    //   Product: '<S12>/Product1'
    //   UnitDelay: '<S12>/Unit Delay'

    rtDW.UnitDelay_DSTATE_n = rtU.RightWheelVelocity * 0.1F + 0.9F *
      rtDW.UnitDelay_DSTATE_n;

    // Product: '<S69>/Product1' incorporates:
    //   DiscreteTransferFcn: '<S65>/Discrete Transfer Fcn'

    rtb_Filter_e = 0.000132448389F * rtDW.DiscreteTransferFcn_states *
      rtConstB.Gain_h;

    // Product: '<S69>/Product2' incorporates:
    //   Constant: '<S69>/L'
    //   DiscreteTransferFcn: '<S68>/Discrete Transfer Fcn'

    rtb_Filter_a = 0.000132448389F * rtDW.DiscreteTransferFcn_states_e * 1.08F;

    // Gain: '<S9>/Gain2' incorporates:
    //   Product: '<S69>/Product3'
    //   Sum: '<S69>/Sum'
    //   UnaryMinus: '<S9>/Unary Minus'

    Gain2 = -((rtb_Filter_e + rtb_Filter_a) * rtConstB.Reciprocal1) *
      95.4929886F;

    // Gain: '<S9>/Gain4' incorporates:
    //   Product: '<S69>/Product4'
    //   Sum: '<S69>/Sum1'

    rtb_Filter_e = (rtb_Filter_e - rtb_Filter_a) * rtConstB.Reciprocal1 *
      95.4929886F;

    // Outport: '<Root>/LogData' incorporates:
    //   UnitDelay: '<S12>/Unit Delay'
    //   UnitDelay: '<S15>/Unit Delay'

    rtY.LogData[0] = Gain1;
    rtY.LogData[1] = rtDW.Ffwd;
    rtY.LogData[2] = rtDW.Fside;
    rtY.LogData[3] = rtb_MultiportSwitch;
    rtY.LogData[4] = rtDW.UnitDelay_DSTATE;
    rtY.LogData[5] = rtDW.UnitDelay_DSTATE_n;
    rtY.LogData[6] = Gain2;
    rtY.LogData[7] = rtb_Filter_e;

    // Sum: '<S9>/Sum1' incorporates:
    //   UnitDelay: '<S12>/Unit Delay'

    rtb_Filter_a = rtb_Filter_e - rtDW.UnitDelay_DSTATE_n;

    // Gain: '<S152>/Filter Coefficient' incorporates:
    //   DiscreteIntegrator: '<S144>/Filter'
    //   Gain: '<S143>/Derivative Gain'
    //   Sum: '<S144>/SumD'

    rtb_FilterCoefficient = (0.8F * rtb_Filter_a - rtDW.Filter_DSTATE) * 100.0F;

    // Outport: '<Root>/Right Wheel Torque' incorporates:
    //   Gain: '<S154>/Proportional Gain'
    //   Sum: '<S158>/Sum'

    rtY.RightWheelTorque = 100.0F * rtb_Filter_a + rtb_FilterCoefficient;

    // Sum: '<S9>/Sum' incorporates:
    //   UnitDelay: '<S15>/Unit Delay'

    rtb_Filter_e = Gain2 - rtDW.UnitDelay_DSTATE;

    // Gain: '<S104>/Filter Coefficient' incorporates:
    //   DiscreteIntegrator: '<S96>/Filter'
    //   Gain: '<S95>/Derivative Gain'
    //   Sum: '<S96>/SumD'

    rtb_FilterCoefficient_e = (0.8F * rtb_Filter_e - rtDW.Filter_DSTATE_m) *
      100.0F;

    // Outport: '<Root>/Left Wheel Torque' incorporates:
    //   Gain: '<S106>/Proportional Gain'
    //   Sum: '<S110>/Sum'

    rtY.LeftWheelTorque = 100.0F * rtb_Filter_e + rtb_FilterCoefficient_e;

    // Sum: '<S8>/Sum'
    Gain1 = rtb_MultiportSwitch - Gain1;

    // Product: '<S51>/NProd Out' incorporates:
    //   DiscreteIntegrator: '<S43>/Filter'
    //   Inport: '<Root>/Kd'
    //   Inport: '<Root>/N'
    //   Product: '<S42>/DProd Out'
    //   Sum: '<S43>/SumD'

    rtb_NProdOut = (Gain1 * rtU.Kd - rtDW.Filter_DSTATE_d) * rtU.N;

    // Sum: '<S8>/Sum5' incorporates:
    //   DiscreteIntegrator: '<S48>/Integrator'
    //   Inport: '<Root>/Kff'
    //   Inport: '<Root>/Kp'
    //   Product: '<S53>/PProd Out'
    //   Product: '<S8>/Product1'
    //   Sum: '<S57>/Sum'

    rtY.LinearMotorCommand = ((Gain1 * rtU.Kp + rtDW.Integrator_DSTATE) +
      rtb_NProdOut) + rtb_MultiportSwitch * rtU.Kff;

    // Saturate: '<S4>/Saturation'
    if (rtY.LinearMotorCommand > 1000.0F) {
      // Sum: '<S8>/Sum5' incorporates:
      //   Outport: '<Root>/Linear Motor Command'

      rtY.LinearMotorCommand = 1000.0F;
    } else if (rtY.LinearMotorCommand < -1000.0F) {
      // Sum: '<S8>/Sum5' incorporates:
      //   Outport: '<Root>/Linear Motor Command'

      rtY.LinearMotorCommand = -1000.0F;
    }

    // End of Saturate: '<S4>/Saturation'

    // Product: '<S45>/IProd Out' incorporates:
    //   Inport: '<Root>/Ki'

    rtb_IProdOut = Gain1 * rtU.Ki;
  }

  {
    real_T HoldSine;

    // Update for Sin: '<S4>/Sine Wave3'
    HoldSine = rtDW.lastSin;
    rtDW.lastSin = rtDW.lastSin * 0.99998026085613712 + rtDW.lastCos *
      0.0062831439655589511;
    rtDW.lastCos = rtDW.lastCos * 0.99998026085613712 - HoldSine *
      0.0062831439655589511;

    // Update for Sin: '<S4>/Sine Wave4'
    HoldSine = rtDW.lastSin_a;
    rtDW.lastSin_a = rtDW.lastSin_a * 0.99992104420381611 + rtDW.lastCos_f *
      0.012566039883352607;
    rtDW.lastCos_f = rtDW.lastCos_f * 0.99992104420381611 - HoldSine *
      0.012566039883352607;

    // Update for Sin: '<S4>/Sine Wave2'
    HoldSine = rtDW.lastSin_i;
    rtDW.lastSin_i = rtDW.lastSin_i * 0.99998026085613712 + rtDW.lastCos_n *
      0.0062831439655589511;
    rtDW.lastCos_n = rtDW.lastCos_n * 0.99998026085613712 - HoldSine *
      0.0062831439655589511;

    // Update for DiscreteTransferFcn: '<S65>/Discrete Transfer Fcn'
    rtDW.DiscreteTransferFcn_states = rtDW.Ffwd - -0.986755133F *
      rtDW.DiscreteTransferFcn_states;

    // Update for DiscreteTransferFcn: '<S68>/Discrete Transfer Fcn'
    rtDW.DiscreteTransferFcn_states_e = rtDW.Fside - -0.986755133F *
      rtDW.DiscreteTransferFcn_states_e;

    // Update for DiscreteIntegrator: '<S144>/Filter'
    rtDW.Filter_DSTATE += 0.002F * rtb_FilterCoefficient;

    // Update for DiscreteIntegrator: '<S96>/Filter'
    rtDW.Filter_DSTATE_m += 0.002F * rtb_FilterCoefficient_e;

    // Update for DiscreteIntegrator: '<S48>/Integrator'
    rtDW.Integrator_DSTATE += rtb_IProdOut;

    // Update for DiscreteIntegrator: '<S43>/Filter'
    rtDW.Filter_DSTATE_d += 0.002F * rtb_NProdOut;
  }

  // Update absolute time for base rate
  // The "clockTick0" counts the number of times the code of this task has
  //  been executed. The absolute time is the multiplication of "clockTick0"
  //  and "Timing.stepSize0". Size of "clockTick0" ensures timer will not
  //  overflow during the application lifespan selected.

  (&rtM)->Timing.t[0] =
    ((time_T)(++(&rtM)->Timing.clockTick0)) * (&rtM)->Timing.stepSize0;

  {
    // Update absolute timer for sample time: [0.002s, 0.0s]
    // The "clockTick1" counts the number of times the code of this task has
    //  been executed. The resolution of this integer timer is 0.002, which is the step size
    //  of the task. Size of "clockTick1" ensures timer will not overflow during the
    //  application lifespan selected.

    (&rtM)->Timing.clockTick1++;
  }
}

// Model initialize function
void Model::initialize()
{
  // Registration code
  {
    // Setup solver object
    rtsiSetSimTimeStepPtr(&(&rtM)->solverInfo, &(&rtM)->Timing.simTimeStep);
    rtsiSetTPtr(&(&rtM)->solverInfo, &rtmGetTPtr((&rtM)));
    rtsiSetStepSizePtr(&(&rtM)->solverInfo, &(&rtM)->Timing.stepSize0);
    rtsiSetErrorStatusPtr(&(&rtM)->solverInfo, (&rtmGetErrorStatus((&rtM))));
    rtsiSetRTModelPtr(&(&rtM)->solverInfo, (&rtM));
  }

  rtsiSetSimTimeStep(&(&rtM)->solverInfo, MAJOR_TIME_STEP);
  rtsiSetSolverName(&(&rtM)->solverInfo,"FixedStepDiscrete");
  rtmSetTPtr((&rtM), &(&rtM)->Timing.tArray[0]);
  (&rtM)->Timing.stepSize0 = 0.002;

  // Initialize DataMapInfo substructure containing ModelMap for C API
  walker_f446_InitializeDataMapInfo((&rtM), &rtDW);

  // Enable for Sin: '<S4>/Sine Wave3'
  rtDW.systemEnable = 1;

  // Enable for Sin: '<S4>/Sine Wave4'
  rtDW.systemEnable_d = 1;

  // Enable for Sin: '<S4>/Sine Wave2'
  rtDW.systemEnable_f = 1;
}

// Constructor
Model::Model() :
  rtU(),
  rtY(),
  rtDW(),
  rtM()
{
  // Currently there is no constructor body generated.
}

// Destructor
Model::~Model()
{
  // Currently there is no destructor body generated.
}

// Real-Time Model get method
Model::RT_MODEL * Model::getRTM()
{
  return (&rtM);
}

//
// File trailer for generated code.
//
// [EOF]
//
