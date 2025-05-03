/**
 * @file gimbal.c
 * @author sethome
 * @brief
 * @version 0.1
 * @date 2022-11-20
 *
 * @copyright Copyright (c) 2022
 *
 */
#include "gimbal.h"
#include "control_setting.h"
#include "NUC_communication.h"
#include "Global_status.h"

#include "IMU_updata.h"
#include "CAN_receive&send.h"

#include "Stm32_time.h"
#include "stm32f4xx_hal.h"

#include "math.h"
struct gimbal_status gimbal;
/* 编码器pid*/
pid_t pitch_location_speed_pid;
pid_t pitch_location_pid;

pid_t yaw_location_speed_pid;
pid_t yaw_location_pid;
/* 陀螺仪pid*/
pid_t pitch_absolute_speed_pid;
pid_t pitch_absolute_pid;

pid_t yaw_absolute_speed_pid;
pid_t yaw_absolute_pid;
/* 自瞄pid*/
pid_t pitch_auto_speed_pid;
pid_t pitch_auto_pid;

pid_t yaw_auto_speed_pid;
pid_t yaw_auto_pid;
/*前馈参数*/
float KF = 300.0f;
// 目标值
float Target = 0.0f;
float Pre_Target = 0.0f;
// 实际值
float Now = 0.0f;
float forward_out;



// 云台初始化
void gimbal_init()
{
	/*编码器pid*/ 
	pid_set(&pitch_location_pid, 20.0f, 0.0f,0.0f, 8000.0f, 0.01f); 		//pitch
	pid_set(&pitch_location_speed_pid, 8000.0f, 0.0f, 0.0f, 30000.0f, 20000.0f); 
	
	pid_set(&yaw_location_speed_pid, 1.0f, 0.0f, 22.0f, 20000.0f, 13000.0f); 
	pid_set(&yaw_location_pid, 72000.0f, 0.0f, 4850.0f, 15000.0f, 0.0f);   		//yaw
	
	/* 陀螺仪pid*/
	pid_set(&pitch_absolute_pid, 12.0f, 0.0f, 0.0f, 30000.0f, 20000.0f); 		//pitch
	pid_set(&pitch_absolute_speed_pid, 11000.0f, 0.0f,0.1f, 20000.0f, 0.01f);	
	
  pid_set(&yaw_absolute_pid, 18.0f, 0, 0.0, 25000.0f, 3600.0f);		//yaw
  pid_set(&yaw_absolute_speed_pid, 9000, 2, 0000, 25000.0f, 0.0f);
	
	/* 自瞄pid*/
	pid_set(&pitch_auto_pid, 6.0f, 0.0f, 0.0f, 27000.0f, 20000.0f);  		//pitch
	pid_set(&pitch_auto_speed_pid, 6000.0f, 0.0f, 0.0f, 20000.0f, 0.01f);

	pid_set(&yaw_auto_pid, 4.0f, 0.0f, 0.0f, 27000.0f, 3600.0f); 		//yaw
	pid_set(&yaw_auto_speed_pid, 4000.0f, 5.0f, 0.0f, 25000.0f, 0.0f);

	/*云台相关参数初始化*/
	gimbal.pitch.now = 0;
	gimbal.pitch.set = 0;
	//  gimbal.pitch.offset = 0;
	gimbal.pitch.stable = 0;
	gimbal.set_pitch_speed = 0;

	gimbal.yaw.now = 0;
	gimbal.yaw.set = 0;
	//  gimbal.yaw.offset = 0;
	gimbal.yaw.stable = 0;
	gimbal.set_yaw_speed = 0;

	/* 云台模式*/
	gimbal.yaw_status = ABSOLUTE; // 为保证控制效果，全为陀螺仪控制
	gimbal.pitch_status = ABSOLUTE;
}

void gimbal_updata()
{
	// 设置云台电机
	decode_as_6020(YAW_MOTOR);
	decode_as_6020(PITCH_MOTOR);
	
	if (gimbal.yaw_status == LOCATION)
	{
	gimbal.yaw_speed = (get_motor_data(YAW_MOTOR).ecd - get_motor_data(YAW_MOTOR).last_ecd) / ECD_MAX;
  gimbal.yaw.now = degree2rad(get_motor_data(YAW_MOTOR).angle_cnt - gimbal.yaw.location_offset);
	}
	else if(gimbal.yaw_status == ABSOLUTE )
	{
		gimbal.yaw.now = IMU_data.AHRS.yaw_rad_cnt;
		gimbal.yaw_speed = cos(IMU_data.AHRS.pitch)*IMU_data.gyro[2]
							-sin(IMU_data.AHRS.pitch)*IMU_data.gyro[0];//多角度融合	
	}
	
	if (gimbal.pitch_status == LOCATION) 
	{
		gimbal.pitch.now = degree2rad(get_motor_data(PITCH_MOTOR).angle - gimbal.pitch.location_offset);
		gimbal.pitch_speed = IMU_data.gyro[1]; // 获取y转轴（对应pitch轴）角速度，做闭环用。
	}
	else if (gimbal.pitch_status == ABSOLUTE)
	{
		gimbal.pitch.now = IMU_data.AHRS.pitch - degree2rad(gimbal.pitch.absoulte_offset); 
		gimbal.pitch_speed = IMU_data.gyro[1];
	}
}

