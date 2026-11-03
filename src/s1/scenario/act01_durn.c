#include "act01_durn.h"

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

static void script_0030(int32_t arg0, float arg1, float arg2, float arg3) {
  Vector3 local_F4;
  local_F4.x = arg1;
  (&local_F4)->y = arg2;
  (&local_F4)->z = arg3;
  Script_EVT_SetPosition(arg0, POS_XYZ, &local_F4);
  return;
}

static void script_0058(int32_t arg0, float arg1, float arg2, float arg3) {
  Vector3i local_F4;
  local_F4.x = (int32_t)(arg1 / 360.0f * (float)0x10000);
  (&local_F4)->y = (int32_t)(arg2 / 360.0f * (float)0x10000);
  (&local_F4)->z = (int32_t)(arg3 / 360.0f * (float)0x10000);
  Script_EVT_SetRotation(arg0, 0x38000000, &local_F4, 0);
  return;
}

static void script_00AC(int32_t arg0, int32_t arg1, int32_t arg2) {
  Vector3 local_F4;
  for (;;) {
    if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0) & Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg1)) {
      Script_EVT_GetPosition(arg1, 4, &local_F4, ENTITY_XFORM_WORLD_SPACE);
      Script_SetLookPointDetails(arg0, 16, &local_F4, arg2);
      Script_FaceLookAtTargetUtils(arg0, 2, &local_F4, 30);
    }
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static int32_t script_00FC(int32_t arg0) {
  int32_t unused_FC;
  GameDateTime local_F4;
  int8_t local_E8[12];
  int32_t local_E4;
  uint8_t unused_E3;
  int8_t local_E2;
  uint8_t unused_E0[2];
  Script_ReturnTimeDate(&local_F4);
  local_E2 = Script_scenUtils(SCENARIO_CMD_GET_STEP_LUT, arg0);
  local_E8[1] = (local_E2 >> 5) & 7;
  if (local_E8[1] == 0) {
    local_E8[1] = 12;
  }
  local_E8[2] = local_E2 & 0x1F;
  if ((&local_F4)->month == local_E8[1]) {
    local_E4 = (&local_F4)->day - local_E8[2];
  } else {
    if (local_E8[1] == 12) {
      local_E8[1] = 0;
    }
    local_E4 = ((&local_F4)->month - local_E8[1]) * 31 + (&local_F4)->day - local_E8[2];
  }
  return local_E4;
}

static void script_01A3(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  local_FC = script_00FC(arg0);
  if (arg1 > local_FC) {
    (void)0;
  } else {
    (void)-1;
  }
  return;
}

static int32_t script_01C6(int32_t arg0) {
  int32_t unused_FC;
  GameDateTime local_F4;
  int32_t local_F0;
  int32_t local_EC;
  Script_ReturnTimeDate(&local_F4);
  local_F0 = (&local_F4)->hours * 60 + (&local_F4)->minutes;
  local_EC = Script_scenUtils(SCENARIO_CMD_GET_STEP_LUT, arg0) * 6;
  return local_F0 - local_EC;
}

static void script_01F9(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  local_FC = script_01C6(arg0);
  if (arg1 > local_FC) {
    (void)0;
  } else {
    (void)-1;
  }
  return;
}

static void script_021C(int32_t arg0, int32_t arg1) {
  int8_t local_F4[12];
  int32_t local_F0;
  local_F0 = script_01C6(arg0);
  local_F4[4] = local_F0 / 60;
  local_F4[5] = local_F0 % 60;
  if ((local_F4[4] < *(int8_t *)(arg1 + 4)) | ((local_F4[4] == *(int8_t *)(arg1 + 4)) & (local_F4[5] < *(int8_t *)(arg1 + 5)))) {
    (void)0;
  } else {
    (void)-1;
  }
  return;
}

static void script_027B(int32_t arg0) {
  int32_t local_F8[2];
  local_F8[0] = arg0;
  local_F8[1] = 0;
  (void)local_F8;
  return;
}

static int32_t script_0290(int32_t arg0) {
  if (arg0 < 0) {
    return -arg0;
  } else {
    return arg0;
  }
}

static void script_02AE(float arg0) {
  if (!(arg0 >= 0.0f)) {
    (void)((float)0 - arg0);
    return;
  } else {
    (void)arg0;
    return;
  }
}

static int32_t script_02CD(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  local_FC = arg1 - arg0;
  local_F8 = arg0 - arg1;
  if ((local_F8 & 0xFFFF) < (local_FC & 0xFFFF)) {
    if (arg0 > arg1) {
      local_F4 = local_F8 * -1;
    } else {
      local_F4 = (local_F8 & 0xFFFF) * -1;
    }
  } else if (arg0 > arg1) {
    local_F4 = local_FC & 0xFFFF;
  } else {
    local_F4 = local_FC & 0xFFFF;
  }
  return local_F4;
}

static void script_0345(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  int32_t local_FC;
  if (task_1CC != arg0) {
    Script_sprintf("Door Index = %d : call_type = %d : type = %d\012", task_B4[arg2], arg3, arg4);
    local_FC = arg1;
    switch (local_FC) {
      case 40731:
        Script_sprintf("Door Open Motion = DEB_AKESIME_FUSUMA_L_F\012");
        break;
      case 40732:
        Script_sprintf("Door Open Motion = DEB_AKESIME_FUSUMA_R_F\012");
        break;
      case 40705:
        Script_sprintf("Door Open Motion = DEB_HIKISIME_DOOR_L_F\012");
        break;
      case 40706:
        Script_sprintf("Door Open Motion = DEB_HIKISIME_DOOR_R_F\012");
        break;
      case 40733:
        Script_sprintf("Door Open Motion = DEB_OSISIME_DOOR_L_F\012");
        break;
      case 40707:
        Script_sprintf("Door Open Motion = DEB_OSISIME_DOOR_R_F\012");
        break;
      case 40734:
        Script_sprintf("Door Open Motion = GAK_AKESIME_FUSUMA_L_F\012");
        break;
      case 40735:
        Script_sprintf("Door Open Motion = GAK_AKESIME_FUSUMA_R_F\012");
        break;
      case 40708:
        Script_sprintf("Door Open Motion = GAK_HIKISIME_DOOR_L_F\012");
        break;
      case 40709:
        Script_sprintf("Door Open Motion = GAK_HIKISIME_DOOR_R_F\012");
        break;
      case 40736:
        Script_sprintf("Door Open Motion = GAK_OSISIME_DOOR_L_F\012");
        break;
      case 40710:
        Script_sprintf("Door Open Motion = GAK_OSISIME_DOOR_R_F\012");
        break;
      case 40737:
        Script_sprintf("Door Open Motion = JIJ_AKESIME_FUSUMA_L_F\012");
        break;
      case 40738:
        Script_sprintf("Door Open Motion = JIJ_AKESIME_FUSUMA_R_F\012");
        break;
      case 40711:
        Script_sprintf("Door Open Motion = JIJ_HIKISIME_DOOR_L_F\012");
        break;
      case 40712:
        Script_sprintf("Door Open Motion = JIJ_HIKISIME_DOOR_R_F\012");
        break;
      case 40739:
        Script_sprintf("Door Open Motion = JIJ_OSISIME_DOOR_L_F\012");
        break;
      case 40713:
        Script_sprintf("Door Open Motion = JIJ_OSISIME_DOOR_R_F\012");
        break;
      case 40740:
        Script_sprintf("Door Open Motion = KOD_AKESIME_FUSUMA_L_F\012");
        break;
      case 40741:
        Script_sprintf("Door Open Motion = KOD_AKESIME_FUSUMA_R_F\012");
        break;
      case 40714:
        Script_sprintf("Door Open Motion = KOD_HIKISIME_DOOR_L_F\012");
        break;
      case 40715:
        Script_sprintf("Door Open Motion = KOD_HIKISIME_DOOR_R_F\012");
        break;
      case 40742:
        Script_sprintf("Door Open Motion = KOD_OSISIME_DOOR_L_F\012");
        break;
      case 40716:
        Script_sprintf("Door Open Motion = KOD_OSISIME_DOOR_R_F\012");
        break;
      case 40743:
        Script_sprintf("Door Open Motion = OTH_AKESIME_FUSUMA_L_F\012");
        break;
      case 40744:
        Script_sprintf("Door Open Motion = OTH_AKESIME_FUSUMA_R_F\012");
        break;
      case 40717:
        Script_sprintf("Door Open Motion = OTH_HIKISIME_DOOR_L_F\012");
        break;
      case 40718:
        Script_sprintf("Door Open Motion = OTH_HIKISIME_DOOR_R_F\012");
        break;
      case 40745:
        Script_sprintf("Door Open Motion = OTH_OSISIME_DOOR_L_F\012");
        break;
      case 40719:
        Script_sprintf("Door Open Motion = OTH_OSISIME_DOOR_R_F\012");
        break;
      case 40746:
        Script_sprintf("Door Open Motion = PNW_AKESIME_FUSUMA_L_F\012");
        break;
      case 40747:
        Script_sprintf("Door Open Motion = PNW_AKESIME_FUSUMA_R_F\012");
        break;
      case 40720:
        Script_sprintf("Door Open Motion = PNW_HIKISIME_DOOR_L_F\012");
        break;
      case 40721:
        Script_sprintf("Door Open Motion = PNW_HIKISIME_DOOR_R_F\012");
        break;
      case 40748:
        Script_sprintf("Door Open Motion = PNW_OSISIME_DOOR_L_F\012");
        break;
      case 40722:
        Script_sprintf("Door Open Motion = PNW_OSISIME_DOOR_R_F\012");
        break;
      case 40749:
        Script_sprintf("Door Open Motion = SIN_AKESIME_FUSUMA_L_F\012");
        break;
      case 40750:
        Script_sprintf("Door Open Motion = SIN_AKESIME_FUSUMA_R_F\012");
        break;
      case 40723:
        Script_sprintf("Door Open Motion = SIN_HIKISIME_DOOR_L_F\012");
        break;
      case 40724:
        Script_sprintf("Door Open Motion = SIN_HIKISIME_DOOR_R_F\012");
        break;
      case 40751:
        Script_sprintf("Door Open Motion = SIN_OSISIME_DOOR_L_F\012");
        break;
      case 40725:
        Script_sprintf("Door Open Motion = SIN_OSISIME_DOOR_R_F\012");
        break;
      case 40757:
        Script_sprintf("Door Open Motion = SYP_AKESIME_FUSUMA_L_F\012");
        break;
      case 40752:
        Script_sprintf("Door Open Motion = SYP_AKESIME_FUSUMA_R_F\012");
        break;
      case 40726:
        Script_sprintf("Door Open Motion = SYP_HIKISIME_DOOR_L_F\012");
        break;
      case 40727:
        Script_sprintf("Door Open Motion = SYP_HIKISIME_DOOR_R_F\012");
        break;
      case 40753:
        Script_sprintf("Door Open Motion = SYP_OSISIME_DOOR_L_F\012");
        break;
      case 40728:
        Script_sprintf("Door Open Motion = SYP_OSISIME_DOOR_R_F\012");
        break;
      case 40758:
        Script_sprintf("Door Open Motion = YKI_AKESIME_FUSUMA_L_F\012");
        break;
      case 40754:
        Script_sprintf("Door Open Motion = YKI_AKESIME_FUSUMA_R_F\012");
        break;
      case 40729:
        Script_sprintf("Door Open Motion = YKI_HIKISIME_DOOR_L_F\012");
        break;
      case 40730:
        Script_sprintf("Door Open Motion = YKI_HIKISIME_DOOR_R_F\012");
        break;
      case 40755:
        Script_sprintf("Door Open Motion = YKI_OSISIME_DOOR_L_F\012");
        break;
      case 40756:
        Script_sprintf("Door Open Motion = YKI_OSISIME_DOOR_R_F\012");
        break;
      case 40759:
        Script_sprintf("Door Open Motion = JIJ_OSISIME_DOOR_AP_L_F\012");
        break;
      case 40760:
        Script_sprintf("Door Open Motion = OTH_OSISIME_DOOR_AP_L_F\012");
        break;
      case 40761:
        Script_sprintf("Door Open Motion = PNW_OSISIME_DOOR_AP_L_F\012");
        break;
      case 40762:
        Script_sprintf("Door Open Motion = SYP_OSISIME_DOOR_AP_L_F\012");
        break;
      case 40763:
        Script_sprintf("Door Open Motion = YKI_OSISIME_DOOR_AP_L_F\012");
        break;
    }
  }
  return;
}

static int32_t script_08F7(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  local_F8 = arg0;
  if (local_F8 == 0) {
    local_FC = 594;
  } else if (local_F8 == 1) {
    local_FC = 595;
  } else if (local_F8 == 2) {
    local_FC = 596;
  } else if (local_F8 == 3) {
    local_FC = 598;
  } else if (local_F8 == 4) {
    local_FC = 599;
  } else if (local_F8 == 5) {
    local_FC = 600;
  } else if (local_F8 == 6) {
    local_FC = 1000;
  } else if (local_F8 == 7) {
    local_FC = 656;
  } else if (local_F8 == 8) {
    local_FC = 601;
  } else if (local_F8 == 9) {
    local_FC = 594;
  } else {
    local_FC = 594;
  }
  return local_FC;
}

static void script_09C8(int32_t arg0, int32_t arg1, int32_t arg2) {
  script_96EB(arg0, arg2, 0);
  return;
}

static void script_09D8(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  float local_F8;
  float local_F4;
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
    local_FC = script_8E80(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
    if ((local_FC == 1) | (local_FC == 2)) {
      local_FC = 1;
    } else {
      local_FC = 0;
    }
    *(int32_t *)&local_F8 = Script_ScenDataUtil(data_F778, 5, Script_ScenDataUtil(data_F188, 1, arg3 * 40 + task_104[arg2] * 4 + local_FC) * 9 + 5);
    *(int32_t *)&local_F4 = Script_ScenDataUtil(data_F778, 5, Script_ScenDataUtil(data_F188, 1, arg3 * 40 + task_104[arg2] * 4 + local_FC) * 9 + 6);
    local_FC = script_8E80(arg0, arg1, 5, local_F8 / 100.0f, local_F4 / 100.0f, arg3, -1);
    script_4164(arg0, arg1, arg2, arg3, local_FC, 0);
  }
  script_96EB(arg0, arg2, 0);
  return;
}

static void script_0ADC(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  int32_t local_F8;
  float local_F4;
  float local_F0;
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
    local_FC = script_8E80(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
    *(int32_t *)&local_F4 = Script_ScenDataUtil(data_F778, 5, Script_ScenDataUtil(data_F188, 1, arg3 * 40 + task_104[arg2] * 4 + local_FC) * 9 + 5);
    local_F4 = 90.0f - local_F4;
    *(int32_t *)&local_F0 = Script_ScenDataUtil(data_F778, 5, Script_ScenDataUtil(data_F188, 1, arg3 * 40 + task_104[arg2] * 4 + local_FC) * 9 + 6);
    if ((Script_ScenDataUtil(task_6C, 4, task_B4[arg2] * 13 + 1) & 0x40) | (Script_ScenDataUtil(task_6C, 4, task_B4[arg2] * 13 + 1) & 0x80)) {
      if (Script_ScenDataUtil(task_6C, 4, task_B4[arg2] * 13 + 1) & 0x40) {
        if ((local_FC == 0) | (local_FC == 1)) {
          local_F8 = 6;
        } else {
          local_F8 = 4;
        }
      } else if ((local_FC == 2) | (local_FC == 3)) {
        local_F8 = 6;
      } else {
        local_F8 = 4;
      }
    } else {
      local_F8 = 4;
    }
    local_FC = script_8E80(arg0, arg1, local_F8, local_F4 / (float)100, local_F0 / (float)100, arg3, task_B4[arg2]);
    script_4164(arg0, arg1, arg2, arg3, local_FC, 1);
  }
  script_96EB(arg0, arg2, 0);
  return;
}

static void script_0C7E(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  float local_F8;
  float local_F4;
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
    local_FC = script_8E80(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
    if ((local_FC == 1) | (local_FC == 2)) {
      local_FC = 1;
    } else {
      local_FC = 0;
    }
    *(int32_t *)&local_F8 = Script_ScenDataUtil(data_F778, 5, Script_ScenDataUtil(data_F188, 1, arg3 * 40 + task_104[arg2] * 4 + local_FC) * 9 + 5);
    *(int32_t *)&local_F4 = Script_ScenDataUtil(data_F778, 5, Script_ScenDataUtil(data_F188, 1, arg3 * 40 + task_104[arg2] * 4 + local_FC) * 9 + 6);
    local_FC = script_8E80(arg0, arg1, 5, local_F8 / 100.0f, local_F4 / 100.0f, arg3, -1);
    if (local_FC == 1) {
      local_FC = 2;
    } else {
      local_FC = 3;
    }
    if ((Script_ScenDataUtil(task_6C, 4, task_B4[arg2] * 13 + 1) & 4) == 0) {
      script_2671(arg0, arg1, arg2, arg3, local_FC, 0);
    } else {
      if (arg0 == task_1CC) {
        script_5A44(arg0, arg1, arg2, arg3, local_FC, 0);
      }
    }
  }
  script_96EB(arg0, arg2, 2);
  return;
}

static void script_0DDF(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  float local_F8;
  float local_F4;
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
    local_FC = script_8E80(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
    if ((local_FC == 1) | (local_FC == 2)) {
      local_FC = 1;
    } else {
      local_FC = 0;
    }
    *(int32_t *)&local_F8 = Script_ScenDataUtil(data_F778, 5, Script_ScenDataUtil(data_F188, 1, arg3 * 40 + task_104[arg2] * 4 + local_FC) * 9 + 5);
    *(int32_t *)&local_F4 = Script_ScenDataUtil(data_F778, 5, Script_ScenDataUtil(data_F188, 1, arg3 * 40 + task_104[arg2] * 4 + local_FC) * 9 + 6);
    local_FC = script_8E80(arg0, arg1, 5, local_F8 / 100.0f, local_F4 / 100.0f, arg3, -1);
    if (local_FC == 1) {
      local_FC = 1;
    } else {
      local_FC = 0;
    }
    if ((Script_ScenDataUtil(task_6C, 4, task_B4[arg2] * 13 + 1) & 4) == 0) {
      script_2671(arg0, arg1, arg2, arg3, local_FC, 0);
    } else {
      if (arg0 == task_1CC) {
        script_5A44(arg0, arg1, arg2, arg3, local_FC, 0);
      }
    }
  }
  script_96EB(arg0, arg2, 2);
  return;
}

static void script_0F40(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  int32_t local_F8;
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
    if ((Script_ScenDataUtil(task_6C, 4, task_B4[arg2] * 13 + 1) & 4) == 0) {
      float local_F4;
      float local_F0;
      local_FC = script_8E80(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
      *(int32_t *)&local_F4 = Script_ScenDataUtil(data_F778, 5, Script_ScenDataUtil(data_F188, 1, arg3 * 40 + task_104[arg2] * 4 + local_FC) * 9 + 5);
      *(int32_t *)&local_F0 = Script_ScenDataUtil(data_F778, 5, Script_ScenDataUtil(data_F188, 1, arg3 * 40 + task_104[arg2] * 4 + local_FC) * 9 + 6);
      if ((Script_ScenDataUtil(task_6C, 4, task_B4[arg2] * 13 + 1) & 0x40) | (Script_ScenDataUtil(task_6C, 4, task_B4[arg2] * 13 + 1) & 0x80)) {
        if (Script_ScenDataUtil(task_6C, 4, task_B4[arg2] * 13 + 1) & 0x40) {
          if ((local_FC == 0) | (local_FC == 1)) {
            local_F8 = 6;
          } else {
            local_F8 = 4;
          }
        } else if ((local_FC == 2) | (local_FC == 3)) {
          local_F8 = 6;
        } else {
          local_F8 = 4;
        }
      } else {
        local_F8 = 4;
      }
      local_FC = script_8E80(arg0, arg1, local_F8, local_F4 / 100.0f, local_F0 / 100.0f, arg3, task_B4[arg2]);
      script_2671(arg0, arg1, arg2, arg3, local_FC, 1);
    } else {
      if (arg0 == task_1CC) {
        float local_F4;
        float local_F0;
        local_FC = script_8E80(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
        *(int32_t *)&local_F4 = Script_ScenDataUtil(data_F778, 5, Script_ScenDataUtil(data_F188, 1, arg3 * 40 + task_104[arg2] * 4 + local_FC) * 9 + 5);
        *(int32_t *)&local_F0 = Script_ScenDataUtil(data_F778, 5, Script_ScenDataUtil(data_F188, 1, arg3 * 40 + task_104[arg2] * 4 + local_FC) * 9 + 6);
        local_FC = script_8E80(arg0, arg1, 6, local_F4 / 100.0f, local_F0 / 100.0f, arg3, task_B4[arg2]);
        script_5A44(arg0, arg1, arg2, arg3, local_FC, 1);
      }
    }
  }
  script_96EB(arg0, arg2, 2);
  return;
}

static void script_11DE(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  float local_F8;
  float local_F4;
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
    local_FC = script_8E80(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
    *(int32_t *)&local_F8 = Script_ScenDataUtil(data_F778, 5, Script_ScenDataUtil(data_F188, 1, arg3 * 40 + task_104[arg2] * 4 + local_FC) * 9 + 5);
    *(int32_t *)&local_F4 = Script_ScenDataUtil(data_F778, 5, Script_ScenDataUtil(data_F188, 1, arg3 * 40 + task_104[arg2] * 4 + local_FC) * 9 + 6);
    script_8E80(arg0, arg1, 0, local_F8 / 100.0f, local_F4 / 100.0f, arg3, -1);
  }
  script_96EB(arg0, arg2, 0);
  return;
}

static void script_12AC(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
    local_FC = script_8E80(arg0, arg1, 4, 0.5f, 0.5f, arg3, -1);
    if (local_FC == 1) {
      local_FC = 2;
    } else {
      local_FC = 3;
    }
    script_35F0(arg0, arg1, arg2, arg3, local_FC, 1);
  }
  script_96EB(arg0, arg2, 1);
  return;
}

static void script_1313(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
    local_FC = script_8E80(arg0, arg1, 4, 0.5f, 0.5f, arg3, -1);
    if (local_FC == 1) {
      local_FC = 1;
    } else {
      local_FC = 0;
    }
    script_35F0(arg0, arg1, arg2, arg3, local_FC, 1);
  }
  script_96EB(arg0, arg2, 1);
  return;
}

static void script_137A(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
    script_8E80(arg0, arg1, 4, 0.0f, 0.5f, arg3, -1);
    script_35F0(arg0, arg1, arg2, arg3, 4, 1);
  }
  script_96EB(arg0, arg2, 1);
  return;
}

static void script_13C1(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  float local_F8;
  float local_F4;
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
    local_FC = script_8E80(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
    if ((local_FC == 1) | (local_FC == 2)) {
      local_FC = 1;
    } else {
      local_FC = 0;
    }
    *(int32_t *)&local_F8 = Script_ScenDataUtil(data_F778, 5, Script_ScenDataUtil(data_F188, 1, arg3 * 40 + task_104[arg2] * 4 + local_FC) * 9 + 5);
    *(int32_t *)&local_F4 = Script_ScenDataUtil(data_F778, 5, Script_ScenDataUtil(data_F188, 1, arg3 * 40 + task_104[arg2] * 4 + local_FC) * 9 + 6);
    if (local_FC == 0) {
      local_FC = script_8E80(arg0, arg1, 5, local_F8 / 100.0f, local_F4 / 100.0f, arg3, -1);
    } else {
      local_FC = script_8E80(arg0, arg1, 5, local_F8 / 100.0f, local_F4 / 100.0f, arg3, -1);
    }
    script_3AAA(arg0, arg1, arg2, arg3, local_FC, 0);
  }
  script_96EB(arg0, arg2, 2);
  return;
}

static void script_14F7(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  Vector3i local_F0;
  Vector3i local_E4;
  Vector3i local_D8;
  Vector3 local_CC;
  Vector3 local_C0;
  Vector3 local_B4;
  Vector3 local_A8;
  Vector3 local_9C;
  float local_98;
  float local_94;
  float local_90;
  float local_8C;
  int32_t local_88;
  int32_t local_84;
  int32_t unused_80;
  int8_t local_FF7F;
  uint8_t unused_FF7C[3];
  float local_FF78;
  float local_FF74;
  if (arg0 == task_1CC) {
    if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
      local_FF7F = script_8E80(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
      *(int32_t *)&local_FF78 = Script_ScenDataUtil(data_F778, 5, Script_ScenDataUtil(data_F188, 1, arg3 * 40 + task_104[arg2] * 4 + local_FF7F) * 9 + 5);
      *(int32_t *)&local_FF74 = Script_ScenDataUtil(data_F778, 5, Script_ScenDataUtil(data_F188, 1, arg3 * 40 + task_104[arg2] * 4 + local_FF7F) * 9 + 6);
      local_FF7F = script_8E80(arg0, arg1, 5, local_FF78 / (float)100, local_FF74 / (float)100, arg3, -1);
      script_9549(arg0, 0);
      *(int32_t *)&local_94 = Script_ScenDataUtil(data_F778, 5, Script_ScenDataUtil(data_F188, 1, arg3 * 40 + task_104[arg2] * 4 + local_FF7F) * 9 + 1);
      *(int32_t *)&local_90 = Script_ScenDataUtil(data_F778, 5, Script_ScenDataUtil(data_F188, 1, arg3 * 40 + task_104[arg2] * 4 + local_FF7F) * 9 + 2);
      local_8C = 0.0f;
      local_FC = Script_ScenDataUtil(data_F778, 4, Script_ScenDataUtil(data_F188, 1, arg3 * 40 + task_104[arg2] * 4 + local_FF7F) * 9);
      local_FC = local_FC | ((Script_FloatUtils(ScriptMathOp_FloatToInt, local_94 - 3.0f) << 16) & 0x7FFF0000);
      local_9C.x = (&task_1D4)->x;
      (&local_9C)->y = task_1D4.y;
      (&local_9C)->z = task_1D4.z;
      Script_charaPosRotUtils(arg1, 12, &local_CC, &local_E4, 2);
      Script_charaPosRotUtils(arg1, 7, &local_C0, &local_D8, 2);
      Script_ryoControl(PCCTL_StartCameraFacingInterpolation, 0);
      if (arg0 == task_1CC) {
        Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
        Script_scenCharaSubmitMoveTargetRequest(arg0, &task_1D4, *(&task_1E0 + 1), local_FC, 0);
        for (;;) {
          if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0) == 0) {
            break;
          }
          if (Script_IsMoveTargetRequestActive(arg0) == 0) {
            break;
          }
          SCNF_WaitFrames_0000(1);
        }
      } else {
        Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
        Script_SetCharaMoveTarget(arg0, &task_1D4, *(&task_1E0 + 1), local_FC);
        for (;;) {
          if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0) == 0) {
            break;
          }
          if (Script_GetCharaMoveStatus(arg0) == 0) {
            break;
          }
          SCNF_WaitFrames_0000(1);
        }
      }
      for (;;) {
        if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0) == 0) {
          break;
        }
        if (Script_IsCharaMotionFinished(arg0)) {
          break;
        }
        local_98 = Script_GetCharaMotionRemainingFramesScaled(arg0);
        Script_ReadMOTNPositionAtFrame(arg0, local_FC, local_98, &local_A8);
        if (!(local_98 < local_94) & !(local_98 > local_90)) {
          if (local_FF7F == 1) {
            if (!(local_98 < local_94) & !(local_98 >= local_94 + 1.0f)) {
              int32_t unused_FF70;
              int32_t local_FF6C;
              local_FF6C = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_B4[arg2], 2);
              local_FF6C = Script_ScenDataUtil(task_74, 4, Script_ScenDataUtil(task_78, 4, local_FF6C * 9 + 1) * 4 + 2);
              if (local_FF6C != -1) {
                if (arg0 == task_1CC) {
                  Script_executeAicaCommand(local_FF6C, 0, 0);
                } else {
                  Script_Execute3DSoundCommand_Wrapper(local_FF6C, 4, &local_9C, 5.0f, 20.0f, 0);
                }
              }
            }
            Script_EVT_GetPosition(arg0, 12, &local_B4, ENTITY_XFORM_MOTION_SPACE);
            (&local_B4)->z = (&local_B4)->z + (&local_A8)->z;
            (&local_F0)->y = Script_FloatUtils(ScriptMathOp_Atan2ReverseVector, (&local_C0)->x - (&local_B4)->x, (&local_C0)->z - (&local_B4)->z);
            (&local_F0)->y = (&local_F0)->y - (int32_t)((local_8C + 95.0f) / 360.0f * (float)0x10000);
            local_F0.x = (&local_D8)->x;
            (&local_F0)->z = (&local_D8)->z;
            Script_charaPosRotUtils(arg1, 7, 0, &local_F0, 0);
            Script_EVT_GetPosition(arg0, 18, &local_B4, ENTITY_XFORM_MOTION_SPACE);
            (&local_B4)->z = (&local_B4)->z + (&local_A8)->z;
            (&local_F0)->y = Script_FloatUtils(ScriptMathOp_Atan2ReverseVector, (&local_CC)->x - (&local_B4)->x, (&local_CC)->z - (&local_B4)->z);
            (&local_F0)->y = (&local_F0)->y + (int32_t)((local_8C + 95.0f) / 360.0f * (float)0x10000);
            local_F0.x = (&local_E4)->x;
            (&local_F0)->z = (&local_E4)->z;
            Script_charaPosRotUtils(arg1, 12, 0, &local_F0, 0);
          } else {
            if (!(local_98 < local_94) & !(local_98 >= local_94 + 1.0f)) {
              int32_t unused_FF70;
              int32_t local_FF6C;
              local_FF6C = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_B4[arg2], 2);
              local_FF6C = Script_ScenDataUtil(task_74, 4, Script_ScenDataUtil(task_78, 4, local_FF6C * 9 + 1) * 4 + 2);
              if (local_FF6C != -1) {
                if (arg0 == task_1CC) {
                  Script_executeAicaCommand(local_FF6C, 0, 0);
                } else {
                  Script_Execute3DSoundCommand_Wrapper(local_FF6C, 4, &local_9C, 5.0f, 20.0f, 0);
                }
              }
            }
            Script_EVT_GetPosition(arg0, 12, &local_B4, ENTITY_XFORM_MOTION_SPACE);
            (&local_B4)->z = (&local_B4)->z + (&local_A8)->z;
            (&local_F0)->y = Script_FloatUtils(ScriptMathOp_Atan2ReverseVector, (&local_C0)->x - (&local_B4)->x, (&local_C0)->z - (&local_B4)->z);
            (&local_F0)->y = -((&local_F0)->y - (int32_t)((local_8C + 95.0f) / 360.0f * (float)0x10000));
            local_F0.x = (&local_D8)->x;
            (&local_F0)->z = (&local_D8)->z;
            Script_charaPosRotUtils(arg1, 7, 0, &local_F0, 0);
            Script_EVT_GetPosition(arg0, 18, &local_B4, ENTITY_XFORM_MOTION_SPACE);
            (&local_B4)->z = (&local_B4)->z + (&local_A8)->z;
            (&local_F0)->y = Script_FloatUtils(ScriptMathOp_Atan2ReverseVector, (&local_CC)->x - (&local_B4)->x, (&local_CC)->z - (&local_B4)->z);
            (&local_F0)->y = -((&local_F0)->y + (int32_t)((local_8C + 95.0f) / 360.0f * (float)0x10000));
            local_F0.x = (&local_E4)->x;
            (&local_F0)->z = (&local_E4)->z;
            Script_charaPosRotUtils(arg1, 12, 0, &local_F0, 0);
          }
        }
        if (!(local_98 < local_90 + 10.0f)) {
          break;
        }
        SCNF_WaitFrames_0000(1);
      }
      script_9549(arg0, 1);
      local_8C = Script_MathUtils(SCRIPT_MATH_INT_TO_FLOAT, (&local_F0)->y);
      local_8C = local_8C / 182.044449f;
      if (!(local_8C <= 180.0f)) {
        local_8C = 360.0f - local_8C;
      }
      if (!(local_8C >= -180.0f)) {
        local_8C = local_8C + 360.0f;
      }
      local_88 = 0x4000;
      local_84 = 0;
      local_90 = 1.0f;
      local_FC = 0;
      {
        int32_t unused_FF70;
        int32_t local_FF6C;
        local_FF6C = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_B4[arg2], 2);
        local_FF6C = Script_ScenDataUtil(task_74, 4, Script_ScenDataUtil(task_78, 4, local_FF6C * 9 + 1) * 4 + 3);
        if (local_FF6C != -1) {
          if (arg0 == task_1CC) {
            Script_executeAicaCommand(local_FF6C, 0, 0);
          } else {
            Script_Execute3DSoundCommand_Wrapper(local_FF6C, 4, &local_9C, 5.0f, 20.0f, 0);
          }
        }
      }
      for (;;) {
        local_94 = local_8C * Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, local_88) * local_90;
        local_90 = Script_MathUtils(SCRIPT_MATH_COS_FIXED16, local_84);
        if (local_FC == 0) {
          local_88 = local_88 + 0x200;
          if (local_88 >= 0xC000) {
            local_FC = 1;
          }
        } else {
          local_88 = local_88 - 0x200;
          if (local_88 <= 0x4000) {
            local_FC = 0;
          }
        }
        local_84 = local_84 + 128;
        if (local_FF7F == 1) {
          (&local_F0)->y = -(int32_t)(((float)0 - local_94) / 360.0f * (float)0x10000);
          Script_charaPosRotUtils(arg1, 7, 0, &local_F0, 0);
          (&local_F0)->y = (int32_t)(((float)0 - local_94) / 360.0f * (float)0x10000);
          Script_charaPosRotUtils(arg1, 12, 0, &local_F0, 0);
        } else {
          (&local_F0)->y = (int32_t)(((float)0 - local_94) / 360.0f * (float)0x10000);
          Script_charaPosRotUtils(arg1, 7, 0, &local_F0, 0);
          (&local_F0)->y = -(int32_t)(((float)0 - local_94) / 360.0f * (float)0x10000);
          Script_charaPosRotUtils(arg1, 12, 0, &local_F0, 0);
        }
        SCNF_WaitFrames_0000(1);
        if (local_84 >= 0x4000) {
          break;
        }
      }
    }
    script_96EB(arg0, arg2, 0);
  } else {
    script_0ADC(arg0, arg1, arg2, arg3);
  }
  return;
}

