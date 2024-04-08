/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "AnalogIn.h"
#include "BufferedSerial.h"
#include "DigitalIn.h"
#include "DigitalOut.h"
#include "PinNames.h"
#include "ThisThread.h"
#include "arm_math.h"
#include "can_helper.h"
#include "cmsis_armclang.h"
#include "mbed.h"
#include "mbed_wait_api.h"
#include <cstdint>
#include <cstring>
#include "CANRaw.h"
#include "rtwtypes.h"
#include "walker_f446.h"
#include "ZLAC8030v2.h"
#include "cmdproc.h"

// Blinking rate in milliseconds
#define BLINKING_RATE     10ms

// PIN Definitions
#define BLUE_LED PC_2
#define GREEN_LED PC_1
#define SER3_TX PC_10
#define SER3_RX PC_11
#define SSI1_TX PA_9
#define SSI1_RX PA_10
#define SSI2_TX PA_2
#define SSI2_RX PA_3
#define ADCINP1 PB_1
#define ADCINP2 PB_0
#define CAN1_TX PA_12
#define CAN1_RX PA_11
#define CAN2_TX PB_13
#define CAN2_RX PB_12
#define MOTOR1ID 0

// MOTOR IDs
const uint8_t  LEFT_WHEEL_ID = 1;
const uint8_t RIGHT_WHEEL_ID = 2;

float32_t left_wheel_velocity = 0;
float32_t right_wheel_velocity = 0;
float32_t left_wheel_torque = 0;
float32_t right_wheel_torque = 0;


#define SERIAL_BAUD 115200
#define CAN_BAUD 1000000

const float ACC_GAIN = pow(2, -12);
const float GYR_GAIN = pow(2, -10);
#define ACC_TO_FLOAT(_acc) ((float_t)_acc)*ACC_GAIN
#define GYR_TO_FLOAT(_gyr) ((float_t)_gyr)*GYR_GAIN

// Raw data array
int16_t accdata[5][3];
int16_t gyrdata[5][3];

// Setup CAN Hardware
CANRaw can1(CAN1_RX, CAN1_TX, CAN_BAUD);
CANRaw can2(CAN2_RX, CAN2_TX, CAN_BAUD);

// #define WHEEL_CAN can2

char can_tx_buff[8] = {'h', 'e', 'l', 'l', 'o', '!', 0, 0};
CANMessage can_msg_in;

const char CLKDATA[6] = {0x55, 0x55, 0x55, 0x55, 0x55, 0xFD};
char SERBUF[16] = {0};

// Debug Serial port
static BufferedSerial pc(SER3_TX, SER3_RX);
// Command processor to update / read variables in real-time
static commandProcessor shell(pc);

/**********************************************************************
 * Global variables that can be set / read from the command line.
 **********************************************************************/

// The body weight support parameter in N.
volatile float32_t BODY_WEIGHT_SUPPORT = 50.0f;
// The MIN/MAX body weight support parameter in N.
const float32_t BWS_MIN = 0;
const float32_t BWS_MAX = 400;

// The forward assist parameter in N.
volatile float32_t FORWARD_ASSIST = 0.0f;
// The MIN/MAX forward assist parameter in N.
const float32_t FWD_ASSIST_MIN = 0;
const float32_t FWD_ASSIST_MAX = 100;

volatile uint8_t log_flag = 1;

/*************** END OF GLOBAL VARIABLES ******************************/


// Initialise the digital pin LED1 as an output
DigitalOut led1(BLUE_LED);
DigitalOut led2(GREEN_LED);

// The SSI ports
// BufferedSerial SSI1(SSI1_TX, SSI1_RX);
// BufferedSerial SSI2(SSI2_TX, SSI2_RX);
DigitalOut SSI1CLK(SSI1_TX);
DigitalIn  SSI1DAT(SSI1_RX);
DigitalOut SSI2CLK(SSI2_TX);
DigitalIn  SSI2DAT(SSI2_RX);

// The ADC ports
AnalogIn AENC1(ADCINP1);
AnalogIn AENC2(ADCINP2);

// Timer to keep track of the current time.
Timer timer;

