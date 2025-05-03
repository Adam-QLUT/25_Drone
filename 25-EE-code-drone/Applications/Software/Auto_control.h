#ifndef __AUTO_CONTROL_H__
#define __AUTO_CONTROL_H__

#include "stdint.h"

#pragma pack(1)

typedef struct
{
    struct
    {
        uint8_t sof;
        uint8_t crc8;
    } FrameHeader; // 2
    struct
    {
        float curr_yaw;
        float curr_pitch;
        float curr_omega;
        uint8_t state;
        uint8_t autoaim;
        uint8_t enemy_color;
    } To_minipc_data; // 15
    struct
    {
        uint16_t crc16;
    } FrameTailer;
    uint8_t enter;
} STM32_data_t;

typedef struct
{
    struct
    {
        uint8_t sof;
        uint8_t crc8;
    } FrameHeader; // 2
    struct
    {
        float shoot_yaw;
        float shoot_pitch;
        uint8_t fire;      // 发弹信号
        uint8_t target_id; // 目标ID,UI显示用
    } from_minipc_data;    // 15
    struct
    {
        uint16_t crc16;
    } FrameTailer;
} MINIPC_data_t;

#pragma pack(4)

void STM32_to_MINIPC();
void decodeMINIPCdata(MINIPC_data_t *target, unsigned char buff[], unsigned int len);
void Auto_control();
void MINIPC_to_STM32();

extern MINIPC_data_t fromMINIPC;
extern STM32_data_t toMINIPC;

#endif