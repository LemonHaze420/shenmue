#include "act01_jabe.h"

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
static const char data_1556A[14] __attribute__((scn_string_literal)) = "scene/01/D000";
static const char data_15578[] __attribute__((scn_string_literal)) = "SCRTEST.BIN";
static const char data_15584[7] __attribute__((scn_string_literal)) = "SA1085";
static const char data_1558B[13] __attribute__((scn_string_literal)) = "M_MOBJ.BIN";

__attribute__((scn_string_bindings(0, data_120B0)))
static void script_0030(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
  float local_F4[3];
  int32_t local_E8[3];
  int32_t local_E4;
  int32_t local_E0;
  int32_t local_DC;
  float local_D8;
  float local_D4;
  int32_t local_D0;
  int8_t local_CF;
  int8_t local_CE;
  int8_t local_CD;
  int8_t local_CC;
  int8_t local_CB;
  int8_t local_CA;
  script_0932(arg0, arg1, arg2, arg3, arg4, arg5);
  local_E8[0] = local_E8[2] = 0;
  while (1) {
    while (*(&task_78 + arg5) & 4) {
      SCNF_WaitFrames_0000(1);
    }
    arg2 = Script_KaidanUtils(CAM_STAIR_QUERY_GUIDE_ENTRY, arg0, task_6C, (const void *)task_70, (const void *)task_74, task_8C);
    if (arg2 >= 0) {
      Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_2);
      Script_ccowUtils(arg0, 9);
      if (*(&task_78 + arg5) & 0x800) {
        Script_scenUtils(SCENARIO_CMD_INIT_SCNT);
      }
      (void)((local_CA = 0) & 1);
      local_E4 = arg2 * 6;
      *(int32_t *)&local_F4[1] = Script_ScenDataUtil((void *)task_70, 4, local_E4 + 1);
      *(int32_t *)&local_F4[0] = Script_ScenDataUtil((void *)task_70, 4, local_E4);
      *(int32_t *)&local_F4[2] = Script_ScenDataUtil((void *)task_70, 4, local_E4 + 2);
      local_DC = Script_ScenDataUtil((void *)task_70, 4, local_E4 + 4);
      local_E8[1] = Script_ScenDataUtil((void *)task_70, 4, local_E4 + 3);
      local_E0 = Script_ScenDataUtil((void *)task_70, 4, local_E4 + 5);
      local_D8 = Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, local_E8[1]);
      local_D4 = Script_MathUtils(SCRIPT_MATH_COS_FIXED16, local_E8[1]);
      script_20A3(arg0, arg5, arg2);
      *(&task_84 + arg5) = arg2 + 1;
      *((int8_t *)&task_B0 + arg5) = 3;
      script_2D34(arg0, arg5, local_DC);
      Script_CharHandlers(arg0, 2, 0, 16);
      (void)((local_CD = -((local_CB = 0) != 0)) & 1);
      if (local_DC & 0xF00 & 0x400) {
        script_0A25(1, arg2);
        if (local_DC & 0xF000 & 0x1000) {
          (void)((task_B8 = -1) & 1);
          while (task_B8 & 8) {
            SCNF_WaitFrames_0000(1);
          }
        }
      }
      if (local_DC & 0x100) {
        (void)((local_CA = -1) & 1);
      }
      if (*(&task_78 + arg5) & 0x400) {
        Script_MotStepUtil(arg0, 0);
      }
      if ((local_DC & 0xF) <= 4) {
        Script_ccowSetFlag80(arg0, 1);
        if ((*(&task_78 + arg5) & 8) == 0) {
          if (local_DC & 0x1000000) {
            Script_scenCreateTask(script_24C5, 2, arg0, 1);
          } else {
            Script_scenCreateTask(script_24C5, 2, arg0, 1);
          }
        }
        if (local_DC & 0x2000) {
          task_B5 = 0;
        } else {
          uint8_t unused_C8[2];
          int32_t local_C4;
          local_C4 = *(&task_7C + arg5);
          switch (local_C4) {
            case 0:
              task_B5 = 0;
              break;
            case 1:
              task_B5 = 1;
              break;
            case 3:
              task_B5 = 1;
              break;
            case 2:
              if (Script_ryoControl(PCCTL_QueryIsRunningRequest)) {
                task_B5 = 1;
              } else {
                task_B5 = 0;
              }
              break;
          }
        }
        if (task_B5) {
          local_F4[0] = *local_F4 + local_D8 * -0.1f;
          local_F4[2] = local_F4[2] + local_D4 * -0.1f;
        } else {
          local_F4[0] = *local_F4 + local_D8 * 0.16f;
          local_F4[2] = local_F4[2] + local_D4 * 0.16f;
        }
        local_CF = script_1B07(arg0, local_E8[1], *local_F4, local_F4[2], arg5, local_DC, local_E0);
        if (local_CA & 4) {
          Script_KaidanUtils(CAM_STAIR_APPLY_GUIDE_FACING, arg0, arg2, (const void *)task_70);
        }
        (void)((local_CC = -1) & 1);
        Script_ccowSetFlag80(arg0, 2);
        Script_ccowUnknown(arg0, 1, 3.0f);
      } else {
        if ((local_DC & 0xF) <= 12) {
          if ((*(&task_78 + arg5) & 8) == 0) {
            Script_scenCreateTask(script_24C5, 2, arg0, 0);
          }
          if (local_DC & 0x2000) {
            task_B5 = 0;
          } else {
            uint8_t unused_C8[2];
            int32_t local_C4;
            local_C4 = *(&task_7C + arg5);
            switch (local_C4) {
              case 0:
                task_B5 = 0;
                break;
              case 1:
                task_B5 = 1;
                break;
              case 3:
                task_B5 = 1;
                break;
              case 2:
                if (Script_ryoControl(PCCTL_QueryIsRunningRequest)) {
                  task_B5 = 1;
                } else {
                  task_B5 = 0;
                }
                break;
            }
          }
          if (task_B5) {
            local_F4[0] = *local_F4 + local_D8 * -0.25f;
            local_F4[2] = local_F4[2] + local_D4 * -0.25f;
          } else {
            local_F4[0] = *local_F4 + local_D8 * 0.15f;
            local_F4[2] = local_F4[2] + local_D4 * 0.15f;
          }
          local_CF = local_E0 - script_1D96(arg0, local_E8[1], *local_F4, local_F4[2], arg5, local_DC, local_E0);
          if (local_CA & 4) {
            Script_KaidanUtils(CAM_STAIR_APPLY_GUIDE_FACING, arg0, arg2, (const void *)task_70);
          }
          Script_ccowSetFlag80(arg0, 2);
          Script_ccowUnknown(arg0, 1, 3.0f);
          (void)((local_CC = 0) & 0x80);
          goto L000525;
        }
      }
      L000525:;
      if ((local_E0 == 1) & ((local_DC & 0xF) <= 4)) {
        if (*((int8_t *)&task_B3 + arg5) == 1) {
          Script_motmToggleFlags(arg0, 1, 0x80);
        }
        if (task_B5) {
          Script_PlayCharaMotion(arg0, MN_AKI_AKI_RUN_MID_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
          SCNF_WaitFrames_0000(1);
          for (;;) {
            if (Script_GetCharaMotionRemainingFramesScaled(arg0) >= 10.0f) {
              break;
            }
            SCNF_WaitFrames_0000(1);
          }
        } else {
          Script_PlayCharaMotion(arg0, MN_AKI_AKI_WALK_SP2_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
          SCNF_WaitFrames_0000(1);
          for (;;) {
            if (Script_GetCharaMotionRemainingFramesScaled(arg0) >= 20.0f) {
              break;
            }
            SCNF_WaitFrames_0000(1);
          }
        }
      } else {
        local_D0 = 1;
        if (local_CC & 0x10) {
          Script_scenCreateTask(script_2ADB, 2, arg0, arg5);
        } else {
          Script_scenCreateTask(script_2C03, 2, arg0, arg5);
        }
        while ((local_CF >= 0) & (local_CF <= local_E0)) {
          local_CE = task_B5;
          if (local_D0) {
            local_D0 = 0;
          } else if (local_DC & 0x2000) {
            task_B5 = 0;
          } else {
            uint8_t unused_C8[2];
            int32_t local_C4;
            local_C4 = *(&task_7C + arg5);
            switch (local_C4) {
              case 0:
                task_B5 = 0;
                break;
              case 1:
                task_B5 = 1;
                break;
              case 3:
                task_B5 = 1;
                break;
              case 2:
                if (Script_ryoControl(PCCTL_QueryIsRunningRequest)) {
                  task_B5 = 1;
                } else {
                  task_B5 = 0;
                }
                break;
            }
          }
          if (local_CB & 8) {
          } else {
            if (*(&task_78 + arg5) & 0x100) {
              if (*(&task_78 + arg5) & 2) {
                if ((Script_GetControllerInput(0) & 0x2000) > 0) {
                  (void)((local_CD = -1) & 4);
                }
              }
            }
            if (local_CC & 0x10) {
              local_CF = local_CF + script_0E0C(arg0, local_CF, local_E0, arg5, local_DC, arg2, local_CE - task_B5);
            } else {
              local_CF = local_CF - script_12FC(arg0, local_CF, arg5, local_DC, arg2, local_CE - task_B5);
            }
            if ((local_CF > 0) & (local_CF < local_E0)) {
              if (!(local_CD & 0x20)) goto L000767;
            }
          }
          L000767:;
        }
        if (local_D0) {
          if (((*(&task_78 + arg5) & 0x10) == 0) & ((local_DC & 0xF) < 2)) {
            if (*((int8_t *)&task_B3 + arg5) == 1) {
              Script_motmToggleFlags(arg0, 1, 0x80);
            }
            if (task_B5) {
              Script_PlayCharaMotion(arg0, MN_AKI_AKI_RUN_MID_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
              SCNF_WaitFrames_0000(1);
              for (;;) {
                if (Script_GetCharaMotionRemainingFramesScaled(arg0) >= 10.0f) {
                  break;
                }
                SCNF_WaitFrames_0000(1);
              }
            } else {
              Script_PlayCharaMotion(arg0, MN_AKI_AKI_WALK_SP2_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
              SCNF_WaitFrames_0000(1);
              for (;;) {
                if (Script_GetCharaMotionRemainingFramesScaled(arg0) >= 20.0f) {
                  break;
                }
                SCNF_WaitFrames_0000(1);
              }
            }
          }
        }
      }
      if ((task_B6 == 1) | (task_B6 == 2)) {
        task_B6 = 3;
      }
      if ((*(&task_78 + arg5) & 2) > 0) {
        if (task_B5) {
          Script_SetCharaMotionPlaybackRate(arg0, 1.2f);
        } else {
          Script_SetCharaMotionPlaybackRate(arg0, 1.3f);
        }
        if (task_BC & 0x10) {
          Script_sprintf("Kaidan : ");
          Script_ryoControl(RYO_RestoreControl);
          (void)((task_BC = 0) & 4);
        }
        script_2E09(arg0, arg5, local_DC);
      }
      Script_ccowUnknown(arg0, 2, 0.07f);
      Script_ccowUtils(arg0, 10);
      if (*((int8_t *)&task_B1 + arg5) != 2) {
        *((int8_t *)&task_B1 + arg5) = 0;
      }
      *((int8_t *)&task_B2 + arg5) = 0;
      *((int8_t *)&task_B0 + arg5) = 0;
      *(&task_84 + arg5) = -1;
      if (*(&task_78 + arg5) & 0x800) {
        Script_scenUtils(SCENARIO_CMD_UPDATE_TIMER);
      }
    } else {
      (void)((task_BC = 0) & 1);
    }
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_0932(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
  int32_t local_FC;
  int32_t local_F8;
  *(&task_78 + arg5) = arg4;
  *(&task_80 + arg5) = arg0;
  *(&task_84 + arg5) = -1;
  *(&task_7C + arg5) = arg1;
  *((int8_t *)&task_B1 + arg5) = 0;
  *((int8_t *)&task_B2 + arg5) = 0;
  Script_sprintf("========== initializing kaidan\012");
  if (arg3) {
    (void)((task_B7 = 0) & 1);
    task_8C = 0x4000;
    task_B4 = 1;
    task_70 = arg3;
    (void)((task_BC = 0) & 1);
    if (arg2) {
      Script_sprintf("please ENDOFKDATA use can't start shuki kaidan\012");
      task_6C = arg2;
    } else {
      Script_CycleManagerUtils(CYCLE_CMD_SET_GLOBAL_CYCLE_PTR, task_70, &task_84);
      task_6C = local_F8 = 0;
      while ((local_FC = Script_ScenDataUtil((void *)task_70, 4, local_F8)) != 0x47C35000) {
        task_6C = task_6C + 1;
        local_F8 = local_F8 + 6;
      }
    }
  } else {
    task_B4 = task_B4 + 1;
  }
  return;
}

static void script_0A25(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  int32_t local_F0;
  local_F4 = 0;
  local_FC = 0;
  while ((local_F8 = Script_ScenDataUtil((void *)task_74, 4, local_FC)) != 3) {
    local_F0 = local_F8;
    if (local_F0 == 0) {
      local_FC = local_FC + 3;
    } else if (local_F0 == 1) {
      if (arg0) {
        if (Script_ScenDataUtil((void *)task_74, 4, local_FC + 1) == arg1) {
          local_F4 = Script_ScenDataUtil((void *)task_74, 4, local_FC + 2);
          if (local_F4 < 4) {
            local_F4 = task_90[local_F4];
            break;
          } else {
            Script_PreserveAccumulator_012C(0, "Kaidan Function Error 1\012");
          }
        }
      }
      local_FC = local_FC + 3;
    } else if (local_F0 == 2) {
      if (arg0 == 0) {
        if (Script_ScenDataUtil((void *)task_74, 4, local_FC + 1) == arg1) {
          local_F4 = Script_ScenDataUtil((void *)task_74, 4, local_FC + 2);
          if (local_F4 < 4) {
            local_F4 = task_90[local_F4];
            break;
          } else {
            Script_PreserveAccumulator_012C(0, "Kaidan Function Error 2\012");
          }
        }
      }
      local_FC = local_FC + 3;
    } else {
      Script_PreserveAccumulator_012C(0, "Kaidan Data Error\012");
    }
  }
  if (local_F4) {
    Script_scenCreateTask(local_F4, 0);
  }
  return;
}

static void script_0B64(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int8_t local_FF;
  int32_t local_F8;
  if (arg2 & 0x2000) {
    task_B5 = 0;
  } else {
    local_F8 = *(&task_7C + arg1);
    switch (local_F8) {
      case 0:
        task_B5 = 0;
        break;
      case 1:
        task_B5 = 1;
        break;
      case 3:
        task_B5 = 1;
        break;
      case 2:
        if (Script_ryoControl(PCCTL_QueryIsRunningRequest)) {
          task_B5 = 1;
        } else {
          task_B5 = 0;
        }
        break;
    }
  }
  if (task_B5) {
    if (arg3 > 1) {
      Script_PlayCharaMotion(arg0, MN_AKI_AKI_NOBORU_KAIDAN_2STEP_SONOBA_LP, -1.0f, -1.0f, 9.0f, 1.0f, 0);
      local_FF = 2;
      if (arg2 & 0x100000) {
        script_2739(arg0);
      } else {
        SCNF_WaitFrames_0000(1);
        while (Script_IsCharaMotionFinished(arg0) == 0) {
          SCNF_WaitFrames_0000(1);
        }
      }
    } else {
      Script_PlayCharaMotion(arg0, MN_AKI_AKI_NOBORU_KAIDAN_2STEP_SONOBA_LP, 0.0f, 8.0f, 1.0f, 1.0f, 0);
      local_FF = arg3;
      SCNF_WaitFrames_0000(1);
      while (Script_IsCharaMotionFinished(arg0) == 0) {
        SCNF_WaitFrames_0000(1);
      }
    }
  } else {
    if (arg3 > 1) {
      Script_PlayCharaMotion(arg0, MN_AKI_AKI_NOBORU_KAIDAN_SONOBA_LP, 0.0f, 29.0f, 5.0f, 1.0f, 0);
      local_FF = 1;
    } else {
      Script_PlayCharaMotion(arg0, MN_AKI_AKI_NOBORU_KAIDAN_SONOBA_LP, 0.0f, 18.0f, 5.0f, 1.0f, 0);
      local_FF = arg3;
    }
    SCNF_WaitFrames_0000(1);
    while (Script_IsCharaMotionFinished(arg0) == 0) {
      SCNF_WaitFrames_0000(1);
    }
  }
  (void)local_FF;
  return;
}

static void script_0D13(int32_t arg0, int32_t arg1, int32_t arg2) {
  int8_t local_FF;
  int32_t local_F8;
  if (arg2 & 0x2000) {
    task_B5 = 0;
  } else {
    local_F8 = *(&task_7C + arg1);
    switch (local_F8) {
      case 0:
        task_B5 = 0;
        break;
      case 1:
        task_B5 = 1;
        break;
      case 3:
        task_B5 = 1;
        break;
      case 2:
        if (Script_ryoControl(PCCTL_QueryIsRunningRequest)) {
          task_B5 = 1;
        } else {
          task_B5 = 0;
        }
        break;
    }
  }
  if (task_B5) {
    local_FF = 0;
    Script_PlayCharaMotion(arg0, MN_AKI_AKI_ORIRU_KAIDAN_2STEP_SONOBA_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  } else {
    Script_PlayCharaMotion(arg0, MN_AKI_AKI_ORIRU_KAIDAN_SONOBA_LP, 0.0f, 29.0f, 18.0f, 1.0f, 0);
    local_FF = 0;
    SCNF_WaitFrames_0000(1);
    while (Script_IsCharaMotionFinished(arg0) == 0) {
      SCNF_WaitFrames_0000(1);
    }
  }
  (void)local_FF;
  return;
}

static int8_t script_0E0C(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5, int32_t arg6) {
  int8_t local_FF;
  uint8_t unused_F0[15];
  task_A0 = 0;
  if (task_B5) {
    local_FF = 4;
    if (arg1 < arg2 - 3) {
      Script_motmToggleFlags(arg0, 2, 0x40);
      if (arg6) {
        if (*((int8_t *)&task_B3 + arg3) == 1) {
          Script_motmToggleFlags(arg0, 1, 0x80);
        }
        Script_PlayCharaMotion(arg0, MN_AKI_AKI_NOBORU_KAIDAN_2STEP_SONOBA_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
      }
      if (arg4 & 0x100000) {
        script_2814(arg0);
      } else {
        SCNF_WaitFrames_0000(1);
        while (Script_IsCharaMotionFinished(arg0) == 0) {
          SCNF_WaitFrames_0000(1);
        }
      }
      Script_motmToggleFlags(arg0, 1, 0x40);
    } else {
      if ((*(&task_78 + arg3) & 8) == 0) {
        task_B6 = 2;
      }
      script_2DB9(arg0, arg3, arg4);
      if (arg4 & 0xF00 & 0x800) {
        script_0A25(0, arg5);
        if (arg4 & 0xF000 & 0x1000) {
          (void)((task_B8 = -1) & 1);
          while (task_B8 & 8) {
            SCNF_WaitFrames_0000(1);
          }
        }
      } else if (arg2 <= arg1 + 2) {
        if (*((int8_t *)&task_B3 + arg3) == 0) {
          Script_motmToggleFlags(arg0, 1, 0x80);
        }
        Script_PlayCharaMotion(arg0, MN_AKI_AKI_RUN_MID_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        SCNF_WaitFrames_0000(1);
        for (;;) {
          if (Script_GetCharaMotionRemainingFramesScaled(arg0) >= 8.0f) {
            break;
          }
          SCNF_WaitFrames_0000(1);
        }
      } else {
        if (arg6) {
          if (*((int8_t *)&task_B3 + arg3) == 1) {
            Script_motmToggleFlags(arg0, 1, 0x80);
          }
          Script_PlayCharaMotion(arg0, MN_AKI_AKI_NOBORU_KAIDAN_2STEP_SONOBA_LP, -1.0f, 5.0f, -1.0f, 1.0f, 0);
          SCNF_WaitFrames_0000(1);
          while (Script_IsCharaMotionFinished(arg0) == 0) {
            SCNF_WaitFrames_0000(1);
          }
        } else {
          SCNF_WaitFrames_0000(1);
          for (;;) {
            if (Script_GetCharaMotionRemainingFramesScaled(arg0) >= 5.0f) {
              break;
            }
            SCNF_WaitFrames_0000(1);
          }
        }
        if (*((int8_t *)&task_B3 + arg3) == 0) {
          Script_motmToggleFlags(arg0, 1, 0x80);
        }
        Script_PlayCharaMotion(arg0, MN_AKI_AKI_RUN_MID_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        SCNF_WaitFrames_0000(1);
        for (;;) {
          if (Script_GetCharaMotionRemainingFramesScaled(arg0) >= 8.0f) {
            break;
          }
          SCNF_WaitFrames_0000(1);
        }
      }
      if ((*(&task_78 + arg3) & 8) == 0) {
        task_B6 = 3;
      }
    }
  } else {
    local_FF = 2;
    if (arg1 < arg2 - 1) {
      if (arg6) {
        if (*((int8_t *)&task_B3 + arg3) == 1) {
          Script_motmToggleFlags(arg0, 1, 0x80);
        }
        Script_PlayCharaMotion(arg0, MN_AKI_AKI_NOBORU_KAIDAN_SONOBA_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
      }
      if (arg4 & 0x100000) {
        script_265E(arg0);
      } else {
        SCNF_WaitFrames_0000(1);
        while (Script_IsCharaMotionFinished(arg0) == 0) {
          SCNF_WaitFrames_0000(1);
        }
      }
    } else {
      Script_ccowSetFlag80(arg0, 1);
      if ((*(&task_78 + arg3) & 8) == 0) {
        task_B6 = 2;
      }
      script_2DB9(arg0, arg3, arg4);
      if (arg6) {
        if (*((int8_t *)&task_B3 + arg3) == 1) {
          Script_motmToggleFlags(arg0, 1, 0x80);
        }
        Script_PlayCharaMotion(arg0, MN_AKI_AKI_NOBORU_KAIDAN_SONOBA_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
      }
      if (arg2 != arg1) {
        SCNF_WaitFrames_0000(1);
        for (;;) {
          if (Script_GetCharaMotionRemainingFramesScaled(arg0) >= 13.0f) {
            break;
          }
          SCNF_WaitFrames_0000(1);
        }
      } else {
        SCNF_WaitFrames_0000(1);
        for (;;) {
          if (Script_GetCharaMotionRemainingFramesScaled(arg0) >= 3.0f) {
            break;
          }
          SCNF_WaitFrames_0000(1);
        }
      }
      Script_ccowSetFlag80(arg0, 2);
      if (arg4 & 0xF00 & 0x800) {
        script_0A25(0, arg5);
        if (arg4 & 0xF000 & 0x1000) {
          (void)((task_B8 = -1) & 2);
          while (task_B8 & 8) {
            SCNF_WaitFrames_0000(1);
          }
        }
      } else {
        if (arg2 == arg1) {
          if (*((int8_t *)&task_B3 + arg3) == 0) {
            Script_motmToggleFlags(arg0, 1, 0x80);
          }
        } else {
          if (*((int8_t *)&task_B3 + arg3) == 1) {
            Script_motmToggleFlags(arg0, 1, 0x80);
          }
        }
        Script_PlayCharaMotion(arg0, MN_AKI_AKI_WALK_SP2_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        SCNF_WaitFrames_0000(1);
        for (;;) {
          if (Script_GetCharaMotionRemainingFramesScaled(arg0) >= 25.0f) {
            break;
          }
          SCNF_WaitFrames_0000(1);
        }
      }
      if ((*(&task_78 + arg3) & 8) == 0) {
        task_B6 = 3;
      }
    }
  }
  return local_FF;
}

static int8_t script_12FC(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
  int8_t local_FF;
  if (task_B5) {
    local_FF = 4;
    if (arg1 >= local_FF) {
      if (arg5) {
        if (*((int8_t *)&task_B3 + arg2) == 1) {
          Script_motmToggleFlags(arg0, 1, 0x80);
        }
        Script_PlayCharaMotion(arg0, MN_AKI_AKI_ORIRU_KAIDAN_2STEP_SONOBA_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
      }
      if (arg3 & 0x100000) {
        script_29E5(arg0, arg2);
      } else {
        SCNF_WaitFrames_0000(1);
        while (Script_IsCharaMotionFinished(arg0) == 0) {
          SCNF_WaitFrames_0000(1);
        }
      }
    } else {
      if (*((int8_t *)&task_B1 + arg2) != 2) {
        *((int8_t *)&task_B1 + arg2) = 0;
      }
      switch (arg1) {
        case 1:
          if (arg5) {
            if (*((int8_t *)&task_B3 + arg2) == 1) {
              Script_motmToggleFlags(arg0, 1, 0x80);
            }
            Script_PlayCharaMotion(arg0, MN_AKI_AKI_ORIRU_KAIDAN_2STEP_SONOBA_LP, 0.0f, 5.0f, 0.0f, 1.0f, 0);
            SCNF_WaitFrames_0000(1);
            while (Script_IsCharaMotionFinished(arg0) == 0) {
              SCNF_WaitFrames_0000(1);
            }
          } else {
            SCNF_WaitFrames_0000(1);
            for (;;) {
              if (Script_GetCharaMotionRemainingFramesScaled(arg0) >= 5.0f) {
                break;
              }
              SCNF_WaitFrames_0000(1);
            }
          }
          break;
        case 2:
          if (arg5) {
            if (*((int8_t *)&task_B3 + arg2) == 1) {
              Script_motmToggleFlags(arg0, 1, 0x80);
            }
            Script_PlayCharaMotion(arg0, MN_AKI_AKI_ORIRU_KAIDAN_2STEP_SONOBA_LP, 0.0f, 7.0f, 0.0f, 1.0f, 0);
            SCNF_WaitFrames_0000(1);
            while (Script_IsCharaMotionFinished(arg0) == 0) {
              SCNF_WaitFrames_0000(1);
            }
          } else {
            SCNF_WaitFrames_0000(1);
            for (;;) {
              if (Script_GetCharaMotionRemainingFramesScaled(arg0) >= 7.0f) {
                break;
              }
              SCNF_WaitFrames_0000(1);
            }
          }
          break;
        case 3:
          if (arg5) {
            if (*((int8_t *)&task_B3 + arg2) == 1) {
              Script_motmToggleFlags(arg0, 1, 0x80);
            }
            Script_PlayCharaMotion(arg0, MN_AKI_AKI_ORIRU_KAIDAN_2STEP_SONOBA_LP, 0.0f, 9.0f, 0.0f, 1.0f, 0);
            SCNF_WaitFrames_0000(1);
            while (Script_IsCharaMotionFinished(arg0) == 0) {
              SCNF_WaitFrames_0000(1);
            }
          } else {
            SCNF_WaitFrames_0000(1);
            for (;;) {
              if (Script_GetCharaMotionRemainingFramesScaled(arg0) >= 9.0f) {
                break;
              }
              SCNF_WaitFrames_0000(1);
            }
          }
          break;
      }
      if ((*(&task_78 + arg2) & 8) == 0) {
        task_B6 = 2;
      }
      script_2DB9(arg0, arg2, arg3);
      if (arg3 & 0xF00 & 0x800) {
        script_0A25(0, arg4);
        if (arg3 & 0xF000 & 0x1000) {
          (void)((task_B8 = -1) & 1);
          while (task_B8 & 8) {
            SCNF_WaitFrames_0000(1);
          }
        }
      } else {
        if (arg1 <= 2) {
          if (*((int8_t *)&task_B3 + arg2) == 1) {
            Script_motmToggleFlags(arg0, 1, 0x80);
          }
        } else {
          if (*((int8_t *)&task_B3 + arg2) == 0) {
            Script_motmToggleFlags(arg0, 1, 0x80);
          }
        }
        Script_PlayCharaMotion(arg0, MN_AKI_AKI_RUN_MID_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        SCNF_WaitFrames_0000(1);
        for (;;) {
          if (Script_GetCharaMotionRemainingFramesScaled(arg0) >= 8.0f) {
            break;
          }
          SCNF_WaitFrames_0000(1);
        }
      }
      if ((*(&task_78 + arg2) & 8) == 0) {
        task_B6 = 3;
      }
    }
  } else {
    local_FF = 2;
    if (arg1 >= local_FF) {
      if (arg5) {
        if (*((int8_t *)&task_B3 + arg2) == 1) {
          Script_motmToggleFlags(arg0, 1, 0x80);
        }
        Script_PlayCharaMotion(arg0, MN_AKI_AKI_ORIRU_KAIDAN_SONOBA_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
      }
      if (arg3 & 0x100000) {
        script_28EF(arg0, arg2);
      } else {
        SCNF_WaitFrames_0000(1);
        while (Script_IsCharaMotionFinished(arg0) == 0) {
          SCNF_WaitFrames_0000(1);
        }
      }
    } else {
      if (*((int8_t *)&task_B1 + arg2) != 2) {
        *((int8_t *)&task_B1 + arg2) = 0;
      }
      if (arg1) {
        if (arg5) {
          if (*((int8_t *)&task_B3 + arg2) == 1) {
            Script_motmToggleFlags(arg0, 1, 0x80);
          }
          Script_PlayCharaMotion(arg0, MN_AKI_AKI_ORIRU_KAIDAN_SONOBA_LP, 0.0f, 15.0f, 0.0f, 1.0f, 0);
          SCNF_WaitFrames_0000(1);
          while (Script_IsCharaMotionFinished(arg0) == 0) {
            SCNF_WaitFrames_0000(1);
          }
        } else {
          SCNF_WaitFrames_0000(1);
          for (;;) {
            if (Script_GetCharaMotionRemainingFramesScaled(arg0) >= 15.0f) {
              break;
            }
            SCNF_WaitFrames_0000(1);
          }
        }
      }
      if ((*(&task_78 + arg2) & 8) == 0) {
        task_B6 = 2;
      }
      if (arg3 & 0xF00 & 0x800) {
        script_0A25(0, arg4);
        if (arg3 & 0xF000 & 0x1000) {
          (void)((task_B8 = -1) & 1);
          while (task_B8 & 8) {
            SCNF_WaitFrames_0000(1);
          }
        }
        script_2DB9(arg0, arg2, arg3);
      } else {
        if (arg1) {
          if (*((int8_t *)&task_B3 + arg2) == 1) {
            Script_motmToggleFlags(arg0, 1, 0x80);
          }
        } else {
          if (*((int8_t *)&task_B3 + arg2) == 0) {
            Script_motmToggleFlags(arg0, 1, 0x80);
          }
        }
        Script_PlayCharaMotion(arg0, MN_AKI_AKI_WALK_SP2_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        SCNF_WaitFrames_0000(1);
        for (;;) {
          if (Script_GetCharaMotionRemainingFramesScaled(arg0) >= 10.0f) {
            break;
          }
          SCNF_WaitFrames_0000(1);
        }
        script_2DB9(arg0, arg2, arg3);
        SCNF_WaitFrames_0000(1);
        for (;;) {
          if (Script_GetCharaMotionRemainingFramesScaled(arg0) >= 20.0f) {
            break;
          }
          SCNF_WaitFrames_0000(1);
        }
      }
      if ((*(&task_78 + arg2) & 8) == 0) {
        task_B6 = 3;
      }
    }
  }
  return local_FF;
}

static void script_18C8(int32_t arg0, int32_t arg1, float arg2, float arg3, int32_t arg4, int32_t arg5) {
  Vector3 local_F4;
  float local_E8[3];
  if (arg5 & 0x2000) {
    task_B5 = 0;
  } else {
    int32_t unused_D4[5];
    int32_t local_D0;
    local_D0 = *(&task_7C + arg4);
    switch (local_D0) {
      case 0:
        task_B5 = 0;
        break;
      case 1:
        task_B5 = 1;
        break;
      case 3:
        task_B5 = 1;
        break;
      case 2:
        if (Script_ryoControl(PCCTL_QueryIsRunningRequest)) {
          task_B5 = 1;
        } else {
          task_B5 = 0;
        }
        break;
    }
  }
  Script_EVT_GetPosition(arg0, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  Script_charaToggleMotEnabled(arg0, 0);
  if (arg5 & 0x100) {
    int32_t unused_E4;
    float local_E0;
    float local_DC;
    float local_D8;
    local_E0 = Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, arg1);
    local_DC = Script_MathUtils(SCRIPT_MATH_COS_FIXED16, arg1);
    local_D8 = (&local_F4)->x - arg2;
    {
      float local_D4;
      local_D4 = (&local_F4)->z - arg3;
      local_D8 = local_D8 * local_DC - local_D4 * local_E0;
    }
    {
      float local_D4;
      local_D4 = 0.0f;
      arg1 = 0x10000 - arg1;
      local_E0 = Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, arg1);
      local_DC = Script_MathUtils(SCRIPT_MATH_COS_FIXED16, arg1);
      local_E8[0] = local_D8 * local_DC - local_D4 * local_E0;
      local_E8[2] = local_D8 * local_E0 + local_D4 * local_DC;
    }
    arg2 = *local_E8 + arg2;
    arg3 = local_E8[2] + arg3;
  }
  if (task_B5) {
    float local_E4;
    local_E4 = 3.0f;
    arg2 = arg2 - (&local_F4)->x;
    arg2 = arg2 / local_E4;
    arg3 = arg3 - (&local_F4)->z;
    arg3 = arg3 / local_E4;
    Script_PlayCharaMotion(arg0, MN_AKI_AKI_RUN_MID_LP, 0.0f, 10.0f, 7.0f, 1.0f, 0);
  } else {
    float local_E4;
    local_E4 = 6.0f;
    arg2 = arg2 - (&local_F4)->x;
    arg2 = arg2 / local_E4;
    arg3 = arg3 - (&local_F4)->z;
    arg3 = arg3 / local_E4;
    Script_PlayCharaMotion(arg0, MN_AKI_AKI_WALK_SP2_LP, 0.0f, 8.0f, 2.0f, 1.0f, 0);
  }
  SCNF_WaitFrames_0000(1);
  while (Script_IsCharaMotionFinished(arg0) == 0) {
    local_F4.x = (&local_F4)->x + arg2;
    (&local_F4)->z = (&local_F4)->z + arg3;
    Script_EVT_SetPosition(arg0, 0x78000000, &local_F4);
    SCNF_WaitFrames_0000(1);
  }
  Script_charaToggleMotEnabled(arg0, 1);
  return;
}

static int32_t script_1B07(int32_t arg0, int32_t arg1, float arg2, float arg3, int32_t arg4, int32_t arg5, int32_t arg6) {
  Vector3 local_F4;
  float local_E8[3];
  Vector3 local_DC;
  Vector3 local_D0;
  float local_CC;
  float local_C8;
  int32_t local_C4;
  float local_C0;
  float local_BC;
  int32_t local_B8;
  task_A0 = 0;
  Script_EVT_GetPosition(arg0, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  if (arg5 & 0x100) {
    local_CC = Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, arg1);
    local_C8 = Script_MathUtils(SCRIPT_MATH_COS_FIXED16, arg1);
    local_C0 = (&local_F4)->x - arg2;
    local_BC = (&local_F4)->z - arg3;
    local_C0 = local_C0 * local_C8 - local_BC * local_CC;
    local_BC = 0.0f;
    local_C4 = 0x10000 - arg1;
    local_CC = Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, local_C4);
    local_C8 = Script_MathUtils(SCRIPT_MATH_COS_FIXED16, local_C4);
    local_E8[0] = local_C0 * local_C8 - local_BC * local_CC;
    local_E8[2] = local_C0 * local_CC + local_BC * local_C8;
    arg2 = *local_E8 + arg2;
    arg3 = local_E8[2] + arg3;
  }
  if (task_B5) {
    Script_motmToggleFlags(arg0, 2, 0x40);
    if (Script_motmUtilsEx(13, arg0, 0) != 0) {
      SCNF_WaitFrames_0000(1);
    }
    Script_motmToggleFlags(arg0, 1, 0x40);
    local_F4.x = arg2;
    (&local_F4)->z = arg3;
    Script_EVT_GetPosition(arg0, 26, &local_DC, ENTITY_XFORM_WORLD_SPACE);
    Script_EVT_GetPosition(arg0, 33, &local_D0, ENTITY_XFORM_WORLD_SPACE);
    if (!((&local_DC)->y >= (&local_D0)->y)) {
      task_A0 = -0x80000000;
      *((int8_t *)&task_B3 + arg4) = 1;
    } else {
      *((int8_t *)&task_B3 + arg4) = 0;
    }
    Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
    if (arg6 > 1) {
      Script_SetCharaMoveTargetEx(arg0, &local_F4, arg1, task_A0 | 0x2CB, 0, -1.0f, 2);
      local_B8 = 2;
    } else {
      Script_SetCharaMoveTargetEx(arg0, &local_F4, arg1, task_A0 | 0x2CB, 0, -1.0f, 2);
      local_B8 = arg6;
    }
  } else {
    local_F4.x = arg2;
    (&local_F4)->z = arg3;
    Script_EVT_GetPosition(arg0, 26, &local_DC, ENTITY_XFORM_WORLD_SPACE);
    Script_EVT_GetPosition(arg0, 33, &local_D0, ENTITY_XFORM_WORLD_SPACE);
    if (!((&local_DC)->y >= (&local_D0)->y)) {
      task_A0 = -0x80000000;
      *((int8_t *)&task_B3 + arg4) = 1;
    } else {
      *((int8_t *)&task_B3 + arg4) = 0;
    }
    Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
    if (arg6 > 1) {
      Script_SetCharaMoveTargetEx(arg0, &local_F4, arg1, task_A0 | 0x2C6, 0, -1.0f, 2);
      local_B8 = 1;
    } else {
      Script_SetCharaMoveTargetEx(arg0, &local_F4, arg1, task_A0 | 0x2C6, 0, -1.0f, 2);
      local_B8 = arg6;
    }
  }
  while (Script_SetCharaXMPTMode(arg0, 2) != 0) {
    SCNF_WaitFrames_0000(1);
  }
  return local_B8;
}

__attribute__((scn_string_bindings(0, data_1214F)))
static int32_t script_1D96(int32_t arg0, int32_t arg1, float arg2, float arg3, int32_t arg4, int32_t arg5, int32_t arg6) {
  Vector3 local_F4;
  float local_E8[3];
  Vector3 local_DC;
  Vector3 local_D0;
  float local_CC;
  float local_C8;
  int32_t local_C4;
  float local_C0;
  float local_BC;
  int32_t local_B8;
  Script_EVT_GetPosition(arg0, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  Script_ccowUtils(arg0, 2);
  if (arg5 & 0x100) {
    local_CC = Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, arg1);
    local_C8 = Script_MathUtils(SCRIPT_MATH_COS_FIXED16, arg1);
    local_C0 = (&local_F4)->x - arg2;
    local_BC = (&local_F4)->z - arg3;
    local_C0 = local_C0 * local_C8 - local_BC * local_CC;
    local_BC = 0.0f;
    local_C4 = 0x10000 - arg1;
    local_CC = Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, local_C4);
    local_C8 = Script_MathUtils(SCRIPT_MATH_COS_FIXED16, local_C4);
    local_E8[0] = local_C0 * local_C8 - local_BC * local_CC;
    local_E8[2] = local_C0 * local_CC + local_BC * local_C8;
    arg2 = *local_E8 + arg2;
    arg3 = local_E8[2] + arg3;
  }
  task_A0 = 0;
  if (task_B5) {
    Script_motmToggleFlags(arg0, 2, 0x40);
    if (Script_motmUtilsEx(13, arg0, 0) != 0) {
      SCNF_WaitFrames_0000(1);
    }
    Script_motmToggleFlags(arg0, 1, 0x40);
    local_F4.x = arg2;
    (&local_F4)->z = arg3;
    Script_EVT_GetPosition(arg0, 26, &local_DC, ENTITY_XFORM_WORLD_SPACE);
    Script_EVT_GetPosition(arg0, 33, &local_D0, ENTITY_XFORM_WORLD_SPACE);
    Script_motmUtilsEx(14, arg0, 0x3F800000);
    if (!((&local_DC)->y >= (&local_D0)->y)) {
      task_A0 = -0x80000000;
      *((int8_t *)&task_B3 + arg4) = 1;
    } else {
      *((int8_t *)&task_B3 + arg4) = 0;
    }
    Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
    if (arg6 > 1) {
      Script_SetCharaMoveTargetEx(arg0, &local_F4, arg1, task_A0 | 0x2F9 | 0x30000, 0, -1.0f, 2);
      local_B8 = 0;
    } else {
      Script_SetCharaMoveTargetEx(arg0, &local_F4, arg1, task_A0 | 0x2F9 | 0x30000, 0, -1.0f, 2);
      local_B8 = arg6;
    }
  } else {
    local_F4.x = arg2;
    (&local_F4)->z = arg3;
    Script_EVT_GetPosition(arg0, 26, &local_DC, ENTITY_XFORM_WORLD_SPACE);
    Script_EVT_GetPosition(arg0, 33, &local_D0, ENTITY_XFORM_WORLD_SPACE);
    if (!((&local_DC)->y >= (&local_D0)->y)) {
      task_A0 = -0x80000000;
      *((int8_t *)&task_B3 + arg4) = 1;
    } else {
      *((int8_t *)&task_B3 + arg4) = 0;
    }
    Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
    if (arg6 > 1) {
      Script_SetCharaMoveTargetEx(arg0, &local_F4, arg1, task_A0 | 0x2C7 | 0x90000, 0, -1.0f, 2);
      local_B8 = 0;
    } else {
      Script_SetCharaMoveTargetEx(arg0, &local_F4, arg1, task_A0 | 0x2C7 | 0x90000, 0, -1.0f, 2);
      local_B8 = arg6;
    }
  }
  while (Script_SetCharaXMPTMode(arg0, 2) != 0) {
    SCNF_WaitFrames_0000(1);
  }
  Script_ccowUtils(arg0, 1);
  script_2D34(arg0, arg4, arg5);
  if (*(&task_78 + arg4) & 2) {
    if (Script_ryoControl(PCCTL_QueryShouldRun)) {
      Script_sprintf("Kaidan : ");
      Script_ryoControl(PCCTL_DisablePlayerControl);
      (void)((task_BC = -1) & 4);
    }
    Script_ccowUtils(arg0, 6);
  }
  return local_B8;
}

__attribute__((scn_string_bindings(1, data_12175)))
static void script_20A3(int32_t arg0, int32_t arg1, int32_t arg2) {
  int32_t local_FC;
  int32_t local_F8;
  int8_t local_F7;
  (void)((local_F7 = 0) & 4);
  arg2 = arg2 & -2;
  local_F8 = Script_ScenDataUtil((void *)task_70, 4, arg2 * 6 + 4);
  local_FC = Script_ScenDataUtil((void *)task_70, 4, (arg2 + 1) * 6 + 4);
  local_FC = local_FC | local_F8;
  if (local_FC & -0x80000000) {
    while (local_FC & -0x80000000) {
      if (local_F7 & 0x80) {
        if (task_C0) {
        } else {
          Script_sprintf("wait ... shuki shuki shuki\012");
        }
      } else {
        if (*(&task_78 + arg1) & 2) {
          if (Script_ryoControl(PCCTL_QueryShouldRun)) {
            Script_sprintf("Kaidan : ");
            Script_ryoControl(PCCTL_DisablePlayerControl);
            (void)((task_BC = -1) & 4);
          }
          Script_ccowUtils(arg0, 6);
        }
        Script_charaToggleMotEnabled(arg0, 0);
        if (Script_ryoControl(PCCTL_QueryIsRunningRequest)) {
          Script_PlayCharaMotion(arg0, MN_AKI_AKI_RUN_EN, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        } else {
          Script_PlayCharaMotion(arg0, MN_AKI_AKI_WALK_EN, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        }
        SCNF_WaitFrames_0000(1);
        while (Script_IsCharaMotionFinished(arg0) == 0) {
          SCNF_WaitFrames_0000(1);
        }
        Script_charaToggleMotEnabled(arg0, 1);
        Script_PlayCharaMotion(arg0, MN_AKI_AKI_MOVE_B_45, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        Script_SetCharaMotionPlaybackRate(arg0, 1.2f);
        SCNF_WaitFrames_0000(1);
        while (Script_IsCharaMotionFinished(arg0) == 0) {
          SCNF_WaitFrames_0000(1);
        }
        (void)((local_F7 = -1) & 4);
      }
      task_C0 = (task_C0 + 1) & 0xF;
      local_F8 = Script_ScenDataUtil((void *)task_70, 4, arg2 * 6 + 4);
      local_FC = Script_ScenDataUtil((void *)task_70, 4, (arg2 + 1) * 6 + 4);
      local_FC = local_FC | local_F8;
      SCNF_WaitFrames_0000(1);
    }
    Script_PlayCharaMotion(arg0, MN_AKI_AKI_WALK_ST, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_motmToggleFlags(arg0, 1, 0x80);
    SCNF_WaitFrames_0000(1);
    while (Script_IsCharaMotionFinished(arg0) == 0) {
      SCNF_WaitFrames_0000(1);
    }
  }
  return;
}

static void script_22D1(int32_t arg0) {
  if (~task_B7) {
    (void)((task_B7 = -1) & 1);
    task_A4 = Script_SetCameraSettings(CAM_UTILS_SET_DISTANCE_FROM_PLAYER, -1.0f);
    task_A8 = Script_SetCameraSettings(CAM_UTILS_SET_HEIGHT_ABOVE_PLAYER, -1.0f);
    task_AC = Script_SetCameraSettings(CAM_UTILS_SET_MAX_Y_ANGLE, -1.0f);
  }
  Script_SetCameraSettings(6, 4);
  Script_convSetCamInactiveFlag(0);
  if (arg0) {
    Script_SetCameraSettings(CAM_UTILS_SET_DISTANCE_FROM_PLAYER, 3.1f);
    Script_SetCameraSettings(CAM_UTILS_SET_HEIGHT_ABOVE_PLAYER, 0.0f);
    Script_SetCameraSettings(CAM_UTILS_SET_MAX_Y_ANGLE, 1.0f);
  } else {
    Script_SetCameraSettings(CAM_UTILS_SET_DISTANCE_FROM_PLAYER, 2.0f);
    Script_SetCameraSettings(CAM_UTILS_SET_HEIGHT_ABOVE_PLAYER, 2.0f);
    Script_SetCameraSettings(CAM_UTILS_SET_MAX_Y_ANGLE, 1.0f);
  }
  return;
}

static void script_2370(float arg0) {
  int32_t local_FC;
  float local_F8;
  local_FC = 0;
  while (local_FC < 10) {
    int32_t local_F4;
    local_F4 = task_B6;
    if ((local_F4 == 0) | (local_F4 == 2) | (local_F4 == 4)) {
      break;
      goto L0023A3;
    }
    L0023A3:;
    local_F8 = task_AC + (arg0 - task_AC) * (float)local_FC / 10.0f;
    Script_SetCameraSettings(CAM_UTILS_SET_MAX_Y_ANGLE, local_F8);
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  {
    int32_t local_F4;
    local_F4 = task_B6;
    if ((local_F4 == 0) | (local_F4 == 2) | (local_F4 == 4)) {
    } else {
      Script_SetCameraSettings(CAM_UTILS_SET_MAX_Y_ANGLE, arg0);
    }
  }
  return;
}

static void script_23FF(float arg0) {
  int32_t local_FC;
  float local_F8;
  local_FC = 0;
  while (local_FC < 50) {
    int32_t local_F4;
    local_F4 = task_B6;
    if ((local_F4 == 0) | (local_F4 == 2) | (local_F4 == 4)) {
      break;
      goto L002432;
    }
    L002432:;
    local_F8 = task_A4 + (arg0 - task_A4) * (float)local_FC / 50.0f;
    Script_SetCameraSettings(CAM_UTILS_SET_DISTANCE_FROM_PLAYER, local_F8);
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  {
    int32_t local_F4;
    local_F4 = task_B6;
    if ((local_F4 == 0) | (local_F4 == 2) | (local_F4 == 4)) {
    } else {
      Script_SetCameraSettings(CAM_UTILS_SET_DISTANCE_FROM_PLAYER, arg0);
    }
  }
  return;
}

static void script_248E(void) {
  Script_SetCameraSettings(6, 7);
  Script_SetCameraSettings(CAM_UTILS_SET_DISTANCE_FROM_PLAYER, task_A4);
  Script_SetCameraSettings(CAM_UTILS_SET_HEIGHT_ABOVE_PLAYER, task_A8);
  Script_SetCameraSettings(CAM_UTILS_SET_MAX_Y_ANGLE, task_AC);
  Script_KaidanUtils(CAM_STAIR_RESET_RETRACTION_LIMIT);
  (void)((task_B7 = 0) & 2);
  return;
}

static int32_t script_24C5(int32_t arg0, int32_t arg1) {
  Vector3 local_F4;
  Vector3 local_E8;
  int8_t local_E7;
  int8_t local_E6;
  task_88 = arg0;
  task_B6 = 1;
  local_E7 = -1;
  (void)((local_E6 = 0) & 2);
  Script_SetGameStateTo(AUTH_None);
  while (task_88 == arg0) {
    if (Script_GetAuthorState_And_SetCamPos(&local_F4, &local_E8) != 17) {
      if (task_B6 != local_E7) {
        uint8_t unused_E4[2];
        int32_t local_E0;
        local_E0 = task_B6;
        if (local_E0 == 1) {
          (void)((local_E6 = -1) & 2);
          script_22D1(arg1);
        } else if (local_E0 == 2) {
          script_248E();
        } else if ((local_E0 == 0) | (local_E0 == 3)) {
          Script_SetCameraSettings(6, 7);
          task_88 = 0;
        } else {
          if (local_E0 == 4) goto L002572;
        }
        L002572:;
        local_E7 = task_B6;
      }
    } else {
      if (task_B6 != local_E7) {
        uint8_t unused_E4[2];
        int32_t local_E0;
        local_E0 = task_B6;
        if ((local_E0 == 0) | (local_E0 == 3)) {
          Script_SetCameraSettings(6, 7);
          task_88 = 0;
        } else if ((local_E0 == 1) | (local_E0 == 2)) {
          Script_SetGameStateTo(AUTH_FirstPerson);
        } else {
          if (local_E0 == 4) goto L0025D2;
        }
        L0025D2:;
        local_E7 = 4;
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  if (task_B6 != 0) {
    if (local_E6 & 0x40) {
      script_248E();
    }
  }
  end:;
}

static void script_25FC(int32_t arg0) {
  int8_t local_FF;
  int32_t local_F8;
  local_F8 = -1;
  local_FF = 0;
  while (local_FF < task_B4) {
    if (arg0 == *(&task_80 + local_FF)) {
      local_F8 = *(&task_84 + local_FF);
    }
    local_FF = local_FF + 1;
  }
  (void)local_F8;
  return;
}

static void script_2643(int32_t arg0) {
  task_74 = arg0;
  return;
}

static void script_264D(int32_t arg0) {
  task_8C = arg0;
  return;
}

static void script_2657(void) {
  (void)task_8C;
  return;
}

__attribute__((scn_string_bindings(0, data_1217F)))
static void script_265E(int32_t arg0) {
  Vector3 local_F4;
  Vector3i local_E8;
  float local_E4;
  float local_E0;
  float local_DC;
  Script_EVT_GetPosition(arg0, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  Script_mtwkGetSetRotation(arg0, &local_E8, 0);
  local_E4 = Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_E8)->y);
  local_E0 = Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_E8)->y);
  SCNF_WaitFrames_0000(1);
  while (Script_IsCharaMotionFinished(arg0) == 0) {
    SCNF_WaitFrames_0000(1);
  }
  local_F4.x = (&local_F4)->x - local_E4 * 0.6f;
  (&local_F4)->y = (&local_F4)->y + 0.45f;
  (&local_F4)->z = (&local_F4)->z - local_E0 * 0.6f;
  local_DC = Script_GetHighestCollisionBelow(arg0, &local_F4);
  if (!(local_DC < (&local_F4)->y - 0.1f)) {
    (&local_F4)->y = local_DC;
    Script_EVT_SetPosition(arg0, 0x78000000, &local_F4);
  } else {
    Script_sprintf("KAIDAN : Warning coli!\012");
  }
  return;
}

__attribute__((scn_string_bindings(0, data_12197)))
static void script_2739(int32_t arg0) {
  Vector3 local_F4;
  Vector3i local_E8;
  float local_E4;
  float local_E0;
  float local_DC;
  Script_EVT_GetPosition(arg0, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  Script_mtwkGetSetRotation(arg0, &local_E8, 0);
  local_E4 = Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_E8)->y);
  local_E0 = Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_E8)->y);
  SCNF_WaitFrames_0000(1);
  while (Script_IsCharaMotionFinished(arg0) == 0) {
    SCNF_WaitFrames_0000(1);
  }
  local_F4.x = (&local_F4)->x - local_E4 * 0.480000019f;
  (&local_F4)->y = (&local_F4)->y + 0.359999985f;
  (&local_F4)->z = (&local_F4)->z - local_E0 * 0.480000019f;
  local_DC = Script_GetHighestCollisionBelow(arg0, &local_F4);
  if (!(local_DC < (&local_F4)->y - 0.1f)) {
    (&local_F4)->y = local_DC;
    Script_EVT_SetPosition(arg0, 0x78000000, &local_F4);
  } else {
    Script_sprintf("KAIDAN : Warning coli!\012");
  }
  return;
}

__attribute__((scn_string_bindings(0, data_121AF)))
static void script_2814(int32_t arg0) {
  Vector3 local_F4;
  Vector3i local_E8;
  float local_E4;
  float local_E0;
  float local_DC;
  Script_EVT_GetPosition(arg0, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  Script_mtwkGetSetRotation(arg0, &local_E8, 0);
  local_E4 = Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_E8)->y);
  local_E0 = Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_E8)->y);
  SCNF_WaitFrames_0000(1);
  while (Script_IsCharaMotionFinished(arg0) == 0) {
    SCNF_WaitFrames_0000(1);
  }
  local_F4.x = (&local_F4)->x - local_E4 * 1.2f;
  (&local_F4)->y = (&local_F4)->y + 0.9f;
  (&local_F4)->z = (&local_F4)->z - local_E0 * 1.2f;
  local_DC = Script_GetHighestCollisionBelow(arg0, &local_F4);
  if (!(local_DC < (&local_F4)->y - 0.1f)) {
    (&local_F4)->y = local_DC;
    Script_EVT_SetPosition(arg0, 0x78000000, &local_F4);
  } else {
    Script_sprintf("KAIDAN : Warning coli!\012");
  }
  return;
}

__attribute__((scn_string_bindings(0, data_121C7)))
static void script_28EF(int32_t arg0, int32_t arg1) {
  Vector3 local_F4;
  Vector3i local_E8;
  float local_E4;
  float local_E0;
  float local_DC;
  if (*((int8_t *)&task_B2 + arg1) == 0) {
    *((int8_t *)&task_B2 + arg1) = 1;
  } else {
    Script_EVT_GetPosition(arg0, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
    Script_mtwkGetSetRotation(arg0, &local_E8, 0);
    local_E4 = Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_E8)->y);
    local_E0 = Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_E8)->y);
    SCNF_WaitFrames_0000(1);
    while (Script_IsCharaMotionFinished(arg0) == 0) {
      SCNF_WaitFrames_0000(1);
    }
    local_F4.x = (&local_F4)->x - local_E4 * 0.6f;
    (&local_F4)->y = (&local_F4)->y - 0.45f;
    (&local_F4)->z = (&local_F4)->z - local_E0 * 0.6f;
    local_DC = Script_GetHighestCollisionBelow(arg0, &local_F4);
    if (!(local_DC < (&local_F4)->y - 0.1f)) {
      (&local_F4)->y = local_DC;
      Script_EVT_SetPosition(arg0, 0x78000000, &local_F4);
    } else {
      Script_sprintf("KAIDAN : Warning coli!\012");
    }
  }
  return;
}

__attribute__((scn_string_bindings(0, data_121DF)))
static void script_29E5(int32_t arg0, int32_t arg1) {
  Vector3 local_F4;
  Vector3i local_E8;
  float local_E4;
  float local_E0;
  float local_DC;
  if (*((int8_t *)&task_B2 + arg1) == 0) {
    *((int8_t *)&task_B2 + arg1) = 1;
  } else {
    Script_EVT_GetPosition(arg0, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
    Script_mtwkGetSetRotation(arg0, &local_E8, 0);
    local_E4 = Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_E8)->y);
    local_E0 = Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_E8)->y);
    SCNF_WaitFrames_0000(1);
    while (Script_IsCharaMotionFinished(arg0) == 0) {
      SCNF_WaitFrames_0000(1);
    }
    local_F4.x = (&local_F4)->x - local_E4 * 1.2f;
    (&local_F4)->y = (&local_F4)->y - 0.9f;
    (&local_F4)->z = (&local_F4)->z - local_E0 * 1.2f;
    local_DC = Script_GetHighestCollisionBelow(arg0, &local_F4);
    if (!(local_DC < (&local_F4)->y - 0.1f)) {
      (&local_F4)->y = local_DC;
      Script_EVT_SetPosition(arg0, 0x78000000, &local_F4);
    } else {
      Script_sprintf("KAIDAN : Warning coli!\012");
    }
  }
  return;
}

__attribute__((scn_string_bindings(0, data_121F7)))
static int32_t script_2ADB(int32_t arg0, int32_t arg1) {
  float local_FC;
  float local_F8;
  if (*((int8_t *)&task_B1 + arg1)) {
    Script_sprintf("KAIDAN : Warning setY!\012");
  } else {
    *((int8_t *)&task_B1 + arg1) = 1;
    if (task_B5) {
      local_FC = local_F8 = -0.001f;
    } else {
      local_FC = local_F8 = 0.0f;
    }
    while (*((int8_t *)&task_B1 + arg1) == 1) {
      if (task_B5) {
        if (!(local_F8 <= -0.16f)) {
          local_FC = local_F8 - 0.001f;
        } else {
          local_FC = -0.001f;
        }
      } else if (!(local_F8 >= 0.0f)) {
        local_FC = local_F8 + 0.001f;
      } else {
        local_FC = 0.0f;
      }
      Script_KaidanSetY(arg0, local_FC);
      local_F8 = local_FC;
      SCNF_WaitFrames_0000(1);
    }
    if (*((int8_t *)&task_B1 + arg1) != 2) {
      for (;;) {
        if (local_FC >= 0.0f) {
          break;
        }
        if (*((int8_t *)&task_B1 + arg1) == 2) {
          *((int8_t *)&task_B1 + arg1) = 0;
          Script_KaidanSetY(arg0, 0.0f);
          break;
        }
        local_FC = local_FC + 0.004f;
        Script_KaidanSetY(arg0, local_FC);
        SCNF_WaitFrames_0000(1);
      }
    } else {
      *((int8_t *)&task_B1 + arg1) = 0;
      Script_KaidanSetY(arg0, 0.0f);
    }
  }
  end:;
}

__attribute__((scn_string_bindings(0, data_1220F)))
static int32_t script_2C03(int32_t arg0, int32_t arg1) {
  float local_FC;
  float local_F8;
  if (*((int8_t *)&task_B1 + arg1)) {
    Script_sprintf("KAIDAN : Warning setY!\012");
  } else {
    *((int8_t *)&task_B1 + arg1) = 1;
    if (task_B5) {
      local_FC = local_F8 = 0.16f;
    } else {
      local_FC = local_F8 = 0.08f;
    }
    while (*((int8_t *)&task_B1 + arg1) == 1) {
      if (task_B5) {
        if (!(local_F8 >= 0.16f)) {
          local_FC = local_F8 + 0.004f;
        } else {
          local_FC = 0.16f;
        }
      } else if (!(local_F8 <= 0.08f)) {
        local_FC = local_F8 - 0.004f;
      } else {
        local_FC = 0.08f;
      }
      Script_KaidanSetY(arg0, local_FC);
      local_F8 = local_FC;
      SCNF_WaitFrames_0000(1);
    }
    if (*((int8_t *)&task_B1 + arg1) != 2) {
      for (;;) {
        if (local_FC <= 0.0f) {
          break;
        }
        if (*((int8_t *)&task_B1 + arg1) == 2) {
          *((int8_t *)&task_B1 + arg1) = 0;
          Script_KaidanSetY(arg0, 0.0f);
          break;
        }
        local_FC = local_FC - 0.004f;
        Script_KaidanSetY(arg0, local_FC);
        SCNF_WaitFrames_0000(1);
      }
    } else {
      *((int8_t *)&task_B1 + arg1) = 0;
      Script_KaidanSetY(arg0, 0.0f);
    }
  }
  end:;
}

static void script_2D34(int32_t arg0, int32_t arg1, int32_t arg2) {
  if ((*(&task_78 + arg1) & 2) > 0) {
    if (Script_ryoControl(PCCTL_QueryShouldRun)) {
      Script_sprintf(data_12227);
      Script_ryoControl(PCCTL_DisablePlayerControl);
      (void)((task_BC = -1) & 4);
    }
    Script_ccowUtils(arg0, 6);
  }
  if (*(&task_78 + arg1) & 0x20) {
    Script_ccowUtils(arg0, 4);
  } else {
    if ((arg2 & 0x100) | (arg2 & 0x4000) | ((arg2 & 0x8000) > 0)) {
      Script_ccowUtils(arg0, 4);
      goto L002DB8;
    }
  }
  L002DB8:;
  return;
}

static void script_2DB9(int32_t arg0, int32_t arg1, int32_t arg2) {
  if (*(&task_78 + arg1) & 0x20) {
    Script_ccowUtils(arg0, 3);
  } else if ((arg2 & 0x8000) > 0) {
  } else {
    if ((arg2 & 0x100) | ((arg2 & 0x4000) > 0)) {
      Script_ccowUtils(arg0, 3);
      goto L002E08;
    }
  }
  L002E08:;
  return;
}

static void script_2E09(int32_t arg0, int32_t arg1, int32_t arg2) {
  Script_ccowUtils(arg0, 5);
  if (*(&task_78 + arg1) & 0x20) {
    Script_ccowUtils(arg0, 3);
  } else {
    if ((arg2 & 0x100) | (arg2 & 0x4000) | ((arg2 & 0x8000) > 0)) {
      Script_ccowUtils(arg0, 3);
      goto L002E57;
    }
  }
  L002E57:;
  return;
}

static void script_2E58(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
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

static void script_31EE(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
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

static void script_387C(int32_t arg0, float arg1, float arg2, float arg3) {
  Vector3 local_F4;
  local_F4.x = arg1;
  (&local_F4)->y = arg2;
  (&local_F4)->z = arg3;
  Script_EVT_SetPosition(arg0, POS_XYZ, &local_F4);
  return;
}

static void script_38A4(int32_t arg0, float arg1, float arg2, float arg3) {
  Vector3i local_F4;
  local_F4.x = (int32_t)(arg1 / 360.0f * (float)0x10000);
  (&local_F4)->y = (int32_t)(arg2 / 360.0f * (float)0x10000);
  (&local_F4)->z = (int32_t)(arg3 / 360.0f * (float)0x10000);
  Script_EVT_SetRotation(arg0, 0x38000000, &local_F4, 0);
  return;
}

static void script_38F8(int32_t arg0, int32_t arg1, int32_t arg2) {
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

static int32_t script_3948(int32_t arg0) {
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

static void script_39EF(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  local_FC = script_3948(arg0);
  if (arg1 > local_FC) {
    (void)0;
  } else {
    (void)-1;
  }
  return;
}

static int32_t script_3A12(int32_t arg0) {
  int32_t unused_FC;
  GameDateTime local_F4;
  int32_t local_F0;
  int32_t local_EC;
  Script_ReturnTimeDate(&local_F4);
  local_F0 = (&local_F4)->hours * 60 + (&local_F4)->minutes;
  local_EC = Script_scenUtils(SCENARIO_CMD_GET_STEP_LUT, arg0) * 6;
  return local_F0 - local_EC;
}

static void script_3A45(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  local_FC = script_3A12(arg0);
  if (arg1 > local_FC) {
    (void)0;
  } else {
    (void)-1;
  }
  return;
}

static void script_3A68(int32_t arg0, int32_t arg1) {
  int8_t local_F4[12];
  int32_t local_F0;
  local_F0 = script_3A12(arg0);
  local_F4[4] = local_F0 / 60;
  local_F4[5] = local_F0 % 60;
  if ((local_F4[4] < *(int8_t *)(arg1 + 4)) | ((local_F4[4] == *(int8_t *)(arg1 + 4)) & (local_F4[5] < *(int8_t *)(arg1 + 5)))) {
    (void)0;
  } else {
    (void)-1;
  }
  return;
}

static void script_3AC7(int32_t arg0) {
  int32_t local_F8[2];
  local_F8[0] = arg0;
  local_F8[1] = 0;
  (void)local_F8;
  return;
}

__attribute__((scn_string_bindings(0, data_12244)))
static void script_3ADC(void) {
  Script_sprintf("TUNED FreeTalk def\012");
  SCNT_Enqueue();
  return;
}

__attribute__((scn_string_bindings(0, data_12258)))
static void script_3AEB(void) {
  Script_sprintf("TUNED FreeTalk def\012");
  SCNT_Enqueue();
  return;
}

static void script_3AFA(void) {
  return;
}

static void script_3AFD(void) {
  return;
}

static void script_3B00(void) {
  return;
}

static void script_3B03(void) {
  return;
}

static void script_3B06(void) {
  (void)((task_D4 = 0) & 0x20);
  return;
}

static void script_3B13(void) {
  task_C8 = Script_freeTalkGetCurrentChara();
  return;
}

static void script_3B1F(void) {
  *(&task_C8 + 1) = Script_GetFreeTalkStatus();
  return;
}

static int32_t script_3B2E(void) {
  return *(&task_C8 + 2) = Script_freeTalkGetAndResetState();
}

static int32_t script_3B3D(int32_t arg0) {
  if (arg0 < 0) {
    return -arg0;
  } else {
    return arg0;
  }
}

static void script_3B5B(float arg0) {
  if (!(arg0 >= 0.0f)) {
    (void)((float)0 - arg0);
    return;
  } else {
    (void)arg0;
    return;
  }
}

static int32_t script_3B7A(int32_t arg0, int32_t arg1) {
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

static void script_3BF2(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  int32_t local_FC;
  if (task_238 != arg0) {
    Script_sprintf("Door Index = %d : call_type = %d : type = %d\012", task_120[arg2], arg3, arg4);
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

static int32_t script_41A4(int32_t arg0) {
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

static void script_4275(int32_t arg0, int32_t arg1, int32_t arg2) {
  script_CF98(arg0, arg2, 0);
  return;
}

static void script_4285(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  float local_F8;
  float local_F4;
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
    local_FC = script_C72D(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
    if ((local_FC == 1) | (local_FC == 2)) {
      local_FC = 1;
    } else {
      local_FC = 0;
    }
    *(int32_t *)&local_F8 = Script_ScenDataUtil(data_1285C, 5, Script_ScenDataUtil(data_1226C, 1, arg3 * 40 + task_170[arg2] * 4 + local_FC) * 9 + 5);
    *(int32_t *)&local_F4 = Script_ScenDataUtil(data_1285C, 5, Script_ScenDataUtil(data_1226C, 1, arg3 * 40 + task_170[arg2] * 4 + local_FC) * 9 + 6);
    local_FC = script_C72D(arg0, arg1, 5, local_F8 / 100.0f, local_F4 / 100.0f, arg3, -1);
    script_7A11(arg0, arg1, arg2, arg3, local_FC, 0);
  }
  script_CF98(arg0, arg2, 0);
  return;
}

static void script_4389(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  int32_t local_F8;
  float local_F4;
  float local_F0;
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
    local_FC = script_C72D(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
    *(int32_t *)&local_F4 = Script_ScenDataUtil(data_1285C, 5, Script_ScenDataUtil(data_1226C, 1, arg3 * 40 + task_170[arg2] * 4 + local_FC) * 9 + 5);
    local_F4 = 90.0f - local_F4;
    *(int32_t *)&local_F0 = Script_ScenDataUtil(data_1285C, 5, Script_ScenDataUtil(data_1226C, 1, arg3 * 40 + task_170[arg2] * 4 + local_FC) * 9 + 6);
    if ((Script_ScenDataUtil(task_D8, 4, task_120[arg2] * 13 + 1) & 0x40) | (Script_ScenDataUtil(task_D8, 4, task_120[arg2] * 13 + 1) & 0x80)) {
      if (Script_ScenDataUtil(task_D8, 4, task_120[arg2] * 13 + 1) & 0x40) {
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
    local_FC = script_C72D(arg0, arg1, local_F8, local_F4 / (float)100, local_F0 / (float)100, arg3, task_120[arg2]);
    script_7A11(arg0, arg1, arg2, arg3, local_FC, 1);
  }
  script_CF98(arg0, arg2, 0);
  return;
}

static void script_452B(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  float local_F8;
  float local_F4;
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
    local_FC = script_C72D(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
    if ((local_FC == 1) | (local_FC == 2)) {
      local_FC = 1;
    } else {
      local_FC = 0;
    }
    *(int32_t *)&local_F8 = Script_ScenDataUtil(data_1285C, 5, Script_ScenDataUtil(data_1226C, 1, arg3 * 40 + task_170[arg2] * 4 + local_FC) * 9 + 5);
    *(int32_t *)&local_F4 = Script_ScenDataUtil(data_1285C, 5, Script_ScenDataUtil(data_1226C, 1, arg3 * 40 + task_170[arg2] * 4 + local_FC) * 9 + 6);
    local_FC = script_C72D(arg0, arg1, 5, local_F8 / 100.0f, local_F4 / 100.0f, arg3, -1);
    if (local_FC == 1) {
      local_FC = 2;
    } else {
      local_FC = 3;
    }
    if ((Script_ScenDataUtil(task_D8, 4, task_120[arg2] * 13 + 1) & 4) == 0) {
      script_5F1E(arg0, arg1, arg2, arg3, local_FC, 0);
    } else {
      if (arg0 == task_238) {
        script_92F1(arg0, arg1, arg2, arg3, local_FC, 0);
      }
    }
  }
  script_CF98(arg0, arg2, 2);
  return;
}

static void script_468C(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  float local_F8;
  float local_F4;
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
    local_FC = script_C72D(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
    if ((local_FC == 1) | (local_FC == 2)) {
      local_FC = 1;
    } else {
      local_FC = 0;
    }
    *(int32_t *)&local_F8 = Script_ScenDataUtil(data_1285C, 5, Script_ScenDataUtil(data_1226C, 1, arg3 * 40 + task_170[arg2] * 4 + local_FC) * 9 + 5);
    *(int32_t *)&local_F4 = Script_ScenDataUtil(data_1285C, 5, Script_ScenDataUtil(data_1226C, 1, arg3 * 40 + task_170[arg2] * 4 + local_FC) * 9 + 6);
    local_FC = script_C72D(arg0, arg1, 5, local_F8 / 100.0f, local_F4 / 100.0f, arg3, -1);
    if (local_FC == 1) {
      local_FC = 1;
    } else {
      local_FC = 0;
    }
    if ((Script_ScenDataUtil(task_D8, 4, task_120[arg2] * 13 + 1) & 4) == 0) {
      script_5F1E(arg0, arg1, arg2, arg3, local_FC, 0);
    } else {
      if (arg0 == task_238) {
        script_92F1(arg0, arg1, arg2, arg3, local_FC, 0);
      }
    }
  }
  script_CF98(arg0, arg2, 2);
  return;
}

static void script_47ED(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  int32_t local_F8;
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
    if ((Script_ScenDataUtil(task_D8, 4, task_120[arg2] * 13 + 1) & 4) == 0) {
      float local_F4;
      float local_F0;
      local_FC = script_C72D(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
      *(int32_t *)&local_F4 = Script_ScenDataUtil(data_1285C, 5, Script_ScenDataUtil(data_1226C, 1, arg3 * 40 + task_170[arg2] * 4 + local_FC) * 9 + 5);
      *(int32_t *)&local_F0 = Script_ScenDataUtil(data_1285C, 5, Script_ScenDataUtil(data_1226C, 1, arg3 * 40 + task_170[arg2] * 4 + local_FC) * 9 + 6);
      if ((Script_ScenDataUtil(task_D8, 4, task_120[arg2] * 13 + 1) & 0x40) | (Script_ScenDataUtil(task_D8, 4, task_120[arg2] * 13 + 1) & 0x80)) {
        if (Script_ScenDataUtil(task_D8, 4, task_120[arg2] * 13 + 1) & 0x40) {
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
      local_FC = script_C72D(arg0, arg1, local_F8, local_F4 / 100.0f, local_F0 / 100.0f, arg3, task_120[arg2]);
      script_5F1E(arg0, arg1, arg2, arg3, local_FC, 1);
    } else {
      if (arg0 == task_238) {
        float local_F4;
        float local_F0;
        local_FC = script_C72D(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
        *(int32_t *)&local_F4 = Script_ScenDataUtil(data_1285C, 5, Script_ScenDataUtil(data_1226C, 1, arg3 * 40 + task_170[arg2] * 4 + local_FC) * 9 + 5);
        *(int32_t *)&local_F0 = Script_ScenDataUtil(data_1285C, 5, Script_ScenDataUtil(data_1226C, 1, arg3 * 40 + task_170[arg2] * 4 + local_FC) * 9 + 6);
        local_FC = script_C72D(arg0, arg1, 6, local_F4 / 100.0f, local_F0 / 100.0f, arg3, task_120[arg2]);
        script_92F1(arg0, arg1, arg2, arg3, local_FC, 1);
      }
    }
  }
  script_CF98(arg0, arg2, 2);
  return;
}

static void script_4A8B(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  float local_F8;
  float local_F4;
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
    local_FC = script_C72D(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
    *(int32_t *)&local_F8 = Script_ScenDataUtil(data_1285C, 5, Script_ScenDataUtil(data_1226C, 1, arg3 * 40 + task_170[arg2] * 4 + local_FC) * 9 + 5);
    *(int32_t *)&local_F4 = Script_ScenDataUtil(data_1285C, 5, Script_ScenDataUtil(data_1226C, 1, arg3 * 40 + task_170[arg2] * 4 + local_FC) * 9 + 6);
    script_C72D(arg0, arg1, 0, local_F8 / 100.0f, local_F4 / 100.0f, arg3, -1);
  }
  script_CF98(arg0, arg2, 0);
  return;
}

static void script_4B59(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
    local_FC = script_C72D(arg0, arg1, 4, 0.5f, 0.5f, arg3, -1);
    if (local_FC == 1) {
      local_FC = 2;
    } else {
      local_FC = 3;
    }
    script_6E9D(arg0, arg1, arg2, arg3, local_FC, 1);
  }
  script_CF98(arg0, arg2, 1);
  return;
}

static void script_4BC0(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
    local_FC = script_C72D(arg0, arg1, 4, 0.5f, 0.5f, arg3, -1);
    if (local_FC == 1) {
      local_FC = 1;
    } else {
      local_FC = 0;
    }
    script_6E9D(arg0, arg1, arg2, arg3, local_FC, 1);
  }
  script_CF98(arg0, arg2, 1);
  return;
}

static void script_4C27(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
    script_C72D(arg0, arg1, 4, 0.0f, 0.5f, arg3, -1);
    script_6E9D(arg0, arg1, arg2, arg3, 4, 1);
  }
  script_CF98(arg0, arg2, 1);
  return;
}

static void script_4C6E(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  float local_F8;
  float local_F4;
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
    local_FC = script_C72D(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
    if ((local_FC == 1) | (local_FC == 2)) {
      local_FC = 1;
    } else {
      local_FC = 0;
    }
    *(int32_t *)&local_F8 = Script_ScenDataUtil(data_1285C, 5, Script_ScenDataUtil(data_1226C, 1, arg3 * 40 + task_170[arg2] * 4 + local_FC) * 9 + 5);
    *(int32_t *)&local_F4 = Script_ScenDataUtil(data_1285C, 5, Script_ScenDataUtil(data_1226C, 1, arg3 * 40 + task_170[arg2] * 4 + local_FC) * 9 + 6);
    if (local_FC == 0) {
      local_FC = script_C72D(arg0, arg1, 5, local_F8 / 100.0f, local_F4 / 100.0f, arg3, -1);
    } else {
      local_FC = script_C72D(arg0, arg1, 5, local_F8 / 100.0f, local_F4 / 100.0f, arg3, -1);
    }
    script_7357(arg0, arg1, arg2, arg3, local_FC, 0);
  }
  script_CF98(arg0, arg2, 2);
  return;
}

static void script_4DA4(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
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
  if (arg0 == task_238) {
    if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
      local_FF7F = script_C72D(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
      *(int32_t *)&local_FF78 = Script_ScenDataUtil(data_1285C, 5, Script_ScenDataUtil(data_1226C, 1, arg3 * 40 + task_170[arg2] * 4 + local_FF7F) * 9 + 5);
      *(int32_t *)&local_FF74 = Script_ScenDataUtil(data_1285C, 5, Script_ScenDataUtil(data_1226C, 1, arg3 * 40 + task_170[arg2] * 4 + local_FF7F) * 9 + 6);
      local_FF7F = script_C72D(arg0, arg1, 5, local_FF78 / (float)100, local_FF74 / (float)100, arg3, -1);
      script_CDF6(arg0, 0);
      *(int32_t *)&local_94 = Script_ScenDataUtil(data_1285C, 5, Script_ScenDataUtil(data_1226C, 1, arg3 * 40 + task_170[arg2] * 4 + local_FF7F) * 9 + 1);
      *(int32_t *)&local_90 = Script_ScenDataUtil(data_1285C, 5, Script_ScenDataUtil(data_1226C, 1, arg3 * 40 + task_170[arg2] * 4 + local_FF7F) * 9 + 2);
      local_8C = 0.0f;
      local_FC = Script_ScenDataUtil(data_1285C, 4, Script_ScenDataUtil(data_1226C, 1, arg3 * 40 + task_170[arg2] * 4 + local_FF7F) * 9);
      local_FC = local_FC | ((Script_FloatUtils(ScriptMathOp_FloatToInt, local_94 - 3.0f) << 16) & 0x7FFF0000);
      local_9C.x = (&task_240)->x;
      (&local_9C)->y = task_240.y;
      (&local_9C)->z = task_240.z;
      Script_charaPosRotUtils(arg1, 12, &local_CC, &local_E4, 2);
      Script_charaPosRotUtils(arg1, 7, &local_C0, &local_D8, 2);
      Script_ryoControl(PCCTL_StartCameraFacingInterpolation, 0);
      if (arg0 == task_238) {
        Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
        Script_scenCharaSubmitMoveTargetRequest(arg0, &task_240, *(&task_24C + 1), local_FC, 0);
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
        Script_SetCharaMoveTarget(arg0, &task_240, *(&task_24C + 1), local_FC);
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
              local_FF6C = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_120[arg2], 2);
              local_FF6C = Script_ScenDataUtil(task_E0, 4, Script_ScenDataUtil(task_E4, 4, local_FF6C * 9 + 1) * 4 + 2);
              if (local_FF6C != -1) {
                if (arg0 == task_238) {
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
              local_FF6C = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_120[arg2], 2);
              local_FF6C = Script_ScenDataUtil(task_E0, 4, Script_ScenDataUtil(task_E4, 4, local_FF6C * 9 + 1) * 4 + 2);
              if (local_FF6C != -1) {
                if (arg0 == task_238) {
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
      script_CDF6(arg0, 1);
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
        local_FF6C = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_120[arg2], 2);
        local_FF6C = Script_ScenDataUtil(task_E0, 4, Script_ScenDataUtil(task_E4, 4, local_FF6C * 9 + 1) * 4 + 3);
        if (local_FF6C != -1) {
          if (arg0 == task_238) {
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
    script_CF98(arg0, arg2, 0);
  } else {
    script_4389(arg0, arg1, arg2, arg3);
  }
  return;
}

static void script_5665(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  float local_F8;
  int32_t local_F4;
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
    local_FC = script_C72D(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
    *(int32_t *)&local_F8 = Script_ScenDataUtil(data_1285C, 5, Script_ScenDataUtil(data_1226C, 1, arg3 * 40 + task_170[arg2] * 4 + local_FC) * 9 + 5);
    local_F8 = 98.0f - local_F8;
    local_F4 = Script_ScenDataUtil(data_1285C, 5, Script_ScenDataUtil(data_1226C, 1, arg3 * 40 + task_170[arg2] * 4 + local_FC) * 9 + 6);
    if ((local_FC == 0) | (local_FC == 3)) {
      local_FC = script_C72D(arg0, arg1, 4, 0.34736f, 0.73184f, arg3, -1);
    } else {
      local_FC = script_C72D(arg0, arg1, 4, 0.4f, 0.712f, arg3, -1);
    }
    script_9A12(arg0, arg1, arg2, arg3, local_FC, 0);
  }
  script_CF98(arg0, arg2, 0);
  return;
}

static int32_t script_5783(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
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
    local_F4 = Script_ScenDataUtil(task_D8, 4, task_120[arg2] * 13 + 6);
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
        local_FC = script_C72D(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
        local_FC = script_C72D(arg0, arg1, 7, 0.45f, 0.0f, arg3, -1);
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
              goto L00593F;
            }
          }
          L00593F:;
        } else {
          local_D4 = 0.45f;
        }
        Script_EVT_GetPosition(arg1, -1, &local_E4, 0);
        Script_mtwkGetSetRotation(arg1, &local_D8, 0);
        local_E4.x = (&local_E4)->x + local_D4 * Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_D8)->y & 0xFFFF);
        (&local_E4)->z = (&local_E4)->z - local_D4 * Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_D8)->y & 0xFFFF);
        *(&task_24C + 1) = Script_FloatUtils(ScriptMathOp_Atan2ReverseVector, (&local_E4)->x - (&task_240)->x, (&local_E4)->z - task_240.z);
        script_A19C(arg0, arg1, arg2, arg3, local_FC, local_F0);
      }
    } else if ((local_BC == 3) | (local_BC == 4) | (local_BC == 5) | (local_BC == 6)) {
      *(int32_t *)&local_D0 = Script_ScenDataUtil(&data_1327C, 5, Script_ScenDataUtil(task_D8, 4, task_120[arg2] * 13 + 6) * 5 + 3);
      *(int32_t *)&local_CC = Script_ScenDataUtil(&data_1327C, 5, Script_ScenDataUtil(task_D8, 4, task_120[arg2] * 13 + 6) * 5 + 4);
      local_FC = script_C72D(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
      local_BC = arg3;
      if ((local_BC == 13) | (local_BC == 14) | (local_BC == 15) | (local_BC == 16) | (local_BC == 20) | (local_BC == 23) | (local_BC == 28) | (local_BC == 29)) {
        if ((Script_ScenDataUtil(task_D8, 4, task_120[arg2] * 13 + 1) & 0x40) | (Script_ScenDataUtil(task_D8, 4, task_120[arg2] * 13 + 1) & 0x80)) {
          if (Script_ScenDataUtil(task_D8, 4, task_120[arg2] * 13 + 1) & 0x40) {
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
      local_FC = script_C72D(arg0, arg1, local_F8, local_D0 / (float)100, local_CC / (float)100, arg3, task_120[arg2]);
      script_A274(arg0, arg1, arg2, arg3, local_FC, 0);
    } else {
      if (local_BC == 7) {
        local_FC = script_C72D(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
        local_BC = arg3;
        if ((local_BC == 13) | (local_BC == 14) | (local_BC == 15) | (local_BC == 16) | (local_BC == 20) | (local_BC == 23) | (local_BC == 28) | (local_BC == 29)) {
          if ((Script_ScenDataUtil(task_D8, 4, task_120[arg2] * 13 + 1) & 0x40) | (Script_ScenDataUtil(task_D8, 4, task_120[arg2] * 13 + 1) & 0x80)) {
            if (Script_ScenDataUtil(task_D8, 4, task_120[arg2] * 13 + 1) & 0x40) {
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
        if (Script_CycleManagerUtils(CYCLE_CMD_IS_DOOR_OR_MOBJ_TYPE2, Script_ScenDataUtil(task_D8, 4, task_120[arg2] * 13 + 12)) == 1) {
          *(int32_t *)&local_D0 = Script_ScenDataUtil(&data_1327C, 5, Script_ScenDataUtil(task_D8, 4, task_120[arg2] * 13 + 6) * 5 + 3);
          *(int32_t *)&local_CC = Script_ScenDataUtil(&data_1327C, 5, Script_ScenDataUtil(task_D8, 4, task_120[arg2] * 13 + 6) * 5 + 4);
          local_FC = script_C72D(arg0, arg1, local_F8, local_D0 / (float)100, local_CC / (float)100, arg3, task_120[arg2]);
          Script_scenUtils(SCENARIO_CMD_INIT_SCNT);
          script_A6EB(arg0, arg1, arg2, arg3, local_FC, 0);
          Script_scenUtils(SCENARIO_CMD_UPDATE_TIMER);
        } else {
          *(int32_t *)&local_D0 = Script_ScenDataUtil(&data_1327C, 5, Script_ScenDataUtil(task_D8, 4, task_120[arg2] * 13 + 6) * 5 + 3);
          *(int32_t *)&local_CC = Script_ScenDataUtil(&data_1327C, 5, Script_ScenDataUtil(task_D8, 4, task_120[arg2] * 13 + 6) * 5 + 4);
          local_FC = script_C72D(arg0, arg1, local_F8, local_D0 / (float)100, local_CC / (float)100, arg3, task_120[arg2]);
          script_A274(arg0, arg1, arg2, arg3, local_FC, 0);
        }
        goto L005E3A;
      }
    }
  }
  L005E3A:;
  script_CF98(arg0, arg2, 0);
  end:;
}

static void script_5E48(int32_t arg0, int32_t arg1) {
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
    Script_SetCharaMoveTarget(arg0, &local_F4, (&local_E8)->y, script_41A4(arg1));
  }
  return;
}

__attribute__((scn_string_bindings(0, data_13D9A)))
static void script_5F1E(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
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
  *(int32_t *)&local_98 = Script_ScenDataUtil(data_1285C, 5, Script_ScenDataUtil(data_1226C, 1, arg3 * 40 + task_170[arg2] * 4 + arg4) * 9 + 1);
  *(int32_t *)&local_94 = Script_ScenDataUtil(data_1285C, 5, Script_ScenDataUtil(data_1226C, 1, arg3 * 40 + task_170[arg2] * 4 + arg4) * 9 + 2);
  *(int32_t *)&local_90 = Script_ScenDataUtil(data_1285C, 5, Script_ScenDataUtil(data_1226C, 1, arg3 * 40 + task_170[arg2] * 4 + arg4) * 9 + 3);
  *(int32_t *)&local_8C = Script_ScenDataUtil(data_1285C, 5, Script_ScenDataUtil(data_1226C, 1, arg3 * 40 + task_170[arg2] * 4 + arg4) * 9 + 4);
  *(int32_t *)&local_88 = Script_ScenDataUtil(data_1285C, 5, Script_ScenDataUtil(data_1226C, 1, arg3 * 40 + task_170[arg2] * 4 + arg4) * 9 + 7);
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
      local_FF78 = Script_ScenDataUtil(data_1285C, 4, Script_ScenDataUtil(data_1226C, 1, arg3 * 40 + task_170[arg2] * 4 + arg4) * 9);
      local_FF7C = 18;
      local_80 = 18;
      local_A4 = 1.0f;
    } else {
      if (arg0 == task_238) {
        local_FF78 = Script_ScenDataUtil(data_1285C, 4, Script_ScenDataUtil(data_1226C, 1, arg3 * 40 + task_170[arg2] * 4 + arg4) * 9) | -0x80000000;
        local_FF7C = 12;
        local_80 = 12;
      } else {
        local_FF78 = Script_ScenDataUtil(data_1285C, 4, Script_ScenDataUtil(data_1226C, 1, arg3 * 40 + task_170[arg2] * 4 + arg4) * 9);
        local_FF7C = 18;
        local_80 = 18;
      }
      local_A4 = -1.0f;
    }
    script_3BF2(arg0, local_FF78, arg2, arg3, arg4);
    local_FF78 = local_FF78 | ((Script_FloatUtils(ScriptMathOp_FloatToInt, local_98 - 3.0f) << 16) & 0x7FFF0000);
    script_CDF6(arg0, 0);
    local_C0.x = (&task_240)->x;
    (&local_C0)->y = task_240.y;
    (&local_C0)->z = task_240.z;
    Script_ryoControl(PCCTL_StartCameraFacingInterpolation, 0);
    Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
    Script_SetCharaMoveTarget(arg0, &task_240, *(&task_24C + 1), local_FF78);
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
    if (task_238 == arg0) {
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
          script_5E48(arg0, task_170[arg2]);
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
            local_FF64 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_120[arg2], 2);
            local_FF64 = Script_ScenDataUtil(task_E0, 4, Script_ScenDataUtil(task_E4, 4, local_FF64 * 9 + 1) * 4 + 2);
            if (local_FF64 != -1) {
              if (arg0 == task_238) {
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
                goto L0064A6;
              }
            }
            L0064A6:;
          } else {
            if ((local_FF64 == 1) | (local_FF64 == 3)) {
              if (!(local_B4 > 0.0f)) {
                local_B4 = 0.0f;
              } else {
                if (!(local_B4 < 0.75f)) {
                  local_B4 = 0.75f;
                  goto L0064E4;
                }
              }
              L0064E4:;
              goto L0064E7;
            }
          }
        }
      }
      L0064E7:;
      if (task_238 == arg0) {
        if (!(local_B8 <= local_94) & !(local_B8 >= local_90)) {
          local_86 = 0;
        }
      } else {
        if (!(local_B8 <= local_94) & !(local_B8 > local_88)) {
          *(int32_t *)&local_9C = Script_ScenDataUtil(data_1285C, 5, Script_ScenDataUtil(data_1226C, 1, arg3 * 40 + task_170[arg2] * 4 + arg4) * 9 + 8);
          local_B4 = local_B4 + local_9C * local_A4;
        }
      }
      if (!(local_B8 < local_90) & !(local_B8 >= local_90 + 1.5f)) {
        if (local_84 == 1) {
          int32_t local_FF64;
          local_FF64 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_120[arg2], 2);
          local_FF64 = Script_ScenDataUtil(task_E0, 4, Script_ScenDataUtil(task_E4, 4, local_FF64 * 9 + 1) * 4 + 3);
          if (local_FF64 != -1) {
            if (arg0 == task_238) {
              Script_executeAicaCommand(local_FF64, 0, 0);
            } else {
              Script_Execute3DSoundCommand_Wrapper(local_FF64, 4, &local_C0, 5.0f, 20.0f, 0);
            }
          }
          local_84 = 2;
        }
        if (task_238 != arg0) {
          if (local_FF74 == 0) {
            local_AC = local_B4 / (local_8C - local_90);
            local_FF74 = 1;
          }
        }
      }
      if (!(local_B8 < local_90) & !(local_B8 > local_8C)) {
        Script_EVT_GetPosition(arg0, local_FF7C, &local_F0, ENTITY_XFORM_MOTION_SPACE);
        if (task_238 == arg0) {
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
                goto L0066B3;
              }
            }
            L0066B3:;
          } else {
            if ((local_FF64 == 1) | (local_FF64 == 3)) {
              if (!(local_B4 < 0.75f)) {
                local_B4 = 0.75f;
              } else {
                if (!(local_B4 > 0.0f)) {
                  local_B4 = 0.0f;
                  goto L0066F1;
                }
              }
              L0066F1:;
              goto L0066F4;
            }
          }
          L0066F4:;
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
              goto L00674D;
            }
          }
        }
      }
      L00674D:;
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
      if (arg0 == task_238) {
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
  script_CDF6(arg0, 1);
  {
    int32_t local_FF78;
    int32_t unused_FF68[4];
    int32_t local_FF64;
    local_FF78 = task_120[arg2];
    local_FF64 = arg3;
    if ((local_FF64 == 9) | (local_FF64 == 10) | (local_FF64 == 15)) {
      if (arg5) {
        Script_charaPosRotUtils(arg1, 12, &local_D8, 0, 0);
        Script_charaPosRotUtils(arg1, 7, &local_CC, 0, 0);
      } else {
        Script_EVT_SetPosition(arg1, POS_XYZ, &local_D8);
      }
      if (task_238 == arg0) {
        local_80 = (Script_ScenDataUtil(task_D8, 4, local_FF78 * 13 + 2) >> 16) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_80, 1);
        local_80 = Script_ScenDataUtil(task_D8, 4, local_FF78 * 13 + 2) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_80, 1);
      }
    } else if ((local_FF64 == 18) | (local_FF64 == 19) | (local_FF64 == 21) | (local_FF64 == 22)) {
      if (task_238 == arg0) {
        local_80 = (Script_ScenDataUtil(task_D8, 4, local_FF78 * 13 + 2) >> 16) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_80, 1);
        {
          int32_t local_FF60;
          local_FF60 = Script_ScenDataUtil(task_D8, 4, local_FF78 * 13 + 1);
          local_FF60 = local_FF60 | 8;
          Script_ScenDataUtil(task_D8, 14, local_FF78 * 13 + 1, local_FF60);
        }
        {
          int32_t local_FF60;
          local_FF60 = Script_ScenDataUtil(task_D8, 4, local_FF78 * 13 + 1);
          local_FF60 = local_FF60 | 0x10;
          Script_ScenDataUtil(task_D8, 14, local_FF78 * 13 + 1, local_FF60);
        }
        {
          int32_t local_FF60;
          local_FF60 = Script_ScenDataUtil(task_D8, 4, local_FF78 * 13 + 1);
          local_FF60 = local_FF60 | 2;
          Script_ScenDataUtil(task_D8, 14, local_FF78 * 13 + 1, local_FF60);
        }
      } else {
        if (arg5) {
          Script_charaPosRotUtils(arg1, 12, &local_D8, 0, 0);
          Script_charaPosRotUtils(arg1, 7, &local_CC, 0, 0);
        } else {
          Script_EVT_SetPosition(arg1, POS_XYZ, &local_D8);
        }
        local_80 = (Script_ScenDataUtil(task_D8, 4, local_FF78 * 13 + 2) >> 16) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_80, 1);
        local_80 = Script_ScenDataUtil(task_D8, 4, local_FF78 * 13 + 2) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_80, 1);
      }
    } else {
      if ((local_FF64 == 20) | (local_FF64 == 23)) {
        if (task_238 == arg0) {
          if ((arg4 == 0) | (arg4 == 1)) {
            {
              int32_t local_FF60;
              local_FF60 = Script_ScenDataUtil(task_D8, 4, local_FF78 * 13 + 1);
              local_FF60 = local_FF60 | 0x10;
              Script_ScenDataUtil(task_D8, 14, local_FF78 * 13 + 1, local_FF60);
            }
            {
              int32_t local_FF60;
              local_FF60 = Script_ScenDataUtil(task_D8, 4, local_FF78 * 13 + 1);
              local_FF60 = local_FF60 | 4;
              Script_ScenDataUtil(task_D8, 14, local_FF78 * 13 + 1, local_FF60);
            }
          } else {
            {
              int32_t local_FF60;
              local_FF60 = Script_ScenDataUtil(task_D8, 4, local_FF78 * 13 + 1);
              local_FF60 = local_FF60 | 8;
              Script_ScenDataUtil(task_D8, 14, local_FF78 * 13 + 1, local_FF60);
            }
            {
              int32_t local_FF60;
              local_FF60 = Script_ScenDataUtil(task_D8, 4, local_FF78 * 13 + 1);
              local_FF60 = local_FF60 | 4;
              Script_ScenDataUtil(task_D8, 14, local_FF78 * 13 + 1, local_FF60);
            }
          }
        } else {
          if (arg5) {
            Script_charaPosRotUtils(arg1, 12, &local_D8, 0, 0);
            Script_charaPosRotUtils(arg1, 7, &local_CC, 0, 0);
          } else {
            Script_EVT_SetPosition(arg1, POS_XYZ, &local_D8);
          }
          local_80 = (Script_ScenDataUtil(task_D8, 4, local_FF78 * 13 + 2) >> 16) & 0xFFFF;
          Script_scenToggleCharaFlag(0, local_80, 1);
          local_80 = Script_ScenDataUtil(task_D8, 4, local_FF78 * 13 + 2) & 0xFFFF;
          Script_scenToggleCharaFlag(0, local_80, 1);
        }
        goto L006E30;
      }
    }
  }
  L006E30:;
  return;
}

static void script_6E31(int32_t arg0, int32_t arg1, int32_t arg2) {
  int32_t local_FC;
  if (task_238 == arg0) {
    local_FC = task_120[arg1];
    {
      int32_t local_F8;
      local_F8 = Script_ScenDataUtil(task_D8, 4, local_FC * 13 + 2) & 0xFFFF;
      Script_scenToggleCharaFlag(0, local_F8, arg2);
    }
    {
      int32_t local_F8;
      local_F8 = (Script_ScenDataUtil(task_D8, 4, local_FC * 13 + 2) >> 16) & 0xFFFF;
      Script_scenToggleCharaFlag(0, local_F8, arg2);
    }
  }
  return;
}

static void script_6E9D(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
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
  local_AC.x = (&task_240)->x;
  (&local_AC)->y = task_240.y;
  (&local_AC)->z = task_240.z;
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
    local_80 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_120[arg2], 2);
    local_80 = Script_ScenDataUtil(task_E0, 4, Script_ScenDataUtil(task_E4, 4, local_80 * 9 + 1) * 4 + 2);
    if (local_80 != -1) {
      if (arg0 == task_238) {
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
        task_148[arg2] = arg0;
        local_98 = -local_98;
        local_84 = 0;
      }
      local_8C = 0;
    } else {
      if ((local_93 == 1) & (local_8C == 0)) {
        int32_t local_80;
        local_80 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_120[arg2], 2);
        local_80 = Script_ScenDataUtil(task_E0, 4, Script_ScenDataUtil(task_E4, 4, local_80 * 9 + 1) * 4 + 3);
        if (local_80 != -1) {
          if (arg0 == task_238) {
            Script_executeAicaCommand(local_80, 0, 0);
          } else {
            Script_Execute3DSoundCommand_Wrapper(local_80, 4, &local_AC, 5.0f, 20.0f, 0);
          }
        }
        local_93 = 0;
        local_8C = 1;
        local_98 = -local_98;
        local_84 = 0;
        task_148[arg2] = 0;
      }
    }
    local_88 = task_120[arg2];
    local_88 = Script_ScenDataUtil(task_D8, 4, local_88 * 13 + 2) & 0xFFFF;
    local_88 = local_88 + ((local_9C - 0x4000) >> 12);
    if (local_84 != local_88) {
      Script_scenToggleCharaFlag(0, local_88, local_8C);
      local_84 = local_88;
    }
    local_9C = local_9C - local_98;
    if (local_9C >= 0xC000) {
      local_9C = 0xC000;
      local_88 = task_120[arg2];
      local_88 = Script_ScenDataUtil(task_D8, 4, local_88 * 13 + 2) & 0xFFFF;
      local_88 = local_88 + 8;
      Script_scenToggleCharaFlag(0, local_88, local_8C);
      break;
    }
    if (local_9C <= 0x4000) {
      local_9C = 0x4000;
      if (local_93 == 0) {
        task_148[arg2] = 0;
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
          goto L007307;
        }
      }
    }
    L007307:;
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

__attribute__((scn_string_bindings(0, data_13DB6)))
static void script_7357(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
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
          local_FC = Script_ScenDataUtil(data_1285C, 4, Script_ScenDataUtil(data_1226C, 1, arg3 * 40 + task_170[arg2] * 4 + arg4) * 9);
          break;
        case 0:
          local_B7 = 0;
          local_F8 = 12;
          local_F0 = 0x4000;
          local_B8 = 1.0f;
          local_FC = Script_ScenDataUtil(data_1285C, 4, Script_ScenDataUtil(data_1226C, 1, arg3 * 40 + task_170[arg2] * 4 + arg4) * 9) | -0x80000000;
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
          local_FC = Script_ScenDataUtil(data_1285C, 4, Script_ScenDataUtil(data_1226C, 1, arg3 * 40 + task_170[arg2] * 4 + arg4) * 9) | -0x80000000;
          break;
        case 0:
          local_B7 = 1;
          local_F8 = 18;
          local_F0 = 0xC000;
          local_B8 = 1.0f;
          local_FC = Script_ScenDataUtil(data_1285C, 4, Script_ScenDataUtil(data_1226C, 1, arg3 * 40 + task_170[arg2] * 4 + arg4) * 9);
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
          local_FC = Script_ScenDataUtil(data_1285C, 4, Script_ScenDataUtil(data_1226C, 1, arg3 * 40 + task_170[arg2] * 4 + arg4) * 9) | -0x80000000;
          break;
        case 0:
          local_B7 = 0;
          local_F8 = 18;
          local_F0 = 0xC000;
          local_B8 = -1.0f;
          local_FC = Script_ScenDataUtil(data_1285C, 4, Script_ScenDataUtil(data_1226C, 1, arg3 * 40 + task_170[arg2] * 4 + arg4) * 9);
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
          local_FC = Script_ScenDataUtil(data_1285C, 4, Script_ScenDataUtil(data_1226C, 1, arg3 * 40 + task_170[arg2] * 4 + arg4) * 9);
          break;
        case 0:
          local_B7 = 1;
          local_F8 = 12;
          local_F0 = 0x4000;
          local_B8 = -1.0f;
          local_FC = Script_ScenDataUtil(data_1285C, 4, Script_ScenDataUtil(data_1226C, 1, arg3 * 40 + task_170[arg2] * 4 + arg4) * 9) | -0x80000000;
          break;
      }
      break;
  }
  *(int32_t *)&local_C4 = Script_ScenDataUtil(data_1285C, 5, Script_ScenDataUtil(data_1226C, 1, arg3 * 40 + task_170[arg2] * 4 + arg4) * 9 + 1);
  *(int32_t *)&local_C0 = Script_ScenDataUtil(data_1285C, 5, Script_ScenDataUtil(data_1226C, 1, arg3 * 40 + task_170[arg2] * 4 + arg4) * 9 + 2);
  local_FC = local_FC | ((Script_FloatUtils(ScriptMathOp_FloatToInt, local_C4 - 3.0f) << 16) & 0x7FFF0000);
  local_BC = 0.0f;
  script_CDF6(arg0, 0);
  local_CC.x = (&task_240)->x;
  (&local_CC)->y = task_240.y;
  (&local_CC)->z = task_240.z;
  Script_ryoControl(PCCTL_StartCameraFacingInterpolation, 0);
  Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
  Script_SetCharaMoveTarget(arg0, &task_240, *(&task_24C + 1), local_FC);
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
        local_A0 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_120[arg2], 2);
        local_A0 = Script_ScenDataUtil(task_E0, 4, Script_ScenDataUtil(task_E4, 4, local_A0 * 9 + 1) * 4 + 2);
        if (local_A0 != -1) {
          if (arg0 == task_238) {
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
  script_CDF6(arg0, 1);
  local_FC = task_120[arg2];
  {
    int32_t local_A0;
    local_A0 = Script_ScenDataUtil(task_D8, 4, local_FC * 13 + 1);
    local_A0 = local_A0 | 8;
    Script_ScenDataUtil(task_D8, 14, local_FC * 13 + 1, local_A0);
  }
  {
    int32_t local_A0;
    local_A0 = Script_ScenDataUtil(task_D8, 4, local_FC * 13 + 1);
    local_A0 = local_A0 | 0x10;
    Script_ScenDataUtil(task_D8, 14, local_FC * 13 + 1, local_A0);
  }
  {
    int32_t local_A0;
    local_A0 = Script_ScenDataUtil(task_D8, 4, local_FC * 13 + 1);
    local_A0 = local_A0 | 4;
    Script_ScenDataUtil(task_D8, 14, local_FC * 13 + 1, local_A0);
  }
  return;
}

__attribute__((scn_string_bindings(0, data_13DD2)))
static void script_7A11(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
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
      if (arg0 == task_238) {
        local_FF5C = arg4;
        switch (local_FF5C) {
          case 1:
            local_CF = 1;
            local_A4 = Script_ScenDataUtil(data_1285C, 4, Script_ScenDataUtil(data_1226C, 1, arg3 * 40 + task_170[arg2] * 4 + arg4) * 9);
            local_C0 = 1.0f;
            local_A0 = 18;
            local_9C = 18;
            local_90 = 15473;
            local_8C = 17294;
            break;
          case 0:
            local_CF = 0;
            local_A4 = Script_ScenDataUtil(data_1285C, 4, Script_ScenDataUtil(data_1226C, 1, arg3 * 40 + task_170[arg2] * 4 + arg4) * 9) | -0x80000000;
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
            local_A4 = Script_ScenDataUtil(data_1285C, 4, Script_ScenDataUtil(data_1226C, 1, arg3 * 40 + task_170[arg2] * 4 + arg4) * 9);
            local_C0 = 1.0f;
            local_A0 = 18;
            local_9C = 18;
            local_90 = 13653;
            break;
          case 0:
            local_CF = 0;
            local_A4 = Script_ScenDataUtil(data_1285C, 4, Script_ScenDataUtil(data_1226C, 1, arg3 * 40 + task_170[arg2] * 4 + arg4) * 9);
            local_C0 = 1.0f;
            local_A0 = 18;
            local_9C = 18;
            local_90 = 19842;
            break;
        }
      }
    } else if ((local_FF5C == 1) | (local_FF5C == 7) | (local_FF5C == 30) | (local_FF5C == 36) | (local_FF5C == 37)) {
      if (arg0 == task_238) {
        local_FF5C = arg4;
        switch (local_FF5C) {
          case 1:
            local_CF = 0;
            local_A4 = Script_ScenDataUtil(data_1285C, 4, Script_ScenDataUtil(data_1226C, 1, arg3 * 40 + task_170[arg2] * 4 + arg4) * 9) | -0x80000000;
            local_C0 = 1.0f;
            local_A0 = 12;
            local_9C = 18;
            local_90 = 50062;
            local_8C = 48241;
            break;
          case 0:
            local_CF = 1;
            local_A4 = Script_ScenDataUtil(data_1285C, 4, Script_ScenDataUtil(data_1226C, 1, arg3 * 40 + task_170[arg2] * 4 + arg4) * 9);
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
            local_A4 = Script_ScenDataUtil(data_1285C, 4, Script_ScenDataUtil(data_1226C, 1, arg3 * 40 + task_170[arg2] * 4 + arg4) * 9);
            local_C0 = 1.0f;
            local_A0 = 18;
            local_9C = 18;
            if (task_170[arg2] == 4) {
              local_90 = 50062;
            } else {
              local_90 = 51882;
            }
            break;
          case 0:
            local_CF = 1;
            local_A4 = Script_ScenDataUtil(data_1285C, 4, Script_ScenDataUtil(data_1226C, 1, arg3 * 40 + task_170[arg2] * 4 + arg4) * 9);
            local_C0 = 1.0f;
            local_A0 = 18;
            local_9C = 18;
            if (task_170[arg2] == 4) {
              local_90 = 46421;
            } else {
              local_90 = 45511;
            }
            break;
        }
      }
    } else if ((local_FF5C == 3) | (local_FF5C == 5) | (local_FF5C == 32) | (local_FF5C == 34)) {
      if (arg0 == task_238) {
        local_FF5C = arg4;
        switch (local_FF5C) {
          case 1:
            local_CF = 1;
            local_A4 = Script_ScenDataUtil(data_1285C, 4, Script_ScenDataUtil(data_1226C, 1, arg3 * 40 + task_170[arg2] * 4 + arg4) * 9) | -0x80000000;
            local_C0 = -1.0f;
            local_A0 = 12;
            local_9C = 12;
            local_90 = 50062;
            local_8C = 48241;
            break;
          case 0:
            local_CF = 0;
            local_A4 = Script_ScenDataUtil(data_1285C, 4, Script_ScenDataUtil(data_1226C, 1, arg3 * 40 + task_170[arg2] * 4 + arg4) * 9);
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
            local_A4 = Script_ScenDataUtil(data_1285C, 4, Script_ScenDataUtil(data_1226C, 1, arg3 * 40 + task_170[arg2] * 4 + arg4) * 9);
            local_C0 = -1.0f;
            local_A0 = 18;
            local_9C = 18;
            local_90 = 51882;
            break;
          case 0:
            local_CF = 0;
            local_A4 = Script_ScenDataUtil(data_1285C, 4, Script_ScenDataUtil(data_1226C, 1, arg3 * 40 + task_170[arg2] * 4 + arg4) * 9);
            local_C0 = -1.0f;
            local_A0 = 18;
            local_9C = 18;
            local_90 = 45511;
            break;
        }
      }
    } else if ((local_FF5C == 2) | (local_FF5C == 8) | (local_FF5C == 31)) {
      if (arg0 == task_238) {
        local_FF5C = arg4;
        switch (local_FF5C) {
          case 1:
            local_CF = 0;
            local_A4 = Script_ScenDataUtil(data_1285C, 4, Script_ScenDataUtil(data_1226C, 1, arg3 * 40 + task_170[arg2] * 4 + arg4) * 9);
            local_C0 = -1.0f;
            local_A0 = 18;
            local_9C = 12;
            local_90 = 15473;
            local_8C = 17294;
            break;
          case 0:
            local_CF = 1;
            local_A4 = Script_ScenDataUtil(data_1285C, 4, Script_ScenDataUtil(data_1226C, 1, arg3 * 40 + task_170[arg2] * 4 + arg4) * 9) | -0x80000000;
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
            local_A4 = Script_ScenDataUtil(data_1285C, 4, Script_ScenDataUtil(data_1226C, 1, arg3 * 40 + task_170[arg2] * 4 + arg4) * 9);
            local_C0 = -1.0f;
            local_A0 = 18;
            local_9C = 18;
            local_90 = 13653;
            break;
          case 0:
            local_CF = 1;
            local_A4 = Script_ScenDataUtil(data_1285C, 4, Script_ScenDataUtil(data_1226C, 1, arg3 * 40 + task_170[arg2] * 4 + arg4) * 9);
            local_C0 = -1.0f;
            local_A0 = 18;
            local_9C = 18;
            local_90 = 19842;
            break;
        }
      }
    } else {
      if ((local_FF5C == 13) | (local_FF5C == 14) | (local_FF5C == 28)) {
        if (arg0 == task_238) {
          local_FF5C = arg4;
          switch (local_FF5C) {
            case 0:
              local_CF = 0;
              local_A4 = Script_ScenDataUtil(data_1285C, 4, Script_ScenDataUtil(data_1226C, 1, arg3 * 40 + task_170[arg2] * 4 + arg4) * 9);
              local_C0 = -1.0f;
              local_A0 = 18;
              local_9C = 12;
              local_90 = 48241;
              local_8C = 50062;
              break;
            case 1:
              local_CF = 1;
              local_A4 = Script_ScenDataUtil(data_1285C, 4, Script_ScenDataUtil(data_1226C, 1, arg3 * 40 + task_170[arg2] * 4 + arg4) * 9) | -0x80000000;
              local_C0 = -1.0f;
              local_A0 = 12;
              local_9C = 12;
              local_90 = 50062;
              local_8C = 48241;
              break;
            case 2:
              local_CF = 1;
              local_A4 = Script_ScenDataUtil(data_1285C, 4, Script_ScenDataUtil(data_1226C, 1, arg3 * 40 + task_170[arg2] * 4 + arg4) * 9);
              local_C0 = 1.0f;
              local_A0 = 18;
              local_9C = 18;
              local_90 = 15473;
              local_8C = 17294;
              break;
            case 3:
              local_CF = 0;
              local_A4 = Script_ScenDataUtil(data_1285C, 4, Script_ScenDataUtil(data_1226C, 1, arg3 * 40 + task_170[arg2] * 4 + arg4) * 9) | -0x80000000;
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
              local_A4 = Script_ScenDataUtil(data_1285C, 4, Script_ScenDataUtil(data_1226C, 1, arg3 * 40 + task_170[arg2] * 4 + arg4) * 9);
              local_C0 = -1.0f;
              local_A0 = 18;
              local_9C = 18;
              local_90 = 45511;
              break;
            case 1:
              local_CF = 1;
              local_A4 = Script_ScenDataUtil(data_1285C, 4, Script_ScenDataUtil(data_1226C, 1, arg3 * 40 + task_170[arg2] * 4 + arg4) * 9);
              local_C0 = -1.0f;
              local_A0 = 18;
              local_9C = 18;
              local_90 = 51882;
              break;
            case 2:
              local_CF = 1;
              local_A4 = Script_ScenDataUtil(data_1285C, 4, Script_ScenDataUtil(data_1226C, 1, arg3 * 40 + task_170[arg2] * 4 + arg4) * 9);
              local_C0 = 1.0f;
              local_A0 = 18;
              local_9C = 18;
              if (task_170[arg2] == 4) {
                local_90 = 14199;
              } else {
                local_90 = 13653;
              }
              break;
            case 3:
              local_CF = 0;
              local_A4 = Script_ScenDataUtil(data_1285C, 4, Script_ScenDataUtil(data_1226C, 1, arg3 * 40 + task_170[arg2] * 4 + arg4) * 9);
              local_C0 = 1.0f;
              local_A0 = 18;
              local_9C = 18;
              if (task_170[arg2] == 4) {
                local_90 = 18022;
              } else {
                local_90 = 19842;
              }
              break;
          }
        }
        local_CE = 1;
        goto L008604;
      }
    }
  }
  L008604:;
  (void)local_90;
  *(int32_t *)&local_B8 = Script_ScenDataUtil(data_1285C, 5, Script_ScenDataUtil(data_1226C, 1, arg3 * 40 + task_170[arg2] * 4 + arg4) * 9 + 1);
  *(int32_t *)&local_B4 = Script_ScenDataUtil(data_1285C, 5, Script_ScenDataUtil(data_1226C, 1, arg3 * 40 + task_170[arg2] * 4 + arg4) * 9 + 2);
  *(int32_t *)&local_B0 = Script_ScenDataUtil(data_1285C, 5, Script_ScenDataUtil(data_1226C, 1, arg3 * 40 + task_170[arg2] * 4 + arg4) * 9 + 3);
  *(int32_t *)&local_AC = Script_ScenDataUtil(data_1285C, 5, Script_ScenDataUtil(data_1226C, 1, arg3 * 40 + task_170[arg2] * 4 + arg4) * 9 + 4);
  *(int32_t *)&local_A8 = Script_ScenDataUtil(data_1285C, 5, Script_ScenDataUtil(data_1226C, 1, arg3 * 40 + task_170[arg2] * 4 + arg4) * 9 + 7);
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
  script_3BF2(arg0, local_A4, arg2, arg3, arg4);
  local_A4 = local_A4 | ((Script_FloatUtils(ScriptMathOp_FloatToInt, local_B8 - 3.0f) << 16) & 0x7FFF0000);
  script_CDF6(arg0, 0);
  local_D0.x = (&task_240)->x;
  (&local_D0)->y = task_240.y;
  (&local_D0)->z = task_240.z;
  Script_ryoControl(PCCTL_StartCameraFacingInterpolation, 0);
  Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
  Script_SetCharaMoveTarget(arg0, &task_240, *(&task_24C + 1), local_A4);
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
    if (arg0 == task_238) {
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
      if (arg0 == task_238) {
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
              script_5E48(arg0, task_170[arg2]);
            }
          }
        }
      }
      if (!(local_C4 > local_B4)) {
        if (!(local_C4 < local_B8) & !(local_C4 > local_B4)) {
          if (arg0 == task_238) {
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
          local_FF50 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_120[arg2], 2);
          local_FF50 = Script_ScenDataUtil(task_E0, 4, Script_ScenDataUtil(task_E4, 4, local_FF50 * 9 + 1) * 4 + 2);
          if (local_FF50 != -1) {
            if (arg0 == task_238) {
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
        local_FF60 = script_3B7A((&local_FF64)->y & 0xFFFF, local_88 & 0xFFFF);
        if (local_FF60 > 910) {
          local_88 = (&local_FF64)->y + 910;
        } else {
          if (local_FF60 < -910) {
            local_88 = (&local_FF64)->y - 910;
            goto L008C09;
          }
        }
        L008C09:;
        if (!(local_C4 >= local_B4)) {
          if (!(local_C0 <= 0.0f)) {
            if (script_3B7A(local_FF70, local_88) > 0) {
              local_88 = local_FF70;
            }
          } else {
            if (script_3B7A(local_FF70, local_88) < 0) {
              local_88 = local_FF70;
            }
          }
        }
      }
      if (arg0 != task_238) {
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
          local_FF74 = (float)script_3B3D(script_3B7A(local_FF70, (&local_FF48)->y) * 360 / 0x10000);
        }
        if (!(local_C4 <= local_B4) & !(local_C4 > local_A8)) {
          *(int32_t *)&local_BC = Script_ScenDataUtil(data_1285C, 5, Script_ScenDataUtil(data_1226C, 1, arg3 * 40 + task_170[arg2] * 4 + arg4) * 9 + 8);
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
          if (arg0 != task_238) {
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
        local_FF50 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_120[arg2], 2);
        local_FF50 = Script_ScenDataUtil(task_E0, 4, Script_ScenDataUtil(task_E4, 4, local_FF50 * 9 + 1) * 4 + 3);
        if (local_FF50 != -1) {
          if (arg0 == task_238) {
            Script_executeAicaCommand(local_FF50, 0, 0);
          } else {
            Script_Execute3DSoundCommand_Wrapper(local_FF50, 4, &local_D0, 5.0f, 20.0f, 0);
          }
        }
        local_A4 = task_120[arg2];
        if (arg0 == task_238) {
          if (task_238 == arg0) {
            local_A0 = (Script_ScenDataUtil(task_D8, 4, local_A4 * 13 + 2) >> 16) & 0xFFFF;
            Script_scenToggleCharaFlag(0, local_A0, 1);
            local_A0 = Script_ScenDataUtil(task_D8, 4, local_A4 * 13 + 2) & 0xFFFF;
            Script_scenToggleCharaFlag(0, local_A0, 1);
          }
        }
      }
      if (!(local_AC <= 0.0f)) {
        if (!(local_C4 <= local_AC)) {
          if (arg0 == task_238) {
            local_FF5C = arg3;
            if ((local_FF5C == 30) | (local_FF5C == 31) | (local_FF5C == 32) | (local_FF5C == 33) | (local_FF5C == 34) | (local_FF5C == 35)) {
              local_FF78 = local_FF78 - 0.06f;
              if (!(local_FF78 >= 0.3f)) {
                local_FF78 = 0.3f;
              }
              DSWK_SetActorOpacityAndDrawCallback(arg1, local_FF78);
              goto L0090B5;
            }
          }
        }
      }
      L0090B5:;
      if (!(local_C4 < local_B0) & !(local_C4 > local_AC)) {
        if (arg0 == task_238) {
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
      if (arg0 != task_238) {
        if (!(local_C4 < local_AC)) {
          break;
        }
        local_C4 = local_C4 + 1.0f;
      }
      SCNF_WaitFrames_0000(1);
    }
  }
  script_CDF6(arg0, 1);
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
  if (arg0 == task_238) {
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
      goto L0092F0;
    }
  }
  L0092F0:;
  return;
}

__attribute__((scn_string_bindings(0, data_13DEE)))
static void script_92F1(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
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
    script_CDF6(arg0, 0);
    local_C4.x = (&task_240)->x;
    (&local_C4)->y = task_240.y;
    (&local_C4)->z = task_240.z;
    Script_ryoControl(PCCTL_StartCameraFacingInterpolation, 0);
    Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
    Script_SetCharaMoveTarget(arg0, &task_240, *(&task_24C + 1), local_94);
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
          local_84 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_120[arg2], 2);
          local_84 = Script_ScenDataUtil(task_E0, 4, Script_ScenDataUtil(task_E4, 4, local_84 * 9 + 1) * 4 + 3);
          if (local_84 != -1) {
            if (arg0 == task_238) {
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
                goto L009615;
              }
            }
            L009615:;
          } else {
            if ((local_84 == 1) | (local_84 == 3)) {
              if (!(local_B8 > -0.75f)) {
                local_B8 = -0.75f;
              } else {
                if (!(local_B8 < 0.0f)) {
                  local_B8 = 0.0f;
                  goto L009651;
                }
              }
              L009651:;
              goto L009654;
            }
          }
        }
      }
      L009654:;
      if (!(local_BC <= local_A4)) {
        int32_t local_84;
        local_84 = arg4;
        if ((local_84 == 0) | (local_84 == 2)) {
          local_B8 = 0.75f;
        } else {
          if ((local_84 == 1) | (local_84 == 3)) {
            local_B8 = -0.75f;
            goto L009699;
          }
        }
      }
      L009699:;
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
  script_CDF6(arg0, 1);
  {
    int32_t local_94;
    local_94 = task_120[arg2];
    local_9C = Script_ScenDataUtil(task_D8, 4, local_94 * 13 + 2) & 0xFFFF;
    Script_scenToggleCharaFlag(0, local_9C, 1);
    local_9C = (Script_ScenDataUtil(task_D8, 4, local_94 * 13 + 2) >> 16) & 0xFFFF;
    Script_scenToggleCharaFlag(0, local_9C, 1);
    {
      int32_t unused_88[3];
      int32_t local_84;
      local_84 = Script_ScenDataUtil(task_D8, 4, local_94 * 13 + 1);
      local_84 = local_84 & -9;
      Script_ScenDataUtil(task_D8, 14, local_94 * 13 + 1, local_84);
    }
    {
      int32_t unused_88[3];
      int32_t local_84;
      local_84 = Script_ScenDataUtil(task_D8, 4, local_94 * 13 + 1);
      local_84 = local_84 & -17;
      Script_ScenDataUtil(task_D8, 14, local_94 * 13 + 1, local_84);
    }
    {
      int32_t unused_88[3];
      int32_t local_84;
      local_84 = Script_ScenDataUtil(task_D8, 4, local_94 * 13 + 1);
      local_84 = local_84 & -5;
      Script_ScenDataUtil(task_D8, 14, local_94 * 13 + 1, local_84);
    }
  }
  return;
}

