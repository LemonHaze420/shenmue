#include "act02_mksg.h"

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
static void script_9EB2(int32_t arg0, float arg1, float arg2, float arg3);
static void script_9EDA(int32_t arg0, float arg1, float arg2, float arg3);
static void script_9F2E(int32_t arg0, int32_t arg1, int32_t arg2);
static int32_t script_9F7E(int32_t arg0);
static void script_A025(int32_t arg0, int32_t arg1);
static int32_t script_A048(int32_t arg0);
static void script_A07B(int32_t arg0, int32_t arg1);
static void script_A09E(int32_t arg0, int32_t arg1);
static void script_A0FD(int32_t arg0);
static void script_A112(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_A4A8(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_AB36(void);
static void script_AB45(void);
static void script_AB54(void);
static void script_AB57(void);
static void script_AB5A(void);
static void script_AB5D(void);
static void script_AB60(void);
static void script_AB6D(void);
static void script_AB79(void);
static int32_t script_AB88(void);
static void script_AB97(void);
static void script_ACCF(int32_t arg0);
static void script_ACDB(int32_t arg0);
static void script_ACE7(int32_t arg0);
static void script_ADDA(int32_t arg0);
static void script_ADF1(int32_t arg0);
static void script_AE32(int32_t arg0);
static void script_AE76(int32_t arg0);
static void script_AEF4(void);
static void script_AF73(void);
static void script_B7EB(int32_t arg0);
static void script_B83B(void);
static int32_t script_B8C1(void);
static void script_BA85(void);
static int32_t script_BB67(int32_t arg0);
static void script_BBE2(int32_t arg0, int32_t arg1);
static int32_t script_BC5A(void);
static void script_BC86(void);
static void script_BD87(void);
static int32_t script_C0A3(int32_t arg0);
static int32_t script_C149(void);
static int32_t script_C613(void);
static int32_t script_CA66(void);
static int32_t script_CFBB(void);
static int32_t script_D13F(void);
static int32_t script_D255(void);
static int32_t script_D30B(void);
static int32_t script_D3A9(void);
static void script_D447(void);
static void script_D497(void);
static void script_D996(int32_t arg0);
static void script_D9C1(int32_t arg0);
static int32_t script_D9EF(int32_t arg0);
static void script_DA0C(int32_t arg0);
static void script_DA2B(int32_t arg0, float arg1);
static void script_DA47(int32_t arg0);
static int32_t script_DA63(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_DB1E(void);
static int32_t script_DB2A(void);
static int32_t script_DB36(void);
static int32_t script_DB42(void);
static int32_t script_DB4E(void);
static int32_t script_DB5A(void);
static int32_t script_DB66(void);
static int32_t script_DB72(void);
static int32_t script_DB7E(int32_t arg0);
static int32_t script_DD02(int32_t arg0);
static int32_t script_DEFF(int32_t arg0);
static int32_t script_E35E(void);
static void script_E373(void);
static void script_ED14(const char *arg0, const char *arg1);
static int32_t script_ED4E(const char *arg0);
static void script_ED69(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_EF3F(const char *arg0);
static int32_t script_F131(void);
static int32_t script_F537(void);
static void script_F55C(int32_t arg0);
static void script_F62C(void);
static void script_F733(void);
static void script_F807(void);
static void script_F81D(void);
static int32_t script_F851(void);
static int32_t script_F86B(void);
static void script_F930(int32_t arg0);
static int32_t script_F93C(int32_t arg0);
static void script_F9A4(void);
static int32_t script_FC24(void);
static int32_t script_FC49(void);
static int32_t script_FC6A(void);
static void script_FC93(int32_t arg0);
void script_FED5(void);
static void script_102B5(int32_t arg0, int32_t arg1);
static void script_1032D(int32_t arg0, int32_t arg1);
static void script_103BD(int32_t arg0, int32_t arg1);
static void script_10421(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_104DE(int32_t arg0, const Vector3 *arg1, const Vector3 *arg2);
static int32_t script_1058C(int32_t arg0, int32_t arg1);
static int32_t script_106DB(int32_t arg0, int32_t arg1);
static int32_t script_10D1C(int32_t arg0, int32_t arg1);
static void script_10D92(int32_t arg0);
static int32_t script_10F19(int32_t arg0, int32_t arg1);
static int32_t script_11305(int32_t arg0);
static void script_1148A(int32_t arg0);
static void script_114C4(int32_t arg0, int32_t arg1);
static void script_11872(int32_t arg0);
static void script_119B5(void);
static void script_11A0D(void);
static void script_11A65(int32_t arg0, int32_t arg1);
static void script_11BEB(int32_t arg0);
static void script_12037(int32_t arg0);
static int32_t script_12362(int32_t arg0);
static void script_123E9(int32_t arg0);
static int32_t script_12432(void);
static void script_129CC(int32_t arg0);
static int32_t script_12A16(int32_t arg0);
static int32_t script_13627(void);
static void script_1371D(int32_t arg0);
static void script_13761(int32_t arg0);

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
static int8_t task_1AC;
static uint8_t task_1AD;
static uint16_t task_1AE;
static int32_t task_1B0;
static int32_t task_1B4;
static int32_t task_1B8;
static float task_1BC;
static float task_1C0;
static int8_t task_1C4;
static int8_t task_1C5;
static int8_t task_1C6;
static int8_t task_1C7;
static int8_t task_1C8;
static int8_t task_1C9;
static int8_t task_1CA;
static int8_t task_1CB;
static int8_t task_1CC;
static int8_t task_1CD;
static uint16_t task_1CE;
static int32_t task_1D0[5];
static int32_t task_1E4;
static int32_t task_1E8;
static float task_1EC;
static float task_1F0;
static float task_1F4;
static float task_1F8;
static float task_1FC;
static int16_t task_200;
static int8_t task_202;
static uint8_t task_203;
static const float data_13794[3] = {1.0f, 1.0f, 1.0f};
static const int32_t data_137A0[4] = {16777215, 16777215, 16777215, 16777215};
static const float data_137B0[3] = {20.0f, 800.0f, 1.0f};
static const int32_t data_137BC = -1;
static DoorMotionSelectorBlock data_137C0[38] = {
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
static ScnDoorMotionDescriptor data_13DB0[72] = {
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
static uint32_t data_147D0 = 0xFFFFFFFF;
typedef struct Row_147D4 { float w0; float w1; float w2; float w3; int32_t w4; } Row_147D4;
static const Row_147D4 data_147D4[7] = {
  {0.0f, 0.0f, 0.0f, 0.0f, -1},
  {0.0f, 0.0f, 0.0f, 0.0f, -1},
  {0.0f, 0.0f, 0.0f, 0.0f, 1534},
  {26.0f, 55.0f, 45.0f, 75.0f, 1555},
  {61.0f, 146.0f, 45.0f, 50.0f, 1554},
  {59.0f, 124.0f, 45.0f, 70.0f, 1543},
  {33.0f, 49.0f, 45.0f, 50.0f, 1543}
};
static const float data_14860[4] = {33.0f, 49.0f, 50.0f, 50.0f};
static void (*data_14870)(int32_t, int32_t, int32_t) = script_0768;
static void (*const data_14874)(int32_t, int32_t, int32_t, int32_t) = script_0778;
static void (*const data_14878)(int32_t, int32_t, int32_t, int32_t) = script_0778;
static void (*const data_1487C)(int32_t, int32_t, int32_t, int32_t) = script_0778;
static void (*const data_14880)(int32_t, int32_t, int32_t, int32_t) = script_0778;
static void (*const data_14884)(int32_t, int32_t, int32_t, int32_t) = script_0778;
static void (*const data_14888)(int32_t, int32_t, int32_t, int32_t) = script_0778;
static void (*const data_1488C)(int32_t, int32_t, int32_t, int32_t) = script_0778;
static void (*const data_14890)(int32_t, int32_t, int32_t, int32_t) = script_0778;
static void (*const data_14894)(int32_t, int32_t, int32_t, int32_t) = script_0A1E;
static void (*const data_14898)(int32_t, int32_t, int32_t, int32_t) = script_0B7F;
static void (*const data_1489C)(int32_t, int32_t, int32_t, int32_t) = script_104C;
static void (*const data_148A0)(int32_t, int32_t, int32_t, int32_t) = script_10B3;
static void (*const data_148A4)(int32_t, int32_t, int32_t, int32_t) = script_087C;
static void (*const data_148A8)(int32_t, int32_t, int32_t, int32_t) = script_087C;
static void (*const data_148AC)(int32_t, int32_t, int32_t, int32_t) = script_0CE0;
static void (*const data_148B0)(int32_t, int32_t, int32_t, int32_t) = script_0F7E;
static void (*const data_148B4)(int32_t, int32_t, int32_t, int32_t) = script_111A;
static void (*const data_148B8)(int32_t, int32_t, int32_t, int32_t) = script_0A1E;
static void (*const data_148BC)(int32_t, int32_t, int32_t, int32_t) = script_0B7F;
static void (*const data_148C0)(int32_t, int32_t, int32_t, int32_t) = script_0CE0;
static void (*const data_148C4)(int32_t, int32_t, int32_t, int32_t) = script_0A1E;
static void (*const data_148C8)(int32_t, int32_t, int32_t, int32_t) = script_0B7F;
static void (*const data_148CC)(int32_t, int32_t, int32_t, int32_t) = script_0CE0;
static void (*const data_148D0)(int32_t, int32_t, int32_t, int32_t) = script_1161;
static void (*const data_148D4)(int32_t, int32_t, int32_t, int32_t) = script_1161;
static void (*const data_148D8)(int32_t, int32_t, int32_t, int32_t) = script_1161;
static void (*const data_148DC)(int32_t, int32_t, int32_t, int32_t) = script_1161;
static void (*const data_148E0)(int32_t, int32_t, int32_t, int32_t) = script_1297;
static void (*const data_148E4)(int32_t, int32_t, int32_t, int32_t) = script_1B58;
static void (*const data_148E8)(int32_t, int32_t, int32_t, int32_t) = script_0778;
static void (*const data_148EC)(int32_t, int32_t, int32_t, int32_t) = script_0778;
static void (*const data_148F0)(int32_t, int32_t, int32_t, int32_t) = script_0778;
static void (*const data_148F4)(int32_t, int32_t, int32_t, int32_t) = script_0778;
static void (*const data_148F8)(int32_t, int32_t, int32_t, int32_t) = script_0778;
static void (*const data_148FC)(int32_t, int32_t, int32_t, int32_t) = script_0778;
static void (*const data_14900)(int32_t, int32_t, int32_t, int32_t) = script_0778;
static void (*const data_14904)(int32_t, int32_t, int32_t, int32_t) = script_0778;
static const char data_14908[46] __attribute__((scn_string_literal)) = "Door Index = %d : call_type = %d : type = %d\012";
static const char data_14936[43] __attribute__((scn_string_literal)) = "Door Open Motion = DEB_AKESIME_FUSUMA_L_F\012";
static const char data_14961[43] __attribute__((scn_string_literal)) = "Door Open Motion = DEB_AKESIME_FUSUMA_R_F\012";
static const char data_1498C[42] __attribute__((scn_string_literal)) = "Door Open Motion = DEB_HIKISIME_DOOR_L_F\012";
static const char data_149B6[42] __attribute__((scn_string_literal)) = "Door Open Motion = DEB_HIKISIME_DOOR_R_F\012";
static const char data_149E0[41] __attribute__((scn_string_literal)) = "Door Open Motion = DEB_OSISIME_DOOR_L_F\012";
static const char data_14A09[41] __attribute__((scn_string_literal)) = "Door Open Motion = DEB_OSISIME_DOOR_R_F\012";
static const char data_14A32[43] __attribute__((scn_string_literal)) = "Door Open Motion = GAK_AKESIME_FUSUMA_L_F\012";
static const char data_14A5D[43] __attribute__((scn_string_literal)) = "Door Open Motion = GAK_AKESIME_FUSUMA_R_F\012";
static const char data_14A88[42] __attribute__((scn_string_literal)) = "Door Open Motion = GAK_HIKISIME_DOOR_L_F\012";
static const char data_14AB2[42] __attribute__((scn_string_literal)) = "Door Open Motion = GAK_HIKISIME_DOOR_R_F\012";
static const char data_14ADC[41] __attribute__((scn_string_literal)) = "Door Open Motion = GAK_OSISIME_DOOR_L_F\012";
static const char data_14B05[41] __attribute__((scn_string_literal)) = "Door Open Motion = GAK_OSISIME_DOOR_R_F\012";
static const char data_14B2E[43] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_AKESIME_FUSUMA_L_F\012";
static const char data_14B59[43] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_AKESIME_FUSUMA_R_F\012";
static const char data_14B84[42] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_HIKISIME_DOOR_L_F\012";
static const char data_14BAE[42] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_HIKISIME_DOOR_R_F\012";
static const char data_14BD8[41] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_OSISIME_DOOR_L_F\012";
static const char data_14C01[41] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_OSISIME_DOOR_R_F\012";
static const char data_14C2A[43] __attribute__((scn_string_literal)) = "Door Open Motion = KOD_AKESIME_FUSUMA_L_F\012";
static const char data_14C55[43] __attribute__((scn_string_literal)) = "Door Open Motion = KOD_AKESIME_FUSUMA_R_F\012";
static const char data_14C80[42] __attribute__((scn_string_literal)) = "Door Open Motion = KOD_HIKISIME_DOOR_L_F\012";
static const char data_14CAA[42] __attribute__((scn_string_literal)) = "Door Open Motion = KOD_HIKISIME_DOOR_R_F\012";
static const char data_14CD4[41] __attribute__((scn_string_literal)) = "Door Open Motion = KOD_OSISIME_DOOR_L_F\012";
static const char data_14CFD[41] __attribute__((scn_string_literal)) = "Door Open Motion = KOD_OSISIME_DOOR_R_F\012";
static const char data_14D26[43] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_AKESIME_FUSUMA_L_F\012";
static const char data_14D51[43] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_AKESIME_FUSUMA_R_F\012";
static const char data_14D7C[42] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_HIKISIME_DOOR_L_F\012";
static const char data_14DA6[42] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_HIKISIME_DOOR_R_F\012";
static const char data_14DD0[41] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_OSISIME_DOOR_L_F\012";
static const char data_14DF9[41] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_OSISIME_DOOR_R_F\012";
static const char data_14E22[43] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_AKESIME_FUSUMA_L_F\012";
static const char data_14E4D[43] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_AKESIME_FUSUMA_R_F\012";
static const char data_14E78[42] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_HIKISIME_DOOR_L_F\012";
static const char data_14EA2[42] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_HIKISIME_DOOR_R_F\012";
static const char data_14ECC[41] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_OSISIME_DOOR_L_F\012";
static const char data_14EF5[41] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_OSISIME_DOOR_R_F\012";
static const char data_14F1E[43] __attribute__((scn_string_literal)) = "Door Open Motion = SIN_AKESIME_FUSUMA_L_F\012";
static const char data_14F49[43] __attribute__((scn_string_literal)) = "Door Open Motion = SIN_AKESIME_FUSUMA_R_F\012";
static const char data_14F74[42] __attribute__((scn_string_literal)) = "Door Open Motion = SIN_HIKISIME_DOOR_L_F\012";
static const char data_14F9E[42] __attribute__((scn_string_literal)) = "Door Open Motion = SIN_HIKISIME_DOOR_R_F\012";
static const char data_14FC8[41] __attribute__((scn_string_literal)) = "Door Open Motion = SIN_OSISIME_DOOR_L_F\012";
static const char data_14FF1[41] __attribute__((scn_string_literal)) = "Door Open Motion = SIN_OSISIME_DOOR_R_F\012";
static const char data_1501A[43] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_AKESIME_FUSUMA_L_F\012";
static const char data_15045[43] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_AKESIME_FUSUMA_R_F\012";
static const char data_15070[42] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_HIKISIME_DOOR_L_F\012";
static const char data_1509A[42] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_HIKISIME_DOOR_R_F\012";
static const char data_150C4[41] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_OSISIME_DOOR_L_F\012";
static const char data_150ED[41] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_OSISIME_DOOR_R_F\012";
static const char data_15116[43] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_AKESIME_FUSUMA_L_F\012";
static const char data_15141[43] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_AKESIME_FUSUMA_R_F\012";
static const char data_1516C[42] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_HIKISIME_DOOR_L_F\012";
static const char data_15196[42] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_HIKISIME_DOOR_R_F\012";
static const char data_151C0[41] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_OSISIME_DOOR_L_F\012";
static const char data_151E9[41] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_OSISIME_DOOR_R_F\012";
static const char data_15212[44] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_OSISIME_DOOR_AP_L_F\012";
static const char data_1523E[44] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_OSISIME_DOOR_AP_L_F\012";
static const char data_1526A[44] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_OSISIME_DOOR_AP_L_F\012";
static const char data_15296[44] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_OSISIME_DOOR_AP_L_F\012";
static const char data_152C2[44] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_OSISIME_DOOR_AP_L_F\012";
static const char data_152EE[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_1530A[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_15326[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_15342[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_1535E[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_1537A[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_15396[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_153B2[13] __attribute__((scn_string_literal)) = "open-move!!\012";
static const char data_153BF[11] __attribute__((scn_string_literal)) = "M_MDOR.BIN";
static const char data_153CA[] __attribute__((scn_string_literal)) = "This Charactor Work is NULL!!\n";
static const char data_153E9[22] __attribute__((scn_string_literal)) = "手動ドア開け番号指定\012";
static const char data_153FF[34] __attribute__((scn_string_literal)) = "無限ループになりそうなので抜ける\012";
static const char data_15421[] __attribute__((scn_string_literal)) = "Don't make BOX Buffer!!\n";
static const char data_1543A[] __attribute__((scn_string_literal)) = "Don't make BOX Buffer!!\n";
static const char data_15453[15] __attribute__((scn_string_literal)) = "Door System : ";
static const char data_15462[15] __attribute__((scn_string_literal)) = "Door System : ";
static const char data_15471[18] __attribute__((scn_string_literal)) = "Door Box is ON!!\012";
static const char data_15483[19] __attribute__((scn_string_literal)) = "Door Box is OFF!!\012";
static const char data_15496[18] __attribute__((scn_string_literal)) = "Door Box is ON!!\012";
static const char data_154A8[20] __attribute__((scn_string_literal)) = "Door Box is OFF!!\012";
static const DSWKDoorEntry data_154BC[11] = {
  {.slot = 0, .flags = 0, .ctrl_mask_pair = 0x640064U, .coarse_metric = 0.0f, .selection_metric = 0.0f, .view_ang_delta = 0, .group = 0, .global1 = 0xFFFFFFFFU, .global2 = 0xFFFFFFFFU, .global3 = 0xFFFFFFFFU, .sndCommandId = 0xFFFFFFFFU, .global5 = 0xFFFFFFFFU, .global6 = 0xFFFFFFFFU},
  {.slot = 3, .flags = 0, .ctrl_mask_pair = 0x640064U, .coarse_metric = 0.0f, .selection_metric = 0.0f, .view_ang_delta = 0, .group = 0, .global1 = 0xFFFFFFFFU, .global2 = 0xFFFFFFFFU, .global3 = 0xFFFFFFFFU, .sndCommandId = 0xFFFFFFFFU, .global5 = 0xFFFFFFFFU, .global6 = 0xFFFFFFFFU},
  {.slot = 6, .flags = 0, .ctrl_mask_pair = 0x640064U, .coarse_metric = 0.0f, .selection_metric = 0.0f, .view_ang_delta = 0, .group = 0, .global1 = 0xFFFFFFFFU, .global2 = 0xFFFFFFFFU, .global3 = 0xFFFFFFFFU, .sndCommandId = 0xFFFFFFFFU, .global5 = 0xFFFFFFFFU, .global6 = 0xFFFFFFFFU},
  {.slot = 9, .flags = 0, .ctrl_mask_pair = 0x640064U, .coarse_metric = 0.0f, .selection_metric = 0.0f, .view_ang_delta = 0, .group = 0, .global1 = 0xFFFFFFFFU, .global2 = 0xFFFFFFFFU, .global3 = 0xFFFFFFFFU, .sndCommandId = 0xFFFFFFFFU, .global5 = 0xFFFFFFFFU, .global6 = 0xFFFFFFFFU},
  {.slot = 12, .flags = 0, .ctrl_mask_pair = 0x640064U, .coarse_metric = 0.0f, .selection_metric = 0.0f, .view_ang_delta = 0, .group = 0, .global1 = 0xFFFFFFFFU, .global2 = 0xFFFFFFFFU, .global3 = 0xFFFFFFFFU, .sndCommandId = 0xFFFFFFFFU, .global5 = 0xFFFFFFFFU, .global6 = 0xFFFFFFFFU},
  {.slot = 15, .flags = 0, .ctrl_mask_pair = 0x640064U, .coarse_metric = 0.0f, .selection_metric = 0.0f, .view_ang_delta = 0, .group = 0, .global1 = 0xFFFFFFFFU, .global2 = 0xFFFFFFFFU, .global3 = 0xFFFFFFFFU, .sndCommandId = 0xFFFFFFFFU, .global5 = 0xFFFFFFFFU, .global6 = 0xFFFFFFFFU},
  {.slot = 18, .flags = 0, .ctrl_mask_pair = 0x640064U, .coarse_metric = 0.0f, .selection_metric = 0.0f, .view_ang_delta = 0, .group = 0, .global1 = 0xFFFFFFFFU, .global2 = 0xFFFFFFFFU, .global3 = 0xFFFFFFFFU, .sndCommandId = 0xFFFFFFFFU, .global5 = 0xFFFFFFFFU, .global6 = 0xFFFFFFFFU},
  {.slot = 21, .flags = 0, .ctrl_mask_pair = 0x640064U, .coarse_metric = 0.0f, .selection_metric = 0.0f, .view_ang_delta = 0, .group = 0, .global1 = 0xFFFFFFFFU, .global2 = 0xFFFFFFFFU, .global3 = 0xFFFFFFFFU, .sndCommandId = 0xFFFFFFFFU, .global5 = 0xFFFFFFFFU, .global6 = 0xFFFFFFFFU},
  {.slot = 24, .flags = 0, .ctrl_mask_pair = 0x640064U, .coarse_metric = 0.0f, .selection_metric = 0.0f, .view_ang_delta = 0, .group = 0, .global1 = 0xFFFFFFFFU, .global2 = 0xFFFFFFFFU, .global3 = 0xFFFFFFFFU, .sndCommandId = 0xFFFFFFFFU, .global5 = 0xFFFFFFFFU, .global6 = 0xFFFFFFFFU},
  {.slot = 26, .flags = 0, .ctrl_mask_pair = 0x640064U, .coarse_metric = 0.0f, .selection_metric = 0.0f, .view_ang_delta = 0, .group = 0, .global1 = 0xFFFFFFFFU, .global2 = 0xFFFFFFFFU, .global3 = 0xFFFFFFFFU, .sndCommandId = 0xFFFFFFFFU, .global5 = 0xFFFFFFFFU, .global6 = 0xFFFFFFFFU},
  {.slot = 29, .flags = 0, .ctrl_mask_pair = 0x640064U, .coarse_metric = 0.0f, .selection_metric = 0.0f, .view_ang_delta = 0, .group = 0, .global1 = 0xFFFFFFFFU, .global2 = 0xFFFFFFFFU, .global3 = 0xFFFFFFFFU, .sndCommandId = 0xFFFFFFFFU, .global5 = 0xFFFFFFFFU, .global6 = 0xFFFFFFFFU}
};
static const int32_t data_156F8[13] = {-1, 0, 6553700, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1};
static const uint8_t data_1572C[4] = {0};
static const int32_t data_15730[30] = {1, -1, 13, 14, -1, 2, 3, -1, 4, 5, -1, 11, 12, -1, 15, 16, -1, 17, 18, -1, 9, 10, -1, 6, -1, 7, 8, -1, 19, -1};
static char data_157A8[] = "B49M2D1G";
static char data_157B1[] = "B49I2B1G";
static char data_157BA[] = "B49I2B2G";
static char data_157C3[] = "B49I2B4G";
static char data_157CC[] = "B49I2B3G";
static const uint8_t data_157D5[3] = {0};
static char *const data_157D8 = data_157A8;
static const int32_t data_157DC[3] = {2, 1966763, 2032299};
static char *const data_157E8 = data_157B1;
static const int32_t data_157EC[3] = {0, -1, -1};
static char *const data_157F8 = data_157BA;
static const int32_t data_157FC[3] = {0, -1, -1};
static char *const data_15808 = data_157C3;
static const int32_t data_1580C[3] = {0, -1, -1};
static char *const data_15818 = data_157CC;
static const int32_t data_1581C[3] = {0, -1, -1};
static const MapObjectRecord data_15828[20] = {
  {.kind = 2, .resourceIndex = 0, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-34.6f, 0.0f, -22.48674f}, .yawFixed16 = -32768},
  {.kind = 1, .resourceIndex = 1, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-19.8f, 0.0f, -22.4862f}, .yawFixed16 = 0},
  {.kind = 2, .resourceIndex = 0, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-66.6026f, 0.0f, -4.11057f}, .yawFixed16 = -16384},
  {.kind = 1, .resourceIndex = 2, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-68.39f, 0.0f, -22.51f}, .yawFixed16 = 0},
  {.kind = 2, .resourceIndex = 0, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-54.50322f, 0.0f, -27.923229f}, .yawFixed16 = 0},
  {.kind = 1, .resourceIndex = 1, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-69.3032f, 0.0f, -27.9237f}, .yawFixed16 = 32767},
  {.kind = 2, .resourceIndex = 0, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-45.903011f, 0.0f, -74.040108f}, .yawFixed16 = 16384},
  {.kind = 2, .resourceIndex = 0, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-44.49f, 0.0f, -50.380451f}, .yawFixed16 = 32768},
  {.kind = 1, .resourceIndex = 3, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-29.69f, 0.0f, -50.38f}, .yawFixed16 = -16383},
  {.kind = 2, .resourceIndex = 0, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-28.08544f, 0.0f, -81.607246f}, .yawFixed16 = 16384},
  {.kind = 1, .resourceIndex = 4, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-27.96f, 0.0f, -72.054901f}, .yawFixed16 = 0},
  {.kind = 2, .resourceIndex = 0, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-22.50049f, 0.0f, -61.699959f}, .yawFixed16 = -16384},
  {.kind = 1, .resourceIndex = 3, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-22.5f, 0.0f, -76.5f}, .yawFixed16 = 0},
  {.kind = 2, .resourceIndex = 0, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-12.51491f, 0.0f, -50.442188f}, .yawFixed16 = -16384},
  {.kind = 1, .resourceIndex = 4, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-12.64f, 0.0f, -59.9949f}, .yawFixed16 = -32767},
  {.kind = 2, .resourceIndex = 0, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-22.51013f, 0.0f, -31.078199f}, .yawFixed16 = -16384},
  {.kind = 1, .resourceIndex = 1, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-22.5096f, 0.0f, -45.878201f}, .yawFixed16 = 16383},
  {.kind = 2, .resourceIndex = 0, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-12.5149f, 0.0f, -20.0473f}, .yawFixed16 = -16384},
  {.kind = 1, .resourceIndex = 4, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-12.64f, 0.0f, -29.6f}, .yawFixed16 = -32767},
  {.kind = 2, .resourceIndex = 0, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-3.95f, 0.0f, -1.0012f}, .yawFixed16 = -16383}
};
static const uint32_t data_15AF8[4] = {0x413042CU, 0x42F042DU, 0x4300432U, 0x3870431U};
static const char data_15B08[20] __attribute__((scn_string_literal)) = "TUNED FreeTalk def\012";
static const char data_15B1C[20] __attribute__((scn_string_literal)) = "TUNED FreeTalk def\012";
static const uint32_t data_15B30 = 380;
static const int32_t data_15B34[6] = {1, 390, 0, 1018, 0, -1};
static const uint32_t data_15B4C = 510;
static const int32_t data_15B50[6] = {1, 525, 0, 1020, 0, -1};
static const uint32_t data_15B68 = 150;
static const int32_t data_15B6C[6] = {1, 160, 0, 1019, 0, -1};
static const uint32_t data_15B84 = 530;
static const int32_t data_15B88[6] = {1, 970, 0, 1014, 0, -1};
static const uint32_t data_15BA0 = 911;
static const int32_t data_15BA4[6] = {1, 155, 0, 1015, 0, -1};
static const char data_15BBC[7] __attribute__((scn_string_literal)) = "FREE02";
static const char data_15BC3[7] __attribute__((scn_string_literal)) = "FREE03";
static const char data_15BCA[7] __attribute__((scn_string_literal)) = "FREE04";
static const char data_15BD1[7] __attribute__((scn_string_literal)) = "FREE01";
static const uint16_t data_15BD8[8] = {1068, 1043, 1069, 1071, 1074, 1072, 1073, 0};
static const uint16_t data_15BE8[8] = {670, 672, 674, 677, 684, 682, 680, 0};
static const uint32_t data_15BF8 = 0x25C025B;
static const uint32_t data_15BFC[2] = {0x25E025DU, 0x2600261U};
static const int32_t data_15C04 = 607;
static const uint32_t data_15C08 = 22;
typedef struct Row_15C0C { float w0; float w1; int32_t w2; } Row_15C0C;
static const Row_15C0C data_15C0C[23] = {
  {-17.65f, -79.69f, 1},
  {-22.6f, -79.68f, 2},
  {-24.48f, -75.44f, 3},
  {-26.71f, -67.92f, 4},
  {-26.42f, -54.34f, 5},
  {-41.52f, -54.29f, 6},
  {-41.57f, -68.0f, 7},
  {-26.71f, -67.92f, 8},
  {-24.48f, -75.44f, 9},
  {-22.6f, -79.68f, 0},
  {-25.47f, -25.6f, 4},
  {-17.31f, -25.66f, 10},
  {-14.76f, -17.56f, 11},
  {-7.47f, -7.49f, 12},
  {-37.89f, -25.6f, 10},
  {-37.87f, -1.52f, 14},
  {-49.73f, -53.84f, 5},
  {-49.73f, -24.82f, 16},
  {-68.68f, -23.52f, 17},
  {-67.82f, -1.69f, 18},
  {-14.99f, -53.6f, 4},
  {-14.99f, -47.77f, 20},
  {0.0f, 0.0f, 0}
};
static const int32_t data_15D20 = 0;
static const uint32_t data_15D24 = 18;
typedef struct Row_15D28 { float w0; float w1; int32_t w2; } Row_15D28;
static const Row_15D28 data_15D28[19] = {
  {-67.44f, -0.24f, 1},
  {-67.92f, -23.56f, 2},
  {-50.49f, -25.27f, 3},
  {-50.59f, -51.89f, 4},
  {-63.08f, -53.13f, 5},
  {-50.59f, -51.89f, 6},
  {-25.68f, -51.96f, 7},
  {-26.96f, -24.36f, 8},
  {-37.04f, -24.14f, 9},
  {-36.96f, -0.31f, 0},
  {-17.31f, -25.66f, 7},
  {-14.76f, -17.56f, 10},
  {-7.47f, -7.49f, 11},
  {-14.99f, -53.6f, 6},
  {-14.99f, -47.77f, 13},
  {-24.3f, -68.86f, 6},
  {-43.3f, -68.86f, 15},
  {-24.3f, -80.04f, 15},
  {0.0f, 0.0f, 0}
};
static const int32_t data_15E0C = 0;
static const uint32_t data_15E10 = 18;
typedef struct Row_15E14 { float w0; float w1; int32_t w2; } Row_15E14;
static const Row_15E14 data_15E14[19] = {
  {-48.91f, -53.98f, 1},
  {-48.7f, -26.26f, 2},
  {-24.5f, -25.96f, 3},
  {-23.87f, -51.45f, 4},
  {-18.23f, -52.0f, 5},
  {-23.87f, -51.45f, 6},
  {-24.53f, -70.45f, 7},
  {-43.96f, -70.41f, 8},
  {-43.9f, -54.22f, 0},
  {-17.31f, -25.66f, 2},
  {-14.76f, -17.56f, 9},
  {-7.47f, -7.49f, 10},
  {-37.89f, -25.6f, 2},
  {-37.89f, -1.52f, 12},
  {-68.68f, -23.52f, 1},
  {-67.82f, -1.68f, 14},
  {-14.99f, -47.77f, 4},
  {-24.3f, -80.04f, 6},
  {0.0f, 0.0f, 0}
};
static const int32_t data_15EF8 = 0;
static const uint32_t data_15EFC = 0xEA0066;
static const int32_t data_15F00 = 236;
static const float data_15F04[3] = {0.05f, 0.1f, 9.0f};
static const uint32_t data_15F10 = 0xEA0066;
static const int32_t data_15F14 = 236;
static const float data_15F18[3] = {0.05f, 0.09f, 9.5f};
static const uint32_t data_15F24 = 0xEA0066;
static const int32_t data_15F28 = 236;
static const float data_15F2C[3] = {0.05f, 0.13f, 9.0f};
static const uint32_t data_15F38 = 0xEA0066;
static const int32_t data_15F3C = 236;
static const float data_15F40[3] = {0.05f, 0.11f, 9.0f};
static const uint32_t data_15F4C = 0xEA0066;
static const int32_t data_15F50 = 236;
static const float data_15F54[3] = {0.05f, 0.12f, 9.0f};
static const uint32_t data_15F60 = SHEN_NAME('AKIR');
static const shen_name data_15F64[5] = {CHARA_KEBA, CHARA_KEBB, CHARA_KEBC, CHARA_KEBD, CHARA_KEBE};
static const uint32_t data_15F78 = SHEN_NAME('LIGH');
static const shen_name data_15F7C[5] = {SHEN_NAME('LIG1'), SHEN_NAME('LIG2'), SHEN_NAME('LIG3'), SHEN_NAME('LIG4'), SHEN_NAME('LIG5')};
static const int8_t data_15F90[8] = {15, 16, 17, 18, 19, 20, 0, 0};
static const int8_t data_15F98[12] = {3, 10, 17, 24, 31, 0, 0, 0, 0, 0, 0, 0};
static const int8_t data_15FA4[12] = {7, 14, 21, 28, 0, 0, 0, 0, 0, 0, 0, 0};
static const int8_t data_15FB0[12] = {4, 11, 18, 25, 0, 0, 0, 0, 0, 0, 0, 0};
static const int8_t data_15FBC[9] = {4, 11, 18, 25, 0, 0, 0, 0, 0};
static const char data_15FC5[59] __attribute__((scn_string_literal)) = " : %d %d %d %d %d %d : %d %d %d %d : %d %d %d %d : %d %d \012";
static const char data_16000[12] __attribute__((scn_string_literal)) = "lacky : %d\012";
static const uint32_t data_1600C = 0x430C0000;
static const float data_16010[2] = {0.0f, -75.0f};
static const uint32_t data_16018 = 0x430C0000;
static const float data_1601C[2] = {0.0f, -75.0f};
static const uint8_t data_16024[12] = {0};
static const uint8_t data_16030[12] = {0};
static const uint32_t data_1603C = 0x42A00000;
static const float data_16040[2] = {0.0f, 5.0f};
static const uint8_t data_16048[12] = {0};
static const uint32_t data_16054 = 0x3F800000;
static const float data_16058[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint32_t data_160DC = 0x3F800000;
static const float data_160E0[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint8_t data_16164[84] = {0};
typedef struct Row_161B8 { float w0; float w1; float w2; int32_t w3; } Row_161B8;
static const Row_161B8 data_161B8[3] = {
  {0.787f, 0.787f, 0.787f, 0},
  {0.0f, 0.0f, 0.0f, 0},
  {0.0f, 0.0f, 0.0f, 0}
};
static const int32_t data_161E8 = 0;
static const uint32_t data_161EC = 0x3F800000;
static const float data_161F0[33] = {
  0.944f, 0.866f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f,
  0.787f, 0.787f, 0.787f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint8_t data_16274[16] = {0};
static const float data_16284[30] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint32_t data_162FC = 0x3F800000;
static const float data_16300[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const int32_t data_16384 = 16;
static char data_16388[] = "E1006A001";
static const uint8_t data_16392[2] = {0};
static char *const data_16394 = data_16388;
static char data_16398[] = "E1006B003";
static char data_163A2[] = "E1006B004";
static char data_163AC[] = "E1006B005";
static char data_163B6[] = "E1006B006";
static char data_163C0[] = "E1006B007";
static char data_163CA[] = "E1006B008";
static char data_163D4[] = "E1006B009";
static char data_163DE[] = "E1006B010";
static char data_163E8[] = "E1006B011";
static char data_163F2[] = "E1006B012";
static char data_163FC[] = "E1006B013";
static char data_16406[] = "E1006B012";
static char data_16410[] = "E1006B017";
static char data_1641A[] = "E1006B018";
static char data_16424[] = "E1006B019";
static char data_1642E[] = "E1006B020";
static char data_16438[] = "E1006B021";
static char data_16442[] = "E1006E003";
static char data_1644C[] = "E1006E004";
static char data_16456[] = "E1006E005";
static char data_16460[] = "E1006E006";
static char data_1646A[] = "E1006E007";
static char data_16474[] = "E1006E008";
static char data_1647E[] = "E1006E020";
static char data_16488[] = "E1006E021";
static char data_16492[] = "E1006E022";
static char data_1649C[] = "E1006E012";
static char data_164A6[] = "E1006E013";
static char data_164B0[] = "E1006E014";
static char data_164BA[] = "E1006E015";
static char data_164C4[] = "E1006E016";
static char data_164CE[] = "E1006E017";
static char data_164D8[] = "E1006E018";
static char data_164E2[] = "E1006E019";
static char data_164EC[] = "E1006F003";
static char data_164F6[] = "E1006F004";
static char data_16500[] = "E1006F005";
static char data_1650A[] = "E1006F006";
static char data_16514[] = "E1006F007";
static char data_1651E[] = "E1006F008";
static char data_16528[] = "E1006F020";
static char data_16532[] = "E1006F021";
static char data_1653C[] = "E1006F022";
static char data_16546[] = "E1006F012";
static char data_16550[] = "E1006F013";
static char data_1655A[] = "E1006F014";
static char data_16564[] = "E1006F015";
static char data_1656E[] = "E1006F016";
static char data_16578[] = "E1006F017";
static char data_16582[] = "E1006F018";
static char data_1658C[] = "E1006F019";
static char data_16596[] = "E1006C003";
static char data_165A0[] = "E1006C004";
static char data_165AA[] = "E1006C005";
static char data_165B4[] = "E1006C006";
static char data_165BE[] = "E1006C007";
static char data_165C8[] = "E1006C008";
static char data_165D2[] = "E1006C019";
static char data_165DC[] = "E1006C020";
static char data_165E6[] = "E1006C021";
static char data_165F0[] = "E1006C009";
static char data_165FA[] = "E1006C010";
static char data_16604[] = "E1006C009";
static char data_1660E[] = "E1006C014";
static char data_16618[] = "E1006C015";
static char data_16622[] = "E1006C016";
static char data_1662C[] = "E1006C017";
static char data_16636[] = "E1006C018";
static char data_16640[] = "E1006D003";
static char data_1664A[] = "E1006D004";
static char data_16654[] = "E1006D005";
static char data_1665E[] = "E1006D006";
static char data_16668[] = "E1006D007";
static char data_16672[] = "E1006D008";
static char data_1667C[] = "E1006D019";
static char data_16686[] = "E1006D020";
static char data_16690[] = "E1006D021";
static char data_1669A[] = "E1006D009";
static char data_166A4[] = "E1006D010";
static char data_166AE[] = "E1006D009";
static char data_166B8[] = "E1006D014";
static char data_166C2[] = "E1006D015";
static char data_166CC[] = "E1006D016";
static char data_166D6[] = "E1006D017";
static char data_166E0[] = "E1006D018";
static const uint8_t data_166EA[2] = {0};
static char *data_166EC = data_16398;
static char *const data_166F0 = data_163A2;
static char *const data_166F4 = data_163AC;
static char *const data_166F8 = data_163B6;
static char *const data_166FC = data_163C0;
static char *const data_16700 = data_163CA;
static char *const data_16704 = data_163D4;
static char *const data_16708 = data_163DE;
static char *const data_1670C = data_163E8;
static char *const data_16710 = data_163F2;
static char *const data_16714 = data_163FC;
static char *const data_16718 = data_16406;
static char *const data_1671C = data_16410;
static char *const data_16720 = data_1641A;
static char *const data_16724 = data_16424;
static char *const data_16728 = data_1642E;
static char *const data_1672C = data_16438;
static char *const data_16730 = data_16442;
static char *const data_16734 = data_1644C;
static char *const data_16738 = data_16456;
static char *const data_1673C = data_16460;
static char *const data_16740 = data_1646A;
static char *const data_16744 = data_16474;
static char *const data_16748 = data_1647E;
static char *const data_1674C = data_16488;
static char *const data_16750 = data_16492;
static char *const data_16754 = data_1649C;
static char *const data_16758 = data_164A6;
static char *const data_1675C = data_164B0;
static char *const data_16760 = data_164BA;
static char *const data_16764 = data_164C4;
static char *const data_16768 = data_164CE;
static char *const data_1676C = data_164D8;
static char *const data_16770 = data_164E2;
static char *const data_16774 = data_164EC;
static char *const data_16778 = data_164F6;
static char *const data_1677C = data_16500;
static char *const data_16780 = data_1650A;
static char *const data_16784 = data_16514;
static char *const data_16788 = data_1651E;
static char *const data_1678C = data_16528;
static char *const data_16790 = data_16532;
static char *const data_16794 = data_1653C;
static char *const data_16798 = data_16546;
static char *const data_1679C = data_16550;
static char *const data_167A0 = data_1655A;
static char *const data_167A4 = data_16564;
static char *const data_167A8 = data_1656E;
static char *const data_167AC = data_16578;
static char *const data_167B0 = data_16582;
static char *const data_167B4 = data_1658C;
static char *const data_167B8 = data_16596;
static char *const data_167BC = data_165A0;
static char *const data_167C0 = data_165AA;
static char *const data_167C4 = data_165B4;
static char *const data_167C8 = data_165BE;
static char *const data_167CC = data_165C8;
static char *const data_167D0 = data_165D2;
static char *const data_167D4 = data_165DC;
static char *const data_167D8 = data_165E6;
static char *const data_167DC = data_165F0;
static char *const data_167E0 = data_165FA;
static char *const data_167E4 = data_16604;
static char *const data_167E8 = data_1660E;
static char *const data_167EC = data_16618;
static char *const data_167F0 = data_16622;
static char *const data_167F4 = data_1662C;
static char *const data_167F8 = data_16636;
static char *const data_167FC = data_16640;
static char *const data_16800 = data_1664A;
static char *const data_16804 = data_16654;
static char *const data_16808 = data_1665E;
static char *const data_1680C = data_16668;
static char *const data_16810 = data_16672;
static char *const data_16814 = data_1667C;
static char *const data_16818 = data_16686;
static char *const data_1681C = data_16690;
static char *const data_16820 = data_1669A;
static char *const data_16824 = data_166A4;
static char *const data_16828 = data_166AE;
static char *const data_1682C = data_166B8;
static char *const data_16830 = data_166C2;
static char *const data_16834 = data_166CC;
static char *const data_16838 = data_166D6;
static char *const data_1683C = data_166E0;
static uint32_t data_16840 = 0x40400000;
static const float data_16844[4] = {3.0f, 3.0f, 3.0f, 5.0f};
static const char data_16854[35] __attribute__((scn_string_literal)) = "MKSG :chara_no %d : voice_no = %d\012";
static const char data_16877[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_1687C[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_16881[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_16886[11] __attribute__((scn_string_literal)) = "ykm005.snd";
static const char data_16891[13] __attribute__((scn_string_literal)) = "f1kyusok.snd";
static const char data_1689E[14] __attribute__((scn_string_literal)) = "e1keibin.snd";
static const uint32_t data_168AC = 0x990098;
static const int32_t data_168B0[4] = {10158234, 10289308, 10420382, 10551456};
static Vector3 data_168C0 = {.x = 0.0f, .y = 0.0f, .z = 0.0f};
static Vector3 data_168CC = {.x = 0.0f, .y = 1000.0f, .z = 0.0f};
static char data_168D8[] = "E1042B001";
static char data_168E2[] = "E1042B002";
static char data_168EC[] = "E1042B003";
static char data_168F6[] = "E1042B004";
static char data_16900[] = "E1042B037";
static char data_1690A[] = "E1042B006";
static char data_16914[] = "E1042A003";
static char data_1691E[] = "E1042B007";
static char data_16928[] = "E1042B008";
static char data_16932[] = "E1042B009";
static char data_1693C[] = "E1042A004";
static char data_16946[] = "E1042B010";
static char data_16950[] = "E1042A005";
static char data_1695A[] = "DUMMY";
static char data_16960[] = "E1042B011";
static char data_1696A[] = "E1042A006";
static char data_16974[] = "E1042B012";
static char data_1697E[] = "E1042B013";
static char data_16988[] = "E1042A007";
static char data_16992[] = "E1042B014";
static char data_1699C[] = "E1042A005";
static char data_169A6[] = "E1042B015";
static char data_169B0[] = "DUMMY";
static char data_169B6[] = "E1042B011";
static char data_169C0[] = "E1042A006";
static char data_169CA[] = "E1042B016";
static char data_169D4[] = "E1042B017";
static char data_169DE[] = "E1042B018";
static char data_169E8[] = "E1042A005";
static char data_169F2[] = "DUMMY";
static char data_169F8[] = "E1042B011";
static char data_16A02[] = "E1042A006";
static char data_16A0C[] = "E1042B020";
static char data_16A16[] = "E1042A008";
static char data_16A20[] = "E1042B021";
static char data_16A2A[] = "E1042B022";
static char data_16A34[] = "E1042B023";
static char data_16A3E[] = "E1042B024";
static char data_16A48[] = "E1042B025";
static char data_16A52[] = "E1042A009";
static char data_16A5C[] = "E1042B026";
static char data_16A66[] = "DUMMY";
static char *data_16A6C = data_168D8;
static char *const data_16A70 = data_168E2;
static char *const data_16A74 = data_168EC;
static char *const data_16A78 = data_168F6;
static char *const data_16A7C = data_16900;
static char *const data_16A80 = data_1690A;
static char *const data_16A84 = data_16914;
static char *const data_16A88 = data_1691E;
static char *const data_16A8C = data_16928;
static char *const data_16A90 = data_16932;
static char *const data_16A94 = data_1693C;
static char *const data_16A98 = data_16946;
static char *const data_16A9C = data_16950;
static char *const data_16AA0 = data_1695A;
static char *const data_16AA4 = data_16960;
static char *const data_16AA8 = data_1696A;
static char *const data_16AAC = data_16974;
static char *const data_16AB0 = data_1697E;
static char *const data_16AB4 = data_16988;
static char *const data_16AB8 = data_16992;
static char *const data_16ABC = data_1699C;
static char *const data_16AC0 = data_169A6;
static char *const data_16AC4 = data_169B0;
static char *const data_16AC8 = data_169B6;
static char *const data_16ACC = data_169C0;
static char *const data_16AD0 = data_169CA;
static char *const data_16AD4 = data_169D4;
static char *const data_16AD8 = data_169DE;
static char *const data_16ADC = data_169E8;
static char *const data_16AE0 = data_169F2;
static char *const data_16AE4 = data_169F8;
static char *const data_16AE8 = data_16A02;
static char *const data_16AEC = data_16A0C;
static char *const data_16AF0 = data_16A16;
static char *const data_16AF4 = data_16A20;
static char *const data_16AF8 = data_16A2A;
static char *const data_16AFC = data_16A34;
static char *const data_16B00 = data_16A3E;
static char *const data_16B04 = data_16A48;
static char *const data_16B08 = data_16A52;
static char *const data_16B0C = data_16A5C;
static char *const data_16B10 = data_16A66;
static const uint32_t data_16B14 = 25;
static const int32_t data_16B18[41] = {
  150, 250, 500, 850, 1000, 1130, 1220, 1480, 1630, 1830, 1910, 2220, -1, 25, 220, 290, 540, 690, 760, 1090, 1190, -1, 25, 220, 290, 515, 655, 845,
  -1, 25, 220, 290, 410, 460, 500, 700, 920, 1020, 1160, 1200, -1
};
static const int8_t data_16BBC[5] = {0, 0, 14, 23, 30};
static const char data_16BC1[10] __attribute__((scn_string_literal)) = "E1042A002";
static const char data_16BCB[13] __attribute__((scn_string_literal)) = "E1042A001";
static const Vector3 data_16BD8 = {.x = 16.0f, .y = 2.34f, .z = -10.0f};
static const Vector4 data_16BE4 = {.x = 1.0f, .y = 0.7f, .z = 0.7f, .w = 1.0f};
static const float data_16BF4[2] = {0.73f, 1.0f};
static const int8_t data_16BFC[24] = {0, 0, 0, 1, 3, 0, 2, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 3, 0, 2, 0, 4};
static const Vector4 data_16C14 = {.x = 0.3f, .y = 0.3f, .z = 0.3f, .w = 0.3f};
static const char data_16C24[6] __attribute__((scn_string_literal)) = "E1042";
static const char data_16C2A[6] __attribute__((scn_string_literal)) = "E1042";
static const char data_16C30[10] __attribute__((scn_string_literal)) = "Q1003B001";
static const char data_16C3A[10] __attribute__((scn_string_literal)) = "Q1003B002";
static const char data_16C44[10] __attribute__((scn_string_literal)) = "Q1003B004";
static const char data_16C4E[10] __attribute__((scn_string_literal)) = "Q1003A003";
static const char data_16C58[10] __attribute__((scn_string_literal)) = "Q1003A004";
static const char data_16C62[10] __attribute__((scn_string_literal)) = "Q1003A005";
static const char data_16C6C[10] __attribute__((scn_string_literal)) = "Q1003A001";
static const char data_16C76[10] __attribute__((scn_string_literal)) = "Q1003A002";
static const Vector3 data_16C80 = {.x = -3.762001f, .y = 0.9f, .z = -4.74130249f};
static const Vector3i data_16C8C = {.x = 0, .y = 0, .z = 0};
static const char data_16C98[] __attribute__((scn_string_literal)) = "KCDT";
static const uint8_t data_16C9D[3] = {0};
static const Vector3i data_16CA0 = {.x = 12016, .y = 63480, .z = 10818};
static const Vector3 data_16CAC = {.x = -2.5719986f, .y = 0.0f, .z = -3.87129974f};
static const Vector3 data_16CB8 = {.x = -3.40200043f, .y = 0.05f, .z = -3.91130066f};
static const Vector3 data_16CC4 = {.x = -3.40200043f, .y = 0.05f, .z = -4.011299f};
static const Vector3 data_16CD0 = {.x = -1.40200043f, .y = 0.0f, .z = -4.591301f};
static const char data_16CDC[8] __attribute__((scn_string_literal)) = "Q1003";
static const Vector3 data_16CE4 = {.x = -1.72200012f, .y = 0.74f, .z = -4.03130341f};
static const Vector3 data_16CF0 = {.x = -2.552002f, .y = 0.0f, .z = -3.851303f};
static const Vector3 data_16CFC = {.x = -0.7f, .y = 0.0f, .z = -7.24f};
static const Vector3 data_16D08 = {.x = -0.7f, .y = 0.0f, .z = -10.07f};
static const Vector3i data_16D14 = {.x = 0, .y = 49152, .z = 0};
static Vector3i data_16D20 = {.x = 0, .y = 0, .z = 29127};
static const char data_16D2C[8] __attribute__((scn_string_literal)) = "Q1003";
static const Vector3 data_16D34 = {.x = -0.7f, .y = 0.0f, .z = -5.745f};
static const Vector3 data_16D40 = {.x = -0.7f, .y = 0.0f, .z = -8.676f};
static const char data_16D4C[] __attribute__((scn_string_literal)) = "scene/02/MKSG";
static const char data_16D5A[] __attribute__((scn_string_literal)) = "SOKO_01";
static const char data_16D62[] __attribute__((scn_string_literal)) = "SOKO_02";
static const char data_16D6A[] __attribute__((scn_string_literal)) = "SOKO_04";
static const char data_16D72[] __attribute__((scn_string_literal)) = "SOKO_05";
static const char data_16D7A[] __attribute__((scn_string_literal)) = "SOKO_06";
static const char data_16D82[] __attribute__((scn_string_literal)) = "SOKO_09";
static const char data_16D8A[] __attribute__((scn_string_literal)) = "SOKO_10";
static const uint8_t data_16D92[2] = {0};
static const uint32_t data_16D94 = 0xC0880000;
static const float data_16D98[50] = {
  -4.71f, -0.7f, -4.71f, -0.7f, -0.1f, -4.25f, -0.1f, -1.0f, 3.3f, -1.8f, 1.0f, -2.3f, 4.0f, -19.56f, -22.94f, -19.56f, 0.2f, -36.18f, 0.2f, -36.18f,
  -22.94f, -1.0f, 13.7f, -26.87f, 0.0f, -18.52f, 18.0f, -39.37f, -23.03f, -39.37f, -2.21f, -66.8f, -2.21f, -66.8f, -23.03f, -1.0f, 9.6f, -52.58f,
  0.0f, -12.59f, 16.0f, 0.25f, -76.74f, 0.25f, -60.05f, -22.88f, -60.05f, -22.88f, -76.74f, -1.0f
};
static const shen_name data_16E60 = SHEN_NAME('33GA');
static const float data_16E64[74] = {
  -12.25f, 0.0f, -68.39f, 11.0f, 0.36f, -46.11f, 0.36f, -29.44f, -22.92f, -29.44f, -22.92f, -46.11f, -1.0f, 13.5f, -16.25f, 0.0f, -36.77f, 14.0f,
  -0.02f, -29.67f, -0.02f, -18.86f, -12.9f, -18.86f, -12.9f, -29.67f, -1.0f, 8.27f, -6.38f, 0.0f, -20.26f, 10.0f, -65.99f, -83.09f, -45.6f, -83.09f,
  -45.6f, -55.5f, -65.99f, -55.5f, -1.0f, 12.75f, -55.7f, 5.0f, -69.4f, 20.0f, -29.45f, -50.8f, -29.45f, -27.63f, -46.13f, -27.63f, -46.13f, -50.8f,
  -1.0f, 11.9f, -37.79f, 0.0f, -39.12f, 11.0f, -0.01f, -60.05f, -0.01f, -49.25f, -12.91f, -49.25f, -12.91f, -60.05f, -1.0f, 8.37f, -6.37f, 0.0f,
  -54.65f, 10.0f
};
static const Vector3 data_16F8C = {.x = 24.0f, .y = -1.8f, .z = -34.0f};
static const char data_16F98[13] __attribute__((scn_string_literal)) = "model/object";
static const char data_16FA5[9] __attribute__((scn_string_literal)) = "SEA3.PVR";
static const char data_16FAE[13] __attribute__((scn_string_literal)) = "model/object";
static const char data_16FBB[11] __attribute__((scn_string_literal)) = "TRSEA4.MT6";
static const char data_16FC6[13] __attribute__((scn_string_literal)) = "model/object";
static const char data_16FD3[13] __attribute__((scn_string_literal)) = "TRWTBL1.BIN";
static const Vector3 data_16FE0 = {.x = 3.0f, .y = 0.0f, .z = 0.0f};
static const Vector3 data_16FEC = {.x = -19.0f, .y = 0.0f, .z = -15.0f};
static const char data_16FF8[] __attribute__((scn_string_literal)) = "scene/02/MKSG";
static const char data_17006[] __attribute__((scn_string_literal)) = "NIGHT";
static const char data_1700C[6] __attribute__((scn_string_literal)) = "NIGHT";
static const char data_17012[1] __attribute__((scn_string_literal)) = "";
static const char data_17013[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_17018[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_1701D[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_17022[11] __attribute__((scn_string_literal)) = "bgm109.snd";
static const char data_1702D[6] __attribute__((scn_string_literal)) = "NIGHT";
static const char data_17033[7] __attribute__((scn_string_literal)) = "SELECT";
static const char data_1703A[14] __attribute__((scn_string_literal)) = "f1kyusok.snd";
static const uint32_t data_17048 = 0xFFFF0000;
static const uint8_t pad_1704C[8] = {0};
static const uint32_t data_17054 = 0x447A0000;
static const uint8_t pad_17058[4] = {0};
static const uint32_t data_1705C = 0x447A0000;
static const uint8_t pad_17060[8] = {0};
static const uint8_t data_17068[2] = {
  0xFF, 0xFF
};
static const char data_1706A[] __attribute__((scn_string_literal)) = "SOKO_01";
static const char data_17072[] __attribute__((scn_string_literal)) = "SOKO_02";
static const char data_1707A[] __attribute__((scn_string_literal)) = "SOKO_04";
static const char data_17082[] __attribute__((scn_string_literal)) = "SOKO_05";
static const char data_1708A[] __attribute__((scn_string_literal)) = "SOKO_06";
static const char data_17092[] __attribute__((scn_string_literal)) = "SOKO_09";
static const char data_1709A[] __attribute__((scn_string_literal)) = "SOKO_10";
static const char data_170A2[] __attribute__((scn_string_literal)) = "scene/02/MKSG";
static const char data_170B0[] __attribute__((scn_string_literal)) = "EVENT";
static const char data_170B6[6] __attribute__((scn_string_literal)) = "E1006";
static const Vector4 data_170BC = {.x = 0.3f, .y = 0.3f, .z = 0.3f, .w = 0.3f};
static const char data_170CC[11] __attribute__((scn_string_literal)) = "M_MOBJ.BIN";
static const char data_170D7[14] __attribute__((scn_string_literal)) = "scene/02/MFSY";
static const char data_170E5[] __attribute__((scn_string_literal)) = "CYCLEMAN.BIN";
static const uint8_t data_170F2[2] = {0};
static const Vector3 data_170F4 = {.x = 0.0f, .y = 0.0f, .z = 0.0f};
static const Vector3 data_17100 = {.x = 1.0f, .y = 1.0f, .z = 1.0f};
static const Vector4 data_1710C = {.x = 1.0f, .y = 0.83f, .z = 0.7f, .w = 0.53f};
static const float data_1711C[2] = {0.7f, 1.0f};
static const uint32_t data_17124 = 1193;
static const int32_t data_17128[35] = {
  66729, 132265, 197801, 263337, 328873, 459945, 525481, 591017, 656553, 722089, 787625, 918697, 984233, 1049769, 1115305, 1180841, 1246377, 1377449,
  1442985, 1508521, 1574057, 1639593, 1705129, 1836201, 1901737, 1967273, 2032809, 2098345, 2163881, 2294953, 2360489, 2426025, 2491561, 2557097,
  2622633
};
static const Vector4 data_171B4 = {.x = 0.18f, .y = 0.18f, .z = 0.18f, .w = 0.18f};
static const Vector4 data_171C4 = {.x = 0.18f, .y = 0.18f, .z = 0.18f, .w = 0.18f};
static const Vector3 data_171D4 = {.x = 0.102f, .y = -0.043f, .z = 0.026f};
static const Vector3i data_171E0 = {.x = 4824, .y = 17293, .z = 0};
static const char data_171EC[7] __attribute__((scn_string_literal)) = "SELECT";
static const char data_171F3[11] __attribute__((scn_string_literal)) = "ykm005.snd";
static const char data_171FE[10] __attribute__((scn_string_literal)) = "E1006A028";
static const char data_17208[12] __attribute__((scn_string_literal)) = "E1006A029";
static const uint32_t data_17214 = 1;
static const float data_17218[3] = {-16.5f, -20.8f, 0.0f};
static const int32_t data_17224 = 196608;
static const Vector3 data_17228 = {.x = -12.61f, .y = 1.79f, .z = -25.06f};
static const char data_17234[10] __attribute__((scn_string_literal)) = "E1006A024";
static const char data_1723E[10] __attribute__((scn_string_literal)) = "E1006A006";
static const char data_17248[12] __attribute__((scn_string_literal)) = "E1006A014";
static const Vector3i data_17254 = {.x = 0, .y = 0, .z = 0};
static const uint32_t data_17260 = 0xC2371773;
static const float data_17264 = 2.4825f;
static const uint32_t data_17268 = 0xC280801AU;
static const Vector3 data_1726C = {.x = -45.7729f, .y = 2.0825f, .z = -64.250198f};
static const Vector3 data_17278 = {.x = -42.91f, .y = 0.0f, .z = -65.69f};
static const char data_17284[12] __attribute__((scn_string_literal)) = "E1006A010";
static const Vector3 data_17290 = {.x = -2.56f, .y = 0.0f, .z = -7.08f};
static const Vector3 data_1729C = {.x = -2.98465f, .y = 0.0f, .z = -7.08242f};
static const char data_172A8[12] __attribute__((scn_string_literal)) = "E1006A026";
static const uint32_t data_172B4 = 1;
typedef struct Row_172B8 { float w0; float w1; float w2; int32_t w3; } Row_172B8;
static const Row_172B8 data_172B8[2] = {
  {-5.37f, -7.1f, 1.0f, 2},
  {-5.4f, -1.0f, 0.0f, 196608}
};
static const uint32_t data_172D8 = 0xFFFF0000;
static const uint32_t data_172DC[3] = {0, 0x638E8000U, 0xC1EB48E9U};
static const float data_172E8 = 2.6197f;
static const uint32_t data_172EC[3] = {0xC1B7ACA5U, 0, 0};
static const int32_t data_172F8[2] = {-65536, 1};
static const uint32_t data_17300[2] = {0x71C78000U, 0xC24C6282U};
static const float data_17308[4] = {2.6297f, -23.9177f, 0.0f, 0.0f};
static const int32_t data_17318[2] = {-65536, 2};
static const uint32_t data_17320[2] = {0x71C70000U, 0xC26E92BDU};
static const float data_17328 = 2.2547f;
static const uint32_t data_1732C[3] = {0xC1D3FCBAU, 0, 0};
static const int32_t data_17338[2] = {-65536, 3};
static const uint32_t data_17340[2] = {0x638EC000U, 0xC1B77B15U};
static const float data_17348[4] = {2.3097f, -69.8284f, 0.0f, 0.0f};
static const int32_t data_17358[2] = {-65536, 4};
static const uint32_t data_17360 = 0x638EC000U;
static const float data_17364[2] = {-22.9809f, 2.5622f};
static const uint32_t data_1736C[3] = {0xC2111A02U, 0, 0};
static const int32_t data_17378[2] = {-65536, 5};
static const uint32_t data_17380[2] = {0x638EC000U, 0xC14F6389U};
static const float data_17388 = 1.7722f;
static const uint32_t data_1738C[3] = {0xC1C870D8U, 0, 0};
static const int32_t data_17398[2] = {-65536, 6};
static const uint32_t data_173A0[2] = {0x638E4000U, 0xC1DD5014U};
static const float data_173A8 = 1.7422f;
static const uint32_t data_173AC[3] = {0xC298C090U, 0, 0};
static const int32_t data_173B8[2] = {-65536, 7};
static const uint32_t data_173C0[2] = {0x638E4000U, 0xC235535BU};
static const float data_173C8 = 2.3046f;
static const uint32_t data_173CC[3] = {0xC2807375U, 0, 0};
static const int32_t data_173D8[2] = {-65536, 8};
static const uint32_t data_173E0[2] = {0x638E8000U, 0xC21153C3U};
static const float data_173E8[4] = {2.3872f, -50.8522f, 0.0f, 0.0f};
static const int32_t data_173F8[2] = {-65536, 9};
static const uint32_t data_17400 = 0x638EC000U;
static const float data_17404[2] = {-12.9118f, 2.0697f};
static const uint32_t data_1740C[3] = {0xC25EA0AAU, 0, 0};
static char data_17418[] = "E1006A001";
static char data_17422[] = "E1006A002";
static char data_1742C[] = "E1006A003";
static char data_17436[] = "E1006A004";
static char data_17440[] = "E1006A005";
static char data_1744A[] = "E1006A006";
static char data_17454[] = "E1006A007";
static char data_1745E[] = "E1006A010";
static char data_17468[] = "E1006A008";
static char data_17472[] = "E1006A009";
static char *data_1747C = data_17418;
static char *const data_17480 = data_17422;
static char *const data_17484 = data_1742C;
static char *const data_17488 = data_17436;
static char *const data_1748C = data_17440;
static char *const data_17490 = data_1744A;
static char *const data_17494 = data_17454;
static char *const data_17498 = data_1745E;
static char *const data_1749C = data_17468;
static char *const data_174A0 = data_17472;
static const uint32_t data_174A4 = 0xC1EB48E9;
static const float data_174A8[2] = {2.8197f, -22.6593f};
static const uint32_t data_174B0 = 0xC24C6282U;
static const float data_174B4 = 3.4297f;
static const uint32_t data_174B8[2] = {0xC1B4F10DU, 0xC26E92BDU};
static const float data_174C0 = 2.7547f;
static const uint32_t data_174C4[2] = {0xC1DE6320U, 0xC1B514AFU};
static const float data_174CC[2] = {2.5097f, -69.8284f};
static const uint32_t data_174D4 = 0xC1B5727CU;
static const float data_174D8 = 2.6622f;
static const uint32_t data_174DC = 0xC2111A02U;
static const float data_174E0[2] = {-12.6618f, 1.7722f};
static const uint32_t data_174E8 = 0xC1C870D8U;
static const float data_174EC[2] = {-27.9641f, 1.8422f};
static const uint32_t data_174F4 = 0xC298C090U;
static const float data_174F8[2] = {-45.7729f, 2.4825f};
static const uint32_t data_17500[2] = {0xC280801AU, 0xC21153C3U};
static const float data_17508[4] = {2.4872f, -50.5522f, -12.6118f, 2.1697f};
static const uint32_t data_17518 = 0xC25EA0AAU;
static const uint32_t data_1751C = 0x980097;
static const int32_t data_17520[4] = {10092697, 10223771, 10354845, 10485919};
static const int8_t data_17530[12] = {2, 2, 0, 3, 3, 3, 1, 1, 2, 3, 0, 0};
static const uint32_t data_1753C = 0xC19E51EC;
static const float data_17540[119] = {
  -22.61f, -19.79f, -22.61f, -36.0f, -22.5f, -36.0f, -22.5f, -39.6f, -24.0f, -39.6f, -24.0f, -39.6f, -2.7f, -39.6f, -2.7f, -66.6f, -22.5f, -66.6f,
  -22.5f, -66.6f, -2.7f, -66.6f, -2.7f, -56.84f, 0.0f, -56.84f, 0.0f, -53.1f, -26.66f, -53.1f, -26.66f, -45.9f, -27.88f, -45.9f, -27.88f, -53.1f,
  -50.4f, -53.1f, -50.4f, -7.7f, -48.25f, -5.16f, -48.25f, -45.9f, -50.38f, -45.9f, -50.38f, -45.72f, -55.8f, -45.72f, -55.8f, -40.5f, -66.6f, -40.5f,
  -66.6f, -40.5f, -55.8f, -40.5f, -55.8f, -27.9f, -55.8f, -27.9f, -55.8f, -40.5f, -72.0f, -40.5f, -72.0f, -27.9f, -72.0f, -27.9f, -72.0f, -25.8f,
  -78.3f, -25.8f, -78.3f, -22.49f, -76.5f, -22.49f, -76.5f, -16.2f, -56.1f, -16.2f, -56.1f, -12.6f, -49.5f, -12.6f, -49.5f, -22.65f, -45.9f, -22.65f,
  -45.9f, -29.69f, -27.88f, -29.69f, -27.88f, -22.5f, -29.59f, -22.5f, -29.59f, -12.6f, -18.9f, -12.6f, -18.9f, -20.89f, -56.09f, -42.3f, -24.0f,
  -48.0f, -47.4f, -48.3f, -42.9f, -21.3f, -48.0f, -16.8f, -48.0f, -27.6f, -45.0f, -55.65f, -26.66f
};
static const int8_t data_1771C[60] = {
  0, 1, 2, 3, 0, 1, 6, 7, 2, 3, 4, 5, 2, 3, 6, 7, 4, 5, 0, 1, 4, 7, 2, 3, 6, 7, 2, 3, 4, 5, 6, 7, 4, 5, 6, 7, 6, 7, 2, 3, 6, 7, 4, 5, 2, 3, 6, 7, 4,
  5, 4, 5, 5, 3, 2, 5, 3, 0, 6, 4
};
static const int8_t data_17758[60] = {
  0, 0, 0, 10, 10, 10, 20, 10, 0, 20, 20, 0, 20, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 0, 10, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};
static const uint32_t data_17794 = 0xC19E6666;
static const float data_17798[39] = {
  -17.47f, 1.41f, -1.0f, -19.8f, -14.81f, 1.96f, 1.0f, -29.7f, -37.17f, 1.58f, -1.8f, -29.7f, -34.52f, 1.8f, 1.5f, -59.81f, -27.79f, -1.5f, 1.55f,
  -59.21f, -27.8f, 1.5f, 1.6f, -53.1f, -37.2f, 1.2f, -1.0f, -53.1f, -33.0f, 1.8f, 1.5f, -17.88f, -76.5f, -1.2f, -1.83f, -15.83f, -76.5f, 1.0f, -1.07f
};
static const int8_t data_17834[10] = {1, 6, 1, 6, 4, 7, 1, 6, 3, 0};
static const char data_1783E[10] __attribute__((scn_string_literal)) = "E1006A025";
static const char data_17848[10] __attribute__((scn_string_literal)) = "E1006A025";
static const char data_17852[10] __attribute__((scn_string_literal)) = "E1006A015";

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
    *(int32_t *)&local_F8 = Script_ScenDataUtil(data_13DB0, 5, Script_ScenDataUtil(data_137C0, 1, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 5);
    *(int32_t *)&local_F4 = Script_ScenDataUtil(data_13DB0, 5, Script_ScenDataUtil(data_137C0, 1, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 6);
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
    *(int32_t *)&local_F4 = Script_ScenDataUtil(data_13DB0, 5, Script_ScenDataUtil(data_137C0, 1, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 5);
    local_F4 = 90.0f - local_F4;
    *(int32_t *)&local_F0 = Script_ScenDataUtil(data_13DB0, 5, Script_ScenDataUtil(data_137C0, 1, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 6);
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
    *(int32_t *)&local_F8 = Script_ScenDataUtil(data_13DB0, 5, Script_ScenDataUtil(data_137C0, 1, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 5);
    *(int32_t *)&local_F4 = Script_ScenDataUtil(data_13DB0, 5, Script_ScenDataUtil(data_137C0, 1, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 6);
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
    *(int32_t *)&local_F8 = Script_ScenDataUtil(data_13DB0, 5, Script_ScenDataUtil(data_137C0, 1, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 5);
    *(int32_t *)&local_F4 = Script_ScenDataUtil(data_13DB0, 5, Script_ScenDataUtil(data_137C0, 1, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 6);
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
      *(int32_t *)&local_F4 = Script_ScenDataUtil(data_13DB0, 5, Script_ScenDataUtil(data_137C0, 1, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 5);
      *(int32_t *)&local_F0 = Script_ScenDataUtil(data_13DB0, 5, Script_ScenDataUtil(data_137C0, 1, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 6);
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
        *(int32_t *)&local_F4 = Script_ScenDataUtil(data_13DB0, 5, Script_ScenDataUtil(data_137C0, 1, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 5);
        *(int32_t *)&local_F0 = Script_ScenDataUtil(data_13DB0, 5, Script_ScenDataUtil(data_137C0, 1, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 6);
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
    *(int32_t *)&local_F8 = Script_ScenDataUtil(data_13DB0, 5, Script_ScenDataUtil(data_137C0, 1, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 5);
    *(int32_t *)&local_F4 = Script_ScenDataUtil(data_13DB0, 5, Script_ScenDataUtil(data_137C0, 1, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 6);
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
    *(int32_t *)&local_F8 = Script_ScenDataUtil(data_13DB0, 5, Script_ScenDataUtil(data_137C0, 1, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 5);
    *(int32_t *)&local_F4 = Script_ScenDataUtil(data_13DB0, 5, Script_ScenDataUtil(data_137C0, 1, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 6);
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
      *(int32_t *)&local_FF78 = Script_ScenDataUtil(data_13DB0, 5, Script_ScenDataUtil(data_137C0, 1, arg3 * 40 + task_98[arg2] * 4 + local_FF7F) * 9 + 5);
      *(int32_t *)&local_FF74 = Script_ScenDataUtil(data_13DB0, 5, Script_ScenDataUtil(data_137C0, 1, arg3 * 40 + task_98[arg2] * 4 + local_FF7F) * 9 + 6);
      local_FF7F = script_8C20(arg0, arg1, 5, local_FF78 / (float)100, local_FF74 / (float)100, arg3, -1);
      script_92E9(arg0, 0);
      *(int32_t *)&local_94 = Script_ScenDataUtil(data_13DB0, 5, Script_ScenDataUtil(data_137C0, 1, arg3 * 40 + task_98[arg2] * 4 + local_FF7F) * 9 + 1);
      *(int32_t *)&local_90 = Script_ScenDataUtil(data_13DB0, 5, Script_ScenDataUtil(data_137C0, 1, arg3 * 40 + task_98[arg2] * 4 + local_FF7F) * 9 + 2);
      local_8C = 0.0f;
      local_FC = Script_ScenDataUtil(data_13DB0, 4, Script_ScenDataUtil(data_137C0, 1, arg3 * 40 + task_98[arg2] * 4 + local_FF7F) * 9);
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
    *(int32_t *)&local_F8 = Script_ScenDataUtil(data_13DB0, 5, Script_ScenDataUtil(data_137C0, 1, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 5);
    local_F8 = 98.0f - local_F8;
    local_F4 = Script_ScenDataUtil(data_13DB0, 5, Script_ScenDataUtil(data_137C0, 1, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 6);
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
      *(int32_t *)&local_D0 = Script_ScenDataUtil(&data_147D0, 5, Script_ScenDataUtil(task_0, 4, task_48[arg2] * 13 + 6) * 5 + 3);
      *(int32_t *)&local_CC = Script_ScenDataUtil(&data_147D0, 5, Script_ScenDataUtil(task_0, 4, task_48[arg2] * 13 + 6) * 5 + 4);
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
          *(int32_t *)&local_D0 = Script_ScenDataUtil(&data_147D0, 5, Script_ScenDataUtil(task_0, 4, task_48[arg2] * 13 + 6) * 5 + 3);
          *(int32_t *)&local_CC = Script_ScenDataUtil(&data_147D0, 5, Script_ScenDataUtil(task_0, 4, task_48[arg2] * 13 + 6) * 5 + 4);
          local_FC = script_8C20(arg0, arg1, local_F8, local_D0 / (float)100, local_CC / (float)100, arg3, task_48[arg2]);
          Script_scenUtils(SCENARIO_CMD_INIT_SCNT);
          script_6BDE(arg0, arg1, arg2, arg3, local_FC, 0);
          Script_scenUtils(SCENARIO_CMD_UPDATE_TIMER);
        } else {
          *(int32_t *)&local_D0 = Script_ScenDataUtil(&data_147D0, 5, Script_ScenDataUtil(task_0, 4, task_48[arg2] * 13 + 6) * 5 + 3);
          *(int32_t *)&local_CC = Script_ScenDataUtil(&data_147D0, 5, Script_ScenDataUtil(task_0, 4, task_48[arg2] * 13 + 6) * 5 + 4);
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

__attribute__((scn_string_bindings(0, data_152EE)))
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
  *(int32_t *)&local_98 = Script_ScenDataUtil(data_13DB0, 5, Script_ScenDataUtil(data_137C0, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 1);
  *(int32_t *)&local_94 = Script_ScenDataUtil(data_13DB0, 5, Script_ScenDataUtil(data_137C0, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 2);
  *(int32_t *)&local_90 = Script_ScenDataUtil(data_13DB0, 5, Script_ScenDataUtil(data_137C0, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 3);
  *(int32_t *)&local_8C = Script_ScenDataUtil(data_13DB0, 5, Script_ScenDataUtil(data_137C0, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 4);
  *(int32_t *)&local_88 = Script_ScenDataUtil(data_13DB0, 5, Script_ScenDataUtil(data_137C0, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 7);
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
      local_FF78 = Script_ScenDataUtil(data_13DB0, 4, Script_ScenDataUtil(data_137C0, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
      local_FF7C = 18;
      local_80 = 18;
      local_A4 = 1.0f;
    } else {
      if (arg0 == task_160) {
        local_FF78 = Script_ScenDataUtil(data_13DB0, 4, Script_ScenDataUtil(data_137C0, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9) | -0x80000000;
        local_FF7C = 12;
        local_80 = 12;
      } else {
        local_FF78 = Script_ScenDataUtil(data_13DB0, 4, Script_ScenDataUtil(data_137C0, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
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
          *(int32_t *)&local_9C = Script_ScenDataUtil(data_13DB0, 5, Script_ScenDataUtil(data_137C0, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 8);
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

__attribute__((scn_string_bindings(0, data_1530A)))
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
          local_FC = Script_ScenDataUtil(data_13DB0, 4, Script_ScenDataUtil(data_137C0, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
          break;
        case 0:
          local_B7 = 0;
          local_F8 = 12;
          local_F0 = 0x4000;
          local_B8 = 1.0f;
          local_FC = Script_ScenDataUtil(data_13DB0, 4, Script_ScenDataUtil(data_137C0, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9) | -0x80000000;
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
          local_FC = Script_ScenDataUtil(data_13DB0, 4, Script_ScenDataUtil(data_137C0, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9) | -0x80000000;
          break;
        case 0:
          local_B7 = 1;
          local_F8 = 18;
          local_F0 = 0xC000;
          local_B8 = 1.0f;
          local_FC = Script_ScenDataUtil(data_13DB0, 4, Script_ScenDataUtil(data_137C0, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
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
          local_FC = Script_ScenDataUtil(data_13DB0, 4, Script_ScenDataUtil(data_137C0, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9) | -0x80000000;
          break;
        case 0:
          local_B7 = 0;
          local_F8 = 18;
          local_F0 = 0xC000;
          local_B8 = -1.0f;
          local_FC = Script_ScenDataUtil(data_13DB0, 4, Script_ScenDataUtil(data_137C0, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
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
          local_FC = Script_ScenDataUtil(data_13DB0, 4, Script_ScenDataUtil(data_137C0, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
          break;
        case 0:
          local_B7 = 1;
          local_F8 = 12;
          local_F0 = 0x4000;
          local_B8 = -1.0f;
          local_FC = Script_ScenDataUtil(data_13DB0, 4, Script_ScenDataUtil(data_137C0, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9) | -0x80000000;
          break;
      }
      break;
  }
  *(int32_t *)&local_C4 = Script_ScenDataUtil(data_13DB0, 5, Script_ScenDataUtil(data_137C0, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 1);
  *(int32_t *)&local_C0 = Script_ScenDataUtil(data_13DB0, 5, Script_ScenDataUtil(data_137C0, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 2);
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

__attribute__((scn_string_bindings(0, data_15326)))
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
            local_A4 = Script_ScenDataUtil(data_13DB0, 4, Script_ScenDataUtil(data_137C0, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
            local_C0 = 1.0f;
            local_A0 = 18;
            local_9C = 18;
            local_90 = 15473;
            local_8C = 17294;
            break;
          case 0:
            local_CF = 0;
            local_A4 = Script_ScenDataUtil(data_13DB0, 4, Script_ScenDataUtil(data_137C0, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9) | -0x80000000;
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
            local_A4 = Script_ScenDataUtil(data_13DB0, 4, Script_ScenDataUtil(data_137C0, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
            local_C0 = 1.0f;
            local_A0 = 18;
            local_9C = 18;
            local_90 = 13653;
            break;
          case 0:
            local_CF = 0;
            local_A4 = Script_ScenDataUtil(data_13DB0, 4, Script_ScenDataUtil(data_137C0, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
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
            local_A4 = Script_ScenDataUtil(data_13DB0, 4, Script_ScenDataUtil(data_137C0, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9) | -0x80000000;
            local_C0 = 1.0f;
            local_A0 = 12;
            local_9C = 18;
            local_90 = 50062;
            local_8C = 48241;
            break;
          case 0:
            local_CF = 1;
            local_A4 = Script_ScenDataUtil(data_13DB0, 4, Script_ScenDataUtil(data_137C0, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
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
            local_A4 = Script_ScenDataUtil(data_13DB0, 4, Script_ScenDataUtil(data_137C0, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
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
            local_A4 = Script_ScenDataUtil(data_13DB0, 4, Script_ScenDataUtil(data_137C0, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
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
            local_A4 = Script_ScenDataUtil(data_13DB0, 4, Script_ScenDataUtil(data_137C0, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9) | -0x80000000;
            local_C0 = -1.0f;
            local_A0 = 12;
            local_9C = 12;
            local_90 = 50062;
            local_8C = 48241;
            break;
          case 0:
            local_CF = 0;
            local_A4 = Script_ScenDataUtil(data_13DB0, 4, Script_ScenDataUtil(data_137C0, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
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
            local_A4 = Script_ScenDataUtil(data_13DB0, 4, Script_ScenDataUtil(data_137C0, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
            local_C0 = -1.0f;
            local_A0 = 18;
            local_9C = 18;
            local_90 = 51882;
            break;
          case 0:
            local_CF = 0;
            local_A4 = Script_ScenDataUtil(data_13DB0, 4, Script_ScenDataUtil(data_137C0, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
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
            local_A4 = Script_ScenDataUtil(data_13DB0, 4, Script_ScenDataUtil(data_137C0, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
            local_C0 = -1.0f;
            local_A0 = 18;
            local_9C = 12;
            local_90 = 15473;
            local_8C = 17294;
            break;
          case 0:
            local_CF = 1;
            local_A4 = Script_ScenDataUtil(data_13DB0, 4, Script_ScenDataUtil(data_137C0, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9) | -0x80000000;
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
            local_A4 = Script_ScenDataUtil(data_13DB0, 4, Script_ScenDataUtil(data_137C0, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
            local_C0 = -1.0f;
            local_A0 = 18;
            local_9C = 18;
            local_90 = 13653;
            break;
          case 0:
            local_CF = 1;
            local_A4 = Script_ScenDataUtil(data_13DB0, 4, Script_ScenDataUtil(data_137C0, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
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
              local_A4 = Script_ScenDataUtil(data_13DB0, 4, Script_ScenDataUtil(data_137C0, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
              local_C0 = -1.0f;
              local_A0 = 18;
              local_9C = 12;
              local_90 = 48241;
              local_8C = 50062;
              break;
            case 1:
              local_CF = 1;
              local_A4 = Script_ScenDataUtil(data_13DB0, 4, Script_ScenDataUtil(data_137C0, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9) | -0x80000000;
              local_C0 = -1.0f;
              local_A0 = 12;
              local_9C = 12;
              local_90 = 50062;
              local_8C = 48241;
              break;
            case 2:
              local_CF = 1;
              local_A4 = Script_ScenDataUtil(data_13DB0, 4, Script_ScenDataUtil(data_137C0, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
              local_C0 = 1.0f;
              local_A0 = 18;
              local_9C = 18;
              local_90 = 15473;
              local_8C = 17294;
              break;
            case 3:
              local_CF = 0;
              local_A4 = Script_ScenDataUtil(data_13DB0, 4, Script_ScenDataUtil(data_137C0, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9) | -0x80000000;
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
              local_A4 = Script_ScenDataUtil(data_13DB0, 4, Script_ScenDataUtil(data_137C0, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
              local_C0 = -1.0f;
              local_A0 = 18;
              local_9C = 18;
              local_90 = 45511;
              break;
            case 1:
              local_CF = 1;
              local_A4 = Script_ScenDataUtil(data_13DB0, 4, Script_ScenDataUtil(data_137C0, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
              local_C0 = -1.0f;
              local_A0 = 18;
              local_9C = 18;
              local_90 = 51882;
              break;
            case 2:
              local_CF = 1;
              local_A4 = Script_ScenDataUtil(data_13DB0, 4, Script_ScenDataUtil(data_137C0, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
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
              local_A4 = Script_ScenDataUtil(data_13DB0, 4, Script_ScenDataUtil(data_137C0, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
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
  *(int32_t *)&local_B8 = Script_ScenDataUtil(data_13DB0, 5, Script_ScenDataUtil(data_137C0, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 1);
  *(int32_t *)&local_B4 = Script_ScenDataUtil(data_13DB0, 5, Script_ScenDataUtil(data_137C0, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 2);
  *(int32_t *)&local_B0 = Script_ScenDataUtil(data_13DB0, 5, Script_ScenDataUtil(data_137C0, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 3);
  *(int32_t *)&local_AC = Script_ScenDataUtil(data_13DB0, 5, Script_ScenDataUtil(data_137C0, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 4);
  *(int32_t *)&local_A8 = Script_ScenDataUtil(data_13DB0, 5, Script_ScenDataUtil(data_137C0, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 7);
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
          *(int32_t *)&local_BC = Script_ScenDataUtil(data_13DB0, 5, Script_ScenDataUtil(data_137C0, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 8);
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

__attribute__((scn_string_bindings(0, data_15342)))
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

__attribute__((scn_string_bindings(0, data_1535E)))
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
      local_8C = Script_ScenDataUtil(data_13DB0, 4, Script_ScenDataUtil(data_137C0, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
      local_A0 = -1.0f;
      local_88 = 18;
      local_84 = 18;
      local_FF78 = 46421;
      local_FF74 = 48787;
      break;
    case 1:
      local_AF = 1;
      local_8C = Script_ScenDataUtil(data_13DB0, 4, Script_ScenDataUtil(data_137C0, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9) | -0x80000000;
      local_A0 = -1.0f;
      local_88 = 12;
      local_84 = 12;
      local_FF78 = 50426;
      local_FF74 = 48969;
      break;
    case 2:
      local_AF = 1;
      local_8C = Script_ScenDataUtil(data_13DB0, 4, Script_ScenDataUtil(data_137C0, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
      local_A0 = 1.0f;
      local_88 = 18;
      local_84 = 18;
      local_FF78 = 15109;
      local_FF74 = 16566;
      break;
    case 3:
      local_AF = 0;
      local_8C = Script_ScenDataUtil(data_13DB0, 4, Script_ScenDataUtil(data_137C0, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9) | -0x80000000;
      local_A0 = 1.0f;
      local_88 = 12;
      local_84 = 12;
      local_FF78 = 19114;
      local_FF74 = 16748;
      break;
  }
  (void)local_FF78;
  *(int32_t *)&local_9C = Script_ScenDataUtil(data_13DB0, 5, Script_ScenDataUtil(data_137C0, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 1);
  *(int32_t *)&local_98 = Script_ScenDataUtil(data_13DB0, 5, Script_ScenDataUtil(data_137C0, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 2);
  *(int32_t *)&local_94 = Script_ScenDataUtil(data_13DB0, 5, Script_ScenDataUtil(data_137C0, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 3);
  *(int32_t *)&local_90 = Script_ScenDataUtil(data_13DB0, 5, Script_ScenDataUtil(data_137C0, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 4);
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

__attribute__((scn_string_bindings(0, data_1537A)))
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
        local_E0 = Script_ScenDataUtil(&data_147D0, 4, local_D4 * 5) | -0x80000000;
        break;
      case 0:
        local_E0 = Script_ScenDataUtil(&data_147D0, 4, local_D4 * 5);
        break;
    }
  } else if ((local_CC == 2) | (local_CC == 4) | (local_CC == 6) | (local_CC == 8) | (local_CC == 10) | (local_CC == 19) | (local_CC == 22) | (local_CC == 24) | (local_CC == 26) | (local_CC == 28)) {
    local_CC = arg4;
    switch (local_CC) {
      case 1:
        local_E0 = Script_ScenDataUtil(&data_147D0, 4, local_D4 * 5);
        break;
      case 0:
        local_E0 = Script_ScenDataUtil(&data_147D0, 4, local_D4 * 5) | -0x80000000;
        break;
    }
  } else {
    if ((local_CC == 13) | (local_CC == 14) | (local_CC == 15) | (local_CC == 16) | (local_CC == 20) | (local_CC == 23) | (local_CC == 29)) {
      local_CC = arg4;
      switch (local_CC) {
        case 0:
          local_E0 = Script_ScenDataUtil(&data_147D0, 4, local_D4 * 5);
          break;
        case 1:
          local_E0 = Script_ScenDataUtil(&data_147D0, 4, local_D4 * 5) | -0x80000000;
          break;
        case 2:
          local_E0 = Script_ScenDataUtil(&data_147D0, 4, local_D4 * 5);
          break;
        case 3:
          local_E0 = Script_ScenDataUtil(&data_147D0, 4, local_D4 * 5) | -0x80000000;
          break;
      }
      goto L006988;
    }
  }
  L006988:;
  *(int32_t *)&local_EC = Script_ScenDataUtil(&data_147D0, 5, local_D4 * 5 + 1);
  *(int32_t *)&local_E8 = Script_ScenDataUtil(&data_147D0, 5, local_D4 * 5 + 2);
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

__attribute__((scn_string_bindings(0, data_15396)))
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
  DSWK_BindTablesAndInitState(arg1, arg2, arg3, arg4, &task_10, task_20, task_48, (shen_name *)task_70, task_98, arg0, arg5, data_137C0, data_13DB0, (shen_name *)task_E8);
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

__attribute__((scn_string_bindings(0, data_15421, 1, data_1543A)))
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

__attribute__((scn_string_bindings(0, data_15453)))
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

__attribute__((scn_string_bindings(0, data_15462)))
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

__attribute__((scn_string_bindings(0, data_15471, 1, data_15483)))
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
      task_110[arg0] = Script_scenCreateTask(Script_ScenDataUtil(&data_14870, 8, local_F8), 4, task_70[arg0], task_E8[arg0], arg0, local_F8);
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
    task_110[arg0] = Script_scenCreateTask(Script_ScenDataUtil(&data_14870, 8, arg2), 4, task_70[arg0], task_E8[arg0], arg0, arg2);
    task_C0[arg0] = task_70[arg0];
    script_8A20(arg0);
  }
  return;
}

__attribute__((scn_string_bindings(0, data_15496, 1, data_154A8)))
static void script_9D79(int32_t arg0, int32_t arg1) {
  if ((task_110[arg0] == 0) & (arg1 >= 0)) {
    script_931B(task_70[arg0], task_E8[arg0], task_48[arg0], arg1);
    task_110[arg0] = Script_scenCreateTask(Script_ScenDataUtil(&data_14870, 8, arg1), 4, task_70[arg0], task_E8[arg0], arg0, arg1);
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

static void script_9EB2(int32_t arg0, float arg1, float arg2, float arg3) {
  Vector3 local_F4;
  local_F4.x = arg1;
  (&local_F4)->y = arg2;
  (&local_F4)->z = arg3;
  Script_EVT_SetPosition(arg0, POS_XYZ, &local_F4);
  return;
}

static void script_9EDA(int32_t arg0, float arg1, float arg2, float arg3) {
  Vector3i local_F4;
  local_F4.x = (int32_t)(arg1 / 360.0f * (float)0x10000);
  (&local_F4)->y = (int32_t)(arg2 / 360.0f * (float)0x10000);
  (&local_F4)->z = (int32_t)(arg3 / 360.0f * (float)0x10000);
  Script_EVT_SetRotation(arg0, 0x38000000, &local_F4, 0);
  return;
}

static void script_9F2E(int32_t arg0, int32_t arg1, int32_t arg2) {
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

static int32_t script_9F7E(int32_t arg0) {
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

static void script_A025(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  local_FC = script_9F7E(arg0);
  if (arg1 > local_FC) {
    (void)0;
  } else {
    (void)-1;
  }
  return;
}

static int32_t script_A048(int32_t arg0) {
  int32_t unused_FC;
  GameDateTime local_F4;
  int32_t local_F0;
  int32_t local_EC;
  Script_ReturnTimeDate(&local_F4);
  local_F0 = (&local_F4)->hours * 60 + (&local_F4)->minutes;
  local_EC = Script_scenUtils(SCENARIO_CMD_GET_STEP_LUT, arg0) * 6;
  return local_F0 - local_EC;
}

static void script_A07B(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  local_FC = script_A048(arg0);
  if (arg1 > local_FC) {
    (void)0;
  } else {
    (void)-1;
  }
  return;
}

static void script_A09E(int32_t arg0, int32_t arg1) {
  int8_t local_F4[12];
  int32_t local_F0;
  local_F0 = script_A048(arg0);
  local_F4[4] = local_F0 / 60;
  local_F4[5] = local_F0 % 60;
  if ((local_F4[4] < *(int8_t *)(arg1 + 4)) | ((local_F4[4] == *(int8_t *)(arg1 + 4)) & (local_F4[5] < *(int8_t *)(arg1 + 5)))) {
    (void)0;
  } else {
    (void)-1;
  }
  return;
}

static void script_A0FD(int32_t arg0) {
  int32_t local_F8[2];
  local_F8[0] = arg0;
  local_F8[1] = 0;
  (void)local_F8;
  return;
}

static void script_A112(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
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

static void script_A4A8(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
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

__attribute__((scn_string_bindings(0, data_15B08)))
static void script_AB36(void) {
  Script_sprintf("TUNED FreeTalk def\012");
  SCNT_Enqueue();
  return;
}

__attribute__((scn_string_bindings(0, data_15B1C)))
static void script_AB45(void) {
  Script_sprintf("TUNED FreeTalk def\012");
  SCNT_Enqueue();
  return;
}

static void script_AB54(void) {
  return;
}

static void script_AB57(void) {
  return;
}

static void script_AB5A(void) {
  return;
}

static void script_AB5D(void) {
  return;
}

static void script_AB60(void) {
  (void)((task_1AC = 0) & 0x20);
  return;
}

static void script_AB6D(void) {
  task_1A0 = Script_freeTalkGetCurrentChara();
  return;
}

static void script_AB79(void) {
  *(&task_1A0 + 1) = Script_GetFreeTalkStatus();
  return;
}

static int32_t script_AB88(void) {
  return *(&task_1A0 + 2) = Script_freeTalkGetAndResetState();
}

static void script_AB97(void) {
  int32_t local_FC;
  local_FC = Script_GetSceneID();
  switch (local_FC) {
    case 1:
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_ENTRY2_TIMED_EVENT_DONE) == 0) {
        Script_TimedTransitionUtils(TIMED_TRANSITION_REGISTER, 1, 0x3044484A, 2, &data_15BA0, 0xD5A6C3, TimedTransitionUpdateMemoItem150);
      }
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JD00_ENTRY20_TIMED_EVENT_DONE) == 0) {
        Script_TimedTransitionUtils(TIMED_TRANSITION_REGISTER, 1, 0x3030444A, 20, &data_15B68, 0xDA3AA3, 0);
      }
      break;
    case 2:
      if ((Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_GMCT_ROUTE_STARTED) == 0) | (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_GMCT_ROUTE_ADVANCED) == 0)) {
        Script_TimedTransitionUtils(TIMED_TRANSITION_REGISTER, 2, 0x54434D47, 1, &data_15B30, 0xB7420F, 0);
      }
      break;
    case 3:
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOZOMI_RESCUE_RUSH_ACTIVE) == 0) {
        Script_TimedTransitionUtils(TIMED_TRANSITION_REGISTER, 3, 0x4F4D4F4A, 20, &data_15B4C, 0x2DC9FE, TimedTransitionRollbackToPreviousDay2330);
      }
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_FIRED_FROM_JOB) & ~Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 970)) {
        if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_MFSY_ENTRY42_TIMED_EVENT_DONE) == 0) {
          Script_TimedTransitionUtils(TIMED_TRANSITION_REGISTER, 3, 0x5953464D, 42, &data_15B84, 0xBBB817, 0);
        }
      }
      break;
  }
  return;
}

static void script_ACCF(int32_t arg0) {
  script_ACE7(arg0);
  return;
}

static void script_ACDB(int32_t arg0) {
  script_ACE7(arg0);
  return;
}

static void script_ACE7(int32_t arg0) {
  if (Script_StepUtils(GET_STEP_HIGHER, 0) == 22) {
    int32_t local_FC;
    local_FC = Script_StepUtils(GET_STEP_LOWER, 0);
    if ((local_FC == 2) | (local_FC == 3)) {
      Script_SetShouldSleepFlag(0);
      goto L00AD20;
    }
  }
  L00AD20:;
  Script_ryoSetChara(arg0);
  Script_facemotUtils(arg0, 1, 0);
  Script_facemotUtils(arg0, 0, 0);
  Script_FaceEx(arg0, 0, 0, 2);
  Script_CharHandlers(arg0, -1, 0, 0);
  script_AB97();
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

static void script_ADDA(int32_t arg0) {
  script_ACDB(arg0);
  for (;;) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_ADF1(int32_t arg0) {
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

static void script_AE32(int32_t arg0) {
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

static void script_AE76(int32_t arg0) {
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

static void script_AEF4(void) {
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_PROGRESS_STEP_28_REACHED)) {
    script_AE76(40);
  } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_PROGRESS_STEP_1E_REACHED)) {
    script_AE76(30);
  } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_PROGRESS_STEP_14_REACHED)) {
    script_AE76(20);
  } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JU00_CATA1_NBO_BRANCH_CHOSEN)) {
    script_AE76(10);
  } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_PROGRESS_STEP05_SA1116_DOJO_DONE)) {
    script_AE76(5);
  } else {
    script_AE76(0);
  }
  return;
}

static void script_AF73(void) {
  int32_t unused_FC;
  GameDateTime local_F4;
  int8_t local_F3;
  int8_t local_F2;
  int8_t local_F1;
  uint8_t unused_EF[2];
  int8_t local_EE;
  int8_t local_ED;
  int8_t local_EC;
  int8_t local_EB;
  int8_t local_EA;
  int8_t local_E9;
  int8_t local_E8;
  int8_t local_E7;
  int8_t local_E6;
  int8_t local_DD[9];
  uint8_t unused_DC;
  int32_t local_D8;
  Script_ReturnTimeDate(&local_F4);
  local_EE = (&local_F4)->year;
  local_ED = (&local_F4)->month;
  local_EC = (&local_F4)->day;
  local_F3 = 0;
  local_D8 = local_ED;
  switch (local_D8) {
    case 12:
      while (local_F3 < 9) {
        local_DD[local_F3] = data_15F98[local_F3];
        local_F3 = local_F3 + 1;
      }
      break;
    case 1:
      while (local_F3 < 9) {
        local_DD[local_F3] = data_15FA4[local_F3];
        local_F3 = local_F3 + 1;
      }
      break;
    case 2:
      while (local_F3 < 9) {
        local_DD[local_F3] = data_15FB0[local_F3];
        local_F3 = local_F3 + 1;
      }
      break;
    case 3:
      while (local_F3 < 9) {
        local_DD[local_F3] = data_15FBC[local_F3];
        local_F3 = local_F3 + 1;
      }
      break;
  }
  local_F3 = 0;
  local_EB = 0;
  while (local_F3 < 9) {
    if (local_EC == local_DD[local_F3]) {
      local_EB = 1;
    }
    local_F3 = local_F3 + 1;
  }
  *(int8_t *)task_1D0 = Script_PlayerStatsUtils(PS_GetStepSlot, 0);
  *((int8_t *)task_1D0 + 1) = Script_PlayerStatsUtils(PS_GetStepSlot, 1);
  *((int8_t *)task_1D0 + 2) = Script_PlayerStatsUtils(PS_GetStepSlot, 2);
  *((int8_t *)task_1D0 + 3) = Script_PlayerStatsUtils(PS_GetStepSlot, 3);
  *((int8_t *)task_1D0 + 4) = Script_PlayerStatsUtils(PS_GetStepSlot, 4);
  *((int8_t *)task_1D0 + 5) = Script_PlayerStatsUtils(PS_GetStepSlot, 5);
  *((int8_t *)task_1D0 + 6) = Script_PlayerStatsUtils(PS_GetStepSlot, 6);
  *((int8_t *)task_1D0 + 7) = Script_PlayerStatsUtils(PS_GetStepSlot, 7);
  *((int8_t *)task_1D0 + 8) = Script_PlayerStatsUtils(PS_GetStepSlot, 8);
  *((int8_t *)task_1D0 + 9) = Script_PlayerStatsUtils(PS_GetStepSlot, 9);
  *((int8_t *)task_1D0 + 10) = Script_PlayerStatsUtils(PS_GetStepSlot, 10);
  *((int8_t *)task_1D0 + 11) = Script_PlayerStatsUtils(PS_GetStepSlot, 11);
  *((int8_t *)task_1D0 + 12) = Script_PlayerStatsUtils(PS_GetStepSlot, 12);
  *((int8_t *)task_1D0 + 13) = Script_PlayerStatsUtils(PS_GetStepSlot, 13);
  *((int8_t *)task_1D0 + 14) = Script_PlayerStatsUtils(PS_GetStepSlot, 14);
  *((int8_t *)task_1D0 + 15) = Script_PlayerStatsUtils(PS_GetStepSlot, 15);
  if (*(int8_t *)task_1D0 == 0) {
    *(int8_t *)task_1D0 = 8;
    Script_PlayerStatsUtils(PS_SetStepSlot, 0, *(int8_t *)task_1D0);
    *((int8_t *)task_1D0 + 1) = 4;
    Script_PlayerStatsUtils(PS_SetStepSlot, 1, *((int8_t *)task_1D0 + 1));
    *((int8_t *)task_1D0 + 2) = 7;
    Script_PlayerStatsUtils(PS_SetStepSlot, 2, *((int8_t *)task_1D0 + 2));
    *((int8_t *)task_1D0 + 3) = 9;
    Script_PlayerStatsUtils(PS_SetStepSlot, 3, *((int8_t *)task_1D0 + 3));
    *((int8_t *)task_1D0 + 4) = 6;
    Script_PlayerStatsUtils(PS_SetStepSlot, 4, *((int8_t *)task_1D0 + 4));
    *((int8_t *)task_1D0 + 5) = 5;
    Script_PlayerStatsUtils(PS_SetStepSlot, 5, *((int8_t *)task_1D0 + 5));
    *((int8_t *)task_1D0 + 6) = 2;
    Script_PlayerStatsUtils(PS_SetStepSlot, 6, *((int8_t *)task_1D0 + 6));
    *((int8_t *)task_1D0 + 7) = 5;
    Script_PlayerStatsUtils(PS_SetStepSlot, 7, *((int8_t *)task_1D0 + 7));
    *((int8_t *)task_1D0 + 8) = 7;
    Script_PlayerStatsUtils(PS_SetStepSlot, 8, *((int8_t *)task_1D0 + 8));
    *((int8_t *)task_1D0 + 9) = 3;
    Script_PlayerStatsUtils(PS_SetStepSlot, 9, *((int8_t *)task_1D0 + 9));
    *((int8_t *)task_1D0 + 10) = 2;
    Script_PlayerStatsUtils(PS_SetStepSlot, 10, *((int8_t *)task_1D0 + 10));
    *((int8_t *)task_1D0 + 11) = 5;
    Script_PlayerStatsUtils(PS_SetStepSlot, 11, *((int8_t *)task_1D0 + 11));
    *((int8_t *)task_1D0 + 12) = 6;
    Script_PlayerStatsUtils(PS_SetStepSlot, 12, *((int8_t *)task_1D0 + 12));
    *((int8_t *)task_1D0 + 13) = 3;
    Script_PlayerStatsUtils(PS_SetStepSlot, 13, *((int8_t *)task_1D0 + 13));
    *((int8_t *)task_1D0 + 14) = 2;
    Script_PlayerStatsUtils(PS_SetStepSlot, 14, *((int8_t *)task_1D0 + 14));
    *((int8_t *)task_1D0 + 15) = 4;
    Script_PlayerStatsUtils(PS_SetStepSlot, 15, *((int8_t *)task_1D0 + 15));
  }
  local_F3 = 0;
  local_F2 = 0;
  local_F1 = 0;
  while (1) {
    if ((*(int8_t *)task_1D0 == Script_PlayerStatsUtils(PS_GetStepSlot, 0)) | (*((int8_t *)task_1D0 + 1) == Script_PlayerStatsUtils(PS_GetStepSlot, 1)) | (*((int8_t *)task_1D0 + 2) == Script_PlayerStatsUtils(PS_GetStepSlot, 2)) | (*((int8_t *)task_1D0 + 3) == Script_PlayerStatsUtils(PS_GetStepSlot, 3)) | (*((int8_t *)task_1D0 + 4) == Script_PlayerStatsUtils(PS_GetStepSlot, 4)) | (*((int8_t *)task_1D0 + 5) == Script_PlayerStatsUtils(PS_GetStepSlot, 5))) {
      local_EA = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 6.0f);
      local_E9 = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 6.0f);
      local_E8 = *((int8_t *)task_1D0 + local_EA);
      local_E7 = *((int8_t *)task_1D0 + local_E9);
      *((int8_t *)task_1D0 + local_EA) = local_E7;
      *((int8_t *)task_1D0 + local_E9) = local_E8;
    } else {
      local_F3 = 1;
    }
    if ((*((int8_t *)task_1D0 + 6) == Script_PlayerStatsUtils(PS_GetStepSlot, 6)) | (*((int8_t *)task_1D0 + 7) == Script_PlayerStatsUtils(PS_GetStepSlot, 7)) | (*((int8_t *)task_1D0 + 8) == Script_PlayerStatsUtils(PS_GetStepSlot, 8)) | (*((int8_t *)task_1D0 + 9) == Script_PlayerStatsUtils(PS_GetStepSlot, 9))) {
      local_EA = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 4.0f);
      local_E9 = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 4.0f);
      local_E8 = *((int8_t *)task_1D0 + (local_EA + 6));
      local_E7 = *((int8_t *)task_1D0 + (local_E9 + 6));
      *((int8_t *)task_1D0 + (local_EA + 6)) = local_E7;
      *((int8_t *)task_1D0 + (local_E9 + 6)) = local_E8;
    } else {
      local_F2 = 1;
    }
    if ((*((int8_t *)task_1D0 + 10) == Script_PlayerStatsUtils(PS_GetStepSlot, 10)) | (*((int8_t *)task_1D0 + 11) == Script_PlayerStatsUtils(PS_GetStepSlot, 11)) | (*((int8_t *)task_1D0 + 12) == Script_PlayerStatsUtils(PS_GetStepSlot, 12)) | (*((int8_t *)task_1D0 + 13) == Script_PlayerStatsUtils(PS_GetStepSlot, 13))) {
      local_EA = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 4.0f);
      local_E9 = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 4.0f);
      local_E8 = *((int8_t *)task_1D0 + (local_EA + 10));
      local_E7 = *((int8_t *)task_1D0 + (local_E9 + 10));
      *((int8_t *)task_1D0 + (local_EA + 10)) = local_E7;
      *((int8_t *)task_1D0 + (local_E9 + 10)) = local_E8;
    } else {
      local_F1 = 1;
    }
    local_EA = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f);
    local_E9 = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f);
    local_E8 = *((int8_t *)task_1D0 + (local_EA + 14));
    local_E7 = *((int8_t *)task_1D0 + (local_E9 + 14));
    *((int8_t *)task_1D0 + (local_EA + 14)) = local_E7;
    *((int8_t *)task_1D0 + (local_E9 + 14)) = local_E8;
    if (-(local_F3 == 1) & -(local_F2 == 1) & -(local_F1 == 1)) {
      break;
    }
  }
  local_F3 = 0;
  local_E6 = 1;
  local_E8 = *(int8_t *)task_1D0;
  while (local_F3 < 16) {
    local_E7 = *((int8_t *)task_1D0 + local_F3);
    if (local_E8 < local_E7) {
      local_E8 = local_E7;
      local_E6 = local_F3 + 1;
    }
    local_F3 = local_F3 + 1;
  }
  Script_scenUtils(SCENARIO_CMD_SET_STEP_LUT, 11, local_E6);
  Script_PlayerStatsUtils(PS_SetStepSlot, 0, *(int8_t *)task_1D0);
  Script_PlayerStatsUtils(PS_SetStepSlot, 1, *((int8_t *)task_1D0 + 1));
  Script_PlayerStatsUtils(PS_SetStepSlot, 2, *((int8_t *)task_1D0 + 2));
  Script_PlayerStatsUtils(PS_SetStepSlot, 3, *((int8_t *)task_1D0 + 3));
  Script_PlayerStatsUtils(PS_SetStepSlot, 4, *((int8_t *)task_1D0 + 4));
  Script_PlayerStatsUtils(PS_SetStepSlot, 5, *((int8_t *)task_1D0 + 5));
  Script_PlayerStatsUtils(PS_SetStepSlot, 6, *((int8_t *)task_1D0 + 6));
  Script_PlayerStatsUtils(PS_SetStepSlot, 7, *((int8_t *)task_1D0 + 7));
  Script_PlayerStatsUtils(PS_SetStepSlot, 8, *((int8_t *)task_1D0 + 8));
  Script_PlayerStatsUtils(PS_SetStepSlot, 9, *((int8_t *)task_1D0 + 9));
  Script_PlayerStatsUtils(PS_SetStepSlot, 10, *((int8_t *)task_1D0 + 10));
  Script_PlayerStatsUtils(PS_SetStepSlot, 11, *((int8_t *)task_1D0 + 11));
  Script_PlayerStatsUtils(PS_SetStepSlot, 12, *((int8_t *)task_1D0 + 12));
  Script_PlayerStatsUtils(PS_SetStepSlot, 13, *((int8_t *)task_1D0 + 13));
  Script_PlayerStatsUtils(PS_SetStepSlot, 14, *((int8_t *)task_1D0 + 14));
  Script_PlayerStatsUtils(PS_SetStepSlot, 15, *((int8_t *)task_1D0 + 15));
  SCNF_WaitFrames_0000(1);
  Script_sprintf(" : %d %d %d %d %d %d : %d %d %d %d : %d %d %d %d : %d %d \012", *(int8_t *)task_1D0, *((int8_t *)task_1D0 + 1), *((int8_t *)task_1D0 + 2), *((int8_t *)task_1D0 + 3), *((int8_t *)task_1D0 + 4), *((int8_t *)task_1D0 + 5), *((int8_t *)task_1D0 + 6), *((int8_t *)task_1D0 + 7), *((int8_t *)task_1D0 + 8), *((int8_t *)task_1D0 + 9), *((int8_t *)task_1D0 + 10), *((int8_t *)task_1D0 + 11), *((int8_t *)task_1D0 + 12), *((int8_t *)task_1D0 + 13), *((int8_t *)task_1D0 + 14), *((int8_t *)task_1D0 + 15));
  Script_sprintf("lacky : %d\012", local_E6);
  return;
}

static void script_B7EB(int32_t arg0) {
  Script_executeAicaCommand(16296, 0, 0);
  Script_executeAicaCommand(1184, 2, 110);
  script_B83B();
  task_1E8 = Script_scenCreateTask(script_B8C1, 0);
  Script_shenSeqUtils(arg0);
  while (Script_shenSeqUtils(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_srchControl((void *)task_1E8);
  return;
}

static void script_B83B(void) {
  Script_LightUtils(4, 4, 0.0f);
  Script_LightUtils(5, 4, 0.0f);
  Script_LightUtils(6, 4, 0.0f);
  Script_LightUtils(7, 4, 0.0f);
  Script_LightUtils(8, 4, 0.0f);
  Script_LightUtils(9, 4, 0.0f);
  Script_LightUtils(10, 4, 0.0f);
  Script_LightUtils(11, 4, 0.0f);
  Script_mapmSetRenderFlag(7, 0);
  task_1E4 = 0;
  task_1EC = 0.0f;
  task_1F0 = 0.0f;
  task_1F4 = 0.0f;
  task_1F8 = 0.0f;
  task_1FC = 0.0f;
  return;
}

static int32_t script_B8C1(void) {
  int32_t local_FC;
  for (;;) {
    local_FC = task_1E4;
    switch (local_FC) {
      case 60:
        Script_mapmSetRenderFlag(7, 1);
        break;
      case 80:
        Script_mapmSetRenderFlag(7, 0);
        break;
      case 90:
        Script_mapmSetRenderFlag(7, 1);
        break;
      case 150:
        Script_mapmSetRenderFlag(7, 0);
        break;
      case 180:
        Script_mapmSetRenderFlag(7, 1);
        break;
    }
    if ((task_1E4 > 20) & (task_1E4 < 80)) {
      task_1EC = task_1EC + 0.0166666675f;
    }
    if ((task_1E4 > 10) & (task_1E4 < 70)) {
      task_1F0 = task_1F0 + 0.0166666675f;
    }
    if (task_1E4 == 80) {
      task_1F4 = 0.1f;
    } else {
      if ((task_1E4 > 80) & (task_1E4 < 110)) {
        task_1F4 = task_1F4 + 0.043333333f;
        goto L00B9AC;
      }
    }
    L00B9AC:;
    if (task_1E4 == 80) {
      task_1F8 = 0.1f;
    } else {
      if ((task_1E4 > 80) & (task_1E4 < 120)) {
        task_1F8 = task_1F8 + 0.0375f;
        goto L00B9E5;
      }
    }
    L00B9E5:;
    if (task_1E4 == 150) {
      task_1FC = 0.1f;
    } else {
      if ((task_1E4 > 150) & (task_1E4 < 210)) {
        task_1FC = task_1FC + 0.0316666663f;
        goto L00BA21;
      }
    }
    L00BA21:;
    Script_LightUtils(10, 4, task_1EC);
    Script_LightUtils(5, 4, task_1F0);
    Script_LightUtils(6, 4, task_1F0);
    Script_LightUtils(7, 4, task_1F0);
    Script_LightUtils(9, 4, task_1F4);
    Script_LightUtils(4, 4, task_1F8);
    Script_LightUtils(11, 4, task_1FC);
    task_1E4 = task_1E4 + 1;
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_BA85(void) {
  Script_ambSoundUtils(1, 1, &data_1600C, 135.0f, 280.0f, 1.0f, 0, &data_16054);
  Script_ambSoundUtils(2, 1, &data_16018, 100.0f, 200.0f, 1.0f, 0, &data_160DC);
  Script_ambSoundUtils(3, 1, data_16024, 170.0f, 100.0f, 1.0f, 2, data_16164);
  Script_ambSoundUtils(4, 1, data_16030, 170.0f, 100.0f, 1.0f, 0, &data_161EC);
  Script_ambSoundUtils(5, 0, &data_1603C, 50.0f, 170.0f, 0.8f, 0, data_16274);
  Script_ambSoundUtils(15, 1, data_16048, 300.0f, 0.0f, 1.0f, 3, &data_162FC);
  Script_Ambient1SoundUtils(0, 23976);
  return;
}

static int32_t script_BB67(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  local_FC = 0;
  local_F8 = arg0;
  switch (local_F8) {
    case 0x4142454B:
      local_FC = 1;
      break;
    case 0x4242454B:
      local_FC = 2;
      break;
    case 0x4342454B:
      local_FC = 3;
      break;
    case 0x4442454B:
      local_FC = 4;
      break;
    case 0x4542454B:
      local_FC = 5;
      break;
  }
  return local_FC;
}

static void script_BBE2(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  int32_t unused_F8;
  if (task_1B4 == 0) {
    local_FC = (arg0 - 1) * 17;
    local_FC = local_FC + arg1 * 3 + Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, ((scn3_bits){.i = Script_ScenDataUtil(&data_16840, 5, arg1)}).f));
    if ((local_FC >= 0) & (local_FC < 85)) {
      task_1B4 = Script_ShowUIMessageFromID((const char *)Script_ScenDataUtil(&data_166EC, 8, local_FC));
      Script_sprintf("MKSG :chara_no %d : voice_no = %d\012", arg0, local_FC);
    }
  }
  return;
}

static int32_t script_BC5A(void) {
  for (;;) {
    if (task_1B4) {
      if (Script_convGetIsActive((const void *)task_1B4) == 0) {
        task_1B4 = 0;
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

__attribute__((scn_string_bindings(0, data_16877, 1, data_1687C, 2, data_16881, 3, data_16886, 4, data_16891)))
static void script_BC86(void) {
  Script_WeatherTaskUtil(AndNotInstructionFlags, 0x784);
  script_E373();
  Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_MKSG_HELP_ROUTE_ACTIVE, 1);
  Script_scenUtils(SCENARIO_CMD_SET_STEP_LUT, 20, 0);
  Script_scenUtils(SCENARIO_CMD_SET_STEP_LUT, 21, 0);
  Script_scenUtils(SCENARIO_CMD_SET_STEP_LUT, 22, 0);
  Script_scenUtils(SCENARIO_CMD_SET_STEP_LUT, 23, 0);
  Script_LoadSND(0, 5, "FREE", 0);
  Script_LoadSND(0, 4, "FREE", 0);
  Script_LoadSND(0, 3, "FREE", 0);
  Script_LoadSND(0, 2, "ykm005.snd", 0);
  Script_executeAicaCommand(16296, 0, 0);
  Script_executeAicaCommand(1184, 2, 127);
  Script_LoadSND(0, 4, "f1kyusok.snd", 0);
  Script_LoadSND(0, 5, "e1keibin.snd", 0);
  Script_SetMapControlTaskFlag(SHEN_NAME('MAGZ'), 1);
  Script_SetMapControlTaskFlag(SHEN_NAME('KAIS'), 1);
  Script_scenToggleVisibleFor(SHEN_NAME('ISU_'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('PEN_'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('CLIP'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('TEL_'), 0);
  return;
}

static void script_BD87(void) {
  int32_t unused_FC;
  GameDateTime local_F4;
  Vector3 local_E8;
  Vector3i local_DC;
  int32_t local_D8;
  int16_t local_D6;
  int8_t local_D5;
  uint8_t unused_D4;
  Vector3i local_C8;
  Script_ryoControl(PCCTL_DisablePlayerControl);
  script_119B5();
  Script_mtwkGetSetRotation(CHARA_AKIR, &local_DC, 0x2000000);
  Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &local_DC, 0);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_charaDisplay(CHARA_AKIR, CDM_Hide);
  if (Script_scenUtils(SCENARIO_CMD_GET_STEP_LUT, 23) == 1) {
    local_D8 = 0x3250414D;
  } else {
    local_D8 = 0x3150414D;
  }
  Script_EVT_GetPosition(CHARA_AKIR, -1, &local_E8, 0);
  (&local_E8)->y = 1.675f;
  Script_EVT_SetPosition(local_D8, POS_XYZ, &local_E8);
  Script_EVT_SetRotation(local_D8, 0x38000000, &local_DC, 0);
  Script_SetGameStateTo(AUTH_EventCam);
  Script_seqEnterEvtCam(500, local_D8, local_D8);
  Script_SetCameraFlags(0);
  SCNF_WaitFrames_0000(1);
  Script_mtwkGetSetRotation(local_D8, &local_DC, 0);
  local_DC.x = 0x4000;
  Script_EVT_SetRotation(local_D8, 0x38000000, &local_DC, 0);
  SCNF_WaitFrames_0000(1);
  Script_SetCycleManagerData(31, (const uint32_t *)&local_E8, (const uint32_t *)&local_DC, 1, 0x31303030);
  Script_MoveObjectToObject(local_D8, 31);
  Script_mtwkGetSetRotation(CHARA_AKIR, &local_C8, 0);
  (&local_C8)->y = -(&local_C8)->y;
  Script_EVT_SetRotation(local_D8, 0x38000000, &local_C8, 0);
  Script_MoveObjectToObject(local_D8, -1);
  SCNF_WaitFrames_0000(1);
  local_D6 = (Script_scenUtils(SCENARIO_CMD_GET_STEP_LUT, 21) << 8) + Script_scenUtils(SCENARIO_CMD_GET_STEP_LUT, 22);
  if (Script_scenUtils(SCENARIO_CMD_GET_STEP_LUT, 23) == 2) {
    local_D6 = local_D6 | 0x80;
  }
  if (Script_scenUtils(SCENARIO_CMD_GET_STEP_LUT, 23) == 4) {
    local_D6 = local_D6 | 0x80;
  }
  local_D6 = local_D6 | 0x20;
  local_D5 = 0;
  while (local_D5 < 10) {
    if ((local_D6 & (1 << local_D5)) == 0) {
      Script_charaPosRotUtils(local_D8, *(int16_t *)((int8_t *)&data_168AC + (local_D5 << 1)), &data_168CC, 0, 0);
    }
    local_D5 = local_D5 + 1;
  }
  if (0) {
    Script_charaDisplay(local_D8, CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(local_D8, CDM_Show);
      goto L00BF9B;
    }
  }
  L00BF9B:;
  Script_scenToggleVisibleFor(local_D8, 1);
  Script_ccowSetFlag80(local_D8, 1);
  Script_ccowUtils(local_D8, 2);
  while (1) {
    Script_ReturnTimeDate(&local_F4);
    if ((Script_GetControllerInput(1) == 4) | (Script_GetControllerInput(1) == 0x200) | (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_EV_STOP_ACTIVE) == 1) | (task_1CA == -1) | (task_1CD == -1)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_SetCameraFlags(-1);
  Script_convSetCamInactiveFlag(1);
  Script_SetGameStateTo(AUTH_None);
  if (-1) {
    Script_charaDisplay(local_D8, CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(local_D8, CDM_Show);
      goto L00C03A;
    }
  }
  L00C03A:;
  Script_scenToggleVisibleFor(local_D8, 0);
  local_D5 = 0;
  while (local_D5 < 10) {
    Script_charaPosRotUtils(local_D8, *(int16_t *)((int8_t *)&data_168AC + (local_D5 << 1)), &data_168C0, 0, 0);
    local_D5 = local_D5 + 1;
  }
  if (task_1CD != -1) {
    script_11A0D();
  }
  Script_charaDisplay(CHARA_AKIR, CDM_Show);
  Script_ryoControl(RYO_RestoreControl);
  SCNF_WaitFrames_0000(1);
  return;
}

static int32_t script_C0A3(int32_t arg0) {
  int32_t local_FC;
  int8_t local_FB;
  local_FC = 0;
  local_FB = data_16BBC[arg0];
  while (1) {
    if (*(&data_16B14 + local_FB) == local_FC) {
      Script_ShowUIMessageFromID((const char *)Script_ScenDataUtil(&data_16A6C, 8, local_FB));
      local_FB = local_FB + 1;
      if (*(&data_16B14 + local_FB) == -1) {
        break;
      }
    }
    if ((arg0 == 1) & (local_FC == 400)) {
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_MFSY_COFFEE_SPECIAL_VARIANT)) {
        Script_ShowUIMessageFromID("E1042A002");
      } else {
        Script_ShowUIMessageFromID("E1042A001");
      }
    }
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_C149(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t local_E4;
  int32_t local_E0;
  local_E4 = 0;
  while (1) {
    local_E0 = local_E4;
    switch (local_E0) {
      case 0:
        Script_PlayCharaMotion(CHARA_AKIR, 4420, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
        break;
      case 131:
        Script_PlayCharaMotion(CHARA_SYZU, 4425, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        Script_motmSetBlendFrames(CHARA_SYZU, MOTM_BLEND_FRAMES_20);
        Script_EVT_GetPosition(CHARA_SYZU, 5, &local_F4, ENTITY_XFORM_WORLD_SPACE);
        Script_SetCharaLookAtPosition(CHARA_AKIR, 6, &local_F4, 0);
        break;
      case 670:
        Script_SetCharaMotionPlaybackRate(CHARA_AKIR, 1.3f);
        Script_SetCharaMotionPlaybackRate(CHARA_SYZU, 1.3f);
        break;
      case 770:
        Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
        break;
      case 800:
        script_D447();
        local_F4.x = 17.5f;
        (&local_F4)->y = 0.0f;
        (&local_F4)->z = -9.6f;
        Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &local_F4);
        {
          Vector3i local_D4;
          (&local_D4)->y = 0;
          Script_EVT_SetRotation(CHARA_AKIR, MATRIX_XFORM_Y, &local_D4, 0);
        }
        local_F4.x = 17.568001f;
        (&local_F4)->y = 0.0f;
        (&local_F4)->z = -10.87f;
        Script_EVT_SetPosition(CHARA_SYZU, POS_XYZ, &local_F4);
        {
          Vector3i local_D4;
          (&local_D4)->y = 0x8000;
          Script_EVT_SetRotation(CHARA_SYZU, MATRIX_XFORM_Y, &local_D4, 0);
        }
        Script_motmSetBlendFrames(CHARA_SYZU, MOTM_BLEND_FRAMES_5);
        Script_PlayCharaMotion(CHARA_AKIR, 4421, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        Script_PlayCharaMotion(CHARA_SYZU, 4426, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        Script_SetCharaMotionPlaybackRate(CHARA_AKIR, 0.0f);
        Script_SetCharaMotionPlaybackRate(CHARA_SYZU, 0.0f);
        Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
        break;
      case 1250:
        Script_SetCharaMotionPlaybackRate(CHARA_AKIR, 1.0f);
        Script_SetCharaMotionPlaybackRate(CHARA_SYZU, 1.0f);
        break;
      case 1320:
        if (0) {
          Script_charaDisplay(SHEN_NAME('MAP1'), CDM_Hide);
        } else {
          if (-1) {
            Script_charaDisplay(SHEN_NAME('MAP1'), CDM_Show);
            goto L00C3CA;
          }
        }
        L00C3CA:;
        Script_scenToggleVisibleFor(SHEN_NAME('MAP1'), 1);
        local_F4.x = 0.124f;
        (&local_F4)->y = 0.03f;
        (&local_F4)->z = -0.048f;
        local_E8.x = -16019;
        (&local_E8)->y = -36135;
        (&local_E8)->z = 29491;
        Script_fixoAttachEx(SHEN_NAME('MAP1'), CHARA_SYZU, 18, &local_F4, &local_E8);
        Script_CharHandlers(CHARA_SYZU, 0, 10, 16);
        break;
      case 1819:
        Script_fixoClearAttachment(SHEN_NAME('MAP1'));
        break;
      case 1820:
        local_F4.x = 0.18f;
        (&local_F4)->y = 0.06f;
        (&local_F4)->z = -0.02f;
        local_E8.x = -19478;
        (&local_E8)->y = -1456;
        (&local_E8)->z = 33314;
        Script_fixoAttachEx(SHEN_NAME('MAP1'), CHARA_AKIR, 18, &local_F4, &local_E8);
        Script_CharHandlers(CHARA_AKIR, 0, 10, 16);
        break;
      case 2200:
        Script_SetCharaLookAtPosition(CHARA_AKIR, -6, 0, 0);
        break;
      case 2250:
        Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
        while (Script_filmCheckOrSetTransitionUtil(-1)) {
          SCNF_WaitFrames_0000(1);
        }
        Script_SetCharaLookAtPosition(CHARA_AKIR, -6, 0, 0);
        Script_fixoClearAttachment(SHEN_NAME('MAP1'));
        if (-1) {
          Script_charaDisplay(SHEN_NAME('MAP1'), CDM_Hide);
        } else {
          if (0) {
            Script_charaDisplay(SHEN_NAME('MAP1'), CDM_Show);
            goto L00C553;
          }
        }
        L00C553:;
        Script_scenToggleVisibleFor(SHEN_NAME('MAP1'), 0);
        goto L00C612;
        break;
    }
    if ((local_E4 > 800) & (local_E4 <= 1780)) {
      Script_EVT_GetPosition(CHARA_SYZU, 5, &local_F4, ENTITY_XFORM_WORLD_SPACE);
      Script_SetLookPointDetails(CHARA_AKIR, 16, &local_F4, 0);
    }
    if ((local_E4 > 1800) & (local_E4 <= 1920)) {
      Script_EVT_GetPosition(CHARA_AKIR, 18, &local_F4, ENTITY_XFORM_WORLD_SPACE);
      Script_SetLookPointDetails(CHARA_AKIR, 16, &local_F4, 0);
    }
    if ((local_E4 > 1920) & (local_E4 <= 2200)) {
      Script_EVT_GetPosition(CHARA_SYZU, 5, &local_F4, ENTITY_XFORM_WORLD_SPACE);
      Script_SetLookPointDetails(CHARA_AKIR, 16, &local_F4, 0);
    }
    local_E4 = local_E4 + 1;
    SCNF_WaitFrames_0000(1);
  }
  L00C612:;
}

static int32_t script_C613(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t local_E4;
  int32_t local_E0;
  local_E4 = 0;
  while (1) {
    local_E0 = local_E4;
    switch (local_E0) {
      case 0:
        script_D447();
        Script_PlayCharaMotion(CHARA_AKIR, 4424, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        Script_PlayCharaMotion(CHARA_SYZU, 4428, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        Script_SetCharaMotionPlaybackRate(CHARA_AKIR, 0.0f);
        Script_SetCharaMotionPlaybackRate(CHARA_SYZU, 0.0f);
        Script_EVT_GetPosition(CHARA_SYZU, 5, &local_F4, ENTITY_XFORM_WORLD_SPACE);
        Script_SetCharaLookAtPosition(CHARA_AKIR, 6, &local_F4, 0);
        Script_motiSetAngFlags(CHARA_SYZU, 1);
        Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
        break;
      case 140:
        Script_SetCharaMotionPlaybackRate(CHARA_AKIR, 1.0f);
        Script_SetCharaMotionPlaybackRate(CHARA_SYZU, 1.0f);
        break;
      case 440:
        if (0) {
          Script_charaDisplay(SHEN_NAME('MAP1'), CDM_Hide);
        } else {
          if (-1) {
            Script_charaDisplay(SHEN_NAME('MAP1'), CDM_Show);
            goto L00C731;
          }
        }
        L00C731:;
        Script_scenToggleVisibleFor(SHEN_NAME('MAP1'), 1);
        local_F4.x = 0.18f;
        (&local_F4)->y = 0.06f;
        (&local_F4)->z = -0.02f;
        local_E8.x = -19478;
        (&local_E8)->y = -1456;
        (&local_E8)->z = 33314;
        Script_fixoAttachEx(SHEN_NAME('MAP1'), CHARA_AKIR, 18, &local_F4, &local_E8);
        Script_CharHandlers(CHARA_AKIR, 0, 10, 16);
        break;
      case 549:
        Script_fixoClearAttachment(SHEN_NAME('MAP1'));
        break;
      case 550:
        local_F4.x = 0.13f;
        (&local_F4)->y = 0.03f;
        (&local_F4)->z = 0.063f;
        local_E8.x = 48605;
        (&local_E8)->y = 37683;
        (&local_E8)->z = 0x8000;
        Script_fixoAttachEx(SHEN_NAME('MAP1'), CHARA_SYZU, 12, &local_F4, &local_E8);
        Script_CharHandlers(CHARA_SYZU, 1, 10, 16);
        break;
      case 984:
        Script_fixoClearAttachment(SHEN_NAME('MAP1'));
        break;
      case 985:
        if (0) {
          Script_charaDisplay(SHEN_NAME('MAP1'), CDM_Hide);
        } else {
          if (-1) {
            Script_charaDisplay(SHEN_NAME('MAP1'), CDM_Show);
            goto L00C875;
          }
        }
        L00C875:;
        Script_scenToggleVisibleFor(SHEN_NAME('MAP1'), 1);
        local_F4.x = 0.18f;
        (&local_F4)->y = 0.06f;
        (&local_F4)->z = -0.02f;
        local_E8.x = -19478;
        (&local_E8)->y = -1456;
        (&local_E8)->z = 33314;
        Script_fixoAttachEx(SHEN_NAME('MAP1'), CHARA_AKIR, 18, &local_F4, &local_E8);
        Script_CharHandlers(CHARA_AKIR, 0, 10, 16);
        break;
      case 1160:
        Script_fixoClearAttachment(SHEN_NAME('MAP1'));
        if (-1) {
          Script_charaDisplay(SHEN_NAME('MAP1'), CDM_Hide);
        } else {
          if (0) {
            Script_charaDisplay(SHEN_NAME('MAP1'), CDM_Show);
            goto L00C922;
          }
        }
        L00C922:;
        Script_scenToggleVisibleFor(SHEN_NAME('MAP1'), 0);
        break;
      case 1390:
        Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
        while (Script_filmCheckOrSetTransitionUtil(-1)) {
          SCNF_WaitFrames_0000(1);
        }
        Script_SetCharaLookAtPosition(CHARA_AKIR, -6, 0, 0);
        goto L00CA65;
        break;
    }
    if ((local_E4 > 850) & (local_E4 <= 865)) {
      Script_EVT_GetPosition(CHARA_SYZU, 17, &local_F4, ENTITY_XFORM_WORLD_SPACE);
      local_F4.x = (&local_F4)->x + 0.01f;
      (&local_F4)->y = (&local_F4)->y - 0.005f;
      Script_SetMotnFlags(CHARA_SYZU, 17, &local_F4, 0x78000000);
    } else if ((local_E4 > 865) & (local_E4 <= 885)) {
      Script_SetMotnFlags(CHARA_SYZU, 17, &local_F4, 0x78000000);
    } else {
      if ((local_E4 > 885) & (local_E4 <= 895)) {
        Script_EVT_GetPosition(CHARA_SYZU, 17, &local_F4, ENTITY_XFORM_WORLD_SPACE);
        local_F4.x = (&local_F4)->x - 0.01f;
        (&local_F4)->y = (&local_F4)->y + 0.005f;
        Script_SetMotnFlags(CHARA_SYZU, 17, &local_F4, 0x78000000);
        goto L00CA51;
      }
    }
    L00CA51:;
    local_E4 = local_E4 + 1;
    SCNF_WaitFrames_0000(1);
  }
  L00CA65:;
}

static int32_t script_CA66(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t local_E4;
  int32_t local_E0;
  local_E4 = 0;
  while (1) {
    local_E0 = local_E4;
    switch (local_E0) {
      case 0:
        script_D447();
        Script_PlayCharaMotion(CHARA_AKIR, 4424, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        Script_PlayCharaMotion(CHARA_SYZU, 4428, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        Script_SetCharaMotionPlaybackRate(CHARA_AKIR, 0.0f);
        Script_SetCharaMotionPlaybackRate(CHARA_SYZU, 0.0f);
        Script_EVT_GetPosition(CHARA_SYZU, 5, &local_F4, ENTITY_XFORM_WORLD_SPACE);
        Script_SetCharaLookAtPosition(CHARA_AKIR, 6, &local_F4, 0);
        Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
        break;
      case 90:
        Script_SetCharaMotionPlaybackRate(CHARA_AKIR, 1.0f);
        Script_SetCharaMotionPlaybackRate(CHARA_SYZU, 1.0f);
        break;
      case 390:
        if (0) {
          Script_charaDisplay(SHEN_NAME('MAP1'), CDM_Hide);
        } else {
          if (-1) {
            Script_charaDisplay(SHEN_NAME('MAP1'), CDM_Show);
            goto L00CB77;
          }
        }
        L00CB77:;
        Script_scenToggleVisibleFor(SHEN_NAME('MAP1'), 1);
        local_F4.x = 0.18f;
        (&local_F4)->y = 0.06f;
        (&local_F4)->z = -0.02f;
        local_E8.x = -19478;
        (&local_E8)->y = -1456;
        (&local_E8)->z = 33314;
        Script_fixoAttachEx(SHEN_NAME('MAP1'), CHARA_AKIR, 18, &local_F4, &local_E8);
        Script_CharHandlers(CHARA_AKIR, 0, 10, 16);
        break;
      case 499:
        Script_fixoClearAttachment(SHEN_NAME('MAP1'));
        break;
      case 500:
        local_F4.x = 0.13f;
        (&local_F4)->y = 0.03f;
        (&local_F4)->z = 0.063f;
        local_E8.x = 48605;
        (&local_E8)->y = 37683;
        (&local_E8)->z = 0x8000;
        Script_fixoAttachEx(SHEN_NAME('MAP1'), CHARA_SYZU, 12, &local_F4, &local_E8);
        Script_CharHandlers(CHARA_SYZU, 1, 10, 16);
        Script_motmToggleFlags(CHARA_AKIR, 2, 0x40);
        Script_motmToggleFlags(CHARA_SYZU, 2, 0x40);
        Script_PlayCharaMotion(CHARA_AKIR, 4424, -1.0f, -1.0f, 690.0f, 0.7f, 0);
        Script_PlayCharaMotion(CHARA_SYZU, 4428, -1.0f, -1.0f, 690.0f, 0.7f, 0);
        Script_motiSetAngFlags(CHARA_SYZU, 1);
        break;
      case 683:
        Script_SetCharaMotionPlaybackRate(CHARA_AKIR, 1.0f);
        Script_SetCharaMotionPlaybackRate(CHARA_SYZU, 1.0f);
        break;
      case 709:
        Script_fixoClearAttachment(SHEN_NAME('MAP1'));
        break;
      case 710:
        if (0) {
          Script_charaDisplay(SHEN_NAME('MAP1'), CDM_Hide);
        } else {
          if (-1) {
            Script_charaDisplay(SHEN_NAME('MAP1'), CDM_Show);
            goto L00CD4E;
          }
        }
        L00CD4E:;
        Script_scenToggleVisibleFor(SHEN_NAME('MAP1'), 1);
        local_F4.x = 0.18f;
        (&local_F4)->y = 0.06f;
        (&local_F4)->z = -0.02f;
        local_E8.x = -19478;
        (&local_E8)->y = -1456;
        (&local_E8)->z = 33314;
        Script_fixoAttachEx(SHEN_NAME('MAP1'), CHARA_AKIR, 18, &local_F4, &local_E8);
        Script_CharHandlers(CHARA_AKIR, 0, 10, 16);
        break;
      case 781:
        Script_EVT_GetPosition(CHARA_SYZU, 5, &local_F4, ENTITY_XFORM_WORLD_SPACE);
        Script_SetLookPointDetails(CHARA_AKIR, 16, &local_F4, 0);
        break;
      case 885:
        Script_fixoClearAttachment(SHEN_NAME('MAP1'));
        if (-1) {
          Script_charaDisplay(SHEN_NAME('MAP1'), CDM_Hide);
        } else {
          if (0) {
            Script_charaDisplay(SHEN_NAME('MAP1'), CDM_Show);
            goto L00CE2A;
          }
        }
        L00CE2A:;
        Script_scenToggleVisibleFor(SHEN_NAME('MAP1'), 0);
        break;
      case 995:
        Script_motmToggleFlags(CHARA_AKIR, 1, 0x40);
        Script_motmToggleFlags(CHARA_SYZU, 1, 0x40);
        Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
        while (Script_filmCheckOrSetTransitionUtil(-1)) {
          SCNF_WaitFrames_0000(1);
        }
        Script_SetCharaLookAtPosition(CHARA_AKIR, -6, 0, 0);
        goto L00CFBA;
        break;
    }
    if ((local_E4 > 720) & (local_E4 <= 780)) {
      Script_EVT_GetPosition(CHARA_AKIR, 17, &local_F4, ENTITY_XFORM_WORLD_SPACE);
      Script_SetLookPointDetails(CHARA_AKIR, 16, &local_F4, 0);
    }
    if ((local_E4 > 500) & (local_E4 <= 515)) {
      Script_EVT_GetPosition(CHARA_SYZU, 17, &local_F4, ENTITY_XFORM_WORLD_SPACE);
      local_F4.x = (&local_F4)->x + 0.01f;
      (&local_F4)->y = (&local_F4)->y - 0.005f;
      Script_SetMotnFlags(CHARA_SYZU, 17, &local_F4, 0x78000000);
    } else if ((local_E4 > 515) & (local_E4 <= 600)) {
      Script_SetMotnFlags(CHARA_SYZU, 17, &local_F4, 0x78000000);
    } else {
      if ((local_E4 > 600) & (local_E4 <= 610)) {
        Script_EVT_GetPosition(CHARA_SYZU, 17, &local_F4, ENTITY_XFORM_WORLD_SPACE);
        local_F4.x = (&local_F4)->x - 0.01f;
        (&local_F4)->y = (&local_F4)->y + 0.01f;
        Script_SetMotnFlags(CHARA_SYZU, 17, &local_F4, 0x78000000);
        goto L00CFA6;
      }
    }
    L00CFA6:;
    local_E4 = local_E4 + 1;
    SCNF_WaitFrames_0000(1);
  }
  L00CFBA:;
}

static int32_t script_CFBB(void) {
  int32_t unused_FC;
  GameDateTime local_F4;
  Vector3 local_E8;
  int32_t unused_DC[3];
  int32_t local_D8;
  int32_t local_D4;
  local_D8 = 0;
  while (1) {
    local_D4 = local_D8;
    if (local_D4 == 0) {
      script_D447();
      Script_PlayCharaMotion(CHARA_AKIR, 4424, -1.0f, -1.0f, -1.0f, 1.0f, 0);
      Script_PlayCharaMotion(CHARA_SYZU, 4428, -1.0f, -1.0f, -1.0f, 1.0f, 0);
      Script_SetCharaMotionPlaybackRate(CHARA_AKIR, 0.0f);
      Script_SetCharaMotionPlaybackRate(CHARA_SYZU, 0.0f);
      Script_EVT_GetPosition(CHARA_SYZU, 5, &local_E8, ENTITY_XFORM_WORLD_SPACE);
      Script_SetCharaLookAtPosition(CHARA_AKIR, 6, &local_E8, 0);
      Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
    } else if (local_D4 == 1350) {
      Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
    } else if (local_D4 == 1380) {
      Script_ReturnTimeDate(&local_F4);
      (&local_F4)->hours = 23;
      (&local_F4)->minutes = 0;
      Script_SetNewDateTime(&local_F4);
      Script_cdttLoadScrUtils(0, 0);
    } else if (local_D4 == 1500) {
      Script_cdttLoadScrUtils(1, 0);
    } else {
      if (local_D4 == 1500) {
        Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
        while (Script_filmCheckOrSetTransitionUtil(-1)) {
          SCNF_WaitFrames_0000(1);
        }
        Script_SetCharaLookAtPosition(CHARA_AKIR, -6, 0, 0);
        break;
        goto L00D12A;
      }
    }
    L00D12A:;
    local_D8 = local_D8 + 1;
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_D13F(void) {
  int32_t local_FC;
  int32_t local_F8;
  Script_SetGameStateTo(AUTH_EventCam);
  local_FC = 0;
  while (1) {
    local_F8 = local_FC;
    switch (local_F8) {
      case 0:
        Script_seqEnterEvtCam(4000, 0, AUTH_None);
        break;
      case 400:
        Script_seqEnterEvtCam(4001, 0, AUTH_None);
        break;
      case 500:
        Script_seqEnterEvtCam(4002, 0, AUTH_None);
        break;
      case 800:
        Script_seqEnterEvtCam(4003, 0, AUTH_None);
        break;
      case 1250:
        Script_seqEnterEvtCam(4004, 0, AUTH_None);
        break;
      case 1320:
        Script_seqEnterEvtCam(4005, 0, AUTH_None);
        break;
      case 1620:
        Script_seqEnterEvtCam(4006, 0, AUTH_None);
        break;
      case 1730:
        Script_seqEnterEvtCam(4007, 0, AUTH_None);
        break;
      case 1995:
        Script_seqEnterEvtCam(4008, 0, AUTH_None);
        break;
      case 2250:
        goto L00D254;
        break;
    }
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  L00D254:;
}

static int32_t script_D255(void) {
  int32_t local_FC;
  int32_t local_F8;
  Script_SetGameStateTo(AUTH_EventCam);
  local_FC = 0;
  while (1) {
    local_F8 = local_FC;
    switch (local_F8) {
      case 0:
        Script_seqEnterEvtCam(4010, 0, AUTH_None);
        break;
      case 222:
        Script_seqEnterEvtCam(4011, 0, AUTH_None);
        break;
      case 550:
        Script_seqEnterEvtCam(4012, 0, AUTH_None);
        break;
      case 985:
        Script_seqEnterEvtCam(4013, 0, AUTH_None);
        break;
      case 1195:
        Script_seqEnterEvtCam(4014, 0, AUTH_None);
        break;
      case 1390:
        goto L00D30A;
        break;
    }
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  L00D30A:;
}

static int32_t script_D30B(void) {
  int32_t local_FC;
  int32_t local_F8;
  Script_SetGameStateTo(AUTH_EventCam);
  local_FC = 0;
  while (1) {
    local_F8 = local_FC;
    switch (local_F8) {
      case 0:
        Script_seqEnterEvtCam(4020, 0, AUTH_None);
        break;
      case 221:
        Script_seqEnterEvtCam(4021, 0, AUTH_None);
        break;
      case 500:
        Script_seqEnterEvtCam(4022, 0, AUTH_None);
        break;
      case 710:
        Script_seqEnterEvtCam(4023, 0, AUTH_None);
        break;
      case 995:
        goto L00D3A8;
        break;
    }
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  L00D3A8:;
}

static int32_t script_D3A9(void) {
  int32_t local_FC;
  int32_t local_F8;
  Script_SetGameStateTo(AUTH_EventCam);
  local_FC = 0;
  while (1) {
    local_F8 = local_FC;
    switch (local_F8) {
      case 0:
        Script_seqEnterEvtCam(4030, 0, AUTH_None);
        break;
      case 223:
        Script_seqEnterEvtCam(4031, 0, AUTH_None);
        break;
      case 684:
        Script_seqEnterEvtCam(4032, 0, AUTH_None);
        break;
      case 919:
        Script_seqEnterEvtCam(4033, 0, AUTH_None);
        break;
      case 1500:
        goto L00D446;
        break;
    }
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  L00D446:;
}

static void script_D447(void) {
  Script_lightResetSlotToDefaults(21);
  Script_lightEnablePointLightAt(21, &data_16BD8);
  Script_LightUtils(21, 2, &data_16BE4);
  Script_LightUtils(21, 4, 0.4f);
  Script_LightUtils(21, 5, 3.0f);
  Script_LightUtils(21, 6, data_16BF4);
  Script_lightSetSlotEnabled(21, 1);
  return;
}

__attribute__((scn_string_bindings(0, data_16C24, 1, data_16C2A)))
static void script_D497(void) {
  Vector3 local_F4;
  int32_t local_F0;
  int32_t local_EC;
  int8_t local_EB;
  local_F0 = Script_scenUtils(SCENARIO_CMD_GET_STEP_LUT, 20) - 1;
  if (local_F0 < 12) {
    if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_MFSY_COFFEE_SPECIAL_VARIANT) == 0) {
      local_F0 = local_F0 + 12;
    }
    local_EB = data_16BFC[local_F0];
  } else {
    local_EB = 0;
  }
  if (local_EB == 2) {
    if (((Script_scenUtils(SCENARIO_CMD_GET_STEP_LUT, 21) << 8) + Script_scenUtils(SCENARIO_CMD_GET_STEP_LUT, 22)) & 0x80) {
      Script_scenUtils(SCENARIO_CMD_SET_STEP_LUT, 23, local_EB);
      local_EB = 0;
    }
  }
  if (local_EB) {
    Script_FaceEx(CHARA_AKIR, 1, 0, 2);
    Script_shenClearFreeState(0);
    Script_LoadVoiceFile(1, "E1042");
    if (0) {
      Script_charaDisplay(CHARA_SYZU, CDM_Hide);
    } else {
      if (-1) {
        Script_charaDisplay(CHARA_SYZU, CDM_Show);
        goto L00D569;
      }
    }
    L00D569:;
    Script_scenToggleVisibleFor(CHARA_SYZU, 1);
    if (0) {
      Script_charaDisplay(SHEN_NAME('HAKO'), CDM_Hide);
    } else {
      if (-1) {
        Script_charaDisplay(SHEN_NAME('HAKO'), CDM_Show);
        goto L00D59D;
      }
    }
    L00D59D:;
    Script_scenToggleVisibleFor(SHEN_NAME('HAKO'), 1);
    Script_ccowUtils(CHARA_SYZU, 2);
    Script_SetCPAMFlags(CHARA_SYZU, 0);
    Script_charaDisplay(CHARA_SYZU, CDM_Show);
    Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_charaToggleMotEnabled(CHARA_AKIR, 1);
    Script_charaToggleMotEnabled(CHARA_SYZU, 1);
    Script_charaSetRefboardFlagTo(CHARA_AKIR, 1);
    Script_charaSetRefboardFlagTo(CHARA_SYZU, 1);
    Script_SetCharaRefBoardPos(CHARA_SYZU, &data_16C14);
    Script_SetCameraFlags(0);
    Script_PREP_Related(-1, 1);
    Script_convSetCamInactiveFlag(1);
    Script_SetGameStateTo(AUTH_None);
    if (local_EB == 1) {
      local_F4.x = 12.0f;
      (&local_F4)->y = 0.0f;
      (&local_F4)->z = -2.4f;
      Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &local_F4);
      {
        uint8_t unused_E8[3];
        Vector3i local_DC;
        (&local_DC)->y = 0x4000;
        Script_EVT_SetRotation(CHARA_AKIR, MATRIX_XFORM_Y, &local_DC, 0);
      }
      local_F4.x = 12.37185f;
      (&local_F4)->y = 0.0f;
      (&local_F4)->z = -2.4f;
      Script_EVT_SetPosition(CHARA_SYZU, POS_XYZ, &local_F4);
      {
        uint8_t unused_E8[3];
        Vector3i local_DC;
        (&local_DC)->y = 0x4000;
        Script_EVT_SetRotation(CHARA_SYZU, MATRIX_XFORM_Y, &local_DC, 0);
      }
    } else {
      local_F4.x = 17.5f;
      (&local_F4)->y = 0.0f;
      (&local_F4)->z = -9.6f;
      Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &local_F4);
      {
        uint8_t unused_E8[3];
        Vector3i local_DC;
        (&local_DC)->y = 0;
        Script_EVT_SetRotation(CHARA_AKIR, MATRIX_XFORM_Y, &local_DC, 0);
      }
      local_F4.x = 17.568001f;
      (&local_F4)->y = 0.0f;
      (&local_F4)->z = -10.87f;
      Script_EVT_SetPosition(CHARA_SYZU, POS_XYZ, &local_F4);
      {
        uint8_t unused_E8[3];
        Vector3i local_DC;
        (&local_DC)->y = 0x8000;
        Script_EVT_SetRotation(CHARA_SYZU, MATRIX_XFORM_Y, &local_DC, 0);
      }
    }
    Script_scenCreateTask(script_C0A3, 1, local_EB);
    local_EC = 1;
    {
      uint8_t unused_E8[3];
      int32_t local_E4;
      local_E4 = local_EB;
      switch (local_E4) {
        case 1:
          Script_scenCreateTask(script_C149, 0);
          Script_scenCreateTask(script_D13F, 0);
          local_EC = 2250;
          break;
        case 2:
          Script_scenCreateTask(script_C613, 0);
          Script_scenCreateTask(script_D255, 0);
          local_EC = 1390;
          break;
        case 3:
          Script_scenCreateTask(script_CA66, 0);
          Script_scenCreateTask(script_D30B, 0);
          local_EC = 995;
          break;
        case 4:
          Script_scenCreateTask(script_CFBB, 0);
          Script_scenCreateTask(script_D3A9, 0);
          local_EC = 1500;
          break;
      }
    }
    local_F0 = 0;
    while (local_F0 < local_EC) {
      local_F0 = local_F0 + 1;
      SCNF_WaitFrames_0000(1);
    }
    Script_scenUtils(SCENARIO_CMD_SET_STEP_LUT, 23, local_EB);
    SCNF_WaitFrames_0000(60);
    local_F4.x = -16.5f;
    (&local_F4)->y = 0.0f;
    (&local_F4)->z = -15.0f;
    Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &local_F4);
    {
      uint8_t unused_E4[7];
      Vector3i local_D8;
      (&local_D8)->y = 0;
      Script_EVT_SetRotation(CHARA_AKIR, MATRIX_XFORM_Y, &local_D8, 0);
    }
    Script_CharHandlers(CHARA_AKIR, -1, 0, 0);
    Script_CharHandlers(CHARA_SYZU, -1, 0, 0);
    if (-1) {
      Script_charaDisplay(CHARA_SYZU, CDM_Hide);
    } else {
      if (0) {
        Script_charaDisplay(CHARA_SYZU, CDM_Show);
        goto L00D8FF;
      }
    }
    L00D8FF:;
    Script_scenToggleVisibleFor(CHARA_SYZU, 0);
    if (-1) {
      Script_charaDisplay(SHEN_NAME('HAKO'), CDM_Hide);
    } else {
      if (0) {
        Script_charaDisplay(SHEN_NAME('HAKO'), CDM_Show);
        goto L00D933;
      }
    }
    L00D933:;
    Script_scenToggleVisibleFor(SHEN_NAME('HAKO'), 0);
    Script_LoadVoiceFile(0, "E1042");
    Script_uiUtils(0, 44, 0);
    Script_SetCameraFlags(1);
    Script_lightSetSlotEnabled(21, 0);
    Script_PREP_Related(-1, 0);
    Script_PREP_Related(0, 1);
    Script_FaceEx(CHARA_AKIR, 0, 0, 2);
    Script_shenClearFreeState(1);
  }
  SCNF_WaitFrames_0000(1);
  return;
}

static void script_D996(int32_t arg0) {
  Script_ScenDataUtil(&*(int32_t *)&task_200, 11, arg0 / 8, Script_ScenDataUtil(&*(int32_t *)&task_200, 1, arg0 / 8) | (1 << (arg0 % 8)));
  return;
}

static void script_D9C1(int32_t arg0) {
  Script_ScenDataUtil(&*(int32_t *)&task_200, 11, arg0 / 8, Script_ScenDataUtil(&*(int32_t *)&task_200, 1, arg0 / 8) & ((1 << (arg0 % 8)) ^ -1));
  return;
}

static int32_t script_D9EF(int32_t arg0) {
  return Script_ScenDataUtil(&*(int32_t *)&task_200, 1, arg0 / 8) & (1 << (arg0 % 8));
}

static void script_DA0C(int32_t arg0) {
  while (script_D9EF(arg0) == 0) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_DA2B(int32_t arg0, float arg1) {
  for (;;) {
    if (Script_GetCharaMotionRemainingFramesScaled(arg0) >= arg1) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_DA47(int32_t arg0) {
  while (Script_IsCharaMotionFinished(arg0) == 0) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static int32_t script_DA63(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Vector3i local_F4;
  int32_t local_F0;
  Script_mtwkGetSetRotation(arg0, &local_F4, 0);
  local_F0 = arg1 - (&local_F4)->y;
  if (local_F0 < 0) {
    local_F0 = -local_F0;
  }
  if (local_F0 >= 0x8000) {
    local_F0 = local_F0 - 0x10000;
    if (local_F0 < 0) {
      local_F0 = -local_F0;
    }
  }
  if (local_F0 <= arg2) {
    (&local_F4)->y = arg1;
    Script_EVT_SetRotation(arg0, 0x38000000, &local_F4, 0);
    (void)1;
  } else {
    if (arg3 == 0) {
      (&local_F4)->y = (&local_F4)->y - arg2;
    } else {
      (&local_F4)->y = (&local_F4)->y + arg2;
    }
    Script_EVT_SetRotation(arg0, 0x38000000, &local_F4, 0);
    (void)0;
  }
  end:;
}

static int32_t script_DB1E(void) {
  return Script_ShowUIMessageFromID("Q1003B001");
}

static int32_t script_DB2A(void) {
  return Script_ShowUIMessageFromID("Q1003B002");
}

static int32_t script_DB36(void) {
  return Script_ShowUIMessageFromID("Q1003B004");
}

static int32_t script_DB42(void) {
  return Script_ShowUIMessageFromID("Q1003A003");
}

static int32_t script_DB4E(void) {
  return Script_ShowUIMessageFromID("Q1003A004");
}

static int32_t script_DB5A(void) {
  return Script_ShowUIMessageFromID("Q1003A005");
}

static int32_t script_DB66(void) {
  return Script_ShowUIMessageFromID("Q1003A001");
}

static int32_t script_DB72(void) {
  return Script_ShowUIMessageFromID("Q1003A002");
}

static int32_t script_DB7E(int32_t arg0) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t local_E4;
  int32_t local_E0;
  Script_charaDisplay(arg0, CDM_Show);
  Script_ccowSetFlag80(arg0, 1);
  Script_SetCharaFlags(arg0, 4);
  Script_SetShadowFlags(arg0, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_SetShadowFlags(arg0, SCRIPT_SHADOW_UPDATE_LIGHT_MASK, 7);
  Script_scenToggleActiveFor(arg0, 0);
  Script_EVT_SetPosition(arg0, POS_XYZ, &data_16C80);
  Script_EVT_SetRotation(arg0, 0x38000000, &data_16C8C, 0);
  script_DA0C(3);
  Script_executeAicaCommand(132265, 0, 0);
  local_E4 = 2;
  local_E0 = 1;
  while (1) {
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
    Script_InterpolateAndScaleAngles(0, local_E4, &local_F4, &local_E8, "KCDT");
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
    local_F4.x = (&local_F4)->x * 10.0f + (&data_16C80)->x;
    (&local_F4)->y = (&local_F4)->y * 10.0f + (&data_16C80)->y;
    (&local_F4)->z = (&local_F4)->z * 10.0f + (&data_16C80)->z;
    Script_EVT_SetPosition(arg0, POS_XYZ, &local_F4);
    Script_EVT_SetRotation(arg0, 0x38000000, &local_E8, 0);
    local_E4 = local_E4 + local_E0;
    if ((local_E4 >= 40) & (script_D9EF(7) == 0)) {
      Script_scenToggleActiveFor(arg0, 1);
    } else {
      Script_scenToggleActiveFor(arg0, 0);
    }
    if (script_D9EF(7)) {
      break;
    }
    if (local_E4 >= 85) {
      Script_EVT_SetRotation(arg0, 0x38000000, &data_16CA0, 0);
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_DD02(int32_t arg0) {
  int32_t local_F4[3];
  int32_t local_E8[3];
  Script_charaDisplay(arg0, CDM_Show);
  Script_ccowSetFlag80(arg0, 1);
  Script_ccowUtils(arg0, 2);
  Script_motmToggleFlags(arg0, 2, 0x40);
  Script_scenToggleActiveFor(arg0, 0);
  Script_EVT_SetPosition(arg0, POS_XYZ, &data_16CAC);
  local_F4[1] = 0xC000;
  Script_EVT_SetRotation(arg0, MATRIX_XFORM_Y, (const Vector3i *)local_F4, 0);
  Script_PlayCharaMotion(arg0, 4394, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  script_DA0C(1);
  Script_motmToggleFlags(arg0, 1, 0x40);
  Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_12);
  Script_PlayCharaMotion(arg0, 4394, 800.0f, -1.0f, -1.0f, 1.0f, 0);
  SCNF_WaitFrames_0000(1);
  script_DA2B(arg0, 840.0f);
  Script_executeAicaCommand(394409, 0, 0);
  script_DA2B(arg0, 850.0f);
  Script_executeAicaCommand(0x1300A0, 4, 0);
  Script_executeAicaCommand(7849, 0, 0);
  Script_EVT_GetPosition(arg0, 12, (Vector3 *)local_F4, 0);
  local_E8[0] = 0;
  local_E8[1] = 0;
  local_E8[2] = 0;
  local_F4[0] = 0x3E90624E;
  local_F4[1] = 0x3E2E147B;
  local_F4[2] = -0x429A9FBE;
  local_E8[0] = 1388;
  local_E8[1] = 44968;
  local_E8[2] = 25164;
  script_DA0C(4);
  if (script_D9EF(5)) {
    Script_scenToggleActiveFor(arg0, 1);
  } else {
    script_DA0C(6);
    Script_EVT_SetPosition(arg0, POS_XYZ, &data_16CB8);
    local_F4[1] = 0xC000;
    Script_EVT_SetRotation(arg0, MATRIX_XFORM_Y, (const Vector3i *)local_F4, 0);
    Script_motmToggleFlags(arg0, 2, 0x40);
    Script_PlayCharaMotion(arg0, 4396, 70.0f, -1.0f, -1.0f, 1.0f, 0);
    script_DA0C(8);
    Script_EVT_SetPosition(arg0, POS_XYZ, &data_16CC4);
    SCNF_WaitFrames_0000(20);
  }
  end:;
}

static int32_t script_DEFF(int32_t arg0) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t local_E4;
  int32_t local_D8[3];
  Script_charaDisplay(arg0, CDM_Show);
  Script_FaceEx(arg0, 1, 0, 1);
  Script_charaVisUtils(arg0, 0);
  Script_FACE_LCG(arg0, 0, 10, 0);
  Script_charaToggleMotEnabled(arg0, 1);
  Script_ccowSetFlag80(arg0, 1);
  Script_ccowUtils(arg0, 2);
  Script_motmToggleFlags(arg0, 2, 0x40);
  Script_SetCharaFlags(arg0, 4);
  Script_SetShadowFlags(arg0, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_SetShadowFlags(arg0, SCRIPT_SHADOW_UPDATE_LIGHT_MASK, 7);
  Script_MotStepUtil(arg0, 1);
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_EVT_SetPosition(arg0, POS_XYZ, &data_16CD0);
  local_D8[1] = 0x4000;
  Script_EVT_SetRotation(arg0, MATRIX_XFORM_Y, (const Vector3i *)local_D8, 0);
  Script_PlayCharaMotion(arg0, 4353, 133.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_SetCharaMotionPlaybackRate(arg0, 0.0f);
  script_DA0C(0);
  Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
  SCNF_WaitFrames_0000(1);
  script_DA2B(arg0, 235.0f);
  Script_clmdSetFlags(arg0, 4, 1);
  SCNF_WaitFrames_0000(1);
  script_DA2B(arg0, 360.0f);
  Script_executeAicaCommand(66729, 0, 0);
  script_DA0C(4);
  Script_motmToggleFlags(arg0, 1, 0x40);
  if (script_D9EF(5)) {
    Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_8);
    Script_PlayCharaMotion(arg0, 4363, 66.0f, -1.0f, -1.0f, 1.0f, 0);
    script_DA0C(7);
    Script_executeAicaCommand(263337, 0, 0);
    Script_EVT_GetPosition(arg0, 18, (Vector3 *)local_D8, 0);
    {
      Vector3i local_CC;
      local_CC.x = 0;
      (&local_CC)->y = 0;
      (&local_CC)->z = 0;
      Script_EVT_SetPosition(SHEN_NAME('KAIS'), POS_XYZ, (const Vector3 *)local_D8);
      Script_EVT_SetRotation(SHEN_NAME('KAIS'), 0x38000000, &local_CC, 0);
    }
    local_D8[0] = 0x3E2E147B;
    local_D8[1] = 0x3E0F5C29;
    local_D8[2] = 0x3CF5C28F;
    {
      Vector3i local_CC;
      local_CC.x = 12854;
      (&local_CC)->y = 45332;
      (&local_CC)->z = 13152;
      Script_fixoAttachEx(SHEN_NAME('KAIS'), arg0, 18, (const Vector3 *)local_D8, &local_CC);
    }
    SCNF_WaitFrames_0000(1);
    script_DA2B(arg0, 92.0f);
    Script_executeAicaCommand(328873, 0, 0);
    script_DA2B(arg0, 140.0f);
    Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_30);
    Script_PlayCharaMotion(arg0, 4378, 70.0f, -1.0f, -1.0f, 1.0f, 0);
    local_E4 = 0x4000;
    script_DA2B(arg0, 150.0f);
    script_DA47(arg0);
    Script_mtwkGetSetRotation(arg0, &local_E8, 0x2000000);
    Script_EVT_SetRotation(arg0, 0x38000000, &local_E8, 0);
    Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_5);
    Script_PlayCharaMotion(arg0, MN_AKI_AKI_RUN_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_clmdSetFlags(arg0, 4, 0);
    while (1) {
      script_DA63(arg0, local_E4, 4551, 1);
      SCNF_WaitFrames_0000(1);
    }
  } else {
    Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_8);
    Script_PlayCharaMotion(arg0, 4382, 60.0f, -1.0f, -1.0f, 1.0f, 0);
    SCNF_WaitFrames_0000(1);
    script_DA2B(arg0, 75.0f);
    Script_executeAicaCommand(197801, 0, 0);
    script_DA2B(arg0, 120.0f);
    Script_PlayCharaMotion(arg0, 4379, 40.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_SetCharaMotionPlaybackRate(arg0, 0.8f);
    local_E4 = 0xC000;
    script_DA47(arg0);
    Script_mtwkGetSetRotation(arg0, &local_E8, 0x2000000);
    Script_EVT_SetRotation(arg0, 0x38000000, &local_E8, 0);
    Script_PlayCharaMotion(arg0, MN_AKI_AKI_RUN_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_clmdSetFlags(arg0, 4, 0);
    while (1) {
      if (script_DA63(arg0, local_E4, 4551, 1)) {
        break;
      }
      SCNF_WaitFrames_0000(1);
    }
    while (1) {
      Script_EVT_GetPosition(arg0, -1, &local_F4, 0);
      if (!((&local_F4)->x < 3.64800262f)) {
        break;
      }
      SCNF_WaitFrames_0000(1);
    }
    while (1) {
      if (script_DA63(arg0, 0xE000, 546, 0)) {
        break;
      }
      SCNF_WaitFrames_0000(1);
    }
    Script_MotStepUtil(arg0, 0);
  }
  end:;
}

static int32_t script_E35E(void) {
  return SCNF_UIUtils(4, 15, 2, 13, 1, 0);
}

__attribute__((scn_string_bindings(0, data_16CDC, 1, data_16D2C)))
static void script_E373(void) {
  int32_t unused_F8[2];
  int32_t local_F4;
  int32_t local_F0;
  int32_t local_EC;
  int32_t local_E8;
  int32_t local_E4;
  int32_t local_E0;
  float local_DC;
  float local_D8;
  int8_t local_D7;
  uint8_t unused_D4[3];
  int32_t local_D0;
  int32_t local_CC;
  int32_t local_C8;
  int32_t local_C4;
  Script_SetSfxOverlayTimeoutTuning(0, SfxMode_SessionAction);
  Script_SetSfxOverlayTimeoutTuning(0, SfxMode_DisableBoost);
  Script_uiUtils(0, 21, 0);
  Script_SetShouldSleepFlag(0);
  Script_LoadVoiceFile(1, "Q1003");
  Script_facemotUtils(CHARA_AKIR, 0, 0);
  Script_FaceEx(CHARA_KEBA, 1, 0, 2);
  (void)((local_D7 = 0) & 1);
  while (1) {
    local_E0 = 0;
    while (local_E0 < 9) {
      script_D9C1(local_E0);
      local_E0 = local_E0 + 1;
    }
    Script_shenSetLightingMode(2);
    Script_SetupLightFromTable(8);
    Script_ProcessFogFromTable(1);
    local_DC = Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 40.0f);
    local_D8 = Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 20.0f);
    local_F4 = Script_scenCreateTask(script_DEFF, 1, CHARA_AKIR);
    *(&local_F4 + 1) = Script_scenCreateTask(script_DD02, 1, CHARA_KEBA);
    *(&local_F4 + 2) = Script_scenCreateTask(script_DB7E, 1, SHEN_NAME('KAIS'));
    if (0) {
      Script_charaDisplay(SHEN_NAME('KAIS'), CDM_Hide);
    } else {
      if (-1) {
        Script_charaDisplay(SHEN_NAME('KAIS'), CDM_Show);
        goto L00E494;
      }
    }
    L00E494:;
    Script_scenToggleVisibleFor(SHEN_NAME('KAIS'), 1);
    Script_charaDisplay(SHEN_NAME('TEL_'), CDM_Show);
    Script_ccowSetFlag80(SHEN_NAME('TEL_'), 1);
    Script_EVT_SetPosition(SHEN_NAME('TEL_'), POS_XYZ, &data_16CE4);
    Script_EVT_SetRotation(SHEN_NAME('TEL_'), 0x38000000, &data_16D14, 0);
    Script_charaDisplay(SHEN_NAME('ISU_'), CDM_Show);
    Script_ccowSetFlag80(SHEN_NAME('ISU_'), 1);
    Script_EVT_SetPosition(SHEN_NAME('ISU_'), POS_XYZ, &data_16CF0);
    *(&local_C8 + 1) = 0x4000;
    Script_EVT_SetRotation(SHEN_NAME('ISU_'), MATRIX_XFORM_Y, (const Vector3i *)&local_C8, 0);
    if (0) {
      Script_charaDisplay(SHEN_NAME('MAGZ'), CDM_Hide);
    } else {
      if (-1) {
        Script_charaDisplay(SHEN_NAME('MAGZ'), CDM_Show);
        goto L00E54B;
      }
    }
    L00E54B:;
    Script_scenToggleVisibleFor(SHEN_NAME('MAGZ'), 1);
    Script_charaPosRotUtils(SHEN_NAME('MAGZ'), 152, 0, &data_16D20, 0);
    if (0) {
      Script_charaDisplay(SHEN_NAME('PEN_'), CDM_Hide);
    } else {
      if (-1) {
        Script_charaDisplay(SHEN_NAME('PEN_'), CDM_Show);
        goto L00E595;
      }
    }
    L00E595:;
    Script_scenToggleVisibleFor(SHEN_NAME('PEN_'), 1);
    if (0) {
      Script_charaDisplay(SHEN_NAME('CLIP'), CDM_Hide);
    } else {
      if (-1) {
        Script_charaDisplay(SHEN_NAME('CLIP'), CDM_Show);
        goto L00E5C9;
      }
    }
    L00E5C9:;
    Script_scenToggleVisibleFor(SHEN_NAME('CLIP'), 1);
    Script_EVT_SetPosition(SHEN_NAME('GATL'), POS_XYZ, &data_16CFC);
    Script_EVT_SetPosition(SHEN_NAME('GATR'), POS_XYZ, &data_16D08);
    Script_ccowUtils(SHEN_NAME('TEL_'), 2);
    Script_ccowUtils(SHEN_NAME('KAIS'), 2);
    Script_ccowUtils(SHEN_NAME('ISU_'), 2);
    local_F0 = 0;
    local_EC = 0;
    local_E8 = 0;
    local_E4 = 0;
    Script_PlayerStatsUtils(PS_Set70MBTimeCapSlot, 9, Script_PlayerStatsUtils(PS_Get70MBTimeCapSlot, 9) + 1);
    while (local_E8 != -1) {
      local_D0 = local_E8;
      switch (local_D0) {
        case 0:
          Script_filmCheckOrSetTransitionUtil(32, 255, 0, 0, 0, 0, 0, 0, 0);
          local_E8 = 1;
          break;
        case 1:
          if (Script_filmCheckOrSetTransitionUtil(-1) == 0) {
            local_E8 = 2;
          }
          break;
        case 2:
          if (local_EC == 5) {
            script_D996(0);
            local_E8 = 3;
          }
          break;
        case 3:
          if (local_EC == 218) {
            Script_executeAicaCommand(1193, 0, 0);
            script_D996(1);
            local_E8 = 4;
          }
          break;
        case 4:
          if (!(Script_GetCharaMotionRemainingFramesScaled(CHARA_AKIR) < 360.0f)) {
            script_D996(2);
            local_E8 = 5;
          }
          break;
        case 5:
          if (!(Script_GetCharaMotionRemainingFramesScaled(CHARA_AKIR) < 380.0f)) {
            script_D996(3);
            local_E8 = 6;
          }
          break;
        case 6:
          if (local_EC == 14) {
            Script_scenCreateTask(script_E35E, 0);
            local_E8 = 7;
          }
          break;
        case 7:
          local_CC = SCNF_UIUtils(0, 0, 0, 0, 0, -1);
          if (local_CC != 0xFFFF) {
            if ((local_CC & 4) == 4) {
              script_D996(5);
            }
          }
          if (local_EC >= 30) {
            if (script_D9EF(5)) {
              local_E8 = 8;
            } else {
              local_E8 = 9;
            }
            script_D996(4);
          }
          break;
        case 8:
          if (local_EC == 14) {
            script_D996(7);
          }
          if (local_F0 == 580) {
            Script_filmCheckOrSetTransitionUtil(32, 0, 0, 0, 0, 255, 0, 0, 0);
            local_E8 = 10;
            Script_executeAicaCommand(2720, 2, 30);
          }
          break;
        case 9:
          if (local_EC == 100) {
            script_D996(6);
          }
          if (local_F0 == 625) {
            Script_filmCheckOrSetTransitionUtil(32, 0, 0, 0, 0, 255, 0, 0, 0);
            local_E8 = 10;
          }
          break;
        case 10:
          if (Script_filmCheckOrSetTransitionUtil(-1) == 0) {
            local_E8 = -1;
          }
          break;
      }
      local_D0 = local_F0;
      switch (local_D0) {
        case 0:
          Script_seqEnterEvtCam(1100, 0, AUTH_None);
          break;
        case 75:
          Script_seqEnterEvtCam(1101, 0, AUTH_None);
          break;
        case 255:
          Script_seqEnterEvtCam(1102, 0, AUTH_None);
          break;
        case 280:
          Script_seqEnterEvtCam(1103, 0, AUTH_None);
          break;
        case 310:
          Script_seqEnterEvtCam(1104, 0, AUTH_None);
          break;
        case 267:
          Script_SetCamShake(0.15f, 0.02f, 0.5f);
          break;
        case 299:
          Script_scenCreateTask(script_DB1E, 0);
          break;
        case 302:
          break;
      }
      if (script_D9EF(5)) {
        local_D0 = local_F0;
        switch (local_D0) {
          case 331:
            Script_seqEnterEvtCam(1110, 0, AUTH_None);
            Script_scenCreateTask(script_DB42, 0);
            break;
          case 365:
            Script_seqEnterEvtCam(1111, 0, AUTH_None);
            break;
          case 391:
            Script_scenCreateTask(script_DB4E, 0);
            break;
          case 485:
            Script_scenCreateTask(script_DB5A, 0);
            break;
        }
      } else {
        local_D0 = local_F0;
        switch (local_D0) {
          case 341:
            Script_seqEnterEvtCam(1120, 0, AUTH_None);
            break;
          case 374:
            Script_seqEnterEvtCam(1121, 0, AUTH_None);
            break;
          case 491:
            script_D996(8);
            Script_seqEnterEvtCam(1122, 0, AUTH_None);
            break;
          case 361:
            Script_scenCreateTask(script_DB66, 0);
            break;
          case 391:
            Script_scenCreateTask(script_DB72, 0);
            break;
          case 432:
            Script_scenCreateTask(script_DB2A, 0);
            break;
          case 511:
            Script_scenCreateTask(script_DB36, 0);
            break;
        }
      }
      local_F0 = local_F0 + 1;
      if (local_E8 != local_E4) {
        local_EC = 0;
        local_E4 = local_E8;
      } else {
        local_EC = local_EC + 1;
      }
      SCNF_WaitFrames_0000(1);
    }
    local_E0 = 0;
    while (local_E0 < 3) {
      Script_srchControl((void *)*(&local_F4 + local_E0));
      local_E0 = local_E0 + 1;
    }
    Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, local_DC);
    Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, local_D8);
    Script_FaceLookAtTargetUtils(CHARA_AKIR, 0);
    if (script_D9EF(5)) {
      Script_fixoClearAttachment(SHEN_NAME('KAIS'));
      SCNF_WaitFrames_0000(1);
      break;
    } else {
      Script_IncrementDateTimeByNum(1);
      Script_ReturnTimeDate((GameDateTime *)&local_C8);
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_AFTER_8PM_GUARD_SHIFT)) {
        *((int8_t *)&local_C8 + 4) = 20;
      } else {
        *((int8_t *)&local_C8 + 4) = 19;
      }
      *((int8_t *)&local_C8 + 5) = 0;
      *((int8_t *)&local_C8 + 6) = 0;
      Script_SetNewDateTime((GameDateTime *)&local_C8);
      if ((*((int8_t *)&local_C8 + 1) == 4) & (*((int8_t *)&local_C8 + 2) == 15)) {
        *((int8_t *)&local_C8 + 4) = 7;
        *((int8_t *)&local_C8 + 5) = 59;
        *((int8_t *)&local_C8 + 6) = 0;
        Script_SetNewDateTime((GameDateTime *)&local_C8);
        Script_shenRequestGameModuleTransition(2, SHEN_NAME('JOMO'), 4);
        SCNF_WaitFrames_0000(1);
      } else {
        if (*((int8_t *)&local_C8 + 3) == 1) {
          script_AF73();
          Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 189, 1);
          goto L00EBEA;
        }
      }
      L00EBEA:;
      Script_WeatherTaskUtil(FreeTaskData);
      Script_WeatherTaskUtil(InitTaskData);
      Script_WeatherTaskUtil(AndNotInstructionFlags, 0x784);
      Script_SetupLightFromTable(8);
      Script_ProcessFogFromTable(1);
      Script_cdttLoadScrUtils(0, 0);
      SCNF_WaitFrames_0000(120);
      Script_cdttLoadScrUtils(1, 0);
      local_C4 = Script_shenGetStats(STATS_CASH);
      Script_SetStats(STATS_CASH, local_C4 + 180);
      Script_PlayerStatsUtils(PS_SetStatU8_1, Script_PlayerStatsUtils(PS_GetStatU8_1) + 1);
      Script_PlayerStatsUtils(PS_SetStatU8_0, Script_PlayerStatsUtils(PS_GetStatU8_0) + 1);
      if (local_D7 == 0) {
        Script_PlayerStatsUtils(PS_Set70MBCounter1, Script_PlayerStatsUtils(PS_Get70MBCounter1) + 1);
        (void)((local_D7 = -1) & 4);
      }
    }
  }
  Script_FaceEx(CHARA_KEBA, 0, 0, 2);
  Script_LoadVoiceFile(0, "Q1003");
  Script_ccowUtils(CHARA_AKIR, 1);
  Script_EVT_SetPosition(SHEN_NAME('GATL'), POS_XYZ, &data_16D34);
  Script_EVT_SetPosition(SHEN_NAME('GATR'), POS_XYZ, &data_16D40);
  Script_SetSfxOverlayTimeoutTuning(2, SfxMode_SessionAction);
  Script_MotStepUtil(CHARA_AKIR, 1);
  Script_PlayerStatsUtils(PS_Set70MBCounter0, Script_PlayerStatsUtils(PS_Get70MBCounter0) + 1);
  script_D9EF(5);
  Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_MKSG_HELP_ROUTE_ACTIVE, 1);
  return;
}

static void script_ED14(const char *arg0, const char *arg1) {
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, arg0, arg1) == 0) {
    SCNF_WaitFrames_0000(1);
  }
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

__attribute__((scn_string_bindings(0, data_16D4C)))
static int32_t script_ED4E(const char *arg0) {
  script_ED14("scene/02/MKSG", arg0);
  return (task_202 = 0) & 0x20;
}

static void script_ED69(int32_t arg0, int32_t arg1, int32_t arg2) {
  if (arg1 == 2) {
    Script_seqEnterEvtCam(5000, 0, AUTH_None);
  }
  (void)((task_184 = 0) & 0x20);
  Script_mapmSetCamFlags(1, 6, 0);
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
  Script_CycleManagerUtils(CYCLE_CMD_PROCESS_SND_EXTRA);
  (void)((task_202 = -1) & 0x20);
  Script_scenCreateTask(script_ED4E, 1, arg0);
  SCNF_WaitFrames_0000(90);
  Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  script_F55C(arg2);
  while (task_202 & 8) {
    SCNF_WaitFrames_0000(1);
  }
  Script_mapmSetCamFlags(1, 9, 1);
  Script_mapmSetRenderFlag(9, 1);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 30.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 30.0f);
  Script_LODUtils(LOD_UTILS_SET_NPC_DISPLAY_RADIUS, 20.0f);
  Script_WeatherTaskUtil(ToggleFogTask, 0);
  Script_WeatherTaskUtil(ToggleSceneLightingFlags, 0);
  Script_SetupLightFromTable(0);
  Script_SetGlobalSceneSlotValue34(0, 1);
  Script_SetGlobalSceneSlotValue34(1, 1);
  Script_SetGlobalSceneSlotValue34(2, 1);
  Script_SetGlobalSceneSlotValue34(3, 1);
  Script_SetGlobalSceneSlotValue34(4, 1);
  Script_SetGlobalSceneSlotValue34(5, 1);
  Script_SetGlobalSceneSlotValue34(6, 1);
  Script_SetGlobalSceneSlotValue34(7, 1);
  Script_SetGlobalSceneSlotValue34(8, 1);
  Script_PREP_Related(-1, 0);
  Script_SetupLightFromTable(arg1);
  Script_PREP_Related(9, 1);
  Script_SetCharaFlags(CHARA_AKIR, 4);
  while ((&task_10)->mode) {
    SCNF_WaitFrames_0000(1);
  }
  SCNF_WaitFrames_0000(30);
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 1);
  if (arg1 == 2) {
    Script_SetGameStateTo(AUTH_None);
  }
  Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_ryoControl(RYO_RestoreControl);
  (void)((task_184 = -1) & 0x20);
  return;
}

static void script_EF3F(const char *arg0) {
  int32_t unused_FC;
  GameDateTime local_F4;
  (void)((task_184 = 0) & 0x20);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 70.0f);
  Script_LODUtils(LOD_UTILS_SET_NPC_DISPLAY_RADIUS, 48.0f);
  SCNF_WaitFrames_0000(120);
  Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  script_F55C(0);
  Script_WeatherTaskUtil(ToggleFogTask, 1);
  Script_WeatherTaskUtil(ToggleSceneLightingFlags, 1);
  Script_SetGlobalSceneSlotValue34(0, 0);
  Script_SetGlobalSceneSlotValue34(1, 0);
  Script_SetGlobalSceneSlotValue34(2, 0);
  Script_SetGlobalSceneSlotValue34(3, 0);
  Script_SetGlobalSceneSlotValue34(4, 0);
  Script_SetGlobalSceneSlotValue34(5, 0);
  Script_SetGlobalSceneSlotValue34(6, 0);
  Script_SetGlobalSceneSlotValue34(7, 0);
  Script_SetGlobalSceneSlotValue34(8, 0);
  Script_PREP_Related(-1, 0);
  Script_ReturnTimeDate(&local_F4);
  if ((&local_F4)->hours >= 19) {
    script_82BB();
    script_83B8(8);
    Script_SetupLightFromTable(0);
    Script_ProcessFogFromTable(0);
    Script_mapmSetRenderFlag(3, 0);
    Script_mapmSetRenderFlag(7, 1);
    Script_PREP_Related(-1, 1);
    Script_SetCharaFlags(CHARA_AKIR, 0);
    Script_SetShadowFlags(0, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 2);
  } else {
    Script_SetCharaFlags(CHARA_AKIR, 4);
    Script_shenSetLightingMode(1);
    Script_LightUtils(0, 0, 2);
    Script_lightSetSlotEnabled(0, 1);
  }
  Script_mapmSetCamFlags(1, 6, 1);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 70.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 60.0f);
  Script_LODUtils(LOD_UTILS_SET_NPC_DISPLAY_RADIUS, 48.0f);
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, arg0);
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PROCESS_COLS, 0);
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 1);
  while ((&task_10)->mode) {
    SCNF_WaitFrames_0000(1);
  }
  SCNF_WaitFrames_0000(10);
  Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_ryoControl(RYO_RestoreControl);
  (void)((task_184 = -1) & 8);
  return;
}

static int32_t script_F131(void) {
  int32_t unused_FC;
  GameDateTime local_F4;
  Vector3 local_E8;
  Vector3 local_DC;
  int32_t local_D8;
  int8_t local_D7;
  int8_t local_D6;
  int8_t local_D5;
  int8_t local_D4;
  int8_t local_D3;
  uint8_t unused_D0[3];
  int32_t local_CC;
  (void)((local_D3 = -1) & 8);
  if (task_1B8) {
    local_D4 = Script_ryoSleepUtils(3, 0, 0);
  } else {
    Script_ryoSleepUtils(3, 1, 0, 1);
    Script_ryoSleepUtils(3, 1, 1, 0);
    local_D4 = 1;
  }
  Script_ReturnTimeDate(&local_F4);
  if ((&local_F4)->hours >= 19) {
    Script_SetCharaFlags(CHARA_AKIR, 0);
    Script_SetShadowFlags(0, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 2);
    (void)((local_D3 = 0) & 8);
  }
  for (;;) {
    Script_ReturnTimeDate(&local_F4);
    if (((&task_10)->mode != 0) & (task_10.id == 0x52494B41)) {
      local_D7 = 0;
      local_CC = task_10.entry_idx;
      switch (local_CC) {
        case 0:
          local_D8 = (int32_t)data_16D5A;
          local_D6 = 1;
          local_D7 = 71;
          local_D5 = 1;
          break;
        case 2:
          local_D8 = (int32_t)data_16D62;
          local_D6 = 2;
          local_D7 = 72;
          local_D5 = 2;
          break;
        case 4:
          local_D8 = (int32_t)data_16D6A;
          local_D6 = 3;
          local_D7 = 73;
          local_D5 = 3;
          break;
        case 5:
          local_D8 = (int32_t)data_16D72;
          local_D6 = 4;
          local_D7 = 74;
          local_D5 = 4;
          break;
        case 6:
          local_D8 = (int32_t)data_16D7A;
          local_D6 = 5;
          local_D7 = 75;
          local_D5 = 5;
          break;
        case 9:
          local_D8 = (int32_t)data_16D82;
          local_D6 = 6;
          local_D7 = 77;
          local_D5 = 7;
          break;
        case 1:
          local_D8 = (int32_t)data_16D8A;
          local_D6 = 7;
          local_D7 = 78;
          local_D5 = 8;
          break;
        case 8:
          Script_WeatherUtils(WeatherScript_SetWccpClipSet0Entry, 6, 1);
          Script_scenUtils(SCENARIO_CMD_INIT_SCNT);
          SCNF_WaitFrames_0000(90);
          Script_executeAicaCommand(2720, 1, 30);
          Script_executeAicaCommand(2720, 2, 30);
          Script_executeAicaCommand(2720, 3, 30);
          Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
          while (Script_filmCheckOrSetTransitionUtil(-1)) {
            SCNF_WaitFrames_0000(1);
          }
          if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_YDB1_SECRET_BOX_OPENING_COMPLETE) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_MS8A_POST_MS08_FOLLOWUP_COMPLETE) == 0)) {
            Script_shenRequestGameModuleTransition(2, SHEN_NAME('MS8A'), 90);
            SCNF_WaitFrames_0000(1);
          } else {
            Script_shenRequestGameModuleTransition(2, SHEN_NAME('MS08'), 0);
            SCNF_WaitFrames_0000(1);
          }
          break;
      }
      if (local_D4 == 1) {
        if (local_D7) {
          Script_PlayerStatsUtils(PS_IncrementSpotVisitCount, local_D7);
        }
        Script_WeatherUtils(WeatherScript_SetWccpClipSet0Entry, local_D5, 1);
        script_ED69(local_D8, local_D6, local_D7);
        Script_ryoSleepUtils(3, 1, 0, 0);
        Script_ryoSleepUtils(3, 1, 1, local_D7);
        local_D4 = 0;
      } else {
        script_EF3F((const char *)local_D8);
        Script_WeatherUtils(WeatherScript_SetWccpClipSet0Entry, local_D5, 0);
        if ((&local_F4)->hours >= 19) {
          (void)((local_D3 = 0) & 8);
        }
        Script_ryoSleepUtils(3, 1, 0, 1);
        Script_ryoSleepUtils(3, 1, 1, 0);
        local_D4 = 1;
      }
    } else {
      if (local_D3 & -(Script_IsPlayerAllowed(0) != 0) & local_D4) {
        if ((&local_F4)->hours >= 19) {
          script_F9A4();
          (void)((local_D3 = 0) & 8);
        }
        goto L00F477;
      }
    }
    L00F477:;
    local_E8.x = 0.2f;
    (&local_E8)->y = 0.0f;
    (&local_E8)->z = -9.48f;
    local_DC.x = 1.71f;
    (&local_DC)->y = 0.0f;
    (&local_DC)->z = -4.11f;
    if (Script_SetTaskDataValue(CHARA_AKIR, &local_E8, &local_DC, 0)) {
      Script_ryoControl(PCCTL_DisablePlayerControl);
      Script_shenClearFreeState(0);
      Script_executeAicaCommand(2720, 1, 30);
      Script_executeAicaCommand(2720, 2, 30);
      Script_executeAicaCommand(2720, 3, 30);
      Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
      while (Script_filmCheckOrSetTransitionUtil(-1)) {
        SCNF_WaitFrames_0000(1);
      }
      Script_shenRequestGameModuleTransition(2, SHEN_NAME('MFSY'), 1);
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_F537(void) {
  while (Script_filmCheckOrSetTransitionUtil(-1) == 0) {
    SCNF_WaitFrames_0000(1);
  }
  Script_cdttLoadScrUtils(1, 0);
  end:;
}

static void script_F55C(int32_t arg0) {
  int32_t local_FC;
  local_FC = arg0;
  if (local_FC == 71) {
    Script_cdttLoadScrUtils(2, 0x31534B4D);
  } else if (local_FC == 72) {
    Script_cdttLoadScrUtils(2, 0x32534B4D);
  } else if (local_FC == 73) {
    Script_cdttLoadScrUtils(2, 0x34534B4D);
  } else if (local_FC == 74) {
    Script_cdttLoadScrUtils(2, 0x35534B4D);
  } else if (local_FC == 75) {
    Script_cdttLoadScrUtils(2, 0x36534B4D);
  } else if (local_FC == 77) {
    Script_cdttLoadScrUtils(2, 0x39534B4D);
  } else if (local_FC == 78) {
    Script_cdttLoadScrUtils(2, 0x30534B4D);
  } else {
    Script_cdttLoadScrUtils(2, 0x47534B4D);
  }
  SCNF_WaitFrames_0000(30);
  Script_scenCreateTask(script_F537, 0);
  return;
}

static void script_F62C(void) {
  int32_t unused_FC;
  GameDateTime local_F4;
  if (Script_WeatherUtils_0(WeatherDataUtil_QueryCurrentFCASFlag, 3)) {
    Script_mapmSetRenderFlag(2, 1);
    Script_mapmSetRenderFlag(4, 0);
    Script_mapmSetRenderFlag(5, 1);
  } else if (Script_WeatherUtils_0(WeatherDataUtil_QueryCurrentFCASFlag, 2)) {
    Script_mapmSetRenderFlag(2, 0);
    Script_mapmSetRenderFlag(4, 0);
    Script_mapmSetRenderFlag(5, 1);
  } else {
    Script_mapmSetRenderFlag(2, 0);
    Script_mapmSetRenderFlag(4, 1);
    Script_mapmSetRenderFlag(5, 0);
  }
  Script_ReturnTimeDate(&local_F4);
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_MS08_SCENE_EXIT_COMPLETE)) {
    if ((&local_F4)->hours < 19) {
      Script_shenSetLightingMode(1);
      Script_LightUtils(0, 0, 2);
      Script_lightSetSlotEnabled(0, 1);
      Script_PREP_Related(-1, 0);
      Script_mapmSetRenderFlag(3, 1);
      Script_mapmSetRenderFlag(7, 0);
    } else {
      Script_SetupLightFromTable(0);
      Script_PREP_Related(-1, 1);
      Script_mapmSetRenderFlag(3, 0);
      Script_mapmSetRenderFlag(7, 1);
    }
  } else {
    Script_mapmSetRenderFlag(3, 0);
    Script_mapmSetRenderFlag(7, 1);
    Script_PREP_Related(0, 1);
  }
  return;
}

static void script_F733(void) {
  int32_t unused_FC;
  GameDateTime local_F4;
  script_7FBB(CHARA_AKIR, (DSWKDoorEntry *)data_154BC, (int32_t *)data_1572C, (DoorResourceDescriptor *)&data_157D8, (MapObjectRecord *)data_15828, 10, 0);
  SCNF_WaitFrames_0000(2);
  DSWK_DoorUtils(DOOR_CMD_SET_STATE_1);
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_MS08_SCENE_EXIT_COMPLETE)) {
    Script_ReturnTimeDate(&local_F4);
    script_8834(3, 7, -1, 0x2302AB, -1, -1, -1);
    DSWK_AssignDoorEntryConfig(3, 0, -1, -1, -1, -1, -1, -1);
    DSWK_AssignDoorEntryConfig(7, 0, -1, -1, -1, -1, -1, -1);
    DSWK_AssignDoorEntryConfig(10, 0, -1, -1, -1, -1, -1, -1);
    if ((&local_F4)->hours < 19) {
      script_8381(3);
      script_8381(7);
      script_8381(10);
    } else {
      script_82BB();
      script_83B8(8);
    }
  }
  return;
}

static void script_F807(void) {
  Script_WeatherUtils(WeatherScript_LoadWccpClipSet0FromTaskOffset, &data_16D94, 9);
  script_F81D();
  return;
}

static void script_F81D(void) {
  int8_t local_FF;
  local_FF = 0;
  while (local_FF < 9) {
    Script_WeatherUtils(WeatherScript_SetWccpClipSet0Entry, local_FF, 0);
    local_FF = local_FF + 1;
  }
  Script_WeatherUtils(WeatherScript_SetWccpClipSet0Entry, 0, 1);
  return;
}

static int32_t script_F851(void) {
  int32_t local_FC;
  local_FC = script_F86B();
  Script_scenCreateTask(script_F93C, 1, local_FC);
  end:;
}

__attribute__((scn_string_bindings(0, data_16F98, 2, data_16FAE, 4, data_16FC6)))
static int32_t script_F86B(void) {
  int32_t local_FC;
  local_FC = Script_MT5Loader_TRWV_TRWAVE(0, &data_16F8C, "model/object", "SEA3.PVR", "model/object", "TRSEA4.MT6", 4.0f, 0);
  Script_MT5Loader_TRWV_TRWAVE(21, local_FC, 8, 8);
  Script_MT5Loader_TRWV_TRWAVE(16, local_FC, -1, 0);
  Script_MT5Loader_TRWV_TRWAVE(13, local_FC, 0.0901960805f, 0.196078435f);
  Script_MT5Loader_TRWV_TRWAVE(15, local_FC, "model/object", "TRWTBL1.BIN");
  Script_MT5Loader_TRWV_TRWAVE(12, local_FC, 0x4000);
  Script_MT5Loader_TRWV_TRWAVE(10, local_FC, 64.0f, 64.0f, 1, 1, 3, 1);
  Script_MT5Loader_TRWV_TRWAVE(24, local_FC, 0, 203, 255, 143);
  Script_MT5Loader_TRWV_TRWAVE(11, local_FC, -0.51f, -0.68f, 0.52f);
  return local_FC;
}

static void script_F930(int32_t arg0) {
  Script_MT5Loader_TRWV_TRWAVE(3, arg0);
  return;
}

static int32_t script_F93C(int32_t arg0) {
  int8_t local_FF;
  (void)((local_FF = -1) & 1);
  for (;;) {
    if (Script_SetTaskDataValue(CHARA_AKIR, &data_16FE0, &data_16FEC, 0)) {
      if (local_FF == 0) {
        Script_MT5Loader_TRWV_TRWAVE(1, arg0);
        (void)((local_FF = -1) & 1);
      }
    } else {
      if (local_FF == -1) {
        Script_MT5Loader_TRWV_TRWAVE(2, arg0);
        (void)((local_FF = 0) & 1);
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

__attribute__((scn_string_bindings(0, data_16FF8, 1, data_17006, 2, data_1700C, 4, data_17013, 5, data_17018, 6, data_1701D, 8, data_1702D, 9, data_17033, 10, data_1703A)))
static void script_F9A4(void) {
  int32_t unused_FC;
  Vector3 local_F0;
  Vector3i local_E4;
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_FaceEx(CHARA_AKIR, 1, 0, 2);
  Script_ResetHandMotion(CHARA_AKIR, -1, 0, 0);
  Script_ResetHandMotion(CHARA_AKIR, -1, 1, 0);
  Script_shenClearFreeState(0);
  Script_scenUtils(SCENARIO_CMD_INIT_SCNT);
  Script_sicnUtils(1, 0, 0);
  SCNF_RequestIconNotification(0, 0, 0, 2);
  Script_EVT_GetPosition(CHARA_AKIR, -1, &local_F0, 0);
  Script_mtwkGetSetRotation(CHARA_AKIR, &local_E4, 0);
  Script_filmCheckOrSetTransitionUtil(20, 0, 0, 0, 0, 255, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_SetupLightFromTable(0);
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
  Script_CycleManagerUtils(CYCLE_CMD_PROCESS_SND_EXTRA);
  script_ED14("scene/02/MKSG", "NIGHT");
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
  Script_AuthorUtils(0, 0, "NIGHT", "");
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
  Script_LoadSND(0, 5, "FREE", 0);
  Script_LoadSND(0, 4, "FREE", 0);
  Script_LoadSND(0, 3, "FREE", 0);
  Script_LoadSND(0, 2, "bgm109.snd", 0);
  Script_filmCheckOrSetTransitionUtil(10, 255, 0, 0, 0, 0, 0, 0, 0);
  script_B7EB(0);
  Script_AuthorUtils(AUTH_FREE, 0);
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "NIGHT");
  script_F62C();
  Script_SetCharaFlags(CHARA_AKIR, 0);
  Script_SetShadowFlags(0, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 2);
  Script_LoadSND(0, 2, "SELECT", 0);
  Script_executeAicaCommand(16296, 0, 0);
  Script_executeAicaCommand(1184, 2, 100);
  Script_LoadSND(0, 4, "f1kyusok.snd", 0);
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 1);
  Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &local_F0);
  Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &local_E4, 0);
  Script_convSetCamInactiveFlag(-1);
  Script_SetGameStateTo(AUTH_None);
  Script_WeatherTaskUtil(FreeTaskData);
  Script_WeatherTaskUtil(InitTaskData);
  script_82BB();
  script_83B8(8);
  Script_filmCheckOrSetTransitionUtil(20, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_ryoControl(RYO_RestoreControl);
  Script_FaceEx(CHARA_AKIR, 0, 0, 2);
  Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
  Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
  Script_shenClearFreeState(1);
  Script_scenUtils(SCENARIO_CMD_UPDATE_TIMER);
  return;
}

static int32_t script_FC24(void) {
  script_AB36();
  for (;;) {
    if (script_AB88() != -1) {
      script_AB60();
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_FC49(void) {
  Script_srchUtils(SRCH_SCRIPT_InitEntryTableFromTaskOffset, &data_17048);
  for (;;) {
    Script_srchUtils(-1);
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_FC6A(void) {
  while (1) {
    if (Script_filmCheckOrSetTransitionUtil(-1)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_cdttLoadScrUtils(1, 0);
  end:;
}

static void script_FC93(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  int8_t local_F7;
  int8_t local_F6;
  int32_t local_F0;
  local_F0 = arg0;
  switch (local_F0) {
    case 71:
      local_FC = (int32_t)data_1706A;
      local_F7 = 1;
      local_F8 = 0;
      local_F6 = 1;
      break;
    case 72:
      local_FC = (int32_t)data_17072;
      local_F7 = 2;
      local_F8 = 2;
      local_F6 = 2;
      break;
    case 73:
      local_FC = (int32_t)data_1707A;
      local_F7 = 3;
      local_F8 = 4;
      local_F6 = 3;
      break;
    case 74:
      local_FC = (int32_t)data_17082;
      local_F7 = 4;
      local_F8 = 5;
      local_F6 = 4;
      break;
    case 75:
      local_FC = (int32_t)data_1708A;
      local_F7 = 5;
      local_F8 = 6;
      local_F6 = 5;
      break;
    case 77:
      local_FC = (int32_t)data_17092;
      local_F7 = 6;
      local_F8 = 9;
      local_F6 = 7;
      break;
    case 78:
      local_FC = (int32_t)data_1709A;
      local_F7 = 7;
      local_F8 = 1;
      local_F6 = 8;
      break;
  }
  Script_WeatherUtils(WeatherScript_SetWccpClipSet0Entry, local_F6, 1);
  script_83B8(local_F8);
  Script_mapmSetCamFlags(1, 6, 0);
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
  Script_CycleManagerUtils(CYCLE_CMD_PROCESS_SND_EXTRA);
  (void)((task_202 = -1) & 1);
  Script_scenCreateTask(script_ED4E, 1, local_FC);
  while (task_202 & 8) {
    SCNF_WaitFrames_0000(1);
  }
  Script_mapmSetCamFlags(1, 9, 1);
  Script_mapmSetRenderFlag(9, 1);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 30.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 30.0f);
  Script_LODUtils(LOD_UTILS_SET_NPC_DISPLAY_RADIUS, 20.0f);
  Script_WeatherTaskUtil(ToggleFogTask, 0);
  Script_WeatherTaskUtil(ToggleSceneLightingFlags, 0);
  Script_SetupLightFromTable(0);
  Script_SetGlobalSceneSlotValue34(0, 1);
  Script_SetGlobalSceneSlotValue34(1, 1);
  Script_SetGlobalSceneSlotValue34(2, 1);
  Script_SetGlobalSceneSlotValue34(3, 1);
  Script_SetGlobalSceneSlotValue34(4, 1);
  Script_SetGlobalSceneSlotValue34(5, 1);
  Script_SetGlobalSceneSlotValue34(6, 1);
  Script_SetGlobalSceneSlotValue34(7, 1);
  Script_SetGlobalSceneSlotValue34(8, 1);
  Script_PREP_Related(-1, 0);
  Script_SetupLightFromTable(local_F7);
  Script_PREP_Related(9, 1);
  Script_SetCharaFlags(CHARA_AKIR, 4);
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 1);
  return;
}

__attribute__((scn_string_bindings(0, data_170A2)))
void script_FED5(void) {
  int32_t unused_FC;
  int32_t local_F8;
  int32_t local_F4;
  task_1B8 = Script_ryoSleepUtils(0);
  Script_filmCheckOrSetTransitionUtil(0, 255, 0, 0, 0, 0, 0, 0, 0);
  Script_scenCreateTask(script_FC6A, 0);
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 10, -1);
  Script_SetMapFlags(0, 1);
  Script_SetCameraSettings(5, 6, 1, 0);
  Script_PlayerStatsUtils(PS_IncrementSpotVisitCount, 70);
  script_F733();
  Script_GetSetWeatherSettings(WEATHER_SETTINGS_SET_DISABLE_WEATHER, 1);
  Script_WeatherUtils_0(WeatherDataUtil_ApplyCurrentWeatherOrEnableDateTimeRead, 0);
  Script_WeatherTaskUtil(InitTaskData);
  script_F62C();
  script_F807();
  script_BA85();
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_MS08_SCENE_EXIT_COMPLETE) == 0) {
    script_ED14("scene/02/MKSG", "EVENT");
    script_BC86();
    Script_shenClearFreeState(0);
    Script_uiUtils(0, 30, 0);
    Script_ryoControl(PCCTL_SetCPCTFlag800Inverted, 0);
    Script_GetConversationDetails(CONV_DETAILS_MODE_DISABLE_PROMPT, 0);
    Script_LoadVoiceFile(1, "E1006");
    (void)((task_1C9 = 0) & 1);
    (void)((task_1CC = 0) & 1);
    task_1C5 = 0;
    (void)((task_1C6 = 0) & 1);
    Script_WeatherTaskUtil(AndNotInstructionFlags, 0x784);
    Script_SetupLightFromTable(0);
    Script_ProcessFogFromTable(0);
    task_1BC = Script_SetCameraSettings(CAM_UTILS_SET_DISTANCE_FROM_PLAYER, -1.0f);
    task_1C0 = Script_SetCameraSettings(CAM_UTILS_SET_HEIGHT_ABOVE_PLAYER, -1.0f);
    Script_cpctStateUtils(1, 1);
    local_F8 = 0;
    while (local_F8 < 6) {
      local_F4 = local_F8;
      if (local_F4 == 0) {
        (void)((task_1C7 = -1) & 4);
        (void)((task_1C8 = 0) & 4);
        Script_scenCreateTask(script_10F19, 2, *(&data_15F60 + local_F8), local_F8);
      } else {
        Script_scenCreateTask(script_106DB, 2, *(&data_15F60 + local_F8), local_F8);
      }
      Script_CharHandlers(*(&data_15F60 + local_F8), 2, 3, 16);
      Script_charaSetRefboardFlagTo(*(&data_15F60 + local_F8), 1);
      Script_SetCharaRefBoardPos(*(&data_15F60 + local_F8), &data_170BC);
      Script_SetShadowFlags(*(&data_15F60 + local_F8), SCRIPT_SHADOW_UPDATE_LIGHT_MASK, 4);
      Script_SetShadowFlags(*(&data_15F60 + local_F8), SCRIPT_SHADOW_UPDATE_LIGHT_MASK, 7);
      Script_SetShadowFlags(*(&data_15F60 + local_F8), SCRIPT_SHADOW_UPDATE_LIGHT_MASK, 8);
      Script_SetShadowFlags(*(&data_15F60 + local_F8), SCRIPT_SHADOW_UPDATE_LIGHT_MASK, 9);
      Script_SetShadowFlags(*(&data_15F60 + local_F8), SCRIPT_SHADOW_UPDATE_LIGHT_MASK, 11);
      local_F8 = local_F8 + 1;
    }
    task_1B4 = 0;
    Script_scenCreateTask(script_BC5A, 0);
    (void)((task_1CC = -1) & 2);
    Script_scenCreateTask(script_12432, 0);
    Script_scenCreateTask(script_12A16, 1, CHARA_AKIR);
    Script_scenCreateTask(script_13627, 0);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_MKSG_HELP_ROUTE_ACTIVE, 1);
    Script_GetCassetteFlags(51, 1, 0, 0);
    Script_SetOwnedItemMenuFlagIfOwned(1, OwnedItem_UIITEMFLAS, 1);
    Script_GetCurrentDrawMode(0, 1);
    Script_GetCurrentDrawMode(1, 1);
    SCNF_WaitFrames_0000(2);
    script_82BB();
    script_83B8(8);
  } else {
    script_ACDB(CHARA_AKIR);
    Script_scenCreateTask(script_F131, 0);
    Script_scenCreateTask(script_F851, 0);
    Script_scenCreateTask(script_FC24, 0);
    Script_scenCreateTask(script_FC49, 0);
    Script_motiLoad(2, SHEN_NAME('FREE'), "M_MOBJ.BIN");
    Script_CycleManagerUtils(CYCLE_CMD_SETUP, "scene/02/MFSY", (int32_t)"CYCLEMAN.BIN", 0x47534B4D);
    if (task_1B8) {
      if (Script_ryoSleepUtils(3, 0, 1)) {
        script_FC93(Script_ryoSleepUtils(3, 0, 1));
      }
      Script_ryoSleepUtils(1);
    }
    Script_ryoSleepUtils(2, 1);
    Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
    while (Script_filmCheckOrSetTransitionUtil(-1)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_shenClearFreeState(1);
    Script_ryoControl(RYO_RestoreControl);
  }
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 10, 0);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 70.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 20.0f);
  return;
}

static void script_102B5(int32_t arg0, int32_t arg1) {
  Vector3 local_F4;
  Vector3i local_E8;
  Script_ccowUtils(arg1, 2);
  Script_RequestFightAnimation(arg0, 1139, 0.0f, 8, 1);
  SCNF_WaitFrames_0000(1);
  local_F4.x = 0.08f;
  (&local_F4)->y = 0.01f;
  (&local_F4)->z = 0.04f;
  local_E8.x = 8829;
  (&local_E8)->y = 22027;
  (&local_E8)->z = 61895;
  Script_fixoAttachEx(arg1, arg0, 12, &local_F4, &local_E8);
  Script_charaDisplay(arg1, CDM_Show);
  return;
}

static void script_1032D(int32_t arg0, int32_t arg1) {
  Script_lightResetSlotToDefaults(arg0);
  Script_LightUtils(arg0, 0, 4);
  Script_lightSpotLightEnable(arg0, &data_170F4, &data_17100);
  Script_LightUtils(arg0, 2, &data_1710C);
  if (arg1 == 0) {
    Script_LightUtils(arg0, 4, 2.0f);
    Script_LightUtils(arg0, 5, 11.0f);
  } else {
    Script_LightUtils(arg0, 4, 1.0f);
    Script_LightUtils(arg0, 5, 7.0f);
  }
  Script_LightUtils(arg0, 6, data_1711C);
  Script_LightUtils(arg0, 10, 2275);
  Script_lightSetSlotEnabled(arg0, 1);
  return;
}

static void script_103BD(int32_t arg0, int32_t arg1) {
  Vector3 local_F4;
  Vector3 local_E8;
  Vector3i local_DC;
  Script_EVT_GetPosition(arg0, 12, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  Script_mtwkGetSetRotation(arg0, &local_DC, 0x42000000);
  local_E8.x = Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_DC)->y);
  (&local_E8)->y = 0.15f;
  (&local_E8)->z = Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_DC)->y);
  Script_LightUtils(arg1, 7, &local_F4);
  Script_LightUtils(arg1, 8, &local_E8);
  return;
}

static void script_10421(int32_t arg0, int32_t arg1, int32_t arg2) {
  Vector3 local_F4;
  Vector3 local_E8;
  int32_t unused_E4;
  int32_t local_DC[2];
  Script_EVT_GetPosition(arg0, 12, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  local_DC[1] = Script_FloatUtils(ScriptMathOp_Atan2ReverseVector, (&local_F4)->x - *(float *)arg2, (&local_F4)->z - *(float *)(arg2 + 8));
  local_DC[0] = Script_FloatUtils(ScriptMathOp_Atan2ReverseVector, Script_MathUtils(SCRIPT_MATH_VEC2_LENGTH, (&local_F4)->x - *(float *)arg2, (&local_F4)->z - *(float *)(arg2 + 8)), (&local_F4)->y - *(float *)(arg2 + 4));
  local_E8.x = (float)0 - Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, local_DC[1]);
  (&local_E8)->y = (float)0 - Script_MathUtils(SCRIPT_MATH_COS_FIXED16, *(int32_t *)local_DC);
  (&local_E8)->z = (float)0 - Script_MathUtils(SCRIPT_MATH_COS_FIXED16, local_DC[1]);
  Script_LightUtils(arg1, 7, &local_F4);
  Script_LightUtils(arg1, 8, &local_E8);
  return;
}

static void script_104DE(int32_t arg0, const Vector3 *arg1, const Vector3 *arg2) {
  Vector3 local_F4;
  int32_t unused_F0;
  int32_t local_E8[2];
  local_E8[1] = Script_FloatUtils(ScriptMathOp_Atan2ReverseVector, arg1->x - arg2->x, arg1->z - arg2->z);
  local_E8[0] = Script_FloatUtils(ScriptMathOp_Atan2ReverseVector, Script_MathUtils(SCRIPT_MATH_VEC2_LENGTH, arg1->x - arg2->x, arg1->z - arg2->z), arg1->y - arg2->y);
  local_F4.x = (float)0 - Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, local_E8[1]);
  (&local_F4)->y = (float)0 - Script_MathUtils(SCRIPT_MATH_COS_FIXED16, *(int32_t *)local_E8);
  (&local_F4)->z = (float)0 - Script_MathUtils(SCRIPT_MATH_COS_FIXED16, local_E8[1]);
  Script_LightUtils(arg0, 7, arg1);
  Script_LightUtils(arg0, 8, &local_F4);
  return;
}

static int32_t script_1058C(int32_t arg0, int32_t arg1) {
  int8_t local_FF;
  int8_t local_FE;
  int8_t local_FD;
  int32_t local_F8;
  local_F8 = arg0;
  if ((local_F8 == 3) | (local_F8 == 4)) {
    local_FF = 1;
    while (local_FF < 6) {
      Script_patrolUtils(*(&data_15F60 + local_FF), arg0, arg1);
      local_FF = local_FF + 1;
    }
  } else {
    switch (local_F8) {
      case 6:
        (void)((task_1CA = 0) & 2);
        local_FF = 1;
        while (local_FF < 6) {
          if ((Script_patrolUtils(*(&data_15F60 + local_FF), 2) == 1) | (Script_patrolUtils(*(&data_15F60 + local_FF), 2) == 5)) {
            (void)((task_1CA = -1) & 0x80);
          }
          local_FF = local_FF + 1;
        }
        break;
      case 5:
        local_FE = 127;
        local_FF = 1;
        while (local_FF < 6) {
          if ((Script_patrolUtils(*(&data_15F60 + local_FF), 2) == 1) | (Script_patrolUtils(*(&data_15F60 + local_FF), 2) == 5)) {
            local_FD = Script_patrolUtils(*(&data_15F60 + local_FF), arg0, arg1);
          } else {
            local_FD = 127;
          }
          if (local_FE > local_FD) {
            local_FE = local_FD;
          }
          local_FF = local_FF + 1;
        }
        (void)local_FE;
        break;
    }
  }
  end:;
}

static int32_t script_106DB(int32_t arg0, int32_t arg1) {
  Vector3 local_F4;
  Vector3 local_E8;
  int32_t unused_E4;
  GameDateTime local_DC;
  int8_t local_DB;
  int8_t local_DA;
  int8_t local_D9;
  int8_t local_D8;
  int8_t local_D7;
  int8_t local_D6;
  uint8_t unused_D4[2];
  int32_t local_D0;
  float local_C4[3];
  Vector3 local_B8;
  int32_t local_B4;
  float local_B0;
  int8_t local_AF;
  int8_t local_AE;
  uint8_t unused_AC[2];
  Vector3 local_A0;
  Vector3 local_94;
  int32_t local_88[3];
  local_DA = 0;
  Script_charaDisplay(arg0, CDM_Show);
  script_1032D(data_15F90[arg1], 0);
  Script_motmGetCharaFlags(arg0, 0, 0);
  for (;;) {
    if (task_1CC & 1) {
      Script_EVT_GetPosition(CHARA_AKIR, -1, &local_E8, 0);
      local_D0 = local_DA;
      switch (local_D0) {
        case 0:
          local_D0 = arg1;
          if (local_D0 == 1) {
            local_F4.x = -24.3008f;
            (&local_F4)->y = 0.0f;
            (&local_F4)->z = -75.1856f;
          } else if (local_D0 == 2) {
            local_F4.x = -69.154999f;
            (&local_F4)->y = 0.0f;
            (&local_F4)->z = -12.6726f;
          } else if (local_D0 == 3) {
            local_F4.x = -26.3948f;
            (&local_F4)->y = 0.0f;
            (&local_F4)->z = -31.552999f;
          } else if (local_D0 == 4) {
            local_F4.x = -48.600399f;
            (&local_F4)->y = 0.0f;
            (&local_F4)->z = -30.4732f;
          } else {
            local_F4.x = -24.206699f;
            (&local_F4)->y = 0.0f;
            (&local_F4)->z = -53.1744f;
          }
          Script_EVT_SetPosition(arg0, POS_XYZ, &local_F4);
          local_C4[1] = 2.2958874e-41f;
          Script_EVT_SetRotation(arg0, MATRIX_XFORM_Y, (const Vector3i *)local_C4, 0);
          local_DA = 1;
          script_102B5(arg0, *(&data_15F78 + arg1));
          break;
        case 1:
          local_D8 = 2;
          local_DA = 2;
          Script_ReturnTimeDate(&local_DC);
          (void)((local_D6 = -1) & 1);
          local_D0 = arg1;
          if (local_D0 == 1) {
            Script_patrolUtils(arg0, 0, &data_15C08, &data_15EFC, data_15BE8, 1, 2);
            if (((&local_DC)->hours == 20) | ((&local_DC)->hours == 23)) {
              (void)((local_D6 = 0) & 0x10);
            }
          } else if (local_D0 == 2) {
            Script_patrolUtils(arg0, 0, &data_15D24, &data_15F10, data_15BE8, 1, 1);
          } else if (local_D0 == 3) {
            Script_patrolUtils(arg0, 0, &data_15D24, &data_15F24, data_15BE8, 1, 7);
            if ((&local_DC)->hours == 23) {
              (void)((local_D6 = 0) & 0x80);
            }
          } else if (local_D0 == 4) {
            Script_patrolUtils(arg0, 0, &data_15E10, &data_15F38, data_15BE8, 1, 1);
          } else {
            Script_patrolUtils(arg0, 0, &data_15E10, &data_15F4C, data_15BE8, 1, 5);
            if ((&local_DC)->hours == 23) {
              (void)((local_D6 = 0) & 0x80);
            }
          }
          if (local_D6 == 0) {
            local_C4[0] = 1000.0f;
            local_C4[2] = 1000.0f;
            Script_EVT_SetPosition(arg0, 0x28000000, (const Vector3 *)local_C4);
            Script_charaDisplay(arg0, CDM_Hide);
            Script_charaDisplay(*(&data_15F78 + arg1), CDM_Hide);
            Script_lightSetSlotEnabled(data_15F90[arg1], 0);
            local_DA = 99;
          }
          local_DB = 0;
          break;
        case 2:
          if (Script_patrolUtils(arg0, 1) & -(task_1C4 == 1)) {
            task_1C4 = 2;
            Script_PlayCharaMotion(arg0, MN_AKI_AKI_RUN_EN, -1.0f, -1.0f, -1.0f, 1.0f, 0);
            task_1B0 = arg0;
          }
          local_D9 = Script_patrolUtils(arg0, 2);
          local_D0 = local_D9;
          switch (local_D0) {
            case 0:
              if (local_D8 == 5) {
                script_BBE2(arg1, 1);
              }
              break;
            case 1:
              if (local_D8 == 5) {
              } else {
                if (local_D8 != 1) {
                  script_BBE2(arg1, 3);
                  local_D7 = 150;
                  local_DB = 15;
                  goto L010A8D;
                }
              }
              L010A8D:;
              local_D7 = local_D7 - 1;
              if (local_D7 == 0) {
                script_BBE2(arg1, 0);
                local_D7 = 150;
              }
              break;
            case 2:
              break;
            case 3:
              break;
            case 4:
              break;
            case 5:
              break;
            case 6:
              break;
          }
          local_D8 = local_D9;
          if (task_1C9 & 0x20) {
            local_DA = 0;
          }
          break;
      }
      if (local_DB) {
        script_103BD(arg0, data_15F90[arg1]);
      } else {
        script_10421(arg0, data_15F90[arg1], (int32_t)&local_E8);
        local_DB = local_DB - 1;
      }
      local_B4 = Script_scenCharaGetMotAnimId(arg0);
      local_B0 = Script_GetCharaMotionRemainingFramesScaled(arg0);
      (void)((local_AE = 0) & 1);
      local_D0 = local_B4;
      switch (local_D0) {
        case 102:
          if ((!(local_B0 >= 0.1f) & !(local_B0 <= -0.1f)) | (!(local_B0 >= 18.1f) & !(local_B0 <= 17.9f))) {
            (void)((local_AE = -1) & 1);
          }
          break;
        case 234:
          if ((!(local_B0 >= 7.1f) & !(local_B0 <= 6.9f)) | (!(local_B0 >= 16.1f) & !(local_B0 <= 15.9f))) {
            (void)((local_AE = -1) & 1);
          }
          break;
      }
      if (local_AE & 0x40) {
        Script_EVT_GetPosition(arg0, -1, (Vector3 *)local_C4, 0);
        Script_EVT_GetPosition(CHARA_AKIR, -1, &local_B8, 0);
        if (!(Script_MathUtils(SCRIPT_MATH_VEC2_LENGTH, (&local_B8)->x - *local_C4, (&local_B8)->z - local_C4[2]) <= 40.0f)) {
          (void)((local_AE = 0) & 1);
        }
      }
      if (local_AE & 0x40) {
        Script_GetAuthorState_And_SetCamPos(&local_A0, &local_94);
        local_88[1] = Script_FloatUtils(ScriptMathOp_Atan2ReverseVector, (&local_94)->x - (&local_A0)->x, (&local_94)->z - (&local_A0)->z);
        local_88[0] = Script_FloatUtils(ScriptMathOp_Atan2ReverseVector, *local_C4 - (&local_B8)->x, local_C4[2] - (&local_B8)->z);
        if (local_88[1] > *(int32_t *)local_88) {
          local_88[2] = local_88[1] - *(int32_t *)local_88;
        } else {
          local_88[2] = *(int32_t *)local_88 - local_88[1];
        }
        local_AF = (arg1 - 1) * 6 + Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f));
        if (local_88[2] > 0x4000) {
          local_AF = local_AF + 3;
        }
        Script_Execute3DSoundCommand_Wrapper(*(&data_17124 + local_AF), 5, (const Vector3 *)local_C4, 10.0f, 40.0f, 0);
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_10D1C(int32_t arg0, int32_t arg1) {
  int32_t unused_E8[6];
  int8_t local_E7;
  uint8_t unused_E4[3];
  local_E7 = 0;
  for (;;) {
    if (Script_IsPlayerAllowed(0) & -(task_1CA == 0)) {
      if (Script_scenUtils(SCENARIO_CMD_GET_STEP_LUT, 23)) {
        if (Script_GetControllerInput(1) == 4) {
          script_BD87();
        }
      }
    }
    if (task_1C7 & 8) {
      if (task_1C8 == 0) {
        Script_GetCameraInputDetails(data_15F90[arg1]);
      }
    }
    if (local_E7) {
      local_E7 = local_E7 - 1;
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_10D92(int32_t arg0) {
  int32_t unused_FC;
  GameDateTime local_F4;
  int32_t local_F0;
  Script_ReturnTimeDate(&local_F4);
  if ((&local_F4)->hours >= 19) {
    Script_IncrementDateTimeByNum(1);
    Script_ReturnTimeDate(&local_F4);
  }
  if (Script_scenUtils(SCENARIO_CMD_GET_STEP_LUT, 23) == 4) {
    (&local_F4)->hours = 23;
  } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_AFTER_8PM_GUARD_SHIFT)) {
    (&local_F4)->hours = 20;
  } else {
    (&local_F4)->hours = 19;
  }
  (&local_F4)->minutes = 0;
  (&local_F4)->seconds = 0;
  Script_SetNewDateTime(&local_F4);
  local_F0 = Script_shenGetStats(STATS_CASH);
  Script_SetStats(STATS_CASH, local_F0 + 180);
  Script_PlayerStatsUtils(PS_SetStatU8_1, Script_PlayerStatsUtils(PS_GetStatU8_1) + 1);
  Script_PlayerStatsUtils(PS_SetStatU8_0, Script_PlayerStatsUtils(PS_GetStatU8_0) + 1);
  if (((&local_F4)->month == 4) & ((&local_F4)->day == 15)) {
    (&local_F4)->hours = 7;
    (&local_F4)->minutes = 59;
    (&local_F4)->seconds = 0;
    Script_SetNewDateTime(&local_F4);
    Script_shenRequestGameModuleTransition(2, SHEN_NAME('JOMO'), 4);
    SCNF_WaitFrames_0000(1);
  } else {
    if ((&local_F4)->dayOfWeek == 1) {
      script_AF73();
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 189, 1);
      goto L010EB3;
    }
  }
  L010EB3:;
  Script_WeatherTaskUtil(FreeTaskData);
  Script_WeatherTaskUtil(InitTaskData);
  Script_WeatherTaskUtil(AndNotInstructionFlags, 0x784);
  Script_lightSetGlobalSceneLighting(&data_171B4);
  Script_ProcessFogFromTable(0);
  Script_cdttLoadScrUtils(0, 0);
  SCNF_WaitFrames_0000(120);
  Script_cdttLoadScrUtils(1, 0);
  script_D497();
  task_1C4 = 0;
  (void)((task_1C9 = -1) & 0x10);
  Script_ccowUtils(arg0, 1);
  script_11A0D();
  return;
}

__attribute__((scn_string_bindings(0, data_171EC, 1, data_171F3)))
static int32_t script_10F19(int32_t arg0, int32_t arg1) {
  Vector3 local_F4;
  int32_t local_F0;
  int32_t unused_E8[2];
  int8_t local_E7;
  int8_t local_E6;
  uint8_t unused_E4[2];
  int32_t local_E0;
  Vector3i local_D4;
  script_ACDB(arg0);
  Script_scenCreateTask(script_10D1C, 2, arg0, arg1);
  Script_lightSetGlobalSceneLighting(&data_171C4);
  SCNF_WaitFrames_0000(1);
  Script_SetShouldSleepFlag(0);
  Script_SetCharaFlags(arg0, 0);
  Script_ryoControl(PCCTL_DisablePlayerControl);
  task_1C4 = 0;
  (void)((task_1CA = 0) & 0x10);
  (void)((task_1CB = 0) & 0x10);
  (void)((local_E6 = -1) & 0x10);
  if (Script_GetCurrentEntryPoint() == 11) {
    (void)((local_E6 = 0) & 0x10);
  }
  if (Script_scenUtils(SCENARIO_CMD_GET_STEP_LUT, 20) != 0) {
    (void)((local_E6 = 0) & 0x10);
  }
  Script_CharHandlers(arg0, 1, 8, 16);
  Script_SetStats(STATS_PLAYER_CONTEXT_ACTION_MODE, 3);
  Script_fixoAttachEx(SHEN_NAME('LIGH'), CHARA_AKIR, 12, &data_171D4, &data_171E0);
  Script_charaDisplay(*(&data_15F78 + arg1), CDM_Show);
  SCNF_WaitFrames_0000(2);
  script_13761(arg0);
  Script_scenCreateTask(script_11305, 1, arg0);
  for (;;) {
    local_E0 = task_1C4;
    switch (local_E0) {
      case 0:
        Script_ryoControl(PCCTL_DisablePlayerControl);
        Script_PlayCharaMotion(arg0, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        local_F4.x = -16.5f;
        (&local_F4)->y = 0.0f;
        (&local_F4)->z = -17.0f;
        Script_EVT_SetPosition(arg0, POS_XYZ, &local_F4);
        (&local_D4)->y = 0;
        Script_EVT_SetRotation(arg0, MATRIX_XFORM_Y, &local_D4, 0);
        Script_InitializeOsag(arg0);
        script_1032D(data_15F90[arg1], 1);
        Script_charaDisplay(*(&data_15F78 + arg1), CDM_Show);
        script_F62C();
        SCNF_WaitFrames_0000(1);
        Script_convSetCamInactiveFlag(1);
        Script_SetGameStateTo(AUTH_None);
        script_123E9(0);
        Script_SetCameraFlags(-1);
        SCNF_WaitFrames_0000(1);
        task_1C4 = 1;
        (void)((task_1C9 = 0) & 0x10);
        (void)((task_1CD = 0) & 0x10);
        script_13761(arg0);
        Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
        (void)((task_1CA = 0) & 0x10);
        (void)((task_1CB = 0) & 0x10);
        script_123E9(0);
        if (local_E6 == -1) {
          script_114C4(arg0, arg1);
          (void)((local_E6 = 0) & 0x10);
          SCNF_WaitFrames_0000(1);
        } else {
          Script_shenClearFreeState(1);
          script_1148A(task_1CA);
          script_11872(arg0);
        }
        Script_charaSetRefboardFlagTo(arg0, 1);
        script_1371D(arg0);
        script_83B8(8);
        SCNF_WaitFrames_0000(1);
        local_F0 = 0;
        break;
      case 1:
        script_1058C(6, 0);
        if ((task_1CA == 0) & (task_1C5 == 1)) {
          Script_SetCameraFlags(0);
          script_11A65(arg0, arg1);
          Script_SetCameraFlags(-1);
        }
        break;
      case 2:
        while (task_1C6 & 4) {
          SCNF_WaitFrames_0000(1);
        }
        Script_SetCameraFlags(0);
        Script_shenClearFreeState(0);
        script_11BEB(arg0);
        local_E7 = Script_scenUtils(SCENARIO_CMD_GET_STEP_LUT, 20);
        local_E7 = local_E7 + 1;
        Script_scenUtils(SCENARIO_CMD_SET_STEP_LUT, 20, local_E7);
        Script_executeAicaCommand(2720, 2, 20);
        SCNF_WaitFrames_0000(20);
        Script_LoadSND(0, 2, "SELECT", 0);
        Script_executeAicaCommand(16296, 0, 0);
        Script_executeAicaCommand(1184, 2, 100);
        script_12037(arg0);
        script_10D92(arg0);
        Script_ccowUtils(task_1B0, 1);
        Script_LoadSND(0, 2, "ykm005.snd", 0);
        Script_shenClearFreeState(1);
        break;
    }
    if (task_1CA != task_1CB) {
      if (task_1CA & 0x40) {
        script_123E9(1);
        script_8381(8);
      } else {
        script_123E9(0);
        script_83B8(8);
      }
      script_1148A(task_1CA);
      (void)((task_1CB = task_1CA) & 2);
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_11305(int32_t arg0) {
  int32_t unused_FC;
  GameDateTime local_F4;
  int32_t local_F0;
  int8_t local_EF;
  uint8_t unused_EC[3];
  for (;;) {
    Script_ReturnTimeDate(&local_F4);
    if ((&local_F4)->hours == 4) {
      (void)((task_1CD = -1) & 0x10);
      if (Script_ryoControl(PCCTL_QueryShouldRun) == 0) {
        while (Script_ryoControl(PCCTL_QueryShouldRun) == 0) {
          SCNF_WaitFrames_0000(1);
        }
      }
      L01134B:;
      if (task_1CD == 0) {
      } else {
        Script_ryoControl(PCCTL_DisablePlayerControl);
        Script_PlayCharaMotion(arg0, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        (void)((task_1CC = 0) & 0x10);
        Script_SetGameStateTo(AUTH_None);
        Script_srchUtils(SRCH_SCRIPT_Reset);
        Script_executeAicaCommand(2720, 2, 30);
        if (Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f)) < 1) {
          local_F0 = Script_ShowUIMessageFromID("E1006A028");
        } else {
          local_F0 = Script_ShowUIMessageFromID("E1006A029");
        }
        Script_filmCheckOrSetTransitionUtil(60, 0, 0, 0, 0, 255, 0, 0, 0);
        while (Script_filmCheckOrSetTransitionUtil(-1)) {
          SCNF_WaitFrames_0000(1);
        }
        (void)((task_1CD = 0) & 1);
        Script_shenClearFreeState(0);
        Script_ccowUtils(CHARA_AKIR, 2);
        script_13761(arg0);
        Script_charaDisplay(SHEN_NAME('LIGH'), CDM_Hide);
        local_EF = Script_scenUtils(SCENARIO_CMD_GET_STEP_LUT, 20);
        local_EF = local_EF + 1;
        Script_scenUtils(SCENARIO_CMD_SET_STEP_LUT, 20, local_EF);
        while (Script_convGetIsActive((const void *)local_F0)) {
          SCNF_WaitFrames_0000(1);
        }
        script_10D92(arg0);
        Script_shenClearFreeState(1);
        goto L01147E;
        goto L01134B;
      }
    }
    L01147E:;
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_1148A(int32_t arg0) {
  if (arg0) {
    Script_executeAicaCommand(81832, 0, 0);
    Script_executeAicaCommand(1184, 2, 127);
  } else {
    Script_executeAicaCommand(16296, 0, 0);
    Script_executeAicaCommand(1184, 2, 127);
  }
  return;
}

static void script_114C4(int32_t arg0, int32_t arg1) {
  int32_t unused_E8[6];
  Vector3 local_DC;
  int32_t unused_D0[3];
  int32_t local_CC;
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_FaceEx(CHARA_AKIR, 1, 0, 2);
  Script_ResetHandMotion(CHARA_AKIR, -1, 0, 0);
  Script_ResetHandMotion(CHARA_AKIR, -1, 1, 0);
  Script_shenClearFreeState(0);
  script_119B5();
  Script_PlayCharaMotion(arg0, MN_AKI_AKI_WALK_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_LineWalkUtils(LNWK_SCRIPT_BEGIN_PATH_WALK, arg0, &data_17214, (int32_t)&data_15BF8, 0);
  for (;;) {
    if (Script_LineWalkUtils(LNWK_SCRIPT_SET_MOVE_SCALE_GET_CMD, arg0, 0.06f) == -1) {
      SCNF_WaitFrames_0000(1);
      Script_PlayCharaMotion(arg0, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_SetGameStateTo(AUTH_EventCam);
  Script_seqEnterEvtCam(553, 0, AUTH_None);
  Script_SetCharaLookAtPosition(arg0, 6, &data_17228, 0);
  Script_ShowUIMessageFromID("E1006A024");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_SetCharaLookAtPosition(arg0, -6, 0, 0);
  Script_seqEnterEvtCam(550, 0, AUTH_None);
  SCNF_CHAR_SCNFApplyFacingTowardPoint(arg0, &data_17228, data_15BD8, 0.0f, 0, 0);
  SCNF_WaitFrames_0000(1);
  script_1371D(arg0);
  Script_ShowUIMessageFromID("E1006A006");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  script_11A0D();
  task_1C5 = 0;
  script_13761(arg0);
  Script_seqEnterEvtCam(551, 0, AUTH_None);
  local_DC.x = -19.49f;
  (&local_DC)->y = 0.0f;
  (&local_DC)->z = -22.93f;
  SCNF_CHAR_SCNFApplyFacingTowardPoint(arg0, &local_DC, data_15BD8, 0.0f, 0, 0);
  Script_PlayCharaMotion(arg0, MN_AKI_AKI_WALK_SP2_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  local_CC = 0;
  for (;;) {
    if (local_CC == 85) {
      break;
    }
    local_CC = local_CC + 1;
    SCNF_WaitFrames_0000(1);
  }
  Script_seqEnterEvtCam(554, 0, AUTH_None);
  Script_PlayCharaMotion(arg0, 4418, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  for (;;) {
    if (Script_IsCharaMotionFinished(arg0)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  SCNF_WaitFrames_0000(1);
  Script_SetGameStateTo(AUTH_EventCam);
  Script_seqEnterEvtCam(552, 0, AUTH_None);
  Script_EVT_GetPosition(arg0, -1, &local_DC, 0);
  local_DC.x = (&local_DC)->x + 1.0f;
  Script_SetCharaMotionPlaybackRate(arg0, 2.5f);
  SCNF_CHAR_SCNFApplyFacingTowardPoint(arg0, &local_DC, data_15BD8, 0.0f, 0, 0);
  Script_EVT_GetPosition(arg0, -1, &local_DC, 0);
  (&local_DC)->z = (&local_DC)->z - 1.0f;
  Script_SetCharaLookAtPosition(arg0, 6, &local_DC, 0);
  local_DC.x = (&local_DC)->x - 1.0f;
  Script_FaceLookAtTargetUtils(arg0, 2, &local_DC, 5);
  Script_ShowUIMessageFromID("E1006A014");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_shenClearFreeState(1);
  for (;;) {
    if ((Script_GetControllerInput(1) & -2049) | (task_1CD == -1)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  local_DC.x = (&local_DC)->x - 0.2f;
  Script_SetCharaMotionPlaybackRate(arg0, 1.5f);
  SCNF_CHAR_SCNFApplyFacingTowardPoint(arg0, &local_DC, data_15BD8, 0.0f, 0, 0);
  Script_FaceLookAtTargetUtils(arg0, 0);
  Script_SetCharaLookAtPosition(arg0, -6, 0, 0);
  Script_convSetCamInactiveFlag(0);
  Script_SetGameStateTo(AUTH_None);
  SCNF_WaitFrames_0000(1);
  Script_ryoControl(RYO_RestoreControl);
  Script_FaceEx(CHARA_AKIR, 0, 0, 2);
  Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
  Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
  Script_shenClearFreeState(1);
  return;
}

static void script_11872(int32_t arg0) {
  {
    Vector3 local_F4;
    local_F4.x = -19.34f;
    (&local_F4)->y = 0.0f;
    (&local_F4)->z = -22.16f;
    Script_EVT_SetPosition(arg0, POS_XYZ, &local_F4);
    SCNF_WaitFrames_0000(1);
    Script_SetGameStateTo(AUTH_EventCam);
    Script_seqSetPlayRoot(&local_F4, &data_17254);
  }
  Script_seqEnterEvtCam(600, 0, AUTH_None);
  {
    Vector3 local_F4;
    Script_EVT_GetPosition(arg0, -1, &local_F4, 0);
    local_F4.x = (&local_F4)->x + 1.0f;
    Script_SetCharaMotionPlaybackRate(arg0, 2.5f);
    Script_EVT_SetFacingTowardPoint(arg0, 0x38000000, &local_F4, 0);
  }
  {
    Vector3 local_F4;
    Script_EVT_GetPosition(arg0, -1, &local_F4, 0);
    (&local_F4)->z = (&local_F4)->z - 1.0f;
    Script_SetCharaLookAtPosition(arg0, 6, &local_F4, 0);
    local_F4.x = (&local_F4)->x - 1.0f;
    Script_FaceLookAtTargetUtils(arg0, 2, &local_F4, 5);
    for (;;) {
      if ((Script_GetControllerInput(1) & -2049) | (task_1CD == -1)) {
        break;
      }
      SCNF_WaitFrames_0000(1);
    }
    local_F4.x = (&local_F4)->x - 0.2f;
    Script_SetCharaMotionPlaybackRate(arg0, 1.5f);
    SCNF_CHAR_SCNFApplyFacingTowardPoint(arg0, &local_F4, data_15BD8, 0.0f, 0, 0);
  }
  Script_FaceLookAtTargetUtils(arg0, 0);
  Script_SetCharaLookAtPosition(arg0, -20, 0, 0);
  Script_convSetCamInactiveFlag(0);
  Script_SetGameStateTo(AUTH_None);
  Script_ryoControl(RYO_RestoreControl);
  return;
}

static void script_119B5(void) {
  int8_t local_FF;
  (void)((task_1CC = 0) & 0x10);
  local_FF = 5;
  while (local_FF) {
    Script_charaDisplay(*(&data_15F60 + local_FF), CDM_Hide);
    Script_charaDisplay(*(&data_15F78 + local_FF), CDM_Hide);
    Script_lightSetSlotEnabled(data_15F90[local_FF], 0);
    local_FF = local_FF - 1;
  }
  return;
}

static void script_11A0D(void) {
  int8_t local_FF;
  (void)((task_1CC = -1) & 2);
  local_FF = 5;
  while (local_FF) {
    Script_charaDisplay(*(&data_15F60 + local_FF), CDM_Show);
    Script_charaDisplay(*(&data_15F78 + local_FF), CDM_Show);
    Script_lightSetSlotEnabled(data_15F90[local_FF], 1);
    local_FF = local_FF - 1;
  }
  return;
}

static void script_11A65(int32_t arg0, int32_t arg1) {
  Vector3i local_F4;
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_FaceEx(CHARA_AKIR, 1, 0, 2);
  Script_ResetHandMotion(CHARA_AKIR, -1, 0, 0);
  Script_ResetHandMotion(CHARA_AKIR, -1, 1, 0);
  Script_shenClearFreeState(0);
  script_119B5();
  Script_SetGameStateTo(AUTH_EventCam);
  Script_seqEnterEvtCam(120, 0, AUTH_None);
  Script_motmToggleFlags(arg0, 2, 0x40);
  Script_EVT_SetPosition(arg0, POS_XYZ, &data_17278);
  (&local_F4)->y = 20024;
  Script_EVT_SetRotation(arg0, MATRIX_XFORM_Y, &local_F4, 0);
  Script_PlayCharaMotion(arg0, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_SetCharaLookAtPosition(arg0, 6, &data_1726C, 0);
  script_10421(arg0, data_15F90[arg1], (int32_t)&data_17260);
  (void)((task_1C7 = 0) & 1);
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  SCNF_WaitFrames_0000(30);
  Script_ShowUIMessageFromID("E1006A010");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  (void)((task_1C7 = -1) & 1);
  Script_convSetCamInactiveFlag(0);
  Script_SetGameStateTo(AUTH_None);
  Script_SetCharaLookAtPosition(arg0, -6, 0, 0);
  task_1C5 = 2;
  script_11A0D();
  Script_motmToggleFlags(arg0, 1, 0x40);
  Script_ryoControl(RYO_RestoreControl);
  Script_FaceEx(CHARA_AKIR, 0, 0, 2);
  Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
  Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
  Script_shenClearFreeState(1);
  return;
}

static void script_11BEB(int32_t arg0) {
  Vector3 local_F4;
  Vector3 local_E8;
  Vector3 local_DC;
  Vector3i local_D0;
  int32_t local_CC;
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_FaceEx(CHARA_AKIR, 1, 0, 2);
  Script_FACE(CHARA_AKIR, 4, 8, 0);
  script_13761(arg0);
  Script_SetRyoMotionDetails(task_1B0);
  (void)((task_1CC = 0) & 1);
  Script_PlayCharaMotion(CHARA_KEBA, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_PlayCharaMotion(CHARA_KEBB, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_PlayCharaMotion(CHARA_KEBC, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_PlayCharaMotion(CHARA_KEBD, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_PlayCharaMotion(CHARA_KEBE, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_ccowUtils(arg0, 2);
  Script_ccowUtils(task_1B0, 2);
  Script_EVT_GetPosition(arg0, -1, &local_F4, 0);
  Script_EVT_GetPosition(task_1B0, -1, &local_E8, 0);
  (&local_D0)->y = Script_FloatUtils(ScriptMathOp_Atan2ReverseVector, (&local_F4)->x - (&local_E8)->x, (&local_F4)->z - (&local_E8)->z);
  {
    Vector3i local_C0;
    (&local_C0)->y = (&local_D0)->y;
    Script_EVT_SetRotation(task_1B0, MATRIX_XFORM_Y, &local_C0, 0);
  }
  (&local_D0)->y = (&local_D0)->y - 0x8000;
  {
    Vector3i local_C0;
    (&local_C0)->y = (&local_D0)->y;
    Script_EVT_SetRotation(arg0, MATRIX_XFORM_Y, &local_C0, 0);
  }
  local_E8.x = -0.3f;
  (&local_E8)->y = 0.0f;
  (&local_E8)->z = -1.1f;
  Script_MoveObjectToObject(task_1B0, arg0);
  Script_EVT_SetPosition(task_1B0, POS_XYZ, &local_E8);
  if (0) {
    Script_charaDisplay(SHEN_NAME('DUMY'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('DUMY'), CDM_Show);
      goto L011DCC;
    }
  }
  L011DCC:;
  Script_scenToggleVisibleFor(SHEN_NAME('DUMY'), 1);
  Script_EVT_GetPosition(arg0, -1, &local_DC, 0);
  Script_mtwkGetSetRotation(arg0, &local_D0, 0);
  Script_EVT_SetPosition(SHEN_NAME('DUMY'), POS_XYZ, &local_DC);
  Script_EVT_SetRotation(SHEN_NAME('DUMY'), 0x38000000, &local_D0, 0);
  Script_charaToggleMotEnabled(arg0, 0);
  Script_charaToggleMotEnabled(task_1B0, 1);
  Script_motmToggleFlags(arg0, 2, 0x40);
  Script_PlayCharaMotion(arg0, 4386, -1.0f, -1.0f, 20.0f, 1.0f, 0);
  Script_PlayCharaMotion(task_1B0, 4400, -1.0f, -1.0f, 20.0f, 1.0f, 0);
  Script_SetGameStateTo(AUTH_EventCam);
  if (Script_EventCamReader(133, SHEN_NAME('DUMY'), SHEN_NAME('DUMY'), SHEN_NAME('DUMY')) == 0) {
    Script_seqEnterEvtCam(133, SHEN_NAME('DUMY'), SHEN_NAME('DUMY'));
  } else if (Script_EventCamReader(132, SHEN_NAME('DUMY'), SHEN_NAME('DUMY'), SHEN_NAME('DUMY')) == 0) {
    Script_seqEnterEvtCam(132, SHEN_NAME('DUMY'), SHEN_NAME('DUMY'));
  } else if (Script_EventCamReader(131, SHEN_NAME('DUMY'), SHEN_NAME('DUMY'), SHEN_NAME('DUMY')) == 0) {
    Script_seqEnterEvtCam(131, SHEN_NAME('DUMY'), SHEN_NAME('DUMY'));
  } else if (Script_EventCamReader(130, SHEN_NAME('DUMY'), SHEN_NAME('DUMY'), SHEN_NAME('DUMY')) == 0) {
    Script_seqEnterEvtCam(130, SHEN_NAME('DUMY'), SHEN_NAME('DUMY'));
  } else {
    Script_seqEnterEvtCam(134, SHEN_NAME('DUMY'), SHEN_NAME('DUMY'));
  }
  local_CC = 0;
  for (;;) {
    int32_t local_C8;
    local_CC = local_CC + 1;
    local_C8 = local_CC;
    switch (local_C8) {
      case 5:
        script_BBE2(script_BB67(task_1B0), 4);
        break;
      case 95:
        Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
        break;
      case 125:
        goto L011FBF;
        break;
    }
    SCNF_WaitFrames_0000(1);
  }
  L011FBF:;
  SCNF_WaitFrames_0000(10);
  Script_MoveObjectToObject(task_1B0, -1);
  Script_motmToggleFlags(arg0, 1, 0x40);
  Script_charaToggleMotEnabled(task_1B0, 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('DUMY'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('DUMY'), CDM_Show);
      goto L01200B;
    }
  }
  L01200B:;
  Script_scenToggleVisibleFor(SHEN_NAME('DUMY'), 0);
  Script_FACE(CHARA_AKIR, 0, 0, 0);
  Script_FaceEx(CHARA_AKIR, 0, 0, 2);
  return;
}

static void script_12037(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  Vector3 local_EC;
  local_EC.x = 0.0f;
  (&local_EC)->y = 0.0f;
  local_F8 = 0;
  Script_FaceEx(CHARA_AKIR, 1, 0, 2);
  Script_FACE(CHARA_AKIR, 4, 8, 0);
  script_119B5();
  Script_charaDisplay(task_1B0, CDM_Show);
  Script_charaDisplay(SHEN_NAME('LIGH'), CDM_Hide);
  Script_ryoControl(PCCTL_UpdateRyoNavYaw, arg0);
  for (;;) {
    if (Script_IsCharaMotionFinished(arg0)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  (&local_EC)->z = 1.0f;
  Script_EVT_SetPosition(SHEN_NAME('GATL'), -0x48000000, &local_EC);
  (&local_EC)->z = -0.7f;
  Script_EVT_SetPosition(SHEN_NAME('GATR'), -0x48000000, &local_EC);
  Script_EVT_SetPosition(arg0, POS_XYZ, &data_17290);
  {
    Vector3i local_E0;
    (&local_E0)->y = -0x4000;
    Script_EVT_SetRotation(arg0, MATRIX_XFORM_Y, &local_E0, 0);
  }
  Script_EVT_SetPosition(task_1B0, POS_XYZ, &data_1729C);
  {
    Vector3i local_E0;
    (&local_E0)->y = -18470;
    Script_EVT_SetRotation(task_1B0, MATRIX_XFORM_Y, &local_E0, 0);
  }
  Script_ccowUtils(task_1B0, 2);
  Script_SetRyoMotionDetails(task_1B0);
  Script_charaToggleMotEnabled(arg0, 1);
  Script_charaToggleMotEnabled(task_1B0, 1);
  Script_PlayCharaMotion(arg0, 4372, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_PlayCharaMotion(task_1B0, 4399, -1.0f, 246.0f, -1.0f, 1.0f, 0);
  SCNF_WaitFrames_0000(1);
  local_FC = 0;
  for (;;) {
    int32_t local_E8;
    local_E8 = local_FC;
    switch (local_E8) {
      case 0:
        Script_filmCheckOrSetTransitionUtil(60, 255, 0, 0, 0, 0, 0, 0, 0);
        Script_seqEnterEvtCam(140, 0, AUTH_None);
        break;
      case 50:
        script_BBE2(script_BB67(task_1B0), 2);
        break;
      case 109:
        Script_seqEnterEvtCam(141, 0, AUTH_None);
        Script_executeAicaCommand(0x2A04A9, 0, 0);
        Script_FACE(CHARA_AKIR, 2, 0, 100);
        break;
      case 155:
        (&local_EC)->z = -1.0f;
        Script_EVT_SetPosition(SHEN_NAME('GATL'), -0x48000000, &local_EC);
        (&local_EC)->z = 0.7f;
        Script_EVT_SetPosition(SHEN_NAME('GATR'), -0x48000000, &local_EC);
        Script_seqEnterEvtCam(142, 0, AUTH_None);
        break;
      case 270:
        Script_seqEnterEvtCam(143, 0, AUTH_None);
        break;
      case 350:
        Script_ShowUIMessageFromID("E1006A026");
        break;
      case 410:
        Script_filmCheckOrSetTransitionUtil(60, 0, 0, 0, 0, 255, 0, 0, 0);
        break;
      case 470:
        goto L012313;
        break;
    }
    if (Script_IsCharaMotionFinished(task_1B0) & -(local_F8 == 0)) {
      local_F8 = Script_scenCreateTask(script_12362, 1, task_1B0);
    }
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  L012313:;
  Script_RequestFightAnimation(task_1B0, 1139, 0.0f, 8, 1);
  Script_ccowUtils(task_1B0, 1);
  Script_srchControl((void *)local_F8);
  script_11A0D();
  (void)((task_1CC = 0) & 2);
  Script_FACE(CHARA_AKIR, 0, 0, 0);
  Script_FaceEx(CHARA_AKIR, 0, 0, 2);
  return;
}

static int32_t script_12362(int32_t arg0) {
  Vector3i local_F4;
  Script_charaToggleMotEnabled(arg0, 0);
  (&local_F4)->y = 0x4000;
  Script_EVT_SetRotation(arg0, MATRIX_XFORM_Y, &local_F4, 0);
  Script_PlayCharaMotion(arg0, MN_AKI_AKI_WALK_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_LineWalkUtils(LNWK_SCRIPT_BEGIN_PATH_WALK, arg0, &data_172B4, (int32_t)&data_15BF8, 0);
  SCNF_WaitFrames_0000(1);
  for (;;) {
    if (Script_LineWalkUtils(LNWK_SCRIPT_SET_MOVE_SCALE_GET_CMD, arg0, 0.06f) == -1) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_123E9(int32_t arg0) {
  int32_t local_FC;
  local_FC = arg0;
  switch (local_FC) {
    case 0:
      Script_SetCameraSettings(CAM_UTILS_SET_DISTANCE_FROM_PLAYER, task_1BC);
      Script_SetCameraSettings(CAM_UTILS_SET_HEIGHT_ABOVE_PLAYER, task_1C0);
      break;
    case 1:
      Script_SetCameraSettings(CAM_UTILS_SET_DISTANCE_FROM_PLAYER, 6.0f);
      Script_SetCameraSettings(CAM_UTILS_SET_HEIGHT_ABOVE_PLAYER, 2.7f);
      break;
  }
  return;
}

static int32_t script_12432(void) {
  Vector3 local_F4;
  Vector3 local_E8;
  Vector3 local_DC;
  Vector3 local_D0;
  int32_t unused_CC;
  float local_C4[2];
  Vector3i local_B8;
  int32_t unused_B4;
  int32_t local_AC[2];
  int32_t local_A8;
  int32_t local_A4;
  int8_t local_A3;
  int8_t local_A2;
  int8_t local_A1;
  int8_t local_A0;
  Script_srchUtils(SRCH_SCRIPT_InitEntryTableFromTaskOffset, &data_172D8);
  Script_SetMaxCharaSearchExtent(0, 3.0f);
  (void)((local_A0 = 0) & 1);
  for (;;) {
    if (task_1CA == 0) {
      local_A8 = Script_srchUtils(SRCH_SCRIPT_UpdateAndPollSelection);
      local_A4 = Script_srchUtils(SRCH_SCRIPT_GetCurrentTargetEntryIndex);
      if (local_A4 >= 0) {
        int32_t local_9C;
        script_119B5();
        (void)((task_1C8 = -1) & 8);
        local_DC.x = local_D0.x = *(float *)((const char *)&data_174A4 + local_A4 * 12);
        (&local_DC)->y = (&local_D0)->y = *(float *)((const char *)&data_174A4 + (local_A4 * 12 + 4));
        (&local_DC)->z = (&local_D0)->z = *(float *)((const char *)&data_174A4 + (local_A4 * 12 + 8));
        local_9C = data_17530[local_A4];
        switch (local_9C) {
          case 0:
            (&local_DC)->z = (&local_DC)->z + 2.0f;
            break;
          case 1:
            local_DC.x = (&local_DC)->x + 2.0f;
            break;
          case 2:
            (&local_DC)->z = (&local_DC)->z - 2.0f;
            break;
          case 3:
            local_DC.x = (&local_DC)->x - 2.0f;
            break;
        }
        script_104DE(data_15F90[0], &local_DC, &local_D0);
        if ((local_A8 >= 0) & (local_A1 == 0)) {
          if (task_1C5 != 2) {
            Script_ShowUIMessageFromID((const char *)Script_ScenDataUtil(&data_1747C, 8, local_A8));
            while (Script_convGetIsActive(0)) {
              SCNF_WaitFrames_0000(1);
            }
            (void)((local_A1 = -1) & 0x10);
          }
          script_129CC(local_A8);
        }
      }
    }
    if (task_1C8 == -1) {
      if (Script_GetControllerInput(1) & -2049) {
        Script_SetGameStateTo(AUTH_None);
        Script_srchUtils(SRCH_SCRIPT_Reset);
        script_11A0D();
        (void)((local_A1 = 0) & 0x10);
        (void)((task_1C8 = 0) & 0x10);
      }
    }
    if ((task_1C5 == 0) & (local_A1 != -1)) {
      Script_EVT_GetPosition(CHARA_AKIR, -1, &local_F4, 0);
      Script_mtwkGetSetRotation(CHARA_AKIR, &local_B8, 0x2000000);
      Script_GetAuthorState_And_SetCamPos(&local_DC, &local_D0);
      local_AC[1] = Script_FloatUtils(ScriptMathOp_Atan2ReverseVector, (&local_D0)->x - (&local_DC)->x, (&local_D0)->z - (&local_DC)->z);
      local_A3 = 0;
      while (local_A3 < 10) {
        int32_t local_9C;
        local_A2 = 0;
        local_E8.x = *(float *)((const char *)&data_174A4 + local_A3 * 12);
        (&local_E8)->y = 0.0f;
        (&local_E8)->z = *(float *)((const char *)&data_174A4 + (local_A3 * 12 + 8));
        if (local_A3 == 7) {
          if (!(Script_MathUtils(SCRIPT_MATH_VEC2_LENGTH, (&local_F4)->x - (&local_E8)->x, (&local_F4)->z - (&local_E8)->z) >= 5.0f)) {
            local_A2 = local_A2 + 3;
          }
        } else {
          if (!(Script_MathUtils(SCRIPT_MATH_VEC2_LENGTH, (&local_F4)->x - (&local_E8)->x, (&local_F4)->z - (&local_E8)->z) >= 6.0f)) {
            local_A2 = local_A2 + 1;
          }
        }
        local_9C = data_17530[local_A3];
        switch (local_9C) {
          case 0:
            if ((local_AC[1] < 13653) | (local_AC[1] > 53703)) {
              if (((&local_B8)->y < 21845) | ((&local_B8)->y > 43690)) {
                local_A2 = local_A2 + 1;
              }
            }
            break;
          case 1:
            if ((local_AC[1] < 30037) & (local_AC[1] > 2730)) {
              if (((&local_B8)->y < 38229) | ((&local_B8)->y > 60074)) {
                local_A2 = local_A2 + 1;
              }
            }
            break;
          case 2:
            if ((local_AC[1] < 46421) & (local_AC[1] > 19114)) {
              if (((&local_B8)->y < 54613) & ((&local_B8)->y > 10922)) {
                local_A2 = local_A2 + 1;
              }
            }
            break;
          case 3:
            if ((local_AC[1] < 62805) & (local_AC[1] > 35498)) {
              if (((&local_B8)->y < 5461) | ((&local_B8)->y > 27306)) {
                local_A2 = local_A2 + 1;
              }
            }
            break;
        }
        (&local_E8)->y = *(float *)((const char *)&data_174A4 + (local_A3 * 12 + 4));
        Script_ScreenToWorldAssign(&local_E8, (void *)local_C4);
        if (-!(*local_C4 <= (float)160) & -!(*local_C4 >= (float)480) & -!(local_C4[1] <= (float)120) & -!(local_C4[1] >= (float)320)) {
          local_A2 = local_A2 + 1;
        }
        if (-(local_A2 >= 3) & -(task_1CA == 0) & -(task_1C5 == 0)) {
          if (task_1C7 == -1) {
            if (local_A3 == 7) {
              task_1C5 = 1;
              script_129CC(local_A3);
            } else {
              if (-(Script_GetControllerInput(1) == 0x400) & Script_ryoControl(PCCTL_QueryShouldRun)) {
                Script_ryoControl(PCCTL_DisablePlayerControl);
                Script_SetCameraFlags(0);
                (void)((task_1C7 = 0) & 2);
                script_119B5();
                Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
                SCNF_WaitFrames_0000(1);
                script_10421(CHARA_AKIR, data_15F90[0], (int32_t)&local_E8);
                Script_SetGameStateTo(AUTH_EventCam);
                Script_seqEnterEvtCam(*(int16_t *)((int8_t *)&data_1751C + (local_A3 << 1)), CHARA_AKIR, CHARA_AKIR);
                Script_ShowUIMessageFromID((const char *)Script_ScenDataUtil(&data_1747C, 8, local_A3));
                while (Script_convGetIsActive(0)) {
                  SCNF_WaitFrames_0000(1);
                }
                Script_SetGameStateTo(AUTH_None);
                script_11A0D();
                (void)((task_1C7 = -1) & 0x10);
                Script_srchUtils(SRCH_SCRIPT_Reset);
                Script_SetCameraFlags(-1);
                Script_ryoControl(RYO_RestoreControl);
                script_129CC(local_A3);
              }
            }
          }
        }
        local_A3 = local_A3 + 1;
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_129CC(int32_t arg0) {
  int16_t local_FE;
  local_FE = (Script_scenUtils(SCENARIO_CMD_GET_STEP_LUT, 21) << 8) | Script_scenUtils(SCENARIO_CMD_GET_STEP_LUT, 22);
  local_FE = local_FE | (1 << arg0);
  Script_scenUtils(SCENARIO_CMD_SET_STEP_LUT, 21, (local_FE & 0xFF00) >> 8);
  Script_scenUtils(SCENARIO_CMD_SET_STEP_LUT, 22, local_FE & 0xFF);
  return;
}

__attribute__((scn_string_bindings(0, data_1783E, 1, data_17848)))
static int32_t script_12A16(int32_t arg0) {
  Vector3 local_F4;
  Vector3 local_E8;
  Vector3 local_DC;
  Vector3i local_D0;
  Vector3i local_C4;
  int32_t unused_C0;
  int8_t local_BF;
  int8_t local_BE;
  int8_t local_BD;
  int8_t local_BC;
  int8_t local_BB;
  int8_t local_BA;
  int8_t local_B9;
  local_BE = 0;
  local_BD = 50;
  (&local_E8)->y = 0.0f;
  (&local_DC)->y = 0.0f;
  (void)((local_BB = 0) & 1);
  (void)((local_BA = 0) & 1);
  Script_camSetRotationStepSize(10);
  for (;;) {
    local_BF = 0;
    (void)((local_BB = 0) & 1);
    if (local_BE) {
      local_BE = local_BE - 1;
    }
    if (Script_ryoControl(PCCTL_QueryShouldRun)) {
      while (local_BF < 60) {
        uint8_t unused_B8;
        int32_t local_B4;
        local_E8.x = *(float *)((const char *)&data_1753C + local_BF * 8);
        (&local_E8)->z = *(float *)((const char *)&data_1753C + (local_BF * 8 + 4));
        local_B4 = data_1771C[local_BF];
        switch (local_B4) {
          case 0:
            local_DC.x = (&local_E8)->x + 1.0f;
            (&local_DC)->z = (&local_E8)->z + 2.0f;
            break;
          case 1:
            local_DC.x = (&local_E8)->x - 2.0f;
            (&local_DC)->z = (&local_E8)->z - 1.0f;
            break;
          case 2:
            local_DC.x = (&local_E8)->x + 2.0f;
            (&local_DC)->z = (&local_E8)->z - 1.0f;
            break;
          case 3:
            local_DC.x = (&local_E8)->x - 1.0f;
            (&local_DC)->z = (&local_E8)->z + 2.0f;
            break;
          case 4:
            local_DC.x = (&local_E8)->x - 1.0f;
            (&local_DC)->z = (&local_E8)->z - 2.0f;
            break;
          case 5:
            local_DC.x = (&local_E8)->x + 2.0f;
            (&local_DC)->z = (&local_E8)->z + 1.0f;
            break;
          case 6:
            local_DC.x = (&local_E8)->x - 2.0f;
            (&local_DC)->z = (&local_E8)->z + 1.0f;
            break;
          case 7:
            local_DC.x = (&local_E8)->x + 1.0f;
            (&local_DC)->z = (&local_E8)->z - 2.0f;
            break;
        }
        if (Script_SetTaskDataValue(CHARA_AKIR, &local_E8, &local_DC, 0)) {
          (void)((local_BB = -1) & 1);
          if (-(Script_GetControllerInput(1) == 0x400) & Script_ryoControl(PCCTL_QueryShouldRun) & -(task_1C4 != 2)) {
            Vector3i local_A8;
            if (task_1CA & -(script_1058C(5, 0) < 2)) {
              if (local_BE == 0) {
                Script_ShowUIMessageFromID("E1006A025");
                local_BE = 30;
              }
              break;
            }
            (void)((local_B9 = task_1CA) & 0x20);
            if ((local_BB == -1) & (local_BA == -1)) {
              SCNF_RequestIconNotification(0, 0, 0, 2);
              (void)((local_BA = 0) & 0x20);
            }
            Script_ryoControl(PCCTL_DisablePlayerControl);
            Script_mtwkGetSetRotation(arg0, &local_D0, 0x42000000);
            Script_mtwkGetSetRotation(arg0, &local_C4, 0x42000000);
            (&local_A8)->y = (&local_C4)->y;
            Script_EVT_SetRotation(arg0, MATRIX_XFORM_Y, &local_A8, 0);
            Script_PlayCharaMotion(arg0, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
            SCNF_WaitFrames_0000(1);
            local_B4 = data_1771C[local_BF];
            if ((local_B4 == 0) | (local_B4 == 5)) {
              local_E8.x = (&local_E8)->x + 0.45f;
              (&local_E8)->z = (&local_E8)->z + 0.45f;
            } else if ((local_B4 == 1) | (local_B4 == 4)) {
              local_E8.x = (&local_E8)->x - 0.45f;
              (&local_E8)->z = (&local_E8)->z - 0.45f;
            } else if ((local_B4 == 2) | (local_B4 == 7)) {
              local_E8.x = (&local_E8)->x + 0.45f;
              (&local_E8)->z = (&local_E8)->z - 0.45f;
            } else {
              if ((local_B4 == 3) | (local_B4 == 6)) {
                local_E8.x = (&local_E8)->x - 0.45f;
                (&local_E8)->z = (&local_E8)->z + 0.45f;
                goto L012DE5;
              }
            }
            L012DE5:;
            local_B4 = data_1771C[local_BF];
            if ((local_B4 == 0) | (local_B4 == 7)) {
              (&local_C4)->y = 0xC000;
            } else if ((local_B4 == 1) | (local_B4 == 2)) {
              (&local_C4)->y = 0;
            } else if ((local_B4 == 3) | (local_B4 == 4)) {
              (&local_C4)->y = 0x4000;
            } else {
              if ((local_B4 == 5) | (local_B4 == 6)) {
                (&local_C4)->y = 0x8000;
                goto L012E65;
              }
            }
            L012E65:;
            if (data_17758[local_BF] == 0) {
              Script_SetGameStateTo(AUTH_EventCam);
              Script_seqSetPlayRoot(&local_E8, &local_C4);
              Script_seqEnterEvtCam(data_1771C[local_BF] + local_BD + 300, 0, AUTH_None);
              if (local_BD) {
                local_BD = 0;
              } else {
                local_BD = 50;
              }
            } else {
              Script_SetGameStateTo(AUTH_EventCam);
              Script_seqSetPlayRoot(&local_E8, &local_C4);
              Script_seqEnterEvtCam(data_1771C[local_BF] + data_17758[local_BF] + 300, 0, AUTH_None);
            }
            Script_SubmitCharaMoveTarget(arg0, &local_E8, (&local_D0)->y, MN_AKI_AKI_STAND_LP, 2.0f);
            while ((Script_GetCharaMoveStatus(arg0) != 0) & (task_1C4 != 2)) {
              SCNF_WaitFrames_0000(1);
            }
            if (task_1C4 == 2) {
              break;
            }
            (void)((local_BC = 0) & 0x80);
            if (task_1C7 == -1) {
              (void)((local_BC = -1) & 0x80);
              script_13761(arg0);
            }
            if (~(-(task_1CA == -1) & -(local_B9 == 0))) {
              script_1058C(4, 1);
            }
            if (task_1C4 != 2) {
              Script_EVT_GetPosition(arg0, -1, &local_F4, 0);
              local_B4 = data_1771C[local_BF];
              if ((local_B4 == 0) | (local_B4 == 7)) {
                local_F4.x = (&local_F4)->x + 1.0f;
              } else if ((local_B4 == 1) | (local_B4 == 2)) {
                (&local_F4)->z = (&local_F4)->z - 1.0f;
              } else if ((local_B4 == 3) | (local_B4 == 4)) {
                local_F4.x = (&local_F4)->x - 1.0f;
              } else {
                if ((local_B4 == 5) | (local_B4 == 6)) {
                  (&local_F4)->z = (&local_F4)->z + 1.0f;
                  goto L013012;
                }
              }
              L013012:;
              Script_SetCharaMotionPlaybackRate(arg0, 2.0f);
              SCNF_CHAR_SCNFApplyFacingTowardPoint(arg0, &local_F4, data_15BD8, 0.0f, 0, 0);
            }
            if (task_1C4 != 2) {
              Script_EVT_GetPosition(arg0, -1, &local_F4, 0);
              local_B4 = data_1771C[local_BF];
              if ((local_B4 == 0) | (local_B4 == 3)) {
                (&local_F4)->z = (&local_F4)->z - 1.0f;
              } else if ((local_B4 == 1) | (local_B4 == 6)) {
                local_F4.x = (&local_F4)->x + 1.0f;
              } else if ((local_B4 == 2) | (local_B4 == 5)) {
                local_F4.x = (&local_F4)->x - 1.0f;
              } else {
                if ((local_B4 == 4) | (local_B4 == 7)) {
                  (&local_F4)->z = (&local_F4)->z + 1.0f;
                  goto L0130DD;
                }
              }
              L0130DD:;
              Script_SetCharaLookAtPosition(arg0, 6, &local_F4, 0);
              local_B4 = data_1771C[local_BF];
              if ((local_B4 == 0) | (local_B4 == 7)) {
                local_F4.x = (&local_F4)->x - 1.0f;
              } else if ((local_B4 == 1) | (local_B4 == 2)) {
                (&local_F4)->z = (&local_F4)->z + 1.0f;
              } else if ((local_B4 == 3) | (local_B4 == 4)) {
                local_F4.x = (&local_F4)->x + 1.0f;
              } else {
                if ((local_B4 == 5) | (local_B4 == 6)) {
                  (&local_F4)->z = (&local_F4)->z - 1.0f;
                  goto L01317E;
                }
              }
              L01317E:;
              Script_FaceLookAtTargetUtils(arg0, 2, &local_F4, 5);
            }
            (void)((task_1C6 = -1) & 0x10);
            while ((Script_GetControllerInput(1) & -2049) == 0) {
              if (task_1C4 == 2) {
                break;
              }
              if (task_1CD == -1) {
                break;
              }
              SCNF_WaitFrames_0000(1);
            }
            Script_FaceLookAtTargetUtils(arg0, 0);
            Script_SetCharaLookAtPosition(arg0, -6, 0, 0);
            if (task_1C4 != 2) {
              local_A8.x = 0;
              (&local_A8)->z = 0;
              local_B4 = data_1771C[local_BF];
              if ((local_B4 == 0) | (local_B4 == 2) | (local_B4 == 4) | (local_B4 == 6)) {
                (&local_A8)->y = 0x6000;
              } else {
                if ((local_B4 == 1) | (local_B4 == 3) | (local_B4 == 5) | (local_B4 == 7)) {
                  (&local_A8)->y = 0xA000;
                  goto L013259;
                }
              }
              L013259:;
              Script_EVT_SetRotation(arg0, -0x6C000000, &local_A8, (int32_t)data_15BD8);
              Script_SetCharaMotionPlaybackRate(arg0, 1.5f);
              for (;;) {
                SCNF_WaitFrames_0000(1);
                if (task_1C4 == 2) {
                  break;
                }
                if (Script_IsCharaMotionFinished(arg0)) {
                  Script_EVT_SetRotation(arg0, -0x70000000, &local_A8, 0);
                  Script_PlayCharaMotion(arg0, MN_AKI_AKI_STAND_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
                  break;
                }
              }
            }
            if (task_1C4 != 2) {
              script_1058C(4, 0);
              if ((task_1C7 == 0) & (local_BC == -1)) {
                script_1371D(arg0);
              }
              Script_ryoControl(RYO_RestoreControl);
              Script_convSetCamInactiveFlag(0);
              Script_SetGameStateTo(AUTH_None);
            }
            (void)((task_1C6 = 0) & 1);
            break;
          }
        }
        local_BF = local_BF + 1;
      }
      local_BF = 0;
      while (local_BF < 10) {
        local_E8.x = *(float *)((const char *)&data_17794 + local_BF * 16);
        (&local_E8)->z = *(float *)((const char *)&data_17794 + (local_BF * 16 + 4));
        local_DC.x = (&local_E8)->x + *(float *)((const char *)&data_17794 + (local_BF * 16 + 8));
        (&local_DC)->z = (&local_E8)->z + *(float *)((const char *)&data_17794 + (local_BF * 16 + 12));
        if (Script_SetTaskDataValue(CHARA_AKIR, &local_E8, &local_DC, 0)) {
          (void)((local_BB = -1) & 1);
          if (-(Script_GetControllerInput(1) == 0x400) & Script_ryoControl(PCCTL_QueryShouldRun)) {
            uint8_t unused_B8;
            Vector3i local_AC;
            if (task_1CA & -(script_1058C(5, 0) < 2)) {
              if (local_BE == 0) {
                Script_ShowUIMessageFromID("E1006A025");
                local_BE = 30;
              }
              break;
            }
            if ((local_BB == -1) & (local_BA == -1)) {
              SCNF_RequestIconNotification(0, 0, 0, 2);
              (void)((local_BA = 0) & 0x20);
            }
            Script_ryoControl(PCCTL_DisablePlayerControl);
            SCNF_WaitFrames_0000(1);
            Script_mtwkGetSetRotation(arg0, &local_C4, 0x42000000);
            (&local_AC)->y = (&local_C4)->y;
            Script_EVT_SetRotation(arg0, MATRIX_XFORM_Y, &local_AC, 0);
            Script_SetGameStateTo(AUTH_EventCam);
            Script_seqEnterEvtCam(data_17834[local_BF] + 300, arg0, arg0);
            (void)((local_BC = 0) & 0x80);
            if (task_1C7 == -1) {
              (void)((local_BC = -1) & 0x80);
              script_13761(arg0);
            }
            script_1058C(4, 1);
            Script_PlayCharaMotion(arg0, MN_AKI_AKI_SIT_TATEHIZA_ST, -1.0f, -1.0f, -1.0f, 1.0f, 0);
            Script_SetCharaMotionPlaybackRate(arg0, 2.5f);
            for (;;) {
              if (Script_IsCharaMotionFinished(arg0)) {
                break;
              }
              SCNF_WaitFrames_0000(1);
            }
            (void)((task_1C6 = -1) & 0x80);
            while ((Script_GetControllerInput(1) & -2049) == 0) {
              if (task_1CA & -(script_1058C(5, 0) <= 0)) {
                Script_ShowUIMessageFromID("E1006A015");
                break;
              }
              if (task_1C4 == 2) {
                break;
              }
              if (task_1CD == -1) {
                break;
              }
              SCNF_WaitFrames_0000(1);
            }
            script_1058C(4, 0);
            if (task_1C4 != 2) {
              Script_PlayCharaMotion(arg0, MN_AKI_AKI_SIT_TATEHIZA_EN, -1.0f, -1.0f, -1.0f, 1.0f, 0);
              Script_SetCharaMotionPlaybackRate(arg0, 3.0f);
              for (;;) {
                if (Script_IsCharaMotionFinished(arg0)) {
                  break;
                }
                SCNF_WaitFrames_0000(1);
              }
              if ((task_1C7 == 0) & (local_BC == -1)) {
                script_1371D(arg0);
              }
              Script_convSetCamInactiveFlag(0);
              Script_SetGameStateTo(AUTH_None);
              Script_ryoControl(RYO_RestoreControl);
            }
            (void)((task_1C6 = 0) & 0x20);
            break;
          }
        }
        local_BF = local_BF + 1;
      }
    }
    if (local_BB == -1) {
      if (local_BA == 0) {
        SCNF_RequestIconNotification(0x100, 0, 1, 1);
      }
    } else {
      if (local_BA == -1) {
        SCNF_RequestIconNotification(0, 0, 0, 2);
      }
    }
    (void)((local_BA = local_BB) & 2);
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_13627(void) {
  int32_t local_FC;
  for (;;) {
    if ((&task_10)->mode) {
      local_FC = task_10.entry_idx;
      if (local_FC == 8) {
        script_119B5();
        script_13761(CHARA_AKIR);
        Script_SetCameraFlags(0);
        SCNF_WaitFrames_0000(90);
        Script_executeAicaCommand(2720, 1, 30);
        Script_executeAicaCommand(2720, 2, 30);
        Script_executeAicaCommand(2720, 3, 30);
        Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
        while (Script_filmCheckOrSetTransitionUtil(-1)) {
          SCNF_WaitFrames_0000(1);
        }
        Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_MKSG_MS08_TRANSITION_TRIGGERED, 1);
        Script_charaSetRefboardFlagTo(CHARA_AKIR, 2);
        if (Script_scenUtils(SCENARIO_CMD_GET_STEP_LUT, 23) >= 2) {
          Script_GetCassetteFlags(50, 1, 0, 0);
        } else {
          if (Script_scenUtils(SCENARIO_CMD_GET_STEP_LUT, 23)) {
            Script_GetCassetteFlags(49, 1, 0, 0);
            goto L013701;
          }
        }
        L013701:;
        Script_shenRequestGameModuleTransition(2, SHEN_NAME('MS08'), 0);
        goto L013711;
      }
    }
    L013711:;
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_1371D(int32_t arg0) {
  script_1058C(3, 1);
  Script_RequestFightAnimation(arg0, 1139, 0.0f, 8, 1);
  SCNF_WaitFrames_0000(4);
  Script_SetStats(STATS_PLAYER_CONTEXT_ACTION_MODE, 3);
  (void)((task_1C7 = -1) & 4);
  Script_lightSetSlotEnabled(data_15F90[0], 1);
  return;
}

static void script_13761(int32_t arg0) {
  (void)((task_1C7 = 0) & 1);
  Script_lightSetSlotEnabled(data_15F90[0], 0);
  script_1058C(3, 0);
  Script_SetStats(STATS_PLAYER_CONTEXT_ACTION_MODE, 0);
  Script_SetRyoMotionDetails(arg0);
  return;
}
