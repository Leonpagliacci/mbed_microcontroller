//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// File: walker_f446.h
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
#ifndef RTW_HEADER_walker_f446_h_
#define RTW_HEADER_walker_f446_h_
#include "rtwtypes.h"
#include "rtw_continuous.h"
#include "rtw_solver.h"
#include "rtw_modelmap.h"
#include <stddef.h>

// Model Code Variants

// Macros for accessing real-time model data structure
#ifndef rtmGetDataMapInfo
#define rtmGetDataMapInfo(rtm)         ((rtm)->DataMapInfo)
#endif

#ifndef rtmSetDataMapInfo
#define rtmSetDataMapInfo(rtm, val)    ((rtm)->DataMapInfo = (val))
#endif

#ifndef rtmGetErrorStatus
#define rtmGetErrorStatus(rtm)         ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
#define rtmSetErrorStatus(rtm, val)    ((rtm)->errorStatus = (val))
#endif

#ifndef rtmGetT
#define rtmGetT(rtm)                   (rtmGetTPtr((rtm))[0])
#endif

#ifndef rtmGetTPtr
#define rtmGetTPtr(rtm)                ((rtm)->Timing.t)
#endif

// Function to get C API Model Mapping Static Info
extern const rtwCAPI_ModelMappingStaticInfo*
  walker_f446_GetCAPIStaticMap(void);

// Class declaration for model walker_f446
class Model
{
  // public data and function members
 public:
  // Block signals and states (default storage) for system '<Root>'
  struct DW {
    real_T lastSin;                    // '<S4>/Sine Wave3'
    real_T lastCos;                    // '<S4>/Sine Wave3'
    real_T lastSin_a;                  // '<S4>/Sine Wave4'
    real_T lastCos_f;                  // '<S4>/Sine Wave4'
    real_T lastSin_i;                  // '<S4>/Sine Wave2'
    real_T lastCos_n;                  // '<S4>/Sine Wave2'
    real32_T Ffwd;                     // '<S9>/Sum2'
    real32_T Fside;                    // '<S9>/Gain1'
    real32_T UnitDelay_DSTATE;         // '<S15>/Unit Delay'
    real32_T UnitDelay_DSTATE_n;       // '<S12>/Unit Delay'
    real32_T DiscreteTransferFcn_states;// '<S65>/Discrete Transfer Fcn'
    real32_T DiscreteTransferFcn_states_e;// '<S68>/Discrete Transfer Fcn'
    real32_T Filter_DSTATE;            // '<S144>/Filter'
    real32_T Filter_DSTATE_m;          // '<S96>/Filter'
    real32_T Integrator_DSTATE;        // '<S48>/Integrator'
    real32_T Filter_DSTATE_d;          // '<S43>/Filter'
    real32_T PrevY;                    // '<S9>/Rate Limiter'
    int32_T clockTickCounter;          // '<S4>/Discrete Pulse Generator'
    int32_T systemEnable;              // '<S4>/Sine Wave3'
    int32_T systemEnable_d;            // '<S4>/Sine Wave4'
    int32_T systemEnable_f;            // '<S4>/Sine Wave2'
  };

  // Invariant block signals (default storage)
  struct ConstB {
    real_T Product;                    // '<S5>/Product'
    real_T Gain;                       // '<S5>/Gain'
    real32_T Gain_h;                   // '<S69>/Gain'
    real32_T Reciprocal1;              // '<S69>/Reciprocal1'
  };

  // External inputs (root inport signals with default storage)
  struct ExtU {
    real32_T TimeSignal;               // '<Root>/TimeSignal'
    real32_T IMU1[6];                  // '<Root>/IMU1'
    real32_T IMU2[6];                  // '<Root>/IMU2'
    int32_T Mode;                      // '<Root>/Mode'
    int32_T EnableAssist;              // '<Root>/EnableAssist'
    real32_T IMU3[6];                  // '<Root>/IMU3'
    real32_T LeftWheelVelocity;        // '<Root>/Left Wheel Velocity'
    real32_T RightWheelVelocity;       // '<Root>/Right Wheel Velocity'
    real32_T SpringEncoder1;           // '<Root>/SpringEncoder1'
    real32_T SpringEncoder2;           // '<Root>/SpringEncoder2'
    real32_T Kp;                       // '<Root>/Kp'
    real32_T Kd;                       // '<Root>/Kd'
    real32_T Ki;                       // '<Root>/Ki'
    real32_T Kff;                      // '<Root>/Kff'
    real32_T SpringEncoder3;           // '<Root>/SpringEncoder3'
    real32_T N;                        // '<Root>/N'
    real32_T BWS;                      // '<Root>/BWS'
    real32_T FwdBckAssist;             // '<Root>/FwdBckAssist'
  };

