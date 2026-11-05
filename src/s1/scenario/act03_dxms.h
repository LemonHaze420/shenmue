#pragma once

#include "scenario.h"
#include "motion/motion.h"

static void script_0030(void);
static void script_0211(void);
static void script_043A(void);
static void script_0687(void);
static void script_08B0(void);
static void script_0931(int32_t arg0);
static void script_0D81(void);
static void script_0E8C(int32_t arg0);
static void script_0EB9(int32_t arg0);
static void script_0F32(int32_t arg0);
static void script_0FD8(void);
static void script_10B3(int32_t arg0);
static void script_10BF(int32_t arg0);
static void script_111A(int32_t arg0);
static int32_t script_119D(void);
static int32_t script_12BA(void);
static int32_t script_1377(void);
static int32_t script_1485(int32_t arg0, int32_t arg1);
static int32_t script_160E(void);
static void script_16A7(void);
static void script_18D4(void);
static void script_19D5(void);
static void script_1A85(void);
static void script_1B07(void);
static void script_1BD8(void);
static void script_1C53(int32_t arg0);
void script_1D54(void);
static int32_t script_1D84(int32_t arg0);
static void script_1FC4(int32_t arg0, int32_t arg1);

static GameDateTime task_0;
static int32_t task_8;
static int32_t task_C;
static int32_t task_10;
static const float data_2028[3] = {1.0f, 1.0f, 1.0f};
static const int32_t data_2034[4] = {16777215, 16777215, 16777215, 16777215};
static const float data_2044[3] = {20.0f, 800.0f, 1.0f};
static const int32_t data_2050 = -1;
static const uint32_t data_2054 = 0xC2480000;
static const float data_2058[2] = {0.0f, 10.0f};
static const uint32_t data_2060 = 0x41A00000;
static const float data_2064[2] = {0.0f, 110.0f};
static const uint32_t data_206C = 0xC2041FF3;
static const float data_2070[2] = {0.0f, 52.1008f};
static const uint32_t data_2078 = 0x41B3AA99;
static const uint32_t data_207C[2] = {0, 0x420947FDU};
static const uint32_t data_2084 = 0x42201A1D;
static const float data_2088[2] = {0.0f, -2.1877f};
static const uint32_t data_2090 = 0xC2BD9F2E;
static const uint32_t data_2094[2] = {0, 0x428A24A9U};
static const uint32_t data_209C = 0x41D6BFB1;
static const uint32_t data_20A0[2] = {0, 0x42CFC5AFU};
static const uint32_t data_20A8 = 0xC1A00000;
static const float data_20AC[2] = {0.0f, 110.0f};
static const uint32_t data_20B4 = 0xC19181A3;
static const uint32_t data_20B8[2] = {0, 0x421B20DFU};
static const uint32_t data_20C0 = 0xC2C00000;
static const float data_20C4[2] = {0.0f, 78.0f};
static const uint32_t data_20CC = 0xC2880000;
static const float data_20D0[2] = {0.0f, 53.0f};
static const uint32_t data_20D8 = 0xC1A00000;
static const float data_20DC[2] = {0.0f, 110.0f};
static const uint32_t data_20E4 = 0xC2041FF3;
static const float data_20E8[2] = {0.0f, 52.1008f};
static const uint32_t data_20F0 = 0x3F4978D5;
static const float data_20F4[33] = {
  0.787f, 0.866f, 0.944f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.944f, 0.866f, 0.787f,
  0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f
};
static const uint8_t data_2178[80] = {0};
typedef struct Row_21C8 { float w0; float w1; float w2; int32_t w3; } Row_21C8;
static const Row_21C8 data_21C8[3] = {
  {1.0f, 1.0f, 1.0f, 0},
  {0.0f, 0.0f, 0.0f, 0},
  {0.0f, 0.0f, 0.0f, 0}
};
static const int32_t data_21F8[2] = {0, 0};
static const uint8_t data_2200[24] = {0};
static const float data_2218[28] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f
};
static const uint8_t data_2288[24] = {0};
static const float data_22A0[28] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f
};
static const uint32_t data_2310 = 0x3F4978D5;
static const float data_2314[33] = {
  0.787f, 0.866f, 0.944f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  0.944f, 0.866f, 0.866f, 0.866f, 0.866f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f
};
static const uint8_t data_2398[24] = {0};
static const float data_23B0[28] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f
};
static const uint32_t data_2420 = 0x3F800000;
static const float data_2424[33] = {
  0.944f, 0.866f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f,
  0.787f, 0.787f, 0.551f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint8_t data_24A8[104] = {0};
static const float data_2510[8] = {1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f};
static const uint8_t data_2530[96] = {0};
static const float data_2590[10] = {1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f};
static const uint32_t data_25B8 = 0x3F266666;
static const float data_25BC[33] = {
  0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f,
  0.708f, 0.787f, 0.866f, 0.944f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint32_t data_2640 = 0x3F800000;
static const float data_2644[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint32_t data_26C8 = 0x3F800000;
static const float data_26CC[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint32_t data_2750 = 0x3F800000;
static const float data_2754[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint32_t data_27D8 = 0xC2480000;
static const float data_27DC[2] = {0.0f, 10.0f};
static const uint32_t data_27E4 = 0x4214CCCD;
static const float data_27E8[2] = {0.0f, 6.6f};
static const uint32_t data_27F0 = 0x41B5999A;
static const float data_27F4[2] = {0.0f, 27.2f};
static const uint32_t data_27FC = 0x41B3999A;
static const float data_2800[2] = {0.0f, 34.32f};
static const uint32_t data_2808 = 0x4220147B;
static const float data_280C[2] = {0.0f, -2.18f};
static const uint32_t data_2814 = 0xC2BD9EB8;
static const float data_2818[2] = {0.0f, 69.07f};
static const uint32_t data_2820 = 0x41D6B852;
static const float data_2824[2] = {0.0f, 103.88f};
static const uint32_t data_282C = 0xC0966666;
static const float data_2830[2] = {0.0f, 30.6f};
static const uint32_t data_2838 = 0xC19170A4;
static const float data_283C[2] = {0.0f, 38.78f};
static const uint32_t data_2844 = 0xC2C00000;
static const float data_2848[2] = {0.0f, 78.0f};
static const uint32_t data_2850 = 0xC2840000;
static const float data_2854[2] = {0.0f, 54.0f};
static const uint32_t data_285C = 0xC1FB0A3D;
static const float data_2860[2] = {0.0f, 40.1f};
static const uint32_t data_2868 = 0xC2C28F5C;
static const float data_286C[2] = {0.0f, 64.48f};
static const uint32_t data_2874 = 0xC2F2FAE1;
static const float data_2878[2] = {0.0f, 94.2f};
static const uint32_t data_2880 = 0xC2BC75C3;
static const float data_2884[2] = {0.0f, 85.91f};
static const uint32_t data_288C = 0x3F4978D5;
static const float data_2890[33] = {
  0.787f, 0.866f, 0.944f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.944f, 0.866f, 0.787f,
  0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f
};
static const uint8_t data_2914[16] = {0};
static const float data_2924[30] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint8_t data_299C[16] = {0};
static const float data_29AC[30] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint8_t data_2A24[24] = {0};
static const float data_2A3C[28] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f
};
static const uint32_t data_2AAC = 0x3F4978D5;
static const float data_2AB0[33] = {
  0.787f, 0.866f, 0.944f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  0.944f, 0.866f, 0.866f, 0.866f, 0.866f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f
};
static const uint8_t data_2B34[24] = {0};
static const float data_2B4C[28] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f
};
static const uint32_t data_2BBC = 0x3F800000;
static const float data_2BC0[33] = {
  0.944f, 0.866f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f,
  0.787f, 0.787f, 0.551f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint8_t data_2C44[16] = {0};
static const float data_2C54[30] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint8_t data_2CCC[96] = {0};
static const float data_2D2C[10] = {1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f};
static const uint32_t data_2D54 = 0x3F266666;
static const float data_2D58[33] = {
  0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f,
  0.708f, 0.787f, 0.866f, 0.944f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint32_t data_2DDC = 0x3F800000;
static const float data_2DE0[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint8_t data_2E64[16] = {0};
static const float data_2E74[30] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint8_t data_2EEC[80] = {0};
static const float data_2F3C[14] = {1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f};
static const uint8_t data_2F74[80] = {0};
static const float data_2FC4[14] = {1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f};
static const uint8_t data_2FFC[80] = {0};
static const float data_304C[14] = {1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f};
static const uint32_t data_3084 = 0xC2480000;
static const float data_3088[2] = {0.0f, 10.0f};
static const uint32_t data_3090 = 0x41E80000;
static const shen_name data_3094[2] = {0, SHEN_NAME('333A')};
static const uint32_t data_309C = 0xC2041FF3;
static const float data_30A0[2] = {0.0f, 52.1008f};
static const uint32_t data_30A8 = 0x41B3AA99;
static const uint32_t data_30AC[2] = {0, 0x420947FDU};
static const uint32_t data_30B4 = 0x42201A1D;
static const float data_30B8[2] = {0.0f, -2.1877f};
static const uint32_t data_30C0 = 0xC2BD9F2E;
static const uint32_t data_30C4[2] = {0, 0x428A24A9U};
static const uint32_t data_30CC = 0x41D6BFB1;
static const uint32_t data_30D0[2] = {0, 0x42CFC5AFU};
static const uint32_t data_30D8 = 0xC1A00000;
static const float data_30DC[2] = {0.0f, 110.0f};
static const uint32_t data_30E4 = 0xC19181A3;
static const uint32_t data_30E8[2] = {0, 0x421B20DFU};
static const uint32_t data_30F0 = 0xC2C00000;
static const float data_30F4[2] = {0.0f, 78.0f};
static const uint32_t data_30FC = 0xC2880000;
static const float data_3100[2] = {0.0f, 53.0f};
static const uint32_t data_3108 = 0xC0B9999A;
static const float data_310C[2] = {0.0f, 26.2f};
static const uint32_t data_3114 = 0xC22E0000;
static const float data_3118[2] = {0.0f, 46.0f};
static const uint32_t data_3120 = 0xC2CE0000;
static const float data_3124[2] = {0.0f, 80.0f};
static const uint32_t data_312C = 0xC2041FF3;
static const float data_3130[2] = {0.0f, 52.1008f};
static const uint32_t data_3138 = 0x3F4978D5;
static const float data_313C[33] = {
  0.787f, 0.866f, 0.944f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.944f, 0.866f, 0.787f,
  0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f
};
static const uint8_t data_31C0[16] = {0};
static const float data_31D0[30] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint8_t data_3248[24] = {0};
static const float data_3260[28] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f
};
static const uint8_t data_32D0[24] = {0};
static const float data_32E8[28] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f
};
static const uint32_t data_3358 = 0x3F4978D5;
static const float data_335C[33] = {
  0.787f, 0.866f, 0.944f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  0.944f, 0.866f, 0.866f, 0.866f, 0.866f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f
};
static const uint8_t data_33E0[24] = {0};
static const float data_33F8[28] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f
};
static const uint32_t data_3468 = 0x3F800000;
static const float data_346C[33] = {
  0.944f, 0.866f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f,
  0.787f, 0.787f, 0.551f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint8_t data_34F0[104] = {0};
static const float data_3558[8] = {1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f};
static const uint8_t data_3578[96] = {0};
static const float data_35D8[10] = {1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f};
static const uint32_t data_3600 = 0x3F266666;
static const float data_3604[33] = {
  0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f,
  0.708f, 0.787f, 0.866f, 0.944f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint32_t data_3688 = 0x3F800000;
static const float data_368C[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint8_t data_3710[16] = {0};
static const float data_3720[30] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint8_t data_3798[16] = {0};
static const float data_37A8[30] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint8_t data_3820[80] = {0};
static const float data_3870[14] = {1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f};
static const uint32_t data_38A8 = 0x3F800000;
static const float data_38AC[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint32_t data_3930 = 0xC2480000;
static const float data_3934[2] = {0.0f, 10.0f};
static const uint32_t data_393C = 0x41A00000;
static const float data_3940[2] = {0.0f, 110.0f};
static const uint32_t data_3948 = 0xC2041FF3;
static const float data_394C[2] = {0.0f, 52.1008f};
static const uint32_t data_3954 = 0x41B3AA99;
static const uint32_t data_3958[2] = {0, 0x420947FDU};
static const uint32_t data_3960 = 0x42201A1D;
static const float data_3964[2] = {0.0f, -2.1877f};
static const uint32_t data_396C = 0xC2BD9F2E;
static const uint32_t data_3970[2] = {0, 0x428A24A9U};
static const uint32_t data_3978 = 0x41D6BFB1;
static const uint32_t data_397C[2] = {0, 0x42CFC5AFU};
static const uint32_t data_3984 = 0xC1A00000;
static const float data_3988[2] = {0.0f, 110.0f};
static const uint32_t data_3990 = 0xC19181A3;
static const uint32_t data_3994[2] = {0, 0x421B20DFU};
static const uint32_t data_399C = 0xC2C00000;
static const float data_39A0[2] = {0.0f, 78.0f};
static const uint32_t data_39A8 = 0xC2880000;
static const float data_39AC[2] = {0.0f, 53.0f};
static const uint32_t data_39B4 = 0x40A66666;
static const float data_39B8[2] = {0.0f, 27.0f};
static const uint32_t data_39C0 = 0xC201999A;
static const float data_39C4[2] = {0.0f, 44.5f};
static const uint32_t data_39CC = 0xC2CE0000;
static const float data_39D0[2] = {0.0f, 80.0f};
static const uint32_t data_39D8 = 0xC2041FF3;
static const float data_39DC[2] = {0.0f, 52.1008f};
static const uint32_t data_39E4 = 0x3F4978D5;
static const float data_39E8[33] = {
  0.787f, 0.866f, 0.944f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.944f, 0.866f, 0.787f,
  0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f
};
static const uint8_t data_3A6C[80] = {0};
typedef struct Row_3ABC { float w0; float w1; float w2; int32_t w3; } Row_3ABC;
static const Row_3ABC data_3ABC[3] = {
  {1.0f, 1.0f, 1.0f, 0},
  {0.0f, 0.0f, 0.0f, 0},
  {0.0f, 0.0f, 0.0f, 0}
};
static const int32_t data_3AEC[2] = {0, 0};
static const uint8_t data_3AF4[24] = {0};
static const float data_3B0C[28] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f
};
static const uint8_t data_3B7C[24] = {0};
static const float data_3B94[28] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f
};
static const uint32_t data_3C04 = 0x3F4978D5;
static const float data_3C08[33] = {
  0.787f, 0.866f, 0.944f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  0.944f, 0.866f, 0.866f, 0.866f, 0.866f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f
};
static const uint8_t data_3C8C[24] = {0};
static const float data_3CA4[28] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f
};
static const uint32_t data_3D14 = 0x3F800000;
static const float data_3D18[33] = {
  0.944f, 0.866f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f,
  0.787f, 0.787f, 0.551f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint8_t data_3D9C[104] = {0};
static const float data_3E04[8] = {1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f};
static const uint8_t data_3E24[96] = {0};
static const float data_3E84[10] = {1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f};
static const uint32_t data_3EAC = 0x3F266666;
static const float data_3EB0[33] = {
  0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f,
  0.708f, 0.787f, 0.866f, 0.944f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint32_t data_3F34 = 0x3F800000;
static const float data_3F38[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint8_t data_3FBC[16] = {0};
static const float data_3FCC[30] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint8_t data_4044[16] = {0};
static const float data_4054[30] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint8_t data_40CC[80] = {0};
static const float data_411C[14] = {1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f};
static const uint32_t data_4154 = 0x3F800000;
static const float data_4158[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const int32_t data_41DC[63] = {
  0, 0, -7281, 0, 2730, -7281, 0, 1456, 0, 0, 0, -18022, 0, 0, -15291, 0, 1092, -16019, 0, 0, 728, 0, 0, -12196, 0, 0, -15109, 0, 364, -14745, 0, 0,
  728, 0, 0, -9466, 0, 728, -14017, 0, 546, -12379, 0, 0, 0, 0, 819, -12014, 0, -1274, -9648, 0, -1638, -11650, 0, 0, 0, -1031244415, 1034469717,
  1112912848, 0, 13034, 0
};
static const Vector4 data_42D8 = {.x = 1.0f, .y = 0.5f, .z = 0.5f, .w = 0.5f};
static const Vector4 data_42E8 = {.x = 1.0f, .y = 0.5f, .z = 0.5f, .w = 0.5f};
static const Vector3i data_42F8 = {.x = -546, .y = -1270, .z = -545};
static const int32_t data_4304[10] = {63715, 728, 0, 0, 62259, 0, 0, 62077, 0, 61531};
static const Vector3i data_432C = {.x = -546, .y = -1270, .z = -545};
static Vector3i data_4338 = {.x = 0, .y = 0, .z = 0};
static const char data_4344[16] __attribute__((scn_string_literal)) = "/scene/03/DXMS/";
static const char data_4354[5] __attribute__((scn_string_literal)) = "XMAS";
static const char data_4359[16] __attribute__((scn_string_literal)) = "/scene/03/DXMS/";
static const char data_4369[5] __attribute__((scn_string_literal)) = "DOOR";
static const char data_436E[6] __attribute__((scn_string_literal)) = "ETCS";
static const Vector3 data_4374 = {.x = 0.145f, .y = 0.20321f, .z = 0.08155f};
static const Vector3i data_4380 = {.x = 33446, .y = 46151, .z = 14974};