static void script_1DB8(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  float local_F8;
  int32_t local_F4;
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
    local_FC = script_8E80(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
    *(int32_t *)&local_F8 = Script_ScenDataUtil(data_F778, 5, Script_ScenDataUtil(data_F188, 1, arg3 * 40 + task_104[arg2] * 4 + local_FC) * 9 + 5);
    local_F8 = 98.0f - local_F8;
    local_F4 = Script_ScenDataUtil(data_F778, 5, Script_ScenDataUtil(data_F188, 1, arg3 * 40 + task_104[arg2] * 4 + local_FC) * 9 + 6);
    if ((local_FC == 0) | (local_FC == 3)) {
      local_FC = script_8E80(arg0, arg1, 4, 0.34736f, 0.73184f, arg3, -1);
    } else {
      local_FC = script_8E80(arg0, arg1, 4, 0.4f, 0.712f, arg3, -1);
    }
    script_6165(arg0, arg1, arg2, arg3, local_FC, 0);
  }
  script_96EB(arg0, arg2, 0);
  return;
}

static int32_t script_1ED6(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  int32_t local_F0;
  Vector3 local_E4;
  Vector3i local_D8;
  float local_D4;
  float local_D0;
  float local_CC;
  int32_t unused_C8;
  GameDateTime local_C0;
  int32_t local_BC;
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
    local_F4 = Script_ScenDataUtil(task_6C, 4, task_B4[arg2] * 13 + 6);
    if ((local_F4 == 6) | (local_F4 == 7)) {
      Script_ReturnTimeDate(&local_C0);
      if (((&local_C0)->hours >= 22) | ((&local_C0)->hours <= 4)) {
        local_F0 = 1;
        local_F4 = 2;
      } else {
        local_F0 = 0;
      }
    } else {
      local_F0 = 0;
    }
    local_BC = local_F4;
    if (local_BC == 2) {
      local_BC = arg3;
      if ((local_BC == 17) | (local_BC == 11) | (local_BC == 12)) {
      } else {
        local_FC = script_8E80(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
        local_FC = script_8E80(arg0, arg1, 7, 0.45f, 0.0f, arg3, -1);
        local_BC = arg3;
        if ((local_BC == 2) | (local_BC == 4) | (local_BC == 6) | (local_BC == 8) | (local_BC == 10) | (local_BC == 12) | (local_BC == 24) | (local_BC == 26) | (local_BC == 31) | (local_BC == 33) | (local_BC == 35)) {
          local_D4 = -0.45f;
        } else if ((local_BC == 13) | (local_BC == 14) | (local_BC == 15) | (local_BC == 16) | (local_BC == 17) | (local_BC == 18) | (local_BC == 19) | (local_BC == 20) | (local_BC == 21) | (local_BC == 22) | (local_BC == 23) | (local_BC == 29)) {
          local_BC = local_FC;
          if ((local_BC == 0) | (local_BC == 1)) {
            local_D4 = -0.45f;
          } else {
            if ((local_BC == 2) | (local_BC == 3)) {
              local_D4 = 0.45f;
              goto L002092;
            }
          }
          L002092:;
        } else {
          local_D4 = 0.45f;
        }
        Script_EVT_GetPosition(arg1, -1, &local_E4, 0);
        Script_mtwkGetSetRotation(arg1, &local_D8, 0);
        local_E4.x = (&local_E4)->x + local_D4 * Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_D8)->y & 0xFFFF);
        (&local_E4)->z = (&local_E4)->z - local_D4 * Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_D8)->y & 0xFFFF);
        *(&task_1E0 + 1) = Script_FloatUtils(ScriptMathOp_Atan2ReverseVector, (&local_E4)->x - (&task_1D4)->x, (&local_E4)->z - task_1D4.z);
        script_68EF(arg0, arg1, arg2, arg3, local_FC, local_F0);
      }
    } else if ((local_BC == 3) | (local_BC == 4) | (local_BC == 5) | (local_BC == 6)) {
      *(int32_t *)&local_D0 = Script_ScenDataUtil(&data_10198, 5, Script_ScenDataUtil(task_6C, 4, task_B4[arg2] * 13 + 6) * 5 + 3);
      *(int32_t *)&local_CC = Script_ScenDataUtil(&data_10198, 5, Script_ScenDataUtil(task_6C, 4, task_B4[arg2] * 13 + 6) * 5 + 4);
      local_FC = script_8E80(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
      local_BC = arg3;
      if ((local_BC == 13) | (local_BC == 14) | (local_BC == 15) | (local_BC == 16) | (local_BC == 20) | (local_BC == 23) | (local_BC == 28) | (local_BC == 29)) {
        if ((Script_ScenDataUtil(task_6C, 4, task_B4[arg2] * 13 + 1) & 0x40) | (Script_ScenDataUtil(task_6C, 4, task_B4[arg2] * 13 + 1) & 0x80)) {
          if (Script_ScenDataUtil(task_6C, 4, task_B4[arg2] * 13 + 1) & 0x40) {
            if ((local_FC == 0) | (local_FC == 1)) {
              local_F8 = 6;
            } else {
              local_F8 = 4;
            }
          } else if ((local_FC == 2) | (local_FC == 3)) {
            local_F8 = 6;
          } else {
            local_F8 = 4;
          }
        } else {
          local_F8 = 4;
        }
      } else {
        local_F8 = 5;
      }
      local_FC = script_8E80(arg0, arg1, local_F8, local_D0 / (float)100, local_CC / (float)100, arg3, task_B4[arg2]);
      script_69C7(arg0, arg1, arg2, arg3, local_FC, 0);
    } else {
      if (local_BC == 7) {
        local_FC = script_8E80(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
        local_BC = arg3;
        if ((local_BC == 13) | (local_BC == 14) | (local_BC == 15) | (local_BC == 16) | (local_BC == 20) | (local_BC == 23) | (local_BC == 28) | (local_BC == 29)) {
          if ((Script_ScenDataUtil(task_6C, 4, task_B4[arg2] * 13 + 1) & 0x40) | (Script_ScenDataUtil(task_6C, 4, task_B4[arg2] * 13 + 1) & 0x80)) {
            if (Script_ScenDataUtil(task_6C, 4, task_B4[arg2] * 13 + 1) & 0x40) {
              if ((local_FC == 0) | (local_FC == 1)) {
                local_F8 = 6;
              } else {
                local_F8 = 4;
              }
            } else if ((local_FC == 2) | (local_FC == 3)) {
              local_F8 = 6;
            } else {
              local_F8 = 4;
            }
          } else {
            local_F8 = 4;
          }
        } else {
          local_F8 = 5;
        }
        if (Script_CycleManagerUtils(CYCLE_CMD_IS_DOOR_OR_MOBJ_TYPE2, Script_ScenDataUtil(task_6C, 4, task_B4[arg2] * 13 + 12)) == 1) {
          *(int32_t *)&local_D0 = Script_ScenDataUtil(&data_10198, 5, Script_ScenDataUtil(task_6C, 4, task_B4[arg2] * 13 + 6) * 5 + 3);
          *(int32_t *)&local_CC = Script_ScenDataUtil(&data_10198, 5, Script_ScenDataUtil(task_6C, 4, task_B4[arg2] * 13 + 6) * 5 + 4);
          local_FC = script_8E80(arg0, arg1, local_F8, local_D0 / (float)100, local_CC / (float)100, arg3, task_B4[arg2]);
          Script_scenUtils(SCENARIO_CMD_INIT_SCNT);
          script_6E3E(arg0, arg1, arg2, arg3, local_FC, 0);
          Script_scenUtils(SCENARIO_CMD_UPDATE_TIMER);
        } else {
          *(int32_t *)&local_D0 = Script_ScenDataUtil(&data_10198, 5, Script_ScenDataUtil(task_6C, 4, task_B4[arg2] * 13 + 6) * 5 + 3);
          *(int32_t *)&local_CC = Script_ScenDataUtil(&data_10198, 5, Script_ScenDataUtil(task_6C, 4, task_B4[arg2] * 13 + 6) * 5 + 4);
          local_FC = script_8E80(arg0, arg1, local_F8, local_D0 / (float)100, local_CC / (float)100, arg3, task_B4[arg2]);
          script_69C7(arg0, arg1, arg2, arg3, local_FC, 0);
        }
        goto L00258D;
      }
    }
  }
  L00258D:;
  script_96EB(arg0, arg2, 0);
  end:;
}

static void script_259B(int32_t arg0, int32_t arg1) {
  Vector3 local_F4;
  Vector3i local_E8;
  Script_EVT_GetPosition(arg0, -1, &local_F4, 0);
  Script_mtwkGetSetRotation(arg0, &local_E8, 0x42000000);
  if ((arg1 == 5) | (arg1 == 6)) {
    local_F4.x = (&local_F4)->x - Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_E8)->y) * 0.4f;
    (&local_F4)->z = (&local_F4)->z - Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_E8)->y) * 0.4f;
  } else {
    local_F4.x = (&local_F4)->x - Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_E8)->y) * 0.3f;
    (&local_F4)->z = (&local_F4)->z - Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_E8)->y) * 0.3f;
  }
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
    Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
    Script_SetCharaMoveTarget(arg0, &local_F4, (&local_E8)->y, script_08F7(arg1));
  }
  return;
}

__attribute__((scn_string_bindings(0, data_10CB6)))
static void script_2671(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
  int8_t local_FF;
  uint8_t unused_FC[3];
  Vector3 local_F0;
  Vector3 local_E4;
  Vector3 local_D8;
  Vector3 local_CC;
  Vector3 local_C0;
  float local_BC;
  float local_B8;
  float local_B4;
  float local_B0;
  float local_AC;
  int32_t local_A8;
  float local_A4;
  int32_t local_A0;
  float local_9C;
  float local_98;
  float local_94;
  float local_90;
  float local_8C;
  float local_88;
  int16_t local_86;
  int16_t local_84;
  int32_t local_80;
  int32_t local_FF7C;
  *(int32_t *)&local_98 = Script_ScenDataUtil(data_F778, 5, Script_ScenDataUtil(data_F188, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9 + 1);
  *(int32_t *)&local_94 = Script_ScenDataUtil(data_F778, 5, Script_ScenDataUtil(data_F188, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9 + 2);
  *(int32_t *)&local_90 = Script_ScenDataUtil(data_F778, 5, Script_ScenDataUtil(data_F188, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9 + 3);
  *(int32_t *)&local_8C = Script_ScenDataUtil(data_F778, 5, Script_ScenDataUtil(data_F188, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9 + 4);
  *(int32_t *)&local_88 = Script_ScenDataUtil(data_F778, 5, Script_ScenDataUtil(data_F188, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9 + 7);
  if (arg5) {
    Script_charaPosRotUtils(arg1, 12, &local_D8, 0, 2);
    Script_charaPosRotUtils(arg1, 7, &local_CC, 0, 2);
  } else {
    Script_EVT_GetPosition(arg1, -1, &local_D8, 0);
    Script_EVT_GetPosition(arg1, -1, &local_CC, 0);
  }
  local_F0.x = 0.0f;
  (&local_F0)->y = 0.0f;
  (&local_F0)->z = 0.0f;
  local_E4.x = 0.0f;
  (&local_E4)->y = 0.0f;
  (&local_E4)->z = 0.0f;
  local_BC = 0.0f;
  local_86 = 0;
  {
    int32_t local_FF78;
    int32_t local_FF74;
    Vector3i local_FF68;
    local_FF74 = 0;
    local_A0 = 0x3F800000;
    local_B4 = 0.0f;
    local_B0 = 0.0f;
    local_AC = 0.0f;
    local_A8 = 0;
    Script_mtwkGetSetRotation(arg1, &local_FF68, 0);
    local_84 = 0;
    if ((arg4 == 1) | (arg4 == 3)) {
      local_FF78 = Script_ScenDataUtil(data_F778, 4, Script_ScenDataUtil(data_F188, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9);
      local_FF7C = 18;
      local_80 = 18;
      local_A4 = 1.0f;
    } else {
      if (arg0 == task_1CC) {
        local_FF78 = Script_ScenDataUtil(data_F778, 4, Script_ScenDataUtil(data_F188, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9) | -0x80000000;
        local_FF7C = 12;
        local_80 = 12;
      } else {
        local_FF78 = Script_ScenDataUtil(data_F778, 4, Script_ScenDataUtil(data_F188, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9);
        local_FF7C = 18;
        local_80 = 18;
      }
      local_A4 = -1.0f;
    }
    script_0345(arg0, local_FF78, arg2, arg3, arg4);
    local_FF78 = local_FF78 | ((Script_FloatUtils(ScriptMathOp_FloatToInt, local_98 - 3.0f) << 16) & 0x7FFF0000);
    script_9549(arg0, 0);
    local_C0.x = (&task_1D4)->x;
    (&local_C0)->y = task_1D4.y;
    (&local_C0)->z = task_1D4.z;
    Script_ryoControl(PCCTL_StartCameraFacingInterpolation, 0);
    Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
    Script_SetCharaMoveTarget(arg0, &task_1D4, *(&task_1E0 + 1), local_FF78);
    for (;;) {
      if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0) == 0) {
        break;
      }
      if (Script_GetCharaMoveStatus(arg0) == 0) {
        break;
      }
      SCNF_WaitFrames_0000(1);
    }
    local_FF78 = local_FF78 & 0xFFFF;
    if (task_1CC == arg0) {
      Script_SetCharaMotionPlaybackRate(arg0, 1.3f);
      local_FF = 0;
    } else {
      local_FF = 1;
    }
    if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
      local_B8 = Script_GetCharaMotionRemainingFramesScaled(arg0);
    }
    for (;;) {
      if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0) == 0) {
        Script_sprintf("This Motion don't Finish!!\012");
        break;
      }
      if (local_FF) {
        if (!(local_B8 <= local_90)) {
          local_FF = 0;
          script_259B(arg0, task_104[arg2]);
        }
      }
      Script_ReadMOTNPositionAtFrame(arg0, local_FF78, local_B8, &local_E4);
      local_E4.x = (&local_E4)->x * local_A4;
      if (!(local_B8 >= local_98)) {
        local_86 = 0;
      }
      if (!(local_B8 < local_98) & !(local_B8 > local_94)) {
        if (!(local_B8 < local_98) & !(local_B8 >= local_98 + 1.5f)) {
          if (local_84 == 0) {
            int32_t local_FF64;
            local_FF64 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_B4[arg2], 2);
            local_FF64 = Script_ScenDataUtil(task_74, 4, Script_ScenDataUtil(task_78, 4, local_FF64 * 9 + 1) * 4 + 2);
            if (local_FF64 != -1) {
              if (arg0 == task_1CC) {
                Script_executeAicaCommand(local_FF64, 0, 0);
              } else {
                Script_Execute3DSoundCommand_Wrapper(local_FF64, 4, &local_C0, 5.0f, 20.0f, 0);
              }
            }
            local_84 = 1;
          }
        }
        Script_EVT_GetPosition(arg0, local_80, &local_F0, ENTITY_XFORM_MOTION_SPACE);
        if (local_86 == 1) {
          local_B0 = (&local_E4)->x;
          local_BC = (&local_F0)->x;
        }
        local_B4 = (&local_E4)->x + ((&local_F0)->x - local_BC);
        local_B4 = local_B4 - local_B0;
        local_B4 = local_B4 * 1.28f;
        {
          int32_t local_FF64;
          local_FF64 = arg4;
          if ((local_FF64 == 0) | (local_FF64 == 2)) {
            if (!(local_B4 < 0.0f)) {
              local_B4 = 0.0f;
            } else {
              if (!(local_B4 > -0.75f)) {
                local_B4 = -0.75f;
                goto L002BF9;
              }
            }
            L002BF9:;
          } else {
            if ((local_FF64 == 1) | (local_FF64 == 3)) {
              if (!(local_B4 > 0.0f)) {
                local_B4 = 0.0f;
              } else {
                if (!(local_B4 < 0.75f)) {
                  local_B4 = 0.75f;
                  goto L002C37;
                }
              }
              L002C37:;
              goto L002C3A;
            }
          }
        }
      }
      L002C3A:;
      if (task_1CC == arg0) {
        if (!(local_B8 <= local_94) & !(local_B8 >= local_90)) {
          local_86 = 0;
        }
      } else {
        if (!(local_B8 <= local_94) & !(local_B8 > local_88)) {
          *(int32_t *)&local_9C = Script_ScenDataUtil(data_F778, 5, Script_ScenDataUtil(data_F188, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9 + 8);
          local_B4 = local_B4 + local_9C * local_A4;
        }
      }
      if (!(local_B8 < local_90) & !(local_B8 >= local_90 + 1.5f)) {
        if (local_84 == 1) {
          int32_t local_FF64;
          local_FF64 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_B4[arg2], 2);
          local_FF64 = Script_ScenDataUtil(task_74, 4, Script_ScenDataUtil(task_78, 4, local_FF64 * 9 + 1) * 4 + 3);
          if (local_FF64 != -1) {
            if (arg0 == task_1CC) {
              Script_executeAicaCommand(local_FF64, 0, 0);
            } else {
              Script_Execute3DSoundCommand_Wrapper(local_FF64, 4, &local_C0, 5.0f, 20.0f, 0);
            }
          }
          local_84 = 2;
        }
        if (task_1CC != arg0) {
          if (local_FF74 == 0) {
            local_AC = local_B4 / (local_8C - local_90);
            local_FF74 = 1;
          }
        }
      }
      if (!(local_B8 < local_90) & !(local_B8 > local_8C)) {
        Script_EVT_GetPosition(arg0, local_FF7C, &local_F0, ENTITY_XFORM_MOTION_SPACE);
        if (task_1CC == arg0) {
          int32_t local_FF64;
          local_B4 = (&local_E4)->x + ((&local_F0)->x - local_BC);
          local_B4 = local_B4 - local_B0;
          local_B4 = local_B4 * 1.28f;
          local_FF64 = arg4;
          if ((local_FF64 == 0) | (local_FF64 == 2)) {
            if (!(local_B4 > -0.75f)) {
              local_B4 = -0.75f;
            } else {
              if (!(local_B4 < 0.0f)) {
                local_B4 = 0.0f;
                goto L002E06;
              }
            }
            L002E06:;
          } else {
            if ((local_FF64 == 1) | (local_FF64 == 3)) {
              if (!(local_B4 < 0.75f)) {
                local_B4 = 0.75f;
              } else {
                if (!(local_B4 > 0.0f)) {
                  local_B4 = 0.0f;
                  goto L002E44;
                }
              }
              L002E44:;
              goto L002E47;
            }
          }
          L002E47:;
        } else {
          int32_t local_FF64;
          local_B4 = local_B4 - local_AC;
          local_FF64 = arg4;
          if ((local_FF64 == 0) | (local_FF64 == 2)) {
            if (!(local_B4 < 0.0f)) {
              local_B4 = 0.0f;
            }
          } else {
            if ((local_FF64 == 1) | (local_FF64 == 3)) {
              if (!(local_B4 > 0.0f)) {
                local_B4 = 0.0f;
              }
              goto L002EA0;
            }
          }
        }
      }
      L002EA0:;
      if (!(local_8C <= 0.0f) & !(local_B8 <= local_8C)) {
        local_B4 = local_B4 - 0.01f;
        if (!(local_B4 > 0.0f)) {
          local_B4 = 0.0f;
        }
      }
      {
        int32_t local_FF64;
        local_FF64 = arg4;
        switch (local_FF64) {
          case 0:
            local_F0.x = (&local_CC)->x;
            (&local_F0)->y = (&local_CC)->y;
            (&local_F0)->z = (&local_CC)->z;
            if (arg5) {
              local_F0.x = (&local_CC)->x - local_B4;
              Script_charaPosRotUtils(arg1, 7, &local_F0, 0, 0);
            } else {
              local_F0.x = (&local_CC)->x + ((float)0 - local_B4) * Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_FF68)->y);
              (&local_F0)->z = (&local_CC)->z + local_B4 * Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_FF68)->y);
              Script_EVT_SetPosition(arg1, POS_XYZ, &local_F0);
            }
            break;
          case 1:
            local_F0.x = (&local_CC)->x;
            (&local_F0)->y = (&local_CC)->y;
            (&local_F0)->z = (&local_CC)->z;
            if (arg5) {
              local_F0.x = (&local_CC)->x + local_B4;
              Script_charaPosRotUtils(arg1, 7, &local_F0, 0, 0);
            } else {
              local_F0.x = (&local_CC)->x - ((float)0 - local_B4) * Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_FF68)->y);
              (&local_F0)->z = (&local_CC)->z - local_B4 * Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_FF68)->y);
              Script_EVT_SetPosition(arg1, POS_XYZ, &local_F0);
            }
            break;
          case 2:
            local_F0.x = (&local_D8)->x;
            (&local_F0)->y = (&local_D8)->y;
            (&local_F0)->z = (&local_D8)->z;
            if (arg5) {
              local_F0.x = (&local_D8)->x + local_B4;
              Script_charaPosRotUtils(arg1, 12, &local_F0, 0, 0);
            } else {
              local_F0.x = (&local_D8)->x - ((float)0 - local_B4) * Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_FF68)->y);
              (&local_F0)->z = (&local_D8)->z - local_B4 * Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_FF68)->y);
              Script_EVT_SetPosition(arg1, POS_XYZ, &local_F0);
            }
            break;
          case 3:
            local_F0.x = (&local_D8)->x;
            (&local_F0)->y = (&local_D8)->y;
            (&local_F0)->z = (&local_D8)->z;
            if (arg5) {
              local_F0.x = (&local_D8)->x - local_B4;
              Script_charaPosRotUtils(arg1, 12, &local_F0, 0, 0);
            } else {
              local_F0.x = (&local_D8)->x + ((float)0 - local_B4) * Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_FF68)->y);
              (&local_F0)->z = (&local_D8)->z + local_B4 * Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_FF68)->y);
              Script_EVT_SetPosition(arg1, POS_XYZ, &local_F0);
            }
            break;
        }
      }
      local_86 = local_86 + 1;
      if (arg0 == task_1CC) {
        local_B8 = Script_GetCharaMotionRemainingFramesScaled(arg0);
        if (Script_IsCharaMotionFinished(arg0)) {
          break;
        }
      } else {
        if (!(local_B8 <= local_8C)) {
          break;
        }
        local_B8 = local_B8 + 1.0f;
      }
      SCNF_WaitFrames_0000(1);
    }
  }
  script_9549(arg0, 1);
  {
    int32_t local_FF78;
    int32_t unused_FF68[4];
    int32_t local_FF64;
    local_FF78 = task_B4[arg2];
    local_FF64 = arg3;
    if ((local_FF64 == 9) | (local_FF64 == 10) | (local_FF64 == 15)) {
      if (arg5) {
        Script_charaPosRotUtils(arg1, 12, &local_D8, 0, 0);
        Script_charaPosRotUtils(arg1, 7, &local_CC, 0, 0);
      } else {
        Script_EVT_SetPosition(arg1, POS_XYZ, &local_D8);
      }
      if (task_1CC == arg0) {
        local_80 = (Script_ScenDataUtil(task_6C, 4, local_FF78 * 13 + 2) >> 16) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_80, 1);
        local_80 = Script_ScenDataUtil(task_6C, 4, local_FF78 * 13 + 2) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_80, 1);
      }
    } else if ((local_FF64 == 18) | (local_FF64 == 19) | (local_FF64 == 21) | (local_FF64 == 22)) {
      if (task_1CC == arg0) {
        local_80 = (Script_ScenDataUtil(task_6C, 4, local_FF78 * 13 + 2) >> 16) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_80, 1);
        {
          int32_t local_FF60;
          local_FF60 = Script_ScenDataUtil(task_6C, 4, local_FF78 * 13 + 1);
          local_FF60 = local_FF60 | 8;
          Script_ScenDataUtil(task_6C, 14, local_FF78 * 13 + 1, local_FF60);
        }
        {
          int32_t local_FF60;
          local_FF60 = Script_ScenDataUtil(task_6C, 4, local_FF78 * 13 + 1);
          local_FF60 = local_FF60 | 0x10;
          Script_ScenDataUtil(task_6C, 14, local_FF78 * 13 + 1, local_FF60);
        }
        {
          int32_t local_FF60;
          local_FF60 = Script_ScenDataUtil(task_6C, 4, local_FF78 * 13 + 1);
          local_FF60 = local_FF60 | 2;
          Script_ScenDataUtil(task_6C, 14, local_FF78 * 13 + 1, local_FF60);
        }
      } else {
        if (arg5) {
          Script_charaPosRotUtils(arg1, 12, &local_D8, 0, 0);
          Script_charaPosRotUtils(arg1, 7, &local_CC, 0, 0);
        } else {
          Script_EVT_SetPosition(arg1, POS_XYZ, &local_D8);
        }
        local_80 = (Script_ScenDataUtil(task_6C, 4, local_FF78 * 13 + 2) >> 16) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_80, 1);
        local_80 = Script_ScenDataUtil(task_6C, 4, local_FF78 * 13 + 2) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_80, 1);
      }
    } else {
      if ((local_FF64 == 20) | (local_FF64 == 23)) {
        if (task_1CC == arg0) {
          if ((arg4 == 0) | (arg4 == 1)) {
            {
              int32_t local_FF60;
              local_FF60 = Script_ScenDataUtil(task_6C, 4, local_FF78 * 13 + 1);
              local_FF60 = local_FF60 | 0x10;
              Script_ScenDataUtil(task_6C, 14, local_FF78 * 13 + 1, local_FF60);
            }
            {
              int32_t local_FF60;
              local_FF60 = Script_ScenDataUtil(task_6C, 4, local_FF78 * 13 + 1);
              local_FF60 = local_FF60 | 4;
              Script_ScenDataUtil(task_6C, 14, local_FF78 * 13 + 1, local_FF60);
            }
          } else {
            {
              int32_t local_FF60;
              local_FF60 = Script_ScenDataUtil(task_6C, 4, local_FF78 * 13 + 1);
              local_FF60 = local_FF60 | 8;
              Script_ScenDataUtil(task_6C, 14, local_FF78 * 13 + 1, local_FF60);
            }
            {
              int32_t local_FF60;
              local_FF60 = Script_ScenDataUtil(task_6C, 4, local_FF78 * 13 + 1);
              local_FF60 = local_FF60 | 4;
              Script_ScenDataUtil(task_6C, 14, local_FF78 * 13 + 1, local_FF60);
            }
          }
        } else {
          if (arg5) {
            Script_charaPosRotUtils(arg1, 12, &local_D8, 0, 0);
            Script_charaPosRotUtils(arg1, 7, &local_CC, 0, 0);
          } else {
            Script_EVT_SetPosition(arg1, POS_XYZ, &local_D8);
          }
          local_80 = (Script_ScenDataUtil(task_6C, 4, local_FF78 * 13 + 2) >> 16) & 0xFFFF;
          Script_scenToggleCharaFlag(0, local_80, 1);
          local_80 = Script_ScenDataUtil(task_6C, 4, local_FF78 * 13 + 2) & 0xFFFF;
          Script_scenToggleCharaFlag(0, local_80, 1);
        }
        goto L003583;
      }
    }
  }
  L003583:;
  return;
}

static void script_3584(int32_t arg0, int32_t arg1, int32_t arg2) {
  int32_t local_FC;
  if (task_1CC == arg0) {
    local_FC = task_B4[arg1];
    {
      int32_t local_F8;
      local_F8 = Script_ScenDataUtil(task_6C, 4, local_FC * 13 + 2) & 0xFFFF;
      Script_scenToggleCharaFlag(0, local_F8, arg2);
    }
    {
      int32_t local_F8;
      local_F8 = (Script_ScenDataUtil(task_6C, 4, local_FC * 13 + 2) >> 16) & 0xFFFF;
      Script_scenToggleCharaFlag(0, local_F8, arg2);
    }
  }
  return;
}

static void script_35F0(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
  Vector3 local_F4;
  Vector3 local_E8;
  Vector3 local_DC;
  Vector3 local_D0;
  Vector3 local_C4;
  Vector3 local_B8;
  Vector3 local_AC;
  Vector3i local_A0;
  int32_t local_9C;
  int32_t local_98;
  float local_94;
  int8_t local_93;
  int8_t local_92;
  uint8_t unused_90[2];
  int32_t local_8C;
  int32_t local_88;
  int32_t local_84;
  if (arg4 == 4) {
    Script_charaPosRotUtils(arg1, 12, &local_C4, 0, 2);
    Script_charaPosRotUtils(arg1, 7, &local_B8, 0, 2);
  } else {
    Script_EVT_GetPosition(arg1, -1, &local_C4, 0);
    Script_EVT_GetPosition(arg1, -1, &local_B8, 0);
  }
  local_AC.x = (&task_1D4)->x;
  (&local_AC)->y = task_1D4.y;
  (&local_AC)->z = task_1D4.z;
  local_DC.x = (&local_C4)->x;
  (&local_DC)->y = (&local_C4)->y;
  (&local_DC)->z = (&local_C4)->z;
  local_D0.x = (&local_B8)->x;
  (&local_D0)->y = (&local_B8)->y;
  (&local_D0)->z = (&local_B8)->z;
  local_9C = 0xC000;
  local_93 = 0;
  local_98 = 0x300;
  Script_EVT_GetPosition(arg1, -1, &local_F4, 0);
  Script_mtwkGetSetRotation(arg1, &local_A0, 0);
  {
    int32_t local_80;
    local_80 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_B4[arg2], 2);
    local_80 = Script_ScenDataUtil(task_74, 4, Script_ScenDataUtil(task_78, 4, local_80 * 9 + 1) * 4 + 2);
    if (local_80 != -1) {
      if (arg0 == task_1CC) {
        Script_executeAicaCommand(local_80, 0, 0);
      } else {
        Script_Execute3DSoundCommand_Wrapper(local_80, 4, &local_AC, 5.0f, 20.0f, 0);
      }
    }
  }
  local_88 = 0;
  local_8C = 0;
  local_84 = 0;
  for (;;) {
    local_92 = 0;
    if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
      Script_EVT_GetPosition(arg0, -1, &local_E8, 0);
      if (Script_CheckTargetWithinAngleRange(&local_F4, 1.5f, (&local_A0)->y, 0x10000, &local_E8)) {
        local_92 = 1;
      }
    }
    if (local_92) {
      if (local_8C) {
        task_DC[arg2] = arg0;
        local_98 = -local_98;
        local_84 = 0;
      }
      local_8C = 0;
    } else {
      if ((local_93 == 1) & (local_8C == 0)) {
        int32_t local_80;
        local_80 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_B4[arg2], 2);
        local_80 = Script_ScenDataUtil(task_74, 4, Script_ScenDataUtil(task_78, 4, local_80 * 9 + 1) * 4 + 3);
        if (local_80 != -1) {
          if (arg0 == task_1CC) {
            Script_executeAicaCommand(local_80, 0, 0);
          } else {
            Script_Execute3DSoundCommand_Wrapper(local_80, 4, &local_AC, 5.0f, 20.0f, 0);
          }
        }
        local_93 = 0;
        local_8C = 1;
        local_98 = -local_98;
        local_84 = 0;
        task_DC[arg2] = 0;
      }
    }
    local_88 = task_B4[arg2];
    local_88 = Script_ScenDataUtil(task_6C, 4, local_88 * 13 + 2) & 0xFFFF;
    local_88 = local_88 + ((local_9C - 0x4000) >> 12);
    if (local_84 != local_88) {
      Script_scenToggleCharaFlag(0, local_88, local_8C);
      local_84 = local_88;
    }
    local_9C = local_9C - local_98;
    if (local_9C >= 0xC000) {
      local_9C = 0xC000;
      local_88 = task_B4[arg2];
      local_88 = Script_ScenDataUtil(task_6C, 4, local_88 * 13 + 2) & 0xFFFF;
      local_88 = local_88 + 8;
      Script_scenToggleCharaFlag(0, local_88, local_8C);
      break;
    }
    if (local_9C <= 0x4000) {
      local_9C = 0x4000;
      if (local_93 == 0) {
        task_DC[arg2] = 0;
        local_93 = 1;
      }
    }
    local_94 = (Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, local_9C) + 1.0f) * 0.45f;
    {
      int32_t local_80;
      local_80 = arg4;
      if (local_80 == 4) {
        local_DC.x = (&local_C4)->x + local_94;
        local_D0.x = (&local_B8)->x - local_94;
        Script_charaPosRotUtils(arg1, 12, &local_DC, 0, 0);
        Script_charaPosRotUtils(arg1, 7, &local_D0, 0, 0);
      } else if ((local_80 == 0) | (local_80 == 1)) {
        local_DC.x = (&local_C4)->x - ((float)0 - local_94) * Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_A0)->y);
        (&local_DC)->z = (&local_C4)->z - local_94 * Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_A0)->y);
        Script_EVT_SetPosition(arg1, POS_XYZ, &local_DC);
      } else {
        if ((local_80 == 2) | (local_80 == 3)) {
          local_D0.x = (&local_B8)->x + ((float)0 - local_94) * Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_A0)->y);
          (&local_D0)->z = (&local_B8)->z + local_94 * Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_A0)->y);
          Script_EVT_SetPosition(arg1, POS_XYZ, &local_D0);
          goto L003A5A;
        }
      }
    }
    L003A5A:;
    SCNF_WaitFrames_0000(1);
  }
  if (arg4 == 4) {
    Script_charaPosRotUtils(arg1, 12, &local_C4, 0, 0);
    Script_charaPosRotUtils(arg1, 7, &local_B8, 0, 0);
  } else {
    Script_EVT_SetPosition(arg1, POS_XYZ, &local_C4);
    Script_EVT_SetPosition(arg1, POS_XYZ, &local_B8);
  }
  return;
}

