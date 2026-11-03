#include "act01_dcbn.h"

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
static void script_AB97(int32_t arg0);
static int32_t script_ABC3(int32_t arg0, int32_t arg1);
static int8_t script_ABF6(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_AD38(int32_t arg0, int32_t arg1);
static void script_AF01(int32_t arg0, int32_t arg1);
static void script_B0D7(int32_t arg0);
static void script_B0E1(void);
static void script_B105(void);
static void script_B23C(int32_t arg0);
static int32_t script_B7F0(int32_t arg0);
static void script_B84F(int32_t arg0);
static int16_t script_B9F0(int8_t arg0, int8_t arg1);
static void script_BBDB(int32_t arg0, int32_t arg1);
static void script_BC2A(int32_t arg0, int32_t arg1);
static void script_BC7D(int32_t arg0, int32_t arg1, int32_t arg2, float arg3);
static int32_t script_BEB4(void);
static int32_t script_BEE2(int32_t arg0, int32_t arg1);
static void script_C256(int32_t arg0);
static void script_C552(int32_t arg0);
static int8_t script_C672(int32_t arg0);
static void script_CF8F(void);
static void script_D843(void);
static void script_DF9E(void);
static void script_E909(void);
static void script_EDCA(void);
static int32_t script_EEF7(void);
static void script_F68D(void);
static int32_t script_F74E(void);
static int32_t script_F787(int32_t arg0);
static void script_F7BE(int32_t arg0);
static void script_FB0B(void);
static void script_FC2C(void);
static void script_FCA9(void);
static void script_FD19(void);
static void script_FDCD(void);
static void script_FE89(void);
static void script_1023E(void);
static void script_1061E(void);
static void script_106AC(void);
static void script_107E4(int32_t arg0);
static void script_107F0(int32_t arg0);
static void script_107FC(int32_t arg0);
static int32_t script_108F8(int32_t arg0);
static void script_1090F(int32_t arg0);
static void script_10950(int32_t arg0);
static void script_10994(int32_t arg0);
static void script_10A12(void);
static int32_t script_10A91(void);
void script_10AC2(void);
static void script_10CA8(void);
static int32_t script_10CAB(void);
static void script_10DE3(void);
static void script_10E97(void);
static void script_10F44(void);
static void script_1116A(void);
static void script_11184(const char *arg0, const char *arg1);
static void script_111A7(const char *arg0, const char *arg1);

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
static GameDateTime task_1A0;
static int32_t task_1A8;
static int32_t task_1AC;
static int32_t task_1B0;
static int8_t task_1B4;
static uint8_t task_1B5;
static uint16_t task_1B6;
static int32_t task_1B8;
static int32_t task_1BC;
static int32_t task_1C0;
static int32_t task_1C4;
static int8_t task_1C8;
static uint8_t task_1C9;
static uint16_t task_1CA;
static int32_t task_1CC;
static int32_t task_1D0;
static int32_t task_1D4;
static int32_t task_1D8;
static int32_t task_1DC;
static int32_t task_1E0;
static int32_t task_1E4;
static int32_t task_1E8;
static int32_t task_1EC;
static int32_t task_1F0;
static int32_t task_1F4;
static int32_t task_1F8;
static int32_t task_1FC;
static int32_t task_200;
static int32_t task_204;
static int8_t task_208;
static uint8_t task_209;
static uint16_t task_20A;
static int32_t task_20C;
static const Vector3 data_111CC = {.x = 1.0f, .y = 1.0f, .z = 1.0f};
static const int32_t data_111D8[4] = {16777215, 16777215, 16777215, 16777215};
static const float data_111E8[3] = {20.0f, 800.0f, 1.0f};
static const int32_t data_111F4 = -1;
static DoorMotionSelectorBlock data_111F8[38] = {
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
static ScnDoorMotionDescriptor data_117E8[72] = {
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
static uint32_t data_12208 = 0xFFFFFFFF;
typedef struct Row_1220C { float w0; float w1; float w2; float w3; int32_t w4; } Row_1220C;
static const Row_1220C data_1220C[7] = {
  {0.0f, 0.0f, 0.0f, 0.0f, -1},
  {0.0f, 0.0f, 0.0f, 0.0f, -1},
  {0.0f, 0.0f, 0.0f, 0.0f, 1534},
  {26.0f, 55.0f, 45.0f, 75.0f, 1555},
  {61.0f, 146.0f, 45.0f, 50.0f, 1554},
  {59.0f, 124.0f, 45.0f, 70.0f, 1543},
  {33.0f, 49.0f, 45.0f, 50.0f, 1543}
};
static const float data_12298[4] = {33.0f, 49.0f, 50.0f, 50.0f};
static void (*data_122A8)(int32_t, int32_t, int32_t) = script_0768;
static void (*const data_122AC)(int32_t, int32_t, int32_t, int32_t) = script_0778;
static void (*const data_122B0)(int32_t, int32_t, int32_t, int32_t) = script_0778;
static void (*const data_122B4)(int32_t, int32_t, int32_t, int32_t) = script_0778;
static void (*const data_122B8)(int32_t, int32_t, int32_t, int32_t) = script_0778;
static void (*const data_122BC)(int32_t, int32_t, int32_t, int32_t) = script_0778;
static void (*const data_122C0)(int32_t, int32_t, int32_t, int32_t) = script_0778;
static void (*const data_122C4)(int32_t, int32_t, int32_t, int32_t) = script_0778;
static void (*const data_122C8)(int32_t, int32_t, int32_t, int32_t) = script_0778;
static void (*const data_122CC)(int32_t, int32_t, int32_t, int32_t) = script_0A1E;
static void (*const data_122D0)(int32_t, int32_t, int32_t, int32_t) = script_0B7F;
static void (*const data_122D4)(int32_t, int32_t, int32_t, int32_t) = script_104C;
static void (*const data_122D8)(int32_t, int32_t, int32_t, int32_t) = script_10B3;
static void (*const data_122DC)(int32_t, int32_t, int32_t, int32_t) = script_087C;
static void (*const data_122E0)(int32_t, int32_t, int32_t, int32_t) = script_087C;
static void (*const data_122E4)(int32_t, int32_t, int32_t, int32_t) = script_0CE0;
static void (*const data_122E8)(int32_t, int32_t, int32_t, int32_t) = script_0F7E;
static void (*const data_122EC)(int32_t, int32_t, int32_t, int32_t) = script_111A;
static void (*const data_122F0)(int32_t, int32_t, int32_t, int32_t) = script_0A1E;
static void (*const data_122F4)(int32_t, int32_t, int32_t, int32_t) = script_0B7F;
static void (*const data_122F8)(int32_t, int32_t, int32_t, int32_t) = script_0CE0;
static void (*const data_122FC)(int32_t, int32_t, int32_t, int32_t) = script_0A1E;
static void (*const data_12300)(int32_t, int32_t, int32_t, int32_t) = script_0B7F;
static void (*const data_12304)(int32_t, int32_t, int32_t, int32_t) = script_0CE0;
static void (*const data_12308)(int32_t, int32_t, int32_t, int32_t) = script_1161;
static void (*const data_1230C)(int32_t, int32_t, int32_t, int32_t) = script_1161;
static void (*const data_12310)(int32_t, int32_t, int32_t, int32_t) = script_1161;
static void (*const data_12314)(int32_t, int32_t, int32_t, int32_t) = script_1161;
static void (*const data_12318)(int32_t, int32_t, int32_t, int32_t) = script_1297;
static void (*const data_1231C)(int32_t, int32_t, int32_t, int32_t) = script_1B58;
static void (*const data_12320)(int32_t, int32_t, int32_t, int32_t) = script_0778;
static void (*const data_12324)(int32_t, int32_t, int32_t, int32_t) = script_0778;
static void (*const data_12328)(int32_t, int32_t, int32_t, int32_t) = script_0778;
static void (*const data_1232C)(int32_t, int32_t, int32_t, int32_t) = script_0778;
static void (*const data_12330)(int32_t, int32_t, int32_t, int32_t) = script_0778;
static void (*const data_12334)(int32_t, int32_t, int32_t, int32_t) = script_0778;
static void (*const data_12338)(int32_t, int32_t, int32_t, int32_t) = script_0778;
static void (*const data_1233C)(int32_t, int32_t, int32_t, int32_t) = script_0778;
static const char data_12340[46] __attribute__((scn_string_literal)) = "Door Index = %d : call_type = %d : type = %d\012";
static const char data_1236E[43] __attribute__((scn_string_literal)) = "Door Open Motion = DEB_AKESIME_FUSUMA_L_F\012";
static const char data_12399[43] __attribute__((scn_string_literal)) = "Door Open Motion = DEB_AKESIME_FUSUMA_R_F\012";
static const char data_123C4[42] __attribute__((scn_string_literal)) = "Door Open Motion = DEB_HIKISIME_DOOR_L_F\012";
static const char data_123EE[42] __attribute__((scn_string_literal)) = "Door Open Motion = DEB_HIKISIME_DOOR_R_F\012";
static const char data_12418[41] __attribute__((scn_string_literal)) = "Door Open Motion = DEB_OSISIME_DOOR_L_F\012";
static const char data_12441[41] __attribute__((scn_string_literal)) = "Door Open Motion = DEB_OSISIME_DOOR_R_F\012";
static const char data_1246A[43] __attribute__((scn_string_literal)) = "Door Open Motion = GAK_AKESIME_FUSUMA_L_F\012";
static const char data_12495[43] __attribute__((scn_string_literal)) = "Door Open Motion = GAK_AKESIME_FUSUMA_R_F\012";
static const char data_124C0[42] __attribute__((scn_string_literal)) = "Door Open Motion = GAK_HIKISIME_DOOR_L_F\012";
static const char data_124EA[42] __attribute__((scn_string_literal)) = "Door Open Motion = GAK_HIKISIME_DOOR_R_F\012";
static const char data_12514[41] __attribute__((scn_string_literal)) = "Door Open Motion = GAK_OSISIME_DOOR_L_F\012";
static const char data_1253D[41] __attribute__((scn_string_literal)) = "Door Open Motion = GAK_OSISIME_DOOR_R_F\012";
static const char data_12566[43] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_AKESIME_FUSUMA_L_F\012";
static const char data_12591[43] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_AKESIME_FUSUMA_R_F\012";
static const char data_125BC[42] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_HIKISIME_DOOR_L_F\012";
static const char data_125E6[42] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_HIKISIME_DOOR_R_F\012";
static const char data_12610[41] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_OSISIME_DOOR_L_F\012";
static const char data_12639[41] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_OSISIME_DOOR_R_F\012";
static const char data_12662[43] __attribute__((scn_string_literal)) = "Door Open Motion = KOD_AKESIME_FUSUMA_L_F\012";
static const char data_1268D[43] __attribute__((scn_string_literal)) = "Door Open Motion = KOD_AKESIME_FUSUMA_R_F\012";
static const char data_126B8[42] __attribute__((scn_string_literal)) = "Door Open Motion = KOD_HIKISIME_DOOR_L_F\012";
static const char data_126E2[42] __attribute__((scn_string_literal)) = "Door Open Motion = KOD_HIKISIME_DOOR_R_F\012";
static const char data_1270C[41] __attribute__((scn_string_literal)) = "Door Open Motion = KOD_OSISIME_DOOR_L_F\012";
static const char data_12735[41] __attribute__((scn_string_literal)) = "Door Open Motion = KOD_OSISIME_DOOR_R_F\012";
static const char data_1275E[43] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_AKESIME_FUSUMA_L_F\012";
static const char data_12789[43] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_AKESIME_FUSUMA_R_F\012";
static const char data_127B4[42] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_HIKISIME_DOOR_L_F\012";
static const char data_127DE[42] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_HIKISIME_DOOR_R_F\012";
static const char data_12808[41] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_OSISIME_DOOR_L_F\012";
static const char data_12831[41] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_OSISIME_DOOR_R_F\012";
static const char data_1285A[43] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_AKESIME_FUSUMA_L_F\012";
static const char data_12885[43] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_AKESIME_FUSUMA_R_F\012";
static const char data_128B0[42] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_HIKISIME_DOOR_L_F\012";
static const char data_128DA[42] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_HIKISIME_DOOR_R_F\012";
static const char data_12904[41] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_OSISIME_DOOR_L_F\012";
static const char data_1292D[41] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_OSISIME_DOOR_R_F\012";
static const char data_12956[43] __attribute__((scn_string_literal)) = "Door Open Motion = SIN_AKESIME_FUSUMA_L_F\012";
static const char data_12981[43] __attribute__((scn_string_literal)) = "Door Open Motion = SIN_AKESIME_FUSUMA_R_F\012";
static const char data_129AC[42] __attribute__((scn_string_literal)) = "Door Open Motion = SIN_HIKISIME_DOOR_L_F\012";
static const char data_129D6[42] __attribute__((scn_string_literal)) = "Door Open Motion = SIN_HIKISIME_DOOR_R_F\012";
static const char data_12A00[41] __attribute__((scn_string_literal)) = "Door Open Motion = SIN_OSISIME_DOOR_L_F\012";
static const char data_12A29[41] __attribute__((scn_string_literal)) = "Door Open Motion = SIN_OSISIME_DOOR_R_F\012";
static const char data_12A52[43] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_AKESIME_FUSUMA_L_F\012";
static const char data_12A7D[43] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_AKESIME_FUSUMA_R_F\012";
static const char data_12AA8[42] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_HIKISIME_DOOR_L_F\012";
static const char data_12AD2[42] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_HIKISIME_DOOR_R_F\012";
static const char data_12AFC[41] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_OSISIME_DOOR_L_F\012";
static const char data_12B25[41] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_OSISIME_DOOR_R_F\012";
static const char data_12B4E[43] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_AKESIME_FUSUMA_L_F\012";
static const char data_12B79[43] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_AKESIME_FUSUMA_R_F\012";
static const char data_12BA4[42] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_HIKISIME_DOOR_L_F\012";
static const char data_12BCE[42] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_HIKISIME_DOOR_R_F\012";
static const char data_12BF8[41] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_OSISIME_DOOR_L_F\012";
static const char data_12C21[41] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_OSISIME_DOOR_R_F\012";
static const char data_12C4A[44] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_OSISIME_DOOR_AP_L_F\012";
static const char data_12C76[44] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_OSISIME_DOOR_AP_L_F\012";
static const char data_12CA2[44] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_OSISIME_DOOR_AP_L_F\012";
static const char data_12CCE[44] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_OSISIME_DOOR_AP_L_F\012";
static const char data_12CFA[44] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_OSISIME_DOOR_AP_L_F\012";
static const char data_12D26[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_12D42[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_12D5E[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_12D7A[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_12D96[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_12DB2[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_12DCE[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_12DEA[13] __attribute__((scn_string_literal)) = "open-move!!\012";
static const char data_12DF7[11] __attribute__((scn_string_literal)) = "M_MDOR.BIN";
static const char data_12E02[] __attribute__((scn_string_literal)) = "This Charactor Work is NULL!!\n";
static const char data_12E21[22] __attribute__((scn_string_literal)) = "手動ドア開け番号指定\012";
static const char data_12E37[34] __attribute__((scn_string_literal)) = "無限ループになりそうなので抜ける\012";
static const char data_12E59[] __attribute__((scn_string_literal)) = "Don't make BOX Buffer!!\n";
static const char data_12E72[] __attribute__((scn_string_literal)) = "Don't make BOX Buffer!!\n";
static const char data_12E8B[15] __attribute__((scn_string_literal)) = "Door System : ";
static const char data_12E9A[15] __attribute__((scn_string_literal)) = "Door System : ";
static const char data_12EA9[18] __attribute__((scn_string_literal)) = "Door Box is ON!!\012";
static const char data_12EBB[19] __attribute__((scn_string_literal)) = "Door Box is OFF!!\012";
static const char data_12ECE[18] __attribute__((scn_string_literal)) = "Door Box is ON!!\012";
static const char data_12EE0[20] __attribute__((scn_string_literal)) = "Door Box is OFF!!\012";
static const DSWKDoorEntry data_12EF4 = {.slot = 0, .flags = 0, .ctrl_mask_pair = 0x640064U, .coarse_metric = 0.0f, .selection_metric = 0.0f, .view_ang_delta = 0, .group = 0, .global1 = 0xFFFFFFFFU, .global2 = 0xFFFFFFFFU, .global3 = 0xFFFFFFFFU, .sndCommandId = 0xFFFFFFFFU, .global5 = 0xFFFFFFFFU, .global6 = 0xFFFFFFFFU};
static const int32_t data_12F28 = -1;
static const uint8_t data_12F2C[4] = {0};
static const int32_t data_12F30 = -1;
static char data_12F34[] = "DOOR";
static const uint8_t data_12F39[3] = {0};
static char *const data_12F3C = data_12F34;
static const int32_t data_12F40[3] = {11, 656043, 721579};
static const MapObjectRecord data_12F4C = {.kind = 2, .resourceIndex = 0, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-0.9f, 0.0f, 1.455f}, .yawFixed16 = 32768};
static const uint32_t data_12F70[4] = {0x413042CU, 0x42F042DU, 0x4300432U, 0x3870431U};
static const char data_12F80[20] __attribute__((scn_string_literal)) = "TUNED FreeTalk def\012";
static const char data_12F94[20] __attribute__((scn_string_literal)) = "TUNED FreeTalk def\012";
static uint8_t data_12FA8[4] = {0};
static const uint32_t data_12FAC[3] = {0x78626B64U, 0x55550000U, 0};
static const float data_12FB8[4] = {0.2f, 0.1f, 0.0f, 0.0f};
static const int32_t data_12FC8[2] = {-65536, 100};
static const uint32_t data_12FD0 = 0x55550000U;
static const float data_12FD4[5] = {-1.8075f, 0.3525f, -1.417f, 0.0f, 0.0f};
static const int32_t data_12FE8[2] = {-65536, 101};
static const uint32_t data_12FF0 = 0x55550000U;
static const float data_12FF4[5] = {-1.1975f, 0.3525f, -1.417f, 0.0f, 0.0f};
static const int32_t data_13008[2] = {-65536, 102};
static const uint32_t data_13010 = 0x55550000U;
static const float data_13014[5] = {-1.1975f, 0.3525f, -1.417f, 0.0f, 0.0f};
static const int32_t data_13028[2] = {-65536, 103};
static const uint32_t data_13030 = 0x55550000U;
static const float data_13034[5] = {-0.58f, 0.7875f, -1.4225f, 0.0f, 0.0f};
static const int32_t data_13048[2] = {-65536, 104};
static const uint32_t data_13050 = 0x55550000U;
static const float data_13054[5] = {-0.085f, 0.7875f, -1.4225f, 0.0f, 0.0f};
static const int32_t data_13068[2] = {-65536, 105};
static const uint32_t data_13070 = 0x55550000U;
static const float data_13074[5] = {-0.58f, 0.4025f, -1.4225f, 0.0f, 0.0f};
static const int32_t data_13088[2] = {-65536, 106};
static const uint32_t data_13090 = 0x55550000U;
static const float data_13094[5] = {-0.085f, 0.4025f, -1.4225f, 0.0f, 0.0f};
static const int32_t data_130A8[2] = {-65536, 200};
static const uint32_t data_130B0 = 0x55550000U;
static const float data_130B4[5] = {0.6409f, 1.56f, -1.2484f, 0.0f, 0.0f};
static const int32_t data_130C8[2] = {-65536, 201};
static const uint32_t data_130D0 = 0x55550000U;
static const float data_130D4[5] = {0.9234f, 1.56f, -1.2484f, 0.0f, 0.0f};
static const int32_t data_130E8[2] = {-65536, 202};
static const uint32_t data_130F0 = 0x55550000U;
static const float data_130F4[5] = {0.9234f, 1.24f, -1.2484f, 0.0f, 0.0f};
static const int32_t data_13108[2] = {-65536, 203};
static const uint32_t data_13110 = 0x55550000U;
static const float data_13114[5] = {0.9234f, 0.8625f, -1.2484f, 0.0f, 0.0f};
static const int32_t data_13128[2] = {-65536, 300};
static const uint32_t data_13130 = 0x5555C000U;
static const float data_13134[5] = {2.5382f, 1.4525f, -0.3411f, 0.0f, 0.0f};
static const int32_t data_13148[2] = {-65536, 301};
static const uint32_t data_13150 = 0x5555C000U;
static const float data_13154[5] = {2.5382f, 1.135f, -0.3411f, 0.0f, 0.0f};
static const int32_t data_13168[2] = {-65536, 302};
static const uint32_t data_13170 = 0x5555C000U;
static const float data_13174[5] = {2.5382f, 0.4275f, -0.3411f, 0.0f, 0.0f};
static const int32_t data_13188[2] = {-65536, 303};
static const uint32_t data_13190 = 0x5555C000U;
static const float data_13194[5] = {2.5345f, 1.55f, 0.5514f, 0.0f, 0.0f};
static const int32_t data_131A8[2] = {-65536, 304};
static const uint32_t data_131B0 = 0x5555C000U;
static const float data_131B4[5] = {2.5345f, 0.8675f, 0.5514f, 0.0f, 0.0f};
static const int32_t data_131C8[2] = {-65536, 305};
static const uint32_t data_131D0 = 0x5555C000U;
static const float data_131D4[5] = {2.5345f, 0.4575f, 0.5514f, 0.0f, 0.0f};
static const int32_t data_131E8[2] = {-65536, 400};
static const uint32_t data_131F0 = 0x55558000U;
static const float data_131F4[5] = {1.0796f, 1.0925f, 0.635f, 0.0f, 0.0f};
static const int32_t data_13208[2] = {-65536, 401};
static const uint32_t data_13210 = 0x55558000U;
static const float data_13214[5] = {0.8096f, 1.0925f, 0.635f, 0.0f, 0.0f};
static const int32_t data_13228[2] = {-65536, 402};
static const uint32_t data_13230 = 0x55558000U;
static const float data_13234[5] = {1.0796f, 0.34f, 0.635f, 0.0f, 0.0f};
static const int32_t data_13248[2] = {-65536, 403};
static const uint32_t data_13250 = 0x55558000U;
static const float data_13254[5] = {0.8096f, 0.34f, 0.635f, 0.0f, 0.0f};
static const int32_t data_13268 = 589824;
static const uint32_t data_1326C[5] = {0x6B746E63U, 0x55550000U, 0, 0, 0};
static const int32_t data_13280[3] = {61, 5, 589824};
static const uint32_t data_1328C[5] = {0x706E6E63U, 0x55550000U, 0, 0, 0};
static const int32_t data_132A0[3] = {60, 1, 589824};
static const uint32_t data_132AC[5] = {0x706D6E63U, 0x55550000U, 0, 0, 0};
static const int32_t data_132C0[3] = {58, 1, 589824};
static const uint32_t data_132CC[5] = {0x6B6D6E63U, 0x55550000U, 0, 0, 0};
static const int32_t data_132E0[3] = {59, 1, 589824};
static const uint32_t data_132EC[5] = {0x31626E63U, 0x55550000U, 0, 0, 0};
static const int32_t data_13300[3] = {52, 3, 589824};
static const uint32_t data_1330C[5] = {0x33626E63U, 0x55550000U, 0, 0, 0};
static const int32_t data_13320[3] = {53, 3, 589824};
static const uint32_t data_1332C[5] = {0x636D6E63U, 0x55550000U, 0, 0, 0};
static const int32_t data_13340[3] = {55, 5, 589824};
static const uint32_t data_1334C[5] = {0x73726E63U, 0x55550000U, 0, 0, 0};
static const int32_t data_13360[3] = {54, 1, 589824};
static const uint32_t data_1336C[3] = {0x31636E63U, 0x55550000U, 0};
static const float data_13378[2] = {0.05f, 0.0f};
static const int32_t data_13380[3] = {9999, 5, 589824};
static const uint32_t data_1338C[3] = {0x32636E63U, 0x55550000U, 0};
static const float data_13398[2] = {0.05f, 0.0f};
static const int32_t data_133A0[3] = {9999, 5, 589824};
static const uint32_t data_133AC[5] = {0x33636E63U, 0x55550000U, 0, 0, 0};
static const int32_t data_133C0[3] = {9999, 1, 589824};
static const uint32_t data_133CC[5] = {0x34636E63U, 0x55550000U, 0, 0, 0};
static const int32_t data_133E0[3] = {9999, 1, 589824};
static const uint32_t data_133EC[3] = {0x35636E63U, 0x55550000U, 0};
static const float data_133F8[2] = {0.05f, 0.0f};
static const int32_t data_13400[3] = {9999, 5, 589824};
static const uint32_t data_1340C[5] = {0x36636E63U, 0x55550000U, 0, 0, 0};
static const int32_t data_13420[3] = {9999, 1, 589824};
static const uint32_t data_1342C[3] = {0x37636E63U, 0x55550000U, 0};
static const float data_13438[2] = {0.05f, 0.0f};
static const int32_t data_13440[3] = {9999, 5, 589824};
static const uint32_t data_1344C[3] = {0x38636E63U, 0x55550000U, 0};
static const float data_13458[2] = {0.05f, 0.0f};
static const int32_t data_13460[3] = {9999, 5, 589824};
static const uint32_t data_1346C[5] = {0x39636E63U, 0x55550000U, 0, 0, 0};
static const int32_t data_13480[3] = {9999, 1, 589824};
static const uint32_t data_1348C[3] = {0x61636E63U, 0x55550000U, 0};
static const float data_13498[2] = {0.05f, 0.0f};
static const int32_t data_134A0[3] = {9999, 5, 589824};
static const uint32_t data_134AC[5] = {0x62636E63U, 0x55550000U, 0, 0, 0};
static const int32_t data_134C0[3] = {9999, 1, 589824};
static const uint32_t data_134CC[3] = {0x63636E63U, 0x55550000U, 0};
static const float data_134D8[2] = {0.05f, 0.0f};
static const int32_t data_134E0[3] = {9999, 5, 589824};
static const uint32_t data_134EC[3] = {0x64636E63U, 0x55550000U, 0};
static const float data_134F8[2] = {0.05f, 0.0f};
static const int32_t data_13500[3] = {9999, 5, 589824};
static const uint32_t data_1350C[3] = {0x65636E63U, 0x55550000U, 0};
static const float data_13518[2] = {0.05f, 0.0f};
static const int32_t data_13520[3] = {9999, 5, 589824};
static const uint32_t data_1352C[5] = {0x66636E63U, 0x55550000U, 0, 0, 0};
static const int32_t data_13540[3] = {9999, 1, 589824};
static const uint32_t data_1354C[5] = {0x67636E63U, 0x55550000U, 0, 0, 0};
static const int32_t data_13560[3] = {9999, 1, 589824};
static const uint32_t data_1356C[3] = {0x68636E63U, 0x55550000U, 0};
static const float data_13578[2] = {0.05f, 0.0f};
static const int32_t data_13580[3] = {9999, 5, 589824};
static const uint32_t data_1358C[3] = {0x69636E63U, 0x55550000U, 0};
static const float data_13598[2] = {0.05f, 0.0f};
static const int32_t data_135A0[3] = {9999, 5, 589824};
static const uint32_t data_135AC[3] = {0x6A636E63U, 0x55550000U, 0};
static const float data_135B8[2] = {0.05f, 0.0f};
static const int32_t data_135C0[3] = {9999, 5, 589824};
static const uint32_t data_135CC[3] = {0x6B636E63U, 0x55550000U, 0};
static const float data_135D8[2] = {0.05f, 0.0f};
static const int32_t data_135E0[3] = {9999, 5, 589824};
static const uint32_t data_135EC[5] = {0x31736E63U, 0x55550000U, 0, 0, 0};
static const int32_t data_13600[3] = {65, 1, 589824};
static const uint32_t data_1360C[5] = {0x63736E63U, 0x55550000U, 0, 0, 0};
static const int32_t data_13620[3] = {63, 5, 589824};
static const uint32_t data_1362C[5] = {0x6B736E63U, 0x55550000U, 0, 0, 0};
static const int32_t data_13640[3] = {64, 5, 589824};
static const uint32_t data_1364C[5] = {0x646B6E63U, 0x55550000U, 0, 0, 0};
static const int32_t data_13660[3] = {51, 5, 589824};
static const uint32_t data_1366C[5] = {0x67646E63U, 0x55550000U, 0, 0, 0};
static const int32_t data_13680[3] = {56, 5, 0};
static const uint32_t data_1368C[7] = {0x32687364U, 0x55558000U, 0, 0, 0, 0, 0};
static const int32_t data_136A8 = 65535;
static uint8_t data_136AC[12] = {0};
static const int32_t data_136B8 = 1181097;
static const uint32_t data_136BC[33] = {
  0x6B746E63U, 0x6B6D6E63U, 0x31626E63U, 0x33626E63U, 0x73726E63U, 0x646B6E63U, 0x67646E63U, 0x63736E63U, 0x6B736E63U, 0x31636E63U, 0x32636E63U,
  0x33636E63U, 0x34636E63U, 0x35636E63U, 0x36636E63U, 0x37636E63U, 0x38636E63U, 0x39636E63U, 0x61636E63U, 0x62636E63U, 0x63636E63U, 0x64636E63U,
  0x65636E63U, 0x66636E63U, 0x67636E63U, 0x68636E63U, 0x69636E63U, 0x6A636E63U, 0x6B636E63U, 0x6E636E63U, 0x6F636E63U, 0x70636E63U, 0x71636E63U
};
static const int32_t data_13740[5] = {-1, 1246633, 0, 0, 1246633};
static const uint32_t data_13754[2] = {0x31736E63U, 0x706E6E63U};
static const int32_t data_1375C[2] = {-1, -1};
static uint32_t data_13764 = 0x6B746E63;
typedef struct Row_13768 { int32_t w0; uint32_t w1; int32_t w2; uint32_t w3; } Row_13768;
static const Row_13768 data_13768[16] = {
  {180, 0x706E6E63U, 300, 0x706D6E63U},
  {500, 0x6B6D6E63U, 100, 0x31626E63U},
  {150, 0x33626E63U, 150, 0x636D6E63U},
  {50, 0x73726E63U, 150, 0x31636E63U},
  {300, 0x32636E63U, 300, 0x33636E63U},
  {300, 0x34636E63U, 300, 0x35636E63U},
  {300, 0x36636E63U, 300, 0x37636E63U},
  {300, 0x38636E63U, 300, 0x39636E63U},
  {300, 0x61636E63U, 300, 0x62636E63U},
  {300, 0x63636E63U, 300, 0x64636E63U},
  {300, 0x65636E63U, 300, 0x66636E63U},
  {300, 0x67636E63U, 300, 0x68636E63U},
  {300, 0x69636E63U, 300, 0x6A636E63U},
  {300, 0x6B636E63U, 300, 0x31736E63U},
  {120, 0x63736E63U, 120, 0x6B736E63U},
  {120, 0x646B6E63U, 500, 0x67646E63U}
};
static const int32_t data_13868[2] = {90, -1};
static const char data_13870[5] __attribute__((scn_string_literal)) = "SY11";
static const char data_13875[9] __attribute__((scn_string_literal)) = "SY11A006";
static const char data_1387E[9] __attribute__((scn_string_literal)) = "SY11A007";
static const char data_13887[9] __attribute__((scn_string_literal)) = "SY11A008";
static const char data_13890[9] __attribute__((scn_string_literal)) = "SY11A002";
static const char data_13899[9] __attribute__((scn_string_literal)) = "SY11A003";
static const char data_138A2[9] __attribute__((scn_string_literal)) = "SY11A004";
static const char data_138AB[9] __attribute__((scn_string_literal)) = "SY11A005";
static const char data_138B4[9] __attribute__((scn_string_literal)) = "SY11A001";
static const char data_138BD[9] __attribute__((scn_string_literal)) = "SY11A009";
static const char data_138C6[9] __attribute__((scn_string_literal)) = "SY11A010";
static const char data_138CF[9] __attribute__((scn_string_literal)) = "SY11A011";
static const char data_138D8[9] __attribute__((scn_string_literal)) = "SY11A009";
static const char data_138E1[5] __attribute__((scn_string_literal)) = "SY11";
static const char data_138E6[5] __attribute__((scn_string_literal)) = "SY11";
static const char data_138EB[9] __attribute__((scn_string_literal)) = "SY11A001";
static const char data_138F4[5] __attribute__((scn_string_literal)) = "SY11";
static const char data_138F9[5] __attribute__((scn_string_literal)) = "SY11";
static const char data_138FE[9] __attribute__((scn_string_literal)) = "SY11A009";
static const char data_13907[9] __attribute__((scn_string_literal)) = "SY11A010";
static const char data_13910[9] __attribute__((scn_string_literal)) = "SY11A011";
static const char data_13919[7] __attribute__((scn_string_literal)) = "SY11";
static const int8_t data_13920[16] = {12, 17, 22, 10, 19, 18, 11, 8, 4, 4, 17, 7, 6, 5, 4, 4};
static const int8_t data_13930[16] = {12, 1, 2, 1, 3, 2, 2, 8, 4, 4, 17, 7, 6, 5, 4, 4};
static const uint32_t data_13940 = 0xA800A7;
static const int32_t data_13944[5] = {8847529, 8978568, 15073509, 15204583, 15335657};
static const uint32_t data_13958 = 0x440043;
static const int32_t data_1395C[8] = {4587589, 4718663, 4849737, 4980811, 5111885, 5242959, 5374033, 85};
static const uint32_t data_1397C = 0x5C005B;
static const int32_t data_13980[10] = {6160477, 6291551, 6422625, 6553699, 6684773, 6815847, 6946921, 7077995, 7209069, 7340143};
static const uint32_t data_139A8 = 0x740073;
static const int32_t data_139AC[4] = {7798902, 7929976, 8061050, 8257660};
static const uint32_t data_139BC = 0x720071;
static const int32_t data_139C0[9] = {10223750, 10354845, 10485919, 10616993, 10748067, 10879141, 11206826, 5767255, 180};
static const uint32_t data_139E4 = 0x810080;
static const int32_t data_139E8[8] = {8585346, 11403437, 11534511, 11665585, 11862195, 11993270, 12452024, 8650879};
static const uint32_t data_13A08 = 0xB900BC;
static const int32_t data_13A0C[5] = {12255418, 13238473, 13500621, 12517579, 200};
static const uint32_t data_13A20 = 0x5A0059;
static const int32_t data_13A24[3] = {5636268, 14221445, 5505107};
static const uint32_t data_13A30 = 0x7D0075;
static const int32_t data_13A34 = 12386442;
static const uint32_t data_13A38 = 0xC600C5;
static const int32_t data_13A3C = 13369543;
static const uint32_t data_13A40 = 0x8C008B;
static const int32_t data_13A44[8] = {9306253, 9437327, 9633937, 148, 9961623, 10092697, 9568411, 149};
static const uint32_t data_13A64 = 0xD700D6;
static const int32_t data_13A68[3] = {14287064, 14876891, 228};
static const uint32_t data_13A74 = 0xDC00D3;
static const int32_t data_13A78[2] = {13893845, 14549213};
static const uint32_t data_13A80 = 0xC100C0;
static const int32_t data_13A84[2] = {12779714, 196};
static const uint32_t data_13A8C = 0xD000CF;
static const int32_t data_13A90 = 13762769;
static const uint32_t data_13A94 = 0xE000DF;
static const int32_t data_13A98 = 14811361;
static const char data_13A9C[169] = "%GACR016G.MT5\000%GACK66BG.MT5\000%GACK66RG.MT5\000%GACR0BNG.MT5\000%GACR0B2G.MT5\000%GACR0B3G.MT5\000%SPBK6REG.MT5\000%SPBK6BRG.MT5\000%SPBK6YEG.MT5\000%SAIK6WHG.MT5\000%SAIK6BKG.MT5\000%SAIK6REG.MT5";
static const char data_13B45[315] = "%GACIAK1G.MT5\000%GACIAK2G.MT5\000%GACOJK1G.MT5\000%GACOJK2G.MT5\000%GACOSA1G.MT5\000%GACOSA2G.MT5\000%GACOLA1G.MT5\000%GACOLA2G.MT5\000%GACOPA1G.MT5\000%GACOPA2G.MT5\000%GACGWF1G.MT5\000%GACGWF2G.MT5\000%GACGJF1G.MT5\000%GACGJF2G.MT5\000%GACIKA1G.MT5\000%GACIKA2G.MT5\000%GACM401G.MT5\000%GACIAA1G.MT5\000%GACIAA2G.MT5\000D\000D\000D\000D\000D\000D\000D\000D\000D\000D\000%GACGJJ1G.MT5\000%GACGJJ2G.MT5";
static const char data_13C80[309] = "%GACIAKAG.MT5\000%GACIAKBG.MT5\000%GACGJKAG.MT5\000%GACGJKBG.MT5\000%GACOSAAG.MT5\000%GACOSABG.MT5\000%GACOLAAG.MT5\000%GACOLABG.MT5\000%GACOPAAG.MT5\000%GACOPABG.MT5\000%GACIWLAG.MT5\000%GACIWLBG.MT5\000%GACGJFAG.MT5\000%GACGJFBG.MT5\000%GACIKAAG.MT5\000%GACIKABG.MT5\000%GACGSNAG.MT5\000%GACGSNBG.MT5\000%GACILIAG.MT5\000%GACILIBG.MT5\000%GACIDUAG.MT5\000%GACIDUBG.MT5";
static const char data_13DB5[167] = "%GACRSO1G.MT5\000%GACRSO2G.MT5\000%GACRTL1G.MT5\000%GACRNUCG.MT5\000%GACRBAKG.MT5\000%GACRBING.MT5\000%GACRFNGG.MT5\000%GACRAMYG.MT5\000%GACRESPG.MT5\000%GACREGGG.MT5\000D\000D\000D\000D\000D\000D\000%GACRFNNG.MT5";
static const char data_13E5C[365] = "%GACRNT1G.MT5\000%GACRNT2G.MT5\000%GACRLOPG.MT5\000%GACRMYUG.MT5\000%GACRCKNG.MT5\000%GACRALFG.MT5\000%GACRRISG.MT5\000%GACRARXG.MT5\000%GACR0OPG.MT5\000%GACR0O2G.MT5\000%GACRPOPG.MT5\000%GACRCOBG.MT5\000%GACRIDAG.MT5\000%GACRSHPG.MT5\000%GACRMOBG.MT5\000%GACRROBG.MT5\000%GACK6TZG.MT5\000%GACRPANG.MT5\000%GACK6DYG.MT5\000D\000D\000D\000D\000D\000D\000D\000%GACRAXXG.MT5\000D\000D\000D\000D\000D\000D\000D\000%GACRRBBG.MT5\000%GACK6TSG.MT5\000%GACRPPNG.MT5\000%GACK6DSG.MT5";
static const char data_13FC9[505] = "%GACK6HHG.MT5\000%GACK6HYG.MT5\000%GACK6HBG.MT5\000%GACK6HGG.MT5\000%GACH5BKG.MT5\000%KAB02FSG.MT5\000%GACO1K2G.MT5\000%GACO1K3G.MT5\000%GACO1K7G.MT5\000%GACO1K8G.MT5\000%GACO1K9G.MT5\000%GACO1K1G.MT5\000%GACO1K4G.MT5\000%GACO1K5G.MT5\000%GACO1K6G.MT5\000%GACS5BSG.MT5\000%GACK6HOG.MT5\000%GACK6HKG.MT5\000%GACK6XHG.MT5\000%GACK6XYG.MT5\000%GACK6XBG.MT5\000%GACK6XGG.MT5\000%GACK6BKG.MT5\000%KABK6FSG.MT5\000%GACOKK2G.MT5\000%GACOKK3G.MT5\000%GACOKK7G.MT5\000%GACOKK8G.MT5\000%GACOKK9G.MT5\000%GACOKK1G.MT5\000%GACOKK4G.MT5\000%GACOKK5G.MT5\000%GACOKK6G.MT5\000%GACS5BBG.MT5\000%GACK6XOG.MT5\000%GACK6XKG.MT5";
static const char data_141C2[211] = "%GACH5TKG.MT5\000%GACGTRXG.MT5\000%GACO1T1G.MT5\000%GACO1T2G.MT5\000%GACO1KOG.MT5\000%GACO1KBG.MT5\000%GACO1KJG.MT5\000%GACH5UKG.MT5\000%GACH5IKG.MT5\000%GACO1FOG.MT5\000%GACGSHEG.MT5\000%GACH5TTG.MT5\000%GACGTXXG.MT5\000%GACOTT1G.MT5\000%GACOTT2G.MT5";
static const char data_14295[113] = "%YKW0201G.MT5\000%YWT0201G.MT5\000%GACRBONG.MT5\000%GACRRENG.MT5\000%GACK6SPG.MT5\000%GACG360G.MT5\000%GACRDR1G.MT5\000%GACRDR2G.MT5";
static const char data_14306[77] = "%GACRSSCG.MT5\000%GACRMETG.MT5\000%GACRGBIG.MT5\000%GACH5TRG.MT5\000D\000D\000D\000%GACH5RRG.MT5";
static const char data_14353[57] = "%GACS5FRG.MT5\000%GACS5FBG.MT5\000%GACO1KUG.MT5\000%GACO1FEG.MT5";
static const char data_1438C[239] = "%GACODG1G.MT5\000%GACODG2G.MT5\000%GACODG3G.MT5\000%GACOCT2G.MT5\000%GACOCATG.MT5\000%GACO1SHG.MT5\000%GACOKSHG.MT5\000%GACO1IRG.MT5\000%GACOKIRG.MT5\000%GACO1HAG.MT5\000%GACOHA2G.MT5\000%GACOHA3G.MT5\000%GACOHA4G.MT5\000%GACO1YGG.MT5\000%GACO1USG.MT5\000%GACOOSHG.MT5\000%GACOOIRG.MT5";
static const char data_1447B[99] = "%GACM400G.MT5\000%GACGSPHG.MT5\000%GACS5HGG.MT5\000%GACGDT1G.MT5\000%GACS5EQG.MT5\000%SYOGSH0G.MT5\000%SYOGHG0G.MT5";
static const char data_144DE[85] = "%GACGCRYG.MT5\000%GACGBIRG.MT5\000%GACH5SLG.MT5\000%GACH5JKG.MT5\000%SYOGMD1G.MT5\000%SYOGMD5G.MT5";
static const char data_14533[71] = "%GACS5N1G.MT5\000%GACS5N2G.MT5\000%GACS5N3G.MT5\000%GACS5N4G.MT5\000%GACS5N5G.MT5";
static const char data_1457A[57] = "%GACGXBNG.MT5\000%GACGYBNG.MT5\000%GACGABNG.MT5\000%GACGBBNG.MT5";
static const char data_145B3[57] = "%GACRMEGG.MT5\000%GACR0GGG.MT5\000%GACRSATG.MT5\000%GACRCASG.MT5";
static const int32_t data_145EC[171] = {
  0, 0, -6005, 0, 4731, -6188, 0, 0, 0, 0, 0, -2727, 0, 0, -9097, 0, 0, -5457, 0, 0, 0, 0, 0, -8734, 0, -909, -7277, 0, 0, -7823, 0, 0, 0, 0, 1456,
  -4002, 0, 0, -9462, 0, 726, -7824, 0, 0, 0, 0, 0, -12011, 0, -1091, -7461, 0, -1636, -9280, 0, 0, 0, 0, 0, -6003, 0, 0, -908, 0, 0, 0, 0, 0, -3819,
  0, 0, -7094, 0, 0, -4547, 0, 0, 0, 0, 0, -2909, 0, 0, -8550, 0, -544, -4001, 0, 0, 0, 0, 0, -4363, 0, 0, -8004, 0, -908, -2180, 0, 0, 0, 0, -1634,
  -6366, 0, -2000, -4365, 0, -1818, -2182, 0, 0, 0, 0, 0, -10922, 0, 0, -10558, 0, 1456, 0, 0, 0, -18022, 0, 0, -17112, 0, 0, -18386, 0, 0, 0, 0, 0,
  -12196, 0, 0, -18204, 0, 0, -16748, 0, 0, 0, 0, 0, -11832, 0, 0, -18386, 0, 0, -15109, 0, 0, 0, 0, 819, -12014, 0, -1274, -16748, 0, -3458, -16201,
  0, 0, 0
};
static const int32_t data_14898[57] = {
  0, 0, -3093, 0, 0, -1455, 0, 0, 0, 0, 0, -3275, 0, 0, -2001, 0, 0, -1637, 0, 0, 0, 0, 0, -12193, 0, 0, -12921, 0, 0, -4185, 0, 0, -363, 0, 0,
  -14195, 0, 0, -10919, 0, 0, -4550, 0, 0, 181, 0, 0, -15651, 0, 0, -10373, 0, 0, -6005, 0, -909, 181
};
static const char data_1497C[18] __attribute__((scn_string_literal)) = "[[[UISRCHDCB26]]]";
static const char data_1498E[18] __attribute__((scn_string_literal)) = "[[[UISRCHDCB25]]]";
static const char data_149A0[18] __attribute__((scn_string_literal)) = "[[[UISRCHDCB23]]]";
static const char data_149B2[18] __attribute__((scn_string_literal)) = "[[[UISRCHDCB22]]]";
static const char data_149C4[18] __attribute__((scn_string_literal)) = "[[[UISRCHDCB18]]]";
static const char data_149D6[18] __attribute__((scn_string_literal)) = "[[[UISRCHDCB19]]]";
static const char data_149E8[18] __attribute__((scn_string_literal)) = "[[[UISRCHDCB24]]]";
static const char data_149FA[18] __attribute__((scn_string_literal)) = "[[[UISRCHDCB20]]]";
static const char data_14A0C[18] __attribute__((scn_string_literal)) = "[[[UISRCHDCB27]]]";
static const char data_14A1E[18] __attribute__((scn_string_literal)) = "[[[UISRCHDCB28]]]";
static const char data_14A30[18] __attribute__((scn_string_literal)) = "[[[UISRCHDCB29]]]";
static const char data_14A42[18] __attribute__((scn_string_literal)) = "[[[UISRCHDCB30]]]";
static const char data_14A54[18] __attribute__((scn_string_literal)) = "[[[UISRCHDCB31]]]";
static const char data_14A66[18] __attribute__((scn_string_literal)) = "[[[UISRCHDCB32]]]";
static const char data_14A78[18] __attribute__((scn_string_literal)) = "[[[UISRCHDCB33]]]";
static const char data_14A8A[18] __attribute__((scn_string_literal)) = "[[[UISRCHDCB34]]]";
static const char data_14A9C[18] __attribute__((scn_string_literal)) = "[[[UISRCHDCB35]]]";
static const char data_14AAE[18] __attribute__((scn_string_literal)) = "[[[UISRCHDCB36]]]";
static const char data_14AC0[18] __attribute__((scn_string_literal)) = "[[[UISRCHDCB37]]]";
static const char data_14AD2[18] __attribute__((scn_string_literal)) = "[[[UISRCHDCB38]]]";
static const char data_14AE4[18] __attribute__((scn_string_literal)) = "[[[UISRCHDCB39]]]";
static const char data_14AF6[18] __attribute__((scn_string_literal)) = "[[[UISRCHDCB40]]]";
static const char data_14B08[18] __attribute__((scn_string_literal)) = "[[[UISRCHDCB41]]]";
static const char data_14B1A[18] __attribute__((scn_string_literal)) = "[[[UISRCHDCB42]]]";
static const char data_14B2C[18] __attribute__((scn_string_literal)) = "[[[UISRCHDCB43]]]";
static const char data_14B3E[18] __attribute__((scn_string_literal)) = "[[[UISRCHDCB44]]]";
static const char data_14B50[18] __attribute__((scn_string_literal)) = "[[[UISRCHDCB45]]]";
static const char data_14B62[18] __attribute__((scn_string_literal)) = "[[[UISRCHDCB46]]]";
static const char data_14B74[18] __attribute__((scn_string_literal)) = "[[[UISRCHDCB14]]]";
static const char data_14B86[18] __attribute__((scn_string_literal)) = "[[[UISRCHDCB16]]]";
static const char data_14B98[18] __attribute__((scn_string_literal)) = "[[[UISRCHDCB15]]]";
static const char data_14BAA[18] __attribute__((scn_string_literal)) = "[[[UISRCHDCB17]]]";
static const char data_14BBC[18] __attribute__((scn_string_literal)) = "[[[UISRCHDCB21]]]";
static const char data_14BCE[11] __attribute__((scn_string_literal)) = "SA1079A003";
static const char data_14BD9[11] __attribute__((scn_string_literal)) = "SA1079A004";
static const char data_14BE4[11] __attribute__((scn_string_literal)) = "SA1079A005";
static const char data_14BEF[11] __attribute__((scn_string_literal)) = "SA1079A006";
static const char data_14BFA[11] __attribute__((scn_string_literal)) = "SA1079A001";
static const char data_14C05[11] __attribute__((scn_string_literal)) = "SA1079A002";
static const Vector3 data_14C10 = {.x = -2.76f, .y = 0.0f, .z = 0.21f};
static const float data_14C1C[6] = {-2.9f, -0.05f, 0.05f, -2.76f, 0.0f, 0.21f};
static const Vector3i data_14C34 = {.x = -4096, .y = 0, .z = 0};
static const int32_t data_14C40[3] = {-11832, 0, 0};
static const char data_14C4C[11] __attribute__((scn_string_literal)) = "SA1079C007";
static const char data_14C57[11] __attribute__((scn_string_literal)) = "SA1079B007";
static const char data_14C62[11] __attribute__((scn_string_literal)) = "SA1079A007";
static const char data_14C6D[11] __attribute__((scn_string_literal)) = "SA1079A009";
static const char data_14C78[11] __attribute__((scn_string_literal)) = "SA1079A011";
static const char data_14C83[11] __attribute__((scn_string_literal)) = "SA1079C013";
static const char data_14C8E[38] = "SA1079B014\000\000\000\000\000\340\377\377\000\000\000\000\000\000\000\000\310\321\377\377";
static const char data_14CB4[11] __attribute__((scn_string_literal)) = "SA1079A013";
static const char data_14CBF[11] __attribute__((scn_string_literal)) = "SA1079C016";
static const char data_14CCA[11] __attribute__((scn_string_literal)) = "SA1079B018";
static const char data_14CD5[11] __attribute__((scn_string_literal)) = "SA1079A014";
static const char data_14CE0[11] __attribute__((scn_string_literal)) = "SA1079C020";
static const char data_14CEB[11] __attribute__((scn_string_literal)) = "SA1079C018";
static const char data_14CF6[11] __attribute__((scn_string_literal)) = "SA1079C015";
static const char data_14D01[11] __attribute__((scn_string_literal)) = "SA1079B025";
static const char data_14D0C[11] __attribute__((scn_string_literal)) = "SA1079B020";
static const char data_14D17[11] __attribute__((scn_string_literal)) = "SA1079B016";
static const char data_14D22[11] __attribute__((scn_string_literal)) = "SA1079B019";
static const char data_14D2D[11] __attribute__((scn_string_literal)) = "SA1079C019";
static const char data_14D38[11] __attribute__((scn_string_literal)) = "SA1079B021";
static const char data_14D43[11] __attribute__((scn_string_literal)) = "SA1079B022";
static const char data_14D4E[11] __attribute__((scn_string_literal)) = "SA1079A015";
static const char data_14D59[11] __attribute__((scn_string_literal)) = "SA1079A016";
static const char data_14D64[11] __attribute__((scn_string_literal)) = "SA1079A017";
static const char data_14D6F[11] __attribute__((scn_string_literal)) = "SA1079C014";
static const char data_14D7A[11] __attribute__((scn_string_literal)) = "SA1079B015";
static const char data_14D85[11] __attribute__((scn_string_literal)) = "SA1079B017";
static const Vector3 data_14D90 = {.x = -2.83f, .y = 0.0f, .z = -0.48f};
static const Vector3i data_14D9C = {.x = -4096, .y = 0, .z = 0};
static const int32_t data_14DA8[3] = {-11832, 0, 0};
static const char data_14DB4[11] __attribute__((scn_string_literal)) = "SA1079A007";
static const char data_14DBF[11] __attribute__((scn_string_literal)) = "SA1079C013";
static const char data_14DCA[11] __attribute__((scn_string_literal)) = "SA1079B014";
static const char data_14DD5[11] __attribute__((scn_string_literal)) = "SA1079C011";
static const char data_14DE0[11] __attribute__((scn_string_literal)) = "SA1079C012";
static const char data_14DEB[11] __attribute__((scn_string_literal)) = "SA1079B011";
static const char data_14DF6[14] __attribute__((scn_string_literal)) = "SA1079B012";
static const uint32_t data_14E04 = 380;
static const int32_t data_14E08[6] = {1, 390, 0, 1018, 0, -1};
static const uint32_t data_14E20 = 510;
static const int32_t data_14E24[6] = {1, 525, 0, 1020, 0, -1};
static const uint32_t data_14E3C = 150;
static const int32_t data_14E40[6] = {1, 160, 0, 1019, 0, -1};
static const uint32_t data_14E58 = 530;
static const int32_t data_14E5C[6] = {1, 970, 0, 1014, 0, -1};
static const uint32_t data_14E74 = 911;
static const int32_t data_14E78[6] = {1, 155, 0, 1015, 0, -1};
static const char data_14E90[7] __attribute__((scn_string_literal)) = "FREE02";
static const char data_14E97[7] __attribute__((scn_string_literal)) = "FREE03";
static const char data_14E9E[7] __attribute__((scn_string_literal)) = "FREE04";
static const char data_14EA5[7] __attribute__((scn_string_literal)) = "FREE01";
static const char data_14EAC[11] __attribute__((scn_string_literal)) = "M_MOBJ.BIN";
static const char data_14EB7[14] __attribute__((scn_string_literal)) = "scene/01/D000";
static const char data_14EC5[] __attribute__((scn_string_literal)) = "SCRTEST.BIN";
static const char data_14ED1[7] __attribute__((scn_string_literal)) = "SA1079";
static const Vector3 data_14ED8 = {.x = 2.8f, .y = 0.0f, .z = 1.38f};
static const Vector3 data_14EE4 = {.x = -3.67f, .y = 0.0f, .z = -2.2f};
static const char data_14EF0[5] __attribute__((scn_string_literal)) = "DCB0";
static const char data_14EF5[5] __attribute__((scn_string_literal)) = "DCB1";
static const char data_14EFA[5] __attribute__((scn_string_literal)) = "DCB2";
static const char data_14EFF[5] __attribute__((scn_string_literal)) = "DCB3";
static const char data_14F04[5] __attribute__((scn_string_literal)) = "DCB4";
static const char data_14F09[] __attribute__((scn_string_literal)) = "/scene/01/DCBN/";
static const char data_14F19[] __attribute__((scn_string_literal)) = "DCB0";
static const char data_14F1E[] __attribute__((scn_string_literal)) = "/scene/01/DCBN/";
static const char data_14F2E[] __attribute__((scn_string_literal)) = "DCB1";
static const char data_14F33[] __attribute__((scn_string_literal)) = "/scene/01/DCBN/";
static const char data_14F43[] __attribute__((scn_string_literal)) = "DCB2";
static const char data_14F48[] __attribute__((scn_string_literal)) = "/scene/01/DCBN/";
static const char data_14F58[] __attribute__((scn_string_literal)) = "DCB3";
static const char data_14F5D[] __attribute__((scn_string_literal)) = "/scene/01/DCBN/";
static const char data_14F6D[] __attribute__((scn_string_literal)) = "DCB4";
static const uint8_t data_14F72[2] = {0};

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
    *(int32_t *)&local_F8 = Script_ScenDataUtil(data_117E8, 5, Script_ScenDataUtil(data_111F8, 1, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 5);
    *(int32_t *)&local_F4 = Script_ScenDataUtil(data_117E8, 5, Script_ScenDataUtil(data_111F8, 1, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 6);
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
    *(int32_t *)&local_F4 = Script_ScenDataUtil(data_117E8, 5, Script_ScenDataUtil(data_111F8, 1, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 5);
    local_F4 = 90.0f - local_F4;
    *(int32_t *)&local_F0 = Script_ScenDataUtil(data_117E8, 5, Script_ScenDataUtil(data_111F8, 1, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 6);
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
    *(int32_t *)&local_F8 = Script_ScenDataUtil(data_117E8, 5, Script_ScenDataUtil(data_111F8, 1, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 5);
    *(int32_t *)&local_F4 = Script_ScenDataUtil(data_117E8, 5, Script_ScenDataUtil(data_111F8, 1, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 6);
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
    *(int32_t *)&local_F8 = Script_ScenDataUtil(data_117E8, 5, Script_ScenDataUtil(data_111F8, 1, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 5);
    *(int32_t *)&local_F4 = Script_ScenDataUtil(data_117E8, 5, Script_ScenDataUtil(data_111F8, 1, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 6);
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
      *(int32_t *)&local_F4 = Script_ScenDataUtil(data_117E8, 5, Script_ScenDataUtil(data_111F8, 1, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 5);
      *(int32_t *)&local_F0 = Script_ScenDataUtil(data_117E8, 5, Script_ScenDataUtil(data_111F8, 1, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 6);
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
        *(int32_t *)&local_F4 = Script_ScenDataUtil(data_117E8, 5, Script_ScenDataUtil(data_111F8, 1, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 5);
        *(int32_t *)&local_F0 = Script_ScenDataUtil(data_117E8, 5, Script_ScenDataUtil(data_111F8, 1, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 6);
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
    *(int32_t *)&local_F8 = Script_ScenDataUtil(data_117E8, 5, Script_ScenDataUtil(data_111F8, 1, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 5);
    *(int32_t *)&local_F4 = Script_ScenDataUtil(data_117E8, 5, Script_ScenDataUtil(data_111F8, 1, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 6);
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
    *(int32_t *)&local_F8 = Script_ScenDataUtil(data_117E8, 5, Script_ScenDataUtil(data_111F8, 1, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 5);
    *(int32_t *)&local_F4 = Script_ScenDataUtil(data_117E8, 5, Script_ScenDataUtil(data_111F8, 1, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 6);
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
      *(int32_t *)&local_FF78 = Script_ScenDataUtil(data_117E8, 5, Script_ScenDataUtil(data_111F8, 1, arg3 * 40 + task_98[arg2] * 4 + local_FF7F) * 9 + 5);
      *(int32_t *)&local_FF74 = Script_ScenDataUtil(data_117E8, 5, Script_ScenDataUtil(data_111F8, 1, arg3 * 40 + task_98[arg2] * 4 + local_FF7F) * 9 + 6);
      local_FF7F = script_8C20(arg0, arg1, 5, local_FF78 / (float)100, local_FF74 / (float)100, arg3, -1);
      script_92E9(arg0, 0);
      *(int32_t *)&local_94 = Script_ScenDataUtil(data_117E8, 5, Script_ScenDataUtil(data_111F8, 1, arg3 * 40 + task_98[arg2] * 4 + local_FF7F) * 9 + 1);
      *(int32_t *)&local_90 = Script_ScenDataUtil(data_117E8, 5, Script_ScenDataUtil(data_111F8, 1, arg3 * 40 + task_98[arg2] * 4 + local_FF7F) * 9 + 2);
      local_8C = 0.0f;
      local_FC = Script_ScenDataUtil(data_117E8, 4, Script_ScenDataUtil(data_111F8, 1, arg3 * 40 + task_98[arg2] * 4 + local_FF7F) * 9);
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
    *(int32_t *)&local_F8 = Script_ScenDataUtil(data_117E8, 5, Script_ScenDataUtil(data_111F8, 1, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 5);
    local_F8 = 98.0f - local_F8;
    local_F4 = Script_ScenDataUtil(data_117E8, 5, Script_ScenDataUtil(data_111F8, 1, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 6);
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
      *(int32_t *)&local_D0 = Script_ScenDataUtil(&data_12208, 5, Script_ScenDataUtil(task_0, 4, task_48[arg2] * 13 + 6) * 5 + 3);
      *(int32_t *)&local_CC = Script_ScenDataUtil(&data_12208, 5, Script_ScenDataUtil(task_0, 4, task_48[arg2] * 13 + 6) * 5 + 4);
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
          *(int32_t *)&local_D0 = Script_ScenDataUtil(&data_12208, 5, Script_ScenDataUtil(task_0, 4, task_48[arg2] * 13 + 6) * 5 + 3);
          *(int32_t *)&local_CC = Script_ScenDataUtil(&data_12208, 5, Script_ScenDataUtil(task_0, 4, task_48[arg2] * 13 + 6) * 5 + 4);
          local_FC = script_8C20(arg0, arg1, local_F8, local_D0 / (float)100, local_CC / (float)100, arg3, task_48[arg2]);
          Script_scenUtils(SCENARIO_CMD_INIT_SCNT);
          script_6BDE(arg0, arg1, arg2, arg3, local_FC, 0);
          Script_scenUtils(SCENARIO_CMD_UPDATE_TIMER);
        } else {
          *(int32_t *)&local_D0 = Script_ScenDataUtil(&data_12208, 5, Script_ScenDataUtil(task_0, 4, task_48[arg2] * 13 + 6) * 5 + 3);
          *(int32_t *)&local_CC = Script_ScenDataUtil(&data_12208, 5, Script_ScenDataUtil(task_0, 4, task_48[arg2] * 13 + 6) * 5 + 4);
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

__attribute__((scn_string_bindings(0, data_12D26)))
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
  *(int32_t *)&local_98 = Script_ScenDataUtil(data_117E8, 5, Script_ScenDataUtil(data_111F8, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 1);
  *(int32_t *)&local_94 = Script_ScenDataUtil(data_117E8, 5, Script_ScenDataUtil(data_111F8, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 2);
  *(int32_t *)&local_90 = Script_ScenDataUtil(data_117E8, 5, Script_ScenDataUtil(data_111F8, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 3);
  *(int32_t *)&local_8C = Script_ScenDataUtil(data_117E8, 5, Script_ScenDataUtil(data_111F8, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 4);
  *(int32_t *)&local_88 = Script_ScenDataUtil(data_117E8, 5, Script_ScenDataUtil(data_111F8, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 7);
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
      local_FF78 = Script_ScenDataUtil(data_117E8, 4, Script_ScenDataUtil(data_111F8, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
      local_FF7C = 18;
      local_80 = 18;
      local_A4 = 1.0f;
    } else {
      if (arg0 == task_160) {
        local_FF78 = Script_ScenDataUtil(data_117E8, 4, Script_ScenDataUtil(data_111F8, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9) | -0x80000000;
        local_FF7C = 12;
        local_80 = 12;
      } else {
        local_FF78 = Script_ScenDataUtil(data_117E8, 4, Script_ScenDataUtil(data_111F8, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
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
          *(int32_t *)&local_9C = Script_ScenDataUtil(data_117E8, 5, Script_ScenDataUtil(data_111F8, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 8);
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

__attribute__((scn_string_bindings(0, data_12D42)))
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
          local_FC = Script_ScenDataUtil(data_117E8, 4, Script_ScenDataUtil(data_111F8, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
          break;
        case 0:
          local_B7 = 0;
          local_F8 = 12;
          local_F0 = 0x4000;
          local_B8 = 1.0f;
          local_FC = Script_ScenDataUtil(data_117E8, 4, Script_ScenDataUtil(data_111F8, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9) | -0x80000000;
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
          local_FC = Script_ScenDataUtil(data_117E8, 4, Script_ScenDataUtil(data_111F8, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9) | -0x80000000;
          break;
        case 0:
          local_B7 = 1;
          local_F8 = 18;
          local_F0 = 0xC000;
          local_B8 = 1.0f;
          local_FC = Script_ScenDataUtil(data_117E8, 4, Script_ScenDataUtil(data_111F8, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
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
          local_FC = Script_ScenDataUtil(data_117E8, 4, Script_ScenDataUtil(data_111F8, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9) | -0x80000000;
          break;
        case 0:
          local_B7 = 0;
          local_F8 = 18;
          local_F0 = 0xC000;
          local_B8 = -1.0f;
          local_FC = Script_ScenDataUtil(data_117E8, 4, Script_ScenDataUtil(data_111F8, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
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
          local_FC = Script_ScenDataUtil(data_117E8, 4, Script_ScenDataUtil(data_111F8, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
          break;
        case 0:
          local_B7 = 1;
          local_F8 = 12;
          local_F0 = 0x4000;
          local_B8 = -1.0f;
          local_FC = Script_ScenDataUtil(data_117E8, 4, Script_ScenDataUtil(data_111F8, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9) | -0x80000000;
          break;
      }
      break;
  }
  *(int32_t *)&local_C4 = Script_ScenDataUtil(data_117E8, 5, Script_ScenDataUtil(data_111F8, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 1);
  *(int32_t *)&local_C0 = Script_ScenDataUtil(data_117E8, 5, Script_ScenDataUtil(data_111F8, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 2);
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

__attribute__((scn_string_bindings(0, data_12D5E)))
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
            local_A4 = Script_ScenDataUtil(data_117E8, 4, Script_ScenDataUtil(data_111F8, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
            local_C0 = 1.0f;
            local_A0 = 18;
            local_9C = 18;
            local_90 = 15473;
            local_8C = 17294;
            break;
          case 0:
            local_CF = 0;
            local_A4 = Script_ScenDataUtil(data_117E8, 4, Script_ScenDataUtil(data_111F8, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9) | -0x80000000;
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
            local_A4 = Script_ScenDataUtil(data_117E8, 4, Script_ScenDataUtil(data_111F8, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
            local_C0 = 1.0f;
            local_A0 = 18;
            local_9C = 18;
            local_90 = 13653;
            break;
          case 0:
            local_CF = 0;
            local_A4 = Script_ScenDataUtil(data_117E8, 4, Script_ScenDataUtil(data_111F8, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
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
            local_A4 = Script_ScenDataUtil(data_117E8, 4, Script_ScenDataUtil(data_111F8, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9) | -0x80000000;
            local_C0 = 1.0f;
            local_A0 = 12;
            local_9C = 18;
            local_90 = 50062;
            local_8C = 48241;
            break;
          case 0:
            local_CF = 1;
            local_A4 = Script_ScenDataUtil(data_117E8, 4, Script_ScenDataUtil(data_111F8, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
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
            local_A4 = Script_ScenDataUtil(data_117E8, 4, Script_ScenDataUtil(data_111F8, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
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
            local_A4 = Script_ScenDataUtil(data_117E8, 4, Script_ScenDataUtil(data_111F8, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
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
            local_A4 = Script_ScenDataUtil(data_117E8, 4, Script_ScenDataUtil(data_111F8, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9) | -0x80000000;
            local_C0 = -1.0f;
            local_A0 = 12;
            local_9C = 12;
            local_90 = 50062;
            local_8C = 48241;
            break;
          case 0:
            local_CF = 0;
            local_A4 = Script_ScenDataUtil(data_117E8, 4, Script_ScenDataUtil(data_111F8, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
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
            local_A4 = Script_ScenDataUtil(data_117E8, 4, Script_ScenDataUtil(data_111F8, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
            local_C0 = -1.0f;
            local_A0 = 18;
            local_9C = 18;
            local_90 = 51882;
            break;
          case 0:
            local_CF = 0;
            local_A4 = Script_ScenDataUtil(data_117E8, 4, Script_ScenDataUtil(data_111F8, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
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
            local_A4 = Script_ScenDataUtil(data_117E8, 4, Script_ScenDataUtil(data_111F8, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
            local_C0 = -1.0f;
            local_A0 = 18;
            local_9C = 12;
            local_90 = 15473;
            local_8C = 17294;
            break;
          case 0:
            local_CF = 1;
            local_A4 = Script_ScenDataUtil(data_117E8, 4, Script_ScenDataUtil(data_111F8, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9) | -0x80000000;
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
            local_A4 = Script_ScenDataUtil(data_117E8, 4, Script_ScenDataUtil(data_111F8, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
            local_C0 = -1.0f;
            local_A0 = 18;
            local_9C = 18;
            local_90 = 13653;
            break;
          case 0:
            local_CF = 1;
            local_A4 = Script_ScenDataUtil(data_117E8, 4, Script_ScenDataUtil(data_111F8, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
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
              local_A4 = Script_ScenDataUtil(data_117E8, 4, Script_ScenDataUtil(data_111F8, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
              local_C0 = -1.0f;
              local_A0 = 18;
              local_9C = 12;
              local_90 = 48241;
              local_8C = 50062;
              break;
            case 1:
              local_CF = 1;
              local_A4 = Script_ScenDataUtil(data_117E8, 4, Script_ScenDataUtil(data_111F8, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9) | -0x80000000;
              local_C0 = -1.0f;
              local_A0 = 12;
              local_9C = 12;
              local_90 = 50062;
              local_8C = 48241;
              break;
            case 2:
              local_CF = 1;
              local_A4 = Script_ScenDataUtil(data_117E8, 4, Script_ScenDataUtil(data_111F8, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
              local_C0 = 1.0f;
              local_A0 = 18;
              local_9C = 18;
              local_90 = 15473;
              local_8C = 17294;
              break;
            case 3:
              local_CF = 0;
              local_A4 = Script_ScenDataUtil(data_117E8, 4, Script_ScenDataUtil(data_111F8, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9) | -0x80000000;
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
              local_A4 = Script_ScenDataUtil(data_117E8, 4, Script_ScenDataUtil(data_111F8, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
              local_C0 = -1.0f;
              local_A0 = 18;
              local_9C = 18;
              local_90 = 45511;
              break;
            case 1:
              local_CF = 1;
              local_A4 = Script_ScenDataUtil(data_117E8, 4, Script_ScenDataUtil(data_111F8, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
              local_C0 = -1.0f;
              local_A0 = 18;
              local_9C = 18;
              local_90 = 51882;
              break;
            case 2:
              local_CF = 1;
              local_A4 = Script_ScenDataUtil(data_117E8, 4, Script_ScenDataUtil(data_111F8, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
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
              local_A4 = Script_ScenDataUtil(data_117E8, 4, Script_ScenDataUtil(data_111F8, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
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
  *(int32_t *)&local_B8 = Script_ScenDataUtil(data_117E8, 5, Script_ScenDataUtil(data_111F8, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 1);
  *(int32_t *)&local_B4 = Script_ScenDataUtil(data_117E8, 5, Script_ScenDataUtil(data_111F8, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 2);
  *(int32_t *)&local_B0 = Script_ScenDataUtil(data_117E8, 5, Script_ScenDataUtil(data_111F8, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 3);
  *(int32_t *)&local_AC = Script_ScenDataUtil(data_117E8, 5, Script_ScenDataUtil(data_111F8, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 4);
  *(int32_t *)&local_A8 = Script_ScenDataUtil(data_117E8, 5, Script_ScenDataUtil(data_111F8, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 7);
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
          *(int32_t *)&local_BC = Script_ScenDataUtil(data_117E8, 5, Script_ScenDataUtil(data_111F8, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 8);
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

__attribute__((scn_string_bindings(0, data_12D7A)))
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

__attribute__((scn_string_bindings(0, data_12D96)))
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
      local_8C = Script_ScenDataUtil(data_117E8, 4, Script_ScenDataUtil(data_111F8, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
      local_A0 = -1.0f;
      local_88 = 18;
      local_84 = 18;
      local_FF78 = 46421;
      local_FF74 = 48787;
      break;
    case 1:
      local_AF = 1;
      local_8C = Script_ScenDataUtil(data_117E8, 4, Script_ScenDataUtil(data_111F8, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9) | -0x80000000;
      local_A0 = -1.0f;
      local_88 = 12;
      local_84 = 12;
      local_FF78 = 50426;
      local_FF74 = 48969;
      break;
    case 2:
      local_AF = 1;
      local_8C = Script_ScenDataUtil(data_117E8, 4, Script_ScenDataUtil(data_111F8, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
      local_A0 = 1.0f;
      local_88 = 18;
      local_84 = 18;
      local_FF78 = 15109;
      local_FF74 = 16566;
      break;
    case 3:
      local_AF = 0;
      local_8C = Script_ScenDataUtil(data_117E8, 4, Script_ScenDataUtil(data_111F8, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9) | -0x80000000;
      local_A0 = 1.0f;
      local_88 = 12;
      local_84 = 12;
      local_FF78 = 19114;
      local_FF74 = 16748;
      break;
  }
  (void)local_FF78;
  *(int32_t *)&local_9C = Script_ScenDataUtil(data_117E8, 5, Script_ScenDataUtil(data_111F8, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 1);
  *(int32_t *)&local_98 = Script_ScenDataUtil(data_117E8, 5, Script_ScenDataUtil(data_111F8, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 2);
  *(int32_t *)&local_94 = Script_ScenDataUtil(data_117E8, 5, Script_ScenDataUtil(data_111F8, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 3);
  *(int32_t *)&local_90 = Script_ScenDataUtil(data_117E8, 5, Script_ScenDataUtil(data_111F8, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 4);
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

__attribute__((scn_string_bindings(0, data_12DB2)))
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
        local_E0 = Script_ScenDataUtil(&data_12208, 4, local_D4 * 5) | -0x80000000;
        break;
      case 0:
        local_E0 = Script_ScenDataUtil(&data_12208, 4, local_D4 * 5);
        break;
    }
  } else if ((local_CC == 2) | (local_CC == 4) | (local_CC == 6) | (local_CC == 8) | (local_CC == 10) | (local_CC == 19) | (local_CC == 22) | (local_CC == 24) | (local_CC == 26) | (local_CC == 28)) {
    local_CC = arg4;
    switch (local_CC) {
      case 1:
        local_E0 = Script_ScenDataUtil(&data_12208, 4, local_D4 * 5);
        break;
      case 0:
        local_E0 = Script_ScenDataUtil(&data_12208, 4, local_D4 * 5) | -0x80000000;
        break;
    }
  } else {
    if ((local_CC == 13) | (local_CC == 14) | (local_CC == 15) | (local_CC == 16) | (local_CC == 20) | (local_CC == 23) | (local_CC == 29)) {
      local_CC = arg4;
      switch (local_CC) {
        case 0:
          local_E0 = Script_ScenDataUtil(&data_12208, 4, local_D4 * 5);
          break;
        case 1:
          local_E0 = Script_ScenDataUtil(&data_12208, 4, local_D4 * 5) | -0x80000000;
          break;
        case 2:
          local_E0 = Script_ScenDataUtil(&data_12208, 4, local_D4 * 5);
          break;
        case 3:
          local_E0 = Script_ScenDataUtil(&data_12208, 4, local_D4 * 5) | -0x80000000;
          break;
      }
      goto L006988;
    }
  }
  L006988:;
  *(int32_t *)&local_EC = Script_ScenDataUtil(&data_12208, 5, local_D4 * 5 + 1);
  *(int32_t *)&local_E8 = Script_ScenDataUtil(&data_12208, 5, local_D4 * 5 + 2);
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

__attribute__((scn_string_bindings(0, data_12DCE)))
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
  DSWK_BindTablesAndInitState(arg1, arg2, arg3, arg4, &task_10, task_20, task_48, (shen_name *)task_70, task_98, arg0, arg5, data_111F8, data_117E8, (shen_name *)task_E8);
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

__attribute__((scn_string_bindings(0, data_12E59, 1, data_12E72)))
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

__attribute__((scn_string_bindings(0, data_12E8B)))
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

__attribute__((scn_string_bindings(0, data_12E9A)))
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

__attribute__((scn_string_bindings(0, data_12EA9, 1, data_12EBB)))
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
      task_110[arg0] = Script_scenCreateTask(Script_ScenDataUtil(&data_122A8, 8, local_F8), 4, task_70[arg0], task_E8[arg0], arg0, local_F8);
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
    task_110[arg0] = Script_scenCreateTask(Script_ScenDataUtil(&data_122A8, 8, arg2), 4, task_70[arg0], task_E8[arg0], arg0, arg2);
    task_C0[arg0] = task_70[arg0];
    script_8A20(arg0);
  }
  return;
}

__attribute__((scn_string_bindings(0, data_12ECE, 1, data_12EE0)))
static void script_9D79(int32_t arg0, int32_t arg1) {
  if ((task_110[arg0] == 0) & (arg1 >= 0)) {
    script_931B(task_70[arg0], task_E8[arg0], task_48[arg0], arg1);
    task_110[arg0] = Script_scenCreateTask(Script_ScenDataUtil(&data_122A8, 8, arg1), 4, task_70[arg0], task_E8[arg0], arg0, arg1);
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

__attribute__((scn_string_bindings(0, data_12F80)))
static void script_AB36(void) {
  Script_sprintf("TUNED FreeTalk def\012");
  SCNT_Enqueue();
  return;
}

__attribute__((scn_string_bindings(0, data_12F94)))
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
  (void)((task_1C8 = 0) & 0x20);
  return;
}

static void script_AB6D(void) {
  task_1BC = Script_freeTalkGetCurrentChara();
  return;
}

static void script_AB79(void) {
  *(&task_1BC + 1) = Script_GetFreeTalkStatus();
  return;
}

static int32_t script_AB88(void) {
  return *(&task_1BC + 2) = Script_freeTalkGetAndResetState();
}

static void script_AB97(int32_t arg0) {
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

static int32_t script_ABC3(int32_t arg0, int32_t arg1) {
  int8_t local_FF;
  int32_t local_F8;
  (void)((local_FF = 0) & 4);
  local_F8 = Script_shenGetStats(STATS_CASH);
  if (local_F8 - arg1 >= arg0) {
    (void)((local_FF = -1) & 4);
  }
  return local_FF & 0x80;
}

static int8_t script_ABF6(int32_t arg0, int32_t arg1, int32_t arg2) {
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
      goto L00AC64;
    }
  }
  L00AC64:;
  local_F8 = local_F8 + local_F4;
  if ((Script_StepUtils(GET_STEP_HIGHER, 0) == 17) & (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_D000_VENDING_LOCK_CLEARED) == 0)) {
    local_F8 = local_F8 + 69000;
    (void)((local_F3 = 0) & 1);
  } else if ((local_EC >= 2202) & (local_EC <= 2204)) {
    (void)((local_F3 = 0) & 1);
    local_F8 = local_F4 = 0;
    local_FF = 4;
  } else {
    (void)((local_F3 = -(script_ABC3(local_F8, arg1) != 0)) & 0x10);
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
      script_AD38(local_FF, arg0);
    }
  }
  return local_FF;
}

__attribute__((scn_string_bindings(0, data_13870, 8, data_138B4, 9, data_138BD, 10, data_138C6, 11, data_138CF, 12, data_138D8, 13, data_138E1)))
static void script_AD38(int32_t arg0, int32_t arg1) {
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

__attribute__((scn_string_bindings(0, data_138E6, 1, data_138EB, 2, data_138F4, 3, data_138F9, 4, data_138FE, 5, data_13907, 6, data_13910, 7, data_13919)))
static void script_AF01(int32_t arg0, int32_t arg1) {
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

static void script_B0D7(int32_t arg0) {
  task_204 = arg0;
  return;
}

static void script_B0E1(void) {
  task_1FC = 0;
  Script_srchUtils(SRCH_SCRIPT_InitEntryTableFromTaskOffset, data_12FA8);
  task_200 = 1;
  task_204 = 0;
  return;
}

static void script_B105(void) {
  int32_t local_FC;
  int16_t local_FA;
  int32_t local_F4;
  task_1F8 = -1;
  task_1F8 = Script_srchUtils(-1);
  if (Script_srchUtils(SRCH_SCRIPT_GetCurrentTargetEntryIndex) >= 0) {
    Script_scenUtils(SCENARIO_CMD_INIT_SCNT);
  } else {
    Script_scenUtils(SCENARIO_CMD_UPDATE_TIMER);
  }
  if (task_1F8 != -1) {
    Script_scenToggleVisibleFor(SHEN_NAME('YKUR'), 1);
    Script_scenToggleVisibleFor(SHEN_NAME('YKUL'), 1);
    Script_ryoControl(PCCTL_DisablePlayerControl);
    local_FC = script_B7F0(task_1F8);
    local_FA = Script_ScenDataUtil(data_12FA8, 2, local_FC * 16 + 1);
    local_F4 = local_FA;
    if ((local_F4 == 0) | (local_F4 == 0) | (local_F4 == -1)) {
      script_BEE2(task_1F8, 0);
    } else if (local_F4 == 9) {
      Script_InvestigationUtils(ShenmueUtils_BeginInteractionMode, 0, 60);
      for (;;) {
        if (Script_InvestigationUtils(ShenmueUtils_PollInteractionModeDone) != 0) {
          break;
        }
        SCNF_WaitFrames_0000(1);
      }
      script_B23C(local_FC);
      Script_InvestigationUtils(ShenmueUtils_RequestInteractionModeExit);
      for (;;) {
        if (Script_InvestigationUtils(ShenmueUtils_PollInteractionModeDone) != 0) {
          break;
        }
        SCNF_WaitFrames_0000(1);
      }
    } else {
      script_B23C(local_FC);
    }
    Script_ryoControl(RYO_RestoreControl);
    Script_scenToggleVisibleFor(SHEN_NAME('YKUR'), 0);
    Script_scenToggleVisibleFor(SHEN_NAME('YKUL'), 0);
  }
  return;
}

static void script_B23C(int32_t arg0) {
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
  local_FC = Script_ScenDataUtil(data_12FA8, 4, arg0 * 8 + 1);
  local_D8 = 0;
  local_E4 = 0;
  for (;;) {
    if (local_FC == Script_ScenDataUtil(&data_13764, 4, local_E4 * 2)) {
      local_D8 = Script_ScenDataUtil(&data_13764, 4, local_E4 * 2 + 1);
      break;
    }
    if (Script_ScenDataUtil(&data_13764, 4, local_E4 * 2) == -1) {
      break;
    }
    local_E4 = local_E4 + 1;
  }
  if (0) {
    local_CF = script_ABF6(2, local_D8, 0);
  } else {
    local_CF = script_ABF6(0, local_D8, 0);
  }
  local_FC = Script_ScenDataUtil(data_12FA8, 4, arg0 * 8 + 1);
  local_E4 = 0;
  if (Script_ScenDataUtil(data_136AC, 4, local_E4) != -1) {
    local_F4 = Script_ScenDataUtil(data_136AC, 4, local_E4);
    local_F0 = Script_ScenDataUtil(data_136AC, 4, local_E4 + 1);
    local_EC = Script_ScenDataUtil(data_136AC, 4, local_E4 + 2);
    local_E8 = Script_ScenDataUtil(data_136AC, 4, local_E4 + 3);
    local_E4 = 4;
    for (;;) {
      if ((Script_ScenDataUtil(data_136AC, 4, local_E4) == -1) & (Script_ScenDataUtil(data_136AC, 4, local_E4 + 1) == -1)) {
        local_F4 = 0;
        local_F0 = 0;
        local_EC = 0;
        local_E8 = 0;
        break;
      }
      if (Script_ScenDataUtil(data_136AC, 4, local_E4) == -1) {
        local_E4 = local_E4 + 1;
        local_F4 = Script_ScenDataUtil(data_136AC, 4, local_E4);
        local_F0 = Script_ScenDataUtil(data_136AC, 4, local_E4 + 1);
        local_EC = Script_ScenDataUtil(data_136AC, 4, local_E4 + 2);
        local_E8 = Script_ScenDataUtil(data_136AC, 4, local_E4 + 3);
        local_E4 = local_E4 + 4;
      }
      if (local_FC == Script_ScenDataUtil(data_136AC, 4, local_E4)) {
        break;
      }
      local_E4 = local_E4 + 1;
    }
  }
  Script_InvestigationUtils(ShenmueUtils_QueueSndCommand, local_F4, local_F0);
  if (Script_ScenDataUtil(data_12FA8, 4, arg0 * 8 + 7) == 0) {
    Script_InvestigationUtils(ShenmueUtils_BeginTargetModeDefault, local_FC);
  } else {
    Script_InvestigationUtils(ShenmueUtils_BeginTargetModeByType, local_FC, Script_ScenDataUtil(data_12FA8, 4, arg0 * 8 + 7) - 1);
  }
  for (;;) {
    SCNF_WaitFrames_0000(1);
    if (Script_InvestigationUtils(ShenmueUtils_UpdateStateMachine) != 0) {
      break;
    }
  }
  if (0) {
    Script_scenCreateTask(script_BEE2, 2, task_1F8, 5);
  } else if (local_CF == 0) {
    Script_scenCreateTask(script_BEE2, 2, task_1F8, 5);
  } else {
    Script_scenCreateTask(script_BEE2, 2, task_1F8, 0);
  }
  local_D0 = 0;
  for (;;) {
    if (task_204 < local_D0) {
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
    local_E4 = script_B7F0(task_1F8);
    local_F8 = Script_ScenDataUtil(data_12FA8, 4, local_E4 * 8 + 6);
    if (local_CF == 1) {
      if (-1) {
        if (0) {
          script_AD38(local_CF, 2);
        } else {
          script_BEE2(task_1F8, 2);
        }
      }
      Script_InvestigationUtils(ShenmueUtils_QueueSndCommand, local_EC, local_E8);
      Script_InvestigationUtils(ShenmueUtils_RequestTransitionVariantA);
      if (0) {
        script_BEE2(task_1F8, 2);
      } else {
        script_BEE2(task_1F8, 0);
      }
    } else if ((local_CF == 2) | (local_CF == 3) | (local_CF == 4)) {
      if (-1) {
        if (0) {
          script_AD38(local_CF, 2);
        } else {
          script_AD38(local_CF, 0);
        }
      }
      Script_InvestigationUtils(ShenmueUtils_QueueSndCommand, local_EC, local_E8);
      Script_InvestigationUtils(ShenmueUtils_RequestTransitionVariantA);
      if (0) {
        script_AD38(local_CF, 0);
      } else {
        script_BEE2(task_1F8, 0);
      }
    } else if ((local_F8 == 0) | (task_200 == 0)) {
      if (task_200 == 0) {
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
          script_BEE2(task_1F8, 1);
        } else {
          Script_InvestigationUtils(ShenmueUtils_QueueSndCommand, local_EC, local_E8);
          Script_InvestigationUtils(ShenmueUtils_RequestTransitionVariantA);
          script_BEE2(task_1F8, 2);
        }
      } else {
        Script_InvestigationUtils(ShenmueUtils_QueueSndCommand, local_EC, local_E8);
        Script_InvestigationUtils(ShenmueUtils_RequestTransitionVariantA);
        script_BEE2(task_1F8, 3);
      }
    }
    if (local_F8 == 0) {
      script_BEE2(task_1F8, 4);
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
      script_C256(local_D8);
      Script_charaSetRenderOpacity(CHARA_AKIR, 1.0f);
      Script_SetGameStateTo(AUTH_None);
      Script_ryoControl(RYO_RestoreControl);
    }
  }
  return;
}

static int32_t script_B7F0(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  int16_t local_F6;
  local_F8 = 0;
  for (;;) {
    local_F6 = Script_ScenDataUtil(data_12FA8, 4, local_F8 * 8);
    if (local_F6 < 0) {
      break;
    }
    if (Script_ScenDataUtil(data_12FA8, 4, local_F8 * 8 + 1) == arg0) {
      local_FC = local_F8;
      break;
    }
    local_F8 = local_F8 + 1;
  }
  return local_FC;
}

static void script_B84F(int32_t arg0) {
  int32_t local_FC;
  if (arg0 != -1) {
    task_208 = arg0;
  }
  local_FC = task_208;
  if (local_FC == 0) {
    Script_gachaponUtils(0, data_13A9C);
  } else if (local_FC == 1) {
    Script_gachaponUtils(0, data_13B45);
  } else if (local_FC == 2) {
    Script_gachaponUtils(0, data_13C80);
  } else if (local_FC == 3) {
    Script_gachaponUtils(0, data_13DB5);
  } else if (local_FC == 4) {
    Script_gachaponUtils(0, data_13E5C);
  } else if (local_FC == 5) {
    Script_gachaponUtils(0, data_13FC9);
  } else if (local_FC == 6) {
    Script_gachaponUtils(0, data_141C2);
  } else if (local_FC == 7) {
    Script_gachaponUtils(0, data_14295);
  } else if (local_FC == 8) {
    Script_gachaponUtils(0, data_14306);
  } else if (local_FC == 9) {
    Script_gachaponUtils(0, data_14353);
  } else if (local_FC == 10) {
    Script_gachaponUtils(0, data_1438C);
  } else if (local_FC == 11) {
    Script_gachaponUtils(0, data_1447B);
  } else if (local_FC == 12) {
    Script_gachaponUtils(0, data_144DE);
  } else if (local_FC == 13) {
    Script_gachaponUtils(0, data_14533);
  } else if (local_FC == 14) {
    Script_gachaponUtils(0, data_1457A);
  } else if (local_FC == 15) {
    Script_gachaponUtils(0, data_145B3);
  } else {
    task_208 = -1;
  }
  return;
}

static int16_t script_B9F0(int8_t arg0, int8_t arg1) {
  int16_t local_FE;
  int8_t local_FD;
  int32_t local_F8;
  local_FE = -1;
  if (arg0 == -1) {
    local_FD = task_208;
  } else {
    local_FD = arg0;
  }
  local_F8 = local_FD;
  switch (local_F8) {
    case 0:
      local_FE = *(int16_t *)((int8_t *)&data_13940 + (arg1 << 1));
      break;
    case 1:
      local_FE = *(int16_t *)((int8_t *)&data_13958 + (arg1 << 1));
      break;
    case 2:
      local_FE = *(int16_t *)((int8_t *)&data_1397C + (arg1 << 1));
      break;
    case 3:
      local_FE = *(int16_t *)((int8_t *)&data_139A8 + (arg1 << 1));
      break;
    case 4:
      local_FE = *(int16_t *)((int8_t *)&data_139BC + (arg1 << 1));
      break;
    case 5:
      local_FE = *(int16_t *)((int8_t *)&data_139E4 + (arg1 << 1));
      break;
    case 6:
      local_FE = *(int16_t *)((int8_t *)&data_13A08 + (arg1 << 1));
      break;
    case 7:
      local_FE = *(int16_t *)((int8_t *)&data_13A20 + (arg1 << 1));
      break;
    case 8:
      local_FE = *(int16_t *)((int8_t *)&data_13A30 + (arg1 << 1));
      break;
    case 9:
      local_FE = *(int16_t *)((int8_t *)&data_13A38 + (arg1 << 1));
      break;
    case 10:
      local_FE = *(int16_t *)((int8_t *)&data_13A40 + (arg1 << 1));
      break;
    case 11:
      local_FE = *(int16_t *)((int8_t *)&data_13A64 + (arg1 << 1));
      break;
    case 12:
      local_FE = *(int16_t *)((int8_t *)&data_13A74 + (arg1 << 1));
      break;
    case 13:
      local_FE = *(int16_t *)((int8_t *)&data_13A80 + (arg1 << 1));
      break;
    case 14:
      local_FE = *(int16_t *)((int8_t *)&data_13A8C + (arg1 << 1));
      break;
    case 15:
      local_FE = *(int16_t *)((int8_t *)&data_13A94 + (arg1 << 1));
      break;
  }
  return local_FE;
}

static void script_BBDB(int32_t arg0, int32_t arg1) {
  for (;;) {
    *(int32_t *)arg0 = Script_FloatUtils(ScriptMathOp_RandomIntBelow, 16);
    if (data_13920[*(int32_t *)arg0] - data_13930[*(int32_t *)arg0]) {
      break;
    }
  }
  *(int32_t *)arg1 = Script_FloatUtils(ScriptMathOp_RandomIntBelow, data_13920[*(int32_t *)arg0] - data_13930[*(int32_t *)arg0]);
  return;
}

static void script_BC2A(int32_t arg0, int32_t arg1) {
  if (arg0 == 0) {
    (void)0;
  } else if (arg0 == 14) {
    (void)0;
  } else if (arg1 < data_13920[arg0] - data_13930[arg0]) {
    (void)1;
  } else if (arg0 == 15) {
    (void)3;
  } else {
    (void)2;
  }
  return;
}

static void script_BC7D(int32_t arg0, int32_t arg1, int32_t arg2, float arg3) {
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
        goto L00BCEE;
      }
      L00BCEE:;
      break;
    case 3:
      local_EC = arg2;
      if (local_EC == 6) {
        (void)((local_F3 = -1) & 1);
        goto L00BD15;
      }
      L00BD15:;
      break;
    case 4:
      local_EC = arg2;
      if ((local_EC == 7) | (local_EC == 15) | (local_EC == 16) | (local_EC == 17) | (local_EC == 18)) {
        (void)((local_F3 = -1) & 1);
        goto L00BD54;
      }
      L00BD54:;
      break;
    case 5:
      (void)((local_F3 = -1) & 1);
      break;
    case 6:
      local_EC = arg2;
      if ((local_EC == 0) | (local_EC == 1) | (local_EC == 2) | (local_EC == 3)) {
        (void)((local_F3 = -1) & 1);
        goto L00BDA2;
      }
      L00BDA2:;
      break;
    case 8:
      local_EC = arg2;
      if (local_EC == 3) {
        (void)((local_F3 = -1) & 1);
        goto L00BDC9;
      }
      L00BDC9:;
      break;
  }
  local_F4.x = (&local_F4)->y = (&local_F4)->z = 1.0f;
  if (local_F3 & 8) {
    arg2 = arg2 + data_13920[arg1];
    local_F4.x = (&local_F4)->x * arg3;
    (&local_F4)->y = (&local_F4)->y * arg3;
    (&local_F4)->z = (&local_F4)->z * arg3;
  } else {
    local_F4.x = (&local_F4)->x * arg3 * 0.33333f;
    (&local_F4)->y = (&local_F4)->y * arg3 * 0.33333f;
    (&local_F4)->z = (&local_F4)->z * arg3 * 0.33333f;
  }
  if (local_F2 & 4) {
    script_B84F(arg1);
    Script_gachaponUtils(5, arg0, arg2);
  } else {
    script_B84F(arg1);
    Script_gachaponUtils(1, arg0, arg2);
  }
  if ((((scn3_bits){.f = (&local_F4)->x}).i != 0x3F800000) & (((scn3_bits){.f = (&local_F4)->x}).i != 0)) {
    Script_hmdlApplyScale(arg0, &local_F4);
  }
  return;
}

static int32_t script_BEB4(void) {
  Script_InvestigationUtils(ShenmueUtils_SetupOrSyncAnchors, SHEN_NAME('YKUL'), 0x52554B59);
  script_B0E1();
  for (;;) {
    script_B105();
    script_FDCD();
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_BEE2(int32_t arg0, int32_t arg1) {
  if (arg1 == 5) {
    int32_t unused_FC;
    int32_t local_F8;
    local_F8 = arg0;
    switch (local_F8) {
      case 0x6B746E63:
        SCNT_ShowSubtitle("[[[UISRCHDCB26]]]");
        break;
      case 0x706E6E63:
        SCNT_ShowSubtitle("[[[UISRCHDCB25]]]");
        break;
      case 0x706D6E63:
        SCNT_ShowSubtitle("[[[UISRCHDCB23]]]");
        break;
      case 0x6B6D6E63:
        SCNT_ShowSubtitle("[[[UISRCHDCB22]]]");
        break;
      case 0x31626E63:
        SCNT_ShowSubtitle("[[[UISRCHDCB18]]]");
        break;
      case 0x33626E63:
        SCNT_ShowSubtitle("[[[UISRCHDCB19]]]");
        break;
      case 0x636D6E63:
        SCNT_ShowSubtitle("[[[UISRCHDCB24]]]");
        break;
      case 0x73726E63:
        SCNT_ShowSubtitle("[[[UISRCHDCB20]]]");
        break;
      case 0x31636E63:
        SCNT_ShowSubtitle("[[[UISRCHDCB27]]]");
        break;
      case 0x32636E63:
        SCNT_ShowSubtitle("[[[UISRCHDCB28]]]");
        break;
      case 0x33636E63:
        SCNT_ShowSubtitle("[[[UISRCHDCB29]]]");
        break;
      case 0x34636E63:
        SCNT_ShowSubtitle("[[[UISRCHDCB30]]]");
        break;
      case 0x35636E63:
        SCNT_ShowSubtitle("[[[UISRCHDCB31]]]");
        break;
      case 0x36636E63:
        SCNT_ShowSubtitle("[[[UISRCHDCB32]]]");
        break;
      case 0x37636E63:
        SCNT_ShowSubtitle("[[[UISRCHDCB33]]]");
        break;
      case 0x38636E63:
        SCNT_ShowSubtitle("[[[UISRCHDCB34]]]");
        break;
      case 0x39636E63:
        SCNT_ShowSubtitle("[[[UISRCHDCB35]]]");
        break;
      case 0x61636E63:
        SCNT_ShowSubtitle("[[[UISRCHDCB36]]]");
        break;
      case 0x62636E63:
        SCNT_ShowSubtitle("[[[UISRCHDCB37]]]");
        break;
      case 0x63636E63:
        SCNT_ShowSubtitle("[[[UISRCHDCB38]]]");
        break;
      case 0x64636E63:
        SCNT_ShowSubtitle("[[[UISRCHDCB39]]]");
        break;
      case 0x65636E63:
        SCNT_ShowSubtitle("[[[UISRCHDCB40]]]");
        break;
      case 0x66636E63:
        SCNT_ShowSubtitle("[[[UISRCHDCB41]]]");
        break;
      case 0x67636E63:
        SCNT_ShowSubtitle("[[[UISRCHDCB42]]]");
        break;
      case 0x68636E63:
        SCNT_ShowSubtitle("[[[UISRCHDCB43]]]");
        break;
      case 0x69636E63:
        SCNT_ShowSubtitle("[[[UISRCHDCB44]]]");
        break;
      case 0x6A636E63:
        SCNT_ShowSubtitle("[[[UISRCHDCB45]]]");
        break;
      case 0x6B636E63:
        SCNT_ShowSubtitle("[[[UISRCHDCB46]]]");
        break;
      case 0x31736E63:
        SCNT_ShowSubtitle("[[[UISRCHDCB14]]]");
        break;
      case 0x63736E63:
        SCNT_ShowSubtitle("[[[UISRCHDCB16]]]");
        break;
      case 0x6B736E63:
        SCNT_ShowSubtitle("[[[UISRCHDCB15]]]");
        break;
      case 0x646B6E63:
        SCNT_ShowSubtitle("[[[UISRCHDCB17]]]");
        break;
      case 0x67646E63:
        SCNT_ShowSubtitle("[[[UISRCHDCB21]]]");
        break;
    }
  }
  if (arg1 == 3) {
    float local_FC;
    local_FC = Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 4.0f);
    if (!(local_FC >= 1.0f)) {
      Script_ShowUIMessageFromID("SA1079A003");
    } else if (!(local_FC >= 2.0f)) {
      Script_ShowUIMessageFromID("SA1079A004");
    } else if (!(local_FC >= 3.0f)) {
      Script_ShowUIMessageFromID("SA1079A005");
    } else {
      Script_ShowUIMessageFromID("SA1079A006");
    }
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
  }
  if (arg1 == 2) {
    float local_FC;
    local_FC = Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f);
    if (!(local_FC >= 1.0f)) {
      Script_ShowUIMessageFromID("SA1079A001");
    } else {
      Script_ShowUIMessageFromID("SA1079A002");
    }
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
  }
  end:;
}

static void script_C256(int32_t arg0) {
  uint8_t unused_EA[22];
  int8_t local_E9;
  script_10F44();
  Script_PlayerStatsUtils(PS_SetCharaItemCount, task_1E4, Script_PlayerStatsUtils(PS_GetCharaItemCount, task_1E4) + 1);
  Script_filmCheckOrSetTransitionUtil(2, 0, 0, 0, 0, 255, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_FaceEx(CHARA_AKIR, 1, 0, 2);
  Script_ResetHandMotion(CHARA_AKIR, -1, 0, 0);
  Script_ResetHandMotion(CHARA_AKIR, -1, 1, 0);
  Script_shenClearFreeState(0);
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_EVT_SetPosition(CHARA_AKIR, 0x78000000, &data_14C10);
  {
    uint8_t unused_E8;
    Vector3i local_DC;
    (&local_DC)->y = 0;
    Script_EVT_SetRotation(CHARA_AKIR, MATRIX_XFORM_Y, &local_DC, 0);
  }
  Script_motmToggleFlags(CHARA_AKIR, 2, 0x40);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  SCNF_WaitFrames_0000(1);
  script_C552(task_1E4);
  Script_ccowUtils(CHARA_AKIR, 2);
  Script_InitializeOsag(CHARA_AKIR);
  Script_ResetHandMotion(CHARA_AKIR, 1, 1, 0);
  Script_ResetHandMotion(CHARA_AKIR, 1, 0, 0);
  task_1D4 = -1;
  task_1F0 = 0;
  local_E9 = script_C672(arg0);
  if (local_E9 == 1) {
    script_CF8F();
    if (task_1D4 != 6) {
      script_D843();
      SCNF_WaitFrames_0000(30);
    } else {
      script_9EB2(0x78626B64, -3.4f, 0.74f, -0.432f);
      script_9EDA(0x78626B64, 0.0f, 0.0f, 0.0f);
      Script_charaDisplay(0x78626B64, CDM_Show);
    }
  }
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, 0x52494B41)) {
    Script_SetCharaLookAtPosition(CHARA_AKIR, -13, 0, 0);
    Script_FaceLookAtTargetUtils(CHARA_AKIR, 0);
  }
  Script_motmToggleFlags(CHARA_AKIR, 2, 0x40);
  {
    uint8_t unused_E8;
    Vector3 local_DC;
    local_DC.x = -2.76f;
    (&local_DC)->z = 0.21f;
    Script_EVT_SetPosition(CHARA_AKIR, 0x28000000, &local_DC);
  }
  {
    uint8_t unused_E8;
    Vector3i local_DC;
    (&local_DC)->y = 0;
    Script_EVT_SetRotation(CHARA_AKIR, MATRIX_XFORM_Y, &local_DC, 0);
  }
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_convSetCamInactiveFlag(1);
  Script_SetGameStateTo(AUTH_None);
  Script_SetCPAMFlags(task_1E4, 1);
  Script_scenToggleVisibleFor(task_1E4, 1);
  Script_CycleManagerUtils(CYCLE_CMD_LOAD_OR_RESET_RUNNING_PROGRAM, task_20C);
  Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT, task_1E4, 0);
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 1);
  Script_ccowUtils(CHARA_AKIR, 1);
  SCNF_WaitFrames_0000(1);
  SCNF_WaitFrames_0000(30);
  SCNF_WaitFrames_0000(1);
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

static void script_C552(int32_t arg0) {
  task_20C = 0;
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
  task_20C = Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT_INTERNAL_HUMAN, arg0);
  if (task_20C == 0) {
    Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT, arg0, 1);
    task_20C = Script_CycleManagerUtils(CYCLE_CMD_GET_CYCLE_PTR_BY_CHARA_ID, arg0);
    while (task_20C == 0) {
      SCNF_WaitFrames_0000(1);
      task_20C = Script_CycleManagerUtils(CYCLE_CMD_GET_CYCLE_PTR_BY_CHARA_ID, arg0);
    }
    L00C5BB:;
    SCNF_WaitFrames_0000(1);
    if (!Script_CycleManagerUtils(CYCLE_CMD_GET_NEXT_TASK_POINTER, task_20C)) goto L00C5BB;
  }
  Script_CycleManagerUtils(CYCLE_CMD_SET_RUNNING_PROGRAM_FIELD_B8, arg0, 0);
  Script_motmToggleFlags(arg0, 2, 0x40);
  script_9EB2(arg0, -2.7f, 0.0f, -0.9f);
  script_9EDA(arg0, 0.0f, -180.0f, 0.0f);
  Script_charaDisplay(arg0, CDM_Show);
  if (arg0 == 0x4F4E5248) {
    Script_PlayCharaMotion(arg0, 28678, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  } else {
    Script_PlayCharaMotion(arg0, 28683, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  }
  Script_InitializeOsag(arg0);
  Script_SetCPAMFlags(arg0, 0);
  return;
}

__attribute__((scn_string_bindings(2, data_14C62, 5, data_14C83)))
static int8_t script_C672(int32_t arg0) {
  int32_t unused_F4[3];
  int32_t local_F0;
  int8_t local_EF;
  uint8_t unused_EC[3];
  int32_t local_E8;
  int32_t local_E4;
  int32_t local_E0;
  int32_t local_DC;
  Script_seqEnterEvtCam(9010, 0, AUTH_None);
  Script_filmCheckOrSetTransitionUtil(15, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  if (task_1B4 == 1) {
    Script_ShowUIMessageFromID("SA1079C007");
  } else {
    Script_ShowUIMessageFromID("SA1079B007");
    Script_motmSetBlendFrames(task_1E4, MOTM_BLEND_FRAMES_20);
    Script_motmToggleFlags(task_1E4, 1, 0x40);
    Script_PlayCharaMotion(task_1E4, 28677, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  }
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  if ((task_1F8 == 0x706E6E63) | (task_1F8 == 0x31736E63)) {
    Script_executeAicaCommand(0x1305A9, 0, 0);
  } else {
    Script_executeAicaCommand(0x1205A9, 0, 0);
  }
  script_E909();
  *(float *)&local_F0 = Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f);
  if (!((float)local_F0 >= 1.0f)) {
    Script_ShowUIMessageFromID("SA1079A007");
  } else if (!((float)local_F0 >= 2.0f)) {
    Script_ShowUIMessageFromID("SA1079A009");
  } else {
    Script_ShowUIMessageFromID("SA1079A011");
  }
  script_9EB2(CHARA_AKIR, -3.16f, 0.094f, 0.173f);
  script_9EDA(CHARA_AKIR, 352.002014f, 355.00119f, 341.004608f);
  Script_motmToggleFlags(CHARA_AKIR, 2, 0x40);
  Script_motmToggleFlags(CHARA_AKIR, 1, 0x20);
  Script_PlayCharaMotion(CHARA_AKIR, 28673, 10.0f, 29.0f, -1.0f, 1.0f, 0);
  Script_seqEnterEvtCam(9030, 0, AUTH_None);
  SCNF_SetCashValue(-arg0, 1);
  Script_motmToggleFlags(CHARA_AKIR, 2, 0x20);
  Script_motmToggleFlags(CHARA_AKIR, 2, 0x40);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_SetHandRootRotation(CHARA_AKIR, 1, 0x15, &data_14C34);
  Script_handSetFloatsFor(CHARA_AKIR, 1, data_145EC, 0);
  Script_handSetFloatsFor(CHARA_AKIR, 0, data_145EC, 0);
  script_9EB2(CHARA_AKIR, -2.76f, 0.0f, 0.21f);
  script_9EDA(CHARA_AKIR, 0.0f, 0.0f, 0.0f);
  Script_InitializeOsag(CHARA_AKIR);
  {
    Vector3 local_D0;
    if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, 0x52494B41) & Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, task_1E4)) {
      Script_EVT_GetPosition(task_1E4, 4, &local_D0, ENTITY_XFORM_WORLD_SPACE);
      Script_SetLookPointDetails(CHARA_AKIR, 20, &local_D0, 2);
      Script_FaceLookAtTargetUtils(CHARA_AKIR, 2, &local_D0, 30);
    }
    if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, task_1E4) & Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, 0x52494B41)) {
      Script_EVT_GetPosition(CHARA_AKIR, 4, &local_D0, ENTITY_XFORM_WORLD_SPACE);
      Script_SetLookPointDetails(task_1E4, 20, &local_D0, 2);
      Script_FaceLookAtTargetUtils(task_1E4, 2, &local_D0, 30);
    }
  }
  local_E8 = script_B7F0(task_1F8);
  local_DC = Script_ScenDataUtil(data_12FA8, 4, local_E8 * 8 + 1);
  local_E0 = Script_ScenDataUtil(data_12FA8, 4, local_E8 * 8 + 6);
  if ((local_DC == 0x31736E63) | (local_DC == 0x63736E63) | (local_DC == 0x6B736E63) | (local_DC == 0x31636E63) | (local_DC == 0x32636E63) | (local_DC == 0x33636E63) | (local_DC == 0x34636E63) | (local_DC == 0x35636E63) | (local_DC == 0x36636E63) | (local_DC == 0x37636E63) | (local_DC == 0x38636E63) | (local_DC == 0x39636E63) | (local_DC == 0x61636E63) | (local_DC == 0x62636E63) | (local_DC == 0x63636E63) | (local_DC == 0x64636E63) | (local_DC == 0x65636E63) | (local_DC == 0x66636E63) | (local_DC == 0x67636E63) | (local_DC == 0x68636E63) | (local_DC == 0x69636E63) | (local_DC == 0x6A636E63) | (local_DC == 0x6B636E63)) {
    local_EF = 1;
  } else {
    local_EF = 0;
  }
  if (task_1D4 == 1) {
    Script_seqEnterEvtCam(9065, CHARA_AKIR, CHARA_AKIR);
  } else {
    Script_seqEnterEvtCam(9060, CHARA_AKIR, CHARA_AKIR);
  }
  {
    int32_t local_D8;
    local_D8 = local_E0;
    switch (local_D8) {
      case 65:
        local_E4 = 0;
        break;
      case 63:
        local_E4 = 1;
        break;
      case 64:
        local_E4 = 2;
        break;
      case 52:
        local_E4 = 5;
        break;
      case 51:
        local_E4 = 6;
        break;
      case 54:
        local_E4 = 7;
        break;
      case 55:
        local_E4 = 8;
        break;
      case 56:
        local_E4 = 9;
        break;
      case 61:
        local_E4 = 10;
        break;
      case 60:
        local_E4 = 11;
        break;
      case 58:
        local_E4 = 12;
        break;
      case 59:
        local_E4 = 13;
        break;
      case 53:
        local_E4 = 46;
        break;
      case 9999:
        local_D8 = task_1F8;
        switch (local_D8) {
          case 0x31636E63:
            local_E4 = 34;
            task_1E8 = 6;
            break;
          case 0x32636E63:
            local_E4 = 35;
            task_1E8 = 5;
            break;
          case 0x33636E63:
            local_E4 = 29;
            task_1E8 = 9;
            break;
          case 0x34636E63:
            local_E4 = 16;
            task_1E8 = 0;
            break;
          case 0x35636E63:
            local_E4 = 15;
            task_1E8 = 2;
            break;
          case 0x36636E63:
            local_E4 = 14;
            task_1E8 = 4;
            break;
          case 0x37636E63:
            local_E4 = 17;
            task_1E8 = 3;
            break;
          case 0x38636E63:
            local_E4 = 30;
            task_1E8 = 1;
            break;
          case 0x39636E63:
            local_E4 = 36;
            task_1E8 = 7;
            break;
          case 0x61636E63:
            local_E4 = 37;
            task_1E8 = 8;
            break;
          case 0x62636E63:
            local_E4 = 23;
            task_1E8 = 14;
            break;
          case 0x63636E63:
            local_E4 = 38;
            task_1E8 = 15;
            break;
          case 0x64636E63:
            local_E4 = 33;
            task_1E8 = 16;
            break;
          case 0x65636E63:
            local_E4 = 39;
            task_1E8 = 17;
            break;
          case 0x66636E63:
            local_E4 = 24;
            task_1E8 = 18;
            break;
          case 0x67636E63:
            local_E4 = 26;
            task_1E8 = 19;
            break;
          case 0x68636E63:
            local_E4 = 31;
            task_1E8 = 20;
            break;
          case 0x69636E63:
            local_E4 = 27;
            task_1E8 = 21;
            break;
          case 0x6A636E63:
            local_E4 = 28;
            task_1E8 = 22;
            break;
          case 0x6B636E63:
            local_E4 = 32;
            task_1E8 = 23;
            break;
        }
        break;
    }
  }
  Script_PlayerStatsUtils(PS_SetSparsePairU8Value, 1, local_E4, Script_PlayerStatsUtils(PS_GetSparsePairU8Value, 1, local_E4) + 1);
  script_DF9E();
  if (local_EF == 1) {
    Script_seqEnterEvtCam(5020, 0, AUTH_None);
  } else {
    Script_seqEnterEvtCam(9590, 0, AUTH_None);
  }
  Script_FaceEx(CHARA_AKIR, 1, 1, 240);
  Script_motmToggleFlags(CHARA_AKIR, 1, 0x20);
  Script_PlayCharaMotion(CHARA_AKIR, 28674, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_motmSetBlendFrames(CHARA_AKIR, MOTM_BLEND_FRAMES_20);
  Script_motmToggleFlags(CHARA_AKIR, 1, 0x40);
  if (local_E0 == 9999) {
    Script_GetCassetteFlags(task_1E8, 1, 0, 1);
    SCNF_StartIconSystemMenu(task_1E8 + 255);
  } else {
    switch (local_E0) {
      case 54:
        Script_GetCassetteFlags(local_E0, 5, 0, 0);
        break;
      case 55:
        Script_GetCassetteFlags(local_E0, 3, 0, 0);
        break;
      case 60:
        Script_GetCassetteFlags(local_E0, 3, 0, 0);
        break;
    }
    SCNF_StartIconSystemMenu(local_E0);
  }
  if (local_EF == 0) {
    script_1061E();
  }
  Script_FaceEx(CHARA_AKIR, 1, 0, 4);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_motmToggleFlags(CHARA_AKIR, 2, 0x20);
  SCNF_WaitFrames_0000(1);
  script_DF9E();
  if (local_EF == 1) {
    if (task_1B4 == 1) {
      Script_ShowUIMessageFromID("SA1079C013");
    } else {
      Script_ShowUIMessageFromID(data_14C8E);
    }
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
  }
  return local_EF;
}

static void script_CF8F(void) {
  int32_t local_FC;
  int32_t unused_F8;
  Vector3 local_EC;
  int32_t unused_E0[3];
  Vector3i local_D4;
  int32_t local_D0;
  int32_t local_CC;
  Vector3 local_C0;
  Script_filmCheckOrSetTransitionUtil(1, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_motmToggleFlags(CHARA_AKIR, 2, 0x40);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_ccowUtils(CHARA_AKIR, 2);
  Script_InitializeOsag(CHARA_AKIR);
  Script_ResetHandMotion(CHARA_AKIR, 1, 1, 0);
  Script_ResetHandMotion(CHARA_AKIR, 1, 0, 0);
  Script_handSetFloatsFor(CHARA_AKIR, 1, data_145EC, 10);
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, 0x52494B41)) {
    Script_SetCharaLookAtPosition(CHARA_AKIR, -13, 0, 0);
    Script_FaceLookAtTargetUtils(CHARA_AKIR, 0);
  }
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, task_1E4)) {
    Script_SetCharaLookAtPosition(task_1E4, -13, 0, 0);
    Script_FaceLookAtTargetUtils(task_1E4, 0);
  }
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, 0x52494B41) & Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, task_1E4)) {
    Script_EVT_GetPosition(task_1E4, 4, &local_C0, ENTITY_XFORM_WORLD_SPACE);
    Script_SetLookPointDetails(CHARA_AKIR, 16, &local_C0, 2);
    Script_FaceLookAtTargetUtils(CHARA_AKIR, 2, &local_C0, 30);
  }
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, task_1E4) & Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, 0x52494B41)) {
    Script_EVT_GetPosition(CHARA_AKIR, 4, &local_C0, ENTITY_XFORM_WORLD_SPACE);
    Script_SetLookPointDetails(task_1E4, 16, &local_C0, 2);
    Script_FaceLookAtTargetUtils(task_1E4, 2, &local_C0, 30);
  }
  script_9EB2(0x78626B64, -2.713f, 0.74f, -0.328f);
  script_9EDA(0x78626B64, 0.0f, 0.0f, 0.0f);
  Script_charaDisplay(0x78626B64, CDM_Show);
  Script_EVT_GetPosition(0x78626B64, -1, &local_EC, 0);
  Script_SetCharaLookAtPosition(CHARA_AKIR, 0x10000, &local_EC, 0);
  Script_FaceLookAtTargetUtils(CHARA_AKIR, 2, &local_EC, 1);
  script_9EB2(CHARA_AKIR, -2.8392f, 0.0f, 0.13f);
  script_9EDA(CHARA_AKIR, 0.0f, 0.0f, 0.0f);
  Script_ResetHandMotion(CHARA_AKIR, 1, 0, 0);
  Script_ResetHandMotion(CHARA_AKIR, 1, 1, 0);
  Script_handSetFloatsFor(CHARA_AKIR, 1, data_145EC, 10);
  Script_motmToggleFlags(CHARA_AKIR, 2, 0x40);
  Script_PlayCharaMotion(CHARA_AKIR, 28681, 70.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_SetCharaMotionPlaybackRate(CHARA_AKIR, 1.0f);
  Script_seqEnterEvtCam(9100, CHARA_AKIR, CHARA_AKIR);
  L00D22B:;
  local_FC = Script_FloatUtils(ScriptMathOp_RandomIntBelow, 1000);
  if ((task_1F0 == 0) & (task_1B8 == 0)) {
    if (local_FC <= 9) {
      task_1D4 = 1;
      goto L00D325;
    }
    if (local_FC <= 28) {
      task_1D4 = 2;
      goto L00D325;
    }
    if (local_FC <= 60) {
      task_1D4 = 3;
      goto L00D325;
    }
    if (local_FC <= 121) {
      task_1D4 = 4;
      goto L00D325;
    }
    if (local_FC <= 245) {
      task_1D4 = 5;
      goto L00D325;
    }
    task_1D4 = 6;
    goto L00D325;
  } else {
    if (local_FC <= 19) {
      task_1D4 = 1;
      goto L00D325;
    }
    if (local_FC <= 58) {
      task_1D4 = 2;
      goto L00D325;
    }
    if (local_FC <= 123) {
      task_1D4 = 3;
      goto L00D325;
    }
    if (local_FC <= 246) {
      task_1D4 = 4;
      goto L00D325;
    }
    if (local_FC <= 495) {
      task_1D4 = 5;
      goto L00D325;
    }
    task_1D4 = 6;
    goto L00D325;
  }
  goto L00D22B;
  L00D325:;
  if (task_1D4 == 6) {
    task_1B0 = task_1B0 + 1;
    if (task_1B0 > 7) {
      task_1D4 = 5;
      task_1B0 = 0;
    }
  }
  Script_PlayerStatsUtils(PS_SetMatrix4x8U16Slot, 1, task_1D4, Script_PlayerStatsUtils(PS_GetMatrix4x8U16Slot, 1, task_1D4) + 1);
  Script_handSetFloatsFor(CHARA_AKIR, 1, data_145EC, 10);
  Script_handSetFloatsFor(CHARA_AKIR, 0, data_145EC, 10);
  local_D0 = 0;
  for (;;) {
    if (local_D0 == 75) {
      Script_sicnUtils(0, 4, 1);
    }
    if (Script_IsCharaMotionFinished(CHARA_AKIR)) {
      Script_PlayCharaMotion(CHARA_AKIR, 28680, -1.0f, -1.0f, -1.0f, 1.0f, 0);
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
  local_EC.x = -2.808f;
  (&local_EC)->y = 1.0f;
  (&local_EC)->z = -0.3913f;
  Script_SetCharaLookAtPosition(CHARA_AKIR, 0x10000, &local_EC, 0);
  Script_FaceLookAtTargetUtils(CHARA_AKIR, 2, &local_EC, 1);
  Script_seqEnterEvtCam(9105, 0, AUTH_None);
  local_EC.x = 0.118f;
  (&local_EC)->y = 0.02f;
  (&local_EC)->z = -0.062f;
  local_D4.x = 8008;
  (&local_D4)->y = 9827;
  (&local_D4)->z = 5183;
  Script_fixoAttachEx(0x6C636B64, CHARA_AKIR, 18, &local_EC, &local_D4);
  Script_PlayCharaMotion(CHARA_AKIR, 28679, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_SetCharaMotionPlaybackRate(CHARA_AKIR, 1.4f);
  local_CC = 0;
  for (;;) {
    local_CC = local_CC + 2;
    if (local_CC == 100) {
      Script_executeAicaCommand(0x1405A9, 0, 0);
    }
    if (local_CC == 170) {
      Script_fixoClearAttachment(0x6C636B64);
      script_9EB2(0x6C636B64, 0.0f, 0.0f, -1000.0f);
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
      if (task_1D4 == 6) {
        local_EC.x = 0.0f;
        (&local_EC)->y = 0.0f;
        (&local_EC)->z = 0.0f;
        Script_charaPosRotUtils(0x706F6B64, 160, &local_EC, 0, 0);
      } else {
        local_EC.x = 0.0f;
        (&local_EC)->y = 0.0f;
        (&local_EC)->z = 0.0f;
        Script_charaPosRotUtils(0x706F6B64, task_1D4 + 152, &local_EC, 0, 0);
      }
      if ((task_1D4 > 0) & (task_1D4 < 6)) {
        local_EC.x = 0.0f;
        (&local_EC)->y = 0.0f;
        (&local_EC)->z = 0.0f;
        if (task_1D4 == 1) {
          Script_charaPosRotUtils(0x706F6B64, 158, &local_EC, 0, 0);
        } else {
          Script_charaPosRotUtils(0x706F6B64, 159, &local_EC, 0, 0);
        }
      }
      Script_seqEnterEvtCam(9110, CHARA_AKIR, CHARA_AKIR);
      SCNF_WaitFrames_0000(1);
    }
    if (local_CC == 216) {
      script_FCA9();
    }
    if (Script_IsCharaMotionFinished(CHARA_AKIR)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_PlayCharaMotion(CHARA_AKIR, 28682, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  for (;;) {
    if (Script_IsCharaMotionFinished(CHARA_AKIR)) {
      break;
    }
    Script_RotateNodeUtils(CHARA_AKIR, &data_111CC, 0);
    SCNF_WaitFrames_0000(1);
  }
  if (task_1D4 == 5) {
    task_1D0 = script_F74E();
  }
  if (task_1D4 == 6) {
    Script_seqEnterEvtCam(9115, CHARA_AKIR, CHARA_AKIR);
    script_FC2C();
  }
  script_FD19();
  if (task_1D4 == 5) {
    script_EDCA();
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
  if (!(task_1D4 + 1 == 1)) goto L00D7E4;
  L00D7E4:;
  if (task_1D4 == 6) {
    SCNF_WaitFrames_0000(20);
  }
  Script_fixoClearAttachment(0x706F6B64);
  script_9EB2(0x706F6B64, 0.0f, 0.0f, -1000.0f);
  Script_FaceLookAtTargetUtils(CHARA_AKIR, 0);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  return;
}

static void script_D843(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t unused_E4;
  int32_t local_E0;
  int32_t local_DC;
  int32_t local_D8;
  Vector3 local_CC;
  script_9EB2(0x78626B64, -3.4f, 0.74f, -0.432f);
  script_9EDA(0x78626B64, 0.0f, 0.0f, 0.0f);
  Script_charaDisplay(0x78626B64, CDM_Show);
  Script_SetCharaLookAtPosition(CHARA_AKIR, -20, 0, 0);
  local_D8 = task_1D4;
  switch (local_D8) {
    case 1:
      task_1D0 = script_EEF7();
      break;
    case 2:
      task_1D0 = script_EEF7();
      break;
    case 3:
      task_1D0 = script_EEF7();
      break;
    case 4:
      task_1D0 = script_EEF7();
      break;
    case 5:
      local_F4.x = -3.108f;
      (&local_F4)->y = 0.754f;
      (&local_F4)->z = -0.441f;
      Script_EVT_SetPosition(0x74687364, POS_XYZ, &local_F4);
      break;
  }
  local_D8 = task_1D4;
  switch (local_D8) {
    case 1:
      local_F4.x = (&local_F4)->y = (&local_F4)->z = 0.7f;
      Script_hmdlApplyScale(task_1D0, &local_F4);
      local_F4.x = -2.72f;
      (&local_F4)->y = 0.791f;
      (&local_F4)->z = -0.376f;
      local_E8.x = 0;
      (&local_E8)->y = 0xC000;
      (&local_E8)->z = 0;
      Script_EVT_SetPosition(task_1D0, POS_XYZ, &local_F4);
      Script_EVT_SetRotation(task_1D0, 0x38000000, &local_E8, 0);
      Script_charaDisplay(task_1D0, CDM_Show);
      break;
    case 2:
      local_F4.x = -2.73f;
      (&local_F4)->y = 0.75f;
      (&local_F4)->z = -0.46f;
      local_E8.x = 0;
      (&local_E8)->y = 0;
      (&local_E8)->z = 0;
      Script_EVT_SetPosition(task_1D0, POS_XYZ, &local_F4);
      Script_EVT_SetRotation(task_1D0, 0x38000000, &local_E8, 0);
      Script_charaDisplay(task_1D0, CDM_Show);
      break;
    case 3:
      local_F4.x = -2.72f;
      (&local_F4)->y = 0.76f;
      (&local_F4)->z = -0.376f;
      local_E8.x = 0xC000;
      (&local_E8)->y = 0;
      (&local_E8)->z = 0;
      Script_EVT_SetPosition(task_1D0, POS_XYZ, &local_F4);
      Script_EVT_SetRotation(task_1D0, 0x38000000, &local_E8, 0);
      Script_charaDisplay(task_1D0, CDM_Show);
      break;
    case 4:
      local_F4.x = -2.72f;
      (&local_F4)->y = 0.773f;
      (&local_F4)->z = -0.376f;
      local_E8.x = 0x8000;
      (&local_E8)->y = 0x8000;
      (&local_E8)->z = 0x8000;
      Script_EVT_SetPosition(task_1D0, POS_XYZ, &local_F4);
      Script_EVT_SetRotation(task_1D0, 0x38000000, &local_E8, 0);
      Script_charaDisplay(task_1D0, CDM_Show);
      break;
    case 5:
      local_F4.x = -2.72f;
      (&local_F4)->y = 0.7886f;
      (&local_F4)->z = -0.376f;
      local_E8.x = 0x8000;
      (&local_E8)->y = 0xC000;
      (&local_E8)->z = 0x8000;
      Script_EVT_SetPosition(0x67676764, POS_XYZ, &local_F4);
      Script_EVT_SetRotation(0x67676764, 0x38000000, &local_E8, 0);
      Script_charaDisplay(0x67676764, CDM_Show);
      Script_EVT_SetPosition(0x66676764, POS_XYZ, &local_F4);
      Script_EVT_SetRotation(0x66676764, 0x38000000, &local_E8, 0);
      Script_charaDisplay(0x66676764, CDM_Show);
      break;
  }
  script_9EB2(CHARA_AKIR, -2.76f, 0.0f, 0.21f);
  Script_InitializeOsag(CHARA_AKIR);
  Script_motmToggleFlags(task_1E4, 2, 4);
  Script_motmToggleFlags(task_1E4, 2, 0x40);
  Script_motmToggleFlags(task_1E4, 1, 0x80);
  Script_motmToggleFlags(task_1E4, 1, 0x20);
  Script_PlayCharaMotion(task_1E4, 28685, -1.0f, -1.0f, -1, 1.0f, 0);
  script_FB0B();
  Script_motmToggleFlags(task_1E4, 2, 0x20);
  SCNF_WaitFrames_0000(10);
  Script_PlayCharaMotion(task_1E4, 28678, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, task_1E4) & Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, 0x52494B41)) {
    Script_EVT_GetPosition(CHARA_AKIR, 4, &local_CC, ENTITY_XFORM_WORLD_SPACE);
    Script_SetLookPointDetails(task_1E4, 16, &local_CC, 2);
    Script_FaceLookAtTargetUtils(task_1E4, 2, &local_CC, 30);
  }
  if (task_1D4 == 5) {
    script_F7BE(task_1D0);
  }
  local_D8 = task_1D4;
  if ((local_D8 == 1) | (local_D8 == 2)) {
    script_9EB2(task_1D0, -1000.0f, -1000.0f, -1000.0f);
    Script_charaDisplay(task_1D0, CDM_Show);
  } else if (local_D8 == 3) {
    script_9EB2(task_1D0, -1000.0f, -1000.0f, -1000.0f);
    Script_charaDisplay(task_1D0, CDM_Show);
  } else {
    if ((local_D8 == 4) | (local_D8 == 5)) {
      Script_charaDisplay(task_1D0, CDM_Hide);
      Script_gachaponUtils(2, task_1D0);
      goto L00DD49;
    }
  }
  L00DD49:;
  Script_seqEnterEvtCam(9118, 0, AUTH_None);
  if (task_1D4 >= 2) {
    Script_motmToggleFlags(CHARA_AKIR, 1, 0x20);
    Script_PlayCharaMotion(CHARA_AKIR, 28674, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  } else {
    Script_ShowUIMessageFromID("SA1079A013");
    Script_motmToggleFlags(CHARA_AKIR, 2, 0x40);
    Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  }
  local_DC = 0;
  for (;;) {
    local_D8 = local_DC;
    if (local_D8 == 40) {
      if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, 0x52494B41) & Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, task_1E4)) {
        Script_EVT_GetPosition(task_1E4, 4, &local_CC, ENTITY_XFORM_WORLD_SPACE);
        Script_SetLookPointDetails(CHARA_AKIR, 16, &local_CC, 2);
        Script_FaceLookAtTargetUtils(CHARA_AKIR, 2, &local_CC, 30);
      }
      break;
      goto L00DE30;
    }
    L00DE30:;
    SCNF_WaitFrames_0000(1);
    local_DC = local_DC + 1;
  }
  Script_motmSetBlendFrames(CHARA_AKIR, MOTM_BLEND_FRAMES_20);
  Script_motmToggleFlags(CHARA_AKIR, 1, 0x40);
  local_D8 = task_1D4;
  if (local_D8 == 1) {
  } else if (local_D8 == 2) {
    if (task_1D0 == 0x70736E63) {
      Script_GetCassetteFlags(38, 1, 0, 0);
      SCNF_StartIconSystemMenu(38);
    } else {
      Script_GetCassetteFlags(37, 1, 0, 0);
      SCNF_StartIconSystemMenu(37);
    }
    script_1061E();
  } else if (local_D8 == 3) {
    Script_GetCassetteFlags(task_1CC, 1, 0, 1);
    local_D8 = task_1CC;
    switch (local_D8) {
      case 10:
        SCNF_StartIconSystemMenu(265);
        break;
      case 11:
        SCNF_StartIconSystemMenu(266);
        break;
      case 12:
        SCNF_StartIconSystemMenu(267);
        break;
      case 13:
        SCNF_StartIconSystemMenu(268);
        break;
    }
    script_1061E();
  } else {
    if ((local_D8 == 4) | (local_D8 == 5)) {
      local_E0 = script_B9F0(-1, task_1CC);
      Script_GetCassetteFlags(local_E0, 1, 0, 0);
      SCNF_StartIconSystemMenu(local_E0);
      script_1061E();
      goto L00DF6E;
    }
  }
  L00DF6E:;
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_motmToggleFlags(CHARA_AKIR, 2, 0x20);
  return;
}

static void script_DF9E(void) {
  script_9EB2(0x6B746E63, -0.185f, 1.09f, -1.42f);
  script_9EDA(0x6B746E63, 0.0f, 0.0f, 0.0f);
  Script_charaDisplay(0x6B746E63, CDM_Show);
  script_9EB2(0x706E6E63, -0.72f, 1.185f, -1.44f);
  script_9EDA(0x706E6E63, 345.0f, 0.0f, 0.0f);
  Script_charaDisplay(0x706E6E63, CDM_Show);
  script_9EB2(0x706D6E63, 0.57f, 1.09f, -1.34f);
  script_9EDA(0x706D6E63, 320.0f, 0.0f, 0.0f);
  Script_charaDisplay(0x706D6E63, CDM_Show);
  script_9EB2(0x6B6D6E63, 0.5f, 0.8f, -1.255f);
  script_9EDA(0x6B6D6E63, 0.0f, 0.0f, 0.0f);
  Script_charaDisplay(0x6B6D6E63, CDM_Show);
  script_9EB2(0x31626E63, -1.225f, 1.135f, -1.475f);
  script_9EDA(0x31626E63, 270.0f, 180.0f, 0.0f);
  Script_charaDisplay(0x31626E63, CDM_Show);
  script_9EB2(0x33626E63, -1.55f, 1.128f, -1.475f);
  script_9EDA(0x33626E63, 270.0f, 0.0f, 0.0f);
  Script_charaDisplay(0x33626E63, CDM_Show);
  script_9EB2(0x636D6E63, -1.92f, 0.661f, -1.402f);
  script_9EDA(0x636D6E63, 90.0f, 180.0f, 0.0f);
  Script_charaDisplay(0x636D6E63, CDM_Show);
  script_9EB2(0x73726E63, -1.17f, 0.695f, -1.44f);
  script_9EDA(0x73726E63, 0.0f, 0.0f, 0.0f);
  Script_charaDisplay(0x73726E63, CDM_Show);
  script_9EB2(0x646B6E63, -1.8f, 1.1f, -1.42f);
  script_9EDA(0x646B6E63, 0.0f, 0.0f, 90.0f);
  Script_charaDisplay(0x646B6E63, CDM_Show);
  script_9EB2(0x67646E63, -1.51f, 0.65f, -1.42f);
  script_9EDA(0x67646E63, 0.0f, 270.0f, 0.0f);
  Script_charaDisplay(0x67646E63, CDM_Show);
  script_9EB2(0x31736E63, -0.61f, 1.572f, -1.44f);
  script_9EDA(0x31736E63, 150.0f, 180.0f, 180.0f);
  Script_charaDisplay(0x31736E63, CDM_Show);
  script_9EB2(0x63736E63, -0.268f, 1.55f, -1.44f);
  script_9EDA(0x63736E63, 90.0f, 180.0f, 180.0f);
  Script_charaDisplay(0x63736E63, CDM_Show);
  script_9EB2(0x6B736E63, 0.1f, 1.515f, -1.385f);
  script_9EDA(0x6B736E63, 90.0f, 180.0f, 180.0f);
  Script_charaDisplay(0x6B736E63, CDM_Show);
  script_9EB2(0x31636E63, 0.0f, 0.0f, -1000.0f);
  script_9EB2(0x32636E63, 0.0f, 0.0f, -1000.0f);
  script_9EB2(0x33636E63, 0.0f, 0.0f, -1000.0f);
  script_9EB2(0x34636E63, 0.0f, 0.0f, -1000.0f);
  script_9EB2(0x35636E63, 0.0f, 0.0f, -1000.0f);
  script_9EB2(0x36636E63, 0.0f, 0.0f, -1000.0f);
  script_9EB2(0x37636E63, 0.0f, 0.0f, -1000.0f);
  script_9EB2(0x38636E63, 0.0f, 0.0f, -1000.0f);
  script_9EB2(0x39636E63, 0.0f, 0.0f, -1000.0f);
  script_9EB2(0x61636E63, 0.0f, 0.0f, -1000.0f);
  script_9EB2(0x62636E63, 0.0f, 0.0f, -1000.0f);
  script_9EB2(0x63636E63, 0.0f, 0.0f, -1000.0f);
  script_9EB2(0x64636E63, 0.0f, 0.0f, -1000.0f);
  script_9EB2(0x65636E63, 0.0f, 0.0f, -1000.0f);
  script_9EB2(0x66636E63, 0.0f, 0.0f, -1000.0f);
  script_9EB2(0x67636E63, 0.0f, 0.0f, -1000.0f);
  script_9EB2(0x68636E63, 0.0f, 0.0f, -1000.0f);
  script_9EB2(0x69636E63, 0.0f, 0.0f, -1000.0f);
  script_9EB2(0x6A636E63, 0.0f, 0.0f, -1000.0f);
  script_9EB2(0x6B636E63, 0.0f, 0.0f, -1000.0f);
  switch (task_1AC) {
    case 0:
      script_9EB2(0x31636E63, -1.86f, 1.503f, -1.4f);
      script_9EDA(0x31636E63, 0.0f, 0.0f, 0.0f);
      Script_charaDisplay(0x31636E63, CDM_Show);
      script_9EB2(0x32636E63, -1.66f, 1.503f, -1.4f);
      script_9EDA(0x32636E63, 0.0f, 0.0f, 0.0f);
      Script_charaDisplay(0x32636E63, CDM_Show);
      script_9EB2(0x33636E63, -1.46f, 1.554f, -1.4f);
      script_9EDA(0x33636E63, 0.0f, 0.0f, 270.0f);
      Script_charaDisplay(0x33636E63, CDM_Show);
      script_9EB2(0x34636E63, -1.26f, 1.554f, -1.4f);
      script_9EDA(0x34636E63, 0.0f, 0.0f, 270.0f);
      Script_charaDisplay(0x34636E63, CDM_Show);
      break;
    case 1:
      script_9EB2(0x35636E63, -1.86f, 1.503f, -1.4f);
      script_9EDA(0x35636E63, 0.0f, 0.0f, 0.0f);
      Script_charaDisplay(0x35636E63, CDM_Show);
      script_9EB2(0x36636E63, -1.66f, 1.554f, -1.4f);
      script_9EDA(0x36636E63, 0.0f, 0.0f, 270.0f);
      Script_charaDisplay(0x36636E63, CDM_Show);
      script_9EB2(0x37636E63, -1.46f, 1.503f, -1.4f);
      script_9EDA(0x37636E63, 0.0f, 0.0f, 0.0f);
      Script_charaDisplay(0x37636E63, CDM_Show);
      script_9EB2(0x38636E63, -1.26f, 1.503f, -1.4f);
      script_9EDA(0x38636E63, 0.0f, 0.0f, 0.0f);
      Script_charaDisplay(0x38636E63, CDM_Show);
      break;
    case 2:
      script_9EB2(0x39636E63, -1.86f, 1.554f, -1.4f);
      script_9EDA(0x39636E63, 0.0f, 0.0f, 270.0f);
      Script_charaDisplay(0x39636E63, CDM_Show);
      script_9EB2(0x61636E63, -1.66f, 1.503f, -1.4f);
      script_9EDA(0x61636E63, 0.0f, 0.0f, 0.0f);
      Script_charaDisplay(0x61636E63, CDM_Show);
      script_9EB2(0x62636E63, -1.46f, 1.554f, -1.4f);
      script_9EDA(0x62636E63, 0.0f, 0.0f, 270.0f);
      Script_charaDisplay(0x62636E63, CDM_Show);
      script_9EB2(0x63636E63, -1.26f, 1.503f, -1.4f);
      script_9EDA(0x63636E63, 0.0f, 0.0f, 0.0f);
      Script_charaDisplay(0x63636E63, CDM_Show);
      break;
    case 3:
      script_9EB2(0x64636E63, -1.86f, 1.503f, -1.4f);
      script_9EDA(0x64636E63, 0.0f, 0.0f, 0.0f);
      Script_charaDisplay(0x64636E63, CDM_Show);
      script_9EB2(0x65636E63, -1.66f, 1.503f, -1.4f);
      script_9EDA(0x65636E63, 0.0f, 0.0f, 0.0f);
      Script_charaDisplay(0x65636E63, CDM_Show);
      script_9EB2(0x66636E63, -1.46f, 1.554f, -1.4f);
      script_9EDA(0x66636E63, 0.0f, 0.0f, 270.0f);
      Script_charaDisplay(0x66636E63, CDM_Show);
      script_9EB2(0x67636E63, -1.26f, 1.554f, -1.4f);
      script_9EDA(0x67636E63, 0.0f, 0.0f, 270.0f);
      Script_charaDisplay(0x67636E63, CDM_Show);
      break;
    case 4:
      script_9EB2(0x68636E63, -1.86f, 1.503f, -1.4f);
      script_9EDA(0x68636E63, 0.0f, 0.0f, 0.0f);
      Script_charaDisplay(0x68636E63, CDM_Show);
      script_9EB2(0x69636E63, -1.66f, 1.503f, -1.4f);
      script_9EDA(0x69636E63, 0.0f, 0.0f, 0.0f);
      Script_charaDisplay(0x69636E63, CDM_Show);
      script_9EB2(0x6A636E63, -1.46f, 1.503f, -1.4f);
      script_9EDA(0x6A636E63, 0.0f, 0.0f, 0.0f);
      Script_charaDisplay(0x6A636E63, CDM_Show);
      script_9EB2(0x6B636E63, -1.26f, 1.503f, -1.4f);
      script_9EDA(0x6B636E63, 0.0f, 0.0f, 0.0f);
      Script_charaDisplay(0x6B636E63, CDM_Show);
      break;
  }
  return;
}

static void script_E909(void) {
  int32_t unused_FC;
  int32_t local_F8;
  local_F8 = task_1F8;
  if (local_F8 == 0x6B746E63) {
    script_9EB2(0x6B746E63, -2.73f, 0.773f, -0.46f);
    script_9EDA(0x6B746E63, 0.0f, 0.0f, 0.0f);
    Script_charaDisplay(0x6B746E63, CDM_Show);
  } else if (local_F8 == 0x706E6E63) {
    script_9EB2(0x706E6E63, -2.73f, 0.77f, -0.46f);
    script_9EDA(0x706E6E63, 270.0f, 0.0f, 0.0f);
    Script_charaDisplay(0x706E6E63, CDM_Show);
  } else if (local_F8 == 0x706D6E63) {
    script_9EB2(0x706D6E63, -2.73f, 0.763f, -0.46f);
    script_9EDA(0x706D6E63, 270.0f, 0.0f, 0.0f);
    Script_charaDisplay(0x706D6E63, CDM_Show);
  } else if (local_F8 == 0x6B6D6E63) {
    script_9EB2(0x6B6D6E63, -2.706f, 0.832f, -0.407f);
    script_9EDA(0x6B6D6E63, 0.0f, 0.0f, 0.0f);
    Script_charaDisplay(0x6B6D6E63, CDM_Show);
  } else if (local_F8 == 0x31626E63) {
    script_9EB2(0x31626E63, -2.73f, 0.77f, -0.46f);
    script_9EDA(0x31626E63, 270.0f, 0.0f, 0.0f);
    Script_charaDisplay(0x31626E63, CDM_Show);
  } else if (local_F8 == 0x33626E63) {
    script_9EB2(0x33626E63, -2.73f, 0.77f, -0.46f);
    script_9EDA(0x33626E63, 270.0f, 0.0f, 0.0f);
    Script_charaDisplay(0x33626E63, CDM_Show);
  } else if (local_F8 == 0x636D6E63) {
    script_9EB2(0x636D6E63, -2.73f, 0.758f, -0.46f);
    script_9EDA(0x636D6E63, 90.0f, 180.0f, 0.0f);
    Script_charaDisplay(0x636D6E63, CDM_Show);
  } else if (local_F8 == 0x73726E63) {
    script_9EB2(0x73726E63, -2.73f, 0.77f, -0.46f);
    script_9EDA(0x73726E63, 0.0f, 90.0f, 90.0f);
    Script_charaDisplay(0x73726E63, CDM_Show);
  } else if (local_F8 == 0x646B6E63) {
    script_9EB2(0x646B6E63, -2.633f, 0.78f, -0.453f);
    script_9EDA(0x646B6E63, 0.0f, 270.0f, 90.0f);
    Script_charaDisplay(0x646B6E63, CDM_Show);
  } else if ((local_F8 == 0x31636E63) | (local_F8 == 0x32636E63) | (local_F8 == 0x33636E63) | (local_F8 == 0x34636E63) | (local_F8 == 0x35636E63) | (local_F8 == 0x36636E63) | (local_F8 == 0x37636E63) | (local_F8 == 0x38636E63) | (local_F8 == 0x39636E63) | (local_F8 == 0x61636E63) | (local_F8 == 0x62636E63) | (local_F8 == 0x63636E63) | (local_F8 == 0x64636E63) | (local_F8 == 0x65636E63) | (local_F8 == 0x66636E63) | (local_F8 == 0x67636E63) | (local_F8 == 0x68636E63) | (local_F8 == 0x69636E63) | (local_F8 == 0x6A636E63) | (local_F8 == 0x6B636E63)) {
    script_9EB2(task_1F8, -2.73f, 0.77f, -0.46f);
    script_9EDA(task_1F8, 0.0f, 90.0f, 90.0f);
    Script_charaDisplay(task_1F8, CDM_Show);
  } else {
    switch (local_F8) {
      case 0x31736E63:
        script_9EB2(0x31736E63, -2.73f, 0.773f, -0.46f);
        script_9EDA(0x31736E63, 270.0f, 0.0f, 0.0f);
        Script_charaDisplay(0x31736E63, CDM_Show);
        break;
      case 0x63736E63:
        script_9EB2(0x63736E63, -2.73f, 0.773f, -0.46f);
        script_9EDA(0x63736E63, 270.0f, 0.0f, 0.0f);
        Script_charaDisplay(0x63736E63, CDM_Show);
        break;
      case 0x6B736E63:
        script_9EB2(0x6B736E63, -2.73f, 0.773f, -0.46f);
        script_9EDA(0x6B736E63, 270.0f, 0.0f, 0.0f);
        Script_charaDisplay(0x6B736E63, CDM_Show);
        break;
      case 0x67646E63:
        script_9EB2(0x67646E63, -2.73f, 0.78f, -0.46f);
        script_9EDA(0x67646E63, 0.0f, 270.0f, 0.0f);
        Script_charaDisplay(0x67646E63, CDM_Show);
        break;
    }
  }
  return;
}

static void script_EDCA(void) {
  int32_t local_FC;
  script_9EB2(0x74687364, -2.75f, 0.754f, -0.441f);
  script_9EB2(0x78626B64, -3.4f, 0.74f, -0.432f);
  script_9EDA(0x78626B64, 0.0f, 0.0f, 0.0f);
  Script_charaDisplay(0x78626B64, CDM_Show);
  if (task_1D4 < 5) {
    if (task_1B4 == 1) {
      Script_ShowUIMessageFromID("SA1079C016");
    } else {
      Script_ShowUIMessageFromID("SA1079B018");
    }
  }
  local_FC = task_1D4;
  switch (local_FC) {
    case 0:
      break;
    case 1:
      Script_seqEnterEvtCam(9121, 0, AUTH_None);
      SCNF_WaitFrames_0000(50);
      break;
    case 2:
      Script_seqEnterEvtCam(9122, 0, AUTH_None);
      SCNF_WaitFrames_0000(50);
      break;
    case 3:
      Script_seqEnterEvtCam(9123, 0, AUTH_None);
      SCNF_WaitFrames_0000(50);
      break;
    case 4:
      Script_seqEnterEvtCam(9124, 0, AUTH_None);
      SCNF_WaitFrames_0000(60);
      break;
    case 5:
      Script_seqEnterEvtCam(9125, 0, AUTH_None);
      SCNF_WaitFrames_0000(100);
      break;
  }
  return;
}

static int32_t script_EEF7(void) {
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
  int8_t local_B5;
  int8_t local_B4;
  int32_t local_B0;
  Script_scenToggleVisibleFor(SHEN_NAME('YKUR'), 1);
  Script_scenToggleVisibleFor(SHEN_NAME('YKUL'), 1);
  Script_scenToggleVisibleFor(SHEN_NAME('URDM'), 1);
  Script_charaDisplay(CHARA_AKIR, CDM_Hide);
  Script_charaDisplay(SHEN_NAME('URDM'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('YKUR'), CDM_Show);
  Script_handSetFloatsFor(SHEN_NAME('YKUR'), 1, data_14898, 10);
  local_F4.x = -2.75f;
  (&local_F4)->y = 0.754f;
  (&local_F4)->z = -0.441f;
  Script_EVT_SetPosition(0x74687364, POS_XYZ, &local_F4);
  Script_charaDisplay(0x74687364, CDM_Show);
  local_DC.x = 17880;
  (&local_DC)->y = 64520;
  (&local_DC)->z = 32210;
  Script_EVT_SetRotation(SHEN_NAME('YKUR'), 0x38000000, &local_DC, 0);
  Script_EVT_SetRotation(SHEN_NAME('URDM'), 0x38000000, &local_DC, 0);
  Script_SetCameraFlags(0);
  Script_SetCharaLookAtPosition(task_1E4, -20, 0, 0);
  script_EDCA();
  local_D0 = 0.0f;
  local_CC = 0.0f;
  {
    int32_t local_AC;
    local_AC = task_1D4;
    switch (local_AC) {
      case 1:
        local_D8 = -2.8125f;
        local_D4 = -0.38f;
        local_B5 = 2;
        local_B4 = 10;
        break;
      case 2:
        local_D8 = -2.764f;
        local_D4 = -0.3f;
        local_B5 = 1;
        local_B4 = 14;
        break;
      case 3:
        local_D8 = -2.8315f;
        local_D4 = -0.208f;
        local_B5 = 3;
        local_B4 = 8;
        break;
      case 4:
        local_D8 = -2.8315f;
        local_D4 = -0.105f;
        local_B5 = 3;
        local_B4 = 8;
        break;
    }
  }
  local_F4.x = local_D8;
  (&local_F4)->y = 0.84f;
  (&local_F4)->z = local_D4;
  if ((task_1D4 == 1) | (task_1D4 == 3) | (task_1D4 == 4)) {
    local_D8 = local_D8 + (float)(local_B4 - 1) * 0.0115f;
    local_B6 = 1;
  } else {
    local_B6 = 0;
  }
  local_C4 = local_D4 + 0.15f;
  local_F4.x = local_D8;
  for (;;) {
    local_C4 = local_C4 - 0.02f;
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
  Script_seqEnterEvtCam(9120, SHEN_NAME('URDM'), SHEN_NAME('URDM'));
  Script_uiUtils(0, 58, 0);
  Script_shenClearFreeState(1);
  Script_sicnUtils(0, 4, 1);
  local_C0 = 0;
  local_B7 = 0;
  local_B8 = 0;
  for (;;) {
    if (local_B7 == 0) {
      if (Script_GetControllerInput(0) == 0x4000) {
        if (local_B6 > 0) {
          local_B6 = local_B6 - 1;
          local_BC = 0;
          local_B7 = 1;
        }
      }
      if (Script_GetControllerInput(0) == 0x8000) {
        if (local_B6 < local_B5) {
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
        local_F4.x = (&local_F4)->x - 0.0105f;
        local_D0 = local_D0 + 0.004f;
        if (!(local_D0 <= 0.02f)) {
          local_D0 = 0.02f;
        }
      } else {
        local_F4.x = (&local_F4)->x + 0.0105f;
        local_D0 = local_D0 - 0.004f;
        if (!(local_D0 >= -0.02f)) {
          local_D0 = -0.02f;
        }
      }
      local_BC = local_BC + 1;
      if (local_BC == local_B4) {
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
        goto L00F36C;
      }
    }
    L00F36C:;
    if (!(local_CC <= 0.001f)) {
      local_CC = local_CC - 0.002f;
    } else {
      if (!(local_CC >= -0.001f)) {
        local_CC = local_CC + 0.002f;
        goto L00F3A2;
      }
    }
    L00F3A2:;
    Script_EVT_SetPosition(SHEN_NAME('URDM'), POS_XYZ, &local_F4);
    local_E8.x = (&local_F4)->x + local_D0;
    (&local_E8)->y = (&local_F4)->y;
    (&local_E8)->z = (&local_F4)->z + local_CC;
    Script_EVT_SetPosition(SHEN_NAME('YKUR'), POS_XYZ, &local_E8);
    Script_scenUtils(SCENARIO_CMD_SET_STEP_LUT, 7, task_1D4 * 10 + local_B6);
    SCNF_WaitFrames_0000(1);
  }
  Script_scenUtils(SCENARIO_CMD_SET_STEP_LUT, 7, 0);
  Script_sicnUtils(2, 4, 0);
  Script_shenClearFreeState(0);
  Script_uiUtils(0, 56, 0);
  script_F68D();
  {
    int32_t local_AC;
    local_AC = task_1D4;
    switch (local_AC) {
      case 1:
        local_AC = local_B6;
        switch (local_AC) {
          case 0:
            local_B0 = 0x72626E63;
            Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 963, 1);
            Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 971, 0);
            Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 966, 0);
            break;
          case 1:
            local_B0 = 0x72726E63;
            Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 963, 0);
            Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 971, 1);
            Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 966, 0);
            break;
          case 2:
            local_B0 = 0x72736E63;
            Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 963, 0);
            Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 971, 0);
            Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 966, 1);
            break;
        }
        break;
      case 2:
        local_AC = local_B6;
        switch (local_AC) {
          case 0:
            local_B0 = 0x70736E63;
            break;
          case 1:
            local_B0 = 0x6F686E63;
            break;
        }
        break;
      case 3:
        local_AC = local_B6;
        switch (local_AC) {
          case 0:
            local_B0 = 0x6E636E63;
            task_1CC = 10;
            break;
          case 1:
            local_B0 = 0x6F636E63;
            task_1CC = 11;
            break;
          case 2:
            local_B0 = 0x70636E63;
            task_1CC = 12;
            break;
          case 3:
            local_B0 = 0x71636E63;
            task_1CC = 13;
            break;
        }
        break;
      case 4:
        local_AC = local_B6;
        switch (local_AC) {
          case 0:
            local_BC = 0;
            break;
          case 1:
            local_BC = 1;
            break;
          case 2:
            local_BC = 3;
            break;
          case 3:
            local_BC = 2;
            break;
        }
        local_B0 = script_F787(local_BC);
        break;
    }
  }
  Script_ShowUIMessageFromID("SA1079A014");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_SetCameraFlags(1);
  Script_charaDisplay(SHEN_NAME('YKUR'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('URDM'), CDM_Hide);
  Script_charaDisplay(CHARA_AKIR, CDM_Show);
  local_F4.x = -3.108f;
  (&local_F4)->y = 0.754f;
  (&local_F4)->z = -0.441f;
  Script_EVT_SetPosition(0x74687364, POS_XYZ, &local_F4);
  return local_B0;
}

static void script_F68D(void) {
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

static int32_t script_F74E(void) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  float local_F0;
  script_BBDB((int32_t)&local_FC, (int32_t)&local_F8);
  local_F0 = 1.0f;
  script_BC7D(0x6E68656B, local_FC, local_F8, local_F0);
  task_1CC = local_F8;
  return local_F4 = 0x6E68656B;
}

static int32_t script_F787(int32_t arg0) {
  int32_t local_FC;
  float local_F8;
  script_B84F(8);
  local_F8 = 1.0f;
  script_BC7D(0x6E68656B, 8, arg0, local_F8);
  task_1CC = arg0;
  return local_FC = 0x6E68656B;
}

static void script_F7BE(int32_t arg0) {
  Vector3 local_F4;
  Script_ResetHandMotion(CHARA_AKIR, 1, 0, 0);
  Script_ResetHandMotion(CHARA_AKIR, 1, 1, 0);
  Script_handSetFloatsFor(CHARA_AKIR, 1, data_145EC, 10);
  Script_handSetFloatsFor(CHARA_AKIR, 0, data_145EC, 10);
  local_F4.x = 0.081f;
  (&local_F4)->y = 0.005f;
  (&local_F4)->z = -0.046f;
  {
    Vector3i local_E8;
    local_E8.x = 25282;
    (&local_E8)->y = 3231;
    (&local_E8)->z = 14226;
    Script_fixoAttachEx(0x66676764, CHARA_AKIR, 18, &local_F4, &local_E8);
  }
  local_F4.x = 0.096f;
  (&local_F4)->y = 0.012f;
  (&local_F4)->z = 0.014f;
  {
    Vector3i local_E8;
    local_E8.x = 12014;
    (&local_E8)->y = 27488;
    (&local_E8)->z = 17476;
    Script_fixoAttachEx(0x67676764, CHARA_AKIR, 12, &local_F4, &local_E8);
  }
  local_F4.x = -2.7449f;
  (&local_F4)->y = 1.0675f;
  (&local_F4)->z = -0.1765f;
  Script_SetCharaLookAtPosition(CHARA_AKIR, 20, &local_F4, 0);
  Script_FaceLookAtTargetUtils(CHARA_AKIR, 2, &local_F4, 1);
  Script_seqEnterEvtCam(9130, 0, AUTH_None);
  Script_PlayCharaMotion(CHARA_AKIR, 28684, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_SetCharaMotionPlaybackRate(CHARA_AKIR, 2.0f);
  {
    Vector3i local_E8;
    int32_t local_E4;
    local_E4 = 0;
    for (;;) {
      int32_t local_E0;
      local_E0 = local_E4;
      switch (local_E0) {
        case 50:
          Script_fixoClearAttachment(0x66676764);
          script_9EB2(0x66676764, 0.0f, 0.0f, -1000.0f);
          local_F4.x = 0.118f;
          (&local_F4)->y = 0.02f;
          (&local_F4)->z = -0.062f;
          local_E8.x = 59708;
          (&local_E8)->y = 43506;
          (&local_E8)->z = 63619;
          Script_fixoAttachEx(arg0, CHARA_AKIR, 18, &local_F4, &local_E8);
          Script_charaDisplay(arg0, CDM_Show);
          break;
        case 80:
          Script_executeAicaCommand(0x1105A9, 0, 0);
          break;
        case 190:
          local_F4.x = -2.5876f;
          (&local_F4)->y = 1.3923f;
          (&local_F4)->z = -0.1915f;
          Script_SetCharaLookAtPosition(CHARA_AKIR, 0x10000, &local_F4, 0);
          Script_FaceLookAtTargetUtils(CHARA_AKIR, 2, &local_F4, 10);
          break;
        case 240:
          Script_fixoClearAttachment(0x67676764);
          script_9EB2(0x67676764, 0.0f, 0.0f, -1000.0f);
          break;
      }
      if (Script_IsCharaMotionFinished(CHARA_AKIR)) {
        break;
      }
      SCNF_WaitFrames_0000(1);
      local_E4 = local_E4 + 2;
    }
  }
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, task_1E4) & Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, 0x52494B41)) {
    int32_t unused_E4[4];
    Vector3 local_D8;
    Script_EVT_GetPosition(CHARA_AKIR, 4, &local_D8, ENTITY_XFORM_WORLD_SPACE);
    Script_SetLookPointDetails(task_1E4, 20, &local_D8, 2);
    Script_FaceLookAtTargetUtils(task_1E4, 2, &local_D8, 30);
  }
  Script_seqEnterEvtCam(9140, CHARA_AKIR, CHARA_AKIR);
  SCNF_WaitFrames_0000(60);
  Script_fixoClearAttachment(arg0);
  script_9EB2(arg0, 0.0f, 0.0f, -1000.0f);
  Script_handSetFloatsFor(CHARA_AKIR, 1, data_145EC, 10);
  Script_handSetFloatsFor(CHARA_AKIR, 0, data_145EC, 10);
  return;
}

static void script_FB0B(void) {
  int32_t local_FC;
  int32_t local_F8;
  if (task_1B4 == 1) {
    if (task_1D4 == 1) {
      Script_seqEnterEvtCam(6060, 0, AUTH_None);
      Script_ShowUIMessageFromID("SA1079C020");
    } else if (task_1D4 == 5) {
      Script_seqEnterEvtCam(9975, 0, AUTH_None);
      Script_ShowUIMessageFromID("SA1079C018");
    } else {
      Script_seqEnterEvtCam(9975, 0, AUTH_None);
      Script_ShowUIMessageFromID("SA1079C015");
    }
  } else if (task_1D4 == 1) {
    Script_seqEnterEvtCam(5060, 0, AUTH_None);
    Script_ShowUIMessageFromID("SA1079B025");
  } else if (task_1D4 == 5) {
    Script_seqEnterEvtCam(9075, 0, AUTH_None);
    Script_ShowUIMessageFromID("SA1079B020");
  } else {
    Script_seqEnterEvtCam(9075, 0, AUTH_None);
    *(float *)&local_FC = Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f);
    if (!((float)local_FC >= 1.0f)) {
      Script_ShowUIMessageFromID("SA1079B016");
    } else {
      Script_ShowUIMessageFromID("SA1079B019");
    }
  }
  local_F8 = 0;
  for (;;) {
    if (local_F8 >= 260) {
      break;
    }
    SCNF_WaitFrames_0000(1);
    local_F8 = local_F8 + 2;
  }
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_FC2C(void) {
  int32_t local_FC;
  if (task_1B4 == 1) {
    Script_seqEnterEvtCam(6030, 0, AUTH_None);
    Script_ShowUIMessageFromID("SA1079C019");
  } else {
    Script_seqEnterEvtCam(5030, 0, AUTH_None);
    *(float *)&local_FC = Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f);
    if (!((float)local_FC >= 1.0f) & (task_1F0 != 0)) {
      Script_ShowUIMessageFromID("SA1079B021");
    } else {
      Script_ShowUIMessageFromID("SA1079B022");
    }
  }
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_FCA9(void) {
  int32_t unused_FC;
  int32_t local_F8;
  local_F8 = task_1D4;
  if ((local_F8 == 1) | (local_F8 == 2) | (local_F8 == 3) | (local_F8 == 4)) {
    Script_ShowUIMessageFromID("SA1079A015");
  } else {
    switch (local_F8) {
      case 5:
        Script_ShowUIMessageFromID("SA1079A016");
        break;
      case 6:
        Script_ShowUIMessageFromID("SA1079A017");
        break;
    }
  }
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_FD19(void) {
  int32_t unused_FC;
  int32_t local_F8;
  if (task_1B4 == 1) {
    if (task_1D4 == 1) {
      Script_seqEnterEvtCam(6050, 0, AUTH_None);
      Script_ShowUIMessageFromID("SA1079C014");
      while (Script_convGetIsActive(0)) {
        SCNF_WaitFrames_0000(1);
      }
    }
  } else {
    local_F8 = task_1D4;
    switch (local_F8) {
      case 1:
        Script_seqEnterEvtCam(5050, 0, AUTH_None);
        Script_ShowUIMessageFromID("SA1079B015");
        while (Script_convGetIsActive(0)) {
          SCNF_WaitFrames_0000(1);
        }
        break;
      case 2:
        Script_seqEnterEvtCam(5050, 0, AUTH_None);
        Script_ShowUIMessageFromID("SA1079B017");
        while (Script_convGetIsActive(0)) {
          SCNF_WaitFrames_0000(1);
        }
        break;
    }
  }
  return;
}

static void script_FDCD(void) {
  Vector3 local_F4;
  int32_t local_F0;
  int32_t local_EC;
  int32_t local_E8;
  Vector3i local_DC;
  if (Script_IsPlayerAllowed(0) == 0) {
  } else {
    Script_EVT_GetPosition(CHARA_AKIR, -1, &local_F4, 0);
    Script_mtwkGetSetRotation(CHARA_AKIR, &local_DC, 0x42000000);
    if (Script_CheckTargetWithinAngleRange(&data_14D90, 1.5f, 0x8000, 5461, &local_F4) & Script_CheckTargetWithinAngleRange(&local_F4, 1.5f, (&local_DC)->y, 7281, &data_14D90)) {
      Script_SetOwnedItemMenuFlagIfOwned(1, OwnedItem_UIITEMWINN, 1);
    } else {
      Script_SetOwnedItemMenuFlagIfOwned(1, OwnedItem_UIITEMWINN, 0);
    }
    local_F0 = Script_cassetteUtils();
    local_EC = (local_F0 & -0x10000) >> 16;
    local_E8 = local_F0 & 0xFFFF;
    if (local_E8 == 2) {
      if (local_EC == 66) {
        script_FE89();
      }
    }
  }
  return;
}

static void script_FE89(void) {
  int32_t local_FC;
  Script_ryoSleepUtils(2, 0);
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_FaceEx(CHARA_AKIR, 1, 0, 2);
  Script_ResetHandMotion(CHARA_AKIR, -1, 0, 0);
  Script_ResetHandMotion(CHARA_AKIR, -1, 1, 0);
  Script_shenClearFreeState(0);
  Script_filmCheckOrSetTransitionUtil(2, 0, 0, 0, 0, 255, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
  Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT, task_1E4, 1);
  local_FC = Script_CycleManagerUtils(CYCLE_CMD_GET_CYCLE_PTR_BY_CHARA_ID, task_1E4);
  while (local_FC == 0) {
    SCNF_WaitFrames_0000(1);
    local_FC = Script_CycleManagerUtils(CYCLE_CMD_GET_CYCLE_PTR_BY_CHARA_ID, task_1E4);
  }
  while (Script_CycleManagerUtils(CYCLE_CMD_GET_NEXT_TASK_POINTER, local_FC) == 0) {
    SCNF_WaitFrames_0000(1);
  }
  Script_CycleManagerUtils(CYCLE_CMD_SET_RUNNING_PROGRAM_FIELD_B8, task_1E4, 0);
  Script_motmToggleFlags(task_1E4, 2, 0x40);
  script_9EB2(task_1E4, -2.7f, 0.0f, -0.9f);
  script_9EDA(task_1E4, 0.0f, 180.0f, 0.0f);
  Script_charaDisplay(task_1E4, CDM_Show);
  Script_SetShadowFlags(CHARA_AKIR, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 0);
  Script_PlayerStatsUtils(PS_SetCharaItemCount, task_1E4, Script_PlayerStatsUtils(PS_GetCharaItemCount, task_1E4) + 1);
  Script_motmToggleFlags(CHARA_AKIR, 2, 0x40);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_SetCPAMFlags(task_1E4, 0);
  Script_SetCPAMFlags(CHARA_AKIR, 0);
  Script_ccowUtils(CHARA_AKIR, 2);
  Script_InitializeOsag(CHARA_AKIR);
  Script_ResetHandMotion(CHARA_AKIR, 1, 1, 0);
  Script_ResetHandMotion(CHARA_AKIR, 1, 0, 0);
  task_1D4 = -1;
  task_1F0 = 1;
  script_1023E();
  script_CF8F();
  if (task_1D4 != 6) {
    script_D843();
    SCNF_WaitFrames_0000(30);
  } else {
    script_9EB2(0x78626B64, -3.4f, 0.74f, -0.432f);
    script_9EDA(0x78626B64, 0.0f, 0.0f, 0.0f);
    Script_charaDisplay(0x78626B64, CDM_Show);
  }
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, 0x52494B41)) {
    Script_SetCharaLookAtPosition(CHARA_AKIR, -13, 0, 0);
    Script_FaceLookAtTargetUtils(CHARA_AKIR, 0);
  }
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, task_1E4)) {
    Script_SetCharaLookAtPosition(task_1E4, -13, 0, 0);
    Script_FaceLookAtTargetUtils(task_1E4, 0);
  }
  Script_motmToggleFlags(CHARA_AKIR, 2, 0x40);
  script_9EB2(CHARA_AKIR, -2.76f, 0.0f, 0.21f);
  Script_InitializeOsag(CHARA_AKIR);
  Script_motmToggleFlags(CHARA_AKIR, 2, 0x20);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
  Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
  Script_ccowUtils(CHARA_AKIR, 1);
  Script_SetCPAMFlags(task_1E4, 1);
  Script_SetCPAMFlags(CHARA_AKIR, 1);
  if (task_1D4 == 6) {
    Script_convSetCamInactiveFlag(1);
  }
  Script_CycleManagerUtils(CYCLE_CMD_LOAD_OR_RESET_RUNNING_PROGRAM, local_FC);
  Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT, task_1E4, 0);
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 1);
  Script_SetShadowFlags(CHARA_AKIR, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 4);
  Script_srchUtils(SRCH_SCRIPT_Reset);
  Script_SetGameStateTo(AUTH_None);
  SCNF_WaitFrames_0000(18);
  SCNF_WaitFrames_0000(1);
  Script_ryoControl(RYO_RestoreControl);
  Script_FaceEx(CHARA_AKIR, 0, 0, 2);
  Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
  Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
  Script_shenClearFreeState(1);
  Script_ryoSleepUtils(2, 1);
  return;
}

__attribute__((scn_string_bindings(0, data_14DB4, 1, data_14DBF)))
static void script_1023E(void) {
  int32_t unused_F0[4];
  Vector3 local_E4;
  script_9EB2(CHARA_AKIR, -2.76f, 0.0f, 0.21f);
  script_9EDA(CHARA_AKIR, 0.0f, 0.0f, 0.0f);
  Script_InitializeOsag(CHARA_AKIR);
  Script_SetHandRootRotation(CHARA_AKIR, 1, 0x15, &data_14D9C);
  Script_handSetFloatsFor(CHARA_AKIR, 1, data_145EC, 0);
  Script_handSetFloatsFor(CHARA_AKIR, 0, data_145EC, 0);
  Script_seqEnterEvtCam(9010, 0, AUTH_None);
  Script_filmCheckOrSetTransitionUtil(5, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, 0x52494B41) & Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, task_1E4)) {
    Script_EVT_GetPosition(task_1E4, 4, &local_E4, ENTITY_XFORM_WORLD_SPACE);
    Script_SetLookPointDetails(CHARA_AKIR, 16, &local_E4, 2);
    Script_FaceLookAtTargetUtils(CHARA_AKIR, 2, &local_E4, 30);
  }
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, task_1E4) & Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, 0x52494B41)) {
    Script_EVT_GetPosition(CHARA_AKIR, 4, &local_E4, ENTITY_XFORM_WORLD_SPACE);
    Script_SetLookPointDetails(task_1E4, 16, &local_E4, 2);
    Script_FaceLookAtTargetUtils(task_1E4, 2, &local_E4, 30);
  }
  SCNF_WaitFrames_0000(40);
  Script_seqEnterEvtCam(9030, 0, AUTH_None);
  script_9EB2(0x6B746164, -2.73f, 0.813f, -0.46f);
  script_9EDA(0x6B746164, 180.0f, 180.0f, 180.0f);
  Script_charaDisplay(0x6B746164, CDM_Show);
  Script_ShowUIMessageFromID("SA1079A007");
  script_9EB2(CHARA_AKIR, -3.16f, 0.094f, 0.173f);
  script_9EDA(CHARA_AKIR, 352.002014f, 355.00119f, 341.004608f);
  Script_motmToggleFlags(CHARA_AKIR, 2, 0x40);
  Script_motmToggleFlags(CHARA_AKIR, 1, 0x20);
  Script_PlayCharaMotion(CHARA_AKIR, 28673, 10.0f, 29.0f, -1.0f, 1.0f, 0);
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_motmToggleFlags(CHARA_AKIR, 2, 0x20);
  script_9EB2(CHARA_AKIR, -2.76f, 0.0f, 0.21f);
  Script_motmToggleFlags(CHARA_AKIR, 2, 0x40);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  script_9EB2(CHARA_AKIR, -2.76f, 0.0f, 0.21f);
  script_9EDA(CHARA_AKIR, 0.0f, 0.0f, 0.0f);
  Script_InitializeOsag(CHARA_AKIR);
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, 0x52494B41) & Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, task_1E4)) {
    Script_EVT_GetPosition(task_1E4, 4, &local_E4, ENTITY_XFORM_WORLD_SPACE);
    Script_SetLookPointDetails(CHARA_AKIR, 20, &local_E4, 2);
    Script_FaceLookAtTargetUtils(CHARA_AKIR, 2, &local_E4, 30);
  }
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, task_1E4) & Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, 0x52494B41)) {
    Script_EVT_GetPosition(CHARA_AKIR, 4, &local_E4, ENTITY_XFORM_WORLD_SPACE);
    Script_SetLookPointDetails(task_1E4, 20, &local_E4, 2);
    Script_FaceLookAtTargetUtils(task_1E4, 2, &local_E4, 30);
  }
  if (task_1D4 != 6) {
    Script_seqEnterEvtCam(9060, CHARA_AKIR, CHARA_AKIR);
  } else {
    Script_seqEnterEvtCam(9065, CHARA_AKIR, CHARA_AKIR);
  }
  script_9EB2(0x6B746164, 0.0f, 0.0f, -1000.0f);
  if (task_1B4 == 1) {
    Script_seqEnterEvtCam(6025, 0, AUTH_None);
    Script_ShowUIMessageFromID("SA1079C013");
  } else {
    Script_seqEnterEvtCam(6020, 0, AUTH_None);
    Script_ShowUIMessageFromID("SA1079B014");
  }
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_GetCassetteFlags_0(66, 1, 0);
  SCNF_WaitFrames_0000(20);
  return;
}

static void script_1061E(void) {
  int32_t local_FC;
  if (task_1D4 == 6) {
    Script_seqEnterEvtCam(5040, 0, AUTH_None);
  }
  *(float *)&local_FC = Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f);
  if (task_1B4 == 1) {
    if (!((float)local_FC >= 1.0f)) {
      Script_ShowUIMessageFromID("SA1079C011");
    } else {
      Script_ShowUIMessageFromID("SA1079C012");
    }
  } else if (!((float)local_FC >= 1.0f)) {
    Script_ShowUIMessageFromID("SA1079B011");
  } else {
    Script_ShowUIMessageFromID("SA1079B012");
  }
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_106AC(void) {
  int32_t local_FC;
  local_FC = Script_GetSceneID();
  switch (local_FC) {
    case 1:
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_ENTRY2_TIMED_EVENT_DONE) == 0) {
        Script_TimedTransitionUtils(TIMED_TRANSITION_REGISTER, 1, 0x3044484A, 2, &data_14E74, 0xD5A6C3, TimedTransitionUpdateMemoItem150);
      }
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JD00_ENTRY20_TIMED_EVENT_DONE) == 0) {
        Script_TimedTransitionUtils(TIMED_TRANSITION_REGISTER, 1, 0x3030444A, 20, &data_14E3C, 0xDA3AA3, 0);
      }
      break;
    case 2:
      if ((Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_GMCT_ROUTE_STARTED) == 0) | (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_GMCT_ROUTE_ADVANCED) == 0)) {
        Script_TimedTransitionUtils(TIMED_TRANSITION_REGISTER, 2, 0x54434D47, 1, &data_14E04, 0xB7420F, 0);
      }
      break;
    case 3:
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOZOMI_RESCUE_RUSH_ACTIVE) == 0) {
        Script_TimedTransitionUtils(TIMED_TRANSITION_REGISTER, 3, 0x4F4D4F4A, 20, &data_14E20, 0x2DC9FE, TimedTransitionRollbackToPreviousDay2330);
      }
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_FIRED_FROM_JOB) & ~Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 970)) {
        if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_MFSY_ENTRY42_TIMED_EVENT_DONE) == 0) {
          Script_TimedTransitionUtils(TIMED_TRANSITION_REGISTER, 3, 0x5953464D, 42, &data_14E58, 0xBBB817, 0);
        }
      }
      break;
  }
  return;
}

static void script_107E4(int32_t arg0) {
  script_107FC(arg0);
  return;
}

static void script_107F0(int32_t arg0) {
  script_107FC(arg0);
  return;
}

static void script_107FC(int32_t arg0) {
  if (Script_StepUtils(GET_STEP_HIGHER, 0) == 22) {
    int32_t local_FC;
    local_FC = Script_StepUtils(GET_STEP_LOWER, 0);
    if ((local_FC == 2) | (local_FC == 3)) {
      Script_SetShouldSleepFlag(0);
      goto L010835;
    }
  }
  L010835:;
  Script_ryoSetChara(arg0);
  Script_facemotUtils(arg0, 1, 0);
  Script_facemotUtils(arg0, 0, 0);
  Script_FaceEx(arg0, 0, 0, 2);
  Script_CharHandlers(arg0, -1, 0, 0);
  script_106AC();
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

static int32_t script_108F8(int32_t arg0) {
  script_107F0(arg0);
  for (;;) {
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_1090F(int32_t arg0) {
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

static void script_10950(int32_t arg0) {
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

static void script_10994(int32_t arg0) {
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

static void script_10A12(void) {
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_PROGRESS_STEP_28_REACHED)) {
    script_10994(40);
  } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_PROGRESS_STEP_1E_REACHED)) {
    script_10994(30);
  } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_PROGRESS_STEP_14_REACHED)) {
    script_10994(20);
  } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JU00_CATA1_NBO_BRANCH_CHOSEN)) {
    script_10994(10);
  } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_PROGRESS_STEP05_SA1116_DOJO_DONE)) {
    script_10994(5);
  } else {
    script_10994(0);
  }
  return;
}

static int32_t script_10A91(void) {
  int8_t local_FF;
  int32_t local_F8;
  script_AB36();
  for (;;) {
    local_FF = script_AB88();
    local_F8 = local_FF;
    if (local_FF != -1) {
      script_AB60();
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

void script_10AC2(void) {
  int32_t unused_FC;
  Vector3 local_F0;
  Script_scenUtils(SCENARIO_CMD_SET_STEP_LUT, 7, 0);
  task_1EC = Script_ryoSleepUtils(0);
  if (task_1EC != 0) {
    Script_ryoSleepUtils(1);
    Script_EVT_GetPosition(CHARA_AKIR, -1, &local_F0, 0);
    if (!((&local_F0)->z <= 1.0f)) {
      (&local_F0)->z = 1.0f;
      Script_EVT_SetPosition(CHARA_AKIR, 0x78000000, &local_F0);
    }
  }
  Script_filmCheckOrSetTransitionUtil(0, 255, 0, 0, 0, 0, 0, 0, 0);
  Script_Ambient1SoundUtils(1, 24232);
  Script_PlayerStatsUtils(PS_IncrementSpotVisitCount, 50);
  script_7FBB(CHARA_AKIR, (DSWKDoorEntry *)&data_12EF4, (int32_t *)data_12F2C, (DoorResourceDescriptor *)&data_12F3C, (MapObjectRecord *)&data_12F4C, 10, 0);
  SCNF_WaitFrames_0000(1);
  SCNF_WaitFrames_0000(1);
  DSWK_DoorUtils(DOOR_CMD_SET_STATE_1);
  script_10DE3();
  task_1AC = 0;
  task_1B0 = 0;
  script_10F44();
  script_10E97();
  Script_motiLoad(Script_GetSceneID(), SHEN_NAME('FREE'), "M_MOBJ.BIN");
  Script_facemotUtils(CHARA_AKIR, 1, 0);
  Script_CycleManagerUtils(CYCLE_CMD_SETUP, "scene/01/D000", (int32_t)"SCRTEST.BIN", 0x4E424344);
  Script_LoadVoiceFile(1, "SA1079");
  script_DF9E();
  Script_scenCreateTask(script_108F8, 1, CHARA_AKIR);
  Script_scenCreateTask(script_10CAB, 0);
  Script_scenCreateTask(script_10A91, 0);
  Script_scenCreateTask(script_BEB4, 0);
  Script_SetShadowFlags(CHARA_AKIR, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 0);
  Script_uiUtils(0, 56, 0);
  Script_CycleManagerUtils(CYCLE_CMD_RESET_LOAD_SETTLE);
  for (;;) {
    if (Script_CycleManagerUtils(CYCLE_CMD_TICK_LOAD_SETTLE_COUNT) == 0) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_cdttLoadScrUtils(1, 0);
  Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_shenClearFreeState(1);
  Script_ryoControl(RYO_RestoreControl);
  Script_ryoSleepUtils(2, 1);
  return;
}

static void script_10CA8(void) {
  return;
}

__attribute__((scn_string_bindings(0, data_14EF0, 1, data_14EF5, 2, data_14EFA, 3, data_14EFF, 4, data_14F04)))
static int32_t script_10CAB(void) {
  int32_t local_FC;
  for (;;) {
    SCNF_WaitFrames_0000(1);
    if (Script_IsPlayerAllowed(0)) {
      if (Script_SetTaskDataValue(CHARA_AKIR, &data_14ED8, &data_14EE4, 0)) {
      } else {
        break;
      }
    }
  }
  Script_ryoSleepUtils(2, 0);
  Script_shenClearFreeState(0);
  Script_ryoControl(PCCTL_RequestNewMode, 3);
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_executeAicaCommand(2720, 1, 30);
  Script_executeAicaCommand(2720, 2, 30);
  Script_executeAicaCommand(2720, 3, 30);
  Script_filmCheckOrSetTransitionUtil(10, 0, 0, 0, 0, 255, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  local_FC = task_1AC;
  switch (local_FC) {
    case 0:
      Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "DCB0");
      break;
    case 1:
      Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "DCB1");
      break;
    case 2:
      Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "DCB2");
      break;
    case 3:
      Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "DCB3");
      break;
    case 4:
      Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "DCB4");
      break;
  }
  Script_shenRequestGameModuleTransition(Script_GetSceneID(), SHEN_NAME('D000'), 15);
  end:;
}

static void script_10DE3(void) {
  int32_t unused_E0[8];
  Script_GetSetWeatherSettings(WEATHER_SETTINGS_SET_DISABLE_WEATHER, 1);
  Script_WeatherUtils_0(WeatherDataUtil_ApplyCurrentWeatherOrEnableDateTimeRead, 0);
  Script_WeatherTaskUtil(InitTaskData);
  Script_WeatherUtils(WeatherScript_ApplyWccpClipSet1, 3.5292f, 1.5826f, -3.729f, 1.8887f, -3.8602f, -2.8944f, 3.579f, -2.4515f, -0.4f, 2.7f);
  Script_WeatherTaskUtil(ToggleFogTask, 0);
  Script_WeatherTaskUtil(ToggleSceneLightingFlags, 0);
  Script_SetGlobalSceneSlotValue34(1, 1);
  Script_SetupLightFromTable(0);
  Script_PREP_Related(-1, 1);
  Script_PREP_Related(1, 0);
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 1);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 50.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 40.0f);
  Script_SetMapFlags(0, 0);
  return;
}

__attribute__((scn_string_bindings(0, data_14F09, 1, data_14F19, 2, data_14F1E, 3, data_14F2E, 4, data_14F33, 5, data_14F43, 6, data_14F48, 7, data_14F58, 8, data_14F5D, 9, data_14F6D)))
static void script_10E97(void) {
  int32_t local_FC;
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
  local_FC = task_1AC;
  switch (local_FC) {
    case 0:
      script_111A7("/scene/01/DCBN/", "DCB0");
      break;
    case 1:
      script_111A7("/scene/01/DCBN/", "DCB1");
      break;
    case 2:
      script_111A7("/scene/01/DCBN/", "DCB2");
      break;
    case 3:
      script_111A7("/scene/01/DCBN/", "DCB3");
      break;
    case 4:
      script_111A7("/scene/01/DCBN/", "DCB4");
      break;
  }
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 1);
  return;
}

static void script_10F44(void) {
  int32_t local_FC;
  Script_ReturnTimeDate(&task_1A0);
  task_1B4 = 0;
  task_1B8 = 0;
  task_1E4 = 0x4F4E5248;
  if (((Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_DJAZ_BALL_EVENT_SUCCESS) == 1) & (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 196) == 0)) | ((Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_NOZOMI_KIDNAPPED) == 1) & (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 527) == 0))) {
    task_1B4 = 1;
    task_1E4 = 0x49484F59;
  } else if (-(task_1A0.month == 1) & (-(task_1A0.day >= 1) & -(task_1A0.day <= 4))) {
    task_1B4 = 1;
    task_1E4 = 0x49484F59;
  } else {
    local_FC = task_1A0.dayOfWeek;
    switch (local_FC) {
      case 0:
        task_1B4 = 1;
        task_1E4 = 0x49484F59;
        break;
      case 1:
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
        task_1B4 = 1;
        task_1E4 = 0x49484F59;
        break;
    }
  }
  if ((-(task_1A0.month == 12) & (-(task_1A0.day == 24) | -(task_1A0.day == 25))) | (-(task_1A0.month == 1) & (-(task_1A0.day >= 1) & -(task_1A0.day <= 4))) | ((-(task_1A0.month == 2) | -(task_1A0.month == 3)) & -(task_1A0.day == 14))) {
    task_1B8 = 1;
  }
  if (task_1A0.month == 12) {
    task_1AC = 0;
  } else if (task_1A0.month == 1) {
    task_1AC = 31;
  } else if (task_1A0.month == 2) {
    task_1AC = 62;
  } else if (task_1A0.month == 3) {
    task_1AC = 90;
  } else {
    task_1AC = 121;
  }
  task_1AC = task_1AC + task_1A0.day + 1;
  task_1AC = task_1AC / 7;
  task_1AC = task_1AC % 5;
  return;
}

static void script_1116A(void) {
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_11184(const char *arg0, const char *arg1) {
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, arg0, arg1) == 0) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_111A7(const char *arg0, const char *arg1) {
  if (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 5, arg1)) {
  } else {
    script_11184(arg0, arg1);
    script_1116A();
  }
  return;
}
