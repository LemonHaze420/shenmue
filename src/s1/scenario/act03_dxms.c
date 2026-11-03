#include "act03_dxms.h"

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

static void script_0030(void) {
  Script_ambSoundUtils(1, 1, &data_2054, 80.0f, 100.0f, 1.0f, 0, &data_20F0);
  Script_ambSoundUtils(2, 1, &data_2060, 30.0f, 150.0f, 1.0f, 2, data_2178);
  Script_ambSoundUtils(3, 0, &data_206C, 20.0f, 90.0f, 0.8f, 0, data_2200);
  Script_ambSoundUtils(4, 0, &data_2078, 3.0f, 10.0f, 0.8f, 0, data_2288);
  Script_ambSoundUtils(5, 1, &data_2084, 15.0f, 120.0f, 0.8f, 0, &data_2310);
  Script_ambSoundUtils(6, 0, &data_2090, 1.5f, 3.0f, 0.8f, 0, data_2398);
  Script_ambSoundUtils(7, 1, &data_209C, 30.0f, 70.0f, 0.9f, 2, &data_2420);
  Script_ambSoundUtils(8, 1, &data_20A8, 30.0f, 150.0f, 1.0f, 0, data_24A8);
  Script_ambSoundUtils(9, 0, &data_20B4, 3.0f, 60.0f, 0.8f, 0, data_2530);
  Script_ambSoundUtils(10, 1, &data_20C0, 28.0f, 70.0f, 1.0f, 0, &data_25B8);
  Script_ambSoundUtils(11, 0, &data_20CC, 5.0f, 60.0f, 1.0f, 0, &data_2640);
  Script_ambSoundUtils(12, 1, &data_20D8, 30.0f, 170.0f, 0.8f, 0, &data_26C8);
  Script_ambSoundUtils(15, 1, &data_20E4, 100.0f, 170.0f, 1.0f, 3, &data_2750);
  Script_Ambient1SoundUtils(0, 23976);
  return;
}

static void script_0211(void) {
  Script_ambSoundUtils(1, 1, &data_27D8, 80.0f, 100.0f, 1.0f, 0, &data_288C);
  Script_ambSoundUtils(2, 0, &data_27E4, 2.5f, 25.0f, 1.0f, 0, data_2914);
  Script_ambSoundUtils(3, 0, &data_27F0, 2.5f, 25.0f, 1.0f, 0, data_299C);
  Script_ambSoundUtils(4, 0, &data_27FC, 3.0f, 10.0f, 1.0f, 0, data_2A24);
  Script_ambSoundUtils(5, 1, &data_2808, 15.0f, 120.0f, 1.0f, 0, &data_2AAC);
  Script_ambSoundUtils(6, 0, &data_2814, 1.5f, 3.0f, 0.8f, 0, data_2B34);
  Script_ambSoundUtils(7, 1, &data_2820, 30.0f, 70.0f, 1.0f, 2, &data_2BBC);
  Script_ambSoundUtils(8, 0, &data_282C, 2.5f, 25.0f, 1.0f, 0, data_2C44);
  Script_ambSoundUtils(9, 0, &data_2838, 5.0f, 40.0f, 1.0f, 0, data_2CCC);
  Script_ambSoundUtils(10, 1, &data_2844, 28.0f, 70.0f, 1.0f, 0, &data_2D54);
  Script_ambSoundUtils(11, 0, &data_2850, 5.0f, 40.0f, 1.0f, 0, &data_2DDC);
  Script_ambSoundUtils(12, 0, &data_285C, 2.5f, 25.0f, 1.0f, 0, data_2E64);
  Script_ambSoundUtils(13, 0, &data_2868, 2.5f, 25.0f, 1.0f, 0, data_2EEC);
  Script_ambSoundUtils(14, 0, &data_2874, 2.5f, 25.0f, 1.0f, 0, data_2F74);
  Script_ambSoundUtils(15, 0, &data_2880, 2.5f, 25.0f, 1.0f, 0, data_2FFC);
  Script_Ambient1SoundUtils(0, 23976);
  return;
}