__attribute__((scn_string_bindings(0, data_10CD2)))
static void script_3AAA(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  int32_t local_F0;
  Vector3 local_E4;
  Vector3 local_D8;
  Vector3 local_CC;
  float local_C8;
  float local_C4;
  float local_C0;
  float local_BC;
  float local_B8;
  int8_t local_B7;
  uint8_t unused_B4[3];
  Vector3i local_A8;
  int32_t local_A4;
  local_A4 = arg3;
  switch (local_A4) {
    case 24:
      local_A4 = arg4;
      switch (local_A4) {
        case 1:
          local_B7 = 1;
          local_F8 = 18;
          local_F0 = 0x4000;
          local_B8 = 1.0f;
          local_FC = Script_ScenDataUtil(data_F778, 4, Script_ScenDataUtil(data_F188, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9);
          break;
        case 0:
          local_B7 = 0;
          local_F8 = 12;
          local_F0 = 0x4000;
          local_B8 = 1.0f;
          local_FC = Script_ScenDataUtil(data_F778, 4, Script_ScenDataUtil(data_F188, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9) | -0x80000000;
          break;
      }
      break;
    case 27:
      local_A4 = arg4;
      switch (local_A4) {
        case 1:
          local_B7 = 0;
          local_F8 = 12;
          local_F0 = 0xC000;
          local_B8 = 1.0f;
          local_FC = Script_ScenDataUtil(data_F778, 4, Script_ScenDataUtil(data_F188, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9) | -0x80000000;
          break;
        case 0:
          local_B7 = 1;
          local_F8 = 18;
          local_F0 = 0xC000;
          local_B8 = 1.0f;
          local_FC = Script_ScenDataUtil(data_F778, 4, Script_ScenDataUtil(data_F188, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9);
          break;
      }
      break;
    case 25:
      local_A4 = arg4;
      switch (local_A4) {
        case 1:
          local_B7 = 1;
          local_F8 = 12;
          local_F0 = 0xC000;
          local_B8 = -1.0f;
          local_FC = Script_ScenDataUtil(data_F778, 4, Script_ScenDataUtil(data_F188, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9) | -0x80000000;
          break;
        case 0:
          local_B7 = 0;
          local_F8 = 18;
          local_F0 = 0xC000;
          local_B8 = -1.0f;
          local_FC = Script_ScenDataUtil(data_F778, 4, Script_ScenDataUtil(data_F188, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9);
          break;
      }
      break;
    case 26:
      local_A4 = arg4;
      switch (local_A4) {
        case 1:
          local_B7 = 0;
          local_F8 = 18;
          local_F0 = 0x4000;
          local_B8 = -1.0f;
          local_FC = Script_ScenDataUtil(data_F778, 4, Script_ScenDataUtil(data_F188, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9);
          break;
        case 0:
          local_B7 = 1;
          local_F8 = 12;
          local_F0 = 0x4000;
          local_B8 = -1.0f;
          local_FC = Script_ScenDataUtil(data_F778, 4, Script_ScenDataUtil(data_F188, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9) | -0x80000000;
          break;
      }
      break;
  }
  *(int32_t *)&local_C4 = Script_ScenDataUtil(data_F778, 5, Script_ScenDataUtil(data_F188, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9 + 1);
  *(int32_t *)&local_C0 = Script_ScenDataUtil(data_F778, 5, Script_ScenDataUtil(data_F188, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9 + 2);
  local_FC = local_FC | ((Script_FloatUtils(ScriptMathOp_FloatToInt, local_C4 - 3.0f) << 16) & 0x7FFF0000);
  local_BC = 0.0f;
  script_9549(arg0, 0);
  local_CC.x = (&task_1D4)->x;
  (&local_CC)->y = task_1D4.y;
  (&local_CC)->z = task_1D4.z;
  Script_ryoControl(PCCTL_StartCameraFacingInterpolation, 0);
  Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
  Script_SetCharaMoveTarget(arg0, &task_1D4, *(&task_1E0 + 1), local_FC);
  for (;;) {
    if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0) == 0) {
      break;
    }
    if (Script_GetCharaMoveStatus(arg0) == 0) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  local_FC = local_FC & 0xFFFF;
  Script_EVT_GetPosition(arg1, -1, &local_E4, 0);
  Script_mtwkGetSetRotation(arg1, &local_A8, 0);
  local_F4 = (&local_A8)->y;
  for (;;) {
    if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0) == 0) {
      Script_sprintf("This Motion don't Finish!!\012");
      break;
    }
    if (Script_IsCharaMotionFinished(arg0)) {
      break;
    }
    local_C8 = Script_GetCharaMotionRemainingFramesScaled(arg0);
    if (!(local_C8 < local_C4) & !(local_C8 > local_C0)) {
      if (local_B7 == 1) {
        local_BC = local_BC - 0.3f;
      }
      if (!(local_C8 < local_C4) & !(local_C8 >= local_C4 + 1.0f)) {
        int32_t local_A0;
        local_A0 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_B4[arg2], 2);
        local_A0 = Script_ScenDataUtil(task_74, 4, Script_ScenDataUtil(task_78, 4, local_A0 * 9 + 1) * 4 + 2);
        if (local_A0 != -1) {
          if (arg0 == task_1CC) {
            Script_executeAicaCommand(local_A0, 0, 0);
          } else {
            Script_Execute3DSoundCommand_Wrapper(local_A0, 4, &local_CC, 5.0f, 20.0f, 0);
          }
        }
      }
      Script_EVT_GetPosition(arg0, local_F8, &local_D8, ENTITY_XFORM_WORLD_SPACE);
      local_F4 = Script_FloatUtils(ScriptMathOp_Atan2ReverseVector, (&local_E4)->x - (&local_D8)->x, (&local_E4)->z - (&local_D8)->z);
      local_F4 = local_F4 + local_F0 + (int32_t)(local_BC * local_B8 / 360.0f * (float)0x10000);
    }
    {
      Vector3i local_98;
      (&local_98)->y = local_F4;
      Script_EVT_SetRotation(arg1, MATRIX_XFORM_Y, &local_98, 0);
    }
    SCNF_WaitFrames_0000(1);
  }
  script_9549(arg0, 1);
  local_FC = task_B4[arg2];
  {
    int32_t local_A0;
    local_A0 = Script_ScenDataUtil(task_6C, 4, local_FC * 13 + 1);
    local_A0 = local_A0 | 8;
    Script_ScenDataUtil(task_6C, 14, local_FC * 13 + 1, local_A0);
  }
  {
    int32_t local_A0;
    local_A0 = Script_ScenDataUtil(task_6C, 4, local_FC * 13 + 1);
    local_A0 = local_A0 | 0x10;
    Script_ScenDataUtil(task_6C, 14, local_FC * 13 + 1, local_A0);
  }
  {
    int32_t local_A0;
    local_A0 = Script_ScenDataUtil(task_6C, 4, local_FC * 13 + 1);
    local_A0 = local_A0 | 4;
    Script_ScenDataUtil(task_6C, 14, local_FC * 13 + 1, local_A0);
  }
  return;
}

__attribute__((scn_string_bindings(0, data_10CEE)))
static void script_4164(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
  Vector3 local_F4;
  Vector3 local_E8;
  int32_t unused_DC[3];
  Vector3 local_D0;
  int8_t local_CF;
  int8_t local_CE;
  uint8_t unused_CC[2];
  float local_C8;
  float local_C4;
  float local_C0;
  float local_BC;
  float local_B8;
  float local_B4;
  float local_B0;
  float local_AC;
  float local_A8;
  int32_t local_A4;
  int32_t local_A0;
  int32_t local_9C;
  int32_t local_98;
  int32_t local_94;
  int32_t local_90;
  int32_t local_8C;
  int32_t local_88;
  int32_t local_84;
  int32_t local_80;
  int32_t local_FF7C;
  float local_FF78;
  float local_FF74;
  int32_t local_FF70;
  Vector3i local_FF64;
  int32_t local_FF60;
  local_CE = 0;
  local_80 = 0;
  local_FF78 = 1.0f;
  local_FF74 = 0.0f;
  {
    int32_t local_FF5C;
    local_FF5C = arg3;
    if ((local_FF5C == 4) | (local_FF5C == 6) | (local_FF5C == 33) | (local_FF5C == 35)) {
      if (arg0 == task_1CC) {
        local_FF5C = arg4;
        switch (local_FF5C) {
          case 1:
            local_CF = 1;
            local_A4 = Script_ScenDataUtil(data_F778, 4, Script_ScenDataUtil(data_F188, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9);
            local_C0 = 1.0f;
            local_A0 = 18;
            local_9C = 18;
            local_90 = 15473;
            local_8C = 17294;
            break;
          case 0:
            local_CF = 0;
            local_A4 = Script_ScenDataUtil(data_F778, 4, Script_ScenDataUtil(data_F188, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9) | -0x80000000;
            local_C0 = 1.0f;
            local_A0 = 12;
            local_9C = 18;
            local_90 = 17294;
            local_8C = 15473;
            break;
        }
      } else {
        local_FF5C = arg4;
        switch (local_FF5C) {
          case 1:
            local_CF = 1;
            local_A4 = Script_ScenDataUtil(data_F778, 4, Script_ScenDataUtil(data_F188, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9);
            local_C0 = 1.0f;
            local_A0 = 18;
            local_9C = 18;
            local_90 = 13653;
            break;
          case 0:
            local_CF = 0;
            local_A4 = Script_ScenDataUtil(data_F778, 4, Script_ScenDataUtil(data_F188, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9);
            local_C0 = 1.0f;
            local_A0 = 18;
            local_9C = 18;
            local_90 = 19842;
            break;
        }
      }
    } else if ((local_FF5C == 1) | (local_FF5C == 7) | (local_FF5C == 30) | (local_FF5C == 36) | (local_FF5C == 37)) {
      if (arg0 == task_1CC) {
        local_FF5C = arg4;
        switch (local_FF5C) {
          case 1:
            local_CF = 0;
            local_A4 = Script_ScenDataUtil(data_F778, 4, Script_ScenDataUtil(data_F188, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9) | -0x80000000;
            local_C0 = 1.0f;
            local_A0 = 12;
            local_9C = 18;
            local_90 = 50062;
            local_8C = 48241;
            break;
          case 0:
            local_CF = 1;
            local_A4 = Script_ScenDataUtil(data_F778, 4, Script_ScenDataUtil(data_F188, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9);
            local_C0 = 1.0f;
            local_A0 = 18;
            local_9C = 18;
            local_90 = 48241;
            local_8C = 50062;
            break;
        }
      } else {
        local_FF5C = arg4;
        switch (local_FF5C) {
          case 1:
            local_CF = 0;
            local_A4 = Script_ScenDataUtil(data_F778, 4, Script_ScenDataUtil(data_F188, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9);
            local_C0 = 1.0f;
            local_A0 = 18;
            local_9C = 18;
            if (task_104[arg2] == 4) {
              local_90 = 50062;
            } else {
              local_90 = 51882;
            }
            break;
          case 0:
            local_CF = 1;
            local_A4 = Script_ScenDataUtil(data_F778, 4, Script_ScenDataUtil(data_F188, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9);
            local_C0 = 1.0f;
            local_A0 = 18;
            local_9C = 18;
            if (task_104[arg2] == 4) {
              local_90 = 46421;
            } else {
              local_90 = 45511;
            }
            break;
        }
      }
    } else if ((local_FF5C == 3) | (local_FF5C == 5) | (local_FF5C == 32) | (local_FF5C == 34)) {
      if (arg0 == task_1CC) {
        local_FF5C = arg4;
        switch (local_FF5C) {
          case 1:
            local_CF = 1;
            local_A4 = Script_ScenDataUtil(data_F778, 4, Script_ScenDataUtil(data_F188, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9) | -0x80000000;
            local_C0 = -1.0f;
            local_A0 = 12;
            local_9C = 12;
            local_90 = 50062;
            local_8C = 48241;
            break;
          case 0:
            local_CF = 0;
            local_A4 = Script_ScenDataUtil(data_F778, 4, Script_ScenDataUtil(data_F188, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9);
            local_C0 = -1.0f;
            local_A0 = 18;
            local_9C = 12;
            local_90 = 48241;
            local_8C = 50062;
            break;
        }
      } else {
        local_FF5C = arg4;
        switch (local_FF5C) {
          case 1:
            local_CF = 1;
            local_A4 = Script_ScenDataUtil(data_F778, 4, Script_ScenDataUtil(data_F188, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9);
            local_C0 = -1.0f;
            local_A0 = 18;
            local_9C = 18;
            local_90 = 51882;
            break;
          case 0:
            local_CF = 0;
            local_A4 = Script_ScenDataUtil(data_F778, 4, Script_ScenDataUtil(data_F188, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9);
            local_C0 = -1.0f;
            local_A0 = 18;
            local_9C = 18;
            local_90 = 45511;
            break;
        }
      }
    } else if ((local_FF5C == 2) | (local_FF5C == 8) | (local_FF5C == 31)) {
      if (arg0 == task_1CC) {
        local_FF5C = arg4;
        switch (local_FF5C) {
          case 1:
            local_CF = 0;
            local_A4 = Script_ScenDataUtil(data_F778, 4, Script_ScenDataUtil(data_F188, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9);
            local_C0 = -1.0f;
            local_A0 = 18;
            local_9C = 12;
            local_90 = 15473;
            local_8C = 17294;
            break;
          case 0:
            local_CF = 1;
            local_A4 = Script_ScenDataUtil(data_F778, 4, Script_ScenDataUtil(data_F188, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9) | -0x80000000;
            local_C0 = -1.0f;
            local_A0 = 12;
            local_9C = 12;
            local_90 = 17294;
            local_8C = 15473;
            break;
        }
      } else {
        local_FF5C = arg4;
        switch (local_FF5C) {
          case 1:
            local_CF = 0;
            local_A4 = Script_ScenDataUtil(data_F778, 4, Script_ScenDataUtil(data_F188, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9);
            local_C0 = -1.0f;
            local_A0 = 18;
            local_9C = 18;
            local_90 = 13653;
            break;
          case 0:
            local_CF = 1;
            local_A4 = Script_ScenDataUtil(data_F778, 4, Script_ScenDataUtil(data_F188, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9);
            local_C0 = -1.0f;
            local_A0 = 18;
            local_9C = 18;
            local_90 = 19842;
            break;
        }
      }
    } else {
      if ((local_FF5C == 13) | (local_FF5C == 14) | (local_FF5C == 28)) {
        if (arg0 == task_1CC) {
          local_FF5C = arg4;
          switch (local_FF5C) {
            case 0:
              local_CF = 0;
              local_A4 = Script_ScenDataUtil(data_F778, 4, Script_ScenDataUtil(data_F188, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9);
              local_C0 = -1.0f;
              local_A0 = 18;
              local_9C = 12;
              local_90 = 48241;
              local_8C = 50062;
              break;
            case 1:
              local_CF = 1;
              local_A4 = Script_ScenDataUtil(data_F778, 4, Script_ScenDataUtil(data_F188, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9) | -0x80000000;
              local_C0 = -1.0f;
              local_A0 = 12;
              local_9C = 12;
              local_90 = 50062;
              local_8C = 48241;
              break;
            case 2:
              local_CF = 1;
              local_A4 = Script_ScenDataUtil(data_F778, 4, Script_ScenDataUtil(data_F188, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9);
              local_C0 = 1.0f;
              local_A0 = 18;
              local_9C = 18;
              local_90 = 15473;
              local_8C = 17294;
              break;
            case 3:
              local_CF = 0;
              local_A4 = Script_ScenDataUtil(data_F778, 4, Script_ScenDataUtil(data_F188, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9) | -0x80000000;
              local_C0 = 1.0f;
              local_A0 = 12;
              local_9C = 18;
              local_90 = 17294;
              local_8C = 15473;
              break;
          }
        } else {
          local_FF5C = arg4;
          switch (local_FF5C) {
            case 0:
              local_CF = 0;
              local_A4 = Script_ScenDataUtil(data_F778, 4, Script_ScenDataUtil(data_F188, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9);
              local_C0 = -1.0f;
              local_A0 = 18;
              local_9C = 18;
              local_90 = 45511;
              break;
            case 1:
              local_CF = 1;
              local_A4 = Script_ScenDataUtil(data_F778, 4, Script_ScenDataUtil(data_F188, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9);
              local_C0 = -1.0f;
              local_A0 = 18;
              local_9C = 18;
              local_90 = 51882;
              break;
            case 2:
              local_CF = 1;
              local_A4 = Script_ScenDataUtil(data_F778, 4, Script_ScenDataUtil(data_F188, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9);
              local_C0 = 1.0f;
              local_A0 = 18;
              local_9C = 18;
              if (task_104[arg2] == 4) {
                local_90 = 14199;
              } else {
                local_90 = 13653;
              }
              break;
            case 3:
              local_CF = 0;
              local_A4 = Script_ScenDataUtil(data_F778, 4, Script_ScenDataUtil(data_F188, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9);
              local_C0 = 1.0f;
              local_A0 = 18;
              local_9C = 18;
              if (task_104[arg2] == 4) {
                local_90 = 18022;
              } else {
                local_90 = 19842;
              }
              break;
          }
        }
        local_CE = 1;
        goto L004D57;
      }
    }
  }
  L004D57:;
  (void)local_90;
  *(int32_t *)&local_B8 = Script_ScenDataUtil(data_F778, 5, Script_ScenDataUtil(data_F188, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9 + 1);
  *(int32_t *)&local_B4 = Script_ScenDataUtil(data_F778, 5, Script_ScenDataUtil(data_F188, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9 + 2);
  *(int32_t *)&local_B0 = Script_ScenDataUtil(data_F778, 5, Script_ScenDataUtil(data_F188, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9 + 3);
  *(int32_t *)&local_AC = Script_ScenDataUtil(data_F778, 5, Script_ScenDataUtil(data_F188, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9 + 4);
  *(int32_t *)&local_A8 = Script_ScenDataUtil(data_F778, 5, Script_ScenDataUtil(data_F188, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9 + 7);
  local_C8 = 0.0f;
  local_FF7C = 0;
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg1)) {
    if (local_CE) {
      {
        int32_t unused_FF5C;
        Vector3i local_FF50;
        Script_charaPosRotUtils(arg1, 12, 0, &local_FF50, 2);
        local_98 = (&local_FF50)->y;
      }
      {
        int32_t unused_FF5C;
        Vector3i local_FF50;
        Script_charaPosRotUtils(arg1, 7, 0, &local_FF50, 2);
        local_94 = (&local_FF50)->y;
      }
      if ((arg4 == 2) | (arg4 == 3)) {
        local_88 = local_98;
        Script_charaPosRotUtils(arg1, 12, &local_F4, 0, 2);
      } else {
        local_88 = local_94;
        Script_charaPosRotUtils(arg1, 7, &local_F4, 0, 2);
      }
      local_FF70 = local_88;
      {
        int32_t unused_FF5C;
        Vector3i local_FF50;
        Vector3 local_FF44;
        Script_EVT_GetPosition(arg1, -1, &local_FF44, 0);
        Script_mtwkGetSetRotation(arg1, &local_FF50, 0);
        local_84 = (&local_FF50)->y;
        {
          float local_FF40;
          float local_FF3C;
          local_FF3C = (&local_F4)->z * Script_MathUtils(SCRIPT_MATH_COS_FIXED16, local_84) - (&local_F4)->x * Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, local_84);
          local_FF40 = (&local_F4)->z * Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, local_84) + (&local_F4)->x * Script_MathUtils(SCRIPT_MATH_COS_FIXED16, local_84);
          local_F4.x = local_FF40 + (&local_FF44)->x;
          (&local_F4)->y = (&local_F4)->y + (&local_FF44)->y;
          (&local_F4)->z = local_FF3C + (&local_FF44)->z;
        }
      }
    } else {
      int32_t unused_FF5C;
      Vector3i local_FF50;
      Script_mtwkGetSetRotation(arg1, &local_FF50, 0);
      local_98 = (&local_FF50)->y;
      local_94 = (&local_FF50)->y;
      local_88 = (&local_FF50)->y;
      local_FF70 = (&local_FF50)->y;
      Script_EVT_GetPosition(arg1, -1, &local_F4, 0);
      local_84 = 0;
    }
  }
  script_0345(arg0, local_A4, arg2, arg3, arg4);
  local_A4 = local_A4 | ((Script_FloatUtils(ScriptMathOp_FloatToInt, local_B8 - 3.0f) << 16) & 0x7FFF0000);
  script_9549(arg0, 0);
  local_D0.x = (&task_1D4)->x;
  (&local_D0)->y = task_1D4.y;
  (&local_D0)->z = task_1D4.z;
  Script_ryoControl(PCCTL_StartCameraFacingInterpolation, 0);
  Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
  Script_SetCharaMoveTarget(arg0, &task_1D4, *(&task_1E0 + 1), local_A4);
  for (;;) {
    if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0) == 0) {
      break;
    }
    if (Script_GetCharaMoveStatus(arg0) == 0) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  local_A4 = local_A4 & 0xFFFF;
  {
    int32_t local_FF5C;
    int8_t local_FF5B;
    if (arg0 == task_1CC) {
      (void)((local_FF5B = 0) & 0x80);
    } else {
      (void)((local_FF5B = -1) & 0x80);
    }
    if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
      local_C4 = Script_GetCharaMotionRemainingFramesScaled(arg0);
    }
    for (;;) {
      if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0) == 0) {
        Script_sprintf("This Motion don't Finish!!\012");
        break;
      }
      if (arg0 == task_1CC) {
        if (Script_IsCharaMotionFinished(arg0)) {
          break;
        }
        local_C4 = Script_GetCharaMotionRemainingFramesScaled(arg0);
      }
      if (local_FF5B & 8) {
        if (!(local_C4 <= local_B0)) {
          (void)((local_FF5B = 0) & 1);
          {
            uint8_t unused_FF54[7];
            int32_t local_FF50;
            local_FF50 = 1;
            if (local_FF50) {
              script_259B(arg0, task_104[arg2]);
            }
          }
        }
      }
      if (!(local_C4 > local_B4)) {
        if (!(local_C4 < local_B8) & !(local_C4 > local_B4)) {
          if (arg0 == task_1CC) {
            if (local_CF == 1) {
              local_C8 = local_C8 - 0.165526f;
            } else {
              local_C8 = local_C8 - 0.145973f;
            }
          }
        }
        if (!(local_C4 < local_B8) & !(local_C4 >= local_B8 + 1.0f)) {
          uint8_t unused_FF54[7];
          int32_t local_FF50;
          local_FF50 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_B4[arg2], 2);
          local_FF50 = Script_ScenDataUtil(task_74, 4, Script_ScenDataUtil(task_78, 4, local_FF50 * 9 + 1) * 4 + 2);
          if (local_FF50 != -1) {
            if (arg0 == task_1CC) {
              Script_executeAicaCommand(local_FF50, 0, 0);
            } else {
              Script_Execute3DSoundCommand_Wrapper(local_FF50, 4, &local_D0, 5.0f, 20.0f, 0);
            }
          }
        }
        Script_EVT_GetPosition(arg0, local_A0, &local_E8, ENTITY_XFORM_WORLD_SPACE);
        local_88 = Script_FloatUtils(ScriptMathOp_Atan2ReverseVector, (&local_F4)->x - (&local_E8)->x, (&local_F4)->z - (&local_E8)->z);
        local_88 = local_88 - local_84;
        local_88 = local_88 + local_90 + (int32_t)(local_C8 * local_C0 / 360.0f * (float)0x10000);
        if (local_CE) {
          if ((arg4 == 2) | (arg4 == 3)) {
            Script_charaPosRotUtils(arg1, 12, 0, &local_FF64, 2);
          } else {
            Script_charaPosRotUtils(arg1, 7, 0, &local_FF64, 2);
          }
        } else {
          Script_mtwkGetSetRotation(arg1, &local_FF64, 0);
        }
        if ((local_CF == 0) & !(local_C4 >= local_B8)) {
          local_88 = (&local_FF64)->y;
        }
        local_FF60 = script_02CD((&local_FF64)->y & 0xFFFF, local_88 & 0xFFFF);
        if (local_FF60 > 910) {
          local_88 = (&local_FF64)->y + 910;
        } else {
          if (local_FF60 < -910) {
            local_88 = (&local_FF64)->y - 910;
            goto L00535C;
          }
        }
        L00535C:;
        if (!(local_C4 >= local_B4)) {
          if (!(local_C0 <= 0.0f)) {
            if (script_02CD(local_FF70, local_88) > 0) {
              local_88 = local_FF70;
            }
          } else {
            if (script_02CD(local_FF70, local_88) < 0) {
              local_88 = local_FF70;
            }
          }
        }
      }
      if (arg0 != task_1CC) {
        if (!(local_B4 > local_C4) & !(local_C4 >= local_B4 + 1.0f)) {
          uint8_t unused_FF54[7];
          Vector3i local_FF48;
          if (local_CE) {
            if ((arg4 == 2) | (arg4 == 3)) {
              Script_charaPosRotUtils(arg1, 12, 0, &local_FF48, 2);
            } else {
              Script_charaPosRotUtils(arg1, 7, 0, &local_FF48, 2);
            }
          } else {
            Script_mtwkGetSetRotation(arg1, &local_FF48, 0);
          }
          local_FF74 = (float)script_0290(script_02CD(local_FF70, (&local_FF48)->y) * 360 / 0x10000);
        }
        if (!(local_C4 <= local_B4) & !(local_C4 > local_A8)) {
          *(int32_t *)&local_BC = Script_ScenDataUtil(data_F778, 5, Script_ScenDataUtil(data_F188, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9 + 8);
          if (!(local_FF74 < 90.0f)) {
            local_BC = 0.0f;
          } else {
            local_FF74 = local_FF74 + local_BC;
            if (!(local_FF74 <= 90.0f)) {
              local_BC = local_FF74 - 90.0f;
            }
          }
          local_88 = local_88 - (int32_t)(local_BC * local_C0 / 360.0f * (float)0x10000);
        }
      }
      if (!(local_C4 < local_B0) & !(local_C4 >= local_B0 + 1.0f)) {
        if (local_80 == 0) {
          if (arg0 != task_1CC) {
            uint8_t unused_FF54[7];
            Vector3i local_FF48;
            int32_t local_FF44;
            if (local_CE) {
              if ((arg4 == 2) | (arg4 == 3)) {
                Script_charaPosRotUtils(arg1, 12, 0, &local_FF48, 2);
              } else {
                Script_charaPosRotUtils(arg1, 7, 0, &local_FF48, 2);
              }
            } else {
              Script_mtwkGetSetRotation(arg1, &local_FF48, 0);
            }
            if (local_CE) {
              if ((arg4 == 2) | (arg4 == 3)) {
                if (!((float)(((&local_FF48)->y - local_98) & 0xFFFF) * 360.0f / (float)0x10000 <= 180.0f)) {
                  local_FF44 = local_98 + 0x10000 - (&local_FF48)->y;
                } else {
                  local_FF44 = (&local_FF48)->y - local_98;
                }
              } else if (!((float)(((&local_FF48)->y - local_94) & 0xFFFF) * 360.0f / (float)0x10000 <= 180.0f)) {
                local_FF44 = local_94 + 0x10000 - (&local_FF48)->y;
              } else {
                local_FF44 = (&local_FF48)->y - local_94;
              }
            } else if (!((float)(((&local_FF48)->y - local_98) & 0xFFFF) * 360.0f / (float)0x10000 <= 180.0f)) {
              local_FF44 = local_98 + 0x10000 - (&local_FF48)->y;
            } else {
              local_FF44 = (&local_FF48)->y - local_98;
            }
            local_FF7C = (int32_t)((float)(local_FF44 & 0xFFFF) * 360.0f / (float)0x10000 / (local_AC - local_B0) * local_C0 / 360.0f * (float)0x10000);
            local_80 = 1;
          }
        }
      }
      if (!(local_C4 < local_AC) & !(local_C4 >= local_AC + 1.0f)) {
        uint8_t unused_FF54[7];
        int32_t local_FF50;
        local_FF50 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_B4[arg2], 2);
        local_FF50 = Script_ScenDataUtil(task_74, 4, Script_ScenDataUtil(task_78, 4, local_FF50 * 9 + 1) * 4 + 3);
        if (local_FF50 != -1) {
          if (arg0 == task_1CC) {
            Script_executeAicaCommand(local_FF50, 0, 0);
          } else {
            Script_Execute3DSoundCommand_Wrapper(local_FF50, 4, &local_D0, 5.0f, 20.0f, 0);
          }
        }
        local_A4 = task_B4[arg2];
        if (arg0 == task_1CC) {
          if (task_1CC == arg0) {
            local_A0 = (Script_ScenDataUtil(task_6C, 4, local_A4 * 13 + 2) >> 16) & 0xFFFF;
            Script_scenToggleCharaFlag(0, local_A0, 1);
            local_A0 = Script_ScenDataUtil(task_6C, 4, local_A4 * 13 + 2) & 0xFFFF;
            Script_scenToggleCharaFlag(0, local_A0, 1);
          }
        }
      }
      if (!(local_AC <= 0.0f)) {
        if (!(local_C4 <= local_AC)) {
          if (arg0 == task_1CC) {
            local_FF5C = arg3;
            if ((local_FF5C == 30) | (local_FF5C == 31) | (local_FF5C == 32) | (local_FF5C == 33) | (local_FF5C == 34) | (local_FF5C == 35)) {
              local_FF78 = local_FF78 - 0.06f;
              if (!(local_FF78 >= 0.3f)) {
                local_FF78 = 0.3f;
              }
              DSWK_SetActorOpacityAndDrawCallback(arg1, local_FF78);
              goto L005808;
            }
          }
        }
      }
      L005808:;
      if (!(local_C4 < local_B0) & !(local_C4 > local_AC)) {
        if (arg0 == task_1CC) {
          if (local_CF == 0) {
            local_C8 = local_C8 - 0.11939f;
          } else {
            local_C8 = local_C8 + 2.381415f;
          }
          Script_EVT_GetPosition(arg0, local_9C, &local_E8, ENTITY_XFORM_WORLD_SPACE);
          local_88 = Script_FloatUtils(ScriptMathOp_Atan2ReverseVector, (&local_F4)->x - (&local_E8)->x, (&local_F4)->z - (&local_E8)->z);
          local_88 = local_88 - local_84;
          local_88 = local_88 + local_8C + (int32_t)(local_C8 * local_C0 / 360.0f * (float)0x10000);
        } else {
          local_88 = local_88 + local_FF7C;
        }
      }
      if (local_CE) {
        uint8_t unused_FF54[7];
        Vector3i local_FF48;
        local_FF48.x = 0;
        (&local_FF48)->z = 0;
        if ((arg4 == 2) | (arg4 == 3)) {
          (&local_FF48)->y = local_88;
          Script_charaPosRotUtils(arg1, 12, 0, &local_FF48, 0);
        } else {
          (&local_FF48)->y = local_88;
          Script_charaPosRotUtils(arg1, 7, 0, &local_FF48, 0);
        }
      } else {
        uint8_t unused_FF54[7];
        Vector3i local_FF48;
        (&local_FF48)->y = local_88;
        Script_EVT_SetRotation(arg1, MATRIX_XFORM_Y, &local_FF48, 0);
      }
      if (arg0 != task_1CC) {
        if (!(local_C4 < local_AC)) {
          break;
        }
        local_C4 = local_C4 + 1.0f;
      }
      SCNF_WaitFrames_0000(1);
    }
  }
  script_9549(arg0, 1);
  if (local_CE) {
    int32_t unused_FF5C;
    Vector3i local_FF50;
    local_FF50.x = 0;
    (&local_FF50)->z = 0;
    (&local_FF50)->y = local_98;
    Script_charaPosRotUtils(arg1, 12, 0, &local_FF50, 0);
    (&local_FF50)->y = local_94;
    Script_charaPosRotUtils(arg1, 7, 0, &local_FF50, 0);
  } else {
    {
      int32_t unused_FF5C;
      Vector3i local_FF50;
      (&local_FF50)->y = local_98;
      Script_EVT_SetRotation(arg1, MATRIX_XFORM_Y, &local_FF50, 0);
    }
    {
      int32_t unused_FF5C;
      Vector3i local_FF50;
      (&local_FF50)->y = local_94;
      Script_EVT_SetRotation(arg1, MATRIX_XFORM_Y, &local_FF50, 0);
    }
  }
  if (arg0 == task_1CC) {
    int32_t local_FF5C;
    local_FF5C = arg3;
    if ((local_FF5C == 30) | (local_FF5C == 31) | (local_FF5C == 32) | (local_FF5C == 33) | (local_FF5C == 34) | (local_FF5C == 35)) {
      for (;;) {
        if (Script_CamCtrl(CamCtrlCmd_CheckPosRotCollision) != -1) {
          break;
        }
        SCNF_WaitFrames_0000(1);
      }
      DSWK_SetActorOpacityAndDrawCallback(arg1, 1.0f);
      goto L005A43;
    }
  }
  L005A43:;
  return;
}

__attribute__((scn_string_bindings(0, data_10D0A)))
static void script_5A44(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
  Vector3 local_F4;
  Vector3 local_E8;
  Vector3 local_DC;
  Vector3 local_D0;
  Vector3 local_C4;
  float local_C0;
  float local_BC;
  float local_B8;
  float local_B4;
  int32_t local_B0;
  float local_AC;
  float local_A8;
  float local_A4;
  int16_t local_A2;
  uint8_t unused_A0[2];
  int32_t local_9C;
  int32_t local_98;
  local_A8 = 30.0f;
  local_A4 = 60.0f;
  if (arg5) {
    Script_charaPosRotUtils(arg1, 12, &local_DC, 0, 2);
    Script_charaPosRotUtils(arg1, 7, &local_D0, 0, 2);
  } else {
    Script_EVT_GetPosition(arg1, -1, &local_DC, 0);
    Script_EVT_GetPosition(arg1, -1, &local_D0, 0);
  }
  local_F4.x = 0.0f;
  (&local_F4)->y = 0.0f;
  (&local_F4)->z = 0.0f;
  local_E8.x = 0.0f;
  (&local_E8)->y = 0.0f;
  (&local_E8)->z = 0.0f;
  local_C0 = 0.0f;
  local_A2 = 0;
  local_B8 = 0.0f;
  local_B4 = 0.0f;
  local_B0 = 0;
  {
    int32_t local_94;
    Vector3i local_88;
    Script_mtwkGetSetRotation(arg1, &local_88, 0);
    if ((arg4 == 1) | (arg4 == 3)) {
      local_94 = 1395;
      local_98 = 18;
      local_9C = 18;
      local_AC = 1.0f;
    } else {
      local_94 = -0x7FFFFA8D;
      local_98 = 12;
      local_9C = 12;
      local_AC = -1.0f;
    }
    local_94 = local_94 | ((Script_FloatUtils(ScriptMathOp_FloatToInt, local_A8 - 3.0f) << 16) & 0x7FFF0000);
    script_9549(arg0, 0);
    local_C4.x = (&task_1D4)->x;
    (&local_C4)->y = task_1D4.y;
    (&local_C4)->z = task_1D4.z;
    Script_ryoControl(PCCTL_StartCameraFacingInterpolation, 0);
    Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
    Script_SetCharaMoveTarget(arg0, &task_1D4, *(&task_1E0 + 1), local_94);
    for (;;) {
      if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0) == 0) {
        break;
      }
      if (Script_GetCharaMoveStatus(arg0) == 0) {
        break;
      }
      SCNF_WaitFrames_0000(1);
    }
    local_94 = local_94 & 0xFFFF;
    for (;;) {
      if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0) == 0) {
        Script_sprintf("This Motion don't Finish!!\012");
        break;
      }
      if (Script_IsCharaMotionFinished(arg0)) {
        break;
      }
      local_BC = Script_GetCharaMotionRemainingFramesScaled(arg0);
      Script_ReadMOTNPositionAtFrame(arg0, local_94, local_BC, &local_E8);
      local_E8.x = (&local_E8)->x * local_AC;
      if (!(local_BC >= local_A8)) {
        local_A2 = 0;
      }
      if (!(local_BC < local_A8) & !(local_BC > local_A4)) {
        if (!(local_BC < local_A8) & !(local_BC >= local_A8 + 1.0f)) {
          int32_t local_84;
          local_84 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_B4[arg2], 2);
          local_84 = Script_ScenDataUtil(task_74, 4, Script_ScenDataUtil(task_78, 4, local_84 * 9 + 1) * 4 + 3);
          if (local_84 != -1) {
            if (arg0 == task_1CC) {
              Script_executeAicaCommand(local_84, 0, 0);
            } else {
              Script_Execute3DSoundCommand_Wrapper(local_84, 4, &local_C4, 5.0f, 20.0f, 0);
            }
          }
        }
        Script_EVT_GetPosition(arg0, local_9C, &local_F4, ENTITY_XFORM_MOTION_SPACE);
        if (local_A2 == 1) {
          local_B4 = (&local_E8)->x;
          local_C0 = (&local_F4)->x;
        }
        local_B8 = (&local_E8)->x + ((&local_F4)->x - local_C0);
        local_B8 = local_B8 - local_B4;
        local_B8 = local_B8 * 1.28f;
        {
          int32_t local_84;
          local_84 = arg4;
          if ((local_84 == 0) | (local_84 == 2)) {
            if (!(local_B8 < 0.75f)) {
              local_B8 = 0.75f;
            } else {
              if (!(local_B8 > 0.0f)) {
                local_B8 = 0.0f;
                goto L005D68;
              }
            }
            L005D68:;
          } else {
            if ((local_84 == 1) | (local_84 == 3)) {
              if (!(local_B8 > -0.75f)) {
                local_B8 = -0.75f;
              } else {
                if (!(local_B8 < 0.0f)) {
                  local_B8 = 0.0f;
                  goto L005DA4;
                }
              }
              L005DA4:;
              goto L005DA7;
            }
          }
        }
      }
      L005DA7:;
      if (!(local_BC <= local_A4)) {
        int32_t local_84;
        local_84 = arg4;
        if ((local_84 == 0) | (local_84 == 2)) {
          local_B8 = 0.75f;
        } else {
          if ((local_84 == 1) | (local_84 == 3)) {
            local_B8 = -0.75f;
            goto L005DEC;
          }
        }
      }
      L005DEC:;
      {
        int32_t local_84;
        local_84 = arg4;
        switch (local_84) {
          case 0:
            local_F4.x = (&local_D0)->x;
            (&local_F4)->y = (&local_D0)->y;
            (&local_F4)->z = (&local_D0)->z;
            if (arg5) {
              local_F4.x = (&local_D0)->x - local_B8;
              Script_charaPosRotUtils(arg1, 7, &local_F4, 0, 0);
            } else {
              local_F4.x = (&local_D0)->x + ((float)0 - local_B8) * Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_88)->y);
              (&local_F4)->z = (&local_D0)->z + local_B8 * Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_88)->y);
              Script_EVT_SetPosition(arg1, POS_XYZ, &local_F4);
            }
            break;
          case 1:
            local_F4.x = (&local_D0)->x;
            (&local_F4)->y = (&local_D0)->y;
            (&local_F4)->z = (&local_D0)->z;
            if (arg5) {
              local_F4.x = (&local_D0)->x + local_B8;
              Script_charaPosRotUtils(arg1, 7, &local_F4, 0, 0);
            } else {
              local_F4.x = (&local_D0)->x - ((float)0 - local_B8) * Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_88)->y);
              (&local_F4)->z = (&local_D0)->z - local_B8 * Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_88)->y);
              Script_EVT_SetPosition(arg1, POS_XYZ, &local_F4);
            }
            break;
          case 2:
            local_F4.x = (&local_DC)->x;
            (&local_F4)->y = (&local_DC)->y;
            (&local_F4)->z = (&local_DC)->z;
            if (arg5) {
              local_F4.x = (&local_DC)->x + local_B8;
              Script_charaPosRotUtils(arg1, 12, &local_F4, 0, 0);
            } else {
              local_F4.x = (&local_DC)->x - ((float)0 - local_B8) * Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_88)->y);
              (&local_F4)->z = (&local_DC)->z - local_B8 * Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_88)->y);
              Script_EVT_SetPosition(arg1, POS_XYZ, &local_F4);
            }
            break;
          case 3:
            local_F4.x = (&local_DC)->x;
            (&local_F4)->y = (&local_DC)->y;
            (&local_F4)->z = (&local_DC)->z;
            if (arg5) {
              local_F4.x = (&local_DC)->x - local_B8;
              Script_charaPosRotUtils(arg1, 12, &local_F4, 0, 0);
            } else {
              local_F4.x = (&local_DC)->x + ((float)0 - local_B8) * Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_88)->y);
              (&local_F4)->z = (&local_DC)->z + local_B8 * Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_88)->y);
              Script_EVT_SetPosition(arg1, POS_XYZ, &local_F4);
            }
            break;
        }
      }
      local_A2 = local_A2 + 1;
      SCNF_WaitFrames_0000(1);
    }
  }
  script_9549(arg0, 1);
  {
    int32_t local_94;
    local_94 = task_B4[arg2];
    local_9C = Script_ScenDataUtil(task_6C, 4, local_94 * 13 + 2) & 0xFFFF;
    Script_scenToggleCharaFlag(0, local_9C, 1);
    local_9C = (Script_ScenDataUtil(task_6C, 4, local_94 * 13 + 2) >> 16) & 0xFFFF;
    Script_scenToggleCharaFlag(0, local_9C, 1);
    {
      int32_t unused_88[3];
      int32_t local_84;
      local_84 = Script_ScenDataUtil(task_6C, 4, local_94 * 13 + 1);
      local_84 = local_84 & -9;
      Script_ScenDataUtil(task_6C, 14, local_94 * 13 + 1, local_84);
    }
    {
      int32_t unused_88[3];
      int32_t local_84;
      local_84 = Script_ScenDataUtil(task_6C, 4, local_94 * 13 + 1);
      local_84 = local_84 & -17;
      Script_ScenDataUtil(task_6C, 14, local_94 * 13 + 1, local_84);
    }
    {
      int32_t unused_88[3];
      int32_t local_84;
      local_84 = Script_ScenDataUtil(task_6C, 4, local_94 * 13 + 1);
      local_84 = local_84 & -5;
      Script_ScenDataUtil(task_6C, 14, local_94 * 13 + 1, local_84);
    }
  }
  return;
}

__attribute__((scn_string_bindings(0, data_10D26)))
static void script_6165(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
  Vector3i local_F4;
  Vector3 local_E8;
  float local_E4;
  float local_E0;
  Vector3 local_D4;
  Vector3 local_C8;
  int32_t unused_BC[3];
  Vector3 local_B0;
  int8_t local_AF;
  uint8_t unused_AC[3];
  float local_A8;
  float local_A4;
  float local_A0;
  float local_9C;
  float local_98;
  float local_94;
  float local_90;
  int32_t local_8C;
  int32_t local_88;
  int32_t local_84;
  int32_t local_80;
  int32_t local_FF7C;
  int32_t local_FF78;
  int32_t local_FF74;
  int32_t local_FF70;
  int32_t local_FF6C;
  int32_t local_FF68;
  local_FF68 = arg4;
  switch (local_FF68) {
    case 0:
      local_AF = 0;
      local_8C = Script_ScenDataUtil(data_F778, 4, Script_ScenDataUtil(data_F188, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9);
      local_A0 = -1.0f;
      local_88 = 18;
      local_84 = 18;
      local_FF78 = 46421;
      local_FF74 = 48787;
      break;
    case 1:
      local_AF = 1;
      local_8C = Script_ScenDataUtil(data_F778, 4, Script_ScenDataUtil(data_F188, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9) | -0x80000000;
      local_A0 = -1.0f;
      local_88 = 12;
      local_84 = 12;
      local_FF78 = 50426;
      local_FF74 = 48969;
      break;
    case 2:
      local_AF = 1;
      local_8C = Script_ScenDataUtil(data_F778, 4, Script_ScenDataUtil(data_F188, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9);
      local_A0 = 1.0f;
      local_88 = 18;
      local_84 = 18;
      local_FF78 = 15109;
      local_FF74 = 16566;
      break;
    case 3:
      local_AF = 0;
      local_8C = Script_ScenDataUtil(data_F778, 4, Script_ScenDataUtil(data_F188, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9) | -0x80000000;
      local_A0 = 1.0f;
      local_88 = 12;
      local_84 = 12;
      local_FF78 = 19114;
      local_FF74 = 16748;
      break;
  }
  (void)local_FF78;
  *(int32_t *)&local_9C = Script_ScenDataUtil(data_F778, 5, Script_ScenDataUtil(data_F188, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9 + 1);
  *(int32_t *)&local_98 = Script_ScenDataUtil(data_F778, 5, Script_ScenDataUtil(data_F188, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9 + 2);
  *(int32_t *)&local_94 = Script_ScenDataUtil(data_F778, 5, Script_ScenDataUtil(data_F188, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9 + 3);
  *(int32_t *)&local_90 = Script_ScenDataUtil(data_F778, 5, Script_ScenDataUtil(data_F188, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9 + 4);
  local_A8 = 0.0f;
  Script_charaPosRotUtils(arg1, 12, 0, &local_F4, 2);
  local_80 = (&local_F4)->y;
  Script_charaPosRotUtils(arg1, 7, 0, &local_F4, 2);
  local_FF7C = (&local_F4)->y;
  if ((arg4 == 2) | (arg4 == 3)) {
    local_FF70 = local_80;
    Script_charaPosRotUtils(arg1, 12, &local_D4, 0, 2);
  } else {
    local_FF70 = local_FF7C;
    Script_charaPosRotUtils(arg1, 7, &local_D4, 0, 2);
  }
  Script_EVT_GetPosition(arg1, -1, &local_E8, 0);
  Script_mtwkGetSetRotation(arg1, &local_F4, 0);
  local_FF6C = (&local_F4)->y;
  local_E0 = (&local_D4)->z * Script_MathUtils(SCRIPT_MATH_COS_FIXED16, local_FF6C) - (&local_D4)->x * Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, local_FF6C);
  local_E4 = (&local_D4)->z * Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, local_FF6C) + (&local_D4)->x * Script_MathUtils(SCRIPT_MATH_COS_FIXED16, local_FF6C);
  local_D4.x = local_E4 + (&local_E8)->x;
  (&local_D4)->y = (&local_D4)->y + (&local_E8)->y;
  (&local_D4)->z = local_E0 + (&local_E8)->z;
  local_8C = local_8C | ((Script_FloatUtils(ScriptMathOp_FloatToInt, local_9C - 3.0f) << 16) & 0x7FFF0000);
  script_9549(arg0, 0);
  local_B0.x = (&task_1D4)->x;
  (&local_B0)->y = task_1D4.y;
  (&local_B0)->z = task_1D4.z;
  Script_ryoControl(PCCTL_StartCameraFacingInterpolation, 0);
  Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
  Script_SetCharaMoveTarget(arg0, &task_1D4, *(&task_1E0 + 1), local_8C);
  for (;;) {
    if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0) == 0) {
      break;
    }
    if (Script_GetCharaMoveStatus(arg0) == 0) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  local_8C = local_8C & 0xFFFF;
  for (;;) {
    if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0) == 0) {
      Script_sprintf("This Motion don't Finish!!\012");
      break;
    }
    if (Script_IsCharaMotionFinished(arg0)) {
      break;
    }
    local_A4 = Script_GetCharaMotionRemainingFramesScaled(arg0);
    if (!(local_A4 < local_9C) & !(local_A4 > local_98)) {
      if (local_AF == 1) {
      } else {
        local_A8 = local_A8 - 0.208874f;
      }
      if (!(local_A4 < local_9C) & !(local_A4 >= local_9C + 1.0f)) {
        int32_t local_FF64;
        local_FF64 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_B4[arg2], 2);
        local_FF64 = Script_ScenDataUtil(task_74, 4, Script_ScenDataUtil(task_78, 4, local_FF64 * 9 + 1) * 4 + 2);
        if (local_FF64 != -1) {
          if (arg0 == task_1CC) {
            Script_executeAicaCommand(local_FF64, 0, 0);
          } else {
            Script_Execute3DSoundCommand_Wrapper(local_FF64, 4, &local_B0, 5.0f, 20.0f, 0);
          }
        }
      }
      Script_EVT_GetPosition(arg0, local_88, &local_C8, ENTITY_XFORM_WORLD_SPACE);
      local_FF70 = Script_FloatUtils(ScriptMathOp_Atan2ReverseVector, (&local_D4)->x - (&local_C8)->x, (&local_D4)->z - (&local_C8)->z);
      local_FF70 = local_FF70 - local_FF6C;
      local_FF70 = local_FF70 + local_FF78 + (int32_t)(local_A8 * local_A0 / 360.0f * (float)0x10000);
    }
    if (!(local_A4 < local_94) & !(local_A4 > local_90)) {
      if (local_AF == 0) {
      } else {
        local_A8 = local_A8 + 0.12844f;
      }
      if (!(local_A4 < local_90) & !(local_A4 >= local_90 + 1.0f)) {
        int32_t local_FF64;
        local_FF64 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_B4[arg2], 2);
        local_FF64 = Script_ScenDataUtil(task_74, 4, Script_ScenDataUtil(task_78, 4, local_FF64 * 9 + 1) * 4 + 3);
        if (local_FF64 != -1) {
          if (arg0 == task_1CC) {
            Script_executeAicaCommand(local_FF64, 0, 0);
          } else {
            Script_Execute3DSoundCommand_Wrapper(local_FF64, 4, &local_B0, 5.0f, 20.0f, 0);
          }
        }
      }
      Script_EVT_GetPosition(arg0, local_84, &local_C8, ENTITY_XFORM_WORLD_SPACE);
      local_FF70 = Script_FloatUtils(ScriptMathOp_Atan2ReverseVector, (&local_D4)->x - (&local_C8)->x, (&local_D4)->z - (&local_C8)->z);
      local_FF70 = local_FF70 - local_FF6C;
      local_FF70 = local_FF70 + local_FF74 + (int32_t)(local_A8 * local_A0 / 360.0f * (float)0x10000);
    }
    local_F4.x = 0;
    (&local_F4)->z = 0;
    if ((arg4 == 2) | (arg4 == 3)) {
      (&local_F4)->y = local_FF70;
      Script_charaPosRotUtils(arg1, 12, 0, &local_F4, 0);
    } else {
      (&local_F4)->y = local_FF70;
      Script_charaPosRotUtils(arg1, 7, 0, &local_F4, 0);
    }
    SCNF_WaitFrames_0000(1);
  }
  script_9549(arg0, 1);
  local_F4.x = 0;
  (&local_F4)->z = 0;
  (&local_F4)->y = local_80;
  Script_charaPosRotUtils(arg1, 12, 0, &local_F4, 0);
  (&local_F4)->y = local_FF7C;
  Script_charaPosRotUtils(arg1, 7, 0, &local_F4, 0);
  return;
}

static void script_68EF(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
  int32_t local_FC;
  int32_t local_F8;
  Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
  Script_SetCharaMoveTarget(arg0, &task_1D4, *(&task_1E0 + 1), MN_AKI_AKI_BASE);
  for (;;) {
    if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0) == 0) {
      break;
    }
    if (Script_GetCharaMoveStatus(arg0) == 0) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  local_F8 = 0;
  for (;;) {
    if (local_F8 == 0) {
      local_F8 = 1;
      if (arg5 != 0) {
        local_FC = 11;
      } else {
        local_FC = Script_ScenDataUtil(task_6C, 4, task_B4[arg2] * 13 + 7);
      }
      if (local_FC == 0) {
        SCNF_WaitFrames_0000(45);
      }
      Script_EnqueueSCNT(1, 0x47524F44, local_FC);
    } else {
      if (Script_EnqueueSCNT(2) == 0) {
        break;
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  return;
}

__attribute__((scn_string_bindings(0, data_10D42)))
static void script_69C7(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
  Vector3 local_F4;
  float local_F0;
  float local_EC;
  float local_E8;
  int32_t unused_E4;
  int32_t local_E0;
  int32_t local_DC;
  int32_t local_D8;
  int32_t local_D4;
  int32_t unused_D0;
  int32_t local_CC;
  local_DC = 0;
  local_D4 = Script_ScenDataUtil(task_6C, 4, task_B4[arg2] * 13 + 6);
  local_CC = arg3;
  if ((local_CC == 1) | (local_CC == 3) | (local_CC == 5) | (local_CC == 7) | (local_CC == 9) | (local_CC == 18) | (local_CC == 21) | (local_CC == 25) | (local_CC == 27) | (local_CC == 36) | (local_CC == 37)) {
    local_CC = arg4;
    switch (local_CC) {
      case 1:
        local_E0 = Script_ScenDataUtil(&data_10198, 4, local_D4 * 5) | -0x80000000;
        break;
      case 0:
        local_E0 = Script_ScenDataUtil(&data_10198, 4, local_D4 * 5);
        break;
    }
  } else if ((local_CC == 2) | (local_CC == 4) | (local_CC == 6) | (local_CC == 8) | (local_CC == 10) | (local_CC == 19) | (local_CC == 22) | (local_CC == 24) | (local_CC == 26) | (local_CC == 28)) {
    local_CC = arg4;
    switch (local_CC) {
      case 1:
        local_E0 = Script_ScenDataUtil(&data_10198, 4, local_D4 * 5);
        break;
      case 0:
        local_E0 = Script_ScenDataUtil(&data_10198, 4, local_D4 * 5) | -0x80000000;
        break;
    }
  } else {
    if ((local_CC == 13) | (local_CC == 14) | (local_CC == 15) | (local_CC == 16) | (local_CC == 20) | (local_CC == 23) | (local_CC == 29)) {
      local_CC = arg4;
      switch (local_CC) {
        case 0:
          local_E0 = Script_ScenDataUtil(&data_10198, 4, local_D4 * 5);
          break;
        case 1:
          local_E0 = Script_ScenDataUtil(&data_10198, 4, local_D4 * 5) | -0x80000000;
          break;
        case 2:
          local_E0 = Script_ScenDataUtil(&data_10198, 4, local_D4 * 5);
          break;
        case 3:
          local_E0 = Script_ScenDataUtil(&data_10198, 4, local_D4 * 5) | -0x80000000;
          break;
      }
      goto L006BE8;
    }
  }
  L006BE8:;
  *(int32_t *)&local_EC = Script_ScenDataUtil(&data_10198, 5, local_D4 * 5 + 1);
  *(int32_t *)&local_E8 = Script_ScenDataUtil(&data_10198, 5, local_D4 * 5 + 2);
  local_E0 = local_E0 | ((Script_FloatUtils(ScriptMathOp_FloatToInt, local_EC - 3.0f) << 16) & 0x7FFF0000);
  local_F4.x = (&task_1D4)->x;
  (&local_F4)->y = task_1D4.y;
  (&local_F4)->z = task_1D4.z;
  Script_ryoControl(PCCTL_StartCameraFacingInterpolation, 0);
  Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
  Script_SetCharaMoveTarget(arg0, &task_1D4, *(&task_1E0 + 1), local_E0);
  for (;;) {
    if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0) == 0) {
      break;
    }
    if (Script_GetCharaMoveStatus(arg0) == 0) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  for (;;) {
    if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0) == 0) {
      Script_sprintf("This Motion don't Finish!!\012");
      break;
    }
    if (Script_IsCharaMotionFinished(arg0)) {
      if (local_DC == 1) {
        if (Script_EnqueueSCNT(2) == 0) {
          break;
        }
      }
    }
    local_F0 = Script_GetCharaMotionRemainingFramesScaled(arg0);
    if (!(local_F0 < local_EC) & !(local_F0 >= local_EC + 1.0f)) {
      int32_t local_C8;
      local_C8 = Script_ScenDataUtil(task_6C, 4, task_B4[arg2] * 13 + 9);
      if (local_C8 != -1) {
        if (arg0 == task_1CC) {
          Script_executeAicaCommand(local_C8, 0, 0);
        } else {
          Script_Execute3DSoundCommand_Wrapper(local_C8, 4, &local_F4, 5.0f, 20.0f, 0);
        }
      }
    }
    if (!(local_F0 < local_E8) & !(local_F0 >= local_E8 + 1.0f)) {
      if ((local_D4 == 6) | (local_D4 == 7)) {
        int32_t local_C8;
        local_C8 = Script_ScenDataUtil(task_6C, 4, task_B4[arg2] * 13 + 9);
        if (local_C8 != -1) {
          if (arg0 == task_1CC) {
            Script_executeAicaCommand(local_C8, 0, 0);
          } else {
            Script_Execute3DSoundCommand_Wrapper(local_C8, 4, &local_F4, 5.0f, 20.0f, 0);
          }
        }
      }
      if (local_DC == 0) {
        local_DC = 1;
        local_D8 = Script_ScenDataUtil(task_6C, 4, task_B4[arg2] * 13 + 7);
        if (local_D8 == 0) {
          SCNF_WaitFrames_0000(45);
        }
        Script_EnqueueSCNT(1, 0x47524F44, local_D8);
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  return;
}

__attribute__((scn_string_bindings(0, data_10D5E)))
static void script_6E3E(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
  Vector3 local_F4;
  int32_t unused_E8[3];
  float local_DC[3];
  Vector3 local_D0;
  float local_C4[3];
  Vector3 local_B8;
  int32_t unused_AC[3];
  int8_t local_AB;
  int8_t local_AA;
  uint8_t unused_A8[2];
  float local_A4;
  float local_A0;
  float local_9C;
  float local_98;
  float local_94;
  int32_t local_90;
  int32_t local_8C;
  int32_t unused_88;
  int32_t local_84;
  int32_t local_80;
  int32_t local_FF7C;
  int32_t local_FF78;
  int32_t local_FF74;
  int32_t unused_FF70;
  int32_t local_FF6C;
  int32_t local_FF68;
  int32_t local_FF64;
  int32_t local_FF60;
  int32_t local_FF5C;
  int32_t local_FF58;
  int32_t unused_FF54;
  Vector3i local_FF48;
  local_AA = 0;
  local_FF5C = 0;
  local_FF58 = 0;
  task_1F8 = 0;
  local_90 = 1543;
  Script_EVT_GetPosition(arg1, -1, &local_D0, 0);
  Script_mtwkGetSetRotation(arg1, &local_FF48, 0);
  local_DC[0] = 0.0f;
  local_DC[1] = 0.0f;
  local_DC[2] = 0.0f;
  {
    int32_t local_FF44;
    local_FF44 = arg3;
    if ((local_FF44 == 4) | (local_FF44 == 6) | (local_FF44 == 33) | (local_FF44 == 35)) {
      local_FF44 = arg4;
      switch (local_FF44) {
        case 1:
          local_AB = 1;
          local_9C = 1.0f;
          local_FF74 = 0;
          break;
        case 0:
          local_AB = 0;
          local_90 = local_90 | -0x80000000;
          local_9C = 1.0f;
          local_FF74 = 1;
          break;
      }
    } else if ((local_FF44 == 1) | (local_FF44 == 7) | (local_FF44 == 30) | (local_FF44 == 36) | (local_FF44 == 37)) {
      local_FF44 = arg4;
      switch (local_FF44) {
        case 1:
          local_AB = 0;
          local_90 = local_90 | -0x80000000;
          local_9C = 1.0f;
          local_FF74 = 1;
          break;
        case 0:
          local_AB = 1;
          local_9C = 1.0f;
          local_FF74 = 0;
          break;
      }
    } else if ((local_FF44 == 3) | (local_FF44 == 5) | (local_FF44 == 32) | (local_FF44 == 34)) {
      local_FF44 = arg4;
      switch (local_FF44) {
        case 1:
          local_AB = 1;
          local_90 = local_90 | -0x80000000;
          local_9C = -1.0f;
          local_FF74 = 1;
          break;
        case 0:
          local_AB = 0;
          local_9C = -1.0f;
          local_FF74 = 0;
          break;
      }
    } else if ((local_FF44 == 2) | (local_FF44 == 8) | (local_FF44 == 31)) {
      local_FF44 = arg4;
      switch (local_FF44) {
        case 1:
          local_AB = 0;
          local_9C = -1.0f;
          local_FF74 = 0;
          break;
        case 0:
          local_AB = 1;
          local_90 = local_90 | -0x80000000;
          local_9C = -1.0f;
          local_FF74 = 1;
          break;
      }
    } else {
      if ((local_FF44 == 13) | (local_FF44 == 14)) {
        local_FF44 = arg4;
        switch (local_FF44) {
          case 0:
            local_AB = 0;
            local_9C = -1.0f;
            local_FF74 = 0;
            break;
          case 1:
            local_AB = 1;
            local_90 = local_90 | -0x80000000;
            local_9C = -1.0f;
            local_FF74 = 1;
            break;
          case 2:
            local_AB = 1;
            local_9C = 1.0f;
            local_FF74 = 0;
            break;
          case 3:
            local_AB = 0;
            local_90 = local_90 | -0x80000000;
            local_9C = 1.0f;
            local_FF74 = 1;
            break;
        }
        local_AA = 1;
        goto L00714A;
      }
    }
  }
  L00714A:;
  if (local_AA) {
    {
      int32_t unused_FF44;
      Vector3i local_FF38;
      Script_charaPosRotUtils(arg1, 12, 0, &local_FF38, 2);
      local_84 = (&local_FF38)->y;
    }
    {
      int32_t unused_FF44;
      Vector3i local_FF38;
      Script_charaPosRotUtils(arg1, 7, 0, &local_FF38, 2);
      local_80 = (&local_FF38)->y;
    }
    if ((arg4 == 2) | (arg4 == 3)) {
      local_FF7C = local_84;
      Script_charaPosRotUtils(arg1, 12, &local_F4, 0, 2);
    } else {
      local_FF7C = local_80;
      Script_charaPosRotUtils(arg1, 7, &local_F4, 0, 2);
    }
    {
      int32_t unused_FF44;
      Vector3i local_FF38;
      Vector3 local_FF2C;
      Script_EVT_GetPosition(arg1, -1, &local_FF2C, 0);
      Script_mtwkGetSetRotation(arg1, &local_FF38, 0);
      local_FF78 = (&local_FF38)->y;
      {
        float local_FF28;
        float local_FF24;
        local_FF24 = (&local_F4)->z * Script_MathUtils(SCRIPT_MATH_COS_FIXED16, local_FF78) - (&local_F4)->x * Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, local_FF78);
        local_FF28 = (&local_F4)->z * Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, local_FF78) + (&local_F4)->x * Script_MathUtils(SCRIPT_MATH_COS_FIXED16, local_FF78);
        local_F4.x = local_FF28 + (&local_FF2C)->x;
        (&local_F4)->y = (&local_F4)->y + (&local_FF2C)->y;
        (&local_F4)->z = local_FF24 + (&local_FF2C)->z;
      }
    }
  } else {
    int32_t unused_FF44;
    Vector3i local_FF38;
    Script_mtwkGetSetRotation(arg1, &local_FF38, 0);
    local_84 = (&local_FF38)->y;
    local_80 = (&local_FF38)->y;
    local_FF7C = (&local_FF38)->y;
    Script_EVT_GetPosition(arg1, -1, &local_F4, 0);
    local_FF78 = 0;
  }
  local_98 = 33.0f;
  local_94 = 49.0f;
  local_B8.x = (&task_1D4)->x;
  (&local_B8)->y = task_1D4.y;
  (&local_B8)->z = task_1D4.z;
  Script_ryoControl(PCCTL_StartCameraFacingInterpolation, 0);
  Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
  Script_SetCharaMoveTarget(arg0, &task_1D4, *(&task_1E0 + 1), local_90);
  for (;;) {
    if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0) == 0) {
      break;
    }
    if (Script_GetCharaMoveStatus(arg0) == 0) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT, 0x524F4F44, 1);
  while ((local_8C = Script_CycleManagerUtils(CYCLE_CMD_GET_CYCLE_PTR_BY_CHARA_ID, 0x524F4F44)) == 0) {
    SCNF_WaitFrames_0000(1);
  }
  for (;;) {
    if (Script_CycleManagerUtils(CYCLE_CMD_GET_NEXT_TASK_POINTER, local_8C) == 1) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_motmGetCharaFlags(SHEN_NAME('DOOR'), 0, 0);
  Script_PlayCharaMotion(SHEN_NAME('DOOR'), MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_charaDisplay(SHEN_NAME('DOOR'), CDM_Show);
  {
    int32_t local_FF44;
    local_FF44 = arg3;
    if ((local_FF44 == 0) | (local_FF44 == 1) | (local_FF44 == 3) | (local_FF44 == 5) | (local_FF44 == 7) | (local_FF44 == 9) | (local_FF44 == 11) | (local_FF44 == 25) | (local_FF44 == 27) | (local_FF44 == 30) | (local_FF44 == 32) | (local_FF44 == 34) | (local_FF44 == 36) | (local_FF44 == 37)) {
      local_FF44 = arg4;
      switch (local_FF44) {
        case 0:
          local_DC[0] = 0.45f;
          local_DC[2] = 0.5f;
          break;
        case 1:
          local_DC[0] = -0.45f;
          local_DC[2] = 0.5f;
          (&local_FF48)->y = (&local_FF48)->y + 0x8000;
          break;
        case 2:
          local_DC[0] = 0.45f;
          local_DC[2] = 0.5f;
          (&local_FF48)->y = (&local_FF48)->y + 0x8000;
          break;
        case 3:
          local_DC[0] = -0.45f;
          local_DC[2] = 0.5f;
          break;
      }
    } else if ((local_FF44 == 2) | (local_FF44 == 4) | (local_FF44 == 6) | (local_FF44 == 8) | (local_FF44 == 10) | (local_FF44 == 12) | (local_FF44 == 24) | (local_FF44 == 26) | (local_FF44 == 31) | (local_FF44 == 33) | (local_FF44 == 35)) {
      local_FF44 = arg4;
      switch (local_FF44) {
        case 0:
          local_DC[0] = -0.45f;
          local_DC[2] = 0.5f;
          break;
        case 1:
          local_DC[0] = 0.45f;
          local_DC[2] = 0.5f;
          (&local_FF48)->y = (&local_FF48)->y + 0x8000;
          break;
        case 2:
          local_DC[0] = -0.45f;
          local_DC[2] = 0.5f;
          (&local_FF48)->y = (&local_FF48)->y + 0x8000;
          break;
        case 3:
          local_DC[0] = 0.45f;
          local_DC[2] = 0.5f;
          break;
      }
    } else if ((local_FF44 == 13) | (local_FF44 == 14) | (local_FF44 == 15) | (local_FF44 == 16) | (local_FF44 == 17) | (local_FF44 == 18) | (local_FF44 == 19) | (local_FF44 == 20) | (local_FF44 == 21) | (local_FF44 == 22) | (local_FF44 == 23) | (local_FF44 == 29)) {
      local_FF44 = arg4;
      switch (local_FF44) {
        case 0:
          local_DC[0] = -0.45f;
          local_DC[2] = 0.5f;
          break;
        case 1:
          local_DC[0] = 0.45f;
          local_DC[2] = 0.5f;
          (&local_FF48)->y = (&local_FF48)->y + 0x8000;
          break;
        case 2:
          local_DC[0] = -0.45f;
          local_DC[2] = 0.5f;
          (&local_FF48)->y = (&local_FF48)->y + 0x8000;
          break;
        case 3:
          local_DC[0] = 0.45f;
          local_DC[2] = 0.5f;
          break;
      }
    } else if (local_FF44 == 28) {
      local_FF44 = arg4;
      switch (local_FF44) {
        case 0:
          local_DC[0] = -0.45f;
          local_DC[2] = -0.5f;
          (&local_FF48)->y = (&local_FF48)->y + 0x8000;
          break;
        case 1:
          local_DC[0] = -0.45f;
          local_DC[2] = 0.5f;
          break;
        case 2:
          local_DC[0] = 0.45f;
          local_DC[2] = 0.5f;
          break;
        case 3:
          local_DC[0] = 0.45f;
          local_DC[2] = -0.5f;
          (&local_FF48)->y = (&local_FF48)->y + 0x8000;
          break;
      }
    } else {
      local_FF44 = arg4;
      switch (local_FF44) {
        case 0:
          local_DC[0] = -0.45f;
          local_DC[2] = -0.5f;
          (&local_FF48)->y = (&local_FF48)->y + 0x8000;
          break;
        case 1:
          local_DC[0] = -0.45f;
          local_DC[2] = 0.5f;
          break;
        case 2:
          local_DC[0] = 0.45f;
          local_DC[2] = 0.5f;
          break;
        case 3:
          local_DC[0] = 0.45f;
          local_DC[2] = -0.5f;
          (&local_FF48)->y = (&local_FF48)->y + 0x8000;
          break;
      }
    }
  }
  local_FF60 = (&local_FF48)->y & 0xFFFF;
  local_C4[2] = local_DC[2] * Script_MathUtils(SCRIPT_MATH_COS_FIXED16, local_FF60) - *local_DC * Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, local_FF60);
  local_C4[0] = local_DC[2] * Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, local_FF60) + *local_DC * Script_MathUtils(SCRIPT_MATH_COS_FIXED16, local_FF60);
  local_D0.x = (&local_F4)->x + *local_C4;
  (&local_D0)->y = (&local_F4)->y;
  (&local_D0)->z = (&local_F4)->z + local_C4[2];
  Script_EVT_SetPosition(SHEN_NAME('DOOR'), POS_XYZ, &local_D0);
  Script_EVT_SetRotation(SHEN_NAME('DOOR'), 0x38000000, &local_FF48, 0);
  local_A4 = 0.0f;
  local_FF68 = 0;
  for (;;) {
    if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0) == 0) {
      Script_sprintf("This Motion don't Finish!!\012");
      break;
    }
    if (Script_IsCharaMotionFinished(arg0)) {
      if (local_FF68 == 1) {
        if (Script_EnqueueSCNT(2) == 0) {
          break;
        }
      }
    }
    if (local_FF68 == 1) {
      if (local_FF5C == 0) {
        int32_t unused_FF44;
        int32_t local_FF40;
        SCNF_WaitFrames_0000(45);
        local_FF40 = Script_ScenDataUtil(task_6C, 4, task_B4[arg2] * 13 + 10);
        if (local_FF40 != -1) {
          if (arg0 == task_1CC) {
            Script_executeAicaCommand(local_FF40, 0, 0);
          } else {
            Script_Execute3DSoundCommand_Wrapper(local_FF40, 4, &local_B8, 5.0f, 20.0f, 0);
          }
        }
        local_FF5C = 1;
      } else {
        if (local_FF58 <= 20) {
          local_A4 = local_A4 - 0.05f;
          local_FF7C = local_FF7C + (int32_t)(local_A4 * local_9C / 360.0f * (float)0x10000);
          if (local_AA) {
            int32_t unused_FF44;
            Vector3i local_FF38;
            local_FF38.x = 0;
            (&local_FF38)->z = 0;
            if ((arg4 == 2) | (arg4 == 3)) {
              (&local_FF38)->y = local_FF7C;
              Script_charaPosRotUtils(arg1, 12, 0, &local_FF38, 0);
            } else {
              (&local_FF38)->y = local_FF7C;
              Script_charaPosRotUtils(arg1, 7, 0, &local_FF38, 0);
            }
          } else {
            int32_t unused_FF44;
            Vector3i local_FF38;
            (&local_FF38)->y = local_FF7C;
            Script_EVT_SetRotation(arg1, MATRIX_XFORM_Y, &local_FF38, 0);
          }
          local_FF58 = local_FF58 + 1;
        }
      }
    }
    local_A0 = Script_GetCharaMotionRemainingFramesScaled(arg0);
    if (!(local_A0 < local_98) & !(local_A0 >= local_98 + 1.0f)) {
      int32_t unused_FF44;
      int32_t local_FF40;
      local_FF40 = Script_ScenDataUtil(task_6C, 4, task_B4[arg2] * 13 + 9);
      if (local_FF40 != -1) {
        if (arg0 == task_1CC) {
          Script_executeAicaCommand(local_FF40, 0, 0);
        } else {
          Script_Execute3DSoundCommand_Wrapper(local_FF40, 4, &local_B8, 5.0f, 20.0f, 0);
        }
      }
    }
    if (!(local_A0 < local_94) & !(local_A0 >= local_94 + 1.0f)) {
      int32_t unused_FF44;
      int32_t local_FF40;
      local_FF40 = Script_ScenDataUtil(task_6C, 4, task_B4[arg2] * 13 + 9);
      if (local_FF40 != -1) {
        if (arg0 == task_1CC) {
          Script_executeAicaCommand(local_FF40, 0, 0);
        } else {
          Script_Execute3DSoundCommand_Wrapper(local_FF40, 4, &local_B8, 5.0f, 20.0f, 0);
        }
      }
      if (local_FF68 == 0) {
        local_FF68 = 1;
        local_FF64 = Script_ScenDataUtil(task_6C, 4, task_B4[arg2] * 13 + 8);
        Script_ConversationUtils(1, local_FF74);
        Script_EnqueueSCNT(1, 0x524F4F44, local_FF64);
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  local_A4 = 0.0f;
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0) != 0) {
    local_FF6C = 0;
    L007C15:;
    if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0) == 0) {
    } else {
      local_A4 = local_A4 + 0.05f;
      local_FF7C = local_FF7C + (int32_t)(local_A4 * local_9C / 360.0f * (float)0x10000);
      if (local_AA) {
        int32_t unused_FF44;
        Vector3i local_FF38;
        local_FF38.x = 0;
        (&local_FF38)->z = 0;
        if ((arg4 == 2) | (arg4 == 3)) {
          (&local_FF38)->y = local_FF7C;
          Script_charaPosRotUtils(arg1, 12, 0, &local_FF38, 0);
        } else {
          (&local_FF38)->y = local_FF7C;
          Script_charaPosRotUtils(arg1, 7, 0, &local_FF38, 0);
        }
      } else {
        int32_t unused_FF44;
        Vector3i local_FF38;
        (&local_FF38)->y = local_FF7C;
        Script_EVT_SetRotation(arg1, MATRIX_XFORM_Y, &local_FF38, 0);
      }
      SCNF_WaitFrames_0000(1);
      local_FF6C = local_FF6C + 1;
      if (!(local_FF6C >= 20)) goto L007C15;
    }
    {
      int32_t unused_FF44;
      int32_t local_FF40;
      local_FF40 = Script_ScenDataUtil(task_6C, 4, task_B4[arg2] * 13 + 11);
      if (local_FF40 != -1) {
        if (arg0 == task_1CC) {
          Script_executeAicaCommand(local_FF40, 0, 0);
        } else {
          Script_Execute3DSoundCommand_Wrapper(local_FF40, 4, &local_B8, 5.0f, 20.0f, 0);
        }
      }
    }
  }
  if (local_AA) {
    int32_t unused_FF44;
    Vector3i local_FF38;
    local_FF38.x = 0;
    (&local_FF38)->z = 0;
    (&local_FF38)->y = local_84;
    Script_charaPosRotUtils(arg1, 12, 0, &local_FF38, 0);
    (&local_FF38)->y = local_80;
    Script_charaPosRotUtils(arg1, 7, 0, &local_FF38, 0);
  } else {
    {
      int32_t unused_FF44;
      Vector3i local_FF38;
      (&local_FF38)->y = local_84;
      Script_EVT_SetRotation(arg1, MATRIX_XFORM_Y, &local_FF38, 0);
    }
    {
      int32_t unused_FF44;
      Vector3i local_FF38;
      (&local_FF38)->y = local_80;
      Script_EVT_SetRotation(arg1, MATRIX_XFORM_Y, &local_FF38, 0);
    }
  }
  Script_CycleManagerUtils(CYCLE_CMD_LOAD_OR_RESET_RUNNING_PROGRAM, local_8C);
  return;
}

static void script_7DD1(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
  int32_t local_FC;
  int32_t unused_F8;
  float local_EC[3];
  Vector3 local_E0;
  float local_D4[3];
  Vector3 local_C8;
  int32_t unused_C4;
  int32_t local_BC[2];
  Vector3i local_B0;
  int32_t local_AC;
  Script_sprintf("open-move!!\012");
  local_BC[1] = arg4;
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0) != 0) {
    Script_EVT_GetPosition(arg1, -1, &local_E0, 0);
    Script_EVT_GetPosition(arg0, -1, &local_C8, 0);
    Script_mtwkGetSetRotation(arg0, &local_B0, 0);
    local_EC[0] = 0.0f;
    local_EC[1] = 0.0f;
    local_EC[2] = 0.0f;
    if (arg0 != task_1CC) {
      local_AC = arg2;
      if ((local_AC == 0) | (local_AC == 1) | (local_AC == 3) | (local_AC == 5) | (local_AC == 7) | (local_AC == 9) | (local_AC == 11) | (local_AC == 25) | (local_AC == 27) | (local_AC == 30) | (local_AC == 32) | (local_AC == 34) | (local_AC == 36) | (local_AC == 37)) {
        local_AC = arg3;
        switch (local_AC) {
          case 0:
            local_EC[0] = 0.45f;
            local_EC[2] = 1.0f;
            break;
          case 1:
            local_EC[0] = 0.45f;
            local_EC[2] = -1.0f;
            break;
          case 2:
            local_EC[0] = 0.45f;
            local_EC[2] = -1.0f;
            break;
          case 3:
            local_EC[0] = 0.45f;
            local_EC[2] = 1.0f;
            break;
        }
      } else if ((local_AC == 2) | (local_AC == 4) | (local_AC == 6) | (local_AC == 8) | (local_AC == 10) | (local_AC == 12) | (local_AC == 24) | (local_AC == 26) | (local_AC == 31) | (local_AC == 33) | (local_AC == 35)) {
        local_AC = arg3;
        switch (local_AC) {
          case 0:
            local_EC[0] = -0.45f;
            local_EC[2] = 1.0f;
            break;
          case 1:
            local_EC[0] = -0.45f;
            local_EC[2] = -1.0f;
            break;
          case 2:
            local_EC[0] = 0.45f;
            local_EC[2] = -1.0f;
            break;
          case 3:
            local_EC[0] = 0.45f;
            local_EC[2] = 1.0f;
            break;
        }
      } else if ((local_AC == 13) | (local_AC == 14) | (local_AC == 15) | (local_AC == 16) | (local_AC == 17) | (local_AC == 18) | (local_AC == 19) | (local_AC == 20) | (local_AC == 21) | (local_AC == 22) | (local_AC == 23) | (local_AC == 29) | (local_AC == 28)) {
        local_AC = arg3;
        switch (local_AC) {
          case 0:
            local_EC[0] = 0.45f;
            local_EC[2] = 1.0f;
            break;
          case 1:
            local_EC[0] = 0.45f;
            local_EC[2] = -1.0f;
            break;
          case 2:
            local_EC[0] = -0.45f;
            local_EC[2] = -1.0f;
            break;
          case 3:
            local_EC[0] = -0.45f;
            local_EC[2] = 1.0f;
            break;
        }
      } else {
        local_AC = arg3;
        switch (local_AC) {
          case 0:
            local_EC[0] = 0.45f;
            local_EC[2] = 1.0f;
            break;
          case 1:
            local_EC[0] = 0.45f;
            local_EC[2] = -1.0f;
            break;
          case 2:
            local_EC[0] = -0.45f;
            local_EC[2] = -1.0f;
            break;
          case 3:
            local_EC[0] = -0.45f;
            local_EC[2] = 1.0f;
            break;
        }
      }
      local_FC = local_BC[1] & 0xFFFF;
      local_D4[2] = local_EC[2] * Script_MathUtils(SCRIPT_MATH_COS_FIXED16, local_FC) - *local_EC * Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, local_FC);
      local_D4[0] = local_EC[2] * Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, local_FC) + *local_EC * Script_MathUtils(SCRIPT_MATH_COS_FIXED16, local_FC);
      local_E0.x = (&local_E0)->x + *local_D4;
      (&local_E0)->z = (&local_E0)->z + local_D4[2];
      if ((arg3 == 0) | (arg3 == 3)) {
        local_BC[1] = local_BC[1] + 0x8000;
      }
      Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
      Script_SetCharaMoveTarget(arg0, &local_E0, local_BC[1], script_08F7(arg5));
    }
  }
  return;
}

