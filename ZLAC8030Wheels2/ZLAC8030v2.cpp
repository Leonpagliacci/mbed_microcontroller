#include "ZLAC8030v2.h"
#include "CAN.h"
#include "mbed_wait_api.h"
#include "tcp.h"
#include <cstdint>


// SDO Command byte definitions
#define SDO_DOWNLOAD_SEG   0b00000000
#define SDO_INIT_DOWNLOAD  0b00100000
#define SDO_INIT_UPLOAD    0b01000000
#define SDO_UPLOAD_SEG     0b01100000
#define SDO_ABORT          0b10000000
#define SDO_BLOCK_UPLOAD   0b10100000
#define SDO_BLOCK_DOWNLOAD 0b11000000
#define SDO_TOGGLE_BIT     0b00010000
#define SDO_SET_N3         0b00001100
#define SDO_SET_N2         0b00001000
#define SDO_SET_N1         0b00000100
#define SDO_SET_N0         0b00000000
#define SDO_SET_E1         0b00000010
#define SDO_SET_S1         0b00000001


// ZLAC8030 Specific definitions
#define INDEX_SERVER_SDO_PARAMS 0x1200
#define SUBINDEX_SDO_RX_COBID 1
#define SUBINDEX_SDO_TX_COBID 2

#define INDEX_RPDO_COM_PARAMS 0x1400
#define SUBINDEX_RPDO_COBID 1
#define SUBINDEX_RPDO_TYPE 2
#define RPDO_TYPE_SYNC 239
#define RPDO_TYPE_IMMEDIATE 254
#define INDEX_RPDO_MAP_PARAMS 0x1600
#define SUBINDEX_RPDO_N_MAPS 0
#define SUBINDEX_RPDO_


// Send a Service Data Object
inline int SDO_SEND(uint8_t ID, CANRaw &can, 
                 uint8_t sdo_cmd, 
                 uint16_t od_main,
                 uint8_t od_sub, 
                 uint8_t data1 = 0,
                 uint8_t data2 = 0,
                 uint8_t data3 = 0,
                 uint8_t data4 = 0) 
{
    tx_buf.id = 0x600 + ID;
    tx_buf.len = 8;
    tx_buf.data[0] = sdo_cmd;
    tx_buf.data[1] = od_main & 0xFF;        // The low byte of OD MAIN
    tx_buf.data[2] = (od_main >> 8) & 0xFF; // The high byte of OD MAIN
    tx_buf.data[3] = od_sub;
    tx_buf.data[4] = data1;
    tx_buf.data[5] = data2;
    tx_buf.data[6] = data3;
    tx_buf.data[7] = data4;

    return can.write(tx_buf);
}

// Configure RPDO0 as the control word (allows enabling motors and other things)
uint8_t RPDO_CONFIG_0(uint8_t node_id, CANRaw &can) {
    // Clear the PDO mapping
    SDO_SEND(node_id, can, 
             SDO_INIT_DOWNLOAD | SDO_SET_N3 | SDO_SET_E1 | SDO_SET_S1, 
             0x1600,
             0x00,
             0x00, 0x00, 0x00, 0x00);

    // Set the PDO to update immediately
    SDO_SEND(node_id, can, 
             SDO_INIT_DOWNLOAD | SDO_SET_N3 | SDO_SET_E1 | SDO_SET_S1, 
             0x1400,
             0x02,
             0xFE, 0x00, 0x00, 0x00);

    // MAP RPDO to control word
    SDO_SEND(node_id, can,
             SDO_INIT_DOWNLOAD | SDO_SET_E1 | SDO_SET_S1,
             0x1600, // Index
             0x01,   // Subindex
             0x10,   // 16 bit data
             0x00,   // Subindex of object to be mapped
             0x40,   
             0x60);  // 16 bit index of control word.

    // Enable RPDO
    SDO_SEND(node_id, can, 
             SDO_INIT_DOWNLOAD | SDO_SET_N3 | SDO_SET_E1 | SDO_SET_S1, 
             0x1600,
             0x00,
             0x01, // Only one object is mapped to the RPDO.
             0x00, 0x00, 0x00);


    return 0;
}


