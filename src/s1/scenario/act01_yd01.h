#pragma once

#include "scenario.h"
#include "motion/motion.h"

static void script_0030(int32_t arg0);
static void script_009B(void);
static void script_015D(int32_t arg0);
static void script_01FF(void);
static void script_025B(int32_t arg0);
static int32_t script_02D5(void);
static void script_0DBE(void);
static int32_t script_0EB2(void);
static int32_t script_0F44(int32_t arg0, int32_t arg1, int32_t arg2);
static int32_t script_0F84(int32_t arg0, int32_t arg1);
void script_0FC4(void);
static int32_t script_0FFC(int32_t arg0);
static void script_107D(void);

static int32_t task_0;
static const float data_1090[3] = {1.0f, 1.0f, 1.0f};
static const int32_t data_109C[4] = {16777215, 16777215, 16777215, 16777215};
static const float data_10AC[3] = {20.0f, 800.0f, 1.0f};
static const int32_t data_10B8 = -1;
static const int32_t data_10BC[57] = {
  0, 0, -5461, 0, -182, 4369, 0, -182, 1092, 0, -546, -6735, 0, 0, -2184, 0, 728, -2366, 0, -182, 1274, 0, 0, -6007, 0, 0, -3276, 0, 0, -2184, 0, 0,
  1456, 0, 182, -3822, 0, 0, -1820, 0, 0, -2184, 0, 182, 1638, 0, -728, -5279, 0, -1638, -2912, 0, 364, -3094, 0, 364, 1456
};
static const int32_t data_11A0[57] = {
  0, 0, -5461, 0, -2366, -1092, 0, -182, 1092, 0, -546, -6735, 0, 0, -4551, 0, 728, -4733, 0, -182, 1274, 0, 0, -6007, 0, 0, -4915, 0, 0, -4369, 0, 0,
  1456, 0, 182, -7463, 0, 0, -5643, 0, 0, -2184, 0, 182, 1638, 0, -728, -5279, 0, -1638, -2912, 0, 364, -3094, 0, 364, 1456
};
static const int32_t data_1284[57] = {
  0, 0, -5461, 0, -2366, -1092, 0, -182, 1092, 0, -546, -6735, 0, 0, -4004, 0, 0, -6553, 0, -182, 1274, 0, 0, -13289, 0, 0, -5279, 0, 0, -8009, 0, 0,
  1456, 0, 182, -8738, 0, 0, -6007, 0, 0, -8738, 0, 182, 1638, 0, -728, -5279, 0, -1274, -6735, 0, -910, -8738, 0, 0, 1638
};
static const int32_t data_1368[57] = {
  0, 0, -5461, 0, -2366, -3458, 0, -182, 1092, 0, -546, -9466, 0, 1820, -12925, 0, 364, -6553, 0, -182, 1274, 0, 0, -13289, 0, 182, -11104, 0, 364,
  -8009, 0, 0, 1456, 0, 182, -10922, 0, 0, -6917, 0, 0, -8738, 0, 182, 1638, 0, -728, -6553, 0, -1274, -6735, 0, -910, -6735, 0, 0, 1638
};
static const int32_t data_144C[57] = {
  0, 0, -9102, 0, -2366, -2002, 0, -182, 910, 0, -546, -2366, 0, 0, -2002, 0, 0, -2912, 0, -182, 1274, 0, 0, -14199, 0, 0, -11468, 0, 0, -13653, 0, 0,
  1456, 0, 182, -11286, 0, 0, -12014, 0, 0, -12743, 0, 182, 1638, 0, -728, -10194, 0, -1274, -12196, 0, -910, -10922, -728, 546, 1638
};
static const int32_t data_1530[57] = {
  0, 0, -10922, 0, 0, -10558, 0, 1456, 0, 0, 0, -18022, 0, 0, -17112, 0, 0, -18386, 0, 0, 0, 0, 0, -12196, 0, 0, -18204, 0, 0, -16748, 0, 0, 0, 0, 0,
  -11832, 0, 0, -18386, 0, 0, -15109, 0, 0, 0, 0, 910, -12014, 0, -728, -16748, 0, -2184, -14381, 0, 0, 0
};
static const Vector4 data_1614 = {.x = 1.0f, .y = 0.2f, .z = 0.2f, .w = 0.2f};
static const char data_1624[16] __attribute__((scn_string_literal)) = "/scene/01/YD01/";
static const char data_1634[5] __attribute__((scn_string_literal)) = "SAKR";
static const char data_1639[13] __attribute__((scn_string_literal)) = "seqdata0.bin";
static const char data_1646[5] __attribute__((scn_string_literal)) = "SAKR";
static const char data_164B[21] = "SAKR\000\000\000\200?\354Q8>\354Q8>\012\327#>";
static const Vector4 data_1660 = {.x = 0.3f, .y = 0.3f, .z = 0.3f, .w = 0.3f};
static const Vector4 data_1670 = {.x = 0.4f, .y = 0.4f, .z = 0.4f, .w = 0.4f};
static const Vector3 data_1680 = {.x = 6.57f, .y = 0.0f, .z = -26.75f};
static const Vector3 data_168C = {.x = 0.01f, .y = 0.01f, .z = 0.01f};
static const char data_1698[6] __attribute__((scn_string_literal)) = "A0128";
static const char data_169E[6] __attribute__((scn_string_literal)) = "A0128";