static void script_821B(int32_t arg0, DSWKDoorEntry *arg1, int32_t *arg2, DoorResourceDescriptor *arg3, MapObjectRecord *arg4, int32_t arg5, int32_t arg6) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t unused_F4;
  task_6C = arg1;
  task_70 = arg2;
  task_74 = arg3;
  task_78 = arg4;
  task_7C.mode = 0;
  task_7C.id = 0;
  task_7C.entry_idx = 0;
  task_1CC = arg0;
  (void)((task_1F0 = -1) & 1);
  task_1EC = 0;
  Script_motiLoad(Script_GetSceneID(), SHEN_NAME('FREE'), "M_MDOR.BIN");
  local_F8 = 0;
  L008285:;
  local_FC = Script_AddCharacterHandler(1, 0, local_F8 * 0x1000000 + 0x30726F64);
  Script_PreserveAccumulator_012C(local_FC, "This Charactor Work is NULL!!\012");
  DSWK_AllocDoorMapcRenderState(local_FC);
  Script_scenToggleVisibleFor(local_FC, 0);
  task_154[local_F8] = local_FC;
  task_8C[local_F8] = -1;
  task_B4[local_F8] = -1;
  task_DC[local_F8] = 0;
  task_104[local_F8] = 0;
  task_12C[local_F8] = 0;
  task_17C[local_F8] = 0;
  local_F8 = local_F8 + 1;
  if (!(local_F8 >= 10)) goto L008285;
  local_F8 = 0;
  for (;;) {
    if (Script_ScenDataUtil(task_6C, 4, local_F8 * 13) == -1) {
      break;
    }
    Script_ScenDataUtil(task_6C, 14, local_F8 * 13 + 1, 0);
    local_F8 = local_F8 + 1;
  }
  Script_EnqueueSCNT(0);
  DSWK_BindTablesAndInitState(arg1, arg2, arg3, arg4, &task_7C, task_8C, task_B4, (shen_name *)task_DC, task_104, arg0, arg5, data_F188, data_F778, (shen_name *)task_154);
  Script_scenCreateTask(script_8AD7, 0);
  return;
}

