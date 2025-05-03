//
// Created by RM UI Designer
//

#include "ui_default_Ungroup_5.h"

#define FRAME_ID 0
#define GROUP_ID 0
#define START_ID 11
#define OBJ_NUM 5
#define FRAME_OBJ_NUM 5

CAT(ui_, CAT(FRAME_OBJ_NUM, _frame_t)) ui_default_Ungroup_5;
ui_interface_round_t *ui_default_Ungroup_DropPoint = (ui_interface_round_t *)&(ui_default_Ungroup_5.data[0]);
ui_interface_line_t *ui_default_Ungroup_DropLine = (ui_interface_line_t *)&(ui_default_Ungroup_5.data[1]);
ui_interface_round_t *ui_default_Ungroup_WarningLight = (ui_interface_round_t *)&(ui_default_Ungroup_5.data[2]);
ui_interface_number_t *ui_default_Ungroup_AutoRobot = (ui_interface_number_t *)&(ui_default_Ungroup_5.data[3]);
ui_interface_ellipse_t *ui_default_Ungroup_Reserved = (ui_interface_ellipse_t *)&(ui_default_Ungroup_5.data[4]);

void _ui_init_default_Ungroup_5() {
    for (int i = 0; i < OBJ_NUM; i++) {
        ui_default_Ungroup_5.data[i].figure_name[0] = FRAME_ID;
        ui_default_Ungroup_5.data[i].figure_name[1] = GROUP_ID;
        ui_default_Ungroup_5.data[i].figure_name[2] = i + START_ID;
        ui_default_Ungroup_5.data[i].operate_tpyel = 1;
    }
    for (int i = OBJ_NUM; i < FRAME_OBJ_NUM; i++) {
        ui_default_Ungroup_5.data[i].operate_tpyel = 0;
    }

		 ui_default_Ungroup_DropPoint->figure_tpye = 2;
    ui_default_Ungroup_DropPoint->layer = 4;
    ui_default_Ungroup_DropPoint->r = 10;
    ui_default_Ungroup_DropPoint->start_x = 970;//960
    ui_default_Ungroup_DropPoint->start_y = 495;
    ui_default_Ungroup_DropPoint->color = 0;
    ui_default_Ungroup_DropPoint->width = 2;

    ui_default_Ungroup_DropLine->figure_tpye = 0;
    ui_default_Ungroup_DropLine->layer = 4;
    ui_default_Ungroup_DropLine->start_x = 970;//960
    ui_default_Ungroup_DropLine->start_y = 541;
    ui_default_Ungroup_DropLine->end_x = 970;
    ui_default_Ungroup_DropLine->end_y = 400;
    ui_default_Ungroup_DropLine->color = 0;
    ui_default_Ungroup_DropLine->width = 3;

    ui_default_Ungroup_WarningLight->figure_tpye = 2;
    ui_default_Ungroup_WarningLight->layer = 6;
    ui_default_Ungroup_WarningLight->r = 9;
    ui_default_Ungroup_WarningLight->start_x = 127;
    ui_default_Ungroup_WarningLight->start_y = 788;
    ui_default_Ungroup_WarningLight->color = 0;
    ui_default_Ungroup_WarningLight->width = 20;

    ui_default_Ungroup_AutoRobot->figure_tpye = 6;
    ui_default_Ungroup_AutoRobot->layer = 5;
    ui_default_Ungroup_AutoRobot->font_size = 30;
    ui_default_Ungroup_AutoRobot->start_x = 1500;
    ui_default_Ungroup_AutoRobot->start_y = 658;
    ui_default_Ungroup_AutoRobot->color = 6;
    ui_default_Ungroup_AutoRobot->number = 0;
    ui_default_Ungroup_AutoRobot->width = 3;

    ui_default_Ungroup_Reserved->figure_tpye = 3;
    ui_default_Ungroup_Reserved->layer = 0;
    ui_default_Ungroup_Reserved->rx = 16;
    ui_default_Ungroup_Reserved->ry = 14;
    ui_default_Ungroup_Reserved->start_x = 1874;
    ui_default_Ungroup_Reserved->start_y = 865;
    ui_default_Ungroup_Reserved->color = 0;
    ui_default_Ungroup_Reserved->width = 1;


    CAT(ui_proc_, CAT(FRAME_OBJ_NUM, _frame))(&ui_default_Ungroup_5);
    SEND_MESSAGE((uint8_t *) &ui_default_Ungroup_5, sizeof(ui_default_Ungroup_5));
}

void _ui_update_default_Ungroup_5() {
    for (int i = 0; i < OBJ_NUM; i++) {
        ui_default_Ungroup_5.data[i].operate_tpyel = 2;
    }

    CAT(ui_proc_, CAT(FRAME_OBJ_NUM, _frame))(&ui_default_Ungroup_5);
    SEND_MESSAGE((uint8_t *) &ui_default_Ungroup_5, sizeof(ui_default_Ungroup_5));
}

void _ui_remove_default_Ungroup_5() {
    for (int i = 0; i < OBJ_NUM; i++) {
        ui_default_Ungroup_5.data[i].operate_tpyel = 3;
    }

    CAT(ui_proc_, CAT(FRAME_OBJ_NUM, _frame))(&ui_default_Ungroup_5);
    SEND_MESSAGE((uint8_t *) &ui_default_Ungroup_5, sizeof(ui_default_Ungroup_5));
}
