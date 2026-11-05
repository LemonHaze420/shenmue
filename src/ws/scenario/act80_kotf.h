#pragma once

#include "scenario.h"
#include "motion/motion.h"

void script_0030(void);
static void script_0049(void);
static void script_006E(int32_t arg0);
static void script_00C7(void);
static int32_t script_030B(int32_t arg0, void *arg1, void *arg2);
static int32_t script_0520(int32_t arg0, void *arg1);
static int32_t script_0748(void);
static void script_0797(int32_t arg0);
static void script_08AB(int32_t arg0);
static void script_08EE(int32_t arg0);
static void script_09D2(int32_t arg0);

static int8_t task_0;
static int8_t task_1;
static int8_t task_2;
static uint8_t task_3;
static int32_t task_4;
static Vector3 task_8;
static Vector3 task_14;
static float task_20;
static int32_t task_24;
static int32_t task_28;
static const float data_09E0[3] = {1.0f, 1.0f, 1.0f};
static const int32_t data_09EC[4] = {16777215, 16777215, 16777215, 16777215};
static const float data_09FC[3] = {20.0f, 800.0f, 1.0f};
static const int32_t data_0A08 = -1;
static char data_0A0C[] = "NETE001";
static char data_0A14[] = "NETE002";
static char data_0A1C[] = "NETE003";
static char data_0A24[] = "NETE004";
static char data_0A2C[] = "NETE005";
static char data_0A34[] = "NETE006";
static char data_0A3C[] = "NETE007";
static char data_0A44[] = "NETE008";
static char data_0A4C[] = "NETE009";
static char data_0A54[] = "NETE010";
static char data_0A5C[] = "NETE011";
static char data_0A64[] = "NETE012";
static char data_0A6C[] = "NETE013";
static char data_0A74[] = "NETE014";
static uint8_t data_0A7C[4] = {0};
static char *const data_0A80 = data_0A0C;
static const int32_t data_0A84 = 40;
static char *const data_0A88 = data_0A14;
static const int32_t data_0A8C = 20;
static char *const data_0A90 = data_0A1C;
static const int32_t data_0A94 = 10;
static char *const data_0A98 = data_0A24;
static const int32_t data_0A9C = 30;
static char *const data_0AA0 = data_0A2C;
static const int32_t data_0AA4 = 50;
static char *const data_0AA8 = data_0A34;
static const uint8_t data_0AAC[4] = {0};
static char *const data_0AB0 = data_0A3C;
static const uint8_t data_0AB4[4] = {0};
static char *const data_0AB8 = data_0A44;
static const int32_t data_0ABC = 30;
static char *const data_0AC0 = data_0A4C;
static const int32_t data_0AC4 = 10;
static char *const data_0AC8 = data_0A54;
static const uint8_t data_0ACC[4] = {0};
static char *const data_0AD0 = data_0A5C;
static const uint8_t data_0AD4[4] = {0};
static char *const data_0AD8 = data_0A64;
static const int32_t data_0ADC = 20;
static char *const data_0AE0 = data_0A6C;
static const int32_t data_0AE4 = 10;
static char *const data_0AE8 = data_0A74;
static const uint8_t data_0AEC[4] = {0};
static uint8_t *const data_0AF0 = data_0A7C;
static const int32_t data_0AF4 = -1;
static const uint32_t data_0AF8 = 9;
static const int32_t data_0AFC[183] = {
  0, 6, 150, 8, 0, 40, 0, -1, -1, -1, -1, 3, 0, 40, 170, 6, 0, 40, 0, -1, -1, -1, -1, 1, 0, 70, 100, 9, 0, 40, 10, -1, -1, -1, -1, 7, 0, 10, 10, 0, 0,
  10, 200, 9, 0, 30, 160, 0, -1, 50, 50, -1, -1, -1, -1, 0, 0, 40, 270, 10, 0, 60, 70, 7, 0, 30, 100, 1, 0, 20, 0, -1, -1, -1, -1, 10, 0, 60, 190, -1,
  -1, -1, -1, 7, 0, 10, 10, 3, 0, 20, 0, -1, -1, -1, -1, 7, 0, 10, 80, 10, 0, 40, 0, -1, -1, -1, -1, 3, 0, 60, 200, 2, 0, 16, 50, 5, 0, 10, 0, -1, -1,
  -1, -1, 10, 0, 80, 0, -1, -1, -1, -1, 0, 0, 40, 90, 7, 0, 10, 0, -1, -1, -1, -1, 1, 0, 10, 70, 3, 0, 20, 0, -1, -1, -1, -1, 7, 0, 20, 20, 0, 0, 20,
  90, 1, 0, 30, 0, -1, -1, -1, -1, 1, 0, 100, 190, 9, 1, 10, 0, -1, -1, -1, -1
};
static const uint8_t data_0DD8[16] = {0};
static const int32_t data_0DE8[69] = {
  4, 2, 0, 120, 0, 0, 12, 3, 0, 132, 0, 0, 0, 4, 6, 140, 0, 0, 8, 2, 6, 185, 0, 0, 12, 3, 6, 197, 0, 0, 0, 4, 9, 0, 0, 0, 14, 2, 9, 34, 0, 0, 14, 3,
  9, 48, 0, 0, 0, 4, 13, 260, 0, 0, 8, 2, 13, 370, 0, 0, 20, 3, 13, 390, 0, 0, 0, 4, -1
};
static const uint32_t data_0EFC[4] = {0xFF000000U, 0xFF000000U, 0xFF000000U, 0xFF000000U};
static const char data_0F0C[4] __attribute__((scn_string_literal)) = "NET";
static const uint32_t data_0F10 = 4000;
static const int32_t data_0F14[13] = {3001, 3002, 3003, 4004, 4005, 3006, 3007, 3008, 4009, 3010, 4011, 3012, 4013};
