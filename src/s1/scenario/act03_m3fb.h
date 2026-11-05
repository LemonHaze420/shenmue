#pragma once

#include "scenario.h"
#include "motion/motion.h"

static void script_0030(int32_t arg0, float arg1, float arg2, float arg3);
static void script_0058(int32_t arg0, float arg1, float arg2, float arg3);
static void script_00AC(int32_t arg0, int32_t arg1, int32_t arg2);
static int32_t script_00FC(int32_t arg0);
static void script_01A3(int32_t arg0, int32_t arg1);
static int32_t script_01C6(int32_t arg0);
static void script_01F9(int32_t arg0, int32_t arg1);
static void script_021C(int32_t arg0, int32_t arg1);
static void script_027B(int32_t arg0);
static void script_0290(void);
static void script_044A(void);
static void script_0574(void);
static void script_05A5(void);
static void script_05D6(void);
static void script_06DC(void);
static void script_06E4(void);
static void script_06ED(void);
static void script_06FC(int32_t arg0);
static void script_0789(int32_t arg0);
static void script_07D9(void);
static void script_0807(int32_t arg0);
static void script_0835(void);
static void script_0869(int32_t arg0);
static void script_089D(int32_t arg0);
static void script_08BB(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_08F5(int32_t arg0, int32_t arg1);
static void script_0929(void);
static void script_0A21(int32_t arg0, int32_t arg1);
static void script_0A51(int32_t arg0);
static void script_0A80(int32_t arg0);
static void script_0B09(void);
static void script_0BC7(void);
static int32_t script_0D08(void);
static int32_t script_0F3E(void);
static void script_123B(void);
static void script_124E(void);
static int32_t script_1261(void);
static void script_139A(void);
static int32_t script_13A7(void);
static void script_162A(void);
static void script_167D(void);
static int32_t script_174F(void);
static int32_t script_17D9(void);
static int32_t script_1837(void);
static int32_t script_18B9(void);
static int32_t script_1983(void);
static void script_1A3F(void);
static int32_t script_1F01(void);
static int32_t script_20A2(void);
static int32_t script_20AE(void);
static int32_t script_20BA(void);
static int32_t script_20C6(void);
static int32_t script_20D2(void);
static int32_t script_20DE(void);
static int32_t script_20EA(void);
static int32_t script_20F6(void);
static int32_t script_2102(void);
static void script_210E(void);
static void script_211C(float arg0);
static void script_22EA(int32_t arg0, int32_t arg1);
static void script_231A(int32_t arg0, int32_t arg1);
static void script_234A(void);
static void script_24D1(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_2521(int32_t arg0, int32_t arg1);
static void script_25E0(int32_t arg0, int32_t arg1);
static void script_27A8(void);
static void script_281C(void);
static void script_2890(void);
static void script_28C3(void);
void script_28F2(void);
static int32_t script_2B46(void);
static void script_2CE9(void);
static void script_2D85(void);
static void script_2EAD(void);
static void script_2EE3(void);
static void script_307C(void);
static void script_3134(int8_t arg0);
static int32_t script_3165(int32_t arg0);
static int32_t script_3199(int32_t arg0);

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
static int8_t task_6C;
static uint8_t task_6D;
static uint16_t task_6E;
static int32_t task_70;
static Vector3 task_74;
static int32_t task_80;
static int8_t task_84;
static uint8_t task_85;
static uint16_t task_86;
static int32_t task_88;
static int8_t task_8C;
static uint8_t task_8D;
static uint16_t task_8E;
static int8_t task_90;
static uint8_t task_91;
static uint8_t task_92;
static const Vector3 data_31D0 = {.x = 1.0f, .y = 1.0f, .z = 1.0f};
static const int32_t data_31DC[4] = {16777215, 16777215, 16777215, 16777215};
static const float data_31EC[3] = {20.0f, 800.0f, 1.0f};
static const int32_t data_31F8 = -1;
static const uint32_t data_31FC = 0x41A00000;
static const float data_3200[2] = {0.0f, -50.0f};
static const uint32_t data_3208 = 0x41A00000;
static const float data_320C[2] = {0.0f, -50.0f};
static const uint32_t data_3214 = 0xC2200000;
static const float data_3218[2] = {0.0f, 80.0f};
static const uint32_t data_3220 = 0xC2D60000;
static const float data_3224[2] = {0.0f, 78.0f};
static const uint8_t data_322C[8] = {0};
static const float data_3234 = 90.0f;
static const uint32_t data_3238 = 0xC1A00000;
static const float data_323C[2] = {0.0f, -50.0f};
static const uint32_t data_3244 = 0xC1A00000;
static const float data_3248[2] = {0.0f, -50.0f};
static const uint32_t data_3250 = 0xC2E60000;
static const float data_3254[2] = {0.0f, 175.0f};
static const uint32_t data_325C = 0xC2DC0000;
static const float data_3260[2] = {0.0f, 10.0f};
static const uint32_t data_3268 = 0x42C80000;
static const float data_326C[2] = {0.0f, -40.0f};
static const uint32_t data_3274 = 0x43020000;
static const float data_3278[2] = {0.0f, 25.0f};
static const uint8_t data_3280[12] = {0};
static const uint32_t data_328C = 0x3F800000;
static const float data_3290[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint32_t data_3314 = 0x3F800000;
static const float data_3318[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint8_t data_339C[16] = {0};
static const float data_33AC[30] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint32_t data_3424 = 0x3F800000;
static const float data_3428[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint32_t data_34AC = 0x3F800000;
static const float data_34B0[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint8_t data_3534[84] = {0};
typedef struct Row_3588 { float w0; float w1; float w2; int32_t w3; } Row_3588;
static const Row_3588 data_3588[3] = {
  {0.787f, 0.787f, 0.787f, 0},
  {0.0f, 0.0f, 0.0f, 0},
  {0.0f, 0.0f, 0.0f, 0}
};
static const int32_t data_35B8 = 0;
static const uint32_t data_35BC = 0x3F800000;
static const float data_35C0[33] = {
  0.944f, 0.866f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f,
  0.787f, 0.787f, 0.787f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint32_t data_3644 = 0x3F4978D5;
static const float data_3648[33] = {
  0.787f, 0.866f, 0.944f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  0.944f, 0.866f, 0.787f, 0.787f, 0.787f, 0.787f, 0.708f, 0.708f, 0.708f, 0.708f
};
static const uint8_t data_36CC[12] = {0};
static const float data_36D8[31] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint32_t data_3754 = 0x3F800000;
static const float data_3758[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint32_t data_37DC = 0x3F800000;
static const float data_37E0[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint32_t data_3864 = 0x3F800000;
static const float data_3868[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint32_t data_38EC = 0x41A00000;
static const float data_38F0[2] = {0.0f, -50.0f};
static const uint32_t data_38F8 = 0x41A00000;
static const float data_38FC[2] = {0.0f, -50.0f};
static const uint32_t data_3904 = 0xC2200000;
static const float data_3908[2] = {0.0f, 80.0f};
static const uint32_t data_3910 = 0xC1A00000;
static const float data_3914[2] = {0.0f, -50.0f};
static const uint32_t data_391C = 0xC1A00000;
static const float data_3920[2] = {0.0f, -50.0f};
static const uint32_t data_3928 = 0xC2E60000;
static const float data_392C[2] = {0.0f, 175.0f};
static const uint32_t data_3934 = 0x42C80000;
static const float data_3938[2] = {0.0f, -40.0f};
static const uint8_t data_3940[12] = {0};
static const uint32_t data_394C = 0x3F800000;
static const float data_3950[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint32_t data_39D4 = 0x3F800000;
static const float data_39D8[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint8_t data_3A5C[16] = {0};
static const float data_3A6C[30] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint8_t data_3AE4[84] = {0};
typedef struct Row_3B38 { float w0; float w1; float w2; int32_t w3; } Row_3B38;
static const Row_3B38 data_3B38[3] = {
  {0.787f, 0.787f, 0.787f, 0},
  {0.0f, 0.0f, 0.0f, 0},
  {0.0f, 0.0f, 0.0f, 0}
};
static const int32_t data_3B68 = 0;
static const uint32_t data_3B6C = 0x3F800000;
static const float data_3B70[33] = {
  0.944f, 0.866f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f,
  0.787f, 0.787f, 0.787f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint32_t data_3BF4 = 0x3F4978D5;
static const float data_3BF8[33] = {
  0.787f, 0.866f, 0.944f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  0.944f, 0.866f, 0.787f, 0.787f, 0.787f, 0.787f, 0.708f, 0.708f, 0.708f, 0.708f
};
static const uint32_t data_3C7C = 0x3F800000;
static const float data_3C80[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint32_t data_3D04 = 0x3F800000;
static const float data_3D08[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint32_t data_3D8C = 0xC2A50000;
static const float data_3D90[2] = {0.0f, 68.0f};
static const uint32_t data_3D98 = 0x3F800000;
static const float data_3D9C[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint32_t data_3E20 = 0xC1D00000;
static const float data_3E24[2] = {0.0f, 76.5f};
static const uint32_t data_3E2C = 0x3F800000;
static const float data_3E30[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint32_t data_3EB4 = 0x41A00000;
static const float data_3EB8[2] = {0.0f, -50.0f};
static const uint32_t data_3EC0 = 0x41A00000;
static const float data_3EC4[2] = {0.0f, -50.0f};
static const uint32_t data_3ECC = 0xC2200000;
static const float data_3ED0[2] = {0.0f, 80.0f};
static const uint32_t data_3ED8 = 0xC1A00000;
static const float data_3EDC[2] = {0.0f, -50.0f};
static const uint32_t data_3EE4 = 0xC1A00000;
static const float data_3EE8[2] = {0.0f, -50.0f};
static const uint32_t data_3EF0 = 0xC2E60000;
static const float data_3EF4[5] = {0.0f, 175.0f, 100.0f, 0.0f, -40.0f};
static const uint8_t data_3F08[12] = {0};
static const uint32_t data_3F14 = 0x3F800000;
static const float data_3F18[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint32_t data_3F9C = 0x3F800000;
static const float data_3FA0[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint8_t data_4024[16] = {0};
static const float data_4034[30] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint8_t data_40AC[84] = {0};
typedef struct Row_4100 { float w0; float w1; float w2; int32_t w3; } Row_4100;
static const Row_4100 data_4100[3] = {
  {0.787f, 0.787f, 0.787f, 0},
  {0.0f, 0.0f, 0.0f, 0},
  {0.0f, 0.0f, 0.0f, 0}
};
static const int32_t data_4130 = 0;
static const uint32_t data_4134 = 0x3F800000;
static const float data_4138[33] = {
  0.944f, 0.866f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f,
  0.787f, 0.787f, 0.787f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint32_t data_41BC = 0x3F4978D5;
static const float data_41C0[33] = {
  0.787f, 0.866f, 0.944f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  0.944f, 0.866f, 0.787f, 0.787f, 0.787f, 0.787f, 0.708f, 0.708f, 0.708f, 0.708f
};
static const uint32_t data_4244 = 0x3F800000;
static const float data_4248[90] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const int32_t data_43B0[684] = {
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
  -1638, 0, 0, 1092, 0, 0, -5461, 0, -2366, -1092, 0, -182, 1092, 0, -546, -6735, 0, 0, -4004, 0, 0, -6553, 0, -182, 1274, 0, 0, -13289, 0, 0, -5279,
  0, 0, -8009, 0, 0, 1456, 0, 182, -8738, 0, 0, -6007, 0, 0, -8738, 0, 182, 1638, 0, -728, -5279, 0, -1274, -6735, 0, -910, -8738, 0, 0, 1638, 0, 0,
  -9102, 0, -2366, -2002, 0, -182, 910, 0, -546, -2366, 0, 0, -2002, 0, 0, -2912, 0, -182, 1274, 0, 0, -14199, 0, 0, -11468, 0, 0, -13653, 0, 0, 1456,
  0, 182, -11286, 0, 0, -12014, 0, 0, -12743, 0, 182, 1638, 0, -728, -10194, 0, -1274, -12196, 0, -910, -10922, -728, 546, 1638, 0, 0, -1820, 0,
  -2366, -728, 0, -364, -1092, 0, -546, -11832, 0, 910, -6735, 0, 0, -6553, 0, -182, 1274, 0, 0, -13289, 0, 0, -6007, 0, 0, -10012, 0, 0, 1092, 0,
  182, -8738, 0, 0, -6735, 0, 0, -9830, 0, 182, 1638, 0, -728, -6553, 0, -1274, -7281, 0, -910, -10740, 0, 0, 1638, 0, 0, -5461, -2912, -2366, -364,
  -5279, 2548, -910, 0, -546, -8374, 0, 0, -9466, 0, 910, -4915, 0, -182, 546, 0, 0, -13289, 0, 0, -5279, 0, 364, -7645, 0, -182, 1456, 0, 182, -8738,
  0, 0, -7281, 0, 182, -6917, 0, -182, 1638, 0, -728, -5279, 0, -1274, -6735, 0, -182, -8009, 0, 0, 1638, -4733, 0, -8556, 0, -2366, -1092, -6189,
  2912, 0, 0, -546, -10922, 0, 1820, -16384, 0, 0, -11468, 0, 182, 546, 0, 0, -10376, 0, 0, -15837, 0, 0, -12743, 0, -182, 728, 0, 182, -13835, 0, 0,
  -16201, 0, 364, -10922, 0, -182, 728, 0, -728, -9648, 0, -3458, -12379, 0, -728, -7463, 0, 0, 1092, 0, 0, -5461, 0, -2366, -1638, 0, -910, 1092, 0,
  -546, -3640, 0, 0, -2366, 0, 0, -1456, 0, -182, 1638, 0, 0, -910, 0, 0, -2730, 0, 0, -1274, 0, 0, 1820, 0, 182, -2730, 0, 0, -1820, 0, 0, -1274, 0,
  182, 2184, 0, -728, -3094, 0, -910, -2548, 0, 910, -1456, -546, 910, 2366
};
static const int32_t data_4E60[57] = {
  0, 0, -3640, 0, 0, -2366, -4369, 1820, -546, 0, 0, -5279, 0, 0, -6007, 0, 0, -2912, 0, 0, -910, 0, 0, -6553, 0, 0, -6735, 0, 0, -3640, 0, 0, -182,
  0, 0, -5461, 0, 0, -6007, 0, 0, -2366, 0, 0, 0, 0, 0, -5643, 0, 0, -4733, 0, 0, -2366, -1456, 0, 728
};
static const Vector4 data_4F44 = {.x = 0.3f, .y = 0.3f, .z = 0.3f, .w = 0.3f};
static Vector3 data_4F54 = {.x = 0.0f, .y = 0.175f, .z = 0.0f};
static Vector3i data_4F60 = {.x = 0, .y = 0, .z = 0};
static Vector3i data_4F6C = {.x = -2730, .y = 0, .z = 0};
static const Vector3 data_4F78 = {.x = 0.0f, .y = 0.0f, .z = 0.0f};
static const uint32_t data_4F84 = SHEN_NAME('AKIR');
typedef struct Row_4F88 { int32_t w0; float w1; int32_t w2; float w3; float w4; int32_t w5; int32_t w6; shen_name w7; } Row_4F88;
static const Row_4F88 data_4F88[9] = {
  {200, -2.6f, 0, 4.2f, 0.0f, 0, 787, CHARA_HARY},
  {60, -2.6f, 0, 2.6f, 180.0f, 25, 2209, CHARA_JONZ},
  {60, -3.3f, 0, 0.4f, 180.0f, 23, 2205, CHARA_DICK},
  {60, -2.2f, 0, -0.1f, 180.0f, 24, 2209, CHARA_RBRT},
  {80, -2.7f, 0, 5.5f, 0.0f, 42, 2209, CHARA_BOB_},
  {80, -3.5f, 0, 5.5f, 0.0f, 43, 2205, CHARA_SERA},
  {60, 2.6f, 0, -4.5f, 90.0f, 11, 8201, CHARA_PAUR},
  {80, 2.6f, 0, -6.6f, 90.0f, 44, 8201, CHARA_SMTH},
  {80, 3.5f, 0, -4.8f, 45.0f, 26, 8198, CHARA_TONY}
};
static const int32_t data_50A8 = 80;
static const float data_50AC[4] = {3.5f, 0.0f, -6.3f, 135.0f};
static const int32_t data_50BC[2] = {22, 8198};
static const char data_50C4[6] __attribute__((scn_string_literal)) = "E1073";
static const char data_50CA[6] __attribute__((scn_string_literal)) = "E1073";
static const char data_50D0[10] __attribute__((scn_string_literal)) = "E1073B001";
static const char data_50DA[10] __attribute__((scn_string_literal)) = "E1073B002";
static const char data_50E4[10] __attribute__((scn_string_literal)) = "E1073B003";
static const char data_50EE[10] __attribute__((scn_string_literal)) = "E1073A001";
static const char data_50F8[10] __attribute__((scn_string_literal)) = "E1073B004";
static const char data_5102[10] __attribute__((scn_string_literal)) = "E1073A002";
static const char data_510C[10] __attribute__((scn_string_literal)) = "E1073B005";
static const char data_5116[10] __attribute__((scn_string_literal)) = "E1073A003";
static const char data_5120[12] __attribute__((scn_string_literal)) = "E1073B006";
static const Vector4 data_512C = {.x = 0.3f, .y = 0.3f, .z = 0.3f, .w = 0.3f};
static const char data_513C[16] __attribute__((scn_string_literal)) = "PRESS A BUTTON";
static const uint32_t data_514C = 0x40E561E5;
static const uint32_t data_5150[3] = {0x42C3D99AU, 0x41825DCCU, 0x42B16D29U};
static const float data_515C = 28.3678f;
static const uint32_t data_5160 = 0x42C99206U;
static const float data_5164 = 19.24f;
static const uint32_t data_5168 = 0x42DBD368U;
static const float data_516C[6] = {-0.3f, 10.1f, 17.5f, 1.2f, 98.8f, 20.0f};
static const char data_5184[16] __attribute__((scn_string_literal)) = "/scene/03/MA00/";
static const char data_5194[7] __attribute__((scn_string_literal)) = "ALLMAP";
static const char data_519B[16] __attribute__((scn_string_literal)) = "/scene/03/MA00/";
static const char data_51AB[7] __attribute__((scn_string_literal)) = "PK_BOX";
static const char data_51B2[16] __attribute__((scn_string_literal)) = "/scene/03/M3FB/";
static const char data_51C2[5] __attribute__((scn_string_literal)) = "M3FB";
static const char data_51C7[6] __attribute__((scn_string_literal)) = "01LFT";
static const char data_51CD[6] __attribute__((scn_string_literal)) = "01LFT";
static const char data_51D3[5] __attribute__((scn_string_literal)) = "M3FB";
static const char data_51D8[7] __attribute__((scn_string_literal)) = "PK_BOX";
static const char data_51DF[7] __attribute__((scn_string_literal)) = "ALLMAP";
static const char data_51E6[13] __attribute__((scn_string_literal)) = "seqdata1.bin";
static const char data_51F3[5] __attribute__((scn_string_literal)) = "M3FB";
static const char data_51F8[13] __attribute__((scn_string_literal)) = "seqdata2.bin";
static const char data_5205[7] __attribute__((scn_string_literal)) = "M3FB";
static const uint32_t data_520C[4] = {0, 0, 0xD1FFFFFFU, 0xD1FFFFFFU};
static const uint32_t data_521C = SHEN_NAME('BOX1');
static const shen_name data_5220[19] = {
  SHEN_NAME('BOX2'), SHEN_NAME('BOX3'), SHEN_NAME('BOX4'), SHEN_NAME('BOX5'), SHEN_NAME('BOX6'), SHEN_NAME('BOX7'), SHEN_NAME('BOX8'),
  SHEN_NAME('BOX9'), SHEN_NAME('BOXA'), SHEN_NAME('BOXB'), SHEN_NAME('BOXC'), SHEN_NAME('BOXD'), SHEN_NAME('BOXE'), SHEN_NAME('BOXF'),
  SHEN_NAME('BOXG'), SHEN_NAME('BOXH'), SHEN_NAME('BOXI'), SHEN_NAME('BOXJ'), SHEN_NAME('BOXK')
};
static const char data_526C[8] __attribute__((scn_string_literal)) = "FREE";