//云台控制
void gimbal_pid_cal()
{
	gimbal_set_pitch(gimbal.pitch.set, 10.0f, 20.0f);//pitch限位
	gimbal_set_yaw( get_motor_data(YAW_MOTOR).angle, -10.0f,56.0f);    //yaw限位
	
	/**********************************yaw轴控制***********************************************/
	
	// 编码器控制
	if (gimbal.yaw_status == LOCATION) 
	{
		gimbal.set_yaw_speed = pid_cal(&yaw_location_pid, gimbal.yaw.now, gimbal.yaw.set);
		set_motor(pid_cal(&yaw_location_speed_pid, gimbal.yaw_speed*KF, gimbal.set_yaw_speed), YAW_MOTOR);
	}
	// 陀螺仪控制
	else if (gimbal.yaw_status == ABSOLUTE ) 
	{
		if(Global.Auto.mode == CAR)	//自瞄单独一套pid
		{
			gimbal.set_yaw_speed = pid_cal(&yaw_auto_pid, gimbal.yaw.now, gimbal.yaw.set);
			set_motor(-pid_cal(&yaw_auto_speed_pid, gimbal.yaw_speed, gimbal.set_yaw_speed) + get_motor_data(YAW_MOTOR).round_speed * KF, YAW_MOTOR);
		}
		else	//其他模式
		{
			gimbal.set_yaw_speed = pid_cal(&yaw_absolute_pid, gimbal.yaw.now, gimbal.yaw.set);
			set_motor(-pid_cal(&yaw_absolute_speed_pid, gimbal.yaw_speed, gimbal.set_yaw_speed) + get_motor_data(YAW_MOTOR).round_speed * KF, YAW_MOTOR);
		}
	}
	else
		gimbal.set_yaw_speed = gimbal.yaw_speed;

	/**********************************pitch轴控制***********************************************/

	// 编码器控制
	if (gimbal.pitch_status == LOCATION)
	{
		gimbal.set_pitch_speed = pid_cal(&pitch_location_pid, gimbal.pitch.now, gimbal.pitch.set);
		set_motor(pid_cal(&pitch_location_speed_pid, gimbal.pitch_speed, gimbal.set_pitch_speed), PITCH_MOTOR);
	}
	// 陀螺仪控制
	else if (gimbal.pitch_status == ABSOLUTE) 
	{
		if(Global.Auto.mode == CAR)	//自瞄单独一套pid
		{
			gimbal.set_pitch_speed = pid_cal(&pitch_auto_pid, gimbal.pitch.now, gimbal.pitch.set);
			set_motor(-pid_cal(&pitch_auto_speed_pid, gimbal.pitch_speed, gimbal.set_pitch_speed), PITCH_MOTOR);
		}
		else	//其他模式
		{
			gimbal.set_pitch_speed = pid_cal(&pitch_absolute_pid, gimbal.pitch.now, gimbal.pitch.set);
			set_motor(-pid_cal(&pitch_absolute_speed_pid, gimbal.pitch_speed, gimbal.set_pitch_speed), PITCH_MOTOR);
		}
	}
	
	else
		gimbal.set_pitch_speed = gimbal.pitch_speed;
}

//自瞄云台控制数据传入
void Gimbal_set_yaw_angle(float angle)
{
	gimbal.yaw.set = angle;
}

void Gimbal_set_pitch_angle(float angle)
{
	gimbal.pitch.set = angle;
}

/**********************************其他控制***********************************************/

// 零点设置，在main.c中被调用
void gimbal_set_offset(float ab_pitch, float ab_yaw, float lo_pitch, float lo_yaw)
{
	gimbal.pitch.absoulte_offset = ab_pitch;
	gimbal.yaw.absoulte_offset = ab_yaw;
	gimbal.pitch.location_offset = lo_pitch;
	gimbal.yaw.location_offset = lo_yaw;
}

// 设置pitch限位
void gimbal_set_pitch(float pitch, float up_angle, float down_angle)
{

	if (rad2degree(pitch) > up_angle)
	{
		gimbal.pitch.set = degree2rad(up_angle);
	}
	else if (rad2degree(pitch) < -down_angle)
	{
		gimbal.pitch.set = degree2rad(-down_angle);
	}
}
//设置yaw轴限位（由于无人机无滑环，此处传入编码器单圈角度值，180~-180度）
void gimbal_set_yaw(float yaw_now_angle,float left_angle,float right_angle)
{
  if (yaw_now_angle > right_angle)		//撞到右限位
  {
		if(gimbal.yaw.set-gimbal.yaw.now<0)		//如果继续往右转
			gimbal.yaw.set = gimbal.yaw.now;
  }
  else if(yaw_now_angle< left_angle)		//撞到左限位
  {
		if(gimbal.yaw.set-gimbal.yaw.now>0)		//如果继续往左转
			gimbal.yaw.set = gimbal.yaw.now;
  }
}