// Event queue for task scheduling
EventQueue queue(32 * EVENTS_EVENT_SIZE);
Thread t(osPriorityRealtime);
// Setup mutex for updating parameters.
Mutex param_update_mutex;

// Initialize Simulink Model
static Model simulink_model;
static Model::ExtU model_inputs;
static Model::ExtY model_outputs;

// Make printf work over the serial port.
FileHandle *mbed::mbed_override_console(int fd)
{
    return &pc;
}

float32_t ssi1_read() {
    const unsigned int databits = 32;
    const unsigned int period = 900;
    volatile uint32_t encoderdata = 0;

    encoderdata = 0;

    SSI1CLK.write(0);
    wait_ns(2000);
    

    for(int i=0; i<databits; i++){
        encoderdata <<= 1;
        SSI1CLK.write(1);
        wait_ns(400);
        encoderdata |= SSI1DAT.read();
        wait_ns(400);
        SSI1CLK.write(0);
        wait_ns(1000);
    }



    SSI1CLK.write(1);
    wait_us(30);

    uint32_t count_only = encoderdata >> 6;
    uint32_t part1 = count_only & 0x1FFF;
    uint32_t part2 = count_only >> 13;

    float32_t encoder_mm = ((part1*2.0f)/(8192.0)) + (part2 * 2.0f);

    return encoder_mm;
}

uint32_t ssi2_read() {
    const unsigned int databits = 32;
    const unsigned int period = 900;
    volatile uint64_t encoderdata = 0;


    encoderdata = 0;

    SSI2CLK.write(0);
    wait_ns(2000);
    

    for(int i=0; i<databits; i++){
        encoderdata <<= 1;
        SSI2CLK.write(1);
        wait_ns(400);
        encoderdata |= SSI2DAT.read();
        wait_ns(400);
        SSI2CLK.write(0);
        wait_ns(1000);
    }



    SSI2CLK.write(1);
    wait_us(30);

    return encoderdata;
}

void set_motor_current(unsigned int id, int16_t cmd) {
    uint8_t sendbuf[8] = {};
    int16_t cmd2send = 0;
    uint16_t cmd2sendu = 0;

    if (cmd > 2000) {
        cmd2send = 2000;
    } else if (cmd < -2000) {
        cmd2send = -2000;
    } else {
        cmd2send = cmd; 
    }

    memcpy(&cmd2sendu, &cmd2send, 2);

    sendbuf[0] = 0xA1;
    sendbuf[1] = 0x00;
    sendbuf[2] = 0x00;
    sendbuf[3] = 0x00;
    sendbuf[4] = cmd2sendu & 0xFF;
    sendbuf[5] = (cmd2sendu >> 8) & 0xFF;
    sendbuf[6] = 0x00;
    sendbuf[7] = 0x00;

    can1.write(CANMessage(0x140+id, sendbuf, 8));
}

void enable_motor(unsigned int id) {
    uint8_t sendbuf[8] = {};
    sendbuf[0] = 0x88;
    can1.write(CANMessage(0x140+id, sendbuf, 8));
}

void clear_motor_errors(unsigned int id) {
    uint8_t sendbuf[8] = {};
    sendbuf[0] = 0x9B;
    can1.write(CANMessage(0x140+id, sendbuf, 8));
}

void set_motor_speed(unsigned int id, int32_t motor_speed) {
    uint8_t sendbuf[8] = {};
    //uint8_t cmd2send[4];

    sendbuf[0] = 0xA2;
    sendbuf[1] = 0x00;
    sendbuf[2] = 0x00;
    sendbuf[3] = 0x00;
    sendbuf[4] = *(uint8_t *)(&motor_speed);
    sendbuf[5] = *((uint8_t *)(&motor_speed)+1);
    sendbuf[6] = *((uint8_t *)(&motor_speed)+2);
    sendbuf[7] = *((uint8_t *)(&motor_speed)+3);

    can1.write(CANMessage(0x140+id, sendbuf, 8));
}