// Configure RPDO1 as the torque command
uint8_t RPDO_CONFIG_1(uint8_t node_id, CANRaw &can) {
    // Clear the PDO mapping
    SDO_SEND(node_id, can, 
             SDO_INIT_DOWNLOAD | SDO_SET_N3 | SDO_SET_E1 | SDO_SET_S1, 
             0x1601,
             0x00,
             0x00, 0x00, 0x00, 0x00);

    // Set the PDO to update immediately
    SDO_SEND(node_id, can, 
             SDO_INIT_DOWNLOAD | SDO_SET_N3 | SDO_SET_E1 | SDO_SET_S1, 
             0x1401,
             0x02,
             0xFE, 0x00, 0x00, 0x00);

    // MAP RPDO1 to the torque command
    SDO_SEND(node_id, can,
             SDO_INIT_DOWNLOAD | SDO_SET_E1 | SDO_SET_S1,
             0x1601, // Index
             0x01,   // Subindex
             0x20,   // 32 bit data
             0x00,   // Subindex of object to be mapped
             0x40,   
             0x23);  // 16 bit index of torque command.

    // Enable the PDO
    SDO_SEND(node_id, can, 
             SDO_INIT_DOWNLOAD | SDO_SET_N3 | SDO_SET_E1 | SDO_SET_S1, 
             0x1601,
             0x00,
             0x01, // Only one object is mapped to the RPDO 
             0x00, 0x00, 0x00);

    return 0;

}


uint8_t TPDO_CONFIG_0(uint8_t node_id, CANRaw &can) {
    // Disable TPDO
    SDO_SEND(node_id, can,
             SDO_INIT_DOWNLOAD | SDO_SET_N3 | SDO_SET_E1 | SDO_SET_S1,
             0x1A00,
             0x00,
             0x00, 0x00, 0x00, 0x00);
    
    // PDO is transmitted every 1 SYNC message
    SDO_SEND(node_id, can,
             SDO_INIT_DOWNLOAD | SDO_SET_N3 | SDO_SET_E1 | SDO_SET_S1,
             0x1800,
             0x02,
             0x01, 0x00, 0x00, 0x00);

    // Map actual motor current to PDO
    SDO_SEND(node_id, can,
             SDO_INIT_DOWNLOAD | SDO_SET_E1 | SDO_SET_S1,
             0x1A00, // Index
             0x00,   // Subindex
             0x10,   // 16 bit data
             0x00,   // Subindex of object to be mapped
             0x1C,   
             0x22);  // 16 bit index of actual motor current.

    // Map actual motor velocity to PDO
    SDO_SEND(node_id, can,
             SDO_INIT_DOWNLOAD | SDO_SET_E1 | SDO_SET_S1,
             0x1A00, // Index
             0x02,   // Subindex
             0x20,   // 32 bit data
             0x00,   // Subindex of object to be mapped
             0x6C,   
             0x60);  // 16 bit index of actual motor velocity.

    // Map status word to PDO
    SDO_SEND(node_id, can,
             SDO_INIT_DOWNLOAD | SDO_SET_E1 | SDO_SET_S1,
             0x1A00, // Index
             0x03,   // Subindex
             0x10,   // 32 bit data
             0x00,   // Subindex of object to be mapped
             0x41,   
             0x60);  // 16 bit index of motor status word.

    // Enable TPDO
    SDO_SEND(node_id, can,
             SDO_INIT_DOWNLOAD | SDO_SET_N3 | SDO_SET_E1 | SDO_SET_S1,
             0x1A00,
             0x00,
             0x03, 0x00, 0x00, 0x00);

    return 0;
}



uint8_t RPDO0Config(uint8_t ID, CANRaw &can)
{
    // Clear mapping
    tx_buf.id = 0x600 + ID;
    tx_buf.len = 8;
    tx_buf.data[0] = 0x2F;
    tx_buf.data[1] = 0x00;
    tx_buf.data[2] = 0x16;
    tx_buf.data[3] = 0x00;
    tx_buf.data[4] = 0x00;
    tx_buf.data[5] = 0x00;
    tx_buf.data[6] = 0x00;
    tx_buf.data[7] = 0x00;
    can.write(tx_buf);// 清空RPDO0映射
    // uint8_t Data[8] = {0x2F, 0x00, 0x14, 0x02, 0xFE, 0x00, 0x00, 0x00}; //RPDO0事件触发

    wait_us(500);
    
    // Set trigger mechanism to SYNC
    tx_buf.data[0] = 0x2F;
    tx_buf.data[1] = 0x00;
    tx_buf.data[2] = 0x14;
    tx_buf.data[3] = 0x02;
    tx_buf.data[4] = 0xFE;
    tx_buf.data[5] = 0x00;
    tx_buf.data[6] = 0x00;
    tx_buf.data[7] = 0x00;
    can.write(tx_buf);

    wait_us(500); 

    // HAL_Delay(5);

    // Mapping RPDO0 to Status word.
    tx_buf.data[0] = 0x23;
    tx_buf.data[1] = 0x00;
    tx_buf.data[2] = 0x16;
    tx_buf.data[3] = 0x01;
    tx_buf.data[4] = 0x10;
    tx_buf.data[5] = 0x00;
    tx_buf.data[6] = 0x40;
    tx_buf.data[7] = 0x60;
    can.write(tx_buf); //RPDO0映射0x6040控制字
    // HAL_Delay(5);

    wait_us(500);

    // Enable PDO0
    tx_buf.data[0] = 0x2F;
    tx_buf.data[1] = 0x00;
    tx_buf.data[2] = 0x16;
    tx_buf.data[3] = 0x00;
    tx_buf.data[4] = 0x01;
    tx_buf.data[5] = 0x00;
    tx_buf.data[6] = 0x00;
    tx_buf.data[7] = 0x00;
    can.write(tx_buf); //RPDO0开启1个映射
    // HAL_Delay(5);

    return 0x00;
}

