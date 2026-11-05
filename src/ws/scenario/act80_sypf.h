#pragma once

#include "scenario.h"
#include "motion/motion.h"

void script_0030(void);
static int32_t script_0174(int32_t arg0);
static int32_t script_0413(int32_t arg0);
static int32_t script_0554(int32_t arg0);
static int32_t script_08C8(int32_t arg0);
static int32_t script_0B29(void);
static int32_t script_0B42(void);
static int32_t script_0BF6(int32_t arg0);
static int32_t script_0C49(void);
static void script_0EE3(int32_t arg0);
static void script_0F26(int32_t arg0);
static void script_1028(int32_t arg0);

static int32_t task_0;
static int8_t task_4;
static uint8_t task_5;
static uint16_t task_6;
static int32_t task_8;
static int32_t task_C;
static int8_t task_10;
static uint8_t task_11;
static uint16_t task_12;
static int8_t task_14;
static uint8_t task_15;
static uint16_t task_16;
static int32_t task_18;
static int32_t task_1C;
static int32_t task_20;
static int32_t task_24;
static int32_t task_28;
static Vector3 task_2C;
static Vector3 task_38;
static float task_44;
static int32_t task_48;
static int32_t task_4C;
static const float data_1038[3] = {1.0f, 1.0f, 1.0f};
static const int32_t data_1044[4] = {16777215, 16777215, 16777215, 16777215};
static const float data_1054[3] = {20.0f, 800.0f, 1.0f};
static const int32_t data_1060 = -1;
static const Vector3 data_1064 = {.x = 2.83f, .y = 1.55f, .z = -2.35f};
static const Vector3 data_1070 = {.x = 3.0f, .y = 1.55f, .z = -2.35f};
static const Vector3 data_107C = {.x = 2.64f, .y = 1.54f, .z = -2.35f};
static const Vector3 data_1088 = {.x = 2.71f, .y = 1.55f, .z = -2.35f};
static const float data_1094[3] = {2.99f, 1.55f, -2.35f};
static const Vector3 data_10A0 = {.x = 2.63f, .y = 1.55f, .z = -2.33f};
static const char data_10AC[10] __attribute__((scn_string_literal)) = "M_SYP.BIN";
static const char data_10B6[4] __attribute__((scn_string_literal)) = "NET";
static const char data_10BA[8] __attribute__((scn_string_literal)) = "NETB001";
static const char data_10C2[8] __attribute__((scn_string_literal)) = "NETB002";
static const char data_10CA[8] __attribute__((scn_string_literal)) = "NETB003";
static const char data_10D2[8] __attribute__((scn_string_literal)) = "NETB004";
static const char data_10DA[8] __attribute__((scn_string_literal)) = "NETB005";
static const char data_10E2[8] __attribute__((scn_string_literal)) = "NETB006";
static const char data_10EA[8] __attribute__((scn_string_literal)) = "NETB007";
static const char data_10F2[8] __attribute__((scn_string_literal)) = "NETB008";
static const char data_10FA[8] __attribute__((scn_string_literal)) = "NETB009";
static const char data_1102[8] __attribute__((scn_string_literal)) = "NETB010";
static const char data_110A[8] __attribute__((scn_string_literal)) = "NETB011";
static const char data_1112[8] __attribute__((scn_string_literal)) = "NETB012";
static const char data_111A[8] __attribute__((scn_string_literal)) = "NETB013";
static const char data_1122[8] __attribute__((scn_string_literal)) = "NETB014";
static const char data_112A[10] __attribute__((scn_string_literal)) = "NETB015";