void can_parser()
{
    static int idmap[32] = {};
    const unsigned int IMU1ID = 17;
    const unsigned int IMU2ID = 18;
    // const unsigned int IMU3ID = 10;
    idmap[IMU1ID] = 1; // IMU1 is id 4
    idmap[IMU2ID] = 2; // IMU2 is id 5
    // idmap[IMU3ID] = 3; // IMU3 is id 11

    const uint8_t ACCEL_REPLY_1 = 15;
    const uint8_t GYRO_REPLY_1  = 18;

    unsigned int imuid = idmap[(can_msg_in.id >> 5)] - 1;
    unsigned int imumsg = can_msg_in.id & 0x1F;

    if (imuid >= 0) {
        // printf("Got IMU ID %d\n", can_msg_in.id >> 5);
        switch(imumsg) {
            case ACCEL_REPLY_1:
                param_update_mutex.lock();
                memcpy(accdata[imuid], can_msg_in.data, 6);
                param_update_mutex.unlock();
                break;
            case GYRO_REPLY_1:
                param_update_mutex.lock();
                memcpy(gyrdata[imuid], can_msg_in.data, 6);
                param_update_mutex.unlock();
                break;
            default:
                break;
        }
    }
}

void can_interrupt_handler() {
    // led1 = !led1;
    can2.read(can_msg_in);
    queue.call(can_parser);
}

void imu_sample_thread(void) {
    // can2.write(CANMessage(0x2B, can_tx_buff, 0)); // Quaternion
    can2.write(CANMessage(0x37, can_tx_buff, 0)); // Raw Data
    // can2.write(CANMessage(0x31, can_tx_buff, 0)); // Gyro
}

void EncoderInitialiseTIM3(void) {
    // configure GPIO PA0 & PA1 aka A0 & A1 as inputs for Encoder
    // Enable clock for GPIOA
    __GPIOA_CLK_ENABLE(); //equivalent from hal_rcc.h
 
    //stm32f4xx.h 
    GPIOA->MODER   |= GPIO_MODER_MODER6_1 | GPIO_MODER_MODER7_1 ;           //PA6 & PA7 as Alternate Function   /*!< GPIO port mode register,               Address offset: 0x00      */
    GPIOA->OTYPER  |= GPIO_OTYPER_OT_6 | GPIO_OTYPER_OT_7 ;                 //PA6 & PA7 as Inputs               /*!< GPIO port output type register,        Address offset: 0x04      */
    GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR6 | GPIO_OSPEEDER_OSPEEDR7 ;     //Low speed                         /*!< GPIO port output speed register,       Address offset: 0x08      */
    GPIOA->PUPDR   |= GPIO_PUPDR_PUPDR6_1 | GPIO_PUPDR_PUPDR7_1 ;           //Pull Down                         /*!< GPIO port pull-up/pull-down register,  Address offset: 0x0C      */
    GPIOA->AFR[0]  |= 0x22000000 ;                                          //AF02 for PA6 & PA7                /*!< GPIO alternate function registers,     Address offset: 0x20-0x24 */
    GPIOA->AFR[1]  |= 0x00000000 ;                                          //nibbles here refer to gpio8..15   /*!< GPIO alternate function registers,     Address offset: 0x20-0x24 */
   
    // configure TIM3 as Encoder input
    // Enable clock for TIM3
    __TIM3_CLK_ENABLE();
 
    TIM3->CR1   = 0x0001;     // CEN(Counter ENable)='1'     < TIM control register 1
    TIM3->SMCR  = TIM_ENCODERMODE_TI12;     // SMS='011' (Encoder mode 3)  < TIM slave mode control register
    TIM3->CCMR1 = 0xF1F1;     // CC1S='01' CC2S='01'         < TIM capture/compare mode register 1
    TIM3->CCMR2 = 0x0000;     //                             < TIM capture/compare mode register 2
    TIM3->CCER  = 0x0011;     // CC1P CC2P                   < TIM capture/compare enable register
    TIM3->PSC   = 0x0000;     // Prescaler = (0+1)           < TIM prescaler
    TIM3->ARR   = 0xffffffff; // reload at 0xfffffff         < TIM auto-reload register
  
    TIM3->CNT = 0x0000;  //reset the counter before we use it  
}
 