static void script_83A7(DSWKDoorEntry *arg0, int32_t *arg1, DoorResourceDescriptor *arg2, MapObjectRecord *arg3, int32_t arg4) {
  task_6C = arg0;
  task_70 = arg1;
  task_74 = arg2;
  task_78 = arg3;
  DSWK_RebindDoorSystemTables(arg0, arg1, arg2, arg3, arg4);
  task_1EC = 1;
  return;
}

static void script_83DB(void) {
  int32_t local_FC;
  local_FC = 0;
  L0083E3:;
  task_8C[local_FC] = -1;
  task_B4[local_FC] = -1;
  task_DC[local_FC] = 0;
  task_104[local_FC] = 0;
  task_12C[local_FC] = 0;
  task_17C[local_FC] = 0;
  local_FC = local_FC + 1;
  if (!(local_FC >= 10)) goto L0083E3;
  return;
}

static void script_8444(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  local_F4 = 0;
  for (;;) {
    Script_sprintf("手動ドア開け番号指定\012");
    if (task_1CC == arg0) {
      Script_EVT_GetPosition(arg0, -1, &task_200, 0);
    }
    local_F8 = DSWK_DoorUtils(DOOR_CMD_ALLOC_ENTRY_SLOT, arg0, arg1);
    if (local_F8 != -1) {
      for (;;) {
        local_FC = DSWK_DoorUtils(DOOR_CMD_TYPE13, local_F8);
        if (local_FC != 2) {
          break;
        }
        SCNF_WaitFrames_0000(1);
      }
      if (local_FC != -2) {
        break;
      }
      local_F4 = local_F4 + 1;
      if (local_F4 >= 120) {
        Script_sprintf("無限ループになりそうなので抜ける\012");
        break;
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  (void)local_FC;
  return;
}

static void script_84E1(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  Vector3 local_F0;
  if (task_1CC == arg0) {
    Script_EVT_GetPosition(arg0, -1, &task_200, 0);
  }
  Script_EVT_GetPosition(arg0, -1, &local_F0, 0);
  local_FC = DSWK_DoorUtils(DOOR_CMD_FIND_CLOSEST_ELEMENT, arg0, &local_F0, arg1);
  (void)local_FC;
  return;
}

static void script_851B(void) {
  int32_t local_FC;
  int32_t local_F8;
  local_FC = 0;
  for (;;) {
    if (Script_ScenDataUtil(task_6C, 4, local_FC * 13) == -1) {
      break;
    }
    local_F8 = Script_ScenDataUtil(task_6C, 4, local_FC * 13 + 1);
    local_F8 = local_F8 | 2;
    Script_ScenDataUtil(task_6C, 14, local_FC * 13 + 1, local_F8);
    local_FC = local_FC + 1;
  }
  return;
}

static void script_857E(void) {
  int32_t local_FC;
  int32_t local_F8;
  local_FC = 0;
  for (;;) {
    if (Script_ScenDataUtil(task_6C, 4, local_FC * 13) == -1) {
      break;
    }
    local_F8 = Script_ScenDataUtil(task_6C, 4, local_FC * 13 + 1);
    local_F8 = local_F8 & -3;
    Script_ScenDataUtil(task_6C, 14, local_FC * 13 + 1, local_F8);
    local_FC = local_FC + 1;
  }
  return;
}

static void script_85E1(int32_t arg0) {
  int32_t local_FC;
  local_FC = Script_ScenDataUtil(task_6C, 4, arg0 * 13 + 1);
  local_FC = local_FC | 2;
  Script_ScenDataUtil(task_6C, 14, arg0 * 13 + 1, local_FC);
  return;
}

static void script_8618(int32_t arg0) {
  int32_t local_FC;
  local_FC = Script_ScenDataUtil(task_6C, 4, arg0 * 13 + 1);
  local_FC = local_FC & -3;
  Script_ScenDataUtil(task_6C, 14, arg0 * 13 + 1, local_FC);
  return;
}

static void script_864F(int32_t arg0) {
  Vector3 local_F4;
  int32_t local_F0;
  int32_t local_EC;
  local_F0 = 0;
  for (;;) {
    if (task_B4[local_F0] == arg0) {
      local_EC = task_154[local_F0];
      if (Script_shenGetNode(local_EC, 7)) {
        if (Script_shenGetNode(local_EC, 5) & Script_shenGetNode(local_EC, 10)) {
          if (Script_ScenDataUtil(task_6C, 4, task_B4[local_F0] * 13 + 1) & 0x100) {
            local_F4.x = 0.0f;
            (&local_F4)->y = 0.0f;
            (&local_F4)->z = 0.0f;
            Script_charaPosRotUtils(local_EC, 10, &local_F4, 0, 0);
            Script_charaPosRotUtils(local_EC, 5, &local_F4, 0, 0);
          } else {
            local_F4.x = 0.0f;
            (&local_F4)->y = -1200.0f;
            (&local_F4)->z = 0.0f;
            Script_charaPosRotUtils(local_EC, 10, &local_F4, 0, 0);
            Script_charaPosRotUtils(local_EC, 5, &local_F4, 0, 0);
          }
        }
      } else {
        if (Script_shenGetNode(local_EC, 10)) {
          if (Script_ScenDataUtil(task_6C, 4, task_B4[local_F0] * 13 + 1) & 0x100) {
            local_F4.x = 0.0f;
            (&local_F4)->y = 0.0f;
            (&local_F4)->z = 0.0f;
            Script_charaPosRotUtils(local_EC, 10, &local_F4, 0, 0);
          } else {
            local_F4.x = 0.0f;
            (&local_F4)->y = -1200.0f;
            (&local_F4)->z = 0.0f;
            Script_charaPosRotUtils(local_EC, 10, &local_F4, 0, 0);
          }
        }
      }
      if (Script_shenGetNode(local_EC, 8) & Script_shenGetNode(local_EC, 13)) {
        if (Script_ScenDataUtil(task_6C, 4, task_B4[local_F0] * 13 + 1) & 0x20) {
          local_F4.x = 0.0f;
          (&local_F4)->y = 0.0f;
          (&local_F4)->z = 0.0f;
          Script_charaPosRotUtils(local_EC, 13, &local_F4, 0, 0);
          (&local_F4)->y = -1000.0f;
          Script_charaPosRotUtils(local_EC, 8, &local_F4, 0, 0);
        } else {
          local_F4.x = 0.0f;
          (&local_F4)->y = -1000.0f;
          (&local_F4)->z = 0.0f;
          Script_charaPosRotUtils(local_EC, 13, &local_F4, 0, 0);
          (&local_F4)->y = 0.0f;
          Script_charaPosRotUtils(local_EC, 8, &local_F4, 0, 0);
        }
      }
    }
    local_F0 = local_F0 + 1;
    if (local_F0 == 10) {
      break;
    }
  }
  return;
}

static void script_8891(int32_t arg0) {
  int32_t local_FC;
  local_FC = Script_ScenDataUtil(task_6C, 4, arg0 * 13 + 1);
  local_FC = local_FC | 0x20;
  Script_ScenDataUtil(task_6C, 14, arg0 * 13 + 1, local_FC);
  script_8999(arg0);
  script_864F(arg0);
  return;
}

static void script_88DA(int32_t arg0) {
  int32_t local_FC;
  local_FC = Script_ScenDataUtil(task_6C, 4, arg0 * 13 + 1);
  local_FC = local_FC & -33;
  Script_ScenDataUtil(task_6C, 14, arg0 * 13 + 1, local_FC);
  script_89A7(arg0);
  script_864F(arg0);
  return;
}

static void script_8923(int32_t arg0, int32_t arg1) {
  if (arg1) {
    int32_t local_FC;
    local_FC = Script_ScenDataUtil(task_6C, 4, arg0 * 13 + 1);
    local_FC = local_FC & -257;
    Script_ScenDataUtil(task_6C, 14, arg0 * 13 + 1, local_FC);
  } else {
    int32_t local_FC;
    local_FC = Script_ScenDataUtil(task_6C, 4, arg0 * 13 + 1);
    local_FC = local_FC | 0x100;
    Script_ScenDataUtil(task_6C, 14, arg0 * 13 + 1, local_FC);
  }
  return;
}

static void script_8999(int32_t arg0) {
  script_8923(arg0, 0);
  return;
}

static void script_89A7(int32_t arg0) {
  script_8923(arg0, 1);
  return;
}

static void script_89B5(int32_t arg0) {
  int32_t local_FC;
  local_FC = 0;
  for (;;) {
    if (Script_ScenDataUtil(task_6C, 4, local_FC * 13) == -1) {
      break;
    }
    if (arg0) {
      int32_t local_F8;
      local_F8 = Script_ScenDataUtil(task_6C, 4, local_FC * 13 + 1);
      local_F8 = local_F8 & -65;
      local_F8 = local_F8 & -129;
      local_F8 = local_F8 | 0x40;
      Script_ScenDataUtil(task_6C, 14, local_FC * 13 + 1, local_F8);
    } else {
      int32_t local_F8;
      local_F8 = Script_ScenDataUtil(task_6C, 4, local_FC * 13 + 1);
      local_F8 = local_F8 & -65;
      local_F8 = local_F8 & -129;
      local_F8 = local_F8 | 0x80;
      Script_ScenDataUtil(task_6C, 14, local_FC * 13 + 1, local_F8);
    }
    local_FC = local_FC + 1;
  }
  return;
}

static void script_8A7C(void) {
  script_89B5(1);
  return;
}

static void script_8A88(void) {
  script_89B5(0);
  return;
}

static void script_8A94(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5, int32_t arg6) {
  int32_t local_FC;
  local_FC = 0;
  for (;;) {
    if (Script_ScenDataUtil(task_6C, 4, local_FC * 13) == -1) {
      break;
    }
    DSWK_AssignDoorEntryConfig(local_FC, arg0, arg1, arg2, arg3, arg4, arg5, arg6);
    local_FC = local_FC + 1;
  }
  return;
}

static int32_t script_8AD7(void) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  SCNF_WaitFrames_0000(1);
  SCNF_WaitFrames_0000(1);
  SCNF_WaitFrames_0000(1);
  local_F4 = -1;
  local_F8 = -1;
  for (;;) {
    if (task_1EC == 0) {
      local_F4 = DSWK_DoorUtils(DOOR_CMD_FIND_LOWEST_FLOAT_INDEX, 0x3FD851EA);
      if (-(local_F4 != -1) & -(task_8C[local_F4] == 1) & Script_IsPlayerAllowed(0) & -((&task_7C)->mode == 0)) {
        local_FC = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_B4[local_F4], 2);
        local_F8 = Script_ScenDataUtil(task_74, 4, Script_ScenDataUtil(task_78, 4, local_FC * 9 + 1) * 4 + 1);
        script_98A8(1, local_FC, local_F4, local_F8);
        if (script_9A5C(local_FC, local_F4, local_F8)) {
          local_F4 = -1;
          local_F8 = -1;
        }
      } else {
        script_98A8(0, local_FC, local_F4, local_F8);
        local_F4 = -1;
        local_F8 = -1;
      }
      script_9C72(local_F4, local_F8);
    } else {
      task_1EC = 0;
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int8_t script_8BE8(int32_t arg0, int32_t arg1, int8_t arg2) {
  Vector3 local_F4;
  Vector3 local_E8;
  Vector3i local_DC;
  int32_t local_D8;
  arg2 = 0;
  Script_EVT_GetPosition(task_1CC, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  Script_mtwkGetSetRotation(task_1CC, &local_DC, 0x42000000);
  local_E8.x = (&local_F4)->x;
  (&local_E8)->y = (&local_F4)->y;
  (&local_E8)->z = (&local_F4)->z;
  if (DSWK_DoorUtils(DOOR_CMD_CALC_VECTOR_FROM_ANGLE, arg0, &local_E8, &local_D8)) {
    local_D8 = ((&local_DC)->y - local_D8) & 0xFFFF;
    if ((local_D8 <= 21845) | (local_D8 >= 43691)) {
      if (DSWK_DoorUtils(DOOR_CMD_TEST_LINE_INTERSECTION, &local_F4, &local_E8) == 0) {
        arg2 = 1;
      }
    }
  }
  return arg2;
}

__attribute__((scn_string_bindings(0, data_10DE9, 1, data_10E02)))
static void script_8C80(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  Vector3 local_EC;
  Vector3i local_E0;
  local_F8 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_B4[arg0], 1);
  if (local_F8 != -1) {
    local_FC = Script_AddCharacterHandler(1, 0, arg0 * 0x1000000 + 0x30786F62);
    if (local_FC == 0) {
      Script_PreserveAccumulator_012C(local_FC, "Don't make BOX Buffer!!\012");
      task_1A4[arg0] = -1;
    } else if (local_FC == arg0 * 0x1000000 + 0x30786F62) {
      task_1A4[arg0] = local_FC;
      *(int32_t *)&local_EC.x = Script_ScenDataUtil(task_78, 4, local_F8 * 9 + 5);
      *(int32_t *)&(&local_EC)->y = Script_ScenDataUtil(task_78, 4, local_F8 * 9 + 6);
      *(int32_t *)&(&local_EC)->z = Script_ScenDataUtil(task_78, 4, local_F8 * 9 + 7);
      Script_EVT_SetPosition(local_FC, POS_XYZ, &local_EC);
      local_E0.x = 0;
      (&local_E0)->y = Script_ScenDataUtil(task_78, 4, local_F8 * 9 + 8);
      (&local_E0)->z = 0;
      Script_EVT_SetRotation(local_FC, 0x38000000, &local_E0, 0);
      *(int32_t *)&local_EC.x = Script_ScenDataUtil(task_78, 4, local_F8 * 9 + 2);
      *(int32_t *)&(&local_EC)->y = Script_ScenDataUtil(task_78, 4, local_F8 * 9 + 3);
      *(int32_t *)&(&local_EC)->z = Script_ScenDataUtil(task_78, 4, local_F8 * 9 + 4);
      Script_hmdlApplyScale(local_FC, &local_EC);
      Script_CreateNPCImageModule(local_FC, (const char *)Script_ScenDataUtil(task_74, 8, Script_ScenDataUtil(task_78, 4, local_F8 * 9 + 1) * 4), 0);
      Script_charaDisplay(local_FC, CDM_Show);
      Script_charaVisUtils(local_FC, 1);
    } else {
      Script_PreserveAccumulator_012C(local_FC, "Don't make BOX Buffer!!\012");
      task_1A4[arg0] = -1;
    }
  } else {
    task_1A4[arg0] = -1;
  }
  return;
}

static void script_8E4F(int32_t arg0) {
  if (task_1A4[arg0] != -1) {
    Script_SetMapControlTaskFlag(task_1A4[arg0], 1);
    task_1A4[arg0] = -1;
  }
  return;
}

static int32_t script_8E80(int32_t arg0, int32_t arg1, int32_t arg2, float arg3, float arg4, int32_t arg5, int32_t arg6) {
  Vector3 local_F4;
  Vector3 local_E8;
  float local_DC[3];
  Vector3i local_D0;
  Vector3i local_C4;
  int32_t local_C0;
  int32_t local_BC;
  if (task_1CC == arg0) {
    local_F4.x = (&task_200)->x;
    (&local_F4)->y = task_200.y;
    (&local_F4)->z = task_200.z;
  } else {
    Script_EVT_GetPosition(arg0, -1, &local_F4, 0);
  }
  Script_EVT_GetPosition(arg1, -1, &local_E8, 0);
  Script_mtwkGetSetRotation(arg1, &local_D0, 0);
  local_C4.x = (&local_D0)->x;
  (&local_C4)->y = (&local_D0)->y;
  (&local_C4)->z = (&local_D0)->z;
  local_C0 = Script_FloatUtils(ScriptMathOp_Atan2ReverseVector, (&local_F4)->x - (&local_E8)->x, (&local_F4)->z - (&local_E8)->z);
  local_C0 = ((local_C0 - (&local_D0)->y) >> 14) & 3;
  if (arg2 != 99) {
    if (arg2 == 6) {
      if ((local_C0 == 0) | (local_C0 == 1)) {
        if ((Script_ScenDataUtil(task_6C, 4, arg6 * 13 + 1) & 8) | (Script_ScenDataUtil(task_6C, 4, arg6 * 13 + 1) & 0x40)) {
          local_C0 = local_C0 ^ 3;
        }
      } else {
        if ((Script_ScenDataUtil(task_6C, 4, arg6 * 13 + 1) & 0x10) | (Script_ScenDataUtil(task_6C, 4, arg6 * 13 + 1) & 0x80)) {
          local_C0 = local_C0 ^ 3;
        }
      }
      arg2 = 4;
    }
    {
      int32_t unused_B8;
      int32_t local_B4;
      local_B4 = arg5;
      if ((local_B4 == 0) | (local_B4 == 1) | (local_B4 == 3) | (local_B4 == 5) | (local_B4 == 7) | (local_B4 == 9) | (local_B4 == 11) | (local_B4 == 25) | (local_B4 == 27) | (local_B4 == 30) | (local_B4 == 32) | (local_B4 == 34) | (local_B4 == 36) | (local_B4 == 37)) {
        local_B4 = local_C0;
        if ((local_B4 == 0) | (local_B4 == 3)) {
          arg4 = (float)0 - arg4;
          (&local_C4)->y = (&local_C4)->y + 0x8000;
        } else {
          if ((local_B4 == 1) | (local_B4 == 2)) goto L00906B;
        }
        L00906B:;
      } else if ((local_B4 == 2) | (local_B4 == 4) | (local_B4 == 6) | (local_B4 == 8) | (local_B4 == 10) | (local_B4 == 12) | (local_B4 == 24) | (local_B4 == 26) | (local_B4 == 31) | (local_B4 == 33) | (local_B4 == 35)) {
        local_B4 = local_C0;
        if ((local_B4 == 2) | (local_B4 == 1)) {
          arg3 = (float)0 - arg3;
        } else {
          if ((local_B4 == 3) | (local_B4 == 0)) {
            arg3 = (float)0 - arg3;
            arg4 = (float)0 - arg4;
            (&local_C4)->y = (&local_C4)->y + 0x8000;
            goto L00910E;
          }
        }
        L00910E:;
      } else if ((local_B4 == 13) | (local_B4 == 14) | (local_B4 == 15) | (local_B4 == 16) | (local_B4 == 17) | (local_B4 == 18) | (local_B4 == 19) | (local_B4 == 20) | (local_B4 == 21) | (local_B4 == 22) | (local_B4 == 23) | (local_B4 == 29)) {
        local_B4 = local_C0;
        switch (local_B4) {
          case 0:
            arg3 = (float)0 - arg3;
            arg4 = (float)0 - arg4;
            (&local_C4)->y = (&local_C4)->y + 0x8000;
            break;
          case 1:
            arg3 = (float)0 - arg3;
            break;
          case 2:
            break;
          case 3:
            arg4 = (float)0 - arg4;
            (&local_C4)->y = (&local_C4)->y + 0x8000;
            break;
        }
      } else if (local_B4 == 28) {
        local_B4 = local_C0;
        switch (local_B4) {
          case 0:
            arg3 = (float)0 - arg3;
            arg4 = (float)0 - arg4;
            (&local_C4)->y = (&local_C4)->y + 0x8000;
            break;
          case 1:
            arg3 = (float)0 - arg3;
            break;
          case 2:
            break;
          case 3:
            arg4 = (float)0 - arg4;
            (&local_C4)->y = (&local_C4)->y + 0x8000;
            break;
        }
      } else {
        local_B4 = local_C0;
        if ((local_B4 == 2) | (local_B4 == 1)) {
        } else {
          if ((local_B4 == 3) | (local_B4 == 0)) {
            arg4 = (float)0 - arg4;
            (&local_C4)->y = (&local_C4)->y + 0x8000;
            goto L0092BA;
          }
        }
      }
    }
    L0092BA:;
    local_BC = (&local_D0)->y & 0xFFFF;
    local_DC[2] = arg4 * Script_MathUtils(SCRIPT_MATH_COS_FIXED16, local_BC) - arg3 * Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, local_BC);
    local_DC[0] = arg4 * Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, local_BC) + arg3 * Script_MathUtils(SCRIPT_MATH_COS_FIXED16, local_BC);
    if (arg2 == -1) {
      int32_t unused_B4[2];
      Vector3 local_A8;
      int32_t local_A4;
      local_A8.x = (&local_E8)->x + *local_DC;
      (&local_A8)->y = (&local_E8)->y;
      (&local_A8)->z = (&local_E8)->z + local_DC[2];
      local_A4 = DSWK_DoorUtils(DOOR_CMD_TEST_LINE_INTERSECTION, &local_F4, &local_A8);
      if ((local_C0 == 1) | (local_C0 == 2)) {
        local_C0 = 1;
      } else {
        local_C0 = 0;
      }
      if (local_A4) {
        local_C0 = -1;
      }
    } else {
      int32_t unused_B8;
      int32_t local_B4;
      local_F4.x = (&local_E8)->x + *local_DC;
      (&local_F4)->y = (&local_E8)->y;
      (&local_F4)->z = (&local_E8)->z + local_DC[2];
      local_B4 = arg2;
      switch (local_B4) {
        case 0:
          Script_EVT_SetPosition(arg0, POS_XYZ, &local_F4);
          Script_EVT_SetRotation(arg0, 0x38000000, &local_C4, 0);
          break;
        case 2:
          Script_EVT_SetPosition(arg0, POS_XYZ, &local_F4);
          Script_EVT_SetRotation(arg0, 0x38000000, &local_C4, 0);
          if ((local_C0 == 1) | (local_C0 == 2)) {
            local_C0 = 1;
          } else {
            local_C0 = 0;
          }
          break;
        case 3:
          if ((local_C0 == 1) | (local_C0 == 2)) {
            local_C0 = 1;
          } else {
            local_C0 = 0;
          }
          break;
        case 4:
          task_1D4.x = (&local_F4)->x;
          task_1D4.y = (&local_F4)->y;
          task_1D4.z = (&local_F4)->z;
          task_1E0 = (&local_C4)->x;
          *(&task_1E0 + 1) = (&local_C4)->y;
          *(&task_1E0 + 2) = (&local_C4)->z;
          break;
        case 5:
          task_1D4.x = (&local_F4)->x;
          task_1D4.y = (&local_F4)->y;
          task_1D4.z = (&local_F4)->z;
          task_1E0 = (&local_C4)->x;
          *(&task_1E0 + 1) = (&local_C4)->y;
          *(&task_1E0 + 2) = (&local_C4)->z;
          if ((local_C0 == 1) | (local_C0 == 2)) {
            local_C0 = 1;
          } else {
            local_C0 = 0;
          }
          break;
        case 7:
          Script_EVT_GetPosition(arg0, -1, &task_1D4, 0);
          task_1E0 = (&local_C4)->x;
          *(&task_1E0 + 1) = (&local_C4)->y;
          *(&task_1E0 + 2) = (&local_C4)->z;
          break;
      }
    }
  }
  return local_C0;
}

static void script_9549(int32_t arg0, int32_t arg1) {
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
    if (task_1CC == arg0) {
      if (arg1) {
        Script_ccowUtils(arg0, 3);
      } else {
        Script_ccowUtils(arg0, 4);
      }
    }
  }
  return;
}

static void script_957B(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  int32_t local_F8;
  local_F8 = script_8E80(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
  if (task_1CC != arg0) {
    if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
      Script_charaToggleMotEnabled(arg0, 1);
    }
  } else if ((local_F8 == 0) | (local_F8 == 1)) {
    if ((Script_ScenDataUtil(task_6C, 4, arg2 * 13 + 1) & 8) == 0) {
      if ((Script_ScenDataUtil(task_6C, 4, arg2 * 13 + 1) & 0x40) != 0) {
        local_FC = Script_ScenDataUtil(task_6C, 4, arg2 * 13 + 2) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_FC, 0);
      } else {
        local_FC = (Script_ScenDataUtil(task_6C, 4, arg2 * 13 + 2) >> 16) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_FC, 0);
      }
    }
  } else {
    if ((Script_ScenDataUtil(task_6C, 4, arg2 * 13 + 1) & 0x10) == 0) {
      if ((Script_ScenDataUtil(task_6C, 4, arg2 * 13 + 1) & 0x80) != 0) {
        local_FC = (Script_ScenDataUtil(task_6C, 4, arg2 * 13 + 2) >> 16) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_FC, 0);
      } else {
        local_FC = Script_ScenDataUtil(task_6C, 4, arg2 * 13 + 2) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_FC, 0);
      }
    }
  }
  return;
}