//RPDO1事件触发
//RPDO1映射0x2340（目标电流）
//RPDO1-COB-ID:0x300 + ID
uint8_t RPDO1Config(uint8_t ID, CANRaw &can)
{
    tx_buf.id = 0x600 + ID;
    tx_buf.len = 8;
    tx_buf.data[0] = 0x2F;
    tx_buf.data[1] = 0x01;
    tx_buf.data[2] = 0x16;
    tx_buf.data[3] = 0x00;
    tx_buf.data[4] = 0x00;
    tx_buf.data[5] = 0x00;
    tx_buf.data[6] = 0x00;
    tx_buf.data[7] = 0x00;
    can.write(tx_buf);// 清空RPDO1映射

    wait_us(500);

    tx_buf.data[0] = 0x2F;
    tx_buf.data[1] = 0x01;
    tx_buf.data[2] = 0x14;
    tx_buf.data[3] = 0x02;
    tx_buf.data[4] = 0xFE;
    tx_buf.data[5] = 0x00;
    tx_buf.data[6] = 0x00;
    tx_buf.data[7] = 0x00;
    tx_buf.id = 0x600 + ID;
    tx_buf.len = 8;
    can.write(tx_buf); //RPDO1事件触发
    // HAL_Delay(5);

    wait_us(500);

    tx_buf.data[0] = 0x23;
    tx_buf.data[1] = 0x01;
    tx_buf.data[2] = 0x16;
    tx_buf.data[3] = 0x01;
    tx_buf.data[4] = 0x20;
    tx_buf.data[5] = 0x00;
    tx_buf.data[6] = 0x40;
    tx_buf.data[7] = 0x23;
    can.write(tx_buf); //RPDO1映射0x2340 目标电流
    // HAL_Delay(5);0x2340 电流指令

    wait_us(500);

    tx_buf.data[0] = 0x2F;
    tx_buf.data[1] = 0x01;
    tx_buf.data[2] = 0x16;
    tx_buf.data[3] = 0x00;
    tx_buf.data[4] = 0x01;
    tx_buf.data[5] = 0x00;
    tx_buf.data[6] = 0x00;
    tx_buf.data[7] = 0x00;
    can.write(tx_buf); //RPDO1开启1个映射
    // HAL_Delay(5);

    return 0x00;
}

//RPDO2事件触发
//RPDO2映射0x60FF（电机目标速度）
//RPDO2-COB-ID:0x400 + ID
uint8_t RPDO2Config(uint8_t ID, CANRaw &can)
{
    tx_buf.id = 0x600 + ID;
    tx_buf.len = 8;
    tx_buf.data[0] = 0x2F;
    tx_buf.data[1] = 0x02;
    tx_buf.data[2] = 0x16;
    tx_buf.data[3] = 0x00;
    tx_buf.data[4] = 0x00;
    tx_buf.data[5] = 0x00;
    tx_buf.data[6] = 0x00;
    tx_buf.data[7] = 0x00;
    can.write(tx_buf);// 清空RPDO2映射

    tx_buf.data[0] = 0x2F;
    tx_buf.data[1] = 0x02;
    tx_buf.data[2] = 0x14;
    tx_buf.data[3] = 0x02;
    tx_buf.data[4] = 0xFE;
    tx_buf.data[5] = 0x00;
    tx_buf.data[6] = 0x00;
    tx_buf.data[7] = 0x00;
    tx_buf.id = 0x600 + ID;
    tx_buf.len = 8;
    can.write(tx_buf); //RPDO2事件触发
    // HAL_Delay(5);

    tx_buf.data[0] = 0x23;
    tx_buf.data[1] = 0x02;
    tx_buf.data[2] = 0x16;
    tx_buf.data[3] = 0x01;
    tx_buf.data[4] = 0x20;
    tx_buf.data[5] = 0x00;
    tx_buf.data[6] = 0xFF;
    tx_buf.data[7] = 0x60;
    tx_buf.id = 0x600 + ID;
    tx_buf.len = 8;
    can.write(tx_buf); //RPDO2映射0x60FF 目标速度
    // HAL_Delay(5);

    tx_buf.data[0] = 0x2F;
    tx_buf.data[1] = 0x02;
    tx_buf.data[2] = 0x16;
    tx_buf.data[3] = 0x00;
    tx_buf.data[4] = 0x01;
    tx_buf.data[5] = 0x00;
    tx_buf.data[6] = 0x00;
    tx_buf.data[7] = 0x00;
    tx_buf.id = 0x600 + ID;
    tx_buf.len = 8;
    can.write(tx_buf); //RPDO2开启1个映射
    // HAL_Delay(5);

    return 0x00;
}