static void script_043A(void) {
  Script_ambSoundUtils(1, 1, &data_3084, 80.0f, 100.0f, 1.0f, 0, &data_3138);
  Script_ambSoundUtils(2, 0, &data_3090, 3.5f, 30.0f, 1.0f, 0, data_31C0);
  Script_ambSoundUtils(3, 0, &data_309C, 20.0f, 90.0f, 1.0f, 0, data_3248);
  Script_ambSoundUtils(4, 0, &data_30A8, 3.0f, 10.0f, 1.0f, 0, data_32D0);
  Script_ambSoundUtils(5, 1, &data_30B4, 15.0f, 120.0f, 1.0f, 0, &data_3358);
  Script_ambSoundUtils(6, 0, &data_30C0, 1.5f, 3.0f, 1.0f, 0, data_33E0);
  Script_ambSoundUtils(7, 1, &data_30CC, 30.0f, 70.0f, 1.0f, 2, &data_3468);
  Script_ambSoundUtils(8, 1, &data_30D8, 30.0f, 150.0f, 1.0f, 0, data_34F0);
  Script_ambSoundUtils(9, 0, &data_30E4, 3.0f, 60.0f, 1.0f, 0, data_3578);
  Script_ambSoundUtils(9, 0, &data_30E4, 3.0f, 60.0f, 1.0f, 0, data_3578);
  Script_ambSoundUtils(10, 1, &data_30F0, 28.0f, 70.0f, 1.0f, 0, &data_3600);
  Script_ambSoundUtils(11, 0, &data_30FC, 5.0f, 60.0f, 1.0f, 0, &data_3688);
  Script_ambSoundUtils(12, 0, &data_3108, 3.5f, 30.0f, 1.0f, 0, data_3710);
  Script_ambSoundUtils(13, 0, &data_3114, 3.5f, 30.0f, 1.0f, 0, data_3798);
  Script_ambSoundUtils(14, 0, &data_3120, 10.0f, 30.0f, 1.0f, 0, data_3820);
  Script_ambSoundUtils(15, 1, &data_312C, 100.0f, 170.0f, 1.0f, 3, &data_38A8);
  Script_Ambient1SoundUtils(0, 23976);
  return;
}

static void script_0687(void) {
  Script_ambSoundUtils(1, 1, &data_3930, 80.0f, 100.0f, 1.0f, 0, &data_39E4);
  Script_ambSoundUtils(2, 1, &data_393C, 30.0f, 150.0f, 1.0f, 2, data_3A6C);
  Script_ambSoundUtils(3, 0, &data_3948, 20.0f, 90.0f, 1.0f, 0, data_3AF4);
  Script_ambSoundUtils(4, 0, &data_3954, 3.0f, 10.0f, 1.0f, 0, data_3B7C);
  Script_ambSoundUtils(5, 1, &data_3960, 15.0f, 120.0f, 1.0f, 0, &data_3C04);
  Script_ambSoundUtils(6, 0, &data_396C, 1.5f, 3.0f, 1.0f, 0, data_3C8C);
  Script_ambSoundUtils(7, 1, &data_3978, 30.0f, 70.0f, 1.0f, 2, &data_3D14);
  Script_ambSoundUtils(8, 1, &data_3984, 30.0f, 150.0f, 1.0f, 0, data_3D9C);
  Script_ambSoundUtils(9, 0, &data_3990, 3.0f, 60.0f, 1.0f, 0, data_3E24);
  Script_ambSoundUtils(10, 1, &data_399C, 28.0f, 70.0f, 1.0f, 0, &data_3EAC);
  Script_ambSoundUtils(11, 0, &data_39A8, 5.0f, 60.0f, 1.0f, 0, &data_3F34);
  Script_ambSoundUtils(12, 0, &data_39B4, 3.5f, 30.0f, 1.0f, 0, data_3FBC);
  Script_ambSoundUtils(13, 0, &data_39C0, 3.5f, 30.0f, 1.0f, 0, data_4044);
  Script_ambSoundUtils(14, 0, &data_39CC, 10.0f, 30.0f, 1.0f, 0, data_40CC);
  Script_ambSoundUtils(15, 1, &data_39D8, 100.0f, 170.0f, 1.0f, 3, &data_4154);
  Script_Ambient1SoundUtils(0, 23976);
  return;
}

static void script_08B0(void) {
  if (-(task_0.month == 12) & -(task_0.day >= 15) & -(task_0.day <= 25)) {
    if (task_0.day >= 24) {
      script_0211();
    } else if (task_0.day >= 19) {
      script_043A();
    } else {
      script_0687();
    }
  } else if ((task_0.month == 1) & (task_0.day <= 5)) {
    script_0687();
  } else {
    script_0030();
  }
  return;
}