__attribute__((scn_string_bindings(0, data_10E1B)))
static void script_96EB(int32_t arg0, int32_t arg1, int32_t arg2) {
  int32_t local_FC;
  if (task_1CC == arg0) {
    task_7C.mode = 0;
    task_7C.id = 0;
    if ((arg2 == 0) | (arg2 == 2)) {
      Script_PlayCharaMotion(task_1CC, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
      if (task_1F0 & 0x80) {
        Script_sprintf("Door System : ");
        Script_ryoControl(RYO_RestoreControl);
      }
      Script_shenClearFreeState(1);
      if (arg2 != 2) {
        int32_t local_F8;
        local_F8 = task_B4[arg1];
        local_FC = Script_ScenDataUtil(task_6C, 4, local_F8 * 13 + 2) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_FC, 1);
        local_FC = (Script_ScenDataUtil(task_6C, 4, local_F8 * 13 + 2) >> 16) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_FC, 1);
      }
    }
  } else {
    if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
      Script_charaToggleMotEnabled(arg0, 0);
    }
    if ((arg2 == 0) | (arg2 == 2)) {
      int32_t local_F8;
      local_F8 = task_B4[arg1];
      local_FC = Script_ScenDataUtil(task_6C, 4, local_F8 * 13 + 2) & 0xFFFF;
      Script_scenToggleCharaFlag(0, local_FC, 1);
      local_FC = (Script_ScenDataUtil(task_6C, 4, local_F8 * 13 + 2) >> 16) & 0xFFFF;
      Script_scenToggleCharaFlag(0, local_FC, 1);
    }
  }
  task_8C[arg1] = 1;
  task_17C[arg1] = 0;
  task_12C[arg1] = 0;
  task_DC[arg1] = 0;
  if (task_1A4[arg1] != -1) {
    Script_SetMapControlTaskFlag(task_1A4[arg1], 1);
    task_1A4[arg1] = -1;
  }
  return;
}

static void script_98A8(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  Vector3 local_E8;
  local_FC = 0;
  if (arg0 == 1) {
    local_F4 = arg3;
    if ((local_F4 == 17) | (local_F4 == 11) | (local_F4 == 12)) {
      local_FC = 0;
    } else if ((Script_ScenDataUtil(task_6C, 4, task_B4[arg2] * 13 + 1) & 2) == 0) {
      if ((Script_ScenDataUtil(task_6C, 4, task_B4[arg2] * 13 + 1) & 4) == 0) {
        local_FC = script_8BE8(arg2, arg3, local_FC);
      } else {
        local_F4 = arg3;
        if ((local_F4 == 21) | (local_F4 == 22) | (local_F4 == 23)) {
          local_FC = script_8BE8(arg2, arg3, local_FC);
          goto L00996C;
        }
      }
      L00996C:;
    } else {
      if ((Script_ScenDataUtil(task_6C, 4, task_B4[arg2] * 13 + 1) & 4) == 0) {
        local_FC = script_8BE8(arg2, arg3, local_FC);
        local_F8 = Script_ScenDataUtil(task_6C, 4, task_B4[arg2] * 13 + 6);
        if (local_F8 == 0) {
          local_FC = 0;
        } else {
          if (local_F8 >= 2) {
            Script_EVT_GetPosition(task_1CC, -1, &local_E8, 0);
            if (Script_CycleManagerUtils(CYCLE_CMD_COUNT_CYCLES_IN_RADIUS, &local_E8, 1.5f)) {
              local_FC = 0;
            }
            goto L009A07;
          }
        }
      }
    }
  }
  L009A07:;
  if (task_17C[arg2]) {
    local_FC = 0;
  }
  Script_scenUtils(SCENARIO_CMD_SET_HELP_PROMPT_OVERRIDE, local_FC);
  if ((local_FC ^ task_1FC) & local_FC) {
    Script_sicnUtils(0, 4, 1);
  }
  if ((local_FC ^ task_1FC) & task_1FC) {
    Script_sicnUtils(2, 4, 0);
  }
  task_1FC = local_FC;
  return;
}

__attribute__((scn_string_bindings(0, data_10E2A)))
static int32_t script_9A5C(int32_t arg0, int32_t arg1, int32_t arg2) {
  int32_t local_FC;
  int32_t local_F8;
  Vector3 local_EC;
  local_FC = 1;
  local_F8 = arg2;
  if ((local_F8 == 17) | (local_F8 == 11) | (local_F8 == 12)) {
    if (((Script_ScenDataUtil(task_6C, 4, task_B4[arg1] * 13 + 1) & 2) == 0) & ((Script_ScenDataUtil(task_6C, 4, task_B4[arg1] * 13 + 1) & 4) == 0)) {
      if (task_17C[arg1] == 0) {
        task_8C[arg1] = 4;
        local_FC = 0;
      }
    }
  } else {
    if (Script_GetControllerInput(1) == 0x400) {
      if (-(task_DC[arg1] != task_1CC) & task_1FC) {
        if ((Script_ScenDataUtil(task_6C, 4, task_B4[arg1] * 13 + 1) & 2) == 0) {
          task_8C[arg1] = 3;
          task_DC[arg1] = task_1CC;
          task_104[arg1] = 0;
          task_7C.mode = 1;
          task_7C.entry_idx = task_B4[arg1];
          local_FC = 0;
        } else {
          if (Script_ScenDataUtil(task_6C, 4, task_B4[arg1] * 13 + 6) >= 2) {
            Script_EVT_GetPosition(task_1CC, -1, &local_EC, 0);
            if (Script_CycleManagerUtils(CYCLE_CMD_COUNT_CYCLES_IN_RADIUS, &local_EC, 1.5f) == 0) {
              task_8C[arg1] = 6;
              task_DC[arg1] = task_1CC;
              task_104[arg1] = 0;
              task_7C.mode = 0;
              task_7C.entry_idx = task_B4[arg1];
              local_FC = 0;
            }
            goto L009C10;
          }
        }
        L009C10:;
        if (local_FC == 0) {
          Script_EVT_GetPosition(task_1CC, -1, &task_200, 0);
          Script_ryoControl(PCCTL_RequestNewMode, 4);
          Script_sprintf("Door System : ");
          Script_shenClearFreeState(0);
          Script_ryoControl(PCCTL_DisablePlayerControl);
          task_7C.id = task_1CC;
          while (Script_ryoControl(PCCTL_QueryRequestIdle) == 0) {
            SCNF_WaitFrames_0000(1);
          }
          Script_ryoControl(PCCTL_RequestNewMode, 0);
        }
      }
    }
  }
  end:;
}

static void script_9C72(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  int32_t local_F8;
  local_FC = 0;
  L009C7A:;
  local_F8 = task_8C[local_FC];
  switch (local_F8) {
    case 6:
      if (!(arg0 != -1)) goto L009C9A;
      L009C9A:;
      script_9D3C(local_FC, arg1);
      break;
    case 5:
      if (!(arg0 != -1)) goto L009CBA;
      L009CBA:;
      script_9D9D(local_FC);
      break;
    case 4:
      if (!(arg0 != -1)) goto L009CD8;
      L009CD8:;
      script_9F42(local_FC, arg0, arg1);
      break;
    case 3:
      if (!(arg0 != -1)) goto L009CFA;
      L009CFA:;
      script_9FDA(local_FC, arg1);
      break;
    case 2:
      script_A0BC(local_FC);
      break;
    case 1:
      break;
  }
  local_FC = local_FC + 1;
  if (!(local_FC >= 10)) goto L009C7A;
  return;
}

static void script_9D3C(int32_t arg0, int32_t arg1) {
  if (task_17C[arg0] == 0) {
    task_17C[arg0] = Script_scenCreateTask(script_1ED6, 4, task_DC[arg0], task_154[arg0], arg0, arg1);
    task_12C[arg0] = task_DC[arg0];
    script_8C80(arg0);
  }
  return;
}

__attribute__((scn_string_bindings(0, data_10E39, 1, data_10E4B)))
static void script_9D9D(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, task_DC[arg0])) {
    if (task_17C[arg0] == 0) {
      if (task_DC[arg0] == task_1CC) {
        task_7C.mode = 1;
        task_7C.entry_idx = task_B4[arg0];
        goto L009DF2;
      }
      L009DF2:;
      local_FC = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_B4[arg0], 2);
      local_F8 = Script_ScenDataUtil(task_74, 4, Script_ScenDataUtil(task_78, 4, local_FC * 9 + 1) * 4 + 1);
      script_957B(task_DC[arg0], task_154[arg0], task_B4[arg0], local_F8);
      task_17C[arg0] = Script_scenCreateTask(Script_ScenDataUtil(&data_10238, 8, local_F8), 4, task_DC[arg0], task_154[arg0], arg0, local_F8);
      task_12C[arg0] = task_DC[arg0];
      if ((Script_ScenDataUtil(task_6C, 4, task_B4[arg0] * 13 + 1) & 0x20000) == 0) {
        Script_sprintf("Door Box is ON!!\012");
        script_8C80(arg0);
      } else {
        Script_sprintf("Door Box is OFF!!\012");
        task_1A4[arg0] = -1;
      }
    }
  } else {
    if (task_8C[arg0] != 5) {
      task_8C[arg0] = 1;
      task_17C[arg0] = 0;
      task_12C[arg0] = 0;
      task_DC[arg0] = 0;
    }
  }
  return;
}

static void script_9F42(int32_t arg0, int32_t arg1, int32_t arg2) {
  if ((task_17C[arg0] == 0) & (arg2 >= 0)) {
    task_DC[arg0] = task_1CC;
    task_7C.mode = 2;
    task_7C.entry_idx = task_B4[arg0];
    task_17C[arg0] = Script_scenCreateTask(Script_ScenDataUtil(&data_10238, 8, arg2), 4, task_DC[arg0], task_154[arg0], arg0, arg2);
    task_12C[arg0] = task_DC[arg0];
    script_8C80(arg0);
  }
  return;
}

__attribute__((scn_string_bindings(0, data_10E5E, 1, data_10E70)))
static void script_9FDA(int32_t arg0, int32_t arg1) {
  if ((task_17C[arg0] == 0) & (arg1 >= 0)) {
    script_957B(task_DC[arg0], task_154[arg0], task_B4[arg0], arg1);
    task_17C[arg0] = Script_scenCreateTask(Script_ScenDataUtil(&data_10238, 8, arg1), 4, task_DC[arg0], task_154[arg0], arg0, arg1);
    task_12C[arg0] = task_DC[arg0];
    if ((Script_ScenDataUtil(task_6C, 4, task_B4[arg0] * 13 + 1) & 0x20000) == 0) {
      Script_sprintf("Door Box is ON!!\012");
      script_8C80(arg0);
    } else {
      Script_sprintf("Door Box is OFF!!\012");
      task_1A4[arg0] = -1;
    }
  }
  return;
}

static void script_A0BC(int32_t arg0) {
  int32_t local_FC;
  task_17C[arg0] = 0;
  task_12C[arg0] = 0;
  task_8C[arg0] = 1;
  local_FC = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_B4[arg0], 2);
  if (local_FC != -1) {
    task_DC[arg0] = 0;
  }
  return;
}

static void script_A113(void) {
  int32_t local_FC;
  local_FC = Script_GetSceneID();
  switch (local_FC) {
    case 1:
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_ENTRY2_TIMED_EVENT_DONE) == 0) {
        Script_TimedTransitionUtils(TIMED_TRANSITION_REGISTER, 1, 0x3044484A, 2, &data_10FA4, 0xD5A6C3, TimedTransitionUpdateMemoItem150);
      }
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JD00_ENTRY20_TIMED_EVENT_DONE) == 0) {
        Script_TimedTransitionUtils(TIMED_TRANSITION_REGISTER, 1, 0x3030444A, 20, &data_10F6C, 0xDA3AA3, 0);
      }
      break;
    case 2:
      if ((Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_GMCT_ROUTE_STARTED) == 0) | (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_GMCT_ROUTE_ADVANCED) == 0)) {
        Script_TimedTransitionUtils(TIMED_TRANSITION_REGISTER, 2, 0x54434D47, 1, &data_10F34, 0xB7420F, 0);
      }
      break;
    case 3:
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOZOMI_RESCUE_RUSH_ACTIVE) == 0) {
        Script_TimedTransitionUtils(TIMED_TRANSITION_REGISTER, 3, 0x4F4D4F4A, 20, &data_10F50, 0x2DC9FE, TimedTransitionRollbackToPreviousDay2330);
      }
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_FIRED_FROM_JOB) & ~Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 970)) {
        if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_MFSY_ENTRY42_TIMED_EVENT_DONE) == 0) {
          Script_TimedTransitionUtils(TIMED_TRANSITION_REGISTER, 3, 0x5953464D, 42, &data_10F88, 0xBBB817, 0);
        }
      }
      break;
  }
  return;
}

static void script_A24B(int32_t arg0) {
  script_A263(arg0);
  return;
}

static void script_A257(int32_t arg0) {
  script_A263(arg0);
  return;
}

static void script_A263(int32_t arg0) {
  if (Script_StepUtils(GET_STEP_HIGHER, 0) == 22) {
    int32_t local_FC;
    local_FC = Script_StepUtils(GET_STEP_LOWER, 0);
    if ((local_FC == 2) | (local_FC == 3)) {
      Script_SetShouldSleepFlag(0);
      goto L00A29C;
    }
  }
  L00A29C:;
  Script_ryoSetChara(arg0);
  Script_facemotUtils(arg0, 1, 0);
  Script_facemotUtils(arg0, 0, 0);
  Script_FaceEx(arg0, 0, 0, 2);
  Script_CharHandlers(arg0, -1, 0, 0);
  script_A113();
  Script_cpctStateUtils(1, 2);
  Script_SetCharaFlags(arg0, 4);
  Script_charaDisplay(arg0, CDM_Show);
  {
    int32_t local_FC;
    local_FC = Script_GetSceneID();
    if (local_FC == 2) {
      Script_LoadVoiceFile(1, "FREE02");
    } else if (local_FC == 3) {
      Script_LoadVoiceFile(1, "FREE03");
    } else if (local_FC == 4) {
      Script_LoadVoiceFile(1, "FREE04");
    } else {
      Script_LoadVoiceFile(1, "FREE01");
    }
  }
  Script_CPCTRelated(arg0, 32, 89);
  Script_clmdSetFlags(arg0, 8, 1);
  return;
}

static void script_A35F(int32_t arg0) {
  script_A257(arg0);
  for (;;) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_A376(int32_t arg0) {
  Vector3i local_F4;
  Script_mtwkGetSetRotation(CHARA_AKIR, &local_F4, 0);
  (&local_F4)->y = ((&local_F4)->y - arg0 + 10922) & 0xFFFF;
  if ((&local_F4)->y < 21845) {
    (void)-1;
  } else {
    (void)0;
  }
  return;
}

static void script_A3B7(int32_t arg0) {
  Vector3i local_F4;
  Script_mtwkGetSetRotation(CHARA_AKIR, &local_F4, 0x42000000);
  (&local_F4)->y = ((&local_F4)->y - arg0 + 10922) & 0xFFFF;
  if ((&local_F4)->y < 21845) {
    (void)-1;
  } else {
    (void)0;
  }
  return;
}

static void script_A3FB(int32_t arg0) {
  int32_t local_FC;
  local_FC = arg0;
  if ((local_FC == 0) | (local_FC == 5)) {
    Script_executeAicaCommand(5288, 0, 0);
  } else if (local_FC == 10) {
    Script_executeAicaCommand(70824, 0, 0);
  } else if (local_FC == 20) {
    Script_executeAicaCommand(136360, 0, 0);
  } else if (local_FC == 30) {
    Script_executeAicaCommand(201896, 0, 0);
  } else {
    Script_executeAicaCommand(267432, 0, 0);
  }
  return;
}

static void script_A479(void) {
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_PROGRESS_STEP_28_REACHED)) {
    script_A3FB(40);
  } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_PROGRESS_STEP_1E_REACHED)) {
    script_A3FB(30);
  } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_PROGRESS_STEP_14_REACHED)) {
    script_A3FB(20);
  } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JU00_CATA1_NBO_BRANCH_CHOSEN)) {
    script_A3FB(10);
  } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_PROGRESS_STEP05_SA1116_DOJO_DONE)) {
    script_A3FB(5);
  } else {
    script_A3FB(0);
  }
  return;
}

static void script_A4F8(int32_t arg0) {
  int8_t local_FF;
  int32_t local_F8;
  local_FF = 0;
  local_F8 = Script_shenGetStats(STATS_CASH);
  if (local_F8 - arg0 < 69000) {
    local_FF = 1;
  }
  (void)local_FF;
  return;
}

static int32_t script_A524(int32_t arg0, int32_t arg1) {
  int8_t local_FF;
  int32_t local_F8;
  (void)((local_FF = 0) & 4);
  local_F8 = Script_shenGetStats(STATS_CASH);
  if (local_F8 - arg1 >= arg0) {
    (void)((local_FF = -1) & 4);
  }
  return local_FF & 0x80;
}

static int8_t script_A557(int32_t arg0, int32_t arg1, int32_t arg2) {
  int8_t local_FF;
  int32_t local_F8;
  int32_t local_F4;
  int8_t local_F3;
  int32_t local_EC;
  int32_t local_E8;
  local_F8 = 0;
  local_F4 = 0;
  local_FF = 0;
  local_EC = Script_StepUtils(GET_STEP_HIGHER, 0) * 100 + Script_StepUtils(GET_STEP_LOWER, 0);
  local_E8 = arg0;
  if ((local_E8 == 0) | (local_E8 == 1)) {
    if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_SPEND_BUDGET_BONUS_ACTIVE)) {
      local_F4 = 320;
    }
  } else {
    if (local_E8 == 2) {
      local_F4 = 160;
      goto L00A5C5;
    }
  }
  L00A5C5:;
  local_F8 = local_F8 + local_F4;
  if ((Script_StepUtils(GET_STEP_HIGHER, 0) == 17) & (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_D000_VENDING_LOCK_CLEARED) == 0)) {
    local_F8 = local_F8 + 69000;
    (void)((local_F3 = 0) & 1);
  } else if ((local_EC >= 2202) & (local_EC <= 2204)) {
    (void)((local_F3 = 0) & 1);
    local_F8 = local_F4 = 0;
    local_FF = 4;
  } else {
    (void)((local_F3 = -(script_A524(local_F8, arg1) != 0)) & 0x10);
  }
  if (~local_F3) {
    if (local_F4 & -(local_F8 == local_F4)) {
      local_FF = 2;
    } else if (local_F8 > local_F4) {
      local_FF = 3;
    } else {
      if (local_FF == 0) {
        local_FF = 1;
      }
    }
    if (arg2) {
      script_A699(local_FF, arg0);
    }
  }
  return local_FF;
}

__attribute__((scn_string_bindings(0, data_10FDC, 8, data_11020, 9, data_11029, 10, data_11032, 11, data_1103B, 12, data_11044, 13, data_1104D)))
static void script_A699(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  if (arg0 == 1) {
  } else {
    Script_LoadVoiceFile(1, "SY11");
    local_F4 = arg0;
    switch (local_F4) {
      case 2:
        if (arg1 == 2) {
          local_F8 = Script_FloatUtils(ScriptMathOp_RandomIntBelow, 3);
          local_F4 = local_F8;
          switch (local_F4) {
            case 0:
              local_FC = Script_ShowUIMessageFromID("SY11A006");
              break;
            case 1:
              local_FC = Script_ShowUIMessageFromID("SY11A007");
              break;
            case 2:
              local_FC = Script_ShowUIMessageFromID("SY11A008");
              break;
          }
        } else {
          local_F8 = Script_FloatUtils(ScriptMathOp_RandomIntBelow, 4);
          local_F4 = local_F8;
          switch (local_F4) {
            case 0:
              local_FC = Script_ShowUIMessageFromID("SY11A002");
              break;
            case 1:
              local_FC = Script_ShowUIMessageFromID("SY11A003");
              break;
            case 2:
              local_FC = Script_ShowUIMessageFromID("SY11A004");
              break;
            case 3:
              local_FC = Script_ShowUIMessageFromID("SY11A005");
              break;
          }
        }
        break;
      case 3:
        local_FC = Script_ShowUIMessageFromID("SY11A001");
        break;
      case 4:
        local_F4 = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f);
        if (local_F4 == 1) {
          local_FC = Script_ShowUIMessageFromID("SY11A009");
        } else if (local_F4 == 2) {
          local_FC = Script_ShowUIMessageFromID("SY11A010");
        } else {
          local_FC = Script_ShowUIMessageFromID("SY11A011");
        }
        break;
      case 5:
        local_FC = Script_ShowUIMessageFromID("SY11A009");
        break;
    }
    while (Script_convGetIsActive((const void *)local_FC)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_LoadVoiceFile(0, "SY11");
  }
  return;
}

__attribute__((scn_string_bindings(0, data_11052, 1, data_11057, 2, data_11060, 3, data_11065, 4, data_1106A, 5, data_11073, 6, data_1107C, 7, data_11085)))
static void script_A862(int32_t arg0, int32_t arg1) {
  int8_t local_FF;
  int8_t local_FE;
  int32_t local_F8;
  int32_t local_F4;
  int32_t local_F0;
  local_FE = 1;
  local_FF = 0;
  local_F4 = Script_StepUtils(GET_STEP_HIGHER, 0) * 100 + Script_StepUtils(GET_STEP_LOWER, 0);
  if ((Script_StepUtils(GET_STEP_HIGHER, 0) == 17) & (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_D000_VENDING_LOCK_CLEARED) == 0)) {
    local_FE = 0;
    if (arg1) {
      if (Script_ryoControl(PCCTL_QueryShouldRun)) {
        Script_ryoControl(PCCTL_DisablePlayerControl);
        Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        local_FF = 1;
      }
      Script_LoadVoiceFile(1, "SY11");
      local_F8 = Script_ShowUIMessageFromID("SY11A001");
      while (Script_convGetIsActive((const void *)local_F8)) {
        SCNF_WaitFrames_0000(1);
      }
      Script_LoadVoiceFile(0, "SY11");
    }
  }
  if ((local_F4 >= 2202) & (local_F4 <= 2204)) {
    local_FE = 0;
    if (arg1) {
      if (Script_ryoControl(PCCTL_QueryShouldRun)) {
        Script_ryoControl(PCCTL_DisablePlayerControl);
        Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        local_FF = 1;
      }
      Script_LoadVoiceFile(1, "SY11");
      local_F0 = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f);
      if (local_F0 == 1) {
        local_F8 = Script_ShowUIMessageFromID("SY11A009");
      } else if (local_F0 == 2) {
        local_F8 = Script_ShowUIMessageFromID("SY11A010");
      } else {
        local_F8 = Script_ShowUIMessageFromID("SY11A011");
      }
      while (Script_convGetIsActive((const void *)local_F8)) {
        SCNF_WaitFrames_0000(1);
      }
      Script_LoadVoiceFile(0, "SY11");
    }
  }
  if (arg1) {
    if (local_FF == 1) {
      Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
      Script_ryoControl(RYO_RestoreControl);
    }
  }
  (void)local_FE;
  return;
}

__attribute__((scn_string_bindings(0, data_1108A)))
static void script_AA38(void) {
  int8_t local_FF;
  int32_t local_F8;
  if (task_20D) {
    local_FF = 3;
  } else {
    local_FF = 2;
  }
  SCNF_WaitFrames_0000(30);
  Script_ShowUIMessageFromID("E1072B024");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  SCNF_WaitFrames_0000(30);
  script_CD16();
  SCNF_WaitFrames_0000(30);
  local_F8 = Script_FloatUtils(ScriptMathOp_RandomIntBelow, local_FF);
  if (local_F8 == 1) {
    Script_ShowUIMessageFromID("E1072B093");
  } else if (local_F8 == 0) {
    Script_ShowUIMessageFromID("E1072B094");
  } else {
    Script_ShowUIMessageFromID("E1072B092");
  }
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static int32_t script_AAE1(int32_t arg0, int32_t arg1) {
  int8_t local_FF;
  if ((Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, arg0) == 0) | task_212) {
    local_FF = arg1;
  } else {
    local_FF = -1;
  }
  end:;
}

