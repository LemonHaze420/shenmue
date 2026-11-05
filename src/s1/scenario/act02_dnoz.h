#pragma once

#include "scenario.h"
#include "motion/motion.h"

static int32_t script_0030(void);
static void script_0060(void);
static void script_0086(void);
static void script_008F(void);
static void script_009E(int32_t arg0);
static void script_012B(int32_t arg0);
static int32_t script_017B(void);
static void script_01A9(int32_t arg0);
static void script_01D7(void);
static void script_020B(int32_t arg0);
static void script_023F(int32_t arg0);
static void script_025D(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_0297(int32_t arg0, int32_t arg1);
static void script_02CB(void);
static void script_03C3(int32_t arg0, int32_t arg1);
static void script_03F3(int32_t arg0);
static void script_042F(int32_t arg0);
static void script_04E4(int32_t arg0);
static void script_0625(void);
static void script_071A(void);
static void script_0780(int32_t arg0);
static void script_07BB(int32_t arg0);
static void script_07E5(int32_t arg0);
static void script_08C2(int32_t arg0);
static void script_0A2B(void);
static void script_0B36(void);
static void script_0B84(int32_t arg0);
static void script_0BBF(int32_t arg0);
void script_0BE9(void);
static int32_t script_0BFC(int32_t arg0);
static void script_1010(void);
static void script_10E7(void);

static const float data_11C0[3] = {1.0f, 1.0f, 1.0f};
static const int32_t data_11CC[4] = {16777215, 16777215, 16777215, 16777215};
static const uint32_t data_11DC = 0xC3D48000;
static const float data_11E0[2] = {-1.8f, -210.0f};
static const char data_11E8[] __attribute__((scn_string_literal)) = "scene/02/model";
static const char data_11F7[] __attribute__((scn_string_literal)) = "C1305S3G.MT5";
static const char data_1204[] __attribute__((scn_string_literal)) = "C1305S1G.MT5";
static const uint8_t pad_1211[5] = {0};
static const uint8_t data_1216[16] = {
  0xA0, 0x41, 0x00, 0x00, 0x48, 0x44, 0x00, 0x00, 0x80, 0x3F, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00
};
static const uint8_t pad_1226[234] = {0};
static const int32_t data_1310[340] = {
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
  -1638, 0, 0, 1092
};
static const int32_t data_1860[342] = {
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
static const int32_t data_1DB8[57] = {
  0, 0, -5461, 0, -2366, -1092, 0, -364, 1092, 0, -546, -6735, 0, 364, -4915, 0, 0, -3640, 0, -182, 1274, 0, 0, -6189, 0, -364, -2730, 0, 0, -5461, 0,
  -364, 2002, 0, 182, -8738, 0, -728, -2002, 0, -182, -6007, 0, -546, 2366, 0, -728, -5279, 0, -1274, -1820, 0, -1638, -5279, 0, 182, 2912
};
static const Vector4 data_1E9C = {.x = 1.0f, .y = 0.6f, .z = 0.6f, .w = 0.6f};
static const Vector4 data_1EAC = {.x = 1.0f, .y = 0.7f, .z = 0.7f, .w = 0.7f};
static const Vector4 data_1EBC = {.x = 1.0f, .y = 0.4f, .z = 0.4f, .w = 0.4f};
static const Vector4 data_1ECC = {.x = 1.0f, .y = 0.6f, .z = 0.6f, .w = 0.6f};
static const char data_1EDC[6] __attribute__((scn_string_literal)) = "01SKI";
static const char data_1EE2[13] __attribute__((scn_string_literal)) = "a1_nozok.snd";
static const char data_1EEF[11] __attribute__((scn_string_literal)) = "bgm068.snd";
static const char data_1EFA[13] __attribute__((scn_string_literal)) = "SEQDATA4.BIN";
static const char data_1F07[14] __attribute__((scn_string_literal)) = "SCENE/02/DNOZ";
static const char data_1F15[13] __attribute__((scn_string_literal)) = "SEQDATA5.BIN";
static const char data_1F22[14] __attribute__((scn_string_literal)) = "SCENE/02/DNOZ";
static const char data_1F30[12] __attribute__((scn_string_literal)) = "M_01SKI.BIN";
static const char data_1F3C[6] __attribute__((scn_string_literal)) = "01SKI";
static const char data_1F42[7] __attribute__((scn_string_literal)) = "01FULB";
static const char data_1F49[12] __attribute__((scn_string_literal)) = "M_01FUL.BIN";
static const char data_1F55[13] __attribute__((scn_string_literal)) = "SEQDATA2.BIN";
static const char data_1F62[14] __attribute__((scn_string_literal)) = "SCENE/02/DNOZ";
static const char data_1F70[13] __attribute__((scn_string_literal)) = "SEQDATA3.BIN";
static const char data_1F7D[14] __attribute__((scn_string_literal)) = "SCENE/02/DNOZ";
static const char data_1F8B[13] __attribute__((scn_string_literal)) = "a1_nonam.snd";
static const char data_1F98[11] __attribute__((scn_string_literal)) = "bgm068.snd";
static const char data_1FA3[9] __attribute__((scn_string_literal)) = "01FULB";
static const FogColourData data_1FAC = {.red = 16.7f, .green = 780.0f, .blue = 1.0f, .colour = 0xFF0D0705U};
static const uint32_t data_1FBC[4] = {0x320E102EU, 0x320E102EU, 0x64555462U, 0x64555462U};
static const Vector4 data_1FCC = {.x = 0.25f, .y = 0.09f, .z = 0.19f, .w = 0.31f};
static const Vector4 data_1FDC = {.x = 1.0f, .y = 0.8f, .z = 0.63f, .w = 0.4f};
static const float data_1FEC[2] = {0.69f, 1.0f};
static const Vector4 data_1FF4 = {.x = 1.0f, .y = 0.4f, .z = 0.4f, .w = 0.4f};
static const Vector4 data_2004 = {.x = 1.0f, .y = 0.6f, .z = 0.6f, .w = 0.6f};
static const char data_2014[] __attribute__((scn_string_literal)) = "sprite";
static const char data_201B[13] __attribute__((scn_string_literal)) = "SCROLL27.SPR";
static const FogColourData data_2028 = {.red = 16.7f, .green = 780.0f, .blue = 1.0f, .colour = 0xFF0D0705U};
static const uint32_t data_2038[4] = {0x320E102EU, 0x320E102EU, 0x64555462U, 0x64555462U};
static const Vector4 data_2048 = {.x = 0.25f, .y = 0.09f, .z = 0.19f, .w = 0.31f};
static const Vector4 data_2058 = {.x = 1.0f, .y = 0.8f, .z = 0.63f, .w = 0.4f};
static const float data_2068[2] = {0.69f, 1.0f};
static const Vector4 data_2070 = {.x = 1.0f, .y = 0.4f, .z = 0.4f, .w = 0.4f};
static const Vector4 data_2080 = {.x = 1.0f, .y = 0.6f, .z = 0.6f, .w = 0.6f};
static const char data_2090[] __attribute__((scn_string_literal)) = "sprite";
static const char data_2097[13] __attribute__((scn_string_literal)) = "SCROLL27.SPR";
