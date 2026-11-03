#include "act01_dhqb.h"

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

static void script_0030(void) {
  Script_ryoControl(PCCTL_DisablePlayerControl);
  return;
}

static void script_0039(void) {
  Script_ryoControl(RYO_RestoreControl);
  Script_SetGameStateTo(AUTH_None);
  return;
}

static void script_0048(int32_t arg0) {
  Script_ccowUtils(arg0, 2);
  if (0) {
    Script_charaDisplay(arg0, CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(arg0, CDM_Show);
      goto L000074;
    }
  }
  L000074:;
  Script_scenToggleVisibleFor(arg0, 1);
  Script_ccowSetFlag80(arg0, 1);
  Script_motmToggleFlags(arg0, 2, 0x40);
  Script_charaToggleMotEnabled(arg0, 0);
  Script_facemotUtils(arg0, 0, 0);
  Script_FaceEx(arg0, 1, 0, 2);
  Script_FaceLookAtTargetUtils(arg0, 0);
  Script_CharHandlers(arg0, 2, 8, 16);
  Script_SetCPAMFlags(arg0, 0);
  Script_charaDisplay(arg0, CDM_Show);
  return;
}

static void script_00D5(int32_t arg0) {
  Script_ccowUtils(arg0, 1);
  Script_ccowSetFlag80(arg0, 2);
  Script_motmToggleFlags(arg0, 1, 0x40);
  Script_charaToggleMotEnabled(arg0, 1);
  Script_FaceEx(arg0, 0, 0, 2);
  Script_FaceLookAtTargetUtils(arg0, 0);
  Script_CharHandlers(arg0, -1, 0, 0);
  Script_SetCPAMFlags(arg0, 1);
  return;
}