static void script_0931(int32_t arg0) {
  Script_mapmSetCamFlags(0, 0);
  Script_shenSeqUtils(0);
  while (Script_shenSeqUtils(-1)) {
    int32_t unused_FC;
    int8_t local_FB;
    local_FB = Script_shenSeqUtils(-7);
    if (local_FB == 1) {
      Script_executeAicaCommand(2720, 2, 30);
      Script_filmCheckOrSetTransitionUtil(0, 255, 0, 0, 0, 0, 0, 0, 0);
      SCNF_WaitFrames_0000(30);
      script_0FD8();
      Script_shenRequestGameModuleTransition(3, SHEN_NAME('MEND'), 0);
      SCNF_WaitFrames_0000(1);
    }
    SCNF_WaitFrames_0000(1);
  }
  script_16A7();
  Script_mapmSetCamFlags(0, 1);
  script_18D4();
  task_C = Script_scenCreateTask(script_1485, 2, SHEN_NAME('CHA1'), SHEN_NAME('ETB1'));
  Script_shenSeqUtils(1);
  while (Script_shenSeqUtils(-1)) {
    int32_t unused_FC;
    int8_t local_FB;
    local_FB = Script_shenSeqUtils(-7);
    if (local_FB == 1) {
      Script_executeAicaCommand(2720, 2, 30);
      Script_filmCheckOrSetTransitionUtil(0, 255, 0, 0, 0, 0, 0, 0, 0);
      SCNF_WaitFrames_0000(30);
      Script_srchControl((void *)task_C);
      script_0FD8();
      Script_shenRequestGameModuleTransition(3, SHEN_NAME('MEND'), 0);
      SCNF_WaitFrames_0000(1);
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_srchControl((void *)task_C);
  script_16A7();
  Script_mapmSetCamFlags(0, 0);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 51.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 20.0f);
  {
    int32_t local_FC;
    local_FC = Script_scenCreateTask(script_119D, 0);
    task_C = Script_scenCreateTask(script_1485, 2, SHEN_NAME('CHA1'), SHEN_NAME('ETB1'));
    script_19D5();
    Script_shenSeqUtils(2);
    while (Script_shenSeqUtils(-1)) {
      int8_t local_FB;
      local_FB = Script_shenSeqUtils(-7);
      if (local_FB == 1) {
        Script_executeAicaCommand(2720, 2, 30);
        Script_filmCheckOrSetTransitionUtil(0, 255, 0, 0, 0, 0, 0, 0, 0);
        SCNF_WaitFrames_0000(30);
        Script_srchControl((void *)local_FC);
        Script_srchControl((void *)task_C);
        script_0FD8();
        Script_shenRequestGameModuleTransition(3, SHEN_NAME('MEND'), 0);
        SCNF_WaitFrames_0000(1);
      }
      SCNF_WaitFrames_0000(1);
    }
    Script_srchControl((void *)local_FC);
  }
  Script_srchControl((void *)task_C);
  script_16A7();
  Script_mapmSetRenderFlag(4, 1);
  Script_mapmSetRenderFlag(5, 1);
  Script_mapmSetRenderFlag(6, 1);
  Script_mapmSetRenderFlag(7, 1);
  Script_mapmSetRenderFlag(8, 1);
  Script_mapmSetRenderFlag(9, 1);
  Script_mapmSetRenderFlag(10, 1);
  Script_mapmSetRenderFlag(11, 1);
  Script_mapmSetRenderFlag(12, 1);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 55.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 40.0f);
  script_1A85();
  task_C = Script_scenCreateTask(script_1485, 2, SHEN_NAME('CHA1'), SHEN_NAME('ETB4'));
  {
    int32_t local_FC;
    local_FC = Script_scenCreateTask(script_12BA, 0);
    Script_shenSeqUtils(3);
    while (Script_shenSeqUtils(-1)) {
      int8_t local_FB;
      local_FB = Script_shenSeqUtils(-7);
      if (local_FB == 1) {
        Script_executeAicaCommand(2720, 2, 30);
        Script_filmCheckOrSetTransitionUtil(0, 255, 0, 0, 0, 0, 0, 0, 0);
        SCNF_WaitFrames_0000(30);
        Script_srchControl((void *)local_FC);
        Script_srchControl((void *)task_C);
        script_0FD8();
        Script_shenRequestGameModuleTransition(3, SHEN_NAME('MEND'), 0);
        SCNF_WaitFrames_0000(1);
      }
      SCNF_WaitFrames_0000(1);
    }
    Script_srchControl((void *)task_C);
    Script_srchControl((void *)local_FC);
  }
  script_16A7();
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 55.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 0.0f);
  script_1B07();
  task_C = Script_scenCreateTask(script_1485, 2, SHEN_NAME('CHA1'), SHEN_NAME('ETB2'));
  task_10 = Script_scenCreateTask(script_1485, 2, SHEN_NAME('CHA2'), SHEN_NAME('ETB5'));
  Script_filmCheckOrSetTransitionUtil(60, 255, 255, 255, 255, 0, 0, 0, 0);
  {
    int32_t local_FC;
    local_FC = Script_scenCreateTask(script_1377, 0);
    Script_shenSeqUtils(4);
    while (Script_shenSeqUtils(-1)) {
      int8_t local_FB;
      local_FB = Script_shenSeqUtils(-7);
      if (local_FB == 1) {
        Script_executeAicaCommand(2720, 2, 30);
        Script_filmCheckOrSetTransitionUtil(0, 255, 0, 0, 0, 0, 0, 0, 0);
        SCNF_WaitFrames_0000(30);
        Script_srchControl((void *)local_FC);
        Script_srchControl((void *)task_C);
        Script_srchControl((void *)task_10);
        script_0FD8();
        Script_shenRequestGameModuleTransition(3, SHEN_NAME('MEND'), 0);
        SCNF_WaitFrames_0000(1);
      }
      SCNF_WaitFrames_0000(1);
    }
    Script_srchControl((void *)local_FC);
  }
  Script_srchControl((void *)task_C);
  Script_srchControl((void *)task_10);
  script_16A7();
  return;
}

