/**
 * @file global_status.h
 * @author sethome
 * @brief 全局状态机
 * @version 0.1
 * @date 2022-03-25
 *
 * @copyright sethome Copyright (c) 2022
 *
 */

#include "stdint.h"

#define __GLOBAL_STATUS_H__
#ifdef __GLOBAL_STATUS_H__

// 错误码
enum err_e
{
    GIMBAL_ERR = 0,
    CHASSIS_ERR,
    SHOOT_ERR,
    CAP_ERR,
    REMOTE_ERR,
    PC_ERR,
};
enum ctl_e
{
    RC = 0,
    PC,
};
// 适合十几个简单状态的情况
struct GlobalStatus_t
{
    uint8_t err[6]; // 9

    enum mode_e
    {
        LOCK = 0,
        FLOW,
        SPIN,

    } mode;

    enum cap_e
    {
        STOP = 0, // 此时电容应在充电
        FULL,     // 全力响应
    } cap;
//auto control
    struct
    {
        enum auto_mode_e
        {
            NONE,       // 不启用自瞄
            CAR,        // 车
            OUTPOST,    // 前哨站
            LOWTARGET,  // 小符
            HIGHTARGET, // 大符
        } mode;
        struct
        {
            float shoot_yaw;
            float shoot_pitch;
            uint8_t Auto_control_online; //自瞄刷新
            uint8_t fire;      // 是否启用自瞄
            uint8_t target_id; // 目标ID
        } input;
    } Auto;
    struct
    {
        /* data */
        float x, y, r; // 底盘移动
        float pitch, yaw;

        uint8_t shooter_status;
        uint8_t shoot_fire;
        uint8_t shoot_num;
        int shoot_RC;

        uint8_t trigger_begin;
        uint8_t trigger_kill;
        uint8_t trigger_kill_cnt;

        enum ctl_e ctl;
    } input;
};

extern struct GlobalStatus_t Global;

void Global_set_err(enum err_e err, uint8_t status);

#endif
// end of file