void EncoderInitialiseTIM4(void) {
    //PB6 PB7 aka D10 MORPHO_PB7
    // Enable clock for GPIOA
    __GPIOB_CLK_ENABLE(); //equivalent from hal_rcc.h
 
    //stm32f4xx.h 
    GPIOB->MODER   |= GPIO_MODER_MODER6_1 | GPIO_MODER_MODER7_1 ;           //PB6 & PB7 as Alternate Function   /*!< GPIO port mode register,               Address offset: 0x00      */
    GPIOB->OTYPER  |= GPIO_OTYPER_OT_6 | GPIO_OTYPER_OT_7 ;                 //PB6 & PB7 as Inputs               /*!< GPIO port output type register,        Address offset: 0x04      */
    GPIOB->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR6 | GPIO_OSPEEDER_OSPEEDR7 ;     //Low speed                         /*!< GPIO port output speed register,       Address offset: 0x08      */
    GPIOB->PUPDR   |= GPIO_PUPDR_PUPDR6_1 | GPIO_PUPDR_PUPDR7_1 ;           //Pull Down                         /*!< GPIO port pull-up/pull-down register,  Address offset: 0x0C      */
    GPIOB->AFR[0]  |= 0x22000000 ;                                          //AF02 for PB6 & PB7                /*!< GPIO alternate function registers,     Address offset: 0x20-0x24 */
    GPIOB->AFR[1]  |= 0x00000000 ;                                          //nibbles here refer to gpio8..15   /*!< GPIO alternate function registers,     Address offset: 0x20-0x24 */
   
    // configure TIM4 as Encoder input
    // Enable clock for TIM4
    __TIM4_CLK_ENABLE();
 
    TIM4->CR1   = 0x0001;     // CEN(Counter ENable)='1'     < TIM control register 1
    TIM4->SMCR  = TIM_ENCODERMODE_TI12;     //               < TIM slave mode control register
    //TIM_ENCODERMODE_TI1 input 1 edges trigger count
    //TIM_ENCODERMODE_TI2 input 2 edges trigger count
    //TIM_ENCODERMODE_TI12 all edges trigger count
    TIM4->CCMR1 = 0xF1F1;     // CC1S='01' CC2S='01'         < TIM capture/compare mode register 1
    //0xF nibble sets up filter
    TIM4->CCMR2 = 0x0000;     //                             < TIM capture/compare mode register 2
    TIM4->CCER  = TIM_CCER_CC1E | TIM_CCER_CC2E;     //     < TIM capture/compare enable register
    TIM4->PSC   = 0x0000;     // Prescaler = (0+1)           < TIM prescaler
    TIM4->ARR   = 0xffff; // reload at 0xfffffff         < TIM auto-reload register
  
    TIM4->CNT = 0x0000;  //reset the counter before we use it  
}

// int16_t read_incremental_encoder2() {
//     uint32_t raw_cnt = TIM4->CNT;
//     int8_t sign_bit = raw_cnt >> 15;
//     int16_t count_bits = static_cast<int16_t>(0x7FFF & raw_cnt);
//     int16_t signed_count = sign_bit ? -count_bits : count_bits;
//     return signed_count;
// }

int16_t read_incremental_encoder2() {
    uint32_t raw_cnt = TIM4->CNT;
    return static_cast<int16_t>(raw_cnt);
}

int16_t read_incremental_encoder1() {
    uint32_t raw_cnt = TIM3->CNT;
    return static_cast<int16_t>(raw_cnt);
}

// uint16_t read_incremental_encoder1() {
//     return TIM3->CNT;
// }