static void script_0D81(void) {
  script_0F32(CHARA_AKIR);
  script_0EB9(SHEN_NAME('ETC1'));
  script_0EB9(SHEN_NAME('ETC2'));
  script_0EB9(SHEN_NAME('ETC5'));
  script_0EB9(SHEN_NAME('ETC6'));
  script_0EB9(SHEN_NAME('ETB1'));
  script_0EB9(SHEN_NAME('ETB2'));
  script_0EB9(SHEN_NAME('ETB4'));
  script_0EB9(SHEN_NAME('ETB5'));
  script_0EB9(SHEN_NAME('GAK1'));
  script_0EB9(SHEN_NAME('GAK2'));
  script_0EB9(CHARA_MASR);
  script_0EB9(CHARA_YOSE);
  script_0E8C(SHEN_NAME('CHA1'));
  script_0E8C(SHEN_NAME('CHA2'));
  script_0E8C(SHEN_NAME('ABAG'));
  script_0E8C(SHEN_NAME('CAR1'));
  script_0E8C(SHEN_NAME('CAR2'));
  Script_ccowSetFlag80(SHEN_NAME('DOG_'), 0);
  Script_ccowUtils(SHEN_NAME('DOG_'), 2);
  Script_charaDisplay(SHEN_NAME('ABAG'), CDM_Show);
  Script_SetShadowRange(1000.0f);
  script_1BD8();
  return;
}

static void script_0E8C(int32_t arg0) {
  Script_charaDisplay(arg0, CDM_Hide);
  Script_ccowSetFlag80(arg0, 1);
  Script_ccowUtils(arg0, 2);
  Script_charaVisUtils(arg0, 1);
  Script_SetCPAMFlags(arg0, 0);
  return;
}

static void script_0EB9(int32_t arg0) {
  Script_charaSetRefboardFlagTo(arg0, 1);
  Script_SetCharaRefBoardPos(arg0, &data_42D8);
  Script_charaDisplay(arg0, CDM_Hide);
  Script_motmToggleFlags(arg0, 2, 0x40);
  Script_ccowSetFlag80(arg0, 1);
  Script_ccowUtils(arg0, 2);
  Script_CharHandlers(arg0, 1, 8, 16);
  Script_CharHandlers(arg0, 0, 8, 16);
  Script_charaVisUtils(arg0, 1);
  Script_SetCPAMFlags(arg0, 0);
  Script_clmdSetFlags(arg0, 2, 0);
  Script_scenToggleVisibleFor(arg0, 0);
  return;
}

static void script_0F32(int32_t arg0) {
  Script_charaSetRefboardFlagTo(arg0, 1);
  Script_SetCharaRefBoardPos(arg0, &data_42E8);
  Script_charaDisplay(arg0, CDM_Show);
  Script_motmToggleFlags(arg0, 2, 0x40);
  Script_ccowSetFlag80(arg0, 1);
  Script_ccowUtils(arg0, 2);
  Script_facemotUtils(arg0, 0, 0);
  Script_FaceEx(arg0, 1, 0, 2);
  Script_facemotUtils(arg0, 1, 0);
  Script_FACE_LCG(arg0, 0, 6, 0);
  Script_CharHandlers(arg0, 1, 8, 16);
  Script_CharHandlers(arg0, 0, 8, 16);
  Script_FaceLookAtTargetUtils(arg0, 0);
  Script_charaVisUtils(arg0, 1);
  Script_SetCPAMFlags(arg0, 0);
  Script_clmdSetFlags(arg0, 2, 0);
  return;
}

static void script_0FD8(void) {
  script_111A(CHARA_AKIR);
  script_10BF(SHEN_NAME('ETC1'));
  script_10BF(SHEN_NAME('ETC2'));
  script_10BF(SHEN_NAME('ETC5'));
  script_10BF(SHEN_NAME('ETC6'));
  script_10BF(SHEN_NAME('ETB1'));
  script_10BF(SHEN_NAME('ETB2'));
  script_10BF(SHEN_NAME('ETB4'));
  script_10BF(SHEN_NAME('ETB5'));
  script_10BF(SHEN_NAME('GAK1'));
  script_10BF(SHEN_NAME('GAK2'));
  script_10BF(CHARA_MASR);
  script_10BF(CHARA_YOSE);
  script_10B3(SHEN_NAME('CHA1'));
  script_10B3(SHEN_NAME('CHA2'));
  script_10B3(SHEN_NAME('ABAG'));
  script_10B3(SHEN_NAME('CAR1'));
  script_10B3(SHEN_NAME('CAR2'));
  return;
}

static void script_10B3(int32_t arg0) {
  Script_SetCPAMFlags(arg0, 1);
  return;
}

static void script_10BF(int32_t arg0) {
  Script_SetCharaLookAtPosition(arg0, -98310, 0, 0);
  Script_SetCPAMFlags(arg0, 1);
  Script_ccowUtils(arg0, 1);
  Script_ccowSetFlag80(arg0, 2);
  Script_charaToggleMotEnabled(arg0, 1);
  Script_CharHandlers(arg0, -1, 0, 0);
  Script_SetCPAMFlags(arg0, 1);
  Script_SetMotnFlag(arg0, 0);
  Script_charaSetRefboardFlagTo(arg0, 0);
  return;
}