  // External outputs (root outports fed by signals with default storage)
  struct ExtY {
    real32_T LogData[8];               // '<Root>/LogData'
    int32_T LEDStates[2];              // '<Root>/LED States'
    real32_T LeftWheelTorque;          // '<Root>/Left Wheel Torque'
    real32_T RightWheelTorque;         // '<Root>/Right Wheel Torque'
    real32_T LinearMotorCommand;       // '<Root>/Linear Motor Command'
  };

  // Real-time Model Data Structure
  struct RT_MODEL {
    const char_T *errorStatus;
    RTWSolverInfo solverInfo;

    //
    //  DataMapInfo:
    //  The following substructure contains information regarding
    //  structures generated in the model's C API.

    struct {
      rtwCAPI_ModelMappingInfo mmi;
      void* dataAddress[18];
      int32_T* vardimsAddress[18];
      RTWLoggingFcnPtr loggingPtrs[18];
    } DataMapInfo;

    //
    //  Timing:
    //  The following substructure contains information regarding
    //  the timing information for the model.

    struct {
      uint32_T clockTick0;
      time_T stepSize0;
      uint32_T clockTick1;
      SimTimeStep simTimeStep;
      time_T *t;
      time_T tArray[2];
    } Timing;
  };

  // Real-Time Model get method
  Model::RT_MODEL * getRTM();

  // Root inports set method
  void setExternalInputs(const ExtU *pExtU)
  {
    rtU = *pExtU;
  }

  // Root outports get method
  const ExtY &getExternalOutputs() const
  {
    return rtY;
  }

  // model initialize function
  void initialize();

  // model step function
  void step();

  // Constructor
  Model();

  // Destructor
  ~Model();

  // private data and function members
 private:
  // External inputs
  ExtU rtU;

  // External outputs
  ExtY rtY;

  // Block states
  DW rtDW;

  // Real-Time Model
  RT_MODEL rtM;
};

extern const Model::ConstB rtConstB;   // constant block i/o

//-
//  These blocks were eliminated from the model due to optimizations:
//
//  Block '<S12>/Data Type Duplicate' : Unused code path elimination
//  Block '<S15>/Data Type Duplicate' : Unused code path elimination
//  Block '<S65>/Gain' : Unused code path elimination