//RPDO3事件触发
//RPDO3映射0x607A（目标位置）
//RPDO3-COB-ID:0x500 + ID
uint8_t RPDO3Config(uint8_t ID, CANRaw &can)
{

    tx_buf.id = 0x600 + ID;
    tx_buf.len = 8;
    tx_buf.data[0] = 0x2F;
    tx_buf.data[1] = 0x03;
    tx_buf.data[2] = 0x16;
    tx_buf.data[3] = 0x00;
    tx_buf.data[4] = 0x00;
    tx_buf.data[5] = 0x00;
    tx_buf.data[6] = 0x00;
    tx_buf.data[7] = 0x00;
    can.write(tx_buf);// 清空RPDO1映射

    tx_buf.data[0] = 0x2F;
    tx_buf.data[1] = 0x03;
    tx_buf.data[2] = 0x14;
    tx_buf.data[3] = 0x02;
    tx_buf.data[4] = 0xFE;
    tx_buf.data[5] = 0x00;
    tx_buf.data[6] = 0x00;
    tx_buf.data[7] = 0x00;
    tx_buf.id = 0x600 + ID;
    tx_buf.len = 8;
    can.write(tx_buf); //RPDO3事件触发
    // HAL_Delay(5);

    tx_buf.data[0] = 0x23;
    tx_buf.data[1] = 0x03;
    tx_buf.data[2] = 0x16;
    tx_buf.data[3] = 0x01;
    tx_buf.data[4] = 0x01;
    tx_buf.data[5] = 0x00;
    tx_buf.data[6] = 0x7A;
    tx_buf.data[7] = 0x60;
    tx_buf.id = 0x600 + ID;
    tx_buf.len = 8;
    can.write(tx_buf); //RPDO3映射0x607A目标位置
    // HAL_Delay(5);

    tx_buf.data[0] = 0x2F;
    tx_buf.data[1] = 0x03;
    tx_buf.data[2] = 0x16;
    tx_buf.data[3] = 0x00;
    tx_buf.data[4] = 0x01;
    tx_buf.data[5] = 0x00;
    tx_buf.data[6] = 0x00;
    tx_buf.data[7] = 0x00;
    tx_buf.id = 0x600 + ID;
    tx_buf.len = 8;
    can.write(tx_buf); //RPDO3开启1个映射
    // HAL_Delay(5);

    return 0x00;
}

//TPDO定时器触发
//TPDO0映射0x221C + 0x606C(实际电流 + 实际速度)
//TPDO0-COB-ID:0x180 + ID
uint8_t TPDO0Config(uint8_t ID, CANRaw &can)
{
    tx_buf.id = 0x600 + ID;
    tx_buf.len = 8;
    tx_buf.data[0] = 0x2F;
    tx_buf.data[1] = 0x00;
    tx_buf.data[2] = 0x1A;
    tx_buf.data[3] = 0x00;
    tx_buf.data[4] = 0x00;
    tx_buf.data[5] = 0x00;
    tx_buf.data[6] = 0x00;
    tx_buf.data[7] = 0x00;
    can.write(tx_buf);// 清空TPDO0映射

    wait_us(500);

    tx_buf.data[0] = 0x23;
    tx_buf.data[1] = 0x00;
    tx_buf.data[2] = 0x1A;
    tx_buf.data[3] = 0x01;
    tx_buf.data[4] = 0x10;  // 0x221c 只有16 bits
    tx_buf.data[5] = 0x00;
    tx_buf.data[6] = 0x1C;
    tx_buf.data[7] = 0x22;
    tx_buf.id = 0x600 + ID;
    tx_buf.len = 8;
    can.write(tx_buf); //map the current feedback 0x221C

    wait_us(500);

    tx_buf.data[0] = 0x23;
    tx_buf.data[1] = 0x00;
    tx_buf.data[2] = 0x1A;
    tx_buf.data[3] = 0x02;
    tx_buf.data[4] = 0x20;   //32 bits 长度映射
    tx_buf.data[5] = 0x00;
    tx_buf.data[6] = 0x6C;
    tx_buf.data[7] = 0x60;
    tx_buf.id = 0x600 + ID;
    tx_buf.len = 8;
    can.write(tx_buf); //map the speed feedback 0x606c

    wait_us(500);

    tx_buf.data[0] = 0x23;
    tx_buf.data[1] = 0x00;
    tx_buf.data[2] = 0x1A;
    tx_buf.data[3] = 0x03;
    tx_buf.data[4] = 0x10;
    tx_buf.data[5] = 0x00;
    tx_buf.data[6] = 0x41;
    tx_buf.data[7] = 0x60;
    tx_buf.id = 0x600 + ID;
    tx_buf.len = 8;
    can.write(tx_buf); //map the driver status 0x6041

    wait_us(500);

    tx_buf.data[0] = 0x2F;
    tx_buf.data[1] = 0x00;
    tx_buf.data[2] = 0x18;
    tx_buf.data[3] = 0x02;
    tx_buf.data[4] = 0x01;
    tx_buf.data[5] = 0x00;
    tx_buf.data[6] = 0x00;
    tx_buf.data[7] = 0x00;
    tx_buf.id = 0x600 + ID;
    tx_buf.len = 8;
    can.write(tx_buf); //TPDO0触发条件设置，1个sync信号触发

    wait_us(500);


    tx_buf.data[0] = 0x2F;
    tx_buf.data[1] = 0x00;
    tx_buf.data[2] = 0x1A;
    tx_buf.data[3] = 0x00;
    tx_buf.data[4] = 0x03;
    tx_buf.data[5] = 0x00;
    tx_buf.data[6] = 0x00;
    tx_buf.data[7] = 0x00;
    tx_buf.id = 0x600 + ID;
    tx_buf.len = 8;
    can.write(tx_buf); //TPDO0开启3个映射
    return 0x00;
}

