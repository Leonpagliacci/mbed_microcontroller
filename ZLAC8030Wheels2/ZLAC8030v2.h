#ifndef ZLAC8030V2_H_
#define ZLAC8030V2_H_

#include "CANRaw.h"
#include "arm_math.h"
#include <cstdint>

// Struct to hold PID control constants.
struct PIDconstant{
    uint8_t anglePidKp = 0;
    uint8_t anglePidKi = 0;
    uint8_t speedPidKp = 0;
    uint8_t speedPidKi = 0;
    uint8_t iqPidKp    = 0;
    uint8_t iqPidKi    = 0;
};

// Struct to hold data related to a wheel.
class ZLAC8030 {
public:
    ZLAC8030(uint8_t nodeId);
    uint8_t   node_id;
    uint16_t  position;
    int64_t   multi_turn_position;
    float32_t voltage , torque  , speed, currency;
    int16_t   phase_ai, phase_bi, phase_ci;
    uint8_t   temp    , error_state;
    uint16_t  raw_position,encoderoffset;
    int32_t   accel_data;
    PIDconstant pid;
private:
};

// Global static variables
static CANMessage tx_buf;

// Function declarations

uint32_t GetCOBID(uint8_t ID, uint8_t type);

// Configure PDOs
uint8_t RPDO0Config(uint8_t ID, CANRaw &can);
uint8_t RPDO1Config(uint8_t ID, CANRaw &can);
uint8_t RPDO2Config(uint8_t ID, CANRaw &can);
uint8_t RPDO3Config(uint8_t ID, CANRaw &can);

uint8_t TPDO0Config(uint8_t ID, CANRaw &can);
uint8_t TPDO1Config(uint8_t ID, CANRaw &can);
uint8_t TPDO2Config(uint8_t ID, CANRaw &can);
uint8_t TPDO3Config(uint8_t ID, CANRaw &can);

// NMT
uint8_t NMTControl(uint8_t ID, uint8_t Data0, uint8_t Data1, CANRaw &can);
uint8_t SYNCMessage(CANRaw &can);

// Higher level functions
uint8_t DriverEnable(uint8_t ID, CANRaw &can);
uint8_t ProfileVelocityInit(uint8_t ID, CANRaw &can);
uint8_t ProfileTorqueInit(uint8_t ID, CANRaw &can);
uint8_t EnterVelocityMode(uint8_t ID, CANRaw &can);
uint8_t EnterTorqueMode(uint8_t ID, CANRaw &can);

void MoveVelocityCMD(uint8_t ID, int32_t Vel, CANRaw &can);
void MovePositionCMD(uint8_t ID, int32_t Vel, CANRaw &can);
void MoveTorqueCMD(uint8_t ID, int32_t currency, CANRaw &can);
void unpack_speed_torque_reply(uint8_t *rx_data, float32_t &pcurrency, float32_t &pspeed);

#endif