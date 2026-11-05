#pragma once

#include "scenario.h"
#include "motion/motion.h"

static void script_0030(void);
static void script_0168(int32_t arg0);
static void script_0174(int32_t arg0);
static void script_0180(int32_t arg0);
static void script_0295(int32_t arg0);
static void script_02AC(int32_t arg0);
static void script_02ED(int32_t arg0);
static void script_0331(int32_t arg0);
static void script_03AF(void);
static void script_042E(void);
static void script_0437(void);
static void script_0446(int32_t arg0);
static void script_04D3(int32_t arg0);
static void script_0523(void);
static void script_0551(int32_t arg0);
static void script_057F(void);
static void script_05B3(int32_t arg0);
static void script_05E7(int32_t arg0);
static void script_0605(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_063F(int32_t arg0, int32_t arg1);
static void script_0673(void);
static void script_076B(int32_t arg0, int32_t arg1);
static void script_079B(int32_t arg0, float arg1, float arg2, float arg3);
static void script_07C3(int32_t arg0, float arg1, float arg2, float arg3);
static void script_0817(int32_t arg0, int32_t arg1, int32_t arg2);
static int32_t script_0867(int32_t arg0);
static void script_090E(int32_t arg0, int32_t arg1);
static int32_t script_0931(int32_t arg0);
static void script_0964(int32_t arg0, int32_t arg1);
static void script_0987(int32_t arg0, int32_t arg1);
static void script_09E6(int32_t arg0);
static void script_09FB(void);
static void script_0BF6(void);
static void script_0CA4(int32_t arg0);
static void script_0D0B(int32_t arg0);
static void script_0D34(int32_t arg0);
static void script_0D72(void);
void script_0ED5(void);
static void script_1052(void);
static void script_1117(void);
static void script_1199(void);
static void script_1245(void);
static void script_1263(void);
static int32_t script_127E(void);
static void script_12CD(const char *arg0, const char *arg1);
static void script_1324(void);
static void script_1327(void);
static void script_132A(void);
static void script_1478(void);
static void script_14EC(void);
static void script_1507(void);
static void script_1518(int32_t arg0, int32_t arg1);
static void script_1542(void);
static void script_1577(void);
static void script_160E(void);
static int32_t script_16F3(void);
static void script_1714(void);
static void script_1778(void);
static int32_t script_17AC(void);
static int32_t script_17B8(void);
static int32_t script_17BB(int32_t arg0);
static void script_1A6C(void);
static void script_1A98(void);
static void script_1ABC(void);
static int32_t script_1AD5(void);
static void script_1B13(void);
static int32_t script_1D31(void);
static void script_1E1B(void);
static void script_1E93(void);
static void script_1F0A(void);
static int32_t script_1F45(int32_t arg0);
static int32_t script_20E7(void);
static void script_2207(void);
static float script_2496(float arg0);
static float script_2654(float arg0);
static void script_27F6(float arg0);
static float script_2859(float arg0);
static float script_28FB(float arg0);
static void script_2952(int32_t arg0);
static void script_2A28(float arg0, int32_t arg1);
static void script_2ABB(float arg0);
static void script_2B1D(void);
static void script_2BAB(void);
static void script_2C39(void);
static void script_2C7E(void);
static int32_t script_2D56(void);
static int32_t script_2DA3(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5, int32_t arg6, float arg7, float arg8);
static int32_t script_2FDD(int32_t arg0, int32_t arg1, int32_t arg2);
static int32_t script_302E(void);

static float task_0;
static float task_4;
static float task_8;
static int32_t task_C;
static int32_t task_10;
static int8_t task_14;
static int8_t task_15;
static int8_t task_16;
static int8_t task_17;
static int8_t task_18;
static int8_t task_19;
static int8_t task_1A;
static int8_t task_1B;
static uint8_t task_1C;
static int8_t task_1D;
static int8_t task_1E;
static int8_t task_1F;
static int8_t task_20;
static uint8_t task_21;
static uint16_t task_22;
static int8_t task_24;
static uint8_t task_25;
static uint16_t task_26;
static Vector3 task_28;
static float task_34;
static int32_t task_38;
static int32_t task_3C;
static float task_40;
static int32_t task_44;
static int32_t task_48;
static float task_4C;
static int32_t task_50;
static int32_t task_54;
static int32_t task_58;
static int32_t task_5C;
static int32_t task_60;
static const float data_30CC[3] = {1.0f, 1.0f, 1.0f};
static const int32_t data_30D8[4] = {16777215, 16777215, 16777215, 16777215};
static const float data_30E8[3] = {20.0f, 800.0f, 1.0f};
static const int32_t data_30F4 = -1;
static const uint32_t data_30F8 = 380;
static const int32_t data_30FC[6] = {1, 390, 0, 1018, 0, -1};
static const uint32_t data_3114 = 510;
static const int32_t data_3118[6] = {1, 525, 0, 1020, 0, -1};
static const uint32_t data_3130 = 150;
static const int32_t data_3134[6] = {1, 160, 0, 1019, 0, -1};
static const uint32_t data_314C = 530;
static const int32_t data_3150[6] = {1, 970, 0, 1014, 0, -1};
static const uint32_t data_3168 = 911;
static const int32_t data_316C[6] = {1, 155, 0, 1015, 0, -1};
static const char data_3184[7] __attribute__((scn_string_literal)) = "FREE02";
static const char data_318B[7] __attribute__((scn_string_literal)) = "FREE03";
static const char data_3192[7] __attribute__((scn_string_literal)) = "FREE04";
static const char data_3199[919] = "FREE01\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000V\325\377\377\000\000\000\000\000\000\000\000\302\326\377\377\000\000\000\000\260\005\000\000\000\000\000\000\000\000\000\000\000\000\000\000\232\271\377\377\000\000\000\000\000\000\000\000(\275\377\377\000\000\000\000\000\000\000\000.\270\377\377\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\\\320\377\377\000\000\000\000\000\000\000\000\344\270\377\377\000\000\000\000\000\000\000\000\224\276\377\377\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\310\321\377\377\000\000\000\000\000\000\000\000.\270\377\377\000\000\000\000\000\000\000\000\373\304\377\377\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\216\003\000\000\022\321\377\377\000\000\000\000x\367\377\377\224\276\377\377\000\000\000\000x\367\377\377\344\330\377\377\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\224\376\377\377~\322\377\377\240\364\377\377\224\376\377\377\357\356\377\377f\006\000\000\216\003\000\000\266\000\000\000\000\000\000\000\000\000\000\000m\301\377\377\000\000\000\000\266\000\000\000\261\245\377\377\000\000\000\000\"\002\000\000\014\266\377\377\000\000\000\000\000\000\000\000\274\373\377\377\000\000\000\000\000\000\000\000\000\300\377\377\000\000\000\000\000\000\000\000\253\252\377\377\000\000\000\000\000\000\000\000\302\266\377\377\000\000\000\000\000\000\000\000\336\375\377\377\000\000\000\000\000\000\000\000\027\254\377\377\000\000\000\000\000\000\000\000\315\254\377\377\000\000\000\000\000\000\000\000x\267\377\377\000\000\000\000\000\000\000\000(\375\377\377\000\000\000\000\216\003\000\000\302\266\377\377\000\000\000\000\266\000\000\000\302\266\377\377\000\000\000\000\\\360\377\377\344\270\377\377\000\000\000\000\000\000\000\000(\375\377\377\000\000\000\000\224\376\377\377?\311\377\377\240\364\377\377\224\376\377\377\261\345\377\377f\006\000\000\216\003\000\000\364\011\000\000\000\000\000\000\000\000\000\000x\267\377\377\000\000\000\000\266\000\000\000\274\233\377\377\000\000\000\000\"\002\000\000\027\254\377\377\000\000\000\000\000\000\000\000\310\361\377\377\000\000\000\000\000\000\000\000\014\266\377\377\000\000\000\000\000\000\000\000\267\240\377\377\000\000\000\000\000\000\000\000\315\254\377\377\000\000\000\000\000\000\000\000\352\363\377\377\000\000\000\000\000\000\000\000#\242\377\377\000\000\000\000\000\000\000\000\331\242\377\377\000\000\000\000\000\000\000\000\203\255\377\377\000\000\000\000\000\000\000\0004\363\377\377\000\000\000\000\216\003\000\000\315\254\377\377\000\000\000\000\266\000\000\000\315\254\377\377\000\000\000\000\\\360\377\377\357\256\377\377\000\000\000\000\000\000\000\0004\363\377\377";
static const int32_t data_3530[114] = {
  0, 0, -5461, 0, -2366, -1092, 0, -182, 1092, 0, -546, -6735, 0, 0, -4004, 0, 0, -2912, 0, -182, 1274, 0, 0, -13289, 0, 0, -5279, 0, 0, -8009, 0, 0,
  1456, 0, 182, -8738, 0, 0, -6007, 0, 0, -8738, 0, 182, 1638, 0, -728, -5279, 0, -1274, -6735, 0, -910, -8738, 0, 0, 1638, 0, 0, -5461, 0, -2366,
  -1092, 0, -182, 1092, 0, -546, -13289, 0, 0, -5643, 0, 0, -8009, 0, -182, 1274, 0, 0, -11832, 0, 0, -5279, 0, 0, -8009, 0, 0, 1456, 0, 182, -8738,
  0, 0, -6007, 0, 0, -8738, 0, 182, 1638, 0, -728, -5279, 0, -1274, -6735, 0, -910, -8738, 0, 0, 1092
};
static const int32_t data_36F8[399] = {
  0, 0, -5461, 0, -2366, -1092, 0, -182, 1092, 0, -546, -3640, 0, 0, -2366, 0, 0, -1456, 0, -182, 1274, 0, 0, -3822, 0, 0, -2730, 0, 0, -1456, 0, 0,
  1456, 0, 182, -3822, 0, 0, -2912, 0, 0, -1638, 0, 182, 1638, 0, -728, -3640, 0, -910, -2730, 0, -546, -1638, 0, 0, 1092, 0, 0, -5461, 0, -2366,
  -1092, 0, -182, 1092, 0, -546, -6735, 0, 0, -4004, 0, 0, -6553, 0, -182, 1274, 0, 0, -13289, 0, 0, -5279, 0, 0, -8009, 0, 0, 1456, 0, 182, -8738, 0,
  0, -6007, 0, 0, -8738, 0, 182, 1638, 0, -728, -5279, 0, -1274, -6735, 0, -910, -8738, 0, 0, 1638, 0, 0, -9102, 0, -2366, -2002, 0, -182, 910, 0,
  -546, -2366, 0, 0, -2002, 0, 0, -2912, 0, -182, 1274, 0, 0, -14199, 0, 0, -11468, 0, 0, -13653, 0, 0, 1456, 0, 182, -11286, 0, 0, -12014, 0, 0,
  -12743, 0, 182, 1638, 0, -728, -10194, 0, -1274, -12196, 0, -910, -10922, -728, 546, 1638, 0, 0, -1820, 0, -2366, -728, 0, -364, -1092, 0, -546,
  -11832, 0, 910, -6735, 0, 0, -6553, 0, -182, 1274, 0, 0, -13289, 0, 0, -6007, 0, 0, -10012, 0, 0, 1092, 0, 182, -8738, 0, 0, -6735, 0, 0, -9830, 0,
  182, 1638, 0, -728, -6553, 0, -1274, -7281, 0, -910, -10740, 0, 0, 1638, 0, 0, -5461, -2912, -2366, -364, -5279, 2548, -910, 0, -546, -8374, 0, 0,
  -9466, 0, 910, -4915, 0, -182, 546, 0, 0, -13289, 0, 0, -5279, 0, 364, -7645, 0, -182, 1456, 0, 182, -8738, 0, 0, -7281, 0, 182, -6917, 0, -182,
  1638, 0, -728, -5279, 0, -1274, -6735, 0, -182, -8009, 0, 0, 1638, -4733, 0, -8556, 0, -2366, -1092, -6189, 2912, 0, 0, -546, -10922, 0, 1820,
  -16384, 0, 0, -11468, 0, 182, 546, 0, 0, -10376, 0, 0, -15837, 0, 0, -12743, 0, -182, 728, 0, 182, -13835, 0, 0, -16201, 0, 364, -10922, 0, -182,
  728, 0, -728, -9648, 0, -3458, -12379, 0, -728, -7463, 0, 0, 1092, 0, 0, -5461, 0, -2366, -1638, 0, -910, 1092, 0, -546, -3640, 0, 0, -2366, 0, 0,
  -1456, 0, -182, 1638, 0, 0, -910, 0, 0, -2730, 0, 0, -1274, 0, 0, 1820, 0, 182, -2730, 0, 0, -1820, 0, 0, -1274, 0, 182, 2184, 0, -728, -3094, 0,
  -910, -2548, 0, 910, -1456, -546, 910, 2366
};
static const Vector4 data_3D34 = {.x = 0.3f, .y = 0.3f, .z = 0.3f, .w = 0.3f};
static const Vector3 data_3D44 = {.x = 0.0f, .y = 0.0f, .z = 0.0f};
static const Vector3 data_3D50 = {.x = 0.0f, .y = 0.02f, .z = 0.0f};
static const char data_3D5C[7] __attribute__((scn_string_literal)) = "SA1101";
static const char data_3D63[11] __attribute__((scn_string_literal)) = "M_BIKE.BIN";
static const char data_3D6E[12] __attribute__((scn_string_literal)) = "system1.snd";
static const char data_3D7A[] __attribute__((scn_string_literal)) = "/scene/03/FBGG";
static const char data_3D89[] __attribute__((scn_string_literal)) = "FBGGST";
static const char data_3D90[] __attribute__((scn_string_literal)) = "/scene/03/FBGG";
static const char data_3D9F[] __attribute__((scn_string_literal)) = "FBGGCAR";
static const uint8_t data_3DA7[1] = {0};
static const uint32_t data_3DA8 = 0x41500000;
static const float data_3DAC[41] = {
  -310.2f, 13.0f, -328.0f, 22.95f, -328.0f, 22.95f, -310.2f, -7.5f, 0.0f, 18.2f, -8.0f, -318.9f, 30.0f, -59.0f, -2685.0f, -59.0f, -3017.0f, 216.0f,
  -3017.0f, 216.0f, -2685.0f, -0.5f, 13.0f, 74.0f, 1.0f, -2863.0f, 1000.0f, 155.0f, -2928.0f, 155.0f, -3197.0f, 233.0f, -3197.0f, 233.0f, -2928.0f,
  -0.5f, 13.0f, 210.0f, 1.0f, -3069.0f, 1000.0f
};
static const char data_3E50[11] __attribute__((scn_string_literal)) = "SA1101A008";
static const char data_3E5B[12] __attribute__((scn_string_literal)) = "system1.snd";
static const char data_3E67[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_3E6C[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_3E71[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_3E76[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_3E7B[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_3E80[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_3E85[7] __attribute__((scn_string_literal)) = "FREE";
static const Vector3 data_3E8C = {.x = 0.0f, .y = 0.02f, .z = 0.0f};
static const Vector3 data_3E98 = {.x = 0.001f, .y = 0.005f, .z = 0.001f};
static const uint32_t data_3EA4 = 0x3F800000;
static const float data_3EA8[2] = {2.0f, 1.0f};
static const char data_3EB0[9] __attribute__((scn_string_literal)) = "BIKS509G";
static const char data_3EB9[11] __attribute__((scn_string_literal)) = "BIKS504G";