__attribute__((scn_string_bindings(0, data_13E0A)))
static void script_9A12(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
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
      local_8C = Script_ScenDataUtil(data_1285C, 4, Script_ScenDataUtil(data_1226C, 1, arg3 * 40 + task_170[arg2] * 4 + arg4) * 9);
      local_A0 = -1.0f;
      local_88 = 18;
      local_84 = 18;
      local_FF78 = 46421;
      local_FF74 = 48787;
      break;
    case 1:
      local_AF = 1;
      local_8C = Script_ScenDataUtil(data_1285C, 4, Script_ScenDataUtil(data_1226C, 1, arg3 * 40 + task_170[arg2] * 4 + arg4) * 9) | -0x80000000;
      local_A0 = -1.0f;
      local_88 = 12;
      local_84 = 12;
      local_FF78 = 50426;
      local_FF74 = 48969;
      break;
    case 2:
      local_AF = 1;
      local_8C = Script_ScenDataUtil(data_1285C, 4, Script_ScenDataUtil(data_1226C, 1, arg3 * 40 + task_170[arg2] * 4 + arg4) * 9);
      local_A0 = 1.0f;
      local_88 = 18;
      local_84 = 18;
      local_FF78 = 15109;
      local_FF74 = 16566;
      break;
    case 3:
      local_AF = 0;
      local_8C = Script_ScenDataUtil(data_1285C, 4, Script_ScenDataUtil(data_1226C, 1, arg3 * 40 + task_170[arg2] * 4 + arg4) * 9) | -0x80000000;
      local_A0 = 1.0f;
      local_88 = 12;
      local_84 = 12;
      local_FF78 = 19114;
      local_FF74 = 16748;
      break;
  }
  (void)local_FF78;
  *(int32_t *)&local_9C = Script_ScenDataUtil(data_1285C, 5, Script_ScenDataUtil(data_1226C, 1, arg3 * 40 + task_170[arg2] * 4 + arg4) * 9 + 1);
  *(int32_t *)&local_98 = Script_ScenDataUtil(data_1285C, 5, Script_ScenDataUtil(data_1226C, 1, arg3 * 40 + task_170[arg2] * 4 + arg4) * 9 + 2);
  *(int32_t *)&local_94 = Script_ScenDataUtil(data_1285C, 5, Script_ScenDataUtil(data_1226C, 1, arg3 * 40 + task_170[arg2] * 4 + arg4) * 9 + 3);
  *(int32_t *)&local_90 = Script_ScenDataUtil(data_1285C, 5, Script_ScenDataUtil(data_1226C, 1, arg3 * 40 + task_170[arg2] * 4 + arg4) * 9 + 4);
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
  script_CDF6(arg0, 0);
  local_B0.x = (&task_240)->x;
  (&local_B0)->y = task_240.y;
  (&local_B0)->z = task_240.z;
  Script_ryoControl(PCCTL_StartCameraFacingInterpolation, 0);
  Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
  Script_SetCharaMoveTarget(arg0, &task_240, *(&task_24C + 1), local_8C);
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
        local_FF64 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_120[arg2], 2);
        local_FF64 = Script_ScenDataUtil(task_E0, 4, Script_ScenDataUtil(task_E4, 4, local_FF64 * 9 + 1) * 4 + 2);
        if (local_FF64 != -1) {
          if (arg0 == task_238) {
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
        local_FF64 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_120[arg2], 2);
        local_FF64 = Script_ScenDataUtil(task_E0, 4, Script_ScenDataUtil(task_E4, 4, local_FF64 * 9 + 1) * 4 + 3);
        if (local_FF64 != -1) {
          if (arg0 == task_238) {
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
  script_CDF6(arg0, 1);
  local_F4.x = 0;
  (&local_F4)->z = 0;
  (&local_F4)->y = local_80;
  Script_charaPosRotUtils(arg1, 12, 0, &local_F4, 0);
  (&local_F4)->y = local_FF7C;
  Script_charaPosRotUtils(arg1, 7, 0, &local_F4, 0);
  return;
}

static void script_A19C(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
  int32_t local_FC;
  int32_t local_F8;
  Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
  Script_SetCharaMoveTarget(arg0, &task_240, *(&task_24C + 1), MN_AKI_AKI_BASE);
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
        local_FC = Script_ScenDataUtil(task_D8, 4, task_120[arg2] * 13 + 7);
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

__attribute__((scn_string_bindings(0, data_13E26)))
static void script_A274(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
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
  local_D4 = Script_ScenDataUtil(task_D8, 4, task_120[arg2] * 13 + 6);
  local_CC = arg3;
  if ((local_CC == 1) | (local_CC == 3) | (local_CC == 5) | (local_CC == 7) | (local_CC == 9) | (local_CC == 18) | (local_CC == 21) | (local_CC == 25) | (local_CC == 27) | (local_CC == 36) | (local_CC == 37)) {
    local_CC = arg4;
    switch (local_CC) {
      case 1:
        local_E0 = Script_ScenDataUtil(&data_1327C, 4, local_D4 * 5) | -0x80000000;
        break;
      case 0:
        local_E0 = Script_ScenDataUtil(&data_1327C, 4, local_D4 * 5);
        break;
    }
  } else if ((local_CC == 2) | (local_CC == 4) | (local_CC == 6) | (local_CC == 8) | (local_CC == 10) | (local_CC == 19) | (local_CC == 22) | (local_CC == 24) | (local_CC == 26) | (local_CC == 28)) {
    local_CC = arg4;
    switch (local_CC) {
      case 1:
        local_E0 = Script_ScenDataUtil(&data_1327C, 4, local_D4 * 5);
        break;
      case 0:
        local_E0 = Script_ScenDataUtil(&data_1327C, 4, local_D4 * 5) | -0x80000000;
        break;
    }
  } else {
    if ((local_CC == 13) | (local_CC == 14) | (local_CC == 15) | (local_CC == 16) | (local_CC == 20) | (local_CC == 23) | (local_CC == 29)) {
      local_CC = arg4;
      switch (local_CC) {
        case 0:
          local_E0 = Script_ScenDataUtil(&data_1327C, 4, local_D4 * 5);
          break;
        case 1:
          local_E0 = Script_ScenDataUtil(&data_1327C, 4, local_D4 * 5) | -0x80000000;
          break;
        case 2:
          local_E0 = Script_ScenDataUtil(&data_1327C, 4, local_D4 * 5);
          break;
        case 3:
          local_E0 = Script_ScenDataUtil(&data_1327C, 4, local_D4 * 5) | -0x80000000;
          break;
      }
      goto L00A495;
    }
  }
  L00A495:;
  *(int32_t *)&local_EC = Script_ScenDataUtil(&data_1327C, 5, local_D4 * 5 + 1);
  *(int32_t *)&local_E8 = Script_ScenDataUtil(&data_1327C, 5, local_D4 * 5 + 2);
  local_E0 = local_E0 | ((Script_FloatUtils(ScriptMathOp_FloatToInt, local_EC - 3.0f) << 16) & 0x7FFF0000);
  local_F4.x = (&task_240)->x;
  (&local_F4)->y = task_240.y;
  (&local_F4)->z = task_240.z;
  Script_ryoControl(PCCTL_StartCameraFacingInterpolation, 0);
  Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
  Script_SetCharaMoveTarget(arg0, &task_240, *(&task_24C + 1), local_E0);
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
      local_C8 = Script_ScenDataUtil(task_D8, 4, task_120[arg2] * 13 + 9);
      if (local_C8 != -1) {
        if (arg0 == task_238) {
          Script_executeAicaCommand(local_C8, 0, 0);
        } else {
          Script_Execute3DSoundCommand_Wrapper(local_C8, 4, &local_F4, 5.0f, 20.0f, 0);
        }
      }
    }
    if (!(local_F0 < local_E8) & !(local_F0 >= local_E8 + 1.0f)) {
      if ((local_D4 == 6) | (local_D4 == 7)) {
        int32_t local_C8;
        local_C8 = Script_ScenDataUtil(task_D8, 4, task_120[arg2] * 13 + 9);
        if (local_C8 != -1) {
          if (arg0 == task_238) {
            Script_executeAicaCommand(local_C8, 0, 0);
          } else {
            Script_Execute3DSoundCommand_Wrapper(local_C8, 4, &local_F4, 5.0f, 20.0f, 0);
          }
        }
      }
      if (local_DC == 0) {
        local_DC = 1;
        local_D8 = Script_ScenDataUtil(task_D8, 4, task_120[arg2] * 13 + 7);
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

__attribute__((scn_string_bindings(0, data_13E42)))
static void script_A6EB(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
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
  task_264 = 0;
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
        goto L00A9F7;
      }
    }
  }
  L00A9F7:;
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
  local_B8.x = (&task_240)->x;
  (&local_B8)->y = task_240.y;
  (&local_B8)->z = task_240.z;
  Script_ryoControl(PCCTL_StartCameraFacingInterpolation, 0);
  Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
  Script_SetCharaMoveTarget(arg0, &task_240, *(&task_24C + 1), local_90);
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
        local_FF40 = Script_ScenDataUtil(task_D8, 4, task_120[arg2] * 13 + 10);
        if (local_FF40 != -1) {
          if (arg0 == task_238) {
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
      local_FF40 = Script_ScenDataUtil(task_D8, 4, task_120[arg2] * 13 + 9);
      if (local_FF40 != -1) {
        if (arg0 == task_238) {
          Script_executeAicaCommand(local_FF40, 0, 0);
        } else {
          Script_Execute3DSoundCommand_Wrapper(local_FF40, 4, &local_B8, 5.0f, 20.0f, 0);
        }
      }
    }
    if (!(local_A0 < local_94) & !(local_A0 >= local_94 + 1.0f)) {
      int32_t unused_FF44;
      int32_t local_FF40;
      local_FF40 = Script_ScenDataUtil(task_D8, 4, task_120[arg2] * 13 + 9);
      if (local_FF40 != -1) {
        if (arg0 == task_238) {
          Script_executeAicaCommand(local_FF40, 0, 0);
        } else {
          Script_Execute3DSoundCommand_Wrapper(local_FF40, 4, &local_B8, 5.0f, 20.0f, 0);
        }
      }
      if (local_FF68 == 0) {
        local_FF68 = 1;
        local_FF64 = Script_ScenDataUtil(task_D8, 4, task_120[arg2] * 13 + 8);
        Script_ConversationUtils(1, local_FF74);
        Script_EnqueueSCNT(1, 0x524F4F44, local_FF64);
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  local_A4 = 0.0f;
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0) != 0) {
    local_FF6C = 0;
    L00B4C2:;
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
      if (!(local_FF6C >= 20)) goto L00B4C2;
    }
    {
      int32_t unused_FF44;
      int32_t local_FF40;
      local_FF40 = Script_ScenDataUtil(task_D8, 4, task_120[arg2] * 13 + 11);
      if (local_FF40 != -1) {
        if (arg0 == task_238) {
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

static void script_B67E(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
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
    if (arg0 != task_238) {
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
      Script_SetCharaMoveTarget(arg0, &local_E0, local_BC[1], script_41A4(arg5));
    }
  }
  return;
}

static void script_BAC8(int32_t arg0, DSWKDoorEntry *arg1, int32_t *arg2, DoorResourceDescriptor *arg3, MapObjectRecord *arg4, int32_t arg5, int32_t arg6) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t unused_F4;
  task_D8 = arg1;
  task_DC = arg2;
  task_E0 = arg3;
  task_E4 = arg4;
  task_E8.mode = 0;
  task_E8.id = 0;
  task_E8.entry_idx = 0;
  task_238 = arg0;
  (void)((task_25C = -1) & 1);
  task_258 = 0;
  Script_motiLoad(Script_GetSceneID(), SHEN_NAME('FREE'), "M_MDOR.BIN");
  local_F8 = 0;
  L00BB32:;
  local_FC = Script_AddCharacterHandler(1, 0, local_F8 * 0x1000000 + 0x30726F64);
  Script_PreserveAccumulator_012C(local_FC, "This Charactor Work is NULL!!\012");
  DSWK_AllocDoorMapcRenderState(local_FC);
  Script_scenToggleVisibleFor(local_FC, 0);
  task_1C0[local_F8] = local_FC;
  task_F8[local_F8] = -1;
  task_120[local_F8] = -1;
  task_148[local_F8] = 0;
  task_170[local_F8] = 0;
  task_198[local_F8] = 0;
  task_1E8[local_F8] = 0;
  local_F8 = local_F8 + 1;
  if (!(local_F8 >= 10)) goto L00BB32;
  local_F8 = 0;
  for (;;) {
    if (Script_ScenDataUtil(task_D8, 4, local_F8 * 13) == -1) {
      break;
    }
    Script_ScenDataUtil(task_D8, 14, local_F8 * 13 + 1, 0);
    local_F8 = local_F8 + 1;
  }
  Script_EnqueueSCNT(0);
  DSWK_BindTablesAndInitState(arg1, arg2, arg3, arg4, &task_E8, task_F8, task_120, (shen_name *)task_148, task_170, arg0, arg5, data_1226C, data_1285C, (shen_name *)task_1C0);
  Script_scenCreateTask(script_C384, 0);
  return;
}

static void script_BC54(DSWKDoorEntry *arg0, int32_t *arg1, DoorResourceDescriptor *arg2, MapObjectRecord *arg3, int32_t arg4) {
  task_D8 = arg0;
  task_DC = arg1;
  task_E0 = arg2;
  task_E4 = arg3;
  DSWK_RebindDoorSystemTables(arg0, arg1, arg2, arg3, arg4);
  task_258 = 1;
  return;
}

static void script_BC88(void) {
  int32_t local_FC;
  local_FC = 0;
  L00BC90:;
  task_F8[local_FC] = -1;
  task_120[local_FC] = -1;
  task_148[local_FC] = 0;
  task_170[local_FC] = 0;
  task_198[local_FC] = 0;
  task_1E8[local_FC] = 0;
  local_FC = local_FC + 1;
  if (!(local_FC >= 10)) goto L00BC90;
  return;
}

static void script_BCF1(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  local_F4 = 0;
  for (;;) {
    Script_sprintf("手動ドア開け番号指定\012");
    if (task_238 == arg0) {
      Script_EVT_GetPosition(arg0, -1, &task_26C, 0);
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

static void script_BD8E(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  Vector3 local_F0;
  if (task_238 == arg0) {
    Script_EVT_GetPosition(arg0, -1, &task_26C, 0);
  }
  Script_EVT_GetPosition(arg0, -1, &local_F0, 0);
  local_FC = DSWK_DoorUtils(DOOR_CMD_FIND_CLOSEST_ELEMENT, arg0, &local_F0, arg1);
  (void)local_FC;
  return;
}

static void script_BDC8(void) {
  int32_t local_FC;
  int32_t local_F8;
  local_FC = 0;
  for (;;) {
    if (Script_ScenDataUtil(task_D8, 4, local_FC * 13) == -1) {
      break;
    }
    local_F8 = Script_ScenDataUtil(task_D8, 4, local_FC * 13 + 1);
    local_F8 = local_F8 | 2;
    Script_ScenDataUtil(task_D8, 14, local_FC * 13 + 1, local_F8);
    local_FC = local_FC + 1;
  }
  return;
}

static void script_BE2B(void) {
  int32_t local_FC;
  int32_t local_F8;
  local_FC = 0;
  for (;;) {
    if (Script_ScenDataUtil(task_D8, 4, local_FC * 13) == -1) {
      break;
    }
    local_F8 = Script_ScenDataUtil(task_D8, 4, local_FC * 13 + 1);
    local_F8 = local_F8 & -3;
    Script_ScenDataUtil(task_D8, 14, local_FC * 13 + 1, local_F8);
    local_FC = local_FC + 1;
  }
  return;
}

static void script_BE8E(int32_t arg0) {
  int32_t local_FC;
  local_FC = Script_ScenDataUtil(task_D8, 4, arg0 * 13 + 1);
  local_FC = local_FC | 2;
  Script_ScenDataUtil(task_D8, 14, arg0 * 13 + 1, local_FC);
  return;
}

static void script_BEC5(int32_t arg0) {
  int32_t local_FC;
  local_FC = Script_ScenDataUtil(task_D8, 4, arg0 * 13 + 1);
  local_FC = local_FC & -3;
  Script_ScenDataUtil(task_D8, 14, arg0 * 13 + 1, local_FC);
  return;
}

static void script_BEFC(int32_t arg0) {
  Vector3 local_F4;
  int32_t local_F0;
  int32_t local_EC;
  local_F0 = 0;
  for (;;) {
    if (task_120[local_F0] == arg0) {
      local_EC = task_1C0[local_F0];
      if (Script_shenGetNode(local_EC, 7)) {
        if (Script_shenGetNode(local_EC, 5) & Script_shenGetNode(local_EC, 10)) {
          if (Script_ScenDataUtil(task_D8, 4, task_120[local_F0] * 13 + 1) & 0x100) {
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
          if (Script_ScenDataUtil(task_D8, 4, task_120[local_F0] * 13 + 1) & 0x100) {
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
        if (Script_ScenDataUtil(task_D8, 4, task_120[local_F0] * 13 + 1) & 0x20) {
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

static void script_C13E(int32_t arg0) {
  int32_t local_FC;
  local_FC = Script_ScenDataUtil(task_D8, 4, arg0 * 13 + 1);
  local_FC = local_FC | 0x20;
  Script_ScenDataUtil(task_D8, 14, arg0 * 13 + 1, local_FC);
  script_C246(arg0);
  script_BEFC(arg0);
  return;
}

static void script_C187(int32_t arg0) {
  int32_t local_FC;
  local_FC = Script_ScenDataUtil(task_D8, 4, arg0 * 13 + 1);
  local_FC = local_FC & -33;
  Script_ScenDataUtil(task_D8, 14, arg0 * 13 + 1, local_FC);
  script_C254(arg0);
  script_BEFC(arg0);
  return;
}

static void script_C1D0(int32_t arg0, int32_t arg1) {
  if (arg1) {
    int32_t local_FC;
    local_FC = Script_ScenDataUtil(task_D8, 4, arg0 * 13 + 1);
    local_FC = local_FC & -257;
    Script_ScenDataUtil(task_D8, 14, arg0 * 13 + 1, local_FC);
  } else {
    int32_t local_FC;
    local_FC = Script_ScenDataUtil(task_D8, 4, arg0 * 13 + 1);
    local_FC = local_FC | 0x100;
    Script_ScenDataUtil(task_D8, 14, arg0 * 13 + 1, local_FC);
  }
  return;
}

static void script_C246(int32_t arg0) {
  script_C1D0(arg0, 0);
  return;
}

static void script_C254(int32_t arg0) {
  script_C1D0(arg0, 1);
  return;
}

static void script_C262(int32_t arg0) {
  int32_t local_FC;
  local_FC = 0;
  for (;;) {
    if (Script_ScenDataUtil(task_D8, 4, local_FC * 13) == -1) {
      break;
    }
    if (arg0) {
      int32_t local_F8;
      local_F8 = Script_ScenDataUtil(task_D8, 4, local_FC * 13 + 1);
      local_F8 = local_F8 & -65;
      local_F8 = local_F8 & -129;
      local_F8 = local_F8 | 0x40;
      Script_ScenDataUtil(task_D8, 14, local_FC * 13 + 1, local_F8);
    } else {
      int32_t local_F8;
      local_F8 = Script_ScenDataUtil(task_D8, 4, local_FC * 13 + 1);
      local_F8 = local_F8 & -65;
      local_F8 = local_F8 & -129;
      local_F8 = local_F8 | 0x80;
      Script_ScenDataUtil(task_D8, 14, local_FC * 13 + 1, local_F8);
    }
    local_FC = local_FC + 1;
  }
  return;
}

static void script_C329(void) {
  script_C262(1);
  return;
}

static void script_C335(void) {
  script_C262(0);
  return;
}

static void script_C341(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5, int32_t arg6) {
  int32_t local_FC;
  local_FC = 0;
  for (;;) {
    if (Script_ScenDataUtil(task_D8, 4, local_FC * 13) == -1) {
      break;
    }
    DSWK_AssignDoorEntryConfig(local_FC, arg0, arg1, arg2, arg3, arg4, arg5, arg6);
    local_FC = local_FC + 1;
  }
  return;
}

static int32_t script_C384(void) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  SCNF_WaitFrames_0000(1);
  SCNF_WaitFrames_0000(1);
  SCNF_WaitFrames_0000(1);
  local_F4 = -1;
  local_F8 = -1;
  for (;;) {
    if (task_258 == 0) {
      local_F4 = DSWK_DoorUtils(DOOR_CMD_FIND_LOWEST_FLOAT_INDEX, 0x3FD851EA);
      if (-(local_F4 != -1) & -(task_F8[local_F4] == 1) & Script_IsPlayerAllowed(0) & -((&task_E8)->mode == 0)) {
        local_FC = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_120[local_F4], 2);
        local_F8 = Script_ScenDataUtil(task_E0, 4, Script_ScenDataUtil(task_E4, 4, local_FC * 9 + 1) * 4 + 1);
        script_D154(1, local_FC, local_F4, local_F8);
        if (script_D308(local_FC, local_F4, local_F8)) {
          local_F4 = -1;
          local_F8 = -1;
        }
      } else {
        script_D154(0, local_FC, local_F4, local_F8);
        local_F4 = -1;
        local_F8 = -1;
      }
      script_D51E(local_F4, local_F8);
    } else {
      task_258 = 0;
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int8_t script_C495(int32_t arg0, int32_t arg1, int8_t arg2) {
  Vector3 local_F4;
  Vector3 local_E8;
  Vector3i local_DC;
  int32_t local_D8;
  arg2 = 0;
  Script_EVT_GetPosition(task_238, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  Script_mtwkGetSetRotation(task_238, &local_DC, 0x42000000);
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

__attribute__((scn_string_bindings(0, data_13ECD, 1, data_13EE6)))
static void script_C52D(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  Vector3 local_EC;
  Vector3i local_E0;
  local_F8 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_120[arg0], 1);
  if (local_F8 != -1) {
    local_FC = Script_AddCharacterHandler(1, 0, arg0 * 0x1000000 + 0x30786F62);
    if (local_FC == 0) {
      Script_PreserveAccumulator_012C(local_FC, "Don't make BOX Buffer!!\012");
      task_210[arg0] = -1;
    } else if (local_FC == arg0 * 0x1000000 + 0x30786F62) {
      task_210[arg0] = local_FC;
      *(int32_t *)&local_EC.x = Script_ScenDataUtil(task_E4, 4, local_F8 * 9 + 5);
      *(int32_t *)&(&local_EC)->y = Script_ScenDataUtil(task_E4, 4, local_F8 * 9 + 6);
      *(int32_t *)&(&local_EC)->z = Script_ScenDataUtil(task_E4, 4, local_F8 * 9 + 7);
      Script_EVT_SetPosition(local_FC, POS_XYZ, &local_EC);
      local_E0.x = 0;
      (&local_E0)->y = Script_ScenDataUtil(task_E4, 4, local_F8 * 9 + 8);
      (&local_E0)->z = 0;
      Script_EVT_SetRotation(local_FC, 0x38000000, &local_E0, 0);
      *(int32_t *)&local_EC.x = Script_ScenDataUtil(task_E4, 4, local_F8 * 9 + 2);
      *(int32_t *)&(&local_EC)->y = Script_ScenDataUtil(task_E4, 4, local_F8 * 9 + 3);
      *(int32_t *)&(&local_EC)->z = Script_ScenDataUtil(task_E4, 4, local_F8 * 9 + 4);
      Script_hmdlApplyScale(local_FC, &local_EC);
      Script_CreateNPCImageModule(local_FC, (const char *)Script_ScenDataUtil(task_E0, 8, Script_ScenDataUtil(task_E4, 4, local_F8 * 9 + 1) * 4), 0);
      Script_charaDisplay(local_FC, CDM_Show);
      Script_charaVisUtils(local_FC, 1);
    } else {
      Script_PreserveAccumulator_012C(local_FC, "Don't make BOX Buffer!!\012");
      task_210[arg0] = -1;
    }
  } else {
    task_210[arg0] = -1;
  }
  return;
}

static void script_C6FC(int32_t arg0) {
  if (task_210[arg0] != -1) {
    Script_SetMapControlTaskFlag(task_210[arg0], 1);
    task_210[arg0] = -1;
  }
  return;
}

static int32_t script_C72D(int32_t arg0, int32_t arg1, int32_t arg2, float arg3, float arg4, int32_t arg5, int32_t arg6) {
  Vector3 local_F4;
  Vector3 local_E8;
  float local_DC[3];
  Vector3i local_D0;
  Vector3i local_C4;
  int32_t local_C0;
  int32_t local_BC;
  if (task_238 == arg0) {
    local_F4.x = (&task_26C)->x;
    (&local_F4)->y = task_26C.y;
    (&local_F4)->z = task_26C.z;
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
        if ((Script_ScenDataUtil(task_D8, 4, arg6 * 13 + 1) & 8) | (Script_ScenDataUtil(task_D8, 4, arg6 * 13 + 1) & 0x40)) {
          local_C0 = local_C0 ^ 3;
        }
      } else {
        if ((Script_ScenDataUtil(task_D8, 4, arg6 * 13 + 1) & 0x10) | (Script_ScenDataUtil(task_D8, 4, arg6 * 13 + 1) & 0x80)) {
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
          if ((local_B4 == 1) | (local_B4 == 2)) goto L00C918;
        }
        L00C918:;
      } else if ((local_B4 == 2) | (local_B4 == 4) | (local_B4 == 6) | (local_B4 == 8) | (local_B4 == 10) | (local_B4 == 12) | (local_B4 == 24) | (local_B4 == 26) | (local_B4 == 31) | (local_B4 == 33) | (local_B4 == 35)) {
        local_B4 = local_C0;
        if ((local_B4 == 2) | (local_B4 == 1)) {
          arg3 = (float)0 - arg3;
        } else {
          if ((local_B4 == 3) | (local_B4 == 0)) {
            arg3 = (float)0 - arg3;
            arg4 = (float)0 - arg4;
            (&local_C4)->y = (&local_C4)->y + 0x8000;
            goto L00C9BB;
          }
        }
        L00C9BB:;
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
            goto L00CB67;
          }
        }
      }
    }
    L00CB67:;
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
          task_240.x = (&local_F4)->x;
          task_240.y = (&local_F4)->y;
          task_240.z = (&local_F4)->z;
          task_24C = (&local_C4)->x;
          *(&task_24C + 1) = (&local_C4)->y;
          *(&task_24C + 2) = (&local_C4)->z;
          break;
        case 5:
          task_240.x = (&local_F4)->x;
          task_240.y = (&local_F4)->y;
          task_240.z = (&local_F4)->z;
          task_24C = (&local_C4)->x;
          *(&task_24C + 1) = (&local_C4)->y;
          *(&task_24C + 2) = (&local_C4)->z;
          if ((local_C0 == 1) | (local_C0 == 2)) {
            local_C0 = 1;
          } else {
            local_C0 = 0;
          }
          break;
        case 7:
          Script_EVT_GetPosition(arg0, -1, &task_240, 0);
          task_24C = (&local_C4)->x;
          *(&task_24C + 1) = (&local_C4)->y;
          *(&task_24C + 2) = (&local_C4)->z;
          break;
      }
    }
  }
  return local_C0;
}

static void script_CDF6(int32_t arg0, int32_t arg1) {
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
    if (task_238 == arg0) {
      if (arg1) {
        Script_ccowUtils(arg0, 3);
      } else {
        Script_ccowUtils(arg0, 4);
      }
    }
  }
  return;
}

static void script_CE28(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  int32_t local_F8;
  local_F8 = script_C72D(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
  if (task_238 != arg0) {
    if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
      Script_charaToggleMotEnabled(arg0, 1);
    }
  } else if ((local_F8 == 0) | (local_F8 == 1)) {
    if ((Script_ScenDataUtil(task_D8, 4, arg2 * 13 + 1) & 8) == 0) {
      if ((Script_ScenDataUtil(task_D8, 4, arg2 * 13 + 1) & 0x40) != 0) {
        local_FC = Script_ScenDataUtil(task_D8, 4, arg2 * 13 + 2) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_FC, 0);
      } else {
        local_FC = (Script_ScenDataUtil(task_D8, 4, arg2 * 13 + 2) >> 16) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_FC, 0);
      }
    }
  } else {
    if ((Script_ScenDataUtil(task_D8, 4, arg2 * 13 + 1) & 0x10) == 0) {
      if ((Script_ScenDataUtil(task_D8, 4, arg2 * 13 + 1) & 0x80) != 0) {
        local_FC = (Script_ScenDataUtil(task_D8, 4, arg2 * 13 + 2) >> 16) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_FC, 0);
      } else {
        local_FC = Script_ScenDataUtil(task_D8, 4, arg2 * 13 + 2) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_FC, 0);
      }
    }
  }
  return;
}

__attribute__((scn_string_bindings(0, data_13EFF)))
static void script_CF98(int32_t arg0, int32_t arg1, int32_t arg2) {
  int32_t local_FC;
  if (task_238 == arg0) {
    task_E8.mode = 0;
    task_E8.id = 0;
    if ((arg2 == 0) | (arg2 == 2)) {
      Script_PlayCharaMotion(task_238, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
      if (task_25C & 0x10) {
        Script_sprintf("Door System : ");
        Script_ryoControl(RYO_RestoreControl);
      }
      Script_shenClearFreeState(1);
      if (arg2 != 2) {
        int32_t local_F8;
        local_F8 = task_120[arg1];
        local_FC = Script_ScenDataUtil(task_D8, 4, local_F8 * 13 + 2) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_FC, 1);
        local_FC = (Script_ScenDataUtil(task_D8, 4, local_F8 * 13 + 2) >> 16) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_FC, 1);
      }
    }
  } else {
    if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
      Script_charaToggleMotEnabled(arg0, 0);
    }
    if ((arg2 == 0) | (arg2 == 2)) {
      int32_t local_F8;
      local_F8 = task_120[arg1];
      local_FC = Script_ScenDataUtil(task_D8, 4, local_F8 * 13 + 2) & 0xFFFF;
      Script_scenToggleCharaFlag(0, local_FC, 1);
      local_FC = (Script_ScenDataUtil(task_D8, 4, local_F8 * 13 + 2) >> 16) & 0xFFFF;
      Script_scenToggleCharaFlag(0, local_FC, 1);
    }
  }
  task_F8[arg1] = 1;
  task_1E8[arg1] = 0;
  task_198[arg1] = 0;
  task_148[arg1] = 0;
  if (task_210[arg1] != -1) {
    Script_SetMapControlTaskFlag(task_210[arg1], 1);
    task_210[arg1] = -1;
  }
  return;
}

static void script_D154(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  Vector3 local_E8;
  local_FC = 0;
  if (arg0 == 1) {
    local_F4 = arg3;
    if ((local_F4 == 17) | (local_F4 == 11) | (local_F4 == 12)) {
      local_FC = 0;
    } else if ((Script_ScenDataUtil(task_D8, 4, task_120[arg2] * 13 + 1) & 2) == 0) {
      if ((Script_ScenDataUtil(task_D8, 4, task_120[arg2] * 13 + 1) & 4) == 0) {
        local_FC = script_C495(arg2, arg3, local_FC);
      } else {
        local_F4 = arg3;
        if ((local_F4 == 21) | (local_F4 == 22) | (local_F4 == 23)) {
          local_FC = script_C495(arg2, arg3, local_FC);
          goto L00D218;
        }
      }
      L00D218:;
    } else {
      if ((Script_ScenDataUtil(task_D8, 4, task_120[arg2] * 13 + 1) & 4) == 0) {
        local_FC = script_C495(arg2, arg3, local_FC);
        local_F8 = Script_ScenDataUtil(task_D8, 4, task_120[arg2] * 13 + 6);
        if (local_F8 == 0) {
          local_FC = 0;
        } else {
          if (local_F8 >= 2) {
            Script_EVT_GetPosition(task_238, -1, &local_E8, 0);
            if (Script_CycleManagerUtils(CYCLE_CMD_COUNT_CYCLES_IN_RADIUS, &local_E8, 1.5f)) {
              local_FC = 0;
            }
            goto L00D2B3;
          }
        }
      }
    }
  }
  L00D2B3:;
  if (task_1E8[arg2]) {
    local_FC = 0;
  }
  Script_scenUtils(SCENARIO_CMD_SET_HELP_PROMPT_OVERRIDE, local_FC);
  if ((local_FC ^ task_268) & local_FC) {
    Script_sicnUtils(0, 4, 1);
  }
  if ((local_FC ^ task_268) & task_268) {
    Script_sicnUtils(2, 4, 0);
  }
  task_268 = local_FC;
  return;
}

__attribute__((scn_string_bindings(0, data_13F0E)))
static int32_t script_D308(int32_t arg0, int32_t arg1, int32_t arg2) {
  int32_t local_FC;
  int32_t local_F8;
  Vector3 local_EC;
  local_FC = 1;
  local_F8 = arg2;
  if ((local_F8 == 17) | (local_F8 == 11) | (local_F8 == 12)) {
    if (((Script_ScenDataUtil(task_D8, 4, task_120[arg1] * 13 + 1) & 2) == 0) & ((Script_ScenDataUtil(task_D8, 4, task_120[arg1] * 13 + 1) & 4) == 0)) {
      if (task_1E8[arg1] == 0) {
        task_F8[arg1] = 4;
        local_FC = 0;
      }
    }
  } else {
    if (Script_GetControllerInput(1) == 0x400) {
      if (-(task_148[arg1] != task_238) & task_268) {
        if ((Script_ScenDataUtil(task_D8, 4, task_120[arg1] * 13 + 1) & 2) == 0) {
          task_F8[arg1] = 3;
          task_148[arg1] = task_238;
          task_170[arg1] = 0;
          task_E8.mode = 1;
          task_E8.entry_idx = task_120[arg1];
          local_FC = 0;
        } else {
          if (Script_ScenDataUtil(task_D8, 4, task_120[arg1] * 13 + 6) >= 2) {
            Script_EVT_GetPosition(task_238, -1, &local_EC, 0);
            if (Script_CycleManagerUtils(CYCLE_CMD_COUNT_CYCLES_IN_RADIUS, &local_EC, 1.5f) == 0) {
              task_F8[arg1] = 6;
              task_148[arg1] = task_238;
              task_170[arg1] = 0;
              task_E8.mode = 0;
              task_E8.entry_idx = task_120[arg1];
              local_FC = 0;
            }
            goto L00D4BC;
          }
        }
        L00D4BC:;
        if (local_FC == 0) {
          Script_EVT_GetPosition(task_238, -1, &task_26C, 0);
          Script_ryoControl(PCCTL_RequestNewMode, 4);
          Script_sprintf("Door System : ");
          Script_shenClearFreeState(0);
          Script_ryoControl(PCCTL_DisablePlayerControl);
          task_E8.id = task_238;
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

static void script_D51E(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  int32_t local_F8;
  local_FC = 0;
  L00D526:;
  local_F8 = task_F8[local_FC];
  switch (local_F8) {
    case 6:
      if (!(arg0 != -1)) goto L00D546;
      L00D546:;
      script_D5E8(local_FC, arg1);
      break;
    case 5:
      if (!(arg0 != -1)) goto L00D566;
      L00D566:;
      script_D649(local_FC);
      break;
    case 4:
      if (!(arg0 != -1)) goto L00D584;
      L00D584:;
      script_D7EE(local_FC, arg0, arg1);
      break;
    case 3:
      if (!(arg0 != -1)) goto L00D5A6;
      L00D5A6:;
      script_D886(local_FC, arg1);
      break;
    case 2:
      script_D968(local_FC);
      break;
    case 1:
      break;
  }
  local_FC = local_FC + 1;
  if (!(local_FC >= 10)) goto L00D526;
  return;
}

static void script_D5E8(int32_t arg0, int32_t arg1) {
  if (task_1E8[arg0] == 0) {
    task_1E8[arg0] = Script_scenCreateTask(script_5783, 4, task_148[arg0], task_1C0[arg0], arg0, arg1);
    task_198[arg0] = task_148[arg0];
    script_C52D(arg0);
  }
  return;
}

__attribute__((scn_string_bindings(0, data_13F1D, 1, data_13F2F)))
static void script_D649(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, task_148[arg0])) {
    if (task_1E8[arg0] == 0) {
      if (task_148[arg0] == task_238) {
        task_E8.mode = 1;
        task_E8.entry_idx = task_120[arg0];
        goto L00D69E;
      }
      L00D69E:;
      local_FC = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_120[arg0], 2);
      local_F8 = Script_ScenDataUtil(task_E0, 4, Script_ScenDataUtil(task_E4, 4, local_FC * 9 + 1) * 4 + 1);
      script_CE28(task_148[arg0], task_1C0[arg0], task_120[arg0], local_F8);
      task_1E8[arg0] = Script_scenCreateTask(Script_ScenDataUtil(&data_1331C, 8, local_F8), 4, task_148[arg0], task_1C0[arg0], arg0, local_F8);
      task_198[arg0] = task_148[arg0];
      if ((Script_ScenDataUtil(task_D8, 4, task_120[arg0] * 13 + 1) & 0x20000) == 0) {
        Script_sprintf("Door Box is ON!!\012");
        script_C52D(arg0);
      } else {
        Script_sprintf("Door Box is OFF!!\012");
        task_210[arg0] = -1;
      }
    }
  } else {
    if (task_F8[arg0] != 5) {
      task_F8[arg0] = 1;
      task_1E8[arg0] = 0;
      task_198[arg0] = 0;
      task_148[arg0] = 0;
    }
  }
  return;
}

static void script_D7EE(int32_t arg0, int32_t arg1, int32_t arg2) {
  if ((task_1E8[arg0] == 0) & (arg2 >= 0)) {
    task_148[arg0] = task_238;
    task_E8.mode = 2;
    task_E8.entry_idx = task_120[arg0];
    task_1E8[arg0] = Script_scenCreateTask(Script_ScenDataUtil(&data_1331C, 8, arg2), 4, task_148[arg0], task_1C0[arg0], arg0, arg2);
    task_198[arg0] = task_148[arg0];
    script_C52D(arg0);
  }
  return;
}

__attribute__((scn_string_bindings(0, data_13F42, 1, data_13F54)))
static void script_D886(int32_t arg0, int32_t arg1) {
  if ((task_1E8[arg0] == 0) & (arg1 >= 0)) {
    script_CE28(task_148[arg0], task_1C0[arg0], task_120[arg0], arg1);
    task_1E8[arg0] = Script_scenCreateTask(Script_ScenDataUtil(&data_1331C, 8, arg1), 4, task_148[arg0], task_1C0[arg0], arg0, arg1);
    task_198[arg0] = task_148[arg0];
    if ((Script_ScenDataUtil(task_D8, 4, task_120[arg0] * 13 + 1) & 0x20000) == 0) {
      Script_sprintf("Door Box is ON!!\012");
      script_C52D(arg0);
    } else {
      Script_sprintf("Door Box is OFF!!\012");
      task_210[arg0] = -1;
    }
  }
  return;
}

static void script_D968(int32_t arg0) {
  int32_t local_FC;
  task_1E8[arg0] = 0;
  task_198[arg0] = 0;
  task_F8[arg0] = 1;
  local_FC = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_120[arg0], 2);
  if (local_FC != -1) {
    task_148[arg0] = 0;
  }
  return;
}

static void script_D9BF(void) {
  int32_t local_FC;
  local_FC = Script_GetSceneID();
  switch (local_FC) {
    case 1:
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_ENTRY2_TIMED_EVENT_DONE) == 0) {
        Script_TimedTransitionUtils(TIMED_TRANSITION_REGISTER, 1, 0x3044484A, 2, &data_14054, 0xD5A6C3, TimedTransitionUpdateMemoItem150);
      }
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JD00_ENTRY20_TIMED_EVENT_DONE) == 0) {
        Script_TimedTransitionUtils(TIMED_TRANSITION_REGISTER, 1, 0x3030444A, 20, &data_1401C, 0xDA3AA3, 0);
      }
      break;
    case 2:
      if ((Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_GMCT_ROUTE_STARTED) == 0) | (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_GMCT_ROUTE_ADVANCED) == 0)) {
        Script_TimedTransitionUtils(TIMED_TRANSITION_REGISTER, 2, 0x54434D47, 1, &data_13FE4, 0xB7420F, 0);
      }
      break;
    case 3:
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOZOMI_RESCUE_RUSH_ACTIVE) == 0) {
        Script_TimedTransitionUtils(TIMED_TRANSITION_REGISTER, 3, 0x4F4D4F4A, 20, &data_14000, 0x2DC9FE, TimedTransitionRollbackToPreviousDay2330);
      }
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_FIRED_FROM_JOB) & ~Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 970)) {
        if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_MFSY_ENTRY42_TIMED_EVENT_DONE) == 0) {
          Script_TimedTransitionUtils(TIMED_TRANSITION_REGISTER, 3, 0x5953464D, 42, &data_14038, 0xBBB817, 0);
        }
      }
      break;
  }
  return;
}

