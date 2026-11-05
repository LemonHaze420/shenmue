#pragma once

#include "scenario.h"
#include "motion/motion.h"

static int32_t script_0030(void);
static void script_0060(void);
static void script_0086(void);
static void script_0097(int32_t arg0);
static void script_0131(int32_t arg0);
static int32_t script_0181(void);
static void script_01AF(int32_t arg0);
static void script_01DD(void);
static void script_0211(int32_t arg0);
static void script_0245(int32_t arg0);
static void script_0263(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_029D(int32_t arg0, int32_t arg1);
static void script_02D1(void);
static void script_03C9(int32_t arg0, int32_t arg1);
static void script_03F9(int32_t arg0);
static int32_t script_0418(int32_t arg0);
static int8_t script_052C(void);
static void script_05B9(int32_t arg0, int32_t arg1);
static void script_05CA(void);
static void script_05F0(void);
static void script_05FA(void);
static void script_0898(int32_t arg0);
static void script_1067(void);
static void script_109D(void);
static int32_t script_10B8(void);
void script_10F1(void);
static void script_113A(void);
static void script_118B(int32_t arg0);
static void script_119A(void);
static int32_t script_123B(void);

static int8_t task_0;
static uint8_t task_1;
static uint16_t task_2;
static int8_t task_4;
static uint8_t task_5;
static uint16_t task_6;
static int32_t task_8;
static int8_t task_C;
static uint8_t task_D;
static uint16_t task_E;
static int8_t task_10;
static uint8_t task_11;
static uint16_t task_12;
static int32_t task_14;
static int32_t task_18;
static int32_t task_1C;
static Vector3 task_20;
static Vector3 task_2C;
static Vector3 task_38;
static int32_t task_44;
static int32_t task_48;
static int32_t task_4C;
static const float data_1254[3] = {1.0f, 1.0f, 1.0f};
static const int32_t data_1260[4] = {16777215, 16777215, 16777215, 16777215};
static const uint32_t data_1270 = 0xC3D48000;
static const float data_1274[2] = {-1.8f, -210.0f};
static const char data_127C[] __attribute__((scn_string_literal)) = "scene/02/model";
static const char data_128B[] __attribute__((scn_string_literal)) = "C1305S3G.MT5";
static const char data_1298[] __attribute__((scn_string_literal)) = "C1305S1G.MT5";
static const uint8_t pad_12A5[5] = {0};
static const uint8_t data_12AA[16] = {
  0xA0, 0x41, 0x00, 0x00, 0x48, 0x44, 0x00, 0x00, 0x80, 0x3F, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00
};
static const uint8_t pad_12BA[234] = {0};
static const int32_t data_13A4[682] = {
  -10922, 0, 0, -10558, 0, 1456, 0, 0, 0, -18022, 0, 0, -17112, 0, 0, -18386, 0, 0, 0, 0, 0, -12196, 0, 0, -18204, 0, 0, -16748, 0, 0, 0, 0, 0,
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
static const char data_1E4C[12] __attribute__((scn_string_literal)) = "M_FACE4.BIN";
static const char data_1E58[6] __attribute__((scn_string_literal)) = "FACE4";
static const char data_1E5E[13] __attribute__((scn_string_literal)) = "SEQDATA1.BIN";
static const char data_1E6B[17] __attribute__((scn_string_literal)) = "SCENE/80/HGSF";
