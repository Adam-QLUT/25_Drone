/**
 * @file shoot.h
 * @author sethome
 * @brief 发射模块
 * @version 0.1
 * @date 2022-11-20
 *
 * @copyright Copyright (c) 2022
 *
 */
#include "stdint.h"
#include "struct_typedef.h"
#define _SHOOT_H_
#ifdef _SHOOT_H_

#define USE_3508_AS_SHOOT_MOTOR

// 拨弹电机配置
#define TRIGGER_MOTOR CAN_1_8
#define A_BULLET_ANGEL 49.0f

// 摩擦轮电机配置 3508
#ifdef USE_3508_AS_SHOOT_MOTOR
#define SHOOT_MOTOR1 CAN_1_4
#define SHOOT_MOTOR2 CAN_1_3
#endif

#pragma anon_unions

enum shoot_speed // 发射机构速度，此处为步兵定制
{
		SHOOT_BEGIN = 5000,
    SHOOT_STOP = 0,
    SHOOT_30 = -6000,//15000
};

enum trigger_status_e // 拨弹轮控制模式
{
    LOCATIONS = 0,
    SPEEDS,
};

typedef struct
{
    /* data */
    enum shoot_speed speed_level;

    // 电机数据
    float shoot_speed[2]; // 摩擦轮速度

    struct
    {
        /* data */
        float now;
        float set;
        float off_set;
    } trigger_location; // 拨弹电机位置

    float trigger_speed; // 拨弹电机速度
    float set_trigger_speed;
    float trigger_given_current; 		
    enum trigger_status_e trigger_status;
} shoot_t;


extern shoot_t shoot;
extern uint8_t trigger_cnt_auto;

// 外部调用
void shoot_init(void);    // 初始化

void shoot_update(void);  // 更新拨弹电机速度等

void shoot_pid_cal(void); // 计算pid

void shoot_speed_limit();//

void Trigger_anti_kill_and_set_speed(float set);

void shoot_trigger_online();//PC操作

void shoot_set_trigger_location(int n);

int shoot_Bullets(int n); // 发射N颗子弹

void triger_clear_cnt();

void shoot_set_trigger_speed(int set);//拨弹速度控制

void PC_trigger_set_speed(float set);
#endif

// end of file
