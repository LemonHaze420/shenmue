#pragma once

#include "scenario.h"
#include "motion/motion.h"

static int32_t script_0030(void);
static void script_0091(void);
static void script_009A(void);
static void script_00A9(int32_t arg0);
static void script_0136(int32_t arg0);
static int32_t script_0186(void);
static void script_01B4(int32_t arg0);
static void script_01E2(void);
static void script_0216(int32_t arg0);
static void script_024A(int32_t arg0);
static void script_0268(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_02A2(int32_t arg0, int32_t arg1);
static void script_02D6(void);
static void script_03CE(int32_t arg0, int32_t arg1);
static void script_03FE(int32_t arg0);
static void script_0609(void);
static void script_06A3(void);
static void script_06ED(int32_t arg0, int32_t arg1);
static void script_0751(void);
static void script_0EAB(void);
static void script_0F57(void);
static int32_t script_0F9A(void);
void script_102C(void);
static int32_t script_103A(void);
static void script_12D3(void);
static void script_13AB(void);
static void script_14CB(void);
static void script_15AF(void);

static const float data_19B4[3] = {1.0f, 1.0f, 1.0f};
static const int32_t data_19C0[4] = {16777215, 16777215, 16777215, 16777215};
static const float data_19D0[3] = {20.0f, 800.0f, 1.0f};
static const int32_t data_19DC[742] = {
  -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -10922, 0, 0, -10558, 0, 1456, 0, 0, 0, -18022, 0, 0, -17112, 0, 0, -18386, 0, 0, 0, 0, 0, -12196, 0, 0, -18204, 0,
  0, -16748, 0, 0, 0, 0, 0, -11832, 0, 0, -18386, 0, 0, -15109, 0, 0, 0, 0, 910, -12014, 0, -2184, -16748, 0, -2184, -10012, 0, 0, 0, 0, -364, -11650,
  -2912, -364, -4369, 1638, 910, 182, 0, 0, -16019, 0, 182, -23119, 0, 546, -18932, 0, 0, -1092, 0, 0, -16384, 0, 0, -21845, 0, 0, -18750, 0, 0, -546,
  0, 0, -21481, 0, 0, -21299, 0, 0, -18568, 0, 0, -728, 0, 910, -18750, 0, 182, -18750, 0, -4004, -18204, 0, 0, -728, 0, -364, -14017, -2912, -364,
  -6735, 1638, 910, 2548, 0, 0, -18568, 0, 182, -25668, 0, 546, -21481, 0, 0, -3640, 0, 0, -18932, 0, 0, -24393, 0, 0, -21299, 0, 0, -3094, 0, 0,
  -24029, 0, 0, -23847, 0, 0, -21117, 0, 0, -3276, 0, 910, -21299, 0, 182, -21299, 0, -4004, -20753, 0, 0, -3276, 0, 0, -5461, 0, -2366, -1092, 0,
  -182, 1092, 0, -546, -6735, 0, 0, -4004, 0, 0, -2912, 0, -182, 1274, 0, 0, -13289, 0, 0, -5279, 0, 0, -8009, 0, 0, 1456, 0, 182, -8738, 0, 0, -6007,
  0, 0, -8738, 0, 182, 1638, 0, -728, -5279, 0, -1274, -6735, 0, -910, -8738, 0, 0, 1638, 0, 0, -5461, 0, -2366, -1092, 0, -182, 1092, 0, -546,
  -13289, 0, 0, -5643, 0, 0, -8009, 0, -182, 1274, 0, 0, -11832, 0, 0, -5279, 0, 0, -8009, 0, 0, 1456, 0, 182, -8738, 0, 0, -6007, 0, 0, -8738, 0,
  182, 1638, 0, -728, -5279, 0, -1274, -6735, 0, -910, -8738, 0, 0, 1092, 0, 0, -5461, 0, -2366, -1092, 0, -182, 1092, 0, -546, -3640, 0, 0, -2366, 0,
  0, -1456, 0, -182, 1274, 0, 0, -3822, 0, 0, -2730, 0, 0, -1456, 0, 0, 1456, 0, 182, -3822, 0, 0, -2912, 0, 0, -1638, 0, 182, 1638, 0, -728, -3640,
  0, -910, -2730, 0, -546, -1638, 0, 0, 1092, 0, 0, -5461, 0, -2366, -1092, 0, -182, 1092, 0, -546, -6735, 0, 0, -4004, 0, 0, -6553, 0, -182, 1274, 0,
  0, -13289, 0, 0, -5279, 0, 0, -8009, 0, 0, 1456, 0, 182, -8738, 0, 0, -6007, 0, 0, -8738, 0, 182, 1638, 0, -728, -5279, 0, -1274, -6735, 0, -910,
  -8738, 0, 0, 1638, 0, 0, -9102, 0, -2366, -2002, 0, -182, 910, 0, -546, -2366, 0, 0, -2002, 0, 0, -2912, 0, -182, 1274, 0, 0, -14199, 0, 0, -11468,
  0, 0, -13653, 0, 0, 1456, 0, 182, -11286, 0, 0, -12014, 0, 0, -12743, 0, 182, 1638, 0, -728, -10194, 0, -1274, -12196, 0, -910, -10922, -728, 546,
  1638, 0, 0, -1820, 0, -2366, -728, 0, -364, -1092, 0, -546, -11832, 0, 910, -6735, 0, 0, -6553, 0, -182, 1274, 0, 0, -13289, 0, 0, -6007, 0, 0,
  -10012, 0, 0, 1092, 0, 182, -8738, 0, 0, -6735, 0, 0, -9830, 0, 182, 1638, 0, -728, -6553, 0, -1274, -7281, 0, -910, -10740, 0, 0, 1638, 0, 0,
  -5461, -2912, -2366, -364, -5279, 2548, -910, 0, -546, -8374, 0, 0, -9466, 0, 910, -4915, 0, -182, 546, 0, 0, -13289, 0, 0, -5279, 0, 364, -7645, 0,
  -182, 1456, 0, 182, -8738, 0, 0, -7281, 0, 182, -6917, 0, -182, 1638, 0, -728, -5279, 0, -1274, -6735, 0, -182, -8009, 0, 0, 1638, -4733, 0, -8556,
  0, -2366, -1092, -6189, 2912, 0, 0, -546, -10922, 0, 1820, -16384, 0, 0, -11468, 0, 182, 546, 0, 0, -10376, 0, 0, -15837, 0, 0, -12743, 0, -182,
  728, 0, 182, -13835, 0, 0, -16201, 0, 364, -10922, 0, -182, 728, 0, -728, -9648, 0, -3458, -12379, 0, -728, -7463, 0, 0, 1092, 0, 0, -5461, 0,
  -2366, -1638, 0, -910, 1092, 0, -546, -3640, 0, 0, -2366, 0, 0, -1456, 0, -182, 1638, 0, 0, -910, 0, 0, -2730, 0, 0, -1274, 0, 0, 1820, 0, 182,
  -2730, 0, 0, -1820, 0, 0, -1274, 0, 182, 2184, 0, -728, -3094, 0, -910, -2548, 0, 910, -1456, -546, 910, 2366
};
static const int32_t data_2574[57] = {
  0, -546, -3458, 0, 1638, -2548, 0, 5825, -546, 0, 728, -10194, 0, 0, -11832, 0, 0, -15837, 0, 364, -3458, 0, 0, -8920, 0, 0, -15109, 0, -182,
  -14745, 0, 0, -2912, 0, 0, -13835, 0, 0, -11286, 0, 0, -14927, 0, 0, -2002, 0, 0, -12379, 0, 0, -8556, 0, -1820, -12925, -2002, -546, -1456
};
static const int32_t data_2658[114] = {
  -1820, 728, -6189, -2002, 1274, -13471, -618, 6371, 819, 0, 0, -18568, 0, 182, -25668, 0, 546, -21481, 0, 0, -3640, 0, 0, -18932, 0, 0, -24393, 0,
  0, -21299, 0, 0, -3094, 0, 0, -24029, 0, 0, -23847, 0, 0, -21117, 0, 0, -3276, 0, 910, -21299, 0, 182, -21299, 0, -4004, -20753, 0, 0, -3276, 0,
  -364, -11650, -2894, -1092, -11086, 1620, 2910, 0, 0, 0, -16019, 0, 182, -23119, 0, 546, -18932, 0, 0, -1092, 0, 0, -16384, 0, 0, -21845, 0, 0,
  -18750, 0, 0, -546, 0, 0, -21481, 0, 0, -21299, 0, 0, -18568, 0, 0, -728, 0, 910, -18750, 0, 182, -18750, 0, -4004, -18204, 0, 0, -728
};
static const int32_t data_2820[57] = {
  -2912, -364, -7281, -3094, 1638, -14563, -1711, 6735, -455, 0, 364, -18932, 0, 182, -25668, 0, 546, -15291, 0, 364, -1820, 0, 0, -16748, 0, 0,
  -24393, 0, 1274, -16566, 0, 182, -2366, 0, 0, -23847, 0, 546, -22209, 0, 1820, -17840, 0, 546, -2912, 0, 910, -21481, 0, 728, -17840, 0, -4004,
  -17476, 0, -364, -3440
};
static const int32_t data_2904[114] = {
  0, 0, -5461, 0, -2366, -1092, 0, 2548, 546, 0, -546, -3822, 0, 1274, -2366, 0, 0, -1456, 0, 6007, 546, 0, 0, -3822, 0, 0, -2730, 0, 0, -5279, 0,
  2184, -182, 0, 182, -3822, 0, 0, -2912, 0, -728, -3458, 0, 728, -546, 0, -728, -3640, 0, -910, -2730, 0, 0, -2730, 364, -546, -910, 0, -182, -10376,
  0, -1092, -4733, 0, 3094, -182, 0, 0, -18022, 0, 0, -17112, 0, 182, -11286, 0, 0, 0, 0, 0, -12196, 0, 0, -18204, 0, 0, -11468, 0, 0, 0, 0, 0,
  -11832, 0, 0, -18386, 0, 0, -10740, 0, 0, -1092, 0, 910, -8556, 0, -3640, -6735, 0, -2184, -12561, 0, 0, 0
};
static const Vector3i data_2ACC = {.x = 2366, .y = 1637, .z = -2182};
static const int32_t data_2AD8[6] = {-5097, 546, 0, 1456, 2366, -9102};
static const Vector4 data_2AF0 = {.x = 1.0f, .y = 0.3f, .z = 0.3f, .w = 0.3f};
static const Vector3 data_2B00 = {.x = 0.034f, .y = 0.355f, .z = -0.168f};
static const Vector3i data_2B0C = {.x = 12833, .y = 50432, .z = 38218};
static const int32_t data_2B18[4] = {0, 0, -9935758, -9935758};
static const int32_t data_2B28[57] = {
  0, -546, -15109, -4004, 3913, -5643, -1274, 6917, -728, 0, 728, -10922, 0, -728, -12925, 0, 0, -14745, 728, 182, -2912, 0, 0, -8920, 0, 0, -15109,
  0, -182, -14745, 0, 0, -2912, 0, 0, -13835, 0, 0, -14927, 0, 0, -12196, 0, 0, -2002, 0, 0, -12379, 0, 0, -14017, 0, -1820, -9284, -2002, -546, -1456
};
static const int32_t data_2C0C[57] = {
  0, 0, -3640, 0, 0, -3640, 0, 0, 0, 0, 0, -3640, 0, 0, -3640, 0, 0, 0, 0, 0, 0, 0, 0, -3640, 0, 0, -3640, 0, 0, 0, 0, 0, 0, 0, 0, -3640, 0, 0, -3640,
  0, 0, 0, 0, 0, 0, 0, 0, -3640, 0, 0, -3640, 0, 0, 0, 0, 0, 0
};
static const int32_t data_2CF0[57] = {
  0, 0, -2730, 0, 0, -910, 0, 546, -182, 0, 0, -8465, 0, 0, -4605, 0, 0, -5552, 0, 0, -182, 0, 0, -7281, 0, 0, -7482, 0, 0, -6189, 0, 0, 0, 0, 0,
  -6262, 0, 0, -8701, 0, 0, -4096, 0, 54, -455, 0, -2548, -9502, 0, -1820, -7008, 0, -546, -1037, 0, -618, -1492
};
static const int32_t data_2DD4[57] = {
  0, 0, -3094, 0, 0, 546, 0, 546, -182, 0, 0, -8465, 0, 0, -4605, 0, 0, -5552, 0, 0, -182, 0, 0, -7281, 0, 0, -7482, 0, 0, -6189, 0, 0, 0, 0, 0,
  -6262, 0, 0, -8701, 0, 0, -4096, 0, 54, -455, 0, -2548, -9502, 0, -1820, -7008, 0, -546, -1037, 0, -618, -1492
};
static const int32_t data_2EB8[57] = {
  0, 0, -5461, 0, -2366, -1638, 0, -910, 1092, 0, -546, -3640, 0, 0, -2366, 0, 0, -1456, 0, -182, 1638, 0, 0, -910, 0, 0, -2730, 0, 0, -1274, 0, 0,
  1820, 0, 182, -2730, 0, 0, -1820, 0, 0, -1274, 0, 182, 2184, 0, -728, -3094, 0, -910, -2548, 0, 910, -1456, -546, 910, 2366
};
static const Vector4 data_2F9C = {.x = 0.3f, .y = 0.3f, .z = 0.3f, .w = 0.3f};
static const char data_2FAC[16] __attribute__((scn_string_literal)) = "/scene/03/YDMA/";
static const char data_2FBC[5] __attribute__((scn_string_literal)) = "YDMA";
static const char data_2FC1[11] __attribute__((scn_string_literal)) = "bgm107.snd";
static const char data_2FCC[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_2FD1[13] __attribute__((scn_string_literal)) = "a1_kstab.snd";
static const char data_2FDE[13] __attribute__((scn_string_literal)) = "a1_tabid.snd";
static const char data_2FEB[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_2FF0[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_2FF5[6] __attribute__((scn_string_literal)) = "A0180";
static const char data_2FFB[11] __attribute__((scn_string_literal)) = "M_0180.BIN";
static const char data_3006[13] __attribute__((scn_string_literal)) = "HMOT0156.BIN";
static const char data_3013[12] __attribute__((scn_string_literal)) = "M_01152.BIN";
static const char data_301F[6] __attribute__((scn_string_literal)) = "A0180";
static const char data_3025[11] __attribute__((scn_string_literal)) = "bgm071.snd";
static const char data_3030[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_3035[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_303A[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_303F[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_3044[24] = "FREE\000\000\000\000\315\314\314=\000\000\264C\205\353\221?\033\031\027\377";
static const Vector3 data_305C = {.x = 0.01032f, .y = 0.04001f, .z = 0.02149f};
static const Vector3 data_3068 = {.x = -0.98f, .y = 0.29f, .z = 10.8789f};
static const Vector3i data_3074 = {.x = 0, .y = 0, .z = 0};
static const Vector3 data_3080 = {.x = -63.379681f, .y = 7.58001f, .z = 105.061493f};
static const Vector3 data_308C = {.x = -53.491001f, .y = 7.822f, .z = 100.286003f};
static const Vector3 data_3098 = {.x = 0.01032f, .y = -0.05999f, .z = 0.02149f};
static const Vector3i data_30A4 = {.x = 0, .y = 21955, .z = 0};
static const char data_30B0[8] __attribute__((scn_string_literal)) = "01152.c";
static const char data_30B8[3] __attribute__((scn_string_literal)) = ".c";
static const char data_30BB[4] __attribute__((scn_string_literal)) = "ALL";
static const char data_30BF[12] __attribute__((scn_string_literal)) = "M_01152.BIN";
static const char data_30CB[11] __attribute__((scn_string_literal)) = "bgm107.snd";
static const char data_30D6[13] __attribute__((scn_string_literal)) = "a1_tabid.snd";
static const char data_30E3[11] __attribute__((scn_string_literal)) = "tsm003.snd";
static const char data_30EE[13] __attribute__((scn_string_literal)) = "a1_kstab.snd";
static const char data_30FB[6] __attribute__((scn_string_literal)) = "A0180";
static const char data_3101[11] __attribute__((scn_string_literal)) = "M_0180.BIN";
static const char data_310C[13] __attribute__((scn_string_literal)) = "HMOT0156.BIN";
static const char data_3119[6] __attribute__((scn_string_literal)) = "A0180";
static const char data_311F[11] __attribute__((scn_string_literal)) = "tsm003.snd";
static const char data_312A[13] __attribute__((scn_string_literal)) = "a1_kstab.snd";
static const char data_3137[6] __attribute__((scn_string_literal)) = "A0180";
static const char data_313D[11] __attribute__((scn_string_literal)) = "M_0180.BIN";
static const char data_3148[13] __attribute__((scn_string_literal)) = "HMOT0156.BIN";
static const char data_3155[6] __attribute__((scn_string_literal)) = "A0180";
static const char data_315B[12] __attribute__((scn_string_literal)) = "M_01152.BIN";
static const char data_3167[11] __attribute__((scn_string_literal)) = "bgm107.snd";
static const char data_3172[14] __attribute__((scn_string_literal)) = "a1_tabid.snd";
