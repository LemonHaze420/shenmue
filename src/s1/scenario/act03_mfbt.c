#include "act03_mfbt.h"

static void script_0030(void);
static void script_0039(void);
static void script_0048(int32_t arg0);
static void script_00D5(int32_t arg0);
static void script_0125(void);
static int32_t script_0153(int32_t arg0);
static void script_0181(void);
static void script_01B5(int32_t arg0);
static void script_01E9(int32_t arg0);
static void script_0207(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_0241(int32_t arg0, int32_t arg1);
static void script_0275(void);
static void script_036D(int32_t arg0, int32_t arg1);
static void script_039D(void);
static int32_t script_03E6(void);
static void script_058B(void);
static int32_t script_05D3(void);
static void script_07FA(void);
static void script_0842(void);
static void script_0874(void);
static void script_08A5(void);
static void script_08C0(int32_t arg0);
static void script_0952(int32_t arg0);
static void script_097B(int32_t arg0);
static void script_099E(int32_t arg0);
static int32_t script_09E4(void);
static void script_0A18(void);
static int32_t script_0BF2(void);
static void script_0C82(int32_t arg0);
static void script_1062(int32_t arg0);
static void script_1534(int32_t arg0);
static int32_t script_16C1(void);
static int32_t script_17B3(void);
static void script_185A(void);
static void script_194A(void);
static void script_19B1(void);
static int32_t script_1CB4(void);
static void script_1CC8(void);
static int32_t script_1DC2(void);
static int32_t script_1E46(void);
static void script_1EB9(void);
static void script_1EEE(void);
static int32_t script_2129(void);
static int32_t script_2180(void);
static int32_t script_21B4(void);
static void script_21E8(void);
static void script_2250(int32_t arg0);
static void script_2319(int32_t arg0, int32_t arg1, const int32_t *arg2, int32_t arg3);
static void script_2349(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_23EB(void);
static void script_247A(void);
static void script_254C(void);
static void script_2A81(void);
static void script_2B7B(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_2BA1(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_2BC4(int32_t arg0);
static void script_2EFA(int32_t arg0);
static void script_3187(int32_t arg0);
static void script_3533(int32_t arg0);
static void script_37C0(void);
static void script_392D(void);
static void script_393B(void);
static void script_3966(void);
static int32_t script_3983(void);
static void script_3A22(void);
static void script_3AD0(void);
static void script_3E43(void);
static void script_3F82(void);
static int32_t script_4027(void);
static int32_t script_40BF(void);
static int32_t script_40F2(void);
static int32_t script_413E(void);
static int32_t script_4158(int32_t arg0);
static int32_t script_4489(int32_t arg0);
static int32_t script_463C(int32_t arg0);
static int32_t script_49CA(void);
static int32_t script_4A30(void);
static int32_t script_4A81(void);
static int32_t script_4AF9(void);
static int32_t script_4B67(int32_t arg0);
static int32_t script_4C72(void);
static int32_t script_4CA1(void);
static int32_t script_4D57(void);
static void script_4D7E(int32_t arg0);
static int32_t script_524C(void);
static void script_527B(int32_t arg0);
static int32_t script_57A8(void);
static int32_t script_5A25(void);
static int32_t script_5EC5(void);
static void script_5EE2(int32_t arg0);
static void script_6685(int32_t arg0);
static void script_6D9E(int32_t arg0);
static void script_7630(void);
static void script_780F(void);
static void script_78C1(void);
static void script_7B1C(void);
static void script_7BCD(void);
static void script_7C7E(void);
static void script_7D3F(void);
static void script_7DD1(int32_t arg0);
static int32_t script_7E7A(void);
static void script_7EA8(int32_t arg0);
static void script_7F2F(int32_t arg0);
static void script_7FB2(void);
static void script_816C(void);
static void script_8296(void);
static void script_82C7(void);
static void script_82F8(void);
static void script_83FE(void);
static void script_8406(void);
static void script_85E8(void);
static void script_86C0(void);
static void script_8794(void);
static void script_8852(void);
static void script_885F(void);
static void script_B8B5(void);
static void script_E723(void);
static void script_E7EA(void);
void script_112BB(void);
static int32_t script_11340(int32_t arg0);
static int32_t script_116D1(void);
static void script_1194F(void);
static void script_1198E(void);
static void script_11A2C(void);
static void script_11A53(int32_t arg0);
static void script_12604(void);
static void script_12619(void);
static void script_1262A(void);
static void script_1263F(void);
static void script_12650(void);
static void script_12665(void);
static void script_12676(int32_t arg0);
static void script_12C97(void);
static void script_131BD(int32_t arg0);
static void script_13272(int32_t arg0);
static void script_13327(const char *arg0, const char *arg1);
static void script_13389(void);
static void script_1340F(void);
static void script_134D1(void);

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
static int8_t task_2C;
static uint8_t task_2D;
static uint16_t task_2E;
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
static int8_t task_6C;
static uint8_t task_6D;
static uint16_t task_6E;
static GameDateTime task_70;
static int32_t task_78;
static int32_t task_7C;
static int8_t task_80;
static uint8_t task_81;
static uint16_t task_82;
static Vector3 task_84;
static Vector3i task_90;
static int8_t task_9C;
static int8_t task_9D;
static int8_t task_9E;
static int8_t task_9F;
static int8_t task_A0;
static uint8_t task_A1;
static uint16_t task_A2;
static int32_t task_A4;
static int32_t task_A8;
static int32_t task_AC;
static int32_t task_B0;
static float task_B4;
static int32_t task_B8;
static int32_t task_BC;
static int32_t task_C0;
static int32_t task_C4;
static Vector3 task_C8;
static int32_t task_D4;
static Vector3 task_D8;
static int32_t task_E4;
static int32_t task_E8;
static int32_t task_EC;
static int8_t task_F0;
static uint8_t task_F1;
static int8_t task_F2;
static int8_t task_F3;
static int32_t task_F4;
static int32_t task_F8;
static int8_t task_FC;
static int8_t task_FD;
static int8_t task_FE;
static int8_t task_FF;
static int8_t task_100;
static uint8_t task_101;
static uint16_t task_102;
static int8_t task_104;
static uint8_t task_105;
static uint16_t task_106;
static int32_t task_108;
static Vector3i task_10C;
static GameDateTime task_118;
static int32_t task_120;
static const Vector3 data_13524 = {.x = 1.0f, .y = 1.0f, .z = 1.0f};
static const int32_t data_13530[4] = {16777215, 16777215, 16777215, 16777215};
static const float data_13540[3] = {20.0f, 800.0f, 1.0f};
static const int32_t data_1354C = -1;
static const int16_t data_13550[32] = {0, 0, 11, 0, 6, 0, 15, 0, 3, 0, 10, 0, 5, 0, 14, 0, 2, 0, 1, 0, 13, 0, 8, 0, 12, 0, 7, 0, 9, 0, 4, 0};
static const int16_t data_13590[96] = {
  23, 0, 18, 0, 0, 0, 11, 0, 6, 0, 20, 0, 15, 0, 22, 0, 17, 0, 3, 0, 10, 0, 5, 0, 14, 0, 2, 0, 21, 0, 16, 0, 1, 0, 13, 0, 8, 0, 24, 0, 19, 0, 12, 0,
  7, 0, 9, 0, 4, 0, 43, 0, 44, 0, 45, 0, 46, 0, 47, 0, 40, 0, 41, 0, 42, 0, 28, 0, 29, 0, 30, 0, 37, 0, 38, 0, 39, 0, 34, 0, 35, 0, 36, 0, 31, 0, 32,
  0, 33, 0, 25, 0, 26, 0, 27, 0
};
static const int16_t data_13650[36] = {
  165, 0, 164, 0, 163, 0, 162, 0, 161, 0, 160, 0, 159, 0, 158, 0, 157, 0, 156, 0, 155, 0, 154, 0, 153, 0, 152, 0, 169, 0, 168, 0, 167, 0, 166, 0
};
static const Vector3 data_13698 = {.x = -0.36f, .y = 0.0f, .z = -0.36f};
static const Vector3i data_136A4 = {.x = 0, .y = 0, .z = 0};
static const Vector3 data_136B0 = {.x = -0.25f, .y = 0.43f, .z = 0.0f};
static const Vector3i data_136BC = {.x = 0, .y = 0, .z = 0};
typedef struct Row_136C8 { float w0; int32_t w1; } Row_136C8;
static const Row_136C8 data_136C8[6] = {
  {0.45f, 0},
  {-0.03f, 0},
  {0.0f, 16384},
  {0.29f, 0},
  {0.0f, 0},
  {0.0f, 0}
};
static const Vector3 data_136F8 = {.x = 0.08f, .y = 0.0f, .z = -0.05f};
static const Vector3i data_13704 = {.x = 0, .y = 0, .z = -16384};
static const uint8_t pad_13710[228] = {0};
static const int32_t data_137F4[171] = {
  0, 0, -10922, 0, 0, -10558, 0, 1456, 0, 0, 0, -18022, 0, 0, -17112, 0, 0, -18386, 0, 0, 0, 0, 0, -12196, 0, 0, -18204, 0, 0, -16748, 0, 0, 0, 0, 0,
  -11832, 0, 0, -18386, 0, 0, -15109, 0, 0, 0, 0, 910, -12014, 0, -2184, -16748, 0, -2184, -10012, 0, 0, 0, 0, -364, -11650, -2912, -364, -4369, 1638,
  910, 182, 0, 0, -16019, 0, 182, -23119, 0, 546, -18932, 0, 0, -1092, 0, 0, -16384, 0, 0, -21845, 0, 0, -18750, 0, 0, -546, 0, 0, -21481, 0, 0,
  -21299, 0, 0, -18568, 0, 0, -728, 0, 910, -18750, 0, 182, -18750, 0, -4004, -18204, 0, 0, -728, 0, -364, -14017, -2912, -364, -6735, 1638, 910,
  2548, 0, 0, -18568, 0, 182, -25668, 0, 546, -21481, 0, 0, -3640, 0, 0, -18932, 0, 0, -24393, 0, 0, -21299, 0, 0, -3094, 0, 0, -24029, 0, 0, -23847,
  0, 0, -21117, 0, 0, -3276, 0, 910, -21299, 0, 182, -21299, 0, -4004, -20753, 0, 0, -3276
};
static const int32_t data_13AA0[171] = {
  0, 0, -5461, 0, -2366, -1092, 0, -182, 1092, 0, -546, -6735, 0, 0, -4004, 0, 0, -2912, 0, -182, 1274, 0, 0, -13289, 0, 0, -5279, 0, 0, -8009, 0, 0,
  1456, 0, 182, -8738, 0, 0, -6007, 0, 0, -8738, 0, 182, 1638, 0, -728, -5279, 0, -1274, -6735, 0, -910, -8738, 0, 0, 1638, 0, 0, -5461, 0, -2366,
  -1092, 0, -182, 1092, 0, -546, -13289, 0, 0, -5643, 0, 0, -8009, 0, -182, 1274, 0, 0, -11832, 0, 0, -5279, 0, 0, -8009, 0, 0, 1456, 0, 182, -8738,
  0, 0, -6007, 0, 0, -8738, 0, 182, 1638, 0, -728, -5279, 0, -1274, -6735, 0, -910, -8738, 0, 0, 1092, 0, 0, -5461, 0, -2366, -1092, 0, -182, 1092, 0,
  -546, -3640, 0, 0, -2366, 0, 0, -1456, 0, -182, 1274, 0, 0, -3822, 0, 0, -2730, 0, 0, -1456, 0, 0, 1456, 0, 182, -3822, 0, 0, -2912, 0, 0, -1638, 0,
  182, 1638, 0, -728, -3640, 0, -910, -2730, 0, -546, -1638, 0, 0, 1092
};
static const int32_t data_13D4C[342] = {
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
static const int32_t data_142A4[57] = {
  0, 0, -1456, 4004, -1456, 0, 0, -182, 910, 0, -546, -5643, 0, 0, -3276, 0, 3276, -1456, 0, 546, 1274, 0, 0, -2548, 0, 0, -910, 0, 1092, -2548, 0,
  182, 1820, 0, 182, -1274, 0, 0, -1456, 0, -1456, -1638, 0, 0, 1820, 0, -728, -546, 0, -1456, -728, 0, -3640, -364, 0, 254, 1274
};
static const int32_t data_14388[57] = {
  0, 0, -4187, 4004, -1456, -364, 0, -182, 910, 0, -546, -7463, 0, 0, -4733, 0, 3276, -1638, 0, 546, 1274, 0, 0, -8374, 0, 0, -4369, 0, 1092, -2548,
  0, 182, 1820, 0, 182, -7281, 0, 0, -3276, 0, -1456, -1638, 0, 0, 1820, 0, -728, -2730, 0, -1456, -910, 0, -3640, -364, 0, 254, 1274
};
static const int32_t data_1446C[57] = {
  0, 0, -5461, 0, -2366, -1092, 0, -182, 1092, 0, -546, -9102, 0, 0, -6553, 0, 0, -5643, 0, -182, 1274, 0, 0, -13289, 0, 0, -5279, 0, 0, -8009, 0, 0,
  1456, 0, 182, -8738, 0, 0, -6007, 0, 0, -8738, 0, 182, 1638, 0, -728, -5279, 0, -1274, -6735, 0, -910, -8738, 0, 0, 1638
};
static const Vector4 data_14550 = {.x = 0.3f, .y = 0.3f, .z = 0.3f, .w = 0.3f};
static const Vector4 data_14560 = {.x = 0.7f, .y = 0.7f, .z = 0.7f, .w = 0.7f};
static const float data_14570[8] = {0.3f, 0.3f, 0.3f, 0.3f, 0.3f, 0.3f, 0.3f, 0.3f};
static const Vector4 data_14590 = {.x = 0.3f, .y = 0.3f, .z = 0.3f, .w = 0.3f};
static const FogColourData data_145A0 = {.red = 0.1f, .green = 410.0f, .blue = 1.02f, .colour = 0xFF3C345BU};
static const uint32_t data_145B0[4] = {0, 0, 0, 0};
static const char data_145C0[] __attribute__((scn_string_literal)) = "/scene/03/MFBT";
static const char data_145CF[] __attribute__((scn_string_literal)) = "TLYE";
static const char data_145D4[5] __attribute__((scn_string_literal)) = "TLYE";
static const char data_145D9[1] __attribute__((scn_string_literal)) = "";
static const char data_145DA[7] __attribute__((scn_string_literal)) = "01TLYE";
static const char data_145E1[11] __attribute__((scn_string_literal)) = "bgm106.snd";
static const char data_145EC[12] __attribute__((scn_string_literal)) = "a1_tlye.snd";
static const char data_145F8[] __attribute__((scn_string_literal)) = "sprite";
static const char data_145FF[13] __attribute__((scn_string_literal)) = "SCROLL19.SPR";
static const char data_1460C[5] __attribute__((scn_string_literal)) = "TLYE";
static const char data_14611[7] __attribute__((scn_string_literal)) = "EVHUM";
static const Vector3 data_14618 = {.x = -53.5f, .y = -0.5f, .z = 10.5f};
static const char data_14624[13] __attribute__((scn_string_literal)) = "model/object";
static const char data_14631[8] __attribute__((scn_string_literal)) = "SEA.PVR";
static const char data_14639[13] __attribute__((scn_string_literal)) = "model/object";
static const char data_14646[242] __attribute__((scn_string_literal)) = "trsea2.mt6";
static const int32_t data_14738[57] = {
  0, 0, -3640, 0, 0, -2366, -4369, 1820, -546, 0, 0, -5279, 0, 0, -6007, 0, 0, -2912, 0, 0, -910, 0, 0, -6553, 0, 0, -6735, 0, 0, -3640, 0, 0, -182,
  0, 0, -5461, 0, 0, -6007, 0, 0, -2366, 0, 0, 0, 0, 0, -5643, 0, 0, -4733, 0, 0, -2366, -1456, 0, 728
};
static const int32_t data_1481C[57] = {
  0, 0, -9466, 0, -546, -2912, 0, 3094, -2366, 0, 0, -18386, 0, 0, -19296, 0, 1092, -14381, 0, 0, 0, 0, 0, -16748, 0, 0, -17658, 0, 0, -15109, 0, 0,
  0, 0, 0, -16566, 0, 0, -18204, 0, 0, -14381, 0, 0, 0, 0, 0, -19478, 0, 0, -18386, 0, -4004, -12925, 0, 0, -546
};
static const int32_t data_14900[57] = {
  0, 0, -3276, 0, 0, 0, 0, 0, 0, 0, 0, -6553, 0, 0, -4369, 0, -546, -3458, 0, 0, 0, 0, 0, -5461, 0, 0, -5643, 0, 0, -3458, 0, 0, 0, 0, 0, -6007, 0, 0,
  -4551, 0, 546, -2548, 0, 0, 0, 0, 0, -6189, 0, -1092, -3094, 0, 0, -2548, 0, 0, 0
};
static const int32_t data_149E4[57] = {
  0, 0, -6007, 0, 0, -728, 0, 0, 0, 0, 0, -5097, 0, 0, -2184, 0, 0, -2002, 0, 0, 0, 0, 0, -5825, 0, 0, -2912, 0, 0, -728, 0, 0, 0, 0, 0, -4915, 0, 0,
  -4187, 0, 0, -1274, 0, 0, 0, 0, 0, -6553, 0, -364, -4369, 0, -546, -1456, 0, 0, 0
};
static const int32_t data_14AC8[57] = {
  0, -1638, -8920, 0, -1274, -1092, -3094, 2912, 1092, 0, -546, -8374, 0, 0, -8920, 0, 910, -9284, 0, -182, 1274, 0, 0, -13289, 0, 0, -6917, 0, -182,
  -9466, 0, 0, 1456, 0, 182, -10740, 0, 0, -7099, 0, 364, -7645, 0, 0, 1638, 0, -728, -7099, 0, -2366, -8920, 0, -910, -4369, 0, 0, 1638
};
static const Vector3i data_14BAC = {.x = 0, .y = 0, .z = 7281};
static const Vector3 data_14BB8 = {.x = 0.01f, .y = 0.01f, .z = 0.01f};
static const Vector3 data_14BC4 = {.x = 0.01f, .y = 0.01f, .z = 0.01f};
static const Vector4 data_14BD0 = {.x = 0.3f, .y = 0.3f, .z = 0.3f, .w = 0.3f};
static const Vector4 data_14BE0 = {.x = 1.0f, .y = 0.3f, .z = 0.3f, .w = 0.3f};
static const char data_14BF0[6] __attribute__((scn_string_literal)) = "01TLY";
static const char data_14BF6[] __attribute__((scn_string_literal)) = "/scene/03/MFBT";
static const char data_14C05[] __attribute__((scn_string_literal)) = "TERY";
static const char data_14C0A[5] __attribute__((scn_string_literal)) = "TERY";
static const char data_14C0F[1] __attribute__((scn_string_literal)) = "";
static const char data_14C10[5] __attribute__((scn_string_literal)) = "TERY";
static const char data_14C15[1] __attribute__((scn_string_literal)) = "";
static const char data_14C16[5] __attribute__((scn_string_literal)) = "TERY";
static const char data_14C1B[1] __attribute__((scn_string_literal)) = "";
static const char data_14C1C[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_14C21[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_14C26[13] __attribute__((scn_string_literal)) = "a1_pipeq.snd";
static const char data_14C33[5] __attribute__((scn_string_literal)) = "TERY";
static const Vector3 data_14C38 = {.x = 0.1f, .y = 0.0f, .z = -0.03f};
static const Vector3i data_14C44 = {.x = 0, .y = 0, .z = 2729};
static const Vector3 data_14C50 = {.x = 0.0f, .y = 0.1f, .z = 0.0f};
static const Vector3i data_14C5C = {.x = 0, .y = 0, .z = 0};
static const char data_14C68[6] __attribute__((scn_string_literal)) = "01TLY";
static const char data_14C6E[7] __attribute__((scn_string_literal)) = "01TLYD";
static const char data_14C75[11] __attribute__((scn_string_literal)) = "bgm120.snd";
static const char data_14C80[12] __attribute__((scn_string_literal)) = "a1_tlyd.snd";
static const Vector3 data_14C8C = {.x = 0.109f, .y = 0.0f, .z = -0.03f};
static const Vector3i data_14C98 = {.x = 62076, .y = 56436, .z = 4002};
static const Vector3 data_14CA4 = {.x = 0.0f, .y = 0.3f, .z = 0.0f};
static const Vector3i data_14CB0 = {.x = 0, .y = 0, .z = 0};
static const Vector3 data_14CBC = {.x = 0.1f, .y = 0.0f, .z = -0.03f};
static const Vector3i data_14CC8 = {.x = 0, .y = 0, .z = 0};
static const Vector3 data_14CD4 = {.x = 0.0f, .y = 0.1f, .z = 0.0f};
static const Vector3 data_14CE0 = {.x = 0.1f, .y = 0.0f, .z = 0.03f};
static const Vector3i data_14CEC = {.x = 0, .y = 0, .z = 0};
static const Vector3 data_14CF8 = {.x = 0.1f, .y = 0.0f, .z = -0.03f};
static const Vector3 data_14D04 = {.x = 0.0f, .y = 0.2f, .z = 0.0f};
static const Vector3 data_14D10 = {.x = 0.0f, .y = 0.23f, .z = 0.0f};
static const Vector3 data_14D1C = {.x = 0.05f, .y = 0.0f, .z = -0.05f};
static const Vector4 data_14D28 = {.x = 1.0f, .y = 0.2f, .z = 0.2f, .w = 0.2f};
static const uint32_t data_14D38 = 0xFFFFFBBC;
static const int32_t data_14D3C[2] = {-3094, 2184};
static const uint32_t data_14D44 = 5461;
static const int32_t data_14D48[2] = {-3640, 0};
static const uint32_t data_14D50 = 0xFFFFEAAB;
static const int32_t data_14D54[2] = {2730, 0};
static const uint32_t data_14D5C = 0xFFFFF27E;
static const int32_t data_14D60[2] = {3458, 0};
static const uint32_t data_14D68 = 0xFFFFE889;
static const uint8_t pad_14D6C[8] = {0};
static const uint32_t data_14D74 = 4551;
static const int32_t data_14D78[2] = {-4551, 0};
static const uint8_t data_14D80[12] = {0};
static const char data_14D8C[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_14D91[13] __attribute__((scn_string_literal)) = "e1kishou.snd";
static const char data_14D9E[38] __attribute__((scn_string_literal)) = "battle_1.snd";
static const char data_14DC4[6] __attribute__((scn_string_literal)) = "E1047";
static const char data_14DCA[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_14DCF[13] __attribute__((scn_string_literal)) = "e1kishou.snd";
static const char data_14DDC[13] __attribute__((scn_string_literal)) = "battle_1.snd";
static const char data_14DE9[7] __attribute__((scn_string_literal)) = "E1047";
static const uint32_t data_14DF0 = 0xC02D70A4;
static const float data_14DF4[2] = {0.0f, 95.29f};
static const uint32_t data_14DFC[3] = {0, 0, 0};
static const Vector3 data_14E08 = {.x = -1.3f, .y = 0.0f, .z = -0.59f};
static const Vector3 data_14E14 = {.x = 1.26f, .y = 0.0f, .z = 0.44f};
static const Vector3 data_14E20 = {.x = -19.39f, .y = 0.0f, .z = 104.32f};
static const Vector3 data_14E2C = {.x = -20.84f, .y = 0.0f, .z = 103.4f};
static const Vector3i data_14E38 = {.x = 0, .y = 47331, .z = 0};
static const Vector3i data_14E44 = {.x = 0, .y = 14563, .z = 0};
static const Vector3i data_14E50 = {.x = 0, .y = 54613, .z = 0};
static const Vector3i data_14E5C = {.x = 0, .y = 54613, .z = 0};
static const uint32_t data_14E68 = 910;
static const int32_t data_14E6C[56] = {
  910, -10922, 9466, 1456, 0, -10922, -910, 910, 0, 0, -3458, 0, 0, -12196, 0, 0, -4551, 0, 0, 1638, 0, 0, -4369, 0, 0, -13471, 0, 0, -4187, 0, 0,
  1092, 0, 0, -8738, 0, 0, -7645, 0, 0, -6371, 0, 0, 1092, 0, 0, -12379, 0, 0, -5825, 0, 0, -4004, -1456, 0, 364
};
static const Vector4 data_14F4C = {.x = 1.0f, .y = 0.4f, .z = 0.4f, .w = 0.4f};
static const char data_14F5C[] __attribute__((scn_string_literal)) = "/scene/03/MFBT";
static const char data_14F6B[] __attribute__((scn_string_literal)) = "KISY";
static const char data_14F70[6] __attribute__((scn_string_literal)) = "E1045";
static const char data_14F76[5] __attribute__((scn_string_literal)) = "KISY";
static const char data_14F7B[6] __attribute__((scn_string_literal)) = "E1045";
static const char data_14F81[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_14F86[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_14F8B[13] __attribute__((scn_string_literal)) = "e1kisqte.snd";
static const char data_14F98[12] __attribute__((scn_string_literal)) = "qtebat.snd";
static const uint32_t data_14FA4[3] = {0xBFA84EA5U, 0, 0x42B60000U};
static const uint32_t data_14FB0[3] = {0, 0xFFFFDC72U, 0};
static const char data_14FBC[10] __attribute__((scn_string_literal)) = "E1045A001";
static const char data_14FC6[10] __attribute__((scn_string_literal)) = "E1045B001";
static const char data_14FD0[10] __attribute__((scn_string_literal)) = "E1045B002";
static const char data_14FDA[10] __attribute__((scn_string_literal)) = "E1045A002";
static const Vector3 data_14FE4 = {.x = 1.4f, .y = 0.05f, .z = 3.77f};
static const int32_t data_14FF0[10] = {0, 5461, 0, 3640, 6189, 0, 12743, 12743, 0, 12743};
static const int32_t data_15018[10] = {62805, 0, 0, 0, 0, 0, 0, 0, 0, 60984};
static const Vector3i data_15040 = {.x = -79, .y = 0, .z = 7};
static const Vector3i data_1504C = {.x = 0, .y = 273, .z = 0};
static const Vector3 data_15058 = {.x = 0.393f, .y = 0.5787f, .z = 0.1303f};
static const Vector3i data_15064 = {.x = 35398, .y = 2845, .z = 60791};
static const Vector3 data_15070 = {.x = 0.5f, .y = 0.55369f, .z = 0.13332f};
static const Vector3i data_1507C = {.x = -30501, .y = 2846, .z = -6200};
static const char data_15088[10] __attribute__((scn_string_literal)) = "E1045C002";
static const char data_15092[10] __attribute__((scn_string_literal)) = "E1045A004";
static const char data_1509C[10] __attribute__((scn_string_literal)) = "E1045B004";
static const char data_150A6[10] __attribute__((scn_string_literal)) = "E1045A005";
static const char data_150B0[10] __attribute__((scn_string_literal)) = "E1045B005";
static const char data_150BA[10] __attribute__((scn_string_literal)) = "E1045A006";
static const char data_150C4[12] __attribute__((scn_string_literal)) = "E1045A007";
static const Vector3 data_150D0 = {.x = -0.3f, .y = 0.0f, .z = -0.1f};
static const Vector3 data_150DC = {.x = 0.6234f, .y = 0.0f, .z = 1.4381f};
static const Vector3 data_150E8 = {.x = 0.40844f, .y = 0.0f, .z = 0.057713f};
static const Vector3 data_150F4 = {.x = -0.473698f, .y = 0.0f, .z = 0.394292f};
static const char data_15100[10] __attribute__((scn_string_literal)) = "E1045A003";
static const char data_1510A[10] __attribute__((scn_string_literal)) = "E1045B003";
static const Vector3 data_15114 = {.x = -2.737f, .y = 0.4f, .z = 91.54f};
static const Vector3 data_15120 = {.x = 0.03f, .y = 0.0f, .z = 0.1f};
static const Vector3 data_1512C = {.x = 0.2f, .y = 0.0f, .z = 0.4f};
static const Vector3 data_15138 = {.x = -1.4607f, .y = 1.5f, .z = 80.155899f};
static const float data_15144[3] = {0.5f, 0.0f, 0.5f};
static const Vector3 data_15150 = {.x = -5.4243f, .y = 1.5f, .z = 78.478699f};
static const float data_1515C[3] = {0.5f, 0.0f, 0.5f};
static const Vector3 data_15168 = {.x = -4.2007f, .y = 1.5f, .z = 87.189102f};
static const float data_15174[3] = {0.5f, 0.0f, 0.5f};
static const int32_t data_15180[57] = {
  0, 0, -7281, 0, 0, -4004, 0, 546, 182, 0, 0, -18204, 0, 0, -16930, 0, 0, -12379, 0, 0, 0, 0, 0, -12196, 0, 0, -18204, 0, 0, -13289, 0, 0, 0, 0, 0,
  -11832, 0, 0, -18386, 0, 0, -13835, 0, 0, 0, 0, 910, -12014, 0, -2184, -16748, 0, -2002, -13835, 0, 0, 364
};
static const int32_t data_15264[114] = {
  0, 364, -13289, 0, 364, -4369, 0, 2912, -1638, 0, 0, -23119, 0, 0, -18932, 0, 0, -15837, 0, 0, 0, 0, 0, -17840, 0, 0, -20571, 0, 0, -15837, 0, 0, 0,
  0, 0, -16566, 0, 0, -21481, 0, 0, -16748, 0, 0, 0, 0, 910, -19660, 0, -2184, -20571, 0, -2002, -16566, 0, 0, 364, 0, 0, -13835, 0, 0, -1820, 0,
  -1456, 0, 0, 0, -1820, 0, 0, -4915, 0, 0, -3458, 0, 0, 0, 0, 0, -2184, 0, 0, -5279, 0, 0, -2548, 0, 0, 0, 0, 0, -3276, 0, 0, -7099, 0, 0, -910, 0,
  0, 0, 0, 0, -5825, 0, 0, -2366, 0, -1274, -4733, 0, 0, 0
};
static const Vector3i data_1542C = {.x = -1274, .y = 910, .z = -1820};
static const Vector3i data_15438 = {.x = 0, .y = 0, .z = -3458};
static const int32_t data_15444[57] = {
  0, 0, -728, 0, 0, -1274, 0, 364, -728, 0, 0, -9466, 0, 0, -11104, 0, 0, -7099, 0, 0, 0, 0, 0, -7827, 0, 0, -15655, 0, 0, -8374, 0, 0, 0, 0, 0,
  -8556, 0, 0, -13289, 0, 0, -9466, 0, 0, 0, 0, 910, -7645, 0, -2184, -11650, 0, -2002, -9830, 0, 0, 364
};
static const int32_t data_15528[57] = {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0
};
static const int32_t data_1560C[57] = {
  0, 0, -1820, 0, -2366, 1274, 0, -182, -1092, 0, -546, -2730, 0, 0, -2912, 0, 0, -2002, 0, -182, 1274, 0, 0, -11468, 0, 0, -17476, 0, 0, -8009, 0, 0,
  1456, 0, 182, -8738, 0, 0, -18932, 0, 0, -8738, 0, 182, 1638, 0, -728, -5279, 0, -1274, -18568, 0, -910, -10376, 0, 0, 1638
};
static const int32_t data_156F0[57] = {
  0, 0, -1820, 0, 1456, 1092, 0, -182, -1092, 0, 0, -910, 0, 0, -2730, 0, 0, 0, 0, 0, 0, 0, 0, -1820, 0, 0, -2184, 0, 0, 0, 0, 0, 0, 0, 0, -2548, 0,
  0, -6189, 0, 0, 0, 0, 0, 0, 0, -910, -6189, 0, -2184, -8192, 0, 0, 0, 0, 0, 0
};
static const int32_t data_157D4[101] = {
  0, 0, -1820, 0, -2366, 1274, 0, -182, -1092, 0, 0, -6007, 0, 0, -8192, 0, 0, -4733, 0, 0, 0, 0, 0, -5097, 0, 0, -14381, 0, 0, -4004, 0, 0, 0, 0, 0,
  -8738, 0, 0, -19842, 0, 0, -4004, 0, 0, 0, 0, -910, -12014, 0, -2184, -18568, 0, 0, -4551, 0, 0, 0, 1104202957, 1144473190, 1051931443, -13881276,
  874783270, 874783270, 1715685691, 1715685691, 1078355558, 1090623898, 1057803469, -12959142, 240937602, 240937602, -14270372, -14270372, 1078355558,
  1090623898, 1057803469, -12959142, 240937602, 240937602, -14270372, -14270372, 1074161254, 1102105805, 1056964608, -14671834, 1074161254,
  1102105805, 1048911544, -11382176, 1104202957, 1144473190, 1051931443, -13881276, 874783270, 874783270, 1715685691, 1715685691, 1065353216,
  1025758986, 1025758986, 1028443341
};
static const Vector4 data_15968 = {.x = 1.0f, .y = 0.4f, .z = 0.4f, .w = 0.4f};
static const float data_15978[3] = {2.1f, 22.1f, 0.26f};
static const int32_t data_15984 = -11382176;
static const uint32_t data_15988[4] = {0x75222224U, 0x75222224U, 0x95454541U, 0x95454541U};
static const Vector4 data_15998 = {.x = 1.0f, .y = 0.5f, .z = 0.5f, .w = 0.5f};
static const float data_159A8[3] = {15.0f, 30.0f, 1.0f};
static const int32_t data_159B4[5] = {-16777216, -16777216, -16777216, -16777216, -16777216};
static const Vector4 data_159C8 = {.x = 1.0f, .y = 0.5f, .z = 0.5f, .w = 0.5f};
static const Vector4 data_159D8 = {.x = 1.0f, .y = 0.4f, .z = 0.4f, .w = 0.4f};
static const float data_159E8[8] = {0.3f, 0.3f, 0.3f, 0.3f, 1.0f, 0.16f, 0.12f, 0.2f};
static const float data_15A08[2] = {1.0f, 1.0f};
static const uint32_t data_15A10 = 0x41A00000;
static const float data_15A14[2] = {0.0f, -50.0f};
static const uint32_t data_15A1C = 0x41A00000;
static const float data_15A20[2] = {0.0f, -50.0f};
static const uint32_t data_15A28 = 0xC2200000;
static const float data_15A2C[2] = {0.0f, 80.0f};
static const uint32_t data_15A34 = 0xC2D60000;
static const float data_15A38[2] = {0.0f, 78.0f};
static const uint8_t data_15A40[8] = {0};
static const float data_15A48 = 90.0f;
static const uint32_t data_15A4C = 0xC1A00000;
static const float data_15A50[2] = {0.0f, -50.0f};
static const uint32_t data_15A58 = 0xC1A00000;
static const float data_15A5C[2] = {0.0f, -50.0f};
static const uint32_t data_15A64 = 0xC2E60000;
static const float data_15A68[2] = {0.0f, 175.0f};
static const uint32_t data_15A70 = 0xC2DC0000;
static const float data_15A74[2] = {0.0f, 10.0f};
static const uint32_t data_15A7C = 0x42C80000;
static const float data_15A80[2] = {0.0f, -40.0f};
static const uint32_t data_15A88 = 0x43020000;
static const float data_15A8C[2] = {0.0f, 25.0f};
static const uint8_t data_15A94[12] = {0};
static const uint32_t data_15AA0 = 0x3F800000;
static const float data_15AA4[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint32_t data_15B28 = 0x3F800000;
static const float data_15B2C[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint8_t data_15BB0[16] = {0};
static const float data_15BC0[30] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint32_t data_15C38 = 0x3F800000;
static const float data_15C3C[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint32_t data_15CC0 = 0x3F800000;
static const float data_15CC4[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint8_t data_15D48[84] = {0};
typedef struct Row_15D9C { float w0; float w1; float w2; int32_t w3; } Row_15D9C;
static const Row_15D9C data_15D9C[3] = {
  {0.787f, 0.787f, 0.787f, 0},
  {0.0f, 0.0f, 0.0f, 0},
  {0.0f, 0.0f, 0.0f, 0}
};
static const int32_t data_15DCC = 0;
static const uint32_t data_15DD0 = 0x3F800000;
static const float data_15DD4[33] = {
  0.944f, 0.866f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f,
  0.787f, 0.787f, 0.787f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint32_t data_15E58 = 0x3F4978D5;
static const float data_15E5C[33] = {
  0.787f, 0.866f, 0.944f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  0.944f, 0.866f, 0.787f, 0.787f, 0.787f, 0.787f, 0.708f, 0.708f, 0.708f, 0.708f
};
static const uint8_t data_15EE0[12] = {0};
static const float data_15EEC[31] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint32_t data_15F68 = 0x3F800000;
static const float data_15F6C[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint32_t data_15FF0 = 0x3F800000;
static const float data_15FF4[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint32_t data_16078 = 0x3F800000;
static const float data_1607C[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint32_t data_16100 = 0x41A00000;
static const float data_16104[2] = {0.0f, -50.0f};
static const uint32_t data_1610C = 0x41A00000;
static const float data_16110[2] = {0.0f, -50.0f};
static const uint32_t data_16118 = 0xC2200000;
static const float data_1611C[2] = {0.0f, 80.0f};
static const uint32_t data_16124 = 0xC1A00000;
static const float data_16128[2] = {0.0f, -50.0f};
static const uint32_t data_16130 = 0xC1A00000;
static const float data_16134[2] = {0.0f, -50.0f};
static const uint32_t data_1613C = 0xC2E60000;
static const float data_16140[2] = {0.0f, 175.0f};
static const uint32_t data_16148 = 0x42C80000;
static const float data_1614C[2] = {0.0f, -40.0f};
static const uint8_t data_16154[12] = {0};
static const uint32_t data_16160 = 0x3F800000;
static const float data_16164[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint32_t data_161E8 = 0x3F800000;
static const float data_161EC[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint8_t data_16270[16] = {0};
static const float data_16280[30] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint8_t data_162F8[84] = {0};
typedef struct Row_1634C { float w0; float w1; float w2; int32_t w3; } Row_1634C;
static const Row_1634C data_1634C[3] = {
  {0.787f, 0.787f, 0.787f, 0},
  {0.0f, 0.0f, 0.0f, 0},
  {0.0f, 0.0f, 0.0f, 0}
};
static const int32_t data_1637C = 0;
static const uint32_t data_16380 = 0x3F800000;
static const float data_16384[33] = {
  0.944f, 0.866f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f,
  0.787f, 0.787f, 0.787f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint32_t data_16408 = 0x3F4978D5;
static const float data_1640C[33] = {
  0.787f, 0.866f, 0.944f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  0.944f, 0.866f, 0.787f, 0.787f, 0.787f, 0.787f, 0.708f, 0.708f, 0.708f, 0.708f
};
static const uint32_t data_16490 = 0x3F800000;
static const float data_16494[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint32_t data_16518 = 0x3F800000;
static const float data_1651C[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint32_t data_165A0 = 0xC2A50000;
static const float data_165A4[2] = {0.0f, 68.0f};
static const uint32_t data_165AC = 0x3F800000;
static const float data_165B0[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint32_t data_16634 = 0xC1D00000;
static const float data_16638[2] = {0.0f, 76.5f};
static const uint32_t data_16640 = 0x3F800000;
static const float data_16644[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint32_t data_166C8 = 0x41A00000;
static const float data_166CC[2] = {0.0f, -50.0f};
static const uint32_t data_166D4 = 0x41A00000;
static const float data_166D8[2] = {0.0f, -50.0f};
static const uint32_t data_166E0 = 0xC2200000;
static const float data_166E4[2] = {0.0f, 80.0f};
static const uint32_t data_166EC = 0xC1A00000;
static const float data_166F0[2] = {0.0f, -50.0f};
static const uint32_t data_166F8 = 0xC1A00000;
static const float data_166FC[2] = {0.0f, -50.0f};
static const uint32_t data_16704 = 0xC2E60000;
static const float data_16708[5] = {0.0f, 175.0f, 100.0f, 0.0f, -40.0f};
static const uint8_t data_1671C[12] = {0};
static const uint32_t data_16728 = 0x3F800000;
static const float data_1672C[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint32_t data_167B0 = 0x3F800000;
static const float data_167B4[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint8_t data_16838[16] = {0};
static const float data_16848[30] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint8_t data_168C0[84] = {0};
typedef struct Row_16914 { float w0; float w1; float w2; int32_t w3; } Row_16914;
static const Row_16914 data_16914[3] = {
  {0.787f, 0.787f, 0.787f, 0},
  {0.0f, 0.0f, 0.0f, 0},
  {0.0f, 0.0f, 0.0f, 0}
};
static const int32_t data_16944 = 0;
static const uint32_t data_16948 = 0x3F800000;
static const float data_1694C[33] = {
  0.944f, 0.866f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f,
  0.787f, 0.787f, 0.787f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint32_t data_169D0 = 0x3F4978D5;
static const float data_169D4[33] = {
  0.787f, 0.866f, 0.944f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  0.944f, 0.866f, 0.787f, 0.787f, 0.787f, 0.787f, 0.708f, 0.708f, 0.708f, 0.708f
};
static const uint32_t data_16A58 = 0x3F800000;
static const float data_16A5C[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const Vector3 data_16AE0 = {.x = -1.4607f, .y = 1.5f, .z = 80.155899f};
static const Vector3 data_16AEC = {.x = 0.5f, .y = 0.0f, .z = 0.5f};
static const Vector3 data_16AF8 = {.x = -5.4243f, .y = 1.5f, .z = 78.478699f};
static const Vector3 data_16B04 = {.x = 0.5f, .y = 0.0f, .z = 0.5f};
static const Vector3 data_16B10 = {.x = -4.2007f, .y = 1.5f, .z = 87.189102f};
static const Vector3 data_16B1C = {.x = 0.5f, .y = 0.0f, .z = 0.5f};
static const char data_16B28[] __attribute__((scn_string_literal)) = "/scene/03/MFBT";
static const char data_16B37[] __attribute__((scn_string_literal)) = "TLYA";
static const char data_16B3C[5] __attribute__((scn_string_literal)) = "TLYA";
static const char data_16B41[1] __attribute__((scn_string_literal)) = "";
static const char data_16B42[7] __attribute__((scn_string_literal)) = "01TLYA";
static const char data_16B49[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_16B4E[11] __attribute__((scn_string_literal)) = "amb013.snd";
static const char data_16B59[11] __attribute__((scn_string_literal)) = "bgm013.snd";
static const char data_16B64[13] __attribute__((scn_string_literal)) = "a1_kbtmm.snd";
static const char data_16B71[5] __attribute__((scn_string_literal)) = "TLYA";
static const char data_16B76[7] __attribute__((scn_string_literal)) = "01TLYA";
static const char data_16B7D[] __attribute__((scn_string_literal)) = "/scene/03/MFBT";
static const char data_16B8C[] __attribute__((scn_string_literal)) = "TLYB";
static const char data_16B91[5] __attribute__((scn_string_literal)) = "TLYB";
static const char data_16B96[1] __attribute__((scn_string_literal)) = "";
static const char data_16B97[7] __attribute__((scn_string_literal)) = "01TLYB";
static const char data_16B9E[11] __attribute__((scn_string_literal)) = "bgm084.snd";
static const char data_16BA9[13] __attribute__((scn_string_literal)) = "a1_teryb.snd";
static const char data_16BB6[5] __attribute__((scn_string_literal)) = "TLYB";
static const char data_16BBB[7] __attribute__((scn_string_literal)) = "01TLYB";
static const char data_16BC2[11] __attribute__((scn_string_literal)) = "bgm013.snd";
static const char data_16BCD[13] __attribute__((scn_string_literal)) = "a1_kakom.snd";
static const char data_16BDA[] __attribute__((scn_string_literal)) = "/scene/03/MFBT";
static const char data_16BE9[] __attribute__((scn_string_literal)) = "TLYC";
static const char data_16BEE[5] __attribute__((scn_string_literal)) = "TLYC";
static const char data_16BF3[1] __attribute__((scn_string_literal)) = "";
static const char data_16BF4[7] __attribute__((scn_string_literal)) = "01TLYC";
static const char data_16BFB[5] __attribute__((scn_string_literal)) = "TLYC";
static const char data_16C00[8] __attribute__((scn_string_literal)) = "01TLYC";
static const Vector3 data_16C08 = {.x = -17.5f, .y = -1.8f, .z = -15.5f};
static const char data_16C14[13] __attribute__((scn_string_literal)) = "model/object";
static const char data_16C21[9] __attribute__((scn_string_literal)) = "SEA2.PVR";
static const char data_16C2A[13] __attribute__((scn_string_literal)) = "model/object";
static const char data_16C37[13] __attribute__((scn_string_literal)) = "TRSEA4.MT6";
static uint32_t data_16C44 = 0x42157AE1;
typedef struct Row_16C48 { int32_t w0; float w1; shen_name w2; int32_t w3; int32_t w4; int32_t w5; float w6; } Row_16C48;
static const Row_16C48 data_16C48[8] = {
  {0, 100.42f, SHEN_NAME('NPC0'), 1, 0, 63533, 37.7f},
  {0, 97.56f, SHEN_NAME('NPC1'), 1, 0, 2287, 36.97f},
  {0, 94.32f, SHEN_NAME('NPC2'), 1, 0, 63533, 28.47f},
  {0, 83.84f, SHEN_NAME('NPC3'), 1, 0, 2287, 25.35f},
  {0, 79.93f, SHEN_NAME('NPC4'), 1, 0, 2289, 31.3f},
  {0, 61.9f, SHEN_NAME('SIT2'), 1, 0, 2336, 3.78f},
  {0, 89.0f, SHEN_NAME('SITM'), 0, 56862, 2336, 20.4f},
  {0, 58.7f, CHARA_SATO, 0, 44273, 2292, 46.88f}
};
static const float data_16D28[2] = {0.0f, 35.8f};
static const shen_name data_16D30[2] = {CHARA_PEDR, 0};
static const int32_t data_16D38[2] = {44273, 2283};
static const Vector3 data_16D40 = {.x = 32.084999f, .y = 0.0f, .z = 100.117996f};
static const uint8_t data_16D4C[4] = {0};
static const int32_t data_16D50[2] = {57344, 0};
static const Vector3 data_16D58 = {.x = 32.410999f, .y = 0.0f, .z = 100.514f};
static const uint8_t data_16D64[4] = {0};
static const int32_t data_16D68[2] = {57344, 0};
static const char data_16D70[11] __attribute__((scn_string_literal)) = "M_FGT1.BIN";
static const char data_16D7B[6] __attribute__((scn_string_literal)) = "E1037";
static const char data_16D81[6] __attribute__((scn_string_literal)) = "BTT00";
static const char data_16D87[11] __attribute__((scn_string_literal)) = "rim008.snd";
static const char data_16D92[13] __attribute__((scn_string_literal)) = "e1batbat.snd";
static const char data_16D9F[13] __attribute__((scn_string_literal)) = "battle_1.snd";
static const char data_16DAC[] __attribute__((scn_string_literal)) = "K0DY";
static const char data_16DB1[] __attribute__((scn_string_literal)) = "K3DY";
static const char data_16DB6[] __attribute__((scn_string_literal)) = "CNDY";
static const char data_16DBB[21] __attribute__((scn_string_literal)) = "PRESS A TO START.";
static const Vector3 data_16DD0 = {.x = 1.5f, .y = 1.5f, .z = 1.5f};
static const Vector3 data_16DDC = {.x = 48.089001f, .y = 0.1f, .z = 38.501999f};
static const Vector3 data_16DE8 = {.x = 48.341f, .y = 0.0f, .z = 37.945999f};
static const Vector4 data_16DF4 = {.x = 1.0f, .y = 0.4f, .z = 0.4f, .w = 0.5f};
static const Vector4 data_16E04 = {.x = 0.0f, .y = 0.0f, .z = 0.0f, .w = 0.0f};
static const char data_16E14[10] __attribute__((scn_string_literal)) = "E1037C001";
static const char data_16E1E[27] __attribute__((scn_string_literal)) = "MFBT: Load PackFile %s:%s\012";
static const char data_16E39[28] __attribute__((scn_string_literal)) = "MFBT: Now Loading PackFile.";
static const char data_16E55[2] __attribute__((scn_string_literal)) = ".";
static const char data_16E57[2] __attribute__((scn_string_literal)) = "\012";
static const char data_16E59[] __attribute__((scn_string_literal)) = "/scene/03/MFBT";
static const char data_16E68[] __attribute__((scn_string_literal)) = "EVHUM";
static const char data_16E6E[6] __attribute__((scn_string_literal)) = "EVHUM";
static const char data_16E74[6] __attribute__((scn_string_literal)) = "EVHUM";
static const char data_16E7A[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_16E7F[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_16E84[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_16E89[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_16E8E[6] __attribute__((scn_string_literal)) = "FREE";

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

static int32_t script_0153(int32_t arg0) {
  Script_filmCheckOrSetTransitionUtil(arg0, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  end:;
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

static void script_039D(void) {
  int32_t local_FC;
  script_07FA();
  local_FC = Script_scenCreateTask(script_03E6, 0);
  Script_shenSeqUtils(2);
  while (Script_shenSeqUtils(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  script_0842();
  Script_scenToggleActiveFor(CHARA_KISY, 1);
  Script_srchControl((void *)local_FC);
  return;
}

static int32_t script_03E6(void) {
  int32_t local_FC;
  local_FC = 0;
  Script_scenToggleActiveFor(CHARA_KISY, 0);
  Script_SetShadowFlags(CHARA_AKIR, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_SetShadowFlags(CHARA_AKIR, SCRIPT_SHADOW_UPDATE_LIGHT_MASK, 14);
  Script_SetShadowFlags(CHARA_KISY, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_SetShadowFlags(CHARA_KISY, SCRIPT_SHADOW_UPDATE_LIGHT_MASK, 14);
  Script_SetShadowFlags(CHARA_TERY, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_SetShadowFlags(CHARA_TERY, SCRIPT_SHADOW_UPDATE_LIGHT_MASK, 14);
  for (;;) {
    if (local_FC == 0) {
      Script_filmCheckOrSetTransitionUtil(1, 255, 0, 0, 0, 0, 0, 0, 0);
      while (Script_filmCheckOrSetTransitionUtil(-1)) {
        SCNF_WaitFrames_0000(1);
      }
      Script_handSetFloatsFor(CHARA_TERY, 1, data_1446C, 1);
      Script_handSetFloatsFor(CHARA_TERY, 0, data_1446C, 1);
    }
    if (local_FC == 119) {
      Script_handSetFloatsFor(CHARA_AKIR, 1, data_142A4, 10);
    }
    if (local_FC == 132) {
      Script_handSetFloatsFor(CHARA_AKIR, 1, data_14388, 6);
    }
    if (local_FC == 147) {
      Script_handSetFloatsFor(CHARA_AKIR, 1, data_13AA0, 10);
    }
    if (local_FC == 845) {
      Script_scenToggleActiveFor(CHARA_KISY, 1);
    }
    if (local_FC == 898) {
      Script_handSetFloatsFor(CHARA_AKIR, 1, data_137F4, 1);
    }
    if (local_FC == 1060) {
      Script_executeAicaCommand(2720, 2, 140);
    }
    if (local_FC == 1110) {
      Script_filmCheckOrSetTransitionUtil(90, 0, 0, 0, 0, 255, 0, 0, 0);
      while (Script_filmCheckOrSetTransitionUtil(-1)) {
        SCNF_WaitFrames_0000(1);
      }
    }
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_058B(void) {
  int32_t local_FC;
  script_07FA();
  local_FC = Script_scenCreateTask(script_05D3, 0);
  Script_scenCreateTask(script_09E4, 0);
  Script_shenSeqUtils(0);
  while (Script_shenSeqUtils(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  script_0842();
  Script_srchControl((void *)local_FC);
  return;
}

static int32_t script_05D3(void) {
  int32_t local_FC;
  Vector3 local_F0;
  local_FC = 0;
  Script_SetShadowFlags(CHARA_AKIR, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_SetShadowFlags(CHARA_KISY, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_SetShadowFlags(CHARA_AKIR, SCRIPT_SHADOW_UPDATE_LIGHT_MASK_0, 2);
  Script_SetShadowFlags(CHARA_KISY, SCRIPT_SHADOW_UPDATE_LIGHT_MASK_0, 2);
  Script_SetShadowFlags(CHARA_AKIR, SCRIPT_SHADOW_UPDATE_LIGHT_MASK_0, 3);
  Script_SetShadowFlags(CHARA_KISY, SCRIPT_SHADOW_UPDATE_LIGHT_MASK_0, 3);
  Script_SetShadowFlags(CHARA_AKIR, SCRIPT_SHADOW_UPDATE_LIGHT_MASK, 1);
  Script_SetShadowFlags(CHARA_KISY, SCRIPT_SHADOW_UPDATE_LIGHT_MASK, 1);
  Script_scenToggleActiveFor(CHARA_KISY, 1);
  for (;;) {
    if (local_FC == 1) {
      Script_handSetFloatsFor(CHARA_AKIR, 1, data_137F4, 1);
      Script_handSetFloatsFor(CHARA_AKIR, 0, data_137F4, 1);
    }
    if ((local_FC > 253) & (local_FC < 344)) {
      Script_EVT_GetPosition(CHARA_AKIR, 5, &local_F0, ENTITY_XFORM_WORLD_SPACE);
      Script_FaceLookAtTargetUtils(CHARA_KISY, 2, &local_F0, 8);
    }
    if (local_FC == 370) {
      Script_FaceLookAtTargetUtils(CHARA_KISY, 0);
    }
    if (local_FC == 750) {
      Script_handSetFloatsFor(CHARA_AKIR, 1, data_13AA0, 45);
      Script_handSetFloatsFor(CHARA_AKIR, 0, data_13AA0, 45);
    }
    if (local_FC == 1440) {
      Script_executeAicaCommand(2720, 2, 80);
    }
    if (local_FC == 1460) {
      Script_filmCheckOrSetTransitionUtil(60, 0, 0, 0, 0, 255, 0, 0, 0);
      while (Script_filmCheckOrSetTransitionUtil(-1)) {
        SCNF_WaitFrames_0000(1);
      }
    }
    if (local_FC == 531) {
      Script_SetCharaRefBoardPos(CHARA_AKIR, &data_14560);
      Script_SetCharaRefBoardPos(CHARA_KISY, &data_14560);
    }
    if (local_FC == 771) {
      Script_SetCharaRefBoardPos(CHARA_AKIR, &data_14550);
      Script_SetCharaRefBoardPos(CHARA_KISY, &data_14550);
    }
    if (local_FC == 881) {
      Script_SetCharaRefBoardPos(CHARA_AKIR, &data_14560);
      Script_SetCharaRefBoardPos(CHARA_KISY, &data_14560);
    }
    if (local_FC == 1081) {
      Script_SetCharaRefBoardPos(CHARA_AKIR, &data_14550);
      Script_SetCharaRefBoardPos(CHARA_KISY, &data_14550);
    }
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_07FA(void) {
  script_08C0(CHARA_AKIR);
  script_08C0(CHARA_TERY);
  script_08C0(CHARA_KISY);
  Script_charaDisplay(SHEN_NAME('PIPE'), CDM_Show);
  Script_SetMotnFlag(CHARA_AKIR, -1);
  Script_SetShadowRange(10000.0f);
  return;
}

static void script_0842(void) {
  script_099E(CHARA_AKIR);
  script_099E(CHARA_TERY);
  script_099E(CHARA_KISY);
  Script_charaDisplay(SHEN_NAME('PIPE'), CDM_Hide);
  return;
}

static void script_0874(void) {
  script_08C0(CHARA_AKIR);
  script_08C0(CHARA_KISY);
  Script_SetMotnFlag(CHARA_AKIR, -1);
  Script_SetShadowRange(10000.0f);
  return;
}

static void script_08A5(void) {
  script_099E(CHARA_AKIR);
  script_099E(CHARA_KISY);
  return;
}

static void script_08C0(int32_t arg0) {
  Script_ccowUtils(arg0, 2);
  Script_ccowSetFlag80(arg0, 1);
  Script_motmGetCharaFlags(arg0, 0, 0);
  Script_motmToggleFlags(arg0, 2, 0x40);
  Script_FaceEx(arg0, 1, 0, 1);
  Script_ResetHandMotion(arg0, 1, 1, 0);
  Script_ResetHandMotion(arg0, 1, 0, 0);
  Script_handSetFloatsFor(arg0, 0, data_13AA0, 10);
  Script_handSetFloatsFor(arg0, 1, data_13AA0, 10);
  Script_SetCPAMFlags(arg0, 0);
  Script_FaceLookAtTargetUtils(arg0, 0);
  Script_charaSetRefboardFlagTo(arg0, 1);
  Script_SetCharaRefBoardPos(arg0, &data_14590);
  return;
}

static void script_0952(int32_t arg0) {
  Script_ccowUtils(arg0, 2);
  Script_ccowSetFlag80(arg0, 1);
  Script_CharHandlers(arg0, 2, 8, 16);
  Script_SetCPAMFlags(arg0, 0);
  return;
}

static void script_097B(int32_t arg0) {
  Script_ccowUtils(arg0, 1);
  Script_SetCPAMFlags(arg0, 1);
  Script_SetCharaLookAtPosition(arg0, -98310, 0, 0);
  return;
}

static void script_099E(int32_t arg0) {
  Script_ccowUtils(arg0, 1);
  Script_SetCPAMFlags(arg0, 1);
  Script_FaceLookAtTargetUtils(arg0, 0);
  Script_SetCharaLookAtPosition(arg0, -98310, 0, 0);
  Script_ResetHandMotion(arg0, 0, 1, 0);
  Script_ResetHandMotion(arg0, 0, 0, 0);
  return;
}

static int32_t script_09E4(void) {
  Script_filmCheckOrSetTransitionUtil(20, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  return Script_srchControl(0);
}

__attribute__((scn_string_bindings(0, data_145C0, 1, data_145CF, 2, data_145D4, 3, data_145D9, 9, data_1460C, 10, data_14611)))
static void script_0A18(void) {
  Script_mapmSetRenderFlag(0, 0);
  Script_mapmSetRenderFlag(1, 0);
  Script_mapmSetRenderFlag(2, 0);
  Script_mapmSetRenderFlag(3, 0);
  SCNF_WaitFrames_0000(1);
  script_13327("/scene/03/MFBT", "TLYE");
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
  Script_AuthorUtils(0, 0, "TLYE", "");
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
  Script_LoadVoiceFile(1, "01TLYE");
  script_134D1();
  Script_LoadSND(0, 2, "bgm106.snd", 0);
  Script_executeAicaCommand(16296, 0, 0);
  Script_executeAicaCommand(1184, 2, 100);
  Script_LoadSND(0, 4, "a1_tlye.snd", 0);
  Script_mapmSetCamFlags(0, 0);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 140.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 100.0f);
  Script_GetSetWeatherSettings(WEATHER_SETTINGS_SET_DISABLE_WEATHER, 0);
  Script_WeatherTaskUtil(FreeTaskData);
  Script_SetupLightFromTable(3);
  Script_SpriteUtils(Scroll_Read1, (int32_t)"sprite", "SCROLL19.SPR");
  Script_SetFogEnabled(1);
  Script_SetFogColour(&data_145A0);
  Script_SetFogFlags(0, data_145B0);
  Script_scenCreateTask(script_0BF2, 0);
  script_058B();
  Script_shenClearFreeState(0);
  Script_executeAicaCommand(2720, 1, 30);
  Script_executeAicaCommand(2720, 2, 30);
  Script_executeAicaCommand(2720, 3, 30);
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "TLYE");
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "EVHUM");
  Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_TERRY_DEFEATED, 1);
  Script_timeSetUpdateEnabled(0);
  {
    int32_t unused_FC;
    GameDateTime local_F4;
    Script_ReturnTimeDate(&local_F4);
    if ((&local_F4)->hours >= 22) {
      Script_IncrementDateTimeByNum(1);
    }
  }
  {
    int32_t unused_FC;
    GameDateTime local_F4;
    Script_ReturnTimeDate(&local_F4);
    (&local_F4)->hours = 5;
    (&local_F4)->minutes = 21;
    Script_SetNewDateTime(&local_F4);
  }
  Script_timeSetUpdateEnabled(1);
  Script_ryoControl(PCCTL_RequestNewMode, 3);
  Script_ryoControl(PCCTL_RequestNewMode, 0);
  Script_shenRequestGameModuleTransition(3, SHEN_NAME('JOMO'), 4);
  SCNF_WaitFrames_0000(1);
  return;
}

__attribute__((scn_string_bindings(0, data_14624, 2, data_14639)))
static int32_t script_0BF2(void) {
  int32_t local_FC;
  local_FC = Script_MT5Loader_TRWV_TRWAVE(0, &data_14618, "model/object", "SEA.PVR", "model/object", "trsea2.mt6", 2.0f, 0);
  Script_MT5Loader_TRWV_TRWAVE(13, local_FC, 0.125490203f, 0.23137255f);
  Script_MT5Loader_TRWV_TRWAVE(21, local_FC, 4, 4);
  Script_MT5Loader_TRWV_TRWAVE(16, local_FC, 0, 0);
  Script_MT5Loader_TRWV_TRWAVE(14, local_FC, 0.1f, 0.01f);
  Script_MT5Loader_TRWV_TRWAVE(10, local_FC, 32.0f, 32.0f, 1, 1, 1, 1);
  return Script_srchControl(0);
}

static void script_0C82(int32_t arg0) {
  int32_t local_FC;
  script_0030();
  script_185A();
  if (arg0) {
    Script_scenCreateTask(script_0153, 1, 1);
  }
  Script_shenSeqUtils(0);
  Script_executeAicaCommand(71848, 0, 0);
  Script_executeAicaCommand(1184, 2, 110);
  local_FC = 0;
  while (Script_shenSeqUtils(-1)) {
    {
      int32_t unused_E0[7];
      int32_t local_DC;
      local_DC = local_FC;
      switch (local_DC) {
        case 253:
          Script_ResetHandMotion(CHARA_AKIR, 1, 1, 0);
          Script_ResetHandMotion(CHARA_AKIR, 1, 0, 0);
          Script_handSetFloatsFor(CHARA_AKIR, 1, data_14738, 1);
          Script_handSetFloatsFor(CHARA_AKIR, 0, data_14738, 1);
          break;
        case 388:
          Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
          Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
          break;
        case 520:
          Script_ResetHandMotion(CHARA_AKIR, 1, 1, 0);
          Script_ResetHandMotion(CHARA_AKIR, 1, 0, 0);
          Script_handSetFloatsFor(CHARA_AKIR, 1, data_14738, 1);
          Script_handSetFloatsFor(CHARA_AKIR, 0, data_14738, 1);
          break;
        case 576:
          Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
          Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
          break;
      }
    }
    {
      int32_t unused_E0[7];
      int32_t local_DC;
      local_DC = local_FC;
      switch (local_DC) {
        case 157:
          Script_ResetHandMotion(CHARA_TERY, 1, 1, 0);
          Script_ResetHandMotion(CHARA_TERY, 1, 0, 0);
          Script_handSetFloatsFor(CHARA_TERY, 1, data_14AC8, 1);
          Script_handSetFloatsFor(CHARA_TERY, 0, data_14738, 1);
          break;
        case 253:
          Script_ResetHandMotion(CHARA_TERY, 0, 1, 0);
          Script_ResetHandMotion(CHARA_TERY, 0, 0, 0);
          break;
        case 460:
          Script_ResetHandMotion(CHARA_TERY, 1, 1, 0);
          Script_handSetFloatsFor(CHARA_TERY, 1, data_14AC8, 1);
          break;
        case 473:
          Script_ResetHandMotion(CHARA_TERY, 1, 0, 0);
          Script_handSetFloatsFor(CHARA_TERY, 0, data_14738, 1);
          break;
        case 576:
          Script_ResetHandMotion(CHARA_TERY, 0, 1, 0);
          Script_ResetHandMotion(CHARA_TERY, 0, 0, 0);
          break;
      }
    }
    {
      int32_t unused_E0[7];
      int32_t local_DC;
      local_DC = local_FC;
      switch (local_DC) {
        case 253:
          Script_ResetHandMotion(CHARA_KISY, 1, 1, 0);
          Script_ResetHandMotion(CHARA_KISY, 1, 0, 0);
          Script_handSetFloatsFor(CHARA_KISY, 1, data_14738, 1);
          Script_handSetFloatsFor(CHARA_KISY, 0, data_14738, 1);
          break;
        case 388:
          Script_ResetHandMotion(CHARA_KISY, 0, 1, 0);
          Script_ResetHandMotion(CHARA_KISY, 0, 0, 0);
          break;
        case 473:
          Script_ResetHandMotion(CHARA_KISY, 1, 1, 0);
          Script_ResetHandMotion(CHARA_KISY, 1, 0, 0);
          Script_handSetFloatsFor(CHARA_KISY, 1, data_14738, 1);
          Script_handSetFloatsFor(CHARA_KISY, 0, data_14738, 1);
          break;
        case 487:
          Script_ResetHandMotion(CHARA_KISY, 0, 1, 0);
          Script_ResetHandMotion(CHARA_KISY, 0, 0, 0);
          break;
      }
    }
    {
      int32_t unused_E0[7];
      int32_t local_DC;
      local_DC = local_FC;
      switch (local_DC) {
        case 253:
          Script_charaDisplay(SHEN_NAME('PIPE'), CDM_Hide);
          break;
        case 387:
          Script_charaDisplay(SHEN_NAME('PIPE'), CDM_Show);
          break;
      }
    }
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  if (arg0) {
    Script_filmCheckOrSetTransitionUtil(1, 0, 0, 0, 0, 255, 0, 0, 0);
    while (Script_filmCheckOrSetTransitionUtil(-1)) {
      SCNF_WaitFrames_0000(1);
    }
  }
  script_194A();
  return;
}

static void script_1062(int32_t arg0) {
  int32_t local_FC;
  int32_t unused_EC[4];
  Vector3 local_E0;
  script_0030();
  script_185A();
  if (arg0) {
    Script_scenCreateTask(script_0153, 1, 1);
  }
  Script_shenSeqUtils(1);
  local_FC = 0;
  while (Script_shenSeqUtils(-1)) {
    {
      int32_t unused_D4[3];
      int32_t local_D0;
      local_D0 = local_FC;
      switch (local_D0) {
        case 0:
          Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 100.0f);
          Script_SetMotnFlag(CHARA_KISY, -1);
          Script_SetMotnFlag(CHARA_TERY, -1);
          Script_SetMaxInterpSpeed(0.0f);
          break;
        case 30:
          Script_EVT_GetPosition(CHARA_TERY, 17, &local_E0, 0);
          break;
        case 462:
          Script_scenCreateTask(script_17B3, 0);
          break;
        case 507:
          Script_scenCreateTask(script_16C1, 0);
          break;
        case 825:
          Script_SetMotnFlag(CHARA_KISY, 0);
          break;
      }
    }
    {
      int32_t unused_D4[3];
      int32_t local_D0;
      local_D0 = local_FC;
      switch (local_D0) {
        case 21:
          Script_ResetHandMotion(CHARA_AKIR, 1, 1, 0);
          Script_handSetFloatsFor(CHARA_AKIR, 1, data_14AC8, 1);
          break;
        case 176:
          Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
          break;
        case 171:
          Script_ResetHandMotion(CHARA_AKIR, 1, 0, 0);
          Script_handSetFloatsFor(CHARA_AKIR, 0, data_14738, 1);
          break;
        case 201:
          Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
          break;
      }
    }
    {
      int32_t unused_D4[3];
      int32_t local_D0;
      local_D0 = local_FC;
      switch (local_D0) {
        case 1:
          Script_ResetHandMotion(CHARA_TERY, 1, 1, 0);
          Script_handSetFloatsFor(CHARA_TERY, 1, data_14AC8, 1);
          Script_SetHandRootRotation(CHARA_TERY, 1, 0x15, &data_14BAC);
          break;
        case 174:
          Script_ResetHandMotion(CHARA_TERY, 0, 1, 0);
          break;
        case 191:
          Script_ResetHandMotion(CHARA_TERY, 1, 0, 0);
          Script_handSetFloatsFor(CHARA_TERY, 0, data_14738, 1);
          break;
        case 206:
          Script_ResetHandMotion(CHARA_TERY, 0, 0, 0);
          break;
        case 341:
          Script_ResetHandMotion(CHARA_TERY, 1, 1, 0);
          Script_handSetFloatsFor(CHARA_TERY, 1, data_1481C, 8);
          Script_ResetHandMotion(CHARA_TERY, 1, 0, 0);
          Script_handSetFloatsFor(CHARA_TERY, 0, data_1481C, 8);
          break;
        case 539:
          Script_ResetHandMotion(CHARA_TERY, 0, 1, 0);
          Script_ResetHandMotion(CHARA_TERY, 0, 0, 0);
          break;
        case 664:
          Script_ResetHandMotion(CHARA_TERY, 1, 1, 0);
          Script_handSetFloatsFor(CHARA_TERY, 1, data_14738, 1);
          Script_ResetHandMotion(CHARA_TERY, 1, 0, 0);
          Script_handSetFloatsFor(CHARA_TERY, 0, data_14738, 1);
          break;
        case 857:
          Script_ResetHandMotion(CHARA_TERY, 0, 1, 0);
          Script_ResetHandMotion(CHARA_TERY, 0, 0, 0);
          break;
        case 887:
          Script_ResetHandMotion(CHARA_TERY, 1, 0, 0);
          Script_handSetFloatsFor(CHARA_TERY, 0, data_14738, 1);
          break;
        case 934:
          Script_ResetHandMotion(CHARA_TERY, 0, 0, 0);
          break;
      }
    }
    {
      int32_t unused_D4[3];
      int32_t local_D0;
      local_D0 = local_FC;
      switch (local_D0) {
        case 1:
          Script_CharHandlers(CHARA_KISY, 0, 8, 16);
          break;
        case 205:
          Script_ResetHandMotion(CHARA_KISY, 1, 1, 0);
          Script_ResetHandMotion(CHARA_KISY, 1, 0, 0);
          Script_handSetFloatsFor(CHARA_KISY, 1, data_149E4, 0);
          Script_handSetFloatsFor(CHARA_KISY, 0, data_14900, 0);
          break;
        case 539:
          Script_ResetHandMotion(CHARA_KISY, 0, 1, 0);
          Script_ResetHandMotion(CHARA_KISY, 0, 0, 0);
          break;
        case 619:
          Script_ResetHandMotion(CHARA_KISY, 1, 1, 0);
          Script_ResetHandMotion(CHARA_KISY, 1, 0, 0);
          Script_handSetFloatsFor(CHARA_KISY, 1, data_14738, 1);
          Script_handSetFloatsFor(CHARA_KISY, 0, data_14738, 1);
          break;
        case 830:
          Script_ResetHandMotion(CHARA_KISY, 0, 1, 0);
          Script_ResetHandMotion(CHARA_KISY, 0, 0, 0);
          break;
      }
    }
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  if (arg0) {
    Script_filmCheckOrSetTransitionUtil(1, 0, 0, 0, 0, 255, 0, 0, 0);
    while (Script_filmCheckOrSetTransitionUtil(-1)) {
      SCNF_WaitFrames_0000(1);
    }
  }
  script_194A();
  return;
}

static void script_1534(int32_t arg0) {
  int32_t local_FC;
  int32_t unused_F8;
  Vector3 local_EC;
  int32_t unused_E0[3];
  int32_t local_DC;
  script_0030();
  script_185A();
  Script_scenToggleVisibleFor(CHARA_TERY, 0);
  Script_charaDisplay(CHARA_TERY, CDM_Hide);
  if (arg0) {
    Script_scenCreateTask(script_0153, 1, 1);
  }
  Script_shenSeqUtils(2);
  local_FC = 0;
  while (Script_shenSeqUtils(-1)) {
    local_DC = local_FC;
    switch (local_DC) {
      case 0:
        Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 100.0f);
        Script_mapmSetCamFlags(0, 0);
        break;
      case 149:
        Script_mapmSetCamFlags(0, 1);
        break;
      case 824:
        Script_FaceLookAtTargetUtils(CHARA_KISY, 0);
        break;
      case 1172:
        Script_FaceLookAtTargetUtils(CHARA_AKIR, 0);
        break;
    }
    if ((local_FC > 725) & (local_FC < 824)) {
      local_EC.x = 38.1f;
      (&local_EC)->y = 1.75f;
      (&local_EC)->z = 51.2f;
      Script_FaceLookAtTargetUtils(CHARA_KISY, 2, &local_EC, 3);
    }
    if ((local_FC > 905) & (local_FC < 1172)) {
      Script_EVT_GetPosition(CHARA_KISY, 5, &local_EC, 0);
      Script_FaceLookAtTargetUtils(CHARA_AKIR, 2, &local_EC, 3);
    }
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  if (arg0) {
    Script_filmCheckOrSetTransitionUtil(60, 0, 0, 0, 0, 255, 0, 0, 0);
    while (Script_filmCheckOrSetTransitionUtil(-1)) {
      SCNF_WaitFrames_0000(1);
    }
  }
  script_194A();
  Script_scenToggleVisibleFor(CHARA_TERY, 1);
  Script_charaDisplay(CHARA_TERY, CDM_Show);
  return;
}

static int32_t script_16C1(void) {
  int32_t local_FC;
  Vector3 local_F0;
  Vector3 local_E4;
  Script_EVT_GetPosition(CHARA_TERY, 11, &local_F0, 0);
  Script_EVT_GetPosition(CHARA_KISY, 5, &local_E4, 0);
  local_F0.x = ((&local_F0)->x + (&local_E4)->x) / 2.0f;
  (&local_F0)->y = ((&local_F0)->y + (&local_E4)->y) / 2.0f;
  (&local_F0)->z = ((&local_F0)->z + (&local_E4)->z) / 2.0f;
  local_FC = Script_EffectUtils(0, &local_F0, &data_14BB8, 60, 0.025f, 0, 0);
  Script_EffectUtils(9, local_FC, 75, 72, 58);
  Script_EffectUtils(8, local_FC, 1.7f, 1.7f);
  Script_EffectUtils(11, local_FC, 114);
  Script_EffectUtils(12, local_FC, 17);
  Script_EffectUtils(1, local_FC);
  SCNF_WaitFrames_0000(15);
  Script_EffectUtils(7, local_FC, 0.0f);
  SCNF_WaitFrames_0000(120);
  Script_EffectUtils(2, local_FC);
  return Script_EffectUtils(3, local_FC);
}

static int32_t script_17B3(void) {
  int32_t local_FC;
  Vector3 local_F0;
  Script_EVT_GetPosition(CHARA_KISY, 2, &local_F0, 0);
  (&local_F0)->y = 0.0f;
  local_FC = Script_EffectUtils(0, &local_F0, &data_14BC4, 60, 0.025f, 0, 0);
  Script_EffectUtils(9, local_FC, 59, 56, 42);
  Script_EffectUtils(8, local_FC, 1.2f, 1.2f);
  Script_EffectUtils(11, local_FC, 114);
  Script_EffectUtils(12, local_FC, 17);
  Script_EffectUtils(1, local_FC);
  SCNF_WaitFrames_0000(15);
  Script_EffectUtils(7, local_FC, 0.0f);
  SCNF_WaitFrames_0000(120);
  Script_EffectUtils(2, local_FC);
  return Script_EffectUtils(3, local_FC);
}

static void script_185A(void) {
  script_7F2F(CHARA_AKIR);
  Script_SetCharaFlags(CHARA_AKIR, 4);
  Script_SetShadowFlags(CHARA_AKIR, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_SetShadowFlags(CHARA_AKIR, SCRIPT_SHADOW_UPDATE_LIGHT_MASK, 14);
  Script_SetCharaRefBoardPos(CHARA_AKIR, &data_14BD0);
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 1);
  script_7F2F(CHARA_KISY);
  Script_SetCharaFlags(CHARA_KISY, 4);
  Script_SetShadowFlags(CHARA_KISY, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_SetShadowFlags(CHARA_KISY, SCRIPT_SHADOW_UPDATE_LIGHT_MASK, 14);
  Script_SetCharaRefBoardPos(CHARA_KISY, &data_14BD0);
  Script_charaSetRefboardFlagTo(CHARA_KISY, 1);
  script_7F2F(CHARA_TERY);
  Script_SetCharaFlags(CHARA_TERY, 4);
  Script_SetShadowFlags(CHARA_TERY, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_SetShadowFlags(CHARA_TERY, SCRIPT_SHADOW_UPDATE_LIGHT_MASK, 14);
  Script_SetCharaRefBoardPos(CHARA_TERY, &data_14BD0);
  Script_charaSetRefboardFlagTo(CHARA_TERY, 1);
  return;
}

static void script_194A(void) {
  script_00D5(CHARA_AKIR);
  Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
  Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
  script_00D5(CHARA_KISY);
  Script_ResetHandMotion(CHARA_KISY, 0, 1, 0);
  Script_ResetHandMotion(CHARA_KISY, 0, 0, 0);
  script_00D5(CHARA_TERY);
  return;
}

__attribute__((scn_string_bindings(0, data_14BF0, 1, data_14BF6, 2, data_14C05, 3, data_14C0A, 4, data_14C0F, 5, data_14C10, 6, data_14C15, 7, data_14C16, 8, data_14C1B, 9, data_14C1C, 10, data_14C21, 12, data_14C33)))
static void script_19B1(void) {
  int32_t unused_FC;
  GameDateTime local_F4;
  int32_t local_F0;
  Script_shenClearFreeState(0);
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_ReturnTimeDate(&local_F4);
  Script_SetupLightFromTable(6);
  Script_mapmSetCamFlags(0, 1);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 30.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 20.0f);
  Script_PREP_Related(-1, 1);
  Script_LoadVoiceFile(1, "01TLY");
  script_13327("/scene/03/MFBT", "TERY");
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
  Script_AuthorUtils(0, 0, "TERY", "");
  Script_AuthorUtils(0, 1, "TERY", "");
  Script_AuthorUtils(0, 2, "TERY", "");
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
  Script_LoadSND(0, 5, "FREE", 0);
  Script_LoadSND(0, 4, "FREE", 0);
  Script_LoadSND(0, 4, "a1_pipeq.snd", 0);
  if (-1) {
    Script_charaDisplay(CHARA_STSI, CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(CHARA_STSI, CDM_Show);
      goto L001AB9;
    }
  }
  L001AB9:;
  Script_scenToggleVisibleFor(CHARA_STSI, 0);
  Script_motmToggleFlags(CHARA_AKIR, 2, 0x80);
  Script_SetCharaRefBoardPos(CHARA_AKIR, &data_14BE0);
  Script_SetCharaRefBoardPos(CHARA_KISY, &data_14BE0);
  Script_SetCharaRefBoardPos(CHARA_TERY, &data_14BE0);
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 1);
  Script_charaSetRefboardFlagTo(CHARA_KISY, 1);
  Script_charaSetRefboardFlagTo(CHARA_TERY, 1);
  (void)1;
  task_7C = 0;
  Script_uiUtils(0, 21, 0);
  Script_SetSfxOverlayTimeoutTuning(0, SfxMode_SessionAction);
  Script_shenSeqUtils(-9, 0, 0);
  Script_shenSeqUtils(-9, 1, 0);
  task_6C = 0;
  for (;;) {
    local_F0 = task_6C;
    switch (local_F0) {
      case 0:
        script_1EB9();
        break;
      case 2:
        script_1EEE();
        Script_sicnUtils(0, 1, 1);
        SCNF_WaitFrames_0000(90);
        Script_sicnUtils(1, 0, 0);
        Script_SetNewDateTime(&local_F4);
        Script_WeatherTaskUtil(FreeTaskData);
        Script_WeatherTaskUtil(InitTaskData);
        Script_WeatherTaskUtil(ToggleSceneLightingFlags, 0);
        Script_PREP_Related(-1, 0);
        Script_SetupLightFromTable(6);
        Script_PREP_Related(-1, 1);
        Script_SetSfxOverlayTimeoutTuning(1, SfxMode_SessionAction);
        task_7C = task_7C + 1;
        break;
      case 1:
        script_1CC8();
        Script_PlayerStatsUtils(PS_Set70MBTimeCapSlot, 17, task_7C + 1);
        Script_PlayerStatsUtils(PS_Set70MBCounter0, Script_PlayerStatsUtils(PS_Get70MBCounter0) + 1);
        if (task_7C > 0) {
          Script_PlayerStatsUtils(PS_Set70MBCounter1, Script_PlayerStatsUtils(PS_Get70MBCounter1) + 1);
        }
        Script_SetSfxOverlayTimeoutTuning(2, SfxMode_SessionAction);
        Script_shenClearFreeState(0);
        goto L001C5A;
        break;
    }
    SCNF_WaitFrames_0000(1);
  }
  L001C5A:;
  Script_executeAicaCommand(2720, 2, 30);
  Script_AuthorUtils(AUTH_FREE, 0);
  Script_AuthorUtils(AUTH_FREE, 1);
  Script_AuthorUtils(AUTH_FREE, 2);
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "TERY");
  Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 936, 1);
  Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 936, 1);
  Script_SetGameStateTo(AUTH_None);
  Script_PREP_Related(-1, 0);
  return;
}

static int32_t script_1CB4(void) {
  Script_seqEnterEvtCam(1001, 0, AUTH_None);
  return Script_srchControl(0);
}

__attribute__((scn_string_bindings(0, data_14C68)))
static void script_1CC8(void) {
  Script_fixoSetAttachmentOffset(SHEN_NAME('PIPE'), 0, &data_14C50, &data_14C5C);
  Script_motmGetCharaFlags(CHARA_TERY, 0, 0);
  Script_ccowUtils(CHARA_TERY, 2);
  Script_motmToggleFlags(CHARA_TERY, 2, 0x40);
  Script_fixoAttachEx(SHEN_NAME('PIPE'), CHARA_TERY, 18, &data_14C38, &data_14C44);
  Script_motmToggleFlags(CHARA_TERY, 1, 0x40);
  Script_scenCreateTask(script_1DC2, 0);
  script_1062(1);
  Script_LoadVoiceFile(0, "01TLY");
  Script_executeAicaCommand(2720, 2, 30);
  Script_LoadVoiceFile(1, "01TLYD");
  script_134D1();
  Script_LoadSND(0, 2, "bgm120.snd", 0);
  Script_executeAicaCommand(16296, 0, 0);
  Script_executeAicaCommand(1184, 2, 124);
  Script_LoadSND(0, 4, "a1_tlyd.snd", 0);
  script_039D();
  Script_filmCheckOrSetTransitionUtil(0, 255, 0, 0, 0, 0, 0, 0, 0);
  Script_shenClearFreeState(0);
  return;
}

static int32_t script_1DC2(void) {
  SCNF_WaitFrames_0000(120);
  Script_fixoSetAttachmentOffset(SHEN_NAME('PIPE'), 0, &data_14CA4, &data_14CB0);
  Script_motmGetCharaFlags(CHARA_AKIR, 0, 0);
  Script_ccowUtils(CHARA_AKIR, 2);
  Script_motmToggleFlags(CHARA_AKIR, 2, 0x40);
  Script_fixoAttachEx(SHEN_NAME('PIPE'), CHARA_AKIR, 18, &data_14C8C, &data_14C98);
  Script_motmToggleFlags(CHARA_AKIR, 1, 0x40);
  SCNF_WaitFrames_0000(80);
  Script_fixoClearAttachment(SHEN_NAME('PIPE'));
  return Script_srchControl(0);
}

static int32_t script_1E46(void) {
  SCNF_WaitFrames_0000(10);
  Script_fixoSetAttachmentOffset(SHEN_NAME('PIPE'), 0, &data_14CD4, &data_14CC8);
  Script_motmGetCharaFlags(CHARA_TERY, 0, 0);
  Script_ccowUtils(CHARA_TERY, 2);
  Script_motmToggleFlags(CHARA_TERY, 2, 0x40);
  Script_fixoAttachEx(SHEN_NAME('PIPE'), CHARA_TERY, 18, &data_14CBC, &data_14CC8);
  Script_motmToggleFlags(CHARA_TERY, 1, 0x40);
  return Script_srchControl(0);
}

static void script_1EB9(void) {
  task_6C = 255;
  Script_scenCreateTask(script_2129, 0);
  Script_scenCreateTask(script_2180, 0);
  Script_scenCreateTask(script_1E46, 0);
  script_0C82(0);
  return;
}

static void script_1EEE(void) {
  task_6C = 255;
  Script_SetCPAMFlags(CHARA_TERY, 0);
  Script_scenCreateTask(script_1CB4, 0);
  Script_EVT_SetPosition(CHARA_AKIR, -0x48000000, &data_14D1C);
  Script_ccowUtils(CHARA_AKIR, 2);
  Script_ccowUtils(CHARA_TERY, 2);
  Script_ccowUtils(CHARA_KISY, 2);
  Script_CharHandlers(CHARA_AKIR, 2, 8, 16);
  Script_charaToggleMotEnabled(CHARA_AKIR, 1);
  Script_charaToggleMotEnabled(CHARA_TERY, 1);
  Script_PlayCharaMotion(CHARA_AKIR, 4353, 60.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_PlayCharaMotion(CHARA_TERY, 4354, 60.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_SetCharaMotionPlaybackRate(CHARA_AKIR, 0.8f);
  Script_SetCharaMotionPlaybackRate(CHARA_TERY, 0.8f);
  SCNF_WaitFrames_0000(36);
  Script_fixoSetAttachmentOffset(SHEN_NAME('PIPE'), 0, &data_14D04, &data_14CEC);
  Script_motmGetCharaFlags(CHARA_TERY, 0, 0);
  Script_ccowUtils(CHARA_TERY, 2);
  Script_motmToggleFlags(CHARA_TERY, 2, 0x40);
  Script_fixoAttachEx(SHEN_NAME('PIPE'), CHARA_TERY, 12, &data_14CE0, &data_14CEC);
  Script_motmToggleFlags(CHARA_TERY, 1, 0x40);
  SCNF_WaitFrames_0000(15);
  Script_executeAicaCommand(0x1704A9, 0, 0);
  SCNF_WaitFrames_0000(5);
  Script_executeAicaCommand(722089, 0, 0);
  SCNF_WaitFrames_0000(19);
  Script_fixoSetAttachmentOffset(SHEN_NAME('PIPE'), 0, &data_14D10, &data_14CEC);
  Script_motmGetCharaFlags(CHARA_TERY, 0, 0);
  Script_ccowUtils(CHARA_TERY, 2);
  Script_motmToggleFlags(CHARA_TERY, 2, 0x40);
  Script_fixoAttachEx(SHEN_NAME('PIPE'), CHARA_TERY, 18, &data_14CF8, &data_14CEC);
  Script_motmToggleFlags(CHARA_TERY, 1, 0x40);
  SCNF_WaitFrames_0000(10);
  Script_scenCreateTask(script_21B4, 0);
  SCNF_WaitFrames_0000(15);
  Script_executeAicaCommand(0x1704A9, 0, 0);
  SCNF_WaitFrames_0000(10);
  Script_executeAicaCommand(0x1704A9, 0, 0);
  SCNF_WaitFrames_0000(15);
  Script_executeAicaCommand(0x1704A9, 0, 0);
  SCNF_WaitFrames_0000(20);
  task_6C = 0;
  return;
}

static int32_t script_2129(void) {
  int32_t local_FC;
  SCNF_WaitFrames_0000(535);
  local_FC = SCNF_UIUtils(0x100, 0, 1, 14, 1, 0);
  if (local_FC != 0xFFFF) {
    if (local_FC & 0x100) {
      task_6C = 1;
      Script_shenSeqUtils(-6);
    } else {
      task_6C = 2;
    }
  }
  return Script_srchControl(0);
}

static int32_t script_2180(void) {
  Script_filmCheckOrSetTransitionUtil(60, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  return Script_srchControl(0);
}

static int32_t script_21B4(void) {
  Script_filmCheckOrSetTransitionUtil(60, 0, 0, 0, 0, 255, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  return Script_srchControl(0);
}

static void script_21E8(void) {
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 120.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 50.0f);
  Script_SetCharaRefBoardPos(CHARA_TERY, &data_14D28);
  Script_SetCharaRefBoardPos(CHARA_STSI, &data_14D28);
  Script_charaSetRefboardFlagTo(CHARA_TERY, 1);
  Script_charaSetRefboardFlagTo(CHARA_STSI, 1);
  Script_SetupLightFromTable(2);
  Script_mapmSetCamFlags(0, 1);
  Script_PREP_Related(-1, 1);
  return;
}

static void script_2250(int32_t arg0) {
  Script_FaceEx(CHARA_KISY, arg0, 0, 2);
  Script_ResetHandMotion(CHARA_KISY, arg0, 1, 0);
  Script_ResetHandMotion(CHARA_KISY, arg0, 0, 0);
  Script_handSetFloatsFor(CHARA_KISY, 1, data_14738, 0);
  Script_handSetFloatsFor(CHARA_KISY, 0, data_14738, 0);
  Script_FACE_LCG(CHARA_KISY, 2, 0, 0);
  Script_FaceEx(CHARA_AKIR, arg0, 0, 2);
  Script_ResetHandMotion(CHARA_AKIR, arg0, 1, 0);
  Script_ResetHandMotion(CHARA_AKIR, arg0, 0, 0);
  Script_handSetFloatsFor(CHARA_AKIR, 1, data_14738, 0);
  Script_handSetFloatsFor(CHARA_AKIR, 0, data_14738, 0);
  Script_FACE_LCG(CHARA_AKIR, 2, 0, 0);
  return;
}

static void script_2319(int32_t arg0, int32_t arg1, const int32_t *arg2, int32_t arg3) {
  if (arg3 == 1) {
    if (arg1 == 1) {
      arg1 = 0;
    } else {
      arg1 = 1;
    }
  }
  Script_handSetFloatsFor(arg0, arg1, arg2, 0);
  return;
}

static void script_2349(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  if (arg3 == 1) {
    if (arg1 == 1) {
      arg1 = 0;
    } else {
      arg1 = 1;
    }
    Script_ScenDataUtil((void *)arg2, 14, 0, -Script_ScenDataUtil((void *)arg2, 4, 0));
    Script_ScenDataUtil((void *)arg2, 14, 1, -Script_ScenDataUtil((void *)arg2, 4, 1));
  }
  Script_SetHandRootRotation(arg0, arg1, 0x15, (const Vector3i *)arg2);
  if (arg3 == 1) {
    Script_ScenDataUtil((void *)arg2, 14, 0, -Script_ScenDataUtil((void *)arg2, 4, 0));
    Script_ScenDataUtil((void *)arg2, 14, 1, -Script_ScenDataUtil((void *)arg2, 4, 1));
  }
  return;
}

__attribute__((scn_string_bindings(0, data_14D8C, 1, data_14D91, 2, data_14D9E)))
static void script_23EB(void) {
  Script_LoadSND(0, 4, "FREE", 0);
  Script_LoadSND(3, 0, 0, 0);
  Script_LoadSND(1, 4, "e1kishou.snd", 0x8000);
  while (Script_LoadSND(2, 0, 0, 0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_LoadSND(1, 5, "battle_1.snd", 0x8000);
  while (Script_LoadSND(2, 0, 0, 0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_LoadSND(4, 0, 0, 0);
  Script_srchControl(0);
  return;
}

__attribute__((scn_string_bindings(0, data_14DC4, 1, data_14DCA, 2, data_14DCF, 3, data_14DDC, 4, data_14DE9)))
static void script_247A(void) {
  Script_timeSetUpdateEnabled(0);
  Script_LoadVoiceFile(1, "E1047");
  Script_LoadSND(0, 4, "FREE", 0);
  Script_LoadSND(0, 4, "e1kishou.snd", 0);
  Script_LoadSND(0, 5, "battle_1.snd", 0);
  script_21E8();
  Script_charaDisplay(CHARA_KISY, CDM_Show);
  Script_charaDisplay(CHARA_AKIR, CDM_Show);
  Script_charaDisplay(CHARA_TERY, CDM_Show);
  Script_charaDisplay(CHARA_STSI, CDM_Show);
  Script_FACE(CHARA_AKIR, 2, 0, 0);
  Script_FACE(CHARA_KISY, 2, 0, 0);
  Script_FaceLookAtTargetUtils(CHARA_AKIR, 0);
  Script_FaceLookAtTargetUtils(CHARA_KISY, 0);
  script_254C();
  Script_LoadVoiceFile(0, "E1047");
  Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_MFBT_KISY_FIGHT_STARTED, 1);
  Script_timeSetUpdateEnabled(1);
  return;
}

static void script_254C(void) {
  Script_SetCycleManagerData(31, &data_14DF0, data_14DFC, 1, 0x31303030);
  Script_MoveObjectToObject(CHARA_AKIR, 31);
  Script_MoveObjectToObject(CHARA_KISY, 31);
  Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &data_14E08);
  Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &data_14E38, 0);
  Script_EVT_SetPosition(CHARA_KISY, POS_XYZ, &data_14E14);
  Script_EVT_SetRotation(CHARA_KISY, 0x38000000, &data_14E44, 0);
  Script_MoveObjectToObject(CHARA_AKIR, -1);
  Script_MoveObjectToObject(CHARA_KISY, -1);
  Script_EVT_SetPosition(CHARA_TERY, POS_XYZ, &data_14E20);
  Script_EVT_SetRotation(CHARA_TERY, 0x38000000, &data_14E50, 0);
  Script_EVT_SetPosition(CHARA_STSI, POS_XYZ, &data_14E2C);
  Script_EVT_SetRotation(CHARA_STSI, 0x38000000, &data_14E5C, 0);
  Script_PlayCharaMotion(CHARA_TERY, MN_AKI_AKI_UDEKUMI_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_PlayCharaMotion(CHARA_STSI, MN_AKI_AKI_UDEKUMI_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_PlayCharaMotion(CHARA_KISY, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_SetCPAMFlags(CHARA_KISY, 0);
  Script_seqEnterEvtCam(5900, CHARA_AKIR, CHARA_AKIR);
  SCNF_WaitFrames_0000(1);
  Script_ryoControl(RYO_RestoreControl);
  Script_vfSetup(0, 20, 20, 0, 0);
  Script_vfStageAssignRingBoundary((const Vector3 *)&data_14DF0, 6.0f);
  task_0 = 0x52494B41;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 0;
  *(&task_0 + 3) = 0;
  *(&task_0 + 4) = 400;
  *(&task_0 + 5) = 0;
  *(&task_0 + 6) = 0;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(CHARA_AKIR, VF_FORMATION_APPLY_FLAGS, VF_HP_TEAM_BASED, 49);
  task_0 = 0x5953494B;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 400;
  *(&task_0 + 5) = 28;
  *(&task_0 + 6) = 0;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(CHARA_KISY, VF_FORMATION_APPLY_FLAGS, 0x200000, 0x800);
  Script_vfConfigureFormationGroup(CHARA_KISY, VF_FORMATION_APPLY_FLAGS, VF_HP_TEAM_BASED, 49);
  SCN_Enqueue_VFGI(2, 99, 1.0f, 3, -1);
  Script_filmCheckOrSetTransitionUtil(20, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_shenClearFreeState(1);
  for (;;) {
    if ((Script_GetFightingStats(-2, 3) & 1) != 0) {
      SCNF_WaitFrames_0000(180);
      break;
    }
    if ((Script_GetFightingStats(-2, 3) & 2) != 0) {
      script_3187(0);
    }
    if ((Script_GetFightingStats(-2, 3) & 4) != 0) {
      script_3187(1);
    }
    if ((Script_GetFightingStats(-2, 3) & 8) != 0) {
      script_3533(0);
    }
    if ((Script_GetFightingStats(-2, 3) & 0x10) != 0) {
      script_3533(1);
    }
    if ((Script_GetFightingStats(-2, 3) & 0x20) != 0) {
      script_2BC4(0);
    }
    if ((Script_GetFightingStats(-2, 3) & 0x40) != 0) {
      script_2BC4(1);
    }
    if ((Script_GetFightingStats(-2, 3) & 0x80) != 0) {
      script_2EFA(0);
    }
    if ((Script_GetFightingStats(-2, 3) & 0x100) != 0) {
      script_2EFA(1);
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_shenClearFreeState(0);
  Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_vfToggleBattleState(scnPostBattleDelay);
  for (;;) {
    if (Script_ryoControl(PCCTL_QueryShouldRun) != 0) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  task_0 = -6;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 0;
  *(&task_0 + 3) = 0;
  *(&task_0 + 4) = 0;
  *(&task_0 + 5) = 13;
  *(&task_0 + 6) = 0;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0;
  Script_vfUtils(&task_0);
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_motmToggleFlags(CHARA_AKIR, 2, 0x80);
  Script_motmToggleFlags(CHARA_KISY, 2, 0x80);
  Script_GetDataFromFightProgram(CHARA_AKIR, 0);
  Script_GetDataFromFightProgram(CHARA_KISY, 0);
  return;
}

static void script_2A81(void) {
  Vector3 local_F4;
  Vector3 local_E8;
  Vector3 local_DC;
  int32_t local_D8;
  Script_EVT_GetPosition(CHARA_AKIR, -1, &local_F4, 0);
  Script_EVT_GetPosition(CHARA_KISY, -1, &local_E8, 0);
  Script_GetAuthorState_And_SetCamPos(&local_DC, 0);
  local_DC.x = (&local_DC)->x - (&local_F4)->x;
  (&local_DC)->z = (&local_DC)->z - (&local_F4)->z;
  local_E8.x = (&local_E8)->x - (&local_F4)->x;
  (&local_E8)->z = (&local_E8)->z - (&local_F4)->z;
  local_D8 = Script_FloatUtils(ScriptMathOp_Atan2ReverseVector, (&local_E8)->x, (&local_E8)->z) - Script_FloatUtils(ScriptMathOp_Atan2ReverseVector, (&local_DC)->x, (&local_DC)->z);
  if (!((float)((local_D8 << 16) >> 16) * 0.00549316406f <= 45.0f)) {
    task_80 = 1;
  } else if (!((float)((local_D8 << 16) >> 16) * 0.00549316406f <= 0.0f)) {
    task_80 = 0;
  } else if (!((float)((local_D8 << 16) >> 16) * 0.00549316406f <= -45.0f)) {
    task_80 = 2;
  } else {
    task_80 = 3;
  }
  return;
}

static void script_2B7B(int32_t arg0, int32_t arg1, int32_t arg2) {
  if (arg0 == 0) {
    Script_seqEnterEvtCam(arg1, arg2, arg2);
  } else {
    Script_seqEnterEvtCam(arg1 + 1, arg2, arg2);
  }
  return;
}

static void script_2BA1(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  if (arg0 == 0) {
    Script_seqEnterEvtCam(arg1, arg3, arg3);
  } else {
    Script_seqEnterEvtCam(arg2, arg3, arg3);
  }
  return;
}

static void script_2BC4(int32_t arg0) {
  Vector3 local_F4;
  int32_t local_F0;
  script_2250(1);
  script_2349(CHARA_KISY, 0, (int32_t)&data_14D50, arg0);
  script_2349(CHARA_AKIR, 0, (int32_t)&data_14D5C, arg0);
  SCNF_WaitFrames_0000(2);
  for (;;) {
    SCNF_WaitFrames_0000(1);
    if (!(Script_GetCharaMotionRemainingFramesScaled(CHARA_AKIR) <= 5.0f)) {
      break;
    }
  }
  script_2A81();
  local_F0 = task_80;
  switch (local_F0) {
    case 0:
      script_2BA1(arg0, 5100, 5105, CHARA_AKIR);
      break;
    case 1:
      script_2BA1(arg0, 5100, 5103, CHARA_AKIR);
      break;
    case 2:
      script_2BA1(arg0, 5104, 5101, CHARA_AKIR);
      break;
    case 3:
      script_2BA1(arg0, 5102, 5101, CHARA_AKIR);
      break;
  }
  for (;;) {
    SCNF_WaitFrames_0000(1);
    if (!(Script_GetCharaMotionRemainingFramesScaled(CHARA_AKIR) <= 54.0f)) {
      break;
    }
  }
  script_2B7B(arg0, 5110, CHARA_AKIR);
  Script_EVT_GetPosition(CHARA_KISY, -1, &local_F4, 0);
  (&local_F4)->y = 1.3f;
  Script_FaceLookAtTargetUtils(CHARA_AKIR, 2, &local_F4, 10);
  Script_EVT_GetPosition(CHARA_AKIR, -1, &local_F4, 0);
  (&local_F4)->y = 1.3f;
  Script_FaceLookAtTargetUtils(CHARA_KISY, 2, &local_F4, 10);
  Script_FACE(CHARA_AKIR, 5, 3, 40);
  SCNF_WaitFrames_0000(40);
  Script_FACE(CHARA_AKIR, 2, 6, 40);
  for (;;) {
    SCNF_WaitFrames_0000(1);
    if (!(Script_GetCharaMotionRemainingFramesScaled(CHARA_AKIR) <= 117.0f)) {
      break;
    }
  }
  Script_FACE(CHARA_KISY, 5, 3, 30);
  SCNF_WaitFrames_0000(30);
  Script_FACE(CHARA_KISY, 2, 6, 30);
  for (;;) {
    SCNF_WaitFrames_0000(1);
    if (!(Script_GetCharaMotionRemainingFramesScaled(CHARA_AKIR) <= 167.0f)) {
      break;
    }
  }
  Script_FACE(CHARA_AKIR, 5, 5, 60);
  Script_FACE(CHARA_KISY, 5, 5, 60);
  SCNF_WaitFrames_0000(60);
  Script_FACE(CHARA_AKIR, 2, 0, 60);
  Script_FACE(CHARA_KISY, 2, 0, 60);
  for (;;) {
    SCNF_WaitFrames_0000(1);
    if (!(Script_GetCharaMotionRemainingFramesScaled(CHARA_AKIR) <= 200.0f)) {
      break;
    }
  }
  for (;;) {
    SCNF_WaitFrames_0000(1);
    if (!(Script_GetCharaMotionRemainingFramesScaled(CHARA_AKIR) <= 298.0f)) {
      break;
    }
  }
  script_2B7B(arg0, 5150, CHARA_AKIR);
  script_2319(CHARA_AKIR, 1, data_1481C, arg0);
  script_2319(CHARA_AKIR, 0, data_1481C, arg0);
  for (;;) {
    SCNF_WaitFrames_0000(1);
    if (Script_IsCharaMotionFinished(CHARA_AKIR) != 0) {
      break;
    }
  }
  Script_SetGameStateTo(AUTH_Combat);
  Script_FaceLookAtTargetUtils(CHARA_AKIR, 0);
  Script_FaceLookAtTargetUtils(CHARA_KISY, 0);
  Script_FACE(CHARA_AKIR, 2, 0, 0);
  Script_FACE(CHARA_KISY, 2, 0, 0);
  script_2250(0);
  return;
}

static void script_2EFA(int32_t arg0) {
  Vector3 local_F4;
  int32_t local_F0;
  script_2250(1);
  script_2349(CHARA_AKIR, 0, (int32_t)&data_14D68, arg0);
  script_2349(CHARA_AKIR, 1, (int32_t)&data_14D74, arg0);
  SCNF_WaitFrames_0000(2);
  for (;;) {
    SCNF_WaitFrames_0000(1);
    if (!(Script_GetCharaMotionRemainingFramesScaled(CHARA_AKIR) <= 5.0f)) {
      break;
    }
  }
  script_2A81();
  local_F0 = task_80;
  switch (local_F0) {
    case 0:
      script_2BA1(arg0, 5200, 5205, CHARA_AKIR);
      break;
    case 1:
      script_2BA1(arg0, 5200, 5203, CHARA_AKIR);
      break;
    case 2:
      script_2BA1(arg0, 5204, 5201, CHARA_AKIR);
      break;
    case 3:
      script_2BA1(arg0, 5202, 5201, CHARA_AKIR);
      break;
  }
  for (;;) {
    SCNF_WaitFrames_0000(1);
    if (!(Script_GetCharaMotionRemainingFramesScaled(CHARA_AKIR) <= 59.0f)) {
      break;
    }
  }
  script_2B7B(arg0, 5210, CHARA_AKIR);
  Script_EVT_GetPosition(CHARA_AKIR, -1, &local_F4, 0);
  (&local_F4)->y = 1.3f;
  Script_FaceLookAtTargetUtils(CHARA_KISY, 2, &local_F4, 10);
  Script_FACE(CHARA_AKIR, 5, 5, 40);
  SCNF_WaitFrames_0000(40);
  Script_FACE(CHARA_AKIR, 2, 8, 40);
  for (;;) {
    SCNF_WaitFrames_0000(1);
    if (!(Script_GetCharaMotionRemainingFramesScaled(CHARA_AKIR) <= 111.0f)) {
      break;
    }
  }
  script_2B7B(arg0, 5230, CHARA_AKIR);
  for (;;) {
    SCNF_WaitFrames_0000(1);
    if (!(Script_GetCharaMotionRemainingFramesScaled(CHARA_AKIR) <= 153.0f)) {
      break;
    }
  }
  script_2B7B(arg0, 5220, CHARA_KISY);
  Script_FACE(CHARA_KISY, 5, 5, 35);
  SCNF_WaitFrames_0000(35);
  Script_FACE(CHARA_KISY, 2, 8, 35);
  for (;;) {
    SCNF_WaitFrames_0000(1);
    if (!(Script_GetCharaMotionRemainingFramesScaled(CHARA_AKIR) <= 180.0f)) {
      break;
    }
  }
  script_2B7B(arg0, 5240, CHARA_KISY);
  for (;;) {
    SCNF_WaitFrames_0000(1);
    if (Script_IsCharaMotionFinished(CHARA_AKIR) != 0) {
      break;
    }
  }
  Script_SetGameStateTo(AUTH_Combat);
  Script_FaceLookAtTargetUtils(CHARA_KISY, 0);
  Script_FACE(CHARA_AKIR, 2, 0, 0);
  Script_FACE(CHARA_KISY, 2, 0, 0);
  script_2250(0);
  return;
}

static void script_3187(int32_t arg0) {
  Vector3 local_F4;
  int32_t local_F0;
  script_2250(1);
  script_2349(CHARA_KISY, 0, (int32_t)&data_14D38, arg0);
  script_2319(CHARA_KISY, 1, data_1481C, arg0);
  script_2319(CHARA_KISY, 0, (const int32_t *)&data_14E68, arg0);
  script_2349(CHARA_AKIR, 1, (int32_t)&data_14D44, arg0);
  script_2319(CHARA_AKIR, 0, data_1481C, arg0);
  SCNF_WaitFrames_0000(2);
  for (;;) {
    SCNF_WaitFrames_0000(1);
    if (!(Script_GetCharaMotionRemainingFramesScaled(CHARA_AKIR) <= 5.0f)) {
      break;
    }
  }
  script_2A81();
  local_F0 = task_80;
  switch (local_F0) {
    case 0:
      script_2BA1(arg0, 5000, 5005, CHARA_AKIR);
      break;
    case 1:
      script_2BA1(arg0, 5000, 5003, CHARA_AKIR);
      break;
    case 2:
      script_2BA1(arg0, 5004, 5001, CHARA_AKIR);
      break;
    case 3:
      script_2BA1(arg0, 5002, 5001, CHARA_AKIR);
      break;
  }
  for (;;) {
    SCNF_WaitFrames_0000(1);
    if (!(Script_GetCharaMotionRemainingFramesScaled(CHARA_AKIR) <= 63.0f)) {
      break;
    }
  }
  script_2B7B(arg0, 5010, CHARA_AKIR);
  Script_EVT_GetPosition(CHARA_KISY, -1, &local_F4, 0);
  (&local_F4)->y = 1.3f;
  Script_FaceLookAtTargetUtils(CHARA_AKIR, 2, &local_F4, 10);
  Script_EVT_GetPosition(CHARA_AKIR, -1, &local_F4, 0);
  (&local_F4)->y = 1.3f;
  Script_FaceLookAtTargetUtils(CHARA_KISY, 2, &local_F4, 10);
  Script_FACE(CHARA_AKIR, 5, 3, 40);
  SCNF_WaitFrames_0000(40);
  Script_FACE(CHARA_AKIR, 2, 6, 40);
  for (;;) {
    SCNF_WaitFrames_0000(1);
    if (!(Script_GetCharaMotionRemainingFramesScaled(CHARA_AKIR) <= 131.0f)) {
      break;
    }
  }
  script_2B7B(arg0, 5020, CHARA_KISY);
  Script_FACE(CHARA_KISY, 5, 3, 30);
  SCNF_WaitFrames_0000(30);
  Script_FACE(CHARA_KISY, 2, 6, 30);
  for (;;) {
    SCNF_WaitFrames_0000(1);
    if (!(Script_GetCharaMotionRemainingFramesScaled(CHARA_AKIR) <= 176.0f)) {
      break;
    }
  }
  script_2B7B(arg0, 5030, CHARA_AKIR);
  Script_FACE(CHARA_AKIR, 5, 5, 60);
  Script_FACE(CHARA_KISY, 5, 5, 60);
  SCNF_WaitFrames_0000(60);
  Script_FACE(CHARA_AKIR, 2, 0, 60);
  Script_FACE(CHARA_KISY, 2, 0, 60);
  for (;;) {
    SCNF_WaitFrames_0000(1);
    if (!(Script_GetCharaMotionRemainingFramesScaled(CHARA_AKIR) <= 265.0f)) {
      break;
    }
  }
  script_2B7B(arg0, 5040, CHARA_AKIR);
  for (;;) {
    SCNF_WaitFrames_0000(1);
    if (!(Script_GetCharaMotionRemainingFramesScaled(CHARA_AKIR) <= 340.0f)) {
      break;
    }
  }
  script_2319(CHARA_KISY, 1, data_14738, arg0);
  script_2319(CHARA_KISY, 0, data_14738, arg0);
  script_2349(CHARA_KISY, 0, (int32_t)data_14D80, arg0);
  for (;;) {
    SCNF_WaitFrames_0000(1);
    if (Script_IsCharaMotionFinished(CHARA_AKIR) != 0) {
      break;
    }
  }
  Script_SetGameStateTo(AUTH_Combat);
  Script_FaceLookAtTargetUtils(CHARA_AKIR, 0);
  Script_FaceLookAtTargetUtils(CHARA_KISY, 0);
  Script_FACE(CHARA_AKIR, 2, 0, 0);
  Script_FACE(CHARA_KISY, 2, 0, 0);
  script_2250(0);
  return;
}

static void script_3533(int32_t arg0) {
  Vector3 local_F4;
  int32_t local_F0;
  script_2250(1);
  SCNF_WaitFrames_0000(2);
  for (;;) {
    SCNF_WaitFrames_0000(1);
    if (!(Script_GetCharaMotionRemainingFramesScaled(CHARA_AKIR) <= 5.0f)) {
      break;
    }
  }
  script_2A81();
  local_F0 = task_80;
  switch (local_F0) {
    case 0:
      script_2BA1(arg0, 5504, 5501, CHARA_AKIR);
      break;
    case 1:
      script_2BA1(arg0, 5502, 5501, CHARA_AKIR);
      break;
    case 2:
      script_2BA1(arg0, 5500, 5505, CHARA_AKIR);
      break;
    case 3:
      script_2BA1(arg0, 5500, 5503, CHARA_AKIR);
      break;
  }
  for (;;) {
    SCNF_WaitFrames_0000(1);
    if (!(Script_GetCharaMotionRemainingFramesScaled(CHARA_AKIR) <= 37.0f)) {
      break;
    }
  }
  script_2B7B(arg0, 5510, CHARA_AKIR);
  Script_EVT_GetPosition(CHARA_AKIR, -1, &local_F4, 0);
  (&local_F4)->y = 1.3f;
  Script_FaceLookAtTargetUtils(CHARA_KISY, 2, &local_F4, 10);
  Script_FACE(CHARA_AKIR, 5, 5, 40);
  SCNF_WaitFrames_0000(40);
  Script_FACE(CHARA_AKIR, 2, 8, 40);
  for (;;) {
    SCNF_WaitFrames_0000(1);
    if (!(Script_GetCharaMotionRemainingFramesScaled(CHARA_AKIR) <= 100.0f)) {
      break;
    }
  }
  script_2B7B(arg0, 5530, CHARA_AKIR);
  for (;;) {
    SCNF_WaitFrames_0000(1);
    if (!(Script_GetCharaMotionRemainingFramesScaled(CHARA_AKIR) <= 136.0f)) {
      break;
    }
  }
  script_2B7B(arg0, 5520, CHARA_KISY);
  Script_FACE(CHARA_KISY, 5, 5, 35);
  SCNF_WaitFrames_0000(35);
  Script_FACE(CHARA_KISY, 2, 8, 35);
  for (;;) {
    SCNF_WaitFrames_0000(1);
    if (!(Script_GetCharaMotionRemainingFramesScaled(CHARA_AKIR) <= 165.0f)) {
      break;
    }
  }
  script_2B7B(arg0, 5540, CHARA_KISY);
  script_2319(CHARA_AKIR, 1, data_1481C, arg0);
  script_2319(CHARA_AKIR, 0, data_1481C, arg0);
  for (;;) {
    SCNF_WaitFrames_0000(1);
    if (Script_IsCharaMotionFinished(CHARA_AKIR) != 0) {
      break;
    }
  }
  Script_SetGameStateTo(AUTH_Combat);
  Script_FaceLookAtTargetUtils(CHARA_KISY, 0);
  Script_FACE(CHARA_AKIR, 2, 0, 0);
  Script_FACE(CHARA_KISY, 2, 0, 0);
  script_2250(0);
  return;
}

static void script_37C0(void) {
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 90.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 45.0f);
  Script_SetCharaRefBoardPos(CHARA_AKIR, &data_14F4C);
  Script_SetCharaRefBoardPos(CHARA_KISY, &data_14F4C);
  Script_SetCharaRefBoardPos(CHARA_TERY, &data_14F4C);
  Script_SetCharaRefBoardPos(CHARA_STSI, &data_14F4C);
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 1);
  Script_charaSetRefboardFlagTo(CHARA_KISY, 1);
  Script_charaSetRefboardFlagTo(CHARA_TERY, 1);
  Script_charaSetRefboardFlagTo(CHARA_STSI, 1);
  (void)1;
  Script_FaceEx(CHARA_KISY, 1, 0, 2);
  Script_FACE_LCG(CHARA_KISY, 2, 0, 0);
  Script_FaceEx(CHARA_AKIR, 1, 0, 2);
  Script_FACE_LCG(CHARA_AKIR, 2, 0, 0);
  Script_FaceEx(CHARA_TERY, 1, 0, 2);
  Script_FACE_LCG(CHARA_TERY, 2, 0, 0);
  Script_ResetHandMotion(CHARA_AKIR, 1, 1, 0);
  Script_ResetHandMotion(CHARA_AKIR, 1, 0, 0);
  Script_handSetFloatsFor(CHARA_AKIR, 1, data_1481C, 0);
  Script_handSetFloatsFor(CHARA_AKIR, 0, data_1481C, 0);
  Script_ResetHandMotion(CHARA_KISY, 1, 1, 0);
  Script_ResetHandMotion(CHARA_KISY, 1, 0, 0);
  Script_handSetFloatsFor(CHARA_KISY, 1, data_14900, 0);
  Script_handSetFloatsFor(CHARA_KISY, 0, data_149E4, 0);
  return;
}

static void script_392D(void) {
  Script_uiUtils(0, 0, 0);
  return;
}

__attribute__((scn_string_bindings(0, data_14F5C, 1, data_14F6B, 2, data_14F70)))
static void script_393B(void) {
  script_13327("/scene/03/MFBT", "KISY");
  Script_LoadVoiceFile(1, "E1045");
  Script_scenCreateTask(script_3983, 0);
  return;
}

__attribute__((scn_string_bindings(0, data_14F76, 1, data_14F7B)))
static void script_3966(void) {
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "KISY");
  Script_LoadVoiceFile(0, "E1045");
  return;
}

__attribute__((scn_string_bindings(0, data_14F81, 1, data_14F86)))
static int32_t script_3983(void) {
  Script_LoadSND(0, 5, "FREE", 0);
  Script_LoadSND(0, 4, "FREE", 0);
  Script_LoadSND(3, 0, 0, 0);
  Script_LoadSND(1, 4, "e1kisqte.snd", 0x8000);
  while (Script_LoadSND(2, 0, 0, 0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_LoadSND(1, 5, "qtebat.snd", 0x8000);
  while (Script_LoadSND(2, 0, 0, 0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_LoadSND(4, 0, 0, 0);
  return Script_srchControl(0);
}

static void script_3A22(void) {
  int32_t unused_FC;
  GameDateTime local_F4;
  int32_t unused_F0;
  int32_t local_EC;
  script_247A();
  Script_ReturnTimeDate(&local_F4);
  task_B8 = 0;
  script_37C0();
  script_393B();
  task_9C = 0;
  for (;;) {
    local_EC = task_9C;
    switch (local_EC) {
      case 0:
        script_3AD0();
        break;
      case 1:
        script_3E43();
        goto L003AC5;
        break;
      case 3:
        script_3F82();
        Script_SetNewDateTime(&local_F4);
        Script_WeatherTaskUtil(FreeTaskData);
        Script_WeatherTaskUtil(InitTaskData);
        Script_WeatherTaskUtil(ToggleSceneLightingFlags, 0);
        Script_PREP_Related(-1, 0);
        Script_SetupLightFromTable(2);
        Script_PREP_Related(-1, 1);
        break;
    }
    SCNF_WaitFrames_0000(1);
  }
  L003AC5:;
  script_3966();
  script_392D();
  return;
}

static void script_3AD0(void) {
  task_9C = 255;
  Script_uiUtils(0, 21, 0);
  Script_SetSfxOverlayTimeoutTuning(0, SfxMode_SessionAction);
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_SetCycleManagerData(31, data_14FA4, data_14FB0, 1, 0x31303030);
  Script_MoveObjectToObject(CHARA_AKIR, 31);
  Script_MoveObjectToObject(CHARA_KISY, 31);
  Script_MoveObjectToObject(CHARA_TERY, 31);
  Script_MoveObjectToObject(CHARA_STSI, 31);
  Script_MoveObjectToObject(SHEN_NAME('HBUK'), 31);
  Script_MoveObjectToObject(SHEN_NAME('HDUT'), 31);
  Script_MoveObjectToObject(SHEN_NAME('HDUO'), 31);
  Script_MoveObjectToObject(SHEN_NAME('HDUA'), 0x52494B41);
  Script_MoveObjectToObject(SHEN_NAME('HDUK'), 0x5953494B);
  task_84.x = 0.7f;
  task_84.y = 0.0f;
  task_84.z = 4.05f;
  Script_EVT_SetPosition(SHEN_NAME('HDUT'), POS_XYZ, &task_84);
  task_90.y = 0;
  task_90.y = 0;
  task_90.z = 0;
  Script_EVT_SetRotation(SHEN_NAME('HDUT'), 0x38000000, &task_90, 0);
  task_84.x = 2.90942f;
  task_84.y = 0.0f;
  task_84.z = 5.7035f;
  Script_EVT_SetPosition(SHEN_NAME('HDUO'), POS_XYZ, &task_84);
  task_90.y = 0;
  task_90.y = 4551;
  task_90.z = 0;
  Script_EVT_SetRotation(SHEN_NAME('HDUO'), 0x38000000, &task_90, 0);
  task_84.x = 0.0f;
  task_84.y = 0.0f;
  task_84.z = 0.0f;
  Script_EVT_SetPosition(SHEN_NAME('HDUA'), POS_XYZ, &task_84);
  task_90.y = 0;
  task_90.y = 0;
  task_90.z = 0;
  Script_EVT_SetRotation(SHEN_NAME('HDUA'), 0x38000000, &task_90, 0);
  task_84.x = 0.0f;
  task_84.y = 0.0f;
  task_84.z = 0.0f;
  Script_EVT_SetPosition(SHEN_NAME('HDUK'), POS_XYZ, &task_84);
  task_90.y = 0;
  task_90.y = 0;
  task_90.z = 0;
  Script_EVT_SetRotation(SHEN_NAME('HDUK'), 0x38000000, &task_90, 0);
  Script_charaToggleMotEnabled(CHARA_AKIR, 0);
  Script_charaToggleMotEnabled(CHARA_KISY, 0);
  Script_charaToggleMotEnabled(CHARA_TERY, 0);
  Script_charaToggleMotEnabled(CHARA_STSI, 0);
  Script_ccowUtils(CHARA_AKIR, 2);
  Script_ccowUtils(CHARA_TERY, 2);
  Script_ccowUtils(CHARA_STSI, 2);
  Script_ccowUtils(CHARA_KISY, 2);
  Script_ccowUtils(SHEN_NAME('HBUK'), 2);
  Script_charaDisplay(SHEN_NAME('HBUK'), CDM_Hide);
  Script_charaDisplay(CHARA_TERY, CDM_Hide);
  Script_charaDisplay(CHARA_STSI, CDM_Hide);
  Script_ccowSetFlag80(SHEN_NAME('HBUK'), 1);
  Script_motmGetCharaFlags(CHARA_AKIR, 0, 0);
  Script_motmGetCharaFlags(CHARA_KISY, 0, 0);
  Script_SetMotnFlag(CHARA_AKIR, -1);
  Script_SetMotnFlag(CHARA_KISY, -1);
  Script_scenToggleActiveFor(CHARA_AKIR, 0);
  Script_scenToggleActiveFor(CHARA_KISY, 0);
  task_9E = 0;
  task_9F = 0;
  task_9D = 0;
  task_A0 = 0;
  task_A8 = Script_scenCreateTask(script_4489, 1, CHARA_KISY);
  task_AC = Script_scenCreateTask(script_463C, 1, CHARA_TERY);
  task_B0 = Script_scenCreateTask(script_4B67, 1, CHARA_STSI);
  task_A4 = Script_scenCreateTask(script_4158, 1, CHARA_AKIR);
  return;
}

static void script_3E43(void) {
  task_9C = 255;
  Script_executeAicaCommand(2720, 2, 30);
  Script_filmCheckOrSetTransitionUtil(60, 0, 0, 0, 0, 255, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_srchControl((void *)task_A4);
  Script_srchControl((void *)task_A8);
  Script_srchControl((void *)task_AC);
  Script_srchControl((void *)task_B0);
  Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 545, 1);
  Script_MoveObjectToObject(CHARA_AKIR, -1);
  Script_MoveObjectToObject(CHARA_KISY, -1);
  Script_MoveObjectToObject(CHARA_TERY, -1);
  Script_MoveObjectToObject(CHARA_STSI, -1);
  Script_charaToggleMotEnabled(CHARA_AKIR, 1);
  Script_ccowUtils(CHARA_AKIR, 1);
  Script_scenToggleActiveFor(CHARA_AKIR, 1);
  Script_scenToggleActiveFor(CHARA_KISY, 1);
  Script_FACE_LCG(CHARA_AKIR, 0, 0, 0);
  Script_FACE_LCG(CHARA_KISY, 0, 0, 0);
  Script_FACE_LCG(CHARA_TERY, 0, 0, 0);
  Script_PlayerStatsUtils(PS_Set70MBTimeCapSlot, 16, task_B8 + 1);
  Script_PlayerStatsUtils(PS_Set70MBCounter0, Script_PlayerStatsUtils(PS_Get70MBCounter0) + 1);
  if (task_B8 > 0) {
    Script_PlayerStatsUtils(PS_Set70MBCounter1, Script_PlayerStatsUtils(PS_Get70MBCounter1) + 1);
  }
  Script_SetSfxOverlayTimeoutTuning(2, SfxMode_SessionAction);
  Script_shenClearFreeState(0);
  return;
}

static void script_3F82(void) {
  task_9C = 255;
  task_B8 = task_B8 + 1;
  Script_filmCheckOrSetTransitionUtil(60, 0, 0, 0, 0, 255, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_sicnUtils(0, 1, 1);
  Script_charaDisplay(SHEN_NAME('HBUK'), CDM_Hide);
  Script_charaDisplay(CHARA_STSI, CDM_Hide);
  Script_charaDisplay(CHARA_TERY, CDM_Hide);
  SCNF_WaitFrames_0000(90);
  Script_sicnUtils(1, 0, 0);
  Script_SetSfxOverlayTimeoutTuning(1, SfxMode_SessionAction);
  task_9E = 0;
  task_9D = 0;
  task_A0 = 0;
  task_9F = 0;
  return;
}

static int32_t script_4027(void) {
  Script_SetCPAMFlags(CHARA_AKIR, 0);
  Script_SetCPAMFlags(CHARA_KISY, 0);
  Script_SetCPAMFlags(CHARA_TERY, 0);
  Script_seqEnterEvtCam(3010, SHEN_NAME('HDUA'), SHEN_NAME('HDUA'));
  SCNF_WaitFrames_0000(60);
  Script_FACE(CHARA_AKIR, 10, 0, 50);
  SCNF_WaitFrames_0000(60);
  Script_seqEnterEvtCam(3011, CHARA_KISY, CHARA_KISY);
  Script_FACE(CHARA_KISY, 10, 2, 50);
  SCNF_WaitFrames_0000(60);
  Script_seqEnterEvtCam(3012, CHARA_KISY, CHARA_KISY);
  return Script_srchControl(0);
}

static int32_t script_40BF(void) {
  Script_seqEnterEvtCam(3213, SHEN_NAME('HDUT'), SHEN_NAME('HDUT'));
  SCNF_WaitFrames_0000(75);
  Script_seqEnterEvtCam(3209, SHEN_NAME('HDUK'), SHEN_NAME('HDUK'));
  return Script_srchControl(0);
}

static int32_t script_40F2(void) {
  Script_seqEnterEvtCam(3026, SHEN_NAME('HDUO'), SHEN_NAME('HDUO'));
  SCNF_WaitFrames_0000(75);
  Script_seqEnterEvtCam(3216, SHEN_NAME('HDUK'), SHEN_NAME('HDUK'));
  SCNF_WaitFrames_0000(93);
  Script_seqEnterEvtCam(3215, SHEN_NAME('HDUK'), SHEN_NAME('HDUK'));
  return Script_srchControl(0);
}

static int32_t script_413E(void) {
  Script_seqEnterEvtCam(3027, SHEN_NAME('HDUO'), SHEN_NAME('HDUO'));
  return Script_srchControl(0);
}

static int32_t script_4158(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  for (;;) {
    local_FC = task_9D;
    switch (local_FC) {
      case 0:
        task_9D = 255;
        SCNF_WaitFrames_0000(2);
        task_84.x = 0.0f;
        task_84.y = 0.0f;
        task_84.z = 0.0f;
        Script_EVT_SetPosition(arg0, POS_XYZ, &task_84);
        task_90.y = 0;
        task_90.y = 0;
        task_90.z = 0;
        Script_EVT_SetRotation(arg0, 0x38000000, &task_90, 0);
        Script_charaToggleMotEnabled(arg0, 0);
        Script_PlayCharaMotion(arg0, 4102, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        Script_FACE(arg0, 4, 2, 0);
        Script_FaceLookAtTargetUtils(arg0, 0);
        Script_scenCreateTask(script_4027, 0);
        task_9D = 1;
        task_9E = 1;
        break;
      case 1:
        SCNF_WaitFrames_0000(1);
        Script_filmCheckOrSetTransitionUtil(60, 255, 0, 0, 0, 0, 0, 0, 0);
        while (Script_filmCheckOrSetTransitionUtil(-1)) {
          SCNF_WaitFrames_0000(1);
        }
        task_9D = 255;
        Script_ShowUIMessageFromID("E1045A001");
        SCNF_WaitFrames_0000(20);
        Script_ShowUIMessageFromID("E1045B001");
        while (Script_convGetIsActive(0)) {
          SCNF_WaitFrames_0000(1);
        }
        Script_ShowUIMessageFromID("E1045B002");
        while (Script_convGetIsActive(0)) {
          SCNF_WaitFrames_0000(1);
        }
        Script_ShowUIMessageFromID("E1045A002");
        while (Script_convGetIsActive(0)) {
          SCNF_WaitFrames_0000(1);
        }
        Script_seqEnterEvtCam(3200, SHEN_NAME('HDUA'), SHEN_NAME('HDUA'));
        task_9D = 2;
        task_9E = 2;
        break;
      case 2:
        task_9D = 255;
        local_F8 = Script_scenCreateTask(script_5EC5, 0);
        Script_scenCreateTask(script_57A8, 0);
        Script_scenCreateTask(script_5A25, 0);
        script_4D7E(arg0);
        Script_srchControl((void *)local_F8);
        task_9F = 1;
        task_A0 = 1;
        break;
      case 3:
        task_9D = 255;
        SCNF_WaitFrames_0000(10);
        Script_charaToggleMotEnabled(arg0, 1);
        Script_PlayCharaMotion(arg0, 4098, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        task_90.y = 0;
        task_90.y = 23665;
        task_90.z = 0;
        Script_EVT_SetRotation(arg0, 0x38000000, &task_90, 0);
        Script_SetCharaMotionPlaybackRate(arg0, 2.0f);
        break;
      case 4:
        task_9D = 255;
        Script_charaToggleMotEnabled(arg0, 1);
        Script_PlayCharaMotion(arg0, 4101, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        task_90.y = 0;
        task_90.y = 38229;
        task_90.z = 0;
        Script_EVT_SetRotation(arg0, 0x38000000, &task_90, 0);
        Script_SetCharaMotionPlaybackRate(arg0, 1.3f);
        SCNF_WaitFrames_0000(50);
        SCNF_WaitFrames_0000(20);
        Script_SetCharaMotionPlaybackRate(arg0, 0.6f);
        for (;;) {
          if (Script_IsCharaMotionFinished(arg0) != 0) {
            break;
          }
          SCNF_WaitFrames_0000(1);
        }
        break;
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_4489(int32_t arg0) {
  int32_t local_FC;
  for (;;) {
    local_FC = task_9E;
    switch (local_FC) {
      case 0:
        task_9E = 255;
        task_84.x = 0.1f;
        task_84.y = 0.0f;
        task_84.z = -1.89f;
        Script_EVT_SetPosition(arg0, POS_XYZ, &task_84);
        task_90.y = 0;
        task_90.y = 32747;
        task_90.z = 0;
        Script_EVT_SetRotation(arg0, 0x38000000, &task_90, 0);
        Script_charaToggleMotEnabled(arg0, 0);
        Script_PlayCharaMotion(arg0, 4105, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        Script_FACE(arg0, 4, 1, 0);
        Script_FaceLookAtTargetUtils(arg0, 0);
        Script_charaDisplay(arg0, CDM_Show);
        break;
      case 1:
        task_9E = 255;
        Script_charaDisplay(arg0, CDM_Show);
        break;
      case 2:
        task_9E = 255;
        script_527B(arg0);
        break;
      case 3:
        task_9E = 255;
        Script_charaToggleMotEnabled(arg0, 1);
        Script_PlayCharaMotion(arg0, 4107, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        task_90.y = 0;
        task_90.y = 47331;
        task_90.z = 0;
        Script_EVT_SetRotation(arg0, 0x38000000, &task_90, 0);
        Script_SetCharaMotionPlaybackRate(arg0, 1.3f);
        SCNF_WaitFrames_0000(50);
        SCNF_WaitFrames_0000(20);
        Script_SetCharaMotionPlaybackRate(arg0, 0.6f);
        for (;;) {
          if (Script_IsCharaMotionFinished(arg0) != 0) {
            break;
          }
          SCNF_WaitFrames_0000(1);
        }
        break;
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_463C(int32_t arg0) {
  int32_t local_FC;
  for (;;) {
    local_FC = task_9F;
    switch (local_FC) {
      case 0:
        {
          Vector3i local_F0;
          task_9F = 255;
          task_84.x = 0.7f;
          task_84.y = 0.0f;
          task_84.z = 4.05f;
          Script_EVT_SetPosition(arg0, POS_XYZ, &task_84);
          task_90.y = 0;
          task_90.y = 0;
          task_90.z = 0;
          Script_EVT_SetRotation(arg0, 0x38000000, &task_90, 0);
          (&local_F0)->y = 0x2000;
          Script_EVT_SetRotation(arg0, -0x70000000, &local_F0, 0);
          Script_charaToggleMotEnabled(arg0, 1);
          Script_PlayCharaMotion(arg0, 4108, -1.0f, -1.0f, -1.0f, 1.0f, 0);
          Script_SetCharaMotionPlaybackRate(arg0, 0.0f);
          Script_EVT_SetPosition(SHEN_NAME('HBUK'), POS_XYZ, &data_14FE4);
          task_90.y = 0;
          task_90.y = 0;
          task_90.z = 0;
          Script_EVT_SetRotation(SHEN_NAME('HBUK'), 0x38000000, &task_90, 0);
          Script_FaceLookAtTargetUtils(CHARA_TERY, 0);
          Script_ReadModelFormat7(arg0, 0, data_14FF0, 0);
          Script_ReadModelFormat7(arg0, 1, data_15018, 0);
          break;
        }
      case 1:
        task_9F = 255;
        Script_charaDisplay(arg0, CDM_Show);
        Script_charaDisplay(SHEN_NAME('HBUK'), CDM_Show);
        task_9F = 2;
        break;
      case 2:
        {
          int32_t local_F8;
          int32_t local_F4;
          Script_FACE(CHARA_TERY, 1, 0, 0);
          task_9F = 255;
          Script_scenCreateTask(script_40BF, 0);
          Script_scenCreateTask(script_4C72, 0);
          SCNF_WaitFrames_0000(50);
          Script_FACE(CHARA_TERY, 5, 0, 30);
          Script_charaToggleMotEnabled(arg0, 1);
          Script_PlayCharaMotion(arg0, 4108, -1.0f, -1.0f, -1.0f, 1.0f, 0);
          Script_SetCharaMotionPlaybackRate(arg0, 0.85f);
          local_F8 = Script_scenCreateTask(script_4A81, 0);
          Script_scenCreateTask(script_49CA, 0);
          SCNF_WaitFrames_0000(47);
          local_F4 = SCNF_UIUtils(4, 10, 2, 3, 1, 0);
          if (local_F4 != 0xFFFF) {
            if (local_F4 & 4) {
              Script_srchControl((void *)local_F8);
              task_9D = 4;
              task_9E = 3;
              task_A0 = 2;
              Script_fixoClearAttachment(SHEN_NAME('HBUK'));
              SCNF_WaitFrames_0000(1);
              Script_scenCreateTask(script_4AF9, 0);
              Script_scenCreateTask(script_40F2, 0);
              Script_scenCreateTask(script_4CA1, 0);
              SCNF_WaitFrames_0000(1);
              for (;;) {
                if (!(Script_GetCharaMotionRemainingFramesScaled(arg0) <= 92.0f)) {
                  break;
                }
                SCNF_WaitFrames_0000(1);
              }
              Script_executeAicaCommand(132265, 0, 0);
              SCNF_WaitFrames_0000(1);
              for (;;) {
                if (!(Script_GetCharaMotionRemainingFramesScaled(arg0) <= 100.0f)) {
                  break;
                }
                SCNF_WaitFrames_0000(1);
              }
              Script_fixoClearAttachment(SHEN_NAME('HBUK'));
              Script_scenCreateTask(script_4A30, 0);
            } else {
              SCNF_WaitFrames_0000(1);
              for (;;) {
                if (!(Script_GetCharaMotionRemainingFramesScaled(arg0) <= 81.0f)) {
                  break;
                }
                SCNF_WaitFrames_0000(1);
              }
              Script_scenCreateTask(script_413E, 0);
              SCNF_WaitFrames_0000(1);
              for (;;) {
                if (!(Script_GetCharaMotionRemainingFramesScaled(arg0) <= 92.0f)) {
                  break;
                }
                SCNF_WaitFrames_0000(1);
              }
              task_9D = 3;
              Script_executeAicaCommand(197801, 0, 0);
              Script_SetCharaMotionPlaybackRate(arg0, 0.0f);
              Script_scenCreateTask(script_4D57, 0);
              Script_fixoClearAttachment(SHEN_NAME('HBUK'));
              task_9C = 3;
            }
          }
          break;
        }
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_49CA(void) {
  int8_t local_FF;
  local_FF = 0;
  for (;;) {
    if (!(Script_GetCharaMotionRemainingFramesScaled(CHARA_TERY) <= 33.0f)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  for (;;) {
    local_FF = local_FF + 1;
    Script_EVT_SetRotation(SHEN_NAME('HBUK'), -0x48000000, &data_15040, 0);
    if (local_FF == 28) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  return Script_srchControl(0);
}

static int32_t script_4A30(void) {
  int8_t local_FF;
  local_FF = 0;
  for (;;) {
    local_FF = local_FF + 1;
    SCNF_WaitFrames_0000(1);
    Script_EVT_SetRotation(SHEN_NAME('HBUK'), -0x48000000, &data_1504C, 0);
    if (local_FF == 18) {
      break;
    }
  }
  Script_executeAicaCommand(328873, 0, 0);
  return Script_srchControl(0);
}

static int32_t script_4A81(void) {
  for (;;) {
    if (!(Script_GetCharaMotionRemainingFramesScaled(CHARA_TERY) <= 57.0f)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_motmGetCharaFlags(CHARA_TERY, 0, 0);
  Script_ccowUtils(CHARA_TERY, 2);
  Script_motmToggleFlags(CHARA_TERY, 2, 0x40);
  Script_fixoAttachEx(SHEN_NAME('HBUK'), CHARA_TERY, 18, &data_15058, &data_15064);
  Script_motmToggleFlags(CHARA_TERY, 1, 0x40);
  return Script_srchControl(0);
}

static int32_t script_4AF9(void) {
  Script_motmGetCharaFlags(CHARA_TERY, 0, 0);
  Script_ccowUtils(CHARA_TERY, 2);
  Script_motmToggleFlags(CHARA_TERY, 2, 0x40);
  Script_fixoAttachEx(SHEN_NAME('HBUK'), CHARA_TERY, 18, &data_15070, &data_1507C);
  Script_motmToggleFlags(CHARA_TERY, 1, 0x40);
  Script_CharHandlers(CHARA_TERY, 2, 8, 16);
  SCNF_WaitFrames_0000(1);
  return Script_srchControl(0);
}

static int32_t script_4B67(int32_t arg0) {
  int32_t local_FC;
  Vector3i local_F0;
  for (;;) {
    local_FC = task_A0;
    switch (local_FC) {
      case 0:
        task_A0 = 255;
        task_84.x = 2.90942f;
        task_84.y = 0.0f;
        task_84.z = 5.7035f;
        Script_EVT_SetPosition(arg0, POS_XYZ, &task_84);
        task_90.y = 0;
        task_90.y = 4551;
        task_90.z = 0;
        Script_EVT_SetRotation(arg0, 0x38000000, &task_90, 0);
        Script_PlayCharaMotion(arg0, MN_AKI_AKI_UDEKUMI_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        for (;;) {
          if (Script_IsCharaMotionFinished(arg0)) {
            break;
          }
          Script_RotateNodeUtils(arg0, &data_13524, 0);
          SCNF_WaitFrames_0000(1);
        }
        break;
      case 1:
        task_A0 = 255;
        Script_charaDisplay(arg0, CDM_Show);
        break;
      case 2:
        (&local_F0)->y = 0x2000;
        Script_EVT_SetRotation(arg0, MATRIX_XFORM_Y, &local_F0, 0);
        break;
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_4C72(void) {
  SCNF_WaitFrames_0000(15);
  Script_ShowUIMessageFromID("E1045C002");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  return Script_srchControl(0);
}

static int32_t script_4CA1(void) {
  Script_ShowUIMessageFromID("E1045A004");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  SCNF_WaitFrames_0000(20);
  SCNF_WaitFrames_0000(100);
  Script_ShowUIMessageFromID("E1045B004");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_ShowUIMessageFromID("E1045A005");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_ShowUIMessageFromID("E1045B005");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_ShowUIMessageFromID("E1045A006");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  task_9C = 1;
  return Script_srchControl(0);
}

static int32_t script_4D57(void) {
  Script_ShowUIMessageFromID("E1045A007");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  return Script_srchControl(0);
}

static void script_4D7E(int32_t arg0) {
  Script_CharHandlers(arg0, 2, 0, 16);
  Script_charaToggleMotEnabled(arg0, 1);
  Script_SetFightProgramStateBytes(arg0, 3, 2);
  Script_PlayCharaMotion(arg0, 4097, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_motmToggleFlags(arg0, 2, 0x40);
  Script_SetCharaMotionPlaybackRate(arg0, 0.8f);
  SCNF_WaitFrames_0000(1);
  for (;;) {
    if (!(task_B4 <= 21.0f)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_GetDataFromFightProgram(arg0, 6);
  SCNF_WaitFrames_0000(1);
  for (;;) {
    if (!(task_B4 <= 30.0f)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_GetDataFromFightProgram(arg0, 0);
  SCNF_WaitFrames_0000(1);
  for (;;) {
    if (!(task_B4 <= 43.0f)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_GetDataFromFightProgram(arg0, 4);
  SCNF_WaitFrames_0000(1);
  for (;;) {
    if (!(task_B4 <= 54.0f)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_GetDataFromFightProgram(arg0, 0);
  SCNF_WaitFrames_0000(1);
  for (;;) {
    if (!(task_B4 <= 60.0f)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_GetDataFromFightProgram(arg0, 5);
  SCNF_WaitFrames_0000(1);
  for (;;) {
    if (!(task_B4 <= 71.0f)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_GetDataFromFightProgram(arg0, 0);
  Script_GetDataFromFightProgram(arg0, 4);
  SCNF_WaitFrames_0000(1);
  for (;;) {
    if (!(task_B4 <= 86.0f)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_GetDataFromFightProgram(arg0, 0);
  SCNF_WaitFrames_0000(1);
  for (;;) {
    if (!(task_B4 <= 97.0f)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_GetDataFromFightProgram(arg0, 5);
  SCNF_WaitFrames_0000(1);
  for (;;) {
    if (!(task_B4 <= 108.0f)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_GetDataFromFightProgram(arg0, 0);
  SCNF_WaitFrames_0000(1);
  for (;;) {
    if (!(task_B4 <= 117.0f)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_PlayCharaMotion(arg0, 4097, 115.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_SetCharaMotionPlaybackRate(arg0, 0.2f);
  Script_SetFightProgramStateBytes(arg0, 1, 1);
  SCNF_WaitFrames_0000(4);
  Script_GetDataFromFightProgram(arg0, 2);
  SCNF_WaitFrames_0000(1);
  for (;;) {
    if (!(task_B4 <= 120.0f)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_GetDataFromFightProgram(arg0, 0);
  Script_PlayCharaMotion(arg0, 4097, 107.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_SetCharaMotionPlaybackRate(arg0, 0.8f);
  SCNF_WaitFrames_0000(1);
  Script_SetFightProgramStateBytes(arg0, 3, 2);
  SCNF_WaitFrames_0000(1);
  for (;;) {
    if (!(task_B4 <= 131.0f)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_GetDataFromFightProgram(arg0, 6);
  SCNF_WaitFrames_0000(1);
  for (;;) {
    if (!(task_B4 <= 139.0f)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_GetDataFromFightProgram(arg0, 0);
  SCNF_WaitFrames_0000(1);
  for (;;) {
    if (!(task_B4 <= 140.0f)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_PlayCharaMotion(arg0, 4097, 128.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_EVT_SetPosition(CHARA_AKIR, -0x48000000, &data_150D0);
  Script_SetCharaMotionPlaybackRate(arg0, 0.3f);
  Script_GetDataFromFightProgram(arg0, 6);
  SCNF_WaitFrames_0000(1);
  SCNF_WaitFrames_0000(1);
  for (;;) {
    if (!(task_B4 <= 142.0f)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_GetDataFromFightProgram(arg0, 0);
  SCNF_WaitFrames_0000(1);
  for (;;) {
    if (!(task_B4 <= 153.0f)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &data_150DC);
  Script_SetCharaMotionPlaybackRate(arg0, 0.8f);
  Script_SetRyoMotionDetails(arg0);
  for (;;) {
    if (Script_IsCharaMotionFinished(arg0) != 0) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_motmToggleFlags(arg0, 1, 0x40);
  Script_charaToggleMotEnabled(arg0, 0);
  Script_PlayCharaMotion(arg0, 4099, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  for (;;) {
    if (Script_IsCharaMotionFinished(arg0)) {
      break;
    }
    Script_RotateNodeUtils(arg0, &data_13524, 0);
    SCNF_WaitFrames_0000(1);
  }
  Script_ShowUIMessageFromID("E1045A003");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_ShowUIMessageFromID("E1045B003");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &data_150E8);
  Script_EVT_SetPosition(CHARA_KISY, POS_XYZ, &data_150F4);
  return;
}

static int32_t script_524C(void) {
  int32_t unused_F4[3];
  Script_motiSetAngFlags(CHARA_KISY, 1);
  for (;;) {
    Script_SetMotnFlags(CHARA_KISY, 11, &data_15114, 0x78000000);
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_527B(int32_t arg0) {
  int32_t local_FC;
  Script_handSetFloatsFor(CHARA_KISY, 1, data_1481C, 0);
  Script_handSetFloatsFor(CHARA_KISY, 0, data_1481C, 0);
  Script_CharHandlers(arg0, 2, 0, 16);
  Script_SetFightProgramStateBytes(arg0, 2, 1);
  Script_charaToggleMotEnabled(arg0, 1);
  Script_PlayCharaMotion(arg0, 4103, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_motmToggleFlags(arg0, 2, 0x40);
  Script_SetCharaMotionPlaybackRate(arg0, 0.8f);
  SCNF_WaitFrames_0000(1);
  for (;;) {
    if (!(task_B4 <= 21.0f)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_GetDataFromFightProgram(arg0, 6);
  SCNF_WaitFrames_0000(1);
  for (;;) {
    if (!(task_B4 <= 31.0f)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_GetDataFromFightProgram(arg0, 0);
  SCNF_WaitFrames_0000(1);
  for (;;) {
    if (!(task_B4 <= 43.0f)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_GetDataFromFightProgram(arg0, 4);
  SCNF_WaitFrames_0000(1);
  for (;;) {
    if (!(task_B4 <= 55.0f)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_GetDataFromFightProgram(arg0, 0);
  SCNF_WaitFrames_0000(1);
  for (;;) {
    if (!(task_B4 <= 57.0f)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_GetDataFromFightProgram(arg0, 5);
  SCNF_WaitFrames_0000(1);
  for (;;) {
    if (!(task_B4 <= 69.0f)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_GetDataFromFightProgram(arg0, 0);
  SCNF_WaitFrames_0000(1);
  for (;;) {
    if (!(task_B4 <= 81.0f)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_GetDataFromFightProgram(arg0, 5);
  SCNF_WaitFrames_0000(1);
  for (;;) {
    if (!(task_B4 <= 92.0f)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_GetDataFromFightProgram(arg0, 0);
  SCNF_WaitFrames_0000(1);
  Script_GetDataFromFightProgram(arg0, 2);
  SCNF_WaitFrames_0000(1);
  for (;;) {
    if (!(task_B4 <= 107.0f)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_GetDataFromFightProgram(arg0, 7);
  SCNF_WaitFrames_0000(1);
  for (;;) {
    if (!(task_B4 <= 116.0f)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_GetDataFromFightProgram(arg0, 0);
  SCNF_WaitFrames_0000(1);
  for (;;) {
    if (!(task_B4 <= 117.0f)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_EVT_SetPosition(CHARA_KISY, -0x48000000, &data_15120);
  Script_PlayCharaMotion(arg0, 4103, 115.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_SetCharaMotionPlaybackRate(arg0, 0.2f);
  SCNF_WaitFrames_0000(1);
  Script_GetDataFromFightProgram(arg0, 7);
  SCNF_WaitFrames_0000(1);
  for (;;) {
    if (!(task_B4 <= 119.0f)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_GetDataFromFightProgram(arg0, 0);
  SCNF_WaitFrames_0000(1);
  for (;;) {
    if (!(task_B4 <= 120.0f)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_EVT_SetPosition(CHARA_KISY, -0x48000000, &data_1512C);
  Script_PlayCharaMotion(arg0, 4103, 107.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_SetCharaMotionPlaybackRate(arg0, 0.8f);
  SCNF_WaitFrames_0000(1);
  Script_GetDataFromFightProgram(arg0, 7);
  SCNF_WaitFrames_0000(1);
  for (;;) {
    if (!(task_B4 <= 123.0f)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_GetDataFromFightProgram(arg0, 0);
  SCNF_WaitFrames_0000(1);
  for (;;) {
    if (!(task_B4 <= 140.0f)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  SCNF_WaitFrames_0000(1);
  Script_PlayCharaMotion(arg0, 4103, 128.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_SetCharaMotionPlaybackRate(arg0, 0.3f);
  SCNF_WaitFrames_0000(5);
  SCNF_WaitFrames_0000(1);
  for (;;) {
    if (!(task_B4 <= 136.0f)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_SetFightProgramStateBytes(arg0, 1, 1);
  SCNF_WaitFrames_0000(1);
  for (;;) {
    if (!(task_B4 <= 153.0f)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_SetCharaMotionPlaybackRate(arg0, 0.8f);
  Script_CharHandlers(arg0, 2, 8, 16);
  Script_handSetFloatsFor(CHARA_KISY, 1, data_149E4, 0);
  Script_handSetFloatsFor(CHARA_KISY, 0, data_14900, 0);
  for (;;) {
    if (!(Script_GetCharaMotionRemainingFramesScaled(arg0) <= 290.0f)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  local_FC = Script_scenCreateTask(script_524C, 0);
  for (;;) {
    if (!(Script_GetCharaMotionRemainingFramesScaled(arg0) <= 360.0f)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_srchControl((void *)local_FC);
  for (;;) {
    if (Script_IsCharaMotionFinished(arg0) != 0) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_motmToggleFlags(arg0, 1, 0x40);
  Script_charaToggleMotEnabled(arg0, 0);
  Script_PlayCharaMotion(arg0, 4104, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  for (;;) {
    if (Script_IsCharaMotionFinished(arg0)) {
      break;
    }
    Script_RotateNodeUtils(arg0, &data_13524, 0);
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static int32_t script_57A8(void) {
  SCNF_WaitFrames_0000(5);
  SCNF_WaitFrames_0000(1);
  for (;;) {
    if (!(task_B4 <= 17.0f)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_seqEnterEvtCam(3201, SHEN_NAME('HDUA'), SHEN_NAME('HDUA'));
  SCNF_WaitFrames_0000(1);
  for (;;) {
    if (!(task_B4 <= 25.0f)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_seqEnterEvtCam(3207, SHEN_NAME('HDUA'), SHEN_NAME('HDUA'));
  SCNF_WaitFrames_0000(1);
  for (;;) {
    if (!(task_B4 <= 35.0f)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_seqEnterEvtCam(3203, SHEN_NAME('HDUK'), SHEN_NAME('HDUK'));
  SCNF_WaitFrames_0000(1);
  for (;;) {
    if (!(task_B4 <= 54.0f)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_seqEnterEvtCam(3205, SHEN_NAME('HDUK'), SHEN_NAME('HDUK'));
  SCNF_WaitFrames_0000(1);
  for (;;) {
    if (!(task_B4 <= 71.0f)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_seqEnterEvtCam(3217, SHEN_NAME('HDUA'), SHEN_NAME('HDUA'));
  SCNF_WaitFrames_0000(1);
  for (;;) {
    if (!(task_B4 <= 92.0f)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_seqEnterEvtCam(3204, SHEN_NAME('HDUK'), SHEN_NAME('HDUK'));
  SCNF_WaitFrames_0000(1);
  for (;;) {
    if (!(task_B4 <= 117.0f)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_seqEnterEvtCam(3300, CHARA_AKIR, CHARA_AKIR);
  SCNF_WaitFrames_0000(1);
  for (;;) {
    if (!(task_B4 <= 120.0f)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_seqEnterEvtCam(3202, SHEN_NAME('HDUA'), SHEN_NAME('HDUA'));
  SCNF_WaitFrames_0000(1);
  for (;;) {
    if (!(task_B4 <= 134.0f)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_seqEnterEvtCam(3203, SHEN_NAME('HDUK'), SHEN_NAME('HDUK'));
  SCNF_WaitFrames_0000(1);
  for (;;) {
    if (!(task_B4 <= 140.0f)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_seqEnterEvtCam(3206, SHEN_NAME('HDUK'), SHEN_NAME('HDUK'));
  SCNF_WaitFrames_0000(1);
  for (;;) {
    if (!(task_B4 <= 153.0f)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_seqEnterEvtCam(3210, SHEN_NAME('HDUT'), SHEN_NAME('HDUT'));
  for (;;) {
    if (Script_IsCharaMotionFinished(CHARA_AKIR) != 0) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_seqEnterEvtCam(3211, SHEN_NAME('HDUA'), SHEN_NAME('HDUA'));
  return Script_srchControl(0);
}

static int32_t script_5A25(void) {
  SCNF_WaitFrames_0000(5);
  SCNF_WaitFrames_0000(1);
  for (;;) {
    if (!(task_B4 <= 23.0f)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_executeAicaCommand(0x2076AB, 0, 0);
  SCNF_WaitFrames_0000(1);
  for (;;) {
    if (!(task_B4 <= 29.0f)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_executeAicaCommand(161451, 0, 0);
  SCNF_WaitFrames_0000(1);
  for (;;) {
    if (!(task_B4 <= 46.0f)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_executeAicaCommand(0x1F76AB, 0, 0);
  SCNF_WaitFrames_0000(1);
  for (;;) {
    if (!(task_B4 <= 50.0f)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_executeAicaCommand(30379, 0, 0);
  SCNF_WaitFrames_0000(1);
  for (;;) {
    if (!(task_B4 <= 64.0f)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_executeAicaCommand(0x1976AB, 0, 0);
  SCNF_WaitFrames_0000(1);
  for (;;) {
    if (!(task_B4 <= 70.0f)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_executeAicaCommand(0x1476AB, 0, 0);
  SCNF_WaitFrames_0000(1);
  for (;;) {
    if (!(task_B4 <= 80.0f)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_executeAicaCommand(0x1F76AB, 0, 0);
  SCNF_WaitFrames_0000(1);
  for (;;) {
    if (!(task_B4 <= 82.0f)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_executeAicaCommand(0x1576AB, 0, 0);
  SCNF_WaitFrames_0000(1);
  for (;;) {
    if (!(task_B4 <= 100.0f)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_executeAicaCommand(0x1A76AB, 0, 0);
  SCNF_WaitFrames_0000(1);
  for (;;) {
    if (!(task_B4 <= 108.0f)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_executeAicaCommand(0x2076AB, 0, 0);
  SCNF_WaitFrames_0000(1);
  for (;;) {
    if (!(task_B4 <= 115.0f)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_executeAicaCommand(161451, 0, 0);
  Script_executeAicaCommand(817067, 0, 0);
  SCNF_WaitFrames_0000(1);
  for (;;) {
    if (!(task_B4 <= 117.0f)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  SCNF_WaitFrames_0000(1);
  for (;;) {
    if (!(task_B4 <= 117.0f)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_executeAicaCommand(161451, 0, 0);
  Script_executeAicaCommand(817067, 0, 0);
  SCNF_WaitFrames_0000(1);
  for (;;) {
    if (!(task_B4 <= 120.0f)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  SCNF_WaitFrames_0000(1);
  for (;;) {
    if (!(task_B4 <= 120.0f)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_executeAicaCommand(0x2076AB, 0, 0);
  SCNF_WaitFrames_0000(1);
  for (;;) {
    if (!(task_B4 <= 115.0f)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_executeAicaCommand(161451, 0, 0);
  Script_executeAicaCommand(817067, 0, 0);
  SCNF_WaitFrames_0000(1);
  for (;;) {
    if (!(task_B4 <= 131.0f)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_executeAicaCommand(0x2076AB, 0, 0);
  SCNF_WaitFrames_0000(1);
  for (;;) {
    if (!(task_B4 <= 134.0f)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_executeAicaCommand(161451, 0, 0);
  Script_executeAicaCommand(161451, 0, 0);
  Script_executeAicaCommand(685995, 0, 0);
  SCNF_WaitFrames_0000(1);
  for (;;) {
    if (!(task_B4 <= 140.0f)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  SCNF_WaitFrames_0000(1);
  for (;;) {
    if (!(task_B4 <= 131.0f)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_executeAicaCommand(0x2076AB, 0, 0);
  SCNF_WaitFrames_0000(1);
  for (;;) {
    if (!(task_B4 <= 136.0f)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_executeAicaCommand(161451, 0, 0);
  Script_executeAicaCommand(161451, 0, 0);
  Script_executeAicaCommand(685995, 0, 0);
  SCNF_WaitFrames_0000(1);
  for (;;) {
    if (!(task_B4 <= 142.0f)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_executeAicaCommand(0x2776AB, 0, 0);
  SCNF_WaitFrames_0000(1);
  for (;;) {
    if (!(task_B4 <= 153.0f)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_executeAicaCommand(0x2776AB, 0, 0);
  Script_executeAicaCommand(0x2776AB, 0, 0);
  return Script_srchControl(0);
}

static int32_t script_5EC5(void) {
  for (;;) {
    task_B4 = Script_GetCharaMotionRemainingFramesScaled(CHARA_AKIR);
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_5EE2(int32_t arg0) {
  int32_t local_FC;
  int32_t unused_F8;
  int32_t local_F4;
  Vector3 local_E8;
  Vector3 local_DC;
  Vector3 local_D0;
  int32_t unused_FF70[24];
  int32_t local_FF6C;
  script_7630();
  Script_scenCreateTask(script_7E7A, 0);
  Script_shenSeqUtils(0);
  local_FC = 0;
  local_F4 = 0;
  while (Script_shenSeqUtils(-1)) {
    {
      int32_t local_FF68;
      local_FF68 = local_FC;
      switch (local_FF68) {
        case 0:
          Script_SetMotnFlag(CHARA_AKIR, -1);
          Script_SetMotnFlag(CHARA_KISY, -1);
          Script_SetMotnFlag(CHARA_STSI, -1);
          Script_charaVisUtils(CHARA_AKIR, 1);
          Script_charaVisUtils(CHARA_STSI, 1);
          Script_charaVisUtils(CHARA_KISY, 1);
          Script_FaceEx(CHARA_AKIR, 1, 0, 1);
          Script_ResetHandMotion(CHARA_AKIR, 1, 1, 0);
          Script_ResetHandMotion(CHARA_AKIR, 1, 0, 0);
          Script_handSetFloatsFor(CHARA_AKIR, 1, data_15180, 1);
          Script_handSetFloatsFor(CHARA_AKIR, 0, data_15180, 1);
          Script_SetHandRootRotation(CHARA_AKIR, 1, 0x15, &data_1542C);
          Script_SetHandRootRotation(CHARA_AKIR, 0, 0x15, &data_1542C);
          Script_SetShadowRange(300.0f);
          Script_charaDisplay(CHARA_KISY, CDM_Hide);
          Script_ResetHandMotion(CHARA_STSI, 1, 1, 0);
          Script_ResetHandMotion(CHARA_STSI, 1, 0, 0);
          Script_handSetFloatsFor(CHARA_STSI, 1, data_13D4C, 1);
          Script_handSetFloatsFor(CHARA_STSI, 0, data_13D4C, 1);
          Script_mapmSetCamFlags(0, 0);
          break;
        case 180:
          Script_charaVisUtils(CHARA_AKIR, 0);
          Script_charaVisUtils(CHARA_STSI, 0);
          Script_mapmSetCamFlags(0, 1);
          break;
        case 450:
          Script_FaceEx(CHARA_AKIR, 0, 0, 1);
          Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
          Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
          break;
        case 743:
          Script_FaceEx(CHARA_AKIR, 1, 0, 1);
          Script_ResetHandMotion(CHARA_AKIR, 1, 1, 0);
          Script_ResetHandMotion(CHARA_AKIR, 1, 0, 0);
          Script_handSetFloatsFor(CHARA_AKIR, 1, data_15180, 1);
          Script_handSetFloatsFor(CHARA_AKIR, 0, data_15180, 1);
          break;
        case 1113:
          Script_FACE(CHARA_AKIR, 2, 0, 10);
          break;
        case 1101:
          script_7DD1(task_BC);
          script_7DD1(task_C0);
          script_7DD1(task_C4);
          Script_EffectUtils(1, task_BC);
          Script_EffectUtils(1, task_C0);
          Script_EffectUtils(1, task_C4);
          break;
        case 1138:
          Script_CharHandlers(CHARA_KISY, 1, 0, 16);
          Script_CharHandlers(CHARA_KISY, 0, 0, 16);
          script_7D3F();
          Script_charaDisplay(CHARA_KISY, CDM_Show);
          Script_lightEnablePointLightAt(11, &local_D0);
          break;
        case 1310:
          Script_FaceEx(CHARA_STSI, 0, 0, 0);
          Script_ResetHandMotion(CHARA_STSI, 0, 1, 0);
          Script_ResetHandMotion(CHARA_STSI, 0, 0, 0);
          Script_FACE(CHARA_AKIR, 7, 0, 20);
          break;
        case 1322:
          Script_FaceEx(CHARA_KISY, 1, 0, 1);
          break;
        case 1324:
          Script_FACE(CHARA_AKIR, 0, 0, 20);
          Script_FACE(CHARA_KISY, 7, 0, 20);
          Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
          Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
          Script_CharHandlers(CHARA_AKIR, 1, 0, 16);
          Script_CharHandlers(CHARA_AKIR, 0, 0, 16);
          break;
        case 1484:
          Script_SetupLightFromTable(4);
          Script_EffectUtils(2, task_BC);
          Script_EffectUtils(2, task_C0);
          Script_EffectUtils(2, task_C4);
          Script_FaceEx(CHARA_AKIR, 0, 0, 1);
          Script_FaceEx(CHARA_KISY, 0, 0, 1);
          Script_FaceEx(CHARA_AKIR, 1, 0, 1);
          Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 120.0f);
          Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 120.0f);
          break;
        case 1567:
          Script_FaceEx(CHARA_KISY, 1, 0, 1);
          Script_ResetHandMotion(CHARA_KISY, 1, 1, 0);
          Script_ResetHandMotion(CHARA_KISY, 1, 0, 0);
          Script_handSetFloatsFor(CHARA_KISY, 1, data_15180, 1);
          Script_handSetFloatsFor(CHARA_KISY, 0, data_15180, 1);
          Script_SetHandRootRotation(CHARA_KISY, 1, 0x15, &data_15438);
          Script_SetHandRootRotation(CHARA_KISY, 0, 0x15, &data_15438);
          Script_ResetHandMotion(CHARA_AKIR, 1, 1, 0);
          Script_ResetHandMotion(CHARA_AKIR, 1, 0, 0);
          Script_FaceEx(CHARA_AKIR, 1, 0, 1);
          Script_handSetFloatsFor(CHARA_AKIR, 1, data_13D4C, 1);
          Script_handSetFloatsFor(CHARA_AKIR, 0, data_13D4C, 1);
          break;
        case 1570:
          Script_handSetFloatsFor(CHARA_AKIR, 1, data_15264, 20);
          Script_handSetFloatsFor(CHARA_AKIR, 0, data_15264, 20);
          break;
        case 1740:
          Script_FACE(CHARA_AKIR, 2, 0, 10);
          break;
        case 1630:
          Script_FACE(CHARA_KISY, 2, 0, 30);
          break;
        case 1514:
          Script_mapmSetCamFlags(0, 0);
          break;
        case 1870:
          Script_handSetFloatsFor(CHARA_KISY, 1, data_149E4, 70);
          Script_handSetFloatsFor(CHARA_KISY, 0, data_14900, 70);
          break;
        case 1900:
          break;
        case 1100:
          Script_SetShadowFlags(CHARA_KISY, SCRIPT_SHADOW_UPDATE_LIGHT_MASK, 11);
          break;
        case 600:
          Script_SetShadowFlags(CHARA_AKIR, SCRIPT_SHADOW_UPDATE_LIGHT_MASK, 11);
          Script_SetShadowFlags(CHARA_STSI, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
          Script_SetShadowFlags(CHARA_STSI, SCRIPT_SHADOW_UPDATE_LIGHT_MASK, 11);
          Script_GetLightPosition(11, &local_E8);
          break;
        case 1300:
          Script_SetShadowFlags(CHARA_KISY, SCRIPT_SHADOW_UPDATE_LIGHT_MASK_0, 11);
          Script_SetShadowFlags(CHARA_AKIR, SCRIPT_SHADOW_UPDATE_LIGHT_MASK_0, 11);
          Script_SetShadowFlags(CHARA_STSI, SCRIPT_SHADOW_UPDATE_LIGHT_MASK_0, 11);
          break;
        case 800:
          Script_GetLightPosition(11, &local_DC);
          local_D0.x = (&local_DC)->x;
          (&local_D0)->y = (&local_DC)->y;
          (&local_D0)->z = (&local_DC)->z;
          local_DC.x = (&local_DC)->x - 3.0f;
          Script_lightEnablePointLightAt(11, &local_DC);
          break;
      }
    }
    if (local_FC == 650) {
      Script_mapmSetCamFlags(0, 1);
    }
    if (local_FC == 516) {
      Script_mapmSetCamFlags(0, 0);
    }
    if ((local_FC < 1484) & (local_FC >= 1102)) {
      int32_t local_FF68;
      local_FF6C = local_FC % 3;
      local_FF68 = local_FF6C;
      switch (local_FF68) {
        case 0:
          Script_EffectUtils(5, task_BC, &data_15138, &task_C8);
          break;
        case 1:
          Script_EffectUtils(5, task_C0, &data_15150, &task_C8);
          break;
        case 2:
          Script_EffectUtils(5, task_C4, &data_15168, &task_C8);
          break;
      }
    }
    local_FC = local_FC + 1;
    local_F4 = local_F4 + 1;
    SCNF_WaitFrames_0000(1);
  }
  Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  script_7B1C();
  return;
}

static void script_6685(int32_t arg0) {
  int32_t local_FC;
  Vector3 local_F0;
  Vector3 local_E4;
  int32_t local_E0;
  script_0030();
  script_780F();
  Script_scenCreateTask(script_7E7A, 0);
  Script_shenSeqUtils(0);
  local_FC = 0;
  while (Script_shenSeqUtils(-1)) {
    local_E0 = local_FC;
    switch (local_E0) {
      case 0:
        Script_SetMotnFlag(CHARA_AKIR, -1);
        Script_SetMotnFlag(CHARA_KISY, -1);
        Script_ResetHandMotion(CHARA_TERY, 1, 1, 0);
        Script_ResetHandMotion(CHARA_TERY, 1, 0, 0);
        Script_ResetHandMotion(CHARA_AKIR, 1, 1, 0);
        Script_ResetHandMotion(CHARA_AKIR, 1, 0, 0);
        Script_ResetHandMotion(CHARA_KISY, 1, 1, 0);
        Script_ResetHandMotion(CHARA_KISY, 1, 0, 0);
        Script_handSetFloatsFor(CHARA_AKIR, 1, data_15444, 0);
        Script_handSetFloatsFor(CHARA_AKIR, 0, data_15444, 0);
        Script_handSetFloatsFor(CHARA_KISY, 0, data_14900, 0);
        Script_handSetFloatsFor(CHARA_KISY, 1, data_149E4, 0);
        Script_handSetFloatsFor(CHARA_TERY, 0, data_13D4C, 0);
        Script_handSetFloatsFor(CHARA_TERY, 1, data_13D4C, 0);
        Script_FACE(CHARA_AKIR, 5, 0, 0);
        Script_FACE(CHARA_KISY, 5, 0, 0);
        Script_FACE(CHARA_TERY, 2, 0, 0);
        local_E4.x = -1.5368f;
        (&local_E4)->y = 1.5346f;
        (&local_E4)->z = 93.963699f;
        Script_FaceLookAtTargetUtils(CHARA_AKIR, 2, &local_E4, 1);
        Script_FaceLookAtTargetUtils(CHARA_KISY, 2, &local_E4, 1);
        local_F0.x = -1.5368f;
        (&local_F0)->y = 1.5346f;
        (&local_F0)->z = 93.963699f;
        Script_GetLightPosition(11, &local_F0);
        break;
      case 200:
        Script_FaceLookAtTargetUtils(CHARA_AKIR, 0);
        Script_FaceLookAtTargetUtils(CHARA_KISY, 0);
        break;
      case 275:
        Script_FACE(CHARA_TERY, 1, 0, 10);
        break;
      case 317:
        Script_handSetFloatsFor(CHARA_AKIR, 1, data_15180, 20);
        Script_handSetFloatsFor(CHARA_AKIR, 0, data_15180, 20);
        Script_handSetFloatsFor(CHARA_KISY, 1, data_15180, 20);
        Script_handSetFloatsFor(CHARA_KISY, 0, data_15180, 20);
        break;
      case 120:
        break;
      case 130:
        Script_handSetFloatsFor(CHARA_TERY, 0, data_15528, 20);
        Script_handSetFloatsFor(CHARA_TERY, 1, data_15528, 20);
        break;
      case 170:
        Script_handSetFloatsFor(CHARA_TERY, 0, data_13D4C, 20);
        Script_handSetFloatsFor(CHARA_TERY, 1, data_13D4C, 20);
        break;
      case 180:
        Script_handSetFloatsFor(CHARA_TERY, 1, data_1560C, 20);
        break;
      case 220:
        Script_handSetFloatsFor(CHARA_TERY, 1, data_156F0, 20);
        break;
      case 266:
        Script_handSetFloatsFor(CHARA_TERY, 1, data_157D4, 15);
        break;
      case 285:
        Script_handSetFloatsFor(CHARA_TERY, 0, data_13D4C, 20);
        Script_handSetFloatsFor(CHARA_TERY, 1, data_13D4C, 20);
        break;
      case 295:
        Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 80.0f);
        Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 80.0f);
        Script_mapmSetCamFlags(0, 0);
        break;
      case 315:
        Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 90.0f);
        Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 90.0f);
        break;
      case 352:
        Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 100.0f);
        Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 100.0f);
        Script_mapmSetCamFlags(0, 0);
        break;
      case 400:
        break;
    }
    if (!((&local_F0)->x - 0.1f <= (&local_E4)->x)) {
      local_E4.x = (&local_E4)->x + 0.1f;
    } else if (!((&local_F0)->x - 0.01f <= (&local_E4)->x)) {
      local_E4.x = (&local_E4)->x + 0.01f;
    } else {
      if (!((&local_F0)->x - 0.001f <= (&local_E4)->x)) {
        local_E4.x = (&local_E4)->x + 0.001f;
        goto L006AC0;
      }
    }
    L006AC0:;
    if (!((&local_F0)->x + 0.1f >= (&local_E4)->x)) {
      local_E4.x = (&local_E4)->x - 0.1f;
    } else if (!((&local_F0)->x + 0.01f >= (&local_E4)->x)) {
      local_E4.x = (&local_E4)->x - 0.01f;
    } else {
      if (!((&local_F0)->x + 0.001f >= (&local_E4)->x)) {
        local_E4.x = (&local_E4)->x - 0.001f;
        goto L006B23;
      }
    }
    L006B23:;
    if (!((&local_F0)->y - 0.1f <= (&local_E4)->y)) {
      (&local_E4)->y = (&local_E4)->y + 0.1f;
    } else if (!((&local_F0)->y - 0.01f <= (&local_E4)->y)) {
      (&local_E4)->y = (&local_E4)->y + 0.01f;
    } else {
      if (!((&local_F0)->y - 0.001f <= (&local_E4)->y)) {
        (&local_E4)->y = (&local_E4)->y + 0.001f;
        goto L006BAA;
      }
    }
    L006BAA:;
    if (!((&local_F0)->y + 0.1f >= (&local_E4)->y)) {
      (&local_E4)->y = (&local_E4)->y - 0.1f;
    } else if (!((&local_F0)->y + 0.01f >= (&local_E4)->y)) {
      (&local_E4)->y = (&local_E4)->y - 0.01f;
    } else {
      if (!((&local_F0)->y + 0.001f >= (&local_E4)->y)) {
        (&local_E4)->y = (&local_E4)->y - 0.001f;
        goto L006C31;
      }
    }
    L006C31:;
    if (!((&local_F0)->z - 0.1f <= (&local_E4)->z)) {
      (&local_E4)->z = (&local_E4)->z + 0.1f;
    } else if (!((&local_F0)->z - 0.01f <= (&local_E4)->z)) {
      (&local_E4)->z = (&local_E4)->z + 0.01f;
    } else {
      if (!((&local_F0)->z - 0.001f <= (&local_E4)->z)) {
        (&local_E4)->z = (&local_E4)->z + 0.001f;
        goto L006CB8;
      }
    }
    L006CB8:;
    if (!((&local_F0)->z + 0.1f >= (&local_E4)->z)) {
      (&local_E4)->z = (&local_E4)->z - 0.1f;
    } else if (!((&local_F0)->z + 0.01f >= (&local_E4)->z)) {
      (&local_E4)->z = (&local_E4)->z - 0.01f;
    } else {
      if (!((&local_F0)->z + 0.001f >= (&local_E4)->z)) {
        (&local_E4)->z = (&local_E4)->z - 0.001f;
        goto L006D3F;
      }
    }
    L006D3F:;
    if (local_FC < 200) {
      Script_FaceLookAtTargetUtils(CHARA_AKIR, 2, &local_E4, 1);
    }
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  script_7BCD();
  return;
}

static void script_6D9E(int32_t arg0) {
  int32_t local_FC;
  int32_t unused_F8;
  float local_EC[3];
  Vector3 local_E0;
  int32_t unused_98[18];
  float local_88[4];
  int32_t local_84;
  local_88[0] = -1.0f;
  local_88[1] = -1.0f;
  local_88[2] = -1.0f;
  local_88[3] = -1.0f;
  script_78C1();
  Script_scenCreateTask(script_7E7A, 0);
  Script_shenSeqUtils(0);
  local_FC = 0;
  while (Script_shenSeqUtils(-1)) {
    local_84 = local_FC;
    switch (local_84) {
      case 0:
        Script_charaVisUtils(CHARA_AKIR, 1);
        Script_charaVisUtils(CHARA_KISY, 1);
        Script_charaVisUtils(SHEN_NAME('BTA_'), 0);
        Script_charaVisUtils(SHEN_NAME('BTB_'), 0);
        Script_charaVisUtils(SHEN_NAME('BTC_'), 0);
        Script_charaVisUtils(SHEN_NAME('BTD_'), 0);
        Script_charaVisUtils(SHEN_NAME('BTE_'), 0);
        Script_charaVisUtils(SHEN_NAME('BTF_'), 0);
        Script_CharHandlers(CHARA_AKIR, 1, 0, 16);
        Script_CharHandlers(CHARA_AKIR, 0, 0, 16);
        local_E0.x = 33.376099f;
        (&local_E0)->y = 1.6791f;
        (&local_E0)->z = 97.802399f;
        local_EC[0] = 33.376099f;
        local_EC[1] = 1.6791f;
        local_EC[2] = 97.802399f;
        Script_FaceLookAtTargetUtils(CHARA_AKIR, 2, &local_E0, 1);
        Script_FaceLookAtTargetUtils(CHARA_KISY, 2, &local_E0, 1);
        break;
      case 160:
        Script_charaVisUtils(SHEN_NAME('BTA_'), 1);
        Script_charaVisUtils(SHEN_NAME('BTB_'), 1);
        Script_charaVisUtils(SHEN_NAME('BTC_'), 1);
        Script_charaVisUtils(SHEN_NAME('BTD_'), 1);
        Script_charaVisUtils(SHEN_NAME('BTE_'), 1);
        Script_charaVisUtils(SHEN_NAME('BTF_'), 1);
        Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 50.0f);
        Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 50.0f);
        Script_FaceLookAtTargetUtils(CHARA_AKIR, 0);
        Script_FaceLookAtTargetUtils(CHARA_KISY, 0);
        break;
      case 385:
        Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 20.0f);
        Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 20.0f);
        Script_ResetHandMotion(CHARA_KISY, 0, 1, 0);
        Script_ResetHandMotion(CHARA_KISY, 0, 0, 0);
        Script_SetCharaFlags(CHARA_AKIR, 0);
        Script_SetCharaFlags(CHARA_KISY, 0);
        break;
      case 434:
        Script_SetCharaFlags(CHARA_AKIR, 4);
        Script_SetCharaFlags(CHARA_KISY, 4);
        Script_FaceEx(CHARA_AKIR, 1, 2, 1);
        Script_FaceEx(CHARA_KISY, 1, 2, 1);
        Script_ResetHandMotion(CHARA_AKIR, 1, 1, 0);
        Script_ResetHandMotion(CHARA_AKIR, 1, 0, 0);
        Script_ResetHandMotion(CHARA_KISY, 1, 1, 0);
        Script_ResetHandMotion(CHARA_KISY, 1, 0, 0);
        Script_handSetFloatsFor(CHARA_AKIR, 1, data_15180, 1);
        Script_handSetFloatsFor(CHARA_AKIR, 0, data_15180, 1);
        Script_handSetFloatsFor(CHARA_KISY, 1, data_15180, 1);
        Script_handSetFloatsFor(CHARA_KISY, 0, data_15180, 1);
        Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 50.0f);
        Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 50.0f);
        break;
      case 550:
        Script_SetCharaFlags(CHARA_AKIR, 0);
        Script_SetCharaFlags(CHARA_KISY, 0);
        break;
      case 710:
        Script_handSetFloatsFor(CHARA_AKIR, 1, data_15180, 70);
        Script_handSetFloatsFor(CHARA_AKIR, 0, data_15180, 70);
        Script_handSetFloatsFor(CHARA_KISY, 1, data_149E4, 70);
        Script_handSetFloatsFor(CHARA_KISY, 0, data_14900, 70);
        break;
      case 680:
        local_EC[0] = 32.8839f;
        local_EC[1] = 1.6791f;
        local_EC[2] = 100.104599f;
        break;
      case 727:
        local_EC[0] = 35.763f;
        local_EC[1] = 1.8605f;
        local_EC[2] = 95.8638f;
        break;
      case 661:
        Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 90.0f);
        Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 90.0f);
        break;
      case 770:
        break;
      case 810:
        Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 20.0f);
        Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 20.0f);
        Script_FaceEx(CHARA_AKIR, 0, 0, 1);
        Script_FaceEx(CHARA_KISY, 0, 0, 1);
        Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
        Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
        Script_ResetHandMotion(CHARA_KISY, 0, 1, 0);
        Script_ResetHandMotion(CHARA_KISY, 0, 0, 0);
        Script_SetShadowFlags(CHARA_AKIR, SCRIPT_SHADOW_UPDATE_LIGHT_MASK_0, 1);
        Script_SetShadowFlags(CHARA_KISY, SCRIPT_SHADOW_UPDATE_LIGHT_MASK_0, 1);
        break;
      case 1010:
        break;
    }
    if (!(*local_EC - 0.1f <= (&local_E0)->x)) {
      local_E0.x = (&local_E0)->x + 0.1f;
    } else if (!(*local_EC - 0.01f <= (&local_E0)->x)) {
      local_E0.x = (&local_E0)->x + 0.01f;
    } else {
      if (!(*local_EC - 0.001f <= (&local_E0)->x)) {
        local_E0.x = (&local_E0)->x + 0.001f;
        goto L0072B8;
      }
    }
    L0072B8:;
    if (!(*local_EC + 0.1f >= (&local_E0)->x)) {
      local_E0.x = (&local_E0)->x - 0.1f;
    } else if (!(*local_EC + 0.01f >= (&local_E0)->x)) {
      local_E0.x = (&local_E0)->x - 0.01f;
    } else {
      if (!(*local_EC + 0.001f >= (&local_E0)->x)) {
        local_E0.x = (&local_E0)->x - 0.001f;
        goto L00731B;
      }
    }
    L00731B:;
    if (!(local_EC[1] - 0.1f <= (&local_E0)->y)) {
      (&local_E0)->y = (&local_E0)->y + 0.1f;
    } else if (!(local_EC[1] - 0.01f <= (&local_E0)->y)) {
      (&local_E0)->y = (&local_E0)->y + 0.01f;
    } else {
      if (!(local_EC[1] - 0.001f <= (&local_E0)->y)) {
        (&local_E0)->y = (&local_E0)->y + 0.001f;
        goto L0073A2;
      }
    }
    L0073A2:;
    if (!(local_EC[1] + 0.1f >= (&local_E0)->y)) {
      (&local_E0)->y = (&local_E0)->y - 0.1f;
    } else if (!(local_EC[1] + 0.01f >= (&local_E0)->y)) {
      (&local_E0)->y = (&local_E0)->y - 0.01f;
    } else {
      if (!(local_EC[1] + 0.001f >= (&local_E0)->y)) {
        (&local_E0)->y = (&local_E0)->y - 0.001f;
        goto L007429;
      }
    }
    L007429:;
    if (!(local_EC[2] - 0.1f <= (&local_E0)->z)) {
      (&local_E0)->z = (&local_E0)->z + 0.1f;
    } else if (!(local_EC[2] - 0.01f <= (&local_E0)->z)) {
      (&local_E0)->z = (&local_E0)->z + 0.01f;
    } else {
      if (!(local_EC[2] - 0.001f <= (&local_E0)->z)) {
        (&local_E0)->z = (&local_E0)->z + 0.001f;
        goto L0074B0;
      }
    }
    L0074B0:;
    if (!(local_EC[2] + 0.1f >= (&local_E0)->z)) {
      (&local_E0)->z = (&local_E0)->z - 0.1f;
    } else if (!(local_EC[2] + 0.01f >= (&local_E0)->z)) {
      (&local_E0)->z = (&local_E0)->z - 0.01f;
    } else {
      if (!(local_EC[2] + 0.001f >= (&local_E0)->z)) {
        (&local_E0)->z = (&local_E0)->z - 0.001f;
        goto L007537;
      }
    }
    L007537:;
    if ((local_FC > 190) & (local_FC < 460)) {
      if (!(*local_88 >= 1.0f)) {
        local_88[0] = *local_88 + 0.01f;
      }
      if (!(local_88[1] >= 0.04f)) {
        local_88[1] = local_88[1] + 0.01f;
      }
      if (!(local_88[2] >= 0.02f)) {
        local_88[2] = local_88[2] + 0.01f;
      }
      if (!(local_88[3] >= 0.05f)) {
        local_88[3] = local_88[3] + 0.01f;
      }
    }
    if ((local_FC > 60) & (local_FC < 810)) {
      Script_FaceLookAtTargetUtils(CHARA_AKIR, 2, &local_E0, 1);
    }
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  script_7C7E();
  return;
}

static void script_7630(void) {
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 300.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 300.0f);
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 1);
  Script_charaSetRefboardFlagTo(CHARA_KISY, 1);
  Script_charaSetRefboardFlagTo(CHARA_STSI, 1);
  Script_SetCharaRefBoardPos(CHARA_AKIR, &data_15968);
  Script_SetCharaRefBoardPos(CHARA_KISY, &data_15968);
  Script_SetCharaRefBoardPos(CHARA_STSI, &data_15968);
  Script_SetupLightFromTable(4);
  script_7F2F(CHARA_AKIR);
  script_7F2F(CHARA_KISY);
  script_7F2F(CHARA_STSI);
  Script_SetCharaFlags(CHARA_AKIR, 4);
  Script_SetCharaFlags(CHARA_KISY, 4);
  Script_SetCharaFlags(CHARA_STSI, 4);
  Script_SetShadowFlags(CHARA_KISY, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_SetShadowFlags(CHARA_AKIR, SCRIPT_SHADOW_UPDATE_LIGHT_MASK_0, 1);
  Script_SetShadowFlags(CHARA_STSI, SCRIPT_SHADOW_UPDATE_LIGHT_MASK_0, 1);
  Script_SetShadowFlags(CHARA_AKIR, SCRIPT_SHADOW_UPDATE_LIGHT_MASK_0, 2);
  Script_SetShadowFlags(CHARA_STSI, SCRIPT_SHADOW_UPDATE_LIGHT_MASK_0, 2);
  Script_SetShadowFlags(CHARA_AKIR, SCRIPT_SHADOW_UPDATE_LIGHT_MASK_0, 3);
  Script_SetShadowFlags(CHARA_STSI, SCRIPT_SHADOW_UPDATE_LIGHT_MASK_0, 3);
  Script_SetShadowFlags(CHARA_AKIR, SCRIPT_SHADOW_UPDATE_LIGHT_MASK_0, 4);
  Script_SetShadowFlags(CHARA_STSI, SCRIPT_SHADOW_UPDATE_LIGHT_MASK_0, 4);
  Script_SetShadowFlags(CHARA_AKIR, SCRIPT_SHADOW_UPDATE_LIGHT_MASK_0, 5);
  Script_SetShadowFlags(CHARA_STSI, SCRIPT_SHADOW_UPDATE_LIGHT_MASK_0, 5);
  Script_SetShadowFlags(CHARA_AKIR, SCRIPT_SHADOW_UPDATE_LIGHT_MASK_0, 6);
  Script_SetShadowFlags(CHARA_STSI, SCRIPT_SHADOW_UPDATE_LIGHT_MASK_0, 6);
  Script_SetShadowFlags(CHARA_AKIR, SCRIPT_SHADOW_UPDATE_LIGHT_MASK_0, 7);
  Script_SetShadowFlags(CHARA_STSI, SCRIPT_SHADOW_UPDATE_LIGHT_MASK_0, 7);
  Script_SetShadowFlags(CHARA_AKIR, SCRIPT_SHADOW_UPDATE_LIGHT_MASK_0, 8);
  Script_SetShadowFlags(CHARA_STSI, SCRIPT_SHADOW_UPDATE_LIGHT_MASK_0, 8);
  Script_SetShadowFlags(CHARA_AKIR, SCRIPT_SHADOW_UPDATE_LIGHT_MASK_0, 9);
  Script_SetShadowFlags(CHARA_STSI, SCRIPT_SHADOW_UPDATE_LIGHT_MASK_0, 9);
  Script_SetShadowFlags(CHARA_AKIR, SCRIPT_SHADOW_UPDATE_LIGHT_MASK_0, 10);
  Script_SetShadowFlags(CHARA_STSI, SCRIPT_SHADOW_UPDATE_LIGHT_MASK_0, 10);
  return;
}

static void script_780F(void) {
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 100.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 100.0f);
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 1);
  Script_charaSetRefboardFlagTo(CHARA_KISY, 1);
  Script_charaSetRefboardFlagTo(CHARA_TERY, 1);
  Script_SetCharaRefBoardPos(CHARA_AKIR, &data_15998);
  Script_SetCharaRefBoardPos(CHARA_KISY, &data_15998);
  Script_SetCharaRefBoardPos(CHARA_TERY, &data_15998);
  script_7F2F(CHARA_AKIR);
  script_7F2F(CHARA_KISY);
  script_7F2F(CHARA_TERY);
  Script_SetCharaFlags(CHARA_AKIR, 4);
  Script_SetCharaFlags(CHARA_KISY, 4);
  Script_SetCharaFlags(CHARA_TERY, 4);
  return;
}

static void script_78C1(void) {
  Vector3i local_F4;
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 1);
  Script_charaSetRefboardFlagTo(CHARA_KISY, 1);
  Script_charaSetRefboardFlagTo(SHEN_NAME('BTA_'), 1);
  Script_charaSetRefboardFlagTo(SHEN_NAME('BTB_'), 1);
  Script_charaSetRefboardFlagTo(SHEN_NAME('BTC_'), 1);
  Script_charaSetRefboardFlagTo(SHEN_NAME('BTD_'), 1);
  Script_charaSetRefboardFlagTo(SHEN_NAME('BTE_'), 1);
  Script_charaSetRefboardFlagTo(SHEN_NAME('BTF_'), 1);
  Script_SetCharaRefBoardPos(CHARA_AKIR, &data_159C8);
  Script_SetCharaRefBoardPos(CHARA_KISY, &data_159C8);
  Script_SetCharaRefBoardPos(SHEN_NAME('BTA_'), &data_159D8);
  Script_SetCharaRefBoardPos(SHEN_NAME('BTB_'), &data_159D8);
  Script_SetCharaRefBoardPos(SHEN_NAME('BTC_'), &data_159D8);
  Script_SetCharaRefBoardPos(SHEN_NAME('BTD_'), &data_159D8);
  Script_SetCharaRefBoardPos(SHEN_NAME('BTE_'), &data_159D8);
  Script_SetCharaRefBoardPos(SHEN_NAME('BTF_'), &data_159D8);
  Script_SetupLightFromTable(5);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 90.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 90.0f);
  local_F4.x = 0;
  (&local_F4)->y = 41870;
  (&local_F4)->z = 0;
  Script_EVT_SetRotation(SHEN_NAME('DOR_'), 0x38000000, &local_F4, 0);
  Script_SetMaxNPCsNear(32);
  script_7F2F(CHARA_AKIR);
  script_7F2F(CHARA_KISY);
  script_0048(SHEN_NAME('BTA_'));
  script_0048(SHEN_NAME('BTB_'));
  script_0048(SHEN_NAME('BTC_'));
  script_0048(SHEN_NAME('BTD_'));
  script_0048(SHEN_NAME('BTE_'));
  script_0048(SHEN_NAME('BTF_'));
  Script_SetMotnFlag(CHARA_KISY, -1);
  Script_SetMotnFlag(CHARA_AKIR, -1);
  Script_SetMotnFlag(SHEN_NAME('BTA_'), -1);
  Script_SetMotnFlag(SHEN_NAME('BTB_'), -1);
  Script_SetMotnFlag(SHEN_NAME('BTC_'), -1);
  Script_SetMotnFlag(SHEN_NAME('BTD_'), -1);
  Script_SetMotnFlag(SHEN_NAME('BTE_'), -1);
  Script_SetMotnFlag(SHEN_NAME('BTF_'), -1);
  Script_SetCharaFlags(CHARA_AKIR, 4);
  Script_SetCharaFlags(CHARA_KISY, 4);
  Script_charaDisplay(CHARA_TERY, CDM_Hide);
  Script_SetCharaFlags(CHARA_AKIR, 4);
  Script_SetCharaFlags(CHARA_KISY, 4);
  script_7EA8(CHARA_AKIR);
  script_7EA8(CHARA_KISY);
  Script_SetShadowFlags(CHARA_AKIR, SCRIPT_SHADOW_UPDATE_LIGHT_MASK, 1);
  Script_SetShadowFlags(CHARA_KISY, SCRIPT_SHADOW_UPDATE_LIGHT_MASK, 1);
  return;
}

static void script_7B1C(void) {
  int32_t unused_F4[3];
  script_00D5(CHARA_AKIR);
  script_00D5(CHARA_STSI);
  script_00D5(CHARA_KISY);
  Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
  Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
  Script_ResetHandMotion(CHARA_KISY, 0, 1, 0);
  Script_ResetHandMotion(CHARA_KISY, 0, 0, 0);
  Script_ResetHandMotion(CHARA_STSI, 0, 1, 0);
  Script_ResetHandMotion(CHARA_STSI, 0, 0, 0);
  Script_SetGameStateTo(AUTH_None);
  script_7EA8(CHARA_AKIR);
  script_7EA8(CHARA_KISY);
  script_7EA8(CHARA_STSI);
  return;
}

static void script_7BCD(void) {
  int32_t unused_F4[3];
  script_00D5(CHARA_AKIR);
  Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
  Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
  Script_ResetHandMotion(CHARA_KISY, 0, 1, 0);
  Script_ResetHandMotion(CHARA_KISY, 0, 0, 0);
  Script_ResetHandMotion(CHARA_TERY, 0, 1, 0);
  Script_ResetHandMotion(CHARA_TERY, 0, 0, 0);
  script_00D5(CHARA_KISY);
  script_00D5(CHARA_TERY);
  Script_SetGameStateTo(AUTH_None);
  script_7EA8(CHARA_AKIR);
  script_7EA8(CHARA_KISY);
  script_7EA8(CHARA_TERY);
  return;
}

static void script_7C7E(void) {
  int32_t unused_F4[3];
  script_00D5(CHARA_AKIR);
  Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
  Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
  Script_ResetHandMotion(CHARA_KISY, 0, 1, 0);
  Script_ResetHandMotion(CHARA_KISY, 0, 0, 0);
  script_00D5(CHARA_KISY);
  script_00D5(SHEN_NAME('BTA_'));
  script_00D5(SHEN_NAME('BTB_'));
  script_00D5(SHEN_NAME('BTC_'));
  script_00D5(SHEN_NAME('BTD_'));
  script_00D5(SHEN_NAME('BTF_'));
  script_00D5(SHEN_NAME('BTE_'));
  Script_SetGameStateTo(AUTH_None);
  script_7EA8(CHARA_AKIR);
  script_7EA8(CHARA_KISY);
  return;
}

static void script_7D3F(void) {
  int32_t unused_F0[4];
  Vector4 local_E0;
  int32_t unused_D4[3];
  Vector3 local_C8;
  local_C8.x = -8.42f;
  (&local_C8)->y = 1.25f;
  (&local_C8)->z = 82.69f;
  Script_lightEnablePointLightAt(3, &local_C8);
  local_E0.x = 1.0f;
  (&local_E0)->y = 0.24f;
  (&local_E0)->z = 0.27f;
  (&local_E0)->w = 0.28f;
  Script_LightUtils(3, 2, &local_E0);
  Script_LightUtils(3, 4, 1.0f);
  Script_LightUtils(3, 5, 1.42f);
  Script_LightUtils(3, 6, data_15A08);
  Script_lightSetSlotEnabled(3, 1);
  return;
}

static void script_7DD1(int32_t arg0) {
  Vector3 local_F4;
  local_F4.x = 0.0f;
  (&local_F4)->y = 0.0f;
  (&local_F4)->z = 0.0176470596f;
  task_C8.x = 0.0f;
  task_C8.y = 0.0f;
  task_C8.z = 0.0f;
  Script_EffectUtils(6, arg0, &local_F4, &task_C8);
  Script_EffectUtils(7, arg0, 0.0960784331f);
  Script_EffectUtils(9, arg0, 178, 172, 212);
  Script_EffectUtils(8, arg0, 10.0f, 3.490196f);
  Script_EffectUtils(10, arg0, 0.260784328f);
  Script_EffectUtils(11, arg0, 98);
  Script_EffectUtils(12, arg0, 28);
  Script_EffectUtils(13, arg0, 50);
  return;
}

static int32_t script_7E7A(void) {
  Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_7EA8(int32_t arg0) {
  Script_SetShadowFlags(arg0, SCRIPT_SHADOW_UPDATE_LIGHT_MASK_0, 1);
  Script_SetShadowFlags(arg0, SCRIPT_SHADOW_UPDATE_LIGHT_MASK_0, 2);
  Script_SetShadowFlags(arg0, SCRIPT_SHADOW_UPDATE_LIGHT_MASK_0, 3);
  Script_SetShadowFlags(arg0, SCRIPT_SHADOW_UPDATE_LIGHT_MASK_0, 4);
  Script_SetShadowFlags(arg0, SCRIPT_SHADOW_UPDATE_LIGHT_MASK_0, 5);
  Script_SetShadowFlags(arg0, SCRIPT_SHADOW_UPDATE_LIGHT_MASK_0, 6);
  Script_SetShadowFlags(arg0, SCRIPT_SHADOW_UPDATE_LIGHT_MASK_0, 7);
  Script_SetShadowFlags(arg0, SCRIPT_SHADOW_UPDATE_LIGHT_MASK_0, 8);
  Script_SetShadowFlags(arg0, SCRIPT_SHADOW_UPDATE_LIGHT_MASK_0, 9);
  Script_SetShadowFlags(arg0, SCRIPT_SHADOW_UPDATE_LIGHT_MASK_0, 10);
  Script_SetShadowFlags(arg0, SCRIPT_SHADOW_UPDATE_LIGHT_MASK_0, 11);
  Script_SetShadowFlags(arg0, SCRIPT_SHADOW_UPDATE_LIGHT_MASK_0, 12);
  return;
}

static void script_7F2F(int32_t arg0) {
  Script_ccowUtils(arg0, 2);
  if (0) {
    Script_charaDisplay(arg0, CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(arg0, CDM_Show);
      goto L007F5B;
    }
  }
  L007F5B:;
  Script_scenToggleVisibleFor(arg0, 1);
  Script_ccowSetFlag80(arg0, 1);
  Script_motmToggleFlags(arg0, 2, 0x40);
  Script_charaToggleMotEnabled(arg0, 0);
  Script_FaceEx(arg0, 1, 0, 2);
  Script_FaceLookAtTargetUtils(arg0, 0);
  Script_CharHandlers(arg0, 2, 8, 16);
  Script_SetCPAMFlags(arg0, 0);
  Script_charaDisplay(arg0, CDM_Show);
  return;
}

static void script_7FB2(void) {
  Script_ambSoundUtils(1, 1, &data_15A10, 135.0f, 280.0f, 1.0f, 0, &data_15AA0);
  Script_ambSoundUtils(2, 1, &data_15A1C, 135.0f, 60.0f, 1.0f, 0, &data_15B28);
  Script_ambSoundUtils(3, 0, &data_15A28, 50.0f, 170.0f, 0.8f, 0, data_15BB0);
  Script_ambSoundUtils(4, 0, &data_15A34, 10.0f, 5.0f, 1.0f, 0, &data_15C38);
  Script_ambSoundUtils(5, 0, data_15A40, 8.0f, 5.0f, 1.0f, 0, &data_15CC0);
  Script_ambSoundUtils(6, 1, &data_15A4C, 170.0f, 100.0f, 1.0f, 2, data_15D48);
  Script_ambSoundUtils(7, 1, &data_15A58, 170.0f, 100.0f, 1.0f, 0, &data_15DD0);
  Script_ambSoundUtils(8, 1, &data_15A64, 40.0f, 70.0f, 0.8f, 0, &data_15E58);
  Script_ambSoundUtils(9, 0, &data_15A70, 3.0f, 60.0f, 0.8f, 0, data_15EE0);
  Script_ambSoundUtils(10, 0, &data_15A7C, 60.0f, 35.0f, 0.7f, 0, &data_15F68);
  Script_ambSoundUtils(13, 0, &data_15A88, 8.0f, 5.0f, 1.0f, 0, &data_15FF0);
  Script_ambSoundUtils(15, 1, data_15A94, 300.0f, 0.0f, 1.0f, 3, &data_16078);
  Script_Ambient1SoundUtils(0, 23976);
  return;
}

static void script_816C(void) {
  Script_ambSoundUtils(1, 1, &data_16100, 135.0f, 280.0f, 1.0f, 0, &data_16160);
  Script_ambSoundUtils(2, 1, &data_1610C, 135.0f, 60.0f, 1.0f, 0, &data_161E8);
  Script_ambSoundUtils(3, 0, &data_16118, 50.0f, 170.0f, 0.8f, 0, data_16270);
  Script_ambSoundUtils(6, 1, &data_16124, 170.0f, 100.0f, 1.0f, 2, data_162F8);
  Script_ambSoundUtils(7, 1, &data_16130, 170.0f, 100.0f, 1.0f, 0, &data_16380);
  Script_ambSoundUtils(8, 1, &data_1613C, 40.0f, 70.0f, 0.8f, 0, &data_16408);
  Script_ambSoundUtils(10, 0, &data_16148, 60.0f, 35.0f, 0.7f, 0, &data_16490);
  Script_ambSoundUtils(15, 1, data_16154, 300.0f, 0.0f, 1.0f, 3, &data_16518);
  Script_Ambient1SoundUtils(0, 23976);
  return;
}

static void script_8296(void) {
  Script_ambSoundUtils(1, 0, &data_165A0, 1.0f, 10.0f, 1.0f, 0, &data_165AC);
  Script_Ambient1SoundUtils(0, 23976);
  return;
}

static void script_82C7(void) {
  Script_ambSoundUtils(1, 0, &data_16634, 1.0f, 10.0f, 1.0f, 0, &data_16640);
  Script_Ambient1SoundUtils(0, 23976);
  return;
}

static void script_82F8(void) {
  Script_ambSoundUtils(1, 1, &data_166C8, 135.0f, 280.0f, 1.0f, 0, &data_16728);
  Script_ambSoundUtils(2, 1, &data_166D4, 135.0f, 60.0f, 1.0f, 0, &data_167B0);
  Script_ambSoundUtils(3, 0, &data_166E0, 50.0f, 170.0f, 0.8f, 0, data_16838);
  Script_ambSoundUtils(6, 1, &data_166EC, 170.0f, 100.0f, 1.0f, 2, data_168C0);
  Script_ambSoundUtils(7, 1, &data_166F8, 170.0f, 100.0f, 1.0f, 0, &data_16948);
  Script_ambSoundUtils(8, 1, &data_16704, 40.0f, 70.0f, 0.8f, 0, &data_169D0);
  Script_ambSoundUtils(15, 1, data_1671C, 300.0f, 0.0f, 1.0f, 3, &data_16A58);
  Script_Ambient1SoundUtils(0, 23976);
  return;
}

static void script_83FE(void) {
  script_7FB2();
  return;
}

__attribute__((scn_string_bindings(0, data_16B28, 1, data_16B37, 2, data_16B3C, 3, data_16B41, 4, data_16B42, 5, data_16B49, 7, data_16B59, 9, data_16B71, 10, data_16B76)))
static void script_8406(void) {
  Script_ReturnTimeDate(&task_70);
  script_13327("/scene/03/MFBT", "TLYA");
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
  Script_AuthorUtils(0, 0, "TLYA", "");
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
  script_8794();
  Script_LoadVoiceFile(1, "01TLYA");
  script_134D1();
  Script_LoadSND(0, 1, "FREE", 0);
  Script_LoadSND(0, 1, "amb013.snd", 0);
  script_816C();
  Script_executeAicaCommand(1184, 1, 127);
  Script_LoadSND(0, 2, "bgm013.snd", 0);
  Script_executeAicaCommand(9640, 0, 0);
  Script_executeAicaCommand(1184, 2, 120);
  Script_LoadSND(0, 4, "a1_kbtmm.snd", 0);
  Script_executeAicaCommand(1184, 4, 196);
  task_BC = Script_EffectUtils(0, &data_16AE0, &data_16AEC, 1, 1.0f, 0, 1);
  Script_EffectUtils(13, task_BC, 50);
  Script_EffectUtils(16, task_BC, 0.03f);
  task_C0 = Script_EffectUtils(0, &data_16AF8, &data_16B04, 1, 1.0f, 0, 1);
  Script_EffectUtils(13, task_C0, 50);
  Script_EffectUtils(16, task_C0, 0.03f);
  task_C4 = Script_EffectUtils(0, &data_16B10, &data_16B1C, 1, 1.0f, 0, 1);
  Script_EffectUtils(13, task_C4, 50);
  Script_EffectUtils(16, task_C4, 0.03f);
  Script_cdttLoadScrUtils(1, 0);
  script_5EE2(1);
  Script_shenClearFreeState(0);
  Script_EffectUtils(3, task_BC);
  Script_EffectUtils(3, task_C0);
  Script_EffectUtils(3, task_C4);
  Script_AuthorUtils(AUTH_FREE, 0);
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "TLYA");
  Script_LoadVoiceFile(0, "01TLYA");
  script_8852();
  return;
}

__attribute__((scn_string_bindings(0, data_16B7D, 1, data_16B8C, 2, data_16B91, 3, data_16B96, 4, data_16B97, 7, data_16BB6, 8, data_16BBB)))
static void script_85E8(void) {
  int32_t unused_FC;
  script_13327("/scene/03/MFBT", "TLYB");
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
  Script_AuthorUtils(0, 0, "TLYB", "");
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
  Script_LoadVoiceFile(1, "01TLYB");
  script_134D1();
  Script_LoadSND(0, 2, "bgm084.snd", 0);
  Script_executeAicaCommand(16296, 0, 0);
  Script_executeAicaCommand(1184, 2, 96);
  Script_LoadSND(0, 4, "a1_teryb.snd", 0);
  Script_motmGetCharaFlags(CHARA_AKIR, 0, 0);
  script_6685(1);
  Script_shenClearFreeState(0);
  Script_executeAicaCommand(2720, 2, 30);
  Script_AuthorUtils(AUTH_FREE, 0);
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "TLYB");
  Script_LoadVoiceFile(0, "01TLYB");
  Script_PREP_Related(-1, 0);
  return;
}

__attribute__((scn_string_bindings(0, data_16BC2, 2, data_16BDA, 3, data_16BE9, 4, data_16BEE, 5, data_16BF3, 6, data_16BF4, 7, data_16BFB, 8, data_16C00)))
static void script_86C0(void) {
  script_134D1();
  Script_LoadSND(0, 2, "bgm013.snd", 0);
  Script_executeAicaCommand(9640, 0, 0);
  Script_executeAicaCommand(1184, 2, 96);
  Script_LoadSND(0, 4, "a1_kakom.snd", 0);
  Script_mapmSetRenderFlag(2, 1);
  script_13327("/scene/03/MFBT", "TLYC");
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
  Script_AuthorUtils(0, 0, "TLYC", "");
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
  Script_LoadVoiceFile(1, "01TLYC");
  script_6D9E(1);
  Script_shenClearFreeState(0);
  Script_executeAicaCommand(2720, 2, 30);
  Script_AuthorUtils(AUTH_FREE, 0);
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "TLYC");
  Script_LoadVoiceFile(0, "01TLYC");
  Script_mapmSetRenderFlag(2, 0);
  return;
}

__attribute__((scn_string_bindings(0, data_16C14, 2, data_16C2A)))
static void script_8794(void) {
  task_D4 = Script_MT5Loader_TRWV_TRWAVE(0, &data_16C08, "model/object", "SEA2.PVR", "model/object", "TRSEA4.MT6", 4.0f, 0);
  Script_MT5Loader_TRWV_TRWAVE(21, task_D4, 8, 8);
  Script_MT5Loader_TRWV_TRWAVE(16, task_D4, -1, 1);
  Script_MT5Loader_TRWV_TRWAVE(13, task_D4, 0.0980392173f, 0.156862751f);
  Script_MT5Loader_TRWV_TRWAVE(14, task_D4, 0.0745098069f, -0.00392156886f);
  Script_MT5Loader_TRWV_TRWAVE(10, task_D4, 64.0f, 64.0f, 3, 1, 1, 1);
  Script_MT5Loader_TRWV_TRWAVE(24, task_D4, 0, 151, 218, 192);
  Script_MT5Loader_TRWV_TRWAVE(11, task_D4, -0.61f, -0.02f, 0.79f);
  return;
}

static void script_8852(void) {
  Script_MT5Loader_TRWV_TRWAVE(3, task_D4);
  return;
}

static void script_885F(void) {
  task_0 = 0x3253494B;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 0;
  *(&task_0 + 4) = 200;
  *(&task_0 + 5) = 29;
  *(&task_0 + 6) = 0;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(SHEN_NAME('KIS2'), VF_FORMATION_APPLY_FLAGS, VF_HP_TEAM_BASED, 70);
  Script_vfConfigureFormationGroup(SHEN_NAME('KIS2'), VF_FORMATION_APPLY_FLAGS, VF_ReactionStateA, 17);
  Script_vfConfigureFormationGroup(SHEN_NAME('KIS2'), VF_FORMATION_APPLY_FLAGS, VF_ReactionStateA, 21);
  task_0 = 0x3043504E;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 10;
  *(&task_0 + 5) = 1;
  *(&task_0 + 6) = 0;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(SHEN_NAME('NPC0'), VF_FORMATION_APPLY_FLAGS, 0x200000, 0x800);
  Script_vfConfigureFormationGroup(SHEN_NAME('NPC0'), VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(SHEN_NAME('NPC0'), VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
  Script_vfConfigureFormationGroup(SHEN_NAME('NPC0'), VF_FORMATION_APPLY_FLAGS, 0x200000, 1);
  task_0 = 0x3143504E;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 10;
  *(&task_0 + 5) = 1;
  *(&task_0 + 6) = 0;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(SHEN_NAME('NPC1'), VF_FORMATION_APPLY_FLAGS, 0x200000, 0x800);
  Script_vfConfigureFormationGroup(SHEN_NAME('NPC1'), VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(SHEN_NAME('NPC1'), VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
  Script_vfConfigureFormationGroup(SHEN_NAME('NPC1'), VF_FORMATION_APPLY_FLAGS, 0x200000, 1);
  task_0 = 0x3243504E;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 10;
  *(&task_0 + 5) = 1;
  *(&task_0 + 6) = 0;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(SHEN_NAME('NPC2'), VF_FORMATION_APPLY_FLAGS, 0x200000, 0x800);
  Script_vfConfigureFormationGroup(SHEN_NAME('NPC2'), VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(SHEN_NAME('NPC2'), VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
  Script_vfConfigureFormationGroup(SHEN_NAME('NPC2'), VF_FORMATION_APPLY_FLAGS, 0x200000, 1);
  task_0 = 0x3343504E;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 10;
  *(&task_0 + 5) = 1;
  *(&task_0 + 6) = 0;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(SHEN_NAME('NPC3'), VF_FORMATION_APPLY_FLAGS, 0x200000, 0x800);
  Script_vfConfigureFormationGroup(SHEN_NAME('NPC3'), VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(SHEN_NAME('NPC3'), VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
  Script_vfConfigureFormationGroup(SHEN_NAME('NPC3'), VF_FORMATION_APPLY_FLAGS, 0x200000, 1);
  task_0 = 0x3443504E;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 10;
  *(&task_0 + 5) = 1;
  *(&task_0 + 6) = 0;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(SHEN_NAME('NPC4'), VF_FORMATION_APPLY_FLAGS, 0x200000, 0x800);
  Script_vfConfigureFormationGroup(SHEN_NAME('NPC4'), VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(SHEN_NAME('NPC4'), VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
  Script_vfConfigureFormationGroup(SHEN_NAME('NPC4'), VF_FORMATION_APPLY_FLAGS, 0x200000, 1);
  task_0 = 0x32544953;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 30;
  *(&task_0 + 5) = 1;
  *(&task_0 + 6) = 0;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(SHEN_NAME('SIT2'), VF_FORMATION_APPLY_FLAGS, 0x200000, 0x800);
  Script_vfConfigureFormationGroup(SHEN_NAME('SIT2'), VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(SHEN_NAME('SIT2'), VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
  Script_vfConfigureFormationGroup(SHEN_NAME('SIT2'), VF_FORMATION_APPLY_FLAGS, 0x200000, 1);
  task_0 = -1;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 10;
  *(&task_0 + 5) = 1;
  *(&task_0 + 6) = 2288;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x41C570A4;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x429DC7AE;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 1);
  task_0 = -1;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 10;
  *(&task_0 + 5) = 1;
  *(&task_0 + 6) = 63489;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x42023D71;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x42ADC28F;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 1);
  task_0 = -1;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 10;
  *(&task_0 + 5) = 1;
  *(&task_0 + 6) = 2287;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x41F4B852;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x42AA999A;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 1);
  task_0 = -1;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 10;
  *(&task_0 + 5) = 3;
  *(&task_0 + 6) = 2290;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x41915C29;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x42A40A3D;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 1);
  task_0 = -1;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 10;
  *(&task_0 + 5) = 3;
  *(&task_0 + 6) = 63496;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x41D43D71;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x42960000;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 1);
  task_0 = -1;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 10;
  *(&task_0 + 5) = 7;
  *(&task_0 + 6) = 63496;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x412D47AE;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x42AA3852;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 1);
  task_0 = -1;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 10;
  *(&task_0 + 5) = 4;
  *(&task_0 + 6) = 63489;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x41311EB8;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x42A970A4;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 1);
  task_0 = -1;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 10;
  *(&task_0 + 5) = 1;
  *(&task_0 + 6) = 2290;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x4106B852;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x42A43333;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 1);
  task_0 = -1;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 10;
  *(&task_0 + 5) = 3;
  *(&task_0 + 6) = 2290;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x40FF0A3D;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x42A0DC29;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 1);
  task_0 = -1;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 10;
  *(&task_0 + 5) = 7;
  *(&task_0 + 6) = 2290;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x41066666;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x4299947B;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 1);
  task_0 = -1;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 10;
  *(&task_0 + 5) = 4;
  *(&task_0 + 6) = 2287;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x41C251EC;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x428F147B;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 1);
  task_0 = -1;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 10;
  *(&task_0 + 5) = 4;
  *(&task_0 + 6) = 63533;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x41BA147B;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x428B051F;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 1);
  task_0 = -1;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 10;
  *(&task_0 + 5) = 7;
  *(&task_0 + 6) = 63533;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x419E3D71;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x429F6666;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 1);
  task_0 = 0x4D544953;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 150;
  *(&task_0 + 5) = 13;
  *(&task_0 + 6) = 0;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(SHEN_NAME('SITM'), VF_FORMATION_APPLY_FLAGS, 0x200000, 0x800);
  Script_vfConfigureFormationGroup(SHEN_NAME('SITM'), VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(SHEN_NAME('SITM'), VF_FORMATION_APPLY_FLAGS, 0x2000000, 0);
  task_0 = -1;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 20;
  *(&task_0 + 5) = 1;
  *(&task_0 + 6) = 2289;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x412570A4;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x428BFAE1;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 1);
  task_0 = -1;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 20;
  *(&task_0 + 5) = 2;
  *(&task_0 + 6) = 2289;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x416A3D71;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x42883D71;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 1);
  task_0 = -1;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 20;
  *(&task_0 + 5) = 1;
  *(&task_0 + 6) = 63496;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x41E770A4;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x4285AE14;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 1);
  task_0 = -1;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 20;
  *(&task_0 + 5) = 3;
  *(&task_0 + 6) = 63489;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x41DB999A;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x42888A3D;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 1);
  task_0 = -1;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 20;
  *(&task_0 + 5) = 3;
  *(&task_0 + 6) = 63533;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x41CECCCD;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x42869EB8;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 1);
  task_0 = -1;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 15;
  *(&task_0 + 5) = 3;
  *(&task_0 + 6) = 63533;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x418F0A3D;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x4293947B;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 1);
  task_0 = -1;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 15;
  *(&task_0 + 5) = 4;
  *(&task_0 + 6) = 63496;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x41728F5C;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x42A151EC;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 1);
  task_0 = -1;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 15;
  *(&task_0 + 5) = 3;
  *(&task_0 + 6) = 63533;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x41675C29;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x4297CCCD;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 1);
  task_0 = -1;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 15;
  *(&task_0 + 5) = 3;
  *(&task_0 + 6) = 2290;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x41775C29;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x429ACCCD;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 1);
  task_0 = -1;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 15;
  *(&task_0 + 5) = 4;
  *(&task_0 + 6) = 63496;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x41970A3D;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x42808A3D;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 1);
  task_0 = -1;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 20;
  *(&task_0 + 5) = 1;
  *(&task_0 + 6) = 2290;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x41E2E148;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x4287BD71;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 1);
  task_0 = -1;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 20;
  *(&task_0 + 5) = 4;
  *(&task_0 + 6) = 63489;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x41D60000;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x428AF0A4;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 1);
  task_0 = -1;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 20;
  *(&task_0 + 5) = 3;
  *(&task_0 + 6) = 63533;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x41D6F5C3;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x42853D71;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 1);
  task_0 = -1;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 20;
  *(&task_0 + 5) = 7;
  *(&task_0 + 6) = 63533;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x420470A4;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x42866B85;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 1);
  task_0 = -1;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 20;
  *(&task_0 + 5) = 2;
  *(&task_0 + 6) = 2288;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x41FAB852;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x4280B333;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 1);
  task_0 = -1;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 20;
  *(&task_0 + 5) = 4;
  *(&task_0 + 6) = 63533;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x41A33333;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x42822E14;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 1);
  task_0 = -1;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 20;
  *(&task_0 + 5) = 2;
  *(&task_0 + 6) = 63489;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x41A35C29;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x4276999A;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 1);
  task_0 = -1;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 20;
  *(&task_0 + 5) = 1;
  *(&task_0 + 6) = 2287;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x419D1EB8;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x428128F6;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 1);
  task_0 = -1;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 20;
  *(&task_0 + 5) = 7;
  *(&task_0 + 6) = 2288;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x41B31EB8;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x42833D71;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 1);
  task_0 = -1;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 20;
  *(&task_0 + 5) = 1;
  *(&task_0 + 6) = 63533;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x419251EC;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x42810A3D;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 1);
  task_0 = -1;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 15;
  *(&task_0 + 5) = 7;
  *(&task_0 + 6) = 63533;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x41F6147B;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x424EEB85;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 1);
  task_0 = -1;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 15;
  *(&task_0 + 5) = 3;
  *(&task_0 + 6) = 2288;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x41F68F5C;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x428551EC;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 1);
  task_0 = -1;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 15;
  *(&task_0 + 5) = 1;
  *(&task_0 + 6) = 2287;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x4218CCCD;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x42517AE1;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 1);
  task_0 = -1;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 15;
  *(&task_0 + 5) = 3;
  *(&task_0 + 6) = 63489;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x420F7AE1;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x4262CCCD;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 1);
  task_0 = -1;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 15;
  *(&task_0 + 5) = 1;
  *(&task_0 + 6) = 63533;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x42157AE1;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x4269CCCD;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 1);
  task_0 = -1;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 20;
  *(&task_0 + 5) = 2;
  *(&task_0 + 6) = 2288;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x4221147B;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x427647AE;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 1);
  task_0 = -1;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 20;
  *(&task_0 + 5) = 3;
  *(&task_0 + 6) = 2287;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x4200851F;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x4274E148;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 1);
  task_0 = -1;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 20;
  *(&task_0 + 5) = 4;
  *(&task_0 + 6) = 2290;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x42180A3D;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x426C8F5C;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 1);
  task_0 = -1;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 20;
  *(&task_0 + 5) = 3;
  *(&task_0 + 6) = 2287;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x420A28F6;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x424E3333;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 1);
  task_0 = -1;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 20;
  *(&task_0 + 5) = 7;
  *(&task_0 + 6) = 63533;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x4213D70A;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x4240CCCD;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 1);
  task_0 = 0x4F544153;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 250;
  *(&task_0 + 5) = 17;
  *(&task_0 + 6) = 0;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(CHARA_SATO, VF_FORMATION_APPLY_FLAGS, 0x200000, 0x800);
  Script_vfConfigureFormationGroup(CHARA_SATO, VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(CHARA_SATO, VF_FORMATION_APPLY_FLAGS, 0x2000000, 0);
  task_0 = -1;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 10;
  *(&task_0 + 5) = 1;
  *(&task_0 + 6) = 63496;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x422BD70A;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x4252CCCD;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 1);
  task_0 = -1;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 10;
  *(&task_0 + 5) = 3;
  *(&task_0 + 6) = 2287;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x42433D71;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x424A6666;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 1);
  task_0 = -1;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 10;
  *(&task_0 + 5) = 1;
  *(&task_0 + 6) = 63489;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x425E51EC;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x4225999A;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 1);
  task_0 = -1;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 10;
  *(&task_0 + 5) = 7;
  *(&task_0 + 6) = 63533;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x423628F6;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x4241C28F;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 1);
  task_0 = -1;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 10;
  *(&task_0 + 5) = 2;
  *(&task_0 + 6) = 2287;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x4245147B;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x42500A3D;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 1);
  task_0 = -1;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 10;
  *(&task_0 + 5) = 2;
  *(&task_0 + 6) = 2290;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x424D1EB8;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x42475C29;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 1);
  task_0 = -1;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 10;
  *(&task_0 + 5) = 7;
  *(&task_0 + 6) = 63533;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x42347AE1;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x422CCCCD;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 1);
  task_0 = -1;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 10;
  *(&task_0 + 5) = 2;
  *(&task_0 + 6) = 2287;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x423FC28F;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x4227999A;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 1);
  task_0 = -1;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 10;
  *(&task_0 + 5) = 7;
  *(&task_0 + 6) = 2287;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x42388F5C;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x422F6666;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 1);
  task_0 = -1;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 10;
  *(&task_0 + 5) = 1;
  *(&task_0 + 6) = 63489;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x4222851F;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x423D3D71;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 1);
  task_0 = -1;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 10;
  *(&task_0 + 5) = 2;
  *(&task_0 + 6) = 2287;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x41F4147B;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x4284199A;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 1);
  task_0 = -1;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 10;
  *(&task_0 + 5) = 7;
  *(&task_0 + 6) = 63533;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x4237CCCD;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x4242AE14;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 1);
  task_0 = -1;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 10;
  *(&task_0 + 5) = 1;
  *(&task_0 + 6) = 2290;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x4245AE14;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x4245C28F;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 1);
  task_0 = -1;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 10;
  *(&task_0 + 5) = 2;
  *(&task_0 + 6) = 2287;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x424F47AE;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x424328F6;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 1);
  task_0 = -1;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 10;
  *(&task_0 + 5) = 7;
  *(&task_0 + 6) = 63533;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x425AAE14;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x423EC28F;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 1);
  task_0 = -1;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 10;
  *(&task_0 + 5) = 3;
  *(&task_0 + 6) = 2288;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x42650000;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x423D0000;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 1);
  task_0 = -1;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 10;
  *(&task_0 + 5) = 2;
  *(&task_0 + 6) = 2287;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x426151EC;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x423247AE;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 1);
  task_0 = -1;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 10;
  *(&task_0 + 5) = 2;
  *(&task_0 + 6) = 63533;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x425970A4;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x4223F5C3;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 1);
  task_0 = 0x52444550;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 500;
  *(&task_0 + 5) = 33;
  *(&task_0 + 6) = 0;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(CHARA_PEDR, VF_FORMATION_APPLY_FLAGS, 0x200000, 0x800);
  Script_vfConfigureFormationGroup(CHARA_PEDR, VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(CHARA_PEDR, VF_FORMATION_APPLY_FLAGS, 0x2000000, 0);
  return;
}

static void script_B8B5(void) {
  task_0 = 0x3043504E;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 10;
  *(&task_0 + 5) = 50;
  *(&task_0 + 6) = 0;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(SHEN_NAME('NPC0'), VF_FORMATION_APPLY_FLAGS, 0x200000, 0x800);
  Script_vfConfigureFormationGroup(SHEN_NAME('NPC0'), VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(SHEN_NAME('NPC0'), VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
  task_0 = 0x3143504E;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 10;
  *(&task_0 + 5) = 50;
  *(&task_0 + 6) = 0;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(SHEN_NAME('NPC1'), VF_FORMATION_APPLY_FLAGS, 0x200000, 0x800);
  Script_vfConfigureFormationGroup(SHEN_NAME('NPC1'), VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(SHEN_NAME('NPC1'), VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
  task_0 = 0x3243504E;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 10;
  *(&task_0 + 5) = 50;
  *(&task_0 + 6) = 0;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(SHEN_NAME('NPC2'), VF_FORMATION_APPLY_FLAGS, 0x200000, 0x800);
  Script_vfConfigureFormationGroup(SHEN_NAME('NPC2'), VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(SHEN_NAME('NPC2'), VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
  task_0 = 0x3343504E;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 10;
  *(&task_0 + 5) = 50;
  *(&task_0 + 6) = 0;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(SHEN_NAME('NPC3'), VF_FORMATION_APPLY_FLAGS, 0x200000, 0x800);
  Script_vfConfigureFormationGroup(SHEN_NAME('NPC3'), VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(SHEN_NAME('NPC3'), VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
  Script_vfConfigureFormationGroup(SHEN_NAME('NPC3'), VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  task_0 = 0x3443504E;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 10;
  *(&task_0 + 5) = 50;
  *(&task_0 + 6) = 0;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(SHEN_NAME('NPC4'), VF_FORMATION_APPLY_FLAGS, 0x200000, 0x800);
  Script_vfConfigureFormationGroup(SHEN_NAME('NPC4'), VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(SHEN_NAME('NPC4'), VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
  task_0 = 0x32544953;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 30;
  *(&task_0 + 5) = 50;
  *(&task_0 + 6) = 0;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(SHEN_NAME('SIT2'), VF_FORMATION_APPLY_FLAGS, 0x200000, 0x800);
  Script_vfConfigureFormationGroup(SHEN_NAME('SIT2'), VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(SHEN_NAME('SIT2'), VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
  task_0 = -1;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 10;
  *(&task_0 + 5) = 50;
  *(&task_0 + 6) = 2288;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x41C570A4;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x429DC7AE;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
  task_0 = -1;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 10;
  *(&task_0 + 5) = 50;
  *(&task_0 + 6) = 63489;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x42023D71;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x42ADC28F;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  task_0 = -1;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 10;
  *(&task_0 + 5) = 50;
  *(&task_0 + 6) = 2287;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x41F4B852;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x42AA999A;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
  task_0 = -1;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 10;
  *(&task_0 + 5) = 52;
  *(&task_0 + 6) = 2290;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x41915C29;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x42A40A3D;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
  task_0 = -1;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 10;
  *(&task_0 + 5) = 52;
  *(&task_0 + 6) = 63496;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x41D43D71;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x42960000;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
  task_0 = -1;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 10;
  *(&task_0 + 5) = 54;
  *(&task_0 + 6) = 63496;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x412D47AE;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x42AA3852;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
  task_0 = -1;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 10;
  *(&task_0 + 5) = 53;
  *(&task_0 + 6) = 63489;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x41311EB8;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x42A970A4;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
  task_0 = -1;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 10;
  *(&task_0 + 5) = 50;
  *(&task_0 + 6) = 2290;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x4106B852;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x42A43333;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
  task_0 = -1;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 10;
  *(&task_0 + 5) = 52;
  *(&task_0 + 6) = 2290;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x40FF0A3D;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x42A0DC29;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
  task_0 = -1;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 10;
  *(&task_0 + 5) = 54;
  *(&task_0 + 6) = 2290;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x41066666;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x4299947B;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
  task_0 = -1;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 10;
  *(&task_0 + 5) = 53;
  *(&task_0 + 6) = 2287;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x41C251EC;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x428F147B;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
  task_0 = -1;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 10;
  *(&task_0 + 5) = 53;
  *(&task_0 + 6) = 63533;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x41BA147B;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x428B051F;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
  task_0 = -1;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 10;
  *(&task_0 + 5) = 54;
  *(&task_0 + 6) = 63533;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x419E3D71;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x429F6666;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
  task_0 = 0x4D544953;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 150;
  *(&task_0 + 5) = 13;
  *(&task_0 + 6) = 0;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(SHEN_NAME('SITM'), VF_FORMATION_APPLY_FLAGS, 0x200000, 0x800);
  Script_vfConfigureFormationGroup(SHEN_NAME('SITM'), VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(SHEN_NAME('SITM'), VF_FORMATION_APPLY_FLAGS, 0x2000000, 0);
  task_0 = -1;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 20;
  *(&task_0 + 5) = 50;
  *(&task_0 + 6) = 2289;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x412570A4;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x428BFAE1;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
  task_0 = -1;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 20;
  *(&task_0 + 5) = 51;
  *(&task_0 + 6) = 2289;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x416A3D71;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x42883D71;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
  task_0 = -1;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 20;
  *(&task_0 + 5) = 30;
  *(&task_0 + 6) = 63496;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x41E770A4;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x4285AE14;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 1);
  task_0 = -1;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 20;
  *(&task_0 + 5) = 52;
  *(&task_0 + 6) = 63489;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x41DB999A;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x42888A3D;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 1);
  task_0 = -1;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 20;
  *(&task_0 + 5) = 52;
  *(&task_0 + 6) = 63533;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x41CECCCD;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x42869EB8;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 1);
  task_0 = -1;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 15;
  *(&task_0 + 5) = 52;
  *(&task_0 + 6) = 63533;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x418F0A3D;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x4293947B;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 1);
  task_0 = -1;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 15;
  *(&task_0 + 5) = 55;
  *(&task_0 + 6) = 63496;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x41728F5C;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x42A151EC;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 1);
  task_0 = -1;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 15;
  *(&task_0 + 5) = 52;
  *(&task_0 + 6) = 63533;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x41675C29;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x4297CCCD;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 1);
  task_0 = -1;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 15;
  *(&task_0 + 5) = 52;
  *(&task_0 + 6) = 2290;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x41775C29;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x429ACCCD;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 1);
  task_0 = -1;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 15;
  *(&task_0 + 5) = 55;
  *(&task_0 + 6) = 63496;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x41970A3D;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x42808A3D;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 1);
  task_0 = -1;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 20;
  *(&task_0 + 5) = 30;
  *(&task_0 + 6) = 2290;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x41E2E148;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x4287BD71;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 1);
  task_0 = -1;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 20;
  *(&task_0 + 5) = 53;
  *(&task_0 + 6) = 63489;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x41D60000;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x428AF0A4;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 1);
  task_0 = -1;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 20;
  *(&task_0 + 5) = 52;
  *(&task_0 + 6) = 63533;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x41D6F5C3;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x42853D71;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 1);
  task_0 = -1;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 20;
  *(&task_0 + 5) = 54;
  *(&task_0 + 6) = 63533;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x420470A4;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x42866B85;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 1);
  task_0 = -1;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 20;
  *(&task_0 + 5) = 51;
  *(&task_0 + 6) = 2288;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x41FAB852;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x4280B333;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 1);
  task_0 = -1;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 20;
  *(&task_0 + 5) = 53;
  *(&task_0 + 6) = 63533;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x41A33333;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x42822E14;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 1);
  task_0 = -1;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 20;
  *(&task_0 + 5) = 51;
  *(&task_0 + 6) = 63489;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x41A35C29;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x4276999A;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 1);
  task_0 = -1;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 20;
  *(&task_0 + 5) = 50;
  *(&task_0 + 6) = 2287;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x419D1EB8;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x428128F6;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 1);
  task_0 = -1;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 20;
  *(&task_0 + 5) = 54;
  *(&task_0 + 6) = 2288;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x41B31EB8;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x42833D71;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 1);
  task_0 = -1;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 20;
  *(&task_0 + 5) = 50;
  *(&task_0 + 6) = 63533;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x419251EC;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x42810A3D;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 1);
  task_0 = -1;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 15;
  *(&task_0 + 5) = 54;
  *(&task_0 + 6) = 63533;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x41F6147B;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x424EEB85;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 1);
  task_0 = -1;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 15;
  *(&task_0 + 5) = 52;
  *(&task_0 + 6) = 2288;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x41F68F5C;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x428551EC;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 1);
  task_0 = -1;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 15;
  *(&task_0 + 5) = 50;
  *(&task_0 + 6) = 2287;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x4218CCCD;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x42517AE1;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 1);
  task_0 = -1;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 15;
  *(&task_0 + 5) = 52;
  *(&task_0 + 6) = 63489;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x420F7AE1;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x4262CCCD;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 1);
  task_0 = -1;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 15;
  *(&task_0 + 5) = 50;
  *(&task_0 + 6) = 63533;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x42157AE1;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x4269CCCD;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 1);
  task_0 = -1;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 20;
  *(&task_0 + 5) = 51;
  *(&task_0 + 6) = 2288;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x4221147B;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x427647AE;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 1);
  task_0 = -1;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 20;
  *(&task_0 + 5) = 52;
  *(&task_0 + 6) = 2287;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x4200851F;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x4274E148;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 1);
  task_0 = -1;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 20;
  *(&task_0 + 5) = 55;
  *(&task_0 + 6) = 2290;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x42180A3D;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x426C8F5C;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 1);
  task_0 = -1;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 20;
  *(&task_0 + 5) = 52;
  *(&task_0 + 6) = 2287;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x420A28F6;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x424E3333;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 1);
  task_0 = -1;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 20;
  *(&task_0 + 5) = 56;
  *(&task_0 + 6) = 63533;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x4213D70A;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x4240CCCD;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 1);
  task_0 = 0x4F544153;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 250;
  *(&task_0 + 5) = 17;
  *(&task_0 + 6) = 0;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(CHARA_SATO, VF_FORMATION_APPLY_FLAGS, 0x200000, 0x800);
  Script_vfConfigureFormationGroup(CHARA_SATO, VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(CHARA_SATO, VF_FORMATION_APPLY_FLAGS, 0x2000000, 0);
  task_0 = -1;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 10;
  *(&task_0 + 5) = 50;
  *(&task_0 + 6) = 63496;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x422BD70A;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x4252CCCD;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 1);
  task_0 = -1;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 10;
  *(&task_0 + 5) = 52;
  *(&task_0 + 6) = 2287;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x42433D71;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x424A6666;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 1);
  task_0 = -1;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 10;
  *(&task_0 + 5) = 50;
  *(&task_0 + 6) = 63489;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x425E51EC;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x4225999A;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 1);
  task_0 = -1;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 10;
  *(&task_0 + 5) = 54;
  *(&task_0 + 6) = 63533;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x423628F6;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x4241C28F;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 1);
  task_0 = -1;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 10;
  *(&task_0 + 5) = 51;
  *(&task_0 + 6) = 2287;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x4245147B;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x42500A3D;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 1);
  task_0 = -1;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 10;
  *(&task_0 + 5) = 51;
  *(&task_0 + 6) = 2290;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x424D1EB8;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x42475C29;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 1);
  task_0 = -1;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 10;
  *(&task_0 + 5) = 54;
  *(&task_0 + 6) = 63533;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x42347AE1;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x422CCCCD;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 1);
  task_0 = -1;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 10;
  *(&task_0 + 5) = 51;
  *(&task_0 + 6) = 2287;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x423FC28F;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x4227999A;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 1);
  task_0 = -1;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 10;
  *(&task_0 + 5) = 54;
  *(&task_0 + 6) = 2287;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x42388F5C;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x422F6666;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 1);
  task_0 = -1;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 10;
  *(&task_0 + 5) = 50;
  *(&task_0 + 6) = 63489;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x4222851F;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x423D3D71;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 1);
  task_0 = -1;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 10;
  *(&task_0 + 5) = 51;
  *(&task_0 + 6) = 2287;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x41F4147B;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x4284199A;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 1);
  task_0 = -1;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 10;
  *(&task_0 + 5) = 54;
  *(&task_0 + 6) = 63533;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x4237CCCD;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x4242AE14;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 1);
  task_0 = -1;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 10;
  *(&task_0 + 5) = 50;
  *(&task_0 + 6) = 2290;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x4245AE14;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x4245C28F;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 1);
  task_0 = -1;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 10;
  *(&task_0 + 5) = 51;
  *(&task_0 + 6) = 2287;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x424F47AE;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x424328F6;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 1);
  task_0 = -1;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 10;
  *(&task_0 + 5) = 54;
  *(&task_0 + 6) = 63533;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x425AAE14;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x423EC28F;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 1);
  task_0 = -1;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 10;
  *(&task_0 + 5) = 52;
  *(&task_0 + 6) = 2288;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x42650000;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x423D0000;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 1);
  task_0 = -1;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 10;
  *(&task_0 + 5) = 51;
  *(&task_0 + 6) = 2287;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x426151EC;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x423247AE;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 1);
  task_0 = -1;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 10;
  *(&task_0 + 5) = 51;
  *(&task_0 + 6) = 63533;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x425970A4;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x4223F5C3;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
  Script_vfConfigureFormationGroup(-1, VF_FORMATION_APPLY_FLAGS, 0x200000, 1);
  task_0 = 0x52444550;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 500;
  *(&task_0 + 5) = 33;
  *(&task_0 + 6) = 0;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(CHARA_PEDR, VF_FORMATION_APPLY_FLAGS, 0x200000, 0x800);
  Script_vfConfigureFormationGroup(CHARA_PEDR, VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(CHARA_PEDR, VF_FORMATION_APPLY_FLAGS, 0x2000000, 0);
  return;
}

static void script_E723(void) {
  task_0 = 0x3253494B;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 500;
  *(&task_0 + 5) = 20;
  *(&task_0 + 6) = 0;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(SHEN_NAME('KIS2'), VF_FORMATION_APPLY_FLAGS, 0x200000, 0x800);
  Script_vfConfigureFormationGroup(SHEN_NAME('KIS2'), VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
  Script_vfConfigureFormationGroup(SHEN_NAME('KIS2'), VF_FORMATION_APPLY_FLAGS, VF_HP_TEAM_BASED, 70);
  Script_vfConfigureFormationGroup(CHARA_AKIR, VF_FORMATION_APPLY_FLAGS, VF_HP_TEAM_BASED, 70);
  return;
}

static void script_E7EA(void) {
  task_0 = -2;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 0;
  *(&task_0 + 3) = 0;
  *(&task_0 + 4) = 0;
  *(&task_0 + 5) = 0;
  *(&task_0 + 6) = 0;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x41C4CCCD;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x429C6666;
  Script_vfUtils(&task_0);
  task_0 = -2;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 0;
  *(&task_0 + 3) = 0;
  *(&task_0 + 4) = 0;
  *(&task_0 + 5) = 0;
  *(&task_0 + 6) = 0;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x416CCCCD;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x42A23333;
  Script_vfUtils(&task_0);
  task_0 = -2;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 0;
  *(&task_0 + 3) = 0;
  *(&task_0 + 4) = 0;
  *(&task_0 + 5) = 0;
  *(&task_0 + 6) = 0;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x41CE6666;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x4282CCCD;
  Script_vfUtils(&task_0);
  task_0 = -2;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 0;
  *(&task_0 + 3) = 0;
  *(&task_0 + 4) = 0;
  *(&task_0 + 5) = 0;
  *(&task_0 + 6) = 0;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x4131999A;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x4296999A;
  Script_vfUtils(&task_0);
  task_0 = -2;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 0;
  *(&task_0 + 3) = 0;
  *(&task_0 + 4) = 0;
  *(&task_0 + 5) = 0;
  *(&task_0 + 6) = 0;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x41833333;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x428A999A;
  Script_vfUtils(&task_0);
  task_0 = -2;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 0;
  *(&task_0 + 3) = 0;
  *(&task_0 + 4) = 0;
  *(&task_0 + 5) = 0;
  *(&task_0 + 6) = 0;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x41B1999A;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x42740000;
  Script_vfUtils(&task_0);
  task_0 = -2;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 0;
  *(&task_0 + 3) = 0;
  *(&task_0 + 4) = 0;
  *(&task_0 + 5) = 0;
  *(&task_0 + 6) = 0;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x41D1999A;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x4295CCCD;
  Script_vfUtils(&task_0);
  task_0 = -2;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 0;
  *(&task_0 + 3) = 0;
  *(&task_0 + 4) = 0;
  *(&task_0 + 5) = 0;
  *(&task_0 + 6) = 0;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x418F3333;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x429B999A;
  Script_vfUtils(&task_0);
  task_0 = -2;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 0;
  *(&task_0 + 3) = 0;
  *(&task_0 + 4) = 0;
  *(&task_0 + 5) = 0;
  *(&task_0 + 6) = 0;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x421A6666;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x427ACCCD;
  Script_vfUtils(&task_0);
  task_0 = -2;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 0;
  *(&task_0 + 3) = 0;
  *(&task_0 + 4) = 0;
  *(&task_0 + 5) = 0;
  *(&task_0 + 6) = 0;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x41FE6666;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x4281CCCD;
  Script_vfUtils(&task_0);
  task_0 = -2;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 0;
  *(&task_0 + 3) = 0;
  *(&task_0 + 4) = 0;
  *(&task_0 + 5) = 0;
  *(&task_0 + 6) = 0;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x4211999A;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x425A0000;
  Script_vfUtils(&task_0);
  task_0 = -2;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 0;
  *(&task_0 + 3) = 0;
  *(&task_0 + 4) = 0;
  *(&task_0 + 5) = 0;
  *(&task_0 + 6) = 0;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x4233999A;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x423ACCCD;
  Script_vfUtils(&task_0);
  task_0 = -2;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 0;
  *(&task_0 + 3) = 0;
  *(&task_0 + 4) = 0;
  *(&task_0 + 5) = 0;
  *(&task_0 + 6) = 0;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x4246CCCD;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x42440000;
  Script_vfUtils(&task_0);
  task_0 = -2;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 0;
  *(&task_0 + 3) = 0;
  *(&task_0 + 4) = 0;
  *(&task_0 + 5) = 0;
  *(&task_0 + 6) = 0;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x425F3333;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x423B3333;
  Script_vfUtils(&task_0);
  task_0 = -2;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 0;
  *(&task_0 + 3) = 0;
  *(&task_0 + 4) = 0;
  *(&task_0 + 5) = 0;
  *(&task_0 + 6) = 0;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x424C0000;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x42173333;
  Script_vfUtils(&task_0);
  task_0 = -2;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 0;
  *(&task_0 + 3) = 0;
  *(&task_0 + 4) = 0;
  *(&task_0 + 5) = 0;
  *(&task_0 + 6) = 0;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x42493333;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x423A6666;
  Script_vfUtils(&task_0);
  task_0 = -2;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 0;
  *(&task_0 + 3) = 0;
  *(&task_0 + 4) = 0;
  *(&task_0 + 5) = 0;
  *(&task_0 + 6) = 0;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x4265999A;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x42306666;
  Script_vfUtils(&task_0);
  task_0 = -4;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 0;
  *(&task_0 + 3) = 0;
  *(&task_0 + 4) = 0;
  *(&task_0 + 5) = 0;
  *(&task_0 + 6) = 0;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0x425047AE;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0x421C0000;
  Script_vfUtils(&task_0);
  *((int8_t *)&task_2C + 1) = 1;
  *((int8_t *)&task_2C + 2) = 1;
  *(int32_t *)(&task_2C + 8) = 0x41C40000;
  *(int32_t *)(&task_2C + 12) = 0x42CD6666;
  *(int32_t *)(&task_2C + 16) = 0x41E0CCCD;
  *(int32_t *)(&task_2C + 20) = 0x42C5CCCD;
  *(int32_t *)(&task_2C + 24) = 0x420D3333;
  *(int32_t *)(&task_2C + 28) = 0x42D3CCCD;
  *(int32_t *)(&task_2C + 32) = 0x41F8CCCD;
  *(int32_t *)(&task_2C + 36) = 0x42DB999A;
  *(int32_t *)(&task_2C + 40) = 0;
  *(int32_t *)(&task_2C + 44) = 0;
  *(int32_t *)(&task_2C + 48) = 79;
  *(int32_t *)(&task_2C + 52) = 0;
  *(int32_t *)(&task_2C + 60) = 0;
  Script_vfSetupAreaMasks(0, &task_2C);
  *((int8_t *)&task_2C + 1) = 1;
  *((int8_t *)&task_2C + 2) = 1;
  *(int32_t *)(&task_2C + 8) = 0x41E0CCCD;
  *(int32_t *)(&task_2C + 12) = 0x42C5CCCD;
  *(int32_t *)(&task_2C + 16) = 0x41FE6666;
  *(int32_t *)(&task_2C + 20) = 0x42BE999A;
  *(int32_t *)(&task_2C + 24) = 0x421ACCCD;
  *(int32_t *)(&task_2C + 28) = 0x42CD3333;
  *(int32_t *)(&task_2C + 32) = 0x420D3333;
  *(int32_t *)(&task_2C + 36) = 0x42D3CCCD;
  *(int32_t *)(&task_2C + 40) = 0;
  *(int32_t *)(&task_2C + 44) = 0;
  *(int32_t *)(&task_2C + 48) = 79;
  *(int32_t *)(&task_2C + 52) = 0;
  *(int32_t *)(&task_2C + 60) = 0;
  Script_vfSetupAreaMasks(1, &task_2C);
  *((int8_t *)&task_2C + 1) = 1;
  *((int8_t *)&task_2C + 2) = 1;
  *(int32_t *)(&task_2C + 8) = 0x41FE6666;
  *(int32_t *)(&task_2C + 12) = 0x42BE999A;
  *(int32_t *)(&task_2C + 16) = 0x420B3333;
  *(int32_t *)(&task_2C + 20) = 0x42B90000;
  *(int32_t *)(&task_2C + 24) = 0x4227999A;
  *(int32_t *)(&task_2C + 28) = 0x42C76666;
  *(int32_t *)(&task_2C + 32) = 0x421ACCCD;
  *(int32_t *)(&task_2C + 36) = 0x42CD3333;
  *(int32_t *)(&task_2C + 40) = 0;
  *(int32_t *)(&task_2C + 44) = 0;
  *(int32_t *)(&task_2C + 48) = 79;
  *(int32_t *)(&task_2C + 52) = 0;
  *(int32_t *)(&task_2C + 60) = 0;
  Script_vfSetupAreaMasks(2, &task_2C);
  *((int8_t *)&task_2C + 1) = 1;
  *((int8_t *)&task_2C + 2) = 1;
  *(int32_t *)(&task_2C + 8) = 0x420B3333;
  *(int32_t *)(&task_2C + 12) = 0x42B90000;
  *(int32_t *)(&task_2C + 16) = 0x42113333;
  *(int32_t *)(&task_2C + 20) = 0x42B8999A;
  *(int32_t *)(&task_2C + 24) = 0x422D999A;
  *(int32_t *)(&task_2C + 28) = 0x42C03333;
  *(int32_t *)(&task_2C + 32) = 0x4227999A;
  *(int32_t *)(&task_2C + 36) = 0x42C76666;
  *(int32_t *)(&task_2C + 40) = 1;
  *(int32_t *)(&task_2C + 44) = 0;
  *(int32_t *)(&task_2C + 48) = 95;
  *(int32_t *)(&task_2C + 52) = 0;
  *(int32_t *)(&task_2C + 60) = 0;
  Script_vfSetupAreaMasks(3, &task_2C);
  *((int8_t *)&task_2C + 1) = 1;
  *((int8_t *)&task_2C + 2) = 1;
  *(int32_t *)(&task_2C + 8) = 0x4227999A;
  *(int32_t *)(&task_2C + 12) = 0x42C76666;
  *(int32_t *)(&task_2C + 16) = 0x422D999A;
  *(int32_t *)(&task_2C + 20) = 0x42C03333;
  *(int32_t *)(&task_2C + 24) = 0x4238CCCD;
  *(int32_t *)(&task_2C + 28) = 0x42C50000;
  *(int32_t *)(&task_2C + 32) = 0x422E6666;
  *(int32_t *)(&task_2C + 36) = 0x42CA6666;
  *(int32_t *)(&task_2C + 40) = 1;
  *(int32_t *)(&task_2C + 44) = 0;
  *(int32_t *)(&task_2C + 48) = 56;
  *(int32_t *)(&task_2C + 52) = 0;
  *(int32_t *)(&task_2C + 60) = 0;
  Script_vfSetupAreaMasks(4, &task_2C);
  *((int8_t *)&task_2C + 1) = 1;
  *((int8_t *)&task_2C + 2) = 1;
  *(int32_t *)(&task_2C + 8) = 0x422D999A;
  *(int32_t *)(&task_2C + 12) = 0x42C03333;
  *(int32_t *)(&task_2C + 16) = 0x4233999A;
  *(int32_t *)(&task_2C + 20) = 0x42BA999A;
  *(int32_t *)(&task_2C + 24) = 0x42406666;
  *(int32_t *)(&task_2C + 28) = 0x42C10000;
  *(int32_t *)(&task_2C + 32) = 0x4238CCCD;
  *(int32_t *)(&task_2C + 36) = 0x42C50000;
  *(int32_t *)(&task_2C + 40) = 1;
  *(int32_t *)(&task_2C + 44) = 0;
  *(int32_t *)(&task_2C + 48) = 48;
  *(int32_t *)(&task_2C + 52) = 0;
  *(int32_t *)(&task_2C + 60) = 0;
  Script_vfSetupAreaMasks(5, &task_2C);
  *((int8_t *)&task_2C + 1) = 1;
  *((int8_t *)&task_2C + 2) = 1;
  *(int32_t *)(&task_2C + 8) = 0x42113333;
  *(int32_t *)(&task_2C + 12) = 0x42B8999A;
  *(int32_t *)(&task_2C + 16) = 0x421D3333;
  *(int32_t *)(&task_2C + 20) = 0x42B40000;
  *(int32_t *)(&task_2C + 24) = 0x422F3333;
  *(int32_t *)(&task_2C + 28) = 0x42BDCCCD;
  *(int32_t *)(&task_2C + 32) = 0x422D999A;
  *(int32_t *)(&task_2C + 36) = 0x42C03333;
  *(int32_t *)(&task_2C + 40) = 1;
  *(int32_t *)(&task_2C + 44) = 0;
  *(int32_t *)(&task_2C + 48) = 207;
  *(int32_t *)(&task_2C + 52) = 0;
  *(int32_t *)(&task_2C + 60) = 0;
  Script_vfSetupAreaMasks(6, &task_2C);
  *((int8_t *)&task_2C + 1) = 1;
  *((int8_t *)&task_2C + 2) = 1;
  *(int32_t *)(&task_2C + 8) = 0x42026666;
  *(int32_t *)(&task_2C + 12) = 0x42B16666;
  *(int32_t *)(&task_2C + 16) = 0x420E6666;
  *(int32_t *)(&task_2C + 20) = 0x42AC3333;
  *(int32_t *)(&task_2C + 24) = 0x421D3333;
  *(int32_t *)(&task_2C + 28) = 0x42B40000;
  *(int32_t *)(&task_2C + 32) = 0x42113333;
  *(int32_t *)(&task_2C + 36) = 0x42B8999A;
  *(int32_t *)(&task_2C + 40) = 3;
  *(int32_t *)(&task_2C + 44) = 0;
  *(int32_t *)(&task_2C + 48) = 192;
  *(int32_t *)(&task_2C + 52) = 0;
  *(int32_t *)(&task_2C + 60) = 0;
  Script_vfSetupAreaMasks(7, &task_2C);
  *((int8_t *)&task_2C + 1) = 1;
  *((int8_t *)&task_2C + 2) = 1;
  *(int32_t *)(&task_2C + 8) = 0x41F26666;
  *(int32_t *)(&task_2C + 12) = 0x42B00000;
  *(int32_t *)(&task_2C + 16) = 0x42060000;
  *(int32_t *)(&task_2C + 20) = 0x42A8999A;
  *(int32_t *)(&task_2C + 24) = 0x420E6666;
  *(int32_t *)(&task_2C + 28) = 0x42AC3333;
  *(int32_t *)(&task_2C + 32) = 0x42026666;
  *(int32_t *)(&task_2C + 36) = 0x42B16666;
  *(int32_t *)(&task_2C + 40) = 7;
  *(int32_t *)(&task_2C + 44) = 0;
  *(int32_t *)(&task_2C + 48) = 0x3C0300;
  *(int32_t *)(&task_2C + 52) = 0;
  *(int32_t *)(&task_2C + 60) = 0;
  Script_vfSetupAreaMasks(8, &task_2C);
  *((int8_t *)&task_2C + 1) = 1;
  *((int8_t *)&task_2C + 2) = 1;
  *(int32_t *)(&task_2C + 8) = 0x41C26666;
  *(int32_t *)(&task_2C + 12) = 0x42A40000;
  *(int32_t *)(&task_2C + 16) = 0x41DA6666;
  *(int32_t *)(&task_2C + 20) = 0x429C0000;
  *(int32_t *)(&task_2C + 24) = 0x42060000;
  *(int32_t *)(&task_2C + 28) = 0x42A8999A;
  *(int32_t *)(&task_2C + 32) = 0x41F26666;
  *(int32_t *)(&task_2C + 36) = 0x42B00000;
  *(int32_t *)(&task_2C + 40) = 7;
  *(int32_t *)(&task_2C + 44) = 0;
  *(int32_t *)(&task_2C + 48) = 0x3C0300;
  *(int32_t *)(&task_2C + 52) = 0;
  *(int32_t *)(&task_2C + 60) = 0;
  Script_vfSetupAreaMasks(9, &task_2C);
  *((int8_t *)&task_2C + 1) = 1;
  *((int8_t *)&task_2C + 2) = 1;
  *(int32_t *)(&task_2C + 8) = 0x4121999A;
  *(int32_t *)(&task_2C + 12) = 0x42B46666;
  *(int32_t *)(&task_2C + 16) = 0x4154CCCD;
  *(int32_t *)(&task_2C + 20) = 0x42AD999A;
  *(int32_t *)(&task_2C + 24) = 0x41873333;
  *(int32_t *)(&task_2C + 28) = 0x42B46666;
  *(int32_t *)(&task_2C + 32) = 0x41580000;
  *(int32_t *)(&task_2C + 36) = 0x42BA999A;
  *(int32_t *)(&task_2C + 40) = 255;
  *(int32_t *)(&task_2C + 44) = 0;
  *(int32_t *)(&task_2C + 48) = 0x3C00;
  *(int32_t *)(&task_2C + 52) = 0;
  *(int32_t *)(&task_2C + 60) = 0;
  Script_vfSetupAreaMasks(10, &task_2C);
  *((int8_t *)&task_2C + 1) = 1;
  *((int8_t *)&task_2C + 2) = 1;
  *(int32_t *)(&task_2C + 8) = 0x4099999A;
  *(int32_t *)(&task_2C + 12) = 0x42A2999A;
  *(int32_t *)(&task_2C + 16) = 0x40ACCCCD;
  *(int32_t *)(&task_2C + 20) = 0x42A0999A;
  *(int32_t *)(&task_2C + 24) = 0x4154CCCD;
  *(int32_t *)(&task_2C + 28) = 0x42AD999A;
  *(int32_t *)(&task_2C + 32) = 0x4121999A;
  *(int32_t *)(&task_2C + 36) = 0x42B46666;
  *(int32_t *)(&task_2C + 40) = 255;
  *(int32_t *)(&task_2C + 44) = 0;
  *(int32_t *)(&task_2C + 48) = 0x7C00;
  *(int32_t *)(&task_2C + 52) = 0;
  *(int32_t *)(&task_2C + 60) = 0;
  Script_vfSetupAreaMasks(11, &task_2C);
  *((int8_t *)&task_2C + 1) = 1;
  *((int8_t *)&task_2C + 2) = 1;
  *(int32_t *)(&task_2C + 8) = 0x3F666666;
  *(int32_t *)(&task_2C + 12) = 0x42A96666;
  *(int32_t *)(&task_2C + 16) = 0x4099999A;
  *(int32_t *)(&task_2C + 20) = 0x42A2999A;
  *(int32_t *)(&task_2C + 24) = 0x4121999A;
  *(int32_t *)(&task_2C + 28) = 0x42B46666;
  *(int32_t *)(&task_2C + 32) = 0x4104CCCD;
  *(int32_t *)(&task_2C + 36) = 0x42B83333;
  *(int32_t *)(&task_2C + 40) = 255;
  *(int32_t *)(&task_2C + 44) = 0;
  *(int32_t *)(&task_2C + 48) = 0x7C00;
  *(int32_t *)(&task_2C + 52) = 0;
  *(int32_t *)(&task_2C + 60) = 0;
  Script_vfSetupAreaMasks(12, &task_2C);
  *((int8_t *)&task_2C + 1) = 1;
  *((int8_t *)&task_2C + 2) = 1;
  *(int32_t *)(&task_2C + 8) = -0x3FD9999A;
  *(int32_t *)(&task_2C + 12) = 0x42A1CCCD;
  *(int32_t *)(&task_2C + 16) = 0x3FC00000;
  *(int32_t *)(&task_2C + 20) = 0x429A999A;
  *(int32_t *)(&task_2C + 24) = 0x4099999A;
  *(int32_t *)(&task_2C + 28) = 0x42A2999A;
  *(int32_t *)(&task_2C + 32) = 0x3F666666;
  *(int32_t *)(&task_2C + 36) = 0x42A96666;
  *(int32_t *)(&task_2C + 40) = 255;
  *(int32_t *)(&task_2C + 44) = 0;
  *(int32_t *)(&task_2C + 48) = 0x3800;
  *(int32_t *)(&task_2C + 52) = 0;
  *(int32_t *)(&task_2C + 60) = 0;
  Script_vfSetupAreaMasks(13, &task_2C);
  *((int8_t *)&task_2C + 1) = 1;
  *((int8_t *)&task_2C + 2) = 1;
  *(int32_t *)(&task_2C + 8) = 0x40ACCCCD;
  *(int32_t *)(&task_2C + 12) = 0x42A0999A;
  *(int32_t *)(&task_2C + 16) = 0x40F33333;
  *(int32_t *)(&task_2C + 20) = 0x429C999A;
  *(int32_t *)(&task_2C + 24) = 0x417CCCCD;
  *(int32_t *)(&task_2C + 28) = 0x42A86666;
  *(int32_t *)(&task_2C + 32) = 0x4154CCCD;
  *(int32_t *)(&task_2C + 36) = 0x42AD999A;
  *(int32_t *)(&task_2C + 40) = 255;
  *(int32_t *)(&task_2C + 44) = 0;
  *(int32_t *)(&task_2C + 48) = 88064;
  *(int32_t *)(&task_2C + 52) = 0;
  *(int32_t *)(&task_2C + 60) = 0;
  Script_vfSetupAreaMasks(14, &task_2C);
  *((int8_t *)&task_2C + 1) = 1;
  *((int8_t *)&task_2C + 2) = 1;
  *(int32_t *)(&task_2C + 8) = 0x417CCCCD;
  *(int32_t *)(&task_2C + 12) = 0x42A86666;
  *(int32_t *)(&task_2C + 16) = 0x41A73333;
  *(int32_t *)(&task_2C + 20) = 0x429E6666;
  *(int32_t *)(&task_2C + 24) = 0x41C26666;
  *(int32_t *)(&task_2C + 28) = 0x42A40000;
  *(int32_t *)(&task_2C + 32) = 0x4198CCCD;
  *(int32_t *)(&task_2C + 36) = 0x42AF3333;
  *(int32_t *)(&task_2C + 40) = 255;
  *(int32_t *)(&task_2C + 44) = 0;
  *(int32_t *)(&task_2C + 48) = 0x1DF8000;
  *(int32_t *)(&task_2C + 52) = 0;
  *(int32_t *)(&task_2C + 60) = 0;
  Script_vfSetupAreaMasks(15, &task_2C);
  *((int8_t *)&task_2C + 1) = 1;
  *((int8_t *)&task_2C + 2) = 1;
  *(int32_t *)(&task_2C + 8) = 0x40F33333;
  *(int32_t *)(&task_2C + 12) = 0x429C999A;
  *(int32_t *)(&task_2C + 16) = 0x41580000;
  *(int32_t *)(&task_2C + 20) = 0x4294999A;
  *(int32_t *)(&task_2C + 24) = 0x41A73333;
  *(int32_t *)(&task_2C + 28) = 0x429E6666;
  *(int32_t *)(&task_2C + 32) = 0x417CCCCD;
  *(int32_t *)(&task_2C + 36) = 0x42A86666;
  *(int32_t *)(&task_2C + 40) = 255;
  *(int32_t *)(&task_2C + 44) = 0;
  *(int32_t *)(&task_2C + 48) = 0x1DFC000;
  *(int32_t *)(&task_2C + 52) = 0;
  *(int32_t *)(&task_2C + 60) = 0;
  Script_vfSetupAreaMasks(16, &task_2C);
  *((int8_t *)&task_2C + 1) = 1;
  *((int8_t *)&task_2C + 2) = 1;
  *(int32_t *)(&task_2C + 8) = 0x406CCCCD;
  *(int32_t *)(&task_2C + 12) = 0x42960000;
  *(int32_t *)(&task_2C + 16) = 0x41066666;
  *(int32_t *)(&task_2C + 20) = 0x428E3333;
  *(int32_t *)(&task_2C + 24) = 0x41580000;
  *(int32_t *)(&task_2C + 28) = 0x4294999A;
  *(int32_t *)(&task_2C + 32) = 0x40F33333;
  *(int32_t *)(&task_2C + 36) = 0x429C999A;
  *(int32_t *)(&task_2C + 40) = 255;
  *(int32_t *)(&task_2C + 44) = 0;
  *(int32_t *)(&task_2C + 48) = 0x1DF8000;
  *(int32_t *)(&task_2C + 52) = 0;
  *(int32_t *)(&task_2C + 60) = 0;
  Script_vfSetupAreaMasks(17, &task_2C);
  *((int8_t *)&task_2C + 1) = 1;
  *((int8_t *)&task_2C + 2) = 1;
  *(int32_t *)(&task_2C + 8) = 0x41A73333;
  *(int32_t *)(&task_2C + 12) = 0x429E6666;
  *(int32_t *)(&task_2C + 16) = 0x41C26666;
  *(int32_t *)(&task_2C + 20) = 0x42980000;
  *(int32_t *)(&task_2C + 24) = 0x41DA6666;
  *(int32_t *)(&task_2C + 28) = 0x429C0000;
  *(int32_t *)(&task_2C + 32) = 0x41C26666;
  *(int32_t *)(&task_2C + 36) = 0x42A40000;
  *(int32_t *)(&task_2C + 40) = 255;
  *(int32_t *)(&task_2C + 44) = 0;
  *(int32_t *)(&task_2C + 48) = 0x1DF8200;
  *(int32_t *)(&task_2C + 52) = 0;
  *(int32_t *)(&task_2C + 60) = 0;
  Script_vfSetupAreaMasks(18, &task_2C);
  *((int8_t *)&task_2C + 1) = 1;
  *((int8_t *)&task_2C + 2) = 1;
  *(int32_t *)(&task_2C + 8) = 0x41580000;
  *(int32_t *)(&task_2C + 12) = 0x4294999A;
  *(int32_t *)(&task_2C + 16) = 0x417E6666;
  *(int32_t *)(&task_2C + 20) = 0x4290999A;
  *(int32_t *)(&task_2C + 24) = 0x41C26666;
  *(int32_t *)(&task_2C + 28) = 0x42980000;
  *(int32_t *)(&task_2C + 32) = 0x41A73333;
  *(int32_t *)(&task_2C + 36) = 0x429E6666;
  *(int32_t *)(&task_2C + 40) = 255;
  *(int32_t *)(&task_2C + 44) = 0;
  *(int32_t *)(&task_2C + 48) = 0x1DF8200;
  *(int32_t *)(&task_2C + 52) = 0;
  *(int32_t *)(&task_2C + 60) = 0;
  Script_vfSetupAreaMasks(19, &task_2C);
  *((int8_t *)&task_2C + 1) = 1;
  *((int8_t *)&task_2C + 2) = 1;
  *(int32_t *)(&task_2C + 8) = 0x41066666;
  *(int32_t *)(&task_2C + 12) = 0x428E3333;
  *(int32_t *)(&task_2C + 16) = 0x4134CCCD;
  *(int32_t *)(&task_2C + 20) = 0x42896666;
  *(int32_t *)(&task_2C + 24) = 0x417E6666;
  *(int32_t *)(&task_2C + 28) = 0x4290999A;
  *(int32_t *)(&task_2C + 32) = 0x41580000;
  *(int32_t *)(&task_2C + 36) = 0x4294999A;
  *(int32_t *)(&task_2C + 40) = 255;
  *(int32_t *)(&task_2C + 44) = 0;
  *(int32_t *)(&task_2C + 48) = 0x1FF8000;
  *(int32_t *)(&task_2C + 52) = 0;
  *(int32_t *)(&task_2C + 60) = 0;
  Script_vfSetupAreaMasks(20, &task_2C);
  *((int8_t *)&task_2C + 1) = 1;
  *((int8_t *)&task_2C + 2) = 1;
  *(int32_t *)(&task_2C + 8) = 0x4099999A;
  *(int32_t *)(&task_2C + 12) = 0x42873333;
  *(int32_t *)(&task_2C + 16) = 0x40ECCCCD;
  *(int32_t *)(&task_2C + 20) = 0x42816666;
  *(int32_t *)(&task_2C + 24) = 0x4134CCCD;
  *(int32_t *)(&task_2C + 28) = 0x42896666;
  *(int32_t *)(&task_2C + 32) = 0x41066666;
  *(int32_t *)(&task_2C + 36) = 0x428E3333;
  *(int32_t *)(&task_2C + 40) = 255;
  *(int32_t *)(&task_2C + 44) = 0;
  *(int32_t *)(&task_2C + 48) = 0x300000;
  *(int32_t *)(&task_2C + 52) = 0;
  *(int32_t *)(&task_2C + 60) = 0;
  Script_vfSetupAreaMasks(21, &task_2C);
  *((int8_t *)&task_2C + 1) = 1;
  *((int8_t *)&task_2C + 2) = 1;
  *(int32_t *)(&task_2C + 8) = 0x41C26666;
  *(int32_t *)(&task_2C + 12) = 0x42980000;
  *(int32_t *)(&task_2C + 16) = 0x41E1999A;
  *(int32_t *)(&task_2C + 20) = 0x428D6666;
  *(int32_t *)(&task_2C + 24) = 0x41FB3333;
  *(int32_t *)(&task_2C + 28) = 0x42943333;
  *(int32_t *)(&task_2C + 32) = 0x41DA6666;
  *(int32_t *)(&task_2C + 36) = 0x429C0000;
  *(int32_t *)(&task_2C + 40) = 15615;
  *(int32_t *)(&task_2C + 44) = 0;
  *(int32_t *)(&task_2C + 48) = 0x1DF8000;
  *(int32_t *)(&task_2C + 52) = 0;
  *(int32_t *)(&task_2C + 60) = 0;
  Script_vfSetupAreaMasks(22, &task_2C);
  *((int8_t *)&task_2C + 1) = 1;
  *((int8_t *)&task_2C + 2) = 1;
  *(int32_t *)(&task_2C + 8) = 0x417E6666;
  *(int32_t *)(&task_2C + 12) = 0x4290999A;
  *(int32_t *)(&task_2C + 16) = 0x41A00000;
  *(int32_t *)(&task_2C + 20) = 0x4287CCCD;
  *(int32_t *)(&task_2C + 24) = 0x41E1999A;
  *(int32_t *)(&task_2C + 28) = 0x428D6666;
  *(int32_t *)(&task_2C + 32) = 0x41C26666;
  *(int32_t *)(&task_2C + 36) = 0x42980000;
  *(int32_t *)(&task_2C + 40) = 15615;
  *(int32_t *)(&task_2C + 44) = 0;
  *(int32_t *)(&task_2C + 48) = 0x37DF8000;
  *(int32_t *)(&task_2C + 52) = 0;
  *(int32_t *)(&task_2C + 60) = 0;
  Script_vfSetupAreaMasks(23, &task_2C);
  *((int8_t *)&task_2C + 1) = 1;
  *((int8_t *)&task_2C + 2) = 1;
  *(int32_t *)(&task_2C + 8) = 0x4134CCCD;
  *(int32_t *)(&task_2C + 12) = 0x42896666;
  *(int32_t *)(&task_2C + 16) = 0x4174CCCD;
  *(int32_t *)(&task_2C + 20) = 0x42813333;
  *(int32_t *)(&task_2C + 24) = 0x41A00000;
  *(int32_t *)(&task_2C + 28) = 0x4287CCCD;
  *(int32_t *)(&task_2C + 32) = 0x417E6666;
  *(int32_t *)(&task_2C + 36) = 0x4290999A;
  *(int32_t *)(&task_2C + 40) = 15615;
  *(int32_t *)(&task_2C + 44) = 0;
  *(int32_t *)(&task_2C + 48) = 0x37DF8000;
  *(int32_t *)(&task_2C + 52) = 0;
  *(int32_t *)(&task_2C + 60) = 0;
  Script_vfSetupAreaMasks(24, &task_2C);
  *((int8_t *)&task_2C + 1) = 1;
  *((int8_t *)&task_2C + 2) = 1;
  *(int32_t *)(&task_2C + 8) = 0x41A00000;
  *(int32_t *)(&task_2C + 12) = 0x4287CCCD;
  *(int32_t *)(&task_2C + 16) = 0x41B73333;
  *(int32_t *)(&task_2C + 20) = 0x4281999A;
  *(int32_t *)(&task_2C + 24) = 0x41F5999A;
  *(int32_t *)(&task_2C + 28) = 0x42886666;
  *(int32_t *)(&task_2C + 32) = 0x41E1999A;
  *(int32_t *)(&task_2C + 36) = 0x428D6666;
  *(int32_t *)(&task_2C + 40) = 31999;
  *(int32_t *)(&task_2C + 44) = 0;
  *(int32_t *)(&task_2C + 48) = -0x48608000;
  *(int32_t *)(&task_2C + 52) = 31;
  *(int32_t *)(&task_2C + 60) = 0;
  Script_vfSetupAreaMasks(25, &task_2C);
  *((int8_t *)&task_2C + 1) = 1;
  *((int8_t *)&task_2C + 2) = 1;
  *(int32_t *)(&task_2C + 8) = 0x4174CCCD;
  *(int32_t *)(&task_2C + 12) = 0x42813333;
  *(int32_t *)(&task_2C + 16) = 0x41900000;
  *(int32_t *)(&task_2C + 20) = 0x42733333;
  *(int32_t *)(&task_2C + 24) = 0x41B73333;
  *(int32_t *)(&task_2C + 28) = 0x4281999A;
  *(int32_t *)(&task_2C + 32) = 0x41A00000;
  *(int32_t *)(&task_2C + 36) = 0x4287CCCD;
  *(int32_t *)(&task_2C + 40) = 31999;
  *(int32_t *)(&task_2C + 44) = 0;
  *(int32_t *)(&task_2C + 48) = -0x48608000;
  *(int32_t *)(&task_2C + 52) = 31;
  *(int32_t *)(&task_2C + 60) = 0;
  Script_vfSetupAreaMasks(26, &task_2C);
  *((int8_t *)&task_2C + 1) = 1;
  *((int8_t *)&task_2C + 2) = 1;
  *(int32_t *)(&task_2C + 8) = 0x41F5999A;
  *(int32_t *)(&task_2C + 12) = 0x42886666;
  *(int32_t *)(&task_2C + 16) = 0x42053333;
  *(int32_t *)(&task_2C + 20) = 0x42826666;
  *(int32_t *)(&task_2C + 24) = 0x42140000;
  *(int32_t *)(&task_2C + 28) = 0x42893333;
  *(int32_t *)(&task_2C + 32) = 0x4208CCCD;
  *(int32_t *)(&task_2C + 36) = 0x428F999A;
  *(int32_t *)(&task_2C + 40) = 0x3FFFF;
  *(int32_t *)(&task_2C + 44) = 0;
  *(int32_t *)(&task_2C + 48) = -0x48000000;
  *(int32_t *)(&task_2C + 52) = 31;
  *(int32_t *)(&task_2C + 60) = 0;
  Script_vfSetupAreaMasks(27, &task_2C);
  *((int8_t *)&task_2C + 1) = 1;
  *((int8_t *)&task_2C + 2) = 1;
  *(int32_t *)(&task_2C + 8) = 0x41B73333;
  *(int32_t *)(&task_2C + 12) = 0x4281999A;
  *(int32_t *)(&task_2C + 16) = 0x41CB3333;
  *(int32_t *)(&task_2C + 20) = 0x42793333;
  *(int32_t *)(&task_2C + 24) = 0x42053333;
  *(int32_t *)(&task_2C + 28) = 0x42826666;
  *(int32_t *)(&task_2C + 32) = 0x41F5999A;
  *(int32_t *)(&task_2C + 36) = 0x42886666;
  *(int32_t *)(&task_2C + 40) = 0x3FFFF;
  *(int32_t *)(&task_2C + 44) = 0;
  *(int32_t *)(&task_2C + 48) = -0x40800000;
  *(int32_t *)(&task_2C + 52) = 31;
  *(int32_t *)(&task_2C + 60) = 0;
  Script_vfSetupAreaMasks(28, &task_2C);
  *((int8_t *)&task_2C + 1) = 1;
  *((int8_t *)&task_2C + 2) = 1;
  *(int32_t *)(&task_2C + 8) = 0x41900000;
  *(int32_t *)(&task_2C + 12) = 0x42733333;
  *(int32_t *)(&task_2C + 16) = 0x41A26666;
  *(int32_t *)(&task_2C + 20) = 0x426C0000;
  *(int32_t *)(&task_2C + 24) = 0x41CB3333;
  *(int32_t *)(&task_2C + 28) = 0x42793333;
  *(int32_t *)(&task_2C + 32) = 0x41B73333;
  *(int32_t *)(&task_2C + 36) = 0x4281999A;
  *(int32_t *)(&task_2C + 40) = 0x3FFFF;
  *(int32_t *)(&task_2C + 44) = 0;
  *(int32_t *)(&task_2C + 48) = -0x800000;
  *(int32_t *)(&task_2C + 52) = 31;
  *(int32_t *)(&task_2C + 60) = 0;
  Script_vfSetupAreaMasks(29, &task_2C);
  *((int8_t *)&task_2C + 1) = 1;
  *((int8_t *)&task_2C + 2) = 1;
  *(int32_t *)(&task_2C + 8) = 0x416CCCCD;
  *(int32_t *)(&task_2C + 12) = 0x42673333;
  *(int32_t *)(&task_2C + 16) = 0x41866666;
  *(int32_t *)(&task_2C + 20) = 0x425E6666;
  *(int32_t *)(&task_2C + 24) = 0x41A26666;
  *(int32_t *)(&task_2C + 28) = 0x426C0000;
  *(int32_t *)(&task_2C + 32) = 0x41900000;
  *(int32_t *)(&task_2C + 36) = 0x42733333;
  *(int32_t *)(&task_2C + 40) = 0x7FFFF;
  *(int32_t *)(&task_2C + 44) = 0;
  *(int32_t *)(&task_2C + 48) = 0x60000000;
  *(int32_t *)(&task_2C + 52) = 0;
  *(int32_t *)(&task_2C + 60) = 0;
  Script_vfSetupAreaMasks(30, &task_2C);
  *((int8_t *)&task_2C + 1) = 1;
  *((int8_t *)&task_2C + 2) = 1;
  *(int32_t *)(&task_2C + 8) = 0x42053333;
  *(int32_t *)(&task_2C + 12) = 0x42826666;
  *(int32_t *)(&task_2C + 16) = 0x4211999A;
  *(int32_t *)(&task_2C + 20) = 0x42766666;
  *(int32_t *)(&task_2C + 24) = 0x42213333;
  *(int32_t *)(&task_2C + 28) = 0x42820000;
  *(int32_t *)(&task_2C + 32) = 0x42140000;
  *(int32_t *)(&task_2C + 36) = 0x42893333;
  *(int32_t *)(&task_2C + 40) = 0x3FFFFF;
  *(int32_t *)(&task_2C + 44) = 0;
  *(int32_t *)(&task_2C + 48) = -0x48000000;
  *(int32_t *)(&task_2C + 52) = 31;
  *(int32_t *)(&task_2C + 60) = 0;
  Script_vfSetupAreaMasks(31, &task_2C);
  *((int8_t *)&task_2C + 1) = 1;
  *((int8_t *)&task_2C + 2) = 1;
  *(int32_t *)(&task_2C + 8) = 0x41CB3333;
  *(int32_t *)(&task_2C + 12) = 0x42793333;
  *(int32_t *)(&task_2C + 16) = 0x41E4CCCD;
  *(int32_t *)(&task_2C + 20) = 0x426C0000;
  *(int32_t *)(&task_2C + 24) = 0x4211999A;
  *(int32_t *)(&task_2C + 28) = 0x42766666;
  *(int32_t *)(&task_2C + 32) = 0x42053333;
  *(int32_t *)(&task_2C + 36) = 0x42826666;
  *(int32_t *)(&task_2C + 40) = 0x3FFFFF;
  *(int32_t *)(&task_2C + 44) = 0;
  *(int32_t *)(&task_2C + 48) = -0x48000000;
  *(int32_t *)(&task_2C + 52) = 31;
  *(int32_t *)(&task_2C + 60) = 0;
  Script_vfSetupAreaMasks(32, &task_2C);
  *((int8_t *)&task_2C + 1) = 1;
  *((int8_t *)&task_2C + 2) = 1;
  *(int32_t *)(&task_2C + 8) = 0x41A26666;
  *(int32_t *)(&task_2C + 12) = 0x426C0000;
  *(int32_t *)(&task_2C + 16) = 0x41C1999A;
  *(int32_t *)(&task_2C + 20) = 0x425C6666;
  *(int32_t *)(&task_2C + 24) = 0x41E4CCCD;
  *(int32_t *)(&task_2C + 28) = 0x426C0000;
  *(int32_t *)(&task_2C + 32) = 0x41CB3333;
  *(int32_t *)(&task_2C + 36) = 0x42793333;
  *(int32_t *)(&task_2C + 40) = 0x3FFFFF;
  *(int32_t *)(&task_2C + 44) = 0;
  *(int32_t *)(&task_2C + 48) = -0x48000000;
  *(int32_t *)(&task_2C + 52) = 31;
  *(int32_t *)(&task_2C + 60) = 0;
  Script_vfSetupAreaMasks(33, &task_2C);
  *((int8_t *)&task_2C + 1) = 1;
  *((int8_t *)&task_2C + 2) = 1;
  *(int32_t *)(&task_2C + 8) = 0x4211999A;
  *(int32_t *)(&task_2C + 12) = 0x42766666;
  *(int32_t *)(&task_2C + 16) = 0x4223999A;
  *(int32_t *)(&task_2C + 20) = 0x42693333;
  *(int32_t *)(&task_2C + 24) = 0x4231999A;
  *(int32_t *)(&task_2C + 28) = 0x42760000;
  *(int32_t *)(&task_2C + 32) = 0x42213333;
  *(int32_t *)(&task_2C + 36) = 0x42820000;
  *(int32_t *)(&task_2C + 40) = 0x1FFFFFF;
  *(int32_t *)(&task_2C + 44) = 0;
  *(int32_t *)(&task_2C + 48) = -0x48000000;
  *(int32_t *)(&task_2C + 52) = 31;
  *(int32_t *)(&task_2C + 60) = 0;
  Script_vfSetupAreaMasks(34, &task_2C);
  *((int8_t *)&task_2C + 1) = 1;
  *((int8_t *)&task_2C + 2) = 1;
  *(int32_t *)(&task_2C + 8) = 0x41E4CCCD;
  *(int32_t *)(&task_2C + 12) = 0x426C0000;
  *(int32_t *)(&task_2C + 16) = 0x4218CCCD;
  *(int32_t *)(&task_2C + 20) = 0x425E0000;
  *(int32_t *)(&task_2C + 24) = 0x4223999A;
  *(int32_t *)(&task_2C + 28) = 0x42693333;
  *(int32_t *)(&task_2C + 32) = 0x4211999A;
  *(int32_t *)(&task_2C + 36) = 0x42766666;
  *(int32_t *)(&task_2C + 40) = 0x1FFFFFF;
  *(int32_t *)(&task_2C + 44) = 0;
  *(int32_t *)(&task_2C + 48) = -0x48000000;
  *(int32_t *)(&task_2C + 52) = 31;
  *(int32_t *)(&task_2C + 60) = 0;
  Script_vfSetupAreaMasks(35, &task_2C);
  *((int8_t *)&task_2C + 1) = 1;
  *((int8_t *)&task_2C + 2) = 1;
  *(int32_t *)(&task_2C + 8) = 0x41C1999A;
  *(int32_t *)(&task_2C + 12) = 0x425C6666;
  *(int32_t *)(&task_2C + 16) = 0x41E26666;
  *(int32_t *)(&task_2C + 20) = 0x424ECCCD;
  *(int32_t *)(&task_2C + 24) = 0x4218CCCD;
  *(int32_t *)(&task_2C + 28) = 0x425E0000;
  *(int32_t *)(&task_2C + 32) = 0x41E4CCCD;
  *(int32_t *)(&task_2C + 36) = 0x426C0000;
  *(int32_t *)(&task_2C + 40) = 0x1FFFFFF;
  *(int32_t *)(&task_2C + 44) = 0;
  *(int32_t *)(&task_2C + 48) = -0x48000000;
  *(int32_t *)(&task_2C + 52) = 127;
  *(int32_t *)(&task_2C + 60) = 0;
  Script_vfSetupAreaMasks(36, &task_2C);
  *((int8_t *)&task_2C + 1) = 1;
  *((int8_t *)&task_2C + 2) = 1;
  *(int32_t *)(&task_2C + 8) = 0x41A5999A;
  *(int32_t *)(&task_2C + 12) = 0x424D999A;
  *(int32_t *)(&task_2C + 16) = 0x41C40000;
  *(int32_t *)(&task_2C + 20) = 0x423ECCCD;
  *(int32_t *)(&task_2C + 24) = 0x41E26666;
  *(int32_t *)(&task_2C + 28) = 0x424ECCCD;
  *(int32_t *)(&task_2C + 32) = 0x41C1999A;
  *(int32_t *)(&task_2C + 36) = 0x425C6666;
  *(int32_t *)(&task_2C + 40) = 0x7FFFFFF;
  *(int32_t *)(&task_2C + 44) = 0;
  *(int32_t *)(&task_2C + 48) = 0;
  *(int32_t *)(&task_2C + 52) = 48;
  *(int32_t *)(&task_2C + 60) = 0;
  Script_vfSetupAreaMasks(37, &task_2C);
  *((int8_t *)&task_2C + 1) = 1;
  *((int8_t *)&task_2C + 2) = 1;
  *(int32_t *)(&task_2C + 8) = 0x41E26666;
  *(int32_t *)(&task_2C + 12) = 0x424ECCCD;
  *(int32_t *)(&task_2C + 16) = 0x4209999A;
  *(int32_t *)(&task_2C + 20) = 0x4236CCCD;
  *(int32_t *)(&task_2C + 24) = 0x42233333;
  *(int32_t *)(&task_2C + 28) = 0x4252CCCD;
  *(int32_t *)(&task_2C + 32) = 0x4218CCCD;
  *(int32_t *)(&task_2C + 36) = 0x425E0000;
  *(int32_t *)(&task_2C + 40) = 0x7FFFFFF;
  *(int32_t *)(&task_2C + 44) = 0;
  *(int32_t *)(&task_2C + 48) = 0;
  *(int32_t *)(&task_2C + 52) = 336;
  *(int32_t *)(&task_2C + 60) = 0;
  Script_vfSetupAreaMasks(38, &task_2C);
  *((int8_t *)&task_2C + 1) = 1;
  *((int8_t *)&task_2C + 2) = 1;
  *(int32_t *)(&task_2C + 8) = 0x42233333;
  *(int32_t *)(&task_2C + 12) = 0x4252CCCD;
  *(int32_t *)(&task_2C + 16) = 0x422C6666;
  *(int32_t *)(&task_2C + 20) = 0x424B3333;
  *(int32_t *)(&task_2C + 24) = 0x4245999A;
  *(int32_t *)(&task_2C + 28) = 0x42600000;
  *(int32_t *)(&task_2C + 32) = 0x423A6666;
  *(int32_t *)(&task_2C + 36) = 0x426B3333;
  *(int32_t *)(&task_2C + 40) = 0x7FFFFFFF;
  *(int32_t *)(&task_2C + 44) = 0;
  *(int32_t *)(&task_2C + 48) = 0;
  *(int32_t *)(&task_2C + 52) = 1920;
  *(int32_t *)(&task_2C + 60) = 0;
  Script_vfSetupAreaMasks(39, &task_2C);
  *((int8_t *)&task_2C + 1) = 1;
  *((int8_t *)&task_2C + 2) = 1;
  *(int32_t *)(&task_2C + 8) = 0x4209999A;
  *(int32_t *)(&task_2C + 12) = 0x4236CCCD;
  *(int32_t *)(&task_2C + 16) = 0x421A0000;
  *(int32_t *)(&task_2C + 20) = 0x4232CCCD;
  *(int32_t *)(&task_2C + 24) = 0x422C6666;
  *(int32_t *)(&task_2C + 28) = 0x424B3333;
  *(int32_t *)(&task_2C + 32) = 0x42233333;
  *(int32_t *)(&task_2C + 36) = 0x4252CCCD;
  *(int32_t *)(&task_2C + 40) = 0x7FFFFFFF;
  *(int32_t *)(&task_2C + 44) = 0;
  *(int32_t *)(&task_2C + 48) = 0;
  *(int32_t *)(&task_2C + 52) = 1984;
  *(int32_t *)(&task_2C + 60) = 0;
  Script_vfSetupAreaMasks(40, &task_2C);
  *((int8_t *)&task_2C + 1) = 1;
  *((int8_t *)&task_2C + 2) = 1;
  *(int32_t *)(&task_2C + 8) = 0x422C6666;
  *(int32_t *)(&task_2C + 12) = 0x424B3333;
  *(int32_t *)(&task_2C + 16) = 0x42260000;
  *(int32_t *)(&task_2C + 20) = 0x4232CCCD;
  *(int32_t *)(&task_2C + 24) = 0x4253999A;
  *(int32_t *)(&task_2C + 28) = 0x42520000;
  *(int32_t *)(&task_2C + 32) = 0x4245999A;
  *(int32_t *)(&task_2C + 36) = 0x42600000;
  *(int32_t *)(&task_2C + 40) = -1;
  *(int32_t *)(&task_2C + 44) = 3;
  *(int32_t *)(&task_2C + 48) = 0;
  *(int32_t *)(&task_2C + 52) = 6016;
  *(int32_t *)(&task_2C + 60) = 0;
  Script_vfSetupAreaMasks(41, &task_2C);
  *((int8_t *)&task_2C + 1) = 1;
  *((int8_t *)&task_2C + 2) = 1;
  *(int32_t *)(&task_2C + 8) = 0x421A0000;
  *(int32_t *)(&task_2C + 12) = 0x4232CCCD;
  *(int32_t *)(&task_2C + 16) = 0x421F3333;
  *(int32_t *)(&task_2C + 20) = 0x422CCCCD;
  *(int32_t *)(&task_2C + 24) = 0x42260000;
  *(int32_t *)(&task_2C + 28) = 0x4232CCCD;
  *(int32_t *)(&task_2C + 32) = 0x422C6666;
  *(int32_t *)(&task_2C + 36) = 0x424B3333;
  *(int32_t *)(&task_2C + 40) = -1;
  *(int32_t *)(&task_2C + 44) = 3;
  *(int32_t *)(&task_2C + 48) = 0;
  *(int32_t *)(&task_2C + 52) = 3968;
  *(int32_t *)(&task_2C + 60) = 0;
  Script_vfSetupAreaMasks(42, &task_2C);
  *((int8_t *)&task_2C + 1) = 1;
  *((int8_t *)&task_2C + 2) = 1;
  *(int32_t *)(&task_2C + 8) = 0x42126666;
  *(int32_t *)(&task_2C + 12) = 0x422B999A;
  *(int32_t *)(&task_2C + 16) = 0x42186666;
  *(int32_t *)(&task_2C + 20) = 0x4225999A;
  *(int32_t *)(&task_2C + 24) = 0x421F3333;
  *(int32_t *)(&task_2C + 28) = 0x422CCCCD;
  *(int32_t *)(&task_2C + 32) = 0x421A0000;
  *(int32_t *)(&task_2C + 36) = 0x4232CCCD;
  *(int32_t *)(&task_2C + 40) = -1;
  *(int32_t *)(&task_2C + 44) = 3;
  *(int32_t *)(&task_2C + 48) = 0;
  *(int32_t *)(&task_2C + 52) = 0xC00;
  *(int32_t *)(&task_2C + 60) = 0;
  Script_vfSetupAreaMasks(43, &task_2C);
  *((int8_t *)&task_2C + 1) = 1;
  *((int8_t *)&task_2C + 2) = 1;
  *(int32_t *)(&task_2C + 8) = 0x42260000;
  *(int32_t *)(&task_2C + 12) = 0x4232CCCD;
  *(int32_t *)(&task_2C + 16) = 0x423B999A;
  *(int32_t *)(&task_2C + 20) = 0x421E0000;
  *(int32_t *)(&task_2C + 24) = 0x426A0000;
  *(int32_t *)(&task_2C + 28) = 0x4246CCCD;
  *(int32_t *)(&task_2C + 32) = 0x4253999A;
  *(int32_t *)(&task_2C + 36) = 0x42520000;
  *(int32_t *)(&task_2C + 40) = -1;
  *(int32_t *)(&task_2C + 44) = 63;
  *(int32_t *)(&task_2C + 48) = 0;
  *(int32_t *)(&task_2C + 52) = 0x3200;
  *(int32_t *)(&task_2C + 60) = 0;
  Script_vfSetupAreaMasks(44, &task_2C);
  *((int8_t *)&task_2C + 1) = 1;
  *((int8_t *)&task_2C + 2) = 1;
  *(int32_t *)(&task_2C + 8) = 0x423B999A;
  *(int32_t *)(&task_2C + 12) = 0x421E0000;
  *(int32_t *)(&task_2C + 16) = 0x42593333;
  *(int32_t *)(&task_2C + 20) = 0x420F999A;
  *(int32_t *)(&task_2C + 24) = 0x427D999A;
  *(int32_t *)(&task_2C + 28) = 0x4232CCCD;
  *(int32_t *)(&task_2C + 32) = 0x426A0000;
  *(int32_t *)(&task_2C + 36) = 0x4246CCCD;
  *(int32_t *)(&task_2C + 40) = -1;
  *(int32_t *)(&task_2C + 44) = 127;
  *(int32_t *)(&task_2C + 48) = 0;
  *(int32_t *)(&task_2C + 52) = 0x7000;
  *(int32_t *)(&task_2C + 60) = 0;
  Script_vfSetupAreaMasks(45, &task_2C);
  *((int8_t *)&task_2C + 1) = 1;
  *((int8_t *)&task_2C + 2) = 1;
  *(int32_t *)(&task_2C + 8) = 0x422CCCCD;
  *(int32_t *)(&task_2C + 12) = 0x42100000;
  *(int32_t *)(&task_2C + 16) = 0x42353333;
  *(int32_t *)(&task_2C + 20) = 0x42086666;
  *(int32_t *)(&task_2C + 24) = 0x42593333;
  *(int32_t *)(&task_2C + 28) = 0x420F999A;
  *(int32_t *)(&task_2C + 32) = 0x423B999A;
  *(int32_t *)(&task_2C + 36) = 0x421E0000;
  *(int32_t *)(&task_2C + 40) = -1;
  *(int32_t *)(&task_2C + 44) = 511;
  *(int32_t *)(&task_2C + 48) = 0;
  *(int32_t *)(&task_2C + 52) = 0x6000;
  *(int32_t *)(&task_2C + 60) = 0;
  Script_vfSetupAreaMasks(46, &task_2C);
  return;
}

void script_112BB(void) {
  Script_filmCheckOrSetTransitionUtil(0, 255, 0, 0, 0, 0, 0, 0, 0);
  Script_SetMapFlags(0, 1);
  Script_motiLoad(3, SHEN_NAME('MFBT'), "M_FGT1.BIN");
  Script_LoadVoiceFile(1, "E1037");
  Script_LoadVoiceFile(1, "BTT00");
  task_FC = 0;
  task_FD = 0;
  task_FF = 0;
  task_100 = 0;
  task_104 = 0;
  Script_SetShouldSleepFlag(0);
  Script_scenCreateTask(script_11340, 1, CHARA_AKIR);
  return;
}

static int32_t script_11340(int32_t arg0) {
  int32_t local_FC;
  Script_GetSetWeatherSettings(WEATHER_SETTINGS_SET_DISABLE_WEATHER, 1);
  Script_WeatherUtils_0(WeatherDataUtil_ApplyCurrentWeatherOrEnableDateTimeRead, 0);
  Script_WeatherTaskUtil(InitTaskData);
  Script_WeatherTaskUtil(ToggleSceneLightingFlags, 0);
  Script_charaDisplay(arg0, CDM_Show);
  Script_ryoSetChara(arg0);
  task_F8 = Script_GetCurrentEntryPoint();
  if (task_F8 != 0) {
    Script_cdttLoadScrUtils(1, 0);
  }
  local_FC = task_F8;
  if (local_FC == 0) {
    Script_SetActionSelectorHUDElementVisibility(1);
    script_1262A();
    Script_facemotUtils(CHARA_AKIR, 0, 0);
    Script_facemotUtils(CHARA_AKIR, 1, 0);
    script_13389();
    script_8406();
    script_3A22();
    script_85E8();
    Script_ReturnTimeDate(&task_118);
    script_12604();
    script_86C0();
    script_1340F();
    task_F3 = 0;
  } else if (local_FC == 1) {
    Script_ReturnTimeDate(&task_118);
    task_118.hours = 22;
    task_118.minutes = 15;
    Script_SetNewDateTime(&task_118);
    SCNF_WaitFrames_0000(1);
    Script_SetDateAndTime_Wrapper(12, 15, 12, 15, 18000);
    task_F3 = 0;
  } else if (local_FC == 2) {
    script_12604();
    Script_filmCheckOrSetTransitionUtil(0, 255, 0, 0, 0, 0, 0, 0, 0);
    Script_ryoControl(PCCTL_DisablePlayerControl);
    Script_filmCheckOrSetTransitionUtil(32, 255, 0, 0, 0, 0, 0, 0, 0);
    while (Script_filmCheckOrSetTransitionUtil(-1)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_shenClearFreeState(1);
    script_12619();
    task_F3 = 1;
  } else if (local_FC == 3) {
    script_12604();
    Script_ReturnTimeDate(&task_118);
    task_118.hours = 23;
    task_118.minutes = 30;
    Script_SetNewDateTime(&task_118);
    SCNF_WaitFrames_0000(1);
    Script_facemotUtils(CHARA_AKIR, 0, 0);
    Script_facemotUtils(CHARA_AKIR, 1, 0);
    script_13389();
    script_19B1();
    script_0A18();
    task_F3 = 1;
  } else if (local_FC == 4) {
    Script_ReturnTimeDate(&task_118);
    task_118.hours = 23;
    task_118.minutes = 30;
    Script_SetNewDateTime(&task_118);
    SCNF_WaitFrames_0000(1);
    Script_facemotUtils(CHARA_AKIR, 0, 0);
    Script_facemotUtils(CHARA_AKIR, 1, 0);
    script_13389();
    script_0A18();
    task_F3 = 1;
  } else if (local_FC == 98) {
    script_12650();
    script_12665();
    Script_ReturnTimeDate(&task_118);
    task_118.hours = 22;
    task_118.minutes = 15;
    Script_SetNewDateTime(&task_118);
    SCNF_WaitFrames_0000(1);
    Script_SetDateAndTime_Wrapper(12, 15, 12, 15, 18000);
    task_F3 = 0;
  } else if (local_FC == 99) {
    Script_ReturnTimeDate(&task_118);
    task_118.year = 86;
    task_118.month = 10;
    task_118.day = 1;
    task_118.hours = 20;
    task_118.minutes = 30;
    Script_SetNewDateTime(&task_118);
    SCNF_WaitFrames_0000(1);
    Script_SetDateAndTime_Wrapper(21, 0, 21, 0, 18000);
    Script_WeatherTaskUtil(FreeTaskData);
    Script_WeatherTaskUtil(InitTaskData);
    Script_SetActionSelectorHUDElementVisibility(1);
    task_F3 = 0;
    task_FE = 0;
  } else {
    task_F3 = 0;
  }
  switch (task_F3) {
    case 0:
      task_F2 = 1;
      Script_scenCreateTask(script_116D1, 0);
      script_1198E();
      if (task_F8 == 99) {
        Script_SetMapControlTaskFlag(SHEN_NAME('KIS2'), 1);
        Script_uiUtils(1, 14, 0);
      }
      for (;;) {
        if (task_F2 == 1) {
          script_11A53(arg0);
        }
        SCNF_WaitFrames_0000(1);
      }
      break;
    case 1:
      Script_ryoControl(RYO_RestoreControl);
      break;
  }
  end:;
}

static int32_t script_116D1(void) {
  Vector3 local_F4;
  for (;;) {
    if (task_FD == 1) {
      if (Script_GetFightingStats(0x52494B41, 0) == 2) {
        Script_vfToggleBattleState(scnPostResultsSetup);
        for (;;) {
          SCNF_WaitFrames_0000(1);
          if (SCN_GetIsFighting() == 2) {
            break;
          }
        }
      }
      if (SCN_GetIsFighting() == 2) {
        task_104 = 0;
        task_FF = 0;
        task_100 = 0;
        Script_ryoControl(PCCTL_DisablePlayerControl);
        Script_shenClearFreeState(0);
        if (Script_GetFightingStats(0x52494B41, 0) == 3) {
          Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
          while (Script_filmCheckOrSetTransitionUtil(-1)) {
            SCNF_WaitFrames_0000(1);
          }
          Script_GetFightingStats(0, 6);
          if (task_F8 != 99) {
            Script_charaDisplay(SHEN_NAME('KIS2'), CDM_Show);
          }
          if (task_F8 == 0) {
            task_0 = -6;
            *(&task_0 + 1) = 0;
            *(&task_0 + 2) = 0;
            *(&task_0 + 3) = 0;
            *(&task_0 + 4) = 0;
            *(&task_0 + 5) = 14;
            *(&task_0 + 6) = 0;
            *(&task_0 + 7) = 0;
            *(&task_0 + 8) = 0;
            *(&task_0 + 9) = 0;
            *(&task_0 + 10) = 0;
            Script_vfUtils(&task_0);
            script_11A2C();
            Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 972, 1);
            script_13389();
            script_19B1();
            script_0A18();
            break;
          } else {
            script_1194F();
            task_FD = 0;
          }
        } else {
          Script_EVT_GetPosition(CHARA_AKIR, 0, &local_F4, 0);
          Script_RunAuthorAndSetState(0, 2.0f, 0, 0, 0, &local_F4, 0, 0.0f, 0, 0);
          Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
          while (Script_filmCheckOrSetTransitionUtil(-1)) {
            SCNF_WaitFrames_0000(1);
          }
          Script_GetFightingStats(0, 6);
          if (task_F8 != 99) {
            Script_charaDisplay(SHEN_NAME('KIS2'), CDM_Show);
          }
          if (task_F8 == 0) {
            Script_SetNewDateTime(&task_118);
            Script_WeatherTaskUtil(FreeTaskData);
            Script_WeatherTaskUtil(InitTaskData);
            Script_WeatherTaskUtil(ToggleSceneLightingFlags, 0);
            Script_executeAicaCommand(2720, 2, 30);
            script_11A2C();
            Script_WeatherUtils_0(WeatherDataUtil_SetInterpolatedWeatherOverride, 1, 1, 50, 0);
            Script_WeatherUtils_0(WeatherDataUtil_SetInterpolatedWeatherOverride, 2, 1, 50, 0);
            script_13389();
            script_86C0();
            script_1340F();
            script_1198E();
          }
          script_1194F();
          task_FD = 0;
        }
      } else {
        script_12C97();
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  return Script_srchControl(0);
}

static void script_1194F(void) {
  Vector3i local_F4;
  task_F2 = 1;
  Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &data_16D40);
  (&local_F4)->y = *(int32_t *)(data_16D4C + 4);
  Script_EVT_SetRotation(CHARA_AKIR, MATRIX_XFORM_Y, &local_F4, 0);
  return;
}

__attribute__((scn_string_bindings(2, data_16D9F)))
static void script_1198E(void) {
  script_134D1();
  Script_LoadSND_Wrapper(0, 0, "rim008.snd", 0, "e1batbat.snd", "battle_1.snd", 0, 0);
  Script_executeAicaCommand(3240, 0, 0);
  script_12676(1);
  script_12604();
  Script_WeatherTaskUtil(ToggleFogTask, 0);
  Script_WeatherTaskUtil(ToggleCloudXYFlags, 0);
  Script_WeatherUtils_0(WeatherDataUtil_SetInterpolatedWeatherOverride, 1, 0, 400, 0);
  Script_WeatherUtils_0(WeatherDataUtil_SetInterpolatedWeatherOverride, 2, 0, 400, 0);
  script_12619();
  Script_PREP_Related(-1, 1);
  Script_mapmSetCamFlags(0, 1);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 30.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 20.0f);
  return;
}

static void script_11A2C(void) {
  Script_PREP_Related(-1, 0);
  Script_WeatherTaskUtil(ToggleFogTask, 1);
  Script_WeatherTaskUtil(ToggleCloudXYFlags, 1);
  script_12676(0);
  return;
}

static void script_11A53(int32_t arg0) {
  task_F2 = 0;
  SCNF_WaitFrames_0000(1);
  Script_motmGetCharaFlags(SHEN_NAME('NPC1'), 2, 0);
  Script_motmGetCharaFlags(SHEN_NAME('NPC2'), 2, 0);
  Script_motmGetCharaFlags(SHEN_NAME('NPC3'), 2, 0);
  Script_motmGetCharaFlags(SHEN_NAME('NPC4'), 2, 0);
  Script_charaDisplay(SHEN_NAME('NPC3'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('NPC4'), CDM_Show);
  Script_SetMaxNPCsNear(32);
  Script_EVT_SetPosition(arg0, POS_XYZ, &data_16D40);
  {
    Vector3i local_F4;
    (&local_F4)->y = *(int32_t *)(data_16D4C + 4);
    Script_EVT_SetRotation(arg0, MATRIX_XFORM_Y, &local_F4, 0);
  }
  Script_PlayCharaMotion(arg0, MN_AKI_AKI_SEKKIN_KAMAE_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  if (task_F8 != 99) {
    Vector3i local_F4;
    Script_motmGetCharaFlags(SHEN_NAME('KIS2'), 2, 0);
    Script_EVT_SetPosition(SHEN_NAME('KIS2'), POS_XYZ, &data_16D58);
    (&local_F4)->y = *(int32_t *)(data_16D64 + 4);
    Script_EVT_SetRotation(SHEN_NAME('KIS2'), MATRIX_XFORM_Y, &local_F4, 0);
    Script_motmToggleFlags(SHEN_NAME('KIS2'), 1, 0x80);
    Script_PlayCharaMotion(SHEN_NAME('KIS2'), MN_OTH_KIS_KAMAE_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_charaDisplay(SHEN_NAME('KIS2'), CDM_Show);
  }
  task_F0 = 0;
  Script_vfSetup(0, 16, 80, 0x40, 20);
  while (task_F0 < 9) {
    task_F4 = Script_ScenDataUtil(&data_16C44, 4, task_F0 * 7 + 3);
    *(int32_t *)&task_D8.x = Script_ScenDataUtil(&data_16C44, 4, task_F0 * 7);
    *(int32_t *)&task_D8.y = Script_ScenDataUtil(&data_16C44, 4, task_F0 * 7 + 1);
    *(int32_t *)&task_D8.z = Script_ScenDataUtil(&data_16C44, 4, task_F0 * 7 + 2);
    task_10C.x = 0;
    task_10C.y = Script_ScenDataUtil(&data_16C44, 4, task_F0 * 7 + 5);
    task_10C.z = 0;
    Script_EVT_SetPosition(task_F4, POS_XYZ, &task_D8);
    if (Script_ScenDataUtil(&data_16C44, 4, task_F0 * 7 + 4) == 1) {
      Script_EVT_SetFacingTowardPoint(task_F4, 0x38000000, &data_16D40, 0);
    } else {
      Script_EVT_SetRotation(task_F4, 0x38000000, &task_10C, 0);
    }
    Script_PlayCharaMotion(task_F4, Script_ScenDataUtil(&data_16C44, 4, task_F0 * 7 + 6), -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_CharHandlers(task_F4, 2, 0, 16);
    Script_charaToggleMotEnabled(task_F4, 1);
    Script_charaDisplay(task_F4, CDM_Show);
    task_F0 = task_F0 + 1;
  }
  Script_charaSetRenderOpacity(SHEN_NAME('SITM'), 0.5f);
  Script_charaSetRenderOpacity(CHARA_SATO, 0.5f);
  Script_charaSetRenderOpacity(CHARA_PEDR, 0.5f);
  Script_charaDisplay(SHEN_NAME('SITM'), CDM_Hide);
  Script_charaVisUtils(CHARA_SATO, 1);
  Script_SetCPAMFlags(CHARA_SATO, 1);
  Script_SetCPAMFlags(CHARA_PEDR, 1);
  Script_vfStageObjectUtils(0, 0, 2);
  Script_vfStageObjectUtils(2, 0, 2, 1, 0x3E4CCCCD, 0, 0, 0x3EE66666, 0, 0);
  Script_vfStageObjectUtils(2, 0, 2, 1, -0x41B33333, 0, 0, 0x3EE66666, 0, 0);
  Script_vfStageObjectUtils(3, 0, 15, 255, 14, 67241);
  Script_vfStageObjectUtils(4, 0, 853673, 329385, 64, 0, 29127, 20.0f, 0.4f);
  Script_vfStageObjectUtils(5, 0, 18, &data_136B0, &data_136BC);
  Script_vfStageObjectUtils(7, 0, 0x314D5244, 0);
  Script_vfStageObjectUtils(12, 0, 3);
  Script_vfStageObjectUtils(11, 0, 0xB01000);
  Script_vfStageObjectUtils(9, 0, 22.4f, 0.0f, 71.49f);
  Script_vfStageObjectUtils(10, 0, 0, 3640, 0);
  Script_vfStageObjectUtils(0, 1, 1);
  Script_vfStageObjectUtils(1, 1, "K0DY", 16, data_13550, 2, 2, 80, 132777);
  Script_vfStageObjectUtils(2, 1, 2, 1, 0, 0x3ECCCCCD, 0, 0x3ECCCCCD, 0, 0);
  Script_vfStageObjectUtils(3, 1, 30, 255, 14, 198313);
  Script_vfStageObjectUtils(4, 1, 525993, 0, 0, 0, 0, 10.0f, 0.0f);
  Script_vfStageObjectUtils(5, 1, 18, &data_13698, &data_136A4);
  Script_vfStageObjectUtils(20, 1, 8);
  Script_vfStageObjectUtils(7, 1, 0x32584F42, 1);
  Script_charaDisplay(SHEN_NAME('BXD2'), CDM_Hide);
  Script_vfStageObjectUtils(8, 1, 0x32445842, 976);
  Script_vfStageObjectUtils(12, 1, 2);
  Script_vfStageObjectUtils(11, 1, 0xB01000);
  Script_vfStageObjectUtils(9, 1, 35.89f, 0.0f, 49.57f);
  Script_vfStageObjectUtils(10, 1, 0, 0x2000, 0);
  Script_vfStageObjectUtils(0, 2, 1);
  Script_vfStageObjectUtils(1, 2, "K3DY", 48, data_13590, 2, 2, 100, 132777);
  Script_vfStageObjectUtils(2, 2, 2, 2, 0x3F2147AE, 0x3EAE147B, -0x4059999A, 0, 0, -0x40D1EB85);
  Script_vfStageObjectUtils(7, 4, 0x31535842, 2);
  Script_charaDisplay(SHEN_NAME('BSD1'), CDM_Hide);
  Script_vfStageObjectUtils(8, 4, 0x31445342, 720);
  Script_vfStageObjectUtils(9, 4, 27.32f, 0.0f, 78.85f);
  Script_vfStageObjectUtils(10, 4, 0, 0xE000, 0);
  Script_vfStageObjectUtils(7, 5, 0x32535842, 2);
  Script_charaDisplay(SHEN_NAME('BSD2'), CDM_Hide);
  Script_vfStageObjectUtils(8, 5, 0x32445342, 720);
  Script_vfStageObjectUtils(9, 5, 39.29f, 0.0f, 57.5f);
  Script_vfStageObjectUtils(10, 5, 0, 24521, 0);
  Script_vfStageObjectUtils(7, 6, 0x33535842, 2);
  Script_charaDisplay(SHEN_NAME('BSD3'), CDM_Hide);
  Script_vfStageObjectUtils(8, 6, 0x33445342, 720);
  Script_vfStageObjectUtils(9, 6, 40.16f, 0.0f, 59.01f);
  Script_vfStageObjectUtils(10, 6, 0, 17967, 0);
  Script_vfStageObjectUtils(7, 11, 0x38535842, 2);
  Script_charaDisplay(SHEN_NAME('BSD8'), CDM_Hide);
  Script_vfStageObjectUtils(8, 11, 0x38445342, 720);
  Script_vfStageObjectUtils(9, 11, 52.7f, 0.0f, 36.73f);
  Script_vfStageObjectUtils(10, 11, 0, 0xE000, 0);
  Script_vfStageObjectUtils(0, 3, 0);
  Script_vfStageObjectUtils(1, 3, "CNDY", 18, data_13650, 2, 2, 100, 263849);
  Script_vfStageObjectUtils(2, 3, 2, 2, 0x3E3851EC, 0x3F4CCCCD, -0x410A3D71, 0, 0, -0x40333333);
  Script_vfStageObjectUtils(7, 13, 0x4E414349, 3);
  Script_charaDisplay(SHEN_NAME('ICDY'), CDM_Hide);
  Script_vfStageObjectUtils(8, 13, 0x59444349, 720);
  Script_vfStageObjectUtils(9, 13, 56.37f, 0.0f, 39.48f);
  Script_vfStageObjectUtils(10, 13, 0, 12724, 0);
  Script_vfStageObjectUtils(0, 4, 4);
  Script_vfStageObjectUtils(2, 4, 2, 1, 0, 0, 0, 0x3DCCCCCD, 0, 0);
  Script_vfStageObjectUtils(2, 4, 2, 1, -0x41666666, 0, 0, 0x3DCCCCCD, 0, 0);
  Script_vfStageObjectUtils(2, 4, 2, 1, -0x40E66666, 0, 0, 0x3DCCCCCD, 0, 0);
  Script_vfStageObjectUtils(3, 4, 15, 255, 0, 0);
  Script_vfStageObjectUtils(4, 4, 525993, 0, 0x600, -5461, 0, 1.0f, 0.01f);
  Script_vfStageObjectUtils(5, 4, 18, &data_136F8, &data_13704);
  Script_vfStageObjectUtils(7, 14, 0x314B5542, 4);
  Script_vfStageObjectUtils(12, 14, 1);
  Script_vfStageObjectUtils(11, 14, 0x605000);
  Script_vfStageObjectUtils(9, 14, 53.07f, 0.0f, 39.8f);
  Script_vfStageObjectUtils(14, 14, 0x4F544153);
  Script_vfStageObjectUtils(0, 5, 0);
  Script_vfStageObjectUtils(2, 5, 2, 2, 0x3F800000, 0x3F000000, -0x40000000, 0, 0, -0x40800000);
  Script_vfStageObjectUtils(7, 15, 0x314E4546, 5);
  Script_vfStageObjectUtils(9, 15, 9.7f, 0.0f, 69.63f);
  Script_vfStageObjectUtils(10, 15, 0, 0x2000, 0);
  Script_vfStageObjectUtils(7, 16, 0x324E4546, 5);
  Script_vfStageObjectUtils(9, 16, 18.9f, 0.0f, 59.8f);
  Script_vfStageObjectUtils(10, 16, 0, 0x2000, 0);
  Script_vfStageObjectUtils(7, 17, 0x334E4546, 5);
  Script_vfStageObjectUtils(9, 17, 46.0f, 0.0f, 36.8f);
  Script_vfStageObjectUtils(10, 17, 0, 0x2000, 0);
  Script_vfStageObjectUtils(13);
  task_0 = arg0;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 0;
  *(&task_0 + 3) = 0;
  *(&task_0 + 4) = 200;
  *(&task_0 + 5) = 0;
  *(&task_0 + 6) = 0;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(arg0, VF_FORMATION_APPLY_FLAGS, VF_ReactionStateA, 68);
  Script_vfConfigureFormationGroup(arg0, VF_FORMATION_APPLY_FLAGS, VF_ReactionStateA, 85);
  if (task_F8 == 98) {
    script_E723();
  } else if (task_F8 != 99) {
    script_885F();
  } else {
    script_B8B5();
  }
  script_E7EA();
  Script_seqEnterEvtCam(9000, 0, AUTH_None);
  SCNF_WaitFrames_0000(1);
  Script_SetGameStateTo(AUTH_None);
  Script_filmCheckOrSetTransitionUtil(0, 255, 0, 0, 0, 0, 0, 0, 0);
  Script_ryoControl(PCCTL_DisablePlayerControl);
  if (task_F8 == 99) {
    if (task_FE == 0) {
      SCNF_WaitFrames_0000(30);
      task_FE = 1;
    }
    Script_uiUtils(5, 1, 0);
    Script_sicnUtils(0, 64, 0);
  }
  Script_filmCheckOrSetTransitionUtil(32, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  if (task_F8 == 99) {
    Script_vfStageObjectUtils(17, 1);
  }
  if (task_F8 == 99) {
    for (;;) {
      if (Script_GetControllerInput(1) == 2) {
        Script_sicnUtils(2, 64, 0);
        Script_uiUtils(7, 1, 0);
        Script_shenClearFreeState(1);
        Script_wazaUtils(MOVE_UI_CMD_OPEN_MOVES_SCREEN, 0, 0, 0);
        while (Script_GetControllerInput(1) != 0x200) {
          SCNF_WaitFrames_0000(1);
        }
        Script_wazaUtils(MOVE_UI_CMD_REQUEST_CLOSE_MOVES_SCREEN, 0, 0, 0);
        Script_uiUtils(5, 1, 0);
        Script_sicnUtils(0, 64, 0);
        Script_shenClearFreeState(0);
        SCNF_WaitFrames_0000(1);
        SCNF_WaitFrames_0000(1);
      } else {
        if (Script_GetControllerInput(1) == 0x400) {
          Script_sicnUtils(2, 64, 0);
          Script_uiUtils(7, 1, 0);
          break;
          goto L012579;
        }
      }
      L012579:;
      SCNF_WaitFrames_0000(1);
    }
  } else {
    if (task_F8 != 0) {
      while (Script_GetControllerInput(1) != 0x400) {
        Script_PrintDebugString(2, 21, "PRESS A TO START.");
        SCNF_WaitFrames_0000(1);
      }
      goto L0125BB;
    }
  }
  L0125BB:;
  if (task_F8 != 99) {
    SCN_Enqueue_VFGI(4, 2, 1.0f, 1, -1);
  } else {
    Script_uiUtils(7, 1, 0);
    Script_SetActionSelectorHUDElementVisibility(1);
    SCN_Enqueue_VFGI(2, 2, 1.0f, 5, -1);
  }
  task_FD = 1;
  return;
}

static void script_12604(void) {
  Script_mapmSetRenderFlag(1, 1);
  Script_mapmSetRenderFlag(2, 0);
  return;
}

static void script_12619(void) {
  Script_ProcessFogFromTable(0);
  Script_SetupLightFromTable(1);
  return;
}

static void script_1262A(void) {
  Script_mapmSetRenderFlag(1, 0);
  Script_mapmSetRenderFlag(2, 1);
  return;
}

static void script_1263F(void) {
  Script_ProcessFogFromTable(0);
  Script_SetupLightFromTable(2);
  return;
}

static void script_12650(void) {
  Script_mapmSetRenderFlag(1, 0);
  Script_mapmSetRenderFlag(2, 1);
  return;
}

static void script_12665(void) {
  Script_ProcessFogFromTable(0);
  Script_SetupLightFromTable(2);
  return;
}

static void script_12676(int32_t arg0) {
  switch (arg0) {
    case 0:
      Script_charaDisplay(SHEN_NAME('KIS2'), CDM_Hide);
      break;
    case 1:
      Script_charaDisplay(SHEN_NAME('KIS2'), CDM_Show);
      break;
  }
  Script_scenToggleVisibleFor(SHEN_NAME('KIS2'), arg0);
  switch (arg0) {
    case 0:
      Script_charaDisplay(SHEN_NAME('NPC0'), CDM_Hide);
      break;
    case 1:
      Script_charaDisplay(SHEN_NAME('NPC0'), CDM_Show);
      break;
  }
  Script_scenToggleVisibleFor(SHEN_NAME('NPC0'), arg0);
  switch (arg0) {
    case 0:
      Script_charaDisplay(SHEN_NAME('NPC1'), CDM_Hide);
      break;
    case 1:
      Script_charaDisplay(SHEN_NAME('NPC1'), CDM_Show);
      break;
  }
  Script_scenToggleVisibleFor(SHEN_NAME('NPC1'), arg0);
  switch (arg0) {
    case 0:
      Script_charaDisplay(SHEN_NAME('NPC2'), CDM_Hide);
      break;
    case 1:
      Script_charaDisplay(SHEN_NAME('NPC2'), CDM_Show);
      break;
  }
  Script_scenToggleVisibleFor(SHEN_NAME('NPC2'), arg0);
  switch (arg0) {
    case 0:
      Script_charaDisplay(SHEN_NAME('NPC3'), CDM_Hide);
      break;
    case 1:
      Script_charaDisplay(SHEN_NAME('NPC3'), CDM_Show);
      break;
  }
  Script_scenToggleVisibleFor(SHEN_NAME('NPC3'), arg0);
  switch (arg0) {
    case 0:
      Script_charaDisplay(SHEN_NAME('NPC4'), CDM_Hide);
      break;
    case 1:
      Script_charaDisplay(SHEN_NAME('NPC4'), CDM_Show);
      break;
  }
  Script_scenToggleVisibleFor(SHEN_NAME('NPC4'), arg0);
  switch (arg0) {
    case 0:
      Script_charaDisplay(SHEN_NAME('SITM'), CDM_Hide);
      break;
    case 1:
      Script_charaDisplay(SHEN_NAME('SITM'), CDM_Show);
      break;
  }
  Script_scenToggleVisibleFor(SHEN_NAME('SITM'), arg0);
  switch (arg0) {
    case 0:
      Script_charaDisplay(SHEN_NAME('SIT2'), CDM_Hide);
      break;
    case 1:
      Script_charaDisplay(SHEN_NAME('SIT2'), CDM_Show);
      break;
  }
  Script_scenToggleVisibleFor(SHEN_NAME('SIT2'), arg0);
  switch (arg0) {
    case 0:
      Script_charaDisplay(CHARA_SATO, CDM_Hide);
      break;
    case 1:
      Script_charaDisplay(CHARA_SATO, CDM_Show);
      break;
  }
  Script_scenToggleVisibleFor(CHARA_SATO, arg0);
  switch (arg0) {
    case 0:
      Script_charaDisplay(CHARA_PEDR, CDM_Hide);
      break;
    case 1:
      Script_charaDisplay(CHARA_PEDR, CDM_Show);
      break;
  }
  Script_scenToggleVisibleFor(CHARA_PEDR, arg0);
  switch (arg0) {
    case 0:
      Script_charaDisplay(SHEN_NAME('DRM1'), CDM_Hide);
      break;
    case 1:
      Script_charaDisplay(SHEN_NAME('DRM1'), CDM_Show);
      break;
  }
  Script_scenToggleVisibleFor(SHEN_NAME('DRM1'), arg0);
  switch (arg0) {
    case 0:
      Script_charaDisplay(SHEN_NAME('BOX2'), CDM_Hide);
      break;
    case 1:
      Script_charaDisplay(SHEN_NAME('BOX2'), CDM_Show);
      break;
  }
  Script_scenToggleVisibleFor(SHEN_NAME('BOX2'), arg0);
  switch (arg0) {
    case 0:
      Script_charaDisplay(SHEN_NAME('BXD2'), CDM_Hide);
      break;
    case 1:
      Script_charaDisplay(SHEN_NAME('BXD2'), CDM_Show);
      break;
  }
  Script_scenToggleVisibleFor(SHEN_NAME('BXD2'), arg0);
  switch (arg0) {
    case 0:
      Script_charaDisplay(SHEN_NAME('BXS1'), CDM_Hide);
      break;
    case 1:
      Script_charaDisplay(SHEN_NAME('BXS1'), CDM_Show);
      break;
  }
  Script_scenToggleVisibleFor(SHEN_NAME('BXS1'), arg0);
  switch (arg0) {
    case 0:
      Script_charaDisplay(SHEN_NAME('BSD1'), CDM_Hide);
      break;
    case 1:
      Script_charaDisplay(SHEN_NAME('BSD1'), CDM_Show);
      break;
  }
  Script_scenToggleVisibleFor(SHEN_NAME('BSD1'), arg0);
  switch (arg0) {
    case 0:
      Script_charaDisplay(SHEN_NAME('BXS2'), CDM_Hide);
      break;
    case 1:
      Script_charaDisplay(SHEN_NAME('BXS2'), CDM_Show);
      break;
  }
  Script_scenToggleVisibleFor(SHEN_NAME('BXS2'), arg0);
  switch (arg0) {
    case 0:
      Script_charaDisplay(SHEN_NAME('BSD2'), CDM_Hide);
      break;
    case 1:
      Script_charaDisplay(SHEN_NAME('BSD2'), CDM_Show);
      break;
  }
  Script_scenToggleVisibleFor(SHEN_NAME('BSD2'), arg0);
  switch (arg0) {
    case 0:
      Script_charaDisplay(SHEN_NAME('BXS3'), CDM_Hide);
      break;
    case 1:
      Script_charaDisplay(SHEN_NAME('BXS3'), CDM_Show);
      break;
  }
  Script_scenToggleVisibleFor(SHEN_NAME('BXS3'), arg0);
  switch (arg0) {
    case 0:
      Script_charaDisplay(SHEN_NAME('BSD3'), CDM_Hide);
      break;
    case 1:
      Script_charaDisplay(SHEN_NAME('BSD3'), CDM_Show);
      break;
  }
  Script_scenToggleVisibleFor(SHEN_NAME('BSD3'), arg0);
  switch (arg0) {
    case 0:
      Script_charaDisplay(SHEN_NAME('BXS8'), CDM_Hide);
      break;
    case 1:
      Script_charaDisplay(SHEN_NAME('BXS8'), CDM_Show);
      break;
  }
  Script_scenToggleVisibleFor(SHEN_NAME('BXS8'), arg0);
  switch (arg0) {
    case 0:
      Script_charaDisplay(SHEN_NAME('BSD8'), CDM_Hide);
      break;
    case 1:
      Script_charaDisplay(SHEN_NAME('BSD8'), CDM_Show);
      break;
  }
  Script_scenToggleVisibleFor(SHEN_NAME('BSD8'), arg0);
  switch (arg0) {
    case 0:
      Script_charaDisplay(SHEN_NAME('ICAN'), CDM_Hide);
      break;
    case 1:
      Script_charaDisplay(SHEN_NAME('ICAN'), CDM_Show);
      break;
  }
  Script_scenToggleVisibleFor(SHEN_NAME('ICAN'), arg0);
  switch (arg0) {
    case 0:
      Script_charaDisplay(SHEN_NAME('ICDY'), CDM_Hide);
      break;
    case 1:
      Script_charaDisplay(SHEN_NAME('ICDY'), CDM_Show);
      break;
  }
  Script_scenToggleVisibleFor(SHEN_NAME('ICDY'), arg0);
  switch (arg0) {
    case 0:
      Script_charaDisplay(SHEN_NAME('BUK1'), CDM_Hide);
      break;
    case 1:
      Script_charaDisplay(SHEN_NAME('BUK1'), CDM_Show);
      break;
  }
  Script_scenToggleVisibleFor(SHEN_NAME('BUK1'), arg0);
  switch (arg0) {
    case 0:
      Script_charaDisplay(SHEN_NAME('FEN1'), CDM_Hide);
      break;
    case 1:
      Script_charaDisplay(SHEN_NAME('FEN1'), CDM_Show);
      break;
  }
  Script_scenToggleVisibleFor(SHEN_NAME('FEN1'), arg0);
  switch (arg0) {
    case 0:
      Script_charaDisplay(SHEN_NAME('FEN2'), CDM_Hide);
      break;
    case 1:
      Script_charaDisplay(SHEN_NAME('FEN2'), CDM_Show);
      break;
  }
  Script_scenToggleVisibleFor(SHEN_NAME('FEN2'), arg0);
  switch (arg0) {
    case 0:
      Script_charaDisplay(SHEN_NAME('FEN3'), CDM_Hide);
      break;
    case 1:
      Script_charaDisplay(SHEN_NAME('FEN3'), CDM_Show);
      break;
  }
  Script_scenToggleVisibleFor(SHEN_NAME('FEN3'), arg0);
  return;
}

static void script_12C97(void) {
  {
    int32_t local_FC;
    local_FC = task_FF;
    switch (local_FC) {
      case 0:
        if (Script_scenCharaGetMotAnimId(SHEN_NAME('SITM')) == 2524) {
          script_131BD(0x4D544953);
          Script_SetCPAMFlags(SHEN_NAME('SITM'), 0);
          task_FF = 1;
          Script_seqEnterEvtCam(31, 0, AUTH_None);
          SCNF_WaitFrames_0000(10);
          Script_executeAicaCommand(788137, 0, 0);
          SCNF_WaitFrames_0000(18);
          Script_executeAicaCommand(0x2B76AB, 0, 0);
          Script_SetCamShake(0.03f, 0.03f, 0.8f);
        }
        break;
      case 1:
        if (Script_IsInEventCam() == 0) {
          task_FF = 2;
          Script_seqEnterEvtCam(32, 0, AUTH_None);
          SCNF_WaitFrames_0000(6);
          Script_executeAicaCommand(0x2D76AB, 0, 0);
          SCNF_WaitFrames_0000(20);
          Script_executeAicaCommand(0x2D76AB, 0, 0);
        }
        break;
      case 2:
        if (Script_IsInEventCam() == 0) {
          task_FF = 3;
          Script_seqEnterEvtCam(13, CHARA_AKIR, CHARA_AKIR);
          SCNF_WaitFrames_0000(2);
          if (task_F8 == 99) {
            Script_SetSearchFov(5);
          } else {
            Script_SetSearchFov(1);
          }
          Script_SetCPAMFlags(SHEN_NAME('SITM'), 1);
          script_13272(0x4D544953);
        }
        break;
    }
  }
  {
    int32_t local_FC;
    local_FC = task_100;
    switch (local_FC) {
      case 0:
        if (Script_scenCharaGetMotAnimId(CHARA_SATO) == 2291) {
          Script_charaVisUtils(CHARA_SATO, 0);
          script_131BD(0x4F544153);
          Script_SetCPAMFlags(CHARA_SATO, 0);
          task_100 = 1;
          Script_SetCharaRefBoardPos(CHARA_SATO, &data_16DF4);
          Script_charaSetRefboardFlagTo(CHARA_SATO, 1);
          Script_seqEnterEvtCam(20, 0, AUTH_None);
          Script_executeAicaCommand(0x1306A9, 0, 0);
          SCNF_WaitFrames_0000(20);
          Script_executeAicaCommand(0x2B76AB, 0, 0);
          Script_SetCamShake(0.03f, 0.03f, 0.8f);
        }
        break;
      case 1:
        if (Script_IsInEventCam() == 0) {
          task_100 = 2;
          Script_seqEnterEvtCam(21, 0, AUTH_None);
          SCNF_WaitFrames_0000(24);
          Script_executeAicaCommand(0x1C76AB, 0, 0);
          SCNF_WaitFrames_0000(10);
          Script_SetCamShake(0.04f, 0.04f, 0.8f);
        }
        break;
      case 2:
        if (Script_IsInEventCam() == 0) {
          task_100 = 3;
          Script_SetCharaRefBoardPos(CHARA_SATO, &data_16E04);
          Script_charaSetRefboardFlagTo(CHARA_SATO, 0);
          Script_seqEnterEvtCam(13, CHARA_AKIR, CHARA_AKIR);
          SCNF_WaitFrames_0000(2);
          if (task_F8 == 99) {
            Script_SetSearchFov(5);
          } else {
            Script_SetSearchFov(1);
          }
          Script_SetCPAMFlags(CHARA_SATO, 1);
          script_13272(0x4F544153);
        }
        break;
    }
  }
  {
    int32_t local_FC;
    local_FC = task_104;
    switch (local_FC) {
      case 0:
        if (Script_scenCharaGetMotAnimId(CHARA_PEDR) == 2455) {
          script_131BD(0x52444550);
          Script_SetCPAMFlags(CHARA_PEDR, 0);
          task_104 = 1;
          Script_seqEnterEvtCam(10, 0, AUTH_None);
          SCNF_WaitFrames_0000(15);
          Script_executeAicaCommand(0x1F76AB, 0, 0);
        }
        break;
      case 1:
        if (Script_IsInEventCam() == 0) {
          task_104 = 2;
          Script_seqEnterEvtCam(11, 0, AUTH_None);
          SCNF_WaitFrames_0000(5);
          Script_executeAicaCommand(0x1676AB, 0, 0);
          task_108 = Script_EffectUtils(0, &data_16DDC, &data_16DD0, 80, 0.012f, 0, 3);
          Script_EffectUtils(9, task_108, 180, 180, 180);
          Script_EffectUtils(14, task_108, 0.01f, 0.4f);
          Script_EffectUtils(12, task_108, 8);
          Script_EffectUtils(1, task_108);
          Script_SetCamShake(0.0f, 0.1f, 0.8f);
        }
        break;
      case 2:
        if (Script_IsInEventCam() == 0) {
          task_104 = 3;
          Script_EVT_SetPosition(CHARA_PEDR, POS_XYZ, &data_16DE8);
          Script_seqEnterEvtCam(12, 0, AUTH_None);
          SCNF_WaitFrames_0000(50);
          Script_ShowUIMessageFromID("E1037C001");
          SCNF_WaitFrames_0000(20);
          Script_filmCheckOrSetTransitionUtil(25, 0, 255, 255, 255, 255, 255, 255, 255);
          while (Script_filmCheckOrSetTransitionUtil(-1)) {
            SCNF_WaitFrames_0000(1);
          }
        }
        break;
      case 3:
        if (Script_IsInEventCam() == 0) {
          task_104 = 4;
          Script_seqEnterEvtCam(13, CHARA_AKIR, CHARA_AKIR);
          SCNF_WaitFrames_0000(4);
          if (task_F8 == 99) {
            Script_SetSearchFov(5);
          } else {
            Script_SetSearchFov(1);
          }
          Script_EffectUtils(2, task_108);
          Script_EffectUtils(3, task_108);
          Script_SetCPAMFlags(CHARA_PEDR, 1);
          script_13272(0x52444550);
          if (task_F8 != 99) {
            Script_charaDisplay(SHEN_NAME('KIS2'), CDM_Hide);
          }
          Script_filmCheckOrSetTransitionUtil(4, 255, 255, 255, 255, 0, 255, 255, 255);
          while (Script_filmCheckOrSetTransitionUtil(-1)) {
            SCNF_WaitFrames_0000(1);
          }
        }
        break;
    }
  }
  return;
}

static void script_131BD(int32_t arg0) {
  Script_charaDisplay(CHARA_AKIR, CDM_Hide);
  Script_charaDisplay(SHEN_NAME('NPC0'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('NPC1'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('NPC2'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('NPC3'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('NPC4'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('SIT2'), CDM_Hide);
  if (arg0 != 0x4D544953) {
    Script_charaDisplay(SHEN_NAME('SITM'), CDM_Hide);
  }
  if (arg0 != 0x4F544153) {
    Script_charaDisplay(CHARA_SATO, CDM_Hide);
    Script_charaDisplay(SHEN_NAME('BUK1'), CDM_Hide);
  }
  if (arg0 != 0x52444550) {
    Script_charaDisplay(CHARA_PEDR, CDM_Hide);
  }
  if (Script_GetCurrentEntryPoint() != 99) {
    Script_charaDisplay(SHEN_NAME('KIS2'), CDM_Hide);
  }
  return;
}

static void script_13272(int32_t arg0) {
  Script_charaDisplay(CHARA_AKIR, CDM_Show);
  Script_charaDisplay(SHEN_NAME('NPC0'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('NPC1'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('NPC2'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('NPC3'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('NPC4'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('SIT2'), CDM_Show);
  if (arg0 != 0x4D544953) {
    Script_charaDisplay(SHEN_NAME('SITM'), CDM_Show);
  }
  if (arg0 != 0x4F544153) {
    Script_charaDisplay(CHARA_SATO, CDM_Show);
    Script_charaDisplay(SHEN_NAME('BUK1'), CDM_Show);
  }
  if (arg0 != 0x52444550) {
    Script_charaDisplay(CHARA_PEDR, CDM_Show);
  }
  if (Script_GetCurrentEntryPoint() != 99) {
    Script_charaDisplay(SHEN_NAME('KIS2'), CDM_Show);
  }
  return;
}

static void script_13327(const char *arg0, const char *arg1) {
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, arg0, arg1) == 0) {
    Script_sprintf("MFBT: Load PackFile %s:%s\012", arg0, arg1);
    SCNF_WaitFrames_0000(1);
  }
  Script_sprintf("MFBT: Now Loading PackFile.");
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2)) {
    SCNF_WaitFrames_0000(1);
    Script_sprintf(".");
  }
  Script_sprintf("\012");
  return;
}

__attribute__((scn_string_bindings(0, data_16E59, 1, data_16E68, 2, data_16E6E)))
static void script_13389(void) {
  script_13327("/scene/03/MFBT", "EVHUM");
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 2, "EVHUM");
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
  Script_facemotUtils(CHARA_TERY, 1, 0);
  Script_facemotUtils(CHARA_KISY, 1, 0);
  Script_facemotUtils(CHARA_STSI, 1, 0);
  Script_facemotUtils(CHARA_TERY, 0, 0);
  Script_facemotUtils(CHARA_KISY, 0, 0);
  Script_facemotUtils(CHARA_STSI, 0, 0);
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
  return;
}

__attribute__((scn_string_bindings(0, data_16E74)))
static void script_1340F(void) {
  Script_FaceEx(CHARA_STSI, 0, 0, 2);
  Script_FaceEx(CHARA_KISY, 0, 0, 2);
  Script_FaceEx(CHARA_TERY, 0, 0, 2);
  Script_ResetHandMotion(CHARA_STSI, 0, 0, 2);
  Script_ResetHandMotion(CHARA_KISY, 0, 0, 2);
  Script_ResetHandMotion(CHARA_TERY, 0, 0, 2);
  Script_faceUpdateFor(CHARA_STSI, 0, 0);
  Script_faceUpdateFor(CHARA_KISY, 0, 0);
  Script_faceUpdateFor(CHARA_TERY, 0, 0);
  Script_faceUpdateFor(CHARA_STSI, 1, 0);
  Script_faceUpdateFor(CHARA_KISY, 1, 0);
  Script_faceUpdateFor(CHARA_TERY, 1, 0);
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "EVHUM");
  return;
}

__attribute__((scn_string_bindings(0, data_16E7A, 1, data_16E7F, 2, data_16E84, 3, data_16E89, 4, data_16E8E)))
static void script_134D1(void) {
  Script_LoadSND(0, 6, "FREE", 0);
  Script_LoadSND(0, 5, "FREE", 0);
  Script_LoadSND(0, 4, "FREE", 0);
  Script_LoadSND(0, 3, "FREE", 0);
  Script_LoadSND(0, 2, "FREE", 0);
  return;
}