static void script_111A(int32_t arg0) {
  Script_FaceLookAtTargetUtils(arg0, 0);
  Script_SetCharaLookAtPosition(arg0, -98310, 0, 0);
  Script_ccowUtils(arg0, 1);
  Script_ccowSetFlag80(arg0, 2);
  Script_FACE(arg0, 0, 0, 0);
  Script_FaceLookAtTargetUtils(arg0, 0);
  Script_CharHandlers(arg0, -1, 0, 0);
  Script_SetCPAMFlags(arg0, 1);
  Script_ResetHandMotion(arg0, 0, 0, 0);
  Script_ResetHandMotion(arg0, 0, 1, 0);
  Script_SetMotnFlag(arg0, 0);
  Script_charaSetRefboardFlagTo(arg0, 0);
  return;
}

static int32_t script_119D(void) {
  int16_t local_FE;
  local_FE = 0;
  Script_mapmSetRenderFlag(4, 0);
  Script_mapmSetRenderFlag(5, 0);
  Script_mapmSetRenderFlag(6, 0);
  Script_mapmSetRenderFlag(7, 0);
  Script_mapmSetRenderFlag(8, 0);
  Script_mapmSetRenderFlag(10, 0);
  Script_mapmSetRenderFlag(12, 0);
  while (1) {
    if (Script_shenSeqUtils(-1)) {
      switch (local_FE) {
        case 50:
          Script_scenToggleVisibleFor(SHEN_NAME('ETB1'), 0);
          Script_charaDisplay(SHEN_NAME('ETB1'), CDM_Hide);
          break;
        case 165:
          Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 25.0f);
          Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 0.0f);
          break;
        case 166:
          Script_mapmSetCamFlags(0, 1);
          Script_charaDisplay(SHEN_NAME('OB02'), CDM_Hide);
          Script_mapmSetRenderFlag(9, 0);
          Script_mapmSetRenderFlag(11, 0);
          break;
        case 210:
          Script_srchControl((void *)task_C);
          break;
        case 211:
          task_C = Script_scenCreateTask(script_1485, 2, SHEN_NAME('CHA1'), SHEN_NAME('ETB4'));
          break;
      }
      local_FE = local_FE + 1;
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_12BA(void) {
  int16_t local_FE;
  local_FE = 0;
  Script_charaDisplay(SHEN_NAME('OB01'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('OB10'), CDM_Hide);
  while (1) {
    if (Script_shenSeqUtils(-1)) {
      switch (local_FE) {
        case 0:
          Script_ResetHandMotion(CHARA_AKIR, 1, 1, 0);
          Script_SetHandRootRotation(CHARA_AKIR, 1, 0x15, &data_42F8);
          Script_handSetFloatsFor(CHARA_AKIR, 1, data_41DC, 0);
          Script_ReadModelFormat7(CHARA_AKIR, 1, data_4304, 0);
          break;
        case 570:
          Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 255, 255, 255);
          break;
      }
      local_FE = local_FE + 1;
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_1377(void) {
  int16_t local_FE;
  local_FE = 0;
  while (1) {
    if (Script_shenSeqUtils(-1)) {
      switch (local_FE) {
        case 0:
          Script_PlayCharaMotion(SHEN_NAME('DOG_'), 59909, -1.0f, -1.0f, -1.0f, 1.0f, 0);
          Script_SetCharaMotionPlaybackRate(SHEN_NAME('DOG_'), 0.4f);
          Script_charaDisplay(SHEN_NAME('CHA2'), CDM_Hide);
          Script_ResetHandMotion(CHARA_AKIR, 1, 1, 0);
          Script_SetHandRootRotation(CHARA_AKIR, 1, 0x15, &data_432C);
          Script_handSetFloatsFor(CHARA_AKIR, 1, data_41DC, 0);
          break;
        case 920:
          Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
          break;
        case 925:
          Script_charaDisplay(SHEN_NAME('CHA2'), CDM_Show);
          break;
        case 1350:
          Script_executeAicaCommand(2720, 2, 30);
          Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
          break;
      }
      local_FE = local_FE + 1;
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_1485(int32_t arg0, int32_t arg1) {
  int8_t local_FF;
  float local_F8;
  Vector3i local_EC;
  Vector3i local_E0;
  Vector3i local_D4;
  local_FF = 0;
  local_EC.x = -3640;
  (&local_EC)->y = 0;
  (&local_EC)->z = 0;
  local_E0.x = -1875;
  (&local_E0)->y = 0;
  (&local_E0)->z = 0;
  local_D4.x = 1875;
  (&local_D4)->y = 0;
  (&local_D4)->z = 0;
  Script_charaPosRotUtils(arg0, 18, 0, &data_4338, 0);
  Script_charaPosRotUtils(arg0, 23, 0, &data_4338, 0);
  Script_charaPosRotUtils(arg0, 1, 0, &data_4338, 0);
  Script_charaPosRotUtils(arg0, 12, 0, &data_4338, 0);
  Script_charaPosRotUtils(arg0, 7, 0, &data_4338, 0);
  while (1) {
    if (local_FF == 1) {
      Script_charaPosRotUtils(arg0, 1, 0, &data_4338, 0);
      Script_charaPosRotUtils(arg0, 12, 0, &data_4338, 0);
      Script_charaPosRotUtils(arg0, 7, 0, &data_4338, 0);
    } else {
      Script_charaPosRotUtils(arg0, 18, 0, &local_EC, 1);
      Script_charaPosRotUtils(arg0, 23, 0, &local_EC, 1);
      Script_charaPosRotUtils(arg0, 1, 0, &local_E0, 1);
      Script_charaPosRotUtils(arg0, 12, 0, &local_D4, 1);
      Script_charaPosRotUtils(arg0, 7, 0, &local_D4, 1);
    }
    local_F8 = Script_GetCharaMotionRemainingFramesScaled(arg1);
    if ((((scn3_bits){.f = local_F8}).i == ((scn3_bits){.f = (float)34}).i) & (local_FF == 0)) {
      local_FF = 1;
    } else {
      if ((((scn3_bits){.f = local_F8}).i == ((scn3_bits){.f = (float)34}).i) & (local_FF == 1)) {
        local_FF = 0;
        goto L001602;
      }
    }
    L001602:;
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_160E(void) {
  int32_t unused_FC;
  Vector3i local_F0;
  local_F0.x = -10922;
  (&local_F0)->y = 0;
  (&local_F0)->z = 0;
  while (1) {
    Script_charaPosRotUtils(SHEN_NAME('CAR1'), 18, 0, &local_F0, 1);
    Script_charaPosRotUtils(SHEN_NAME('CAR1'), 13, 0, &local_F0, 1);
    Script_charaPosRotUtils(SHEN_NAME('CAR1'), 8, 0, &local_F0, 1);
    Script_charaPosRotUtils(SHEN_NAME('CAR2'), 18, 0, &local_F0, 1);
    Script_charaPosRotUtils(SHEN_NAME('CAR2'), 13, 0, &local_F0, 1);
    Script_charaPosRotUtils(SHEN_NAME('CAR2'), 8, 0, &local_F0, 1);
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_16A7(void) {
  Script_charaSetRefboardFlagTo(CHARA_YOSE, 0);
  Script_charaSetRefboardFlagTo(CHARA_MASR, 0);
  Script_charaSetRefboardFlagTo(SHEN_NAME('ETC1'), 0);
  Script_charaSetRefboardFlagTo(SHEN_NAME('ETC2'), 0);
  Script_charaSetRefboardFlagTo(SHEN_NAME('ETC5'), 0);
  Script_charaSetRefboardFlagTo(SHEN_NAME('ETC6'), 0);
  Script_charaSetRefboardFlagTo(SHEN_NAME('GAK1'), 0);
  Script_charaSetRefboardFlagTo(SHEN_NAME('GAK2'), 0);
  Script_charaSetRefboardFlagTo(SHEN_NAME('DOG_'), 0);
  Script_charaSetRefboardFlagTo(SHEN_NAME('ETB1'), 0);
  Script_charaSetRefboardFlagTo(SHEN_NAME('ETB2'), 0);
  Script_charaSetRefboardFlagTo(SHEN_NAME('ETB4'), 0);
  Script_charaSetRefboardFlagTo(SHEN_NAME('ETB5'), 0);
  Script_scenToggleVisibleFor(CHARA_YOSE, 0);
  Script_scenToggleVisibleFor(CHARA_MASR, 0);
  Script_scenToggleVisibleFor(SHEN_NAME('ETC1'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('ETC2'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('ETC5'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('ETC6'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('GAK1'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('GAK2'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('ETB1'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('ETB2'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('ETB4'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('ETB5'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('DOG_'), 0);
  Script_charaDisplay(CHARA_YOSE, CDM_Hide);
  Script_charaDisplay(CHARA_MASR, CDM_Hide);
  Script_charaDisplay(SHEN_NAME('ETC1'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('ETC2'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('ETC5'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('ETC6'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('GAK1'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('GAK2'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('ETB1'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('ETB2'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('ETB4'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('ETB5'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('DOG_'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('CHA1'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('CHA2'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('CAR1'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('CAR2'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('OB01'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('OB02'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('OB09'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('OB10'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('OB13'), CDM_Hide);
  return;
}

static void script_18D4(void) {
  Script_charaSetRefboardFlagTo(SHEN_NAME('GAK1'), 1);
  Script_charaSetRefboardFlagTo(SHEN_NAME('GAK2'), 1);
  Script_charaSetRefboardFlagTo(SHEN_NAME('ETC1'), 1);
  Script_charaSetRefboardFlagTo(SHEN_NAME('ETC2'), 1);
  Script_charaSetRefboardFlagTo(SHEN_NAME('ETC5'), 1);
  Script_charaSetRefboardFlagTo(SHEN_NAME('ETB1'), 1);
  Script_scenToggleVisibleFor(SHEN_NAME('ETC1'), 1);
  Script_scenToggleVisibleFor(SHEN_NAME('ETC2'), 1);
  Script_scenToggleVisibleFor(SHEN_NAME('ETC5'), 1);
  Script_scenToggleVisibleFor(SHEN_NAME('GAK1'), 1);
  Script_scenToggleVisibleFor(SHEN_NAME('GAK2'), 1);
  Script_scenToggleVisibleFor(SHEN_NAME('ETB1'), 1);
  Script_charaDisplay(SHEN_NAME('ETC1'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('ETC2'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('ETC5'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('ETB1'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('GAK1'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('GAK2'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('CHA1'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('OB01'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('OB02'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('OB13'), CDM_Show);
  return;
}

static void script_19D5(void) {
  Script_charaSetRefboardFlagTo(CHARA_YOSE, 1);
  Script_charaSetRefboardFlagTo(CHARA_MASR, 1);
  Script_charaSetRefboardFlagTo(SHEN_NAME('ETB1'), 1);
  Script_charaSetRefboardFlagTo(SHEN_NAME('ETB4'), 1);
  Script_scenToggleVisibleFor(CHARA_YOSE, 1);
  Script_scenToggleVisibleFor(CHARA_MASR, 1);
  Script_scenToggleVisibleFor(SHEN_NAME('ETB1'), 1);
  Script_scenToggleVisibleFor(SHEN_NAME('ETB4'), 1);
  Script_charaDisplay(CHARA_YOSE, CDM_Show);
  Script_charaDisplay(CHARA_MASR, CDM_Show);
  Script_charaDisplay(SHEN_NAME('ETB1'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('ETB4'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('CHA1'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('OB01'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('OB13'), CDM_Show);
  return;
}

static void script_1A85(void) {
  Script_charaSetRefboardFlagTo(CHARA_YOSE, 1);
  Script_charaSetRefboardFlagTo(CHARA_MASR, 1);
  Script_charaSetRefboardFlagTo(SHEN_NAME('ETB4'), 1);
  Script_scenToggleVisibleFor(CHARA_YOSE, 1);
  Script_scenToggleVisibleFor(CHARA_MASR, 1);
  Script_scenToggleVisibleFor(SHEN_NAME('ETB4'), 1);
  Script_charaDisplay(CHARA_YOSE, CDM_Show);
  Script_charaDisplay(CHARA_MASR, CDM_Show);
  Script_charaDisplay(SHEN_NAME('ETB4'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('CHA1'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('OB13'), CDM_Show);
  return;
}

static void script_1B07(void) {
  Script_charaSetRefboardFlagTo(SHEN_NAME('ETC6'), 1);
  Script_charaSetRefboardFlagTo(SHEN_NAME('ETB2'), 1);
  Script_charaSetRefboardFlagTo(SHEN_NAME('ETB5'), 1);
  Script_charaSetRefboardFlagTo(SHEN_NAME('DOG_'), 1);
  Script_scenToggleVisibleFor(SHEN_NAME('ETC6'), 1);
  Script_scenToggleVisibleFor(SHEN_NAME('ETB2'), 1);
  Script_scenToggleVisibleFor(SHEN_NAME('ETB5'), 1);
  Script_scenToggleVisibleFor(SHEN_NAME('DOG_'), 1);
  Script_charaDisplay(SHEN_NAME('ETC6'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('ETB2'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('ETB5'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('DOG_'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('CHA1'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('CHA2'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('CAR1'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('CAR2'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('OB09'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('OB10'), CDM_Show);
  return;
}

static void script_1BD8(void) {
  script_1C53(SHEN_NAME('DR01'));
  script_1C53(SHEN_NAME('DR02'));
  script_1C53(SHEN_NAME('DR03'));
  script_1C53(SHEN_NAME('DR04'));
  script_1C53(SHEN_NAME('DR05'));
  script_1C53(SHEN_NAME('DR06'));
  script_1C53(SHEN_NAME('DR07'));
  script_1C53(SHEN_NAME('DR08'));
  script_1C53(SHEN_NAME('DR09'));
  script_1C53(SHEN_NAME('DR10'));
  return;
}

static void script_1C53(int32_t arg0) {
  {
    Vector3 local_F4;
    local_F4.x = (&local_F4)->y = (&local_F4)->z = 0.0f;
    if (Script_shenGetNode(arg0, 7)) {
      if (Script_shenGetNode(arg0, 5) & Script_shenGetNode(arg0, 10)) {
        local_F4.x = 0.0f;
        (&local_F4)->y = 0.0f;
        (&local_F4)->z = 0.0f;
        Script_charaPosRotUtils(arg0, 10, &local_F4, 0, 0);
        Script_charaPosRotUtils(arg0, 5, &local_F4, 0, 0);
      }
    } else {
      if (Script_shenGetNode(arg0, 10)) {
        local_F4.x = 0.0f;
        (&local_F4)->y = 0.0f;
        (&local_F4)->z = 0.0f;
        Script_charaPosRotUtils(arg0, 10, &local_F4, 0, 0);
      }
    }
  }
  if (Script_shenGetNode(arg0, 8) & Script_shenGetNode(arg0, 13)) {
    Vector3 local_F4;
    local_F4.x = 0.0f;
    (&local_F4)->y = 0.0f;
    (&local_F4)->z = 0.0f;
    Script_charaPosRotUtils(arg0, 13, &local_F4, 0, 0);
    (&local_F4)->y = -1000.0f;
    Script_charaPosRotUtils(arg0, 8, &local_F4, 0, 0);
  }
  return;
}

void script_1D54(void) {
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_filmCheckOrSetTransitionUtil(0, 255, 0, 0, 0, 0, 0, 0, 0);
  Script_scenCreateTask(script_1D84, 1, CHARA_AKIR);
  return;
}

__attribute__((scn_string_bindings(0, data_4344, 2, data_4359)))
static int32_t script_1D84(int32_t arg0) {
  int8_t local_FF;
  int32_t local_F8;
  local_FF = Script_GetCurrentEntryPoint();
  Script_ReturnTimeDate(&task_0);
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, "/scene/03/DXMS/", "XMAS") == 0) {
    SCNF_WaitFrames_0000(1);
  }
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2)) {
    SCNF_WaitFrames_0000(1);
  }
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, "/scene/03/DXMS/", "DOOR") == 0) {
    SCNF_WaitFrames_0000(1);
  }
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2)) {
    SCNF_WaitFrames_0000(1);
  }
  script_0D81();
  Script_ryoSetChara(arg0);
  Script_GetSetWeatherSettings(WEATHER_SETTINGS_SET_DISABLE_WEATHER, 1);
  Script_WeatherUtils_0(WeatherDataUtil_ApplyCurrentWeatherOrEnableDateTimeRead, 0);
  Script_WeatherTaskUtil(InitTaskData);
  Script_mapmSetRenderFlag(14, 0);
  Script_mapmSetRenderFlag(15, 0);
  Script_mapmSetRenderFlag(16, 0);
  Script_mapmSetRenderFlag(17, 0);
  Script_mapmSetRenderFlag(18, 0);
  if (Script_WeatherUtils_0(WeatherDataUtil_QueryCurrentFCASFlag, 3)) {
    Script_mapmSetRenderFlag(15, 1);
  } else {
    Script_mapmSetRenderFlag(14, 1);
  }
  if (Script_WeatherUtils_0(WeatherDataUtil_QueryCurrentFCASFlag, 4)) {
    Script_CreateNPCImageModule(SHEN_NAME('ETC1'), 0, 0);
    Script_CreateNPCImageModule(SHEN_NAME('ETC1'), "ETCS", 0);
    Script_mapmSetRenderFlag(16, 1);
  } else {
    if (Script_WeatherUtils_0(WeatherDataUtil_QueryCurrentFCASFlag, 5)) {
      Script_mapmSetRenderFlag(17, 1);
      goto L001ECF;
    }
  }
  L001ECF:;
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 60.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 40.0f);
  local_F8 = Script_scenCreateTask(script_160E, 0);
  script_1FC4(CHARA_AKIR, SHEN_NAME('ABAG'));
  Script_WeatherUtils(WeatherScript_ApplyWccpClipSet1, -75.8f, 62.51f, -69.12f, 62.51f, -69.12f, 69.64f, -75.79f, 69.64f, -0.1f, 3.4f);
  script_08B0();
  Script_executeAicaCommand(1184, 1, 96);
  Script_executeAicaCommand(7080, 0, 0);
  Script_executeAicaCommand(1184, 2, 96);
  Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
  script_0931(5);
  Script_srchControl((void *)local_F8);
  script_0FD8();
  Script_fixoClearAttachment(SHEN_NAME('ABAG'));
  Script_SetRyoMotionDetails(CHARA_AKIR);
  Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 973, 1);
  Script_shenRequestGameModuleTransition(3, SHEN_NAME('MEND'), 0);
  SCNF_WaitFrames_0000(1);
  end:;
}

static void script_1FC4(int32_t arg0, int32_t arg1) {
  Script_ccowUtils(arg1, 2);
  Script_motmToggleFlags(arg0, 2, 0x40);
  Script_RequestFightAnimation(arg0, 4126, 0.0f, 13, 0);
  SCNF_WaitFrames_0000(1);
  Script_fixoAttachEx(arg1, arg0, 2, &data_4374, &data_4380);
  Script_CharHandlers(arg0, 0, 0, 16);
  Script_CharHandlers(arg0, 1, 8, 16);
  Script_charaDisplay(arg1, CDM_Show);
  return;
}
