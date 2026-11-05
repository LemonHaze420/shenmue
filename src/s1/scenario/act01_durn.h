#pragma once

#include "scenario.h"
#include "motion/motion.h"

static void script_0030(int32_t arg0, float arg1, float arg2, float arg3);
static void script_0058(int32_t arg0, float arg1, float arg2, float arg3);
static void script_00AC(int32_t arg0, int32_t arg1, int32_t arg2);
static int32_t script_00FC(int32_t arg0);
static void script_01A3(int32_t arg0, int32_t arg1);
static int32_t script_01C6(int32_t arg0);
static void script_01F9(int32_t arg0, int32_t arg1);
static void script_021C(int32_t arg0, int32_t arg1);
static void script_027B(int32_t arg0);
static int32_t script_0290(int32_t arg0);
static void script_02AE(float arg0);
static int32_t script_02CD(int32_t arg0, int32_t arg1);
static void script_0345(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4);
static int32_t script_08F7(int32_t arg0);
static void script_09C8(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_09D8(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_0ADC(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_0C7E(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_0DDF(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_0F40(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_11DE(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_12AC(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_1313(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_137A(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_13C1(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_14F7(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_1DB8(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_1ED6(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_259B(int32_t arg0, int32_t arg1);
static void script_2671(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_3584(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_35F0(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_3AAA(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_4164(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_5A44(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_6165(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_68EF(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_69C7(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_6E3E(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_7DD1(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_821B(int32_t arg0, DSWKDoorEntry *arg1, int32_t *arg2, DoorResourceDescriptor *arg3, MapObjectRecord *arg4, int32_t arg5, int32_t arg6);
static void script_83A7(DSWKDoorEntry *arg0, int32_t *arg1, DoorResourceDescriptor *arg2, MapObjectRecord *arg3, int32_t arg4);
static void script_83DB(void);
static void script_8444(int32_t arg0, int32_t arg1);
static void script_84E1(int32_t arg0, int32_t arg1);
static void script_851B(void);
static void script_857E(void);
static void script_85E1(int32_t arg0);
static void script_8618(int32_t arg0);
static void script_864F(int32_t arg0);
static void script_8891(int32_t arg0);
static void script_88DA(int32_t arg0);
static void script_8923(int32_t arg0, int32_t arg1);
static void script_8999(int32_t arg0);
static void script_89A7(int32_t arg0);
static void script_89B5(int32_t arg0);
static void script_8A7C(void);
static void script_8A88(void);
static void script_8A94(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5, int32_t arg6);
static int32_t script_8AD7(void);
static int8_t script_8BE8(int32_t arg0, int32_t arg1, int8_t arg2);
static void script_8C80(int32_t arg0);
static void script_8E4F(int32_t arg0);
static int32_t script_8E80(int32_t arg0, int32_t arg1, int32_t arg2, float arg3, float arg4, int32_t arg5, int32_t arg6);
static void script_9549(int32_t arg0, int32_t arg1);
static void script_957B(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_96EB(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_98A8(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_9A5C(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_9C72(int32_t arg0, int32_t arg1);
static void script_9D3C(int32_t arg0, int32_t arg1);
static void script_9D9D(int32_t arg0);
static void script_9F42(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_9FDA(int32_t arg0, int32_t arg1);
static void script_A0BC(int32_t arg0);
static void script_A113(void);
static void script_A24B(int32_t arg0);
static void script_A257(int32_t arg0);
static void script_A263(int32_t arg0);
static void script_A35F(int32_t arg0);
static void script_A376(int32_t arg0);
static void script_A3B7(int32_t arg0);
static void script_A3FB(int32_t arg0);
static void script_A479(void);
static void script_A4F8(int32_t arg0);
static int32_t script_A524(int32_t arg0, int32_t arg1);
static int8_t script_A557(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_A699(int32_t arg0, int32_t arg1);
static void script_A862(int32_t arg0, int32_t arg1);
static void script_AA38(void);
static int32_t script_AAE1(int32_t arg0, int32_t arg1);
static void script_AB07(int32_t arg0);
static int16_t script_B455(void);
static void script_BA5A(int32_t arg0);
static int16_t script_BECE(void);
static void script_C13A(int32_t arg0);
static void script_C2E4(int32_t arg0);
static int32_t script_C324(void);
static int32_t script_C3A1(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_C3DD(void);
static int32_t script_C43E(void);
static int32_t script_C55C(int32_t arg0, int32_t arg1);
static void script_C5E9(int32_t arg0, int32_t arg1);
static void script_C64E(int32_t arg0);
static void script_C730(void);
static void script_C97F(void);
static void script_CB8C(int32_t arg0);
static int8_t script_CBB7(void);
static int32_t script_CCE6(void);
static void script_CD16(void);
static void script_CD98(void);
static void script_CE0A(void);
static void script_CF69(void);
static void script_CFE2(int32_t arg0);
static void script_D0B9(void);
static void script_D0F8(void);
static void script_D136(void);
static void script_D174(void);
static int32_t script_D1B2(int32_t arg0);
static int32_t script_D205(int32_t arg0);
static void script_D25B(void);
static void script_D39C(void);
static void script_D3BD(void);
static void script_D442(void);
static void script_D48B(void);
static void script_D4CA(void);
static void script_D4D7(void);
static void script_D4E4(void);
static void script_D4F1(void);
static void script_DC9D(void);
static void script_DCD1(int32_t arg0, int32_t arg1);
static void script_DD3D(int32_t arg0, int32_t arg1);
static void script_DE78(int32_t arg0);
static void script_E094(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_E42A(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_EAB8(void);
static void script_EAC7(void);
static void script_EAD6(void);
static void script_EAD9(void);
static void script_EADC(void);
static void script_EADF(void);
static void script_EAE2(void);
static void script_EAEF(void);
static void script_EAFB(void);
static void script_EB0A(void);
void script_EB19(void);
static int32_t script_EDB7(void);

static int32_t task_0;
static int32_t task_4;
static int32_t task_8;
static int32_t task_C;
static int32_t task_10;
static int32_t task_14;
static int32_t task_18;
static int32_t task_1C;
static int32_t task_20;
static int32_t task_24;
static int32_t task_28;
static int32_t task_2C;
static int32_t task_30;
static int32_t task_34;
static int32_t task_38;
static int32_t task_3C;
static int32_t task_40;
static int32_t task_44;
static int32_t task_48;
static int32_t task_4C;
static int32_t task_50;
static int32_t task_54;
static int32_t task_58;
static int32_t task_5C;
static int32_t task_60;
static int32_t task_64;
static int32_t task_68;
static DSWKDoorEntry *task_6C;
static int32_t *task_70;
static DoorResourceDescriptor *task_74;
static MapObjectRecord *task_78;
static DoorSlotState task_7C;
static int32_t task_88;
static int32_t task_8C[10];
static int32_t task_B4[10];
static int32_t task_DC[10];
static int32_t task_104[10];
static int32_t task_12C[10];
static int32_t task_154[10];
static int32_t task_17C[10];
static int32_t task_1A4[10];
static int32_t task_1CC;
static int32_t task_1D0;
static Vector3 task_1D4;
static int32_t task_1E0;
static int32_t task_1E4;
static int32_t task_1E8;
static int8_t task_1EC;
static uint8_t task_1ED;
static uint16_t task_1EE;
static int8_t task_1F0;
static uint8_t task_1F1;
static uint16_t task_1F2;
static int32_t task_1F4;
static int32_t task_1F8;
static int8_t task_1FC;
static uint8_t task_1FD;
static uint16_t task_1FE;
static Vector3 task_200;
static int8_t task_20C;
static int8_t task_20D;
static uint8_t task_20E;
static int8_t task_20F;
static int16_t task_210;
static int8_t task_212;
static int8_t task_213;
static int8_t task_214;
static uint8_t task_215;
static int16_t task_216;
static int32_t task_218[2];
static int32_t task_220[2];
static float task_228;
static float task_22C;
static Vector3 task_230;
static int16_t task_23C;
static uint16_t task_23E;
static int32_t task_240;
static int32_t task_244;
static int32_t task_248;
static int8_t task_24C;
static uint8_t task_24D;
static uint16_t task_24E;
static const Vector3 data_F15C = {.x = 1.0f, .y = 1.0f, .z = 1.0f};
static const int32_t data_F168[4] = {16777215, 16777215, 16777215, 16777215};
static const float data_F178[3] = {20.0f, 800.0f, 1.0f};
static const int32_t data_F184 = -1;
static DoorMotionSelectorBlock data_F188[38] = {
  {.motion_desc_index = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
  {.motion_desc_index = {7, 6, 6, 7, 15, 14, 14, 14, 21, 20, 20, 20, 27, 26, 26, 26, 33, 32, 32, 32, 39, 38, 38, 38, 45, 44, 44, 44, 51, 50, 50, 50, 57, 56, 56, 56, 63, 62, 62, 62}},
  {.motion_desc_index = {7, 6, 6, 7, 16, 13, 13, 13, 22, 19, 19, 19, 28, 25, 25, 25, 34, 31, 31, 31, 40, 37, 37, 37, 46, 43, 43, 43, 52, 49, 49, 49, 58, 55, 55, 55, 64, 61, 61, 61}},
  {.motion_desc_index = {6, 7, 7, 6, 13, 16, 16, 16, 19, 22, 22, 22, 25, 28, 28, 28, 31, 34, 34, 34, 37, 40, 40, 40, 43, 46, 46, 46, 49, 52, 52, 52, 55, 58, 58, 58, 61, 64, 64, 64}},
  {.motion_desc_index = {6, 7, 7, 6, 14, 15, 15, 15, 20, 21, 21, 21, 26, 27, 27, 27, 32, 33, 33, 33, 38, 39, 39, 39, 44, 45, 45, 45, 50, 51, 51, 51, 56, 57, 57, 57, 62, 63, 63, 63}},
  {.motion_desc_index = {6, 7, 7, 6, 13, 16, 16, 16, 19, 22, 22, 22, 25, 28, 28, 28, 31, 34, 34, 34, 37, 40, 40, 40, 43, 46, 46, 46, 49, 52, 52, 52, 55, 58, 58, 58, 61, 64, 64, 64}},
  {.motion_desc_index = {6, 7, 7, 6, 14, 15, 15, 15, 20, 21, 21, 21, 26, 27, 27, 27, 32, 33, 33, 33, 38, 39, 39, 39, 44, 45, 45, 45, 50, 51, 51, 51, 56, 57, 57, 57, 62, 63, 63, 63}},
  {.motion_desc_index = {7, 6, 6, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
  {.motion_desc_index = {7, 6, 6, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
  {.motion_desc_index = {1, 1, 1, 1, 11, 12, 12, 12, 17, 18, 18, 18, 23, 24, 24, 24, 29, 30, 30, 30, 35, 36, 36, 36, 41, 42, 42, 42, 47, 48, 48, 48, 53, 54, 54, 54, 59, 60, 60, 60}},
  {.motion_desc_index = {1, 1, 1, 1, 12, 11, 11, 11, 18, 17, 17, 17, 24, 23, 23, 23, 30, 29, 29, 29, 36, 35, 35, 35, 42, 41, 41, 41, 48, 47, 47, 47, 54, 53, 53, 53, 60, 59, 59, 59}},
  {.motion_desc_index = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
  {.motion_desc_index = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
  {.motion_desc_index = {6, 7, 7, 6, 13, 16, 15, 14, 19, 22, 21, 20, 25, 28, 27, 26, 31, 34, 33, 32, 37, 40, 39, 38, 43, 46, 45, 44, 49, 52, 51, 50, 55, 58, 57, 56, 61, 64, 63, 62}},
  {.motion_desc_index = {6, 7, 7, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
  {.motion_desc_index = {1, 1, 1, 1, 12, 11, 12, 11, 18, 17, 18, 17, 24, 23, 24, 23, 30, 29, 30, 29, 36, 35, 36, 35, 42, 41, 42, 41, 48, 47, 48, 47, 54, 53, 54, 53, 60, 59, 60, 59}},
  {.motion_desc_index = {0, 0, 0, 0, 11, 12, 11, 12, 17, 18, 17, 18, 23, 24, 23, 24, 29, 30, 29, 30, 35, 36, 35, 36, 41, 42, 41, 42, 47, 48, 47, 48, 53, 54, 53, 54, 59, 60, 59, 60}},
  {.motion_desc_index = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
  {.motion_desc_index = {2, 2, 2, 2, 11, 12, 12, 12, 17, 18, 18, 18, 23, 24, 24, 24, 29, 30, 30, 30, 35, 36, 36, 36, 41, 42, 42, 42, 47, 48, 48, 48, 53, 54, 54, 54, 59, 60, 60, 60}},
  {.motion_desc_index = {2, 2, 2, 2, 12, 11, 11, 11, 18, 17, 17, 17, 24, 23, 23, 23, 30, 29, 29, 29, 36, 35, 35, 35, 42, 41, 41, 41, 48, 47, 47, 47, 54, 53, 53, 53, 60, 59, 59, 59}},
  {.motion_desc_index = {2, 2, 2, 2, 12, 11, 12, 11, 18, 17, 18, 17, 24, 23, 24, 23, 30, 29, 30, 29, 36, 35, 36, 35, 42, 41, 42, 41, 48, 47, 48, 47, 54, 53, 54, 53, 60, 59, 60, 59}},
  {.motion_desc_index = {3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
  {.motion_desc_index = {3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
  {.motion_desc_index = {3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
  {.motion_desc_index = {4, 5, 5, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
  {.motion_desc_index = {4, 5, 5, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
  {.motion_desc_index = {5, 4, 4, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
  {.motion_desc_index = {5, 4, 4, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
  {.motion_desc_index = {8, 8, 8, 8, 13, 16, 15, 14, 19, 22, 21, 20, 25, 28, 27, 26, 31, 34, 33, 32, 37, 40, 39, 38, 43, 46, 45, 44, 49, 52, 51, 50, 55, 58, 57, 56, 61, 64, 63, 62}},
  {.motion_desc_index = {9, 10, 10, 9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
  {.motion_desc_index = {7, 6, 6, 7, 15, 14, 14, 14, 21, 20, 20, 20, 27, 26, 26, 26, 33, 32, 32, 32, 39, 38, 38, 38, 45, 44, 44, 44, 51, 50, 50, 50, 57, 56, 56, 56, 63, 62, 62, 62}},
  {.motion_desc_index = {7, 6, 6, 7, 16, 13, 13, 13, 22, 19, 19, 19, 28, 25, 25, 25, 34, 31, 31, 31, 40, 37, 37, 37, 46, 43, 43, 43, 52, 49, 49, 49, 58, 55, 55, 55, 64, 61, 61, 61}},
  {.motion_desc_index = {6, 7, 7, 6, 13, 16, 16, 16, 19, 22, 22, 22, 25, 28, 28, 28, 31, 34, 34, 34, 37, 40, 40, 40, 43, 46, 46, 46, 49, 52, 52, 52, 55, 58, 58, 58, 61, 64, 64, 64}},
  {.motion_desc_index = {6, 7, 7, 6, 14, 15, 15, 15, 20, 21, 21, 21, 26, 27, 27, 27, 32, 33, 33, 33, 38, 39, 39, 39, 44, 45, 45, 45, 50, 51, 51, 51, 56, 57, 57, 57, 62, 63, 63, 63}},
  {.motion_desc_index = {6, 7, 7, 6, 13, 16, 16, 16, 19, 22, 22, 22, 25, 28, 28, 28, 31, 34, 34, 34, 37, 40, 40, 40, 43, 46, 46, 46, 49, 52, 52, 52, 55, 58, 58, 58, 61, 64, 64, 64}},
  {.motion_desc_index = {6, 7, 7, 6, 14, 15, 15, 15, 20, 21, 21, 21, 26, 27, 27, 27, 32, 33, 33, 33, 38, 39, 39, 39, 44, 45, 45, 45, 50, 51, 51, 51, 56, 57, 57, 57, 62, 63, 63, 63}},
  {.motion_desc_index = {7, 6, 6, 7, 15, 14, 14, 14, 21, 20, 20, 20, 65, 26, 26, 26, 33, 32, 32, 32, 66, 38, 38, 38, 67, 44, 44, 44, 51, 50, 50, 50, 68, 56, 56, 56, 69, 62, 62, 62}},
  {.motion_desc_index = {7, 6, 6, 7, 15, 14, 14, 14, 21, 70, 20, 20, 65, 26, 26, 26, 33, 71, 32, 32, 66, 38, 38, 38, 67, 44, 44, 44, 51, 50, 50, 50, 68, 56, 56, 56, 69, 62, 62, 62}}
};
static ScnDoorMotionDescriptor data_F778[72] = {
  {.motion_id = -1, .frame_open_sfx_begin = 0.0f, .frame_phase1_end = 0.0f, .frame_close_sfx_begin = 0.0f, .frame_motion_end = 0.0f, .approach_off_x = 0.0f, .approach_off_z = 0.0f, .npc_tail_end_frame = 0.0f, .npc_tail_delta_per_frame = 0.0f},
  {.motion_id = 631, .frame_open_sfx_begin = 19.0f, .frame_phase1_end = 52.0f, .frame_close_sfx_begin = 96.0f, .frame_motion_end = 132.0f, .approach_off_x = 45.0f, .approach_off_z = 50.0f, .npc_tail_end_frame = 0.0f, .npc_tail_delta_per_frame = 0.0f},
  {.motion_id = 637, .frame_open_sfx_begin = 19.0f, .frame_phase1_end = 53.0f, .frame_close_sfx_begin = 0.0f, .frame_motion_end = 0.0f, .approach_off_x = 45.0f, .approach_off_z = 50.0f, .npc_tail_end_frame = 0.0f, .npc_tail_delta_per_frame = 0.0f},
  {.motion_id = 636, .frame_open_sfx_begin = 30.0f, .frame_phase1_end = 64.0f, .frame_close_sfx_begin = 0.0f, .frame_motion_end = 0.0f, .approach_off_x = 45.0f, .approach_off_z = 50.0f, .npc_tail_end_frame = 0.0f, .npc_tail_delta_per_frame = 0.0f},
  {.motion_id = 628, .frame_open_sfx_begin = 19.0f, .frame_phase1_end = 47.0f, .frame_close_sfx_begin = 0.0f, .frame_motion_end = 0.0f, .approach_off_x = 45.0f, .approach_off_z = 80.0f, .npc_tail_end_frame = 0.0f, .npc_tail_delta_per_frame = 0.0f},
  {.motion_id = 54, .frame_open_sfx_begin = 33.0f, .frame_phase1_end = 70.0f, .frame_close_sfx_begin = 0.0f, .frame_motion_end = 0.0f, .approach_off_x = 50.0f, .approach_off_z = 80.0f, .npc_tail_end_frame = 0.0f, .npc_tail_delta_per_frame = 0.0f},
  {.motion_id = 646, .frame_open_sfx_begin = 19.0f, .frame_phase1_end = 45.0f, .frame_close_sfx_begin = 106.0f, .frame_motion_end = 141.0f, .approach_off_x = 45.0f, .approach_off_z = 80.0f, .npc_tail_end_frame = 0.0f, .npc_tail_delta_per_frame = 0.0f},
  {.motion_id = 649, .frame_open_sfx_begin = 30.0f, .frame_phase1_end = 73.0f, .frame_close_sfx_begin = 108.0f, .frame_motion_end = 135.0f, .approach_off_x = 45.0f, .approach_off_z = 50.0f, .npc_tail_end_frame = 0.0f, .npc_tail_delta_per_frame = 0.0f},
  {.motion_id = 1365, .frame_open_sfx_begin = 30.0f, .frame_phase1_end = 73.0f, .frame_close_sfx_begin = 0.0f, .frame_motion_end = 0.0f, .approach_off_x = 0.0f, .approach_off_z = 89.5f, .npc_tail_end_frame = 0.0f, .npc_tail_delta_per_frame = 0.0f},
  {.motion_id = 1548, .frame_open_sfx_begin = 34.0f, .frame_phase1_end = 88.0f, .frame_close_sfx_begin = 125.0f, .frame_motion_end = 185.0f, .approach_off_x = 63.264f, .approach_off_z = 73.183998f, .npc_tail_end_frame = 0.0f, .npc_tail_delta_per_frame = 0.0f},
  {.motion_id = 1549, .frame_open_sfx_begin = 28.0f, .frame_phase1_end = 73.0f, .frame_close_sfx_begin = 108.0f, .frame_motion_end = 163.0f, .approach_off_x = 58.0f, .approach_off_z = 71.2f, .npc_tail_end_frame = 0.0f, .npc_tail_delta_per_frame = 0.0f},
  {.motion_id = 40731, .frame_open_sfx_begin = 21.0f, .frame_phase1_end = 45.0f, .frame_close_sfx_begin = 95.0f, .frame_motion_end = 125.0f, .approach_off_x = 45.0f, .approach_off_z = 56.0f, .npc_tail_end_frame = 51.0f, .npc_tail_delta_per_frame = 0.025f},
  {.motion_id = 40732, .frame_open_sfx_begin = 20.0f, .frame_phase1_end = 43.0f, .frame_close_sfx_begin = 107.0f, .frame_motion_end = 135.0f, .approach_off_x = 45.0f, .approach_off_z = 56.5f, .npc_tail_end_frame = 48.0f, .npc_tail_delta_per_frame = 0.04f},
  {.motion_id = 40705, .frame_open_sfx_begin = 21.0f, .frame_phase1_end = 35.0f, .frame_close_sfx_begin = 102.0f, .frame_motion_end = 126.0f, .approach_off_x = 48.08f, .approach_off_z = 83.538002f, .npc_tail_end_frame = 65.0f, .npc_tail_delta_per_frame = 1.7136f},
  {.motion_id = 40706, .frame_open_sfx_begin = 41.0f, .frame_phase1_end = 61.0f, .frame_close_sfx_begin = 140.0f, .frame_motion_end = 182.0f, .approach_off_x = 48.978001f, .approach_off_z = 82.223f, .npc_tail_end_frame = 93.0f, .npc_tail_delta_per_frame = 2.4333f},
  {.motion_id = 40733, .frame_open_sfx_begin = 33.0f, .frame_phase1_end = 59.0f, .frame_close_sfx_begin = 105.0f, .frame_motion_end = 136.0f, .approach_off_x = 34.54f, .approach_off_z = 86.077003f, .npc_tail_end_frame = 64.0f, .npc_tail_delta_per_frame = 1.7972f},
  {.motion_id = 40707, .frame_open_sfx_begin = 48.0f, .frame_phase1_end = 88.0f, .frame_close_sfx_begin = 134.0f, .frame_motion_end = 193.0f, .approach_off_x = 50.158001f, .approach_off_z = 51.297001f, .npc_tail_end_frame = 107.0f, .npc_tail_delta_per_frame = 1.206f},
  {.motion_id = 40734, .frame_open_sfx_begin = 22.0f, .frame_phase1_end = 45.0f, .frame_close_sfx_begin = 100.0f, .frame_motion_end = 130.0f, .approach_off_x = 60.0f, .approach_off_z = 55.0f, .npc_tail_end_frame = 52.0f, .npc_tail_delta_per_frame = 0.0183f},
  {.motion_id = 40735, .frame_open_sfx_begin = 20.0f, .frame_phase1_end = 37.0f, .frame_close_sfx_begin = 114.0f, .frame_motion_end = 136.0f, .approach_off_x = 68.5f, .approach_off_z = 60.0f, .npc_tail_end_frame = 42.0f, .npc_tail_delta_per_frame = 0.0798f},
  {.motion_id = 40708, .frame_open_sfx_begin = 23.0f, .frame_phase1_end = 44.0f, .frame_close_sfx_begin = 118.0f, .frame_motion_end = 149.0f, .approach_off_x = 81.0f, .approach_off_z = 58.0f, .npc_tail_end_frame = 64.0f, .npc_tail_delta_per_frame = 1.2643f},
  {.motion_id = 40709, .frame_open_sfx_begin = 51.0f, .frame_phase1_end = 64.0f, .frame_close_sfx_begin = 140.0f, .frame_motion_end = 175.0f, .approach_off_x = 76.0f, .approach_off_z = 80.0f, .npc_tail_end_frame = 101.0f, .npc_tail_delta_per_frame = 1.7926f},
  {.motion_id = 40736, .frame_open_sfx_begin = 35.0f, .frame_phase1_end = 70.0f, .frame_close_sfx_begin = 133.0f, .frame_motion_end = 169.0f, .approach_off_x = 62.75f, .approach_off_z = 59.0f, .npc_tail_end_frame = 70.0f, .npc_tail_delta_per_frame = 1.9468f},
  {.motion_id = 40710, .frame_open_sfx_begin = 33.0f, .frame_phase1_end = 58.0f, .frame_close_sfx_begin = 96.0f, .frame_motion_end = 142.0f, .approach_off_x = 55.0f, .approach_off_z = 69.0f, .npc_tail_end_frame = 79.0f, .npc_tail_delta_per_frame = 2.28709984f},
  {.motion_id = 40737, .frame_open_sfx_begin = 24.0f, .frame_phase1_end = 51.0f, .frame_close_sfx_begin = 89.0f, .frame_motion_end = 123.0f, .approach_off_x = 54.0f, .approach_off_z = 56.5f, .npc_tail_end_frame = 58.0f, .npc_tail_delta_per_frame = 0.0249f},
  {.motion_id = 40738, .frame_open_sfx_begin = 25.0f, .frame_phase1_end = 43.0f, .frame_close_sfx_begin = 94.0f, .frame_motion_end = 123.0f, .approach_off_x = 48.3f, .approach_off_z = 58.0f, .npc_tail_end_frame = 54.0f, .npc_tail_delta_per_frame = 0.0236f},
  {.motion_id = 40711, .frame_open_sfx_begin = 20.0f, .frame_phase1_end = 38.0f, .frame_close_sfx_begin = 95.0f, .frame_motion_end = 125.0f, .approach_off_x = 47.703999f, .approach_off_z = 76.788002f, .npc_tail_end_frame = 60.0f, .npc_tail_delta_per_frame = 1.9167f},
  {.motion_id = 40712, .frame_open_sfx_begin = 45.0f, .frame_phase1_end = 71.0f, .frame_close_sfx_begin = 165.0f, .frame_motion_end = 217.0f, .approach_off_x = 53.195f, .approach_off_z = 88.927002f, .npc_tail_end_frame = 97.0f, .npc_tail_delta_per_frame = 1.4981f},
  {.motion_id = 40739, .frame_open_sfx_begin = 46.0f, .frame_phase1_end = 95.0f, .frame_close_sfx_begin = 150.0f, .frame_motion_end = 200.0f, .approach_off_x = 39.535f, .approach_off_z = 89.264999f, .npc_tail_end_frame = 99.0f, .npc_tail_delta_per_frame = 0.5833f},
  {.motion_id = 40713, .frame_open_sfx_begin = 34.0f, .frame_phase1_end = 110.0f, .frame_close_sfx_begin = 167.0f, .frame_motion_end = 202.0f, .approach_off_x = 48.733002f, .approach_off_z = 59.613998f, .npc_tail_end_frame = 130.0f, .npc_tail_delta_per_frame = 0.75f},
  {.motion_id = 40740, .frame_open_sfx_begin = 22.0f, .frame_phase1_end = 41.0f, .frame_close_sfx_begin = 85.0f, .frame_motion_end = 110.0f, .approach_off_x = 63.0f, .approach_off_z = 47.0f, .npc_tail_end_frame = 47.0f, .npc_tail_delta_per_frame = 0.028f},
  {.motion_id = 40741, .frame_open_sfx_begin = 23.0f, .frame_phase1_end = 41.0f, .frame_close_sfx_begin = 90.0f, .frame_motion_end = 114.0f, .approach_off_x = 73.0f, .approach_off_z = 46.0f, .npc_tail_end_frame = 47.0f, .npc_tail_delta_per_frame = 0.0289f},
  {.motion_id = 40714, .frame_open_sfx_begin = 22.0f, .frame_phase1_end = 36.0f, .frame_close_sfx_begin = 105.0f, .frame_motion_end = 132.0f, .approach_off_x = 67.096001f, .approach_off_z = 56.924f, .npc_tail_end_frame = 59.0f, .npc_tail_delta_per_frame = 1.6836f},
  {.motion_id = 40715, .frame_open_sfx_begin = 45.0f, .frame_phase1_end = 59.0f, .frame_close_sfx_begin = 140.0f, .frame_motion_end = 180.0f, .approach_off_x = 91.0f, .approach_off_z = 60.0f, .npc_tail_end_frame = 95.0f, .npc_tail_delta_per_frame = 1.8126f},
  {.motion_id = 40742, .frame_open_sfx_begin = 23.0f, .frame_phase1_end = 67.0f, .frame_close_sfx_begin = 105.0f, .frame_motion_end = 162.0f, .approach_off_x = 50.1f, .approach_off_z = 54.112f, .npc_tail_end_frame = 80.0f, .npc_tail_delta_per_frame = 0.3366f},
  {.motion_id = 40716, .frame_open_sfx_begin = 32.0f, .frame_phase1_end = 60.0f, .frame_close_sfx_begin = 96.0f, .frame_motion_end = 150.0f, .approach_off_x = 62.0f, .approach_off_z = 63.0f, .npc_tail_end_frame = 86.0f, .npc_tail_delta_per_frame = 1.39560008f},
  {.motion_id = 40743, .frame_open_sfx_begin = 21.0f, .frame_phase1_end = 48.0f, .frame_close_sfx_begin = 80.0f, .frame_motion_end = 113.0f, .approach_off_x = 49.0f, .approach_off_z = 57.5f, .npc_tail_end_frame = 54.0f, .npc_tail_delta_per_frame = 0.0223f},
  {.motion_id = 40744, .frame_open_sfx_begin = 24.0f, .frame_phase1_end = 47.0f, .frame_close_sfx_begin = 94.0f, .frame_motion_end = 120.0f, .approach_off_x = 49.0f, .approach_off_z = 58.5f, .npc_tail_end_frame = 50.0f, .npc_tail_delta_per_frame = 0.0327f},
  {.motion_id = 40717, .frame_open_sfx_begin = 20.0f, .frame_phase1_end = 39.0f, .frame_close_sfx_begin = 98.0f, .frame_motion_end = 125.0f, .approach_off_x = 51.0f, .approach_off_z = 80.081001f, .npc_tail_end_frame = 47.0f, .npc_tail_delta_per_frame = 1.9319f},
  {.motion_id = 40718, .frame_open_sfx_begin = 35.0f, .frame_phase1_end = 55.0f, .frame_close_sfx_begin = 110.0f, .frame_motion_end = 152.0f, .approach_off_x = 51.819f, .approach_off_z = 82.554001f, .npc_tail_end_frame = 87.0f, .npc_tail_delta_per_frame = 2.2963f},
  {.motion_id = 40745, .frame_open_sfx_begin = 33.0f, .frame_phase1_end = 64.0f, .frame_close_sfx_begin = 100.0f, .frame_motion_end = 138.0f, .approach_off_x = 39.535f, .approach_off_z = 95.652f, .npc_tail_end_frame = 71.0f, .npc_tail_delta_per_frame = 1.1599f},
  {.motion_id = 40719, .frame_open_sfx_begin = 43.0f, .frame_phase1_end = 73.0f, .frame_close_sfx_begin = 100.0f, .frame_motion_end = 135.0f, .approach_off_x = 50.158001f, .approach_off_z = 59.491001f, .npc_tail_end_frame = 88.0f, .npc_tail_delta_per_frame = 1.0387f},
  {.motion_id = 40746, .frame_open_sfx_begin = 21.0f, .frame_phase1_end = 41.0f, .frame_close_sfx_begin = 84.0f, .frame_motion_end = 106.0f, .approach_off_x = 45.0f, .approach_off_z = 54.0f, .npc_tail_end_frame = 43.0f, .npc_tail_delta_per_frame = 0.03f},
  {.motion_id = 40747, .frame_open_sfx_begin = 51.0f, .frame_phase1_end = 76.0f, .frame_close_sfx_begin = 156.0f, .frame_motion_end = 184.0f, .approach_off_x = 51.0f, .approach_off_z = 52.5f, .npc_tail_end_frame = 79.0f, .npc_tail_delta_per_frame = 0.0283f},
  {.motion_id = 40720, .frame_open_sfx_begin = 25.0f, .frame_phase1_end = 48.0f, .frame_close_sfx_begin = 100.0f, .frame_motion_end = 129.0f, .approach_off_x = 56.070999f, .approach_off_z = 70.459f, .npc_tail_end_frame = 64.0f, .npc_tail_delta_per_frame = 1.4962f},
  {.motion_id = 40721, .frame_open_sfx_begin = 64.0f, .frame_phase1_end = 99.0f, .frame_close_sfx_begin = 160.0f, .frame_motion_end = 200.0f, .approach_off_x = 43.352001f, .approach_off_z = 106.598999f, .npc_tail_end_frame = 131.0f, .npc_tail_delta_per_frame = 2.0784f},
  {.motion_id = 40748, .frame_open_sfx_begin = 38.0f, .frame_phase1_end = 66.0f, .frame_close_sfx_begin = 102.0f, .frame_motion_end = 133.0f, .approach_off_x = 52.025002f, .approach_off_z = 100.740997f, .npc_tail_end_frame = 69.0f, .npc_tail_delta_per_frame = 0.7168f},
  {.motion_id = 40722, .frame_open_sfx_begin = 47.0f, .frame_phase1_end = 108.0f, .frame_close_sfx_begin = 153.0f, .frame_motion_end = 200.0f, .approach_off_x = 53.535f, .approach_off_z = 90.637001f, .npc_tail_end_frame = 115.0f, .npc_tail_delta_per_frame = 0.0565f},
  {.motion_id = 40749, .frame_open_sfx_begin = 22.0f, .frame_phase1_end = 42.0f, .frame_close_sfx_begin = 84.0f, .frame_motion_end = 105.0f, .approach_off_x = 51.0f, .approach_off_z = 44.5f, .npc_tail_end_frame = 43.0f, .npc_tail_delta_per_frame = 0.02f},
  {.motion_id = 40750, .frame_open_sfx_begin = 39.0f, .frame_phase1_end = 67.0f, .frame_close_sfx_begin = 140.0f, .frame_motion_end = 176.0f, .approach_off_x = 47.0f, .approach_off_z = 54.1f, .npc_tail_end_frame = 75.0f, .npc_tail_delta_per_frame = 0.025f},
  {.motion_id = 40723, .frame_open_sfx_begin = 22.0f, .frame_phase1_end = 46.0f, .frame_close_sfx_begin = 110.0f, .frame_motion_end = 135.0f, .approach_off_x = 60.0f, .approach_off_z = 68.424004f, .npc_tail_end_frame = 67.0f, .npc_tail_delta_per_frame = 1.7189f},
  {.motion_id = 40724, .frame_open_sfx_begin = 39.0f, .frame_phase1_end = 68.0f, .frame_close_sfx_begin = 124.0f, .frame_motion_end = 173.0f, .approach_off_x = 54.183998f, .approach_off_z = 76.428001f, .npc_tail_end_frame = 98.0f, .npc_tail_delta_per_frame = 1.9291f},
  {.motion_id = 40751, .frame_open_sfx_begin = 38.0f, .frame_phase1_end = 70.0f, .frame_close_sfx_begin = 102.0f, .frame_motion_end = 134.0f, .approach_off_x = 45.0f, .approach_off_z = 96.102997f, .npc_tail_end_frame = 70.0f, .npc_tail_delta_per_frame = 0.0f},
  {.motion_id = 40725, .frame_open_sfx_begin = 40.0f, .frame_phase1_end = 84.0f, .frame_close_sfx_begin = 130.0f, .frame_motion_end = 190.0f, .approach_off_x = 50.0f, .approach_off_z = 81.102997f, .npc_tail_end_frame = 110.0f, .npc_tail_delta_per_frame = 1.0671f},
  {.motion_id = 40757, .frame_open_sfx_begin = 21.0f, .frame_phase1_end = 45.0f, .frame_close_sfx_begin = 84.0f, .frame_motion_end = 109.0f, .approach_off_x = 48.5f, .approach_off_z = 44.5f, .npc_tail_end_frame = 46.0f, .npc_tail_delta_per_frame = 0.01f},
  {.motion_id = 40752, .frame_open_sfx_begin = 39.0f, .frame_phase1_end = 67.0f, .frame_close_sfx_begin = 110.0f, .frame_motion_end = 146.0f, .approach_off_x = 49.5f, .approach_off_z = 42.5f, .npc_tail_end_frame = 75.0f, .npc_tail_delta_per_frame = 0.0119f},
  {.motion_id = 40726, .frame_open_sfx_begin = 20.0f, .frame_phase1_end = 44.0f, .frame_close_sfx_begin = 110.0f, .frame_motion_end = 136.0f, .approach_off_x = 60.0f, .approach_off_z = 68.424004f, .npc_tail_end_frame = 56.0f, .npc_tail_delta_per_frame = 0.7448f},
  {.motion_id = 40727, .frame_open_sfx_begin = 42.0f, .frame_phase1_end = 66.0f, .frame_close_sfx_begin = 127.0f, .frame_motion_end = 185.0f, .approach_off_x = 54.183998f, .approach_off_z = 81.102997f, .npc_tail_end_frame = 100.0f, .npc_tail_delta_per_frame = 1.8151f},
  {.motion_id = 40753, .frame_open_sfx_begin = 36.0f, .frame_phase1_end = 57.0f, .frame_close_sfx_begin = 85.0f, .frame_motion_end = 111.0f, .approach_off_x = 30.0f, .approach_off_z = 81.102997f, .npc_tail_end_frame = 62.0f, .npc_tail_delta_per_frame = 1.421f},
  {.motion_id = 40728, .frame_open_sfx_begin = 37.0f, .frame_phase1_end = 87.0f, .frame_close_sfx_begin = 130.0f, .frame_motion_end = 194.0f, .approach_off_x = 50.0f, .approach_off_z = 71.102997f, .npc_tail_end_frame = 101.0f, .npc_tail_delta_per_frame = 0.7448f},
  {.motion_id = 40758, .frame_open_sfx_begin = 21.0f, .frame_phase1_end = 52.0f, .frame_close_sfx_begin = 90.0f, .frame_motion_end = 123.0f, .approach_off_x = 47.553001f, .approach_off_z = 52.632999f, .npc_tail_end_frame = 54.0f, .npc_tail_delta_per_frame = 0.01f},
  {.motion_id = 40754, .frame_open_sfx_begin = 24.0f, .frame_phase1_end = 47.0f, .frame_close_sfx_begin = 104.0f, .frame_motion_end = 133.0f, .approach_off_x = 48.0f, .approach_off_z = 52.132999f, .npc_tail_end_frame = 53.0f, .npc_tail_delta_per_frame = 0.0247f},
  {.motion_id = 40729, .frame_open_sfx_begin = 18.0f, .frame_phase1_end = 42.0f, .frame_close_sfx_begin = 108.0f, .frame_motion_end = 138.0f, .approach_off_x = 45.0f, .approach_off_z = 78.463997f, .npc_tail_end_frame = 68.0f, .npc_tail_delta_per_frame = 1.3195f},
  {.motion_id = 40730, .frame_open_sfx_begin = 35.0f, .frame_phase1_end = 56.0f, .frame_close_sfx_begin = 143.0f, .frame_motion_end = 183.0f, .approach_off_x = 55.4f, .approach_off_z = 84.542999f, .npc_tail_end_frame = 85.0f, .npc_tail_delta_per_frame = 1.9986f},
  {.motion_id = 40755, .frame_open_sfx_begin = 32.0f, .frame_phase1_end = 65.0f, .frame_close_sfx_begin = 110.0f, .frame_motion_end = 144.0f, .approach_off_x = 41.187f, .approach_off_z = 95.0f, .npc_tail_end_frame = 66.0f, .npc_tail_delta_per_frame = 0.2855f},
  {.motion_id = 40756, .frame_open_sfx_begin = 44.0f, .frame_phase1_end = 75.0f, .frame_close_sfx_begin = 105.0f, .frame_motion_end = 151.0f, .approach_off_x = 50.629002f, .approach_off_z = 56.424f, .npc_tail_end_frame = 90.0f, .npc_tail_delta_per_frame = 1.2961f},
  {.motion_id = 40759, .frame_open_sfx_begin = 55.0f, .frame_phase1_end = 90.0f, .frame_close_sfx_begin = 150.0f, .frame_motion_end = 180.0f, .approach_off_x = 30.0f, .approach_off_z = 80.0f, .npc_tail_end_frame = 90.0f, .npc_tail_delta_per_frame = 0.0f},
  {.motion_id = 40760, .frame_open_sfx_begin = 29.0f, .frame_phase1_end = 60.0f, .frame_close_sfx_begin = 71.0f, .frame_motion_end = 110.0f, .approach_off_x = 47.535f, .approach_off_z = 75.652f, .npc_tail_end_frame = 71.0f, .npc_tail_delta_per_frame = 0.1295f},
  {.motion_id = 40761, .frame_open_sfx_begin = 29.0f, .frame_phase1_end = 66.0f, .frame_close_sfx_begin = 104.0f, .frame_motion_end = 200.0f, .approach_off_x = 52.025002f, .approach_off_z = 80.740997f, .npc_tail_end_frame = 67.0f, .npc_tail_delta_per_frame = 0.0231f},
  {.motion_id = 40762, .frame_open_sfx_begin = 36.0f, .frame_phase1_end = 62.0f, .frame_close_sfx_begin = 73.0f, .frame_motion_end = 114.0f, .approach_off_x = 45.0f, .approach_off_z = 71.102997f, .npc_tail_end_frame = 62.0f, .npc_tail_delta_per_frame = 0.0f},
  {.motion_id = 40763, .frame_open_sfx_begin = 32.0f, .frame_phase1_end = 60.0f, .frame_close_sfx_begin = 78.0f, .frame_motion_end = 120.0f, .approach_off_x = 44.284f, .approach_off_z = 81.165001f, .npc_tail_end_frame = 60.0f, .npc_tail_delta_per_frame = 0.0f},
  {.motion_id = 40764, .frame_open_sfx_begin = 52.0f, .frame_phase1_end = 74.0f, .frame_close_sfx_begin = 180.0f, .frame_motion_end = 225.0f, .approach_off_x = 45.0f, .approach_off_z = 65.0f, .npc_tail_end_frame = 95.0f, .npc_tail_delta_per_frame = 2.0f},
  {.motion_id = 40765, .frame_open_sfx_begin = 52.0f, .frame_phase1_end = 76.0f, .frame_close_sfx_begin = 190.0f, .frame_motion_end = 240.0f, .approach_off_x = 45.0f, .approach_off_z = 60.0f, .npc_tail_end_frame = 98.0f, .npc_tail_delta_per_frame = 1.7727f}
};
static uint32_t data_10198 = 0xFFFFFFFF;
typedef struct Row_1019C { float w0; float w1; float w2; float w3; int32_t w4; } Row_1019C;
static const Row_1019C data_1019C[7] = {
  {0.0f, 0.0f, 0.0f, 0.0f, -1},
  {0.0f, 0.0f, 0.0f, 0.0f, -1},
  {0.0f, 0.0f, 0.0f, 0.0f, 1534},
  {26.0f, 55.0f, 45.0f, 75.0f, 1555},
  {61.0f, 146.0f, 45.0f, 50.0f, 1554},
  {59.0f, 124.0f, 45.0f, 70.0f, 1543},
  {33.0f, 49.0f, 45.0f, 50.0f, 1543}
};
static const float data_10228[4] = {33.0f, 49.0f, 50.0f, 50.0f};
static void (*data_10238)(int32_t, int32_t, int32_t) = script_09C8;
static void (*const data_1023C)(int32_t, int32_t, int32_t, int32_t) = script_09D8;
static void (*const data_10240)(int32_t, int32_t, int32_t, int32_t) = script_09D8;
static void (*const data_10244)(int32_t, int32_t, int32_t, int32_t) = script_09D8;
static void (*const data_10248)(int32_t, int32_t, int32_t, int32_t) = script_09D8;
static void (*const data_1024C)(int32_t, int32_t, int32_t, int32_t) = script_09D8;
static void (*const data_10250)(int32_t, int32_t, int32_t, int32_t) = script_09D8;
static void (*const data_10254)(int32_t, int32_t, int32_t, int32_t) = script_09D8;
static void (*const data_10258)(int32_t, int32_t, int32_t, int32_t) = script_09D8;
static void (*const data_1025C)(int32_t, int32_t, int32_t, int32_t) = script_0C7E;
static void (*const data_10260)(int32_t, int32_t, int32_t, int32_t) = script_0DDF;
static void (*const data_10264)(int32_t, int32_t, int32_t, int32_t) = script_12AC;
static void (*const data_10268)(int32_t, int32_t, int32_t, int32_t) = script_1313;
static void (*const data_1026C)(int32_t, int32_t, int32_t, int32_t) = script_0ADC;
static void (*const data_10270)(int32_t, int32_t, int32_t, int32_t) = script_0ADC;
static void (*const data_10274)(int32_t, int32_t, int32_t, int32_t) = script_0F40;
static void (*const data_10278)(int32_t, int32_t, int32_t, int32_t) = script_11DE;
static void (*const data_1027C)(int32_t, int32_t, int32_t, int32_t) = script_137A;
static void (*const data_10280)(int32_t, int32_t, int32_t, int32_t) = script_0C7E;
static void (*const data_10284)(int32_t, int32_t, int32_t, int32_t) = script_0DDF;
static void (*const data_10288)(int32_t, int32_t, int32_t, int32_t) = script_0F40;
static void (*const data_1028C)(int32_t, int32_t, int32_t, int32_t) = script_0C7E;
static void (*const data_10290)(int32_t, int32_t, int32_t, int32_t) = script_0DDF;
static void (*const data_10294)(int32_t, int32_t, int32_t, int32_t) = script_0F40;
static void (*const data_10298)(int32_t, int32_t, int32_t, int32_t) = script_13C1;
static void (*const data_1029C)(int32_t, int32_t, int32_t, int32_t) = script_13C1;
static void (*const data_102A0)(int32_t, int32_t, int32_t, int32_t) = script_13C1;
static void (*const data_102A4)(int32_t, int32_t, int32_t, int32_t) = script_13C1;
static void (*const data_102A8)(int32_t, int32_t, int32_t, int32_t) = script_14F7;
static void (*const data_102AC)(int32_t, int32_t, int32_t, int32_t) = script_1DB8;
static void (*const data_102B0)(int32_t, int32_t, int32_t, int32_t) = script_09D8;
static void (*const data_102B4)(int32_t, int32_t, int32_t, int32_t) = script_09D8;
static void (*const data_102B8)(int32_t, int32_t, int32_t, int32_t) = script_09D8;
static void (*const data_102BC)(int32_t, int32_t, int32_t, int32_t) = script_09D8;
static void (*const data_102C0)(int32_t, int32_t, int32_t, int32_t) = script_09D8;
static void (*const data_102C4)(int32_t, int32_t, int32_t, int32_t) = script_09D8;
static void (*const data_102C8)(int32_t, int32_t, int32_t, int32_t) = script_09D8;
static void (*const data_102CC)(int32_t, int32_t, int32_t, int32_t) = script_09D8;
static const char data_102D0[46] __attribute__((scn_string_literal)) = "Door Index = %d : call_type = %d : type = %d\012";
static const char data_102FE[43] __attribute__((scn_string_literal)) = "Door Open Motion = DEB_AKESIME_FUSUMA_L_F\012";
static const char data_10329[43] __attribute__((scn_string_literal)) = "Door Open Motion = DEB_AKESIME_FUSUMA_R_F\012";
static const char data_10354[42] __attribute__((scn_string_literal)) = "Door Open Motion = DEB_HIKISIME_DOOR_L_F\012";
static const char data_1037E[42] __attribute__((scn_string_literal)) = "Door Open Motion = DEB_HIKISIME_DOOR_R_F\012";
static const char data_103A8[41] __attribute__((scn_string_literal)) = "Door Open Motion = DEB_OSISIME_DOOR_L_F\012";
static const char data_103D1[41] __attribute__((scn_string_literal)) = "Door Open Motion = DEB_OSISIME_DOOR_R_F\012";
static const char data_103FA[43] __attribute__((scn_string_literal)) = "Door Open Motion = GAK_AKESIME_FUSUMA_L_F\012";
static const char data_10425[43] __attribute__((scn_string_literal)) = "Door Open Motion = GAK_AKESIME_FUSUMA_R_F\012";
static const char data_10450[42] __attribute__((scn_string_literal)) = "Door Open Motion = GAK_HIKISIME_DOOR_L_F\012";
static const char data_1047A[42] __attribute__((scn_string_literal)) = "Door Open Motion = GAK_HIKISIME_DOOR_R_F\012";
static const char data_104A4[41] __attribute__((scn_string_literal)) = "Door Open Motion = GAK_OSISIME_DOOR_L_F\012";
static const char data_104CD[41] __attribute__((scn_string_literal)) = "Door Open Motion = GAK_OSISIME_DOOR_R_F\012";
static const char data_104F6[43] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_AKESIME_FUSUMA_L_F\012";
static const char data_10521[43] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_AKESIME_FUSUMA_R_F\012";
static const char data_1054C[42] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_HIKISIME_DOOR_L_F\012";
static const char data_10576[42] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_HIKISIME_DOOR_R_F\012";
static const char data_105A0[41] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_OSISIME_DOOR_L_F\012";
static const char data_105C9[41] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_OSISIME_DOOR_R_F\012";
static const char data_105F2[43] __attribute__((scn_string_literal)) = "Door Open Motion = KOD_AKESIME_FUSUMA_L_F\012";
static const char data_1061D[43] __attribute__((scn_string_literal)) = "Door Open Motion = KOD_AKESIME_FUSUMA_R_F\012";
static const char data_10648[42] __attribute__((scn_string_literal)) = "Door Open Motion = KOD_HIKISIME_DOOR_L_F\012";
static const char data_10672[42] __attribute__((scn_string_literal)) = "Door Open Motion = KOD_HIKISIME_DOOR_R_F\012";
static const char data_1069C[41] __attribute__((scn_string_literal)) = "Door Open Motion = KOD_OSISIME_DOOR_L_F\012";
static const char data_106C5[41] __attribute__((scn_string_literal)) = "Door Open Motion = KOD_OSISIME_DOOR_R_F\012";
static const char data_106EE[43] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_AKESIME_FUSUMA_L_F\012";
static const char data_10719[43] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_AKESIME_FUSUMA_R_F\012";
static const char data_10744[42] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_HIKISIME_DOOR_L_F\012";
static const char data_1076E[42] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_HIKISIME_DOOR_R_F\012";
static const char data_10798[41] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_OSISIME_DOOR_L_F\012";
static const char data_107C1[41] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_OSISIME_DOOR_R_F\012";
static const char data_107EA[43] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_AKESIME_FUSUMA_L_F\012";
static const char data_10815[43] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_AKESIME_FUSUMA_R_F\012";
static const char data_10840[42] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_HIKISIME_DOOR_L_F\012";
static const char data_1086A[42] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_HIKISIME_DOOR_R_F\012";
static const char data_10894[41] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_OSISIME_DOOR_L_F\012";
static const char data_108BD[41] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_OSISIME_DOOR_R_F\012";
static const char data_108E6[43] __attribute__((scn_string_literal)) = "Door Open Motion = SIN_AKESIME_FUSUMA_L_F\012";
static const char data_10911[43] __attribute__((scn_string_literal)) = "Door Open Motion = SIN_AKESIME_FUSUMA_R_F\012";
static const char data_1093C[42] __attribute__((scn_string_literal)) = "Door Open Motion = SIN_HIKISIME_DOOR_L_F\012";
static const char data_10966[42] __attribute__((scn_string_literal)) = "Door Open Motion = SIN_HIKISIME_DOOR_R_F\012";
static const char data_10990[41] __attribute__((scn_string_literal)) = "Door Open Motion = SIN_OSISIME_DOOR_L_F\012";
static const char data_109B9[41] __attribute__((scn_string_literal)) = "Door Open Motion = SIN_OSISIME_DOOR_R_F\012";
static const char data_109E2[43] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_AKESIME_FUSUMA_L_F\012";
static const char data_10A0D[43] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_AKESIME_FUSUMA_R_F\012";
static const char data_10A38[42] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_HIKISIME_DOOR_L_F\012";
static const char data_10A62[42] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_HIKISIME_DOOR_R_F\012";
static const char data_10A8C[41] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_OSISIME_DOOR_L_F\012";
static const char data_10AB5[41] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_OSISIME_DOOR_R_F\012";
static const char data_10ADE[43] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_AKESIME_FUSUMA_L_F\012";
static const char data_10B09[43] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_AKESIME_FUSUMA_R_F\012";
static const char data_10B34[42] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_HIKISIME_DOOR_L_F\012";
static const char data_10B5E[42] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_HIKISIME_DOOR_R_F\012";
static const char data_10B88[41] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_OSISIME_DOOR_L_F\012";
static const char data_10BB1[41] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_OSISIME_DOOR_R_F\012";
static const char data_10BDA[44] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_OSISIME_DOOR_AP_L_F\012";
static const char data_10C06[44] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_OSISIME_DOOR_AP_L_F\012";
static const char data_10C32[44] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_OSISIME_DOOR_AP_L_F\012";
static const char data_10C5E[44] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_OSISIME_DOOR_AP_L_F\012";
static const char data_10C8A[44] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_OSISIME_DOOR_AP_L_F\012";
static const char data_10CB6[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_10CD2[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_10CEE[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_10D0A[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_10D26[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_10D42[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_10D5E[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_10D7A[13] __attribute__((scn_string_literal)) = "open-move!!\012";
static const char data_10D87[11] __attribute__((scn_string_literal)) = "M_MDOR.BIN";
static const char data_10D92[] __attribute__((scn_string_literal)) = "This Charactor Work is NULL!!\n";
static const char data_10DB1[22] __attribute__((scn_string_literal)) = "手動ドア開け番号指定\012";
static const char data_10DC7[34] __attribute__((scn_string_literal)) = "無限ループになりそうなので抜ける\012";
static const char data_10DE9[] __attribute__((scn_string_literal)) = "Don't make BOX Buffer!!\n";
static const char data_10E02[] __attribute__((scn_string_literal)) = "Don't make BOX Buffer!!\n";
static const char data_10E1B[15] __attribute__((scn_string_literal)) = "Door System : ";
static const char data_10E2A[15] __attribute__((scn_string_literal)) = "Door System : ";
static const char data_10E39[18] __attribute__((scn_string_literal)) = "Door Box is ON!!\012";
static const char data_10E4B[19] __attribute__((scn_string_literal)) = "Door Box is OFF!!\012";
static const char data_10E5E[18] __attribute__((scn_string_literal)) = "Door Box is ON!!\012";
static const char data_10E70[20] __attribute__((scn_string_literal)) = "Door Box is OFF!!\012";
static const DSWKDoorEntry data_10E84 = {.slot = 0, .flags = 0, .ctrl_mask_pair = 0x640064U, .coarse_metric = 0.0f, .selection_metric = 0.0f, .view_ang_delta = 0, .group = 0, .global1 = 0xFFFFFFFFU, .global2 = 0xFFFFFFFFU, .global3 = 0xFFFFFFFFU, .sndCommandId = 0xFFFFFFFFU, .global5 = 0xFFFFFFFFU, .global6 = 0xFFFFFFFFU};
static const int32_t data_10EB8[13] = {-1, 0, 6553700, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1};
static const uint8_t data_10EEC[4] = {0};
static const int32_t data_10EF0 = -1;
static char data_10EF4[] = "DR02_015";
static const uint8_t data_10EFD[3] = {0};
static char *const data_10F00 = data_10EF4;
static const int32_t data_10F04[3] = {2, 1311403, 1376939};
static const MapObjectRecord data_10F10 = {.kind = 2, .resourceIndex = 0, .scale = {1.0f, 1.0f, 1.0f}, .pos = {3.3f, 0.0f, 1.82f}, .yawFixed16 = 0};
static const uint32_t data_10F34 = 380;
static const int32_t data_10F38[6] = {1, 390, 0, 1018, 0, -1};
static const uint32_t data_10F50 = 510;
static const int32_t data_10F54[6] = {1, 525, 0, 1020, 0, -1};
static const uint32_t data_10F6C = 150;
static const int32_t data_10F70[6] = {1, 160, 0, 1019, 0, -1};
static const uint32_t data_10F88 = 530;
static const int32_t data_10F8C[6] = {1, 970, 0, 1014, 0, -1};
static const uint32_t data_10FA4 = 911;
static const int32_t data_10FA8[6] = {1, 155, 0, 1015, 0, -1};
static const char data_10FC0[7] __attribute__((scn_string_literal)) = "FREE02";
static const char data_10FC7[7] __attribute__((scn_string_literal)) = "FREE03";
static const char data_10FCE[7] __attribute__((scn_string_literal)) = "FREE04";
static const char data_10FD5[7] __attribute__((scn_string_literal)) = "FREE01";
static const char data_10FDC[5] __attribute__((scn_string_literal)) = "SY11";
static const char data_10FE1[9] __attribute__((scn_string_literal)) = "SY11A006";
static const char data_10FEA[9] __attribute__((scn_string_literal)) = "SY11A007";
static const char data_10FF3[9] __attribute__((scn_string_literal)) = "SY11A008";
static const char data_10FFC[9] __attribute__((scn_string_literal)) = "SY11A002";
static const char data_11005[9] __attribute__((scn_string_literal)) = "SY11A003";
static const char data_1100E[9] __attribute__((scn_string_literal)) = "SY11A004";
static const char data_11017[9] __attribute__((scn_string_literal)) = "SY11A005";
static const char data_11020[9] __attribute__((scn_string_literal)) = "SY11A001";
static const char data_11029[9] __attribute__((scn_string_literal)) = "SY11A009";
static const char data_11032[9] __attribute__((scn_string_literal)) = "SY11A010";
static const char data_1103B[9] __attribute__((scn_string_literal)) = "SY11A011";
static const char data_11044[9] __attribute__((scn_string_literal)) = "SY11A009";
static const char data_1104D[5] __attribute__((scn_string_literal)) = "SY11";
static const char data_11052[5] __attribute__((scn_string_literal)) = "SY11";
static const char data_11057[9] __attribute__((scn_string_literal)) = "SY11A001";
static const char data_11060[5] __attribute__((scn_string_literal)) = "SY11";
static const char data_11065[5] __attribute__((scn_string_literal)) = "SY11";
static const char data_1106A[9] __attribute__((scn_string_literal)) = "SY11A009";
static const char data_11073[9] __attribute__((scn_string_literal)) = "SY11A010";
static const char data_1107C[9] __attribute__((scn_string_literal)) = "SY11A011";
static const char data_11085[5] __attribute__((scn_string_literal)) = "SY11";
static const char data_1108A[10] __attribute__((scn_string_literal)) = "E1072B024";
static const char data_11094[10] __attribute__((scn_string_literal)) = "E1072B093";
static const char data_1109E[10] __attribute__((scn_string_literal)) = "E1072B094";
static const char data_110A8[10] __attribute__((scn_string_literal)) = "E1072B092";
static const char data_110B2[10] __attribute__((scn_string_literal)) = "E1072B131";
static const char data_110BC[10] __attribute__((scn_string_literal)) = "E1072B132";
static const char data_110C6[10] __attribute__((scn_string_literal)) = "E1072B133";
static const char data_110D0[10] __attribute__((scn_string_literal)) = "E1072B136";
static const char data_110DA[10] __attribute__((scn_string_literal)) = "E1072B135";
static const char data_110E4[10] __attribute__((scn_string_literal)) = "E1072B136";
static const char data_110EE[10] __attribute__((scn_string_literal)) = "E1072B137";
static const char data_110F8[10] __attribute__((scn_string_literal)) = "E1072B138";
static const char data_11102[10] __attribute__((scn_string_literal)) = "E1072B139";
static const char data_1110C[10] __attribute__((scn_string_literal)) = "E1072B140";
static const char data_11116[10] __attribute__((scn_string_literal)) = "E1072B141";
static const char data_11120[10] __attribute__((scn_string_literal)) = "E1072B142";
static const char data_1112A[10] __attribute__((scn_string_literal)) = "E1072B143";
static const char data_11134[10] __attribute__((scn_string_literal)) = "E1072B144";
static const char data_1113E[10] __attribute__((scn_string_literal)) = "E1072B145";
static const char data_11148[10] __attribute__((scn_string_literal)) = "E1072B146";
static const char data_11152[10] __attribute__((scn_string_literal)) = "E1072B147";
static const char data_1115C[10] __attribute__((scn_string_literal)) = "E1072B148";
static const char data_11166[10] __attribute__((scn_string_literal)) = "E1072B149";
static const char data_11170[10] __attribute__((scn_string_literal)) = "E1072B150";
static const char data_1117A[10] __attribute__((scn_string_literal)) = "E1072B151";
static const char data_11184[10] __attribute__((scn_string_literal)) = "E1072B152";
static const char data_1118E[10] __attribute__((scn_string_literal)) = "E1072B153";
static const char data_11198[10] __attribute__((scn_string_literal)) = "E1072B154";
static const char data_111A2[10] __attribute__((scn_string_literal)) = "E1072B153";
static const char data_111AC[10] __attribute__((scn_string_literal)) = "E1072B154";
static const char data_111B6[10] __attribute__((scn_string_literal)) = "E1072B155";
static const char data_111C0[10] __attribute__((scn_string_literal)) = "E1072B156";
static const char data_111CA[10] __attribute__((scn_string_literal)) = "E1072B157";
static const char data_111D4[10] __attribute__((scn_string_literal)) = "E1072B158";
static const char data_111DE[10] __attribute__((scn_string_literal)) = "E1072B159";
static const char data_111E8[10] __attribute__((scn_string_literal)) = "E1072B160";
static const char data_111F2[10] __attribute__((scn_string_literal)) = "E1072B161";
static const char data_111FC[10] __attribute__((scn_string_literal)) = "E1072B162";
static const char data_11206[10] __attribute__((scn_string_literal)) = "E1072B163";
static const char data_11210[10] __attribute__((scn_string_literal)) = "E1072B164";
static const char data_1121A[10] __attribute__((scn_string_literal)) = "E1072B165";
static const char data_11224[10] __attribute__((scn_string_literal)) = "E1072B166";
static const char data_1122E[10] __attribute__((scn_string_literal)) = "E1072B167";
static const char data_11238[10] __attribute__((scn_string_literal)) = "E1072B169";
static const char data_11242[10] __attribute__((scn_string_literal)) = "E1072B168";
static const char data_1124C[10] __attribute__((scn_string_literal)) = "E1072B170";
static const char data_11256[10] __attribute__((scn_string_literal)) = "E1072B171";
static const char data_11260[10] __attribute__((scn_string_literal)) = "E1072B172";
static const char data_1126A[10] __attribute__((scn_string_literal)) = "E1072B162";
static const char data_11274[10] __attribute__((scn_string_literal)) = "E1072B173";
static const char data_1127E[10] __attribute__((scn_string_literal)) = "E1072B174";
static const char data_11288[10] __attribute__((scn_string_literal)) = "E1072B175";
static const char data_11292[10] __attribute__((scn_string_literal)) = "E1072B176";
static const char data_1129C[10] __attribute__((scn_string_literal)) = "E1072B177";
static const char data_112A6[10] __attribute__((scn_string_literal)) = "E1072B179";
static const char data_112B0[10] __attribute__((scn_string_literal)) = "E1072B180";
static const char data_112BA[10] __attribute__((scn_string_literal)) = "E1072B181";
static const char data_112C4[10] __attribute__((scn_string_literal)) = "E1072B182";
static const char data_112CE[10] __attribute__((scn_string_literal)) = "E1072B183";
static const char data_112D8[10] __attribute__((scn_string_literal)) = "E1072B184";
static const char data_112E2[10] __attribute__((scn_string_literal)) = "E1072B185";
static const char data_112EC[10] __attribute__((scn_string_literal)) = "E1072B186";
static const char data_112F6[10] __attribute__((scn_string_literal)) = "E1072B187";
static const char data_11300[10] __attribute__((scn_string_literal)) = "E1072B188";
static const char data_1130A[10] __attribute__((scn_string_literal)) = "E1072B189";
static const char data_11314[10] __attribute__((scn_string_literal)) = "E1072B190";
static const char data_1131E[10] __attribute__((scn_string_literal)) = "E1072B191";
static const char data_11328[10] __attribute__((scn_string_literal)) = "E1072B192";
static const char data_11332[10] __attribute__((scn_string_literal)) = "E1072B193";
static const char data_1133C[10] __attribute__((scn_string_literal)) = "E1072B194";
static const char data_11346[10] __attribute__((scn_string_literal)) = "E1072B195";
static const char data_11350[10] __attribute__((scn_string_literal)) = "E1072B196";
static const char data_1135A[10] __attribute__((scn_string_literal)) = "E1072B197";
static const char data_11364[10] __attribute__((scn_string_literal)) = "E1072B198";
static const char data_1136E[10] __attribute__((scn_string_literal)) = "E1072B199";
static const char data_11378[10] __attribute__((scn_string_literal)) = "E1072B200";
static const char data_11382[10] __attribute__((scn_string_literal)) = "E1072B201";
static const char data_1138C[10] __attribute__((scn_string_literal)) = "E1072B202";
static const char data_11396[10] __attribute__((scn_string_literal)) = "E1072B203";
static const char data_113A0[10] __attribute__((scn_string_literal)) = "E1072B178";
static const char data_113AA[10] __attribute__((scn_string_literal)) = "E1072B096";
static const char data_113B4[10] __attribute__((scn_string_literal)) = "E1072B097";
static const char data_113BE[10] __attribute__((scn_string_literal)) = "E1072B098";
static const char data_113C8[10] __attribute__((scn_string_literal)) = "E1072B099";
static const char data_113D2[10] __attribute__((scn_string_literal)) = "E1072B100";
static const char data_113DC[10] __attribute__((scn_string_literal)) = "E1072B101";
static const char data_113E6[10] __attribute__((scn_string_literal)) = "E1072B102";
static const char data_113F0[10] __attribute__((scn_string_literal)) = "E1072B103";
static const char data_113FA[10] __attribute__((scn_string_literal)) = "E1072B104";
static const char data_11404[10] __attribute__((scn_string_literal)) = "E1072B105";
static const char data_1140E[10] __attribute__((scn_string_literal)) = "E1072B106";
static const char data_11418[10] __attribute__((scn_string_literal)) = "E1072B107";
static const char data_11422[10] __attribute__((scn_string_literal)) = "E1072B108";
static const char data_1142C[10] __attribute__((scn_string_literal)) = "E1072B109";
static const char data_11436[10] __attribute__((scn_string_literal)) = "E1072B110";
static const char data_11440[10] __attribute__((scn_string_literal)) = "E1072B111";
static const char data_1144A[10] __attribute__((scn_string_literal)) = "E1072B112";
static const char data_11454[10] __attribute__((scn_string_literal)) = "E1072B113";
static const char data_1145E[10] __attribute__((scn_string_literal)) = "E1072B114";
static const char data_11468[10] __attribute__((scn_string_literal)) = "E1072B115";
static const char data_11472[10] __attribute__((scn_string_literal)) = "E1072B116";
static const char data_1147C[10] __attribute__((scn_string_literal)) = "E1072B117";
static const char data_11486[10] __attribute__((scn_string_literal)) = "E1072B118";
static const char data_11490[10] __attribute__((scn_string_literal)) = "E1072B119";
static const char data_1149A[10] __attribute__((scn_string_literal)) = "E1072B120";
static const char data_114A4[10] __attribute__((scn_string_literal)) = "E1072B118";
static const char data_114AE[10] __attribute__((scn_string_literal)) = "E1072B121";
static const char data_114B8[10] __attribute__((scn_string_literal)) = "E1072B122";
static const char data_114C2[10] __attribute__((scn_string_literal)) = "E1072B123";
static const char data_114CC[10] __attribute__((scn_string_literal)) = "E1072B124";
static const char data_114D6[10] __attribute__((scn_string_literal)) = "E1072B125";
static const char data_114E0[10] __attribute__((scn_string_literal)) = "E1072B126";
static const char data_114EA[10] __attribute__((scn_string_literal)) = "E1072B127";
static const char data_114F4[10] __attribute__((scn_string_literal)) = "E1072B128";
static const char data_114FE[10] __attribute__((scn_string_literal)) = "E1072B129";
static const char data_11508[10] __attribute__((scn_string_literal)) = "E1072B130";
static const char data_11512[10] __attribute__((scn_string_literal)) = "E1072B012";
static const char data_1151C[10] __attribute__((scn_string_literal)) = "E1072B013";
static const char data_11526[10] __attribute__((scn_string_literal)) = "E1072B014";
static const char data_11530[10] __attribute__((scn_string_literal)) = "E1072B015";
static const char data_1153A[10] __attribute__((scn_string_literal)) = "E1072B016";
static const char data_11544[10] __attribute__((scn_string_literal)) = "E1072B017";
static const char data_1154E[10] __attribute__((scn_string_literal)) = "E1072B018";
static const char data_11558[10] __attribute__((scn_string_literal)) = "E1072B019";
static const char data_11562[10] __attribute__((scn_string_literal)) = "E1072B020";
static const char data_1156C[12] __attribute__((scn_string_literal)) = "E1072B021";
static char data_11578[] = "UNO_";
static const uint8_t data_1157D[16] = {
  0x80, 0x00, 0x00, 0x66, 0x66, 0x06, 0x40, 0x89, 0x41, 0x40, 0x3F, 0x5F, 0x98, 0x24, 0xC0, 0x06
};
static const uint8_t data_1158D[16] = {
  0x10, 0x00, 0x00, 0x52, 0x4B, 0x4B, 0x49, 0xAA, 0x8A, 0x00, 0x00, 0x66, 0x66, 0x06, 0x40, 0xF0
};
static const uint8_t data_1159D[16] = {
  0xA7, 0x46, 0x3F, 0xE8, 0xD9, 0x24, 0xC0, 0x07, 0x10, 0x00, 0x00, 0x48, 0x41, 0x54, 0x4F, 0xAA
};
static const uint8_t data_115AD[16] = {
  0x8A, 0x00, 0x00, 0x66, 0x66, 0x06, 0x40, 0x1B, 0x2F, 0x3D, 0x3F, 0xEA, 0x04, 0x24, 0xC0, 0x01
};
static const uint8_t data_115BD[16] = {
  0x10, 0x00, 0x00, 0x43, 0x48, 0x52, 0x4C, 0x00, 0x80, 0x00, 0x00, 0x66, 0x66, 0x06, 0x40, 0x7F
};
static const uint8_t data_115CD[16] = {
  0x6A, 0x3C, 0x3F, 0x76, 0x71, 0x23, 0xC0, 0x0E, 0x10, 0x00, 0x00, 0x41, 0x53, 0x44, 0x41, 0x00
};
static const uint8_t data_115DD[16] = {
  0x80, 0x00, 0x00, 0x66, 0x66, 0x06, 0x40, 0x64, 0x3B, 0x3F, 0x3F, 0x76, 0x71, 0x23, 0xC0, 0x13
};
static const uint8_t data_115ED[16] = {
  0x10, 0x00, 0x00, 0x47, 0x4F, 0x52, 0x4F, 0x00, 0x80, 0x00, 0x40, 0xC9, 0x76, 0x06, 0x40, 0x1B
};
static const uint8_t data_115FD[16] = {
  0x2F, 0x3D, 0x3F, 0xAF, 0x25, 0x24, 0xC0, 0x08, 0x10, 0x00, 0x00, 0x43, 0x48, 0x41, 0x49, 0x00
};
static const uint8_t data_1160D[16] = {
  0x80, 0x00, 0x40, 0x8F, 0xC2, 0x05, 0x40, 0xA8, 0xC6, 0x4B, 0x3F, 0x5D, 0x6D, 0x25, 0xC0, 0x03
};
static const uint8_t data_1161D[16] = {
  0x10, 0x00, 0x00, 0x53, 0x45, 0x53, 0x49, 0x00, 0x80, 0x00, 0x00, 0x66, 0x66, 0x06, 0x40, 0xDB
};
static const uint8_t data_1162D[16] = {
  0xF9, 0x3E, 0x3F, 0x76, 0x71, 0x23, 0xC0, 0x11, 0x10, 0x00, 0x00, 0x4D, 0x41, 0x52, 0x4B, 0x00
};
static const uint8_t data_1163D[16] = {
  0x80, 0x00, 0x40, 0x66, 0x66, 0x06, 0x40, 0x35, 0x5E, 0x3A, 0x3F, 0x76, 0x71, 0x23, 0xC0, 0x1C
};
static const uint8_t data_1164D[16] = {
  0x10, 0x00, 0x00, 0x53, 0x54, 0x52, 0x49, 0x00, 0x80, 0x00, 0x40, 0x66, 0x66, 0x06, 0x40, 0xF6
};
static const uint8_t data_1165D[16] = {
  0x28, 0x3C, 0x3F, 0x76, 0x71, 0x23, 0xC0, 0x1D, 0x10, 0x00, 0x00, 0x53, 0x49, 0x4E, 0x46, 0x00
};
static const uint8_t data_1166D[16] = {
  0x80, 0x00, 0x00, 0x66, 0x66, 0x06, 0x40, 0x89, 0x41, 0x40, 0x3F, 0x76, 0x71, 0x23, 0xC0, 0x13
};
static const uint8_t data_1167D[16] = {
  0x10, 0x00, 0x00, 0x54, 0x4F, 0x4E, 0x59, 0x00, 0x80, 0x00, 0x40, 0x66, 0x66, 0x06, 0x40, 0x1B
};
static const uint8_t data_1168D[16] = {
  0x2F, 0x3D, 0x3F, 0x76, 0x71, 0x23, 0xC0, 0x1D, 0x10, 0x00, 0x00, 0x53, 0x4D, 0x54, 0x48, 0x05
};
static const uint8_t data_1169D[16] = {
  0x9B, 0x00, 0x40, 0x66, 0x66, 0x06, 0x40, 0x6D, 0xE7, 0x3B, 0x3F, 0x76, 0x71, 0x23, 0xC0, 0x01
};
static const uint8_t data_116AD[16] = {
  0x10, 0x00, 0x00, 0x53, 0x4D, 0x53, 0x49, 0x38, 0x8E, 0x00, 0x40, 0x66, 0x66, 0x06, 0x40, 0xC9
};
static const uint8_t data_116BD[16] = {
  0x76, 0x3E, 0x3F, 0x76, 0x71, 0x23, 0xC0, 0x21, 0x10, 0x00, 0x00, 0x48, 0x49, 0x52, 0x41, 0x1C
};
static const uint8_t data_116CD[16] = {
  0xC7, 0x00, 0x00, 0xA2, 0x45, 0x06, 0x40, 0x89, 0x41, 0x40, 0x3F, 0x38, 0x67, 0x24, 0xC0, 0x04
};
static const uint8_t data_116DD[16] = {
  0x10, 0x00, 0x00, 0x4B, 0x45, 0x42, 0x47, 0xD2, 0x87, 0x00, 0x00, 0x66, 0x66, 0x06, 0x40, 0xB6
};
static const uint8_t data_116ED[16] = {
  0xF3, 0x3D, 0x3F, 0x76, 0x71, 0x23, 0xC0, 0x0D, 0x10, 0x00, 0x00, 0x4B, 0x45, 0x42, 0x42, 0x11
};
static const uint8_t data_116FD[16] = {
  0x91, 0x00, 0x00, 0x66, 0x66, 0x06, 0x40, 0xB6, 0xF3, 0x3D, 0x3F, 0x76, 0x71, 0x23, 0xC0, 0x0D
};
static const uint8_t data_1170D[16] = {
  0x10, 0x00, 0x00, 0x53, 0x49, 0x4E, 0x44, 0x55, 0x95, 0x00, 0x00, 0x66, 0x66, 0x06, 0x40, 0x27
};
static const uint8_t data_1171D[16] = {
  0x31, 0x48, 0x3F, 0x4B, 0xEA, 0x24, 0xC0, 0x1E, 0x10, 0x00, 0x00, 0x54, 0x4D, 0x52, 0x41, 0x3E
};
static const uint8_t data_1172D[16] = {
  0x89, 0x00, 0x00, 0x66, 0x66, 0x06, 0x40, 0x7F, 0x6A, 0x3C, 0x3F, 0x76, 0x71, 0x23, 0xC0, 0x09
};
static const uint8_t data_1173D[16] = {
  0x10, 0x00, 0x00, 0x4F, 0x4B, 0x59, 0x53, 0x00, 0x80, 0x00, 0x00, 0x66, 0x66, 0x06, 0x40, 0x77
};
static const uint8_t data_1174D[16] = {
  0xBE, 0x3F, 0x3F, 0x88, 0xF4, 0x23, 0xC0, 0x22, 0x10, 0x00, 0x00, 0x52, 0x42, 0x52, 0x54, 0x16
};
static const uint8_t data_1175D[16] = {
  0x8C, 0x00, 0x00, 0x66, 0x66, 0x06, 0x40, 0x1B, 0x2F, 0x3D, 0x3F, 0x76, 0x71, 0x23, 0xC0, 0x01
};
static const uint8_t data_1176D[16] = {
  0x10, 0x00, 0x00, 0x48, 0x4D, 0x52, 0x4F, 0x00, 0x80, 0x00, 0x00, 0x66, 0x66, 0x06, 0x40, 0xC9
};
static const uint8_t data_1177D[16] = {
  0x76, 0x3E, 0x3F, 0x76, 0x71, 0x23, 0xC0, 0x0A, 0x10, 0x00, 0x00, 0x44, 0x49, 0x43, 0x4B, 0x3E
};
static const uint8_t data_1178D[16] = {
  0x89, 0x00, 0x00, 0x66, 0x66, 0x06, 0x40, 0x2D, 0xB2, 0x3D, 0x3F, 0x76, 0x71, 0x23, 0xC0, 0x0B
};
static const uint8_t data_1179D[16] = {
  0x10, 0x00, 0x00, 0x53, 0x4F, 0x53, 0x49, 0x00, 0x80, 0x00, 0x00, 0x66, 0x66, 0x06, 0x40, 0x89
};
static const uint8_t data_117AD[16] = {
  0x41, 0x40, 0x3F, 0x76, 0x71, 0x23, 0xC0, 0x20, 0x10, 0x00, 0x00, 0x68, 0x61, 0x6E, 0x61, 0x00
};
static const uint8_t data_117BD[16] = {
  0x00, 0x00, 0x20, 0x66, 0x66, 0x06, 0x40, 0xBC, 0x74, 0x53, 0x3F, 0x76, 0x71, 0x23, 0xC0, 0x9A
};
static const uint8_t data_117CD[16] = {
  0x99, 0x19, 0x3E, 0x74, 0x61, 0x64, 0x6F, 0x00, 0x00, 0x00, 0x20, 0x7D, 0x3F, 0x05, 0x40, 0x60
};
static const uint8_t data_117DD[16] = {
  0xE5, 0x50, 0x3F, 0x76, 0x71, 0x23, 0xC0, 0x0A, 0xD7, 0x23, 0x3D, 0x6B, 0x69, 0x62, 0x61, 0x00
};
static const uint8_t data_117ED[16] = {
  0x00, 0x00, 0x20, 0x66, 0x66, 0x06, 0x40, 0xA2, 0x45, 0x56, 0x3F, 0x76, 0x71, 0x23, 0xC0, 0x8F
};
static const uint8_t data_117FD[16] = {
  0xC2, 0xF5, 0x3C, 0x74, 0x6F, 0x6B, 0x65, 0x00, 0x00, 0x00, 0x20, 0x66, 0x66, 0x06, 0x40, 0x19
};
static const uint8_t data_1180D[16] = {
  0x04, 0x56, 0x3F, 0x76, 0x71, 0x23, 0xC0, 0xCD, 0xCC, 0x4C, 0x3E, 0x74, 0x75, 0x62, 0x61, 0x00
};
static const uint8_t data_1181D[16] = {
  0x00, 0x00, 0x20, 0x66, 0x66, 0x06, 0x40, 0x52, 0xB8, 0x5E, 0x3F, 0x9C, 0xA2, 0x23, 0xC0, 0x33
};
static const uint8_t data_1182D[16] = {
  0x33, 0x33, 0x3F, 0x72, 0x6F, 0x75, 0x5F, 0x00, 0x00, 0x00, 0x20, 0x66, 0x66, 0x06, 0x40, 0x73
};
static const uint8_t data_1183D[16] = {
  0x68, 0x51, 0x3F, 0x76, 0x71, 0x23, 0xC0, 0xCD, 0xCC, 0xCC, 0x3E, 0x73, 0x61, 0x72, 0x61, 0x00
};
static const uint8_t data_1184D[16] = {
  0x00, 0x00, 0x20, 0x66, 0x66, 0x06, 0x40, 0x08, 0xAC, 0x5C, 0x3F, 0x76, 0x71, 0x23, 0xC0, 0x3D
};
static const uint8_t data_1185D[16] = {
  0x0A, 0x57, 0x3E, 0x6F, 0x6E, 0x6F, 0x69, 0x00, 0x00, 0x00, 0x20, 0x50, 0x8D, 0x07, 0x40, 0x19
};
static const uint8_t data_1186D[16] = {
  0x04, 0x56, 0x3F, 0x2A, 0x3A, 0x22, 0xC0, 0xAE, 0x47, 0xE1, 0x3D, 0x6C, 0x6F, 0x67, 0x6F, 0x00
};
static const uint8_t data_1187D[16] = {
  0x00, 0x00, 0x20, 0x66, 0x66, 0x06, 0x40, 0x3F, 0x35, 0x5E, 0x3F, 0x76, 0x71, 0x23, 0xC0, 0xCD
};
static const uint8_t data_1188D[7] = {
  0xCC, 0xCC, 0x3E, 0xFF, 0xFF, 0xFF, 0xFF
};
static const char data_11894[10] __attribute__((scn_string_literal)) = "E1072B089";
static const char data_1189E[10] __attribute__((scn_string_literal)) = "E1072B090";
static const char data_118A8[10] __attribute__((scn_string_literal)) = "E1072B091";
static const char data_118B2[10] __attribute__((scn_string_literal)) = "E1072A015";
static const char data_118BC[10] __attribute__((scn_string_literal)) = "E1072B006";
static const char data_118C6[10] __attribute__((scn_string_literal)) = "E1072A020";
static const char data_118D0[10] __attribute__((scn_string_literal)) = "E1072B006";
static const char data_118DA[10] __attribute__((scn_string_literal)) = "E1072B025";
static const char data_118E4[10] __attribute__((scn_string_literal)) = "E1072B026";
static const char data_118EE[10] __attribute__((scn_string_literal)) = "E1072B027";
static const char data_118F8[10] __attribute__((scn_string_literal)) = "E1072B009";
static const char data_11902[10] __attribute__((scn_string_literal)) = "E1072B024";
static const char data_1190C[10] __attribute__((scn_string_literal)) = "E1072B010";
static const char data_11916[10] __attribute__((scn_string_literal)) = "E1072A001";
static const char data_11920[10] __attribute__((scn_string_literal)) = "E1072A002";
static const char data_1192A[10] __attribute__((scn_string_literal)) = "E1072B001";
static const char data_11934[10] __attribute__((scn_string_literal)) = "E1072B002";
static const char data_1193E[10] __attribute__((scn_string_literal)) = "E1072A003";
static const char data_11948[10] __attribute__((scn_string_literal)) = "E1072A004";
static const char data_11952[10] __attribute__((scn_string_literal)) = "E1072B003";
static const char data_1195C[10] __attribute__((scn_string_literal)) = "E1072B088";
static const char data_11966[10] __attribute__((scn_string_literal)) = "E1072A001";
static const char data_11970[10] __attribute__((scn_string_literal)) = "E1072B059";
static const char data_1197A[10] __attribute__((scn_string_literal)) = "E1072A005";
static const char data_11984[10] __attribute__((scn_string_literal)) = "E1072B005";
static const char data_1198E[10] __attribute__((scn_string_literal)) = "E1072B006";
static const char data_11998[10] __attribute__((scn_string_literal)) = "E1072A006";
static const char data_119A2[10] __attribute__((scn_string_literal)) = "E1072A008";
static const char data_119AC[10] __attribute__((scn_string_literal)) = "E1072B060";
static const char data_119B6[10] __attribute__((scn_string_literal)) = "E1072A009";
static const char data_119C0[10] __attribute__((scn_string_literal)) = "E1072A017";
static const char data_119CA[10] __attribute__((scn_string_literal)) = "E1072A018";
static const char data_119D4[10] __attribute__((scn_string_literal)) = "E1072B009";
static const char data_119DE[10] __attribute__((scn_string_literal)) = "E1072B024";
static const char data_119E8[10] __attribute__((scn_string_literal)) = "E1072B010";
static const char data_119F2[10] __attribute__((scn_string_literal)) = "E1072B011";
static const char data_119FC[10] __attribute__((scn_string_literal)) = "E1072B022";
static const char data_11A06[10] __attribute__((scn_string_literal)) = "E1072A012";
static const char data_11A10[10] __attribute__((scn_string_literal)) = "E1072B059";
static const char data_11A1A[10] __attribute__((scn_string_literal)) = "E1072A019";
static const char data_11A24[10] __attribute__((scn_string_literal)) = "E1072B005";
static const char data_11A2E[10] __attribute__((scn_string_literal)) = "E1072B006";
static const char data_11A38[10] __attribute__((scn_string_literal)) = "E1072A006";
static const char data_11A42[10] __attribute__((scn_string_literal)) = "E1072A005";
static const char data_11A4C[10] __attribute__((scn_string_literal)) = "E1072B005";
static const char data_11A56[31] __attribute__((scn_string_literal)) = "●赤先ほど占ったばかりですが…";
static const char data_11A75[27] __attribute__((scn_string_literal)) = "●赤もう一度、占いますか？";
static const char data_11A90[28] = "E1072A012\000\000\000,\004\023\004-\004/\0042\0040\0041\004\207\003";
static const char data_11AAC[20] __attribute__((scn_string_literal)) = "TUNED FreeTalk def\012";
static const char data_11AC0[20] __attribute__((scn_string_literal)) = "TUNED FreeTalk def\012";
static const char data_11AD4[6] __attribute__((scn_string_literal)) = "E1072";
static const char data_11ADA[11] __attribute__((scn_string_literal)) = "M_MOBJ.BIN";
static const char data_11AE5[14] __attribute__((scn_string_literal)) = "scene/01/D000";
static const char data_11AF3[] __attribute__((scn_string_literal)) = "CYCLEMAN.BIN.BIN";
static const char data_11B04[10] __attribute__((scn_string_literal)) = "E1072B055";
static const char data_11B0E[10] __attribute__((scn_string_literal)) = "E1072B054";
