#include "act01_yq14.h"

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

static int32_t script_039D(int32_t arg0, int32_t arg1) {
  if (arg1 > 0) {
    SCNF_WaitFrames_0000(arg1);
  }
  Script_executeAicaCommand(arg0, 0, 0);
  return Script_srchControl(0);
}

static int32_t script_03C1(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  task_18[arg1] = 0;
  task_24[arg1] = 0;
  task_30[arg1] = 0;
  task_3C[arg1] = arg2;
  task_48[arg1] = arg3;
  task_54 = 0;
  Script_CharHandlers(arg0, 0, task_3C[arg1], 16);
  Script_CharHandlers(arg0, 1, task_48[arg1], 16);
  for (;;) {
    task_18[arg1] = 0;
    while (task_18[arg1] == 0) {
      SCNF_WaitFrames_0000(1);
    }
    task_54 = task_30[arg1] | 0x40000000;
    Script_scenCreateTask(arg4, 3, arg0, arg1, task_18[arg1] + task_24[arg1]);
  }
  end:;
}

static int32_t script_0498(int32_t arg0, int32_t arg1, int32_t arg2, float arg3, int32_t arg4, int32_t arg5) {
  int32_t local_FC;
  local_FC = task_54;
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
  Script_CharHandlers(arg0, 0, task_3C[arg1], 16);
  Script_CharHandlers(arg0, 1, task_48[arg1], 16);
  return Script_srchControl(0);
}

