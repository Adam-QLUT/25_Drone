/**
 * @file shoot.c
 * @author sethome
 * @brief 发射模块
 * @version 0.1
 * @date 2022-11-20
 *
 * @copyright Copyright (c) 2022
 *
 */
#include "shoot.h"
#include "stdio.h"
#include "pid.h"
#include "Stm32_time.h"
#include "math.h"
#include "global_status.h"
#include "CAN_receive&send.h"
#include "referee_handle_pack.h"
#include "NUC_communication.h"

pid_t trigger_speed_pid;
pid_t trigger_location_pid;

pid_t shoot1_speed_pid;
pid_t shoot2_speed_pid;

shoot_t shoot;

uint16_t booster_cnt = 0;
uint8_t booster_status = 0;
uint8_t trigger_cnt_auto;

// 初始化
void shoot_init()
{
#ifdef USE_3508_AS_SHOOT_MOTOR
	// 摩擦轮电机
	pid_set(&shoot1_speed_pid, 25, 0, 0, 3500, 0);
	pid_set(&shoot2_speed_pid, 25, 0, 0, 3500, 0);

#endif
	// 拨弹电机
	pid_set(&trigger_speed_pid, 9, 0, 0.1, 16000, 0);
	pid_set(&trigger_location_pid, 10, 0, 1, 6000, 0);

	// 改用双环控制后，如果想让拨弹速度变大，那么位置环的p数值一定不能小，而且建议有弹链的情况下p稍微超调一点，可以起到预压紧固的效果
	shoot.trigger_status = SPEEDS;
	//	shoot.trigger_status = LOCATIONS;//默认双环控制
	shoot.speed_level = SHOOT_STOP;
	shoot.trigger_location.set = 0; // 初始上电检测的位置不知道为什么会变成5左右，加个固定补偿
	shoot.trigger_location.now = 0;
}

// 更新拨弹电机数据
void shoot_update()
{
#ifdef USE_3508_AS_SHOOT_MOTOR
	// 如果使用3508作为摩擦轮电机的话
	decode_as_3508(SHOOT_MOTOR1);
	decode_as_3508(SHOOT_MOTOR2);
	shoot.shoot_speed[0] = get_motor_data(SHOOT_MOTOR1).speed_rpm;
	shoot.shoot_speed[1] = get_motor_data(SHOOT_MOTOR2).speed_rpm;
#endif

	decode_as_2006(TRIGGER_MOTOR);
	shoot.trigger_location.now = get_motor_data(TRIGGER_MOTOR).angle_cnt;
	shoot.trigger_speed = get_motor_data(TRIGGER_MOTOR).speed_rpm;

	shoot.trigger_given_current = get_motor_data(TRIGGER_MOTOR).given_current;
}

void shoot_set_shoot_Motor_speed(float speed)
{
#ifdef USE_3508_AS_SHOOT_MOTOR
	// 如果使用3508作为摩擦轮电机的话
	set_motor(pid_cal(&shoot1_speed_pid, get_motor_data(SHOOT_MOTOR1).speed_rpm, speed), SHOOT_MOTOR1);
	set_motor(pid_cal(&shoot2_speed_pid, get_motor_data(SHOOT_MOTOR2).speed_rpm, -speed), SHOOT_MOTOR2);
#else
	// 适配其他拨弹电机
	PWM_snaill_set(PIN_2, (uint16_t)speed);
	PWM_snaill_set(PIN_3, (uint16_t)speed);
#endif
}

void shoot_pid_cal(void) // 融合了多个功能，射速切换，热量控制，自瞄非自瞄pid不同
{
	float set;
		set = 4000.0f;
	// 摩擦轮设定
	decode_as_3508(SHOOT_MOTOR1);
	shoot_set_shoot_Motor_speed((float)shoot.speed_level);

	decode_as_2006(TRIGGER_MOTOR);

	// 拨弹电机设定

	if (shoot.trigger_status == SPEEDS) // 速度控制
	{
			pid_set(&trigger_speed_pid, 7, 0.1, 5, 16000, 300);
	}

	if (Global.input.shoot_fire)
	{
		Trigger_anti_kill_and_set_speed(set/2.0f);
	}
	else // 遥控器控制
		Trigger_anti_kill_and_set_speed(Global.input.shoot_RC*20.0f);
	// 速度环
	if (shoot.set_trigger_speed || get_motor_data(TRIGGER_MOTOR).speed_rpm >= 5)
		set_motor(pid_cal(&trigger_speed_pid, get_motor_data(TRIGGER_MOTOR).speed_rpm, shoot.set_trigger_speed), TRIGGER_MOTOR);
	
	if (!shoot.set_trigger_speed && get_motor_data(TRIGGER_MOTOR).speed_rpm <= 5)
		set_motor(0, TRIGGER_MOTOR); // 防抖直接给拨弹盘电机给0
}

void shoot_speed_limit()
{
	if (Global.input.shooter_status == 0)
	{
		shoot.speed_level = SHOOT_STOP;
		booster_status = 0;
		booster_cnt = 0;
	}
	else if (Global.input.shooter_status == 1)
	{
		booster_cnt++;
		if (booster_cnt >= 500)
		{
			booster_status = 1;
		}

		if (booster_status == 1)
		{
			shoot.speed_level = SHOOT_30;
		}
		else if (booster_status == 0)
			shoot.speed_level = SHOOT_BEGIN; // 刚开始时让摩擦轮反转防止卡弹，不然就寄了
	}
}

void Trigger_anti_kill_and_set_speed(float set) // 设置拨弹盘速度，内嵌堵转情况下自动反转功能
{
	shoot.trigger_status = SPEEDS;			 // 速度环控制，进行模式切换
	if (shoot.trigger_given_current > 10000) // 达到卡弹电流阈值
		Global.input.trigger_kill_cnt = 18;	 // 延时作用

	if (Global.input.trigger_kill_cnt > 5)
	{
		Global.input.trigger_kill_cnt--;
		Global.input.trigger_kill = 1; // 卡弹标志位置一
	}
	else if (Global.input.trigger_kill_cnt == 5) // 总共执行13次反转
		Global.input.trigger_kill = 0;

	if (Global.input.trigger_kill == 1)
	{
		shoot.set_trigger_speed = -10000;
	}

	else // 下面才是正常发弹逻辑
	{
		shoot.set_trigger_speed = set; // 正常传入
	}
}


//end of file