__attribute__((scn_string_bindings(3, data_110D0, 5, data_110E4, 22, data_1118E, 23, data_11198, 24, data_111A2, 25, data_111AC, 33, data_111FC, 44, data_1126A)))
static void script_AB07(int32_t arg0) {
  int32_t local_FC;
  local_FC = arg0;
  if (local_FC == 0) {
    Script_ShowUIMessageFromID("E1072B131");
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_CLUE_SURROUNDED_BY_FLOWERS, 1);
  } else if (local_FC == 1) {
    Script_ShowUIMessageFromID("E1072B132");
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_CLUE_DANCES_TO_CHEERFUL_MUSIC, 1);
  } else if (local_FC == 2) {
    Script_ShowUIMessageFromID("E1072B133");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_ShowUIMessageFromID("E1072B136");
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_CLUE_COOK_SHOWS_NEW_PATH, 1);
  } else if (local_FC == 3) {
    Script_ShowUIMessageFromID("E1072B135");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_ShowUIMessageFromID("E1072B136");
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_CLUE_GUIDE_IN_PARK, 1);
  } else if (local_FC == 4) {
    Script_ShowUIMessageFromID("E1072B137");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_ShowUIMessageFromID("E1072B138");
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_CLUE_NEON_LIGHTS_GUIDE, 1);
  } else if (local_FC == 5) {
    Script_ShowUIMessageFromID("E1072B139");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_ShowUIMessageFromID("E1072B140");
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_CLUE_WAITS_ON_DARK_STREET, 1);
  } else if (local_FC == 6) {
    Script_ShowUIMessageFromID("E1072B141");
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_CLUE_ON_NIGHT_STREETS, 1);
  } else if (local_FC == 7) {
    Script_ShowUIMessageFromID("E1072B142");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_ShowUIMessageFromID("E1072B143");
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_CLUE_SEEK_AMUSEMENT, 1);
  } else if (local_FC == 8) {
    Script_ShowUIMessageFromID("E1072B144");
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_CLUE_TWO_MEN_WITH_ARM_TATTOOS, 1);
  } else if (local_FC == 9) {
    Script_ShowUIMessageFromID("E1072B145");
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_CLUE_TWO_MEN_NEAR_MOTORCYCLES, 1);
  } else if (local_FC == 10) {
    Script_ShowUIMessageFromID("E1072B146");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_ShowUIMessageFromID("E1072B147");
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_CLUE_OLD_APARTMENT, 1);
  } else if (local_FC == 11) {
    Script_ShowUIMessageFromID("E1072B148");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_ShowUIMessageFromID("E1072B149");
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_CLUE_APARTMENT_DRAGON_DOOR, 1);
  } else if (local_FC == 12) {
    Script_ShowUIMessageFromID("E1072B150");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_ShowUIMessageFromID("E1072B151");
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_CLUE_POND_AND_CHERRY_TREE, 1);
  } else if (local_FC == 13) {
    Script_ShowUIMessageFromID("E1072B152");
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_CLUE_HELP_ONE_TROUBLED, 1);
  } else if (local_FC == 14) {
    Script_ShowUIMessageFromID("E1072B153");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_ShowUIMessageFromID("E1072B154");
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_CLUE_EAST_AMONG_OLD_CHINA, 1);
  } else if (local_FC == 44) {
    Script_ShowUIMessageFromID("E1072B153");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_ShowUIMessageFromID("E1072B154");
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_DURN_FORTUNE_PERSON_EAST_AMONG_OLD_WARES, 1);
  } else if (local_FC == 15) {
    Script_ShowUIMessageFromID("E1072B155");
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_CLUE_TERMS_INTERCHANGEABLE, 1);
  } else if (local_FC == 16) {
    Script_ShowUIMessageFromID("E1072B156");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_ShowUIMessageFromID("E1072B157");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_ShowUIMessageFromID("E1072B158");
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_HEAVEN_IS_DRAGON_EARTH_IS_COMRADE, 1);
  } else if (local_FC == 17) {
    Script_ShowUIMessageFromID("E1072B159");
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_CLUE_ONE_FOND_OF_YOU, 1);
  } else if (local_FC == 18) {
    Script_ShowUIMessageFromID("E1072B160");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_ShowUIMessageFromID("E1072B161");
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_CLUE_SHOP_NEAR_BUS_STOP, 1);
  } else if (local_FC == 19) {
    Script_ShowUIMessageFromID("E1072B162");
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_CLUE_KEEP_SEARCHING, 1);
  } else if (local_FC == 20) {
    Script_ShowUIMessageFromID("E1072B163");
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_CLUE_ALLEY_WITH_WOODEN_CRATES, 1);
  } else if (local_FC == 21) {
    Script_ShowUIMessageFromID("E1072B164");
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_CLUE_SEA_WIND_FROM_NW, 1);
  } else if (local_FC == 22) {
    Script_ShowUIMessageFromID("E1072B165");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_ShowUIMessageFromID("E1072B166");
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_CLUE_FORTUNE_IN_NW_AT_NIGHT, 1);
  } else if (local_FC == 23) {
    Script_ShowUIMessageFromID("E1072B167");
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_CLUE_SOMEONE_FAMILIAR, 1);
  } else if (local_FC == 24) {
    Script_ShowUIMessageFromID("E1072B169");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_ShowUIMessageFromID("E1072B168");
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_CLUE_ELDER_AMONG_ANTIQUES, 1);
  } else if (local_FC == 25) {
    Script_ShowUIMessageFromID("E1072B170");
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_CLUE_ITEM_NEAR_IN_THE_DARK, 1);
  } else if (local_FC == 26) {
    Script_ShowUIMessageFromID("E1072B171");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_ShowUIMessageFromID("E1072B172");
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_CLUE_SWORD_THEN_HANDGUARD, 1);
  } else if (local_FC == 27) {
    Script_ShowUIMessageFromID("E1072B162");
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_CLUE_GODS_HELP_THE_PERSEVERING, 1);
  } else if (local_FC == 28) {
    Script_ShowUIMessageFromID("E1072B173");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_ShowUIMessageFromID("E1072B174");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_ShowUIMessageFromID("E1072B175");
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_CLUE_FATHERS_HEAVEN_WORDS, 1);
  } else if (local_FC == 29) {
    Script_ShowUIMessageFromID("E1072B176");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_ShowUIMessageFromID("E1072B177");
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_DURN_FORTUNE_SEEK_GLOBAL_TRAVEL_AGENCY, 1);
  } else if (local_FC == 30) {
    Script_ShowUIMessageFromID("E1072B179");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_ShowUIMessageFromID("E1072B180");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_ShowUIMessageFromID("E1072B181");
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_DURN_FORTUNE_SEEKER_SHOWS_THE_WAY, 1);
  } else if (local_FC == 31) {
    Script_ShowUIMessageFromID("E1072B182");
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_CLUE_LUCKY_TRAVEL_AGENCY, 1);
  } else if (local_FC == 32) {
    Script_ShowUIMessageFromID("E1072B183");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_ShowUIMessageFromID("E1072B184");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_CLUE_HE_HAS_GONE, 1);
  } else if (local_FC == 33) {
    Script_ShowUIMessageFromID("E1072B185");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_ShowUIMessageFromID("E1072B186");
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_CLUE_LUCKY_PERSON_NE_BY_SEA, 1);
  } else if (local_FC == 34) {
    Script_ShowUIMessageFromID("E1072B187");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_ShowUIMessageFromID("E1072B188");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_ShowUIMessageFromID("E1072B189");
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_CLUE_FISHERMENS_BUILDING, 1);
  } else if (local_FC == 35) {
    Script_ShowUIMessageFromID("E1072B190");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_ShowUIMessageFromID("E1072B191");
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_CLUE_FALLEN_ANGELS, 1);
  } else if (local_FC == 36) {
    Script_ShowUIMessageFromID("E1072B192");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_ShowUIMessageFromID("E1072B193");
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_CLUE_EVIL_IN_THE_DARK, 1);
  } else if (local_FC == 37) {
    Script_ShowUIMessageFromID("E1072B194");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_ShowUIMessageFromID("E1072B195");
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_CLUE_HELP_A_NEW_FRIEND, 1);
  } else if (local_FC == 38) {
    Script_ShowUIMessageFromID("E1072B196");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_ShowUIMessageFromID("E1072B197");
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_CLUE_EVIL_SEA_AT_NIGHT, 1);
  } else if (local_FC == 39) {
    Script_ShowUIMessageFromID("E1072B198");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_ShowUIMessageFromID("E1072B199");
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_CLUE_GRUDGE_HAS_HINT, 1);
  } else if (local_FC == 40) {
    Script_ShowUIMessageFromID("E1072B200");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_ShowUIMessageFromID("E1072B201");
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_CLUE_TWO_PATHS, 1);
  } else if (local_FC == 41) {
    Script_ShowUIMessageFromID("E1072B202");
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_DURN_FORTUNE_SHOP_WITH_MOTORCYCLES, 1);
  } else if (local_FC == 42) {
    Script_ShowUIMessageFromID("E1072B203");
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_CLUE_FRIENDS_LEAVING, 1);
  } else if (local_FC == 43) {
    Script_ShowUIMessageFromID("E1072B178");
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_DURN_FORTUNE_FAMILIAR_PERSON_GUIDES, 1);
  } else {
    script_AA38();
  }
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static int16_t script_B455(void) {
  int16_t local_FE;
  int16_t local_FC;
  int32_t local_F8;
  if (task_212 == 0) {
    local_FE = Script_GetOrResetScenarioStepUniqueItemCount(1);
  } else {
    local_FE = 10;
  }
  local_F8 = SAVE_Script_UpdateForSaveData(4, 4);
  local_FC = -1;
  if ((task_216 == 103) & (local_FE > 4)) {
    local_FC = script_AAE1(51, 0);
  } else if (task_216 == 104) {
    local_FC = script_AAE1(52, 1);
  } else if ((task_216 == 201) & (local_FE > 4)) {
    local_FC = script_AAE1(53, 2);
  } else if ((task_216 == 204) & (local_FE > 4)) {
    local_FC = script_AAE1(54, 3);
  } else if ((task_216 == 301) & (local_FE > 4)) {
    local_FC = script_AAE1(56, 4);
  } else if ((task_216 == 302) & (local_FE > 4)) {
    local_FC = script_AAE1(55, 5);
  } else if (task_216 == 401) {
    local_FC = script_AAE1(57, 6);
  } else if ((task_216 == 402) | (task_216 == 403)) {
    local_FC = script_AAE1(58, 7);
  } else if ((task_216 == 501) & (local_FE > 4)) {
    if (local_FE > 9) {
      local_FC = script_AAE1(66, 9);
    } else {
      local_FC = script_AAE1(59, 8);
    }
  } else if ((task_216 == 502) & (local_FE > 4)) {
    if (local_FE > 9) {
      local_FC = script_AAE1(68, 11);
    } else {
      local_FC = script_AAE1(67, 10);
    }
  } else if (task_216 == 601) {
    local_FC = script_AAE1(69, 12);
  } else if (task_216 == 802) {
    local_FC = script_AAE1(71, 13);
  } else if ((task_216 == 803) & (local_FE > 4)) {
    local_FC = script_AAE1(72, 14);
  } else if ((task_216 == 804) & (local_F8 > 7200)) {
    local_FC = script_AAE1(73, 44);
  } else if ((task_216 == 901) & (Script_scenUtils(SCENARIO_CMD_GET_STEP_LUT, 9) > 2)) {
    if (Script_scenUtils(SCENARIO_CMD_GET_STEP_LUT, 9) > 4) {
      local_FC = script_AAE1(76, 16);
    } else {
      local_FC = script_AAE1(74, 15);
    }
  } else if ((task_216 == 1001) & (local_FE > 4)) {
    if (local_FE > 9) {
      local_FC = script_AAE1(78, 18);
    } else {
      local_FC = script_AAE1(77, 17);
    }
  } else if ((task_216 == 1101) | (task_216 == 1102)) {
    local_FC = script_AAE1(79, 19);
  } else if (task_216 == 1103) {
    local_FC = script_AAE1(81, 20);
  } else if (task_216 == 1201) {
    local_FC = script_AAE1(82, 21);
  } else if ((task_216 >= 1202) & (task_216 < 1301)) {
    local_FC = script_AAE1(83, 22);
  } else if ((task_216 >= 1301) & (task_216 < 1401)) {
    local_FC = script_AAE1(84, 23);
  } else if (task_216 == 1401) {
    local_FC = script_AAE1(86, 24);
  } else if (task_216 == 1501) {
    local_FC = script_AAE1(87, 25);
  } else if (task_216 == 1503) {
    local_FC = script_AAE1(88, 26);
  } else if (task_216 == 1504) {
    local_FC = script_AAE1(89, 27);
  } else if (task_216 == 1601) {
    local_FC = script_AAE1(91, 28);
  } else if (task_216 == 1702) {
    local_FC = script_AAE1(92, 29);
  } else if (task_216 == 1703) {
    local_FC = script_AAE1(171, 43);
  } else if ((task_216 == 1704) & (local_FE > 4)) {
    local_FC = script_AAE1(93, 30);
  } else if ((task_216 >= 1705) & (task_216 < 1902)) {
    local_FC = script_AAE1(94, 31);
  } else if ((task_216 == 1902) | (task_216 == 1903)) {
    local_FC = script_AAE1(96, 32);
  } else if (task_216 == 2001) {
    local_FC = script_AAE1(97, 33);
  } else if (task_216 == 2101) {
    local_FC = script_AAE1(98, 34);
  } else if (task_216 == 2102) {
    local_FC = script_AAE1(99, 35);
  } else if (task_216 == 2103) {
    local_FC = script_AAE1(111, 36);
  } else if (task_216 == 2104) {
    local_FC = script_AAE1(112, 37);
  } else if (task_216 == 2105) {
    local_FC = script_AAE1(113, 38);
  } else if (task_216 == 2106) {
    local_FC = script_AAE1(114, 39);
  } else if (task_216 == 2108) {
    local_FC = script_AAE1(116, 40);
  } else if ((task_216 == 2202) | (task_216 == 2203)) {
    local_FC = script_AAE1(117, 41);
  } else {
    if (task_216 == 2301) {
      local_FC = script_AAE1(118, 42);
      goto L00BA56;
    }
  }
  L00BA56:;
  return local_FC;
}

__attribute__((scn_string_bindings(22, data_11486, 25, data_114A4)))
static void script_BA5A(int32_t arg0) {
  int32_t local_FC;
  local_FC = arg0;
  if (local_FC == 0) {
    Script_ShowUIMessageFromID("E1072B096");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_ShowUIMessageFromID("E1072B097");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_ShowUIMessageFromID("E1072B098");
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_DURN_FORTUNE_SCISSORS_RAZORS_KNIVES, 1);
  } else if (local_FC == 1) {
    Script_ShowUIMessageFromID("E1072B099");
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_DURN_FORTUNE_LONGTIME_RESIDENT_AIDS, 1);
  } else if (local_FC == 2) {
    Script_ShowUIMessageFromID("E1072B100");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_ShowUIMessageFromID("E1072B101");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_ShowUIMessageFromID("E1072B102");
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_DURN_FORTUNE_TURMOIL_IN_NIGHT_TOWN, 1);
  } else if (local_FC == 3) {
    Script_ShowUIMessageFromID("E1072B103");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_ShowUIMessageFromID("E1072B104");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_ShowUIMessageFromID("E1072B105");
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_DURN_FORTUNE_MAN_TIED_TO_DESTINY, 1);
  } else if (local_FC == 4) {
    Script_ShowUIMessageFromID("E1072B106");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_ShowUIMessageFromID("E1072B107");
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_DURN_FORTUNE_WOMAN_BEFORE_MIRROR, 1);
  } else if (local_FC == 5) {
    Script_ShowUIMessageFromID("E1072B108");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_ShowUIMessageFromID("E1072B109");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_ShowUIMessageFromID("E1072B110");
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_DURN_FORTUNE_HELPER_IN_SEA_SCENTED_LAND, 1);
  } else if (local_FC == 6) {
    Script_ShowUIMessageFromID("E1072B111");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_ShowUIMessageFromID("E1072B112");
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_DURN_FORTUNE_PERSON_IN_DARK_WAREHOUSE, 1);
  } else if (local_FC == 7) {
    Script_ShowUIMessageFromID("E1072B113");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_ShowUIMessageFromID("E1072B114");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_ShowUIMessageFromID("E1072B115");
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_DURN_FORTUNE_FAMILY_CREST, 1);
  } else if (local_FC == 8) {
    Script_ShowUIMessageFromID("E1072B116");
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_DURN_FORTUNE_LARGE_AXE, 1);
  } else if (local_FC == 9) {
    Script_ShowUIMessageFromID("E1072B117");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_ShowUIMessageFromID("E1072B118");
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_DURN_FORTUNE_EYES_IN_THE_DARK, 1);
  } else if (local_FC == 10) {
    Script_ShowUIMessageFromID("E1072B119");
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_DURN_FORTUNE_TROUBLE_WITH_WOMEN, 1);
  } else if (local_FC == 11) {
    Script_ShowUIMessageFromID("E1072B120");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_ShowUIMessageFromID("E1072B118");
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_DURN_FORTUNE_INAUSPICIOUS_SHADOW, 1);
  } else if (local_FC == 12) {
    Script_ShowUIMessageFromID("E1072B121");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_ShowUIMessageFromID("E1072B122");
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_DURN_FORTUNE_NEW_FRIEND_BY_THE_SEA, 1);
  } else if (local_FC == 13) {
    Script_ShowUIMessageFromID("E1072B123");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_ShowUIMessageFromID("E1072B124");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_ShowUIMessageFromID("E1072B125");
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_DURN_FORTUNE_ANGEL_FAR_EAST, 1);
  } else if (local_FC == 14) {
    Script_ShowUIMessageFromID("E1072B126");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_ShowUIMessageFromID("E1072B127");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_ShowUIMessageFromID("E1072B128");
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_DURN_FORTUNE_SERPENT_EYES, 1);
  } else if (local_FC == 15) {
    Script_ShowUIMessageFromID("E1072B129");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_ShowUIMessageFromID("E1072B130");
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_DURN_FORTUNE_BLUE_DRAGON_PASSAGE, 1);
  } else {
    script_AA38();
  }
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static int16_t script_BECE(void) {
  int16_t local_FE;
  local_FE = -1;
  if ((task_216 >= 103) & (task_216 < 201)) {
    local_FE = script_AAE1(119, 0);
  } else if ((task_216 >= 201) & (task_216 < 204)) {
    local_FE = script_AAE1(121, 1);
  } else if ((task_216 >= 204) & (task_216 < 303)) {
    local_FE = script_AAE1(122, 2);
  } else if ((task_216 >= 401) & (task_216 < 502)) {
    local_FE = script_AAE1(123, 3);
  } else if ((task_216 >= 502) & (task_216 < 804)) {
    local_FE = script_AAE1(124, 4);
  } else if ((task_216 >= 804) & (task_216 < 1101)) {
    local_FE = script_AAE1(126, 5);
  } else if ((task_216 >= 1101) & (task_216 < 1301)) {
    local_FE = script_AAE1(127, 6);
  } else if ((task_216 >= 1301) & (task_216 < 1401)) {
    local_FE = script_AAE1(128, 7);
  } else if ((task_216 >= 1401) & (task_216 < 1601)) {
    local_FE = script_AAE1(129, 8);
  } else if ((task_216 >= 1601) & (task_216 < 1603)) {
    local_FE = script_AAE1(131, 9);
  } else if ((task_216 >= 1702) & (task_216 < 1706)) {
    local_FE = script_AAE1(132, 10);
  } else if ((task_216 >= 1706) & (task_216 < 2001)) {
    local_FE = script_AAE1(133, 11);
  } else if ((task_216 >= 2001) & (task_216 < 2102)) {
    local_FE = script_AAE1(134, 12);
  } else if ((task_216 >= 2102) & (task_216 < 2106)) {
    local_FE = script_AAE1(136, 13);
  } else if ((task_216 >= 2106) & (task_216 < 2204)) {
    local_FE = script_AAE1(137, 14);
  } else {
    if (task_216 == 2301) {
      local_FE = script_AAE1(138, 15);
      goto L00C136;
    }
  }
  L00C136:;
  return local_FE;
}

static void script_C13A(int32_t arg0) {
  int32_t local_FC;
  arg0 = arg0 % 10;
  local_FC = arg0;
  if (local_FC == 0) {
    Script_ShowUIMessageFromID("E1072B012");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
  } else if (local_FC == 1) {
    Script_ShowUIMessageFromID("E1072B013");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
  } else if (local_FC == 2) {
    Script_ShowUIMessageFromID("E1072B014");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
  } else if (local_FC == 3) {
    Script_ShowUIMessageFromID("E1072B015");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
  } else if (local_FC == 4) {
    Script_ShowUIMessageFromID("E1072B016");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
  } else if (local_FC == 5) {
    Script_ShowUIMessageFromID("E1072B017");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
  } else if (local_FC == 6) {
    Script_ShowUIMessageFromID("E1072B018");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
  } else if (local_FC == 7) {
    Script_ShowUIMessageFromID("E1072B019");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
  } else if (local_FC == 8) {
    Script_ShowUIMessageFromID("E1072B020");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
  } else {
    Script_ShowUIMessageFromID("E1072B021");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
  }
  return;
}

static void script_C2E4(int32_t arg0) {
  int8_t local_FF;
  int32_t local_F8;
  local_FF = 0;
  for (;;) {
    local_F8 = Script_ScenDataUtil(data_11578, 4, local_FF);
    if ((local_F8 == arg0) | (local_F8 == -1)) {
      break;
    } else {
      local_FF = local_FF + 6;
    }
  }
  (void)local_FF;
  return;
}

static int32_t script_C324(void) {
  int16_t local_FE;
  int32_t local_F8;
  local_FE = 300;
  for (;;) {
    local_FE = local_FE - 1;
    if (task_23C | (local_FE == 0)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  if (local_FE == 0) {
    local_F8 = Script_FloatUtils(ScriptMathOp_RandomIntBelow, 3);
    if (local_F8 == 0) {
      Script_ShowUIMessageFromID("E1072B089");
    } else if (local_F8 == 1) {
      Script_ShowUIMessageFromID("E1072B090");
    } else {
      Script_ShowUIMessageFromID("E1072B091");
    }
  }
  return Script_srchControl(0);
}

static int32_t script_C3A1(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  Script_scenCreateTask(script_C324, 0);
  task_23C = 0;
  local_FC = SCNF_RequestActionSelectorFromParams_0008(arg0, arg1, arg2, arg3, 600, 0xFFFF, ACTION_SELECTOR_MODE_STANDARD_TIMEOUT_CANCEL);
  task_23C = 1;
  return local_FC;
}

static void script_C3DD(void) {
  int8_t local_FF;
  local_FF = 0;
  for (;;) {
    if (task_218[local_FF] != 0) {
      Script_charaDisplay(task_218[local_FF], CDM_Hide);
    }
    if (task_220[local_FF]) {
      Script_CycleManagerUtils(CYCLE_CMD_LOAD_OR_RESET_RUNNING_PROGRAM, task_220[local_FF]);
    }
    local_FF = local_FF + 1;
    if (local_FF > 1) {
      break;
    }
  }
  return;
}

static int32_t script_C43E(void) {
  Vector3 local_F4;
  int8_t local_F3;
  int32_t local_EC;
  local_F3 = 0;
  local_F4.x = 0.4f;
  (&local_F4)->y = 0.4f;
  (&local_F4)->z = 0.4f;
  Script_SetCPAMFlags(SHEN_NAME('FIR3'), 0);
  Script_hmdlApplyScale(SHEN_NAME('FIR3'), &local_F4);
  task_22C = 0.0f;
  Script_charaSetRenderOpacity(SHEN_NAME('FIR3'), task_22C);
  for (;;) {
    local_EC = local_F3;
    switch (local_EC) {
      case 0:
        Script_scenToggleVisibleFor(SHEN_NAME('FIR3'), 0);
        Script_charaDisplay(SHEN_NAME('FIR3'), CDM_Hide);
        local_F3 = 1;
        break;
      case 1:
        if (*task_218 == 0x5F756F72) {
          if (task_213) {
            local_F3 = 2;
            Script_scenToggleVisibleFor(SHEN_NAME('FIR3'), 1);
            Script_charaDisplay(SHEN_NAME('FIR3'), CDM_Show);
          }
        }
        break;
      case 2:
        Script_charaSetRenderOpacity(SHEN_NAME('FIR3'), task_22C);
        if ((task_213 == 0) | (*task_218 != 0x5F756F72)) {
          local_F3 = 0;
        }
        break;
    }
    task_216 = Script_StepUtils(GET_STEP_HIGHER, 0) * 100 + Script_StepUtils(GET_STEP_LOWER, 0);
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_C55C(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  local_FC = 0;
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0) == 0) {
    if (arg1 & 0x40000000) {
      Script_skyCtrlLuminance(0, 1);
    } else {
      Script_skyCtrlLuminance(0, 0);
    }
    Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT, arg0, 1);
    while ((local_FC = Script_CycleManagerUtils(CYCLE_CMD_GET_CYCLE_PTR_BY_CHARA_ID, arg0)) == 0) {
      SCNF_WaitFrames_0000(1);
    }
    while (Script_CycleManagerUtils(CYCLE_CMD_GET_NEXT_TASK_POINTER, local_FC) == 0) {
      SCNF_WaitFrames_0000(1);
    }
    Script_skyCtrlLuminance(0, 0);
  }
  return local_FC;
}

static void script_C5E9(int32_t arg0, int32_t arg1) {
  *(int32_t *)&task_230.x = Script_ScenDataUtil(data_11578, 4, arg1 * 6 + 2);
  *(int32_t *)&task_230.y = Script_ScenDataUtil(data_11578, 4, arg1 * 6 + 3);
  *(int32_t *)&task_230.z = Script_ScenDataUtil(data_11578, 4, arg1 * 6 + 4);
  Script_EVT_SetPosition(arg0, POS_XYZ, &task_230);
  return;
}

static void script_C64E(int32_t arg0) {
  task_218[arg0] = 0;
  task_220[arg0] = 0;
  if (*((int8_t *)&*(int32_t *)&task_210 + arg0) != -1) {
    task_218[arg0] = Script_ScenDataUtil(data_11578, 4, *((int8_t *)&*(int32_t *)&task_210 + arg0) * 6);
    task_220[arg0] = script_C55C(task_218[arg0], Script_ScenDataUtil(data_11578, 4, *((int8_t *)&*(int32_t *)&task_210 + arg0) * 6 + 1));
    if (*((int8_t *)&*(int32_t *)&task_210 + arg0) <= 23) {
      Script_motmToggleFlags(task_218[arg0], 1, 4);
      Script_PlayCharaMotion(task_218[arg0], Script_ScenDataUtil(data_11578, 4, *((int8_t *)&*(int32_t *)&task_210 + arg0) * 6 + 5), -1.0f, -1.0f, -1.0f, 1.0f, 0);
    }
  }
  return;
}

static void script_C730(void) {
  if (task_216 == 103) {
    *(int8_t *)&task_210 = 24;
  } else if (task_216 == 204) {
    *(int8_t *)&task_210 = 1;
  } else if (task_216 == 301) {
    *(int8_t *)&task_210 = 11;
  } else if (task_216 == 302) {
    *(int8_t *)&task_210 = 2;
  } else if (task_216 == 401) {
    *(int8_t *)&task_210 = 12;
  } else if (task_216 == 501) {
    *(int8_t *)&task_210 = 13;
  } else if (task_216 == 502) {
    if (Script_GetOrResetScenarioStepUniqueItemCount(1) > 9) {
      *(int8_t *)&task_210 = 25;
    } else {
      *(int8_t *)&task_210 = -1;
    }
  } else if ((task_216 >= 803) & (task_216 < 901)) {
    *(int8_t *)&task_210 = 4;
  } else if (task_216 == 1001) {
    if (Script_GetOrResetScenarioStepUniqueItemCount(1) > 9) {
      *(int8_t *)&task_210 = 14;
    } else {
      *(int8_t *)&task_210 = -1;
    }
  } else if ((task_216 >= 1101) & (task_216 < 1103)) {
    *(int8_t *)&task_210 = 15;
  } else if (task_216 == 1103) {
    *(int8_t *)&task_210 = 26;
  } else if ((task_216 >= 1201) & (task_216 < 1301)) {
    *(int8_t *)&task_210 = 16;
  } else if (task_216 == 1401) {
    *(int8_t *)&task_210 = 27;
  } else if (task_216 == 1501) {
    *(int8_t *)&task_210 = 28;
  } else if (task_216 == 1503) {
    *(int8_t *)&task_210 = 29;
  } else if (task_216 == 1702) {
    *(int8_t *)&task_210 = 17;
  } else if ((task_216 >= 1705) & (task_216 < 1902)) {
    *(int8_t *)&task_210 = 7;
  } else if (task_216 == 2101) {
    *(int8_t *)&task_210 = 18;
  } else if (task_216 == 2102) {
    *(int8_t *)&task_210 = 19;
  } else if (task_216 == 2103) {
    *(int8_t *)&task_210 = 20;
  } else if (task_216 == 2105) {
    *(int8_t *)&task_210 = 21;
  } else if (task_216 == 2106) {
    *(int8_t *)&task_210 = 22;
  } else if ((task_216 >= 2202) & (task_216 < 2204)) {
    *(int8_t *)&task_210 = 23;
  } else {
    *(int8_t *)&task_210 = -1;
  }
  script_C64E(0);
  return;
}

static void script_C97F(void) {
  if ((task_216 >= 103) & (task_216 < 201)) {
    *((int8_t *)&*(int32_t *)&task_210 + 1) = 0;
  } else if ((task_216 >= 201) & (task_216 < 204)) {
    *((int8_t *)&*(int32_t *)&task_210 + 1) = 1;
  } else if ((task_216 >= 204) & (task_216 < 303)) {
    *((int8_t *)&*(int32_t *)&task_210 + 1) = 2;
  } else if ((task_216 >= 401) & (task_216 < 502)) {
    *((int8_t *)&*(int32_t *)&task_210 + 1) = 3;
  } else if ((task_216 >= 502) & (task_216 < 804)) {
    *((int8_t *)&*(int32_t *)&task_210 + 1) = 4;
  } else if ((task_216 >= 804) & (task_216 < 1101)) {
    *((int8_t *)&*(int32_t *)&task_210 + 1) = 5;
  } else if ((task_216 >= 1101) & (task_216 < 1205)) {
    *((int8_t *)&*(int32_t *)&task_210 + 1) = 30;
  } else if ((task_216 >= 1301) & (task_216 < 1401)) {
    *((int8_t *)&*(int32_t *)&task_210 + 1) = 28;
  } else if ((task_216 >= 1401) & (task_216 < 1601)) {
    *((int8_t *)&*(int32_t *)&task_210 + 1) = 31;
  } else if ((task_216 >= 1601) & (task_216 < 1603)) {
    *((int8_t *)&*(int32_t *)&task_210 + 1) = 6;
  } else if ((task_216 >= 1702) & (task_216 < 1706)) {
    *((int8_t *)&*(int32_t *)&task_210 + 1) = 7;
  } else if ((task_216 >= 1706) & (task_216 < 2001)) {
    *((int8_t *)&*(int32_t *)&task_210 + 1) = 6;
  } else if ((task_216 >= 2001) & (task_216 < 2102)) {
    *((int8_t *)&*(int32_t *)&task_210 + 1) = 8;
  } else if ((task_216 >= 2102) & (task_216 < 2106)) {
    *((int8_t *)&*(int32_t *)&task_210 + 1) = 32;
  } else if ((task_216 >= 2106) & (task_216 < 2204)) {
    *((int8_t *)&*(int32_t *)&task_210 + 1) = 9;
  } else if (task_216 == 2301) {
    *((int8_t *)&*(int32_t *)&task_210 + 1) = 10;
  } else {
    *((int8_t *)&*(int32_t *)&task_210 + 1) = -1;
  }
  script_C64E(1);
  return;
}

static void script_CB8C(int32_t arg0) {
  int32_t unused_FC;
  Script_CycleManagerUtils(CYCLE_CMD_SET_CYCLE_MANAGER_ENABLED, 1);
  script_C730();
  script_C97F();
  Script_CycleManagerUtils(CYCLE_CMD_SET_CYCLE_MANAGER_ENABLED, 0);
  if (arg0) {
    Script_srchControl(0);
  }
  return;
}

__attribute__((scn_string_bindings(1, data_118BC, 3, data_118D0)))
static int8_t script_CBB7(void) {
  int8_t local_FF;
  int32_t local_F8;
  local_FF = script_A557(0, 300, 0);
  local_F8 = local_FF;
  if ((local_F8 == 1) | (local_F8 == 2)) {
    if (Script_scenCharaGetMotAnimId(CHARA_AKIR) == 726) {
      Script_seqEnterEvtCam(15, 0, AUTH_None);
    } else {
      Script_ConversationUtils(0, 0, 0x52494B41, 0x534D414E, -1.0f, 200.0f);
    }
    Script_ShowUIMessageFromID("E1072A015");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_ShowUIMessageFromID("E1072B006");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
  } else {
    if (local_F8 == 3) {
      if (Script_scenCharaGetMotAnimId(CHARA_AKIR) == 726) {
        Script_seqEnterEvtCam(15, 0, AUTH_None);
      } else {
        Script_ConversationUtils(0, 0, 0x52494B41, 0x534D414E, -1.0f, 200.0f);
      }
      Script_ShowUIMessageFromID("E1072A020");
      while (Script_convGetIsActive(0)) {
        SCNF_WaitFrames_0000(1);
      }
      Script_ShowUIMessageFromID("E1072B006");
      while (Script_convGetIsActive(0)) {
        SCNF_WaitFrames_0000(1);
      }
      goto L00CCE2;
    }
  }
  L00CCE2:;
  return local_FF;
}

static int32_t script_CCE6(void) {
  for (;;) {
    if (Script_IsCharaMotionFinished(CHARA_AKIR)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_clmdSetFlags(CHARA_AKIR, 4, 1);
  return Script_srchControl(0);
}

static void script_CD16(void) {
  int32_t local_FC;
  local_FC = Script_FloatUtils(ScriptMathOp_RandomIntBelow, 3);
  if (local_FC == 0) {
    Script_ShowUIMessageFromID("E1072B025");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
  } else if (local_FC == 1) {
    Script_ShowUIMessageFromID("E1072B026");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
  } else {
    Script_ShowUIMessageFromID("E1072B027");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
  }
  return;
}

__attribute__((scn_string_bindings(0, data_118F8, 1, data_11902, 2, data_1190C)))
static void script_CD98(void) {
  Script_ShowUIMessageFromID("E1072B009");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  SCNF_WaitFrames_0000(20);
  Script_ShowUIMessageFromID("E1072B024");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  script_CD16();
  SCNF_WaitFrames_0000(20);
  Script_ShowUIMessageFromID("E1072B010");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

__attribute__((scn_string_bindings(0, data_11916)))
static void script_CE0A(void) {
  Script_ConversationUtils(0, 0, 0x52494B41, 0x534D414E, -1.0f, 200.0f);
  if (Script_FloatUtils(ScriptMathOp_RandomIntBelow, 2)) {
    Script_ShowUIMessageFromID("E1072A001");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
  } else {
    Script_ShowUIMessageFromID("E1072A002");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
  }
  Script_ConversationUtils(0, 1, 0x534D414E, 0x52494B41, 1.0f, 200.0f);
  if (Script_FloatUtils(ScriptMathOp_RandomIntBelow, 2)) {
    Script_ShowUIMessageFromID("E1072B001");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
  } else {
    Script_ShowUIMessageFromID("E1072B002");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
  }
  Script_ConversationUtils(0, 0, 0x52494B41, 0x534D414E, -1.0f, 200.0f);
  if (Script_FloatUtils(ScriptMathOp_RandomIntBelow, 2)) {
    Script_ShowUIMessageFromID("E1072A003");
  } else {
    Script_ShowUIMessageFromID("E1072A004");
  }
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_ShowUIMessageFromID("E1072B003");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_ShowUIMessageFromID("E1072B088");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

__attribute__((scn_string_bindings(0, data_11966, 1, data_11970)))
static void script_CF69(void) {
  Script_ConversationUtils(0, 0, 0x52494B41, 0x534D414E, -1.0f, 200.0f);
  Script_ShowUIMessageFromID("E1072A001");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_ConversationUtils(0, 1, 0x534D414E, 0x52494B41, 1.0f, 200.0f);
  Script_ShowUIMessageFromID("E1072B059");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

__attribute__((scn_string_bindings(0, data_1197A, 1, data_11984, 2, data_1198E, 3, data_11998)))
static void script_CFE2(int32_t arg0) {
  if (arg0) {
    Script_seqEnterEvtCam(14, 0, AUTH_None);
  }
  Script_ShowUIMessageFromID("E1072A005");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  if (arg0 == 0) {
    Script_ConversationUtils(0, 0, 0x534D414E, 0x52494B41, 1.0f, 200.0f);
  }
  Script_ShowUIMessageFromID("E1072B005");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_ShowUIMessageFromID("E1072B006");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  if (arg0 == 0) {
    Script_ConversationUtils(0, 0, 0x52494B41, 0x534D414E, -1.0f, 200.0f);
  }
  Script_ShowUIMessageFromID("E1072A006");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_D0B9(void) {
  Script_ShowUIMessageFromID("E1072A008");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_ShowUIMessageFromID("E1072B060");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_D0F8(void) {
  Script_ConversationUtils(0, 0, 0x52494B41, 0x534D414E, -1.0f, 200.0f);
  Script_ShowUIMessageFromID("E1072A009");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_D136(void) {
  Script_ConversationUtils(0, 0, 0x52494B41, 0x534D414E, -1.0f, 200.0f);
  Script_ShowUIMessageFromID("E1072A017");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_D174(void) {
  Script_ConversationUtils(0, 0, 0x52494B41, 0x534D414E, -1.0f, 200.0f);
  Script_ShowUIMessageFromID("E1072A018");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static int32_t script_D1B2(int32_t arg0) {
  if (*task_218) {
    Script_seqEnterEvtCam(11, 0, AUTH_None);
  } else {
    Script_seqEnterEvtCam(51, 0, AUTH_None);
  }
  task_20F = 0;
  if (task_214) {
    script_CD98();
    SCNF_WaitFrames_0000(30);
  }
  script_AB07(arg0);
  task_20F = 1;
  return Script_srchControl(0);
}

static int32_t script_D205(int32_t arg0) {
  if (task_218[1]) {
    Script_seqEnterEvtCam(12, 0, AUTH_None);
  } else {
    Script_seqEnterEvtCam(51, 0, AUTH_None);
  }
  task_20F = 0;
  if (task_214) {
    script_CD98();
    SCNF_WaitFrames_0000(30);
  }
  script_BA5A(arg0);
  task_20F = 1;
  return Script_srchControl(0);
}

__attribute__((scn_string_bindings(0, data_119D4, 1, data_119DE, 2, data_119E8)))
static void script_D25B(void) {
  Script_seqEnterEvtCam(13, 0, AUTH_None);
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 189) == 0) {
    script_AA38();
    task_214 = 0;
  } else {
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 189, 0);
    Script_ShowUIMessageFromID("E1072B009");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_ShowUIMessageFromID("E1072B024");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_ShowUIMessageFromID("E1072B010");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    SCNF_WaitFrames_0000(30);
    Script_ShowUIMessageFromID("E1072B011");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    SCNF_WaitFrames_0000(30);
    script_C13A(Script_scenUtils(SCENARIO_CMD_GET_STEP_LUT, 11));
    SCNF_WaitFrames_0000(30);
    Script_ShowUIMessageFromID("E1072B022");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    SCNF_WaitFrames_0000(30);
    Script_ConversationUtils(0, 539, 0x52494B41, 0x534D414E, -1.0f, 200.0f);
    Script_ShowUIMessageFromID("E1072A012");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    task_214 = 1;
  }
  return;
}

__attribute__((scn_string_bindings(0, data_11A10)))
static void script_D39C(void) {
  Script_ShowUIMessageFromID("E1072B059");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

__attribute__((scn_string_bindings(1, data_11A24, 2, data_11A2E, 3, data_11A38)))
static void script_D3BD(void) {
  Script_seqEnterEvtCam(14, 0, AUTH_None);
  Script_ShowUIMessageFromID("E1072A019");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_ShowUIMessageFromID("E1072B005");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_ShowUIMessageFromID("E1072B006");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_ShowUIMessageFromID("E1072A006");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

__attribute__((scn_string_bindings(0, data_11A42, 1, data_11A4C)))
static void script_D442(void) {
  Script_seqEnterEvtCam(14, 0, AUTH_None);
  Script_ShowUIMessageFromID("E1072A005");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_ShowUIMessageFromID("E1072B005");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_D48B(void) {
  Script_ConversationUtils(0, 1, 0x534D414E, 0x52494B41, 1.0f, 200.0f);
  SCNT_ShowSubtitle("●赤先ほど占ったばかりですが…");
  SCNT_ShowSubtitle("●赤もう一度、占いますか？");
  script_C3A1(0xFFFF, 49, 1, 0xFFFF);
  return;
}

static void script_D4CA(void) {
  script_D0F8();
  script_D48B();
  return;
}

static void script_D4D7(void) {
  script_D136();
  script_D48B();
  return;
}

static void script_D4E4(void) {
  script_D174();
  script_D48B();
  return;
}

static void script_D4F1(void) {
  int8_t local_FF;
  int8_t local_FE;
  uint8_t unused_FC[2];
  Vector3 local_F0;
  int32_t local_EC;
  int8_t local_EB;
  uint8_t unused_E4[7];
  int32_t local_E0;
  Script_PlayerStatsUtils(PS_SetCharaItemCount, 0x534D414E, Script_PlayerStatsUtils(PS_GetCharaItemCount, 0x534D414E) + 1);
  local_F0.x = 2.1f;
  (&local_F0)->y = 0.75f;
  (&local_F0)->z = -2.5538f;
  local_FE = 0;
  local_FF = 0;
  Script_SetCPAMFlags(CHARA_NAMS, 0);
  Script_SetCPAMFlags(CHARA_AKIR, 0);
  SCNF_WaitFrames_0000(1);
  Script_charaSetRenderOpacity(CHARA_AKIR, 1.0f);
  for (;;) {
    local_EC = local_FF;
    switch (local_EC) {
      case 0:
        local_FF = 1;
        Script_ccowUtils(CHARA_AKIR, 2);
        Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        break;
      case 1:
        Script_charaSetRenderOpacity(CHARA_AKIR, 1.0f);
        if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, 0x534D414E) & Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, 0x52494B41)) {
          Script_EVT_GetPosition(CHARA_AKIR, 4, (Vector3 *)&local_E0, ENTITY_XFORM_WORLD_SPACE);
          Script_SetLookPointDetails(CHARA_NAMS, 16, (const Vector3 *)&local_E0, 2);
          Script_FaceLookAtTargetUtils(CHARA_NAMS, 2, (const Vector3 *)&local_E0, 30);
        }
        if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, 0x52494B41) & Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, 0x534D414E)) {
          Script_EVT_GetPosition(CHARA_NAMS, 4, (Vector3 *)&local_E0, ENTITY_XFORM_WORLD_SPACE);
          Script_SetLookPointDetails(CHARA_AKIR, 16, (const Vector3 *)&local_E0, 2);
          Script_FaceLookAtTargetUtils(CHARA_AKIR, 2, (const Vector3 *)&local_E0, 30);
        }
        if (task_20D) {
          script_CF69();
        } else {
          script_CE0A();
        }
        local_FE = script_C3A1(0xFFFF, 49, 1, 0xFFFF);
        if (local_FE != 49) {
          script_CFE2(0);
          Script_SetGameStateTo(AUTH_None);
          Script_convSetCamInactiveFlag(1);
          local_FF = 11;
        } else if (script_CBB7()) {
          Script_SetGameStateTo(AUTH_None);
          Script_convSetCamInactiveFlag(1);
          local_FF = 11;
        } else {
          local_FF = 2;
        }
        break;
      case 2:
        local_FF = 3;
        local_E0 = 0x4002A162;
        *(&local_E0 + 2) = -0x404449BA;
        Script_EVT_SetPosition(CHARA_AKIR, 0x28000000, (const Vector3 *)&local_E0);
        *(&local_E0 + 1) = 0;
        Script_EVT_SetRotation(CHARA_AKIR, MATRIX_XFORM_Y, (const Vector3i *)&local_E0, 0);
        Script_motmToggleFlags(CHARA_AKIR, 1, 8);
        Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_SIT_CHAIR, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        Script_scenCreateTask(script_CCE6, 0);
        break;
      case 3:
        if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, 0x534D414E) & Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, 0x52494B41)) {
          Script_EVT_GetPosition(CHARA_AKIR, 4, (Vector3 *)&local_E0, ENTITY_XFORM_WORLD_SPACE);
          Script_SetLookPointDetails(CHARA_NAMS, 16, (const Vector3 *)&local_E0, 2);
          Script_FaceLookAtTargetUtils(CHARA_NAMS, 2, (const Vector3 *)&local_E0, 30);
        }
        if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, 0x52494B41) & Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, 0x534D414E)) {
          Script_EVT_GetPosition(CHARA_NAMS, 4, (Vector3 *)&local_E0, ENTITY_XFORM_WORLD_SPACE);
          Script_SetLookPointDetails(CHARA_AKIR, 16, (const Vector3 *)&local_E0, 2);
          Script_FaceLookAtTargetUtils(CHARA_AKIR, 2, (const Vector3 *)&local_E0, 30);
        }
        Script_seqEnterEvtCam(10, 0, AUTH_None);
        script_D0B9();
        Script_shenCashUtils(1);
        local_FE = script_C3A1(65, 64, 52, 34);
        Script_shenCashUtils(0);
        local_EC = local_FE;
        if (local_EC == 64) {
          local_FF = 5;
          script_D0F8();
        } else if (local_EC == 65) {
          local_FF = 6;
          script_D136();
        } else if (local_EC == 52) {
          local_FF = 7;
          script_D174();
        } else {
          local_FF = 9;
        }
        break;
      case 4:
        local_FF = 8;
        SCNF_SetCashValue(-300, 1);
        task_20D = 1;
        break;
      case 5:
        local_FF = 8;
        Script_SetCharaLookAtPosition(CHARA_NAMS, 1, &local_F0, 1);
        local_EB = script_B455();
        if (local_EB == -1) {
          task_214 = 0;
          local_FF = 8;
        } else {
          task_214 = 1;
          local_FF = 4;
        }
        Script_scenCreateTask(script_D1B2, 1, local_EB);
        script_DE78(0);
        Script_SetCharaLookAtPosition(CHARA_NAMS, -98312, 0, 0);
        break;
      case 6:
        local_FF = 8;
        Script_SetCharaLookAtPosition(CHARA_NAMS, 1, &local_F0, 1);
        local_EB = script_BECE();
        if (local_EB == -1) {
          task_214 = 0;
          local_FF = 8;
        } else {
          task_214 = 1;
          local_FF = 4;
        }
        Script_scenCreateTask(script_D205, 1, local_EB);
        script_DE78(1);
        Script_SetCharaLookAtPosition(CHARA_NAMS, -98312, 0, 0);
        break;
      case 7:
        Script_SetCharaLookAtPosition(CHARA_NAMS, 1, &local_F0, 1);
        Script_PlayCharaMotion(CHARA_NAMS, 4117, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        script_D25B();
        if (task_214) {
          local_FF = 4;
        } else {
          local_FF = 8;
        }
        Script_SetCharaLookAtPosition(CHARA_NAMS, -98312, 0, 0);
        break;
      case 8:
        Script_motmSetBlendFrames(CHARA_NAMS, MOTM_BLEND_FRAMES_20);
        Script_PlayCharaMotion(CHARA_NAMS, 4120, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        local_FF = 10;
        script_D39C();
        if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_EV_STOP_ACTIVE)) {
          script_D3BD();
        } else {
          local_FE = script_C3A1(0xFFFF, 49, 1, 0xFFFF);
          if (local_FE == 49) {
            if (script_CBB7() == 0) {
              local_FF = 3;
            }
          } else {
            script_D3BD();
          }
        }
        break;
      case 9:
        local_FF = 10;
        script_CFE2(1);
        break;
      case 10:
        local_FF = 11;
        Script_motmSetBlendFrames(CHARA_AKIR, MOTM_BLEND_FRAMES_20);
        Script_motmToggleFlags(CHARA_AKIR, 2, 8);
        Script_charaToggleMotEnabled(CHARA_AKIR, 1);
        Script_motmToggleFlags(CHARA_AKIR, 1, 0x20);
        Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_SIT_CHAIR, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        Script_clmdSetFlags(CHARA_AKIR, 4, 0);
        SCNF_WaitFrames_0000(25);
        Script_SetGameStateTo(AUTH_None);
        Script_convSetCamInactiveFlag(1);
        local_E0 = 0x40351EB8;
        *(&local_E0 + 2) = -0x407C28F6;
        Script_EVT_SetPosition(CHARA_AKIR, 0x28000000, (const Vector3 *)&local_E0);
        *(&local_E0 + 1) = 0;
        Script_EVT_SetRotation(CHARA_AKIR, MATRIX_XFORM_Y, (const Vector3i *)&local_E0, 0);
        Script_InitializeOsag(CHARA_AKIR);
        Script_motmToggleFlags(CHARA_AKIR, 2, 0x20);
        Script_motmUtilsEx(0, 0x52494B41, 0);
        Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        break;
      case 11:
        local_FF = 255;
        if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, 0x534D414E)) {
          Script_SetCharaLookAtPosition(CHARA_NAMS, -13, 0, 0);
          Script_FaceLookAtTargetUtils(CHARA_NAMS, 0);
        }
        if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, 0x52494B41)) {
          Script_SetCharaLookAtPosition(CHARA_AKIR, -10, 0, 0);
          Script_FaceLookAtTargetUtils(CHARA_AKIR, 0);
        }
        Script_ccowUtils(CHARA_AKIR, 1);
        Script_SetCPAMFlags(CHARA_NAMS, 1);
        Script_SetCPAMFlags(CHARA_AKIR, 1);
        goto L00DC9C;
        break;
      case 12:
        if (Script_GetControllerInput(1) & 0x200) {
          local_FF = 10;
        }
        break;
    }
    SCNF_WaitFrames_0000(1);
  }
  L00DC9C:;
  return;
}