//TPDO1定时器触发
//TPDO1定时器100ms
//TPDO1映射0x606C(反馈速度)
//TPDO1-COB-ID:0x280 + ID
uint8_t TPDO1Config(uint8_t ID, CANRaw &can)
{
    tx_buf.id = 0x600 + ID;
    tx_buf.len = 8;
    tx_buf.data[0] = 0x2F;
    tx_buf.data[1] = 0x01;
    tx_buf.data[2] = 0x1A;
    tx_buf.data[3] = 0x00;
    tx_buf.data[4] = 0x00;
    tx_buf.data[5] = 0x00;
    tx_buf.data[6] = 0x00;
    tx_buf.data[7] = 0x00;
    can.write(tx_buf);// 清空TPDO1映射

    tx_buf.data[0] = 0x2F;
    tx_buf.data[1] = 0x01;
    tx_buf.data[2] = 0x18;
    tx_buf.data[3] = 0x02;
    tx_buf.data[4] = 0xFF;
    tx_buf.data[5] = 0x00;
    tx_buf.data[6] = 0x00;
    tx_buf.data[7] = 0x00;
    tx_buf.id = 0x600 + ID;
    tx_buf.len = 8;
    can.write(tx_buf); //TPDO1定时器触发
    // HAL_Delay(5);

    tx_buf.data[0] = 0x2B;
    tx_buf.data[1] = 0x01;
    tx_buf.data[2] = 0x18;
    tx_buf.data[3] = 0x05;
    tx_buf.data[4] = 0xC8;
    tx_buf.data[5] = 0x00;
    tx_buf.data[6] = 0x00;
    tx_buf.data[7] = 0x00;
    tx_buf.id = 0x600 + ID;
    tx_buf.len = 8;
    can.write(tx_buf); //TPDOA定时器200*0.5ms
    // HAL_Delay(5);

    tx_buf.data[0] = 0x23;
    tx_buf.data[1] = 0x01;
    tx_buf.data[2] = 0x1A;
    tx_buf.data[3] = 0x01;
    tx_buf.data[4] = 0x10;
    tx_buf.data[5] = 0x00;
    tx_buf.data[6] = 0x77;
    tx_buf.data[7] = 0x60;
    tx_buf.id = 0x600 + ID;
    tx_buf.len = 8;
    can.write(tx_buf); //TPDO1映射0x606C(反馈速度)
    // HAL_Delay(5);

    tx_buf.data[0] = 0x2F;
    tx_buf.data[1] = 0x01;
    tx_buf.data[2] = 0x1A;
    tx_buf.data[3] = 0x00;
    tx_buf.data[4] = 0x01;
    tx_buf.data[5] = 0x00;
    tx_buf.data[6] = 0x00;
    tx_buf.data[7] = 0x00;
    tx_buf.id = 0x600 + ID;
    tx_buf.len = 8;
    can.write(tx_buf); //TPDO1开启1个映射
    // HAL_Delay(5);

    return 0x00;
}

