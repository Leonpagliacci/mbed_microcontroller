//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// File: walker_f446_capi.cpp
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
#include <stddef.h>
#include "rtw_capi.h"
#ifdef HOST_CAPI_BUILD
#include "walker_f446_capi_host.h"
#define sizeof(s)                      ((size_t)(0xFFFF))
#undef rt_offsetof
#define rt_offsetof(s,el)              ((uint16_T)(0xFFFF))
#define TARGET_CONST
#define TARGET_STRING(s)               (s)
#else                                  // HOST_CAPI_BUILD
#include "builtin_typeid_types.h"
#include "walker_f446.h"
#include "walker_f446_capi.h"
#ifdef LIGHT_WEIGHT_CAPI
#define TARGET_CONST
#define TARGET_STRING(s)               ((NULL))
#else
#define TARGET_CONST                   const
#define TARGET_STRING(s)               (s)
#endif
#endif                                 // HOST_CAPI_BUILD

// Block output signal information
static rtwCAPI_Signals rtBlockSignals[] = {
  // addrMapIndex, sysNum, blockPath,
  //  signalName, portNumber, dataTypeIndex, dimIndex, fxpIndex, sTimeIndex

  { 0, 0, TARGET_STRING("walker_f446/Walker Controller/Chirp Signal3/Gain"),
    TARGET_STRING(""), 0, 0, 0, 0, 0 },

  { 1, 0, TARGET_STRING("walker_f446/Walker Controller/Chirp Signal3/Product"),
    TARGET_STRING(""), 0, 0, 0, 0, 0 },

  { 2, 0, TARGET_STRING("walker_f446/Walker Controller/Wheel Control/Gain1"),
    TARGET_STRING("Fside"), 0, 1, 0, 0, 1 },

  { 3, 0, TARGET_STRING("walker_f446/Walker Controller/Wheel Control/Sum2"),
    TARGET_STRING("Ffwd"), 0, 1, 0, 0, 1 },

  { 4, 0, TARGET_STRING("walker_f446/Walker Controller/Wheel Control/Walker Inverse Kinematics/Gain"),
    TARGET_STRING(""), 0, 1, 0, 0, 0 },

  { 5, 0, TARGET_STRING("walker_f446/Walker Controller/Wheel Control/Walker Inverse Kinematics/Reciprocal1"),
    TARGET_STRING(""), 0, 1, 0, 0, 0 },

  { 6, 0, TARGET_STRING("walker_f446/Walker Controller/IIR Filter/IIR Filter/Low-pass/IIR Low Pass Filter/Add1"),
    TARGET_STRING(""), 0, 1, 0, 0, 1 },

  { 7, 0, TARGET_STRING("walker_f446/Walker Controller/IIR Filter/IIR Filter/Low-pass/IIR Low Pass Filter/Unit Delay"),
    TARGET_STRING(""), 0, 1, 0, 0, 1 },

  { 8, 0, TARGET_STRING("walker_f446/Walker Controller/IIR Filter1/IIR Filter/Low-pass/IIR Low Pass Filter/Add1"),
    TARGET_STRING(""), 0, 1, 0, 0, 1 },

  { 9, 0, TARGET_STRING("walker_f446/Walker Controller/IIR Filter1/IIR Filter/Low-pass/IIR Low Pass Filter/Unit Delay"),
    TARGET_STRING(""), 0, 1, 0, 0, 1 },

  {
    0, 0, (NULL), (NULL), 0, 0, 0, 0, 0
  }
};

// Individual block tuning is not valid when inline parameters is *
//  selected. An empty map is produced to provide a consistent     *
//  interface independent  of inlining parameters.                 *

static rtwCAPI_BlockParameters rtBlockParameters[] = {
  // addrMapIndex, blockPath,
  //  paramName, dataTypeIndex, dimIndex, fixPtIdx

  {
    0, (NULL), (NULL), 0, 0, 0
  }
};