static void script_DAF7(int32_t arg0) {
  script_DB0F(arg0);
  return;
}

static void script_DB03(int32_t arg0) {
  script_DB0F(arg0);
  return;
}

static void script_DB0F(int32_t arg0) {
  if (Script_StepUtils(GET_STEP_HIGHER, 0) == 22) {
    int32_t local_FC;
    local_FC = Script_StepUtils(GET_STEP_LOWER, 0);
    if ((local_FC == 2) | (local_FC == 3)) {
      Script_SetShouldSleepFlag(0);
      goto L00DB48;
    }
  }
  L00DB48:;
  Script_ryoSetChara(arg0);
  Script_facemotUtils(arg0, 1, 0);
  Script_facemotUtils(arg0, 0, 0);
  Script_FaceEx(arg0, 0, 0, 2);
  Script_CharHandlers(arg0, -1, 0, 0);
  script_D9BF();
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

static int32_t script_DC02(int32_t arg0) {
  script_DB03(arg0);
  for (;;) {
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_DC19(int32_t arg0) {
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

static void script_DC5A(int32_t arg0) {
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

static void script_DC9E(int32_t arg0) {
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

static void script_DD1C(void) {
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_PROGRESS_STEP_28_REACHED)) {
    script_DC9E(40);
  } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_PROGRESS_STEP_1E_REACHED)) {
    script_DC9E(30);
  } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_PROGRESS_STEP_14_REACHED)) {
    script_DC9E(20);
  } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JU00_CATA1_NBO_BRANCH_CHOSEN)) {
    script_DC9E(10);
  } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_PROGRESS_STEP05_SA1116_DOJO_DONE)) {
    script_DC9E(5);
  } else {
    script_DC9E(0);
  }
  return;
}