//TPDO2定时器触发
//TPDO2定时器时间100ms
//TPDO2映射0x6064 00（电机反馈位置）
//TPDO2-COB-ID:0x380 + ID
uint8_t TPDO2Config(uint8_t ID, CANRaw &can)
{
    tx_buf.id = 0x600 + ID;
    tx_buf.len = 8;
    tx_buf.data[0] = 0x2F;
    tx_buf.data[1] = 0x02;
    tx_buf.data[2] = 0x1A;
    tx_buf.data[3] = 0x00;
    tx_buf.data[4] = 0x00;
    tx_buf.data[5] = 0x00;
    tx_buf.data[6] = 0x00;
    tx_buf.data[7] = 0x00;
    can.write(tx_buf);// 清空TPDO2映射

    tx_buf.data[0] = 0x2F;
    tx_buf.data[1] = 0x02;
    tx_buf.data[2] = 0x18;
    tx_buf.data[3] = 0x02;
    tx_buf.data[4] = 0xFF;
    tx_buf.data[5] = 0x00;
    tx_buf.data[6] = 0x00;
    tx_buf.data[7] = 0x00;
    tx_buf.id = 0x600 + ID;
    tx_buf.len = 8;
    can.write(tx_buf); //TPDO2事件触发
    // HAL_Delay(5);

    tx_buf.data[0] = 0x2B;
    tx_buf.data[1] = 0x02;
    tx_buf.data[2] = 0x18;
    tx_buf.data[3] = 0x05;
    tx_buf.data[4] = 0xC8;
    tx_buf.data[5] = 0x00;
    tx_buf.data[6] = 0x00;
    tx_buf.data[7] = 0x00;
    tx_buf.id = 0x600 + ID;
    tx_buf.len = 8;
    can.write(tx_buf); //TPDO2定时器200*0.5ms
    // HAL_Delay(5);

    tx_buf.data[0] = 0x23;
    tx_buf.data[1] = 0x02;
    tx_buf.data[2] = 0x1A;
    tx_buf.data[3] = 0x01;
    tx_buf.data[4] = 0x20;
    tx_buf.data[5] = 0x00;
    tx_buf.data[6] = 0x64;
    tx_buf.data[7] = 0x60;
    tx_buf.id = 0x600 + ID;
    tx_buf.len = 8;
    can.write(tx_buf); //TPDO2映射0x6064 00
    // HAL_Delay(5);

    tx_buf.data[0] = 0x2F;
    tx_buf.data[1] = 0x02;
    tx_buf.data[2] = 0x1A;
    tx_buf.data[3] = 0x00;
    tx_buf.data[4] = 0x01;
    tx_buf.data[5] = 0x00;
    tx_buf.data[6] = 0x00;
    tx_buf.data[7] = 0x00;
    tx_buf.id = 0x600 + ID;
    tx_buf.len = 8;
    can.write(tx_buf); //TPDO2开启1个映射
    // HAL_Delay(5);

    return 0x00;
}

//TPDO3事件触发
//TPDO3映射0x603F 00（故障码）
//TPDO0禁止时间500ms
//TPDO3-COB-ID:0x480 + ID
uint8_t TPDO3Config(uint8_t ID, CANRaw &can)
{
    tx_buf.id = 0x600 + ID;
    tx_buf.len = 8;
    tx_buf.data[0] = 0x2F;
    tx_buf.data[1] = 0x03;
    tx_buf.data[2] = 0x1A;
    tx_buf.data[3] = 0x00;
    tx_buf.data[4] = 0x00;
    tx_buf.data[5] = 0x00;
    tx_buf.data[6] = 0x00;
    tx_buf.data[7] = 0x00;
    can.write(tx_buf);// 清空TPDO3映射

    tx_buf.data[0] = 0x2F;
    tx_buf.data[1] = 0x03;
    tx_buf.data[2] = 0x18;
    tx_buf.data[3] = 0x02;
    tx_buf.data[4] = 0xFE;
    tx_buf.data[5] = 0x00;
    tx_buf.data[6] = 0x00;
    tx_buf.data[7] = 0x00;
    tx_buf.id = 0x600 + ID;
    tx_buf.len = 8;
    can.write(tx_buf); //TPDO3事件触发
    // HAL_Delay(5);

    tx_buf.data[0] = 0x2B;
    tx_buf.data[1] = 0x03;
    tx_buf.data[2] = 0x18;
    tx_buf.data[3] = 0x03;
    tx_buf.data[4] = 0xE8;
    tx_buf.data[5] = 0x03;
    tx_buf.data[6] = 0x00;
    tx_buf.data[7] = 0x00;
    tx_buf.id = 0x600 + ID;
    tx_buf.len = 8;
    can.write(tx_buf); //TPDO3禁止时间1000*0.5ms
    // HAL_Delay(5);

    tx_buf.data[0] = 0x23;
    tx_buf.data[1] = 0x03;
    tx_buf.data[2] = 0x1A;
    tx_buf.data[3] = 0x01;
    tx_buf.data[4] = 0x10;
    tx_buf.data[5] = 0x00;
    tx_buf.data[6] = 0x3F;
    tx_buf.data[7] = 0x60;
    tx_buf.id = 0x600 + ID;
    tx_buf.len = 8;
    can.write(tx_buf); //TPDO3映射0x603F 00
    // HAL_Delay(5);

    tx_buf.data[0] = 0x2F;
    tx_buf.data[1] = 0x03;
    tx_buf.data[2] = 0x1A;
    tx_buf.data[3] = 0x00;
    tx_buf.data[4] = 0x01;
    tx_buf.data[5] = 0x00;
    tx_buf.data[6] = 0x00;
    tx_buf.data[7] = 0x00;
    tx_buf.id = 0x600 + ID;
    tx_buf.len = 8;
    can.write(tx_buf); //TPDO3开启1个映射
    // HAL_Delay(5);

    return 0x00;
}

