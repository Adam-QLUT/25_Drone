#ifndef NUC_COMMUNICATION_H
#define NUC_COMMUNICATION_H


#define DATA_SIZE 30
#define YAW_DATA_HIS 1
#define PITCH_DATA_HIS 0
#define ANGLE_TO_RAD 0.01745329251994329576923690768489f
#define RAD_TO_ANGLE 57.295779513082320876798154814105f
#define PI 3.1415926f


#include "referee_handle_pack.h"

extern float auto_yaw,auto_pitch,visual_yaw,trans_delay_data,raw_vision_data;

typedef struct
{
	uint8_t header;
	uint8_t useless;
	uint8_t target_id;
	float yaw;
	float pitch;
	float pitch_speed;
	float yaw_speed;//上面的角度是°为单位，收到的速度是弧度/s。
	uint8_t distance;
  uint8_t shoot;//发弹标志位
	uint8_t shoot_mode;//发弹标志位
	uint32_t his_time_data;
	char unknown[4];//原先是20，改为16，因为时间戳占用了四个字节
	uint16_t checksum;
} __attribute__((packed)) NUC_data_t;


extern NUC_data_t fromNUC;

//自瞄

typedef struct
{
    struct
    {
        uint8_t sof;
        uint8_t crc8;
    }__attribute__((packed)) FrameHeader; // 2
    struct
    {
        float curr_yaw;
        float curr_pitch;
        float curr_omega;
        uint8_t state;
        uint8_t autoaim;
        uint8_t enemy_color;
    }__attribute__((packed)) To_minipc_data; // 15
    struct
    {
        uint16_t crc16;
    }__attribute__((packed)) FrameTailer;
    uint8_t enter;
}__attribute__((packed)) STM32_data_t;


typedef struct  
{
    struct
    {
        uint8_t sof;
        uint8_t crc8;
    }__attribute__((packed)) FrameHeader; // 2
    struct
    {
        float shoot_yaw;
        float shoot_pitch;
        uint8_t fire;      // 发弹信号
        uint8_t target_id; // 目标ID,UI显示用
    }__attribute__((packed)) from_minipc_data;    // 15
    struct
    {
        uint16_t crc16;
    }__attribute__((packed)) FrameTailer;
}__attribute__((packed)) MINIPC_data_t;


void STM32_to_MINIPC();
void decodeMINIPCdata(MINIPC_data_t *target, unsigned char buff[], unsigned int len);
void Auto_control();
void MINIPC_to_STM32();

extern MINIPC_data_t fromMINIPC;
extern STM32_data_t toMINIPC;

#endif
//end of file
