#pragma once

#include "scenario.h"
#include "motion/motion.h"

static void script_0030(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_0932(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_0A25(int32_t arg0, int32_t arg1);
static void script_0B64(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_0D13(int32_t arg0, int32_t arg1, int32_t arg2);
static int8_t script_0E0C(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5, int32_t arg6);
static int8_t script_12FC(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_18C8(int32_t arg0, int32_t arg1, float arg2, float arg3, int32_t arg4, int32_t arg5);
static int32_t script_1B07(int32_t arg0, int32_t arg1, float arg2, float arg3, int32_t arg4, int32_t arg5, int32_t arg6);
static int32_t script_1D96(int32_t arg0, int32_t arg1, float arg2, float arg3, int32_t arg4, int32_t arg5, int32_t arg6);
static void script_20A3(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_22D1(int32_t arg0);
static void script_2370(float arg0);
static void script_23FF(float arg0);
static void script_248E(void);
static int32_t script_24C5(int32_t arg0, int32_t arg1);
static void script_25FC(int32_t arg0);
static void script_2643(int32_t arg0);
static void script_264D(int32_t arg0);
static void script_2657(void);
static void script_265E(int32_t arg0);
static void script_2739(int32_t arg0);
static void script_2814(int32_t arg0);
static void script_28EF(int32_t arg0, int32_t arg1);
static void script_29E5(int32_t arg0, int32_t arg1);
static int32_t script_2ADB(int32_t arg0, int32_t arg1);
static int32_t script_2C03(int32_t arg0, int32_t arg1);
static void script_2D34(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_2DB9(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_2E09(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_2E58(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_31EE(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_387C(int32_t arg0, float arg1, float arg2, float arg3);
static void script_38A4(int32_t arg0, float arg1, float arg2, float arg3);
static void script_38F8(int32_t arg0, int32_t arg1, int32_t arg2);
static int32_t script_3948(int32_t arg0);
static void script_39EF(int32_t arg0, int32_t arg1);
static int32_t script_3A12(int32_t arg0);
static void script_3A45(int32_t arg0, int32_t arg1);
static void script_3A68(int32_t arg0, int32_t arg1);
static void script_3AC7(int32_t arg0);
static void script_3ADC(void);
static void script_3AEB(void);
static void script_3AFA(void);
static void script_3AFD(void);
static void script_3B00(void);
static void script_3B03(void);
static void script_3B06(void);
static void script_3B13(void);
static void script_3B1F(void);
static int32_t script_3B2E(void);
static int32_t script_3B3D(int32_t arg0);
static void script_3B5B(float arg0);
static int32_t script_3B7A(int32_t arg0, int32_t arg1);
static void script_3BF2(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4);
static int32_t script_41A4(int32_t arg0);
static void script_4275(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_4285(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_4389(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_452B(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_468C(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_47ED(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_4A8B(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_4B59(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_4BC0(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_4C27(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_4C6E(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_4DA4(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_5665(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_5783(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_5E48(int32_t arg0, int32_t arg1);
static void script_5F1E(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_6E31(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_6E9D(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_7357(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_7A11(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_92F1(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_9A12(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_A19C(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_A274(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_A6EB(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_B67E(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_BAC8(int32_t arg0, DSWKDoorEntry *arg1, int32_t *arg2, DoorResourceDescriptor *arg3, MapObjectRecord *arg4, int32_t arg5, int32_t arg6);
static void script_BC54(DSWKDoorEntry *arg0, int32_t *arg1, DoorResourceDescriptor *arg2, MapObjectRecord *arg3, int32_t arg4);
static void script_BC88(void);
static void script_BCF1(int32_t arg0, int32_t arg1);
static void script_BD8E(int32_t arg0, int32_t arg1);
static void script_BDC8(void);
static void script_BE2B(void);
static void script_BE8E(int32_t arg0);
static void script_BEC5(int32_t arg0);
static void script_BEFC(int32_t arg0);
static void script_C13E(int32_t arg0);
static void script_C187(int32_t arg0);
static void script_C1D0(int32_t arg0, int32_t arg1);
static void script_C246(int32_t arg0);
static void script_C254(int32_t arg0);
static void script_C262(int32_t arg0);
static void script_C329(void);
static void script_C335(void);
static void script_C341(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5, int32_t arg6);
static int32_t script_C384(void);
static int8_t script_C495(int32_t arg0, int32_t arg1, int8_t arg2);
static void script_C52D(int32_t arg0);
static void script_C6FC(int32_t arg0);
static int32_t script_C72D(int32_t arg0, int32_t arg1, int32_t arg2, float arg3, float arg4, int32_t arg5, int32_t arg6);
static void script_CDF6(int32_t arg0, int32_t arg1);
static void script_CE28(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_CF98(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_D154(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_D308(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_D51E(int32_t arg0, int32_t arg1);
static void script_D5E8(int32_t arg0, int32_t arg1);
static void script_D649(int32_t arg0);
static void script_D7EE(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_D886(int32_t arg0, int32_t arg1);
static void script_D968(int32_t arg0);
static void script_D9BF(void);
static void script_DAF7(int32_t arg0);
static void script_DB03(int32_t arg0);
static void script_DB0F(int32_t arg0);
static int32_t script_DC02(int32_t arg0);
static void script_DC19(int32_t arg0);
static void script_DC5A(int32_t arg0);
static void script_DC9E(int32_t arg0);
static void script_DD1C(void);
static void script_DD9B(int32_t arg0);
static int32_t script_DDC7(int32_t arg0, int32_t arg1);
static int8_t script_DDFA(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_DF3C(int32_t arg0, int32_t arg1);
static void script_E105(int32_t arg0, int32_t arg1);
static void script_E2DB(int32_t arg0);
static void script_E2E5(void);
static void script_E309(void);
static void script_E440(int32_t arg0);
static int32_t script_E9F4(int32_t arg0);
static void script_EA53(int32_t arg0);
static int16_t script_EBF4(int8_t arg0, int8_t arg1);
static void script_EDDF(int32_t arg0, int32_t arg1);
static void script_EE2E(int32_t arg0, int32_t arg1);
static void script_EE81(int32_t arg0, int32_t arg1, int32_t arg2, float arg3);
static int32_t script_F0B8(void);
static int32_t script_F0E6(int32_t arg0, int32_t arg1);
static void script_F215(int32_t arg0);
static int8_t script_F478(int32_t arg0);
static void script_F86B(void);
static void script_1002C(void);
static void script_10563(void);
static void script_106A8(void);
static void script_1083C(void);
static int32_t script_108DC(void);
static void script_10DFA(void);
static int32_t script_10EBB(void);
static int32_t script_10EF4(int32_t arg0);
static void script_10F2B(int32_t arg0);
static void script_112A4(void);
static void script_113D9(void);
static void script_1144D(void);
static void script_1150B(void);
static void script_115E8(void);
static void script_116EC(void);
static void script_1192E(void);
static void script_11CA5(void);
void script_11DA8(void);
static int32_t script_11FF1(void);
static int32_t script_1205C(void);

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
static int32_t task_6C;
static int32_t task_70;
static int32_t task_74;
static int32_t task_78;
static int32_t task_7C;
static int32_t task_80;
static int32_t task_84;
static int32_t task_88;
static int32_t task_8C;
static int32_t task_90[4];
static int32_t task_A0;
static float task_A4;
static float task_A8;
static float task_AC;
static int8_t task_B0;
static int8_t task_B1;
static int8_t task_B2;
static int8_t task_B3;
static int8_t task_B4;
static int8_t task_B5;
static int8_t task_B6;
static int8_t task_B7;
static int8_t task_B8;
static uint8_t task_B9;
static uint16_t task_BA;
static int8_t task_BC;
static uint8_t task_BD;
static uint16_t task_BE;
static int32_t task_C0;
static int32_t task_C4;
static int32_t task_C8;
static int32_t task_CC;
static int32_t task_D0;
static int8_t task_D4;
static uint8_t task_D5;
static uint16_t task_D6;
static DSWKDoorEntry *task_D8;
static int32_t *task_DC;
static DoorResourceDescriptor *task_E0;
static MapObjectRecord *task_E4;
static DoorSlotState task_E8;
static int32_t task_F4;
static int32_t task_F8[10];
static int32_t task_120[10];
static int32_t task_148[10];
static int32_t task_170[10];
static int32_t task_198[10];
static int32_t task_1C0[10];
static int32_t task_1E8[10];
static int32_t task_210[10];
static int32_t task_238;
static int32_t task_23C;
static Vector3 task_240;
static int32_t task_24C;
static int32_t task_250;
static int32_t task_254;
static int8_t task_258;
static uint8_t task_259;
static uint16_t task_25A;
static int8_t task_25C;
static uint8_t task_25D;
static uint16_t task_25E;
static int32_t task_260;
static int32_t task_264;
static int8_t task_268;
static uint8_t task_269;
static uint16_t task_26A;
static Vector3 task_26C;
static int32_t task_278;
static int32_t task_27C;
static int32_t task_280;
static int32_t task_284;
static int32_t task_288;
static int32_t task_28C;
static int32_t task_290;
static int32_t task_294;
static int32_t task_298;
static int32_t task_29C;
static int32_t task_2A0;
static int32_t task_2A4;
static int32_t task_2A8;
static int32_t task_2AC;
static int32_t task_2B0;
static int32_t task_2B4;
static int32_t task_2B8;
static int32_t task_2BC;
static int32_t task_2C0;
static int32_t task_2C4;
static int32_t task_2C8;
static int32_t task_2CC;
static int32_t task_2D0;
static int32_t task_2D4;
static int8_t task_2D8;
static uint8_t task_2D9;
static uint16_t task_2DA;
static int32_t task_2DC;
static const Vector3 data_12084 = {.x = 1.0f, .y = 1.0f, .z = 1.0f};
static const int32_t data_12090[4] = {16777215, 16777215, 16777215, 16777215};
static const float data_120A0[3] = {20.0f, 800.0f, 1.0f};
static const int32_t data_120AC = -1;
static const char data_120B0[10] __attribute__((scn_string_literal)) = "Kaidan : ";
static const char data_120BA[32] __attribute__((scn_string_literal)) = "========== initializing kaidan\012";
static const char data_120DA[48] __attribute__((scn_string_literal)) = "please ENDOFKDATA use can't start shuki kaidan\012";
static const char data_1210A[] __attribute__((scn_string_literal)) = "Kaidan Function Error 1\n";
static const char data_12123[] __attribute__((scn_string_literal)) = "Kaidan Function Error 2\n";
static const char data_1213C[] __attribute__((scn_string_literal)) = "Kaidan Data Error\n";
static const char data_1214F[10] __attribute__((scn_string_literal)) = "Kaidan : ";
static const char data_12159[28] __attribute__((scn_string_literal)) = "wait ... shuki shuki shuki\012";
static const char data_12175[10] __attribute__((scn_string_literal)) = "Kaidan : ";
static const char data_1217F[24] __attribute__((scn_string_literal)) = "KAIDAN : Warning coli!\012";
static const char data_12197[24] __attribute__((scn_string_literal)) = "KAIDAN : Warning coli!\012";
static const char data_121AF[24] __attribute__((scn_string_literal)) = "KAIDAN : Warning coli!\012";
static const char data_121C7[24] __attribute__((scn_string_literal)) = "KAIDAN : Warning coli!\012";
static const char data_121DF[24] __attribute__((scn_string_literal)) = "KAIDAN : Warning coli!\012";
static const char data_121F7[24] __attribute__((scn_string_literal)) = "KAIDAN : Warning setY!\012";
static const char data_1220F[24] __attribute__((scn_string_literal)) = "KAIDAN : Warning setY!\012";
static const char data_12227[29] = "Kaidan : \000\000\000\000,\004\023\004-\004/\0042\0040\0041\004\207\003";
static const char data_12244[20] __attribute__((scn_string_literal)) = "TUNED FreeTalk def\012";
static const char data_12258[20] __attribute__((scn_string_literal)) = "TUNED FreeTalk def\012";
static DoorMotionSelectorBlock data_1226C[38] = {
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
static ScnDoorMotionDescriptor data_1285C[72] = {
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
static uint32_t data_1327C = 0xFFFFFFFF;
typedef struct Row_13280 { float w0; float w1; float w2; float w3; int32_t w4; } Row_13280;
static const Row_13280 data_13280[7] = {
  {0.0f, 0.0f, 0.0f, 0.0f, -1},
  {0.0f, 0.0f, 0.0f, 0.0f, -1},
  {0.0f, 0.0f, 0.0f, 0.0f, 1534},
  {26.0f, 55.0f, 45.0f, 75.0f, 1555},
  {61.0f, 146.0f, 45.0f, 50.0f, 1554},
  {59.0f, 124.0f, 45.0f, 70.0f, 1543},
  {33.0f, 49.0f, 45.0f, 50.0f, 1543}
};
static const float data_1330C[4] = {33.0f, 49.0f, 50.0f, 50.0f};
static void (*data_1331C)(int32_t, int32_t, int32_t) = script_4275;
static void (*const data_13320)(int32_t, int32_t, int32_t, int32_t) = script_4285;
static void (*const data_13324)(int32_t, int32_t, int32_t, int32_t) = script_4285;
static void (*const data_13328)(int32_t, int32_t, int32_t, int32_t) = script_4285;
static void (*const data_1332C)(int32_t, int32_t, int32_t, int32_t) = script_4285;
static void (*const data_13330)(int32_t, int32_t, int32_t, int32_t) = script_4285;
static void (*const data_13334)(int32_t, int32_t, int32_t, int32_t) = script_4285;
static void (*const data_13338)(int32_t, int32_t, int32_t, int32_t) = script_4285;
static void (*const data_1333C)(int32_t, int32_t, int32_t, int32_t) = script_4285;
static void (*const data_13340)(int32_t, int32_t, int32_t, int32_t) = script_452B;
static void (*const data_13344)(int32_t, int32_t, int32_t, int32_t) = script_468C;
static void (*const data_13348)(int32_t, int32_t, int32_t, int32_t) = script_4B59;
static void (*const data_1334C)(int32_t, int32_t, int32_t, int32_t) = script_4BC0;
static void (*const data_13350)(int32_t, int32_t, int32_t, int32_t) = script_4389;
static void (*const data_13354)(int32_t, int32_t, int32_t, int32_t) = script_4389;
static void (*const data_13358)(int32_t, int32_t, int32_t, int32_t) = script_47ED;
static void (*const data_1335C)(int32_t, int32_t, int32_t, int32_t) = script_4A8B;
static void (*const data_13360)(int32_t, int32_t, int32_t, int32_t) = script_4C27;
static void (*const data_13364)(int32_t, int32_t, int32_t, int32_t) = script_452B;
static void (*const data_13368)(int32_t, int32_t, int32_t, int32_t) = script_468C;
static void (*const data_1336C)(int32_t, int32_t, int32_t, int32_t) = script_47ED;
static void (*const data_13370)(int32_t, int32_t, int32_t, int32_t) = script_452B;
static void (*const data_13374)(int32_t, int32_t, int32_t, int32_t) = script_468C;
static void (*const data_13378)(int32_t, int32_t, int32_t, int32_t) = script_47ED;
static void (*const data_1337C)(int32_t, int32_t, int32_t, int32_t) = script_4C6E;
static void (*const data_13380)(int32_t, int32_t, int32_t, int32_t) = script_4C6E;
static void (*const data_13384)(int32_t, int32_t, int32_t, int32_t) = script_4C6E;
static void (*const data_13388)(int32_t, int32_t, int32_t, int32_t) = script_4C6E;
static void (*const data_1338C)(int32_t, int32_t, int32_t, int32_t) = script_4DA4;
static void (*const data_13390)(int32_t, int32_t, int32_t, int32_t) = script_5665;
static void (*const data_13394)(int32_t, int32_t, int32_t, int32_t) = script_4285;
static void (*const data_13398)(int32_t, int32_t, int32_t, int32_t) = script_4285;
static void (*const data_1339C)(int32_t, int32_t, int32_t, int32_t) = script_4285;
static void (*const data_133A0)(int32_t, int32_t, int32_t, int32_t) = script_4285;
static void (*const data_133A4)(int32_t, int32_t, int32_t, int32_t) = script_4285;
static void (*const data_133A8)(int32_t, int32_t, int32_t, int32_t) = script_4285;
static void (*const data_133AC)(int32_t, int32_t, int32_t, int32_t) = script_4285;
static void (*const data_133B0)(int32_t, int32_t, int32_t, int32_t) = script_4285;
static const char data_133B4[46] __attribute__((scn_string_literal)) = "Door Index = %d : call_type = %d : type = %d\012";
static const char data_133E2[43] __attribute__((scn_string_literal)) = "Door Open Motion = DEB_AKESIME_FUSUMA_L_F\012";
static const char data_1340D[43] __attribute__((scn_string_literal)) = "Door Open Motion = DEB_AKESIME_FUSUMA_R_F\012";
static const char data_13438[42] __attribute__((scn_string_literal)) = "Door Open Motion = DEB_HIKISIME_DOOR_L_F\012";
static const char data_13462[42] __attribute__((scn_string_literal)) = "Door Open Motion = DEB_HIKISIME_DOOR_R_F\012";
static const char data_1348C[41] __attribute__((scn_string_literal)) = "Door Open Motion = DEB_OSISIME_DOOR_L_F\012";
static const char data_134B5[41] __attribute__((scn_string_literal)) = "Door Open Motion = DEB_OSISIME_DOOR_R_F\012";
static const char data_134DE[43] __attribute__((scn_string_literal)) = "Door Open Motion = GAK_AKESIME_FUSUMA_L_F\012";
static const char data_13509[43] __attribute__((scn_string_literal)) = "Door Open Motion = GAK_AKESIME_FUSUMA_R_F\012";
static const char data_13534[42] __attribute__((scn_string_literal)) = "Door Open Motion = GAK_HIKISIME_DOOR_L_F\012";
static const char data_1355E[42] __attribute__((scn_string_literal)) = "Door Open Motion = GAK_HIKISIME_DOOR_R_F\012";
static const char data_13588[41] __attribute__((scn_string_literal)) = "Door Open Motion = GAK_OSISIME_DOOR_L_F\012";
static const char data_135B1[41] __attribute__((scn_string_literal)) = "Door Open Motion = GAK_OSISIME_DOOR_R_F\012";
static const char data_135DA[43] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_AKESIME_FUSUMA_L_F\012";
static const char data_13605[43] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_AKESIME_FUSUMA_R_F\012";
static const char data_13630[42] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_HIKISIME_DOOR_L_F\012";
static const char data_1365A[42] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_HIKISIME_DOOR_R_F\012";
static const char data_13684[41] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_OSISIME_DOOR_L_F\012";
static const char data_136AD[41] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_OSISIME_DOOR_R_F\012";
static const char data_136D6[43] __attribute__((scn_string_literal)) = "Door Open Motion = KOD_AKESIME_FUSUMA_L_F\012";
static const char data_13701[43] __attribute__((scn_string_literal)) = "Door Open Motion = KOD_AKESIME_FUSUMA_R_F\012";
static const char data_1372C[42] __attribute__((scn_string_literal)) = "Door Open Motion = KOD_HIKISIME_DOOR_L_F\012";
static const char data_13756[42] __attribute__((scn_string_literal)) = "Door Open Motion = KOD_HIKISIME_DOOR_R_F\012";
static const char data_13780[41] __attribute__((scn_string_literal)) = "Door Open Motion = KOD_OSISIME_DOOR_L_F\012";
static const char data_137A9[41] __attribute__((scn_string_literal)) = "Door Open Motion = KOD_OSISIME_DOOR_R_F\012";
static const char data_137D2[43] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_AKESIME_FUSUMA_L_F\012";
static const char data_137FD[43] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_AKESIME_FUSUMA_R_F\012";
static const char data_13828[42] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_HIKISIME_DOOR_L_F\012";
static const char data_13852[42] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_HIKISIME_DOOR_R_F\012";
static const char data_1387C[41] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_OSISIME_DOOR_L_F\012";
static const char data_138A5[41] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_OSISIME_DOOR_R_F\012";
static const char data_138CE[43] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_AKESIME_FUSUMA_L_F\012";
static const char data_138F9[43] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_AKESIME_FUSUMA_R_F\012";
static const char data_13924[42] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_HIKISIME_DOOR_L_F\012";
static const char data_1394E[42] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_HIKISIME_DOOR_R_F\012";
static const char data_13978[41] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_OSISIME_DOOR_L_F\012";
static const char data_139A1[41] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_OSISIME_DOOR_R_F\012";
static const char data_139CA[43] __attribute__((scn_string_literal)) = "Door Open Motion = SIN_AKESIME_FUSUMA_L_F\012";
static const char data_139F5[43] __attribute__((scn_string_literal)) = "Door Open Motion = SIN_AKESIME_FUSUMA_R_F\012";
static const char data_13A20[42] __attribute__((scn_string_literal)) = "Door Open Motion = SIN_HIKISIME_DOOR_L_F\012";
static const char data_13A4A[42] __attribute__((scn_string_literal)) = "Door Open Motion = SIN_HIKISIME_DOOR_R_F\012";
static const char data_13A74[41] __attribute__((scn_string_literal)) = "Door Open Motion = SIN_OSISIME_DOOR_L_F\012";
static const char data_13A9D[41] __attribute__((scn_string_literal)) = "Door Open Motion = SIN_OSISIME_DOOR_R_F\012";
static const char data_13AC6[43] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_AKESIME_FUSUMA_L_F\012";
static const char data_13AF1[43] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_AKESIME_FUSUMA_R_F\012";
static const char data_13B1C[42] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_HIKISIME_DOOR_L_F\012";
static const char data_13B46[42] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_HIKISIME_DOOR_R_F\012";
static const char data_13B70[41] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_OSISIME_DOOR_L_F\012";
static const char data_13B99[41] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_OSISIME_DOOR_R_F\012";
static const char data_13BC2[43] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_AKESIME_FUSUMA_L_F\012";
static const char data_13BED[43] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_AKESIME_FUSUMA_R_F\012";
static const char data_13C18[42] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_HIKISIME_DOOR_L_F\012";
static const char data_13C42[42] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_HIKISIME_DOOR_R_F\012";
static const char data_13C6C[41] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_OSISIME_DOOR_L_F\012";
static const char data_13C95[41] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_OSISIME_DOOR_R_F\012";
static const char data_13CBE[44] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_OSISIME_DOOR_AP_L_F\012";
static const char data_13CEA[44] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_OSISIME_DOOR_AP_L_F\012";
static const char data_13D16[44] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_OSISIME_DOOR_AP_L_F\012";
static const char data_13D42[44] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_OSISIME_DOOR_AP_L_F\012";
static const char data_13D6E[44] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_OSISIME_DOOR_AP_L_F\012";
static const char data_13D9A[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_13DB6[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_13DD2[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_13DEE[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_13E0A[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_13E26[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_13E42[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_13E5E[13] __attribute__((scn_string_literal)) = "open-move!!\012";
static const char data_13E6B[11] __attribute__((scn_string_literal)) = "M_MDOR.BIN";
static const char data_13E76[] __attribute__((scn_string_literal)) = "This Charactor Work is NULL!!\n";
static const char data_13E95[22] __attribute__((scn_string_literal)) = "手動ドア開け番号指定\012";
static const char data_13EAB[34] __attribute__((scn_string_literal)) = "無限ループになりそうなので抜ける\012";
static const char data_13ECD[] __attribute__((scn_string_literal)) = "Don't make BOX Buffer!!\n";
static const char data_13EE6[] __attribute__((scn_string_literal)) = "Don't make BOX Buffer!!\n";
static const char data_13EFF[15] __attribute__((scn_string_literal)) = "Door System : ";
static const char data_13F0E[15] __attribute__((scn_string_literal)) = "Door System : ";
static const char data_13F1D[18] __attribute__((scn_string_literal)) = "Door Box is ON!!\012";
static const char data_13F2F[19] __attribute__((scn_string_literal)) = "Door Box is OFF!!\012";
static const char data_13F42[18] __attribute__((scn_string_literal)) = "Door Box is ON!!\012";
static const char data_13F54[20] __attribute__((scn_string_literal)) = "Door Box is OFF!!\012";
static const DSWKDoorEntry data_13F68 = {.slot = 0, .flags = 0, .ctrl_mask_pair = 0x640064U, .coarse_metric = 0.0f, .selection_metric = 0.0f, .view_ang_delta = 0, .group = 0, .global1 = 0xFFFFFFFFU, .global2 = 0xFFFFFFFFU, .global3 = 0xFFFFFFFFU, .sndCommandId = 0xFFFFFFFFU, .global5 = 0xFFFFFFFFU, .global6 = 0xFFFFFFFFU};
static const int32_t data_13F9C = -1;
static const uint8_t data_13FA0[4] = {0};
static const int32_t data_13FA4 = -1;
static char data_13FA8[] = "DOOR";
static const uint8_t data_13FAD[3] = {0};
static char *const data_13FB0 = data_13FA8;
static const int32_t data_13FB4[3] = {15, 1180331, 1245867};
static const MapObjectRecord data_13FC0 = {.kind = 2, .resourceIndex = 0, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-0.92f, 0.0f, 1.76f}, .yawFixed16 = 32768};
static const uint32_t data_13FE4 = 380;
static const int32_t data_13FE8[6] = {1, 390, 0, 1018, 0, -1};
static const uint32_t data_14000 = 510;
static const int32_t data_14004[6] = {1, 525, 0, 1020, 0, -1};
static const uint32_t data_1401C = 150;
static const int32_t data_14020[6] = {1, 160, 0, 1019, 0, -1};
static const uint32_t data_14038 = 530;
static const int32_t data_1403C[6] = {1, 970, 0, 1014, 0, -1};
static const uint32_t data_14054 = 911;
static const int32_t data_14058[6] = {1, 155, 0, 1015, 0, -1};
static const char data_14070[7] __attribute__((scn_string_literal)) = "FREE02";
static const char data_14077[7] __attribute__((scn_string_literal)) = "FREE03";
static const char data_1407E[7] __attribute__((scn_string_literal)) = "FREE04";
static const char data_14085[7] __attribute__((scn_string_literal)) = "FREE01";
static uint8_t data_1408C[4] = {0};
static const uint32_t data_14090[3] = {0x78626B64U, 0x71C70000U, 0};
static const float data_1409C[5] = {0.2f, 0.1f, 0.0f, 0.0f, 0.0f};
static const uint32_t data_140B0[3] = {0x32687364U, 0x71C78000U, 0};
static const float data_140BC[4] = {0.035f, 0.2f, 0.0f, 0.0f};
static const int32_t data_140CC[2] = {-65536, 100};
static const uint32_t data_140D4 = 0x71C7C000U;
static const float data_140D8[5] = {1.52f, 0.67f, -0.85f, 0.0f, 0.0f};
static const int32_t data_140EC[2] = {-65536, 101};
static const uint32_t data_140F4 = 0x71C7C000U;
static const float data_140F8[5] = {1.52f, 0.67f, -1.5f, 0.0f, 0.0f};
static const int32_t data_1410C[2] = {-65536, 102};
static const uint32_t data_14114 = 0x71C7C000U;
static const float data_14118[5] = {1.52f, 1.3f, -1.4f, 0.0f, 0.0f};
static const int32_t data_1412C[2] = {-65536, 103};
static const uint32_t data_14134 = 0x71C74000U;
static const float data_14138[5] = {-2.17f, 1.2f, 1.04f, 0.0f, 0.0f};
static const int32_t data_1414C = 589824;
static const uint32_t data_14150[5] = {0x72736164U, 0x71C7C000U, 0, 0, 0};
static const int32_t data_14164[3] = {62, 0, 589824};
static const uint32_t data_14170[5] = {0x31706164U, 0x71C78000U, 0, 0, 0};
static const int32_t data_14184[3] = {65, 0, 589824};
static const uint32_t data_14190[5] = {0x70696164U, 0x71C7C000U, 0, 0, 0};
static const int32_t data_141A4[3] = {57, 0, 589824};
static const uint32_t data_141B0[5] = {0x6F686364U, 0x71C7A38EU, 0, 0, 0};
static const int32_t data_141C4[3] = {63, 5, 589824};
static const uint32_t data_141D0[5] = {0x72686364U, 0x71C78000U, 0, 0, 0};
static const int32_t data_141E4[3] = {64, 5, 65535};
static uint8_t data_141F0[12] = {0};
typedef struct Row_141FC { int32_t w0; uint32_t w1; uint32_t w2; uint32_t w3; uint32_t w4; int32_t w5; } Row_141FC;
static const Row_141FC data_141FC[2] = {
  {1181097, 0x72736164U, 0x70696164U, 0x6F686364U, 0x72686364U, -1},
  {1246633, 0x0U, 0x0U, 0x0U, 0x31706164U, -1}
};
static const int32_t data_1422C = -1;
static char data_14230[] = "dasrZ";
static const uint8_t data_14236[16] = {
  0x00, 0x00, 0x64, 0x61, 0x70, 0x31, 0x78, 0x00, 0x00, 0x00, 0x64, 0x61, 0x69, 0x70, 0x5A, 0x00
};
static const uint8_t data_14246[16] = {
  0x00, 0x00, 0x64, 0x63, 0x68, 0x6F, 0x78, 0x00, 0x00, 0x00, 0x64, 0x63, 0x68, 0x72, 0x78, 0x00
};
static const uint8_t data_14256[6] = {
  0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF
};
static const char data_1425C[5] __attribute__((scn_string_literal)) = "SY11";
static const char data_14261[9] __attribute__((scn_string_literal)) = "SY11A006";
static const char data_1426A[9] __attribute__((scn_string_literal)) = "SY11A007";
static const char data_14273[9] __attribute__((scn_string_literal)) = "SY11A008";
static const char data_1427C[9] __attribute__((scn_string_literal)) = "SY11A002";
static const char data_14285[9] __attribute__((scn_string_literal)) = "SY11A003";
static const char data_1428E[9] __attribute__((scn_string_literal)) = "SY11A004";
static const char data_14297[9] __attribute__((scn_string_literal)) = "SY11A005";
static const char data_142A0[9] __attribute__((scn_string_literal)) = "SY11A001";
static const char data_142A9[9] __attribute__((scn_string_literal)) = "SY11A009";
static const char data_142B2[9] __attribute__((scn_string_literal)) = "SY11A010";
static const char data_142BB[9] __attribute__((scn_string_literal)) = "SY11A011";
static const char data_142C4[9] __attribute__((scn_string_literal)) = "SY11A009";
static const char data_142CD[5] __attribute__((scn_string_literal)) = "SY11";
static const char data_142D2[5] __attribute__((scn_string_literal)) = "SY11";
static const char data_142D7[9] __attribute__((scn_string_literal)) = "SY11A001";
static const char data_142E0[5] __attribute__((scn_string_literal)) = "SY11";
static const char data_142E5[5] __attribute__((scn_string_literal)) = "SY11";
static const char data_142EA[9] __attribute__((scn_string_literal)) = "SY11A009";
static const char data_142F3[9] __attribute__((scn_string_literal)) = "SY11A010";
static const char data_142FC[9] __attribute__((scn_string_literal)) = "SY11A011";
static const char data_14305[7] __attribute__((scn_string_literal)) = "SY11";
static const int8_t data_1430C[16] = {12, 17, 22, 10, 19, 18, 11, 8, 4, 4, 17, 7, 6, 5, 4, 4};
static const int8_t data_1431C[16] = {12, 1, 2, 1, 3, 2, 2, 8, 4, 4, 17, 7, 6, 5, 4, 4};
static const uint32_t data_1432C = 0xA800A7;
static const int32_t data_14330[5] = {8847529, 8978568, 15073509, 15204583, 15335657};
static const uint32_t data_14344 = 0x440043;
static const int32_t data_14348[8] = {4587589, 4718663, 4849737, 4980811, 5111885, 5242959, 5374033, 85};
static const uint32_t data_14368 = 0x5C005B;
static const int32_t data_1436C[10] = {6160477, 6291551, 6422625, 6553699, 6684773, 6815847, 6946921, 7077995, 7209069, 7340143};
static const uint32_t data_14394 = 0x740073;
static const int32_t data_14398[4] = {7798902, 7929976, 8061050, 8257660};
static const uint32_t data_143A8 = 0x720071;
static const int32_t data_143AC[9] = {10223750, 10354845, 10485919, 10616993, 10748067, 10879141, 11206826, 5767255, 180};
static const uint32_t data_143D0 = 0x810080;
static const int32_t data_143D4[8] = {8585346, 11403437, 11534511, 11665585, 11862195, 11993270, 12452024, 8650879};
static const uint32_t data_143F4 = 0xB900BC;
static const int32_t data_143F8[5] = {12255418, 13238473, 13500621, 12517579, 200};
static const uint32_t data_1440C = 0x5A0059;
static const int32_t data_14410[3] = {5636268, 14221445, 5505107};
static const uint32_t data_1441C = 0x7D0075;
static const int32_t data_14420 = 12386442;
static const uint32_t data_14424 = 0xC600C5;
static const int32_t data_14428 = 13369543;
static const uint32_t data_1442C = 0x8C008B;
static const int32_t data_14430[8] = {9306253, 9437327, 9633937, 148, 9961623, 10092697, 9568411, 149};
static const uint32_t data_14450 = 0xD700D6;
static const int32_t data_14454[3] = {14287064, 14876891, 228};
static const uint32_t data_14460 = 0xDC00D3;
static const int32_t data_14464[2] = {13893845, 14549213};
static const uint32_t data_1446C = 0xC100C0;
static const int32_t data_14470[2] = {12779714, 196};
static const uint32_t data_14478 = 0xD000CF;
static const int32_t data_1447C = 13762769;
static const uint32_t data_14480 = 0xE000DF;
static const int32_t data_14484 = 14811361;
static const char data_14488[169] = "%GACR016G.MT5\000%GACK66BG.MT5\000%GACK66RG.MT5\000%GACR0BNG.MT5\000%GACR0B2G.MT5\000%GACR0B3G.MT5\000%SPBK6REG.MT5\000%SPBK6BRG.MT5\000%SPBK6YEG.MT5\000%SAIK6WHG.MT5\000%SAIK6BKG.MT5\000%SAIK6REG.MT5";
static const char data_14531[315] = "%GACIAK1G.MT5\000%GACIAK2G.MT5\000%GACOJK1G.MT5\000%GACOJK2G.MT5\000%GACOSA1G.MT5\000%GACOSA2G.MT5\000%GACOLA1G.MT5\000%GACOLA2G.MT5\000%GACOPA1G.MT5\000%GACOPA2G.MT5\000%GACGWF1G.MT5\000%GACGWF2G.MT5\000%GACGJF1G.MT5\000%GACGJF2G.MT5\000%GACIKA1G.MT5\000%GACIKA2G.MT5\000%GACM401G.MT5\000%GACIAA1G.MT5\000%GACIAA2G.MT5\000D\000D\000D\000D\000D\000D\000D\000D\000D\000D\000%GACGJJ1G.MT5\000%GACGJJ2G.MT5";
static const char data_1466C[309] = "%GACIAKAG.MT5\000%GACIAKBG.MT5\000%GACGJKAG.MT5\000%GACGJKBG.MT5\000%GACOSAAG.MT5\000%GACOSABG.MT5\000%GACOLAAG.MT5\000%GACOLABG.MT5\000%GACOPAAG.MT5\000%GACOPABG.MT5\000%GACIWLAG.MT5\000%GACIWLBG.MT5\000%GACGJFAG.MT5\000%GACGJFBG.MT5\000%GACIKAAG.MT5\000%GACIKABG.MT5\000%GACGSNAG.MT5\000%GACGSNBG.MT5\000%GACILIAG.MT5\000%GACILIBG.MT5\000%GACIDUAG.MT5\000%GACIDUBG.MT5";
static const char data_147A1[167] = "%GACRSO1G.MT5\000%GACRSO2G.MT5\000%GACRTL1G.MT5\000%GACRNUCG.MT5\000%GACRBAKG.MT5\000%GACRBING.MT5\000%GACRFNGG.MT5\000%GACRAMYG.MT5\000%GACRESPG.MT5\000%GACREGGG.MT5\000D\000D\000D\000D\000D\000D\000%GACRFNNG.MT5";
static const char data_14848[365] = "%GACRNT1G.MT5\000%GACRNT2G.MT5\000%GACRLOPG.MT5\000%GACRMYUG.MT5\000%GACRCKNG.MT5\000%GACRALFG.MT5\000%GACRRISG.MT5\000%GACRARXG.MT5\000%GACR0OPG.MT5\000%GACR0O2G.MT5\000%GACRPOPG.MT5\000%GACRCOBG.MT5\000%GACRIDAG.MT5\000%GACRSHPG.MT5\000%GACRMOBG.MT5\000%GACRROBG.MT5\000%GACK6TZG.MT5\000%GACRPANG.MT5\000%GACK6DYG.MT5\000D\000D\000D\000D\000D\000D\000D\000%GACRAXXG.MT5\000D\000D\000D\000D\000D\000D\000D\000%GACRRBBG.MT5\000%GACK6TSG.MT5\000%GACRPPNG.MT5\000%GACK6DSG.MT5";
static const char data_149B5[505] = "%GACK6HHG.MT5\000%GACK6HYG.MT5\000%GACK6HBG.MT5\000%GACK6HGG.MT5\000%GACH5BKG.MT5\000%KAB02FSG.MT5\000%GACO1K2G.MT5\000%GACO1K3G.MT5\000%GACO1K7G.MT5\000%GACO1K8G.MT5\000%GACO1K9G.MT5\000%GACO1K1G.MT5\000%GACO1K4G.MT5\000%GACO1K5G.MT5\000%GACO1K6G.MT5\000%GACS5BSG.MT5\000%GACK6HOG.MT5\000%GACK6HKG.MT5\000%GACK6XHG.MT5\000%GACK6XYG.MT5\000%GACK6XBG.MT5\000%GACK6XGG.MT5\000%GACK6BKG.MT5\000%KABK6FSG.MT5\000%GACOKK2G.MT5\000%GACOKK3G.MT5\000%GACOKK7G.MT5\000%GACOKK8G.MT5\000%GACOKK9G.MT5\000%GACOKK1G.MT5\000%GACOKK4G.MT5\000%GACOKK5G.MT5\000%GACOKK6G.MT5\000%GACS5BBG.MT5\000%GACK6XOG.MT5\000%GACK6XKG.MT5";
static const char data_14BAE[211] = "%GACH5TKG.MT5\000%GACGTRXG.MT5\000%GACO1T1G.MT5\000%GACO1T2G.MT5\000%GACO1KOG.MT5\000%GACO1KBG.MT5\000%GACO1KJG.MT5\000%GACH5UKG.MT5\000%GACH5IKG.MT5\000%GACO1FOG.MT5\000%GACGSHEG.MT5\000%GACH5TTG.MT5\000%GACGTXXG.MT5\000%GACOTT1G.MT5\000%GACOTT2G.MT5";
static const char data_14C81[113] = "%YKW0201G.MT5\000%YWT0201G.MT5\000%GACRBONG.MT5\000%GACRRENG.MT5\000%GACK6SPG.MT5\000%GACG360G.MT5\000%GACRDR1G.MT5\000%GACRDR2G.MT5";
static const char data_14CF2[77] = "%GACRSSCG.MT5\000%GACRMETG.MT5\000%GACRGBIG.MT5\000%GACH5TRG.MT5\000D\000D\000D\000%GACH5RRG.MT5";
static const char data_14D3F[57] = "%GACS5FRG.MT5\000%GACS5FBG.MT5\000%GACO1KUG.MT5\000%GACO1FEG.MT5";
static const char data_14D78[239] = "%GACODG1G.MT5\000%GACODG2G.MT5\000%GACODG3G.MT5\000%GACOCT2G.MT5\000%GACOCATG.MT5\000%GACO1SHG.MT5\000%GACOKSHG.MT5\000%GACO1IRG.MT5\000%GACOKIRG.MT5\000%GACO1HAG.MT5\000%GACOHA2G.MT5\000%GACOHA3G.MT5\000%GACOHA4G.MT5\000%GACO1YGG.MT5\000%GACO1USG.MT5\000%GACOOSHG.MT5\000%GACOOIRG.MT5";
static const char data_14E67[99] = "%GACM400G.MT5\000%GACGSPHG.MT5\000%GACS5HGG.MT5\000%GACGDT1G.MT5\000%GACS5EQG.MT5\000%SYOGSH0G.MT5\000%SYOGHG0G.MT5";
static const char data_14ECA[85] = "%GACGCRYG.MT5\000%GACGBIRG.MT5\000%GACH5SLG.MT5\000%GACH5JKG.MT5\000%SYOGMD1G.MT5\000%SYOGMD5G.MT5";
static const char data_14F1F[71] = "%GACS5N1G.MT5\000%GACS5N2G.MT5\000%GACS5N3G.MT5\000%GACS5N4G.MT5\000%GACS5N5G.MT5";
static const char data_14F66[57] = "%GACGXBNG.MT5\000%GACGYBNG.MT5\000%GACGABNG.MT5\000%GACGBBNG.MT5";
static const char data_14F9F[57] = "%GACRMEGG.MT5\000%GACR0GGG.MT5\000%GACRSATG.MT5\000%GACRCASG.MT5";
static const int32_t data_14FD8[57] = {
  0, 0, -6005, 0, 4731, -6188, 0, 0, 0, 0, 0, -2727, 0, 0, -9097, 0, 0, -5457, 0, 0, 0, 0, 0, -8734, 0, -909, -7277, 0, 0, -7823, 0, 0, 0, 0, 1456,
  -4002, 0, 0, -9462, 0, 726, -7824, 0, 0, 0, 0, 0, -12011, 0, -1091, -7461, 0, -1636, -9280, 0, 0, 0
};
static const int32_t data_150BC[114] = {
  0, 0, -6003, 0, 0, -908, 0, 0, 0, 0, 0, -3819, 0, 0, -7094, 0, 0, -4547, 0, 0, 0, 0, 0, -2909, 0, 0, -8550, 0, -544, -4001, 0, 0, 0, 0, 0, -4363, 0,
  0, -8004, 0, -908, -2180, 0, 0, 0, 0, -1634, -6366, 0, -2000, -4365, 0, -1818, -2182, 0, 0, 0, 0, 0, -10922, 0, 0, -10558, 0, 1456, 0, 0, 0, -18022,
  0, 0, -17112, 0, 0, -18386, 0, 0, 0, 0, 0, -12196, 0, 0, -18204, 0, 0, -16748, 0, 0, 0, 0, 0, -11832, 0, 0, -18386, 0, 0, -15109, 0, 0, 0, 0, 819,
  -12014, 0, -1274, -16748, 0, -3458, -16201, 0, 0, 0
};
static const int32_t data_15284[57] = {
  0, 0, -3093, 0, 0, -1455, 0, 0, 0, 0, 0, -3275, 0, 0, -2001, 0, 0, -1637, 0, 0, 0, 0, 0, -12193, 0, 0, -12921, 0, 0, -4185, 0, 0, -363, 0, 0,
  -14195, 0, 0, -10919, 0, 0, -4550, 0, 0, 181, 0, 0, -15651, 0, 0, -10373, 0, 0, -6005, 0, -909, 181
};
static const char data_15368[17] __attribute__((scn_string_literal)) = "[[[UISRCHJB12]]]";
static const char data_15379[17] __attribute__((scn_string_literal)) = "[[[UISRCHJB10]]]";
static const char data_1538A[17] __attribute__((scn_string_literal)) = "[[[UISRCHJB13]]]";
static const char data_1539B[17] __attribute__((scn_string_literal)) = "[[[UISRCHJB11]]]";
static const char data_153AC[17] __attribute__((scn_string_literal)) = "[[[UISRCHJB09]]]";
static const char data_153BD[11] __attribute__((scn_string_literal)) = "SA1085A003";
static const char data_153C8[11] __attribute__((scn_string_literal)) = "SA1085A004";
static const char data_153D3[11] __attribute__((scn_string_literal)) = "SA1085A005";
static const char data_153DE[11] __attribute__((scn_string_literal)) = "SA1085A006";
static const char data_153E9[11] __attribute__((scn_string_literal)) = "SA1085A001";
static const char data_153F4[12] __attribute__((scn_string_literal)) = "SA1085A002";
static const Vector3i data_15400 = {.x = -4096, .y = 0, .z = 0};
static const Vector3i data_1540C = {.x = -11832, .y = 0, .z = 0};
static const char data_15418[11] __attribute__((scn_string_literal)) = "SA1085B001";
static const char data_15423[11] __attribute__((scn_string_literal)) = "SA1085A009";
static const char data_1542E[38] = "SA1085B041\000\000\000\000\000\340\377\377\000\000\000\000\000\000\000\000\310\321\377\377";
static const char data_15454[11] __attribute__((scn_string_literal)) = "SA1085A040";
static const char data_1545F[11] __attribute__((scn_string_literal)) = "SA1085B045";
static const char data_1546A[11] __attribute__((scn_string_literal)) = "SA1085B046";
static const char data_15475[11] __attribute__((scn_string_literal)) = "SA1085B047";
static const char data_15480[11] __attribute__((scn_string_literal)) = "SA1085B015";
static const char data_1548B[11] __attribute__((scn_string_literal)) = "SA1085B048";
static const char data_15496[11] __attribute__((scn_string_literal)) = "SA1085B049";
static const char data_154A1[11] __attribute__((scn_string_literal)) = "SA1085B015";
static const char data_154AC[11] __attribute__((scn_string_literal)) = "SA1085B054";
static const char data_154B7[11] __attribute__((scn_string_literal)) = "SA1085B052";
static const char data_154C2[11] __attribute__((scn_string_literal)) = "SA1085A039";
static const char data_154CD[11] __attribute__((scn_string_literal)) = "SA1085A038";
static const char data_154D8[11] __attribute__((scn_string_literal)) = "SA1085A039";
static const char data_154E3[11] __attribute__((scn_string_literal)) = "SA1085A041";
static const char data_154EE[11] __attribute__((scn_string_literal)) = "SA1085A042";
static const char data_154F9[11] __attribute__((scn_string_literal)) = "SA1085B042";
static const char data_15504[11] __attribute__((scn_string_literal)) = "SA1085B043";
static const char data_1550F[11] __attribute__((scn_string_literal)) = "SA1085B042";
static const char data_1551A[11] __attribute__((scn_string_literal)) = "SA1085B022";
static const char data_15525[11] __attribute__((scn_string_literal)) = "SA1085B044";
static const Vector3 data_15530 = {.x = -0.7679f, .y = 0.0f, .z = -0.8946f};
static const Vector3i data_1553C = {.x = -4096, .y = 0, .z = 0};
static const Vector3i data_15548 = {.x = -11832, .y = 0, .z = 0};
static const char data_15554[11] __attribute__((scn_string_literal)) = "SA1085A011";
static const char data_1555F[11] __attribute__((scn_string_literal)) = "SA1085B041";
static const char data_1556A[14] __attribute__((scn_string_literal)) = "scene/03/D000";
static const char data_15578[] __attribute__((scn_string_literal)) = "SCRTEST.BIN";
static const char data_15584[7] __attribute__((scn_string_literal)) = "SA1085";
static const char data_1558B[13] __attribute__((scn_string_literal)) = "M_MOBJ.BIN";