uint32_t GetCOBID(uint8_t ID, uint8_t type)
{
    uint32_t retval = 0;
    switch (type)
    {
    case 0:
        retval = (0x80);
        break;
    case 1:
        retval = (0x280 + ID);
        break;
    case 2:
        retval = (0x380 + ID);
        break;        
    default:
        break;
    }
    return retval; 
}

//NMT发送
//写入0x01 0x01：开启1号驱动PDO传输
uint8_t NMTControl(uint8_t ID, uint8_t Data0, uint8_t Data1, CANRaw &can)
{
    uint8_t Data[2] = {Data0, Data1};

    tx_buf.id = 0x000 + ID;
    tx_buf.len = 0x2;
    tx_buf.data[0] = Data0;
    tx_buf.data[1] = Data1;
    return can.write(tx_buf);
}

uint8_t SYNCMessage(CANRaw &can)
{
    // No data for the SYNC message
    tx_buf.len = 0;
    // Set the data bytes to zero.
    memset(tx_buf.data, 0, 8);
    tx_buf.id = 0x80; // SYNC COB-ID

    // Send the sync message
    return can.write(tx_buf);
}

//使能电机
uint8_t DriverEnable(uint8_t ID, CANRaw &can)
{
    tx_buf.data[0] = 0x0F;
    tx_buf.data[1] = 0x00;
    tx_buf.id = 0x200 + ID;
    tx_buf.len = 0x2;
    return can.write(tx_buf);
}

//速度模式初始化
uint8_t ProfileVelocityInit(uint8_t ID, CANRaw &can)
{
    uint32_t acc = 30; //unit:r/sec^2 
    // uint8_t Data[8] = {0x2F, 0x60, 0x60, 0x00, 0x03, 0x00, 0x00, 0x00}; //设置速度模式
    tx_buf.id = 0x600 + ID;
    tx_buf.len = 8;
    tx_buf.data[0] = 0x2F;
    tx_buf.data[1] = 0x60;
    tx_buf.data[2] = 0x60;
    tx_buf.data[3] = 0x00;
    tx_buf.data[4] = 0x03;
    tx_buf.data[5] = 0x00;
    tx_buf.data[6] = 0x00;
    tx_buf.data[7] = 0x00;
    can.write(tx_buf);
    // HAL_Delay(5);

    uint32_t acc_count = acc*4096/10;
    acc_count = __REV(acc_count);
    uint8_t *ptr = (uint8_t*) &acc_count;
    tx_buf.data[0] = 0x23;
    tx_buf.data[1] = 0x83;
    tx_buf.data[2] = 0x60;
    tx_buf.data[3] = 0x00;
    tx_buf.data[4] = *(ptr + 0);
    tx_buf.data[5] = *(ptr + 1);
    tx_buf.data[6] = *(ptr + 2);
    tx_buf.data[7] = *(ptr + 3);//高位在后
    tx_buf.id = 0x600 + ID;
    tx_buf.len = 8;
    can.write(tx_buf); //设置电机加速度r/s^2
    // HAL_Delay(5);

    tx_buf.data[0] = 0x23;
    tx_buf.data[1] = 0x84;
    tx_buf.data[2] = 0x60;
    tx_buf.data[3] = 0x00;
    tx_buf.data[4] = *(ptr + 0);
    tx_buf.data[5] = *(ptr + 1);
    tx_buf.data[6] = *(ptr + 2);
    tx_buf.data[7] = *(ptr + 3);
    tx_buf.id = 0x600 + ID;
    tx_buf.len = 8;
    can.write(tx_buf); //设置电机减速度r/s^2
    // HAL_Delay(5);

    return 0x00;
}

