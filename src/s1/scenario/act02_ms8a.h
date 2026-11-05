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
static void script_0207(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_0241(int32_t arg0, int32_t arg1);
static void script_0275(void);
static void script_036D(int32_t arg0, int32_t arg1);
static int32_t script_039D(int32_t arg0, int32_t arg1, int32_t arg2);
static int32_t script_03D7(int32_t arg0, int32_t arg1);
static void script_040B(int32_t arg0, int32_t arg1);
static void script_066D(int32_t arg0, int32_t arg1);
static void script_087C(int32_t arg0, int32_t arg1);
static void script_0C86(int32_t arg0, int32_t arg1);
static void script_0F62(int32_t arg0, int32_t arg1);
static void script_11D4(int32_t arg0, int32_t arg1);
static void script_163A(int32_t arg0, int32_t arg1);
static void script_1897(int32_t arg0, int32_t arg1);
static int32_t script_1B31(void);
static int32_t script_1B7C(void);
static void script_1BDD(int32_t arg0);
static void script_1C18(int32_t arg0);
static void script_1C42(void);
static void script_1E18(void);
static void script_1F75(const char *arg0, const char *arg1);
void script_1FC9(void);
static int32_t script_1FD7(void);
static void script_23ED(void);
static int32_t script_240F(void);

static int32_t task_0;
static int8_t task_4;
static uint8_t task_5;
static uint16_t task_6;
typedef struct Row_242C { float w0; float w1; float w2; int32_t w3; int32_t w4; int32_t w5; int32_t w6; } Row_242C;
static const Row_242C data_242C[9] = {
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
static const int32_t data_2528[5] = {0, 0, 0, 0, 0};
static const int32_t data_253C[285] = {
  0, 0, -10922, 0, 0, -10558, 0, 1456, 0, 0, 0, -18022, 0, 0, -17112, 0, 0, -18386, 0, 0, 0, 0, 0, -12196, 0, 0, -18204, 0, 0, -16748, 0, 0, 0, 0, 0,
  -11832, 0, 0, -18386, 0, 0, -15109, 0, 0, 0, 0, 910, -12014, 0, -2184, -16748, 0, -2184, -10012, 0, 0, 0, 0, -364, -11650, -2912, -364, -4369, 1638,
  910, 182, 0, 0, -16019, 0, 182, -23119, 0, 546, -18932, 0, 0, -1092, 0, 0, -16384, 0, 0, -21845, 0, 0, -18750, 0, 0, -546, 0, 0, -21481, 0, 0,
  -21299, 0, 0, -18568, 0, 0, -728, 0, 910, -18750, 0, 182, -18750, 0, -4004, -18204, 0, 0, -728, 0, -364, -14017, -2912, -364, -6735, 1638, 910,
  2548, 0, 0, -18568, 0, 182, -25668, 0, 546, -21481, 0, 0, -3640, 0, 0, -18932, 0, 0, -24393, 0, 0, -21299, 0, 0, -3094, 0, 0, -24029, 0, 0, -23847,
  0, 0, -21117, 0, 0, -3276, 0, 910, -21299, 0, 182, -21299, 0, -4004, -20753, 0, 0, -3276, 0, 0, -5461, 0, -2366, -1092, 0, -182, 1092, 0, -546,
  -6735, 0, 0, -4004, 0, 0, -2912, 0, -182, 1274, 0, 0, -13289, 0, 0, -5279, 0, 0, -8009, 0, 0, 1456, 0, 182, -8738, 0, 0, -6007, 0, 0, -8738, 0, 182,
  1638, 0, -728, -5279, 0, -1274, -6735, 0, -910, -8738, 0, 0, 1638, 0, 0, -5461, 0, -2366, -1092, 0, -182, 1092, 0, -546, -13289, 0, 0, -5643, 0, 0,
  -8009, 0, -182, 1274, 0, 0, -11832, 0, 0, -5279, 0, 0, -8009, 0, 0, 1456, 0, 182, -8738, 0, 0, -6007, 0, 0, -8738, 0, 182, 1638, 0, -728, -5279, 0,
  -1274, -6735, 0, -910, -8738, 0, 0, 1092
};
static const int32_t data_29B0[57] = {
  0, 0, -5461, 0, -2366, -1092, 0, -182, 1092, 0, -546, -3640, 0, 0, -2366, 0, 0, -1456, 0, -182, 1274, 0, 0, -3822, 0, 0, -2730, 0, 0, -1456, 0, 0,
  1456, 0, 182, -3822, 0, 0, -2912, 0, 0, -1638, 0, 182, 1638, 0, -728, -3640, 0, -910, -2730, 0, -546, -1638, 0, 0, 1092
};
static const int32_t data_2A94[57] = {
  0, 0, -5461, 0, -2366, -1092, 0, -182, 1092, 0, -546, -6735, 0, 0, -4004, 0, 0, -6553, 0, -182, 1274, 0, 0, -13289, 0, 0, -5279, 0, 0, -8009, 0, 0,
  1456, 0, 182, -8738, 0, 0, -6007, 0, 0, -8738, 0, 182, 1638, 0, -728, -5279, 0, -1274, -6735, 0, -910, -8738, 0, 0, 1638
};
static const int32_t data_2B78[228] = {
  0, 0, -9102, 0, -2366, -2002, 0, -182, 910, 0, -546, -2366, 0, 0, -2002, 0, 0, -2912, 0, -182, 1274, 0, 0, -14199, 0, 0, -11468, 0, 0, -13653, 0, 0,
  1456, 0, 182, -11286, 0, 0, -12014, 0, 0, -12743, 0, 182, 1638, 0, -728, -10194, 0, -1274, -12196, 0, -910, -10922, -728, 546, 1638, 0, 0, -1820, 0,
  -2366, -728, 0, -364, -1092, 0, -546, -11832, 0, 910, -6735, 0, 0, -6553, 0, -182, 1274, 0, 0, -13289, 0, 0, -6007, 0, 0, -10012, 0, 0, 1092, 0,
  182, -8738, 0, 0, -6735, 0, 0, -9830, 0, 182, 1638, 0, -728, -6553, 0, -1274, -7281, 0, -910, -10740, 0, 0, 1638, 0, 0, -5461, -2912, -2366, -364,
  -5279, 2548, -910, 0, -546, -8374, 0, 0, -9466, 0, 910, -4915, 0, -182, 546, 0, 0, -13289, 0, 0, -5279, 0, 364, -7645, 0, -182, 1456, 0, 182, -8738,
  0, 0, -7281, 0, 182, -6917, 0, -182, 1638, 0, -728, -5279, 0, -1274, -6735, 0, -182, -8009, 0, 0, 1638, -4733, 0, -8556, 0, -2366, -1092, -6189,
  2912, 0, 0, -546, -10922, 0, 1820, -16384, 0, 0, -11468, 0, 182, 546, 0, 0, -10376, 0, 0, -15837, 0, 0, -12743, 0, -182, 728, 0, 182, -13835, 0, 0,
  -16201, 0, 364, -10922, 0, -182, 728, 0, -728, -9648, 0, -3458, -12379, 0, -728, -7463, 0, 0, 1092
};
static const int32_t data_2F08[57] = {
  0, 0, -5461, 0, -2366, -1638, 0, -910, 1092, 0, -546, -3640, 0, 0, -2366, 0, 0, -1456, 0, -182, 1638, 0, 0, -910, 0, 0, -2730, 0, 0, -1274, 0, 0,
  1820, 0, 182, -2730, 0, 0, -1820, 0, 0, -1274, 0, 182, 2184, 0, -728, -3094, 0, -910, -2548, 0, 910, -1456, -546, 910, 2366
};
static const int32_t data_2FEC[57] = {
  0, 0, -5461, 0, -2366, -1092, 0, 728, 0, 0, -546, -3640, 0, 0, -2366, 0, 0, -1456, 0, -182, 1274, 0, 0, -3822, 0, 0, -2730, 0, 0, -1456, 0, 0, 1456,
  0, 182, -3822, 0, 0, -2912, 0, 0, -1638, 0, 182, 1638, 0, -728, -3640, 0, -910, -2730, 0, -546, -1638, 0, 0, 1092
};
static const char data_30D0[2] __attribute__((scn_string_literal)) = "\012";
static const char data_30D2[11] __attribute__((scn_string_literal)) = "tsm001.snd";
static const char data_30DD[11] __attribute__((scn_string_literal)) = "tsm005.snd";
static const Vector4 data_30E8 = {.x = 1.0f, .y = 0.2f, .z = 0.2f, .w = 0.2f};
static const char data_30F8[33] __attribute__((scn_string_literal)) = "MS8A: Load PackFile Start %s:%s\012";
static const char data_3119[34] __attribute__((scn_string_literal)) = "MS8A: Load PackFile Failed %s:%s\012";
static const char data_313B[] __attribute__((scn_string_literal)) = "scene/02/MS8A";
static const char data_3149[] __attribute__((scn_string_literal)) = "MS8A";
static const char data_314E[6] __attribute__((scn_string_literal)) = "A0159";
static const char data_3154[5] __attribute__((scn_string_literal)) = "MS8A";
static const char data_3159[1] __attribute__((scn_string_literal)) = "";
static const char data_315A[5] __attribute__((scn_string_literal)) = "MS8A";
static const char data_315F[1] __attribute__((scn_string_literal)) = "";
static const char data_3160[5] __attribute__((scn_string_literal)) = "MS8A";
static const char data_3165[1] __attribute__((scn_string_literal)) = "";
static const char data_3166[5] __attribute__((scn_string_literal)) = "MS8A";
static const char data_316B[1] __attribute__((scn_string_literal)) = "";
static const char data_316C[5] __attribute__((scn_string_literal)) = "MS8A";
static const char data_3171[1] __attribute__((scn_string_literal)) = "";
static const char data_3172[5] __attribute__((scn_string_literal)) = "MS8A";
static const char data_3177[1] __attribute__((scn_string_literal)) = "";
static const char data_3178[5] __attribute__((scn_string_literal)) = "MS8A";
static const char data_317D[1] __attribute__((scn_string_literal)) = "";
static const char data_317E[5] __attribute__((scn_string_literal)) = "MS8A";
static const char data_3183[1] __attribute__((scn_string_literal)) = "";
static const char data_3184[5] __attribute__((scn_string_literal)) = "MS8A";
static const char data_3189[23] = "A0159\000\000\000\000\200?\315\314L>\315\314L>\315\314L>";