static void script_DC9D(void) {
  float local_FC;
  for (;;) {
    local_FC = Script_GetCharaMotionRemainingFramesScaled(CHARA_NAMS);
    if (!(local_FC <= 190.0f) & !(local_FC >= 210.0f)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_DCD1(int32_t arg0, int32_t arg1) {
  Vector3i local_F4;
  int32_t local_F0;
  local_F4.x = 0;
  (&local_F4)->y = 0;
  (&local_F4)->z = 0;
  local_F0 = arg1;
  switch (local_F0) {
    case 30:
      local_F4.x = 8009;
      break;
    case 31:
      local_F4.x = 4187;
      (&local_F4)->y = 36408;
      (&local_F4)->z = 6735;
      break;
  }
  Script_EVT_SetRotation(arg0, 0x38000000, &local_F4, 0);
  return;
}

static void script_DD3D(int32_t arg0, int32_t arg1) {
  Vector3i local_F4;
  int32_t local_F0;
  Vector3 local_E4;
  float local_E0;
  script_C5E9(arg0, arg1);
  local_F0 = Script_ScenDataUtil(data_11578, 4, arg1 * 6 + 1);
  if (local_F0 & 0x40000000) {
    task_228 = 0.9f;
  } else if (local_F0 & 0x20000000) {
    task_228 = 0.6f;
  } else {
    task_228 = 0.408f;
  }
  local_F4.x = 0;
  (&local_F4)->y = local_F0 & 0xFFFF;
  (&local_F4)->z = 0;
  Script_SetCPAMFlags(arg0, 0);
  if (arg1 <= 23) {
    Script_EVT_SetRotation(arg0, 0x38000000, &local_F4, 0);
    Script_GetSetMotionScale(arg0, 1, 0.1f);
    Script_GetSetMotionScale(arg0, 3, 0.1f);
    Script_clmdSetFlags(arg0, 8, 1);
    Script_clmdSetFlags(arg0, 2, 1);
    Script_charaToggleMotEnabled(arg0, 0);
  } else {
    script_DCD1(arg0, arg1);
    *(int32_t *)&local_E0 = Script_ScenDataUtil(data_11578, 4, arg1 * 6 + 5);
    local_E4.x = local_E0;
    (&local_E4)->y = local_E0;
    (&local_E4)->z = local_E0;
    Script_hmdlApplyScale(arg0, &local_E4);
  }
  Script_scenToggleVisibleFor(arg0, 1);
  Script_charaDisplay(arg0, CDM_Show);
  Script_charaSetRenderOpacity(arg0, 0.0f);
  return;
}

static void script_DE78(int32_t arg0) {
  int8_t local_FF;
  int16_t local_FC;
  int32_t local_F8;
  Vector3 local_EC;
  Vector3 local_E0;
  float local_DC;
  float local_D8;
  float local_D4;
  if (task_214) {
    local_F8 = task_218[arg0];
    local_FF = *((int8_t *)&*(int32_t *)&task_210 + arg0);
  } else {
    local_F8 = 0;
  }
  Script_motmSetBlendFrames(CHARA_NAMS, MOTM_BLEND_FRAMES_20);
  Script_PlayCharaMotion(CHARA_NAMS, 4118, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  for (;;) {
    if (Script_IsCharaMotionFinished(CHARA_NAMS)) {
      break;
    }
    Script_RotateNodeUtils(CHARA_NAMS, &data_F15C, 0);
    SCNF_WaitFrames_0000(1);
  }
  Script_PlayCharaMotion(CHARA_NAMS, 4117, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  if (local_F8 == 0) {
    for (;;) {
      if (task_20F) {
        break;
      }
      SCNF_WaitFrames_0000(1);
    }
  } else {
    script_DD3D(local_F8, local_FF);
    local_D8 = 0.622666f;
    local_D4 = 0.353577f;
    local_FC = 250;
    script_DC9D();
    if (local_F8 == 0x5F756F72) {
      task_213 = 1;
    }
    for (;;) {
      Script_EVT_GetPosition(CHARA_NAMS, 18, &local_E0, ENTITY_XFORM_WORLD_SPACE);
      local_DC = Script_MathUtils(SCRIPT_MATH_TASK_VEC3_DISTANCE, &local_E0, &task_230);
      if (!(local_D8 >= local_DC)) {
        local_D8 = local_DC;
      }
      if (!(local_D4 <= local_DC)) {
        local_D4 = local_DC;
      }
      task_22C = (task_228 - 0.1f) * (local_D8 - local_DC) / (local_D8 - local_D4) + 0.1f;
      Script_charaSetRenderOpacity(local_F8, task_22C);
      SCNF_WaitFrames_0000(1);
      if (local_FC != 0) {
        local_FC = local_FC - 1;
      }
      if (-(local_FC == 0) & task_20F) {
        break;
      }
    }
    task_213 = 0;
    local_EC.x = 102.1f;
    (&local_EC)->y = 0.751f;
    (&local_EC)->z = -2.5538f;
    Script_EVT_SetPosition(local_F8, POS_XYZ, &local_EC);
    Script_scenToggleVisibleFor(local_F8, 0);
    Script_charaDisplay(local_F8, CDM_Hide);
  }
  Script_ConversationUtils(0, 539, 0x52494B41, 0x534D414E, -1.0f, 200.0f);
  Script_ShowUIMessageFromID(data_11A90);
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_E094(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t local_E4;
  int32_t local_E0;
  local_E0 = arg3;
  switch (local_E0) {
    case 0:
      local_F4.x = 0.1f;
      (&local_F4)->y = 0.0f;
      (&local_F4)->z = -0.01f;
      local_E8.x = 0;
      (&local_E8)->y = 64443;
      (&local_E8)->z = 0;
      local_E4 = 18;
      break;
    case 1:
      local_F4.x = 0.163f;
      (&local_F4)->y = 0.015f;
      (&local_F4)->z = 0.014f;
      local_E8.x = 0;
      (&local_E8)->y = 0x8000;
      (&local_E8)->z = 0;
      local_E4 = 18;
      break;
    case 2:
      local_F4.x = 0.064f;
      (&local_F4)->y = -0.071f;
      (&local_F4)->z = -0.366f;
      local_E8.x = 22570;
      (&local_E8)->y = 47422;
      (&local_E8)->z = 50974;
      local_E4 = 18;
      break;
    case 3:
      local_F4.x = 0.1f;
      (&local_F4)->y = 0.0f;
      (&local_F4)->z = -0.036f;
      local_E8.x = 0;
      (&local_E8)->y = 0;
      (&local_E8)->z = 0;
      local_E4 = 18;
      break;
    case 4:
      local_F4.x = 0.563f;
      (&local_F4)->y = -0.18f;
      (&local_F4)->z = -0.033f;
      local_E8.x = 0;
      (&local_E8)->y = 0;
      (&local_E8)->z = 0x3000;
      local_E4 = 18;
      break;
    case 5:
      local_F4.x = 0.129f;
      (&local_F4)->y = -0.037f;
      (&local_F4)->z = -0.233f;
      local_E8.x = 18313;
      (&local_E8)->y = 46148;
      (&local_E8)->z = 56615;
      local_E4 = 18;
      break;
    case 6:
      local_F4.x = 0.143f;
      (&local_F4)->y = 0.024f;
      (&local_F4)->z = 0.01f;
      local_E8.x = 10920;
      (&local_E8)->y = 11829;
      (&local_E8)->z = 4550;
      local_E4 = 18;
      break;
    case 7:
      local_F4.x = -0.066f;
      (&local_F4)->y = -1.05f;
      (&local_F4)->z = -0.413f;
      local_E8.x = 5367;
      (&local_E8)->y = 0x2000;
      (&local_E8)->z = 2274;
      local_E4 = 18;
      break;
    case 8:
      local_F4.x = 0.135f;
      (&local_F4)->y = 0.0f;
      (&local_F4)->z = -0.384f;
      local_E8.x = 46789;
      (&local_E8)->y = 49970;
      (&local_E8)->z = 20929;
      local_E4 = 18;
      break;
    case 9:
      local_F4.x = 0.1f;
      (&local_F4)->y = 0.0f;
      (&local_F4)->z = -0.02f;
      local_E8.x = 0;
      (&local_E8)->y = 0;
      (&local_E8)->z = 0;
      local_E4 = 18;
      break;
  }
  if (Script_motnUtils(2, arg0)) {
    Script_motnUtils(1, arg0, 0);
  }
  SCNF_WaitFrames_0000(1);
  Script_motmToggleFlags(arg0, 2, 0x40);
  Script_motmGetCharaFlags(arg0, 0, 0);
  Script_PlayCharaMotion(arg0, arg2, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  SCNF_WaitFrames_0000(1);
  Script_fixoAttachEx(arg1, arg0, local_E4, &local_F4, &local_E8);
  Script_motmGetCharaFlags(arg0, 2, 0);
  if (Script_motnUtils(2, arg0)) {
    Script_motnUtils(1, arg0, 1);
  }
  return;
}

static void script_E42A(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t local_E4;
  int32_t local_E0;
  local_E0 = arg3;
  switch (local_E0) {
    case 0:
      local_F4.x = 0.066f;
      (&local_F4)->y = 0.0f;
      (&local_F4)->z = -0.016f;
      local_E8.x = 0xC000;
      (&local_E8)->y = 0;
      (&local_E8)->z = 0xD000;
      local_E4 = 18;
      break;
    case 1:
      local_F4.x = 0.071f;
      (&local_F4)->y = -0.9f;
      (&local_F4)->z = -0.197f;
      local_E8.x = 1820;
      (&local_E8)->y = -64350;
      (&local_E8)->z = 364;
      local_E4 = 18;
      break;
    case 2:
      local_F4.x = 0.147f;
      (&local_F4)->y = -0.86f;
      (&local_F4)->z = -0.35f;
      local_E8.x = 3913;
      (&local_E8)->y = 0;
      (&local_E8)->z = 910;
      local_E4 = 18;
      break;
    case 3:
      local_F4.x = -0.29f;
      (&local_F4)->y = -1.095f;
      (&local_F4)->z = -0.123f;
      local_E8.x = 43146;
      (&local_E8)->y = 47880;
      (&local_E8)->z = 19113;
      local_E4 = 18;
      break;
    case 4:
      local_F4.x = 0.113f;
      (&local_F4)->y = 0.0f;
      (&local_F4)->z = -0.035f;
      local_E8.x = 182;
      (&local_E8)->y = 62441;
      (&local_E8)->z = 65171;
      local_E4 = 18;
      break;
    case 5:
      local_F4.x = 0.088f;
      (&local_F4)->y = 0.036f;
      (&local_F4)->z = -0.048f;
      local_E8.x = 5279;
      (&local_E8)->y = 37523;
      (&local_E8)->z = 5825;
      local_E4 = 18;
      break;
    case 6:
      local_F4.x = 0.103f;
      (&local_F4)->y = 0.012f;
      (&local_F4)->z = 0.021f;
      local_E8.x = 59892;
      (&local_E8)->y = 3845;
      (&local_E8)->z = 364;
      local_E4 = 12;
      break;
    case 7:
      local_F4.x = 0.1136f;
      (&local_F4)->y = 0.0f;
      (&local_F4)->z = 0.013f;
      local_E8.x = 44540;
      (&local_E8)->y = 43679;
      (&local_E8)->z = 13666;
      local_E4 = 18;
      break;
    case 8:
      local_F4.x = 0.131f;
      (&local_F4)->y = -0.045f;
      (&local_F4)->z = -0.375f;
      local_E8.x = 18204;
      (&local_E8)->y = 47058;
      (&local_E8)->z = 58166;
      local_E4 = 18;
      break;
    case 9:
      local_F4.x = 0.07f;
      (&local_F4)->y = 0.01f;
      (&local_F4)->z = -0.02f;
      local_E8.x = 0x4000;
      (&local_E8)->y = 0xC000;
      (&local_E8)->z = 0xA000;
      local_E4 = 18;
      break;
    case 10:
      local_F4.x = 0.049f;
      (&local_F4)->y = 0.001f;
      (&local_F4)->z = -0.011f;
      local_E8.x = 64169;
      (&local_E8)->y = 0;
      (&local_E8)->z = 0;
      local_E4 = 18;
      break;
    case 11:
      local_F4.x = 0.1f;
      (&local_F4)->y = 0.0f;
      (&local_F4)->z = -0.02f;
      local_E8.x = 0;
      (&local_E8)->y = 0;
      (&local_E8)->z = 0;
      local_E4 = 18;
      break;
    case 12:
      local_F4.x = 0.114f;
      (&local_F4)->y = -0.018f;
      (&local_F4)->z = -0.055f;
      local_E8.x = 0x1000;
      (&local_E8)->y = 0;
      (&local_E8)->z = 0;
      local_E4 = 18;
      break;
    case 13:
      local_F4.x = 0.257f;
      (&local_F4)->y = -0.072f;
      (&local_F4)->z = 0.064f;
      local_E8.x = 0xF000;
      (&local_E8)->y = 0xA000;
      (&local_E8)->z = 0x2000;
      local_E4 = 18;
      break;
    case 14:
      local_F4.x = 0.061f;
      (&local_F4)->y = 0.015f;
      (&local_F4)->z = 0.026f;
      local_E8.x = 19842;
      (&local_E8)->y = 61986;
      (&local_E8)->z = 55977;
      local_E4 = 12;
      break;
    case 15:
      local_F4.x = 0.2f;
      (&local_F4)->y = 0.0f;
      (&local_F4)->z = 0.03f;
      local_E8.x = 0xC000;
      (&local_E8)->y = 30947;
      (&local_E8)->z = 0x4000;
      local_E4 = 12;
      break;
    case 16:
      local_F4.x = 0.089f;
      (&local_F4)->y = 0.016f;
      (&local_F4)->z = -0.017f;
      local_E8.x = 0x4000;
      (&local_E8)->y = 0;
      (&local_E8)->z = 0;
      local_E4 = 18;
      break;
    case 17:
      local_F4.x = 0.11f;
      (&local_F4)->y = 0.055f;
      (&local_F4)->z = -0.005f;
      local_E8.x = 47336;
      (&local_E8)->y = 18232;
      (&local_E8)->z = 59134;
      local_E4 = 18;
      break;
    case 18:
      local_F4.x = 0.132f;
      (&local_F4)->y = 0.002f;
      (&local_F4)->z = -0.019f;
      local_E8.x = 36416;
      (&local_E8)->y = 33701;
      (&local_E8)->z = 47303;
      local_E4 = 18;
      break;
    case 19:
      local_F4.x = 0.134f;
      (&local_F4)->y = -0.033f;
      (&local_F4)->z = -0.036f;
      local_E8.x = 0;
      (&local_E8)->y = 41859;
      (&local_E8)->z = 0;
      local_E4 = 18;
      break;
  }
  Script_motmToggleFlags(arg0, 2, 0x40);
  Script_motmGetCharaFlags(arg0, 0, 0);
  Script_PlayCharaMotion(arg0, arg2, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  SCNF_WaitFrames_0000(1);
  Script_fixoAttachEx(arg1, arg0, local_E4, &local_F4, &local_E8);
  Script_motmGetCharaFlags(arg0, 2, 0);
  return;
}

__attribute__((scn_string_bindings(0, data_11AAC)))
static void script_EAB8(void) {
  Script_sprintf("TUNED FreeTalk def\012");
  SCNT_Enqueue();
  return;
}

__attribute__((scn_string_bindings(0, data_11AC0)))
static void script_EAC7(void) {
  Script_sprintf("TUNED FreeTalk def\012");
  SCNT_Enqueue();
  return;
}

static void script_EAD6(void) {
  return;
}

static void script_EAD9(void) {
  return;
}

static void script_EADC(void) {
  return;
}

static void script_EADF(void) {
  return;
}

static void script_EAE2(void) {
  (void)((task_24C = 0) & 0x20);
  return;
}

static void script_EAEF(void) {
  task_240 = Script_freeTalkGetCurrentChara();
  return;
}

static void script_EAFB(void) {
  *(&task_240 + 1) = Script_GetFreeTalkStatus();
  return;
}

static void script_EB0A(void) {
  *(&task_240 + 2) = Script_freeTalkGetAndResetState();
  return;
}

void script_EB19(void) {
  int16_t local_FE;
  uint8_t unused_FC[2];
  int32_t local_F8;
  if (Script_ryoSleepUtils(0)) {
    Script_ryoSleepUtils(1);
  }
  task_20C = Script_GetCurrentEntryPoint();
  Script_PlayerStatsUtils(PS_IncrementSpotVisitCount, 52);
  Script_filmCheckOrSetTransitionUtil(0, 255, 0, 0, 0, 0, 0, 0, 0);
  Script_SetCurrentScrollEnabled(0);
  Script_GetSetWeatherSettings(WEATHER_SETTINGS_SET_DISABLE_WEATHER, 1);
  Script_WeatherUtils_0(WeatherDataUtil_ApplyCurrentWeatherOrEnableDateTimeRead, 0);
  Script_WeatherTaskUtil(InitTaskData);
  Script_WeatherUtils(WeatherScript_ApplyWccpClipSet1, 4.0939f, 2.129f, -0.0877f, 1.9737f, -0.1803f, -4.2329f, 4.0855f, -4.0192f, -0.3f, 2.7f);
  Script_WeatherTaskUtil(ToggleFogTask, 0);
  Script_WeatherTaskUtil(ToggleSceneLightingFlags, 0);
  Script_SetGlobalSceneSlotValue34(1, 1);
  Script_SetupLightFromTable(0);
  Script_ProcessFogFromTable(0);
  task_20D = 0;
  task_213 = 0;
  task_212 = 0;
  task_214 = 0;
  task_218[0] = 0;
  task_218[1] = 0;
  Script_LoadVoiceFile(1, "E1072");
  Script_SetFogFlags(2, 0);
  local_FE = Script_GetSceneID();
  Script_motiLoad(local_FE, SHEN_NAME('FREE'), "M_MOBJ.BIN");
  script_821B(CHARA_AKIR, (DSWKDoorEntry *)&data_10E84, (int32_t *)data_10EEC, (DoorResourceDescriptor *)&data_10F00, (MapObjectRecord *)&data_10F10, 1, 0);
  script_A257(CHARA_AKIR);
  SCNF_WaitFrames_0000(1);
  script_EAB8();
  Script_scenCreateTask(script_C43E, 0);
  Script_CycleManagerUtils(CYCLE_CMD_SETUP, "scene/01/D000", (int32_t)"CYCLEMAN.BIN.BIN", 0x4E525544);
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
  Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT, 0x534D414E, 1);
  while ((local_F8 = Script_CycleManagerUtils(CYCLE_CMD_GET_CYCLE_PTR_BY_CHARA_ID, 0x534D414E)) == 0) {
    SCNF_WaitFrames_0000(1);
  }
  while (Script_CycleManagerUtils(CYCLE_CMD_GET_NEXT_TASK_POINTER, local_F8) == 0) {
    SCNF_WaitFrames_0000(1);
  }
  Script_CycleManagerUtils(CYCLE_CMD_SET_CYCLE_MANAGER_ENABLED, 0);
  {
    Vector3 local_EC;
    local_EC.x = 2.1f;
    (&local_EC)->z = -3.4f;
    Script_EVT_SetPosition(CHARA_NAMS, 0x28000000, &local_EC);
  }
  {
    Vector3i local_EC;
    (&local_EC)->y = 0x8000;
    Script_EVT_SetRotation(CHARA_NAMS, MATRIX_XFORM_Y, &local_EC, 0);
  }
  Script_PlayCharaMotion(CHARA_NAMS, 4120, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_charaDisplay(CHARA_NAMS, CDM_Show);
  SCNF_WaitFrames_0000(1);
  SCNF_WaitFrames_0000(1);
  DSWK_DoorUtils(DOOR_CMD_SET_STATE_1);
  Script_TimedTransitionUtils(TIMED_TRANSITION_REGISTER, Script_GetSceneID(), 0x30303044, 23, (const void *)-1, 959, 6);
  Script_uiUtils(0, 54, 0);
  Script_scenCreateTask(script_EDB7, 0);
  Script_skyCtrlLuminance(1, -10, -10, -10);
  Script_ryoSleepUtils(2, 1);
  return;
}

static int32_t script_EDB7(void) {
  int8_t local_FF;
  int8_t local_FE;
  uint8_t unused_FC[2];
  Vector3 local_F0;
  Vector3 local_E4;
  Vector3i local_D8;
  local_FF = 0;
  local_FE = 0;
  local_F0.x = 1.1411f;
  (&local_F0)->y = 0.0f;
  (&local_F0)->z = -1.9665f;
  local_E4.x = 3.3411f;
  (&local_E4)->y = 0.0f;
  (&local_E4)->z = -0.6665f;
  script_CB8C(0);
  Script_seqEnterEvtCam(50, 0, AUTH_None);
  Script_cdttLoadScrUtils(1, 0);
  Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, 0x534D414E) & Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, 0x52494B41)) {
    Vector3 local_CC;
    Script_EVT_GetPosition(CHARA_AKIR, 4, &local_CC, ENTITY_XFORM_WORLD_SPACE);
    Script_SetLookPointDetails(CHARA_NAMS, 16, &local_CC, 2);
    Script_FaceLookAtTargetUtils(CHARA_NAMS, 2, &local_CC, 30);
  }
  if (Script_FloatUtils(ScriptMathOp_RandomIntBelow, 2)) {
    Script_ShowUIMessageFromID("E1072B055");
  } else {
    Script_ShowUIMessageFromID("E1072B054");
  }
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  SCNF_WaitFrames_0000(30);
  Script_shenClearFreeState(1);
  Script_ryoControl(RYO_RestoreControl);
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, 0x534D414E)) {
    Script_SetCharaLookAtPosition(CHARA_NAMS, -13, 0, 0);
    Script_FaceLookAtTargetUtils(CHARA_NAMS, 0);
  }
  Script_SetGameStateTo(AUTH_None);
  Script_sicnUtils(0, 1, 1);
  for (;;) {
    if ((&task_7C)->mode) {
      int32_t local_D4;
      local_D4 = task_7C.entry_idx;
      if (local_D4 == 0) {
        Script_ryoSleepUtils(2, 0);
        SCNF_WaitFrames_0000(60);
        Script_executeAicaCommand(2720, 1, 30);
        Script_executeAicaCommand(2720, 2, 30);
        Script_executeAicaCommand(2720, 3, 30);
        Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
        while (Script_filmCheckOrSetTransitionUtil(-1)) {
          SCNF_WaitFrames_0000(1);
        }
        Script_shenRequestGameModuleTransition(Script_GetSceneID(), SHEN_NAME('D000'), 23);
        goto L00EFA9;
      }
    }
    L00EFA9:;
    if (Script_IsPlayerAllowed(0)) {
      if (local_FE == 1) {
        Script_sicnUtils(0, 1, 1);
        local_FE = 0;
      }
      Script_mtwkGetSetRotation(CHARA_AKIR, &local_D8, 0);
      (&local_D8)->y = ((&local_D8)->y + 0x4000) & 0xFFFF;
      if (-((&local_D8)->y < 0x8000) & Script_SetTaskDataValue(CHARA_AKIR, &local_F0, &local_E4, 0)) {
        if (local_FF == 0) {
          SCNF_RequestIconNotification(0x100, 0, 0, 1);
          local_FF = 1;
        }
        if (Script_GetControllerInput(1) == 0x400) {
          SCNF_RequestIconNotification(0, 0, 0, 2);
          local_FF = 0;
          Script_sicnUtils(2, 1, 0);
          local_FE = 1;
          Script_ryoControl(PCCTL_DisablePlayerControl);
          Script_FaceEx(CHARA_AKIR, 1, 0, 2);
          Script_ResetHandMotion(CHARA_AKIR, -1, 0, 0);
          Script_ResetHandMotion(CHARA_AKIR, -1, 1, 0);
          Script_shenClearFreeState(0);
          script_D4F1();
          Script_ryoControl(RYO_RestoreControl);
          Script_FaceEx(CHARA_AKIR, 0, 0, 2);
          Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
          Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
          Script_shenClearFreeState(1);
          if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_EV_STOP_ACTIVE)) {
            Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_EV_STOP_ACTIVE, 0);
          }
        }
      } else {
        if (local_FF) {
          SCNF_RequestIconNotification(0, 0, 0, 2);
          local_FF = 0;
          goto L00F118;
        }
      }
      L00F118:;
    } else {
      if (local_FE == 0) {
        Script_sicnUtils(2, 1, 0);
        local_FE = 1;
      }
      if (local_FF) {
        SCNF_RequestIconNotification(0, 0, 0, 2);
        local_FF = 0;
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}