//Set torque acceleration to max.
uint8_t ProfileTorqueInit(uint8_t ID, CANRaw &can)
{
    uint32_t slope = 10000; //now set to be 10000 mA/s 
    // uint8_t Data[8] = {0x2F, 0x60, 0x60, 0x00, 0x03, 0x00, 0x00, 0x00}; //设置力矩模式
    tx_buf.id = 0x600 + ID;
    tx_buf.len = 8;
    tx_buf.data[0] = 0x2F;
    tx_buf.data[1] = 0x60;
    tx_buf.data[2] = 0x60;
    tx_buf.data[3] = 0x00;
    tx_buf.data[4] = 0x04;
    tx_buf.data[5] = 0x00;
    tx_buf.data[6] = 0x00;
    tx_buf.data[7] = 0x00;
    can.write(tx_buf);
    // HAL_Delay(5);

    wait_us(500);

    // slope = __REV(slope);
    // uint8_t *ptr = (uint8_t*) &slope;
    tx_buf.data[0] = 0x23;
    tx_buf.data[1] = 0x13;
    tx_buf.data[2] = 0x21;
    tx_buf.data[3] = 0x00;
    tx_buf.data[4] = slope & 0xFF;
    tx_buf.data[5] = (slope >> 8) & 0xFF;
    tx_buf.data[6] = (slope >> 16) & 0xFF;
    tx_buf.data[7] = (slope >> 24) & 0xFF;
    tx_buf.id = 0x600 + ID;
    tx_buf.len = 8;
    can.write(tx_buf); //设置电流“加速度”
    // HAL_Delay(5);

    return 0x00;
}


uint8_t EnterVelocityMode(uint8_t ID, CANRaw &can){
    RPDO0Config(ID, can);
    
    RPDO2Config(ID, can);

    TPDO0Config(ID, can); //Tpdo 映射为实际速度和电流 0.01A

    
    ProfileVelocityInit(ID, can);
    // usleep(200);
    wait_us(200);

    NMTControl(0x00, 0x01, ID, can);
    // usleep(200);
    wait_us(200);

    DriverEnable(ID, can);

    return 0;
}

uint8_t EnterTorqueMode(uint8_t ID, CANRaw &can){
    RPDO0Config(ID, can);

    wait_us(500);
    
    RPDO1Config(ID, can);

    wait_us(500);

    TPDO0Config(ID, can); //Tpdo 映射为实际速度和电流 0.01A

    wait_us(500);
    
    ProfileTorqueInit(ID, can);
    wait_us(1000);

    NMTControl(0x00, 0x01, ID, can);
    wait_us(1000);

    DriverEnable(ID, can);

    return 0;
}

/* set the velosity of the wheel and run
    input: vel--velocity (r/min)
            ID--node ID indicate the wheel
    output: none
 */
void MoveVelocityCMD(uint8_t ID, int32_t Vel, CANRaw &can){
    int32_t vel_count = __REV(Vel*4096);
    int8_t *ptr = (int8_t*) &vel_count;
    tx_buf.data[0] = *(ptr + 0);
    tx_buf.data[1] = *(ptr + 1);
    tx_buf.data[2] = *(ptr + 2);
    tx_buf.data[3] = *(ptr + 3);//高位在后
    tx_buf.id = 0x400 + ID;
    tx_buf.len = 0x4;
    can.write(tx_buf);
}
/* set the velosity of the wheel and run
    input: vel--velocity (r/min)
            ID--node ID indicate the wheel
    output: none
 */
void MovePositionCMD(uint8_t ID, int32_t Vel, CANRaw &can){
    int32_t vel_count = __REV(Vel);
    int8_t *ptr = (int8_t*) &vel_count;
    tx_buf.data[0] = *(ptr + 0);
    tx_buf.data[1] = *(ptr + 1);
    tx_buf.data[2] = *(ptr + 2);
    tx_buf.data[3] = *(ptr + 3);//高位在后
    tx_buf.id = 0x500 + ID;
    tx_buf.len = 0x4;
    can.write(tx_buf);
}

/* set the torque of the wheel and run
    input: torque-- (0.01A)
            ID--node ID indicate the wheel
    output: none
 */
void MoveTorqueCMD(uint8_t ID, int32_t current, CANRaw &can) {
    tx_buf.data[0] = current & 0xFF;
    tx_buf.data[1] = (current >> 8) & 0xFF;
    tx_buf.data[2] = (current >> 16) & 0xFF;
    tx_buf.data[3] = (current >> 24) & 0xFF;
    tx_buf.id = 0x300 + ID;
    tx_buf.len = 0x4;
    can.write(tx_buf);
}

void unpack_speed_torque_reply(uint8_t *rx_data, float32_t &pcurrency, float32_t &pspeed)
{
    /*  based on the PDO mapping, the first 4 bytes are current information
    and the last 4 bytes are velocity information
    */
    int16_t torque_count = (rx_data[1] << 8) | rx_data[0];
    int32_t velocity_count = (uint32_t(rx_data[5]) << 24) | uint32_t((rx_data[4]) << 16) | uint32_t((rx_data[3]) << 8) | uint32_t(rx_data[2]);
    pcurrency = (torque_count*0.01f); //A
    pspeed = (velocity_count*6.0f/4096.0f);  //rpm
}