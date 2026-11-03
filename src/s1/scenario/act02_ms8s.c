#include "act02_ms8s.h"

static void script_0030(void);
static void script_0168(int32_t arg0);
static void script_0174(int32_t arg0);
static void script_0180(int32_t arg0);
static void script_027C(int32_t arg0);
static void script_0293(int32_t arg0);
static void script_02D4(int32_t arg0);
static void script_0318(int32_t arg0);
static void script_0396(void);
static int32_t script_0415(int32_t arg0);
static void script_0433(float arg0);
static int32_t script_0452(int32_t arg0, int32_t arg1);
static void script_04CA(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4);
static int32_t script_0A7C(int32_t arg0);
static void script_0B4D(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_0B5D(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_0C61(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_0E03(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_0F64(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_10C5(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_1363(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_1431(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_1498(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_14FF(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_1546(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_167C(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_1F3D(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_205B(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_2720(int32_t arg0, int32_t arg1);
static void script_27F6(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_3709(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_3775(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_3C2F(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_42E9(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_5BC9(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_62EA(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_6A74(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_6B4C(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_6FC3(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_7F56(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_83A0(int32_t arg0, DSWKDoorEntry *arg1, int32_t *arg2, DoorResourceDescriptor *arg3, MapObjectRecord *arg4, int32_t arg5, int32_t arg6);
static void script_852C(DSWKDoorEntry *arg0, int32_t *arg1, DoorResourceDescriptor *arg2, MapObjectRecord *arg3, int32_t arg4);
static void script_8560(void);
static void script_85C9(int32_t arg0, int32_t arg1);
static void script_8666(int32_t arg0, int32_t arg1);
static void script_86A0(void);
static void script_8703(void);
static void script_8766(int32_t arg0);
static void script_879D(int32_t arg0);
static void script_87D4(int32_t arg0);
static void script_8A16(int32_t arg0);
static void script_8A5F(int32_t arg0);
static void script_8AA8(int32_t arg0, int32_t arg1);
static void script_8B1E(int32_t arg0);
static void script_8B2C(int32_t arg0);
static void script_8B3A(int32_t arg0);
static void script_8C01(void);
static void script_8C0D(void);
static void script_8C19(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5, int32_t arg6);
static int32_t script_8C5C(void);
static int8_t script_8D6D(int32_t arg0, int32_t arg1, int8_t arg2);
static void script_8E05(int32_t arg0);
static void script_8FD4(int32_t arg0);
static int32_t script_9005(int32_t arg0, int32_t arg1, int32_t arg2, float arg3, float arg4, int32_t arg5, int32_t arg6);
static void script_96CE(int32_t arg0, int32_t arg1);
static void script_9700(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_9870(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_9A2C(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_9BE0(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_9DF6(int32_t arg0, int32_t arg1);
static void script_9EC0(int32_t arg0, int32_t arg1);
static void script_9F21(int32_t arg0);
static void script_A0C6(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_A15E(int32_t arg0, int32_t arg1);
static void script_A240(int32_t arg0);
static int32_t script_A297(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_AB99(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_AC8C(int32_t arg0, int32_t arg1);
static void script_ADCB(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_AF7A(int32_t arg0, int32_t arg1, int32_t arg2);
static int8_t script_B073(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5, int32_t arg6);
static int8_t script_B563(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_BB2F(int32_t arg0, int32_t arg1, float arg2, float arg3, int32_t arg4, int32_t arg5);
static int32_t script_BD6E(int32_t arg0, int32_t arg1, float arg2, float arg3, int32_t arg4, int32_t arg5, int32_t arg6);
static int32_t script_BFFD(int32_t arg0, int32_t arg1, float arg2, float arg3, int32_t arg4, int32_t arg5, int32_t arg6);
static void script_C30A(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_C538(int32_t arg0);
static void script_C5D7(float arg0);
static void script_C666(float arg0);
static void script_C6F5(void);
static int32_t script_C72C(int32_t arg0, int32_t arg1);
static void script_C863(int32_t arg0);
static void script_C8AA(int32_t arg0);
static void script_C8B4(int32_t arg0);
static void script_C8BE(void);
static void script_C8C5(int32_t arg0);
static void script_C9A0(int32_t arg0);
static void script_CA7B(int32_t arg0);
static void script_CB56(int32_t arg0, int32_t arg1);
static void script_CC4C(int32_t arg0, int32_t arg1);
static int32_t script_CD42(int32_t arg0, int32_t arg1);
static int32_t script_CE6A(int32_t arg0, int32_t arg1);
static void script_CF9B(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_D020(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_D070(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_D0BF(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_D25C(void);
static void script_D37C(void);
static void script_D5D6(int32_t arg0);
static void script_D732(int32_t arg0, int32_t arg1);
static void script_E179(int32_t arg0);
static int32_t script_E54E(int32_t arg0);
static void script_E5B3(int32_t arg0);
static void script_E72A(void);
static void script_E734(void);
static void script_E73E(void);
static void script_E748(int32_t arg0);
static void script_E752(int32_t arg0);
static void script_E8F8(void);
static int32_t script_EB70(void);
static int32_t script_EBC5(void);
static void script_EC8E(int32_t arg0);
static void script_F14D(int32_t arg0);
static int32_t script_F334(void);
static void script_F368(void);
static int32_t script_F380(void);
static int32_t script_F3A3(void);
static int32_t script_F6FB(void);
static int32_t script_F73D(void);
static void script_F7CE(void);
static void script_F9B9(int32_t arg0);
static int32_t script_FAD3(void);
static int32_t script_FAFB(void);
static void script_FC17(int32_t arg0);
static void script_101F7(void);
static void script_10309(void);
static void script_10381(void);
void script_10409(void);
static void script_106CC(const char *arg0, const char *arg1);
static void script_10706(int32_t arg0, float arg1, float arg2, float arg3, int32_t arg4);
static void script_10746(void);
static void script_1075F(int32_t arg0);
static void script_10777(void);
static int32_t script_10937(void);

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
static GameDateTime task_1FC;
static uint8_t task_204;
static int8_t task_205;
static int8_t task_206;
static int8_t task_207;
static int8_t task_208;
static int8_t task_209;
static int8_t task_20A;
static int8_t task_20B;
static int8_t task_20C;
static uint8_t task_20D;
static uint16_t task_20E;
static int8_t task_210;
static uint8_t task_211;
static uint16_t task_212;
static int32_t task_214;
static int32_t task_218;
static int32_t task_21C;
static int32_t task_220;
static int32_t task_224;
static int32_t task_228;
static int32_t task_22C;
static int32_t task_230;
static int32_t task_234;
static Vector3 task_238;
static Vector3i task_244;
static int32_t task_250;
static int32_t task_254;
static int32_t task_258;
static int32_t task_25C;
static int32_t task_260;
static int32_t task_264;
static int32_t task_268;
static int32_t task_26C;
static int32_t task_270;
static int32_t task_274;
static int32_t task_278;
static int8_t task_27C;
static uint8_t task_27D;
static uint16_t task_27E;
static int8_t task_280;
static uint8_t task_281;
static uint16_t task_282;
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
static int32_t task_2D4[100];
static int32_t task_464;
static int32_t task_468;
static int32_t task_46C;
static int32_t task_470;
static int32_t task_474;
static int32_t task_478;
static int32_t task_47C;
static int32_t task_480;
static int32_t task_484;
static int32_t task_488;
static int32_t task_48C;
static int32_t task_490;
static int32_t task_494;
static int32_t task_498;
static int32_t task_49C;
static int8_t task_4A0;
static uint8_t task_4A1;
static uint16_t task_4A2;
static const float data_10A4C[3] = {1.0f, 1.0f, 1.0f};
static const int32_t data_10A58[4] = {16777215, 16777215, 16777215, 16777215};
static const float data_10A68[3] = {20.0f, 800.0f, 1.0f};
static const int32_t data_10A74 = -1;
static const uint32_t data_10A78 = 380;
static const int32_t data_10A7C[6] = {1, 390, 0, 1018, 0, -1};
static const uint32_t data_10A94 = 510;
static const int32_t data_10A98[6] = {1, 525, 0, 1020, 0, -1};
static const uint32_t data_10AB0 = 150;
static const int32_t data_10AB4[6] = {1, 160, 0, 1019, 0, -1};
static const uint32_t data_10ACC = 530;
static const int32_t data_10AD0[6] = {1, 970, 0, 1014, 0, -1};
static const uint32_t data_10AE8 = 911;
static const int32_t data_10AEC[6] = {1, 155, 0, 1015, 0, -1};
static const char data_10B04[7] __attribute__((scn_string_literal)) = "FREE02";
static const char data_10B0B[7] __attribute__((scn_string_literal)) = "FREE03";
static const char data_10B12[7] __attribute__((scn_string_literal)) = "FREE04";
static const char data_10B19[7] __attribute__((scn_string_literal)) = "FREE01";
static DoorMotionSelectorBlock data_10B20[38] = {
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
static ScnDoorMotionDescriptor data_11110[72] = {
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
static uint32_t data_11B30 = 0xFFFFFFFF;
typedef struct Row_11B34 { float w0; float w1; float w2; float w3; int32_t w4; } Row_11B34;
static const Row_11B34 data_11B34[7] = {
  {0.0f, 0.0f, 0.0f, 0.0f, -1},
  {0.0f, 0.0f, 0.0f, 0.0f, -1},
  {0.0f, 0.0f, 0.0f, 0.0f, 1534},
  {26.0f, 55.0f, 45.0f, 75.0f, 1555},
  {61.0f, 146.0f, 45.0f, 50.0f, 1554},
  {59.0f, 124.0f, 45.0f, 70.0f, 1543},
  {33.0f, 49.0f, 45.0f, 50.0f, 1543}
};
static const float data_11BC0[4] = {33.0f, 49.0f, 50.0f, 50.0f};
static void (*data_11BD0)(int32_t, int32_t, int32_t) = script_0B4D;
static void (*const data_11BD4)(int32_t, int32_t, int32_t, int32_t) = script_0B5D;
static void (*const data_11BD8)(int32_t, int32_t, int32_t, int32_t) = script_0B5D;
static void (*const data_11BDC)(int32_t, int32_t, int32_t, int32_t) = script_0B5D;
static void (*const data_11BE0)(int32_t, int32_t, int32_t, int32_t) = script_0B5D;
static void (*const data_11BE4)(int32_t, int32_t, int32_t, int32_t) = script_0B5D;
static void (*const data_11BE8)(int32_t, int32_t, int32_t, int32_t) = script_0B5D;
static void (*const data_11BEC)(int32_t, int32_t, int32_t, int32_t) = script_0B5D;
static void (*const data_11BF0)(int32_t, int32_t, int32_t, int32_t) = script_0B5D;
static void (*const data_11BF4)(int32_t, int32_t, int32_t, int32_t) = script_0E03;
static void (*const data_11BF8)(int32_t, int32_t, int32_t, int32_t) = script_0F64;
static void (*const data_11BFC)(int32_t, int32_t, int32_t, int32_t) = script_1431;
static void (*const data_11C00)(int32_t, int32_t, int32_t, int32_t) = script_1498;
static void (*const data_11C04)(int32_t, int32_t, int32_t, int32_t) = script_0C61;
static void (*const data_11C08)(int32_t, int32_t, int32_t, int32_t) = script_0C61;
static void (*const data_11C0C)(int32_t, int32_t, int32_t, int32_t) = script_10C5;
static void (*const data_11C10)(int32_t, int32_t, int32_t, int32_t) = script_1363;
static void (*const data_11C14)(int32_t, int32_t, int32_t, int32_t) = script_14FF;
static void (*const data_11C18)(int32_t, int32_t, int32_t, int32_t) = script_0E03;
static void (*const data_11C1C)(int32_t, int32_t, int32_t, int32_t) = script_0F64;
static void (*const data_11C20)(int32_t, int32_t, int32_t, int32_t) = script_10C5;
static void (*const data_11C24)(int32_t, int32_t, int32_t, int32_t) = script_0E03;
static void (*const data_11C28)(int32_t, int32_t, int32_t, int32_t) = script_0F64;
static void (*const data_11C2C)(int32_t, int32_t, int32_t, int32_t) = script_10C5;
static void (*const data_11C30)(int32_t, int32_t, int32_t, int32_t) = script_1546;
static void (*const data_11C34)(int32_t, int32_t, int32_t, int32_t) = script_1546;
static void (*const data_11C38)(int32_t, int32_t, int32_t, int32_t) = script_1546;
static void (*const data_11C3C)(int32_t, int32_t, int32_t, int32_t) = script_1546;
static void (*const data_11C40)(int32_t, int32_t, int32_t, int32_t) = script_167C;
static void (*const data_11C44)(int32_t, int32_t, int32_t, int32_t) = script_1F3D;
static void (*const data_11C48)(int32_t, int32_t, int32_t, int32_t) = script_0B5D;
static void (*const data_11C4C)(int32_t, int32_t, int32_t, int32_t) = script_0B5D;
static void (*const data_11C50)(int32_t, int32_t, int32_t, int32_t) = script_0B5D;
static void (*const data_11C54)(int32_t, int32_t, int32_t, int32_t) = script_0B5D;
static void (*const data_11C58)(int32_t, int32_t, int32_t, int32_t) = script_0B5D;
static void (*const data_11C5C)(int32_t, int32_t, int32_t, int32_t) = script_0B5D;
static void (*const data_11C60)(int32_t, int32_t, int32_t, int32_t) = script_0B5D;
static void (*const data_11C64)(int32_t, int32_t, int32_t, int32_t) = script_0B5D;
static const char data_11C68[46] __attribute__((scn_string_literal)) = "Door Index = %d : call_type = %d : type = %d\012";
static const char data_11C96[43] __attribute__((scn_string_literal)) = "Door Open Motion = DEB_AKESIME_FUSUMA_L_F\012";
static const char data_11CC1[43] __attribute__((scn_string_literal)) = "Door Open Motion = DEB_AKESIME_FUSUMA_R_F\012";
static const char data_11CEC[42] __attribute__((scn_string_literal)) = "Door Open Motion = DEB_HIKISIME_DOOR_L_F\012";
static const char data_11D16[42] __attribute__((scn_string_literal)) = "Door Open Motion = DEB_HIKISIME_DOOR_R_F\012";
static const char data_11D40[41] __attribute__((scn_string_literal)) = "Door Open Motion = DEB_OSISIME_DOOR_L_F\012";
static const char data_11D69[41] __attribute__((scn_string_literal)) = "Door Open Motion = DEB_OSISIME_DOOR_R_F\012";
static const char data_11D92[43] __attribute__((scn_string_literal)) = "Door Open Motion = GAK_AKESIME_FUSUMA_L_F\012";
static const char data_11DBD[43] __attribute__((scn_string_literal)) = "Door Open Motion = GAK_AKESIME_FUSUMA_R_F\012";
static const char data_11DE8[42] __attribute__((scn_string_literal)) = "Door Open Motion = GAK_HIKISIME_DOOR_L_F\012";
static const char data_11E12[42] __attribute__((scn_string_literal)) = "Door Open Motion = GAK_HIKISIME_DOOR_R_F\012";
static const char data_11E3C[41] __attribute__((scn_string_literal)) = "Door Open Motion = GAK_OSISIME_DOOR_L_F\012";
static const char data_11E65[41] __attribute__((scn_string_literal)) = "Door Open Motion = GAK_OSISIME_DOOR_R_F\012";
static const char data_11E8E[43] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_AKESIME_FUSUMA_L_F\012";
static const char data_11EB9[43] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_AKESIME_FUSUMA_R_F\012";
static const char data_11EE4[42] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_HIKISIME_DOOR_L_F\012";
static const char data_11F0E[42] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_HIKISIME_DOOR_R_F\012";
static const char data_11F38[41] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_OSISIME_DOOR_L_F\012";
static const char data_11F61[41] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_OSISIME_DOOR_R_F\012";
static const char data_11F8A[43] __attribute__((scn_string_literal)) = "Door Open Motion = KOD_AKESIME_FUSUMA_L_F\012";
static const char data_11FB5[43] __attribute__((scn_string_literal)) = "Door Open Motion = KOD_AKESIME_FUSUMA_R_F\012";
static const char data_11FE0[42] __attribute__((scn_string_literal)) = "Door Open Motion = KOD_HIKISIME_DOOR_L_F\012";
static const char data_1200A[42] __attribute__((scn_string_literal)) = "Door Open Motion = KOD_HIKISIME_DOOR_R_F\012";
static const char data_12034[41] __attribute__((scn_string_literal)) = "Door Open Motion = KOD_OSISIME_DOOR_L_F\012";
static const char data_1205D[41] __attribute__((scn_string_literal)) = "Door Open Motion = KOD_OSISIME_DOOR_R_F\012";
static const char data_12086[43] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_AKESIME_FUSUMA_L_F\012";
static const char data_120B1[43] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_AKESIME_FUSUMA_R_F\012";
static const char data_120DC[42] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_HIKISIME_DOOR_L_F\012";
static const char data_12106[42] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_HIKISIME_DOOR_R_F\012";
static const char data_12130[41] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_OSISIME_DOOR_L_F\012";
static const char data_12159[41] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_OSISIME_DOOR_R_F\012";
static const char data_12182[43] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_AKESIME_FUSUMA_L_F\012";
static const char data_121AD[43] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_AKESIME_FUSUMA_R_F\012";
static const char data_121D8[42] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_HIKISIME_DOOR_L_F\012";
static const char data_12202[42] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_HIKISIME_DOOR_R_F\012";
static const char data_1222C[41] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_OSISIME_DOOR_L_F\012";
static const char data_12255[41] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_OSISIME_DOOR_R_F\012";
static const char data_1227E[43] __attribute__((scn_string_literal)) = "Door Open Motion = SIN_AKESIME_FUSUMA_L_F\012";
static const char data_122A9[43] __attribute__((scn_string_literal)) = "Door Open Motion = SIN_AKESIME_FUSUMA_R_F\012";
static const char data_122D4[42] __attribute__((scn_string_literal)) = "Door Open Motion = SIN_HIKISIME_DOOR_L_F\012";
static const char data_122FE[42] __attribute__((scn_string_literal)) = "Door Open Motion = SIN_HIKISIME_DOOR_R_F\012";
static const char data_12328[41] __attribute__((scn_string_literal)) = "Door Open Motion = SIN_OSISIME_DOOR_L_F\012";
static const char data_12351[41] __attribute__((scn_string_literal)) = "Door Open Motion = SIN_OSISIME_DOOR_R_F\012";
static const char data_1237A[43] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_AKESIME_FUSUMA_L_F\012";
static const char data_123A5[43] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_AKESIME_FUSUMA_R_F\012";
static const char data_123D0[42] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_HIKISIME_DOOR_L_F\012";
static const char data_123FA[42] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_HIKISIME_DOOR_R_F\012";
static const char data_12424[41] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_OSISIME_DOOR_L_F\012";
static const char data_1244D[41] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_OSISIME_DOOR_R_F\012";
static const char data_12476[43] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_AKESIME_FUSUMA_L_F\012";
static const char data_124A1[43] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_AKESIME_FUSUMA_R_F\012";
static const char data_124CC[42] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_HIKISIME_DOOR_L_F\012";
static const char data_124F6[42] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_HIKISIME_DOOR_R_F\012";
static const char data_12520[41] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_OSISIME_DOOR_L_F\012";
static const char data_12549[41] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_OSISIME_DOOR_R_F\012";
static const char data_12572[44] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_OSISIME_DOOR_AP_L_F\012";
static const char data_1259E[44] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_OSISIME_DOOR_AP_L_F\012";
static const char data_125CA[44] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_OSISIME_DOOR_AP_L_F\012";
static const char data_125F6[44] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_OSISIME_DOOR_AP_L_F\012";
static const char data_12622[44] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_OSISIME_DOOR_AP_L_F\012";
static const char data_1264E[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_1266A[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_12686[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_126A2[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_126BE[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_126DA[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_126F6[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_12712[13] __attribute__((scn_string_literal)) = "open-move!!\012";
static const char data_1271F[11] __attribute__((scn_string_literal)) = "M_MDOR.BIN";
static const char data_1272A[] __attribute__((scn_string_literal)) = "This Charactor Work is NULL!!\n";
static const char data_12749[22] __attribute__((scn_string_literal)) = "手動ドア開け番号指定\012";
static const char data_1275F[34] __attribute__((scn_string_literal)) = "無限ループになりそうなので抜ける\012";
static const char data_12781[] __attribute__((scn_string_literal)) = "Don't make BOX Buffer!!\n";
static const char data_1279A[] __attribute__((scn_string_literal)) = "Don't make BOX Buffer!!\n";
static const char data_127B3[15] __attribute__((scn_string_literal)) = "Door System : ";
static const char data_127C2[15] __attribute__((scn_string_literal)) = "Door System : ";
static const char data_127D1[18] __attribute__((scn_string_literal)) = "Door Box is ON!!\012";
static const char data_127E3[19] __attribute__((scn_string_literal)) = "Door Box is OFF!!\012";
static const char data_127F6[18] __attribute__((scn_string_literal)) = "Door Box is ON!!\012";
static const char data_12808[20] __attribute__((scn_string_literal)) = "Door Box is OFF!!\012";
static const DSWKDoorEntry data_1281C[3] = {
  {.slot = 0, .flags = 0, .ctrl_mask_pair = 0x640064U, .coarse_metric = 0.0f, .selection_metric = 0.0f, .view_ang_delta = 0, .group = 0, .global1 = 0xFFFFFFFFU, .global2 = 0xFFFFFFFFU, .global3 = 0xFFFFFFFFU, .sndCommandId = 0xFFFFFFFFU, .global5 = 0xFFFFFFFFU, .global6 = 0xFFFFFFFFU},
  {.slot = 2, .flags = 0, .ctrl_mask_pair = 0x640064U, .coarse_metric = 0.0f, .selection_metric = 0.0f, .view_ang_delta = 0, .group = 0, .global1 = 0xFFFFFFFFU, .global2 = 0xFFFFFFFFU, .global3 = 0xFFFFFFFFU, .sndCommandId = 0xFFFFFFFFU, .global5 = 0xFFFFFFFFU, .global6 = 0xFFFFFFFFU},
  {.slot = 4, .flags = 0, .ctrl_mask_pair = 0x640064U, .coarse_metric = 0.0f, .selection_metric = 0.0f, .view_ang_delta = 0, .group = 0, .global1 = 0xFFFFFFFFU, .global2 = 0xFFFFFFFFU, .global3 = 0xFFFFFFFFU, .sndCommandId = 0xFFFFFFFFU, .global5 = 0xFFFFFFFFU, .global6 = 0xFFFFFFFFU}
};
static const int32_t data_128B8[13] = {-1, 0, 6553700, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1};
static const uint8_t data_128EC[4] = {0};
static const int32_t data_128F0[5] = {-1, 1, -1, 2, -1};
static char data_12904[] = "MS8S_DOOR";
static char data_1290E[] = "MS8S_DOOR";
static char data_12918[] = "MS8S_DOOR";
static const uint8_t data_12922[2] = {0};
static char *const data_12924 = data_12904;
static const int32_t data_12928[3] = {31, 1966763, 2032299};
static char *const data_12934 = data_1290E;
static const int32_t data_12938[3] = {31, 1966763, 2032299};
static char *const data_12944 = data_12918;
static const int32_t data_12948[3] = {31, 1966763, 2032299};
static const MapObjectRecord data_12954[3] = {
  {.kind = 2, .resourceIndex = 0, .scale = {1.0f, 1.0f, 1.0f}, .pos = {22.165022f, 4.28f, 139.939743f}, .yawFixed16 = -8191},
  {.kind = 2, .resourceIndex = 1, .scale = {1.0f, 1.0f, 1.0f}, .pos = {25.98341f, 4.28f, 143.758133f}, .yawFixed16 = -8191},
  {.kind = 2, .resourceIndex = 2, .scale = {1.0f, 1.0f, 1.0f}, .pos = {13.343f, 4.28f, 131.130524f}, .yawFixed16 = -8192}
};
static const char data_129C0[10] __attribute__((scn_string_literal)) = "Kaidan : ";
static const char data_129CA[32] __attribute__((scn_string_literal)) = "========== initializing kaidan\012";
static const char data_129EA[48] __attribute__((scn_string_literal)) = "please ENDOFKDATA use can't start shuki kaidan\012";
static const char data_12A1A[] __attribute__((scn_string_literal)) = "Kaidan Function Error 1\n";
static const char data_12A33[] __attribute__((scn_string_literal)) = "Kaidan Function Error 2\n";
static const char data_12A4C[] __attribute__((scn_string_literal)) = "Kaidan Data Error\n";
static const char data_12A5F[10] __attribute__((scn_string_literal)) = "Kaidan : ";
static const char data_12A69[28] __attribute__((scn_string_literal)) = "wait ... shuki shuki shuki\012";
static const char data_12A85[10] __attribute__((scn_string_literal)) = "Kaidan : ";
static const char data_12A8F[24] __attribute__((scn_string_literal)) = "KAIDAN : Warning coli!\012";
static const char data_12AA7[24] __attribute__((scn_string_literal)) = "KAIDAN : Warning coli!\012";
static const char data_12ABF[24] __attribute__((scn_string_literal)) = "KAIDAN : Warning coli!\012";
static const char data_12AD7[24] __attribute__((scn_string_literal)) = "KAIDAN : Warning coli!\012";
static const char data_12AEF[24] __attribute__((scn_string_literal)) = "KAIDAN : Warning coli!\012";
static const char data_12B07[24] __attribute__((scn_string_literal)) = "KAIDAN : Warning setY!\012";
static const char data_12B1F[24] __attribute__((scn_string_literal)) = "KAIDAN : Warning setY!\012";
static const char data_12B37[13] __attribute__((scn_string_literal)) = "Kaidan : ";
static const uint32_t data_12B44 = 0x41090F28;
typedef struct Row_12B48 { float w0; uint32_t w1; int32_t w2; int32_t w3; int32_t w4; float w5; } Row_12B48;
static const Row_12B48 data_12B48[2] = {
  {0.225f, 0x4317A183U, 40960, 1048832, 19, 12.3844f},
  {4.275f, 0x431B7190U, 8192, 264, 19, 100000.0f}
};
static const uint8_t data_12B78[8] = {0};
static const float data_12B80[2] = {0.5f, 0.0f};
static const int32_t data_12B88 = 1;
static const float data_12B8C = 0.5f;
static const int32_t data_12B90 = 3;
static uint32_t data_12B94 = 131073;
static const shen_name data_12B98 = SHEN_NAME('TB00');
static const uint32_t data_12B9C[6] = {0x55552000U, 0, 0, 0, 0, 0};
static const int32_t data_12BB4 = 131073;
static const shen_name data_12BB8 = SHEN_NAME('TB02');
static const uint32_t data_12BBC[6] = {0x5555E000U, 0, 0, 0, 0, 0};
static const int32_t data_12BD4 = 131073;
static const shen_name data_12BD8 = SHEN_NAME('TB10');
static const uint32_t data_12BDC[6] = {0x5555A000U, 0, 0, 0, 0, 0};
static const int32_t data_12BF4 = 131073;
static const shen_name data_12BF8 = SHEN_NAME('TB11');
static const uint32_t data_12BFC[6] = {0x5555E000U, 0, 0, 0, 0, 0};
static const int32_t data_12C14 = 131073;
static const shen_name data_12C18 = SHEN_NAME('TB12');
static const uint32_t data_12C1C[6] = {0x5555E000U, 0, 0, 0, 0, 0};
static const int32_t data_12C34 = 1;
static const shen_name data_12C38 = SHEN_NAME('ZAR0');
static const uint32_t data_12C3C[6] = {0x55552000U, 0, 0, 0, 0, 0};
static const int32_t data_12C54 = 131073;
static const shen_name data_12C58 = SHEN_NAME('PEN0');
static const uint32_t data_12C5C[6] = {0x5555E000U, 0, 0, 0, 0, 0};
static const int32_t data_12C74 = 131074;
static const shen_name data_12C78 = SHEN_NAME('PEN1');
static const float data_12C7C[6] = {40128.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
static const int32_t data_12C94 = 131073;
static const shen_name data_12C98 = SHEN_NAME('YNM0');
static const uint32_t data_12C9C[6] = {0x55552000U, 0, 0, 0, 0, 0};
static const int32_t data_12CB4 = 131073;
static const shen_name data_12CB8 = SHEN_NAME('YNM1');
static const uint32_t data_12CBC[6] = {0x55556000U, 0, 0, 0, 0, 0};
static const int32_t data_12CD4 = 131073;
static const shen_name data_12CD8 = SHEN_NAME('YNM2');
static const uint32_t data_12CDC[6] = {0x5555E000U, 0, 0, 0, 0, 0};
static const int32_t data_12CF4 = 131074;
static const shen_name data_12CF8 = SHEN_NAME('YNM4');
static const float data_12CFC[6] = {40128.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
static const int32_t data_12D14 = 131074;
static const shen_name data_12D18 = SHEN_NAME('FUT0');
static const float data_12D1C[6] = {40128.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
static const int32_t data_12D34 = 131073;
static const shen_name data_12D38 = SHEN_NAME('FUT1');
static const uint32_t data_12D3C[6] = {0x5555E000U, 0, 0, 0, 0, 0};
static const int32_t data_12D54 = 2;
static const shen_name data_12D58 = SHEN_NAME('SHO0');
static const float data_12D5C[6] = {40128.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
static const int32_t data_12D74 = 1;
static const shen_name data_12D78 = SHEN_NAME('SHO1');
static const uint32_t data_12D7C[6] = {0x5555E000U, 0, 0, 0, 0, 0};
static const int32_t data_12D94[2] = {-65536, 0};
static const uint32_t data_12D9C = 0x8E388000U;
static const float data_12DA0[5] = {22.85f, 0.61f, 143.76f, 0.0f, 0.0f};
static const int32_t data_12DB4[2] = {-65536, 1};
static const uint32_t data_12DBC = 0xA38EE000U;
static const float data_12DC0[5] = {20.13f, 0.69f, 133.31f, 0.0f, 0.0f};
static const int32_t data_12DD4[2] = {-65536, 2};
static const uint32_t data_12DDC = 0x5555A000U;
static const float data_12DE0[5] = {28.51f, 1.1f, 141.42f, 0.0f, 0.0f};
static const int32_t data_12DF4[2] = {-65536, 3};
static const uint32_t data_12DFC = 0x55552000U;
static const float data_12E00[5] = {15.45f, 1.4f, 136.14f, 0.0f, 0.0f};
static const int32_t data_12E14[2] = {-65536, 4};
static const uint32_t data_12E1C = 0x8E38A000U;
static const float data_12E20[5] = {22.1f, 1.16f, 144.06f, 0.0f, 0.0f};
static const int32_t data_12E34[2] = {-65536, 5};
static const uint32_t data_12E3C = 0x5555A000U;
static const float data_12E40[5] = {22.5f, 1.59f, 135.18f, 0.0f, 0.0f};
static const int32_t data_12E54[2] = {-65536, 6};
static const uint32_t data_12E5C = 0x5555A000U;
static const float data_12E60[5] = {23.48f, 5.37f, 146.66f, 0.0f, 0.0f};
static const int32_t data_12E74[2] = {-65536, 10};
static const uint32_t data_12E7C = 0x5555E000U;
static const float data_12E80[5] = {21.84f, 5.88f, 139.59f, 0.0f, 0.0f};
static const int32_t data_12E94[2] = {-65536, 7};
static const uint32_t data_12E9C = 0x5555E000U;
static const float data_12EA0[5] = {25.68f, 5.88f, 143.46f, 0.0f, 0.0f};
static const int32_t data_12EB4[2] = {-65535, 8};
static const uint32_t data_12EBC = 0x5555E000U;
static const float data_12EC0[5] = {28.5f, 5.97f, 139.97f, 0.0f, 0.0f};
static const int32_t data_12ED4[2] = {-65535, 9};
static const uint32_t data_12EDC = 0x5555E000U;
static const float data_12EE0[5] = {26.67f, 5.97f, 138.03f, 0.0f, 0.0f};
static const int32_t data_12EF4[2] = {-65535, 14};
static const uint32_t data_12EFC = 0x5555A000U;
static const float data_12F00[5] = {28.27f, 4.63f, 141.51f, 0.0f, 0.0f};
static const int32_t data_12F14[2] = {-65535, 15};
static const uint32_t data_12F1C = 0x5555A000U;
static const float data_12F20[5] = {27.24f, 6.2f, 142.6f, 0.0f, 0.0f};
static const int32_t data_12F34[2] = {-65534, 11};
static const uint32_t data_12F3C = 0x5555E000U;
static const float data_12F40[5] = {23.61f, 6.18f, 135.64f, 0.0f, 0.0f};
static const int32_t data_12F54[2] = {-65534, 12};
static const uint32_t data_12F5C = 0x5555E000U;
static const float data_12F60[5] = {22.11f, 6.18f, 134.18f, 0.0f, 0.0f};
static const int32_t data_12F74[2] = {-65534, 13};
static const uint32_t data_12F7C = 0x55552000U;
static const float data_12F80[5] = {21.35f, 6.18f, 136.15f, 0.0f, 0.0f};
static const int32_t data_12F94 = 65535;
static uint32_t data_12F98 = 0xFFFFFFFF;
static uint8_t data_12F9C[12] = {0};
static const int32_t data_12FA8 = 2033065;
static const shen_name data_12FAC[8] = {
  SHEN_NAME('TB00'), SHEN_NAME('TB01'), SHEN_NAME('TB02'), SHEN_NAME('TB10'), SHEN_NAME('TB11'), SHEN_NAME('TB12'), SHEN_NAME('FUT0'),
  SHEN_NAME('FUT1')
};
static const int32_t data_12FCC[5] = {-1, 0, 0, 0, 2164137};
static const shen_name data_12FE0[5] = {SHEN_NAME('YNM0'), SHEN_NAME('YNM1'), SHEN_NAME('YNM2'), SHEN_NAME('YNM3'), SHEN_NAME('YNM4')};
static const int32_t data_12FF4[5] = {-1, 0, 0, 0, 2229673};
static const shen_name data_13008[2] = {SHEN_NAME('PEN0'), SHEN_NAME('PEN1')};
static const int32_t data_13010[3] = {-1, -1, 0};
static const Vector4 data_1301C = {.x = 0.78f, .y = 0.82f, .z = 0.59f, .w = 0.46f};
static const float data_1302C[19] = {
  0.89f, 0.89f, 0.13f, 0.39f, 0.28f, 0.14f, 1.1f, 1.84f, 0.93f, 0.58f, 4.1813f, 0.996f, 0.7922f, 1000.0f, 1000.0f, 1000.0f, 1000.0f, 1000.0f, 1000.0f
};
static const char data_13078[28] __attribute__((scn_string_literal)) = "Map Start!!!!!!!!!!!!!!!!!\012";
static const char data_13094[21] __attribute__((scn_string_literal)) = "このアイテムもってる";
static const char data_130A9[43] __attribute__((scn_string_literal)) = "tansu naka = %d\012";
static const Vector3i data_130D4 = {.x = 16384, .y = 32768, .z = 0};
static const Vector3 data_130E0 = {.x = 22.271f, .y = 4.275f, .z = 139.194504f};
static const Vector3 data_130EC = {.x = 26.1152f, .y = 4.275f, .z = 142.987503f};
static const Vector3 data_130F8 = {.x = 24.890699f, .y = 4.275f, .z = 144.214493f};
static const Vector3 data_13104 = {.x = 21.0723f, .y = 4.275f, .z = 140.396194f};
static const char data_13110[11] __attribute__((scn_string_literal)) = "E1009BA007";
static const char data_1311B[11] __attribute__((scn_string_literal)) = "E1009BA008";
static const char data_13126[11] __attribute__((scn_string_literal)) = "E1009BA009";
static const char data_13131[11] __attribute__((scn_string_literal)) = "E1009BA006";
static const char data_1313C[11] __attribute__((scn_string_literal)) = "E1009BA008";
static const char data_13147[11] __attribute__((scn_string_literal)) = "E1009BA009";
static const char data_13152[11] __attribute__((scn_string_literal)) = "E1009BA022";
static const char data_1315D[11] __attribute__((scn_string_literal)) = "E1009BA023";
static const char data_13168[11] __attribute__((scn_string_literal)) = "E1009BA024";
static const char data_13173[11] __attribute__((scn_string_literal)) = "E1009BA034";
static const char data_1317E[11] __attribute__((scn_string_literal)) = "E1009BA029";
static const char data_13189[11] __attribute__((scn_string_literal)) = "E1009BA030";
static const char data_13194[11] __attribute__((scn_string_literal)) = "E1009BA035";
static const char data_1319F[11] __attribute__((scn_string_literal)) = "E1009BA036";
static const char data_131AA[11] __attribute__((scn_string_literal)) = "E1009BA037";
static const char data_131B5[11] __attribute__((scn_string_literal)) = "E1009BA038";
static const char data_131C0[11] __attribute__((scn_string_literal)) = "E1009BA036";
static const char data_131CB[11] __attribute__((scn_string_literal)) = "E1009BA039";
static const char data_131D6[11] __attribute__((scn_string_literal)) = "E1009BA040";
static const char data_131E1[11] __attribute__((scn_string_literal)) = "E1009BA026";
static const char data_131EC[11] __attribute__((scn_string_literal)) = "E1009BA041";
static const char data_131F7[11] __attribute__((scn_string_literal)) = "E1009BA009";
static const char data_13202[11] __attribute__((scn_string_literal)) = "E1009BA019";
static const char data_1320D[11] __attribute__((scn_string_literal)) = "E1009BA020";
static Vector3 data_13218 = {.x = 0.0f, .y = 0.0f, .z = 0.0f};
static Vector3i data_13224 = {.x = 0, .y = 0, .z = 0};
static const Vector3 data_13230 = {.x = 18.01f, .y = 4.275f, .z = 137.16f};
static const Vector3 data_1323C = {.x = 23.125f, .y = 0.0f, .z = 146.3f};
static const Vector3 data_13248 = {.x = 4.4257f, .y = 0.0f, .z = 140.0f};
static const Vector3 data_13254 = {.x = 22.271f, .y = 4.275f, .z = 139.194504f};
static const Vector3 data_13260 = {.x = 26.1152f, .y = 4.275f, .z = 142.987503f};
static const Vector3 data_1326C = {.x = 14.81f, .y = 5.78f, .z = 149.9f};
static const Vector3 data_13278 = {.x = 16.77f, .y = 4.275f, .z = 151.87f};
static const char data_13284[11] __attribute__((scn_string_literal)) = "E1009BA002";
static const char data_1328F[11] __attribute__((scn_string_literal)) = "E1009BA002";
static const char data_1329A[11] __attribute__((scn_string_literal)) = "E1009BA006";
static const char data_132A5[11] __attribute__((scn_string_literal)) = "E1009BA028";
static const char data_132B0[12] __attribute__((scn_string_literal)) = "E1009BA003";
static const Vector3 data_132BC = {.x = 17.69f, .y = 4.275f, .z = 136.69f};
static const Vector3 data_132C8 = {.x = 16.15f, .y = 4.275f, .z = 135.18f};
static const float data_132D4[6] = {15.15f, 4.275f, 133.48f, 14.14f, 5.8f, 132.09f};
static const Vector3 data_132EC = {.x = 16.63f, .y = 5.6f, .z = 131.87f};
static const float data_132F8[3] = {15.4f, 5.3f, 131.0f};
static const char data_13304[11] __attribute__((scn_string_literal)) = "E1009BA013";
static const char data_1330F[11] __attribute__((scn_string_literal)) = "E1009BB001";
static const char data_1331A[11] __attribute__((scn_string_literal)) = "E1009BC001";
static const char data_13325[11] __attribute__((scn_string_literal)) = "E1009BB002";
static const char data_13330[11] __attribute__((scn_string_literal)) = "E1009BC002";
static const char data_1333B[11] __attribute__((scn_string_literal)) = "E1009BC003";
static const char data_13346[11] __attribute__((scn_string_literal)) = "E1009BB003";
static const char data_13351[11] __attribute__((scn_string_literal)) = "E1009BC004";
static const char data_1335C[11] __attribute__((scn_string_literal)) = "E1009BA014";
static const char data_13367[13] __attribute__((scn_string_literal)) = "E1009BA017";
static const Vector3 data_13374 = {.x = 14.5f, .y = 0.0f, .z = 144.5f};
static const Vector3 data_13380 = {.x = 15.0296f, .y = 4.275f, .z = 134.593094f};
static const char data_1338C[11] __attribute__((scn_string_literal)) = "E1009BA018";
static const char data_13397[7] __attribute__((scn_string_literal)) = "E1009B";
static const char data_1339E[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_133A3[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_133A8[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_133AD[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_133B2[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_133B7[12] __attribute__((scn_string_literal)) = "fre1100.snd";
static const char data_133C3[13] __attribute__((scn_string_literal)) = "f1new8so.snd";
static const uint32_t data_133D0 = 0xBFB33333;
static const float data_133D4[13] = {143.6f, 15.8f, 126.3f, 30.1f, 140.6f, 12.9f, 157.9f, -0.3f, 10.8f, 14.4f, 1.0f, 146.1f, 15.0f};
static const Vector4 data_13408 = {.x = 1.0f, .y = 0.3f, .z = 0.3f, .w = 0.3f};
static const char data_13418[22] __attribute__((scn_string_literal)) = "res_tachikiki_end,%d\012";
static const char data_1342E[29] __attribute__((scn_string_literal)) = "res_tachikiki_kyusai_end,%d\012";
static const char data_1344B[24] __attribute__((scn_string_literal)) = "res_1kai_hitorigoto,%d\012";
static const char data_13463[24] __attribute__((scn_string_literal)) = "res_left_hitorigoto,%d\012";
static const char data_1347B[25] __attribute__((scn_string_literal)) = "res_right_hitorigoto,%d\012";
static const char data_13494[13] __attribute__((scn_string_literal)) = "res_futo,%d\012";
static const char data_134A1[15] __attribute__((scn_string_literal)) = "res_search,%d\012";
static const char data_134B0[13] __attribute__((scn_string_literal)) = "res_game,%d\012";
static const char data_134BD[15] __attribute__((scn_string_literal)) = "res_limit,%d\012";

static void script_0030(void) {
  int32_t local_FC;
  local_FC = Script_GetSceneID();
  switch (local_FC) {
    case 1:
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_ENTRY2_TIMED_EVENT_DONE) == 0) {
        Script_TimedTransitionUtils(TIMED_TRANSITION_REGISTER, 1, 0x3044484A, 2, &data_10AE8, 0xD5A6C3, TimedTransitionUpdateMemoItem150);
      }
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JD00_ENTRY20_TIMED_EVENT_DONE) == 0) {
        Script_TimedTransitionUtils(TIMED_TRANSITION_REGISTER, 1, 0x3030444A, 20, &data_10AB0, 0xDA3AA3, 0);
      }
      break;
    case 2:
      if ((Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_GMCT_ROUTE_STARTED) == 0) | (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_GMCT_ROUTE_ADVANCED) == 0)) {
        Script_TimedTransitionUtils(TIMED_TRANSITION_REGISTER, 2, 0x54434D47, 1, &data_10A78, 0xB7420F, 0);
      }
      break;
    case 3:
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOZOMI_RESCUE_RUSH_ACTIVE) == 0) {
        Script_TimedTransitionUtils(TIMED_TRANSITION_REGISTER, 3, 0x4F4D4F4A, 20, &data_10A94, 0x2DC9FE, TimedTransitionRollbackToPreviousDay2330);
      }
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_FIRED_FROM_JOB) & ~Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 970)) {
        if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_MFSY_ENTRY42_TIMED_EVENT_DONE) == 0) {
          Script_TimedTransitionUtils(TIMED_TRANSITION_REGISTER, 3, 0x5953464D, 42, &data_10ACC, 0xBBB817, 0);
        }
      }
      break;
  }
  return;
}

static void script_0168(int32_t arg0) {
  script_0180(arg0);
  return;
}

static void script_0174(int32_t arg0) {
  script_0180(arg0);
  return;
}

static void script_0180(int32_t arg0) {
  if (Script_StepUtils(GET_STEP_HIGHER, 0) == 22) {
    int32_t local_FC;
    local_FC = Script_StepUtils(GET_STEP_LOWER, 0);
    if ((local_FC == 2) | (local_FC == 3)) {
      Script_SetShouldSleepFlag(0);
      goto L0001B9;
    }
  }
  L0001B9:;
  Script_ryoSetChara(arg0);
  Script_facemotUtils(arg0, 1, 0);
  Script_facemotUtils(arg0, 0, 0);
  Script_FaceEx(arg0, 0, 0, 2);
  Script_CharHandlers(arg0, -1, 0, 0);
  script_0030();
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

static void script_027C(int32_t arg0) {
  script_0174(arg0);
  for (;;) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_0293(int32_t arg0) {
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

static void script_02D4(int32_t arg0) {
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

static void script_0318(int32_t arg0) {
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

static void script_0396(void) {
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_PROGRESS_STEP_28_REACHED)) {
    script_0318(40);
  } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_PROGRESS_STEP_1E_REACHED)) {
    script_0318(30);
  } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_PROGRESS_STEP_14_REACHED)) {
    script_0318(20);
  } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JU00_CATA1_NBO_BRANCH_CHOSEN)) {
    script_0318(10);
  } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_PROGRESS_STEP05_SA1116_DOJO_DONE)) {
    script_0318(5);
  } else {
    script_0318(0);
  }
  return;
}

static int32_t script_0415(int32_t arg0) {
  if (arg0 < 0) {
    return -arg0;
  } else {
    return arg0;
  }
}

static void script_0433(float arg0) {
  if (!(arg0 >= 0.0f)) {
    (void)((float)0 - arg0);
    return;
  } else {
    (void)arg0;
    return;
  }
}

static int32_t script_0452(int32_t arg0, int32_t arg1) {
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

static void script_04CA(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
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

static int32_t script_0A7C(int32_t arg0) {
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

static void script_0B4D(int32_t arg0, int32_t arg1, int32_t arg2) {
  script_9870(arg0, arg2, 0);
  return;
}

static void script_0B5D(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  float local_F8;
  float local_F4;
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
    local_FC = script_9005(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
    if ((local_FC == 1) | (local_FC == 2)) {
      local_FC = 1;
    } else {
      local_FC = 0;
    }
    *(int32_t *)&local_F8 = Script_ScenDataUtil(data_11110, 5, Script_ScenDataUtil(data_10B20, 1, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 5);
    *(int32_t *)&local_F4 = Script_ScenDataUtil(data_11110, 5, Script_ScenDataUtil(data_10B20, 1, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 6);
    local_FC = script_9005(arg0, arg1, 5, local_F8 / 100.0f, local_F4 / 100.0f, arg3, -1);
    script_42E9(arg0, arg1, arg2, arg3, local_FC, 0);
  }
  script_9870(arg0, arg2, 0);
  return;
}

static void script_0C61(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  int32_t local_F8;
  float local_F4;
  float local_F0;
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
    local_FC = script_9005(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
    *(int32_t *)&local_F4 = Script_ScenDataUtil(data_11110, 5, Script_ScenDataUtil(data_10B20, 1, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 5);
    local_F4 = 90.0f - local_F4;
    *(int32_t *)&local_F0 = Script_ScenDataUtil(data_11110, 5, Script_ScenDataUtil(data_10B20, 1, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 6);
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
    local_FC = script_9005(arg0, arg1, local_F8, local_F4 / (float)100, local_F0 / (float)100, arg3, task_48[arg2]);
    script_42E9(arg0, arg1, arg2, arg3, local_FC, 1);
  }
  script_9870(arg0, arg2, 0);
  return;
}

static void script_0E03(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  float local_F8;
  float local_F4;
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
    local_FC = script_9005(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
    if ((local_FC == 1) | (local_FC == 2)) {
      local_FC = 1;
    } else {
      local_FC = 0;
    }
    *(int32_t *)&local_F8 = Script_ScenDataUtil(data_11110, 5, Script_ScenDataUtil(data_10B20, 1, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 5);
    *(int32_t *)&local_F4 = Script_ScenDataUtil(data_11110, 5, Script_ScenDataUtil(data_10B20, 1, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 6);
    local_FC = script_9005(arg0, arg1, 5, local_F8 / 100.0f, local_F4 / 100.0f, arg3, -1);
    if (local_FC == 1) {
      local_FC = 2;
    } else {
      local_FC = 3;
    }
    if ((Script_ScenDataUtil(task_0, 4, task_48[arg2] * 13 + 1) & 4) == 0) {
      script_27F6(arg0, arg1, arg2, arg3, local_FC, 0);
    } else {
      if (arg0 == task_160) {
        script_5BC9(arg0, arg1, arg2, arg3, local_FC, 0);
      }
    }
  }
  script_9870(arg0, arg2, 2);
  return;
}

static void script_0F64(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  float local_F8;
  float local_F4;
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
    local_FC = script_9005(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
    if ((local_FC == 1) | (local_FC == 2)) {
      local_FC = 1;
    } else {
      local_FC = 0;
    }
    *(int32_t *)&local_F8 = Script_ScenDataUtil(data_11110, 5, Script_ScenDataUtil(data_10B20, 1, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 5);
    *(int32_t *)&local_F4 = Script_ScenDataUtil(data_11110, 5, Script_ScenDataUtil(data_10B20, 1, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 6);
    local_FC = script_9005(arg0, arg1, 5, local_F8 / 100.0f, local_F4 / 100.0f, arg3, -1);
    if (local_FC == 1) {
      local_FC = 1;
    } else {
      local_FC = 0;
    }
    if ((Script_ScenDataUtil(task_0, 4, task_48[arg2] * 13 + 1) & 4) == 0) {
      script_27F6(arg0, arg1, arg2, arg3, local_FC, 0);
    } else {
      if (arg0 == task_160) {
        script_5BC9(arg0, arg1, arg2, arg3, local_FC, 0);
      }
    }
  }
  script_9870(arg0, arg2, 2);
  return;
}

static void script_10C5(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  int32_t local_F8;
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
    if ((Script_ScenDataUtil(task_0, 4, task_48[arg2] * 13 + 1) & 4) == 0) {
      float local_F4;
      float local_F0;
      local_FC = script_9005(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
      *(int32_t *)&local_F4 = Script_ScenDataUtil(data_11110, 5, Script_ScenDataUtil(data_10B20, 1, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 5);
      *(int32_t *)&local_F0 = Script_ScenDataUtil(data_11110, 5, Script_ScenDataUtil(data_10B20, 1, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 6);
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
      local_FC = script_9005(arg0, arg1, local_F8, local_F4 / 100.0f, local_F0 / 100.0f, arg3, task_48[arg2]);
      script_27F6(arg0, arg1, arg2, arg3, local_FC, 1);
    } else {
      if (arg0 == task_160) {
        float local_F4;
        float local_F0;
        local_FC = script_9005(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
        *(int32_t *)&local_F4 = Script_ScenDataUtil(data_11110, 5, Script_ScenDataUtil(data_10B20, 1, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 5);
        *(int32_t *)&local_F0 = Script_ScenDataUtil(data_11110, 5, Script_ScenDataUtil(data_10B20, 1, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 6);
        local_FC = script_9005(arg0, arg1, 6, local_F4 / 100.0f, local_F0 / 100.0f, arg3, task_48[arg2]);
        script_5BC9(arg0, arg1, arg2, arg3, local_FC, 1);
      }
    }
  }
  script_9870(arg0, arg2, 2);
  return;
}

static void script_1363(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  float local_F8;
  float local_F4;
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
    local_FC = script_9005(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
    *(int32_t *)&local_F8 = Script_ScenDataUtil(data_11110, 5, Script_ScenDataUtil(data_10B20, 1, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 5);
    *(int32_t *)&local_F4 = Script_ScenDataUtil(data_11110, 5, Script_ScenDataUtil(data_10B20, 1, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 6);
    script_9005(arg0, arg1, 0, local_F8 / 100.0f, local_F4 / 100.0f, arg3, -1);
  }
  script_9870(arg0, arg2, 0);
  return;
}

static void script_1431(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
    local_FC = script_9005(arg0, arg1, 4, 0.5f, 0.5f, arg3, -1);
    if (local_FC == 1) {
      local_FC = 2;
    } else {
      local_FC = 3;
    }
    script_3775(arg0, arg1, arg2, arg3, local_FC, 1);
  }
  script_9870(arg0, arg2, 1);
  return;
}

static void script_1498(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
    local_FC = script_9005(arg0, arg1, 4, 0.5f, 0.5f, arg3, -1);
    if (local_FC == 1) {
      local_FC = 1;
    } else {
      local_FC = 0;
    }
    script_3775(arg0, arg1, arg2, arg3, local_FC, 1);
  }
  script_9870(arg0, arg2, 1);
  return;
}

static void script_14FF(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
    script_9005(arg0, arg1, 4, 0.0f, 0.5f, arg3, -1);
    script_3775(arg0, arg1, arg2, arg3, 4, 1);
  }
  script_9870(arg0, arg2, 1);
  return;
}

static void script_1546(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  float local_F8;
  float local_F4;
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
    local_FC = script_9005(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
    if ((local_FC == 1) | (local_FC == 2)) {
      local_FC = 1;
    } else {
      local_FC = 0;
    }
    *(int32_t *)&local_F8 = Script_ScenDataUtil(data_11110, 5, Script_ScenDataUtil(data_10B20, 1, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 5);
    *(int32_t *)&local_F4 = Script_ScenDataUtil(data_11110, 5, Script_ScenDataUtil(data_10B20, 1, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 6);
    if (local_FC == 0) {
      local_FC = script_9005(arg0, arg1, 5, local_F8 / 100.0f, local_F4 / 100.0f, arg3, -1);
    } else {
      local_FC = script_9005(arg0, arg1, 5, local_F8 / 100.0f, local_F4 / 100.0f, arg3, -1);
    }
    script_3C2F(arg0, arg1, arg2, arg3, local_FC, 0);
  }
  script_9870(arg0, arg2, 2);
  return;
}

static void script_167C(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
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
      local_FF7F = script_9005(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
      *(int32_t *)&local_FF78 = Script_ScenDataUtil(data_11110, 5, Script_ScenDataUtil(data_10B20, 1, arg3 * 40 + task_98[arg2] * 4 + local_FF7F) * 9 + 5);
      *(int32_t *)&local_FF74 = Script_ScenDataUtil(data_11110, 5, Script_ScenDataUtil(data_10B20, 1, arg3 * 40 + task_98[arg2] * 4 + local_FF7F) * 9 + 6);
      local_FF7F = script_9005(arg0, arg1, 5, local_FF78 / (float)100, local_FF74 / (float)100, arg3, -1);
      script_96CE(arg0, 0);
      *(int32_t *)&local_94 = Script_ScenDataUtil(data_11110, 5, Script_ScenDataUtil(data_10B20, 1, arg3 * 40 + task_98[arg2] * 4 + local_FF7F) * 9 + 1);
      *(int32_t *)&local_90 = Script_ScenDataUtil(data_11110, 5, Script_ScenDataUtil(data_10B20, 1, arg3 * 40 + task_98[arg2] * 4 + local_FF7F) * 9 + 2);
      local_8C = 0.0f;
      local_FC = Script_ScenDataUtil(data_11110, 4, Script_ScenDataUtil(data_10B20, 1, arg3 * 40 + task_98[arg2] * 4 + local_FF7F) * 9);
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
      script_96CE(arg0, 1);
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
    script_9870(arg0, arg2, 0);
  } else {
    script_0C61(arg0, arg1, arg2, arg3);
  }
  return;
}

static void script_1F3D(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  float local_F8;
  int32_t local_F4;
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
    local_FC = script_9005(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
    *(int32_t *)&local_F8 = Script_ScenDataUtil(data_11110, 5, Script_ScenDataUtil(data_10B20, 1, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 5);
    local_F8 = 98.0f - local_F8;
    local_F4 = Script_ScenDataUtil(data_11110, 5, Script_ScenDataUtil(data_10B20, 1, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 6);
    if ((local_FC == 0) | (local_FC == 3)) {
      local_FC = script_9005(arg0, arg1, 4, 0.34736f, 0.73184f, arg3, -1);
    } else {
      local_FC = script_9005(arg0, arg1, 4, 0.4f, 0.712f, arg3, -1);
    }
    script_62EA(arg0, arg1, arg2, arg3, local_FC, 0);
  }
  script_9870(arg0, arg2, 0);
  return;
}

static int32_t script_205B(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
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
        local_FC = script_9005(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
        local_FC = script_9005(arg0, arg1, 7, 0.45f, 0.0f, arg3, -1);
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
              goto L002217;
            }
          }
          L002217:;
        } else {
          local_D4 = 0.45f;
        }
        Script_EVT_GetPosition(arg1, -1, &local_E4, 0);
        Script_mtwkGetSetRotation(arg1, &local_D8, 0);
        local_E4.x = (&local_E4)->x + local_D4 * Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_D8)->y & 0xFFFF);
        (&local_E4)->z = (&local_E4)->z - local_D4 * Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_D8)->y & 0xFFFF);
        *(&task_174 + 1) = Script_FloatUtils(ScriptMathOp_Atan2ReverseVector, (&local_E4)->x - (&task_168)->x, (&local_E4)->z - task_168.z);
        script_6A74(arg0, arg1, arg2, arg3, local_FC, local_F0);
      }
    } else if ((local_BC == 3) | (local_BC == 4) | (local_BC == 5) | (local_BC == 6)) {
      *(int32_t *)&local_D0 = Script_ScenDataUtil(&data_11B30, 5, Script_ScenDataUtil(task_0, 4, task_48[arg2] * 13 + 6) * 5 + 3);
      *(int32_t *)&local_CC = Script_ScenDataUtil(&data_11B30, 5, Script_ScenDataUtil(task_0, 4, task_48[arg2] * 13 + 6) * 5 + 4);
      local_FC = script_9005(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
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
      local_FC = script_9005(arg0, arg1, local_F8, local_D0 / (float)100, local_CC / (float)100, arg3, task_48[arg2]);
      script_6B4C(arg0, arg1, arg2, arg3, local_FC, 0);
    } else {
      if (local_BC == 7) {
        local_FC = script_9005(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
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
          *(int32_t *)&local_D0 = Script_ScenDataUtil(&data_11B30, 5, Script_ScenDataUtil(task_0, 4, task_48[arg2] * 13 + 6) * 5 + 3);
          *(int32_t *)&local_CC = Script_ScenDataUtil(&data_11B30, 5, Script_ScenDataUtil(task_0, 4, task_48[arg2] * 13 + 6) * 5 + 4);
          local_FC = script_9005(arg0, arg1, local_F8, local_D0 / (float)100, local_CC / (float)100, arg3, task_48[arg2]);
          Script_scenUtils(SCENARIO_CMD_INIT_SCNT);
          script_6FC3(arg0, arg1, arg2, arg3, local_FC, 0);
          Script_scenUtils(SCENARIO_CMD_UPDATE_TIMER);
        } else {
          *(int32_t *)&local_D0 = Script_ScenDataUtil(&data_11B30, 5, Script_ScenDataUtil(task_0, 4, task_48[arg2] * 13 + 6) * 5 + 3);
          *(int32_t *)&local_CC = Script_ScenDataUtil(&data_11B30, 5, Script_ScenDataUtil(task_0, 4, task_48[arg2] * 13 + 6) * 5 + 4);
          local_FC = script_9005(arg0, arg1, local_F8, local_D0 / (float)100, local_CC / (float)100, arg3, task_48[arg2]);
          script_6B4C(arg0, arg1, arg2, arg3, local_FC, 0);
        }
        goto L002712;
      }
    }
  }
  L002712:;
  script_9870(arg0, arg2, 0);
  end:;
}

static void script_2720(int32_t arg0, int32_t arg1) {
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
    Script_SetCharaMoveTarget(arg0, &local_F4, (&local_E8)->y, script_0A7C(arg1));
  }
  return;
}

__attribute__((scn_string_bindings(0, data_1264E)))
static void script_27F6(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
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
  *(int32_t *)&local_98 = Script_ScenDataUtil(data_11110, 5, Script_ScenDataUtil(data_10B20, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 1);
  *(int32_t *)&local_94 = Script_ScenDataUtil(data_11110, 5, Script_ScenDataUtil(data_10B20, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 2);
  *(int32_t *)&local_90 = Script_ScenDataUtil(data_11110, 5, Script_ScenDataUtil(data_10B20, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 3);
  *(int32_t *)&local_8C = Script_ScenDataUtil(data_11110, 5, Script_ScenDataUtil(data_10B20, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 4);
  *(int32_t *)&local_88 = Script_ScenDataUtil(data_11110, 5, Script_ScenDataUtil(data_10B20, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 7);
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
      local_FF78 = Script_ScenDataUtil(data_11110, 4, Script_ScenDataUtil(data_10B20, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
      local_FF7C = 18;
      local_80 = 18;
      local_A4 = 1.0f;
    } else {
      if (arg0 == task_160) {
        local_FF78 = Script_ScenDataUtil(data_11110, 4, Script_ScenDataUtil(data_10B20, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9) | -0x80000000;
        local_FF7C = 12;
        local_80 = 12;
      } else {
        local_FF78 = Script_ScenDataUtil(data_11110, 4, Script_ScenDataUtil(data_10B20, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
        local_FF7C = 18;
        local_80 = 18;
      }
      local_A4 = -1.0f;
    }
    script_04CA(arg0, local_FF78, arg2, arg3, arg4);
    local_FF78 = local_FF78 | ((Script_FloatUtils(ScriptMathOp_FloatToInt, local_98 - 3.0f) << 16) & 0x7FFF0000);
    script_96CE(arg0, 0);
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
          script_2720(arg0, task_98[arg2]);
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
                goto L002D7E;
              }
            }
            L002D7E:;
          } else {
            if ((local_FF64 == 1) | (local_FF64 == 3)) {
              if (!(local_B4 > 0.0f)) {
                local_B4 = 0.0f;
              } else {
                if (!(local_B4 < 0.75f)) {
                  local_B4 = 0.75f;
                  goto L002DBC;
                }
              }
              L002DBC:;
              goto L002DBF;
            }
          }
        }
      }
      L002DBF:;
      if (task_160 == arg0) {
        if (!(local_B8 <= local_94) & !(local_B8 >= local_90)) {
          local_86 = 0;
        }
      } else {
        if (!(local_B8 <= local_94) & !(local_B8 > local_88)) {
          *(int32_t *)&local_9C = Script_ScenDataUtil(data_11110, 5, Script_ScenDataUtil(data_10B20, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 8);
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
                goto L002F8B;
              }
            }
            L002F8B:;
          } else {
            if ((local_FF64 == 1) | (local_FF64 == 3)) {
              if (!(local_B4 < 0.75f)) {
                local_B4 = 0.75f;
              } else {
                if (!(local_B4 > 0.0f)) {
                  local_B4 = 0.0f;
                  goto L002FC9;
                }
              }
              L002FC9:;
              goto L002FCC;
            }
          }
          L002FCC:;
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
              goto L003025;
            }
          }
        }
      }
      L003025:;
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
  script_96CE(arg0, 1);
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
        goto L003708;
      }
    }
  }
  L003708:;
  return;
}

static void script_3709(int32_t arg0, int32_t arg1, int32_t arg2) {
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

static void script_3775(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
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
          goto L003BDF;
        }
      }
    }
    L003BDF:;
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

__attribute__((scn_string_bindings(0, data_1266A)))
static void script_3C2F(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
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
          local_FC = Script_ScenDataUtil(data_11110, 4, Script_ScenDataUtil(data_10B20, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
          break;
        case 0:
          local_B7 = 0;
          local_F8 = 12;
          local_F0 = 0x4000;
          local_B8 = 1.0f;
          local_FC = Script_ScenDataUtil(data_11110, 4, Script_ScenDataUtil(data_10B20, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9) | -0x80000000;
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
          local_FC = Script_ScenDataUtil(data_11110, 4, Script_ScenDataUtil(data_10B20, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9) | -0x80000000;
          break;
        case 0:
          local_B7 = 1;
          local_F8 = 18;
          local_F0 = 0xC000;
          local_B8 = 1.0f;
          local_FC = Script_ScenDataUtil(data_11110, 4, Script_ScenDataUtil(data_10B20, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
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
          local_FC = Script_ScenDataUtil(data_11110, 4, Script_ScenDataUtil(data_10B20, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9) | -0x80000000;
          break;
        case 0:
          local_B7 = 0;
          local_F8 = 18;
          local_F0 = 0xC000;
          local_B8 = -1.0f;
          local_FC = Script_ScenDataUtil(data_11110, 4, Script_ScenDataUtil(data_10B20, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
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
          local_FC = Script_ScenDataUtil(data_11110, 4, Script_ScenDataUtil(data_10B20, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
          break;
        case 0:
          local_B7 = 1;
          local_F8 = 12;
          local_F0 = 0x4000;
          local_B8 = -1.0f;
          local_FC = Script_ScenDataUtil(data_11110, 4, Script_ScenDataUtil(data_10B20, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9) | -0x80000000;
          break;
      }
      break;
  }
  *(int32_t *)&local_C4 = Script_ScenDataUtil(data_11110, 5, Script_ScenDataUtil(data_10B20, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 1);
  *(int32_t *)&local_C0 = Script_ScenDataUtil(data_11110, 5, Script_ScenDataUtil(data_10B20, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 2);
  local_FC = local_FC | ((Script_FloatUtils(ScriptMathOp_FloatToInt, local_C4 - 3.0f) << 16) & 0x7FFF0000);
  local_BC = 0.0f;
  script_96CE(arg0, 0);
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
  script_96CE(arg0, 1);
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

__attribute__((scn_string_bindings(0, data_12686)))
static void script_42E9(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
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
            local_A4 = Script_ScenDataUtil(data_11110, 4, Script_ScenDataUtil(data_10B20, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
            local_C0 = 1.0f;
            local_A0 = 18;
            local_9C = 18;
            local_90 = 15473;
            local_8C = 17294;
            break;
          case 0:
            local_CF = 0;
            local_A4 = Script_ScenDataUtil(data_11110, 4, Script_ScenDataUtil(data_10B20, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9) | -0x80000000;
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
            local_A4 = Script_ScenDataUtil(data_11110, 4, Script_ScenDataUtil(data_10B20, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
            local_C0 = 1.0f;
            local_A0 = 18;
            local_9C = 18;
            local_90 = 13653;
            break;
          case 0:
            local_CF = 0;
            local_A4 = Script_ScenDataUtil(data_11110, 4, Script_ScenDataUtil(data_10B20, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
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
            local_A4 = Script_ScenDataUtil(data_11110, 4, Script_ScenDataUtil(data_10B20, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9) | -0x80000000;
            local_C0 = 1.0f;
            local_A0 = 12;
            local_9C = 18;
            local_90 = 50062;
            local_8C = 48241;
            break;
          case 0:
            local_CF = 1;
            local_A4 = Script_ScenDataUtil(data_11110, 4, Script_ScenDataUtil(data_10B20, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
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
            local_A4 = Script_ScenDataUtil(data_11110, 4, Script_ScenDataUtil(data_10B20, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
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
            local_A4 = Script_ScenDataUtil(data_11110, 4, Script_ScenDataUtil(data_10B20, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
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
            local_A4 = Script_ScenDataUtil(data_11110, 4, Script_ScenDataUtil(data_10B20, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9) | -0x80000000;
            local_C0 = -1.0f;
            local_A0 = 12;
            local_9C = 12;
            local_90 = 50062;
            local_8C = 48241;
            break;
          case 0:
            local_CF = 0;
            local_A4 = Script_ScenDataUtil(data_11110, 4, Script_ScenDataUtil(data_10B20, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
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
            local_A4 = Script_ScenDataUtil(data_11110, 4, Script_ScenDataUtil(data_10B20, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
            local_C0 = -1.0f;
            local_A0 = 18;
            local_9C = 18;
            local_90 = 51882;
            break;
          case 0:
            local_CF = 0;
            local_A4 = Script_ScenDataUtil(data_11110, 4, Script_ScenDataUtil(data_10B20, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
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
            local_A4 = Script_ScenDataUtil(data_11110, 4, Script_ScenDataUtil(data_10B20, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
            local_C0 = -1.0f;
            local_A0 = 18;
            local_9C = 12;
            local_90 = 15473;
            local_8C = 17294;
            break;
          case 0:
            local_CF = 1;
            local_A4 = Script_ScenDataUtil(data_11110, 4, Script_ScenDataUtil(data_10B20, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9) | -0x80000000;
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
            local_A4 = Script_ScenDataUtil(data_11110, 4, Script_ScenDataUtil(data_10B20, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
            local_C0 = -1.0f;
            local_A0 = 18;
            local_9C = 18;
            local_90 = 13653;
            break;
          case 0:
            local_CF = 1;
            local_A4 = Script_ScenDataUtil(data_11110, 4, Script_ScenDataUtil(data_10B20, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
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
              local_A4 = Script_ScenDataUtil(data_11110, 4, Script_ScenDataUtil(data_10B20, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
              local_C0 = -1.0f;
              local_A0 = 18;
              local_9C = 12;
              local_90 = 48241;
              local_8C = 50062;
              break;
            case 1:
              local_CF = 1;
              local_A4 = Script_ScenDataUtil(data_11110, 4, Script_ScenDataUtil(data_10B20, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9) | -0x80000000;
              local_C0 = -1.0f;
              local_A0 = 12;
              local_9C = 12;
              local_90 = 50062;
              local_8C = 48241;
              break;
            case 2:
              local_CF = 1;
              local_A4 = Script_ScenDataUtil(data_11110, 4, Script_ScenDataUtil(data_10B20, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
              local_C0 = 1.0f;
              local_A0 = 18;
              local_9C = 18;
              local_90 = 15473;
              local_8C = 17294;
              break;
            case 3:
              local_CF = 0;
              local_A4 = Script_ScenDataUtil(data_11110, 4, Script_ScenDataUtil(data_10B20, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9) | -0x80000000;
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
              local_A4 = Script_ScenDataUtil(data_11110, 4, Script_ScenDataUtil(data_10B20, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
              local_C0 = -1.0f;
              local_A0 = 18;
              local_9C = 18;
              local_90 = 45511;
              break;
            case 1:
              local_CF = 1;
              local_A4 = Script_ScenDataUtil(data_11110, 4, Script_ScenDataUtil(data_10B20, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
              local_C0 = -1.0f;
              local_A0 = 18;
              local_9C = 18;
              local_90 = 51882;
              break;
            case 2:
              local_CF = 1;
              local_A4 = Script_ScenDataUtil(data_11110, 4, Script_ScenDataUtil(data_10B20, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
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
              local_A4 = Script_ScenDataUtil(data_11110, 4, Script_ScenDataUtil(data_10B20, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
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
        goto L004EDC;
      }
    }
  }
  L004EDC:;
  (void)local_90;
  *(int32_t *)&local_B8 = Script_ScenDataUtil(data_11110, 5, Script_ScenDataUtil(data_10B20, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 1);
  *(int32_t *)&local_B4 = Script_ScenDataUtil(data_11110, 5, Script_ScenDataUtil(data_10B20, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 2);
  *(int32_t *)&local_B0 = Script_ScenDataUtil(data_11110, 5, Script_ScenDataUtil(data_10B20, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 3);
  *(int32_t *)&local_AC = Script_ScenDataUtil(data_11110, 5, Script_ScenDataUtil(data_10B20, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 4);
  *(int32_t *)&local_A8 = Script_ScenDataUtil(data_11110, 5, Script_ScenDataUtil(data_10B20, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 7);
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
  script_04CA(arg0, local_A4, arg2, arg3, arg4);
  local_A4 = local_A4 | ((Script_FloatUtils(ScriptMathOp_FloatToInt, local_B8 - 3.0f) << 16) & 0x7FFF0000);
  script_96CE(arg0, 0);
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
              script_2720(arg0, task_98[arg2]);
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
        local_FF60 = script_0452((&local_FF64)->y & 0xFFFF, local_88 & 0xFFFF);
        if (local_FF60 > 910) {
          local_88 = (&local_FF64)->y + 910;
        } else {
          if (local_FF60 < -910) {
            local_88 = (&local_FF64)->y - 910;
            goto L0054E1;
          }
        }
        L0054E1:;
        if (!(local_C4 >= local_B4)) {
          if (!(local_C0 <= 0.0f)) {
            if (script_0452(local_FF70, local_88) > 0) {
              local_88 = local_FF70;
            }
          } else {
            if (script_0452(local_FF70, local_88) < 0) {
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
          local_FF74 = (float)script_0415(script_0452(local_FF70, (&local_FF48)->y) * 360 / 0x10000);
        }
        if (!(local_C4 <= local_B4) & !(local_C4 > local_A8)) {
          *(int32_t *)&local_BC = Script_ScenDataUtil(data_11110, 5, Script_ScenDataUtil(data_10B20, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 8);
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
              goto L00598D;
            }
          }
        }
      }
      L00598D:;
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
  script_96CE(arg0, 1);
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
      goto L005BC8;
    }
  }
  L005BC8:;
  return;
}

__attribute__((scn_string_bindings(0, data_126A2)))
static void script_5BC9(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
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
    script_96CE(arg0, 0);
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
                goto L005EED;
              }
            }
            L005EED:;
          } else {
            if ((local_84 == 1) | (local_84 == 3)) {
              if (!(local_B8 > -0.75f)) {
                local_B8 = -0.75f;
              } else {
                if (!(local_B8 < 0.0f)) {
                  local_B8 = 0.0f;
                  goto L005F29;
                }
              }
              L005F29:;
              goto L005F2C;
            }
          }
        }
      }
      L005F2C:;
      if (!(local_BC <= local_A4)) {
        int32_t local_84;
        local_84 = arg4;
        if ((local_84 == 0) | (local_84 == 2)) {
          local_B8 = 0.75f;
        } else {
          if ((local_84 == 1) | (local_84 == 3)) {
            local_B8 = -0.75f;
            goto L005F71;
          }
        }
      }
      L005F71:;
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
  script_96CE(arg0, 1);
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

__attribute__((scn_string_bindings(0, data_126BE)))
static void script_62EA(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
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
      local_8C = Script_ScenDataUtil(data_11110, 4, Script_ScenDataUtil(data_10B20, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
      local_A0 = -1.0f;
      local_88 = 18;
      local_84 = 18;
      local_FF78 = 46421;
      local_FF74 = 48787;
      break;
    case 1:
      local_AF = 1;
      local_8C = Script_ScenDataUtil(data_11110, 4, Script_ScenDataUtil(data_10B20, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9) | -0x80000000;
      local_A0 = -1.0f;
      local_88 = 12;
      local_84 = 12;
      local_FF78 = 50426;
      local_FF74 = 48969;
      break;
    case 2:
      local_AF = 1;
      local_8C = Script_ScenDataUtil(data_11110, 4, Script_ScenDataUtil(data_10B20, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
      local_A0 = 1.0f;
      local_88 = 18;
      local_84 = 18;
      local_FF78 = 15109;
      local_FF74 = 16566;
      break;
    case 3:
      local_AF = 0;
      local_8C = Script_ScenDataUtil(data_11110, 4, Script_ScenDataUtil(data_10B20, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9) | -0x80000000;
      local_A0 = 1.0f;
      local_88 = 12;
      local_84 = 12;
      local_FF78 = 19114;
      local_FF74 = 16748;
      break;
  }
  (void)local_FF78;
  *(int32_t *)&local_9C = Script_ScenDataUtil(data_11110, 5, Script_ScenDataUtil(data_10B20, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 1);
  *(int32_t *)&local_98 = Script_ScenDataUtil(data_11110, 5, Script_ScenDataUtil(data_10B20, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 2);
  *(int32_t *)&local_94 = Script_ScenDataUtil(data_11110, 5, Script_ScenDataUtil(data_10B20, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 3);
  *(int32_t *)&local_90 = Script_ScenDataUtil(data_11110, 5, Script_ScenDataUtil(data_10B20, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 4);
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
  script_96CE(arg0, 0);
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
  script_96CE(arg0, 1);
  local_F4.x = 0;
  (&local_F4)->z = 0;
  (&local_F4)->y = local_80;
  Script_charaPosRotUtils(arg1, 12, 0, &local_F4, 0);
  (&local_F4)->y = local_FF7C;
  Script_charaPosRotUtils(arg1, 7, 0, &local_F4, 0);
  return;
}

static void script_6A74(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
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

__attribute__((scn_string_bindings(0, data_126DA)))
static void script_6B4C(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
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
        local_E0 = Script_ScenDataUtil(&data_11B30, 4, local_D4 * 5) | -0x80000000;
        break;
      case 0:
        local_E0 = Script_ScenDataUtil(&data_11B30, 4, local_D4 * 5);
        break;
    }
  } else if ((local_CC == 2) | (local_CC == 4) | (local_CC == 6) | (local_CC == 8) | (local_CC == 10) | (local_CC == 19) | (local_CC == 22) | (local_CC == 24) | (local_CC == 26) | (local_CC == 28)) {
    local_CC = arg4;
    switch (local_CC) {
      case 1:
        local_E0 = Script_ScenDataUtil(&data_11B30, 4, local_D4 * 5);
        break;
      case 0:
        local_E0 = Script_ScenDataUtil(&data_11B30, 4, local_D4 * 5) | -0x80000000;
        break;
    }
  } else {
    if ((local_CC == 13) | (local_CC == 14) | (local_CC == 15) | (local_CC == 16) | (local_CC == 20) | (local_CC == 23) | (local_CC == 29)) {
      local_CC = arg4;
      switch (local_CC) {
        case 0:
          local_E0 = Script_ScenDataUtil(&data_11B30, 4, local_D4 * 5);
          break;
        case 1:
          local_E0 = Script_ScenDataUtil(&data_11B30, 4, local_D4 * 5) | -0x80000000;
          break;
        case 2:
          local_E0 = Script_ScenDataUtil(&data_11B30, 4, local_D4 * 5);
          break;
        case 3:
          local_E0 = Script_ScenDataUtil(&data_11B30, 4, local_D4 * 5) | -0x80000000;
          break;
      }
      goto L006D6D;
    }
  }
  L006D6D:;
  *(int32_t *)&local_EC = Script_ScenDataUtil(&data_11B30, 5, local_D4 * 5 + 1);
  *(int32_t *)&local_E8 = Script_ScenDataUtil(&data_11B30, 5, local_D4 * 5 + 2);
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

__attribute__((scn_string_bindings(0, data_126F6)))
static void script_6FC3(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
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
        goto L0072CF;
      }
    }
  }
  L0072CF:;
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
    L007D9A:;
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
      if (!(local_FF6C >= 20)) goto L007D9A;
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

static void script_7F56(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
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
      Script_SetCharaMoveTarget(arg0, &local_E0, local_BC[1], script_0A7C(arg5));
    }
  }
  return;
}

static void script_83A0(int32_t arg0, DSWKDoorEntry *arg1, int32_t *arg2, DoorResourceDescriptor *arg3, MapObjectRecord *arg4, int32_t arg5, int32_t arg6) {
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
  L00840A:;
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
  if (!(local_F8 >= 10)) goto L00840A;
  local_F8 = 0;
  for (;;) {
    if (Script_ScenDataUtil(task_0, 4, local_F8 * 13) == -1) {
      break;
    }
    Script_ScenDataUtil(task_0, 14, local_F8 * 13 + 1, 0);
    local_F8 = local_F8 + 1;
  }
  Script_EnqueueSCNT(0);
  DSWK_BindTablesAndInitState(arg1, arg2, arg3, arg4, &task_10, task_20, task_48, (shen_name *)task_70, task_98, arg0, arg5, data_10B20, data_11110, (shen_name *)task_E8);
  Script_scenCreateTask(script_8C5C, 0);
  return;
}

static void script_852C(DSWKDoorEntry *arg0, int32_t *arg1, DoorResourceDescriptor *arg2, MapObjectRecord *arg3, int32_t arg4) {
  task_0 = arg0;
  task_4 = arg1;
  task_8 = arg2;
  task_C = arg3;
  DSWK_RebindDoorSystemTables(arg0, arg1, arg2, arg3, arg4);
  task_180 = 1;
  return;
}

static void script_8560(void) {
  int32_t local_FC;
  local_FC = 0;
  L008568:;
  task_20[local_FC] = -1;
  task_48[local_FC] = -1;
  task_70[local_FC] = 0;
  task_98[local_FC] = 0;
  task_C0[local_FC] = 0;
  task_110[local_FC] = 0;
  local_FC = local_FC + 1;
  if (!(local_FC >= 10)) goto L008568;
  return;
}

static void script_85C9(int32_t arg0, int32_t arg1) {
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

static void script_8666(int32_t arg0, int32_t arg1) {
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

static void script_86A0(void) {
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

static void script_8703(void) {
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

static void script_8766(int32_t arg0) {
  int32_t local_FC;
  local_FC = Script_ScenDataUtil(task_0, 4, arg0 * 13 + 1);
  local_FC = local_FC | 2;
  Script_ScenDataUtil(task_0, 14, arg0 * 13 + 1, local_FC);
  return;
}

static void script_879D(int32_t arg0) {
  int32_t local_FC;
  local_FC = Script_ScenDataUtil(task_0, 4, arg0 * 13 + 1);
  local_FC = local_FC & -3;
  Script_ScenDataUtil(task_0, 14, arg0 * 13 + 1, local_FC);
  return;
}

static void script_87D4(int32_t arg0) {
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

static void script_8A16(int32_t arg0) {
  int32_t local_FC;
  local_FC = Script_ScenDataUtil(task_0, 4, arg0 * 13 + 1);
  local_FC = local_FC | 0x20;
  Script_ScenDataUtil(task_0, 14, arg0 * 13 + 1, local_FC);
  script_8B1E(arg0);
  script_87D4(arg0);
  return;
}

static void script_8A5F(int32_t arg0) {
  int32_t local_FC;
  local_FC = Script_ScenDataUtil(task_0, 4, arg0 * 13 + 1);
  local_FC = local_FC & -33;
  Script_ScenDataUtil(task_0, 14, arg0 * 13 + 1, local_FC);
  script_8B2C(arg0);
  script_87D4(arg0);
  return;
}

static void script_8AA8(int32_t arg0, int32_t arg1) {
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

static void script_8B1E(int32_t arg0) {
  script_8AA8(arg0, 0);
  return;
}

static void script_8B2C(int32_t arg0) {
  script_8AA8(arg0, 1);
  return;
}

static void script_8B3A(int32_t arg0) {
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

static void script_8C01(void) {
  script_8B3A(1);
  return;
}

static void script_8C0D(void) {
  script_8B3A(0);
  return;
}

static void script_8C19(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5, int32_t arg6) {
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

static int32_t script_8C5C(void) {
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
        script_9A2C(1, local_FC, local_F4, local_F8);
        if (script_9BE0(local_FC, local_F4, local_F8)) {
          local_F4 = -1;
          local_F8 = -1;
        }
      } else {
        script_9A2C(0, local_FC, local_F4, local_F8);
        local_F4 = -1;
        local_F8 = -1;
      }
      script_9DF6(local_F4, local_F8);
    } else {
      task_180 = 0;
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int8_t script_8D6D(int32_t arg0, int32_t arg1, int8_t arg2) {
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

__attribute__((scn_string_bindings(0, data_12781, 1, data_1279A)))
static void script_8E05(int32_t arg0) {
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

static void script_8FD4(int32_t arg0) {
  if (task_138[arg0] != -1) {
    Script_SetMapControlTaskFlag(task_138[arg0], 1);
    task_138[arg0] = -1;
  }
  return;
}

static int32_t script_9005(int32_t arg0, int32_t arg1, int32_t arg2, float arg3, float arg4, int32_t arg5, int32_t arg6) {
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
          if ((local_B4 == 1) | (local_B4 == 2)) goto L0091F0;
        }
        L0091F0:;
      } else if ((local_B4 == 2) | (local_B4 == 4) | (local_B4 == 6) | (local_B4 == 8) | (local_B4 == 10) | (local_B4 == 12) | (local_B4 == 24) | (local_B4 == 26) | (local_B4 == 31) | (local_B4 == 33) | (local_B4 == 35)) {
        local_B4 = local_C0;
        if ((local_B4 == 2) | (local_B4 == 1)) {
          arg3 = (float)0 - arg3;
        } else {
          if ((local_B4 == 3) | (local_B4 == 0)) {
            arg3 = (float)0 - arg3;
            arg4 = (float)0 - arg4;
            (&local_C4)->y = (&local_C4)->y + 0x8000;
            goto L009293;
          }
        }
        L009293:;
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
            goto L00943F;
          }
        }
      }
    }
    L00943F:;
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

static void script_96CE(int32_t arg0, int32_t arg1) {
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

static void script_9700(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  int32_t local_F8;
  local_F8 = script_9005(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
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

__attribute__((scn_string_bindings(0, data_127B3)))
static void script_9870(int32_t arg0, int32_t arg1, int32_t arg2) {
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

static void script_9A2C(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
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
        local_FC = script_8D6D(arg2, arg3, local_FC);
      } else {
        local_F4 = arg3;
        if ((local_F4 == 21) | (local_F4 == 22) | (local_F4 == 23)) {
          local_FC = script_8D6D(arg2, arg3, local_FC);
          goto L009AF0;
        }
      }
      L009AF0:;
    } else {
      if ((Script_ScenDataUtil(task_0, 4, task_48[arg2] * 13 + 1) & 4) == 0) {
        local_FC = script_8D6D(arg2, arg3, local_FC);
        local_F8 = Script_ScenDataUtil(task_0, 4, task_48[arg2] * 13 + 6);
        if (local_F8 == 0) {
          local_FC = 0;
        } else {
          if (local_F8 >= 2) {
            Script_EVT_GetPosition(task_160, -1, &local_E8, 0);
            if (Script_CycleManagerUtils(CYCLE_CMD_COUNT_CYCLES_IN_RADIUS, &local_E8, 1.5f)) {
              local_FC = 0;
            }
            goto L009B8B;
          }
        }
      }
    }
  }
  L009B8B:;
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

__attribute__((scn_string_bindings(0, data_127C2)))
static int32_t script_9BE0(int32_t arg0, int32_t arg1, int32_t arg2) {
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
            goto L009D94;
          }
        }
        L009D94:;
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

static void script_9DF6(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  int32_t local_F8;
  local_FC = 0;
  L009DFE:;
  local_F8 = task_20[local_FC];
  switch (local_F8) {
    case 6:
      if (!(arg0 != -1)) goto L009E1E;
      L009E1E:;
      script_9EC0(local_FC, arg1);
      break;
    case 5:
      if (!(arg0 != -1)) goto L009E3E;
      L009E3E:;
      script_9F21(local_FC);
      break;
    case 4:
      if (!(arg0 != -1)) goto L009E5C;
      L009E5C:;
      script_A0C6(local_FC, arg0, arg1);
      break;
    case 3:
      if (!(arg0 != -1)) goto L009E7E;
      L009E7E:;
      script_A15E(local_FC, arg1);
      break;
    case 2:
      script_A240(local_FC);
      break;
    case 1:
      break;
  }
  local_FC = local_FC + 1;
  if (!(local_FC >= 10)) goto L009DFE;
  return;
}

static void script_9EC0(int32_t arg0, int32_t arg1) {
  if (task_110[arg0] == 0) {
    task_110[arg0] = Script_scenCreateTask(script_205B, 4, task_70[arg0], task_E8[arg0], arg0, arg1);
    task_C0[arg0] = task_70[arg0];
    script_8E05(arg0);
  }
  return;
}

__attribute__((scn_string_bindings(0, data_127D1, 1, data_127E3)))
static void script_9F21(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, task_70[arg0])) {
    if (task_110[arg0] == 0) {
      if (task_70[arg0] == task_160) {
        task_10.mode = 1;
        task_10.entry_idx = task_48[arg0];
        goto L009F76;
      }
      L009F76:;
      local_FC = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_48[arg0], 2);
      local_F8 = Script_ScenDataUtil(task_8, 4, Script_ScenDataUtil(task_C, 4, local_FC * 9 + 1) * 4 + 1);
      script_9700(task_70[arg0], task_E8[arg0], task_48[arg0], local_F8);
      task_110[arg0] = Script_scenCreateTask(Script_ScenDataUtil(&data_11BD0, 8, local_F8), 4, task_70[arg0], task_E8[arg0], arg0, local_F8);
      task_C0[arg0] = task_70[arg0];
      if ((Script_ScenDataUtil(task_0, 4, task_48[arg0] * 13 + 1) & 0x20000) == 0) {
        Script_sprintf("Door Box is ON!!\012");
        script_8E05(arg0);
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

static void script_A0C6(int32_t arg0, int32_t arg1, int32_t arg2) {
  if ((task_110[arg0] == 0) & (arg2 >= 0)) {
    task_70[arg0] = task_160;
    task_10.mode = 2;
    task_10.entry_idx = task_48[arg0];
    task_110[arg0] = Script_scenCreateTask(Script_ScenDataUtil(&data_11BD0, 8, arg2), 4, task_70[arg0], task_E8[arg0], arg0, arg2);
    task_C0[arg0] = task_70[arg0];
    script_8E05(arg0);
  }
  return;
}

__attribute__((scn_string_bindings(0, data_127F6, 1, data_12808)))
static void script_A15E(int32_t arg0, int32_t arg1) {
  if ((task_110[arg0] == 0) & (arg1 >= 0)) {
    script_9700(task_70[arg0], task_E8[arg0], task_48[arg0], arg1);
    task_110[arg0] = Script_scenCreateTask(Script_ScenDataUtil(&data_11BD0, 8, arg1), 4, task_70[arg0], task_E8[arg0], arg0, arg1);
    task_C0[arg0] = task_70[arg0];
    if ((Script_ScenDataUtil(task_0, 4, task_48[arg0] * 13 + 1) & 0x20000) == 0) {
      Script_sprintf("Door Box is ON!!\012");
      script_8E05(arg0);
    } else {
      Script_sprintf("Door Box is OFF!!\012");
      task_138[arg0] = -1;
    }
  }
  return;
}

static void script_A240(int32_t arg0) {
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

__attribute__((scn_string_bindings(0, data_129C0)))
static int32_t script_A297(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
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
  script_AB99(arg0, arg1, arg2, arg3, arg4, arg5);
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
      script_C30A(arg0, arg5, arg2);
      *(&task_1B8 + arg5) = arg2 + 1;
      *((int8_t *)&task_1E4 + arg5) = 3;
      script_CF9B(arg0, arg5, local_DC);
      Script_CharHandlers(arg0, 2, 0, 16);
      (void)((local_CD = -((local_CB = 0) != 0)) & 1);
      if (local_DC & 0xF00 & 0x400) {
        script_AC8C(1, arg2);
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
            Script_scenCreateTask(script_C72C, 2, arg0, 1);
          } else {
            Script_scenCreateTask(script_C72C, 2, arg0, 1);
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
        local_CF = script_BD6E(arg0, local_E8[1], *local_F4, local_F4[2], arg5, local_DC, local_E0);
        if (local_CA & 4) {
          Script_KaidanUtils(CAM_STAIR_APPLY_GUIDE_FACING, arg0, arg2, (const void *)task_1A4);
        }
        (void)((local_CC = -1) & 1);
        Script_ccowSetFlag80(arg0, 2);
        Script_ccowUnknown(arg0, 1, 3.0f);
      } else {
        if ((local_DC & 0xF) <= 12) {
          if ((*(&task_1AC + arg5) & 8) == 0) {
            Script_scenCreateTask(script_C72C, 2, arg0, 0);
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
          local_CF = local_E0 - script_BFFD(arg0, local_E8[1], *local_F4, local_F4[2], arg5, local_DC, local_E0);
          if (local_CA & 4) {
            Script_KaidanUtils(CAM_STAIR_APPLY_GUIDE_FACING, arg0, arg2, (const void *)task_1A4);
          }
          Script_ccowSetFlag80(arg0, 2);
          Script_ccowUnknown(arg0, 1, 3.0f);
          (void)((local_CC = 0) & 0x80);
          goto L00A78C;
        }
      }
      L00A78C:;
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
          Script_scenCreateTask(script_CD42, 2, arg0, arg5);
        } else {
          Script_scenCreateTask(script_CE6A, 2, arg0, arg5);
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
              local_CF = local_CF + script_B073(arg0, local_CF, local_E0, arg5, local_DC, arg2, local_CE - task_1E9);
            } else {
              local_CF = local_CF - script_B563(arg0, local_CF, arg5, local_DC, arg2, local_CE - task_1E9);
            }
            if ((local_CF > 0) & (local_CF < local_E0)) {
              if (!(local_CD & 0x20)) goto L00A9CE;
            }
          }
          L00A9CE:;
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
        script_D070(arg0, arg5, local_DC);
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

static void script_AB99(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
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

static void script_AC8C(int32_t arg0, int32_t arg1) {
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

static void script_ADCB(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
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
        script_C9A0(arg0);
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

static void script_AF7A(int32_t arg0, int32_t arg1, int32_t arg2) {
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

static int8_t script_B073(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5, int32_t arg6) {
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
        script_CA7B(arg0);
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
      script_D020(arg0, arg3, arg4);
      if (arg4 & 0xF00 & 0x800) {
        script_AC8C(0, arg5);
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
        script_C8C5(arg0);
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
      script_D020(arg0, arg3, arg4);
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
        script_AC8C(0, arg5);
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

static int8_t script_B563(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
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
        script_CC4C(arg0, arg2);
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
      script_D020(arg0, arg2, arg3);
      if (arg3 & 0xF00 & 0x800) {
        script_AC8C(0, arg4);
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
        script_CB56(arg0, arg2);
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
        script_AC8C(0, arg4);
        if (arg3 & 0xF000 & 0x1000) {
          (void)((task_1EC = -1) & 1);
          while (task_1EC & 1) {
            SCNF_WaitFrames_0000(1);
          }
        }
        script_D020(arg0, arg2, arg3);
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
        script_D020(arg0, arg2, arg3);
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

static void script_BB2F(int32_t arg0, int32_t arg1, float arg2, float arg3, int32_t arg4, int32_t arg5) {
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

static int32_t script_BD6E(int32_t arg0, int32_t arg1, float arg2, float arg3, int32_t arg4, int32_t arg5, int32_t arg6) {
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

__attribute__((scn_string_bindings(0, data_12A5F)))
static int32_t script_BFFD(int32_t arg0, int32_t arg1, float arg2, float arg3, int32_t arg4, int32_t arg5, int32_t arg6) {
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
  script_CF9B(arg0, arg4, arg5);
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

__attribute__((scn_string_bindings(1, data_12A85)))
static void script_C30A(int32_t arg0, int32_t arg1, int32_t arg2) {
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

static void script_C538(int32_t arg0) {
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

static void script_C5D7(float arg0) {
  int32_t local_FC;
  float local_F8;
  local_FC = 0;
  while (local_FC < 10) {
    int32_t local_F4;
    local_F4 = task_1EA;
    if ((local_F4 == 0) | (local_F4 == 2) | (local_F4 == 4)) {
      break;
      goto L00C60A;
    }
    L00C60A:;
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

static void script_C666(float arg0) {
  int32_t local_FC;
  float local_F8;
  local_FC = 0;
  while (local_FC < 50) {
    int32_t local_F4;
    local_F4 = task_1EA;
    if ((local_F4 == 0) | (local_F4 == 2) | (local_F4 == 4)) {
      break;
      goto L00C699;
    }
    L00C699:;
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

static void script_C6F5(void) {
  Script_SetCameraSettings(6, 7);
  Script_SetCameraSettings(CAM_UTILS_SET_DISTANCE_FROM_PLAYER, task_1D8);
  Script_SetCameraSettings(CAM_UTILS_SET_HEIGHT_ABOVE_PLAYER, task_1DC);
  Script_SetCameraSettings(CAM_UTILS_SET_MAX_Y_ANGLE, task_1E0);
  Script_KaidanUtils(CAM_STAIR_RESET_RETRACTION_LIMIT);
  (void)((task_1EB = 0) & 2);
  return;
}

static int32_t script_C72C(int32_t arg0, int32_t arg1) {
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
          script_C538(arg1);
        } else if (local_E0 == 2) {
          script_C6F5();
        } else if ((local_E0 == 0) | (local_E0 == 3)) {
          Script_SetCameraSettings(6, 7);
          task_1BC = 0;
        } else {
          if (local_E0 == 4) goto L00C7D9;
        }
        L00C7D9:;
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
          if (local_E0 == 4) goto L00C839;
        }
        L00C839:;
        local_E7 = 4;
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  if (task_1EA != 0) {
    if (local_E6 & 0x40) {
      script_C6F5();
    }
  }
  end:;
}

static void script_C863(int32_t arg0) {
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

static void script_C8AA(int32_t arg0) {
  task_1A8 = arg0;
  return;
}

static void script_C8B4(int32_t arg0) {
  task_1C0 = arg0;
  return;
}

static void script_C8BE(void) {
  (void)task_1C0;
  return;
}

__attribute__((scn_string_bindings(0, data_12A8F)))
static void script_C8C5(int32_t arg0) {
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

__attribute__((scn_string_bindings(0, data_12AA7)))
static void script_C9A0(int32_t arg0) {
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

__attribute__((scn_string_bindings(0, data_12ABF)))
static void script_CA7B(int32_t arg0) {
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

__attribute__((scn_string_bindings(0, data_12AD7)))
static void script_CB56(int32_t arg0, int32_t arg1) {
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

__attribute__((scn_string_bindings(0, data_12AEF)))
static void script_CC4C(int32_t arg0, int32_t arg1) {
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

__attribute__((scn_string_bindings(0, data_12B07)))
static int32_t script_CD42(int32_t arg0, int32_t arg1) {
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

__attribute__((scn_string_bindings(0, data_12B1F)))
static int32_t script_CE6A(int32_t arg0, int32_t arg1) {
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

__attribute__((scn_string_bindings(0, data_12B37)))
static void script_CF9B(int32_t arg0, int32_t arg1, int32_t arg2) {
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
      goto L00D01F;
    }
  }
  L00D01F:;
  return;
}

static void script_D020(int32_t arg0, int32_t arg1, int32_t arg2) {
  if (*(&task_1AC + arg1) & 0x20) {
    Script_ccowUtils(arg0, 3);
  } else if ((arg2 & 0x8000) > 0) {
  } else {
    if ((arg2 & 0x100) | ((arg2 & 0x4000) > 0)) {
      Script_ccowUtils(arg0, 3);
      goto L00D06F;
    }
  }
  L00D06F:;
  return;
}

static void script_D070(int32_t arg0, int32_t arg1, int32_t arg2) {
  Script_ccowUtils(arg0, 5);
  if (*(&task_1AC + arg1) & 0x20) {
    Script_ccowUtils(arg0, 3);
  } else {
    if ((arg2 & 0x100) | (arg2 & 0x4000) | ((arg2 & 0x8000) > 0)) {
      Script_ccowUtils(arg0, 3);
      goto L00D0BE;
    }
  }
  L00D0BE:;
  return;
}

static void script_D0BF(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Vector3 local_F4;
  int32_t local_F0;
  int16_t local_EE;
  task_290 = arg3;
  task_294 = arg2;
  task_264 = 0;
  Script_srchUtils(SRCH_SCRIPT_InitEntryTableFromTaskOffset, &data_12B94);
  task_280 = arg1;
  task_278 = arg0;
  task_27C = 1;
  task_464 = 0;
  task_298 = 0;
  Script_scenToggleVisibleFor(task_290, 0);
  Script_scenToggleVisibleFor(task_294, 0);
  if (arg0 == 1) {
    Script_lightResetSlotToDefaults(28);
    Script_LightUtils(28, 0, 4);
    Script_LightUtils(28, 2, &data_1301C);
    Script_LightUtils(28, 4, 1.7f);
    Script_LightUtils(28, 5, 2.8f);
    Script_LightUtils(28, 6, data_1302C);
    Script_LightUtils(28, 10, 1820);
  }
  local_F0 = 0;
  for (;;) {
    local_EE = Script_ScenDataUtil(&data_12B94, 2, local_F0 * 16);
    if (local_EE < 0) {
      break;
    }
    if ((Script_ScenDataUtil(&data_12B94, 2, local_F0 * 16 + 1) == 2) | (Script_ScenDataUtil(&data_12B94, 2, local_F0 * 16 + 1) == 9)) {
      if (Script_ScenDataUtil(&data_12B94, 4, local_F0 * 8 + 6) != 0) {
        Script_EVT_GetPosition(Script_ScenDataUtil(&data_12B94, 4, local_F0 * 8 + 1), -1, &local_F4, 0);
        if (!((&local_F4)->x < 9999.0f) & !((&local_F4)->z < 9999.0f)) {
          task_464 = task_464 + 1;
          task_2D4[task_464] = Script_ScenDataUtil(&data_12B94, 4, local_F0 * 8 + 1);
        }
      }
    }
    local_F0 = local_F0 + 1;
  }
  return;
}

static void script_D25C(void) {
  int32_t unused_FC;
  int32_t local_F8;
  int32_t local_F4;
  int32_t local_F0;
  Vector3 local_E4;
  local_F0 = 0;
  for (;;) {
    local_F4 = Script_ScenDataUtil(&data_12F98, 4, local_F0 * 5);
    local_F8 = Script_ScenDataUtil(&data_12F98, 4, local_F0 * 5 + 1);
    if (local_F4 == -1) {
      break;
    }
    Script_EVT_GetPosition(local_F8, -1, &local_E4, 0);
    if (!((&local_E4)->x < 9999.0f) & !((&local_E4)->z < 9999.0f)) {
      task_464 = task_464 + 1;
      task_2D4[task_464] = local_F8;
    } else if (Script_ScenDataUtil(&data_12F98, 4, local_F0 * 5 + 3) == 0) {
      Script_mtwkGetSetRotation(local_F8, &task_244, 0);
      Script_EVT_GetPosition(local_F8, -1, &task_238, 0);
      Script_fixoAttachEx(local_F8, local_F4, 0, &task_238, &task_244);
    } else {
      task_244.x = 0;
      task_244.y = 0;
      task_244.z = 0;
      task_238.x = 0.0f;
      task_238.y = 0.0f;
      task_238.z = 0.0f;
      Script_CalculateMatrixRotationFromParams(local_F8, local_F4, 0, &task_238, &task_244);
    }
    local_F0 = local_F0 + 1;
  }
  return;
}

static void script_D37C(void) {
  int16_t local_FE;
  int32_t local_F8;
  task_25C = -1;
  task_25C = Script_srchUtils(-1);
  if (Script_srchUtils(SRCH_SCRIPT_GetCurrentTargetEntryIndex) >= 0) {
    if (task_278 == 1) {
      Script_lightSetSlotEnabled(28, 1);
      script_E5B3(CHARA_AKIR);
      task_284 = 1;
    }
    Script_scenUtils(SCENARIO_CMD_INIT_SCNT);
    if (task_280) {
      task_260 = Script_srchUtils(SRCH_SCRIPT_GetCurrentTargetEntryIndex);
      local_FE = Script_ScenDataUtil(&data_12B94, 2, task_260 * 16 + 1);
      if ((local_FE < 0) & (Script_ScenDataUtil(&data_12B94, 4, task_260 * 8 + 7) > 0)) {
        Script_sprintf("Map Start!!!!!!!!!!!!!!!!!\012");
        script_E752(task_260);
      } else {
        if (task_288 != task_260) {
          for (;;) {
            if (!(task_278 == 1)) goto L00D448;
            L00D448:;
            if (Script_srchUtils(SRCH_SCRIPT_UpdateAndPollSelection) == Script_ScenDataUtil(&data_12B94, 4, Script_srchUtils(SRCH_SCRIPT_GetCurrentTargetEntryIndex) * 8 + 1)) {
              break;
            }
            if (Script_srchUtils(SRCH_SCRIPT_GetCurrentTargetEntryIndex) < 0) {
              break;
            }
            SCNF_WaitFrames_0000(1);
          }
          for (;;) {
            if ((MMTK_ScriptMemoToggle(2, 0) == 0) & (Script_GetConversationDetails(CONV_DETAILS_MODE_GET_GAME_STATE, 0) == 7)) {
              break;
            }
          }
          task_298 = 9;
          script_F14D(Script_ScenDataUtil(&data_12B94, 4, Script_srchUtils(SRCH_SCRIPT_GetCurrentTargetEntryIndex) * 8 + 1));
          task_288 = Script_srchUtils(SRCH_SCRIPT_GetCurrentTargetEntryIndex);
        }
      }
    }
    task_270 = 1;
  } else {
    if (task_284 == 1) {
      Script_lightSetSlotEnabled(28, 0);
      task_284 = 0;
    }
    Script_scenUtils(SCENARIO_CMD_UPDATE_TIMER);
    task_270 = 0;
    if (task_280) {
      task_288 = -1;
    }
  }
  if (task_25C != -1) {
    task_268 = 0;
    task_26C = 0;
    task_274 = 0;
    Script_ryoControl(PCCTL_DisablePlayerControl);
    Script_convSetCamInactiveFlag(1);
    local_F8 = script_E54E(task_25C);
    local_FE = Script_ScenDataUtil(&data_12B94, 2, local_F8 * 16 + 1);
    Script_scenToggleVisibleFor(task_290, 1);
    Script_scenToggleVisibleFor(task_294, 1);
    if (local_FE < 0) {
      task_298 = 8;
      script_F14D(task_25C);
    } else {
      script_D5D6(local_F8);
    }
    Script_ryoControl(RYO_RestoreControl);
    Script_scenToggleVisibleFor(task_290, 0);
    Script_scenToggleVisibleFor(task_294, 0);
  }
  (void)task_25C;
  return;
}

static void script_D5D6(int32_t arg0) {
  int16_t local_FE;
  uint8_t unused_F0[14];
  int32_t local_EC;
  local_FE = Script_ScenDataUtil(&data_12B94, 2, arg0 * 16 + 1);
  local_EC = local_FE;
  if (local_EC == 0) {
    task_298 = 8;
    script_F14D(task_25C);
  } else {
    switch (local_EC) {
      case 0:
        task_298 = 8;
        script_F14D(task_25C);
        break;
      case 2:
        script_E179(arg0);
        break;
      case 3:
        script_D732(arg0, 3);
        break;
      case 4:
        script_D732(arg0, 10);
        break;
      case 5:
        script_D732(arg0, 20);
        break;
      case 6:
        script_D732(arg0, 30);
        break;
      case 7:
        script_D732(arg0, 40);
        break;
      case 8:
        script_D732(arg0, 50);
        break;
      case 9:
        Script_InvestigationUtils(ShenmueUtils_BeginInteractionMode, 0, 60);
        for (;;) {
          if (Script_InvestigationUtils(ShenmueUtils_PollInteractionModeDone) != 0) {
            break;
          }
          SCNF_WaitFrames_0000(1);
        }
        script_E179(arg0);
        Script_InvestigationUtils(ShenmueUtils_RequestInteractionModeExit);
        for (;;) {
          if (Script_InvestigationUtils(ShenmueUtils_PollInteractionModeDone) != 0) {
            break;
          }
          SCNF_WaitFrames_0000(1);
        }
        break;
    }
  }
  return;
}

static void script_D732(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t unused_F0[2];
  int32_t local_EC;
  int32_t local_E8;
  int32_t local_E4;
  int32_t local_E0;
  int32_t unused_D8[2];
  int32_t local_D4;
  int32_t local_D0;
  int32_t local_CC;
  int32_t unused_B4[6];
  int32_t local_B0;
  int32_t local_AC;
  int32_t local_A8;
  int32_t local_A4;
  int32_t local_A0;
  int32_t local_9C;
  int32_t local_98;
  int32_t local_94;
  int8_t local_93;
  uint8_t unused_92;
  int8_t local_91;
  uint8_t unused_90;
  Vector3 local_84;
  int32_t unused_80;
  float local_FF78[2];
  int32_t local_FF74;
  local_91 = 0;
  local_93 = 0;
  local_FC = Script_ScenDataUtil(&data_12B94, 4, arg0 * 8 + 1);
  local_CC = Script_ScenDataUtil(&data_12B94, 4, arg0 * 8 + 6);
  local_D0 = Script_ScenDataUtil(&data_12B94, 4, arg0 * 8 + 7);
  local_E4 = 0;
  if (Script_ScenDataUtil(data_12F9C, 4, local_E4) != -1) {
    local_B0 = Script_ScenDataUtil(data_12F9C, 4, local_E4);
    local_AC = Script_ScenDataUtil(data_12F9C, 4, local_E4 + 1);
    local_A8 = Script_ScenDataUtil(data_12F9C, 4, local_E4 + 2);
    local_A4 = Script_ScenDataUtil(data_12F9C, 4, local_E4 + 3);
    local_E4 = 4;
    for (;;) {
      if ((Script_ScenDataUtil(data_12F9C, 4, local_E4) == -1) & (Script_ScenDataUtil(data_12F9C, 4, local_E4 + 1) == -1)) {
        local_B0 = 0;
        local_AC = 0;
        local_A8 = 0;
        local_A4 = 0;
        break;
      }
      if (Script_ScenDataUtil(data_12F9C, 4, local_E4) == -1) {
        local_E4 = local_E4 + 1;
        local_B0 = Script_ScenDataUtil(data_12F9C, 4, local_E4);
        local_AC = Script_ScenDataUtil(data_12F9C, 4, local_E4 + 1);
        local_A8 = Script_ScenDataUtil(data_12F9C, 4, local_E4 + 2);
        local_A4 = Script_ScenDataUtil(data_12F9C, 4, local_E4 + 3);
        local_E4 = local_E4 + 4;
      }
      if (local_FC == Script_ScenDataUtil(data_12F9C, 4, local_E4)) {
        break;
      }
      local_E4 = local_E4 + 1;
    }
  }
  if (task_280) {
    task_298 = 4;
    script_F14D(local_FC);
  }
  if (task_274 == 0) {
    if (local_D0 != 0) {
      task_29C = *(int32_t *)((char *)&task_21C + (local_D0 - 1) * 56);
      *(&task_29C + 1) = *(int32_t *)((char *)&task_21C + ((local_D0 - 1) * 56 + 4));
      *(&task_29C + 2) = *(int32_t *)((char *)&task_21C + ((local_D0 - 1) * 56 + 8));
      *(&task_29C + 3) = *(int32_t *)((char *)&task_21C + ((local_D0 - 1) * 56 + 12));
      *(&task_29C + 4) = *(int32_t *)((char *)&task_21C + ((local_D0 - 1) * 56 + 16));
      *(&task_29C + 5) = *(int32_t *)((char *)&task_21C + ((local_D0 - 1) * 56 + 20));
      *(&task_29C + 6) = *(int32_t *)((char *)&task_21C + ((local_D0 - 1) * 56 + 24));
      *(&task_29C + 7) = *(int32_t *)((char *)&task_21C + ((local_D0 - 1) * 2 + 1) * 28);
      *(&task_29C + 8) = *(int32_t *)((char *)&task_21C + (((local_D0 - 1) * 2 + 1) * 28 + 4));
      *(&task_29C + 9) = *(int32_t *)((char *)&task_21C + (((local_D0 - 1) * 2 + 1) * 28 + 8));
      *(&task_29C + 10) = *(int32_t *)((char *)&task_21C + (((local_D0 - 1) * 2 + 1) * 28 + 12));
      *(&task_29C + 11) = *(int32_t *)((char *)&task_21C + (((local_D0 - 1) * 2 + 1) * 28 + 16));
      *(&task_29C + 12) = *(int32_t *)((char *)&task_21C + (((local_D0 - 1) * 2 + 1) * 28 + 20));
      *(&task_29C + 13) = *(int32_t *)((char *)&task_21C + (((local_D0 - 1) * 2 + 1) * 28 + 24));
      Script_InvestigationUtils(ShenmueUtils_LoadSpecialAssetEntry, &task_29C);
    }
    if (local_CC != 0) {
      arg1 = local_CC - 1;
    }
    Script_InvestigationUtils(ShenmueUtils_QueueSndCommand, local_B0, local_AC);
    Script_InvestigationUtils(ShenmueUtils_BeginInteractionMode, local_FC, arg1);
    local_E0 = local_E0 + 1;
    for (;;) {
      if (task_278 == 1) {
        script_E5B3(local_FC);
      }
      if (Script_InvestigationUtils(ShenmueUtils_PollInteractionModeDone) != 0) {
        break;
      }
      SCNF_WaitFrames_0000(1);
      local_E0 = local_E0 + 1;
      if (local_E0 > 30) {
        if (Script_GetControllerInput(1) == 0x400) {
          local_91 = 1;
        }
      }
    }
    local_D4 = local_FC;
    task_298 = 5;
    script_F14D(local_FC);
    local_E4 = 0;
    for (;;) {
      local_EC = Script_ScenDataUtil(&data_12F98, 4, local_E4 * 5);
      if (local_EC == -1) {
        break;
      }
      if (local_EC == task_25C) {
        local_E8 = Script_ScenDataUtil(&data_12F98, 4, local_E4 * 5 + 1);
        local_F8 = Script_ScenDataUtil(&data_12F98, 4, local_E4 * 5 + 2);
        local_FF74 = Script_ScenDataUtil(&data_12F98, 4, local_E4 * 5 + 4);
        Script_fixoClearAttachment(local_E8);
        Script_MoveObjectToObject(local_E8, local_FC);
        Script_EVT_GetPosition(local_E8, -1, &task_238, 0);
        Script_mtwkGetSetRotation(local_E8, &task_244, 0);
        Script_MoveObjectToObject(local_E8, -1);
        local_E4 = -1;
        break;
      }
      local_E4 = local_E4 + 1;
    }
    local_E0 = 0;
    if (Script_ScenDataUtil(data_12F9C, 4, local_E0) != -1) {
      local_A0 = Script_ScenDataUtil(data_12F9C, 4, local_E0);
      local_9C = Script_ScenDataUtil(data_12F9C, 4, local_E0 + 1);
      local_98 = Script_ScenDataUtil(data_12F9C, 4, local_E0 + 2);
      local_94 = Script_ScenDataUtil(data_12F9C, 4, local_E0 + 3);
      local_E0 = 4;
      for (;;) {
        if ((Script_ScenDataUtil(data_12F9C, 4, local_E0) == -1) & (Script_ScenDataUtil(data_12F9C, 4, local_E0 + 1) == -1)) {
          local_A0 = 0;
          local_9C = 0;
          local_98 = 0;
          local_94 = 0;
          break;
        }
        if (Script_ScenDataUtil(data_12F9C, 4, local_E0) == -1) {
          local_E0 = local_E0 + 1;
          local_A0 = Script_ScenDataUtil(data_12F9C, 4, local_E0);
          local_9C = Script_ScenDataUtil(data_12F9C, 4, local_E0 + 1);
          local_98 = Script_ScenDataUtil(data_12F9C, 4, local_E0 + 2);
          local_94 = Script_ScenDataUtil(data_12F9C, 4, local_E0 + 3);
          local_E0 = local_E0 + 4;
        }
        if (local_E8 == Script_ScenDataUtil(data_12F9C, 4, local_E0)) {
          break;
        }
        local_E0 = local_E0 + 1;
      }
    }
    local_E0 = 0;
    for (;;) {
      if (task_464 == local_E0) {
        break;
      }
      local_E0 = local_E0 + 1;
      if (task_2D4[local_E0] == local_E8) {
        Script_sprintf("このアイテムもってる");
        local_E4 = 0;
      }
    }
    if (local_E4 == -1) {
      if (Script_scenCheckCharaExist(local_E8)) {
        Script_sprintf("tansu naka = %d\012", Script_charaDisplay(local_E8, CDM_Query));
        if (Script_charaDisplay(local_E8, CDM_Query) == 0) {
          local_E4 = 0;
        }
      }
    }
    if (task_26C == 1) {
      local_F8 = 0;
    }
    if ((local_E4 == -1) & (task_27C >= 0)) {
      for (;;) {
        if (task_278 == 1) {
          script_E5B3(local_FC);
        }
        Script_InvestigationUtils(ShenmueUtils_UpdateSpecialAssetInput);
        if ((Script_GetControllerInput(0) & 0x400) | (local_91 == 1)) {
          Script_EVT_GetPosition(local_E8, -1, &local_84, 0);
          Script_ScreenToWorldAssign(&local_84, (void *)local_FF78);
          if (-!(*local_FF78 <= 80.0f) & -!(*local_FF78 >= 560.0f) & -!(local_FF78[1] <= 60.0f) & -!(local_FF78[1] >= 420.0f)) {
            if (local_F8 > -1) {
              break;
            }
          }
        }
        if (Script_GetControllerInput(0) & 0x200 & -(task_27C == 1)) {
          local_F8 = -1;
          break;
        }
        SCNF_WaitFrames_0000(1);
      }
      if (local_F8 != -1) {
        Script_InvestigationUtils(ShenmueUtils_QueueSndCommand, local_A0, local_9C);
        if (local_FF74 == 0) {
          Script_InvestigationUtils(ShenmueUtils_BeginTargetModeDefault, local_E8);
        } else {
          Script_InvestigationUtils(ShenmueUtils_BeginTargetModeByType, local_E8, local_FF74 - 1);
        }
        for (;;) {
          if (Script_InvestigationUtils(ShenmueUtils_UpdateStateMachine) != 0) {
            break;
          }
          SCNF_WaitFrames_0000(1);
        }
        if (task_278 == 1) {
          script_E5B3(local_FC);
        }
        task_298 = 1;
        script_F14D(local_E8);
        if (task_27C >= 0) {
          for (;;) {
            if (task_278 == 1) {
              script_E5B3(local_FC);
            }
            if ((local_91 == 1) & (task_27C == 1)) {
              break;
            }
            if (Script_GetControllerInput(0) & 0x400 & -(task_27C == 1)) {
              break;
            }
            if (Script_GetControllerInput(0) & 0x200 & -(task_27C == 1)) {
              local_F8 = 0;
              break;
            }
            SCNF_WaitFrames_0000(1);
            if (task_268 == 0) {
              Script_InvestigationUtils(ShenmueUtils_UpdateInterpolatedSpeed);
            }
          }
          if (local_F8 == 0) {
            if (task_280) {
              task_298 = 3;
              script_F14D(local_E8);
            }
            Script_InvestigationUtils(ShenmueUtils_QueueSndCommand, local_98, local_94);
            Script_InvestigationUtils(ShenmueUtils_RequestTransitionVariantA);
          } else {
            if (task_280) {
              task_298 = 2;
              script_F14D(local_E8);
            }
            if (local_F8 & 0x8000000) {
              Script_GetCassetteFlags(local_F8, 1, 0, 1);
              SCNF_StartIconSystemMenu(local_F8 + 255);
            } else {
              Script_GetCassetteFlags(local_F8, 1, 0, 0);
              SCNF_StartIconSystemMenu(local_F8);
            }
            Script_InvestigationUtils(ShenmueUtils_RequestTransitionVariantB);
            task_464 = task_464 + 1;
            task_2D4[task_464] = local_E8;
            local_93 = 1;
          }
          for (;;) {
            if (Script_InvestigationUtils(ShenmueUtils_UpdateStateMachine) != 0) {
              break;
            }
            SCNF_WaitFrames_0000(1);
          }
        }
      }
      Script_fixoAttachEx(local_E8, local_FC, 0, &task_238, &task_244);
    } else {
      if (task_27C >= 0) {
        for (;;) {
          if (task_278 == 1) {
            script_E5B3(local_FC);
          }
          Script_InvestigationUtils(ShenmueUtils_UpdateSpecialAssetInput);
          if ((local_91 == 1) & (task_27C == 1)) {
            break;
          }
          if (Script_GetControllerInput(0) & 0x400 & -(task_27C == 1)) {
            break;
          }
          if (Script_GetControllerInput(0) & 0x200 & -(task_27C == 1)) {
            break;
          }
          SCNF_WaitFrames_0000(1);
        }
      }
    }
    if (task_27C >= 0) {
      Script_InvestigationUtils(ShenmueUtils_QueueSndCommand, local_A8, local_A4);
      Script_InvestigationUtils(ShenmueUtils_RequestInteractionModeExit);
      task_298 = 6;
      script_F14D(local_D4);
      for (;;) {
        if (Script_InvestigationUtils(ShenmueUtils_PollInteractionModeDone) != 0) {
          break;
        }
        SCNF_WaitFrames_0000(1);
      }
      task_298 = 7;
      script_F14D(local_D4);
      if (!(task_278 == 1)) goto L00E14B;
      L00E14B:;
      if (local_93 == 1) {
        Script_srchUtils(SRCH_SCRIPT_Reset);
        Script_SetGameStateTo(AUTH_None);
      }
    } else {
      task_27C = 1;
      Script_srchUtils(SRCH_SCRIPT_Reset);
      Script_SetGameStateTo(AUTH_None);
    }
  }
  return;
}

static void script_E179(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  int32_t local_F0;
  int32_t local_EC;
  int32_t local_E8;
  int32_t local_E4;
  int8_t local_E3;
  int8_t local_E2;
  local_E3 = 0;
  local_FC = Script_ScenDataUtil(&data_12B94, 4, arg0 * 8 + 1);
  local_E4 = 0;
  if (Script_ScenDataUtil(data_12F9C, 4, local_E4) != -1) {
    local_F4 = Script_ScenDataUtil(data_12F9C, 4, local_E4);
    local_F0 = Script_ScenDataUtil(data_12F9C, 4, local_E4 + 1);
    local_EC = Script_ScenDataUtil(data_12F9C, 4, local_E4 + 2);
    local_E8 = Script_ScenDataUtil(data_12F9C, 4, local_E4 + 3);
    local_E4 = 4;
    for (;;) {
      if ((Script_ScenDataUtil(data_12F9C, 4, local_E4) == -1) & (Script_ScenDataUtil(data_12F9C, 4, local_E4 + 1) == -1)) {
        local_F4 = 0;
        local_F0 = 0;
        local_EC = 0;
        local_E8 = 0;
        break;
      }
      if (Script_ScenDataUtil(data_12F9C, 4, local_E4) == -1) {
        local_E4 = local_E4 + 1;
        local_F4 = Script_ScenDataUtil(data_12F9C, 4, local_E4);
        local_F0 = Script_ScenDataUtil(data_12F9C, 4, local_E4 + 1);
        local_EC = Script_ScenDataUtil(data_12F9C, 4, local_E4 + 2);
        local_E8 = Script_ScenDataUtil(data_12F9C, 4, local_E4 + 3);
        local_E4 = local_E4 + 4;
      }
      if (local_FC == Script_ScenDataUtil(data_12F9C, 4, local_E4)) {
        break;
      }
      local_E4 = local_E4 + 1;
    }
  }
  if (Script_charaDisplay(local_FC, CDM_Query)) {
    Script_InvestigationUtils(ShenmueUtils_QueueSndCommand, local_F4, local_F0);
    if (Script_ScenDataUtil(&data_12B94, 4, arg0 * 8 + 7) == 0) {
      Script_InvestigationUtils(ShenmueUtils_BeginTargetModeDefault, local_FC);
    } else {
      Script_InvestigationUtils(ShenmueUtils_BeginTargetModeByType, local_FC, Script_ScenDataUtil(&data_12B94, 4, arg0 * 8 + 7) - 1);
    }
    local_E2 = 0;
    for (;;) {
      SCNF_WaitFrames_0000(1);
      if (Script_InvestigationUtils(ShenmueUtils_UpdateStateMachine) != 0) {
        break;
      }
      if (Script_GetControllerInput(1) == 0x400) {
        local_E2 = 1;
      }
    }
    if (task_278 == 1) {
      script_E5B3(local_FC);
    }
    task_298 = 1;
    script_F14D(task_25C);
    local_F8 = Script_ScenDataUtil(&data_12B94, 4, arg0 * 8 + 6);
    for (;;) {
      if (task_278 == 1) {
        script_E5B3(local_FC);
      }
      SCNF_WaitFrames_0000(1);
      if (task_268 == 0) {
        Script_InvestigationUtils(ShenmueUtils_UpdateInterpolatedSpeed);
      }
      if ((local_E2 == 1) & (task_27C == 1)) {
        break;
      }
      if (Script_GetControllerInput(0) & 0x400 & -(task_27C == 1)) {
        break;
      }
      if (Script_GetControllerInput(0) & 0x200 & -(task_27C == 1)) {
        local_F8 = 0;
        break;
      }
    }
    if (task_26C == 1) {
      local_F8 = 0;
    }
    if (local_F8 == 0) {
      Script_InvestigationUtils(ShenmueUtils_QueueSndCommand, local_EC, local_E8);
      Script_InvestigationUtils(ShenmueUtils_RequestTransitionVariantA);
    } else {
      if (task_280) {
        task_298 = 2;
        script_F14D(task_25C);
      }
      if (local_F8 & 0x8000000) {
        Script_GetCassetteFlags(local_F8, 1, 0, 1);
        SCNF_StartIconSystemMenu(local_F8 + 255);
      } else {
        Script_GetCassetteFlags(local_F8, 1, 0, 0);
        SCNF_StartIconSystemMenu(local_F8);
      }
      Script_InvestigationUtils(ShenmueUtils_RequestTransitionVariantB);
      task_464 = task_464 + 1;
      task_2D4[task_464] = local_FC;
      local_E3 = 1;
    }
    for (;;) {
      SCNF_WaitFrames_0000(1);
      if (Script_InvestigationUtils(ShenmueUtils_UpdateStateMachine) != 0) {
        break;
      }
    }
    if (!(task_278 == 1)) goto L00E516;
    L00E516:;
    if (local_E3 == 1) {
      Script_srchUtils(SRCH_SCRIPT_Reset);
      Script_SetGameStateTo(AUTH_None);
    }
    if (local_F8 == 0) {
      if (task_280) {
        task_298 = 3;
        script_F14D(task_25C);
      }
    }
  }
  return;
}

static int32_t script_E54E(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  int16_t local_F6;
  local_FC = 0;
  local_F8 = 0;
  for (;;) {
    local_F6 = Script_ScenDataUtil(&data_12B94, 2, local_F8 * 16);
    if (local_F6 < 0) {
      break;
    }
    if (Script_ScenDataUtil(&data_12B94, 4, local_F8 * 8 + 1) == arg0) {
      local_FC = local_F8;
      break;
    }
    local_F8 = local_F8 + 1;
  }
  return local_FC;
}

static void script_E5B3(int32_t arg0) {
  Vector3 local_F4;
  int32_t unused_E8[3];
  Vector3 local_DC;
  Vector3 local_D0;
  Vector3 local_C4;
  int32_t unused_C0;
  int32_t local_B8[2];
  int32_t local_B4;
  int16_t local_B2;
  uint8_t unused_B0[2];
  local_B4 = script_E54E(arg0);
  local_B2 = Script_ScenDataUtil(&data_12B94, 2, local_B4 * 16 + 1);
  if (local_B2 < 0) {
    *(int32_t *)&local_F4.x = Script_ScenDataUtil(&data_12B94, 4, local_B4 * 8 + 3);
    *(int32_t *)&(&local_F4)->y = Script_ScenDataUtil(&data_12B94, 4, local_B4 * 8 + 4);
    *(int32_t *)&(&local_F4)->z = Script_ScenDataUtil(&data_12B94, 4, local_B4 * 8 + 5);
  } else {
    Script_EVT_GetPosition(arg0, -1, &local_F4, 0);
  }
  Script_GetAuthorState_And_SetCamPos(0, &local_DC);
  Script_GetAuthorState_And_SetCamPos(&local_D0, 0);
  local_B8[1] = Script_FloatUtils(ScriptMathOp_Atan2ReverseVector, (&local_D0)->x - (&local_DC)->x, (&local_D0)->z - (&local_DC)->z);
  local_B8[0] = Script_FloatUtils(ScriptMathOp_Atan2ReverseVector, (&local_D0)->z - (&local_DC)->z, (&local_D0)->y - (&local_DC)->y);
  local_C4.x = (float)0 - Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, local_B8[1]);
  (&local_C4)->y = (float)0 - Script_MathUtils(SCRIPT_MATH_COS_FIXED16, *(int32_t *)local_B8);
  (&local_C4)->z = (float)0 - Script_MathUtils(SCRIPT_MATH_COS_FIXED16, local_B8[1]);
  Script_lightSpotLightEnable(28, &local_D0, &local_C4);
  Script_LightUtils(28, 2, &data_1301C);
  Script_LightUtils(28, 4, 0.85f);
  Script_LightUtils(28, 5, 7.0f);
  Script_LightUtils(28, 6, data_1302C);
  Script_LightUtils(28, 10, 7500);
  return;
}

static void script_E72A(void) {
  task_27C = 0;
  return;
}

static void script_E734(void) {
  task_27C = 1;
  return;
}

static void script_E73E(void) {
  task_27C = -1;
  return;
}

static void script_E748(int32_t arg0) {
  task_278 = arg0;
  return;
}

static void script_E752(int32_t arg0) {
  int32_t local_F4[3];
  int32_t unused_F0;
  int32_t local_E8[2];
  local_F4[0] = Script_ScenDataUtil(&data_12B94, 4, arg0 * 8 + 3);
  local_F4[1] = Script_ScenDataUtil(&data_12B94, 4, arg0 * 8 + 4);
  local_F4[2] = Script_ScenDataUtil(&data_12B94, 4, arg0 * 8 + 5);
  local_E8[1] = Script_ScenDataUtil(&data_12B94, 2, arg0 * 16 + 4);
  task_468 = Script_ScenDataUtil(&data_12B94, 4, arg0 * 8 + 7) - 1;
  *(&task_468 + 1) = 0;
  *(&task_468 + 2) = 0;
  *(&task_468 + 3) = 0;
  *(&task_468 + 4) = 0;
  *(&task_468 + 5) = local_E8[1];
  *(&task_468 + 6) = 0;
  *(&task_468 + 7) = -1;
  *(&task_468 + 8) = 0;
  *(&task_468 + 9) = 0;
  *(&task_468 + 10) = 0;
  *(&task_468 + 11) = 0;
  *(&task_468 + 12) = 0;
  *(&task_468 + 13) = 0;
  Script_InvestigationUtils(ShenmueUtils_LoadSpecialAssetEntry, &task_468);
  Script_InvestigationUtils(ShenmueUtils_BeginInteractionMode, (void *)local_F4, 60);
  for (;;) {
    if (Script_InvestigationUtils(ShenmueUtils_PollInteractionModeDone) != 0) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  for (;;) {
    Script_InvestigationUtils(ShenmueUtils_UpdateSpecialAssetInput);
    SCNF_WaitFrames_0000(1);
    if (Script_GetControllerInput(0) == 0x400) {
      break;
    }
    if (Script_GetControllerInput(0) == 0x200) {
      break;
    }
  }
  Script_InvestigationUtils(ShenmueUtils_RequestInteractionModeExit);
  for (;;) {
    if (Script_InvestigationUtils(ShenmueUtils_PollInteractionModeDone) != 0) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_srchUtils(SRCH_SCRIPT_Reset);
  return;
}

static void script_E8F8(void) {
  script_10706(SHEN_NAME('TB00'), 23.72f, 5.04f, 139.49f, 59164);
  script_10706(SHEN_NAME('TB01'), 0.0f, 0.0f, 0.0f, 0);
  script_10706(SHEN_NAME('TB02'), 25.84f, 5.04f, 141.29f, 42780);
  script_10706(SHEN_NAME('TB10'), 25.39f, 5.04f, 140.38f, 58800);
  script_10706(SHEN_NAME('TB11'), 26.91f, 5.04f, 138.9f, 0xA000);
  script_10706(SHEN_NAME('TB12'), 28.35f, 5.04f, 140.24f, 9830);
  script_10706(SHEN_NAME('ZAR0'), 25.03f, 5.07f, 138.23f, 0);
  script_10706(SHEN_NAME('ZAR1'), 0.0f, 0.0f, 0.0f, 0);
  script_10706(SHEN_NAME('ZAR2'), 0.0f, 0.0f, 0.0f, 0);
  script_10706(SHEN_NAME('ZAR3'), 0.0f, 0.0f, 0.0f, 0);
  script_10706(SHEN_NAME('ZAR4'), 0.0f, 0.0f, 0.0f, 0);
  script_10706(SHEN_NAME('PEN0'), 26.57f, 5.03f, 138.46f, 0x2000);
  script_10706(SHEN_NAME('PEN1'), 24.31f, 5.03f, 137.13f, 0);
  script_10706(SHEN_NAME('YNM0'), 24.44f, 5.04f, 139.04f, 0);
  script_10706(SHEN_NAME('YNM1'), 26.25f, 5.04f, 140.71f, 0);
  script_10706(SHEN_NAME('YNM2'), 28.12f, 5.04f, 140.16f, 0);
  script_10706(SHEN_NAME('YNM3'), 0.0f, 0.0f, 0.0f, 0);
  script_10706(SHEN_NAME('YNM4'), 24.55f, 5.04f, 137.86f, 0);
  script_10706(SHEN_NAME('FUT0'), 24.01f, 5.04f, 137.21f, 0x8000);
  Script_EVT_SetRotation(SHEN_NAME('FUT0'), 0x38000000, &data_130D4, 0);
  script_10706(SHEN_NAME('FUT1'), 27.33f, 5.032f, 139.11f, 54613);
  script_10706(SHEN_NAME('SHO0'), 24.4f, 5.032f, 137.48f, 25486);
  script_10706(SHEN_NAME('SHO1'), 27.96f, 5.032f, 139.79f, 61895);
  return;
}

static int32_t script_EB70(void) {
  int32_t unused_FC;
  task_4A0 = 0;
  Script_InvestigationUtils(ShenmueUtils_SetupOrSyncAnchors, SHEN_NAME('YKUL'), 0x52554B59);
  script_D0BF(0, 1, 0x4C554B59, 0x52554B59);
  Script_srchUtils(SRCH_SCRIPT_SetEntrySelectorFilter, task_210);
  Script_scenCreateTask(script_EBC5, 0);
  for (;;) {
    script_D37C();
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_EBC5(void) {
  Vector3 local_F4;
  for (;;) {
    Script_EVT_GetPosition(CHARA_AKIR, -1, &local_F4, 0);
    if (!((&local_F4)->y <= 4.0f)) {
      if (Script_CheckTargetWithinAngleRange(&data_130E0, 0.3f, 0, 0x8000, &local_F4)) {
        task_210 = 2;
        Script_srchUtils(SRCH_SCRIPT_SetEntrySelectorFilter, task_210);
      }
      if (Script_CheckTargetWithinAngleRange(&data_130EC, 0.3f, 0, 0x8000, &local_F4)) {
        task_210 = 1;
        Script_srchUtils(SRCH_SCRIPT_SetEntrySelectorFilter, task_210);
      }
      if (Script_CheckTargetWithinAngleRange(&data_130F8, 0.3f, 0, 0x8000, &local_F4) | Script_CheckTargetWithinAngleRange(&data_13104, 0.3f, 0, 0x8000, &local_F4)) {
        task_210 = 0;
        Script_srchUtils(SRCH_SCRIPT_SetEntrySelectorFilter, task_210);
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

__attribute__((scn_string_bindings(1, data_1311B, 2, data_13126, 3, data_13131, 4, data_1313C, 5, data_13147, 13, data_1319F, 16, data_131C0, 21, data_131F7)))
static void script_EC8E(int32_t arg0) {
  int8_t local_FF;
  int32_t local_F8;
  for (;;) {
    local_FF = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f));
    if (local_FF != task_4A0) {
      task_4A0 = local_FF;
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  local_F8 = arg0;
  switch (local_F8) {
    case 0:
      local_F8 = task_4A0;
      switch (local_F8) {
        case 0:
          Script_ShowUIMessageFromID("E1009BA007");
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
          break;
        case 1:
          Script_ShowUIMessageFromID("E1009BA008");
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
          break;
        case 2:
          Script_ShowUIMessageFromID("E1009BA009");
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
          break;
      }
      break;
    case 1:
      local_F8 = task_4A0;
      switch (local_F8) {
        case 0:
          Script_ShowUIMessageFromID("E1009BA006");
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
          break;
        case 1:
          Script_ShowUIMessageFromID("E1009BA008");
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
          break;
        case 2:
          Script_ShowUIMessageFromID("E1009BA009");
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
          break;
      }
      break;
    case 2:
      local_F8 = task_4A0;
      switch (local_F8) {
        case 0:
          Script_ShowUIMessageFromID("E1009BA022");
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
          break;
        case 1:
          Script_ShowUIMessageFromID("E1009BA023");
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
          break;
        case 2:
          Script_ShowUIMessageFromID("E1009BA024");
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
          break;
      }
      break;
    case 3:
      local_F8 = task_4A0;
      switch (local_F8) {
        case 0:
          Script_ShowUIMessageFromID("E1009BA034");
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
          break;
        case 1:
          Script_ShowUIMessageFromID("E1009BA029");
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
          break;
        case 2:
          Script_ShowUIMessageFromID("E1009BA030");
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
          break;
      }
      break;
    case 4:
      local_F8 = task_4A0;
      switch (local_F8) {
        case 0:
          Script_ShowUIMessageFromID("E1009BA035");
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
          break;
        case 1:
          Script_ShowUIMessageFromID("E1009BA036");
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
          break;
        case 2:
          Script_ShowUIMessageFromID("E1009BA037");
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
          break;
      }
      break;
    case 5:
      local_F8 = task_4A0;
      switch (local_F8) {
        case 0:
          Script_ShowUIMessageFromID("E1009BA038");
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
          break;
        case 1:
          Script_ShowUIMessageFromID("E1009BA036");
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
          break;
        case 2:
          Script_ShowUIMessageFromID("E1009BA039");
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
          break;
      }
      break;
    case 6:
      local_F8 = task_4A0;
      switch (local_F8) {
        case 0:
          Script_ShowUIMessageFromID("E1009BA040");
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
          break;
        case 1:
          Script_ShowUIMessageFromID("E1009BA026");
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
          break;
        case 2:
          Script_ShowUIMessageFromID("E1009BA041");
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
          break;
      }
      break;
    case 7:
      if (task_20C & 0x40) {
        Script_ShowUIMessageFromID("E1009BA009");
        while (Script_convGetIsActive(0)) {
          SCNF_WaitFrames_0000(1);
        }
      } else {
        Script_ShowUIMessageFromID("E1009BA019");
        while (Script_convGetIsActive(0)) {
          SCNF_WaitFrames_0000(1);
        }
      }
      break;
    case 8:
      Script_ShowUIMessageFromID("E1009BA020");
      while (Script_convGetIsActive(0)) {
        SCNF_WaitFrames_0000(1);
      }
      (void)((task_20C = -1) & 0x10);
      break;
  }
  return;
}

static void script_F14D(int32_t arg0) {
  int32_t local_FC;
  local_FC = arg0;
  if ((local_FC == 0x30304254) | (local_FC == 0x32304254) | (local_FC == 0x30314254) | (local_FC == 0x31314254) | (local_FC == 0x32314254)) {
    if (task_298 == 1) {
      script_EC8E(4);
    }
  } else if (local_FC == 0x3052415A) {
    Script_ryoControl(PCCTL_DisablePlayerControl);
    script_EC8E(5);
    Script_ryoControl(RYO_RestoreControl);
  } else if ((local_FC == 0x304E4550) | (local_FC == 0x314E4550) | (local_FC == 0x304D4E59) | (local_FC == 0x314D4E59) | (local_FC == 0x324D4E59) | (local_FC == 0x344D4E59)) {
    if (task_298 == 1) {
      script_EC8E(6);
    }
  } else if ((local_FC == 0x304F4853) | (local_FC == 0x314F4853)) {
    Script_ryoControl(PCCTL_DisablePlayerControl);
    script_EC8E(3);
    Script_ryoControl(RYO_RestoreControl);
  } else if (local_FC == 0x31545546) {
    if (task_298 == 1) {
      script_EC8E(7);
    }
  } else if (local_FC == 0x30545546) {
    if (task_298 == 1) {
      script_EC8E(8);
    }
  } else if ((local_FC == 0) | (local_FC == 1) | (local_FC == 2) | (local_FC == 4) | (local_FC == 3) | (local_FC == 6)) {
    Script_ryoControl(PCCTL_DisablePlayerControl);
    script_EC8E(0);
    Script_ryoControl(RYO_RestoreControl);
  } else if (local_FC == 5) {
  } else if (local_FC == 7) {
  } else if (local_FC == 8) {
  } else if (local_FC == 9) {
  } else if (local_FC == 10) {
  } else {
    if ((local_FC == 11) | (local_FC == 12) | (local_FC == 13) | (local_FC == 14) | (local_FC == 15)) {
      Script_ryoControl(PCCTL_DisablePlayerControl);
      script_EC8E(2);
      Script_ryoControl(RYO_RestoreControl);
      goto L00F333;
    }
  }
  L00F333:;
  return;
}

static int32_t script_F334(void) {
  Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  return Script_srchControl(0);
}

static void script_F368(void) {
  SAVE_Script_UpdateForSaveData(0, 0);
  SAVE_Script_UpdateForSaveData(3, 0, task_214);
  return;
}

static int32_t script_F380(void) {
  for (;;) {
    if (task_218 >= 0) {
      task_218 = task_218 - 1;
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_F3A3(void) {
  int32_t local_F4[3];
  int32_t local_F0;
  int32_t local_EC;
  local_F4[0] = 0x418DAE14;
  local_F4[1] = 0x4088CCCD;
  local_F4[2] = 0x4318B5C3;
  local_F0 = 0x2000;
  local_EC = 0x40800000;
  script_10706(CHARA_TMHN, 15.56f, 4.275f, 131.09f, 40413);
  script_10706(CHARA_TKSA, 16.7792f, 4.275f, 132.35f, 7094);
  Script_motmGetCharaFlags(CHARA_TMHN, 0, 0);
  Script_motmGetCharaFlags(CHARA_TKSA, 0, 0);
  script_F368();
  Script_scenCreateTask(script_F380, 0);
  for (;;) {
    Script_EVT_GetPosition(CHARA_AKIR, -1, &data_13218, 0);
    Script_mtwkGetSetRotation(CHARA_AKIR, &data_13224, 0x2000000);
    if (~task_208) {
      if (SAVE_Script_UpdateForSaveData(4, 0) < 0) {
        while (Script_TMEXUtilities(0, 0) == 0) {
          SCNF_WaitFrames_0000(1);
        }
        while (Script_TMEXUtilities(1) != 0) {
          SCNF_WaitFrames_0000(1);
        }
        Script_ryoControl(RYO_RestoreControl);
      }
    }
    if (task_218 < 0) {
      while (Script_TMEXUtilities(0, 0) == 0) {
        SCNF_WaitFrames_0000(1);
      }
      while (Script_TMEXUtilities(1) != 0) {
        SCNF_WaitFrames_0000(1);
      }
      Script_ryoControl(RYO_RestoreControl);
    }
    if (Script_IsPlayerAllowed(0)) {
      if (~task_207) {
        if (Script_CheckTargetWithinAngleRange(&data_13230, 2.0f, 0x2000, 0x4000, &data_13218) & -!((&data_13218)->y <= 4.0f)) {
          (void)((task_208 = -1) & 0x40);
          (void)((task_207 = -1) & 0x40);
          script_FC17(0);
        }
      }
      if (~task_207 & ~task_208) {
        if ((SAVE_Script_UpdateForSaveData(4, 0) < 0) & (*&task_1B8 == -1)) {
          Script_EVT_GetPosition(CHARA_AKIR, -1, &data_13218, 0);
          if (!((&data_13218)->y <= 4.0f)) {
            (void)((task_208 = -1) & 1);
            (void)((task_207 = -1) & 1);
            script_FC17(1);
          } else {
            (void)((task_208 = -1) & 1);
            (void)((task_209 = -1) & 1);
            script_101F7();
          }
        }
      }
      if (~task_207) {
        if ((task_218 < 0) & (*&task_1B8 == -1)) {
          (void)((task_208 = -1) & 1);
          (void)((task_207 = -1) & 1);
          script_FC17(1);
        }
      }
      if (~task_209) {
        if (Script_SetTaskDataValue(CHARA_AKIR, &data_1323C, &data_13248, 0) & -!((&data_13218)->y >= 4.0f)) {
          (void)((task_209 = -1) & 1);
          script_F9B9(1);
        }
      }
      if (~task_20A) {
        if (Script_CheckTargetWithinAngleRange(&data_13254, 0.3f, 0, 0x8000, &data_13218) & -!((&data_13218)->y <= 4.0f)) {
          (void)((task_20A = -1) & 8);
          script_F9B9(2);
        }
      }
      if (~task_20B) {
        if (Script_CheckTargetWithinAngleRange(&data_13260, 0.3f, 0, 0x8000, &data_13218) & -!((&data_13218)->y <= 4.0f)) {
          (void)((task_20B = -1) & 0x20);
          script_F9B9(3);
        }
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_F6FB(void) {
  Script_seqEnterEvtCam(1201, 0, AUTH_None);
  SCNF_WaitFrames_0000(62);
  Script_seqEnterEvtCam(1202, 0, AUTH_None);
  SCNF_WaitFrames_0000(35);
  SCNF_WaitFrames_0000(10);
  Script_seqEnterEvtCam(1203, 0, AUTH_None);
  return Script_srchControl(0);
}

static int32_t script_F73D(void) {
  SCNF_WaitFrames_0000(1);
  for (;;) {
    if (!(Script_GetCharaMotionRemainingFramesScaled(CHARA_AKIR) <= 140.0f)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_executeAicaCommand(0x1805A9, 0, 0);
  SCNF_WaitFrames_0000(2);
  Script_executeAicaCommand(0x1805A9, 0, 0);
  SCNF_WaitFrames_0000(1);
  for (;;) {
    if (!(Script_GetCharaMotionRemainingFramesScaled(CHARA_AKIR) <= 255.0f)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_SetCharaLookAtPosition(CHARA_AKIR, 6, &data_1326C, 0);
  return Script_srchControl(0);
}

__attribute__((scn_string_bindings(0, data_13284, 1, data_1328F)))
static void script_F7CE(void) {
  script_10309();
  script_1075F(CHARA_AKIR);
  Script_executeAicaCommand(0x1605A9, 0, 0);
  if (task_205 == 0) {
    script_10706(CHARA_AKIR, 18.42f, 3.58f, 153.52f, 0x2000);
    Script_scenCreateTask(script_F73D, 0);
    Script_scenCreateTask(script_F6FB, 0);
    Script_motmToggleFlags(CHARA_AKIR, 2, 0x40);
    Script_PlayCharaMotion(CHARA_AKIR, 8193, -1.0f, 65.0f, -1.0f, 1000.0f, 0);
    SCNF_WaitFrames_0000(2);
    Script_PlayCharaMotion(CHARA_AKIR, 8193, 66.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_scenCreateTask(script_F334, 0);
    SCNF_WaitFrames_0000(1);
    for (;;) {
      if (!(Script_GetCharaMotionRemainingFramesScaled(CHARA_AKIR) <= 170.0f)) {
        break;
      }
      SCNF_WaitFrames_0000(1);
    }
    Script_ShowUIMessageFromID("E1009BA002");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    SCNF_WaitFrames_0000(1);
    for (;;) {
      if (!(Script_GetCharaMotionRemainingFramesScaled(CHARA_AKIR) <= 265.0f)) {
        break;
      }
      SCNF_WaitFrames_0000(1);
    }
    Script_SetCharaLookAtPosition(CHARA_AKIR, -20, 0, 0);
    Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &data_13278);
    Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  } else {
    Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
    while (Script_filmCheckOrSetTransitionUtil(-1)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_ShowUIMessageFromID("E1009BA002");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
  }
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 2);
  script_10381();
  return;
}

__attribute__((scn_string_bindings(0, data_1329A)))
static void script_F9B9(int32_t arg0) {
  int32_t local_FC;
  script_10309();
  local_FC = arg0;
  switch (local_FC) {
    case 1:
      Script_ccowUtils(CHARA_AKIR, 1);
      SCNF_WaitFrames_0000(30);
      Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
      Script_ShowUIMessageFromID("E1009BA006");
      while (Script_convGetIsActive(0)) {
        SCNF_WaitFrames_0000(1);
      }
      break;
    case 2:
      Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
      SCNF_WaitFrames_0000(30);
      Script_ShowUIMessageFromID("E1009BA028");
      while (Script_convGetIsActive(0)) {
        SCNF_WaitFrames_0000(1);
      }
      break;
    case 3:
      Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
      SCNF_WaitFrames_0000(30);
      Script_ShowUIMessageFromID("E1009BA003");
      while (Script_convGetIsActive(0)) {
        SCNF_WaitFrames_0000(1);
      }
      break;
  }
  script_10381();
  return;
}

static int32_t script_FAD3(void) {
  Script_seqEnterEvtCam(1102, 0, AUTH_None);
  SCNF_WaitFrames_0000(350);
  Script_seqEnterEvtCam(1103, 0, AUTH_None);
  return Script_srchControl(0);
}

static int32_t script_FAFB(void) {
  SCNF_WaitFrames_0000(1);
  for (;;) {
    if (!(Script_GetCharaMotionRemainingFramesScaled(CHARA_AKIR) <= 46.0f)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_executeAicaCommand(460201, 0, 0);
  SCNF_WaitFrames_0000(1);
  for (;;) {
    if (!(Script_GetCharaMotionRemainingFramesScaled(CHARA_AKIR) <= 63.0f)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_executeAicaCommand(460201, 0, 0);
  SCNF_WaitFrames_0000(1);
  for (;;) {
    if (!(Script_GetCharaMotionRemainingFramesScaled(CHARA_AKIR) <= 88.0f)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_executeAicaCommand(460201, 0, 0);
  SCNF_WaitFrames_0000(1);
  for (;;) {
    if (!(Script_GetCharaMotionRemainingFramesScaled(CHARA_AKIR) <= 110.0f)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_executeAicaCommand(460201, 0, 0);
  SCNF_WaitFrames_0000(1);
  for (;;) {
    if (!(Script_GetCharaMotionRemainingFramesScaled(CHARA_AKIR) <= 139.0f)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_executeAicaCommand(460201, 0, 0);
  return Script_srchControl(0);
}

static void script_FC17(int32_t arg0) {
  int32_t local_F4[3];
  int32_t local_F0;
  int32_t unused_EC;
  Vector3i local_E0;
  script_10309();
  script_1075F(CHARA_AKIR);
  Script_motmSetBlendFrames(CHARA_AKIR, MOTM_BLEND_FRAMES_15);
  if (arg0) {
    Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
    while (Script_filmCheckOrSetTransitionUtil(-1)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &data_132BC);
    local_F4[1] = 0x2000;
    Script_EVT_SetRotation(CHARA_AKIR, MATRIX_XFORM_Y, (const Vector3i *)local_F4, 0);
    Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_seqEnterEvtCam(1101, CHARA_AKIR, CHARA_AKIR);
    Script_convSetCamInactiveFlag(1);
    SCNF_WaitFrames_0000(1);
    Script_SetGameStateTo(AUTH_None);
    SCNF_WaitFrames_0000(1);
    if (Script_scenSetAssignCVWK(CHARA_AKIR, 1) == 0) {
      Script_scenSetAssignCVWK(CHARA_AKIR, 0, 1);
    }
    Script_srchUtils(SRCH_SCRIPT_SetEntrySelectorFilter, 0);
    SCNF_WaitFrames_0000(10);
  }
  Script_scenCharaSubmitMoveTargetRequest(CHARA_AKIR, &data_132C8, 0x2000, MN_AKI_AKI_STAND_LP, 0);
  Script_seqEnterEvtCam(1105, 0, AUTH_None);
  SCNF_WaitFrames_0000(1);
  if (arg0) {
    Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
    while (Script_filmCheckOrSetTransitionUtil(-1)) {
      SCNF_WaitFrames_0000(1);
    }
  }
  while (Script_IsMoveTargetRequestActive(CHARA_AKIR) != 0) {
    SCNF_WaitFrames_0000(1);
  }
  Script_seqEnterEvtCam(1106, 0, AUTH_None);
  local_F4[1] = 5097;
  Script_EVT_SetRotation(CHARA_AKIR, MATRIX_XFORM_Y, (const Vector3i *)local_F4, 0);
  Script_ShowUIMessageFromID("E1009BA013");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_PlayCharaMotion(CHARA_AKIR, 8194, 40.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_scenCreateTask(script_FAFB, 0);
  Script_FaceLookAtTargetUtils(CHARA_AKIR, 2, &data_132EC, 60);
  SCNF_WaitFrames_0000(1);
  for (;;) {
    if (!(Script_GetCharaMotionRemainingFramesScaled(CHARA_AKIR) <= 160.0f)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_SetCharaMotionPlaybackRate(CHARA_AKIR, 0.0f);
  Script_PlayCharaMotion(CHARA_TKSA, 4097, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_SetCharaMotionPlaybackRate(CHARA_TKSA, 0.5f);
  Script_motmToggleFlags(CHARA_TMHN, 1, 0x80);
  Script_PlayCharaMotion(CHARA_TMHN, 4098, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_scenCreateTask(script_FAD3, 0);
  Script_ShowUIMessageFromID("E1009BB001");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_ShowUIMessageFromID("E1009BC001");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_ShowUIMessageFromID("E1009BB002");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_ShowUIMessageFromID("E1009BC002");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_ShowUIMessageFromID("E1009BC003");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_SetCharaMotionPlaybackRate(CHARA_TKSA, 0.0f);
  Script_ShowUIMessageFromID("E1009BB003");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_ShowUIMessageFromID("E1009BC004");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  SCNF_WaitFrames_0000(30);
  Script_seqEnterEvtCam(1104, 0, AUTH_None);
  Script_FACE(CHARA_AKIR, 2, 0, 60);
  Script_ShowUIMessageFromID("E1009BA014");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  SCNF_WaitFrames_0000(30);
  Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_SetCharaMotionPlaybackRate(CHARA_TKSA, 1.0f);
  Script_motmToggleFlags(CHARA_AKIR, 2, 0x40);
  Script_SetCharaLookAtPosition(CHARA_AKIR, -20, 0, 0);
  local_F4[1] = 2184;
  Script_EVT_SetRotation(CHARA_AKIR, MATRIX_XFORM_Y, (const Vector3i *)local_F4, 0);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_FACE(CHARA_AKIR, 0, 0, 0);
  Script_FaceLookAtTargetUtils(CHARA_AKIR, 0);
  local_F4[0] = 0x418A0000;
  local_F4[1] = 0x4088CCCD;
  local_F4[2] = 0x43184000;
  local_F0 = 0xA000;
  Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, (const Vector3 *)local_F4);
  (&local_E0)->y = local_F0;
  Script_EVT_SetRotation(CHARA_AKIR, MATRIX_XFORM_Y, &local_E0, 0);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  SCNF_WaitFrames_0000(1);
  Script_seqEnterEvtCam(1300, CHARA_AKIR, CHARA_AKIR);
  if (Script_scenSetAssignCVWK(CHARA_AKIR, 1) == 0) {
    Script_scenSetAssignCVWK(CHARA_AKIR, 0, 1);
  }
  Script_srchUtils(SRCH_SCRIPT_SetEntrySelectorFilter, 0);
  Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_ShowUIMessageFromID("E1009BA017");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_executeAicaCommand(2720, 1, 30);
  Script_executeAicaCommand(2720, 2, 30);
  Script_executeAicaCommand(2720, 3, 30);
  Script_filmCheckOrSetTransitionUtil(60, 0, 0, 0, 0, 255, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_MS8S_MAIN_EVENT_COMPLETE, 1);
  Script_shenRequestGameModuleTransition(2, SHEN_NAME('MFSY'), 5);
  SCNF_WaitFrames_0000(1);
  return;
}

static void script_101F7(void) {
  script_10309();
  script_1075F(CHARA_AKIR);
  Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &data_13374);
  Script_EVT_SetFacingTowardPoint(CHARA_AKIR, 0x38000000, &data_13380, 0);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_SetCharaLookAtPosition(CHARA_AKIR, 6, &data_13380, 0);
  Script_seqEnterEvtCam(1000, CHARA_AKIR, -1);
  Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_ShowUIMessageFromID("E1009BA018");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_SetCharaLookAtPosition(CHARA_AKIR, -6, 0, 0);
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 2);
  script_10381();
  return;
}

static void script_10309(void) {
  Vector3i local_F4;
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_FaceEx(CHARA_AKIR, 1, 0, 2);
  Script_ResetHandMotion(CHARA_AKIR, -1, 0, 0);
  Script_ResetHandMotion(CHARA_AKIR, -1, 1, 0);
  Script_shenClearFreeState(0);
  Script_FaceLookAtTargetUtils(CHARA_AKIR, 0);
  Script_mtwkGetSetRotation(CHARA_AKIR, &local_F4, 0x2000000);
  Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &local_F4, 0);
  Script_ccowUtils(CHARA_AKIR, 2);
  return;
}

static void script_10381(void) {
  Script_ccowUtils(CHARA_AKIR, 1);
  Script_FACE(CHARA_AKIR, 0, 0, 0);
  Script_FaceLookAtTargetUtils(CHARA_AKIR, 0);
  Script_convSetCamInactiveFlag(1);
  Script_SetGameStateTo(AUTH_None);
  SCNF_WaitFrames_0000(1);
  Script_motmToggleFlags(CHARA_AKIR, 1, 0x40);
  Script_ryoControl(RYO_RestoreControl);
  Script_FaceEx(CHARA_AKIR, 0, 0, 2);
  Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
  Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
  Script_shenClearFreeState(1);
  return;
}

__attribute__((scn_string_bindings(1, data_1339E, 2, data_133A3, 3, data_133A8, 4, data_133AD, 5, data_133B2)))
void script_10409(void) {
  int32_t unused_FC;
  Script_ryoSleepUtils(2, 1);
  if (Script_ryoSleepUtils(0) == -1) {
    (void)((task_206 = -1) & 0x20);
  } else {
    (void)((task_206 = 0) & 0x20);
  }
  Script_filmCheckOrSetTransitionUtil(0, 255, 0, 0, 0, 0, 0, 0, 0);
  Script_SetMapFlags(0, 0);
  Script_SetShouldSleepFlag(0);
  Script_ReturnTimeDate(&task_1FC);
  task_205 = Script_GetCurrentEntryPoint();
  Script_PlayerStatsUtils(PS_IncrementSpotVisitCount, 83);
  Script_LoadVoiceFile(1, "E1009B");
  Script_facemotUtils(CHARA_AKIR, 0, 0);
  Script_LoadSND_Wrapper(0, 0, "FREE", "FREE", "FREE", "FREE", "FREE", 0);
  Script_LoadSND_Wrapper(0, 0, "fre1100.snd", 0, "f1new8so.snd", 0, 0, 0);
  Script_executeAicaCommand(16296, 0, 0);
  Script_executeAicaCommand(1184, 2, 100);
  script_83A0(CHARA_AKIR, (DSWKDoorEntry *)data_1281C, (int32_t *)data_128EC, (DoorResourceDescriptor *)&data_12924, (MapObjectRecord *)data_12954, 3, 0);
  SCNF_WaitFrames_0000(2);
  DSWK_DoorUtils(DOOR_CMD_SET_STATE_1);
  SCNF_WaitFrames_0000(2);
  script_8766(2);
  SCNF_WaitFrames_0000(2);
  Script_scenCreateTask(script_A297, 6, CHARA_AKIR, 2, 0, &data_12B44, 2, 0);
  script_C8AA((int32_t)data_12B78);
  script_10777();
  Script_scenCreateTask(script_10937, 0);
  SCNF_WaitFrames_0000(1);
  script_E8F8();
  Script_scenCreateTask(script_EB70, 0);
  Script_GetSetWeatherSettings(WEATHER_SETTINGS_SET_DISABLE_WEATHER, 1);
  Script_WeatherUtils_0(WeatherDataUtil_ApplyCurrentWeatherOrEnableDateTimeRead, 0);
  Script_WeatherTaskUtil(InitTaskData);
  script_10746();
  Script_WeatherTaskUtil(ToggleFogTask, 0);
  Script_WeatherTaskUtil(ToggleSceneLightingFlags, 0);
  Script_SetGlobalSceneSlotValue34(2, 1);
  SCNF_WaitFrames_0000(1);
  Script_SetupLightFromTable(0);
  Script_ProcessFogFromTable(0);
  Script_PREP_Related(-1, 1);
  Script_PREP_Related(2, 0);
  Script_SetCharaFlags(CHARA_AKIR, 4);
  Script_SetShadowFlags(CHARA_AKIR, SCRIPT_SHADOW_UPDATE_LIGHT_MASK, 1);
  Script_SetShadowFlags(CHARA_AKIR, SCRIPT_SHADOW_UPDATE_LIGHT_MASK, 2);
  Script_SetShadowFlags(CHARA_AKIR, SCRIPT_SHADOW_SET_LIGHT_RADIUS, 30.0f);
  script_1075F(CHARA_AKIR);
  Script_cdttLoadScrUtils(1, 0);
  if (task_206 & 1) {
    Script_ryoSleepUtils(1);
    Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
    while (Script_filmCheckOrSetTransitionUtil(-1)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_ryoControl(RYO_RestoreControl);
    Script_FaceEx(CHARA_AKIR, 0, 0, 2);
    Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
    Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
    Script_shenClearFreeState(1);
  } else {
    Script_convSetCamInactiveFlag(1);
    SCNF_WaitFrames_0000(1);
    script_F7CE();
  }
  Script_scenCreateTask(script_F3A3, 0);
  return;
}

static void script_106CC(const char *arg0, const char *arg1) {
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, arg0, arg1) == 0) {
    SCNF_WaitFrames_0000(1);
  }
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_10706(int32_t arg0, float arg1, float arg2, float arg3, int32_t arg4) {
  Vector3 local_F4;
  int32_t unused_E8[3];
  Vector3i local_DC;
  local_F4.x = arg1;
  (&local_F4)->y = arg2;
  (&local_F4)->z = arg3;
  Script_EVT_SetPosition(arg0, POS_XYZ, &local_F4);
  (&local_DC)->y = arg4;
  Script_EVT_SetRotation(arg0, MATRIX_XFORM_Y, &local_DC, 0);
  return;
}

static void script_10746(void) {
  SCNF_WaitFrames_0000(1);
  Script_WeatherUtils(WeatherScript_LoadWccpClipSet0FromTaskOffset, &data_133D0, 1);
  return;
}

static void script_1075F(int32_t arg0) {
  Script_charaSetRefboardFlagTo(arg0, 2);
  Script_SetCharaRefBoardPos(arg0, &data_13408);
  return;
}

static void script_10777(void) {
  (void)((task_207 = 0) & 4);
  (void)((task_208 = 0) & 4);
  if (task_205 == 0) {
    (void)((task_209 = 0) & 4);
  } else {
    (void)((task_209 = -1) & 4);
  }
  (void)((task_20B = 0) & 4);
  (void)((task_20A = 0) & 4);
  (void)((task_20C = 0) & 4);
  task_210 = 0;
  task_214 = 14400;
  task_218 = 28800;
  if (task_206 & 1) {
    (void)((task_207 = -(Script_ryoSleepUtils(3, 0, 0) != 0)) & 4);
    (void)((task_208 = -(Script_ryoSleepUtils(3, 0, 1) != 0)) & 4);
    (void)((task_209 = -(Script_ryoSleepUtils(3, 0, 2) != 0)) & 4);
    (void)((task_20A = -(Script_ryoSleepUtils(3, 0, 3) != 0)) & 4);
    (void)((task_20B = -(Script_ryoSleepUtils(3, 0, 4) != 0)) & 4);
    (void)((task_20C = -(Script_ryoSleepUtils(3, 0, 5) != 0)) & 4);
    task_210 = Script_ryoSleepUtils(3, 0, 6);
    task_214 = (Script_ryoSleepUtils(3, 0, 7) << 8) + Script_ryoSleepUtils(3, 0, 8);
    task_218 = (Script_ryoSleepUtils(3, 0, 9) << 8) + Script_ryoSleepUtils(3, 0, 10);
    Script_sprintf("res_tachikiki_end,%d\012", task_207);
    Script_sprintf("res_tachikiki_kyusai_end,%d\012", task_208);
    Script_sprintf("res_1kai_hitorigoto,%d\012", task_209);
    Script_sprintf("res_left_hitorigoto,%d\012", task_20A);
    Script_sprintf("res_right_hitorigoto,%d\012", task_20B);
    Script_sprintf("res_futo,%d\012", task_20C);
    Script_sprintf("res_search,%d\012", task_210);
    Script_sprintf("res_game,%d\012", task_214);
    Script_sprintf("res_limit,%d\012", task_218);
  }
  return;
}

static int32_t script_10937(void) {
  int8_t local_FF;
  int8_t local_FE;
  int8_t local_FD;
  int8_t local_FC;
  for (;;) {
    Script_ryoSleepUtils(3, 1, 0, task_207);
    Script_ryoSleepUtils(3, 1, 1, task_208);
    Script_ryoSleepUtils(3, 1, 2, task_209);
    Script_ryoSleepUtils(3, 1, 3, task_20A);
    Script_ryoSleepUtils(3, 1, 4, task_20B);
    Script_ryoSleepUtils(3, 1, 5, task_20C);
    Script_ryoSleepUtils(3, 1, 6, task_210);
    if (SAVE_Script_UpdateForSaveData(4, 0) <= 0) {
      local_FE = 0;
      local_FF = 0;
    } else {
      local_FF = SAVE_Script_UpdateForSaveData(4, 0) & 0xFF;
      local_FE = SAVE_Script_UpdateForSaveData(4, 0) >> 8;
    }
    Script_ryoSleepUtils(3, 1, 7, local_FE);
    Script_ryoSleepUtils(3, 1, 8, local_FF);
    if (task_218 <= 0) {
      local_FC = 0;
      local_FD = 0;
    } else {
      local_FD = task_218 & 0xFF;
      local_FC = task_218 >> 8;
    }
    Script_ryoSleepUtils(3, 1, 9, local_FC);
    Script_ryoSleepUtils(3, 1, 10, local_FD);
    SCNF_WaitFrames_0000(1);
  }
  end:;
}