static void script_0125(void) {
  Script_filmCheckOrSetTransitionUtil(80, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_0153(int32_t arg0) {
  Script_filmCheckOrSetTransitionUtil(arg0, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_0181(void) {
  Script_filmCheckOrSetTransitionUtil(80, 255, 255, 255, 255, 0, 255, 255, 255);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_01B5(int32_t arg0) {
  Script_filmCheckOrSetTransitionUtil(arg0, 255, 255, 255, 255, 0, 255, 255, 255);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_01E9(int32_t arg0) {
  Script_filmCheckOrSetTransitionUtil(arg0, 255, 255, 255, 255, 150, 68, 50, 0);
  return;
}

static void script_0207(int32_t arg0, int32_t arg1, int32_t arg2) {
  int32_t local_FC;
  Script_GetDataFromFightProgram(arg0, arg1);
  local_FC = 0;
  while (local_FC < arg2) {
    local_FC = local_FC + 1;
    Script_SetFightProgramStateBytes(arg0, local_FC, 1);
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_0241(int32_t arg0, int32_t arg1) {
  while (arg1 > 0) {
    Script_SetFightProgramStateBytes(arg0, arg1, 1);
    arg1 = arg1 - 1;
    SCNF_WaitFrames_0000(1);
  }
  Script_GetDataFromFightProgram(arg0, 0);
  return;
}

static void script_0275(void) {
  int8_t local_DD[35];
  int8_t local_DC;
  int8_t local_DB;
  int8_t local_DA;
  int8_t local_D9;
  int32_t local_D4;
  local_DC = 0;
  while (local_DC < 35) {
    local_DD[local_DC] = local_DC % 4;
    local_DC = local_DC + 1;
  }
  local_D9 = 0;
  for (;;) {
    local_D4 = local_D9;
    switch (local_D4) {
      case 0:
        local_DB = 0;
        local_DA = 17;
        local_D9 = 1;
        break;
      case 1:
        local_DB = 17;
        local_DA = 35;
        local_D9 = 0;
        break;
    }
    local_DC = local_DB;
    while (local_DC < local_DA) {
      Script_SetImageModuleTaskDataFlag(SHEN_NAME('FIR1'), local_DD[local_DC]);
      Script_SetImageModuleTaskDataFlag(SHEN_NAME('FIR2'), local_DD[local_DC]);
      Script_SetImageModuleTaskDataFlag(SHEN_NAME('FIR3'), local_DD[local_DC]);
      Script_SetImageModuleTaskDataFlag(SHEN_NAME('FIR4'), local_DD[local_DC]);
      local_DD[local_DC] = local_DD[local_DC] + 1;
      local_DD[local_DC] = local_DD[local_DC] % 6;
      local_DC = local_DC + 1;
    }
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_036D(int32_t arg0, int32_t arg1) {
  Script_FACE_LCG(arg0, 1, arg1, 0);
  SCNF_WaitFrames_0000(arg1 + 1);
  Script_FACE_LCG(arg0, 2, arg1, 0);
  SCNF_WaitFrames_0000(arg1);
  return;
}

static void script_039D(int32_t arg0, float arg1, float arg2, float arg3) {
  Vector3 local_F4;
  local_F4.x = arg1;
  (&local_F4)->y = arg2;
  (&local_F4)->z = arg3;
  Script_EVT_SetPosition(arg0, POS_XYZ, &local_F4);
  return;
}

static void script_03C5(int32_t arg0, float arg1, float arg2, float arg3) {
  Vector3i local_F4;
  local_F4.x = (int32_t)(arg1 / 360.0f * (float)0x10000);
  (&local_F4)->y = (int32_t)(arg2 / 360.0f * (float)0x10000);
  (&local_F4)->z = (int32_t)(arg3 / 360.0f * (float)0x10000);
  Script_EVT_SetRotation(arg0, 0x38000000, &local_F4, 0);
  return;
}

static void script_0419(int32_t arg0, int32_t arg1, int32_t arg2) {
  Vector3 local_F4;
  for (;;) {
    if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0) & Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg1)) {
      Script_EVT_GetPosition(arg1, 4, &local_F4, ENTITY_XFORM_WORLD_SPACE);
      Script_SetLookPointDetails(arg0, 16, &local_F4, arg2);
      Script_FaceLookAtTargetUtils(arg0, 2, &local_F4, 30);
    }
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static int32_t script_0469(int32_t arg0) {
  int32_t unused_FC;
  GameDateTime local_F4;
  int8_t local_E8[12];
  int32_t local_E4;
  uint8_t unused_E3;
  int8_t local_E2;
  uint8_t unused_E0[2];
  Script_ReturnTimeDate(&local_F4);
  local_E2 = Script_scenUtils(SCENARIO_CMD_GET_STEP_LUT, arg0);
  local_E8[1] = (local_E2 >> 5) & 7;
  if (local_E8[1] == 0) {
    local_E8[1] = 12;
  }
  local_E8[2] = local_E2 & 0x1F;
  if ((&local_F4)->month == local_E8[1]) {
    local_E4 = (&local_F4)->day - local_E8[2];
  } else {
    if (local_E8[1] == 12) {
      local_E8[1] = 0;
    }
    local_E4 = ((&local_F4)->month - local_E8[1]) * 31 + (&local_F4)->day - local_E8[2];
  }
  return local_E4;
}

static void script_0510(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  local_FC = script_0469(arg0);
  if (arg1 > local_FC) {
    (void)0;
  } else {
    (void)-1;
  }
  return;
}

static int32_t script_0533(int32_t arg0) {
  int32_t unused_FC;
  GameDateTime local_F4;
  int32_t local_F0;
  int32_t local_EC;
  Script_ReturnTimeDate(&local_F4);
  local_F0 = (&local_F4)->hours * 60 + (&local_F4)->minutes;
  local_EC = Script_scenUtils(SCENARIO_CMD_GET_STEP_LUT, arg0) * 6;
  return local_F0 - local_EC;
}

static void script_0566(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  local_FC = script_0533(arg0);
  if (arg1 > local_FC) {
    (void)0;
  } else {
    (void)-1;
  }
  return;
}

static void script_0589(int32_t arg0, int32_t arg1) {
  int8_t local_F4[12];
  int32_t local_F0;
  local_F0 = script_0533(arg0);
  local_F4[4] = local_F0 / 60;
  local_F4[5] = local_F0 % 60;
  if ((local_F4[4] < *(int8_t *)(arg1 + 4)) | ((local_F4[4] == *(int8_t *)(arg1 + 4)) & (local_F4[5] < *(int8_t *)(arg1 + 5)))) {
    (void)0;
  } else {
    (void)-1;
  }
  return;
}

static void script_05E8(int32_t arg0) {
  int32_t local_F8[2];
  local_F8[0] = arg0;
  local_F8[1] = 0;
  (void)local_F8;
  return;
}

static int32_t script_05FD(int32_t arg0, int32_t arg1) {
  if (arg1 > 0) {
    SCNF_WaitFrames_0000(arg1);
  }
  Script_executeAicaCommand(arg0, 0, 0);
  return Script_srchControl(0);
}

static int32_t script_0621(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  task_28[arg1] = 0;
  task_44[arg1] = 0;
  task_60[arg1] = 0;
  task_7C[arg1] = arg2;
  task_98[arg1] = arg3;
  task_B4 = 0;
  Script_CharHandlers(arg0, 0, task_7C[arg1], 16);
  Script_CharHandlers(arg0, 1, task_98[arg1], 16);
  for (;;) {
    task_28[arg1] = 0;
    while (task_28[arg1] == 0) {
      SCNF_WaitFrames_0000(1);
    }
    task_B4 = task_60[arg1] | 0x40000000;
    Script_scenCreateTask(arg4, 3, arg0, arg1, task_28[arg1] + task_44[arg1]);
  }
  end:;
}

static int32_t script_06F8(int32_t arg0, int32_t arg1, int32_t arg2, float arg3, int32_t arg4, int32_t arg5) {
  int32_t local_FC;
  local_FC = task_B4;
  if (arg1 > 0) {
    SCNF_WaitFrames_0000(arg1);
  }
  Script_motmGetCharaFlags(arg4, 0, 0);
  Script_motmToggleFlags(arg4, 1, 0x40);
  Script_PlayCharaMotion(arg4, arg0, arg2, -1.0f, -1.0f, 1.0f, 0);
  Script_SetCharaMotionPlaybackRate(arg4, arg3);
  if (local_FC & -0x80000000) {
    Script_motmToggleFlags(arg4, 1, 0x80);
  }
  if (local_FC & 0x40000000) {
    Script_charaToggleMotEnabled(arg4, 1);
  }
  if (local_FC & 0x20000000) {
    Script_charaToggleMotEnabled(arg4, 0);
  }
  return Script_srchControl(0);
}

static int32_t script_078F(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  if (arg3 > 0) {
    SCNF_WaitFrames_0000(arg3);
  }
  Script_CharHandlers(arg0, 2, arg2, 16);
  if (arg4 > 0) {
    SCNF_WaitFrames_0000(arg4);
  }
  Script_CharHandlers(arg0, 0, task_7C[arg1], 16);
  Script_CharHandlers(arg0, 1, task_98[arg1], 16);
  return Script_srchControl(0);
}

static int32_t script_07F1(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  local_FC = task_B4;
  if (arg2 > 0) {
    SCNF_WaitFrames_0000(arg2);
  }
  if (local_FC & 0x10000000) {
    Script_GetDataFromFightProgram(arg0, arg1);
  }
  if (arg3 > 0) {
    SCNF_WaitFrames_0000(arg3);
  }
  if (local_FC & 0x10000000) {
    Script_GetDataFromFightProgram(arg0, 0);
  }
  return Script_srchControl(0);
}

static void script_084D(int32_t arg0, float arg1, int32_t arg2) {
  int32_t local_FC;
  Vector3i local_F0;
  local_FC = task_B4;
  if (arg2 > 0) {
    SCNF_WaitFrames_0000(arg2);
  }
  if (local_FC & -0x80000000) {
    arg1 = (float)0 - arg1;
  }
  (&local_F0)->y = (int32_t)(arg1 / 360.0f * (float)0x10000);
  Script_EVT_SetRotation(arg0, -0x70000000, &local_F0, 0);
  Script_srchControl(0);
  return;
}

static void script_08AA(int32_t arg0) {
  while (Script_IsCharaMotionFinished(arg0) == 0) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static int32_t script_08C6(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, void *arg4, int32_t arg5) {
  int32_t local_FC;
  task_10C[arg1] = Script_scenCreateTask(script_0621, 5, arg0, arg1, arg2, arg3, arg5);
  task_B8[arg1] = arg0;
  task_D4[arg1] = 0;
  task_F0[arg1] = 0;
  task_128 = 4;
  for (;;) {
    SCNF_WaitFrames_0000(1);
    if (task_D4[arg1]) {
      local_FC = task_F0[arg1] * 12;
      {
        int32_t local_F8;
        local_F8 = Script_ScenDataUtil(arg4, 4, local_FC + 5);
        if (local_F8 > 0) {
          SCNF_WaitFrames_0000(local_F8);
        }
      }
      {
        int32_t local_F8;
        local_F8 = Script_ScenDataUtil(arg4, 4, local_FC + 8);
        if (local_F8 > 0) {
          task_28[arg1] = local_F8;
        }
      }
      {
        int32_t local_F8;
        local_F8 = Script_ScenDataUtil(arg4, 4, local_FC + 6);
        if (local_F8 > 0) {
          SCNF_WaitFrames_0000(local_F8);
        }
      }
      if (task_44[arg1] == 0) {
        Script_scenCreateTask(script_0C36, 4, Script_ScenDataUtil(arg4, 4, local_FC), Script_ScenDataUtil(arg4, 4, local_FC + 1), Script_ScenDataUtil(arg4, 4, local_FC + 2), Script_ScenDataUtil(arg4, 4, local_FC + 3));
      }
      {
        int32_t local_F8;
        local_F8 = Script_ScenDataUtil(arg4, 4, local_FC + 7);
        if (local_F8 > 0) {
          SCNF_WaitFrames_0000(local_F8);
        }
      }
      {
        int32_t local_F8;
        local_F8 = Script_ScenDataUtil(arg4, 4, local_FC + 9);
        if (local_F8 > 0) {
          task_28[arg1] = local_F8;
        }
      }
      script_0B34();
      {
        int32_t local_F8;
        if (task_134 == Script_ScenDataUtil(arg4, 4, local_FC)) {
          task_130 = 1;
          local_F8 = Script_ScenDataUtil(arg4, 4, local_FC + 10);
        } else {
          if (task_44[arg1] == 0) {
            task_13C = task_13C + Script_ScenDataUtil(arg4, 4, local_FC + 4);
          }
          task_130 = 0;
          local_F8 = Script_ScenDataUtil(arg4, 4, local_FC + 11);
        }
        if (local_F8 > 0) {
          task_28[arg1] = local_F8;
        }
      }
      script_0B24(arg1);
    }
  }
  end:;
}

static void script_0AED(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  task_D4[arg0] = 1;
  task_F0[arg0] = arg1;
  task_44[arg0] = arg2;
  task_60[arg0] = arg3;
  return;
}

static void script_0B24(int32_t arg0) {
  task_D4[arg0] = 0;
  return;
}

static void script_0B34(void) {
  SCNF_WaitFrames_0000(2);
  while (task_134 == 0) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_0B54(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  script_0AED(arg0, arg1, 0, arg3);
  script_0AED(arg2, arg1, 1, arg3);
  while (task_D4[arg0]) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_0B8E(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  script_0AED(arg0, arg1, 0, arg3);
  script_0AED(arg2, arg1, 1, arg3);
  script_0AED(arg2 + 1, arg1, 2, arg3);
  while (task_D4[arg0]) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_0BDA(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  script_0AED(arg0, arg1, 0, arg4);
  script_0AED(arg2, arg1, 1, arg4);
  script_0AED(arg3, arg1, 2, arg4);
  while (task_D4[arg0]) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_0C23(int32_t arg0) {
  task_128 = arg0;
  Script_SetSfxOverlayTimeoutTuning(arg0, SfxMode_SetBoostStepFrames);
  return;
}

static int32_t script_0C36(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  int32_t local_F8;
  local_FC = arg1 + arg3;
  local_F8 = arg2 + arg3;
  Script_scenCreateTask(script_0C99, 2, arg1, arg3);
  Script_SetSfxOverlayTimeoutTuning(local_FC, SfxMode_SetMaxFrames);
  if (arg0 == 68) {
    SCNF_UIUtils(0x44, local_F8, 5, 0, 1, 0);
  } else {
    SCNF_UIUtils(arg0, local_F8, 2, 0, 1, 0);
  }
  return Script_srchControl(0);
}

static int32_t script_0C99(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  int32_t local_F8;
  task_138 = arg0;
  task_134 = 0;
  task_12C = 0;
  local_F8 = 0xFFFF;
  for (;;) {
    local_FC = SCNF_UIUtils(0, 0, 0, 0, 0, -1);
    task_12C = task_12C + 1;
    if (local_FC != 0xFFFF) {
      if (local_FC != 0xA000) {
        local_F8 = local_FC;
      }
    }
    if (arg0 == -1) {
      if (local_F8 != 0xFFFF) {
        break;
      }
    } else {
      if (task_12C >= arg0 + arg1 + 4) {
        break;
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  if ((local_F8 & 0xFFF) == 0) {
    local_F8 = 0xFFFF;
  }
  task_134 = local_F8 & 0xFFF;
  return Script_srchControl(0);
}

static int32_t script_0D5A(int32_t arg0, int32_t arg1, int32_t arg2) {
  {
    Vector3 local_F4;
    Script_EVT_GetPosition(arg0, -1, &local_F4, 0);
    task_140[arg1] = arg0;
    task_15C[arg1][0] = (&local_F4)->x;
    task_15C[arg1][1] = (&local_F4)->y;
    task_15C[arg1][2] = (&local_F4)->z;
  }
  task_1B0[arg1] = 0.0f;
  for (;;) {
    Vector3 local_F4;
    SCNF_WaitFrames_0000(1);
    Script_EVT_GetPosition(arg0, -1, &local_F4, 0);
    task_15C[arg1][0] = (&local_F4)->x;
    task_15C[arg1][1] = (&local_F4)->y;
    task_15C[arg1][2] = (&local_F4)->z;
    task_1B0[arg1] = Script_MathUtils(SCRIPT_MATH_VEC2_LENGTH, task_15C[arg1][0] - task_15C[arg2][0], task_15C[arg1][2] - task_15C[arg2][2]);
  }
  end:;
}

static void script_0E48(void) {
  task_1CC = 0.0f;
  task_1D0 = 0.0f;
  task_1D4 = 0.0f;
  for (;;) {
    SCNF_WaitFrames_0000(1);
    if (Script_GetInputData(2, 0) & 4) {
      if (Script_GetInputData(0, 2) & 0x4000) {
        task_1CC = task_1CC - 0.01f;
      }
      if (Script_GetInputData(0, 2) & 0x8000) {
        task_1CC = task_1CC + 0.01f;
      }
      if (Script_GetInputData(0, 2) & 0x1000) {
        task_1D0 = task_1D0 - 0.01f;
      }
      if (Script_GetInputData(0, 2) & 0x2000) {
        task_1D0 = task_1D0 + 0.01f;
      }
      if (Script_GetInputData(0, 2) & 0x40) {
        task_1D4 = task_1D4 - 1.0f;
      }
      if (Script_GetInputData(0, 2) & 0x80) {
        task_1D4 = task_1D4 + 1.0f;
      }
    }
  }
  return;
}

static void script_0F36(int32_t arg0, int32_t arg1) {
  Vector3 local_F4;
  Vector3i local_E8;
  Script_MoveObjectToObject(arg0, arg1);
  Script_EVT_GetPosition(arg0, -1, &local_F4, 0);
  Script_mtwkGetSetRotation(arg0, &local_E8, 0);
  Script_MoveObjectToObject(arg0, -1);
  return;
}

static void script_0F5F(int32_t arg0, int32_t arg1, float arg2, float arg3, float arg4) {
  float local_F4[3];
  Vector3i local_E8;
  int32_t local_E4;
  Vector3 local_D8;
  local_E4 = Script_CEWPRelated(arg0);
  Script_MoveObjectToObject(arg0, arg1);
  local_F4[0] = arg2;
  local_F4[1] = 0.0f;
  local_F4[2] = arg3;
  local_D8.x = arg2;
  (&local_D8)->z = arg3;
  Script_EVT_SetPosition(arg0, 0x28000000, &local_D8);
  local_E8.x = 0;
  (&local_E8)->y = (int32_t)(arg4 / 360.0f * (float)0x10000);
  (&local_E8)->z = 0;
  Script_EVT_SetRotation(arg0, 0x38000000, &local_E8, 0);
  Script_MoveObjectToObject(arg0, local_E4);
  Script_InitializeOsag(arg0);
  return;
}

static void script_0FEE(int32_t arg0, int32_t arg1, const Vector3 *arg2) {
  int32_t local_FC;
  local_FC = Script_CEWPRelated(arg0);
  Script_MoveObjectToObject(arg0, arg1);
  Script_EVT_SetPosition(arg0, POS_XYZ, arg2);
  Script_MoveObjectToObject(arg0, local_FC);
  Script_InitializeOsag(arg0);
  return;
}

static void script_1021(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
  Script_scenCreateTask(script_103B, 6, arg0, arg1, arg2, arg3, arg4, arg5);
  return;
}

static int32_t script_103B(int32_t arg0, int32_t arg1, float arg2, float arg3, float arg4, int32_t arg5) {
  if (arg5 >= 0) {
    SCNF_WaitFrames_0000(arg5);
  }
  script_0F5F(arg0, arg1, arg2, arg3, arg4);
  return Script_srchControl(0);
}

static void script_1066(int32_t arg0, int32_t arg1, float arg2, float arg3, float arg4, float arg5) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t local_E4;
  local_E4 = Script_CEWPRelated(arg0);
  Script_MoveObjectToObject(arg0, arg1);
  local_F4.x = arg2;
  (&local_F4)->y = arg3;
  (&local_F4)->z = arg4;
  Script_EVT_SetPosition(arg0, POS_XYZ, &local_F4);
  local_E8.x = 0;
  (&local_E8)->y = (int32_t)(arg5 / 360.0f * (float)0x10000);
  (&local_E8)->z = 0;
  Script_EVT_SetRotation(arg0, 0x38000000, &local_E8, 0);
  Script_MoveObjectToObject(arg0, local_E4);
  Script_InitializeOsag(arg0);
  return;
}

static void script_10E6(int32_t arg0, int32_t arg1, float arg2) {
  int32_t unused_F4[3];
  Vector3i local_E8;
  int32_t local_E4;
  local_E4 = Script_CEWPRelated(arg0);
  Script_MoveObjectToObject(arg0, arg1);
  local_E8.x = 0;
  (&local_E8)->y = (int32_t)(arg2 / 360.0f * (float)0x10000);
  (&local_E8)->z = 0;
  Script_EVT_SetRotation(arg0, 0x38000000, &local_E8, 0);
  Script_MoveObjectToObject(arg0, local_E4);
  return;
}

static void script_113A(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Script_scenCreateTask(script_1150, 4, arg0, arg1, arg2, arg3);
  return;
}

static int32_t script_1150(int32_t arg0, int32_t arg1, float arg2, int32_t arg3) {
  if (arg3 >= 0) {
    SCNF_WaitFrames_0000(arg3);
  }
  script_10E6(arg0, arg1, arg2);
  return Script_srchControl(0);
}

static void script_1177(int32_t arg0, float arg1) {
  Vector3i local_F4;
  (&local_F4)->y = (int32_t)(arg1 / 360.0f * (float)0x10000);
  Script_EVT_SetRotation(arg0, -0x70000000, &local_F4, 0);
  return;
}

static void script_11A0(int32_t arg0, float arg1, float arg2, float arg3) {
  Vector3i local_F4;
  local_F4.x = (int32_t)(arg1 / 360.0f * (float)0x10000);
  (&local_F4)->y = (int32_t)(arg2 / 360.0f * (float)0x10000);
  (&local_F4)->z = (int32_t)(arg3 / 360.0f * (float)0x10000);
  Script_EVT_SetRotation(arg0, -0x48000000, &local_F4, 0);
  return;
}

static void script_11F4(int32_t arg0, int32_t arg1, int32_t arg2) {
  Script_scenCreateTask(script_1208, 3, arg0, arg1, arg2);
  return;
}

static int32_t script_1208(int32_t arg0, float arg1, int32_t arg2) {
  if (arg2 >= 0) {
    SCNF_WaitFrames_0000(arg2);
  }
  script_1177(arg0, arg1);
  return Script_srchControl(0);
}

static void script_122D(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  Script_scenCreateTask(script_1245, 5, arg0, arg1, arg2, arg3, arg4);
  return;
}

static int32_t script_1245(int32_t arg0, float arg1, float arg2, float arg3, int32_t arg4) {
  if (arg4 >= 0) {
    SCNF_WaitFrames_0000(arg4);
  }
  script_11A0(arg0, arg1, arg2, arg3);
  return Script_srchControl(0);
}

static void script_126E(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  Script_scenCreateTask(script_1286, 5, arg0, arg1, arg2, arg3, arg4);
  return;
}

static int32_t script_1286(int32_t arg0, float arg1, float arg2, float arg3, int32_t arg4) {
  float local_FC;
  if (arg4 >= 0) {
    SCNF_WaitFrames_0000(arg4);
  }
  local_FC = (arg2 - arg1) / arg3;
  for (;;) {
    if (arg3 <= 0.0f) {
      break;
    }
    script_11A0(arg0, 0.0f, 0.0f, local_FC);
    arg3 = arg3 - 1.0f;
    SCNF_WaitFrames_0000(1);
  }
  return Script_srchControl(0);
}

static void script_12DB(int32_t arg0, int32_t arg1) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t local_E4;
  local_E4 = arg0;
  while (local_E4 <= arg1) {
    Script_EVT_GetPosition(task_B8[local_E4], -1, &local_F4, 0);
    Script_mtwkGetSetRotation(task_B8[local_E4], &local_E8, 0);
    task_1D8[local_E4][0] = (&local_F4)->x;
    task_1D8[local_E4][1] = (&local_F4)->y;
    task_1D8[local_E4][2] = (&local_F4)->z;
    task_22C[local_E4][0] = (&local_E8)->x;
    task_22C[local_E4][1] = (&local_E8)->y;
    task_22C[local_E4][2] = (&local_E8)->z;
    task_280[local_E4] = Script_GetCharaMotionRemainingFramesScaled(task_B8[local_E4]);
    task_29C[local_E4] = Script_scenCharaGetMotAnimId(task_B8[local_E4]);
    local_E4 = local_E4 + 1;
  }
  return;
}

static void script_13C0(int32_t arg0, int32_t arg1) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t local_E4;
  local_E4 = arg0;
  while (local_E4 <= arg1) {
    local_F4.x = task_1D8[local_E4][0];
    (&local_F4)->y = task_1D8[local_E4][1];
    (&local_F4)->z = task_1D8[local_E4][2];
    local_E8.x = task_22C[local_E4][0];
    (&local_E8)->y = task_22C[local_E4][1];
    (&local_E8)->z = task_22C[local_E4][2];
    Script_EVT_SetPosition(task_B8[local_E4], POS_XYZ, &local_F4);
    Script_EVT_SetRotation(task_B8[local_E4], 0x38000000, &local_E8, 0);
    Script_PlayCharaMotion(task_B8[local_E4], task_29C[local_E4], task_280[local_E4], -1.0f, -1.0f, 1.0f, 0);
    local_E4 = local_E4 + 1;
  }
  return;
}

static void script_14A9(int32_t arg0) {
  task_2BC = arg0;
  task_2B8 = 1;
  Script_PlayerStatsUtils(PS_Set70MBTimeCapSlot, task_2BC, task_2B8);
  return;
}

static void script_14C7(void) {
  int32_t local_FC;
  if (task_2B8 == 1) {
    local_FC = Script_PlayerStatsUtils(PS_Get70MBCounter1);
    Script_PlayerStatsUtils(PS_Set70MBCounter1, local_FC + 1);
  }
  if (task_2B8 < 255) {
    task_2B8 = task_2B8 + 1;
  }
  Script_PlayerStatsUtils(PS_Set70MBTimeCapSlot, task_2BC, task_2B8);
  return;
}

static void script_150F(void) {
  int32_t local_FC;
  local_FC = Script_PlayerStatsUtils(PS_Get70MBCounter0);
  Script_PlayerStatsUtils(PS_Set70MBCounter0, local_FC + 1);
  return;
}

static void script_152A(void) {
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_1544(const char *arg0, const char *arg1) {
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 5, arg1) == 0) {
    while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, arg0, arg1) == 0) {
      SCNF_WaitFrames_0000(1);
    }
    script_152A();
  }
  return;
}

static void script_1582(const char *arg0) {
  if (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 5, arg0) != 0) {
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, arg0);
  }
  return;
}

static void script_15A3(int32_t arg0) {
  L0015A5:;
  SCNF_WaitFrames_0000(1);
  if (!Script_CycleManagerUtils(CYCLE_CMD_GET_NEXT_TASK_POINTER, arg0)) goto L0015A5;
  return;
}

static void script_15BA(int32_t arg0) {
  int32_t local_FC;
  local_FC = 0;
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0) != 0) {
    local_FC = Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT_INTERNAL_HUMAN, arg0);
  } else {
    Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT, arg0, 1);
    while (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0) == 0) {
      local_FC = Script_CycleManagerUtils(CYCLE_CMD_GET_CYCLE_PTR_BY_CHARA_ID, arg0);
      while (local_FC == 0) {
        SCNF_WaitFrames_0000(1);
        local_FC = Script_CycleManagerUtils(CYCLE_CMD_GET_CYCLE_PTR_BY_CHARA_ID, arg0);
      }
      script_15A3(local_FC);
    }
  }
  (void)local_FC;
  return;
}

static void script_1640(int32_t arg0, int32_t arg1) {
  if (arg1) {
    Script_CycleManagerUtils(CYCLE_CMD_LOAD_OR_RESET_RUNNING_PROGRAM, arg1);
  }
  Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT, arg0, 0);
  arg1 = 0;
  return;
}

static int32_t script_1663(int32_t arg0, int32_t arg1, int32_t arg2) {
  int32_t local_FC;
  local_FC = arg2;
  switch (local_FC) {
    case 1:
      Script_scenCreateTask(script_078F, 5, arg0, arg1, 0, -1, 30);
      Script_scenCreateTask(script_06F8, 6, 20489, -1, 0, 0x3F800000, arg0, arg1);
      break;
    case 2:
      Script_scenCreateTask(script_078F, 5, arg0, arg1, 0, -1, 30);
      Script_scenCreateTask(script_06F8, 6, 20506, -1, 0, 0x3F800000, arg0, arg1);
      break;
    case 3:
      Script_scenCreateTask(script_05FD, 2, 0x1376AB, 10);
      Script_scenCreateTask(script_05FD, 2, 0x1C76AB, 17);
      Script_scenCreateTask(script_078F, 5, arg0, arg1, 0, -1, 30);
      Script_scenCreateTask(script_06F8, 6, 20493, -1, 0x41400000, 0x3F800000, arg0, arg1);
      break;
    case 4:
      Script_scenCreateTask(script_05FD, 2, 722089, 40);
      Script_scenCreateTask(script_078F, 5, arg0, arg1, 0, -1, 30);
      Script_scenCreateTask(script_06F8, 6, 20507, -1, 0x41400000, 0x3F800000, arg0, arg1);
      Script_scenCreateTask(script_06F8, 6, 20508, 40, 0x41400000, 0x3F800000, arg0, arg1);
      break;
    case 5:
      Script_scenCreateTask(script_05FD, 2, 30379, 3);
      Script_scenCreateTask(script_078F, 5, arg0, arg1, 0, -1, 30);
      Script_scenCreateTask(script_06F8, 6, 20489, -1, 0x41C00000, 0x3F800000, arg0, arg1);
      break;
    case 6:
      Script_scenCreateTask(script_078F, 5, arg0, arg1, 0, -1, 30);
      Script_scenCreateTask(script_06F8, 6, 20506, -1, 0x41C00000, 0x3F800000, arg0, arg1);
      break;
    case 7:
      Script_scenCreateTask(script_05FD, 2, 0x1376AB, 10);
      Script_scenCreateTask(script_05FD, 2, 0x1C76AB, 17);
      Script_scenCreateTask(script_078F, 5, arg0, arg1, 0, -1, 30);
      Script_scenCreateTask(script_06F8, 6, 20493, -1, 0x40E00000, 0x3F800000, arg0, arg1);
      break;
    case 8:
      Script_scenCreateTask(script_05FD, 2, 722089, 40);
      Script_scenCreateTask(script_078F, 5, arg0, arg1, 0, -1, 30);
      Script_scenCreateTask(script_06F8, 6, 20507, -1, 0x40E00000, 0x3F800000, arg0, arg1);
      Script_scenCreateTask(script_06F8, 6, 20508, 40, 0x40E00000, 0x3F800000, arg0, arg1);
      break;
    case 9:
      Script_scenCreateTask(script_05FD, 2, 30379, 10);
      Script_scenCreateTask(script_05FD, 2, 0x2876AB, 50);
      Script_scenCreateTask(script_07F1, 4, arg0, 1, -1, 30);
      Script_scenCreateTask(script_078F, 5, arg0, arg1, 0, -1, 30);
      Script_scenCreateTask(script_06F8, 6, 20489, -1, 0x42C80000, 0x3F800000, arg0, arg1);
      break;
    case 10:
      Script_scenCreateTask(script_078F, 5, arg0, arg1, 0, -1, 30);
      Script_scenCreateTask(script_06F8, 6, 20506, -1, 0x42C80000, 0x3F800000, arg0, arg1);
      break;
    case 11:
      Script_scenCreateTask(script_06F8, 6, 20501, -1, 0, 0x3F800000, arg0, arg1);
      break;
    case 12:
      Script_scenCreateTask(script_05FD, 2, 0x2976AB, 50);
      Script_scenCreateTask(script_06F8, 6, 20530, -1, 0, 0x3F800000, arg0, arg1);
      break;
    case 13:
      Script_scenCreateTask(script_078F, 5, arg0, arg1, 0, -1, 30);
      Script_scenCreateTask(script_06F8, 6, 20501, -1, 0x42640000, 0x3F800000, arg0, arg1);
      break;
    case 14:
      Script_scenCreateTask(script_05FD, 2, 161451, 9);
      Script_scenCreateTask(script_05FD, 2, 0x2876AB, 50);
      Script_scenCreateTask(script_078F, 5, arg0, arg1, 0, -1, 30);
      Script_scenCreateTask(script_06F8, 6, 20518, -1, 0x42640000, 0x3F800000, arg0, arg1);
      break;
    case 15:
      Script_scenCreateTask(script_05FD, 2, 30379, 9);
      Script_scenCreateTask(script_078F, 5, arg0, arg1, 0, -1, 30);
      Script_scenCreateTask(script_06F8, 6, 20485, -1, 0x41400000, 0x3F800000, arg0, arg1);
      break;
    case 16:
      Script_scenCreateTask(script_078F, 5, arg0, arg1, 0, -1, 30);
      Script_scenCreateTask(script_06F8, 6, 20514, -1, 0x41400000, 0x3F800000, arg0, arg1);
      break;
    case 17:
      Script_scenCreateTask(script_078F, 5, arg0, arg1, 0, -1, 30);
      Script_scenCreateTask(script_06F8, 6, 20485, -1, 0x42200000, 0x3F800000, arg0, arg1);
      break;
    case 18:
      Script_scenCreateTask(script_05FD, 2, 30379, 13);
      Script_scenCreateTask(script_05FD, 2, 0x2876AB, 50);
      Script_scenCreateTask(script_078F, 5, arg0, arg1, 0, -1, 30);
      Script_scenCreateTask(script_06F8, 6, 20514, -1, 0x42200000, 0x3F800000, arg0, arg1);
      break;
    case 19:
      Script_scenCreateTask(script_05FD, 2, 161451, 15);
      Script_scenCreateTask(script_05FD, 2, 0x2876AB, 50);
      Script_scenCreateTask(script_078F, 5, arg0, arg1, 0, -1, 30);
      Script_scenCreateTask(script_06F8, 6, 20490, -1, 0x42000000, 0x3F800000, arg0, arg1);
      break;
    case 20:
      Script_scenCreateTask(script_078F, 5, arg0, arg1, 0, -1, 30);
      Script_scenCreateTask(script_06F8, 6, 20516, -1, 0x42000000, 0x3F800000, arg0, arg1);
      break;
    case 21:
      break;
    case 22:
      Script_scenCreateTask(script_06F8, 6, 20519, -1, 0x42C80000, 0x3F800000, arg0, arg1);
      break;
    case 23:
      Script_scenCreateTask(script_078F, 5, arg0, arg1, 0, -1, 30);
      Script_scenCreateTask(script_06F8, 6, 20492, -1, 0x41900000, 0x3F800000, arg0, arg1);
      break;
    case 24:
      Script_scenCreateTask(script_05FD, 2, 0x1F04A9, -1);
      Script_scenCreateTask(script_05FD, 2, 0x1376AB, 13);
      Script_scenCreateTask(script_05FD, 2, 30379, 30);
      Script_scenCreateTask(script_078F, 5, arg0, arg1, 0, -1, 30);
      Script_scenCreateTask(script_06F8, 6, 20524, -1, 0x42800000, 0x3F800000, arg0, arg1);
      break;
    case 25:
      Script_scenCreateTask(script_05FD, 2, 30379, 7);
      Script_scenCreateTask(script_05FD, 2, 0x2876AB, 33);
      Script_scenCreateTask(script_06F8, 6, 20483, -1, 0, 0x3F800000, arg0, arg1);
      break;
    case 26:
      Script_scenCreateTask(script_06F8, 6, 20519, -1, 0x431C0000, 0x3F800000, arg0, arg1);
      break;
    case 27:
      Script_scenCreateTask(script_07F1, 4, arg0, 7, 10, 12);
      Script_scenCreateTask(script_078F, 5, arg0, arg1, 0, -1, 30);
      Script_scenCreateTask(script_06F8, 6, 20540, -1, 0x428C0000, 0x3F800000, arg0, arg1);
      break;
    case 28:
      Script_scenCreateTask(script_05FD, 2, 30379, 21);
      Script_scenCreateTask(script_078F, 5, arg0, arg1, 0, -1, 30);
      Script_scenCreateTask(script_06F8, 6, 20528, 18, 0, 0x3F800000, arg0, arg1);
      break;
    case 29:
      SCNF_WaitFrames_0000(30);
      Script_scenCreateTask(script_05FD, 2, 30379, 27);
      Script_scenCreateTask(script_078F, 5, arg0, arg1, 0, -1, 30);
      Script_scenCreateTask(script_06F8, 6, 20496, 22, 0, 0x3F800000, arg0, arg1);
      break;
    case 30:
      SCNF_WaitFrames_0000(30);
      Script_scenCreateTask(script_078F, 5, arg0, arg1, 0, -1, 30);
      Script_scenCreateTask(script_06F8, 6, 20527, -1, 0x40800000, 0x3F800000, arg0, arg1);
      break;
    case 31:
      Script_scenCreateTask(script_07F1, 4, arg0, 4, 10, 10);
      Script_scenCreateTask(script_078F, 5, arg0, arg1, 0, -1, 30);
      Script_scenCreateTask(script_06F8, 6, 20496, -1, 0x41A00000, 0x3F800000, arg0, arg1);
      break;
    case 32:
      Script_scenCreateTask(script_05FD, 2, 30379, 20);
      Script_scenCreateTask(script_05FD, 2, 0x2876AB, 33);
      Script_scenCreateTask(script_078F, 5, arg0, arg1, 0, -1, 30);
      Script_scenCreateTask(script_06F8, 6, 20527, -1, 0x42380000, 0x3F800000, arg0, arg1);
      break;
    case 33:
      Script_scenCreateTask(script_05FD, 2, 30379, 25);
      Script_scenCreateTask(script_05FD, 2, 0x2876AB, 47);
      Script_scenCreateTask(script_078F, 5, arg0, arg1, 0, -1, 30);
      Script_scenCreateTask(script_06F8, 6, 20494, -1, 0x41D80000, 0x3F800000, arg0, arg1);
      break;
    case 34:
      Script_scenCreateTask(script_078F, 5, arg0, arg1, 0, -1, 30);
      Script_scenCreateTask(script_06F8, 6, 20525, -1, 0x42380000, 0x3F800000, arg0, arg1);
      break;
  }
  return Script_srchControl(0);
}

__attribute__((scn_string_bindings(0, data_69DC)))
static int32_t script_2088(int32_t arg0, int32_t arg1, int32_t arg2) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t local_E4;
  int32_t local_E0;
  int32_t local_DC;
  if (arg2 > 0) {
    SCNF_WaitFrames_0000(arg2);
  }
  if (arg1 != 0) {
    Script_charaDisplay(arg1, CDM_Hide);
  }
  Script_charaDisplay(arg0, CDM_Show);
  local_E4 = 2;
  local_E0 = 2;
  Script_scenCreateTask(script_05FD, 2, 66729, (10 - local_E4) / local_E0);
  Script_scenCreateTask(script_05FD, 2, 132265, (10 - local_E4) / local_E0);
  Script_scenCreateTask(script_05FD, 2, 197801, (20 - local_E4) / local_E0);
  Script_scenCreateTask(script_05FD, 2, 263337, (30 - local_E4) / local_E0);
  Script_scenCreateTask(script_05FD, 2, 328873, (40 - local_E4) / local_E0);
  for (;;) {
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 2, "MPK00");
    local_DC = 0;
    while (local_DC < 47) {
      Script_InterpolateAndScaleAngles(local_DC, local_E4, &local_F4, &local_E8, "TBDN");
      local_F4.x = (&local_F4)->x * 10.0f;
      (&local_F4)->y = (&local_F4)->y * 10.0f;
      (&local_F4)->z = (&local_F4)->z * 10.0f;
      Script_charaPosRotUtils(arg0, *(&data_688C + local_DC), &local_F4, &local_E8, 0);
      local_DC = local_DC + 1;
    }
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
    local_E4 = local_E4 + local_E0;
    if (local_E4 >= 184) {
      Script_srchControl(0);
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

__attribute__((scn_string_bindings(0, data_69E7)))
static int32_t script_21FA(int32_t arg0, int32_t arg1, int32_t arg2) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t local_E4;
  int32_t local_E0;
  int32_t local_DC;
  if (arg2 > 0) {
    SCNF_WaitFrames_0000(arg2);
  }
  if (arg1 != 0) {
    Script_charaDisplay(arg1, CDM_Hide);
  }
  Script_charaDisplay(arg0, CDM_Show);
  local_E4 = 188;
  local_E0 = 2;
  Script_scenCreateTask(script_05FD, 2, 459945, (213 - local_E4) / local_E0);
  for (;;) {
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 2, "MPK00");
    local_DC = 0;
    while (local_DC < 1) {
      Script_InterpolateAndScaleAngles(local_DC, local_E4, &local_F4, &local_E8, "CIDN");
      local_F4.x = (&local_F4)->x * 10.0f;
      (&local_F4)->y = (&local_F4)->y * 10.0f;
      (&local_F4)->z = (&local_F4)->z * 10.0f;
      Script_charaPosRotUtils(arg0, *(&data_6948 + local_DC), &local_F4, &local_E8, 0);
      local_DC = local_DC + 1;
    }
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
    local_E4 = local_E4 + local_E0;
    if (local_E4 >= 326) {
      Script_srchControl(0);
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

__attribute__((scn_string_bindings(0, data_69F2)))
static int32_t script_230E(int32_t arg0, int32_t arg1, int32_t arg2) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t local_E4;
  int32_t local_E0;
  int32_t local_DC;
  if (arg2 > 0) {
    SCNF_WaitFrames_0000(arg2);
  }
  if (arg1 != 0) {
    Script_charaDisplay(arg1, CDM_Hide);
  }
  Script_charaDisplay(arg0, CDM_Show);
  local_E4 = 2;
  local_E0 = 2;
  Script_scenCreateTask(script_05FD, 2, 787625, (10 - local_E4) / local_E0);
  for (;;) {
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 2, "MPK00");
    local_DC = 0;
    while (local_DC < 6) {
      Script_InterpolateAndScaleAngles(local_DC, local_E4, &local_F4, &local_E8, "CB1D");
      local_F4.x = (&local_F4)->x * 10.0f;
      (&local_F4)->y = (&local_F4)->y * 10.0f;
      (&local_F4)->z = (&local_F4)->z * 10.0f;
      Script_charaPosRotUtils(arg0, *(int32_t *)(data_694C + (local_DC << 2)), &local_F4, &local_E8, 0);
      local_DC = local_DC + 1;
    }
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
    local_E4 = local_E4 + local_E0;
    if (local_E4 >= 112) {
      Script_srchControl(0);
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

__attribute__((scn_string_bindings(0, data_69FD)))
static int32_t script_241F(int32_t arg0, int32_t arg1, int32_t arg2) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t local_E4;
  int32_t local_E0;
  int32_t local_DC;
  if (arg2 > 0) {
    SCNF_WaitFrames_0000(arg2);
  }
  if (arg1 != 0) {
    Script_charaDisplay(arg1, CDM_Hide);
  }
  Script_charaDisplay(arg0, CDM_Show);
  local_E4 = 2;
  local_E0 = 2;
  Script_scenCreateTask(script_05FD, 2, 853161, (10 - local_E4) / local_E0);
  Script_scenCreateTask(script_05FD, 2, 459945, (30 - local_E4) / local_E0);
  Script_scenCreateTask(script_05FD, 2, 525481, (30 - local_E4) / local_E0);
  Script_scenCreateTask(script_05FD, 2, 918697, (60 - local_E4) / local_E0);
  for (;;) {
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 2, "MPK00");
    local_DC = 0;
    while (local_DC < 5) {
      Script_InterpolateAndScaleAngles(local_DC, local_E4, &local_F4, &local_E8, "CB2D");
      local_F4.x = (&local_F4)->x * 10.0f;
      (&local_F4)->y = (&local_F4)->y * 10.0f;
      (&local_F4)->z = (&local_F4)->z * 10.0f;
      Script_charaPosRotUtils(arg0, *(&data_6964 + local_DC), &local_F4, &local_E8, 0);
      local_DC = local_DC + 1;
    }
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
    local_E4 = local_E4 + local_E0;
    if (local_E4 >= 276) {
      Script_srchControl(0);
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

__attribute__((scn_string_bindings(0, data_6A08)))
static int32_t script_2579(int32_t arg0, int32_t arg1, int32_t arg2) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t local_E4;
  int32_t local_E0;
  int32_t local_DC;
  if (arg2 > 0) {
    SCNF_WaitFrames_0000(arg2);
  }
  if (arg1 != 0) {
    Script_charaDisplay(arg1, CDM_Hide);
  }
  Script_charaDisplay(arg0, CDM_Show);
  local_E4 = 2;
  local_E0 = 2;
  Script_scenCreateTask(script_05FD, 2, 787625, (10 - local_E4) / local_E0);
  for (;;) {
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 2, "MPK00");
    local_DC = 0;
    while (local_DC < 6) {
      Script_InterpolateAndScaleAngles(local_DC, local_E4, &local_F4, &local_E8, "CB3D");
      local_F4.x = (&local_F4)->x * 10.0f;
      (&local_F4)->y = (&local_F4)->y * 10.0f;
      (&local_F4)->z = (&local_F4)->z * 10.0f;
      Script_charaPosRotUtils(arg0, *(int32_t *)(data_6978 + (local_DC << 2)), &local_F4, &local_E8, 0);
      local_DC = local_DC + 1;
    }
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
    local_E4 = local_E4 + local_E0;
    if (local_E4 >= 140) {
      Script_srchControl(0);
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

__attribute__((scn_string_bindings(0, data_6A13)))
static int32_t script_268B(int32_t arg0, int32_t arg1, int32_t arg2) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t local_E4;
  int32_t local_E0;
  int32_t local_DC;
  if (arg2 > 0) {
    SCNF_WaitFrames_0000(arg2);
  }
  if (arg1 != 0) {
    Script_charaDisplay(arg1, CDM_Hide);
  }
  Script_charaDisplay(arg0, CDM_Show);
  local_E4 = 2;
  local_E0 = 2;
  Script_scenCreateTask(script_05FD, 2, 787625, (10 - local_E4) / local_E0);
  for (;;) {
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 2, "MPK00");
    local_DC = 0;
    while (local_DC < 8) {
      Script_InterpolateAndScaleAngles(local_DC, local_E4, &local_F4, &local_E8, "CB4D");
      local_F4.x = (&local_F4)->x * 10.0f;
      (&local_F4)->y = (&local_F4)->y * 10.0f;
      (&local_F4)->z = (&local_F4)->z * 10.0f;
      Script_charaPosRotUtils(arg0, *(&data_6990 + local_DC), &local_F4, &local_E8, 0);
      local_DC = local_DC + 1;
    }
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
    local_E4 = local_E4 + local_E0;
    if (local_E4 >= 170) {
      Script_srchControl(0);
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

__attribute__((scn_string_bindings(0, data_6A1E)))
static int32_t script_279D(int32_t arg0, int32_t arg1, int32_t arg2) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t local_E4;
  int32_t local_E0;
  int32_t local_DC;
  if (arg2 > 0) {
    SCNF_WaitFrames_0000(arg2);
  }
  if (arg1 != 0) {
    Script_charaDisplay(arg1, CDM_Hide);
  }
  Script_charaDisplay(arg0, CDM_Show);
  local_E4 = 2;
  local_E0 = 2;
  Script_scenCreateTask(script_05FD, 2, 591017, (10 - local_E4) / local_E0);
  Script_scenCreateTask(script_05FD, 2, 656553, (15 - local_E4) / local_E0);
  for (;;) {
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 2, "MPK00");
    local_DC = 0;
    while (local_DC < 10) {
      Script_InterpolateAndScaleAngles(local_DC, local_E4, &local_F4, &local_E8, "TIDN");
      local_F4.x = (&local_F4)->x * 10.0f;
      (&local_F4)->y = (&local_F4)->y * 10.0f;
      (&local_F4)->z = (&local_F4)->z * 10.0f;
      Script_charaPosRotUtils(arg0, *(&data_69B0 + local_DC), &local_F4, &local_E8, 0);
      local_DC = local_DC + 1;
    }
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
    local_E4 = local_E4 + local_E0;
    if (local_E4 >= 200) {
      Script_srchControl(0);
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

__attribute__((scn_string_bindings(0, data_6A29)))
static int32_t script_28C7(int32_t arg0, int32_t arg1, int32_t arg2) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t local_E4;
  int32_t local_E0;
  int32_t local_DC;
  if (arg2 > 0) {
    SCNF_WaitFrames_0000(arg2);
  }
  if (arg1 != 0) {
    Script_charaDisplay(arg1, CDM_Hide);
  }
  Script_charaDisplay(arg0, CDM_Show);
  Script_MoveObjectToObject(SHEN_NAME('DUMD'), 0x304D5544);
  Script_MoveObjectToObject(arg0, 0x444D5544);
  local_E4 = 492;
  local_E0 = 2;
  Script_scenCreateTask(script_05FD, 2, 0x1004A9, (502 - local_E4) / local_E0);
  Script_scenCreateTask(script_05FD, 2, 0x1104A9, (0x200 - local_E4) / local_E0);
  Script_scenCreateTask(script_2A84, 2, arg0, 40);
  for (;;) {
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 2, "MPK00");
    local_DC = 0;
    while (local_DC < 1) {
      Script_InterpolateAndScaleAngles(local_DC, local_E4, &local_F4, &local_E8, "MKDN");
      local_F4.x = (&local_F4)->x * 10.0f;
      (&local_F4)->y = (&local_F4)->y * 10.0f;
      (&local_F4)->z = (&local_F4)->z * 10.0f;
      Script_EVT_SetPosition(arg0, POS_XYZ, &local_F4);
      Script_EVT_SetRotation(arg0, 0x38000000, &local_E8, 0);
      local_DC = local_DC + 1;
    }
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
    local_E4 = local_E4 + local_E0;
    if (local_E4 >= 554) {
      Script_srchControl(0);
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

__attribute__((scn_string_bindings(0, data_6A40, 2, data_6A5B)))
static void script_2A1F(void) {
  task_2C0 = Script_MT5Loader_TRWV_TRWAVE(0, &data_6A34, "scene/01/DHQB", "MIKOWAVE.PVR", "scene/01/DHQB", data_6A69, 0.005f, 0x20000000);
  Script_MT5Loader_TRWV_TRWAVE(24, task_2C0, 0, 255, 161, 64);
  return;
}

static void script_2A66(void) {
  if (task_2C0 != -1) {
    Script_MT5Loader_TRWV_TRWAVE(3, task_2C0);
  }
  task_2C0 = -1;
  return;
}

static int32_t script_2A84(int32_t arg0, int32_t arg1) {
  Vector3 local_F4;
  float local_F0;
  int32_t local_EC;
  if (task_2C0 != -1) {
    if (arg1 > 0) {
      SCNF_WaitFrames_0000(arg1);
    }
    Script_EVT_GetPosition(arg0, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
    local_F0 = 0.01f;
    Script_MT5Loader_TRWV_TRWAVE(13, task_2C0, local_F0, 1.0f);
    Script_MT5Loader_TRWV_TRWAVE(14, task_2C0, -0.5f, 0.103921592f);
    Script_MT5Loader_TRWV_TRWAVE(500, task_2C0, arg0);
    local_EC = 0;
    for (;;) {
      Script_EVT_GetPosition(arg0, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
      (&local_F4)->y = (&local_F4)->y + 0.0449999981f;
      Script_MT5Loader_TRWV_TRWAVE(5, task_2C0, &local_F4);
      local_F0 = local_F0 * 0.95f;
      Script_MT5Loader_TRWV_TRWAVE(13, task_2C0, local_F0, 0.6f);
      local_EC = local_EC + 1;
      if (local_EC >= 120) {
        local_F4.x = 0.0f;
        (&local_F4)->y = 1000.0f;
        (&local_F4)->z = 0.0f;
        Script_MT5Loader_TRWV_TRWAVE(5, task_2C0, &local_F4);
        Script_srchControl(0);
      }
      SCNF_WaitFrames_0000(1);
    }
  }
  end:;
}

static int32_t script_2B87(int32_t arg0, int32_t arg1, float arg2, int32_t arg3, const void *arg4) {
  SCNF_WaitFrames_0000(1);
  task_2C4[arg1] = arg0;
  task_2E0[arg1] = arg3;
  script_2C1D(arg1, arg2, arg4);
  for (;;) {
    if (task_2FC[arg1]) {
      task_334[arg1] = Script_walk3D(ActivateGetIdx, arg0, task_350[arg1]);
    }
    if (task_334[arg1] != -1) {
      task_318[arg1] = task_318[arg1] + 1;
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_2C1D(int32_t arg0, float arg1, const void *arg2) {
  int32_t local_FC;
  task_2FC[arg0] = 0;
  task_318[arg0] = 0;
  task_334[arg0] = 0;
  task_350[arg0] = arg1;
  local_FC = task_2E0[arg0];
  if (local_FC == 0) {
    Script_PlayCharaMotion(task_2C4[arg0], MN_AKI_AKI_L_STAND, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    goto L002C93;
  }
  L002C93:;
  Script_walk3D(SetupPath, task_2C4[arg0], arg2, 0);
  return;
}

static void script_2CAC(int32_t arg0, float arg1) {
  int32_t local_FC;
  task_350[arg0] = arg1;
  task_318[arg0] = 0;
  task_2FC[arg0] = 1;
  local_FC = task_2E0[arg0];
  if (local_FC == 0) {
    Script_charaToggleMotEnabled(task_2C4[arg0], 0);
    Script_SetCharaMotionPlaybackRate(task_2C4[arg0], 1.0f);
    Script_PlayCharaMotion(task_2C4[arg0], MN_AKI_AKI_WALK_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    goto L002D38;
  }
  L002D38:;
  return;
}

static void script_2D39(int32_t arg0, int32_t arg1, int32_t arg2) {
  Script_scenCreateTask(script_2D4D, 3, arg0, arg1, arg2);
  return;
}

static int32_t script_2D4D(int32_t arg0, float arg1, int32_t arg2) {
  if (arg2 >= 0) {
    SCNF_WaitFrames_0000(arg2);
  }
  script_2CAC(arg0, arg1);
  return Script_srchControl(0);
}

static void script_2D72(int32_t arg0) {
  int32_t local_FC;
  task_2FC[arg0] = 0;
  local_FC = task_2E0[arg0];
  if (local_FC == 0) {
    Script_charaToggleMotEnabled(task_2C4[arg0], 1);
    Script_SetCharaMotionPlaybackRate(task_2C4[arg0], 1.0f);
    goto L002DBE;
  }
  L002DBE:;
  return;
}

static void script_2DBF(int32_t arg0, int32_t arg1) {
  Script_scenCreateTask(script_2DD1, 2, arg0, arg1);
  return;
}

static int32_t script_2DD1(int32_t arg0, int32_t arg1) {
  if (arg1 >= 0) {
    SCNF_WaitFrames_0000(arg1);
  }
  script_2D72(arg0);
  return Script_srchControl(0);
}

static int32_t script_2DF4(void) {
  Script_shenSetLightingMode(2);
  Script_SetupLightFromTable(1);
  Script_ProcessFogFromTable(0);
  Script_PREP_Related(-1, 1);
  Script_SetCurrentScrollEnabled(0);
  Script_SetFogFlags(2, 0x2C2E2F);
  for (;;) {
    if (task_8 == 1) {
      Script_srchControl(0);
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_2E40(void) {
  Script_SetMapFlags(0, 0);
  Script_GetSetWeatherSettings(WEATHER_SETTINGS_SET_DISABLE_WEATHER, 0);
  for (;;) {
    if (task_8 == 1) {
      Script_srchControl(0);
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_2E70(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Script_scenCreateTask(script_2E86, 4, arg0, arg1, arg2, arg3);
  return;
}

static int32_t script_2E86(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  if (arg3 >= 0) {
    SCNF_WaitFrames_0000(arg3);
  }
  Script_seqEnterEvtCam(arg0, arg1, arg2);
  return Script_srchControl(0);
}

static void script_2EAA(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Script_scenCreateTask(script_2EC0, 4, arg0, arg1, arg2, arg3);
  return;
}

static int32_t script_2EC0(float arg0, float arg1, float arg2, int32_t arg3) {
  if (arg3 >= 0) {
    SCNF_WaitFrames_0000(arg3);
  }
  Script_SetCamShake(arg0, arg1, arg2);
  return Script_srchControl(0);
}

static void script_2EE5(int32_t arg0, int32_t arg1) {
  Script_scenCreateTask(script_2EF7, 2, arg0, arg1);
  return;
}

static int32_t script_2EF7(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  if (arg1 >= 0) {
    SCNF_WaitFrames_0000(arg1);
  }
  Script_StartOrFreeSEQ(0);
  local_FC = arg0;
  switch (local_FC) {
    case 0:
      Script_ShowUIMessageFromID("E1030A001");
      break;
    case 1:
      Script_ShowUIMessageFromID("E1030A002");
      break;
    case 2:
      Script_ShowUIMessageFromID("E1030A003");
      break;
    case 3:
      Script_ShowUIMessageFromID("E1030A004");
      break;
    case 4:
      Script_ShowUIMessageFromID("E1030A005");
      break;
    case 5:
      Script_ShowUIMessageFromID("E1030A006");
      break;
    case 6:
      Script_ShowUIMessageFromID("E1030A007");
      break;
    case 7:
      Script_ShowUIMessageFromID("E1030A008");
      break;
    case 8:
      Script_ShowUIMessageFromID("E1030A009");
      break;
    case 9:
      Script_ShowUIMessageFromID("E1030A010");
      break;
    case 10:
      Script_ShowUIMessageFromID("E1030A011");
      break;
    case 11:
      Script_ShowUIMessageFromID("E1030A012");
      break;
    case 12:
      Script_ShowUIMessageFromID("E1030A013");
      break;
    case 13:
      Script_ShowUIMessageFromID("E1030A014");
      break;
    case 14:
      Script_ShowUIMessageFromID("E1030A015");
      break;
    case 15:
      Script_ShowUIMessageFromID("E1030A016");
      break;
    case 16:
      Script_ShowUIMessageFromID("E1030A017");
      break;
    case 17:
      Script_ShowUIMessageFromID("E1030A018");
      break;
    case 18:
      Script_ShowUIMessageFromID("E1030A019");
      break;
    case 19:
      Script_ShowUIMessageFromID("E1030A020");
      break;
    case 20:
      Script_ShowUIMessageFromID("E1030A021");
      break;
    case 21:
      Script_ShowUIMessageFromID("E1030B001");
      break;
    case 22:
      Script_ShowUIMessageFromID("E1030B002");
      break;
    case 23:
      Script_ShowUIMessageFromID("E1030B003");
      break;
    case 24:
      Script_ShowUIMessageFromID("E1030B004");
      break;
    case 25:
      Script_ShowUIMessageFromID("E1030B005");
      break;
    case 26:
      Script_ShowUIMessageFromID("E1030G001");
      break;
    case 27:
      Script_ShowUIMessageFromID("E1030G002");
      break;
    case 28:
      Script_ShowUIMessageFromID("E1030G003");
      break;
    case 29:
      Script_ShowUIMessageFromID("E1030G004");
      break;
    case 30:
      Script_ShowUIMessageFromID("E1030D001");
      break;
    case 31:
      Script_ShowUIMessageFromID("E1030D002");
      break;
    case 32:
      Script_ShowUIMessageFromID("E1030D003");
      break;
    case 33:
      Script_ShowUIMessageFromID("E1030D004");
      break;
    case 34:
      Script_ShowUIMessageFromID("E1030D005");
      break;
    case 35:
      Script_ShowUIMessageFromID("E1030D006");
      break;
    case 36:
      Script_ShowUIMessageFromID("E1030E001");
      break;
    case 37:
      Script_ShowUIMessageFromID("E1030E002");
      break;
    case 38:
      Script_ShowUIMessageFromID("E1030E003");
      break;
    case 39:
      Script_ShowUIMessageFromID("E1030E004");
      break;
    case 40:
      Script_ShowUIMessageFromID("E1030E005");
      break;
    case 41:
      Script_ShowUIMessageFromID("E1030E006");
      break;
    case 42:
      Script_ShowUIMessageFromID("E1030E007");
      break;
    case 43:
      Script_ShowUIMessageFromID("E1030E008");
      break;
    case 44:
      Script_ShowUIMessageFromID("E1030E009");
      break;
    case 45:
      Script_ShowUIMessageFromID("E1030E010");
      break;
    case 46:
      Script_ShowUIMessageFromID("E1030E011");
      break;
    case 47:
      Script_ShowUIMessageFromID("E1030E012");
      break;
    case 48:
      Script_ShowUIMessageFromID("E1030E013");
      break;
    case 49:
      Script_ShowUIMessageFromID("E1030E014");
      break;
    case 50:
      Script_ShowUIMessageFromID("E1030E015");
      break;
    case 51:
      Script_ShowUIMessageFromID("E1030E016");
      break;
    case 52:
      Script_ShowUIMessageFromID("E1030E017");
      break;
    case 53:
      Script_ShowUIMessageFromID("E1030E018");
      break;
    case 54:
      Script_ShowUIMessageFromID("E1030E019");
      break;
  }
  return Script_srchControl(0);
}

static void script_33A1(int32_t arg0, int32_t arg1) {
  task_36C = arg0;
  task_0 = 0;
  return;
}

static void script_33B2(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  if (arg1 >= 0) {
    SCNF_WaitFrames_0000(arg1);
  }
  local_FC = arg0;
  switch (local_FC) {
    case 0:
      SCNT_ShowSubtitle("オート０");
      break;
    case 1:
      SCNT_ShowSubtitle("オート１");
      break;
    case 2:
      SCNT_ShowSubtitle("開始０");
      break;
    case 3:
      SCNT_ShowSubtitle("成功０");
      break;
    case 4:
      SCNT_ShowSubtitle("失敗０");
      break;
    case 5:
      SCNT_ShowSubtitle("死亡０");
      break;
    case 6:
      SCNT_ShowSubtitle("開始１");
      break;
    case 7:
      SCNT_ShowSubtitle("成功１");
      break;
    case 8:
      SCNT_ShowSubtitle("失敗１");
      break;
    case 9:
      SCNT_ShowSubtitle("開始２");
      break;
    case 10:
      SCNT_ShowSubtitle("成功２");
      break;
    case 11:
      SCNT_ShowSubtitle("失敗２");
      break;
    case 12:
      SCNT_ShowSubtitle("開始３");
      break;
    case 13:
      SCNT_ShowSubtitle("成功３");
      break;
    case 14:
      SCNT_ShowSubtitle("失敗３");
      break;
    case 15:
      SCNT_ShowSubtitle("開始４");
      break;
    case 16:
      SCNT_ShowSubtitle("成功４");
      break;
    case 17:
      SCNT_ShowSubtitle("失敗４");
      break;
    case 18:
      SCNT_ShowSubtitle("開始５");
      break;
    case 19:
      SCNT_ShowSubtitle("成功５");
      break;
    case 20:
      SCNT_ShowSubtitle("失敗５");
      break;
    case 21:
      SCNT_ShowSubtitle("開始６");
      break;
    case 22:
      SCNT_ShowSubtitle("成功６");
      break;
    case 23:
      SCNT_ShowSubtitle("失敗６");
      break;
  }
  Script_srchControl(0);
  return;
}

static void script_356A(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Script_scenCreateTask(script_3580, 4, arg0, arg1, arg2, arg3);
  return;
}

static int32_t script_3580(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  if (arg3 > 0) {
    SCNF_WaitFrames_0000(arg3);
  }
  if (arg1) {
    Script_charaDisplay(arg0, CDM_Show);
  } else {
    Script_charaDisplay(arg0, CDM_Hide);
  }
  Script_scenToggleActiveFor(arg0, arg2);
  return Script_srchControl(0);
}

static void script_35BC(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  Script_scenCreateTask(script_35D4, 5, arg0, arg1, arg2, arg3, arg4);
  return;
}

static int32_t script_35D4(int32_t arg0, int32_t arg1, float arg2, int32_t arg3, int32_t arg4) {
  Vector3 local_F4;
  int32_t local_F0;
  if (arg3 > 0) {
    SCNF_WaitFrames_0000(arg3);
  }
  local_F0 = arg4;
  while (local_F0 > 0) {
    Script_EVT_GetPosition(arg1, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
    (&local_F4)->y = (&local_F4)->y + arg2;
    Script_SetLookPointDetails(arg0, 98308, &local_F4, 0);
    local_F0 = local_F0 - 1;
    SCNF_WaitFrames_0000(1);
  }
  Script_SetCharaLookAtPosition(arg0, -98308, 0, 0);
  return Script_srchControl(0);
}

static void script_364F(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5, int32_t arg6) {
  Script_scenCreateTask(script_366B, 7, arg2, arg6, arg3, arg4, arg0, arg1, arg5);
  return;
}

static int32_t script_366B(int32_t arg0, int32_t arg1, int32_t arg2, float arg3, int32_t arg4, int32_t arg5, int32_t arg6) {
  if (arg1 > 0) {
    SCNF_WaitFrames_0000(arg1);
  }
  Script_charaToggleMotEnabled(arg4, 1);
  Script_PlayCharaMotion(arg4, arg0, arg2, -1.0f, -1.0f, 1.0f, 0);
  Script_SetCharaMotionPlaybackRate(arg4, arg3);
  if (arg6) {
    Script_motmToggleFlags(arg4, 1, 0x80);
  }
  return Script_srchControl(0);
}

static void script_36C1(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  Script_scenCreateTask(script_36D9, 5, arg0, arg1, arg2, arg3, arg4);
  return;
}

static int32_t script_36D9(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  if (arg4 >= 0) {
    SCNF_WaitFrames_0000(arg4);
  }
  Script_FACE(arg0, arg1, arg2, arg3);
  return Script_srchControl(0);
}

static void script_3700(void) {
  Script_charaDisplay(SHEN_NAME('MLK1'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('MLK0'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('TBN1'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('TBN0'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('CIS4'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('CIS2'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('CIS1'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('CIS0'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('CB11'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('CB10'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('CB21'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('CB20'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('CB31'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('CB30'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('TBI1'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('TBI0'), CDM_Hide);
  return;
}

static int32_t script_37B3(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  int32_t unused_F8;
  int32_t local_F4;
  int32_t local_F0;
  int32_t unused_EC;
  GameDateTime local_E4;
  int32_t local_C8[7];
  int32_t unused_C4;
  GameDateTime local_BC;
  int8_t local_BB;
  int8_t local_BA;
  uint8_t unused_B8[2];
  SCNF_WaitFrames_0000(5);
  local_F4 = Script_scenCreateTask(script_2B87, 5, arg0, arg1, 0x3DCCCCCD, 0, &data_6A88);
  SCNF_WaitFrames_0000(5);
  local_F0 = Script_SetMaxNPCsNear(7);
  Script_SetSfxOverlayTimeoutTuning(0, SfxMode_SessionAction);
  Script_uiUtils(0, 21, 0);
  script_14A9(3);
  Script_PlayerStatsUtils(PS_IncrementSpotVisitCount, 42);
  Script_ReturnTimeDate(&local_E4);
  for (;;) {
    Script_SetNewDateTime(&local_E4);
    if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_DBYO_DJAZ_DHQB_CHAIN_ACTIVE) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_DHQB_CHALLENGE_COMPLETE) == 0)) {
    } else {
      Script_filmCheckOrSetTransitionUtil(0, 255, 0, 0, 0, 0, 0, 0, 0);
      for (;;) {
        Script_PrintDebugString(15, 14, "PRESS A BUTTON");
        if (Script_GetInputData(0, 2) & 0x400) {
          break;
        }
        SCNF_WaitFrames_0000(1);
      }
    }
    Script_filmCheckOrSetTransitionUtil(60, 255, 0, 0, 0, 0, 0, 0, 0);
    Script_Ambient1SoundUtils(1, 24232);
    script_3700();
    script_46B3(arg0, arg1);
    script_12DB(0, 3);
    Script_executeAicaCommand(2720, 1, 5);
    Script_executeAicaCommand(2720, 3, 5);
    for (;;) {
      Script_executeAicaCommand(156840, 0, 0);
      Script_executeAicaCommand(1184, 2, 127);
      local_FC = Script_scenCreateTask(script_3B03, 2, arg0, arg1);
      task_13C = 0;
      script_13C0(0, 3);
      for (;;) {
        if (task_13C >= 10) {
          Script_srchControl((void *)local_FC);
          break;
        } else {
          if (task_13C == -1) {
            Script_srchControl((void *)local_FC);
            break;
            goto L00393F;
          }
        }
        L00393F:;
        SCNF_WaitFrames_0000(1);
      }
      Script_executeAicaCommand(2720, 2, 30);
      if (task_13C == -1) {
        script_4E64(arg0, arg1);
        Script_SetSfxOverlayTimeoutTuning(2, SfxMode_SessionAction);
        script_150F();
        if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_DBYO_DJAZ_DHQB_CHAIN_ACTIVE) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_DHQB_CHALLENGE_COMPLETE) == 0)) {
          Script_SetMaxNPCsNear(local_F0);
          Script_ReturnTimeDate(&local_E4);
          if ((&local_E4)->hours <= 4) {
            Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 197, 1);
          }
          Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_DHQB_CHALLENGE_COMPLETE, 1);
          Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_DHQB_POST_CHALLENGE_RETURN_HOME, 1);
          Script_ReturnTimeDate(&local_BC);
          if ((&local_BC)->month == 12) {
            local_BA = 0;
          } else {
            local_BA = (&local_BC)->month;
          }
          local_BB = (local_BA << 5) | (&local_BC)->day;
          Script_scenUtils(SCENARIO_CMD_SET_STEP_LUT, 12, local_BB);
          local_C8[0] = 22;
          local_C8[1] = 30;
          local_C8[2] = 23;
          local_C8[3] = 30;
          local_C8[4] = 1;
          local_C8[5] = 0x4F4D4F4A;
          local_C8[6] = 3;
          Script_SetHomeArea((const void *)local_C8);
          Script_shenRequestGameModuleTransition(1, SHEN_NAME('D000'), 3);
          SCNF_WaitFrames_0000(1);
        } else {
          Script_executeAicaCommand(16296, 0, 0);
          Script_executeAicaCommand(1184, 1, 127);
          break;
        }
      } else {
        Script_filmCheckOrSetTransitionUtil(60, 0, 0, 0, 0, 255, 0, 0, 0);
        while (Script_filmCheckOrSetTransitionUtil(-1)) {
          SCNF_WaitFrames_0000(1);
        }
        Script_sicnUtils(0, 1, 1);
        SCNF_WaitFrames_0000(90);
        Script_sicnUtils(2, 1, 0);
        Script_SetSfxOverlayTimeoutTuning(1, SfxMode_SessionAction);
        script_14C7();
        Script_executeAicaCommand(16296, 0, 0);
        Script_executeAicaCommand(1184, 1, 127);
        break;
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_3B03(int32_t arg0, int32_t arg1) {
  Script_charaToggleMotEnabled(arg0, 1);
  Script_charaToggleMotEnabled(task_B8[1], 1);
  Script_charaToggleMotEnabled(task_B8[2], 1);
  Script_charaToggleMotEnabled(task_B8[3], 1);
  Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_7);
  Script_motmSetBlendFrames(task_B8[1], MOTM_BLEND_FRAMES_7);
  Script_motmSetBlendFrames(task_B8[2], MOTM_BLEND_FRAMES_7);
  Script_motmSetBlendFrames(task_B8[3], MOTM_BLEND_FRAMES_7);
  script_0F5F(task_B8[1], arg0, 0.82703f, 0.42759f, 79.755699f);
  script_0F5F(task_B8[5], -1, 0.6f, -3.0f, 270.0f);
  script_33A1(2, -1);
  script_356A(arg0, 1, 0, -1);
  script_356A(task_B8[1], 1, 0, -1);
  script_356A(task_B8[2], 1, 0, -1);
  script_356A(task_B8[3], 1, 0, -1);
  script_356A(task_B8[5], 1, 0, -1);
  script_356A(SHEN_NAME('MLK0'), 0, 0, -1);
  script_2E70(8081, SHEN_NAME('DUM1'), SHEN_NAME('DUM1'), -1);
  script_36C1(arg0, 2, 0, 16, -1);
  script_0B54(arg1, 0, 1, 0);
  if (task_130 == 0) {
    script_33A1(4, -1);
    script_36C1(arg0, 4, 0, 8, -1);
    script_36C1(arg0, 2, 0, 16, 16);
    script_0B54(arg1, 1, 1, 0);
    if (task_130 == 0) {
      script_33A1(8, -1);
      script_36C1(arg0, 4, 0, 8, -1);
      script_2E70(8082, SHEN_NAME('DUM0'), SHEN_NAME('DUM0'), -1);
      Script_scenCreateTask(script_2579, 3, SHEN_NAME('CB10'), SHEN_NAME('CB11'), 22);
      SCNF_WaitFrames_0000(30);
      task_13C = 10;
      SCNF_WaitFrames_0000(1);
    } else {
      script_33A1(7, -1);
      script_2EE5(24, 30);
      script_2E70(8082, SHEN_NAME('DUM0'), SHEN_NAME('DUM0'), -1);
      script_2E70(8088, SHEN_NAME('DUM0'), SHEN_NAME('DUM0'), 40);
      script_2EAA(0x3E4CCCCD, 0x3E4CCCCD, 0x3F333333, 45);
      script_364F(task_B8[5], 5, 20537, 0, 0x3F800000, 0, 10);
      script_364F(task_B8[5], 5, 20538, 0, 0x3F800000, 0, 99);
      script_364F(task_B8[5], 5, 20539, 0, 0x3F800000, 0, 149);
      script_1021(task_B8[1], -1, 0x3FE66666, -0x3F600000, 0x43480000, 40);
      Script_scenCreateTask(script_230E, 3, SHEN_NAME('CB10'), SHEN_NAME('CB11'), 28);
      Script_scenCreateTask(script_241F, 3, SHEN_NAME('CB20'), SHEN_NAME('CB21'), 35);
    }
  } else {
    script_33A1(3, -1);
    script_2EE5(24, 30);
    script_2E70(8082, SHEN_NAME('DUM0'), SHEN_NAME('DUM0'), -1);
    script_2E70(8088, SHEN_NAME('DUM0'), SHEN_NAME('DUM0'), 40);
    script_2EAA(0x3E4CCCCD, 0x3E4CCCCD, 0x3F333333, 45);
    script_364F(task_B8[5], 5, 20537, 0, 0x3F800000, 0, 10);
    script_364F(task_B8[5], 5, 20538, 0, 0x3F800000, 0, 99);
    script_364F(task_B8[5], 5, 20539, 0, 0x3F800000, 0, 149);
    script_1021(task_B8[1], -1, 0x3FE66666, -0x3F600000, 0x43480000, 40);
    Script_scenCreateTask(script_230E, 3, SHEN_NAME('CB10'), SHEN_NAME('CB11'), 28);
    Script_scenCreateTask(script_241F, 3, SHEN_NAME('CB20'), SHEN_NAME('CB21'), 35);
  }
  script_36C1(arg0, 5, 0, 16, -1);
  script_36C1(arg0, 2, 0, 16, 30);
  script_35BC(arg0, task_B8[1], 0, 30, 60);
  SCNF_WaitFrames_0000(30);
  script_364F(task_B8[2], 2, 20517, 0, 0x3F800000, 0, -1);
  script_364F(task_B8[3], 3, 20521, 0, 0x3F800000, 0, -1);
  SCNF_WaitFrames_0000(30);
  script_0F5F(arg0, -1, 2.7614f, -4.1403f, 90.0f);
  script_0F5F(task_B8[2], arg0, -1.48452f, 1.66446f, -153.177994f);
  script_33A1(9, -1);
  script_2EE5(26, 30);
  script_36C1(arg0, 7, 0, 4, 46);
  script_356A(arg0, 1, 0, -1);
  script_356A(task_B8[1], 1, 0, -1);
  script_356A(task_B8[2], 1, 0, -1);
  script_356A(task_B8[3], 1, 0, -1);
  script_2E70(8083, SHEN_NAME('DUM1'), SHEN_NAME('DUM1'), -1);
  script_0B54(arg1, 2, 2, 0);
  if (task_130 == 0) {
    script_0F5F(task_B8[2], arg0, -0.78292f, 0.86011f, -55.9207f);
    script_33A1(11, -1);
    script_36C1(arg0, 4, 0, 8, -1);
    script_36C1(arg0, 2, 0, 16, 16);
    script_0B54(arg1, 3, 2, 0);
    if (task_130 == 0) {
      script_11F4(task_B8[2], 0x42700000, -1);
      script_33A1(14, -1);
      script_2EE5(29, 60);
      script_36C1(arg0, 4, 0, 8, -1);
      SCNF_WaitFrames_0000(60);
      task_13C = 10;
      SCNF_WaitFrames_0000(1);
    } else {
      script_33A1(13, -1);
      script_2EE5(27, 30);
      script_2E70(8089, SHEN_NAME('DUM1'), SHEN_NAME('DUM1'), -1);
      script_2E70(8084, SHEN_NAME('DUM0'), SHEN_NAME('DUM0'), 20);
      script_2EAA(0x3E4CCCCD, 0x3E4CCCCD, 0x3F333333, 30);
      Script_scenCreateTask(script_279D, 3, SHEN_NAME('TBI0'), SHEN_NAME('TBI1'), 17);
    }
  } else {
    script_33A1(10, -1);
    script_2EE5(27, 30);
    script_2E70(8089, SHEN_NAME('DUM1'), SHEN_NAME('DUM1'), -1);
    script_2E70(8084, SHEN_NAME('DUM0'), SHEN_NAME('DUM0'), 20);
    script_2EAA(0x3E4CCCCD, 0x3E4CCCCD, 0x3F333333, 30);
    script_36C1(arg0, 2, 0, 16, 16);
    Script_scenCreateTask(script_279D, 3, SHEN_NAME('TBI0'), SHEN_NAME('TBI1'), 20);
  }
  SCNF_WaitFrames_0000(42);
  script_1021(arg0, task_B8[3], 0x3F84BFB1, -0x4079D3ED, 0x43020000, 28);
  script_1021(task_B8[5], -1, 0x3F800000, -0x3FE00000, 0x43870000, 28);
  script_33A1(15, -1);
  script_2EE5(30, 48);
  script_35BC(arg0, task_B8[3], 0x3F99999A, 28, 20);
  script_356A(arg0, 1, 0, 28);
  script_356A(task_B8[1], 1, 0, 28);
  script_356A(task_B8[2], 1, 0, 28);
  script_356A(task_B8[3], 1, 0, 28);
  script_356A(SHEN_NAME('CIS2'), 1, 0, 28);
  script_356A(SHEN_NAME('CIS1'), 0, 0, 28);
  script_356A(SHEN_NAME('CIS4'), 0, 0, 28);
  script_2E70(8087, SHEN_NAME('DUM1'), SHEN_NAME('DUM1'), 28);
  script_0B54(arg1, 4, 3, 0);
  if (task_130 == 0) {
    script_33A1(17, -1);
    script_2E70(8090, SHEN_NAME('DUM0'), SHEN_NAME('DUM0'), -1);
    script_36C1(arg0, 4, 0, 8, -1);
    script_1021(arg0, task_B8[3], 0x3F3CB296, -0x40C074A7, 0x43020000, -1);
    SCNF_WaitFrames_0000(30);
    task_13C = 10;
    SCNF_WaitFrames_0000(1);
  } else {
    script_33A1(16, -1);
    script_2E70(8090, SHEN_NAME('DUM0'), SHEN_NAME('DUM0'), -1);
    script_356A(SHEN_NAME('CIS2'), 0, 0, 28);
    script_1021(arg0, task_B8[3], 0x3F84BFB1, -0x4079D3ED, 0x43020000, -1);
    Script_scenCreateTask(script_21FA, 3, SHEN_NAME('CIS0'), SHEN_NAME('CIS1'), 28);
  }
  SCNF_WaitFrames_0000(2);
  script_33A1(18, -1);
  script_0B54(arg1, 5, 3, 0x10000000);
  if (task_130 == 0) {
    SCNF_WaitFrames_0000(10);
    script_1021(arg0, task_B8[3], -0x4158F9B1, -0x406AF1AA, 0x434F0000, -1);
    script_1021(task_B8[3], -1, 0x4019999A, -0x3FC00000, 0x439D8000, -1);
    script_33A1(20, -1);
    script_2EE5(31, -1);
    script_2E70(8091, SHEN_NAME('DUM1'), SHEN_NAME('DUM1'), -1);
    script_36C1(arg0, 4, 0, 8, -1);
    script_36C1(arg0, 2, 0, 16, 16);
    script_0B54(arg1, 6, 3, 0x10000000);
    if (task_130 == 0) {
      script_33A1(23, -1);
      script_2EE5(33, 10);
      script_36C1(arg0, 4, 0, 8, -1);
      script_11F4(task_B8[3], -0x3E100000, -1);
      SCNF_WaitFrames_0000(50);
      task_13C = 10;
      SCNF_WaitFrames_0000(1);
    } else {
      script_33A1(22, -1);
      script_2EE5(32, 20);
      script_364F(task_B8[5], 5, 20536, 0, 0x3F800000, 0, 20);
    }
  } else {
    script_1021(arg0, task_B8[3], -0x4158F9B1, -0x4077BE77, 0x434F0000, -1);
    script_1021(task_B8[3], -1, 0x40200000, -0x3FC00000, 0x439D8000, -1);
    script_33A1(19, -1);
    script_2EE5(32, 20);
    script_2E70(8085, SHEN_NAME('DUM1'), SHEN_NAME('DUM1'), -1);
    script_2E70(8086, SHEN_NAME('DUM0'), SHEN_NAME('DUM0'), 45);
    script_2E70(8085, SHEN_NAME('DUM1'), SHEN_NAME('DUM1'), 80);
    script_2EAA(0x3E4CCCCD, 0x3E4CCCCD, 0x3F333333, 50);
    script_364F(task_B8[5], 5, 20536, 0, 0x3F800000, 0, 20);
    Script_scenCreateTask(script_268B, 3, SHEN_NAME('CB30'), SHEN_NAME('CB31'), 26);
    Script_scenCreateTask(script_2088, 3, SHEN_NAME('TBN0'), SHEN_NAME('TBN1'), 50);
    SCNF_WaitFrames_0000(50);
  }
  SCNF_WaitFrames_0000(90);
  task_13C = -1;
  SCNF_WaitFrames_0000(1);
  end:;
}

static void script_46B3(int32_t arg0, int32_t arg1) {
  Script_charaToggleMotEnabled(arg0, 1);
  Script_charaToggleMotEnabled(task_B8[1], 1);
  Script_charaToggleMotEnabled(task_B8[2], 1);
  Script_charaToggleMotEnabled(task_B8[3], 1);
  Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_IMMEDIATE);
  Script_motmSetBlendFrames(task_B8[1], MOTM_BLEND_FRAMES_IMMEDIATE);
  Script_motmSetBlendFrames(task_B8[2], MOTM_BLEND_FRAMES_IMMEDIATE);
  Script_motmSetBlendFrames(task_B8[3], MOTM_BLEND_FRAMES_IMMEDIATE);
  script_33A1(0, -1);
  script_2E70(8093, SHEN_NAME('DUM0'), SHEN_NAME('DUM0'), -1);
  script_2E70(8094, SHEN_NAME('DUM0'), SHEN_NAME('DUM0'), 183);
  script_2E70(8095, SHEN_NAME('DUM0'), SHEN_NAME('DUM0'), 250);
  script_2E70(8097, SHEN_NAME('DUM0'), SHEN_NAME('DUM0'), 483);
  script_2E70(8098, SHEN_NAME('DUM0'), SHEN_NAME('DUM0'), 620);
  script_2E70(8099, SHEN_NAME('DUM0'), SHEN_NAME('DUM0'), 792);
  script_2E70(8100, SHEN_NAME('DUM0'), SHEN_NAME('DUM0'), 301);
  script_2E70(8101, SHEN_NAME('DUM0'), SHEN_NAME('DUM0'), 57);
  script_36C1(arg0, 0, 0, 16, -1);
  script_36C1(arg0, 2, 0, 16, 360);
  script_1021(arg0, -1, 0x3F4CCCCD, -0x3F066666, 0x43700000, -1);
  script_1021(arg0, -1, 0x40600000, -0x3F600000, 0x42F00000, 183);
  script_1021(task_B8[1], -1, 0x40D9999A, -0x3FACCCCD, 0x42B40000, -1);
  script_1021(task_B8[2], -1, 0x40933333, -0x3FE66666, 0x43870000, -1);
  script_1021(task_B8[3], -1, 0x402CCCCD, -0x3FE00000, -0x3EE00000, -1);
  script_1021(task_B8[5], -1, 0x3F800000, -0x3FC00000, 0x43870000, -1);
  script_1021(SHEN_NAME('DOR0'), -1, 0, -0x3EFE6666, 0x43110000, -1);
  script_1021(SHEN_NAME('DOR0'), -1, 0, -0x3EFE6666, 0x43340000, 70);
  script_2EE5(36, 220);
  script_2EE5(35, 295);
  script_2EE5(54, 430);
  script_2EE5(10, 500);
  script_2EE5(37, 560);
  script_2EE5(11, 600);
  script_364F(arg0, arg1, 20531, 0, 0x3F800000, 0, -1);
  script_364F(arg0, arg1, 20532, 0x41A00000, 0x3F800000, 0, 183);
  script_364F(task_B8[1], 1, 20503, 0, 0x3F800000, 0, -1);
  script_364F(task_B8[2], 2, 20517, 0, 0x3F800000, 0, -1);
  script_364F(task_B8[3], 3, 20522, 0, 0x3F800000, 0, -1);
  script_364F(task_B8[5], 5, 20534, 0, 0x3F800000, 0, -1);
  script_364F(task_B8[5], 5, 20533, 0, 0x3F800000, 0, 10);
  script_356A(arg0, 1, 1, -1);
  script_356A(task_B8[1], 1, 0, -1);
  script_356A(task_B8[2], 1, 0, -1);
  script_356A(task_B8[3], 1, 0, -1);
  script_356A(task_B8[5], 1, 0, -1);
  script_35BC(arg0, SHEN_NAME('MLK0'), 0, 300, 120);
  script_35BC(arg0, task_B8[5], 0x3F99999A, 560, 200);
  script_35BC(task_B8[1], task_B8[2], 0x3F99999A, -1, 100);
  script_35BC(task_B8[1], arg0, 0x3F99999A, 100, 120);
  script_35BC(task_B8[2], arg0, 0x3F99999A, -1, 300);
  script_35BC(task_B8[3], task_B8[5], 0x3F99999A, -1, 120);
  script_35BC(task_B8[3], task_B8[5], 0x3F4CCCCD, 450, 120);
  Script_scenCreateTask(script_05FD, 2, 0x2004A9, 60);
  Script_scenCreateTask(script_05FD, 2, 0x1C04A9, 125);
  Script_scenCreateTask(script_05FD, 2, 0x1C04A9, 157);
  Script_scenCreateTask(script_05FD, 2, 0x1C04A9, 174);
  Script_scenCreateTask(script_05FD, 2, 0x1C04A9, 200);
  Script_scenCreateTask(script_05FD, 2, 0x1C04A9, 232);
  Script_scenCreateTask(script_28C7, 3, SHEN_NAME('MLK0'), SHEN_NAME('MLK1'), 260);
  SCNF_WaitFrames_0000(483);
  script_33A1(0, -1);
  Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_25);
  Script_motmSetBlendFrames(task_B8[1], MOTM_BLEND_FRAMES_25);
  Script_motmSetBlendFrames(task_B8[2], MOTM_BLEND_FRAMES_25);
  Script_motmSetBlendFrames(task_B8[3], MOTM_BLEND_FRAMES_25);
  script_1021(arg0, -1, 0x402CCCCD, -0x3F800000, 0x43160000, -1);
  script_1021(arg0, -1, 0x402CCCCD, -0x3F800000, 0x42B40000, 137);
  script_1021(task_B8[1], arg0, 0x3F1FEE4E, 0x3FFE6080, 0, 213);
  script_1021(task_B8[1], arg0, 0x3F53B83D, 0x3EDAED14, 0x429F82EB, 309);
  script_2EE5(21, 200);
  script_2EE5(0, 300);
  script_2EE5(22, 340);
  script_2EE5(23, 420);
  script_364F(arg0, arg1, 89, 0, 0x3F800000, 0, 160);
  script_364F(arg0, arg1, 20541, 0x3F800000, 0x3F800000, 0, 213);
  script_364F(arg0, arg1, 20481, 0, 0x3F800000, 0, 332);
  script_364F(task_B8[1], 1, 20509, 0, 0x3F800000, 0, 60);
  script_364F(task_B8[1], 1, 20542, 0x3F800000, 0x3F800000, 0, 213);
  script_364F(task_B8[1], 1, 20504, 0, 0x3F800000, 0, 332);
  script_356A(arg0, 1, 1, -1);
  script_356A(task_B8[1], 1, 0, -1);
  script_356A(task_B8[2], 1, 0, -1);
  script_356A(task_B8[3], 1, 0, -1);
  script_356A(task_B8[5], 1, 0, -1);
  script_35BC(task_B8[1], arg0, 0x3FA66666, 320, 80);
  Script_scenCreateTask(script_05FD, 2, 0x1B04A9, 176);
  Script_scenCreateTask(script_05FD, 2, 0x1B04A9, 190);
  Script_scenCreateTask(script_05FD, 2, 0x1B04A9, 206);
  Script_scenCreateTask(script_05FD, 2, 0x1B04A9, 222);
  Script_scenCreateTask(script_05FD, 2, 0x1B04A9, 235);
  Script_scenCreateTask(script_05FD, 2, 0x1B04A9, 252);
  Script_scenCreateTask(script_05FD, 2, 0x1604A9, 260);
  Script_scenCreateTask(script_05FD, 2, 984233, 310);
  SCNF_WaitFrames_0000(428);
  return;
}

static void script_4E64(int32_t arg0, int32_t arg1) {
  Script_charaToggleMotEnabled(arg0, 1);
  Script_charaToggleMotEnabled(task_B8[1], 1);
  Script_charaToggleMotEnabled(task_B8[2], 1);
  Script_charaToggleMotEnabled(task_B8[3], 1);
  Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_25);
  Script_motmSetBlendFrames(task_B8[1], MOTM_BLEND_FRAMES_25);
  Script_motmSetBlendFrames(task_B8[2], MOTM_BLEND_FRAMES_25);
  Script_motmSetBlendFrames(task_B8[3], MOTM_BLEND_FRAMES_25);
  Script_motmSetBlendFrames(task_B8[5], MOTM_BLEND_FRAMES_25);
  script_1021(arg0, -1, 0x403F0069, -0x3F94CE70, 0x43220000, 150);
  script_1021(task_B8[5], -1, 0x3F666666, -0x3FF33333, 0x43870000, 150);
  script_33A1(1, -1);
  script_2E70(8103, SHEN_NAME('DUM0'), SHEN_NAME('DUM0'), -1);
  script_2E70(8104, SHEN_NAME('DUM0'), SHEN_NAME('DUM0'), 150);
  script_2E70(8105, SHEN_NAME('DUM0'), SHEN_NAME('DUM0'), 260);
  script_2E70(8106, SHEN_NAME('DUM0'), SHEN_NAME('DUM0'), 420);
  script_2E70(8107, SHEN_NAME('DUM0'), SHEN_NAME('DUM0'), 530);
  script_2E70(8108, SHEN_NAME('DUM0'), SHEN_NAME('DUM0'), 590);
  script_2E70(8109, SHEN_NAME('DUM0'), SHEN_NAME('DUM0'), 850);
  script_2E70(8110, SHEN_NAME('DUM0'), SHEN_NAME('DUM0'), 1190);
  script_2E70(8111, SHEN_NAME('DUM0'), SHEN_NAME('DUM0'), 1440);
  script_2E70(8112, SHEN_NAME('DUM0'), SHEN_NAME('DUM0'), 1510);
  script_36C1(arg0, 2, 0, 16, -1);
  script_36C1(arg0, 7, 0, 16, 530);
  script_36C1(arg0, 2, 0, 16, 550);
  script_36C1(arg0, 7, 0, 16, 1450);
  script_36C1(arg0, 0, 0, 16, 1466);
  script_36C1(arg0, 2, 0, 16, 1482);
  script_2EE5(38, -1);
  script_2EE5(39, 60);
  script_2EE5(12, 150);
  script_2EE5(40, 260);
  script_2EE5(13, 300);
  script_2EE5(41, 330);
  script_2EE5(14, 420);
  script_2EE5(42, 460);
  script_2EE5(15, 530);
  script_2EE5(48, 590);
  script_2EE5(49, 730);
  script_2EE5(16, 850);
  script_2EE5(50, 910);
  script_2EE5(51, 1010);
  script_2EE5(19, 1110);
  script_2EE5(52, 1190);
  script_2EE5(53, 1310);
  script_2EE5(20, 1440);
  script_2EE5(47, 1510);
  script_364F(arg0, arg1, 20532, 0x43C80000, 0x3F800000, 0, -1);
  script_364F(arg0, arg1, 20532, 0x43C80000, 0x3F800000, 0, 150);
  script_364F(arg0, arg1, 20532, 0x43C80000, 0x3F800000, 0, 420);
  script_364F(arg0, arg1, 20532, 0x43C80000, 0x3F800000, 0, 590);
  script_364F(arg0, arg1, 20532, 0x43C80000, 0x3F800000, 0, 850);
  script_364F(arg0, arg1, 20532, 0x43C80000, 0x3F800000, 0, 1200);
  script_364F(arg0, arg1, 20532, 0x43C80000, 0x3F800000, 0, 1440);
  script_364F(arg0, arg1, 20532, 0x43C80000, 0x3F800000, 0, 1510);
  script_364F(task_B8[5], 5, 20533, 0x43960000, 0x3F800000, 0, -1);
  script_364F(task_B8[5], 5, 20533, 0x43960000, 0x3F800000, 0, 260);
  script_364F(task_B8[5], 5, 20533, 0x43960000, 0x3F800000, 0, 590);
  script_364F(task_B8[5], 5, 20533, 0x43960000, 0x3F800000, 0, 850);
  script_364F(task_B8[5], 5, 20533, 0x43960000, 0x3F800000, 0, 1440);
  script_364F(task_B8[5], 5, 20535, 0, 0x3F800000, 0, 1510);
  script_356A(arg0, 1, 1, -1);
  script_356A(task_B8[1], 1, 0, -1);
  script_356A(task_B8[2], 1, 0, -1);
  script_356A(task_B8[3], 1, 0, -1);
  script_356A(task_B8[5], 1, 0, -1);
  script_35BC(task_B8[5], arg0, 0x3F99999A, 1510, 110);
  SCNF_WaitFrames_0000(60);
  Script_executeAicaCommand(16296, 0, 0);
  Script_executeAicaCommand(1184, 1, 127);
  SCNF_WaitFrames_0000(1540);
  Script_filmCheckOrSetTransitionUtil(60, 0, 0, 0, 0, 255, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_executeAicaCommand(2720, 1, 30);
  SCNF_WaitFrames_0000(60);
  return;
}

void script_53E0(void) {
  Script_filmCheckOrSetTransitionUtil(0, 255, 0, 0, 0, 0, 0, 0, 0);
  SCNF_WaitFrames_0000(5);
  script_5428(0);
  task_0 = 0;
  for (;;) {
    task_0 = task_0 + 1;
    SCNF_WaitFrames_0000(1);
  }
  return;
}

__attribute__((scn_string_bindings(0, data_6D95)))
static void script_5428(int32_t arg0) {
  task_8 = 0;
  Script_LoadVoiceFile(1, "E1030");
  Script_scenCreateTask(script_2DF4, 0);
  Script_scenCreateTask(script_2E40, 0);
  Script_scenCreateTask(script_5914, 2, SHEN_NAME('DOR0'), 0);
  Script_scenCreateTask(script_5914, 2, SHEN_NAME('TBN0'), 0);
  Script_scenCreateTask(script_5914, 2, SHEN_NAME('TBN1'), 0);
  Script_scenCreateTask(script_5914, 2, SHEN_NAME('CIS0'), 0);
  Script_scenCreateTask(script_5914, 2, SHEN_NAME('CIS1'), 0);
  Script_scenCreateTask(script_595A, 3, SHEN_NAME('CIS2'), 0, CHARA_JONZ);
  Script_scenCreateTask(script_5914, 2, SHEN_NAME('CIS3'), 0);
  Script_scenCreateTask(script_5914, 2, SHEN_NAME('CIS4'), 0);
  Script_scenCreateTask(script_5914, 2, SHEN_NAME('CIS5'), 0);
  Script_scenCreateTask(script_5914, 2, SHEN_NAME('CB10'), 0);
  Script_scenCreateTask(script_5914, 2, SHEN_NAME('CB11'), 0);
  Script_scenCreateTask(script_5914, 2, SHEN_NAME('CB20'), 0);
  Script_scenCreateTask(script_5914, 2, SHEN_NAME('CB21'), 0);
  Script_scenCreateTask(script_5914, 2, SHEN_NAME('CB30'), 0);
  Script_scenCreateTask(script_5914, 2, SHEN_NAME('CB31'), 0);
  Script_scenCreateTask(script_5914, 2, SHEN_NAME('TBI0'), 0);
  Script_scenCreateTask(script_5914, 2, SHEN_NAME('TBI1'), 0);
  Script_scenCreateTask(script_5B57, 2, SHEN_NAME('MLK0'), 0);
  Script_scenCreateTask(script_5A5E, 3, SHEN_NAME('MLK1'), 0, CHARA_HATO);
  Script_scenCreateTask(script_583B, 4, CHARA_SERA, 1, 0, 0);
  Script_scenCreateTask(script_583B, 4, CHARA_HARY, 2, 0, 0);
  Script_scenCreateTask(script_583B, 4, CHARA_JONZ, 3, 0, 0);
  Script_scenCreateTask(script_583B, 4, CHARA_HATO, 5, 0, 0);
  Script_scenCreateTask(script_56A9, 2, CHARA_AKIR, 0);
  task_4 = Script_scenCreateTask(script_37B3, 2, CHARA_AKIR, 0);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 20.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 20.0f);
  Script_SetCameraFlags(0);
  Script_SetActionSelectorHUDElementVisibility(1);
  return;
}

__attribute__((scn_string_bindings(0, data_6D9B)))
static void script_565A(void) {
  task_8 = 1;
  Script_SetActionSelectorHUDElementVisibility(0);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 100.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 100.0f);
  Script_LoadVoiceFile(0, "E1030");
  Script_executeAicaCommand(131232, 0, 0);
  Script_srchControl((void *)task_4);
  SCNF_WaitFrames_0000(1);
  return;
}

static int32_t script_56A9(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  int32_t local_F8;
  local_FC = Script_scenCreateTask(script_0D5A, 3, arg0, arg1, 0);
  local_F8 = Script_scenCreateTask(script_08C6, 6, arg0, arg1, 0, 0, &data_673C, script_1663);
  Script_facemotUtils(arg0, 0, 0);
  Script_FaceEx(arg0, 1, 2, 4);
  Script_FACE(arg0, 2, 0, 4);
  Script_FACE_LCG(arg0, 2, 4, 0);
  Script_FaceLookAtTargetUtils(arg0, 0);
  Script_ryoSetChara(arg0);
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_ccowUtils(arg0, 2);
  Script_motmGetCharaFlags(arg0, 0, 0);
  Script_motmToggleFlags(arg0, 1, 0x40);
  Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_7);
  Script_SetFightProgramStateBytes(arg0, 2, 1);
  Script_SetCPAMFlags(arg0, 0);
  Script_MoveObjectToObject(arg0, 0x304D5544);
  Script_MoveObjectToObject(SHEN_NAME('DUM1'), arg0);
  Script_MoveObjectToObject(SHEN_NAME('DUMS'), 0x41524553);
  Script_EVT_SetPosition(SHEN_NAME('DUM1'), POS_XYZ, &data_6DA4);
  Script_EVT_SetRotation(SHEN_NAME('DUM1'), 0x38000000, &data_6DB0, 0);
  Script_EVT_SetPosition(SHEN_NAME('DUMS'), POS_XYZ, &data_6DA4);
  Script_EVT_SetRotation(SHEN_NAME('DUMS'), 0x38000000, &data_6DB0, 0);
  Script_SetShadowFlags(arg0, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_scenToggleActiveFor(arg0, 0);
  Script_SetCharaFlags(arg0, 4);
  Script_charaDisplay(arg0, CDM_Show);
  for (;;) {
    if (task_8 == 1) {
      Script_faceUpdateFor(arg0, 0, 0);
      Script_srchControl((void *)task_10C[arg1]);
      Script_srchControl((void *)local_F8);
      Script_srchControl((void *)local_FC);
      Script_srchControl(0);
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_583B(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  int32_t local_F8;
  local_FC = Script_scenCreateTask(script_0D5A, 3, arg0, arg1, 0);
  local_F8 = Script_scenCreateTask(script_08C6, 6, arg0, arg1, arg2, arg3, &data_673C, script_1663);
  Script_ccowUtils(arg0, 2);
  Script_charaToggleMotEnabled(arg0, 1);
  Script_motmGetCharaFlags(arg0, 0, 0);
  Script_motmToggleFlags(arg0, 1, 0x40);
  Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_7);
  Script_SetFightProgramStateBytes(arg0, 2, 1);
  Script_SetCPAMFlags(arg0, 0);
  Script_MoveObjectToObject(arg0, 0x304D5544);
  Script_SetShadowFlags(arg0, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_scenToggleActiveFor(arg0, 0);
  Script_SetCharaFlags(arg0, 4);
  for (;;) {
    if (task_8 == 1) {
      Script_srchControl((void *)task_10C[arg1]);
      Script_srchControl((void *)local_F8);
      Script_srchControl((void *)local_FC);
      Script_srchControl(0);
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_5914(int32_t arg0, int32_t arg1) {
  Script_MoveObjectToObject(arg0, 0x304D5544);
  Script_SetShadowFlags(arg0, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_scenToggleActiveFor(arg0, 0);
  Script_SetCharaFlags(arg0, 0);
  for (;;) {
    if (task_8 == 1) {
      Script_srchControl(0);
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_595A(int32_t arg0, int32_t arg1, int32_t arg2) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t local_E4;
  Script_MoveObjectToObject(arg0, 0x304D5544);
  Script_SetShadowFlags(arg0, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_scenToggleActiveFor(arg0, 0);
  Script_SetCharaFlags(arg0, 0);
  Script_charaDisplay(arg0, CDM_Show);
  local_E4 = arg1;
  switch (local_E4) {
    case 0:
      local_F4.x = 0.086f;
      (&local_F4)->y = -0.136f;
      (&local_F4)->z = -0.041f;
      local_E8.x = 1820;
      (&local_E8)->y = 0;
      (&local_E8)->z = 64262;
      Script_fixoAttachEx(arg0, arg2, 18, &local_F4, &local_E8);
      break;
    case 1:
      local_F4.x = -0.086f;
      (&local_F4)->y = -0.136f;
      (&local_F4)->z = -0.041f;
      local_E8.x = -1820;
      (&local_E8)->y = 0;
      (&local_E8)->z = -64262;
      Script_fixoAttachEx(arg0, arg2, 12, &local_F4, &local_E8);
      break;
  }
  for (;;) {
    if (task_8 == 1) {
      Script_srchControl(0);
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_5A5E(int32_t arg0, int32_t arg1, int32_t arg2) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t local_E4;
  Script_SetShadowFlags(arg0, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_scenToggleActiveFor(arg0, 0);
  Script_SetCharaFlags(arg0, 0);
  Script_charaDisplay(arg0, CDM_Show);
  local_E4 = arg1;
  switch (local_E4) {
    case 0:
      local_F4.x = 0.067f;
      (&local_F4)->y = -0.77f;
      (&local_F4)->z = 0.01f;
      local_E8.x = 1820;
      (&local_E8)->y = 0;
      (&local_E8)->z = 64262;
      Script_fixoAttachEx(arg0, arg2, 18, &local_F4, &local_E8);
      break;
    case 1:
      local_F4.x = -0.067f;
      (&local_F4)->y = -0.77f;
      (&local_F4)->z = 0.01f;
      local_E8.x = -1820;
      (&local_E8)->y = 0;
      (&local_E8)->z = -64262;
      Script_fixoAttachEx(arg0, arg2, 12, &local_F4, &local_E8);
      break;
  }
  for (;;) {
    if (task_8 == 1) {
      Script_srchControl(0);
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_5B57(int32_t arg0, int32_t arg1) {
  Script_MoveObjectToObject(arg0, 0x304D5544);
  Script_SetShadowFlags(arg0, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_scenToggleActiveFor(arg0, 0);
  Script_SetCharaFlags(arg0, 0);
  script_2A1F();
  for (;;) {
    if (task_8 == 1) {
      script_2A66();
      Script_srchControl(0);
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}
