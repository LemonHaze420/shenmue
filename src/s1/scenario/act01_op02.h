#pragma once

#include "scenario.h"
#include "motion/motion.h"

static void script_0030(int32_t arg0);
static void script_0C00(void);
static void script_0C5A(void);
static int32_t script_0D6C(void);
static void script_0E2B(int32_t arg0);
static void script_0E58(int32_t arg0);
static void script_0EF2(void);
static void script_0F03(int32_t arg0);
static void script_0F0F(int32_t arg0);
void script_0F33(void);
static void script_0F51(void);
static void script_0F6F(void);
static void script_0F85(void);
static void script_0F9B(void);
static void script_0FBD(void);
static void script_0FEF(void);
static int32_t script_1011(int32_t arg0);

static const float data_1094[3] = {1.0f, 1.0f, 1.0f};
static const int32_t data_10A0[4] = {16777215, 16777215, 16777215, 16777215};
static const float data_10B0[3] = {20.0f, 800.0f, 1.0f};
static const int32_t data_10BC = -1;
static const Vector4 data_10C0 = {.x = 1.0f, .y = 0.4f, .z = 0.3f, .w = 0.3f};
static const int32_t data_10D0[57] = {
  0, 0, 1638, 0, 0, -1456, 0, 0, -728, 0, 0, -2366, 0, 0, -2366, 0, 0, -1456, 0, 0, 0, 0, 0, -7463, 0, 1274, -10012, 0, 728, 1092, 0, 0, 0, 0, 0,
  -9102, 0, 2002, -12743, 0, 2366, 728, 0, 0, 0, 0, 0, -9648, 0, -1820, -10012, 0, 2184, -4369, 0, 0, 0
};
static const int32_t data_11B4[57] = {
  0, 0, 1638, 0, 0, -1456, 0, 0, -728, 0, 0, -2366, 0, 0, -2366, 0, 0, -1456, 0, 0, 0, 0, 0, -8009, 0, 1274, -9102, 0, 728, -364, 0, 0, 0, 0, 0,
  -9102, 0, 2002, -12743, 0, 2366, 728, 0, 0, 0, 0, 0, -9648, 0, -1820, -10012, 0, 2184, -4369, 0, 0, 0
};
static const Vector3i data_1298 = {.x = 10194, .y = 0, .z = 0};
static const char data_12A4[5] __attribute__((scn_string_literal)) = "KAKU";
static const char data_12A9[5] __attribute__((scn_string_literal)) = "HAWK";
static const char data_12AE[16] __attribute__((scn_string_literal)) = "/scene/01/OP02/";
static const char data_12BE[5] __attribute__((scn_string_literal)) = "SHDR";
static const char data_12C3[5] __attribute__((scn_string_literal)) = "SHDR";
static const Vector4 data_12C8 = {.x = 1.0f, .y = 0.4f, .z = 0.3f, .w = 0.3f};
static const char data_12D8[16] __attribute__((scn_string_literal)) = "/scene/01/OP02/";
static const char data_12E8[9] __attribute__((scn_string_literal)) = "OP02.afs";
static const char data_12F1[7] __attribute__((scn_string_literal)) = "SHDR";
static const FogColourData data_12F8 = {.red = 20.0f, .green = 800.0f, .blue = 0.36f, .colour = 0xFFFF6900U};
static const uint32_t data_1308[4] = {0x96413D23U, 0x96413D23U, 0, 0};
static const uint32_t data_1318[4] = {0xB0952D23U, 0xB0952D23U, 0x22787270U, 0x22787270U};
static const FogColourData data_1328 = {.red = 20.0f, .green = 800.0f, .blue = 0.36f, .colour = 0xFFFF6900U};
static const uint32_t data_1338[4] = {0x96413D23U, 0x96413D23U, 0, 0};
static const uint32_t data_1348[4] = {0xB0952D23U, 0xB0952D23U, 0x22787270U, 0x22787270U};
static const Vector3 data_1358 = {.x = 5020.0f, .y = -210.0f, .z = -5620.0f};
static const char data_1364[8] __attribute__((scn_string_literal)) = "A0100";