static void script_DD9B(int32_t arg0) {
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

static int32_t script_DDC7(int32_t arg0, int32_t arg1) {
  int8_t local_FF;
  int32_t local_F8;
  (void)((local_FF = 0) & 4);
  local_F8 = Script_shenGetStats(STATS_CASH);
  if (local_F8 - arg1 >= arg0) {
    (void)((local_FF = -1) & 4);
  }
  return local_FF & 0x80;
}

static int8_t script_DDFA(int32_t arg0, int32_t arg1, int32_t arg2) {
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
      goto L00DE68;
    }
  }
  L00DE68:;
  local_F8 = local_F8 + local_F4;
  if ((Script_StepUtils(GET_STEP_HIGHER, 0) == 17) & (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_D000_VENDING_LOCK_CLEARED) == 0)) {
    local_F8 = local_F8 + 69000;
    (void)((local_F3 = 0) & 1);
  } else if ((local_EC >= 2202) & (local_EC <= 2204)) {
    (void)((local_F3 = 0) & 1);
    local_F8 = local_F4 = 0;
    local_FF = 4;
  } else {
    (void)((local_F3 = -(script_DDC7(local_F8, arg1) != 0)) & 0x10);
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
      script_DF3C(local_FF, arg0);
    }
  }
  return local_FF;
}