// Main control thread.
void control_task(void) {
    static int32_t task_call_counter=0;
    static int first_run = 1;
    static float32_t t_start = 0.0f;

    float32_t absolute_encoder1_mm = 0;
    float32_t absolute_encoder2_mm = 0;

    // Send SYNC message over CANOpen bus.
    // SYNCMessage(can1);

    /********** SET MODEL INPUTS **********/

    // Read the encoder
    absolute_encoder1_mm = ssi1_read();
    // absolute_encoder_count2 = ssi2_read();

    if (first_run == 1) {
        t_start = (duration_cast<std::chrono::microseconds>(timer.elapsed_time()).count())/1000000.0F;
    }

    
    // Set the clock time.
    model_inputs.TimeSignal = (duration_cast<std::chrono::microseconds>(timer.elapsed_time()).count())/1000000.0F;
    
    // Encoders
    model_inputs.SpringEncoder1 = absolute_encoder1_mm;
    model_inputs.SpringEncoder2 = read_incremental_encoder1();
    model_inputs.SpringEncoder3 = read_incremental_encoder2();

    // PID Gains
    model_inputs.Kp = 3.05;
    model_inputs.Kd = 0.35;
    model_inputs.Ki = 0.0;
    model_inputs.Kff = 0.8;
    model_inputs.N = 800;

    // Assistance parameters
    model_inputs.BWS = 20; // Newton
    // model_inputs.FwdBckAssist = 20; // Newton

    // Wheel velocity feedback.
    model_inputs.LeftWheelVelocity = left_wheel_velocity;
    model_inputs.RightWheelVelocity = right_wheel_velocity;

    // IMUs
    // model_inputs.IMU1[0] = ACC_TO_FLOAT(accdata[0][1]);
    // model_inputs.IMU1[1] = ACC_TO_FLOAT(-accdata[0][0]);
    // model_inputs.IMU1[2] = ACC_TO_FLOAT(-accdata[0][2]);

    // model_inputs.IMU1[3] = GYR_TO_FLOAT(-gyrdata[0][1]);
    // model_inputs.IMU1[4] = GYR_TO_FLOAT(gyrdata[0][0]);
    // model_inputs.IMU1[5] = GYR_TO_FLOAT(gyrdata[0][2]);

    // model_inputs.IMU2[0] = ACC_TO_FLOAT(accdata[1][1]);
    // model_inputs.IMU2[1] = ACC_TO_FLOAT(-accdata[1][0]);
    // model_inputs.IMU2[2] = ACC_TO_FLOAT(-accdata[1][2]);

    // model_inputs.IMU2[3] = GYR_TO_FLOAT(-gyrdata[1][1]);
    // model_inputs.IMU2[4] = GYR_TO_FLOAT(gyrdata[1][0]);
    // model_inputs.IMU2[5] = GYR_TO_FLOAT(gyrdata[1][2]);
    // model_inputs.Mode = 3;
    
    if (first_run == 1) {
        model_inputs.EnableAssist = 1;
        first_run = 0;
        model_inputs.Mode = 5;
        model_inputs.FwdBckAssist = 0;
    }

    if (task_call_counter > 1000) {
        model_inputs.EnableAssist = 1;

        // Modes:
        // 1 - Sinsuoidal
        // 2 - Chirp
        // 3 - -5000 -> -15000 sinusoid.
        // 4 - +/- 5000 sinusoid.
        // 5 - Force Map.
        model_inputs.Mode = 5;
        model_inputs.FwdBckAssist = 20;
    }

    // Lock the mutex.
    param_update_mutex.lock();
    
    simulink_model.setExternalInputs(&model_inputs);

    /******** END SET MODEL INPUTS *********/

    // Step through the model
    simulink_model.step();

    // Read model outputs
    model_outputs = simulink_model.getExternalOutputs();

    // Unlock the mutex.
    param_update_mutex.unlock();

    // Set motor outputs
    // MoveTorqueCMD(LEFT_WHEEL_ID , model_outputs.LeftWheelTorque > 300 ? 100 : -100, can1);
    // MoveTorqueCMD(LEFT_WHEEL_ID , model_outputs.LeftWheelTorque, can1);
    // MoveTorqueCMD(RIGHT_WHEEL_ID, 0, can1);
    set_motor_current(MOTOR1ID, model_outputs.LinearMotorCommand);
    // set_motor_current(MOTOR1ID, 0);
    // set_motor_speed(0, 10000);

    // wait_ns(100);
    // Wheel outputs.
    MoveTorqueCMD(LEFT_WHEEL_ID, 
                  static_cast<int32_t>(model_outputs.LeftWheelTorque), 
                  can2);
    // wait_ns(100);
    MoveTorqueCMD(RIGHT_WHEEL_ID, 
                  static_cast<int32_t>(model_outputs.RightWheelTorque), 
                  can2);
    // wait_ns(100);
    SYNCMessage(can2);
    // MoveTorqueCMD(RIGHT_WHEEL_ID, 80*cos(2*3.141592*1*0.5*model_inputs.TimeSignal), can2);

    // Set LED States
    led1 = model_outputs.LEDStates[0];
    
    
    
    // Logging
    if ((task_call_counter%1 == 0) && log_flag == 1) {
        // Request IMU Sample
        // queue.call(imu_sample_thread);

        // printf("%d\n", static_cast<int>((absolute_encoder1_mm-1494.428)*1000));
        // Wheels

        // printf("%d,%d,%d,%d,%d,%d,%d,%d\n",
        //            static_cast<int>(model_outputs.LogData[0]*100),     // Z - Spring force
        //            static_cast<int>(model_outputs.LogData[1]*100),     // Fwd Spring force
        //            static_cast<int>(model_outputs.LogData[2]*100),     // Side Spring force
        //            static_cast<int>(model_outputs.LogData[3]*100),     // Z - Spring Command
        //            static_cast<int>(model_outputs.LogData[4]*10),      // Left wheel velocity
        //            static_cast<int>(model_outputs.LogData[5]*10),      // Right wheel velocity
        //            static_cast<int>(model_outputs.LogData[6]*10),      // Left wheel command
        //            static_cast<int>(model_outputs.LogData[7]*10));     // Right wheel command.

        printf(
            "%da",
            static_cast<int>(model_outputs.LogData[0]*1000)     // Z - Spring force
        );     // Right wheel command.
    }
    task_call_counter++;
}

