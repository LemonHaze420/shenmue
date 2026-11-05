#pragma once

#include "scenario.h"
#include "motion/motion.h"

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