// Block states information
static rtwCAPI_States rtBlockStates[] = {
  // addrMapIndex, contStateStartIndex, blockPath,
  //  stateName, pathAlias, dWorkIndex, dataTypeIndex, dimIndex,
  //  fixPtIdx, sTimeIndex, isContinuous, hierInfoIdx, flatElemIdx

  { 10, -1, TARGET_STRING(
    "walker_f446/Walker Controller/Wheel Control/ Forward-Backward\nAdmittance/Discrete\nTransfer Fcn"),
    TARGET_STRING("states"), "", 0, 1, 0, 0, 1, 0, -1, 0 },

  { 11, -1, TARGET_STRING(
    "walker_f446/Walker Controller/Wheel Control/Left-Right\nAdmittance/Discrete\nTransfer Fcn"),
    TARGET_STRING("states"), "", 0, 1, 0, 0, 1, 0, -1, 0 },

  { 12, -1, TARGET_STRING(
    "walker_f446/Walker Controller/IIR Filter/IIR Filter/Low-pass/IIR Low Pass Filter/Unit Delay"),
    TARGET_STRING("DSTATE"), "", 0, 1, 0, 0, 1, 0, -1, 0 },

  { 13, -1, TARGET_STRING(
    "walker_f446/Walker Controller/IIR Filter1/IIR Filter/Low-pass/IIR Low Pass Filter/Unit Delay"),
    TARGET_STRING("DSTATE"), "", 0, 1, 0, 0, 1, 0, -1, 0 },

  { 14, -1, TARGET_STRING(
    "walker_f446/Walker Controller/SEA Controller/Discrete PID Controller/Filter/Disc. Forward Euler Filter/Filter"),
    TARGET_STRING("DSTATE"), "", 0, 1, 0, 0, 1, 0, -1, 0 },

  { 15, -1, TARGET_STRING(
    "walker_f446/Walker Controller/SEA Controller/Discrete PID Controller/Integrator/Discrete/Integrator"),
    TARGET_STRING("DSTATE"), "", 0, 1, 0, 0, 1, 0, -1, 0 },

  { 16, -1, TARGET_STRING(
    "walker_f446/Walker Controller/Wheel Control/Discrete PID Controller/Filter/Disc. Forward Euler Filter/Filter"),
    TARGET_STRING("DSTATE"), "", 0, 1, 0, 0, 1, 0, -1, 0 },

  { 17, -1, TARGET_STRING(
    "walker_f446/Walker Controller/Wheel Control/Discrete PID Controller1/Filter/Disc. Forward Euler Filter/Filter"),
    TARGET_STRING("DSTATE"), "", 0, 1, 0, 0, 1, 0, -1, 0 },

  {
    0, -1, (NULL), (NULL), (NULL), 0, 0, 0, 0, 0, 0, -1, 0
  }
};

// Tunable variable parameters
static rtwCAPI_ModelParameters rtModelParameters[] = {
  // addrMapIndex, varName, dataTypeIndex, dimIndex, fixPtIndex
  { 0, (NULL), 0, 0, 0 }
};

#ifndef HOST_CAPI_BUILD

// Initialize Data Address
static void InitializeDataAddr(void* dataAddr[], Model::DW *rtDW)
{
  dataAddr[0] = (void*) (&rtConstB.Gain);
  dataAddr[1] = (void*) (&rtConstB.Product);
  dataAddr[2] = (void*) (&rtDW->Fside);
  dataAddr[3] = (void*) (&rtDW->Ffwd);
  dataAddr[4] = (void*) (&rtConstB.Gain_h);
  dataAddr[5] = (void*) (&rtConstB.Reciprocal1);
  dataAddr[6] = (void*) (&rtDW->UnitDelay_DSTATE_n);
  dataAddr[7] = (void*) (&rtDW->UnitDelay_DSTATE_n);
  dataAddr[8] = (void*) (&rtDW->UnitDelay_DSTATE);
  dataAddr[9] = (void*) (&rtDW->UnitDelay_DSTATE);
  dataAddr[10] = (void*) (&rtDW->DiscreteTransferFcn_states);
  dataAddr[11] = (void*) (&rtDW->DiscreteTransferFcn_states_e);
  dataAddr[12] = (void*) (&rtDW->UnitDelay_DSTATE_n);
  dataAddr[13] = (void*) (&rtDW->UnitDelay_DSTATE);
  dataAddr[14] = (void*) (&rtDW->Filter_DSTATE_d);
  dataAddr[15] = (void*) (&rtDW->Integrator_DSTATE);
  dataAddr[16] = (void*) (&rtDW->Filter_DSTATE_m);
  dataAddr[17] = (void*) (&rtDW->Filter_DSTATE);
}

#endif