__attribute__((scn_string_bindings(0, data_1425C, 8, data_142A0, 9, data_142A9, 10, data_142B2, 11, data_142BB, 12, data_142C4, 13, data_142CD)))
static void script_DF3C(int32_t arg0, int32_t arg1) {
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

__attribute__((scn_string_bindings(0, data_142D2, 1, data_142D7, 2, data_142E0, 3, data_142E5, 4, data_142EA, 5, data_142F3, 6, data_142FC, 7, data_14305)))
static void script_E105(int32_t arg0, int32_t arg1) {
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

static void script_E2DB(int32_t arg0) {
  task_2D4 = arg0;
  return;
}

static void script_E2E5(void) {
  task_2CC = 0;
  Script_srchUtils(SRCH_SCRIPT_InitEntryTableFromTaskOffset, data_1408C);
  task_2D0 = 1;
  task_2D4 = 0;
  return;
}

static void script_E309(void) {
  int32_t local_FC;
  int16_t local_FA;
  int32_t local_F4;
  task_2C8 = -1;
  task_2C8 = Script_srchUtils(-1);
  if (Script_srchUtils(SRCH_SCRIPT_GetCurrentTargetEntryIndex) >= 0) {
    Script_scenUtils(SCENARIO_CMD_INIT_SCNT);
  } else {
    Script_scenUtils(SCENARIO_CMD_UPDATE_TIMER);
  }
  if (task_2C8 != -1) {
    Script_scenToggleVisibleFor(SHEN_NAME('YKUR'), 1);
    Script_scenToggleVisibleFor(SHEN_NAME('YKUL'), 1);
    Script_ryoControl(PCCTL_DisablePlayerControl);
    local_FC = script_E9F4(task_2C8);
    local_FA = Script_ScenDataUtil(data_1408C, 2, local_FC * 16 + 1);
    local_F4 = local_FA;
    if ((local_F4 == 0) | (local_F4 == 0) | (local_F4 == -1)) {
      script_F0E6(task_2C8, 0);
    } else if (local_F4 == 9) {
      Script_InvestigationUtils(ShenmueUtils_BeginInteractionMode, 0, 60);
      for (;;) {
        if (Script_InvestigationUtils(ShenmueUtils_PollInteractionModeDone) != 0) {
          break;
        }
        SCNF_WaitFrames_0000(1);
      }
      script_E440(local_FC);
      Script_InvestigationUtils(ShenmueUtils_RequestInteractionModeExit);
      for (;;) {
        if (Script_InvestigationUtils(ShenmueUtils_PollInteractionModeDone) != 0) {
          break;
        }
        SCNF_WaitFrames_0000(1);
      }
    } else {
      script_E440(local_FC);
    }
    Script_ryoControl(RYO_RestoreControl);
    Script_scenToggleVisibleFor(SHEN_NAME('YKUR'), 0);
    Script_scenToggleVisibleFor(SHEN_NAME('YKUL'), 0);
  }
  return;
}

static void script_E440(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  int32_t local_F0;
  int32_t local_EC;
  int32_t local_E8;
  int32_t local_E4;
  int8_t local_E3;
  int32_t local_DC;
  int32_t local_D8;
  int32_t local_D4;
  int32_t local_D0;
  int8_t local_CF;
  local_E3 = 0;
  local_FC = Script_ScenDataUtil(data_1408C, 4, arg0 * 8 + 1);
  local_D8 = 0;
  local_E4 = 0;
  for (;;) {
    if (local_FC == Script_ScenDataUtil(data_14230, 4, local_E4 * 2)) {
      local_D8 = Script_ScenDataUtil(data_14230, 4, local_E4 * 2 + 1);
      break;
    }
    if (Script_ScenDataUtil(data_14230, 4, local_E4 * 2) == -1) {
      break;
    }
    local_E4 = local_E4 + 1;
  }
  if (0) {
    local_CF = script_DDFA(2, local_D8, 0);
  } else {
    local_CF = script_DDFA(0, local_D8, 0);
  }
  local_FC = Script_ScenDataUtil(data_1408C, 4, arg0 * 8 + 1);
  local_E4 = 0;
  if (Script_ScenDataUtil(data_141F0, 4, local_E4) != -1) {
    local_F4 = Script_ScenDataUtil(data_141F0, 4, local_E4);
    local_F0 = Script_ScenDataUtil(data_141F0, 4, local_E4 + 1);
    local_EC = Script_ScenDataUtil(data_141F0, 4, local_E4 + 2);
    local_E8 = Script_ScenDataUtil(data_141F0, 4, local_E4 + 3);
    local_E4 = 4;
    for (;;) {
      if ((Script_ScenDataUtil(data_141F0, 4, local_E4) == -1) & (Script_ScenDataUtil(data_141F0, 4, local_E4 + 1) == -1)) {
        local_F4 = 0;
        local_F0 = 0;
        local_EC = 0;
        local_E8 = 0;
        break;
      }
      if (Script_ScenDataUtil(data_141F0, 4, local_E4) == -1) {
        local_E4 = local_E4 + 1;
        local_F4 = Script_ScenDataUtil(data_141F0, 4, local_E4);
        local_F0 = Script_ScenDataUtil(data_141F0, 4, local_E4 + 1);
        local_EC = Script_ScenDataUtil(data_141F0, 4, local_E4 + 2);
        local_E8 = Script_ScenDataUtil(data_141F0, 4, local_E4 + 3);
        local_E4 = local_E4 + 4;
      }
      if (local_FC == Script_ScenDataUtil(data_141F0, 4, local_E4)) {
        break;
      }
      local_E4 = local_E4 + 1;
    }
  }
  Script_InvestigationUtils(ShenmueUtils_QueueSndCommand, local_F4, local_F0);
  if (Script_ScenDataUtil(data_1408C, 4, arg0 * 8 + 7) == 0) {
    Script_InvestigationUtils(ShenmueUtils_BeginTargetModeDefault, local_FC);
  } else {
    Script_InvestigationUtils(ShenmueUtils_BeginTargetModeByType, local_FC, Script_ScenDataUtil(data_1408C, 4, arg0 * 8 + 7) - 1);
  }
  for (;;) {
    SCNF_WaitFrames_0000(1);
    if (Script_InvestigationUtils(ShenmueUtils_UpdateStateMachine) != 0) {
      break;
    }
  }
  if (0) {
    Script_scenCreateTask(script_F0E6, 2, task_2C8, 5);
  } else if (local_CF == 0) {
    Script_scenCreateTask(script_F0E6, 2, task_2C8, 5);
  } else {
    Script_scenCreateTask(script_F0E6, 2, task_2C8, 0);
  }
  local_D0 = 0;
  for (;;) {
    if (task_2D4 < local_D0) {
      break;
    }
    local_D0 = local_D0 + 1;
    SCNF_WaitFrames_0000(1);
  }
  local_DC = Script_shenGetStats(STATS_CASH);
  if (local_D8 == 0) {
    Script_InvestigationUtils(ShenmueUtils_QueueSndCommand, local_EC, local_E8);
    Script_InvestigationUtils(ShenmueUtils_RequestTransitionVariantA);
    for (;;) {
      if (Script_InvestigationUtils(ShenmueUtils_UpdateStateMachine) != 0) {
        break;
      }
      SCNF_WaitFrames_0000(1);
    }
  } else {
    local_E4 = script_E9F4(task_2C8);
    local_F8 = Script_ScenDataUtil(data_1408C, 4, local_E4 * 8 + 6);
    if (local_CF == 1) {
      if (-1) {
        if (0) {
          script_DF3C(local_CF, 2);
        } else {
          script_F0E6(task_2C8, 2);
        }
      }
      Script_InvestigationUtils(ShenmueUtils_QueueSndCommand, local_EC, local_E8);
      Script_InvestigationUtils(ShenmueUtils_RequestTransitionVariantA);
      if (0) {
        script_F0E6(task_2C8, 2);
      } else {
        script_F0E6(task_2C8, 0);
      }
    } else if ((local_CF == 2) | (local_CF == 3) | (local_CF == 4)) {
      if (-1) {
        if (0) {
          script_DF3C(local_CF, 2);
        } else {
          script_DF3C(local_CF, 0);
        }
      }
      Script_InvestigationUtils(ShenmueUtils_QueueSndCommand, local_EC, local_E8);
      Script_InvestigationUtils(ShenmueUtils_RequestTransitionVariantA);
      if (0) {
        script_DF3C(local_CF, 0);
      } else {
        script_F0E6(task_2C8, 0);
      }
    } else if ((local_F8 == 0) | (task_2D0 == 0)) {
      if (task_2D0 == 0) {
        for (;;) {
          if (Script_GetControllerInput(1) == 0x400) {
            break;
          }
          SCNF_WaitFrames_0000(1);
        }
      }
      Script_InvestigationUtils(ShenmueUtils_QueueSndCommand, local_EC, local_E8);
      Script_InvestigationUtils(ShenmueUtils_RequestTransitionVariantA);
    } else {
      local_D4 = SCNF_RequestActionSelectorFromParams_0008(0xFFFF, 40, 41, 0xFFFF, 0x7FFF, 0xFFFF, ACTION_SELECTOR_MODE_STANDARD_TIMEOUT_CANCEL);
      if (local_D4 == 40) {
        if (local_CF == 0) {
          if (local_F8 != 9999) {
            Script_GetCassetteFlags(local_F8, 1, 0, 0);
          }
          Script_InvestigationUtils(ShenmueUtils_RequestTransitionVariantB);
          local_E3 = 1;
          script_F0E6(task_2C8, 1);
        } else {
          Script_InvestigationUtils(ShenmueUtils_QueueSndCommand, local_EC, local_E8);
          Script_InvestigationUtils(ShenmueUtils_RequestTransitionVariantA);
          script_F0E6(task_2C8, 2);
        }
      } else {
        Script_InvestigationUtils(ShenmueUtils_QueueSndCommand, local_EC, local_E8);
        Script_InvestigationUtils(ShenmueUtils_RequestTransitionVariantA);
        script_F0E6(task_2C8, 3);
      }
    }
    if (local_F8 == 0) {
      script_F0E6(task_2C8, 4);
    }
    for (;;) {
      if (Script_InvestigationUtils(ShenmueUtils_UpdateStateMachine) != 0) {
        break;
      }
      SCNF_WaitFrames_0000(1);
    }
    if (local_E3 == 1) {
      Script_srchUtils(SRCH_SCRIPT_Reset);
      Script_charaSetRenderOpacity(CHARA_AKIR, 0.0f);
      script_F215(local_D8);
      Script_charaSetRenderOpacity(CHARA_AKIR, 1.0f);
      Script_SetGameStateTo(AUTH_None);
      Script_ryoControl(RYO_RestoreControl);
    }
  }
  return;
}

static int32_t script_E9F4(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  int16_t local_F6;
  local_F8 = 0;
  for (;;) {
    local_F6 = Script_ScenDataUtil(data_1408C, 4, local_F8 * 8);
    if (local_F6 < 0) {
      break;
    }
    if (Script_ScenDataUtil(data_1408C, 4, local_F8 * 8 + 1) == arg0) {
      local_FC = local_F8;
      break;
    }
    local_F8 = local_F8 + 1;
  }
  return local_FC;
}

static void script_EA53(int32_t arg0) {
  int32_t local_FC;
  if (arg0 != -1) {
    task_2D8 = arg0;
  }
  local_FC = task_2D8;
  if (local_FC == 0) {
    Script_gachaponUtils(0, data_14488);
  } else if (local_FC == 1) {
    Script_gachaponUtils(0, data_14531);
  } else if (local_FC == 2) {
    Script_gachaponUtils(0, data_1466C);
  } else if (local_FC == 3) {
    Script_gachaponUtils(0, data_147A1);
  } else if (local_FC == 4) {
    Script_gachaponUtils(0, data_14848);
  } else if (local_FC == 5) {
    Script_gachaponUtils(0, data_149B5);
  } else if (local_FC == 6) {
    Script_gachaponUtils(0, data_14BAE);
  } else if (local_FC == 7) {
    Script_gachaponUtils(0, data_14C81);
  } else if (local_FC == 8) {
    Script_gachaponUtils(0, data_14CF2);
  } else if (local_FC == 9) {
    Script_gachaponUtils(0, data_14D3F);
  } else if (local_FC == 10) {
    Script_gachaponUtils(0, data_14D78);
  } else if (local_FC == 11) {
    Script_gachaponUtils(0, data_14E67);
  } else if (local_FC == 12) {
    Script_gachaponUtils(0, data_14ECA);
  } else if (local_FC == 13) {
    Script_gachaponUtils(0, data_14F1F);
  } else if (local_FC == 14) {
    Script_gachaponUtils(0, data_14F66);
  } else if (local_FC == 15) {
    Script_gachaponUtils(0, data_14F9F);
  } else {
    task_2D8 = -1;
  }
  return;
}

static int16_t script_EBF4(int8_t arg0, int8_t arg1) {
  int16_t local_FE;
  int8_t local_FD;
  int32_t local_F8;
  local_FE = -1;
  if (arg0 == -1) {
    local_FD = task_2D8;
  } else {
    local_FD = arg0;
  }
  local_F8 = local_FD;
  switch (local_F8) {
    case 0:
      local_FE = *(int16_t *)((int8_t *)&data_1432C + (arg1 << 1));
      break;
    case 1:
      local_FE = *(int16_t *)((int8_t *)&data_14344 + (arg1 << 1));
      break;
    case 2:
      local_FE = *(int16_t *)((int8_t *)&data_14368 + (arg1 << 1));
      break;
    case 3:
      local_FE = *(int16_t *)((int8_t *)&data_14394 + (arg1 << 1));
      break;
    case 4:
      local_FE = *(int16_t *)((int8_t *)&data_143A8 + (arg1 << 1));
      break;
    case 5:
      local_FE = *(int16_t *)((int8_t *)&data_143D0 + (arg1 << 1));
      break;
    case 6:
      local_FE = *(int16_t *)((int8_t *)&data_143F4 + (arg1 << 1));
      break;
    case 7:
      local_FE = *(int16_t *)((int8_t *)&data_1440C + (arg1 << 1));
      break;
    case 8:
      local_FE = *(int16_t *)((int8_t *)&data_1441C + (arg1 << 1));
      break;
    case 9:
      local_FE = *(int16_t *)((int8_t *)&data_14424 + (arg1 << 1));
      break;
    case 10:
      local_FE = *(int16_t *)((int8_t *)&data_1442C + (arg1 << 1));
      break;
    case 11:
      local_FE = *(int16_t *)((int8_t *)&data_14450 + (arg1 << 1));
      break;
    case 12:
      local_FE = *(int16_t *)((int8_t *)&data_14460 + (arg1 << 1));
      break;
    case 13:
      local_FE = *(int16_t *)((int8_t *)&data_1446C + (arg1 << 1));
      break;
    case 14:
      local_FE = *(int16_t *)((int8_t *)&data_14478 + (arg1 << 1));
      break;
    case 15:
      local_FE = *(int16_t *)((int8_t *)&data_14480 + (arg1 << 1));
      break;
  }
  return local_FE;
}

static void script_EDDF(int32_t arg0, int32_t arg1) {
  for (;;) {
    *(int32_t *)arg0 = Script_FloatUtils(ScriptMathOp_RandomIntBelow, 16);
    if (data_1430C[*(int32_t *)arg0] - data_1431C[*(int32_t *)arg0]) {
      break;
    }
  }
  *(int32_t *)arg1 = Script_FloatUtils(ScriptMathOp_RandomIntBelow, data_1430C[*(int32_t *)arg0] - data_1431C[*(int32_t *)arg0]);
  return;
}

static void script_EE2E(int32_t arg0, int32_t arg1) {
  if (arg0 == 0) {
    (void)0;
  } else if (arg0 == 14) {
    (void)0;
  } else if (arg1 < data_1430C[arg0] - data_1431C[arg0]) {
    (void)1;
  } else if (arg0 == 15) {
    (void)3;
  } else {
    (void)2;
  }
  return;
}

static void script_EE81(int32_t arg0, int32_t arg1, int32_t arg2, float arg3) {
  Vector3 local_F4;
  int8_t local_F3;
  int8_t local_F2;
  int32_t local_EC;
  (void)((local_F3 = 0) & 1);
  if (!(arg3 >= 0.0f)) {
    (void)((local_F2 = -1) & 1);
    arg3 = arg3 * -1.0f;
  } else {
    (void)((local_F2 = 0) & 1);
  }
  local_EC = arg1;
  switch (local_EC) {
    case 1:
      local_EC = arg2;
      if ((local_EC == 0) | (local_EC == 1) | (local_EC == 12) | (local_EC == 13)) {
        (void)((local_F3 = -1) & 1);
        goto L00EEF2;
      }
      L00EEF2:;
      break;
    case 3:
      local_EC = arg2;
      if (local_EC == 6) {
        (void)((local_F3 = -1) & 1);
        goto L00EF19;
      }
      L00EF19:;
      break;
    case 4:
      local_EC = arg2;
      if ((local_EC == 7) | (local_EC == 15) | (local_EC == 16) | (local_EC == 17) | (local_EC == 18)) {
        (void)((local_F3 = -1) & 1);
        goto L00EF58;
      }
      L00EF58:;
      break;
    case 5:
      (void)((local_F3 = -1) & 1);
      break;
    case 6:
      local_EC = arg2;
      if ((local_EC == 0) | (local_EC == 1) | (local_EC == 2) | (local_EC == 3)) {
        (void)((local_F3 = -1) & 1);
        goto L00EFA6;
      }
      L00EFA6:;
      break;
    case 8:
      local_EC = arg2;
      if (local_EC == 3) {
        (void)((local_F3 = -1) & 1);
        goto L00EFCD;
      }
      L00EFCD:;
      break;
  }
  local_F4.x = (&local_F4)->y = (&local_F4)->z = 1.0f;
  if (local_F3 & 8) {
    arg2 = arg2 + data_1430C[arg1];
    local_F4.x = (&local_F4)->x * arg3;
    (&local_F4)->y = (&local_F4)->y * arg3;
    (&local_F4)->z = (&local_F4)->z * arg3;
  } else {
    local_F4.x = (&local_F4)->x * arg3 * 0.33333f;
    (&local_F4)->y = (&local_F4)->y * arg3 * 0.33333f;
    (&local_F4)->z = (&local_F4)->z * arg3 * 0.33333f;
  }
  if (local_F2 & 4) {
    script_EA53(arg1);
    Script_gachaponUtils(5, arg0, arg2);
  } else {
    script_EA53(arg1);
    Script_gachaponUtils(1, arg0, arg2);
  }
  if ((((scn3_bits){.f = (&local_F4)->x}).i != 0x3F800000) & (((scn3_bits){.f = (&local_F4)->x}).i != 0)) {
    Script_hmdlApplyScale(arg0, &local_F4);
  }
  return;
}

static int32_t script_F0B8(void) {
  Script_InvestigationUtils(ShenmueUtils_SetupOrSyncAnchors, SHEN_NAME('YKUL'), 0x52554B59);
  script_E2E5();
  for (;;) {
    script_E309();
    script_115E8();
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_F0E6(int32_t arg0, int32_t arg1) {
  if (arg1 == 5) {
    int32_t unused_FC;
    int32_t local_F8;
    local_F8 = arg0;
    switch (local_F8) {
      case 0x72736164:
        SCNT_ShowSubtitle("[[[UISRCHJB12]]]");
        break;
      case 0x72686364:
        SCNT_ShowSubtitle("[[[UISRCHJB10]]]");
        break;
      case 0x70696164:
        SCNT_ShowSubtitle("[[[UISRCHJB13]]]");
        break;
      case 0x6F686364:
        SCNT_ShowSubtitle("[[[UISRCHJB11]]]");
        break;
      case 0x31706164:
        SCNT_ShowSubtitle("[[[UISRCHJB09]]]");
        break;
    }
  }
  if (arg1 == 3) {
    float local_FC;
    local_FC = Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 4.0f);
    if (!(local_FC >= 1.0f)) {
      Script_ShowUIMessageFromID("SA1085A003");
    } else if (!(local_FC >= 2.0f)) {
      Script_ShowUIMessageFromID("SA1085A004");
    } else if (!(local_FC >= 3.0f)) {
      Script_ShowUIMessageFromID("SA1085A005");
    } else {
      Script_ShowUIMessageFromID("SA1085A006");
    }
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
  }
  if (arg1 == 2) {
    float local_FC;
    local_FC = Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f);
    if (!(local_FC >= 1.0f)) {
      Script_ShowUIMessageFromID("SA1085A001");
    } else {
      Script_ShowUIMessageFromID("SA1085A002");
    }
  }
  end:;
}

static void script_F215(int32_t arg0) {
  int8_t local_FF;
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_FaceEx(CHARA_AKIR, 1, 0, 2);
  Script_ResetHandMotion(CHARA_AKIR, -1, 0, 0);
  Script_ResetHandMotion(CHARA_AKIR, -1, 1, 0);
  Script_shenClearFreeState(0);
  Script_PlayerStatsUtils(PS_SetCharaItemCount, 0x41544553, Script_PlayerStatsUtils(PS_GetCharaItemCount, 0x41544553) + 1);
  Script_motmToggleFlags(CHARA_AKIR, 2, 0x40);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_SetCPAMFlags(CHARA_SETA, 0);
  Script_ccowUtils(CHARA_AKIR, 2);
  Script_InitializeOsag(CHARA_AKIR);
  Script_ResetHandMotion(CHARA_AKIR, 1, 1, 0);
  Script_ResetHandMotion(CHARA_AKIR, 1, 0, 0);
  task_2B4 = -1;
  local_FF = script_F478(arg0);
  if (local_FF == 1) {
    script_F86B();
    if (task_2B4 != 6) {
      script_1002C();
      SCNF_WaitFrames_0000(10);
    } else {
      script_387C(0x78626B64, -0.04f, 0.74f, -0.9f);
      script_38A4(0x78626B64, 0.0f, 0.0f, 0.0f);
      Script_charaDisplay(0x78626B64, CDM_Show);
    }
  }
  script_387C(CHARA_AKIR, -0.65f, 0.0f, -0.15f);
  Script_InitializeOsag(CHARA_AKIR);
  Script_motmToggleFlags(CHARA_SETA, 2, 0x20);
  Script_motmToggleFlags(CHARA_AKIR, 2, 0x20);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
  Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
  Script_ccowUtils(CHARA_AKIR, 1);
  Script_SetCPAMFlags(CHARA_SETA, 1);
  Script_SetCharaLookAtPosition(CHARA_AKIR, -20, 0, 0);
  Script_SetCharaLookAtPosition(CHARA_SETA, -20, 0, 0);
  Script_convSetCamInactiveFlag(1);
  Script_SetGameStateTo(AUTH_None);
  SCNF_WaitFrames_0000(1);
  SCNF_WaitFrames_0000(18);
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_EV_STOP_ACTIVE)) {
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_EV_STOP_ACTIVE, 0);
  }
  Script_ryoControl(RYO_RestoreControl);
  Script_FaceEx(CHARA_AKIR, 0, 0, 2);
  Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
  Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
  Script_shenClearFreeState(1);
  return;
}

