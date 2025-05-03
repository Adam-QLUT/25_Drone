#include "Auto_control.h"
#include "Global_status.h"
#include "Gimbal.h"

#include "IMU_updata.h"

#include "CRC8_CRC16.h"
#include "string.h"

STM32_data_t toMINIPC;
MINIPC_data_t fromMINIPC;

uint8_t data[128];
uint8_t rx_data[100];

void decodeMINIPCdata(MINIPC_data_t *target, unsigned char buff[], unsigned int len)
{
    memcpy(target, buff , len);
    return 0;
}

int encodeSTM32(STM32_data_t *target, unsigned char tx_buff[], unsigned int len)
{
    memcpy(tx_buff, target, sizeof(STM32_data_t));
    return 0;
}

void STM32_to_MINIPC()
{
    toMINIPC.FrameHeader.sof = 0xA5;
    toMINIPC.FrameHeader.crc8 = 0x00;
    toMINIPC.To_minipc_data.curr_pitch = IMU_data.AHRS.pitch;
    toMINIPC.To_minipc_data.curr_yaw = IMU_data.AHRS.yaw;
    toMINIPC.To_minipc_data.curr_omega = cos(IMU_data.AHRS.pitch) * IMU_data.gyro[2] - sin(IMU_data.AHRS.pitch)*IMU_data.gyro[1];
    toMINIPC.To_minipc_data.autoaim = 1;
    toMINIPC.To_minipc_data.enemy_color = 1;
    toMINIPC.To_minipc_data.state = 0;
    toMINIPC.FrameTailer.crc16 = get_CRC16_check_sum((uint8_t *)&toMINIPC.FrameHeader.sof, 17, 0xffff);
    toMINIPC.enter = 0x0A;
    encodeSTM32(&toMINIPC, data, sizeof(STM32_data_t));
    VirCom_send(data, sizeof(STM32_data_t));
}

void MINIPC_to_STM32(){
    Global.Auto.input.shoot_pitch=fromMINIPC.from_minipc_data.shoot_pitch;
    Global.Auto.input.shoot_yaw=fromMINIPC.from_minipc_data.shoot_yaw;
    Global.Auto.input.fire=fromMINIPC.from_minipc_data.fire;
    Global.Auto.input.target_id=fromMINIPC.from_minipc_data.target_id;
}

void Auto_control(){
    if(Global.Auto.input.Auto_control_online==1){
    Gimbal_set_yaw_angle( (180.0 / 3.14159265358979323846)*(Global.Auto.input.shoot_yaw));
    Gimbal_set_pitch_angle( (180.0 / 3.14159265358979323846)*(Global.Auto.input.shoot_pitch));
    Global.Auto.input.Auto_control_online=0;
    }
}
