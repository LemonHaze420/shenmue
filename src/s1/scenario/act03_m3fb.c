#include "act03_m3fb.h"

static void script_0030(int32_t arg0, float arg1, float arg2, float arg3);
static void script_0058(int32_t arg0, float arg1, float arg2, float arg3);
static void script_00AC(int32_t arg0, int32_t arg1, int32_t arg2);
static int32_t script_00FC(int32_t arg0);
static void script_01A3(int32_t arg0, int32_t arg1);
static int32_t script_01C6(int32_t arg0);
static void script_01F9(int32_t arg0, int32_t arg1);
static void script_021C(int32_t arg0, int32_t arg1);
static void script_027B(int32_t arg0);
static void script_0290(void);
static void script_044A(void);
static void script_0574(void);
static void script_05A5(void);
static void script_05D6(void);
static void script_06DC(void);
static void script_06E4(void);
static void script_06ED(void);
static void script_06FC(int32_t arg0);
static void script_0789(int32_t arg0);
static void script_07D9(void);
static void script_0807(int32_t arg0);
static void script_0835(void);
static void script_0869(int32_t arg0);
static void script_089D(int32_t arg0);
static void script_08BB(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_08F5(int32_t arg0, int32_t arg1);
static void script_0929(void);
static void script_0A21(int32_t arg0, int32_t arg1);
static void script_0A51(int32_t arg0);
static void script_0A80(int32_t arg0);
static void script_0B09(void);
static void script_0BC7(void);
static int32_t script_0D08(void);
static int32_t script_0F3E(void);
static void script_123B(void);
static void script_124E(void);
static int32_t script_1261(void);
static void script_139A(void);
static int32_t script_13A7(void);
static void script_162A(void);
static void script_167D(void);
static int32_t script_174F(void);
static int32_t script_17D9(void);
static int32_t script_1837(void);
static int32_t script_18B9(void);
static int32_t script_1983(void);
static void script_1A3F(void);
static int32_t script_1F01(void);
static int32_t script_20A2(void);
static int32_t script_20AE(void);
static int32_t script_20BA(void);
static int32_t script_20C6(void);
static int32_t script_20D2(void);
static int32_t script_20DE(void);
static int32_t script_20EA(void);
static int32_t script_20F6(void);
static int32_t script_2102(void);
static void script_210E(void);
static void script_211C(float arg0);
static void script_22EA(int32_t arg0, int32_t arg1);
static void script_231A(int32_t arg0, int32_t arg1);
static void script_234A(void);
static void script_24D1(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_2521(int32_t arg0, int32_t arg1);
static void script_25E0(int32_t arg0, int32_t arg1);
static void script_27A8(void);
static void script_281C(void);
static void script_2890(void);
static void script_28C3(void);
void script_28F2(void);
static int32_t script_2B46(void);
static void script_2CE9(void);
static void script_2D85(void);
static void script_2EAD(void);
static void script_2EE3(void);
static void script_307C(void);
static void script_3134(int8_t arg0);
static int32_t script_3165(int32_t arg0);
static int32_t script_3199(int32_t arg0);

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
static int8_t task_6C;
static uint8_t task_6D;
static uint16_t task_6E;
static int32_t task_70;
static Vector3 task_74;
static int32_t task_80;
static int8_t task_84;
static uint8_t task_85;
static uint16_t task_86;
static int32_t task_88;
static int8_t task_8C;
static uint8_t task_8D;
static uint16_t task_8E;
static int8_t task_90;
static uint8_t task_91;
static uint8_t task_92;
static const Vector3 data_31D0 = {.x = 1.0f, .y = 1.0f, .z = 1.0f};
static const int32_t data_31DC[4] = {16777215, 16777215, 16777215, 16777215};
static const float data_31EC[3] = {20.0f, 800.0f, 1.0f};
static const int32_t data_31F8 = -1;
static const uint32_t data_31FC = 0x41A00000;
static const float data_3200[2] = {0.0f, -50.0f};
static const uint32_t data_3208 = 0x41A00000;
static const float data_320C[2] = {0.0f, -50.0f};
static const uint32_t data_3214 = 0xC2200000;
static const float data_3218[2] = {0.0f, 80.0f};
static const uint32_t data_3220 = 0xC2D60000;
static const float data_3224[2] = {0.0f, 78.0f};
static const uint8_t data_322C[8] = {0};
static const float data_3234 = 90.0f;
static const uint32_t data_3238 = 0xC1A00000;
static const float data_323C[2] = {0.0f, -50.0f};
static const uint32_t data_3244 = 0xC1A00000;
static const float data_3248[2] = {0.0f, -50.0f};
static const uint32_t data_3250 = 0xC2E60000;
static const float data_3254[2] = {0.0f, 175.0f};
static const uint32_t data_325C = 0xC2DC0000;
static const float data_3260[2] = {0.0f, 10.0f};
static const uint32_t data_3268 = 0x42C80000;
static const float data_326C[2] = {0.0f, -40.0f};
static const uint32_t data_3274 = 0x43020000;
static const float data_3278[2] = {0.0f, 25.0f};
static const uint8_t data_3280[12] = {0};
static const uint32_t data_328C = 0x3F800000;
static const float data_3290[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint32_t data_3314 = 0x3F800000;
static const float data_3318[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint8_t data_339C[16] = {0};
static const float data_33AC[30] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint32_t data_3424 = 0x3F800000;
static const float data_3428[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint32_t data_34AC = 0x3F800000;
static const float data_34B0[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint8_t data_3534[84] = {0};
typedef struct Row_3588 { float w0; float w1; float w2; int32_t w3; } Row_3588;
static const Row_3588 data_3588[3] = {
  {0.787f, 0.787f, 0.787f, 0},
  {0.0f, 0.0f, 0.0f, 0},
  {0.0f, 0.0f, 0.0f, 0}
};
static const int32_t data_35B8 = 0;
static const uint32_t data_35BC = 0x3F800000;
static const float data_35C0[33] = {
  0.944f, 0.866f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f,
  0.787f, 0.787f, 0.787f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint32_t data_3644 = 0x3F4978D5;
static const float data_3648[33] = {
  0.787f, 0.866f, 0.944f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  0.944f, 0.866f, 0.787f, 0.787f, 0.787f, 0.787f, 0.708f, 0.708f, 0.708f, 0.708f
};
static const uint8_t data_36CC[12] = {0};
static const float data_36D8[31] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint32_t data_3754 = 0x3F800000;
static const float data_3758[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint32_t data_37DC = 0x3F800000;
static const float data_37E0[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint32_t data_3864 = 0x3F800000;
static const float data_3868[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint32_t data_38EC = 0x41A00000;
static const float data_38F0[2] = {0.0f, -50.0f};
static const uint32_t data_38F8 = 0x41A00000;
static const float data_38FC[2] = {0.0f, -50.0f};
static const uint32_t data_3904 = 0xC2200000;
static const float data_3908[2] = {0.0f, 80.0f};
static const uint32_t data_3910 = 0xC1A00000;
static const float data_3914[2] = {0.0f, -50.0f};
static const uint32_t data_391C = 0xC1A00000;
static const float data_3920[2] = {0.0f, -50.0f};
static const uint32_t data_3928 = 0xC2E60000;
static const float data_392C[2] = {0.0f, 175.0f};
static const uint32_t data_3934 = 0x42C80000;
static const float data_3938[2] = {0.0f, -40.0f};
static const uint8_t data_3940[12] = {0};
static const uint32_t data_394C = 0x3F800000;
static const float data_3950[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint32_t data_39D4 = 0x3F800000;
static const float data_39D8[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint8_t data_3A5C[16] = {0};
static const float data_3A6C[30] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint8_t data_3AE4[84] = {0};
typedef struct Row_3B38 { float w0; float w1; float w2; int32_t w3; } Row_3B38;
static const Row_3B38 data_3B38[3] = {
  {0.787f, 0.787f, 0.787f, 0},
  {0.0f, 0.0f, 0.0f, 0},
  {0.0f, 0.0f, 0.0f, 0}
};
static const int32_t data_3B68 = 0;
static const uint32_t data_3B6C = 0x3F800000;
static const float data_3B70[33] = {
  0.944f, 0.866f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f,
  0.787f, 0.787f, 0.787f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint32_t data_3BF4 = 0x3F4978D5;
static const float data_3BF8[33] = {
  0.787f, 0.866f, 0.944f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  0.944f, 0.866f, 0.787f, 0.787f, 0.787f, 0.787f, 0.708f, 0.708f, 0.708f, 0.708f
};
static const uint32_t data_3C7C = 0x3F800000;
static const float data_3C80[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint32_t data_3D04 = 0x3F800000;
static const float data_3D08[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint32_t data_3D8C = 0xC2A50000;
static const float data_3D90[2] = {0.0f, 68.0f};
static const uint32_t data_3D98 = 0x3F800000;
static const float data_3D9C[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint32_t data_3E20 = 0xC1D00000;
static const float data_3E24[2] = {0.0f, 76.5f};
static const uint32_t data_3E2C = 0x3F800000;
static const float data_3E30[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint32_t data_3EB4 = 0x41A00000;
static const float data_3EB8[2] = {0.0f, -50.0f};
static const uint32_t data_3EC0 = 0x41A00000;
static const float data_3EC4[2] = {0.0f, -50.0f};
static const uint32_t data_3ECC = 0xC2200000;
static const float data_3ED0[2] = {0.0f, 80.0f};
static const uint32_t data_3ED8 = 0xC1A00000;
static const float data_3EDC[2] = {0.0f, -50.0f};
static const uint32_t data_3EE4 = 0xC1A00000;
static const float data_3EE8[2] = {0.0f, -50.0f};
static const uint32_t data_3EF0 = 0xC2E60000;
static const float data_3EF4[5] = {0.0f, 175.0f, 100.0f, 0.0f, -40.0f};
static const uint8_t data_3F08[12] = {0};
static const uint32_t data_3F14 = 0x3F800000;
static const float data_3F18[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint32_t data_3F9C = 0x3F800000;
static const float data_3FA0[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint8_t data_4024[16] = {0};
static const float data_4034[30] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint8_t data_40AC[84] = {0};
typedef struct Row_4100 { float w0; float w1; float w2; int32_t w3; } Row_4100;
static const Row_4100 data_4100[3] = {
  {0.787f, 0.787f, 0.787f, 0},
  {0.0f, 0.0f, 0.0f, 0},
  {0.0f, 0.0f, 0.0f, 0}
};
static const int32_t data_4130 = 0;
static const uint32_t data_4134 = 0x3F800000;
static const float data_4138[33] = {
  0.944f, 0.866f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f,
  0.787f, 0.787f, 0.787f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint32_t data_41BC = 0x3F4978D5;
static const float data_41C0[33] = {
  0.787f, 0.866f, 0.944f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  0.944f, 0.866f, 0.787f, 0.787f, 0.787f, 0.787f, 0.708f, 0.708f, 0.708f, 0.708f
};
static const uint32_t data_4244 = 0x3F800000;
static const float data_4248[90] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const int32_t data_43B0[684] = {
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
static const int32_t data_4E60[57] = {
  0, 0, -3640, 0, 0, -2366, -4369, 1820, -546, 0, 0, -5279, 0, 0, -6007, 0, 0, -2912, 0, 0, -910, 0, 0, -6553, 0, 0, -6735, 0, 0, -3640, 0, 0, -182,
  0, 0, -5461, 0, 0, -6007, 0, 0, -2366, 0, 0, 0, 0, 0, -5643, 0, 0, -4733, 0, 0, -2366, -1456, 0, 728
};
static const Vector4 data_4F44 = {.x = 0.3f, .y = 0.3f, .z = 0.3f, .w = 0.3f};
static Vector3 data_4F54 = {.x = 0.0f, .y = 0.175f, .z = 0.0f};
static Vector3i data_4F60 = {.x = 0, .y = 0, .z = 0};
static Vector3i data_4F6C = {.x = -2730, .y = 0, .z = 0};
static const Vector3 data_4F78 = {.x = 0.0f, .y = 0.0f, .z = 0.0f};
static const uint32_t data_4F84 = SHEN_NAME('AKIR');
typedef struct Row_4F88 { int32_t w0; float w1; int32_t w2; float w3; float w4; int32_t w5; int32_t w6; shen_name w7; } Row_4F88;
static const Row_4F88 data_4F88[9] = {
  {200, -2.6f, 0, 4.2f, 0.0f, 0, 787, CHARA_HARY},
  {60, -2.6f, 0, 2.6f, 180.0f, 25, 2209, CHARA_JONZ},
  {60, -3.3f, 0, 0.4f, 180.0f, 23, 2205, CHARA_DICK},
  {60, -2.2f, 0, -0.1f, 180.0f, 24, 2209, CHARA_RBRT},
  {80, -2.7f, 0, 5.5f, 0.0f, 42, 2209, CHARA_BOB_},
  {80, -3.5f, 0, 5.5f, 0.0f, 43, 2205, CHARA_SERA},
  {60, 2.6f, 0, -4.5f, 90.0f, 11, 8201, CHARA_PAUR},
  {80, 2.6f, 0, -6.6f, 90.0f, 44, 8201, CHARA_SMTH},
  {80, 3.5f, 0, -4.8f, 45.0f, 26, 8198, CHARA_TONY}
};
static const int32_t data_50A8 = 80;
static const float data_50AC[4] = {3.5f, 0.0f, -6.3f, 135.0f};
static const int32_t data_50BC[2] = {22, 8198};
static const char data_50C4[6] __attribute__((scn_string_literal)) = "E1073";
static const char data_50CA[6] __attribute__((scn_string_literal)) = "E1073";
static const char data_50D0[10] __attribute__((scn_string_literal)) = "E1073B001";
static const char data_50DA[10] __attribute__((scn_string_literal)) = "E1073B002";
static const char data_50E4[10] __attribute__((scn_string_literal)) = "E1073B003";
static const char data_50EE[10] __attribute__((scn_string_literal)) = "E1073A001";
static const char data_50F8[10] __attribute__((scn_string_literal)) = "E1073B004";
static const char data_5102[10] __attribute__((scn_string_literal)) = "E1073A002";
static const char data_510C[10] __attribute__((scn_string_literal)) = "E1073B005";
static const char data_5116[10] __attribute__((scn_string_literal)) = "E1073A003";
static const char data_5120[12] __attribute__((scn_string_literal)) = "E1073B006";
static const Vector4 data_512C = {.x = 0.3f, .y = 0.3f, .z = 0.3f, .w = 0.3f};
static const char data_513C[16] __attribute__((scn_string_literal)) = "PRESS A BUTTON";
static const uint32_t data_514C = 0x40E561E5;
static const uint32_t data_5150[3] = {0x42C3D99AU, 0x41825DCCU, 0x42B16D29U};
static const float data_515C = 28.3678f;
static const uint32_t data_5160 = 0x42C99206U;
static const float data_5164 = 19.24f;
static const uint32_t data_5168 = 0x42DBD368U;
static const float data_516C[6] = {-0.3f, 10.1f, 17.5f, 1.2f, 98.8f, 20.0f};
static const char data_5184[16] __attribute__((scn_string_literal)) = "/scene/03/MA00/";
static const char data_5194[7] __attribute__((scn_string_literal)) = "ALLMAP";
static const char data_519B[16] __attribute__((scn_string_literal)) = "/scene/03/MA00/";
static const char data_51AB[7] __attribute__((scn_string_literal)) = "PK_BOX";
static const char data_51B2[16] __attribute__((scn_string_literal)) = "/scene/03/M3FB/";
static const char data_51C2[5] __attribute__((scn_string_literal)) = "M3FB";
static const char data_51C7[6] __attribute__((scn_string_literal)) = "01LFT";
static const char data_51CD[6] __attribute__((scn_string_literal)) = "01LFT";
static const char data_51D3[5] __attribute__((scn_string_literal)) = "M3FB";
static const char data_51D8[7] __attribute__((scn_string_literal)) = "PK_BOX";
static const char data_51DF[7] __attribute__((scn_string_literal)) = "ALLMAP";
static const char data_51E6[13] __attribute__((scn_string_literal)) = "seqdata1.bin";
static const char data_51F3[5] __attribute__((scn_string_literal)) = "M3FB";
static const char data_51F8[13] __attribute__((scn_string_literal)) = "seqdata2.bin";
static const char data_5205[7] __attribute__((scn_string_literal)) = "M3FB";
static const uint32_t data_520C[4] = {0, 0, 0xD1FFFFFFU, 0xD1FFFFFFU};
static const uint32_t data_521C = SHEN_NAME('BOX1');
static const shen_name data_5220[19] = {
  SHEN_NAME('BOX2'), SHEN_NAME('BOX3'), SHEN_NAME('BOX4'), SHEN_NAME('BOX5'), SHEN_NAME('BOX6'), SHEN_NAME('BOX7'), SHEN_NAME('BOX8'),
  SHEN_NAME('BOX9'), SHEN_NAME('BOXA'), SHEN_NAME('BOXB'), SHEN_NAME('BOXC'), SHEN_NAME('BOXD'), SHEN_NAME('BOXE'), SHEN_NAME('BOXF'),
  SHEN_NAME('BOXG'), SHEN_NAME('BOXH'), SHEN_NAME('BOXI'), SHEN_NAME('BOXJ'), SHEN_NAME('BOXK')
};
static const char data_526C[8] __attribute__((scn_string_literal)) = "FREE";

static void script_0030(int32_t arg0, float arg1, float arg2, float arg3) {
  Vector3 local_F4;
  local_F4.x = arg1;
  (&local_F4)->y = arg2;
  (&local_F4)->z = arg3;
  Script_EVT_SetPosition(arg0, POS_XYZ, &local_F4);
  return;
}

static void script_0058(int32_t arg0, float arg1, float arg2, float arg3) {
  Vector3i local_F4;
  local_F4.x = (int32_t)(arg1 / 360.0f * (float)0x10000);
  (&local_F4)->y = (int32_t)(arg2 / 360.0f * (float)0x10000);
  (&local_F4)->z = (int32_t)(arg3 / 360.0f * (float)0x10000);
  Script_EVT_SetRotation(arg0, 0x38000000, &local_F4, 0);
  return;
}

static void script_00AC(int32_t arg0, int32_t arg1, int32_t arg2) {
  Vector3 local_F4;
  for (;;) {
    if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0) & Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg1)) {
      Script_EVT_GetPosition(arg1, 4, &local_F4, ENTITY_XFORM_WORLD_SPACE);
      Script_SetLookPointDetails(arg0, 16, &local_F4, arg2);
      Script_FaceLookAtTargetUtils(arg0, 2, &local_F4, 30);
    }
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static int32_t script_00FC(int32_t arg0) {
  int32_t unused_FC;
  GameDateTime local_F4;
  int8_t local_E8[12];
  int32_t local_E4;
  uint8_t unused_E3;
  int8_t local_E2;
  uint8_t unused_E0[2];
  Script_ReturnTimeDate(&local_F4);
  local_E2 = Script_scenUtils(SCENARIO_CMD_GET_STEP_LUT, arg0);
  local_E8[1] = (local_E2 >> 5) & 7;
  if (local_E8[1] == 0) {
    local_E8[1] = 12;
  }
  local_E8[2] = local_E2 & 0x1F;
  if ((&local_F4)->month == local_E8[1]) {
    local_E4 = (&local_F4)->day - local_E8[2];
  } else {
    if (local_E8[1] == 12) {
      local_E8[1] = 0;
    }
    local_E4 = ((&local_F4)->month - local_E8[1]) * 31 + (&local_F4)->day - local_E8[2];
  }
  return local_E4;
}

static void script_01A3(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  local_FC = script_00FC(arg0);
  if (arg1 > local_FC) {
    (void)0;
  } else {
    (void)-1;
  }
  return;
}

static int32_t script_01C6(int32_t arg0) {
  int32_t unused_FC;
  GameDateTime local_F4;
  int32_t local_F0;
  int32_t local_EC;
  Script_ReturnTimeDate(&local_F4);
  local_F0 = (&local_F4)->hours * 60 + (&local_F4)->minutes;
  local_EC = Script_scenUtils(SCENARIO_CMD_GET_STEP_LUT, arg0) * 6;
  return local_F0 - local_EC;
}

static void script_01F9(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  local_FC = script_01C6(arg0);
  if (arg1 > local_FC) {
    (void)0;
  } else {
    (void)-1;
  }
  return;
}

static void script_021C(int32_t arg0, int32_t arg1) {
  int8_t local_F4[12];
  int32_t local_F0;
  local_F0 = script_01C6(arg0);
  local_F4[4] = local_F0 / 60;
  local_F4[5] = local_F0 % 60;
  if ((local_F4[4] < *(int8_t *)(arg1 + 4)) | ((local_F4[4] == *(int8_t *)(arg1 + 4)) & (local_F4[5] < *(int8_t *)(arg1 + 5)))) {
    (void)0;
  } else {
    (void)-1;
  }
  return;
}

static void script_027B(int32_t arg0) {
  int32_t local_F8[2];
  local_F8[0] = arg0;
  local_F8[1] = 0;
  (void)local_F8;
  return;
}

static void script_0290(void) {
  Script_ambSoundUtils(1, 1, &data_31FC, 135.0f, 280.0f, 1.0f, 0, &data_328C);
  Script_ambSoundUtils(2, 1, &data_3208, 135.0f, 60.0f, 1.0f, 0, &data_3314);
  Script_ambSoundUtils(3, 0, &data_3214, 50.0f, 170.0f, 0.8f, 0, data_339C);
  Script_ambSoundUtils(4, 0, &data_3220, 10.0f, 5.0f, 1.0f, 0, &data_3424);
  Script_ambSoundUtils(5, 0, data_322C, 8.0f, 5.0f, 1.0f, 0, &data_34AC);
  Script_ambSoundUtils(6, 1, &data_3238, 170.0f, 100.0f, 1.0f, 2, data_3534);
  Script_ambSoundUtils(7, 1, &data_3244, 170.0f, 100.0f, 1.0f, 0, &data_35BC);
  Script_ambSoundUtils(8, 1, &data_3250, 40.0f, 70.0f, 0.8f, 0, &data_3644);
  Script_ambSoundUtils(9, 0, &data_325C, 3.0f, 60.0f, 0.8f, 0, data_36CC);
  Script_ambSoundUtils(10, 0, &data_3268, 60.0f, 35.0f, 0.7f, 0, &data_3754);
  Script_ambSoundUtils(13, 0, &data_3274, 8.0f, 5.0f, 1.0f, 0, &data_37DC);
  Script_ambSoundUtils(15, 1, data_3280, 300.0f, 0.0f, 1.0f, 3, &data_3864);
  Script_Ambient1SoundUtils(0, 23976);
  return;
}

static void script_044A(void) {
  Script_ambSoundUtils(1, 1, &data_38EC, 135.0f, 280.0f, 1.0f, 0, &data_394C);
  Script_ambSoundUtils(2, 1, &data_38F8, 135.0f, 60.0f, 1.0f, 0, &data_39D4);
  Script_ambSoundUtils(3, 0, &data_3904, 50.0f, 170.0f, 0.8f, 0, data_3A5C);
  Script_ambSoundUtils(6, 1, &data_3910, 170.0f, 100.0f, 1.0f, 2, data_3AE4);
  Script_ambSoundUtils(7, 1, &data_391C, 170.0f, 100.0f, 1.0f, 0, &data_3B6C);
  Script_ambSoundUtils(8, 1, &data_3928, 40.0f, 70.0f, 0.8f, 0, &data_3BF4);
  Script_ambSoundUtils(10, 0, &data_3934, 60.0f, 35.0f, 0.7f, 0, &data_3C7C);
  Script_ambSoundUtils(15, 1, data_3940, 300.0f, 0.0f, 1.0f, 3, &data_3D04);
  Script_Ambient1SoundUtils(0, 23976);
  return;
}

static void script_0574(void) {
  Script_ambSoundUtils(1, 0, &data_3D8C, 1.0f, 10.0f, 1.0f, 0, &data_3D98);
  Script_Ambient1SoundUtils(0, 23976);
  return;
}

static void script_05A5(void) {
  Script_ambSoundUtils(1, 0, &data_3E20, 1.0f, 10.0f, 1.0f, 0, &data_3E2C);
  Script_Ambient1SoundUtils(0, 23976);
  return;
}

static void script_05D6(void) {
  Script_ambSoundUtils(1, 1, &data_3EB4, 135.0f, 280.0f, 1.0f, 0, &data_3F14);
  Script_ambSoundUtils(2, 1, &data_3EC0, 135.0f, 60.0f, 1.0f, 0, &data_3F9C);
  Script_ambSoundUtils(3, 0, &data_3ECC, 50.0f, 170.0f, 0.8f, 0, data_4024);
  Script_ambSoundUtils(6, 1, &data_3ED8, 170.0f, 100.0f, 1.0f, 2, data_40AC);
  Script_ambSoundUtils(7, 1, &data_3EE4, 170.0f, 100.0f, 1.0f, 0, &data_4134);
  Script_ambSoundUtils(8, 1, &data_3EF0, 40.0f, 70.0f, 0.8f, 0, &data_41BC);
  Script_ambSoundUtils(15, 1, data_3F08, 300.0f, 0.0f, 1.0f, 3, &data_4244);
  Script_Ambient1SoundUtils(0, 23976);
  return;
}

static void script_06DC(void) {
  script_0290();
  return;
}

static void script_06E4(void) {
  Script_ryoControl(PCCTL_DisablePlayerControl);
  return;
}

static void script_06ED(void) {
  Script_ryoControl(RYO_RestoreControl);
  Script_SetGameStateTo(AUTH_None);
  return;
}

static void script_06FC(int32_t arg0) {
  Script_ccowUtils(arg0, 2);
  if (0) {
    Script_charaDisplay(arg0, CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(arg0, CDM_Show);
      goto L000728;
    }
  }
  L000728:;
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

static void script_0789(int32_t arg0) {
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

static void script_07D9(void) {
  Script_filmCheckOrSetTransitionUtil(80, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_0807(int32_t arg0) {
  Script_filmCheckOrSetTransitionUtil(arg0, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_0835(void) {
  Script_filmCheckOrSetTransitionUtil(80, 255, 255, 255, 255, 0, 255, 255, 255);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_0869(int32_t arg0) {
  Script_filmCheckOrSetTransitionUtil(arg0, 255, 255, 255, 255, 0, 255, 255, 255);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_089D(int32_t arg0) {
  Script_filmCheckOrSetTransitionUtil(arg0, 255, 255, 255, 255, 150, 68, 50, 0);
  return;
}

static void script_08BB(int32_t arg0, int32_t arg1, int32_t arg2) {
  int32_t local_FC;
  Script_GetDataFromFightProgram(arg0, arg1);
  local_FC = 0;
  while (local_FC < arg2) {
    local_FC = local_FC + 1;
    Script_SetFightProgramStateBytes(arg0, local_FC, 1);
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_08F5(int32_t arg0, int32_t arg1) {
  while (arg1 > 0) {
    Script_SetFightProgramStateBytes(arg0, arg1, 1);
    arg1 = arg1 - 1;
    SCNF_WaitFrames_0000(1);
  }
  Script_GetDataFromFightProgram(arg0, 0);
  return;
}

static void script_0929(void) {
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

static void script_0A21(int32_t arg0, int32_t arg1) {
  Script_FACE_LCG(arg0, 1, arg1, 0);
  SCNF_WaitFrames_0000(arg1 + 1);
  Script_FACE_LCG(arg0, 2, arg1, 0);
  SCNF_WaitFrames_0000(arg1);
  return;
}

static void script_0A51(int32_t arg0) {
  Script_ccowUtils(arg0, 2);
  Script_ccowSetFlag80(arg0, 1);
  Script_motmToggleFlags(arg0, 2, 0x40);
  Script_SetCPAMFlags(arg0, 0);
  Script_SetCharaFlags(arg0, 4);
  return;
}

static void script_0A80(int32_t arg0) {
  Script_ccowUtils(arg0, 2);
  Script_ccowSetFlag80(arg0, 1);
  Script_motmToggleFlags(arg0, 2, 0x40);
  Script_SetCPAMFlags(arg0, 0);
  Script_SetCharaFlags(arg0, 4);
  Script_motmGetCharaFlags(arg0, 0, 0);
  Script_facemotUtils(arg0, 0, 0);
  Script_facemotUtils(arg0, 1, 0);
  Script_FaceEx(arg0, 1, 0, 1);
  Script_ResetHandMotion(arg0, 1, 1, 0);
  Script_ResetHandMotion(arg0, 1, 0, 0);
  Script_FACE_LCG(arg0, 0, 6, 0);
  Script_FaceLookAtTargetUtils(arg0, 0);
  return;
}

static void script_0B09(void) {
  Script_SetShadowFlags(0, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_SetShadowFlags(CHARA_AKIR, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 1);
  Script_SetCharaRefBoardPos(CHARA_AKIR, &data_4F44);
  Script_charaSetRefboardFlagTo(CHARA_HARY, 1);
  Script_SetCharaRefBoardPos(CHARA_HARY, &data_4F44);
  Script_charaSetRefboardFlagTo(CHARA_JONZ, 1);
  Script_SetCharaRefBoardPos(CHARA_JONZ, &data_4F44);
  Script_charaSetRefboardFlagTo(CHARA_DICK, 1);
  Script_SetCharaRefBoardPos(CHARA_DICK, &data_4F44);
  Script_charaSetRefboardFlagTo(CHARA_RBRT, 1);
  Script_SetCharaRefBoardPos(CHARA_RBRT, &data_4F44);
  Script_charaSetRefboardFlagTo(CHARA_BOB_, 1);
  Script_SetCharaRefBoardPos(CHARA_BOB_, &data_4F44);
  return;
}

static void script_0BC7(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  script_0A80(CHARA_AKIR);
  Script_ResetHandMotion(CHARA_AKIR, 1, 1, 0);
  Script_ResetHandMotion(CHARA_AKIR, 1, 0, 0);
  Script_SetMotnFlag(CHARA_AKIR, -1);
  Script_motmToggleFlags(CHARA_AKIR, 2, 0x40);
  script_0A51(CHARA_HARY);
  script_0A51(CHARA_JONZ);
  script_0A51(CHARA_DICK);
  script_0A51(CHARA_RBRT);
  script_0A51(CHARA_BOB_);
  Script_ccowSetFlag80(SHEN_NAME('BAKO'), 1);
  script_0B09();
  Script_handSetFloatsFor(CHARA_AKIR, 0, data_4E60, 10);
  Script_handSetFloatsFor(CHARA_AKIR, 1, data_4E60, 10);
  task_70 = 0;
  Script_EVT_GetPosition(SHEN_NAME('FORK'), -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  Script_EVT_SetPosition(SHEN_NAME('ZEK5'), 0x78000000, &local_F4);
  Script_mtwkGetSetRotation(SHEN_NAME('FORK'), &local_E8, 0x40000000);
  Script_EVT_SetRotation(SHEN_NAME('ZEK5'), 0x78000000, &local_E8, 0);
  Script_MoveObjectToObject(SHEN_NAME('ZEK5'), 0x4B524F46);
  Script_charaPosRotUtils(SHEN_NAME('FORK'), 3, &data_4F54, &data_4F60, 0);
  Script_SetCharaFlags(SHEN_NAME('FORK'), 8);
  Script_SetCharaFlags(SHEN_NAME('BAKO'), 8);
  return;
}

static int32_t script_0D08(void) {
  task_70 = 0;
  for (;;) {
    {
      int32_t unused_F4[3];
      int32_t local_F0;
      local_F0 = task_70;
      switch (local_F0) {
        case 1:
          Script_FACE(CHARA_AKIR, 0, 0, 8);
          break;
        case 231:
          Script_FACE(CHARA_AKIR, 2, 0, 8);
          break;
        case 726:
          Script_FACE(CHARA_AKIR, 0, 0, 8);
          break;
        case 1371:
          Script_FACE(CHARA_AKIR, 2, 0, 8);
          break;
      }
    }
    {
      int32_t unused_F4[3];
      int32_t local_F0;
      local_F0 = task_70;
      switch (local_F0) {
        case 1:
          Script_CharHandlers(CHARA_AKIR, 2, 8, 16);
          Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
          Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
          break;
        case 681:
          Script_ResetHandMotion(CHARA_AKIR, 1, 1, 0);
          Script_ResetHandMotion(CHARA_AKIR, 1, 0, 0);
          Script_handSetFloatsFor(CHARA_AKIR, 0, data_4E60, 8);
          Script_handSetFloatsFor(CHARA_AKIR, 1, data_4E60, 8);
          break;
        case 726:
          Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
          Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
          break;
        case 1371:
          Script_ResetHandMotion(CHARA_AKIR, 1, 1, 0);
          Script_ResetHandMotion(CHARA_AKIR, 1, 0, 0);
          Script_handSetFloatsFor(CHARA_AKIR, 0, data_4E60, 8);
          Script_handSetFloatsFor(CHARA_AKIR, 1, data_4E60, 8);
          break;
      }
    }
    {
      int32_t unused_F4[3];
      int32_t local_F0;
      local_F0 = task_70;
      switch (local_F0) {
        case 1:
          Script_charaDisplay(SHEN_NAME('SHT1'), CDM_Hide);
          break;
        case 1396:
          Script_executeAicaCommand(0x1604A9, 0, 0);
          break;
      }
    }
    if ((task_70 < 250) | (task_70 > 1370)) {
      Script_charaPosRotUtils(SHEN_NAME('FORK'), 18, 0, &data_4F6C, 1);
      Script_charaPosRotUtils(SHEN_NAME('FORK'), 23, 0, &data_4F6C, 1);
      Script_charaPosRotUtils(SHEN_NAME('FORK'), 8, 0, &data_4F6C, 1);
    }
    task_70 = task_70 + 1;
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_0F3E(void) {
  Vector3 local_F4;
  Vector3 local_E8;
  int32_t unused_DC[3];
  int32_t local_D8;
  task_70 = 0;
  for (;;) {
    {
      int32_t local_D4;
      local_D4 = task_70;
      switch (local_D4) {
        case 1:
          Script_FACE(CHARA_AKIR, 0, 0, 8);
          break;
        case 141:
          Script_FACE(CHARA_AKIR, 2, 0, 8);
          break;
        case 281:
          Script_FACE(CHARA_AKIR, 0, 0, 8);
          break;
        case 471:
          Script_FACE(CHARA_AKIR, 5, 0, 8);
          break;
        case 521:
          Script_FACE(CHARA_AKIR, 0, 0, 8);
          break;
        case 601:
          Script_FACE(CHARA_AKIR, 2, 0, 8);
          break;
      }
    }
    {
      int32_t local_D4;
      local_D4 = task_70;
      switch (local_D4) {
        case 1:
          Script_ResetHandMotion(CHARA_AKIR, 1, 1, 0);
          Script_ResetHandMotion(CHARA_AKIR, 1, 0, 0);
          Script_handSetFloatsFor(CHARA_AKIR, 0, data_4E60, 8);
          Script_handSetFloatsFor(CHARA_AKIR, 1, data_4E60, 8);
          break;
        case 141:
          Script_CharHandlers(CHARA_AKIR, 2, 8, 16);
          Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
          Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
          break;
        case 880:
          Script_ResetHandMotion(CHARA_AKIR, 1, 1, 0);
          Script_ResetHandMotion(CHARA_AKIR, 1, 0, 0);
          Script_handSetFloatsFor(CHARA_AKIR, 0, data_43B0, 16);
          Script_handSetFloatsFor(CHARA_AKIR, 1, data_43B0, 16);
          break;
      }
    }
    {
      int32_t local_D4;
      local_D4 = task_70;
      switch (local_D4) {
        case 1:
          Script_refbSetActive(0);
          Script_SetupLightFromTable(1);
          break;
        case 156:
          Script_SetupLightFromTable(2);
          Script_EVT_GetPosition(SHEN_NAME('SHUT'), -1, &task_74, 0);
          task_74.y = 0.3f;
          local_D8 = Script_scenCreateTask(script_1261, 0);
          break;
        case 250:
          Script_refbSetActive(1);
          Script_SetupLightFromTable(3);
          Script_mapmSetRenderFlag(1, 0);
          Script_mapmSetRenderFlag(4, 0);
          Script_mapmSetRenderFlag(9, 0);
          Script_mapmSetRenderFlag(3, 0);
          Script_mapmSetRenderFlag(10, 0);
          break;
      }
    }
    if (task_70 == 1) {
      Script_charaDisplay(SHEN_NAME('LEFC'), CDM_Show);
      Script_EVT_GetPosition(SHEN_NAME('LEFC'), -1, &local_E8, 0);
    } else if ((task_70 >= 2) & (task_70 <= 154)) {
      Script_EVT_GetPosition(SHEN_NAME('SHUT'), -1, &local_F4, 0);
      (&local_E8)->y = (&local_F4)->y - 3.6351f;
      Script_EVT_SetPosition(SHEN_NAME('LEFC'), POS_XYZ, &local_E8);
    } else {
      switch (task_70) {
        case 155:
          Script_charaDisplay(SHEN_NAME('LEFC'), CDM_Hide);
          break;
        case 1000:
          script_123B();
          break;
      }
    }
    task_70 = task_70 + 1;
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_123B(void) {
  Script_FACE_LCG(CHARA_AKIR, 2, 4, 0);
  return;
}

static void script_124E(void) {
  Script_FACE_LCG(CHARA_AKIR, 0, 0, 0);
  return;
}

static int32_t script_1261(void) {
  Vector3 local_F4;
  Vector3 local_E8;
  local_F4.x = 0.0f;
  (&local_F4)->y = 0.01f;
  (&local_F4)->z = 0.0f;
  local_E8.x = 0.0f;
  (&local_E8)->y = 0.0f;
  (&local_E8)->z = 0.0f;
  task_80 = Script_EffectUtils(0, &task_74, &data_4F78, 100, 0.001f, 0, 3);
  (void)((task_84 = -1) & 1);
  Script_EffectUtils(12, task_80, 9);
  Script_EffectUtils(11, task_80, 255);
  Script_EffectUtils(9, task_80, 138, 128, 113);
  Script_EffectUtils(10, task_80, 0.5f);
  Script_EffectUtils(8, task_80, 2.509804f, 2.54901958f);
  Script_EffectUtils(7, task_80, 0.0125490194f);
  Script_EffectUtils(6, task_80, &local_F4, &local_E8);
  Script_EffectUtils(15, task_80, 55.863998f, 0.70588237f);
  Script_EffectUtils(14, task_80, 1.0f, 0.631372571f);
  Script_EffectUtils(1, task_80);
  Script_EffectUtils(7, task_80, 0.1f);
  SCNF_WaitFrames_0000(8);
  Script_EffectUtils(7, task_80, 0.0f);
  SCNF_WaitFrames_0000(150);
  Script_EffectUtils(3, task_80);
  return Script_srchControl(0);
}

static void script_139A(void) {
  Script_EffectUtils(2, task_80);
  return;
}

static int32_t script_13A7(void) {
  Vector3 local_F4;
  int32_t unused_EC[2];
  int32_t local_E8;
  int32_t local_E4;
  int32_t local_E0;
  int32_t local_DC;
  Script_shenClearFreeState(1);
  Script_SetGameStateTo(AUTH_None);
  Script_convSetCamInactiveFlag(1);
  script_234A();
  script_22EA(0, 9);
  script_24D1(0, 5, 1);
  script_24D1(6, 9, 0);
  local_E4 = Script_SetMaxNPCsNear(16);
  local_E0 = Script_GetFightingStats(0, 5);
  Script_SetActionSelectorHUDElementVisibility(1);
  script_27A8();
  script_2521(1, 5);
  if (Script_GetFightingStats(0x52494B41, 0) == 3) {
    Script_scenToggleCharaFlag(0, 100, 0);
    script_281C();
    script_231A(1, 5);
    script_24D1(6, 9, 1);
    script_162A();
    task_0 = -7;
    *(&task_0 + 1) = 0;
    *(&task_0 + 2) = 0;
    *(&task_0 + 3) = 0;
    *(&task_0 + 4) = 0;
    *(&task_0 + 5) = local_E0;
    *(&task_0 + 6) = 0;
    *(&task_0 + 7) = 0;
    *(&task_0 + 8) = 0;
    *(&task_0 + 9) = 0;
    *(&task_0 + 10) = 0;
    Script_vfUtils(&task_0);
    script_2521(6, 9);
    if (Script_GetFightingStats(0x52494B41, 0) == 3) {
      local_E8 = 1;
    } else {
      local_E8 = 0;
    }
  } else {
    local_E8 = 0;
  }
  Script_EVT_GetPosition(CHARA_AKIR, 0, &local_F4, 0);
  Script_RunAuthorAndSetState(0, 2.0f, 0, 0, 0, &local_F4, 0, 0.0f, 0, 0);
  if (local_E8 == 1) {
    task_0 = -6;
    *(&task_0 + 1) = 0;
    *(&task_0 + 2) = 0;
    *(&task_0 + 3) = 0;
    *(&task_0 + 4) = 0;
    *(&task_0 + 5) = 8;
    *(&task_0 + 6) = 0;
    *(&task_0 + 7) = 0;
    *(&task_0 + 8) = 0;
    *(&task_0 + 9) = 0;
    *(&task_0 + 10) = 0;
    Script_vfUtils(&task_0);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_MAD_ANGELS_BEHIND_WAREHOUSE_17, 1);
    local_DC = 4;
  } else {
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_MAD_ANGELS_BEHIND_WAREHOUSE_17, 0);
    local_DC = 2;
  }
  Script_SetMaxNPCsNear(local_E4);
  Script_shenClearFreeState(0);
  Script_filmCheckOrSetTransitionUtil(60, 0, 0, 0, 0, 255, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  script_2890();
  if (local_E8) {
    script_1A3F();
  }
  task_6C = local_DC;
  Script_SetActionSelectorHUDElementVisibility(0);
  return Script_srchControl(0);
}

static void script_162A(void) {
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_shenClearFreeState(0);
  Script_uiUtils(0, 20, 0);
  task_8C = 0;
  script_167D();
  for (;;) {
    if (task_8C >= 5) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_shenClearFreeState(1);
  Script_SetGameStateTo(AUTH_None);
  Script_convSetCamInactiveFlag(1);
  return;
}

static void script_167D(void) {
  int32_t local_FC;
  task_88 = 0;
  for (;;) {
    if (task_88 > 160) {
      if (Script_IsInEventCam() == 0) {
        break;
      }
    }
    local_FC = task_88;
    switch (local_FC) {
      case 0:
        Script_seqEnterEvtCam(100, SHEN_NAME('BPNT'), SHEN_NAME('BPNT'));
        Script_scenCreateTask(script_174F, 0);
        Script_scenCreateTask(script_1983, 0);
        break;
      case 10:
        Script_scenCreateTask(script_18B9, 0);
        break;
      case 60:
        Script_seqEnterEvtCam(101, SHEN_NAME('BPNT'), SHEN_NAME('BPNT'));
        break;
      case 85:
        Script_scenCreateTask(script_1837, 0);
        break;
      case 110:
        Script_scenCreateTask(script_17D9, 0);
        break;
    }
    task_88 = task_88 + 1;
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static int32_t script_174F(void) {
  Script_MoveObjectToObject(CHARA_AKIR, 0x544E5042);
  script_0030(CHARA_AKIR, -2.2f, 0.0f, -5.6f);
  script_0058(CHARA_AKIR, 0.0f, -90.0f, 0.0f);
  Script_MoveObjectToObject(CHARA_AKIR, -1);
  Script_motmToggleFlags(CHARA_AKIR, 2, 0x80);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_BATTLE_KAMAE_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  task_8C = task_8C + 1;
  return Script_srchControl(0);
}

static int32_t script_17D9(void) {
  Script_motmToggleFlags(CHARA_SERA, 1, 0x80);
  Script_motmSetBlendFrames(CHARA_SERA, MOTM_BLEND_FRAMES_20);
  Script_PlayCharaMotion(CHARA_SERA, 2336, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_SetCharaMotionPlaybackRate(CHARA_SERA, 0.7f);
  task_8C = task_8C + 1;
  return Script_srchControl(0);
}

static int32_t script_1837(void) {
  Script_PlayCharaMotion(CHARA_PAUR, 8200, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  for (;;) {
    if (Script_IsCharaMotionFinished(CHARA_PAUR)) {
      break;
    }
    Script_RotateNodeUtils(CHARA_PAUR, &data_31D0, 0);
    SCNF_WaitFrames_0000(1);
  }
  Script_PlayCharaMotion(CHARA_PAUR, 8199, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  task_8C = task_8C + 1;
  return Script_srchControl(0);
}

static int32_t script_18B9(void) {
  Script_motmToggleFlags(CHARA_SMTH, 1, 0x80);
  Script_motmSetBlendFrames(CHARA_TONY, MOTM_BLEND_FRAMES_20);
  Script_PlayCharaMotion(CHARA_SMTH, 8197, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  for (;;) {
    if (Script_IsCharaMotionFinished(CHARA_SMTH)) {
      break;
    }
    if (task_88 == 55) {
      Script_executeAicaCommand(328873, 0, 0);
    }
    if (task_88 > 80) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_motmToggleFlags(CHARA_SMTH, 1, 0x80);
  Script_motmSetBlendFrames(CHARA_SMTH, MOTM_BLEND_FRAMES_20);
  Script_PlayCharaMotion(CHARA_SMTH, 2213, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  task_8C = task_8C + 1;
  return Script_srchControl(0);
}

static int32_t script_1983(void) {
  Script_motmSetBlendFrames(CHARA_TONY, MOTM_BLEND_FRAMES_20);
  Script_PlayCharaMotion(CHARA_TONY, 8197, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  for (;;) {
    if (Script_IsCharaMotionFinished(CHARA_TONY)) {
      break;
    }
    if (task_88 == 45) {
      Script_executeAicaCommand(263337, 0, 0);
    }
    if (task_88 > 59) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_motmSetBlendFrames(CHARA_TONY, MOTM_BLEND_FRAMES_20);
  Script_PlayCharaMotion(CHARA_TONY, 2209, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_SetCharaMotionPlaybackRate(CHARA_TONY, 0.7f);
  task_8C = task_8C + 1;
  return Script_srchControl(0);
}

__attribute__((scn_string_bindings(0, data_50C4, 1, data_50CA)))
static void script_1A3F(void) {
  float local_FC;
  Script_LoadVoiceFile(1, "E1073");
  Script_FaceEx(CHARA_AKIR, 1, 2, 1);
  Script_FaceLookAtTargetUtils(CHARA_AKIR, 0);
  script_231A(6, 9);
  script_22EA(1, 5);
  script_24D1(0, 0, 1);
  script_24D1(1, 1, 1);
  task_90 = 0;
  Script_ccowUtils(CHARA_AKIR, 2);
  Script_ccowUtils(CHARA_HARY, 2);
  Script_SetCPAMFlags(CHARA_AKIR, 0);
  Script_SetCPAMFlags(CHARA_HARY, 0);
  Script_MoveObjectToObject(SHEN_NAME('DMMY'), 0x544E5042);
  script_0030(SHEN_NAME('DMMY'), -3.6f, 0.0f, 3.8014f);
  script_0058(SHEN_NAME('DMMY'), 0.0f, 0.0f, 0.0f);
  Script_MoveObjectToObject(CHARA_AKIR, 0x594D4D44);
  Script_MoveObjectToObject(CHARA_HARY, 0x594D4D44);
  Script_MoveObjectToObject(CHARA_JONZ, 0x544E5042);
  Script_MoveObjectToObject(CHARA_BOB_, 0x544E5042);
  Script_MoveObjectToObject(CHARA_RBRT, 0x544E5042);
  Script_MoveObjectToObject(CHARA_DICK, 0x544E5042);
  script_0030(CHARA_AKIR, 0.0f, 0.0f, 0.0f);
  script_0058(CHARA_AKIR, 0.0f, 0.0f, 0.0f);
  script_0030(CHARA_HARY, -0.51373f, 0.0f, -1.70858f);
  script_0058(CHARA_HARY, 0.0f, 0.0f, 0.0f);
  script_0030(CHARA_JONZ, -4.07f, 0.0f, 4.5f);
  script_0058(CHARA_JONZ, 0.0f, 60.0f, 0.0f);
  script_0030(CHARA_BOB_, -2.7613f, 0.0f, -2.5463f);
  script_0058(CHARA_BOB_, 0.0f, 200.0f, 0.0f);
  script_0030(CHARA_RBRT, -4.1072f, 0.0f, -0.061f);
  script_0058(CHARA_RBRT, 0.0f, 300.0f, 0.0f);
  script_0030(CHARA_DICK, -2.24f, 0.0f, 5.47f);
  script_0058(CHARA_DICK, 0.0f, 95.0f, 0.0f);
  Script_motmToggleFlags(CHARA_AKIR, 2, 0x40);
  Script_motmToggleFlags(CHARA_HARY, 2, 0x40);
  Script_motmToggleFlags(CHARA_JONZ, 2, 0x40);
  Script_motmToggleFlags(CHARA_BOB_, 2, 0x40);
  Script_motmToggleFlags(CHARA_RBRT, 2, 0x40);
  Script_motmToggleFlags(CHARA_DICK, 2, 0x40);
  Script_PlayCharaMotion(CHARA_AKIR, 8193, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_PlayCharaMotion(CHARA_HARY, 8196, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_PlayCharaMotion(CHARA_JONZ, 8194, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_PlayCharaMotion(CHARA_BOB_, 8195, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_PlayCharaMotion(CHARA_RBRT, 8194, -1.0f, -1.0f, 50.0f, 1.0f, 0);
  Script_PlayCharaMotion(CHARA_DICK, 8195, -1.0f, -1.0f, 50.0f, 1.0f, 0);
  Script_CharHandlers(CHARA_AKIR, 2, 0, 16);
  Script_motmToggleFlags(CHARA_AKIR, 1, 0x80);
  Script_motmToggleFlags(CHARA_HARY, 1, 0x80);
  Script_SetCharaMotionPlaybackRate(CHARA_AKIR, 0.8f);
  Script_SetCharaMotionPlaybackRate(CHARA_HARY, 0.8f);
  SCNF_WaitFrames_0000(1);
  Script_motmToggleFlags(CHARA_AKIR, 1, 0x40);
  Script_motmToggleFlags(CHARA_HARY, 1, 0x40);
  Script_motmToggleFlags(CHARA_JONZ, 1, 0x40);
  Script_motmToggleFlags(CHARA_BOB_, 1, 0x40);
  Script_motmToggleFlags(CHARA_RBRT, 1, 0x40);
  Script_motmToggleFlags(CHARA_DICK, 1, 0x40);
  Script_scenCreateTask(script_3165, 1, 30);
  Script_charaToggleMotEnabled(CHARA_AKIR, 1);
  Script_charaToggleMotEnabled(CHARA_HARY, 1);
  Script_scenCreateTask(script_1F01, 0);
  while (Script_IsCharaMotionFinished(CHARA_HARY) == 0) {
    local_FC = Script_GetCharaMotionRemainingFramesScaled(CHARA_HARY);
    script_211C(local_FC);
    if (!(local_FC <= 820.0f)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_SetCharaMotionPlaybackRate(CHARA_AKIR, 1.0f);
  Script_SetCharaMotionPlaybackRate(CHARA_HARY, 1.0f);
  Script_MoveObjectToObject(CHARA_AKIR, -1);
  Script_MoveObjectToObject(CHARA_HARY, -1);
  Script_MoveObjectToObject(SHEN_NAME('DMMY'), -1);
  Script_SetCPAMFlags(CHARA_AKIR, 1);
  Script_SetCPAMFlags(CHARA_HARY, 1);
  Script_ccowUtils(CHARA_AKIR, 1);
  Script_ccowUtils(CHARA_HARY, 1);
  Script_LoadVoiceFile(0, "E1073");
  Script_SetGameStateTo(AUTH_None);
  return;
}

static int32_t script_1F01(void) {
  int32_t local_FC;
  int32_t local_F8;
  local_FC = 0;
  for (;;) {
    local_F8 = local_FC;
    switch (local_F8) {
      case 0:
        Script_seqEnterEvtCam(103, SHEN_NAME('BPNT'), SHEN_NAME('BPNT'));
        break;
      case 100:
        Script_seqEnterEvtCam(104, SHEN_NAME('BPNT'), SHEN_NAME('BPNT'));
        break;
      case 246:
        Script_seqEnterEvtCam(105, SHEN_NAME('BPNT'), SHEN_NAME('BPNT'));
        break;
      case 288:
        Script_SetCamShake(0.1f, 0.08f, 0.75f);
        break;
      case 290:
        Script_executeAicaCommand(0x1C04A9, 0, 0);
        break;
      case 300:
        Script_executeAicaCommand(0x1B04A9, 0, 0);
        Script_seqEnterEvtCam(106, SHEN_NAME('BPNT'), SHEN_NAME('BPNT'));
        Script_FACE(CHARA_AKIR, 5, 0, 15);
        break;
      case 375:
        Script_seqEnterEvtCam(107, SHEN_NAME('BPNT'), SHEN_NAME('BPNT'));
        Script_FACE(CHARA_AKIR, 2, 0, 1);
        break;
      case 730:
        Script_executeAicaCommand(0x1B04A9, 0, 0);
        Script_seqEnterEvtCam(108, SHEN_NAME('BPNT'), SHEN_NAME('BPNT'));
        Script_FACE(CHARA_AKIR, 5, 0, 15);
        break;
      case 780:
        Script_seqEnterEvtCam(109, SHEN_NAME('BPNT'), SHEN_NAME('BPNT'));
        break;
      case 860:
        Script_executeAicaCommand(132265, 0, 0);
        break;
      case 1000:
        goto L00209B;
        break;
    }
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  L00209B:;
  return Script_srchControl(0);
}

static int32_t script_20A2(void) {
  return Script_ShowUIMessageFromID("E1073B001");
}

static int32_t script_20AE(void) {
  return Script_ShowUIMessageFromID("E1073B002");
}

static int32_t script_20BA(void) {
  return Script_ShowUIMessageFromID("E1073B003");
}

static int32_t script_20C6(void) {
  return Script_ShowUIMessageFromID("E1073A001");
}

static int32_t script_20D2(void) {
  return Script_ShowUIMessageFromID("E1073B004");
}

static int32_t script_20DE(void) {
  return Script_ShowUIMessageFromID("E1073A002");
}

static int32_t script_20EA(void) {
  return Script_ShowUIMessageFromID("E1073B005");
}

static int32_t script_20F6(void) {
  return Script_ShowUIMessageFromID("E1073A003");
}

static int32_t script_2102(void) {
  return Script_ShowUIMessageFromID("E1073B006");
}

static void script_210E(void) {
  task_90 = task_90 + 1;
  return;
}

static void script_211C(float arg0) {
  int32_t local_FC;
  local_FC = task_90;
  switch (local_FC) {
    case 0:
      if (!(arg0 <= 20.0f)) {
        Script_scenCreateTask(script_20A2, 0);
        script_210E();
      }
      break;
    case 1:
      if (!(arg0 <= 50.0f)) {
        Script_scenCreateTask(script_20AE, 0);
        script_210E();
      }
      break;
    case 2:
      if (!(arg0 <= 160.0f)) {
        Script_scenCreateTask(script_20BA, 0);
        script_210E();
      }
      break;
    case 3:
      if (!(arg0 <= 250.0f)) {
        Script_scenCreateTask(script_20C6, 0);
        script_210E();
      }
      break;
    case 4:
      if (!(arg0 <= 310.0f)) {
        Script_scenCreateTask(script_20D2, 0);
        script_210E();
      }
      break;
    case 5:
      if (!(arg0 <= 430.0f)) {
        Script_scenCreateTask(script_20DE, 0);
        script_210E();
      }
      break;
    case 6:
      if (!(arg0 <= 470.0f)) {
        Script_scenCreateTask(script_20EA, 0);
        script_210E();
      }
      break;
    case 7:
      if (!(arg0 <= 590.0f)) {
        Script_scenCreateTask(script_20F6, 0);
        script_210E();
      }
      break;
    case 8:
      if (!(arg0 <= 630.0f)) {
        Script_scenCreateTask(script_2102, 0);
        script_210E();
      }
      break;
    case 9:
      if (!(arg0 <= 760.0f)) {
        script_210E();
      }
      break;
    case 10:
      Script_executeAicaCommand(2720, 1, 30);
      Script_executeAicaCommand(2720, 2, 30);
      Script_executeAicaCommand(2720, 3, 30);
      Script_scenCreateTask(script_3199, 1, 60);
      script_210E();
      break;
  }
  return;
}

static void script_22EA(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  local_FC = arg0;
  while (local_FC <= arg1) {
    Script_charaDisplay(*(const uint32_t *)((const char *)&data_4F84 + local_FC * 32), CDM_Show);
    local_FC = local_FC + 1;
  }
  return;
}

static void script_231A(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  local_FC = arg0;
  while (local_FC <= arg1) {
    Script_charaDisplay(*(const uint32_t *)((const char *)&data_4F84 + local_FC * 32), CDM_Hide);
    local_FC = local_FC + 1;
  }
  return;
}

static void script_234A(void) {
  {
    int32_t local_FC;
    local_FC = 0;
    while (local_FC < 10) {
      int32_t local_F8;
      local_F8 = *(const uint32_t *)((const char *)&data_4F84 + local_FC * 32);
      Script_MoveObjectToObject(local_F8, 0x544E5042);
      script_0030(local_F8, *(float *)((const char *)&data_4F84 + (local_FC * 32 + 8)), *(float *)((const char *)&data_4F84 + (local_FC * 32 + 12)), *(float *)((const char *)&data_4F84 + (local_FC * 32 + 16)));
      script_0058(local_F8, 0.0f, *(float *)((const char *)&data_4F84 + (local_FC * 32 + 20)), 0.0f);
      Script_MoveObjectToObject(local_F8, -1);
      Script_motmToggleFlags(local_F8, 2, 0x40);
      Script_SetMotnFlag(local_F8, -1);
      if (local_FC == 0) {
        Script_FaceEx(local_F8, 0, 0, 1);
        Script_ResetHandMotion(local_F8, 0, 1, 0);
        Script_ResetHandMotion(local_F8, 0, 0, 0);
      } else {
        Script_PlayCharaMotion(local_F8, *(const uint32_t *)((const char *)&data_4F84 + (local_FC * 32 + 28)), -1.0f, -1.0f, -1.0f, 1.0f, 0);
        if ((local_FC == 6) | (local_FC == 8)) {
          Script_motmToggleFlags(local_F8, 1, 0x80);
        }
      }
      Script_SetCPAMFlags(local_F8, 1);
      Script_SetCharaFlags(local_F8, 0);
      Script_scenToggleActiveFor(local_F8, 1);
      local_FC = local_FC + 1;
    }
  }
  SCNF_WaitFrames_0000(1);
  {
    int32_t local_FC;
    local_FC = 0;
    while (local_FC < 10) {
      int32_t local_F8;
      local_F8 = *(const uint32_t *)((const char *)&data_4F84 + local_FC * 32);
      Script_motmSetBlendFrames(local_F8, MOTM_BLEND_FRAMES_5);
      Script_motmToggleFlags(local_F8, 1, 0x40);
      Script_SetMotnFlag(local_F8, 0);
      local_FC = local_FC + 1;
    }
  }
  return;
}

static void script_24D1(int32_t arg0, int32_t arg1, int32_t arg2) {
  int32_t local_FC;
  local_FC = arg0;
  while (local_FC <= arg1) {
    if (arg2 == 1) {
      Script_SetCharaRefBoardPos(*(const uint32_t *)((const char *)&data_4F84 + local_FC * 32), &data_512C);
    }
    Script_charaSetRefboardFlagTo(*(const uint32_t *)((const char *)&data_4F84 + local_FC * 32), arg2);
    local_FC = local_FC + 1;
  }
  return;
}

static void script_2521(int32_t arg0, int32_t arg1) {
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
  script_25E0(arg0, arg1);
  SCN_Enqueue_VFGI(3, 99, 1.0f, 0, -1);
  while (SCN_GetIsFighting() != 2) {
    SCNF_WaitFrames_0000(1);
  }
  Script_ryoControl(PCCTL_DisablePlayerControl);
  return;
}

static void script_25E0(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  local_FC = arg0;
  while (local_FC <= arg1) {
    if ((local_FC == 3) | (local_FC == 6)) {
      task_0 = *(const uint32_t *)((const char *)&data_4F84 + local_FC * 32);
      *(&task_0 + 1) = 0;
      *(&task_0 + 2) = 1;
      *(&task_0 + 3) = 1;
      *(&task_0 + 4) = *(const uint32_t *)((const char *)&data_4F84 + (local_FC * 32 + 4));
      *(&task_0 + 5) = *(const uint32_t *)((const char *)&data_4F84 + (local_FC * 32 + 24));
      *(&task_0 + 6) = 0;
      *(&task_0 + 7) = 0;
      *(&task_0 + 8) = 0;
      *(&task_0 + 9) = 0;
      *(&task_0 + 10) = 0;
      Script_vfUtils(&task_0);
      Script_vfConfigureFormationGroup(*(const uint32_t *)((const char *)&data_4F84 + local_FC * 32), VF_FORMATION_APPLY_FLAGS, 0x200000, 0x800);
    } else {
      task_0 = *(const uint32_t *)((const char *)&data_4F84 + local_FC * 32);
      *(&task_0 + 1) = 0;
      *(&task_0 + 2) = 1;
      *(&task_0 + 3) = 1;
      *(&task_0 + 4) = *(const uint32_t *)((const char *)&data_4F84 + (local_FC * 32 + 4));
      *(&task_0 + 5) = *(const uint32_t *)((const char *)&data_4F84 + (local_FC * 32 + 24));
      *(&task_0 + 6) = 0;
      *(&task_0 + 7) = 0;
      *(&task_0 + 8) = 0;
      *(&task_0 + 9) = 0;
      *(&task_0 + 10) = 0;
      Script_vfUtils(&task_0);
      Script_vfConfigureFormationGroup(*(const uint32_t *)((const char *)&data_4F84 + local_FC * 32), VF_FORMATION_APPLY_FLAGS, 0x200000, 0x800);
      Script_vfConfigureFormationGroup(*(const uint32_t *)((const char *)&data_4F84 + local_FC * 32), VF_FORMATION_APPLY_FLAGS, 0x200000, 2050);
      Script_vfConfigureFormationGroup(*(const uint32_t *)((const char *)&data_4F84 + local_FC * 32), VF_FORMATION_APPLY_FLAGS, 0x400000, 129);
    }
    local_FC = local_FC + 1;
  }
  return;
}

static void script_27A8(void) {
  int32_t local_FC;
  int32_t local_F8;
  local_FC = 6;
  while (local_FC <= 9) {
    local_F8 = *(const uint32_t *)((const char *)&data_4F84 + local_FC * 32);
    Script_SetCPAMFlags(local_F8, 0);
    Script_charaVisUtils(local_F8, 1);
    if (-1) {
      Script_charaDisplay(local_F8, CDM_Hide);
    } else {
      if (0) {
        Script_charaDisplay(local_F8, CDM_Show);
        goto L0027FD;
      }
    }
    L0027FD:;
    Script_scenToggleVisibleFor(local_F8, 0);
    Script_scenToggleActiveFor(local_F8, 0);
    local_FC = local_FC + 1;
  }
  return;
}

static void script_281C(void) {
  int32_t local_FC;
  int32_t local_F8;
  local_FC = 6;
  while (local_FC <= 9) {
    local_F8 = *(const uint32_t *)((const char *)&data_4F84 + local_FC * 32);
    Script_SetCPAMFlags(local_F8, 1);
    Script_charaVisUtils(local_F8, 0);
    if (0) {
      Script_charaDisplay(local_F8, CDM_Hide);
    } else {
      if (-1) {
        Script_charaDisplay(local_F8, CDM_Show);
        goto L002871;
      }
    }
    L002871:;
    Script_scenToggleVisibleFor(local_F8, 1);
    Script_scenToggleActiveFor(local_F8, 1);
    local_FC = local_FC + 1;
  }
  return;
}

static void script_2890(void) {
  int32_t local_FC;
  local_FC = 0;
  while (local_FC < 10) {
    Script_motmToggleFlags(*(const uint32_t *)((const char *)&data_4F84 + local_FC * 32), 2, 0x80);
    local_FC = local_FC + 1;
  }
  return;
}

static void script_28C3(void) {
  for (;;) {
    Script_PrintDebugString(15, 15, "PRESS A BUTTON");
    if (Script_GetInputData(1, 1) & 0x400) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  return;
}

__attribute__((scn_string_bindings(0, data_5184, 1, data_5194, 2, data_519B, 3, data_51AB, 5, data_51C2)))
void script_28F2(void) {
  Script_filmCheckOrSetTransitionUtil(0, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, "/scene/03/MA00/", "ALLMAP") == 0) {
    SCNF_WaitFrames_0000(1);
  }
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2)) {
    SCNF_WaitFrames_0000(1);
  }
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, "/scene/03/MA00/", "PK_BOX") == 0) {
    SCNF_WaitFrames_0000(1);
  }
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2)) {
    SCNF_WaitFrames_0000(1);
  }
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, "/scene/03/M3FB/", "M3FB") == 0) {
    SCNF_WaitFrames_0000(1);
  }
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2)) {
    SCNF_WaitFrames_0000(1);
  }
  script_06DC();
  Script_GetSetWeatherSettings(WEATHER_SETTINGS_SET_DISABLE_WEATHER, 1);
  Script_WeatherUtils_0(WeatherDataUtil_ApplyCurrentWeatherOrEnableDateTimeRead, 0);
  Script_WeatherTaskUtil(InitTaskData);
  SCNF_WaitFrames_0000(1);
  Script_WeatherUtils(WeatherScript_LoadWccpClipSet0FromTaskOffset, &data_514C, 1);
  if (Script_WeatherUtils_0(WeatherDataUtil_QueryCurrentFCASFlag, 2)) {
    Script_mapmSetRenderFlag(1, 0);
    Script_mapmSetRenderFlag(5, 0);
    Script_mapmSetRenderFlag(3, 1);
    Script_mapmSetRenderFlag(7, 1);
  } else {
    if (Script_WeatherUtils_0(WeatherDataUtil_QueryCurrentFCASFlag, 3)) {
      Script_mapmSetRenderFlag(1, 0);
      Script_mapmSetRenderFlag(5, 0);
      Script_mapmSetRenderFlag(3, 1);
      Script_mapmSetRenderFlag(7, 1);
      Script_mapmSetRenderFlag(10, 1);
      goto L002A65;
    }
  }
  L002A65:;
  Script_flmwSetFollowerLoadIdList(SHEN_NAME('FKDM'), 0x31584F42, 0x32584F42, 0x33584F42, 0x34584F42, 0x35584F42, 0x36584F42, 0x37584F42, 0x38584F42, 0x39584F42, 0x41584F42, 0x42584F42, 0x43584F42, 0x44584F42, 0x45584F42, 0x46584F42, 0x47584F42, 0x48584F42, 0x49584F42, 0x4A584F42, 0x4B584F42, -1);
  Script_ForkliftUtils(0x4D444B46, FORK_CMD_RESTORE_FOLLOWER_LOADS);
  SCNF_WaitFrames_0000(1);
  script_307C();
  Script_ryoSleepUtils(2, 0);
  script_0030(SHEN_NAME('BPNT'), 17.688499f, 0.0f, 99.072403f);
  script_0058(SHEN_NAME('BPNT'), 0.0f, 45.0f, 0.0f);
  Script_cdttLoadScrUtils(1, 0);
  Script_scenCreateTask(script_3165, 1, 30);
  Script_scenCreateTask(script_2B46, 0);
  return;
}

__attribute__((scn_string_bindings(0, data_51C7, 1, data_51CD, 2, data_51D3, 3, data_51D8, 4, data_51DF)))
static int32_t script_2B46(void) {
  int32_t unused_FC;
  GameDateTime local_F4;
  int32_t unused_F0;
  int32_t local_EC;
  int32_t local_E8;
  task_6C = 0;
  for (;;) {
    local_E8 = task_6C;
    switch (local_E8) {
      case 0:
        Script_ryoControl(PCCTL_DisablePlayerControl);
        Script_LoadVoiceFile(1, "01LFT");
        script_0BC7();
        script_231A(3, 9);
        script_2CE9();
        Script_ReturnTimeDate(&local_F4);
        script_0BC7();
        script_22EA(3, 5);
        script_2D85();
        Script_scenCreateTask(script_13A7, 0);
        task_6C = -1;
        break;
      case 2:
        Script_shenClearFreeState(1);
        Script_ryoControl(PCCTL_DisablePlayerControl);
        Script_sicnUtils(0, 1, 1);
        Script_uiUtils(0, 20, 0);
        Script_scenToggleCharaFlag(0, 100, 1);
        local_EC = 0;
        for (;;) {
          local_EC = local_EC + 1;
          if (local_EC > 90) {
            break;
          }
          SCNF_WaitFrames_0000(1);
        }
        Script_sicnUtils(2, 1, 0);
        Script_shenClearFreeState(0);
        task_6C = 3;
        break;
      case 3:
        Script_SetNewDateTime(&local_F4);
        script_0BC7();
        script_22EA(1, 5);
        script_231A(6, 9);
        script_2D85();
        Script_scenCreateTask(script_13A7, 0);
        task_6C = -1;
        break;
      case 4:
        Script_flmwFreeFollowerLoadIds(SHEN_NAME('FKDM'));
        Script_LoadVoiceFile(0, "01LFT");
        script_3134(1);
        Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "M3FB");
        Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "PK_BOX");
        Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "ALLMAP");
        Script_shenRequestGameModuleTransition(3, SHEN_NAME('MA00'), 1);
        goto L002CE2;
        break;
    }
    SCNF_WaitFrames_0000(1);
  }
  L002CE2:;
  return Script_srchControl(0);
}

__attribute__((scn_string_bindings(1, data_51F3)))
static void script_2CE9(void) {
  int32_t local_FC;
  script_2EAD();
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
  Script_AuthorUtils(0, 0, "seqdata1.bin", "M3FB");
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
  local_FC = Script_scenCreateTask(script_0D08, 0);
  Script_shenSeqUtils(0);
  while (Script_shenSeqUtils(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_srchControl((void *)local_FC);
  Script_AuthorUtils(AUTH_FREE, 0);
  Script_SetActionSelectorHUDElementVisibility(1);
  Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

__attribute__((scn_string_bindings(1, data_5205)))
static void script_2D85(void) {
  int32_t local_FC;
  int32_t local_F8;
  Script_scenCreateTask(script_3165, 1, 30);
  (void)((task_84 = 0) & 1);
  script_2EE3();
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
  Script_AuthorUtils(0, 1, "seqdata2.bin", "M3FB");
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
  local_FC = Script_scenCreateTask(script_0F3E, 0);
  Script_shenSeqUtils(1);
  while (Script_shenSeqUtils(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  local_F8 = Script_shenSeqUtils(-7);
  if (local_F8) {
    script_0030(SHEN_NAME('SHUT'), 22.986799f, 1.6888f, 104.36f);
    script_0030(SHEN_NAME('FORK'), 19.992001f, 0.0f, 102.584999f);
    script_0030(SHEN_NAME('BAKO'), 19.028f, 0.15f, 101.621002f);
    Script_SetupLightFromTable(3);
    Script_charaDisplay(SHEN_NAME('LEFC'), CDM_Hide);
    Script_mapmSetRenderFlag(1, 0);
    Script_mapmSetRenderFlag(4, 0);
    Script_mapmSetRenderFlag(9, 0);
    Script_refbSetActive(1);
    if (task_84 & 0x40) {
      script_139A();
    }
  }
  script_124E();
  Script_srchControl((void *)local_FC);
  Script_AuthorUtils(AUTH_FREE, 1);
  return;
}

static void script_2EAD(void) {
  Script_SetupLightFromTable(0);
  Script_SetFogFlags(0, data_520C);
  Script_charaDisplay(SHEN_NAME('SHUT'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('DOOR'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('LEFC'), CDM_Hide);
  return;
}

static void script_2EE3(void) {
  Script_WeatherTaskUtil(ToggleFogTask, 0);
  Script_WeatherTaskUtil(ToggleSceneLightingFlags, 0);
  Script_WeatherTaskUtil(AndNotInstructionFlags, 3);
  Script_SetGlobalSceneSlotValue34(1, 1);
  Script_SetGlobalSceneSlotValue34(3, 1);
  Script_SetGlobalSceneSlotValue34(4, 1);
  Script_SetGlobalSceneSlotValue34(9, 1);
  Script_SetGlobalSceneSlotValue34(10, 1);
  Script_mapmSetRenderFlag(0, 0);
  Script_mapmSetRenderFlag(2, 0);
  Script_mapmSetRenderFlag(4, 1);
  Script_mapmSetRenderFlag(5, 0);
  Script_mapmSetRenderFlag(6, 0);
  Script_mapmSetRenderFlag(7, 0);
  Script_mapmSetRenderFlag(8, 0);
  Script_mapmSetRenderFlag(9, 1);
  Script_mapmSetRenderFlag(11, 0);
  Script_mapmSetRenderFlag(12, 0);
  Script_mapmSetRenderFlag(13, 0);
  Script_mapmSetRenderFlag(14, 0);
  Script_mapmSetRenderFlag(15, 0);
  Script_mapmSetRenderFlag(16, 1);
  if (Script_WeatherUtils_0(WeatherDataUtil_QueryCurrentFCASFlag, 2)) {
    Script_mapmSetRenderFlag(1, 0);
    Script_mapmSetRenderFlag(3, 1);
    Script_mapmSetRenderFlag(10, 0);
  } else if (Script_WeatherUtils_0(WeatherDataUtil_QueryCurrentFCASFlag, 3)) {
    Script_mapmSetRenderFlag(1, 0);
    Script_mapmSetRenderFlag(3, 1);
    Script_mapmSetRenderFlag(10, 1);
  } else {
    Script_mapmSetRenderFlag(1, 1);
    Script_mapmSetRenderFlag(3, 0);
    Script_mapmSetRenderFlag(10, 0);
  }
  Script_mapmSetCamFlags(0, 0);
  Script_charaDisplay(SHEN_NAME('SHUT'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('DOOR'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('SHT0'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('SHT1'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('SHT2'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('DOR1'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('DOR2'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('DOR3'), CDM_Hide);
  return;
}

static void script_307C(void) {
  Vector3 local_F4;
  Vector3 local_E8;
  int32_t local_E4;
  int32_t local_E0;
  local_F4.x = 6.72f;
  (&local_F4)->y = 0.0f;
  (&local_F4)->z = 89.34f;
  local_E8.x = 28.45f;
  (&local_E8)->y = 0.0f;
  (&local_E8)->z = 110.0f;
  local_E4 = 0;
  while (local_E4 < 20) {
    local_E0 = *(&data_521C + local_E4);
    if (Script_SetTaskDataValue(local_E0, &local_F4, &local_E8, 0)) {
      Script_charaDisplay(local_E0, CDM_Show);
      Script_scenToggleVisibleFor(local_E0, 0);
    } else {
      if (-1) {
        Script_charaDisplay(local_E0, CDM_Hide);
      } else {
        if (0) {
          Script_charaDisplay(local_E0, CDM_Show);
          goto L00311E;
        }
      }
      L00311E:;
      Script_scenToggleVisibleFor(local_E0, 0);
    }
    local_E4 = local_E4 + 1;
  }
  return;
}

static void script_3134(int8_t arg0) {
  int8_t local_FF;
  local_FF = 6;
  for (;;) {
    Script_LoadSND(0, local_FF, "FREE", 0);
    if (local_FF <= arg0) {
      break;
    }
    local_FF = local_FF - 1;
  }
  return;
}

static int32_t script_3165(int32_t arg0) {
  Script_filmCheckOrSetTransitionUtil(arg0, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  return Script_srchControl(0);
}

static int32_t script_3199(int32_t arg0) {
  Script_filmCheckOrSetTransitionUtil(arg0, 0, 0, 0, 0, 255, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  return Script_srchControl(0);
}