static int8_t script_F478(int32_t arg0) {
  int32_t unused_F0[4];
  int8_t local_EF;
  uint8_t unused_EC[3];
  int32_t local_E8;
  int32_t local_E4;
  int32_t local_E0;
  Script_filmCheckOrSetTransitionUtil(1, 0, 0, 0, 0, 255, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  script_387C(CHARA_SETA, -0.9f, 0.0f, -1.5f);
  script_38A4(CHARA_SETA, 0.0f, 200.0f, 0.0f);
  Script_InitializeOsag(CHARA_AKIR);
  script_387C(CHARA_AKIR, -0.56f, 0.0f, -0.28f);
  script_38A4(CHARA_AKIR, 0.0f, 0.0f, 0.0f);
  Script_SetHandRootRotation(CHARA_AKIR, 1, 0x15, &data_15400);
  Script_handSetFloatsFor(CHARA_AKIR, 1, data_14FD8, 0);
  Script_handSetFloatsFor(CHARA_AKIR, 0, data_14FD8, 0);
  Script_seqEnterEvtCam(10, 0, AUTH_None);
  Script_filmCheckOrSetTransitionUtil(5, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_ShowUIMessageFromID("SA1085B001");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_ShowUIMessageFromID("SA1085A009");
  script_106A8();
  Script_motmToggleFlags(CHARA_AKIR, 2, 0x40);
  Script_motmToggleFlags(CHARA_AKIR, 1, 0x20);
  Script_PlayCharaMotion(CHARA_AKIR, 28673, 10.0f, 29.0f, -1.0f, 1.0f, 0);
  Script_InitializeOsag(CHARA_AKIR);
  Script_seqEnterEvtCam(30, 0, AUTH_None);
  SCNF_SetCashValue(-arg0, 1);
  Script_motmToggleFlags(CHARA_AKIR, 2, 0x20);
  script_387C(CHARA_AKIR, -0.5f, 0.0f, -0.25f);
  Script_motmToggleFlags(CHARA_AKIR, 2, 0x40);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_SetHandRootRotation(CHARA_AKIR, 1, 0x15, &data_1540C);
  Script_handSetFloatsFor(CHARA_AKIR, 1, data_150BC, 10);
  {
    Vector3 local_D4;
    if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, 0x52494B41) & Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, 0x41544553)) {
      Script_EVT_GetPosition(CHARA_SETA, 4, &local_D4, ENTITY_XFORM_WORLD_SPACE);
      Script_SetLookPointDetails(CHARA_AKIR, 20, &local_D4, 2);
      Script_FaceLookAtTargetUtils(CHARA_AKIR, 2, &local_D4, 30);
    }
    if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, 0x41544553) & Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, 0x52494B41)) {
      Script_EVT_GetPosition(CHARA_AKIR, 4, &local_D4, ENTITY_XFORM_WORLD_SPACE);
      Script_SetLookPointDetails(CHARA_SETA, 20, &local_D4, 2);
      Script_FaceLookAtTargetUtils(CHARA_SETA, 2, &local_D4, 30);
    }
  }
  local_E8 = script_E9F4(task_2C8);
  local_E0 = Script_ScenDataUtil(data_1408C, 4, local_E8 * 8 + 6);
  if ((local_E0 == 65) | (local_E0 == 63) | (local_E0 == 64)) {
    local_EF = 1;
  } else {
    local_EF = 0;
  }
  if (local_EF == 0) {
    Script_seqEnterEvtCam(60, CHARA_AKIR, CHARA_AKIR);
  } else {
    Script_seqEnterEvtCam(65, CHARA_AKIR, CHARA_AKIR);
  }
  if ((local_E0 == 57) | (local_E0 == 62)) {
    Script_GetCassetteFlags(local_E0, 2, 0, 0);
  }
  SCNF_StartIconSystemMenu(local_E0);
  {
    int32_t local_DC;
    local_DC = local_E0;
    switch (local_DC) {
      case 65:
        local_E4 = 0;
        break;
      case 63:
        local_E4 = 1;
        break;
      case 64:
        local_E4 = 2;
        break;
      case 62:
        local_E4 = 3;
        break;
      case 57:
        local_E4 = 4;
        break;
    }
  }
  Script_PlayerStatsUtils(PS_SetSparsePairU8Value, 0, local_E4, Script_PlayerStatsUtils(PS_GetSparsePairU8Value, 0, local_E4) + 1);
  script_10563();
  if (local_EF == 1) {
    Script_seqEnterEvtCam(66, CHARA_AKIR, CHARA_AKIR);
    Script_ShowUIMessageFromID(data_1542E);
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
  }
  return local_EF;
}

