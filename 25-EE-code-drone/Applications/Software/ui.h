//
// Created by RM UI Designer
//

#ifndef UI_H
#define UI_H
#ifdef __cplusplus
extern "C" {
#endif

#include "ui_interface.h"

#include "ui_default_Ungroup_0.h"
#include "ui_default_Ungroup_1.h"
#include "ui_default_Ungroup_2.h"
#include "ui_default_Ungroup_3.h"
#include "ui_default_Ungroup_4.h"
#include "ui_default_Ungroup_5.h"

void ui_init();
void ui_updata();
void ui_supercap(float votage);
void char_change();
void ui_chassis(float angle);
void ui_auto(uint8_t status);
void ui_chassisline();
void ui_WarningLight();
void ui_Spin();




#define ui_init_default_Ungroup() \
_ui_init_default_Ungroup_0(); \
_ui_init_default_Ungroup_1(); \
_ui_init_default_Ungroup_2(); \
_ui_init_default_Ungroup_3(); \
_ui_init_default_Ungroup_4(); \
_ui_init_default_Ungroup_5()

#define ui_update_default_Ungroup() \
_ui_update_default_Ungroup_0(); \
_ui_update_default_Ungroup_1(); \
_ui_update_default_Ungroup_2(); \
_ui_update_default_Ungroup_3(); \
_ui_update_default_Ungroup_4(); \
_ui_update_default_Ungroup_5()

#define ui_remove_default_Ungroup() \
_ui_remove_default_Ungroup_0(); \
_ui_remove_default_Ungroup_1(); \
_ui_remove_default_Ungroup_2(); \
_ui_remove_default_Ungroup_3(); \
_ui_remove_default_Ungroup_4(); \
_ui_remove_default_Ungroup_5()
    


#ifdef __cplusplus
}
#endif

#endif //UI_H
