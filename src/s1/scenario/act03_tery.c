#include "act03_tery.h"

static void script_0030(void);
static void script_0039(void);
static void script_0048(int32_t arg0);
static void script_00D5(int32_t arg0);
static void script_0125(void);
static int32_t script_0153(int32_t arg0);
static void script_0181(void);
static void script_01B5(int32_t arg0);
static void script_01E9(int32_t arg0);
static int32_t script_0207(int32_t arg0, int32_t arg1, int32_t arg2);
static int32_t script_0241(int32_t arg0, int32_t arg1);
static void script_0275(void);
static void script_036D(int32_t arg0, int32_t arg1);
static void script_039D(void);
static void script_0557(void);
static void script_0681(void);
static void script_06B2(void);
static void script_06E3(void);
static void script_07E9(void);
static void script_07F1(int32_t arg0);
static void script_081F(void);
static int32_t script_0901(void);
static int32_t script_0AB4(void);
static void script_0C17(void);
static void script_0CB8(void);
static void script_0D71(int32_t arg0);
static void script_0DE4(int32_t arg0);
static void script_0E0D(int32_t arg0);
static void script_0E30(int32_t arg0);
static void script_0E76(int32_t arg0, int32_t arg1);
static void script_1210(int32_t arg0, int32_t arg1);
static void script_191C(int32_t arg0, int32_t arg1);
static void script_1B81(void);
static void script_1DF1(void);
static void script_1F65(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4);
static int32_t script_1F7D(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4);
static void script_1FA4(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static int32_t script_1FBE(int32_t arg0, int32_t arg1, int32_t arg2, float arg3, int32_t arg4, int32_t arg5);
static void script_2046(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static int32_t script_2060(int32_t arg0, int32_t arg1, float arg2, float arg3, float arg4, int32_t arg5);
static void script_20EB(int32_t arg0, int32_t arg1);
static int32_t script_20FD(int32_t arg0, int32_t arg1);
static void script_2121(int32_t arg0, int32_t arg1, int32_t arg2);
static int32_t script_2135(int32_t arg0, float arg1, int32_t arg2);
static void script_217E(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_2194(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_21D7(int32_t arg0, int32_t arg1, int32_t arg2);
static int32_t script_21EB(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_221D(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_2233(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_227F(int32_t arg0, int32_t arg1);
static int32_t script_2291(int32_t arg0, int32_t arg1);
static void script_23C9(int32_t arg0);
static void script_243C(int32_t arg0);
static void script_2459(int32_t arg0);
static void script_2479(void);
static void script_2584(void);
static void script_2B20(void);
static void script_2CF1(void);
static void script_3129(void);
static void script_32BE(int32_t arg0);
static int32_t script_32CE(int32_t arg0);
static void script_3330(int32_t arg0);
static int32_t script_3340(int32_t arg0);
static void script_3383(int32_t arg0);
static int32_t script_3393(int32_t arg0);
static void script_33D6(int32_t arg0);
static int32_t script_33E6(int32_t arg0);
static void script_341B(void);
static void script_3D77(void);
void script_4596(void);
static int32_t script_462F(void);
static void script_49F5(void);
static void script_4BC4(void);
static void script_4E4D(void);
static void script_4E9B(int32_t arg0, int32_t arg1);
static int32_t script_4EB4(const char *arg0, const char *arg1);
static void script_4F38(void);
static void script_504E(void);
static void script_512D(void);

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
static int32_t task_70;
static int32_t task_74;
static int32_t task_78;
static int8_t task_7C;
static uint8_t task_7D;
static uint16_t task_7E;
static GameDateTime task_80;
static int32_t task_88;
typedef struct Row_51F0 { float w0; float w1; float w2; int32_t w3; int32_t w4; int32_t w5; int32_t w6; } Row_51F0;
static const Row_51F0 data_51F0[9] = {
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
static const int32_t data_52EC[5] = {0, 0, 0, 0, 0};
static const int32_t data_5300[684] = {
  0, 0, -10922, 0, 0, -10558, 0, 1456, 0, 0, 0, -18022, 0, 0, -17112, 0, 0, -18386, 0, 0, 0, 0, 0, -12196, 0, 0, -18204, 0, 0, -16748, 0, 0, 0, 0, 0,
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
  -1638, 0, 0, 1092, 0, 0, -5461, 0, -2366, -1092, 0, -182, 1092, 0, -546, -6735, 0, 0, -4004, 0, 0, -6553, 0, -182, 1274, 0, 0, -13289, 0, 0, -5279,
  0, 0, -8009, 0, 0, 1456, 0, 182, -8738, 0, 0, -6007, 0, 0, -8738, 0, 182, 1638, 0, -728, -5279, 0, -1274, -6735, 0, -910, -8738, 0, 0, 1638, 0, 0,
  -9102, 0, -2366, -2002, 0, -182, 910, 0, -546, -2366, 0, 0, -2002, 0, 0, -2912, 0, -182, 1274, 0, 0, -14199, 0, 0, -11468, 0, 0, -13653, 0, 0, 1456,
  0, 182, -11286, 0, 0, -12014, 0, 0, -12743, 0, 182, 1638, 0, -728, -10194, 0, -1274, -12196, 0, -910, -10922, -728, 546, 1638, 0, 0, -1820, 0,
  -2366, -728, 0, -364, -1092, 0, -546, -11832, 0, 910, -6735, 0, 0, -6553, 0, -182, 1274, 0, 0, -13289, 0, 0, -6007, 0, 0, -10012, 0, 0, 1092, 0,
  182, -8738, 0, 0, -6735, 0, 0, -9830, 0, 182, 1638, 0, -728, -6553, 0, -1274, -7281, 0, -910, -10740, 0, 0, 1638, 0, 0, -5461, -2912, -2366, -364,
  -5279, 2548, -910, 0, -546, -8374, 0, 0, -9466, 0, 910, -4915, 0, -182, 546, 0, 0, -13289, 0, 0, -5279, 0, 364, -7645, 0, -182, 1456, 0, 182, -8738,
  0, 0, -7281, 0, 182, -6917, 0, -182, 1638, 0, -728, -5279, 0, -1274, -6735, 0, -182, -8009, 0, 0, 1638, -4733, 0, -8556, 0, -2366, -1092, -6189,
  2912, 0, 0, -546, -10922, 0, 1820, -16384, 0, 0, -11468, 0, 182, 546, 0, 0, -10376, 0, 0, -15837, 0, 0, -12743, 0, -182, 728, 0, 182, -13835, 0, 0,
  -16201, 0, 364, -10922, 0, -182, 728, 0, -728, -9648, 0, -3458, -12379, 0, -728, -7463, 0, 0, 1092, 0, 0, -5461, 0, -2366, -1638, 0, -910, 1092, 0,
  -546, -3640, 0, 0, -2366, 0, 0, -1456, 0, -182, 1638, 0, 0, -910, 0, 0, -2730, 0, 0, -1274, 0, 0, 1820, 0, 182, -2730, 0, 0, -1820, 0, 0, -1274, 0,
  182, 2184, 0, -728, -3094, 0, -910, -2548, 0, 910, -1456, -546, 910, 2366
};
static const uint32_t data_5DB0 = 0x41A00000;
static const float data_5DB4[2] = {0.0f, -50.0f};
static const uint32_t data_5DBC = 0x41A00000;
static const float data_5DC0[2] = {0.0f, -50.0f};
static const uint32_t data_5DC8 = 0xC2200000;
static const float data_5DCC[2] = {0.0f, 80.0f};
static const uint32_t data_5DD4 = 0xC2D60000;
static const float data_5DD8[2] = {0.0f, 78.0f};
static const uint8_t data_5DE0[8] = {0};
static const float data_5DE8 = 90.0f;
static const uint32_t data_5DEC = 0xC1A00000;
static const float data_5DF0[2] = {0.0f, -50.0f};
static const uint32_t data_5DF8 = 0xC1A00000;
static const float data_5DFC[2] = {0.0f, -50.0f};
static const uint32_t data_5E04 = 0xC2E60000;
static const float data_5E08[2] = {0.0f, 175.0f};
static const uint32_t data_5E10 = 0xC2DC0000;
static const float data_5E14[2] = {0.0f, 10.0f};
static const uint32_t data_5E1C = 0x42C80000;
static const float data_5E20[2] = {0.0f, -40.0f};
static const uint32_t data_5E28 = 0x43020000;
static const float data_5E2C[2] = {0.0f, 25.0f};
static const uint8_t data_5E34[12] = {0};
static const uint32_t data_5E40 = 0x3F800000;
static const float data_5E44[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint32_t data_5EC8 = 0x3F800000;
static const float data_5ECC[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint8_t data_5F50[16] = {0};
static const float data_5F60[30] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint32_t data_5FD8 = 0x3F800000;
static const float data_5FDC[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint32_t data_6060 = 0x3F800000;
static const float data_6064[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint8_t data_60E8[84] = {0};
typedef struct Row_613C { float w0; float w1; float w2; int32_t w3; } Row_613C;
static const Row_613C data_613C[3] = {
  {0.787f, 0.787f, 0.787f, 0},
  {0.0f, 0.0f, 0.0f, 0},
  {0.0f, 0.0f, 0.0f, 0}
};
static const int32_t data_616C = 0;
static const uint32_t data_6170 = 0x3F800000;
static const float data_6174[33] = {
  0.944f, 0.866f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f,
  0.787f, 0.787f, 0.787f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint32_t data_61F8 = 0x3F4978D5;
static const float data_61FC[33] = {
  0.787f, 0.866f, 0.944f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  0.944f, 0.866f, 0.787f, 0.787f, 0.787f, 0.787f, 0.708f, 0.708f, 0.708f, 0.708f
};
static const uint8_t data_6280[12] = {0};
static const float data_628C[31] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint32_t data_6308 = 0x3F800000;
static const float data_630C[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint32_t data_6390 = 0x3F800000;
static const float data_6394[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint32_t data_6418 = 0x3F800000;
static const float data_641C[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint32_t data_64A0 = 0x41A00000;
static const float data_64A4[2] = {0.0f, -50.0f};
static const uint32_t data_64AC = 0x41A00000;
static const float data_64B0[2] = {0.0f, -50.0f};
static const uint32_t data_64B8 = 0xC2200000;
static const float data_64BC[2] = {0.0f, 80.0f};
static const uint32_t data_64C4 = 0xC1A00000;
static const float data_64C8[2] = {0.0f, -50.0f};
static const uint32_t data_64D0 = 0xC1A00000;
static const float data_64D4[2] = {0.0f, -50.0f};
static const uint32_t data_64DC = 0xC2E60000;
static const float data_64E0[2] = {0.0f, 175.0f};
static const uint32_t data_64E8 = 0x42C80000;
static const float data_64EC[2] = {0.0f, -40.0f};
static const uint8_t data_64F4[12] = {0};
static const uint32_t data_6500 = 0x3F800000;
static const float data_6504[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint32_t data_6588 = 0x3F800000;
static const float data_658C[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint8_t data_6610[16] = {0};
static const float data_6620[30] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint8_t data_6698[84] = {0};
typedef struct Row_66EC { float w0; float w1; float w2; int32_t w3; } Row_66EC;
static const Row_66EC data_66EC[3] = {
  {0.787f, 0.787f, 0.787f, 0},
  {0.0f, 0.0f, 0.0f, 0},
  {0.0f, 0.0f, 0.0f, 0}
};
static const int32_t data_671C = 0;
static const uint32_t data_6720 = 0x3F800000;
static const float data_6724[33] = {
  0.944f, 0.866f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f,
  0.787f, 0.787f, 0.787f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint32_t data_67A8 = 0x3F4978D5;
static const float data_67AC[33] = {
  0.787f, 0.866f, 0.944f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  0.944f, 0.866f, 0.787f, 0.787f, 0.787f, 0.787f, 0.708f, 0.708f, 0.708f, 0.708f
};
static const uint32_t data_6830 = 0x3F800000;
static const float data_6834[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint32_t data_68B8 = 0x3F800000;
static const float data_68BC[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint32_t data_6940 = 0xC2A50000;
static const float data_6944[2] = {0.0f, 68.0f};
static const uint32_t data_694C = 0x3F800000;
static const float data_6950[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint32_t data_69D4 = 0xC1D00000;
static const float data_69D8[2] = {0.0f, 76.5f};
static const uint32_t data_69E0 = 0x3F800000;
static const float data_69E4[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint32_t data_6A68 = 0x41A00000;
static const float data_6A6C[2] = {0.0f, -50.0f};
static const uint32_t data_6A74 = 0x41A00000;
static const float data_6A78[2] = {0.0f, -50.0f};
static const uint32_t data_6A80 = 0xC2200000;
static const float data_6A84[2] = {0.0f, 80.0f};
static const uint32_t data_6A8C = 0xC1A00000;
static const float data_6A90[2] = {0.0f, -50.0f};
static const uint32_t data_6A98 = 0xC1A00000;
static const float data_6A9C[2] = {0.0f, -50.0f};
static const uint32_t data_6AA4 = 0xC2E60000;
static const float data_6AA8[5] = {0.0f, 175.0f, 100.0f, 0.0f, -40.0f};
static const uint8_t data_6ABC[12] = {0};
static const uint32_t data_6AC8 = 0x3F800000;
static const float data_6ACC[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint32_t data_6B50 = 0x3F800000;
static const float data_6B54[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint8_t data_6BD8[16] = {0};
static const float data_6BE8[30] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint8_t data_6C60[84] = {0};
typedef struct Row_6CB4 { float w0; float w1; float w2; int32_t w3; } Row_6CB4;
static const Row_6CB4 data_6CB4[3] = {
  {0.787f, 0.787f, 0.787f, 0},
  {0.0f, 0.0f, 0.0f, 0},
  {0.0f, 0.0f, 0.0f, 0}
};
static const int32_t data_6CE4 = 0;
static const uint32_t data_6CE8 = 0x3F800000;
static const float data_6CEC[33] = {
  0.944f, 0.866f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f,
  0.787f, 0.787f, 0.787f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint32_t data_6D70 = 0x3F4978D5;
static const float data_6D74[33] = {
  0.787f, 0.866f, 0.944f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  0.944f, 0.866f, 0.787f, 0.787f, 0.787f, 0.787f, 0.708f, 0.708f, 0.708f, 0.708f
};
static const uint32_t data_6DF8 = 0x3F800000;
static const float data_6DFC[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const int32_t data_6E80[57] = {
  0, 0, -6189, 0, 1638, -3822, 0, 546, -546, 0, -1820, -12014, 0, 0, -16384, 0, 182, -12743, 0, 0, -1274, 0, 546, -10558, 0, 0, -14017, 0, 0, -16201,
  0, 0, -1092, 0, 728, -9830, 0, 0, -14199, 0, 0, -15291, 0, 0, -1092, 0, -1638, -8009, 0, 0, -12014, 0, -3458, -13653, 0, -364, -910
};
static const int32_t data_6F64[57] = {
  0, 0, -6189, 0, 1638, -3822, 0, 546, -546, 0, -1820, -11832, 0, 0, -16384, 0, 364, -16384, 0, -182, -546, 0, 546, -7827, 0, 0, -13471, 0, 0, -18204,
  0, 0, -182, 0, 728, -8738, 0, 0, -14199, 0, 0, -17112, 0, 0, -546, 0, -364, -8009, 0, 0, -12561, 0, -3458, -15109, 0, -364, -364
};
static const Vector4 data_7048 = {.x = 0.3f, .y = 0.3f, .z = 0.3f, .w = 0.3f};
static const Vector4 data_7058 = {.x = 1.0f, .y = 0.2f, .z = 0.2f, .w = 0.2f};
static const float data_7068[4] = {1.0f, 0.1f, 0.1f, 0.1f};
static const char data_7078[10] __attribute__((scn_string_literal)) = "E1077A001";
static const char data_7082[10] __attribute__((scn_string_literal)) = "E1077A002";
static const char data_708C[10] __attribute__((scn_string_literal)) = "E1077A003";
static const char data_7096[10] __attribute__((scn_string_literal)) = "E1077B001";
static const char data_70A0[10] __attribute__((scn_string_literal)) = "E1077B002";
static const char data_70AA[10] __attribute__((scn_string_literal)) = "E1077B003";
static const char data_70B4[10] __attribute__((scn_string_literal)) = "E1077C001";
static const char data_70BE[10] __attribute__((scn_string_literal)) = "E1077C002";
static const char data_70C8[10] __attribute__((scn_string_literal)) = "E1077D001";
static const char data_70D2[10] __attribute__((scn_string_literal)) = "E1077D002";
static const char data_70DC[10] __attribute__((scn_string_literal)) = "E1077D003";
static const char data_70E6[10] __attribute__((scn_string_literal)) = "E1077E001";
static const char data_70F0[12] __attribute__((scn_string_literal)) = "E1077F001";
static const Vector3 data_70FC = {.x = 1000.0f, .y = 0.0f, .z = 1000.0f};
static const Vector4 data_7108 = {.x = 0.6f, .y = 0.6f, .z = 0.6f, .w = 0.6f};
static const char data_7118[] __attribute__((scn_string_literal)) = "scene/03/TERY";
static const char data_7126[] __attribute__((scn_string_literal)) = "TERYB1";
static const char data_712D[11] __attribute__((scn_string_literal)) = "bgm013.snd";
static const char data_7138[13] __attribute__((scn_string_literal)) = "e1terryt.snd";
static const char data_7145[7] __attribute__((scn_string_literal)) = "TERYB1";
static const char data_714C[] __attribute__((scn_string_literal)) = "scene/03/TERY";
static const char data_715A[] __attribute__((scn_string_literal)) = "TERYB2";
static const uint8_t data_7161[3] = {0};
static const Vector4 data_7164 = {.x = 0.3f, .y = 0.06f, .z = 1.40129846e-45f, .w = 0.3f};
static const char data_7174[8] __attribute__((scn_string_literal)) = "TERYB2";
static const uint32_t data_717C = 0x3F0A3D71;
static const float data_7180[11] = {0.61f, 0.42f, 3.5f, -0.12f, 0.33f, 0.06f, 1.14f, 4.81f, -4.46f, 7.02f, 0.8f};
static const int32_t data_71AC = 3058;
static const uint32_t data_71B0 = 0x3F0A3D71;
static const float data_71B4[11] = {0.61f, 0.42f, 3.5f, -0.22f, 1.0f, -1.0f, 3.07f, 4.24f, -6.9f, 7.58f, 0.98f};
static const int32_t data_71E0 = 4150;
static const uint32_t data_71E4 = 0x3F0A3D71;
static const float data_71E8[11] = {0.61f, 0.42f, 3.5f, -0.64f, 0.63f, 0.43f, 3.18f, 4.83f, -4.27f, 7.17f, 0.8f};
static const int32_t data_7214 = 3058;
static const uint32_t data_7218 = 0x3F0A3D71;
static const float data_721C[11] = {0.61f, 0.42f, 3.5f, 0.38f, 0.77f, 0.39f, 6.17f, 5.29f, -2.9f, 7.02f, 0.8f};
static const int32_t data_7248 = 3058;
static const char data_724C[13] __attribute__((scn_string_literal)) = "battle_1.snd";
static const char data_7259[13] __attribute__((scn_string_literal)) = "battle_1.snd";
static const char data_7266[] __attribute__((scn_string_literal)) = "scene/03/TERY";
static const char data_7274[] __attribute__((scn_string_literal)) = "BIKE";
static const char data_7279[6] __attribute__((scn_string_literal)) = "MPK00";
static const char data_727F[6] __attribute__((scn_string_literal)) = "MPK00";
static const char data_7285[6] __attribute__((scn_string_literal)) = "E1077";
static const char data_728B[11] __attribute__((scn_string_literal)) = "M_FGT1.BIN";
static const char data_7296[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_729B[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_72A0[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_72A5[13] __attribute__((scn_string_literal)) = "e1terryt.snd";
static const char data_72B2[5] __attribute__((scn_string_literal)) = "BIKE";
static const char data_72B7[6] __attribute__((scn_string_literal)) = "E1077";
static const char data_72BD[5] __attribute__((scn_string_literal)) = "BIKE";
static const char data_72C2[5] __attribute__((scn_string_literal)) = "BIKE";
static const char data_72C7[13] __attribute__((scn_string_literal)) = "seqdata2.bin";
static const char data_72D4[5] __attribute__((scn_string_literal)) = "BIKE";
static const char data_72D9[13] __attribute__((scn_string_literal)) = "seqdata3.bin";
static const char data_72E6[5] __attribute__((scn_string_literal)) = "BIKE";
static const char data_72EB[13] __attribute__((scn_string_literal)) = "seqdata4.bin";
static const char data_72F8[5] __attribute__((scn_string_literal)) = "BIKE";
static const char data_72FD[13] __attribute__((scn_string_literal)) = "seqdata5.bin";
static const char data_730A[5] __attribute__((scn_string_literal)) = "BIKE";
static const char data_730F[13] __attribute__((scn_string_literal)) = "seqdata6.bin";
static const char data_731C[5] __attribute__((scn_string_literal)) = "BIKE";
static const char data_7321[13] __attribute__((scn_string_literal)) = "seqdata7.bin";
static const char data_732E[5] __attribute__((scn_string_literal)) = "BIKE";
static const char data_7333[13] __attribute__((scn_string_literal)) = "seqdata8.bin";
static const char data_7340[5] __attribute__((scn_string_literal)) = "BIKE";
static const char data_7345[13] __attribute__((scn_string_literal)) = "seqdata9.bin";
static const char data_7352[5] __attribute__((scn_string_literal)) = "BIKE";
static const char data_7357[13] __attribute__((scn_string_literal)) = "seqdataA.bin";
static const char data_7364[5] __attribute__((scn_string_literal)) = "BIKE";
static const char data_7369[13] __attribute__((scn_string_literal)) = "seqdataB.bin";
static const char data_7376[5] __attribute__((scn_string_literal)) = "BIKE";
static const char data_737B[6] __attribute__((scn_string_literal)) = "YKG_M";
static const char data_7381[7] __attribute__((scn_string_literal)) = "YKB_M";
static const uint32_t data_7388[4] = {0, 0, 0x64996FA5U, 0x64996FA5U};
static const FogColourData data_7398 = {.red = 0.1f, .green = 0.0f, .blue = 0.25f, .colour = 0xFF392C63U};
static const char data_73A8[] __attribute__((scn_string_literal)) = "scene/03/TERY";
static const char data_73B6[] __attribute__((scn_string_literal)) = "TER2";
static const char data_73BB[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_73C0[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_73C5[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_73CA[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_73CF[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_73D4[11] __attribute__((scn_string_literal)) = "bgm084.snd";
static const char data_73DF[13] __attribute__((scn_string_literal)) = "a1_terry.snd";
static const char data_73EC[6] __attribute__((scn_string_literal)) = "01TAP";
static const char data_73F2[] __attribute__((scn_string_literal)) = "sprite";
static const char data_73F9[13] __attribute__((scn_string_literal)) = "SCROLL25.SPR";
static const char data_7406[5] __attribute__((scn_string_literal)) = "TER2";
static const char data_740B[9] __attribute__((scn_string_literal)) = "seqdataC";
static const char data_7414[5] __attribute__((scn_string_literal)) = "TER2";
static const char data_7419[9] __attribute__((scn_string_literal)) = "seqdataD";
static const char data_7422[5] __attribute__((scn_string_literal)) = "TER2";
static const char data_7427[9] __attribute__((scn_string_literal)) = "seqdataE";
static const char data_7430[5] __attribute__((scn_string_literal)) = "TER2";
static const char data_7435[5] __attribute__((scn_string_literal)) = "TER2";
static const char data_743A[6] __attribute__((scn_string_literal)) = "01TAP";
static const char data_7440[28] __attribute__((scn_string_literal)) = " Load PackFile Start %s:%s\012";
static const char data_745C[28] __attribute__((scn_string_literal)) = "Load PackFile Failed %s:%s\012";

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

static int32_t script_0207(int32_t arg0, int32_t arg1, int32_t arg2) {
  int32_t local_FC;
  Script_GetDataFromFightProgram(arg0, arg1);
  local_FC = 0;
  while (local_FC < arg2) {
    local_FC = local_FC + 1;
    Script_SetFightProgramStateBytes(arg0, local_FC, 1);
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_0241(int32_t arg0, int32_t arg1) {
  while (arg1 > 0) {
    Script_SetFightProgramStateBytes(arg0, arg1, 1);
    arg1 = arg1 - 1;
    SCNF_WaitFrames_0000(1);
  }
  Script_GetDataFromFightProgram(arg0, 0);
  end:;
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
  Script_ambSoundUtils(1, 1, &data_5DB0, 135.0f, 280.0f, 1.0f, 0, &data_5E40);
  Script_ambSoundUtils(2, 1, &data_5DBC, 135.0f, 60.0f, 1.0f, 0, &data_5EC8);
  Script_ambSoundUtils(3, 0, &data_5DC8, 50.0f, 170.0f, 0.8f, 0, data_5F50);
  Script_ambSoundUtils(4, 0, &data_5DD4, 10.0f, 5.0f, 1.0f, 0, &data_5FD8);
  Script_ambSoundUtils(5, 0, data_5DE0, 8.0f, 5.0f, 1.0f, 0, &data_6060);
  Script_ambSoundUtils(6, 1, &data_5DEC, 170.0f, 100.0f, 1.0f, 2, data_60E8);
  Script_ambSoundUtils(7, 1, &data_5DF8, 170.0f, 100.0f, 1.0f, 0, &data_6170);
  Script_ambSoundUtils(8, 1, &data_5E04, 40.0f, 70.0f, 0.8f, 0, &data_61F8);
  Script_ambSoundUtils(9, 0, &data_5E10, 3.0f, 60.0f, 0.8f, 0, data_6280);
  Script_ambSoundUtils(10, 0, &data_5E1C, 60.0f, 35.0f, 0.7f, 0, &data_6308);
  Script_ambSoundUtils(13, 0, &data_5E28, 8.0f, 5.0f, 1.0f, 0, &data_6390);
  Script_ambSoundUtils(15, 1, data_5E34, 300.0f, 0.0f, 1.0f, 3, &data_6418);
  Script_Ambient1SoundUtils(0, 23976);
  return;
}

static void script_0557(void) {
  Script_ambSoundUtils(1, 1, &data_64A0, 135.0f, 280.0f, 1.0f, 0, &data_6500);
  Script_ambSoundUtils(2, 1, &data_64AC, 135.0f, 60.0f, 1.0f, 0, &data_6588);
  Script_ambSoundUtils(3, 0, &data_64B8, 50.0f, 170.0f, 0.8f, 0, data_6610);
  Script_ambSoundUtils(6, 1, &data_64C4, 170.0f, 100.0f, 1.0f, 2, data_6698);
  Script_ambSoundUtils(7, 1, &data_64D0, 170.0f, 100.0f, 1.0f, 0, &data_6720);
  Script_ambSoundUtils(8, 1, &data_64DC, 40.0f, 70.0f, 0.8f, 0, &data_67A8);
  Script_ambSoundUtils(10, 0, &data_64E8, 60.0f, 35.0f, 0.7f, 0, &data_6830);
  Script_ambSoundUtils(15, 1, data_64F4, 300.0f, 0.0f, 1.0f, 3, &data_68B8);
  Script_Ambient1SoundUtils(0, 23976);
  return;
}

static void script_0681(void) {
  Script_ambSoundUtils(1, 0, &data_6940, 1.0f, 10.0f, 1.0f, 0, &data_694C);
  Script_Ambient1SoundUtils(0, 23976);
  return;
}

static void script_06B2(void) {
  Script_ambSoundUtils(1, 0, &data_69D4, 1.0f, 10.0f, 1.0f, 0, &data_69E0);
  Script_Ambient1SoundUtils(0, 23976);
  return;
}

static void script_06E3(void) {
  Script_ambSoundUtils(1, 1, &data_6A68, 135.0f, 280.0f, 1.0f, 0, &data_6AC8);
  Script_ambSoundUtils(2, 1, &data_6A74, 135.0f, 60.0f, 1.0f, 0, &data_6B50);
  Script_ambSoundUtils(3, 0, &data_6A80, 50.0f, 170.0f, 0.8f, 0, data_6BD8);
  Script_ambSoundUtils(6, 1, &data_6A8C, 170.0f, 100.0f, 1.0f, 2, data_6C60);
  Script_ambSoundUtils(7, 1, &data_6A98, 170.0f, 100.0f, 1.0f, 0, &data_6CE8);
  Script_ambSoundUtils(8, 1, &data_6AA4, 40.0f, 70.0f, 0.8f, 0, &data_6D70);
  Script_ambSoundUtils(15, 1, data_6ABC, 300.0f, 0.0f, 1.0f, 3, &data_6DF8);
  Script_Ambient1SoundUtils(0, 23976);
  return;
}

static void script_07E9(void) {
  script_039D();
  return;
}

static void script_07F1(int32_t arg0) {
  if (Script_shenSeqUtils(-7) != 0) {
  } else {
    Script_shenSeqUtils(arg0);
    while (Script_shenSeqUtils(-1)) {
      SCNF_WaitFrames_0000(1);
    }
  }
  return;
}

static void script_081F(void) {
  int32_t local_FC;
  int32_t local_F8;
  script_0C17();
  local_FC = Script_scenCreateTask(script_0901, 0);
  local_F8 = Script_scenCreateTask(script_0AB4, 0);
  Script_filmCheckOrSetTransitionUtil(60, 255, 0, 0, 0, 0, 0, 0, 0);
  script_07F1(0);
  script_07F1(1);
  script_07F1(2);
  script_07F1(3);
  script_07F1(4);
  script_07F1(5);
  script_07F1(6);
  script_07F1(7);
  script_07F1(8);
  script_07F1(9);
  Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_executeAicaCommand(196768, 0, 0);
  script_0CB8();
  Script_srchControl((void *)local_FC);
  Script_srchControl((void *)local_F8);
  return;
}

static int32_t script_0901(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t unused_E4;
  int32_t local_E0;
  Script_FaceEx(CHARA_AKIR, 0, 0, 1);
  Script_handSetFloatsFor(CHARA_AKIR, 1, data_6E80, 1);
  Script_handSetFloatsFor(CHARA_AKIR, 0, data_6F64, 1);
  local_E8.x = -1274;
  (&local_E8)->y = -2184;
  (&local_E8)->z = 0;
  Script_SetHandRootRotation(CHARA_AKIR, 1, 0x15, &local_E8);
  local_E8.x = -546;
  (&local_E8)->y = 1638;
  (&local_E8)->z = -1638;
  Script_SetHandRootRotation(CHARA_AKIR, 0, 0x15, &local_E8);
  Script_SetLampState(0, SHEN_NAME('BIKE'), 1);
  local_E0 = 0;
  for (;;) {
    if (local_E0 == 1) {
      local_F4.x = 0.023f;
      (&local_F4)->y = 0.002f;
      (&local_F4)->z = 0.15f;
      local_E8.x = 17019;
      (&local_E8)->y = 94;
      (&local_E8)->z = 48610;
      Script_fixoAttachEx(SHEN_NAME('HELM'), CHARA_AKIR, 5, &local_F4, &local_E8);
    }
    if (local_E0 == 485) {
      Script_SetLampState(0, SHEN_NAME('BIKE'), 0);
    }
    if (local_E0 == 490) {
      Script_SetLampState(0, SHEN_NAME('BIKE'), 1);
    }
    if (local_E0 == 827) {
      Script_SetLampState(0, SHEN_NAME('BIKE'), 0);
    }
    if (local_E0 == 832) {
      Script_SetLampState(0, SHEN_NAME('BIKE'), 1);
    }
    if (local_E0 == 903) {
      Script_SetLampState(0, SHEN_NAME('BIKE'), 0);
    }
    if (local_E0 == 929) {
      Script_SetLampState(0, SHEN_NAME('BIKE'), 1);
    }
    if (local_E0 == 1066) {
      Script_executeAicaCommand(2720, 2, 100);
    }
    local_E0 = local_E0 + 1;
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_0AB4(void) {
  Vector3 local_F4;
  Vector3 local_E8;
  float local_DC[3];
  float local_D8;
  float local_D4;
  Vector3i local_C8;
  local_E8.x = 0.0f;
  (&local_E8)->y = 0.0f;
  (&local_E8)->z = 0.0f;
  for (;;) {
    local_F4.x = (&local_E8)->x;
    (&local_F4)->y = (&local_E8)->y;
    (&local_F4)->z = (&local_E8)->z;
    Script_EVT_GetPosition(SHEN_NAME('BIKE'), -1, &local_E8, 0);
    local_DC[0] = (&local_E8)->x - (&local_F4)->x;
    local_DC[1] = (&local_E8)->y - (&local_F4)->y;
    local_DC[2] = (&local_E8)->z - (&local_F4)->z;
    local_DC[0] = *local_DC * *local_DC;
    local_DC[1] = local_DC[1] * local_DC[1];
    local_DC[2] = local_DC[2] * local_DC[2];
    local_D8 = *local_DC * *local_DC + local_DC[1] * local_DC[1] + local_DC[2] * local_DC[2];
    local_D4 = local_D8 * 1000000.0f;
    if (!(local_D4 <= 300.0f)) {
      local_D4 = 300.0f;
    }
    if (!(local_D4 >= 0.5f)) {
      local_D4 = 0.0f;
    }
    local_C8.x = -(int32_t)(local_D4 / 360.0f * (float)0x10000);
    (&local_C8)->y = 0;
    (&local_C8)->z = 0;
    Script_charaPosRotUtils(SHEN_NAME('BIKE'), 23, 0, &local_C8, 1);
    Script_charaPosRotUtils(SHEN_NAME('BIKE'), 18, 0, &local_C8, 1);
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_0C17(void) {
  script_0D71(CHARA_AKIR);
  script_0DE4(CHARA_HTNK);
  script_0DE4(CHARA_TDSI);
  Script_scenToggleVisibleFor(CHARA_HTNK, 1);
  Script_scenToggleVisibleFor(CHARA_TDSI, 1);
  Script_charaDisplay(CHARA_HTNK, CDM_Show);
  Script_charaDisplay(CHARA_TDSI, CDM_Show);
  Script_charaDisplay(SHEN_NAME('BIKE'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('HIKA'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('SUJI'), CDM_Show);
  Script_ccowSetFlag80(SHEN_NAME('HIKA'), 1);
  Script_ccowSetFlag80(SHEN_NAME('HELM'), 1);
  Script_charaDisplay(SHEN_NAME('HELM'), CDM_Show);
  Script_SetVector4ByOffset(&data_7048);
  return;
}

static void script_0CB8(void) {
  script_0E30(CHARA_AKIR);
  if (-1) {
    Script_charaDisplay(CHARA_HTNK, CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(CHARA_HTNK, CDM_Show);
      goto L000CEE;
    }
  }
  L000CEE:;
  Script_scenToggleVisibleFor(CHARA_HTNK, 0);
  if (-1) {
    Script_charaDisplay(CHARA_TDSI, CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(CHARA_TDSI, CDM_Show);
      goto L000D22;
    }
  }
  L000D22:;
  Script_scenToggleVisibleFor(CHARA_TDSI, 0);
  Script_charaDisplay(CHARA_HTNK, CDM_Hide);
  Script_charaDisplay(CHARA_TDSI, CDM_Hide);
  Script_charaDisplay(SHEN_NAME('BIKE'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('HIKA'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('SUJI'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('HELM'), CDM_Hide);
  return;
}

static void script_0D71(int32_t arg0) {
  Script_ccowUtils(arg0, 2);
  Script_ccowSetFlag80(arg0, 1);
  Script_motmGetCharaFlags(arg0, 0, 0);
  Script_FaceEx(arg0, 1, 0, 1);
  Script_ResetHandMotion(arg0, 1, 1, 0);
  Script_ResetHandMotion(arg0, 1, 0, 0);
  Script_handSetFloatsFor(arg0, 0, data_5300, 10);
  Script_handSetFloatsFor(arg0, 1, data_5300, 10);
  Script_SetCPAMFlags(arg0, 0);
  Script_FaceLookAtTargetUtils(arg0, 0);
  return;
}

static void script_0DE4(int32_t arg0) {
  Script_ccowUtils(arg0, 2);
  Script_ccowSetFlag80(arg0, 1);
  Script_CharHandlers(arg0, 2, 8, 16);
  Script_SetCPAMFlags(arg0, 0);
  return;
}

static void script_0E0D(int32_t arg0) {
  Script_ccowUtils(arg0, 1);
  Script_SetCPAMFlags(arg0, 1);
  Script_SetCharaLookAtPosition(arg0, -98310, 0, 0);
  return;
}

static void script_0E30(int32_t arg0) {
  Script_ccowUtils(arg0, 1);
  Script_SetCPAMFlags(arg0, 1);
  Script_FaceLookAtTargetUtils(arg0, 0);
  Script_SetCharaLookAtPosition(arg0, -98310, 0, 0);
  Script_ResetHandMotion(arg0, 0, 1, 0);
  Script_ResetHandMotion(arg0, 0, 0, 0);
  return;
}

static void script_0E76(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  int32_t unused_D4[10];
  Vector4 local_C4;
  int32_t local_C0;
  script_0030();
  script_1B81();
  if (arg0) {
    Script_scenCreateTask(script_0153, 1, 60);
  }
  Script_shenSeqUtils(arg1);
  local_FC = 0;
  while (Script_shenSeqUtils(-1)) {
    local_C0 = local_FC;
    switch (local_C0) {
      case 0:
        Script_executeAicaCommand(16296, 0, 0);
        Script_executeAicaCommand(1184, 2, 90);
        local_C4.x = 0.0f;
        (&local_C4)->y = -0.2f;
        (&local_C4)->z = -0.2f;
        (&local_C4)->w = -0.2f;
        Script_SetCharaRefBoardPos(CHARA_HRSK, &local_C4);
        if (-1) {
          Script_charaDisplay(CHARA_CHAI, CDM_Hide);
        } else {
          if (0) {
            Script_charaDisplay(CHARA_CHAI, CDM_Show);
            goto L000F2D;
          }
        }
        L000F2D:;
        Script_scenToggleVisibleFor(CHARA_CHAI, 0);
        break;
      case 640:
        Script_FaceEx(CHARA_AKIR, 0, 0, 1);
        Script_FaceEx(CHARA_TERY, 0, 0, 1);
        Script_FaceEx(CHARA_HRSK, 0, 0, 1);
        Script_charaDisplay(CHARA_HRSK, CDM_Hide);
        break;
      case 795:
        Script_FaceEx(CHARA_HRSK, 1, 0, 1);
        Script_charaDisplay(CHARA_HRSK, CDM_Show);
        break;
      case 850:
        Script_FaceEx(CHARA_HRSK, 0, 0, 1);
        Script_charaDisplay(CHARA_STSI, CDM_Hide);
        Script_clmdSetFlags(CHARA_MRSK, 2, 1);
        Script_clmdSetFlags(CHARA_STSI, 2, 1);
        Script_clmdSetFlags(CHARA_PEDR, 2, 1);
        Script_clmdSetFlags(CHARA_AKIR, 2, 1);
        Script_clmdSetFlags(CHARA_TERY, 2, 1);
        break;
      case 901:
        Script_FaceEx(CHARA_AKIR, 1, 0, 1);
        Script_clmdSetFlags(CHARA_AKIR, 2, 0);
        break;
      case 964:
        Script_FaceEx(CHARA_AKIR, 0, 0, 1);
        Script_charaDisplay(CHARA_STSI, CDM_Show);
        Script_clmdSetFlags(CHARA_AKIR, 2, 1);
        Script_clmdSetFlags(CHARA_HRSK, 2, 0);
        Script_clmdSetFlags(CHARA_TERY, 2, 1);
        Script_clmdSetFlags(CHARA_PEDR, 2, 1);
        break;
      case 1049:
        Script_FaceEx(CHARA_AKIR, 1, 0, 1);
        Script_FaceEx(CHARA_HRSK, 1, 0, 1);
        Script_clmdSetFlags(CHARA_AKIR, 2, 0);
        Script_clmdSetFlags(CHARA_TERY, 2, 0);
        break;
      case 1320:
        Script_FaceEx(CHARA_AKIR, 0, 0, 1);
        Script_FaceEx(CHARA_HRSK, 0, 0, 1);
        Script_clmdSetFlags(CHARA_AKIR, 2, 1);
        Script_clmdSetFlags(CHARA_HRSK, 2, 1);
        Script_clmdSetFlags(CHARA_TERY, 2, 1);
        break;
      case 1399:
        Script_FaceEx(CHARA_AKIR, 1, 0, 1);
        Script_FaceEx(CHARA_HRSK, 1, 0, 1);
        Script_clmdSetFlags(CHARA_AKIR, 2, 0);
        Script_clmdSetFlags(CHARA_HRSK, 2, 0);
        break;
      case 1479:
        Script_FaceEx(CHARA_TERY, 1, 0, 1);
        Script_FaceEx(CHARA_AKIR, 0, 0, 1);
        Script_FaceEx(CHARA_HRSK, 0, 0, 1);
        Script_clmdSetFlags(CHARA_TERY, 2, 0);
        break;
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
  if (Script_shenSeqUtils(-7) == 0) {
    script_1DF1();
  }
  return;
}

static void script_1210(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  int32_t unused_F8;
  Vector3 local_EC;
  int32_t unused_D4[6];
  Vector4 local_C4;
  int32_t local_C0;
  if (Script_shenSeqUtils(-7) != 0) {
  } else {
    script_0030();
    script_1B81();
    if (arg0) {
      Script_scenCreateTask(script_0153, 1, 60);
    }
    Script_shenSeqUtils(arg1);
    local_FC = 0;
    while (Script_shenSeqUtils(-1)) {
      local_C0 = local_FC;
      switch (local_C0) {
        case 0:
          local_C4.x = 0.0f;
          (&local_C4)->y = 0.1f;
          (&local_C4)->z = 0.1f;
          (&local_C4)->w = 0.1f;
          Script_SetCharaRefBoardPos(CHARA_HRSK, &local_C4);
          Script_charaSetRefboardFlagTo(CHARA_HRSK, 1);
          if (-1) {
            Script_charaDisplay(CHARA_CHAI, CDM_Hide);
          } else {
            if (0) {
              Script_charaDisplay(CHARA_CHAI, CDM_Show);
              goto L0012CE;
            }
          }
          L0012CE:;
          Script_scenToggleVisibleFor(CHARA_CHAI, 0);
          Script_FaceEx(CHARA_TERY, 0, 0, 1);
          Script_clmdSetFlags(CHARA_MRSK, 2, 1);
          Script_clmdSetFlags(CHARA_STSI, 2, 1);
          Script_clmdSetFlags(CHARA_PEDR, 2, 1);
          Script_clmdSetFlags(CHARA_AKIR, 2, 0);
          Script_clmdSetFlags(CHARA_TERY, 2, 1);
          Script_charaDisplay(CHARA_MRSK, CDM_Hide);
          break;
        case 91:
          Script_FaceEx(CHARA_AKIR, 1, 0, 1);
          Script_FaceEx(CHARA_HRSK, 1, 0, 1);
          break;
        case 166:
          Script_FaceEx(CHARA_AKIR, 0, 0, 1);
          Script_FaceEx(CHARA_HRSK, 0, 0, 1);
          Script_FaceEx(CHARA_TERY, 1, 0, 1);
          break;
        case 234:
          Script_FaceEx(CHARA_TERY, 0, 0, 1);
          Script_FaceEx(CHARA_AKIR, 1, 0, 1);
          break;
        case 534:
          Script_FaceEx(CHARA_AKIR, 0, 0, 1);
          Script_clmdSetFlags(CHARA_AKIR, 2, 1);
          Script_charaDisplay(CHARA_MRSK, CDM_Show);
          break;
        case 669:
          Script_FaceEx(CHARA_TERY, 1, 0, 1);
          Script_clmdSetFlags(CHARA_TERY, 2, 0);
          break;
        case 711:
          Script_FaceEx(CHARA_TERY, 0, 0, 1);
          Script_charaDisplay(CHARA_MRSK, CDM_Hide);
          Script_charaDisplay(CHARA_AKIR, CDM_Hide);
          Script_charaDisplay(CHARA_HRSK, CDM_Hide);
          break;
        case 758:
          Script_CharHandlers(CHARA_TERY, 0, 1, 16);
          break;
        case 829:
          Script_FaceEx(CHARA_TERY, 1, 0, 1);
          Script_charaDisplay(CHARA_AKIR, CDM_Show);
          Script_charaDisplay(CHARA_HRSK, CDM_Show);
          break;
        case 1064:
          Script_FaceEx(CHARA_AKIR, 1, 0, 1);
          Script_FaceEx(CHARA_TERY, 0, 0, 1);
          break;
        case 1105:
          Script_FaceEx(CHARA_AKIR, 0, 0, 1);
          Script_FaceEx(CHARA_TERY, 1, 0, 1);
          break;
        case 1173:
          Script_FaceEx(CHARA_AKIR, 1, 0, 1);
          Script_clmdSetFlags(CHARA_TERY, 2, 1);
          Script_charaDisplay(CHARA_STSI, CDM_Hide);
          Script_charaDisplay(CHARA_PEDR, CDM_Hide);
          break;
        case 1520:
          Script_charaDisplay(CHARA_MRSK, CDM_Show);
          break;
        case 1720:
          Script_FaceEx(CHARA_TERY, 0, 0, 1);
          break;
        case 1870:
          Script_FaceEx(CHARA_HRSK, 1, 0, 1);
          break;
        case 2000:
          Script_CharHandlers(CHARA_TERY, 0, 8, 16);
          break;
        case 2069:
          Script_FaceEx(CHARA_AKIR, 0, 0, 1);
          Script_FaceEx(CHARA_HRSK, 0, 0, 1);
          Script_FaceEx(CHARA_TERY, 1, 0, 1);
          break;
        case 2110:
          Script_CharHandlers(CHARA_TERY, 0, 1, 16);
          Script_clmdSetFlags(CHARA_TERY, 2, 0);
          Script_charaDisplay(CHARA_PEDR, CDM_Show);
          Script_charaDisplay(CHARA_MRSK, CDM_Hide);
          break;
        case 2296:
          Script_FaceEx(CHARA_AKIR, 1, 0, 1);
          Script_FaceEx(CHARA_TERY, 0, 0, 1);
          break;
        case 2350:
          Script_FaceEx(CHARA_AKIR, 0, 0, 1);
          Script_clmdSetFlags(CHARA_TERY, 2, 1);
          Script_charaDisplay(CHARA_MRSK, CDM_Show);
          Script_charaDisplay(CHARA_STSI, CDM_Show);
          break;
        case 2584:
          Script_FaceEx(CHARA_AKIR, 1, 0, 1);
          Script_FaceEx(CHARA_TERY, 1, 0, 1);
          Script_charaDisplay(CHARA_MRSK, CDM_Hide);
          Script_charaDisplay(CHARA_STSI, CDM_Hide);
          Script_charaDisplay(CHARA_PEDR, CDM_Hide);
          break;
        case 2695:
          Script_FaceEx(CHARA_AKIR, 0, 0, 1);
          break;
        case 2959:
          Script_FaceEx(CHARA_AKIR, 1, 0, 1);
          Script_FaceEx(CHARA_HRSK, 1, 0, 1);
          Script_FaceEx(CHARA_TERY, 0, 0, 1);
          break;
        case 3112:
          Script_FaceEx(CHARA_AKIR, 0, 0, 1);
          Script_FaceEx(CHARA_HRSK, 0, 0, 1);
          Script_FaceEx(CHARA_TERY, 1, 0, 1);
          Script_clmdSetFlags(CHARA_TERY, 2, 0);
          Script_clmdSetFlags(CHARA_AKIR, 2, 0);
          Script_charaDisplay(CHARA_MRSK, CDM_Show);
          Script_charaDisplay(CHARA_PEDR, CDM_Show);
          break;
        case 3301:
          Script_CharHandlers(CHARA_TERY, 0, 8, 16);
          local_EC.x = 15.8561f;
          (&local_EC)->y = -0.015f;
          (&local_EC)->z = -2.1813f;
          Script_EVT_SetPosition(SHEN_NAME('BOX1'), POS_XYZ, &local_EC);
          Script_EVT_SetPosition(SHEN_NAME('BOX3'), POS_XYZ, &local_EC);
          Script_charaDisplay(CHARA_STSI, CDM_Show);
          Script_clmdSetFlags(CHARA_AKIR, 2, 1);
          Script_charaDisplay(CHARA_STSI, CDM_Show);
          break;
        case 3659:
          Script_FaceEx(CHARA_AKIR, 1, 0, 1);
          Script_charaDisplay(CHARA_MRSK, CDM_Hide);
          Script_charaDisplay(CHARA_STSI, CDM_Hide);
          break;
        case 3747:
          Script_FaceEx(CHARA_HRSK, 1, 0, 1);
          Script_FaceEx(CHARA_TERY, 0, 0, 1);
          Script_charaDisplay(CHARA_STSI, CDM_Show);
          break;
        case 3845:
          Script_filmCheckOrSetTransitionUtil(25, 0, 0, 0, 0, 255, 0, 0, 0);
          while (Script_filmCheckOrSetTransitionUtil(-1)) {
            SCNF_WaitFrames_0000(1);
          }
          break;
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
    if (Script_shenSeqUtils(-7) == 0) {
      script_1DF1();
    }
  }
  return;
}

static void script_191C(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  int32_t unused_EC[4];
  Vector3i local_E0;
  int32_t unused_C4[7];
  int32_t local_C0;
  local_E0.x = 0;
  (&local_E0)->y = 36408;
  (&local_E0)->z = 0;
  Script_EVT_SetRotation(SHEN_NAME('TDR1'), 0x38000000, &local_E0, 0);
  if (Script_shenSeqUtils(-7) != 0) {
  } else {
    script_0030();
    script_1B81();
    if (arg0) {
      Script_scenCreateTask(script_0153, 1, 20);
    }
    Script_shenSeqUtils(arg1);
    local_FC = 0;
    while (Script_shenSeqUtils(-1)) {
      local_C0 = local_FC;
      switch (local_C0) {
        case 0:
          Script_SetShadowFlags(CHARA_CHAI, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
          Script_SetShadowFlags(CHARA_CHAI, SCRIPT_SHADOW_UPDATE_LIGHT_MASK, 15);
          if (-1) {
            Script_charaDisplay(CHARA_TERY, CDM_Hide);
          } else {
            if (0) {
              Script_charaDisplay(CHARA_TERY, CDM_Show);
              goto L0019E1;
            }
          }
          L0019E1:;
          Script_scenToggleVisibleFor(CHARA_TERY, 0);
          if (-1) {
            Script_charaDisplay(CHARA_PEDR, CDM_Hide);
          } else {
            if (0) {
              Script_charaDisplay(CHARA_PEDR, CDM_Show);
              goto L001A15;
            }
          }
          L001A15:;
          Script_scenToggleVisibleFor(CHARA_PEDR, 0);
          if (-1) {
            Script_charaDisplay(CHARA_STSI, CDM_Hide);
          } else {
            if (0) {
              Script_charaDisplay(CHARA_STSI, CDM_Show);
              goto L001A49;
            }
          }
          L001A49:;
          Script_scenToggleVisibleFor(CHARA_STSI, 0);
          if (-1) {
            Script_charaDisplay(CHARA_MRSK, CDM_Hide);
          } else {
            if (0) {
              Script_charaDisplay(CHARA_MRSK, CDM_Show);
              goto L001A7D;
            }
          }
          L001A7D:;
          Script_scenToggleVisibleFor(CHARA_MRSK, 0);
          Script_clmdSetFlags(CHARA_HRSK, 2, 0);
          Script_clmdSetFlags(CHARA_AKIR, 2, 0);
          break;
        case 110:
          Script_scenCreateTask(script_0207, 3, CHARA_CHAI, 1, 3);
          break;
        case 150:
          Script_scenCreateTask(script_0241, 2, CHARA_CHAI, 3);
          break;
        case 160:
          Script_executeAicaCommand(2720, 2, 20);
          Script_filmCheckOrSetTransitionUtil(20, 0, 0, 0, 0, 255, 0, 0, 0);
          while (Script_filmCheckOrSetTransitionUtil(-1)) {
            SCNF_WaitFrames_0000(1);
          }
          break;
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
    if (Script_shenSeqUtils(-7) == 0) {
      script_1DF1();
    }
  }
  return;
}

static void script_1B81(void) {
  Vector3 local_F4;
  Script_SetVector4ByOffset(&data_7058);
  script_0048(CHARA_AKIR);
  script_0048(CHARA_TERY);
  script_0048(CHARA_PEDR);
  script_0048(CHARA_STSI);
  script_0048(CHARA_HRSK);
  script_0048(CHARA_MRSK);
  script_0048(CHARA_CHAI);
  Script_clmdSetFlags(CHARA_AKIR, 8, 1);
  Script_clmdSetFlags(CHARA_TERY, 8, 1);
  Script_clmdSetFlags(CHARA_PEDR, 8, 1);
  Script_clmdSetFlags(CHARA_STSI, 8, 1);
  Script_clmdSetFlags(CHARA_HRSK, 8, 1);
  Script_clmdSetFlags(CHARA_MRSK, 8, 1);
  Script_clmdSetFlags(CHARA_CHAI, 8, 1);
  Script_SetMotnFlag(CHARA_AKIR, -1);
  Script_SetMotnFlag(CHARA_TERY, -1);
  Script_SetMotnFlag(CHARA_PEDR, -1);
  Script_SetMotnFlag(CHARA_STSI, -1);
  Script_SetMotnFlag(CHARA_HRSK, -1);
  Script_SetMotnFlag(CHARA_MRSK, -1);
  Script_SetMotnFlag(CHARA_CHAI, -1);
  Script_SetCharaRefBoardPos(CHARA_AKIR, &data_7058);
  Script_SetCharaRefBoardPos(CHARA_TERY, &data_7058);
  Script_SetCharaRefBoardPos(CHARA_PEDR, &data_7058);
  Script_SetCharaRefBoardPos(CHARA_STSI, &data_7058);
  Script_SetCharaRefBoardPos(CHARA_HRSK, &data_7058);
  Script_SetCharaRefBoardPos(CHARA_MRSK, &data_7058);
  Script_SetCharaRefBoardPos(CHARA_CHAI, &data_7058);
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 1);
  Script_charaSetRefboardFlagTo(CHARA_TERY, 1);
  Script_charaSetRefboardFlagTo(CHARA_PEDR, 1);
  Script_charaSetRefboardFlagTo(CHARA_STSI, 1);
  Script_charaSetRefboardFlagTo(CHARA_HRSK, 1);
  Script_charaSetRefboardFlagTo(CHARA_MRSK, 1);
  Script_charaSetRefboardFlagTo(CHARA_CHAI, 1);
  Script_SetShadowFlags(CHARA_AKIR, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 0);
  Script_SetShadowFlags(CHARA_TERY, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 0);
  Script_SetShadowFlags(CHARA_PEDR, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 0);
  Script_SetShadowFlags(CHARA_STSI, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 0);
  Script_SetShadowFlags(CHARA_HRSK, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 0);
  Script_SetShadowFlags(CHARA_MRSK, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 0);
  Script_SetShadowFlags(CHARA_CHAI, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 0);
  local_F4.x = 15.8561f;
  (&local_F4)->y = 0.095f;
  (&local_F4)->z = -2.1813f;
  Script_EVT_SetPosition(SHEN_NAME('BOX1'), POS_XYZ, &local_F4);
  Script_EVT_SetPosition(SHEN_NAME('BOX3'), POS_XYZ, &local_F4);
  return;
}

static void script_1DF1(void) {
  Vector3 local_F4;
  script_00D5(CHARA_AKIR);
  script_00D5(CHARA_TERY);
  script_00D5(CHARA_PEDR);
  script_00D5(CHARA_STSI);
  script_00D5(CHARA_HRSK);
  script_00D5(CHARA_MRSK);
  script_00D5(CHARA_CHAI);
  Script_SetMotnFlag(CHARA_AKIR, 0);
  Script_SetMotnFlag(CHARA_TERY, 0);
  Script_SetMotnFlag(CHARA_PEDR, 0);
  Script_SetMotnFlag(CHARA_STSI, 0);
  Script_SetMotnFlag(CHARA_HRSK, 0);
  Script_SetMotnFlag(CHARA_MRSK, 0);
  Script_SetMotnFlag(CHARA_CHAI, 0);
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 2);
  Script_charaSetRefboardFlagTo(CHARA_TERY, 2);
  Script_charaSetRefboardFlagTo(CHARA_PEDR, 2);
  Script_charaSetRefboardFlagTo(CHARA_STSI, 2);
  Script_charaSetRefboardFlagTo(CHARA_HRSK, 2);
  Script_charaSetRefboardFlagTo(CHARA_MRSK, 2);
  Script_charaSetRefboardFlagTo(CHARA_CHAI, 2);
  if (-1) {
    Script_charaDisplay(CHARA_CHAI, CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(CHARA_CHAI, CDM_Show);
      goto L001F17;
    }
  }
  L001F17:;
  Script_scenToggleVisibleFor(CHARA_CHAI, 0);
  local_F4.x = 15.8561f;
  (&local_F4)->y = 0.095f;
  (&local_F4)->z = -2.1813f;
  Script_EVT_SetPosition(SHEN_NAME('BOX1'), POS_XYZ, &local_F4);
  Script_EVT_SetPosition(SHEN_NAME('BOX3'), POS_XYZ, &local_F4);
  return;
}

static void script_1F65(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  Script_scenCreateTask(script_1F7D, 5, arg0, arg1, arg2, arg3, arg4);
  return;
}

static int32_t script_1F7D(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  if (arg4 >= 0) {
    SCNF_WaitFrames_0000(arg4);
  }
  Script_FACE(arg0, arg1, arg2, arg3);
  return Script_srchControl(0);
}

static void script_1FA4(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
  Script_scenCreateTask(script_1FBE, 6, arg1, arg5, arg2, arg3, arg0, arg4);
  return;
}

static int32_t script_1FBE(int32_t arg0, int32_t arg1, int32_t arg2, float arg3, int32_t arg4, int32_t arg5) {
  if (arg1 > 0) {
    SCNF_WaitFrames_0000(arg1);
  }
  Script_motmGetCharaFlags(arg4, 1, 0);
  Script_charaToggleMotEnabled(arg4, 1);
  Script_motmToggleFlags(arg4, 1, 0x40);
  Script_SetCharaMotionPlaybackRate(arg4, arg3);
  Script_PlayCharaMotion(arg4, arg0, arg2, -1.0f, -1.0f, 1.0f, 0);
  Script_SetCharaMotionPlaybackRate(arg4, arg3);
  if (arg5 == 1) {
    Script_motmToggleFlags(arg4, 1, 0x80);
  }
  if (arg5 == 2) {
    Script_motmToggleFlags(arg4, 2, 0x40);
  }
  return Script_srchControl(0);
}

static void script_2046(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
  Script_scenCreateTask(script_2060, 6, arg0, arg1, arg2, arg3, arg4, arg5);
  return;
}

static int32_t script_2060(int32_t arg0, int32_t arg1, float arg2, float arg3, float arg4, int32_t arg5) {
  Vector3i local_F4;
  Vector3 local_E8;
  if (arg5 >= 0) {
    SCNF_WaitFrames_0000(arg5);
  }
  Script_MoveObjectToObject(arg0, arg1);
  local_E8.x = arg2;
  (&local_E8)->y = 0.0f;
  (&local_E8)->z = arg3;
  Script_EVT_SetPosition(arg0, POS_XYZ, &local_E8);
  local_F4.x = 0;
  (&local_F4)->y = (int32_t)(arg4 / 360.0f * (float)0x10000);
  (&local_F4)->z = 0;
  Script_EVT_SetRotation(arg0, 0x38000000, &local_F4, 0);
  Script_InitializeOsag(arg0);
  Script_MoveObjectToObject(arg0, -1);
  return Script_srchControl(0);
}

static void script_20EB(int32_t arg0, int32_t arg1) {
  Script_scenCreateTask(script_20FD, 2, arg0, arg1);
  return;
}

static int32_t script_20FD(int32_t arg0, int32_t arg1) {
  if (arg1 >= 0) {
    SCNF_WaitFrames_0000(arg1);
  }
  Script_executeAicaCommand(arg0, 0, 0);
  return Script_srchControl(0);
}

static void script_2121(int32_t arg0, int32_t arg1, int32_t arg2) {
  Script_scenCreateTask(script_2135, 3, arg0, arg1, arg2);
  return;
}

static int32_t script_2135(int32_t arg0, float arg1, int32_t arg2) {
  Vector3i local_F4;
  if (arg2 >= 0) {
    SCNF_WaitFrames_0000(arg2);
  }
  local_F4.x = 0;
  (&local_F4)->y = (int32_t)(arg1 / 360.0f * (float)0x10000);
  (&local_F4)->z = 0;
  return Script_EVT_SetRotation(arg0, -0x48000000, &local_F4, 0);
}

static void script_217E(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Script_scenCreateTask(script_2194, 4, arg0, arg1, arg2, arg3);
  return;
}

static int32_t script_2194(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Vector3 local_F4;
  if (arg3 > 0) {
    SCNF_WaitFrames_0000(arg3);
  }
  Script_EVT_GetPosition(arg1, 5, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  Script_SetLookPointDetails(arg0, arg2, &local_F4, 0);
  Script_FaceLookAtTargetUtils(arg0, 2, &local_F4, 8);
  return Script_srchControl(0);
}

static void script_21D7(int32_t arg0, int32_t arg1, int32_t arg2) {
  Script_scenCreateTask(script_21EB, 3, arg0, arg1, arg2);
  return;
}

static int32_t script_21EB(int32_t arg0, int32_t arg1, int32_t arg2) {
  if (arg2 > 0) {
    SCNF_WaitFrames_0000(arg2);
  }
  Script_SetCharaLookAtPosition(arg0, -arg1, 0, 0);
  Script_FaceLookAtTargetUtils(arg0, 0);
  return Script_srchControl(0);
}

static void script_221D(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Script_scenCreateTask(script_2233, 4, arg0, arg1, arg2, arg3);
  return;
}

static int32_t script_2233(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  if (arg3 >= 0) {
    SCNF_WaitFrames_0000(arg3);
  }
  if (arg0 == -1) {
    Script_SetGameStateTo(AUTH_None);
  } else if (arg1 == -1) {
    Script_seqEnterEvtCam(arg0, 0, AUTH_None);
  } else {
    Script_seqEnterEvtCam(arg0, arg1, arg2);
  }
  return Script_srchControl(0);
}

static void script_227F(int32_t arg0, int32_t arg1) {
  Script_scenCreateTask(script_2291, 2, arg0, arg1);
  return;
}

static int32_t script_2291(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  if (arg1 >= 0) {
    SCNF_WaitFrames_0000(arg1);
  }
  Script_StartOrFreeSEQ(0);
  local_FC = arg0;
  switch (local_FC) {
    case 0:
      Script_ShowUIMessageFromID("E1077A001");
      break;
    case 1:
      Script_ShowUIMessageFromID("E1077A002");
      break;
    case 2:
      Script_ShowUIMessageFromID("E1077A003");
      break;
    case 3:
      Script_ShowUIMessageFromID("E1077B001");
      break;
    case 4:
      Script_ShowUIMessageFromID("E1077B002");
      break;
    case 5:
      Script_ShowUIMessageFromID("E1077B003");
      break;
    case 6:
      Script_ShowUIMessageFromID("E1077C001");
      break;
    case 7:
      Script_ShowUIMessageFromID("E1077C002");
      break;
    case 8:
      Script_ShowUIMessageFromID("E1077D001");
      break;
    case 9:
      Script_ShowUIMessageFromID("E1077D002");
      break;
    case 10:
      Script_ShowUIMessageFromID("E1077D003");
      break;
    case 11:
      Script_ShowUIMessageFromID("E1077E001");
      break;
    case 12:
      Script_ShowUIMessageFromID("E1077F001");
      break;
  }
  return Script_srchControl(0);
}

static void script_23C9(int32_t arg0) {
  Script_scenToggleVisibleFor(arg0, 1);
  Script_ccowUtils(arg0, 2);
  Script_charaSetClipAngle(arg0, 10.0f);
  Script_SetCharaRefBoardPos(arg0, &data_7108);
  Script_charaSetRefboardFlagTo(arg0, 1);
  Script_SetCharaRefBoardPos(arg0, &data_7108);
  Script_charaDisplay(arg0, CDM_Show);
  Script_scenToggleActiveFor(arg0, 1);
  Script_EVT_SetPosition(arg0, POS_XYZ, &data_70FC);
  Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_10);
  Script_SetCPAMFlags(arg0, 0);
  return;
}

static void script_243C(int32_t arg0) {
  Script_ccowUtils(arg0, 1);
  Script_scenToggleActiveFor(arg0, 0);
  Script_SetCPAMFlags(arg0, 1);
  return;
}

static void script_2459(int32_t arg0) {
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
    Script_scenToggleVisibleFor(arg0, 0);
    Script_charaDisplay(arg0, CDM_Hide);
  }
  return;
}

static void script_2479(void) {
  script_2459(CHARA_CHRL);
  script_2459(CHARA_OKYS);
  script_2459(CHARA_STSI);
  script_2459(CHARA_TERY);
  script_2459(CHARA_PEDR);
  script_2459(CHARA_HRSK);
  script_2459(CHARA_MRSK);
  script_2459(CHARA_HTNK);
  script_2459(CHARA_OKYS);
  script_2459(CHARA_TDSI);
  script_2459(SHEN_NAME('NPC1'));
  script_2459(SHEN_NAME('NPC2'));
  script_2459(SHEN_NAME('NPC3'));
  script_2459(SHEN_NAME('NPC4'));
  script_2459(SHEN_NAME('NPC5'));
  script_2459(SHEN_NAME('NPC6'));
  script_2459(SHEN_NAME('NPC7'));
  script_2459(SHEN_NAME('NPC8'));
  script_2459(SHEN_NAME('NPC9'));
  script_2459(SHEN_NAME('NPCA'));
  script_2459(SHEN_NAME('NPCB'));
  script_2459(SHEN_NAME('NPCC'));
  return;
}

__attribute__((scn_string_bindings(0, data_7118, 1, data_7126, 3, data_7138)))
static void script_2584(void) {
  script_504E();
  Script_mapmSetCamFlags(0, 0);
  task_74 = 0;
  Script_CharHandlers(CHARA_AKIR, 2, 0, 16);
  script_4E9B((int32_t)"scene/03/TERY", (int32_t)"TERYB1");
  Script_LoadSND(0, 2, "bgm013.snd", 0);
  Script_LoadSND(0, 4, "e1terryt.snd", 0);
  Script_executeAicaCommand(9640, 0, 0);
  Script_executeAicaCommand(1184, 2, 104);
  while (task_78 == 1) {
    SCNF_WaitFrames_0000(1);
  }
  Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
  script_2479();
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 100.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 100.0f);
  Script_motmSetBlendFrames(CHARA_AKIR, MOTM_BLEND_FRAMES_10);
  Script_charaDisplay(SHEN_NAME('BIKE'), CDM_Show);
  script_23C9(CHARA_AKIR);
  script_23C9(CHARA_HTNK);
  script_23C9(CHARA_TDSI);
  script_23C9(SHEN_NAME('NPC1'));
  script_23C9(SHEN_NAME('NPC2'));
  script_23C9(SHEN_NAME('NPC3'));
  script_23C9(SHEN_NAME('NPC4'));
  script_23C9(SHEN_NAME('NPC5'));
  script_23C9(SHEN_NAME('NPC6'));
  script_221D(1000, SHEN_NAME('BASE'), SHEN_NAME('BASE'), -1);
  script_221D(1001, SHEN_NAME('BASE'), SHEN_NAME('BASE'), 130);
  script_221D(1002, SHEN_NAME('BASE'), SHEN_NAME('BASE'), 190);
  script_217E(CHARA_AKIR, CHARA_TDSI, 1, 40);
  script_21D7(CHARA_AKIR, 1, 270);
  script_227F(3, -1);
  script_227F(4, 55);
  script_227F(0, 130);
  script_227F(5, 190);
  script_227F(6, 300);
  script_1F65(CHARA_AKIR, 2, 0, 8, 290);
  script_1F65(CHARA_AKIR, 7, 0, 8, 330);
  script_2046(CHARA_AKIR, 0x45534142, 0x43052B85, 0x420547AE, 0x43250000, -1);
  script_2046(CHARA_HTNK, 0x45534142, 0x430228F6, 0x42177AE1, 0x439D8000, -1);
  script_2046(CHARA_TDSI, 0x45534142, 0x430275C3, 0x421B7AE1, 0x43AA0000, -1);
  script_1FA4(CHARA_AKIR, 102, 0, 0x3F800000, 2, -1);
  script_1FA4(CHARA_AKIR, 89, 0, 0x3F800000, 0, 80);
  script_1FA4(CHARA_HTNK, 668, 0, 0x3F333333, 2, -1);
  script_1FA4(CHARA_TDSI, 668, 0, 0x3F800000, 2, -1);
  SCNF_WaitFrames_0000(400);
  script_221D(1003, SHEN_NAME('BASE'), SHEN_NAME('BASE'), -1);
  script_221D(1004, SHEN_NAME('BASE'), SHEN_NAME('BASE'), 100);
  script_2046(SHEN_NAME('TDR1'), -1, 0x40C9999A, 0x3D23D70A, 0x43160000, -1);
  script_2046(CHARA_AKIR, 0x45534142, 0x430591EC, 0x42168F5C, 0x43250000, -1);
  script_2046(CHARA_HTNK, 0x45534142, 0x430228F6, 0x42177AE1, 0x43820000, -1);
  script_2046(CHARA_TDSI, 0x45534142, 0x430275C3, 0x421B7AE1, 0x438C0000, -1);
  script_2046(SHEN_NAME('NPC1'), 0x45534142, 0x43088CCD, 0x4216CCCD, 0x42960000, -1);
  script_2046(SHEN_NAME('NPC2'), 0x45534142, 0x4303947B, 0x422870A4, 0x438C0000, -1);
  script_2046(SHEN_NAME('NPC3'), 0x45534142, 0x43064CCD, 0x4206CCCD, 0x43430000, -1);
  script_2046(SHEN_NAME('NPC4'), 0x45534142, 0x430811EC, 0x42117AE1, 0x430C0000, -1);
  script_2046(SHEN_NAME('NPC5'), 0x45534142, 0x4306D70A, 0x422F47AE, 0x41A00000, -1);
  script_2046(SHEN_NAME('NPC6'), 0x45534142, 0x43010F5C, 0x42057AE1, 0x43660000, -1);
  script_227F(7, 100);
  script_1F65(CHARA_AKIR, 2, 0, 8, 100);
  script_1FA4(CHARA_AKIR, 1192, 0, 0x3F666666, 0, -1);
  script_1FA4(CHARA_AKIR, 787, 0, 0x3F800000, 1, 150);
  script_1FA4(SHEN_NAME('NPC1'), 34052, 0x42780000, 0x3F333333, 1, -1);
  script_1FA4(SHEN_NAME('NPC2'), 34052, 0x42A40000, 0x3F800000, 1, -1);
  script_1FA4(SHEN_NAME('NPC3'), 34049, 0, 0x3F800000, 0, -1);
  script_1FA4(SHEN_NAME('NPC4'), 34049, 0, 0x3F333333, 0, -1);
  script_1FA4(SHEN_NAME('NPC5'), 34050, 0, 0x3F800000, 0, -1);
  script_1FA4(SHEN_NAME('NPC5'), 34051, 0, 0x3F800000, 0, 50);
  script_1FA4(SHEN_NAME('NPC6'), 34050, 0, 0x3F333333, 0, -1);
  script_1FA4(SHEN_NAME('NPC6'), 34051, 0, 0x3F800000, 0, 60);
  SCNF_WaitFrames_0000(200);
  return;
}

__attribute__((scn_string_bindings(0, data_7145, 1, data_714C, 2, data_715A)))
static void script_2B20(void) {
  Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "TERYB1");
  Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
  script_4E9B((int32_t)"scene/03/TERY", (int32_t)"TERYB2");
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 30.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 30.0f);
  script_504E();
  task_74 = 0;
  script_23C9(CHARA_AKIR);
  script_221D(2000, SHEN_NAME('BASE'), SHEN_NAME('BASE'), -1);
  script_221D(2001, SHEN_NAME('BASE'), SHEN_NAME('BASE'), 120);
  Script_motmSetBlendFrames(CHARA_AKIR, MOTM_BLEND_FRAMES_10);
  script_1F65(CHARA_AKIR, 2, 0, 8, -1);
  script_1F65(CHARA_AKIR, 0, 0, 8, 60);
  script_2046(SHEN_NAME('TDR1'), -1, 0x40C9999A, 0x3D23D70A, 0x43160000, -1);
  script_2046(CHARA_AKIR, 0x45534142, 0x4306A666, 0x42147AE1, 0x43390000, -1);
  script_2046(CHARA_AKIR, 0x45534142, 0x4306A666, 0x4250E148, 0x43390000, 120);
  script_1FA4(CHARA_AKIR, 787, 0, 0x3F800000, 2, -1);
  script_1FA4(CHARA_AKIR, 102, 0, 0x3F800000, 0, 70);
  SCNF_WaitFrames_0000(150);
  while (task_78 == 1) {
    SCNF_WaitFrames_0000(1);
  }
  Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_2CF1(void) {
  Script_WeatherTaskUtil(ToggleSceneLightingFlags, 0);
  Script_WeatherTaskUtil(ToggleFogTask, 0);
  Script_WeatherUtils_0(WeatherDataUtil_SetInterpolatedWeatherOverride, 1, 0, 1, 0);
  Script_WeatherUtils_0(WeatherDataUtil_SetInterpolatedWeatherOverride, 2, 0, 1, 0);
  Script_lightSetSlotEnabled(1, 0);
  Script_lightSetSlotEnabled(2, 0);
  Script_lightSetSlotEnabled(3, 0);
  Script_PREP_Related(-1, 0);
  Script_shenSetLightingMode(2);
  Script_lightSetGlobalSceneLighting(&data_7164);
  script_2479();
  script_23C9(CHARA_AKIR);
  script_23C9(CHARA_OKYS);
  script_23C9(SHEN_NAME('NPC7'));
  script_23C9(SHEN_NAME('NPC8'));
  script_23C9(SHEN_NAME('NPC9'));
  script_23C9(SHEN_NAME('NPCA'));
  script_23C9(SHEN_NAME('NPCB'));
  script_23C9(SHEN_NAME('NPCC'));
  script_512D();
  task_74 = 0;
  Script_motmSetBlendFrames(CHARA_OKYS, MOTM_BLEND_FRAMES_20);
  script_1FA4(CHARA_AKIR, 787, 0, 0x3F800000, 0, -1);
  script_2046(CHARA_AKIR, -1, 0x4057AE14, -0x3FDD70A4, 0, -1);
  SCNF_WaitFrames_0000(5);
  Script_seqEnterEvtCam(0, -1, -1);
  Script_convSetCamInactiveFlag(1);
  Script_SetGameStateTo(AUTH_None);
  Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
  script_221D(2002, -1, -1, -1);
  script_221D(2003, -1, -1, 40);
  script_221D(2007, -1, -1, 100);
  script_221D(2004, -1, -1, 170);
  script_221D(2005, -1, -1, 270);
  script_221D(2006, -1, -1, 360);
  script_2046(SHEN_NAME('TDR1'), -1, 0x40C9999A, 0x3D23D70A, 0x43340000, -1);
  script_2046(CHARA_AKIR, -1, 0x40500000, -0x3F633333, 0x43938000, 170);
  script_2046(CHARA_OKYS, -1, 0x40EF0A3D, -0x3F10F5C3, 0x43020000, 40);
  script_2046(SHEN_NAME('NPC7'), -1, 0x3FFC28F6, -0x3FDE147B, 0x43960000, 40);
  script_2046(SHEN_NAME('NPC8'), -1, 0x409570A4, -0x40C51EB8, 0x420C0000, 40);
  script_2046(SHEN_NAME('NPC9'), -1, 0x40CBD70A, -0x4008F5C3, 0x42820000, 40);
  script_2046(SHEN_NAME('NPCA'), -1, 0x40BF0A3D, -0x3F4BD70A, 0x42F00000, 40);
  script_2046(SHEN_NAME('NPCB'), -1, 0x40AF0A3D, -0x3F1BD70A, 0x43240000, 40);
  script_1FA4(CHARA_AKIR, 102, 0, 0x3F800000, 2, -1);
  script_1FA4(CHARA_AKIR, 38, 0, 0x3F800000, 0, 60);
  script_1FA4(CHARA_OKYS, 668, 0, 0x3F800000, 2, 40);
  script_1FA4(CHARA_OKYS, 755, 0, 0x3F800000, 0, 460);
  script_1FA4(SHEN_NAME('NPC7'), 34051, 0, 0x3F800000, 2, 40);
  script_1FA4(SHEN_NAME('NPC8'), 34049, 0, 0x3F800000, 2, 40);
  script_1FA4(SHEN_NAME('NPC9'), 49, 0, 0x3F800000, 2, 40);
  script_1FA4(SHEN_NAME('NPCA'), 34051, 0, 0x3F800000, 2, 40);
  script_1FA4(SHEN_NAME('NPCB'), 34049, 0, 0x3F800000, 2, 40);
  script_20EB(1193, 20);
  script_227F(1, 40);
  script_227F(8, 170);
  script_227F(9, 210);
  script_227F(2, 270);
  script_227F(10, 360);
  script_32BE(100);
  script_3330(130);
  script_3383(160);
  script_33D6(270);
  script_1F65(CHARA_AKIR, 7, 0, 8, 40);
  script_1F65(CHARA_AKIR, 2, 0, 8, 270);
  SCNF_WaitFrames_0000(480);
  return;
}

__attribute__((scn_string_bindings(0, data_7174)))
static void script_3129(void) {
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 30.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 30.0f);
  task_74 = 0;
  script_23C9(CHARA_AKIR);
  script_2046(CHARA_OKYS, -1, 0x406147AE, -0x3F247AE1, 0x43340000, -1);
  script_2046(SHEN_NAME('NPC7'), -1, 0x4059999A, -0x404147AE, 0x42200000, -1);
  script_2046(SHEN_NAME('NPC8'), -1, 0x416AB852, -0x3EE451EC, 0x420C0000, -1);
  script_2046(SHEN_NAME('NPC9'), -1, 0x40A570A4, -0x3FA8F5C3, 0x43AF0000, -1);
  script_2046(SHEN_NAME('NPCA'), -1, 0x425FE148, -0x3F4BD70A, 0x42F00000, -1);
  script_2046(SHEN_NAME('NPCB'), -1, 0x425DE148, -0x3F1BD70A, 0x43240000, -1);
  script_221D(3000, -1, -1, -1);
  Script_motmSetBlendFrames(CHARA_AKIR, MOTM_BLEND_FRAMES_10);
  script_2046(CHARA_AKIR, -1, 0x4063D70A, -0x3F71EB85, 0x43938000, -1);
  script_1FA4(CHARA_AKIR, 787, 0, 0x3F800000, 2, -1);
  script_1FA4(CHARA_AKIR, 102, 0, 0x3F800000, 0, 70);
  SCNF_WaitFrames_0000(100);
  Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "TERYB2");
  return;
}

static void script_32BE(int32_t arg0) {
  Script_scenCreateTask(script_32CE, 1, arg0);
  return;
}

static int32_t script_32CE(int32_t arg0) {
  if (arg0 >= 0) {
    SCNF_WaitFrames_0000(arg0);
  }
  Script_filmCheckOrSetTransitionUtil(10, 255, 0, 0, 0, 0, 0, 0, 0);
  SCNF_WaitFrames_0000(10);
  script_20EB(66729, -1);
  Script_lightUtils(1, 4, &data_717C);
  Script_LightUtils(1, 5, 7.02f);
  return Script_srchControl(0);
}

static void script_3330(int32_t arg0) {
  Script_scenCreateTask(script_3340, 1, arg0);
  return;
}

static int32_t script_3340(int32_t arg0) {
  if (arg0 >= 0) {
    SCNF_WaitFrames_0000(arg0);
  }
  script_20EB(66729, -1);
  Script_lightUtils(2, 4, &data_71B0);
  Script_LightUtils(2, 5, 7.58f);
  return Script_srchControl(0);
}

static void script_3383(int32_t arg0) {
  Script_scenCreateTask(script_3393, 1, arg0);
  return;
}

static int32_t script_3393(int32_t arg0) {
  if (arg0 >= 0) {
    SCNF_WaitFrames_0000(arg0);
  }
  script_20EB(66729, -1);
  Script_lightUtils(3, 4, &data_71E4);
  Script_LightUtils(3, 5, 7.17f);
  return Script_srchControl(0);
}

static void script_33D6(int32_t arg0) {
  Script_scenCreateTask(script_33E6, 1, arg0);
  return;
}

static int32_t script_33E6(int32_t arg0) {
  if (arg0 >= 0) {
    SCNF_WaitFrames_0000(arg0);
  }
  Script_lightUtils(1, 4, &data_7218);
  Script_LightUtils(1, 5, 7.02f);
  return Script_srchControl(0);
}

__attribute__((scn_string_bindings(0, data_724C)))
static void script_341B(void) {
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 100.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 100.0f);
  Script_LoadSND(0, 5, "battle_1.snd", 0);
  Script_ReturnTimeDate(&task_80);
  for (;;) {
    script_23C9(CHARA_AKIR);
    script_23C9(CHARA_HTNK);
    script_23C9(CHARA_TDSI);
    script_23C9(SHEN_NAME('NPC1'));
    script_23C9(SHEN_NAME('NPC2'));
    script_23C9(SHEN_NAME('NPC3'));
    script_23C9(SHEN_NAME('NPC4'));
    script_23C9(SHEN_NAME('NPC5'));
    script_23C9(SHEN_NAME('NPC6'));
    script_243C(CHARA_AKIR);
    script_243C(CHARA_HTNK);
    script_243C(CHARA_TDSI);
    script_243C(SHEN_NAME('NPC1'));
    script_243C(SHEN_NAME('NPC2'));
    script_243C(SHEN_NAME('NPC3'));
    script_243C(SHEN_NAME('NPC4'));
    script_243C(SHEN_NAME('NPC5'));
    script_243C(SHEN_NAME('NPC6'));
    script_2046(CHARA_AKIR, 0x45534142, 0x4305D99A, 0x4214851F, 0x42F00000, -1);
    script_2046(CHARA_HTNK, 0x45534142, 0x430228F6, 0x42177AE1, 0x43898000, -1);
    script_2046(CHARA_TDSI, 0x45534142, 0x430275C3, 0x421B7AE1, 0x43898000, -1);
    script_2046(SHEN_NAME('NPC1'), 0x45534142, 0x4307E8F6, 0x421F28F6, 0x42960000, -1);
    script_2046(SHEN_NAME('NPC2'), 0x45534142, 0x43033D71, 0x421FAE14, 0x438C0000, -1);
    script_2046(SHEN_NAME('NPC3'), 0x45534142, 0x43064CCD, 0x4206CCCD, 0x43430000, -1);
    script_2046(SHEN_NAME('NPC4'), 0x45534142, 0x430811EC, 0x42117AE1, 0x430C0000, -1);
    script_2046(SHEN_NAME('NPC5'), 0x45534142, 0x430623D7, 0x42280A3D, 0x41A00000, -1);
    script_2046(SHEN_NAME('NPC6'), 0x45534142, 0x4302428F, 0x42096666, 0x43660000, -1);
    SCNF_WaitFrames_0000(1);
    Script_seqEnterEvtCam(0, -1, -1);
    Script_convSetCamInactiveFlag(1);
    Script_SetGameStateTo(AUTH_None);
    Script_vfSetup(0, 20, 20, 0, 0);
    task_0 = 0x52494B41;
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
    task_0 = 0x4B4E5448;
    *(&task_0 + 1) = 0;
    *(&task_0 + 2) = 1;
    *(&task_0 + 3) = 1;
    *(&task_0 + 4) = 120;
    *(&task_0 + 5) = 14;
    *(&task_0 + 6) = 0;
    *(&task_0 + 7) = 0;
    *(&task_0 + 8) = 0;
    *(&task_0 + 9) = 0;
    *(&task_0 + 10) = 0;
    Script_vfUtils(&task_0);
    Script_vfConfigureFormationGroup(CHARA_HTNK, VF_FORMATION_APPLY_FLAGS, 0x200000, 0x800);
    Script_vfConfigureFormationGroup(CHARA_HTNK, VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
    Script_vfConfigureFormationGroup(CHARA_HTNK, VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
    task_0 = 0x49534454;
    *(&task_0 + 1) = 0;
    *(&task_0 + 2) = 1;
    *(&task_0 + 3) = 1;
    *(&task_0 + 4) = 120;
    *(&task_0 + 5) = 15;
    *(&task_0 + 6) = 0;
    *(&task_0 + 7) = 0;
    *(&task_0 + 8) = 0;
    *(&task_0 + 9) = 0;
    *(&task_0 + 10) = 0;
    Script_vfUtils(&task_0);
    Script_vfConfigureFormationGroup(CHARA_TDSI, VF_FORMATION_APPLY_FLAGS, 0x200000, 0x800);
    Script_vfConfigureFormationGroup(CHARA_TDSI, VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
    Script_vfConfigureFormationGroup(CHARA_TDSI, VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
    task_0 = 0x3143504E;
    *(&task_0 + 1) = 0;
    *(&task_0 + 2) = 1;
    *(&task_0 + 3) = 1;
    *(&task_0 + 4) = 70;
    *(&task_0 + 5) = 41;
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
    *(&task_0 + 4) = 70;
    *(&task_0 + 5) = 40;
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
    *(&task_0 + 4) = 70;
    *(&task_0 + 5) = 37;
    *(&task_0 + 6) = 0;
    *(&task_0 + 7) = 0;
    *(&task_0 + 8) = 0;
    *(&task_0 + 9) = 0;
    *(&task_0 + 10) = 0;
    Script_vfUtils(&task_0);
    Script_vfConfigureFormationGroup(SHEN_NAME('NPC3'), VF_FORMATION_APPLY_FLAGS, 0x200000, 0x800);
    Script_vfConfigureFormationGroup(SHEN_NAME('NPC3'), VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
    Script_vfConfigureFormationGroup(SHEN_NAME('NPC3'), VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
    task_0 = 0x3443504E;
    *(&task_0 + 1) = 0;
    *(&task_0 + 2) = 1;
    *(&task_0 + 3) = 1;
    *(&task_0 + 4) = 70;
    *(&task_0 + 5) = 38;
    *(&task_0 + 6) = 0;
    *(&task_0 + 7) = 0;
    *(&task_0 + 8) = 0;
    *(&task_0 + 9) = 0;
    *(&task_0 + 10) = 0;
    Script_vfUtils(&task_0);
    Script_vfConfigureFormationGroup(SHEN_NAME('NPC4'), VF_FORMATION_APPLY_FLAGS, 0x200000, 0x800);
    Script_vfConfigureFormationGroup(SHEN_NAME('NPC4'), VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
    Script_vfConfigureFormationGroup(SHEN_NAME('NPC4'), VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
    task_0 = 0x3543504E;
    *(&task_0 + 1) = 0;
    *(&task_0 + 2) = 1;
    *(&task_0 + 3) = 1;
    *(&task_0 + 4) = 70;
    *(&task_0 + 5) = 39;
    *(&task_0 + 6) = 0;
    *(&task_0 + 7) = 0;
    *(&task_0 + 8) = 0;
    *(&task_0 + 9) = 0;
    *(&task_0 + 10) = 0;
    Script_vfUtils(&task_0);
    Script_vfConfigureFormationGroup(SHEN_NAME('NPC5'), VF_FORMATION_APPLY_FLAGS, 0x200000, 0x800);
    Script_vfConfigureFormationGroup(SHEN_NAME('NPC5'), VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
    Script_vfConfigureFormationGroup(SHEN_NAME('NPC5'), VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
    task_0 = 0x3643504E;
    *(&task_0 + 1) = 0;
    *(&task_0 + 2) = 1;
    *(&task_0 + 3) = 1;
    *(&task_0 + 4) = 70;
    *(&task_0 + 5) = 41;
    *(&task_0 + 6) = 0;
    *(&task_0 + 7) = 0;
    *(&task_0 + 8) = 0;
    *(&task_0 + 9) = 0;
    *(&task_0 + 10) = 0;
    Script_vfUtils(&task_0);
    Script_vfConfigureFormationGroup(SHEN_NAME('NPC6'), VF_FORMATION_APPLY_FLAGS, 0x200000, 0x800);
    Script_vfConfigureFormationGroup(SHEN_NAME('NPC6'), VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
    Script_vfConfigureFormationGroup(SHEN_NAME('NPC6'), VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
    SCN_Enqueue_VFGI(2, 99, 1.0f, 1, -1);
    while (SCN_GetIsFighting() != 2) {
      SCNF_WaitFrames_0000(1);
    }
    if (Script_GetFightingStats(0x52494B41, 0) == 3) {
      break;
    } else {
      Script_uiUtils(0, 20, 0);
      Script_executeAicaCommand(2720, 2, 30);
      Script_filmCheckOrSetTransitionUtil(60, 0, 0, 0, 0, 255, 0, 0, 0);
      while (Script_filmCheckOrSetTransitionUtil(-1)) {
        SCNF_WaitFrames_0000(1);
      }
      Script_shenClearFreeState(1);
      Script_sicnUtils(0, 1, 1);
      SCNF_WaitFrames_0000(90);
      Script_shenClearFreeState(0);
      Script_SetNewDateTime(&task_80);
      Script_WeatherTaskUtil(FreeTaskData);
      Script_WeatherTaskUtil(InitTaskData);
      Script_sicnUtils(2, 1, 0);
      Script_executeAicaCommand(9640, 0, 0);
      Script_executeAicaCommand(1184, 2, 104);
      script_2584();
      Script_uiUtils(0, 0, 0);
    }
  }
  Script_charaDisplay(SHEN_NAME('BIKE'), CDM_Hide);
  return;
}

__attribute__((scn_string_bindings(0, data_7259)))
static void script_3D77(void) {
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 30.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 30.0f);
  Script_LoadSND(0, 5, "battle_1.snd", 0);
  Script_ReturnTimeDate(&task_80);
  for (;;) {
    script_23C9(CHARA_AKIR);
    script_23C9(CHARA_OKYS);
    script_23C9(SHEN_NAME('NPC7'));
    script_23C9(SHEN_NAME('NPC8'));
    script_23C9(SHEN_NAME('NPC9'));
    script_23C9(SHEN_NAME('NPCA'));
    script_23C9(SHEN_NAME('NPCB'));
    script_23C9(SHEN_NAME('NPCC'));
    script_243C(CHARA_AKIR);
    script_243C(CHARA_OKYS);
    script_243C(SHEN_NAME('NPC7'));
    script_243C(SHEN_NAME('NPC8'));
    script_243C(SHEN_NAME('NPC9'));
    script_243C(SHEN_NAME('NPCA'));
    script_243C(SHEN_NAME('NPCB'));
    script_243C(SHEN_NAME('NPCC'));
    script_2046(CHARA_AKIR, -1, 0x4057AE14, -0x3F623D71, 0, -1);
    script_2046(CHARA_OKYS, -1, 0x40EF0A3D, -0x3F10F5C3, 0x43020000, -1);
    script_2046(SHEN_NAME('NPC7'), -1, 0x4017AE14, -0x3FDE147B, 0x43960000, -1);
    script_2046(SHEN_NAME('NPC8'), -1, 0x409570A4, -0x40628F5C, 0x420C0000, -1);
    script_2046(SHEN_NAME('NPC9'), -1, 0x40B570A4, -0x3FD7AE14, 0x42820000, -1);
    script_2046(SHEN_NAME('NPCA'), -1, 0x40A8A3D7, -0x3F5570A4, 0x42F00000, -1);
    script_2046(SHEN_NAME('NPCB'), -1, 0x407E147B, -0x3F2F0A3D, 0x43240000, -1);
    SCNF_WaitFrames_0000(1);
    Script_seqEnterEvtCam(0, -1, -1);
    Script_convSetCamInactiveFlag(1);
    Script_SetGameStateTo(AUTH_None);
    Script_vfSetup(0, 20, 20, 0, 0);
    task_0 = 0x52494B41;
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
    task_0 = 0x53594B4F;
    *(&task_0 + 1) = 0;
    *(&task_0 + 2) = 1;
    *(&task_0 + 3) = 1;
    *(&task_0 + 4) = 180;
    *(&task_0 + 5) = 45;
    *(&task_0 + 6) = 0;
    *(&task_0 + 7) = 0;
    *(&task_0 + 8) = 0;
    *(&task_0 + 9) = 0;
    *(&task_0 + 10) = 0;
    Script_vfUtils(&task_0);
    Script_vfConfigureFormationGroup(CHARA_OKYS, VF_FORMATION_APPLY_FLAGS, 0x200000, 0x800);
    task_0 = 0x3743504E;
    *(&task_0 + 1) = 0;
    *(&task_0 + 2) = 1;
    *(&task_0 + 3) = 1;
    *(&task_0 + 4) = 80;
    *(&task_0 + 5) = 39;
    *(&task_0 + 6) = 0;
    *(&task_0 + 7) = 0;
    *(&task_0 + 8) = 0;
    *(&task_0 + 9) = 0;
    *(&task_0 + 10) = 0;
    Script_vfUtils(&task_0);
    Script_vfConfigureFormationGroup(SHEN_NAME('NPC7'), VF_FORMATION_APPLY_FLAGS, 0x200000, 0x800);
    Script_vfConfigureFormationGroup(SHEN_NAME('NPC7'), VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
    Script_vfConfigureFormationGroup(SHEN_NAME('NPC7'), VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
    task_0 = 0x3843504E;
    *(&task_0 + 1) = 0;
    *(&task_0 + 2) = 1;
    *(&task_0 + 3) = 1;
    *(&task_0 + 4) = 80;
    *(&task_0 + 5) = 40;
    *(&task_0 + 6) = 0;
    *(&task_0 + 7) = 0;
    *(&task_0 + 8) = 0;
    *(&task_0 + 9) = 0;
    *(&task_0 + 10) = 0;
    Script_vfUtils(&task_0);
    Script_vfConfigureFormationGroup(SHEN_NAME('NPC8'), VF_FORMATION_APPLY_FLAGS, 0x200000, 0x800);
    Script_vfConfigureFormationGroup(SHEN_NAME('NPC8'), VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
    Script_vfConfigureFormationGroup(SHEN_NAME('NPC8'), VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
    task_0 = 0x3943504E;
    *(&task_0 + 1) = 0;
    *(&task_0 + 2) = 1;
    *(&task_0 + 3) = 1;
    *(&task_0 + 4) = 80;
    *(&task_0 + 5) = 37;
    *(&task_0 + 6) = 0;
    *(&task_0 + 7) = 0;
    *(&task_0 + 8) = 0;
    *(&task_0 + 9) = 0;
    *(&task_0 + 10) = 0;
    Script_vfUtils(&task_0);
    Script_vfConfigureFormationGroup(SHEN_NAME('NPC9'), VF_FORMATION_APPLY_FLAGS, 0x200000, 0x800);
    Script_vfConfigureFormationGroup(SHEN_NAME('NPC9'), VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
    Script_vfConfigureFormationGroup(SHEN_NAME('NPC9'), VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
    task_0 = 0x4143504E;
    *(&task_0 + 1) = 0;
    *(&task_0 + 2) = 1;
    *(&task_0 + 3) = 1;
    *(&task_0 + 4) = 80;
    *(&task_0 + 5) = 38;
    *(&task_0 + 6) = 0;
    *(&task_0 + 7) = 0;
    *(&task_0 + 8) = 0;
    *(&task_0 + 9) = 0;
    *(&task_0 + 10) = 0;
    Script_vfUtils(&task_0);
    Script_vfConfigureFormationGroup(SHEN_NAME('NPCA'), VF_FORMATION_APPLY_FLAGS, 0x200000, 0x800);
    Script_vfConfigureFormationGroup(SHEN_NAME('NPCA'), VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
    Script_vfConfigureFormationGroup(SHEN_NAME('NPCA'), VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
    task_0 = 0x4243504E;
    *(&task_0 + 1) = 0;
    *(&task_0 + 2) = 1;
    *(&task_0 + 3) = 1;
    *(&task_0 + 4) = 80;
    *(&task_0 + 5) = 41;
    *(&task_0 + 6) = 0;
    *(&task_0 + 7) = 0;
    *(&task_0 + 8) = 0;
    *(&task_0 + 9) = 0;
    *(&task_0 + 10) = 0;
    Script_vfUtils(&task_0);
    Script_vfConfigureFormationGroup(SHEN_NAME('NPCB'), VF_FORMATION_APPLY_FLAGS, 0x200000, 0x800);
    Script_vfConfigureFormationGroup(SHEN_NAME('NPCB'), VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
    Script_vfConfigureFormationGroup(SHEN_NAME('NPCB'), VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
    SCN_Enqueue_VFGI(2, 99, 1.0f, 1, -1);
    while (SCN_GetIsFighting() != 2) {
      SCNF_WaitFrames_0000(1);
    }
    if (Script_GetFightingStats(0x52494B41, 0) == 3) {
      break;
    } else {
      Script_uiUtils(0, 20, 0);
      Script_executeAicaCommand(2720, 2, 30);
      Script_filmCheckOrSetTransitionUtil(60, 0, 0, 0, 0, 255, 0, 0, 0);
      while (Script_filmCheckOrSetTransitionUtil(-1)) {
        SCNF_WaitFrames_0000(1);
      }
      Script_shenClearFreeState(1);
      Script_sicnUtils(0, 1, 1);
      SCNF_WaitFrames_0000(90);
      Script_sicnUtils(2, 1, 0);
      Script_shenClearFreeState(0);
      Script_SetNewDateTime(&task_80);
      Script_WeatherTaskUtil(FreeTaskData);
      Script_WeatherTaskUtil(InitTaskData);
      Script_filmCheckOrSetTransitionUtil(60, 255, 0, 0, 0, 0, 0, 0, 0);
      Script_executeAicaCommand(9640, 0, 0);
      Script_executeAicaCommand(1184, 2, 104);
      script_2CF1();
      Script_uiUtils(0, 0, 0);
    }
  }
  task_0 = -6;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 0;
  *(&task_0 + 3) = 0;
  *(&task_0 + 4) = 0;
  *(&task_0 + 5) = 12;
  *(&task_0 + 6) = 0;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0;
  Script_vfUtils(&task_0);
  Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
  return;
}

void script_4596(void) {
  int32_t unused_FC;
  GameDateTime local_F4;
  Script_filmCheckOrSetTransitionUtil(0, 255, 0, 0, 0, 0, 0, 0, 0);
  task_78 = 0;
  task_74 = 0;
  Script_SetMapFlags(0, 1);
  Script_GetSetWeatherSettings(WEATHER_SETTINGS_SET_DISABLE_WEATHER, 1);
  Script_WeatherUtils_0(WeatherDataUtil_ApplyCurrentWeatherOrEnableDateTimeRead, 0);
  Script_WeatherTaskUtil(InitTaskData);
  Script_ReturnTimeDate(&local_F4);
  task_7C = Script_WeatherUtils_0(WeatherDataUtil_GetWeatherClassForDateTime, (&local_F4)->year, (&local_F4)->month, (&local_F4)->day, (&local_F4)->hours, (&local_F4)->minutes);
  Script_scenCreateTask(script_462F, 0);
  for (;;) {
    task_74 = task_74 + 1;
    SCNF_WaitFrames_0000(1);
  }
  return;
}

__attribute__((scn_string_bindings(0, data_7266, 1, data_7274, 2, data_7279, 3, data_727F, 4, data_7285, 6, data_7296, 7, data_729B, 8, data_72A0, 9, data_72A5, 10, data_72B2, 11, data_72B7, 12, data_72BD)))
static int32_t script_462F(void) {
  int32_t unused_FC;
  int32_t local_F8;
  local_F8 = Script_SetMaxNPCsNear(32);
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 10, -1);
  script_4E9B((int32_t)"scene/03/TERY", (int32_t)"BIKE");
  while (task_78 == 1) {
    SCNF_WaitFrames_0000(1);
  }
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 2, "MPK00");
  Script_facemotUtils(CHARA_AKIR, 1, 0);
  Script_motmSetBlendFrames(CHARA_AKIR, MOTM_BLEND_FRAMES_10);
  Script_SetActionSelectorHUDElementVisibility(1);
  script_07E9();
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_FaceEx(CHARA_AKIR, 1, 0, 2);
  Script_ResetHandMotion(CHARA_AKIR, -1, 0, 0);
  Script_ResetHandMotion(CHARA_AKIR, -1, 1, 0);
  Script_shenClearFreeState(0);
  Script_ryoSetChara(CHARA_AKIR);
  Script_cdttLoadScrUtils(1, 0);
  script_2046(SHEN_NAME('BASE'), -1, 0x43098000, 0x4271999A, 0x43336666, -1);
  if (Script_GetCurrentEntryPoint() == 99) {
    Script_ryoControl(PCCTL_DisablePlayerControl);
    Script_SetShouldSleepFlag(0);
    Script_SetGameStateTo(AUTH_None);
    script_23C9(CHARA_HTNK);
    script_23C9(CHARA_TDSI);
    script_49F5();
    Script_PREP_Related(-1, 0);
    Script_SetupLightFromTable(2);
    Script_PREP_Related(-1, 1);
    Script_SetCameraFlags(0);
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 2, "MPK00");
    Script_facemotUtils(CHARA_AKIR, 0, 0);
    Script_FaceEx(CHARA_AKIR, 1, 0, 1);
    Script_LoadVoiceFile(1, "E1077");
    Script_motiLoad(3, CHARA_TERY, "M_FGT1.BIN");
    Script_LoadSND(0, 6, "FREE", 0);
    Script_LoadSND(0, 5, "FREE", 0);
    Script_LoadSND(0, 4, "FREE", 0);
    Script_LoadSND(0, 4, "e1terryt.snd", 0);
    script_2584();
    Script_ReallocateUNFO(task_6C);
    script_341B();
    script_2B20();
    script_2CF1();
    script_3D77();
    script_3129();
    Script_SetCameraFlags(1);
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "BIKE");
    script_4BC4();
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 909, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 833, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 527, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_PROMISED_TERRY_TO_FIGHT_CHEN_GUI_ZHANG, 1);
    Script_SetMaxNPCsNear(local_F8);
    Script_LoadVoiceFile(0, "E1077");
    Script_cdttUtil(Mode6);
    Script_shenRequestGameModuleTransition(3, SHEN_NAME('NBIK'), 0);
    SCNF_WaitFrames_0000(1);
  } else {
    Script_charaDisplay(CHARA_AKIR, CDM_Show);
    Script_ryoControl(RYO_RestoreControl);
    Script_FaceEx(CHARA_AKIR, 0, 0, 2);
    Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
    Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
    Script_shenClearFreeState(1);
    Script_ryoControl(PCCTL_DisablePlayerControl);
    Script_SetShouldSleepFlag(0);
    Script_SetGameStateTo(AUTH_None);
    Script_FaceEx(CHARA_AKIR, 1, 0, 1);
    if ((Script_GetCurrentEntryPoint() == 90) | (Script_GetCurrentEntryPoint() == 93)) {
      Script_PREP_Related(-1, 0);
      Script_SetupLightFromTable(2);
      Script_PREP_Related(-1, 1);
    }
    {
      int32_t local_F4;
      local_F4 = Script_GetCurrentEntryPoint();
      if (local_F4 == 0) {
        Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "BIKE");
        goto L00493F;
      }
    }
    L00493F:;
    for (;;) {
      int32_t local_F4;
      local_F4 = Script_GetCurrentEntryPoint();
      switch (local_F4) {
        case 0:
          script_4BC4();
          break;
        case 1:
          script_49F5();
          break;
        case 90:
          script_2584();
          break;
        case 91:
          script_2B20();
          break;
        case 92:
          script_2CF1();
          break;
        case 93:
          script_341B();
          break;
        case 94:
          script_3D77();
          break;
        case 95:
          script_341B();
          script_2B20();
          break;
        case 96:
          script_3D77();
          script_2CF1();
          break;
      }
      SCNF_WaitFrames_0000(1);
    }
  }
  end:;
}

__attribute__((scn_string_bindings(0, data_72C2, 2, data_72D4, 4, data_72E6, 6, data_72F8, 8, data_730A, 10, data_731C, 12, data_732E, 14, data_7340, 16, data_7352, 18, data_7364, 20, data_7376)))
static void script_49F5(void) {
  script_4F38();
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 140.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 140.0f);
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 2, "BIKE");
  Script_AuthorUtils(0, 0, "seqdata2.bin", "BIKE");
  Script_AuthorUtils(0, 1, "seqdata3.bin", "BIKE");
  Script_AuthorUtils(0, 2, "seqdata4.bin", "BIKE");
  Script_AuthorUtils(0, 3, "seqdata5.bin", "BIKE");
  Script_AuthorUtils(0, 4, "seqdata6.bin", "BIKE");
  Script_AuthorUtils(0, 5, "seqdata7.bin", "BIKE");
  Script_AuthorUtils(0, 6, "seqdata8.bin", "BIKE");
  Script_AuthorUtils(0, 7, "seqdata9.bin", "BIKE");
  Script_AuthorUtils(0, 8, "seqdataA.bin", "BIKE");
  Script_AuthorUtils(0, 9, "seqdataB.bin", "BIKE");
  Script_CreateNPCImageModule(CHARA_AKIR, 0, 0);
  Script_CreateNPCImageModule(CHARA_AKIR, "YKG_M", 0);
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
  Script_PREP_Related(-1, 0);
  Script_SetupLightFromTable(1);
  Script_PREP_Related(-1, 1);
  script_081F();
  Script_CreateNPCImageModule(CHARA_AKIR, 0, 0);
  Script_CreateNPCImageModule(CHARA_AKIR, "YKB_M", 0);
  Script_executeAicaCommand(2720, 2, 60);
  Script_SetLampState(0, SHEN_NAME('BIKE'), 0);
  Script_AuthorUtils(AUTH_FREE, 0);
  Script_AuthorUtils(AUTH_FREE, 1);
  Script_AuthorUtils(AUTH_FREE, 2);
  Script_AuthorUtils(AUTH_FREE, 3);
  Script_AuthorUtils(AUTH_FREE, 4);
  Script_AuthorUtils(AUTH_FREE, 5);
  Script_AuthorUtils(AUTH_FREE, 6);
  Script_AuthorUtils(AUTH_FREE, 7);
  Script_AuthorUtils(AUTH_FREE, 8);
  Script_AuthorUtils(AUTH_FREE, 9);
  return;
}

__attribute__((scn_string_bindings(0, data_73A8, 1, data_73B6, 2, data_73BB, 3, data_73C0, 4, data_73C5, 5, data_73CA, 6, data_73CF, 9, data_73EC, 12, data_7406, 14, data_7414, 16, data_7422, 18, data_7430, 19, data_7435, 20, data_743A)))
static void script_4BC4(void) {
  int32_t unused_E4[7];
  script_512D();
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 15.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 15.0f);
  script_2479();
  Script_GetSetWeatherSettings(WEATHER_SETTINGS_SET_DISABLE_WEATHER, 0);
  Script_PREP_Related(-1, 0);
  Script_SetupLightFromTable(0);
  Script_PREP_Related(-1, 1);
  Script_GetCurrentDrawMode(0, 1);
  Script_GetCurrentDrawMode(1, 1);
  script_4E9B((int32_t)"scene/03/TERY", (int32_t)"TER2");
  while (task_78 == 1) {
    SCNF_WaitFrames_0000(1);
  }
  Script_LoadSND(0, 6, "FREE", 0);
  Script_LoadSND(0, 5, "FREE", 0);
  Script_LoadSND(0, 4, "FREE", 0);
  Script_LoadSND(0, 3, "FREE", 0);
  Script_LoadSND(0, 2, "FREE", 0);
  Script_LoadSND(0, 2, "bgm084.snd", 0);
  Script_LoadSND(0, 4, "a1_terry.snd", 0);
  Script_LoadVoiceFile(1, "01TAP");
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
  Script_SetFogEnabled(1);
  Script_SetFogColour(&data_7398);
  Script_SetFogFlags(0, data_7388);
  Script_SpriteUtils(Scroll_Read1, (int32_t)"sprite", "SCROLL25.SPR");
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 2, "TER2");
  Script_AuthorUtils(0, 11, "seqdataC", "TER2");
  Script_AuthorUtils(0, 12, "seqdataD", "TER2");
  Script_AuthorUtils(0, 13, "seqdataE", "TER2");
  Script_facemotUtils(CHARA_TERY, 0, 0);
  Script_facemotUtils(CHARA_CHAI, 0, 0);
  Script_facemotUtils(CHARA_HRSK, 0, 0);
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
  Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
  script_0E76(0, 11);
  script_1210(0, 12);
  Script_WeatherUtils_0(WeatherDataUtil_SetInterpolatedWeatherOverride, 1, 1, 30, 0);
  Script_WeatherUtils_0(WeatherDataUtil_SetInterpolatedWeatherOverride, 2, 1, 30, 0);
  script_191C(1, 13);
  if (Script_shenSeqUtils(-7) != 0) {
    Script_executeAicaCommand(2720, 2, 30);
    Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
    while (Script_filmCheckOrSetTransitionUtil(-1)) {
      SCNF_WaitFrames_0000(1);
    }
  }
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "TER2");
  Script_GetCurrentDrawMode(0, 0);
  Script_GetCurrentDrawMode(1, 0);
  Script_LoadVoiceFile(0, "01TAP");
  Script_AuthorUtils(AUTH_FREE, 11);
  Script_AuthorUtils(AUTH_FREE, 12);
  Script_AuthorUtils(AUTH_FREE, 13);
  return;
}

static void script_4E4D(void) {
  int32_t local_F4[3];
  for (;;) {
    local_F4[0] = -0x3CC10000;
    local_F4[1] = -0x3FC66666;
    local_F4[2] = -0x3D300000;
    Script_ClearAccumulator_00FE(1, 0x3F4CCCCD, 10, 2, 2);
    Script_ClearAccumulator_00FE(3, local_F4, 8, 9);
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_4E9B(int32_t arg0, int32_t arg1) {
  Script_scenCreateTask(script_4EB4, 2, arg0, arg1);
  task_78 = 1;
  return;
}

static int32_t script_4EB4(const char *arg0, const char *arg1) {
  Script_sprintf(" Load PackFile Start %s:%s\012", arg0, arg1);
  if (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 5, arg1)) {
    task_78 = 0;
    Script_srchControl(0);
  }
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, arg0, arg1) == 0) {
    Script_sprintf("Load PackFile Failed %s:%s\012", arg0, arg1);
    SCNF_WaitFrames_0000(1);
  }
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2)) {
    SCNF_WaitFrames_0000(1);
  }
  SCNF_WaitFrames_0000(1);
  task_78 = 0;
  return Script_srchControl(0);
}

static void script_4F38(void) {
  Script_mapmSetRenderFlag(0, 1);
  Script_mapmSetRenderFlag(1, 1);
  Script_mapmSetRenderFlag(2, 0);
  Script_mapmSetRenderFlag(3, 1);
  Script_mapmSetRenderFlag(4, 0);
  Script_mapmSetRenderFlag(5, 1);
  Script_mapmSetRenderFlag(6, 0);
  Script_mapmSetRenderFlag(7, 1);
  Script_mapmSetRenderFlag(8, 0);
  Script_mapmSetRenderFlag(9, 0);
  Script_mapmSetRenderFlag(10, 1);
  Script_mapmSetRenderFlag(11, 1);
  Script_mapmSetRenderFlag(12, 0);
  Script_mapmSetRenderFlag(13, 1);
  Script_mapmSetRenderFlag(14, 0);
  Script_mapmSetRenderFlag(15, 0);
  Script_mapmSetRenderFlag(16, 0);
  Script_mapmSetRenderFlag(17, 0);
  Script_mapmSetRenderFlag(18, 0);
  Script_mapmSetRenderFlag(19, 0);
  Script_mapmSetRenderFlag(20, 0);
  if (task_7C == 0) {
    Script_mapmSetRenderFlag(2, 1);
    Script_mapmSetRenderFlag(6, 1);
    Script_mapmSetRenderFlag(12, 1);
  } else {
    Script_mapmSetRenderFlag(4, 1);
    Script_mapmSetRenderFlag(8, 1);
    Script_mapmSetRenderFlag(14, 1);
    if (task_7C == 3) {
      Script_mapmSetRenderFlag(15, 1);
    }
  }
  return;
}

static void script_504E(void) {
  Script_mapmSetRenderFlag(0, 0);
  Script_mapmSetRenderFlag(1, 0);
  Script_mapmSetRenderFlag(2, 0);
  Script_mapmSetRenderFlag(3, 0);
  Script_mapmSetRenderFlag(4, 0);
  Script_mapmSetRenderFlag(5, 0);
  Script_mapmSetRenderFlag(6, 0);
  Script_mapmSetRenderFlag(7, 0);
  Script_mapmSetRenderFlag(8, 0);
  Script_mapmSetRenderFlag(9, 0);
  Script_mapmSetRenderFlag(10, 0);
  Script_mapmSetRenderFlag(11, 0);
  Script_mapmSetRenderFlag(12, 0);
  Script_mapmSetRenderFlag(13, 0);
  Script_mapmSetRenderFlag(14, 0);
  Script_mapmSetRenderFlag(15, 0);
  Script_mapmSetRenderFlag(16, 1);
  Script_mapmSetRenderFlag(17, 0);
  Script_mapmSetRenderFlag(18, 1);
  Script_mapmSetRenderFlag(19, 0);
  Script_mapmSetRenderFlag(20, 0);
  if (task_7C == 3) {
    Script_mapmSetRenderFlag(20, 1);
  } else {
    Script_mapmSetRenderFlag(19, 1);
  }
  return;
}

static void script_512D(void) {
  Script_mapmSetRenderFlag(0, 0);
  Script_mapmSetRenderFlag(1, 0);
  Script_mapmSetRenderFlag(2, 0);
  Script_mapmSetRenderFlag(3, 0);
  Script_mapmSetRenderFlag(4, 0);
  Script_mapmSetRenderFlag(5, 0);
  Script_mapmSetRenderFlag(6, 0);
  Script_mapmSetRenderFlag(7, 0);
  Script_mapmSetRenderFlag(8, 0);
  Script_mapmSetRenderFlag(9, 0);
  Script_mapmSetRenderFlag(10, 0);
  Script_mapmSetRenderFlag(11, 0);
  Script_mapmSetRenderFlag(12, 0);
  Script_mapmSetRenderFlag(13, 0);
  Script_mapmSetRenderFlag(14, 0);
  Script_mapmSetRenderFlag(15, 0);
  Script_mapmSetRenderFlag(16, 0);
  Script_mapmSetRenderFlag(17, 1);
  Script_mapmSetRenderFlag(18, 0);
  Script_mapmSetRenderFlag(19, 0);
  Script_mapmSetRenderFlag(20, 0);
  return;
}