// Initialize Data Run-Time Dimension Buffer Address
#ifndef HOST_CAPI_BUILD

static void InitializeVarDimsAddr(int32_T* vardimsAddr[])
{
  vardimsAddr[0] = (NULL);
}

#endif

#ifndef HOST_CAPI_BUILD

// Initialize logging function pointers
static void InitializeLoggingFunctions(RTWLoggingFcnPtr loggingPtrs[])
{
  loggingPtrs[0] = (NULL);
  loggingPtrs[1] = (NULL);
  loggingPtrs[2] = (NULL);
  loggingPtrs[3] = (NULL);
  loggingPtrs[4] = (NULL);
  loggingPtrs[5] = (NULL);
  loggingPtrs[6] = (NULL);
  loggingPtrs[7] = (NULL);
  loggingPtrs[8] = (NULL);
  loggingPtrs[9] = (NULL);
  loggingPtrs[10] = (NULL);
  loggingPtrs[11] = (NULL);
  loggingPtrs[12] = (NULL);
  loggingPtrs[13] = (NULL);
  loggingPtrs[14] = (NULL);
  loggingPtrs[15] = (NULL);
  loggingPtrs[16] = (NULL);
  loggingPtrs[17] = (NULL);
}

#endif

// Data Type Map - use dataTypeMapIndex to access this structure
static TARGET_CONST rtwCAPI_DataTypeMap rtDataTypeMap[] = {
  // cName, mwName, numElements, elemMapIndex, dataSize, slDataId, *
  //  isComplex, isPointer, enumStorageType
  { "double", "real_T", 0, 0, sizeof(real_T), (uint8_T)SS_DOUBLE, 0, 0, 0 },

  { "float", "real32_T", 0, 0, sizeof(real32_T), (uint8_T)SS_SINGLE, 0, 0, 0 }
};

#ifdef HOST_CAPI_BUILD
#undef sizeof
#endif

// Structure Element Map - use elemMapIndex to access this structure
static TARGET_CONST rtwCAPI_ElementMap rtElementMap[] = {
  // elementName, elementOffset, dataTypeIndex, dimIndex, fxpIndex
  { (NULL), 0, 0, 0, 0 },
};

// Dimension Map - use dimensionMapIndex to access elements of ths structure
static rtwCAPI_DimensionMap rtDimensionMap[] = {
  // dataOrientation, dimArrayIndex, numDims, vardimsIndex
  { rtwCAPI_SCALAR, 0, 2, 0 }
};

// Dimension Array- use dimArrayIndex to access elements of this array
static uint_T rtDimensionArray[] = {
  1,                                   // 0
  1                                    // 1
};

// C-API stores floating point values in an array. The elements of this  *
//  are unique. This ensures that values which are shared across the model*
//  are stored in the most efficient way. These values are referenced by  *
//            - rtwCAPI_FixPtMap.fracSlopePtr,                            *
//            - rtwCAPI_FixPtMap.biasPtr,                                 *
//            - rtwCAPI_SampleTimeMap.samplePeriodPtr,                    *
//            - rtwCAPI_SampleTimeMap.sampleOffsetPtr
static const real_T rtcapiStoredFloats[] = {
  0.002, 0.0
};

// Fixed Point Map
static rtwCAPI_FixPtMap rtFixPtMap[] = {
  // fracSlopePtr, biasPtr, scaleType, wordLength, exponent, isSigned
  { (NULL), (NULL), rtwCAPI_FIX_RESERVED, 0, 0, (boolean_T)0 },
};

// Sample Time Map - use sTimeIndex to access elements of ths structure
static rtwCAPI_SampleTimeMap rtSampleTimeMap[] = {
  // samplePeriodPtr, sampleOffsetPtr, tid, samplingMode
  { (NULL), (NULL), -2, 0 },

  { static_cast<const void *>(&rtcapiStoredFloats[0]), static_cast<const void *>
    (&rtcapiStoredFloats[1]), static_cast<int8_T>(1), static_cast<uint8_T>(0) }
};

