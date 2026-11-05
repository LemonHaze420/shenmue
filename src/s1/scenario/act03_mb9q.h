#pragma once

#include "scenario.h"
#include "motion/motion.h"

static void script_0030(void);
static void script_0039(void);
static void script_0048(int32_t arg0);
static void script_00D5(int32_t arg0);
static int32_t script_0125(void);
static void script_0153(int32_t arg0);
static void script_0181(void);
static void script_01B5(int32_t arg0);
static void script_01E9(int32_t arg0);
static void script_0207(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_0241(int32_t arg0, int32_t arg1);
static void script_0275(void);
static void script_036D(int32_t arg0, int32_t arg1);
static int32_t script_039D(int32_t arg0, int32_t arg1);
static int32_t script_03C1(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4);
static int32_t script_0498(int32_t arg0, int32_t arg1, int32_t arg2, float arg3, int32_t arg4, int32_t arg5);
static int32_t script_052F(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4);
static int32_t script_0591(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_05ED(int32_t arg0, float arg1, int32_t arg2);
static void script_064A(int32_t arg0);
static int32_t script_0666(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, void *arg4, int32_t arg5);
static void script_088D(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_08C4(int32_t arg0);
static void script_08D4(void);
static void script_08F4(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_092E(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_097A(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4);
static void script_09C3(int32_t arg0);
static int32_t script_09D6(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_0A39(int32_t arg0, int32_t arg1);
static int32_t script_0AFA(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_0BE8(void);
static void script_0CD6(int32_t arg0, int32_t arg1);
static void script_0CFF(int32_t arg0, int32_t arg1, float arg2, float arg3, float arg4);
static void script_0D8E(int32_t arg0, int32_t arg1, const Vector3 *arg2);
static void script_0DC1(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static int32_t script_0DDB(int32_t arg0, int32_t arg1, float arg2, float arg3, float arg4, int32_t arg5);
static void script_0E06(int32_t arg0, int32_t arg1, float arg2, float arg3, float arg4, float arg5);
static void script_0E86(int32_t arg0, int32_t arg1, float arg2);
static void script_0EDA(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_0EF0(int32_t arg0, int32_t arg1, float arg2, int32_t arg3);
static void script_0F17(int32_t arg0, float arg1);
static void script_0F40(int32_t arg0, float arg1, float arg2, float arg3);
static void script_0F94(int32_t arg0, int32_t arg1, int32_t arg2);
static int32_t script_0FA8(int32_t arg0, float arg1, int32_t arg2);
static void script_0FCD(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4);
static int32_t script_0FE5(int32_t arg0, float arg1, float arg2, float arg3, int32_t arg4);
static void script_100E(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4);
static int32_t script_1026(int32_t arg0, float arg1, float arg2, float arg3, int32_t arg4);
static void script_107B(int32_t arg0, int32_t arg1);
static void script_1160(int32_t arg0, int32_t arg1);
static void script_1249(int32_t arg0);
static void script_1267(void);
static void script_12AF(void);
static void script_12CA(void);
static void script_12E4(const char *arg0, const char *arg1);
static void script_1322(const char *arg0);
static void script_1343(int32_t arg0);
static void script_135A(int32_t arg0);
static void script_13E0(int32_t arg0, int32_t arg1);
static void script_1403(int32_t arg0);
static int32_t script_1643(int32_t arg0, int32_t arg1, int32_t arg2);
static int32_t script_2B73(int32_t arg0, int32_t arg1, int32_t arg2);
static int32_t script_2C83(int32_t arg0, int32_t arg1, int32_t arg2);
static int32_t script_2D93(int32_t arg0, int32_t arg1, int32_t arg2);
static int32_t script_2EA5(int32_t arg0, int32_t arg1, int32_t arg2);
static int32_t script_2FEF(int32_t arg0, int32_t arg1, int32_t arg2);
static int32_t script_3100(int32_t arg0);
static int32_t script_323F(int32_t arg0, int32_t arg1, float arg2, int32_t arg3, const void *arg4);
static void script_32D5(int32_t arg0, float arg1, const void *arg2);
static void script_3370(int32_t arg0, float arg1);
static void script_34BC(int32_t arg0, int32_t arg1, int32_t arg2);
static int32_t script_34D0(int32_t arg0, float arg1, int32_t arg2);
static void script_34F5(int32_t arg0);
static void script_35D8(int32_t arg0, int32_t arg1);
static int32_t script_35EA(int32_t arg0, int32_t arg1);
static int32_t script_360D(void);
static int32_t script_367C(void);
static void script_36B2(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_36C8(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_36EC(int32_t arg0, int32_t arg1);
static int32_t script_36FE(int32_t arg0, int32_t arg1);
static void script_3932(int32_t arg0, int32_t arg1);
static void script_3943(int32_t arg0, int32_t arg1);
static void script_3C93(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4);
static int32_t script_3CAB(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4);
static void script_3CD2(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4);
static int32_t script_3CEA(int32_t arg0, int32_t arg1, float arg2, int32_t arg3, int32_t arg4);
static void script_3D65(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_3D7B(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_3DB7(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5, int32_t arg6);
static int32_t script_3DD3(int32_t arg0, int32_t arg1, int32_t arg2, float arg3, int32_t arg4, int32_t arg5, int32_t arg6);
static void script_3E29(void);
static int32_t script_3F24(int32_t arg0, int32_t arg1);
static int32_t script_4A6A(int32_t arg0, int32_t arg1);
static void script_6298(int32_t arg0, int32_t arg1);
static void script_64A7(int32_t arg0, int32_t arg1);
static void script_68DE(int32_t arg0, int32_t arg1);
static void script_72F6(int32_t arg0);
static void script_7525(int32_t arg0, int32_t arg1);
void script_7981(void);
static void script_79C9(int32_t arg0);
static void script_7C5E(void);
static int32_t script_7CB7(int32_t arg0, int32_t arg1);
static int32_t script_7E69(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_7F4D(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_8075(int32_t arg0, int32_t arg1);
static int32_t script_80D6(int32_t arg0, int32_t arg1, int32_t arg2);

static int32_t task_0;
static int32_t task_4;
static int32_t task_8;
static int32_t task_C;
static int32_t task_10;
static int32_t task_14;
static int32_t task_18[8];
static int32_t task_38[8];
static int32_t task_58[8];
static int32_t task_78[8];
static int32_t task_98[8];
static int32_t task_B8;
static int32_t task_BC[8];
static int32_t task_DC[8];
static int32_t task_FC[8];
static int32_t task_11C[8];
static int32_t task_13C;
static int32_t task_140;
static int32_t task_144;
static int32_t task_148;
static int32_t task_14C;
static int32_t task_150;
static int32_t task_154[8];
static float task_174[8][3];
static float task_1D4[8];
static float task_1F4;
static float task_1F8;
static float task_1FC;
static float task_200[8][3];
static int32_t task_260[8][3];
static float task_2C0[8];
static int32_t task_2E0[8];
static int32_t task_300;
static int32_t task_304;
static int32_t task_308[8];
static int32_t task_328[8];
static int32_t task_348[8];
static int32_t task_368[8];
static int32_t task_388[8];
static float task_3A8[8];
static int32_t task_3C8;
static const int32_t data_81B4[399] = {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -10922, 0, 0, -10558, 0, 1456, 0, 0, 0, -18022, 0, 0, -17112, 0, 0, -18386, 0, 0, 0, 0, 0, -12196, 0, 0, -18204, 0, 0,
  -16748, 0, 0, 0, 0, 0, -11832, 0, 0, -18386, 0, 0, -15109, 0, 0, 0, 0, 910, -12014, 0, -2184, -16748, 0, -2184, -10012, 0, 0, 0, 0, -364, -11650,
  -2912, -364, -4369, 1638, 910, 182, 0, 0, -16019, 0, 182, -23119, 0, 546, -18932, 0, 0, -1092, 0, 0, -16384, 0, 0, -21845, 0, 0, -18750, 0, 0, -546,
  0, 0, -21481, 0, 0, -21299, 0, 0, -18568, 0, 0, -728, 0, 910, -18750, 0, 182, -18750, 0, -4004, -18204, 0, 0, -728, 0, -364, -14017, -2912, -364,
  -6735, 1638, 910, 2548, 0, 0, -18568, 0, 182, -25668, 0, 546, -21481, 0, 0, -3640, 0, 0, -18932, 0, 0, -24393, 0, 0, -21299, 0, 0, -3094, 0, 0,
  -24029, 0, 0, -23847, 0, 0, -21117, 0, 0, -3276, 0, 910, -21299, 0, 182, -21299, 0, -4004, -20753, 0, 0, -3276, 0, 0, -5461, 0, -2366, -1092, 0,
  -182, 1092, 0, -546, -6735, 0, 0, -4004, 0, 0, -2912, 0, -182, 1274, 0, 0, -13289, 0, 0, -5279, 0, 0, -8009, 0, 0, 1456, 0, 182, -8738, 0, 0, -6007,
  0, 0, -8738, 0, 182, 1638, 0, -728, -5279, 0, -1274, -6735, 0, -910, -8738, 0, 0, 1638, 0, 0, -5461, 0, -2366, -1092, 0, -182, 1092, 0, -546,
  -13289, 0, 0, -5643, 0, 0, -8009, 0, -182, 1274, 0, 0, -11832, 0, 0, -5279, 0, 0, -8009, 0, 0, 1456, 0, 182, -8738, 0, 0, -6007, 0, 0, -8738, 0,
  182, 1638, 0, -728, -5279, 0, -1274, -6735, 0, -910, -8738, 0, 0, 1092, 0, 0, -5461, 0, -2366, -1092, 0, -182, 1092, 0, -546, -3640, 0, 0, -2366, 0,
  0, -1456, 0, -182, 1274, 0, 0, -3822, 0, 0, -2730, 0, 0, -1456, 0, 0, 1456, 0, 182, -3822, 0, 0, -2912, 0, 0, -1638, 0, 182, 1638, 0, -728, -3640,
  0, -910, -2730, 0, -546, -1638, 0, 0, 1092
};
static const int32_t data_87F0[342] = {
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
static const int32_t data_8D48[57] = {
  0, 0, -7281, 0, 1456, -1274, 0, 1638, -1456, 0, 0, -18204, 0, 0, -16930, 0, 0, -12379, 0, 0, 0, 0, 0, -12196, 0, 0, -18204, 0, 0, -13289, 0, 0, 0,
  0, 0, -11832, 0, 0, -18386, 0, 0, -13835, 0, 0, 0, 0, 910, -12014, 0, -2184, -16748, 0, -2002, -13835, 0, 0, 364
};
static const int32_t data_8E2C[57] = {
  0, 0, -910, 0, 3822, 2366, 0, 3822, 910, 0, 0, -4915, 0, 0, -7099, 0, 0, -3276, 0, 0, 0, 0, 0, -4369, 0, 0, -7099, 0, 0, -2730, 0, 0, 0, 0, 0,
  -1274, 0, 0, -10012, 0, 0, -2730, 0, 0, 364, 0, 910, -9102, 0, -2184, -4915, 0, -2002, -3276, 0, 0, 364
};
static const Vector3i data_8F10 = {.x = 1638, .y = 2366, .z = -2912};
static const Vector3i data_8F1C = {.x = 0, .y = 0, .z = 0};
static const uint32_t data_8F28 = 256;
static const int32_t data_8F2C[203] = {
  20, 20, 4, 2, -1, -1, -1, 1, -1, 3, 5, 512, 20, 20, 4, 3, -1, 19, -1, -1, -1, 7, 9, 1, 20, 20, 4, 1, -1, 33, -1, -1, 11, 14, 17, 1, 20, 20, 4, 5,
  -1, 55, -1, -1, -1, 14, 20, 256, 20, 16, 4, 3, -1, 2, -1, 23, -1, 25, 27, 512, 20, 20, 4, 3, -1, -1, -1, -1, -1, 29, 31, 512, 20, 20, 4, 4, -1, 5,
  -1, 33, -1, 36, 39, 256, 20, 20, 4, 4, -1, 9, -1, -1, -1, 42, 45, 512, 20, 20, 4, 5, -1, 15, -1, -1, -1, 50, 48, 512, 20, 16, 4, 4, -1, -1, 18, -1,
  52, 54, 56, 512, 20, 20, 4, 3, -1, 14, -1, -1, -1, 58, 60, 512, 20, 20, 4, 0, -1, -1, 10, -1, 62, 65, 68, 256, 10, 10, 4, 0, -1, -1, -1, -1, -1, 71,
  74, 64, 20, 10, 4, 3, -1, -1, 12, -1, 83, 85, 87, 16, 10, 10, 4, 3, -1, -1, -1, -1, -1, 89, 91, 256, 20, 16, 4, 3, -1, -1, -1, -1, -1, 93, 95, 512,
  7, 7, 4, 3, -1, -1, -1, -1, -1, 97, 99
};
static const uint32_t data_9258 = 158;
static const int32_t data_925C[6] = {152, 153, 154, 155, 156, 157};
static const uint32_t data_9274 = 157;
static const int32_t data_9278[5] = {155, 154, 152, 156, 153};
static const uint32_t data_928C = 165;
static const int32_t data_9290[17] = {164, 163, 162, 161, 160, 159, 158, 157, 156, 155, 154, 153, 152, 169, 168, 167, 166};
static const uint8_t data_92D4[4] = {0};
static const int32_t data_92D8[36] = {
  1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36
};
static const char data_9368[6] __attribute__((scn_string_literal)) = "MPK00";
static const char data_936E[] __attribute__((scn_string_literal)) = "PPDN";
static const char data_9373[6] __attribute__((scn_string_literal)) = "MPK00";
static const char data_9379[] __attribute__((scn_string_literal)) = "PDN2";
static const char data_937E[6] __attribute__((scn_string_literal)) = "MPK00";
static const char data_9384[] __attribute__((scn_string_literal)) = "DRDN";
static const uint8_t data_9389[3] = {0};
static Vector3 data_938C = {.x = 1000.0f, .y = 0.0f, .z = 0.0f};
static const char data_9398[6] __attribute__((scn_string_literal)) = "MPK00";
static const char data_939E[] __attribute__((scn_string_literal)) = "IKDN";
static const char data_93A3[6] __attribute__((scn_string_literal)) = "MPK00";
static const char data_93A9[] __attribute__((scn_string_literal)) = "KADY";
static const uint8_t data_93AE[2] = {0};
static const Vector3 data_93B0 = {.x = 0.0f, .y = 0.02f, .z = 0.0f};
static const FogColourData data_93BC = {.red = 0.1f, .green = 40.0f, .blue = 0.18f, .colour = 0};
static const Vector4 data_93CC = {.x = 0.5f, .y = 0.5f, .z = 0.5f, .w = 0.5f};
static const uint32_t data_93DC = 0x4030A3D7;
typedef struct Row_93E0 { float w0; float w1; float w2; int32_t w3; } Row_93E0;
static const Row_93E0 data_93E0[2] = {
  {0.0f, 11.28f, 0.0f, 1},
  {8.03f, 0.0f, 9.5f, 0}
};
static const int32_t data_9400[2] = {2, -11062528};
static const uint32_t data_9408 = 0x40E3D70A;
typedef struct Row_940C { float w0; float w1; float w2; int32_t w3; } Row_940C;
static const Row_940C data_940C[2] = {
  {0.0f, 10.0f, 0.0f, 1},
  {13.85f, 0.0f, 7.35f, 0}
};
static const int32_t data_942C[2] = {2, -11062528};
static const uint32_t data_9434 = 0x4122147B;
static const float data_9438[3] = {0.0f, 7.58f, 0.0f};
static const int32_t data_9444 = 1;
static const float data_9448[4] = {9.86f, 0.0f, 9.77f, 1.5f};
static const int32_t data_9458 = 2;
static const float data_945C[4] = {7.48f, 0.0f, 10.21f, 0.0f};
static const int32_t data_946C[2] = {3, -11062528};
static const char data_9474[10] __attribute__((scn_string_literal)) = "E1022C004";
static const char data_947E[10] __attribute__((scn_string_literal)) = "E1022B009";
static const char data_9488[10] __attribute__((scn_string_literal)) = "E1022B010";
static const char data_9492[10] __attribute__((scn_string_literal)) = "E1022B011";
static const char data_949C[10] __attribute__((scn_string_literal)) = "E1022D004";
static const char data_94A6[10] __attribute__((scn_string_literal)) = "E1022A006";
static const char data_94B0[10] __attribute__((scn_string_literal)) = "E1022D002";
static const char data_94BA[10] __attribute__((scn_string_literal)) = "E1022D003";
static const char data_94C4[10] __attribute__((scn_string_literal)) = "E1022B012";
static const char data_94CE[10] __attribute__((scn_string_literal)) = "E1022B013";
static const char data_94D8[10] __attribute__((scn_string_literal)) = "E1022C005";
static const char data_94E2[10] __attribute__((scn_string_literal)) = "E1022B014";
static const char data_94EC[10] __attribute__((scn_string_literal)) = "E1022B015";
static const char data_94F6[10] __attribute__((scn_string_literal)) = "E1022C002";
static const char data_9500[10] __attribute__((scn_string_literal)) = "E1022C003";
static const char data_950A[10] __attribute__((scn_string_literal)) = "E1022F001";
static const char data_9514[10] __attribute__((scn_string_literal)) = "E1022F002";
static const char data_951E[10] __attribute__((scn_string_literal)) = "E1022G001";
static const char data_9528[10] __attribute__((scn_string_literal)) = "E1022G002";
static const char data_9532[10] __attribute__((scn_string_literal)) = "E1022I001";
static const char data_953C[10] __attribute__((scn_string_literal)) = "E1022I002";
static const char data_9546[10] __attribute__((scn_string_literal)) = "E1022J001";
static const char data_9550[10] __attribute__((scn_string_literal)) = "E1022J002";
static const char data_955A[10] __attribute__((scn_string_literal)) = "E1022K001";
static const char data_9564[10] __attribute__((scn_string_literal)) = "E1022K002";
static const char data_956E[9] __attribute__((scn_string_literal)) = "オート０";
static const char data_9577[9] __attribute__((scn_string_literal)) = "オート１";
static const char data_9580[7] __attribute__((scn_string_literal)) = "開始０";
static const char data_9587[7] __attribute__((scn_string_literal)) = "成功０";
static const char data_958E[7] __attribute__((scn_string_literal)) = "失敗０";
static const char data_9595[7] __attribute__((scn_string_literal)) = "死亡０";
static const char data_959C[7] __attribute__((scn_string_literal)) = "開始１";
static const char data_95A3[7] __attribute__((scn_string_literal)) = "成功１";
static const char data_95AA[7] __attribute__((scn_string_literal)) = "失敗１";
static const char data_95B1[7] __attribute__((scn_string_literal)) = "開始２";
static const char data_95B8[7] __attribute__((scn_string_literal)) = "成功２";
static const char data_95BF[7] __attribute__((scn_string_literal)) = "失敗２";
static const char data_95C6[7] __attribute__((scn_string_literal)) = "開始３";
static const char data_95CD[7] __attribute__((scn_string_literal)) = "成功３";
static const char data_95D4[7] __attribute__((scn_string_literal)) = "失敗３";
static const char data_95DB[7] __attribute__((scn_string_literal)) = "開始４";
static const char data_95E2[7] __attribute__((scn_string_literal)) = "成功４";
static const char data_95E9[7] __attribute__((scn_string_literal)) = "失敗４";
static const char data_95F0[7] __attribute__((scn_string_literal)) = "開始５";
static const char data_95F7[7] __attribute__((scn_string_literal)) = "成功５";
static const char data_95FE[7] __attribute__((scn_string_literal)) = "失敗５";
static const char data_9605[7] __attribute__((scn_string_literal)) = "開始６";
static const char data_960C[7] __attribute__((scn_string_literal)) = "成功６";
static const char data_9613[7] __attribute__((scn_string_literal)) = "失敗６";
static const char data_961A[7] __attribute__((scn_string_literal)) = "開始７";
static const char data_9621[7] __attribute__((scn_string_literal)) = "成功７";
static const char data_9628[7] __attribute__((scn_string_literal)) = "失敗７";
static const char data_962F[7] __attribute__((scn_string_literal)) = "開始８";
static const char data_9636[7] __attribute__((scn_string_literal)) = "成功８";
static const char data_963D[7] __attribute__((scn_string_literal)) = "失敗８";
static const char data_9644[7] __attribute__((scn_string_literal)) = "開始９";
static const char data_964B[7] __attribute__((scn_string_literal)) = "成功９";
static const char data_9652[7] __attribute__((scn_string_literal)) = "失敗９";
static const char data_9659[9] __attribute__((scn_string_literal)) = "開始１０";
static const char data_9662[9] __attribute__((scn_string_literal)) = "成功１０";
static const char data_966B[9] __attribute__((scn_string_literal)) = "失敗１０";
static const char data_9674[9] __attribute__((scn_string_literal)) = "開始１１";
static const char data_967D[9] __attribute__((scn_string_literal)) = "成功１１";
static const char data_9686[9] __attribute__((scn_string_literal)) = "失敗１１";
static const char data_968F[9] __attribute__((scn_string_literal)) = "開始１２";
static const char data_9698[9] __attribute__((scn_string_literal)) = "成功１２";
static const char data_96A1[9] __attribute__((scn_string_literal)) = "失敗１２";
static const char data_96AA[9] __attribute__((scn_string_literal)) = "開始１３";
static const char data_96B3[9] __attribute__((scn_string_literal)) = "成功１３";
static const char data_96BC[9] __attribute__((scn_string_literal)) = "失敗１３";
static const char data_96C5[9] __attribute__((scn_string_literal)) = "開始１４";
static const char data_96CE[9] __attribute__((scn_string_literal)) = "成功１４";
static const char data_96D7[9] __attribute__((scn_string_literal)) = "失敗１４";
static const char data_96E0[6] __attribute__((scn_string_literal)) = "MPK00";
static const char data_96E6[] __attribute__((scn_string_literal)) = "PPDN";
static const char data_96EB[17] __attribute__((scn_string_literal)) = "PRESS A BUTTON";
static Vector3 data_96FC = {.x = 1000.0f, .y = 0.0f, .z = 0.0f};
static const char data_9708[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_970D[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_9712[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_9717[13] __attribute__((scn_string_literal)) = "a1_qrenq.snd";
static const char data_9724[11] __attribute__((scn_string_literal)) = "bgm035.snd";
static const char data_972F[11] __attribute__((scn_string_literal)) = "bgm067.snd";
static const char data_973A[13] __attribute__((scn_string_literal)) = "e1minqte.snd";
static const char data_9747[6] __attribute__((scn_string_literal)) = "019RN";
static const char data_974D[6] __attribute__((scn_string_literal)) = "E1022";
static const char data_9753[6] __attribute__((scn_string_literal)) = "019RN";
static const char data_9759[6] __attribute__((scn_string_literal)) = "MPK00";
static const char data_975F[13] __attribute__((scn_string_literal)) = "seqdata1.bin";
static const char data_976C[6] __attribute__((scn_string_literal)) = "MPK00";
static const char data_9772[6] __attribute__((scn_string_literal)) = "E1022";
static const Vector3 data_9778 = {.x = 0.0f, .y = 0.0f, .z = 0.0f};
static const Vector3i data_9784 = {.x = 0, .y = 0, .z = 0};
