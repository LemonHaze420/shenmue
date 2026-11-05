#pragma once

#include "scenario.h"
#include "motion/motion.h"

static void script_0030(void);
static void script_0039(void);
static void script_0048(int32_t arg0);
static void script_00D5(int32_t arg0);
static int32_t script_0125(void);
static int32_t script_0153(int32_t arg0);
static void script_0181(void);
static int32_t script_01B5(int32_t arg0);
static void script_01E9(int32_t arg0);
static int32_t script_0207(int32_t arg0, int32_t arg1, int32_t arg2);
static int32_t script_0241(int32_t arg0, int32_t arg1);
static int32_t script_0275(void);
static int32_t script_036D(int32_t arg0, int32_t arg1);
static void script_039D(void);
static int32_t script_04CA(void);
static int32_t script_0855(void);
static int32_t script_0E3F(void);
static int32_t script_0F51(void);
static int32_t script_1558(void);
static int32_t script_2F29(void);
static void script_2F96(void);
static void script_3105(void);
static int32_t script_3172(void);
static int32_t script_337F(void);
static int32_t script_3ABB(void);
static int32_t script_3CED(void);
static int32_t script_3F5D(void);
static int32_t script_4160(void);
static int32_t script_43B6(void);
static int32_t script_46DE(void);
static int32_t script_49A9(void);
static int32_t script_4A9C(void);
static int32_t script_4BE1(void);
static void script_4C9D(void);
static int32_t script_4CF9(void);
static int32_t script_4E3D(void);
static int32_t script_4EF1(void);
static int32_t script_52D8(void);
static int32_t script_53BF(void);
static void script_54BD(void);
static int32_t script_5519(void);
static int32_t script_5726(void);
static int32_t script_608A(void);
static int32_t script_66FC(void);
static int32_t script_6A37(void);
static void script_6A54(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_6AA5(int32_t arg0, int32_t arg1, int32_t arg2);
static int32_t script_6B01(int32_t arg0, int32_t arg1);
static int32_t script_6B5D(int32_t arg0, int32_t arg1, int32_t arg2);
static int32_t script_6BF7(void);
static int32_t script_6DCD(void);
static void script_6EE1(int32_t arg0);
static void script_6F0F(int32_t arg0);
static int32_t script_6F3D(void);
static int32_t script_712B(void);
static int32_t script_71AA(void);
static void script_7255(void);
static int32_t script_72C2(void);
static int32_t script_74DF(void);
static int32_t script_78ED(void);
static int32_t script_792C(void);
static int32_t script_7969(void);
static void script_7A18(void);
static void script_7AA0(void);
static void script_7B28(void);
static void script_7C96(void);
static void script_7E04(void);
static void script_7F72(void);
static void script_80E0(void);
static void script_82EF(void);
static void script_84FE(void);
static void script_866C(void);
static void script_87DA(void);
static void script_89E9(void);
static void script_8BF8(void);
static void script_8D66(void);
static void script_8ED4(void);
static void script_9042(void);
static void script_9251(void);
static void script_93BF(void);
static void script_93C9(void);
static void script_9BD7(void);
static void script_A2BA(void);
static void script_B5BD(void);
static void script_BD33(void);
static void script_C164(void);
static void script_C806(void);
static int32_t script_CF5D(void);
static int32_t script_CFDA(void);
static int32_t script_D06C(void);
static int32_t script_D0B9(void);
static int32_t script_D1EE(void);
static void script_D23B(void);
static int32_t script_D288(void);
static int32_t script_D34D(void);
static int32_t script_D3B4(void);
static void script_D3DD(void);
static void script_D406(int32_t arg0);
static void script_D4DD(void);
static int32_t script_D57C(void);
void script_D610(void);
static int32_t script_D61E(void);

static int32_t task_0;
static int32_t task_4;
static int32_t task_8;
static int32_t task_C;
static int32_t task_10;
static int32_t task_14;
static int32_t task_18;
static int8_t task_1C;
static int8_t task_1D;
static int8_t task_1E;
static int8_t task_1F;
static int8_t task_20;
static int8_t task_21;
static int8_t task_22;
static int8_t task_23;
static int8_t task_24;
static int8_t task_25;
static int8_t task_26;
static int8_t task_27;
static int8_t task_28;
static int8_t task_29;
static int8_t task_2A;
static int8_t task_2B;
static int8_t task_2C;
static int8_t task_2D;
static int8_t task_2E;
static int8_t task_2F;
static int8_t task_30;
static uint8_t task_31;
static uint16_t task_32;
static int32_t task_34;
static int32_t task_38;
static int32_t task_3C;
typedef struct Row_D720 { float w0; float w1; float w2; int32_t w3; int32_t w4; int32_t w5; int32_t w6; } Row_D720;
static const Row_D720 data_D720[9] = {
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
static const int32_t data_D81C[5] = {0, 0, 0, 0, 0};
static const int32_t data_D830[57] = {
  0, 0, -10922, 0, 0, -10558, 0, 1456, 0, 0, 0, -18022, 0, 0, -17112, 0, 0, -18386, 0, 0, 0, 0, 0, -12196, 0, 0, -18204, 0, 0, -16748, 0, 0, 0, 0, 0,
  -11832, 0, 0, -18386, 0, 0, -15109, 0, 0, 0, 0, 910, -12014, 0, -2184, -16748, 0, -2184, -10012, 0, 0, 0
};
static const int32_t data_D914[57] = {
  0, -364, -11650, -2912, -364, -4369, 1638, 910, 182, 0, 0, -16019, 0, 182, -23119, 0, 546, -18932, 0, 0, -1092, 0, 0, -16384, 0, 0, -21845, 0, 0,
  -18750, 0, 0, -546, 0, 0, -21481, 0, 0, -21299, 0, 0, -18568, 0, 0, -728, 0, 910, -18750, 0, 182, -18750, 0, -4004, -18204, 0, 0, -728
};
static const int32_t data_D9F8[228] = {
  0, -364, -14017, -2912, -364, -6735, 1638, 910, 2548, 0, 0, -18568, 0, 182, -25668, 0, 546, -21481, 0, 0, -3640, 0, 0, -18932, 0, 0, -24393, 0, 0,
  -21299, 0, 0, -3094, 0, 0, -24029, 0, 0, -23847, 0, 0, -21117, 0, 0, -3276, 0, 910, -21299, 0, 182, -21299, 0, -4004, -20753, 0, 0, -3276, 0, 0,
  -5461, 0, -2366, -1092, 0, -182, 1092, 0, -546, -6735, 0, 0, -4004, 0, 0, -2912, 0, -182, 1274, 0, 0, -13289, 0, 0, -5279, 0, 0, -8009, 0, 0, 1456,
  0, 182, -8738, 0, 0, -6007, 0, 0, -8738, 0, 182, 1638, 0, -728, -5279, 0, -1274, -6735, 0, -910, -8738, 0, 0, 1638, 0, 0, -5461, 0, -2366, -1092, 0,
  -182, 1092, 0, -546, -13289, 0, 0, -5643, 0, 0, -8009, 0, -182, 1274, 0, 0, -11832, 0, 0, -5279, 0, 0, -8009, 0, 0, 1456, 0, 182, -8738, 0, 0,
  -6007, 0, 0, -8738, 0, 182, 1638, 0, -728, -5279, 0, -1274, -6735, 0, -910, -8738, 0, 0, 1092, 0, 0, -5461, 0, -2366, -1092, 0, -182, 1092, 0, -546,
  -3640, 0, 0, -2366, 0, 0, -1456, 0, -182, 1274, 0, 0, -3822, 0, 0, -2730, 0, 0, -1456, 0, 0, 1456, 0, 182, -3822, 0, 0, -2912, 0, 0, -1638, 0, 182,
  1638, 0, -728, -3640, 0, -910, -2730, 0, -546, -1638, 0, 0, 1092
};
static const int32_t data_DD88[285] = {
  0, 0, -5461, 0, -2366, -1092, 0, -182, 1092, 0, -546, -6735, 0, 0, -4004, 0, 0, -6553, 0, -182, 1274, 0, 0, -13289, 0, 0, -5279, 0, 0, -8009, 0, 0,
  1456, 0, 182, -8738, 0, 0, -6007, 0, 0, -8738, 0, 182, 1638, 0, -728, -5279, 0, -1274, -6735, 0, -910, -8738, 0, 0, 1638, 0, 0, -9102, 0, -2366,
  -2002, 0, -182, 910, 0, -546, -2366, 0, 0, -2002, 0, 0, -2912, 0, -182, 1274, 0, 0, -14199, 0, 0, -11468, 0, 0, -13653, 0, 0, 1456, 0, 182, -11286,
  0, 0, -12014, 0, 0, -12743, 0, 182, 1638, 0, -728, -10194, 0, -1274, -12196, 0, -910, -10922, -728, 546, 1638, 0, 0, -1820, 0, -2366, -728, 0, -364,
  -1092, 0, -546, -11832, 0, 910, -6735, 0, 0, -6553, 0, -182, 1274, 0, 0, -13289, 0, 0, -6007, 0, 0, -10012, 0, 0, 1092, 0, 182, -8738, 0, 0, -6735,
  0, 0, -9830, 0, 182, 1638, 0, -728, -6553, 0, -1274, -7281, 0, -910, -10740, 0, 0, 1638, 0, 0, -5461, -2912, -2366, -364, -5279, 2548, -910, 0,
  -546, -8374, 0, 0, -9466, 0, 910, -4915, 0, -182, 546, 0, 0, -13289, 0, 0, -5279, 0, 364, -7645, 0, -182, 1456, 0, 182, -8738, 0, 0, -7281, 0, 182,
  -6917, 0, -182, 1638, 0, -728, -5279, 0, -1274, -6735, 0, -182, -8009, 0, 0, 1638, -4733, 0, -8556, 0, -2366, -1092, -6189, 2912, 0, 0, -546,
  -10922, 0, 1820, -16384, 0, 0, -11468, 0, 182, 546, 0, 0, -10376, 0, 0, -15837, 0, 0, -12743, 0, -182, 728, 0, 182, -13835, 0, 0, -16201, 0, 364,
  -10922, 0, -182, 728, 0, -728, -9648, 0, -3458, -12379, 0, -728, -7463, 0, 0, 1092
};
static const int32_t data_E1FC[57] = {
  0, 0, -5461, 0, -2366, -1638, 0, -910, 1092, 0, -546, -3640, 0, 0, -2366, 0, 0, -1456, 0, -182, 1638, 0, 0, -910, 0, 0, -2730, 0, 0, -1274, 0, 0,
  1820, 0, 182, -2730, 0, 0, -1820, 0, 0, -1274, 0, 182, 2184, 0, -728, -3094, 0, -910, -2548, 0, 910, -1456, -546, 910, 2366
};
static const uint32_t data_E2E0 = 0x41C00000;
static const float data_E2E4[2] = {0.0f, -33.0f};
static const uint32_t data_E2EC = 0x41A00000;
static const float data_E2F0[2] = {0.0f, 18.0f};
static const uint32_t data_E2F8 = 0xC1B80000;
static const float data_E2FC[2] = {0.0f, 13.0f};
static const uint32_t data_E304 = 0x40E00000;
static const float data_E308[2] = {0.0f, 12.0f};
static const uint8_t data_E310[8] = {0};
static const float data_E318 = -35.0f;
static const uint8_t data_E31C[8] = {0};
static const float data_E324 = -35.0f;
static const uint32_t data_E328 = 0x42300000;
static const float data_E32C[2] = {0.0f, -24.0f};
static const uint32_t data_E334 = 0xC1A00000;
static const float data_E338[2] = {0.0f, 20.0f};
static const uint32_t data_E340 = 0x3F800000;
static const float data_E344[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.8f, 0.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint32_t data_E3C8 = 0x3F800000;
static const float data_E3CC[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.8f, 0.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint32_t data_E450 = 0x3F800000;
static const float data_E454[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.8f, 0.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint32_t data_E4D8 = 0x3F800000;
static const float data_E4DC[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint8_t data_E560[80] = {0};
typedef struct Row_E5B0 { float w0; float w1; float w2; int32_t w3; } Row_E5B0;
static const Row_E5B0 data_E5B0[3] = {
  {0.8f, 0.8f, 0.8f, 0},
  {0.0f, 0.0f, 0.0f, 0},
  {0.0f, 0.0f, 0.0f, 0}
};
static const int32_t data_E5E0[2] = {0, 0};
static const uint8_t data_E5E8[96] = {0};
static const float data_E648[10] = {1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f};
static const uint32_t data_E670 = 0x3F800000;
static const float data_E674[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint32_t data_E6F8 = 0x3F800000;
static const float data_E6FC[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const Vector3 data_E780 = {.x = 9.3f, .y = -0.08f, .z = 11.0f};
static const uint32_t data_E78C = 1;
typedef struct Row_E790 { float w0; float w1; float w2; int32_t w3; } Row_E790;
static const Row_E790 data_E790[7] = {
  {9.2f, 7.2f, 0.4f, 1},
  {8.6f, 6.6f, 0.4f, 1},
  {6.6f, 6.8f, 0.4f, 1},
  {5.4f, 7.6f, 0.4f, 1},
  {5.4f, 8.6f, 0.4f, 1},
  {6.2f, 9.0f, 0.4f, 1},
  {8.2f, 9.2f, 0.4f, 196608}
};
static const uint32_t data_E800 = 1;
typedef struct Row_E804 { float w0; float w1; float w2; int32_t w3; } Row_E804;
static const Row_E804 data_E804[4] = {
  {9.2f, 8.2f, 0.6f, 1},
  {8.3f, 6.9f, 0.6f, 1},
  {7.3f, 7.1f, 0.8f, 1},
  {7.6f, 7.9f, 0.6f, 196608}
};
static const uint32_t data_E844 = 1;
typedef struct Row_E848 { float w0; float w1; float w2; int32_t w3; } Row_E848;
static const Row_E848 data_E848[4] = {
  {5.7f, 7.6f, 0.4f, 1},
  {6.1f, 7.1f, 0.4f, 1},
  {6.8f, 6.6f, 0.4f, 1},
  {7.6f, 7.2f, 0.4f, 196608}
};
static const uint32_t data_E888 = 1;
typedef struct Row_E88C { float w0; float w1; float w2; int32_t w3; } Row_E88C;
static const Row_E88C data_E88C[5] = {
  {6.5f, 9.2f, 0.6f, 1},
  {5.7f, 8.5f, 1.0f, 1},
  {6.3f, 7.4f, 1.0f, 1},
  {9.0f, 6.3f, 1.0f, 1},
  {8.5f, 8.2f, 1.0f, 196608}
};
static const uint32_t data_E8DC = 1;
typedef struct Row_E8E0 { float w0; float w1; float w2; int32_t w3; } Row_E8E0;
static const Row_E8E0 data_E8E0[5] = {
  {9.4f, 6.8f, 1.0f, 1},
  {8.1f, 6.5f, 1.0f, 1},
  {7.1f, 7.3f, 1.0f, 1},
  {7.3f, 8.4f, 1.0f, 1},
  {7.2f, 9.4f, 1.0f, 196608}
};
static const uint32_t data_E930 = 1;
typedef struct Row_E934 { float w0; float w1; float w2; int32_t w3; } Row_E934;
static const Row_E934 data_E934[4] = {
  {7.1f, 7.5f, 1.2f, 1},
  {5.8f, 7.4f, 1.3f, 1},
  {4.7f, 8.1f, 1.3f, 1},
  {4.5f, 8.6f, 1.3f, 196608}
};
static const int32_t data_E974[57] = {
  0, 0, -5461, 9284, -3094, -1092, 0, -1092, 1092, 0, -546, -7281, 0, 0, -3276, 0, 0, -4733, 0, -182, 1274, 0, 0, -11468, 0, 0, -4733, 0, 0, -3276, 0,
  0, 1456, 0, 182, -5097, 0, 0, -4187, 0, 0, -3276, 0, 182, 1638, 0, 0, -7099, 0, -728, -5279, 0, -1274, -2548, 0, 546, 1638
};
static const FogColourData data_EA58 = {.red = 0.1f, .green = 190.0f, .blue = 0.9f, .colour = 0xFF515767U};
static const uint32_t data_EA68[4] = {0xFF202622U, 0xFF202622U, 0xA7787C74U, 0xA7787C74U};
static const Vector4 data_EA78 = {.x = 0.3f, .y = 0.3f, .z = 0.3f, .w = 0.3f};
static const Vector4 data_EA88 = {.x = 1.0f, .y = 0.82f, .z = 0.82f, .w = 1.0f};
static const Vector3 data_EA98 = {.x = -0.85f, .y = -0.51f, .z = -0.13f};
static const FogColourData data_EAA4 = {.red = 0.1f, .green = 190.0f, .blue = 0.9f, .colour = 0xFF515767U};
static const uint32_t data_EAB4[4] = {0xFF202622U, 0xFF202622U, 0xA7787C74U, 0xA7787C74U};
static const Vector4 data_EAC4 = {.x = 0.3f, .y = 0.3f, .z = 0.3f, .w = 0.3f};
static const float data_EAD4[2] = {0.96f, 1.0f};
static const FogColourData data_EADC = {.red = 1.1f, .green = 65.0f, .blue = 1.0f, .colour = 0xFF3F4D67U};
static const uint32_t data_EAEC[4] = {0xFF202622U, 0xFF202622U, 0xA7787C74U, 0xA7787C74U};
static const Vector4 data_EAFC = {.x = 0.3f, .y = 0.3f, .z = 0.3f, .w = 0.3f};
static const Vector4 data_EB0C = {.x = 1.0f, .y = 0.88f, .z = 0.87f, .w = 1.0f};
static const Vector3 data_EB1C = {.x = 0.75f, .y = -0.73f, .z = 1.0f};
static const int32_t data_EB28[57] = {
  0, 182, -6007, 0, 5461, 6844, 0, 728, 0, 0, 0, -6116, 0, 0, -2257, 0, 0, 218, 0, 0, 364, 0, 0, -5570, 0, 0, -1401, 0, 0, -254, 0, 0, 0, 0, 0, -5843,
  0, 0, -3222, 0, 0, 473, 0, 509, -655, 0, 0, -7099, 0, 0, -1019, 0, 0, -182, 0, -309, -837
};
static const int32_t data_EC0C[57] = {
  0, 182, -6007, 0, 5461, 6844, 0, 728, -910, 0, 0, -6116, 0, 0, -3167, 0, 0, -873, 0, 0, 0, 0, 0, -5570, 0, 0, -1401, 0, 0, -254, 0, 0, 0, 0, 0,
  -5843, 0, 0, -3222, 0, 0, 473, 0, 509, -655, 0, 0, -7099, 0, 0, -1019, 0, 0, -182, 0, -309, -837
};
static const int32_t data_ECF0[57] = {
  0, 1274, -13107, 0, -1456, -655, 0, -546, -728, 0, -546, -7099, 0, 0, -6553, 0, 1092, 1456, 0, -182, 728, 0, 0, -9284, 0, 0, -8192, 0, -364, 0, 0,
  182, 2002, 0, 182, -10740, 0, 0, -4132, 0, 0, -728, 0, -182, 1820, 0, -728, -9648, 0, 0, -5206, -1092, -364, -1638, -2002, -182, 1092
};
static const int32_t data_EDD4[57] = {
  0, 0, -5461, 0, -2366, -1638, 0, -182, 1092, 0, -546, -4551, 0, 0, -1456, 0, 0, -2912, 0, -182, 1456, 0, 0, -3458, 0, 0, -3458, 0, 0, -364, 0, 0,
  1638, 0, 182, -3458, 0, 0, -3640, 0, 0, -364, 0, 182, 2184, 0, -1638, -2548, 0, -1274, -2730, 0, 910, -1456, 0, 0, 2002
};
static const int32_t data_EEB8[57] = {
  0, 0, -5461, 0, -2366, -1638, 0, -182, 1092, 0, -546, -4551, 0, 0, -728, 0, 2002, 3458, 0, -182, 1456, 0, 0, -2002, 0, 0, -5097, 0, 182, 1638, 0, 0,
  1638, 0, 182, -4915, 0, 0, -2548, 0, -728, 0, 0, 182, 2184, 0, -1638, -2548, 0, -1274, -2730, 0, 364, 182, 0, 0, 2002
};
static const FogColourData data_EF9C = {.red = 0.1f, .green = 150.0f, .blue = 1.04f, .colour = 0xFF17191BU};
static const Vector4 data_EFAC = {.x = 0.27f, .y = 0.27f, .z = 0.27f, .w = 0.27f};
static const float data_EFBC[4] = {0.76f, 1.0f, 1.0f, 1.0f};
static const FogColourData data_EFCC = {.red = 0.0001f, .green = 75.0f, .blue = 1.16f, .colour = 0xFF868480U};
static const FogColourData data_EFDC = {.red = 2.1f, .green = 245.0f, .blue = 1.2f, .colour = 0xFF171916U};
static const Vector4 data_EFEC = {.x = 0.3f, .y = 0.3f, .z = 0.3f, .w = 0.3f};
static const Vector4 data_EFFC = {.x = 1.0f, .y = 1.0f, .z = 1.0f, .w = 1.0f};
static const Vector3 data_F00C = {.x = -0.13f, .y = -1.0f, .z = 1.0f};
static const uint32_t data_F018[4] = {0xB0000000U, 0xB0000000U, 0xFF000000U, 0xFF000000U};
static const Vector4 data_F028 = {.x = 0.22f, .y = 0.22f, .z = 0.22f, .w = 0.22f};
static const Vector4 data_F038 = {.x = 1.0f, .y = 0.52f, .z = 0.55f, .w = 0.64f};
static const Vector3 data_F048 = {.x = -0.13f, .y = -1.0f, .z = 1.0f};
static const Vector4 data_F054 = {.x = 1.0f, .y = 0.16f, .z = 0.12f, .w = 0.2f};
static const Vector4 data_F064 = {.x = 1.0f, .y = 0.16f, .z = 0.12f, .w = 0.2f};
static const int32_t data_F074[57] = {
  0, 0, -5461, 0, -2366, -1092, 0, -182, 1092, 0, -546, -3276, 0, 364, -2366, 0, 728, -4915, 0, -182, 1274, 0, 0, -3276, 0, 0, -2548, 0, 0, -5825, 0,
  0, 1456, 0, 182, -8738, 0, 0, -2548, 0, 0, -5825, 0, 182, 1638, 0, -728, -5279, 0, -1274, -3822, 0, -910, -6735, 0, 0, 1638
};
static const FogColourData data_F158 = {.red = 0.1f, .green = 130.0f, .blue = 0.62f, .colour = 0xFF4F5963U};
static const uint32_t data_F168[4] = {0x98122622U, 0x98122622U, 0x78728074U, 0x78728074U};
static const Vector4 data_F178 = {.x = 0.19f, .y = 0.26f, .z = 0.32f, .w = 0.38f};
static const Vector4 data_F188 = {.x = 1.0f, .y = 0.81f, .z = 0.96f, .w = 0.89f};
static const Vector3 data_F198 = {.x = -0.5f, .y = -0.2f, .z = -1.0f};
static const FogColourData data_F1A4 = {.red = 0.1f, .green = 190.0f, .blue = 1.3f, .colour = 0xFF312B3FU};
static const uint32_t data_F1B4[4] = {0xFF202622U, 0xFF202622U, 0xA7787C74U, 0xA7787C74U};
static const Vector4 data_F1C4 = {.x = 0.18f, .y = 0.13f, .z = 0.18f, .w = 0.28f};
static const Vector4 data_F1D4 = {.x = 1.0f, .y = 0.42f, .z = 0.44f, .w = 0.45f};
static const Vector3 data_F1E4 = {.x = 0.38f, .y = -0.47f, .z = 0.45f};
static const Vector4 data_F1F0 = {.x = 1.0f, .y = 0.04f, .z = 0.02f, .w = 0.08f};
static const FogColourData data_F200 = {.red = 0.1f, .green = 190.0f, .blue = 1.3f, .colour = 0xFF312B3FU};
static const uint32_t data_F210[4] = {0xFF202622U, 0xFF202622U, 0xA7787C74U, 0xA7787C74U};
static const Vector4 data_F220 = {.x = 0.18f, .y = 0.13f, .z = 0.18f, .w = 0.28f};
static const Vector4 data_F230 = {.x = 1.0f, .y = 0.42f, .z = 0.44f, .w = 0.45f};
static const Vector3 data_F240 = {.x = 0.38f, .y = -0.47f, .z = 0.45f};
static const Vector4 data_F24C = {.x = 1.0f, .y = 0.04f, .z = 0.02f, .w = 0.08f};
static const FogColourData data_F25C = {.red = 0.1f, .green = 300.0f, .blue = 2.65f, .colour = 0xFF000000U};
static const uint32_t data_F26C[4] = {0xFF000000U, 0xFF000000U, 0xFF000000U, 0xFF000000U};
static const Vector4 data_F27C = {.x = 0.13f, .y = 0.13f, .z = 0.13f, .w = 0.13f};
static const FogColourData data_F28C = {.red = 0.1f, .green = 300.0f, .blue = 2.65f, .colour = 0xFF000000U};
static const uint32_t data_F29C[4] = {0xFF000000U, 0xFF000000U, 0xFF000000U, 0xFF000000U};
static const Vector4 data_F2AC = {.x = 0.11f, .y = 0.11f, .z = 0.11f, .w = 0.11f};
static const FogColourData data_F2BC = {.red = 0.1f, .green = 300.0f, .blue = 2.65f, .colour = 0xFF000000U};
static const uint32_t data_F2CC[4] = {0xFF000000U, 0xFF000000U, 0xFF000000U, 0xFF000000U};
static const Vector4 data_F2DC = {.x = 0.1f, .y = 0.1f, .z = 0.1f, .w = 0.1f};
static const FogColourData data_F2EC = {.red = 0.1f, .green = 300.0f, .blue = 2.65f, .colour = 0xFF000000U};
static const uint32_t data_F2FC[4] = {0xFF000000U, 0xFF000000U, 0xFF000000U, 0xFF000000U};
static const Vector4 data_F30C = {.x = 0.14f, .y = 0.14f, .z = 0.14f, .w = 0.14f};
static const FogColourData data_F31C = {.red = 0.1f, .green = 300.0f, .blue = 2.65f, .colour = 0xFF000000U};
static const uint32_t data_F32C[4] = {0xFF000000U, 0xFF000000U, 0xFF000000U, 0xFF000000U};
static const Vector4 data_F33C = {.x = 0.42f, .y = 0.42f, .z = 0.42f, .w = 0.42f};
static const FogColourData data_F34C = {.red = 0.1f, .green = 300.0f, .blue = 2.65f, .colour = 0xFF000000U};
static const uint32_t data_F35C[4] = {0xFF000000U, 0xFF000000U, 0xFF000000U, 0xFF000000U};
static const Vector4 data_F36C = {.x = 0.2f, .y = 0.2f, .z = 0.2f, .w = 0.2f};
static const FogColourData data_F37C = {.red = 0.1f, .green = 300.0f, .blue = 2.65f, .colour = 0xFF000000U};
static const uint32_t data_F38C[4] = {0xFF000000U, 0xFF000000U, 0xFF000000U, 0xFF000000U};
static const Vector4 data_F39C = {.x = 0.09f, .y = 0.09f, .z = 0.09f, .w = 0.09f};
static const FogColourData data_F3AC = {.red = 0.1f, .green = 300.0f, .blue = 2.65f, .colour = 0xFF000000U};
static const uint32_t data_F3BC[4] = {0xFF000000U, 0xFF000000U, 0xFF000000U, 0xFF000000U};
static const Vector4 data_F3CC = {.x = 0.25f, .y = 0.25f, .z = 0.25f, .w = 0.25f};
static const FogColourData data_F3DC = {.red = 0.1f, .green = 300.0f, .blue = 2.65f, .colour = 0xFF000000U};
static const uint32_t data_F3EC[4] = {0xFF000000U, 0xFF000000U, 0xFF000000U, 0xFF000000U};
static const Vector4 data_F3FC = {.x = 0.13f, .y = 0.13f, .z = 0.13f, .w = 0.13f};
static const FogColourData data_F40C = {.red = 0.1f, .green = 300.0f, .blue = 2.65f, .colour = 0xFF000000U};
static const uint32_t data_F41C[4] = {0xFF000000U, 0xFF000000U, 0xFF000000U, 0xFF000000U};
static const Vector4 data_F42C = {.x = 0.15f, .y = 0.15f, .z = 0.15f, .w = 0.15f};
static const FogColourData data_F43C = {.red = 0.1f, .green = 300.0f, .blue = 2.65f, .colour = 0xFF000000U};
static const uint32_t data_F44C[4] = {0xFF000000U, 0xFF000000U, 0xFF000000U, 0xFF000000U};
static const Vector4 data_F45C = {.x = 0.38f, .y = 0.38f, .z = 0.38f, .w = 0.38f};
static const FogColourData data_F46C = {.red = 0.1f, .green = 300.0f, .blue = 2.65f, .colour = 0xFF000000U};
static const uint32_t data_F47C[4] = {0xFF000000U, 0xFF000000U, 0xFF000000U, 0xFF000000U};
static const Vector4 data_F48C = {.x = 0.38f, .y = 0.38f, .z = 0.38f, .w = 0.38f};
static const FogColourData data_F49C = {.red = 0.1f, .green = 300.0f, .blue = 2.65f, .colour = 0xFF000000U};
static const uint32_t data_F4AC[4] = {0xFF000000U, 0xFF000000U, 0xFF000000U, 0xFF000000U};
static const Vector4 data_F4BC = {.x = 0.26f, .y = 0.26f, .z = 0.26f, .w = 0.26f};
static const FogColourData data_F4CC = {.red = 0.1f, .green = 300.0f, .blue = 2.65f, .colour = 0xFF000000U};
static const uint32_t data_F4DC[4] = {0xFF000000U, 0xFF000000U, 0xFF000000U, 0xFF000000U};
static const Vector4 data_F4EC = {.x = 0.15f, .y = 0.15f, .z = 0.15f, .w = 0.15f};
static const FogColourData data_F4FC = {.red = 0.1f, .green = 300.0f, .blue = 2.65f, .colour = 0xFF000000U};
static const uint32_t data_F50C[4] = {0xFF000000U, 0xFF000000U, 0xFF000000U, 0xFF000000U};
static const Vector4 data_F51C = {.x = 0.13f, .y = 0.13f, .z = 0.13f, .w = 0.13f};
static const char data_F52C[14] __attribute__((scn_string_literal)) = "scene/01/OP00";
static const char data_F53A[9] __attribute__((scn_string_literal)) = "OP99.afs";
static const char data_F543[12] __attribute__((scn_string_literal)) = "M_0101A.BIN";
static const char data_F54F[9] __attribute__((scn_string_literal)) = "M_UO.BIN";
static const char data_F558[13] __attribute__((scn_string_literal)) = "HMOT0102.BIN";
static const char data_F565[12] __attribute__((scn_string_literal)) = "M_01103.BIN";
static const char data_F571[11] __attribute__((scn_string_literal)) = "M_0119.BIN";
static const char data_F57C[6] __attribute__((scn_string_literal)) = "A0114";
static const char data_F582[6] __attribute__((scn_string_literal)) = "OPEN1";
static const char data_F588[13] __attribute__((scn_string_literal)) = "model/object";
static const char data_F595[12] __attribute__((scn_string_literal)) = "trlake1.pvr";
static const char data_F5A1[13] __attribute__((scn_string_literal)) = "model/object";
static const char data_F5AE[13] __attribute__((scn_string_literal)) = "trsea025.mt6";
static const char data_F5BB[13] __attribute__((scn_string_literal)) = "model/object";
static const char data_F5C8[12] __attribute__((scn_string_literal)) = "grid025.bin";
static const char data_F5D4[17] __attribute__((scn_string_literal)) = "OP99.afs#002#003";
static const char data_F5E5[17] __attribute__((scn_string_literal)) = "OP99.afs#030#031";
static const char data_F5F6[17] __attribute__((scn_string_literal)) = "OP99.afs#000#001";
static const char data_F607[17] __attribute__((scn_string_literal)) = "OP99.afs#028#029";
static const char data_F618[17] __attribute__((scn_string_literal)) = "OP99.afs#008#009";
static const char data_F629[17] __attribute__((scn_string_literal)) = "OP99.afs#000#001";
static const char data_F63A[17] __attribute__((scn_string_literal)) = "OP99.afs#002#003";
static const char data_F64B[17] __attribute__((scn_string_literal)) = "OP99.afs#004#005";
static const char data_F65C[17] __attribute__((scn_string_literal)) = "OP99.afs#006#007";
static const char data_F66D[17] __attribute__((scn_string_literal)) = "OP99.afs#008#009";
static const char data_F67E[17] __attribute__((scn_string_literal)) = "OP99.afs#010#011";
static const char data_F68F[17] __attribute__((scn_string_literal)) = "OP99.afs#012#013";
static const char data_F6A0[17] __attribute__((scn_string_literal)) = "OP99.afs#014#015";
static const char data_F6B1[17] __attribute__((scn_string_literal)) = "OP99.afs#016#017";
static const char data_F6C2[17] __attribute__((scn_string_literal)) = "OP99.afs#018#019";
static const char data_F6D3[17] __attribute__((scn_string_literal)) = "OP99.afs#020#021";
static const char data_F6E4[17] __attribute__((scn_string_literal)) = "OP99.afs#022#023";
static const char data_F6F5[17] __attribute__((scn_string_literal)) = "OP99.afs#024#025";
static const char data_F706[17] __attribute__((scn_string_literal)) = "OP99.afs#026#027";
static const char data_F717[17] __attribute__((scn_string_literal)) = "OP99.afs#028#029";
static const char data_F728[20] __attribute__((scn_string_literal)) = "OP99.afs#030#031";
static const uint32_t data_F73C[4] = {0xFF001244U, 0xFF001244U, 0xD7361A2EU, 0xD7361A2EU};
static const uint32_t data_F74C[4] = {0x28000000U, 0x28000000U, 0xDD000000U, 0xDD000000U};
static const char data_F75C[6] __attribute__((scn_string_literal)) = "OPEN2";
static const char data_F762[17] __attribute__((scn_string_literal)) = "OP99.afs#012#013";
static const char data_F773[17] __attribute__((scn_string_literal)) = "OP99.afs#010#011";
static const char data_F784[17] __attribute__((scn_string_literal)) = "OP99.afs#014#015";
static const char data_F795[17] __attribute__((scn_string_literal)) = "OP99.afs#000#001";
static const char data_F7A6[17] __attribute__((scn_string_literal)) = "OP99.afs#002#003";
static const char data_F7B7[17] __attribute__((scn_string_literal)) = "OP99.afs#004#005";
static const char data_F7C8[17] __attribute__((scn_string_literal)) = "OP99.afs#006#007";
static const char data_F7D9[17] __attribute__((scn_string_literal)) = "OP99.afs#008#009";
static const char data_F7EA[17] __attribute__((scn_string_literal)) = "OP99.afs#010#011";
static const char data_F7FB[17] __attribute__((scn_string_literal)) = "OP99.afs#012#013";
static const char data_F80C[17] __attribute__((scn_string_literal)) = "OP99.afs#014#015";
static const char data_F81D[17] __attribute__((scn_string_literal)) = "OP99.afs#016#017";
static const char data_F82E[17] __attribute__((scn_string_literal)) = "OP99.afs#018#019";
static const char data_F83F[17] __attribute__((scn_string_literal)) = "OP99.afs#020#021";
static const char data_F850[17] __attribute__((scn_string_literal)) = "OP99.afs#022#023";
static const char data_F861[17] __attribute__((scn_string_literal)) = "OP99.afs#024#025";
static const char data_F872[17] __attribute__((scn_string_literal)) = "OP99.afs#026#027";
static const char data_F883[17] __attribute__((scn_string_literal)) = "OP99.afs#028#029";
static const char data_F894[17] __attribute__((scn_string_literal)) = "OP99.afs#030#031";
static const char data_F8A5[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_F8AA[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_F8AF[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_F8B4[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_F8B9[11] __attribute__((scn_string_literal)) = "amb036.snd";
static const char data_F8C4[11] __attribute__((scn_string_literal)) = "bgm049.snd";
static const char data_F8CF[13] __attribute__((scn_string_literal)) = "a1_tkine.snd";
static const char data_F8DC[7] __attribute__((scn_string_literal)) = "A01103";
static const char data_F8E3[17] __attribute__((scn_string_literal)) = "OP99.afs#000#001";
static const char data_F8F4[17] __attribute__((scn_string_literal)) = "OP99.afs#020#021";
static const char data_F905[17] __attribute__((scn_string_literal)) = "OP99.afs#026#027";
static const char data_F916[17] __attribute__((scn_string_literal)) = "OP99.afs#022#023";
static const char data_F927[17] __attribute__((scn_string_literal)) = "OP99.afs#000#001";
static const char data_F938[17] __attribute__((scn_string_literal)) = "OP99.afs#002#003";
static const char data_F949[17] __attribute__((scn_string_literal)) = "OP99.afs#004#005";
static const char data_F95A[17] __attribute__((scn_string_literal)) = "OP99.afs#006#007";
static const char data_F96B[17] __attribute__((scn_string_literal)) = "OP99.afs#008#009";
static const char data_F97C[17] __attribute__((scn_string_literal)) = "OP99.afs#010#011";
static const char data_F98D[17] __attribute__((scn_string_literal)) = "OP99.afs#012#013";
static const char data_F99E[17] __attribute__((scn_string_literal)) = "OP99.afs#014#015";
static const char data_F9AF[17] __attribute__((scn_string_literal)) = "OP99.afs#016#017";
static const char data_F9C0[17] __attribute__((scn_string_literal)) = "OP99.afs#018#019";
static const char data_F9D1[17] __attribute__((scn_string_literal)) = "OP99.afs#020#021";
static const char data_F9E2[17] __attribute__((scn_string_literal)) = "OP99.afs#022#023";
static const char data_F9F3[17] __attribute__((scn_string_literal)) = "OP99.afs#024#025";
static const char data_FA04[17] __attribute__((scn_string_literal)) = "OP99.afs#026#027";
static const char data_FA15[17] __attribute__((scn_string_literal)) = "OP99.afs#028#029";
static const char data_FA26[17] __attribute__((scn_string_literal)) = "OP99.afs#030#031";
static const char data_FA37[6] __attribute__((scn_string_literal)) = "A0119";
static const char data_FA3D[13] __attribute__((scn_string_literal)) = "a1_unasa.snd";
static const char data_FA4A[11] __attribute__((scn_string_literal)) = "bgm120.snd";
static const char data_FA55[11] __attribute__((scn_string_literal)) = "tsm006.snd";
static const Vector4 data_FA60 = {.x = 1.0f, .y = 0.16f, .z = 0.12f, .w = 0.2f};
