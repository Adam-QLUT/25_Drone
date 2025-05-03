//
// Created by RM UI Designer
//

#include "ui_default_Ungroup_4.h"

#define FRAME_ID 0
#define GROUP_ID 0
#define START_ID 4
#define OBJ_NUM 7
#define FRAME_OBJ_NUM 7

CAT(ui_, CAT(FRAME_OBJ_NUM, _frame_t)) ui_default_Ungroup_4;
ui_interface_arc_t *ui_default_Ungroup_Chassis = (ui_interface_arc_t *)&(ui_default_Ungroup_4.data[0]);
ui_interface_rect_t *ui_default_Ungroup_NewRect = (ui_interface_rect_t *)&(ui_default_Ungroup_4.data[1]);
ui_interface_rect_t *ui_default_Ungroup_EnergyLine = (ui_interface_rect_t *)&(ui_default_Ungroup_4.data[2]);
ui_interface_line_t *ui_default_Ungroup_MoveLineleft = (ui_interface_line_t *)&(ui_default_Ungroup_4.data[3]);
ui_interface_line_t *ui_default_Ungroup_MoveLineright = (ui_interface_line_t *)&(ui_default_Ungroup_4.data[4]);
ui_interface_rect_t *ui_default_Ungroup_AutoRect = (ui_interface_rect_t *)&(ui_default_Ungroup_4.data[5]);
ui_interface_round_t *ui_default_Ungroup_AutoRound = (ui_interface_round_t *)&(ui_default_Ungroup_4.data[6]);

void _ui_init_default_Ungroup_4() {
    for (int i = 0; i < OBJ_NUM; i++) {
        ui_default_Ungroup_4.data[i].figure_name[0] = FRAME_ID;
        ui_default_Ungroup_4.data[i].figure_name[1] = GROUP_ID;
        ui_default_Ungroup_4.data[i].figure_name[2] = i + START_ID;
        ui_default_Ungroup_4.data[i].operate_tpyel = 1;
    }
    for (int i = OBJ_NUM; i < FRAME_OBJ_NUM; i++) {
        ui_default_Ungroup_4.data[i].operate_tpyel = 0;
    }

    ui_default_Ungroup_Chassis->figure_tpye = 4;
    ui_default_Ungroup_Chassis->layer = 0;
    ui_default_Ungroup_Chassis->rx = 360;
    ui_default_Ungroup_Chassis->ry = 360;
    ui_default_Ungroup_Chassis->start_x = 960;
    ui_default_Ungroup_Chassis->start_y = 521;
    ui_default_Ungroup_Chassis->color = 1;
    ui_default_Ungroup_Chassis->width = 8;
    ui_default_Ungroup_Chassis->start_angle = 160;
    ui_default_Ungroup_Chassis->end_angle = 200;

    ui_default_Ungroup_NewRect->figure_tpye = 1;
    ui_default_Ungroup_NewRect->layer = 1;
    ui_default_Ungroup_NewRect->start_x = 600;
    ui_default_Ungroup_NewRect->start_y = 110;
    ui_default_Ungroup_NewRect->color = 6;
    ui_default_Ungroup_NewRect->width = 1;
    ui_default_Ungroup_NewRect->end_x = 1430;
    ui_default_Ungroup_NewRect->end_y = 154;

    ui_default_Ungroup_EnergyLine->figure_tpye = 1;
    ui_default_Ungroup_EnergyLine->layer = 1;
    ui_default_Ungroup_EnergyLine->start_x = 620;
    ui_default_Ungroup_EnergyLine->start_y = 120;
    ui_default_Ungroup_EnergyLine->color = 2;
    ui_default_Ungroup_EnergyLine->width = 15;
    ui_default_Ungroup_EnergyLine->end_x = 1400;
    ui_default_Ungroup_EnergyLine->end_y = 135;

    ui_default_Ungroup_MoveLineleft->figure_tpye = 0;
    ui_default_Ungroup_MoveLineleft->layer = 2;
    ui_default_Ungroup_MoveLineleft->start_x = 600;
    ui_default_Ungroup_MoveLineleft->start_y = 0;
    ui_default_Ungroup_MoveLineleft->end_x = 811;
    ui_default_Ungroup_MoveLineleft->end_y = 409;
    ui_default_Ungroup_MoveLineleft->color = 6;
    ui_default_Ungroup_MoveLineleft->width = 3;

    ui_default_Ungroup_MoveLineright->figure_tpye = 0;
    ui_default_Ungroup_MoveLineright->layer = 2;
    ui_default_Ungroup_MoveLineright->start_x = 1321;
    ui_default_Ungroup_MoveLineright->start_y = 0;
    ui_default_Ungroup_MoveLineright->end_x = 1110;
    ui_default_Ungroup_MoveLineright->end_y = 409;
    ui_default_Ungroup_MoveLineright->color = 6;
    ui_default_Ungroup_MoveLineright->width = 3;

    ui_default_Ungroup_AutoRect->figure_tpye = 1;
    ui_default_Ungroup_AutoRect->layer = 3;
    ui_default_Ungroup_AutoRect->start_x = 608;
    ui_default_Ungroup_AutoRect->start_y = 313;
    ui_default_Ungroup_AutoRect->color = 8;
    ui_default_Ungroup_AutoRect->width = 3;
    ui_default_Ungroup_AutoRect->end_x = 1328;
    ui_default_Ungroup_AutoRect->end_y = 793;

    ui_default_Ungroup_AutoRound->figure_tpye = 2;
    ui_default_Ungroup_AutoRound->layer = 3;
    ui_default_Ungroup_AutoRound->r = 14;
    ui_default_Ungroup_AutoRound->start_x = 960;
    ui_default_Ungroup_AutoRound->start_y = 540;
    ui_default_Ungroup_AutoRound->color = 4;
    ui_default_Ungroup_AutoRound->width = 2;


    CAT(ui_proc_, CAT(FRAME_OBJ_NUM, _frame))(&ui_default_Ungroup_4);
    SEND_MESSAGE((uint8_t *) &ui_default_Ungroup_4, sizeof(ui_default_Ungroup_4));
}

void _ui_update_default_Ungroup_4() {
    for (int i = 0; i < OBJ_NUM; i++) {
        ui_default_Ungroup_4.data[i].operate_tpyel = 2;
    }

    CAT(ui_proc_, CAT(FRAME_OBJ_NUM, _frame))(&ui_default_Ungroup_4);
    SEND_MESSAGE((uint8_t *) &ui_default_Ungroup_4, sizeof(ui_default_Ungroup_4));
}

void _ui_remove_default_Ungroup_4() {
    for (int i = 0; i < OBJ_NUM; i++) {
        ui_default_Ungroup_4.data[i].operate_tpyel = 3;
    }

    CAT(ui_proc_, CAT(FRAME_OBJ_NUM, _frame))(&ui_default_Ungroup_4);
    SEND_MESSAGE((uint8_t *) &ui_default_Ungroup_4, sizeof(ui_default_Ungroup_4));
}
