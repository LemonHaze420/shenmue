#pragma once

#include "scenario.h"
#include "motion/motion.h"
#include "scene/01/yq14/m_yq14.h"

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
static void script_1403(void);
static int32_t script_1468(void);
static void script_14A3(void);
static void script_14ED(void);
static int32_t script_153A(void);
static void script_1587(void);
static int32_t script_15CD(void);
static void script_1608(void);
static void script_1652(void);
static int32_t script_1699(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_1B5A(int32_t arg0, int32_t arg1, int32_t arg2);
static int32_t script_1C4D(int32_t arg0, int32_t arg1, int32_t arg2);
static int32_t script_1D40(int32_t arg0, int32_t arg1, int32_t arg2);
static int32_t script_1E1B(int32_t arg0, int32_t arg1);
static void script_1EE7(int32_t arg0, int32_t arg1);
static int32_t script_1FB2(int32_t arg0, int32_t arg1, int32_t arg2);
static int32_t script_208D(int32_t arg0, int32_t arg1, float arg2, int32_t arg3, const void *arg4);
static void script_20FA(int32_t arg0, float arg1, const void *arg2);
static void script_217C(int32_t arg0, float arg1);
static void script_21FC(int32_t arg0);
static int32_t script_2249(void);
static void script_22F2(int32_t arg0, int32_t arg1);
static void script_2432(int32_t arg0, int32_t arg1);
static void script_25B5(int32_t arg0, int32_t arg1);
static int32_t script_279E(void);
static void script_283C(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_2852(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_2876(int32_t arg0, int32_t arg1);
static int32_t script_2888(int32_t arg0, int32_t arg1);
static void script_2A53(int32_t arg0, int32_t arg1);
static void script_2A64(int32_t arg0, int32_t arg1);
static int32_t script_2B83(int32_t arg0, int32_t arg1);
static int32_t script_2E5B(int32_t arg0, int32_t arg1);
void script_3342(void);
static void script_3392(void);
static void script_33F8(int32_t arg0);
static void script_35DF(void);
static int32_t script_3632(int32_t arg0, int32_t arg1);
static int32_t script_3705(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_37CA(int32_t arg0, int32_t arg1);
static int32_t script_3822(int32_t arg0, int32_t arg1, int32_t arg2);
static int32_t script_38CA(int32_t arg0, int32_t arg1);

static int32_t task_0;
static int32_t task_4;
static int32_t task_8;
static int32_t task_C;
static int32_t task_10;
static int32_t task_14;
static int32_t task_18[3];
static int32_t task_24[3];
static int32_t task_30[3];
static int32_t task_3C[3];
static int32_t task_48[3];
static int32_t task_54;
static int32_t task_58[3];
static int32_t task_64[3];
static int32_t task_70[3];
static int32_t task_7C[3];
static int32_t task_88;
static int32_t task_8C;
static int32_t task_90;
static int32_t task_94;
static int32_t task_98;
static int32_t task_9C;
static int32_t task_A0[3];
static float task_AC[3][3];
static float task_D0[3];
static float task_DC;
static float task_E0;
static float task_E4;
static float task_E8[3][3];
static int32_t task_10C[3][3];
static float task_130[3];
static int32_t task_13C[3];
static int32_t task_148;
static int32_t task_14C;
static int32_t task_150[3];
static int32_t task_15C[3];
static int32_t task_168[3];
static int32_t task_174[3];
static float task_180[3];
static int32_t task_18C;
static const int32_t data_391C[741] = {
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
static const char data_44B0[16] __attribute__((scn_string_literal)) = "シーン再生14_1\012";
static const char data_44C0[6] __attribute__((scn_string_literal)) = "test\012";
static const char data_44C6[16] __attribute__((scn_string_literal)) = "シーン再生14_2\012";
static const char data_44D6[6] __attribute__((scn_string_literal)) = "test\012";
static const uint32_t data_44DC = 256;
static const int32_t data_44E0[47] = {
  20, 20, 4, 2, -1, -1, -1, 1, -1, 3, 5, 512, 20, 20, 4, 3, -1, 19, -1, -1, -1, 7, 9, 512, 20, 20, 4, 5, -1, 15, -1, -1, -1, 13, 11, 1024, 20, 20, 4,
  0, -1, -1, -1, -1, -1, 15, 17
};
static const uint8_t data_459C[4] = {0};
static const int32_t data_45A0[7] = {6, 3, 5, 2, 1, 7, 4};
static const uint8_t data_45BC[4] = {0};
static const int32_t data_45C0[11] = {11, 6, 3, 10, 5, 2, 1, 8, 7, 9, 4};
static const uint8_t data_45EC[4] = {0};
static const int32_t data_45F0[36] = {
  1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36
};
static const uint32_t data_4680 = 0x3F800000;
static const float data_4684[69] = {
  0.95f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, -0.06f, -0.75f, -0.63f, 0.0f, 1.0f, 0.55f, 0.57f, 0.6f, -115.9f, 1.55f, 69.2f, 7.5f, 0.88f,
  1.0f, 0.0f, 1.0f, 0.0f, 72.3f, 1.0f, 0.59f, 0.36f, 0.27f, -120.6f, 3.2f, 77.6f, 5.6f, 0.88f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.21f, 0.11f,
  0.23f, -124.2f, 1.55f, 65.5f, 7.5f, 0.71f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.02f, 0.5f, 0.68f, -124.2f, 1.03f, 65.5f, 2.8f, 0.86f, 1.0f, 0.0f,
  0.0f, 0.0f, 0.0f
};
static const char data_4798[] __attribute__((scn_string_literal)) = "YDAD";
static const char data_479D[] __attribute__((scn_string_literal)) = "YDBD";
static const char data_47A2[] __attribute__((scn_string_literal)) = "BC0D";
static const char data_47A7[] __attribute__((scn_string_literal)) = "BC1D";
static const char data_47AC[] __attribute__((scn_string_literal)) = "BC2D";
static const char data_47B1[] __attribute__((scn_string_literal)) = "KADY";
static const uint8_t data_47B6[16] = {
  0x00, 0x00, 0xAE, 0x47, 0x61, 0x3F, 0xAE, 0x47, 0x61, 0x3F, 0xAE, 0x47, 0x61, 0x3F, 0xAE, 0x47
};
static const uint8_t data_47C6[16] = {
  0x61, 0x3F, 0xAE, 0x47, 0xE1, 0x3D, 0x9A, 0x99, 0x19, 0x3E, 0x0A, 0xD7, 0x23, 0x3E, 0xAE, 0x47
};
static const uint8_t data_47D6[2] = {
  0x61, 0x3E
};
static const Vector4 data_47D8 = {.x = 0.0f, .y = 0.04f, .z = 0.1f, .w = 0.26f};
static const float data_47E8[3] = {2.0f, 95.0f, 0.4f};
static const int32_t data_47F4 = -1441793;
static const FogColourData data_47F8 = {.red = 2.0f, .green = 126.0f, .blue = 0.4f, .colour = 0xFF081125U};
static const int32_t data_4808[2] = {16777215, 16777215};
static const uint32_t data_4810[2] = {0xEDFFFFFFU, 0xEDFFFFFFU};
static const uint32_t data_4818[4] = {0xFF060A16U, 0xFF060A16U, 0xFF2C324EU, 0xFF2C324EU};
static const char data_4828[] __attribute__((scn_string_literal)) = "scroll";
static const char data_482F[13] __attribute__((scn_string_literal)) = "SCROLL99.SPR";
static const char data_483C[] __attribute__((scn_string_literal)) = "scroll";
static const char data_4843[13] __attribute__((scn_string_literal)) = "SCROLL00.SPR";
static const uint32_t data_4850 = 0xC2ED0000;
static const float data_4854[3] = {-1.8f, 69.0f, 1.0f};
static const int32_t data_4860 = 1;
static const float data_4864[4] = {-123.2f, -1.8f, 65.5f, 1.0f};
static const int32_t data_4874[2] = {2, -11062528};
static const char data_487C[10] __attribute__((scn_string_literal)) = "E1002B001";
static const char data_4886[10] __attribute__((scn_string_literal)) = "E1002B002";
static const char data_4890[10] __attribute__((scn_string_literal)) = "E1002B003";
static const char data_489A[10] __attribute__((scn_string_literal)) = "E1002B004";
static const char data_48A4[10] __attribute__((scn_string_literal)) = "E1002B005";
static const char data_48AE[10] __attribute__((scn_string_literal)) = "E1002B006";
static const char data_48B8[10] __attribute__((scn_string_literal)) = "E1002B007";
static const char data_48C2[10] __attribute__((scn_string_literal)) = "E1002B008";
static const char data_48CC[10] __attribute__((scn_string_literal)) = "E1002B009";
static const char data_48D6[10] __attribute__((scn_string_literal)) = "E1002B010";
static const char data_48E0[10] __attribute__((scn_string_literal)) = "E1002C001";
static const char data_48EA[10] __attribute__((scn_string_literal)) = "E1002C002";
static const char data_48F4[10] __attribute__((scn_string_literal)) = "E1002C003";
static const char data_48FE[10] __attribute__((scn_string_literal)) = "E1002C004";
static const char data_4908[10] __attribute__((scn_string_literal)) = "E1002C005";
static const char data_4912[10] __attribute__((scn_string_literal)) = "E1002C006";
static const char data_491C[10] __attribute__((scn_string_literal)) = "E1002C007";
static const char data_4926[10] __attribute__((scn_string_literal)) = "E1002C008";
static const char data_4930[10] __attribute__((scn_string_literal)) = "E1002C009";
static const char data_493A[10] __attribute__((scn_string_literal)) = "E1002D001";
static const char data_4944[7] __attribute__((scn_string_literal)) = "開始０";
static const char data_494B[7] __attribute__((scn_string_literal)) = "成功０";
static const char data_4952[7] __attribute__((scn_string_literal)) = "失敗０";
static const char data_4959[7] __attribute__((scn_string_literal)) = "開始１";
static const char data_4960[7] __attribute__((scn_string_literal)) = "成功１";
static const char data_4967[7] __attribute__((scn_string_literal)) = "失敗１";
static const char data_496E[7] __attribute__((scn_string_literal)) = "開始２";
static const char data_4975[7] __attribute__((scn_string_literal)) = "成功２";
static const char data_497C[7] __attribute__((scn_string_literal)) = "失敗２";
static const char data_4983[7] __attribute__((scn_string_literal)) = "開始３";
static const char data_498A[7] __attribute__((scn_string_literal)) = "成功３";
static const char data_4991[7] __attribute__((scn_string_literal)) = "失敗３";
static const char data_4998[7] __attribute__((scn_string_literal)) = "開始４";
static const char data_499F[7] __attribute__((scn_string_literal)) = "成功４";
static const char data_49A6[7] __attribute__((scn_string_literal)) = "失敗４";
static const char data_49AD[15] __attribute__((scn_string_literal)) = "PRESS A BUTTON";
static const char data_49BC[12] __attribute__((scn_string_literal)) = "n1014_4.snd";
static const char data_49C8[12] __attribute__((scn_string_literal)) = "n1sqt_4.snd";
static const char data_49D4[12] __attribute__((scn_string_literal)) = "system1.snd";
static const char data_49E0[12] __attribute__((scn_string_literal)) = "s2025_4.snd";
static const char data_49EC[11] __attribute__((scn_string_literal)) = "qtebat.snd";
static const char data_49F7[11] __attribute__((scn_string_literal)) = "bgm041.snd";
static const char data_4A02[6] __attribute__((scn_string_literal)) = "E1002";
static const char data_4A08[7] __attribute__((scn_string_literal)) = "A01114";
static const char data_4A0F[11] __attribute__((scn_string_literal)) = "M_YQ14.BIN";
static const char data_4A1A[12] __attribute__((scn_string_literal)) = "M_01114.BIN";
static const char data_4A26[7] __attribute__((scn_string_literal)) = "A01114";
static const char data_4A2D[7] __attribute__((scn_string_literal)) = "E1002";
