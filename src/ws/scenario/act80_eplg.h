#pragma once

#include "scenario.h"
#include "motion/motion.h"

static void script_0030(void);
static void script_0041(int32_t arg0);
static void script_00DB(int32_t arg0);
static void script_012B(void);
static int32_t script_0159(int32_t arg0);
static void script_0187(void);
static void script_01BB(int32_t arg0);
static void script_01EF(int32_t arg0);
static void script_020D(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_0247(int32_t arg0, int32_t arg1);
static void script_027B(void);
static int32_t script_0373(int32_t arg0, int32_t arg1);
static void script_03A3(int32_t arg0);
static void script_0435(void);
static void script_046A(void);
static void script_04F6(void);
static void script_0661(int32_t arg0, int32_t arg1);
static void script_0748(int32_t arg0, float arg1, float arg2, float arg3);
static void script_0770(int32_t arg0, float arg1, float arg2, float arg3);
static void script_07C4(int32_t arg0, int32_t arg1, int32_t arg2);
static int32_t script_0814(int32_t arg0);
static void script_08BB(int32_t arg0, int32_t arg1);
static int32_t script_08DE(int32_t arg0);
static void script_0911(int32_t arg0, int32_t arg1);
static void script_0934(int32_t arg0, int32_t arg1);
static void script_0993(int32_t arg0);
static int32_t script_09A8(void);
static void script_09C1(int32_t arg0);
static void script_09F0(int32_t arg0);
static void script_0A29(void);
static void script_0AF4(void);
static int32_t script_0B71(void);
static void script_0D9E(int32_t arg0, int32_t arg1);
static void script_1061(void);
static void script_109D(void);
static void script_10CC(int32_t arg0, int32_t arg1);
static int32_t script_1105(int32_t arg0, int32_t arg1);
static void script_11AE(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_14B3(void);
void script_16B9(void);
static int32_t script_18BE(void);
static int32_t script_199D(int32_t arg0);
static void script_1AF9(void);
static void script_1BCF(void);
static int32_t script_1CB4(void);
static void script_1CD5(void);
static void script_1CEE(void);
static int32_t script_1D12(int32_t arg0);
static void script_1D82(int32_t arg0, int32_t arg1);

static int8_t task_0;
static int8_t task_1;
static int8_t task_2;
static int8_t task_3;
static int32_t task_4;
static int32_t task_8;
static int32_t task_C;
static int32_t task_10;
typedef struct Row_1D9C { float w0; float w1; float w2; int32_t w3; int32_t w4; int32_t w5; int32_t w6; } Row_1D9C;
static const Row_1D9C data_1D9C[9] = {
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
static const int32_t data_1E98[5] = {0, 0, 0, 0, 0};
static const int32_t data_1EAC[57] = {
  0, 0, -10922, 0, 0, -10558, 0, 1456, 0, 0, 0, -18022, 0, 0, -17112, 0, 0, -18386, 0, 0, 0, 0, 0, -12196, 0, 0, -18204, 0, 0, -16748, 0, 0, 0, 0, 0,
  -11832, 0, 0, -18386, 0, 0, -15109, 0, 0, 0, 0, 910, -12014, 0, -2184, -16748, 0, -2184, -10012, 0, 0, 0
};
static const int32_t data_1F90[171] = {
  0, -364, -11650, -2912, -364, -4369, 1638, 910, 182, 0, 0, -16019, 0, 182, -23119, 0, 546, -18932, 0, 0, -1092, 0, 0, -16384, 0, 0, -21845, 0, 0,
  -18750, 0, 0, -546, 0, 0, -21481, 0, 0, -21299, 0, 0, -18568, 0, 0, -728, 0, 910, -18750, 0, 182, -18750, 0, -4004, -18204, 0, 0, -728, 0, -364,
  -14017, -2912, -364, -6735, 1638, 910, 2548, 0, 0, -18568, 0, 182, -25668, 0, 546, -21481, 0, 0, -3640, 0, 0, -18932, 0, 0, -24393, 0, 0, -21299, 0,
  0, -3094, 0, 0, -24029, 0, 0, -23847, 0, 0, -21117, 0, 0, -3276, 0, 910, -21299, 0, 182, -21299, 0, -4004, -20753, 0, 0, -3276, 0, 0, -5461, 0,
  -2366, -1092, 0, -182, 1092, 0, -546, -6735, 0, 0, -4004, 0, 0, -2912, 0, -182, 1274, 0, 0, -13289, 0, 0, -5279, 0, 0, -8009, 0, 0, 1456, 0, 182,
  -8738, 0, 0, -6007, 0, 0, -8738, 0, 182, 1638, 0, -728, -5279, 0, -1274, -6735, 0, -910, -8738, 0, 0, 1638
};
static const int32_t data_223C[57] = {
  0, 0, -5461, 0, -2366, -1092, 0, -182, 1092, 0, -546, -13289, 0, 0, -5643, 0, 0, -8009, 0, -182, 1274, 0, 0, -11832, 0, 0, -5279, 0, 0, -8009, 0, 0,
  1456, 0, 182, -8738, 0, 0, -6007, 0, 0, -8738, 0, 182, 1638, 0, -728, -5279, 0, -1274, -6735, 0, -910, -8738, 0, 0, 1092
};
static const int32_t data_2320[57] = {
  0, 0, -5461, 0, -2366, -1092, 0, -182, 1092, 0, -546, -3640, 0, 0, -2366, 0, 0, -1456, 0, -182, 1274, 0, 0, -3822, 0, 0, -2730, 0, 0, -1456, 0, 0,
  1456, 0, 182, -3822, 0, 0, -2912, 0, 0, -1638, 0, 182, 1638, 0, -728, -3640, 0, -910, -2730, 0, -546, -1638, 0, 0, 1092
};
static const int32_t data_2404[342] = {
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
static const char data_295C[11] __attribute__((scn_string_literal)) = "TS1003A001";
static const char data_2967[11] __attribute__((scn_string_literal)) = "TS1003A002";
static const char data_2972[11] __attribute__((scn_string_literal)) = "TS1003A003";
static const char data_297D[7] __attribute__((scn_string_literal)) = "save!\012";
static const int32_t data_2984[57] = {
  0, 0, -3640, 0, 0, -2366, -4369, 1820, -546, 0, 0, -5279, 0, 0, -6007, 0, 0, -2912, 0, 0, -910, 0, 0, -6553, 0, 0, -6735, 0, 0, -3640, 0, 0, -182,
  0, 0, -5461, 0, 0, -6007, 0, 0, -2366, 0, 0, 0, 0, 0, -5643, 0, 0, -4733, 0, 0, -2366, -1456, 0, 728
};
static const Vector4 data_2A68 = {.x = 0.3f, .y = 0.3f, .z = 0.3f, .w = 0.3f};
static const int32_t data_2A78[57] = {
  0, 0, -5461, 0, -2366, -1638, 0, -182, 1456, 0, -546, -4915, 0, 0, -3640, 0, 1092, -1456, 0, 0, 1638, 0, 0, -5097, 0, 0, -2730, 0, -364, -1638, 0,
  0, 2366, 0, 182, -2912, 0, 0, -2912, 0, -546, -2002, 0, -182, 2184, 0, -728, -5097, 0, -2548, -3458, 0, -546, -2366, -546, 182, 2366
};
static const int32_t data_2B5C[57] = {
  0, 0, -5461, 0, -2366, -1638, 0, -2366, 1456, 0, -546, -4915, 0, 0, -3094, 0, 1092, -728, 0, 0, 1638, 0, 0, -4369, 0, 0, -2730, 0, -364, -1638, 0,
  0, 2366, 0, 182, -2184, 0, 0, -4369, 0, -546, -546, 0, -182, 2184, 0, -728, -5097, 0, -2548, -2002, 0, -546, -2366, -546, 182, 2366
};
static const Vector3 data_2C40 = {.x = -0.01f, .y = 0.19f, .z = -0.51f};
static const Vector3 data_2C4C = {.x = -0.3f, .y = 0.31f, .z = -1.0f};
static const Vector4 data_2C58 = {.x = 1.0f, .y = 0.5f, .z = 0.5f, .w = 0.5f};
static const Vector4 data_2C68 = {.x = 1.0f, .y = 1.0f, .z = 0.82f, .w = 0.73f};
static const FogColourData data_2C78 = {.red = 0.1f, .green = 50.0f, .blue = 1.2f, .colour = 0xFFFFFFFFU};
static const char data_2C88[9] __attribute__((scn_string_literal)) = "ALLVOICE";
static const char data_2C91[12] __attribute__((scn_string_literal)) = "M_01YUK.BIN";
static const char data_2C9D[13] __attribute__((scn_string_literal)) = "a1_ytai2.snd";
static const char data_2CAA[13] __attribute__((scn_string_literal)) = "bgmtk003.snd";
static const char data_2CB7[14] __attribute__((scn_string_literal)) = "scene/80/TIKN";
static const char data_2CC5[] __attribute__((scn_string_literal)) = "SCRTEST.BIN";
static const char data_2CD1[15] __attribute__((scn_string_literal)) = "/scene/80/EPLG";
static const char data_2CE0[7] __attribute__((scn_string_literal)) = "MODELS";
static const char data_2CE7[13] __attribute__((scn_string_literal)) = "bgmtk003.snd";
static const char data_2CF4[14] __attribute__((scn_string_literal)) = "scene/80/TIKN";
static const char data_2D02[] __attribute__((scn_string_literal)) = "SCRTEST.BIN";
static const char data_2D0E[6] __attribute__((scn_string_literal)) = "KMA_L";
static const char data_2D14[5] __attribute__((scn_string_literal)) = "DOOR";
static const char data_2D19[5] __attribute__((scn_string_literal)) = "SLTM";
static const char data_2D1E[5] __attribute__((scn_string_literal)) = "DRAM";
static const char data_2D23[9] __attribute__((scn_string_literal)) = "DATK30NG";
static const Vector4 data_2D2C = {.x = 1.0f, .y = 0.2f, .z = 0.2f, .w = 0.2f};
static const FogColourData data_2D3C = {.red = 5.0f, .green = 100.0f, .blue = 0.48f, .colour = 0xFF17191BU};
static const char data_2D4C[16] __attribute__((scn_string_literal)) = "読み込み失敗\012";
