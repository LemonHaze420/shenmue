#pragma once

#include "scenario.h"
#include "motion/motion.h"

static void script_0030(void);
static void script_0039(void);
static void script_0048(int32_t arg0);
static void script_00D5(int32_t arg0);
static void script_0125(void);
static int32_t script_0153(int32_t arg0);
static void script_0181(void);
static void script_01B5(int32_t arg0);
static void script_01E9(int32_t arg0);
static int32_t script_0207(int32_t arg0, int32_t arg1, int32_t arg2);
static int32_t script_0241(int32_t arg0, int32_t arg1);
static void script_0275(void);
static void script_036D(int32_t arg0, int32_t arg1);
static void script_039D(void);
static void script_0557(void);
static void script_0681(void);
static void script_06B2(void);
static void script_06E3(void);
static void script_07E9(void);
static void script_07F1(int32_t arg0);
static void script_081F(void);
static int32_t script_0901(void);
static int32_t script_0AB4(void);
static void script_0C17(void);
static void script_0CB8(void);
static void script_0D71(int32_t arg0);
static void script_0DE4(int32_t arg0);
static void script_0E0D(int32_t arg0);
static void script_0E30(int32_t arg0);
static void script_0E76(int32_t arg0, int32_t arg1);
static void script_1210(int32_t arg0, int32_t arg1);
static void script_191C(int32_t arg0, int32_t arg1);
static void script_1B81(void);
static void script_1DF1(void);
static void script_1F65(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4);
static int32_t script_1F7D(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4);
static void script_1FA4(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static int32_t script_1FBE(int32_t arg0, int32_t arg1, int32_t arg2, float arg3, int32_t arg4, int32_t arg5);
static void script_2046(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static int32_t script_2060(int32_t arg0, int32_t arg1, float arg2, float arg3, float arg4, int32_t arg5);
static void script_20EB(int32_t arg0, int32_t arg1);
static int32_t script_20FD(int32_t arg0, int32_t arg1);
static void script_2121(int32_t arg0, int32_t arg1, int32_t arg2);
static int32_t script_2135(int32_t arg0, float arg1, int32_t arg2);
static void script_217E(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_2194(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_21D7(int32_t arg0, int32_t arg1, int32_t arg2);
static int32_t script_21EB(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_221D(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_2233(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_227F(int32_t arg0, int32_t arg1);
static int32_t script_2291(int32_t arg0, int32_t arg1);
static void script_23C9(int32_t arg0);
static void script_243C(int32_t arg0);
static void script_2459(int32_t arg0);
static void script_2479(void);
static void script_2584(void);
static void script_2B20(void);
static void script_2CF1(void);
static void script_3129(void);
static void script_32BE(int32_t arg0);
static int32_t script_32CE(int32_t arg0);
static void script_3330(int32_t arg0);
static int32_t script_3340(int32_t arg0);
static void script_3383(int32_t arg0);
static int32_t script_3393(int32_t arg0);
static void script_33D6(int32_t arg0);
static int32_t script_33E6(int32_t arg0);
static void script_341B(void);
static void script_3D77(void);
void script_4596(void);
static int32_t script_462F(void);
static void script_49F5(void);
static void script_4BC4(void);
static void script_4E4D(void);
static void script_4E9B(int32_t arg0, int32_t arg1);
static int32_t script_4EB4(const char *arg0, const char *arg1);
static void script_4F38(void);
static void script_504E(void);
static void script_512D(void);

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
static int8_t task_7C;
static uint8_t task_7D;
static uint16_t task_7E;
static GameDateTime task_80;
static int32_t task_88;
typedef struct Row_51F0 { float w0; float w1; float w2; int32_t w3; int32_t w4; int32_t w5; int32_t w6; } Row_51F0;
static const Row_51F0 data_51F0[9] = {
  {1.0f, 1.0f, 1.0f, 16777215, 16777215, 16777215, 16777215},
  {20.0f, 800.0f, 1.0f, -1, 0, 0, 0},
  {0.0f, 0.0f, 0.0f, 0, 0, 0, 0},
  {0.0f, 0.0f, 0.0f, 0, 0, 0, 0},
  {0.0f, 0.0f, 0.0f, 0, 0, 0, 0},
  {0.0f, 0.0f, 0.0f, 0, 0, 0, 0},
  {0.0f, 0.0f, 0.0f, 0, 0, 0, 0},
  {0.0f, 0.0f, 0.0f, 0, 0, 0, 0},
  {0.0f, 0.0f, 0.0f, 0, 0, 0, 0}
};
static const int32_t data_52EC[5] = {0, 0, 0, 0, 0};
static const int32_t data_5300[684] = {
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
static const uint32_t data_5DB0 = 0x41A00000;
static const float data_5DB4[2] = {0.0f, -50.0f};
static const uint32_t data_5DBC = 0x41A00000;
static const float data_5DC0[2] = {0.0f, -50.0f};
static const uint32_t data_5DC8 = 0xC2200000;
static const float data_5DCC[2] = {0.0f, 80.0f};
static const uint32_t data_5DD4 = 0xC2D60000;
static const float data_5DD8[2] = {0.0f, 78.0f};
static const uint8_t data_5DE0[8] = {0};
static const float data_5DE8 = 90.0f;
static const uint32_t data_5DEC = 0xC1A00000;
static const float data_5DF0[2] = {0.0f, -50.0f};
static const uint32_t data_5DF8 = 0xC1A00000;
static const float data_5DFC[2] = {0.0f, -50.0f};
static const uint32_t data_5E04 = 0xC2E60000;
static const float data_5E08[2] = {0.0f, 175.0f};
static const uint32_t data_5E10 = 0xC2DC0000;
static const float data_5E14[2] = {0.0f, 10.0f};
static const uint32_t data_5E1C = 0x42C80000;
static const float data_5E20[2] = {0.0f, -40.0f};
static const uint32_t data_5E28 = 0x43020000;
static const float data_5E2C[2] = {0.0f, 25.0f};
static const uint8_t data_5E34[12] = {0};
static const uint32_t data_5E40 = 0x3F800000;
static const float data_5E44[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint32_t data_5EC8 = 0x3F800000;
static const float data_5ECC[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint8_t data_5F50[16] = {0};
static const float data_5F60[30] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint32_t data_5FD8 = 0x3F800000;
static const float data_5FDC[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint32_t data_6060 = 0x3F800000;
static const float data_6064[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint8_t data_60E8[84] = {0};
typedef struct Row_613C { float w0; float w1; float w2; int32_t w3; } Row_613C;
static const Row_613C data_613C[3] = {
  {0.787f, 0.787f, 0.787f, 0},
  {0.0f, 0.0f, 0.0f, 0},
  {0.0f, 0.0f, 0.0f, 0}
};
static const int32_t data_616C = 0;
static const uint32_t data_6170 = 0x3F800000;
static const float data_6174[33] = {
  0.944f, 0.866f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f,
  0.787f, 0.787f, 0.787f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint32_t data_61F8 = 0x3F4978D5;
static const float data_61FC[33] = {
  0.787f, 0.866f, 0.944f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  0.944f, 0.866f, 0.787f, 0.787f, 0.787f, 0.787f, 0.708f, 0.708f, 0.708f, 0.708f
};
static const uint8_t data_6280[12] = {0};
static const float data_628C[31] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint32_t data_6308 = 0x3F800000;
static const float data_630C[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint32_t data_6390 = 0x3F800000;
static const float data_6394[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint32_t data_6418 = 0x3F800000;
static const float data_641C[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint32_t data_64A0 = 0x41A00000;
static const float data_64A4[2] = {0.0f, -50.0f};
static const uint32_t data_64AC = 0x41A00000;
static const float data_64B0[2] = {0.0f, -50.0f};
static const uint32_t data_64B8 = 0xC2200000;
static const float data_64BC[2] = {0.0f, 80.0f};
static const uint32_t data_64C4 = 0xC1A00000;
static const float data_64C8[2] = {0.0f, -50.0f};
static const uint32_t data_64D0 = 0xC1A00000;
static const float data_64D4[2] = {0.0f, -50.0f};
static const uint32_t data_64DC = 0xC2E60000;
static const float data_64E0[2] = {0.0f, 175.0f};
static const uint32_t data_64E8 = 0x42C80000;
static const float data_64EC[2] = {0.0f, -40.0f};
static const uint8_t data_64F4[12] = {0};
static const uint32_t data_6500 = 0x3F800000;
static const float data_6504[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint32_t data_6588 = 0x3F800000;
static const float data_658C[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint8_t data_6610[16] = {0};
static const float data_6620[30] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint8_t data_6698[84] = {0};
typedef struct Row_66EC { float w0; float w1; float w2; int32_t w3; } Row_66EC;
static const Row_66EC data_66EC[3] = {
  {0.787f, 0.787f, 0.787f, 0},
  {0.0f, 0.0f, 0.0f, 0},
  {0.0f, 0.0f, 0.0f, 0}
};
static const int32_t data_671C = 0;
static const uint32_t data_6720 = 0x3F800000;
static const float data_6724[33] = {
  0.944f, 0.866f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f,
  0.787f, 0.787f, 0.787f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint32_t data_67A8 = 0x3F4978D5;
static const float data_67AC[33] = {
  0.787f, 0.866f, 0.944f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  0.944f, 0.866f, 0.787f, 0.787f, 0.787f, 0.787f, 0.708f, 0.708f, 0.708f, 0.708f
};
static const uint32_t data_6830 = 0x3F800000;
static const float data_6834[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint32_t data_68B8 = 0x3F800000;
static const float data_68BC[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint32_t data_6940 = 0xC2A50000;
static const float data_6944[2] = {0.0f, 68.0f};
static const uint32_t data_694C = 0x3F800000;
static const float data_6950[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint32_t data_69D4 = 0xC1D00000;
static const float data_69D8[2] = {0.0f, 76.5f};
static const uint32_t data_69E0 = 0x3F800000;
static const float data_69E4[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint32_t data_6A68 = 0x41A00000;
static const float data_6A6C[2] = {0.0f, -50.0f};
static const uint32_t data_6A74 = 0x41A00000;
static const float data_6A78[2] = {0.0f, -50.0f};
static const uint32_t data_6A80 = 0xC2200000;
static const float data_6A84[2] = {0.0f, 80.0f};
static const uint32_t data_6A8C = 0xC1A00000;
static const float data_6A90[2] = {0.0f, -50.0f};
static const uint32_t data_6A98 = 0xC1A00000;
static const float data_6A9C[2] = {0.0f, -50.0f};
static const uint32_t data_6AA4 = 0xC2E60000;
static const float data_6AA8[5] = {0.0f, 175.0f, 100.0f, 0.0f, -40.0f};
static const uint8_t data_6ABC[12] = {0};
static const uint32_t data_6AC8 = 0x3F800000;
static const float data_6ACC[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint32_t data_6B50 = 0x3F800000;
static const float data_6B54[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint8_t data_6BD8[16] = {0};
static const float data_6BE8[30] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint8_t data_6C60[84] = {0};
typedef struct Row_6CB4 { float w0; float w1; float w2; int32_t w3; } Row_6CB4;
static const Row_6CB4 data_6CB4[3] = {
  {0.787f, 0.787f, 0.787f, 0},
  {0.0f, 0.0f, 0.0f, 0},
  {0.0f, 0.0f, 0.0f, 0}
};
static const int32_t data_6CE4 = 0;
static const uint32_t data_6CE8 = 0x3F800000;
static const float data_6CEC[33] = {
  0.944f, 0.866f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f,
  0.787f, 0.787f, 0.787f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint32_t data_6D70 = 0x3F4978D5;
static const float data_6D74[33] = {
  0.787f, 0.866f, 0.944f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  0.944f, 0.866f, 0.787f, 0.787f, 0.787f, 0.787f, 0.708f, 0.708f, 0.708f, 0.708f
};
static const uint32_t data_6DF8 = 0x3F800000;
static const float data_6DFC[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const int32_t data_6E80[57] = {
  0, 0, -6189, 0, 1638, -3822, 0, 546, -546, 0, -1820, -12014, 0, 0, -16384, 0, 182, -12743, 0, 0, -1274, 0, 546, -10558, 0, 0, -14017, 0, 0, -16201,
  0, 0, -1092, 0, 728, -9830, 0, 0, -14199, 0, 0, -15291, 0, 0, -1092, 0, -1638, -8009, 0, 0, -12014, 0, -3458, -13653, 0, -364, -910
};
static const int32_t data_6F64[57] = {
  0, 0, -6189, 0, 1638, -3822, 0, 546, -546, 0, -1820, -11832, 0, 0, -16384, 0, 364, -16384, 0, -182, -546, 0, 546, -7827, 0, 0, -13471, 0, 0, -18204,
  0, 0, -182, 0, 728, -8738, 0, 0, -14199, 0, 0, -17112, 0, 0, -546, 0, -364, -8009, 0, 0, -12561, 0, -3458, -15109, 0, -364, -364
};
static const Vector4 data_7048 = {.x = 0.3f, .y = 0.3f, .z = 0.3f, .w = 0.3f};
static const Vector4 data_7058 = {.x = 1.0f, .y = 0.2f, .z = 0.2f, .w = 0.2f};
static const float data_7068[4] = {1.0f, 0.1f, 0.1f, 0.1f};
static const char data_7078[10] __attribute__((scn_string_literal)) = "E1077A001";
static const char data_7082[10] __attribute__((scn_string_literal)) = "E1077A002";
static const char data_708C[10] __attribute__((scn_string_literal)) = "E1077A003";
static const char data_7096[10] __attribute__((scn_string_literal)) = "E1077B001";
static const char data_70A0[10] __attribute__((scn_string_literal)) = "E1077B002";
static const char data_70AA[10] __attribute__((scn_string_literal)) = "E1077B003";
static const char data_70B4[10] __attribute__((scn_string_literal)) = "E1077C001";
static const char data_70BE[10] __attribute__((scn_string_literal)) = "E1077C002";
static const char data_70C8[10] __attribute__((scn_string_literal)) = "E1077D001";
static const char data_70D2[10] __attribute__((scn_string_literal)) = "E1077D002";
static const char data_70DC[10] __attribute__((scn_string_literal)) = "E1077D003";
static const char data_70E6[10] __attribute__((scn_string_literal)) = "E1077E001";
static const char data_70F0[12] __attribute__((scn_string_literal)) = "E1077F001";
static const Vector3 data_70FC = {.x = 1000.0f, .y = 0.0f, .z = 1000.0f};
static const Vector4 data_7108 = {.x = 0.6f, .y = 0.6f, .z = 0.6f, .w = 0.6f};
static const char data_7118[] __attribute__((scn_string_literal)) = "scene/03/TERY";
static const char data_7126[] __attribute__((scn_string_literal)) = "TERYB1";
static const char data_712D[11] __attribute__((scn_string_literal)) = "bgm013.snd";
static const char data_7138[13] __attribute__((scn_string_literal)) = "e1terryt.snd";
static const char data_7145[7] __attribute__((scn_string_literal)) = "TERYB1";
static const char data_714C[] __attribute__((scn_string_literal)) = "scene/03/TERY";
static const char data_715A[] __attribute__((scn_string_literal)) = "TERYB2";
static const uint8_t data_7161[3] = {0};
static const Vector4 data_7164 = {.x = 0.3f, .y = 0.06f, .z = 1.40129846e-45f, .w = 0.3f};
static const char data_7174[8] __attribute__((scn_string_literal)) = "TERYB2";
static const uint32_t data_717C = 0x3F0A3D71;
static const float data_7180[11] = {0.61f, 0.42f, 3.5f, -0.12f, 0.33f, 0.06f, 1.14f, 4.81f, -4.46f, 7.02f, 0.8f};
static const int32_t data_71AC = 3058;
static const uint32_t data_71B0 = 0x3F0A3D71;
static const float data_71B4[11] = {0.61f, 0.42f, 3.5f, -0.22f, 1.0f, -1.0f, 3.07f, 4.24f, -6.9f, 7.58f, 0.98f};
static const int32_t data_71E0 = 4150;
static const uint32_t data_71E4 = 0x3F0A3D71;
static const float data_71E8[11] = {0.61f, 0.42f, 3.5f, -0.64f, 0.63f, 0.43f, 3.18f, 4.83f, -4.27f, 7.17f, 0.8f};
static const int32_t data_7214 = 3058;
static const uint32_t data_7218 = 0x3F0A3D71;
static const float data_721C[11] = {0.61f, 0.42f, 3.5f, 0.38f, 0.77f, 0.39f, 6.17f, 5.29f, -2.9f, 7.02f, 0.8f};
static const int32_t data_7248 = 3058;
static const char data_724C[13] __attribute__((scn_string_literal)) = "battle_1.snd";
static const char data_7259[13] __attribute__((scn_string_literal)) = "battle_1.snd";
static const char data_7266[] __attribute__((scn_string_literal)) = "scene/03/TERY";
static const char data_7274[] __attribute__((scn_string_literal)) = "BIKE";
static const char data_7279[6] __attribute__((scn_string_literal)) = "MPK00";
static const char data_727F[6] __attribute__((scn_string_literal)) = "MPK00";
static const char data_7285[6] __attribute__((scn_string_literal)) = "E1077";
static const char data_728B[11] __attribute__((scn_string_literal)) = "M_FGT1.BIN";
static const char data_7296[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_729B[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_72A0[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_72A5[13] __attribute__((scn_string_literal)) = "e1terryt.snd";
static const char data_72B2[5] __attribute__((scn_string_literal)) = "BIKE";
static const char data_72B7[6] __attribute__((scn_string_literal)) = "E1077";
static const char data_72BD[5] __attribute__((scn_string_literal)) = "BIKE";
static const char data_72C2[5] __attribute__((scn_string_literal)) = "BIKE";
static const char data_72C7[13] __attribute__((scn_string_literal)) = "seqdata2.bin";
static const char data_72D4[5] __attribute__((scn_string_literal)) = "BIKE";
static const char data_72D9[13] __attribute__((scn_string_literal)) = "seqdata3.bin";
static const char data_72E6[5] __attribute__((scn_string_literal)) = "BIKE";
static const char data_72EB[13] __attribute__((scn_string_literal)) = "seqdata4.bin";
static const char data_72F8[5] __attribute__((scn_string_literal)) = "BIKE";
static const char data_72FD[13] __attribute__((scn_string_literal)) = "seqdata5.bin";
static const char data_730A[5] __attribute__((scn_string_literal)) = "BIKE";
static const char data_730F[13] __attribute__((scn_string_literal)) = "seqdata6.bin";
static const char data_731C[5] __attribute__((scn_string_literal)) = "BIKE";
static const char data_7321[13] __attribute__((scn_string_literal)) = "seqdata7.bin";
static const char data_732E[5] __attribute__((scn_string_literal)) = "BIKE";
static const char data_7333[13] __attribute__((scn_string_literal)) = "seqdata8.bin";
static const char data_7340[5] __attribute__((scn_string_literal)) = "BIKE";
static const char data_7345[13] __attribute__((scn_string_literal)) = "seqdata9.bin";
static const char data_7352[5] __attribute__((scn_string_literal)) = "BIKE";
static const char data_7357[13] __attribute__((scn_string_literal)) = "seqdataA.bin";
static const char data_7364[5] __attribute__((scn_string_literal)) = "BIKE";
static const char data_7369[13] __attribute__((scn_string_literal)) = "seqdataB.bin";
static const char data_7376[5] __attribute__((scn_string_literal)) = "BIKE";
static const char data_737B[6] __attribute__((scn_string_literal)) = "YKG_M";
static const char data_7381[7] __attribute__((scn_string_literal)) = "YKB_M";
static const uint32_t data_7388[4] = {0, 0, 0x64996FA5U, 0x64996FA5U};
static const FogColourData data_7398 = {.red = 0.1f, .green = 0.0f, .blue = 0.25f, .colour = 0xFF392C63U};
static const char data_73A8[] __attribute__((scn_string_literal)) = "scene/03/TERY";
static const char data_73B6[] __attribute__((scn_string_literal)) = "TER2";
static const char data_73BB[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_73C0[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_73C5[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_73CA[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_73CF[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_73D4[11] __attribute__((scn_string_literal)) = "bgm084.snd";
static const char data_73DF[13] __attribute__((scn_string_literal)) = "a1_terry.snd";
static const char data_73EC[6] __attribute__((scn_string_literal)) = "01TAP";
static const char data_73F2[] __attribute__((scn_string_literal)) = "sprite";
static const char data_73F9[13] __attribute__((scn_string_literal)) = "SCROLL25.SPR";
static const char data_7406[5] __attribute__((scn_string_literal)) = "TER2";
static const char data_740B[9] __attribute__((scn_string_literal)) = "seqdataC";
static const char data_7414[5] __attribute__((scn_string_literal)) = "TER2";
static const char data_7419[9] __attribute__((scn_string_literal)) = "seqdataD";
static const char data_7422[5] __attribute__((scn_string_literal)) = "TER2";
static const char data_7427[9] __attribute__((scn_string_literal)) = "seqdataE";
static const char data_7430[5] __attribute__((scn_string_literal)) = "TER2";
static const char data_7435[5] __attribute__((scn_string_literal)) = "TER2";
static const char data_743A[6] __attribute__((scn_string_literal)) = "01TAP";
static const char data_7440[28] __attribute__((scn_string_literal)) = " Load PackFile Start %s:%s\012";
static const char data_745C[28] __attribute__((scn_string_literal)) = "Load PackFile Failed %s:%s\012";