void ZLAC8030_can_parser()
{
    float current;
    float speed;

    if (can_msg_in.id == 0x181) {
        unpack_speed_torque_reply(can_msg_in.data, current, speed);
        left_wheel_torque = current;
        left_wheel_velocity = speed;
        // printf("C: %d  V: %d\n", int(current*100), int(speed*100));
    } else if (can_msg_in.id == 0x182) {
        unpack_speed_torque_reply(can_msg_in.data, current, speed);
        right_wheel_torque = current;
        right_wheel_velocity = speed;
        // printf("C: %d  V: %d\n", int(current*100), int(speed*100));
    }  else {
        // printf("%x :: ", can_msg_in.id, can_msg_in.len);
        // for (int i=0; i<can_msg_in.len; i++) {
        //     printf("%02x ", can_msg_in.data[i]);
        // }
        // printf("\n");
    }
}

void ZLAC8030_can_interrupt_handler() {
    
    // Flash our LED
    led2 = !led2;

    // Read the CAN Message
    can2.read(can_msg_in);

    // Parse messages
    queue.call(ZLAC8030_can_parser);
}

/****************** SHELL FUNCTIONS ******************/

void shell_task() {
    shell.poll();
    // led1 = !led1;
}

void dlog(int argc, char **argv) {
    log_flag ^= 1;
}

// Function to set or get the Body Weight Support parameter.
void bws_set_get(int argc, char **argv) {
    int8_t v;
    float32_t cv;

    // If we have two arguments
    if (argc == 3) {
        // If the first argument is an equal sign
        if (argv[1][0] == '=') {
            // Convert the second argument to an integer.
            v = atoi(argv[2]);
            // Set the body weight support by casting the integer to a float.
            cv = static_cast<float>(v);
            // If the converted value is within range, then update.
            if ((cv > BWS_MIN) && (cv < BWS_MAX)) {
                BODY_WEIGHT_SUPPORT = cv;
            } else {
                printf("E: Cannot set bws to %d", static_cast<int>(cv));
            }
        }
    } else {
        // Print the body weight support after casting it to an integer.
        printf("%d\n", static_cast<int>(BODY_WEIGHT_SUPPORT));
    }
}

