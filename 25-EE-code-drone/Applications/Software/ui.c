#include <ui.h>
#include "referee_handle_pack.h"
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "Global_status.h"
#include "string.h"
#include "shoot.h"
#include "NUC_communication.h"
#include "Stm32_time.h"


void ui_init(){
  ui_self_id=get_robot_id();
  osDelay(20);
  _ui_init_default_Ungroup_0();
  osDelay(20);		
  _ui_init_default_Ungroup_1();
	osDelay(20);		
  _ui_init_default_Ungroup_2();
	osDelay(20);		
  _ui_init_default_Ungroup_3();
	osDelay(20);		
  _ui_init_default_Ungroup_4();
  osDelay(20);		
  _ui_init_default_Ungroup_5();
	osDelay(20);		
}

void ui_updata(){
  _ui_update_default_Ungroup_4();
  osDelay(2);
  _ui_update_default_Ungroup_0();
  osDelay(2);
  _ui_update_default_Ungroup_1();
  osDelay(2);
  _ui_update_default_Ungroup_2();
  osDelay(2);
  _ui_update_default_Ungroup_3();
  osDelay(2);
  _ui_update_default_Ungroup_5();
  osDelay(2);
}

void ui_supercap(float votage){
  float cnt=votage*votage;
  float percent=(cnt-49.0f)/480.0f;
  if(percent>=1)
    percent=1;
  if(percent<=0)
    percent = 0;
  if(percent >=0.6)
    ui_default_Ungroup_EnergyLine->color = 2;//绿色
  if(percent<=0.6)
    ui_default_Ungroup_EnergyLine->color = 3;//橙色
  if(percent <=0.3)
    ui_default_Ungroup_EnergyLine->color = 4;//紫红色
  ui_default_Ungroup_EnergyLine->end_x = ui_default_Ungroup_EnergyLine->start_x +795.0f*percent;
  
  if(Global.cap != FULL){
    ui_default_Ungroup_NewRect->color=6;
    ui_default_Ungroup_NewRect->width=1;
  }
  else{
    ui_default_Ungroup_NewRect->color=1;
    ui_default_Ungroup_NewRect->width=8;

  }
}


void char_change()
{
  //摩擦轮
  if(shoot.shoot_speed[1] < 6500)
	{
    ui_default_Ungroup_DropPoint->color=7;
  }
	else
	{
    ui_default_Ungroup_DropPoint->color=2;
  }

}




void ui_auto(uint8_t status){//自瞄相关ui
   switch (status)//自瞄圈
	 {
		 case 0:		 
      ui_default_Ungroup_AutoRound->color=4;//紫红色
			break;
		 case 1:
      ui_default_Ungroup_AutoRound->color=3;//橙色
			break;
		 case 2:
      ui_default_Ungroup_AutoRound->color=2;//绿色
			break;
			default:
      ui_default_Ungroup_AutoRound->color=4;//紫红色
			break;			
	 }	
	 
	 

	if((int32_t)fromNUC.target_id != 255)
	ui_default_Ungroup_AutoRobot->number=(int32_t)fromNUC.target_id;
	else
	ui_default_Ungroup_AutoRobot->number=0;
}