static void script_F86B(void) {
  int32_t local_FC;
  int32_t unused_F8;
  Vector3 local_EC;
  int32_t unused_E0[3];
  Vector3i local_D4;
  int32_t local_D0;
  int32_t local_CC;
  int32_t local_C8;
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, 0x52494B41)) {
    Script_SetCharaLookAtPosition(CHARA_AKIR, -13, 0, 0);
    Script_FaceLookAtTargetUtils(CHARA_AKIR, 0);
  }
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, 0x41544553)) {
    Script_SetCharaLookAtPosition(CHARA_SETA, -13, 0, 0);
    Script_FaceLookAtTargetUtils(CHARA_SETA, 0);
  }
  Script_EVT_GetPosition(0x78626B64, -1, &local_EC, 0);
  Script_SetCharaLookAtPosition(CHARA_AKIR, 0x10000, &local_EC, 0);
  Script_FaceLookAtTargetUtils(CHARA_AKIR, 2, &local_EC, 1);
  script_387C(0x78626B64, -0.387f, 0.74f, -0.69f);
  script_38A4(0x78626B64, 0.0f, 0.0f, 0.0f);
  Script_charaDisplay(0x78626B64, CDM_Show);
  script_387C(CHARA_AKIR, -0.47f, 0.0f, -0.23f);
  script_38A4(CHARA_AKIR, 0.0f, 0.0f, 0.0f);
  Script_ResetHandMotion(CHARA_AKIR, 1, 0, 0);
  Script_ResetHandMotion(CHARA_AKIR, 1, 1, 0);
  Script_handSetFloatsFor(CHARA_AKIR, 1, data_14FD8, 10);
  Script_motmToggleFlags(CHARA_AKIR, 2, 0x40);
  Script_PlayCharaMotion(CHARA_AKIR, 28684, 70.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_SetCharaMotionPlaybackRate(CHARA_AKIR, 1.0f);
  Script_seqEnterEvtCam(100, CHARA_AKIR, CHARA_AKIR);
  L00F9D3:;
  local_FC = Script_FloatUtils(ScriptMathOp_RandomIntBelow, 1000);
  if (local_FC <= 0) {
    task_2B4 = 0;
  } else if (local_FC <= 9) {
    task_2B4 = 1;
  } else if (local_FC <= 28) {
    task_2B4 = 2;
  } else if (local_FC <= 60) {
    task_2B4 = 3;
  } else if (local_FC <= 121) {
    task_2B4 = 4;
  } else if (local_FC <= 245) {
    task_2B4 = 5;
  } else {
    task_2B4 = 6;
    goto L00FA61;
    goto L00F9D3;
  }
  L00FA61:;
  Script_PlayerStatsUtils(PS_SetMatrix4x8U16Slot, 0, task_2B4, Script_PlayerStatsUtils(PS_GetMatrix4x8U16Slot, 0, task_2B4) + 1);
  Script_handSetFloatsFor(CHARA_AKIR, 1, data_14FD8, 10);
  Script_handSetFloatsFor(CHARA_AKIR, 0, data_14FD8, 10);
  local_D0 = 0;
  for (;;) {
    if (local_D0 == 75) {
      Script_sicnUtils(0, 4, 1);
    }
    if (Script_IsCharaMotionFinished(CHARA_AKIR)) {
      Script_PlayCharaMotion(CHARA_AKIR, 28683, -1.0f, -1.0f, -1.0f, 1.0f, 0);
      Script_executeAicaCommand(0x1505A9, 0, 0);
    } else {
      if ((local_D0 > 60) & (Script_GetControllerInput(1) == 0x400)) {
        break;
      }
      local_D0 = local_D0 + 1;
      if (local_D0 == 180) {
        break;
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_sicnUtils(2, 4, 0);
  local_EC.x = -0.4169f;
  (&local_EC)->y = 1.3309f;
  (&local_EC)->z = -0.4693f;
  Script_SetCharaLookAtPosition(CHARA_AKIR, 0x10000, &local_EC, 0);
  Script_FaceLookAtTargetUtils(CHARA_AKIR, 2, &local_EC, 1);
  Script_seqEnterEvtCam(105, 0, AUTH_None);
  Script_scenToggleVisibleFor(0x6C636B64, 1);
  Script_charaDisplay(0x6C636B64, CDM_Show);
  local_EC.x = 0.118f;
  (&local_EC)->y = 0.02f;
  (&local_EC)->z = -0.062f;
  local_D4.x = 8008;
  (&local_D4)->y = 9827;
  (&local_D4)->z = 5183;
  Script_fixoAttachEx(0x6C636B64, CHARA_AKIR, 18, &local_EC, &local_D4);
  Script_PlayCharaMotion(CHARA_AKIR, 28682, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_SetCharaMotionPlaybackRate(CHARA_AKIR, 1.2f);
  local_CC = 0;
  for (;;) {
    local_CC = local_CC + 2;
    if (local_CC == 100) {
      Script_executeAicaCommand(0x1405A9, 0, 0);
    }
    if (local_CC == 170) {
      Script_fixoClearAttachment(0x6C636B64);
      Script_scenToggleVisibleFor(0x6C636B64, 0);
      Script_charaDisplay(0x6C636B64, CDM_Hide);
      Script_scenToggleVisibleFor(0x706F6B64, 1);
      Script_charaDisplay(0x706F6B64, CDM_Show);
      local_EC.x = 0.137f;
      (&local_EC)->y = 0.046f;
      (&local_EC)->z = -0.05f;
      local_D4.x = 17668;
      (&local_D4)->y = 52596;
      (&local_D4)->z = 39763;
      Script_fixoAttachEx(0x706F6B64, CHARA_AKIR, 18, &local_EC, &local_D4);
      local_D0 = 0;
      for (;;) {
        local_EC.x = 0.0f;
        (&local_EC)->y = 0.0f;
        (&local_EC)->z = -0.004f;
        Script_charaPosRotUtils(0x706F6B64, local_D0 + 152, &local_EC, 0, 0);
        local_D0 = local_D0 + 1;
        if (local_D0 == 9) {
          break;
        }
      }
      if (task_2B4 == 6) {
        local_EC.x = 0.0f;
        (&local_EC)->y = 0.0f;
        (&local_EC)->z = 0.0f;
        Script_charaPosRotUtils(0x706F6B64, 160, &local_EC, 0, 0);
      } else {
        local_EC.x = 0.0f;
        (&local_EC)->y = 0.0f;
        (&local_EC)->z = 0.0f;
        Script_charaPosRotUtils(0x706F6B64, task_2B4 + 152, &local_EC, 0, 0);
      }
      if ((task_2B4 > 0) & (task_2B4 < 6)) {
        local_EC.x = 0.0f;
        (&local_EC)->y = 0.0f;
        (&local_EC)->z = 0.0f;
        if (task_2B4 == 1) {
          Script_charaPosRotUtils(0x706F6B64, 158, &local_EC, 0, 0);
        } else {
          Script_charaPosRotUtils(0x706F6B64, 159, &local_EC, 0, 0);
        }
      }
      SCNF_WaitFrames_0000(1);
      Script_seqEnterEvtCam(110, CHARA_AKIR, CHARA_AKIR);
    }
    if (local_CC == 220) {
      script_1144D();
    }
    if (Script_IsCharaMotionFinished(CHARA_AKIR)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_EVT_GetPosition(0x6C636B64, -1, &local_EC, 0);
  Script_SetCharaLookAtPosition(CHARA_AKIR, 66048, &local_EC, 1);
  Script_FaceLookAtTargetUtils(CHARA_AKIR, 2, &local_EC, 1);
  Script_PlayCharaMotion(CHARA_AKIR, 28679, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  for (;;) {
    if (Script_IsCharaMotionFinished(CHARA_AKIR)) {
      break;
    }
    Script_RotateNodeUtils(CHARA_AKIR, &data_12084, 0);
    SCNF_WaitFrames_0000(1);
  }
  local_C8 = task_2B4;
  switch (local_C8) {
    case 0:
      task_2B0 = script_10EF4(7);
      break;
    case 1:
      task_2B0 = script_10EF4(6);
      break;
    case 2:
      task_2B0 = script_10EF4(0);
      break;
    case 3:
      task_2B0 = script_10EF4(1);
      break;
    case 5:
      task_2B0 = script_10EBB();
      break;
  }
  Script_FaceLookAtTargetUtils(CHARA_AKIR, 0);
  if ((task_2B4 == 6) | (task_2B4 == 4)) {
    Script_seqEnterEvtCam(115, CHARA_AKIR, CHARA_AKIR);
    if (task_2B4 == 6) {
      script_113D9();
    }
  }
  script_1150B();
  if ((task_2B4 == 4) | (task_2B4 == 6)) {
  } else {
    script_1083C();
  }
  local_D0 = 0;
  for (;;) {
    local_EC.x = 0.0f;
    (&local_EC)->y = 0.0f;
    (&local_EC)->z = -0.004f;
    Script_charaPosRotUtils(0x706F6B64, local_D0 + 152, &local_EC, 0, 0);
    local_D0 = local_D0 + 1;
    if (local_D0 == 9) {
      break;
    }
  }
  if (!(task_2B4 + 1 == 1)) goto L00FFED;
  L00FFED:;
  if (task_2B4 == 6) {
    SCNF_WaitFrames_0000(20);
  }
  Script_fixoClearAttachment(0x706F6B64);
  Script_scenToggleVisibleFor(0x706F6B64, 0);
  Script_charaDisplay(0x706F6B64, CDM_Hide);
  Script_FaceLookAtTargetUtils(CHARA_AKIR, 0);
  return;
}

static void script_1002C(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t unused_E4;
  int32_t local_E0;
  script_387C(0x78626B64, -0.04f, 0.74f, -0.9f);
  script_38A4(0x78626B64, 0.0f, 0.0f, 0.0f);
  Script_charaDisplay(0x78626B64, CDM_Show);
  {
    Vector3 local_D4;
    if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, 0x52494B41) & Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, 0x41544553)) {
      Script_EVT_GetPosition(CHARA_SETA, 4, &local_D4, ENTITY_XFORM_WORLD_SPACE);
      Script_SetLookPointDetails(CHARA_AKIR, 16, &local_D4, 2);
      Script_FaceLookAtTargetUtils(CHARA_AKIR, 2, &local_D4, 30);
    }
    if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, 0x41544553) & Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, 0x52494B41)) {
      Script_EVT_GetPosition(CHARA_AKIR, 4, &local_D4, ENTITY_XFORM_WORLD_SPACE);
      Script_SetLookPointDetails(CHARA_SETA, 16, &local_D4, 2);
      Script_FaceLookAtTargetUtils(CHARA_SETA, 2, &local_D4, 30);
    }
  }
  {
    int32_t local_DC;
    local_DC = task_2B4;
    if ((local_DC == 0) | (local_DC == 1) | (local_DC == 2) | (local_DC == 3)) {
    } else {
      switch (local_DC) {
        case 4:
          task_2B0 = script_108DC();
          break;
        case 5:
          break;
      }
    }
  }
  {
    int32_t local_DC;
    local_DC = task_2B4;
    if ((local_DC == 0) | (local_DC == 1)) {
      local_F4.x = (&local_F4)->y = (&local_F4)->z = 0.5f;
      Script_hmdlApplyScale(task_2B0, &local_F4);
      local_F4.x = -0.408f;
      (&local_F4)->y = 0.819f;
      (&local_F4)->z = -0.84f;
      local_E8.x = 0x8000;
      (&local_E8)->y = 0x8000;
      (&local_E8)->z = 0x8000;
      Script_EVT_SetPosition(task_2B0, POS_XYZ, &local_F4);
      Script_EVT_SetRotation(task_2B0, 0x38000000, &local_E8, 0);
      Script_charaDisplay(task_2B0, CDM_Show);
    } else if ((local_DC == 2) | (local_DC == 3)) {
      local_F4.x = (&local_F4)->y = (&local_F4)->z = 1.0f;
      Script_hmdlApplyScale(task_2B0, &local_F4);
      local_F4.x = -0.462f;
      (&local_F4)->y = 0.871f;
      (&local_F4)->z = -0.804f;
      local_E8.x = 0x8000;
      (&local_E8)->y = 0x8000;
      (&local_E8)->z = 0x8000;
      Script_EVT_SetPosition(task_2B0, POS_XYZ, &local_F4);
      Script_EVT_SetRotation(task_2B0, 0x38000000, &local_E8, 0);
      Script_charaDisplay(task_2B0, CDM_Show);
    } else if (local_DC == 4) {
      local_F4.x = -0.408f;
      (&local_F4)->y = 0.785f;
      (&local_F4)->z = -0.84f;
      local_E8.x = 0x8000;
      (&local_E8)->y = 0x8000;
      (&local_E8)->z = 0x8000;
      Script_EVT_SetPosition(task_2B0, POS_XYZ, &local_F4);
      Script_EVT_SetRotation(task_2B0, 0x38000000, &local_E8, 0);
      Script_charaDisplay(task_2B0, CDM_Show);
    } else if (local_DC == 5) {
      Script_scenToggleVisibleFor(0x67676764, 1);
      Script_charaDisplay(0x67676764, CDM_Show);
      Script_scenToggleVisibleFor(0x66676764, 1);
      Script_charaDisplay(0x66676764, CDM_Show);
      local_F4.x = -0.408f;
      (&local_F4)->y = 0.7886f;
      (&local_F4)->z = -0.84f;
      local_E8.x = 0x8000;
      (&local_E8)->y = 0xC000;
      (&local_E8)->z = 0x8000;
      Script_EVT_SetPosition(0x67676764, POS_XYZ, &local_F4);
      Script_EVT_SetRotation(0x67676764, 0x38000000, &local_E8, 0);
      Script_charaDisplay(0x67676764, CDM_Show);
      Script_EVT_SetPosition(0x66676764, POS_XYZ, &local_F4);
      Script_EVT_SetRotation(0x66676764, 0x38000000, &local_E8, 0);
      Script_charaDisplay(0x66676764, CDM_Show);
    } else {
      local_F4.x = -0.408f;
      (&local_F4)->y = 0.772f;
      (&local_F4)->z = -0.84f;
      local_E8.x = 0x8000;
      (&local_E8)->y = 0;
      (&local_E8)->z = 0x8000;
      Script_EVT_SetPosition(task_2B0, POS_XYZ, &local_F4);
      Script_EVT_SetRotation(task_2B0, 0x38000000, &local_E8, 0);
      Script_charaDisplay(task_2B0, CDM_Show);
    }
  }
  Script_motmToggleFlags(CHARA_SETA, 2, 4);
  Script_motmToggleFlags(CHARA_SETA, 2, 0x40);
  Script_motmToggleFlags(CHARA_SETA, 1, 0x80);
  Script_motmToggleFlags(CHARA_SETA, 1, 0x20);
  Script_PlayCharaMotion(CHARA_SETA, 28686, -1.0f, -1.0f, -1, 1.0f, 0);
  script_112A4();
  SCNF_WaitFrames_0000(165);
  Script_PlayCharaMotion(CHARA_SETA, 28687, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  if (task_2B4 == 5) {
    script_10F2B(task_2B0);
  }
  script_387C(CHARA_AKIR, -0.65f, 0.0f, -0.15f);
  Script_InitializeOsag(CHARA_AKIR);
  Script_charaDisplay(task_2B0, CDM_Hide);
  Script_gachaponUtils(2, task_2B0);
  Script_seqEnterEvtCam(118, 0, AUTH_None);
  Script_motmToggleFlags(CHARA_AKIR, 1, 0x20);
  Script_PlayCharaMotion(CHARA_AKIR, 28673, 0.0f, 30.0f, -1.0f, 1.0f, 0);
  local_E0 = script_EBF4(-1, task_2AC);
  Script_GetCassetteFlags(local_E0, 1, 0, 0);
  SCNF_StartIconSystemMenu(local_E0);
  return;
}

static void script_10563(void) {
  script_387C(0x72736164, 1.86f, 0.63f, 0.655f);
  script_38A4(0x72736164, 15.0f, 270.0f, 260.0f);
  Script_charaDisplay(0x72736164, CDM_Show);
  script_387C(0x31706164, 1.49f, 0.655f, 1.475f);
  script_38A4(0x31706164, 100.0f, 0.0f, 180.0f);
  Script_charaDisplay(0x31706164, CDM_Show);
  script_387C(0x70696164, 1.81f, 0.63f, 1.05f);
  script_38A4(0x70696164, 8.0f, 270.0f, 270.0f);
  Script_charaDisplay(0x70696164, CDM_Show);
  script_387C(0x6F686364, 1.94f, 0.61f, 1.32f);
  script_38A4(0x6F686364, 90.0f, 318.0f, 180.0f);
  Script_charaDisplay(0x6F686364, CDM_Show);
  script_387C(0x72686364, 1.195f, 0.61f, 1.44f);
  script_38A4(0x72686364, 90.0f, 270.0f, 180.0f);
  Script_charaDisplay(0x6F686364, CDM_Show);
  return;
}

static void script_106A8(void) {
  int32_t local_FC;
  local_FC = task_2C8;
  switch (local_FC) {
    case 0x72736164:
      script_387C(0x72736164, -0.39f, 0.755f, -0.8f);
      script_38A4(0x72736164, 90.0f, 180.0f, 180.0f);
      Script_charaDisplay(0x72736164, CDM_Show);
      break;
    case 0x31706164:
      script_387C(0x31706164, -0.38f, 0.755f, -0.84f);
      script_38A4(0x31706164, 270.0f, 0.0f, 0.0f);
      Script_charaDisplay(0x31706164, CDM_Show);
      break;
    case 0x70696164:
      script_387C(0x70696164, -0.39f, 0.755f, -0.8f);
      script_38A4(0x70696164, 90.0f, 180.0f, 180.0f);
      Script_charaDisplay(0x70696164, CDM_Show);
      break;
    case 0x6F686364:
      script_387C(0x6F686364, -0.39f, 0.755f, -0.86f);
      script_38A4(0x6F686364, 90.0f, 180.0f, 180.0f);
      Script_charaDisplay(0x6F686364, CDM_Show);
      break;
    case 0x72686364:
      script_387C(0x72686364, -0.39f, 0.757f, -0.86f);
      script_38A4(0x72686364, 90.0f, 90.0f, 180.0f);
      Script_charaDisplay(0x72686364, CDM_Show);
      break;
  }
  return;
}

static void script_1083C(void) {
  int32_t local_FC;
  local_FC = task_2B4;
  switch (local_FC) {
    case 0:
      Script_seqEnterEvtCam(126, 0, AUTH_None);
      SCNF_WaitFrames_0000(100);
      break;
    case 1:
      Script_seqEnterEvtCam(121, 0, AUTH_None);
      SCNF_WaitFrames_0000(100);
      break;
    case 2:
      Script_seqEnterEvtCam(122, 0, AUTH_None);
      SCNF_WaitFrames_0000(100);
      break;
    case 3:
      Script_seqEnterEvtCam(123, 0, AUTH_None);
      SCNF_WaitFrames_0000(100);
      break;
    case 5:
      Script_seqEnterEvtCam(125, 0, AUTH_None);
      SCNF_WaitFrames_0000(120);
      break;
  }
  return;
}

static int32_t script_108DC(void) {
  Vector3 local_F4;
  Vector3 local_E8;
  Vector3i local_DC;
  float local_D8;
  float local_D4;
  float local_D0;
  float local_CC;
  int32_t unused_C8;
  float local_C4;
  int32_t local_C0;
  int32_t local_BC;
  int32_t local_B8;
  int8_t local_B7;
  int8_t local_B6;
  uint8_t unused_B4[2];
  int32_t local_B0;
  int32_t local_AC;
  Script_scenToggleVisibleFor(SHEN_NAME('YKUR'), 1);
  Script_scenToggleVisibleFor(SHEN_NAME('YKUL'), 1);
  Script_scenToggleVisibleFor(SHEN_NAME('URDM'), 1);
  Script_charaDisplay(CHARA_AKIR, CDM_Hide);
  Script_charaDisplay(SHEN_NAME('URDM'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('YKUR'), CDM_Show);
  Script_handSetFloatsFor(SHEN_NAME('YKUR'), 1, data_15284, 10);
  local_F4.x = -0.56f;
  (&local_F4)->y = 0.752f;
  (&local_F4)->z = -0.9f;
  Script_EVT_SetPosition(0x74687364, POS_XYZ, &local_F4);
  Script_charaDisplay(0x74687364, CDM_Show);
  local_DC.x = 17880;
  (&local_DC)->y = 64520;
  (&local_DC)->z = 32210;
  Script_EVT_SetRotation(SHEN_NAME('YKUR'), 0x38000000, &local_DC, 0);
  Script_EVT_SetRotation(SHEN_NAME('URDM'), 0x38000000, &local_DC, 0);
  Script_SetCameraFlags(0);
  Script_seqEnterEvtCam(124, 0x74687364, 0x74687364);
  SCNF_WaitFrames_0000(80);
  local_D8 = -0.577999949f;
  local_D4 = -0.531f;
  local_C4 = -0.3f;
  for (;;) {
    local_C4 = local_C4 - 0.01f;
    local_F4.x = -0.577999949f;
    (&local_F4)->y = 0.84f;
    (&local_F4)->z = local_C4;
    Script_EVT_SetPosition(SHEN_NAME('YKUR'), POS_XYZ, &local_F4);
    if (!(local_C4 - local_D4 >= 0.0001f)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  local_F4.x = local_D8;
  (&local_F4)->y = 0.84f;
  (&local_F4)->z = local_D4;
  Script_EVT_SetPosition(SHEN_NAME('URDM'), POS_XYZ, &local_F4);
  local_D0 = 0.0f;
  local_CC = 0.0f;
  Script_seqEnterEvtCam(120, SHEN_NAME('URDM'), SHEN_NAME('URDM'));
  Script_uiUtils(0, 58, 0);
  Script_shenClearFreeState(1);
  Script_sicnUtils(0, 4, 1);
  local_C0 = 0;
  local_B7 = 0;
  local_B6 = 1;
  local_B8 = 0;
  for (;;) {
    if (local_B7 == 0) {
      if (Script_GetControllerInput(0) & 0x4000) {
        if (local_B6 > 0) {
          local_B6 = local_B6 - 1;
          local_BC = 0;
          local_B7 = 1;
        }
      }
      if (Script_GetControllerInput(0) & 0x8000) {
        if (local_B6 < 3) {
          local_B6 = local_B6 + 1;
          local_BC = 0;
          local_B7 = 2;
        }
      }
      if (Script_GetControllerInput(1) == 0x400) {
        break;
      }
      local_B8 = local_B8 + 1;
      if (local_B8 > 1800) {
        break;
      }
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_EV_STOP_ACTIVE)) {
        break;
      }
    } else {
      if (local_B7 == 1) {
        local_F4.x = (&local_F4)->x - 0.019f;
        local_D0 = local_D0 + 0.004f;
        if (!(local_D0 <= 0.02f)) {
          local_D0 = 0.02f;
        }
      } else {
        local_F4.x = (&local_F4)->x + 0.019f;
        local_D0 = local_D0 - 0.004f;
        if (!(local_D0 >= -0.02f)) {
          local_D0 = -0.02f;
        }
      }
      local_BC = local_BC + 1;
      if (local_BC == 6) {
        local_B7 = 0;
      }
    }
    if (local_C0 < 50) {
      (&local_F4)->y = (&local_F4)->y + 0.00015f;
    } else {
      (&local_F4)->y = (&local_F4)->y - 0.00015f;
    }
    local_C0 = local_C0 + 1;
    if (local_C0 == 100) {
      local_C0 = 0;
    }
    if (!(local_D0 <= 0.001f)) {
      local_D0 = local_D0 - 0.002f;
    } else {
      if (!(local_D0 >= -0.001f)) {
        local_D0 = local_D0 + 0.002f;
        goto L010C5E;
      }
    }
    L010C5E:;
    if (!(local_CC <= 0.001f)) {
      local_CC = local_CC - 0.002f;
    } else {
      if (!(local_CC >= -0.001f)) {
        local_CC = local_CC + 0.002f;
        goto L010C94;
      }
    }
    L010C94:;
    Script_EVT_SetPosition(SHEN_NAME('URDM'), POS_XYZ, &local_F4);
    local_E8.x = (&local_F4)->x + local_D0;
    (&local_E8)->y = (&local_F4)->y;
    (&local_E8)->z = (&local_F4)->z + local_CC;
    Script_EVT_SetPosition(SHEN_NAME('YKUR'), POS_XYZ, &local_E8);
    Script_scenUtils(SCENARIO_CMD_SET_STEP_LUT, 7, task_2B4 * 10 + local_B6);
    SCNF_WaitFrames_0000(1);
  }
  Script_scenUtils(SCENARIO_CMD_SET_STEP_LUT, 7, 0);
  Script_sicnUtils(2, 4, 0);
  Script_shenClearFreeState(0);
  Script_uiUtils(0, 56, 0);
  script_10DFA();
  local_AC = local_B6;
  switch (local_AC) {
    case 0:
      local_BC = 5;
      break;
    case 1:
      local_BC = 4;
      break;
    case 2:
      local_BC = 3;
      break;
    case 3:
      local_BC = 2;
      break;
  }
  local_B0 = script_10EF4(local_BC);
  Script_ShowUIMessageFromID("SA1085A040");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_SetCameraFlags(1);
  Script_charaDisplay(SHEN_NAME('YKUR'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('URDM'), CDM_Hide);
  Script_charaDisplay(CHARA_AKIR, CDM_Show);
  local_F4.x = -0.75f;
  (&local_F4)->y = 0.752f;
  (&local_F4)->z = -0.9f;
  Script_EVT_SetPosition(0x74687364, POS_XYZ, &local_F4);
  return local_B0;
}

static void script_10DFA(void) {
  int32_t local_FC;
  Vector3i local_F0;
  Script_charaDisplay(SHEN_NAME('URDM'), CDM_Hide);
  local_F0.x = 17880;
  (&local_F0)->y = 64520;
  (&local_F0)->z = 32210;
  local_FC = 0;
  for (;;) {
    local_FC = local_FC + 1;
    local_F0.x = (&local_F0)->x + 200;
    Script_EVT_SetRotation(SHEN_NAME('YKUR'), 0x38000000, &local_F0, 0);
    SCNF_WaitFrames_0000(1);
    Script_charaDisplay(SHEN_NAME('YKUR'), CDM_Show);
    if (local_FC == 6) {
      break;
    }
  }
  for (;;) {
    local_FC = local_FC - 1;
    local_F0.x = (&local_F0)->x - 200;
    Script_EVT_SetRotation(SHEN_NAME('YKUR'), 0x38000000, &local_F0, 0);
    SCNF_WaitFrames_0000(1);
    Script_charaDisplay(SHEN_NAME('YKUR'), CDM_Show);
    if (local_FC == 0) {
      break;
    }
  }
  return;
}

static int32_t script_10EBB(void) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  float local_F0;
  script_EDDF((int32_t)&local_FC, (int32_t)&local_F8);
  local_F0 = 1.0f;
  script_EE81(0x6E68656B, local_FC, local_F8, local_F0);
  task_2AC = local_F8;
  return local_F4 = 0x6E68656B;
}

static int32_t script_10EF4(int32_t arg0) {
  int32_t local_FC;
  float local_F8;
  script_EA53(7);
  local_F8 = 1.0f;
  script_EE81(0x6E68656B, 7, arg0, local_F8);
  task_2AC = arg0;
  return local_FC = 0x6E68656B;
}

static void script_10F2B(int32_t arg0) {
  Script_ResetHandMotion(CHARA_AKIR, 1, 0, 0);
  Script_ResetHandMotion(CHARA_AKIR, 1, 1, 0);
  Script_handSetFloatsFor(CHARA_AKIR, 1, data_14FD8, 10);
  Script_handSetFloatsFor(CHARA_AKIR, 0, data_14FD8, 10);
  Script_scenToggleVisibleFor(0x66676764, 1);
  Script_charaDisplay(0x66676764, CDM_Show);
  {
    Vector3 local_F4;
    Vector3i local_E8;
    local_F4.x = 0.081f;
    (&local_F4)->y = 0.005f;
    (&local_F4)->z = -0.046f;
    local_E8.x = 25282;
    (&local_E8)->y = 3231;
    (&local_E8)->z = 14226;
    Script_fixoAttachEx(0x66676764, CHARA_AKIR, 18, &local_F4, &local_E8);
  }
  Script_scenToggleVisibleFor(0x67676764, 1);
  Script_charaDisplay(0x67676764, CDM_Show);
  {
    Vector3 local_F4;
    Vector3i local_E8;
    local_F4.x = 0.109f;
    (&local_F4)->y = 0.005f;
    (&local_F4)->z = 0.056f;
    local_E8.x = 52428;
    (&local_E8)->y = 27306;
    (&local_E8)->z = 64807;
    Script_fixoAttachEx(0x67676764, CHARA_AKIR, 12, &local_F4, &local_E8);
  }
  {
    Vector3 local_F4;
    Script_EVT_GetPosition(0x78626B64, -1, &local_F4, 0);
    Script_SetCharaLookAtPosition(CHARA_AKIR, 6, &local_F4, 0);
    Script_FaceLookAtTargetUtils(CHARA_AKIR, 2, &local_F4, 1);
  }
  Script_seqEnterEvtCam(130, 0, AUTH_None);
  Script_PlayCharaMotion(CHARA_AKIR, 28685, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_SetCharaMotionPlaybackRate(CHARA_AKIR, 2.0f);
  {
    Vector3 local_F4;
    Vector3i local_E8;
    int32_t local_E4;
    local_E4 = 0;
    for (;;) {
      int32_t local_E0;
      local_E0 = local_E4;
      switch (local_E0) {
        case 80:
          Script_executeAicaCommand(0x1105A9, 0, 0);
          break;
        case 190:
          Script_fixoClearAttachment(0x66676764);
          Script_scenToggleVisibleFor(0x66676764, 0);
          Script_charaDisplay(0x66676764, CDM_Hide);
          local_F4.x = 0.118f;
          (&local_F4)->y = 0.02f;
          (&local_F4)->z = -0.062f;
          local_E8.x = 59708;
          (&local_E8)->y = 43506;
          (&local_E8)->z = 63619;
          Script_fixoAttachEx(arg0, CHARA_AKIR, 18, &local_F4, &local_E8);
          Script_charaDisplay(arg0, CDM_Show);
          break;
        case 240:
          Script_fixoClearAttachment(0x67676764);
          Script_scenToggleVisibleFor(0x67676764, 0);
          Script_charaDisplay(0x67676764, CDM_Hide);
          break;
      }
      if (Script_IsCharaMotionFinished(CHARA_AKIR)) {
        break;
      }
      SCNF_WaitFrames_0000(1);
      local_E4 = local_E4 + 2;
    }
  }
  {
    int32_t unused_E4[7];
    Vector3 local_D8;
    if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, 0x52494B41) & Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, 0x41544553)) {
      Script_EVT_GetPosition(CHARA_SETA, 4, &local_D8, ENTITY_XFORM_WORLD_SPACE);
      Script_SetLookPointDetails(CHARA_AKIR, 20, &local_D8, 2);
      Script_FaceLookAtTargetUtils(CHARA_AKIR, 2, &local_D8, 30);
    }
    if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, 0x41544553) & Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, 0x52494B41)) {
      Script_EVT_GetPosition(CHARA_AKIR, 4, &local_D8, ENTITY_XFORM_WORLD_SPACE);
      Script_SetLookPointDetails(CHARA_SETA, 20, &local_D8, 2);
      Script_FaceLookAtTargetUtils(CHARA_SETA, 2, &local_D8, 30);
    }
  }
  Script_seqEnterEvtCam(140, CHARA_AKIR, CHARA_AKIR);
  SCNF_WaitFrames_0000(60);
  Script_fixoClearAttachment(arg0);
  script_387C(arg0, 0.0f, -1000.0f, 0.0f);
  Script_handSetFloatsFor(CHARA_AKIR, 1, data_150BC, 10);
  Script_handSetFloatsFor(CHARA_AKIR, 0, data_150BC, 10);
  return;
}