static rtwCAPI_ModelMappingStaticInfo mmiStatic = {
  // Signals:{signals, numSignals,
  //            rootInputs, numRootInputs,
  //            rootOutputs, numRootOutputs},
  //  Params: {blockParameters, numBlockParameters,
  //           modelParameters, numModelParameters},
  //  States: {states, numStates},
  //  Maps:   {dataTypeMap, dimensionMap, fixPtMap,
  //           elementMap, sampleTimeMap, dimensionArray},
  //  TargetType: targetType

  { rtBlockSignals, 10,
    (NULL), 0,
    (NULL), 0 },

  { rtBlockParameters, 0,
    rtModelParameters, 0 },

  { rtBlockStates, 8 },

  { rtDataTypeMap, rtDimensionMap, rtFixPtMap,
    rtElementMap, rtSampleTimeMap, rtDimensionArray },
  "float",

  { 2360973993U,
    124937047U,
    1055999048U,
    3346620614U },
  (NULL), 0,
  (boolean_T)0
};

// Function to get C API Model Mapping Static Info
const rtwCAPI_ModelMappingStaticInfo*
  walker_f446_GetCAPIStaticMap(void)
{
  return &mmiStatic;
}

// Cache pointers into DataMapInfo substructure of RTModel
#ifndef HOST_CAPI_BUILD

void walker_f446_InitializeDataMapInfo(Model::RT_MODEL *const rtM, Model::DW
  *rtDW)
{
  // Set C-API version
  rtwCAPI_SetVersion(rtM->DataMapInfo.mmi, 1);

  // Cache static C-API data into the Real-time Model Data structure
  rtwCAPI_SetStaticMap(rtM->DataMapInfo.mmi, &mmiStatic);

  // Cache static C-API logging data into the Real-time Model Data structure
  rtwCAPI_SetLoggingStaticMap(rtM->DataMapInfo.mmi, (NULL));

  // Cache C-API Data Addresses into the Real-Time Model Data structure
  InitializeDataAddr(rtM->DataMapInfo.dataAddress, rtDW);
  rtwCAPI_SetDataAddressMap(rtM->DataMapInfo.mmi, rtM->DataMapInfo.dataAddress);

  // Cache C-API Data Run-Time Dimension Buffer Addresses into the Real-Time Model Data structure 
  InitializeVarDimsAddr(rtM->DataMapInfo.vardimsAddress);
  rtwCAPI_SetVarDimsAddressMap(rtM->DataMapInfo.mmi,
    rtM->DataMapInfo.vardimsAddress);

  // Set Instance specific path
  rtwCAPI_SetPath(rtM->DataMapInfo.mmi, (NULL));
  rtwCAPI_SetFullPath(rtM->DataMapInfo.mmi, (NULL));

  // Cache C-API logging function pointers into the Real-Time Model Data structure 
  InitializeLoggingFunctions(rtM->DataMapInfo.loggingPtrs);
  rtwCAPI_SetLoggingPtrs(rtM->DataMapInfo.mmi, rtM->DataMapInfo.loggingPtrs);

  // Cache the instance C-API logging pointer
  rtwCAPI_SetInstanceLoggingInfo(rtM->DataMapInfo.mmi, (NULL));

  // Set reference to submodels
  rtwCAPI_SetChildMMIArray(rtM->DataMapInfo.mmi, (NULL));
  rtwCAPI_SetChildMMIArrayLen(rtM->DataMapInfo.mmi, 0);
}

#else                                  // HOST_CAPI_BUILD
#ifdef __cplusplus

extern "C" {

#endif

  void walker_f446_host_InitializeDataMapInfo(walker_f446_host_DataMapInfo_T
    *dataMap, const char *path)
  {
    // Set C-API version
    rtwCAPI_SetVersion(dataMap->mmi, 1);

    // Cache static C-API data into the Real-time Model Data structure
    rtwCAPI_SetStaticMap(dataMap->mmi, &mmiStatic);

    // host data address map is NULL
    rtwCAPI_SetDataAddressMap(dataMap->mmi, (NULL));

    // host vardims address map is NULL
    rtwCAPI_SetVarDimsAddressMap(dataMap->mmi, (NULL));

    // Set Instance specific path
    rtwCAPI_SetPath(dataMap->mmi, path);
    rtwCAPI_SetFullPath(dataMap->mmi, (NULL));

    // Set reference to submodels
    rtwCAPI_SetChildMMIArray(dataMap->mmi, (NULL));
    rtwCAPI_SetChildMMIArrayLen(dataMap->mmi, 0);
  }

#ifdef __cplusplus

}
#endif
#endif                                 // HOST_CAPI_BUILD

//
// File trailer for generated code.
//
// [EOF]
//
