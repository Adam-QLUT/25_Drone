#include "CAN_receive&send.h"
#include "IMU_updata.h"
#include "DBUS_remote_control.h"
#include "PWM_control.h"
#include "LED_control.h"
#include "AHRS_MiddleWare.h"
#include "cap_ctl.h"
#include "Laser.h"
#include "can.h"
#include "freertos.h"
#include "cmsis_os2.h"

#include "referee_handle_pack.h"
#include "referee_usart_task.h"
#include "usbd_cdc_if.h"
#include "USB_VirCom.h"

#include "ui.h"
#include "Global_status.h"
#include "Error_detect.h"
#include "chassis_move.h"
#include "gimbal.h"
#include "shoot.h"
#include "math.h"
#include "RampFunc.h"
#include "NUC_communication.h"
#include "control_setting.h"
#include "Stm32_time.h"
#include "usb_device.h"

extern float DBUStime1;

uint8_t is_input_for_rc = 1;
uint32_t Time_LEAN_delay = 0;
uint32_t Time_SPIN_delay = 0;
uint32_t Time_TANK_delay = 0;
uint32_t Time_delay_lid = 0;
uint32_t Time_ANTI_delay = 0;
uint32_t Time_delay_friction_wheel = 0;
uint32_t Time_delay_resetcan = 0;
uint32_t Time_delay_Switch = 0;
uint8_t coverop = 0;

float KS;
uint8_t shoot_flag = 0;

/******************************遥控器拨杆任务**************************************************/
void Remote_Control()
{
	if ((Get_sys_time_ms() - DBUStime1) > 1000.0f)
	{
		image_to_RC(&RC_data); // 切换到图传链路
	}
	// 左上右中  初始不启动摩擦轮的PC模式
	if (switch_is_up(RC_L_SW) && switch_is_mid(RC_R_SW))
	{
		Global.input.ctl = PC;
	}
	// 左上右上  初始启动摩擦轮的PC模式
	if (switch_is_up(RC_L_SW) && switch_is_up(RC_R_SW))
	{
				Global.input.ctl=PC;	
	}
	else
	{
		if(shoot_flag == 0)
			Global.input.shooter_status = 0;
		shoot_flag = 1;	
	}
	// 左中右中   RC
	if (switch_is_mid(RC_L_SW) && switch_is_mid(RC_R_SW))
	{
		Global.mode = FLOW;
		Global.input.ctl=RC;
	}

	// 左下右下   锁死
	if (switch_is_down(RC_L_SW) && switch_is_down(RC_R_SW))
	{
		Global.mode = LOCK;
	}



	// 左下右中   AUTO_AIM
	if (switch_is_down(RC_L_SW) && switch_is_mid(RC_R_SW))
	{
		Global.Auto.mode = CAR;
	}
	else
		Global.Auto.mode = NONE;
}
/******************************RC遥控器操作逻辑**************************************************/
void Remote_Control_RC()
{
	
	// 移动
//	Global.input.r = RC_data.rc.ch[2] / 110.0f;
//	Global.input.x = RC_data.rc.ch[0] / 110.0f;
//	Global.input.y = RC_data.rc.ch[1] / 80.0f;
	// 自瞄
	if (Global.Auto.mode == CAR)
	{
		Global.input.yaw = 0;
		Global.input.pitch = 0;
		Auto_control();
	}
	else
	{
		fromNUC.pitch = 0;
		fromNUC.yaw = 0;
		Global.input.yaw = RC_data.rc.ch[2] / 110000.0f;
		Global.input.pitch = -RC_data.rc.ch[3] / 80000.0f;
	}
	// CH4 波轮
	// 摩擦轮逻辑
	if (RC_data.rc.ch[4] == 0)
	{
		Global.input.shooter_status = 0;
	}
	if ((RC_data.rc.ch[4] > 300 && RC_data.rc.ch[4] < 660))
	{
		Global.input.shoot_num = 1;		 // 连发
		Global.input.shooter_status = 1; // 摩擦轮启动
	}
	// 发弹逻辑
	if (RC_data.rc.ch[4] > 600 && RC_data.rc.ch[4] <= 660)
	{
		Global.input.shoot_RC = 100;
	}
	else if (RC_data.rc.ch[4] > 5000)
	// 对于这个抽象操作的解释：拨轮上拨0-7000，下拨0-660,所以上下两个逻辑
	{
		Global.input.shoot_RC = 220;
	}
	else
	{
		Global.input.shoot_RC = 0; // 没有拨到位置不发子弹
	}

}

