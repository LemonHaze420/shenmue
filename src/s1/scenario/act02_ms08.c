#include "act02_ms08.h"

static int32_t script_0030(int32_t arg0);
static void script_004E(float arg0);
static int32_t script_006D(int32_t arg0, int32_t arg1);
static void script_00E5(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4);
static int32_t script_0697(int32_t arg0);
static void script_0768(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_0778(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_087C(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_0A1E(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_0B7F(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_0CE0(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_0F7E(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_104C(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_10B3(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_111A(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_1161(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_1297(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_1B58(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_1C76(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_233B(int32_t arg0, int32_t arg1);
static void script_2411(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_3324(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_3390(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_384A(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_3F04(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_57E4(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_5F05(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_668F(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_6767(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_6BDE(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_7B71(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_7FBB(int32_t arg0, DSWKDoorEntry *arg1, int32_t *arg2, DoorResourceDescriptor *arg3, MapObjectRecord *arg4, int32_t arg5, int32_t arg6);
static void script_8147(DSWKDoorEntry *arg0, int32_t *arg1, DoorResourceDescriptor *arg2, MapObjectRecord *arg3, int32_t arg4);
static void script_817B(void);
static void script_81E4(int32_t arg0, int32_t arg1);
static void script_8281(int32_t arg0, int32_t arg1);
static void script_82BB(void);
static void script_831E(void);
static void script_8381(int32_t arg0);
static void script_83B8(int32_t arg0);
static void script_83EF(int32_t arg0);
static void script_8631(int32_t arg0);
static void script_867A(int32_t arg0);
static void script_86C3(int32_t arg0, int32_t arg1);
static void script_8739(int32_t arg0);
static void script_8747(int32_t arg0);
static void script_8755(int32_t arg0);
static void script_881C(void);
static void script_8828(void);
static void script_8834(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5, int32_t arg6);
static int32_t script_8877(void);
static int8_t script_8988(int32_t arg0, int32_t arg1, int8_t arg2);
static void script_8A20(int32_t arg0);
static void script_8BEF(int32_t arg0);
static int32_t script_8C20(int32_t arg0, int32_t arg1, int32_t arg2, float arg3, float arg4, int32_t arg5, int32_t arg6);
static void script_92E9(int32_t arg0, int32_t arg1);
static void script_931B(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_948B(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_9647(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_97FB(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_9A11(int32_t arg0, int32_t arg1);
static void script_9ADB(int32_t arg0, int32_t arg1);
static void script_9B3C(int32_t arg0);
static void script_9CE1(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_9D79(int32_t arg0, int32_t arg1);
static void script_9E5B(int32_t arg0);
static int32_t script_9EB2(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_A7B4(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_A8A7(int32_t arg0, int32_t arg1);
static void script_A9E6(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_AB95(int32_t arg0, int32_t arg1, int32_t arg2);
static int8_t script_AC8E(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5, int32_t arg6);
static int8_t script_B17E(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_B74A(int32_t arg0, int32_t arg1, float arg2, float arg3, int32_t arg4, int32_t arg5);
static int32_t script_B989(int32_t arg0, int32_t arg1, float arg2, float arg3, int32_t arg4, int32_t arg5, int32_t arg6);
static int32_t script_BC18(int32_t arg0, int32_t arg1, float arg2, float arg3, int32_t arg4, int32_t arg5, int32_t arg6);
static void script_BF25(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_C153(int32_t arg0);
static void script_C1F2(float arg0);
static void script_C281(float arg0);
static void script_C310(void);
static int32_t script_C347(int32_t arg0, int32_t arg1);
static void script_C47E(int32_t arg0);
static void script_C4C5(int32_t arg0);
static void script_C4CF(int32_t arg0);
static void script_C4D9(void);
static void script_C4E0(int32_t arg0);
static void script_C5BB(int32_t arg0);
static void script_C696(int32_t arg0);
static void script_C771(int32_t arg0, int32_t arg1);
static void script_C867(int32_t arg0, int32_t arg1);
static int32_t script_C95D(int32_t arg0, int32_t arg1);
static int32_t script_CA85(int32_t arg0, int32_t arg1);
static void script_CBB6(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_CC3B(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_CC8B(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_CCDA(int32_t arg0, float arg1, float arg2, float arg3);
static void script_CD02(int32_t arg0, float arg1, float arg2, float arg3);
static void script_CD56(int32_t arg0, int32_t arg1, int32_t arg2);
static int32_t script_CDA6(int32_t arg0);
static void script_CE4D(int32_t arg0, int32_t arg1);
static int32_t script_CE70(int32_t arg0);
static void script_CEA3(int32_t arg0, int32_t arg1);
static void script_CEC6(int32_t arg0, int32_t arg1);
static void script_CF25(int32_t arg0);
static void script_CF3A(void);
static void script_CF43(void);
static void script_CF52(int32_t arg0);
static void script_CFDF(int32_t arg0);
static void script_D02F(void);
static void script_D05D(int32_t arg0);
static void script_D08B(void);
static void script_D0BF(int32_t arg0);
static void script_D0F3(int32_t arg0);
static void script_D111(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_D14B(int32_t arg0, int32_t arg1);
static void script_D17F(void);
static void script_D277(int32_t arg0, int32_t arg1);
static void script_D2A7(void);
static void script_D305(void);
static void script_D358(void);
static void script_D635(void);
static int32_t script_DDAD(void);
static void script_DED1(void);
static void script_DFC3(void);
static void script_E041(int32_t arg0);
static void script_E07C(int32_t arg0);
static void script_E0A6(void);
static void script_E0CB(void);
static void script_E137(void);
static void script_E19A(void);
static void script_E1C1(void);
static void script_E2F9(int32_t arg0);
static void script_E305(int32_t arg0);
static void script_E311(int32_t arg0);
static void script_E40D(int32_t arg0);
static void script_E424(int32_t arg0);
static void script_E465(int32_t arg0);
static void script_E4A9(int32_t arg0);
static void script_E527(void);
void script_E5A6(void);
static void script_E791(void);
static void script_E7B6(void);
static void script_E859(int32_t arg0);
static void script_E8C3(int32_t arg0);
static void script_E97A(int32_t arg0, int32_t arg1);
static void script_EC71(int32_t arg0);
static void script_EFE8(int32_t arg0);
static void script_F15C(int32_t arg0, int32_t arg1);
static int32_t script_F876(int32_t arg0);
static int32_t script_F8D4(int32_t arg0);
static void script_F935(int32_t arg0);
static void script_FA0E(int32_t arg0);
static void script_FC42(int32_t arg0, int32_t arg1);
static int32_t script_FC93(void);
static int32_t script_FCDF(int32_t arg0);
static int32_t script_FE4C(int32_t arg0);
static int32_t script_102EA(void);
static int32_t script_1041D(int32_t arg0, int32_t arg1);
static int32_t script_106DC(void);
static void script_107AF(void);
static int32_t script_1082D(void);
static void script_10856(const char *arg0, const char *arg1);
static void script_10890(void);

static DSWKDoorEntry *task_0;
static int32_t *task_4;
static DoorResourceDescriptor *task_8;
static MapObjectRecord *task_C;
static DoorSlotState task_10;
static int32_t task_1C;
static int32_t task_20[10];
static int32_t task_48[10];
static int32_t task_70[10];
static int32_t task_98[10];
static int32_t task_C0[10];
static int32_t task_E8[10];
static int32_t task_110[10];
static int32_t task_138[10];
static int32_t task_160;
static int32_t task_164;
static Vector3 task_168;
static int32_t task_174;
static int32_t task_178;
static int32_t task_17C;
static int8_t task_180;
static uint8_t task_181;
static uint16_t task_182;
static int8_t task_184;
static uint8_t task_185;
static uint16_t task_186;
static int32_t task_188;
static int32_t task_18C;
static int8_t task_190;
static uint8_t task_191;
static uint16_t task_192;
static Vector3 task_194;
static int32_t task_1A0;
static int32_t task_1A4;
static int32_t task_1A8;
static int32_t task_1AC;
static int32_t task_1B0;
static int32_t task_1B4;
static int32_t task_1B8;
static int32_t task_1BC;
static int32_t task_1C0;
static int32_t task_1C4[4];
static int32_t task_1D4;
static float task_1D8;
static float task_1DC;
static float task_1E0;
static int8_t task_1E4;
static int8_t task_1E5;
static int8_t task_1E6;
static int8_t task_1E7;
static int8_t task_1E8;
static int8_t task_1E9;
static int8_t task_1EA;
static int8_t task_1EB;
static int8_t task_1EC;
static uint8_t task_1ED;
static uint16_t task_1EE;
static int8_t task_1F0;
static uint8_t task_1F1;
static uint16_t task_1F2;
static int32_t task_1F4;
static int32_t task_1F8;
static int32_t task_1FC[7];
static Vector3 task_218;
static Vector3i task_224;
static int32_t task_230;
static int32_t task_234;
static int32_t task_238;
static int8_t task_23C;
static int8_t task_23D;
static int8_t task_23E;
static int8_t task_23F;
static const Vector3 data_10AE8 = {.x = 1.0f, .y = 1.0f, .z = 1.0f};
static const int32_t data_10AF4[4] = {16777215, 16777215, 16777215, 16777215};
static const float data_10B04[3] = {20.0f, 800.0f, 1.0f};
static const int32_t data_10B10 = -1;
static DoorMotionSelectorBlock data_10B14[38] = {
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
static ScnDoorMotionDescriptor data_11104[72] = {
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
static uint32_t data_11B24 = 0xFFFFFFFF;
typedef struct Row_11B28 { float w0; float w1; float w2; float w3; int32_t w4; } Row_11B28;
static const Row_11B28 data_11B28[7] = {
  {0.0f, 0.0f, 0.0f, 0.0f, -1},
  {0.0f, 0.0f, 0.0f, 0.0f, -1},
  {0.0f, 0.0f, 0.0f, 0.0f, 1534},
  {26.0f, 55.0f, 45.0f, 75.0f, 1555},
  {61.0f, 146.0f, 45.0f, 50.0f, 1554},
  {59.0f, 124.0f, 45.0f, 70.0f, 1543},
  {33.0f, 49.0f, 45.0f, 50.0f, 1543}
};
static const float data_11BB4[4] = {33.0f, 49.0f, 50.0f, 50.0f};
static void (*data_11BC4)(int32_t, int32_t, int32_t) = script_0768;
static void (*const data_11BC8)(int32_t, int32_t, int32_t, int32_t) = script_0778;
static void (*const data_11BCC)(int32_t, int32_t, int32_t, int32_t) = script_0778;
static void (*const data_11BD0)(int32_t, int32_t, int32_t, int32_t) = script_0778;
static void (*const data_11BD4)(int32_t, int32_t, int32_t, int32_t) = script_0778;
static void (*const data_11BD8)(int32_t, int32_t, int32_t, int32_t) = script_0778;
static void (*const data_11BDC)(int32_t, int32_t, int32_t, int32_t) = script_0778;
static void (*const data_11BE0)(int32_t, int32_t, int32_t, int32_t) = script_0778;
static void (*const data_11BE4)(int32_t, int32_t, int32_t, int32_t) = script_0778;
static void (*const data_11BE8)(int32_t, int32_t, int32_t, int32_t) = script_0A1E;
static void (*const data_11BEC)(int32_t, int32_t, int32_t, int32_t) = script_0B7F;
static void (*const data_11BF0)(int32_t, int32_t, int32_t, int32_t) = script_104C;
static void (*const data_11BF4)(int32_t, int32_t, int32_t, int32_t) = script_10B3;
static void (*const data_11BF8)(int32_t, int32_t, int32_t, int32_t) = script_087C;
static void (*const data_11BFC)(int32_t, int32_t, int32_t, int32_t) = script_087C;
static void (*const data_11C00)(int32_t, int32_t, int32_t, int32_t) = script_0CE0;
static void (*const data_11C04)(int32_t, int32_t, int32_t, int32_t) = script_0F7E;
static void (*const data_11C08)(int32_t, int32_t, int32_t, int32_t) = script_111A;
static void (*const data_11C0C)(int32_t, int32_t, int32_t, int32_t) = script_0A1E;
static void (*const data_11C10)(int32_t, int32_t, int32_t, int32_t) = script_0B7F;
static void (*const data_11C14)(int32_t, int32_t, int32_t, int32_t) = script_0CE0;
static void (*const data_11C18)(int32_t, int32_t, int32_t, int32_t) = script_0A1E;
static void (*const data_11C1C)(int32_t, int32_t, int32_t, int32_t) = script_0B7F;
static void (*const data_11C20)(int32_t, int32_t, int32_t, int32_t) = script_0CE0;
static void (*const data_11C24)(int32_t, int32_t, int32_t, int32_t) = script_1161;
static void (*const data_11C28)(int32_t, int32_t, int32_t, int32_t) = script_1161;
static void (*const data_11C2C)(int32_t, int32_t, int32_t, int32_t) = script_1161;
static void (*const data_11C30)(int32_t, int32_t, int32_t, int32_t) = script_1161;
static void (*const data_11C34)(int32_t, int32_t, int32_t, int32_t) = script_1297;
static void (*const data_11C38)(int32_t, int32_t, int32_t, int32_t) = script_1B58;
static void (*const data_11C3C)(int32_t, int32_t, int32_t, int32_t) = script_0778;
static void (*const data_11C40)(int32_t, int32_t, int32_t, int32_t) = script_0778;
static void (*const data_11C44)(int32_t, int32_t, int32_t, int32_t) = script_0778;
static void (*const data_11C48)(int32_t, int32_t, int32_t, int32_t) = script_0778;
static void (*const data_11C4C)(int32_t, int32_t, int32_t, int32_t) = script_0778;
static void (*const data_11C50)(int32_t, int32_t, int32_t, int32_t) = script_0778;
static void (*const data_11C54)(int32_t, int32_t, int32_t, int32_t) = script_0778;
static void (*const data_11C58)(int32_t, int32_t, int32_t, int32_t) = script_0778;
static const char data_11C5C[46] __attribute__((scn_string_literal)) = "Door Index = %d : call_type = %d : type = %d\012";
static const char data_11C8A[43] __attribute__((scn_string_literal)) = "Door Open Motion = DEB_AKESIME_FUSUMA_L_F\012";
static const char data_11CB5[43] __attribute__((scn_string_literal)) = "Door Open Motion = DEB_AKESIME_FUSUMA_R_F\012";
static const char data_11CE0[42] __attribute__((scn_string_literal)) = "Door Open Motion = DEB_HIKISIME_DOOR_L_F\012";
static const char data_11D0A[42] __attribute__((scn_string_literal)) = "Door Open Motion = DEB_HIKISIME_DOOR_R_F\012";
static const char data_11D34[41] __attribute__((scn_string_literal)) = "Door Open Motion = DEB_OSISIME_DOOR_L_F\012";
static const char data_11D5D[41] __attribute__((scn_string_literal)) = "Door Open Motion = DEB_OSISIME_DOOR_R_F\012";
static const char data_11D86[43] __attribute__((scn_string_literal)) = "Door Open Motion = GAK_AKESIME_FUSUMA_L_F\012";
static const char data_11DB1[43] __attribute__((scn_string_literal)) = "Door Open Motion = GAK_AKESIME_FUSUMA_R_F\012";
static const char data_11DDC[42] __attribute__((scn_string_literal)) = "Door Open Motion = GAK_HIKISIME_DOOR_L_F\012";
static const char data_11E06[42] __attribute__((scn_string_literal)) = "Door Open Motion = GAK_HIKISIME_DOOR_R_F\012";
static const char data_11E30[41] __attribute__((scn_string_literal)) = "Door Open Motion = GAK_OSISIME_DOOR_L_F\012";
static const char data_11E59[41] __attribute__((scn_string_literal)) = "Door Open Motion = GAK_OSISIME_DOOR_R_F\012";
static const char data_11E82[43] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_AKESIME_FUSUMA_L_F\012";
static const char data_11EAD[43] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_AKESIME_FUSUMA_R_F\012";
static const char data_11ED8[42] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_HIKISIME_DOOR_L_F\012";
static const char data_11F02[42] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_HIKISIME_DOOR_R_F\012";
static const char data_11F2C[41] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_OSISIME_DOOR_L_F\012";
static const char data_11F55[41] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_OSISIME_DOOR_R_F\012";
static const char data_11F7E[43] __attribute__((scn_string_literal)) = "Door Open Motion = KOD_AKESIME_FUSUMA_L_F\012";
static const char data_11FA9[43] __attribute__((scn_string_literal)) = "Door Open Motion = KOD_AKESIME_FUSUMA_R_F\012";
static const char data_11FD4[42] __attribute__((scn_string_literal)) = "Door Open Motion = KOD_HIKISIME_DOOR_L_F\012";
static const char data_11FFE[42] __attribute__((scn_string_literal)) = "Door Open Motion = KOD_HIKISIME_DOOR_R_F\012";
static const char data_12028[41] __attribute__((scn_string_literal)) = "Door Open Motion = KOD_OSISIME_DOOR_L_F\012";
static const char data_12051[41] __attribute__((scn_string_literal)) = "Door Open Motion = KOD_OSISIME_DOOR_R_F\012";
static const char data_1207A[43] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_AKESIME_FUSUMA_L_F\012";
static const char data_120A5[43] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_AKESIME_FUSUMA_R_F\012";
static const char data_120D0[42] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_HIKISIME_DOOR_L_F\012";
static const char data_120FA[42] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_HIKISIME_DOOR_R_F\012";
static const char data_12124[41] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_OSISIME_DOOR_L_F\012";
static const char data_1214D[41] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_OSISIME_DOOR_R_F\012";
static const char data_12176[43] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_AKESIME_FUSUMA_L_F\012";
static const char data_121A1[43] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_AKESIME_FUSUMA_R_F\012";
static const char data_121CC[42] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_HIKISIME_DOOR_L_F\012";
static const char data_121F6[42] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_HIKISIME_DOOR_R_F\012";
static const char data_12220[41] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_OSISIME_DOOR_L_F\012";
static const char data_12249[41] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_OSISIME_DOOR_R_F\012";
static const char data_12272[43] __attribute__((scn_string_literal)) = "Door Open Motion = SIN_AKESIME_FUSUMA_L_F\012";
static const char data_1229D[43] __attribute__((scn_string_literal)) = "Door Open Motion = SIN_AKESIME_FUSUMA_R_F\012";
static const char data_122C8[42] __attribute__((scn_string_literal)) = "Door Open Motion = SIN_HIKISIME_DOOR_L_F\012";
static const char data_122F2[42] __attribute__((scn_string_literal)) = "Door Open Motion = SIN_HIKISIME_DOOR_R_F\012";
static const char data_1231C[41] __attribute__((scn_string_literal)) = "Door Open Motion = SIN_OSISIME_DOOR_L_F\012";
static const char data_12345[41] __attribute__((scn_string_literal)) = "Door Open Motion = SIN_OSISIME_DOOR_R_F\012";
static const char data_1236E[43] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_AKESIME_FUSUMA_L_F\012";
static const char data_12399[43] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_AKESIME_FUSUMA_R_F\012";
static const char data_123C4[42] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_HIKISIME_DOOR_L_F\012";
static const char data_123EE[42] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_HIKISIME_DOOR_R_F\012";
static const char data_12418[41] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_OSISIME_DOOR_L_F\012";
static const char data_12441[41] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_OSISIME_DOOR_R_F\012";
static const char data_1246A[43] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_AKESIME_FUSUMA_L_F\012";
static const char data_12495[43] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_AKESIME_FUSUMA_R_F\012";
static const char data_124C0[42] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_HIKISIME_DOOR_L_F\012";
static const char data_124EA[42] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_HIKISIME_DOOR_R_F\012";
static const char data_12514[41] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_OSISIME_DOOR_L_F\012";
static const char data_1253D[41] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_OSISIME_DOOR_R_F\012";
static const char data_12566[44] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_OSISIME_DOOR_AP_L_F\012";
static const char data_12592[44] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_OSISIME_DOOR_AP_L_F\012";
static const char data_125BE[44] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_OSISIME_DOOR_AP_L_F\012";
static const char data_125EA[44] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_OSISIME_DOOR_AP_L_F\012";
static const char data_12616[44] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_OSISIME_DOOR_AP_L_F\012";
static const char data_12642[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_1265E[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_1267A[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_12696[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_126B2[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_126CE[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_126EA[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_12706[13] __attribute__((scn_string_literal)) = "open-move!!\012";
static const char data_12713[11] __attribute__((scn_string_literal)) = "M_MDOR.BIN";
static const char data_1271E[] __attribute__((scn_string_literal)) = "This Charactor Work is NULL!!\n";
static const char data_1273D[22] __attribute__((scn_string_literal)) = "手動ドア開け番号指定\012";
static const char data_12753[34] __attribute__((scn_string_literal)) = "無限ループになりそうなので抜ける\012";
static const char data_12775[] __attribute__((scn_string_literal)) = "Don't make BOX Buffer!!\n";
static const char data_1278E[] __attribute__((scn_string_literal)) = "Don't make BOX Buffer!!\n";
static const char data_127A7[15] __attribute__((scn_string_literal)) = "Door System : ";
static const char data_127B6[15] __attribute__((scn_string_literal)) = "Door System : ";
static const char data_127C5[18] __attribute__((scn_string_literal)) = "Door Box is ON!!\012";
static const char data_127D7[19] __attribute__((scn_string_literal)) = "Door Box is OFF!!\012";
static const char data_127EA[18] __attribute__((scn_string_literal)) = "Door Box is ON!!\012";
static const char data_127FC[20] __attribute__((scn_string_literal)) = "Door Box is OFF!!\012";
static const DSWKDoorEntry data_12810 = {.slot = 0, .flags = 0, .ctrl_mask_pair = 0x640064U, .coarse_metric = 0.0f, .selection_metric = 0.0f, .view_ang_delta = 0, .group = 0, .global1 = 0xFFFFFFFFU, .global2 = 0xFFFFFFFFU, .global3 = 0xFFFFFFFFU, .sndCommandId = 0xFFFFFFFFU, .global5 = 0xFFFFFFFFU, .global6 = 0xFFFFFFFFU};
static const int32_t data_12844 = -1;
static const uint8_t data_12848[4] = {0};
static const int32_t data_1284C = -1;
static char data_12850[] = "DOOR";
static const uint8_t data_12855[3] = {0};
static char *const data_12858 = data_12850;
static const int32_t data_1285C[3] = {2, 1966763, 2032299};
static const MapObjectRecord data_12868 = {.kind = 2, .resourceIndex = 0, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-27.06f, 0.0f, -1.8f}, .yawFixed16 = -16384};
static const char data_1288C[10] __attribute__((scn_string_literal)) = "Kaidan : ";
static const char data_12896[32] __attribute__((scn_string_literal)) = "========== initializing kaidan\012";
static const char data_128B6[48] __attribute__((scn_string_literal)) = "please ENDOFKDATA use can't start shuki kaidan\012";
static const char data_128E6[] __attribute__((scn_string_literal)) = "Kaidan Function Error 1\n";
static const char data_128FF[] __attribute__((scn_string_literal)) = "Kaidan Function Error 2\n";
static const char data_12918[] __attribute__((scn_string_literal)) = "Kaidan Data Error\n";
static const char data_1292B[10] __attribute__((scn_string_literal)) = "Kaidan : ";
static const char data_12935[28] __attribute__((scn_string_literal)) = "wait ... shuki shuki shuki\012";
static const char data_12951[10] __attribute__((scn_string_literal)) = "Kaidan : ";
static const char data_1295B[24] __attribute__((scn_string_literal)) = "KAIDAN : Warning coli!\012";
static const char data_12973[24] __attribute__((scn_string_literal)) = "KAIDAN : Warning coli!\012";
static const char data_1298B[24] __attribute__((scn_string_literal)) = "KAIDAN : Warning coli!\012";
static const char data_129A3[24] __attribute__((scn_string_literal)) = "KAIDAN : Warning coli!\012";
static const char data_129BB[24] __attribute__((scn_string_literal)) = "KAIDAN : Warning coli!\012";
static const char data_129D3[24] __attribute__((scn_string_literal)) = "KAIDAN : Warning setY!\012";
static const char data_129EB[24] __attribute__((scn_string_literal)) = "KAIDAN : Warning setY!\012";
static const char data_12A03[241] __attribute__((scn_string_literal)) = "Kaidan : ";
static const int32_t data_12AF4[342] = {
  0, 0, -10922, 0, 0, -10558, 0, 1456, 0, 0, 0, -18022, 0, 0, -17112, 0, 0, -18386, 0, 0, 0, 0, 0, -12196, 0, 0, -18204, 0, 0, -16748, 0, 0, 0, 0, 0,
  -11832, 0, 0, -18386, 0, 0, -15109, 0, 0, 0, 0, 910, -12014, 0, -2184, -16748, 0, -2184, -10012, 0, 0, 0, 0, -364, -11650, -2912, -364, -4369, 1638,
  910, 182, 0, 0, -16019, 0, 182, -23119, 0, 546, -18932, 0, 0, -1092, 0, 0, -16384, 0, 0, -21845, 0, 0, -18750, 0, 0, -546, 0, 0, -21481, 0, 0,
  -21299, 0, 0, -18568, 0, 0, -728, 0, 910, -18750, 0, 182, -18750, 0, -4004, -18204, 0, 0, -728, 0, -364, -14017, -2912, -364, -6735, 1638, 910,
  2548, 0, 0, -18568, 0, 182, -25668, 0, 546, -21481, 0, 0, -3640, 0, 0, -18932, 0, 0, -24393, 0, 0, -21299, 0, 0, -3094, 0, 0, -24029, 0, 0, -23847,
  0, 0, -21117, 0, 0, -3276, 0, 910, -21299, 0, 182, -21299, 0, -4004, -20753, 0, 0, -3276, 0, 0, -5461, 0, -2366, -1092, 0, -182, 1092, 0, -546,
  -6735, 0, 0, -4004, 0, 0, -2912, 0, -182, 1274, 0, 0, -13289, 0, 0, -5279, 0, 0, -8009, 0, 0, 1456, 0, 182, -8738, 0, 0, -6007, 0, 0, -8738, 0, 182,
  1638, 0, -728, -5279, 0, -1274, -6735, 0, -910, -8738, 0, 0, 1638, 0, 0, -5461, 0, -2366, -1092, 0, -182, 1092, 0, -546, -13289, 0, 0, -5643, 0, 0,
  -8009, 0, -182, 1274, 0, 0, -11832, 0, 0, -5279, 0, 0, -8009, 0, 0, 1456, 0, 182, -8738, 0, 0, -6007, 0, 0, -8738, 0, 182, 1638, 0, -728, -5279, 0,
  -1274, -6735, 0, -910, -8738, 0, 0, 1092, 0, 0, -5461, 0, -2366, -1092, 0, -182, 1092, 0, -546, -3640, 0, 0, -2366, 0, 0, -1456, 0, -182, 1274, 0,
  0, -3822, 0, 0, -2730, 0, 0, -1456, 0, 0, 1456, 0, 182, -3822, 0, 0, -2912, 0, 0, -1638, 0, 182, 1638, 0, -728, -3640, 0, -910, -2730, 0, -546,
  -1638, 0, 0, 1092
};
static const int32_t data_1304C[342] = {
  0, 0, -5461, 0, -2366, -1092, 0, -182, 1092, 0, -546, -6735, 0, 0, -4004, 0, 0, -6553, 0, -182, 1274, 0, 0, -13289, 0, 0, -5279, 0, 0, -8009, 0, 0,
  1456, 0, 182, -8738, 0, 0, -6007, 0, 0, -8738, 0, 182, 1638, 0, -728, -5279, 0, -1274, -6735, 0, -910, -8738, 0, 0, 1638, 0, 0, -9102, 0, -2366,
  -2002, 0, -182, 910, 0, -546, -2366, 0, 0, -2002, 0, 0, -2912, 0, -182, 1274, 0, 0, -14199, 0, 0, -11468, 0, 0, -13653, 0, 0, 1456, 0, 182, -11286,
  0, 0, -12014, 0, 0, -12743, 0, 182, 1638, 0, -728, -10194, 0, -1274, -12196, 0, -910, -10922, -728, 546, 1638, 0, 0, -1820, 0, -2366, -728, 0, -364,
  -1092, 0, -546, -11832, 0, 910, -6735, 0, 0, -6553, 0, -182, 1274, 0, 0, -13289, 0, 0, -6007, 0, 0, -10012, 0, 0, 1092, 0, 182, -8738, 0, 0, -6735,
  0, 0, -9830, 0, 182, 1638, 0, -728, -6553, 0, -1274, -7281, 0, -910, -10740, 0, 0, 1638, 0, 0, -5461, -2912, -2366, -364, -5279, 2548, -910, 0,
  -546, -8374, 0, 0, -9466, 0, 910, -4915, 0, -182, 546, 0, 0, -13289, 0, 0, -5279, 0, 364, -7645, 0, -182, 1456, 0, 182, -8738, 0, 0, -7281, 0, 182,
  -6917, 0, -182, 1638, 0, -728, -5279, 0, -1274, -6735, 0, -182, -8009, 0, 0, 1638, -4733, 0, -8556, 0, -2366, -1092, -6189, 2912, 0, 0, -546,
  -10922, 0, 1820, -16384, 0, 0, -11468, 0, 182, 546, 0, 0, -10376, 0, 0, -15837, 0, 0, -12743, 0, -182, 728, 0, 182, -13835, 0, 0, -16201, 0, 364,
  -10922, 0, -182, 728, 0, -728, -9648, 0, -3458, -12379, 0, -728, -7463, 0, 0, 1092, 0, 0, -5461, 0, -2366, -1638, 0, -910, 1092, 0, -546, -3640, 0,
  0, -2366, 0, 0, -1456, 0, -182, 1638, 0, 0, -910, 0, 0, -2730, 0, 0, -1274, 0, 0, 1820, 0, 182, -2730, 0, 0, -1820, 0, 0, -1274, 0, 182, 2184, 0,
  -728, -3094, 0, -910, -2548, 0, 910, -1456, -546, 910, 2366
};
static const uint32_t data_135A4 = 0xC1B40000;
typedef struct Row_135A8 { float w0; float w1; int32_t w2; int32_t w3; int32_t w4; float w5; } Row_135A8;
static const Row_135A8 data_135A8[4] = {
  {0.2f, -18.95f, 49152, 1048832, 24, -15.6f},
  {5.3f, -18.95f, 16384, 265, 24, -22.5f},
  {0.2f, -0.9f, 49152, 1048832, 24, -15.6f},
  {5.3f, -0.9f, 16384, 265, 24, 100000.0f}
};
static const uint8_t data_13608[8] = {0};
typedef struct Row_13610 { float w0; int32_t w1; int32_t w2; } Row_13610;
static const Row_13610 data_13610[3] = {
  {0.55f, 0, 1},
  {0.55f, 0, 2},
  {0.55f, 0, 3}
};
static const float data_13634 = 0.55f;
static const int32_t data_13638 = 3;
static const uint32_t data_1363C = 0xFFFF0000;
static const float data_13640[7] = {0.0f, 39936.0f, -14.8103f, 0.3216f, -13.2794f, 0.0f, 0.0f};
static const int32_t data_1365C[2] = {-65536, 1};
static const float data_13664[6] = {40128.0f, -4.5059f, 7.3896f, -5.9561f, 0.0f, 0.0f};
static const int32_t data_1367C[2] = {-65536, 2};
static const float data_13684[6] = {40128.0f, -4.5059f, 7.3896f, -2.2016f, 0.0f, 0.0f};
static const int32_t data_1369C[2] = {-65536, 3};
static const uint32_t data_136A4[2] = {0x638E0000U, 0xC1923368U};
static const float data_136AC[4] = {1.41f, -19.5222f, 0.0f, 0.0f};
static const int32_t data_136BC = 131072;
static const shen_name data_136C0 = SHEN_NAME('SARA');
static const uint32_t data_136C4[6] = {0x4000E000U, 0, 0, 0, 0, 0};
static const int32_t data_136DC = 131072;
static const shen_name data_136E0 = SHEN_NAME('TUB1');
static const uint32_t data_136E4 = 0x4000E000U;
static const float data_136E8[5] = {-0.14f, 0.2f, 0.14f, 0.0f, 0.0f};
static const int32_t data_136FC = 131072;
static const shen_name data_13700 = SHEN_NAME('TUB1');
static const uint32_t data_13704 = 0x40006000U;
static const float data_13708[5] = {0.14f, 0.2f, -0.14f, 0.0f, 0.0f};
static const int32_t data_1371C = 131072;
static const shen_name data_13720 = SHEN_NAME('TUB2');
static const uint32_t data_13724 = 0x2AAAC000U;
static const float data_13728[5] = {-0.2f, 0.3f, 0.0f, 0.0f, 0.0f};
static const int32_t data_1373C = 131072;
static const shen_name data_13740 = SHEN_NAME('TUB2');
static const uint32_t data_13744 = 0x2AAA4000U;
static const float data_13748[6] = {0.2f, 0.3f, 0.0f, 0.0f, 0.0f, 0.0f};
static const shen_name data_13760 = SHEN_NAME('TUB3');
static const uint32_t data_13764 = 0x4000C000U;
static const float data_13768[6] = {-0.3f, 0.3f, 0.0f, 0.0f, 0.0f, 0.0f};
static const shen_name data_13780 = SHEN_NAME('TUB3');
static const uint32_t data_13784 = 0x40004000U;
static const float data_13788[6] = {0.3f, 0.3f, 0.0f, 0.0f, 0.0f, 0.0f};
static const shen_name data_137A0 = SHEN_NAME('TUB4');
static const uint32_t data_137A4 = 0x4000C000U;
static const float data_137A8[6] = {-0.1f, 0.2f, 0.0f, 0.0f, 0.0f, 0.0f};
static const shen_name data_137C0 = SHEN_NAME('TUB4');
static const uint32_t data_137C4 = 0x40004000U;
static const float data_137C8[5] = {0.1f, 0.2f, 0.0f, 0.0f, 0.0f};
static const int32_t data_137DC = 131072;
static const shen_name data_137E0 = SHEN_NAME('TUB5');
static const uint32_t data_137E4 = 0x4000C000U;
static const float data_137E8[5] = {-0.1f, 0.15f, 0.0f, 0.0f, 0.0f};
static const int32_t data_137FC = 131072;
static const shen_name data_13800 = SHEN_NAME('TUB5');
static const uint32_t data_13804 = 0x40004000U;
static const float data_13808[5] = {0.1f, 0.15f, 0.0f, 0.0f, 0.0f};
static const int32_t data_1381C = 131072;
static const shen_name data_13820 = SHEN_NAME('TUB6');
static const uint32_t data_13824[2] = {0x4000E000U, 0};
static const float data_1382C[4] = {0.15f, 0.0f, 0.0f, 0.0f};
static const int32_t data_1383C = 131072;
static const shen_name data_13840 = SHEN_NAME('TUB6');
static const uint32_t data_13844[2] = {0x40006000U, 0};
static const float data_1384C[5] = {0.15f, 0.0f, 0.0f, 0.0f, 0.0f};
static const shen_name data_13860[3] = {SHEN_NAME('OJK0'), 0, 0};
static const float data_1386C[5] = {0.5f, -0.5f, 0.0f, 0.0f, 0.0f};
static const shen_name data_13880 = SHEN_NAME('OJK1');
static const uint32_t data_13884 = 0x5555EAAAU;
static const float data_13888[6] = {-0.2f, 0.5f, 0.25f, 0.0f, 0.0f, 0.0f};
static const shen_name data_138A0[3] = {SHEN_NAME('OJK2'), 0, 0};
static const float data_138AC[5] = {0.5f, -0.5f, 0.0f, 0.0f, 0.0f};
static const shen_name data_138C0[4] = {SHEN_NAME('OJK3'), 0, 0, 0};
static const float data_138D0[4] = {-0.5f, 0.0f, 0.0f, 0.0f};
static const shen_name data_138E0[3] = {SHEN_NAME('OJK4'), 0, 0};
static const float data_138EC[5] = {0.5f, -0.5f, 0.0f, 0.0f, 0.0f};
static const shen_name data_13900[3] = {SHEN_NAME('OJL0'), 0, 0};
static const float data_1390C[5] = {0.5f, -0.5f, 0.0f, 0.0f, 0.0f};
static const shen_name data_13920[3] = {SHEN_NAME('OJL1'), 0, 0};
static const float data_1392C[5] = {0.5f, -0.5f, 0.0f, 0.0f, 0.0f};
static const shen_name data_13940 = SHEN_NAME('OJL2');
static const uint32_t data_13944 = 0x55552AAAU;
static const float data_13948[6] = {0.2f, 0.0f, 0.15f, 0.0f, 0.0f, 0.0f};
static const shen_name data_13960 = SHEN_NAME('OJL2');
static const uint32_t data_13964 = 0x5555AAAAU;
static const float data_13968[6] = {-0.2f, 0.0f, -0.15f, 0.0f, 0.0f, 0.0f};
static const shen_name data_13980 = SHEN_NAME('OJL3');
static const uint32_t data_13984 = 0x5555E000U;
static const float data_13988[6] = {-0.2f, 0.0f, 0.2f, 0.0f, 0.0f, 0.0f};
static const shen_name data_139A0 = SHEN_NAME('OJL3');
static const uint32_t data_139A4 = 0x55556000U;
static const float data_139A8[5] = {0.2f, 0.0f, -0.2f, 0.0f, 0.0f};
static const int32_t data_139BC = 131072;
static const shen_name data_139C0 = SHEN_NAME('KAB1');
static const uint32_t data_139C4[2] = {0x2AAAE000U, 0};
static const float data_139CC[4] = {0.1f, 0.0f, 0.0f, 0.0f};
static const int32_t data_139DC = 131072;
static const shen_name data_139E0 = SHEN_NAME('KAB2');
static const uint32_t data_139E4[2] = {0x2AAAC000U, 0};
static const float data_139EC[5] = {0.1f, 0.0f, 0.0f, 0.0f, 0.0f};
static const shen_name data_13A00 = SHEN_NAME('KIBK');
static const uint32_t data_13A04 = 0x4000C000U;
static const float data_13A08[6] = {-0.5f, 0.25f, -0.2f, 0.0f, 0.0f, 0.0f};
static const shen_name data_13A20 = SHEN_NAME('KIBK');
static const uint32_t data_13A24 = 0x40004000U;
static const float data_13A28[6] = {0.1f, 0.3f, -0.2f, 0.0f, 0.0f, 0.0f};
static const shen_name data_13A40 = SHEN_NAME('KBK2');
static const uint32_t data_13A44[2] = {0x55554000U, 0};
static const float data_13A4C[5] = {1.1f, 0.0f, 0.0f, 0.0f, 0.0f};
static const shen_name data_13A60 = SHEN_NAME('WBOD');
static const uint32_t data_13A64 = 0x55558000U;
static const float data_13A68[6] = {0.16f, -0.48f, 0.0f, 0.0f, 0.0f, 0.0f};
static const shen_name data_13A80 = SHEN_NAME('OJA0');
static const int32_t data_13A84[2] = {32768, 0};
static const float data_13A8C[5] = {0.5f, -0.5f, 0.0f, 0.0f, 0.0f};
static const shen_name data_13AA0 = SHEN_NAME('OJA1');
static const int32_t data_13AA4[2] = {32768, 0};
static const float data_13AAC[5] = {0.5f, -0.5f, 0.0f, 0.0f, 0.0f};
static const shen_name data_13AC0 = SHEN_NAME('OJA2');
static const int32_t data_13AC4[2] = {32768, 0};
static const float data_13ACC[5] = {0.5f, -0.5f, 0.0f, 0.0f, 0.0f};
static const shen_name data_13AE0 = SHEN_NAME('OJC0');
static const int32_t data_13AE4[2] = {32768, 0};
static const float data_13AEC[5] = {0.5f, -0.5f, 0.0f, 0.0f, 0.0f};
static const shen_name data_13B00 = SHEN_NAME('OJB0');
static const int32_t data_13B04[2] = {32768, 0};
static const float data_13B0C[5] = {0.5f, -0.5f, 0.0f, 0.0f, 0.0f};
static const shen_name data_13B20 = SHEN_NAME('OJB2');
static const int32_t data_13B24[2] = {32768, 0};
static const float data_13B2C[5] = {0.5f, -0.5f, 0.0f, 0.0f, 0.0f};
static const shen_name data_13B40 = SHEN_NAME('OJD0');
static const uint32_t data_13B44[3] = {0x4000E000U, 0, 0};
static const float data_13B50[4] = {0.21f, 0.0f, 0.0f, 0.0f};
static const shen_name data_13B60 = SHEN_NAME('OJD0');
static const uint32_t data_13B64[3] = {0x40006000U, 0, 0};
static const float data_13B70[4] = {-0.21f, 0.0f, 0.0f, 0.0f};
static const shen_name data_13B80 = SHEN_NAME('OJD1');
static const uint32_t data_13B84[2] = {0x4000C000U, 0};
static const float data_13B8C[5] = {0.1f, 0.3f, 0.0f, 0.0f, 0.0f};
static const shen_name data_13BA0 = SHEN_NAME('OJD1');
static const uint32_t data_13BA4[2] = {0x40004000U, 0};
static const float data_13BAC[5] = {0.1f, -0.3f, 0.0f, 0.0f, 0.0f};
static const shen_name data_13BC0 = SHEN_NAME('OJE0');
static const uint32_t data_13BC4 = 0x5555C000U;
static const float data_13BC8[6] = {0.2f, 0.0f, -0.2f, 0.0f, 0.0f, 0.0f};
static const shen_name data_13BE0 = SHEN_NAME('OJE0');
static const uint32_t data_13BE4 = 0x55554000U;
static const float data_13BE8[6] = {-0.2f, 0.0f, 0.2f, 0.0f, 0.0f, 0.0f};
static const shen_name data_13C00 = SHEN_NAME('OJH0');
static const uint32_t data_13C04 = 0x5555E000U;
static const float data_13C08[6] = {0.2f, 0.0f, 0.2f, 0.0f, 0.0f, 0.0f};
static const shen_name data_13C20 = SHEN_NAME('OJH0');
static const uint32_t data_13C24 = 0x5555671CU;
static const float data_13C28[6] = {-0.2f, 0.0f, -0.2f, 0.0f, 0.0f, 0.0f};
static const shen_name data_13C40 = SHEN_NAME('OJI0');
static const uint32_t data_13C44[3] = {0x4000E000U, 0, 0};
static const float data_13C50[4] = {0.14f, 0.0f, 0.0f, 0.0f};
static const shen_name data_13C60 = SHEN_NAME('OJI0');
static const uint32_t data_13C64[3] = {0x40006000U, 0, 0};
static const float data_13C70[4] = {-0.14f, 0.0f, 0.0f, 0.0f};
static const shen_name data_13C80 = SHEN_NAME('OJF0');
static const int32_t data_13C84[2] = {57344, 0};
static const float data_13C8C[5] = {0.5f, -1.3f, 0.0f, 0.0f, 0.0f};
static const shen_name data_13CA0 = SHEN_NAME('OJG0');
static const uint32_t data_13CA4[3] = {0x4000E000U, 0, 0};
static const float data_13CB0[4] = {0.21f, 0.0f, 0.0f, 0.0f};
static const shen_name data_13CC0 = SHEN_NAME('OJG0');
static const uint32_t data_13CC4[3] = {0x40006000U, 0, 0};
static const float data_13CD0[4] = {-0.21f, 0.0f, 0.0f, 0.0f};
static const shen_name data_13CE0 = SHEN_NAME('OJJ0');
static const uint32_t data_13CE4[3] = {0x4000C000U, 0, 0};
static const float data_13CF0[4] = {0.3f, 0.0f, 0.0f, 0.0f};
static const shen_name data_13D00 = SHEN_NAME('OJJ0');
static const uint32_t data_13D04[3] = {0x40004000U, 0, 0};
static const float data_13D10[3] = {-0.3f, 0.0f, 0.0f};
static const int32_t data_13D1C = 65535;
static const int32_t data_13D20[57] = {
  0, 0, -3276, 0, 0, 0, 0, 0, 0, 0, 0, -6553, 0, 0, -4369, 0, -546, -3458, 0, 0, 0, 0, 0, -5461, 0, 0, -5643, 0, 0, -3458, 0, 0, 0, 0, 0, -6007, 0, 0,
  -4551, 0, 546, -2548, 0, 0, 0, 0, 0, -6189, 0, -1092, -3094, 0, 0, -2548, 0, 0, 0
};
static const int32_t data_13E04[57] = {
  0, 0, -6007, 0, 0, -728, 0, 0, 0, 0, 0, -5097, 0, 0, -2184, 0, 0, -2002, 0, 0, 0, 0, 0, -5825, 0, 0, -2912, 0, 0, -728, 0, 0, 0, 0, 0, -4915, 0, 0,
  -4187, 0, 0, -1274, 0, 0, 0, 0, 0, -6553, 0, -364, -4369, 0, -546, -1456, 0, 0, 0
};
static const int32_t data_13EE8[57] = {
  0, -1273, -729, 0, 0, -4369, 0, 1456, 0, 0, 0, -18022, 0, 0, -17112, 0, 0, -18386, 0, 0, 0, 0, 0, -12196, 0, 0, -18204, 0, 0, -16748, 0, 0, 0, 0, 0,
  -11832, 0, 0, -18386, 0, 0, -15109, 0, 0, 0, 0, 910, -12014, 0, -2184, -16748, 0, -2184, -10012, 0, 0, 0
};
static const char data_13FCC[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_13FD1[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_13FD6[11] __attribute__((scn_string_literal)) = "tsm005.snd";
static const char data_13FE1[13] __attribute__((scn_string_literal)) = "a1_chide.snd";
static const char data_13FEE[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_13FF3[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_13FF8[11] __attribute__((scn_string_literal)) = "tsm005.snd";
static const char data_14003[13] __attribute__((scn_string_literal)) = "a1_chide.snd";
static const uint32_t data_14010 = 0x3F800000;
static const float data_14014[11] = {1.0f, 1.0f, 1.7f, -0.01f, 0.01f, 0.01f, -18.42f, 6.27f, -17.25f, 2.26f, 0.71f};
static const int32_t data_14040 = 4000;
static const Vector4 data_14044 = {.x = 1.0f, .y = 0.5f, .z = 0.5f, .w = 0.5f};
static const uint32_t data_14054 = 0x3F800000;
static const float data_14058[11] = {1.0f, 1.0f, 1.7f, -0.01f, 0.01f, 0.01f, -18.42f, 6.27f, -17.25f, 2.26f, 0.71f};
static const int32_t data_14084 = 4000;
static uint32_t data_14088 = 152;
static const int32_t data_1408C[12] = {153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164};
static const char data_140BC[] __attribute__((scn_string_literal)) = "DAT1";
static const uint8_t data_140C1[3] = {0};
static const Vector3 data_140C4 = {.x = -18.3f, .y = 9.6f, .z = -18.4f};
static const Vector3i data_140D0 = {.x = 0, .y = 0, .z = 1820};
static const Vector4 data_140DC = {.x = 1.0f, .y = 0.5f, .z = 0.5f, .w = 0.5f};
static const Vector4 data_140EC = {.x = 1.0f, .y = 0.3f, .z = 0.3f, .w = 0.3f};
static const uint32_t data_140FC = 380;
static const int32_t data_14100[6] = {1, 390, 0, 1018, 0, -1};
static const uint32_t data_14118 = 510;
static const int32_t data_1411C[6] = {1, 525, 0, 1020, 0, -1};
static const uint32_t data_14134 = 150;
static const int32_t data_14138[6] = {1, 160, 0, 1019, 0, -1};
static const uint32_t data_14150 = 530;
static const int32_t data_14154[6] = {1, 970, 0, 1014, 0, -1};
static const uint32_t data_1416C = 911;
static const int32_t data_14170[6] = {1, 155, 0, 1015, 0, -1};
static const char data_14188[7] __attribute__((scn_string_literal)) = "FREE02";
static const char data_1418F[7] __attribute__((scn_string_literal)) = "FREE03";
static const char data_14196[7] __attribute__((scn_string_literal)) = "FREE04";
static const char data_1419D[7] __attribute__((scn_string_literal)) = "FREE01";
static const char data_141A4[] __attribute__((scn_string_literal)) = "SCENE/02/MS08";
static const char data_141B2[] __attribute__((scn_string_literal)) = "MS08";
static const uint8_t data_141B7[1] = {0};
static const Vector3 data_141B8 = {.x = -16.9769f, .y = 1.3342f, .z = -11.8746f};
static const Vector3i data_141C4 = {.x = 22461, .y = 8192, .z = 32762};
static const char data_141D0[13] __attribute__((scn_string_literal)) = "SEQDATA0.BIN";
static const char data_141DD[5] __attribute__((scn_string_literal)) = "MS08";
static const char data_141E2[13] __attribute__((scn_string_literal)) = "SEQDATA1.BIN";
static const char data_141EF[5] __attribute__((scn_string_literal)) = "MS08";
static const char data_141F4[13] __attribute__((scn_string_literal)) = "SEQDATA3.BIN";
static const char data_14201[5] __attribute__((scn_string_literal)) = "MS08";
static const char data_14206[13] __attribute__((scn_string_literal)) = "SEQDATA6.BIN";
static const char data_14213[5] __attribute__((scn_string_literal)) = "MS08";
static const char data_14218[6] __attribute__((scn_string_literal)) = "A0154";
static const char data_1421E[6] __attribute__((scn_string_literal)) = "E1010";
static const Vector4 data_14224 = {.x = 1.0f, .y = 0.2f, .z = 0.2f, .w = 0.2f};
static const Vector3 data_14234 = {.x = 1.1f, .y = 1.1f, .z = 1.1f};
static const Vector3 data_14240 = {.x = 1.0f, .y = 0.6f, .z = 1.0f};
static const Vector3 data_1424C = {.x = 0.0f, .y = 0.0f, .z = 0.0f};
static const Vector3 data_14258 = {.x = 1.0f, .y = 1.0f, .z = 1.0f};
static const Vector4 data_14264 = {.x = 1.0f, .y = 0.83f, .z = 0.7f, .w = 0.53f};
static const float data_14274[2] = {0.9f, 1.0f};
static const char data_1427C[10] __attribute__((scn_string_literal)) = "E1010A008";
static const char data_14286[10] __attribute__((scn_string_literal)) = "E1010A016";
static const char data_14290[10] __attribute__((scn_string_literal)) = "E1010A087";
static const char data_1429A[10] __attribute__((scn_string_literal)) = "E1010A033";
static const char data_142A4[10] __attribute__((scn_string_literal)) = "E1010A084";
static const char data_142AE[10] __attribute__((scn_string_literal)) = "E1010A011";
static const Vector3 data_142B8 = {.x = -25.0145f, .y = 0.7752f, .z = -13.5594f};
static const char data_142C4[10] __attribute__((scn_string_literal)) = "E1010A074";
static const char data_142CE[10] __attribute__((scn_string_literal)) = "E1010A075";
static const char data_142D8[10] __attribute__((scn_string_literal)) = "E1010A076";
static const char data_142E2[10] __attribute__((scn_string_literal)) = "E1010A039";
static const char data_142EC[10] __attribute__((scn_string_literal)) = "E1010A077";
static const char data_142F6[10] __attribute__((scn_string_literal)) = "E1010A078";
static const char data_14300[10] __attribute__((scn_string_literal)) = "E1010A076";
static const char data_1430A[10] __attribute__((scn_string_literal)) = "E1010A039";
static const char data_14314[10] __attribute__((scn_string_literal)) = "E1010A077";
static const char data_1431E[10] __attribute__((scn_string_literal)) = "E1010A078";
static const char data_14328[10] __attribute__((scn_string_literal)) = "E1010A081";
static const char data_14332[10] __attribute__((scn_string_literal)) = "E1010A076";
static const char data_1433C[10] __attribute__((scn_string_literal)) = "E1010A039";
static const char data_14346[10] __attribute__((scn_string_literal)) = "E1010A077";
static const char data_14350[10] __attribute__((scn_string_literal)) = "E1010A078";
static const char data_1435A[10] __attribute__((scn_string_literal)) = "E1010A083";
static const char data_14364[10] __attribute__((scn_string_literal)) = "E1010A007";
static const char data_1436E[10] __attribute__((scn_string_literal)) = "E1010A079";
static const char data_14378[10] __attribute__((scn_string_literal)) = "E1010A080";
static const char data_14382[10] __attribute__((scn_string_literal)) = "E1010A087";
static const char data_1438C[10] __attribute__((scn_string_literal)) = "E1010A088";
static const char data_14396[10] __attribute__((scn_string_literal)) = "E1010A025";
static const char data_143A0[10] __attribute__((scn_string_literal)) = "E1010A089";
static const char data_143AA[10] __attribute__((scn_string_literal)) = "E1010A041";
static const char data_143B4[10] __attribute__((scn_string_literal)) = "E1010A042";
static const char data_143BE[10] __attribute__((scn_string_literal)) = "E1010A082";
static const char data_143C8[10] __attribute__((scn_string_literal)) = "E1010A028";
static const char data_143D2[10] __attribute__((scn_string_literal)) = "E1010A085";
static const char data_143DC[10] __attribute__((scn_string_literal)) = "E1010A043";
static const char data_143E6[10] __attribute__((scn_string_literal)) = "E1010A044";
static const char data_143F0[10] __attribute__((scn_string_literal)) = "E1010A045";
static const char data_143FA[10] __attribute__((scn_string_literal)) = "E1010A042";
static const char data_14404[10] __attribute__((scn_string_literal)) = "E1010A042";
static const char data_1440E[10] __attribute__((scn_string_literal)) = "E1010A018";
static const char data_14418[10] __attribute__((scn_string_literal)) = "E1010A085";
static const char data_14422[10] __attribute__((scn_string_literal)) = "E1010A036";
static const char data_1442C[10] __attribute__((scn_string_literal)) = "E1010A037";
static const char data_14436[10] __attribute__((scn_string_literal)) = "E1010A014";
static const char data_14440[10] __attribute__((scn_string_literal)) = "E1010A017";
static const char data_1444A[10] __attribute__((scn_string_literal)) = "E1010A049";
static const char data_14454[10] __attribute__((scn_string_literal)) = "E1010A050";
static const char data_1445E[10] __attribute__((scn_string_literal)) = "E1010A051";
static const char data_14468[10] __attribute__((scn_string_literal)) = "E1010A039";
static const char data_14472[10] __attribute__((scn_string_literal)) = "E1010A055";
static const char data_1447C[10] __attribute__((scn_string_literal)) = "E1010A015";
static const char data_14486[10] __attribute__((scn_string_literal)) = "E1010A052";
static const char data_14490[10] __attribute__((scn_string_literal)) = "E1010A053";
static const char data_1449A[10] __attribute__((scn_string_literal)) = "E1010A054";
static const char data_144A4[10] __attribute__((scn_string_literal)) = "E1010A070";
static const char data_144AE[10] __attribute__((scn_string_literal)) = "E1010A071";
static const char data_144B8[10] __attribute__((scn_string_literal)) = "E1010A072";
static const char data_144C2[10] __attribute__((scn_string_literal)) = "E1010A090";
static const char data_144CC[10] __attribute__((scn_string_literal)) = "E1010A091";
static const char data_144D6[10] __attribute__((scn_string_literal)) = "E1010A092";
static const char data_144E0[10] __attribute__((scn_string_literal)) = "E1010A064";
static const char data_144EA[10] __attribute__((scn_string_literal)) = "E1010A065";
static const char data_144F4[10] __attribute__((scn_string_literal)) = "E1010A066";
static const char data_144FE[10] __attribute__((scn_string_literal)) = "E1010A067";
static const char data_14508[12] __attribute__((scn_string_literal)) = "E1010A068";
static const Vector3 data_14514 = {.x = -1.34f, .y = 5.4f, .z = -8.85f};
static const Vector3 data_14520 = {.x = -1.34f, .y = 5.4f, .z = -8.75f};
static const char data_1452C[10] __attribute__((scn_string_literal)) = "E1010A019";
static const char data_14536[10] __attribute__((scn_string_literal)) = "E1010A020";
static const char data_14540[10] __attribute__((scn_string_literal)) = "E1010A038";
static const char data_1454A[10] __attribute__((scn_string_literal)) = "E1010A021";
static const char data_14554[12] __attribute__((scn_string_literal)) = "E1010A039";
static const Vector3 data_14560 = {.x = 0.137f, .y = 0.05f, .z = 0.027f};
static const Vector3i data_1456C = {.x = 10467, .y = 14563, .z = 0};
static const Vector3 data_14578 = {.x = -27.0413f, .y = 0.0f, .z = -2.244f};
static const Vector3 data_14584 = {.x = -0.9031f, .y = 5.4f, .z = -8.1052f};
static const char data_14590[10] __attribute__((scn_string_literal)) = "E1010A022";
static const char data_1459A[10] __attribute__((scn_string_literal)) = "E1010A086";
static const char data_145A4[6] __attribute__((scn_string_literal)) = "E1010";
static const char data_145AA[11] __attribute__((scn_string_literal)) = "tsm005.snd";
static const char data_145B5[13] __attribute__((scn_string_literal)) = "SEQDATA0.BIN";
static const char data_145C2[14] __attribute__((scn_string_literal)) = "SCENE/02/MS08";
static const char data_145D0[13] __attribute__((scn_string_literal)) = "SEQDATA1.BIN";
static const char data_145DD[14] __attribute__((scn_string_literal)) = "SCENE/02/MS08";
static const char data_145EB[13] __attribute__((scn_string_literal)) = "SEQDATA3.BIN";
static const char data_145F8[14] __attribute__((scn_string_literal)) = "SCENE/02/MS08";
static const char data_14606[13] __attribute__((scn_string_literal)) = "SEQDATA6.BIN";
static const char data_14613[14] __attribute__((scn_string_literal)) = "SCENE/02/MS08";
static const char data_14621[5] __attribute__((scn_string_literal)) = "1 .c";
static const char data_14626[5] __attribute__((scn_string_literal)) = "2 .c";
static const char data_1462B[11] __attribute__((scn_string_literal)) = "tsm005.snd";
static const char data_14636[13] __attribute__((scn_string_literal)) = "a1_chide.snd";
static const char data_14643[6] __attribute__((scn_string_literal)) = "A0154";
static const char data_14649[11] __attribute__((scn_string_literal)) = "M_0154.BIN";
static const char data_14654[6] __attribute__((scn_string_literal)) = "A0154";
static const char data_1465A[11] __attribute__((scn_string_literal)) = "tsm005.snd";
static const char data_14665[13] __attribute__((scn_string_literal)) = "a1_chide.snd";
static const char data_14672[6] __attribute__((scn_string_literal)) = "A0154";
static const char data_14678[11] __attribute__((scn_string_literal)) = "M_0154.BIN";
static const char data_14683[9] __attribute__((scn_string_literal)) = "A0154";

static int32_t script_0030(int32_t arg0) {
  if (arg0 < 0) {
    return -arg0;
  } else {
    return arg0;
  }
}

static void script_004E(float arg0) {
  if (!(arg0 >= 0.0f)) {
    (void)((float)0 - arg0);
    return;
  } else {
    (void)arg0;
    return;
  }
}

static int32_t script_006D(int32_t arg0, int32_t arg1) {
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

static void script_00E5(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  int32_t local_FC;
  if (task_160 != arg0) {
    Script_sprintf("Door Index = %d : call_type = %d : type = %d\012", task_48[arg2], arg3, arg4);
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

static int32_t script_0697(int32_t arg0) {
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

static void script_0768(int32_t arg0, int32_t arg1, int32_t arg2) {
  script_948B(arg0, arg2, 0);
  return;
}

static void script_0778(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  float local_F8;
  float local_F4;
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
    local_FC = script_8C20(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
    if ((local_FC == 1) | (local_FC == 2)) {
      local_FC = 1;
    } else {
      local_FC = 0;
    }
    *(int32_t *)&local_F8 = Script_ScenDataUtil(data_11104, 5, Script_ScenDataUtil(data_10B14, 1, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 5);
    *(int32_t *)&local_F4 = Script_ScenDataUtil(data_11104, 5, Script_ScenDataUtil(data_10B14, 1, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 6);
    local_FC = script_8C20(arg0, arg1, 5, local_F8 / 100.0f, local_F4 / 100.0f, arg3, -1);
    script_3F04(arg0, arg1, arg2, arg3, local_FC, 0);
  }
  script_948B(arg0, arg2, 0);
  return;
}

static void script_087C(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  int32_t local_F8;
  float local_F4;
  float local_F0;
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
    local_FC = script_8C20(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
    *(int32_t *)&local_F4 = Script_ScenDataUtil(data_11104, 5, Script_ScenDataUtil(data_10B14, 1, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 5);
    local_F4 = 90.0f - local_F4;
    *(int32_t *)&local_F0 = Script_ScenDataUtil(data_11104, 5, Script_ScenDataUtil(data_10B14, 1, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 6);
    if ((Script_ScenDataUtil(task_0, 4, task_48[arg2] * 13 + 1) & 0x40) | (Script_ScenDataUtil(task_0, 4, task_48[arg2] * 13 + 1) & 0x80)) {
      if (Script_ScenDataUtil(task_0, 4, task_48[arg2] * 13 + 1) & 0x40) {
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
    local_FC = script_8C20(arg0, arg1, local_F8, local_F4 / (float)100, local_F0 / (float)100, arg3, task_48[arg2]);
    script_3F04(arg0, arg1, arg2, arg3, local_FC, 1);
  }
  script_948B(arg0, arg2, 0);
  return;
}

static void script_0A1E(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  float local_F8;
  float local_F4;
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
    local_FC = script_8C20(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
    if ((local_FC == 1) | (local_FC == 2)) {
      local_FC = 1;
    } else {
      local_FC = 0;
    }
    *(int32_t *)&local_F8 = Script_ScenDataUtil(data_11104, 5, Script_ScenDataUtil(data_10B14, 1, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 5);
    *(int32_t *)&local_F4 = Script_ScenDataUtil(data_11104, 5, Script_ScenDataUtil(data_10B14, 1, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 6);
    local_FC = script_8C20(arg0, arg1, 5, local_F8 / 100.0f, local_F4 / 100.0f, arg3, -1);
    if (local_FC == 1) {
      local_FC = 2;
    } else {
      local_FC = 3;
    }
    if ((Script_ScenDataUtil(task_0, 4, task_48[arg2] * 13 + 1) & 4) == 0) {
      script_2411(arg0, arg1, arg2, arg3, local_FC, 0);
    } else {
      if (arg0 == task_160) {
        script_57E4(arg0, arg1, arg2, arg3, local_FC, 0);
      }
    }
  }
  script_948B(arg0, arg2, 2);
  return;
}

static void script_0B7F(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  float local_F8;
  float local_F4;
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
    local_FC = script_8C20(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
    if ((local_FC == 1) | (local_FC == 2)) {
      local_FC = 1;
    } else {
      local_FC = 0;
    }
    *(int32_t *)&local_F8 = Script_ScenDataUtil(data_11104, 5, Script_ScenDataUtil(data_10B14, 1, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 5);
    *(int32_t *)&local_F4 = Script_ScenDataUtil(data_11104, 5, Script_ScenDataUtil(data_10B14, 1, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 6);
    local_FC = script_8C20(arg0, arg1, 5, local_F8 / 100.0f, local_F4 / 100.0f, arg3, -1);
    if (local_FC == 1) {
      local_FC = 1;
    } else {
      local_FC = 0;
    }
    if ((Script_ScenDataUtil(task_0, 4, task_48[arg2] * 13 + 1) & 4) == 0) {
      script_2411(arg0, arg1, arg2, arg3, local_FC, 0);
    } else {
      if (arg0 == task_160) {
        script_57E4(arg0, arg1, arg2, arg3, local_FC, 0);
      }
    }
  }
  script_948B(arg0, arg2, 2);
  return;
}

static void script_0CE0(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  int32_t local_F8;
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
    if ((Script_ScenDataUtil(task_0, 4, task_48[arg2] * 13 + 1) & 4) == 0) {
      float local_F4;
      float local_F0;
      local_FC = script_8C20(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
      *(int32_t *)&local_F4 = Script_ScenDataUtil(data_11104, 5, Script_ScenDataUtil(data_10B14, 1, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 5);
      *(int32_t *)&local_F0 = Script_ScenDataUtil(data_11104, 5, Script_ScenDataUtil(data_10B14, 1, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 6);
      if ((Script_ScenDataUtil(task_0, 4, task_48[arg2] * 13 + 1) & 0x40) | (Script_ScenDataUtil(task_0, 4, task_48[arg2] * 13 + 1) & 0x80)) {
        if (Script_ScenDataUtil(task_0, 4, task_48[arg2] * 13 + 1) & 0x40) {
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
      local_FC = script_8C20(arg0, arg1, local_F8, local_F4 / 100.0f, local_F0 / 100.0f, arg3, task_48[arg2]);
      script_2411(arg0, arg1, arg2, arg3, local_FC, 1);
    } else {
      if (arg0 == task_160) {
        float local_F4;
        float local_F0;
        local_FC = script_8C20(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
        *(int32_t *)&local_F4 = Script_ScenDataUtil(data_11104, 5, Script_ScenDataUtil(data_10B14, 1, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 5);
        *(int32_t *)&local_F0 = Script_ScenDataUtil(data_11104, 5, Script_ScenDataUtil(data_10B14, 1, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 6);
        local_FC = script_8C20(arg0, arg1, 6, local_F4 / 100.0f, local_F0 / 100.0f, arg3, task_48[arg2]);
        script_57E4(arg0, arg1, arg2, arg3, local_FC, 1);
      }
    }
  }
  script_948B(arg0, arg2, 2);
  return;
}

static void script_0F7E(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  float local_F8;
  float local_F4;
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
    local_FC = script_8C20(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
    *(int32_t *)&local_F8 = Script_ScenDataUtil(data_11104, 5, Script_ScenDataUtil(data_10B14, 1, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 5);
    *(int32_t *)&local_F4 = Script_ScenDataUtil(data_11104, 5, Script_ScenDataUtil(data_10B14, 1, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 6);
    script_8C20(arg0, arg1, 0, local_F8 / 100.0f, local_F4 / 100.0f, arg3, -1);
  }
  script_948B(arg0, arg2, 0);
  return;
}

static void script_104C(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
    local_FC = script_8C20(arg0, arg1, 4, 0.5f, 0.5f, arg3, -1);
    if (local_FC == 1) {
      local_FC = 2;
    } else {
      local_FC = 3;
    }
    script_3390(arg0, arg1, arg2, arg3, local_FC, 1);
  }
  script_948B(arg0, arg2, 1);
  return;
}

static void script_10B3(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
    local_FC = script_8C20(arg0, arg1, 4, 0.5f, 0.5f, arg3, -1);
    if (local_FC == 1) {
      local_FC = 1;
    } else {
      local_FC = 0;
    }
    script_3390(arg0, arg1, arg2, arg3, local_FC, 1);
  }
  script_948B(arg0, arg2, 1);
  return;
}

static void script_111A(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
    script_8C20(arg0, arg1, 4, 0.0f, 0.5f, arg3, -1);
    script_3390(arg0, arg1, arg2, arg3, 4, 1);
  }
  script_948B(arg0, arg2, 1);
  return;
}

static void script_1161(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  float local_F8;
  float local_F4;
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
    local_FC = script_8C20(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
    if ((local_FC == 1) | (local_FC == 2)) {
      local_FC = 1;
    } else {
      local_FC = 0;
    }
    *(int32_t *)&local_F8 = Script_ScenDataUtil(data_11104, 5, Script_ScenDataUtil(data_10B14, 1, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 5);
    *(int32_t *)&local_F4 = Script_ScenDataUtil(data_11104, 5, Script_ScenDataUtil(data_10B14, 1, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 6);
    if (local_FC == 0) {
      local_FC = script_8C20(arg0, arg1, 5, local_F8 / 100.0f, local_F4 / 100.0f, arg3, -1);
    } else {
      local_FC = script_8C20(arg0, arg1, 5, local_F8 / 100.0f, local_F4 / 100.0f, arg3, -1);
    }
    script_384A(arg0, arg1, arg2, arg3, local_FC, 0);
  }
  script_948B(arg0, arg2, 2);
  return;
}

static void script_1297(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
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
  if (arg0 == task_160) {
    if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
      local_FF7F = script_8C20(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
      *(int32_t *)&local_FF78 = Script_ScenDataUtil(data_11104, 5, Script_ScenDataUtil(data_10B14, 1, arg3 * 40 + task_98[arg2] * 4 + local_FF7F) * 9 + 5);
      *(int32_t *)&local_FF74 = Script_ScenDataUtil(data_11104, 5, Script_ScenDataUtil(data_10B14, 1, arg3 * 40 + task_98[arg2] * 4 + local_FF7F) * 9 + 6);
      local_FF7F = script_8C20(arg0, arg1, 5, local_FF78 / (float)100, local_FF74 / (float)100, arg3, -1);
      script_92E9(arg0, 0);
      *(int32_t *)&local_94 = Script_ScenDataUtil(data_11104, 5, Script_ScenDataUtil(data_10B14, 1, arg3 * 40 + task_98[arg2] * 4 + local_FF7F) * 9 + 1);
      *(int32_t *)&local_90 = Script_ScenDataUtil(data_11104, 5, Script_ScenDataUtil(data_10B14, 1, arg3 * 40 + task_98[arg2] * 4 + local_FF7F) * 9 + 2);
      local_8C = 0.0f;
      local_FC = Script_ScenDataUtil(data_11104, 4, Script_ScenDataUtil(data_10B14, 1, arg3 * 40 + task_98[arg2] * 4 + local_FF7F) * 9);
      local_FC = local_FC | ((Script_FloatUtils(ScriptMathOp_FloatToInt, local_94 - 3.0f) << 16) & 0x7FFF0000);
      local_9C.x = (&task_168)->x;
      (&local_9C)->y = task_168.y;
      (&local_9C)->z = task_168.z;
      Script_charaPosRotUtils(arg1, 12, &local_CC, &local_E4, 2);
      Script_charaPosRotUtils(arg1, 7, &local_C0, &local_D8, 2);
      Script_ryoControl(PCCTL_StartCameraFacingInterpolation, 0);
      if (arg0 == task_160) {
        Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
        Script_scenCharaSubmitMoveTargetRequest(arg0, &task_168, *(&task_174 + 1), local_FC, 0);
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
        Script_SetCharaMoveTarget(arg0, &task_168, *(&task_174 + 1), local_FC);
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
              local_FF6C = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_48[arg2], 2);
              local_FF6C = Script_ScenDataUtil(task_8, 4, Script_ScenDataUtil(task_C, 4, local_FF6C * 9 + 1) * 4 + 2);
              if (local_FF6C != -1) {
                if (arg0 == task_160) {
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
              local_FF6C = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_48[arg2], 2);
              local_FF6C = Script_ScenDataUtil(task_8, 4, Script_ScenDataUtil(task_C, 4, local_FF6C * 9 + 1) * 4 + 2);
              if (local_FF6C != -1) {
                if (arg0 == task_160) {
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
      script_92E9(arg0, 1);
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
        local_FF6C = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_48[arg2], 2);
        local_FF6C = Script_ScenDataUtil(task_8, 4, Script_ScenDataUtil(task_C, 4, local_FF6C * 9 + 1) * 4 + 3);
        if (local_FF6C != -1) {
          if (arg0 == task_160) {
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
    script_948B(arg0, arg2, 0);
  } else {
    script_087C(arg0, arg1, arg2, arg3);
  }
  return;
}

static void script_1B58(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  float local_F8;
  int32_t local_F4;
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
    local_FC = script_8C20(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
    *(int32_t *)&local_F8 = Script_ScenDataUtil(data_11104, 5, Script_ScenDataUtil(data_10B14, 1, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 5);
    local_F8 = 98.0f - local_F8;
    local_F4 = Script_ScenDataUtil(data_11104, 5, Script_ScenDataUtil(data_10B14, 1, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 6);
    if ((local_FC == 0) | (local_FC == 3)) {
      local_FC = script_8C20(arg0, arg1, 4, 0.34736f, 0.73184f, arg3, -1);
    } else {
      local_FC = script_8C20(arg0, arg1, 4, 0.4f, 0.712f, arg3, -1);
    }
    script_5F05(arg0, arg1, arg2, arg3, local_FC, 0);
  }
  script_948B(arg0, arg2, 0);
  return;
}

static int32_t script_1C76(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
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
    local_F4 = Script_ScenDataUtil(task_0, 4, task_48[arg2] * 13 + 6);
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
        local_FC = script_8C20(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
        local_FC = script_8C20(arg0, arg1, 7, 0.45f, 0.0f, arg3, -1);
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
              goto L001E32;
            }
          }
          L001E32:;
        } else {
          local_D4 = 0.45f;
        }
        Script_EVT_GetPosition(arg1, -1, &local_E4, 0);
        Script_mtwkGetSetRotation(arg1, &local_D8, 0);
        local_E4.x = (&local_E4)->x + local_D4 * Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_D8)->y & 0xFFFF);
        (&local_E4)->z = (&local_E4)->z - local_D4 * Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_D8)->y & 0xFFFF);
        *(&task_174 + 1) = Script_FloatUtils(ScriptMathOp_Atan2ReverseVector, (&local_E4)->x - (&task_168)->x, (&local_E4)->z - task_168.z);
        script_668F(arg0, arg1, arg2, arg3, local_FC, local_F0);
      }
    } else if ((local_BC == 3) | (local_BC == 4) | (local_BC == 5) | (local_BC == 6)) {
      *(int32_t *)&local_D0 = Script_ScenDataUtil(&data_11B24, 5, Script_ScenDataUtil(task_0, 4, task_48[arg2] * 13 + 6) * 5 + 3);
      *(int32_t *)&local_CC = Script_ScenDataUtil(&data_11B24, 5, Script_ScenDataUtil(task_0, 4, task_48[arg2] * 13 + 6) * 5 + 4);
      local_FC = script_8C20(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
      local_BC = arg3;
      if ((local_BC == 13) | (local_BC == 14) | (local_BC == 15) | (local_BC == 16) | (local_BC == 20) | (local_BC == 23) | (local_BC == 28) | (local_BC == 29)) {
        if ((Script_ScenDataUtil(task_0, 4, task_48[arg2] * 13 + 1) & 0x40) | (Script_ScenDataUtil(task_0, 4, task_48[arg2] * 13 + 1) & 0x80)) {
          if (Script_ScenDataUtil(task_0, 4, task_48[arg2] * 13 + 1) & 0x40) {
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
      local_FC = script_8C20(arg0, arg1, local_F8, local_D0 / (float)100, local_CC / (float)100, arg3, task_48[arg2]);
      script_6767(arg0, arg1, arg2, arg3, local_FC, 0);
    } else {
      if (local_BC == 7) {
        local_FC = script_8C20(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
        local_BC = arg3;
        if ((local_BC == 13) | (local_BC == 14) | (local_BC == 15) | (local_BC == 16) | (local_BC == 20) | (local_BC == 23) | (local_BC == 28) | (local_BC == 29)) {
          if ((Script_ScenDataUtil(task_0, 4, task_48[arg2] * 13 + 1) & 0x40) | (Script_ScenDataUtil(task_0, 4, task_48[arg2] * 13 + 1) & 0x80)) {
            if (Script_ScenDataUtil(task_0, 4, task_48[arg2] * 13 + 1) & 0x40) {
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
        if (Script_CycleManagerUtils(CYCLE_CMD_IS_DOOR_OR_MOBJ_TYPE2, Script_ScenDataUtil(task_0, 4, task_48[arg2] * 13 + 12)) == 1) {
          *(int32_t *)&local_D0 = Script_ScenDataUtil(&data_11B24, 5, Script_ScenDataUtil(task_0, 4, task_48[arg2] * 13 + 6) * 5 + 3);
          *(int32_t *)&local_CC = Script_ScenDataUtil(&data_11B24, 5, Script_ScenDataUtil(task_0, 4, task_48[arg2] * 13 + 6) * 5 + 4);
          local_FC = script_8C20(arg0, arg1, local_F8, local_D0 / (float)100, local_CC / (float)100, arg3, task_48[arg2]);
          Script_scenUtils(SCENARIO_CMD_INIT_SCNT);
          script_6BDE(arg0, arg1, arg2, arg3, local_FC, 0);
          Script_scenUtils(SCENARIO_CMD_UPDATE_TIMER);
        } else {
          *(int32_t *)&local_D0 = Script_ScenDataUtil(&data_11B24, 5, Script_ScenDataUtil(task_0, 4, task_48[arg2] * 13 + 6) * 5 + 3);
          *(int32_t *)&local_CC = Script_ScenDataUtil(&data_11B24, 5, Script_ScenDataUtil(task_0, 4, task_48[arg2] * 13 + 6) * 5 + 4);
          local_FC = script_8C20(arg0, arg1, local_F8, local_D0 / (float)100, local_CC / (float)100, arg3, task_48[arg2]);
          script_6767(arg0, arg1, arg2, arg3, local_FC, 0);
        }
        goto L00232D;
      }
    }
  }
  L00232D:;
  script_948B(arg0, arg2, 0);
  end:;
}

static void script_233B(int32_t arg0, int32_t arg1) {
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
    Script_SetCharaMoveTarget(arg0, &local_F4, (&local_E8)->y, script_0697(arg1));
  }
  return;
}

__attribute__((scn_string_bindings(0, data_12642)))
static void script_2411(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
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
  *(int32_t *)&local_98 = Script_ScenDataUtil(data_11104, 5, Script_ScenDataUtil(data_10B14, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 1);
  *(int32_t *)&local_94 = Script_ScenDataUtil(data_11104, 5, Script_ScenDataUtil(data_10B14, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 2);
  *(int32_t *)&local_90 = Script_ScenDataUtil(data_11104, 5, Script_ScenDataUtil(data_10B14, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 3);
  *(int32_t *)&local_8C = Script_ScenDataUtil(data_11104, 5, Script_ScenDataUtil(data_10B14, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 4);
  *(int32_t *)&local_88 = Script_ScenDataUtil(data_11104, 5, Script_ScenDataUtil(data_10B14, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 7);
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
      local_FF78 = Script_ScenDataUtil(data_11104, 4, Script_ScenDataUtil(data_10B14, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
      local_FF7C = 18;
      local_80 = 18;
      local_A4 = 1.0f;
    } else {
      if (arg0 == task_160) {
        local_FF78 = Script_ScenDataUtil(data_11104, 4, Script_ScenDataUtil(data_10B14, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9) | -0x80000000;
        local_FF7C = 12;
        local_80 = 12;
      } else {
        local_FF78 = Script_ScenDataUtil(data_11104, 4, Script_ScenDataUtil(data_10B14, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
        local_FF7C = 18;
        local_80 = 18;
      }
      local_A4 = -1.0f;
    }
    script_00E5(arg0, local_FF78, arg2, arg3, arg4);
    local_FF78 = local_FF78 | ((Script_FloatUtils(ScriptMathOp_FloatToInt, local_98 - 3.0f) << 16) & 0x7FFF0000);
    script_92E9(arg0, 0);
    local_C0.x = (&task_168)->x;
    (&local_C0)->y = task_168.y;
    (&local_C0)->z = task_168.z;
    Script_ryoControl(PCCTL_StartCameraFacingInterpolation, 0);
    Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
    Script_SetCharaMoveTarget(arg0, &task_168, *(&task_174 + 1), local_FF78);
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
    if (task_160 == arg0) {
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
          script_233B(arg0, task_98[arg2]);
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
            local_FF64 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_48[arg2], 2);
            local_FF64 = Script_ScenDataUtil(task_8, 4, Script_ScenDataUtil(task_C, 4, local_FF64 * 9 + 1) * 4 + 2);
            if (local_FF64 != -1) {
              if (arg0 == task_160) {
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
                goto L002999;
              }
            }
            L002999:;
          } else {
            if ((local_FF64 == 1) | (local_FF64 == 3)) {
              if (!(local_B4 > 0.0f)) {
                local_B4 = 0.0f;
              } else {
                if (!(local_B4 < 0.75f)) {
                  local_B4 = 0.75f;
                  goto L0029D7;
                }
              }
              L0029D7:;
              goto L0029DA;
            }
          }
        }
      }
      L0029DA:;
      if (task_160 == arg0) {
        if (!(local_B8 <= local_94) & !(local_B8 >= local_90)) {
          local_86 = 0;
        }
      } else {
        if (!(local_B8 <= local_94) & !(local_B8 > local_88)) {
          *(int32_t *)&local_9C = Script_ScenDataUtil(data_11104, 5, Script_ScenDataUtil(data_10B14, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 8);
          local_B4 = local_B4 + local_9C * local_A4;
        }
      }
      if (!(local_B8 < local_90) & !(local_B8 >= local_90 + 1.5f)) {
        if (local_84 == 1) {
          int32_t local_FF64;
          local_FF64 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_48[arg2], 2);
          local_FF64 = Script_ScenDataUtil(task_8, 4, Script_ScenDataUtil(task_C, 4, local_FF64 * 9 + 1) * 4 + 3);
          if (local_FF64 != -1) {
            if (arg0 == task_160) {
              Script_executeAicaCommand(local_FF64, 0, 0);
            } else {
              Script_Execute3DSoundCommand_Wrapper(local_FF64, 4, &local_C0, 5.0f, 20.0f, 0);
            }
          }
          local_84 = 2;
        }
        if (task_160 != arg0) {
          if (local_FF74 == 0) {
            local_AC = local_B4 / (local_8C - local_90);
            local_FF74 = 1;
          }
        }
      }
      if (!(local_B8 < local_90) & !(local_B8 > local_8C)) {
        Script_EVT_GetPosition(arg0, local_FF7C, &local_F0, ENTITY_XFORM_MOTION_SPACE);
        if (task_160 == arg0) {
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
                goto L002BA6;
              }
            }
            L002BA6:;
          } else {
            if ((local_FF64 == 1) | (local_FF64 == 3)) {
              if (!(local_B4 < 0.75f)) {
                local_B4 = 0.75f;
              } else {
                if (!(local_B4 > 0.0f)) {
                  local_B4 = 0.0f;
                  goto L002BE4;
                }
              }
              L002BE4:;
              goto L002BE7;
            }
          }
          L002BE7:;
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
              goto L002C40;
            }
          }
        }
      }
      L002C40:;
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
      if (arg0 == task_160) {
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
  script_92E9(arg0, 1);
  {
    int32_t local_FF78;
    int32_t unused_FF68[4];
    int32_t local_FF64;
    local_FF78 = task_48[arg2];
    local_FF64 = arg3;
    if ((local_FF64 == 9) | (local_FF64 == 10) | (local_FF64 == 15)) {
      if (arg5) {
        Script_charaPosRotUtils(arg1, 12, &local_D8, 0, 0);
        Script_charaPosRotUtils(arg1, 7, &local_CC, 0, 0);
      } else {
        Script_EVT_SetPosition(arg1, POS_XYZ, &local_D8);
      }
      if (task_160 == arg0) {
        local_80 = (Script_ScenDataUtil(task_0, 4, local_FF78 * 13 + 2) >> 16) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_80, 1);
        local_80 = Script_ScenDataUtil(task_0, 4, local_FF78 * 13 + 2) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_80, 1);
      }
    } else if ((local_FF64 == 18) | (local_FF64 == 19) | (local_FF64 == 21) | (local_FF64 == 22)) {
      if (task_160 == arg0) {
        local_80 = (Script_ScenDataUtil(task_0, 4, local_FF78 * 13 + 2) >> 16) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_80, 1);
        {
          int32_t local_FF60;
          local_FF60 = Script_ScenDataUtil(task_0, 4, local_FF78 * 13 + 1);
          local_FF60 = local_FF60 | 8;
          Script_ScenDataUtil(task_0, 14, local_FF78 * 13 + 1, local_FF60);
        }
        {
          int32_t local_FF60;
          local_FF60 = Script_ScenDataUtil(task_0, 4, local_FF78 * 13 + 1);
          local_FF60 = local_FF60 | 0x10;
          Script_ScenDataUtil(task_0, 14, local_FF78 * 13 + 1, local_FF60);
        }
        {
          int32_t local_FF60;
          local_FF60 = Script_ScenDataUtil(task_0, 4, local_FF78 * 13 + 1);
          local_FF60 = local_FF60 | 2;
          Script_ScenDataUtil(task_0, 14, local_FF78 * 13 + 1, local_FF60);
        }
      } else {
        if (arg5) {
          Script_charaPosRotUtils(arg1, 12, &local_D8, 0, 0);
          Script_charaPosRotUtils(arg1, 7, &local_CC, 0, 0);
        } else {
          Script_EVT_SetPosition(arg1, POS_XYZ, &local_D8);
        }
        local_80 = (Script_ScenDataUtil(task_0, 4, local_FF78 * 13 + 2) >> 16) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_80, 1);
        local_80 = Script_ScenDataUtil(task_0, 4, local_FF78 * 13 + 2) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_80, 1);
      }
    } else {
      if ((local_FF64 == 20) | (local_FF64 == 23)) {
        if (task_160 == arg0) {
          if ((arg4 == 0) | (arg4 == 1)) {
            {
              int32_t local_FF60;
              local_FF60 = Script_ScenDataUtil(task_0, 4, local_FF78 * 13 + 1);
              local_FF60 = local_FF60 | 0x10;
              Script_ScenDataUtil(task_0, 14, local_FF78 * 13 + 1, local_FF60);
            }
            {
              int32_t local_FF60;
              local_FF60 = Script_ScenDataUtil(task_0, 4, local_FF78 * 13 + 1);
              local_FF60 = local_FF60 | 4;
              Script_ScenDataUtil(task_0, 14, local_FF78 * 13 + 1, local_FF60);
            }
          } else {
            {
              int32_t local_FF60;
              local_FF60 = Script_ScenDataUtil(task_0, 4, local_FF78 * 13 + 1);
              local_FF60 = local_FF60 | 8;
              Script_ScenDataUtil(task_0, 14, local_FF78 * 13 + 1, local_FF60);
            }
            {
              int32_t local_FF60;
              local_FF60 = Script_ScenDataUtil(task_0, 4, local_FF78 * 13 + 1);
              local_FF60 = local_FF60 | 4;
              Script_ScenDataUtil(task_0, 14, local_FF78 * 13 + 1, local_FF60);
            }
          }
        } else {
          if (arg5) {
            Script_charaPosRotUtils(arg1, 12, &local_D8, 0, 0);
            Script_charaPosRotUtils(arg1, 7, &local_CC, 0, 0);
          } else {
            Script_EVT_SetPosition(arg1, POS_XYZ, &local_D8);
          }
          local_80 = (Script_ScenDataUtil(task_0, 4, local_FF78 * 13 + 2) >> 16) & 0xFFFF;
          Script_scenToggleCharaFlag(0, local_80, 1);
          local_80 = Script_ScenDataUtil(task_0, 4, local_FF78 * 13 + 2) & 0xFFFF;
          Script_scenToggleCharaFlag(0, local_80, 1);
        }
        goto L003323;
      }
    }
  }
  L003323:;
  return;
}

static void script_3324(int32_t arg0, int32_t arg1, int32_t arg2) {
  int32_t local_FC;
  if (task_160 == arg0) {
    local_FC = task_48[arg1];
    {
      int32_t local_F8;
      local_F8 = Script_ScenDataUtil(task_0, 4, local_FC * 13 + 2) & 0xFFFF;
      Script_scenToggleCharaFlag(0, local_F8, arg2);
    }
    {
      int32_t local_F8;
      local_F8 = (Script_ScenDataUtil(task_0, 4, local_FC * 13 + 2) >> 16) & 0xFFFF;
      Script_scenToggleCharaFlag(0, local_F8, arg2);
    }
  }
  return;
}

static void script_3390(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
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
  local_AC.x = (&task_168)->x;
  (&local_AC)->y = task_168.y;
  (&local_AC)->z = task_168.z;
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
    local_80 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_48[arg2], 2);
    local_80 = Script_ScenDataUtil(task_8, 4, Script_ScenDataUtil(task_C, 4, local_80 * 9 + 1) * 4 + 2);
    if (local_80 != -1) {
      if (arg0 == task_160) {
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
        task_70[arg2] = arg0;
        local_98 = -local_98;
        local_84 = 0;
      }
      local_8C = 0;
    } else {
      if ((local_93 == 1) & (local_8C == 0)) {
        int32_t local_80;
        local_80 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_48[arg2], 2);
        local_80 = Script_ScenDataUtil(task_8, 4, Script_ScenDataUtil(task_C, 4, local_80 * 9 + 1) * 4 + 3);
        if (local_80 != -1) {
          if (arg0 == task_160) {
            Script_executeAicaCommand(local_80, 0, 0);
          } else {
            Script_Execute3DSoundCommand_Wrapper(local_80, 4, &local_AC, 5.0f, 20.0f, 0);
          }
        }
        local_93 = 0;
        local_8C = 1;
        local_98 = -local_98;
        local_84 = 0;
        task_70[arg2] = 0;
      }
    }
    local_88 = task_48[arg2];
    local_88 = Script_ScenDataUtil(task_0, 4, local_88 * 13 + 2) & 0xFFFF;
    local_88 = local_88 + ((local_9C - 0x4000) >> 12);
    if (local_84 != local_88) {
      Script_scenToggleCharaFlag(0, local_88, local_8C);
      local_84 = local_88;
    }
    local_9C = local_9C - local_98;
    if (local_9C >= 0xC000) {
      local_9C = 0xC000;
      local_88 = task_48[arg2];
      local_88 = Script_ScenDataUtil(task_0, 4, local_88 * 13 + 2) & 0xFFFF;
      local_88 = local_88 + 8;
      Script_scenToggleCharaFlag(0, local_88, local_8C);
      break;
    }
    if (local_9C <= 0x4000) {
      local_9C = 0x4000;
      if (local_93 == 0) {
        task_70[arg2] = 0;
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
          goto L0037FA;
        }
      }
    }
    L0037FA:;
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

__attribute__((scn_string_bindings(0, data_1265E)))
static void script_384A(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
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
          local_FC = Script_ScenDataUtil(data_11104, 4, Script_ScenDataUtil(data_10B14, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
          break;
        case 0:
          local_B7 = 0;
          local_F8 = 12;
          local_F0 = 0x4000;
          local_B8 = 1.0f;
          local_FC = Script_ScenDataUtil(data_11104, 4, Script_ScenDataUtil(data_10B14, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9) | -0x80000000;
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
          local_FC = Script_ScenDataUtil(data_11104, 4, Script_ScenDataUtil(data_10B14, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9) | -0x80000000;
          break;
        case 0:
          local_B7 = 1;
          local_F8 = 18;
          local_F0 = 0xC000;
          local_B8 = 1.0f;
          local_FC = Script_ScenDataUtil(data_11104, 4, Script_ScenDataUtil(data_10B14, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
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
          local_FC = Script_ScenDataUtil(data_11104, 4, Script_ScenDataUtil(data_10B14, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9) | -0x80000000;
          break;
        case 0:
          local_B7 = 0;
          local_F8 = 18;
          local_F0 = 0xC000;
          local_B8 = -1.0f;
          local_FC = Script_ScenDataUtil(data_11104, 4, Script_ScenDataUtil(data_10B14, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
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
          local_FC = Script_ScenDataUtil(data_11104, 4, Script_ScenDataUtil(data_10B14, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
          break;
        case 0:
          local_B7 = 1;
          local_F8 = 12;
          local_F0 = 0x4000;
          local_B8 = -1.0f;
          local_FC = Script_ScenDataUtil(data_11104, 4, Script_ScenDataUtil(data_10B14, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9) | -0x80000000;
          break;
      }
      break;
  }
  *(int32_t *)&local_C4 = Script_ScenDataUtil(data_11104, 5, Script_ScenDataUtil(data_10B14, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 1);
  *(int32_t *)&local_C0 = Script_ScenDataUtil(data_11104, 5, Script_ScenDataUtil(data_10B14, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 2);
  local_FC = local_FC | ((Script_FloatUtils(ScriptMathOp_FloatToInt, local_C4 - 3.0f) << 16) & 0x7FFF0000);
  local_BC = 0.0f;
  script_92E9(arg0, 0);
  local_CC.x = (&task_168)->x;
  (&local_CC)->y = task_168.y;
  (&local_CC)->z = task_168.z;
  Script_ryoControl(PCCTL_StartCameraFacingInterpolation, 0);
  Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
  Script_SetCharaMoveTarget(arg0, &task_168, *(&task_174 + 1), local_FC);
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
        local_A0 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_48[arg2], 2);
        local_A0 = Script_ScenDataUtil(task_8, 4, Script_ScenDataUtil(task_C, 4, local_A0 * 9 + 1) * 4 + 2);
        if (local_A0 != -1) {
          if (arg0 == task_160) {
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
  script_92E9(arg0, 1);
  local_FC = task_48[arg2];
  {
    int32_t local_A0;
    local_A0 = Script_ScenDataUtil(task_0, 4, local_FC * 13 + 1);
    local_A0 = local_A0 | 8;
    Script_ScenDataUtil(task_0, 14, local_FC * 13 + 1, local_A0);
  }
  {
    int32_t local_A0;
    local_A0 = Script_ScenDataUtil(task_0, 4, local_FC * 13 + 1);
    local_A0 = local_A0 | 0x10;
    Script_ScenDataUtil(task_0, 14, local_FC * 13 + 1, local_A0);
  }
  {
    int32_t local_A0;
    local_A0 = Script_ScenDataUtil(task_0, 4, local_FC * 13 + 1);
    local_A0 = local_A0 | 4;
    Script_ScenDataUtil(task_0, 14, local_FC * 13 + 1, local_A0);
  }
  return;
}

__attribute__((scn_string_bindings(0, data_1267A)))
static void script_3F04(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
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
      if (arg0 == task_160) {
        local_FF5C = arg4;
        switch (local_FF5C) {
          case 1:
            local_CF = 1;
            local_A4 = Script_ScenDataUtil(data_11104, 4, Script_ScenDataUtil(data_10B14, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
            local_C0 = 1.0f;
            local_A0 = 18;
            local_9C = 18;
            local_90 = 15473;
            local_8C = 17294;
            break;
          case 0:
            local_CF = 0;
            local_A4 = Script_ScenDataUtil(data_11104, 4, Script_ScenDataUtil(data_10B14, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9) | -0x80000000;
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
            local_A4 = Script_ScenDataUtil(data_11104, 4, Script_ScenDataUtil(data_10B14, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
            local_C0 = 1.0f;
            local_A0 = 18;
            local_9C = 18;
            local_90 = 13653;
            break;
          case 0:
            local_CF = 0;
            local_A4 = Script_ScenDataUtil(data_11104, 4, Script_ScenDataUtil(data_10B14, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
            local_C0 = 1.0f;
            local_A0 = 18;
            local_9C = 18;
            local_90 = 19842;
            break;
        }
      }
    } else if ((local_FF5C == 1) | (local_FF5C == 7) | (local_FF5C == 30) | (local_FF5C == 36) | (local_FF5C == 37)) {
      if (arg0 == task_160) {
        local_FF5C = arg4;
        switch (local_FF5C) {
          case 1:
            local_CF = 0;
            local_A4 = Script_ScenDataUtil(data_11104, 4, Script_ScenDataUtil(data_10B14, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9) | -0x80000000;
            local_C0 = 1.0f;
            local_A0 = 12;
            local_9C = 18;
            local_90 = 50062;
            local_8C = 48241;
            break;
          case 0:
            local_CF = 1;
            local_A4 = Script_ScenDataUtil(data_11104, 4, Script_ScenDataUtil(data_10B14, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
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
            local_A4 = Script_ScenDataUtil(data_11104, 4, Script_ScenDataUtil(data_10B14, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
            local_C0 = 1.0f;
            local_A0 = 18;
            local_9C = 18;
            if (task_98[arg2] == 4) {
              local_90 = 50062;
            } else {
              local_90 = 51882;
            }
            break;
          case 0:
            local_CF = 1;
            local_A4 = Script_ScenDataUtil(data_11104, 4, Script_ScenDataUtil(data_10B14, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
            local_C0 = 1.0f;
            local_A0 = 18;
            local_9C = 18;
            if (task_98[arg2] == 4) {
              local_90 = 46421;
            } else {
              local_90 = 45511;
            }
            break;
        }
      }
    } else if ((local_FF5C == 3) | (local_FF5C == 5) | (local_FF5C == 32) | (local_FF5C == 34)) {
      if (arg0 == task_160) {
        local_FF5C = arg4;
        switch (local_FF5C) {
          case 1:
            local_CF = 1;
            local_A4 = Script_ScenDataUtil(data_11104, 4, Script_ScenDataUtil(data_10B14, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9) | -0x80000000;
            local_C0 = -1.0f;
            local_A0 = 12;
            local_9C = 12;
            local_90 = 50062;
            local_8C = 48241;
            break;
          case 0:
            local_CF = 0;
            local_A4 = Script_ScenDataUtil(data_11104, 4, Script_ScenDataUtil(data_10B14, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
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
            local_A4 = Script_ScenDataUtil(data_11104, 4, Script_ScenDataUtil(data_10B14, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
            local_C0 = -1.0f;
            local_A0 = 18;
            local_9C = 18;
            local_90 = 51882;
            break;
          case 0:
            local_CF = 0;
            local_A4 = Script_ScenDataUtil(data_11104, 4, Script_ScenDataUtil(data_10B14, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
            local_C0 = -1.0f;
            local_A0 = 18;
            local_9C = 18;
            local_90 = 45511;
            break;
        }
      }
    } else if ((local_FF5C == 2) | (local_FF5C == 8) | (local_FF5C == 31)) {
      if (arg0 == task_160) {
        local_FF5C = arg4;
        switch (local_FF5C) {
          case 1:
            local_CF = 0;
            local_A4 = Script_ScenDataUtil(data_11104, 4, Script_ScenDataUtil(data_10B14, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
            local_C0 = -1.0f;
            local_A0 = 18;
            local_9C = 12;
            local_90 = 15473;
            local_8C = 17294;
            break;
          case 0:
            local_CF = 1;
            local_A4 = Script_ScenDataUtil(data_11104, 4, Script_ScenDataUtil(data_10B14, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9) | -0x80000000;
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
            local_A4 = Script_ScenDataUtil(data_11104, 4, Script_ScenDataUtil(data_10B14, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
            local_C0 = -1.0f;
            local_A0 = 18;
            local_9C = 18;
            local_90 = 13653;
            break;
          case 0:
            local_CF = 1;
            local_A4 = Script_ScenDataUtil(data_11104, 4, Script_ScenDataUtil(data_10B14, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
            local_C0 = -1.0f;
            local_A0 = 18;
            local_9C = 18;
            local_90 = 19842;
            break;
        }
      }
    } else {
      if ((local_FF5C == 13) | (local_FF5C == 14) | (local_FF5C == 28)) {
        if (arg0 == task_160) {
          local_FF5C = arg4;
          switch (local_FF5C) {
            case 0:
              local_CF = 0;
              local_A4 = Script_ScenDataUtil(data_11104, 4, Script_ScenDataUtil(data_10B14, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
              local_C0 = -1.0f;
              local_A0 = 18;
              local_9C = 12;
              local_90 = 48241;
              local_8C = 50062;
              break;
            case 1:
              local_CF = 1;
              local_A4 = Script_ScenDataUtil(data_11104, 4, Script_ScenDataUtil(data_10B14, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9) | -0x80000000;
              local_C0 = -1.0f;
              local_A0 = 12;
              local_9C = 12;
              local_90 = 50062;
              local_8C = 48241;
              break;
            case 2:
              local_CF = 1;
              local_A4 = Script_ScenDataUtil(data_11104, 4, Script_ScenDataUtil(data_10B14, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
              local_C0 = 1.0f;
              local_A0 = 18;
              local_9C = 18;
              local_90 = 15473;
              local_8C = 17294;
              break;
            case 3:
              local_CF = 0;
              local_A4 = Script_ScenDataUtil(data_11104, 4, Script_ScenDataUtil(data_10B14, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9) | -0x80000000;
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
              local_A4 = Script_ScenDataUtil(data_11104, 4, Script_ScenDataUtil(data_10B14, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
              local_C0 = -1.0f;
              local_A0 = 18;
              local_9C = 18;
              local_90 = 45511;
              break;
            case 1:
              local_CF = 1;
              local_A4 = Script_ScenDataUtil(data_11104, 4, Script_ScenDataUtil(data_10B14, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
              local_C0 = -1.0f;
              local_A0 = 18;
              local_9C = 18;
              local_90 = 51882;
              break;
            case 2:
              local_CF = 1;
              local_A4 = Script_ScenDataUtil(data_11104, 4, Script_ScenDataUtil(data_10B14, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
              local_C0 = 1.0f;
              local_A0 = 18;
              local_9C = 18;
              if (task_98[arg2] == 4) {
                local_90 = 14199;
              } else {
                local_90 = 13653;
              }
              break;
            case 3:
              local_CF = 0;
              local_A4 = Script_ScenDataUtil(data_11104, 4, Script_ScenDataUtil(data_10B14, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
              local_C0 = 1.0f;
              local_A0 = 18;
              local_9C = 18;
              if (task_98[arg2] == 4) {
                local_90 = 18022;
              } else {
                local_90 = 19842;
              }
              break;
          }
        }
        local_CE = 1;
        goto L004AF7;
      }
    }
  }
  L004AF7:;
  (void)local_90;
  *(int32_t *)&local_B8 = Script_ScenDataUtil(data_11104, 5, Script_ScenDataUtil(data_10B14, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 1);
  *(int32_t *)&local_B4 = Script_ScenDataUtil(data_11104, 5, Script_ScenDataUtil(data_10B14, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 2);
  *(int32_t *)&local_B0 = Script_ScenDataUtil(data_11104, 5, Script_ScenDataUtil(data_10B14, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 3);
  *(int32_t *)&local_AC = Script_ScenDataUtil(data_11104, 5, Script_ScenDataUtil(data_10B14, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 4);
  *(int32_t *)&local_A8 = Script_ScenDataUtil(data_11104, 5, Script_ScenDataUtil(data_10B14, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 7);
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
  script_00E5(arg0, local_A4, arg2, arg3, arg4);
  local_A4 = local_A4 | ((Script_FloatUtils(ScriptMathOp_FloatToInt, local_B8 - 3.0f) << 16) & 0x7FFF0000);
  script_92E9(arg0, 0);
  local_D0.x = (&task_168)->x;
  (&local_D0)->y = task_168.y;
  (&local_D0)->z = task_168.z;
  Script_ryoControl(PCCTL_StartCameraFacingInterpolation, 0);
  Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
  Script_SetCharaMoveTarget(arg0, &task_168, *(&task_174 + 1), local_A4);
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
    if (arg0 == task_160) {
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
      if (arg0 == task_160) {
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
              script_233B(arg0, task_98[arg2]);
            }
          }
        }
      }
      if (!(local_C4 > local_B4)) {
        if (!(local_C4 < local_B8) & !(local_C4 > local_B4)) {
          if (arg0 == task_160) {
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
          local_FF50 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_48[arg2], 2);
          local_FF50 = Script_ScenDataUtil(task_8, 4, Script_ScenDataUtil(task_C, 4, local_FF50 * 9 + 1) * 4 + 2);
          if (local_FF50 != -1) {
            if (arg0 == task_160) {
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
        local_FF60 = script_006D((&local_FF64)->y & 0xFFFF, local_88 & 0xFFFF);
        if (local_FF60 > 910) {
          local_88 = (&local_FF64)->y + 910;
        } else {
          if (local_FF60 < -910) {
            local_88 = (&local_FF64)->y - 910;
            goto L0050FC;
          }
        }
        L0050FC:;
        if (!(local_C4 >= local_B4)) {
          if (!(local_C0 <= 0.0f)) {
            if (script_006D(local_FF70, local_88) > 0) {
              local_88 = local_FF70;
            }
          } else {
            if (script_006D(local_FF70, local_88) < 0) {
              local_88 = local_FF70;
            }
          }
        }
      }
      if (arg0 != task_160) {
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
          local_FF74 = (float)script_0030(script_006D(local_FF70, (&local_FF48)->y) * 360 / 0x10000);
        }
        if (!(local_C4 <= local_B4) & !(local_C4 > local_A8)) {
          *(int32_t *)&local_BC = Script_ScenDataUtil(data_11104, 5, Script_ScenDataUtil(data_10B14, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 8);
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
          if (arg0 != task_160) {
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
        local_FF50 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_48[arg2], 2);
        local_FF50 = Script_ScenDataUtil(task_8, 4, Script_ScenDataUtil(task_C, 4, local_FF50 * 9 + 1) * 4 + 3);
        if (local_FF50 != -1) {
          if (arg0 == task_160) {
            Script_executeAicaCommand(local_FF50, 0, 0);
          } else {
            Script_Execute3DSoundCommand_Wrapper(local_FF50, 4, &local_D0, 5.0f, 20.0f, 0);
          }
        }
        local_A4 = task_48[arg2];
        if (arg0 == task_160) {
          if (task_160 == arg0) {
            local_A0 = (Script_ScenDataUtil(task_0, 4, local_A4 * 13 + 2) >> 16) & 0xFFFF;
            Script_scenToggleCharaFlag(0, local_A0, 1);
            local_A0 = Script_ScenDataUtil(task_0, 4, local_A4 * 13 + 2) & 0xFFFF;
            Script_scenToggleCharaFlag(0, local_A0, 1);
          }
        }
      }
      if (!(local_AC <= 0.0f)) {
        if (!(local_C4 <= local_AC)) {
          if (arg0 == task_160) {
            local_FF5C = arg3;
            if ((local_FF5C == 30) | (local_FF5C == 31) | (local_FF5C == 32) | (local_FF5C == 33) | (local_FF5C == 34) | (local_FF5C == 35)) {
              local_FF78 = local_FF78 - 0.06f;
              if (!(local_FF78 >= 0.3f)) {
                local_FF78 = 0.3f;
              }
              DSWK_SetActorOpacityAndDrawCallback(arg1, local_FF78);
              goto L0055A8;
            }
          }
        }
      }
      L0055A8:;
      if (!(local_C4 < local_B0) & !(local_C4 > local_AC)) {
        if (arg0 == task_160) {
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
      if (arg0 != task_160) {
        if (!(local_C4 < local_AC)) {
          break;
        }
        local_C4 = local_C4 + 1.0f;
      }
      SCNF_WaitFrames_0000(1);
    }
  }
  script_92E9(arg0, 1);
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
  if (arg0 == task_160) {
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
      goto L0057E3;
    }
  }
  L0057E3:;
  return;
}

__attribute__((scn_string_bindings(0, data_12696)))
static void script_57E4(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
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
    script_92E9(arg0, 0);
    local_C4.x = (&task_168)->x;
    (&local_C4)->y = task_168.y;
    (&local_C4)->z = task_168.z;
    Script_ryoControl(PCCTL_StartCameraFacingInterpolation, 0);
    Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
    Script_SetCharaMoveTarget(arg0, &task_168, *(&task_174 + 1), local_94);
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
          local_84 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_48[arg2], 2);
          local_84 = Script_ScenDataUtil(task_8, 4, Script_ScenDataUtil(task_C, 4, local_84 * 9 + 1) * 4 + 3);
          if (local_84 != -1) {
            if (arg0 == task_160) {
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
                goto L005B08;
              }
            }
            L005B08:;
          } else {
            if ((local_84 == 1) | (local_84 == 3)) {
              if (!(local_B8 > -0.75f)) {
                local_B8 = -0.75f;
              } else {
                if (!(local_B8 < 0.0f)) {
                  local_B8 = 0.0f;
                  goto L005B44;
                }
              }
              L005B44:;
              goto L005B47;
            }
          }
        }
      }
      L005B47:;
      if (!(local_BC <= local_A4)) {
        int32_t local_84;
        local_84 = arg4;
        if ((local_84 == 0) | (local_84 == 2)) {
          local_B8 = 0.75f;
        } else {
          if ((local_84 == 1) | (local_84 == 3)) {
            local_B8 = -0.75f;
            goto L005B8C;
          }
        }
      }
      L005B8C:;
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
  script_92E9(arg0, 1);
  {
    int32_t local_94;
    local_94 = task_48[arg2];
    local_9C = Script_ScenDataUtil(task_0, 4, local_94 * 13 + 2) & 0xFFFF;
    Script_scenToggleCharaFlag(0, local_9C, 1);
    local_9C = (Script_ScenDataUtil(task_0, 4, local_94 * 13 + 2) >> 16) & 0xFFFF;
    Script_scenToggleCharaFlag(0, local_9C, 1);
    {
      int32_t unused_88[3];
      int32_t local_84;
      local_84 = Script_ScenDataUtil(task_0, 4, local_94 * 13 + 1);
      local_84 = local_84 & -9;
      Script_ScenDataUtil(task_0, 14, local_94 * 13 + 1, local_84);
    }
    {
      int32_t unused_88[3];
      int32_t local_84;
      local_84 = Script_ScenDataUtil(task_0, 4, local_94 * 13 + 1);
      local_84 = local_84 & -17;
      Script_ScenDataUtil(task_0, 14, local_94 * 13 + 1, local_84);
    }
    {
      int32_t unused_88[3];
      int32_t local_84;
      local_84 = Script_ScenDataUtil(task_0, 4, local_94 * 13 + 1);
      local_84 = local_84 & -5;
      Script_ScenDataUtil(task_0, 14, local_94 * 13 + 1, local_84);
    }
  }
  return;
}

__attribute__((scn_string_bindings(0, data_126B2)))
static void script_5F05(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
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
      local_8C = Script_ScenDataUtil(data_11104, 4, Script_ScenDataUtil(data_10B14, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
      local_A0 = -1.0f;
      local_88 = 18;
      local_84 = 18;
      local_FF78 = 46421;
      local_FF74 = 48787;
      break;
    case 1:
      local_AF = 1;
      local_8C = Script_ScenDataUtil(data_11104, 4, Script_ScenDataUtil(data_10B14, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9) | -0x80000000;
      local_A0 = -1.0f;
      local_88 = 12;
      local_84 = 12;
      local_FF78 = 50426;
      local_FF74 = 48969;
      break;
    case 2:
      local_AF = 1;
      local_8C = Script_ScenDataUtil(data_11104, 4, Script_ScenDataUtil(data_10B14, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
      local_A0 = 1.0f;
      local_88 = 18;
      local_84 = 18;
      local_FF78 = 15109;
      local_FF74 = 16566;
      break;
    case 3:
      local_AF = 0;
      local_8C = Script_ScenDataUtil(data_11104, 4, Script_ScenDataUtil(data_10B14, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9) | -0x80000000;
      local_A0 = 1.0f;
      local_88 = 12;
      local_84 = 12;
      local_FF78 = 19114;
      local_FF74 = 16748;
      break;
  }
  (void)local_FF78;
  *(int32_t *)&local_9C = Script_ScenDataUtil(data_11104, 5, Script_ScenDataUtil(data_10B14, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 1);
  *(int32_t *)&local_98 = Script_ScenDataUtil(data_11104, 5, Script_ScenDataUtil(data_10B14, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 2);
  *(int32_t *)&local_94 = Script_ScenDataUtil(data_11104, 5, Script_ScenDataUtil(data_10B14, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 3);
  *(int32_t *)&local_90 = Script_ScenDataUtil(data_11104, 5, Script_ScenDataUtil(data_10B14, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 4);
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
  script_92E9(arg0, 0);
  local_B0.x = (&task_168)->x;
  (&local_B0)->y = task_168.y;
  (&local_B0)->z = task_168.z;
  Script_ryoControl(PCCTL_StartCameraFacingInterpolation, 0);
  Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
  Script_SetCharaMoveTarget(arg0, &task_168, *(&task_174 + 1), local_8C);
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
        local_FF64 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_48[arg2], 2);
        local_FF64 = Script_ScenDataUtil(task_8, 4, Script_ScenDataUtil(task_C, 4, local_FF64 * 9 + 1) * 4 + 2);
        if (local_FF64 != -1) {
          if (arg0 == task_160) {
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
        local_FF64 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_48[arg2], 2);
        local_FF64 = Script_ScenDataUtil(task_8, 4, Script_ScenDataUtil(task_C, 4, local_FF64 * 9 + 1) * 4 + 3);
        if (local_FF64 != -1) {
          if (arg0 == task_160) {
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
  script_92E9(arg0, 1);
  local_F4.x = 0;
  (&local_F4)->z = 0;
  (&local_F4)->y = local_80;
  Script_charaPosRotUtils(arg1, 12, 0, &local_F4, 0);
  (&local_F4)->y = local_FF7C;
  Script_charaPosRotUtils(arg1, 7, 0, &local_F4, 0);
  return;
}

static void script_668F(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
  int32_t local_FC;
  int32_t local_F8;
  Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
  Script_SetCharaMoveTarget(arg0, &task_168, *(&task_174 + 1), MN_AKI_AKI_BASE);
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
        local_FC = Script_ScenDataUtil(task_0, 4, task_48[arg2] * 13 + 7);
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

__attribute__((scn_string_bindings(0, data_126CE)))
static void script_6767(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
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
  local_D4 = Script_ScenDataUtil(task_0, 4, task_48[arg2] * 13 + 6);
  local_CC = arg3;
  if ((local_CC == 1) | (local_CC == 3) | (local_CC == 5) | (local_CC == 7) | (local_CC == 9) | (local_CC == 18) | (local_CC == 21) | (local_CC == 25) | (local_CC == 27) | (local_CC == 36) | (local_CC == 37)) {
    local_CC = arg4;
    switch (local_CC) {
      case 1:
        local_E0 = Script_ScenDataUtil(&data_11B24, 4, local_D4 * 5) | -0x80000000;
        break;
      case 0:
        local_E0 = Script_ScenDataUtil(&data_11B24, 4, local_D4 * 5);
        break;
    }
  } else if ((local_CC == 2) | (local_CC == 4) | (local_CC == 6) | (local_CC == 8) | (local_CC == 10) | (local_CC == 19) | (local_CC == 22) | (local_CC == 24) | (local_CC == 26) | (local_CC == 28)) {
    local_CC = arg4;
    switch (local_CC) {
      case 1:
        local_E0 = Script_ScenDataUtil(&data_11B24, 4, local_D4 * 5);
        break;
      case 0:
        local_E0 = Script_ScenDataUtil(&data_11B24, 4, local_D4 * 5) | -0x80000000;
        break;
    }
  } else {
    if ((local_CC == 13) | (local_CC == 14) | (local_CC == 15) | (local_CC == 16) | (local_CC == 20) | (local_CC == 23) | (local_CC == 29)) {
      local_CC = arg4;
      switch (local_CC) {
        case 0:
          local_E0 = Script_ScenDataUtil(&data_11B24, 4, local_D4 * 5);
          break;
        case 1:
          local_E0 = Script_ScenDataUtil(&data_11B24, 4, local_D4 * 5) | -0x80000000;
          break;
        case 2:
          local_E0 = Script_ScenDataUtil(&data_11B24, 4, local_D4 * 5);
          break;
        case 3:
          local_E0 = Script_ScenDataUtil(&data_11B24, 4, local_D4 * 5) | -0x80000000;
          break;
      }
      goto L006988;
    }
  }
  L006988:;
  *(int32_t *)&local_EC = Script_ScenDataUtil(&data_11B24, 5, local_D4 * 5 + 1);
  *(int32_t *)&local_E8 = Script_ScenDataUtil(&data_11B24, 5, local_D4 * 5 + 2);
  local_E0 = local_E0 | ((Script_FloatUtils(ScriptMathOp_FloatToInt, local_EC - 3.0f) << 16) & 0x7FFF0000);
  local_F4.x = (&task_168)->x;
  (&local_F4)->y = task_168.y;
  (&local_F4)->z = task_168.z;
  Script_ryoControl(PCCTL_StartCameraFacingInterpolation, 0);
  Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
  Script_SetCharaMoveTarget(arg0, &task_168, *(&task_174 + 1), local_E0);
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
      local_C8 = Script_ScenDataUtil(task_0, 4, task_48[arg2] * 13 + 9);
      if (local_C8 != -1) {
        if (arg0 == task_160) {
          Script_executeAicaCommand(local_C8, 0, 0);
        } else {
          Script_Execute3DSoundCommand_Wrapper(local_C8, 4, &local_F4, 5.0f, 20.0f, 0);
        }
      }
    }
    if (!(local_F0 < local_E8) & !(local_F0 >= local_E8 + 1.0f)) {
      if ((local_D4 == 6) | (local_D4 == 7)) {
        int32_t local_C8;
        local_C8 = Script_ScenDataUtil(task_0, 4, task_48[arg2] * 13 + 9);
        if (local_C8 != -1) {
          if (arg0 == task_160) {
            Script_executeAicaCommand(local_C8, 0, 0);
          } else {
            Script_Execute3DSoundCommand_Wrapper(local_C8, 4, &local_F4, 5.0f, 20.0f, 0);
          }
        }
      }
      if (local_DC == 0) {
        local_DC = 1;
        local_D8 = Script_ScenDataUtil(task_0, 4, task_48[arg2] * 13 + 7);
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

__attribute__((scn_string_bindings(0, data_126EA)))
static void script_6BDE(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
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
  task_18C = 0;
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
        goto L006EEA;
      }
    }
  }
  L006EEA:;
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
  local_B8.x = (&task_168)->x;
  (&local_B8)->y = task_168.y;
  (&local_B8)->z = task_168.z;
  Script_ryoControl(PCCTL_StartCameraFacingInterpolation, 0);
  Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
  Script_SetCharaMoveTarget(arg0, &task_168, *(&task_174 + 1), local_90);
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
        local_FF40 = Script_ScenDataUtil(task_0, 4, task_48[arg2] * 13 + 10);
        if (local_FF40 != -1) {
          if (arg0 == task_160) {
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
      local_FF40 = Script_ScenDataUtil(task_0, 4, task_48[arg2] * 13 + 9);
      if (local_FF40 != -1) {
        if (arg0 == task_160) {
          Script_executeAicaCommand(local_FF40, 0, 0);
        } else {
          Script_Execute3DSoundCommand_Wrapper(local_FF40, 4, &local_B8, 5.0f, 20.0f, 0);
        }
      }
    }
    if (!(local_A0 < local_94) & !(local_A0 >= local_94 + 1.0f)) {
      int32_t unused_FF44;
      int32_t local_FF40;
      local_FF40 = Script_ScenDataUtil(task_0, 4, task_48[arg2] * 13 + 9);
      if (local_FF40 != -1) {
        if (arg0 == task_160) {
          Script_executeAicaCommand(local_FF40, 0, 0);
        } else {
          Script_Execute3DSoundCommand_Wrapper(local_FF40, 4, &local_B8, 5.0f, 20.0f, 0);
        }
      }
      if (local_FF68 == 0) {
        local_FF68 = 1;
        local_FF64 = Script_ScenDataUtil(task_0, 4, task_48[arg2] * 13 + 8);
        Script_ConversationUtils(1, local_FF74);
        Script_EnqueueSCNT(1, 0x524F4F44, local_FF64);
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  local_A4 = 0.0f;
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0) != 0) {
    local_FF6C = 0;
    L0079B5:;
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
      if (!(local_FF6C >= 20)) goto L0079B5;
    }
    {
      int32_t unused_FF44;
      int32_t local_FF40;
      local_FF40 = Script_ScenDataUtil(task_0, 4, task_48[arg2] * 13 + 11);
      if (local_FF40 != -1) {
        if (arg0 == task_160) {
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

static void script_7B71(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
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
    if (arg0 != task_160) {
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
      Script_SetCharaMoveTarget(arg0, &local_E0, local_BC[1], script_0697(arg5));
    }
  }
  return;
}

static void script_7FBB(int32_t arg0, DSWKDoorEntry *arg1, int32_t *arg2, DoorResourceDescriptor *arg3, MapObjectRecord *arg4, int32_t arg5, int32_t arg6) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t unused_F4;
  task_0 = arg1;
  task_4 = arg2;
  task_8 = arg3;
  task_C = arg4;
  task_10.mode = 0;
  task_10.id = 0;
  task_10.entry_idx = 0;
  task_160 = arg0;
  (void)((task_184 = -1) & 1);
  task_180 = 0;
  Script_motiLoad(Script_GetSceneID(), SHEN_NAME('FREE'), "M_MDOR.BIN");
  local_F8 = 0;
  L008025:;
  local_FC = Script_AddCharacterHandler(1, 0, local_F8 * 0x1000000 + 0x30726F64);
  Script_PreserveAccumulator_012C(local_FC, "This Charactor Work is NULL!!\012");
  DSWK_AllocDoorMapcRenderState(local_FC);
  Script_scenToggleVisibleFor(local_FC, 0);
  task_E8[local_F8] = local_FC;
  task_20[local_F8] = -1;
  task_48[local_F8] = -1;
  task_70[local_F8] = 0;
  task_98[local_F8] = 0;
  task_C0[local_F8] = 0;
  task_110[local_F8] = 0;
  local_F8 = local_F8 + 1;
  if (!(local_F8 >= 10)) goto L008025;
  local_F8 = 0;
  for (;;) {
    if (Script_ScenDataUtil(task_0, 4, local_F8 * 13) == -1) {
      break;
    }
    Script_ScenDataUtil(task_0, 14, local_F8 * 13 + 1, 0);
    local_F8 = local_F8 + 1;
  }
  Script_EnqueueSCNT(0);
  DSWK_BindTablesAndInitState(arg1, arg2, arg3, arg4, &task_10, task_20, task_48, (shen_name *)task_70, task_98, arg0, arg5, data_10B14, data_11104, (shen_name *)task_E8);
  Script_scenCreateTask(script_8877, 0);
  return;
}

static void script_8147(DSWKDoorEntry *arg0, int32_t *arg1, DoorResourceDescriptor *arg2, MapObjectRecord *arg3, int32_t arg4) {
  task_0 = arg0;
  task_4 = arg1;
  task_8 = arg2;
  task_C = arg3;
  DSWK_RebindDoorSystemTables(arg0, arg1, arg2, arg3, arg4);
  task_180 = 1;
  return;
}

static void script_817B(void) {
  int32_t local_FC;
  local_FC = 0;
  L008183:;
  task_20[local_FC] = -1;
  task_48[local_FC] = -1;
  task_70[local_FC] = 0;
  task_98[local_FC] = 0;
  task_C0[local_FC] = 0;
  task_110[local_FC] = 0;
  local_FC = local_FC + 1;
  if (!(local_FC >= 10)) goto L008183;
  return;
}

static void script_81E4(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  local_F4 = 0;
  for (;;) {
    Script_sprintf("手動ドア開け番号指定\012");
    if (task_160 == arg0) {
      Script_EVT_GetPosition(arg0, -1, &task_194, 0);
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

static void script_8281(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  Vector3 local_F0;
  if (task_160 == arg0) {
    Script_EVT_GetPosition(arg0, -1, &task_194, 0);
  }
  Script_EVT_GetPosition(arg0, -1, &local_F0, 0);
  local_FC = DSWK_DoorUtils(DOOR_CMD_FIND_CLOSEST_ELEMENT, arg0, &local_F0, arg1);
  (void)local_FC;
  return;
}

static void script_82BB(void) {
  int32_t local_FC;
  int32_t local_F8;
  local_FC = 0;
  for (;;) {
    if (Script_ScenDataUtil(task_0, 4, local_FC * 13) == -1) {
      break;
    }
    local_F8 = Script_ScenDataUtil(task_0, 4, local_FC * 13 + 1);
    local_F8 = local_F8 | 2;
    Script_ScenDataUtil(task_0, 14, local_FC * 13 + 1, local_F8);
    local_FC = local_FC + 1;
  }
  return;
}

static void script_831E(void) {
  int32_t local_FC;
  int32_t local_F8;
  local_FC = 0;
  for (;;) {
    if (Script_ScenDataUtil(task_0, 4, local_FC * 13) == -1) {
      break;
    }
    local_F8 = Script_ScenDataUtil(task_0, 4, local_FC * 13 + 1);
    local_F8 = local_F8 & -3;
    Script_ScenDataUtil(task_0, 14, local_FC * 13 + 1, local_F8);
    local_FC = local_FC + 1;
  }
  return;
}

static void script_8381(int32_t arg0) {
  int32_t local_FC;
  local_FC = Script_ScenDataUtil(task_0, 4, arg0 * 13 + 1);
  local_FC = local_FC | 2;
  Script_ScenDataUtil(task_0, 14, arg0 * 13 + 1, local_FC);
  return;
}

static void script_83B8(int32_t arg0) {
  int32_t local_FC;
  local_FC = Script_ScenDataUtil(task_0, 4, arg0 * 13 + 1);
  local_FC = local_FC & -3;
  Script_ScenDataUtil(task_0, 14, arg0 * 13 + 1, local_FC);
  return;
}

static void script_83EF(int32_t arg0) {
  Vector3 local_F4;
  int32_t local_F0;
  int32_t local_EC;
  local_F0 = 0;
  for (;;) {
    if (task_48[local_F0] == arg0) {
      local_EC = task_E8[local_F0];
      if (Script_shenGetNode(local_EC, 7)) {
        if (Script_shenGetNode(local_EC, 5) & Script_shenGetNode(local_EC, 10)) {
          if (Script_ScenDataUtil(task_0, 4, task_48[local_F0] * 13 + 1) & 0x100) {
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
          if (Script_ScenDataUtil(task_0, 4, task_48[local_F0] * 13 + 1) & 0x100) {
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
        if (Script_ScenDataUtil(task_0, 4, task_48[local_F0] * 13 + 1) & 0x20) {
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

static void script_8631(int32_t arg0) {
  int32_t local_FC;
  local_FC = Script_ScenDataUtil(task_0, 4, arg0 * 13 + 1);
  local_FC = local_FC | 0x20;
  Script_ScenDataUtil(task_0, 14, arg0 * 13 + 1, local_FC);
  script_8739(arg0);
  script_83EF(arg0);
  return;
}

static void script_867A(int32_t arg0) {
  int32_t local_FC;
  local_FC = Script_ScenDataUtil(task_0, 4, arg0 * 13 + 1);
  local_FC = local_FC & -33;
  Script_ScenDataUtil(task_0, 14, arg0 * 13 + 1, local_FC);
  script_8747(arg0);
  script_83EF(arg0);
  return;
}

static void script_86C3(int32_t arg0, int32_t arg1) {
  if (arg1) {
    int32_t local_FC;
    local_FC = Script_ScenDataUtil(task_0, 4, arg0 * 13 + 1);
    local_FC = local_FC & -257;
    Script_ScenDataUtil(task_0, 14, arg0 * 13 + 1, local_FC);
  } else {
    int32_t local_FC;
    local_FC = Script_ScenDataUtil(task_0, 4, arg0 * 13 + 1);
    local_FC = local_FC | 0x100;
    Script_ScenDataUtil(task_0, 14, arg0 * 13 + 1, local_FC);
  }
  return;
}

static void script_8739(int32_t arg0) {
  script_86C3(arg0, 0);
  return;
}

static void script_8747(int32_t arg0) {
  script_86C3(arg0, 1);
  return;
}

static void script_8755(int32_t arg0) {
  int32_t local_FC;
  local_FC = 0;
  for (;;) {
    if (Script_ScenDataUtil(task_0, 4, local_FC * 13) == -1) {
      break;
    }
    if (arg0) {
      int32_t local_F8;
      local_F8 = Script_ScenDataUtil(task_0, 4, local_FC * 13 + 1);
      local_F8 = local_F8 & -65;
      local_F8 = local_F8 & -129;
      local_F8 = local_F8 | 0x40;
      Script_ScenDataUtil(task_0, 14, local_FC * 13 + 1, local_F8);
    } else {
      int32_t local_F8;
      local_F8 = Script_ScenDataUtil(task_0, 4, local_FC * 13 + 1);
      local_F8 = local_F8 & -65;
      local_F8 = local_F8 & -129;
      local_F8 = local_F8 | 0x80;
      Script_ScenDataUtil(task_0, 14, local_FC * 13 + 1, local_F8);
    }
    local_FC = local_FC + 1;
  }
  return;
}

static void script_881C(void) {
  script_8755(1);
  return;
}

static void script_8828(void) {
  script_8755(0);
  return;
}

static void script_8834(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5, int32_t arg6) {
  int32_t local_FC;
  local_FC = 0;
  for (;;) {
    if (Script_ScenDataUtil(task_0, 4, local_FC * 13) == -1) {
      break;
    }
    DSWK_AssignDoorEntryConfig(local_FC, arg0, arg1, arg2, arg3, arg4, arg5, arg6);
    local_FC = local_FC + 1;
  }
  return;
}

static int32_t script_8877(void) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  SCNF_WaitFrames_0000(1);
  SCNF_WaitFrames_0000(1);
  SCNF_WaitFrames_0000(1);
  local_F4 = -1;
  local_F8 = -1;
  for (;;) {
    if (task_180 == 0) {
      local_F4 = DSWK_DoorUtils(DOOR_CMD_FIND_LOWEST_FLOAT_INDEX, 0x3FD851EA);
      if (-(local_F4 != -1) & -(task_20[local_F4] == 1) & Script_IsPlayerAllowed(0) & -((&task_10)->mode == 0)) {
        local_FC = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_48[local_F4], 2);
        local_F8 = Script_ScenDataUtil(task_8, 4, Script_ScenDataUtil(task_C, 4, local_FC * 9 + 1) * 4 + 1);
        script_9647(1, local_FC, local_F4, local_F8);
        if (script_97FB(local_FC, local_F4, local_F8)) {
          local_F4 = -1;
          local_F8 = -1;
        }
      } else {
        script_9647(0, local_FC, local_F4, local_F8);
        local_F4 = -1;
        local_F8 = -1;
      }
      script_9A11(local_F4, local_F8);
    } else {
      task_180 = 0;
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int8_t script_8988(int32_t arg0, int32_t arg1, int8_t arg2) {
  Vector3 local_F4;
  Vector3 local_E8;
  Vector3i local_DC;
  int32_t local_D8;
  arg2 = 0;
  Script_EVT_GetPosition(task_160, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  Script_mtwkGetSetRotation(task_160, &local_DC, 0x42000000);
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

__attribute__((scn_string_bindings(0, data_12775, 1, data_1278E)))
static void script_8A20(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  Vector3 local_EC;
  Vector3i local_E0;
  local_F8 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_48[arg0], 1);
  if (local_F8 != -1) {
    local_FC = Script_AddCharacterHandler(1, 0, arg0 * 0x1000000 + 0x30786F62);
    if (local_FC == 0) {
      Script_PreserveAccumulator_012C(local_FC, "Don't make BOX Buffer!!\012");
      task_138[arg0] = -1;
    } else if (local_FC == arg0 * 0x1000000 + 0x30786F62) {
      task_138[arg0] = local_FC;
      *(int32_t *)&local_EC.x = Script_ScenDataUtil(task_C, 4, local_F8 * 9 + 5);
      *(int32_t *)&(&local_EC)->y = Script_ScenDataUtil(task_C, 4, local_F8 * 9 + 6);
      *(int32_t *)&(&local_EC)->z = Script_ScenDataUtil(task_C, 4, local_F8 * 9 + 7);
      Script_EVT_SetPosition(local_FC, POS_XYZ, &local_EC);
      local_E0.x = 0;
      (&local_E0)->y = Script_ScenDataUtil(task_C, 4, local_F8 * 9 + 8);
      (&local_E0)->z = 0;
      Script_EVT_SetRotation(local_FC, 0x38000000, &local_E0, 0);
      *(int32_t *)&local_EC.x = Script_ScenDataUtil(task_C, 4, local_F8 * 9 + 2);
      *(int32_t *)&(&local_EC)->y = Script_ScenDataUtil(task_C, 4, local_F8 * 9 + 3);
      *(int32_t *)&(&local_EC)->z = Script_ScenDataUtil(task_C, 4, local_F8 * 9 + 4);
      Script_hmdlApplyScale(local_FC, &local_EC);
      Script_CreateNPCImageModule(local_FC, (const char *)Script_ScenDataUtil(task_8, 8, Script_ScenDataUtil(task_C, 4, local_F8 * 9 + 1) * 4), 0);
      Script_charaDisplay(local_FC, CDM_Show);
      Script_charaVisUtils(local_FC, 1);
    } else {
      Script_PreserveAccumulator_012C(local_FC, "Don't make BOX Buffer!!\012");
      task_138[arg0] = -1;
    }
  } else {
    task_138[arg0] = -1;
  }
  return;
}

static void script_8BEF(int32_t arg0) {
  if (task_138[arg0] != -1) {
    Script_SetMapControlTaskFlag(task_138[arg0], 1);
    task_138[arg0] = -1;
  }
  return;
}

static int32_t script_8C20(int32_t arg0, int32_t arg1, int32_t arg2, float arg3, float arg4, int32_t arg5, int32_t arg6) {
  Vector3 local_F4;
  Vector3 local_E8;
  float local_DC[3];
  Vector3i local_D0;
  Vector3i local_C4;
  int32_t local_C0;
  int32_t local_BC;
  if (task_160 == arg0) {
    local_F4.x = (&task_194)->x;
    (&local_F4)->y = task_194.y;
    (&local_F4)->z = task_194.z;
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
        if ((Script_ScenDataUtil(task_0, 4, arg6 * 13 + 1) & 8) | (Script_ScenDataUtil(task_0, 4, arg6 * 13 + 1) & 0x40)) {
          local_C0 = local_C0 ^ 3;
        }
      } else {
        if ((Script_ScenDataUtil(task_0, 4, arg6 * 13 + 1) & 0x10) | (Script_ScenDataUtil(task_0, 4, arg6 * 13 + 1) & 0x80)) {
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
          if ((local_B4 == 1) | (local_B4 == 2)) goto L008E0B;
        }
        L008E0B:;
      } else if ((local_B4 == 2) | (local_B4 == 4) | (local_B4 == 6) | (local_B4 == 8) | (local_B4 == 10) | (local_B4 == 12) | (local_B4 == 24) | (local_B4 == 26) | (local_B4 == 31) | (local_B4 == 33) | (local_B4 == 35)) {
        local_B4 = local_C0;
        if ((local_B4 == 2) | (local_B4 == 1)) {
          arg3 = (float)0 - arg3;
        } else {
          if ((local_B4 == 3) | (local_B4 == 0)) {
            arg3 = (float)0 - arg3;
            arg4 = (float)0 - arg4;
            (&local_C4)->y = (&local_C4)->y + 0x8000;
            goto L008EAE;
          }
        }
        L008EAE:;
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
            goto L00905A;
          }
        }
      }
    }
    L00905A:;
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
          task_168.x = (&local_F4)->x;
          task_168.y = (&local_F4)->y;
          task_168.z = (&local_F4)->z;
          task_174 = (&local_C4)->x;
          *(&task_174 + 1) = (&local_C4)->y;
          *(&task_174 + 2) = (&local_C4)->z;
          break;
        case 5:
          task_168.x = (&local_F4)->x;
          task_168.y = (&local_F4)->y;
          task_168.z = (&local_F4)->z;
          task_174 = (&local_C4)->x;
          *(&task_174 + 1) = (&local_C4)->y;
          *(&task_174 + 2) = (&local_C4)->z;
          if ((local_C0 == 1) | (local_C0 == 2)) {
            local_C0 = 1;
          } else {
            local_C0 = 0;
          }
          break;
        case 7:
          Script_EVT_GetPosition(arg0, -1, &task_168, 0);
          task_174 = (&local_C4)->x;
          *(&task_174 + 1) = (&local_C4)->y;
          *(&task_174 + 2) = (&local_C4)->z;
          break;
      }
    }
  }
  return local_C0;
}

static void script_92E9(int32_t arg0, int32_t arg1) {
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
    if (task_160 == arg0) {
      if (arg1) {
        Script_ccowUtils(arg0, 3);
      } else {
        Script_ccowUtils(arg0, 4);
      }
    }
  }
  return;
}

static void script_931B(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  int32_t local_F8;
  local_F8 = script_8C20(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
  if (task_160 != arg0) {
    if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
      Script_charaToggleMotEnabled(arg0, 1);
    }
  } else if ((local_F8 == 0) | (local_F8 == 1)) {
    if ((Script_ScenDataUtil(task_0, 4, arg2 * 13 + 1) & 8) == 0) {
      if ((Script_ScenDataUtil(task_0, 4, arg2 * 13 + 1) & 0x40) != 0) {
        local_FC = Script_ScenDataUtil(task_0, 4, arg2 * 13 + 2) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_FC, 0);
      } else {
        local_FC = (Script_ScenDataUtil(task_0, 4, arg2 * 13 + 2) >> 16) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_FC, 0);
      }
    }
  } else {
    if ((Script_ScenDataUtil(task_0, 4, arg2 * 13 + 1) & 0x10) == 0) {
      if ((Script_ScenDataUtil(task_0, 4, arg2 * 13 + 1) & 0x80) != 0) {
        local_FC = (Script_ScenDataUtil(task_0, 4, arg2 * 13 + 2) >> 16) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_FC, 0);
      } else {
        local_FC = Script_ScenDataUtil(task_0, 4, arg2 * 13 + 2) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_FC, 0);
      }
    }
  }
  return;
}

__attribute__((scn_string_bindings(0, data_127A7)))
static void script_948B(int32_t arg0, int32_t arg1, int32_t arg2) {
  int32_t local_FC;
  if (task_160 == arg0) {
    task_10.mode = 0;
    task_10.id = 0;
    if ((arg2 == 0) | (arg2 == 2)) {
      Script_PlayCharaMotion(task_160, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
      if (task_184 & 0x20) {
        Script_sprintf("Door System : ");
        Script_ryoControl(RYO_RestoreControl);
      }
      Script_shenClearFreeState(1);
      if (arg2 != 2) {
        int32_t local_F8;
        local_F8 = task_48[arg1];
        local_FC = Script_ScenDataUtil(task_0, 4, local_F8 * 13 + 2) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_FC, 1);
        local_FC = (Script_ScenDataUtil(task_0, 4, local_F8 * 13 + 2) >> 16) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_FC, 1);
      }
    }
  } else {
    if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
      Script_charaToggleMotEnabled(arg0, 0);
    }
    if ((arg2 == 0) | (arg2 == 2)) {
      int32_t local_F8;
      local_F8 = task_48[arg1];
      local_FC = Script_ScenDataUtil(task_0, 4, local_F8 * 13 + 2) & 0xFFFF;
      Script_scenToggleCharaFlag(0, local_FC, 1);
      local_FC = (Script_ScenDataUtil(task_0, 4, local_F8 * 13 + 2) >> 16) & 0xFFFF;
      Script_scenToggleCharaFlag(0, local_FC, 1);
    }
  }
  task_20[arg1] = 1;
  task_110[arg1] = 0;
  task_C0[arg1] = 0;
  task_70[arg1] = 0;
  if (task_138[arg1] != -1) {
    Script_SetMapControlTaskFlag(task_138[arg1], 1);
    task_138[arg1] = -1;
  }
  return;
}

static void script_9647(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  Vector3 local_E8;
  local_FC = 0;
  if (arg0 == 1) {
    local_F4 = arg3;
    if ((local_F4 == 17) | (local_F4 == 11) | (local_F4 == 12)) {
      local_FC = 0;
    } else if ((Script_ScenDataUtil(task_0, 4, task_48[arg2] * 13 + 1) & 2) == 0) {
      if ((Script_ScenDataUtil(task_0, 4, task_48[arg2] * 13 + 1) & 4) == 0) {
        local_FC = script_8988(arg2, arg3, local_FC);
      } else {
        local_F4 = arg3;
        if ((local_F4 == 21) | (local_F4 == 22) | (local_F4 == 23)) {
          local_FC = script_8988(arg2, arg3, local_FC);
          goto L00970B;
        }
      }
      L00970B:;
    } else {
      if ((Script_ScenDataUtil(task_0, 4, task_48[arg2] * 13 + 1) & 4) == 0) {
        local_FC = script_8988(arg2, arg3, local_FC);
        local_F8 = Script_ScenDataUtil(task_0, 4, task_48[arg2] * 13 + 6);
        if (local_F8 == 0) {
          local_FC = 0;
        } else {
          if (local_F8 >= 2) {
            Script_EVT_GetPosition(task_160, -1, &local_E8, 0);
            if (Script_CycleManagerUtils(CYCLE_CMD_COUNT_CYCLES_IN_RADIUS, &local_E8, 1.5f)) {
              local_FC = 0;
            }
            goto L0097A6;
          }
        }
      }
    }
  }
  L0097A6:;
  if (task_110[arg2]) {
    local_FC = 0;
  }
  Script_scenUtils(SCENARIO_CMD_SET_HELP_PROMPT_OVERRIDE, local_FC);
  if ((local_FC ^ task_190) & local_FC) {
    Script_sicnUtils(0, 4, 1);
  }
  if ((local_FC ^ task_190) & task_190) {
    Script_sicnUtils(2, 4, 0);
  }
  task_190 = local_FC;
  return;
}

__attribute__((scn_string_bindings(0, data_127B6)))
static int32_t script_97FB(int32_t arg0, int32_t arg1, int32_t arg2) {
  int32_t local_FC;
  int32_t local_F8;
  Vector3 local_EC;
  local_FC = 1;
  local_F8 = arg2;
  if ((local_F8 == 17) | (local_F8 == 11) | (local_F8 == 12)) {
    if (((Script_ScenDataUtil(task_0, 4, task_48[arg1] * 13 + 1) & 2) == 0) & ((Script_ScenDataUtil(task_0, 4, task_48[arg1] * 13 + 1) & 4) == 0)) {
      if (task_110[arg1] == 0) {
        task_20[arg1] = 4;
        local_FC = 0;
      }
    }
  } else {
    if (Script_GetControllerInput(1) == 0x400) {
      if (-(task_70[arg1] != task_160) & task_190) {
        if ((Script_ScenDataUtil(task_0, 4, task_48[arg1] * 13 + 1) & 2) == 0) {
          task_20[arg1] = 3;
          task_70[arg1] = task_160;
          task_98[arg1] = 0;
          task_10.mode = 1;
          task_10.entry_idx = task_48[arg1];
          local_FC = 0;
        } else {
          if (Script_ScenDataUtil(task_0, 4, task_48[arg1] * 13 + 6) >= 2) {
            Script_EVT_GetPosition(task_160, -1, &local_EC, 0);
            if (Script_CycleManagerUtils(CYCLE_CMD_COUNT_CYCLES_IN_RADIUS, &local_EC, 1.5f) == 0) {
              task_20[arg1] = 6;
              task_70[arg1] = task_160;
              task_98[arg1] = 0;
              task_10.mode = 0;
              task_10.entry_idx = task_48[arg1];
              local_FC = 0;
            }
            goto L0099AF;
          }
        }
        L0099AF:;
        if (local_FC == 0) {
          Script_EVT_GetPosition(task_160, -1, &task_194, 0);
          Script_ryoControl(PCCTL_RequestNewMode, 4);
          Script_sprintf("Door System : ");
          Script_shenClearFreeState(0);
          Script_ryoControl(PCCTL_DisablePlayerControl);
          task_10.id = task_160;
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

static void script_9A11(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  int32_t local_F8;
  local_FC = 0;
  L009A19:;
  local_F8 = task_20[local_FC];
  switch (local_F8) {
    case 6:
      if (!(arg0 != -1)) goto L009A39;
      L009A39:;
      script_9ADB(local_FC, arg1);
      break;
    case 5:
      if (!(arg0 != -1)) goto L009A59;
      L009A59:;
      script_9B3C(local_FC);
      break;
    case 4:
      if (!(arg0 != -1)) goto L009A77;
      L009A77:;
      script_9CE1(local_FC, arg0, arg1);
      break;
    case 3:
      if (!(arg0 != -1)) goto L009A99;
      L009A99:;
      script_9D79(local_FC, arg1);
      break;
    case 2:
      script_9E5B(local_FC);
      break;
    case 1:
      break;
  }
  local_FC = local_FC + 1;
  if (!(local_FC >= 10)) goto L009A19;
  return;
}

static void script_9ADB(int32_t arg0, int32_t arg1) {
  if (task_110[arg0] == 0) {
    task_110[arg0] = Script_scenCreateTask(script_1C76, 4, task_70[arg0], task_E8[arg0], arg0, arg1);
    task_C0[arg0] = task_70[arg0];
    script_8A20(arg0);
  }
  return;
}

__attribute__((scn_string_bindings(0, data_127C5, 1, data_127D7)))
static void script_9B3C(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, task_70[arg0])) {
    if (task_110[arg0] == 0) {
      if (task_70[arg0] == task_160) {
        task_10.mode = 1;
        task_10.entry_idx = task_48[arg0];
        goto L009B91;
      }
      L009B91:;
      local_FC = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_48[arg0], 2);
      local_F8 = Script_ScenDataUtil(task_8, 4, Script_ScenDataUtil(task_C, 4, local_FC * 9 + 1) * 4 + 1);
      script_931B(task_70[arg0], task_E8[arg0], task_48[arg0], local_F8);
      task_110[arg0] = Script_scenCreateTask(Script_ScenDataUtil(&data_11BC4, 8, local_F8), 4, task_70[arg0], task_E8[arg0], arg0, local_F8);
      task_C0[arg0] = task_70[arg0];
      if ((Script_ScenDataUtil(task_0, 4, task_48[arg0] * 13 + 1) & 0x20000) == 0) {
        Script_sprintf("Door Box is ON!!\012");
        script_8A20(arg0);
      } else {
        Script_sprintf("Door Box is OFF!!\012");
        task_138[arg0] = -1;
      }
    }
  } else {
    if (task_20[arg0] != 5) {
      task_20[arg0] = 1;
      task_110[arg0] = 0;
      task_C0[arg0] = 0;
      task_70[arg0] = 0;
    }
  }
  return;
}

static void script_9CE1(int32_t arg0, int32_t arg1, int32_t arg2) {
  if ((task_110[arg0] == 0) & (arg2 >= 0)) {
    task_70[arg0] = task_160;
    task_10.mode = 2;
    task_10.entry_idx = task_48[arg0];
    task_110[arg0] = Script_scenCreateTask(Script_ScenDataUtil(&data_11BC4, 8, arg2), 4, task_70[arg0], task_E8[arg0], arg0, arg2);
    task_C0[arg0] = task_70[arg0];
    script_8A20(arg0);
  }
  return;
}

__attribute__((scn_string_bindings(0, data_127EA, 1, data_127FC)))
static void script_9D79(int32_t arg0, int32_t arg1) {
  if ((task_110[arg0] == 0) & (arg1 >= 0)) {
    script_931B(task_70[arg0], task_E8[arg0], task_48[arg0], arg1);
    task_110[arg0] = Script_scenCreateTask(Script_ScenDataUtil(&data_11BC4, 8, arg1), 4, task_70[arg0], task_E8[arg0], arg0, arg1);
    task_C0[arg0] = task_70[arg0];
    if ((Script_ScenDataUtil(task_0, 4, task_48[arg0] * 13 + 1) & 0x20000) == 0) {
      Script_sprintf("Door Box is ON!!\012");
      script_8A20(arg0);
    } else {
      Script_sprintf("Door Box is OFF!!\012");
      task_138[arg0] = -1;
    }
  }
  return;
}

static void script_9E5B(int32_t arg0) {
  int32_t local_FC;
  task_110[arg0] = 0;
  task_C0[arg0] = 0;
  task_20[arg0] = 1;
  local_FC = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_48[arg0], 2);
  if (local_FC != -1) {
    task_70[arg0] = 0;
  }
  return;
}

__attribute__((scn_string_bindings(0, data_1288C)))
static int32_t script_9EB2(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
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
  script_A7B4(arg0, arg1, arg2, arg3, arg4, arg5);
  local_E8[0] = local_E8[2] = 0;
  while (1) {
    while (*(&task_1AC + arg5) & 4) {
      SCNF_WaitFrames_0000(1);
    }
    arg2 = Script_KaidanUtils(CAM_STAIR_QUERY_GUIDE_ENTRY, arg0, task_1A0, (const void *)task_1A4, (const void *)task_1A8, task_1C0);
    if (arg2 >= 0) {
      Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_2);
      Script_ccowUtils(arg0, 9);
      if (*(&task_1AC + arg5) & 0x800) {
        Script_scenUtils(SCENARIO_CMD_INIT_SCNT);
      }
      (void)((local_CA = 0) & 1);
      local_E4 = arg2 * 6;
      *(int32_t *)&local_F4[1] = Script_ScenDataUtil((void *)task_1A4, 4, local_E4 + 1);
      *(int32_t *)&local_F4[0] = Script_ScenDataUtil((void *)task_1A4, 4, local_E4);
      *(int32_t *)&local_F4[2] = Script_ScenDataUtil((void *)task_1A4, 4, local_E4 + 2);
      local_DC = Script_ScenDataUtil((void *)task_1A4, 4, local_E4 + 4);
      local_E8[1] = Script_ScenDataUtil((void *)task_1A4, 4, local_E4 + 3);
      local_E0 = Script_ScenDataUtil((void *)task_1A4, 4, local_E4 + 5);
      local_D8 = Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, local_E8[1]);
      local_D4 = Script_MathUtils(SCRIPT_MATH_COS_FIXED16, local_E8[1]);
      script_BF25(arg0, arg5, arg2);
      *(&task_1B8 + arg5) = arg2 + 1;
      *((int8_t *)&task_1E4 + arg5) = 3;
      script_CBB6(arg0, arg5, local_DC);
      Script_CharHandlers(arg0, 2, 0, 16);
      (void)((local_CD = -((local_CB = 0) != 0)) & 1);
      if (local_DC & 0xF00 & 0x400) {
        script_A8A7(1, arg2);
        if (local_DC & 0xF000 & 0x1000) {
          (void)((task_1EC = -1) & 1);
          while (task_1EC & 1) {
            SCNF_WaitFrames_0000(1);
          }
        }
      }
      if (local_DC & 0x100) {
        (void)((local_CA = -1) & 1);
      }
      if (*(&task_1AC + arg5) & 0x400) {
        Script_MotStepUtil(arg0, 0);
      }
      if ((local_DC & 0xF) <= 4) {
        Script_ccowSetFlag80(arg0, 1);
        if ((*(&task_1AC + arg5) & 8) == 0) {
          if (local_DC & 0x1000000) {
            Script_scenCreateTask(script_C347, 2, arg0, 1);
          } else {
            Script_scenCreateTask(script_C347, 2, arg0, 1);
          }
        }
        if (local_DC & 0x2000) {
          task_1E9 = 0;
        } else {
          uint8_t unused_C8[2];
          int32_t local_C4;
          local_C4 = *(&task_1B0 + arg5);
          switch (local_C4) {
            case 0:
              task_1E9 = 0;
              break;
            case 1:
              task_1E9 = 1;
              break;
            case 3:
              task_1E9 = 1;
              break;
            case 2:
              if (Script_ryoControl(PCCTL_QueryIsRunningRequest)) {
                task_1E9 = 1;
              } else {
                task_1E9 = 0;
              }
              break;
          }
        }
        if (task_1E9) {
          local_F4[0] = *local_F4 + local_D8 * -0.1f;
          local_F4[2] = local_F4[2] + local_D4 * -0.1f;
        } else {
          local_F4[0] = *local_F4 + local_D8 * 0.16f;
          local_F4[2] = local_F4[2] + local_D4 * 0.16f;
        }
        local_CF = script_B989(arg0, local_E8[1], *local_F4, local_F4[2], arg5, local_DC, local_E0);
        if (local_CA & 4) {
          Script_KaidanUtils(CAM_STAIR_APPLY_GUIDE_FACING, arg0, arg2, (const void *)task_1A4);
        }
        (void)((local_CC = -1) & 1);
        Script_ccowSetFlag80(arg0, 2);
        Script_ccowUnknown(arg0, 1, 3.0f);
      } else {
        if ((local_DC & 0xF) <= 12) {
          if ((*(&task_1AC + arg5) & 8) == 0) {
            Script_scenCreateTask(script_C347, 2, arg0, 0);
          }
          if (local_DC & 0x2000) {
            task_1E9 = 0;
          } else {
            uint8_t unused_C8[2];
            int32_t local_C4;
            local_C4 = *(&task_1B0 + arg5);
            switch (local_C4) {
              case 0:
                task_1E9 = 0;
                break;
              case 1:
                task_1E9 = 1;
                break;
              case 3:
                task_1E9 = 1;
                break;
              case 2:
                if (Script_ryoControl(PCCTL_QueryIsRunningRequest)) {
                  task_1E9 = 1;
                } else {
                  task_1E9 = 0;
                }
                break;
            }
          }
          if (task_1E9) {
            local_F4[0] = *local_F4 + local_D8 * -0.25f;
            local_F4[2] = local_F4[2] + local_D4 * -0.25f;
          } else {
            local_F4[0] = *local_F4 + local_D8 * 0.15f;
            local_F4[2] = local_F4[2] + local_D4 * 0.15f;
          }
          local_CF = local_E0 - script_BC18(arg0, local_E8[1], *local_F4, local_F4[2], arg5, local_DC, local_E0);
          if (local_CA & 4) {
            Script_KaidanUtils(CAM_STAIR_APPLY_GUIDE_FACING, arg0, arg2, (const void *)task_1A4);
          }
          Script_ccowSetFlag80(arg0, 2);
          Script_ccowUnknown(arg0, 1, 3.0f);
          (void)((local_CC = 0) & 0x80);
          goto L00A3A7;
        }
      }
      L00A3A7:;
      if ((local_E0 == 1) & ((local_DC & 0xF) <= 4)) {
        if (*((int8_t *)&task_1E7 + arg5) == 1) {
          Script_motmToggleFlags(arg0, 1, 0x80);
        }
        if (task_1E9) {
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
          Script_scenCreateTask(script_C95D, 2, arg0, arg5);
        } else {
          Script_scenCreateTask(script_CA85, 2, arg0, arg5);
        }
        while ((local_CF >= 0) & (local_CF <= local_E0)) {
          local_CE = task_1E9;
          if (local_D0) {
            local_D0 = 0;
          } else if (local_DC & 0x2000) {
            task_1E9 = 0;
          } else {
            uint8_t unused_C8[2];
            int32_t local_C4;
            local_C4 = *(&task_1B0 + arg5);
            switch (local_C4) {
              case 0:
                task_1E9 = 0;
                break;
              case 1:
                task_1E9 = 1;
                break;
              case 3:
                task_1E9 = 1;
                break;
              case 2:
                if (Script_ryoControl(PCCTL_QueryIsRunningRequest)) {
                  task_1E9 = 1;
                } else {
                  task_1E9 = 0;
                }
                break;
            }
          }
          if (local_CB & 8) {
          } else {
            if (*(&task_1AC + arg5) & 0x100) {
              if (*(&task_1AC + arg5) & 2) {
                if ((Script_GetControllerInput(0) & 0x2000) > 0) {
                  (void)((local_CD = -1) & 4);
                }
              }
            }
            if (local_CC & 0x10) {
              local_CF = local_CF + script_AC8E(arg0, local_CF, local_E0, arg5, local_DC, arg2, local_CE - task_1E9);
            } else {
              local_CF = local_CF - script_B17E(arg0, local_CF, arg5, local_DC, arg2, local_CE - task_1E9);
            }
            if ((local_CF > 0) & (local_CF < local_E0)) {
              if (!(local_CD & 0x20)) goto L00A5E9;
            }
          }
          L00A5E9:;
        }
        if (local_D0) {
          if (((*(&task_1AC + arg5) & 0x10) == 0) & ((local_DC & 0xF) < 2)) {
            if (*((int8_t *)&task_1E7 + arg5) == 1) {
              Script_motmToggleFlags(arg0, 1, 0x80);
            }
            if (task_1E9) {
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
      if ((task_1EA == 1) | (task_1EA == 2)) {
        task_1EA = 3;
      }
      if ((*(&task_1AC + arg5) & 2) > 0) {
        if (task_1E9) {
          Script_SetCharaMotionPlaybackRate(arg0, 1.2f);
        } else {
          Script_SetCharaMotionPlaybackRate(arg0, 1.3f);
        }
        if (task_1F0 & 2) {
          Script_sprintf("Kaidan : ");
          Script_ryoControl(RYO_RestoreControl);
          (void)((task_1F0 = 0) & 4);
        }
        script_CC8B(arg0, arg5, local_DC);
      }
      Script_ccowUnknown(arg0, 2, 0.07f);
      Script_ccowUtils(arg0, 10);
      if (*((int8_t *)&task_1E5 + arg5) != 2) {
        *((int8_t *)&task_1E5 + arg5) = 0;
      }
      *((int8_t *)&task_1E6 + arg5) = 0;
      *((int8_t *)&task_1E4 + arg5) = 0;
      *(&task_1B8 + arg5) = -1;
      if (*(&task_1AC + arg5) & 0x800) {
        Script_scenUtils(SCENARIO_CMD_UPDATE_TIMER);
      }
    } else {
      (void)((task_1F0 = 0) & 1);
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_A7B4(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
  int32_t local_FC;
  int32_t local_F8;
  *(&task_1AC + arg5) = arg4;
  *(&task_1B4 + arg5) = arg0;
  *(&task_1B8 + arg5) = -1;
  *(&task_1B0 + arg5) = arg1;
  *((int8_t *)&task_1E5 + arg5) = 0;
  *((int8_t *)&task_1E6 + arg5) = 0;
  Script_sprintf("========== initializing kaidan\012");
  if (arg3) {
    (void)((task_1EB = 0) & 1);
    task_1C0 = 0x4000;
    task_1E8 = 1;
    task_1A4 = arg3;
    (void)((task_1F0 = 0) & 1);
    if (arg2) {
      Script_sprintf("please ENDOFKDATA use can't start shuki kaidan\012");
      task_1A0 = arg2;
    } else {
      Script_CycleManagerUtils(CYCLE_CMD_SET_GLOBAL_CYCLE_PTR, task_1A4, &task_1B8);
      task_1A0 = local_F8 = 0;
      while ((local_FC = Script_ScenDataUtil((void *)task_1A4, 4, local_F8)) != 0x47C35000) {
        task_1A0 = task_1A0 + 1;
        local_F8 = local_F8 + 6;
      }
    }
  } else {
    task_1E8 = task_1E8 + 1;
  }
  return;
}

static void script_A8A7(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  int32_t local_F0;
  local_F4 = 0;
  local_FC = 0;
  while ((local_F8 = Script_ScenDataUtil((void *)task_1A8, 4, local_FC)) != 3) {
    local_F0 = local_F8;
    if (local_F0 == 0) {
      local_FC = local_FC + 3;
    } else if (local_F0 == 1) {
      if (arg0) {
        if (Script_ScenDataUtil((void *)task_1A8, 4, local_FC + 1) == arg1) {
          local_F4 = Script_ScenDataUtil((void *)task_1A8, 4, local_FC + 2);
          if (local_F4 < 4) {
            local_F4 = task_1C4[local_F4];
            break;
          } else {
            Script_PreserveAccumulator_012C(0, "Kaidan Function Error 1\012");
          }
        }
      }
      local_FC = local_FC + 3;
    } else if (local_F0 == 2) {
      if (arg0 == 0) {
        if (Script_ScenDataUtil((void *)task_1A8, 4, local_FC + 1) == arg1) {
          local_F4 = Script_ScenDataUtil((void *)task_1A8, 4, local_FC + 2);
          if (local_F4 < 4) {
            local_F4 = task_1C4[local_F4];
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

static void script_A9E6(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int8_t local_FF;
  int32_t local_F8;
  if (arg2 & 0x2000) {
    task_1E9 = 0;
  } else {
    local_F8 = *(&task_1B0 + arg1);
    switch (local_F8) {
      case 0:
        task_1E9 = 0;
        break;
      case 1:
        task_1E9 = 1;
        break;
      case 3:
        task_1E9 = 1;
        break;
      case 2:
        if (Script_ryoControl(PCCTL_QueryIsRunningRequest)) {
          task_1E9 = 1;
        } else {
          task_1E9 = 0;
        }
        break;
    }
  }
  if (task_1E9) {
    if (arg3 > 1) {
      Script_PlayCharaMotion(arg0, MN_AKI_AKI_NOBORU_KAIDAN_2STEP_SONOBA_LP, -1.0f, -1.0f, 9.0f, 1.0f, 0);
      local_FF = 2;
      if (arg2 & 0x100000) {
        script_C5BB(arg0);
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

static void script_AB95(int32_t arg0, int32_t arg1, int32_t arg2) {
  int8_t local_FF;
  int32_t local_F8;
  if (arg2 & 0x2000) {
    task_1E9 = 0;
  } else {
    local_F8 = *(&task_1B0 + arg1);
    switch (local_F8) {
      case 0:
        task_1E9 = 0;
        break;
      case 1:
        task_1E9 = 1;
        break;
      case 3:
        task_1E9 = 1;
        break;
      case 2:
        if (Script_ryoControl(PCCTL_QueryIsRunningRequest)) {
          task_1E9 = 1;
        } else {
          task_1E9 = 0;
        }
        break;
    }
  }
  if (task_1E9) {
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

static int8_t script_AC8E(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5, int32_t arg6) {
  int8_t local_FF;
  uint8_t unused_F0[15];
  task_1D4 = 0;
  if (task_1E9) {
    local_FF = 4;
    if (arg1 < arg2 - 3) {
      Script_motmToggleFlags(arg0, 2, 0x40);
      if (arg6) {
        if (*((int8_t *)&task_1E7 + arg3) == 1) {
          Script_motmToggleFlags(arg0, 1, 0x80);
        }
        Script_PlayCharaMotion(arg0, MN_AKI_AKI_NOBORU_KAIDAN_2STEP_SONOBA_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
      }
      if (arg4 & 0x100000) {
        script_C696(arg0);
      } else {
        SCNF_WaitFrames_0000(1);
        while (Script_IsCharaMotionFinished(arg0) == 0) {
          SCNF_WaitFrames_0000(1);
        }
      }
      Script_motmToggleFlags(arg0, 1, 0x40);
    } else {
      if ((*(&task_1AC + arg3) & 8) == 0) {
        task_1EA = 2;
      }
      script_CC3B(arg0, arg3, arg4);
      if (arg4 & 0xF00 & 0x800) {
        script_A8A7(0, arg5);
        if (arg4 & 0xF000 & 0x1000) {
          (void)((task_1EC = -1) & 1);
          while (task_1EC & 1) {
            SCNF_WaitFrames_0000(1);
          }
        }
      } else if (arg2 <= arg1 + 2) {
        if (*((int8_t *)&task_1E7 + arg3) == 0) {
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
          if (*((int8_t *)&task_1E7 + arg3) == 1) {
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
        if (*((int8_t *)&task_1E7 + arg3) == 0) {
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
      if ((*(&task_1AC + arg3) & 8) == 0) {
        task_1EA = 3;
      }
    }
  } else {
    local_FF = 2;
    if (arg1 < arg2 - 1) {
      if (arg6) {
        if (*((int8_t *)&task_1E7 + arg3) == 1) {
          Script_motmToggleFlags(arg0, 1, 0x80);
        }
        Script_PlayCharaMotion(arg0, MN_AKI_AKI_NOBORU_KAIDAN_SONOBA_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
      }
      if (arg4 & 0x100000) {
        script_C4E0(arg0);
      } else {
        SCNF_WaitFrames_0000(1);
        while (Script_IsCharaMotionFinished(arg0) == 0) {
          SCNF_WaitFrames_0000(1);
        }
      }
    } else {
      Script_ccowSetFlag80(arg0, 1);
      if ((*(&task_1AC + arg3) & 8) == 0) {
        task_1EA = 2;
      }
      script_CC3B(arg0, arg3, arg4);
      if (arg6) {
        if (*((int8_t *)&task_1E7 + arg3) == 1) {
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
        script_A8A7(0, arg5);
        if (arg4 & 0xF000 & 0x1000) {
          (void)((task_1EC = -1) & 2);
          while (task_1EC & 1) {
            SCNF_WaitFrames_0000(1);
          }
        }
      } else {
        if (arg2 == arg1) {
          if (*((int8_t *)&task_1E7 + arg3) == 0) {
            Script_motmToggleFlags(arg0, 1, 0x80);
          }
        } else {
          if (*((int8_t *)&task_1E7 + arg3) == 1) {
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
      if ((*(&task_1AC + arg3) & 8) == 0) {
        task_1EA = 3;
      }
    }
  }
  return local_FF;
}

static int8_t script_B17E(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
  int8_t local_FF;
  if (task_1E9) {
    local_FF = 4;
    if (arg1 >= local_FF) {
      if (arg5) {
        if (*((int8_t *)&task_1E7 + arg2) == 1) {
          Script_motmToggleFlags(arg0, 1, 0x80);
        }
        Script_PlayCharaMotion(arg0, MN_AKI_AKI_ORIRU_KAIDAN_2STEP_SONOBA_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
      }
      if (arg3 & 0x100000) {
        script_C867(arg0, arg2);
      } else {
        SCNF_WaitFrames_0000(1);
        while (Script_IsCharaMotionFinished(arg0) == 0) {
          SCNF_WaitFrames_0000(1);
        }
      }
    } else {
      if (*((int8_t *)&task_1E5 + arg2) != 2) {
        *((int8_t *)&task_1E5 + arg2) = 0;
      }
      switch (arg1) {
        case 1:
          if (arg5) {
            if (*((int8_t *)&task_1E7 + arg2) == 1) {
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
            if (*((int8_t *)&task_1E7 + arg2) == 1) {
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
            if (*((int8_t *)&task_1E7 + arg2) == 1) {
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
      if ((*(&task_1AC + arg2) & 8) == 0) {
        task_1EA = 2;
      }
      script_CC3B(arg0, arg2, arg3);
      if (arg3 & 0xF00 & 0x800) {
        script_A8A7(0, arg4);
        if (arg3 & 0xF000 & 0x1000) {
          (void)((task_1EC = -1) & 1);
          while (task_1EC & 1) {
            SCNF_WaitFrames_0000(1);
          }
        }
      } else {
        if (arg1 <= 2) {
          if (*((int8_t *)&task_1E7 + arg2) == 1) {
            Script_motmToggleFlags(arg0, 1, 0x80);
          }
        } else {
          if (*((int8_t *)&task_1E7 + arg2) == 0) {
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
      if ((*(&task_1AC + arg2) & 8) == 0) {
        task_1EA = 3;
      }
    }
  } else {
    local_FF = 2;
    if (arg1 >= local_FF) {
      if (arg5) {
        if (*((int8_t *)&task_1E7 + arg2) == 1) {
          Script_motmToggleFlags(arg0, 1, 0x80);
        }
        Script_PlayCharaMotion(arg0, MN_AKI_AKI_ORIRU_KAIDAN_SONOBA_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
      }
      if (arg3 & 0x100000) {
        script_C771(arg0, arg2);
      } else {
        SCNF_WaitFrames_0000(1);
        while (Script_IsCharaMotionFinished(arg0) == 0) {
          SCNF_WaitFrames_0000(1);
        }
      }
    } else {
      if (*((int8_t *)&task_1E5 + arg2) != 2) {
        *((int8_t *)&task_1E5 + arg2) = 0;
      }
      if (arg1) {
        if (arg5) {
          if (*((int8_t *)&task_1E7 + arg2) == 1) {
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
      if ((*(&task_1AC + arg2) & 8) == 0) {
        task_1EA = 2;
      }
      if (arg3 & 0xF00 & 0x800) {
        script_A8A7(0, arg4);
        if (arg3 & 0xF000 & 0x1000) {
          (void)((task_1EC = -1) & 1);
          while (task_1EC & 1) {
            SCNF_WaitFrames_0000(1);
          }
        }
        script_CC3B(arg0, arg2, arg3);
      } else {
        if (arg1) {
          if (*((int8_t *)&task_1E7 + arg2) == 1) {
            Script_motmToggleFlags(arg0, 1, 0x80);
          }
        } else {
          if (*((int8_t *)&task_1E7 + arg2) == 0) {
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
        script_CC3B(arg0, arg2, arg3);
        SCNF_WaitFrames_0000(1);
        for (;;) {
          if (Script_GetCharaMotionRemainingFramesScaled(arg0) >= 20.0f) {
            break;
          }
          SCNF_WaitFrames_0000(1);
        }
      }
      if ((*(&task_1AC + arg2) & 8) == 0) {
        task_1EA = 3;
      }
    }
  }
  return local_FF;
}

static void script_B74A(int32_t arg0, int32_t arg1, float arg2, float arg3, int32_t arg4, int32_t arg5) {
  Vector3 local_F4;
  float local_E8[3];
  if (arg5 & 0x2000) {
    task_1E9 = 0;
  } else {
    int32_t unused_D4[5];
    int32_t local_D0;
    local_D0 = *(&task_1B0 + arg4);
    switch (local_D0) {
      case 0:
        task_1E9 = 0;
        break;
      case 1:
        task_1E9 = 1;
        break;
      case 3:
        task_1E9 = 1;
        break;
      case 2:
        if (Script_ryoControl(PCCTL_QueryIsRunningRequest)) {
          task_1E9 = 1;
        } else {
          task_1E9 = 0;
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
  if (task_1E9) {
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

static int32_t script_B989(int32_t arg0, int32_t arg1, float arg2, float arg3, int32_t arg4, int32_t arg5, int32_t arg6) {
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
  task_1D4 = 0;
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
  if (task_1E9) {
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
      task_1D4 = -0x80000000;
      *((int8_t *)&task_1E7 + arg4) = 1;
    } else {
      *((int8_t *)&task_1E7 + arg4) = 0;
    }
    Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
    if (arg6 > 1) {
      Script_SetCharaMoveTargetEx(arg0, &local_F4, arg1, task_1D4 | 0x2CB, 0, -1.0f, 2);
      local_B8 = 2;
    } else {
      Script_SetCharaMoveTargetEx(arg0, &local_F4, arg1, task_1D4 | 0x2CB, 0, -1.0f, 2);
      local_B8 = arg6;
    }
  } else {
    local_F4.x = arg2;
    (&local_F4)->z = arg3;
    Script_EVT_GetPosition(arg0, 26, &local_DC, ENTITY_XFORM_WORLD_SPACE);
    Script_EVT_GetPosition(arg0, 33, &local_D0, ENTITY_XFORM_WORLD_SPACE);
    if (!((&local_DC)->y >= (&local_D0)->y)) {
      task_1D4 = -0x80000000;
      *((int8_t *)&task_1E7 + arg4) = 1;
    } else {
      *((int8_t *)&task_1E7 + arg4) = 0;
    }
    Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
    if (arg6 > 1) {
      Script_SetCharaMoveTargetEx(arg0, &local_F4, arg1, task_1D4 | 0x2C6, 0, -1.0f, 2);
      local_B8 = 1;
    } else {
      Script_SetCharaMoveTargetEx(arg0, &local_F4, arg1, task_1D4 | 0x2C6, 0, -1.0f, 2);
      local_B8 = arg6;
    }
  }
  while (Script_SetCharaXMPTMode(arg0, 2) != 0) {
    SCNF_WaitFrames_0000(1);
  }
  return local_B8;
}

__attribute__((scn_string_bindings(0, data_1292B)))
static int32_t script_BC18(int32_t arg0, int32_t arg1, float arg2, float arg3, int32_t arg4, int32_t arg5, int32_t arg6) {
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
  task_1D4 = 0;
  if (task_1E9) {
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
      task_1D4 = -0x80000000;
      *((int8_t *)&task_1E7 + arg4) = 1;
    } else {
      *((int8_t *)&task_1E7 + arg4) = 0;
    }
    Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
    if (arg6 > 1) {
      Script_SetCharaMoveTargetEx(arg0, &local_F4, arg1, task_1D4 | 0x2F9 | 0x30000, 0, -1.0f, 2);
      local_B8 = 0;
    } else {
      Script_SetCharaMoveTargetEx(arg0, &local_F4, arg1, task_1D4 | 0x2F9 | 0x30000, 0, -1.0f, 2);
      local_B8 = arg6;
    }
  } else {
    local_F4.x = arg2;
    (&local_F4)->z = arg3;
    Script_EVT_GetPosition(arg0, 26, &local_DC, ENTITY_XFORM_WORLD_SPACE);
    Script_EVT_GetPosition(arg0, 33, &local_D0, ENTITY_XFORM_WORLD_SPACE);
    if (!((&local_DC)->y >= (&local_D0)->y)) {
      task_1D4 = -0x80000000;
      *((int8_t *)&task_1E7 + arg4) = 1;
    } else {
      *((int8_t *)&task_1E7 + arg4) = 0;
    }
    Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
    if (arg6 > 1) {
      Script_SetCharaMoveTargetEx(arg0, &local_F4, arg1, task_1D4 | 0x2C7 | 0x90000, 0, -1.0f, 2);
      local_B8 = 0;
    } else {
      Script_SetCharaMoveTargetEx(arg0, &local_F4, arg1, task_1D4 | 0x2C7 | 0x90000, 0, -1.0f, 2);
      local_B8 = arg6;
    }
  }
  while (Script_SetCharaXMPTMode(arg0, 2) != 0) {
    SCNF_WaitFrames_0000(1);
  }
  Script_ccowUtils(arg0, 1);
  script_CBB6(arg0, arg4, arg5);
  if (*(&task_1AC + arg4) & 2) {
    if (Script_ryoControl(PCCTL_QueryShouldRun)) {
      Script_sprintf("Kaidan : ");
      Script_ryoControl(PCCTL_DisablePlayerControl);
      (void)((task_1F0 = -1) & 4);
    }
    Script_ccowUtils(arg0, 6);
  }
  return local_B8;
}

__attribute__((scn_string_bindings(1, data_12951)))
static void script_BF25(int32_t arg0, int32_t arg1, int32_t arg2) {
  int32_t local_FC;
  int32_t local_F8;
  int8_t local_F7;
  (void)((local_F7 = 0) & 4);
  arg2 = arg2 & -2;
  local_F8 = Script_ScenDataUtil((void *)task_1A4, 4, arg2 * 6 + 4);
  local_FC = Script_ScenDataUtil((void *)task_1A4, 4, (arg2 + 1) * 6 + 4);
  local_FC = local_FC | local_F8;
  if (local_FC & -0x80000000) {
    while (local_FC & -0x80000000) {
      if (local_F7 & 0x80) {
        if (task_1F4) {
        } else {
          Script_sprintf("wait ... shuki shuki shuki\012");
        }
      } else {
        if (*(&task_1AC + arg1) & 2) {
          if (Script_ryoControl(PCCTL_QueryShouldRun)) {
            Script_sprintf("Kaidan : ");
            Script_ryoControl(PCCTL_DisablePlayerControl);
            (void)((task_1F0 = -1) & 4);
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
      task_1F4 = (task_1F4 + 1) & 0xF;
      local_F8 = Script_ScenDataUtil((void *)task_1A4, 4, arg2 * 6 + 4);
      local_FC = Script_ScenDataUtil((void *)task_1A4, 4, (arg2 + 1) * 6 + 4);
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

static void script_C153(int32_t arg0) {
  if (~task_1EB) {
    (void)((task_1EB = -1) & 1);
    task_1D8 = Script_SetCameraSettings(CAM_UTILS_SET_DISTANCE_FROM_PLAYER, -1.0f);
    task_1DC = Script_SetCameraSettings(CAM_UTILS_SET_HEIGHT_ABOVE_PLAYER, -1.0f);
    task_1E0 = Script_SetCameraSettings(CAM_UTILS_SET_MAX_Y_ANGLE, -1.0f);
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

static void script_C1F2(float arg0) {
  int32_t local_FC;
  float local_F8;
  local_FC = 0;
  while (local_FC < 10) {
    int32_t local_F4;
    local_F4 = task_1EA;
    if ((local_F4 == 0) | (local_F4 == 2) | (local_F4 == 4)) {
      break;
      goto L00C225;
    }
    L00C225:;
    local_F8 = task_1E0 + (arg0 - task_1E0) * (float)local_FC / 10.0f;
    Script_SetCameraSettings(CAM_UTILS_SET_MAX_Y_ANGLE, local_F8);
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  {
    int32_t local_F4;
    local_F4 = task_1EA;
    if ((local_F4 == 0) | (local_F4 == 2) | (local_F4 == 4)) {
    } else {
      Script_SetCameraSettings(CAM_UTILS_SET_MAX_Y_ANGLE, arg0);
    }
  }
  return;
}

static void script_C281(float arg0) {
  int32_t local_FC;
  float local_F8;
  local_FC = 0;
  while (local_FC < 50) {
    int32_t local_F4;
    local_F4 = task_1EA;
    if ((local_F4 == 0) | (local_F4 == 2) | (local_F4 == 4)) {
      break;
      goto L00C2B4;
    }
    L00C2B4:;
    local_F8 = task_1D8 + (arg0 - task_1D8) * (float)local_FC / 50.0f;
    Script_SetCameraSettings(CAM_UTILS_SET_DISTANCE_FROM_PLAYER, local_F8);
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  {
    int32_t local_F4;
    local_F4 = task_1EA;
    if ((local_F4 == 0) | (local_F4 == 2) | (local_F4 == 4)) {
    } else {
      Script_SetCameraSettings(CAM_UTILS_SET_DISTANCE_FROM_PLAYER, arg0);
    }
  }
  return;
}

static void script_C310(void) {
  Script_SetCameraSettings(6, 7);
  Script_SetCameraSettings(CAM_UTILS_SET_DISTANCE_FROM_PLAYER, task_1D8);
  Script_SetCameraSettings(CAM_UTILS_SET_HEIGHT_ABOVE_PLAYER, task_1DC);
  Script_SetCameraSettings(CAM_UTILS_SET_MAX_Y_ANGLE, task_1E0);
  Script_KaidanUtils(CAM_STAIR_RESET_RETRACTION_LIMIT);
  (void)((task_1EB = 0) & 2);
  return;
}

static int32_t script_C347(int32_t arg0, int32_t arg1) {
  Vector3 local_F4;
  Vector3 local_E8;
  int8_t local_E7;
  int8_t local_E6;
  task_1BC = arg0;
  task_1EA = 1;
  local_E7 = -1;
  (void)((local_E6 = 0) & 2);
  Script_SetGameStateTo(AUTH_None);
  while (task_1BC == arg0) {
    if (Script_GetAuthorState_And_SetCamPos(&local_F4, &local_E8) != 17) {
      if (task_1EA != local_E7) {
        uint8_t unused_E4[2];
        int32_t local_E0;
        local_E0 = task_1EA;
        if (local_E0 == 1) {
          (void)((local_E6 = -1) & 2);
          script_C153(arg1);
        } else if (local_E0 == 2) {
          script_C310();
        } else if ((local_E0 == 0) | (local_E0 == 3)) {
          Script_SetCameraSettings(6, 7);
          task_1BC = 0;
        } else {
          if (local_E0 == 4) goto L00C3F4;
        }
        L00C3F4:;
        local_E7 = task_1EA;
      }
    } else {
      if (task_1EA != local_E7) {
        uint8_t unused_E4[2];
        int32_t local_E0;
        local_E0 = task_1EA;
        if ((local_E0 == 0) | (local_E0 == 3)) {
          Script_SetCameraSettings(6, 7);
          task_1BC = 0;
        } else if ((local_E0 == 1) | (local_E0 == 2)) {
          Script_SetGameStateTo(AUTH_FirstPerson);
        } else {
          if (local_E0 == 4) goto L00C454;
        }
        L00C454:;
        local_E7 = 4;
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  if (task_1EA != 0) {
    if (local_E6 & 0x40) {
      script_C310();
    }
  }
  end:;
}

static void script_C47E(int32_t arg0) {
  int8_t local_FF;
  int32_t local_F8;
  local_F8 = -1;
  local_FF = 0;
  while (local_FF < task_1E8) {
    if (arg0 == *(&task_1B4 + local_FF)) {
      local_F8 = *(&task_1B8 + local_FF);
    }
    local_FF = local_FF + 1;
  }
  (void)local_F8;
  return;
}

static void script_C4C5(int32_t arg0) {
  task_1A8 = arg0;
  return;
}

static void script_C4CF(int32_t arg0) {
  task_1C0 = arg0;
  return;
}

static void script_C4D9(void) {
  (void)task_1C0;
  return;
}

__attribute__((scn_string_bindings(0, data_1295B)))
static void script_C4E0(int32_t arg0) {
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

__attribute__((scn_string_bindings(0, data_12973)))
static void script_C5BB(int32_t arg0) {
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

__attribute__((scn_string_bindings(0, data_1298B)))
static void script_C696(int32_t arg0) {
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

__attribute__((scn_string_bindings(0, data_129A3)))
static void script_C771(int32_t arg0, int32_t arg1) {
  Vector3 local_F4;
  Vector3i local_E8;
  float local_E4;
  float local_E0;
  float local_DC;
  if (*((int8_t *)&task_1E6 + arg1) == 0) {
    *((int8_t *)&task_1E6 + arg1) = 1;
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

__attribute__((scn_string_bindings(0, data_129BB)))
static void script_C867(int32_t arg0, int32_t arg1) {
  Vector3 local_F4;
  Vector3i local_E8;
  float local_E4;
  float local_E0;
  float local_DC;
  if (*((int8_t *)&task_1E6 + arg1) == 0) {
    *((int8_t *)&task_1E6 + arg1) = 1;
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

__attribute__((scn_string_bindings(0, data_129D3)))
static int32_t script_C95D(int32_t arg0, int32_t arg1) {
  float local_FC;
  float local_F8;
  if (*((int8_t *)&task_1E5 + arg1)) {
    Script_sprintf("KAIDAN : Warning setY!\012");
  } else {
    *((int8_t *)&task_1E5 + arg1) = 1;
    if (task_1E9) {
      local_FC = local_F8 = -0.001f;
    } else {
      local_FC = local_F8 = 0.0f;
    }
    while (*((int8_t *)&task_1E5 + arg1) == 1) {
      if (task_1E9) {
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
    if (*((int8_t *)&task_1E5 + arg1) != 2) {
      for (;;) {
        if (local_FC >= 0.0f) {
          break;
        }
        if (*((int8_t *)&task_1E5 + arg1) == 2) {
          *((int8_t *)&task_1E5 + arg1) = 0;
          Script_KaidanSetY(arg0, 0.0f);
          break;
        }
        local_FC = local_FC + 0.004f;
        Script_KaidanSetY(arg0, local_FC);
        SCNF_WaitFrames_0000(1);
      }
    } else {
      *((int8_t *)&task_1E5 + arg1) = 0;
      Script_KaidanSetY(arg0, 0.0f);
    }
  }
  end:;
}

__attribute__((scn_string_bindings(0, data_129EB)))
static int32_t script_CA85(int32_t arg0, int32_t arg1) {
  float local_FC;
  float local_F8;
  if (*((int8_t *)&task_1E5 + arg1)) {
    Script_sprintf("KAIDAN : Warning setY!\012");
  } else {
    *((int8_t *)&task_1E5 + arg1) = 1;
    if (task_1E9) {
      local_FC = local_F8 = 0.16f;
    } else {
      local_FC = local_F8 = 0.08f;
    }
    while (*((int8_t *)&task_1E5 + arg1) == 1) {
      if (task_1E9) {
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
    if (*((int8_t *)&task_1E5 + arg1) != 2) {
      for (;;) {
        if (local_FC <= 0.0f) {
          break;
        }
        if (*((int8_t *)&task_1E5 + arg1) == 2) {
          *((int8_t *)&task_1E5 + arg1) = 0;
          Script_KaidanSetY(arg0, 0.0f);
          break;
        }
        local_FC = local_FC - 0.004f;
        Script_KaidanSetY(arg0, local_FC);
        SCNF_WaitFrames_0000(1);
      }
    } else {
      *((int8_t *)&task_1E5 + arg1) = 0;
      Script_KaidanSetY(arg0, 0.0f);
    }
  }
  end:;
}

__attribute__((scn_string_bindings(0, data_12A03)))
static void script_CBB6(int32_t arg0, int32_t arg1, int32_t arg2) {
  if ((*(&task_1AC + arg1) & 2) > 0) {
    if (Script_ryoControl(PCCTL_QueryShouldRun)) {
      Script_sprintf("Kaidan : ");
      Script_ryoControl(PCCTL_DisablePlayerControl);
      (void)((task_1F0 = -1) & 4);
    }
    Script_ccowUtils(arg0, 6);
  }
  if (*(&task_1AC + arg1) & 0x20) {
    Script_ccowUtils(arg0, 4);
  } else {
    if ((arg2 & 0x100) | (arg2 & 0x4000) | ((arg2 & 0x8000) > 0)) {
      Script_ccowUtils(arg0, 4);
      goto L00CC3A;
    }
  }
  L00CC3A:;
  return;
}

static void script_CC3B(int32_t arg0, int32_t arg1, int32_t arg2) {
  if (*(&task_1AC + arg1) & 0x20) {
    Script_ccowUtils(arg0, 3);
  } else if ((arg2 & 0x8000) > 0) {
  } else {
    if ((arg2 & 0x100) | ((arg2 & 0x4000) > 0)) {
      Script_ccowUtils(arg0, 3);
      goto L00CC8A;
    }
  }
  L00CC8A:;
  return;
}

static void script_CC8B(int32_t arg0, int32_t arg1, int32_t arg2) {
  Script_ccowUtils(arg0, 5);
  if (*(&task_1AC + arg1) & 0x20) {
    Script_ccowUtils(arg0, 3);
  } else {
    if ((arg2 & 0x100) | (arg2 & 0x4000) | ((arg2 & 0x8000) > 0)) {
      Script_ccowUtils(arg0, 3);
      goto L00CCD9;
    }
  }
  L00CCD9:;
  return;
}

static void script_CCDA(int32_t arg0, float arg1, float arg2, float arg3) {
  Vector3 local_F4;
  local_F4.x = arg1;
  (&local_F4)->y = arg2;
  (&local_F4)->z = arg3;
  Script_EVT_SetPosition(arg0, POS_XYZ, &local_F4);
  return;
}

static void script_CD02(int32_t arg0, float arg1, float arg2, float arg3) {
  Vector3i local_F4;
  local_F4.x = (int32_t)(arg1 / 360.0f * (float)0x10000);
  (&local_F4)->y = (int32_t)(arg2 / 360.0f * (float)0x10000);
  (&local_F4)->z = (int32_t)(arg3 / 360.0f * (float)0x10000);
  Script_EVT_SetRotation(arg0, 0x38000000, &local_F4, 0);
  return;
}

static void script_CD56(int32_t arg0, int32_t arg1, int32_t arg2) {
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

static int32_t script_CDA6(int32_t arg0) {
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

static void script_CE4D(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  local_FC = script_CDA6(arg0);
  if (arg1 > local_FC) {
    (void)0;
  } else {
    (void)-1;
  }
  return;
}

static int32_t script_CE70(int32_t arg0) {
  int32_t unused_FC;
  GameDateTime local_F4;
  int32_t local_F0;
  int32_t local_EC;
  Script_ReturnTimeDate(&local_F4);
  local_F0 = (&local_F4)->hours * 60 + (&local_F4)->minutes;
  local_EC = Script_scenUtils(SCENARIO_CMD_GET_STEP_LUT, arg0) * 6;
  return local_F0 - local_EC;
}

static void script_CEA3(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  local_FC = script_CE70(arg0);
  if (arg1 > local_FC) {
    (void)0;
  } else {
    (void)-1;
  }
  return;
}

static void script_CEC6(int32_t arg0, int32_t arg1) {
  int8_t local_F4[12];
  int32_t local_F0;
  local_F0 = script_CE70(arg0);
  local_F4[4] = local_F0 / 60;
  local_F4[5] = local_F0 % 60;
  if ((local_F4[4] < *(int8_t *)(arg1 + 4)) | ((local_F4[4] == *(int8_t *)(arg1 + 4)) & (local_F4[5] < *(int8_t *)(arg1 + 5)))) {
    (void)0;
  } else {
    (void)-1;
  }
  return;
}

static void script_CF25(int32_t arg0) {
  int32_t local_F8[2];
  local_F8[0] = arg0;
  local_F8[1] = 0;
  (void)local_F8;
  return;
}

static void script_CF3A(void) {
  Script_ryoControl(PCCTL_DisablePlayerControl);
  return;
}

static void script_CF43(void) {
  Script_ryoControl(RYO_RestoreControl);
  Script_SetGameStateTo(AUTH_None);
  return;
}

static void script_CF52(int32_t arg0) {
  Script_ccowUtils(arg0, 2);
  if (0) {
    Script_charaDisplay(arg0, CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(arg0, CDM_Show);
      goto L00CF7E;
    }
  }
  L00CF7E:;
  Script_scenToggleVisibleFor(arg0, 1);
  Script_ccowSetFlag80(arg0, 1);
  Script_motmToggleFlags(arg0, 2, 0x40);
  Script_charaToggleMotEnabled(arg0, 0);
  Script_facemotUtils(arg0, 0, 0);
  Script_FaceEx(arg0, 1, 0, 2);
  Script_FaceLookAtTargetUtils(arg0, 0);
  Script_CharHandlers(arg0, 2, 8, 16);
  Script_SetCPAMFlags(arg0, 0);
  Script_charaDisplay(arg0, CDM_Show);
  return;
}

static void script_CFDF(int32_t arg0) {
  Script_ccowUtils(arg0, 1);
  Script_ccowSetFlag80(arg0, 2);
  Script_motmToggleFlags(arg0, 1, 0x40);
  Script_charaToggleMotEnabled(arg0, 1);
  Script_FaceEx(arg0, 0, 0, 2);
  Script_FaceLookAtTargetUtils(arg0, 0);
  Script_CharHandlers(arg0, -1, 0, 0);
  Script_SetCPAMFlags(arg0, 1);
  return;
}

static void script_D02F(void) {
  Script_filmCheckOrSetTransitionUtil(80, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_D05D(int32_t arg0) {
  Script_filmCheckOrSetTransitionUtil(arg0, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_D08B(void) {
  Script_filmCheckOrSetTransitionUtil(80, 255, 255, 255, 255, 0, 255, 255, 255);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_D0BF(int32_t arg0) {
  Script_filmCheckOrSetTransitionUtil(arg0, 255, 255, 255, 255, 0, 255, 255, 255);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_D0F3(int32_t arg0) {
  Script_filmCheckOrSetTransitionUtil(arg0, 255, 255, 255, 255, 150, 68, 50, 0);
  return;
}

static void script_D111(int32_t arg0, int32_t arg1, int32_t arg2) {
  int32_t local_FC;
  Script_GetDataFromFightProgram(arg0, arg1);
  local_FC = 0;
  while (local_FC < arg2) {
    local_FC = local_FC + 1;
    Script_SetFightProgramStateBytes(arg0, local_FC, 1);
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_D14B(int32_t arg0, int32_t arg1) {
  while (arg1 > 0) {
    Script_SetFightProgramStateBytes(arg0, arg1, 1);
    arg1 = arg1 - 1;
    SCNF_WaitFrames_0000(1);
  }
  Script_GetDataFromFightProgram(arg0, 0);
  return;
}

static void script_D17F(void) {
  int8_t local_DD[35];
  int8_t local_DC;
  int8_t local_DB;
  int8_t local_DA;
  int8_t local_D9;
  int32_t local_D4;
  local_DC = 0;
  while (local_DC < 35) {
    local_DD[local_DC] = local_DC % 4;
    local_DC = local_DC + 1;
  }
  local_D9 = 0;
  for (;;) {
    local_D4 = local_D9;
    switch (local_D4) {
      case 0:
        local_DB = 0;
        local_DA = 17;
        local_D9 = 1;
        break;
      case 1:
        local_DB = 17;
        local_DA = 35;
        local_D9 = 0;
        break;
    }
    local_DC = local_DB;
    while (local_DC < local_DA) {
      Script_SetImageModuleTaskDataFlag(SHEN_NAME('FIR1'), local_DD[local_DC]);
      Script_SetImageModuleTaskDataFlag(SHEN_NAME('FIR2'), local_DD[local_DC]);
      Script_SetImageModuleTaskDataFlag(SHEN_NAME('FIR3'), local_DD[local_DC]);
      Script_SetImageModuleTaskDataFlag(SHEN_NAME('FIR4'), local_DD[local_DC]);
      local_DD[local_DC] = local_DD[local_DC] + 1;
      local_DD[local_DC] = local_DD[local_DC] % 6;
      local_DC = local_DC + 1;
    }
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_D277(int32_t arg0, int32_t arg1) {
  Script_FACE_LCG(arg0, 1, arg1, 0);
  SCNF_WaitFrames_0000(arg1 + 1);
  Script_FACE_LCG(arg0, 2, arg1, 0);
  SCNF_WaitFrames_0000(arg1);
  return;
}

__attribute__((scn_string_bindings(0, data_13FCC, 1, data_13FD1, 2, data_13FD6, 3, data_13FE1)))
static void script_D2A7(void) {
  int32_t unused_FC;
  Script_LoadSND(0, 4, "FREE", 0);
  Script_LoadSND(0, 3, "FREE", 0);
  Script_LoadSND(0, 2, "tsm005.snd", 0);
  Script_LoadSND(0, 4, "a1_chide.snd", 0);
  Script_executeAicaCommand(1184, 2, 100);
  Script_shenSeqUtils(0);
  script_D358();
  script_D635();
  return;
}

__attribute__((scn_string_bindings(0, data_13FEE, 1, data_13FF3, 2, data_13FF8, 3, data_14003)))
static void script_D305(void) {
  Script_LoadSND(0, 4, "FREE", 0);
  Script_LoadSND(0, 3, "FREE", 0);
  Script_LoadSND(0, 2, "tsm005.snd", 0);
  Script_LoadSND(0, 4, "a1_chide.snd", 0);
  Script_executeAicaCommand(1184, 2, 100);
  script_D635();
  return;
}

static void script_D358(void) {
  int32_t local_FC;
  int32_t unused_DC[8];
  Vector4 local_CC;
  int32_t unused_A0[11];
  Vector3i local_94;
  int32_t unused_8C[2];
  int32_t local_88;
  Script_lightUtils(15, 4, &data_14010);
  script_CF3A();
  script_DED1();
  Script_charaDisplay(CHARA_AKIR, CDM_Show);
  Script_charaDisplay(CHARA_TAIJ, CDM_Show);
  Script_charaDisplay(CHARA_KISY, CDM_Show);
  Script_charaDisplay(SHEN_NAME('SARA'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('SARC'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('TEGS'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('SHIN'), CDM_Hide);
  Script_SetMotnFlag(CHARA_AKIR, -1);
  Script_SetMotnFlag(CHARA_KISY, -1);
  Script_SetMotnFlag(CHARA_TAIJ, -1);
  Script_lightSetSlotEnabled(1, 0);
  Script_lightSetSlotEnabled(4, 0);
  Script_lightSetSlotEnabled(5, 0);
  Script_lightSetSlotEnabled(6, 0);
  Script_lightSetSlotEnabled(7, 1);
  Script_lightSetSlotEnabled(8, 0);
  Script_lightSetSlotEnabled(9, 0);
  Script_lightSetSlotEnabled(10, 0);
  Script_lightSetSlotEnabled(11, 0);
  Script_mapmSetRenderFlag(0, 1);
  Script_mapmSetRenderFlag(1, 1);
  Script_mapmSetRenderFlag(2, 1);
  Script_mapmSetRenderFlag(3, 0);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 10.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 10.0f);
  (void)1;
  local_FC = 0;
  while (Script_shenSeqUtils(-1)) {
    local_88 = local_FC;
    switch (local_88) {
      case 0:
        (&local_CC)->y = 0.1f;
        (&local_CC)->z = 0.1f;
        (&local_CC)->w = 0.1f;
        Script_lightSetGlobalSceneLighting(&local_CC);
        script_E041(CHARA_AKIR);
        Script_charaSetRefboardFlagTo(CHARA_AKIR, 0);
        Script_SetCharaFlags(CHARA_AKIR, 4);
        Script_SetCharaFlags(CHARA_KISY, 4);
        Script_SetCharaFlags(CHARA_TAIJ, 4);
        Script_scenToggleActiveFor(CHARA_AKIR, 0);
        Script_scenToggleActiveFor(CHARA_KISY, 0);
        Script_scenToggleActiveFor(CHARA_TAIJ, 0);
        Script_mapmSetRenderFlag(0, 0);
        Script_mapmSetRenderFlag(1, 1);
        Script_mapmSetRenderFlag(2, 0);
        Script_mapmSetRenderFlag(3, 0);
        break;
      case 65:
        Script_SetCharaRefBoardPos(CHARA_AKIR, &data_14044);
        Script_charaSetRefboardFlagTo(CHARA_AKIR, 1);
        (&local_CC)->y = 0.3f;
        (&local_CC)->z = 0.3f;
        (&local_CC)->w = 0.3f;
        Script_lightSetGlobalSceneLighting(&local_CC);
        Script_lightSetSlotEnabled(7, 1);
        Script_lightSetSlotEnabled(8, 1);
        Script_scenToggleActiveFor(CHARA_AKIR, 1);
        Script_mapmSetRenderFlag(0, 0);
        Script_mapmSetRenderFlag(1, 1);
        Script_mapmSetRenderFlag(2, 0);
        Script_mapmSetRenderFlag(3, 0);
        break;
      case 90:
        local_94.x = 364;
        (&local_94)->y = 9830;
        (&local_94)->z = -4187;
        Script_SetHandRootRotation(CHARA_AKIR, 0, 0x15, &local_94);
        Script_charaDisplay(SHEN_NAME('SARA'), CDM_Hide);
        Script_charaDisplay(SHEN_NAME('SARC'), CDM_Show);
        Script_scenCreateTask(script_DDAD, 0);
        break;
    }
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_D635(void) {
  int32_t local_FC;
  int32_t unused_F4[2];
  Vector3 local_E8;
  int32_t unused_DC[3];
  Vector4 local_CC;
  int32_t unused_B0[7];
  Vector4 local_A0;
  Vector3i local_94;
  int32_t unused_8C[2];
  int32_t local_88;
  Script_lightUtils(15, 4, &data_14054);
  script_CF3A();
  script_DED1();
  Script_charaDisplay(CHARA_AKIR, CDM_Show);
  Script_charaDisplay(CHARA_TAIJ, CDM_Show);
  Script_charaDisplay(CHARA_KISY, CDM_Show);
  Script_charaDisplay(SHEN_NAME('SARA'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('TEGS'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('SHIN'), CDM_Hide);
  Script_SetMotnFlag(CHARA_AKIR, -1);
  Script_SetMotnFlag(CHARA_KISY, -1);
  Script_SetMotnFlag(CHARA_TAIJ, -1);
  Script_lightSetSlotEnabled(1, 0);
  Script_lightSetSlotEnabled(4, 0);
  Script_lightSetSlotEnabled(5, 0);
  Script_lightSetSlotEnabled(6, 0);
  Script_lightSetSlotEnabled(7, 1);
  Script_lightSetSlotEnabled(8, 0);
  Script_lightSetSlotEnabled(9, 0);
  Script_lightSetSlotEnabled(10, 0);
  Script_lightSetSlotEnabled(11, 0);
  Script_mapmSetRenderFlag(0, 1);
  Script_mapmSetRenderFlag(1, 1);
  Script_mapmSetRenderFlag(2, 1);
  Script_mapmSetRenderFlag(3, 0);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 10.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 10.0f);
  local_94.x = 0;
  (&local_94)->y = 0;
  (&local_94)->z = 0;
  Script_SetHandRootRotation(CHARA_AKIR, 0, 0x15, &local_94);
  Script_executeAicaCommand(4520, 0, 0);
  Script_executeAicaCommand(4520, 0, 0);
  Script_executeAicaCommand(1184, 2, 107);
  (&local_A0)->y = 0.12f;
  (&local_A0)->z = 0.13f;
  (&local_A0)->w = 0.15f;
  Script_SetVector4ByOffset(&local_A0);
  (void)1;
  Script_scenToggleActiveFor(CHARA_KISY, 1);
  script_E041(CHARA_AKIR);
  Script_SetCharaFlags(CHARA_AKIR, 4);
  Script_SetCharaFlags(CHARA_KISY, 4);
  Script_SetCharaFlags(CHARA_TAIJ, 4);
  Script_scenToggleActiveFor(CHARA_TAIJ, 0);
  Script_SetVector4ByOffset(&local_A0);
  Script_lightSetSlotEnabled(7, 1);
  Script_lightSetSlotEnabled(8, 1);
  Script_scenToggleActiveFor(CHARA_AKIR, 1);
  Script_mapmSetRenderFlag(0, 0);
  Script_mapmSetRenderFlag(1, 1);
  Script_mapmSetRenderFlag(2, 0);
  Script_mapmSetRenderFlag(3, 0);
  local_94.x = 0;
  (&local_94)->y = 0;
  (&local_94)->z = 0;
  Script_SetHandRootRotation(CHARA_AKIR, 0, 0x15, &local_94);
  Script_executeAicaCommand(4520, 0, 0);
  Script_executeAicaCommand(4520, 0, 0);
  Script_executeAicaCommand(1184, 2, 107);
  (&local_A0)->y = 0.3f;
  (&local_A0)->z = 0.3f;
  (&local_A0)->w = 0.3f;
  Script_SetVector4ByOffset(&local_A0);
  (void)1;
  Script_scenToggleActiveFor(CHARA_KISY, 1);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 16.5f);
  script_E041(CHARA_KISY);
  script_E07C(CHARA_AKIR);
  Script_FACE_LCG(CHARA_KISY, 2, 8, 0);
  script_E041(CHARA_AKIR);
  (&local_CC)->y = 0.18f;
  (&local_CC)->z = 0.18f;
  (&local_CC)->w = 0.18f;
  Script_lightSetGlobalSceneLighting(&local_CC);
  Script_charaSetRefboardFlagTo(CHARA_KISY, 0);
  if (Script_shenSeqUtils(-7) == 0) {
    Script_shenSeqUtils(7);
    local_FC = -115;
    while (Script_shenSeqUtils(-1)) {
      local_88 = local_FC;
      if (local_88 == 65) {
        (&local_CC)->y = 0.3f;
        (&local_CC)->z = 0.3f;
        (&local_CC)->w = 0.3f;
        Script_lightSetGlobalSceneLighting(&local_CC);
        Script_SetCharaRefBoardPos(CHARA_KISY, &local_A0);
        Script_charaSetRefboardFlagTo(CHARA_KISY, 1);
      } else if (local_88 == 400) {
        Script_handSetFloatsFor(CHARA_KISY, 0, data_13D20, 20);
        Script_handSetFloatsFor(CHARA_KISY, 1, data_13E04, 20);
        Script_FACE_LCG(CHARA_KISY, 0, 6, 0);
        Script_handSetFloatsFor(CHARA_AKIR, 1, data_12AF4, 20);
        Script_handSetFloatsFor(CHARA_AKIR, 0, data_12AF4, 20);
      } else if (local_88 == 385) {
        Script_InitializeOsag(CHARA_AKIR);
      } else if (local_88 == 565) {
        script_E07C(CHARA_AKIR);
        script_E07C(CHARA_TAIJ);
        script_E07C(CHARA_KISY);
        Script_charaDisplay(SHEN_NAME('SHIN'), CDM_Show);
        Script_scenToggleActiveFor(CHARA_TAIJ, 1);
        Script_lightSetSlotEnabled(15, 1);
        Script_lightSetSlotEnabled(10, 1);
        Script_SetShadowFlags(CHARA_TAIJ, SCRIPT_SHADOW_UPDATE_LIGHT_MASK, 10);
      } else if (local_88 == 710) {
        (void)1;
        script_E041(CHARA_TAIJ);
        Script_charaDisplay(SHEN_NAME('SHIN'), CDM_Hide);
        Script_lightSetSlotEnabled(15, 0);
        Script_SetShadowFlags(CHARA_TAIJ, SCRIPT_SHADOW_UPDATE_LIGHT_MASK_0, 10);
        Script_SetShadowFlags(CHARA_TAIJ, SCRIPT_SHADOW_UPDATE_LIGHT_MASK, 8);
      } else if (local_88 == 900) {
        Script_CharHandlers(CHARA_KISY, 2, 8, 16);
      } else if (local_88 == 965) {
        Script_scenToggleActiveFor(CHARA_AKIR, 0);
        Script_scenToggleActiveFor(CHARA_KISY, 0);
        Script_scenToggleActiveFor(CHARA_TAIJ, 0);
      } else if (local_88 == 1260) {
        script_E041(CHARA_AKIR);
      } else {
        switch (local_88) {
          case 1260:
            Script_scenToggleActiveFor(CHARA_AKIR, 1);
            Script_scenToggleActiveFor(CHARA_KISY, 1);
            Script_scenToggleActiveFor(CHARA_TAIJ, 1);
            break;
          case 1500:
            Script_scenToggleActiveFor(CHARA_AKIR, 0);
            break;
          case 1537:
            Script_scenToggleActiveFor(CHARA_TAIJ, 0);
            local_94.x = 0;
            (&local_94)->y = -2729;
            (&local_94)->z = -2183;
            Script_SetHandRootRotation(CHARA_AKIR, 1, 0x15, &local_94);
            Script_handSetFloatsFor(CHARA_AKIR, 1, data_13EE8, 1);
            Script_charaDisplay(SHEN_NAME('TEGS'), CDM_Show);
            local_E8.x = 0.2f;
            (&local_E8)->y = -0.018f;
            (&local_E8)->z = 0.0f;
            local_94.x = 64625;
            (&local_94)->y = 15473;
            (&local_94)->z = 0xF000;
            Script_fixoAttachEx(SHEN_NAME('TEGS'), CHARA_AKIR, 18, &local_E8, &local_94);
            break;
          case 1640:
            Script_scenToggleActiveFor(CHARA_TAIJ, 1);
            Script_charaDisplay(SHEN_NAME('TEGS'), CDM_Hide);
            Script_fixoClearAttachment(SHEN_NAME('TEGS'));
            break;
          case 1671:
            Script_scenToggleActiveFor(CHARA_AKIR, 1);
            break;
        }
      }
      Script_InitializeOsag(CHARA_KISY);
      local_FC = local_FC + 1;
      SCNF_WaitFrames_0000(1);
    }
    script_DFC3();
    if (Script_shenSeqUtils(-7) == 0) {
      script_E0A6();
      if (Script_shenSeqUtils(-7) == 0) {
        script_DED1();
        Script_scenToggleActiveFor(CHARA_AKIR, 1);
        Script_scenToggleActiveFor(CHARA_KISY, 1);
        Script_scenToggleActiveFor(CHARA_TAIJ, 1);
        Script_shenSeqUtils(4);
        local_FC = 0;
        Script_lightSetSlotEnabled(5, 1);
        while (Script_shenSeqUtils(-1)) {
          if (local_FC == 1250) {
            Script_lightSetSlotEnabled(5, 0);
          }
          if (local_FC == 1800) {
            break;
          }
          local_FC = local_FC + 1;
          SCNF_WaitFrames_0000(1);
        }
      }
    }
  }
  Script_executeAicaCommand(2720, 2, 40);
  Script_filmCheckOrSetTransitionUtil(20, 0, 0, 0, 0, 255, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  script_DFC3();
  Script_SetMotnFlag(CHARA_AKIR, 0);
  Script_SetMotnFlag(CHARA_KISY, 0);
  Script_SetMotnFlag(CHARA_TAIJ, 0);
  Script_shenSeqUtils(-4);
  SCNF_WaitFrames_0000(30);
  return;
}

static int32_t script_DDAD(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t local_E4;
  int32_t local_E0;
  Script_charaDisplay(SHEN_NAME('SARA'), CDM_Hide);
  local_F4.x = -17.6103f;
  (&local_F4)->y = 0.0036f;
  (&local_F4)->z = -11.662f;
  Script_EVT_SetPosition(SHEN_NAME('SARC'), POS_XYZ, &local_F4);
  local_E8.x = 1704;
  (&local_E8)->y = 2178;
  (&local_E8)->z = 757;
  Script_EVT_SetRotation(SHEN_NAME('SARC'), 0x38000000, &local_E8, 0);
  local_E4 = 0;
  local_E0 = 2;
  while (local_E0 < 66) {
    while (local_E4 < 13) {
      Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
      Script_InterpolateAndScaleAngles(local_E4, local_E0, &local_F4, &local_E8, "DAT1");
      Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
      local_F4.x = (&local_F4)->x * (float)10;
      (&local_F4)->y = (&local_F4)->y * (float)10;
      (&local_F4)->z = (&local_F4)->z * (float)10;
      Script_charaPosRotUtils(SHEN_NAME('SARC'), Script_ScenDataUtil(&data_14088, 4, local_E4), &local_F4, &local_E8, 0);
      local_E4 = local_E4 + 1;
    }
    local_E4 = 0;
    local_E0 = local_E0 + 2;
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_DED1(void) {
  Script_SetVector4ByOffset(&data_140DC);
  (void)1;
  script_CF52(CHARA_AKIR);
  script_CF52(CHARA_KISY);
  script_CF52(CHARA_TAIJ);
  Script_facemotUtils(CHARA_AKIR, 1, 0);
  Script_facemotUtils(CHARA_KISY, 1, 0);
  Script_facemotUtils(CHARA_TAIJ, 1, 0);
  Script_SetMaxInterpSpeed(0.4f);
  Script_SetMaxNPCsNear(32);
  Script_EVT_SetPosition(SHEN_NAME('SHIN'), 0x78000000, &data_140C4);
  Script_EVT_SetRotation(SHEN_NAME('SHIN'), 0x78000000, &data_140D0, 0);
  Script_SetShadowFlags(CHARA_AKIR, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_SetShadowFlags(CHARA_KISY, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_SetShadowFlags(CHARA_TAIJ, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_SetShadowFlags(CHARA_AKIR, SCRIPT_SHADOW_UPDATE_LIGHT_MASK, 8);
  Script_SetShadowFlags(CHARA_KISY, SCRIPT_SHADOW_UPDATE_LIGHT_MASK, 8);
  Script_SetShadowFlags(CHARA_TAIJ, SCRIPT_SHADOW_UPDATE_LIGHT_MASK, 8);
  Script_SetShadowRange(60.0f);
  return;
}

static void script_DFC3(void) {
  script_CFDF(CHARA_AKIR);
  script_CFDF(CHARA_KISY);
  script_CFDF(CHARA_TAIJ);
  script_E07C(CHARA_AKIR);
  script_E07C(CHARA_KISY);
  script_E07C(CHARA_TAIJ);
  Script_FaceEx(CHARA_AKIR, 0, 0, 1);
  Script_FaceEx(CHARA_KISY, 0, 0, 1);
  Script_FaceEx(CHARA_TAIJ, 0, 0, 1);
  Script_SetGameStateTo(AUTH_None);
  return;
}

static void script_E041(int32_t arg0) {
  Script_ResetHandMotion(arg0, 1, 1, 0);
  Script_ResetHandMotion(arg0, 1, 0, 0);
  Script_handSetFloatsFor(arg0, 1, data_1304C, 1);
  Script_handSetFloatsFor(arg0, 0, data_1304C, 1);
  return;
}

static void script_E07C(int32_t arg0) {
  Script_ResetHandMotion(arg0, 0, 1, 0);
  Script_ResetHandMotion(arg0, 0, 0, 0);
  Script_CharHandlers(arg0, 2, 8, 16);
  return;
}

static void script_E0A6(void) {
  int32_t unused_F8[2];
  script_CF3A();
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 25.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 0.0f);
  script_E0CB();
  script_E19A();
  return;
}

static void script_E0CB(void) {
  int32_t unused_F4[3];
  int16_t local_F2;
  uint8_t unused_F0[2];
  (void)1;
  Script_SetVector4ByOffset(&data_140EC);
  local_F2 = 0;
  script_E137();
  Script_shenSeqUtils(1);
  while (Script_shenSeqUtils(-1)) {
    if (local_F2 == 800) {
      Script_scenToggleActiveFor(CHARA_AKIR, 0);
      Script_scenToggleActiveFor(CHARA_KISY, 0);
      Script_scenToggleActiveFor(CHARA_TAIJ, 0);
    }
    local_F2 = local_F2 + 1;
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_E137(void) {
  script_CF52(CHARA_AKIR);
  script_CF52(CHARA_KISY);
  script_CF52(CHARA_TAIJ);
  Script_facemotUtils(CHARA_AKIR, 1, 0);
  Script_facemotUtils(CHARA_KISY, 1, 0);
  Script_facemotUtils(CHARA_TAIJ, 1, 0);
  Script_SetMaxInterpSpeed(0.4f);
  Script_SetShadowFlags(0, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  return;
}

static void script_E19A(void) {
  script_CFDF(CHARA_AKIR);
  script_CFDF(CHARA_KISY);
  script_CFDF(CHARA_TAIJ);
  return;
}

static void script_E1C1(void) {
  int32_t local_FC;
  local_FC = Script_GetSceneID();
  switch (local_FC) {
    case 1:
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_ENTRY2_TIMED_EVENT_DONE) == 0) {
        Script_TimedTransitionUtils(TIMED_TRANSITION_REGISTER, 1, 0x3044484A, 2, &data_1416C, 0xD5A6C3, TimedTransitionUpdateMemoItem150);
      }
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JD00_ENTRY20_TIMED_EVENT_DONE) == 0) {
        Script_TimedTransitionUtils(TIMED_TRANSITION_REGISTER, 1, 0x3030444A, 20, &data_14134, 0xDA3AA3, 0);
      }
      break;
    case 2:
      if ((Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_GMCT_ROUTE_STARTED) == 0) | (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_GMCT_ROUTE_ADVANCED) == 0)) {
        Script_TimedTransitionUtils(TIMED_TRANSITION_REGISTER, 2, 0x54434D47, 1, &data_140FC, 0xB7420F, 0);
      }
      break;
    case 3:
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOZOMI_RESCUE_RUSH_ACTIVE) == 0) {
        Script_TimedTransitionUtils(TIMED_TRANSITION_REGISTER, 3, 0x4F4D4F4A, 20, &data_14118, 0x2DC9FE, TimedTransitionRollbackToPreviousDay2330);
      }
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_FIRED_FROM_JOB) & ~Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 970)) {
        if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_MFSY_ENTRY42_TIMED_EVENT_DONE) == 0) {
          Script_TimedTransitionUtils(TIMED_TRANSITION_REGISTER, 3, 0x5953464D, 42, &data_14150, 0xBBB817, 0);
        }
      }
      break;
  }
  return;
}

static void script_E2F9(int32_t arg0) {
  script_E311(arg0);
  return;
}

static void script_E305(int32_t arg0) {
  script_E311(arg0);
  return;
}

static void script_E311(int32_t arg0) {
  if (Script_StepUtils(GET_STEP_HIGHER, 0) == 22) {
    int32_t local_FC;
    local_FC = Script_StepUtils(GET_STEP_LOWER, 0);
    if ((local_FC == 2) | (local_FC == 3)) {
      Script_SetShouldSleepFlag(0);
      goto L00E34A;
    }
  }
  L00E34A:;
  Script_ryoSetChara(arg0);
  Script_facemotUtils(arg0, 1, 0);
  Script_facemotUtils(arg0, 0, 0);
  Script_FaceEx(arg0, 0, 0, 2);
  Script_CharHandlers(arg0, -1, 0, 0);
  script_E1C1();
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

static void script_E40D(int32_t arg0) {
  script_E305(arg0);
  for (;;) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_E424(int32_t arg0) {
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

static void script_E465(int32_t arg0) {
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

static void script_E4A9(int32_t arg0) {
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

static void script_E527(void) {
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_PROGRESS_STEP_28_REACHED)) {
    script_E4A9(40);
  } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_PROGRESS_STEP_1E_REACHED)) {
    script_E4A9(30);
  } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_PROGRESS_STEP_14_REACHED)) {
    script_E4A9(20);
  } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JU00_CATA1_NBO_BRANCH_CHOSEN)) {
    script_E4A9(10);
  } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_PROGRESS_STEP05_SA1116_DOJO_DONE)) {
    script_E4A9(5);
  } else {
    script_E4A9(0);
  }
  return;
}

__attribute__((scn_string_bindings(0, data_141A4, 1, data_141B2, 2, data_141D0, 3, data_141DD, 4, data_141E2, 5, data_141EF, 6, data_141F4, 7, data_14201, 8, data_14206, 9, data_14213, 10, data_14218, 11, data_1421E)))
void script_E5A6(void) {
  task_238 = Script_ryoSleepUtils(0);
  Script_filmCheckOrSetTransitionUtil(0, 255, 0, 0, 0, 0, 0, 0, 0);
  Script_SetMapFlags(0, 0);
  Script_scenCreateTask(script_1082D, 0);
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 10, -1);
  script_10856("SCENE/02/MS08", "MS08");
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 10, 0);
  Script_EVT_SetPosition(SHEN_NAME('SARA'), POS_XYZ, &data_141B8);
  Script_EVT_SetRotation(SHEN_NAME('SARA'), 0x38000000, &data_141C4, 0);
  Script_uiUtils(0, 0, 0);
  Script_PlayerStatsUtils(PS_IncrementSpotVisitCount, 76);
  script_107AF();
  task_234 = 0;
  task_230 = 0;
  script_10890();
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
  Script_AuthorUtils(0, 0, "SEQDATA0.BIN", "MS08");
  Script_AuthorUtils(0, 1, "SEQDATA1.BIN", "MS08");
  Script_AuthorUtils(0, 4, "SEQDATA3.BIN", "MS08");
  Script_AuthorUtils(0, 7, "SEQDATA6.BIN", "MS08");
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
  Script_LoadVoiceFile(1, "A0154");
  Script_LoadVoiceFile(1, "E1010");
  Script_mapmSetRenderFlag(0, 0);
  Script_mapmSetRenderFlag(1, 0);
  Script_scenCreateTask(script_FE4C, 1, CHARA_AKIR);
  task_234 = Script_scenCreateTask(script_FCDF, 1, CHARA_AKIR);
  task_230 = Script_scenCreateTask(script_102EA, 0);
  Script_scenCreateTask(script_106DC, 0);
  Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_MKSG_MS08_TRANSITION_TRIGGERED, 1);
  Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_CHEN_RESIDENCE_CALLED, 0);
  script_E7B6();
  script_E791();
  if (Script_GetOwnedItemCount(51, 0) == 0) {
    Script_GetCassetteFlags(51, 1, 0, 0);
  }
  Script_SetOwnedItemMenuFlagIfOwned(1, OwnedItem_UIITEMFLAS, 1);
  Script_SetOwnedItemMenuFlagIfOwned(1, OwnedItem_UIITEMCBAT, 1);
  if (task_238) {
    Script_ryoSleepUtils(1);
  }
  Script_ryoSleepUtils(2, 1);
  return;
}

static void script_E791(void) {
  Script_SetVector4ByOffset(&data_14224);
  Script_lightSetSlotEnabled(5, 0);
  Script_lightSetSlotEnabled(7, 0);
  Script_lightSetSlotEnabled(8, 0);
  return;
}

static void script_E7B6(void) {
  int8_t local_FF;
  Script_InvestigationUtils(ShenmueUtils_SetupOrSyncAnchors, SHEN_NAME('ARML'), 0x524D5241);
  Script_srchUtils(SRCH_SCRIPT_InitEntryTableFromTaskOffset, &data_1363C);
  Script_srchSetDistanceTarget(1.5f);
  Script_SetMaxCharaSearchExtent(0, 2.0f);
  Script_scenCreateTask(script_9EB2, 6, CHARA_AKIR, 2, 0, &data_135A4, 2, 0);
  script_C4C5((int32_t)data_13608);
  local_FF = 0;
  for (;;) {
    if (local_FF == 24) {
      break;
    }
    *((int8_t *)task_1FC + local_FF) = 0;
    local_FF = local_FF + 1;
  }
  Script_hmdlApplyScale(SHEN_NAME('OJD1'), &data_14234);
  Script_hmdlApplyScale(SHEN_NAME('OJL3'), &data_14240);
  return;
}

static void script_E859(int32_t arg0) {
  Script_lightResetSlotToDefaults(arg0);
  Script_LightUtils(arg0, 0, 4);
  Script_lightSpotLightEnable(arg0, &data_1424C, &data_14258);
  Script_LightUtils(arg0, 2, &data_14264);
  Script_LightUtils(arg0, 4, 1.0f);
  Script_LightUtils(arg0, 5, 7.0f);
  Script_LightUtils(arg0, 6, data_14274);
  Script_LightUtils(arg0, 10, 2730);
  Script_lightSetSlotEnabled(arg0, 1);
  return;
}

static void script_E8C3(int32_t arg0) {
  Vector3 local_F4;
  Vector3 local_E8;
  Vector3 local_DC;
  int32_t unused_D8;
  int32_t local_D0[2];
  Script_GetAuthorState_And_SetCamPos(&local_F4, &local_E8);
  local_D0[1] = Script_FloatUtils(ScriptMathOp_Atan2ReverseVector, (&local_F4)->x - (&local_E8)->x, (&local_F4)->z - (&local_E8)->z);
  local_D0[0] = Script_FloatUtils(ScriptMathOp_Atan2ReverseVector, Script_MathUtils(SCRIPT_MATH_VEC2_LENGTH, (&local_F4)->x - (&local_E8)->x, (&local_F4)->z - (&local_E8)->z), (&local_F4)->y - (&local_E8)->y);
  local_DC.x = (float)0 - Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, local_D0[1]);
  (&local_DC)->y = (float)0 - Script_MathUtils(SCRIPT_MATH_COS_FIXED16, *(int32_t *)local_D0);
  (&local_DC)->z = (float)0 - Script_MathUtils(SCRIPT_MATH_COS_FIXED16, local_D0[1]);
  Script_LightUtils(14, 7, &local_F4);
  Script_LightUtils(14, 8, &local_DC);
  return;
}

__attribute__((scn_string_bindings(2, data_14290)))
static void script_E97A(int32_t arg0, int32_t arg1) {
  (void)((task_23E = 0) & 1);
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_uiUtils(0, 1, 0);
  {
    int32_t local_FC;
    local_FC = arg1;
    if ((local_FC == 0x32425554) | (local_FC == 0x35425554) | (local_FC == 0x36425554)) {
      Script_InvestigationUtils(ShenmueUtils_QueueSndCommand, 525737, 0);
    } else if ((local_FC == 0x3142414B) | (local_FC == 0x3242414B)) {
      Script_InvestigationUtils(ShenmueUtils_QueueSndCommand, 132521, 0);
    } else {
      if (local_FC == 0x4B42494B) {
        Script_InvestigationUtils(ShenmueUtils_QueueSndCommand, 263593, 0);
        goto L00EA0F;
      }
    }
  }
  L00EA0F:;
  if (Script_InvestigationUtils(ShenmueUtils_BeginTargetModeDefault, arg1) == 0) {
    Script_PlayCharaMotion(arg0, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    for (;;) {
      SCNF_WaitFrames_0000(1);
      if (Script_InvestigationUtils(ShenmueUtils_UpdateStateMachine) != 0) {
        break;
      }
    }
    {
      int32_t local_FC;
      local_FC = arg1;
      switch (local_FC) {
        case 0x32425554:
          if (*((int8_t *)task_1FC + 3) == 0) {
            Script_ShowUIMessageFromID("E1010A008");
            *((int8_t *)task_1FC + 3) = 1;
          } else {
            script_EFE8(0);
          }
          break;
        case 0x35425554:
          if (*((int8_t *)task_1FC + 6) == 0) {
            Script_ShowUIMessageFromID("E1010A016");
            *((int8_t *)task_1FC + 6) = 1;
          } else {
            script_EFE8(1);
          }
          break;
        case 0x36425554:
          Script_ShowUIMessageFromID("E1010A087");
          break;
        case 0x3142414B:
          if (*((int8_t *)task_1FC + 1) == 0) {
            Script_ShowUIMessageFromID("E1010A033");
            *((int8_t *)task_1FC + 1) = 1;
          } else {
            script_EFE8(0);
          }
          break;
        case 0x3242414B:
          if (*((int8_t *)task_1FC + 7) == 0) {
            Script_ShowUIMessageFromID("E1010A084");
            *((int8_t *)task_1FC + 7) = 1;
          } else {
            script_EFE8(1);
          }
          break;
        case 0x4B42494B:
          Script_ShowUIMessageFromID("E1010A011");
          break;
      }
    }
    for (;;) {
      SCNF_WaitFrames_0000(1);
      Script_InvestigationUtils(ShenmueUtils_UpdateInterpolatedSpeed);
      if ((Script_GetControllerInput(1) == 0x400) | (Script_GetControllerInput(1) == 0x200) | (task_23F == -1)) {
        break;
      }
    }
    {
      int32_t local_FC;
      local_FC = arg1;
      if ((local_FC == 0x32425554) | (local_FC == 0x35425554) | (local_FC == 0x36425554)) {
        Script_InvestigationUtils(ShenmueUtils_QueueSndCommand, 0, 591273);
      } else if ((local_FC == 0x3142414B) | (local_FC == 0x3242414B)) {
        Script_InvestigationUtils(ShenmueUtils_QueueSndCommand, 0, 198057);
      } else {
        if (local_FC == 0x4B42494B) {
          Script_InvestigationUtils(ShenmueUtils_QueueSndCommand, 0, 329129);
          goto L00EC31;
        }
      }
    }
    L00EC31:;
    Script_InvestigationUtils(ShenmueUtils_RequestTransitionVariantA);
    for (;;) {
      SCNF_WaitFrames_0000(1);
      if (Script_InvestigationUtils(ShenmueUtils_UpdateStateMachine) != 0) {
        break;
      }
    }
  }
  Script_uiUtils(0, 1, 0);
  (void)((task_23E = -1) & 2);
  Script_ryoControl(RYO_RestoreControl);
  return;
}

static void script_EC71(int32_t arg0) {
  Vector3 local_F4;
  float local_E8[3];
  Vector3 local_DC;
  Vector3i local_D0;
  int32_t local_C4[3];
  float local_C0;
  Vector3i local_B4;
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_SetGameStateTo(AUTH_EventCam);
  Script_seqEnterEvtCam(1000, 0, AUTH_None);
  local_F4.x = -24.08f;
  (&local_F4)->y = 0.0f;
  (&local_F4)->z = -13.38f;
  Script_EVT_SetPosition(arg0, POS_XYZ, &local_F4);
  (&local_B4)->y = 0x4000;
  Script_EVT_SetRotation(arg0, MATRIX_XFORM_Y, &local_B4, 0);
  local_D0.x = 0;
  (&local_D0)->y = 0;
  (&local_D0)->z = 0;
  local_E8[0] = -24.4f;
  local_E8[1] = 1.0f;
  local_E8[2] = -13.84f;
  local_C4[0] = 1365;
  local_C4[1] = 0;
  local_C4[2] = 0;
  Script_PlayCharaMotion(arg0, 4373, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  SCNF_WaitFrames_0000(1);
  for (;;) {
    local_C0 = Script_GetCharaMotionRemainingFramesScaled(arg0);
    if (!(local_C0 < 52.0f) & !(local_C0 >= 90.0f)) {
      local_D0.x = (&local_D0)->x - 455;
      local_C4[0] = *(int32_t *)local_C4 + 455;
      local_DC.x = *local_E8;
      (&local_DC)->y = local_E8[1] + Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, *(int32_t *)local_C4) * 0.8f;
      (&local_DC)->z = local_E8[2] + Script_MathUtils(SCRIPT_MATH_COS_FIXED16, *(int32_t *)local_C4) * 0.8f;
      Script_SetMotnFlags(arg0, 17, &local_DC, 0x38000000);
    }
    if (!(local_C0 <= 90.0f)) {
      local_D0.x = 48241;
    }
    if (Script_IsCharaMotionFinished(arg0)) {
      break;
    }
    Script_charaPosRotUtils(SHEN_NAME('KBK2'), 13, 0, &local_D0, 0);
    SCNF_WaitFrames_0000(1);
  }
  Script_seqEnterEvtCam(1001, 0, AUTH_None);
  Script_SetCharaLookAtPosition(arg0, 1, &data_142B8, 0);
  SCNF_WaitFrames_0000(30);
  Script_ShowUIMessageFromID("E1010A074");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_ShowUIMessageFromID("E1010A075");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_SetCharaLookAtPosition(arg0, -1, 0, 0);
  Script_seqEnterEvtCam(1002, 0, AUTH_None);
  local_D0.x = 48241;
  (&local_D0)->y = 0;
  (&local_D0)->z = 0;
  local_C4[0] = *(int32_t *)local_C4 - 455;
  Script_PlayCharaMotion(arg0, 4373, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_motmToggleFlags(arg0, 1, 0x20);
  SCNF_WaitFrames_0000(1);
  for (;;) {
    local_C0 = Script_GetCharaMotionRemainingFramesScaled(arg0);
    if (!(local_C0 < 52.0f) & !(local_C0 >= 90.0f)) {
      local_D0.x = (&local_D0)->x + 455;
      local_C4[0] = *(int32_t *)local_C4 - 455;
      local_DC.x = *local_E8;
      (&local_DC)->y = local_E8[1] + Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, *(int32_t *)local_C4) * 0.8f;
      (&local_DC)->z = local_E8[2] + Script_MathUtils(SCRIPT_MATH_COS_FIXED16, *(int32_t *)local_C4) * 0.8f;
      Script_SetMotnFlags(arg0, 17, &local_DC, 0x38000000);
    }
    if (((scn3_bits){.f = local_C0}).i == 0x42540000) {
      local_D0.x = 0;
    }
    if (Script_IsCharaMotionFinished(arg0)) {
      break;
    }
    Script_charaPosRotUtils(SHEN_NAME('KBK2'), 13, 0, &local_D0, 0);
    SCNF_WaitFrames_0000(1);
  }
  Script_motmToggleFlags(arg0, 2, 0x20);
  Script_srchUtils(SRCH_SCRIPT_Reset);
  Script_PlayCharaMotion(arg0, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_ryoControl(RYO_RestoreControl);
  SCNF_WaitFrames_0000(2);
  Script_convSetCamInactiveFlag(1);
  Script_SetGameStateTo(AUTH_None);
  return;
}

__attribute__((scn_string_bindings(0, data_142D8, 1, data_142E2, 2, data_142EC, 3, data_142F6, 4, data_14300, 5, data_1430A, 6, data_14314, 7, data_1431E, 9, data_14332, 10, data_1433C, 11, data_14346, 12, data_14350)))
static void script_EFE8(int32_t arg0) {
  int32_t local_FC;
  local_FC = arg0;
  switch (local_FC) {
    case 0:
      local_FC = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 4.0f));
      if (local_FC == 0) {
        Script_ShowUIMessageFromID("E1010A076");
      } else if (local_FC == 1) {
        Script_ShowUIMessageFromID("E1010A039");
      } else if (local_FC == 2) {
        Script_ShowUIMessageFromID("E1010A077");
      } else {
        Script_ShowUIMessageFromID("E1010A078");
      }
      break;
    case 1:
      local_FC = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 5.0f));
      if (local_FC == 0) {
        Script_ShowUIMessageFromID("E1010A076");
      } else if (local_FC == 1) {
        Script_ShowUIMessageFromID("E1010A039");
      } else if (local_FC == 2) {
        Script_ShowUIMessageFromID("E1010A077");
      } else if (local_FC == 3) {
        Script_ShowUIMessageFromID("E1010A078");
      } else {
        Script_ShowUIMessageFromID("E1010A081");
      }
      break;
    case 2:
      local_FC = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 5.0f));
      if (local_FC == 0) {
        Script_ShowUIMessageFromID("E1010A076");
      } else if (local_FC == 1) {
        Script_ShowUIMessageFromID("E1010A039");
      } else if (local_FC == 2) {
        Script_ShowUIMessageFromID("E1010A077");
      } else if (local_FC == 3) {
        Script_ShowUIMessageFromID("E1010A078");
      } else {
        Script_ShowUIMessageFromID("E1010A083");
      }
      break;
  }
  return;
}

__attribute__((scn_string_bindings(3, data_14382, 8, data_143B4, 11, data_143D2, 15, data_143FA, 16, data_14404, 18, data_14418, 26, data_14468)))
static void script_F15C(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  local_FC = arg1;
  if (local_FC == 0x31425554) {
    if (*((int8_t *)task_1FC + 2) == 0) {
      Script_ShowUIMessageFromID("E1010A007");
      *((int8_t *)task_1FC + 2) = 1;
    } else {
      script_EFE8(0);
    }
  } else if (local_FC == 0x33425554) {
    if (*((int8_t *)task_1FC + 4) == 0) {
      Script_ShowUIMessageFromID("E1010A079");
      *((int8_t *)task_1FC + 4) = 1;
    } else {
      script_EFE8(0);
    }
  } else if (local_FC == 0x34425554) {
    if (*((int8_t *)task_1FC + 5) == 0) {
      Script_ShowUIMessageFromID("E1010A080");
      *((int8_t *)task_1FC + 5) = 1;
    } else {
      script_EFE8(1);
    }
  } else if ((local_FC == 0x304B4A4F) | (local_FC == 0x314B4A4F) | (local_FC == 0x324B4A4F) | (local_FC == 0x334B4A4F) | (local_FC == 0x344B4A4F) | (local_FC == 0x304C4A4F) | (local_FC == 0x314C4A4F) | (local_FC == 0x324C4A4F)) {
    if ((*((int8_t *)task_1FC + 12) == 0) | (*((int8_t *)task_1FC + 12) == 1) | (*((int8_t *)task_1FC + 12) == 2)) {
      local_FC = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f));
      if (local_FC == 0) {
        Script_ShowUIMessageFromID("E1010A087");
      } else {
        Script_ShowUIMessageFromID("E1010A088");
      }
      *((int8_t *)task_1FC + 12) = *((int8_t *)task_1FC + 12) + 1;
    } else {
      if (*((int8_t *)task_1FC + 12) == 3) {
        local_FC = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 4.0f));
        if (local_FC == 0) {
          Script_ShowUIMessageFromID("E1010A025");
        } else if (local_FC == 1) {
          Script_ShowUIMessageFromID("E1010A089");
        } else {
          Script_ShowUIMessageFromID("E1010A041");
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
          Script_ShowUIMessageFromID("E1010A042");
        }
        goto L00F34A;
      }
    }
    L00F34A:;
  } else if (local_FC == 0x334C4A4F) {
    if (*((int8_t *)task_1FC + 9) == 0) {
      Script_ShowUIMessageFromID("E1010A082");
      *((int8_t *)task_1FC + 9) = 1;
    } else {
      script_EFE8(2);
    }
  } else if (local_FC == 0) {
    if (*((int8_t *)task_1FC + 13) == 0) {
      Script_ShowUIMessageFromID("E1010A028");
      *((int8_t *)task_1FC + 13) = 1;
    } else {
      script_EFE8(1);
    }
  } else if (local_FC == 0x4B42494B) {
    Script_ShowUIMessageFromID("E1010A085");
  } else if (local_FC == 0x30464A4F) {
    if (*((int8_t *)task_1FC + 14) == 0) {
      Script_ShowUIMessageFromID("E1010A043");
      *((int8_t *)task_1FC + 14) = 1;
    } else {
      local_FC = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f));
      if (local_FC == 0) {
        Script_ShowUIMessageFromID("E1010A044");
      } else if (local_FC == 1) {
        Script_ShowUIMessageFromID("E1010A045");
      } else {
        Script_ShowUIMessageFromID("E1010A042");
      }
    }
  } else if (local_FC == 0x30444A4F) {
    Script_ShowUIMessageFromID("E1010A042");
  } else if (local_FC == 0x31444A4F) {
    Script_ShowUIMessageFromID("E1010A018");
  } else if (local_FC == 0x30454A4F) {
    if (*((int8_t *)task_1FC + 8) == 0) {
      Script_ShowUIMessageFromID("E1010A085");
      *((int8_t *)task_1FC + 8) = 1;
    } else {
      script_EFE8(1);
    }
  } else if (local_FC == 0x30484A4F) {
    if (*((int8_t *)task_1FC + 10) == 0) {
      Script_ShowUIMessageFromID("E1010A036");
      *((int8_t *)task_1FC + 10) = 1;
    } else {
      script_EFE8(1);
    }
  } else if (local_FC == 0x30494A4F) {
    if (*((int8_t *)task_1FC + 11) == 0) {
      Script_ShowUIMessageFromID("E1010A037");
      *((int8_t *)task_1FC + 11) = 1;
    } else {
      script_EFE8(1);
    }
  } else if (local_FC == 0x30474A4F) {
    Script_ShowUIMessageFromID("E1010A014");
  } else if (local_FC == 0x304A4A4F) {
    Script_ShowUIMessageFromID("E1010A017");
  } else if ((local_FC == 1) | (local_FC == 2)) {
    local_FC = *((int8_t *)task_1FC + 16);
    if (local_FC == 0) {
      Script_ShowUIMessageFromID("E1010A049");
      *((int8_t *)task_1FC + 16) = *((int8_t *)task_1FC + 16) + 1;
    } else if (local_FC == 1) {
      Script_ShowUIMessageFromID("E1010A050");
      *((int8_t *)task_1FC + 16) = *((int8_t *)task_1FC + 16) + 1;
    } else {
      local_FC = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f));
      if (local_FC == 0) {
        Script_ShowUIMessageFromID("E1010A051");
      } else {
        Script_ShowUIMessageFromID("E1010A039");
      }
    }
  } else if ((local_FC == 0x30414A4F) | (local_FC == 0x31414A4F) | (local_FC == 0x32414A4F) | (local_FC == 0x30434A4F)) {
    if (*((int8_t *)task_1FC + 18) == 0) {
      Script_ShowUIMessageFromID("E1010A055");
      *((int8_t *)task_1FC + 18) = 1;
    } else {
      Script_ShowUIMessageFromID("E1010A015");
    }
  } else if ((local_FC == 0x30424A4F) | (local_FC == 0x32424A4F)) {
    if (*((int8_t *)task_1FC + 17) == 0) {
      Script_ShowUIMessageFromID("E1010A052");
      *((int8_t *)task_1FC + 17) = 1;
    } else {
      local_FC = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f));
      if (local_FC == 0) {
        Script_ShowUIMessageFromID("E1010A053");
      } else {
        Script_ShowUIMessageFromID("E1010A054");
      }
    }
  } else {
    switch (local_FC) {
      case 0x444F4257:
        if (*((int8_t *)task_1FC + 21) == 0) {
          Script_ShowUIMessageFromID("E1010A070");
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
          Script_ShowUIMessageFromID("E1010A071");
          *((int8_t *)task_1FC + 21) = 1;
        } else {
          Script_ShowUIMessageFromID("E1010A072");
        }
        break;
      case 3:
        local_FC = *((int8_t *)task_1FC + 22);
        if (local_FC == 0) {
          Script_ShowUIMessageFromID("E1010A090");
          *((int8_t *)task_1FC + 22) = *((int8_t *)task_1FC + 22) + 1;
        } else if (local_FC == 1) {
          Script_ShowUIMessageFromID("E1010A091");
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
          Script_ShowUIMessageFromID("E1010A092");
          *((int8_t *)task_1FC + 22) = *((int8_t *)task_1FC + 22) + 1;
        } else {
          script_EFE8(0);
        }
        break;
      case 4:
        if (Script_ryoControl(PCCTL_QueryShouldRun)) {
          *(int8_t *)task_1FC = *(int8_t *)task_1FC + 1;
          if (*(int8_t *)task_1FC == 5) {
            local_FC = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 5.0f));
            if (local_FC == 0) {
              Script_ShowUIMessageFromID("E1010A064");
            } else if (local_FC == 1) {
              Script_ShowUIMessageFromID("E1010A065");
            } else if (local_FC == 2) {
              Script_ShowUIMessageFromID("E1010A066");
            } else if (local_FC == 3) {
              Script_ShowUIMessageFromID("E1010A067");
            } else {
              Script_ShowUIMessageFromID("E1010A068");
            }
            *(int8_t *)task_1FC = 0;
          }
        }
        break;
    }
  }
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static int32_t script_F876(int32_t arg0) {
  int32_t local_FC;
  for (;;) {
    local_FC = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_GetCharaMotionRemainingFramesScaled(arg0));
    switch (local_FC) {
      case 65:
        Script_executeAicaCommand(1449, 0, 0);
        break;
      case 90:
        Script_executeAicaCommand(1449, 0, 0);
        break;
      case 170:
        goto L00F8D3;
        break;
    }
    SCNF_WaitFrames_0000(1);
  }
  L00F8D3:;
}

static int32_t script_F8D4(int32_t arg0) {
  int32_t local_FC;
  for (;;) {
    local_FC = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_GetCharaMotionRemainingFramesScaled(arg0));
    switch (local_FC) {
      case 10:
        Script_executeAicaCommand(66985, 0, 0);
        break;
      case 18:
        Script_executeAicaCommand(66985, 0, 0);
        break;
      case 20:
        goto L00F934;
        break;
    }
    SCNF_WaitFrames_0000(1);
  }
  L00F934:;
}

static void script_F935(int32_t arg0) {
  Script_PlayCharaMotion(arg0, 4412, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  for (;;) {
    if (Script_IsCharaMotionFinished(arg0)) {
      break;
    }
    Script_RotateNodeUtils(arg0, &data_10AE8, 0);
    SCNF_WaitFrames_0000(1);
  }
  Script_scenCreateTask(script_F8D4, 1, arg0);
  Script_PlayCharaMotion(arg0, 4411, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  for (;;) {
    if (Script_IsCharaMotionFinished(arg0)) {
      break;
    }
    Script_RotateNodeUtils(arg0, &data_10AE8, 0);
    SCNF_WaitFrames_0000(1);
  }
  Script_PlayCharaMotion(arg0, 4410, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  for (;;) {
    if (Script_IsCharaMotionFinished(arg0)) {
      break;
    }
    Script_RotateNodeUtils(arg0, &data_10AE8, 0);
    SCNF_WaitFrames_0000(1);
  }
  return;
}

__attribute__((scn_string_bindings(4, data_14554)))
static void script_FA0E(int32_t arg0) {
  int32_t local_FC;
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_FaceEx(CHARA_AKIR, 1, 0, 2);
  Script_ResetHandMotion(CHARA_AKIR, -1, 0, 0);
  Script_ResetHandMotion(CHARA_AKIR, -1, 1, 0);
  Script_shenClearFreeState(0);
  Script_SetGameStateTo(AUTH_EventCam);
  Script_seqEnterEvtCam(200, 0, AUTH_None);
  Script_scenCharaSubmitMoveTargetRequest(arg0, &data_14514, 0x8000, 4412, 0);
  while (Script_IsMoveTargetRequestActive(arg0) != 0) {
    SCNF_WaitFrames_0000(1);
  }
  script_F935(arg0);
  Script_ShowUIMessageFromID("E1010A019");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  script_F935(arg0);
  Script_seqEnterEvtCam(201, 0, AUTH_None);
  Script_ShowUIMessageFromID("E1010A020");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_scenCharaSubmitMoveTargetRequest(arg0, &data_14520, 0x8000, 4406, 0);
  while (Script_IsMoveTargetRequestActive(arg0) != 0) {
    SCNF_WaitFrames_0000(1);
  }
  Script_seqEnterEvtCam(202, 0, AUTH_None);
  Script_scenCreateTask(script_F876, 1, arg0);
  Script_charaToggleMotEnabled(arg0, 0);
  Script_PlayCharaMotion(arg0, 4406, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  for (;;) {
    if (Script_IsCharaMotionFinished(arg0)) {
      break;
    }
    Script_RotateNodeUtils(arg0, &data_10AE8, 0);
    SCNF_WaitFrames_0000(1);
  }
  local_FC = *((int8_t *)task_1FC + 23);
  if (local_FC == 0) {
    Script_ShowUIMessageFromID("E1010A038");
    *((int8_t *)task_1FC + 23) = *((int8_t *)task_1FC + 23) + 1;
  } else if (local_FC == 1) {
    Script_ShowUIMessageFromID("E1010A021");
    *((int8_t *)task_1FC + 23) = *((int8_t *)task_1FC + 23) + 1;
  } else {
    Script_ShowUIMessageFromID("E1010A039");
  }
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_charaToggleMotEnabled(arg0, 1);
  Script_convSetCamInactiveFlag(1);
  Script_SetGameStateTo(AUTH_None);
  Script_ryoControl(RYO_RestoreControl);
  Script_FaceEx(CHARA_AKIR, 0, 0, 2);
  Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
  Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
  Script_shenClearFreeState(1);
  return;
}

static void script_FC42(int32_t arg0, int32_t arg1) {
  Script_ccowUtils(arg1, 2);
  SCNF_WaitFrames_0000(1);
  Script_SetStats(STATS_PLAYER_CONTEXT_ACTION_MODE, 3);
  Script_fixoAttachEx(arg1, arg0, 12, &data_14560, &data_1456C);
  Script_CharHandlers(CHARA_AKIR, 1, 8, 16);
  Script_charaDisplay(arg1, CDM_Show);
  script_E859(14);
  return;
}

static int32_t script_FC93(void) {
  int32_t local_FC;
  for (;;) {
    local_FC = task_23C;
    switch (local_FC) {
      case 0:
        break;
      case 1:
        Script_GetCameraInputDetails(14);
        break;
      case 2:
        script_E8C3(0x52494B41);
        break;
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_FCDF(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  for (;;) {
    local_FC = Script_srchUtils(-1);
    if (Script_srchUtils(SRCH_SCRIPT_GetCurrentTargetEntryIndex) >= 0) {
      task_23C = 2;
    } else {
      task_23C = 1;
    }
    if (local_FC != -1) {
      local_F8 = local_FC;
      if (local_F8 == 0x41524153) {
        if ((Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_MKSG_MS08_TRANSITION_TRIGGERED) == 1) & (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_MS08_SCENE_EXIT_COMPLETE) == 0)) {
          if (task_23F == 0) {
            Script_scenCreateTask(script_1041D, 2, arg0, 0);
            (void)((task_23F = -1) & 0x80);
          }
        }
      } else if ((local_F8 == 0x32425554) | (local_F8 == 0x35425554) | (local_F8 == 0x36425554) | (local_F8 == 0x3242414B)) {
        script_E97A(arg0, local_FC);
      } else if (local_F8 == 0x3142414B) {
        Script_InvestigationUtils(ShenmueUtils_BeginInteractionMode, 0, 60);
        for (;;) {
          if (Script_InvestigationUtils(ShenmueUtils_PollInteractionModeDone) != 0) {
            break;
          }
          SCNF_WaitFrames_0000(1);
        }
        script_E97A(arg0, local_FC);
        Script_InvestigationUtils(ShenmueUtils_RequestInteractionModeExit);
        for (;;) {
          if (Script_InvestigationUtils(ShenmueUtils_PollInteractionModeDone) != 0) {
            break;
          }
          SCNF_WaitFrames_0000(1);
        }
      } else if (local_F8 == 0x324B424B) {
        script_EC71(arg0);
      } else {
        Script_ryoControl(PCCTL_DisablePlayerControl);
        Script_ryoControl(PCCTL_UpdateRyoNavYaw, arg0);
        script_F15C(arg0, local_FC);
        Script_ryoControl(RYO_RestoreControl);
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_FE4C(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  script_E305(arg0);
  Script_SetShouldSleepFlag(0);
  Script_SetShadowFlags(arg0, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_scenCreateTask(script_FC93, 0);
  script_FC42(arg0, SHEN_NAME('LIGH'));
  if (task_238) {
    (void)((task_23D = -(Script_ryoSleepUtils(3, 0, 1) != 0)) & 0x10);
    if (task_23D == 0) {
      Script_SetStats(STATS_PLAYER_CONTEXT_ACTION_MODE, 0);
      Script_lightSetSlotEnabled(14, 0);
    }
  } else {
    if (Script_GetBatteryLifePair(BATTERY_GET_PERCENT, 0) > 0) {
      Script_lightSetSlotEnabled(14, 1);
      (void)((task_23D = -1) & 8);
    } else {
      Script_lightSetSlotEnabled(14, 0);
      Script_SetStats(STATS_PLAYER_CONTEXT_ACTION_MODE, 0);
      (void)((task_23D = 0) & 8);
    }
    Script_ryoSleepUtils(3, 1, 1, task_23D);
  }
  if (task_23D & 8) {
    Script_GetBatteryLifePair(BATTERY_SET_DECREMENT_ENABLED, 0, 1);
  }
  Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_shenClearFreeState(1);
  Script_ryoControl(RYO_RestoreControl);
  for (;;) {
    Script_EVT_GetPosition(arg0, -1, &task_218, 0);
    Script_mtwkGetSetRotation(arg0, &task_224, 0x42000000);
    if (-(Script_srchUtils(SRCH_SCRIPT_GetCurrentTargetEntryIndex) < 0) & Script_IsPlayerAllowed(0)) {
      int32_t unused_EC[3];
      Vector3 local_E0;
      Vector3 local_D4;
      local_D4.x = (&task_218)->x + Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, task_224.y) * 0.3f;
      (&local_D4)->y = task_218.y;
      (&local_D4)->z = task_218.z + Script_MathUtils(SCRIPT_MATH_COS_FIXED16, task_224.y) * 0.3f;
      Script_EVT_GetPosition(SHEN_NAME('SARA'), -1, &local_E0, 0);
      (&local_E0)->y = 0.0f;
      if (Script_CheckTargetWithinAngleRange(&local_E0, 1.5f, 22755, 12743, &task_218)) {
        if (Script_CheckTargetWithinAngleRange(&local_D4, 1.4f, task_224.y, 7281, &local_E0)) {
          if (Script_GetControllerInput(1) == 0x400) {
            if (task_23F == 0) {
              script_1041D(arg0, 0);
            }
          }
        }
      } else if (Script_CheckTargetWithinAngleRange(&data_14578, 1.5f, 0xC000, 12743, &task_218) & Script_CheckTargetWithinAngleRange(&local_D4, 1.4f, task_224.y, 7281, &data_14578)) {
        if (Script_GetControllerInput(1) == 0x400) {
          Script_ryoControl(PCCTL_DisablePlayerControl);
          Script_ryoControl(PCCTL_UpdateRyoNavYaw, arg0);
          Script_PlayCharaMotion(arg0, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
          Script_ShowUIMessageFromID("E1010A022");
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
          Script_ShowUIMessageFromID("E1010A086");
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
          Script_ryoControl(RYO_RestoreControl);
        }
      } else if (Script_CheckTargetWithinAngleRange(&data_14584, 1.5f, 0, 12743, &task_218) & Script_CheckTargetWithinAngleRange(&local_D4, 1.4f, task_224.y, 7281, &data_14584)) {
        if (Script_GetControllerInput(1) == 0x400) {
          script_FA0E(arg0);
        }
      } else {
        if (Script_GetControllerInput(1) == 0x400) {
          script_F15C(arg0, 4);
          goto L010172;
        }
      }
      L010172:;
      while (Script_convGetIsActive(0)) {
        SCNF_WaitFrames_0000(1);
      }
    }
    local_FC = Script_cassetteUtils();
    local_F8 = (local_FC & -0x10000) >> 16;
    switch (local_F8) {
      case 51:
        if (task_23D & 8) {
          Script_executeAicaCommand(722345, 0, 0);
          Script_SetStats(STATS_PLAYER_CONTEXT_ACTION_MODE, 0);
          (void)((task_23D = 0) & 1);
          Script_lightSetSlotEnabled(14, 0);
          Script_ryoSleepUtils(3, 1, 1, task_23D);
          Script_GetBatteryLifePair(BATTERY_SET_DECREMENT_ENABLED, 0, 0);
        } else {
          if (Script_GetBatteryLifePair(BATTERY_GET_PERCENT, 0) > 0) {
            (void)((task_23D = -1) & 1);
            Script_lightSetSlotEnabled(14, 1);
            Script_executeAicaCommand(656809, 0, 0);
            Script_SetStats(STATS_PLAYER_CONTEXT_ACTION_MODE, 3);
            Script_ryoSleepUtils(3, 1, 1, task_23D);
            Script_GetBatteryLifePair(BATTERY_SET_DECREMENT_ENABLED, 0, 1);
          }
          Script_GetCassetteFlags(52, 1, 0, 0);
          Script_GetCassetteFlags_0(52, 1, 0);
        }
        break;
      case 52:
        while (Script_convGetIsActive(0)) {
          SCNF_WaitFrames_0000(1);
        }
        break;
    }
    {
      int32_t unused_EC[3];
      int32_t local_E8;
      local_E8 = Script_GetBatteryLifePair(BATTERY_GET_PERCENT, 0);
      if (task_23D & -(local_E8 <= 0) & Script_IsPlayerAllowed()) {
        Script_executeAicaCommand(722345, 0, 0);
        Script_SetStats(STATS_PLAYER_CONTEXT_ACTION_MODE, 0);
        (void)((task_23D = 0) & 1);
        Script_lightSetSlotEnabled(14, 0);
        Script_ryoSleepUtils(3, 1, 1, task_23D);
        Script_GetBatteryLifePair(BATTERY_SET_DECREMENT_ENABLED, 0, 0);
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_102EA(void) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  int32_t unused_F0;
  GameDateTime local_E8;
  int8_t local_E7;
  uint8_t unused_E4[3];
  (void)((task_23E = -1) & 1);
  (void)((task_23F = 0) & 1);
  Script_ReturnTimeDate(&local_E8);
  local_E7 = ((&local_E8)->hours * 60 + (&local_E8)->minutes) / 6;
  Script_scenUtils(SCENARIO_CMD_SET_STEP_LUT, 16, local_E7);
  local_F8 = script_CE70(16);
  if (task_238) {
    local_F4 = Script_ryoSleepUtils(3, 0, 0);
  } else {
    local_F4 = 0;
    Script_ryoSleepUtils(3, 1, 0, local_F4);
  }
  while (1) {
    local_FC = script_CE70(16);
    if (local_FC != local_F8) {
      local_F4 = local_F4 + 1;
      local_F8 = local_FC;
      Script_ryoSleepUtils(3, 1, 0, local_F4);
    }
    if (local_F4 >= 240) {
      if (task_23F == 0) {
        (void)((task_23F = -1) & 2);
        while (Script_TMEXUtilities(0, 0) == 0) {
          SCNF_WaitFrames_0000(1);
        }
        while (Script_TMEXUtilities(1) != 0) {
          SCNF_WaitFrames_0000(1);
        }
        Script_ryoControl(PCCTL_DisablePlayerControl);
        Script_scenCreateTask(script_1041D, 2, CHARA_AKIR, 1);
        (void)((task_23F = -1) & 2);
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

__attribute__((scn_string_bindings(0, data_145A4, 1, data_145AA)))
static int32_t script_1041D(int32_t arg0, int32_t arg1) {
  int32_t unused_FC;
  GameDateTime local_F4;
  Vector3 local_E8;
  Vector3i local_DC;
  int32_t local_D8;
  (void)((task_23F = -1) & 2);
  Script_GetBatteryLifePair(BATTERY_SET_DECREMENT_ENABLED, 0, 0);
  if (task_230) {
    Script_srchControl((void *)task_230);
  }
  if (task_23E == 0) {
    while (task_23E == 0) {
      SCNF_WaitFrames_0000(1);
    }
  }
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_InvestigationUtils(ShenmueUtils_SetupOrSyncAnchors, 0, 0);
  Script_srchUtils(SRCH_SCRIPT_Reset);
  Script_GetConversationDetails(CONV_DETAILS_MODE_CLOSE_CONVERSATION_UI, 0);
  Script_GetConversationDetails(CONV_DETAILS_MODE_RESET_STATE, 0);
  Script_wazaUtils(MOVE_UI_CMD_REQUEST_CLOSE_MOVES_SCREEN, 0, 0, 0);
  if (task_234) {
    Script_srchControl((void *)task_234);
  }
  if (arg1) {
    Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
    while (Script_filmCheckOrSetTransitionUtil(-1)) {
      SCNF_WaitFrames_0000(1);
    }
  }
  Script_SetStats(STATS_PLAYER_CONTEXT_ACTION_MODE, 0);
  Script_PlayCharaMotion(arg0, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_charaDisplay(SHEN_NAME('ARML'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('ARMR'), CDM_Hide);
  Script_ccowUtils(CHARA_AKIR, 2);
  Script_LoadVoiceFile(0, "E1010");
  Script_lightSetSlotEnabled(14, 0);
  Script_fixoClearAttachment(SHEN_NAME('LIGH'));
  local_E8.x = -17.025f;
  (&local_E8)->y = 1.223f;
  (&local_E8)->z = -11.471f;
  Script_EVT_SetPosition(SHEN_NAME('LIGH'), POS_XYZ, &local_E8);
  local_DC.x = 0;
  (&local_DC)->y = 0;
  (&local_DC)->z = 0xC000;
  Script_EVT_SetRotation(SHEN_NAME('LIGH'), 0x38000000, &local_DC, 0);
  Script_charaDisplay(SHEN_NAME('LIGH'), CDM_Show);
  if (arg1) {
    Script_filmCheckOrSetTransitionUtil(20, 255, 0, 0, 0, 0, 0, 0, 0);
  }
  Script_LoadSND(0, 2, "tsm005.snd", 0);
  if (arg1) {
    script_D305();
  } else {
    script_D2A7();
  }
  Script_ReturnTimeDate(&local_F4);
  if ((&local_F4)->hours >= 19) {
    Script_IncrementDateTimeByNum(1);
    Script_ReturnTimeDate(&local_F4);
  }
  (&local_F4)->hours = 8;
  (&local_F4)->minutes = 30;
  (&local_F4)->seconds = 0;
  Script_SetNewDateTime(&local_F4);
  local_D8 = Script_GetOwnedItemCount(51, 0);
  Script_GetCassetteFlags_0(51, 0, 0);
  local_D8 = local_D8 - 1;
  if (local_D8) {
    Script_GetCassetteFlags(51, local_D8, 0, 0);
  }
  Script_SetOwnedItemMenuFlagIfOwned(1, OwnedItem_UIITEMFLAS, 0);
  Script_SetOwnedItemMenuFlagIfOwned(1, OwnedItem_UIITEMCBAT, 0);
  Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_MS08_SCENE_EXIT_COMPLETE, 1);
  if (((&local_F4)->month == 4) & ((&local_F4)->day == 15)) {
    (&local_F4)->hours = 7;
    (&local_F4)->minutes = 59;
    (&local_F4)->seconds = 0;
    Script_SetNewDateTime(&local_F4);
    Script_shenRequestGameModuleTransition(2, SHEN_NAME('JOMO'), 4);
    SCNF_WaitFrames_0000(1);
  } else {
    Script_shenRequestGameModuleTransition(2, SHEN_NAME('JOMO'), 5);
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_106DC(void) {
  int32_t local_FC;
  script_7FBB(CHARA_AKIR, (DSWKDoorEntry *)&data_12810, (int32_t *)data_12848, (DoorResourceDescriptor *)&data_12858, (MapObjectRecord *)&data_12868, 10, 0);
  SCNF_WaitFrames_0000(2);
  DSWK_DoorUtils(DOOR_CMD_SET_STATE_1);
  script_82BB();
  for (;;) {
    if ((&task_10)->mode) {
      local_FC = task_10.entry_idx;
      if (local_FC == 0) {
        Script_scenUtils(SCENARIO_CMD_INIT_SCNT);
        SCNF_WaitFrames_0000(90);
        Script_executeAicaCommand(2720, 1, 30);
        Script_executeAicaCommand(2720, 2, 30);
        Script_executeAicaCommand(2720, 3, 30);
        Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
        while (Script_filmCheckOrSetTransitionUtil(-1)) {
          SCNF_WaitFrames_0000(1);
        }
        Script_shenRequestGameModuleTransition(2, SHEN_NAME('MKSG'), 8);
        SCNF_WaitFrames_0000(1);
        goto L0107A3;
      }
    }
    L0107A3:;
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_107AF(void) {
  Script_GetSetWeatherSettings(WEATHER_SETTINGS_SET_DISABLE_WEATHER, 1);
  Script_WeatherUtils_0(WeatherDataUtil_ApplyCurrentWeatherOrEnableDateTimeRead, 0);
  Script_WeatherTaskUtil(InitTaskData);
  Script_WeatherUtils(WeatherScript_ApplyWccpClipSet1, -28.33f, -20.97f, 1.58f, -21.0f, 1.62f, 0.92f, -28.34f, 0.96f, -0.3f, 13.15f);
  Script_WeatherTaskUtil(ToggleFogTask, 0);
  Script_WeatherTaskUtil(ToggleSceneLightingFlags, 0);
  Script_SetGlobalSceneSlotValue34(4, 1);
  Script_SetupLightFromTable(0);
  Script_ProcessFogFromTable(0);
  return;
}

static int32_t script_1082D(void) {
  while (1) {
    if (Script_filmCheckOrSetTransitionUtil(-1)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_cdttLoadScrUtils(1, 0);
  end:;
}

static void script_10856(const char *arg0, const char *arg1) {
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, arg0, arg1) == 0) {
    SCNF_WaitFrames_0000(1);
  }
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

__attribute__((scn_string_bindings(0, data_145B5, 1, data_145C2, 2, data_145D0, 3, data_145DD, 4, data_145EB, 5, data_145F8, 6, data_14606, 7, data_14613, 10, data_1462B, 11, data_14636, 12, data_14643, 13, data_14649, 14, data_14654, 15, data_1465A, 16, data_14665, 17, data_14672, 18, data_14678, 19, data_14683)))
static void script_10890(void) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  SCNF_WaitFrames_0000(1);
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_mapmSetRenderFlag(1, 0);
  local_F4 = Script_GetCurrentEntryPoint();
  if (local_F4 == 90) {
    local_FC = 0;
    Script_AuthorUtils(0, 0, "SEQDATA0.BIN", "SCENE/02/MS08");
    Script_AuthorUtils(0, 1, "SEQDATA1.BIN", "SCENE/02/MS08");
    Script_AuthorUtils(0, 4, "SEQDATA3.BIN", "SCENE/02/MS08");
    Script_AuthorUtils(0, 7, "SEQDATA6.BIN", "SCENE/02/MS08");
    while (1) {
      Script_filmCheckOrSetTransitionUtil(1, 255, 0, 0, 0, 0, 0, 0, 0);
      while (Script_filmCheckOrSetTransitionUtil(-1)) {
        SCNF_WaitFrames_0000(1);
      }
      while (Script_GetControllerInput(0) != 0x400) {
        if (Script_GetControllerInput(1) == 64) {
          local_FC = local_FC - 1;
        }
        if (Script_GetControllerInput(1) == 128) {
          local_FC = local_FC + 1;
        }
        if (local_FC > 0) {
          local_FC = 1;
        }
        if (local_FC < 0) {
          local_FC = 1;
        }
        local_F4 = local_FC;
        switch (local_F4) {
          case 0:
            Script_PrintDebugString(3, 25, "1 .c");
            break;
          case 1:
            Script_PrintDebugString(3, 25, "2 .c");
            break;
        }
        SCNF_WaitFrames_0000(1);
      }
      Script_filmCheckOrSetTransitionUtil(1, 0, 0, 0, 0, 255, 0, 0, 0);
      while (Script_filmCheckOrSetTransitionUtil(-1)) {
        SCNF_WaitFrames_0000(1);
      }
      SCNF_WaitFrames_0000(60);
      local_F4 = local_FC;
      switch (local_F4) {
        case 0:
          Script_LoadSND(0, 2, "tsm005.snd", 0);
          Script_LoadSND(0, 4, "a1_chide.snd", 0);
          Script_LoadVoiceFile(1, "A0154");
          local_F8 = Script_motiLoad(2, SHEN_NAME('MS08'), "M_0154.BIN");
          script_D2A7();
          Script_LoadVoiceFile(0, "A0154");
          break;
        case 1:
          Script_LoadSND(0, 2, "tsm005.snd", 0);
          Script_LoadSND(0, 4, "a1_chide.snd", 0);
          Script_LoadVoiceFile(1, "A0154");
          local_F8 = Script_motiLoad(2, SHEN_NAME('MS08'), "M_0154.BIN");
          script_D305();
          Script_LoadVoiceFile(0, "A0154");
          break;
      }
      Script_ReallocateUNFO(local_F8);
      SCNF_WaitFrames_0000(60);
    }
    goto L010AE7;
  }
  L010AE7:;
  return;
}
