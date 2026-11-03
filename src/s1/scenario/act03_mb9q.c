#include "act03_mb9q.h"

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

static int32_t script_0125(void) {
  Script_filmCheckOrSetTransitionUtil(80, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  end:;
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

static int32_t script_039D(int32_t arg0, int32_t arg1) {
  if (arg1 > 0) {
    SCNF_WaitFrames_0000(arg1);
  }
  Script_executeAicaCommand(arg0, 0, 0);
  return Script_srchControl(0);
}

static int32_t script_03C1(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  task_18[arg1] = 0;
  task_38[arg1] = 0;
  task_58[arg1] = 0;
  task_78[arg1] = arg2;
  task_98[arg1] = arg3;
  task_B8 = 0;
  Script_CharHandlers(arg0, 0, task_78[arg1], 16);
  Script_CharHandlers(arg0, 1, task_98[arg1], 16);
  for (;;) {
    task_18[arg1] = 0;
    while (task_18[arg1] == 0) {
      SCNF_WaitFrames_0000(1);
    }
    task_B8 = task_58[arg1] | 0x40000000;
    Script_scenCreateTask(arg4, 3, arg0, arg1, task_18[arg1] + task_38[arg1]);
  }
  end:;
}

static int32_t script_0498(int32_t arg0, int32_t arg1, int32_t arg2, float arg3, int32_t arg4, int32_t arg5) {
  int32_t local_FC;
  local_FC = task_B8;
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

static int32_t script_052F(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  if (arg3 > 0) {
    SCNF_WaitFrames_0000(arg3);
  }
  Script_CharHandlers(arg0, 2, arg2, 16);
  if (arg4 > 0) {
    SCNF_WaitFrames_0000(arg4);
  }
  Script_CharHandlers(arg0, 0, task_78[arg1], 16);
  Script_CharHandlers(arg0, 1, task_98[arg1], 16);
  return Script_srchControl(0);
}

static int32_t script_0591(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  local_FC = task_B8;
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

static int32_t script_05ED(int32_t arg0, float arg1, int32_t arg2) {
  int32_t local_FC;
  Vector3i local_F0;
  local_FC = task_B8;
  if (arg2 > 0) {
    SCNF_WaitFrames_0000(arg2);
  }
  if (local_FC & -0x80000000) {
    arg1 = (float)0 - arg1;
  }
  (&local_F0)->y = (int32_t)(arg1 / 360.0f * (float)0x10000);
  Script_EVT_SetRotation(arg0, -0x70000000, &local_F0, 0);
  return Script_srchControl(0);
}

static void script_064A(int32_t arg0) {
  while (Script_IsCharaMotionFinished(arg0) == 0) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static int32_t script_0666(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, void *arg4, int32_t arg5) {
  int32_t local_FC;
  task_11C[arg1] = Script_scenCreateTask(script_03C1, 5, arg0, arg1, arg2, arg3, arg5);
  task_BC[arg1] = arg0;
  task_DC[arg1] = 0;
  task_FC[arg1] = 0;
  task_13C = 4;
  for (;;) {
    SCNF_WaitFrames_0000(1);
    if (task_DC[arg1]) {
      local_FC = task_FC[arg1] * 12;
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
          task_18[arg1] = local_F8;
        }
      }
      {
        int32_t local_F8;
        local_F8 = Script_ScenDataUtil(arg4, 4, local_FC + 6);
        if (local_F8 > 0) {
          SCNF_WaitFrames_0000(local_F8);
        }
      }
      if (task_38[arg1] == 0) {
        Script_scenCreateTask(script_09D6, 4, Script_ScenDataUtil(arg4, 4, local_FC), Script_ScenDataUtil(arg4, 4, local_FC + 1), Script_ScenDataUtil(arg4, 4, local_FC + 2), Script_ScenDataUtil(arg4, 4, local_FC + 3));
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
          task_18[arg1] = local_F8;
        }
      }
      script_08D4();
      {
        int32_t local_F8;
        if (task_148 == Script_ScenDataUtil(arg4, 4, local_FC)) {
          task_144 = 1;
          local_F8 = Script_ScenDataUtil(arg4, 4, local_FC + 10);
        } else {
          if (task_38[arg1] == 0) {
            task_150 = task_150 + Script_ScenDataUtil(arg4, 4, local_FC + 4);
          }
          task_144 = 0;
          local_F8 = Script_ScenDataUtil(arg4, 4, local_FC + 11);
        }
        if (local_F8 > 0) {
          task_18[arg1] = local_F8;
        }
      }
      script_08C4(arg1);
    }
  }
  end:;
}

static void script_088D(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  task_DC[arg0] = 1;
  task_FC[arg0] = arg1;
  task_38[arg0] = arg2;
  task_58[arg0] = arg3;
  return;
}

static void script_08C4(int32_t arg0) {
  task_DC[arg0] = 0;
  return;
}

static void script_08D4(void) {
  SCNF_WaitFrames_0000(2);
  while (task_148 == 0) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_08F4(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  script_088D(arg0, arg1, 0, arg3);
  script_088D(arg2, arg1, 1, arg3);
  while (task_DC[arg0]) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_092E(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  script_088D(arg0, arg1, 0, arg3);
  script_088D(arg2, arg1, 1, arg3);
  script_088D(arg2 + 1, arg1, 2, arg3);
  while (task_DC[arg0]) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_097A(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  script_088D(arg0, arg1, 0, arg4);
  script_088D(arg2, arg1, 1, arg4);
  script_088D(arg3, arg1, 2, arg4);
  while (task_DC[arg0]) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_09C3(int32_t arg0) {
  task_13C = arg0;
  Script_SetSfxOverlayTimeoutTuning(arg0, SfxMode_SetBoostStepFrames);
  return;
}

static int32_t script_09D6(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  int32_t local_F8;
  local_FC = arg1 + arg3;
  local_F8 = arg2 + arg3;
  Script_scenCreateTask(script_0A39, 2, arg1, arg3);
  Script_SetSfxOverlayTimeoutTuning(local_FC, SfxMode_SetMaxFrames);
  if (arg0 == 68) {
    SCNF_UIUtils(0x44, local_F8, 5, 0, 1, 0);
  } else {
    SCNF_UIUtils(arg0, local_F8, 2, 0, 1, 0);
  }
  return Script_srchControl(0);
}

static int32_t script_0A39(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  int32_t local_F8;
  task_14C = arg0;
  task_148 = 0;
  task_140 = 0;
  local_F8 = 0xFFFF;
  for (;;) {
    local_FC = SCNF_UIUtils(0, 0, 0, 0, 0, -1);
    task_140 = task_140 + 1;
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
      if (task_140 >= arg0 + arg1 + 4) {
        break;
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  if ((local_F8 & 0xFFF) == 0) {
    local_F8 = 0xFFFF;
  }
  task_148 = local_F8 & 0xFFF;
  return Script_srchControl(0);
}

static int32_t script_0AFA(int32_t arg0, int32_t arg1, int32_t arg2) {
  {
    Vector3 local_F4;
    Script_EVT_GetPosition(arg0, -1, &local_F4, 0);
    task_154[arg1] = arg0;
    task_174[arg1][0] = (&local_F4)->x;
    task_174[arg1][1] = (&local_F4)->y;
    task_174[arg1][2] = (&local_F4)->z;
  }
  task_1D4[arg1] = 0.0f;
  for (;;) {
    Vector3 local_F4;
    SCNF_WaitFrames_0000(1);
    Script_EVT_GetPosition(arg0, -1, &local_F4, 0);
    task_174[arg1][0] = (&local_F4)->x;
    task_174[arg1][1] = (&local_F4)->y;
    task_174[arg1][2] = (&local_F4)->z;
    task_1D4[arg1] = Script_MathUtils(SCRIPT_MATH_VEC2_LENGTH, task_174[arg1][0] - task_174[arg2][0], task_174[arg1][2] - task_174[arg2][2]);
  }
  end:;
}

static void script_0BE8(void) {
  task_1F4 = 0.0f;
  task_1F8 = 0.0f;
  task_1FC = 0.0f;
  for (;;) {
    SCNF_WaitFrames_0000(1);
    if (Script_GetInputData(2, 0) & 4) {
      if (Script_GetInputData(0, 2) & 0x4000) {
        task_1F4 = task_1F4 - 0.01f;
      }
      if (Script_GetInputData(0, 2) & 0x8000) {
        task_1F4 = task_1F4 + 0.01f;
      }
      if (Script_GetInputData(0, 2) & 0x1000) {
        task_1F8 = task_1F8 - 0.01f;
      }
      if (Script_GetInputData(0, 2) & 0x2000) {
        task_1F8 = task_1F8 + 0.01f;
      }
      if (Script_GetInputData(0, 2) & 0x40) {
        task_1FC = task_1FC - 1.0f;
      }
      if (Script_GetInputData(0, 2) & 0x80) {
        task_1FC = task_1FC + 1.0f;
      }
    }
  }
  return;
}

static void script_0CD6(int32_t arg0, int32_t arg1) {
  Vector3 local_F4;
  Vector3i local_E8;
  Script_MoveObjectToObject(arg0, arg1);
  Script_EVT_GetPosition(arg0, -1, &local_F4, 0);
  Script_mtwkGetSetRotation(arg0, &local_E8, 0);
  Script_MoveObjectToObject(arg0, -1);
  return;
}

static void script_0CFF(int32_t arg0, int32_t arg1, float arg2, float arg3, float arg4) {
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

static void script_0D8E(int32_t arg0, int32_t arg1, const Vector3 *arg2) {
  int32_t local_FC;
  local_FC = Script_CEWPRelated(arg0);
  Script_MoveObjectToObject(arg0, arg1);
  Script_EVT_SetPosition(arg0, POS_XYZ, arg2);
  Script_MoveObjectToObject(arg0, local_FC);
  Script_InitializeOsag(arg0);
  return;
}

static void script_0DC1(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
  Script_scenCreateTask(script_0DDB, 6, arg0, arg1, arg2, arg3, arg4, arg5);
  return;
}

static int32_t script_0DDB(int32_t arg0, int32_t arg1, float arg2, float arg3, float arg4, int32_t arg5) {
  if (arg5 >= 0) {
    SCNF_WaitFrames_0000(arg5);
  }
  script_0CFF(arg0, arg1, arg2, arg3, arg4);
  return Script_srchControl(0);
}

static void script_0E06(int32_t arg0, int32_t arg1, float arg2, float arg3, float arg4, float arg5) {
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

static void script_0E86(int32_t arg0, int32_t arg1, float arg2) {
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

static void script_0EDA(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Script_scenCreateTask(script_0EF0, 4, arg0, arg1, arg2, arg3);
  return;
}

static int32_t script_0EF0(int32_t arg0, int32_t arg1, float arg2, int32_t arg3) {
  if (arg3 >= 0) {
    SCNF_WaitFrames_0000(arg3);
  }
  script_0E86(arg0, arg1, arg2);
  return Script_srchControl(0);
}

static void script_0F17(int32_t arg0, float arg1) {
  Vector3i local_F4;
  (&local_F4)->y = (int32_t)(arg1 / 360.0f * (float)0x10000);
  Script_EVT_SetRotation(arg0, -0x70000000, &local_F4, 0);
  return;
}

static void script_0F40(int32_t arg0, float arg1, float arg2, float arg3) {
  Vector3i local_F4;
  local_F4.x = (int32_t)(arg1 / 360.0f * (float)0x10000);
  (&local_F4)->y = (int32_t)(arg2 / 360.0f * (float)0x10000);
  (&local_F4)->z = (int32_t)(arg3 / 360.0f * (float)0x10000);
  Script_EVT_SetRotation(arg0, -0x48000000, &local_F4, 0);
  return;
}

static void script_0F94(int32_t arg0, int32_t arg1, int32_t arg2) {
  Script_scenCreateTask(script_0FA8, 3, arg0, arg1, arg2);
  return;
}

static int32_t script_0FA8(int32_t arg0, float arg1, int32_t arg2) {
  if (arg2 >= 0) {
    SCNF_WaitFrames_0000(arg2);
  }
  script_0F17(arg0, arg1);
  return Script_srchControl(0);
}

static void script_0FCD(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  Script_scenCreateTask(script_0FE5, 5, arg0, arg1, arg2, arg3, arg4);
  return;
}

static int32_t script_0FE5(int32_t arg0, float arg1, float arg2, float arg3, int32_t arg4) {
  if (arg4 >= 0) {
    SCNF_WaitFrames_0000(arg4);
  }
  script_0F40(arg0, arg1, arg2, arg3);
  return Script_srchControl(0);
}

static void script_100E(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  Script_scenCreateTask(script_1026, 5, arg0, arg1, arg2, arg3, arg4);
  return;
}

static int32_t script_1026(int32_t arg0, float arg1, float arg2, float arg3, int32_t arg4) {
  float local_FC;
  if (arg4 >= 0) {
    SCNF_WaitFrames_0000(arg4);
  }
  local_FC = (arg2 - arg1) / arg3;
  for (;;) {
    if (arg3 <= 0.0f) {
      break;
    }
    script_0F40(arg0, 0.0f, 0.0f, local_FC);
    arg3 = arg3 - 1.0f;
    SCNF_WaitFrames_0000(1);
  }
  return Script_srchControl(0);
}

static void script_107B(int32_t arg0, int32_t arg1) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t local_E4;
  local_E4 = arg0;
  while (local_E4 <= arg1) {
    Script_EVT_GetPosition(task_BC[local_E4], -1, &local_F4, 0);
    Script_mtwkGetSetRotation(task_BC[local_E4], &local_E8, 0);
    task_200[local_E4][0] = (&local_F4)->x;
    task_200[local_E4][1] = (&local_F4)->y;
    task_200[local_E4][2] = (&local_F4)->z;
    task_260[local_E4][0] = (&local_E8)->x;
    task_260[local_E4][1] = (&local_E8)->y;
    task_260[local_E4][2] = (&local_E8)->z;
    task_2C0[local_E4] = Script_GetCharaMotionRemainingFramesScaled(task_BC[local_E4]);
    task_2E0[local_E4] = Script_scenCharaGetMotAnimId(task_BC[local_E4]);
    local_E4 = local_E4 + 1;
  }
  return;
}

static void script_1160(int32_t arg0, int32_t arg1) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t local_E4;
  local_E4 = arg0;
  while (local_E4 <= arg1) {
    local_F4.x = task_200[local_E4][0];
    (&local_F4)->y = task_200[local_E4][1];
    (&local_F4)->z = task_200[local_E4][2];
    local_E8.x = task_260[local_E4][0];
    (&local_E8)->y = task_260[local_E4][1];
    (&local_E8)->z = task_260[local_E4][2];
    Script_EVT_SetPosition(task_BC[local_E4], POS_XYZ, &local_F4);
    Script_EVT_SetRotation(task_BC[local_E4], 0x38000000, &local_E8, 0);
    Script_PlayCharaMotion(task_BC[local_E4], task_2E0[local_E4], task_2C0[local_E4], -1.0f, -1.0f, 1.0f, 0);
    local_E4 = local_E4 + 1;
  }
  return;
}

static void script_1249(int32_t arg0) {
  task_304 = arg0;
  task_300 = 1;
  Script_PlayerStatsUtils(PS_Set70MBTimeCapSlot, task_304, task_300);
  return;
}

static void script_1267(void) {
  int32_t local_FC;
  if (task_300 == 1) {
    local_FC = Script_PlayerStatsUtils(PS_Get70MBCounter1);
    Script_PlayerStatsUtils(PS_Set70MBCounter1, local_FC + 1);
  }
  if (task_300 < 255) {
    task_300 = task_300 + 1;
  }
  Script_PlayerStatsUtils(PS_Set70MBTimeCapSlot, task_304, task_300);
  return;
}

static void script_12AF(void) {
  int32_t local_FC;
  local_FC = Script_PlayerStatsUtils(PS_Get70MBCounter0);
  Script_PlayerStatsUtils(PS_Set70MBCounter0, local_FC + 1);
  return;
}

static void script_12CA(void) {
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_12E4(const char *arg0, const char *arg1) {
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 5, arg1) == 0) {
    while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, arg0, arg1) == 0) {
      SCNF_WaitFrames_0000(1);
    }
    script_12CA();
  }
  return;
}

static void script_1322(const char *arg0) {
  if (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 5, arg0) != 0) {
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, arg0);
  }
  return;
}

static void script_1343(int32_t arg0) {
  L001345:;
  SCNF_WaitFrames_0000(1);
  if (!Script_CycleManagerUtils(CYCLE_CMD_GET_NEXT_TASK_POINTER, arg0)) goto L001345;
  return;
}

static void script_135A(int32_t arg0) {
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
      script_1343(local_FC);
    }
  }
  (void)local_FC;
  return;
}

static void script_13E0(int32_t arg0, int32_t arg1) {
  if (arg1) {
    Script_CycleManagerUtils(CYCLE_CMD_LOAD_OR_RESET_RUNNING_PROGRAM, arg1);
  }
  Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT, arg0, 0);
  arg1 = 0;
  return;
}

static void script_1403(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t unused_E8[4];
  Vector3 local_DC;
  int32_t unused_D0[3];
  int32_t local_CC;
  local_F8 = Script_shenSeqUtils(-8, 0);
  Script_shenSeqUtils(-9, 0, 0);
  if (arg0) {
    Script_scenCreateTask(script_0125, 0);
  }
  Script_shenSeqUtils(0);
  local_FC = 0;
  while (Script_shenSeqUtils(-1)) {
    local_CC = local_FC;
    switch (local_CC) {
      case 1:
        Script_SetMaxInterpSpeed(0.0f);
        Script_SetMotnFlag(CHARA_AKIR, -1);
        Script_handSetFloatsFor(CHARA_AKIR, 0, data_8D48, 1);
        Script_handSetFloatsFor(CHARA_AKIR, 1, data_8D48, 1);
        Script_SetHandRootRotation(CHARA_AKIR, 1, 0x15, &data_8F10);
        break;
      case 380:
        Script_handSetFloatsFor(CHARA_AKIR, 1, data_87F0, 20);
        break;
      case 400:
        Script_handSetFloatsFor(CHARA_AKIR, 1, data_8E2C, 10);
        break;
      case 790:
        Script_handSetFloatsFor(CHARA_AKIR, 1, data_8D48, 10);
        break;
      case 740:
        local_DC.x = 40.138f;
        (&local_DC)->y = 0.7061f;
        (&local_DC)->z = 76.8479f;
        Script_FaceLookAtTargetUtils(CHARA_AKIR, 2, &local_DC, 40);
        Script_handSetFloatsFor(CHARA_AKIR, 1, data_87F0, 20);
        Script_SetHandRootRotation(CHARA_AKIR, 1, 0x15, &data_8F1C);
        break;
      case 780:
        local_DC.x = 38.190701f;
        (&local_DC)->y = -1.449f;
        (&local_DC)->z = 81.391602f;
        Script_FaceLookAtTargetUtils(CHARA_AKIR, 2, &local_DC, 10);
        break;
      case 800:
        local_DC.x = 40.411598f;
        (&local_DC)->y = 0.6376f;
        (&local_DC)->z = 78.208397f;
        Script_FaceLookAtTargetUtils(CHARA_AKIR, 0);
        break;
      case 824:
        Script_filmCheckOrSetTransitionUtil(56, 0, 0, 0, 0, 255, 0, 0, 0);
        while (Script_filmCheckOrSetTransitionUtil(-1)) {
          SCNF_WaitFrames_0000(1);
        }
        break;
    }
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  Script_SetMotnFlag(CHARA_AKIR, 0);
  Script_SetMaxInterpSpeed(0.5f);
  Script_shenSeqUtils(-9, 0, local_F8);
  return;
}

static int32_t script_1643(int32_t arg0, int32_t arg1, int32_t arg2) {
  int32_t local_FC;
  local_FC = arg2;
  switch (local_FC) {
    case 1:
      break;
    case 2:
      Script_scenCreateTask(script_0498, 6, 64017, -1, 0, 0x3F800000, arg0, arg1);
      break;
    case 3:
      Script_scenCreateTask(script_0498, 6, 64127, -1, 0x41000000, 0x3F800000, arg0, arg1);
      break;
    case 4:
      Script_scenCreateTask(script_039D, 2, 161451, 17);
      Script_scenCreateTask(script_039D, 2, 0x1976AB, 13);
      Script_scenCreateTask(script_039D, 2, 0x2876AB, 58);
      Script_scenCreateTask(script_0498, 6, 64015, -1, 0, 0x3F800000, arg0, arg1);
      break;
    case 5:
      Script_scenCreateTask(script_039D, 2, 30379, 8);
      Script_scenCreateTask(script_0498, 6, 64005, -1, 0, 0x3F800000, arg0, arg1);
      break;
    case 6:
      Script_scenCreateTask(script_052F, 5, arg0, arg1, 0, -1, 20);
      Script_scenCreateTask(script_0498, 6, 64016, -1, 0, 0x3F800000, arg0, arg1);
      break;
    case 7:
      Script_scenCreateTask(script_0591, 4, arg0, 4, 10, 10);
      Script_scenCreateTask(script_052F, 5, arg0, arg1, 0, -1, 13);
      Script_scenCreateTask(script_0498, 6, 64002, -1, 0, 0x3F800000, arg0, arg1);
      break;
    case 8:
      Script_scenCreateTask(script_039D, 2, 30379, 13);
      Script_scenCreateTask(script_039D, 2, 0x2876AB, 35);
      Script_scenCreateTask(script_0498, 6, 64013, -1, 0x41F00000, 0x3F800000, arg0, arg1);
      break;
    case 9:
      Script_scenCreateTask(script_039D, 2, 30379, 5);
      Script_scenCreateTask(script_0498, 6, 64005, -1, 0, 0x3F800000, arg0, arg1);
      task_B8 = task_B8 | 0x20000000;
      break;
    case 10:
      Script_scenCreateTask(script_052F, 5, arg0, arg1, 0, -1, 13);
      Script_scenCreateTask(script_0498, 6, 64016, -1, 0, 0x3F800000, arg0, arg1);
      task_B8 = task_B8 | 0x20000000;
      break;
    case 11:
      Script_scenCreateTask(script_0498, 6, 64001, -1, 0x40000000, 0x3F800000, arg0, arg1);
      break;
    case 12:
      Script_scenCreateTask(script_0498, 6, 64012, -1, 0, 0x3F800000, arg0, arg1);
      break;
    case 13:
      Script_scenCreateTask(script_0498, 6, 64023, -1, 0, 0x3F800000, arg0, arg1);
      break;
    case 14:
      Script_scenCreateTask(script_0498, 6, 64006, -1, 0, 0x3F800000, arg0, arg1);
      break;
    case 15:
      Script_scenCreateTask(script_039D, 2, 0x2876AB, 48);
      Script_scenCreateTask(script_0498, 6, 64018, -1, 0x40000000, 0x3F800000, arg0, arg1);
      break;
    case 16:
      Script_scenCreateTask(script_039D, 2, 161451, 40);
      Script_scenCreateTask(script_039D, 2, 0x2876AB, 52);
      Script_scenCreateTask(script_0498, 6, 64024, 20, 0x41C80000, 0x3F800000, arg0, arg1);
      break;
    case 17:
      Script_scenCreateTask(script_0498, 6, 64007, -1, 0x40000000, 0x3F800000, arg0, arg1);
      break;
    case 18:
      Script_scenCreateTask(script_0498, 6, 64019, -1, 0, 0x3F800000, arg0, arg1);
      break;
    case 19:
      Script_scenCreateTask(script_039D, 2, 30379, 28);
      Script_scenCreateTask(script_052F, 5, arg0, arg1, 0, -1, 30);
      Script_scenCreateTask(script_0498, 6, 64025, -1, 0, 0x3F800000, arg0, arg1);
      break;
    case 20:
      Script_scenCreateTask(script_0498, 6, 64008, -1, 0x40000000, 0x3F800000, arg0, arg1);
      task_B8 = task_B8 | 0x20000000;
      break;
    case 21:
      Script_scenCreateTask(script_0498, 6, 64020, -1, 0, 0x3F800000, arg0, arg1);
      task_B8 = task_B8 | 0x20000000;
      break;
    case 22:
      Script_scenCreateTask(script_039D, 2, 161451, 21);
      Script_scenCreateTask(script_0498, 6, 64026, -1, 0, 0x3F800000, arg0, arg1);
      task_B8 = task_B8 | 0x20000000;
      break;
    case 23:
      break;
    case 24:
      break;
    case 25:
      Script_scenCreateTask(script_0591, 4, arg0, 5, 20, 12);
      Script_scenCreateTask(script_039D, 2, 30379, 28);
      Script_scenCreateTask(script_039D, 2, 0x1176AB, 16);
      Script_scenCreateTask(script_052F, 5, arg0, arg1, 0, -1, 60);
      Script_scenCreateTask(script_0498, 6, 64129, -1, 0x41100000, 0x3F800000, arg0, arg1);
      break;
    case 26:
      Script_scenCreateTask(script_039D, 2, 0x2876AB, 49);
      Script_scenCreateTask(script_0498, 6, 64043, -1, 0x42240000, 0x3F800000, arg0, arg1);
      break;
    case 27:
      Script_scenCreateTask(script_0498, 6, 64033, -1, 0, 0x3F800000, arg0, arg1);
      break;
    case 28:
      Script_scenCreateTask(script_039D, 2, 30379, 19);
      Script_scenCreateTask(script_052F, 5, arg0, arg1, 0, -1, 30);
      Script_scenCreateTask(script_0498, 6, 64044, -1, 0x42180000, 0x3F800000, arg0, arg1);
      break;
    case 29:
      Script_scenCreateTask(script_039D, 2, 161451, 30);
      Script_scenCreateTask(script_0498, 6, 64034, -1, 0, 0x3F800000, arg0, arg1);
      break;
    case 30:
      Script_scenCreateTask(script_039D, 2, 0x2876AB, 70);
      Script_scenCreateTask(script_0498, 6, 64045, -1, 0, 0x3F800000, arg0, arg1);
      break;
    case 31:
      Script_scenCreateTask(script_0498, 6, 64033, -1, 0x40E00000, 0x3F800000, arg0, arg1);
      task_B8 = task_B8 | 0x20000000;
      break;
    case 32:
      Script_scenCreateTask(script_039D, 2, 30379, 14);
      Script_scenCreateTask(script_052F, 5, arg0, arg1, 0, -1, 30);
      Script_scenCreateTask(script_0498, 6, 64044, -1, 0x422C0000, 0x3F800000, arg0, arg1);
      task_B8 = task_B8 | 0x20000000;
      break;
    case 33:
      Script_scenCreateTask(script_0498, 6, 64031, -1, 0, 0x3F800000, arg0, arg1);
      break;
    case 34:
      Script_scenCreateTask(script_0498, 6, 64042, 24, 0, 0x3F800000, arg0, arg1);
      break;
    case 35:
      break;
    case 36:
      Script_scenCreateTask(script_0498, 6, 64029, -1, 0x41200000, 0x3F800000, arg0, arg1);
      break;
    case 37:
      Script_scenCreateTask(script_039D, 2, 0x1976AB, 5);
      Script_scenCreateTask(script_0498, 6, 64040, -1, 0x41C80000, 0x3F800000, arg0, arg1);
      break;
    case 38:
      Script_scenCreateTask(script_0498, 6, 64047, 7, 0, 0x3F800000, arg0, arg1);
      break;
    case 39:
      Script_scenCreateTask(script_0498, 6, 64031, -1, 0x420C0000, 0x3F800000, arg0, arg1);
      break;
    case 40:
      Script_scenCreateTask(script_039D, 2, 30379, 20);
      Script_scenCreateTask(script_052F, 5, arg0, arg1, 0, -1, 30);
      Script_scenCreateTask(script_0498, 6, 64042, -1, 0x40C00000, 0x3F800000, arg0, arg1);
      break;
    case 41:
      break;
    case 42:
      Script_scenCreateTask(script_039D, 2, 161451, 14);
      Script_scenCreateTask(script_039D, 2, 30379, 36);
      Script_scenCreateTask(script_0498, 6, 64128, -1, 0, 0x3F800000, arg0, arg1);
      break;
    case 43:
      Script_scenCreateTask(script_039D, 2, 0x2876AB, 76);
      Script_scenCreateTask(script_0498, 6, 64041, -1, 0, 0x3F800000, arg0, arg1);
      break;
    case 44:
      Script_scenCreateTask(script_039D, 2, 0x2876AB, 33);
      Script_scenCreateTask(script_0498, 6, 64047, -1, 0x41C80000, 0x3F800000, arg0, arg1);
      break;
    case 45:
      Script_scenCreateTask(script_0498, 6, 64048, -1, 0, 0x3F800000, arg0, arg1);
      break;
    case 46:
      Script_scenCreateTask(script_0498, 6, 64049, -1, 0, 0x3F800000, arg0, arg1);
      break;
    case 47:
      Script_scenCreateTask(script_039D, 2, 161451, 21);
      Script_scenCreateTask(script_0498, 6, 64050, -1, 0x41B00000, 0x3F800000, arg0, arg1);
      Script_scenCreateTask(script_05ED, 3, arg0, -0x3D380000, -1);
      break;
    case 48:
      Script_scenCreateTask(script_0498, 6, 64060, -1, 0, 0x3F800000, arg0, arg1);
      task_B8 = task_B8 | 0x20000000;
      break;
    case 49:
      Script_scenCreateTask(script_039D, 2, 161451, 12);
      Script_scenCreateTask(script_0498, 6, 64072, -1, 0, 0x3F800000, arg0, arg1);
      task_B8 = task_B8 | 0x20000000;
      break;
    case 50:
      Script_scenCreateTask(script_039D, 2, 0x1376AB, 13);
      Script_scenCreateTask(script_0498, 6, 64059, -1, 0x41200000, 0x3F8CCCCD, arg0, arg1);
      break;
    case 51:
      Script_scenCreateTask(script_039D, 2, 0x2876AB, 55);
      Script_scenCreateTask(script_0498, 6, 64071, -1, 0x41200000, 0x3F8CCCCD, arg0, arg1);
      Script_scenCreateTask(script_05ED, 3, arg0, 0x41A00000, -1);
      break;
    case 52:
      break;
    case 53:
      Script_scenCreateTask(script_0498, 6, 64064, -1, 0, 0x3F800000, arg0, arg1);
      break;
    case 54:
      Script_scenCreateTask(script_0591, 4, arg0, 4, 7, 10);
      Script_scenCreateTask(script_0591, 4, arg0, 5, 12, 10);
      Script_scenCreateTask(script_039D, 2, 30379, 22);
      Script_scenCreateTask(script_052F, 5, arg0, arg1, 0, -1, 50);
      Script_scenCreateTask(script_0498, 6, 64053, -1, 0x41200000, 0x3F800000, arg0, arg1);
      break;
    case 55:
      Script_scenCreateTask(script_039D, 2, 0x2876AB, 38);
      Script_scenCreateTask(script_0498, 6, 64064, -1, 0x41200000, 0x3F800000, arg0, arg1);
      break;
    case 56:
      Script_scenCreateTask(script_0498, 6, 64055, -1, 0x41600000, 0x3F800000, arg0, arg1);
      Script_scenCreateTask(script_0498, 6, 64052, 2, 0, 0x3F800000, arg0, arg1);
      break;
    case 57:
      Script_scenCreateTask(script_039D, 2, 161451, 2);
      Script_scenCreateTask(script_0498, 6, 64066, -1, 0x41600000, 0x3F800000, arg0, arg1);
      Script_scenCreateTask(script_0498, 6, 64063, 2, 0, 0x3F800000, arg0, arg1);
      break;
    case 58:
      Script_scenCreateTask(script_039D, 2, 161451, 18);
      Script_scenCreateTask(script_0498, 6, 64051, -1, 0x41F00000, 0x3F800000, arg0, arg1);
      break;
    case 59:
      Script_scenCreateTask(script_039D, 2, 0x2876AB, 36);
      Script_scenCreateTask(script_0498, 6, 64062, -1, 0x41F00000, 0x3F800000, arg0, arg1);
      break;
    case 60:
      Script_scenCreateTask(script_0498, 6, 64052, -1, 0x422C0000, 0x3F800000, arg0, arg1);
      break;
    case 61:
      Script_scenCreateTask(script_039D, 2, 30379, 18);
      Script_scenCreateTask(script_0498, 6, 64063, -1, 0x42200000, 0x3F800000, arg0, arg1);
      break;
    case 62:
      Script_scenCreateTask(script_0498, 6, 20745, -1, 0, 0x3F800000, arg0, arg1);
      Script_scenCreateTask(script_05ED, 3, arg0, 0x43340000, -1);
      break;
    case 63:
      Script_scenCreateTask(script_0498, 6, 20802, -1, 0, 0x3F800000, arg0, arg1);
      break;
    case 64:
      Script_scenCreateTask(script_0498, 6, 20807, -1, 0, 0x3F800000, arg0, arg1);
      break;
    case 65:
      Script_scenCreateTask(script_0498, 6, 20745, -1, 0x41900000, 0x3F800000, arg0, arg1);
      Script_scenCreateTask(script_039D, 2, 0x1176AB, 4);
      Script_scenCreateTask(script_039D, 2, 0x1176AB, 17);
      break;
    case 66:
      Script_scenCreateTask(script_0498, 6, 20802, -1, 0x41900000, 0x3F800000, arg0, arg1);
      break;
    case 67:
      Script_scenCreateTask(script_0498, 6, 20807, -1, 0x41900000, 0x3F800000, arg0, arg1);
      break;
    case 68:
      Script_scenCreateTask(script_0498, 6, 20767, -1, 0x41900000, 0x3F800000, arg0, arg1);
      Script_scenCreateTask(script_0498, 6, 787, 23, 0, 0x3F800000, arg0, arg1);
      Script_scenCreateTask(script_0498, 6, 64043, 36, 0x42700000, 0x3F800000, arg0, arg1);
      Script_scenCreateTask(script_039D, 2, 161451, 4);
      Script_scenCreateTask(script_039D, 2, 161451, 17);
      Script_scenCreateTask(script_039D, 2, 0x2876AB, 66);
      break;
    case 69:
      Script_scenCreateTask(script_0498, 6, 20802, -1, 0x41900000, 0x3F800000, arg0, arg1);
      SCNF_WaitFrames_0000(23);
      Script_scenCreateTask(script_0498, 6, 64075, -1, 0, 0x3F666666, arg0, arg1);
      Script_scenCreateTask(script_039D, 2, 161451, 26);
      task_B8 = task_B8 ^ -0x80000000;
      break;
    case 70:
      Script_scenCreateTask(script_0498, 6, 20807, -1, 0x41900000, 0x3F800000, arg0, arg1);
      Script_scenCreateTask(script_0498, 6, 787, 23, 0, 0x3F800000, arg0, arg1);
      break;
    case 71:
      Script_scenCreateTask(script_0591, 4, arg0, 6, -1, 20);
      Script_scenCreateTask(script_0498, 6, 20744, -1, 0x40A00000, 0x3F800000, arg0, arg1);
      break;
    case 72:
      Script_scenCreateTask(script_0498, 6, 20801, -1, 0x40A00000, 0x3F800000, arg0, arg1);
      Script_scenCreateTask(script_039D, 2, 161451, 10);
      break;
    case 73:
      Script_scenCreateTask(script_0498, 6, 20806, -1, 0, 0x3F800000, arg0, arg1);
      Script_scenCreateTask(script_039D, 2, 161451, 17);
      break;
    case 74:
      Script_scenCreateTask(script_0498, 6, 787, -1, 0, 0x3F800000, arg0, arg1);
      Script_scenCreateTask(script_0498, 6, 64043, 13, 0x42700000, 0x3F800000, arg0, arg1);
      Script_scenCreateTask(script_039D, 2, 0x2876AB, 43);
      break;
    case 75:
      Script_scenCreateTask(script_0498, 6, 64075, -1, 0, 0x3F666666, arg0, arg1);
      Script_scenCreateTask(script_039D, 2, 161451, 26);
      task_B8 = task_B8 ^ -0x80000000;
      break;
    case 76:
      Script_scenCreateTask(script_0498, 6, 787, -1, 0, 0x3F800000, arg0, arg1);
      break;
    case 83:
      Script_scenCreateTask(script_0498, 6, 64114, -1, 0, 0x3F800000, arg0, arg1);
      break;
    case 84:
      Script_scenCreateTask(script_0498, 6, 64118, -1, 0, 0x3F800000, arg0, arg1);
      break;
    case 85:
      Script_scenCreateTask(script_0498, 6, 64114, -1, 0x41800000, 0x3F800000, arg0, arg1);
      break;
    case 86:
      Script_scenCreateTask(script_039D, 2, 0x1976AB, 13);
      break;
    case 87:
      Script_scenCreateTask(script_0498, 6, 64116, -1, 0x41800000, 0x3F800000, arg0, arg1);
      break;
    case 88:
      Script_scenCreateTask(script_039D, 2, 161451, 13);
      break;
    case 89:
      Script_scenCreateTask(script_0498, 6, 64114, -1, 0x42080000, 0x3F800000, arg0, arg1);
      break;
    case 90:
      Script_scenCreateTask(script_039D, 2, 0x1976AB, 10);
      break;
    case 91:
      Script_scenCreateTask(script_0498, 6, 64115, -1, 0x42080000, 0x3F800000, arg0, arg1);
      break;
    case 92:
      Script_scenCreateTask(script_039D, 2, 161451, 10);
      break;
    case 93:
      Script_scenCreateTask(script_0591, 4, arg0, 4, 20, 10);
      Script_scenCreateTask(script_0498, 6, 64119, -1, 0, 0x3F800000, arg0, arg1);
      break;
    case 94:
      Script_scenCreateTask(script_0498, 6, 64122, -1, 0, 0x3F800000, arg0, arg1);
      Script_scenCreateTask(script_05ED, 3, arg0, 0x43340000, -1);
      break;
    case 95:
      Script_scenCreateTask(script_0498, 6, 64121, -1, 0, 0x3F800000, arg0, arg1);
      break;
    case 96:
      Script_scenCreateTask(script_0498, 6, 64123, -1, 0, 0x3F800000, arg0, arg1);
      Script_scenCreateTask(script_039D, 2, 161451, 16);
      Script_scenCreateTask(script_05ED, 3, arg0, 0x43340000, -1);
      break;
    case 97:
      Script_scenCreateTask(script_0498, 6, 64120, -1, 0x41A00000, 0x3F99999A, arg0, arg1);
      Script_scenCreateTask(script_052F, 5, arg0, arg1, 0, -1, 90);
      Script_scenCreateTask(script_039D, 2, 161451, 18);
      break;
    case 98:
      Script_scenCreateTask(script_0498, 6, 64123, -1, 0x41A00000, 0x3F99999A, arg0, arg1);
      Script_scenCreateTask(script_039D, 2, 0x2876AB, 32);
      break;
    case 99:
      Script_scenCreateTask(script_0498, 6, 64121, -1, 0x41A00000, 0x3F800000, arg0, arg1);
      Script_scenCreateTask(script_039D, 2, 0x2876AB, 40);
      break;
    case 100:
      Script_scenCreateTask(script_0498, 6, 64124, -1, 0x41700000, 0x3F800000, arg0, arg1);
      Script_scenCreateTask(script_039D, 2, 161451, 25);
      break;
  }
  return Script_srchControl(0);
}

__attribute__((scn_string_bindings(0, data_9368, 1, data_936E)))
static int32_t script_2B73(int32_t arg0, int32_t arg1, int32_t arg2) {
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
  Script_scenCreateTask(script_039D, 2, 1193, (15 - local_E4) / local_E0);
  for (;;) {
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 2, "MPK00");
    local_DC = 0;
    while (local_DC < 7) {
      Script_InterpolateAndScaleAngles(local_DC, local_E4, &local_F4, &local_E8, "PPDN");
      local_F4.x = (&local_F4)->x * 10.0f;
      (&local_F4)->y = (&local_F4)->y * 10.0f;
      (&local_F4)->z = (&local_F4)->z * 10.0f;
      Script_charaPosRotUtils(arg0, *(&data_9258 + local_DC), &local_F4, &local_E8, 0);
      local_DC = local_DC + 1;
    }
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
    local_E4 = local_E4 + local_E0;
    if (local_E4 >= 180) {
      Script_srchControl(0);
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

__attribute__((scn_string_bindings(0, data_9373)))
static int32_t script_2C83(int32_t arg0, int32_t arg1, int32_t arg2) {
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
  Script_scenCreateTask(script_039D, 2, 1193, (15 - local_E4) / local_E0);
  for (;;) {
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 2, "MPK00");
    local_DC = 0;
    while (local_DC < 7) {
      Script_InterpolateAndScaleAngles(local_DC, local_E4, &local_F4, &local_E8, "PDN2");
      local_F4.x = (&local_F4)->x * 10.0f;
      (&local_F4)->y = (&local_F4)->y * 10.0f;
      (&local_F4)->z = (&local_F4)->z * 10.0f;
      Script_charaPosRotUtils(arg0, *(&data_9258 + local_DC), &local_F4, &local_E8, 0);
      local_DC = local_DC + 1;
    }
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
    local_E4 = local_E4 + local_E0;
    if (local_E4 >= 204) {
      Script_srchControl(0);
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

__attribute__((scn_string_bindings(0, data_937E)))
static int32_t script_2D93(int32_t arg0, int32_t arg1, int32_t arg2) {
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
  Script_scenCreateTask(script_039D, 2, 66729, (10 - local_E4) / local_E0);
  for (;;) {
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 2, "MPK00");
    local_DC = 0;
    while (local_DC < 6) {
      Script_InterpolateAndScaleAngles(local_DC, local_E4, &local_F4, &local_E8, "DRDN");
      local_F4.x = (&local_F4)->x * 10.0f;
      (&local_F4)->y = (&local_F4)->y * 10.0f;
      (&local_F4)->z = (&local_F4)->z * 10.0f;
      Script_charaPosRotUtils(arg0, *(&data_9274 + local_DC), &local_F4, &local_E8, 0);
      local_DC = local_DC + 1;
    }
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
    local_E4 = local_E4 + local_E0;
    if (local_E4 >= 188) {
      Script_srchControl(0);
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

__attribute__((scn_string_bindings(0, data_9398)))
static int32_t script_2EA5(int32_t arg0, int32_t arg1, int32_t arg2) {
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
  Script_scenCreateTask(script_039D, 2, 197801, (10 - local_E4) / local_E0);
  for (;;) {
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 2, "MPK00");
    local_DC = 0;
    while (local_DC < 18) {
      Script_InterpolateAndScaleAngles(local_DC, local_E4, &local_F4, &local_E8, "IKDN");
      local_F4.x = (&local_F4)->x * 10.0f;
      (&local_F4)->y = (&local_F4)->y * 10.0f;
      (&local_F4)->z = (&local_F4)->z * 10.0f;
      Script_charaPosRotUtils(arg0, *(&data_928C + local_DC), &local_F4, &local_E8, 0);
      local_DC = local_DC + 1;
    }
    if (local_E4 > 61) {
      Script_charaPosRotUtils(arg0, *&data_928C, &data_938C, 0, 0);
      Script_charaPosRotUtils(arg0, *(&data_928C + 12), &data_938C, 0, 0);
    }
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
    local_E4 = local_E4 + local_E0;
    if (local_E4 >= 180) {
      Script_srchControl(0);
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

__attribute__((scn_string_bindings(0, data_93A3)))
static int32_t script_2FEF(int32_t arg0, int32_t arg1, int32_t arg2) {
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
  local_E0 = 1;
  Script_scenCreateTask(script_039D, 2, 132265, (5 - local_E4) / local_E0);
  for (;;) {
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 2, "MPK00");
    local_DC = 0;
    while (local_DC < 37) {
      Script_InterpolateAndScaleAngles(local_DC, local_E4, &local_F4, &local_E8, "KADY");
      local_F4.x = (&local_F4)->x * 10.0f;
      (&local_F4)->y = (&local_F4)->y * 10.0f;
      (&local_F4)->z = (&local_F4)->z * 10.0f;
      Script_charaPosRotUtils(arg0, *(int32_t *)(data_92D4 + (local_DC << 2)), &local_F4, &local_E8, 0);
      local_DC = local_DC + 1;
    }
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
    local_E4 = local_E4 + local_E0;
    if (local_E4 >= 100) {
      Script_srchControl(0);
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_3100(int32_t arg0) {
  int32_t local_FC;
  Vector3 local_F0;
  Vector3 local_E4;
  Vector3 local_D8;
  if (arg0 > 0) {
    SCNF_WaitFrames_0000(arg0);
  }
  Script_EVT_GetPosition(CHARA_AKIR, -1, &local_F0, ENTITY_XFORM_WORLD_SPACE);
  local_E4.x = 0.0f;
  (&local_E4)->y = 0.01f;
  (&local_E4)->z = 0.0f;
  local_D8.x = 0.0f;
  (&local_D8)->y = 0.0f;
  (&local_D8)->z = 0.0f;
  local_FC = Script_EffectUtils(0, &local_F0, &data_93B0, 100, 0.001f, 0, 3);
  Script_EffectUtils(12, local_FC, 32);
  Script_EffectUtils(11, local_FC, 255);
  Script_EffectUtils(9, local_FC, 142, 125, 110);
  Script_EffectUtils(10, local_FC, 0.262745112f);
  Script_EffectUtils(8, local_FC, 2.54901958f, 1.254902f);
  Script_EffectUtils(7, local_FC, 0.0f);
  Script_EffectUtils(6, local_FC, &local_E4, &local_D8);
  Script_EffectUtils(15, local_FC, 55.863998f, 1.41176474f);
  Script_EffectUtils(14, local_FC, 0.752941191f, 0.00784313772f);
  Script_EffectUtils(1, local_FC);
  Script_EffectUtils(7, local_FC, 0.1f);
  SCNF_WaitFrames_0000(8);
  Script_EffectUtils(7, local_FC, 0.0f);
  SCNF_WaitFrames_0000(150);
  Script_EffectUtils(3, local_FC);
  return Script_srchControl(0);
}

static int32_t script_323F(int32_t arg0, int32_t arg1, float arg2, int32_t arg3, const void *arg4) {
  SCNF_WaitFrames_0000(1);
  task_308[arg1] = arg0;
  task_328[arg1] = arg3;
  script_32D5(arg1, arg2, arg4);
  for (;;) {
    if (task_348[arg1]) {
      task_388[arg1] = Script_walk3D(ActivateGetIdx, arg0, task_3A8[arg1]);
    }
    if (task_388[arg1] != -1) {
      task_368[arg1] = task_368[arg1] + 1;
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_32D5(int32_t arg0, float arg1, const void *arg2) {
  int32_t local_FC;
  task_348[arg0] = 0;
  task_368[arg0] = 0;
  task_388[arg0] = 0;
  task_3A8[arg0] = arg1;
  local_FC = task_328[arg0];
  if ((local_FC == 0) | (local_FC == 1) | (local_FC == 2)) {
    Script_PlayCharaMotion(task_308[arg0], MN_AKI_AKI_L_STAND, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    goto L003357;
  }
  L003357:;
  Script_walk3D(SetupPath, task_308[arg0], arg2, 0);
  return;
}

static void script_3370(int32_t arg0, float arg1) {
  int32_t local_FC;
  task_3A8[arg0] = arg1;
  task_368[arg0] = 0;
  task_348[arg0] = 1;
  local_FC = task_328[arg0];
  switch (local_FC) {
    case 0:
      Script_charaToggleMotEnabled(task_308[arg0], 0);
      Script_SetCharaMotionPlaybackRate(task_308[arg0], 1.0f);
      Script_PlayCharaMotion(task_308[arg0], MN_AKI_AKI_WALK_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
      break;
    case 1:
      Script_charaToggleMotEnabled(task_308[arg0], 0);
      Script_SetCharaMotionPlaybackRate(task_308[arg0], 1.0f);
      Script_PlayCharaMotion(task_308[arg0], MN_AKI_AKI_RUN_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
      break;
    case 2:
      Script_motmToggleFlags(task_308[arg0], 1, 4);
      Script_charaToggleMotEnabled(task_308[arg0], 0);
      Script_SetCharaMotionPlaybackRate(task_308[arg0], 2.5f);
      Script_PlayCharaMotion(task_308[arg0], 64102, -1.0f, -1.0f, -1.0f, 1.0f, 0);
      break;
  }
  return;
}

static void script_34BC(int32_t arg0, int32_t arg1, int32_t arg2) {
  Script_scenCreateTask(script_34D0, 3, arg0, arg1, arg2);
  return;
}

static int32_t script_34D0(int32_t arg0, float arg1, int32_t arg2) {
  if (arg2 >= 0) {
    SCNF_WaitFrames_0000(arg2);
  }
  script_3370(arg0, arg1);
  return Script_srchControl(0);
}

static void script_34F5(int32_t arg0) {
  int32_t local_FC;
  task_348[arg0] = 0;
  local_FC = task_328[arg0];
  switch (local_FC) {
    case 0:
      Script_charaToggleMotEnabled(task_308[arg0], 1);
      Script_SetCharaMotionPlaybackRate(task_308[arg0], 1.0f);
      break;
    case 1:
      Script_charaToggleMotEnabled(task_308[arg0], 1);
      Script_SetCharaMotionPlaybackRate(task_308[arg0], 1.0f);
      Script_PlayCharaMotion(task_308[arg0], MN_AKI_AKI_RUN_EN, 2.0f, -1.0f, -1.0f, 1.0f, 0);
      break;
    case 2:
      Script_motmToggleFlags(task_308[arg0], 2, 4);
      Script_charaToggleMotEnabled(task_308[arg0], 1);
      Script_SetCharaMotionPlaybackRate(task_308[arg0], 1.0f);
      break;
  }
  return;
}

static void script_35D8(int32_t arg0, int32_t arg1) {
  Script_scenCreateTask(script_35EA, 2, arg0, arg1);
  return;
}

static int32_t script_35EA(int32_t arg0, int32_t arg1) {
  if (arg1 >= 0) {
    SCNF_WaitFrames_0000(arg1);
  }
  script_34F5(arg0);
  return Script_srchControl(0);
}

static int32_t script_360D(void) {
  Script_shenSetLightingMode(2);
  Script_SetupLightFromTable(1);
  Script_SetFogColour(&data_93BC);
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 1);
  Script_SetCharaRefBoardPos(CHARA_AKIR, &data_93CC);
  Script_lightSetSlotEnabled(16, 0);
  Script_lightSetSlotEnabled(2, 0);
  Script_lightSetSlotEnabled(4, 0);
  Script_lightSetSlotEnabled(6, 0);
  for (;;) {
    if (task_8 == 1) {
      Script_srchControl(0);
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_367C(void) {
  Script_SetCurrentScrollEnabled(0);
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

static void script_36B2(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Script_scenCreateTask(script_36C8, 4, arg0, arg1, arg2, arg3);
  return;
}

static int32_t script_36C8(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  if (arg3 >= 0) {
    SCNF_WaitFrames_0000(arg3);
  }
  Script_seqEnterEvtCam(arg0, arg1, arg2);
  return Script_srchControl(0);
}

static void script_36EC(int32_t arg0, int32_t arg1) {
  Script_scenCreateTask(script_36FE, 2, arg0, arg1);
  return;
}

static int32_t script_36FE(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  if (arg1 >= 0) {
    SCNF_WaitFrames_0000(arg1);
  }
  Script_StartOrFreeSEQ(0);
  local_FC = arg0;
  switch (local_FC) {
    case 0:
      Script_ShowUIMessageFromID("E1022C004");
      break;
    case 1:
      Script_ShowUIMessageFromID("E1022B009");
      break;
    case 2:
      Script_ShowUIMessageFromID("E1022B010");
      break;
    case 3:
      Script_ShowUIMessageFromID("E1022B011");
      break;
    case 4:
      Script_ShowUIMessageFromID("E1022D004");
      break;
    case 5:
      Script_ShowUIMessageFromID("E1022A006");
      break;
    case 6:
      Script_ShowUIMessageFromID("E1022D002");
      break;
    case 7:
      Script_ShowUIMessageFromID("E1022D003");
      break;
    case 8:
      Script_ShowUIMessageFromID("E1022B012");
      break;
    case 9:
      Script_ShowUIMessageFromID("E1022B013");
      break;
    case 10:
      Script_ShowUIMessageFromID("E1022C005");
      break;
    case 11:
      Script_ShowUIMessageFromID("E1022B014");
      break;
    case 12:
      Script_ShowUIMessageFromID("E1022B015");
      break;
    case 13:
      Script_ShowUIMessageFromID("E1022C002");
      break;
    case 14:
      Script_ShowUIMessageFromID("E1022C003");
      break;
    case 15:
      Script_ShowUIMessageFromID("E1022F001");
      break;
    case 16:
      Script_ShowUIMessageFromID("E1022F002");
      break;
    case 17:
      Script_ShowUIMessageFromID("E1022G001");
      break;
    case 18:
      Script_ShowUIMessageFromID("E1022G002");
      break;
    case 19:
      Script_ShowUIMessageFromID("E1022I001");
      break;
    case 20:
      Script_ShowUIMessageFromID("E1022I002");
      break;
    case 21:
      Script_ShowUIMessageFromID("E1022J001");
      break;
    case 22:
      Script_ShowUIMessageFromID("E1022J002");
      break;
    case 23:
      Script_ShowUIMessageFromID("E1022K001");
      break;
    case 24:
      Script_ShowUIMessageFromID("E1022K002");
      break;
  }
  return Script_srchControl(0);
}

static void script_3932(int32_t arg0, int32_t arg1) {
  task_3C8 = arg0;
  task_0 = 0;
  return;
}

static void script_3943(int32_t arg0, int32_t arg1) {
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
    case 24:
      SCNT_ShowSubtitle("開始７");
      break;
    case 25:
      SCNT_ShowSubtitle("成功７");
      break;
    case 26:
      SCNT_ShowSubtitle("失敗７");
      break;
    case 27:
      SCNT_ShowSubtitle("開始８");
      break;
    case 28:
      SCNT_ShowSubtitle("成功８");
      break;
    case 29:
      SCNT_ShowSubtitle("失敗８");
      break;
    case 30:
      SCNT_ShowSubtitle("開始９");
      break;
    case 31:
      SCNT_ShowSubtitle("成功９");
      break;
    case 32:
      SCNT_ShowSubtitle("失敗９");
      break;
    case 33:
      SCNT_ShowSubtitle("開始１０");
      break;
    case 34:
      SCNT_ShowSubtitle("成功１０");
      break;
    case 35:
      SCNT_ShowSubtitle("失敗１０");
      break;
    case 36:
      SCNT_ShowSubtitle("開始１１");
      break;
    case 37:
      SCNT_ShowSubtitle("成功１１");
      break;
    case 38:
      SCNT_ShowSubtitle("失敗１１");
      break;
    case 39:
      SCNT_ShowSubtitle("開始１２");
      break;
    case 40:
      SCNT_ShowSubtitle("成功１２");
      break;
    case 41:
      SCNT_ShowSubtitle("失敗１２");
      break;
    case 42:
      SCNT_ShowSubtitle("開始１３");
      break;
    case 43:
      SCNT_ShowSubtitle("成功１３");
      break;
    case 44:
      SCNT_ShowSubtitle("失敗１３");
      break;
    case 45:
      SCNT_ShowSubtitle("開始１４");
      break;
    case 46:
      SCNT_ShowSubtitle("成功１４");
      break;
    case 47:
      SCNT_ShowSubtitle("失敗１４");
      break;
  }
  Script_srchControl(0);
  return;
}

static void script_3C93(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  Script_scenCreateTask(script_3CAB, 5, arg0, arg1, arg2, arg3, arg4);
  return;
}

static int32_t script_3CAB(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  if (arg4 >= 0) {
    SCNF_WaitFrames_0000(arg4);
  }
  Script_FACE(arg0, arg1, arg2, arg3);
  return Script_srchControl(0);
}

static void script_3CD2(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  Script_scenCreateTask(script_3CEA, 5, arg0, arg1, arg2, arg3, arg4);
  return;
}

static int32_t script_3CEA(int32_t arg0, int32_t arg1, float arg2, int32_t arg3, int32_t arg4) {
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

static void script_3D65(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Script_scenCreateTask(script_3D7B, 4, arg0, arg1, arg2, arg3);
  return;
}

static int32_t script_3D7B(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
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

static void script_3DB7(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5, int32_t arg6) {
  Script_scenCreateTask(script_3DD3, 7, arg2, arg6, arg3, arg4, arg0, arg1, arg5);
  return;
}

static int32_t script_3DD3(int32_t arg0, int32_t arg1, int32_t arg2, float arg3, int32_t arg4, int32_t arg5, int32_t arg6) {
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

__attribute__((scn_string_bindings(0, data_96E0, 1, data_96E6)))
static void script_3E29(void) {
  int32_t local_FC;
  Vector3 local_F0;
  Vector3i local_E4;
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 2, "MPK00");
  local_FC = 0;
  while (local_FC < 7) {
    Script_InterpolateAndScaleAngles(local_FC, 2, &local_F0, &local_E4, "PPDN");
    local_F0.x = (&local_F0)->x * 10.0f;
    (&local_F0)->y = (&local_F0)->y * 10.0f;
    (&local_F0)->z = (&local_F0)->z * 10.0f;
    Script_charaPosRotUtils(SHEN_NAME('PIP0'), *(&data_9258 + local_FC), &local_F0, &local_E4, 0);
    local_FC = local_FC + 1;
  }
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
  Script_charaDisplay(SHEN_NAME('PIP0'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('PPD0'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('DRM3'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('DRD0'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('IKA1'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('IKA0'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('KBK1'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('KBK0'), CDM_Hide);
  return;
}

static int32_t script_3F24(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  int32_t unused_F8;
  int32_t local_EC[3];
  int32_t local_E8;
  int32_t unused_E4;
  GameDateTime local_DC;
  int32_t local_D8;
  SCNF_WaitFrames_0000(5);
  local_EC[0] = Script_scenCreateTask(script_323F, 5, arg0, arg1, 0x3DCCCCCD, 0, &data_93DC);
  local_EC[1] = Script_scenCreateTask(script_323F, 5, task_BC[6], 6, 0x3DCCCCCD, 1, &data_9408);
  local_EC[2] = Script_scenCreateTask(script_323F, 5, task_BC[1], 1, 0x3DCCCCCD, 2, &data_9434);
  SCNF_WaitFrames_0000(5);
  local_E8 = Script_SetMaxNPCsNear(32);
  Script_SetSfxOverlayTimeoutTuning(0, SfxMode_SessionAction);
  Script_uiUtils(0, 21, 0);
  script_1249(14);
  Script_PlayerStatsUtils(PS_IncrementSpotVisitCount, 85);
  for (;;) {
    if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_LAN_DI_STILL_IN_HARBOR) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_MFSY_HARBOR_FOREIGNERS_INQUIRY) == 0)) {
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
    script_3E29();
    script_68DE(arg0, arg1);
    script_107B(0, 7);
    Script_ReturnTimeDate(&local_DC);
    for (;;) {
      Script_SetNewDateTime(&local_DC);
      local_FC = Script_scenCreateTask(script_4A6A, 2, arg0, arg1);
      task_150 = 0;
      script_1160(0, 7);
      SCNF_WaitFrames_0000(110);
      Script_executeAicaCommand(156840, 0, 0);
      Script_executeAicaCommand(1184, 2, 127);
      for (;;) {
        if (task_150 >= 10) {
          Script_srchControl((void *)local_FC);
          local_D8 = task_3C8;
          if ((local_D8 == 6) | (local_D8 == 8)) {
            Vector3i local_CC;
            script_3932(5, -1);
            script_3C93(arg0, 4, 0, 8, 5);
            script_36B2(8050, SHEN_NAME('DUM1'), SHEN_NAME('DUM1'), -1);
            script_3D65(SHEN_NAME('BIN_'), 0, 0, -1);
            script_3D65(task_BC[2], 0, 0, -1);
            Script_charaToggleMotEnabled(arg0, 1);
            Script_charaToggleMotEnabled(task_BC[1], 1);
            SCNF_WaitFrames_0000(20);
            (&local_CC)->y = -10922;
            Script_EVT_SetRotation(arg0, -0x70000000, &local_CC, 0);
            Script_PlayCharaMotion(arg0, 64086, 20.0f, -1.0f, -1.0f, 1.0f, 0);
            Script_PlayCharaMotion(task_BC[1], MN_EXB_YKI_AKI_STANDBY3, 0.0f, -1.0f, -1.0f, 1.0f, 0);
            Script_scenCreateTask(script_039D, 2, 0x2876AB, 22);
          } else if ((local_D8 == 9) | (local_D8 == 11) | (local_D8 == 14)) {
            script_3932(5, -1);
            script_3C93(arg0, 4, 0, 5, 5);
            script_36B2(8050, SHEN_NAME('DUM1'), SHEN_NAME('DUM1'), -1);
            Script_charaToggleMotEnabled(arg0, 1);
            Script_charaToggleMotEnabled(task_BC[2], 1);
            Script_PlayCharaMotion(arg0, 64013, 38.0f, -1.0f, -1.0f, 1.0f, 0);
            Script_scenCreateTask(script_039D, 2, 0x2876AB, 27);
          } else if (local_D8 == 15) {
            script_3932(5, -1);
            script_3C93(arg0, 4, 0, 8, 5);
            script_36B2(8050, SHEN_NAME('DUM1'), SHEN_NAME('DUM1'), -1);
            SCNF_WaitFrames_0000(16);
            Script_PlayCharaMotion(arg0, 64013, 42.0f, -1.0f, -1.0f, 1.0f, 0);
            Script_scenCreateTask(script_039D, 2, 0x2876AB, 23);
          } else if (local_D8 == 16) {
            Vector3i local_CC;
            script_3932(5, -1);
            script_3C93(arg0, 4, 0, 8, 5);
            script_36B2(8050, SHEN_NAME('DUM1'), SHEN_NAME('DUM1'), -1);
            Script_charaToggleMotEnabled(arg0, 1);
            SCNF_WaitFrames_0000(20);
            (&local_CC)->y = -5461;
            Script_EVT_SetRotation(arg0, -0x70000000, &local_CC, 0);
            Script_PlayCharaMotion(arg0, 64086, 20.0f, -1.0f, -1.0f, 1.0f, 0);
            Script_scenCreateTask(script_039D, 2, 0x2876AB, 22);
          } else if (local_D8 == 17) {
            script_3932(5, -1);
            script_3C93(arg0, 4, 0, 8, 5);
            script_36B2(8050, SHEN_NAME('DUM1'), SHEN_NAME('DUM1'), -1);
            Script_charaToggleMotEnabled(arg0, 1);
            SCNF_WaitFrames_0000(20);
            Script_PlayCharaMotion(arg0, 64086, 20.0f, -1.0f, -1.0f, 1.0f, 0);
            Script_PlayCharaMotion(task_BC[4], MN_EXB_YKI_AKI_STANDBY3, 0.0f, -1.0f, -1.0f, 1.0f, 0);
            Script_scenCreateTask(script_039D, 2, 0x2876AB, 22);
          } else if (local_D8 == 27) {
            script_3932(5, -1);
            script_3C93(arg0, 4, 0, 8, 5);
            script_36B2(8050, SHEN_NAME('DUM1'), SHEN_NAME('DUM1'), -1);
            Script_charaToggleMotEnabled(arg0, 1);
            Script_charaToggleMotEnabled(task_BC[5], 1);
            SCNF_WaitFrames_0000(9);
            Script_PlayCharaMotion(arg0, 64013, 35.0f, -1.0f, -1.0f, 1.0f, 0);
            Script_motmToggleFlags(arg0, 1, 0x80);
            Script_scenCreateTask(script_039D, 2, 0x2876AB, 30);
          } else if (local_D8 == 29) {
            script_3932(5, -1);
            script_3C93(arg0, 4, 0, 8, 5);
            script_36B2(8050, SHEN_NAME('DUM1'), SHEN_NAME('DUM1'), -1);
            script_0CFF(task_BC[5], arg0, -0.74f, -0.74f, -70.0f);
            Script_charaToggleMotEnabled(arg0, 1);
            Script_charaToggleMotEnabled(task_BC[5], 1);
            SCNF_WaitFrames_0000(3);
            Script_PlayCharaMotion(arg0, 64013, 35.0f, -1.0f, -1.0f, 1.0f, 0);
            Script_motmToggleFlags(arg0, 1, 0x80);
            Script_scenCreateTask(script_039D, 2, 0x2876AB, 30);
          } else if (local_D8 == 30) {
            script_3932(5, -1);
            script_3C93(arg0, 4, 0, 8, 5);
            script_36B2(8050, SHEN_NAME('DUM1'), SHEN_NAME('DUM1'), -1);
            SCNF_WaitFrames_0000(10);
            Script_PlayCharaMotion(arg0, 64086, 20.0f, -1.0f, -1.0f, 1.0f, 0);
            Script_scenCreateTask(script_039D, 2, 0x2876AB, 22);
          } else if ((local_D8 == 32) | (local_D8 == 47)) {
            script_3932(5, -1);
            script_36EC(11, -1);
            script_3C93(arg0, 4, 0, 12, 5);
            script_36B2(8050, SHEN_NAME('DUM1'), SHEN_NAME('DUM1'), -1);
            SCNF_WaitFrames_0000(14);
            Script_PlayCharaMotion(arg0, 64013, 48.0f, -1.0f, -1.0f, 1.0f, 0);
            Script_motmToggleFlags(arg0, 1, 0x80);
            Script_scenCreateTask(script_039D, 2, 0x2876AB, 17);
          } else if (local_D8 == 33) {
            script_3932(5, -1);
            script_3C93(arg0, 4, 0, 8, 5);
            script_36B2(8050, SHEN_NAME('DUM1'), SHEN_NAME('DUM1'), -1);
            SCNF_WaitFrames_0000(4);
            Script_PlayCharaMotion(arg0, 64013, 38.0f, -1.0f, -1.0f, 1.0f, 0);
            Script_scenCreateTask(script_039D, 2, 0x2876AB, 27);
          } else if (local_D8 == 36) {
            script_3932(5, -1);
            script_3C93(arg0, 4, 0, 8, 5);
            script_36B2(8050, SHEN_NAME('DUM1'), SHEN_NAME('DUM1'), -1);
            Script_PlayCharaMotion(arg0, 64013, 38.0f, -1.0f, -1.0f, 1.0f, 0);
            Script_motmToggleFlags(arg0, 1, 0x80);
            Script_scenCreateTask(script_039D, 2, 0x2876AB, 27);
          } else if (local_D8 == 39) {
            script_3932(5, -1);
            script_3C93(arg0, 4, 0, 8, 8);
            script_36B2(8056, SHEN_NAME('DUM1'), SHEN_NAME('DUM1'), -1);
            script_0CFF(arg0, task_BC[7], 0.0f, -1.9f, 180.0f);
            SCNF_WaitFrames_0000(6);
            Script_PlayCharaMotion(arg0, 64013, 38.0f, -1.0f, -1.0f, 1.0f, 0);
            Script_scenCreateTask(script_039D, 2, 0x2876AB, 27);
            SCNF_WaitFrames_0000(14);
            script_0F17(task_BC[7], 90.0f);
            Script_PlayCharaMotion(task_BC[7], MN_EXB_YKI_AKI_STANDBY3, 0.0f, -1.0f, -1.0f, 1.0f, 0);
          } else if (local_D8 == 41) {
            script_3932(5, -1);
            script_3C93(arg0, 4, 0, 8, 8);
            script_36B2(8055, SHEN_NAME('DUM1'), SHEN_NAME('DUM1'), -1);
          } else {
            script_3932(5, -1);
            script_3C93(arg0, 4, 0, 8, 8);
            script_36B2(8050, SHEN_NAME('DUM1'), SHEN_NAME('DUM1'), -1);
          }
          break;
        } else {
          if (task_150 == -1) {
            Script_srchControl((void *)local_FC);
            break;
            goto L0048C1;
          }
        }
        L0048C1:;
        SCNF_WaitFrames_0000(1);
      }
      Script_executeAicaCommand(2720, 2, 30);
      if (task_150 == -1) {
        Script_filmCheckOrSetTransitionUtil(60, 0, 0, 0, 0, 255, 0, 0, 0);
        while (Script_filmCheckOrSetTransitionUtil(-1)) {
          SCNF_WaitFrames_0000(1);
        }
        SCNF_WaitFrames_0000(60);
        script_7525(arg0, arg1);
        script_3E29();
        script_12AF();
        Script_SetSfxOverlayTimeoutTuning(2, SfxMode_SessionAction);
        if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_LAN_DI_STILL_IN_HARBOR) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_MFSY_HARBOR_FOREIGNERS_INQUIRY) == 0)) {
          Script_SetMaxNPCsNear(local_E8);
          Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_MFSY_HARBOR_FOREIGNERS_INQUIRY, 1);
          Script_shenRequestGameModuleTransition(3, SHEN_NAME('MA00'), 1);
          SCNF_WaitFrames_0000(1);
        } else {
          Script_executeAicaCommand(91304, 0, 0);
          Script_executeAicaCommand(1184, 2, 127);
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
        script_3E29();
        script_1267();
        Script_SetSfxOverlayTimeoutTuning(1, SfxMode_SessionAction);
        if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_LAN_DI_STILL_IN_HARBOR) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_MFSY_HARBOR_FOREIGNERS_INQUIRY) == 0)) {
          Script_filmCheckOrSetTransitionUtil(60, 255, 0, 0, 0, 0, 0, 0, 0);
          Script_executeAicaCommand(91304, 0, 0);
          Script_executeAicaCommand(1184, 2, 127);
        } else {
          Script_executeAicaCommand(91304, 0, 0);
          Script_executeAicaCommand(1184, 2, 127);
          break;
        }
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_4A6A(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  local_FC = 0;
  script_3C93(arg0, 2, 0, 1, -1);
  script_3C93(task_BC[6], 2, 0, 1, -1);
  script_3C93(task_BC[7], 2, 0, 1, -1);
  script_36EC(4, -1);
  script_36EC(5, 50);
  script_36B2(8070, SHEN_NAME('DUMS'), SHEN_NAME('DUMS'), -1);
  script_36B2(8071, SHEN_NAME('DUM1'), SHEN_NAME('DUM1'), 50);
  script_3D65(SHEN_NAME('BIN_'), 0, 0, -1);
  script_3D65(arg0, 1, 0, -1);
  script_3D65(task_BC[1], 1, 0, -1);
  script_3D65(task_BC[2], 0, 0, -1);
  script_3D65(task_BC[3], 0, 0, -1);
  script_3D65(task_BC[4], 0, 0, -1);
  script_3D65(task_BC[5], 0, 0, -1);
  script_3D65(task_BC[6], 0, 0, -1);
  script_3D65(task_BC[7], 0, 0, -1);
  script_3DB7(task_BC[1], 1, 88, 0, 0x3F800000, 0, -1);
  script_3DB7(task_BC[2], 2, 20494, 0, 0x3F800000, 0, 67);
  script_3DB7(task_BC[5], 5, 20482, 0, 0x3F800000, 0, 80);
  SCNF_WaitFrames_0000(110);
  script_0CFF(arg0, 0x304D5544, 7.5f, 10.1f, 280.5f);
  script_0CFF(task_BC[1], arg0, -0.08f, -0.7f, 160.0f);
  script_0CFF(task_BC[6], 0x304D5544, 13.68f, 8.3f, 100.0f);
  script_0F94(task_BC[2], 0x42B40000, -1);
  script_0F94(task_BC[4], 0x43070000, -1);
  script_0F94(task_BC[5], 0x42340000, -1);
  Script_PlayCharaMotion(task_BC[1], 64109, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_PlayCharaMotion(task_BC[2], MN_EXB_YKI_AKI_STANDBY3, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_PlayCharaMotion(task_BC[4], MN_EXB_YKI_AKI_STANDBY3, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_SetCharaMotionPlaybackRate(task_BC[2], 0.5f);
  Script_charaToggleMotEnabled(arg0, 1);
  Script_charaToggleMotEnabled(task_BC[1], 1);
  Script_charaToggleMotEnabled(task_BC[2], 1);
  Script_charaToggleMotEnabled(task_BC[3], 1);
  Script_charaToggleMotEnabled(task_BC[4], 1);
  Script_charaToggleMotEnabled(task_BC[5], 1);
  Script_charaToggleMotEnabled(task_BC[6], 1);
  Script_charaToggleMotEnabled(task_BC[7], 1);
  Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_7);
  Script_motmSetBlendFrames(task_BC[1], MOTM_BLEND_FRAMES_7);
  Script_motmSetBlendFrames(task_BC[2], MOTM_BLEND_FRAMES_IMMEDIATE);
  Script_motmSetBlendFrames(task_BC[3], MOTM_BLEND_FRAMES_7);
  Script_motmSetBlendFrames(task_BC[4], MOTM_BLEND_FRAMES_IMMEDIATE);
  Script_motmSetBlendFrames(task_BC[5], MOTM_BLEND_FRAMES_7);
  Script_motmSetBlendFrames(task_BC[6], MOTM_BLEND_FRAMES_7);
  Script_motmSetBlendFrames(task_BC[7], MOTM_BLEND_FRAMES_7);
  script_3932(2, -1);
  script_36EC(6, 40);
  script_36B2(8051, SHEN_NAME('DUM1'), SHEN_NAME('DUM1'), -1);
  script_3D65(SHEN_NAME('BIN_'), 1, 0, -1);
  script_3D65(arg0, 1, 0, -1);
  script_3D65(task_BC[1], 1, 0, -1);
  script_3D65(task_BC[2], 1, 0, -1);
  script_3D65(task_BC[3], 0, 0, -1);
  script_3D65(task_BC[4], 1, 0, -1);
  script_3D65(task_BC[5], 1, 0, -1);
  script_3D65(task_BC[6], 1, 0, -1);
  script_3D65(task_BC[7], 0, 0, -1);
  SCNF_WaitFrames_0000(30);
  task_144 = 0;
  while (task_144 == 0) {
    script_3932(6, -1);
    script_08F4(arg1, 8, 1, 0x10000000);
    if (task_144) {
      script_3932(7, -1);
      script_36EC(7, 30);
      script_36B2(8054, SHEN_NAME('DUM1'), SHEN_NAME('DUM1'), 20);
      script_3D65(SHEN_NAME('BIN_'), 0, 0, 20);
      script_3D65(arg0, 1, 1, 20);
      script_3D65(task_BC[1], 1, 1, 20);
      script_3D65(task_BC[2], 0, 0, 20);
      script_3D65(task_BC[4], 1, 1, 20);
    } else {
      script_3932(8, -1);
      script_3C93(arg0, 4, 0, 8, 5);
      script_3C93(arg0, 2, 0, 16, 16);
    }
  }
  Script_scenCreateTask(script_2D93, 3, SHEN_NAME('DRD0'), SHEN_NAME('DRM3'), 40);
  Script_PlayCharaMotion(task_BC[3], MN_EXB_YKI_AKI_STANDBY3, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_SetCharaMotionPlaybackRate(task_BC[3], 0.5f);
  script_0F94(task_BC[3], -0x3D4C0000, -1);
  SCNF_WaitFrames_0000(70);
  Script_motmSetBlendFrames(task_BC[2], MOTM_BLEND_FRAMES_7);
  Script_motmSetBlendFrames(task_BC[4], MOTM_BLEND_FRAMES_7);
  script_0CFF(arg0, 0x304D5544, 8.3f, 10.9f, 280.0f);
  script_0CFF(task_BC[2], arg0, -0.25f, -2.8f, 180.0f);
  script_3932(9, -1);
  script_36EC(17, 28);
  script_36B2(8051, SHEN_NAME('DUM1'), SHEN_NAME('DUM1'), -1);
  script_3D65(SHEN_NAME('BIN_'), 1, 0, -1);
  script_3D65(arg0, 1, 0, -1);
  script_3D65(task_BC[1], 1, 0, -1);
  script_3D65(task_BC[2], 1, 0, -1);
  script_3D65(task_BC[3], 1, 0, -1);
  script_3D65(task_BC[4], 1, 0, -1);
  script_3D65(task_BC[5], 0, 0, -1);
  script_3D65(task_BC[6], 1, 0, -1);
  script_3D65(task_BC[7], 0, 0, -1);
  script_08F4(arg1, 0, 2, 0x10000000);
  if (task_144) {
    script_3932(10, -1);
    script_36EC(18, 30);
    script_36B2(8052, SHEN_NAME('DUM1'), SHEN_NAME('DUM1'), 10);
    script_3D65(arg0, 1, 1, 10);
    script_3D65(task_BC[3], 1, 1, 10);
    script_3D65(task_BC[4], 1, 1, 10);
  } else {
    script_3932(11, -1);
    script_3C93(arg0, 4, 0, 8, 5);
    script_3C93(arg0, 2, 0, 16, 16);
    script_36B2(8054, SHEN_NAME('DUM1'), SHEN_NAME('DUM1'), -1);
    script_3D65(task_BC[4], 1, 1, -1);
  }
  while (task_144 == 0) {
    script_08F4(arg1, 1, 2, 0x10000000);
    if (task_144) {
      script_3932(13, -1);
      script_36EC(18, 30);
      script_36B2(8052, SHEN_NAME('DUM1'), SHEN_NAME('DUM1'), -1);
      script_3D65(arg0, 1, 1, -1);
      script_3D65(task_BC[3], 1, 1, -1);
      script_3D65(task_BC[4], 1, 1, -1);
    } else {
      script_3932(14, -1);
      script_3C93(arg0, 4, 0, 8, 5);
      script_3C93(arg0, 2, 0, 16, 16);
      script_36B2(8057, SHEN_NAME('DUM1'), SHEN_NAME('DUM1'), -1);
      script_3D65(task_BC[1], 0, 0, -1);
      script_3D65(task_BC[4], 1, 0, -1);
      script_3D65(task_BC[5], 1, 0, -1);
    }
  }
  Script_PlayCharaMotion(task_BC[3], MN_AKI_AKI_BATTLE_KAMAE2_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_PlayCharaMotion(task_BC[5], MN_AKI_AKI_BATTLE_KAMAE_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  SCNF_WaitFrames_0000(60);
  script_0CFF(arg0, 0x304D5544, 10.3f, 8.45f, 300.0f);
  script_0CFF(task_BC[3], arg0, -1.35f, -0.7f, -90.0f);
  script_0CFF(task_BC[4], arg0, 2.3f, -1.4f, 110.0f);
  script_0CFF(task_BC[5], 0x304D5544, 12.7f, 7.3f, 105.0f);
  script_3932(15, -1);
  script_36B2(8056, SHEN_NAME('DUM1'), SHEN_NAME('DUM1'), -1);
  script_3D65(SHEN_NAME('BIN_'), 0, 0, -1);
  script_3D65(arg0, 1, 0, -1);
  script_3D65(task_BC[1], 0, 0, -1);
  script_3D65(task_BC[2], 0, 0, -1);
  script_3D65(task_BC[3], 1, 1, -1);
  script_3D65(task_BC[4], 1, 1, -1);
  script_3D65(task_BC[5], 1, 0, -1);
  script_3D65(task_BC[6], 1, 0, -1);
  script_3D65(task_BC[7], 1, 0, -1);
  script_092E(arg1, 6, 3, 0x10000000);
  if (task_144 == 0) {
    script_3932(17, -1);
    script_3C93(arg0, 4, 0, 18, 5);
    script_3C93(arg0, 2, 0, 26, 16);
    script_36B2(8050, SHEN_NAME('DUM1'), SHEN_NAME('DUM1'), -1);
    script_36EC(21, -1);
    script_0CFF(task_BC[3], arg0, -1.0f, -0.6f, -90.0f);
    Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_15);
    Script_motmSetBlendFrames(task_BC[3], MOTM_BLEND_FRAMES_15);
    Script_motmSetBlendFrames(task_BC[4], MOTM_BLEND_FRAMES_15);
    SCNF_WaitFrames_0000(20);
    script_0DC1(task_BC[4], arg0, 0x3EAE147B, 0x3EB33333, 0x41A00000, 33);
    script_0DC1(task_BC[3], arg0, -0x42B33333, -0x40CCCCCD, 0x43480000, 33);
    script_0F94(arg0, 0x42B40000, 33);
    script_097A(arg1, 2, 4, 3, 0x10000000);
    if (task_144) {
      script_3932(43, -1);
    }
    while (task_144 == 0) {
      script_3C93(arg0, 4, 0, 15, 5);
      script_3C93(arg0, 2, 0, 23, 16);
      script_097A(arg1, 3, 4, 3, 0x10000000);
      if (task_144) {
        script_3932(43, -1);
      }
    }
    script_0DC1(task_BC[4], arg0, 0x41300000, 0x41100000, 0, 95);
    SCNF_WaitFrames_0000(95);
  } else {
    script_3932(16, -1);
    script_36EC(19, 28);
    script_36B2(8057, SHEN_NAME('DUM1'), SHEN_NAME('DUM1'), -1);
    script_092E(arg1, 7, 3, 0x10000000);
    if (task_144 == 0) {
      script_3932(20, -1);
      script_3C93(arg0, 4, 0, 20, 10);
      script_3C93(arg0, 2, 0, 33, 16);
      script_36B2(8050, SHEN_NAME('DUM1'), SHEN_NAME('DUM1'), -1);
      script_3DB7(task_BC[4], 4, 1054, 0, 0x3F800000, 1, 35);
      script_0F94(task_BC[4], 0x43340000, 35);
      SCNF_WaitFrames_0000(90);
      script_0DC1(task_BC[3], arg0, 0x3EB33333, 0x3F8CCCCD, -0x3E600000, 10);
      script_0DC1(task_BC[4], arg0, -0x40C00000, 0x3FE66666, -0x3E600000, 10);
      script_0F94(arg0, 0x43340000, 10);
      script_0F94(task_BC[4], 0x43340000, 10);
      script_36B2(8057, SHEN_NAME('DUM1'), SHEN_NAME('DUM1'), 10);
      script_092E(arg1, 11, 3, 0x10000000);
      if (task_144 == 0) {
        script_3932(23, -1);
        script_3C93(arg0, 4, 0, 8, 5);
        script_3C93(arg0, 2, 0, 16, 16);
        SCNF_WaitFrames_0000(120);
        task_150 = 10;
        SCNF_WaitFrames_0000(1);
      }
      script_3932(24, -1);
      script_092E(arg1, 12, 3, 0x10000000);
      if (task_144 == 0) {
        script_3932(26, -1);
        script_3C93(arg0, 4, 0, 8, 5);
        script_3C93(arg0, 2, 0, 16, 16);
        SCNF_WaitFrames_0000(120);
        task_150 = 10;
        SCNF_WaitFrames_0000(1);
      } else {
        script_3932(25, -1);
        script_36B2(8058, SHEN_NAME('DUM1'), SHEN_NAME('DUM1'), -1);
        script_3DB7(arg0, arg1, 787, 0, 0x3F800000, 1, 28);
        script_3DB7(arg0, arg1, 102, 0, 0x3F800000, 0, 58);
        script_3DB7(arg0, arg1, 787, 0, 0x3F800000, 0, 93);
        script_3DB7(task_BC[3], 3, 64013, 0x42200000, 0x3F800000, 0, 5);
        script_3DB7(task_BC[4], 4, 64013, 0x42200000, 0x3F800000, 1, 20);
        Script_scenCreateTask(script_039D, 2, 0x2876AB, 30);
        Script_scenCreateTask(script_039D, 2, 0x2876AB, 45);
        SCNF_WaitFrames_0000(78);
      }
    } else {
      script_3932(19, -1);
      script_36EC(20, 10);
      script_36EC(22, 35);
      script_36B2(8050, SHEN_NAME('DUM1'), SHEN_NAME('DUM1'), -1);
      Script_scenCreateTask(script_2EA5, 3, SHEN_NAME('IKA0'), SHEN_NAME('IKA1'), 20);
      local_FC = 1;
      SCNF_WaitFrames_0000(50);
    }
  }
  Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_7);
  script_0CFF(arg0, 0x304D5544, 10.19f, 7.6f, 250.5f);
  script_0CFF(task_BC[5], arg0, -1.3f, -1.8f, -90.0f);
  script_3932(27, -1);
  script_36EC(23, 30);
  script_36B2(8057, SHEN_NAME('DUM1'), SHEN_NAME('DUM1'), -1);
  script_3D65(SHEN_NAME('BIN_'), 0, 0, -1);
  script_3D65(arg0, 1, 1, -1);
  script_3D65(task_BC[1], 0, 0, -1);
  script_3D65(task_BC[2], 0, 0, -1);
  script_3D65(task_BC[3], 1, 0, -1);
  script_3D65(task_BC[4], 1, 0, -1);
  script_3D65(task_BC[5], 1, 0, -1);
  script_3D65(task_BC[6], 1, 0, -1);
  script_3D65(task_BC[7], 1, 0, -1);
  script_08F4(arg1, 4, 5, -0x70000000);
  if (task_144) {
    script_3932(28, -1);
    script_36EC(24, 30);
    script_36B2(8068, SHEN_NAME('DUM1'), SHEN_NAME('DUM1'), -1);
    script_0F17(task_BC[5], -10.0f);
    Script_scenCreateTask(script_2FEF, 3, SHEN_NAME('KBK0'), SHEN_NAME('KBK1'), 30);
  } else {
    Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_15);
    Script_motmSetBlendFrames(task_BC[5], MOTM_BLEND_FRAMES_15);
    script_3932(29, -1);
    script_3C93(arg0, 4, 0, 15, 5);
    script_3C93(arg0, 2, 0, 16, 16);
    SCNF_WaitFrames_0000(9);
    while (task_144 == 0) {
      script_08F4(arg1, 5, 5, -0x70000000);
      if (task_144 == 0) {
        script_3C93(arg0, 4, 0, 5, 5);
        script_3C93(arg0, 2, 0, 12, 12);
        script_0CFF(task_BC[5], arg0, -0.74f, -0.84f, -70.0f);
        SCNF_WaitFrames_0000(6);
      } else {
        script_36EC(24, 30);
      }
    }
  }
  Script_PlayCharaMotion(task_BC[6], MN_EXB_YKI_AKI_STANDBY3, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  SCNF_WaitFrames_0000(30);
  SCNF_WaitFrames_0000(20);
  if (local_FC) {
    Script_charaPosRotUtils(SHEN_NAME('IKA0'), *&data_928C, &data_96FC, 0, 0);
    Script_charaPosRotUtils(SHEN_NAME('IKA0'), *(&data_928C + 7), &data_96FC, 0, 0);
    Script_charaPosRotUtils(SHEN_NAME('IKA0'), *(&data_928C + 9), &data_96FC, 0, 0);
    Script_charaPosRotUtils(SHEN_NAME('IKA0'), *(&data_928C + 11), &data_96FC, 0, 0);
    Script_charaPosRotUtils(SHEN_NAME('IKA0'), *(&data_928C + 15), &data_96FC, 0, 0);
    Script_charaPosRotUtils(SHEN_NAME('IKA0'), *(&data_928C + 12), &data_96FC, 0, 0);
    Script_charaPosRotUtils(SHEN_NAME('IKA0'), *(&data_928C + 10), &data_96FC, 0, 0);
  }
  Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_7);
  script_0CFF(task_BC[6], arg0, 0.4f, -2.3f, 180.0f);
  script_3932(30, -1);
  script_36B2(8058, SHEN_NAME('DUM1'), SHEN_NAME('DUM1'), -1);
  script_3D65(SHEN_NAME('BIN_'), 0, 0, -1);
  script_3D65(arg0, 1, 1, -1);
  script_3D65(task_BC[1], 0, 0, -1);
  script_3D65(task_BC[2], 0, 0, -1);
  script_3D65(task_BC[3], 0, 0, -1);
  script_3D65(task_BC[4], 1, 0, -1);
  script_3D65(task_BC[5], 1, 0, -1);
  script_3D65(task_BC[6], 1, 1, -1);
  script_3D65(task_BC[7], 1, 0, -1);
  script_08F4(arg1, 9, 6, 0x10000000);
  if (task_144 == 0) {
    script_3932(32, -1);
    script_3C93(arg0, 4, 0, 8, 5);
    script_3C93(arg0, 2, 0, 16, 16);
    script_36B2(8050, SHEN_NAME('DUM1'), SHEN_NAME('DUM1'), -1);
    script_08F4(arg1, 10, 6, 0x10000000);
    if (task_144 == 0) {
      script_3932(47, -1);
      script_3C93(arg0, 4, 0, 8, 5);
      script_3C93(arg0, 2, 0, 16, 16);
      task_150 = 10;
      SCNF_WaitFrames_0000(1);
    } else {
      script_3932(46, -1);
      script_3C93(task_BC[6], 4, 0, 8, 30);
      script_36B2(8057, SHEN_NAME('DUM1'), SHEN_NAME('DUM1'), -1);
      script_36EC(12, -1);
    }
  } else {
    script_3932(31, -1);
    script_3C93(task_BC[6], 4, 0, 8, 30);
    script_36B2(8054, SHEN_NAME('DUM1'), SHEN_NAME('DUM1'), -1);
    script_36B2(8051, SHEN_NAME('DUM1'), SHEN_NAME('DUM1'), 19);
    script_36EC(12, -1);
  }
  SCNF_WaitFrames_0000(30);
  Script_PlayCharaMotion(task_BC[7], MN_EXB_YKI_AKI_STANDBY3, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  SCNF_WaitFrames_0000(20);
  script_0DC1(task_BC[7], arg0, -0x4023D70A, -0x41333333, -0x3E280000, 12);
  script_0F94(arg0, -0x3D900000, 12);
  script_3932(33, -1);
  script_36EC(13, -1);
  script_36B2(8054, SHEN_NAME('DUM1'), SHEN_NAME('DUM1'), 12);
  script_3D65(SHEN_NAME('BIN_'), 0, 0, 12);
  script_3D65(arg0, 1, 1, 12);
  script_3D65(task_BC[1], 0, 0, 12);
  script_3D65(task_BC[2], 0, 0, 12);
  script_3D65(task_BC[3], 0, 0, 12);
  script_3D65(task_BC[4], 0, 0, 12);
  script_3D65(task_BC[5], 0, 0, 12);
  script_3D65(task_BC[6], 1, 1, 12);
  script_3D65(task_BC[7], 1, 1, 12);
  script_08F4(arg1, 13, 7, 0x10000000);
  if (task_144 == 0) {
    script_3932(35, -1);
    script_3C93(arg0, 4, 0, 6, 5);
    script_3C93(arg0, 2, 0, 5, 12);
  } else {
    script_3932(34, -1);
  }
  script_3932(36, -1);
  script_08F4(arg1, 14, 7, 0x10000000);
  if (task_144 == 0) {
    script_3932(38, -1);
    script_3C93(arg0, 4, 0, 3, 5);
    script_3C93(arg0, 2, 0, 10, 10);
  } else {
    script_3932(37, -1);
  }
  script_0DC1(task_BC[7], arg0, -0x40B33333, -0x40E66666, 0x43340000, 29);
  script_3932(39, -1);
  script_08F4(arg1, 15, 7, 0x10000000);
  if (task_144 == 0) {
    script_3932(41, -1);
    script_3C93(arg0, 4, 0, 8, 5);
    script_3C93(arg0, 2, 0, 16, 16);
    script_36B2(8055, SHEN_NAME('DUM1'), SHEN_NAME('DUM1'), -1);
    script_08F4(arg1, 16, 7, 0x10000000);
    if (task_144 == 0) {
      task_150 = 10;
      SCNF_WaitFrames_0000(1);
    } else {
      script_36EC(14, 20);
      script_3C93(task_BC[7], 4, 0, 20, 5);
      Script_scenCreateTask(script_2B73, 3, SHEN_NAME('PPD0'), SHEN_NAME('PIP0'), 40);
    }
  } else if (Script_wazaUtils(MOVE_UI_CMD_GET_MOVE_SKILL_VALUE, 1, 14, 1)) {
    script_64A7(arg0, arg1);
  } else {
    script_6298(arg0, arg1);
  }
  SCNF_WaitFrames_0000(80);
  task_150 = -1;
  SCNF_WaitFrames_0000(1);
  end:;
}

static void script_6298(int32_t arg0, int32_t arg1) {
  script_3932(40, -1);
  script_36EC(14, 90);
  script_36B2(8052, SHEN_NAME('DUM1'), SHEN_NAME('DUM1'), -1);
  Script_scenCreateTask(script_039D, 2, 161451, 29);
  Script_scenCreateTask(script_039D, 2, 0x2876AB, 109);
  SCNF_WaitFrames_0000(33);
  Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_IMMEDIATE);
  Script_motmSetBlendFrames(task_BC[7], MOTM_BLEND_FRAMES_IMMEDIATE);
  script_3DB7(arg0, arg1, 64119, 0x41A00000, 0x3F800000, 0, -1);
  script_3DB7(task_BC[7], 7, 64122, 0x41A00000, 0x3F800000, 0, -1);
  script_36B2(8050, SHEN_NAME('DUM1'), SHEN_NAME('DUM1'), -1);
  Script_scenCreateTask(script_039D, 2, 161451, 9);
  Script_scenCreateTask(script_0591, 4, arg0, 4, 2, 10);
  script_0DC1(arg0, 0x304D5544, 0x41400000, 0x4104CCCD, 0x433E8000, -1);
  script_0DC1(task_BC[7], 0x304D5544, 0x414BAE14, 0x4108F5C3, 0x41280000, -1);
  SCNF_WaitFrames_0000(12);
  script_3DB7(arg0, arg1, 64119, 0x41800000, 0x3F800000, 0, -1);
  script_3DB7(task_BC[7], 7, 64122, 0x41800000, 0x3F800000, 0, -1);
  script_36B2(8055, SHEN_NAME('DUM1'), SHEN_NAME('DUM1'), -1);
  Script_scenCreateTask(script_039D, 2, 161451, 13);
  Script_scenCreateTask(script_0591, 4, arg0, 4, 6, 5);
  Script_scenCreateTask(script_2B73, 3, SHEN_NAME('PPD0'), SHEN_NAME('PIP0'), 65);
  script_0DC1(arg0, 0x304D5544, 0x41400000, 0x4104CCCD, 0x433E8000, -1);
  script_0DC1(task_BC[7], 0x304D5544, 0x414BAE14, 0x4108F5C3, 0x41280000, -1);
  return;
}

static void script_64A7(int32_t arg0, int32_t arg1) {
  SCNF_WaitFrames_0000(1);
  script_3932(40, -1);
  script_36EC(14, 90);
  Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_IMMEDIATE);
  Script_motmSetBlendFrames(task_BC[7], MOTM_BLEND_FRAMES_IMMEDIATE);
  Script_CharHandlers(arg0, 2, 1, 16);
  task_B8 = task_B8 | 0x10000000;
  script_3DB7(arg0, arg1, 64132, 0, 0x3F800000, 0, 5);
  script_3DB7(task_BC[7], 7, 64133, 0x40A00000, 0x3F800000, 0, -1);
  script_36B2(8072, SHEN_NAME('DUM0'), SHEN_NAME('DUM0'), -1);
  script_0DC1(task_BC[7], arg0, -0x42333333, -0x406CCCCD, 0x42BE0000, -1);
  script_0DC1(arg0, 0x304D5544, 0x41380000, 0x41000000, 0x43848000, -1);
  Script_scenCreateTask(script_039D, 2, 620203, 5);
  Script_scenCreateTask(script_039D, 2, 816811, 10);
  Script_scenCreateTask(script_039D, 2, 620203, 12);
  Script_scenCreateTask(script_039D, 2, 0x1776AB, 18);
  Script_scenCreateTask(script_039D, 2, 161451, 18);
  Script_scenCreateTask(script_0591, 4, arg0, 1, -1, 20);
  SCNF_WaitFrames_0000(23);
  script_3DB7(arg0, arg1, 64132, 0x40C00000, 0x3F800000, 0, -1);
  script_3DB7(task_BC[7], 7, 64133, 0x41800000, 0x3F800000, 0, -1);
  script_36B2(8073, SHEN_NAME('DUM0'), SHEN_NAME('DUM0'), -1);
  script_0DC1(arg0, 0x304D5544, 0x41380000, 0x41000000, 0x43848000, -1);
  script_0DC1(task_BC[7], arg0, -0x42333333, 0, 0x42BE0000, -1);
  Script_scenCreateTask(script_039D, 2, 161451, 7);
  Script_scenCreateTask(script_039D, 2, 0x1776AB, 7);
  Script_scenCreateTask(script_0591, 4, arg0, 1, -1, 4);
  SCNF_WaitFrames_0000(13);
  script_3DB7(arg0, arg1, 64132, 0, 0x3F800000, 0, -1);
  script_3DB7(task_BC[7], 7, 64133, 0x41200000, 0x3F800000, 0, -1);
  script_36B2(8074, SHEN_NAME('DUM0'), SHEN_NAME('DUM0'), -1);
  script_0DC1(arg0, 0x304D5544, 0x41380000, 0x41000000, 0x43848000, -1);
  script_0DC1(task_BC[7], arg0, -0x42333333, -0x414CCCCD, 0x42BE0000, -1);
  Script_scenCreateTask(script_039D, 2, 816811, 20);
  Script_scenCreateTask(script_039D, 2, 620203, 28);
  Script_scenCreateTask(script_039D, 2, 0x1876AB, 47);
  Script_scenCreateTask(script_039D, 2, 161451, 47);
  Script_scenCreateTask(script_0591, 4, arg0, 1, 4, 50);
  SCNF_WaitFrames_0000(1);
  Script_scenCreateTask(script_3100, 1, 44);
  Script_SetCharaMotionPlaybackRate(arg0, 0.25f);
  Script_SetCharaMotionPlaybackRate(task_BC[7], 0.25f);
  Script_executeAicaCommand(131232, 0, 0);
  SCNF_WaitFrames_0000(44);
  Script_executeAicaCommand(222376, 0, 0);
  Script_executeAicaCommand(1184, 2, 127);
  Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
  Script_SetCharaMotionPlaybackRate(task_BC[7], 1.0f);
  Script_scenCreateTask(script_2C83, 3, SHEN_NAME('PPD0'), SHEN_NAME('PIP0'), 12);
  Script_scenCreateTask(script_039D, 2, 132265, 22);
  SCNF_WaitFrames_0000(15);
  Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_15);
  Script_motmToggleFlags(arg0, 1, 0x80);
  Script_PlayCharaMotion(arg0, MN_AKI_AKI_BATTLE_KAMAE_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_SetCharaMotionPlaybackRate(arg0, 0.4f);
  Script_CharHandlers(arg0, 2, 0, 16);
  Script_GetDataFromFightProgram(arg0, 0);
  SCNF_WaitFrames_0000(30);
  Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
  Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_7);
  return;
}

static void script_68DE(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  Script_lightSetSlotEnabled(1, 0);
  Script_lightSetSlotEnabled(3, 0);
  Script_MoveObjectToObject(CHARA_AKIR, 0x304D5544);
  Script_MoveObjectToObject(CHARA_JONZ, 0x304D5544);
  Script_MoveObjectToObject(CHARA_HARY, 0x304D5544);
  Script_MoveObjectToObject(CHARA_TONY, 0x304D5544);
  Script_MoveObjectToObject(CHARA_SMTH, 0x304D5544);
  script_0CFF(SHEN_NAME('DUM0'), -1, 27.0f, 78.0f, 45.0f);
  script_0CFF(arg0, 0x304D5544, 0.75f, 11.7f, 285.0f);
  script_0CFF(task_BC[1], 0x304D5544, 10.3f, 6.3f, 205.0f);
  script_0CFF(task_BC[2], 0x304D5544, 11.6f, 9.9f, 10.0f);
  script_0CFF(task_BC[3], 0x304D5544, 12.7f, 6.0f, 225.0f);
  script_0CFF(task_BC[4], 0x304D5544, 13.1f, 9.8f, 350.0f);
  script_0CFF(task_BC[5], 0x304D5544, 13.9f, 9.2f, 5.0f);
  script_0CFF(task_BC[6], 0x304D5544, 4.22f, 11.2f, 300.0f);
  script_0CFF(task_BC[7], 0x304D5544, 14.62f, 7.6f, 95.0f);
  script_32D5(arg1, 0.1f, &data_93DC);
  script_32D5(6, 0.1f, &data_9408);
  script_32D5(1, 0.1f, &data_9434);
  Script_PlayCharaMotion(task_BC[1], 20490, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_PlayCharaMotion(task_BC[2], 20492, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_PlayCharaMotion(task_BC[3], 20503, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_PlayCharaMotion(task_BC[4], 20503, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_PlayCharaMotion(task_BC[5], 20490, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_PlayCharaMotion(task_BC[6], MN_AKI_AKI_BATTLE_KAMAE_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_PlayCharaMotion(task_BC[7], 20508, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_charaToggleMotEnabled(arg0, 1);
  Script_charaToggleMotEnabled(task_BC[1], 1);
  Script_charaToggleMotEnabled(task_BC[2], 1);
  Script_charaToggleMotEnabled(task_BC[3], 1);
  Script_charaToggleMotEnabled(task_BC[4], 1);
  Script_charaToggleMotEnabled(task_BC[5], 1);
  Script_charaToggleMotEnabled(task_BC[6], 1);
  Script_charaToggleMotEnabled(task_BC[7], 1);
  Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_15);
  Script_motmSetBlendFrames(task_BC[1], MOTM_BLEND_FRAMES_15);
  Script_motmSetBlendFrames(task_BC[2], MOTM_BLEND_FRAMES_15);
  Script_motmSetBlendFrames(task_BC[3], MOTM_BLEND_FRAMES_15);
  Script_motmSetBlendFrames(task_BC[4], MOTM_BLEND_FRAMES_15);
  Script_motmSetBlendFrames(task_BC[5], MOTM_BLEND_FRAMES_15);
  Script_motmSetBlendFrames(task_BC[6], MOTM_BLEND_FRAMES_15);
  Script_motmSetBlendFrames(task_BC[7], MOTM_BLEND_FRAMES_15);
  script_3932(0, -1);
  script_3C93(arg0, 2, 0, 5, -1);
  script_3C93(task_BC[6], 1, 0, 5, -1);
  script_3C93(task_BC[7], 1, 0, 5, -1);
  script_36B2(8060, SHEN_NAME('DUM0'), SHEN_NAME('DUM0'), -1);
  script_36B2(8064, SHEN_NAME('DUM0'), SHEN_NAME('DUM0'), 184);
  script_36B2(8067, SHEN_NAME('DUM0'), SHEN_NAME('DUM0'), 220);
  script_36B2(8069, SHEN_NAME('DUM0'), SHEN_NAME('DUM0'), 300);
  script_36B2(8061, SHEN_NAME('DUM0'), SHEN_NAME('DUM0'), 400);
  script_36B2(8063, SHEN_NAME('DUM0'), SHEN_NAME('DUM0'), 450);
  script_3D65(SHEN_NAME('BIN_'), 1, 0, -1);
  script_3D65(arg0, 0, 0, -1);
  script_3D65(task_BC[1], 1, 0, -1);
  script_3D65(task_BC[2], 1, 1, -1);
  script_3D65(task_BC[3], 0, 0, -1);
  script_3D65(task_BC[4], 1, 0, -1);
  script_3D65(task_BC[5], 1, 0, -1);
  script_3D65(task_BC[6], 1, 1, -1);
  script_3D65(task_BC[7], 1, 0, -1);
  script_3D65(SHEN_NAME('BIN_'), 0, 0, 184);
  script_3D65(arg0, 0, 0, 184);
  script_3D65(task_BC[1], 0, 0, 184);
  script_3D65(task_BC[2], 0, 0, 184);
  script_3D65(task_BC[3], 0, 0, 184);
  script_3D65(task_BC[4], 1, 0, 184);
  script_3D65(task_BC[5], 1, 1, 184);
  script_3D65(task_BC[6], 1, 1, 184);
  script_3D65(task_BC[7], 1, 1, 184);
  script_3D65(SHEN_NAME('BIN_'), 1, 0, 300);
  script_3D65(arg0, 0, 0, 300);
  script_3D65(task_BC[1], 1, 0, 300);
  script_3D65(task_BC[2], 1, 1, 300);
  script_3D65(task_BC[3], 1, 0, 300);
  script_3D65(task_BC[4], 1, 0, 300);
  script_3D65(task_BC[5], 1, 0, 300);
  script_3D65(task_BC[6], 1, 0, 300);
  script_3D65(task_BC[7], 1, 0, 300);
  script_3D65(SHEN_NAME('BIN_'), 1, 0, 450);
  script_3D65(arg0, 1, 0, 450);
  script_3D65(task_BC[1], 1, 0, 450);
  script_3D65(task_BC[2], 1, 0, 450);
  script_3D65(task_BC[3], 1, 0, 450);
  script_3D65(task_BC[4], 1, 0, 450);
  script_3D65(task_BC[5], 0, 0, 450);
  script_3D65(task_BC[6], 1, 0, 450);
  script_3D65(task_BC[7], 1, 0, 450);
  Script_filmCheckOrSetTransitionUtil(0, 255, 0, 0, 0, 0, 0, 0, 0);
  local_FC = 0;
  while (local_FC < 95) {
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  Script_filmCheckOrSetTransitionUtil(60, 255, 0, 0, 0, 0, 0, 0, 0);
  script_3370(6, 0.15f);
  SCNF_WaitFrames_0000(1);
  Script_SetCharaMotionPlaybackRate(task_BC[6], 0.75f);
  SCNF_WaitFrames_0000(57);
  script_34F5(6);
  SCNF_WaitFrames_0000(1);
  Script_SetCharaMotionPlaybackRate(task_BC[6], 0.75f);
  script_36EC(0, 30);
  script_36EC(1, 60);
  script_36EC(2, 100);
  script_3DB7(task_BC[1], 1, 20482, 0, 0x3F800000, 0, 219);
  script_3DB7(task_BC[2], 2, 20491, 0, 0x3F800000, 0, 280);
  script_3DB7(task_BC[2], 2, 20495, 0x42700000, 0x3F800000, 1, 290);
  script_3DB7(task_BC[3], 3, 20504, 0, 0x3F800000, 0, 50);
  script_3DB7(task_BC[3], 3, 20495, 0, 0x3F800000, 0, 320);
  script_3DB7(task_BC[4], 4, 20504, 0, 0x3F800000, 1, 30);
  script_3DB7(task_BC[4], 4, 20495, 0, 0x3F800000, 1, 210);
  script_3DB7(task_BC[7], 7, 20511, 0, 0x3F800000, 0, -1);
  script_3DB7(task_BC[7], 7, 20510, 0, 0x3F800000, 0, 29);
  script_3DB7(task_BC[6], 6, 772, 0, 0x3F800000, 0, 7);
  script_3DB7(task_BC[6], 6, 771, 0, 0x3F800000, 0, 41);
  script_3DB7(task_BC[6], 6, 64130, 0, 0x3F800000, 0, 101);
  script_3DB7(task_BC[6], 6, 64131, 0, 0x3F800000, 0, 135);
  script_3DB7(task_BC[6], 6, 20481, 0, 0x3F800000, 0, 250);
  script_0F94(task_BC[6], 0x43340000, 250);
  SCNF_WaitFrames_0000(210);
  script_34BC(arg1, 0x3D343958, 10);
  script_35D8(arg1, 130);
  script_34BC(1, 0x3D343958, 75);
  script_35D8(1, 190);
  script_3DB7(arg0, arg1, 88, 0, 0x3F800000, 0, 130);
  SCNF_WaitFrames_0000(190);
  Script_lightSetSlotEnabled(1, 1);
  return;
}

static void script_72F6(int32_t arg0) {
  Script_charaToggleMotEnabled(arg0, 1);
  Script_charaToggleMotEnabled(task_BC[1], 1);
  Script_charaToggleMotEnabled(task_BC[2], 1);
  Script_charaToggleMotEnabled(task_BC[3], 1);
  Script_charaToggleMotEnabled(task_BC[4], 1);
  Script_charaToggleMotEnabled(task_BC[5], 1);
  Script_charaToggleMotEnabled(task_BC[6], 1);
  Script_charaToggleMotEnabled(task_BC[7], 1);
  Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_15);
  Script_motmSetBlendFrames(task_BC[1], MOTM_BLEND_FRAMES_15);
  Script_motmSetBlendFrames(task_BC[2], MOTM_BLEND_FRAMES_15);
  Script_motmSetBlendFrames(task_BC[3], MOTM_BLEND_FRAMES_15);
  Script_motmSetBlendFrames(task_BC[4], MOTM_BLEND_FRAMES_15);
  Script_motmSetBlendFrames(task_BC[5], MOTM_BLEND_FRAMES_15);
  Script_motmSetBlendFrames(task_BC[6], MOTM_BLEND_FRAMES_IMMEDIATE);
  Script_motmSetBlendFrames(task_BC[7], MOTM_BLEND_FRAMES_15);
  script_3932(1, -1);
  script_36B2(8066, SHEN_NAME('DUM0'), SHEN_NAME('DUM0'), 70);
  script_3D65(SHEN_NAME('BIN_'), 1, 0, 70);
  script_3D65(arg0, 1, 1, 70);
  script_3D65(task_BC[1], 1, 0, 70);
  script_3D65(task_BC[2], 1, 0, 70);
  script_3D65(task_BC[3], 0, 0, 70);
  script_3D65(task_BC[4], 0, 0, 70);
  script_3D65(task_BC[5], 0, 0, 70);
  script_3D65(task_BC[6], 1, 1, 70);
  script_3D65(task_BC[7], 0, 0, 70);
  SCNF_WaitFrames_0000(70);
  script_0CFF(arg0, 0x304D5544, 12.0f, 8.4f, 95.0f);
  script_0CFF(task_BC[6], arg0, 0.75f, -1.6f, 0.0f);
  Script_PlayCharaMotion(task_BC[6], 64126, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_PlayCharaMotion(arg0, 64125, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  SCNF_WaitFrames_0000(104);
  return;
}

__attribute__((scn_string_bindings(0, data_9708, 1, data_970D, 2, data_9712, 7, data_9747)))
static void script_7525(int32_t arg0, int32_t arg1) {
  Script_LoadSND(0, 5, "FREE", 0);
  Script_LoadSND(0, 4, "FREE", 0);
  Script_LoadSND(0, 2, "FREE", 0);
  Script_LoadSND(0, 4, "a1_qrenq.snd", 0);
  Script_LoadSND(0, 2, "bgm035.snd", 0);
  Script_SetupLightFromTable(0);
  Script_lightSetSlotEnabled(2, 0);
  Script_PREP_Related(-1, 1);
  Script_ResetHandMotion(CHARA_AKIR, 1, 0, 0);
  Script_ResetHandMotion(CHARA_AKIR, 1, 1, 0);
  Script_motmToggleFlags(CHARA_AKIR, 2, 0x40);
  Script_motmToggleFlags(CHARA_JONZ, 2, 0x40);
  Script_motmToggleFlags(CHARA_RBRT, 2, 0x40);
  Script_motmToggleFlags(CHARA_TONY, 2, 0x40);
  Script_motmToggleFlags(CHARA_SMTH, 2, 0x40);
  Script_charaToggleMotEnabled(CHARA_AKIR, 0);
  Script_charaToggleMotEnabled(CHARA_JONZ, 0);
  Script_charaToggleMotEnabled(CHARA_RBRT, 0);
  Script_charaToggleMotEnabled(CHARA_TONY, 0);
  Script_charaToggleMotEnabled(CHARA_SMTH, 0);
  Script_MoveObjectToObject(CHARA_AKIR, -1);
  Script_MoveObjectToObject(CHARA_JONZ, -1);
  Script_MoveObjectToObject(CHARA_RBRT, -1);
  Script_MoveObjectToObject(CHARA_TONY, -1);
  Script_MoveObjectToObject(CHARA_SMTH, -1);
  script_3C93(arg0, 2, 0, 5, -1);
  script_3C93(task_BC[6], 1, 0, 5, -1);
  script_3C93(task_BC[7], 2, 0, 5, -1);
  script_3D65(SHEN_NAME('BIN_'), 0, 0, -1);
  script_3D65(arg0, 1, 1, -1);
  script_3D65(task_BC[1], 0, 0, -1);
  script_3D65(task_BC[2], 0, 0, -1);
  script_3D65(task_BC[3], 1, 0, -1);
  script_3D65(task_BC[4], 1, 0, -1);
  script_3D65(task_BC[5], 0, 0, -1);
  script_3D65(task_BC[6], 1, 1, -1);
  script_3D65(task_BC[7], 0, 0, -1);
  Script_executeAicaCommand(13224, 0, 0);
  Script_executeAicaCommand(1184, 2, 120);
  script_1403(1);
  Script_executeAicaCommand(2720, 2, 30);
  Script_motmToggleFlags(CHARA_AKIR, 1, 0x40);
  Script_motmToggleFlags(CHARA_JONZ, 1, 0x40);
  Script_motmToggleFlags(CHARA_RBRT, 1, 0x40);
  Script_motmToggleFlags(CHARA_TONY, 1, 0x40);
  Script_motmToggleFlags(CHARA_SMTH, 1, 0x40);
  Script_charaToggleMotEnabled(CHARA_AKIR, 1);
  Script_charaToggleMotEnabled(CHARA_JONZ, 1);
  Script_charaToggleMotEnabled(CHARA_RBRT, 1);
  Script_charaToggleMotEnabled(CHARA_TONY, 1);
  Script_charaToggleMotEnabled(CHARA_SMTH, 1);
  Script_MoveObjectToObject(CHARA_AKIR, 0x304D5544);
  Script_MoveObjectToObject(CHARA_JONZ, 0x304D5544);
  Script_MoveObjectToObject(CHARA_RBRT, 0x304D5544);
  Script_MoveObjectToObject(CHARA_TONY, 0x304D5544);
  Script_MoveObjectToObject(CHARA_SMTH, 0x304D5544);
  Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
  Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
  script_3DB7(*task_BC, 0, 0xFFFF, 0, 0x3F800000, 0, -1);
  script_3DB7(task_BC[3], 3, 0xFFFF, 0, 0x3F800000, 0, -1);
  script_3DB7(task_BC[4], 4, 0xFFFF, 0, 0x3F800000, 0, -1);
  script_3DB7(task_BC[6], 6, 0xFFFF, 0, 0x3F800000, 0, -1);
  script_3DB7(task_BC[7], 7, 0xFFFF, 0, 0x3F800000, 0, -1);
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_LAN_DI_STILL_IN_HARBOR) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_MFSY_HARBOR_FOREIGNERS_INQUIRY) == 0)) {
  } else {
    Script_LoadSND(0, 2, "bgm067.snd", 0);
    Script_LoadSND(0, 4, "e1minqte.snd", 0);
    Script_LoadVoiceFile(0, "019RN");
    Script_executeAicaCommand(91304, 0, 0);
    Script_executeAicaCommand(1184, 2, 127);
    Script_PREP_Related(-1, 0);
    Script_SetupLightFromTable(1);
    Script_lightSetSlotEnabled(16, 0);
    Script_lightSetSlotEnabled(2, 0);
    Script_lightSetSlotEnabled(4, 0);
    Script_lightSetSlotEnabled(6, 0);
  }
  return;
}

void script_7981(void) {
  Script_filmCheckOrSetTransitionUtil(0, 255, 0, 0, 0, 0, 0, 0, 0);
  SCNF_WaitFrames_0000(5);
  script_79C9(0);
  task_0 = 0;
  for (;;) {
    task_0 = task_0 + 1;
    SCNF_WaitFrames_0000(1);
  }
  return;
}

__attribute__((scn_string_bindings(0, data_974D, 1, data_9753, 2, data_9759, 4, data_976C)))
static void script_79C9(int32_t arg0) {
  task_8 = 0;
  Script_LoadVoiceFile(1, "E1022");
  Script_LoadVoiceFile(1, "019RN");
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 2, "MPK00");
  Script_AuthorUtils(0, 0, "seqdata1.bin", "MPK00");
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
  Script_scenCreateTask(script_360D, 0);
  Script_scenCreateTask(script_367C, 0);
  Script_scenCreateTask(script_8075, 2, SHEN_NAME('DOR0'), 0);
  Script_scenCreateTask(script_8075, 2, SHEN_NAME('IKA0'), 0);
  Script_scenCreateTask(script_8075, 2, SHEN_NAME('IKA1'), 0);
  Script_scenCreateTask(script_8075, 2, SHEN_NAME('PIP0'), 0);
  Script_scenCreateTask(script_8075, 2, SHEN_NAME('PPD0'), 0);
  Script_scenCreateTask(script_8075, 2, SHEN_NAME('DRM0'), 0);
  Script_scenCreateTask(script_8075, 2, SHEN_NAME('DRM1'), 0);
  Script_scenCreateTask(script_8075, 2, SHEN_NAME('DRM2'), 0);
  Script_scenCreateTask(script_8075, 2, SHEN_NAME('DRM3'), 0);
  Script_scenCreateTask(script_8075, 2, SHEN_NAME('DRD0'), 0);
  Script_scenCreateTask(script_8075, 2, SHEN_NAME('KBK0'), 0);
  Script_scenCreateTask(script_8075, 2, SHEN_NAME('KBK1'), 0);
  Script_scenCreateTask(script_8075, 2, SHEN_NAME('KBK2'), 0);
  Script_scenCreateTask(script_8075, 2, SHEN_NAME('KBK3'), 0);
  Script_scenCreateTask(script_8075, 2, SHEN_NAME('KBK4'), 0);
  Script_scenCreateTask(script_8075, 2, SHEN_NAME('KBK5'), 0);
  Script_scenCreateTask(script_80D6, 3, SHEN_NAME('BIN_'), 0, CHARA_DICK);
  Script_scenCreateTask(script_7E69, 4, CHARA_SERA, 1, 0, 0);
  Script_scenCreateTask(script_7E69, 4, CHARA_DICK, 2, 0, 0);
  Script_scenCreateTask(script_7E69, 4, CHARA_JONZ, 3, 0, 0);
  Script_scenCreateTask(script_7E69, 4, CHARA_RBRT, 4, 0, 0);
  Script_scenCreateTask(script_7E69, 4, CHARA_HARY, 5, 0, 0);
  Script_scenCreateTask(script_7F4D, 4, CHARA_TONY, 6, 0, 0);
  Script_scenCreateTask(script_7F4D, 4, CHARA_SMTH, 7, 0, 0);
  Script_scenCreateTask(script_7CB7, 2, CHARA_AKIR, 0);
  task_4 = Script_scenCreateTask(script_3F24, 2, CHARA_AKIR, 0);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 20.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 20.0f);
  Script_SetCameraFlags(0);
  Script_camSetRotationStepSize(0);
  Script_SetActionSelectorHUDElementVisibility(1);
  return;
}

__attribute__((scn_string_bindings(0, data_9772)))
static void script_7C5E(void) {
  task_8 = 1;
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 20.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 20.0f);
  Script_SetActionSelectorHUDElementVisibility(0);
  Script_SetCameraFlags(1);
  Script_camSetRotationStepSize(3);
  Script_AuthorUtils(AUTH_FREE, 0);
  Script_LoadVoiceFile(0, "E1022");
  Script_srchControl((void *)task_4);
  SCNF_WaitFrames_0000(1);
  return;
}

static int32_t script_7CB7(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  int32_t local_F8;
  local_FC = Script_scenCreateTask(script_0AFA, 3, arg0, arg1, 0);
  local_F8 = Script_scenCreateTask(script_0666, 6, arg0, arg1, 0, 2, &data_8F28, script_1643);
  Script_facemotUtils(arg0, 0, 0);
  Script_facemotUtils(arg0, 1, 0);
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
  Script_EVT_SetPosition(SHEN_NAME('DUM1'), POS_XYZ, &data_9778);
  Script_EVT_SetRotation(SHEN_NAME('DUM1'), 0x38000000, &data_9784, 0);
  Script_EVT_SetPosition(SHEN_NAME('DUMS'), POS_XYZ, &data_9778);
  Script_EVT_SetRotation(SHEN_NAME('DUMS'), 0x38000000, &data_9784, 0);
  Script_SetShadowFlags(arg0, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_SetShadowFlags(arg0, SCRIPT_SHADOW_UPDATE_LIGHT_MASK, 5);
  Script_scenToggleActiveFor(arg0, 0);
  Script_SetCharaFlags(arg0, 4);
  Script_charaDisplay(arg0, CDM_Show);
  for (;;) {
    if (task_8 == 1) {
      Script_faceUpdateFor(arg0, 1, 0);
      Script_faceUpdateFor(arg0, 0, 0);
      Script_srchControl((void *)task_11C[arg1]);
      Script_srchControl((void *)local_F8);
      Script_srchControl((void *)local_FC);
      Script_srchControl(0);
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_7E69(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  int32_t local_F8;
  local_FC = Script_scenCreateTask(script_0AFA, 3, arg0, arg1, 0);
  local_F8 = Script_scenCreateTask(script_0666, 6, arg0, arg1, arg2, arg3, &data_8F28, script_1643);
  Script_ccowUtils(arg0, 2);
  Script_charaToggleMotEnabled(arg0, 1);
  Script_motmGetCharaFlags(arg0, 0, 0);
  Script_motmToggleFlags(arg0, 1, 0x40);
  Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_7);
  Script_SetFightProgramStateBytes(arg0, 2, 1);
  Script_SetCPAMFlags(arg0, 0);
  Script_MoveObjectToObject(arg0, 0x304D5544);
  Script_SetShadowFlags(arg0, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_SetShadowFlags(arg0, SCRIPT_SHADOW_UPDATE_LIGHT_MASK, 5);
  Script_scenToggleActiveFor(arg0, 0);
  Script_SetCharaFlags(arg0, 4);
  for (;;) {
    if (task_8 == 1) {
      Script_srchControl((void *)task_11C[arg1]);
      Script_srchControl((void *)local_F8);
      Script_srchControl((void *)local_FC);
      Script_srchControl(0);
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_7F4D(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  int32_t local_F8;
  local_FC = Script_scenCreateTask(script_0AFA, 3, arg0, arg1, 0);
  local_F8 = Script_scenCreateTask(script_0666, 6, arg0, arg1, arg2, arg3, &data_8F28, script_1643);
  Script_facemotUtils(arg0, 0, 0);
  Script_FaceEx(arg0, 1, 0, 4);
  Script_FACE(arg0, 0, 0, 4);
  Script_FACE_LCG(arg0, 2, 4, 0);
  Script_FaceLookAtTargetUtils(arg0, 0);
  Script_ccowUtils(arg0, 2);
  Script_charaToggleMotEnabled(arg0, 1);
  Script_motmGetCharaFlags(arg0, 0, 0);
  Script_motmToggleFlags(arg0, 1, 0x40);
  Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_7);
  Script_SetFightProgramStateBytes(arg0, 2, 1);
  Script_SetCPAMFlags(arg0, 0);
  Script_MoveObjectToObject(arg0, 0x304D5544);
  Script_SetShadowFlags(arg0, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_SetShadowFlags(arg0, SCRIPT_SHADOW_UPDATE_LIGHT_MASK, 5);
  Script_scenToggleActiveFor(arg0, 0);
  Script_SetCharaFlags(arg0, 4);
  for (;;) {
    if (task_8 == 1) {
      Script_faceUpdateFor(arg0, 0, 0);
      Script_srchControl((void *)task_11C[arg1]);
      Script_srchControl((void *)local_F8);
      Script_srchControl((void *)local_FC);
      Script_srchControl(0);
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_8075(int32_t arg0, int32_t arg1) {
  Script_ccowUtils(arg0, 2);
  Script_MoveObjectToObject(arg0, 0x304D5544);
  Script_SetShadowFlags(arg0, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_SetShadowFlags(arg0, SCRIPT_SHADOW_UPDATE_LIGHT_MASK, 5);
  Script_scenToggleActiveFor(arg0, 0);
  Script_SetCharaFlags(arg0, 0);
  for (;;) {
    if (task_8 == 1) {
      Script_charaDisplay(arg0, CDM_Hide);
      Script_srchControl(0);
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_80D6(int32_t arg0, int32_t arg1, int32_t arg2) {
  Vector3 local_F4;
  Vector3i local_E8;
  Script_ccowUtils(arg0, 2);
  Script_MoveObjectToObject(arg0, 0x304D5544);
  Script_SetShadowFlags(arg0, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_SetShadowFlags(arg0, SCRIPT_SHADOW_UPDATE_LIGHT_MASK, 5);
  Script_scenToggleActiveFor(arg0, 0);
  Script_SetCharaFlags(arg0, 0);
  Script_charaDisplay(arg0, CDM_Show);
  Script_PlayCharaMotion(arg2, MN_AKI_AKI_STAND_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  SCNF_WaitFrames_0000(3);
  local_F4.x = 0.145f;
  (&local_F4)->y = 0.078f;
  (&local_F4)->z = 0.066f;
  local_E8.x = 2184;
  (&local_E8)->y = 0;
  (&local_E8)->z = 61804;
  Script_fixoAttachEx(arg0, arg2, 12, &local_F4, &local_E8);
  for (;;) {
    if (task_8 == 1) {
      Script_charaDisplay(arg0, CDM_Hide);
      Script_srchControl(0);
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}
