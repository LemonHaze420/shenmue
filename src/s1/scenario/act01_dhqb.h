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
static void script_039D(int32_t arg0, float arg1, float arg2, float arg3);
static void script_03C5(int32_t arg0, float arg1, float arg2, float arg3);
static void script_0419(int32_t arg0, int32_t arg1, int32_t arg2);
static int32_t script_0469(int32_t arg0);
static void script_0510(int32_t arg0, int32_t arg1);
static int32_t script_0533(int32_t arg0);
static void script_0566(int32_t arg0, int32_t arg1);
static void script_0589(int32_t arg0, int32_t arg1);
static void script_05E8(int32_t arg0);
static int32_t script_05FD(int32_t arg0, int32_t arg1);
static int32_t script_0621(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4);
static int32_t script_06F8(int32_t arg0, int32_t arg1, int32_t arg2, float arg3, int32_t arg4, int32_t arg5);
static int32_t script_078F(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4);
static int32_t script_07F1(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_084D(int32_t arg0, float arg1, int32_t arg2);
static void script_08AA(int32_t arg0);
static int32_t script_08C6(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, void *arg4, int32_t arg5);
static void script_0AED(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_0B24(int32_t arg0);
static void script_0B34(void);
static void script_0B54(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_0B8E(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_0BDA(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4);
static void script_0C23(int32_t arg0);
static int32_t script_0C36(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_0C99(int32_t arg0, int32_t arg1);
static int32_t script_0D5A(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_0E48(void);
static void script_0F36(int32_t arg0, int32_t arg1);
static void script_0F5F(int32_t arg0, int32_t arg1, float arg2, float arg3, float arg4);
static void script_0FEE(int32_t arg0, int32_t arg1, const Vector3 *arg2);
static void script_1021(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static int32_t script_103B(int32_t arg0, int32_t arg1, float arg2, float arg3, float arg4, int32_t arg5);
static void script_1066(int32_t arg0, int32_t arg1, float arg2, float arg3, float arg4, float arg5);
static void script_10E6(int32_t arg0, int32_t arg1, float arg2);
static void script_113A(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_1150(int32_t arg0, int32_t arg1, float arg2, int32_t arg3);
static void script_1177(int32_t arg0, float arg1);
static void script_11A0(int32_t arg0, float arg1, float arg2, float arg3);
static void script_11F4(int32_t arg0, int32_t arg1, int32_t arg2);
static int32_t script_1208(int32_t arg0, float arg1, int32_t arg2);
static void script_122D(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4);
static int32_t script_1245(int32_t arg0, float arg1, float arg2, float arg3, int32_t arg4);
static void script_126E(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4);
static int32_t script_1286(int32_t arg0, float arg1, float arg2, float arg3, int32_t arg4);
static void script_12DB(int32_t arg0, int32_t arg1);
static void script_13C0(int32_t arg0, int32_t arg1);
static void script_14A9(int32_t arg0);
static void script_14C7(void);
static void script_150F(void);
static void script_152A(void);
static void script_1544(const char *arg0, const char *arg1);
static void script_1582(const char *arg0);
static void script_15A3(int32_t arg0);
static void script_15BA(int32_t arg0);
static void script_1640(int32_t arg0, int32_t arg1);
static int32_t script_1663(int32_t arg0, int32_t arg1, int32_t arg2);
static int32_t script_2088(int32_t arg0, int32_t arg1, int32_t arg2);
static int32_t script_21FA(int32_t arg0, int32_t arg1, int32_t arg2);
static int32_t script_230E(int32_t arg0, int32_t arg1, int32_t arg2);
static int32_t script_241F(int32_t arg0, int32_t arg1, int32_t arg2);
static int32_t script_2579(int32_t arg0, int32_t arg1, int32_t arg2);
static int32_t script_268B(int32_t arg0, int32_t arg1, int32_t arg2);
static int32_t script_279D(int32_t arg0, int32_t arg1, int32_t arg2);
static int32_t script_28C7(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_2A1F(void);
static void script_2A66(void);
static int32_t script_2A84(int32_t arg0, int32_t arg1);
static int32_t script_2B87(int32_t arg0, int32_t arg1, float arg2, int32_t arg3, const void *arg4);
static void script_2C1D(int32_t arg0, float arg1, const void *arg2);
static void script_2CAC(int32_t arg0, float arg1);
static void script_2D39(int32_t arg0, int32_t arg1, int32_t arg2);
static int32_t script_2D4D(int32_t arg0, float arg1, int32_t arg2);
static void script_2D72(int32_t arg0);
static void script_2DBF(int32_t arg0, int32_t arg1);
static int32_t script_2DD1(int32_t arg0, int32_t arg1);
static int32_t script_2DF4(void);
static int32_t script_2E40(void);
static void script_2E70(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_2E86(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_2EAA(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_2EC0(float arg0, float arg1, float arg2, int32_t arg3);
static void script_2EE5(int32_t arg0, int32_t arg1);
static int32_t script_2EF7(int32_t arg0, int32_t arg1);
static void script_33A1(int32_t arg0, int32_t arg1);
static void script_33B2(int32_t arg0, int32_t arg1);
static void script_356A(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_3580(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_35BC(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4);
static int32_t script_35D4(int32_t arg0, int32_t arg1, float arg2, int32_t arg3, int32_t arg4);
static void script_364F(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5, int32_t arg6);
static int32_t script_366B(int32_t arg0, int32_t arg1, int32_t arg2, float arg3, int32_t arg4, int32_t arg5, int32_t arg6);
static void script_36C1(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4);
static int32_t script_36D9(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4);
static void script_3700(void);
static int32_t script_37B3(int32_t arg0, int32_t arg1);
static int32_t script_3B03(int32_t arg0, int32_t arg1);
static void script_46B3(int32_t arg0, int32_t arg1);
static void script_4E64(int32_t arg0, int32_t arg1);
void script_53E0(void);
static void script_5428(int32_t arg0);
static void script_565A(void);
static int32_t script_56A9(int32_t arg0, int32_t arg1);
static int32_t script_583B(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_5914(int32_t arg0, int32_t arg1);
static int32_t script_595A(int32_t arg0, int32_t arg1, int32_t arg2);
static int32_t script_5A5E(int32_t arg0, int32_t arg1, int32_t arg2);
static int32_t script_5B57(int32_t arg0, int32_t arg1);

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
static int32_t task_28[7];
static int32_t task_44[7];
static int32_t task_60[7];
static int32_t task_7C[7];
static int32_t task_98[7];
static int32_t task_B4;
static int32_t task_B8[7];
static int32_t task_D4[7];
static int32_t task_F0[7];
static int32_t task_10C[7];
static int32_t task_128;
static int32_t task_12C;
static int32_t task_130;
static int32_t task_134;
static int32_t task_138;
static int32_t task_13C;
static int32_t task_140[7];
static float task_15C[7][3];
static float task_1B0[7];
static float task_1CC;
static float task_1D0;
static float task_1D4;
static float task_1D8[7][3];
static int32_t task_22C[7][3];
static float task_280[7];
static int32_t task_29C[7];
static int32_t task_2B8;
static int32_t task_2BC;
static int32_t task_2C0;
static int32_t task_2C4[7];
static int32_t task_2E0[7];
static int32_t task_2FC[7];
static int32_t task_318[7];
static int32_t task_334[7];
static float task_350[7];
static int32_t task_36C;
static const int32_t data_5BA8[741] = {
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
static const uint32_t data_673C = 512;
static const int32_t data_6740[83] = {
  20, 18, 0, 0, -1, -1, -1, 1, -1, 3, 5, 512, 24, 18, 0, 0, 48, -1, -1, -1, -1, 7, 9, 256, 24, 18, 0, 0, -1, 28, -1, 11, -1, 13, 15, 512, 24, 18, 0,
  0, -1, -1, -1, -1, -1, 17, 19, 512, 24, 18, 0, 0, -1, 28, -1, 21, -1, 23, 25, 256, 24, 18, 0, 0, -1, -1, -1, -1, -1, 27, 29, 512, 24, 18, 0, 0, -1,
  17, -1, -1, -1, 31, 33
};
static const uint32_t data_688C = 198;
static const int32_t data_6890[46] = {
  152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180,
  181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197
};
static const uint32_t data_6948 = 152;
static const uint8_t data_694C[4] = {0};
static const int32_t data_6950[5] = {1, 2, 3, 4, 5};
static const uint32_t data_6964 = 152;
static const int32_t data_6968[4] = {153, 154, 155, 156};
static const uint8_t data_6978[4] = {0};
static const int32_t data_697C[5] = {1, 2, 3, 4, 5};
static const uint32_t data_6990 = 159;
static const int32_t data_6994[7] = {158, 157, 156, 155, 154, 153, 152};
static const uint32_t data_69B0 = 152;
static const int32_t data_69B4[10] = {153, 154, 155, 156, 157, 158, 159, 160, 161, 152};
static const char data_69DC[6] __attribute__((scn_string_literal)) = "MPK00";
static const char data_69E2[] __attribute__((scn_string_literal)) = "TBDN";
static const char data_69E7[6] __attribute__((scn_string_literal)) = "MPK00";
static const char data_69ED[] __attribute__((scn_string_literal)) = "CIDN";
static const char data_69F2[6] __attribute__((scn_string_literal)) = "MPK00";
static const char data_69F8[] __attribute__((scn_string_literal)) = "CB1D";
static const char data_69FD[6] __attribute__((scn_string_literal)) = "MPK00";
static const char data_6A03[] __attribute__((scn_string_literal)) = "CB2D";
static const char data_6A08[6] __attribute__((scn_string_literal)) = "MPK00";
static const char data_6A0E[] __attribute__((scn_string_literal)) = "CB3D";
static const char data_6A13[6] __attribute__((scn_string_literal)) = "MPK00";
static const char data_6A19[] __attribute__((scn_string_literal)) = "CB4D";
static const char data_6A1E[6] __attribute__((scn_string_literal)) = "MPK00";
static const char data_6A24[] __attribute__((scn_string_literal)) = "TIDN";
static const char data_6A29[6] __attribute__((scn_string_literal)) = "MPK00";
static const char data_6A2F[] __attribute__((scn_string_literal)) = "MKDN";
static const Vector3 data_6A34 = {.x = 0.0f, .y = 0.0f, .z = 0.0f};
static const char data_6A40[14] __attribute__((scn_string_literal)) = "scene/01/DHQB";
static const char data_6A4E[13] __attribute__((scn_string_literal)) = "MIKOWAVE.PVR";
static const char data_6A5B[14] __attribute__((scn_string_literal)) = "scene/01/DHQB";
static const char data_6A69[31] = "MIKOWAVE.MT6\000\000\000\000\000\000\000\315\314\314=\217\302\365=\315\314\314=";
static const uint32_t data_6A88 = 0x4030A3D7;
typedef struct Row_6A8C { float w0; float w1; float w2; int32_t w3; } Row_6A8C;
static const Row_6A8C data_6A8C[2] = {
  {0.0f, 11.28f, 0.0f, 1},
  {8.03f, 0.0f, 9.5f, 0}
};
static const int32_t data_6AAC[2] = {2, -11062528};
static const char data_6AB4[10] __attribute__((scn_string_literal)) = "E1030A001";
static const char data_6ABE[10] __attribute__((scn_string_literal)) = "E1030A002";
static const char data_6AC8[10] __attribute__((scn_string_literal)) = "E1030A003";
static const char data_6AD2[10] __attribute__((scn_string_literal)) = "E1030A004";
static const char data_6ADC[10] __attribute__((scn_string_literal)) = "E1030A005";
static const char data_6AE6[10] __attribute__((scn_string_literal)) = "E1030A006";
static const char data_6AF0[10] __attribute__((scn_string_literal)) = "E1030A007";
static const char data_6AFA[10] __attribute__((scn_string_literal)) = "E1030A008";
static const char data_6B04[10] __attribute__((scn_string_literal)) = "E1030A009";
static const char data_6B0E[10] __attribute__((scn_string_literal)) = "E1030A010";
static const char data_6B18[10] __attribute__((scn_string_literal)) = "E1030A011";
static const char data_6B22[10] __attribute__((scn_string_literal)) = "E1030A012";
static const char data_6B2C[10] __attribute__((scn_string_literal)) = "E1030A013";
static const char data_6B36[10] __attribute__((scn_string_literal)) = "E1030A014";
static const char data_6B40[10] __attribute__((scn_string_literal)) = "E1030A015";
static const char data_6B4A[10] __attribute__((scn_string_literal)) = "E1030A016";
static const char data_6B54[10] __attribute__((scn_string_literal)) = "E1030A017";
static const char data_6B5E[10] __attribute__((scn_string_literal)) = "E1030A018";
static const char data_6B68[10] __attribute__((scn_string_literal)) = "E1030A019";
static const char data_6B72[10] __attribute__((scn_string_literal)) = "E1030A020";
static const char data_6B7C[10] __attribute__((scn_string_literal)) = "E1030A021";
static const char data_6B86[10] __attribute__((scn_string_literal)) = "E1030B001";
static const char data_6B90[10] __attribute__((scn_string_literal)) = "E1030B002";
static const char data_6B9A[10] __attribute__((scn_string_literal)) = "E1030B003";
static const char data_6BA4[10] __attribute__((scn_string_literal)) = "E1030B004";
static const char data_6BAE[10] __attribute__((scn_string_literal)) = "E1030B005";
static const char data_6BB8[10] __attribute__((scn_string_literal)) = "E1030G001";
static const char data_6BC2[10] __attribute__((scn_string_literal)) = "E1030G002";
static const char data_6BCC[10] __attribute__((scn_string_literal)) = "E1030G003";
static const char data_6BD6[10] __attribute__((scn_string_literal)) = "E1030G004";
static const char data_6BE0[10] __attribute__((scn_string_literal)) = "E1030D001";
static const char data_6BEA[10] __attribute__((scn_string_literal)) = "E1030D002";
static const char data_6BF4[10] __attribute__((scn_string_literal)) = "E1030D003";
static const char data_6BFE[10] __attribute__((scn_string_literal)) = "E1030D004";
static const char data_6C08[10] __attribute__((scn_string_literal)) = "E1030D005";
static const char data_6C12[10] __attribute__((scn_string_literal)) = "E1030D006";
static const char data_6C1C[10] __attribute__((scn_string_literal)) = "E1030E001";
static const char data_6C26[10] __attribute__((scn_string_literal)) = "E1030E002";
static const char data_6C30[10] __attribute__((scn_string_literal)) = "E1030E003";
static const char data_6C3A[10] __attribute__((scn_string_literal)) = "E1030E004";
static const char data_6C44[10] __attribute__((scn_string_literal)) = "E1030E005";
static const char data_6C4E[10] __attribute__((scn_string_literal)) = "E1030E006";
static const char data_6C58[10] __attribute__((scn_string_literal)) = "E1030E007";
static const char data_6C62[10] __attribute__((scn_string_literal)) = "E1030E008";
static const char data_6C6C[10] __attribute__((scn_string_literal)) = "E1030E009";
static const char data_6C76[10] __attribute__((scn_string_literal)) = "E1030E010";
static const char data_6C80[10] __attribute__((scn_string_literal)) = "E1030E011";
static const char data_6C8A[10] __attribute__((scn_string_literal)) = "E1030E012";
static const char data_6C94[10] __attribute__((scn_string_literal)) = "E1030E013";
static const char data_6C9E[10] __attribute__((scn_string_literal)) = "E1030E014";
static const char data_6CA8[10] __attribute__((scn_string_literal)) = "E1030E015";
static const char data_6CB2[10] __attribute__((scn_string_literal)) = "E1030E016";
static const char data_6CBC[10] __attribute__((scn_string_literal)) = "E1030E017";
static const char data_6CC6[10] __attribute__((scn_string_literal)) = "E1030E018";
static const char data_6CD0[10] __attribute__((scn_string_literal)) = "E1030E019";
static const char data_6CDA[9] __attribute__((scn_string_literal)) = "オート０";
static const char data_6CE3[9] __attribute__((scn_string_literal)) = "オート１";
static const char data_6CEC[7] __attribute__((scn_string_literal)) = "開始０";
static const char data_6CF3[7] __attribute__((scn_string_literal)) = "成功０";
static const char data_6CFA[7] __attribute__((scn_string_literal)) = "失敗０";
static const char data_6D01[7] __attribute__((scn_string_literal)) = "死亡０";
static const char data_6D08[7] __attribute__((scn_string_literal)) = "開始１";
static const char data_6D0F[7] __attribute__((scn_string_literal)) = "成功１";
static const char data_6D16[7] __attribute__((scn_string_literal)) = "失敗１";
static const char data_6D1D[7] __attribute__((scn_string_literal)) = "開始２";
static const char data_6D24[7] __attribute__((scn_string_literal)) = "成功２";
static const char data_6D2B[7] __attribute__((scn_string_literal)) = "失敗２";
static const char data_6D32[7] __attribute__((scn_string_literal)) = "開始３";
static const char data_6D39[7] __attribute__((scn_string_literal)) = "成功３";
static const char data_6D40[7] __attribute__((scn_string_literal)) = "失敗３";
static const char data_6D47[7] __attribute__((scn_string_literal)) = "開始４";
static const char data_6D4E[7] __attribute__((scn_string_literal)) = "成功４";
static const char data_6D55[7] __attribute__((scn_string_literal)) = "失敗４";
static const char data_6D5C[7] __attribute__((scn_string_literal)) = "開始５";
static const char data_6D63[7] __attribute__((scn_string_literal)) = "成功５";
static const char data_6D6A[7] __attribute__((scn_string_literal)) = "失敗５";
static const char data_6D71[7] __attribute__((scn_string_literal)) = "開始６";
static const char data_6D78[7] __attribute__((scn_string_literal)) = "成功６";
static const char data_6D7F[7] __attribute__((scn_string_literal)) = "失敗６";
static const char data_6D86[15] __attribute__((scn_string_literal)) = "PRESS A BUTTON";
static const char data_6D95[6] __attribute__((scn_string_literal)) = "E1030";
static const char data_6D9B[9] __attribute__((scn_string_literal)) = "E1030";
static const Vector3 data_6DA4 = {.x = 0.0f, .y = 0.0f, .z = 0.0f};
static const Vector3i data_6DB0 = {.x = 0, .y = 0, .z = 0};