/******************************PC操作逻辑**************************************************/
void Remote_Control_PC()
{
	if (switch_is_up(RC_L_SW) && switch_is_up(RC_R_SW))
		{
			if(shoot_flag == 0)
				Global.input.shooter_status = 0;
			if (IF_KEY_PRESSED_R)		
			{
				if (Get_sys_time_ms() - Time_delay_friction_wheel > 350)
				{
					if (Global.input.shooter_status == 0) // 切换状态
					{
						Global.input.shooter_status = 1;//发射标志置位
					}
					else
					{
						Global.input.shooter_status = 0;//发射机构关闭
					}
					Time_delay_friction_wheel = Get_sys_time_ms();
				}
			}
			shoot_flag = 1;			
		}
		else if (switch_is_up(RC_L_SW) && switch_is_mid(RC_R_SW))
		{
			Global.input.shooter_status = 1;
			shoot_flag = 0;
		}
	// 自瞄状态检测
		if(RC_data.mouse.press_r == 1)
		{
			Global.Auto.mode = CAR;
		}
		else
		{
			
			Global.Auto.mode = NONE;
		}
		
	// 云台
	if (Global.Auto.mode == CAR) // 自瞄控制
	{
		Global.input.yaw = 0;
		Global.input.pitch = 0;
		Auto_control();
	}
	else // 手动模式 操作手接管yaw+pitch
	{
		//vision_reset();
		Global.input.yaw = MOUSE_X_MOVE_SPEED / 3000.0f;
		Global.input.pitch = MOUSE_Y_MOVE_SPEED / 2500.0f;
	}

	/*************发射模式控制****************/
	Global.input.shoot_fire = RC_data.mouse.press_l;
	// 摩擦轮开关
	if (IF_KEY_PRESSED_R)
	{
		if (Get_sys_time_ms() - Time_delay_friction_wheel > 350)
		{
			if (Global.input.shooter_status == 0) // 切换状态
			{
				Global.input.shooter_status = 1; // 发射标志置位
			}
			else
			{
				Global.input.shooter_status = 0; // 发射机构关闭
			}

			Time_delay_friction_wheel = Get_sys_time_ms();
		}
	}

	if (IF_KEY_PRESSED_Z) // UI初始化
	{
		//Global.input.ui_init = 1;
	}


//	if (IF_KEY_PRESSED_G)
//	{
//		if (Get_sys_time_ms() - Time_delay_friction_wheel > 350)
//		{
//			if (Global.input.fly_status == 0) // 切换状态
//			{
//				Global.input.fly_status = 1; // 发射标志置位
//				gimbal.pitch_status = LOCATION;
//			}
//			else if (IF_KEY_PRESSED_Q)
//			{
//				Global.input.fly_status = 0; // 发射机构关闭
//				gimbal.pitch_status = ABSOLUTE;
//			}
//			else
//			{
//				Global.input.fly_status = 0; // 发射机构关闭
//				gimbal.pitch_status = ABSOLUTE;
//			}

//			Time_delay_friction_wheel = Get_sys_time_ms();
//		}
//	}
}

void remote_control_task()
{
	Remote_Control();
	if (Global.input.ctl == PC)
		Remote_Control_PC();
	else if (Global.input.ctl == RC)
		Remote_Control_RC();
	else
	{
	}
}