__attribute__((scn_string_bindings(3, data_15480, 6, data_154A1)))
static void script_112A4(void) {
  int32_t local_FC;
  int32_t local_F8;
  if ((task_2B4 == 2) | (task_2B4 == 3)) {
    Script_seqEnterEvtCam(75, CHARA_AKIR, CHARA_AKIR);
  } else {
    Script_seqEnterEvtCam(70, CHARA_AKIR, CHARA_AKIR);
  }
  local_F8 = task_2B4;
  if ((local_F8 == 0) | (local_F8 == 1) | (local_F8 == 2) | (local_F8 == 3)) {
    local_FC = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f));
    local_F8 = local_FC;
    switch (local_F8) {
      case 0:
        Script_ShowUIMessageFromID("SA1085B045");
        break;
      case 1:
        Script_ShowUIMessageFromID("SA1085B046");
        break;
      case 2:
        Script_ShowUIMessageFromID("SA1085B047");
        break;
    }
  } else {
    switch (local_F8) {
      case 4:
        Script_ShowUIMessageFromID("SA1085B015");
        break;
      case 5:
        local_FC = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f));
        local_F8 = local_FC;
        switch (local_F8) {
          case 0:
            Script_ShowUIMessageFromID("SA1085B048");
            break;
          case 1:
            Script_ShowUIMessageFromID("SA1085B049");
            break;
          case 2:
            Script_ShowUIMessageFromID("SA1085B015");
            break;
        }
        break;
    }
  }
  return;
}

static void script_113D9(void) {
  int32_t local_FC;
  int32_t local_F8;
  local_FC = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f));
  local_F8 = local_FC;
  switch (local_F8) {
    case 0:
      Script_ShowUIMessageFromID("SA1085B054");
      while (Script_convGetIsActive(0)) {
        SCNF_WaitFrames_0000(1);
      }
      break;
    case 1:
      Script_ShowUIMessageFromID("SA1085B052");
      while (Script_convGetIsActive(0)) {
        SCNF_WaitFrames_0000(1);
      }
      break;
  }
  return;
}

__attribute__((scn_string_bindings(0, data_154C2, 2, data_154D8)))
static void script_1144D(void) {
  int32_t unused_FC;
  int32_t local_F8;
  local_F8 = task_2B4;
  if ((local_F8 == 0) | (local_F8 == 1)) {
    Script_ShowUIMessageFromID("SA1085A039");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
  } else if ((local_F8 == 2) | (local_F8 == 3)) {
    Script_ShowUIMessageFromID("SA1085A038");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
  } else {
    switch (local_F8) {
      case 4:
        Script_ShowUIMessageFromID("SA1085A039");
        while (Script_convGetIsActive(0)) {
          SCNF_WaitFrames_0000(1);
        }
        break;
      case 5:
        Script_ShowUIMessageFromID("SA1085A041");
        break;
      case 6:
        Script_ShowUIMessageFromID("SA1085A042");
        break;
    }
  }
  return;
}

__attribute__((scn_string_bindings(0, data_154F9, 2, data_1550F)))
static void script_1150B(void) {
  int32_t local_FC;
  int32_t local_F8;
  local_F8 = task_2B4;
  if ((local_F8 == 0) | (local_F8 == 1)) {
    local_FC = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f));
    local_F8 = local_FC;
    switch (local_F8) {
      case 0:
        Script_ShowUIMessageFromID("SA1085B042");
        break;
      case 1:
        Script_ShowUIMessageFromID("SA1085B043");
        break;
    }
  } else if ((local_F8 == 2) | (local_F8 == 3)) {
    local_FC = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f));
    local_F8 = local_FC;
    switch (local_F8) {
      case 0:
        Script_ShowUIMessageFromID("SA1085B042");
        break;
      case 1:
        Script_ShowUIMessageFromID("SA1085B022");
        break;
    }
  } else {
    switch (local_F8) {
      case 4:
        Script_ShowUIMessageFromID("SA1085B044");
        break;
      case 5:
        break;
    }
  }
  return;
}

static void script_115E8(void) {
  Vector3 local_F4;
  Vector3 local_E8;
  Vector3i local_DC;
  int32_t local_D8;
  int32_t local_D4;
  int32_t local_D0;
  if (Script_IsPlayerAllowed(0) == 0) {
  } else {
    Script_EVT_GetPosition(CHARA_AKIR, -1, &local_E8, ENTITY_XFORM_WORLD_SPACE);
    Script_mtwkGetSetRotation(CHARA_AKIR, &local_DC, 0x42000000);
    local_F4.x = (&local_E8)->x + Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_DC)->y) * 0.3f;
    (&local_F4)->y = 0.0f;
    (&local_F4)->z = (&local_E8)->z + Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_DC)->y) * 0.3f;
    if (Script_CheckTargetWithinAngleRange(&data_15530, 1.8f, 0x8000, 12743, &local_E8) & Script_CheckTargetWithinAngleRange(&local_F4, 1.7f, (&local_DC)->y, 7281, &data_15530)) {
      Script_SetOwnedItemMenuFlagIfOwned(1, OwnedItem_UIITEMWINN, 1);
    } else {
      Script_SetOwnedItemMenuFlagIfOwned(1, OwnedItem_UIITEMWINN, 0);
    }
    local_D8 = Script_cassetteUtils();
    local_D4 = (local_D8 & -0x10000) >> 16;
    local_D0 = local_D8 & 0xFFFF;
    if (local_D0 == 2) {
      if (local_D4 == 66) {
        script_116EC();
      }
    }
  }
  return;
}

static void script_116EC(void) {
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_FaceEx(CHARA_AKIR, 1, 0, 2);
  Script_ResetHandMotion(CHARA_AKIR, -1, 0, 0);
  Script_ResetHandMotion(CHARA_AKIR, -1, 1, 0);
  Script_shenClearFreeState(0);
  Script_PlayerStatsUtils(PS_SetCharaItemCount, 0x41544553, Script_PlayerStatsUtils(PS_GetCharaItemCount, 0x41544553) + 1);
  Script_motmToggleFlags(CHARA_AKIR, 2, 0x40);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_SetCPAMFlags(CHARA_SETA, 0);
  Script_ccowUtils(CHARA_AKIR, 2);
  Script_InitializeOsag(CHARA_AKIR);
  Script_ResetHandMotion(CHARA_AKIR, 1, 1, 0);
  Script_ResetHandMotion(CHARA_AKIR, 1, 0, 0);
  task_2B4 = -1;
  script_1192E();
  script_F86B();
  if (task_2B4 != 6) {
    script_1002C();
    SCNF_WaitFrames_0000(30);
  } else {
    script_387C(0x78626B64, -0.04f, 0.74f, -0.9f);
    script_38A4(0x78626B64, 0.0f, 0.0f, 0.0f);
    Script_charaDisplay(0x78626B64, CDM_Show);
  }
  Script_SetCharaLookAtPosition(CHARA_AKIR, -20, 0, 0);
  Script_SetCharaLookAtPosition(CHARA_JUKY, -20, 0, 0);
  script_387C(CHARA_AKIR, -0.65f, 0.0f, -0.15f);
  Script_InitializeOsag(CHARA_AKIR);
  Script_motmToggleFlags(CHARA_AKIR, 2, 0x20);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
  Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
  Script_ccowUtils(CHARA_AKIR, 1);
  Script_SetCPAMFlags(CHARA_SETA, 1);
  Script_SetCPAMFlags(CHARA_AKIR, 1);
  if (task_2B4 == 6) {
    Script_convSetCamInactiveFlag(1);
  }
  Script_srchUtils(SRCH_SCRIPT_Reset);
  Script_SetGameStateTo(AUTH_None);
  SCNF_WaitFrames_0000(1);
  Script_ryoControl(RYO_RestoreControl);
  Script_FaceEx(CHARA_AKIR, 0, 0, 2);
  Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
  Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
  Script_shenClearFreeState(1);
  return;
}

static void script_1192E(void) {
  int32_t unused_F0[4];
  Vector3 local_E4;
  Script_filmCheckOrSetTransitionUtil(2, 0, 0, 0, 0, 255, 0, 0, 0);
  SCNF_WaitFrames_0000(1);
  Script_SetCPAMFlags(CHARA_AKIR, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  script_387C(CHARA_SETA, -0.9f, 0.0f, -1.5f);
  script_38A4(CHARA_SETA, 0.0f, 200.0f, 0.0f);
  Script_InitializeOsag(CHARA_AKIR);
  script_387C(CHARA_AKIR, -0.56f, 0.0f, -0.28f);
  script_38A4(CHARA_AKIR, 0.0f, 0.0f, 0.0f);
  Script_InitializeOsag(CHARA_AKIR);
  Script_SetHandRootRotation(CHARA_AKIR, 1, 0x15, &data_1553C);
  Script_handSetFloatsFor(CHARA_AKIR, 1, data_14FD8, 0);
  Script_handSetFloatsFor(CHARA_AKIR, 0, data_14FD8, 0);
  Script_seqEnterEvtCam(10, 0, AUTH_None);
  Script_filmCheckOrSetTransitionUtil(5, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  SCNF_WaitFrames_0000(40);
  Script_ShowUIMessageFromID("SA1085A011");
  Script_motmToggleFlags(CHARA_AKIR, 2, 0x40);
  Script_motmToggleFlags(CHARA_AKIR, 1, 0x20);
  Script_PlayCharaMotion(CHARA_AKIR, 28673, 10.0f, 29.0f, -1.0f, 1.0f, 0);
  Script_scenToggleVisibleFor(0x6B746164, 1);
  Script_charaDisplay(0x6B746164, CDM_Show);
  script_387C(0x6B746164, -0.39f, 0.813f, -0.86f);
  script_38A4(0x6B746164, 180.0f, 180.0f, 180.0f);
  Script_charaDisplay(0x6B746164, CDM_Show);
  Script_seqEnterEvtCam(30, 0, AUTH_None);
  SCNF_WaitFrames_0000(60);
  Script_motmToggleFlags(CHARA_AKIR, 2, 0x20);
  script_387C(CHARA_AKIR, -0.5f, 0.0f, -0.25f);
  Script_motmToggleFlags(CHARA_AKIR, 2, 0x40);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_SetHandRootRotation(CHARA_AKIR, 1, 0x15, &data_15548);
  Script_handSetFloatsFor(CHARA_AKIR, 1, data_150BC, 10);
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, 0x52494B41) & Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, 0x41544553)) {
    Script_EVT_GetPosition(CHARA_SETA, 4, &local_E4, ENTITY_XFORM_WORLD_SPACE);
    Script_SetLookPointDetails(CHARA_AKIR, 20, &local_E4, 2);
    Script_FaceLookAtTargetUtils(CHARA_AKIR, 2, &local_E4, 30);
  }
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, 0x41544553) & Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, 0x52494B41)) {
    Script_EVT_GetPosition(CHARA_AKIR, 4, &local_E4, ENTITY_XFORM_WORLD_SPACE);
    Script_SetLookPointDetails(CHARA_SETA, 20, &local_E4, 2);
    Script_FaceLookAtTargetUtils(CHARA_SETA, 2, &local_E4, 30);
  }
  SCNF_WaitFrames_0000(1);
  if (task_2B4 != 6) {
    Script_seqEnterEvtCam(60, CHARA_AKIR, CHARA_AKIR);
  } else {
    Script_seqEnterEvtCam(65, CHARA_AKIR, CHARA_AKIR);
  }
  Script_scenToggleVisibleFor(0x6B746164, 0);
  Script_charaDisplay(0x6B746164, CDM_Hide);
  Script_seqEnterEvtCam(66, CHARA_AKIR, CHARA_AKIR);
  Script_ShowUIMessageFromID("SA1085B041");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_GetCassetteFlags_0(66, 1, 0);
  SCNF_WaitFrames_0000(20);
  return;
}

static void script_11CA5(void) {
  int32_t local_FC;
  Script_CycleManagerUtils(CYCLE_CMD_SETUP, "scene/01/D000", (int32_t)"SCRTEST.BIN", 0x4542414A);
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
  Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT, 0x41544553, 1);
  local_FC = Script_CycleManagerUtils(CYCLE_CMD_GET_CYCLE_PTR_BY_CHARA_ID, 0x41544553);
  while (local_FC == 0) {
    SCNF_WaitFrames_0000(1);
    local_FC = Script_CycleManagerUtils(CYCLE_CMD_GET_CYCLE_PTR_BY_CHARA_ID, 0x41544553);
  }
  while (Script_CycleManagerUtils(CYCLE_CMD_GET_NEXT_TASK_POINTER, local_FC) == 0) {
    SCNF_WaitFrames_0000(1);
  }
  script_387C(CHARA_SETA, -0.9f, 0.0f, -1.5f);
  script_38A4(CHARA_SETA, 0.0f, 180.0f, 0.0f);
  Script_motmToggleFlags(CHARA_SETA, 1, 4);
  Script_PlayCharaMotion(CHARA_SETA, 28687, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_CycleManagerUtils(CYCLE_CMD_SET_RUNNING_PROGRAM_FIELD_B8, 0x41544553, 0);
  Script_SetShadowFlags(CHARA_AKIR, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 0);
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 1);
  return;
}

void script_11DA8(void) {
  Script_scenUtils(SCENARIO_CMD_SET_STEP_LUT, 7, 0);
  task_2DC = Script_ryoSleepUtils(0);
  Script_filmCheckOrSetTransitionUtil(0, 255, 0, 0, 0, 0, 0, 0, 0);
  Script_SetCameraSettings(5, 6, 1, 0);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 100.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 10.0f);
  Script_SetMapFlags(0, 0);
  SCNF_WaitFrames_0000(1);
  SCNF_WaitFrames_0000(1);
  Script_LoadVoiceFile(1, "SA1085");
  Script_motiLoad(1, SHEN_NAME('FREE'), "M_MOBJ.BIN");
  Script_facemotUtils(CHARA_AKIR, 1, 0);
  Script_scenCreateTask(script_DC02, 1, CHARA_AKIR);
  Script_scenCreateTask(script_1205C, 0);
  Script_GetSetWeatherSettings(WEATHER_SETTINGS_SET_DISABLE_WEATHER, 1);
  Script_WeatherUtils_0(WeatherDataUtil_ApplyCurrentWeatherOrEnableDateTimeRead, 0);
  Script_WeatherTaskUtil(InitTaskData);
  Script_WeatherUtils(WeatherScript_ApplyWccpClipSet1, 2.5218f, 1.9654f, -3.0005f, 1.984f, -3.1005f, -5.5148f, 2.4686f, -5.4861f, -0.3f, 3.0f);
  Script_WeatherTaskUtil(ToggleFogTask, 0);
  Script_WeatherTaskUtil(ToggleSceneLightingFlags, 0);
  Script_SetupLightFromTable(0);
  Script_SetGlobalSceneSlotValue34(1, 1);
  Script_TimedTransitionUtils(TIMED_TRANSITION_REGISTER, Script_GetSceneID(), 0x3030444A, 3, (const void *)-1, 0x121F420, 6);
  Script_PlayerStatsUtils(PS_IncrementSpotVisitCount, 23);
  (void)1;
  script_BAC8(CHARA_AKIR, (DSWKDoorEntry *)&data_13F68, (int32_t *)data_13FA0, (DoorResourceDescriptor *)&data_13FB0, (MapObjectRecord *)&data_13FC0, 2, 0);
  SCNF_WaitFrames_0000(1);
  SCNF_WaitFrames_0000(1);
  DSWK_DoorUtils(DOOR_CMD_SET_STATE_1);
  script_BDC8();
  script_BEC5(0);
  Script_scenCreateTask(script_11FF1, 0);
  Script_scenCreateTask(script_F0B8, 0);
  script_11CA5();
  Script_CycleManagerUtils(CYCLE_CMD_RESET_LOAD_SETTLE);
  for (;;) {
    if (Script_CycleManagerUtils(CYCLE_CMD_TICK_LOAD_SETTLE_COUNT) == 0) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_uiUtils(0, 56, 0);
  Script_shenClearFreeState(1);
  Script_ryoControl(RYO_RestoreControl);
  SCNF_WaitFrames_0000(1);
  SCNF_WaitFrames_0000(1);
  if (task_2DC != 0) {
    Script_ryoSleepUtils(1);
  }
  Script_ryoSleepUtils(2, 1);
  Script_cdttLoadScrUtils(1, 0);
  Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static int32_t script_11FF1(void) {
  for (;;) {
    if ((&task_E8)->mode) {
      if (task_E8.entry_idx == 0) {
        SCNF_WaitFrames_0000(30);
        Script_shenClearFreeState(0);
        Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
        while (Script_filmCheckOrSetTransitionUtil(-1)) {
          SCNF_WaitFrames_0000(1);
        }
        Script_shenRequestGameModuleTransition(1, SHEN_NAME('JD00'), 3);
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_1205C(void) {
  script_3ADC();
  for (;;) {
    if (script_3B2E() != -1) {
      script_3B06();
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}