//-
//  The generated code includes comments that allow you to trace directly
//  back to the appropriate location in the model.  The basic format
//  is <system>/block_name, where system is the system number (uniquely
//  assigned by Simulink) and block_name is the name of the block.
//
//  Use the MATLAB hilite_system command to trace the generated code back
//  to the model.  For example,
//
//  hilite_system('<S3>')    - opens system 3
//  hilite_system('<S3>/Kp') - opens and selects block Kp which resides in S3
//
//  Here is the system hierarchy for this model
//
//  '<Root>' : 'walker_f446'
//  '<S1>'   : 'walker_f446/IMU Data Conversion'
//  '<S2>'   : 'walker_f446/IMU Data Conversion1'
//  '<S3>'   : 'walker_f446/IMU Data Conversion2'
//  '<S4>'   : 'walker_f446/Walker Controller'
//  '<S5>'   : 'walker_f446/Walker Controller/Chirp Signal3'
//  '<S6>'   : 'walker_f446/Walker Controller/IIR Filter'
//  '<S7>'   : 'walker_f446/Walker Controller/IIR Filter1'
//  '<S8>'   : 'walker_f446/Walker Controller/SEA Controller'
//  '<S9>'   : 'walker_f446/Walker Controller/Wheel Control'
//  '<S10>'  : 'walker_f446/Walker Controller/IIR Filter/IIR Filter'
//  '<S11>'  : 'walker_f446/Walker Controller/IIR Filter/IIR Filter/Low-pass'
//  '<S12>'  : 'walker_f446/Walker Controller/IIR Filter/IIR Filter/Low-pass/IIR Low Pass Filter'
//  '<S13>'  : 'walker_f446/Walker Controller/IIR Filter1/IIR Filter'
//  '<S14>'  : 'walker_f446/Walker Controller/IIR Filter1/IIR Filter/Low-pass'
//  '<S15>'  : 'walker_f446/Walker Controller/IIR Filter1/IIR Filter/Low-pass/IIR Low Pass Filter'
//  '<S16>'  : 'walker_f446/Walker Controller/SEA Controller/Discrete PID Controller'
//  '<S17>'  : 'walker_f446/Walker Controller/SEA Controller/Discrete PID Controller/Anti-windup'
//  '<S18>'  : 'walker_f446/Walker Controller/SEA Controller/Discrete PID Controller/D Gain'
//  '<S19>'  : 'walker_f446/Walker Controller/SEA Controller/Discrete PID Controller/Filter'
//  '<S20>'  : 'walker_f446/Walker Controller/SEA Controller/Discrete PID Controller/Filter ICs'
//  '<S21>'  : 'walker_f446/Walker Controller/SEA Controller/Discrete PID Controller/I Gain'
//  '<S22>'  : 'walker_f446/Walker Controller/SEA Controller/Discrete PID Controller/Ideal P Gain'
//  '<S23>'  : 'walker_f446/Walker Controller/SEA Controller/Discrete PID Controller/Ideal P Gain Fdbk'
//  '<S24>'  : 'walker_f446/Walker Controller/SEA Controller/Discrete PID Controller/Integrator'
//  '<S25>'  : 'walker_f446/Walker Controller/SEA Controller/Discrete PID Controller/Integrator ICs'
//  '<S26>'  : 'walker_f446/Walker Controller/SEA Controller/Discrete PID Controller/N Copy'
//  '<S27>'  : 'walker_f446/Walker Controller/SEA Controller/Discrete PID Controller/N Gain'
//  '<S28>'  : 'walker_f446/Walker Controller/SEA Controller/Discrete PID Controller/P Copy'
//  '<S29>'  : 'walker_f446/Walker Controller/SEA Controller/Discrete PID Controller/Parallel P Gain'
//  '<S30>'  : 'walker_f446/Walker Controller/SEA Controller/Discrete PID Controller/Reset Signal'
//  '<S31>'  : 'walker_f446/Walker Controller/SEA Controller/Discrete PID Controller/Saturation'
//  '<S32>'  : 'walker_f446/Walker Controller/SEA Controller/Discrete PID Controller/Saturation Fdbk'
//  '<S33>'  : 'walker_f446/Walker Controller/SEA Controller/Discrete PID Controller/Sum'
//  '<S34>'  : 'walker_f446/Walker Controller/SEA Controller/Discrete PID Controller/Sum Fdbk'
//  '<S35>'  : 'walker_f446/Walker Controller/SEA Controller/Discrete PID Controller/Tracking Mode'
//  '<S36>'  : 'walker_f446/Walker Controller/SEA Controller/Discrete PID Controller/Tracking Mode Sum'
//  '<S37>'  : 'walker_f446/Walker Controller/SEA Controller/Discrete PID Controller/Tsamp - Integral'
//  '<S38>'  : 'walker_f446/Walker Controller/SEA Controller/Discrete PID Controller/Tsamp - Ngain'
//  '<S39>'  : 'walker_f446/Walker Controller/SEA Controller/Discrete PID Controller/postSat Signal'
//  '<S40>'  : 'walker_f446/Walker Controller/SEA Controller/Discrete PID Controller/preSat Signal'
//  '<S41>'  : 'walker_f446/Walker Controller/SEA Controller/Discrete PID Controller/Anti-windup/Passthrough'
//  '<S42>'  : 'walker_f446/Walker Controller/SEA Controller/Discrete PID Controller/D Gain/External Parameters'
//  '<S43>'  : 'walker_f446/Walker Controller/SEA Controller/Discrete PID Controller/Filter/Disc. Forward Euler Filter'
//  '<S44>'  : 'walker_f446/Walker Controller/SEA Controller/Discrete PID Controller/Filter ICs/Internal IC - Filter'
//  '<S45>'  : 'walker_f446/Walker Controller/SEA Controller/Discrete PID Controller/I Gain/External Parameters'
//  '<S46>'  : 'walker_f446/Walker Controller/SEA Controller/Discrete PID Controller/Ideal P Gain/Passthrough'
//  '<S47>'  : 'walker_f446/Walker Controller/SEA Controller/Discrete PID Controller/Ideal P Gain Fdbk/Disabled'
//  '<S48>'  : 'walker_f446/Walker Controller/SEA Controller/Discrete PID Controller/Integrator/Discrete'
//  '<S49>'  : 'walker_f446/Walker Controller/SEA Controller/Discrete PID Controller/Integrator ICs/Internal IC'
//  '<S50>'  : 'walker_f446/Walker Controller/SEA Controller/Discrete PID Controller/N Copy/Disabled'
//  '<S51>'  : 'walker_f446/Walker Controller/SEA Controller/Discrete PID Controller/N Gain/External Parameters'
//  '<S52>'  : 'walker_f446/Walker Controller/SEA Controller/Discrete PID Controller/P Copy/Disabled'
//  '<S53>'  : 'walker_f446/Walker Controller/SEA Controller/Discrete PID Controller/Parallel P Gain/External Parameters'
//  '<S54>'  : 'walker_f446/Walker Controller/SEA Controller/Discrete PID Controller/Reset Signal/Disabled'
//  '<S55>'  : 'walker_f446/Walker Controller/SEA Controller/Discrete PID Controller/Saturation/Passthrough'
//  '<S56>'  : 'walker_f446/Walker Controller/SEA Controller/Discrete PID Controller/Saturation Fdbk/Disabled'
//  '<S57>'  : 'walker_f446/Walker Controller/SEA Controller/Discrete PID Controller/Sum/Sum_PID'
//  '<S58>'  : 'walker_f446/Walker Controller/SEA Controller/Discrete PID Controller/Sum Fdbk/Disabled'
//  '<S59>'  : 'walker_f446/Walker Controller/SEA Controller/Discrete PID Controller/Tracking Mode/Disabled'
//  '<S60>'  : 'walker_f446/Walker Controller/SEA Controller/Discrete PID Controller/Tracking Mode Sum/Passthrough'
//  '<S61>'  : 'walker_f446/Walker Controller/SEA Controller/Discrete PID Controller/Tsamp - Integral/Passthrough'
//  '<S62>'  : 'walker_f446/Walker Controller/SEA Controller/Discrete PID Controller/Tsamp - Ngain/Passthrough'
//  '<S63>'  : 'walker_f446/Walker Controller/SEA Controller/Discrete PID Controller/postSat Signal/Forward_Path'
//  '<S64>'  : 'walker_f446/Walker Controller/SEA Controller/Discrete PID Controller/preSat Signal/Forward_Path'
//  '<S65>'  : 'walker_f446/Walker Controller/Wheel Control/ Forward-Backward Admittance'
//  '<S66>'  : 'walker_f446/Walker Controller/Wheel Control/Discrete PID Controller'
//  '<S67>'  : 'walker_f446/Walker Controller/Wheel Control/Discrete PID Controller1'
//  '<S68>'  : 'walker_f446/Walker Controller/Wheel Control/Left-Right Admittance'
//  '<S69>'  : 'walker_f446/Walker Controller/Wheel Control/Walker Inverse Kinematics'
//  '<S70>'  : 'walker_f446/Walker Controller/Wheel Control/Discrete PID Controller/Anti-windup'
//  '<S71>'  : 'walker_f446/Walker Controller/Wheel Control/Discrete PID Controller/D Gain'
//  '<S72>'  : 'walker_f446/Walker Controller/Wheel Control/Discrete PID Controller/Filter'
//  '<S73>'  : 'walker_f446/Walker Controller/Wheel Control/Discrete PID Controller/Filter ICs'
//  '<S74>'  : 'walker_f446/Walker Controller/Wheel Control/Discrete PID Controller/I Gain'
//  '<S75>'  : 'walker_f446/Walker Controller/Wheel Control/Discrete PID Controller/Ideal P Gain'
//  '<S76>'  : 'walker_f446/Walker Controller/Wheel Control/Discrete PID Controller/Ideal P Gain Fdbk'
//  '<S77>'  : 'walker_f446/Walker Controller/Wheel Control/Discrete PID Controller/Integrator'
//  '<S78>'  : 'walker_f446/Walker Controller/Wheel Control/Discrete PID Controller/Integrator ICs'
//  '<S79>'  : 'walker_f446/Walker Controller/Wheel Control/Discrete PID Controller/N Copy'
//  '<S80>'  : 'walker_f446/Walker Controller/Wheel Control/Discrete PID Controller/N Gain'
//  '<S81>'  : 'walker_f446/Walker Controller/Wheel Control/Discrete PID Controller/P Copy'
//  '<S82>'  : 'walker_f446/Walker Controller/Wheel Control/Discrete PID Controller/Parallel P Gain'
//  '<S83>'  : 'walker_f446/Walker Controller/Wheel Control/Discrete PID Controller/Reset Signal'
//  '<S84>'  : 'walker_f446/Walker Controller/Wheel Control/Discrete PID Controller/Saturation'
//  '<S85>'  : 'walker_f446/Walker Controller/Wheel Control/Discrete PID Controller/Saturation Fdbk'
//  '<S86>'  : 'walker_f446/Walker Controller/Wheel Control/Discrete PID Controller/Sum'
//  '<S87>'  : 'walker_f446/Walker Controller/Wheel Control/Discrete PID Controller/Sum Fdbk'
//  '<S88>'  : 'walker_f446/Walker Controller/Wheel Control/Discrete PID Controller/Tracking Mode'
//  '<S89>'  : 'walker_f446/Walker Controller/Wheel Control/Discrete PID Controller/Tracking Mode Sum'
//  '<S90>'  : 'walker_f446/Walker Controller/Wheel Control/Discrete PID Controller/Tsamp - Integral'
//  '<S91>'  : 'walker_f446/Walker Controller/Wheel Control/Discrete PID Controller/Tsamp - Ngain'
//  '<S92>'  : 'walker_f446/Walker Controller/Wheel Control/Discrete PID Controller/postSat Signal'
//  '<S93>'  : 'walker_f446/Walker Controller/Wheel Control/Discrete PID Controller/preSat Signal'
//  '<S94>'  : 'walker_f446/Walker Controller/Wheel Control/Discrete PID Controller/Anti-windup/Disabled'
//  '<S95>'  : 'walker_f446/Walker Controller/Wheel Control/Discrete PID Controller/D Gain/Internal Parameters'
//  '<S96>'  : 'walker_f446/Walker Controller/Wheel Control/Discrete PID Controller/Filter/Disc. Forward Euler Filter'
//  '<S97>'  : 'walker_f446/Walker Controller/Wheel Control/Discrete PID Controller/Filter ICs/Internal IC - Filter'
//  '<S98>'  : 'walker_f446/Walker Controller/Wheel Control/Discrete PID Controller/I Gain/Disabled'
//  '<S99>'  : 'walker_f446/Walker Controller/Wheel Control/Discrete PID Controller/Ideal P Gain/Passthrough'
//  '<S100>' : 'walker_f446/Walker Controller/Wheel Control/Discrete PID Controller/Ideal P Gain Fdbk/Disabled'
//  '<S101>' : 'walker_f446/Walker Controller/Wheel Control/Discrete PID Controller/Integrator/Disabled'
//  '<S102>' : 'walker_f446/Walker Controller/Wheel Control/Discrete PID Controller/Integrator ICs/Disabled'
//  '<S103>' : 'walker_f446/Walker Controller/Wheel Control/Discrete PID Controller/N Copy/Disabled'
//  '<S104>' : 'walker_f446/Walker Controller/Wheel Control/Discrete PID Controller/N Gain/Internal Parameters'
//  '<S105>' : 'walker_f446/Walker Controller/Wheel Control/Discrete PID Controller/P Copy/Disabled'
//  '<S106>' : 'walker_f446/Walker Controller/Wheel Control/Discrete PID Controller/Parallel P Gain/Internal Parameters'
//  '<S107>' : 'walker_f446/Walker Controller/Wheel Control/Discrete PID Controller/Reset Signal/Disabled'
//  '<S108>' : 'walker_f446/Walker Controller/Wheel Control/Discrete PID Controller/Saturation/Passthrough'
//  '<S109>' : 'walker_f446/Walker Controller/Wheel Control/Discrete PID Controller/Saturation Fdbk/Disabled'
//  '<S110>' : 'walker_f446/Walker Controller/Wheel Control/Discrete PID Controller/Sum/Sum_PD'
//  '<S111>' : 'walker_f446/Walker Controller/Wheel Control/Discrete PID Controller/Sum Fdbk/Disabled'
//  '<S112>' : 'walker_f446/Walker Controller/Wheel Control/Discrete PID Controller/Tracking Mode/Disabled'
//  '<S113>' : 'walker_f446/Walker Controller/Wheel Control/Discrete PID Controller/Tracking Mode Sum/Passthrough'
//  '<S114>' : 'walker_f446/Walker Controller/Wheel Control/Discrete PID Controller/Tsamp - Integral/Disabled wSignal Specification'
//  '<S115>' : 'walker_f446/Walker Controller/Wheel Control/Discrete PID Controller/Tsamp - Ngain/Passthrough'
//  '<S116>' : 'walker_f446/Walker Controller/Wheel Control/Discrete PID Controller/postSat Signal/Forward_Path'
//  '<S117>' : 'walker_f446/Walker Controller/Wheel Control/Discrete PID Controller/preSat Signal/Forward_Path'
//  '<S118>' : 'walker_f446/Walker Controller/Wheel Control/Discrete PID Controller1/Anti-windup'
//  '<S119>' : 'walker_f446/Walker Controller/Wheel Control/Discrete PID Controller1/D Gain'
//  '<S120>' : 'walker_f446/Walker Controller/Wheel Control/Discrete PID Controller1/Filter'
//  '<S121>' : 'walker_f446/Walker Controller/Wheel Control/Discrete PID Controller1/Filter ICs'
//  '<S122>' : 'walker_f446/Walker Controller/Wheel Control/Discrete PID Controller1/I Gain'
//  '<S123>' : 'walker_f446/Walker Controller/Wheel Control/Discrete PID Controller1/Ideal P Gain'
//  '<S124>' : 'walker_f446/Walker Controller/Wheel Control/Discrete PID Controller1/Ideal P Gain Fdbk'
//  '<S125>' : 'walker_f446/Walker Controller/Wheel Control/Discrete PID Controller1/Integrator'
//  '<S126>' : 'walker_f446/Walker Controller/Wheel Control/Discrete PID Controller1/Integrator ICs'
//  '<S127>' : 'walker_f446/Walker Controller/Wheel Control/Discrete PID Controller1/N Copy'
//  '<S128>' : 'walker_f446/Walker Controller/Wheel Control/Discrete PID Controller1/N Gain'
//  '<S129>' : 'walker_f446/Walker Controller/Wheel Control/Discrete PID Controller1/P Copy'
//  '<S130>' : 'walker_f446/Walker Controller/Wheel Control/Discrete PID Controller1/Parallel P Gain'
//  '<S131>' : 'walker_f446/Walker Controller/Wheel Control/Discrete PID Controller1/Reset Signal'
//  '<S132>' : 'walker_f446/Walker Controller/Wheel Control/Discrete PID Controller1/Saturation'
//  '<S133>' : 'walker_f446/Walker Controller/Wheel Control/Discrete PID Controller1/Saturation Fdbk'
//  '<S134>' : 'walker_f446/Walker Controller/Wheel Control/Discrete PID Controller1/Sum'
//  '<S135>' : 'walker_f446/Walker Controller/Wheel Control/Discrete PID Controller1/Sum Fdbk'
//  '<S136>' : 'walker_f446/Walker Controller/Wheel Control/Discrete PID Controller1/Tracking Mode'
//  '<S137>' : 'walker_f446/Walker Controller/Wheel Control/Discrete PID Controller1/Tracking Mode Sum'
//  '<S138>' : 'walker_f446/Walker Controller/Wheel Control/Discrete PID Controller1/Tsamp - Integral'
//  '<S139>' : 'walker_f446/Walker Controller/Wheel Control/Discrete PID Controller1/Tsamp - Ngain'
//  '<S140>' : 'walker_f446/Walker Controller/Wheel Control/Discrete PID Controller1/postSat Signal'
//  '<S141>' : 'walker_f446/Walker Controller/Wheel Control/Discrete PID Controller1/preSat Signal'
//  '<S142>' : 'walker_f446/Walker Controller/Wheel Control/Discrete PID Controller1/Anti-windup/Disabled'
//  '<S143>' : 'walker_f446/Walker Controller/Wheel Control/Discrete PID Controller1/D Gain/Internal Parameters'
//  '<S144>' : 'walker_f446/Walker Controller/Wheel Control/Discrete PID Controller1/Filter/Disc. Forward Euler Filter'
//  '<S145>' : 'walker_f446/Walker Controller/Wheel Control/Discrete PID Controller1/Filter ICs/Internal IC - Filter'
//  '<S146>' : 'walker_f446/Walker Controller/Wheel Control/Discrete PID Controller1/I Gain/Disabled'
//  '<S147>' : 'walker_f446/Walker Controller/Wheel Control/Discrete PID Controller1/Ideal P Gain/Passthrough'
//  '<S148>' : 'walker_f446/Walker Controller/Wheel Control/Discrete PID Controller1/Ideal P Gain Fdbk/Disabled'
//  '<S149>' : 'walker_f446/Walker Controller/Wheel Control/Discrete PID Controller1/Integrator/Disabled'
//  '<S150>' : 'walker_f446/Walker Controller/Wheel Control/Discrete PID Controller1/Integrator ICs/Disabled'
//  '<S151>' : 'walker_f446/Walker Controller/Wheel Control/Discrete PID Controller1/N Copy/Disabled'
//  '<S152>' : 'walker_f446/Walker Controller/Wheel Control/Discrete PID Controller1/N Gain/Internal Parameters'
//  '<S153>' : 'walker_f446/Walker Controller/Wheel Control/Discrete PID Controller1/P Copy/Disabled'
//  '<S154>' : 'walker_f446/Walker Controller/Wheel Control/Discrete PID Controller1/Parallel P Gain/Internal Parameters'
//  '<S155>' : 'walker_f446/Walker Controller/Wheel Control/Discrete PID Controller1/Reset Signal/Disabled'
//  '<S156>' : 'walker_f446/Walker Controller/Wheel Control/Discrete PID Controller1/Saturation/Passthrough'
//  '<S157>' : 'walker_f446/Walker Controller/Wheel Control/Discrete PID Controller1/Saturation Fdbk/Disabled'
//  '<S158>' : 'walker_f446/Walker Controller/Wheel Control/Discrete PID Controller1/Sum/Sum_PD'
//  '<S159>' : 'walker_f446/Walker Controller/Wheel Control/Discrete PID Controller1/Sum Fdbk/Disabled'
//  '<S160>' : 'walker_f446/Walker Controller/Wheel Control/Discrete PID Controller1/Tracking Mode/Disabled'
//  '<S161>' : 'walker_f446/Walker Controller/Wheel Control/Discrete PID Controller1/Tracking Mode Sum/Passthrough'
//  '<S162>' : 'walker_f446/Walker Controller/Wheel Control/Discrete PID Controller1/Tsamp - Integral/Disabled wSignal Specification'
//  '<S163>' : 'walker_f446/Walker Controller/Wheel Control/Discrete PID Controller1/Tsamp - Ngain/Passthrough'
//  '<S164>' : 'walker_f446/Walker Controller/Wheel Control/Discrete PID Controller1/postSat Signal/Forward_Path'
//  '<S165>' : 'walker_f446/Walker Controller/Wheel Control/Discrete PID Controller1/preSat Signal/Forward_Path'


//-
//  Requirements for '<Root>': walker_f446

#endif                                 // RTW_HEADER_walker_f446_h_

//
// File trailer for generated code.
//
// [EOF]
//