static int32_t script_0591(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  local_FC = task_54;
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
  local_FC = task_54;
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
  task_7C[arg1] = Script_scenCreateTask(script_03C1, 5, arg0, arg1, arg2, arg3, arg5);
  task_58[arg1] = arg0;
  task_64[arg1] = 0;
  task_70[arg1] = 0;
  task_88 = 4;
  for (;;) {
    SCNF_WaitFrames_0000(1);
    if (task_64[arg1]) {
      local_FC = task_70[arg1] * 12;
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
      if (task_24[arg1] == 0) {
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
        if (task_94 == Script_ScenDataUtil(arg4, 4, local_FC)) {
          task_90 = 1;
          local_F8 = Script_ScenDataUtil(arg4, 4, local_FC + 10);
        } else {
          if (task_24[arg1] == 0) {
            task_9C = task_9C + Script_ScenDataUtil(arg4, 4, local_FC + 4);
          }
          task_90 = 0;
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
  task_64[arg0] = 1;
  task_70[arg0] = arg1;
  task_24[arg0] = arg2;
  task_30[arg0] = arg3;
  return;
}

static void script_08C4(int32_t arg0) {
  task_64[arg0] = 0;
  return;
}

static void script_08D4(void) {
  SCNF_WaitFrames_0000(2);
  while (task_94 == 0) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_08F4(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  script_088D(arg0, arg1, 0, arg3);
  script_088D(arg2, arg1, 1, arg3);
  while (task_64[arg0]) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_092E(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  script_088D(arg0, arg1, 0, arg3);
  script_088D(arg2, arg1, 1, arg3);
  script_088D(arg2 + 1, arg1, 2, arg3);
  while (task_64[arg0]) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_097A(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  script_088D(arg0, arg1, 0, arg4);
  script_088D(arg2, arg1, 1, arg4);
  script_088D(arg3, arg1, 2, arg4);
  while (task_64[arg0]) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_09C3(int32_t arg0) {
  task_88 = arg0;
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
  task_98 = arg0;
  task_94 = 0;
  task_8C = 0;
  local_F8 = 0xFFFF;
  for (;;) {
    local_FC = SCNF_UIUtils(0, 0, 0, 0, 0, -1);
    task_8C = task_8C + 1;
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
      if (task_8C >= arg0 + arg1 + 4) {
        break;
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  if ((local_F8 & 0xFFF) == 0) {
    local_F8 = 0xFFFF;
  }
  task_94 = local_F8 & 0xFFF;
  return Script_srchControl(0);
}

static int32_t script_0AFA(int32_t arg0, int32_t arg1, int32_t arg2) {
  {
    Vector3 local_F4;
    Script_EVT_GetPosition(arg0, -1, &local_F4, 0);
    task_A0[arg1] = arg0;
    task_AC[arg1][0] = (&local_F4)->x;
    task_AC[arg1][1] = (&local_F4)->y;
    task_AC[arg1][2] = (&local_F4)->z;
  }
  task_D0[arg1] = 0.0f;
  for (;;) {
    Vector3 local_F4;
    SCNF_WaitFrames_0000(1);
    Script_EVT_GetPosition(arg0, -1, &local_F4, 0);
    task_AC[arg1][0] = (&local_F4)->x;
    task_AC[arg1][1] = (&local_F4)->y;
    task_AC[arg1][2] = (&local_F4)->z;
    task_D0[arg1] = Script_MathUtils(SCRIPT_MATH_VEC2_LENGTH, task_AC[arg1][0] - task_AC[arg2][0], task_AC[arg1][2] - task_AC[arg2][2]);
  }
  end:;
}

static void script_0BE8(void) {
  task_DC = 0.0f;
  task_E0 = 0.0f;
  task_E4 = 0.0f;
  for (;;) {
    SCNF_WaitFrames_0000(1);
    if (Script_GetInputData(2, 0) & 4) {
      if (Script_GetInputData(0, 2) & 0x4000) {
        task_DC = task_DC - 0.01f;
      }
      if (Script_GetInputData(0, 2) & 0x8000) {
        task_DC = task_DC + 0.01f;
      }
      if (Script_GetInputData(0, 2) & 0x1000) {
        task_E0 = task_E0 - 0.01f;
      }
      if (Script_GetInputData(0, 2) & 0x2000) {
        task_E0 = task_E0 + 0.01f;
      }
      if (Script_GetInputData(0, 2) & 0x40) {
        task_E4 = task_E4 - 1.0f;
      }
      if (Script_GetInputData(0, 2) & 0x80) {
        task_E4 = task_E4 + 1.0f;
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
    Script_EVT_GetPosition(task_58[local_E4], -1, &local_F4, 0);
    Script_mtwkGetSetRotation(task_58[local_E4], &local_E8, 0);
    task_E8[local_E4][0] = (&local_F4)->x;
    task_E8[local_E4][1] = (&local_F4)->y;
    task_E8[local_E4][2] = (&local_F4)->z;
    task_10C[local_E4][0] = (&local_E8)->x;
    task_10C[local_E4][1] = (&local_E8)->y;
    task_10C[local_E4][2] = (&local_E8)->z;
    task_130[local_E4] = Script_GetCharaMotionRemainingFramesScaled(task_58[local_E4]);
    task_13C[local_E4] = Script_scenCharaGetMotAnimId(task_58[local_E4]);
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
    local_F4.x = task_E8[local_E4][0];
    (&local_F4)->y = task_E8[local_E4][1];
    (&local_F4)->z = task_E8[local_E4][2];
    local_E8.x = task_10C[local_E4][0];
    (&local_E8)->y = task_10C[local_E4][1];
    (&local_E8)->z = task_10C[local_E4][2];
    Script_EVT_SetPosition(task_58[local_E4], POS_XYZ, &local_F4);
    Script_EVT_SetRotation(task_58[local_E4], 0x38000000, &local_E8, 0);
    Script_PlayCharaMotion(task_58[local_E4], task_13C[local_E4], task_130[local_E4], -1.0f, -1.0f, 1.0f, 0);
    local_E4 = local_E4 + 1;
  }
  return;
}

static void script_1249(int32_t arg0) {
  task_14C = arg0;
  task_148 = 1;
  Script_PlayerStatsUtils(PS_Set70MBTimeCapSlot, task_14C, task_148);
  return;
}

static void script_1267(void) {
  int32_t local_FC;
  if (task_148 == 1) {
    local_FC = Script_PlayerStatsUtils(PS_Get70MBCounter1);
    Script_PlayerStatsUtils(PS_Set70MBCounter1, local_FC + 1);
  }
  if (task_148 < 255) {
    task_148 = task_148 + 1;
  }
  Script_PlayerStatsUtils(PS_Set70MBTimeCapSlot, task_14C, task_148);
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

static void script_1403(void) {
  int32_t local_FC;
  int32_t unused_F8;
  script_14A3();
  Script_sprintf("シーン再生14_1\012");
  Script_filmCheckOrSetTransitionUtil(60, 255, 0, 0, 0, 0, 0, 0, 0);
  local_FC = Script_scenCreateTask(script_1468, 0);
  Script_shenSeqUtils(0);
  while (Script_shenSeqUtils(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_srchControl((void *)local_FC);
  Script_set_val_to_1(2, 0);
  script_14ED();
  return;
}

__attribute__((scn_string_bindings(0, data_44C0)))
static int32_t script_1468(void) {
  int16_t local_FE;
  uint8_t unused_F0[14];
  local_FE = 0;
  for (;;) {
    if (Script_shenSeqUtils(-1)) {
      local_FE = local_FE + 1;
      SCNF_WaitFrames_0000(1);
    } else {
      Script_sprintf("test\012");
      local_FE = 0;
      break;
    }
  }
  end:;
}

static void script_14A3(void) {
  int32_t unused_F0[4];
  script_0048(CHARA_AKIR);
  script_0048(CHARA_TONY);
  script_0048(CHARA_SMTH);
  Script_facemotUtils(CHARA_AKIR, 1, 0);
  Script_charaDisplay(SHEN_NAME('BIN_'), CDM_Hide);
  Script_SetShadowFlags(0, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  return;
}

static void script_14ED(void) {
  script_00D5(CHARA_AKIR);
  script_00D5(CHARA_TONY);
  script_00D5(CHARA_SMTH);
  Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
  Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
  Script_SetGameStateTo(AUTH_None);
  return;
}

static int32_t script_153A(void) {
  float local_FC;
  for (;;) {
    local_FC = Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 1.2f);
    if (!(local_FC >= 0.7f)) {
      Script_lightSetSlotEnabled(1, 1);
      Script_lightSetSlotEnabled(5, 0);
    } else {
      Script_lightSetSlotEnabled(5, 1);
      Script_lightSetSlotEnabled(1, 0);
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_1587(void) {
  int32_t local_FC;
  script_1608();
  Script_sprintf("シーン再生14_2\012");
  local_FC = Script_scenCreateTask(script_15CD, 0);
  Script_shenSeqUtils(1);
  while (Script_shenSeqUtils(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  script_1652();
  Script_srchControl((void *)local_FC);
  return;
}

__attribute__((scn_string_bindings(0, data_44D6)))
static int32_t script_15CD(void) {
  int32_t unused_F4[3];
  int16_t local_F2;
  uint8_t unused_F0[2];
  local_F2 = 0;
  for (;;) {
    if (Script_shenSeqUtils(-1)) {
      local_F2 = local_F2 + 1;
      SCNF_WaitFrames_0000(1);
    } else {
      Script_sprintf("test\012");
      local_F2 = 0;
      break;
    }
  }
  end:;
}

static void script_1608(void) {
  int32_t unused_F0[4];
  script_0048(CHARA_AKIR);
  script_0048(CHARA_TONY);
  script_0048(CHARA_SMTH);
  Script_facemotUtils(CHARA_AKIR, 1, 0);
  Script_charaDisplay(SHEN_NAME('BIN_'), CDM_Show);
  Script_SetShadowFlags(0, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  return;
}

static void script_1652(void) {
  script_00D5(CHARA_AKIR);
  script_00D5(CHARA_TONY);
  script_00D5(CHARA_SMTH);
  Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
  Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
  return;
}

static int32_t script_1699(int32_t arg0, int32_t arg1, int32_t arg2) {
  int32_t local_FC;
  local_FC = arg2;
  switch (local_FC) {
    case 1:
      break;
    case 2:
      Script_scenCreateTask(script_0498, 6, 20497, -1, 0, 0x3F800000, arg0, arg1);
      break;
    case 3:
      Script_scenCreateTask(script_0591, 4, arg0, 1, 11, 4);
      Script_scenCreateTask(script_0498, 6, 20487, -1, 0x41000000, 0x3F800000, arg0, arg1);
      break;
    case 4:
      Script_scenCreateTask(script_039D, 2, 161451, 17);
      Script_scenCreateTask(script_039D, 2, 0x1976AB, 13);
      Script_scenCreateTask(script_039D, 2, 0x2876AB, 58);
      Script_scenCreateTask(script_0498, 6, 20495, -1, 0, 0x3F800000, arg0, arg1);
      break;
    case 5:
      Script_scenCreateTask(script_039D, 2, 30379, 8);
      Script_scenCreateTask(script_0498, 6, 20488, -1, 0, 0x3F800000, arg0, arg1);
      break;
    case 6:
      Script_scenCreateTask(script_052F, 5, arg0, arg1, 0, -1, 20);
      Script_scenCreateTask(script_0498, 6, 20496, -1, 0, 0x3F800000, arg0, arg1);
      break;
    case 7:
      Script_scenCreateTask(script_0591, 4, arg0, 1, -1, 15);
      Script_scenCreateTask(script_052F, 5, arg0, arg1, 0, -1, 13);
      Script_scenCreateTask(script_0498, 6, 20482, -1, 0, 0x3F800000, arg0, arg1);
      break;
    case 8:
      Script_scenCreateTask(script_039D, 2, 30379, 13);
      Script_scenCreateTask(script_039D, 2, 0x2876AB, 35);
      Script_scenCreateTask(script_0498, 6, 20493, -1, 0x41F00000, 0x3F800000, arg0, arg1);
      break;
    case 9:
      Script_scenCreateTask(script_0498, 6, 20483, -1, 0, 0x3F800000, arg0, arg1);
      break;
    case 10:
      Script_scenCreateTask(script_039D, 2, 161451, 7);
      Script_scenCreateTask(script_0498, 6, 20494, -1, 0x41F00000, 0x3F800000, arg0, arg1);
      Script_scenCreateTask(script_05ED, 3, arg0, -0x3E380000, -1);
      break;
    case 11:
      Script_scenCreateTask(script_0498, 6, 20491, -1, 0, 0x3F800000, arg0, arg1);
      task_54 = task_54 | 0x20000000;
      break;
    case 12:
      Script_scenCreateTask(script_039D, 2, 161451, 12);
      Script_scenCreateTask(script_0498, 6, 20499, -1, 0, 0x3F800000, arg0, arg1);
      task_54 = task_54 | 0x20000000;
      break;
    case 13:
      Script_scenCreateTask(script_039D, 2, 0x1376AB, 13);
      Script_scenCreateTask(script_0591, 4, arg0, 1, -1, 15);
      Script_scenCreateTask(script_0498, 6, 20490, -1, 0x41200000, 0x3F8CCCCD, arg0, arg1);
      break;
    case 14:
      Script_scenCreateTask(script_039D, 2, 0x2876AB, 55);
      Script_scenCreateTask(script_0498, 6, 20498, -1, 0x41200000, 0x3F8CCCCD, arg0, arg1);
      Script_scenCreateTask(script_05ED, 3, arg0, 0x41A00000, -1);
      break;
    case 15:
      Script_scenCreateTask(script_039D, 2, 161451, 26);
      Script_scenCreateTask(script_0591, 4, arg0, 1, -1, 50);
      Script_scenCreateTask(script_0498, 6, 20484, -1, 0, 0x3F666666, arg0, arg1);
      task_54 = task_54 ^ -0x80000000;
      break;
    case 16:
      Script_scenCreateTask(script_039D, 2, 0x2876AB, 47);
      Script_scenCreateTask(script_0498, 6, 20500, -1, 0x422C0000, 0x3F800000, arg0, arg1);
      break;
    case 17:
      Script_scenCreateTask(script_039D, 2, 0x2876AB, 40);
      Script_scenCreateTask(script_0498, 6, 20500, -1, 0x42480000, 0x3F800000, arg0, arg1);
      Script_scenCreateTask(script_05ED, 3, arg0, -0x3E100000, -1);
      break;
    case 18:
      Script_scenCreateTask(script_039D, 2, 30379, 18);
      Script_scenCreateTask(script_0591, 4, arg0, 1, 10, 10);
      Script_scenCreateTask(script_052F, 5, arg0, arg1, 0, -1, 30);
      Script_scenCreateTask(script_0498, 6, 20485, -1, 0, 0x3F800000, arg0, arg1);
      break;
  }
  return Script_srchControl(0);
}

static void script_1B5A(int32_t arg0, int32_t arg1, int32_t arg2) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t local_E4;
  int32_t local_E0;
  int32_t local_DC;
  SCNF_WaitFrames_0000(arg2);
  Script_charaDisplay(arg0, CDM_Show);
  Script_charaDisplay(arg1, CDM_Hide);
  local_E4 = 2;
  local_E0 = 2;
  Script_scenCreateTask(script_039D, 2, 264105, (20 - local_E4) / local_E0);
  Script_scenCreateTask(script_039D, 2, 198569, (35 - local_E4) / local_E0);
  for (;;) {
    local_DC = 0;
    while (local_DC < 8) {
      Script_InterpolateAndScaleAngles(local_DC, local_E4, &local_F4, &local_E8, "YDAD");
      local_F4.x = (&local_F4)->x * 10.0f;
      (&local_F4)->y = (&local_F4)->y * 10.0f;
      (&local_F4)->z = (&local_F4)->z * 10.0f;
      Script_charaPosRotUtils(arg0, *(int32_t *)(data_459C + (local_DC << 2)), &local_F4, &local_E8, 0);
      local_DC = local_DC + 1;
    }
    local_E4 = local_E4 + local_E0;
    if (local_E4 >= 110) {
      Script_srchControl(0);
    }
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static int32_t script_1C4D(int32_t arg0, int32_t arg1, int32_t arg2) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t local_E4;
  int32_t local_E0;
  int32_t local_DC;
  SCNF_WaitFrames_0000(arg2);
  Script_charaDisplay(arg0, CDM_Show);
  Script_charaDisplay(arg1, CDM_Hide);
  local_E4 = 2;
  local_E0 = 2;
  Script_scenCreateTask(script_039D, 2, 264105, (20 - local_E4) / local_E0);
  Script_scenCreateTask(script_039D, 2, 198569, (35 - local_E4) / local_E0);
  for (;;) {
    local_DC = 0;
    while (local_DC < 8) {
      Script_InterpolateAndScaleAngles(local_DC, local_E4, &local_F4, &local_E8, "YDBD");
      local_F4.x = (&local_F4)->x * 10.0f;
      (&local_F4)->y = (&local_F4)->y * 10.0f;
      (&local_F4)->z = (&local_F4)->z * 10.0f;
      Script_charaPosRotUtils(arg0, *(int32_t *)(data_459C + (local_DC << 2)), &local_F4, &local_E8, 0);
      local_DC = local_DC + 1;
    }
    local_E4 = local_E4 + local_E0;
    if (local_E4 >= 110) {
      Script_srchControl(0);
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_1D40(int32_t arg0, int32_t arg1, int32_t arg2) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t local_E4;
  int32_t local_E0;
  int32_t local_DC;
  SCNF_WaitFrames_0000(arg2);
  Script_charaDisplay(arg0, CDM_Show);
  Script_charaDisplay(arg1, CDM_Hide);
  local_E4 = 2;
  local_E0 = 2;
  Script_scenCreateTask(script_039D, 2, 329641, (15 - local_E4) / local_E0);
  for (;;) {
    local_DC = 0;
    while (local_DC < 12) {
      Script_InterpolateAndScaleAngles(local_DC, local_E4, &local_F4, &local_E8, "BC0D");
      local_F4.x = (&local_F4)->x * 10.0f;
      (&local_F4)->y = (&local_F4)->y * 10.0f;
      (&local_F4)->z = (&local_F4)->z * 10.0f;
      Script_charaPosRotUtils(arg0, *(int32_t *)(data_45BC + (local_DC << 2)), &local_F4, &local_E8, 0);
      local_DC = local_DC + 1;
    }
    local_E4 = local_E4 + local_E0;
    if (local_E4 >= 72) {
      Script_srchControl(0);
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_1E1B(int32_t arg0, int32_t arg1) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t local_E4;
  int32_t local_E0;
  int32_t local_DC;
  SCNF_WaitFrames_0000(arg1);
  local_E4 = 2;
  local_E0 = 2;
  Script_scenCreateTask(script_039D, 2, 329641, (20 - local_E4) / local_E0);
  for (;;) {
    local_DC = 0;
    while (local_DC < 12) {
      Script_InterpolateAndScaleAngles(local_DC, local_E4, &local_F4, &local_E8, "BC1D");
      local_F4.x = (&local_F4)->x * 10.0f;
      (&local_F4)->y = (&local_F4)->y * 10.0f;
      (&local_F4)->z = (&local_F4)->z * 10.0f;
      Script_charaPosRotUtils(arg0, *(int32_t *)(data_45BC + (local_DC << 2)), &local_F4, &local_E8, 0);
      local_DC = local_DC + 1;
    }
    local_E4 = local_E4 + local_E0;
    if (local_E4 >= 132) {
      Script_srchControl(0);
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_1EE7(int32_t arg0, int32_t arg1) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t local_E4;
  int32_t local_E0;
  int32_t local_DC;
  SCNF_WaitFrames_0000(arg1);
  local_E4 = 2;
  local_E0 = 2;
  Script_scenCreateTask(script_039D, 2, 329641, (20 - local_E4) / local_E0);
  for (;;) {
    local_DC = 0;
    while (local_DC < 12) {
      Script_InterpolateAndScaleAngles(local_DC, local_E4, &local_F4, &local_E8, "BC2D");
      local_F4.x = (&local_F4)->x * 10.0f;
      (&local_F4)->y = (&local_F4)->y * 10.0f;
      (&local_F4)->z = (&local_F4)->z * 10.0f;
      Script_charaPosRotUtils(arg0, *(int32_t *)(data_45BC + (local_DC << 2)), &local_F4, &local_E8, 0);
      local_DC = local_DC + 1;
    }
    local_E4 = local_E4 + local_E0;
    if (local_E4 >= 120) {
      Script_srchControl(0);
    }
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static int32_t script_1FB2(int32_t arg0, int32_t arg1, int32_t arg2) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t local_E4;
  int32_t local_E0;
  int32_t local_DC;
  SCNF_WaitFrames_0000(arg2);
  Script_charaDisplay(arg0, CDM_Show);
  Script_charaDisplay(arg1, CDM_Hide);
  local_E4 = 2;
  local_E0 = 1;
  Script_scenCreateTask(script_039D, 2, 328362, (10 - local_E4) / local_E0);
  for (;;) {
    local_DC = 0;
    while (local_DC < 37) {
      Script_InterpolateAndScaleAngles(local_DC, local_E4, &local_F4, &local_E8, "KADY");
      local_F4.x = (&local_F4)->x * 10.0f;
      (&local_F4)->y = (&local_F4)->y * 10.0f;
      (&local_F4)->z = (&local_F4)->z * 10.0f;
      Script_charaPosRotUtils(arg0, *(int32_t *)(data_45EC + (local_DC << 2)), &local_F4, &local_E8, 0);
      local_DC = local_DC + 1;
    }
    local_E4 = local_E4 + local_E0;
    if (local_E4 >= 100) {
      Script_srchControl(0);
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_208D(int32_t arg0, int32_t arg1, float arg2, int32_t arg3, const void *arg4) {
  SCNF_WaitFrames_0000(1);
  task_150[arg1] = arg0;
  task_15C[arg1] = arg3;
  script_20FA(arg1, arg2, arg4);
  for (;;) {
    if (task_168[arg1]) {
      task_174[arg1] = Script_walk3D(ActivateGetIdx, arg0, task_180[arg1]);
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_20FA(int32_t arg0, float arg1, const void *arg2) {
  int32_t local_FC;
  task_168[arg0] = 0;
  task_174[arg0] = 0;
  task_180[arg0] = arg1;
  local_FC = task_15C[arg0];
  if (local_FC == 0) {
    Script_PlayCharaMotion(task_150[arg0], MN_AKI_AKI_L_STAND, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    goto L002163;
  }
  L002163:;
  Script_walk3D(SetupPath, task_150[arg0], arg2, 0);
  return;
}

static void script_217C(int32_t arg0, float arg1) {
  int32_t local_FC;
  task_180[arg0] = arg1;
  task_168[arg0] = 1;
  local_FC = task_15C[arg0];
  if (local_FC == 0) {
    Script_charaToggleMotEnabled(task_150[arg0], 0);
    Script_SetCharaMotionPlaybackRate(task_150[arg0], 1.0f);
    Script_PlayCharaMotion(task_150[arg0], MN_AKI_AKI_WALK_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    goto L0021FB;
  }
  L0021FB:;
  return;
}

static void script_21FC(int32_t arg0) {
  int32_t local_FC;
  task_168[arg0] = 0;
  local_FC = task_15C[arg0];
  if (local_FC == 0) {
    Script_charaToggleMotEnabled(task_150[arg0], 1);
    Script_SetCharaMotionPlaybackRate(task_150[arg0], 1.0f);
    goto L002248;
  }
  L002248:;
  return;
}

static int32_t script_2249(void) {
  int32_t unused_EC[5];
  int32_t local_E8;
  local_E8 = Script_scenCreateTask(script_153A, 0);
  Script_shenSetLightingMode(2);
  Script_SetupLightFromTable(0);
  Script_SetFogFlags(0, data_4818);
  Script_SetFogColour(&data_47F8);
  (void)1;
  Script_SetVector4ByOffset(&data_47D8);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 35.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 30.0f);
  for (;;) {
    SCNF_WaitFrames_0000(1);
    if (task_8 == 1) {
      Script_shenSetLightingMode(1);
      Script_lightSetSlotEnabled(0, 1);
      Script_LightUtils(0, 0, 2);
      (void)0;
      Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 100.0f);
      Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 10.0f);
      Script_srchControl((void *)local_E8);
      Script_srchControl(0);
    }
  }
  end:;
}

static void script_22F2(int32_t arg0, int32_t arg1) {
  Vector4 local_F0;
  Vector3 local_E4;
  float local_E0;
  int32_t local_DC;
  local_DC = arg0 * 14;
  local_F0.x = *(float *)(&data_4680 + local_DC);
  (&local_F0)->y = *(float *)(&data_4680 + (local_DC + 1));
  (&local_F0)->z = *(float *)(&data_4680 + (local_DC + 2));
  (&local_F0)->w = *(float *)(&data_4680 + (local_DC + 3));
  local_E4.x = *(float *)(&data_4680 + (local_DC + 10));
  (&local_E4)->y = *(float *)(&data_4680 + (local_DC + 11));
  (&local_E4)->z = *(float *)(&data_4680 + (local_DC + 12));
  local_E0 = *(float *)(&data_4680 + (local_DC + 9));
  Script_LightUtils(arg0, 0, 2);
  Script_LightUtils(arg0, 2, &local_F0);
  Script_LightUtils(arg0, 8, &local_E4);
  Script_LightUtils(arg0, 4, local_E0);
  if (arg1 & -0x80000000) {
    Script_lightSetSlotEnabled(arg0, 1);
  }
  if (arg1 & 0x40000000) {
    Script_lightSetSlotEnabled(arg0, 0);
  }
  for (;;) {
    if (arg1 & 0x20000000) {
      if (!(Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 1.0f) >= 0.1f)) {
        Script_lightSetSlotEnabled(arg0, 0);
      } else {
        Script_lightSetSlotEnabled(arg0, 1);
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_2432(int32_t arg0, int32_t arg1) {
  Vector4 local_F0;
  Vector3 local_E4;
  int32_t local_DC[2];
  float local_D8;
  float local_D4;
  int32_t local_D0;
  local_D0 = arg0 * 14;
  local_F0.x = *(float *)(&data_4680 + local_D0);
  (&local_F0)->y = *(float *)(&data_4680 + (local_D0 + 1));
  (&local_F0)->z = *(float *)(&data_4680 + (local_D0 + 2));
  (&local_F0)->w = *(float *)(&data_4680 + (local_D0 + 3));
  local_E4.x = *(float *)(&data_4680 + (local_D0 + 4));
  (&local_E4)->y = *(float *)(&data_4680 + (local_D0 + 5));
  (&local_E4)->z = *(float *)(&data_4680 + (local_D0 + 6));
  local_D8 = *(float *)(&data_4680 + (local_D0 + 7));
  local_DC[0] = *(&data_4680 + (local_D0 + 8));
  local_DC[1] = 0;
  local_D4 = *(float *)(&data_4680 + (local_D0 + 9));
  Script_LightUtils(arg0, 0, 3);
  Script_LightUtils(arg0, 2, &local_F0);
  Script_LightUtils(arg0, 7, &local_E4);
  Script_LightUtils(arg0, 5, local_D8);
  Script_LightUtils(arg0, 6, (const float *)local_DC);
  Script_LightUtils(arg0, 4, local_D4);
  if (arg1 & -0x80000000) {
    Script_lightSetSlotEnabled(arg0, 1);
  }
  if (arg1 & 0x40000000) {
    Script_lightSetSlotEnabled(arg0, 0);
  }
  for (;;) {
    if (arg1 & 0x20000000) {
      if (!(Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 1.0f) >= 0.1f)) {
        Script_lightSetSlotEnabled(arg0, 0);
      } else {
        Script_lightSetSlotEnabled(arg0, 1);
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_25B5(int32_t arg0, int32_t arg1) {
  Vector4 local_F0;
  Vector3 local_E4;
  Vector3 local_D8;
  int32_t local_D0[2];
  float local_CC;
  int32_t local_C8;
  float local_C4;
  int32_t local_C0;
  local_C0 = arg0 * 14;
  local_F0.x = *(float *)(&data_4680 + local_C0);
  (&local_F0)->y = *(float *)(&data_4680 + (local_C0 + 1));
  (&local_F0)->z = *(float *)(&data_4680 + (local_C0 + 2));
  (&local_F0)->w = *(float *)(&data_4680 + (local_C0 + 3));
  local_E4.x = *(float *)(&data_4680 + (local_C0 + 4));
  (&local_E4)->y = *(float *)(&data_4680 + (local_C0 + 5));
  (&local_E4)->z = *(float *)(&data_4680 + (local_C0 + 6));
  local_CC = *(float *)(&data_4680 + (local_C0 + 7));
  local_D0[0] = *(&data_4680 + (local_C0 + 8));
  local_D0[1] = 0;
  local_C4 = *(float *)(&data_4680 + (local_C0 + 9));
  local_D8.x = *(float *)(&data_4680 + (local_C0 + 10));
  (&local_D8)->y = *(float *)(&data_4680 + (local_C0 + 11));
  (&local_D8)->z = *(float *)(&data_4680 + (local_C0 + 12));
  local_C8 = *(&data_4680 + (local_C0 + 13));
  Script_LightUtils(arg0, 0, 4);
  Script_LightUtils(arg0, 2, &local_F0);
  Script_LightUtils(arg0, 7, &local_E4);
  Script_LightUtils(arg0, 5, local_CC);
  Script_LightUtils(arg0, 6, (const float *)local_D0);
  Script_LightUtils(arg0, 8, &local_D8);
  Script_LightUtils(arg0, 10, local_C8);
  Script_LightUtils(arg0, 4, local_C4);
  if (arg1 & -0x80000000) {
    Script_lightSetSlotEnabled(arg0, 1);
  }
  if (arg1 & 0x40000000) {
    Script_lightSetSlotEnabled(arg0, 0);
  }
  for (;;) {
    if (arg1 & 0x20000000) {
      if (!(Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 1.0f) >= 0.1f)) {
        Script_lightSetSlotEnabled(arg0, 0);
      } else {
        Script_lightSetSlotEnabled(arg0, 1);
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  return;
}

__attribute__((scn_string_bindings(0, data_4828, 2, data_483C)))
static int32_t script_279E(void) {
  int32_t unused_FC;
  GameDateTime local_F4;
  int32_t unused_F0;
  GameDateTime local_E8;
  Script_ReturnTimeDate(&local_F4);
  Script_ReturnTimeDate(&local_E8);
  (&local_F4)->hours = 22;
  (&local_F4)->minutes = 0;
  (&local_E8)->hours = 23;
  (&local_E8)->minutes = 0;
  Script_SetNewDateTimeWrapper(&local_F4, &local_E8, 30);
  Script_SpriteUtils(Scroll_Read1, (int32_t)"scroll", "SCROLL99.SPR");
  Script_SetMapFlags(0, 1);
  Script_GetSetWeatherSettings(WEATHER_SETTINGS_SET_DISABLE_WEATHER, 1);
  Script_WeatherUtils_0(WeatherDataUtil_ApplyCurrentWeatherOrEnableDateTimeRead, 0);
  for (;;) {
    SCNF_WaitFrames_0000(1);
    if (task_8 == 1) {
      Script_SpriteUtils(Scroll_Read1, (int32_t)"scroll", "SCROLL00.SPR");
      Script_GetSetWeatherSettings(WEATHER_SETTINGS_SET_DISABLE_WEATHER, 1);
      Script_srchControl(0);
    }
  }
  end:;
}

static void script_283C(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Script_scenCreateTask(script_2852, 4, arg0, arg1, arg2, arg3);
  return;
}

static int32_t script_2852(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  if (arg3 >= 0) {
    SCNF_WaitFrames_0000(arg3);
  }
  Script_seqEnterEvtCam(arg0, arg1, arg2);
  return Script_srchControl(0);
}

static void script_2876(int32_t arg0, int32_t arg1) {
  Script_scenCreateTask(script_2888, 2, arg0, arg1);
  return;
}

static int32_t script_2888(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  if (arg1 >= 0) {
    SCNF_WaitFrames_0000(arg1);
  }
  Script_StartOrFreeSEQ(0);
  local_FC = arg0;
  switch (local_FC) {
    case 0:
      Script_ShowUIMessageFromID("E1002B001");
      break;
    case 1:
      Script_ShowUIMessageFromID("E1002B002");
      break;
    case 2:
      Script_ShowUIMessageFromID("E1002B003");
      break;
    case 3:
      Script_ShowUIMessageFromID("E1002B004");
      break;
    case 4:
      Script_ShowUIMessageFromID("E1002B005");
      break;
    case 5:
      Script_ShowUIMessageFromID("E1002B006");
      break;
    case 6:
      Script_ShowUIMessageFromID("E1002B007");
      break;
    case 7:
      Script_ShowUIMessageFromID("E1002B008");
      break;
    case 8:
      Script_ShowUIMessageFromID("E1002B009");
      break;
    case 9:
      Script_ShowUIMessageFromID("E1002B010");
      break;
    case 10:
      Script_ShowUIMessageFromID("E1002C001");
      break;
    case 11:
      Script_ShowUIMessageFromID("E1002C002");
      break;
    case 12:
      Script_ShowUIMessageFromID("E1002C003");
      break;
    case 13:
      Script_ShowUIMessageFromID("E1002C004");
      break;
    case 14:
      Script_ShowUIMessageFromID("E1002C005");
      break;
    case 15:
      Script_ShowUIMessageFromID("E1002C006");
      break;
    case 16:
      Script_ShowUIMessageFromID("E1002C007");
      break;
    case 17:
      Script_ShowUIMessageFromID("E1002C008");
      break;
    case 18:
      Script_ShowUIMessageFromID("E1002C009");
      break;
    case 19:
      Script_ShowUIMessageFromID("E1002D001");
      break;
  }
  return Script_srchControl(0);
}

static void script_2A53(int32_t arg0, int32_t arg1) {
  task_18C = arg0;
  task_0 = 0;
  return;
}

static void script_2A64(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  if (arg1 >= 0) {
    SCNF_WaitFrames_0000(arg1);
  }
  local_FC = arg0;
  switch (local_FC) {
    case 0:
      SCNT_ShowSubtitle("開始０");
      break;
    case 1:
      SCNT_ShowSubtitle("成功０");
      break;
    case 2:
      SCNT_ShowSubtitle("失敗０");
      break;
    case 3:
      SCNT_ShowSubtitle("開始１");
      break;
    case 4:
      SCNT_ShowSubtitle("成功１");
      break;
    case 5:
      SCNT_ShowSubtitle("失敗１");
      break;
    case 6:
      SCNT_ShowSubtitle("開始２");
      break;
    case 7:
      SCNT_ShowSubtitle("成功２");
      break;
    case 8:
      SCNT_ShowSubtitle("失敗２");
      break;
    case 9:
      SCNT_ShowSubtitle("開始３");
      break;
    case 10:
      SCNT_ShowSubtitle("成功３");
      break;
    case 11:
      SCNT_ShowSubtitle("失敗３");
      break;
    case 12:
      SCNT_ShowSubtitle("開始４");
      break;
    case 13:
      SCNT_ShowSubtitle("成功４");
      break;
    case 14:
      SCNT_ShowSubtitle("失敗４");
      break;
  }
  Script_srchControl(0);
  return;
}

static int32_t script_2B83(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  int32_t local_F0;
  Vector3i local_E4;
  local_F8 = Script_scenCreateTask(script_208D, 5, arg0, arg1, 0x3D343958, 0, &data_4850);
  local_F4 = Script_SetMaxNPCsNear(7);
  local_F0 = 0;
  SCNF_WaitFrames_0000(5);
  Script_SetSfxOverlayTimeoutTuning(0, SfxMode_SessionAction);
  Script_uiUtils(0, 21, 0);
  for (;;) {
    Script_filmCheckOrSetTransitionUtil(0, 255, 0, 0, 0, 0, 0, 0, 0);
    Script_PlayCharaMotion(arg0, MN_AKI_AKI_STAND_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    for (;;) {
      Script_PrintDebugString(15, 14, "PRESS A BUTTON");
      if (Script_GetInputData(0, 2) & 0x400) {
        break;
      }
      SCNF_WaitFrames_0000(1);
    }
    Script_filmCheckOrSetTransitionUtil(60, 255, 0, 0, 0, 0, 0, 0, 0);
    Script_charaDisplay(SHEN_NAME('KBK1'), CDM_Show);
    Script_charaDisplay(SHEN_NAME('KBK0'), CDM_Hide);
    Script_charaDisplay(SHEN_NAME('BCA0'), CDM_Show);
    Script_charaDisplay(SHEN_NAME('BCD0'), CDM_Hide);
    Script_charaDisplay(SHEN_NAME('YDM0'), CDM_Show);
    Script_charaDisplay(SHEN_NAME('YDD0'), CDM_Hide);
    if (local_F0) {
      Script_sicnUtils(0, 1, 0);
    }
    Script_LoadSND(0, 4, "n1014_4.snd", 0);
    Script_executeAicaCommand(6312, 0, 0);
    script_1403();
    script_1587();
    Script_ccowUtils(arg0, 2);
    Script_ccowUtils(task_58[1], 2);
    Script_ccowUtils(task_58[2], 2);
    Script_filmCheckOrSetTransitionUtil(0, 0, 0, 0, 0, 0, 0, 0, 0);
    Script_executeAicaCommand(2720, 2, 64);
    if (local_F0) {
      Script_sicnUtils(1, 0, 0);
    }
    Script_LoadSND(0, 4, "n1sqt_4.snd", 0);
    Script_executeAicaCommand(71848, 0, 0);
    Script_executeAicaCommand(1184, 2, 127);
    local_FC = Script_scenCreateTask(script_2E5B, 2, arg0, arg1);
    task_9C = 0;
    local_F0 = 1;
    for (;;) {
      if (task_9C >= 10) {
        Script_srchControl((void *)local_FC);
        if (task_18C == 3) {
          script_283C(8000, arg0, arg0, -1);
        }
        if (task_18C != 14) {
          SCNF_WaitFrames_0000(40);
          (&local_E4)->y = -12743;
          Script_EVT_SetRotation(arg0, -0x70000000, &local_E4, 0);
          Script_PlayCharaMotion(arg0, MN_AKI_SIPAII_YARARE_W2QTE, 20.0f, -1.0f, -1.0f, 1.0f, 0);
        }
        break;
      }
      if (task_9C == -1) {
        Script_srchControl((void *)local_FC);
        break;
      }
      SCNF_WaitFrames_0000(1);
    }
    Script_executeAicaCommand(2720, 2, 64);
    Script_filmCheckOrSetTransitionUtil(60, 0, 0, 0, 0, 255, 0, 0, 0);
    while (Script_filmCheckOrSetTransitionUtil(-1)) {
      SCNF_WaitFrames_0000(1);
    }
    if (task_9C == -1) {
      script_21FC(arg1);
      SCNF_WaitFrames_0000(10);
      SCNF_WaitFrames_0000(60);
    }
    Script_SetSfxOverlayTimeoutTuning(1, SfxMode_SessionAction);
  }
  end:;
}

static int32_t script_2E5B(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  script_0CFF(arg0, -1, -119.644997f, 71.609001f, 337.659302f);
  script_0CFF(task_58[1], arg0, 0.18f, -0.6f, 160.0f);
  script_0CFF(task_58[2], arg0, -0.4f, -3.3f, 180.0f);
  Script_PlayCharaMotion(task_58[1], MN_OTH_GIA_TUKAMI_QTE, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_PlayCharaMotion(task_58[2], MN_YKI_YKI_NOMU_DRINK_F, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_motmToggleFlags(task_58[2], 1, 0x80);
  script_20FA(arg1, 0.044f, &data_4850);
  script_2A53(0, -1);
  script_283C(8000, arg0, arg0, -1);
  SCNF_WaitFrames_0000(30);
  script_2876(2, -1);
  task_90 = 0;
  while (task_90 == 0) {
    script_2A53(3, -1);
    script_08F4(arg1, 2, 1, 0);
    if (task_90) {
      script_2A53(4, -1);
      script_2876(7, 50);
      script_283C(8001, arg0, arg0, -1);
    } else {
      script_2A53(5, -1);
      script_283C(8001, arg0, arg0, -1);
      local_FC = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 4.0f));
      if (local_FC == 0) {
        script_2876(4, 10);
      } else if (local_FC == 1) {
        script_2876(5, 10);
      } else if (local_FC == 2) {
        script_2876(6, 10);
      } else {
        script_2876(3, 10);
      }
    }
  }
  Script_scenCreateTask(script_1D40, 3, SHEN_NAME('BCD0'), SHEN_NAME('BCA0'), 50);
  SCNF_WaitFrames_0000(70);
  script_0CFF(task_58[2], arg0, -0.25f, -2.8f, 180.0f);
  script_2A53(6, -1);
  script_2876(10, -1);
  script_283C(8000, arg0, arg0, -1);
  script_08F4(arg1, 0, 2, 0);
  if (task_90) {
    script_2A53(7, -1);
    script_2876(11, 15);
    script_283C(8001, arg0, arg0, -1);
    script_283C(8005, SHEN_NAME('DUM0'), SHEN_NAME('DUM0'), 40);
    script_283C(8001, arg0, arg0, 100);
    Script_scenCreateTask(script_1C4D, 3, SHEN_NAME('YDD0'), SHEN_NAME('YDM0'), 50);
    SCNF_WaitFrames_0000(68);
  } else {
    script_2A53(8, -1);
    script_2876(16, -1);
    script_283C(8001, arg0, arg0, -1);
    script_08F4(arg1, 1, 2, 0);
    if (task_90) {
      script_2A53(10, -1);
      script_2876(14, 12);
      script_283C(8000, arg0, arg0, -1);
      script_283C(8001, arg0, arg0, 30);
      Script_scenCreateTask(script_1E1B, 2, SHEN_NAME('BCD0'), 30);
      SCNF_WaitFrames_0000(55);
    } else {
      script_2A53(11, -1);
      script_2876(15, -1);
      SCNF_WaitFrames_0000(20);
      script_0CFF(task_58[2], arg0, -0.2f, -1.5f, 160.0f);
      script_08F4(arg1, 3, 2, -0x80000000);
      script_283C(8000, arg0, arg0, -1);
      if (task_90) {
        script_2A53(13, -1);
        script_2876(18, 24);
        script_283C(8000, arg0, arg0, -1);
        script_283C(8002, SHEN_NAME('DUM0'), SHEN_NAME('DUM0'), 30);
        script_283C(8001, arg0, arg0, 100);
        Script_scenCreateTask(script_1E1B, 2, SHEN_NAME('BCD0'), 30);
        script_064A(arg0);
      } else {
        script_2A53(14, -1);
        script_2876(13, -1);
        script_283C(8003, SHEN_NAME('DUM0'), SHEN_NAME('DUM0'), 18);
        Script_scenCreateTask(script_1FB2, 3, SHEN_NAME('KBK0'), SHEN_NAME('KBK1'), 30);
        SCNF_WaitFrames_0000(60);
        Script_PlayCharaMotion(task_58[2], MN_YKI_YKI_NOMU_DRINK_F, 30.0f, -1.0f, -1.0f, 1.0f, 0);
        Script_motmToggleFlags(task_58[2], 1, 0x80);
        task_9C = 10;
        SCNF_WaitFrames_0000(1);
      }
    }
  }
  script_217C(arg1, 0.044f);
  Script_SetCameraSettings(5, 6, 1, 0);
  Script_convSetCamInactiveFlag(0);
  SCNF_WaitFrames_0000(80);
  Script_SetGameStateTo(AUTH_None);
  task_9C = -1;
  SCNF_WaitFrames_0000(1);
  end:;
}

void script_3342(void) {
  Script_filmCheckOrSetTransitionUtil(0, 255, 0, 0, 0, 0, 0, 0, 0);
  SCNF_WaitFrames_0000(1);
  SCNF_WaitFrames_0000(1);
  script_33F8(0);
  task_0 = 0;
  for (;;) {
    task_0 = task_0 + 1;
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_3392(void) {
  Script_filmCheckOrSetTransitionUtil(60, 0, 0, 0, 0, 255, 0, 0, 0);
  Script_filmCheckOrSetTransitionUtil(60, 0, 0, 0, 0, 255, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  script_33F8(0);
  for (;;) {
    if (task_8 == 1) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  return;
}

__attribute__((scn_string_bindings(4, data_4A02, 5, data_4A08)))
static void script_33F8(int32_t arg0) {
  task_8 = 0;
  Script_scenCreateTask(script_2249, 0);
  Script_scenCreateTask(script_279E, 0);
  SCNF_WaitFrames_0000(1);
  Script_scenCreateTask(script_37CA, 2, SHEN_NAME('DRM0'), 0);
  Script_scenCreateTask(script_37CA, 2, SHEN_NAME('DRM1'), 0);
  Script_scenCreateTask(script_37CA, 2, SHEN_NAME('DRM2'), 0);
  Script_scenCreateTask(script_37CA, 2, SHEN_NAME('KBK0'), 0);
  Script_scenCreateTask(script_37CA, 2, SHEN_NAME('KBK1'), 0);
  Script_scenCreateTask(script_37CA, 2, SHEN_NAME('KBK2'), 0);
  Script_scenCreateTask(script_38CA, 2, SHEN_NAME('YDM0'), 0);
  Script_scenCreateTask(script_38CA, 2, SHEN_NAME('YDD0'), 0);
  Script_scenCreateTask(script_37CA, 2, SHEN_NAME('BCA0'), 0);
  Script_scenCreateTask(script_37CA, 2, SHEN_NAME('BCD0'), 0);
  Script_scenCreateTask(script_3822, 3, SHEN_NAME('BIN_'), 0, CHARA_SMTH);
  Script_scenCreateTask(script_3705, 4, CHARA_TONY, 1, 2, 2);
  Script_scenCreateTask(script_3705, 4, CHARA_SMTH, 2, 2, 0);
  Script_scenCreateTask(script_3632, 2, CHARA_AKIR, 0);
  task_4 = Script_scenCreateTask(script_2B83, 2, CHARA_AKIR, 0);
  Script_executeAicaCommand(131232, 0, 0);
  Script_LoadSND(0, 0, "system1.snd", 0);
  Script_LoadSND(0, 1, "s2025_4.snd", 0);
  Script_LoadSND(0, 5, "qtebat.snd", 0);
  Script_LoadSND(0, 2, "bgm041.snd", 0);
  Script_LoadVoiceFile(1, "E1002");
  Script_LoadVoiceFile(1, "A01114");
  task_C = Script_motiLoad(1, SHEN_NAME('YQ14'), "M_YQ14.BIN");
  *(&task_C + 1) = Script_motiLoad(1, SHEN_NAME('YQ14'), "M_01114.BIN");
  SCNF_WaitFrames_0000(1);
  return;
}

__attribute__((scn_string_bindings(0, data_4A26, 1, data_4A2D)))
static void script_35DF(void) {
  task_8 = 1;
  Script_srchControl((void *)task_4);
  Script_executeAicaCommand(131232, 0, 0);
  Script_LoadVoiceFile(0, "A01114");
  Script_LoadVoiceFile(0, "E1002");
  Script_ReallocateUNFO(*&task_C);
  Script_ReallocateUNFO(*(&task_C + 1));
  SCNF_WaitFrames_0000(1);
  return;
}

static int32_t script_3632(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  int32_t local_F8;
  local_FC = Script_scenCreateTask(script_0AFA, 3, arg0, arg1, 0);
  local_F8 = Script_scenCreateTask(script_0666, 6, arg0, arg1, 2, 2, &data_44DC, script_1699);
  Script_charaDisplay(arg0, CDM_Show);
  Script_ryoSetChara(arg0);
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_motmToggleFlags(arg0, 1, 0x40);
  Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_7);
  Script_SetCPAMFlags(arg0, 0);
  Script_SetShadowFlags(arg0, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_SetShadowFlags(arg0, SCRIPT_SHADOW_UPDATE_LIGHT_MASK, 1);
  Script_SetShadowFlags(arg0, SCRIPT_SHADOW_UPDATE_LIGHT_MASK, 5);
  Script_clmdSetFlags(arg0, 8, 1);
  for (;;) {
    SCNF_WaitFrames_0000(1);
    if (task_8 == 1) {
      Script_ccowUtils(arg0, 1);
      Script_srchControl((void *)task_7C[arg1]);
      Script_srchControl((void *)local_F8);
      Script_srchControl((void *)local_FC);
      Script_srchControl(0);
    }
  }
  end:;
}

static int32_t script_3705(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  int32_t local_F8;
  local_FC = Script_scenCreateTask(script_0AFA, 3, arg0, arg1, 0);
  local_F8 = Script_scenCreateTask(script_0666, 6, arg0, arg1, arg2, arg3, &data_44DC, script_1699);
  Script_charaDisplay(arg0, CDM_Show);
  Script_charaToggleMotEnabled(arg0, 1);
  Script_motmToggleFlags(arg0, 1, 0x40);
  Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_7);
  Script_SetCPAMFlags(arg0, 0);
  Script_SetShadowFlags(arg0, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_SetShadowFlags(arg0, SCRIPT_SHADOW_UPDATE_LIGHT_MASK, 1);
  Script_SetShadowFlags(arg0, SCRIPT_SHADOW_UPDATE_LIGHT_MASK, 5);
  for (;;) {
    SCNF_WaitFrames_0000(1);
    if (task_8 == 1) {
      Script_charaDisplay(arg0, CDM_Hide);
      Script_srchControl((void *)task_7C[arg1]);
      Script_srchControl((void *)local_F8);
      Script_srchControl((void *)local_FC);
      Script_srchControl(0);
    }
  }
  end:;
}

static int32_t script_37CA(int32_t arg0, int32_t arg1) {
  Script_ccowUtils(arg0, 2);
  Script_SetShadowFlags(arg0, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_SetShadowFlags(arg0, SCRIPT_SHADOW_UPDATE_LIGHT_MASK, 1);
  Script_SetShadowFlags(arg0, SCRIPT_SHADOW_UPDATE_LIGHT_MASK, 5);
  Script_scenToggleActiveFor(arg0, 0);
  for (;;) {
    SCNF_WaitFrames_0000(1);
    if (task_8 == 1) {
      Script_charaDisplay(arg0, CDM_Hide);
      Script_srchControl(0);
    }
  }
  end:;
}

static int32_t script_3822(int32_t arg0, int32_t arg1, int32_t arg2) {
  Vector3 local_F4;
  Vector3i local_E8;
  Script_charaDisplay(arg0, CDM_Show);
  Script_ccowUtils(arg0, 2);
  Script_SetShadowFlags(arg0, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_SetShadowFlags(arg0, SCRIPT_SHADOW_UPDATE_LIGHT_MASK, 1);
  Script_SetShadowFlags(arg0, SCRIPT_SHADOW_UPDATE_LIGHT_MASK, 5);
  Script_scenToggleActiveFor(arg0, 0);
  local_F4.x = 0.08f;
  (&local_F4)->y = -0.1f;
  (&local_F4)->z = 0.04f;
  local_E8.x = 0;
  (&local_E8)->y = 0;
  (&local_E8)->z = 0;
  Script_fixoAttachEx(arg0, arg2, 12, &local_F4, &local_E8);
  for (;;) {
    SCNF_WaitFrames_0000(1);
    if (task_8 == 1) {
      Script_charaDisplay(arg0, CDM_Hide);
      Script_srchControl(0);
    }
  }
  end:;
}

static int32_t script_38CA(int32_t arg0, int32_t arg1) {
  Script_SetShadowFlags(arg0, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_SetShadowFlags(arg0, SCRIPT_SHADOW_UPDATE_LIGHT_MASK, 1);
  Script_SetShadowFlags(arg0, SCRIPT_SHADOW_UPDATE_LIGHT_MASK, 5);
  Script_scenToggleActiveFor(arg0, 0);
  for (;;) {
    SCNF_WaitFrames_0000(1);
    if (task_8 == 1) {
      Script_charaDisplay(arg0, CDM_Hide);
      Script_srchControl(0);
    }
  }
  end:;
}
