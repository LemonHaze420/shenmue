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
static void script_0299(void);
static void script_02A8(int32_t arg0);
static void script_0335(int32_t arg0);
static void script_0385(void);
static void script_03B3(int32_t arg0);
static void script_03E1(void);
static void script_0415(int32_t arg0);
static void script_0449(int32_t arg0);
static void script_0467(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_04A1(int32_t arg0, int32_t arg1);
static void script_04D5(void);
static void script_05CD(int32_t arg0, int32_t arg1);
static void script_05FD(int32_t arg0);
static void script_0660(int32_t arg0);
static void script_0731(void);
static int32_t script_078B(void);
static void script_07A4(void);
static int32_t script_0805(void);
static int32_t script_1133(void);
static void script_1948(void);
static int32_t script_2353(void);
static int32_t script_2FED(void);
static int32_t script_3646(void);
void script_3D09(void);

static int32_t task_0;
static int32_t task_4;
static const float data_43CC[3] = {1.0f, 1.0f, 1.0f};
static const int32_t data_43D8[4] = {16777215, 16777215, 16777215, 16777215};
static const float data_43E8[3] = {20.0f, 800.0f, 1.0f};
static const int32_t data_43F4[115] = {
  -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -10922, 0, 0, -10558, 0, 1456, 0, 0, 0, -18022, 0, 0, -17112, 0, 0, -18386, 0, 0, 0, 0, 0, -12196, 0, 0, -18204, 0,
  0, -16748, 0, 0, 0, 0, 0, -11832, 0, 0, -18386, 0, 0, -15109, 0, 0, 0, 0, 910, -12014, 0, -2184, -16748, 0, -2184, -10012, 0, 0, 0
};
static const int32_t data_45C0[627] = {
  0, -364, -11650, -2912, -364, -4369, 1638, 910, 182, 0, 0, -16019, 0, 182, -23119, 0, 546, -18932, 0, 0, -1092, 0, 0, -16384, 0, 0, -21845, 0, 0,
  -18750, 0, 0, -546, 0, 0, -21481, 0, 0, -21299, 0, 0, -18568, 0, 0, -728, 0, 910, -18750, 0, 182, -18750, 0, -4004, -18204, 0, 0, -728, 0, -364,
  -14017, -2912, -364, -6735, 1638, 910, 2548, 0, 0, -18568, 0, 182, -25668, 0, 546, -21481, 0, 0, -3640, 0, 0, -18932, 0, 0, -24393, 0, 0, -21299, 0,
  0, -3094, 0, 0, -24029, 0, 0, -23847, 0, 0, -21117, 0, 0, -3276, 0, 910, -21299, 0, 182, -21299, 0, -4004, -20753, 0, 0, -3276, 0, 0, -5461, 0,
  -2366, -1092, 0, -182, 1092, 0, -546, -6735, 0, 0, -4004, 0, 0, -2912, 0, -182, 1274, 0, 0, -13289, 0, 0, -5279, 0, 0, -8009, 0, 0, 1456, 0, 182,
  -8738, 0, 0, -6007, 0, 0, -8738, 0, 182, 1638, 0, -728, -5279, 0, -1274, -6735, 0, -910, -8738, 0, 0, 1638, 0, 0, -5461, 0, -2366, -1092, 0, -182,
  1092, 0, -546, -13289, 0, 0, -5643, 0, 0, -8009, 0, -182, 1274, 0, 0, -11832, 0, 0, -5279, 0, 0, -8009, 0, 0, 1456, 0, 182, -8738, 0, 0, -6007, 0,
  0, -8738, 0, 182, 1638, 0, -728, -5279, 0, -1274, -6735, 0, -910, -8738, 0, 0, 1092, 0, 0, -5461, 0, -2366, -1092, 0, -182, 1092, 0, -546, -3640, 0,
  0, -2366, 0, 0, -1456, 0, -182, 1274, 0, 0, -3822, 0, 0, -2730, 0, 0, -1456, 0, 0, 1456, 0, 182, -3822, 0, 0, -2912, 0, 0, -1638, 0, 182, 1638, 0,
  -728, -3640, 0, -910, -2730, 0, -546, -1638, 0, 0, 1092, 0, 0, -5461, 0, -2366, -1092, 0, -182, 1092, 0, -546, -6735, 0, 0, -4004, 0, 0, -6553, 0,
  -182, 1274, 0, 0, -13289, 0, 0, -5279, 0, 0, -8009, 0, 0, 1456, 0, 182, -8738, 0, 0, -6007, 0, 0, -8738, 0, 182, 1638, 0, -728, -5279, 0, -1274,
  -6735, 0, -910, -8738, 0, 0, 1638, 0, 0, -9102, 0, -2366, -2002, 0, -182, 910, 0, -546, -2366, 0, 0, -2002, 0, 0, -2912, 0, -182, 1274, 0, 0,
  -14199, 0, 0, -11468, 0, 0, -13653, 0, 0, 1456, 0, 182, -11286, 0, 0, -12014, 0, 0, -12743, 0, 182, 1638, 0, -728, -10194, 0, -1274, -12196, 0,
  -910, -10922, -728, 546, 1638, 0, 0, -1820, 0, -2366, -728, 0, -364, -1092, 0, -546, -11832, 0, 910, -6735, 0, 0, -6553, 0, -182, 1274, 0, 0,
  -13289, 0, 0, -6007, 0, 0, -10012, 0, 0, 1092, 0, 182, -8738, 0, 0, -6735, 0, 0, -9830, 0, 182, 1638, 0, -728, -6553, 0, -1274, -7281, 0, -910,
  -10740, 0, 0, 1638, 0, 0, -5461, -2912, -2366, -364, -5279, 2548, -910, 0, -546, -8374, 0, 0, -9466, 0, 910, -4915, 0, -182, 546, 0, 0, -13289, 0,
  0, -5279, 0, 364, -7645, 0, -182, 1456, 0, 182, -8738, 0, 0, -7281, 0, 182, -6917, 0, -182, 1638, 0, -728, -5279, 0, -1274, -6735, 0, -182, -8009,
  0, 0, 1638, -4733, 0, -8556, 0, -2366, -1092, -6189, 2912, 0, 0, -546, -10922, 0, 1820, -16384, 0, 0, -11468, 0, 182, 546, 0, 0, -10376, 0, 0,
  -15837, 0, 0, -12743, 0, -182, 728, 0, 182, -13835, 0, 0, -16201, 0, 364, -10922, 0, -182, 728, 0, -728, -9648, 0, -3458, -12379, 0, -728, -7463, 0,
  0, 1092, 0, 0, -5461, 0, -2366, -1638, 0, -910, 1092, 0, -546, -3640, 0, 0, -2366, 0, 0, -1456, 0, -182, 1638, 0, 0, -910, 0, 0, -2730, 0, 0, -1274,
  0, 0, 1820, 0, 182, -2730, 0, 0, -1820, 0, 0, -1274, 0, 182, 2184, 0, -728, -3094, 0, -910, -2548, 0, 910, -1456, -546, 910, 2366
};
static const int32_t data_4F8C[57] = {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0
};
static const int32_t data_5070[57] = {
  0, 0, -3640, 0, 0, -2366, -4369, 1820, -546, 0, 0, -5279, 0, 0, -6007, 0, 0, -2912, 0, 0, -910, 0, 0, -6553, 0, 0, -6735, 0, 0, -3640, 0, 0, -182,
  0, 0, -5461, 0, 0, -6007, 0, 0, -2366, 0, 0, 0, 0, 0, -5643, 0, 0, -4733, 0, 0, -2366, -1456, 0, 728
};
static const int32_t data_5154[171] = {
  0, 0, -9466, 0, -546, -2912, 0, 3094, -2366, 0, 0, -18386, 0, 0, -19296, 0, 1092, -14381, 0, 0, 0, 0, 0, -16748, 0, 0, -17658, 0, 0, -15109, 0, 0,
  0, 0, 0, -16566, 0, 0, -18204, 0, 0, -14381, 0, 0, 0, 0, 0, -19478, 0, 0, -18386, 0, -4004, -12925, 0, 0, -546, 0, 0, -3276, 0, 0, 0, 0, 0, 0, 0, 0,
  -6553, 0, 0, -4369, 0, -546, -3458, 0, 0, 0, 0, 0, -5461, 0, 0, -5643, 0, 0, -3458, 0, 0, 0, 0, 0, -6007, 0, 0, -4551, 0, 546, -2548, 0, 0, 0, 0, 0,
  -6189, 0, -1092, -3094, 0, 0, -2548, 0, 0, 0, 0, 0, -6007, 0, 0, -728, 0, 0, 0, 0, 0, -5097, 0, 0, -2184, 0, 0, -2002, 0, 0, 0, 0, 0, -5825, 0, 0,
  -2912, 0, 0, -728, 0, 0, 0, 0, 0, -4915, 0, 0, -4187, 0, 0, -1274, 0, 0, 0, 0, 0, -6553, 0, -364, -4369, 0, -546, -1456, 0, 0, 0
};
static const Vector4 data_5400 = {.x = 0.3f, .y = 0.5f, .z = 0.4f, .w = 0.4f};
static const int32_t data_5410[57] = {
  0, -728, -4205, 0, 2366, 928, 0, 728, -2184, 0, 0, -2639, 0, 0, -3695, 0, 0, -9193, 0, 0, 0, 0, 0, -4004, 0, 0, -8574, 0, 0, -6917, 0, 0, 0, 0, 0,
  -1729, 0, 0, -8701, 0, 0, -4096, 0, 47, -455, 0, -728, -3313, 0, -364, -5570, 0, -1638, -2494, 0, -254, -1310
};
static const int32_t data_54F4[57] = {
  0, 0, 0, 0, 0, 1456, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2184, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1456, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, -2912, 0, 0, 0, 0
};
static const int32_t data_55D8[57] = {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2002, 0, 0, 0, 0, 0, 0, 0, 0, -2184, 0, 0, 1274, 0, 0, 0, 0, 0, 0, 0, 0, -1456, 0, 0,
  546, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};
static const int32_t data_56BC[57] = {
  0, 0, 0, 0, 0, 0, 0, 1638, 0, 0, 0, -6189, 0, 0, -3822, 0, 0, -2366, 0, 0, 3640, 0, 0, -4187, 0, 0, -2730, 0, 0, -910, 0, 0, 2730, 0, 0, -728, 0, 0,
  -1456, 0, 0, 0, 0, 0, 2912, 0, 0, -1638, 0, 0, -364, 0, 0, 0, 0, 728, 3094
};
static const int32_t data_57A0[57] = {
  0, 0, -2184, 0, 0, -2184, 0, 0, -1456, 0, 0, -1638, 0, 0, -1638, 0, 0, -1456, 0, 0, 0, 0, 0, -2184, 0, 0, -2184, 0, 0, -1456, 0, 0, 0, 0, 0, -2912,
  0, 0, -2184, 0, 0, -2184, 0, 0, 0, 0, 0, -2184, 0, 0, -2184, 0, -1456, -2912, 0, 0, 0
};
static const int32_t data_5884[57] = {
  0, 0, 0, 0, 0, 0, 0, -1820, 0, 0, 0, 0, 0, 0, -364, 0, 0, -1092, 0, 0, 0, 0, 0, 0, 0, 0, -546, 0, 0, -364, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -364, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};
static const int32_t data_5968[57] = {
  0, 0, 0, 0, 0, -1820, 0, 0, 0, 0, 0, -1456, 0, 0, 0, 0, 0, 546, 0, 0, 0, 0, 0, -1274, 0, 0, -3822, 0, 0, -364, 0, 0, 0, 0, 0, -728, 0, 0, -1638, 0,
  0, -1638, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};
static const int32_t data_5A4C[57] = {
  0, -1274, 0, 0, 0, -2366, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -364, 0, 0, -364, 0, 0, 0, 0, 0, -546, 0, 0, -546, 0, 0,
  -1092, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};
static const int32_t data_5B30[57] = {
  0, 0, -1092, 0, 0, 4004, 0, -1820, 910, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -364, 0, 0, -364, 0, 0, 0, 0, 0, -546, 0, 0, -546, 0, 0,
  -1092, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};
static const int32_t data_5C14[57] = {
  0, -728, -3094, 0, 910, -2366, 0, 0, 0, 0, 0, -2184, 0, 0, -3458, 0, 0, -4733, 0, 0, 0, 0, 0, -2366, 0, 0, -3094, 0, 0, -2002, 0, 0, 0, 0, 0, -1638,
  0, 0, -910, 0, 0, -1820, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2548, 0, 0, 0
};
static const char data_5CF8[12] __attribute__((scn_string_literal)) = "bgm013.snd";
static const Vector3i data_5D04 = {.x = 10194, .y = 0, .z = 0};
static const Vector3i data_5D10 = {.x = 0, .y = -1456, .z = 0};
static const int32_t data_5D1C[10] = {0, 63715, 63715, 0, 4915, 0, 0, 6189, 0, 0};
static const int32_t data_5D44[10] = {0, -63715, -63715, 0, -4915, 0, 0, -6189, 0, 0};
static const Vector3 data_5D6C = {.x = 0.091f, .y = -0.019f, .z = 0.0107f};
static const Vector3i data_5D78 = {.x = 25486, .y = 4551, .z = 18204};
static const int32_t data_5D84[10] = {0, 61895, 0, 0, 0, 0, 0, 4733, 0, 0};
static const int32_t data_5DAC[10] = {62259, 0, 0, 0, 59710, 0, 0, 61895, 0, 0};
static const Vector3i data_5DD4 = {.x = 2184, .y = -546, .z = 0};
static const Vector3i data_5DE0 = {.x = 0, .y = -910, .z = 0};
static const FogColourData data_5DEC = {.red = 4.1f, .green = 50.0f, .blue = 0.82f, .colour = 0xFFFFD9D7U};
static const char data_5DFC[6] __attribute__((scn_string_literal)) = "A0165";
static const char data_5E02[11] __attribute__((scn_string_literal)) = "M_0165.BIN";
static const char data_5E0D[11] __attribute__((scn_string_literal)) = "bgm073.snd";
static const char data_5E18[13] __attribute__((scn_string_literal)) = "a1_asiar.snd";
static const char data_5E25[6] __attribute__((scn_string_literal)) = "A0165";
static const char data_5E2B[6] __attribute__((scn_string_literal)) = "A0174";
static const char data_5E31[11] __attribute__((scn_string_literal)) = "M_0174.BIN";
static const char data_5E3C[13] __attribute__((scn_string_literal)) = "HMOT0151.BIN";
static const char data_5E49[11] __attribute__((scn_string_literal)) = "bgm063.snd";
static const char data_5E54[13] __attribute__((scn_string_literal)) = "a1_hijym.snd";
static const char data_5E61[6] __attribute__((scn_string_literal)) = "A0174";
static const char data_5E67[6] __attribute__((scn_string_literal)) = "A0174";
static const char data_5E6D[11] __attribute__((scn_string_literal)) = "M_0174.BIN";
static const char data_5E78[13] __attribute__((scn_string_literal)) = "HMOT0151.BIN";
static const char data_5E85[11] __attribute__((scn_string_literal)) = "bgm063.snd";
static const char data_5E90[13] __attribute__((scn_string_literal)) = "a1_hijym.snd";
static const char data_5E9D[6] __attribute__((scn_string_literal)) = "A0174";
static const char data_5EA3[6] __attribute__((scn_string_literal)) = "01ETU";
static const char data_5EA9[12] __attribute__((scn_string_literal)) = "M_01ETU.BIN";
static const char data_5EB5[11] __attribute__((scn_string_literal)) = "bgm073.snd";
static const char data_5EC0[13] __attribute__((scn_string_literal)) = "a1_etuko.snd";
static const char data_5ECD[6] __attribute__((scn_string_literal)) = "01ETU";
static const char data_5ED3[6] __attribute__((scn_string_literal)) = "01ETU";
static const char data_5ED9[12] __attribute__((scn_string_literal)) = "M_01ETU.BIN";
static const char data_5EE5[11] __attribute__((scn_string_literal)) = "bgm073.snd";
static const char data_5EF0[13] __attribute__((scn_string_literal)) = "a1_etuko.snd";
static const char data_5EFD[7] __attribute__((scn_string_literal)) = "01ETU";
