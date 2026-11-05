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
static void script_039D(int32_t arg0);
static int32_t script_0451(void);
static void script_0948(void);
static void script_09B1(void);
static void script_0A12(int32_t arg0, int32_t arg1);
static void script_0A3D(void);
static void script_0B94(void);
static void script_0C62(int32_t arg0);
static void script_0C9E(int32_t arg0);
static void script_0D3D(void);
static void script_0DB5(void);
static void script_0E19(void);
static void script_0E7D(void);
static void script_0EE8(void);
static int32_t script_0F08(void);
static int32_t script_0F21(void);
static int32_t script_16D1(void);
static int32_t script_173C(void);
static void script_194C(void);
static int32_t script_1A56(void);
static void script_1C79(void);
static void script_1CA6(float arg0);
static void script_1CEF(int32_t arg0);
static int32_t script_1F3D(void);
static void script_1FA2(void);
void script_20C3(void);
static int32_t script_20EA(int32_t arg0);

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
typedef struct Row_22F4 { float w0; float w1; float w2; int32_t w3; int32_t w4; int32_t w5; int32_t w6; } Row_22F4;
static const Row_22F4 data_22F4[9] = {
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
static const int32_t data_23F0[5] = {0, 0, 0, 0, 0};
static const int32_t data_2404[171] = {
  0, 0, -10922, 0, 0, -10558, 0, 1456, 0, 0, 0, -18022, 0, 0, -17112, 0, 0, -18386, 0, 0, 0, 0, 0, -12196, 0, 0, -18204, 0, 0, -16748, 0, 0, 0, 0, 0,
  -11832, 0, 0, -18386, 0, 0, -15109, 0, 0, 0, 0, 910, -12014, 0, -2184, -16748, 0, -2184, -10012, 0, 0, 0, 0, -364, -11650, -2912, -364, -4369, 1638,
  910, 182, 0, 0, -16019, 0, 182, -23119, 0, 546, -18932, 0, 0, -1092, 0, 0, -16384, 0, 0, -21845, 0, 0, -18750, 0, 0, -546, 0, 0, -21481, 0, 0,
  -21299, 0, 0, -18568, 0, 0, -728, 0, 910, -18750, 0, 182, -18750, 0, -4004, -18204, 0, 0, -728, 0, -364, -14017, -2912, -364, -6735, 1638, 910,
  2548, 0, 0, -18568, 0, 182, -25668, 0, 546, -21481, 0, 0, -3640, 0, 0, -18932, 0, 0, -24393, 0, 0, -21299, 0, 0, -3094, 0, 0, -24029, 0, 0, -23847,
  0, 0, -21117, 0, 0, -3276, 0, 910, -21299, 0, 182, -21299, 0, -4004, -20753, 0, 0, -3276
};
static const int32_t data_26B0[513] = {
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
static const int32_t data_2EB4[57] = {
  0, 728, -6189, -4915, 2366, 1638, -6735, 2730, -1274, 0, 0, -8556, 0, 0, -15837, 0, 0, -14017, 0, 0, -910, 0, 0, -12561, 0, -182, -13471, 0, -364,
  -13471, 0, 0, -546, 0, 0, -11104, 0, 0, -13289, 0, 0, -11286, 0, 0, -364, 0, 0, -4733, 0, -1274, -9102, 0, -1638, -11468, -1456, 0, 182
};
static const int32_t data_2F98[114] = {
  0, 728, -3640, 0, 0, -2184, 0, 6553, 0, 0, 0, -4369, 0, 0, -4369, 0, 0, -3640, 0, 0, 0, 0, 0, -4369, 0, 0, -3640, 0, 0, -3640, 0, 0, 0, 0, 0, -5097,
  0, 0, -4369, 0, 0, -2912, 0, 0, 0, 0, 0, -4369, 0, -2912, -3640, 0, 0, -4369, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -727, 0, 0, -2547, 0, 0,
  -2183, 0, 0, 0, 0, 0, 0, 0, 0, -2729, 0, 0, -2001, 0, 0, 0, 0, 0, -1091, 0, 0, -1637, 0, 0, -2183, 0, 0, 0, 0, -1637, -3093, 0, 0, -3457, 0, 0, 0,
  0, 0, 0
};
static const char data_3160[12] __attribute__((scn_string_literal)) = "M_01123.BIN";
static const char data_316C[11] __attribute__((scn_string_literal)) = "amb024.snd";
static const char data_3177[13] __attribute__((scn_string_literal)) = "a1_jytat.snd";
static const char data_3184[8] __attribute__((scn_string_literal)) = "test\012";
static const int32_t data_318C[114] = {
  0, 0, -3640, 0, 0, -2366, -4369, 1820, -546, 0, 0, -5279, 0, 0, -6007, 0, 0, -2912, 0, 0, -910, 0, 0, -6553, 0, 0, -6735, 0, 0, -3640, 0, 0, -182,
  0, 0, -5461, 0, 0, -6007, 0, 0, -2366, 0, 0, 0, 0, 0, -5643, 0, 0, -4733, 0, 0, -2366, -1456, 0, 728, 0, 0, -3640, 0, 0, -2730, -4004, 1820, -546,
  0, 0, -5461, 0, 0, -6371, 0, 0, -3276, 0, 0, -1092, 0, 0, -6735, 0, 0, -6917, 0, 0, -4004, 0, 0, -364, 0, 0, -6189, 0, 0, -6735, 0, 0, -3094, 182,
  182, -182, 0, 0, -7099, 0, 0, -6917, 0, 0, -3640, -546, 0, -182
};
static const int32_t data_3354[57] = {
  0, 0, -3640, 0, 0, -2366, -4369, 1820, -546, 0, 0, -5279, 0, 0, -6007, 0, 0, -2912, 0, 0, -910, 0, 0, -6553, 0, 0, -6735, 0, 0, -3640, 0, 0, -182,
  0, 0, -6007, 0, 0, -6917, 0, 0, -2730, 0, 0, 0, 0, 0, -9830, 0, 0, -8009, 0, 0, -2912, -1456, 0, 728
};
static const int32_t data_3438[57] = {
  0, 0, -4733, 0, 0, -2366, -4369, 1820, -546, 0, 0, -9102, 0, 0, -9830, 0, 0, -4551, 0, 0, 546, 0, 0, -11286, 0, 0, -9102, 0, 0, -4369, 0, 0, -182,
  0, 0, -7827, 0, 0, -7645, 0, 0, -2730, 0, 0, 0, 0, 0, -6553, 0, 0, -5825, 0, 0, -2730, -1456, 0, -728
};
static const int32_t data_351C[57] = {
  0, 0, -5643, 0, 0, -3094, -4369, 1820, -546, 0, 0, -2366, 0, 0, 182, 0, 0, -2912, 0, 0, -910, 0, 0, -7463, 0, 0, -11104, 0, 0, -7281, 0, 0, -364, 0,
  0, -10012, 0, 0, -12196, 0, 0, -6553, 0, 0, 0, 0, 0, -10558, 0, -364, -9284, 0, -364, -5279, -1456, 0, 364
};
static const int32_t data_3600[57] = {
  0, 0, -3640, 0, 0, -2366, -4369, 1820, -546, 0, 0, -5279, 0, 0, -4004, 0, 0, -2548, 0, 0, -910, 0, 0, -11286, 0, 0, -15109, 0, 0, -8192, 0, 0, -182,
  0, 1820, -11104, 0, 0, -11832, 0, 0, -8009, 0, 0, 0, 0, 0, -11286, 0, 0, -10194, 0, 0, -6917, -1456, 0, 728
};
static const char data_36E4[6] __attribute__((scn_string_literal)) = "A0172";
static const char data_36EA[11] __attribute__((scn_string_literal)) = "M_0172.BIN";
static const char data_36F5[12] __attribute__((scn_string_literal)) = "system1.snd";
static const char data_3701[11] __attribute__((scn_string_literal)) = "tsm001.snd";
static const char data_370C[11] __attribute__((scn_string_literal)) = "amb024.snd";
static const char data_3717[12] __attribute__((scn_string_literal)) = "a1_bgam.snd";
static const char data_3723[13] __attribute__((scn_string_literal)) = "battle_1.snd";
static const char data_3730[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_3735[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_373A[13] __attribute__((scn_string_literal)) = "e1batchi.snd";
static const char data_3747[13] __attribute__((scn_string_literal)) = "battle_1.snd";
static const char data_3754[6] __attribute__((scn_string_literal)) = "A0172";
static const char data_375A[6] __attribute__((scn_string_literal)) = "A0172";
static const char data_3760[11] __attribute__((scn_string_literal)) = "M_0172.BIN";
static const char data_376B[11] __attribute__((scn_string_literal)) = "amb024.snd";
static const char data_3776[12] __attribute__((scn_string_literal)) = "a1_bgam.snd";
static const char data_3782[6] __attribute__((scn_string_literal)) = "A0172";
static const Vector4 data_3788 = {.x = 0.07f, .y = 0.07f, .z = 0.07f, .w = 0.07f};
static const uint32_t data_3798[4] = {0x22FFFFFFU, 0x22FFFFFFU, 0xD9FFFFFFU, 0xD9FFFFFFU};
static const char data_37A8[7] __attribute__((scn_string_literal)) = "A0172A";
static const char data_37AF[11] __attribute__((scn_string_literal)) = "M_0172.BIN";
static const char data_37BA[11] __attribute__((scn_string_literal)) = "amb024.snd";
static const char data_37C5[12] __attribute__((scn_string_literal)) = "a1_bgam.snd";
static const char data_37D1[] __attribute__((scn_string_literal)) = "sprite";
static const char data_37D8[13] __attribute__((scn_string_literal)) = "SCROLL00.SPR";
static const char data_37E5[7] __attribute__((scn_string_literal)) = "A0172A";
static const uint32_t data_37EC[4] = {0x22FFFFFFU, 0x22FFFFFFU, 0xD9FFFFFFU, 0xD9FFFFFFU};
static Vector3 data_37FC = {.x = 2.7f, .y = 0.0f, .z = 0.0f};
static const Vector3 data_3808 = {.x = 3.069f, .y = 0.0f, .z = -4.912f};
static const Vector3 data_3814 = {.x = 2.744f, .y = 0.0f, .z = -2.581f};
static const char data_3820[11] __attribute__((scn_string_literal)) = "M_FGT4.BIN";
static const char data_382B[6] __attribute__((scn_string_literal)) = "A0172";
static const char data_3831[11] __attribute__((scn_string_literal)) = "M_0172.BIN";
static const char data_383C[12] __attribute__((scn_string_literal)) = "system1.snd";
static const char data_3848[11] __attribute__((scn_string_literal)) = "tsm001.snd";
static const char data_3853[11] __attribute__((scn_string_literal)) = "amb024.snd";
static const char data_385E[12] __attribute__((scn_string_literal)) = "a1_bgam.snd";
static const char data_386A[13] __attribute__((scn_string_literal)) = "battle_1.snd";
static const char data_3877[9] __attribute__((scn_string_literal)) = "A0172";
