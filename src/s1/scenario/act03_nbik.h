#pragma once

#include "scenario.h"
#include "motion/motion.h"

static void script_0030(void);
static void script_0039(void);
static void script_0048(int32_t arg0);
static void script_00D5(int32_t arg0);
static void script_0125(void);
static void script_0153(int32_t arg0);
static void script_0181(void);
static void script_01B5(int32_t arg0);
static void script_01E9(int32_t arg0);
static void script_0207(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_0241(int32_t arg0, int32_t arg1);
static void script_0275(void);
static void script_036D(int32_t arg0, int32_t arg1);
static int32_t script_039D(void);
static int32_t script_03B8(void);
static int32_t script_03CD(void);
static int32_t script_03E3(void);
static int32_t script_03F8(void);
static int32_t script_040D(void);
static int32_t script_0422(void);
static int32_t script_0438(void);
static int32_t script_044D(void);
static int32_t script_0462(void);
static int32_t script_0477(void);
static int32_t script_048C(void);
static void script_049B(void);
static void script_0623(void);
static void script_0E36(void *arg0, void *arg1);
static int32_t script_0EBA(void);
static int32_t script_1029(void);
static void script_1201(void);
static void script_1382(void);
static void script_142C(int32_t arg0);
static void script_14C8(int32_t arg0);
static void script_14F1(int32_t arg0);
static void script_1514(int32_t arg0);
static int32_t script_155A(void);
static int32_t script_15B0(void);
void script_15E4(void);
static int32_t script_15F7(int32_t arg0);

static int32_t task_0;
static const float data_17A0[3] = {1.0f, 1.0f, 1.0f};
static const int32_t data_17AC[4] = {16777215, 16777215, 16777215, 16777215};
static const float data_17BC[3] = {20.0f, 800.0f, 1.0f};
static const int32_t data_17C8[229] = {
  -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -10922, 0, 0, -10558, 0, 1456, 0, 0, 0, -18022, 0, 0, -17112, 0, 0, -18386, 0, 0, 0, 0, 0, -12196, 0, 0, -18204, 0,
  0, -16748, 0, 0, 0, 0, 0, -11832, 0, 0, -18386, 0, 0, -15109, 0, 0, 0, 0, 910, -12014, 0, -2184, -16748, 0, -2184, -10012, 0, 0, 0, 0, -364, -11650,
  -2912, -364, -4369, 1638, 910, 182, 0, 0, -16019, 0, 182, -23119, 0, 546, -18932, 0, 0, -1092, 0, 0, -16384, 0, 0, -21845, 0, 0, -18750, 0, 0, -546,
  0, 0, -21481, 0, 0, -21299, 0, 0, -18568, 0, 0, -728, 0, 910, -18750, 0, 182, -18750, 0, -4004, -18204, 0, 0, -728, 0, -364, -14017, -2912, -364,
  -6735, 1638, 910, 2548, 0, 0, -18568, 0, 182, -25668, 0, 546, -21481, 0, 0, -3640, 0, 0, -18932, 0, 0, -24393, 0, 0, -21299, 0, 0, -3094, 0, 0,
  -24029, 0, 0, -23847, 0, 0, -21117, 0, 0, -3276, 0, 910, -21299, 0, 182, -21299, 0, -4004, -20753, 0, 0, -3276
};
static const int32_t data_1B5C[513] = {
  0, 0, -5461, 0, -2366, -1092, 0, -182, 1092, 0, -546, -6735, 0, 0, -4004, 0, 0, -2912, 0, -182, 1274, 0, 0, -13289, 0, 0, -5279, 0, 0, -8009, 0, 0,
  1456, 0, 182, -8738, 0, 0, -6007, 0, 0, -8738, 0, 182, 1638, 0, -728, -5279, 0, -1274, -6735, 0, -910, -8738, 0, 0, 1638, 0, 0, -5461, 0, -2366,
  -1092, 0, -182, 1092, 0, -546, -13289, 0, 0, -5643, 0, 0, -8009, 0, -182, 1274, 0, 0, -11832, 0, 0, -5279, 0, 0, -8009, 0, 0, 1456, 0, 182, -8738,
  0, 0, -6007, 0, 0, -8738, 0, 182, 1638, 0, -728, -5279, 0, -1274, -6735, 0, -910, -8738, 0, 0, 1092, 0, 0, -5461, 0, -2366, -1092, 0, -182, 1092, 0,
  -546, -3640, 0, 0, -2366, 0, 0, -1456, 0, -182, 1274, 0, 0, -3822, 0, 0, -2730, 0, 0, -1456, 0, 0, 1456, 0, 182, -3822, 0, 0, -2912, 0, 0, -1638, 0,
  182, 1638, 0, -728, -3640, 0, -910, -2730, 0, -546, -1638, 0, 0, 1092, 0, 0, -5461, 0, -2366, -1092, 0, -182, 1092, 0, -546, -6735, 0, 0, -4004, 0,
  0, -6553, 0, -182, 1274, 0, 0, -13289, 0, 0, -5279, 0, 0, -8009, 0, 0, 1456, 0, 182, -8738, 0, 0, -6007, 0, 0, -8738, 0, 182, 1638, 0, -728, -5279,
  0, -1274, -6735, 0, -910, -8738, 0, 0, 1638, 0, 0, -9102, 0, -2366, -2002, 0, -182, 910, 0, -546, -2366, 0, 0, -2002, 0, 0, -2912, 0, -182, 1274, 0,
  0, -14199, 0, 0, -11468, 0, 0, -13653, 0, 0, 1456, 0, 182, -11286, 0, 0, -12014, 0, 0, -12743, 0, 182, 1638, 0, -728, -10194, 0, -1274, -12196, 0,
  -910, -10922, -728, 546, 1638, 0, 0, -1820, 0, -2366, -728, 0, -364, -1092, 0, -546, -11832, 0, 910, -6735, 0, 0, -6553, 0, -182, 1274, 0, 0,
  -13289, 0, 0, -6007, 0, 0, -10012, 0, 0, 1092, 0, 182, -8738, 0, 0, -6735, 0, 0, -9830, 0, 182, 1638, 0, -728, -6553, 0, -1274, -7281, 0, -910,
  -10740, 0, 0, 1638, 0, 0, -5461, -2912, -2366, -364, -5279, 2548, -910, 0, -546, -8374, 0, 0, -9466, 0, 910, -4915, 0, -182, 546, 0, 0, -13289, 0,
  0, -5279, 0, 364, -7645, 0, -182, 1456, 0, 182, -8738, 0, 0, -7281, 0, 182, -6917, 0, -182, 1638, 0, -728, -5279, 0, -1274, -6735, 0, -182, -8009,
  0, 0, 1638, -4733, 0, -8556, 0, -2366, -1092, -6189, 2912, 0, 0, -546, -10922, 0, 1820, -16384, 0, 0, -11468, 0, 182, 546, 0, 0, -10376, 0, 0,
  -15837, 0, 0, -12743, 0, -182, 728, 0, 182, -13835, 0, 0, -16201, 0, 364, -10922, 0, -182, 728, 0, -728, -9648, 0, -3458, -12379, 0, -728, -7463, 0,
  0, 1092, 0, 0, -5461, 0, -2366, -1638, 0, -910, 1092, 0, -546, -3640, 0, 0, -2366, 0, 0, -1456, 0, -182, 1638, 0, 0, -910, 0, 0, -2730, 0, 0, -1274,
  0, 0, 1820, 0, 182, -2730, 0, 0, -1820, 0, 0, -1274, 0, 182, 2184, 0, -728, -3094, 0, -910, -2548, 0, 910, -1456, -546, 910, 2366
};
static const int32_t data_2360[57] = {
  0, 0, -6189, 0, 3640, -4187, -2002, 546, 546, 0, -1820, -12014, 0, 546, -17840, 0, 182, -12743, 0, 0, -1274, 0, 546, -11286, 0, 0, -14017, 0, 0,
  -15837, 0, 0, -1092, 0, 728, -9830, 0, 0, -14199, 0, 0, -14563, 0, 0, -1092, 0, -1638, -8009, 0, 0, -12014, -182, -3458, -14563, 0, -728, -1274
};
static const int32_t data_2444[57] = {
  0, 0, -8920, 0, 5643, -6371, 0, 1274, 1456, 0, -1820, -11832, 0, 0, -16384, 0, 364, -14199, 0, -182, -364, 0, 546, -7827, 0, 0, -13471, 0, 0,
  -18204, 0, 0, -182, 0, 728, -8738, 0, 0, -14199, 0, 0, -17112, 0, 0, -546, 0, -364, -8009, 0, 0, -12561, 0, -3458, -15109, 0, -364, -364
};
static const char data_2528[67] __attribute__((scn_string_literal)) = "●青Like a dream I once saw,＆it passes by as it touches my cheek.";
static const char data_256B[64] __attribute__((scn_string_literal)) = "●青This street leading to the ocean＆carries the smell of sea.";
static const char data_25AB[43] __attribute__((scn_string_literal)) = "●青Before love makes me tell a white lie.";
static const char data_25D6[64] __attribute__((scn_string_literal)) = "●青This loneliness...＆I want you to hold it in your arms now.";
static const char data_2616[38] __attribute__((scn_string_literal)) = "●青If you will always be by my side,";
static const char data_263C[47] __attribute__((scn_string_literal)) = "●青there'll be no sorrow or tears in my eyes.";
static const char data_266B[57] __attribute__((scn_string_literal)) = "●青Can't live without you.＆Why did you have to decide?";
static const char data_26A4[45] __attribute__((scn_string_literal)) = "●青You won't be aware of my pains and lies.";
static const char data_26D1[40] __attribute__((scn_string_literal)) = "●青Don't leave me alone, stay with me.";
static const char data_26F9[43] __attribute__((scn_string_literal)) = "●青I don't know if you'll ever look back.";
static const char data_2724[32] __attribute__((scn_string_literal)) = "●青I'll remember you forever.";
static const int32_t data_2744[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
static const int32_t data_276C[10] = {64625, 4915, 0, 0, 0, 0, 0, 0, 0, 0};
static const int32_t data_2794[20] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 64625, 4915, 0, 0, 0, 0, 0, 0, 0, 0};
static const Vector4 data_27E4 = {.x = 0.1f, .y = 0.1f, .z = 0.1f, .w = 0.1f};
static const Vector4 data_27F4 = {.x = 0.3f, .y = 0.3f, .z = 0.3f, .w = 0.3f};
static const FogColourData data_2804 = {.red = 10.0f, .green = 200.0f, .blue = 0.75f, .colour = 0xFF000612U};
static const uint32_t data_2814[4] = {0, 0, 0, 0};
static const char data_2824[] __attribute__((scn_string_literal)) = "sprite";
static const char data_282B[13] __attribute__((scn_string_literal)) = "SCROLL27.SPR";
static const char data_2838[8] __attribute__((scn_string_literal)) = "01BIKEB";
static const char data_2840[13] __attribute__((scn_string_literal)) = "a1_nbike.snd";
static const char data_284D[] __attribute__((scn_string_literal)) = "sprite";
static const char data_2854[13] __attribute__((scn_string_literal)) = "SCROLL25.SPR";
static const char data_2861[13] __attribute__((scn_string_literal)) = "M_01BIKE.BIN";
static const char data_286E[] __attribute__((scn_string_literal)) = "sprite";
static const char data_2875[13] __attribute__((scn_string_literal)) = "SCROLL25.SPR";
static const char data_2882[13] __attribute__((scn_string_literal)) = "M_01BIKE.BIN";
static const char data_288F[] __attribute__((scn_string_literal)) = "sprite";
static const char data_2896[13] __attribute__((scn_string_literal)) = "SCROLL25.SPR";
static const char data_28A3[13] __attribute__((scn_string_literal)) = "M_01BIKE.BIN";