// Function to set or get the Forward Assist parameter.
void fwd_assist_set_get(int argc, char **argv) {
    int8_t v;
    float32_t cv;

    // If we have two arguments
    if (argc == 3) {
        // If the first argument is an equal sign
        if (argv[1][0] == '=') {
            // Convert the second argument to an integer.
            v = atoi(argv[2]);
            // Set the forward assist by casting the integer to a float.
            cv = static_cast<float>(v);
            // If the converted value is outside the range, don't do anything
            if ((cv < FWD_ASSIST_MAX) && (cv > FWD_ASSIST_MIN)) {
                FORWARD_ASSIST = cv;
            } else {
                printf("E: Cannot set bws to %d", static_cast<int>(cv));
            }
        }
    } else {
        // Print the forward assist after casting it to an integer.
        printf("%d\n", static_cast<int>(FORWARD_ASSIST));
    }
}

/**************** END SHELL FUNCTIONS ******************/
/* serial port send task  */
void print_encoder_task() {
    float32_t absolute_encoder1_mm = 0;
    int16_t enc1;
    int16_t enc2;
    // Read the encoder
    absolute_encoder1_mm = ssi1_read();
    enc1 = read_incremental_encoder1();
    enc2 = read_incremental_encoder2();

    // Read the encoder
    uint8_t sendbuf[8] = {};

    memcpy(sendbuf, &enc1, 2);
    memcpy(sendbuf+2, &enc2, 2);

    // sendbuf[0] = 0x00;
    // sendbuf[1] = 0x00;
    // sendbuf[2] = 0x00;
    // sendbuf[3] = 0x00;
    sendbuf[4] = (static_cast<int>(absolute_encoder1_mm * 1000) >> 24)& 0xFF;
    sendbuf[5] = (static_cast<int>(absolute_encoder1_mm * 1000) >> 16)& 0xFF;
    sendbuf[6] = (static_cast<int>(absolute_encoder1_mm * 1000) >> 8)& 0xFF;
    sendbuf[7] =  static_cast<int>(absolute_encoder1_mm * 1000)& 0xFF;

    can1.write(CANMessage(0x140, sendbuf, 8));





    // printf(
    //     "%da",
    //     static_cast<int>(absolute_encoder1_mm*1000)     // Z - Spring force
    // );     
}


int main()
{
    uint32_t encbits = 0;
    // Set serial port baud rate.
    pc.set_baud(SERIAL_BAUD);

    // Start the timer
    timer.start();

    // Start the event queue.
    t.start(callback(&queue, &EventQueue::dispatch_forever));

    // Initialize encoders
    EncoderInitialiseTIM3();
    EncoderInitialiseTIM4();
    SSI1CLK = 1;
    SSI2CLK = 1;

    // Enable the motors
    // EnterVelocityMode(LEFT_WHEEL_ID, can2);
    // EnterVelocityMode(RIGHT_WHEEL_ID, can2);

    // EnterVelocityMode(LEFT_WHEEL_ID , can2);
    EnterTorqueMode(RIGHT_WHEEL_ID, can2);
    EnterTorqueMode(LEFT_WHEEL_ID, can2);
    // printf("Entered torque mode.\n");


    // Initialize the simulink model.
    simulink_model.initialize();
    printf("Simulink model initialized.\n");
    
    can2.filter(0x180, 0xFF0, CANStandard, 0);

    // Attach interrupt handler for wheels
    can2.attach(&ZLAC8030_can_interrupt_handler, CAN::RxIrq);
    
    // can2.attach(&can_interrupt_handler, CAN::RxIrq);

    // Set up the command processor.
    //Initialize the command processor.
    shell.begin(PROMPT_OFF, ECHO_OFF);
    shell.add("log", dlog);
    shell.add("bws", bws_set_get);
    shell.add("fa", fwd_assist_set_get);


    ThisThread::sleep_for(5000ms);

    // Set the control task to run at 500Hz
    queue.call_every(1ms, print_encoder_task);


    // MoveTorqueCMD(LEFT_WHEEL_ID , 0, can1);

    while (true) {
        led1 = !led1;
        ThisThread::sleep_for(1000ms);
        // SYNCMessage(can2);
        // MoveVelocityCMD(LEFT_WHEEL_ID , led1? 10000 : -10000, can2);
    }
}
