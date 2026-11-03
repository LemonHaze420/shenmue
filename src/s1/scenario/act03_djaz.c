#include "act03_djaz.h"

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
static void script_9648(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_97FC(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_9A12(int32_t arg0, int32_t arg1);
static void script_9ADC(int32_t arg0, int32_t arg1);
static void script_9B3D(int32_t arg0);
static void script_9CE2(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_9D7A(int32_t arg0, int32_t arg1);
static void script_9E5C(int32_t arg0);
static int32_t script_9EB3(void);
static void script_9F63(int32_t arg0, int32_t arg1);
static int32_t script_9F73(int32_t arg0);
static int32_t script_9F87(int32_t arg0);
static int32_t script_9FA9(int32_t arg0);
static int32_t script_9FBB(int32_t arg0);
static int32_t script_9FCD(int32_t arg0, int32_t arg1);
static void script_A017(void);
static int32_t script_A0FE(int32_t arg0);
static void script_A12E(void);
static void script_A15E(void);
static void script_A18E(void);
static void script_A459(void);
static void script_AC72(int32_t arg0);
static void script_AFAA(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_B848(int32_t arg0);
static void script_BC27(int32_t arg0);
static void script_C256(int32_t arg0);
static void script_C41A(void);
static void script_C583(void);
static void script_C821(void);
static void script_C93A(void);
static void script_C9FA(void);
static int32_t script_CA5D(void);
static void script_CAF2(void);
static void script_CB20(void);
static void script_CB4E(void);
static void script_CB5C(void);
static void script_CB5F(void);
static void script_CB92(void);
static void script_CBED(void);
static void script_CC4B(void);
static void script_CCA6(void);
static int32_t script_CD0E(void);
static int32_t script_CD54(void);
static int32_t script_CD9A(void);
static int32_t script_CDD4(void);
static int32_t script_CDFB(void);
static int32_t script_CE31(void);
static int32_t script_CE58(void);
static int32_t script_CE8E(void);
static int32_t script_CEB5(void);
static int32_t script_CEEB(void);
static int32_t script_CF12(void);
static int32_t script_CFB5(void);
static int32_t script_D110(void);
static int32_t script_D224(void);
static int32_t script_D2B7(void);
static int32_t script_D36A(void);
static int32_t script_D3FD(void);
static int32_t script_D6B8(void);
static void script_D74B(void);
static int32_t script_D77D(void);
static void script_D7FC(void);
static void script_D815(void);
static void script_D82E(void);
static void script_D8A9(void);
static int32_t script_D8ED(void);
static void script_D9AE(void);
static void script_D9D7(void);
static int32_t script_DA5B(int8_t arg0);
static void script_DBEF(int32_t arg0);
static void script_DD7F(void);
static void script_DD99(void);
static void script_DDD2(void);
static void script_DDDA(void);
static int32_t script_E100(void);
static int32_t script_E386(void);
static void script_E522(int32_t arg0, int32_t arg1);
static int32_t script_E583(int32_t arg0);
static void script_E902(int32_t arg0, int32_t arg1);
static void script_E9BE(int32_t arg0, int32_t arg1);
static void script_ED66(int32_t arg0, int32_t arg1, const Vector3 *arg2, const Vector3 *arg3, const Vector3 *arg4);
static void script_EF06(int32_t arg0, int32_t arg1, const Vector3 *arg2, const Vector3 *arg3, const Vector3 *arg4);
static int8_t script_F0A6(int32_t arg0);
static int8_t script_F892(int32_t arg0, float arg1, float arg2, int32_t arg3, int32_t arg4);
static void script_FF02(int32_t arg0);
static int32_t script_10224(int32_t arg0, int32_t arg1, float arg2);
static int32_t script_105B5(void);
static int32_t script_10667(void);
static int32_t script_1075E(void);
static void script_107FF(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_10814(const void *arg0);
static void script_10834(int32_t arg0, int32_t arg1, Vector3 *arg2, Vector3i *arg3);
static void script_10949(Vector3 *arg0, float arg1, float arg2, float arg3);
static void script_10964(Vector3 *arg0, float arg1, float arg2, float arg3);
static void script_10991(Vector3i *arg0, float arg1, float arg2, float arg3);
static void script_109D6(Vector3i *arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_10A03(int32_t arg0, float arg1, float arg2, float arg3);
static void script_10A5A(void);
static void script_10AC8(void);
static void script_10AEF(void);
static int32_t script_10B4C(int32_t arg0, const Vector3 *arg1, float arg2, float arg3);
static int32_t script_10B90(int32_t arg0);
static void script_1101F(int32_t arg0);
static void script_11087(int32_t arg0);
static void script_110D9(int32_t arg0);
static void script_11141(int32_t arg0, float arg1, float arg2, float arg3);
static void script_11169(int32_t arg0, float arg1, float arg2, float arg3);
static void script_111BD(int32_t arg0, int32_t arg1, int32_t arg2);
static int32_t script_1120D(int32_t arg0);
static void script_112B4(int32_t arg0, int32_t arg1);
static int32_t script_112D7(int32_t arg0);
static void script_1130A(int32_t arg0, int32_t arg1);
static void script_1132D(int32_t arg0, int32_t arg1);
static void script_1138C(int32_t arg0);
static void script_113A1(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_11737(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_11DC5(void);
static void script_11DD4(void);
static void script_11DE3(void);
static void script_11DE6(void);
static void script_11DE9(void);
static void script_11DEC(void);
static void script_11DEF(void);
static int32_t script_11DFC(void);
static void script_11E08(void);
static int32_t script_11E17(void);
static void script_11E26(void);
static void script_11F5E(int32_t arg0);
static void script_11F6A(int32_t arg0);
static void script_11F76(int32_t arg0);
static int32_t script_12072(int32_t arg0);
static void script_12089(int32_t arg0);
static void script_120CA(int32_t arg0);
static void script_1210E(int32_t arg0);
static void script_1218C(void);
void script_1220B(void);
static int32_t script_123CA(void);
static int32_t script_12444(void);

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
static int32_t task_20C;
static int32_t task_210;
static int32_t task_214;
static int32_t task_218;
static int32_t task_21C;
static int32_t task_220;
static int32_t task_224;
static int32_t task_228;
static int32_t task_22C;
static int32_t task_230;
static int32_t task_234[5];
static int32_t task_248;
static float task_24C;
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
static int32_t task_27C;
static int32_t task_280;
static int32_t task_284;
static int32_t task_288;
static float task_28C;
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
static Vector3 task_2D8;
static Vector3 task_2E4;
static int8_t task_2F0;
static int8_t task_2F1;
static int8_t task_2F2;
static int8_t task_2F3;
static int32_t task_2F4;
static int8_t task_2F8;
static int8_t task_2F9;
static int8_t task_2FA;
static int8_t task_2FB;
static int8_t task_2FC;
static uint8_t task_2FD;
static uint16_t task_2FE;
static int32_t task_300;
static int32_t task_304;
static int32_t task_308;
static int32_t task_30C;
static int32_t task_310;
static int32_t task_314;
static int32_t task_318;
static int32_t task_31C;
static int32_t task_320;
static int8_t task_324;
static uint8_t task_325;
static uint16_t task_326;
static const float data_1250C[3] = {1.0f, 1.0f, 1.0f};
static const int32_t data_12518[4] = {16777215, 16777215, 16777215, 16777215};
static const float data_12528[3] = {20.0f, 800.0f, 1.0f};
static const int32_t data_12534 = -1;
static DoorMotionSelectorBlock data_12538[38] = {
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
static ScnDoorMotionDescriptor data_12B28[72] = {
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
static uint32_t data_13548 = 0xFFFFFFFF;
typedef struct Row_1354C { float w0; float w1; float w2; float w3; int32_t w4; } Row_1354C;
static const Row_1354C data_1354C[7] = {
  {0.0f, 0.0f, 0.0f, 0.0f, -1},
  {0.0f, 0.0f, 0.0f, 0.0f, -1},
  {0.0f, 0.0f, 0.0f, 0.0f, 1534},
  {26.0f, 55.0f, 45.0f, 75.0f, 1555},
  {61.0f, 146.0f, 45.0f, 50.0f, 1554},
  {59.0f, 124.0f, 45.0f, 70.0f, 1543},
  {33.0f, 49.0f, 45.0f, 50.0f, 1543}
};
static const float data_135D8[4] = {33.0f, 49.0f, 50.0f, 50.0f};
static void (*data_135E8)(int32_t, int32_t, int32_t) = script_0768;
static void (*const data_135EC)(int32_t, int32_t, int32_t, int32_t) = script_0778;
static void (*const data_135F0)(int32_t, int32_t, int32_t, int32_t) = script_0778;
static void (*const data_135F4)(int32_t, int32_t, int32_t, int32_t) = script_0778;
static void (*const data_135F8)(int32_t, int32_t, int32_t, int32_t) = script_0778;
static void (*const data_135FC)(int32_t, int32_t, int32_t, int32_t) = script_0778;
static void (*const data_13600)(int32_t, int32_t, int32_t, int32_t) = script_0778;
static void (*const data_13604)(int32_t, int32_t, int32_t, int32_t) = script_0778;
static void (*const data_13608)(int32_t, int32_t, int32_t, int32_t) = script_0778;
static void (*const data_1360C)(int32_t, int32_t, int32_t, int32_t) = script_0A1E;
static void (*const data_13610)(int32_t, int32_t, int32_t, int32_t) = script_0B7F;
static void (*const data_13614)(int32_t, int32_t, int32_t, int32_t) = script_104C;
static void (*const data_13618)(int32_t, int32_t, int32_t, int32_t) = script_10B3;
static void (*const data_1361C)(int32_t, int32_t, int32_t, int32_t) = script_087C;
static void (*const data_13620)(int32_t, int32_t, int32_t, int32_t) = script_087C;
static void (*const data_13624)(int32_t, int32_t, int32_t, int32_t) = script_0CE0;
static void (*const data_13628)(int32_t, int32_t, int32_t, int32_t) = script_0F7E;
static void (*const data_1362C)(int32_t, int32_t, int32_t, int32_t) = script_111A;
static void (*const data_13630)(int32_t, int32_t, int32_t, int32_t) = script_0A1E;
static void (*const data_13634)(int32_t, int32_t, int32_t, int32_t) = script_0B7F;
static void (*const data_13638)(int32_t, int32_t, int32_t, int32_t) = script_0CE0;
static void (*const data_1363C)(int32_t, int32_t, int32_t, int32_t) = script_0A1E;
static void (*const data_13640)(int32_t, int32_t, int32_t, int32_t) = script_0B7F;
static void (*const data_13644)(int32_t, int32_t, int32_t, int32_t) = script_0CE0;
static void (*const data_13648)(int32_t, int32_t, int32_t, int32_t) = script_1161;
static void (*const data_1364C)(int32_t, int32_t, int32_t, int32_t) = script_1161;
static void (*const data_13650)(int32_t, int32_t, int32_t, int32_t) = script_1161;
static void (*const data_13654)(int32_t, int32_t, int32_t, int32_t) = script_1161;
static void (*const data_13658)(int32_t, int32_t, int32_t, int32_t) = script_1297;
static void (*const data_1365C)(int32_t, int32_t, int32_t, int32_t) = script_1B58;
static void (*const data_13660)(int32_t, int32_t, int32_t, int32_t) = script_0778;
static void (*const data_13664)(int32_t, int32_t, int32_t, int32_t) = script_0778;
static void (*const data_13668)(int32_t, int32_t, int32_t, int32_t) = script_0778;
static void (*const data_1366C)(int32_t, int32_t, int32_t, int32_t) = script_0778;
static void (*const data_13670)(int32_t, int32_t, int32_t, int32_t) = script_0778;
static void (*const data_13674)(int32_t, int32_t, int32_t, int32_t) = script_0778;
static void (*const data_13678)(int32_t, int32_t, int32_t, int32_t) = script_0778;
static void (*const data_1367C)(int32_t, int32_t, int32_t, int32_t) = script_0778;
static const char data_13680[46] __attribute__((scn_string_literal)) = "Door Index = %d : call_type = %d : type = %d\012";
static const char data_136AE[43] __attribute__((scn_string_literal)) = "Door Open Motion = DEB_AKESIME_FUSUMA_L_F\012";
static const char data_136D9[43] __attribute__((scn_string_literal)) = "Door Open Motion = DEB_AKESIME_FUSUMA_R_F\012";
static const char data_13704[42] __attribute__((scn_string_literal)) = "Door Open Motion = DEB_HIKISIME_DOOR_L_F\012";
static const char data_1372E[42] __attribute__((scn_string_literal)) = "Door Open Motion = DEB_HIKISIME_DOOR_R_F\012";
static const char data_13758[41] __attribute__((scn_string_literal)) = "Door Open Motion = DEB_OSISIME_DOOR_L_F\012";
static const char data_13781[41] __attribute__((scn_string_literal)) = "Door Open Motion = DEB_OSISIME_DOOR_R_F\012";
static const char data_137AA[43] __attribute__((scn_string_literal)) = "Door Open Motion = GAK_AKESIME_FUSUMA_L_F\012";
static const char data_137D5[43] __attribute__((scn_string_literal)) = "Door Open Motion = GAK_AKESIME_FUSUMA_R_F\012";
static const char data_13800[42] __attribute__((scn_string_literal)) = "Door Open Motion = GAK_HIKISIME_DOOR_L_F\012";
static const char data_1382A[42] __attribute__((scn_string_literal)) = "Door Open Motion = GAK_HIKISIME_DOOR_R_F\012";
static const char data_13854[41] __attribute__((scn_string_literal)) = "Door Open Motion = GAK_OSISIME_DOOR_L_F\012";
static const char data_1387D[41] __attribute__((scn_string_literal)) = "Door Open Motion = GAK_OSISIME_DOOR_R_F\012";
static const char data_138A6[43] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_AKESIME_FUSUMA_L_F\012";
static const char data_138D1[43] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_AKESIME_FUSUMA_R_F\012";
static const char data_138FC[42] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_HIKISIME_DOOR_L_F\012";
static const char data_13926[42] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_HIKISIME_DOOR_R_F\012";
static const char data_13950[41] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_OSISIME_DOOR_L_F\012";
static const char data_13979[41] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_OSISIME_DOOR_R_F\012";
static const char data_139A2[43] __attribute__((scn_string_literal)) = "Door Open Motion = KOD_AKESIME_FUSUMA_L_F\012";
static const char data_139CD[43] __attribute__((scn_string_literal)) = "Door Open Motion = KOD_AKESIME_FUSUMA_R_F\012";
static const char data_139F8[42] __attribute__((scn_string_literal)) = "Door Open Motion = KOD_HIKISIME_DOOR_L_F\012";
static const char data_13A22[42] __attribute__((scn_string_literal)) = "Door Open Motion = KOD_HIKISIME_DOOR_R_F\012";
static const char data_13A4C[41] __attribute__((scn_string_literal)) = "Door Open Motion = KOD_OSISIME_DOOR_L_F\012";
static const char data_13A75[41] __attribute__((scn_string_literal)) = "Door Open Motion = KOD_OSISIME_DOOR_R_F\012";
static const char data_13A9E[43] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_AKESIME_FUSUMA_L_F\012";
static const char data_13AC9[43] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_AKESIME_FUSUMA_R_F\012";
static const char data_13AF4[42] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_HIKISIME_DOOR_L_F\012";
static const char data_13B1E[42] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_HIKISIME_DOOR_R_F\012";
static const char data_13B48[41] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_OSISIME_DOOR_L_F\012";
static const char data_13B71[41] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_OSISIME_DOOR_R_F\012";
static const char data_13B9A[43] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_AKESIME_FUSUMA_L_F\012";
static const char data_13BC5[43] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_AKESIME_FUSUMA_R_F\012";
static const char data_13BF0[42] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_HIKISIME_DOOR_L_F\012";
static const char data_13C1A[42] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_HIKISIME_DOOR_R_F\012";
static const char data_13C44[41] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_OSISIME_DOOR_L_F\012";
static const char data_13C6D[41] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_OSISIME_DOOR_R_F\012";
static const char data_13C96[43] __attribute__((scn_string_literal)) = "Door Open Motion = SIN_AKESIME_FUSUMA_L_F\012";
static const char data_13CC1[43] __attribute__((scn_string_literal)) = "Door Open Motion = SIN_AKESIME_FUSUMA_R_F\012";
static const char data_13CEC[42] __attribute__((scn_string_literal)) = "Door Open Motion = SIN_HIKISIME_DOOR_L_F\012";
static const char data_13D16[42] __attribute__((scn_string_literal)) = "Door Open Motion = SIN_HIKISIME_DOOR_R_F\012";
static const char data_13D40[41] __attribute__((scn_string_literal)) = "Door Open Motion = SIN_OSISIME_DOOR_L_F\012";
static const char data_13D69[41] __attribute__((scn_string_literal)) = "Door Open Motion = SIN_OSISIME_DOOR_R_F\012";
static const char data_13D92[43] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_AKESIME_FUSUMA_L_F\012";
static const char data_13DBD[43] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_AKESIME_FUSUMA_R_F\012";
static const char data_13DE8[42] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_HIKISIME_DOOR_L_F\012";
static const char data_13E12[42] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_HIKISIME_DOOR_R_F\012";
static const char data_13E3C[41] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_OSISIME_DOOR_L_F\012";
static const char data_13E65[41] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_OSISIME_DOOR_R_F\012";
static const char data_13E8E[43] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_AKESIME_FUSUMA_L_F\012";
static const char data_13EB9[43] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_AKESIME_FUSUMA_R_F\012";
static const char data_13EE4[42] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_HIKISIME_DOOR_L_F\012";
static const char data_13F0E[42] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_HIKISIME_DOOR_R_F\012";
static const char data_13F38[41] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_OSISIME_DOOR_L_F\012";
static const char data_13F61[41] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_OSISIME_DOOR_R_F\012";
static const char data_13F8A[44] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_OSISIME_DOOR_AP_L_F\012";
static const char data_13FB6[44] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_OSISIME_DOOR_AP_L_F\012";
static const char data_13FE2[44] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_OSISIME_DOOR_AP_L_F\012";
static const char data_1400E[44] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_OSISIME_DOOR_AP_L_F\012";
static const char data_1403A[44] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_OSISIME_DOOR_AP_L_F\012";
static const char data_14066[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_14082[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_1409E[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_140BA[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_140D6[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_140F2[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_1410E[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_1412A[13] __attribute__((scn_string_literal)) = "open-move!!\012";
static const char data_14137[11] __attribute__((scn_string_literal)) = "M_MDOR.BIN";
static const char data_14142[] __attribute__((scn_string_literal)) = "This Charactor Work is NULL!!\n";
static const char data_14161[22] __attribute__((scn_string_literal)) = "手動ドア開け番号指定\012";
static const char data_14177[34] __attribute__((scn_string_literal)) = "無限ループになりそうなので抜ける\012";
static const char data_14199[] __attribute__((scn_string_literal)) = "Don't make BOX Buffer!!\n";
static const char data_141B2[] __attribute__((scn_string_literal)) = "Don't make BOX Buffer!!\n";
static const char data_141CB[15] __attribute__((scn_string_literal)) = "Door System : ";
static const char data_141DA[15] __attribute__((scn_string_literal)) = "Door System : ";
static const char data_141E9[18] __attribute__((scn_string_literal)) = "Door Box is ON!!\012";
static const char data_141FB[19] __attribute__((scn_string_literal)) = "Door Box is OFF!!\012";
static const char data_1420E[18] __attribute__((scn_string_literal)) = "Door Box is ON!!\012";
static const char data_14220[20] __attribute__((scn_string_literal)) = "Door Box is OFF!!\012";
static const DSWKDoorEntry data_14234 = {.slot = 0, .flags = 0, .ctrl_mask_pair = 0x640064U, .coarse_metric = 0.0f, .selection_metric = 0.0f, .view_ang_delta = 0, .group = 0, .global1 = 0xFFFFFFFFU, .global2 = 0xFFFFFFFFU, .global3 = 0xFFFFFFFFU, .sndCommandId = 0xFFFFFFFFU, .global5 = 0xFFFFFFFFU, .global6 = 0xFFFFFFFFU};
static const int32_t data_14268 = -1;
static const uint8_t data_1426C[4] = {0};
static const int32_t data_14270 = -1;
static char data_14274[] = "DOOR";
static const uint8_t data_14279[3] = {0};
static char *const data_1427C = data_14274;
static const int32_t data_14280[3] = {28, 1049259, 1114795};
static const MapObjectRecord data_1428C = {.kind = 2, .resourceIndex = 0, .scale = {1.0f, 1.0f, 1.0f}, .pos = {3.9f, 0.0f, 0.02f}, .yawFixed16 = 0};
static const uint32_t data_142B0 = 0xFFFF0000;
typedef struct Row_142B4 { int32_t w0; uint32_t w1; float w2; float w3; float w4; int32_t w5; int32_t w6; int32_t w7; } Row_142B4;
static const Row_142B4 data_142B4[11] = {
  {0, 0x0U, 2.246f, 0.786f, -8.705f, 0, 0, -65536},
  {1, 0x0U, 4.2f, 0.7862f, -8.705f, 0, 0, -65536},
  {2, 0x78E3DB05U, 4.45f, 1.06f, -2.2f, 0, 0, -65536},
  {3, 0x78E3C000U, 4.5f, 1.06f, -3.785f, 0, 0, -65536},
  {4, 0x78E3C000U, 4.3f, 1.2f, -4.485f, 0, 0, -65536},
  {5, 0x71C7C000U, 5.1f, 1.55f, -0.97f, 0, 0, -65536},
  {6, 0x71C7C000U, 5.1f, 1.35f, -0.97f, 0, 0, -65536},
  {7, 0x71C7C000U, 5.1f, 1.15f, -0.97f, 0, 0, -65536},
  {8, 0x71C7C000U, 5.1f, 1.55f, -0.43f, 0, 0, -65536},
  {9, 0x71C7C000U, 5.1f, 1.35f, -0.43f, 0, 0, -65536},
  {10, 0x71C7C000U, 5.1f, 1.15f, -0.43f, 0, 0, 65535}
};
static const uint32_t data_14414 = 152;
static const int32_t data_14418[9] = {153, 154, 155, 157, 158, 159, 160, 161, 156};
static const uint32_t data_1443C = 153;
static const int32_t data_14440[3] = {152, 152, 153};
static const uint32_t data_1444C = 65546;
static const int32_t data_14450[794] = {
  0, 655360, 3, 0, 589834, 0, 655360, 7, 0, 65545, 0, 262144, 720906, 0, 1441802, 0, 589824, 131106, 0, 851978, 0, 0, 20, 0, 1, 0, 65536, 0, 0,
  2424842, 0, 65536, 0, 0, 1, 0, 65536, 0, 0, 1, 0, 655360, 21, 0, 2359306, 0, 655360, 32, 0, 2228234, 0, 589824, 131106, 0, 851978, 0, 655360, 2, 0,
  19, 0, 0, 105, 0, 1, 0, 655360, 39, 0, 1441802, 0, 0, 1, 0, 2555913, 2, 655360, 12, 0, 1, 0, 851968, 1, 0, 5242881, 0, 65536, 0, 0, 131085, 0,
  786432, 3932170, 0, 3080201, 2, 65536, 15, 0, 655370, 0, 786432, 524305, 0, 196621, 0, 786432, 458756, 0, 458764, 0, 786432, 589829, 0, 393228, 7,
  786432, 458760, 0, 720908, 2, 786432, 1638412, 0, 851980, 0, 786432, 14, 0, 1966081, 0, 655360, 19, 0, 393216, 0, 65536, 0, 0, 1, 0, 65536, 0, 0, 1,
  0, 65536, 0, 0, 262157, 0, 655360, 20, 0, 4587521, 0, 262144, 786532, 1, 65547, 58, 655360, 31, 0, 1, 0, 720896, 7274512, 0, 1769482, 0, 851968, 7,
  0, 131083, 67, 65536, 0, 0, 2162698, 0, 655360, 28, 0, 1, 0, 851968, 8, 0, 4390923, 60, 65536, 0, 0, 589837, 0, 720896, 6553617, 0, 1769482, 0,
  65536, 0, 0, 196619, 32, 65536, 0, 0, 720909, 0, 720896, 7274514, 0, 1245195, 76, 720896, 5570580, 0, 1376267, 111, 65536, 0, 0, 16, 0, 65536, 0, 0,
  1, 0, 65536, 0, 0, 2031620, -65535, 262144, 65567, 65535, 2621444, 3014657, 262144, 65586, 47, 3276800, 0, 65536, 0, 0, 1, 0, 65536, 0, 0, 1, 0,
  65536, 0, 0, 1, 0, 65536, 0, 0, 1, 0, 65536, 0, 0, 1, 0, 65536, 0, 0, 1, 0, 65536, 0, 0, 1, 0, 65536, 0, 0, 1, 0, 65536, 0, 0, 1, 0, 65536, 0, 0, 1,
  0, 65536, 0, 0, 786445, 0, 720896, 4980740, 0, 1, 0, 589824, 65567, 0, 524298, 0, 851968, 13, 0, 1441803, 40, 65536, 0, 0, 1, 0, 589824, 131110, 0,
  917514, 0, 65536, 30, 0, 131081, 0, 589824, 11, 0, 1966081, 0, 786432, 2621456, 0, 1835017, 1, 655360, 19, 0, 983050, 0, 655360, 30, 0, 1769485, 0,
  655360, 23, 0, 3932160, 0, 65536, 0, 0, 1, 0, 851968, 27, 0, 9175041, 0, 65536, 0, 0, 3932160, 0, 65536, 0, 0, 458765, 0, 720896, 3932173, 0, 1, 0,
  851968, 15, 0, 2490379, 95, 655360, 30, 0, 1048589, 0, 720896, 6094921, 0, 1, 0, 655360, 26, 0, 1507341, 0, 65536, 60, 0, 1245194, 0, 327680, 65548,
  0, 393216, 0, 65536, 0, 0, 1, 0, 65536, 0, 0, 1, 0, 65536, 0, 0, 1, 0, 65536, 0, 0, 1, 0, 65536, 0, 0, 1, 0, 65536, 0, 0, 1, 0, 65536, 0, 0, 1, 0,
  65536, 0, 0, 1179658, 0, 589824, 131105, 0, 655369, 0, 65536, 120, 0, 524297, 0, 65536, 30, 0, 1245194, 0, 65536, 0, 0, 1, 0, 655360, 5, 0, 262154,
  0, 655360, 6, 0, 4587520, 0, 65536, 0, 0, 1, 0, 65536, 0, 0, 1, 0, 65536, 0, 0, 1, 0, 65536, 0, 0, 1, 0, 65536, 0, 0, 1, 0, 65536, 0, 0, 1, 0,
  65536, 0, 0, 1, 0, 65536, 0, 0, 1, 0, 65536, 0, 0, 4915204, 196616, 262144, 524368, 2, 5898244, 65544, 65536, 0, 0, 1, 0, 65536, 0, 0, 1, 0, 65536,
  0, 0, 1, 0, 65536, 0, 0, 1, 0, 65536, 0, 0, 1, 0, 65536, 0, 0, 1, 0, 851968, 17, 0, 65545, 0, 65536, 0, 0, 2883595, 150, 720896, 9830445, 0,
  1048586, 0, 655360, 26, 0, 3932161, 0, 655360, 19, 0, 327680, 0, 65536, 0, 0, 1, 0, 65536, 0, 0, 1, 0, 65536, 0, 0, 2031626, 0, 589824, 131113, 0,
  65545, 0, 851968, 17, 0, 1507339, 85, 589824, 131104, 0, 1179661, 0, 655360, 16, 0, 327691, 102, 851968, 19, 0, 393227, 15, 655360, 27, 0, 2818049,
  0, 851968, 20, 0, 1572875, 120, 65536, 0, 0, 1638411, 111, 65536, 0, 0, 1441805, 0, 655360, 30, 0, 458763, 45, 655360, 26, 0, 1507341, 0, 720896,
  26, 0, 2490377, 2, 65536, 80, 0, 1245194, 0, 0, 6, 0, 1, 0, 65536, 0, 0, 851977, 0, 589824, 18, 0, 1, 0, 851968, 24, 0, 524299, 29, 589824, 16, 0,
  65545, 0, 589824, 131125, 0, 1966081, 0, 655360, 16, 0, 2031626, 0, 851968, 25, 0, 1769483, 128, 589824, 14, 0, 1703946, 0, 851968, 26, 0, 2949121,
  0, 655360, 29, 0, 589835, 30, 655360, 35, 0, 1114122, 0, 65536, 15, 0, 1245194, 0, 65536, 30, 0, 1966090, 0, 0, 5, 0, 1, 0, 65536, 0, 0, 1, 0,
  65536, 0, 0, 720909, 0, 655360, 31, 0, 786437, 0, 720896, 1966119, 0, 2621451, 70, 1048576, 0, 0, 1, 0, 262144, 65638, 65535, 6815748, 3014657, 0,
  50, 0, 1, 0, 65536, 0, 0, 786445, 0, 720896, 2555918, 0, 2686976, 0, 1245184, 0, 0, 1, 0, 65536, 0, 0
};
static const char data_150B8[14] __attribute__((scn_string_literal)) = "return : %d \012";
static const char data_150C6[10] __attribute__((scn_string_literal)) = "E1039A001";
static const char data_150D0[10] __attribute__((scn_string_literal)) = "E1039A002";
static const char data_150DA[10] __attribute__((scn_string_literal)) = "E1039A003";
static const char data_150E4[10] __attribute__((scn_string_literal)) = "E1039A004";
static const char data_150EE[10] __attribute__((scn_string_literal)) = "E1039A005";
static const char data_150F8[10] __attribute__((scn_string_literal)) = "E1039A006";
static const char data_15102[10] __attribute__((scn_string_literal)) = "E1039A007";
static const char data_1510C[10] __attribute__((scn_string_literal)) = "E1039A008";
static const char data_15116[10] __attribute__((scn_string_literal)) = "E1039A009";
static const char data_15120[10] __attribute__((scn_string_literal)) = "E1039A010";
static const char data_1512A[10] __attribute__((scn_string_literal)) = "E1039A011";
static const char data_15134[10] __attribute__((scn_string_literal)) = "E1039A012";
static const char data_1513E[10] __attribute__((scn_string_literal)) = "E1039A013";
static const char data_15148[10] __attribute__((scn_string_literal)) = "E1039A014";
static const char data_15152[10] __attribute__((scn_string_literal)) = "E1039A015";
static const char data_1515C[10] __attribute__((scn_string_literal)) = "E1039B001";
static const char data_15166[10] __attribute__((scn_string_literal)) = "E1039B002";
static const char data_15170[10] __attribute__((scn_string_literal)) = "E1039B003";
static const char data_1517A[10] __attribute__((scn_string_literal)) = "E1039B004";
static const char data_15184[10] __attribute__((scn_string_literal)) = "E1039B005";
static const char data_1518E[10] __attribute__((scn_string_literal)) = "E1039B006";
static const char data_15198[10] __attribute__((scn_string_literal)) = "E1039B007";
static const char data_151A2[10] __attribute__((scn_string_literal)) = "E1039B008";
static const char data_151AC[10] __attribute__((scn_string_literal)) = "E1039B009";
static const char data_151B6[10] __attribute__((scn_string_literal)) = "E1039B010";
static const char data_151C0[10] __attribute__((scn_string_literal)) = "E1039B011";
static const char data_151CA[10] __attribute__((scn_string_literal)) = "E1039B012";
static const char data_151D4[10] __attribute__((scn_string_literal)) = "E1039B013";
static const char data_151DE[10] __attribute__((scn_string_literal)) = "E1039B014";
static const char data_151E8[10] __attribute__((scn_string_literal)) = "E1039B015";
static const char data_151F2[10] __attribute__((scn_string_literal)) = "E1039B016";
static const char data_151FC[10] __attribute__((scn_string_literal)) = "E1039B017";
static const char data_15206[10] __attribute__((scn_string_literal)) = "E1039B018";
static const char data_15210[10] __attribute__((scn_string_literal)) = "E1039B019";
static const char data_1521A[10] __attribute__((scn_string_literal)) = "E1039B020";
static const char data_15224[10] __attribute__((scn_string_literal)) = "E1039B021";
static const char data_1522E[10] __attribute__((scn_string_literal)) = "E1039B022";
static const char data_15238[10] __attribute__((scn_string_literal)) = "E1039B023";
static const char data_15242[10] __attribute__((scn_string_literal)) = "E1039B024";
static const char data_1524C[10] __attribute__((scn_string_literal)) = "E1039B025";
static const char data_15256[10] __attribute__((scn_string_literal)) = "E1039B026";
static const char data_15260[10] __attribute__((scn_string_literal)) = "E1039B027";
static const char data_1526A[10] __attribute__((scn_string_literal)) = "E1039B028";
static const char data_15274[10] __attribute__((scn_string_literal)) = "E1039B029";
static const char data_1527E[10] __attribute__((scn_string_literal)) = "E1039B030";
static const char data_15288[10] __attribute__((scn_string_literal)) = "E1039B031";
static const char data_15292[10] __attribute__((scn_string_literal)) = "E1039B032";
static const char data_1529C[10] __attribute__((scn_string_literal)) = "E1039B033";
static const char data_152A6[10] __attribute__((scn_string_literal)) = "E1039B034";
static const char data_152B0[10] __attribute__((scn_string_literal)) = "E1039B035";
static const char data_152BA[10] __attribute__((scn_string_literal)) = "E1039B036";
static const char data_152C4[10] __attribute__((scn_string_literal)) = "E1039B037";
static const char data_152CE[10] __attribute__((scn_string_literal)) = "E1039B038";
static const char data_152D8[10] __attribute__((scn_string_literal)) = "E1039B039";
static const char data_152E2[10] __attribute__((scn_string_literal)) = "E1039B040";
static const char data_152EC[10] __attribute__((scn_string_literal)) = "E1039B041";
static const char data_152F6[10] __attribute__((scn_string_literal)) = "E1039B042";
static const char data_15300[10] __attribute__((scn_string_literal)) = "E1039B043";
static const char data_1530A[10] __attribute__((scn_string_literal)) = "E1039B044";
static const char data_15314[10] __attribute__((scn_string_literal)) = "E1039B045";
static const char data_1531E[10] __attribute__((scn_string_literal)) = "E1039B046";
static const char data_15328[10] __attribute__((scn_string_literal)) = "E1039B047";
static const char data_15332[10] __attribute__((scn_string_literal)) = "E1039B048";
static const char data_1533C[10] __attribute__((scn_string_literal)) = "E1039B049";
static const char data_15346[10] __attribute__((scn_string_literal)) = "E1039B050";
static const char data_15350[10] __attribute__((scn_string_literal)) = "E1039B051";
static const char data_1535A[10] __attribute__((scn_string_literal)) = "E1039C001";
static const char data_15364[10] __attribute__((scn_string_literal)) = "E1039C002";
static const char data_1536E[10] __attribute__((scn_string_literal)) = "E1039C003";
static const char data_15378[10] __attribute__((scn_string_literal)) = "E1039C004";
static const char data_15382[10] __attribute__((scn_string_literal)) = "E1039C005";
static const char data_1538C[10] __attribute__((scn_string_literal)) = "E1039C006";
static const char data_15396[10] __attribute__((scn_string_literal)) = "E1039C007";
static const char data_153A0[10] __attribute__((scn_string_literal)) = "E1039C008";
static const char data_153AA[6] __attribute__((scn_string_literal)) = "E1039";
static const char data_153B0[6] __attribute__((scn_string_literal)) = "E1039";
static const char data_153B6[21] __attribute__((scn_string_literal)) = "ＡＵＴＯ＿０１: %d \012";
static const char data_153CB[21] __attribute__((scn_string_literal)) = "ＡＵＴＯ＿ＮＯ: %d \012";
static const char data_153E0[21] __attribute__((scn_string_literal)) = "ROBART_TALK_ST: %d \012";
static const char data_153F5[17] __attribute__((scn_string_literal)) = "day_chk: %d, %d\012";
static const char data_15406[6] __attribute__((scn_string_literal)) = "E1039";
static const char data_1540C[21] __attribute__((scn_string_literal)) = "ＡＵＴＯ＿０１: %d \012";
static const char data_15421[25] __attribute__((scn_string_literal)) = "VOICE_NO %03d  frame %d\012";
static const char data_1543A[10] __attribute__((scn_string_literal)) = " DEG: %5d";
static const char data_15444[12] __attribute__((scn_string_literal)) = " P  : %d";
static const uint32_t data_15450 = 1;
typedef struct Row_15454 { float w0; float w1; float w2; int32_t w3; } Row_15454;
static const Row_15454 data_15454[2] = {
  {3.8f, -5.5f, 1.0f, 2},
  {3.8f, -6.3f, 1.0f, 196608}
};
static const char data_15474[12] __attribute__((scn_string_literal)) = "消えるよ1 \012";
static const uint32_t data_15480 = 1;
typedef struct Row_15484 { float w0; float w1; float w2; int32_t w3; } Row_15484;
static const Row_15484 data_15484[3] = {
  {3.88f, -6.8f, 1.0f, 2},
  {2.65f, -7.15f, 1.0f, 3},
  {2.6f, -7.45f, 1.0f, 196608}
};
static const char data_154B4[12] __attribute__((scn_string_literal)) = "消えるよ2 \012";
static const uint32_t data_154C0 = 1;
typedef struct Row_154C4 { float w0; float w1; float w2; int32_t w3; } Row_154C4;
static const Row_154C4 data_154C4[2] = {
  {3.2f, -7.25f, 1.0f, 2},
  {3.4f, -4.0f, 1.0f, 196608}
};
static const char data_154E4[376] = "\217\301\202\246\202\351\202\3463 \012\000\000\000\000\000\000\000\000\000\000\000\226\000\276\012\000\000\000\000\000\000\000\000\226\000\276\012\000\000\000\000\000\000\000\000\214\000\214\012\000\000\000\000\000\000\000\000\202\000\214\012\000\000\000\000\000\000\000\000x\000\214\012\000\000\000\000\000\000\000\000n\000\214\012\001\000\001\000\001\000\000\000d\000\214\012\001\000\002\000\001\000\000\0002\000\214\012\002\000\003\000\002\000\000\000\000\000\214\012\003\000\004\000\002\000\000\000\336\015\214\012\004\000\006\000\003\000\000\000\254\015\214\012\005\000\006\000\002\000\000\000\300\015\226\012\006\000\006\000\001\000\000\000\324\015\240\012\007\000\006\000\000\000\000\000\350\015\252\012\010\000\007\000\376\377\000\000\374\015\264\012\010\000\007\000\374\377\000\000\000\000\276\012\007\000\010\000\372\377\000\000\000\000\310\012\006\000\011\000\370\377\000\000\000\000\322\012\005\000\012\000\366\377\000\000\000\000\334\012\004\000\013\000\365\377\000\000\000\000\346\012\003\000\015\000\364\377\000\000\000\000\360\012\002\000\016\000\364\377\000\000\000\000\372\012\001\000\017\000\365\377\000\000\012\000\004\013\001\000\020\000\365\377\000\000\024\000\016\013\000\000\021\000\366\377\000\000\024\000\030\013\000\000\022\000\366\377\000\000\036\000\"\013\374\377\024\000\371\377\000\0002\000T\013\374\377\026\000\374\377\000\0002\000\206\013\374\377\030\000\377\377\000\0002\000\270\013\374\377\033\000\002\000\000\0002\000\352\013";
static const Vector3 data_1565C = {.x = 0.0f, .y = 0.0f, .z = 0.0f};
static const char data_15668[] __attribute__((scn_string_literal)) = "BDAG";
static const uint8_t data_1566D[3] = {0};
static const Vector3 data_15670 = {.x = 1.0f, .y = 1.0f, .z = 1.0f};
static const char data_1567C[] __attribute__((scn_string_literal)) = "BLKS";
static const char data_15681[15] __attribute__((scn_string_literal)) = "PRESS A BUTTON";
static const uint32_t data_15690 = 0xC116E561;
static const float data_15694[3] = {-9.431001f, -7.991f, -7.991f};
static const uint32_t data_156A0 = 0x3FD6872B;
static const uint32_t data_156A4 = 0x4093B645U;
static const uint32_t data_156A8 = 0x3FDB39C1;
static const float data_156AC[3] = {3.2006f, 1.7127f, 3.2006f};
static const uint32_t data_156B8 = 0x4045D97F;
static const float data_156BC[3] = {4.5793f, 3.0914f, 4.5793f};
static const uint32_t data_156C8 = 0xC1162D0F;
static const float data_156CC = -8.035999f;
static const uint32_t data_156D0 = 0x3F95E180;
static const uint32_t data_156D4 = 0xC0A6A94BU;
static const float data_156D8 = 5.208295f;
static const uint32_t data_156DC[5] = {0xBF95E12DU, 0xBF53F78BU, 0x3F53F6EBU, 0x3F95E1E9U, 0xC0A6AB2BU};
static const float data_156F0 = 5.208265f;
static const uint32_t data_156F4[3] = {0xBF95E23BU, 0xBF53F70CU, 0x3F53F7ACU};
static const uint32_t data_15700 = 0xC136FBCF;
static const uint32_t data_15704 = 0x40D56CBAU;
static const float data_15708[2] = {-26.10067f, -4.068941f};
static const uint32_t data_15710[5] = {0xC0B20C0EU, 0xC13DBA4EU, 0xC155A68EU, 0x410ADD22U, 0xC1C0BC11U};
static const float data_15724 = -5.985486f;
static const uint32_t data_15728[2] = {0xC0D4BECCU, 0xC12C612EU};
static const uint32_t data_15730 = 0x3FCE1B09;
static const float data_15734[5] = {3.146f, 4.6818f, 4.6818f, 3.146f, 1.6102f};
static const uint32_t data_15748 = 0xC117E219;
static const float data_1574C[5] = {-9.5271f, -9.4927f, -7.9293f, -7.8949f, -7.9293f};
static const Vector3 data_15760 = {.x = 0.0f, .y = 0.0f, .z = 0.05f};
static const Vector3i data_1576C = {.x = 0, .y = 50062, .z = 16384};
static const char data_15778[10] __attribute__((scn_string_literal)) = "E1039B036";
static const char data_15782[10] __attribute__((scn_string_literal)) = "E1039A015";
static const char data_1578C[12] __attribute__((scn_string_literal)) = "E1039B038";
static const Vector3 data_15798 = {.x = 0.0f, .y = 0.02f, .z = 0.05f};
static const int32_t data_157A4[3] = {0, 49152, 910};
static const char data_157B0[8] __attribute__((scn_string_literal)) = "ang %f\012";
static const char data_157B8[8] __attribute__((scn_string_literal)) = "ang %f\012";
static const char data_157C0[16] __attribute__((scn_string_literal)) = "f1jazzba.snd";
static const Vector3 data_157D0 = {.x = 2.5f, .y = 0.0f, .z = -5.0f};
static const Vector3 data_157DC = {.x = 3.8f, .y = 0.0f, .z = -5.5f};
static const char data_157E8[] __attribute__((scn_string_literal)) = "33S@";
static const uint8_t pad_157ED[6] = {0};
static const uint8_t data_157F3[1] = {
  0xC1
};
static const char data_157F4[12] __attribute__((scn_string_literal)) = " gold: %d \012";
static const char data_15800[14] __attribute__((scn_string_literal)) = "scene/03/D000";
static const char data_1580E[] __attribute__((scn_string_literal)) = "SCRTEST.BIN";
static const char data_1581A[17] __attribute__((scn_string_literal)) = " h1 load error \012";
static const char data_1582B[17] __attribute__((scn_string_literal)) = " h2 load error \012";
static const char data_1583C[17] __attribute__((scn_string_literal)) = " h3 load error \012";
static const char data_1584D[12] __attribute__((scn_string_literal)) = " EVENT_ON \012";
static const char data_15859[39] = " EVENT_OFF \012\000\000\000\377\377\377\377\377\377\000\000,\004\023\004-\004/\0042\0040\0041\004\207\003";
static const char data_15880[20] __attribute__((scn_string_literal)) = "TUNED FreeTalk def\012";
static const char data_15894[20] __attribute__((scn_string_literal)) = "TUNED FreeTalk def\012";
static const uint32_t data_158A8 = 380;
static const int32_t data_158AC[6] = {1, 390, 0, 1018, 0, -1};
static const uint32_t data_158C4 = 510;
static const int32_t data_158C8[6] = {1, 525, 0, 1020, 0, -1};
static const uint32_t data_158E0 = 150;
static const int32_t data_158E4[6] = {1, 160, 0, 1019, 0, -1};
static const uint32_t data_158FC = 530;
static const int32_t data_15900[6] = {1, 970, 0, 1014, 0, -1};
static const uint32_t data_15918 = 911;
static const int32_t data_1591C[6] = {1, 155, 0, 1015, 0, -1};
static const char data_15934[7] __attribute__((scn_string_literal)) = "FREE02";
static const char data_1593B[7] __attribute__((scn_string_literal)) = "FREE03";
static const char data_15942[7] __attribute__((scn_string_literal)) = "FREE04";
static const char data_15949[7] __attribute__((scn_string_literal)) = "FREE01";
static const char data_15950[12] __attribute__((scn_string_literal)) = "M_MOBJ.BIN";

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
    *(int32_t *)&local_F8 = Script_ScenDataUtil(data_12B28, 5, Script_ScenDataUtil(data_12538, 1, arg3 * 40 + task_104[arg2] * 4 + local_FC) * 9 + 5);
    *(int32_t *)&local_F4 = Script_ScenDataUtil(data_12B28, 5, Script_ScenDataUtil(data_12538, 1, arg3 * 40 + task_104[arg2] * 4 + local_FC) * 9 + 6);
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
    *(int32_t *)&local_F4 = Script_ScenDataUtil(data_12B28, 5, Script_ScenDataUtil(data_12538, 1, arg3 * 40 + task_104[arg2] * 4 + local_FC) * 9 + 5);
    local_F4 = 90.0f - local_F4;
    *(int32_t *)&local_F0 = Script_ScenDataUtil(data_12B28, 5, Script_ScenDataUtil(data_12538, 1, arg3 * 40 + task_104[arg2] * 4 + local_FC) * 9 + 6);
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
    local_FC = script_8C20(arg0, arg1, local_F8, local_F4 / (float)100, local_F0 / (float)100, arg3, task_B4[arg2]);
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
    *(int32_t *)&local_F8 = Script_ScenDataUtil(data_12B28, 5, Script_ScenDataUtil(data_12538, 1, arg3 * 40 + task_104[arg2] * 4 + local_FC) * 9 + 5);
    *(int32_t *)&local_F4 = Script_ScenDataUtil(data_12B28, 5, Script_ScenDataUtil(data_12538, 1, arg3 * 40 + task_104[arg2] * 4 + local_FC) * 9 + 6);
    local_FC = script_8C20(arg0, arg1, 5, local_F8 / 100.0f, local_F4 / 100.0f, arg3, -1);
    if (local_FC == 1) {
      local_FC = 2;
    } else {
      local_FC = 3;
    }
    if ((Script_ScenDataUtil(task_6C, 4, task_B4[arg2] * 13 + 1) & 4) == 0) {
      script_2411(arg0, arg1, arg2, arg3, local_FC, 0);
    } else {
      if (arg0 == task_1CC) {
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
    *(int32_t *)&local_F8 = Script_ScenDataUtil(data_12B28, 5, Script_ScenDataUtil(data_12538, 1, arg3 * 40 + task_104[arg2] * 4 + local_FC) * 9 + 5);
    *(int32_t *)&local_F4 = Script_ScenDataUtil(data_12B28, 5, Script_ScenDataUtil(data_12538, 1, arg3 * 40 + task_104[arg2] * 4 + local_FC) * 9 + 6);
    local_FC = script_8C20(arg0, arg1, 5, local_F8 / 100.0f, local_F4 / 100.0f, arg3, -1);
    if (local_FC == 1) {
      local_FC = 1;
    } else {
      local_FC = 0;
    }
    if ((Script_ScenDataUtil(task_6C, 4, task_B4[arg2] * 13 + 1) & 4) == 0) {
      script_2411(arg0, arg1, arg2, arg3, local_FC, 0);
    } else {
      if (arg0 == task_1CC) {
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
    if ((Script_ScenDataUtil(task_6C, 4, task_B4[arg2] * 13 + 1) & 4) == 0) {
      float local_F4;
      float local_F0;
      local_FC = script_8C20(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
      *(int32_t *)&local_F4 = Script_ScenDataUtil(data_12B28, 5, Script_ScenDataUtil(data_12538, 1, arg3 * 40 + task_104[arg2] * 4 + local_FC) * 9 + 5);
      *(int32_t *)&local_F0 = Script_ScenDataUtil(data_12B28, 5, Script_ScenDataUtil(data_12538, 1, arg3 * 40 + task_104[arg2] * 4 + local_FC) * 9 + 6);
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
      local_FC = script_8C20(arg0, arg1, local_F8, local_F4 / 100.0f, local_F0 / 100.0f, arg3, task_B4[arg2]);
      script_2411(arg0, arg1, arg2, arg3, local_FC, 1);
    } else {
      if (arg0 == task_1CC) {
        float local_F4;
        float local_F0;
        local_FC = script_8C20(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
        *(int32_t *)&local_F4 = Script_ScenDataUtil(data_12B28, 5, Script_ScenDataUtil(data_12538, 1, arg3 * 40 + task_104[arg2] * 4 + local_FC) * 9 + 5);
        *(int32_t *)&local_F0 = Script_ScenDataUtil(data_12B28, 5, Script_ScenDataUtil(data_12538, 1, arg3 * 40 + task_104[arg2] * 4 + local_FC) * 9 + 6);
        local_FC = script_8C20(arg0, arg1, 6, local_F4 / 100.0f, local_F0 / 100.0f, arg3, task_B4[arg2]);
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
    *(int32_t *)&local_F8 = Script_ScenDataUtil(data_12B28, 5, Script_ScenDataUtil(data_12538, 1, arg3 * 40 + task_104[arg2] * 4 + local_FC) * 9 + 5);
    *(int32_t *)&local_F4 = Script_ScenDataUtil(data_12B28, 5, Script_ScenDataUtil(data_12538, 1, arg3 * 40 + task_104[arg2] * 4 + local_FC) * 9 + 6);
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
    *(int32_t *)&local_F8 = Script_ScenDataUtil(data_12B28, 5, Script_ScenDataUtil(data_12538, 1, arg3 * 40 + task_104[arg2] * 4 + local_FC) * 9 + 5);
    *(int32_t *)&local_F4 = Script_ScenDataUtil(data_12B28, 5, Script_ScenDataUtil(data_12538, 1, arg3 * 40 + task_104[arg2] * 4 + local_FC) * 9 + 6);
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
  if (arg0 == task_1CC) {
    if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
      local_FF7F = script_8C20(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
      *(int32_t *)&local_FF78 = Script_ScenDataUtil(data_12B28, 5, Script_ScenDataUtil(data_12538, 1, arg3 * 40 + task_104[arg2] * 4 + local_FF7F) * 9 + 5);
      *(int32_t *)&local_FF74 = Script_ScenDataUtil(data_12B28, 5, Script_ScenDataUtil(data_12538, 1, arg3 * 40 + task_104[arg2] * 4 + local_FF7F) * 9 + 6);
      local_FF7F = script_8C20(arg0, arg1, 5, local_FF78 / (float)100, local_FF74 / (float)100, arg3, -1);
      script_92E9(arg0, 0);
      *(int32_t *)&local_94 = Script_ScenDataUtil(data_12B28, 5, Script_ScenDataUtil(data_12538, 1, arg3 * 40 + task_104[arg2] * 4 + local_FF7F) * 9 + 1);
      *(int32_t *)&local_90 = Script_ScenDataUtil(data_12B28, 5, Script_ScenDataUtil(data_12538, 1, arg3 * 40 + task_104[arg2] * 4 + local_FF7F) * 9 + 2);
      local_8C = 0.0f;
      local_FC = Script_ScenDataUtil(data_12B28, 4, Script_ScenDataUtil(data_12538, 1, arg3 * 40 + task_104[arg2] * 4 + local_FF7F) * 9);
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
    *(int32_t *)&local_F8 = Script_ScenDataUtil(data_12B28, 5, Script_ScenDataUtil(data_12538, 1, arg3 * 40 + task_104[arg2] * 4 + local_FC) * 9 + 5);
    local_F8 = 98.0f - local_F8;
    local_F4 = Script_ScenDataUtil(data_12B28, 5, Script_ScenDataUtil(data_12538, 1, arg3 * 40 + task_104[arg2] * 4 + local_FC) * 9 + 6);
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
        *(&task_1E0 + 1) = Script_FloatUtils(ScriptMathOp_Atan2ReverseVector, (&local_E4)->x - (&task_1D4)->x, (&local_E4)->z - task_1D4.z);
        script_668F(arg0, arg1, arg2, arg3, local_FC, local_F0);
      }
    } else if ((local_BC == 3) | (local_BC == 4) | (local_BC == 5) | (local_BC == 6)) {
      *(int32_t *)&local_D0 = Script_ScenDataUtil(&data_13548, 5, Script_ScenDataUtil(task_6C, 4, task_B4[arg2] * 13 + 6) * 5 + 3);
      *(int32_t *)&local_CC = Script_ScenDataUtil(&data_13548, 5, Script_ScenDataUtil(task_6C, 4, task_B4[arg2] * 13 + 6) * 5 + 4);
      local_FC = script_8C20(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
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
      local_FC = script_8C20(arg0, arg1, local_F8, local_D0 / (float)100, local_CC / (float)100, arg3, task_B4[arg2]);
      script_6767(arg0, arg1, arg2, arg3, local_FC, 0);
    } else {
      if (local_BC == 7) {
        local_FC = script_8C20(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
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
          *(int32_t *)&local_D0 = Script_ScenDataUtil(&data_13548, 5, Script_ScenDataUtil(task_6C, 4, task_B4[arg2] * 13 + 6) * 5 + 3);
          *(int32_t *)&local_CC = Script_ScenDataUtil(&data_13548, 5, Script_ScenDataUtil(task_6C, 4, task_B4[arg2] * 13 + 6) * 5 + 4);
          local_FC = script_8C20(arg0, arg1, local_F8, local_D0 / (float)100, local_CC / (float)100, arg3, task_B4[arg2]);
          Script_scenUtils(SCENARIO_CMD_INIT_SCNT);
          script_6BDE(arg0, arg1, arg2, arg3, local_FC, 0);
          Script_scenUtils(SCENARIO_CMD_UPDATE_TIMER);
        } else {
          *(int32_t *)&local_D0 = Script_ScenDataUtil(&data_13548, 5, Script_ScenDataUtil(task_6C, 4, task_B4[arg2] * 13 + 6) * 5 + 3);
          *(int32_t *)&local_CC = Script_ScenDataUtil(&data_13548, 5, Script_ScenDataUtil(task_6C, 4, task_B4[arg2] * 13 + 6) * 5 + 4);
          local_FC = script_8C20(arg0, arg1, local_F8, local_D0 / (float)100, local_CC / (float)100, arg3, task_B4[arg2]);
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

__attribute__((scn_string_bindings(0, data_14066)))
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
  *(int32_t *)&local_98 = Script_ScenDataUtil(data_12B28, 5, Script_ScenDataUtil(data_12538, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9 + 1);
  *(int32_t *)&local_94 = Script_ScenDataUtil(data_12B28, 5, Script_ScenDataUtil(data_12538, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9 + 2);
  *(int32_t *)&local_90 = Script_ScenDataUtil(data_12B28, 5, Script_ScenDataUtil(data_12538, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9 + 3);
  *(int32_t *)&local_8C = Script_ScenDataUtil(data_12B28, 5, Script_ScenDataUtil(data_12538, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9 + 4);
  *(int32_t *)&local_88 = Script_ScenDataUtil(data_12B28, 5, Script_ScenDataUtil(data_12538, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9 + 7);
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
      local_FF78 = Script_ScenDataUtil(data_12B28, 4, Script_ScenDataUtil(data_12538, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9);
      local_FF7C = 18;
      local_80 = 18;
      local_A4 = 1.0f;
    } else {
      if (arg0 == task_1CC) {
        local_FF78 = Script_ScenDataUtil(data_12B28, 4, Script_ScenDataUtil(data_12538, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9) | -0x80000000;
        local_FF7C = 12;
        local_80 = 12;
      } else {
        local_FF78 = Script_ScenDataUtil(data_12B28, 4, Script_ScenDataUtil(data_12538, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9);
        local_FF7C = 18;
        local_80 = 18;
      }
      local_A4 = -1.0f;
    }
    script_00E5(arg0, local_FF78, arg2, arg3, arg4);
    local_FF78 = local_FF78 | ((Script_FloatUtils(ScriptMathOp_FloatToInt, local_98 - 3.0f) << 16) & 0x7FFF0000);
    script_92E9(arg0, 0);
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
          script_233B(arg0, task_104[arg2]);
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
      if (task_1CC == arg0) {
        if (!(local_B8 <= local_94) & !(local_B8 >= local_90)) {
          local_86 = 0;
        }
      } else {
        if (!(local_B8 <= local_94) & !(local_B8 > local_88)) {
          *(int32_t *)&local_9C = Script_ScenDataUtil(data_12B28, 5, Script_ScenDataUtil(data_12538, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9 + 8);
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
  script_92E9(arg0, 1);
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
        goto L003323;
      }
    }
  }
  L003323:;
  return;
}

static void script_3324(int32_t arg0, int32_t arg1, int32_t arg2) {
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

__attribute__((scn_string_bindings(0, data_14082)))
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
          local_FC = Script_ScenDataUtil(data_12B28, 4, Script_ScenDataUtil(data_12538, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9);
          break;
        case 0:
          local_B7 = 0;
          local_F8 = 12;
          local_F0 = 0x4000;
          local_B8 = 1.0f;
          local_FC = Script_ScenDataUtil(data_12B28, 4, Script_ScenDataUtil(data_12538, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9) | -0x80000000;
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
          local_FC = Script_ScenDataUtil(data_12B28, 4, Script_ScenDataUtil(data_12538, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9) | -0x80000000;
          break;
        case 0:
          local_B7 = 1;
          local_F8 = 18;
          local_F0 = 0xC000;
          local_B8 = 1.0f;
          local_FC = Script_ScenDataUtil(data_12B28, 4, Script_ScenDataUtil(data_12538, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9);
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
          local_FC = Script_ScenDataUtil(data_12B28, 4, Script_ScenDataUtil(data_12538, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9) | -0x80000000;
          break;
        case 0:
          local_B7 = 0;
          local_F8 = 18;
          local_F0 = 0xC000;
          local_B8 = -1.0f;
          local_FC = Script_ScenDataUtil(data_12B28, 4, Script_ScenDataUtil(data_12538, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9);
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
          local_FC = Script_ScenDataUtil(data_12B28, 4, Script_ScenDataUtil(data_12538, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9);
          break;
        case 0:
          local_B7 = 1;
          local_F8 = 12;
          local_F0 = 0x4000;
          local_B8 = -1.0f;
          local_FC = Script_ScenDataUtil(data_12B28, 4, Script_ScenDataUtil(data_12538, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9) | -0x80000000;
          break;
      }
      break;
  }
  *(int32_t *)&local_C4 = Script_ScenDataUtil(data_12B28, 5, Script_ScenDataUtil(data_12538, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9 + 1);
  *(int32_t *)&local_C0 = Script_ScenDataUtil(data_12B28, 5, Script_ScenDataUtil(data_12538, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9 + 2);
  local_FC = local_FC | ((Script_FloatUtils(ScriptMathOp_FloatToInt, local_C4 - 3.0f) << 16) & 0x7FFF0000);
  local_BC = 0.0f;
  script_92E9(arg0, 0);
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
  script_92E9(arg0, 1);
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

__attribute__((scn_string_bindings(0, data_1409E)))
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
      if (arg0 == task_1CC) {
        local_FF5C = arg4;
        switch (local_FF5C) {
          case 1:
            local_CF = 1;
            local_A4 = Script_ScenDataUtil(data_12B28, 4, Script_ScenDataUtil(data_12538, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9);
            local_C0 = 1.0f;
            local_A0 = 18;
            local_9C = 18;
            local_90 = 15473;
            local_8C = 17294;
            break;
          case 0:
            local_CF = 0;
            local_A4 = Script_ScenDataUtil(data_12B28, 4, Script_ScenDataUtil(data_12538, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9) | -0x80000000;
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
            local_A4 = Script_ScenDataUtil(data_12B28, 4, Script_ScenDataUtil(data_12538, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9);
            local_C0 = 1.0f;
            local_A0 = 18;
            local_9C = 18;
            local_90 = 13653;
            break;
          case 0:
            local_CF = 0;
            local_A4 = Script_ScenDataUtil(data_12B28, 4, Script_ScenDataUtil(data_12538, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9);
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
            local_A4 = Script_ScenDataUtil(data_12B28, 4, Script_ScenDataUtil(data_12538, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9) | -0x80000000;
            local_C0 = 1.0f;
            local_A0 = 12;
            local_9C = 18;
            local_90 = 50062;
            local_8C = 48241;
            break;
          case 0:
            local_CF = 1;
            local_A4 = Script_ScenDataUtil(data_12B28, 4, Script_ScenDataUtil(data_12538, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9);
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
            local_A4 = Script_ScenDataUtil(data_12B28, 4, Script_ScenDataUtil(data_12538, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9);
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
            local_A4 = Script_ScenDataUtil(data_12B28, 4, Script_ScenDataUtil(data_12538, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9);
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
            local_A4 = Script_ScenDataUtil(data_12B28, 4, Script_ScenDataUtil(data_12538, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9) | -0x80000000;
            local_C0 = -1.0f;
            local_A0 = 12;
            local_9C = 12;
            local_90 = 50062;
            local_8C = 48241;
            break;
          case 0:
            local_CF = 0;
            local_A4 = Script_ScenDataUtil(data_12B28, 4, Script_ScenDataUtil(data_12538, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9);
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
            local_A4 = Script_ScenDataUtil(data_12B28, 4, Script_ScenDataUtil(data_12538, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9);
            local_C0 = -1.0f;
            local_A0 = 18;
            local_9C = 18;
            local_90 = 51882;
            break;
          case 0:
            local_CF = 0;
            local_A4 = Script_ScenDataUtil(data_12B28, 4, Script_ScenDataUtil(data_12538, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9);
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
            local_A4 = Script_ScenDataUtil(data_12B28, 4, Script_ScenDataUtil(data_12538, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9);
            local_C0 = -1.0f;
            local_A0 = 18;
            local_9C = 12;
            local_90 = 15473;
            local_8C = 17294;
            break;
          case 0:
            local_CF = 1;
            local_A4 = Script_ScenDataUtil(data_12B28, 4, Script_ScenDataUtil(data_12538, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9) | -0x80000000;
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
            local_A4 = Script_ScenDataUtil(data_12B28, 4, Script_ScenDataUtil(data_12538, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9);
            local_C0 = -1.0f;
            local_A0 = 18;
            local_9C = 18;
            local_90 = 13653;
            break;
          case 0:
            local_CF = 1;
            local_A4 = Script_ScenDataUtil(data_12B28, 4, Script_ScenDataUtil(data_12538, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9);
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
              local_A4 = Script_ScenDataUtil(data_12B28, 4, Script_ScenDataUtil(data_12538, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9);
              local_C0 = -1.0f;
              local_A0 = 18;
              local_9C = 12;
              local_90 = 48241;
              local_8C = 50062;
              break;
            case 1:
              local_CF = 1;
              local_A4 = Script_ScenDataUtil(data_12B28, 4, Script_ScenDataUtil(data_12538, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9) | -0x80000000;
              local_C0 = -1.0f;
              local_A0 = 12;
              local_9C = 12;
              local_90 = 50062;
              local_8C = 48241;
              break;
            case 2:
              local_CF = 1;
              local_A4 = Script_ScenDataUtil(data_12B28, 4, Script_ScenDataUtil(data_12538, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9);
              local_C0 = 1.0f;
              local_A0 = 18;
              local_9C = 18;
              local_90 = 15473;
              local_8C = 17294;
              break;
            case 3:
              local_CF = 0;
              local_A4 = Script_ScenDataUtil(data_12B28, 4, Script_ScenDataUtil(data_12538, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9) | -0x80000000;
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
              local_A4 = Script_ScenDataUtil(data_12B28, 4, Script_ScenDataUtil(data_12538, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9);
              local_C0 = -1.0f;
              local_A0 = 18;
              local_9C = 18;
              local_90 = 45511;
              break;
            case 1:
              local_CF = 1;
              local_A4 = Script_ScenDataUtil(data_12B28, 4, Script_ScenDataUtil(data_12538, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9);
              local_C0 = -1.0f;
              local_A0 = 18;
              local_9C = 18;
              local_90 = 51882;
              break;
            case 2:
              local_CF = 1;
              local_A4 = Script_ScenDataUtil(data_12B28, 4, Script_ScenDataUtil(data_12538, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9);
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
              local_A4 = Script_ScenDataUtil(data_12B28, 4, Script_ScenDataUtil(data_12538, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9);
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
        goto L004AF7;
      }
    }
  }
  L004AF7:;
  (void)local_90;
  *(int32_t *)&local_B8 = Script_ScenDataUtil(data_12B28, 5, Script_ScenDataUtil(data_12538, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9 + 1);
  *(int32_t *)&local_B4 = Script_ScenDataUtil(data_12B28, 5, Script_ScenDataUtil(data_12538, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9 + 2);
  *(int32_t *)&local_B0 = Script_ScenDataUtil(data_12B28, 5, Script_ScenDataUtil(data_12538, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9 + 3);
  *(int32_t *)&local_AC = Script_ScenDataUtil(data_12B28, 5, Script_ScenDataUtil(data_12538, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9 + 4);
  *(int32_t *)&local_A8 = Script_ScenDataUtil(data_12B28, 5, Script_ScenDataUtil(data_12538, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9 + 7);
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
              script_233B(arg0, task_104[arg2]);
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
          local_FF74 = (float)script_0030(script_006D(local_FF70, (&local_FF48)->y) * 360 / 0x10000);
        }
        if (!(local_C4 <= local_B4) & !(local_C4 > local_A8)) {
          *(int32_t *)&local_BC = Script_ScenDataUtil(data_12B28, 5, Script_ScenDataUtil(data_12538, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9 + 8);
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
              goto L0055A8;
            }
          }
        }
      }
      L0055A8:;
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
      goto L0057E3;
    }
  }
  L0057E3:;
  return;
}

__attribute__((scn_string_bindings(0, data_140BA)))
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

__attribute__((scn_string_bindings(0, data_140D6)))
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
      local_8C = Script_ScenDataUtil(data_12B28, 4, Script_ScenDataUtil(data_12538, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9);
      local_A0 = -1.0f;
      local_88 = 18;
      local_84 = 18;
      local_FF78 = 46421;
      local_FF74 = 48787;
      break;
    case 1:
      local_AF = 1;
      local_8C = Script_ScenDataUtil(data_12B28, 4, Script_ScenDataUtil(data_12538, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9) | -0x80000000;
      local_A0 = -1.0f;
      local_88 = 12;
      local_84 = 12;
      local_FF78 = 50426;
      local_FF74 = 48969;
      break;
    case 2:
      local_AF = 1;
      local_8C = Script_ScenDataUtil(data_12B28, 4, Script_ScenDataUtil(data_12538, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9);
      local_A0 = 1.0f;
      local_88 = 18;
      local_84 = 18;
      local_FF78 = 15109;
      local_FF74 = 16566;
      break;
    case 3:
      local_AF = 0;
      local_8C = Script_ScenDataUtil(data_12B28, 4, Script_ScenDataUtil(data_12538, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9) | -0x80000000;
      local_A0 = 1.0f;
      local_88 = 12;
      local_84 = 12;
      local_FF78 = 19114;
      local_FF74 = 16748;
      break;
  }
  (void)local_FF78;
  *(int32_t *)&local_9C = Script_ScenDataUtil(data_12B28, 5, Script_ScenDataUtil(data_12538, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9 + 1);
  *(int32_t *)&local_98 = Script_ScenDataUtil(data_12B28, 5, Script_ScenDataUtil(data_12538, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9 + 2);
  *(int32_t *)&local_94 = Script_ScenDataUtil(data_12B28, 5, Script_ScenDataUtil(data_12538, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9 + 3);
  *(int32_t *)&local_90 = Script_ScenDataUtil(data_12B28, 5, Script_ScenDataUtil(data_12538, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9 + 4);
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

__attribute__((scn_string_bindings(0, data_140F2)))
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
  local_D4 = Script_ScenDataUtil(task_6C, 4, task_B4[arg2] * 13 + 6);
  local_CC = arg3;
  if ((local_CC == 1) | (local_CC == 3) | (local_CC == 5) | (local_CC == 7) | (local_CC == 9) | (local_CC == 18) | (local_CC == 21) | (local_CC == 25) | (local_CC == 27) | (local_CC == 36) | (local_CC == 37)) {
    local_CC = arg4;
    switch (local_CC) {
      case 1:
        local_E0 = Script_ScenDataUtil(&data_13548, 4, local_D4 * 5) | -0x80000000;
        break;
      case 0:
        local_E0 = Script_ScenDataUtil(&data_13548, 4, local_D4 * 5);
        break;
    }
  } else if ((local_CC == 2) | (local_CC == 4) | (local_CC == 6) | (local_CC == 8) | (local_CC == 10) | (local_CC == 19) | (local_CC == 22) | (local_CC == 24) | (local_CC == 26) | (local_CC == 28)) {
    local_CC = arg4;
    switch (local_CC) {
      case 1:
        local_E0 = Script_ScenDataUtil(&data_13548, 4, local_D4 * 5);
        break;
      case 0:
        local_E0 = Script_ScenDataUtil(&data_13548, 4, local_D4 * 5) | -0x80000000;
        break;
    }
  } else {
    if ((local_CC == 13) | (local_CC == 14) | (local_CC == 15) | (local_CC == 16) | (local_CC == 20) | (local_CC == 23) | (local_CC == 29)) {
      local_CC = arg4;
      switch (local_CC) {
        case 0:
          local_E0 = Script_ScenDataUtil(&data_13548, 4, local_D4 * 5);
          break;
        case 1:
          local_E0 = Script_ScenDataUtil(&data_13548, 4, local_D4 * 5) | -0x80000000;
          break;
        case 2:
          local_E0 = Script_ScenDataUtil(&data_13548, 4, local_D4 * 5);
          break;
        case 3:
          local_E0 = Script_ScenDataUtil(&data_13548, 4, local_D4 * 5) | -0x80000000;
          break;
      }
      goto L006988;
    }
  }
  L006988:;
  *(int32_t *)&local_EC = Script_ScenDataUtil(&data_13548, 5, local_D4 * 5 + 1);
  *(int32_t *)&local_E8 = Script_ScenDataUtil(&data_13548, 5, local_D4 * 5 + 2);
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

__attribute__((scn_string_bindings(0, data_1410E)))
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
      Script_SetCharaMoveTarget(arg0, &local_E0, local_BC[1], script_0697(arg5));
    }
  }
  return;
}

static void script_7FBB(int32_t arg0, DSWKDoorEntry *arg1, int32_t *arg2, DoorResourceDescriptor *arg3, MapObjectRecord *arg4, int32_t arg5, int32_t arg6) {
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
  L008025:;
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
  if (!(local_F8 >= 10)) goto L008025;
  local_F8 = 0;
  for (;;) {
    if (Script_ScenDataUtil(task_6C, 4, local_F8 * 13) == -1) {
      break;
    }
    Script_ScenDataUtil(task_6C, 14, local_F8 * 13 + 1, 0);
    local_F8 = local_F8 + 1;
  }
  Script_EnqueueSCNT(0);
  DSWK_BindTablesAndInitState(arg1, arg2, arg3, arg4, &task_7C, task_8C, task_B4, (shen_name *)task_DC, task_104, arg0, arg5, data_12538, data_12B28, (shen_name *)task_154);
  Script_scenCreateTask(script_8877, 0);
  return;
}

static void script_8147(DSWKDoorEntry *arg0, int32_t *arg1, DoorResourceDescriptor *arg2, MapObjectRecord *arg3, int32_t arg4) {
  task_6C = arg0;
  task_70 = arg1;
  task_74 = arg2;
  task_78 = arg3;
  DSWK_RebindDoorSystemTables(arg0, arg1, arg2, arg3, arg4);
  task_1EC = 1;
  return;
}

static void script_817B(void) {
  int32_t local_FC;
  local_FC = 0;
  L008183:;
  task_8C[local_FC] = -1;
  task_B4[local_FC] = -1;
  task_DC[local_FC] = 0;
  task_104[local_FC] = 0;
  task_12C[local_FC] = 0;
  task_17C[local_FC] = 0;
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

static void script_8281(int32_t arg0, int32_t arg1) {
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

static void script_82BB(void) {
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

static void script_831E(void) {
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

static void script_8381(int32_t arg0) {
  int32_t local_FC;
  local_FC = Script_ScenDataUtil(task_6C, 4, arg0 * 13 + 1);
  local_FC = local_FC | 2;
  Script_ScenDataUtil(task_6C, 14, arg0 * 13 + 1, local_FC);
  return;
}

static void script_83B8(int32_t arg0) {
  int32_t local_FC;
  local_FC = Script_ScenDataUtil(task_6C, 4, arg0 * 13 + 1);
  local_FC = local_FC & -3;
  Script_ScenDataUtil(task_6C, 14, arg0 * 13 + 1, local_FC);
  return;
}

static void script_83EF(int32_t arg0) {
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

static void script_8631(int32_t arg0) {
  int32_t local_FC;
  local_FC = Script_ScenDataUtil(task_6C, 4, arg0 * 13 + 1);
  local_FC = local_FC | 0x20;
  Script_ScenDataUtil(task_6C, 14, arg0 * 13 + 1, local_FC);
  script_8739(arg0);
  script_83EF(arg0);
  return;
}

static void script_867A(int32_t arg0) {
  int32_t local_FC;
  local_FC = Script_ScenDataUtil(task_6C, 4, arg0 * 13 + 1);
  local_FC = local_FC & -33;
  Script_ScenDataUtil(task_6C, 14, arg0 * 13 + 1, local_FC);
  script_8747(arg0);
  script_83EF(arg0);
  return;
}

static void script_86C3(int32_t arg0, int32_t arg1) {
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
    if (Script_ScenDataUtil(task_6C, 4, local_FC * 13) == -1) {
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
    if (task_1EC == 0) {
      local_F4 = DSWK_DoorUtils(DOOR_CMD_FIND_LOWEST_FLOAT_INDEX, 0x3FD851EA);
      if (-(local_F4 != -1) & -(task_8C[local_F4] == 1) & Script_IsPlayerAllowed(0) & -((&task_7C)->mode == 0)) {
        local_FC = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_B4[local_F4], 2);
        local_F8 = Script_ScenDataUtil(task_74, 4, Script_ScenDataUtil(task_78, 4, local_FC * 9 + 1) * 4 + 1);
        script_9648(1, local_FC, local_F4, local_F8);
        if (script_97FC(local_FC, local_F4, local_F8)) {
          local_F4 = -1;
          local_F8 = -1;
        }
      } else {
        script_9648(0, local_FC, local_F4, local_F8);
        local_F4 = -1;
        local_F8 = -1;
      }
      script_9A12(local_F4, local_F8);
    } else {
      task_1EC = 0;
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

__attribute__((scn_string_bindings(0, data_14199, 1, data_141B2)))
static void script_8A20(int32_t arg0) {
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

static void script_8BEF(int32_t arg0) {
  if (task_1A4[arg0] != -1) {
    Script_SetMapControlTaskFlag(task_1A4[arg0], 1);
    task_1A4[arg0] = -1;
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

static void script_92E9(int32_t arg0, int32_t arg1) {
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

static void script_931B(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  int32_t local_F8;
  local_F8 = script_8C20(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
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

__attribute__((scn_string_bindings(0, data_141CB)))
static void script_948B(int32_t arg0, int32_t arg1, int32_t arg2) {
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

static void script_9648(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
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
        local_FC = script_8988(arg2, arg3, local_FC);
      } else {
        local_F4 = arg3;
        if ((local_F4 == 21) | (local_F4 == 22) | (local_F4 == 23)) {
          local_FC = script_8988(arg2, arg3, local_FC);
          goto L00970C;
        }
      }
      L00970C:;
    } else {
      if ((Script_ScenDataUtil(task_6C, 4, task_B4[arg2] * 13 + 1) & 4) == 0) {
        local_FC = script_8988(arg2, arg3, local_FC);
        local_F8 = Script_ScenDataUtil(task_6C, 4, task_B4[arg2] * 13 + 6);
        if (local_F8 == 0) {
          local_FC = 0;
        } else {
          if (local_F8 >= 2) {
            Script_EVT_GetPosition(task_1CC, -1, &local_E8, 0);
            if (Script_CycleManagerUtils(CYCLE_CMD_COUNT_CYCLES_IN_RADIUS, &local_E8, 1.5f)) {
              local_FC = 0;
            }
            goto L0097A7;
          }
        }
      }
    }
  }
  L0097A7:;
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

__attribute__((scn_string_bindings(0, data_141DA)))
static int32_t script_97FC(int32_t arg0, int32_t arg1, int32_t arg2) {
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
            goto L0099B0;
          }
        }
        L0099B0:;
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

static void script_9A12(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  int32_t local_F8;
  local_FC = 0;
  L009A1A:;
  local_F8 = task_8C[local_FC];
  switch (local_F8) {
    case 6:
      if (!(arg0 != -1)) goto L009A3A;
      L009A3A:;
      script_9ADC(local_FC, arg1);
      break;
    case 5:
      if (!(arg0 != -1)) goto L009A5A;
      L009A5A:;
      script_9B3D(local_FC);
      break;
    case 4:
      if (!(arg0 != -1)) goto L009A78;
      L009A78:;
      script_9CE2(local_FC, arg0, arg1);
      break;
    case 3:
      if (!(arg0 != -1)) goto L009A9A;
      L009A9A:;
      script_9D7A(local_FC, arg1);
      break;
    case 2:
      script_9E5C(local_FC);
      break;
    case 1:
      break;
  }
  local_FC = local_FC + 1;
  if (!(local_FC >= 10)) goto L009A1A;
  return;
}

static void script_9ADC(int32_t arg0, int32_t arg1) {
  if (task_17C[arg0] == 0) {
    task_17C[arg0] = Script_scenCreateTask(script_1C76, 4, task_DC[arg0], task_154[arg0], arg0, arg1);
    task_12C[arg0] = task_DC[arg0];
    script_8A20(arg0);
  }
  return;
}

__attribute__((scn_string_bindings(0, data_141E9, 1, data_141FB)))
static void script_9B3D(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, task_DC[arg0])) {
    if (task_17C[arg0] == 0) {
      if (task_DC[arg0] == task_1CC) {
        task_7C.mode = 1;
        task_7C.entry_idx = task_B4[arg0];
        goto L009B92;
      }
      L009B92:;
      local_FC = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_B4[arg0], 2);
      local_F8 = Script_ScenDataUtil(task_74, 4, Script_ScenDataUtil(task_78, 4, local_FC * 9 + 1) * 4 + 1);
      script_931B(task_DC[arg0], task_154[arg0], task_B4[arg0], local_F8);
      task_17C[arg0] = Script_scenCreateTask(Script_ScenDataUtil(&data_135E8, 8, local_F8), 4, task_DC[arg0], task_154[arg0], arg0, local_F8);
      task_12C[arg0] = task_DC[arg0];
      if ((Script_ScenDataUtil(task_6C, 4, task_B4[arg0] * 13 + 1) & 0x20000) == 0) {
        Script_sprintf("Door Box is ON!!\012");
        script_8A20(arg0);
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

static void script_9CE2(int32_t arg0, int32_t arg1, int32_t arg2) {
  if ((task_17C[arg0] == 0) & (arg2 >= 0)) {
    task_DC[arg0] = task_1CC;
    task_7C.mode = 2;
    task_7C.entry_idx = task_B4[arg0];
    task_17C[arg0] = Script_scenCreateTask(Script_ScenDataUtil(&data_135E8, 8, arg2), 4, task_DC[arg0], task_154[arg0], arg0, arg2);
    task_12C[arg0] = task_DC[arg0];
    script_8A20(arg0);
  }
  return;
}

__attribute__((scn_string_bindings(0, data_1420E, 1, data_14220)))
static void script_9D7A(int32_t arg0, int32_t arg1) {
  if ((task_17C[arg0] == 0) & (arg1 >= 0)) {
    script_931B(task_DC[arg0], task_154[arg0], task_B4[arg0], arg1);
    task_17C[arg0] = Script_scenCreateTask(Script_ScenDataUtil(&data_135E8, 8, arg1), 4, task_DC[arg0], task_154[arg0], arg0, arg1);
    task_12C[arg0] = task_DC[arg0];
    if ((Script_ScenDataUtil(task_6C, 4, task_B4[arg0] * 13 + 1) & 0x20000) == 0) {
      Script_sprintf("Door Box is ON!!\012");
      script_8A20(arg0);
    } else {
      Script_sprintf("Door Box is OFF!!\012");
      task_1A4[arg0] = -1;
    }
  }
  return;
}

static void script_9E5C(int32_t arg0) {
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

static int32_t script_9EB3(void) {
  int32_t local_FC;
  int32_t local_F8;
  Script_srchUtils(SRCH_SCRIPT_InitEntryTableFromTaskOffset, &data_142B0);
  for (;;) {
    local_FC = Script_srchUtils(-1);
    local_F8 = local_FC;
    switch (local_F8) {
      case 0:
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
        break;
      case 7:
        break;
      case 8:
        break;
      case 9:
        break;
      case 10:
        break;
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_9F63(int32_t arg0, int32_t arg1) {
  task_234[arg0] = arg1;
  return;
}

static int32_t script_9F73(int32_t arg0) {
  int32_t local_FC;
  local_FC = task_234[arg0];
  return local_FC;
}

static int32_t script_9F87(int32_t arg0) {
  task_234[1] = SCNF_RequestActionSelector_000C(0x7FFF, 0xFFFF, 3);
  return Script_srchControl(0);
}

static int32_t script_9FA9(int32_t arg0) {
  script_C256(arg0);
  return Script_srchControl(0);
}

static int32_t script_9FBB(int32_t arg0) {
  script_BC27(arg0);
  return Script_srchControl(0);
}

static int32_t script_9FCD(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  local_FC = SCNF_UIUtils(arg0, arg1, 1, 14, 1, 0);
  {
    int32_t local_F8;
    local_F8 = local_FC & 0xF000;
    task_234[2] = local_F8;
  }
  {
    int32_t local_F8;
    local_F8 = local_FC & 0x7FF;
    task_234[3] = local_F8;
  }
  return Script_srchControl(0);
}

static void script_A017(void) {
  int32_t local_FC;
  int32_t local_F8;
  local_FC = Script_GetControllerInput(0);
  local_F8 = local_FC;
  if (local_F8 == 0x8000) {
    script_9F63(4, 2);
  } else if (local_F8 == 0x4000) {
    script_9F63(4, 3);
  } else if (local_F8 == 0x2000) {
    script_9F63(4, 1);
  } else if (local_F8 == 0x1000) {
    script_9F63(4, 0);
  } else if (local_F8 == 0x400) {
    script_9F63(4, 4);
  } else if (local_F8 == 0x200) {
    script_9F63(4, 5);
  } else if (local_F8 == 4) {
    script_9F63(4, 6);
  } else if (local_F8 == 2) {
    script_9F63(4, 7);
  } else {
    script_9F63(4, 13);
  }
  SCNF_WaitFrames_0000(1);
  return;
}

static int32_t script_A0FE(int32_t arg0) {
  task_234[1] = SCNF_RequestActionSelectorFromParams_0008(0xFFFF, 46, 47, 0xFFFF, 300, 0xFFFF, ACTION_SELECTOR_MODE_STANDARD_TIMEOUT_CANCEL);
  return Script_srchControl(0);
}

static void script_A12E(void) {
  task_234[1] = SCNF_RequestActionSelectorFromParams_0008(0xFFFF, 46, 47, 0xFFFF, 0x7FFF, 0xFFFF, ACTION_SELECTOR_MODE_STANDARD);
  Script_srchControl(0);
  return;
}

static void script_A15E(void) {
  task_234[1] = SCNF_RequestActionSelectorFromParams_0008(0xFFFF, 46, 47, 0xFFFF, 0x7FFF, 0xFFFF, ACTION_SELECTOR_MODE_STANDARD);
  Script_srchControl(0);
  return;
}

static void script_A18E(void) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  int16_t local_F2;
  int16_t local_F0;
  int16_t local_EE;
  int16_t local_EC;
  int16_t local_EA;
  int32_t local_E4;
  local_FC = 0;
  local_F2 = 2;
  while (local_F2 != 19) {
    local_F2 = *(int16_t *)((int8_t *)&data_1444C + (local_FC << 1));
    local_F0 = *(int16_t *)((int8_t *)&data_1444C + ((local_FC + 1) << 1));
    local_EE = *(int16_t *)((int8_t *)&data_1444C + ((local_FC + 2) << 1));
    local_EC = *(int16_t *)((int8_t *)&data_1444C + ((local_FC + 3) << 1));
    local_EA = *(int16_t *)((int8_t *)&data_1444C + ((local_FC + 4) << 1));
    local_E4 = local_F2;
    switch (local_E4) {
      case 19:
        goto L00A458;
        break;
      case 0:
        local_FC = local_F0 * 15 - 5;
        break;
      case 1:
        if (local_F0 == 0) {
          SCNF_WaitFrames_0000(1);
        } else {
          SCNF_WaitFrames_0000(local_F0);
        }
        break;
      case 2:
        task_234[1] = -1;
        Script_scenCreateTask(script_9F87, 1, local_F0);
        break;
      case 3:
        task_234[3] = -1;
        Script_scenCreateTask(script_9FCD, 2, local_EE, local_EC);
        break;
      case 15:
        script_A017();
        break;
      case 5:
        task_234[local_F0] = local_EE;
        break;
      case 6:
        task_234[local_F0] = task_234[local_F0] + local_EE;
        break;
      case 7:
        task_234[local_F0] = task_234[local_F0] - local_EE;
        break;
      case 8:
        local_F8 = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, (float)local_EE);
        task_234[local_F0] = local_F8;
        break;
      case 4:
        local_F8 = task_234[local_EE];
        if (local_F8 == local_EC) {
          local_FC = local_F0 * 15 - 5;
        }
        break;
      case 14:
        local_F8 = task_234[local_EE];
        local_F4 = task_234[local_EC];
        if (local_F8 == local_F4) {
          local_FC = local_F0 * 15 - 5;
        }
        break;
      case 9:
        script_AFAA(local_EE, local_F0, local_EC);
        break;
      case 10:
        script_AC72(local_F0);
        break;
      case 13:
        script_B848(local_F0);
        break;
      case 12:
        Script_scenCreateTask(script_9FA9, 1, local_F0);
        SCNF_WaitFrames_0000(local_EE);
        break;
      case 11:
        Script_scenCreateTask(script_9FBB, 1, local_F0);
        SCNF_WaitFrames_0000(local_EE);
        break;
      case 16:
        task_234[1] = -1;
        Script_scenCreateTask(script_A0FE, 1, local_F0);
        break;
    }
    local_FC = local_FC + 5;
  }
  L00A458:;
  return;
}

static void script_A459(void) {
  int16_t local_FE;
  int16_t local_FC;
  int16_t local_FA;
  int16_t local_F8;
  int16_t local_F6;
  uint8_t unused_F4[2];
  int32_t local_F0;
  int32_t unused_E4[3];
  Vector3 local_D8;
  int32_t unused_C8[4];
  GameDateTime local_C0;
  Script_ryoControl(PCCTL_UpdateRyoNavYaw, 0x52494B41);
  script_C41A();
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_STAND_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_PlayCharaMotion(CHARA_GJHM, 4110, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_PlayCharaMotion(CHARA_GJBM, 4132, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_scenCreateTask(script_D36A, 0);
  Script_scenCreateTask(script_D77D, 0);
  script_CB92();
  Script_scenCreateTask(script_CDFB, 0);
  Script_scenCreateTask(script_CE58, 0);
  Script_EVT_GetPosition(CHARA_AKIR, -1, &local_D8, ENTITY_XFORM_WORLD_SPACE);
  (&local_D8)->y = 1.2f;
  Script_ReturnTimeDate(&local_C0);
  local_FA = (&local_C0)->hours;
  local_F8 = (&local_C0)->minutes;
  local_F6 = (&local_C0)->seconds;
  if ((local_FA >= 22) & (local_F8 >= 30)) {
    int32_t local_BC;
    local_FE = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f);
    local_BC = local_FE;
    switch (local_BC) {
      case 0:
        Script_seqEnterEvtCam(7608, 0, AUTH_None);
        SCNF_WaitFrames_0000(30);
        Script_scenCreateTask(script_9FBB, 1, 73);
        SCNF_WaitFrames_0000(70);
        Script_seqEnterEvtCam(7626, 0, AUTH_None);
        Script_scenCreateTask(script_9FBB, 1, 65);
        SCNF_WaitFrames_0000(70);
        break;
      case 1:
        Script_seqEnterEvtCam(7608, 0, AUTH_None);
        SCNF_WaitFrames_0000(30);
        Script_scenCreateTask(script_9FBB, 1, 71);
        SCNF_WaitFrames_0000(70);
        Script_seqEnterEvtCam(7626, 0, AUTH_None);
        Script_scenCreateTask(script_9FBB, 1, 66);
        SCNF_WaitFrames_0000(70);
        break;
    }
  } else {
    Script_seqEnterEvtCam(7700, 0, AUTH_None);
    SCNF_WaitFrames_0000(30);
    local_FE = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f);
    {
      int32_t local_BC;
      local_BC = local_FE;
      switch (local_BC) {
        case 0:
          Script_scenCreateTask(script_9FBB, 1, 47);
          break;
        case 1:
          Script_scenCreateTask(script_9FBB, 1, 48);
          break;
        case 2:
          Script_scenCreateTask(script_9FBB, 1, 49);
          break;
      }
    }
    Script_scenCreateTask(script_CD0E, 0);
    SCNF_WaitFrames_0000(90);
    local_F0 = SCNF_RequestActionSelectorFromParams_0008(0xFFFF, 46, 47, 0xFFFF, 300, 0xFFFF, ACTION_SELECTOR_MODE_STANDARD_TIMEOUT_CANCEL);
    if (local_F0 == 46) {
      int32_t local_BC;
      Script_seqEnterEvtCam(7705, 0, AUTH_None);
      Script_scenCreateTask(script_9FBB, 1, 14);
      SCNF_WaitFrames_0000(35);
      Script_seqEnterEvtCam(7621, 0, AUTH_None);
      Script_scenCreateTask(script_9FBB, 1, 50);
      script_D815();
      Script_charaDisplay(SHEN_NAME('BAL0'), CDM_Hide);
      Script_charaDisplay(SHEN_NAME('BAL9'), CDM_Hide);
      SCNF_WaitFrames_0000(1);
      SCNF_WaitFrames_0000(19);
      Script_PlayCharaMotion(CHARA_GJHM, 4114, 0.0f, 100.0f, 0.0f, 1.0f, 10);
      Script_scenCreateTask(script_D3FD, 0);
      SCNF_WaitFrames_0000(30);
      Script_motmToggleFlags(CHARA_AKIR, 1, 0x80);
      Script_PlayCharaMotion(CHARA_AKIR, 4106, 0.0f, 70.0f, 0.0f, 1.0f, 10);
      SCNF_WaitFrames_0000(30);
      Script_executeAicaCommand(918953, 0, 0);
      Script_PlayCharaMotion(CHARA_GJBM, 4130, -1.0f, -1.0f, -1.0f, 1.0f, 0);
      Script_SetCharaMotionPlaybackRate(CHARA_GJBM, 0.5f);
      SCNF_WaitFrames_0000(20);
      Script_scenCreateTask(script_D6B8, 0);
      Script_scenCreateTask(script_CDD4, 0);
      Script_seqEnterEvtCam(7690, 0, AUTH_None);
      script_CCA6();
      Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
      while (Script_filmCheckOrSetTransitionUtil(-1)) {
        SCNF_WaitFrames_0000(1);
      }
      Script_PlayCharaMotion(CHARA_GJHM, 4130, -1.0f, -1.0f, -1.0f, 1.0f, 0);
      Script_PlayCharaMotion(CHARA_AKIR, 4105, 0.0f, 160.0f, 30.0f, 1.0f, 10);
      SCNF_WaitFrames_0000(120);
      Script_PlayCharaMotion(CHARA_AKIR, 4102, 0.0f, 183.0f, 0.0f, 1.0f, 10);
      SCNF_WaitFrames_0000(40);
      Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
      while (Script_filmCheckOrSetTransitionUtil(-1)) {
        SCNF_WaitFrames_0000(1);
      }
      Script_scenCreateTask(script_CDFB, 0);
      script_D82E();
      local_FC = script_DA5B(0);
      Script_sprintf("return : %d \012", local_FC);
      local_BC = local_FC;
      switch (local_BC) {
        case 2:
          script_D8A9();
          Script_seqEnterEvtCam(7702, 0, AUTH_None);
          SCNF_WaitFrames_0000(20);
          Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_STAND_LP, 0.0f, 60.0f, 0.0f, 1.0f, 30);
          if (task_248 == 1) {
            Script_scenCreateTask(script_9FBB, 1, 55);
            SCNF_WaitFrames_0000(90);
          }
          if ((task_248 >= 2) & (task_248 <= 5)) {
            Script_scenCreateTask(script_9FBB, 1, 58);
            SCNF_WaitFrames_0000(90);
          }
          if (task_248 >= 6) {
            Script_scenCreateTask(script_9FBB, 1, 59);
            SCNF_WaitFrames_0000(90);
          }
          Script_PlayCharaMotion(CHARA_GJHM, 4114, 0.0f, 100.0f, 0.0f, 1.0f, 10);
          Script_seqEnterEvtCam(7630, 0, AUTH_None);
          SCNF_WaitFrames_0000(20);
          Script_scenCreateTask(script_9FBB, 1, 66);
          Script_SetCharaLookAtPosition(CHARA_GJHM, 2, &local_D8, 1);
          SCNF_WaitFrames_0000(20);
          break;
        case 4:
          script_D8A9();
          Script_seqEnterEvtCam(7632, 0, AUTH_None);
          Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_STAND_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
          Script_PlayCharaMotion(CHARA_GJHM, 4132, -1.0f, -1.0f, -1.0f, 1.0f, 0);
          SCNF_WaitFrames_0000(20);
          Script_scenCreateTask(script_9FBB, 1, 13);
          SCNF_WaitFrames_0000(70);
          Script_seqEnterEvtCam(7626, 0, AUTH_None);
          Script_scenCreateTask(script_9FBB, 1, 62);
          SCNF_WaitFrames_0000(70);
          Script_scenCreateTask(script_9FBB, 1, 65);
          SCNF_WaitFrames_0000(70);
          break;
        case 3:
          break;
      }
    } else {
      int32_t local_BC;
      Script_seqEnterEvtCam(7701, 0, AUTH_None);
      Script_scenCreateTask(script_9FBB, 1, 13);
      SCNF_WaitFrames_0000(70);
      Script_seqEnterEvtCam(7704, 0, AUTH_None);
      Script_scenCreateTask(script_CD54, 0);
      local_FE = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f);
      local_BC = local_FE;
      switch (local_BC) {
        case 0:
          Script_scenCreateTask(script_9FBB, 1, 73);
          break;
        case 1:
          Script_scenCreateTask(script_9FBB, 1, 74);
          break;
      }
      SCNF_WaitFrames_0000(70);
    }
  }
  Script_filmCheckOrSetTransitionUtil(40, 0, 0, 0, 0, 255, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_SetCharaLookAtPosition(CHARA_AKIR, -98312, 0, 0);
  Script_FaceLookAtTargetUtils(CHARA_AKIR, 0);
  Script_SetCharaLookAtPosition(CHARA_GJHM, -98312, 0, 0);
  Script_FaceLookAtTargetUtils(CHARA_GJHM, 0);
  Script_SetCharaLookAtPosition(CHARA_GJBM, -98312, 0, 0);
  Script_FaceLookAtTargetUtils(CHARA_GJBM, 0);
  Script_charaDisplay(SHEN_NAME('B_QW'), CDM_Show);
  Script_charaDisplay(CHARA_AKIR, CDM_Show);
  Script_PlayCharaMotion(CHARA_GJHM, 4110, 0.0f, 223.0f, 0.0f, 0.7f, 10);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_STAND_LP, 0.0f, 60.0f, 0.0f, 1.0f, 30);
  Script_scenCreateTask(script_D36A, 0);
  SCNF_WaitFrames_0000(10);
  if (local_FC == 3) {
  } else {
    script_CBED();
  }
  SCNF_WaitFrames_0000(1);
  script_C821();
  SCNF_WaitFrames_0000(1);
  SCNF_WaitFrames_0000(1);
  Script_convSetCamInactiveFlag(1);
  Script_SetGameStateTo(AUTH_None);
  Script_filmCheckOrSetTransitionUtil(32, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_AC72(int32_t arg0) {
  int32_t local_FC;
  local_FC = arg0;
  switch (local_FC) {
    case 0:
      break;
    case 1:
      script_C41A();
      break;
    case 2:
      script_C583();
      break;
    case 3:
      script_C93A();
      break;
    case 17:
      script_CB5F();
      break;
    case 20:
      script_CB92();
      break;
    case 21:
      script_CBED();
      break;
    case 22:
      script_CC4B();
      break;
    case 23:
      script_CCA6();
      break;
    case 24:
      Script_scenCreateTask(script_CF12, 0);
      break;
    case 25:
      Script_scenCreateTask(script_CFB5, 0);
      break;
    case 26:
      Script_scenCreateTask(script_D110, 0);
      break;
    case 11:
      Script_scenCreateTask(script_D224, 0);
      break;
    case 12:
      Script_scenCreateTask(script_D2B7, 0);
      break;
    case 13:
      Script_scenCreateTask(script_D36A, 0);
      break;
    case 14:
      Script_scenCreateTask(script_D3FD, 0);
      break;
    case 15:
      Script_scenCreateTask(script_D6B8, 0);
      break;
    case 16:
      script_D74B();
      break;
    case 7:
      script_D7FC();
      break;
    case 8:
      script_D815();
      break;
    case 4:
      script_9F63(8, script_DA5B(1));
      break;
    case 5:
      script_D82E();
      break;
    case 6:
      script_D8A9();
      break;
    case 9:
      Script_scenCreateTask(script_D77D, 0);
      break;
    case 10:
      Script_scenCreateTask(script_D8ED, 0);
      break;
    case 27:
      Script_scenCreateTask(script_CD0E, 0);
      break;
    case 28:
      Script_scenCreateTask(script_CD54, 0);
      break;
    case 29:
      Script_scenCreateTask(script_CD9A, 0);
      break;
    case 30:
      Script_scenCreateTask(script_CDD4, 0);
      break;
    case 31:
      Script_scenCreateTask(script_CDFB, 0);
      break;
    case 32:
      Script_scenCreateTask(script_CE31, 0);
      break;
    case 33:
      Script_scenCreateTask(script_CE58, 0);
      break;
    case 34:
      Script_scenCreateTask(script_CE8E, 0);
      break;
    case 35:
      Script_scenCreateTask(script_CEB5, 0);
      break;
    case 36:
      Script_scenCreateTask(script_CEEB, 0);
      break;
    case 37:
      script_C9FA();
      break;
    case 38:
      Script_scenCreateTask(script_CA5D, 0);
      break;
    case 39:
      script_D9D7();
      break;
    case 18:
      script_CB20();
      break;
    case 19:
      script_CAF2();
      break;
  }
  return;
}

static void script_AFAA(int32_t arg0, int32_t arg1, int32_t arg2) {
  int32_t local_FC;
  local_FC = arg0;
  switch (local_FC) {
    case 0:
      local_FC = arg1;
      switch (local_FC) {
        case 0:
          break;
        case 1:
          Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_STAND_LP, 0.0f, 60.0f, 0.0f, 1.0f, 30);
          break;
        case 2:
          Script_motmToggleFlags(CHARA_AKIR, 1, 0x80);
          break;
        case 3:
          Script_motmToggleFlags(CHARA_AKIR, 2, 0x80);
          break;
        case 4:
          Script_PlayCharaMotion(CHARA_AKIR, 4097, 0.0f, 177.0f, 0.0f, 1.0f, 10);
          break;
        case 5:
          Script_PlayCharaMotion(CHARA_AKIR, 4098, 0.0f, 109.0f, 0.0f, 1.0f, 10);
          break;
        case 6:
          Script_PlayCharaMotion(CHARA_AKIR, 4100, 0.0f, 79.0f, 0.0f, 1.0f, 10);
          break;
        case 7:
          Script_PlayCharaMotion(CHARA_AKIR, 4101, 0.0f, 89.0f, 0.0f, 1.0f, 10);
          break;
        case 8:
          Script_PlayCharaMotion(CHARA_AKIR, 4102, 0.0f, 183.0f, 0.0f, 1.0f, 10);
          break;
        case 9:
          Script_PlayCharaMotion(CHARA_AKIR, 4103, 0.0f, 109.0f, 0.0f, 1.0f, 10);
          break;
        case 10:
          Script_PlayCharaMotion(CHARA_AKIR, 4105, 0.0f, 160.0f, 30.0f, 1.0f, 10);
          break;
        case 11:
          Script_PlayCharaMotion(CHARA_AKIR, 4106, 0.0f, 70.0f, 0.0f, 1.0f, 10);
          break;
        case 12:
          Script_PlayCharaMotion(CHARA_AKIR, 4107, 0.0f, 97.0f, 18.0f, 3.0f, 10);
          break;
        case 13:
          Script_PlayCharaMotion(CHARA_AKIR, 4104, 0.0f, 216.0f, 0.0f, 1.0f, 10);
          break;
        case 14:
          Script_FaceEx(CHARA_AKIR, 1, 0, 4);
          break;
        case 15:
          Script_FaceEx(CHARA_AKIR, 1, 1, 4);
          break;
        case 16:
          Script_FaceEx(CHARA_AKIR, 1, 2, 4);
          break;
        case 17:
          Script_FaceEx(CHARA_AKIR, 1, 3, 4);
          break;
        case 18:
          Script_FaceEx(CHARA_AKIR, 1, 4, 4);
          break;
        case 19:
          Script_FaceEx(CHARA_AKIR, 1, 8, 4);
          break;
        case 20:
          Script_FaceEx(CHARA_AKIR, 1, 7, 4);
          break;
        case 21:
          Script_FaceEx(CHARA_AKIR, 1, 9, 4);
          break;
        case 22:
          Script_FaceEx(CHARA_AKIR, 1, 5, 4);
          break;
        case 23:
          Script_FaceEx(CHARA_AKIR, 1, 6, 4);
          break;
        case 24:
          Script_FaceEx(CHARA_AKIR, 1, 11, 4);
          break;
        case 25:
          Script_FaceEx(CHARA_AKIR, 1, 10, 4);
          break;
      }
      break;
    case 1:
      local_FC = arg1;
      switch (local_FC) {
        case 0:
          break;
        case 14:
          Script_FaceEx(CHARA_GJBM, 1, 0, 4);
          break;
        case 20:
          Script_FaceEx(CHARA_GJBM, 1, 7, 4);
          break;
        case 21:
          Script_FaceEx(CHARA_GJBM, 1, 9, 4);
          break;
        case 27:
          Script_PlayCharaMotion(CHARA_GJBM, 4132, -1.0f, -1.0f, -1.0f, 1.0f, 0);
          break;
        case 28:
          Script_PlayCharaMotion(CHARA_GJBM, 4130, -1.0f, -1.0f, -1.0f, 1.0f, 0);
          Script_SetCharaMotionPlaybackRate(CHARA_GJBM, 0.5f);
          break;
        case 29:
          Script_PlayCharaMotion(CHARA_GJBM, 4109, 0.0f, 101.0f, 0.0f, 1.0f, 10);
          break;
        case 30:
          Script_PlayCharaMotion(CHARA_GJBM, 4108, 0.0f, 141.0f, 0.0f, 1.0f, 10);
          break;
        case 31:
          Script_PlayCharaMotion(CHARA_GJBM, 4118, 0.0f, 199.0f, 0.0f, 1.0f, 10);
          break;
      }
      break;
    case 2:
      local_FC = arg1;
      switch (local_FC) {
        case 0:
          break;
        case 14:
          Script_FaceEx(CHARA_GJHM, 1, 0, 4);
          break;
        case 20:
          Script_FaceEx(CHARA_GJHM, 1, 7, 4);
          break;
        case 21:
          Script_FaceEx(CHARA_GJHM, 1, 9, 4);
          break;
        case 32:
          Script_PlayCharaMotion(CHARA_GJHM, 4132, -1.0f, -1.0f, -1.0f, 1.0f, 0);
          break;
        case 33:
          Script_PlayCharaMotion(CHARA_GJHM, 4130, -1.0f, -1.0f, -1.0f, 1.0f, 0);
          break;
        case 34:
          Script_PlayCharaMotion(CHARA_GJHM, 4110, 0.0f, 223.0f, 0.0f, 0.7f, 10);
          break;
        case 35:
          Script_PlayCharaMotion(CHARA_GJHM, 4111, 0.0f, 116.0f, 0.0f, 1.0f, 10);
          break;
        case 36:
          Script_PlayCharaMotion(CHARA_GJHM, 4112, 0.0f, 164.0f, 0.0f, 1.0f, 10);
          break;
        case 37:
          Script_PlayCharaMotion(CHARA_GJHM, 4113, 0.0f, 213.0f, 0.0f, 1.0f, 10);
          break;
        case 38:
          Script_PlayCharaMotion(CHARA_GJHM, 4114, 0.0f, 100.0f, 0.0f, 1.0f, 10);
          break;
        case 39:
          Script_PlayCharaMotion(CHARA_GJHM, 4115, 0.0f, 141.0f, 0.0f, 1.0f, 10);
          break;
        case 40:
          Script_PlayCharaMotion(CHARA_GJHM, 4116, 0.0f, 180.0f, 20.0f, 1.0f, 10);
          break;
        case 41:
          Script_PlayCharaMotion(CHARA_GJHM, 4117, 0.0f, 240.0f, 0.0f, 2.0f, 10);
          break;
        case 42:
          Script_PlayCharaMotion(CHARA_GJHM, 4118, 0.0f, 199.0f, 0.0f, 1.0f, 10);
          break;
        case 43:
          Script_PlayCharaMotion(CHARA_GJHM, 4119, 0.0f, 223.0f, 0.0f, 1.0f, 30);
          break;
        case 44:
          Script_PlayCharaMotion(CHARA_GJHM, 4120, 0.0f, 146.0f, 0.0f, 1.0f, 10);
          break;
        case 45:
          Script_PlayCharaMotion(CHARA_GJHM, 4121, 0.0f, 235.0f, 30.0f, 1.0f, 10);
          break;
        case 46:
          Script_PlayCharaMotion(CHARA_GJHM, 4122, 0.0f, 152.0f, 0.0f, 1.0f, 10);
          break;
        case 47:
          Script_PlayCharaMotion(CHARA_GJHM, 4123, 0.0f, 76.0f, 0.0f, 2.0f, 10);
          break;
        case 48:
          Script_PlayCharaMotion(CHARA_GJHM, 4124, 0.0f, 67.0f, 0.0f, 1.0f, 10);
          break;
        case 49:
          Script_PlayCharaMotion(CHARA_GJHM, 4125, 0.0f, 75.0f, 0.0f, 1.0f, 10);
          break;
        case 50:
          Script_PlayCharaMotion(CHARA_GJHM, 4126, 0.0f, 40.0f, 0.0f, 1.0f, 10);
          break;
        case 51:
          Script_PlayCharaMotion(CHARA_GJHM, 4127, 0.0f, 30.0f, 0.0f, 1.0f, 10);
          break;
        case 52:
          Script_PlayCharaMotion(CHARA_GJHM, 4128, 0.0f, 30.0f, 0.0f, 1.0f, 10);
          break;
        case 53:
          Script_PlayCharaMotion(CHARA_GJHM, 4129, 0.0f, 390.0f, 0.0f, 1.0f, 10);
          break;
      }
      break;
  }
  return;
}

static void script_B848(int32_t arg0) {
  {
    int32_t local_FC;
    local_FC = arg0;
    switch (local_FC) {
      case 0:
        break;
      case 1:
        Script_seqEnterEvtCam(7601, 0, AUTH_None);
        break;
      case 2:
        Script_seqEnterEvtCam(7602, 0, AUTH_None);
        break;
      case 3:
        Script_seqEnterEvtCam(7603, 0, AUTH_None);
        break;
      case 4:
        Script_seqEnterEvtCam(7604, 0, AUTH_None);
        break;
      case 5:
        Script_seqEnterEvtCam(7605, 0, AUTH_None);
        break;
      case 6:
        Script_seqEnterEvtCam(7606, 0, AUTH_None);
        break;
      case 7:
        Script_seqEnterEvtCam(7607, 0, AUTH_None);
        break;
      case 8:
        Script_seqEnterEvtCam(7608, 0, AUTH_None);
        break;
      case 9:
        Script_seqEnterEvtCam(7609, 0, AUTH_None);
        break;
      case 10:
        Script_seqEnterEvtCam(7610, 0, AUTH_None);
        break;
      case 11:
        Script_seqEnterEvtCam(7611, 0, AUTH_None);
        break;
      case 12:
        Script_seqEnterEvtCam(7620, 0, AUTH_None);
        break;
      case 13:
        Script_seqEnterEvtCam(7621, 0, AUTH_None);
        break;
      case 14:
        Script_seqEnterEvtCam(7625, 0, AUTH_None);
        break;
      case 15:
        Script_seqEnterEvtCam(7626, 0, AUTH_None);
        break;
      case 16:
        Script_seqEnterEvtCam(7627, 0, AUTH_None);
        break;
      case 17:
        Script_seqEnterEvtCam(7630, 0, AUTH_None);
        break;
      case 18:
        Script_seqEnterEvtCam(7631, 0, AUTH_None);
        break;
      case 19:
        Script_seqEnterEvtCam(7632, 0, AUTH_None);
        break;
      case 20:
        Script_seqEnterEvtCam(7633, 0, AUTH_None);
        break;
      case 21:
        Script_seqEnterEvtCam(7634, 0, AUTH_None);
        break;
      case 22:
        Script_seqEnterEvtCam(7635, 0, AUTH_None);
        break;
      case 23:
        Script_seqEnterEvtCam(7636, 0, AUTH_None);
        break;
      case 24:
        Script_seqEnterEvtCam(7640, 0, AUTH_None);
        break;
      case 25:
        Script_seqEnterEvtCam(7641, 0, AUTH_None);
        break;
      case 26:
        Script_seqEnterEvtCam(7642, 0, AUTH_None);
        break;
      case 27:
        Script_seqEnterEvtCam(7690, 0, AUTH_None);
        break;
      case 28:
        Script_seqEnterEvtCam(7691, 0, AUTH_None);
        break;
      case 29:
        Script_seqEnterEvtCam(7692, SHEN_NAME('B_QW'), SHEN_NAME('B_QW'));
        break;
      case 30:
        Script_seqEnterEvtCam(7693, 0, AUTH_None);
        break;
      case 31:
        Script_seqEnterEvtCam(7694, 0, AUTH_None);
        break;
      case 32:
        SCNF_WaitFrames_0000(15);
        Script_seqEnterEvtCam(7695, 0, AUTH_None);
        break;
      case 33:
        SCNF_WaitFrames_0000(15);
        Script_seqEnterEvtCam(7696, 0, AUTH_None);
        break;
    }
  }
  {
    int32_t local_FC;
    local_FC = arg0;
    if (local_FC == 0) {
    } else if (local_FC == 1) {
      Script_filmCheckOrSetTransitionUtil(32, 255, 0, 0, 0, 0, 0, 0, 0);
      while (Script_filmCheckOrSetTransitionUtil(-1)) {
        SCNF_WaitFrames_0000(1);
      }
    } else if (local_FC == 29) {
      Script_charaSetRenderOpacity(SHEN_NAME('B_QW'), 0.5f);
      Script_charaSetRenderOpacity(SHEN_NAME('BAL0'), 0.8f);
      Script_SetCamFactor(0.0f);
    } else if (local_FC == 30) {
      Script_charaSetRenderOpacity(SHEN_NAME('B_QW'), 1.0f);
      Script_charaSetRenderOpacity(SHEN_NAME('BAL0'), 1.0f);
      Script_SetCamFactor(1.0f);
    } else {
      Script_SetCamFactor(0.0f);
    }
  }
  SCNF_WaitFrames_0000(1);
  return;
}

__attribute__((scn_string_bindings(14, data_15152, 50, data_152BA, 52, data_152CE)))
static void script_BC27(int32_t arg0) {
  int32_t local_FC;
  local_FC = arg0;
  switch (local_FC) {
    case 75:
      break;
    case 1:
      Script_ShowUIMessageFromID("E1039A001");
      break;
    case 2:
      Script_ShowUIMessageFromID("E1039A002");
      break;
    case 3:
      Script_ShowUIMessageFromID("E1039A003");
      break;
    case 4:
      Script_ShowUIMessageFromID("E1039A004");
      break;
    case 5:
      Script_ShowUIMessageFromID("E1039A005");
      break;
    case 6:
      Script_ShowUIMessageFromID("E1039A006");
      break;
    case 7:
      Script_ShowUIMessageFromID("E1039A007");
      break;
    case 8:
      Script_ShowUIMessageFromID("E1039A008");
      break;
    case 9:
      Script_ShowUIMessageFromID("E1039A009");
      break;
    case 10:
      Script_ShowUIMessageFromID("E1039A010");
      break;
    case 11:
      Script_ShowUIMessageFromID("E1039A011");
      break;
    case 12:
      Script_ShowUIMessageFromID("E1039A012");
      break;
    case 13:
      Script_ShowUIMessageFromID("E1039A013");
      break;
    case 14:
      Script_ShowUIMessageFromID("E1039A014");
      break;
    case 15:
      Script_ShowUIMessageFromID("E1039A015");
      break;
    case 16:
      Script_ShowUIMessageFromID("E1039B001");
      break;
    case 17:
      Script_ShowUIMessageFromID("E1039B002");
      break;
    case 18:
      Script_ShowUIMessageFromID("E1039B003");
      break;
    case 19:
      Script_ShowUIMessageFromID("E1039B004");
      break;
    case 20:
      Script_ShowUIMessageFromID("E1039B005");
      break;
    case 21:
      Script_ShowUIMessageFromID("E1039B006");
      break;
    case 22:
      Script_ShowUIMessageFromID("E1039B007");
      break;
    case 23:
      Script_ShowUIMessageFromID("E1039B008");
      break;
    case 24:
      Script_ShowUIMessageFromID("E1039B009");
      break;
    case 25:
      Script_ShowUIMessageFromID("E1039B010");
      break;
    case 26:
      Script_ShowUIMessageFromID("E1039B011");
      break;
    case 27:
      Script_ShowUIMessageFromID("E1039B012");
      break;
    case 28:
      Script_ShowUIMessageFromID("E1039B013");
      break;
    case 29:
      Script_ShowUIMessageFromID("E1039B014");
      break;
    case 30:
      Script_ShowUIMessageFromID("E1039B015");
      break;
    case 31:
      Script_ShowUIMessageFromID("E1039B016");
      break;
    case 32:
      Script_ShowUIMessageFromID("E1039B017");
      break;
    case 33:
      Script_ShowUIMessageFromID("E1039B018");
      break;
    case 34:
      Script_ShowUIMessageFromID("E1039B019");
      break;
    case 35:
      Script_ShowUIMessageFromID("E1039B020");
      break;
    case 36:
      Script_ShowUIMessageFromID("E1039B021");
      break;
    case 37:
      Script_ShowUIMessageFromID("E1039B022");
      break;
    case 38:
      Script_ShowUIMessageFromID("E1039B023");
      break;
    case 39:
      Script_ShowUIMessageFromID("E1039B024");
      break;
    case 40:
      Script_ShowUIMessageFromID("E1039B025");
      break;
    case 41:
      Script_ShowUIMessageFromID("E1039B026");
      break;
    case 42:
      Script_ShowUIMessageFromID("E1039B027");
      break;
    case 43:
      Script_ShowUIMessageFromID("E1039B028");
      break;
    case 44:
      Script_ShowUIMessageFromID("E1039B029");
      break;
    case 45:
      Script_ShowUIMessageFromID("E1039B030");
      break;
    case 46:
      Script_ShowUIMessageFromID("E1039B031");
      break;
    case 47:
      Script_ShowUIMessageFromID("E1039B032");
      break;
    case 48:
      Script_ShowUIMessageFromID("E1039B033");
      break;
    case 49:
      Script_ShowUIMessageFromID("E1039B034");
      break;
    case 50:
      Script_ShowUIMessageFromID("E1039B035");
      break;
    case 51:
      Script_ShowUIMessageFromID("E1039B036");
      break;
    case 52:
      Script_ShowUIMessageFromID("E1039B037");
      break;
    case 53:
      Script_ShowUIMessageFromID("E1039B038");
      break;
    case 54:
      Script_ShowUIMessageFromID("E1039B039");
      break;
    case 55:
      Script_ShowUIMessageFromID("E1039B040");
      break;
    case 56:
      Script_ShowUIMessageFromID("E1039B041");
      break;
    case 57:
      Script_ShowUIMessageFromID("E1039B042");
      break;
    case 58:
      Script_ShowUIMessageFromID("E1039B043");
      break;
    case 59:
      Script_ShowUIMessageFromID("E1039B044");
      break;
    case 60:
      Script_ShowUIMessageFromID("E1039B045");
      break;
    case 61:
      Script_ShowUIMessageFromID("E1039B046");
      break;
    case 62:
      Script_ShowUIMessageFromID("E1039B047");
      break;
    case 63:
      Script_ShowUIMessageFromID("E1039B048");
      break;
    case 64:
      Script_ShowUIMessageFromID("E1039B049");
      break;
    case 65:
      Script_ShowUIMessageFromID("E1039B050");
      break;
    case 66:
      Script_ShowUIMessageFromID("E1039B051");
      break;
    case 67:
      Script_ShowUIMessageFromID("E1039C001");
      break;
    case 68:
      Script_ShowUIMessageFromID("E1039C002");
      break;
    case 69:
      Script_ShowUIMessageFromID("E1039C003");
      break;
    case 70:
      Script_ShowUIMessageFromID("E1039C004");
      break;
    case 71:
      Script_ShowUIMessageFromID("E1039C005");
      break;
    case 72:
      Script_ShowUIMessageFromID("E1039C006");
      break;
    case 73:
      Script_ShowUIMessageFromID("E1039C007");
      break;
    case 74:
      Script_ShowUIMessageFromID("E1039C008");
      break;
  }
  SCNF_WaitFrames_0000(1);
  return;
}

static void script_C256(int32_t arg0) {
  int32_t local_FC;
  local_FC = arg0;
  switch (local_FC) {
    case 0:
      break;
    case 1:
      Script_executeAicaCommand(1449, 0, 0);
      break;
    case 2:
      Script_executeAicaCommand(66985, 0, 0);
      break;
    case 3:
      Script_executeAicaCommand(132521, 0, 0);
      break;
    case 4:
      Script_executeAicaCommand(198057, 0, 0);
      break;
    case 5:
      Script_executeAicaCommand(263593, 0, 0);
      break;
    case 6:
      Script_executeAicaCommand(263593, 0, 0);
      break;
    case 7:
      Script_executeAicaCommand(329129, 0, 0);
      break;
    case 8:
      Script_executeAicaCommand(394665, 0, 0);
      break;
    case 9:
      Script_executeAicaCommand(460201, 0, 0);
      break;
    case 10:
      Script_executeAicaCommand(525737, 0, 0);
      break;
    case 11:
      Script_executeAicaCommand(591273, 0, 0);
      break;
    case 12:
      Script_executeAicaCommand(656809, 0, 0);
      break;
    case 13:
      Script_executeAicaCommand(722345, 0, 0);
      break;
    case 14:
      Script_executeAicaCommand(787881, 0, 0);
      break;
    case 15:
      Script_executeAicaCommand(853417, 0, 0);
      break;
    case 16:
      Script_executeAicaCommand(918953, 0, 0);
      break;
    case 17:
      Script_executeAicaCommand(0x1105A9, 0, 0);
      break;
  }
  SCNF_WaitFrames_0000(1);
  return;
}

__attribute__((scn_string_bindings(0, data_153AA)))
static void script_C41A(void) {
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_FaceEx(CHARA_AKIR, 1, 0, 2);
  Script_ResetHandMotion(CHARA_AKIR, -1, 0, 0);
  Script_ResetHandMotion(CHARA_AKIR, -1, 1, 0);
  Script_shenClearFreeState(0);
  Script_scenUtils(SCENARIO_CMD_INIT_SCNT);
  Script_facemotUtils(CHARA_GJHM, 0, 0);
  Script_facemotUtils(CHARA_GJBM, 0, 0);
  Script_motmToggleFlags(CHARA_GJHM, 1, 0x40);
  Script_motmSetBlendFrames(CHARA_GJHM, MOTM_BLEND_FRAMES_30);
  Script_motmToggleFlags(CHARA_AKIR, 1, 0x40);
  Script_motmSetBlendFrames(CHARA_AKIR, MOTM_BLEND_FRAMES_20);
  Script_LoadVoiceFile(1, "E1039");
  Script_executeAicaCommand(1184, 1, 80);
  Script_executeAicaCommand(1184, 4, 127);
  Script_SetCPAMFlags(CHARA_AKIR, 0);
  Script_SetCPAMFlags(CHARA_GJBM, 0);
  Script_SetCPAMFlags(CHARA_GJHM, 0);
  Script_SetCameraFlags(0);
  Script_scenToggleActiveFor(CHARA_AKIR, 0);
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 1);
  Script_scenToggleActiveFor(CHARA_GJHM, 0);
  Script_charaSetRefboardFlagTo(CHARA_GJHM, 1);
  Script_scenToggleActiveFor(CHARA_GJBM, 0);
  Script_charaSetRefboardFlagTo(CHARA_GJBM, 1);
  Script_charaDisplay(SHEN_NAME('B_QW'), CDM_Show);
  Script_PlayCharaMotion(CHARA_GJHM, 4132, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_ccowUtils(CHARA_AKIR, 2);
  Script_ccowUtils(CHARA_GJHM, 2);
  return;
}

__attribute__((scn_string_bindings(0, data_153B0, 1, data_153B6)))
static void script_C583(void) {
  int16_t local_FE;
  uint8_t unused_F8[6];
  GameDateTime local_F0;
  int32_t local_EC;
  Script_LoadVoiceFile(0, "E1039");
  Script_executeAicaCommand(1184, 1, 100);
  Script_SetCameraSettings(5, 0, 1, 0);
  Script_convSetCamInactiveFlag(1);
  Script_SetCPAMFlags(CHARA_AKIR, 1);
  Script_SetCPAMFlags(CHARA_GJBM, 1);
  Script_SetCPAMFlags(CHARA_GJHM, 1);
  Script_SetGameStateTo(AUTH_None);
  Script_SetCameraFlags(1);
  Script_scenToggleActiveFor(CHARA_AKIR, 1);
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 2);
  Script_scenToggleActiveFor(CHARA_GJHM, 1);
  Script_charaSetRefboardFlagTo(CHARA_GJHM, 2);
  Script_scenToggleActiveFor(CHARA_GJBM, 1);
  Script_charaSetRefboardFlagTo(CHARA_GJBM, 2);
  Script_PlayCharaMotion(CHARA_HIRI, 4131, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_ccowUtils(CHARA_AKIR, 1);
  Script_ccowUtils(CHARA_GJHM, 1);
  Script_scenUtils(SCENARIO_CMD_UPDATE_TIMER);
  Script_sprintf("ＡＵＴＯ＿０１: %d \012", script_9F73(11));
  Script_sprintf("ＡＵＴＯ＿ＮＯ: %d \012", script_9F73(12));
  Script_sprintf("ROBART_TALK_ST: %d \012", Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 876));
  if (script_9F73(12) == 0) {
    if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 876) == 1) {
      if (script_9F73(8) == 2) {
        Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_DJAZ_BALL_EVENT_SUCCESS, 1);
        Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 823, 1);
        Script_ReturnTimeDate(&local_F0);
        if (((&local_F0)->hours >= 19) & ((&local_F0)->hours <= 23)) {
          local_EC = (&local_F0)->month;
          switch (local_EC) {
            case 1:
              local_FE = (&local_F0)->day + 31;
              break;
            case 2:
              local_FE = (&local_F0)->day + 62;
              break;
            case 3:
              local_FE = (&local_F0)->day + 90;
              break;
            case 12:
              local_FE = (&local_F0)->day;
              break;
          }
          Script_scenUtils(SCENARIO_CMD_SET_STEP_LUT, 31, local_FE);
        }
      } else {
        Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 823, 1);
      }
    }
  } else {
    script_CBED();
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 874, 1);
    Script_ryoControl(RYO_RestoreControl);
    Script_FaceEx(CHARA_AKIR, 0, 0, 2);
    Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
    Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
    Script_shenClearFreeState(1);
    Script_filmCheckOrSetTransitionUtil(32, 255, 0, 0, 0, 0, 0, 0, 0);
    while (Script_filmCheckOrSetTransitionUtil(-1)) {
      SCNF_WaitFrames_0000(1);
    }
  }
  Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 876, 1);
  Script_sprintf("day_chk: %d, %d\012", local_FE, Script_scenUtils(SCENARIO_CMD_GET_STEP_LUT, 31));
  return;
}

__attribute__((scn_string_bindings(0, data_15406)))
static void script_C821(void) {
  Script_LoadVoiceFile(0, "E1039");
  Script_executeAicaCommand(1184, 1, 100);
  Script_SetCameraSettings(5, 0, 1, 0);
  Script_SetCPAMFlags(CHARA_AKIR, 1);
  Script_SetCPAMFlags(CHARA_GJBM, 1);
  Script_SetCPAMFlags(CHARA_GJHM, 1);
  Script_SetCameraFlags(1);
  Script_scenToggleActiveFor(CHARA_AKIR, 1);
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 2);
  Script_scenToggleActiveFor(CHARA_GJHM, 1);
  Script_charaSetRefboardFlagTo(CHARA_GJHM, 2);
  Script_scenToggleActiveFor(CHARA_GJBM, 1);
  Script_charaSetRefboardFlagTo(CHARA_GJBM, 2);
  Script_PlayCharaMotion(CHARA_HIRI, 4131, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_ccowUtils(CHARA_AKIR, 1);
  Script_ccowUtils(CHARA_GJHM, 1);
  Script_scenUtils(SCENARIO_CMD_UPDATE_TIMER);
  Script_ryoControl(RYO_RestoreControl);
  Script_FaceEx(CHARA_AKIR, 0, 0, 2);
  Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
  Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
  Script_shenClearFreeState(1);
  Script_ryoControl(RYO_RestoreControl);
  return;
}

__attribute__((scn_string_bindings(0, data_1540C)))
static void script_C93A(void) {
  script_9F63(1, 0);
  script_9F63(2, 0);
  script_9F63(3, 0);
  script_9F63(4, 0);
  script_9F63(5, 0);
  script_9F63(6, 1);
  script_9F63(7, 0);
  script_9F63(8, 0);
  script_9F63(9, 0);
  script_9F63(10, 0);
  script_9F63(11, Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 876));
  script_9F63(12, Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 874));
  script_9F63(13, 0);
  script_9F63(14, 0);
  Script_sprintf("ＡＵＴＯ＿０１: %d \012", script_9F73(11));
  return;
}

static void script_C9FA(void) {
  int32_t local_FC;
  int32_t local_F8;
  local_FC = 0;
  while (local_FC < 75) {
    script_BC27(local_FC);
    local_F8 = 0;
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
      local_F8 = local_F8 + 1;
    }
    Script_sprintf("VOICE_NO %03d  frame %d\012", local_FC + 1, local_F8);
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static int32_t script_CA5D(void) {
  int32_t local_FC;
  Vector3 local_F0;
  Vector3i local_E4;
  while (1) {
    Script_mtwkGetSetRotation(SHEN_NAME('BAL1'), &local_E4, 0);
    Script_EVT_GetPosition(SHEN_NAME('B_QW'), -1, &local_F0, 0);
    local_FC = Script_FloatUtils(ScriptMathOp_Atan2ReverseVector, *(&task_28C + 2) - *(&task_24C + 2), *(&task_28C + 4) - *(&task_24C + 4));
    Script_PrintDebugString(5, 23, " DEG: %5d", local_FC / 182);
    Script_PrintDebugString(5, 24, " P  : %d", local_FC);
    SCNF_WaitFrames_0000(1);
    if (script_9F73(5) == 1) {
      break;
    }
  }
  return Script_srchControl(0);
}

static void script_CAF2(void) {
  Script_filmCheckOrSetTransitionUtil(40, 0, 0, 0, 0, 255, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_CB20(void) {
  Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_CB4E(void) {
  int32_t local_FC;
  local_FC = Script_shenGetStats(STATS_CASH);
  return;
}

static void script_CB5C(void) {
  int32_t unused_FC;
  return;
}

static void script_CB5F(void) {
  int32_t local_FC;
  local_FC = Script_shenGetStats(STATS_CASH);
  local_FC = local_FC - 1000;
  SCNF_SetCashValue(-1000, 1);
  Script_SetStats(STATS_CASH, local_FC);
  SCNF_WaitFrames_0000(1);
  return;
}

static void script_CB92(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  local_F4.x = 4.0f;
  (&local_F4)->y = 0.0f;
  (&local_F4)->z = -6.3f;
  Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &local_F4);
  local_E8.x = 0;
  (&local_E8)->y = 0;
  (&local_E8)->z = 0;
  Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &local_E8, 0);
  return;
}

static void script_CBED(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  local_F4.x = 3.5f;
  (&local_F4)->y = 0.0f;
  (&local_F4)->z = -4.5f;
  Script_EVT_SetPosition(CHARA_AKIR, 0x78000000, &local_F4);
  local_E8.x = 0;
  (&local_E8)->y = 0x8000;
  (&local_E8)->z = 0;
  Script_EVT_SetRotation(CHARA_AKIR, 0x78000000, &local_E8, 0);
  return;
}

static void script_CC4B(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  local_F4.x = 4.0f;
  (&local_F4)->y = 0.0f;
  (&local_F4)->z = -4.4f;
  Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &local_F4);
  local_E8.x = 0;
  (&local_E8)->y = 0;
  (&local_E8)->z = 0;
  Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &local_E8, 0);
  return;
}

static void script_CCA6(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t unused_DC[3];
  local_F4.x = 2.6f;
  (&local_F4)->y = 0.0f;
  (&local_F4)->z = -7.45f;
  Script_EVT_SetPosition(CHARA_AKIR, 0x78000000, &local_F4);
  local_E8.x = 0;
  (&local_E8)->y = 1820;
  (&local_E8)->z = 0;
  Script_EVT_SetRotation(CHARA_AKIR, 0x78000000, &local_E8, 0);
  Script_motmSetBlendFrames(CHARA_AKIR, MOTM_BLEND_FRAMES_30);
  return;
}

static int32_t script_CD0E(void) {
  Vector3 local_F4;
  Script_EVT_GetPosition(CHARA_GJHM, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  (&local_F4)->y = 1.2f;
  Script_SetCharaLookAtPosition(CHARA_AKIR, 2, &local_F4, 2);
  Script_FaceLookAtTargetUtils(CHARA_AKIR, 2, &local_F4, 10);
  return Script_srchControl(0);
}

static int32_t script_CD54(void) {
  Vector3 local_F4;
  Script_EVT_GetPosition(CHARA_GJBM, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  (&local_F4)->y = 1.2f;
  Script_SetCharaLookAtPosition(CHARA_AKIR, 2, &local_F4, 2);
  Script_FaceLookAtTargetUtils(CHARA_AKIR, 2, &local_F4, 10);
  return Script_srchControl(0);
}

static int32_t script_CD9A(void) {
  Vector3 local_F4;
  Script_EVT_GetPosition(CHARA_HIRI, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  Script_SetCharaLookAtPosition(CHARA_AKIR, 2, &local_F4, 2);
  Script_FaceLookAtTargetUtils(CHARA_AKIR, 2, &local_F4, 10);
  return Script_srchControl(0);
}

static int32_t script_CDD4(void) {
  int32_t unused_F4[3];
  Script_SetCharaLookAtPosition(CHARA_AKIR, -98312, 0, 0);
  Script_FaceLookAtTargetUtils(CHARA_AKIR, 0);
  return Script_srchControl(0);
}

static int32_t script_CDFB(void) {
  Vector3 local_F4;
  Script_EVT_GetPosition(CHARA_AKIR, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  (&local_F4)->y = 1.2f;
  Script_SetCharaLookAtPosition(CHARA_GJHM, 1, &local_F4, 0);
  return Script_srchControl(0);
}

static int32_t script_CE31(void) {
  int32_t unused_F4[3];
  Script_SetCharaLookAtPosition(CHARA_GJHM, -98312, 0, 0);
  Script_FaceLookAtTargetUtils(CHARA_GJHM, 0);
  return Script_srchControl(0);
}

static int32_t script_CE58(void) {
  Vector3 local_F4;
  Script_EVT_GetPosition(CHARA_AKIR, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  (&local_F4)->y = 1.2f;
  Script_SetCharaLookAtPosition(CHARA_GJBM, 1, &local_F4, 0);
  return Script_srchControl(0);
}

static int32_t script_CE8E(void) {
  int32_t unused_F4[3];
  Script_SetCharaLookAtPosition(CHARA_GJBM, -98312, 0, 0);
  Script_FaceLookAtTargetUtils(CHARA_GJBM, 0);
  return Script_srchControl(0);
}

static int32_t script_CEB5(void) {
  Vector3 local_F4;
  Script_EVT_GetPosition(CHARA_AKIR, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  (&local_F4)->y = 1.2f;
  Script_SetCharaLookAtPosition(CHARA_HIRI, 1, &local_F4, 0);
  return Script_srchControl(0);
}

static int32_t script_CEEB(void) {
  int32_t unused_F4[3];
  Script_SetCharaLookAtPosition(CHARA_HIRI, -98312, 0, 0);
  Script_FaceLookAtTargetUtils(CHARA_HIRI, 0);
  return Script_srchControl(0);
}

static int32_t script_CF12(void) {
  int16_t local_FE;
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_WALK_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_LineWalkUtils(LNWK_SCRIPT_BEGIN_PATH_WALK, CHARA_AKIR, &data_15450, 0x3F800000, 0);
  while (1) {
    local_FE = Script_LineWalkUtils(LNWK_SCRIPT_SET_MOVE_SCALE_GET_CMD, CHARA_AKIR, 0.04f);
    if (local_FE == -1) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_STAND_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_sprintf("消えるよ1 \012");
  return Script_srchControl(0);
}

static int32_t script_CFB5(void) {
  int16_t local_FE;
  Vector3 local_F0;
  local_F0.x = 2.65f;
  (&local_F0)->y = 0.8f;
  (&local_F0)->z = -8.3f;
  Script_motmSetBlendFrames(CHARA_AKIR, MOTM_BLEND_FRAMES_15);
  Script_PlayCharaMotion(CHARA_AKIR, 4100, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  SCNF_WaitFrames_0000(20);
  Script_RequestFightAnimation(CHARA_AKIR, 4100, 0.0f, 14, 0);
  Script_SetCharaLookAtPosition(CHARA_AKIR, -98312, 0, 0);
  Script_FaceLookAtTargetUtils(CHARA_AKIR, 0);
  SCNF_WaitFrames_0000(11);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_WALK_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  SCNF_WaitFrames_0000(1);
  Script_LineWalkUtils(LNWK_SCRIPT_BEGIN_PATH_WALK, CHARA_AKIR, &data_15480, 0x4000, 64);
  while (1) {
    local_FE = Script_LineWalkUtils(LNWK_SCRIPT_SET_MOVE_SCALE_GET_CMD, CHARA_AKIR, 0.035f);
    if (local_FE == -1) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  SCNF_CHAR_SCNFApplyFacingTowardPoint(CHARA_AKIR, &local_F0, 0, 0.0f, 0, 0);
  SCNF_WaitFrames_0000(2);
  Script_SetRyoMotionDetails(CHARA_AKIR);
  Script_PlayCharaMotion(CHARA_AKIR, 4105, 0.0f, 160.0f, 0.0f, 1.5f, 10);
  Script_sprintf("消えるよ2 \012");
  return Script_srchControl(0);
}

static int32_t script_D110(void) {
  int16_t local_FE;
  uint8_t unused_F0[14];
  Vector3 local_E4;
  Vector3i local_D8;
  local_E4.x = 3.1f;
  (&local_E4)->y = 0.0f;
  (&local_E4)->z = -7.5f;
  Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &local_E4);
  local_D8.x = 0;
  (&local_D8)->y = 0x8000;
  (&local_D8)->z = 0;
  Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &local_D8, 0);
  Script_motmSetBlendFrames(CHARA_AKIR, MOTM_BLEND_FRAMES_1);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_WALK_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_LineWalkUtils(LNWK_SCRIPT_BEGIN_PATH_WALK, CHARA_AKIR, &data_154C0, 0x4000, 64);
  while (1) {
    local_FE = Script_LineWalkUtils(LNWK_SCRIPT_SET_MOVE_SCALE_GET_CMD, CHARA_AKIR, 0.03f);
    if (local_FE == -1) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_motmSetBlendFrames(CHARA_AKIR, MOTM_BLEND_FRAMES_15);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_STAND_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_sprintf(data_154E4);
  return Script_srchControl(0);
}

static int32_t script_D224(void) {
  Vector3 local_F4;
  Vector3 local_E8;
  Vector3i local_DC;
  local_E8.x = 0.36f;
  (&local_E8)->y = 1.0f;
  (&local_E8)->z = 0.46f;
  local_DC.x = 0;
  (&local_DC)->y = 4551;
  (&local_DC)->z = 46421;
  Script_fixoClearAttachment(SHEN_NAME('B_QW'));
  SCNF_WaitFrames_0000(1);
  Script_EVT_GetPosition(CHARA_GJHM, 18, &local_F4, 0);
  Script_EVT_SetPosition(SHEN_NAME('B_QW'), POS_XYZ, &local_F4);
  Script_fixoAttachEx(SHEN_NAME('B_QW'), CHARA_GJHM, 18, &local_E8, &local_DC);
  SCNF_WaitFrames_0000(1);
  return Script_srchControl(0);
}

static int32_t script_D2B7(void) {
  int16_t local_FE;
  uint8_t unused_FC[2];
  Vector3 local_F0;
  int32_t unused_E4[3];
  Vector3i local_D8;
  local_D8.x = 35498;
  (&local_D8)->y = 0;
  (&local_D8)->z = 2366;
  Script_EVT_SetRotation(SHEN_NAME('B_QW'), 0x38000000, &local_D8, 0);
  Script_fixoClearAttachment(SHEN_NAME('B_QW'));
  SCNF_WaitFrames_0000(1);
  local_FE = 0;
  while (local_FE < 150) {
    Script_EVT_GetPosition(CHARA_GJHM, 18, &local_F0, 0);
    local_F0.x = (&local_F0)->x - 1.2f;
    (&local_F0)->y = (&local_F0)->y - 0.39f;
    Script_EVT_SetPosition(SHEN_NAME('B_QW'), POS_XYZ, &local_F0);
    SCNF_WaitFrames_0000(1);
    local_FE = local_FE + 1;
  }
  SCNF_WaitFrames_0000(1);
  return Script_srchControl(0);
}

static int32_t script_D36A(void) {
  Vector3 local_F4;
  Vector3 local_E8;
  Vector3i local_DC;
  local_E8.x = 0.28f;
  (&local_E8)->y = 0.963f;
  (&local_E8)->z = 0.082f;
  local_DC.x = 0;
  (&local_DC)->y = 1274;
  (&local_DC)->z = 47331;
  Script_fixoClearAttachment(SHEN_NAME('B_QW'));
  SCNF_WaitFrames_0000(1);
  Script_EVT_GetPosition(CHARA_GJHM, 18, &local_F4, 0);
  Script_EVT_SetPosition(SHEN_NAME('B_QW'), POS_XYZ, &local_F4);
  Script_fixoAttachEx(SHEN_NAME('B_QW'), CHARA_GJHM, 18, &local_E8, &local_DC);
  SCNF_WaitFrames_0000(1);
  return Script_srchControl(0);
}

static int32_t script_D3FD(void) {
  Vector3 local_F4;
  Vector3 local_E8;
  Vector3i local_DC;
  Vector3 local_D0;
  float local_C4[3];
  float local_B8[3];
  Vector3i local_AC;
  int32_t local_A0[3];
  int32_t local_94[3];
  Vector3 local_88;
  Vector3i local_FF7C;
  int16_t local_FF7A;
  SCNF_WaitFrames_0000(37);
  Script_EVT_GetPosition(SHEN_NAME('B_QW'), -1, &local_D0, 0);
  Script_mtwkGetSetRotation(SHEN_NAME('B_QW'), &local_AC, 0);
  local_C4[0] = 3.7897f;
  local_C4[1] = 1.785f;
  local_C4[2] = -6.4838f;
  local_A0[0] = 16785;
  local_A0[1] = 25526;
  local_A0[2] = 19151;
  Script_fixoClearAttachment(SHEN_NAME('B_QW'));
  local_88.x = (&local_D0)->x;
  (&local_88)->y = (&local_D0)->y;
  (&local_88)->z = (&local_D0)->z;
  local_FF7C.x = (&local_AC)->x;
  (&local_FF7C)->y = (&local_AC)->y;
  (&local_FF7C)->z = (&local_AC)->z;
  local_B8[0] = ((&local_D0)->x - *local_C4) / (float)25;
  local_B8[1] = ((&local_D0)->y - local_C4[1] + 0.3f) / (float)15;
  local_B8[2] = ((&local_D0)->z - local_C4[2]) / (float)25;
  local_94[0] = ((&local_AC)->x - *(int32_t *)local_A0) / 25;
  local_94[1] = ((&local_AC)->y - local_A0[1]) / 25;
  local_94[2] = ((&local_AC)->z - local_A0[2]) / 25;
  local_FF7A = 0;
  while (local_FF7A < 25) {
    local_88.x = (&local_88)->x - *local_B8;
    (&local_88)->z = (&local_88)->z - local_B8[2];
    local_FF7C.x = (&local_FF7C)->x - *(int32_t *)local_94;
    (&local_FF7C)->y = (&local_FF7C)->y - local_94[1];
    (&local_FF7C)->z = (&local_FF7C)->z - local_94[2];
    if ((local_FF7A >= 0) & (local_FF7A <= 7)) {
      (&local_88)->y = (&local_88)->y + 0.0375f;
    }
    if ((local_FF7A >= 8) & (local_FF7A <= 10)) {
      (&local_88)->y = (&local_88)->y;
    }
    if ((local_FF7A >= 11) & (local_FF7A <= 24)) {
      (&local_88)->y = (&local_88)->y - local_B8[1];
    }
    Script_EVT_SetPosition(SHEN_NAME('B_QW'), POS_XYZ, &local_88);
    Script_EVT_SetRotation(SHEN_NAME('B_QW'), 0x38000000, &local_FF7C, 0);
    local_FF7A = local_FF7A + 1;
    SCNF_WaitFrames_0000(1);
  }
  local_E8.x = 0.2f;
  (&local_E8)->y = 0.4f;
  (&local_E8)->z = 0.1f;
  local_DC.x = 0;
  (&local_DC)->y = 1820;
  (&local_DC)->z = 46421;
  Script_EVT_GetPosition(CHARA_AKIR, 12, &local_F4, 0);
  Script_EVT_SetPosition(SHEN_NAME('B_QW'), POS_XYZ, &local_F4);
  Script_fixoAttachEx(SHEN_NAME('B_QW'), CHARA_AKIR, 12, &local_E8, &local_DC);
  SCNF_WaitFrames_0000(1);
  return Script_srchControl(0);
}

static int32_t script_D6B8(void) {
  Vector3 local_F4;
  Vector3 local_E8;
  Vector3i local_DC;
  local_E8.x = 0.37f;
  (&local_E8)->y = 1.15f;
  (&local_E8)->z = 0.21f;
  local_DC.x = 0;
  (&local_DC)->y = 2184;
  (&local_DC)->z = 46603;
  Script_fixoClearAttachment(SHEN_NAME('B_QW'));
  SCNF_WaitFrames_0000(1);
  Script_EVT_GetPosition(CHARA_AKIR, 18, &local_F4, 0);
  Script_EVT_SetPosition(SHEN_NAME('B_QW'), POS_XYZ, &local_F4);
  Script_fixoAttachEx(SHEN_NAME('B_QW'), CHARA_AKIR, 18, &local_E8, &local_DC);
  SCNF_WaitFrames_0000(1);
  return Script_srchControl(0);
}

static void script_D74B(void) {
  Script_fixoClearAttachment(SHEN_NAME('B_QW'));
  Script_EVT_SetPosition(SHEN_NAME('B_QW'), POS_XYZ, &data_1565C);
  Script_charaDisplay(SHEN_NAME('B_QW'), CDM_Hide);
  SCNF_WaitFrames_0000(1);
  return;
}

static int32_t script_D77D(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  int16_t local_E6;
  local_E6 = 0;
  while (local_E6 < 2) {
    Script_InterpolateAndScaleAngles(local_E6, 2, &local_F4, &local_E8, "BDAG");
    local_F4.x = (&local_F4)->x * (float)10;
    (&local_F4)->y = (&local_F4)->y * (float)10;
    (&local_F4)->z = (&local_F4)->z * (float)10;
    Script_charaPosRotUtils(SHEN_NAME('BANK'), *(&data_1443C + local_E6), &local_F4, &local_E8, 0);
    local_E6 = local_E6 + 1;
  }
  return Script_srchControl(0);
}

static void script_D7FC(void) {
  Script_charaDisplay(SHEN_NAME('BANK'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('10BL'), CDM_Show);
  return;
}

static void script_D815(void) {
  Script_charaDisplay(SHEN_NAME('10BL'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('BANK'), CDM_Show);
  return;
}

static void script_D82E(void) {
  Script_charaDisplay(SHEN_NAME('BAL0'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('BAL9'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('10BL'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('BANK'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('B_QW'), CDM_Hide);
  Script_shenClearFreeState(1);
  Script_fixoClearAttachment(SHEN_NAME('B_QW'));
  Script_charaDisplay(CHARA_AKIR, CDM_Hide);
  Script_sicnUtils(0, 1, 1);
  Script_uiUtils(0, 5, 0);
  SCNF_WaitFrames_0000(1);
  SCNF_WaitFrames_0000(1);
  return;
}

static void script_D8A9(void) {
  int32_t unused_FC;
  Script_hmdlApplyScale(SHEN_NAME('B_QW'), &data_15670);
  Script_charaDisplay(SHEN_NAME('B_QW'), CDM_Show);
  Script_charaDisplay(CHARA_AKIR, CDM_Show);
  Script_sicnUtils(2, 1, 0);
  Script_uiUtils(0, 0, 0);
  Script_shenClearFreeState(0);
  return;
}

static int32_t script_D8ED(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  int16_t local_E6;
  int16_t local_E4;
  SCNF_WaitFrames_0000(2);
  Script_charaDisplay(CHARA_GJBM, CDM_Hide);
  local_E4 = 2;
  while (local_E4 < 200) {
    local_E6 = 0;
    while (local_E6 < 10) {
      Script_InterpolateAndScaleAngles(local_E6, local_E4, &local_F4, &local_E8, "BLKS");
      local_F4.x = (&local_F4)->x * (float)10;
      (&local_F4)->y = (&local_F4)->y * (float)10;
      (&local_F4)->z = (&local_F4)->z * (float)10;
      Script_charaPosRotUtils(SHEN_NAME('10BL'), *(&data_14414 + local_E6), &local_F4, &local_E8, 0);
      local_E6 = local_E6 + 1;
    }
    local_E4 = local_E4 + 2;
    SCNF_WaitFrames_0000(1);
  }
  Script_charaDisplay(CHARA_GJBM, CDM_Show);
  return Script_srchControl(0);
}

static void script_D9AE(void) {
  int32_t unused_FC;
  GameDateTime local_F4;
  int16_t local_F2;
  int16_t local_F0;
  Script_ReturnTimeDate(&local_F4);
  local_F2 = (&local_F4)->hours;
  local_F0 = (&local_F4)->minutes;
  if (!local_F2) goto L00D9D0;
  L00D9D0:;
  Script_SetNewDateTime(&local_F4);
  return;
}

static void script_D9D7(void) {
  int32_t local_FC;
  local_FC = 0;
  while (1) {
    if (local_FC <= 16) {
      Script_PrintDebugString(13, 21, "PRESS A BUTTON");
    }
    if (local_FC == 32) {
      local_FC = 0;
    }
    script_A017();
    if (script_9F73(4) == 4) {
      break;
    }
    local_FC = local_FC + 1;
  }
  Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static int32_t script_DA5B(int8_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  Vector3 local_EC;
  Vector3i local_E0;
  int8_t local_DF;
  task_2FB = 0;
  task_2F9 = arg0;
  local_DF = 0;
  Script_shenClearFreeState(1);
  script_10A5A();
  Script_EVT_GetPosition(SHEN_NAME('B_QW'), -1, &local_EC, ENTITY_XFORM_WORLD_SPACE);
  Script_mtwkGetSetRotation(SHEN_NAME('B_QW'), &local_E0, 0x40000000);
  if (task_2F9 == 1) {
    local_FC = Script_scenCreateTask(script_E100, 0);
    script_DBEF(local_DF);
    Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
    while (Script_filmCheckOrSetTransitionUtil(-1)) {
      SCNF_WaitFrames_0000(1);
    }
    script_DDD2();
    Script_srchControl((void *)local_FC);
  } else {
    task_248 = 0;
    local_F8 = Script_scenCreateTask(script_105B5, 0);
    local_DF = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 4.0f));
    for (;;) {
      task_2FB = 0;
      task_248 = task_248 + 1;
      local_FC = Script_scenCreateTask(script_E100, 0);
      script_DBEF(local_DF);
      Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
      while (Script_filmCheckOrSetTransitionUtil(-1)) {
        SCNF_WaitFrames_0000(1);
      }
      script_DDD2();
      Script_srchControl((void *)local_FC);
      if ((task_2FB == 2) | (task_2FB == 3) | (task_2FB == 4)) {
        break;
      } else {
        SCNF_WaitFrames_0000(60);
      }
    }
    Script_srchControl((void *)local_F8);
  }
  if (task_2FB != 3) {
    Script_EVT_SetPosition(SHEN_NAME('B_QW'), 0x78000000, &local_EC);
    Script_EVT_SetRotation(SHEN_NAME('B_QW'), 0x78000000, &local_E0, 0);
  }
  script_10AC8();
  return task_2FB;
}

static void script_DBEF(int32_t arg0) {
  Vector3 local_F4;
  Vector3 local_E8;
  if (arg0 == 3) {
    script_10949(&local_E8, 2.3334f, 0.78f, -9.209f);
  } else if (arg0 == 2) {
    script_10949(&local_E8, 1.9684f, 0.78f, -9.262f);
  } else if (arg0 == 1) {
    script_10949(&local_E8, 2.3694f, 0.78f, -8.867f);
  } else {
    script_10949(&local_E8, 2.2394f, 0.78f, -9.2f);
  }
  script_10949(&local_F4, 2.39f, 0.78f, -8.306f);
  Script_EVT_SetPosition(SHEN_NAME('BAL9'), POS_XYZ, &local_E8);
  Script_EVT_SetPosition(SHEN_NAME('BAL0'), POS_XYZ, &local_F4);
  Script_MoveObjectToObject(SHEN_NAME('B_QW'), 0x304C4142);
  Script_EVT_SetRotation(SHEN_NAME('B_QW'), 0x38000000, &data_1576C, 0);
  Script_EVT_SetPosition(SHEN_NAME('B_QW'), POS_XYZ, &data_15760);
  task_2F0 = 1;
  task_2F1 = 1;
  task_2F2 = 1;
  task_2F3 = 1;
  Script_EVT_SetFacingTowardPoint(SHEN_NAME('BAL0'), 0x38000000, &local_E8, 0);
  Script_charaDisplay(SHEN_NAME('BAL0'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('BAL9'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('B_QW'), CDM_Show);
  Script_charaSetRenderOpacity(SHEN_NAME('BAL0'), 1.0f);
  Script_charaSetRenderOpacity(SHEN_NAME('B_QW'), 1.0f);
  task_2FA = 1;
  script_10949(&task_2D8, 0.0f, 0.0f, 0.0f);
  script_10949(&task_2E4, 0.0f, 0.0f, 0.0f);
  task_2FC = 0;
  return;
}

__attribute__((scn_string_bindings(0, data_15778)))
static void script_DD7F(void) {
  int32_t local_FC;
  local_FC = Script_ShowUIMessageFromID("E1039B036");
  script_10814((const void *)local_FC);
  return;
}

__attribute__((scn_string_bindings(0, data_15782, 1, data_1578C)))
static void script_DD99(void) {
  {
    int32_t local_FC;
    local_FC = Script_ShowUIMessageFromID("E1039A015");
    script_10814((const void *)local_FC);
  }
  SCNF_WaitFrames_0000(15);
  {
    int32_t local_FC;
    local_FC = Script_ShowUIMessageFromID("E1039B038");
    script_10814((const void *)local_FC);
  }
  return;
}

static void script_DDD2(void) {
  script_DDDA();
  return;
}

__attribute__((scn_string_bindings(0, data_157B0, 1, data_157B8)))
static void script_DDDA(void) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  int32_t local_F0;
  int16_t local_EE;
  uint8_t unused_EC[2];
  float local_E8;
  Vector3i local_DC;
  Vector3 local_D0;
  int8_t local_CF;
  int8_t local_CE;
  uint8_t unused_C8[6];
  GameDateTime local_C0;
  Vector3i local_B4;
  Script_mtwkGetSetRotation(SHEN_NAME('BAL0'), &local_DC, 0);
  local_EE = 0;
  local_E8 = 0.0f;
  task_2F8 = 65;
  local_CF = 0;
  local_CE = 1;
  local_F4 = -1;
  local_FC = -1;
  local_F8 = -1;
  Script_EVT_GetPosition(SHEN_NAME('BAL9'), -1, &local_D0, 0);
  for (;;) {
    if ((task_2FB != 6) & (task_2FB != 4)) {
      if (Script_GetControllerInput(2) == 0x8000) {
        if ((local_CF == 0) | (local_CF == 1)) {
          if (!(local_E8 <= -48.0f)) {
            local_EE = local_EE - 9;
            local_E8 = local_E8 - 0.05f;
            Script_sprintf("ang %f\012", local_E8);
          }
        }
      }
      if (Script_GetControllerInput(2) == 0x4000) {
        if ((local_CF == 0) | (local_CF == 1)) {
          if (!(local_E8 >= 48.0f)) {
            local_EE = local_EE + 9;
            local_E8 = local_E8 + 0.05f;
            Script_sprintf("ang %f\012", local_E8);
          }
        }
      }
      if (Script_GetControllerInput(1) == 0x200) {
        if (task_2F9 == 0) {
          if (local_CE == 1) {
            script_DD7F();
            local_F0 = SCNF_RequestActionSelectorFromParams_0008(0xFFFF, 37, 34, 0xFFFF, 600, 0xFFFF, ACTION_SELECTOR_MODE_STANDARD_TIMEOUT_CANCEL);
            if (local_F0 == 37) {
              script_DD99();
            } else {
              task_2FB = 4;
              task_2FC = 1;
              break;
            }
          }
        }
        if (local_CF == 1) {
          task_2FA = 1;
          local_CF = 0;
          local_CE = 1;
        }
      }
      if (Script_GetControllerInput(1) == 0x400) {
        switch (local_CF) {
          case 0:
            local_CF = 1;
            local_CE = 0;
            task_2FA = 3;
            break;
          case 1:
            task_2FA = 1;
            script_E522((&local_DC)->y + local_EE, task_2F8);
            local_FC = Script_scenCreateTask(script_E386, 0);
            local_F8 = Script_scenCreateTask(script_10667, 0);
            local_CF = 2;
            local_CE = 0;
            Script_EVT_SetPosition(SHEN_NAME('B_QW'), POS_XYZ, &data_15798);
            Script_MoveObjectToObject(SHEN_NAME('B_QW'), -1);
            local_F4 = Script_scenCreateTask(script_1075E, 0);
            task_2FB = 5;
            goto L00E0BA;
            break;
        }
      }
    }
    Script_ReturnTimeDate(&local_C0);
    if (task_2F9 == 1) {
      if (((&local_C0)->hours >= 4) & ((&local_C0)->hours <= 17)) {
        task_2FB = 3;
        task_2FC = 1;
        break;
      }
    } else {
      if ((&local_C0)->hours >= 23) {
        if ((&local_C0)->minutes >= 30) {
          task_2FB = 3;
          task_2FC = 1;
          break;
        }
      }
    }
    if (task_2F9 == 0) {
      if (task_2FC == 1) {
        break;
      }
    }
    (&local_B4)->y = (&local_DC)->y + local_EE;
    Script_EVT_SetRotation(SHEN_NAME('BAL0'), MATRIX_XFORM_Y, &local_B4, 0);
    SCNF_WaitFrames_0000(1);
  }
  L00E0BA:;
  for (;;) {
    if (task_2FC == 1) {
      if (local_F4 != -1) {
        Script_srchControl((void *)local_F4);
      }
      if (local_F8 != -1) {
        Script_srchControl((void *)local_F8);
      }
      if (local_FC != -1) {
        Script_srchControl((void *)local_FC);
      }
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static int32_t script_E100(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t local_E4;
  for (;;) {
    local_E4 = task_2FA;
    switch (local_E4) {
      case 1:
        script_107FF(200, SHEN_NAME('BLDM'), 0);
        Script_charaSetRenderOpacity(SHEN_NAME('BAL0'), 1.0f);
        Script_charaSetRenderOpacity(SHEN_NAME('B_QW'), 1.0f);
        script_10949(&local_F4, 0.0f, 0.02f, 0.05f);
        script_10991(&local_E8, 0.0f, 275.0f, 90.0f);
        if (Script_CEWPRelated(SHEN_NAME('B_QW')) != -1) {
          Script_EVT_SetPosition(SHEN_NAME('B_QW'), POS_XYZ, &local_F4);
          Script_EVT_SetRotation(SHEN_NAME('B_QW'), 0x38000000, &local_E8, 0);
        }
        task_2FA = 0;
        break;
      case 3:
        script_107FF(210, SHEN_NAME('BAL0'), 0);
        Script_charaSetRenderOpacity(SHEN_NAME('BAL0'), 0.5f);
        Script_charaSetRenderOpacity(SHEN_NAME('B_QW'), 0.7f);
        script_10949(&local_F4, 0.0f, 0.0f, 0.2f);
        script_10991(&local_E8, 0.0f, 270.0f, 0.0f);
        if (Script_CEWPRelated(SHEN_NAME('B_QW')) != -1) {
          Script_EVT_SetPosition(SHEN_NAME('B_QW'), POS_XYZ, &local_F4);
          Script_EVT_SetRotation(SHEN_NAME('B_QW'), 0x38000000, &local_E8, 0);
        }
        break;
      case 2:
        script_107FF(200, SHEN_NAME('BLDM'), 0);
        Script_charaSetRenderOpacity(SHEN_NAME('BAL0'), 1.0f);
        task_2FA = 0;
        break;
      case 4:
        script_107FF(220, SHEN_NAME('BLDM'), 0);
        task_2FA = 0;
        break;
      case 5:
        script_107FF(230, SHEN_NAME('BLDM'), 0);
        task_2FA = 0;
        break;
      case 6:
        script_107FF(240, SHEN_NAME('BLDM'), 0);
        task_2FA = 0;
        break;
      case 7:
        script_107FF(250, SHEN_NAME('BLDM'), 0);
        task_2FA = 0;
        break;
      case 8:
        script_107FF(260, SHEN_NAME('BLDM'), 0);
        task_2FA = 0;
        break;
      case 9:
        script_107FF(270, SHEN_NAME('BLDM'), 0);
        task_2FA = 0;
        break;
      case 10:
        Script_charaSetRenderOpacity(SHEN_NAME('BAL0'), 1.0f);
        script_107FF(280, SHEN_NAME('BLDM'), 0);
        task_2FA = 0;
        break;
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_E386(void) {
  Vector3 local_F4;
  int8_t local_F3;
  int8_t local_F2;
  int32_t local_EC;
  local_F3 = 0;
  local_F2 = 0;
  for (;;) {
    Script_EVT_GetPosition(SHEN_NAME('BAL9'), -1, &local_F4, 0);
    for (;;) {
      if (!(((&local_F4)->x - *(float *)(&data_15730 + local_F3)) * ((&local_F4)->x - *(float *)(&data_15730 + local_F3)) + ((&local_F4)->z - *(float *)(&data_15748 + local_F3)) * ((&local_F4)->z - *(float *)(&data_15748 + local_F3)) > 0.160000011f)) {
        local_EC = local_F3;
        switch (local_EC) {
          case 0:
            task_2FA = 4;
            SCNF_WaitFrames_0000(60);
            local_F2 = 1;
            goto L00E516;
            break;
          case 1:
            task_2FA = 5;
            SCNF_WaitFrames_0000(60);
            local_F2 = 1;
            goto L00E516;
            break;
          case 2:
            task_2FA = 6;
            SCNF_WaitFrames_0000(60);
            local_F2 = 1;
            goto L00E516;
            break;
          case 3:
            task_2FA = 7;
            SCNF_WaitFrames_0000(60);
            local_F2 = 1;
            goto L00E516;
            break;
          case 4:
            task_2FA = 8;
            SCNF_WaitFrames_0000(60);
            local_F2 = 1;
            goto L00E516;
            break;
          case 5:
            task_2FA = 9;
            SCNF_WaitFrames_0000(60);
            local_F2 = 1;
            goto L00E516;
            break;
        }
      }
      if (!((&local_F4)->x <= 3.43f)) {
        task_2FA = 10;
      } else {
        task_2FA = 2;
      }
      local_F2 = 0;
      local_F3 = local_F3 + 1;
      if (local_F3 >= 6) {
        local_F3 = 0;
        break;
      }
    }
    L00E516:;
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_E522(int32_t arg0, int32_t arg1) {
  task_2D8.x = Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, arg0) * (float)arg1 * (float)-1 / (float)1000;
  task_2D8.y = 0.0f;
  task_2D8.z = Script_MathUtils(SCRIPT_MATH_COS_FIXED16, arg0) * (float)arg1 * (float)-1 / (float)1000;
  Script_executeAicaCommand(1449, 0, 0);
  Script_scenCreateTask(script_E583, 1, 0x304C4142);
  return;
}

static int32_t script_E583(int32_t arg0) {
  float local_FC;
  float local_F8;
  float local_F4;
  float local_F0;
  int8_t local_EF;
  int8_t local_EE;
  Vector3 local_E0;
  Vector3 local_D4;
  Vector3i local_C8;
  Vector3i local_BC;
  local_FC = 0.985f;
  local_F8 = 0.985f;
  script_10991(&local_C8, 0.0f, 0.0f, 0.0f);
  script_10991(&local_BC, 0.0f, 0.0f, 0.0f);
  for (;;) {
    if (!(Script_MathUtils(SCRIPT_MATH_VEC2_LENGTH, (&task_2D8)->x, task_2D8.z) <= 1.00000004e-36f)) {
      task_2F2 = 2;
    }
    if (!(Script_MathUtils(SCRIPT_MATH_VEC2_LENGTH, (&task_2E4)->x, task_2E4.z) <= 1.00000004e-36f)) {
      task_2F3 = 2;
    }
    script_FF02(SHEN_NAME('BAL9'));
    script_FF02(SHEN_NAME('BAL0'));
    local_EF = script_F0A6(SHEN_NAME('BAL0'));
    local_EE = script_F0A6(SHEN_NAME('BAL9'));
    if ((local_EF == 0) & (local_EE == 0)) {
      script_E9BE(SHEN_NAME('BAL0'), SHEN_NAME('BAL9'));
    }
    local_F4 = Script_MathUtils(SCRIPT_MATH_VEC2_LENGTH, (&task_2D8)->x, task_2D8.z);
    if (!(local_F4 <= -1.00000004e-36f) & !(local_F4 >= 1.00000004e-36f)) {
    } else {
      Script_EVT_GetPosition(SHEN_NAME('BAL0'), -1, &local_E0, 0);
      script_10964(&local_E0, (&task_2D8)->x, task_2D8.y, task_2D8.z);
      Script_EVT_SetFacingTowardPoint(SHEN_NAME('BAL0'), 0x38000000, &local_E0, 0);
      script_109D6(&local_C8, -(int32_t)(local_F4 * 360.0f / 0.1884f / 360.0f * (float)0x10000), 0, 0);
      Script_EVT_SetRotation(SHEN_NAME('BAL0'), -0x48000000, &local_C8, 0);
    }
    local_F0 = Script_MathUtils(SCRIPT_MATH_VEC2_LENGTH, (&task_2E4)->x, task_2E4.z);
    if (!(local_F0 <= -1.00000004e-36f) & !(local_F0 >= 1.00000004e-36f)) {
    } else {
      Script_EVT_GetPosition(SHEN_NAME('BAL9'), -1, &local_D4, 0);
      script_10964(&local_D4, (&task_2E4)->x, task_2E4.y, task_2E4.z);
      Script_EVT_SetFacingTowardPoint(SHEN_NAME('BAL9'), 0x38000000, &local_D4, 0);
      script_109D6(&local_BC, -(int32_t)(local_F0 * 360.0f / 0.1884f / 360.0f * (float)0x10000), 0, 0);
      Script_EVT_SetRotation(SHEN_NAME('BAL9'), -0x48000000, &local_BC, 0);
    }
    if (!(Script_MathUtils(SCRIPT_MATH_VEC2_LENGTH, (&task_2D8)->x, task_2D8.z) <= 0.01f)) {
      local_FC = 0.985f;
    }
    if (!(Script_MathUtils(SCRIPT_MATH_VEC2_LENGTH, (&task_2D8)->x, task_2D8.z) <= 0.001f)) {
      task_2D8.x = (&task_2D8)->x * local_FC;
      task_2D8.y = 0.0f;
      task_2D8.z = task_2D8.z * local_FC;
    } else {
      task_2D8.x = 0.0f;
      task_2D8.y = 0.0f;
      task_2D8.z = 0.0f;
      if ((task_2F2 == 2) & (task_2F0 == 1)) {
        task_2F2 = 3;
      }
    }
    if (!(Script_MathUtils(SCRIPT_MATH_VEC2_LENGTH, (&task_2E4)->x, task_2E4.z) <= 0.01f)) {
      local_F8 = 0.985f;
    }
    if (!(Script_MathUtils(SCRIPT_MATH_VEC2_LENGTH, (&task_2E4)->x, task_2E4.z) <= 0.001f)) {
      task_2E4.x = (&task_2E4)->x * local_F8;
      task_2E4.y = 0.0f;
      task_2E4.z = task_2E4.z * local_F8;
    } else {
      task_2E4.x = 0.0f;
      task_2E4.y = 0.0f;
      task_2E4.z = 0.0f;
      if ((task_2F3 == 2) & (task_2F1 == 1)) {
        task_2F3 = 3;
      }
    }
    if (task_2FC == 1) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  return Script_srchControl(0);
}

static void script_E902(int32_t arg0, int32_t arg1) {
  Vector3 local_F4;
  Vector3 local_E8;
  switch (arg0) {
    case 0x304C4142:
      script_10949(&local_E8, (&task_2D8)->x, task_2D8.y, task_2D8.z);
      break;
    case 0x394C4142:
      script_10949(&local_E8, (&task_2E4)->x, task_2E4.y, task_2E4.z);
      break;
  }
  Script_EVT_GetPosition(arg0, -1, &local_F4, 0);
  local_E8.x = (&local_E8)->x - (float)arg1 * 1.0f;
  (&local_E8)->y = 0.0f;
  (&local_E8)->z = (&local_E8)->z - (float)arg1 * 1.0f;
  script_10964(&local_E8, (&local_F4)->x, (&local_F4)->y, (&local_F4)->z);
  Script_EVT_SetPosition(arg0, POS_XYZ, &local_E8);
  (void)1;
  return;
}

static void script_E9BE(int32_t arg0, int32_t arg1) {
  Vector3 local_F4;
  Vector3 local_E8;
  Vector3 local_DC;
  Vector3 local_D0;
  Vector3 local_C4;
  Vector3 local_B8;
  float local_B4;
  float local_B0;
  float local_AC;
  float local_A8;
  float local_A4;
  int8_t local_A3;
  (&local_F4)->y = 0.0f;
  (&local_E8)->y = 0.0f;
  (&local_DC)->y = 0.0f;
  (&local_D0)->y = 0.0f;
  (&local_C4)->y = 0.0f;
  (&local_B8)->y = 0.0f;
  switch (arg0) {
    case 0x304C4142:
      script_10949(&local_DC, (&task_2D8)->x, task_2D8.y, task_2D8.z);
      break;
    case 0x394C4142:
      script_10949(&local_DC, (&task_2E4)->x, task_2E4.y, task_2E4.z);
      break;
  }
  switch (arg1) {
    case 0x304C4142:
      script_10949(&local_D0, (&task_2D8)->x, task_2D8.y, task_2D8.z);
      break;
    case 0x394C4142:
      script_10949(&local_D0, (&task_2E4)->x, task_2E4.y, task_2E4.z);
      break;
  }
  Script_EVT_GetPosition(arg0, -1, &local_F4, 0);
  Script_EVT_GetPosition(arg1, -1, &local_E8, 0);
  local_B4 = 0.0f;
  for (;;) {
    local_A8 = local_B4 * (&local_DC)->x + (&local_F4)->x - (local_B4 * (&local_D0)->x + (&local_E8)->x);
    local_A4 = local_B4 * (&local_DC)->z + (&local_F4)->z - (local_B4 * (&local_D0)->z + (&local_E8)->z);
    local_B4 = local_B4 + 0.01f;
    local_B0 = local_B4 * (&local_DC)->x + (&local_F4)->x - (local_B4 * (&local_D0)->x + (&local_E8)->x);
    local_AC = local_B4 * (&local_DC)->z + (&local_F4)->z - (local_B4 * (&local_D0)->z + (&local_E8)->z);
    if (!(Script_MathUtils(SCRIPT_MATH_VEC2_LENGTH, local_B0, local_AC) > 0.06f)) {
      local_A3 = 1;
      if (!(0.06f - Script_MathUtils(SCRIPT_MATH_VEC2_LENGTH, local_B0, local_AC) <= Script_MathUtils(SCRIPT_MATH_VEC2_LENGTH, local_A8, local_A4) - 0.06f)) {
        local_B4 = local_B4 - 0.01f;
      }
      break;
    } else {
      if (!(local_B4 < 1.0f)) {
        local_A3 = 0;
        local_B4 = 1.0f;
        break;
        goto L00EBB9;
      }
    }
    L00EBB9:;
  }
  if (!(local_B4 < 1.0f)) {
    local_B4 = 1.0f;
  }
  local_C4.x = local_B4 * (&local_DC)->x + (&local_F4)->x;
  (&local_C4)->y = (&local_F4)->y;
  (&local_C4)->z = local_B4 * (&local_DC)->z + (&local_F4)->z;
  local_B8.x = local_B4 * (&local_D0)->x + (&local_E8)->x;
  (&local_B8)->y = (&local_E8)->y;
  (&local_B8)->z = local_B4 * (&local_D0)->z + (&local_E8)->z;
  Script_EVT_SetPosition(arg0, POS_XYZ, &local_C4);
  Script_EVT_SetPosition(arg1, POS_XYZ, &local_B8);
  if (local_A3 == 1) {
    SCNF_WaitFrames_0000(1);
    script_ED66(arg0, arg1, &local_C4, &local_B8, &local_DC);
    script_EF06(arg1, arg0, &local_B8, &local_C4, &local_D0);
    local_C4.x = (1.0f - local_B4) * (&task_2D8)->x + (&local_C4)->x;
    (&local_C4)->y = (&local_C4)->y;
    (&local_C4)->z = (1.0f - local_B4) * task_2D8.z + (&local_C4)->z;
    local_B8.x = (1.0f - local_B4) * (&task_2E4)->x + (&local_B8)->x;
    (&local_B8)->y = (&local_B8)->y;
    (&local_B8)->z = (1.0f - local_B4) * task_2E4.z + (&local_B8)->z;
    Script_EVT_SetPosition(arg0, POS_XYZ, &local_C4);
    Script_EVT_SetPosition(arg1, POS_XYZ, &local_B8);
    if (!(Script_MathUtils(SCRIPT_MATH_VEC2_LENGTH, (&local_DC)->x, (&local_DC)->z) <= 0.01f)) {
      Script_executeAicaCommand(198057, 0, 0);
    } else {
      Script_executeAicaCommand(263593, 0, 0);
    }
    script_F0A6(arg0);
    script_F0A6(arg1);
  }
  return;
}

static void script_ED66(int32_t arg0, int32_t arg1, const Vector3 *arg2, const Vector3 *arg3, const Vector3 *arg4) {
  float local_F4[3];
  float local_E8[3];
  float local_DC[3];
  float local_D8;
  local_E8[1] = 0.0f;
  local_DC[1] = 0.0f;
  local_F4[0] = (arg3->x - arg2->x) / Script_MathUtils(SCRIPT_MATH_VEC2_LENGTH, arg3->x - arg2->x, arg3->z - arg2->z);
  local_F4[1] = 0.0f;
  local_F4[2] = (arg3->z - arg2->z) / Script_MathUtils(SCRIPT_MATH_VEC2_LENGTH, arg3->x - arg2->x, arg3->z - arg2->z);
  local_D8 = ((arg3->x - arg2->x) * arg4->x + arg4->z * (arg3->z - arg2->z)) / Script_MathUtils(SCRIPT_MATH_VEC2_LENGTH, arg3->x - arg2->x, arg3->z - arg2->z);
  local_DC[0] = local_D8 * *local_F4;
  local_DC[1] = 0.0f;
  local_DC[2] = local_D8 * local_F4[2];
  local_E8[0] = arg4->x - *local_DC;
  local_E8[1] = 0.0f;
  local_E8[2] = arg4->z - local_DC[2];
  switch (arg0) {
    case 0x304C4142:
      script_10949(&task_2D8, *local_E8, local_E8[1], local_E8[2]);
      break;
    case 0x394C4142:
      script_10949(&task_2E4, *local_E8, local_E8[1], local_E8[2]);
      break;
  }
  switch (arg1) {
    case 0x304C4142:
      script_10949(&task_2D8, *local_DC, local_DC[1], local_DC[2]);
      break;
    case 0x394C4142:
      script_10949(&task_2E4, *local_DC, local_DC[1], local_DC[2]);
      break;
  }
  return;
}

static void script_EF06(int32_t arg0, int32_t arg1, const Vector3 *arg2, const Vector3 *arg3, const Vector3 *arg4) {
  float local_F4[3];
  float local_E8[3];
  float local_DC[3];
  float local_D8;
  local_E8[1] = 0.0f;
  local_DC[1] = 0.0f;
  local_F4[0] = (arg3->x - arg2->x) / Script_MathUtils(SCRIPT_MATH_VEC2_LENGTH, arg3->x - arg2->x, arg3->z - arg2->z);
  local_F4[1] = 0.0f;
  local_F4[2] = (arg3->z - arg2->z) / Script_MathUtils(SCRIPT_MATH_VEC2_LENGTH, arg3->x - arg2->x, arg3->z - arg2->z);
  local_D8 = ((arg3->x - arg2->x) * arg4->x + arg4->z * (arg3->z - arg2->z)) / Script_MathUtils(SCRIPT_MATH_VEC2_LENGTH, arg3->x - arg2->x, arg3->z - arg2->z);
  local_DC[0] = local_D8 * *local_F4;
  local_DC[1] = 0.0f;
  local_DC[2] = local_D8 * local_F4[2];
  local_E8[0] = arg4->x - *local_DC;
  local_E8[1] = 0.0f;
  local_E8[2] = arg4->z - local_DC[2];
  switch (arg0) {
    case 0x304C4142:
      script_10964(&task_2D8, *local_E8, local_E8[1], local_E8[2]);
      break;
    case 0x394C4142:
      script_10964(&task_2E4, *local_E8, local_E8[1], local_E8[2]);
      break;
  }
  switch (arg1) {
    case 0x304C4142:
      script_10964(&task_2D8, *local_DC, local_DC[1], local_DC[2]);
      break;
    case 0x394C4142:
      script_10964(&task_2E4, *local_DC, local_DC[1], local_DC[2]);
      break;
  }
  return;
}

static int8_t script_F0A6(int32_t arg0) {
  Vector3 local_F4;
  float local_E8[3];
  int32_t unused_DC[3];
  Vector3 local_D0;
  Vector3 local_C4;
  float local_C0;
  float local_BC;
  int32_t unused_B4[2];
  float local_B0;
  float local_AC;
  float local_A8;
  float local_A4;
  float local_A0;
  int32_t unused_98[2];
  Vector3 local_8C;
  float local_88;
  float local_84;
  float local_80;
  int8_t local_FF7F;
  uint8_t unused_FF7E;
  int8_t local_FF7D;
  int8_t local_FF7C;
  int8_t local_FF7B;
  int8_t local_FF7A;
  uint8_t unused_FF78[2];
  (&local_D0)->y = 0.0f;
  (&local_F4)->y = 0.0f;
  (&local_C4)->y = 0.0f;
  local_E8[1] = 0.0f;
  local_FF7F = 0;
  local_A4 = 0.0f;
  local_FF7D = 0;
  local_FF7C = 0;
  local_FF7B = 0;
  local_FF7A = 0;
  switch (arg0) {
    case 0x304C4142:
      script_10949(&local_F4, (&task_2D8)->x, task_2D8.y, task_2D8.z);
      break;
    case 0x394C4142:
      script_10949(&local_F4, (&task_2E4)->x, task_2E4.y, task_2E4.z);
      break;
  }
  Script_EVT_GetPosition(arg0, -1, &local_D0, 0);
  if (!((&local_D0)->x >= (&local_D0)->x + (&local_F4)->x)) {
    local_8C.x = (&local_D0)->x;
    local_88 = (&local_D0)->x + (&local_F4)->x;
  } else {
    local_88 = (&local_D0)->x;
    local_8C.x = (&local_D0)->x + (&local_F4)->x;
  }
  if (!((&local_D0)->z >= (&local_D0)->z + (&local_F4)->z)) {
    local_84 = (&local_D0)->z;
    local_80 = (&local_D0)->z + (&local_F4)->z;
  } else {
    local_80 = (&local_D0)->z;
    local_84 = (&local_D0)->z + (&local_F4)->z;
  }
  if (!((&local_F4)->x <= -1.00000004e-36f) & !((&local_F4)->x >= 1.00000004e-36f)) {
    local_B0 = 0.0f;
    local_AC = (&local_D0)->x;
  } else {
    local_B0 = (&local_F4)->z / (&local_F4)->x;
    local_AC = (&local_D0)->z - local_B0 * (&local_D0)->x;
    local_A8 = Script_MathUtils(SCRIPT_MATH_VEC2_LENGTH, (&local_F4)->x, (&local_F4)->z);
  }
  local_FF7F = 0;
  for (;;) {
    if (!(local_B0 <= -1.00000004e-36f) & !(local_B0 >= 1.00000004e-36f)) {
      local_C0 = local_AC;
    } else {
      local_C0 = (*(float *)(&data_15690 + local_FF7F) - local_AC) / local_B0;
    }
    local_BC = *(float *)(&data_15690 + local_FF7F);
    if (!(local_84 > local_BC) & !(local_BC > local_80)) {
      if (!(*(float *)(&data_156A8 + local_FF7F) > local_C0) & !(local_C0 > *(float *)(&data_156B8 + local_FF7F))) {
        local_E8[0] = (&local_F4)->x * (1.0f - Script_MathUtils(SCRIPT_MATH_INT_TO_FLOAT, task_2F8) / 500.0f);
        local_E8[1] = 0.0f;
        local_E8[2] = (float)0 - (&local_F4)->z;
        switch (arg0) {
          case 0x304C4142:
            script_10949(&task_2D8, *local_E8, local_E8[1], local_E8[2]);
            break;
          case 0x394C4142:
            script_10949(&task_2E4, *local_E8, local_E8[1], local_E8[2]);
            break;
        }
        local_FF7B = 1;
        local_FF7A = local_FF7F;
        local_FF7D = 1;
        break;
      }
    }
    local_FF7F = local_FF7F + 1;
    if (local_FF7F >= 4) {
      break;
    }
  }
  local_FF7F = 0;
  for (;;) {
    if (local_FF7D == 0) {
      local_C0 = *(float *)(&data_156A0 + local_FF7F);
      local_BC = *(float *)(&data_156A0 + local_FF7F) * local_B0 + local_AC;
      if (!(local_8C.x > local_C0) & !(local_C0 > local_88)) {
        if (!(*(float *)&data_156C8 > local_BC) & !(local_BC > *(float *)(&data_156C8 + 1))) {
          local_E8[0] = (float)0 - (&local_F4)->x;
          local_E8[1] = 0.0f;
          local_E8[2] = (&local_F4)->z * (1.0f - Script_MathUtils(SCRIPT_MATH_INT_TO_FLOAT, task_2F8) / 500.0f);
          switch (arg0) {
            case 0x304C4142:
              script_10949(&task_2D8, *local_E8, local_E8[1], local_E8[2]);
              break;
            case 0x394C4142:
              script_10949(&task_2E4, *local_E8, local_E8[1], local_E8[2]);
              break;
          }
          local_FF7B = 2;
          local_FF7A = local_FF7F;
          local_FF7D = 1;
          break;
        }
      }
    }
    local_FF7F = local_FF7F + 1;
    if (local_FF7F >= 2) {
      break;
    }
  }
  local_FF7F = 0;
  if (local_FF7D == 0) {
    for (;;) {
      if (!(local_B0 - *(float *)(&data_156D0 + local_FF7F) <= -1.00000004e-36f) & !(local_B0 - *(float *)(&data_156D0 + local_FF7F) >= 1.00000004e-36f)) {
      } else {
        local_C0 = (*(float *)(&data_15700 + local_FF7F) - local_AC) / (local_B0 - *(float *)(&data_156D0 + local_FF7F));
        local_BC = local_C0 * local_B0 + local_AC;
        if (-!(*(float *)&data_156A0 >= local_C0) & -!(local_C0 >= *(float *)(&data_156A0 + 1)) & -!(*(float *)&data_15690 >= local_BC) & -!(local_BC >= *(float *)(&data_15690 + 2))) {
        } else {
          if (-!(local_C0 <= 1.63599992f) & -!(local_C0 >= 4.65599966f) & -!(local_BC <= -9.471001f) & -!(local_BC >= -7.951f)) {
            if (-!(local_8C.x > local_C0) & -!(local_C0 > local_88) & -!(local_84 > local_BC) & -!(local_BC > local_80)) {
              local_A0 = (*(float *)(&data_156D0 + local_FF7F) * (&local_F4)->x - (&local_F4)->z) * (float)-1 / (*(float *)(&data_156D0 + local_FF7F) * *(float *)(&data_156D0 + local_FF7F) + 1.0f);
              local_E8[0] = (&local_F4)->x + *(float *)(&data_156D0 + local_FF7F) * local_A0 * (float)2;
              local_E8[1] = 0.0f;
              local_E8[2] = (&local_F4)->z - local_A0 * (float)2;
              switch (arg0) {
                case 0x304C4142:
                  script_10949(&task_2D8, *local_E8, local_E8[1], local_E8[2]);
                  break;
                case 0x394C4142:
                  script_10949(&task_2E4, *local_E8, local_E8[1], local_E8[2]);
                  break;
              }
              local_FF7B = 3;
              local_FF7A = local_FF7F;
              local_FF7D = 1;
              break;
            }
            goto L00F665;
          }
        }
      }
      L00F665:;
      local_FF7F = local_FF7F + 1;
      if (local_FF7F >= 12) {
        break;
      }
    }
  }
  if (local_FF7D == 1) {
    if (!((&local_F4)->x <= -1.00000004e-36f) & !((&local_F4)->x >= 1.00000004e-36f)) {
      local_A4 = Script_MathUtils(SCRIPT_MATH_ABS, (local_BC - (&local_D0)->z) / (&local_F4)->z);
    } else {
      local_A4 = Script_MathUtils(SCRIPT_MATH_ABS, (local_C0 - (&local_D0)->x) / (&local_F4)->x);
    }
    local_C4.x = local_A4 * (&local_F4)->x + (&local_D0)->x;
    (&local_C4)->y = (&local_D0)->y;
    (&local_C4)->z = local_A4 * (&local_F4)->z + (&local_D0)->z;
    Script_EVT_SetPosition(arg0, POS_XYZ, &local_C4);
    if (!(Script_MathUtils(SCRIPT_MATH_VEC2_LENGTH, (&local_F4)->x, (&local_F4)->z) <= 0.01f)) {
      Script_executeAicaCommand(329129, 0, 0);
    } else {
      if (!(Script_MathUtils(SCRIPT_MATH_VEC2_LENGTH, (&local_F4)->x, (&local_F4)->z) <= 0.0015f)) {
        Script_executeAicaCommand(394665, 0, 0);
        goto L00F767;
      }
    }
    L00F767:;
    local_C4.x = *local_E8 * (1.0f - local_A4) + local_C0;
    (&local_C4)->y = (&local_D0)->y;
    (&local_C4)->z = local_E8[2] * (1.0f - local_A4) + local_BC;
    if (script_F892(arg0, *local_E8 * (1.0f - local_A4), local_E8[2] * (1.0f - local_A4), local_FF7B, local_FF7A) == 1) {
      switch (arg0) {
        case 0x304C4142:
          task_2D8.x = (&task_2D8)->x * (1.0f - local_A4);
          task_2D8.y = task_2D8.y * (1.0f - local_A4);
          task_2D8.z = task_2D8.z * (1.0f - local_A4);
          break;
        case 0x394C4142:
          task_2E4.x = (&task_2E4)->x * (1.0f - local_A4);
          task_2E4.y = task_2E4.y * (1.0f - local_A4);
          task_2E4.z = task_2E4.z * (1.0f - local_A4);
          break;
      }
    } else {
      Script_EVT_SetPosition(arg0, POS_XYZ, &local_C4);
      script_FF02(arg0);
    }
  }
  return local_FF7D;
}

static int8_t script_F892(int32_t arg0, float arg1, float arg2, int32_t arg3, int32_t arg4) {
  float local_F4[3];
  float local_E8[3];
  Vector3 local_DC;
  Vector3 local_D0;
  float local_CC;
  float local_C8;
  float local_C4;
  float local_C0;
  float local_BC;
  float local_B8;
  float local_B4;
  float local_B0;
  float local_AC;
  float local_A8;
  float local_A4;
  int8_t local_A3;
  int8_t local_A2;
  int8_t local_A1;
  (&local_DC)->y = 0.0f;
  local_F4[1] = 0.0f;
  (&local_D0)->y = 0.0f;
  local_E8[1] = 0.0f;
  local_A3 = 0;
  local_B8 = 0.0f;
  local_A2 = 0;
  local_A1 = 0;
  local_F4[0] = arg1;
  local_F4[2] = arg2;
  Script_EVT_GetPosition(arg0, -1, &local_DC, 0);
  if (!((&local_DC)->x >= (&local_DC)->x + *local_F4)) {
    local_B0 = (&local_DC)->x;
    local_AC = (&local_DC)->x + *local_F4;
  } else {
    local_AC = (&local_DC)->x;
    local_B0 = (&local_DC)->x + *local_F4;
  }
  if (!((&local_DC)->z >= (&local_DC)->z + local_F4[2])) {
    local_A8 = (&local_DC)->z;
    local_A4 = (&local_DC)->z + local_F4[2];
  } else {
    local_A4 = (&local_DC)->z;
    local_A8 = (&local_DC)->z + local_F4[2];
  }
  if (!(*local_F4 <= -1.00000004e-36f) & !(*local_F4 >= 1.00000004e-36f)) {
    local_C4 = 0.0f;
    local_C0 = (&local_DC)->x;
  } else {
    local_C4 = local_F4[2] / *local_F4;
    local_C0 = (&local_DC)->z - local_C4 * (&local_DC)->x;
    local_BC = Script_MathUtils(SCRIPT_MATH_VEC2_LENGTH, *local_F4, local_F4[2]);
  }
  local_A3 = 0;
  for (;;) {
    if (!(local_C4 <= -1.00000004e-36f) & !(local_C4 >= 1.00000004e-36f)) {
      local_CC = local_C0;
    } else {
      local_CC = (*(float *)(&data_15690 + local_A3) - local_C0) / local_C4;
    }
    local_C8 = *(float *)(&data_15690 + local_A3);
    if (!(local_A8 > local_C8) & !(local_C8 > local_A4)) {
      if (!(*(float *)(&data_156A8 + local_A3) > local_CC) & !(local_CC > *(float *)(&data_156B8 + local_A3))) {
        if ((arg3 != 1) & (local_A3 != arg4)) {
          local_E8[0] = *local_F4 * (1.0f - Script_MathUtils(SCRIPT_MATH_INT_TO_FLOAT, task_2F8) / 500.0f);
          local_E8[1] = 0.0f;
          local_E8[2] = (float)0 - local_F4[2];
          switch (arg0) {
            case 0x304C4142:
              script_10949(&task_2D8, *local_E8, local_E8[1], local_E8[2]);
              break;
            case 0x394C4142:
              script_10949(&task_2E4, *local_E8, local_E8[1], local_E8[2]);
              break;
          }
          local_A2 = 1;
          break;
        }
      }
    }
    local_A3 = local_A3 + 1;
    if (local_A3 >= 4) {
      break;
    }
  }
  local_A3 = 0;
  for (;;) {
    if (local_A2 == 0) {
      local_CC = *(float *)(&data_156A0 + local_A3);
      local_C8 = *(float *)(&data_156A0 + local_A3) * local_C4 + local_C0;
      if (!(local_B0 > local_CC) & !(local_CC > local_AC)) {
        if (!(*(float *)&data_156C8 > local_C8) & !(local_C8 > *(float *)(&data_156C8 + 1))) {
          if ((arg3 != 2) & (local_A3 != arg4)) {
            local_E8[0] = (float)0 - *local_F4;
            local_E8[1] = 0.0f;
            local_E8[2] = local_F4[2] * (1.0f - Script_MathUtils(SCRIPT_MATH_INT_TO_FLOAT, task_2F8) / 500.0f);
            switch (arg0) {
              case 0x304C4142:
                script_10949(&task_2D8, *local_E8, local_E8[1], local_E8[2]);
                break;
              case 0x394C4142:
                script_10949(&task_2E4, *local_E8, local_E8[1], local_E8[2]);
                break;
            }
            local_A2 = 1;
            break;
          }
        }
      }
    }
    local_A3 = local_A3 + 1;
    if (local_A3 >= 2) {
      break;
    }
  }
  local_A3 = 0;
  if (local_A2 == 0) {
    for (;;) {
      if (!(local_C4 - *(float *)(&data_156D0 + local_A3) <= -1.00000004e-36f) & !(local_C4 - *(float *)(&data_156D0 + local_A3) >= 1.00000004e-36f)) {
      } else {
        local_CC = (*(float *)(&data_15700 + local_A3) - local_C0) / (local_C4 - *(float *)(&data_156D0 + local_A3));
        local_C8 = local_CC * local_C4 + local_C0;
        if (-!(*(float *)&data_156A0 >= local_CC) & -!(local_CC >= *(float *)(&data_156A0 + 1)) & -!(*(float *)&data_15690 >= local_C8) & -!(local_C8 >= *(float *)(&data_15690 + 2))) {
        } else {
          if (-!(local_CC <= 1.63599992f) & -!(local_CC >= 4.65599966f) & -!(local_C8 <= -9.471001f) & -!(local_C8 >= -7.951f)) {
            if (-!(local_B0 > local_CC) & -!(local_CC > local_AC) & -!(local_A8 > local_C8) & -!(local_C8 > local_A4)) {
              if ((arg3 != 3) & (local_A3 != arg4)) {
                local_B4 = (*(float *)(&data_156D0 + local_A3) * *local_F4 - local_F4[2]) * (float)-1 / (*(float *)(&data_156D0 + local_A3) * *(float *)(&data_156D0 + local_A3) + 1.0f);
                local_E8[0] = *local_F4 + *(float *)(&data_156D0 + local_A3) * local_B4 * (float)2;
                local_E8[1] = 0.0f;
                local_E8[2] = local_F4[2] - local_B4 * (float)2;
                switch (arg0) {
                  case 0x304C4142:
                    script_10949(&task_2D8, *local_E8, local_E8[1], local_E8[2]);
                    break;
                  case 0x394C4142:
                    script_10949(&task_2E4, *local_E8, local_E8[1], local_E8[2]);
                    break;
                }
                local_A2 = 1;
                break;
              }
            }
            goto L00FDDE;
          }
        }
      }
      L00FDDE:;
      local_A3 = local_A3 + 1;
      if (local_A3 >= 12) {
        break;
      }
    }
  }
  if (local_A2 == 1) {
    local_B8 = Script_MathUtils(SCRIPT_MATH_ABS, (local_CC - (&local_DC)->x) / *local_F4);
    local_D0.x = local_B8 * *local_F4 + (&local_DC)->x;
    (&local_D0)->y = (&local_DC)->y;
    (&local_D0)->z = local_B8 * local_F4[2] + (&local_DC)->z;
    Script_EVT_SetPosition(arg0, POS_XYZ, &local_D0);
    if (!(Script_MathUtils(SCRIPT_MATH_VEC2_LENGTH, *local_F4, local_F4[2]) <= 0.01f)) {
      Script_executeAicaCommand(329129, 0, 0);
    } else {
      if (!(Script_MathUtils(SCRIPT_MATH_VEC2_LENGTH, *local_F4, local_F4[2]) <= 0.0015f)) {
        Script_executeAicaCommand(394665, 0, 0);
        goto L00FEA7;
      }
    }
    L00FEA7:;
    SCNF_WaitFrames_0000(1);
    local_D0.x = *local_E8 * (1.0f - local_B8) + local_CC;
    (&local_D0)->y = (&local_DC)->y;
    (&local_D0)->z = local_E8[2] * (1.0f - local_B8) + local_C8;
    Script_EVT_SetPosition(arg0, POS_XYZ, &local_D0);
    script_FF02(arg0);
  }
  return local_A2;
}

static void script_FF02(int32_t arg0) {
  float local_FC;
  float local_F8;
  float local_F4;
  float local_F0;
  float local_EC;
  float local_E8;
  float local_E4;
  float local_E0;
  float local_DC;
  float local_D8;
  float local_D4;
  Vector3 local_C8;
  Vector3 local_BC;
  Vector3 local_B0;
  int8_t local_AF;
  int8_t local_AE;
  (&local_C8)->y = 0.0f;
  (&local_BC)->y = 0.0f;
  (&local_B0)->y = 0.0f;
  local_AF = 0;
  local_AE = 0;
  Script_EVT_GetPosition(arg0, -1, &local_BC, 0);
  switch (arg0) {
    case 0x304C4142:
      script_10949(&local_C8, (&task_2D8)->x, task_2D8.y, task_2D8.z);
      break;
    case 0x394C4142:
      script_10949(&local_C8, (&task_2E4)->x, task_2E4.y, task_2E4.z);
      break;
  }
  if (!((&local_C8)->x <= -1.00000004e-36f) & !((&local_C8)->x >= 1.00000004e-36f)) {
  } else {
    local_FC = (&local_C8)->z / (&local_C8)->x;
    local_F8 = (&local_BC)->z - local_FC * (&local_BC)->x;
    for (;;) {
      local_F4 = *(float *)(&data_15730 + local_AE);
      local_F0 = *(float *)(&data_15748 + local_AE);
      local_EC = Script_MathUtils(SCRIPT_MATH_ABS, local_FC * local_F4 - local_F0 + local_F8) / Script_MathUtils(SCRIPT_MATH_VEC2_LENGTH, local_FC, 1.0f);
      if (!(local_EC > 0.08f)) {
        local_E8 = (local_FC * local_F0 + local_F4 - local_FC * local_F8) / (local_FC * local_FC + 1.0f);
        local_E4 = Script_MathUtils(SCRIPT_MATH_SQRT, Script_MathUtils(SCRIPT_MATH_ABS, local_E8 * local_E8 - ((local_F8 - local_F0) * (local_F8 - local_F0) + local_F4 * local_F4 - 0.0064f) / (local_FC * local_FC + 1.0f)));
        local_DC = local_E8 + local_E4;
        local_D8 = local_E8 - local_E4;
        if (!(Script_MathUtils(SCRIPT_MATH_ABS, local_DC - (&local_BC)->x) > Script_MathUtils(SCRIPT_MATH_ABS, local_D8 - (&local_BC)->x))) {
          local_E0 = local_DC;
        } else {
          local_E0 = local_D8;
        }
        if (!((&local_BC)->x > (&local_BC)->x + (&local_C8)->x)) {
          if (!((&local_BC)->x > local_E0) & !(local_E0 > (&local_BC)->x + (&local_C8)->x)) {
            local_AF = 1;
          }
        } else {
          if (!((&local_BC)->x + (&local_C8)->x > local_E0) & !(local_E0 > (&local_BC)->x)) {
            local_AF = 1;
          }
        }
        if (!(((&local_BC)->x - *(float *)(&data_15730 + local_AE)) * ((&local_BC)->x - *(float *)(&data_15730 + local_AE)) + ((&local_BC)->z - *(float *)(&data_15748 + local_AE)) * ((&local_BC)->z - *(float *)(&data_15748 + local_AE)) > 0.0064f)) {
          local_AF = 1;
        }
        if (local_AF == 1) {
          script_10949(&local_B0, local_E0, (&local_BC)->y, local_FC * local_E0 + local_F8);
          Script_EVT_SetPosition(arg0, POS_XYZ, &local_B0);
          SCNF_WaitFrames_0000(1);
          local_D4 = Script_MathUtils(SCRIPT_MATH_VEC2_LENGTH, (&local_C8)->x, (&local_C8)->z);
          switch (arg0) {
            case 0x304C4142:
              script_10949(&task_2D8, 0.0f, 0.0f, 0.0f);
              task_2F2 = 1;
              task_2F0 = 4;
              break;
            case 0x394C4142:
              script_10949(&task_2E4, 0.0f, 0.0f, 0.0f);
              task_2F3 = 1;
              task_2F1 = 4;
              break;
          }
          Script_scenCreateTask(script_10224, 3, arg0, local_AE, local_D4);
          break;
        }
      }
      local_AE = local_AE + 1;
      if (local_AE >= 6) {
        break;
      }
    }
  }
  (void)local_AF;
  return;
}

static int32_t script_10224(int32_t arg0, int32_t arg1, float arg2) {
  float local_FC;
  float local_F8;
  float local_F4;
  float local_F0;
  float local_EC;
  float local_E8;
  int32_t unused_DC[3];
  Vector3 local_D0;
  Vector3 local_C4;
  Vector3 local_B8;
  Vector3i local_AC;
  int8_t local_AB;
  uint8_t unused_A8[3];
  int32_t local_A4;
  (&local_D0)->y = 0.0f;
  local_F0 = 0.005f;
  (&local_C4)->y = 0.0f;
  local_AB = 1;
  Script_EVT_GetPosition(arg0, -1, &local_D0, 0);
  local_FC = Script_MathUtils(SCRIPT_MATH_VEC2_LENGTH, *(float *)(&data_15730 + arg1) - (&local_D0)->x, *(float *)(&data_15748 + arg1) - (&local_D0)->z);
  local_F4 = arg2 * (*(float *)(&data_15730 + arg1) - (&local_D0)->x) / local_FC;
  local_EC = arg2 * (*(float *)(&data_15748 + arg1) - (&local_D0)->z) / local_FC;
  for (;;) {
    Script_EVT_GetPosition(arg0, -1, &local_C4, 0);
    local_F8 = Script_MathUtils(SCRIPT_MATH_VEC2_LENGTH, *(float *)(&data_15730 + arg1) - (&local_C4)->x, *(float *)(&data_15748 + arg1) - (&local_C4)->z);
    local_A4 = local_AB;
    switch (local_A4) {
      case 1:
        script_10964(&local_C4, local_F4, 0.0f, local_EC);
        local_F8 = Script_MathUtils(SCRIPT_MATH_VEC2_LENGTH, *(float *)(&data_15730 + arg1) - (&local_C4)->x, *(float *)(&data_15748 + arg1) - (&local_C4)->z);
        if (!(local_F8 > 0.06f)) {
          local_AB = 2;
        }
        break;
      case 2:
        script_10964(&local_C4, local_F4, 0.0f, local_EC);
        local_F8 = Script_MathUtils(SCRIPT_MATH_VEC2_LENGTH, *(float *)(&data_15730 + arg1) - (&local_C4)->x, *(float *)(&data_15748 + arg1) - (&local_C4)->z);
        if (!(local_F8 <= 0.06f)) {
          (&local_C4)->y = (&local_C4)->y - 0.06f;
        } else {
          (&local_C4)->y = (&local_C4)->y - (0.06f - local_F8);
        }
        if (!(local_F8 > 0.03f) | !(local_F8 <= 0.06f)) {
          local_AB = 3;
        }
        break;
      case 3:
        Script_executeAicaCommand(591273, 0, 0);
        local_AB = 4;
        break;
      case 4:
        if (!(((&local_C4)->x - *(float *)(&data_15730 + arg1)) * ((&local_C4)->x - *(float *)(&data_15730 + arg1)) + ((&local_C4)->z - *(float *)(&data_15748 + arg1)) * ((&local_C4)->z - *(float *)(&data_15748 + arg1)) > 0.0009f)) {
          script_10964(&local_C4, local_F4, 0.0f, local_EC);
        }
        script_10964(&local_C4, 0.0f, (float)0 - local_F0, 0.0f);
        if (!((&local_C4)->y >= 0.6f)) {
          local_AB = 5;
        }
        break;
      case 5:
        switch (arg0) {
          case 0x304C4142:
            task_2F0 = 5;
            break;
          case 0x394C4142:
            task_2F1 = 5;
            break;
        }
        goto L0105AE;
        break;
    }
    Script_EVT_SetPosition(arg0, POS_XYZ, &local_C4);
    local_E8 = Script_MathUtils(SCRIPT_MATH_VEC2_LENGTH, local_F4, local_EC);
    if (!(local_E8 <= -1.00000004e-36f) & !(local_E8 >= 1.00000004e-36f)) {
    } else {
      Script_EVT_GetPosition(arg0, -1, &local_B8, 0);
      script_10964(&local_B8, local_F4, 0.0f, local_EC);
      Script_EVT_SetFacingTowardPoint(arg0, 0x38000000, &local_B8, 0);
      script_109D6(&local_AC, -(int32_t)(local_E8 * 360.0f / 0.1884f / 360.0f * (float)0x10000), 0, 0);
      Script_EVT_SetRotation(arg0, -0x48000000, &local_AC, 0);
    }
    local_F4 = local_F4 * 0.985f;
    local_EC = local_EC * 0.985f;
    if (local_AB == 4) {
      local_F0 = local_F0 + local_F0;
    }
    SCNF_WaitFrames_0000(1);
  }
  L0105AE:;
  return Script_srchControl(0);
}

static int32_t script_105B5(void) {
  int32_t local_FC;
  int32_t unused_F8;
  GameDateTime local_F0;
  int32_t unused_EC;
  GameDateTime local_E4;
  Script_ReturnTimeDate(&local_E4);
  for (;;) {
    Script_ReturnTimeDate(&local_F0);
    if (task_2FB == 0) {
      if ((&local_F0)->hours >= (&local_E4)->hours + 1) {
        if ((&local_F0)->minutes >= (&local_E4)->minutes) {
          task_2FB = 6;
          script_DD7F();
          local_FC = SCNF_RequestActionSelectorFromParams_0008(0xFFFF, 37, 34, 0xFFFF, 600, 0xFFFF, ACTION_SELECTOR_MODE_STANDARD_TIMEOUT_CANCEL);
          if (local_FC == 37) {
            script_DD99();
            Script_ReturnTimeDate(&local_E4);
            task_2FB = 0;
          } else {
            task_2FB = 4;
            task_2FC = 1;
            break;
          }
        }
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  for (;;) {
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_10667(void) {
  int8_t local_FF;
  int8_t local_FE;
  local_FF = 0;
  local_FE = 0;
  SCNF_WaitFrames_0000(1);
  for (;;) {
    if (task_2F1 == 5) {
      task_2FB = 2;
      task_2FC = 1;
      break;
    } else if (task_2F0 == 5) {
      task_2FB = 1;
      task_2FC = 1;
      break;
    } else if (-(task_2F3 == 3) & -(task_2F2 == 3) & -(task_2F1 == 1)) {
      local_FF = local_FF + 1;
    } else if (-(task_2F3 == 1) & -(task_2F2 == 3) & -(task_2F1 == 1)) {
      local_FE = local_FE + 1;
    } else {
      local_FF = 0;
      local_FE = 0;
    }
    if (local_FF == 2) {
      task_2FB = 1;
      task_2FC = 1;
      break;
    } else {
      if (local_FE == 2) {
        task_2FB = 1;
        task_2FC = 1;
        break;
        goto L010747;
      }
    }
    L010747:;
    SCNF_WaitFrames_0000(1);
  }
  for (;;) {
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_1075E(void) {
  float local_FC;
  int8_t local_FB;
  local_FC = 0.9f;
  local_FB = 1;
  for (;;) {
    local_FC = 1.0f - 1.0f / Script_MathUtils(SCRIPT_MATH_INT_TO_FLOAT, 60) * Script_MathUtils(SCRIPT_MATH_INT_TO_FLOAT, local_FB);
    if (!(local_FC <= -1.00000004e-36f) & !(local_FC >= 1.00000004e-36f)) {
      local_FC = 0.0f;
    }
    Script_charaSetRenderOpacity(SHEN_NAME('B_QW'), local_FC);
    local_FB = local_FB + 1;
    if (local_FB > 60) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_charaDisplay(SHEN_NAME('B_QW'), CDM_Hide);
  Script_charaSetRenderOpacity(SHEN_NAME('B_QW'), 1.0f);
  for (;;) {
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_107FF(int32_t arg0, int32_t arg1, int32_t arg2) {
  SCNF_WaitFrames_0000(arg2);
  Script_seqEnterEvtCam(arg0, arg1, arg1);
  return;
}

static void script_10814(const void *arg0) {
  SCNF_WaitFrames_0000(1);
  while (Script_convGetIsActive(arg0)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_10834(int32_t arg0, int32_t arg1, Vector3 *arg2, Vector3i *arg3) {
  Vector3 local_F4;
  Vector3i local_E8;
  if (arg3 != 0) {
    Script_mtwkGetSetRotation(arg0, &local_E8, 0);
    script_109D6(arg3, (&local_E8)->x, (&local_E8)->y, (&local_E8)->z);
    if (arg1 == 0x52494B41) {
      Script_motmSetBlendFrames(CHARA_AKIR, MOTM_BLEND_FRAMES_IMMEDIATE);
    }
    Script_EVT_SetRotation(arg1, 0x38000000, arg3, 0);
  }
  if (arg2 != 0) {
    Script_EVT_GetPosition(arg0, -1, &local_F4, 0);
    if (arg3 != 0) {
      arg2->x = Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_E8)->y) * arg2->x + Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_E8)->y) * arg2->z;
      arg2->z = ((float)0 - Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_E8)->y)) * arg2->x + Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_E8)->y) * arg2->z;
    }
    script_10964(arg2, (&local_F4)->x, (&local_F4)->y, (&local_F4)->z);
    if (arg1 == 0x52494B41) {
      Script_motmSetBlendFrames(CHARA_AKIR, MOTM_BLEND_FRAMES_IMMEDIATE);
      Script_InitializeOsag(CHARA_AKIR);
    }
    Script_EVT_SetPosition(arg1, POS_XYZ, arg2);
  }
  return;
}

static void script_10949(Vector3 *arg0, float arg1, float arg2, float arg3) {
  arg0->x = arg1;
  arg0->y = arg2;
  arg0->z = arg3;
  return;
}

static void script_10964(Vector3 *arg0, float arg1, float arg2, float arg3) {
  arg0->x = arg0->x + arg1;
  arg0->y = arg0->y + arg2;
  arg0->z = arg0->z + arg3;
  return;
}

static void script_10991(Vector3i *arg0, float arg1, float arg2, float arg3) {
  arg0->x = (int32_t)(arg1 / 360.0f * (float)0x10000);
  arg0->y = (int32_t)(arg2 / 360.0f * (float)0x10000);
  arg0->z = (int32_t)(arg3 / 360.0f * (float)0x10000);
  return;
}

static void script_109D6(Vector3i *arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  arg0->x = arg0->x + arg1;
  arg0->y = arg0->y + arg2;
  arg0->z = arg0->z + arg3;
  return;
}

static void script_10A03(int32_t arg0, float arg1, float arg2, float arg3) {
  *(int32_t *)arg0 = *(int32_t *)arg0 + (int32_t)(arg1 / 360.0f * (float)0x10000);
  *(int32_t *)(arg0 + 4) = *(int32_t *)(arg0 + 4) + (int32_t)(arg2 / 360.0f * (float)0x10000);
  *(int32_t *)(arg0 + 8) = *(int32_t *)(arg0 + 8) + (int32_t)(arg3 / 360.0f * (float)0x10000);
  return;
}

static void script_10A5A(void) {
  Script_LoadSND(0, 3, "f1jazzba.snd", 0);
  Script_ccowSetFlag80(SHEN_NAME('BAL0'), 1);
  Script_ccowSetFlag80(SHEN_NAME('BAL9'), 1);
  Script_ccowUtils(SHEN_NAME('BAL0'), 2);
  Script_ccowUtils(SHEN_NAME('BAL9'), 2);
  Script_charaDisplay(CHARA_AKIR, CDM_Hide);
  Script_scenToggleActiveFor(SHEN_NAME('BAL0'), 0);
  Script_scenToggleActiveFor(SHEN_NAME('BAL9'), 0);
  Script_scenToggleActiveFor(SHEN_NAME('B_QW'), 0);
  return;
}

static void script_10AC8(void) {
  Script_scenToggleActiveFor(SHEN_NAME('BAL0'), 1);
  Script_scenToggleActiveFor(SHEN_NAME('BAL9'), 1);
  Script_scenToggleActiveFor(SHEN_NAME('B_QW'), 1);
  return;
}

static void script_10AEF(void) {
  int32_t local_E4[7];
  local_E4[0] = 999;
  local_E4[1] = 0;
  local_E4[2] = 28;
  local_E4[3] = 0;
  local_E4[4] = 1;
  local_E4[5] = 0x4F4D4F4A;
  local_E4[6] = 3;
  Script_SetShouldSleepFlag(0);
  Script_SetHomeArea((const void *)local_E4);
  Script_SetShouldSleepFlag(1);
  SCNF_WaitFrames_0000(1);
  return;
}

static int32_t script_10B4C(int32_t arg0, const Vector3 *arg1, float arg2, float arg3) {
  int32_t local_FC;
  Vector3 local_F0;
  int32_t unused_E4[3];
  Vector3i local_D8;
  Script_EVT_GetPosition(arg0, -1, &local_F0, 0);
  Script_mtwkGetSetRotation(arg0, &local_D8, 0x2000000);
  local_FC = Script_CheckTargetWithinAngleRange(&local_F0, arg2, (&local_D8)->y, (int32_t)(arg3 / 360.0f * (float)0x10000), arg1);
  return local_FC;
}

static int32_t script_10B90(int32_t arg0) {
  int16_t local_FE;
  int16_t local_FC;
  int16_t local_FA;
  int16_t local_F8;
  int16_t local_F6;
  uint8_t unused_F4[2];
  int16_t local_F2;
  int16_t local_F0;
  int16_t local_EE;
  uint8_t unused_EC[2];
  int32_t local_E8;
  int32_t local_E4;
  int32_t local_E0;
  int32_t local_DC;
  int32_t unused_D0[3];
  GameDateTime local_C8;
  local_FE = Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_DBYO_DJAZ_DHQB_CHAIN_ACTIVE);
  local_FC = Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_DHQB_CHALLENGE_COMPLETE);
  local_FA = Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 823);
  local_F8 = Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 876);
  local_F6 = Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_NOZOMI_KIDNAPPED);
  local_E8 = Script_shenGetStats(STATS_CASH);
  Script_sprintf(" gold: %d \012", local_E8);
  Script_CycleManagerUtils(CYCLE_CMD_SETUP, "scene/03/D000", (int32_t)"SCRTEST.BIN", 0x5A414A44);
  if (((local_FE == 1) & (local_FA == 0)) | (-(local_FC == 1) & -(local_FA == 1) & -(local_F6 == 0))) {
    Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
    Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT, 0x49524948, 1);
    local_E4 = Script_CycleManagerUtils(CYCLE_CMD_GET_CYCLE_PTR_BY_CHARA_ID, 0x49524948);
    while (local_E4 == 0) {
      Script_sprintf(" h1 load error \012");
      SCNF_WaitFrames_0000(1);
      local_E4 = Script_CycleManagerUtils(CYCLE_CMD_GET_CYCLE_PTR_BY_CHARA_ID, 0x49524948);
    }
    L010C79:;
    SCNF_WaitFrames_0000(1);
    if (!Script_CycleManagerUtils(CYCLE_CMD_GET_NEXT_TASK_POINTER, local_E4)) goto L010C79;
    Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT, 0x4D484A47, 1);
    local_E0 = Script_CycleManagerUtils(CYCLE_CMD_GET_CYCLE_PTR_BY_CHARA_ID, 0x4D484A47);
    while (local_E0 == 0) {
      Script_sprintf(" h2 load error \012");
      SCNF_WaitFrames_0000(1);
      local_E0 = Script_CycleManagerUtils(CYCLE_CMD_GET_CYCLE_PTR_BY_CHARA_ID, 0x4D484A47);
    }
    L010CDA:;
    SCNF_WaitFrames_0000(1);
    if (!Script_CycleManagerUtils(CYCLE_CMD_GET_NEXT_TASK_POINTER, local_E0)) goto L010CDA;
    Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT, 0x4D424A47, 1);
    local_DC = Script_CycleManagerUtils(CYCLE_CMD_GET_CYCLE_PTR_BY_CHARA_ID, 0x4D424A47);
    while (local_DC == 0) {
      Script_sprintf(" h3 load error \012");
      SCNF_WaitFrames_0000(1);
      local_DC = Script_CycleManagerUtils(CYCLE_CMD_GET_CYCLE_PTR_BY_CHARA_ID, 0x4D424A47);
    }
    L010D3B:;
    SCNF_WaitFrames_0000(1);
    if (!Script_CycleManagerUtils(CYCLE_CMD_GET_NEXT_TASK_POINTER, local_DC)) goto L010D3B;
    Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 1);
    script_1101F(CHARA_HIRI);
    script_110D9(CHARA_GJHM);
    script_11087(CHARA_GJBM);
    Script_sprintf(" EVENT_ON \012");
  }
  if (-(local_FE == 1) & -(local_F8 == 0) & -(local_FC == 0) & -(local_E8 >= 1000)) {
    Script_cdttLoadScrUtils(1, 0);
    script_A18E();
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 876, 1);
    local_F8 = 1;
    goto L010DC3;
  }
  L010DC3:;
  Script_CycleManagerUtils(CYCLE_CMD_RESET_LOAD_SETTLE);
  for (;;) {
    if (Script_CycleManagerUtils(CYCLE_CMD_TICK_LOAD_SETTLE_COUNT) == 0) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_cdttLoadScrUtils(1, 0);
  Script_filmCheckOrSetTransitionUtil(32, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  SCNF_WaitFrames_0000(1);
  Script_shenClearFreeState(1);
  Script_ryoControl(RYO_RestoreControl);
  while (-(local_FE == 1) & -(local_FA == 0) & -(local_FC == 0) & -(local_E8 >= 1000)) {
    if (Script_SetTaskDataValue(CHARA_AKIR, &data_157D0, &data_157DC, 0) & script_10B4C(CHARA_AKIR, (const Vector3 *)"33S@", 4.0f, 45.0f)) {
      if (Script_IsPlayerAllowed(0)) {
        script_A18E();
      }
    }
    local_FE = Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_DBYO_DJAZ_DHQB_CHAIN_ACTIVE);
    local_FA = Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 823);
    if (local_FA == 1) {
      Script_sprintf(data_15859);
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_DJAZ_BALL_EVENT_SUCCESS) == 1) {
        script_10AEF();
      }
      Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
      Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT, 0x49524948, 0);
      SCNF_WaitFrames_0000(1);
      Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT, 0x4D484A47, 0);
      SCNF_WaitFrames_0000(1);
      Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT, 0x4D424A47, 0);
      SCNF_WaitFrames_0000(1);
      Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 1);
      Script_executeAicaCommand(2720, 1, 30);
      Script_executeAicaCommand(2720, 2, 30);
      Script_executeAicaCommand(2720, 3, 30);
      Script_shenRequestGameModuleTransition(Script_GetSceneID(), SHEN_NAME('D000'), 13);
      SCNF_WaitFrames_0000(1);
    }
    SCNF_WaitFrames_0000(1);
  }
  while (-(local_FC == 1) & -(local_FA == 1) & -(local_F6 == 0)) {
    Script_ReturnTimeDate(&local_C8);
    local_F2 = (&local_C8)->hours;
    local_F0 = (&local_C8)->minutes;
    local_EE = (&local_C8)->seconds;
    if ((local_F2 >= 22) & (local_F0 >= 30)) {
      break;
    } else {
      if (Script_SetTaskDataValue(CHARA_AKIR, &data_157D0, &data_157DC, 0) & script_10B4C(CHARA_AKIR, (const Vector3 *)"33S@", 4.0f, 45.0f)) {
        if (Script_IsPlayerAllowed(0)) {
          Script_ryoControl(PCCTL_DisablePlayerControl);
          script_A459();
        }
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  SCNF_WaitFrames_0000(1);
  return Script_srchControl(0);
}

static void script_1101F(int32_t arg0) {
  {
    Vector3 local_F4;
    local_F4.x = 5.4644f;
    (&local_F4)->z = -3.9343f;
    Script_EVT_SetPosition(arg0, 0x28000000, &local_F4);
  }
  {
    Vector3i local_F4;
    (&local_F4)->y = 0x42AA4F5C;
    Script_EVT_SetRotation(arg0, MATRIX_XFORM_Y, &local_F4, 0);
  }
  Script_charaSetRefboardFlagTo(arg0, 2);
  Script_PlayCharaMotion(arg0, 4131, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  return;
}

static void script_11087(int32_t arg0) {
  script_11141(arg0, 1.1f, 0.0f, -8.7f);
  script_11169(arg0, 0.0f, 270.0f, 0.0f);
  Script_charaSetRefboardFlagTo(arg0, 2);
  Script_PlayCharaMotion(arg0, 4132, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  return;
}

static void script_110D9(int32_t arg0) {
  script_11141(arg0, 5.3f, 0.0f, -8.45f);
  script_11169(arg0, 0.0f, 85.0f, 0.0f);
  Script_charaSetRefboardFlagTo(arg0, 2);
  Script_PlayCharaMotion(CHARA_GJHM, 4110, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_scenCreateTask(script_D36A, 0);
  SCNF_WaitFrames_0000(30);
  return;
}

static void script_11141(int32_t arg0, float arg1, float arg2, float arg3) {
  Vector3 local_F4;
  local_F4.x = arg1;
  (&local_F4)->y = arg2;
  (&local_F4)->z = arg3;
  Script_EVT_SetPosition(arg0, POS_XYZ, &local_F4);
  return;
}

static void script_11169(int32_t arg0, float arg1, float arg2, float arg3) {
  Vector3i local_F4;
  local_F4.x = (int32_t)(arg1 / 360.0f * (float)0x10000);
  (&local_F4)->y = (int32_t)(arg2 / 360.0f * (float)0x10000);
  (&local_F4)->z = (int32_t)(arg3 / 360.0f * (float)0x10000);
  Script_EVT_SetRotation(arg0, 0x38000000, &local_F4, 0);
  return;
}

static void script_111BD(int32_t arg0, int32_t arg1, int32_t arg2) {
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

static int32_t script_1120D(int32_t arg0) {
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

static void script_112B4(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  local_FC = script_1120D(arg0);
  if (arg1 > local_FC) {
    (void)0;
  } else {
    (void)-1;
  }
  return;
}

static int32_t script_112D7(int32_t arg0) {
  int32_t unused_FC;
  GameDateTime local_F4;
  int32_t local_F0;
  int32_t local_EC;
  Script_ReturnTimeDate(&local_F4);
  local_F0 = (&local_F4)->hours * 60 + (&local_F4)->minutes;
  local_EC = Script_scenUtils(SCENARIO_CMD_GET_STEP_LUT, arg0) * 6;
  return local_F0 - local_EC;
}

static void script_1130A(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  local_FC = script_112D7(arg0);
  if (arg1 > local_FC) {
    (void)0;
  } else {
    (void)-1;
  }
  return;
}

static void script_1132D(int32_t arg0, int32_t arg1) {
  int8_t local_F4[12];
  int32_t local_F0;
  local_F0 = script_112D7(arg0);
  local_F4[4] = local_F0 / 60;
  local_F4[5] = local_F0 % 60;
  if ((local_F4[4] < *(int8_t *)(arg1 + 4)) | ((local_F4[4] == *(int8_t *)(arg1 + 4)) & (local_F4[5] < *(int8_t *)(arg1 + 5)))) {
    (void)0;
  } else {
    (void)-1;
  }
  return;
}

static void script_1138C(int32_t arg0) {
  int32_t local_F8[2];
  local_F8[0] = arg0;
  local_F8[1] = 0;
  (void)local_F8;
  return;
}

static void script_113A1(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
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

static void script_11737(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
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

__attribute__((scn_string_bindings(0, data_15880)))
static void script_11DC5(void) {
  Script_sprintf("TUNED FreeTalk def\012");
  SCNT_Enqueue();
  return;
}

__attribute__((scn_string_bindings(0, data_15894)))
static void script_11DD4(void) {
  Script_sprintf("TUNED FreeTalk def\012");
  SCNT_Enqueue();
  return;
}

static void script_11DE3(void) {
  return;
}

static void script_11DE6(void) {
  return;
}

static void script_11DE9(void) {
  return;
}

static void script_11DEC(void) {
  return;
}

static void script_11DEF(void) {
  (void)((task_324 = 0) & 0x20);
  return;
}

static int32_t script_11DFC(void) {
  return task_318 = Script_freeTalkGetCurrentChara();
}

static void script_11E08(void) {
  *(&task_318 + 1) = Script_GetFreeTalkStatus();
  return;
}

static int32_t script_11E17(void) {
  return *(&task_318 + 2) = Script_freeTalkGetAndResetState();
}

static void script_11E26(void) {
  int32_t local_FC;
  local_FC = Script_GetSceneID();
  switch (local_FC) {
    case 1:
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_ENTRY2_TIMED_EVENT_DONE) == 0) {
        Script_TimedTransitionUtils(TIMED_TRANSITION_REGISTER, 1, 0x3044484A, 2, &data_15918, 0xD5A6C3, TimedTransitionUpdateMemoItem150);
      }
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JD00_ENTRY20_TIMED_EVENT_DONE) == 0) {
        Script_TimedTransitionUtils(TIMED_TRANSITION_REGISTER, 1, 0x3030444A, 20, &data_158E0, 0xDA3AA3, 0);
      }
      break;
    case 2:
      if ((Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_GMCT_ROUTE_STARTED) == 0) | (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_GMCT_ROUTE_ADVANCED) == 0)) {
        Script_TimedTransitionUtils(TIMED_TRANSITION_REGISTER, 2, 0x54434D47, 1, &data_158A8, 0xB7420F, 0);
      }
      break;
    case 3:
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOZOMI_RESCUE_RUSH_ACTIVE) == 0) {
        Script_TimedTransitionUtils(TIMED_TRANSITION_REGISTER, 3, 0x4F4D4F4A, 20, &data_158C4, 0x2DC9FE, TimedTransitionRollbackToPreviousDay2330);
      }
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_FIRED_FROM_JOB) & ~Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 970)) {
        if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_MFSY_ENTRY42_TIMED_EVENT_DONE) == 0) {
          Script_TimedTransitionUtils(TIMED_TRANSITION_REGISTER, 3, 0x5953464D, 42, &data_158FC, 0xBBB817, 0);
        }
      }
      break;
  }
  return;
}

static void script_11F5E(int32_t arg0) {
  script_11F76(arg0);
  return;
}

static void script_11F6A(int32_t arg0) {
  script_11F76(arg0);
  return;
}

static void script_11F76(int32_t arg0) {
  if (Script_StepUtils(GET_STEP_HIGHER, 0) == 22) {
    int32_t local_FC;
    local_FC = Script_StepUtils(GET_STEP_LOWER, 0);
    if ((local_FC == 2) | (local_FC == 3)) {
      Script_SetShouldSleepFlag(0);
      goto L011FAF;
    }
  }
  L011FAF:;
  Script_ryoSetChara(arg0);
  Script_facemotUtils(arg0, 1, 0);
  Script_facemotUtils(arg0, 0, 0);
  Script_FaceEx(arg0, 0, 0, 2);
  Script_CharHandlers(arg0, -1, 0, 0);
  script_11E26();
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

static int32_t script_12072(int32_t arg0) {
  script_11F6A(arg0);
  for (;;) {
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_12089(int32_t arg0) {
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

static void script_120CA(int32_t arg0) {
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

static void script_1210E(int32_t arg0) {
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

static void script_1218C(void) {
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_PROGRESS_STEP_28_REACHED)) {
    script_1210E(40);
  } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_PROGRESS_STEP_1E_REACHED)) {
    script_1210E(30);
  } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_PROGRESS_STEP_14_REACHED)) {
    script_1210E(20);
  } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JU00_CATA1_NBO_BRANCH_CHOSEN)) {
    script_1210E(10);
  } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_PROGRESS_STEP05_SA1116_DOJO_DONE)) {
    script_1210E(5);
  } else {
    script_1210E(0);
  }
  return;
}

void script_1220B(void) {
  int32_t unused_FC;
  Script_filmCheckOrSetTransitionUtil(0, 255, 0, 0, 0, 0, 0, 0, 0);
  Script_PREP_Related(-1, 1);
  Script_PREP_Related(1, 0);
  Script_PlayerStatsUtils(PS_IncrementSpotVisitCount, 44);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 45.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 10.0f);
  script_11DC5();
  Script_Ambient1SoundUtils(1, 24232);
  Script_GetSetWeatherSettings(WEATHER_SETTINGS_SET_DISABLE_WEATHER, 1);
  Script_WeatherUtils_0(WeatherDataUtil_ApplyCurrentWeatherOrEnableDateTimeRead, 0);
  Script_WeatherTaskUtil(InitTaskData);
  Script_WeatherUtils(WeatherScript_ApplyWccpClipSet1, 6.3734f, 0.4694f, -0.0821f, 0.2626f, -0.4886f, -11.9475f, 6.5282f, -11.9522f, -0.3f, 3.1f);
  Script_WeatherTaskUtil(ToggleFogTask, 0);
  Script_WeatherTaskUtil(ToggleSceneLightingFlags, 0);
  Script_SetGlobalSceneSlotValue34(1, 1);
  Script_SetupLightFromTable(0);
  if (Script_ryoSleepUtils(0) != 0) {
    Script_ryoSleepUtils(1);
  }
  Script_motiLoad(Script_GetSceneID(), SHEN_NAME('FREE'), "M_MOBJ.BIN");
  script_7FBB(CHARA_AKIR, (DSWKDoorEntry *)&data_14234, (int32_t *)data_1426C, (DoorResourceDescriptor *)&data_1427C, (MapObjectRecord *)&data_1428C, 10, 0);
  SCNF_WaitFrames_0000(1);
  SCNF_WaitFrames_0000(1);
  DSWK_DoorUtils(DOOR_CMD_SET_STATE_1);
  Script_scenCreateTask(script_12072, 1, CHARA_AKIR);
  Script_scenCreateTask(script_10B90, 1, 0);
  Script_scenCreateTask(script_12444, 0);
  Script_scenCreateTask(script_9EB3, 0);
  Script_scenCreateTask(script_123CA, 0);
  Script_TimedTransitionUtils(TIMED_TRANSITION_REGISTER, Script_GetSceneID(), 0x30303044, 13, (const void *)-1, 0x3D1043, 6);
  SCNF_WaitFrames_0000(1);
  Script_CycleManagerUtils(CYCLE_CMD_RESET_LOAD_SETTLE);
  for (;;) {
    if (Script_CycleManagerUtils(CYCLE_CMD_TICK_LOAD_SETTLE_COUNT) == 0) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_ryoSleepUtils(2, 1);
  return;
}

static int32_t script_123CA(void) {
  int8_t local_FF;
  int32_t local_F8;
  script_11DC5();
  for (;;) {
    local_FF = script_11E17();
    local_F8 = local_FF;
    if (local_F8 == 130) {
      local_F8 = script_11DFC();
      switch (local_F8) {
        case 0x49524948:
          break;
        case 0x54524252:
          break;
        case 0x5F424F42:
          break;
      }
      script_11DEF();
    } else {
      if (local_FF != -1) {
        script_11DEF();
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_12444(void) {
  int32_t local_F4[3];
  int32_t local_E8[3];
  int32_t local_E4;
  local_F4[0] = 0;
  local_F4[1] = 0;
  local_F4[2] = 0;
  local_E8[0] = 0;
  local_E8[1] = 0;
  local_E8[2] = 0;
  for (;;) {
    if ((&task_7C)->mode) {
      local_E4 = task_7C.entry_idx;
      if (local_E4 == 0) {
        SCNF_WaitFrames_0000(90);
        Script_executeAicaCommand(2720, 1, 30);
        Script_executeAicaCommand(2720, 2, 30);
        Script_executeAicaCommand(2720, 3, 30);
        Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
        while (Script_filmCheckOrSetTransitionUtil(-1)) {
          SCNF_WaitFrames_0000(1);
        }
        Script_shenRequestGameModuleTransition(Script_GetSceneID(), SHEN_NAME('D000'), 13);
        SCNF_WaitFrames_0000(1);
        goto L012500;
      }
    }
    L012500:;
    SCNF_WaitFrames_0000(1);
  }
  end:;
}
