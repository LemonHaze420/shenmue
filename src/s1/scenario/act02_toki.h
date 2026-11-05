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
static void script_03F3(void);
static void script_04F4(int32_t arg0);
static void script_0A9C(void);
static void script_0FC4(void);
static void script_1117(void);
void script_1168(void);
static int32_t script_1184(int32_t arg0);
static void script_1271(void);
static void script_1356(void);
static void script_1457(int32_t arg0);

static int32_t task_0;
static float task_4;
static float task_8;
static float task_C;
static float task_10;
static float task_14;
static float task_18;
static Vector4 task_1C;
static const float data_159C[3] = {1.0f, 1.0f, 1.0f};
static const int32_t data_15A8[4] = {16777215, 16777215, 16777215, 16777215};
static const uint32_t data_15B8 = 0xC3D48000;
static const float data_15BC[2] = {-1.8f, -210.0f};
static const char data_15C4[] __attribute__((scn_string_literal)) = "scene/02/model";
static const char data_15D3[] __attribute__((scn_string_literal)) = "C1305S3G.MT5";
static const char data_15E0[] __attribute__((scn_string_literal)) = "C1305S1G.MT5";
static const uint8_t pad_15ED[5] = {0};
static const uint8_t data_15F2[16] = {
  0xA0, 0x41, 0x00, 0x00, 0x48, 0x44, 0x00, 0x00, 0x80, 0x3F, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00
};
static const uint8_t pad_1602[234] = {0};
static const int32_t data_16EC[340] = {
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
static const int32_t data_1C3C[342] = {
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
static const Vector3i data_2194 = {.x = 0, .y = -5461, .z = 1820};
static const int32_t data_21A0[57] = {
  0, 0, 0, 0, 0, -546, 0, 0, -1456, 0, 0, 0, 0, 0, -6189, 0, 0, -6917, 0, 0, 0, 0, 0, 0, 0, 0, -5643, 0, 0, -8738, 0, 0, 0, 0, 0, 0, 0, 0, -9830, 0,
  0, -7281, 0, 0, 0, 0, 0, 0, 0, 0, -8009, 0, -2912, -9830, 0, 0, 0
};
static const int32_t data_2284[57] = {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -3640, 0, 0, -364, 0, 0, 0, 0, 0, 0, 0, 0, -2912, 0, 0, -3822, 0, 0, 0, 0, 0, 0, 0, 0, -1274, 0, 0, -8192,
  0, 0, 0, 0, 0, 0, 0, 0, -1456, 0, -1092, -10376, 0, 0, 0
};
static const Vector3i data_2368 = {.x = 0, .y = -728, .z = 728};
static const int32_t data_2374[57] = {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -4551, 0, 0, -4551, 0, 0, 0, 0, 0, 0, 0, 0, -4915, 0, 0, -6553, 0, 0, 0, 0, 0, 0, 0, 0, -8009, 0, 0,
  -5643, 0, 0, 0, 0, 0, 0, 0, 0, -7281, 0, -2366, -6553, 0, 0, 0
};
static const int32_t data_2458[57] = {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1638, 0, 0, -2548, 0, 0, 1092, 0, 0, 0, 0, 0, -2002, 0, 0, -910, 0, 0, 546, 0, 0, 0, 0, 0, -1820, 0, 0,
  -728, 0, 0, 546, 0, 0, 0, 0, 0, -1820, 0, 0, -728, 0, 0, 364
};
static const int32_t data_253C[57] = {
  0, 0, -2730, 0, 0, -1820, 0, 0, 0, 0, 0, -4915, 0, 0, -13289, 0, 0, -6007, 0, 0, 0, 0, 0, -3276, 0, -910, -15837, 0, 0, -4369, 0, 0, 0, 0, 0, -4733,
  0, 0, -14199, 0, 0, -4915, 0, 0, 0, 0, 0, -10922, 0, 0, -10012, 0, -2912, -3094, 0, 0, 0
};
static const Vector3 data_2620 = {.x = 1.5f, .y = 1.5f, .z = 1.5f};
static const uint32_t data_262C = 0x3E99999A;
static const float data_2630[3] = {0.21f, 0.17f, 0.12f};
static const uint32_t data_263C = 0x3E99999A;
static const float data_2640[3] = {0.17f, 0.21f, 0.36f};
static const Vector3i data_264C = {.x = 0, .y = -3094, .z = -364};
static const Vector3i data_2658 = {.x = 0, .y = 3094, .z = 364};
static const Vector3 data_2664 = {.x = 0.153f, .y = 0.0016f, .z = -0.023f};
static const Vector3i data_2670 = {.x = 49152, .y = 0, .z = 0};
static const float data_267C[3] = {0.086f, -0.006f, 0.094f};
static const int32_t data_2688[3] = {5461, 0, 0};
static const Vector3i data_2694 = {.x = 0, .y = -3094, .z = -364};
static const Vector3i data_26A0 = {.x = 0, .y = 3094, .z = 364};
static const float data_26AC[8] = {1.0f, 0.1f, 0.1f, 0.1f, 1.0f, 0.3f, 0.3f, 0.3f};
static const Vector4 data_26CC = {.x = 1.0f, .y = 0.3f, .z = 0.3f, .w = 0.3f};
static const char data_26DC[7] __attribute__((scn_string_literal)) = "start\012";
static const char data_26E3[5] __attribute__((scn_string_literal)) = "end\012";
static const char data_26E8[12] __attribute__((scn_string_literal)) = "auth start\012";
static const char data_26F4[10] __attribute__((scn_string_literal)) = "01HOK_4.c";
static const char data_26FE[10] __attribute__((scn_string_literal)) = "01HOK_2.c";
static const char data_2708[10] __attribute__((scn_string_literal)) = "01HOK_3.c";
static const char data_2712[13] __attribute__((scn_string_literal)) = "SEQDATA0.BIN";
static const char data_271F[14] __attribute__((scn_string_literal)) = "SCENE/02/TOKI";
static const char data_272D[13] __attribute__((scn_string_literal)) = "SEQDATA1.BIN";
static const char data_273A[14] __attribute__((scn_string_literal)) = "SCENE/02/TOKI";
static const char data_2748[13] __attribute__((scn_string_literal)) = "SEQDATA2.BIN";
static const char data_2755[14] __attribute__((scn_string_literal)) = "SCENE/02/TOKI";
static const char data_2763[13] __attribute__((scn_string_literal)) = "a1_hokky.snd";
static const char data_2770[11] __attribute__((scn_string_literal)) = "bgm050.snd";
static const char data_277B[6] __attribute__((scn_string_literal)) = "01HOK";
static const char data_2781[12] __attribute__((scn_string_literal)) = "M_01HOK.BIN";
static const char data_278D[] __attribute__((scn_string_literal)) = "sprite";
static const char data_2794[13] __attribute__((scn_string_literal)) = "SCROLL25.SPR";
static const char data_27A1[7] __attribute__((scn_string_literal)) = "01HOK";
