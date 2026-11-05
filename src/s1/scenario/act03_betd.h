#pragma once

#include "scenario.h"
#include "motion/motion.h"

static void script_0030(int32_t arg0);
static void script_00E4(int32_t arg0);
static void script_0134(int32_t arg0);
static void script_01F6(void);
static void script_026B(void);
static int32_t script_029E(void);
static int32_t script_02A1(void);
static int32_t script_02A4(void);
static void script_0784(void);
static int32_t script_0A94(int32_t arg0);
void script_0CC5(void);

static int32_t task_0;
static int32_t task_4;
static const float data_0CD8[3] = {1.0f, 1.0f, 1.0f};
static const int32_t data_0CE4[4] = {16777215, 16777215, 16777215, 16777215};
static const float data_0CF4[3] = {20.0f, 800.0f, 1.0f};
static const int32_t data_0D00 = -1;
static const int32_t data_0D04[57] = {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0
};
static const int32_t data_0DE8[57] = {
  0, 0, -3640, 0, 0, -2366, -4369, 1820, -546, 0, 0, -5279, 0, 0, -6007, 0, 0, -2912, 0, 0, -910, 0, 0, -6553, 0, 0, -6735, 0, 0, -3640, 0, 0, -182,
  0, 0, -5461, 0, 0, -6007, 0, 0, -2366, 0, 0, 0, 0, 0, -5643, 0, 0, -4733, 0, 0, -2366, -1456, 0, 728
};
static const int32_t data_0ECC[171] = {
  0, 0, -9466, 0, -546, -2912, 0, 3094, -2366, 0, 0, -18386, 0, 0, -19296, 0, 1092, -14381, 0, 0, 0, 0, 0, -16748, 0, 0, -17658, 0, 0, -15109, 0, 0,
  0, 0, 0, -16566, 0, 0, -18204, 0, 0, -14381, 0, 0, 0, 0, 0, -19478, 0, 0, -18386, 0, -4004, -12925, 0, 0, -546, 0, 0, -3276, 0, 0, 0, 0, 0, 0, 0, 0,
  -6553, 0, 0, -4369, 0, -546, -3458, 0, 0, 0, 0, 0, -5461, 0, 0, -5643, 0, 0, -3458, 0, 0, 0, 0, 0, -6007, 0, 0, -4551, 0, 546, -2548, 0, 0, 0, 0, 0,
  -6189, 0, -1092, -3094, 0, 0, -2548, 0, 0, 0, 0, 0, -6007, 0, 0, -728, 0, 0, 0, 0, 0, -5097, 0, 0, -2184, 0, 0, -2002, 0, 0, 0, 0, 0, -5825, 0, 0,
  -2912, 0, 0, -728, 0, 0, 0, 0, 0, -4915, 0, 0, -4187, 0, 0, -1274, 0, 0, 0, 0, 0, -6553, 0, -364, -4369, 0, -546, -1456, 0, 0, 0
};
static const int32_t data_1178[57] = {
  0, 1274, -13107, 0, -1456, -655, 0, -546, -728, 0, -546, -7099, 0, 0, -6553, 0, 1092, 1456, 0, -182, 728, 0, 0, -9284, 0, 0, -8192, 0, -364, 0, 0,
  182, 2002, 0, 182, -10740, 0, 0, -4132, 0, 0, -728, 0, -182, 1820, 0, -728, -9648, 0, 0, -5206, -1092, -364, -1638, -2002, -182, 1092
};
static const Vector4 data_125C = {.x = 0.5f, .y = 0.5f, .z = 0.5f, .w = 0.5f};
static const FogColourData data_126C = {.red = 0.1f, .green = 150.0f, .blue = 1.04f, .colour = 0xFF17191BU};
static const Vector4 data_127C = {.x = 0.27f, .y = 0.27f, .z = 0.27f, .w = 0.27f};
static const float data_128C[4] = {0.76f, 1.0f, 1.0f, 1.0f};
static const Vector4 data_129C = {.x = 1.0f, .y = 0.16f, .z = 0.12f, .w = 0.2f};
static const uint32_t data_12AC[4] = {0, 0, 0, 0};
static const char data_12BC[7] __attribute__((scn_string_literal)) = "01BEDA";
static const char data_12C3[13] __attribute__((scn_string_literal)) = "M_01BEDA.BIN";
static const char data_12D0[11] __attribute__((scn_string_literal)) = "bgm127.snd";
static const char data_12DB[13] __attribute__((scn_string_literal)) = "a1_baden.snd";
static const char data_12E8[8] __attribute__((scn_string_literal)) = "01BEDA";
