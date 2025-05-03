/**
 * @file chassis_move.h
 * @author sethome
 * @brief 底盘控制文件
 * @version 0.1
 * @date 2022-11-20
 *
 * @copyright Copyright (c) 2022
 *
 */
#include "struct_typedef.h"
#include "referee_handle_pack.h"

#define __chassis_move_H_
#ifdef __chassis_move_H_

/* motorID 1 %++++++% 0
				++++
				++++
			2 %++++++% 3 */
#define chassis_FR CAN_1_1
#define chassis_FL CAN_1_2
#define chassis_BL CAN_1_3
#define chassis_BR CAN_1_4

struct chassis_status
{
	struct
	{
		float x, y, r;
		float now_x, now_y, now_r;
		float last_x, last_y, last_r;
		float max_x, max_y, max_r; // m/s
	} speed;

	struct
	{
		float now_x, now_y, now_r;
		float max_x, max_y, max_r; // m/s^2
	} acc;
	enum chassis_status_e
	{
		park = 0,
		move,
	} status;

	int32_t wheel_current[4]; // PID输出的电调电流
	double wheel_speed[4];
	double angle_set[4];
	double angle_now[4];

	uint8_t is_open_cap;
};
extern struct chassis_status chassis;

struct cap
{
	float cap_vol;
	float cap_now_vol;
	float cap_last_vol;
	float cap_error_vol;
};
extern struct cap cap_chassis;

extern ext_power_heat_data_t power_heat_data;

extern float Power;
extern float wheel_rpm[4];
extern uint16_t Engerny_buffer;

void chassis_move_init(void);				// 底盘移动初始化
void chassis_moto_speed_current_calc(void); // 计算马达速度
void chassis_Flow_mode(void);
void chassis_Spin_mode(void);
void val_limit(float *val, float MAX);
/*********************以下函数没有被用到，保留以防以后用到***********************/
// 变速小陀螺
float generate_random_float(float min, float max);
float random_anti_vision_r_s(float min, float max);
int RampInc_float(int16_t *buffer, float now, float ramp);
// 飞坡用
float fly_speed_up(float *set, float acceleration_increase_rate, float limit);
float fly_speed_up_ex(float a, float b, float c, float v);

#endif

// end of file
