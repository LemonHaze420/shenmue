#include "act03_mbqc.h"

static void script_0030(void);
static void script_01EA(void);
static void script_0314(void);
static void script_0345(void);
static void script_0376(void);
static void script_047C(void);
static int32_t script_0484(int32_t arg0, int32_t arg1);
static int32_t script_04A8(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4);
static int32_t script_057F(int32_t arg0, int32_t arg1, int32_t arg2, float arg3, int32_t arg4, int32_t arg5);
static void script_0616(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4);
static void script_0678(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_06D4(int32_t arg0, float arg1, int32_t arg2);
static void script_0731(int32_t arg0);
static int32_t script_074D(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, void *arg4, int32_t arg5);
static void script_0974(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_09AB(int32_t arg0);
static void script_09BB(void);
static void script_09DB(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_0A15(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_0A61(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4);
static void script_0AAA(int32_t arg0);
static int32_t script_0ABD(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_0B20(int32_t arg0, int32_t arg1);
static void script_0BE1(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_0CCF(void);
static void script_0DBD(int32_t arg0, int32_t arg1);
static void script_0DE6(int32_t arg0, int32_t arg1, float arg2, float arg3, float arg4);
static void script_0E75(int32_t arg0, int32_t arg1, const Vector3 *arg2);
static void script_0EA8(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static int32_t script_0EC2(int32_t arg0, int32_t arg1, float arg2, float arg3, float arg4, int32_t arg5);
static void script_0EED(int32_t arg0, int32_t arg1, float arg2, float arg3, float arg4, float arg5);
static void script_0F6D(int32_t arg0, int32_t arg1, float arg2);
static void script_0FC1(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_0FD7(int32_t arg0, int32_t arg1, float arg2, int32_t arg3);
static void script_0FFE(int32_t arg0, float arg1);
static void script_1027(int32_t arg0, float arg1, float arg2, float arg3);
static void script_107B(int32_t arg0, int32_t arg1, int32_t arg2);
static int32_t script_108F(int32_t arg0, float arg1, int32_t arg2);
static void script_10B4(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4);
static int32_t script_10CC(int32_t arg0, float arg1, float arg2, float arg3, int32_t arg4);
static void script_10F5(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4);
static int32_t script_110D(int32_t arg0, float arg1, float arg2, float arg3, int32_t arg4);
static void script_1162(int32_t arg0, int32_t arg1);
static void script_1247(int32_t arg0, int32_t arg1);
static void script_1330(int32_t arg0);
static void script_134E(void);
static void script_1396(void);
static void script_13B1(void);
static void script_13CB(const char *arg0, const char *arg1);
static void script_1409(const char *arg0);
static void script_142A(int32_t arg0);
static int32_t script_1441(int32_t arg0);
static void script_14C7(int32_t arg0, int32_t arg1);
static void script_14EA(int32_t arg0);
static int32_t script_1DBA(int32_t arg0, int32_t arg1, int32_t arg2);
static int32_t script_240B(int32_t arg0, int32_t arg1, int32_t arg2);
static int32_t script_2555(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_269F(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_2855(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_2AC7(int32_t arg0, int32_t arg1, int32_t arg2);
static int32_t script_2C11(int32_t arg0, int32_t arg1, int32_t arg2);
static int32_t script_2D23(int32_t arg0, int32_t arg1, int32_t arg2);
static int32_t script_2E35(int32_t arg0, int32_t arg1);
static void script_3017(int32_t arg0, int32_t arg1, float arg2, float arg3, float arg4, float arg5);
static int32_t script_3487(int32_t arg0, int32_t arg1, int32_t arg2);
static int32_t script_355F(int32_t arg0, int32_t arg1, float arg2, int32_t arg3, const void *arg4);
static void script_367C(int32_t arg0, float arg1, const void *arg2, int32_t arg3);
static void script_370B(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4);
static int32_t script_3723(int32_t arg0, float arg1, float arg2, float arg3, int32_t arg4);
static void script_377F(int32_t arg0, float arg1);
static void script_37C3(int32_t arg0, int32_t arg1, int32_t arg2);
static int32_t script_37D7(int32_t arg0, float arg1, int32_t arg2);
static void script_37FC(int32_t arg0);
static void script_3826(int32_t arg0, int32_t arg1);
static int32_t script_3838(int32_t arg0, int32_t arg1);
static int32_t script_385B(void);
static void script_394D(int32_t arg0, int32_t arg1);
static void script_3A8D(int32_t arg0, int32_t arg1);
static int32_t script_3C10(int32_t arg0, int32_t arg1);
static int32_t script_3DF9(void);
static void script_3E5D(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_3E73(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_3E9E(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_3EB4(float arg0, float arg1, float arg2, int32_t arg3);
static void script_3ED9(int32_t arg0, int32_t arg1);
static int32_t script_3EEB(int32_t arg0, int32_t arg1);
static void script_410A(int32_t arg0, int32_t arg1);
static void script_411B(int32_t arg0, int32_t arg1);
static void script_448D(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static int32_t script_44A7(int32_t arg0, int32_t arg1, int32_t arg2, float arg3, float arg4, int32_t arg5);
static void script_44E1(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4);
static int32_t script_44F9(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4);
static void script_4520(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4);
static int32_t script_4538(int32_t arg0, int32_t arg1, float arg2, int32_t arg3, int32_t arg4);
static void script_45B0(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_45C6(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_4602(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5, int32_t arg6);
static int32_t script_461E(int32_t arg0, int32_t arg1, int32_t arg2, float arg3, int32_t arg4, int32_t arg5, int32_t arg6);
static void script_467F(int32_t arg0, int32_t arg1);
static void script_46CF(int32_t arg0, int32_t arg1);
static void script_4726(int32_t arg0, int32_t arg1);
static void script_4780(int32_t arg0, int32_t arg1);
static void script_4817(int32_t arg0);
static void script_4820(int32_t arg0);
static void script_482E(int32_t arg0);
static void script_483C(void);
static void script_4B7E(int32_t arg0, int32_t arg1);
static int32_t script_50C4(int32_t arg0, int32_t arg1);
static void script_56E9(int32_t arg0, int32_t arg1);
static void script_5E06(int32_t arg0, int32_t arg1);
static int32_t script_6326(int32_t arg0, int32_t arg1);
static void script_697A(int32_t arg0, int32_t arg1);
static void script_6CE0(int32_t arg0, int32_t arg1);
static int32_t script_6D0A(int32_t arg0, int32_t arg1);
static void script_7DC0(int32_t arg0, int32_t arg1);
static void script_8009(int32_t arg0, int32_t arg1);
static void script_853E(int32_t arg0, int32_t arg1);
static void script_9AD0(int32_t arg0, int32_t arg1);
void script_A05A(void);
static void script_A0A2(int32_t arg0);
static void script_A157(void);
static void script_A3BC(void);
static void script_A633(void);
static int32_t script_A6A2(int32_t arg0, int32_t arg1, int32_t arg2);
static int32_t script_A8C2(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4);
static int32_t script_AA4C(int32_t arg0);
static int32_t script_AB33(int32_t arg0, int32_t arg1);
static int32_t script_AB70(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4);
static int32_t script_AC6A(int32_t arg0, int32_t arg1, int32_t arg2);
static int32_t script_ACF0(int32_t arg0, int32_t arg1, int32_t arg2);
static int32_t script_ADDA(int32_t arg0, int32_t arg1, int32_t arg2);

static int32_t task_0;
static int32_t task_4;
static int32_t task_8;
static int32_t task_C;
static int32_t task_10;
static int32_t task_14;
static int32_t task_18;
static int32_t task_1C;
static int32_t task_20[4];
static int32_t task_30[4];
static int32_t task_40[4];
static int32_t task_50[4];
static int32_t task_60[4];
static int32_t task_70;
static int32_t task_74[4];
static int32_t task_84[4];
static int32_t task_94[4];
static int32_t task_A4[4];
static int32_t task_B4;
static int32_t task_B8;
static int32_t task_BC;
static int32_t task_C0;
static int32_t task_C4;
static int32_t task_C8;
static int32_t task_CC[4];
static float task_DC[4][3];
static float task_10C[4];
static float task_11C;
static float task_120;
static float task_124;
static float task_128[4][3];
static int32_t task_158[4][3];
static float task_188[4];
static int32_t task_198[4];
static int32_t task_1A8;
static int32_t task_1AC;
static int32_t task_1B0[7];
static int32_t task_1CC[7];
static int32_t task_1E8[7];
static int32_t task_204[7];
static int32_t task_220[7];
static int32_t task_23C[7];
static int32_t task_258[7];
static float task_274[7];
static int32_t task_290;
static int32_t task_294;
static const uint32_t data_AEC8 = 0x41A00000;
static const float data_AECC[2] = {0.0f, -50.0f};
static const uint32_t data_AED4 = 0x41A00000;
static const float data_AED8[2] = {0.0f, -50.0f};
static const uint32_t data_AEE0 = 0xC2200000;
static const float data_AEE4[2] = {0.0f, 80.0f};
static const uint32_t data_AEEC = 0xC2D60000;
static const float data_AEF0[2] = {0.0f, 78.0f};
static const uint8_t data_AEF8[8] = {0};
static const float data_AF00 = 90.0f;
static const uint32_t data_AF04 = 0xC1A00000;
static const float data_AF08[2] = {0.0f, -50.0f};
static const uint32_t data_AF10 = 0xC1A00000;
static const float data_AF14[2] = {0.0f, -50.0f};
static const uint32_t data_AF1C = 0xC2E60000;
static const float data_AF20[2] = {0.0f, 175.0f};
static const uint32_t data_AF28 = 0xC2DC0000;
static const float data_AF2C[2] = {0.0f, 10.0f};
static const uint32_t data_AF34 = 0x42C80000;
static const float data_AF38[2] = {0.0f, -40.0f};
static const uint32_t data_AF40 = 0x43020000;
static const float data_AF44[2] = {0.0f, 25.0f};
static const uint8_t data_AF4C[12] = {0};
static const uint32_t data_AF58 = 0x3F800000;
static const float data_AF5C[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint32_t data_AFE0 = 0x3F800000;
static const float data_AFE4[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint8_t data_B068[16] = {0};
static const float data_B078[30] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint32_t data_B0F0 = 0x3F800000;
static const float data_B0F4[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint32_t data_B178 = 0x3F800000;
static const float data_B17C[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint8_t data_B200[84] = {0};
typedef struct Row_B254 { float w0; float w1; float w2; int32_t w3; } Row_B254;
static const Row_B254 data_B254[3] = {
  {0.787f, 0.787f, 0.787f, 0},
  {0.0f, 0.0f, 0.0f, 0},
  {0.0f, 0.0f, 0.0f, 0}
};
static const int32_t data_B284 = 0;
static const uint32_t data_B288 = 0x3F800000;
static const float data_B28C[33] = {
  0.944f, 0.866f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f,
  0.787f, 0.787f, 0.787f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint32_t data_B310 = 0x3F4978D5;
static const float data_B314[33] = {
  0.787f, 0.866f, 0.944f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  0.944f, 0.866f, 0.787f, 0.787f, 0.787f, 0.787f, 0.708f, 0.708f, 0.708f, 0.708f
};
static const uint8_t data_B398[12] = {0};
static const float data_B3A4[31] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint32_t data_B420 = 0x3F800000;
static const float data_B424[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint32_t data_B4A8 = 0x3F800000;
static const float data_B4AC[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint32_t data_B530 = 0x3F800000;
static const float data_B534[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint32_t data_B5B8 = 0x41A00000;
static const float data_B5BC[2] = {0.0f, -50.0f};
static const uint32_t data_B5C4 = 0x41A00000;
static const float data_B5C8[2] = {0.0f, -50.0f};
static const uint32_t data_B5D0 = 0xC2200000;
static const float data_B5D4[2] = {0.0f, 80.0f};
static const uint32_t data_B5DC = 0xC1A00000;
static const float data_B5E0[2] = {0.0f, -50.0f};
static const uint32_t data_B5E8 = 0xC1A00000;
static const float data_B5EC[2] = {0.0f, -50.0f};
static const uint32_t data_B5F4 = 0xC2E60000;
static const float data_B5F8[2] = {0.0f, 175.0f};
static const uint32_t data_B600 = 0x42C80000;
static const float data_B604[2] = {0.0f, -40.0f};
static const uint8_t data_B60C[12] = {0};
static const uint32_t data_B618 = 0x3F800000;
static const float data_B61C[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint32_t data_B6A0 = 0x3F800000;
static const float data_B6A4[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint8_t data_B728[16] = {0};
static const float data_B738[30] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint8_t data_B7B0[84] = {0};
typedef struct Row_B804 { float w0; float w1; float w2; int32_t w3; } Row_B804;
static const Row_B804 data_B804[3] = {
  {0.787f, 0.787f, 0.787f, 0},
  {0.0f, 0.0f, 0.0f, 0},
  {0.0f, 0.0f, 0.0f, 0}
};
static const int32_t data_B834 = 0;
static const uint32_t data_B838 = 0x3F800000;
static const float data_B83C[33] = {
  0.944f, 0.866f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f,
  0.787f, 0.787f, 0.787f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint32_t data_B8C0 = 0x3F4978D5;
static const float data_B8C4[33] = {
  0.787f, 0.866f, 0.944f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  0.944f, 0.866f, 0.787f, 0.787f, 0.787f, 0.787f, 0.708f, 0.708f, 0.708f, 0.708f
};
static const uint32_t data_B948 = 0x3F800000;
static const float data_B94C[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint32_t data_B9D0 = 0x3F800000;
static const float data_B9D4[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint32_t data_BA58 = 0xC2A50000;
static const float data_BA5C[2] = {0.0f, 68.0f};
static const uint32_t data_BA64 = 0x3F800000;
static const float data_BA68[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint32_t data_BAEC = 0xC1D00000;
static const float data_BAF0[2] = {0.0f, 76.5f};
static const uint32_t data_BAF8 = 0x3F800000;
static const float data_BAFC[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint32_t data_BB80 = 0x41A00000;
static const float data_BB84[2] = {0.0f, -50.0f};
static const uint32_t data_BB8C = 0x41A00000;
static const float data_BB90[2] = {0.0f, -50.0f};
static const uint32_t data_BB98 = 0xC2200000;
static const float data_BB9C[2] = {0.0f, 80.0f};
static const uint32_t data_BBA4 = 0xC1A00000;
static const float data_BBA8[2] = {0.0f, -50.0f};
static const uint32_t data_BBB0 = 0xC1A00000;
static const float data_BBB4[2] = {0.0f, -50.0f};
static const uint32_t data_BBBC = 0xC2E60000;
static const float data_BBC0[5] = {0.0f, 175.0f, 100.0f, 0.0f, -40.0f};
static const uint8_t data_BBD4[12] = {0};
static const uint32_t data_BBE0 = 0x3F800000;
static const float data_BBE4[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint32_t data_BC68 = 0x3F800000;
static const float data_BC6C[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint8_t data_BCF0[16] = {0};
static const float data_BD00[30] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint8_t data_BD78[84] = {0};
typedef struct Row_BDCC { float w0; float w1; float w2; int32_t w3; } Row_BDCC;
static const Row_BDCC data_BDCC[3] = {
  {0.787f, 0.787f, 0.787f, 0},
  {0.0f, 0.0f, 0.0f, 0},
  {0.0f, 0.0f, 0.0f, 0}
};
static const int32_t data_BDFC = 0;
static const uint32_t data_BE00 = 0x3F800000;
static const float data_BE04[33] = {
  0.944f, 0.866f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f,
  0.787f, 0.787f, 0.787f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint32_t data_BE88 = 0x3F4978D5;
static const float data_BE8C[33] = {
  0.787f, 0.866f, 0.944f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  0.944f, 0.866f, 0.787f, 0.787f, 0.787f, 0.787f, 0.708f, 0.708f, 0.708f, 0.708f
};
static const uint32_t data_BF10 = 0x3F800000;
static const float data_BF14[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const int32_t data_BF98[57] = {
  0, 0, -5461, 0, -2366, -1092, 0, -182, 1092, 0, -546, -6735, 0, 0, -4004, 0, 0, -6553, 0, -182, 1274, 0, 0, -13289, 0, 0, -5279, 0, 0, -8009, 0, 0,
  1456, 0, 182, -8738, 0, 0, -6007, 0, 0, -8738, 0, 182, 1638, 0, -728, -5279, 0, -1274, -6735, 0, -910, -8738, 0, 0, 1638
};
static const int32_t data_C07C[57] = {
  0, 0, -7281, 0, 0, -4004, 0, 546, 182, 0, 0, -18204, 0, 0, -16930, 0, 0, -12379, 0, 0, 0, 0, 0, -12196, 0, 0, -18204, 0, 0, -13289, 0, 0, 0, 0, 0,
  -11832, 0, 0, -18386, 0, 0, -13835, 0, 0, 0, 0, 910, -12014, 0, -2184, -16748, 0, -2002, -13835, 0, 0, 364
};
static const int32_t data_C160[57] = {
  0, 0, -1820, 0, -546, 2184, 0, -3276, 364, 0, -546, -6735, 0, 0, -3822, 0, 0, -728, 0, -182, 1274, 0, 0, -4187, 0, 0, -5279, 0, 0, -364, 0, 0, 1456,
  0, 182, -8738, 0, 0, -6007, 0, 0, -1638, 0, 0, 1638, 0, -728, -5279, 0, -1274, -6735, 0, 0, -2730, 0, 0, 1638
};
static const int32_t data_C244[69] = {
  0, 0, -1820, 0, -1092, 1092, 0, -182, -546, 0, -546, -6735, 0, 0, -5279, 0, 0, -9102, 0, -182, 1274, 0, 0, -4187, 0, 0, -6735, 0, 0, -10376, 0, 0,
  1456, 0, 182, -3276, 0, 0, -8556, 0, 0, -10012, 0, 0, 1638, 0, -728, -5279, 0, -1274, -9284, 0, 2730, -7827, 0, 910, 1274, -7281, 2184, 5643, 10922,
  2912, 4004, 0, 0, 0, 10922, 2912, 4004
};
static const Vector3i data_C358 = {.x = 0, .y = 2912, .z = 0};
static const int32_t data_C364[9] = {-5095, -4730, 0, -5095, -9281, 0, -3458, -11286, 0};
static const Vector3i data_C388 = {.x = -1092, .y = -2548, .z = -2912};
static const Vector3i data_C394 = {.x = 182, .y = -5459, .z = 0};
static const int32_t data_C3A0[20] = {0, 3640, 0, 0, 60256, 1820, 59528, 59528, 0, 59528, 0, 61895, 0, 0, 3094, 0, 7645, 7281, 0, 4733};
static const uint32_t data_C3F0 = 4;
static const int32_t data_C3F4[143] = {
  24, 20, 0, 0, -1, -1, -1, -1, -1, 1, 3, 64, 24, 20, 0, 0, -1, -1, -1, -1, -1, 5, 7, 256, 24, 20, 0, 0, -1, -1, -1, -1, -1, 9, 11, 4, 24, 20, 0, 0,
  -1, -1, -1, -1, -1, 13, -1, 4, 24, 20, 0, 0, -1, -1, -1, -1, -1, 15, -1, 4, 24, 20, 0, 0, -1, -1, -1, -1, -1, 17, -1, 4, 24, 20, 0, 0, -1, -1, -1,
  -1, -1, -1, -1, 68, 24, 20, 0, 0, -1, -1, -1, -1, -1, -1, -1, 64, 24, 20, 0, 0, -1, -1, -1, -1, -1, -1, -1, 64, 24, 20, 0, 0, -1, 32, -1, 19, -1,
  21, 23, 64, 24, 20, 0, 0, -1, 32, -1, 25, -1, 27, 29, 256, 24, 20, 0, 0, -1, -1, -1, 31, -1, 33, 35
};
static const uint32_t data_C630 = 157;
static const int32_t data_C634[5] = {155, 154, 153, 152, 156};
static const uint32_t data_C648 = 152;
static const int32_t data_C64C[6] = {153, 154, 155, 156, 157, 158};
static const uint32_t data_C664 = 152;
static const int32_t data_C668[4] = {153, 154, 155, 156};
static const uint32_t data_C678 = 152;
static const int32_t data_C67C[4] = {153, 154, 155, 156};
static const uint32_t data_C68C = 152;
static const int32_t data_C690[23] = {153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175};
static const uint32_t data_C6EC = 152;
static const int32_t data_C6F0[2] = {153, 154};
static const uint32_t data_C6F8 = 0x3F800000;
static const float data_C6FC[26] = {
  1.0f, 0.96f, 0.94f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.5f, -0.56f, -0.59f, 1.0f, 0.0f, 1.0f, 1.0f, 0.92f, 0.92f, -98.72f, 0.84f, 61.68f, 5.3f, 0.72f,
  2.7f, 0.6f, -0.16f, -1.0f
};
static const int32_t data_C764 = 4000;
static const float data_C768[13] = {1.0f, 1.0f, 0.92f, 0.92f, 94.34f, 0.99f, 22.47f, 10.0f, 0.65f, 2.7f, -1.0f, 0.2f, -0.5f};
static const int32_t data_C79C = 4000;
static const float data_C7A0[13] = {1.0f, 1.0f, 0.92f, 0.92f, 95.22f, 0.99f, 29.07f, 10.0f, 0.71f, 2.7f, -0.63f, 0.2f, 0.2f};
static const int32_t data_C7D4 = 4000;
static const char data_C7D8[9] __attribute__((scn_string_literal)) = "QTE_MBQC";
static const char data_C7E1[] __attribute__((scn_string_literal)) = "BIKA";
static const char data_C7E6[9] __attribute__((scn_string_literal)) = "QTE_MBQC";
static const char data_C7EF[] __attribute__((scn_string_literal)) = "BIKB";
static const char data_C7F4[9] __attribute__((scn_string_literal)) = "QTE_MBQC";
static const char data_C7FD[] __attribute__((scn_string_literal)) = "BIKC";
static const char data_C802[9] __attribute__((scn_string_literal)) = "QTE_MBQC";
static const char data_C80B[] __attribute__((scn_string_literal)) = "BIKD";
static const char data_C810[9] __attribute__((scn_string_literal)) = "QTE_MBQC";
static const char data_C819[] __attribute__((scn_string_literal)) = "BIKY";
static const char data_C81E[9] __attribute__((scn_string_literal)) = "QTE_MBQC";
static const char data_C827[] __attribute__((scn_string_literal)) = "HAKB";
static const char data_C82C[9] __attribute__((scn_string_literal)) = "QTE_MBQC";
static const char data_C835[] __attribute__((scn_string_literal)) = "HAKA";
static const uint8_t data_C83A[2] = {0};
static const Vector3 data_C83C = {.x = 0.0f, .y = 0.02f, .z = 0.0f};
typedef struct Row_C848 { int32_t w0; float w1; float w2; } Row_C848;
static const Row_C848 data_C848[4] = {
  {0, 0.0f, 0.0f},
  {0, 0.005f, -2.5f},
  {0, 0.0f, 0.3f},
  {0, 0.0f, 0.0f}
};
static const Vector4 data_C878 = {.x = 0.4f, .y = 0.4f, .z = 0.4f, .w = 0.4f};
static const FogColourData data_C888 = {.red = 40.0f, .green = 60.0f, .blue = 0.95654f, .colour = 0xFF080C10U};
static const uint32_t data_C898 = 0xC2CEF333;
static const uint32_t data_C89C[3] = {0, 0x428D1646U, 0};
static const int32_t data_C8A8 = 1;
static const float data_C8AC[4] = {-106.0f, 0.0f, 76.0f, 3.5f};
static const int32_t data_C8BC = 1;
static const float data_C8C0[4] = {-114.0f, 0.0f, 92.0f, 3.5f};
static const int32_t data_C8D0 = 2;
static const float data_C8D4[4] = {-113.0f, 0.0f, 107.0f, 3.5f};
static const int32_t data_C8E4 = 3;
static const float data_C8E8[4] = {-73.0f, 0.0f, 107.0f, 3.5f};
static const int32_t data_C8F8[2] = {4, -11062528};
static const uint32_t data_C900 = 0xC2C20000;
static const float data_C904[3] = {0.0f, 107.0f, 0.0f};
static const int32_t data_C910 = 1;
static const float data_C914[4] = {-74.0f, 0.0f, 107.0f, 1.0f};
static const int32_t data_C924 = 2;
static const float data_C928[4] = {-64.0f, 0.0f, 106.0f, 1.0f};
static const int32_t data_C938 = 3;
static const float data_C93C[4] = {-44.0f, 0.0f, 107.0f, 1.0f};
static const int32_t data_C94C = 5;
static const float data_C950[4] = {-15.3f, 0.0f, 106.8f, 5.0f};
static const int32_t data_C960 = 7;
static const float data_C964[4] = {-8.0f, 0.0f, 98.7f, 5.0f};
static const int32_t data_C974 = 8;
static const float data_C978[4] = {14.2f, 0.0f, 74.2f, 4.0f};
static const int32_t data_C988[2] = {9, -11062528};
static const uint32_t data_C990 = 0xC1980000;
static const float data_C994[3] = {0.0f, 104.0f, 4.0f};
static const int32_t data_C9A0 = 1;
static const float data_C9A4[4] = {20.2f, 0.0f, 73.2f, 4.0f};
static const int32_t data_C9B4 = 2;
static const float data_C9B8[4] = {45.2f, 0.0f, 98.8f, 4.0f};
static const int32_t data_C9C8 = 2;
static const float data_C9CC[4] = {57.0f, 0.0f, 106.7f, 4.0f};
static const int32_t data_C9DC = 2;
static const float data_C9E0[4] = {59.7f, 0.0f, 120.2f, 4.0f};
static const int32_t data_C9F0 = 3;
static const float data_C9F4[4] = {35.0f, 0.0f, 139.0f, 4.0f};
static const int32_t data_CA04 = 4;
static const float data_CA08[4] = {-9.5f, 0.0f, 102.8f, 4.0f};
static const int32_t data_CA18 = 5;
static const float data_CA1C[4] = {14.2f, 0.0f, 74.2f, 4.0f};
static const int32_t data_CA2C[2] = {6, -11062528};
static const uint32_t data_CA34 = 0x42400000;
static const float data_CA38[3] = {0.0f, 127.0f, 0.0f};
static const int32_t data_CA44 = 1;
static const float data_CA48[4] = {39.7f, 0.0f, 132.2f, 4.0f};
static const int32_t data_CA58 = 2;
static const float data_CA5C[4] = {32.3f, 0.0f, 140.0f, 4.0f};
static const int32_t data_CA6C = 3;
static const float data_CA70[4] = {30.8f, 0.0f, 143.9f, 0.0f};
static const int32_t data_CA80[2] = {1, -11062528};
static const uint32_t data_CA88 = 0x42400000;
static const float data_CA8C[3] = {0.0f, 127.0f, 0.0f};
static const int32_t data_CA98 = 1;
static const float data_CA9C[4] = {40.0f, 0.0f, 133.0f, 4.0f};
static const int32_t data_CAAC = 2;
static const float data_CAB0[4] = {32.3f, 0.0f, 140.5f, 4.0f};
static const int32_t data_CAC0 = 3;
static const float data_CAC4[4] = {30.8f, 0.0f, 143.9f, 0.0f};
static const int32_t data_CAD4[2] = {1, -11062528};
static const uint32_t data_CADC = 0x41F66666;
static const float data_CAE0[3] = {0.0f, 143.9f, 0.0f};
static const int32_t data_CAEC = 1;
static const float data_CAF0[4] = {27.3f, 0.0f, 147.4f, 3.0f};
static const int32_t data_CB00 = 2;
static const float data_CB04[4] = {23.6f, 0.0f, 154.1f, 3.0f};
static const int32_t data_CB14 = 3;
static const float data_CB18[4] = {20.4f, 0.0f, 158.7f, 3.0f};
static const int32_t data_CB28 = 3;
static const float data_CB2C[4] = {8.6f, 0.0f, 163.7f, 4.0f};
static const int32_t data_CB3C = 4;
static const float data_CB40[4] = {-1.4f, 0.0f, 152.4f, 5.0f};
static const int32_t data_CB50 = 6;
static const float data_CB54[4] = {-4.8f, 0.0f, 139.0f, 5.0f};
static const int32_t data_CB64 = 6;
static const float data_CB68[4] = {1.9f, 0.0f, 129.8f, 0.0f};
static const int32_t data_CB78[2] = {7, -11062528};
static const uint32_t data_CB80 = 0x41F66666;
static const float data_CB84[3] = {0.0f, 143.9f, 0.0f};
static const int32_t data_CB90 = 1;
static const float data_CB94[4] = {27.3f, 0.0f, 147.4f, 3.0f};
static const int32_t data_CBA4 = 2;
static const float data_CBA8[4] = {21.7f, 0.0f, 154.1f, 3.0f};
static const int32_t data_CBB8 = 3;
static const float data_CBBC[4] = {20.4f, 0.0f, 158.7f, 3.0f};
static const int32_t data_CBCC = 3;
static const float data_CBD0[4] = {8.6f, 0.0f, 163.7f, 4.0f};
static const int32_t data_CBE0 = 4;
static const float data_CBE4[4] = {-1.4f, 0.0f, 152.4f, 5.0f};
static const int32_t data_CBF4 = 6;
static const float data_CBF8[4] = {-4.8f, 0.0f, 139.0f, 5.0f};
static const int32_t data_CC08 = 6;
static const float data_CC0C[4] = {1.9f, 0.0f, 129.8f, 0.0f};
static const int32_t data_CC1C[2] = {7, -11062528};
static const uint32_t data_CC24 = 0xBF800000;
static const float data_CC28[3] = {0.0f, 133.5f, 0.0f};
static const int32_t data_CC34 = 1;
static const float data_CC38[4] = {5.1f, 0.0f, 117.7f, 4.0f};
static const int32_t data_CC48 = 2;
static const float data_CC4C[4] = {1.5f, 0.0f, 111.5f, 0.0f};
static const int32_t data_CC5C[2] = {3, -11062528};
static const uint32_t data_CC64 = 0xBF800000;
typedef struct Row_CC68 { float w0; float w1; float w2; int32_t w3; } Row_CC68;
static const Row_CC68 data_CC68[2] = {
  {0.0f, 133.5f, 0.0f, 1},
  {14.5f, 0.0f, 118.2f, 0}
};
static const int32_t data_CC88[2] = {2, -11062528};
static const uint32_t data_CC90 = 0xBF800000;
static const float data_CC94[3] = {0.0f, 133.5f, 0.0f};
static const int32_t data_CCA0 = 1;
static const float data_CCA4[4] = {10.0f, 0.0f, 123.5f, 4.0f};
static const int32_t data_CCB4 = 2;
static const float data_CCB8[4] = {21.8f, 0.0f, 127.4f, 1.0f};
static const int32_t data_CCC8 = 3;
static const float data_CCCC[4] = {27.9f, 0.0f, 132.4f, 0.0f};
static const int32_t data_CCDC = 4;
static const float data_CCE0[4] = {35.3f, 0.0f, 139.6f, 0.0f};
static const int32_t data_CCF0[2] = {5, -11062528};
static const uint32_t data_CCF8 = 0x3F8CCCCD;
static const float data_CCFC[3] = {0.0f, 113.5f, 0.0f};
static const int32_t data_CD08 = 1;
static const float data_CD0C[4] = {-11.8f, 0.0f, 99.9f, 4.0f};
static const int32_t data_CD1C = 2;
static const float data_CD20[4] = {-21.1f, 0.0f, 106.5f, 4.0f};
static const int32_t data_CD30 = 6;
static const float data_CD34[4] = {-29.6f, 0.0f, 106.6f, 0.0f};
static const int32_t data_CD44[2] = {6, -11062528};
static const uint32_t data_CD4C = 0x3F8CCCCD;
typedef struct Row_CD50 { float w0; float w1; float w2; int32_t w3; } Row_CD50;
static const Row_CD50 data_CD50[2] = {
  {0.0f, 113.5f, 0.0f, 1},
  {-13.8f, 0.0f, 98.5f, 0}
};
static const int32_t data_CD70[2] = {2, -11062528};
static const uint32_t data_CD78 = 0x3F8CCCCD;
static const float data_CD7C[3] = {0.0f, 113.5f, 0.0f};
static const int32_t data_CD88 = 1;
static const float data_CD8C[4] = {-11.8f, 0.0f, 99.9f, 4.0f};
static const int32_t data_CD9C = 2;
static const float data_CDA0[4] = {-5.2f, 0.0f, 95.4f, 4.0f};
static const int32_t data_CDB0 = 3;
static const float data_CDB4[4] = {2.6f, 0.0f, 91.9f, 0.0f};
static const int32_t data_CDC4[2] = {4, -11062528};
static const char data_CDCC[] __attribute__((scn_string_literal)) = "ff&@";
static const uint8_t data_CDD1[16] = {
  0x00, 0x00, 0x00, 0xCD, 0xCC, 0xB7, 0x42, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00
};
static const uint8_t data_CDE1[16] = {
  0x00, 0x32, 0x42, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3A, 0x42, 0x00, 0x00, 0x80, 0x40, 0x02
};
static const uint8_t data_CDF1[16] = {
  0x00, 0x00, 0x00, 0xCD, 0xCC, 0x62, 0x42, 0x00, 0x00, 0x00, 0x00, 0xCD, 0xCC, 0x2A, 0x42, 0x00
};
static const uint8_t data_CE01[16] = {
  0x00, 0x80, 0x40, 0x03, 0x00, 0x00, 0x00, 0x66, 0x66, 0xA0, 0x42, 0x00, 0x00, 0x00, 0x00, 0x33
};
static const uint8_t data_CE11[16] = {
  0x33, 0xAB, 0x41, 0x00, 0x00, 0x00, 0x41, 0x04, 0x00, 0x00, 0x00, 0xCD, 0xCC, 0xAC, 0x42, 0x00
};
static const uint8_t data_CE21[16] = {
  0x00, 0x00, 0x00, 0x33, 0x33, 0xB3, 0x41, 0x00, 0x00, 0x00, 0x41, 0x05, 0x00, 0x00, 0x00, 0x9A
};
static const uint8_t data_CE31[16] = {
  0x99, 0xCA, 0x42, 0x00, 0x00, 0x00, 0x00, 0x66, 0x66, 0xAE, 0x41, 0x00, 0x00, 0x00, 0x00, 0x06
};
static const uint8_t pad_CE41[4] = {0};
static const uint8_t data_CE45[3] = {
  0x33, 0x57, 0xFF
};
static const uint32_t data_CE48 = 0xC2C80000;
static const float data_CE4C[3] = {0.0f, 59.0f, 3.5f};
static const int32_t data_CE58 = 1;
static const float data_CE5C[4] = {-101.0f, 0.0f, 65.0f, 3.5f};
static const int32_t data_CE6C = 1;
static const float data_CE70[4] = {-106.0f, 0.0f, 75.0f, 3.5f};
static const int32_t data_CE80 = 2;
static const float data_CE84[4] = {-110.0f, 0.0f, 80.0f, 3.5f};
static const int32_t data_CE94 = 3;
static const float data_CE98[4] = {-113.0f, 0.0f, 107.0f, 3.5f};
static const int32_t data_CEA8 = 4;
static const float data_CEAC[4] = {-73.0f, 0.0f, 107.0f, 3.5f};
static const int32_t data_CEBC[2] = {5, -11062528};
static const uint32_t data_CEC4 = 0xC2AA0000;
static const float data_CEC8[3] = {0.0f, 106.0f, 0.0f};
static const int32_t data_CED4 = 1;
static const float data_CED8[4] = {-44.0f, 0.0f, 107.2f, 1.0f};
static const int32_t data_CEE8 = 2;
static const float data_CEEC[4] = {-22.0f, 0.0f, 106.0f, 1.0f};
static const int32_t data_CEFC = 3;
static const float data_CF00[4] = {-15.3f, 0.0f, 107.0f, 5.0f};
static const int32_t data_CF10 = 4;
static const float data_CF14[4] = {-9.0f, 0.0f, 99.7f, 5.0f};
static const int32_t data_CF24 = 5;
static const float data_CF28[4] = {14.2f, 0.0f, 74.2f, 4.0f};
static const int32_t data_CF38[2] = {6, -11062528};
static const char data_CF40[] __attribute__((scn_string_literal)) = "33#A";
static const uint8_t data_CF45[16] = {
  0x00, 0x00, 0x00, 0x66, 0x66, 0xA6, 0x42, 0x00, 0x00, 0x80, 0x40, 0x01, 0x00, 0x00, 0x00, 0x9A
};
static const uint8_t data_CF55[16] = {
  0x99, 0xA1, 0x41, 0x00, 0x00, 0x00, 0x00, 0x66, 0x66, 0x92, 0x42, 0x00, 0x00, 0x80, 0x40, 0x02
};
static const uint8_t pad_CF65[5] = {0};
static const uint8_t data_CF6A[16] = {
  0x7A, 0x42, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xEA, 0x42, 0x00, 0x00, 0x80, 0x40, 0x03, 0x00
};
static const uint8_t pad_CF7A[4] = {0};
static const uint8_t data_CF7E[16] = {
  0x16, 0x42, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x43, 0x00, 0x00, 0x80, 0x40, 0x04, 0x00
};
static const uint8_t data_CF8E[16] = {
  0x00, 0x00, 0x66, 0x66, 0xB6, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1C, 0x43, 0x00, 0x00
};
static const uint8_t data_CF9E[16] = {
  0x80, 0x40, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
static const uint8_t data_CFAE[16] = {
  0x27, 0x43, 0x00, 0x00, 0x80, 0x40, 0x06, 0x00, 0x00, 0x00, 0x33, 0x33, 0x2B, 0xC1, 0x00, 0x00
};
static const uint8_t pad_CFBE[4] = {0};
static const uint8_t data_CFC2[16] = {
  0x0D, 0x43, 0x00, 0x00, 0x80, 0x40, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x41, 0x00, 0x00
};
static const uint8_t data_CFD2[16] = {
  0x00, 0x00, 0x33, 0x33, 0xF3, 0x42, 0x00, 0x00, 0x80, 0x40, 0x08, 0x00, 0x00, 0x00, 0x33, 0x33
};
static const uint8_t data_CFE2[16] = {
  0x33, 0xC1, 0x00, 0x00, 0x00, 0x00, 0x66, 0x66, 0xC6, 0x42, 0x00, 0x00, 0x80, 0x40, 0x09, 0x00
};
static const uint8_t pad_CFF2[4] = {0};
static const uint8_t data_CFF6[16] = {
  0x94, 0x41, 0x00, 0x00, 0x00, 0x00, 0xCD, 0xCC, 0x8E, 0x42, 0x00, 0x00, 0x80, 0x40, 0x0A, 0x00
};
static const uint8_t data_D006[16] = {
  0x00, 0x00, 0x66, 0x66, 0x40, 0x42, 0x00, 0x00, 0x00, 0x00, 0xCD, 0xCC, 0x30, 0x42, 0x00, 0x00
};
static const uint8_t data_D016[10] = {
  0x00, 0x00, 0x0B, 0x00, 0x00, 0x00, 0x00, 0x33, 0x57, 0xFF
};
static const uint32_t data_D020 = 0x41966666;
static const float data_D024[3] = {0.0f, 71.0f, 0.0f};
static const int32_t data_D030 = 1;
static const float data_D034[4] = {44.5f, 0.0f, 46.5f, 8.0f};
static const int32_t data_D044 = 2;
static const float data_D048[4] = {66.2f, 0.0f, 35.0f, 8.0f};
static const int32_t data_D058 = 3;
static const float data_D05C[4] = {80.2f, 0.0f, 21.4f, 8.0f};
static const int32_t data_D06C = 4;
static const float data_D070[4] = {86.4f, 0.0f, 22.4f, 8.0f};
static const int32_t data_D080 = 5;
static const float data_D084[4] = {92.1f, 0.0f, 28.6f, 0.0f};
static const int32_t data_D094[2] = {6, -11062528};
static const uint32_t data_D09C = 0xC2BE6666;
static const float data_D0A0[3] = {0.0f, 61.2f, 3.5f};
static const int32_t data_D0AC = 1;
static const float data_D0B0[4] = {-99.0f, 0.0f, 64.0f, 3.5f};
static const int32_t data_D0C0 = 1;
static const float data_D0C4[4] = {-106.0f, 0.0f, 70.6f, 3.5f};
static const int32_t data_D0D4 = 2;
static const float data_D0D8[4] = {-109.0f, 0.0f, 82.0f, 3.5f};
static const int32_t data_D0E8 = 3;
static const float data_D0EC[4] = {-113.0f, 0.0f, 107.0f, 3.5f};
static const int32_t data_D0FC = 4;
static const float data_D100[4] = {-73.0f, 0.0f, 107.0f, 3.5f};
static const int32_t data_D110[2] = {5, -11062528};
static const uint32_t data_D118 = 0xC2B40000;
static const float data_D11C[3] = {0.0f, 108.0f, 0.0f};
static const int32_t data_D128 = 1;
static const float data_D12C[4] = {-44.0f, 0.0f, 108.2f, 1.0f};
static const int32_t data_D13C = 2;
static const float data_D140[4] = {-22.0f, 0.0f, 108.0f, 1.0f};
static const int32_t data_D150 = 3;
static const float data_D154[4] = {-15.3f, 0.0f, 108.0f, 5.0f};
static const int32_t data_D164 = 4;
static const float data_D168[4] = {-9.0f, 0.0f, 99.7f, 5.0f};
static const int32_t data_D178 = 5;
static const float data_D17C[4] = {14.2f, 0.0f, 74.2f, 4.0f};
static const int32_t data_D18C[2] = {6, -11062528};
static const uint32_t data_D194 = 0xC2C20000;
static const float data_D198[3] = {0.0f, 59.0f, 3.5f};
static const int32_t data_D1A4 = 1;
static const float data_D1A8[4] = {-100.0f, 0.0f, 65.0f, 3.5f};
static const int32_t data_D1B8 = 1;
static const float data_D1BC[4] = {-103.5f, 0.0f, 71.0f, 3.5f};
static const int32_t data_D1CC = 2;
static const float data_D1D0[4] = {-107.0f, 0.0f, 85.0f, 3.5f};
static const int32_t data_D1E0 = 3;
static const float data_D1E4[4] = {-113.0f, 0.0f, 107.0f, 3.5f};
static const int32_t data_D1F4 = 4;
static const float data_D1F8[4] = {-73.0f, 0.0f, 107.0f, 3.5f};
static const int32_t data_D208[2] = {5, -11062528};
static const uint32_t data_D210 = 0xC2C80000;
static const float data_D214[3] = {0.0f, 65.0f, 3.5f};
static const int32_t data_D220 = 1;
static const float data_D224[4] = {-104.0f, 0.0f, 71.0f, 3.5f};
static const int32_t data_D234 = 2;
static const float data_D238[4] = {-106.0f, 0.0f, 75.0f, 3.5f};
static const int32_t data_D248[2] = {3, -11062528};
static const char data_D250[10] __attribute__((scn_string_literal)) = "E1069B001";
static const char data_D25A[10] __attribute__((scn_string_literal)) = "E1069B002";
static const char data_D264[10] __attribute__((scn_string_literal)) = "E1069C001";
static const char data_D26E[10] __attribute__((scn_string_literal)) = "E1069D001";
static const char data_D278[10] __attribute__((scn_string_literal)) = "E1069B003";
static const char data_D282[10] __attribute__((scn_string_literal)) = "E1069B004";
static const char data_D28C[10] __attribute__((scn_string_literal)) = "E1069B004";
static const char data_D296[10] __attribute__((scn_string_literal)) = "E1069B005";
static const char data_D2A0[10] __attribute__((scn_string_literal)) = "E1069C002";
static const char data_D2AA[10] __attribute__((scn_string_literal)) = "E1069A001";
static const char data_D2B4[10] __attribute__((scn_string_literal)) = "E1069C003";
static const char data_D2BE[10] __attribute__((scn_string_literal)) = "E1069B006";
static const char data_D2C8[10] __attribute__((scn_string_literal)) = "E1069A002";
static const char data_D2D2[10] __attribute__((scn_string_literal)) = "E1069B007";
static const char data_D2DC[10] __attribute__((scn_string_literal)) = "E1069A003";
static const char data_D2E6[10] __attribute__((scn_string_literal)) = "E1069B008";
static const char data_D2F0[10] __attribute__((scn_string_literal)) = "E1069A004";
static const char data_D2FA[10] __attribute__((scn_string_literal)) = "E1069B009";
static const char data_D304[10] __attribute__((scn_string_literal)) = "E1069B010";
static const char data_D30E[10] __attribute__((scn_string_literal)) = "E1069B011";
static const char data_D318[10] __attribute__((scn_string_literal)) = "E1069E001";
static const char data_D322[10] __attribute__((scn_string_literal)) = "E1069F001";
static const char data_D32C[10] __attribute__((scn_string_literal)) = "E1069B012";
static const char data_D336[10] __attribute__((scn_string_literal)) = "E1069A005";
static const char data_D340[9] __attribute__((scn_string_literal)) = "オート０";
static const char data_D349[9] __attribute__((scn_string_literal)) = "オート１";
static const char data_D352[9] __attribute__((scn_string_literal)) = "オート３";
static const char data_D35B[9] __attribute__((scn_string_literal)) = "オート４";
static const char data_D364[7] __attribute__((scn_string_literal)) = "開始０";
static const char data_D36B[7] __attribute__((scn_string_literal)) = "成功０";
static const char data_D372[7] __attribute__((scn_string_literal)) = "失敗０";
static const char data_D379[7] __attribute__((scn_string_literal)) = "死亡０";
static const char data_D380[7] __attribute__((scn_string_literal)) = "開始１";
static const char data_D387[7] __attribute__((scn_string_literal)) = "成功１";
static const char data_D38E[7] __attribute__((scn_string_literal)) = "失敗１";
static const char data_D395[7] __attribute__((scn_string_literal)) = "開始２";
static const char data_D39C[7] __attribute__((scn_string_literal)) = "成功２";
static const char data_D3A3[7] __attribute__((scn_string_literal)) = "失敗２";
static const char data_D3AA[7] __attribute__((scn_string_literal)) = "開始３";
static const char data_D3B1[7] __attribute__((scn_string_literal)) = "成功３";
static const char data_D3B8[7] __attribute__((scn_string_literal)) = "失敗３";
static const char data_D3BF[7] __attribute__((scn_string_literal)) = "開始４";
static const char data_D3C6[7] __attribute__((scn_string_literal)) = "成功４";
static const char data_D3CD[7] __attribute__((scn_string_literal)) = "失敗４";
static const char data_D3D4[7] __attribute__((scn_string_literal)) = "開始５";
static const char data_D3DB[7] __attribute__((scn_string_literal)) = "成功５";
static const char data_D3E2[7] __attribute__((scn_string_literal)) = "失敗５";
static const char data_D3E9[7] __attribute__((scn_string_literal)) = "開始６";
static const char data_D3F0[7] __attribute__((scn_string_literal)) = "成功６";
static const char data_D3F7[7] __attribute__((scn_string_literal)) = "失敗６";
static const char data_D3FE[7] __attribute__((scn_string_literal)) = "開始７";
static const char data_D405[7] __attribute__((scn_string_literal)) = "成功７";
static const char data_D40C[7] __attribute__((scn_string_literal)) = "失敗７";
static const char data_D413[7] __attribute__((scn_string_literal)) = "開始８";
static const char data_D41A[7] __attribute__((scn_string_literal)) = "成功８";
static const char data_D421[7] __attribute__((scn_string_literal)) = "失敗８";
static const char data_D428[7] __attribute__((scn_string_literal)) = "開始９";
static const char data_D42F[7] __attribute__((scn_string_literal)) = "成功９";
static const char data_D436[7] __attribute__((scn_string_literal)) = "失敗９";
static const char data_D43D[9] __attribute__((scn_string_literal)) = "開始１０";
static const char data_D446[9] __attribute__((scn_string_literal)) = "成功１０";
static const char data_D44F[9] __attribute__((scn_string_literal)) = "失敗１０";
static const char data_D458[9] __attribute__((scn_string_literal)) = "開始１１";
static const char data_D461[9] __attribute__((scn_string_literal)) = "成功１１";
static const char data_D46A[9] __attribute__((scn_string_literal)) = "失敗１１";
static const char data_D473[9] __attribute__((scn_string_literal)) = "開始１２";
static const char data_D47C[9] __attribute__((scn_string_literal)) = "成功１２";
static const char data_D485[9] __attribute__((scn_string_literal)) = "失敗１２";
static const char data_D48E[9] __attribute__((scn_string_literal)) = "開始１３";
static const char data_D497[9] __attribute__((scn_string_literal)) = "成功１３";
static const char data_D4A0[9] __attribute__((scn_string_literal)) = "失敗１３";
static const char data_D4A9[9] __attribute__((scn_string_literal)) = "開始１４";
static const char data_D4B2[9] __attribute__((scn_string_literal)) = "成功１４";
static const char data_D4BB[9] __attribute__((scn_string_literal)) = "失敗１４";
static const Vector3 data_D4C4 = {.x = 0.0f, .y = 0.0f, .z = 0.0f};
static const Vector3i data_D4D0 = {.x = 0, .y = 0, .z = 0};
static const int32_t data_D4DC[10] = {0, 4551, 0, 0, 364, 0, 5643, 4369, 0, 12743};
static const int32_t data_D504[10] = {0, 60984, 0, 0, 65171, 0, 59892, 61166, 0, 52792};
static const char data_D52C[16] __attribute__((scn_string_literal)) = "PRESS A BUTTON";
static const uint32_t data_D53C = 0x41966666;
static const float data_D540[2] = {0.0f, 71.0f};
static const char data_D548[] __attribute__((scn_string_literal)) = "ff&@";
static const uint8_t data_D54D[7] = {
  0x00, 0x00, 0x00, 0xCD, 0xCC, 0xB7, 0x42
};
static const int32_t data_D554[10] = {0, 4551, 0, 0, 364, 0, 5643, 4369, 0, 12743};
static const int32_t data_D57C[10] = {0, 60984, 0, 0, 65171, 0, 59892, 61166, 0, 52792};
static const Vector3 data_D5A4 = {.x = 111.0f, .y = 0.0f, .z = 27.3f};
static const char data_D5B0[11] __attribute__((scn_string_literal)) = "bgm013.snd";
static const char data_D5BB[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_D5C0[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_D5C5[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_D5CA[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_D5CF[11] __attribute__((scn_string_literal)) = "amb013.snd";
static const char data_D5DA[6] __attribute__((scn_string_literal)) = "E1069";
static const char data_D5E0[] __attribute__((scn_string_literal)) = "scene/03/MFSY";
static const char data_D5EE[] __attribute__((scn_string_literal)) = "MBQC";
static const char data_D5F3[11] __attribute__((scn_string_literal)) = "bgm076.snd";
static const char data_D5FE[12] __attribute__((scn_string_literal)) = "e1biche.snd";
static const char data_D60A[11] __attribute__((scn_string_literal)) = "qtebat.snd";
static const char data_D615[11] __attribute__((scn_string_literal)) = "M_MBQC.BIN";
static const char data_D620[] __attribute__((scn_string_literal)) = "scene/03/MFSY";
static const char data_D62E[] __attribute__((scn_string_literal)) = "QTE_MBQC";
static const char data_D637[] __attribute__((scn_string_literal)) = "QTE_MBQC";
static const char data_D640[8] __attribute__((scn_string_literal)) = "FREE";
static const FogColourData data_D648 = {.red = 10.0f, .green = 800.0f, .blue = 0.95654f, .colour = 0xFF081222U};
static const float data_D658[3] = {10.0f, 30.0f, 0.95654f};
static const int32_t data_D664 = -16249840;
static const char data_D668[13] __attribute__((scn_string_literal)) = "battle_1.snd";
static const char data_D675[13] __attribute__((scn_string_literal)) = "seqdata0.bin";
static const char data_D682[14] __attribute__((scn_string_literal)) = "scene/03/MBQC";
static const char data_D690[11] __attribute__((scn_string_literal)) = "M_BIKE.BIN";
static const char data_D69B[11] __attribute__((scn_string_literal)) = "M_MB9Q.BIN";
static const char data_D6A6[12] __attribute__((scn_string_literal)) = "M_01CBK.BIN";
static const char data_D6B2[11] __attribute__((scn_string_literal)) = "M_FGT1.BIN";
static const char data_D6BD[] __attribute__((scn_string_literal)) = "scene/03/MFSY";
static const char data_D6CB[] __attribute__((scn_string_literal)) = "BAT_MBQC";
static const char data_D6D4[9] __attribute__((scn_string_literal)) = "BAT_MBQC";
static const char data_D6DD[13] __attribute__((scn_string_literal)) = "seqdata0.bin";
static const char data_D6EA[9] __attribute__((scn_string_literal)) = "BAT_MBQC";
static const char data_D6F3[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_D6F8[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_D6FD[11] __attribute__((scn_string_literal)) = "a1_cbk.snd";
static const char data_D708[11] __attribute__((scn_string_literal)) = "bgm063.snd";
static const char data_D713[6] __attribute__((scn_string_literal)) = "01CBK";
static const char data_D719[6] __attribute__((scn_string_literal)) = "01CBK";
static const char data_D71F[] __attribute__((scn_string_literal)) = "BAT_MBQC";
static const char data_D728[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_D72D[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_D732[] __attribute__((scn_string_literal)) = "MBQC";
static const char data_D737[9] __attribute__((scn_string_literal)) = "E1069";
static const Vector3 data_D740 = {.x = 0.0f, .y = 0.0f, .z = 0.0f};
static const Vector3i data_D74C = {.x = 0, .y = 0, .z = 0};
static const Vector3 data_D758 = {.x = 0.0f, .y = 0.0f, .z = 0.0f};
static const Vector3i data_D764 = {.x = 0, .y = 0, .z = 0};
static const uint8_t data_00D770[24] = {0};
static const Vector3 data_D788 = {.x = 0.0f, .y = 0.0f, .z = 0.0f};
static const Vector3i data_D794 = {.x = 0, .y = 0, .z = 0};
static const Vector3 data_D7A0 = {.x = 0.0f, .y = 0.005f, .z = -2.5f};
static const Vector3 data_D7AC = {.x = 0.9f, .y = 0.9f, .z = 0.9f};
static const Vector3 data_D7B8 = {.x = 0.0f, .y = 0.0f, .z = 0.0f};
static const Vector3i data_D7C4 = {.x = 0, .y = 0, .z = 0};
static const float data_D7D0[3] = {0.0f, 0.01f, -2.5f};
static const Vector3 data_D7DC = {.x = 0.9f, .y = 0.9f, .z = 0.9f};
static const Vector3 data_D7E8 = {.x = 0.0f, .y = 0.1f, .z = -0.1f};
static const Vector3 data_D7F4 = {.x = 0.0f, .y = 0.0f, .z = 0.0f};
static const Vector3i data_D800 = {.x = 0, .y = 0, .z = 0};
static const Vector3 data_D80C = {.x = 0.9f, .y = 0.9f, .z = 0.9f};

static void script_0030(void) {
  Script_ambSoundUtils(1, 1, &data_AEC8, 135.0f, 280.0f, 1.0f, 0, &data_AF58);
  Script_ambSoundUtils(2, 1, &data_AED4, 135.0f, 60.0f, 1.0f, 0, &data_AFE0);
  Script_ambSoundUtils(3, 0, &data_AEE0, 50.0f, 170.0f, 0.8f, 0, data_B068);
  Script_ambSoundUtils(4, 0, &data_AEEC, 10.0f, 5.0f, 1.0f, 0, &data_B0F0);
  Script_ambSoundUtils(5, 0, data_AEF8, 8.0f, 5.0f, 1.0f, 0, &data_B178);
  Script_ambSoundUtils(6, 1, &data_AF04, 170.0f, 100.0f, 1.0f, 2, data_B200);
  Script_ambSoundUtils(7, 1, &data_AF10, 170.0f, 100.0f, 1.0f, 0, &data_B288);
  Script_ambSoundUtils(8, 1, &data_AF1C, 40.0f, 70.0f, 0.8f, 0, &data_B310);
  Script_ambSoundUtils(9, 0, &data_AF28, 3.0f, 60.0f, 0.8f, 0, data_B398);
  Script_ambSoundUtils(10, 0, &data_AF34, 60.0f, 35.0f, 0.7f, 0, &data_B420);
  Script_ambSoundUtils(13, 0, &data_AF40, 8.0f, 5.0f, 1.0f, 0, &data_B4A8);
  Script_ambSoundUtils(15, 1, data_AF4C, 300.0f, 0.0f, 1.0f, 3, &data_B530);
  Script_Ambient1SoundUtils(0, 23976);
  return;
}

static void script_01EA(void) {
  Script_ambSoundUtils(1, 1, &data_B5B8, 135.0f, 280.0f, 1.0f, 0, &data_B618);
  Script_ambSoundUtils(2, 1, &data_B5C4, 135.0f, 60.0f, 1.0f, 0, &data_B6A0);
  Script_ambSoundUtils(3, 0, &data_B5D0, 50.0f, 170.0f, 0.8f, 0, data_B728);
  Script_ambSoundUtils(6, 1, &data_B5DC, 170.0f, 100.0f, 1.0f, 2, data_B7B0);
  Script_ambSoundUtils(7, 1, &data_B5E8, 170.0f, 100.0f, 1.0f, 0, &data_B838);
  Script_ambSoundUtils(8, 1, &data_B5F4, 40.0f, 70.0f, 0.8f, 0, &data_B8C0);
  Script_ambSoundUtils(10, 0, &data_B600, 60.0f, 35.0f, 0.7f, 0, &data_B948);
  Script_ambSoundUtils(15, 1, data_B60C, 300.0f, 0.0f, 1.0f, 3, &data_B9D0);
  Script_Ambient1SoundUtils(0, 23976);
  return;
}

static void script_0314(void) {
  Script_ambSoundUtils(1, 0, &data_BA58, 1.0f, 10.0f, 1.0f, 0, &data_BA64);
  Script_Ambient1SoundUtils(0, 23976);
  return;
}

static void script_0345(void) {
  Script_ambSoundUtils(1, 0, &data_BAEC, 1.0f, 10.0f, 1.0f, 0, &data_BAF8);
  Script_Ambient1SoundUtils(0, 23976);
  return;
}

static void script_0376(void) {
  Script_ambSoundUtils(1, 1, &data_BB80, 135.0f, 280.0f, 1.0f, 0, &data_BBE0);
  Script_ambSoundUtils(2, 1, &data_BB8C, 135.0f, 60.0f, 1.0f, 0, &data_BC68);
  Script_ambSoundUtils(3, 0, &data_BB98, 50.0f, 170.0f, 0.8f, 0, data_BCF0);
  Script_ambSoundUtils(6, 1, &data_BBA4, 170.0f, 100.0f, 1.0f, 2, data_BD78);
  Script_ambSoundUtils(7, 1, &data_BBB0, 170.0f, 100.0f, 1.0f, 0, &data_BE00);
  Script_ambSoundUtils(8, 1, &data_BBBC, 40.0f, 70.0f, 0.8f, 0, &data_BE88);
  Script_ambSoundUtils(15, 1, data_BBD4, 300.0f, 0.0f, 1.0f, 3, &data_BF10);
  Script_Ambient1SoundUtils(0, 23976);
  return;
}

static void script_047C(void) {
  script_0030();
  return;
}

static int32_t script_0484(int32_t arg0, int32_t arg1) {
  if (arg1 > 0) {
    SCNF_WaitFrames_0000(arg1);
  }
  Script_executeAicaCommand(arg0, 0, 0);
  return Script_srchControl(0);
}

static int32_t script_04A8(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  task_20[arg1] = 0;
  task_30[arg1] = 0;
  task_40[arg1] = 0;
  task_50[arg1] = arg2;
  task_60[arg1] = arg3;
  task_70 = 0;
  Script_CharHandlers(arg0, 0, task_50[arg1], 16);
  Script_CharHandlers(arg0, 1, task_60[arg1], 16);
  for (;;) {
    task_20[arg1] = 0;
    while (task_20[arg1] == 0) {
      SCNF_WaitFrames_0000(1);
    }
    task_70 = task_40[arg1] | 0x40000000;
    Script_scenCreateTask(arg4, 3, arg0, arg1, task_20[arg1] + task_30[arg1]);
  }
  end:;
}

static int32_t script_057F(int32_t arg0, int32_t arg1, int32_t arg2, float arg3, int32_t arg4, int32_t arg5) {
  int32_t local_FC;
  local_FC = task_70;
  if (arg1 > 0) {
    SCNF_WaitFrames_0000(arg1);
  }
  Script_motmGetCharaFlags(arg4, 0, 0);
  Script_motmToggleFlags(arg4, 1, 0x40);
  Script_PlayCharaMotion(arg4, arg0, arg2, -1.0f, -1.0f, 1.0f, 0);
  Script_SetCharaMotionPlaybackRate(arg4, arg3);
  if (local_FC & -0x80000000) {
    Script_motmToggleFlags(arg4, 1, 0x80);
  }
  if (local_FC & 0x40000000) {
    Script_charaToggleMotEnabled(arg4, 1);
  }
  if (local_FC & 0x20000000) {
    Script_charaToggleMotEnabled(arg4, 0);
  }
  return Script_srchControl(0);
}

static void script_0616(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  if (arg3 > 0) {
    SCNF_WaitFrames_0000(arg3);
  }
  Script_CharHandlers(arg0, 2, arg2, 16);
  if (arg4 > 0) {
    SCNF_WaitFrames_0000(arg4);
  }
  Script_CharHandlers(arg0, 0, task_50[arg1], 16);
  Script_CharHandlers(arg0, 1, task_60[arg1], 16);
  Script_srchControl(0);
  return;
}

static void script_0678(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  local_FC = task_70;
  if (arg2 > 0) {
    SCNF_WaitFrames_0000(arg2);
  }
  if (local_FC & 0x10000000) {
    Script_GetDataFromFightProgram(arg0, arg1);
  }
  if (arg3 > 0) {
    SCNF_WaitFrames_0000(arg3);
  }
  if (local_FC & 0x10000000) {
    Script_GetDataFromFightProgram(arg0, 0);
  }
  Script_srchControl(0);
  return;
}

static void script_06D4(int32_t arg0, float arg1, int32_t arg2) {
  int32_t local_FC;
  Vector3i local_F0;
  local_FC = task_70;
  if (arg2 > 0) {
    SCNF_WaitFrames_0000(arg2);
  }
  if (local_FC & -0x80000000) {
    arg1 = (float)0 - arg1;
  }
  (&local_F0)->y = (int32_t)(arg1 / 360.0f * (float)0x10000);
  Script_EVT_SetRotation(arg0, -0x70000000, &local_F0, 0);
  Script_srchControl(0);
  return;
}

static void script_0731(int32_t arg0) {
  while (Script_IsCharaMotionFinished(arg0) == 0) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static int32_t script_074D(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, void *arg4, int32_t arg5) {
  int32_t local_FC;
  task_A4[arg1] = Script_scenCreateTask(script_04A8, 5, arg0, arg1, arg2, arg3, arg5);
  task_74[arg1] = arg0;
  task_84[arg1] = 0;
  task_94[arg1] = 0;
  task_B4 = 4;
  for (;;) {
    SCNF_WaitFrames_0000(1);
    if (task_84[arg1]) {
      local_FC = task_94[arg1] * 12;
      {
        int32_t local_F8;
        local_F8 = Script_ScenDataUtil(arg4, 4, local_FC + 5);
        if (local_F8 > 0) {
          SCNF_WaitFrames_0000(local_F8);
        }
      }
      {
        int32_t local_F8;
        local_F8 = Script_ScenDataUtil(arg4, 4, local_FC + 8);
        if (local_F8 > 0) {
          task_20[arg1] = local_F8;
        }
      }
      {
        int32_t local_F8;
        local_F8 = Script_ScenDataUtil(arg4, 4, local_FC + 6);
        if (local_F8 > 0) {
          SCNF_WaitFrames_0000(local_F8);
        }
      }
      if (task_30[arg1] == 0) {
        Script_scenCreateTask(script_0ABD, 4, Script_ScenDataUtil(arg4, 4, local_FC), Script_ScenDataUtil(arg4, 4, local_FC + 1), Script_ScenDataUtil(arg4, 4, local_FC + 2), Script_ScenDataUtil(arg4, 4, local_FC + 3));
      }
      {
        int32_t local_F8;
        local_F8 = Script_ScenDataUtil(arg4, 4, local_FC + 7);
        if (local_F8 > 0) {
          SCNF_WaitFrames_0000(local_F8);
        }
      }
      {
        int32_t local_F8;
        local_F8 = Script_ScenDataUtil(arg4, 4, local_FC + 9);
        if (local_F8 > 0) {
          task_20[arg1] = local_F8;
        }
      }
      script_09BB();
      {
        int32_t local_F8;
        if (task_C0 == Script_ScenDataUtil(arg4, 4, local_FC)) {
          task_BC = 1;
          local_F8 = Script_ScenDataUtil(arg4, 4, local_FC + 10);
        } else {
          if (task_30[arg1] == 0) {
            task_C8 = task_C8 + Script_ScenDataUtil(arg4, 4, local_FC + 4);
          }
          task_BC = 0;
          local_F8 = Script_ScenDataUtil(arg4, 4, local_FC + 11);
        }
        if (local_F8 > 0) {
          task_20[arg1] = local_F8;
        }
      }
      script_09AB(arg1);
    }
  }
  end:;
}

static void script_0974(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  task_84[arg0] = 1;
  task_94[arg0] = arg1;
  task_30[arg0] = arg2;
  task_40[arg0] = arg3;
  return;
}

static void script_09AB(int32_t arg0) {
  task_84[arg0] = 0;
  return;
}

static void script_09BB(void) {
  SCNF_WaitFrames_0000(2);
  while (task_C0 == 0) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_09DB(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  script_0974(arg0, arg1, 0, arg3);
  script_0974(arg2, arg1, 1, arg3);
  while (task_84[arg0]) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_0A15(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  script_0974(arg0, arg1, 0, arg3);
  script_0974(arg2, arg1, 1, arg3);
  script_0974(arg2 + 1, arg1, 2, arg3);
  while (task_84[arg0]) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_0A61(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  script_0974(arg0, arg1, 0, arg4);
  script_0974(arg2, arg1, 1, arg4);
  script_0974(arg3, arg1, 2, arg4);
  while (task_84[arg0]) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_0AAA(int32_t arg0) {
  task_B4 = arg0;
  Script_SetSfxOverlayTimeoutTuning(arg0, SfxMode_SetBoostStepFrames);
  return;
}

static int32_t script_0ABD(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  int32_t local_F8;
  local_FC = arg1 + arg3;
  local_F8 = arg2 + arg3;
  Script_scenCreateTask(script_0B20, 2, arg1, arg3);
  Script_SetSfxOverlayTimeoutTuning(local_FC, SfxMode_SetMaxFrames);
  if (arg0 == 68) {
    SCNF_UIUtils(0x44, local_F8, 5, 0, 1, 0);
  } else {
    SCNF_UIUtils(arg0, local_F8, 2, 0, 1, 0);
  }
  return Script_srchControl(0);
}

static int32_t script_0B20(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  int32_t local_F8;
  task_C4 = arg0;
  task_C0 = 0;
  task_B8 = 0;
  local_F8 = 0xFFFF;
  for (;;) {
    local_FC = SCNF_UIUtils(0, 0, 0, 0, 0, -1);
    task_B8 = task_B8 + 1;
    if (local_FC != 0xFFFF) {
      if (local_FC != 0xA000) {
        local_F8 = local_FC;
      }
    }
    if (arg0 == -1) {
      if (local_F8 != 0xFFFF) {
        break;
      }
    } else {
      if (task_B8 >= arg0 + arg1 + 4) {
        break;
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  if ((local_F8 & 0xFFF) == 0) {
    local_F8 = 0xFFFF;
  }
  task_C0 = local_F8 & 0xFFF;
  return Script_srchControl(0);
}

static void script_0BE1(int32_t arg0, int32_t arg1, int32_t arg2) {
  {
    Vector3 local_F4;
    Script_EVT_GetPosition(arg0, -1, &local_F4, 0);
    task_CC[arg1] = arg0;
    task_DC[arg1][0] = (&local_F4)->x;
    task_DC[arg1][1] = (&local_F4)->y;
    task_DC[arg1][2] = (&local_F4)->z;
  }
  task_10C[arg1] = 0.0f;
  for (;;) {
    Vector3 local_F4;
    SCNF_WaitFrames_0000(1);
    Script_EVT_GetPosition(arg0, -1, &local_F4, 0);
    task_DC[arg1][0] = (&local_F4)->x;
    task_DC[arg1][1] = (&local_F4)->y;
    task_DC[arg1][2] = (&local_F4)->z;
    task_10C[arg1] = Script_MathUtils(SCRIPT_MATH_VEC2_LENGTH, task_DC[arg1][0] - task_DC[arg2][0], task_DC[arg1][2] - task_DC[arg2][2]);
  }
  return;
}

static void script_0CCF(void) {
  task_11C = 0.0f;
  task_120 = 0.0f;
  task_124 = 0.0f;
  for (;;) {
    SCNF_WaitFrames_0000(1);
    if (Script_GetInputData(2, 0) & 4) {
      if (Script_GetInputData(0, 2) & 0x4000) {
        task_11C = task_11C - 0.01f;
      }
      if (Script_GetInputData(0, 2) & 0x8000) {
        task_11C = task_11C + 0.01f;
      }
      if (Script_GetInputData(0, 2) & 0x1000) {
        task_120 = task_120 - 0.01f;
      }
      if (Script_GetInputData(0, 2) & 0x2000) {
        task_120 = task_120 + 0.01f;
      }
      if (Script_GetInputData(0, 2) & 0x40) {
        task_124 = task_124 - 1.0f;
      }
      if (Script_GetInputData(0, 2) & 0x80) {
        task_124 = task_124 + 1.0f;
      }
    }
  }
  return;
}

static void script_0DBD(int32_t arg0, int32_t arg1) {
  Vector3 local_F4;
  Vector3i local_E8;
  Script_MoveObjectToObject(arg0, arg1);
  Script_EVT_GetPosition(arg0, -1, &local_F4, 0);
  Script_mtwkGetSetRotation(arg0, &local_E8, 0);
  Script_MoveObjectToObject(arg0, -1);
  return;
}

static void script_0DE6(int32_t arg0, int32_t arg1, float arg2, float arg3, float arg4) {
  float local_F4[3];
  Vector3i local_E8;
  int32_t local_E4;
  Vector3 local_D8;
  local_E4 = Script_CEWPRelated(arg0);
  Script_MoveObjectToObject(arg0, arg1);
  local_F4[0] = arg2;
  local_F4[1] = 0.0f;
  local_F4[2] = arg3;
  local_D8.x = arg2;
  (&local_D8)->z = arg3;
  Script_EVT_SetPosition(arg0, 0x28000000, &local_D8);
  local_E8.x = 0;
  (&local_E8)->y = (int32_t)(arg4 / 360.0f * (float)0x10000);
  (&local_E8)->z = 0;
  Script_EVT_SetRotation(arg0, 0x38000000, &local_E8, 0);
  Script_MoveObjectToObject(arg0, local_E4);
  Script_InitializeOsag(arg0);
  return;
}

static void script_0E75(int32_t arg0, int32_t arg1, const Vector3 *arg2) {
  int32_t local_FC;
  local_FC = Script_CEWPRelated(arg0);
  Script_MoveObjectToObject(arg0, arg1);
  Script_EVT_SetPosition(arg0, POS_XYZ, arg2);
  Script_MoveObjectToObject(arg0, local_FC);
  Script_InitializeOsag(arg0);
  return;
}

static void script_0EA8(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
  Script_scenCreateTask(script_0EC2, 6, arg0, arg1, arg2, arg3, arg4, arg5);
  return;
}

static int32_t script_0EC2(int32_t arg0, int32_t arg1, float arg2, float arg3, float arg4, int32_t arg5) {
  if (arg5 >= 0) {
    SCNF_WaitFrames_0000(arg5);
  }
  script_0DE6(arg0, arg1, arg2, arg3, arg4);
  return Script_srchControl(0);
}

static void script_0EED(int32_t arg0, int32_t arg1, float arg2, float arg3, float arg4, float arg5) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t local_E4;
  local_E4 = Script_CEWPRelated(arg0);
  Script_MoveObjectToObject(arg0, arg1);
  local_F4.x = arg2;
  (&local_F4)->y = arg3;
  (&local_F4)->z = arg4;
  Script_EVT_SetPosition(arg0, POS_XYZ, &local_F4);
  local_E8.x = 0;
  (&local_E8)->y = (int32_t)(arg5 / 360.0f * (float)0x10000);
  (&local_E8)->z = 0;
  Script_EVT_SetRotation(arg0, 0x38000000, &local_E8, 0);
  Script_MoveObjectToObject(arg0, local_E4);
  Script_InitializeOsag(arg0);
  return;
}

static void script_0F6D(int32_t arg0, int32_t arg1, float arg2) {
  int32_t unused_F4[3];
  Vector3i local_E8;
  int32_t local_E4;
  local_E4 = Script_CEWPRelated(arg0);
  Script_MoveObjectToObject(arg0, arg1);
  local_E8.x = 0;
  (&local_E8)->y = (int32_t)(arg2 / 360.0f * (float)0x10000);
  (&local_E8)->z = 0;
  Script_EVT_SetRotation(arg0, 0x38000000, &local_E8, 0);
  Script_MoveObjectToObject(arg0, local_E4);
  return;
}

static void script_0FC1(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Script_scenCreateTask(script_0FD7, 4, arg0, arg1, arg2, arg3);
  return;
}

static int32_t script_0FD7(int32_t arg0, int32_t arg1, float arg2, int32_t arg3) {
  if (arg3 >= 0) {
    SCNF_WaitFrames_0000(arg3);
  }
  script_0F6D(arg0, arg1, arg2);
  return Script_srchControl(0);
}

static void script_0FFE(int32_t arg0, float arg1) {
  Vector3i local_F4;
  (&local_F4)->y = (int32_t)(arg1 / 360.0f * (float)0x10000);
  Script_EVT_SetRotation(arg0, -0x70000000, &local_F4, 0);
  return;
}

static void script_1027(int32_t arg0, float arg1, float arg2, float arg3) {
  Vector3i local_F4;
  local_F4.x = (int32_t)(arg1 / 360.0f * (float)0x10000);
  (&local_F4)->y = (int32_t)(arg2 / 360.0f * (float)0x10000);
  (&local_F4)->z = (int32_t)(arg3 / 360.0f * (float)0x10000);
  Script_EVT_SetRotation(arg0, -0x48000000, &local_F4, 0);
  return;
}

static void script_107B(int32_t arg0, int32_t arg1, int32_t arg2) {
  Script_scenCreateTask(script_108F, 3, arg0, arg1, arg2);
  return;
}

static int32_t script_108F(int32_t arg0, float arg1, int32_t arg2) {
  if (arg2 >= 0) {
    SCNF_WaitFrames_0000(arg2);
  }
  script_0FFE(arg0, arg1);
  return Script_srchControl(0);
}

static void script_10B4(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  Script_scenCreateTask(script_10CC, 5, arg0, arg1, arg2, arg3, arg4);
  return;
}

static int32_t script_10CC(int32_t arg0, float arg1, float arg2, float arg3, int32_t arg4) {
  if (arg4 >= 0) {
    SCNF_WaitFrames_0000(arg4);
  }
  script_1027(arg0, arg1, arg2, arg3);
  return Script_srchControl(0);
}

static void script_10F5(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  Script_scenCreateTask(script_110D, 5, arg0, arg1, arg2, arg3, arg4);
  return;
}

static int32_t script_110D(int32_t arg0, float arg1, float arg2, float arg3, int32_t arg4) {
  float local_FC;
  if (arg4 >= 0) {
    SCNF_WaitFrames_0000(arg4);
  }
  local_FC = (arg2 - arg1) / arg3;
  for (;;) {
    if (arg3 <= 0.0f) {
      break;
    }
    script_1027(arg0, 0.0f, 0.0f, local_FC);
    arg3 = arg3 - 1.0f;
    SCNF_WaitFrames_0000(1);
  }
  return Script_srchControl(0);
}

static void script_1162(int32_t arg0, int32_t arg1) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t local_E4;
  local_E4 = arg0;
  while (local_E4 <= arg1) {
    Script_EVT_GetPosition(task_74[local_E4], -1, &local_F4, 0);
    Script_mtwkGetSetRotation(task_74[local_E4], &local_E8, 0);
    task_128[local_E4][0] = (&local_F4)->x;
    task_128[local_E4][1] = (&local_F4)->y;
    task_128[local_E4][2] = (&local_F4)->z;
    task_158[local_E4][0] = (&local_E8)->x;
    task_158[local_E4][1] = (&local_E8)->y;
    task_158[local_E4][2] = (&local_E8)->z;
    task_188[local_E4] = Script_GetCharaMotionRemainingFramesScaled(task_74[local_E4]);
    task_198[local_E4] = Script_scenCharaGetMotAnimId(task_74[local_E4]);
    local_E4 = local_E4 + 1;
  }
  return;
}

static void script_1247(int32_t arg0, int32_t arg1) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t local_E4;
  local_E4 = arg0;
  while (local_E4 <= arg1) {
    local_F4.x = task_128[local_E4][0];
    (&local_F4)->y = task_128[local_E4][1];
    (&local_F4)->z = task_128[local_E4][2];
    local_E8.x = task_158[local_E4][0];
    (&local_E8)->y = task_158[local_E4][1];
    (&local_E8)->z = task_158[local_E4][2];
    Script_EVT_SetPosition(task_74[local_E4], POS_XYZ, &local_F4);
    Script_EVT_SetRotation(task_74[local_E4], 0x38000000, &local_E8, 0);
    Script_PlayCharaMotion(task_74[local_E4], task_198[local_E4], task_188[local_E4], -1.0f, -1.0f, 1.0f, 0);
    local_E4 = local_E4 + 1;
  }
  return;
}

static void script_1330(int32_t arg0) {
  task_1AC = arg0;
  task_1A8 = 1;
  Script_PlayerStatsUtils(PS_Set70MBTimeCapSlot, task_1AC, task_1A8);
  return;
}

static void script_134E(void) {
  int32_t local_FC;
  if (task_1A8 == 1) {
    local_FC = Script_PlayerStatsUtils(PS_Get70MBCounter1);
    Script_PlayerStatsUtils(PS_Set70MBCounter1, local_FC + 1);
  }
  if (task_1A8 < 255) {
    task_1A8 = task_1A8 + 1;
  }
  Script_PlayerStatsUtils(PS_Set70MBTimeCapSlot, task_1AC, task_1A8);
  return;
}

static void script_1396(void) {
  int32_t local_FC;
  local_FC = Script_PlayerStatsUtils(PS_Get70MBCounter0);
  Script_PlayerStatsUtils(PS_Set70MBCounter0, local_FC + 1);
  return;
}

static void script_13B1(void) {
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_13CB(const char *arg0, const char *arg1) {
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 5, arg1) == 0) {
    while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, arg0, arg1) == 0) {
      SCNF_WaitFrames_0000(1);
    }
    script_13B1();
  }
  return;
}

static void script_1409(const char *arg0) {
  if (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 5, arg0) != 0) {
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, arg0);
  }
  return;
}

static void script_142A(int32_t arg0) {
  L00142C:;
  SCNF_WaitFrames_0000(1);
  if (!Script_CycleManagerUtils(CYCLE_CMD_GET_NEXT_TASK_POINTER, arg0)) goto L00142C;
  return;
}

static int32_t script_1441(int32_t arg0) {
  int32_t local_FC;
  local_FC = 0;
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0) != 0) {
    local_FC = Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT_INTERNAL_HUMAN, arg0);
  } else {
    Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT, arg0, 1);
    while (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0) == 0) {
      local_FC = Script_CycleManagerUtils(CYCLE_CMD_GET_CYCLE_PTR_BY_CHARA_ID, arg0);
      while (local_FC == 0) {
        SCNF_WaitFrames_0000(1);
        local_FC = Script_CycleManagerUtils(CYCLE_CMD_GET_CYCLE_PTR_BY_CHARA_ID, arg0);
      }
      script_142A(local_FC);
    }
  }
  return local_FC;
}

static void script_14C7(int32_t arg0, int32_t arg1) {
  if (arg1) {
    Script_CycleManagerUtils(CYCLE_CMD_LOAD_OR_RESET_RUNNING_PROGRAM, arg1);
  }
  Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT, arg0, 0);
  arg1 = 0;
  return;
}

static void script_14EA(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  float local_E8[3];
  Vector3i local_DC;
  Vector3i local_D0;
  int32_t local_CC;
  local_DC.x = 0;
  (&local_DC)->y = 0;
  (&local_DC)->z = 0;
  local_D0.x = (&local_DC)->x;
  (&local_D0)->y = (&local_DC)->y;
  (&local_D0)->z = (&local_DC)->z;
  local_F4 = 0;
  local_E8[0] = 0.0f;
  local_E8[1] = 0.0f;
  local_E8[2] = 0.0f;
  local_FC = Script_shenSeqUtils(-8, 0);
  Script_shenSeqUtils(-9, 0, 0);
  if (arg0) {
    Script_filmCheckOrSetTransitionUtil(60, 255, 0, 0, 0, 0, 0, 0, 0);
  }
  Script_shenSeqUtils(0);
  local_F8 = 0;
  while (Script_shenSeqUtils(-1)) {
    local_CC = local_F8;
    switch (local_CC) {
      case 1:
        Script_mapmSetCamFlags(0, 1);
        Script_SetMaxInterpSpeed(0.0f);
        Script_SetMotnFlag(CHARA_CHRL, -1);
        Script_handSetFloatsFor(CHARA_AKIR, 1, data_C07C, 1);
        Script_handSetFloatsFor(CHARA_AKIR, 0, data_C07C, 1);
        Script_handSetFloatsFor(CHARA_CHRL, 0, data_BF98, 1);
        Script_handSetFloatsFor(CHARA_CHRL, 1, data_BF98, 1);
        break;
      case 250:
        Script_handSetFloatsFor(CHARA_CHRL, 0, data_C160, 20);
        Script_handSetFloatsFor(CHARA_AKIR, 1, data_BF98, 20);
        local_DC.x = 0;
        (&local_DC)->y = -4551;
        (&local_DC)->z = 0;
        local_F4 = 20;
        local_E8[0] = (float)((&local_DC)->x - (&local_D0)->x) / 20.0f;
        local_E8[1] = (float)((&local_DC)->y - (&local_D0)->y) / 20.0f;
        local_E8[2] = (float)((&local_DC)->z - (&local_D0)->z) / 20.0f;
        break;
      case 320:
        Script_handSetFloatsFor(CHARA_AKIR, 0, data_BF98, 30);
        local_DC.x = 0;
        (&local_DC)->y = 0;
        (&local_DC)->z = 0;
        local_D0.x = 0;
        (&local_D0)->y = 0;
        (&local_D0)->z = 0;
        Script_SetHandRootRotation(CHARA_CHRL, 0, 0x15, &data_C358);
        Script_SetHandRootRotation(CHARA_AKIR, 0, 0x15, &data_C394);
        break;
      case 360:
        Script_handSetFloatsFor(CHARA_CHRL, 0, data_C244, 25);
        Script_handSetFloatsFor(CHARA_CHRL, 1, data_C244, 30);
        break;
      case 479:
        Script_SetHandRootRotation(CHARA_CHRL, 1, 0x15, &data_C388);
        break;
      case 520:
        Script_handSetFloatsFor(CHARA_AKIR, 1, data_C07C, 20);
        break;
      case 525:
        Script_handSetFloatsFor(CHARA_CHRL, 0, data_C160, 10);
        break;
      case 0x500:
        Script_handSetFloatsFor(CHARA_CHRL, 0, data_C160, 10);
        Script_handSetFloatsFor(CHARA_CHRL, 1, data_C160, 10);
        Script_handSetFloatsFor(CHARA_AKIR, 0, data_C07C, 30);
        break;
      case 580:
        Script_handSetFloatsFor(CHARA_CHRL, 0, data_C244, 30);
        break;
      case 800:
        Script_mapmSetCamFlags(0, 0);
        break;
      case 940:
        Script_mapmSetCamFlags(0, 1);
        break;
      case 1530:
        Script_filmCheckOrSetTransitionUtil(50, 0, 0, 0, 0, 255, 0, 0, 0);
        while (Script_filmCheckOrSetTransitionUtil(-1)) {
          SCNF_WaitFrames_0000(1);
        }
        break;
    }
    if (local_F8 == 700) {
      Script_charaDisplay(SHEN_NAME('BTA_'), CDM_Show);
      Script_charaDisplay(SHEN_NAME('BTH_'), CDM_Show);
      Script_charaDisplay(SHEN_NAME('BTK_'), CDM_Show);
    }
    if (local_F8 == 940) {
      Script_charaDisplay(SHEN_NAME('BTA_'), CDM_Hide);
      Script_charaDisplay(SHEN_NAME('BTH_'), CDM_Hide);
      Script_charaDisplay(SHEN_NAME('BTK_'), CDM_Hide);
    }
    if (local_F8 == 480) {
      Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 20.0f);
      Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 20.0f);
    }
    if (local_F8 == 650) {
      Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 50.0f);
      Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 50.0f);
    }
    if (local_F8 == 772) {
      Script_FaceEx(CHARA_AKIR, 0, 0, 0);
      Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
      Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
      Script_FaceEx(CHARA_CHRL, 0, 0, 0);
      Script_ResetHandMotion(CHARA_CHRL, 0, 0, 0);
      Script_ResetHandMotion(CHARA_CHRL, 0, 1, 0);
    }
    if (local_F8 == 934) {
      Script_FaceEx(CHARA_AKIR, 1, 0, 0);
      Script_ResetHandMotion(CHARA_AKIR, 1, 0, 0);
      Script_ResetHandMotion(CHARA_AKIR, 1, 1, 0);
      Script_FaceEx(CHARA_CHRL, 1, 0, 0);
      Script_ResetHandMotion(CHARA_CHRL, 1, 0, 0);
      Script_ResetHandMotion(CHARA_CHRL, 1, 1, 0);
      Script_handSetFloatsFor(CHARA_AKIR, 1, data_C07C, 0);
      Script_handSetFloatsFor(CHARA_AKIR, 0, data_BF98, 0);
      Script_handSetFloatsFor(CHARA_CHRL, 0, data_C244, 0);
      Script_handSetFloatsFor(CHARA_CHRL, 1, data_C244, 0);
    }
    if (local_F4 != 0) {
      *(float *)&local_D0.x = (float)(&local_D0)->x + *local_E8;
      *(float *)&(&local_D0)->y = (float)(&local_D0)->y + local_E8[1];
      *(float *)&(&local_D0)->z = (float)(&local_D0)->z + local_E8[2];
      local_F4 = local_F4 - 1;
    } else {
      if (!((float)(&local_DC)->x - 0.1f <= (float)(&local_D0)->x)) {
        *(float *)&local_D0.x = (float)(&local_D0)->x + 0.1f;
      } else if (!((float)(&local_DC)->x - 0.01f <= (float)(&local_D0)->x)) {
        *(float *)&local_D0.x = (float)(&local_D0)->x + 0.01f;
      } else {
        if (!((float)(&local_DC)->x - 0.001f <= (float)(&local_D0)->x)) {
          *(float *)&local_D0.x = (float)(&local_D0)->x + 0.001f;
          goto L001AC9;
        }
      }
      L001AC9:;
      if (!((float)(&local_DC)->x + 0.1f >= (float)(&local_D0)->x)) {
        *(float *)&local_D0.x = (float)(&local_D0)->x - 0.1f;
      } else if (!((float)(&local_DC)->x + 0.01f >= (float)(&local_D0)->x)) {
        *(float *)&local_D0.x = (float)(&local_D0)->x - 0.01f;
      } else {
        if (!((float)(&local_DC)->x + 0.001f >= (float)(&local_D0)->x)) {
          *(float *)&local_D0.x = (float)(&local_D0)->x - 0.001f;
          goto L001B35;
        }
      }
      L001B35:;
      if (!((float)(&local_DC)->y - 0.1f <= (float)(&local_D0)->y)) {
        *(float *)&(&local_D0)->y = (float)(&local_D0)->y + 0.1f;
      } else if (!((float)(&local_DC)->y - 0.01f <= (float)(&local_D0)->y)) {
        *(float *)&(&local_D0)->y = (float)(&local_D0)->y + 0.01f;
      } else {
        if (!((float)(&local_DC)->y - 0.001f <= (float)(&local_D0)->y)) {
          *(float *)&(&local_D0)->y = (float)(&local_D0)->y + 0.001f;
          goto L001BC5;
        }
      }
      L001BC5:;
      if (!((float)(&local_DC)->y + 0.1f >= (float)(&local_D0)->y)) {
        *(float *)&(&local_D0)->y = (float)(&local_D0)->y - 0.1f;
      } else if (!((float)(&local_DC)->y + 0.01f >= (float)(&local_D0)->y)) {
        *(float *)&(&local_D0)->y = (float)(&local_D0)->y - 0.01f;
      } else {
        if (!((float)(&local_DC)->y + 0.001f >= (float)(&local_D0)->y)) {
          *(float *)&(&local_D0)->y = (float)(&local_D0)->y - 0.001f;
          goto L001C55;
        }
      }
      L001C55:;
      if (!((float)(&local_DC)->z - 0.1f <= (float)(&local_D0)->z)) {
        *(float *)&(&local_D0)->z = (float)(&local_D0)->z + 0.1f;
      } else if (!((float)(&local_DC)->z - 0.01f <= (float)(&local_D0)->z)) {
        *(float *)&(&local_D0)->z = (float)(&local_D0)->z + 0.01f;
      } else {
        if (!((float)(&local_DC)->z - 0.001f <= (float)(&local_D0)->z)) {
          *(float *)&(&local_D0)->z = (float)(&local_D0)->z + 0.001f;
          goto L001CE5;
        }
      }
      L001CE5:;
      if (!((float)(&local_DC)->z + 0.1f >= (float)(&local_D0)->z)) {
        *(float *)&(&local_D0)->z = (float)(&local_D0)->z - 0.1f;
      } else if (!((float)(&local_DC)->z + 0.01f >= (float)(&local_D0)->z)) {
        *(float *)&(&local_D0)->z = (float)(&local_D0)->z - 0.01f;
      } else {
        if (!((float)(&local_DC)->z + 0.001f >= (float)(&local_D0)->z)) {
          *(float *)&(&local_D0)->z = (float)(&local_D0)->z - 0.001f;
          goto L001D75;
        }
      }
    }
    L001D75:;
    Script_SetHandRootRotation(CHARA_AKIR, 1, 0x15, &local_D0);
    local_F8 = local_F8 + 1;
    SCNF_WaitFrames_0000(1);
  }
  Script_SetMotnFlag(CHARA_CHRL, 0);
  Script_SetMaxInterpSpeed(0.5f);
  Script_shenSeqUtils(-9, 0, local_FC);
  return;
}

static int32_t script_1DBA(int32_t arg0, int32_t arg1, int32_t arg2) {
  int32_t local_FC;
  local_FC = arg2;
  switch (local_FC) {
    case 1:
      Script_scenCreateTask(script_0484, 2, 620203, 17);
      Script_scenCreateTask(script_057F, 6, 21003, -1, 0, 0x3F800000, arg0, arg1);
      break;
    case 2:
      break;
    case 3:
      Script_scenCreateTask(script_0484, 2, 161451, 17);
      Script_scenCreateTask(script_0484, 2, 0x2776AB, 32);
      Script_scenCreateTask(script_057F, 6, 21006, -1, 0, 0x3F800000, arg0, arg1);
      task_70 = task_70 ^ -0x80000000;
      break;
    case 4:
      break;
    case 5:
      Script_scenCreateTask(script_0484, 2, 0x2176AB, 17);
      Script_scenCreateTask(script_0484, 2, 0x2776AB, 36);
      Script_scenCreateTask(script_057F, 6, 21001, -1, 0, 0x3F800000, arg0, arg1);
      break;
    case 6:
      break;
    case 7:
      Script_scenCreateTask(script_0484, 2, 161451, 19);
      Script_scenCreateTask(script_0484, 2, 0x2776AB, 36);
      Script_scenCreateTask(script_057F, 6, 21006, -1, 0, 0x3F800000, arg0, arg1);
      break;
    case 8:
      break;
    case 9:
      Script_scenCreateTask(script_0484, 2, 0x2976AB, 4);
      Script_scenCreateTask(script_057F, 6, 20999, -1, 0, 0x3F99999A, arg0, arg1);
      break;
    case 10:
      Script_scenCreateTask(script_0484, 2, 161451, 22);
      Script_scenCreateTask(script_0484, 2, 0x2876AB, 36);
      Script_scenCreateTask(script_057F, 6, 21012, 8, 0, 0x3F99999A, arg0, arg1);
      break;
    case 11:
      Script_scenCreateTask(script_0484, 2, 161451, 25);
      Script_scenCreateTask(script_0484, 2, 0x2876AB, 36);
      Script_scenCreateTask(script_057F, 6, 21007, 2, 0, 0x3F99999A, arg0, arg1);
      break;
    case 12:
      break;
    case 13:
      Script_scenCreateTask(script_057F, 6, 21000, -1, 0x42480000, 0x40000000, arg0, arg1);
      task_70 = task_70 ^ -0x80000000;
      break;
    case 14:
      break;
    case 15:
      Script_scenCreateTask(script_057F, 6, 21000, 10, 0x42480000, 0x40000000, arg0, arg1);
      task_70 = task_70 ^ -0x80000000;
      break;
    case 16:
      break;
    case 17:
      Script_scenCreateTask(script_057F, 6, 21000, 20, 0x42480000, 0x40000000, arg0, arg1);
      Script_scenCreateTask(script_057F, 6, 21000, 50, 0x432F0000, 0x40000000, arg0, arg1);
      task_70 = task_70 ^ -0x80000000;
      break;
    case 18:
      break;
    case 19:
      break;
    case 20:
      Script_scenCreateTask(script_057F, 6, 21009, -1, 0, 0x3F800000, arg0, arg1);
      Script_scenCreateTask(script_0484, 2, 0x1B76AB, 70);
      break;
    case 21:
      Script_scenCreateTask(script_057F, 6, 21002, -1, 0x42480000, 0x3F800000, arg0, arg1);
      break;
    case 22:
      Script_scenCreateTask(script_057F, 6, 21009, -1, 0x42700000, 0x3F800000, arg0, arg1);
      break;
    case 23:
      Script_scenCreateTask(script_057F, 6, 21005, -1, 0x42700000, 0x3F800000, arg0, arg1);
      Script_scenCreateTask(script_0484, 2, 30379, 10);
      break;
    case 24:
      Script_scenCreateTask(script_057F, 6, 21009, -1, 0x42700000, 0x3F800000, arg0, arg1);
      break;
    case 25:
      break;
    case 26:
      Script_scenCreateTask(script_057F, 6, 21009, -1, 0, 0x3F800000, arg0, arg1);
      Script_scenCreateTask(script_0484, 2, 0x1B76AB, 70);
      break;
    case 27:
      Script_scenCreateTask(script_057F, 6, 21002, -1, 0x42480000, 0x3F800000, arg0, arg1);
      break;
    case 28:
      Script_scenCreateTask(script_057F, 6, 21009, -1, 0x42700000, 0x3F800000, arg0, arg1);
      break;
    case 29:
      Script_scenCreateTask(script_057F, 6, 21004, -1, 0x42400000, 0x3F800000, arg0, arg1);
      Script_scenCreateTask(script_0484, 2, 161451, 10);
      Script_scenCreateTask(script_0484, 2, 0x2876AB, 20);
      break;
    case 30:
      Script_scenCreateTask(script_057F, 6, 21009, -1, 0x42700000, 0x3F800000, arg0, arg1);
      break;
    case 31:
      break;
    case 32:
      break;
    case 33:
      Script_scenCreateTask(script_057F, 6, 20998, -1, 0x41700000, 0x3F800000, arg0, arg1);
      break;
    case 34:
      Script_scenCreateTask(script_057F, 6, 21011, -1, 0, 0x3F800000, arg0, arg1);
      Script_scenCreateTask(script_0484, 2, 161451, 30);
      Script_scenCreateTask(script_0484, 2, 0x2876AB, 40);
      break;
    case 35:
      Script_scenCreateTask(script_057F, 6, 21004, -1, 0x42480000, 0x3F800000, arg0, arg1);
      break;
    case 36:
      Script_scenCreateTask(script_057F, 6, 21009, -1, 0x42700000, 0x3F800000, arg0, arg1);
      Script_scenCreateTask(script_0484, 2, 0x1B76AB, 10);
      Script_scenCreateTask(script_0484, 2, 161451, 20);
      Script_scenCreateTask(script_0484, 2, 0x2876AB, 30);
      break;
  }
  return Script_srchControl(0);
}

__attribute__((scn_string_bindings(0, data_C7D8)))
static int32_t script_240B(int32_t arg0, int32_t arg1, int32_t arg2) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t local_E4;
  int32_t local_E0;
  int32_t local_DC;
  if (arg2 > 0) {
    SCNF_WaitFrames_0000(arg2);
  }
  if (arg1 != 0) {
    Script_charaDisplay(arg1, CDM_Hide);
  }
  Script_charaDisplay(arg0, CDM_Show);
  Script_EVT_GetPosition(arg1, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  Script_mtwkGetSetRotation(arg1, &local_E8, 0x40000000);
  Script_EVT_SetPosition(arg0, 0x78000000, &local_F4);
  Script_EVT_SetRotation(arg0, 0x78000000, &local_E8, 0);
  local_E4 = 2;
  local_E0 = 2;
  Script_scenCreateTask(script_0484, 2, 197801, (15 - local_E4) / local_E0);
  while (local_E4 <= 380) {
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 2, "QTE_MBQC");
    local_DC = 0;
    while (local_DC < 6) {
      Script_InterpolateAndScaleAngles(local_DC, local_E4, &local_F4, &local_E8, "BIKA");
      local_F4.x = (&local_F4)->x * 10.0f;
      (&local_F4)->y = (&local_F4)->y * 10.0f;
      (&local_F4)->z = (&local_F4)->z * 10.0f;
      Script_charaPosRotUtils(arg0, *(&data_C630 + local_DC), &local_F4, &local_E8, 0);
      local_DC = local_DC + 1;
    }
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
    local_E4 = local_E4 + local_E0;
    SCNF_WaitFrames_0000(1);
  }
  return Script_srchControl(0);
}

__attribute__((scn_string_bindings(0, data_C7E6)))
static int32_t script_2555(int32_t arg0, int32_t arg1, int32_t arg2) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t local_E4;
  int32_t local_E0;
  int32_t local_DC;
  if (arg2 > 0) {
    SCNF_WaitFrames_0000(arg2);
  }
  if (arg1 != 0) {
    Script_charaDisplay(arg1, CDM_Hide);
  }
  Script_charaDisplay(arg0, CDM_Show);
  Script_EVT_GetPosition(arg1, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  Script_mtwkGetSetRotation(arg1, &local_E8, 0x40000000);
  Script_EVT_SetPosition(arg0, 0x78000000, &local_F4);
  Script_EVT_SetRotation(arg0, 0x78000000, &local_E8, 0);
  local_E4 = 2;
  local_E0 = 2;
  Script_scenCreateTask(script_0484, 2, 0x2304A9, (100 - local_E4) / local_E0);
  while (local_E4 <= 340) {
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 2, "QTE_MBQC");
    local_DC = 0;
    while (local_DC < 6) {
      Script_InterpolateAndScaleAngles(local_DC, local_E4, &local_F4, &local_E8, "BIKB");
      local_F4.x = (&local_F4)->x * 10.0f;
      (&local_F4)->y = (&local_F4)->y * 10.0f;
      (&local_F4)->z = (&local_F4)->z * 10.0f;
      Script_charaPosRotUtils(arg0, *(&data_C630 + local_DC), &local_F4, &local_E8, 0);
      local_DC = local_DC + 1;
    }
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
    local_E4 = local_E4 + local_E0;
    SCNF_WaitFrames_0000(1);
  }
  return Script_srchControl(0);
}

__attribute__((scn_string_bindings(0, data_C7F4)))
static void script_269F(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  if (arg3 > 0) {
    SCNF_WaitFrames_0000(arg3);
  }
  if (arg2 != 0) {
    Script_charaDisplay(arg2, CDM_Hide);
  }
  Script_charaDisplay(arg1, CDM_Show);
  {
    Vector3 local_F4;
    Vector3i local_E8;
    local_F4.x = 0.0f;
    (&local_F4)->y = 0.0f;
    (&local_F4)->z = 0.0f;
    local_E8.x = 0;
    (&local_E8)->y = 0;
    (&local_E8)->z = 0;
    Script_MoveObjectToObject(arg0, arg1);
    Script_EVT_SetPosition(arg0, POS_XYZ, &local_F4);
    Script_EVT_SetRotation(arg0, 0x38000000, &local_E8, 0);
  }
  {
    Vector3 local_F4;
    Vector3i local_E8;
    int32_t local_E4;
    int32_t local_E0;
    Script_EVT_GetPosition(arg2, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
    Script_mtwkGetSetRotation(arg2, &local_E8, 0x40000000);
    Script_EVT_SetPosition(arg1, 0x78000000, &local_F4);
    Script_EVT_SetRotation(arg1, 0x78000000, &local_E8, 0);
    local_E4 = 2;
    local_E0 = 2;
    while (local_E4 <= 210) {
      Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
      Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 2, "QTE_MBQC");
      {
        int32_t local_DC;
        local_DC = 0;
        while (local_DC < 6) {
          Script_InterpolateAndScaleAngles(local_DC, local_E4, &local_F4, &local_E8, "BIKC");
          local_F4.x = (&local_F4)->x * 10.0f;
          (&local_F4)->y = (&local_F4)->y * 10.0f;
          (&local_F4)->z = (&local_F4)->z * 10.0f;
          Script_charaPosRotUtils(arg1, *(&data_C648 + local_DC), &local_F4, &local_E8, 0);
          if (local_DC == 0) {
            Script_EVT_SetPosition(arg0, POS_XYZ, &local_F4);
            {
              Script_EVT_SetRotation(arg0, 0x38000000, &local_E8, 0);
            }
          }
          local_DC = local_DC + 1;
        }
      }
      Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
      {
        local_E4 = local_E4 + local_E0;
      }
      SCNF_WaitFrames_0000(1);
    }
  }
  Script_MoveObjectToObject(arg0, 0x304D5544);
  Script_srchControl(0);
  return;
}

__attribute__((scn_string_bindings(0, data_C802)))
static int32_t script_2855(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  if (arg3 > 0) {
    SCNF_WaitFrames_0000(arg3);
  }
  if (arg2 != 0) {
    Script_charaDisplay(arg2, CDM_Hide);
  }
  Script_charaDisplay(arg1, CDM_Show);
  {
    Vector3 local_F4;
    int32_t unused_E8[3];
    Vector3i local_DC;
    local_F4.x = 0.0f;
    (&local_F4)->y = 1.0f;
    (&local_F4)->z = 0.25f;
    local_DC.x = 0;
    (&local_DC)->y = 0;
    (&local_DC)->z = 0;
    Script_SetupTransformMatrixForMotionTask(arg0, 0, arg1, 152, &local_F4, &local_DC);
    Script_EVT_SetPosition(arg0, POS_XYZ, &local_F4);
    Script_EVT_SetRotation(arg0, 0x38000000, &local_DC, 0);
  }
  {
    Vector3 local_F4;
    Vector3 local_E8;
    Vector3i local_DC;
    Vector3i local_D0;
    int32_t local_CC;
    int32_t local_C8;
    Script_EVT_GetPosition(arg2, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
    Script_mtwkGetSetRotation(arg2, &local_DC, 0x40000000);
    (&local_F4)->y = (&local_F4)->y + 0.1f;
    Script_EVT_SetPosition(arg1, 0x78000000, &local_F4);
    Script_EVT_SetRotation(arg1, 0x78000000, &local_DC, 0);
    local_CC = 2;
    local_C8 = 2;
    while (local_CC <= 200) {
      Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
      Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 2, "QTE_MBQC");
      {
        int32_t local_C4;
        local_C4 = 0;
        while (local_C4 < 5) {
          Script_InterpolateAndScaleAngles(local_C4, local_CC, &local_F4, &local_DC, "BIKD");
          local_F4.x = (&local_F4)->x * 10.0f;
          (&local_F4)->y = (&local_F4)->y * 10.0f;
          (&local_F4)->z = (&local_F4)->z * 10.0f;
          Script_charaPosRotUtils(arg1, *(&data_C664 + local_C4), &local_F4, &local_DC, 0);
          if (*(&data_C664 + local_C4) == 152) {
            local_E8.x = (&local_F4)->x;
            (&local_E8)->y = (&local_F4)->y;
            (&local_E8)->z = (&local_F4)->z - 1.8f;
            {
              local_D0.x = 0;
              (&local_D0)->y = (&local_DC)->y;
              (&local_D0)->z = 1820;
              Script_EVT_SetPosition(SHEN_NAME('RYY0'), POS_XYZ, &local_E8);
              Script_EVT_SetRotation(SHEN_NAME('RYY0'), 0x38000000, &local_D0, 0);
            }
          }
          if (*(&data_C664 + local_C4) == 154) {
            Script_EVT_SetPosition(SHEN_NAME('DMBY'), POS_XYZ, &local_F4);
            {
              Script_EVT_SetRotation(SHEN_NAME('DMBY'), 0x38000000, &local_DC, 0);
            }
          }
          local_C4 = local_C4 + 1;
        }
      }
      Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
      {
        local_CC = local_CC + local_C8;
      }
      SCNF_WaitFrames_0000(1);
    }
  }
  SCNF_WaitFrames_0000(60);
  Script_SetupTransformMatrixForMotionTask(arg0, 0, 0, 0, 0, 0);
  return Script_srchControl(0);
}

__attribute__((scn_string_bindings(0, data_C810)))
static int32_t script_2AC7(int32_t arg0, int32_t arg1, int32_t arg2) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t local_E4;
  int32_t local_E0;
  int32_t local_DC;
  if (arg2 > 0) {
    SCNF_WaitFrames_0000(arg2);
  }
  if (arg1 != 0) {
    Script_charaDisplay(arg1, CDM_Hide);
  }
  Script_charaDisplay(arg0, CDM_Show);
  Script_EVT_GetPosition(arg1, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  Script_mtwkGetSetRotation(arg1, &local_E8, 0x40000000);
  Script_EVT_SetPosition(arg0, 0x78000000, &local_F4);
  Script_EVT_SetRotation(arg0, 0x78000000, &local_E8, 0);
  local_E4 = 2;
  local_E0 = 2;
  Script_scenCreateTask(script_0484, 2, 0x2204A9, (50 - local_E4) / local_E0);
  while (local_E4 <= 320) {
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 2, "QTE_MBQC");
    local_DC = 0;
    while (local_DC < 5) {
      Script_InterpolateAndScaleAngles(local_DC, local_E4, &local_F4, &local_E8, "BIKY");
      local_F4.x = (&local_F4)->x * 10.0f;
      (&local_F4)->y = (&local_F4)->y * 10.0f;
      (&local_F4)->z = (&local_F4)->z * 10.0f;
      Script_charaPosRotUtils(arg0, *(&data_C678 + local_DC), &local_F4, &local_E8, 0);
      local_DC = local_DC + 1;
    }
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
    local_E4 = local_E4 + local_E0;
    SCNF_WaitFrames_0000(1);
  }
  return Script_srchControl(0);
}

__attribute__((scn_string_bindings(0, data_C81E)))
static int32_t script_2C11(int32_t arg0, int32_t arg1, int32_t arg2) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t local_E4;
  int32_t local_E0;
  int32_t local_DC;
  if (arg2 > 0) {
    SCNF_WaitFrames_0000(arg2);
  }
  if (arg1 != 0) {
    Script_charaDisplay(arg1, CDM_Hide);
  }
  Script_charaDisplay(arg0, CDM_Show);
  local_E4 = 2;
  local_E0 = 2;
  Script_scenCreateTask(script_0484, 2, 656553, (5 - local_E4) / local_E0);
  for (;;) {
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 2, "QTE_MBQC");
    local_DC = 0;
    while (local_DC < 24) {
      Script_InterpolateAndScaleAngles(local_DC, local_E4, &local_F4, &local_E8, "HAKB");
      local_F4.x = (&local_F4)->x * 10.0f;
      (&local_F4)->y = (&local_F4)->y * 10.0f;
      (&local_F4)->z = (&local_F4)->z * 10.0f;
      Script_charaPosRotUtils(arg0, *(&data_C68C + local_DC), &local_F4, &local_E8, 0);
      local_DC = local_DC + 1;
    }
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
    local_E4 = local_E4 + local_E0;
    if (local_E4 >= 200) {
      Script_srchControl(0);
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

__attribute__((scn_string_bindings(0, data_C82C)))
static int32_t script_2D23(int32_t arg0, int32_t arg1, int32_t arg2) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t local_E4;
  int32_t local_E0;
  int32_t local_DC;
  if (arg2 > 0) {
    SCNF_WaitFrames_0000(arg2);
  }
  if (arg1 != 0) {
    Script_charaDisplay(arg1, CDM_Hide);
  }
  Script_charaDisplay(arg0, CDM_Show);
  local_E4 = 2;
  local_E0 = 2;
  Script_scenCreateTask(script_0484, 2, 591017, (60 - local_E4) / local_E0);
  for (;;) {
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 2, "QTE_MBQC");
    local_DC = 0;
    while (local_DC < 3) {
      Script_InterpolateAndScaleAngles(local_DC, local_E4, &local_F4, &local_E8, "HAKA");
      local_F4.x = (&local_F4)->x * 10.0f;
      (&local_F4)->y = (&local_F4)->y * 10.0f;
      (&local_F4)->z = (&local_F4)->z * 10.0f;
      Script_charaPosRotUtils(arg0, *(&data_C6EC + local_DC), &local_F4, &local_E8, 0);
      local_DC = local_DC + 1;
    }
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
    local_E4 = local_E4 + local_E0;
    if (local_E4 >= 200) {
      Script_srchControl(0);
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_2E35(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  int32_t local_F0;
  Vector3 local_E4;
  int32_t local_D8[3];
  int32_t local_CC[3];
  Vector3i local_C0;
  int32_t local_BC;
  if (arg1 >= 0) {
    SCNF_WaitFrames_0000(arg1);
  }
  local_BC = arg0;
  if (local_BC == 0) {
    local_F4 = 0x30424D44;
    local_F8 = 0x304B4942;
  } else if (local_BC == 1) {
    local_F4 = 0x31424D44;
    local_F8 = 0x314B4942;
  } else if (local_BC == 2) {
    local_F4 = 0x32424D44;
    local_F8 = 0x324B4942;
  } else {
    local_F4 = 0x30424D44;
    local_F8 = 0x304B4942;
  }
  Script_EVT_GetPosition(local_F4, -1, &local_E4, ENTITY_XFORM_WORLD_SPACE);
  local_D8[0] = 0;
  local_D8[1] = 0x3C23D70A;
  local_D8[2] = 0;
  local_CC[0] = 0;
  local_CC[1] = 0;
  local_CC[2] = 0;
  local_F0 = Script_EffectUtils(0, &local_E4, &data_C83C, 10, 0.001f, 0, 0);
  Script_EffectUtils(12, local_F0, 32);
  Script_EffectUtils(11, local_F0, 255);
  Script_EffectUtils(9, local_F0, 64, 62, 60);
  Script_EffectUtils(10, local_F0, 0.262745112f);
  Script_EffectUtils(8, local_F0, 2.54901958f, 1.254902f);
  Script_EffectUtils(7, local_F0, 0.0f);
  Script_EffectUtils(1, local_F0);
  Script_EffectUtils(7, local_F0, 0.1f);
  SCNF_WaitFrames_0000(8);
  local_FC = 40;
  local_C0.x = 0;
  (&local_C0)->y = 0;
  (&local_C0)->z = 0;
  while (local_FC > 0) {
    Script_EVT_GetPosition(local_F4, -1, &local_E4, ENTITY_XFORM_WORLD_SPACE);
    Script_EffectUtils(5, local_F0, &local_E4, &data_C83C);
    Script_charaPosRotUtils(local_F8, 18, 0, &local_C0, 0);
    local_C0.x = (&local_C0)->x + -29127;
    local_FC = local_FC - 1;
    SCNF_WaitFrames_0000(1);
  }
  SCNF_WaitFrames_0000(60);
  Script_EffectUtils(3, local_F0);
  return Script_srchControl(0);
}

static void script_3017(int32_t arg0, int32_t arg1, float arg2, float arg3, float arg4, float arg5) {
  Vector3 local_F4;
  Vector3 local_E8;
  float local_B8[4][3];
  Vector3 local_AC;
  Vector3 local_A0;
  Vector3 local_94;
  Vector3 local_88;
  Vector3 local_FF7C;
  Vector3 local_FF70;
  int32_t unused_FF64[3];
  int32_t local_FF60;
  int32_t unused_FF54[3];
  int32_t local_FF50;
  int32_t local_FF4C;
  float local_FF48;
  float local_FF44;
  int32_t local_FF40;
  local_FF40 = arg0;
  switch (local_FF40) {
    case 0:
      local_FF4C = 0;
      while (local_FF4C < 4) {
        *(&local_FF60 + local_FF4C) = -1;
        *(&local_FF50 + local_FF4C) = -1;
        local_FF4C = local_FF4C + 1;
      }
      break;
    case 1:
      local_F4.x = arg2;
      (&local_F4)->y = arg3;
      (&local_F4)->z = arg4;
      local_AC.x = 0.1f;
      (&local_AC)->y = 0.0f;
      (&local_AC)->z = 0.1f;
      local_94.x = 0.0f;
      (&local_94)->y = 0.1f;
      (&local_94)->z = 0.0f;
      local_FF7C.x = 0.05f;
      (&local_FF7C)->y = 0.05f;
      (&local_FF7C)->z = 0.05f;
      *(&local_FF60 + arg1) = Script_SetEFPT(10, &local_F4, &local_AC, 50, 0.0f, 0, arg5, 0, 0);
      Script_SetEFPT(11, *(&local_FF60 + arg1), 255, 255, 160, 0x20);
      Script_SetEFPT(6, *(&local_FF60 + arg1), &local_94, &local_FF7C);
      Script_SetEFPT(8, *(&local_FF60 + arg1), 0.03f);
      Script_SetEFPT(1, *(&local_FF60 + arg1));
      local_A0.x = 0.05f;
      (&local_A0)->y = 0.0f;
      (&local_A0)->z = 0.05f;
      local_88.x = 0.05f;
      (&local_88)->y = 0.03f;
      (&local_88)->z = 0.0f;
      local_FF70.x = 0.02f;
      (&local_FF70)->y = 0.02f;
      (&local_FF70)->z = 0.02f;
      *(&local_FF50 + arg1) = Script_SetEFPT(10, &local_F4, &local_A0, 200, 0.0f, 0, arg5, 0, 0);
      Script_SetEFPT(11, *(&local_FF50 + arg1), 85, 255, 160, 0x20);
      Script_SetEFPT(6, *(&local_FF50 + arg1), &local_88, &local_FF70);
      Script_SetEFPT(8, *(&local_FF50 + arg1), 0.05f);
      Script_SetEFPT(1, *(&local_FF50 + arg1));
      local_B8[arg1][0] = (&local_F4)->x;
      local_B8[arg1][1] = (&local_F4)->y;
      local_B8[arg1][2] = (&local_F4)->z;
      break;
    case 2:
      local_F4.x = arg2;
      (&local_F4)->y = arg3;
      (&local_F4)->z = arg4;
      Script_GetAuthorState_And_SetCamPos(&local_E8, 0);
      local_FF48 = Script_MathUtils(SCRIPT_MATH_TASK_VEC3_DISTANCE, &local_F4, &local_E8);
      local_FF44 = Script_MathUtils(SCRIPT_MATH_VEC2_LENGTH, (&local_F4)->x - local_B8[arg1][0], (&local_F4)->z - local_B8[arg1][2]);
      Script_SetEFPT(5, *(&local_FF60 + arg1), &local_F4, &local_AC);
      Script_SetEFPT(7, *(&local_FF60 + arg1), local_FF44 * local_FF44 * 0.03f);
      Script_SetEFPT(8, *(&local_FF60 + arg1), local_FF48 * 0.003f);
      local_88.x = ((&local_F4)->x - local_B8[arg1][0]) / 2.0f;
      (&local_88)->y = 0.03f;
      (&local_88)->z = ((&local_F4)->z - local_B8[arg1][2]) / 2.0f;
      Script_SetEFPT(6, *(&local_FF50 + arg1), &local_88, &local_FF70);
      Script_SetEFPT(5, *(&local_FF50 + arg1), &local_F4, &local_A0);
      Script_SetEFPT(7, *(&local_FF50 + arg1), local_FF44 * local_FF44 * 0.5f);
      Script_SetEFPT(8, *(&local_FF50 + arg1), local_FF48 * 0.005f);
      local_B8[arg1][0] = (&local_F4)->x;
      local_B8[arg1][1] = (&local_F4)->y;
      local_B8[arg1][2] = (&local_F4)->z;
      break;
    case 3:
      local_B8[arg1][0] = arg2;
      local_B8[arg1][1] = arg3;
      local_B8[arg1][2] = arg4;
      break;
    case 9:
      Script_SetEFPT(3, *(&local_FF60 + arg1));
      Script_SetEFPT(3, *(&local_FF50 + arg1));
      *(&local_FF60 + arg1) = -1;
      *(&local_FF50 + arg1) = -1;
      break;
  }
  return;
}

static int32_t script_3487(int32_t arg0, int32_t arg1, int32_t arg2) {
  Vector3 local_F4;
  float local_F0;
  int32_t local_EC;
  if (arg1 >= 0) {
    SCNF_WaitFrames_0000(arg1);
  }
  script_3017(0, 0, 0.0f, 0.0f, 0.0f, 0.0f);
  Script_EVT_GetPosition(arg0, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  local_F0 = (&local_F4)->y;
  script_3017(1, 0, (&local_F4)->x, (&local_F4)->y + 0.01f, (&local_F4)->z, local_F0);
  local_EC = arg2;
  while (local_EC > 0) {
    Script_EVT_GetPosition(arg0, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
    script_3017(2, 0, (&local_F4)->x, (&local_F4)->y + 0.01f, (&local_F4)->z, local_F0);
    local_EC = local_EC - 1;
    SCNF_WaitFrames_0000(1);
  }
  script_3017(9, 0, (&local_F4)->x, (&local_F4)->y, (&local_F4)->z, local_F0);
  return Script_srchControl(0);
}

static int32_t script_355F(int32_t arg0, int32_t arg1, float arg2, int32_t arg3, const void *arg4) {
  Vector3i local_F4;
  local_F4.x = 0;
  (&local_F4)->y = 0;
  (&local_F4)->z = 0;
  SCNF_WaitFrames_0000(1);
  task_1B0[arg1] = arg0;
  task_1E8[arg1] = arg3;
  script_367C(arg1, arg2, arg4, 0);
  for (;;) {
    if (task_204[arg1]) {
      task_23C[arg1] = Script_walk3D(ActivateGetIdx, task_1B0[arg1], task_274[arg1]);
      if (task_1E8[arg1] == 0) {
        Script_charaPosRotUtils(task_1CC[arg1], 23, 0, &local_F4, 0);
        Script_charaPosRotUtils(task_1CC[arg1], 18, 0, &local_F4, 0);
        local_F4.x = (&local_F4)->x - (int32_t)(task_274[arg1] * 100.0f / 360.0f * (float)0x10000);
      }
    }
    if (task_23C[arg1] != -1) {
      task_220[arg1] = task_220[arg1] + 1;
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_367C(int32_t arg0, float arg1, const void *arg2, int32_t arg3) {
  int32_t local_FC;
  task_258[arg0] = 0;
  task_204[arg0] = 0;
  task_220[arg0] = 0;
  task_23C[arg0] = 0;
  task_274[arg0] = arg1;
  local_FC = task_1E8[arg0];
  if (local_FC == 0) {
    if (!(arg1 < 9.0f)) {
      task_258[arg0] = 1;
    }
    goto L0036F2;
  }
  L0036F2:;
  Script_walk3D(SetupPath, task_1B0[arg0], arg2, arg3);
  return;
}

static void script_370B(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  Script_scenCreateTask(script_3723, 5, arg0, arg1, arg2, arg3, arg4);
  return;
}

static int32_t script_3723(int32_t arg0, float arg1, float arg2, float arg3, int32_t arg4) {
  int32_t unused_F4[3];
  float local_F0;
  if (arg4 >= 0) {
    SCNF_WaitFrames_0000(arg4);
  }
  local_F0 = (arg2 - arg1) / arg3;
  for (;;) {
    if (arg3 < 0.0f) {
      break;
    }
    task_274[arg0] = arg1;
    arg1 = arg1 + local_F0;
    arg3 = arg3 - 1.0f;
    SCNF_WaitFrames_0000(1);
  }
  return Script_srchControl(0);
}

static void script_377F(int32_t arg0, float arg1) {
  int32_t unused_F4[3];
  int32_t local_F0;
  task_274[arg0] = arg1;
  task_220[arg0] = 0;
  task_204[arg0] = 1;
  local_F0 = task_1E8[arg0];
  if (local_F0 == 0) goto L0037C2;
  L0037C2:;
  return;
}

static void script_37C3(int32_t arg0, int32_t arg1, int32_t arg2) {
  Script_scenCreateTask(script_37D7, 3, arg0, arg1, arg2);
  return;
}

static int32_t script_37D7(int32_t arg0, float arg1, int32_t arg2) {
  if (arg2 >= 0) {
    SCNF_WaitFrames_0000(arg2);
  }
  script_377F(arg0, arg1);
  return Script_srchControl(0);
}

static void script_37FC(int32_t arg0) {
  int32_t local_FC;
  task_204[arg0] = 0;
  local_FC = task_1E8[arg0];
  if (local_FC == 0) goto L003825;
  L003825:;
  return;
}

static void script_3826(int32_t arg0, int32_t arg1) {
  Script_scenCreateTask(script_3838, 2, arg0, arg1);
  return;
}

static int32_t script_3838(int32_t arg0, int32_t arg1) {
  if (arg1 >= 0) {
    SCNF_WaitFrames_0000(arg1);
  }
  script_37FC(arg0);
  return Script_srchControl(0);
}

static int32_t script_385B(void) {
  int32_t local_F0[4];
  local_F0[1] = Script_scenCreateTask(script_3C10, 2, 1, -0x80000000);
  local_F0[2] = Script_scenCreateTask(script_3C10, 2, 2, -0x80000000);
  local_F0[3] = Script_scenCreateTask(script_3C10, 2, 3, -0x80000000);
  Script_shenSetLightingMode(2);
  Script_SetupLightFromTable(0);
  Script_ProcessFogFromTable(0);
  Script_PREP_Related(-1, 1);
  Script_SetFogColour(&data_C888);
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 1);
  Script_charaSetRefboardFlagTo(CHARA_CHRL, 1);
  Script_SetCharaRefBoardPos(CHARA_AKIR, &data_C878);
  Script_SetCharaRefBoardPos(CHARA_CHRL, &data_C878);
  for (;;) {
    if (task_8 == 5) {
      Script_charaSetRefboardFlagTo(CHARA_AKIR, 2);
      Script_srchControl((void *)local_F0[1]);
      Script_srchControl((void *)local_F0[2]);
      Script_srchControl((void *)local_F0[3]);
      Script_srchControl(0);
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_394D(int32_t arg0, int32_t arg1) {
  Vector4 local_F0;
  Vector3 local_E4;
  float local_E0;
  int32_t local_DC;
  local_DC = arg0 * 14;
  local_F0.x = *(float *)(&data_C6F8 + local_DC);
  (&local_F0)->y = *(float *)(&data_C6F8 + (local_DC + 1));
  (&local_F0)->z = *(float *)(&data_C6F8 + (local_DC + 2));
  (&local_F0)->w = *(float *)(&data_C6F8 + (local_DC + 3));
  local_E4.x = *(float *)(&data_C6F8 + (local_DC + 10));
  (&local_E4)->y = *(float *)(&data_C6F8 + (local_DC + 11));
  (&local_E4)->z = *(float *)(&data_C6F8 + (local_DC + 12));
  local_E0 = *(float *)(&data_C6F8 + (local_DC + 9));
  Script_LightUtils(arg0, 0, 2);
  Script_LightUtils(arg0, 2, &local_F0);
  Script_LightUtils(arg0, 8, &local_E4);
  Script_LightUtils(arg0, 4, local_E0);
  if (arg1 & -0x80000000) {
    Script_lightSetSlotEnabled(arg0, 1);
  }
  if (arg1 & 0x40000000) {
    Script_lightSetSlotEnabled(arg0, 0);
  }
  for (;;) {
    if (arg1 & 0x20000000) {
      if (!(Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 1.0f) >= 0.1f)) {
        Script_lightSetSlotEnabled(arg0, 0);
      } else {
        Script_lightSetSlotEnabled(arg0, 1);
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_3A8D(int32_t arg0, int32_t arg1) {
  Vector4 local_F0;
  Vector3 local_E4;
  int32_t local_DC[2];
  float local_D8;
  float local_D4;
  int32_t local_D0;
  local_D0 = arg0 * 14;
  local_F0.x = *(float *)(&data_C6F8 + local_D0);
  (&local_F0)->y = *(float *)(&data_C6F8 + (local_D0 + 1));
  (&local_F0)->z = *(float *)(&data_C6F8 + (local_D0 + 2));
  (&local_F0)->w = *(float *)(&data_C6F8 + (local_D0 + 3));
  local_E4.x = *(float *)(&data_C6F8 + (local_D0 + 4));
  (&local_E4)->y = *(float *)(&data_C6F8 + (local_D0 + 5));
  (&local_E4)->z = *(float *)(&data_C6F8 + (local_D0 + 6));
  local_D8 = *(float *)(&data_C6F8 + (local_D0 + 7));
  local_DC[0] = *(&data_C6F8 + (local_D0 + 8));
  local_DC[1] = 0;
  local_D4 = *(float *)(&data_C6F8 + (local_D0 + 9));
  Script_LightUtils(arg0, 0, 3);
  Script_LightUtils(arg0, 2, &local_F0);
  Script_LightUtils(arg0, 7, &local_E4);
  Script_LightUtils(arg0, 5, local_D8);
  Script_LightUtils(arg0, 6, (const float *)local_DC);
  Script_LightUtils(arg0, 4, local_D4);
  if (arg1 & -0x80000000) {
    Script_lightSetSlotEnabled(arg0, 1);
  }
  if (arg1 & 0x40000000) {
    Script_lightSetSlotEnabled(arg0, 0);
  }
  for (;;) {
    if (arg1 & 0x20000000) {
      if (!(Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 1.0f) >= 0.1f)) {
        Script_lightSetSlotEnabled(arg0, 0);
      } else {
        Script_lightSetSlotEnabled(arg0, 1);
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static int32_t script_3C10(int32_t arg0, int32_t arg1) {
  Vector4 local_F0;
  Vector3 local_E4;
  Vector3 local_D8;
  int32_t local_D0[2];
  float local_CC;
  int32_t local_C8;
  float local_C4;
  int32_t local_C0;
  local_C0 = arg0 * 14;
  local_F0.x = *(float *)(&data_C6F8 + local_C0);
  (&local_F0)->y = *(float *)(&data_C6F8 + (local_C0 + 1));
  (&local_F0)->z = *(float *)(&data_C6F8 + (local_C0 + 2));
  (&local_F0)->w = *(float *)(&data_C6F8 + (local_C0 + 3));
  local_E4.x = *(float *)(&data_C6F8 + (local_C0 + 4));
  (&local_E4)->y = *(float *)(&data_C6F8 + (local_C0 + 5));
  (&local_E4)->z = *(float *)(&data_C6F8 + (local_C0 + 6));
  local_CC = *(float *)(&data_C6F8 + (local_C0 + 7));
  local_D0[0] = *(&data_C6F8 + (local_C0 + 8));
  local_D0[1] = 0;
  local_C4 = *(float *)(&data_C6F8 + (local_C0 + 9));
  local_D8.x = *(float *)(&data_C6F8 + (local_C0 + 10));
  (&local_D8)->y = *(float *)(&data_C6F8 + (local_C0 + 11));
  (&local_D8)->z = *(float *)(&data_C6F8 + (local_C0 + 12));
  local_C8 = *(&data_C6F8 + (local_C0 + 13));
  Script_LightUtils(arg0, 0, 4);
  Script_LightUtils(arg0, 2, &local_F0);
  Script_LightUtils(arg0, 7, &local_E4);
  Script_LightUtils(arg0, 5, local_CC);
  Script_LightUtils(arg0, 6, (const float *)local_D0);
  Script_LightUtils(arg0, 8, &local_D8);
  Script_LightUtils(arg0, 10, local_C8);
  Script_LightUtils(arg0, 4, local_C4);
  if (arg1 & -0x80000000) {
    Script_lightSetSlotEnabled(arg0, 1);
  }
  if (arg1 & 0x40000000) {
    Script_lightSetSlotEnabled(arg0, 0);
  }
  for (;;) {
    if (arg1 & 0x20000000) {
      if (!(Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 1.0f) >= 0.1f)) {
        Script_lightSetSlotEnabled(arg0, 0);
      } else {
        Script_lightSetSlotEnabled(arg0, 1);
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_3DF9(void) {
  Script_SetMapFlags(0, 1);
  Script_GetSetWeatherSettings(WEATHER_SETTINGS_SET_DISABLE_WEATHER, 1);
  Script_WeatherUtils_0(WeatherDataUtil_ApplyCurrentWeatherOrEnableDateTimeRead, 0);
  Script_WeatherTaskUtil(InitTaskData);
  Script_WeatherTaskUtil(ToggleFogTask, 0);
  Script_WeatherTaskUtil(ToggleSceneLightingFlags, 0);
  for (;;) {
    if (task_8 == 5) {
      Script_WeatherTaskUtil(ToggleFogTask, 1);
      Script_WeatherTaskUtil(ToggleSceneLightingFlags, 1);
      Script_srchControl(0);
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_3E5D(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Script_scenCreateTask(script_3E73, 4, arg0, arg1, arg2, arg3);
  return;
}

static int32_t script_3E73(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  if (arg3 >= 0) {
    SCNF_WaitFrames_0000(arg3);
  }
  Script_CamCtrl(CamCtrlCmd_Reset);
  Script_seqEnterEvtCam(arg0, arg1, arg2);
  return Script_srchControl(0);
}

static void script_3E9E(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Script_scenCreateTask(script_3EB4, 4, arg0, arg1, arg2, arg3);
  return;
}

static int32_t script_3EB4(float arg0, float arg1, float arg2, int32_t arg3) {
  if (arg3 >= 0) {
    SCNF_WaitFrames_0000(arg3);
  }
  Script_SetCamShake(arg0, arg1, arg2);
  return Script_srchControl(0);
}

static void script_3ED9(int32_t arg0, int32_t arg1) {
  Script_scenCreateTask(script_3EEB, 2, arg0, arg1);
  return;
}

__attribute__((scn_string_bindings(5, data_D282, 6, data_D28C)))
static int32_t script_3EEB(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  if (arg1 >= 0) {
    SCNF_WaitFrames_0000(arg1);
  }
  Script_StartOrFreeSEQ(0);
  local_FC = arg0;
  switch (local_FC) {
    case 0:
      Script_ShowUIMessageFromID("E1069B001");
      break;
    case 1:
      Script_ShowUIMessageFromID("E1069B002");
      break;
    case 2:
      Script_ShowUIMessageFromID("E1069C001");
      break;
    case 3:
      Script_ShowUIMessageFromID("E1069D001");
      break;
    case 4:
      Script_ShowUIMessageFromID("E1069B003");
      break;
    case 5:
      Script_ShowUIMessageFromID("E1069B004");
      break;
    case 6:
      Script_ShowUIMessageFromID("E1069B004");
      break;
    case 7:
      Script_ShowUIMessageFromID("E1069B005");
      break;
    case 8:
      Script_ShowUIMessageFromID("E1069C002");
      break;
    case 9:
      Script_ShowUIMessageFromID("E1069A001");
      break;
    case 10:
      Script_ShowUIMessageFromID("E1069C003");
      break;
    case 11:
      Script_ShowUIMessageFromID("E1069B006");
      break;
    case 12:
      Script_ShowUIMessageFromID("E1069A002");
      break;
    case 13:
      Script_ShowUIMessageFromID("E1069B007");
      break;
    case 14:
      Script_ShowUIMessageFromID("E1069A003");
      break;
    case 15:
      Script_ShowUIMessageFromID("E1069B008");
      break;
    case 16:
      Script_ShowUIMessageFromID("E1069A004");
      break;
    case 17:
      Script_ShowUIMessageFromID("E1069B009");
      break;
    case 18:
      Script_ShowUIMessageFromID("E1069B010");
      break;
    case 19:
      Script_ShowUIMessageFromID("E1069B011");
      break;
    case 20:
      Script_ShowUIMessageFromID("E1069E001");
      break;
    case 21:
      Script_ShowUIMessageFromID("E1069F001");
      break;
    case 22:
      Script_ShowUIMessageFromID("E1069B012");
      break;
    case 23:
      Script_ShowUIMessageFromID("E1069A005");
      break;
  }
  return Script_srchControl(0);
}

static void script_410A(int32_t arg0, int32_t arg1) {
  task_290 = arg0;
  task_0 = 0;
  return;
}

static void script_411B(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  if (arg1 >= 0) {
    SCNF_WaitFrames_0000(arg1);
  }
  local_FC = arg0;
  switch (local_FC) {
    case 0:
      SCNT_ShowSubtitle("オート０");
      break;
    case 1:
      SCNT_ShowSubtitle("オート１");
      break;
    case 2:
      SCNT_ShowSubtitle("オート３");
      break;
    case 3:
      SCNT_ShowSubtitle("オート４");
      break;
    case 4:
      SCNT_ShowSubtitle("開始０");
      break;
    case 5:
      SCNT_ShowSubtitle("成功０");
      break;
    case 6:
      SCNT_ShowSubtitle("失敗０");
      break;
    case 7:
      SCNT_ShowSubtitle("死亡０");
      break;
    case 8:
      SCNT_ShowSubtitle("開始１");
      break;
    case 9:
      SCNT_ShowSubtitle("成功１");
      break;
    case 10:
      SCNT_ShowSubtitle("失敗１");
      break;
    case 11:
      SCNT_ShowSubtitle("開始２");
      break;
    case 12:
      SCNT_ShowSubtitle("成功２");
      break;
    case 13:
      SCNT_ShowSubtitle("失敗２");
      break;
    case 14:
      SCNT_ShowSubtitle("開始３");
      break;
    case 15:
      SCNT_ShowSubtitle("成功３");
      break;
    case 16:
      SCNT_ShowSubtitle("失敗３");
      break;
    case 17:
      SCNT_ShowSubtitle("開始４");
      break;
    case 18:
      SCNT_ShowSubtitle("成功４");
      break;
    case 19:
      SCNT_ShowSubtitle("失敗４");
      break;
    case 20:
      SCNT_ShowSubtitle("開始５");
      break;
    case 21:
      SCNT_ShowSubtitle("成功５");
      break;
    case 22:
      SCNT_ShowSubtitle("失敗５");
      break;
    case 23:
      SCNT_ShowSubtitle("開始６");
      break;
    case 24:
      SCNT_ShowSubtitle("成功６");
      break;
    case 25:
      SCNT_ShowSubtitle("失敗６");
      break;
    case 26:
      SCNT_ShowSubtitle("開始７");
      break;
    case 27:
      SCNT_ShowSubtitle("成功７");
      break;
    case 28:
      SCNT_ShowSubtitle("失敗７");
      break;
    case 29:
      SCNT_ShowSubtitle("開始８");
      break;
    case 30:
      SCNT_ShowSubtitle("成功８");
      break;
    case 31:
      SCNT_ShowSubtitle("失敗８");
      break;
    case 32:
      SCNT_ShowSubtitle("開始９");
      break;
    case 33:
      SCNT_ShowSubtitle("成功９");
      break;
    case 34:
      SCNT_ShowSubtitle("失敗９");
      break;
    case 35:
      SCNT_ShowSubtitle("開始１０");
      break;
    case 36:
      SCNT_ShowSubtitle("成功１０");
      break;
    case 37:
      SCNT_ShowSubtitle("失敗１０");
      break;
    case 38:
      SCNT_ShowSubtitle("開始１１");
      break;
    case 39:
      SCNT_ShowSubtitle("成功１１");
      break;
    case 40:
      SCNT_ShowSubtitle("失敗１１");
      break;
    case 41:
      SCNT_ShowSubtitle("開始１２");
      break;
    case 42:
      SCNT_ShowSubtitle("成功１２");
      break;
    case 43:
      SCNT_ShowSubtitle("失敗１２");
      break;
    case 44:
      SCNT_ShowSubtitle("開始１３");
      break;
    case 45:
      SCNT_ShowSubtitle("成功１３");
      break;
    case 46:
      SCNT_ShowSubtitle("失敗１３");
      break;
    case 47:
      SCNT_ShowSubtitle("開始１４");
      break;
    case 48:
      SCNT_ShowSubtitle("成功１４");
      break;
    case 49:
      SCNT_ShowSubtitle("失敗１４");
      break;
  }
  Script_srchControl(0);
  return;
}

static void script_448D(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
  Script_scenCreateTask(script_44A7, 6, arg0, arg1, arg2, arg3, arg4, arg5);
  return;
}

static int32_t script_44A7(int32_t arg0, int32_t arg1, int32_t arg2, float arg3, float arg4, int32_t arg5) {
  Vector3 local_F4;
  if (arg5 >= 0) {
    SCNF_WaitFrames_0000(arg5);
  }
  Script_EVT_GetPosition(arg0, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  Script_Execute3DSoundCommand_Wrapper(arg1, arg2, &local_F4, arg3, arg4, 0);
  return Script_srchControl(0);
}

static void script_44E1(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  Script_scenCreateTask(script_44F9, 5, arg0, arg1, arg2, arg3, arg4);
  return;
}

static int32_t script_44F9(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  if (arg4 >= 0) {
    SCNF_WaitFrames_0000(arg4);
  }
  Script_FACE(arg0, arg1, arg2, arg3);
  return Script_srchControl(0);
}

static void script_4520(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  Script_scenCreateTask(script_4538, 5, arg0, arg1, arg2, arg3, arg4);
  return;
}

static int32_t script_4538(int32_t arg0, int32_t arg1, float arg2, int32_t arg3, int32_t arg4) {
  Vector3 local_F4;
  int32_t local_F0;
  if (arg3 > 0) {
    SCNF_WaitFrames_0000(arg3);
  }
  local_F0 = arg4;
  while (local_F0 > 0) {
    Script_EVT_GetPosition(arg1, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
    (&local_F4)->y = (&local_F4)->y + arg2;
    Script_SetLookPointDetails(arg0, 15, &local_F4, 0);
    local_F0 = local_F0 - 1;
    SCNF_WaitFrames_0000(1);
  }
  Script_SetCharaLookAtPosition(arg0, -98312, 0, 0);
  return Script_srchControl(0);
}

static void script_45B0(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Script_scenCreateTask(script_45C6, 4, arg0, arg1, arg2, arg3);
  return;
}

static int32_t script_45C6(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  if (arg3 > 0) {
    SCNF_WaitFrames_0000(arg3);
  }
  if (arg1) {
    Script_charaDisplay(arg0, CDM_Show);
  } else {
    Script_charaDisplay(arg0, CDM_Hide);
  }
  Script_scenToggleActiveFor(arg0, arg2);
  return Script_srchControl(0);
}

static void script_4602(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5, int32_t arg6) {
  Script_scenCreateTask(script_461E, 7, arg2, arg6, arg3, arg4, arg0, arg1, arg5);
  return;
}

static int32_t script_461E(int32_t arg0, int32_t arg1, int32_t arg2, float arg3, int32_t arg4, int32_t arg5, int32_t arg6) {
  if (arg1 > 0) {
    SCNF_WaitFrames_0000(arg1);
  }
  Script_charaToggleMotEnabled(arg4, 1);
  Script_motmToggleFlags(arg4, 2, 0x80);
  Script_PlayCharaMotion(arg4, arg0, arg2, -1.0f, -1.0f, 1.0f, 0);
  Script_SetCharaMotionPlaybackRate(arg4, arg3);
  if (arg6) {
    Script_motmToggleFlags(arg4, 1, 0x80);
  }
  return Script_srchControl(0);
}

static void script_467F(int32_t arg0, int32_t arg1) {
  Vector3 local_F4;
  Vector3i local_E8;
  local_F4.x = 0.0f;
  (&local_F4)->y = -0.85f;
  (&local_F4)->z = -0.2f;
  local_E8.x = 0;
  (&local_E8)->y = 0;
  (&local_E8)->z = 0;
  Script_MoveObjectToObject(arg0, -1);
  Script_fixoAttachEx(arg0, arg1, 0, &local_F4, &local_E8);
  return;
}

static void script_46CF(int32_t arg0, int32_t arg1) {
  Vector3 local_F4;
  Vector3i local_E8;
  local_F4.x = 0.9f;
  (&local_F4)->y = -0.03f;
  (&local_F4)->z = -0.01f;
  local_E8.x = 0;
  (&local_E8)->y = 0xC000;
  (&local_E8)->z = 12925;
  Script_MoveObjectToObject(arg0, -1);
  Script_fixoAttachEx(arg0, arg1, 20, &local_F4, &local_E8);
  return;
}

static void script_4726(int32_t arg0, int32_t arg1) {
  Vector3 local_F4;
  Vector3i local_E8;
  local_F4.x = 0.0f;
  (&local_F4)->y = 0.0f;
  (&local_F4)->z = 0.3f;
  local_E8.x = 0;
  (&local_E8)->y = 0;
  (&local_E8)->z = 0;
  Script_MoveObjectToObject(arg0, arg1);
  Script_EVT_SetPosition(arg0, POS_XYZ, &local_F4);
  Script_EVT_SetRotation(arg0, 0x38000000, &local_E8, 0);
  return;
}

static void script_4780(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  Vector3 local_F0;
  Vector3i local_E4;
  local_F0.x = 0.0f;
  (&local_F0)->y = 0.0f;
  (&local_F0)->z = 0.0f;
  local_E4.x = 0;
  (&local_E4)->y = 0;
  (&local_E4)->z = 0;
  local_FC = Script_CEWPRelated(arg1);
  Script_MoveObjectToObject(arg0, -1);
  Script_MoveObjectToObject(arg1, arg0);
  Script_EVT_SetPosition(arg1, POS_XYZ, &local_F0);
  Script_EVT_SetRotation(arg1, 0x38000000, &local_E4, 0);
  Script_MoveObjectToObject(arg1, local_FC);
  Script_MoveObjectToObject(arg0, arg1);
  Script_EVT_SetPosition(arg0, POS_XYZ, &local_F0);
  Script_EVT_SetRotation(arg0, 0x38000000, &local_E4, 0);
  return;
}

static void script_4817(int32_t arg0) {
  Script_fixoClearAttachment(arg0);
  return;
}

static void script_4820(int32_t arg0) {
  Script_MoveObjectToObject(arg0, 0x304D5544);
  return;
}

static void script_482E(int32_t arg0) {
  Script_MoveObjectToObject(arg0, 0x304D5544);
  return;
}

static void script_483C(void) {
  Script_ReadModelFormat7(*task_74, 0, data_D4DC, 10);
  Script_ReadModelFormat7(*task_74, 1, data_D504, 10);
  Script_ReadModelFormat7(task_74[1], 0, data_D4DC, 10);
  Script_ReadModelFormat7(task_74[1], 1, data_D504, 10);
  Script_ReadModelFormat7(task_74[2], 0, data_D4DC, 10);
  Script_ReadModelFormat7(task_74[3], 0, data_D4DC, 10);
  Script_ReadModelFormat7(task_74[3], 1, data_D504, 10);
  Script_SetLampState(0, SHEN_NAME('BIK0'), 0);
  Script_SetLampState(0, SHEN_NAME('BIK1'), 0);
  Script_SetLampState(0, SHEN_NAME('BIK2'), 0);
  Script_charaDisplay(SHEN_NAME('MKZ0'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('KBK0'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('KBK1'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('KBD0'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('KBD1'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('BIK0'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('BIK2'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('BK0A'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('BK2A'), CDM_Hide);
  Script_charaDisplay(CHARA_BIKA, CDM_Hide);
  Script_charaDisplay(SHEN_NAME('BIKC'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('BIKD'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('BIKY'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('RYY0'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('RYY1'), CDM_Hide);
  script_4817(SHEN_NAME('BIK2'));
  Script_MoveObjectToObject(SHEN_NAME('BIK0'), 0x30424D44);
  Script_MoveObjectToObject(SHEN_NAME('BIK1'), 0x31424D44);
  Script_MoveObjectToObject(SHEN_NAME('BIK2'), 0x32424D44);
  Script_MoveObjectToObject(*task_74, 0x304D5544);
  Script_MoveObjectToObject(task_74[1], 0x304D5544);
  Script_MoveObjectToObject(task_74[2], 0x304D5544);
  Script_MoveObjectToObject(task_74[3], 0x304D5544);
  Script_EVT_SetPosition(*task_74, POS_XYZ, &data_D4C4);
  Script_EVT_SetRotation(*task_74, 0x38000000, &data_D4D0, 0);
  Script_EVT_SetPosition(task_74[1], POS_XYZ, &data_D4C4);
  Script_EVT_SetRotation(task_74[1], 0x38000000, &data_D4D0, 0);
  Script_EVT_SetPosition(task_74[2], POS_XYZ, &data_D4C4);
  Script_EVT_SetRotation(task_74[2], 0x38000000, &data_D4D0, 0);
  Script_EVT_SetPosition(task_74[3], POS_XYZ, &data_D4C4);
  Script_EVT_SetRotation(task_74[3], 0x38000000, &data_D4D0, 0);
  Script_EVT_SetPosition(SHEN_NAME('BIK0'), POS_XYZ, &data_D4C4);
  Script_EVT_SetRotation(SHEN_NAME('BIK0'), 0x38000000, &data_D4D0, 0);
  Script_EVT_SetPosition(SHEN_NAME('BIK1'), POS_XYZ, &data_D4C4);
  Script_EVT_SetRotation(SHEN_NAME('BIK1'), 0x38000000, &data_D4D0, 0);
  Script_EVT_SetPosition(SHEN_NAME('BIK2'), POS_XYZ, &data_D4C4);
  Script_EVT_SetRotation(SHEN_NAME('BIK2'), 0x38000000, &data_D4D0, 0);
  Script_lightSetSlotEnabled(1, 0);
  Script_lightSetSlotEnabled(2, 0);
  Script_lightSetSlotEnabled(3, 0);
  Script_scenCreateTask(script_0484, 2, 525481, -1);
  Script_scenCreateTask(script_0484, 2, 984233, -1);
  Script_scenCreateTask(script_0484, 2, 0x1104A9, -1);
  return;
}

static void script_4B7E(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  int32_t unused_F8;
  int32_t local_EC[3];
  int32_t unused_E8;
  GameDateTime local_E0;
  int32_t local_DC;
  local_EC[0] = Script_scenCreateTask(script_355F, 5, 0x30424D44, 4, 0x3DCCCCCD, 0, &data_D210);
  local_EC[1] = Script_scenCreateTask(script_355F, 5, 0x31424D44, 5, 0x3DCCCCCD, 0, &data_D210);
  local_EC[2] = Script_scenCreateTask(script_355F, 5, 0x32424D44, 6, 0x3DCCCCCD, 0, &data_D210);
  task_1CC[4] = 0x304B4942;
  task_1CC[5] = 0x314B4942;
  task_1CC[6] = 0x324B4942;
  SCNF_WaitFrames_0000(5);
  Script_SetSfxOverlayTimeoutTuning(0, SfxMode_SessionAction);
  script_1330(13);
  Script_ReturnTimeDate(&local_E0);
  for (;;) {
    Script_SetNewDateTime(&local_E0);
    Script_WeatherTaskUtil(FreeTaskData);
    Script_WeatherTaskUtil(InitTaskData);
    Script_uiUtils(0, 21, 0);
    if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_MARKS_BROTHER_KILLED) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_LAN_DI_STILL_IN_HARBOR) == 0)) {
    } else {
      Script_filmCheckOrSetTransitionUtil(0, 255, 0, 0, 0, 0, 0, 0, 0);
      for (;;) {
        Script_PrintDebugString(15, 14, "PRESS A BUTTON");
        if (Script_GetInputData(0, 2) & 0x400) {
          break;
        }
        SCNF_WaitFrames_0000(1);
      }
    }
    Script_filmCheckOrSetTransitionUtil(60, 255, 0, 0, 0, 0, 0, 0, 0);
    script_483C();
    script_56E9(arg0, arg1);
    script_1162(0, 3);
    L004CE9:;
    Script_executeAicaCommand(475048, 0, 0);
    Script_executeAicaCommand(1184, 2, 110);
    local_FC = Script_scenCreateTask(script_50C4, 2, arg0, arg1);
    task_C8 = 0;
    script_1247(0, 3);
    for (;;) {
      if (task_C8 >= 10) {
        Script_srchControl((void *)local_FC);
        local_DC = task_290;
        script_410A(7, -1);
        SCNF_WaitFrames_0000(90);
        break;
      } else {
        if (task_C8 == -1) {
          Script_srchControl((void *)local_FC);
          break;
          goto L004D6D;
        }
      }
      L004D6D:;
      SCNF_WaitFrames_0000(1);
    }
    if (task_C8 == -1) {
      script_37FC(6);
      script_5E06(arg0, arg1);
    } else {
      Script_executeAicaCommand(2720, 2, 30);
      Script_filmCheckOrSetTransitionUtil(60, 0, 0, 0, 0, 255, 0, 0, 0);
      while (Script_filmCheckOrSetTransitionUtil(-1)) {
        SCNF_WaitFrames_0000(1);
      }
      script_37FC(6);
      Script_sicnUtils(0, 1, 1);
      SCNF_WaitFrames_0000(90);
      Script_sicnUtils(2, 1, 0);
      Script_SetSfxOverlayTimeoutTuning(1, SfxMode_SessionAction);
      script_134E();
      goto L00507B;
    }
    script_697A(arg0, arg1);
    local_FC = Script_scenCreateTask(script_6326, 2, arg0, arg1);
    task_C8 = 0;
    for (;;) {
      if (task_C8 >= 10) {
        Script_srchControl((void *)local_FC);
        local_DC = task_290;
        if (local_DC == 10) {
          script_410A(7, -1);
          SCNF_WaitFrames_0000(90);
        } else if (local_DC == 13) {
          script_410A(7, -1);
          SCNF_WaitFrames_0000(90);
        } else {
          script_410A(7, -1);
          SCNF_WaitFrames_0000(90);
        }
        break;
      } else {
        if (task_C8 == -1) {
          Script_srchControl((void *)local_FC);
          break;
          goto L004EB1;
        }
      }
      L004EB1:;
      SCNF_WaitFrames_0000(1);
    }
    if (task_C8 == -1) {
      script_37FC(4);
      script_37FC(5);
      script_37FC(6);
      script_6CE0(arg0, arg1);
    } else {
      Script_executeAicaCommand(2720, 2, 30);
      Script_filmCheckOrSetTransitionUtil(60, 0, 0, 0, 0, 255, 0, 0, 0);
      while (Script_filmCheckOrSetTransitionUtil(-1)) {
        SCNF_WaitFrames_0000(1);
      }
      script_37FC(4);
      script_37FC(5);
      script_37FC(6);
      Script_sicnUtils(0, 1, 1);
      SCNF_WaitFrames_0000(90);
      Script_sicnUtils(2, 1, 0);
      Script_SetSfxOverlayTimeoutTuning(1, SfxMode_SessionAction);
      script_134E();
      goto L00507B;
    }
    script_7DC0(arg0, arg1);
    local_FC = Script_scenCreateTask(script_6D0A, 2, arg0, arg1);
    task_C8 = 0;
    for (;;) {
      if (task_C8 >= 10) {
        Script_srchControl((void *)local_FC);
        local_DC = task_290;
        script_410A(7, -1);
        SCNF_WaitFrames_0000(90);
        break;
      } else {
        if (task_C8 == -1) {
          Script_srchControl((void *)local_FC);
          break;
          goto L004FDB;
        }
      }
      L004FDB:;
      SCNF_WaitFrames_0000(1);
    }
    if (task_C8 == -1) {
      script_37FC(6);
      script_8009(arg0, arg1);
      goto L00507B;
    } else {
      Script_executeAicaCommand(2720, 2, 30);
      Script_filmCheckOrSetTransitionUtil(60, 0, 0, 0, 0, 255, 0, 0, 0);
      while (Script_filmCheckOrSetTransitionUtil(-1)) {
        SCNF_WaitFrames_0000(1);
      }
      script_37FC(6);
      Script_sicnUtils(0, 1, 1);
      SCNF_WaitFrames_0000(90);
      Script_sicnUtils(2, 1, 0);
      Script_SetSfxOverlayTimeoutTuning(1, SfxMode_SessionAction);
      script_134E();
      goto L00507B;
    }
    goto L004CE9;
    L00507B:;
    if (task_C8 == -1) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_SetSfxOverlayTimeoutTuning(2, SfxMode_SessionAction);
  Script_shenClearFreeState(0);
  script_1396();
  Script_srchControl((void *)*(int32_t *)local_EC);
  Script_srchControl((void *)local_EC[1]);
  Script_srchControl((void *)local_EC[2]);
  return;
}

static int32_t script_50C4(int32_t arg0, int32_t arg1) {
  Script_charaToggleMotEnabled(arg0, 1);
  Script_charaToggleMotEnabled(task_74[1], 1);
  Script_charaToggleMotEnabled(task_74[2], 1);
  Script_charaToggleMotEnabled(task_74[3], 1);
  Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_10);
  Script_motmSetBlendFrames(task_74[1], MOTM_BLEND_FRAMES_10);
  Script_motmSetBlendFrames(task_74[2], MOTM_BLEND_FRAMES_10);
  Script_motmSetBlendFrames(task_74[3], MOTM_BLEND_FRAMES_10);
  script_45B0(arg0, 1, 0, -1);
  script_45B0(task_74[1], 1, 0, -1);
  script_45B0(task_74[2], 1, 0, -1);
  script_45B0(task_74[3], 1, 0, -1);
  Script_scenCreateTask(script_0484, 2, 1193, -1);
  Script_scenCreateTask(script_0484, 2, 1193, 20);
  Script_scenCreateTask(script_0484, 2, 66729, -1);
  Script_scenCreateTask(script_0484, 2, 132265, -1);
  Script_scenCreateTask(script_0484, 2, 787625, -1);
  script_410A(4, -1);
  script_3ED9(4, -1);
  script_44E1(arg0, 2, 0, 10, -1);
  script_3E5D(8403, SHEN_NAME('DUM0'), SHEN_NAME('DUM0'), -1);
  script_4602(task_74[1], 1, 20995, 0, 0x3F800000, 0, -1);
  script_37C3(4, 0, -1);
  script_370B(4, 0, 0x3F000000, 0x428C0000, 18);
  script_370B(4, 0x3F000000, 0x3DCCCCCD, 0x41F00000, 120);
  script_370B(4, 0x3DCCCCCD, 0x3E4CCCCD, 0x41F00000, 240);
  script_0EA8(arg0, -1, -0x3D36CCCD, 0x42813333, 0x43A8C000, -1);
  script_09DB(arg1, 0, 1, 0);
  if (task_BC == 0) {
    script_410A(6, -1);
    script_44E1(arg0, 4, 0, 16, 10);
    script_44E1(arg0, 2, 0, 16, 30);
    script_3E5D(8405, SHEN_NAME('DUM0'), SHEN_NAME('DUM0'), -1);
  } else {
    script_410A(5, -1);
    script_3E5D(8404, SHEN_NAME('DUM0'), SHEN_NAME('DUM0'), -1);
  }
  script_37C3(5, 0, -1);
  SCNF_WaitFrames_0000(20);
  Script_scenCreateTask(script_2E35, 2, 1, -1);
  Script_scenCreateTask(script_0484, 2, 1193, -1);
  Script_scenCreateTask(script_0484, 2, 1193, 20);
  Script_scenCreateTask(script_0484, 2, 66729, -1);
  Script_scenCreateTask(script_0484, 2, 0x1204A9, -1);
  script_410A(8, -1);
  script_3ED9(5, -1);
  script_370B(5, 0, 0x3EE66666, 0x42700000, 10);
  script_370B(5, 0x3EE66666, 0x3DCCCCCD, 0x41F00000, 120);
  script_370B(5, 0x3DCCCCCD, 0x3E4CCCCD, 0x41F00000, 260);
  script_09DB(arg1, 1, 2, 0);
  if (task_BC == 0) {
    script_410A(10, -1);
    script_44E1(arg0, 4, 0, 16, 10);
    script_44E1(arg0, 2, 0, 16, 30);
    script_3E5D(8408, SHEN_NAME('DUM0'), SHEN_NAME('DUM0'), -1);
    script_0EA8(arg0, -1, -0x3D336666, 0x42853333, 0x43964000, -1);
  } else {
    script_410A(9, -1);
    script_3E5D(8407, SHEN_NAME('DUM0'), SHEN_NAME('DUM0'), -1);
    script_0EA8(arg0, -1, -0x3D336666, 0x42853333, 0x43964000, -1);
  }
  SCNF_WaitFrames_0000(50);
  Script_scenCreateTask(script_2E35, 2, 2, -1);
  Script_scenCreateTask(script_0484, 2, 853161, -1);
  Script_scenCreateTask(script_0484, 2, 853161, 20);
  Script_scenCreateTask(script_0484, 2, 66729, 10);
  Script_scenCreateTask(script_0484, 2, 787625, 20);
  script_410A(11, -1);
  script_3E5D(8409, SHEN_NAME('DUM0'), SHEN_NAME('DUM0'), -1);
  script_37C3(6, 0, 10);
  script_370B(6, 0, 0x3E99999A, 0x41F00000, 10);
  script_0EA8(arg0, -1, -0x3D35999A, 0x42863333, 0x4394C000, -1);
  script_09DB(arg1, 2, 3, 0);
  if (task_BC == 0) {
    Script_lightSetSlotEnabled(1, 0);
    script_410A(13, -1);
    script_44E1(arg0, 4, 0, 16, 10);
    script_3E5D(8411, SHEN_NAME('DUM0'), SHEN_NAME('DUM0'), -1);
    script_3E9E(0x3F000000, 0x3E4CCCCD, 0x3D4CCCCD, 30);
    task_C8 = 10;
    SCNF_WaitFrames_0000(1);
  } else {
    Script_lightSetSlotEnabled(1, 0);
    script_410A(12, -1);
    script_3E5D(8410, SHEN_NAME('DUM0'), SHEN_NAME('DUM0'), -1);
    script_107B(arg0, -0x3DB80000, -1);
    script_367C(6, 0.1f, &data_D210, 0);
    script_37C3(6, 0x3E99999A, -1);
    script_370B(6, 0x3E99999A, 0, 0x42F00000, -1);
    SCNF_WaitFrames_0000(21);
    Script_scenCreateTask(script_2555, 3, CHARA_BIKA, SHEN_NAME('BIK2'), -1);
    Script_scenCreateTask(script_3487, 3, SHEN_NAME('BIK2'), 30, 90);
    script_45B0(SHEN_NAME('RY20'), 0, 0, -1);
    script_45B0(SHEN_NAME('RY21'), 0, 0, -1);
    script_4520(arg0, SHEN_NAME('BIK0'), 0x3F800000, 50, 90);
    script_4820(task_74[3]);
    SCNF_WaitFrames_0000(50);
  }
  SCNF_WaitFrames_0000(90);
  task_C8 = -1;
  SCNF_WaitFrames_0000(1);
  end:;
}

static void script_56E9(int32_t arg0, int32_t arg1) {
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 50.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 50.0f);
  Script_charaToggleMotEnabled(arg0, 1);
  Script_charaToggleMotEnabled(task_74[1], 1);
  Script_charaToggleMotEnabled(task_74[2], 1);
  Script_charaToggleMotEnabled(task_74[3], 1);
  Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_IMMEDIATE);
  Script_motmSetBlendFrames(task_74[1], MOTM_BLEND_FRAMES_IMMEDIATE);
  Script_motmSetBlendFrames(task_74[2], MOTM_BLEND_FRAMES_IMMEDIATE);
  Script_motmSetBlendFrames(task_74[3], MOTM_BLEND_FRAMES_IMMEDIATE);
  script_4726(task_74[1], 0x304B4942);
  script_4726(task_74[2], 0x314B4942);
  script_4726(task_74[3], 0x324B4942);
  script_367C(4, 0.1f, &data_CE48, 1);
  script_367C(5, 0.1f, &data_D09C, 1);
  script_367C(6, 0.1f, &data_D194, 1);
  script_0EA8(arg0, -1, -0x3D360000, 0x42856666, 0x43A8C000, -1);
  script_0EA8(SHEN_NAME('DMB0'), -1, -0x3D380000, 0x426C0000, 0x43340000, -1);
  script_0EA8(SHEN_NAME('DMB1'), -1, -0x3D41999A, 0x4274CCCD, 0x42F00000, -1);
  script_0EA8(SHEN_NAME('DMB2'), -1, -0x3D3E0000, 0x426C0000, 0x43196666, -1);
  script_410A(0, -1);
  script_3ED9(1, 150);
  script_3ED9(2, 240);
  script_3ED9(3, 360);
  script_44E1(arg0, 0, 0, 16, -1);
  script_44E1(arg0, 4, 0, 16, 60);
  script_44E1(arg0, 2, 0, 16, 90);
  script_3E5D(8456, SHEN_NAME('DUM1'), SHEN_NAME('DUM1'), -1);
  script_3E5D(8400, SHEN_NAME('DUM1'), SHEN_NAME('DUM1'), 62);
  script_3E5D(8401, SHEN_NAME('DUM0'), SHEN_NAME('DUM0'), 86);
  script_3E5D(0x2100, SHEN_NAME('DMB0'), SHEN_NAME('DMB0'), 150);
  script_3E5D(8449, SHEN_NAME('DMB1'), SHEN_NAME('DMB1'), 240);
  script_3E5D(8450, SHEN_NAME('DMB2'), SHEN_NAME('DMB2'), 360);
  script_3E5D(8402, SHEN_NAME('DUM1'), SHEN_NAME('DUM1'), 488);
  script_4602(arg0, arg1, 20997, 0, 0x3F800000, 0, -1);
  script_4602(task_74[1], 1, 20994, 0, 0x3F800000, 0, -1);
  script_4602(task_74[1], 1, 20993, 0, 0x3F800000, 0, 505);
  script_4602(task_74[2], 2, 21010, 0, 0x3F800000, 0, -1);
  script_4602(task_74[3], 3, 20994, 0, 0x3F800000, 0, -1);
  script_4602(task_74[3], 3, 20993, 0, 0x3F800000, 0, 605);
  if (0) {
    Script_charaDisplay(task_74[1], CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(task_74[1], CDM_Show);
      goto L005A31;
    }
  }
  L005A31:;
  Script_scenToggleVisibleFor(task_74[1], 1);
  if (0) {
    Script_charaDisplay(task_74[2], CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(task_74[2], CDM_Show);
      goto L005A6B;
    }
  }
  L005A6B:;
  Script_scenToggleVisibleFor(task_74[2], 1);
  if (0) {
    Script_charaDisplay(task_74[3], CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(task_74[3], CDM_Show);
      goto L005AA5;
    }
  }
  L005AA5:;
  Script_scenToggleVisibleFor(task_74[3], 1);
  script_45B0(arg0, 1, 0, -1);
  script_45B0(task_74[1], 1, 0, -1);
  script_45B0(task_74[2], 1, 0, -1);
  script_45B0(task_74[3], 1, 0, -1);
  script_45B0(SHEN_NAME('RY00'), 0, 0, -1);
  script_45B0(SHEN_NAME('RY01'), 0, 0, -1);
  script_45B0(SHEN_NAME('RY10'), 0, 0, -1);
  script_45B0(SHEN_NAME('RY11'), 0, 0, -1);
  script_45B0(SHEN_NAME('RY20'), 0, 0, -1);
  script_45B0(SHEN_NAME('RY21'), 0, 0, -1);
  script_45B0(SHEN_NAME('BIK0'), 1, 0, -1);
  script_45B0(SHEN_NAME('BIK1'), 1, 0, -1);
  script_45B0(SHEN_NAME('BIK2'), 1, 0, -1);
  script_45B0(CHARA_BIKA, 0, 0, -1);
  script_45B0(SHEN_NAME('BIKY'), 0, 0, -1);
  script_45B0(SHEN_NAME('RY00'), 1, 0, 55);
  script_45B0(SHEN_NAME('RY01'), 1, 0, 55);
  script_45B0(SHEN_NAME('RY10'), 1, 0, 59);
  script_45B0(SHEN_NAME('RY11'), 1, 0, 59);
  script_45B0(SHEN_NAME('RY20'), 1, 0, 57);
  Script_scenCreateTask(script_2E35, 2, 0, 475);
  Script_scenCreateTask(script_0484, 2, 0x2803AB, 20);
  Script_scenCreateTask(script_0484, 2, 0x2803AB, 38);
  Script_scenCreateTask(script_0484, 2, 0x2803AB, 65);
  Script_lightSetSlotEnabled(1, 0);
  SCNF_WaitFrames_0000(55);
  Script_lightSetSlotEnabled(1, 1);
  Script_filmCheckOrSetTransitionUtil(90, 255, 255, 255, 255, 0, 0, 0, 0);
  Script_scenCreateTask(script_0484, 2, 853161, -1);
  Script_scenCreateTask(script_0484, 2, 853161, 30);
  Script_scenCreateTask(script_0484, 2, 853161, 40);
  Script_scenCreateTask(script_0484, 2, 459945, 40);
  Script_scenCreateTask(script_0484, 2, 918697, 60);
  Script_scenCreateTask(script_0484, 2, 0x1004A9, 70);
  Script_scenCreateTask(script_0484, 2, 853161, 100);
  Script_scenCreateTask(script_0484, 2, 853161, 120);
  Script_scenCreateTask(script_0484, 2, 853161, 180);
  Script_scenCreateTask(script_0484, 2, 853161, 250);
  Script_scenCreateTask(script_0484, 2, 853161, 300);
  Script_scenCreateTask(script_0484, 2, 853161, 400);
  Script_scenCreateTask(script_0484, 2, 853161, 420);
  Script_scenCreateTask(script_0484, 2, 787625, 440);
  Script_scenCreateTask(script_0484, 2, 525481, 440);
  Script_scenCreateTask(script_0484, 2, 984233, 470);
  Script_scenCreateTask(script_0484, 2, 0x1104A9, 500);
  SCNF_WaitFrames_0000(10);
  Script_executeAicaCommand(409512, 0, 0);
  Script_executeAicaCommand(1184, 2, 110);
  SCNF_WaitFrames_0000(456);
  Script_executeAicaCommand(2720, 2, 30);
  return;
}

static void script_5E06(int32_t arg0, int32_t arg1) {
  Script_charaToggleMotEnabled(arg0, 1);
  Script_charaToggleMotEnabled(task_74[1], 1);
  Script_charaToggleMotEnabled(task_74[2], 1);
  Script_charaToggleMotEnabled(task_74[3], 1);
  Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_15);
  Script_motmSetBlendFrames(task_74[1], MOTM_BLEND_FRAMES_15);
  Script_motmSetBlendFrames(task_74[2], MOTM_BLEND_FRAMES_15);
  Script_motmSetBlendFrames(task_74[3], MOTM_BLEND_FRAMES_15);
  script_10B4(SHEN_NAME('BIK2'), 0, 0, 0x42700000, -1);
  script_10F5(SHEN_NAME('BIK2'), 0x42700000, 0, 0x42000000, 55);
  script_107B(arg0, 0x43340000, 146);
  script_0EA8(arg0, -1, -0x3D2DE666, 0x4289CCCD, 0x43340000, -1);
  script_0EA8(SHEN_NAME('DMB2'), -1, -0x3D314CCD, 0x428E4CCD, 0, -1);
  script_410A(1, -1);
  script_44E1(arg0, 2, 0, 16, -1);
  script_3E5D(8412, SHEN_NAME('DUM0'), SHEN_NAME('DUM0'), -1);
  script_3E5D(8413, SHEN_NAME('DUM0'), SHEN_NAME('DUM0'), 135);
  script_3E5D(8424, SHEN_NAME('DUM0'), SHEN_NAME('DMB2'), 231);
  script_3E5D(8473, SHEN_NAME('DUM0'), SHEN_NAME('DMB2'), 302);
  script_4602(arg0, arg1, 20996, 0, 0x40000000, 0, -1);
  script_4602(arg0, arg1, 21008, 0, 0x40000000, 0, 146);
  script_4602(arg0, arg1, 21000, 0x437A0000, 0x40000000, 0, 204);
  script_4602(arg0, arg1, 21000, 0, 0x40400000, 0, 280);
  script_45B0(arg0, 1, 0, -1);
  script_45B0(task_74[1], 0, 0, -1);
  script_45B0(task_74[2], 0, 0, -1);
  script_45B0(task_74[3], 1, 0, -1);
  script_45B0(SHEN_NAME('BIK0'), 0, 0, -1);
  script_45B0(SHEN_NAME('BIK1'), 0, 0, -1);
  script_45B0(SHEN_NAME('BIK2'), 1, 0, -1);
  script_45B0(CHARA_BIKA, 0, 0, -1);
  script_45B0(SHEN_NAME('BIKY'), 0, 0, -1);
  script_45B0(SHEN_NAME('RY00'), 0, 0, -1);
  script_45B0(SHEN_NAME('RY01'), 0, 0, -1);
  script_45B0(SHEN_NAME('RY10'), 0, 0, -1);
  script_45B0(SHEN_NAME('RY11'), 0, 0, -1);
  script_45B0(SHEN_NAME('RY20'), 0, 0, -1);
  script_45B0(SHEN_NAME('RY20'), 1, 0, 231);
  script_45B0(SHEN_NAME('RY20'), 0, 0, 302);
  script_45B0(SHEN_NAME('RY20'), 1, 0, 339);
  script_367C(6, 0.1f, &data_C898, 1);
  script_37C3(6, 0, 204);
  script_370B(6, 0, 0x3E99999A, 0x42200000, 204);
  Script_scenCreateTask(script_2E35, 2, 2, 146);
  Script_scenCreateTask(script_2E35, 2, 2, 204);
  Script_scenCreateTask(script_0484, 2, 0x2803AB, 10);
  Script_scenCreateTask(script_0484, 2, 0x2803AB, 20);
  Script_scenCreateTask(script_0484, 2, 0x2803AB, 30);
  Script_scenCreateTask(script_0484, 2, 394409, 60);
  Script_scenCreateTask(script_0484, 2, 853161, 128);
  Script_scenCreateTask(script_0484, 2, 132265, 150);
  Script_scenCreateTask(script_0484, 2, 787625, 150);
  Script_scenCreateTask(script_0484, 2, 787625, 210);
  Script_scenCreateTask(script_0484, 2, 0x1304A9, 230);
  Script_scenCreateTask(script_0484, 2, 132265, 280);
  Script_scenCreateTask(script_0484, 2, 0x1204A9, 300);
  Script_scenCreateTask(script_0484, 2, 0x1304A9, 320);
  Script_SetLampState(0, SHEN_NAME('BIK2'), 1);
  SCNF_WaitFrames_0000(146);
  script_467F(SHEN_NAME('BIK2'), arg0);
  SCNF_WaitFrames_0000(58);
  script_4780(arg0, SHEN_NAME('DMB2'));
  SCNF_WaitFrames_0000(27);
  script_46CF(SHEN_NAME('BIK2'), arg0);
  Script_SetLampState(0, SHEN_NAME('BIK2'), 0);
  script_370B(6, 0x3E99999A, 0x3ECCCCCD, 0x42200000, 40);
  SCNF_WaitFrames_0000(140);
  script_37FC(4);
  script_37FC(5);
  script_37FC(6);
  script_4820(task_74[1]);
  script_4820(task_74[2]);
  script_4820(task_74[3]);
  script_4817(SHEN_NAME('BIK2'));
  script_482E(arg0);
  script_4780(SHEN_NAME('BIK2'), SHEN_NAME('DMB2'));
  return;
}

static int32_t script_6326(int32_t arg0, int32_t arg1) {
  Vector3 local_F4;
  Vector3i local_E8;
  Script_charaToggleMotEnabled(arg0, 1);
  Script_charaToggleMotEnabled(task_74[1], 1);
  Script_charaToggleMotEnabled(task_74[2], 1);
  Script_charaToggleMotEnabled(task_74[3], 1);
  Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_10);
  Script_motmSetBlendFrames(task_74[1], MOTM_BLEND_FRAMES_10);
  Script_motmSetBlendFrames(task_74[2], MOTM_BLEND_FRAMES_10);
  Script_motmSetBlendFrames(task_74[3], MOTM_BLEND_FRAMES_10);
  script_45B0(arg0, 1, 0, -1);
  script_45B0(task_74[1], 1, 0, -1);
  script_45B0(task_74[2], 1, 0, -1);
  script_45B0(task_74[3], 1, 0, -1);
  Script_scenCreateTask(script_0484, 2, 66729, -1);
  Script_scenCreateTask(script_0484, 2, 0x1F04A9, 40);
  script_410A(4, -1);
  script_44E1(arg0, 2, 0, 16, -1);
  script_3ED9(8, 30);
  script_09DB(arg1, 9, 2, 0);
  if (task_BC == 0) {
    script_410A(6, -1);
    script_3ED9(9, 30);
    script_44E1(arg0, 4, 0, 16, 10);
    script_44E1(arg0, 2, 0, 16, 30);
    script_3E5D(8452, SHEN_NAME('DMB2'), SHEN_NAME('DMB2'), -1);
    script_3E5D(8453, SHEN_NAME('DMB2'), SHEN_NAME('DMB2'), 40);
    script_370B(6, 0x3E99999A, 0x3E19999A, 0x42200000, -1);
    script_370B(5, 0x3E4CCCCD, 0x3E800000, 0x41F00000, 30);
  } else {
    script_410A(5, -1);
    script_3E5D(8472, SHEN_NAME('DUM1'), SHEN_NAME('DUM1'), -1);
    script_370B(6, 0x3E99999A, 0x3E19999A, 0x42200000, -1);
    script_370B(5, 0x3E4CCCCD, 0x3E800000, 0x41F00000, 30);
  }
  SCNF_WaitFrames_0000(40);
  Script_scenCreateTask(script_0484, 2, 0x1F04A9, -1);
  Script_scenCreateTask(script_0484, 2, 66729, 50);
  script_410A(8, -1);
  script_370B(6, 0x3E19999A, 0x3E99999A, 0x42200000, -1);
  script_370B(5, 0x3E800000, 0x3E4CCCCD, 0x41A00000, -1);
  script_09DB(arg1, 10, 2, 0);
  if (task_BC == 0) {
    script_410A(10, -1);
    script_44E1(arg0, 7, 0, 16, 10);
    script_44E1(arg0, 4, 0, 16, 40);
    script_3E5D(8425, SHEN_NAME('DMB2'), SHEN_NAME('DMB2'), -1);
    script_370B(6, 0x3E99999A, 0, 0x42A00000, 12);
    script_370B(5, 0x3E4CCCCD, 0x3E99999A, 0x41A00000, -1);
    SCNF_WaitFrames_0000(20);
    Script_scenCreateTask(script_2AC7, 3, SHEN_NAME('BIKY'), SHEN_NAME('BIK2'), -1);
    Script_scenCreateTask(script_3487, 3, SHEN_NAME('BIK2'), 10, 120);
    script_45B0(SHEN_NAME('RY20'), 0, 0, -1);
    script_45B0(SHEN_NAME('RY21'), 0, 0, -1);
    task_C8 = 10;
    SCNF_WaitFrames_0000(1);
  } else {
    script_410A(9, -1);
    script_3E5D(8451, SHEN_NAME('DMB2'), SHEN_NAME('DMB2'), -1);
    script_370B(6, 0x3E99999A, 0x3E800000, 0x41800000, 12);
    script_370B(5, 0x3E4CCCCD, 0x3E99999A, 0x41A00000, -1);
  }
  Script_scenCreateTask(script_0484, 2, 66729, 40);
  script_410A(11, -1);
  script_09DB(arg1, 11, 2, 0);
  if (task_BC == 0) {
    script_410A(13, -1);
    script_44E1(arg0, 7, 0, 16, 30);
    script_44E1(arg0, 4, 0, 16, 50);
    script_3E5D(8431, SHEN_NAME('DMB2'), SHEN_NAME('DMB2'), -1);
    script_370B(6, 0x3E99999A, 0, 0x42A00000, -1);
    script_370B(5, 0x3E99999A, 0x3ECCCCCD, 0x41F00000, -1);
    SCNF_WaitFrames_0000(20);
    Script_scenCreateTask(script_2AC7, 3, SHEN_NAME('BIKY'), SHEN_NAME('BIK2'), -1);
    Script_scenCreateTask(script_3487, 3, SHEN_NAME('BIK2'), 10, 120);
    script_45B0(SHEN_NAME('RY20'), 0, 0, -1);
    script_45B0(SHEN_NAME('RY21'), 0, 0, -1);
    task_C8 = 10;
    SCNF_WaitFrames_0000(1);
  } else {
    script_410A(12, -1);
    script_3ED9(10, 30);
    script_3E5D(8420, SHEN_NAME('DMB2'), SHEN_NAME('DMB2'), -1);
    script_3E5D(8433, SHEN_NAME('DUM0'), SHEN_NAME('DMB2'), 60);
    script_370B(6, 0x3E800000, 0x3E99999A, 0x41F00000, -1);
    script_370B(5, 0x3E99999A, 0, 0x42A00000, -1);
    local_F4.x = 0.0f;
    (&local_F4)->y = -0.03f;
    (&local_F4)->z = -0.1f;
    local_E8.x = 0;
    (&local_E8)->y = 0;
    (&local_E8)->z = 0;
    Script_EVT_SetPosition(arg0, POS_XYZ, &local_F4);
    Script_EVT_SetRotation(arg0, 0x38000000, &local_E8, 0);
    script_10F5(SHEN_NAME('BIK2'), 0, 0x41C80000, 0x41700000, 55);
    script_10F5(SHEN_NAME('BIK2'), 0x41C80000, 0, 0x41C80000, 80);
    Script_scenCreateTask(script_0484, 2, 0x2004A9, 60);
    SCNF_WaitFrames_0000(30);
    Script_scenCreateTask(script_240B, 3, CHARA_BIKA, SHEN_NAME('BIK1'), -1);
    Script_scenCreateTask(script_3487, 3, SHEN_NAME('BIK1'), 10, 30);
    script_45B0(SHEN_NAME('RY10'), 0, 0, -1);
    script_45B0(SHEN_NAME('RY11'), 0, 0, -1);
    script_4820(task_74[2]);
  }
  SCNF_WaitFrames_0000(100);
  task_C8 = -1;
  SCNF_WaitFrames_0000(1);
  end:;
}

static void script_697A(int32_t arg0, int32_t arg1) {
  Vector3 local_F4;
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 80.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 80.0f);
  Script_charaToggleMotEnabled(arg0, 1);
  Script_charaToggleMotEnabled(task_74[1], 1);
  Script_charaToggleMotEnabled(task_74[2], 1);
  Script_charaToggleMotEnabled(task_74[3], 1);
  Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_IMMEDIATE);
  Script_motmSetBlendFrames(task_74[1], MOTM_BLEND_FRAMES_IMMEDIATE);
  Script_motmSetBlendFrames(task_74[2], MOTM_BLEND_FRAMES_IMMEDIATE);
  Script_motmSetBlendFrames(task_74[3], MOTM_BLEND_FRAMES_IMMEDIATE);
  Script_executeAicaCommand(0x1300A0, 4, 0);
  script_4726(task_74[1], 0x304B4942);
  script_4726(task_74[2], 0x314B4942);
  script_4726(*task_74, 0x324B4942);
  script_0EA8(SHEN_NAME('DMB0'), -1, -0x3D560000, 0x42D40000, 0x43870000, -1);
  script_0EA8(SHEN_NAME('DMB1'), -1, -0x3D4C0000, 0x42D80000, 0x43870000, -1);
  script_0EA8(SHEN_NAME('DMB2'), -1, -0x3D3E0000, 0x42D60000, 0x43870000, -1);
  script_410A(2, -1);
  script_3ED9(7, -1);
  script_44E1(arg0, 2, 0, 16, -1);
  script_3E5D(8419, SHEN_NAME('DUM1'), SHEN_NAME('DUM1'), -1);
  script_4602(task_74[1], 1, 20995, 0, 0x3F800000, 0, -1);
  script_4602(task_74[2], 2, 20995, 0, 0x3F800000, 0, -1);
  if (-1) {
    Script_charaDisplay(task_74[3], CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(task_74[3], CDM_Show);
      goto L006B39;
    }
  }
  L006B39:;
  Script_scenToggleVisibleFor(task_74[3], 0);
  script_45B0(arg0, 1, 0, -1);
  script_45B0(task_74[1], 1, 0, -1);
  script_45B0(task_74[2], 1, 0, -1);
  script_45B0(task_74[3], 0, 0, -1);
  script_45B0(SHEN_NAME('BIK0'), 1, 0, -1);
  script_45B0(SHEN_NAME('BIK1'), 1, 0, -1);
  script_45B0(SHEN_NAME('BIK2'), 1, 0, -1);
  script_45B0(SHEN_NAME('RY00'), 1, 0, -1);
  script_45B0(SHEN_NAME('RY01'), 1, 0, -1);
  script_45B0(SHEN_NAME('RY10'), 1, 0, -1);
  script_45B0(SHEN_NAME('RY11'), 1, 0, -1);
  script_45B0(SHEN_NAME('RY20'), 1, 0, -1);
  script_367C(4, 0.1f, &data_CEC4, 1);
  script_367C(5, 0.1f, &data_D118, 1);
  script_367C(6, 9.1f, &data_C900, 1);
  script_377F(4, 0.3f);
  script_377F(5, 0.22f);
  script_377F(6, 0.3f);
  SCNF_WaitFrames_0000(1);
  Script_EVT_GetPosition(CHARA_AKIR, -1, &local_F4, 0);
  (&local_F4)->z = (&local_F4)->z - 0.3f;
  Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &local_F4);
  Script_scenCreateTask(script_0484, 2, 0x1F04A9, -1);
  SCNF_WaitFrames_0000(10);
  return;
}

static void script_6CE0(int32_t arg0, int32_t arg1) {
  script_4820(task_74[1]);
  script_4820(task_74[2]);
  script_4820(*task_74);
  return;
}

static int32_t script_6D0A(int32_t arg0, int32_t arg1) {
  Script_charaToggleMotEnabled(arg0, 1);
  Script_charaToggleMotEnabled(task_74[1], 1);
  Script_charaToggleMotEnabled(task_74[2], 1);
  Script_charaToggleMotEnabled(task_74[3], 1);
  Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_10);
  Script_motmSetBlendFrames(task_74[1], MOTM_BLEND_FRAMES_10);
  Script_motmSetBlendFrames(task_74[2], MOTM_BLEND_FRAMES_10);
  Script_motmSetBlendFrames(task_74[3], MOTM_BLEND_FRAMES_10);
  script_45B0(arg0, 1, 0, -1);
  script_45B0(task_74[1], 1, 0, -1);
  script_45B0(task_74[2], 1, 0, -1);
  script_45B0(task_74[3], 1, 0, -1);
  script_410A(4, -1);
  script_09DB(arg1, 3, 1, 0);
  if (task_BC == 0) {
    script_410A(6, -1);
    script_3E5D(8427, SHEN_NAME('DUM0'), SHEN_NAME('DUM0'), -1);
    script_3E5D(8414, SHEN_NAME('DMB2'), SHEN_NAME('DMB2'), 101);
    script_370B(6, 0x3F000000, 0x3E4CCCCD, 0x428C0000, -1);
    script_370B(6, 0x3E4CCCCD, 0x3ECCCCCD, 0x41E00000, 100);
    script_10F5(arg0, 0, -0x3E380000, 0x41700000, 50);
    script_10F5(arg0, -0x3E380000, 0, 0x41A00000, 80);
    Script_scenCreateTask(script_0484, 2, 0x1304A9, -1);
    Script_scenCreateTask(script_0484, 2, 0x1204A9, -1);
    Script_scenCreateTask(script_0484, 2, 0x1A04A9, 110);
    SCNF_WaitFrames_0000(20);
  } else {
    script_410A(5, -1);
    script_3E5D(8422, SHEN_NAME('DUM0'), SHEN_NAME('DUM0'), -1);
    script_3E5D(8414, SHEN_NAME('DMB2'), SHEN_NAME('DMB2'), 95);
    script_3E9E(0x3D4CCCCD, 0x3CA3D70A, 0x3E4CCCCD, 52);
    script_370B(6, 0x3F000000, 0x3E99999A, 0x42700000, -1);
    script_370B(6, 0x3E99999A, 0x3ECCCCCD, 0x41E00000, 80);
    Script_scenCreateTask(script_0484, 2, 66729, -1);
    Script_scenCreateTask(script_0484, 2, 132265, 20);
    Script_scenCreateTask(script_0484, 2, 787625, 30);
    Script_scenCreateTask(script_0484, 2, 0x1A04A9, 100);
    Script_scenCreateTask(script_2E35, 2, 2, -1);
  }
  script_10F5(arg0, 0, 0x41200000, 0x420C0000, 150);
  script_10F5(arg0, 0x41200000, 0, 0x41C80000, 160);
  SCNF_WaitFrames_0000(160);
  script_410A(8, -1);
  script_09DB(arg1, 5, 1, 0);
  if (task_BC == 0) {
    script_410A(10, -1);
    script_3E5D(8432, SHEN_NAME('DMB2'), SHEN_NAME('DMB2'), 20);
    script_370B(6, 0x3ECCCCCD, 0x3E19999A, 0x42480000, -1);
    script_370B(6, 0x3E19999A, 0x3F000000, 0x41E00000, 85);
    script_10F5(arg0, 0, -0x3E380000, 0x41700000, 20);
    script_10F5(arg0, -0x3E380000, 0, 0x41C80000, 50);
    Script_scenCreateTask(script_0484, 2, 66729, -1);
    Script_scenCreateTask(script_0484, 2, 0x1204A9, -1);
    SCNF_WaitFrames_0000(15);
  } else {
    script_410A(9, -1);
    script_3E5D(8434, SHEN_NAME('DUM0'), SHEN_NAME('DMB2'), 10);
    script_3E5D(8432, SHEN_NAME('DMB2'), SHEN_NAME('DMB2'), 46);
    script_370B(6, 0x3ECCCCCD, 0x3E4CCCCD, 0x42200000, -1);
    script_370B(6, 0x3E4CCCCD, 0x3F000000, 0x41E00000, 70);
    script_10F5(arg0, 0, -0x3E600000, 0x41700000, -1);
    script_10F5(arg0, -0x3E600000, 0, 0x41C80000, 15);
    Script_scenCreateTask(script_0484, 2, 66729, -1);
    Script_scenCreateTask(script_0484, 2, 787625, -1);
    Script_scenCreateTask(script_2E35, 2, 2, -1);
  }
  SCNF_WaitFrames_0000(73);
  Script_scenCreateTask(script_0484, 2, 0x1304A9, -1);
  script_410A(11, -1);
  script_09DB(arg1, 6, 1, 0);
  if (task_BC == 0) {
    script_410A(13, -1);
    script_3E5D(8455, SHEN_NAME('DUM0'), SHEN_NAME('DMB2'), -1);
    script_367C(6, 0.3f, &data_CA88, 1);
    script_37C3(6, 0x3E99999A, -1);
    script_370B(4, 0x3EAE147B, 0x3EBD70A4, 0x42700000, -1);
    script_370B(4, 0x3EBD70A4, 0x3EAE147B, 0x42700000, 60);
    script_10F5(arg0, 0, 0x41700000, 0x41700000, 50);
    script_10F5(arg0, 0x41700000, 0, 0x41700000, 65);
    script_0EA8(SHEN_NAME('DMB2'), 0x304D5544, 0x42400000, 0x42FE0000, 0x43004CCD, -1);
    Script_scenCreateTask(script_0484, 2, 66729, -1);
    Script_scenCreateTask(script_2D23, 3, SHEN_NAME('KBD0'), SHEN_NAME('KBK0'), 35);
  } else {
    script_410A(12, -1);
    script_3E5D(8447, SHEN_NAME('DUM0'), SHEN_NAME('DMB2'), -1);
    script_367C(6, 0.3f, &data_CA34, 1);
    script_37C3(6, 0x3E99999A, -1);
    script_107B(SHEN_NAME('DMB2'), -0x3F200000, -1);
    script_10F5(arg0, 0, 0x41700000, 0x41700000, 50);
    script_10F5(arg0, 0x41700000, 0, 0x41700000, 65);
    script_0EA8(SHEN_NAME('DMB2'), 0x304D5544, 0x42400000, 0x42FE0000, 0x43004CCD, -1);
    Script_scenCreateTask(script_0484, 2, 0x1904A9, -1);
  }
  script_10F5(SHEN_NAME('BIK0'), 0, -0x3E380000, 0x41700000, 75);
  script_10F5(SHEN_NAME('BIK0'), -0x3E380000, 0, 0x41C80000, 90);
  SCNF_WaitFrames_0000(50);
  Script_scenCreateTask(script_0484, 2, 0x1304A9, -1);
  script_410A(11, -1);
  script_09DB(arg1, 8, 1, 0);
  if (task_BC == 0) {
    script_410A(13, -1);
    script_3E5D(8435, SHEN_NAME('DMB2'), SHEN_NAME('DMB2'), -1);
    script_3E5D(8445, SHEN_NAME('DUM0'), SHEN_NAME('DMB2'), 74);
    script_367C(6, 0.3f, &data_CB80, 1);
    script_37C3(6, 0x3E99999A, -1);
    script_370B(4, 0x3EAE147B, 0x3EBD70A4, 0x42700000, -1);
    script_370B(4, 0x3EBD70A4, 0x3EAE147B, 0x42700000, 60);
    script_0EA8(SHEN_NAME('DMB2'), 0x304D5544, 0x41F66666, 0x430FE666, 0x43062FDF, -1);
    Script_scenCreateTask(script_0484, 2, 0x1304A9, -1);
    Script_scenCreateTask(script_2C11, 3, SHEN_NAME('KBD1'), SHEN_NAME('KBK1'), 32);
  } else {
    script_410A(12, -1);
    script_3E5D(8445, SHEN_NAME('DUM0'), SHEN_NAME('DMB2'), -1);
    script_367C(6, 0.3f, &data_CADC, 1);
    script_37C3(6, 0x3E99999A, -1);
    script_0EA8(SHEN_NAME('DMB2'), 0x304D5544, 0x41F66666, 0x430FE666, 0x43062FDF, -1);
    Script_scenCreateTask(script_0484, 2, 0x1304A9, -1);
  }
  script_10F5(arg0, 0, -0x3E380000, 0x41A00000, 55);
  script_10F5(arg0, -0x3E380000, 0, 0x41A00000, 75);
  script_10F5(arg0, 0, -0x3E380000, 0x41A00000, 140);
  script_10F5(arg0, -0x3E380000, 0, 0x42700000, 165);
  script_3E5D(8446, SHEN_NAME('DUM0'), SHEN_NAME('DMB2'), 140);
  script_3E5D(8454, SHEN_NAME('DUM0'), SHEN_NAME('DUM0'), 172);
  script_4602(arg0, arg1, 21000, 0x42480000, 0x40000000, 1, 80);
  script_4602(arg0, arg1, 21000, 0x433E0000, 0x40000000, 1, 95);
  Script_scenCreateTask(script_0484, 2, 787625, 75);
  Script_scenCreateTask(script_0484, 2, 787625, 155);
  Script_scenCreateTask(script_0484, 2, 0x1304A9, 70);
  Script_scenCreateTask(script_0484, 2, 66729, 120);
  Script_scenCreateTask(script_0484, 2, 0x1304A9, 165);
  Script_scenCreateTask(script_2E35, 2, 2, 75);
  Script_scenCreateTask(script_2E35, 2, 2, 155);
  SCNF_WaitFrames_0000(192);
  Script_scenCreateTask(script_0484, 2, 0x1304A9, -1);
  script_410A(11, -1);
  script_09DB(arg1, 7, 1, 0);
  if (task_C0 == 4) {
    script_410A(13, -1);
    script_3ED9(12, 180);
    script_3E5D(8471, SHEN_NAME('DUM0'), SHEN_NAME('DUM0'), -1);
    script_3E5D(8470, SHEN_NAME('DUM0'), SHEN_NAME('DUM0'), 45);
    script_3E5D(8474, SHEN_NAME('DMB2'), SHEN_NAME('DMB2'), 180);
    script_4602(arg0, arg1, 20994, 0, 0x3E99999A, 0, 180);
    script_10F5(arg0, 0, -0x3E380000, 0x41700000, 30);
    script_10F5(arg0, -0x3E380000, 0, 0x41C80000, 50);
    script_367C(6, 0.3f, &data_CC90, 1);
    script_37C3(6, 0x3E99999A, -1);
    script_370B(6, 0x3E99999A, 0, 0x42700000, 120);
    script_0EA8(SHEN_NAME('DMB2'), 0x304D5544, -0x40800000, 0x43058000, 0x439CE666, -1);
    Script_scenCreateTask(script_0484, 2, 66729, 45);
    Script_scenCreateTask(script_0484, 2, 787625, 50);
    Script_scenCreateTask(script_0484, 2, 0x1604A9, 60);
    Script_scenCreateTask(script_2E35, 2, 2, 50);
    SCNF_WaitFrames_0000(190);
    task_C8 = 10;
    SCNF_WaitFrames_0000(1);
  } else if (task_C0 == 64) {
    script_410A(12, -1);
    script_3E5D(8438, SHEN_NAME('DUM0'), SHEN_NAME('DMB2'), -1);
    script_3E5D(8439, SHEN_NAME('DUM0'), SHEN_NAME('DUM0'), 40);
    script_367C(6, 0.3f, &data_CC24, 1);
    script_37C3(6, 0x3E99999A, -1);
    script_107B(SHEN_NAME('DMB2'), 0x41B00000, -1);
    script_10F5(arg0, 0, 0x41C80000, 0x41700000, 45);
    script_10F5(arg0, 0x41C80000, 0, 0x41C80000, 60);
    script_0EA8(SHEN_NAME('DMB2'), 0x304D5544, -0x40800000, 0x43058000, 0x439CE666, -1);
    Script_scenCreateTask(script_0484, 2, 66729, 45);
  } else {
    script_410A(12, -1);
    script_3ED9(12, 130);
    script_3E5D(8436, SHEN_NAME('DMB2'), SHEN_NAME('DMB2'), -1);
    script_3E5D(8474, SHEN_NAME('DMB2'), SHEN_NAME('DMB2'), 130);
    script_4602(arg0, arg1, 20994, 0, 0x3E99999A, 0, 75);
    script_367C(6, 0.3f, &data_CC64, 1);
    script_37C3(6, 0x3E99999A, -1);
    script_370B(6, 0x3E99999A, 0, 0x41F00000, 50);
    script_0EA8(SHEN_NAME('DMB2'), 0x304D5544, -0x40800000, 0x43058000, 0x439CE666, -1);
    Script_scenCreateTask(script_0484, 2, 132265, 30);
    Script_scenCreateTask(script_0484, 2, 0x1204A9, 40);
    Script_scenCreateTask(script_0484, 2, 66729, -1);
    Script_scenCreateTask(script_0484, 2, 0x1604A9, 60);
    SCNF_WaitFrames_0000(150);
    task_C8 = 10;
    SCNF_WaitFrames_0000(1);
  }
  SCNF_WaitFrames_0000(50);
  Script_scenCreateTask(script_0484, 2, 0x1304A9, -1);
  script_410A(11, -1);
  script_09DB(arg1, 7, 1, 0);
  if (task_C0 == 64) {
    script_410A(13, -1);
    script_3ED9(12, 130);
    script_3E5D(8436, SHEN_NAME('DMB2'), SHEN_NAME('DMB2'), -1);
    script_3E5D(8474, SHEN_NAME('DMB2'), SHEN_NAME('DMB2'), 130);
    script_4602(arg0, arg1, 20994, 0, 0x3E99999A, 0, 115);
    script_367C(6, 0.3f, &data_CCF8, 1);
    script_37C3(6, 0x3E99999A, -1);
    script_370B(6, 0x3E99999A, 0, 0x42700000, 60);
    script_10F5(arg0, 0, 0x41C80000, 0x41700000, 40);
    script_10F5(arg0, 0x41C80000, 0, 0x41C80000, 65);
    script_0EA8(SHEN_NAME('DMB2'), 0x304D5544, 0x3F8CCCCD, 0x42E30000, 0x422E28F6, -1);
    Script_scenCreateTask(script_0484, 2, 132265, 80);
    Script_scenCreateTask(script_0484, 2, 66729, -1);
    Script_scenCreateTask(script_0484, 2, 0x1604A9, 60);
    SCNF_WaitFrames_0000(160);
    task_C8 = 10;
    SCNF_WaitFrames_0000(1);
  } else if (task_C0 == 4) {
    script_410A(12, -1);
    script_3E5D(8440, SHEN_NAME('DUM0'), SHEN_NAME('DMB2'), -1);
    script_4602(arg0, arg1, 21000, 0x42480000, 0x40000000, 1, 32);
    script_4602(arg0, arg1, 21000, 0x432A0000, 0x40000000, 1, 52);
    script_367C(6, 0.3f, &data_CD78, 1);
    script_37C3(6, 0x3E99999A, -1);
    script_0EA8(SHEN_NAME('DMB2'), 0x304D5544, 0x3F8CCCCD, 0x42E30000, 0x422E28F6, -1);
    script_10F5(arg0, 0, -0x3E600000, 0x41C80000, 10);
    script_10F5(arg0, -0x3E600000, 0, 0x41C80000, 30);
    Script_scenCreateTask(script_0484, 2, 66729, -1);
    Script_scenCreateTask(script_0484, 2, 787625, 30);
    Script_scenCreateTask(script_2E35, 2, 2, 30);
  } else {
    script_410A(12, -1);
    script_3ED9(12, 100);
    script_3E5D(8436, SHEN_NAME('DMB2'), SHEN_NAME('DMB2'), -1);
    script_3E5D(8474, SHEN_NAME('DMB2'), SHEN_NAME('DMB2'), 100);
    script_4602(arg0, arg1, 20994, 0, 0x3E99999A, 0, 70);
    script_367C(6, 0.3f, &data_CD4C, 1);
    script_37C3(6, 0x3E99999A, -1);
    script_370B(6, 0x3E99999A, 0, 0x41F00000, 40);
    script_0EA8(SHEN_NAME('DMB2'), 0x304D5544, 0x3F8CCCCD, 0x42E30000, 0x422E28F6, -1);
    Script_scenCreateTask(script_0484, 2, 66729, -1);
    Script_scenCreateTask(script_0484, 2, 132265, -1);
    Script_scenCreateTask(script_0484, 2, 0x1204A9, 10);
    Script_scenCreateTask(script_0484, 2, 0x1604A9, 60);
    SCNF_WaitFrames_0000(120);
    task_C8 = 10;
    SCNF_WaitFrames_0000(1);
  }
  SCNF_WaitFrames_0000(110);
  Script_SetLampState(0, SHEN_NAME('BIK2'), 0);
  task_C8 = -1;
  SCNF_WaitFrames_0000(1);
  end:;
}

static void script_7DC0(int32_t arg0, int32_t arg1) {
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 60.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 60.0f);
  Script_charaToggleMotEnabled(arg0, 1);
  Script_charaToggleMotEnabled(task_74[1], 1);
  Script_charaToggleMotEnabled(task_74[2], 1);
  Script_charaToggleMotEnabled(task_74[3], 1);
  Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_IMMEDIATE);
  Script_motmSetBlendFrames(task_74[1], MOTM_BLEND_FRAMES_IMMEDIATE);
  Script_motmSetBlendFrames(task_74[2], MOTM_BLEND_FRAMES_IMMEDIATE);
  Script_motmSetBlendFrames(task_74[3], MOTM_BLEND_FRAMES_IMMEDIATE);
  script_4726(task_74[1], 0x304B4942);
  script_4780(arg0, SHEN_NAME('DMB2'));
  script_46CF(SHEN_NAME('BIK2'), arg0);
  script_0EA8(SHEN_NAME('DMB0'), -1, 0x41233333, 0x42A66666, 0x432A8000, -1);
  script_0EA8(SHEN_NAME('DMB2'), -1, -0x3E680000, 0x42D00000, 0x4399C000, -1);
  script_410A(2, -1);
  script_3ED9(11, -1);
  script_3E5D(8418, SHEN_NAME('DMB2'), SHEN_NAME('DMB2'), -1);
  script_4602(arg0, arg1, 21000, 0x437A0000, 0x40000000, 0, -1);
  script_4602(task_74[1], arg1, 20995, 0, 0x3F800000, 0, -1);
  if (-1) {
    Script_charaDisplay(task_74[2], CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(task_74[2], CDM_Show);
      goto L007F39;
    }
  }
  L007F39:;
  Script_scenToggleVisibleFor(task_74[2], 0);
  script_45B0(arg0, 1, 0, -1);
  script_45B0(task_74[1], 1, 0, -1);
  script_45B0(task_74[2], 0, 0, -1);
  script_45B0(task_74[3], 0, 0, -1);
  script_45B0(SHEN_NAME('RY20'), 1, 0, -1);
  script_367C(4, 0.1f, "33#A", 1);
  script_367C(6, 0.1f, &data_C990, 1);
  script_37C3(4, 0x3EAE147B, -1);
  script_37C3(6, 0x3F000000, -1);
  Script_scenCreateTask(script_0484, 2, 0x1A04A9, -1);
  SCNF_WaitFrames_0000(20);
  return;
}

__attribute__((scn_string_bindings(0, data_CDCC, 1, data_D548)))
static void script_8009(int32_t arg0, int32_t arg1) {
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 40.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 40.0f);
  Script_SetCurrentScrollEnabled(0);
  Script_executeAicaCommand(540584, 0, 0);
  Script_executeAicaCommand(1184, 2, 110);
  Script_ReadModelFormat7(*task_74, 0, data_D554, 10);
  Script_ReadModelFormat7(*task_74, 1, data_D57C, 10);
  script_410A(2, -1);
  script_3E5D(8441, SHEN_NAME('DMB0'), SHEN_NAME('DMB0'), 1);
  script_3E5D(8443, SHEN_NAME('DMB2'), SHEN_NAME('DMB2'), 170);
  script_3E5D(8469, SHEN_NAME('DUM0'), SHEN_NAME('DUM0'), 277);
  script_367C(4, 0.4f, &data_D020, 1);
  script_367C(6, 0.5f, "ff&@", 1);
  script_37C3(4, 0x3ECCCCCD, -1);
  script_37C3(6, 0x3F000000, -1);
  script_370B(4, 0x3ECCCCCD, 0x3E99999A, 0x42700000, -1);
  script_370B(4, 0x3E99999A, 0x3F000000, 0x41F00000, 110);
  script_370B(4, 0x3F000000, 0x3E4CCCCD, 0x41F00000, 190);
  script_370B(4, 0x3E4CCCCD, 0, 0x428C0000, 245);
  script_370B(6, 0x3F000000, 0x3E4CCCCD, 0x42700000, 110);
  script_370B(6, 0x3E4CCCCD, 0x3EE4DD2F, 0x41F00000, 170);
  script_370B(6, 0x3EE4DD2F, 0x3E4CCCCD, 0x41F00000, 220);
  script_370B(6, 0x3E4CCCCD, 0, 0x42700000, 290);
  script_0E75(SHEN_NAME('DMB0'), -1, (const Vector3 *)&data_D53C);
  script_0E75(SHEN_NAME('DMB2'), -1, (const Vector3 *)"ff&@");
  script_10F5(SHEN_NAME('BIK0'), 0, -0x3EE00000, 0x41F00000, 95);
  script_10F5(SHEN_NAME('BIK0'), -0x3EE00000, 0, 0x42200000, 125);
  script_10F5(SHEN_NAME('BIK0'), 0, -0x3E380000, 0x41F00000, 190);
  script_10F5(SHEN_NAME('BIK0'), -0x3E380000, 0, 0x42200000, 220);
  script_10F5(CHARA_AKIR, 0, -0x3E900000, 0x41A00000, 115);
  script_10F5(CHARA_AKIR, -0x3E900000, 0, 0x42200000, 135);
  script_10F5(CHARA_AKIR, 0, -0x3E380000, 0x41A00000, 230);
  script_10F5(CHARA_AKIR, -0x3E380000, 0, 0x41F00000, 260);
  script_45B0(SHEN_NAME('RY00'), 0, 0, 277);
  script_45B0(SHEN_NAME('RY01'), 0, 0, 277);
  script_45B0(SHEN_NAME('RY20'), 0, 0, 277);
  script_45B0(SHEN_NAME('RY21'), 0, 0, 277);
  script_45B0(SHEN_NAME('RYY0'), 1, 0, 277);
  script_45B0(SHEN_NAME('RYY1'), 1, 0, 277);
  Script_scenCreateTask(script_0484, 2, 0x1F04A9, -1);
  Script_scenCreateTask(script_0484, 2, 66729, 60);
  Script_scenCreateTask(script_0484, 2, 0x1304A9, 100);
  Script_scenCreateTask(script_0484, 2, 66729, 180);
  Script_scenCreateTask(script_0484, 2, 66729, 240);
  Script_scenCreateTask(script_0484, 2, 459945, 300);
  Script_scenCreateTask(script_0484, 2, 918697, 300);
  Script_scenCreateTask(script_0484, 2, 525481, 585);
  Script_scenCreateTask(script_0484, 2, 984233, 600);
  Script_SetLampState(0, SHEN_NAME('BIK2'), 1);
  SCNF_WaitFrames_0000(245);
  Script_scenCreateTask(script_0484, 2, 132265, -1);
  SCNF_WaitFrames_0000(32);
  Script_SetLampState(0, SHEN_NAME('BIK2'), 0);
  Script_scenCreateTask(script_2855, 4, *task_74, SHEN_NAME('BIKY'), SHEN_NAME('BIK2'), -1);
  Script_scenCreateTask(script_0484, 2, 0x1204A9, -1);
  script_4817(SHEN_NAME('BIK2'));
  script_482E(arg0);
  script_4780(SHEN_NAME('BIK2'), SHEN_NAME('DMB2'));
  SCNF_WaitFrames_0000(105);
  script_37FC(4);
  script_37FC(6);
  Script_filmCheckOrSetTransitionUtil(60, 0, 0, 0, 0, 255, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  script_4820(task_74[1]);
  script_4602(CHARA_AKIR, 0, 0xFFFF, 0, 0x3F800000, 0, -1);
  script_4602(CHARA_CHRL, 0, 0xFFFF, 0, 0x3F800000, 0, -1);
  script_45B0(SHEN_NAME('BIKY'), 0, 0, -1);
  script_45B0(SHEN_NAME('RYY0'), 0, 0, -1);
  script_45B0(SHEN_NAME('RYY1'), 0, 0, -1);
  Script_SetCurrentScrollEnabled(1);
  return;
}

static void script_853E(int32_t arg0, int32_t arg1) {
  Vector3 local_F4;
  int32_t unused_F0;
  GameDateTime local_E8;
  int32_t local_BC[11];
  SCNF_WaitFrames_0000(5);
  Script_filmCheckOrSetTransitionUtil(60, 255, 0, 0, 0, 0, 0, 0, 0);
  Script_uiUtils(0, 20, 0);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 50.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 50.0f);
  Script_SetCurrentScrollEnabled(0);
  Script_lightSetSlotEnabled(2, 1);
  script_4726(task_74[1], 0x30424D44);
  script_4726(*task_74, 0x32424D44);
  Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_IMMEDIATE);
  Script_motmSetBlendFrames(task_74[1], MOTM_BLEND_FRAMES_IMMEDIATE);
  Script_motmSetBlendFrames(CHARA_HTNK, MOTM_BLEND_FRAMES_IMMEDIATE);
  Script_motmSetBlendFrames(CHARA_TDSI, MOTM_BLEND_FRAMES_IMMEDIATE);
  Script_motmSetBlendFrames(SHEN_NAME('BTA_'), MOTM_BLEND_FRAMES_IMMEDIATE);
  Script_motmSetBlendFrames(SHEN_NAME('BTH_'), MOTM_BLEND_FRAMES_IMMEDIATE);
  Script_motmSetBlendFrames(SHEN_NAME('BTK_'), MOTM_BLEND_FRAMES_IMMEDIATE);
  script_45B0(SHEN_NAME('BIK0'), 1, 0, -1);
  script_45B0(SHEN_NAME('BIK2'), 0, 0, -1);
  script_45B0(SHEN_NAME('BK2A'), 1, 0, -1);
  script_45B0(SHEN_NAME('RY00'), 1, 0, -1);
  script_45B0(SHEN_NAME('RY01'), 1, 0, -1);
  script_45B0(SHEN_NAME('RY20'), 1, 0, -1);
  script_45B0(SHEN_NAME('RY21'), 1, 0, -1);
  script_45B0(CHARA_AKIR, 1, 0, -1);
  script_45B0(CHARA_CHRL, 1, 0, -1);
  script_45B0(CHARA_HTNK, 0, 0, -1);
  script_45B0(CHARA_TDSI, 0, 0, -1);
  script_45B0(SHEN_NAME('BTA_'), 0, 0, -1);
  script_45B0(SHEN_NAME('BTH_'), 0, 0, -1);
  script_45B0(SHEN_NAME('BTK_'), 0, 0, -1);
  script_45B0(CHARA_HTNK, 1, 0, 270);
  script_45B0(CHARA_TDSI, 1, 0, 270);
  script_45B0(SHEN_NAME('BTA_'), 1, 0, 270);
  script_45B0(SHEN_NAME('BTH_'), 1, 0, 270);
  script_45B0(SHEN_NAME('BTK_'), 1, 0, 270);
  script_410A(2, -1);
  script_3E5D(8458, SHEN_NAME('DMB0'), SHEN_NAME('DMB0'), -1);
  script_3E5D(8457, SHEN_NAME('DMB0'), SHEN_NAME('DMB0'), 200);
  script_3E5D(8459, SHEN_NAME('DUM1'), SHEN_NAME('DUM1'), 270);
  script_3E5D(8460, SHEN_NAME('DUM1'), SHEN_NAME('DUM1'), 310);
  script_3E5D(8461, SHEN_NAME('DUM1'), SHEN_NAME('DUM1'), 350);
  script_3E5D(8462, SHEN_NAME('DMB0'), SHEN_NAME('DMB0'), 400);
  script_3E5D(8465, SHEN_NAME('DMB0'), SHEN_NAME('DMB0'), 640);
  script_3ED9(13, -1);
  script_3ED9(14, 140);
  script_3ED9(15, 200);
  script_3ED9(16, 270);
  script_3ED9(17, 400);
  script_3ED9(18, 540);
  script_3ED9(19, 650);
  script_0DE6(SHEN_NAME('DMB0'), 0x304D5544, 94.3f, 29.7f, 300.0f);
  script_0DE6(SHEN_NAME('DMB2'), 0x304D5544, 93.5f, 22.0f, 240.0f);
  script_0DE6(CHARA_HTNK, 0x304D5544, 96.5f, 23.4f, 120.0f);
  script_0DE6(CHARA_TDSI, 0x304D5544, 97.5f, 24.0f, 90.0f);
  script_0DE6(SHEN_NAME('BTA_'), 0x304D5544, 90.7f, 24.1f, 255.0f);
  script_0DE6(SHEN_NAME('BTH_'), 0x304D5544, 91.1f, 26.4f, 290.0f);
  script_0DE6(SHEN_NAME('BTK_'), 0x304D5544, 89.3f, 23.4f, 280.0f);
  script_0EA8(CHARA_AKIR, 0x304D5544, 0x42B96666, 0x41B4CCCD, 0x43480000, 270);
  script_107B(CHARA_AKIR, -0x3CF90000, 200);
  script_4602(CHARA_AKIR, 0, 20995, 0, 0x3F800000, 0, -1);
  script_4602(task_74[1], 1, 20995, 0, 0x3F800000, 0, -1);
  script_4602(CHARA_AKIR, 0, 40980, 0x42DC0000, 0x3FB33333, 0, 150);
  script_4602(CHARA_AKIR, 0, 40980, 0x43C80000, 0x3FB33333, 0, 200);
  script_4602(CHARA_AKIR, 0, 102, 0, 0x3F4CCCCD, 0, 270);
  script_4602(CHARA_AKIR, 0, 1192, 0, 0x3F666666, 0, 310);
  script_4602(CHARA_AKIR, 0, 787, 0, 0x3F800000, 1, 640);
  script_4602(CHARA_HTNK, 0, 20481, 0, 0x3F800000, 0, 280);
  script_4602(CHARA_TDSI, 0, 20495, 0x42780000, 0x3F333333, 1, 280);
  script_4602(SHEN_NAME('BTA_'), 0, 20491, 0x428A0000, 0x3F666666, 1, 280);
  script_4602(SHEN_NAME('BTH_'), 0, 20491, 0x42780000, 0x3F4CCCCD, 0, 280);
  script_4602(SHEN_NAME('BTK_'), 0, 20495, 0x42740000, 0x3F000000, 0, 280);
  Script_scenCreateTask(script_0484, 2, 0x2803AB, 295);
  Script_scenCreateTask(script_0484, 2, 0x2803AB, 310);
  Script_scenCreateTask(script_0484, 2, 0x4303AB, 325);
  Script_scenCreateTask(script_0484, 2, 0x4303AB, 355);
  SCNF_WaitFrames_0000(20);
  Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_20);
  script_4820(task_74[1]);
  script_4820(*task_74);
  Script_MoveObjectToObject(CHARA_AKIR, -1);
  Script_MoveObjectToObject(CHARA_CHRL, -1);
  Script_MoveObjectToObject(SHEN_NAME('BIK0'), -1);
  Script_MoveObjectToObject(SHEN_NAME('BIK1'), -1);
  Script_MoveObjectToObject(SHEN_NAME('BIK2'), -1);
  Script_EVT_GetPosition(*task_74, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  Script_SetLookPointDetails(task_74[1], 98312, &local_F4, 0);
  SCNF_WaitFrames_0000(720);
  Script_SetCharaLookAtPosition(task_74[1], -98312, 0, 0);
  Script_filmCheckOrSetTransitionUtil(15, 0, 0, 0, 0, 255, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_LoadSND(0, 2, "bgm013.snd", 0);
  Script_SetCPAMFlags(CHARA_AKIR, 1);
  Script_SetCPAMFlags(CHARA_CHRL, 1);
  Script_SetCPAMFlags(CHARA_HTNK, 1);
  Script_SetCPAMFlags(CHARA_TDSI, 1);
  Script_SetCPAMFlags(SHEN_NAME('BTA_'), 1);
  Script_SetCPAMFlags(SHEN_NAME('BTH_'), 1);
  Script_SetCPAMFlags(SHEN_NAME('BTK_'), 1);
  Script_SetCameraSettings(5, 6, 1, 0);
  Script_convSetCamInactiveFlag(1);
  Script_SetGameStateTo(AUTH_None);
  Script_scenToggleVisibleFor(task_74[1], 0);
  Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
  Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
  Script_FaceEx(CHARA_AKIR, 0, 0, 0);
  Script_FaceEx(CHARA_CHRL, 0, 0, 0);
  Script_faceUpdateFor(CHARA_AKIR, 0, 0);
  Script_faceUpdateFor(CHARA_CHRL, 0, 0);
  Script_lightSetSlotEnabled(2, 0);
  Script_lightSetSlotEnabled(3, 0);
  Script_lightSetSlotEnabled(34, 0);
  Script_lightSetSlotEnabled(35, 0);
  Script_lightSetSlotEnabled(36, 0);
  Script_lightSetSlotEnabled(37, 0);
  Script_lightSetSlotEnabled(38, 0);
  Script_lightSetSlotEnabled(85, 0);
  Script_lightSetSlotEnabled(86, 0);
  Script_lightSetSlotEnabled(87, 0);
  Script_lightSetSlotEnabled(88, 0);
  Script_lightSetSlotEnabled(94, 0);
  Script_lightSetSlotEnabled(96, 0);
  Script_lightSetSlotEnabled(97, 0);
  Script_lightSetSlotEnabled(98, 0);
  Script_lightSetSlotEnabled(99, 0);
  Script_lightSetSlotEnabled(110, 0);
  Script_lightSetSlotEnabled(111, 0);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 35.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 45.0f);
  Script_filmCheckOrSetTransitionUtil(15, 255, 0, 0, 0, 0, 0, 0, 0);
  Script_ReturnTimeDate(&local_E8);
  for (;;) {
    Script_SetNewDateTime(&local_E8);
    Script_WeatherTaskUtil(FreeTaskData);
    Script_WeatherTaskUtil(InitTaskData);
    Script_executeAicaCommand(9640, 0, 0);
    Script_executeAicaCommand(1184, 2, 127);
    script_0DE6(CHARA_HTNK, 0x304D5544, 107.5f, 26.4f, 120.0f);
    script_0DE6(CHARA_TDSI, 0x304D5544, 108.5f, 27.0f, 90.0f);
    script_0DE6(SHEN_NAME('BTA_'), 0x304D5544, 101.7f, 27.1f, 255.0f);
    script_0DE6(SHEN_NAME('BTH_'), 0x304D5544, 102.1f, 29.4f, 290.0f);
    script_0DE6(SHEN_NAME('BTK_'), 0x304D5544, 100.3f, 26.4f, 280.0f);
    script_0DE6(CHARA_AKIR, 0x304D5544, 103.7f, 25.6f, 200.0f);
    script_4602(CHARA_HTNK, 0, 787, 0, 0x3F800000, 0, -1);
    script_4602(CHARA_TDSI, 0, 787, 0, 0x3F800000, 0, -1);
    script_4602(SHEN_NAME('BTA_'), 0, 787, 0, 0x3F800000, 0, -1);
    script_4602(SHEN_NAME('BTH_'), 0, 787, 0, 0x3F800000, 0, -1);
    script_4602(SHEN_NAME('BTK_'), 0, 787, 0, 0x3F800000, 0, -1);
    Script_vfSetup(0, 20, 20, 0, 0);
    Script_vfStageAssignRingBoundary(&data_D5A4, 14.0f);
    local_BC[0] = 0x52494B41;
    local_BC[1] = 0;
    local_BC[2] = 0;
    local_BC[3] = 0;
    local_BC[4] = 200;
    local_BC[5] = 0;
    local_BC[6] = 0;
    local_BC[7] = 0;
    local_BC[8] = 0;
    local_BC[9] = 0;
    local_BC[10] = 0;
    Script_vfUtils((const void *)local_BC);
    local_BC[0] = 0x4B4E5448;
    local_BC[1] = 0;
    local_BC[2] = 1;
    local_BC[3] = 1;
    local_BC[4] = 100;
    local_BC[5] = 14;
    local_BC[6] = 0;
    local_BC[7] = 0;
    local_BC[8] = 0;
    local_BC[9] = 0;
    local_BC[10] = 0;
    Script_vfUtils((const void *)local_BC);
    Script_vfConfigureFormationGroup(CHARA_HTNK, VF_FORMATION_APPLY_FLAGS, 0x200000, 0x800);
    Script_vfConfigureFormationGroup(CHARA_HTNK, VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
    Script_vfConfigureFormationGroup(CHARA_HTNK, VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
    local_BC[0] = 0x49534454;
    local_BC[1] = 0;
    local_BC[2] = 1;
    local_BC[3] = 1;
    local_BC[4] = 100;
    local_BC[5] = 14;
    local_BC[6] = 0;
    local_BC[7] = 0;
    local_BC[8] = 0;
    local_BC[9] = 0;
    local_BC[10] = 0;
    Script_vfUtils((const void *)local_BC);
    Script_vfConfigureFormationGroup(CHARA_TDSI, VF_FORMATION_APPLY_FLAGS, 0x200000, 0x800);
    Script_vfConfigureFormationGroup(CHARA_TDSI, VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
    Script_vfConfigureFormationGroup(CHARA_TDSI, VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
    local_BC[0] = 0x5F415442;
    local_BC[1] = 0;
    local_BC[2] = 1;
    local_BC[3] = 1;
    local_BC[4] = 60;
    local_BC[5] = 37;
    local_BC[6] = 0;
    local_BC[7] = 0;
    local_BC[8] = 0;
    local_BC[9] = 0;
    local_BC[10] = 0;
    Script_vfUtils((const void *)local_BC);
    Script_vfConfigureFormationGroup(SHEN_NAME('BTA_'), VF_FORMATION_APPLY_FLAGS, 0x200000, 0x800);
    Script_vfConfigureFormationGroup(SHEN_NAME('BTA_'), VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
    Script_vfConfigureFormationGroup(SHEN_NAME('BTA_'), VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
    local_BC[0] = 0x5F485442;
    local_BC[1] = 0;
    local_BC[2] = 1;
    local_BC[3] = 1;
    local_BC[4] = 60;
    local_BC[5] = 38;
    local_BC[6] = 0;
    local_BC[7] = 0;
    local_BC[8] = 0;
    local_BC[9] = 0;
    local_BC[10] = 0;
    Script_vfUtils((const void *)local_BC);
    Script_vfConfigureFormationGroup(SHEN_NAME('BTH_'), VF_FORMATION_APPLY_FLAGS, 0x200000, 0x800);
    Script_vfConfigureFormationGroup(SHEN_NAME('BTH_'), VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
    Script_vfConfigureFormationGroup(SHEN_NAME('BTH_'), VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
    local_BC[0] = 0x5F4B5442;
    local_BC[1] = 0;
    local_BC[2] = 1;
    local_BC[3] = 1;
    local_BC[4] = 60;
    local_BC[5] = 39;
    local_BC[6] = 0;
    local_BC[7] = 0;
    local_BC[8] = 0;
    local_BC[9] = 0;
    local_BC[10] = 0;
    Script_vfUtils((const void *)local_BC);
    Script_vfConfigureFormationGroup(SHEN_NAME('BTK_'), VF_FORMATION_APPLY_FLAGS, 0x200000, 0x800);
    Script_vfConfigureFormationGroup(SHEN_NAME('BTK_'), VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
    Script_vfConfigureFormationGroup(SHEN_NAME('BTK_'), VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
    SCN_Enqueue_VFGI(2, 99, 1.0f, 1, -1);
    for (;;) {
      if (SCN_GetIsFighting() == 2) {
        if (Script_GetFightingStats(0x52494B41, 0) == 3) {
          break;
        } else {
          break;
        }
      }
      SCNF_WaitFrames_0000(1);
    }
    if (Script_GetFightingStats(0x52494B41, 0) == 3) {
      break;
    } else {
      Script_executeAicaCommand(2720, 2, 30);
      Script_filmCheckOrSetTransitionUtil(60, 0, 0, 0, 0, 255, 0, 0, 0);
      while (Script_filmCheckOrSetTransitionUtil(-1)) {
        SCNF_WaitFrames_0000(1);
      }
      Script_sicnUtils(0, 1, 1);
      SCNF_WaitFrames_0000(90);
      Script_sicnUtils(2, 1, 0);
      Script_filmCheckOrSetTransitionUtil(60, 255, 0, 0, 0, 0, 0, 0, 0);
    }
  }
  local_BC[0] = -6;
  local_BC[1] = 0;
  local_BC[2] = 0;
  local_BC[3] = 0;
  local_BC[4] = 0;
  local_BC[5] = 10;
  local_BC[6] = 0;
  local_BC[7] = 0;
  local_BC[8] = 0;
  local_BC[9] = 0;
  local_BC[10] = 0;
  Script_vfUtils((const void *)local_BC);
  Script_scenToggleVisibleFor(task_74[1], 1);
  Script_facemotUtils(CHARA_AKIR, 0, 0);
  Script_facemotUtils(CHARA_CHRL, 0, 0);
  Script_FaceEx(CHARA_AKIR, 1, 2, 4);
  Script_FaceEx(CHARA_CHRL, 1, 2, 4);
  script_410A(2, -1);
  script_3ED9(22, -1);
  script_3ED9(23, 120);
  script_45B0(CHARA_HTNK, 0, 0, -1);
  script_45B0(CHARA_TDSI, 0, 0, -1);
  script_45B0(SHEN_NAME('BTA_'), 0, 0, -1);
  script_45B0(SHEN_NAME('BTH_'), 0, 0, -1);
  script_45B0(SHEN_NAME('BTK_'), 0, 0, -1);
  script_45B0(CHARA_HTNK, 1, 0, 120);
  script_45B0(CHARA_TDSI, 1, 0, 120);
  script_45B0(SHEN_NAME('BTA_'), 1, 0, 120);
  script_45B0(SHEN_NAME('BTH_'), 1, 0, 120);
  script_45B0(SHEN_NAME('BTK_'), 1, 0, 120);
  script_3E5D(8466, SHEN_NAME('DMB0'), SHEN_NAME('DMB0'), -1);
  script_3E5D(8467, SHEN_NAME('DUM1'), SHEN_NAME('DUM1'), 120);
  script_3E5D(8468, SHEN_NAME('DMB0'), SHEN_NAME('DMB0'), 170);
  script_0EA8(CHARA_AKIR, -1, 0x42C53333, 0x41D80000, 0x42F00000, -1);
  script_0EA8(task_74[1], -1, 0x42BC3333, 0x41E00000, 0x43520000, 120);
  script_4602(CHARA_AKIR, 0, 787, 0, 0x3F800000, 0, -1);
  script_4602(task_74[1], 0, 20495, 0x42740000, 0x3F800000, 1, 120);
  SCNF_WaitFrames_0000(250);
  Script_scenToggleVisibleFor(CHARA_HTNK, 0);
  Script_scenToggleVisibleFor(CHARA_TDSI, 0);
  Script_scenToggleVisibleFor(SHEN_NAME('BTA_'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('BTH_'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('BTK_'), 0);
  Script_SetCameraSettings(5, 6, 1, 0);
  Script_convSetCamInactiveFlag(1);
  Script_SetGameStateTo(AUTH_None);
  Script_FaceEx(CHARA_CHRL, 0, 0, 0);
  Script_FaceEx(CHARA_AKIR, 0, 0, 0);
  Script_faceUpdateFor(CHARA_AKIR, 0, 0);
  Script_faceUpdateFor(CHARA_CHRL, 0, 0);
  Script_ReturnTimeDate(&local_E8);
  for (;;) {
    Script_SetNewDateTime(&local_E8);
    Script_WeatherTaskUtil(FreeTaskData);
    Script_WeatherTaskUtil(InitTaskData);
    script_0DE6(CHARA_AKIR, 0x304D5544, 99.8f, 26.5f, 105.0f);
    script_0DE6(CHARA_CHRL, 0x304D5544, 98.4f, 27.0f, 285.0f);
    script_4602(task_74[1], 0, 787, 0, 0x3F800000, 0, -1);
    Script_vfSetup(0, 20, 20, 0, 0);
    Script_vfStageAssignRingBoundary(&data_D5A4, 14.0f);
    local_BC[0] = 0x52494B41;
    local_BC[1] = 0;
    local_BC[2] = 0;
    local_BC[3] = 0;
    local_BC[4] = 200;
    local_BC[5] = 0;
    local_BC[6] = 0;
    local_BC[7] = 0;
    local_BC[8] = 0;
    local_BC[9] = 0;
    local_BC[10] = 0;
    Script_vfUtils((const void *)local_BC);
    local_BC[0] = 0x4C524843;
    local_BC[1] = 0;
    local_BC[2] = 1;
    local_BC[3] = 1;
    local_BC[4] = 200;
    local_BC[5] = 46;
    local_BC[6] = 0;
    local_BC[7] = 0;
    local_BC[8] = 0;
    local_BC[9] = 0;
    local_BC[10] = 0;
    Script_vfUtils((const void *)local_BC);
    Script_vfConfigureFormationGroup(CHARA_CHRL, VF_FORMATION_APPLY_FLAGS, 0x200000, 0x800);
    SCN_Enqueue_VFGI(2, 99, 1.0f, 1, -1);
    for (;;) {
      if (SCN_GetIsFighting() == 2) {
        if (Script_GetFightingStats(0x52494B41, 0) == 3) {
          break;
        } else {
          break;
        }
      }
      SCNF_WaitFrames_0000(1);
    }
    Script_executeAicaCommand(2720, 2, 30);
    Script_filmCheckOrSetTransitionUtil(60, 0, 0, 0, 0, 255, 0, 0, 0);
    while (Script_filmCheckOrSetTransitionUtil(-1)) {
      SCNF_WaitFrames_0000(1);
    }
    if (Script_GetFightingStats(0x52494B41, 0) == 3) {
      break;
    } else {
      Script_sicnUtils(0, 1, 1);
      SCNF_WaitFrames_0000(90);
      Script_sicnUtils(2, 1, 0);
      Script_executeAicaCommand(9640, 0, 0);
      Script_executeAicaCommand(1184, 2, 127);
      Script_filmCheckOrSetTransitionUtil(60, 255, 0, 0, 0, 0, 0, 0, 0);
    }
  }
  local_BC[0] = -6;
  local_BC[1] = 0;
  local_BC[2] = 0;
  local_BC[3] = 0;
  local_BC[4] = 0;
  local_BC[5] = 11;
  local_BC[6] = 0;
  local_BC[7] = 0;
  local_BC[8] = 0;
  local_BC[9] = 0;
  local_BC[10] = 0;
  Script_vfUtils((const void *)local_BC);
  Script_SetCPAMFlags(CHARA_AKIR, 0);
  Script_SetCPAMFlags(CHARA_CHRL, 0);
  Script_SetCPAMFlags(CHARA_HTNK, 0);
  Script_SetCPAMFlags(CHARA_TDSI, 0);
  Script_SetCPAMFlags(SHEN_NAME('BTA_'), 0);
  Script_SetCPAMFlags(SHEN_NAME('BTH_'), 0);
  Script_SetCPAMFlags(SHEN_NAME('BTK_'), 0);
  script_4602(CHARA_AKIR, 0, 0xFFFF, 0, 0x3F800000, 0, -1);
  script_4602(CHARA_CHRL, 0, 0xFFFF, 0, 0x3F800000, 0, -1);
  script_4602(CHARA_HTNK, 0, 0xFFFF, 0, 0x3F800000, 0, -1);
  script_4602(CHARA_TDSI, 0, 0xFFFF, 0, 0x3F800000, 0, -1);
  script_4602(SHEN_NAME('BTA_'), 0, 0xFFFF, 0, 0x3F800000, 0, -1);
  script_4602(SHEN_NAME('BTH_'), 0, 0xFFFF, 0, 0x3F800000, 0, -1);
  script_4602(SHEN_NAME('BTK_'), 0, 0xFFFF, 0, 0x3F800000, 0, -1);
  Script_facemotUtils(CHARA_AKIR, 0, 0);
  Script_facemotUtils(CHARA_CHRL, 0, 0);
  Script_FaceEx(CHARA_AKIR, 1, 2, 4);
  Script_FaceEx(CHARA_CHRL, 1, 2, 4);
  SCNF_WaitFrames_0000(10);
  Script_SetCurrentScrollEnabled(1);
  Script_lightSetSlotEnabled(2, 0);
  Script_lightSetSlotEnabled(3, 0);
  Script_uiUtils(0, 0, 0);
  return;
}

static void script_9AD0(int32_t arg0, int32_t arg1) {
  SCNF_WaitFrames_0000(5);
  Script_PREP_Related(-1, 0);
  Script_SetupLightFromTable(16);
  Script_PREP_Related(-1, 1);
  Script_SetCharaFlags(CHARA_AKIR, 4);
  Script_SetCharaFlags(CHARA_CHRL, 4);
  if (0) {
    Script_charaDisplay(CHARA_HTNK, CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(CHARA_HTNK, CDM_Show);
      goto L009B33;
    }
  }
  L009B33:;
  Script_scenToggleVisibleFor(CHARA_HTNK, 1);
  if (0) {
    Script_charaDisplay(CHARA_TDSI, CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(CHARA_TDSI, CDM_Show);
      goto L009B67;
    }
  }
  L009B67:;
  Script_scenToggleVisibleFor(CHARA_TDSI, 1);
  Script_scenToggleVisibleFor(SHEN_NAME('BTA_'), 1);
  Script_scenToggleVisibleFor(SHEN_NAME('BTH_'), 1);
  Script_scenToggleVisibleFor(SHEN_NAME('BTK_'), 1);
  Script_SetShadowFlags(CHARA_AKIR, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_SetShadowFlags(CHARA_CHRL, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_SetShadowFlags(SHEN_NAME('BTA_'), SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_SetShadowFlags(SHEN_NAME('BTH_'), SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_SetShadowFlags(SHEN_NAME('BTK_'), SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_SetShadowFlags(CHARA_AKIR, SCRIPT_SHADOW_UPDATE_LIGHT_MASK, 3);
  Script_SetShadowFlags(CHARA_CHRL, SCRIPT_SHADOW_UPDATE_LIGHT_MASK, 3);
  Script_SetShadowFlags(SHEN_NAME('BTA_'), SCRIPT_SHADOW_UPDATE_LIGHT_MASK, 3);
  Script_SetShadowFlags(SHEN_NAME('BTH_'), SCRIPT_SHADOW_UPDATE_LIGHT_MASK, 3);
  Script_SetShadowFlags(SHEN_NAME('BTK_'), SCRIPT_SHADOW_UPDATE_LIGHT_MASK, 3);
  Script_SetShadowFlags(CHARA_AKIR, SCRIPT_SHADOW_UPDATE_LIGHT_MASK_0, 2);
  Script_SetShadowFlags(CHARA_CHRL, SCRIPT_SHADOW_UPDATE_LIGHT_MASK_0, 2);
  Script_SetShadowFlags(SHEN_NAME('BTA_'), SCRIPT_SHADOW_UPDATE_LIGHT_MASK_0, 2);
  Script_SetShadowFlags(SHEN_NAME('BTH_'), SCRIPT_SHADOW_UPDATE_LIGHT_MASK_0, 2);
  Script_SetShadowFlags(SHEN_NAME('BTK_'), SCRIPT_SHADOW_UPDATE_LIGHT_MASK_0, 2);
  Script_SetShadowFlags(CHARA_AKIR, SCRIPT_SHADOW_UPDATE_LIGHT_MASK_0, 1);
  Script_SetShadowFlags(CHARA_CHRL, SCRIPT_SHADOW_UPDATE_LIGHT_MASK_0, 1);
  Script_SetShadowFlags(SHEN_NAME('BTA_'), SCRIPT_SHADOW_UPDATE_LIGHT_MASK_0, 1);
  Script_SetShadowFlags(SHEN_NAME('BTH_'), SCRIPT_SHADOW_UPDATE_LIGHT_MASK_0, 1);
  Script_SetShadowFlags(SHEN_NAME('BTK_'), SCRIPT_SHADOW_UPDATE_LIGHT_MASK_0, 1);
  script_45B0(CHARA_AKIR, 1, 1, -1);
  script_45B0(CHARA_CHRL, 1, 1, -1);
  script_45B0(SHEN_NAME('BTA_'), 1, 1, -1);
  script_45B0(SHEN_NAME('BTH_'), 1, 1, -1);
  script_45B0(SHEN_NAME('BTK_'), 1, 1, -1);
  script_45B0(SHEN_NAME('BIK0'), 1, 0, -1);
  script_45B0(SHEN_NAME('BIK1'), 0, 0, -1);
  script_45B0(SHEN_NAME('BIK2'), 0, 0, -1);
  script_45B0(SHEN_NAME('BK2A'), 1, 0, -1);
  script_45B0(SHEN_NAME('RY00'), 1, 0, -1);
  script_45B0(SHEN_NAME('RY01'), 1, 0, -1);
  script_45B0(SHEN_NAME('RY10'), 0, 0, -1);
  script_45B0(SHEN_NAME('RY11'), 0, 0, -1);
  script_45B0(SHEN_NAME('RY20'), 1, 0, -1);
  script_45B0(SHEN_NAME('RY21'), 1, 0, -1);
  Script_motmToggleFlags(CHARA_AKIR, 2, 0x40);
  Script_motmToggleFlags(CHARA_CHRL, 2, 0x40);
  Script_motmToggleFlags(SHEN_NAME('BTA_'), 2, 0x40);
  Script_motmToggleFlags(SHEN_NAME('BTH_'), 2, 0x40);
  Script_motmToggleFlags(SHEN_NAME('BTK_'), 2, 0x40);
  Script_charaToggleMotEnabled(CHARA_AKIR, 0);
  Script_charaToggleMotEnabled(CHARA_CHRL, 0);
  Script_charaToggleMotEnabled(SHEN_NAME('BTA_'), 0);
  Script_charaToggleMotEnabled(SHEN_NAME('BTH_'), 0);
  Script_charaToggleMotEnabled(SHEN_NAME('BTK_'), 0);
  Script_ResetHandMotion(CHARA_AKIR, 1, 0, 0);
  Script_ResetHandMotion(CHARA_AKIR, 1, 1, 0);
  Script_ResetHandMotion(CHARA_CHRL, 1, 0, 0);
  Script_ResetHandMotion(CHARA_CHRL, 1, 1, 0);
  Script_executeAicaCommand(18088, 0, 0);
  Script_executeAicaCommand(1184, 2, 102);
  script_14EA(1);
  Script_executeAicaCommand(2720, 2, 30);
  SCNF_WaitFrames_0000(60);
  Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
  Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
  Script_ResetHandMotion(CHARA_CHRL, 0, 0, 0);
  Script_ResetHandMotion(CHARA_CHRL, 0, 1, 0);
  Script_motmToggleFlags(CHARA_AKIR, 1, 0x40);
  Script_motmToggleFlags(CHARA_CHRL, 1, 0x40);
  Script_motmToggleFlags(SHEN_NAME('BTA_'), 1, 0x40);
  Script_motmToggleFlags(SHEN_NAME('BTH_'), 1, 0x40);
  Script_motmToggleFlags(SHEN_NAME('BTK_'), 1, 0x40);
  Script_charaToggleMotEnabled(CHARA_AKIR, 1);
  Script_charaToggleMotEnabled(CHARA_CHRL, 1);
  Script_charaToggleMotEnabled(SHEN_NAME('BTA_'), 1);
  Script_charaToggleMotEnabled(SHEN_NAME('BTH_'), 1);
  Script_charaToggleMotEnabled(SHEN_NAME('BTK_'), 1);
  script_4602(CHARA_AKIR, 0, 0xFFFF, 0, 0x3F800000, 0, -1);
  script_4602(CHARA_CHRL, 0, 0xFFFF, 0, 0x3F800000, 0, -1);
  script_4602(CHARA_HTNK, 0, 0xFFFF, 0, 0x3F800000, 0, -1);
  script_4602(CHARA_TDSI, 0, 0xFFFF, 0, 0x3F800000, 0, -1);
  script_4602(SHEN_NAME('BTA_'), 0, 0xFFFF, 0, 0x3F800000, 0, -1);
  script_4602(SHEN_NAME('BTH_'), 0, 0xFFFF, 0, 0x3F800000, 0, -1);
  script_4602(SHEN_NAME('BTK_'), 0, 0xFFFF, 0, 0x3F800000, 0, -1);
  SCNF_WaitFrames_0000(1);
  Script_PREP_Related(-1, 0);
  Script_SetupLightFromTable(0);
  Script_ProcessFogFromTable(0);
  Script_PREP_Related(-1, 1);
  return;
}

void script_A05A(void) {
  Script_filmCheckOrSetTransitionUtil(0, 255, 0, 0, 0, 0, 0, 0, 0);
  SCNF_WaitFrames_0000(5);
  script_A0A2(0);
  task_0 = 0;
  for (;;) {
    task_0 = task_0 + 1;
    SCNF_WaitFrames_0000(1);
  }
  return;
}

__attribute__((scn_string_bindings(0, data_D5BB, 1, data_D5C0, 2, data_D5C5, 3, data_D5CA, 5, data_D5DA, 6, data_D5E0, 7, data_D5EE)))
static void script_A0A2(int32_t arg0) {
  task_8 = 1;
  Script_LoadSND(0, 2, "FREE", 0);
  Script_LoadSND(0, 3, "FREE", 0);
  Script_LoadSND(0, 4, "FREE", 0);
  Script_LoadSND(0, 5, "FREE", 0);
  Script_LoadSND(0, 1, "amb013.snd", 0);
  script_047C();
  Script_LoadVoiceFile(1, "E1069");
  script_13CB("scene/03/MFSY", "MBQC");
  Script_SetCameraFlags(0);
  Script_camSetRotationStepSize(0);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 50.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 50.0f);
  Script_SetFogFlags(2, 0);
  task_C = Script_SetMaxNPCsNear(32);
  return;
}

__attribute__((scn_string_bindings(4, data_D620, 5, data_D62E, 6, data_D637, 7, data_D640)))
static void script_A157(void) {
  task_8 = 2;
  Script_GetCurrentDrawMode(1, 0);
  Script_LoadSND(0, 2, "bgm076.snd", 0);
  Script_LoadSND(0, 4, "e1biche.snd", 0);
  Script_LoadSND(0, 5, "qtebat.snd", 0);
  task_10 = Script_motiLoad(3, SHEN_NAME('MBQC'), "M_MBQC.BIN");
  script_13CB("scene/03/MFSY", "QTE_MBQC");
  Script_scenCreateTask(script_385B, 0);
  Script_scenCreateTask(script_3DF9, 0);
  Script_scenCreateTask(script_ADDA, 3, SHEN_NAME('MKZ0'), 1, SHEN_NAME('RID0'));
  Script_scenCreateTask(script_AB33, 2, SHEN_NAME('KBD0'), 0);
  Script_scenCreateTask(script_AB33, 2, SHEN_NAME('KBD1'), 0);
  Script_scenCreateTask(script_AB33, 2, SHEN_NAME('KBK0'), 0);
  Script_scenCreateTask(script_AB33, 2, SHEN_NAME('KBK1'), 0);
  Script_scenCreateTask(script_AB33, 2, CHARA_BIKA, 0);
  Script_scenCreateTask(script_AB33, 2, SHEN_NAME('BIKC'), 0);
  Script_scenCreateTask(script_AB33, 2, SHEN_NAME('BIKD'), 0);
  Script_scenCreateTask(script_ACF0, 3, SHEN_NAME('BIKY'), SHEN_NAME('RYY0'), SHEN_NAME('RYY1'));
  Script_scenCreateTask(script_AB70, 5, SHEN_NAME('BIK0'), 4, SHEN_NAME('DMB0'), SHEN_NAME('RY00'), SHEN_NAME('RY01'));
  Script_scenCreateTask(script_AB70, 5, SHEN_NAME('BIK1'), 5, SHEN_NAME('DMB1'), SHEN_NAME('RY10'), SHEN_NAME('RY11'));
  Script_scenCreateTask(script_AB70, 5, SHEN_NAME('BIK2'), 6, SHEN_NAME('DMB2'), SHEN_NAME('RY20'), SHEN_NAME('RY21'));
  Script_scenCreateTask(script_AC6A, 3, SHEN_NAME('BK0A'), 0, 0x304B4942);
  Script_scenCreateTask(script_AC6A, 3, SHEN_NAME('BK2A'), 0, 0x324B4942);
  Script_scenCreateTask(script_A8C2, 5, SHEN_NAME('RID0'), 2, SHEN_NAME('DUM3'), 0, 0);
  Script_scenCreateTask(script_A8C2, 5, SHEN_NAME('RID1'), 3, SHEN_NAME('DUM4'), 0, 0);
  Script_scenCreateTask(script_A8C2, 5, CHARA_CHRL, 1, SHEN_NAME('DUM2'), 0, 0);
  Script_scenCreateTask(script_A6A2, 3, CHARA_AKIR, 0, SHEN_NAME('DUM1'));
  script_4B7E(CHARA_AKIR, 0);
  script_1409("QTE_MBQC");
  Script_ReallocateUNFO(*&task_10);
  Script_LoadSND(0, 5, "FREE", 0);
  Script_GetCurrentDrawMode(1, 1);
  return;
}

__attribute__((scn_string_bindings(1, data_D675, 7, data_D6BD, 8, data_D6CB, 9, data_D6D4, 10, data_D6DD, 11, data_D6EA, 12, data_D6F3, 13, data_D6F8, 16, data_D713, 17, data_D719, 18, data_D71F, 19, data_D728, 20, data_D72D)))
static void script_A3BC(void) {
  int32_t local_FC;
  int32_t local_F8;
  task_8 = 3;
  Script_LoadSND(0, 5, "battle_1.snd", 0);
  Script_AuthorUtils(0, 0, "seqdata0.bin", "scene/03/MBQC");
  *(&task_10 + 1) = Script_motiLoad(3, SHEN_NAME('FREE'), "M_BIKE.BIN");
  *(&task_10 + 2) = Script_motiLoad(3, SHEN_NAME('MB9Q'), "M_MB9Q.BIN");
  *(&task_10 + 3) = Script_motiLoad(3, SHEN_NAME('MBQC'), "M_01CBK.BIN");
  task_10 = Script_motiLoad(3, SHEN_NAME('MBQC'), "M_FGT1.BIN");
  script_13CB("scene/03/MFSY", "BAT_MBQC");
  local_FC = script_1441(0x4B4E5448);
  local_F8 = script_1441(0x49534454);
  SCNF_WaitFrames_0000(5);
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 2, "BAT_MBQC");
  Script_AuthorUtils(0, 0, "seqdata0.bin", "BAT_MBQC");
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
  Script_scenCreateTask(script_AA4C, 1, CHARA_HTNK);
  Script_scenCreateTask(script_AA4C, 1, CHARA_TDSI);
  Script_scenCreateTask(script_AA4C, 1, SHEN_NAME('BTA_'));
  Script_scenCreateTask(script_AA4C, 1, SHEN_NAME('BTH_'));
  Script_scenCreateTask(script_AA4C, 1, SHEN_NAME('BTK_'));
  script_853E(CHARA_AKIR, 0);
  Script_ReallocateUNFO(*&task_10);
  Script_LoadSND(0, 2, "FREE", 0);
  Script_LoadSND(0, 5, "FREE", 0);
  Script_LoadSND(0, 4, "a1_cbk.snd", 0);
  Script_LoadSND(0, 2, "bgm063.snd", 0);
  Script_GetCurrentDrawMode(1, 0);
  Script_LoadVoiceFile(1, "01CBK");
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 80.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 80.0f);
  Script_SetFogColour(&data_D648);
  script_9AD0(0x52494B41, 0);
  Script_LoadVoiceFile(0, "01CBK");
  Script_GetCurrentDrawMode(1, 1);
  Script_AuthorUtils(AUTH_FREE, 0);
  script_14C7(0x4B4E5448, local_FC);
  script_14C7(0x49534454, local_F8);
  script_1409("BAT_MBQC");
  Script_ReallocateUNFO(*(&task_10 + 1));
  Script_ReallocateUNFO(*(&task_10 + 2));
  Script_ReallocateUNFO(*(&task_10 + 3));
  Script_LoadSND(0, 4, "FREE", 0);
  Script_LoadSND(0, 2, "FREE", 0);
  return;
}

__attribute__((scn_string_bindings(0, data_D732, 1, data_D737)))
static void script_A633(void) {
  task_8 = 5;
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 220.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 100.0f);
  Script_SetMaxNPCsNear(task_C);
  Script_SetCameraSettings(5, 6, 1, 0);
  Script_convSetCamInactiveFlag(1);
  SCNF_WaitFrames_0000(2);
  Script_SetGameStateTo(AUTH_None);
  Script_SetCameraFlags(1);
  Script_camSetRotationStepSize(3);
  script_1409("MBQC");
  Script_LoadVoiceFile(0, "E1069");
  return;
}

static int32_t script_A6A2(int32_t arg0, int32_t arg1, int32_t arg2) {
  int32_t unused_FC;
  int32_t local_F8;
  float local_F4;
  local_F8 = Script_scenCreateTask(script_074D, 6, arg0, arg1, 0, 2, &data_C3F0, script_1DBA);
  Script_facemotUtils(arg0, 0, 0);
  Script_facemotUtils(arg0, 1, 0);
  Script_FaceEx(arg0, 1, 2, 4);
  Script_FACE(arg0, 2, 0, 4);
  Script_FACE_LCG(arg0, 2, 4, 0);
  Script_FaceLookAtTargetUtils(arg0, 0);
  Script_ryoSetChara(arg0);
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_ccowUtils(arg0, 2);
  Script_motmGetCharaFlags(arg0, 0, 0);
  Script_motmToggleFlags(arg0, 1, 0x40);
  Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_7);
  Script_PlayCharaMotion(arg0, MN_AKI_AKI_L_STAND, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_SetFightProgramStateBytes(arg0, 2, 1);
  Script_SetupClothData(arg0, 8);
  Script_SetCPAMFlags(arg0, 0);
  Script_MoveObjectToObject(arg0, 0x304D5544);
  Script_MoveObjectToObject(arg2, arg0);
  Script_EVT_SetPosition(arg2, POS_XYZ, &data_D740);
  Script_EVT_SetRotation(arg2, 0x38000000, &data_D74C, 0);
  Script_SetCharaFlags(arg0, 0);
  Script_SetShadowFlags(arg0, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_scenToggleActiveFor(arg0, 0);
  Script_charaVisUtils(arg0, 1);
  Script_charaDisplay(arg0, CDM_Show);
  local_F4 = Script_charaSetClipAngle(arg0, -1.0f);
  Script_charaSetClipAngle(arg0, 30.0f);
  for (;;) {
    if (task_8 == 5) {
      Script_SetCharaFlags(arg0, 0);
      Script_SetShadowFlags(arg0, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
      Script_scenToggleActiveFor(arg0, 1);
      Script_charaDisplay(arg0, CDM_Show);
      Script_charaSetClipAngle(arg0, local_F4);
      Script_ccowUtils(arg0, 1);
      Script_motmGetCharaFlags(arg0, 3, 0);
      Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_5);
      Script_PlayCharaMotion(arg0, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
      Script_CharHandlers(arg0, -1, 0, 0);
      Script_SetCPAMFlags(arg0, 1);
      Script_FaceEx(arg0, 0, 0, 0);
      Script_faceUpdateFor(arg0, 0, 0);
      Script_faceUpdateFor(arg0, 1, 0);
      Script_srchControl((void *)task_A4[arg1]);
      Script_srchControl((void *)local_F8);
      Script_srchControl(0);
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_A8C2(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  int32_t unused_FC;
  int32_t local_F8;
  float local_F4;
  local_F8 = Script_scenCreateTask(script_074D, 6, arg0, arg1, arg3, arg4, &data_C3F0, script_1DBA);
  if (arg1 == 1) {
    Script_facemotUtils(arg0, 0, 0);
    Script_facemotUtils(arg0, 1, 0);
    Script_FaceEx(arg0, 1, 1, 4);
    Script_FACE(arg0, 1, 0, 4);
    Script_FACE_LCG(arg0, 2, 4, 0);
    Script_FaceLookAtTargetUtils(arg0, 0);
  }
  Script_ccowUtils(arg0, 2);
  Script_charaToggleMotEnabled(arg0, 1);
  Script_motmGetCharaFlags(arg0, 0, 0);
  Script_motmToggleFlags(arg0, 1, 0x40);
  Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_7);
  Script_PlayCharaMotion(arg0, MN_AKI_AKI_L_STAND, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_SetFightProgramStateBytes(arg0, 2, 1);
  Script_SetCPAMFlags(arg0, 0);
  Script_MoveObjectToObject(arg0, 0x304D5544);
  Script_MoveObjectToObject(arg2, arg0);
  Script_EVT_SetPosition(arg2, POS_XYZ, &data_D758);
  Script_EVT_SetRotation(arg2, 0x38000000, &data_D764, 0);
  Script_SetCharaFlags(arg0, 0);
  Script_SetShadowFlags(arg0, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_SetShadowFlags(arg0, SCRIPT_SHADOW_UPDATE_LIGHT_MASK, 2);
  Script_SetShadowFlags(arg0, SCRIPT_SHADOW_UPDATE_LIGHT_MASK, 3);
  Script_scenToggleActiveFor(arg0, 0);
  Script_charaDisplay(arg0, CDM_Show);
  local_F4 = Script_charaSetClipAngle(arg0, -1.0f);
  Script_charaSetClipAngle(arg0, 30.0f);
  for (;;) {
    if (task_8 == 5) {
      Script_srchControl((void *)task_A4[arg1]);
      Script_srchControl((void *)local_F8);
      Script_srchControl(0);
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_AA4C(int32_t arg0) {
  float local_FC;
  Script_ccowUtils(arg0, 2);
  Script_charaToggleMotEnabled(arg0, 1);
  Script_motmGetCharaFlags(arg0, 0, 0);
  Script_motmToggleFlags(arg0, 1, 0x40);
  Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_5);
  Script_PlayCharaMotion(arg0, MN_AKI_AKI_L_STAND, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_SetFightProgramStateBytes(arg0, 2, 1);
  Script_SetCPAMFlags(arg0, 0);
  Script_MoveObjectToObject(arg0, 0x304D5544);
  Script_SetCharaFlags(arg0, 0);
  Script_SetShadowFlags(arg0, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_SetShadowFlags(arg0, SCRIPT_SHADOW_UPDATE_LIGHT_MASK, 2);
  Script_SetShadowFlags(arg0, SCRIPT_SHADOW_UPDATE_LIGHT_MASK, 3);
  Script_scenToggleActiveFor(arg0, 0);
  Script_charaDisplay(arg0, CDM_Show);
  Script_charaVisUtils(arg0, 1);
  local_FC = Script_charaSetClipAngle(arg0, -1.0f);
  Script_charaSetClipAngle(arg0, 30.0f);
  for (;;) {
    if (task_8 == 5) {
      Script_srchControl(0);
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_AB33(int32_t arg0, int32_t arg1) {
  Script_MoveObjectToObject(arg0, 0x304D5544);
  Script_SetShadowFlags(arg0, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_scenToggleActiveFor(arg0, 0);
  for (;;) {
    if (task_8 == 5) {
      Script_srchControl(0);
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_AB70(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  int32_t unused_E8[6];
  Script_MoveObjectToObject(arg2, 0x304D5544);
  Script_MoveObjectToObject(arg0, arg2);
  Script_MoveObjectToObject(arg3, arg2);
  Script_MoveObjectToObject(arg4, arg0);
  Script_EVT_SetPosition(arg3, POS_XYZ, &data_D7A0);
  Script_EVT_SetRotation(arg3, 0x38000000, &data_D794, 0);
  Script_EVT_SetPosition(arg4, POS_XYZ, &data_D788);
  Script_EVT_SetRotation(arg4, 0x38000000, &data_D794, 0);
  Script_SetLampState(0, arg0, 0);
  Script_scenToggleActiveFor(arg0, 0);
  Script_scenToggleActiveFor(arg3, 0);
  Script_scenToggleActiveFor(arg4, 0);
  Script_hmdlApplyScale(arg0, &data_D7AC);
  Script_hmdlApplyScale(arg4, &data_D7AC);
  Script_charaSetClipAngle(arg0, 30.0f);
  Script_charaSetClipAngle(arg3, 30.0f);
  Script_charaSetClipAngle(arg4, 30.0f);
  Script_charaDisplay(arg0, CDM_Show);
  Script_charaDisplay(arg3, CDM_Show);
  Script_charaDisplay(arg4, CDM_Show);
  for (;;) {
    if (task_8 == 5) {
      Script_srchControl(0);
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_AC6A(int32_t arg0, int32_t arg1, int32_t arg2) {
  if (arg2 != 0) {
    Script_MoveObjectToObject(arg0, arg2);
    Script_EVT_SetPosition(arg0, POS_XYZ, &data_D7B8);
    Script_EVT_SetRotation(arg0, 0x38000000, &data_D7C4, 0);
  } else {
    Script_MoveObjectToObject(arg0, 0x304D5544);
  }
  Script_scenToggleActiveFor(arg0, 0);
  Script_hmdlApplyScale(arg0, &data_D7DC);
  Script_charaSetClipAngle(arg0, 30.0f);
  Script_charaDisplay(arg0, CDM_Show);
  for (;;) {
    if (task_8 == 5) {
      Script_srchControl(0);
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_ACF0(int32_t arg0, int32_t arg1, int32_t arg2) {
  Script_MoveObjectToObject(arg0, 0x304D5544);
  Script_MoveObjectToObject(arg1, arg0);
  Script_MoveObjectToObject(arg2, 0x59424D44);
  Script_MoveObjectToObject(SHEN_NAME('DMBY'), arg0);
  Script_EVT_SetPosition(arg1, POS_XYZ, &data_D7F4);
  Script_EVT_SetRotation(arg1, 0x38000000, &data_D800, 0);
  Script_EVT_SetPosition(arg2, POS_XYZ, &data_D7E8);
  Script_EVT_SetRotation(arg2, 0x38000000, &data_D800, 0);
  Script_scenToggleActiveFor(arg0, 0);
  Script_scenToggleActiveFor(arg1, 0);
  Script_scenToggleActiveFor(arg2, 0);
  Script_hmdlApplyScale(arg2, &data_D80C);
  Script_charaSetClipAngle(arg0, 30.0f);
  Script_charaSetClipAngle(arg1, 30.0f);
  Script_charaSetClipAngle(arg2, 30.0f);
  Script_charaDisplay(arg0, CDM_Show);
  Script_charaDisplay(arg1, CDM_Show);
  Script_charaDisplay(arg2, CDM_Show);
  for (;;) {
    if (task_8 == 5) {
      Script_srchControl(0);
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_ADDA(int32_t arg0, int32_t arg1, int32_t arg2) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t local_E4;
  SCNF_WaitFrames_0000(1);
  Script_scenToggleActiveFor(arg0, 0);
  Script_charaDisplay(arg0, CDM_Show);
  local_E4 = arg1;
  switch (local_E4) {
    case 0:
      local_F4.x = -0.055f;
      (&local_F4)->y = -0.1f;
      (&local_F4)->z = -0.025f;
      local_E8.x = 0;
      (&local_E8)->y = 0;
      (&local_E8)->z = 62805;
      Script_fixoAttachEx(arg0, arg2, 18, &local_F4, &local_E8);
      break;
    case 1:
      local_F4.x = 0.055f;
      (&local_F4)->y = -0.1f;
      (&local_F4)->z = 0.025f;
      local_E8.x = 0;
      (&local_E8)->y = 0;
      (&local_E8)->z = 62805;
      Script_fixoAttachEx(arg0, arg2, 12, &local_F4, &local_E8);
      break;
  }
  for (;;) {
    if (task_8 == 5) {
      Script_srchControl(0);
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}
