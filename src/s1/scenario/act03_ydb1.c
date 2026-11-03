#include "act03_ydb1.h"

static void script_0030(int32_t arg0, const char *arg1, const char *arg2);
static void script_0094(const char *arg0);
static void script_00B5(int32_t arg0);
static void script_00CC(int32_t arg0);
static void script_0152(int32_t arg0, int32_t arg1);
static int32_t script_0175(int32_t arg0, int32_t arg1);
static void script_0199(int32_t arg0, int32_t arg1);
static int32_t script_01AB(const char *arg0, int32_t arg1);
static void script_01D2(int32_t arg0, int32_t arg1);
static int32_t script_01E4(int32_t arg0, int32_t arg1, int32_t arg2, const Vector3 *arg3, const Vector3i *arg4, int32_t arg5);
static void script_024F(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static int32_t script_0269(int32_t arg0, int32_t arg1);
static void script_0289(int32_t arg0, int32_t arg1);
static int32_t script_029B(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_02DF(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_02F3(int32_t arg0);
static int32_t script_030F(int32_t arg0, int32_t arg1, int32_t arg2, float arg3, int32_t arg4, int32_t arg5);
static void script_03B3(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static int32_t script_03CD(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4);
static void script_0412(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4);
static int32_t script_042A(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_045E(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_0474(int32_t arg0, int32_t arg1, const Vector3i *arg2, int32_t arg3);
static void script_04BC(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_04D2(int32_t arg0, int32_t arg1, const int32_t *arg2, int32_t arg3, int32_t arg4);
static void script_04F8(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4);
static int32_t script_0510(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4);
static void script_0537(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4);
static int32_t script_054F(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_058C(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_05A2(int32_t arg0, int32_t arg1, float arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_0626(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static int32_t script_0640(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_06A9(int32_t arg0, int32_t arg1, int32_t arg2);
static int32_t script_06BD(int32_t arg0, int32_t arg1, const Vector3 *arg2, int32_t arg3);
static void script_0721(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_0737(int32_t arg0, int32_t arg1, float arg2, float arg3, float arg4, int32_t arg5);
static void script_0792(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static int32_t script_07AC(int32_t arg0, int32_t arg1, const Vector3i *arg2, int32_t arg3);
static void script_0812(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_0828(int32_t arg0, int32_t arg1, float arg2, int32_t arg3);
static void script_0893(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_08A9(int32_t arg0, const Vector3i *arg1, int32_t arg2);
static void script_08F3(int32_t arg0, int32_t arg1, int32_t arg2);
static int32_t script_0907(int32_t arg0, float arg1, int32_t arg2);
static void script_0956(int32_t arg0, int32_t arg1, int32_t arg2);
static int32_t script_096A(int32_t arg0, float arg1, float arg2, float arg3, int32_t arg4);
static void script_09E3(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4);
static int32_t script_09FB(int32_t arg0, float arg1, float arg2, float arg3, int32_t arg4);
static void script_0A74(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4);
static int32_t script_0A8C(int32_t arg0, float arg1, float arg2, float arg3, int32_t arg4);
static void script_0B05(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4);
static int32_t script_0B1D(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_0B62(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_0B78(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_0B9C(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_0BB2(float arg0, float arg1, float arg2, int32_t arg3);
static void script_0BD7(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_0BED(void);
static void script_0C48(int32_t arg0, int32_t arg1);
static void script_0DBA(int32_t arg0, int32_t arg1);
static void script_0E0E(void);
static void script_0E6D(void);
static void script_0F98(void);
static void script_0FBF(void);
static void script_10F7(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_1294(void);
static int32_t script_13B4(void);
static void script_160E(int32_t arg0);
static void script_176A(int32_t arg0, int32_t arg1);
static void script_21CD(int32_t arg0);
static int32_t script_25A2(int32_t arg0);
static void script_2607(int32_t arg0);
static void script_277E(void);
static void script_2788(void);
static void script_2792(void);
static void script_279C(int32_t arg0);
static void script_27A6(int32_t arg0);
static int32_t script_294C(void);
static int32_t script_3A93(void);
static void script_3B38(void);
static void script_4135(int32_t arg0);
static void script_4658(int32_t arg0);
static int32_t script_60EF(void);
static int32_t script_61A2(void);
static int32_t script_6225(void);
static int32_t script_62AB(void);
static void script_62C0(void);
static void script_65D6(void);
static void script_6631(void);
static void script_6894(void);
static void script_6C55(void);
static void script_6F93(void);
static void script_7017(void);
static int32_t script_7358(int32_t arg0);
static void script_7468(void);
static int32_t script_74BB(void);
static void script_7583(void);
static void script_7642(void);
static void script_7ADA(void);
static int32_t script_7E8C(void);
static void script_7EDB(void);
static void script_8753(void);
static int32_t script_8891(void);
static int32_t script_90CD(int32_t arg0);
static void script_91CC(void);
static void script_9355(void);
static void script_9422(void);
static void script_94B2(void);
static void script_9640(int32_t arg0, float arg1);
static void script_965F(int32_t arg0);
static int32_t script_967B(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4);
static void script_96F1(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4);
static void script_9709(void);
static void script_9891(void);
static int32_t script_9BB3(void);
static void script_9D35(void);
static void script_9E2D(void);
static int32_t script_A0EA(void);
static void script_A18D(void);
static int32_t script_A992(void);
static void script_AA12(void);
static int32_t script_AA3A(void);
static int32_t script_AA64(void);
static int32_t script_AA72(int32_t arg0);
static int32_t script_AB96(void);
static void script_AC9D(void);
static int32_t script_ACD2(void);
static void script_AD92(void);
static void script_B019(int32_t arg0);
static void script_B1AD(int32_t arg0);
static void script_B21F(void);
static void script_B358(int32_t arg0);
static void script_B723(int32_t arg0);
static void script_BC3F(void);
static void script_C0FC(void);
static int32_t script_C7F8(void);
static void script_C858(void);
static void script_CCC1(void);
static void script_CE68(void);
void script_CEB6(void);
static void script_D06D(int32_t arg0, int32_t arg1, int32_t arg2);
static int32_t script_D14B(int32_t arg0);
static void script_D789(int32_t arg0, int32_t arg1);
static void script_D810(int32_t arg0, int32_t arg1);
static void script_D87D(int32_t arg0, int32_t arg1);
static void script_D8C1(void);
static void script_D908(void);
static void script_DA3B(void);
static void script_DC39(void);
static int32_t script_DD28(void);
static void script_DDE3(int32_t arg0);
static int32_t script_E106(int32_t arg0);
static void script_E207(void);
static void script_E34E(void);
static void script_E4BE(void);

static int32_t task_0[16][5];
static int32_t task_140[32][2];
static int32_t task_240;
static int32_t task_244;
static int32_t task_248;
static int32_t task_24C;
static int8_t task_250;
static uint8_t task_251;
static uint16_t task_252;
static int32_t task_254;
static int32_t task_258;
static int32_t task_25C;
static int32_t task_260;
static int32_t task_264;
static int32_t task_268;
static int32_t task_26C;
static int32_t task_270;
static int32_t task_274;
static int32_t task_278;
static int32_t task_27C;
static int32_t task_280;
static Vector3 task_284;
static Vector3i task_290;
static int32_t task_29C;
static int32_t task_2A0;
static int32_t task_2A4;
static int32_t task_2A8;
static int32_t task_2AC;
static int32_t task_2B0;
static int32_t task_2B4;
static int32_t task_2B8;
static int32_t task_2BC;
static int32_t task_2C0;
static int32_t task_2C4;
static int32_t task_2C8;
static int32_t task_2CC;
static int32_t task_2D0;
static int32_t task_2D4;
static int32_t task_2D8;
static int32_t task_2DC;
static int32_t task_2E0;
static int32_t task_2E4;
static int32_t task_2E8;
static int32_t task_2EC;
static int32_t task_2F0;
static int32_t task_2F4;
static int32_t task_2F8;
static int32_t task_2FC[2];
static int32_t task_304[5];
static int32_t task_318;
static int32_t task_31C;
static int32_t task_320;
static int32_t task_324;
static int32_t task_328;
static int32_t task_32C;
static int32_t task_330;
static int32_t task_334;
static int32_t task_338[20];
static int32_t task_388;
static int32_t task_38C;
static int32_t task_390;
static int32_t task_394[4];
static int32_t task_3A4;
static int32_t task_3A8;
static int32_t task_3AC;
static int32_t task_3B0;
static int32_t task_3B4;
static int32_t task_3B8;
static int32_t task_3BC;
static int32_t task_3C0;
static int32_t task_3C4;
static int32_t task_3C8;
static Vector3 task_3CC;
static int32_t task_3D8;
static int32_t task_3DC;
static int32_t task_3E0;
static int32_t task_3E4;
static Vector3 task_3E8;
static Vector3i task_3F4;
static int32_t task_400;
static int32_t task_404;
static int32_t task_408;
static int32_t task_40C;
static int32_t task_410;
static int32_t task_414;
static int32_t task_418;
static int32_t task_41C;
static int32_t task_420;
static int32_t task_424;
static int32_t task_428;
static int8_t task_42C;
static uint8_t task_42D;
static uint16_t task_42E;
static int8_t task_430;
static uint8_t task_431;
static uint16_t task_432;
static int32_t task_434;
static int32_t task_438;
static int32_t task_43C;
static int32_t task_440;
static int32_t task_444;
static int32_t task_448;
static int32_t task_44C;
static int32_t task_450;
static int32_t task_454;
static int32_t task_458;
static int32_t task_45C;
static int32_t task_460;
static int32_t task_464;
static int32_t task_468;
static int32_t task_46C;
static int32_t task_470;
static int32_t task_474;
static int32_t task_478;
static int32_t task_47C;
static int32_t task_480;
static int32_t task_484[100];
static int32_t task_614;
static int32_t task_618;
static int32_t task_61C;
static int32_t task_620;
static int32_t task_624;
static int32_t task_628;
static int32_t task_62C;
static int32_t task_630;
static int32_t task_634;
static int32_t task_638;
static int32_t task_63C;
static int32_t task_640;
static int32_t task_644;
static int32_t task_648;
static int32_t task_64C;
static int32_t task_650;
static int32_t task_654;
static int32_t task_658;
static int32_t task_65C;
static int32_t task_660;
static int32_t task_664[4];
static int8_t task_674[4][9];
static const Vector3 data_E860 = {.x = 1.0f, .y = 1.0f, .z = 1.0f};
static const int32_t data_E86C[4] = {16777215, 16777215, 16777215, 16777215};
static const float data_E87C[3] = {20.0f, 800.0f, 1.0f};
static const int32_t data_E888 = -1;
static const Vector3 data_E88C = {.x = 0.0f, .y = 0.0f, .z = 0.0f};
static const Vector3i data_E898 = {.x = 0, .y = 0, .z = 0};
static const uint32_t data_E8A4 = 380;
static const int32_t data_E8A8[6] = {1, 390, 0, 1018, 0, -1};
static const uint32_t data_E8C0 = 510;
static const int32_t data_E8C4[6] = {1, 525, 0, 1020, 0, -1};
static const uint32_t data_E8DC = 150;
static const int32_t data_E8E0[6] = {1, 160, 0, 1019, 0, -1};
static const uint32_t data_E8F8 = 530;
static const int32_t data_E8FC[6] = {1, 970, 0, 1014, 0, -1};
static const uint32_t data_E914 = 911;
static const int32_t data_E918[6] = {1, 155, 0, 1015, 0, -1};
static const Vector3 data_E930 = {.x = 7.88008f, .y = 1.21f, .z = 1.45f};
static uint32_t data_E93C = 131072;
static const shen_name data_E940 = SHEN_NAME('TUB2');
static const uint32_t data_E944[2] = {0x80004000U, 0};
static const float data_E94C[4] = {0.15f, 0.0f, 0.0f, 0.0f};
static const int32_t data_E95C = 131072;
static const shen_name data_E960 = SHEN_NAME('TUB3');
static const uint32_t data_E964[2] = {0x80008000U, 0};
static const float data_E96C[5] = {0.15f, 0.0f, 0.0f, 0.0f, 0.0f};
static const shen_name data_E980 = SHEN_NAME('DENW');
static const uint32_t data_E984 = 0x71C7C000U;
static const float data_E988[5] = {-0.1f, 0.0f, 0.0f, 0.0f, 0.0f};
static const int32_t data_E99C = 131072;
static const shen_name data_E9A0 = SHEN_NAME('YAKU');
static const uint32_t data_E9A4[5] = {0x71C78000U, 0, 0, 0, 0};
static const int32_t data_E9B8[2] = {5, 131072};
static const shen_name data_E9C0 = SHEN_NAME('YAK2');
static const uint32_t data_E9C4[5] = {0x71C70000U, 0, 0, 0, 0};
static const int32_t data_E9D8[2] = {5, 0};
static const shen_name data_E9E0 = CHARA_ONO_;
static const uint32_t data_E9E4[2] = {0x71C74000U, 0};
static const float data_E9EC[4] = {0.3559f, -0.0414f, 0.0f, 0.0f};
static const int32_t data_E9FC = 589824;
static const shen_name data_EA00 = SHEN_NAME('BALL');
static const uint32_t data_EA04[6] = {0x71C70000U, 0, 0, 0, 0, 0};
static const int32_t data_EA1C = 589824;
static const shen_name data_EA20 = SHEN_NAME('BOOK');
static const uint32_t data_EA24[4] = {0x71C78000U, 0, 0, 0};
static const int32_t data_EA34[3] = {21, 0, 131072};
static const shen_name data_EA40 = SHEN_NAME('BNT0');
static const uint32_t data_EA44[7] = {0x71C7C000U, 0, 0, 0, 0, 0, 0};
static const shen_name data_EA60 = SHEN_NAME('BX4B');
static const uint32_t data_EA64[6] = {0x71C70000U, 0, 0, 0, 0, 0};
static const int32_t data_EA7C = 393472;
static const shen_name data_EA80 = SHEN_NAME('UBOX');
static const uint32_t data_EA84[2] = {0x71C70000U, 0};
static const float data_EA8C[5] = {0.2f, 0.3f, 0.0f, 0.0f, 0.0f};
static const shen_name data_EAA0 = SHEN_NAME('ROU4');
static const uint32_t data_EAA4[6] = {0x71C74000U, 0, 0, 0, 0, 0};
static const int32_t data_EABC = 196608;
static const shen_name data_EAC0 = SHEN_NAME('TNS0');
static const uint32_t data_EAC4[4] = {0x71C7C000U, 0, 0, 0};
static const int32_t data_EAD4[3] = {4, 5, 196608};
static const shen_name data_EAE0 = SHEN_NAME('TNS1');
static const uint32_t data_EAE4[4] = {0x71C7C000U, 0, 0, 0};
static const int32_t data_EAF4[3] = {4, 4, 196608};
static const shen_name data_EB00 = SHEN_NAME('TNS2');
static const uint32_t data_EB04[4] = {0x71C7C000U, 0, 0, 0};
static const int32_t data_EB14[3] = {4, 6, 196608};
static const shen_name data_EB20 = SHEN_NAME('TNS3');
static const uint32_t data_EB24[4] = {0x71C7C000U, 0, 0, 0};
static const int32_t data_EB34[3] = {4, 7, 196608};
static const shen_name data_EB40 = SHEN_NAME('TNS4');
static const uint32_t data_EB44[4] = {0x71C7C000U, 0, 0, 0};
static const int32_t data_EB54[3] = {4, 8, 327680};
static const shen_name data_EB60 = SHEN_NAME('TNS5');
static const uint32_t data_EB64[2] = {0x71C7C000U, 0};
static const float data_EB6C[3] = {0.25f, 0.0f, 0.0f};
static const int32_t data_EB78[2] = {3, 196608};
static const shen_name data_EB80 = SHEN_NAME('HIK1');
static const uint32_t data_EB84[5] = {0x71C7C000U, 0, 0, 0, 0};
static const int32_t data_EB98[2] = {1, 196608};
static const shen_name data_EBA0 = SHEN_NAME('HIK2');
static const uint32_t data_EBA4[6] = {0x71C7C000U, 0, 0, 0, 0, 0};
static const int32_t data_EBBC = 196608;
static const shen_name data_EBC0 = SHEN_NAME('HIK3');
static const uint32_t data_EBC4[5] = {0x71C7C000U, 0, 0, 0, 0};
static const int32_t data_EBD8[2] = {2, 0};
static const shen_name data_EBE0 = SHEN_NAME('ROU1');
static const uint32_t data_EBE4[7] = {0x71C7C000U, 0, 0, 0, 0, 0, 0};
static const shen_name data_EC00 = SHEN_NAME('ROU2');
static const uint32_t data_EC04[7] = {0x71C7C000U, 0, 0, 0, 0, 0, 0};
static const shen_name data_EC20 = SHEN_NAME('ROU3');
static const uint32_t data_EC24[7] = {0x71C78000U, 0, 0, 0, 0, 0, 0};
static const shen_name data_EC40 = SHEN_NAME('HAKO');
static const uint32_t data_EC44[2] = {0x71C78000U, 0};
static const float data_EC4C[4] = {0.5f, 0.0f, 0.0f, 0.0f};
static const int32_t data_EC5C = 589824;
static const shen_name data_EC60 = SHEN_NAME('KNIS');
static const uint32_t data_EC64[6] = {0x71C78000U, 0, 0, 0, 0, 0};
static const int32_t data_EC7C = 131072;
static const shen_name data_EC80 = SHEN_NAME('MATI');
static const uint32_t data_EC84[4] = {0x71C78000U, 0, 0, 0};
static const int32_t data_EC94[3] = {55, 0, 0};
static const shen_name data_ECA0 = SHEN_NAME('DAMY');
static const uint32_t data_ECA4[7] = {0x71C70000U, 0, 0, 0, 0, 0, 0};
static const shen_name data_ECC0[3] = {SHEN_NAME('MUKA'), 0, 0};
static const float data_ECCC[5] = {0.3f, 0.0f, 0.0f, 0.0f, 0.0f};
static const shen_name data_ECE0[2] = {SHEN_NAME('MUKA'), 0};
static const float data_ECE8[6] = {100.5f, 0.0f, 0.5f, 0.0f, 0.0f, 0.0f};
static const shen_name data_ED00[2] = {SHEN_NAME('MUKA'), 0};
static const float data_ED08[6] = {-100.5f, 0.0f, 0.5f, 0.0f, 0.0f, 0.0f};
static const shen_name data_ED20[2] = {SHEN_NAME('MUKA'), 0};
static const float data_ED28[6] = {100.5f, 0.0f, -0.5f, 0.0f, 0.0f, 0.0f};
static const shen_name data_ED40[2] = {SHEN_NAME('MUKA'), 0};
static const float data_ED48[6] = {-100.5f, 0.0f, -0.5f, 0.0f, 0.0f, 0.0f};
static const shen_name data_ED60[2] = {SHEN_NAME('MUKA'), 0};
static const float data_ED68[6] = {-100.75f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
static const shen_name data_ED80[2] = {SHEN_NAME('MUKA'), 0};
static const float data_ED88[6] = {100.0f, 0.0f, 0.75f, 0.0f, 0.0f, 0.0f};
static const shen_name data_EDA0 = SHEN_NAME('FUDA');
static const uint32_t data_EDA4[2] = {0x71C74000U, 0};
static const float data_EDAC[4] = {0.05f, 0.0f, 0.0f, 0.0f};
static const int32_t data_EDBC[2] = {-65536, 4};
static const uint32_t data_EDC4 = 0x71C72AAAU;
static const float data_EDC8[5] = {0.9224f, 1.2864f, 2.2065f, 0.0f, 0.0f};
static const int32_t data_EDDC[3] = {-65536, 7, 0};
static const float data_EDE8[5] = {3.9994f, 1.916f, 4.0053f, 0.0f, 0.0f};
static const int32_t data_EDFC[2] = {-65536, 8};
static const uint32_t data_EE04 = 0x71C74000U;
static const float data_EE08[5] = {0.0342f, 1.3f, 2.9097f, 0.0f, 0.0f};
static const int32_t data_EE1C[2] = {-65536, 12};
static const uint32_t data_EE24 = 0x71C78000U;
static const float data_EE28[5] = {3.4967f, 1.104f, 7.2008f, 0.0f, 0.0f};
static const int32_t data_EE3C[2] = {-65536, 13};
static const uint32_t data_EE44 = 0x71C7CE38U;
static const float data_EE48[6] = {7.818f, 1.2f, 1.4687f, 0.0f, 0.0f, 0.0f};
static const shen_name data_EE60 = SHEN_NAME('TIRA');
static const uint32_t data_EE64[6] = {0x71C78000U, 0, 0, 0, 0, 0};
static const int32_t data_EE7C = 65535;
static char data_EE80[] = "TNS4SZAS";
static const uint8_t pad_EE89[11] = {0};
static const char str_EE94[] __attribute__((scn_string_literal)) = "TNS3KKAZ";
static const uint8_t pad_EE9D[11] = {0};
static const char str_EEA8[] __attribute__((scn_string_literal)) = "HIK3IROU6";
static const uint8_t pad_EEB2[10] = {0};
static const char str_EEBC[] __attribute__((scn_string_literal)) = "UBOXSAKA";
static const uint8_t pad_EEC5[11] = {0};
static const char str_EED0[] __attribute__((scn_string_literal)) = "HIK1KUSK";
static const uint8_t pad_EED9[11] = {0};
static const int32_t data_EEE4 = -1;
static uint32_t data_EEE8 = 0x1B05A9;
static const int32_t data_EEEC[3] = {0, 1836457, 0};
static const shen_name data_EEF8[8] = {
  SHEN_NAME('TNS0'), SHEN_NAME('TNS1'), SHEN_NAME('TNS2'), SHEN_NAME('TNS3'), SHEN_NAME('TNS4'), SHEN_NAME('HIK1'), SHEN_NAME('HIK2'),
  SHEN_NAME('HIK3')
};
static const int32_t data_EF18[5] = {-1, 1508777, 0, 1508777, 0};
static const shen_name data_EF2C[2] = {SHEN_NAME('BX4B'), SHEN_NAME('BX7B')};
static const int32_t data_EF34[5] = {-1, 3474857, 0, 0, 3474857};
static const shen_name data_EF48 = SHEN_NAME('KKAZ');
static const int32_t data_EF4C[5] = {-1, 3409321, 0, 0, 0};
static const shen_name data_EF60 = SHEN_NAME('KNIS');
static const int32_t data_EF64[5] = {-1, 2688425, 0, 0, 2753961};
static const shen_name data_EF78[4] = {SHEN_NAME('YAKU'), SHEN_NAME('YAK2'), SHEN_NAME('TUB2'), SHEN_NAME('TUB3')};
static const int32_t data_EF88[5] = {-1, 3212713, 0, 0, 2819497};
static const shen_name data_EF9C[2] = {SHEN_NAME('KUSK'), SHEN_NAME('BALL')};
static const int32_t data_EFA4[5] = {-1, 2360745, 0, 0, 2885033};
static const shen_name data_EFB8 = SHEN_NAME('UBOX');
static const int32_t data_EFBC[5] = {-1, 2229673, 0, 0, 2229673};
static const shen_name data_EFD0 = SHEN_NAME('SAKA');
static const int32_t data_EFD4[5] = {-1, 3081641, 0, 3016105, 3147177};
static const shen_name data_EFE8 = SHEN_NAME('TNS5');
static const int32_t data_EFEC[5] = {-1, 2295209, 0, 0, 2229673};
static const shen_name data_F000 = SHEN_NAME('BNT0');
static const int32_t data_F004[5] = {-1, 3343785, 0, 0, 2819497};
static const shen_name data_F018 = SHEN_NAME('BOOK');
static const int32_t data_F01C[2] = {-1, -1};
static const uint8_t data_F024[20] = {0};
typedef struct Row_F038 { int32_t w0; int32_t w1; int32_t w2; int32_t w3; float w4; float w5; int32_t w6; } Row_F038;
static const Row_F038 data_F038[15] = {
  {16384, 0, -1, 0, 0.0f, 0.0f, 0},
  {0, 0, 1, 0, 0.0f, 0.0f, 0},
  {16384, 0, -1, 0, 0.0f, 0.0f, 0},
  {0, 0, 2, 0, -0.15f, -0.05f, 0},
  {0, 0, -1, 0, 0.0f, 0.0f, 0},
  {0, 0, 3, 0, 0.0f, 0.05f, 0},
  {0, 0, -1, 0, 0.0f, 0.0f, 0},
  {0, 0, 4, 0, 0.0f, 0.05f, 0},
  {0, 0, -1, 0, 0.0f, 0.0f, 0},
  {0, 0, 5, 0, 0.0f, 0.05f, 0},
  {0, 0, -1, 0, 0.0f, 0.0f, 0},
  {0, 0, 6, 0, 0.0f, 0.05f, 0},
  {0, 0, -1, 0, 0.0f, 0.0f, 0},
  {0, 0, 7, 0, 0.0f, 0.05f, 0},
  {0, 0, -1, 0, 0.0f, 0.0f, 0}
};
static const int32_t data_F1DC[2] = {0, 0};
static char data_F1E4[] = "$KYOM400G.MT5";
static const char str_F1F2[] __attribute__((scn_string_literal)) = "$KYOM401G.MT5";
static const char str_F200[] __attribute__((scn_string_literal)) = "$TIKM4T0G.MT5";
static const char str_F20E[] __attribute__((scn_string_literal)) = "$TIKM4T1G.MT5";
static const char str_F21C[] __attribute__((scn_string_literal)) = "$TIKM4T2G.MT5";
static const char str_F22A[] __attribute__((scn_string_literal)) = "$TIKM4T3G.MT5";
static const char str_F238[] __attribute__((scn_string_literal)) = "$TIKM4T4G.MT5";
static const char str_F246[] __attribute__((scn_string_literal)) = "$TIKM4T5G.MT5";
static const uint8_t pad_F254[4] = {0};
static const Vector4 data_F258 = {.x = 0.78f, .y = 0.82f, .z = 0.59f, .w = 0.46f};
static const float data_F268[19] = {
  0.89f, 0.89f, 0.13f, 0.39f, 0.28f, 0.14f, 1.1f, 1.84f, 0.93f, 0.58f, 4.1813f, 0.996f, 0.7922f, 1000.0f, 1000.0f, 1000.0f, 1000.0f, 1000.0f, 1000.0f
};
static const char data_F2B4[28] __attribute__((scn_string_literal)) = "Map Start!!!!!!!!!!!!!!!!!\012";
static const char data_F2D0[21] __attribute__((scn_string_literal)) = "このアイテムもってる";
static const char data_F2E5[19] __attribute__((scn_string_literal)) = "tansu naka = %d\012";
static const Vector3 data_F2F8 = {.x = 0.116f, .y = -0.005f, .z = -0.018f};
static const Vector3i data_F304 = {.x = 1455, .y = 14196, .z = 0};
static const Vector3 data_F310 = {.x = 0.7041f, .y = 0.0f, .z = 5.6678f};
static const Vector3i data_F31C = {.x = 0, .y = 16384, .z = 0};
static const Vector3 data_F328 = {.x = 0.029f, .y = -0.226f, .z = -0.015f};
static const Vector3i data_F334 = {.x = 65171, .y = 32395, .z = 63170};
static const Vector3 data_F340 = {.x = 1000.0f, .y = 1000.0f, .z = 1000.0f};
static const char data_F34C[10] __attribute__((scn_string_literal)) = "E1012A272";
static const char data_F356[10] __attribute__((scn_string_literal)) = "E1012A014";
static const char data_F360[10] __attribute__((scn_string_literal)) = "E1012A271";
static const char data_F36A[10] __attribute__((scn_string_literal)) = "E1012A288";
static const char data_F374[10] __attribute__((scn_string_literal)) = "E1012A289";
static const char data_F37E[10] __attribute__((scn_string_literal)) = "E1012A112";
static const char data_F388[10] __attribute__((scn_string_literal)) = "E1012A198";
static const char data_F392[10] __attribute__((scn_string_literal)) = "E1012A135";
static const char data_F39C[10] __attribute__((scn_string_literal)) = "E1012A066";
static const char data_F3A6[10] __attribute__((scn_string_literal)) = "E1012A190";
static const char data_F3B0[10] __attribute__((scn_string_literal)) = "E1012A192";
static const char data_F3BA[21] __attribute__((scn_string_literal)) = "FUKUSAN  frame = %d\012";
static const char data_F3CF[21] __attribute__((scn_string_literal)) = "KAITYUU nokori = %d\012";
static const char data_F3E4[10] __attribute__((scn_string_literal)) = "E1012A101";
static const char data_F3EE[10] __attribute__((scn_string_literal)) = "E1012A103";
static const char data_F3F8[10] __attribute__((scn_string_literal)) = "E1012A167";
static const char data_F402[10] __attribute__((scn_string_literal)) = "E1012A168";
static const char data_F40C[10] __attribute__((scn_string_literal)) = "E1012A169";
static const char data_F416[10] __attribute__((scn_string_literal)) = "E1012A101";
static const char data_F420[10] __attribute__((scn_string_literal)) = "E1012A103";
static const char data_F42A[10] __attribute__((scn_string_literal)) = "E1012A092";
static const char data_F434[10] __attribute__((scn_string_literal)) = "E1012A093";
static const char data_F43E[10] __attribute__((scn_string_literal)) = "E1012A094";
static const char data_F448[10] __attribute__((scn_string_literal)) = "E1012A095";
static const char data_F452[10] __attribute__((scn_string_literal)) = "E1012A096";
static const char data_F45C[10] __attribute__((scn_string_literal)) = "E1012A097";
static const char data_F466[10] __attribute__((scn_string_literal)) = "E1012A098";
static const char data_F470[10] __attribute__((scn_string_literal)) = "E1012A099";
static const char data_F47A[10] __attribute__((scn_string_literal)) = "E1012A100";
static const char data_F484[10] __attribute__((scn_string_literal)) = "E1012A103";
static const char data_F48E[10] __attribute__((scn_string_literal)) = "E1012A104";
static const char data_F498[10] __attribute__((scn_string_literal)) = "E1012A105";
static const char data_F4A2[10] __attribute__((scn_string_literal)) = "E1012A106";
static const char data_F4AC[10] __attribute__((scn_string_literal)) = "E1012A107";
static const char data_F4B6[10] __attribute__((scn_string_literal)) = "E1012A085";
static const char data_F4C0[10] __attribute__((scn_string_literal)) = "E1012A084";
static const char data_F4CA[10] __attribute__((scn_string_literal)) = "E1012A086";
static const Vector3 data_F4D4 = {.x = 6.873f, .y = 0.0f, .z = 6.6538f};
static const Vector3 data_F4E0 = {.x = 6.921f, .y = 0.0f, .z = 6.5531f};
typedef struct Row_F4EC { float w0; int32_t w1; float w2; } Row_F4EC;
static const Row_F4EC data_F4EC[2] = {
  {1000.0f, 0, 0.0f},
  {3.0289f, 0, 6.1558f}
};
static const char data_F504[10] __attribute__((scn_string_literal)) = "E1012A187";
static const char data_F50E[10] __attribute__((scn_string_literal)) = "E1012A188";
static const char data_F518[10] __attribute__((scn_string_literal)) = "E1012A189";
static const char data_F522[10] __attribute__((scn_string_literal)) = "E1012A019";
static const char data_F52C[10] __attribute__((scn_string_literal)) = "E1012A057";
static const char data_F536[10] __attribute__((scn_string_literal)) = "E1012A058";
static const char data_F540[10] __attribute__((scn_string_literal)) = "E1012A236";
static const char data_F54A[10] __attribute__((scn_string_literal)) = "E1012A256";
static const char data_F554[10] __attribute__((scn_string_literal)) = "E1012A257";
static const char data_F55E[10] __attribute__((scn_string_literal)) = "E1012A061";
static const char data_F568[10] __attribute__((scn_string_literal)) = "E1012A062";
static const char data_F572[10] __attribute__((scn_string_literal)) = "E1012A110";
static const char data_F57C[10] __attribute__((scn_string_literal)) = "E1012A187";
static const char data_F586[10] __attribute__((scn_string_literal)) = "E1012A188";
static const char data_F590[10] __attribute__((scn_string_literal)) = "E1012A189";
static const char data_F59A[10] __attribute__((scn_string_literal)) = "E1012A040";
static const char data_F5A4[10] __attribute__((scn_string_literal)) = "E1012A182";
static const char data_F5AE[10] __attribute__((scn_string_literal)) = "E1012A183";
static const char data_F5B8[10] __attribute__((scn_string_literal)) = "E1012A184";
static const char data_F5C2[10] __attribute__((scn_string_literal)) = "E1012A185";
static const char data_F5CC[10] __attribute__((scn_string_literal)) = "E1012A187";
static const char data_F5D6[10] __attribute__((scn_string_literal)) = "E1012A188";
static const char data_F5E0[10] __attribute__((scn_string_literal)) = "E1012A189";
static const char data_F5EA[10] __attribute__((scn_string_literal)) = "E1012A194";
static const char data_F5F4[10] __attribute__((scn_string_literal)) = "E1012A198";
static const char data_F5FE[10] __attribute__((scn_string_literal)) = "E1012A187";
static const char data_F608[10] __attribute__((scn_string_literal)) = "E1012A188";
static const char data_F612[10] __attribute__((scn_string_literal)) = "E1012A189";
static const char data_F61C[10] __attribute__((scn_string_literal)) = "E1012A195";
static const char data_F626[10] __attribute__((scn_string_literal)) = "E1012A187";
static const char data_F630[10] __attribute__((scn_string_literal)) = "E1012A188";
static const char data_F63A[10] __attribute__((scn_string_literal)) = "E1012A189";
static const char data_F644[10] __attribute__((scn_string_literal)) = "E1012A197";
static const char data_F64E[10] __attribute__((scn_string_literal)) = "E1012A193";
static const char data_F658[10] __attribute__((scn_string_literal)) = "E1012A191";
static const char data_F662[10] __attribute__((scn_string_literal)) = "E1012A194";
static const char data_F66C[10] __attribute__((scn_string_literal)) = "E1012A198";
static const char data_F676[10] __attribute__((scn_string_literal)) = "E1012A187";
static const char data_F680[10] __attribute__((scn_string_literal)) = "E1012A188";
static const char data_F68A[10] __attribute__((scn_string_literal)) = "E1012A189";
static const char data_F694[10] __attribute__((scn_string_literal)) = "E1012A195";
static const char data_F69E[10] __attribute__((scn_string_literal)) = "E1012A187";
static const char data_F6A8[10] __attribute__((scn_string_literal)) = "E1012A188";
static const char data_F6B2[10] __attribute__((scn_string_literal)) = "E1012A189";
static const char data_F6BC[10] __attribute__((scn_string_literal)) = "E1012A197";
static const char data_F6C6[10] __attribute__((scn_string_literal)) = "E1012A193";
static const char data_F6D0[10] __attribute__((scn_string_literal)) = "E1012A191";
static const char data_F6DA[10] __attribute__((scn_string_literal)) = "E1012A194";
static const char data_F6E4[10] __attribute__((scn_string_literal)) = "E1012A198";
static const char data_F6EE[10] __attribute__((scn_string_literal)) = "E1012A187";
static const char data_F6F8[10] __attribute__((scn_string_literal)) = "E1012A188";
static const char data_F702[10] __attribute__((scn_string_literal)) = "E1012A189";
static const char data_F70C[10] __attribute__((scn_string_literal)) = "E1012A195";
static const char data_F716[10] __attribute__((scn_string_literal)) = "E1012A187";
static const char data_F720[10] __attribute__((scn_string_literal)) = "E1012A188";
static const char data_F72A[10] __attribute__((scn_string_literal)) = "E1012A189";
static const char data_F734[10] __attribute__((scn_string_literal)) = "E1012A197";
static const char data_F73E[10] __attribute__((scn_string_literal)) = "E1012A193";
static const char data_F748[10] __attribute__((scn_string_literal)) = "E1012A191";
static const char data_F752[10] __attribute__((scn_string_literal)) = "E1012A194";
static const char data_F75C[10] __attribute__((scn_string_literal)) = "E1012A198";
static const char data_F766[10] __attribute__((scn_string_literal)) = "E1012A187";
static const char data_F770[10] __attribute__((scn_string_literal)) = "E1012A188";
static const char data_F77A[10] __attribute__((scn_string_literal)) = "E1012A189";
static const char data_F784[10] __attribute__((scn_string_literal)) = "E1012A195";
static const char data_F78E[10] __attribute__((scn_string_literal)) = "E1012A187";
static const char data_F798[10] __attribute__((scn_string_literal)) = "E1012A188";
static const char data_F7A2[10] __attribute__((scn_string_literal)) = "E1012A189";
static const char data_F7AC[10] __attribute__((scn_string_literal)) = "E1012A197";
static const char data_F7B6[10] __attribute__((scn_string_literal)) = "E1012A193";
static const char data_F7C0[10] __attribute__((scn_string_literal)) = "E1012A191";
static const char data_F7CA[10] __attribute__((scn_string_literal)) = "E1012A187";
static const char data_F7D4[10] __attribute__((scn_string_literal)) = "E1012A188";
static const char data_F7DE[10] __attribute__((scn_string_literal)) = "E1012A189";
static const char data_F7E8[10] __attribute__((scn_string_literal)) = "E1012A187";
static const char data_F7F2[10] __attribute__((scn_string_literal)) = "E1012A188";
static const char data_F7FC[10] __attribute__((scn_string_literal)) = "E1012A189";
static const char data_F806[10] __attribute__((scn_string_literal)) = "E1012A138";
static const char data_F810[10] __attribute__((scn_string_literal)) = "E1012A139";
static const char data_F81A[10] __attribute__((scn_string_literal)) = "E1012A140";
static const char data_F824[10] __attribute__((scn_string_literal)) = "E1012A131";
static const char data_F82E[10] __attribute__((scn_string_literal)) = "E1012A137";
static const char data_F838[10] __attribute__((scn_string_literal)) = "E1012A141";
static const char data_F842[10] __attribute__((scn_string_literal)) = "E1012A142";
static const char data_F84C[10] __attribute__((scn_string_literal)) = "E1012A268";
static const char data_F856[10] __attribute__((scn_string_literal)) = "E1012A295";
static const char data_F860[10] __attribute__((scn_string_literal)) = "E1012A144";
static const char data_F86A[10] __attribute__((scn_string_literal)) = "E1012A144";
static const char data_F874[10] __attribute__((scn_string_literal)) = "E1012A079";
static const char data_F87E[10] __attribute__((scn_string_literal)) = "E1012A080";
static const char data_F888[10] __attribute__((scn_string_literal)) = "E1012A180";
static const char data_F892[10] __attribute__((scn_string_literal)) = "E1012A298";
static const char data_F89C[10] __attribute__((scn_string_literal)) = "E1012A290";
static const char data_F8A6[10] __attribute__((scn_string_literal)) = "E1012A293";
static const char data_F8B0[10] __attribute__((scn_string_literal)) = "E1012A294";
static const char data_F8BA[10] __attribute__((scn_string_literal)) = "E1012A295";
static const char data_F8C4[10] __attribute__((scn_string_literal)) = "E1012A254";
static const char data_F8CE[10] __attribute__((scn_string_literal)) = "E1012A153";
static const char data_F8D8[10] __attribute__((scn_string_literal)) = "E1012A299";
static const char data_F8E2[10] __attribute__((scn_string_literal)) = "E1012A300";
static const char data_F8EC[10] __attribute__((scn_string_literal)) = "E1012A274";
static const char data_F8F6[10] __attribute__((scn_string_literal)) = "E1012A275";
static const char data_F900[10] __attribute__((scn_string_literal)) = "E1012A158";
static const char data_F90A[10] __attribute__((scn_string_literal)) = "E1012A159";
static const char data_F914[10] __attribute__((scn_string_literal)) = "E1012A296";
static const char data_F91E[10] __attribute__((scn_string_literal)) = "E1012A297";
static const char data_F928[10] __attribute__((scn_string_literal)) = "E1012A164";
static const char data_F932[10] __attribute__((scn_string_literal)) = "E1012A135";
static const char data_F93C[10] __attribute__((scn_string_literal)) = "E1012A150";
static const char data_F946[10] __attribute__((scn_string_literal)) = "E1012A259";
static const char data_F950[12] __attribute__((scn_string_literal)) = "E1012A260";
static const Vector3 data_F95C = {.x = 2.7134f, .y = 0.0f, .z = 6.8502f};
static const Vector3 data_F968 = {.x = 7.7508f, .y = 0.0f, .z = 0.6502f};
static const char data_F974[32] = "HUKU2 END\012\000\000\001\000\005\000\261\277\214\276\000\000\200@\000\000\200?\000\000\003";
static const Vector3 data_F994 = {.x = 1000.0f, .y = 0.0f, .z = 0.0f};
typedef struct Row_F9A0 { float w0; int32_t w1; float w2; } Row_F9A0;
static const Row_F9A0 data_F9A0[2] = {
  {3.9047f, 0, 3.4063f},
  {-6.3f, 0, 4.0f}
};
static const char data_F9B8[10] __attribute__((scn_string_literal)) = "E1012B065";
static const char data_F9C2[10] __attribute__((scn_string_literal)) = "E1012B266";
static const char data_F9CC[10] __attribute__((scn_string_literal)) = "E1012A230";
static const char data_F9D6[10] __attribute__((scn_string_literal)) = "E1012B059";
static const char data_F9E0[10] __attribute__((scn_string_literal)) = "E1012B060";
static const char data_F9EA[10] __attribute__((scn_string_literal)) = "E1012A250";
static const char data_F9F4[10] __attribute__((scn_string_literal)) = "E1012A251";
static const char data_F9FE[90] = "E1012B061\000\232\231\311\300\037\205k\276ff&@^K\330?\000\000\000\000\016\276\230@\001\000\005\000\232\231\311\300\000\000\200@\000\000\200?\002\000\005\000\261\277\214\276\000\000\200@\000\000\200?\000\000\003\000\002\000\005\000-C\034@\000\000\200@\000\000\200?\000\000\003";
static const char data_FA58[10] __attribute__((scn_string_literal)) = "E1012B038";
static const char data_FA62[10] __attribute__((scn_string_literal)) = "E1012A242";
static const char data_FA6C[10] __attribute__((scn_string_literal)) = "E1012B002";
static const char data_FA76[10] __attribute__((scn_string_literal)) = "E1012B003";
static const char data_FA80[10] __attribute__((scn_string_literal)) = "E1012A212";
static const char data_FA8A[10] __attribute__((scn_string_literal)) = "E1012B004";
static const char data_FA94[24] = "E1012A213\000\000\000\232\231\311\300\037\205k\276ff&@";
static const Vector3 data_FAAC = {.x = 1.6898f, .y = 0.0f, .z = 4.7732f};
static const int32_t data_FAB8 = 327681;
static const float data_FABC[3] = {-6.3f, 4.0f, 1.0f};
static const int32_t data_FAC8 = 327682;
static const float data_FACC[3] = {-0.2749f, 4.0f, 1.0f};
static const int32_t data_FAD8[2] = {196608, 327682};
static const float data_FAE0[3] = {2.4416f, 4.0f, 1.0f};
static const int32_t data_FAEC[11] = {196608, 0, 5825, 63715, 0, 60074, 0, 56433, 58254, 0, 52792};
static const char data_FB18[10] __attribute__((scn_string_literal)) = "E1012B067";
static const char data_FB22[10] __attribute__((scn_string_literal)) = "E1012A242";
static const char data_FB2C[10] __attribute__((scn_string_literal)) = "E1012B068";
static const char data_FB36[10] __attribute__((scn_string_literal)) = "E1012A243";
static const uint32_t data_FB40 = 327681;
static const float data_FB44[3] = {4.1198f, 0.2785f, 1.0f};
static const int32_t data_FB50 = 196608;
static const Vector3 data_FB54 = {.x = 4.6898f, .y = 0.0f, .z = 3.4732f};
static const Vector3 data_FB60 = {.x = 4.6698f, .y = 0.0f, .z = 1.4932f};
static const uint32_t data_FB6C = 0x4091EB85;
static const float data_FB70[2] = {0.8f, 1.13f};
static const char data_FB78[10] __attribute__((scn_string_literal)) = "E1012B051";
static const char data_FB82[10] __attribute__((scn_string_literal)) = "E1012B052";
static const char data_FB8C[10] __attribute__((scn_string_literal)) = "E1012A245";
static const char data_FB96[10] __attribute__((scn_string_literal)) = "E1012A246";
static const Vector3 data_FBA0 = {.x = 0.11f, .y = 0.06f, .z = 0.013f};
static const Vector3i data_FBAC = {.x = 18022, .y = 46785, .z = 46057};
static const char data_FBB8[10] __attribute__((scn_string_literal)) = "E1012B051";
static const char data_FBC2[30] = "E1012A244\000\001\000\005\0003\304\247@=\012\327?\000\000\200?\000\000\003";
static const Vector3 data_FBE0 = {.x = 5.8918f, .y = 0.0f, .z = 2.7596f};
static const Vector3i data_FBEC = {.x = 0, .y = 1820, .z = 0};
static const float data_FBF8[3] = {5.8351f, 0.0f, 2.3668f};
static const Vector3 data_FC04 = {.x = 5.4418f, .y = 0.0f, .z = 1.8596f};
static const Vector3i data_FC10 = {.x = 0, .y = 54613, .z = 0};
static const Vector3 data_FC1C = {.x = 1000.0f, .y = 0.0f, .z = 0.0f};
static const char data_FC28[10] __attribute__((scn_string_literal)) = "E1012B056";
static const char data_FC32[10] __attribute__((scn_string_literal)) = "E1012A230";
static const char data_FC3C[10] __attribute__((scn_string_literal)) = "E1012B057";
static const char data_FC46[10] __attribute__((scn_string_literal)) = "E1012A248";
static const char data_FC50[10] __attribute__((scn_string_literal)) = "E1012B058";
static const char data_FC5A[10] __attribute__((scn_string_literal)) = "E1012A249";
static const char data_FC64[10] __attribute__((scn_string_literal)) = "E1012B059";
static const char data_FC6E[10] __attribute__((scn_string_literal)) = "E1012A250";
static const char data_FC78[10] __attribute__((scn_string_literal)) = "E1012B060";
static const char data_FC82[10] __attribute__((scn_string_literal)) = "E1012A251";
static const char data_FC8C[12] __attribute__((scn_string_literal)) = "E1012B061";
static const Vector3 data_FC98 = {.x = -6.4f, .y = -0.29f, .z = -5.64f};
static const Vector3 data_FCA4 = {.x = -6.4f, .y = -0.29f, .z = -6.24f};
static const Vector3 data_FCB0 = {.x = -6.26f, .y = 2.57f, .z = -9.31f};
static const Vector3 data_FCBC = {.x = -6.13f, .y = 2.12f, .z = -9.07f};
static const Vector3 data_FCC8 = {.x = -5.98f, .y = 2.73f, .z = -7.63f};
static const Vector3 data_FCD4 = {.x = -6.84f, .y = 2.73f, .z = -7.69f};
static const Vector3i data_FCE0 = {.x = 0, .y = 0, .z = 0};
static const Vector3i data_FCEC = {.x = 52064, .y = 31857, .z = 0};
static const Vector3i data_FCF8 = {.x = 12925, .y = 34042, .z = 2366};
static const Vector3i data_FD04 = {.x = 49152, .y = 62805, .z = 910};
static const Vector4 data_FD10 = {.x = 0.4f, .y = 0.4f, .z = 0.4f, .w = 0.4f};
static const char data_FD20[10] __attribute__((scn_string_literal)) = "E1012A174";
static const char data_FD2A[10] __attribute__((scn_string_literal)) = "E1012B036";
static const char data_FD34[10] __attribute__((scn_string_literal)) = "E1012A232";
static const char data_FD3E[10] __attribute__((scn_string_literal)) = "E1012B037";
static const char data_FD48[12] __attribute__((scn_string_literal)) = "E1012A233";
static const int8_t data_FD54[12] = {3, 10, 17, 24, 31, 0, 0, 0, 0, 0, 0, 0};
static const int8_t data_FD60[12] = {7, 14, 21, 28, 0, 0, 0, 0, 0, 0, 0, 0};
static const int8_t data_FD6C[12] = {4, 11, 18, 25, 0, 0, 0, 0, 0, 0, 0, 0};
static const int8_t data_FD78[9] = {4, 11, 18, 25, 0, 0, 0, 0, 0};
static const char data_FD81[59] __attribute__((scn_string_literal)) = " : %d %d %d %d %d %d : %d %d %d %d : %d %d %d %d : %d %d \012";
static const char data_FDBC[12] __attribute__((scn_string_literal)) = "lacky : %d\012";
static const Vector4 data_FDC8 = {.x = 1.0f, .y = 0.82f, .z = 0.59f, .w = 0.46f};
static const float data_FDD8[11] = {0.71f, 0.71f, 1.0f, 0.62f, 0.44f, 0.11f, 0.3f, 3.2f, 4.01f, 1.83f, 3.98f};
static const Vector4 data_FE04 = {.x = 0.09f, .y = 0.09f, .z = 0.09f, .w = 0.09f};
static const Vector4 data_FE14 = {.x = 0.16f, .y = 0.16f, .z = 0.16f, .w = 0.16f};
static const Vector4 data_FE24 = {.x = 0.2f, .y = 0.2f, .z = 0.2f, .w = 0.2f};
static uint32_t data_FE34 = 0x3DB851EC;
static const float data_FE38[10] = {0.39f, 0.28f, 0.14f, 0.8f, 3.0f, 0.71f, 0.58f, 7.352f, 0.977f, 6.9672f};
static uint32_t data_FE60 = 0x3DB851EC;
static const float data_FE64[10] = {0.39f, 0.28f, 0.14f, 0.8f, 3.0f, 0.71f, 0.58f, 7.8153f, 0.6f, 1.715f};
static uint32_t data_FE8C = 0x3DB851EC;
static const float data_FE90[10] = {0.39f, 0.28f, 0.14f, 0.8f, 3.0f, 0.93f, 0.58f, 1.758f, 0.922f, 7.1292f};
static uint32_t data_FEB8 = 0x3DB851EC;
static const float data_FEBC[46] = {
  0.39f, 0.28f, 0.14f, 0.8f, 3.0f, 0.93f, 0.58f, 0.864f, 1.058f, 1.0872f, 7.351f, 0.856f, 6.9662f, 7.351f, 0.856f, 6.9662f, 7.351f, 0.856f, 6.9662f,
  7.8153f, 0.6f, 1.7152f, 7.8153f, 0.6f, 1.7152f, 7.8153f, 0.6f, 1.7152f, 1.758f, 0.863f, 7.1292f, 1.758f, 0.858f, 7.1292f, 1.758f, 0.858f, 7.1292f,
  0.864f, 1.058f, 1.0872f, 0.864f, 1.058f, 1.0872f, 0.864f, 1.058f, 1.0872f
};
static const uint32_t data_FF74 = SHEN_NAME('RO11');
static const shen_name data_FF78[3] = {SHEN_NAME('RO21'), SHEN_NAME('RO31'), SHEN_NAME('RO41')};
static const uint32_t data_FF84 = SHEN_NAME('FIR1');
static const shen_name data_FF88[3] = {SHEN_NAME('FIR2'), SHEN_NAME('FIR3'), SHEN_NAME('FIR5')};
static const float data_FF94[3] = {-6.4407f, -0.23f, -4.8865f};
static const Vector3 data_FFA0 = {.x = -9.44f, .y = 0.0f, .z = -3.58f};
static const Vector3 data_FFAC = {.x = 17.0f, .y = 0.0f, .z = 9.11f};
static const char data_FFB8[12] __attribute__((scn_string_literal)) = "E1012A287";
static const Vector3 data_FFC4 = {.x = -6.4407f, .y = -0.23f, .z = -4.8865f};
static const Vector3 data_FFD0 = {.x = 0.109f, .y = 0.78f, .z = 5.4f};
static const Vector3i data_FFDC = {.x = 0, .y = 16384, .z = 34588};
static const char data_FFE8[10] __attribute__((scn_string_literal)) = "E1012A083";
static const char data_FFF2[22] = "E1012A288\000RI}?\000\000\000\000\031s①";
static const Vector3 data_10008 = {.x = 0.8894f, .y = 0.0f, .z = 5.6328f};
static const Vector3 data_10014 = {.x = 0.109f, .y = 0.78f, .z = 5.4f};
static const Vector3i data_10020 = {.x = 0, .y = 16384, .z = 34588};
static const Vector3 data_1002C = {.x = 6.27f, .y = 0.08f, .z = 0.97f};
static const Vector3 data_10038 = {.x = -0.38f, .y = 0.0f, .z = 0.93f};
static const char data_10044[10] __attribute__((scn_string_literal)) = "E1012A040";
static const char data_1004E[10] __attribute__((scn_string_literal)) = "E1012A041";
static const char data_10058[12] __attribute__((scn_string_literal)) = "E1012A042";
static const Vector3 data_10064 = {.x = 0.029f, .y = -0.226f, .z = -0.015f};
static const Vector3i data_10070 = {.x = 182, .y = 32395, .z = 63170};
static const Vector3 data_1007C = {.x = 6.27f, .y = 0.08f, .z = 0.97f};
static const Vector3 data_10088 = {.x = -0.38f, .y = 0.0f, .z = 0.93f};
static const char data_10094[12] __attribute__((scn_string_literal)) = "E1012A043";
static const Vector3 data_100A0 = {.x = 6.5542f, .y = 0.1558f, .z = -0.2961f};
static const char data_100AC[12] __attribute__((scn_string_literal)) = "E1012A045";
static const Vector3 data_100B8 = {.x = 6.5542f, .y = 0.1558f, .z = -0.2961f};
static const Vector3i data_100C4 = {.x = 0, .y = 0, .z = 0};
static const Vector3 data_100D0 = {.x = 6.52369f, .y = 0.0f, .z = 0.76372f};
static const Vector3i data_100DC = {.x = 0, .y = 16384, .z = 0};
typedef struct Row_100E8 { float w0; float w1; float w2; int32_t w3; int32_t w4; int32_t w5; } Row_100E8;
static const Row_100E8 data_100E8[3] = {
  {0.164f, 0.001f, -0.026f, 14848, 37983, 63169},
  {0.198f, 0.039f, -0.094f, 14848, 48903, 57890},
  {0.205f, -0.03f, -0.086f, 63532, 65353, 4173}
};
static const int32_t data_10130[3] = {0, 8736, -6551};
static const Vector3 data_1013C = {.x = 0.147f, .y = 0.047f, .z = -0.041f};
static const Vector3i data_10148 = {.x = 58434, .y = 60801, .z = 8359};
static const Vector3 data_10154 = {.x = 7.0863f, .y = 0.0492f, .z = 0.7153f};
static const Vector3i data_10160 = {.x = 16932, .y = 32241, .z = 16376};
static const float data_1016C[6] = {6.2496f, 0.032f, 1.0096f, 0.0f, 0.0f, 0.0f};
static const Vector3 data_10184 = {.x = 0.107f, .y = -0.026f, .z = 0.022f};
static const Vector3i data_10190 = {.x = 15281, .y = 52974, .z = 16729};
static const Vector3 data_1019C = {.x = 6.27f, .y = 0.08f, .z = 0.97f};
static const Vector3 data_101A8 = {.x = -0.38f, .y = 0.0f, .z = 0.93f};
static const char data_101B4[10] __attribute__((scn_string_literal)) = "E1012A276";
static const char data_101BE[10] __attribute__((scn_string_literal)) = "E1012A278";
static const char data_101C8[10] __attribute__((scn_string_literal)) = "E1012A279";
static const char data_101D2[10] __attribute__((scn_string_literal)) = "E1012A081";
static const uint32_t data_101DC = SHEN_NAME('HN01');
static const shen_name data_101E0[17] = {
  SHEN_NAME('HN02'), SHEN_NAME('HN03'), SHEN_NAME('HN04'), SHEN_NAME('HN05'), SHEN_NAME('HN06'), SHEN_NAME('HN07'), SHEN_NAME('HN08'),
  SHEN_NAME('HN09'), SHEN_NAME('HN10'), SHEN_NAME('HN11'), SHEN_NAME('HN12'), SHEN_NAME('HN13'), SHEN_NAME('HN14'), SHEN_NAME('HN15'),
  SHEN_NAME('HN16'), SHEN_NAME('HN17'), SHEN_NAME('HN18')
};
static const Vector3 data_10224 = {.x = 1000.0f, .y = 0.0f, .z = 0.0f};
static const char data_10230[6] __attribute__((scn_string_literal)) = "MPK00";
static const char data_10236[] __attribute__((scn_string_literal)) = "DYN2";
static const uint8_t data_1023B[13] = {
  0x00, 0xE1, 0x0B, 0x7F, 0x40, 0x7B, 0x14, 0xFE, 0x3F, 0x78, 0x0B, 0x80, 0x40
};
static const Vector3 data_10248 = {.x = 3.5899f, .y = 0.006f, .z = 4.095f};
static const Vector3i data_10254 = {.x = 0, .y = 54613, .z = 0};
static const Vector3 data_10260 = {.x = 0.156f, .y = -0.016f, .z = 0.026f};
static const Vector3i data_1026C = {.x = 64623, .y = 0, .z = 49503};
static const char data_10278[60] = "E1012A253\000\000\000\221\355\374>\000\000\000\000\370S\003\277\356\016\000\000\000\000\000\000\256%\000\000X9\364>%\006\001>\236\357\347\276\345\330\000\000\334\302\000\000\2259";
static const Vector3 data_102B4 = {.x = 7.1f, .y = 0.51f, .z = 6.04f};
static const Vector3 data_102C0 = {.x = 0.04f, .y = 0.03f, .z = -1.0f};
static const float data_102CC[3] = {0.455f, 0.033f, -0.521f};
static const int32_t data_102D8[3] = {4004, 363, 9646};
static const Vector3 data_102E4 = {.x = 0.173f, .y = 0.031f, .z = -0.006f};
static const Vector3i data_102F0 = {.x = 59530, .y = 45151, .z = 63170};
static const Vector3 data_102FC = {.x = 6.9213f, .y = 0.0f, .z = 6.7797f};
static const char data_10308[12] __attribute__((scn_string_literal)) = "E1012A074";
static const Vector3 data_10314 = {.x = 6.819f, .y = -0.1f, .z = 6.8966f};
static const Vector3 data_10320 = {.x = 7.2565f, .y = -0.1f, .z = 1.6284f};
static const Vector3 data_1032C = {.x = 1.8398f, .y = -0.1f, .z = 6.531f};
static const Vector3 data_10338 = {.x = 1.3389f, .y = 0.0f, .z = 1.4422f};
static const Vector3i data_10344 = {.x = 0, .y = 49152, .z = 0};
static const Vector3i data_10350 = {.x = 0, .y = 49152, .z = 0};
static const Vector3i data_1035C = {.x = 0, .y = 32768, .z = 0};
static const Vector3i data_10368 = {.x = 0, .y = 16384, .z = 0};
static const Vector3 data_10374 = {.x = 0.144f, .y = -0.037f, .z = 0.032f};
static const Vector3 data_10380 = {.x = 0.134f, .y = -0.013f, .z = 0.036f};
static const Vector3 data_1038C = {.x = 0.138f, .y = -0.006f, .z = 0.039f};
static const Vector3 data_10398 = {.x = 0.11f, .y = -0.006f, .z = 0.044f};
static const Vector3i data_103A4 = {.x = 36352, .y = 45056, .z = 34980};
static const Vector3i data_103B0 = {.x = 0, .y = 22755, .z = 8823};
static const Vector3i data_103BC = {.x = 35419, .y = 35845, .z = 39298};
static const Vector3i data_103C8 = {.x = 63493, .y = 35333, .z = 3629};
static const uint32_t data_103D4 = 0x40E7020C;
static const float data_103D8[2] = {0.96f, 6.958f};
static const uint32_t data_103E0 = 0x40F56873;
static const float data_103E4[2] = {0.71f, 1.648f};
static const uint32_t data_103EC = 0x3FE33333;
static const float data_103F0[2] = {0.937f, 6.99f};
static const uint32_t data_103F8 = 0x3F7D2F1B;
static const float data_103FC[2] = {1.13f, 1.138f};
static const Vector3 data_10404 = {.x = 0.15352f, .y = 0.00543f, .z = -0.03765f};
static const Vector3i data_10410 = {.x = 16208, .y = 6632, .z = -531};
static const float data_1041C[3] = {0.126f, 0.01f, -0.078f};
static const int32_t data_10428[3] = {1274, 726, 12921};
static const Vector3 data_10434 = {.x = 0.12602f, .y = 0.0194f, .z = -0.07814f};
static const Vector3i data_10440 = {.x = 7100, .y = -20670, .z = -5280};
static const Vector3 data_1044C = {.x = 6.78512f, .y = 0.0f, .z = 6.93103f};
static const Vector3 data_10458 = {.x = 7.20058f, .y = 0.0f, .z = 1.6771f};
static const Vector3 data_10464 = {.x = 1.79375f, .y = 0.0f, .z = 6.55893f};
static const Vector3 data_10470 = {.x = 1.39775f, .y = 0.0f, .z = 1.30095f};
static const Vector3 data_1047C = {.x = 0.135f, .y = 0.005f, .z = -0.11f};
static const Vector3i data_10488 = {.x = 0, .y = 49152, .z = 0};
static const Vector3i data_10494 = {.x = 0, .y = 49152, .z = 0};
static const Vector3i data_104A0 = {.x = 0, .y = 32768, .z = 0};
static const Vector3i data_104AC = {.x = 0, .y = 16384, .z = 0};
static const Vector3i data_104B8 = {.x = 60803, .y = 58074, .z = 363};
static const Vector3 data_104C4 = {.x = 7.63f, .y = 0.85f, .z = 1.78f};
static const uint8_t pad_104D0[12] = {0};
static const Vector3 data_104DC = {.x = 6.8346f, .y = 1.1817f, .z = 4.2377f};
static const uint32_t data_104E8 = 0x40D48312;
static const float data_104EC[2] = {0.001f, 2.976f};
static const Vector3 data_104F4 = {.x = 0.246f, .y = -0.111f, .z = -0.011f};
static const Vector3i data_10500 = {.x = 47698, .y = 35852, .z = 54069};
static const Vector3 data_1050C = {.x = 6.3842f, .y = 0.0f, .z = 4.3417f};
static const Vector3 data_10518 = {.x = 0.141f, .y = 0.05f, .z = -0.072f};
static const Vector3i data_10524 = {.x = 61168, .y = 16379, .z = 6734};
static const Vector3 data_10530 = {.x = 8.1723f, .y = 0.0f, .z = 4.056f};
static Vector3 data_1053C = {.x = 0.0f, .y = 0.0f, .z = 0.0f};
static const char data_10548[10] __attribute__((scn_string_literal)) = "E1012A119";
static const char data_10552[10] __attribute__((scn_string_literal)) = "E1012A177";
static const char data_1055C[12] __attribute__((scn_string_literal)) = "E1012A178";
static const Vector3 data_10568 = {.x = 2.13037f, .y = 0.0f, .z = 1.47989f};
static const Vector3i data_10574 = {.x = 0, .y = 0, .z = 0};
static const Vector3 data_10580 = {.x = 0.11707f, .y = 0.00757f, .z = -0.04618f};
static const Vector3i data_1058C = {.x = 20415, .y = 1030, .z = -21097};
static const Vector3 data_10598 = {.x = 0.15672f, .y = -0.274f, .z = 0.0831f};
static const Vector3i data_105A4 = {.x = 51821, .y = 1537, .z = 18136};
static const Vector3 data_105B0 = {.x = 0.13467f, .y = 0.04983f, .z = 0.03832f};
static const Vector3i data_105BC = {.x = -11948, .y = 990, .z = 15958};
static const Vector3 data_105C8 = {.x = 2.0278f, .y = 0.208f, .z = 1.7903f};
static const Vector3 data_105D4 = {.x = 2.064f, .y = 0.02f, .z = 2.977f};
static const Vector3i data_105E0 = {.x = 0, .y = 32768, .z = 0};
static const Vector3 data_105EC = {.x = 0.0f, .y = 0.0f, .z = 0.0f};
static const Vector3i data_105F8 = {.x = 0, .y = 0, .z = 0};
static const Vector3 data_10604 = {.x = 2.5863f, .y = 0.039f, .z = 1.4193f};
static const Vector3i data_10610 = {.x = 54424, .y = 32241, .z = 16376};
static const Vector3 data_1061C = {.x = 2.5863f, .y = 0.23f, .z = 1.1f};
static const Vector3 data_10628 = {.x = 0.21f, .y = 0.0f, .z = -0.8f};
static const char data_10634[10] __attribute__((scn_string_literal)) = "E1012A208";
static const char data_1063E[10] __attribute__((scn_string_literal)) = "E1012A209";
static const char data_10648[8] __attribute__((scn_string_literal)) = "HAKO_4B";
static const Vector3 data_10650 = {.x = 3.9661f, .y = 0.0f, .z = 0.57016f};
static const Vector3i data_1065C = {.x = 0, .y = 49152, .z = 0};
static const Vector3 data_10668 = {.x = 0.0f, .y = 0.0f, .z = 0.0f};
static const Vector3i data_10674 = {.x = 0, .y = 0, .z = 0};
static const Vector3 data_10680 = {.x = 6.574f, .y = 0.2129f, .z = -0.021f};
static const Vector3 data_1068C = {.x = 1000.0f, .y = 0.0f, .z = 0.0f};
static const char data_10698[20] = "E1012\000\000\000囮\031>囮\031>囮\031>";
static const Vector3 data_106AC = {.x = 1.4f, .y = 1.3f, .z = 1.4f};
static const float data_106B8[3] = {7.9424f, 1.1933f, 1.4459f};
static const Vector3 data_106C4 = {.x = -5.85f, .y = 0.0f, .z = -6.77f};
static const Vector3 data_106D0 = {.x = -6.85f, .y = 0.0f, .z = -7.47f};
static const Vector3 data_106DC = {.x = -4.7f, .y = 0.0f, .z = -2.9f};
static const Vector3 data_106E8 = {.x = -8.1f, .y = 0.0f, .z = -4.1f};
static const float data_106F4[3] = {-6.44f, -0.23f, -6.0294f};
static const Vector3 data_10700 = {.x = 7.5946f, .y = 0.0f, .z = 0.2f};
static const Vector3 data_1070C = {.x = 3.7934f, .y = 0.0f, .z = 3.7934f};
static const Vector3 data_10718 = {.x = -0.238f, .y = 0.0f, .z = 5.0076f};
static const Vector3 data_10724 = {.x = 0.0619f, .y = -0.23f, .z = 2.9076f};
static const Vector3 data_10730 = {.x = 0.029f, .y = -0.226f, .z = -0.015f};
static const Vector3i data_1073C = {.x = 65171, .y = 32395, .z = 63170};
static const char data_10748[6] __attribute__((scn_string_literal)) = "MPK00";
static const char data_1074E[13] __attribute__((scn_string_literal)) = "HMOT_DB1.BIN";
static const char data_1075B[13] __attribute__((scn_string_literal)) = "HMOT_STD.BIN";
static const char data_10768[12] __attribute__((scn_string_literal)) = "E1012A083";
static const Vector3 data_10774 = {.x = 4.6022f, .y = 0.0f, .z = 0.537f};
static const Vector3 data_10780 = {.x = 4.1692f, .y = 0.991f, .z = 0.623f};
static const Vector3 data_1078C = {.x = 4.7412f, .y = 0.994f, .z = 0.588f};
static const Vector3 data_10798 = {.x = 5.0912f, .y = 0.146f, .z = 0.7f};
static const Vector3i data_107A4 = {.x = 0, .y = 43690, .z = 0};
static const Vector3 data_107B0 = {.x = 6.5627f, .y = -0.0004f, .z = 0.3441f};
static const Vector3i data_107BC = {.x = 46887, .y = 49340, .z = 18403};
static const Vector3 data_107C8 = {.x = 6.4408f, .y = 0.0321f, .z = 0.3646f};
static const Vector3i data_107D4 = {.x = 23286, .y = 9631, .z = 29398};
static const Vector3 data_107E0 = {.x = 5.4462f, .y = 0.718f, .z = 0.813f};
static const float data_107EC[3] = {6.574f, 0.2129f, -0.021f};
static const Vector3 data_107F8 = {.x = 6.5392f, .y = 0.0428f, .z = 2.0235f};
static const Vector3i data_10804 = {.x = 49672, .y = 14006, .z = 33396};
static const char data_10810[6] __attribute__((scn_string_literal)) = "MPK00";
static const char data_10816[] __attribute__((scn_string_literal)) = "DYN3";
static const uint8_t data_1081B[13] = {
  0x00, 0x9A, 0x99, 0x19, 0x3E, 0x9A, 0x99, 0x19, 0x3E, 0x9A, 0x99, 0x19, 0x3E
};
static const Vector3 data_10828 = {.x = 1.4f, .y = 1.3f, .z = 1.4f};
static const Vector3 data_10834 = {.x = -6.401f, .y = -0.29f, .z = -6.347f};
static const int32_t data_10840[3] = {0, 30947, 0};
static const char data_1084C[12] __attribute__((scn_string_literal)) = "E1012A001";
static const uint32_t data_10858 = SHEN_NAME('HN01');
static const shen_name data_1085C[17] = {
  SHEN_NAME('HN02'), SHEN_NAME('HN03'), SHEN_NAME('HN04'), SHEN_NAME('HN05'), SHEN_NAME('HN06'), SHEN_NAME('HN07'), SHEN_NAME('HN08'),
  SHEN_NAME('HN09'), SHEN_NAME('HN10'), SHEN_NAME('HN11'), SHEN_NAME('HN12'), SHEN_NAME('HN13'), SHEN_NAME('HN14'), SHEN_NAME('HN15'),
  SHEN_NAME('HN16'), SHEN_NAME('HN17'), SHEN_NAME('HN18')
};
static const char data_108A0[7] __attribute__((scn_string_literal)) = "start\012";
static const char data_108A7[6] __attribute__((scn_string_literal)) = "MPK00";
static const char data_108AD[] __attribute__((scn_string_literal)) = "DYN1";
static const uint8_t data_108B2[2] = {0};

static void script_0030(int32_t arg0, const char *arg1, const char *arg2) {
  for (;;) {
    if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0) != 0) {
      break;
    }
    if (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 5, arg2) == 0) {
      while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, arg1, arg2) == 0) {
        SCNF_WaitFrames_0000(1);
      }
      while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2)) {
        SCNF_WaitFrames_0000(1);
      }
    }
  }
  return;
}

static void script_0094(const char *arg0) {
  if (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 5, arg0) != 0) {
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, arg0);
  }
  return;
}

static void script_00B5(int32_t arg0) {
  L0000B7:;
  SCNF_WaitFrames_0000(1);
  if (!Script_CycleManagerUtils(CYCLE_CMD_GET_NEXT_TASK_POINTER, arg0)) goto L0000B7;
  return;
}

static void script_00CC(int32_t arg0) {
  int32_t local_FC;
  local_FC = 0;
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0) != 0) {
    local_FC = Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT_INTERNAL_HUMAN, arg0);
  } else {
    Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT, arg0, 1);
    while (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0) == 0) {
      local_FC = Script_CycleManagerUtils(CYCLE_CMD_GET_CYCLE_PTR_BY_CHARA_ID, arg0);
      while (local_FC == 0) {
        SCNF_WaitFrames_0000(1);
        local_FC = Script_CycleManagerUtils(CYCLE_CMD_GET_CYCLE_PTR_BY_CHARA_ID, arg0);
      }
      script_00B5(local_FC);
    }
  }
  (void)local_FC;
  return;
}

static void script_0152(int32_t arg0, int32_t arg1) {
  if (arg1) {
    Script_CycleManagerUtils(CYCLE_CMD_LOAD_OR_RESET_RUNNING_PROGRAM, arg1);
  }
  Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT, arg0, 0);
  arg1 = 0;
  return;
}

static int32_t script_0175(int32_t arg0, int32_t arg1) {
  if (arg1 > 0) {
    SCNF_WaitFrames_0000(arg1);
  }
  Script_executeAicaCommand(arg0, 0, 0);
  return Script_srchControl(0);
}

static void script_0199(int32_t arg0, int32_t arg1) {
  Script_scenCreateTask(script_0175, 2, arg0, arg1);
  return;
}

static int32_t script_01AB(const char *arg0, int32_t arg1) {
  if (arg1 > 0) {
    SCNF_WaitFrames_0000(arg1);
  }
  Script_StartOrFreeSEQ(0);
  Script_ShowUIMessageFromID(arg0);
  return Script_srchControl(0);
}

static void script_01D2(int32_t arg0, int32_t arg1) {
  Script_scenCreateTask(script_01AB, 2, arg0, arg1);
  return;
}

static int32_t script_01E4(int32_t arg0, int32_t arg1, int32_t arg2, const Vector3 *arg3, const Vector3i *arg4, int32_t arg5) {
  Vector3 local_F4;
  Vector3i local_E8;
  local_F4.x = arg3->x;
  (&local_F4)->y = arg3->y;
  (&local_F4)->z = arg3->z;
  local_E8.x = arg4->x;
  (&local_E8)->y = arg4->y;
  (&local_E8)->z = arg4->z;
  if (arg5 >= 0) {
    SCNF_WaitFrames_0000(arg5);
  }
  Script_fixoAttachEx(arg0, arg1, arg2, arg3, arg4);
  return Script_srchControl(0);
}

static void script_024F(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
  Script_scenCreateTask(script_01E4, 6, arg0, arg1, arg2, arg3, arg4, arg5);
  return;
}

static int32_t script_0269(int32_t arg0, int32_t arg1) {
  if (arg1 >= 0) {
    SCNF_WaitFrames_0000(arg1);
  }
  Script_fixoClearAttachment(arg0);
  return Script_srchControl(0);
}

static void script_0289(int32_t arg0, int32_t arg1) {
  Script_scenCreateTask(script_0269, 2, arg0, arg1);
  return;
}

static int32_t script_029B(int32_t arg0, int32_t arg1, int32_t arg2) {
  if (arg2 >= 0) {
    SCNF_WaitFrames_0000(arg2);
  }
  Script_MoveObjectToObject(arg0, arg1);
  Script_EVT_SetPosition(arg0, POS_XYZ, &data_E88C);
  Script_EVT_SetRotation(arg0, 0x38000000, &data_E898, 0);
  return Script_srchControl(0);
}

static void script_02DF(int32_t arg0, int32_t arg1, int32_t arg2) {
  Script_scenCreateTask(script_029B, 3, arg0, arg1, arg2);
  return;
}

static void script_02F3(int32_t arg0) {
  while (Script_IsCharaMotionFinished(arg0) == 0) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static int32_t script_030F(int32_t arg0, int32_t arg1, int32_t arg2, float arg3, int32_t arg4, int32_t arg5) {
  if (arg5 >= 0) {
    SCNF_WaitFrames_0000(arg5);
  }
  if (arg4 & -0x80000000) {
    Script_motmToggleFlags(arg0, 2, 0x40);
  } else {
    Script_motmToggleFlags(arg0, 1, 0x40);
  }
  if (arg4 & 0x40000000) {
    Script_motmToggleFlags(arg0, 1, 0x80);
  } else {
    Script_motmToggleFlags(arg0, 2, 0x80);
  }
  if (arg4 & 0x20000000) {
    Script_charaToggleMotEnabled(arg0, 0);
  } else {
    Script_charaToggleMotEnabled(arg0, 1);
  }
  Script_SetCharaMotionPlaybackRate(arg0, arg3);
  Script_PlayCharaMotion(arg0, arg1, arg2, -1.0f, -1.0f, 1.0f, 0);
  return Script_srchControl(0);
}

static void script_03B3(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
  Script_scenCreateTask(script_030F, 6, arg0, arg1, arg2, arg3, arg4, arg5);
  return;
}

static int32_t script_03CD(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  if (arg3 > 0) {
    SCNF_WaitFrames_0000(arg3);
  }
  Script_CharHandlers(arg0, 2, arg1, 16);
  if (arg4 > 0) {
    SCNF_WaitFrames_0000(arg4);
  }
  Script_CharHandlers(arg0, 2, arg2, 16);
  return Script_srchControl(0);
}

static void script_0412(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  Script_scenCreateTask(script_03CD, 5, arg0, arg1, arg2, arg3, arg4);
  return;
}

static int32_t script_042A(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  if (arg3 >= 0) {
    SCNF_WaitFrames_0000(arg3);
  }
  Script_SetNewHandMotion(arg0, arg1, arg2, 0, 0, 0, 0, 1.0f, 0);
  return Script_srchControl(0);
}

static void script_045E(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Script_scenCreateTask(script_042A, 4, arg0, arg1, arg2, arg3);
  return;
}

static int32_t script_0474(int32_t arg0, int32_t arg1, const Vector3i *arg2, int32_t arg3) {
  Vector3i local_F4;
  local_F4.x = arg2->x;
  (&local_F4)->y = arg2->y;
  (&local_F4)->z = arg2->z;
  if (arg3 >= 0) {
    SCNF_WaitFrames_0000(arg3);
  }
  Script_SetHandRootRotation(arg0, arg1, 0x15, arg2);
  return Script_srchControl(0);
}

static void script_04BC(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Script_scenCreateTask(script_0474, 4, arg0, arg1, arg2, arg3);
  return;
}

static int32_t script_04D2(int32_t arg0, int32_t arg1, const int32_t *arg2, int32_t arg3, int32_t arg4) {
  if (arg4 >= 0) {
    SCNF_WaitFrames_0000(arg4);
  }
  Script_handSetFloatsFor(arg0, arg1, arg2, arg3);
  return Script_srchControl(0);
}

static void script_04F8(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  Script_scenCreateTask(script_04D2, 5, arg0, arg1, arg2, arg3, arg4);
  return;
}

static int32_t script_0510(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  if (arg4 >= 0) {
    SCNF_WaitFrames_0000(arg4);
  }
  Script_FACE(arg0, arg1, arg2, arg3);
  return Script_srchControl(0);
}

static void script_0537(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  Script_scenCreateTask(script_0510, 5, arg0, arg1, arg2, arg3, arg4);
  return;
}

static int32_t script_054F(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  if (arg2 > 0) {
    SCNF_WaitFrames_0000(arg2);
  }
  Script_GetDataFromFightProgram(arg0, arg1);
  if (arg3 > 0) {
    SCNF_WaitFrames_0000(arg3);
  }
  Script_GetDataFromFightProgram(arg0, 0);
  return Script_srchControl(0);
}

static void script_058C(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Script_scenCreateTask(script_054F, 4, arg0, arg1, arg2, arg3);
  return;
}

static int32_t script_05A2(int32_t arg0, int32_t arg1, float arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
  Vector3 local_F4;
  int32_t local_F0;
  if (arg4 > 0) {
    SCNF_WaitFrames_0000(arg4);
  }
  local_F0 = arg5;
  while (local_F0 > 0) {
    Script_EVT_GetPosition(arg1, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
    (&local_F4)->y = (&local_F4)->y + arg2;
    Script_SetLookPointDetails(arg0, arg3 + 0x18000, &local_F4, 0);
    local_F0 = local_F0 - 1;
    SCNF_WaitFrames_0000(1);
  }
  Script_SetCharaLookAtPosition(arg0, -(arg3 + 0x18000), 0, 0);
  return Script_srchControl(0);
}

static void script_0626(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
  Script_scenCreateTask(script_05A2, 6, arg0, arg1, arg2, arg3, arg4, arg5);
  return;
}

static int32_t script_0640(int32_t arg0, int32_t arg1, int32_t arg2) {
  int32_t local_FC;
  if (arg2 > 0) {
    SCNF_WaitFrames_0000(arg2);
  }
  local_FC = arg1;
  while (local_FC > 0) {
    Script_FACE_LCG(arg0, 1, 2, 0);
    SCNF_WaitFrames_0000(2);
    Script_FACE_LCG(arg0, 2, 2, 0);
    SCNF_WaitFrames_0000(Script_FloatUtils(ScriptMathOp_RandomIntBelow, 100) + 50);
    local_FC = local_FC - 1;
  }
  return Script_srchControl(0);
}

static void script_06A9(int32_t arg0, int32_t arg1, int32_t arg2) {
  Script_scenCreateTask(script_0640, 3, arg0, arg1, arg2);
  return;
}

static int32_t script_06BD(int32_t arg0, int32_t arg1, const Vector3 *arg2, int32_t arg3) {
  Vector3 local_F4;
  int32_t local_F0;
  local_F4.x = arg2->x;
  (&local_F4)->y = arg2->y;
  (&local_F4)->z = arg2->z;
  if (arg3 >= 0) {
    SCNF_WaitFrames_0000(arg3);
  }
  local_F0 = Script_CEWPRelated(arg0);
  Script_MoveObjectToObject(arg0, arg1);
  Script_EVT_SetPosition(arg0, POS_XYZ, arg2);
  Script_MoveObjectToObject(arg0, local_F0);
  return Script_srchControl(0);
}

static void script_0721(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Script_scenCreateTask(script_06BD, 4, arg0, arg1, arg2, arg3);
  return;
}

static int32_t script_0737(int32_t arg0, int32_t arg1, float arg2, float arg3, float arg4, int32_t arg5) {
  Vector3 local_F4;
  int32_t local_F0;
  local_F4.x = arg2;
  (&local_F4)->y = arg3;
  (&local_F4)->z = arg4;
  if (arg5 >= 0) {
    SCNF_WaitFrames_0000(arg5);
  }
  local_F0 = Script_CEWPRelated(arg0);
  Script_MoveObjectToObject(arg0, arg1);
  Script_EVT_SetPosition(arg0, POS_XYZ, &local_F4);
  Script_MoveObjectToObject(arg0, local_F0);
  return Script_srchControl(0);
}

static void script_0792(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
  Script_scenCreateTask(script_0737, 6, arg0, arg1, arg2, arg3, arg4, arg5);
  return;
}

static int32_t script_07AC(int32_t arg0, int32_t arg1, const Vector3i *arg2, int32_t arg3) {
  Vector3i local_F4;
  int32_t local_F0;
  local_F4.x = arg2->x;
  (&local_F4)->y = arg2->y;
  (&local_F4)->z = arg2->z;
  if (arg3 >= 0) {
    SCNF_WaitFrames_0000(arg3);
  }
  local_F0 = Script_CEWPRelated(arg0);
  Script_MoveObjectToObject(arg0, arg1);
  Script_EVT_SetRotation(arg0, 0x38000000, arg2, 0);
  Script_MoveObjectToObject(arg0, local_F0);
  return Script_srchControl(0);
}

static void script_0812(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Script_scenCreateTask(script_07AC, 4, arg0, arg1, arg2, arg3);
  return;
}

static int32_t script_0828(int32_t arg0, int32_t arg1, float arg2, int32_t arg3) {
  Vector3i local_F4;
  int32_t local_F0;
  local_F4.x = 0;
  (&local_F4)->y = (int32_t)(arg2 / 360.0f * (float)0x10000);
  (&local_F4)->z = 0;
  if (arg3 >= 0) {
    SCNF_WaitFrames_0000(arg3);
  }
  local_F0 = Script_CEWPRelated(arg0);
  Script_MoveObjectToObject(arg0, arg1);
  Script_EVT_SetRotation(arg0, 0x38000000, &local_F4, 0);
  Script_MoveObjectToObject(arg0, local_F0);
  return Script_srchControl(0);
}

static void script_0893(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Script_scenCreateTask(script_0828, 4, arg0, arg1, arg2, arg3);
  return;
}

static int32_t script_08A9(int32_t arg0, const Vector3i *arg1, int32_t arg2) {
  Vector3i local_F4;
  local_F4.x = arg1->x;
  (&local_F4)->y = arg1->y;
  (&local_F4)->z = arg1->z;
  if (arg2 >= 0) {
    SCNF_WaitFrames_0000(arg2);
  }
  Script_EVT_SetRotation(arg0, -0x48000000, arg1, 0);
  return Script_srchControl(0);
}

static void script_08F3(int32_t arg0, int32_t arg1, int32_t arg2) {
  Script_scenCreateTask(script_08A9, 3, arg0, arg1, arg2);
  return;
}

static int32_t script_0907(int32_t arg0, float arg1, int32_t arg2) {
  Vector3i local_F4;
  local_F4.x = 0;
  (&local_F4)->y = (int32_t)(arg1 / 360.0f * (float)0x10000);
  (&local_F4)->z = 0;
  if (arg2 >= 0) {
    SCNF_WaitFrames_0000(arg2);
  }
  Script_EVT_SetRotation(arg0, -0x48000000, &local_F4, 0);
  return Script_srchControl(0);
}

static void script_0956(int32_t arg0, int32_t arg1, int32_t arg2) {
  Script_scenCreateTask(script_0907, 3, arg0, arg1, arg2);
  return;
}

static int32_t script_096A(int32_t arg0, float arg1, float arg2, float arg3, int32_t arg4) {
  int32_t local_F4[3];
  float local_F0;
  if (arg4 >= 0) {
    SCNF_WaitFrames_0000(arg4);
  }
  local_F4[1] = 0;
  local_F4[2] = 0;
  local_F0 = (arg2 - arg1) / arg3;
  for (;;) {
    if (arg3 <= 0.0f) {
      break;
    }
    local_F4[0] = (int32_t)(local_F0 / 360.0f * (float)0x10000);
    script_08F3(arg0, (int32_t)local_F4, -1);
    arg3 = arg3 - 1.0f;
    SCNF_WaitFrames_0000(1);
  }
  return Script_srchControl(0);
}

static void script_09E3(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  Script_scenCreateTask(script_096A, 5, arg0, arg1, arg2, arg3, arg4);
  return;
}

static int32_t script_09FB(int32_t arg0, float arg1, float arg2, float arg3, int32_t arg4) {
  int32_t local_F4[3];
  float local_F0;
  if (arg4 >= 0) {
    SCNF_WaitFrames_0000(arg4);
  }
  local_F4[0] = 0;
  local_F4[2] = 0;
  local_F0 = (arg2 - arg1) / arg3;
  for (;;) {
    if (arg3 <= 0.0f) {
      break;
    }
    local_F4[1] = (int32_t)(local_F0 / 360.0f * (float)0x10000);
    script_08F3(arg0, (int32_t)local_F4, -1);
    arg3 = arg3 - 1.0f;
    SCNF_WaitFrames_0000(1);
  }
  return Script_srchControl(0);
}

static void script_0A74(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  Script_scenCreateTask(script_09FB, 5, arg0, arg1, arg2, arg3, arg4);
  return;
}

static int32_t script_0A8C(int32_t arg0, float arg1, float arg2, float arg3, int32_t arg4) {
  int32_t local_F4[3];
  float local_F0;
  if (arg4 >= 0) {
    SCNF_WaitFrames_0000(arg4);
  }
  local_F4[0] = 0;
  local_F4[1] = 0;
  local_F0 = (arg2 - arg1) / arg3;
  for (;;) {
    if (arg3 <= 0.0f) {
      break;
    }
    local_F4[2] = (int32_t)(local_F0 / 360.0f * (float)0x10000);
    script_08F3(arg0, (int32_t)local_F4, -1);
    arg3 = arg3 - 1.0f;
    SCNF_WaitFrames_0000(1);
  }
  return Script_srchControl(0);
}

static void script_0B05(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  Script_scenCreateTask(script_0A8C, 5, arg0, arg1, arg2, arg3, arg4);
  return;
}

static int32_t script_0B1D(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  if (arg3 > 0) {
    SCNF_WaitFrames_0000(arg3);
  }
  if (arg1) {
    Script_charaDisplay(arg0, CDM_Show);
    Script_scenToggleActiveFor(arg0, arg2);
  } else {
    Script_charaDisplay(arg0, CDM_Hide);
    Script_scenToggleActiveFor(arg0, 0);
  }
  return Script_srchControl(0);
}

static void script_0B62(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Script_scenCreateTask(script_0B1D, 4, arg0, arg1, arg2, arg3);
  return;
}

static int32_t script_0B78(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  if (arg3 >= 0) {
    SCNF_WaitFrames_0000(arg3);
  }
  Script_seqEnterEvtCam(arg0, arg1, arg2);
  return Script_srchControl(0);
}

static void script_0B9C(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Script_scenCreateTask(script_0B78, 4, arg0, arg1, arg2, arg3);
  return;
}

static int32_t script_0BB2(float arg0, float arg1, float arg2, int32_t arg3) {
  if (arg3 >= 0) {
    SCNF_WaitFrames_0000(arg3);
  }
  Script_SetCamShake(arg0, arg1, arg2);
  return Script_srchControl(0);
}

static void script_0BD7(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Script_scenCreateTask(script_0BB2, 4, arg0, arg1, arg2, arg3);
  return;
}

static void script_0BED(void) {
  Script_shenClearFreeState(0);
  Script_SetCameraFlags(0);
  task_240 = 0;
  task_244 = 0;
  task_248 = Script_SetMaxNPCsNear(16);
  task_24C = Script_ryoControl(PCCTL_QueryShouldRun);
  if (task_24C) {
    Script_ryoControl(PCCTL_DisablePlayerControl);
    Script_scenUtils(SCENARIO_CMD_INIT_SCNT);
    Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
    Script_CycleManagerUtils(CYCLE_CMD_PROCESS_SND_EXTRA);
  }
  return;
}

static void script_0C48(int32_t arg0, int32_t arg1) {
  if (task_240 < 16) {
    task_0[task_240][0] = arg0;
    task_0[task_240][1] = arg1;
    task_0[task_240][3] = Script_ccowUtils(arg0, 7);
    task_0[task_240][2] = Script_ccowUtils(arg0, 8);
    task_0[task_240][4] = Script_motmGetCharaFlags(arg0, 0, 1);
    if (arg1 & -0x80000000) {
      Script_facemotUtils(arg0, 0, 0);
    }
    if (arg1 & 0x40000000) {
      Script_facemotUtils(arg0, 1, 0);
    }
    Script_FaceEx(arg0, 1, 0, 0);
    Script_ResetHandMotion(arg0, 1, 1, 0);
    Script_ResetHandMotion(arg0, 1, 0, 0);
    Script_FACE(arg0, 0, 0, 0);
    Script_FACE_LCG(arg0, 2, 0, 0);
    Script_FaceLookAtTargetUtils(arg0, 0);
    Script_ccowUtils(arg0, 2);
    Script_motmGetCharaFlags(arg0, 0, 0);
    Script_charaToggleMotEnabled(arg0, 1);
    Script_motmToggleFlags(arg0, 1, 0x40);
    Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_15);
    Script_PlayCharaMotion(arg0, MN_AKI_AKI_STAND_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_SetFightProgramStateBytes(arg0, 2, 1);
    Script_SetCPAMFlags(arg0, 0);
    Script_SetShadowFlags(arg0, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
    Script_SetCharaFlags(arg0, 4);
    Script_scenToggleActiveFor(arg0, 1);
    task_240 = task_240 + 1;
  }
  return;
}

static void script_0DBA(int32_t arg0, int32_t arg1) {
  if (task_244 < 32) {
    task_140[task_244][0] = arg0;
    task_140[task_244][1] = arg1;
    Script_SetShadowFlags(arg0, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
    Script_SetCharaFlags(arg0, 0);
    Script_scenToggleActiveFor(arg0, 1);
    task_244 = task_244 + 1;
  }
  return;
}

static void script_0E0E(void) {
  while (task_240 > 0) {
    script_0E6D();
  }
  while (task_244 > 0) {
    script_0F98();
  }
  if (task_24C) {
    Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 1);
    Script_SetGameStateTo(AUTH_None);
    Script_scenUtils(SCENARIO_CMD_UPDATE_TIMER);
    Script_ryoControl(RYO_RestoreControl);
  }
  Script_SetMaxNPCsNear(task_248);
  Script_shenClearFreeState(1);
  Script_SetCameraFlags(1);
  return;
}

static void script_0E6D(void) {
  int32_t local_FC;
  int32_t local_F8;
  if (task_240 > 0) {
    task_240 = task_240 - 1;
    local_FC = task_0[task_240][0];
    local_F8 = task_0[task_240][1];
    if (task_0[task_240][3]) {
      Script_ccowUtils(local_FC, 5);
    }
    if (task_0[task_240][2]) {
      Script_ccowUtils(local_FC, 3);
    }
    Script_motmGetCharaFlags(local_FC, task_0[task_240][4], 0);
    Script_motmToggleFlags(local_FC, 1, 0x40);
    Script_motmSetBlendFrames(local_FC, MOTM_BLEND_FRAMES_5);
    Script_PlayCharaMotion(local_FC, 0xFFFF, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_SetFightProgramStateBytes(local_FC, 5, 2);
    Script_SetCPAMFlags(local_FC, 1);
    Script_ResetHandMotion(local_FC, 0, 1, 0);
    Script_ResetHandMotion(local_FC, 0, 0, 0);
    Script_FaceEx(local_FC, 0, 0, 0);
    if (local_F8 & 0x40000000) {
      if (local_FC != 0x52494B41) {
        Script_faceUpdateFor(local_FC, 1, 0);
      }
    }
    if (local_F8 & -0x80000000) {
      Script_faceUpdateFor(local_FC, 0, 0);
    }
  }
  return;
}

static void script_0F98(void) {
  int32_t local_FC;
  if (task_244 > 0) {
    task_244 = task_244 - 1;
    local_FC = task_140[task_244][0];
  }
  return;
}

static void script_0FBF(void) {
  int32_t local_FC;
  local_FC = Script_GetSceneID();
  switch (local_FC) {
    case 1:
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_ENTRY2_TIMED_EVENT_DONE) == 0) {
        Script_TimedTransitionUtils(TIMED_TRANSITION_REGISTER, 1, 0x3044484A, 2, &data_E914, 0xD5A6C3, TimedTransitionUpdateMemoItem150);
      }
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JD00_ENTRY20_TIMED_EVENT_DONE) == 0) {
        Script_TimedTransitionUtils(TIMED_TRANSITION_REGISTER, 1, 0x3030444A, 20, &data_E8DC, 0xDA3AA3, 0);
      }
      break;
    case 2:
      if ((Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_GMCT_ROUTE_STARTED) == 0) | (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_GMCT_ROUTE_ADVANCED) == 0)) {
        Script_TimedTransitionUtils(TIMED_TRANSITION_REGISTER, 2, 0x54434D47, 1, &data_E8A4, 0xB7420F, 0);
      }
      break;
    case 3:
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOZOMI_RESCUE_RUSH_ACTIVE) == 0) {
        Script_TimedTransitionUtils(TIMED_TRANSITION_REGISTER, 3, 0x4F4D4F4A, 20, &data_E8C0, 0x2DC9FE, TimedTransitionRollbackToPreviousDay2330);
      }
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_FIRED_FROM_JOB) & ~Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 970)) {
        if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_MFSY_ENTRY42_TIMED_EVENT_DONE) == 0) {
          Script_TimedTransitionUtils(TIMED_TRANSITION_REGISTER, 3, 0x5953464D, 42, &data_E8F8, 0xBBB817, 0);
        }
      }
      break;
  }
  return;
}

static void script_10F7(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Vector3 local_F4;
  int32_t local_F0;
  int16_t local_EE;
  task_440 = arg3;
  task_444 = arg2;
  task_414 = 0;
  Script_srchUtils(SRCH_SCRIPT_InitEntryTableFromTaskOffset, &data_E93C);
  task_430 = arg1;
  task_428 = arg0;
  task_42C = 1;
  task_614 = 0;
  task_448 = 0;
  Script_scenToggleVisibleFor(task_440, 0);
  Script_scenToggleVisibleFor(task_444, 0);
  if (arg0 == 1) {
    Script_lightResetSlotToDefaults(28);
    Script_LightUtils(28, 0, 4);
    Script_LightUtils(28, 2, &data_F258);
    Script_LightUtils(28, 4, 1.7f);
    Script_LightUtils(28, 5, 2.8f);
    Script_LightUtils(28, 6, data_F268);
    Script_LightUtils(28, 10, 1820);
  }
  local_F0 = 0;
  for (;;) {
    local_EE = Script_ScenDataUtil(&data_E93C, 2, local_F0 * 16);
    if (local_EE < 0) {
      break;
    }
    if ((Script_ScenDataUtil(&data_E93C, 2, local_F0 * 16 + 1) == 2) | (Script_ScenDataUtil(&data_E93C, 2, local_F0 * 16 + 1) == 9)) {
      if (Script_ScenDataUtil(&data_E93C, 4, local_F0 * 8 + 6) != 0) {
        Script_EVT_GetPosition(Script_ScenDataUtil(&data_E93C, 4, local_F0 * 8 + 1), -1, &local_F4, 0);
        if (!((&local_F4)->x < 9999.0f) & !((&local_F4)->z < 9999.0f)) {
          task_614 = task_614 + 1;
          task_484[task_614] = Script_ScenDataUtil(&data_E93C, 4, local_F0 * 8 + 1);
        }
      }
    }
    local_F0 = local_F0 + 1;
  }
  return;
}

static void script_1294(void) {
  int32_t unused_FC;
  int32_t local_F8;
  int32_t local_F4;
  int32_t local_F0;
  Vector3 local_E4;
  local_F0 = 0;
  for (;;) {
    local_F4 = Script_ScenDataUtil(data_EE80, 4, local_F0 * 5);
    local_F8 = Script_ScenDataUtil(data_EE80, 4, local_F0 * 5 + 1);
    if (local_F4 == -1) {
      break;
    }
    Script_EVT_GetPosition(local_F8, -1, &local_E4, 0);
    if (!((&local_E4)->x < 9999.0f) & !((&local_E4)->z < 9999.0f)) {
      task_614 = task_614 + 1;
      task_484[task_614] = local_F8;
    } else if (Script_ScenDataUtil(data_EE80, 4, local_F0 * 5 + 3) == 0) {
      Script_mtwkGetSetRotation(local_F8, &task_3F4, 0);
      Script_EVT_GetPosition(local_F8, -1, &task_3E8, 0);
      Script_fixoAttachEx(local_F8, local_F4, 0, &task_3E8, &task_3F4);
    } else {
      task_3F4.x = 0;
      task_3F4.y = 0;
      task_3F4.z = 0;
      task_3E8.x = 0.0f;
      task_3E8.y = 0.0f;
      task_3E8.z = 0.0f;
      Script_CalculateMatrixRotationFromParams(local_F8, local_F4, 0, &task_3E8, &task_3F4);
    }
    local_F0 = local_F0 + 1;
  }
  return;
}

static int32_t script_13B4(void) {
  int16_t local_FE;
  int32_t local_F8;
  task_40C = -1;
  task_40C = Script_srchUtils(-1);
  if (Script_srchUtils(SRCH_SCRIPT_GetCurrentTargetEntryIndex) >= 0) {
    if (task_428 == 1) {
      Script_lightSetSlotEnabled(28, 1);
      script_2607(CHARA_AKIR);
      task_434 = 1;
    }
    Script_scenUtils(SCENARIO_CMD_INIT_SCNT);
    if (task_430) {
      task_410 = Script_srchUtils(SRCH_SCRIPT_GetCurrentTargetEntryIndex);
      local_FE = Script_ScenDataUtil(&data_E93C, 2, task_410 * 16 + 1);
      if ((local_FE < 0) & (Script_ScenDataUtil(&data_E93C, 4, task_410 * 8 + 7) > 0)) {
        Script_sprintf("Map Start!!!!!!!!!!!!!!!!!\012");
        script_27A6(task_410);
      } else {
        if (task_438 != task_410) {
          for (;;) {
            if (!(task_428 == 1)) goto L001480;
            L001480:;
            if (Script_srchUtils(SRCH_SCRIPT_UpdateAndPollSelection) == Script_ScenDataUtil(&data_E93C, 4, Script_srchUtils(SRCH_SCRIPT_GetCurrentTargetEntryIndex) * 8 + 1)) {
              break;
            }
            if (Script_srchUtils(SRCH_SCRIPT_GetCurrentTargetEntryIndex) < 0) {
              break;
            }
            SCNF_WaitFrames_0000(1);
          }
          for (;;) {
            if ((MMTK_ScriptMemoToggle(2, 0) == 0) & (Script_GetConversationDetails(CONV_DETAILS_MODE_GET_GAME_STATE, 0) == 7)) {
              break;
            }
          }
          task_448 = 9;
          script_4658(Script_ScenDataUtil(&data_E93C, 4, Script_srchUtils(SRCH_SCRIPT_GetCurrentTargetEntryIndex) * 8 + 1));
          task_438 = Script_srchUtils(SRCH_SCRIPT_GetCurrentTargetEntryIndex);
        }
      }
    }
    task_420 = 1;
  } else {
    if (task_434 == 1) {
      Script_lightSetSlotEnabled(28, 0);
      task_434 = 0;
    }
    Script_scenUtils(SCENARIO_CMD_UPDATE_TIMER);
    task_420 = 0;
    if (task_430) {
      task_438 = -1;
    }
  }
  if (task_40C != -1) {
    task_418 = 0;
    task_41C = 0;
    task_424 = 0;
    Script_ryoControl(PCCTL_DisablePlayerControl);
    Script_convSetCamInactiveFlag(1);
    local_F8 = script_25A2(task_40C);
    local_FE = Script_ScenDataUtil(&data_E93C, 2, local_F8 * 16 + 1);
    Script_scenToggleVisibleFor(task_440, 1);
    Script_scenToggleVisibleFor(task_444, 1);
    if (local_FE < 0) {
      task_448 = 8;
      script_4658(task_40C);
    } else {
      script_160E(local_F8);
    }
    Script_ryoControl(RYO_RestoreControl);
    Script_scenToggleVisibleFor(task_440, 0);
    Script_scenToggleVisibleFor(task_444, 0);
  }
  return task_40C;
}

static void script_160E(int32_t arg0) {
  int16_t local_FE;
  uint8_t unused_F0[14];
  int32_t local_EC;
  local_FE = Script_ScenDataUtil(&data_E93C, 2, arg0 * 16 + 1);
  local_EC = local_FE;
  if (local_EC == 0) {
    task_448 = 8;
    script_4658(task_40C);
  } else {
    switch (local_EC) {
      case 0:
        task_448 = 8;
        script_4658(task_40C);
        break;
      case 2:
        script_21CD(arg0);
        break;
      case 3:
        script_176A(arg0, 3);
        break;
      case 4:
        script_176A(arg0, 10);
        break;
      case 5:
        script_176A(arg0, 20);
        break;
      case 6:
        script_176A(arg0, 30);
        break;
      case 7:
        script_176A(arg0, 40);
        break;
      case 8:
        script_176A(arg0, 50);
        break;
      case 9:
        Script_InvestigationUtils(ShenmueUtils_BeginInteractionMode, 0, 60);
        for (;;) {
          if (Script_InvestigationUtils(ShenmueUtils_PollInteractionModeDone) != 0) {
            break;
          }
          SCNF_WaitFrames_0000(1);
        }
        script_21CD(arg0);
        Script_InvestigationUtils(ShenmueUtils_RequestInteractionModeExit);
        for (;;) {
          if (Script_InvestigationUtils(ShenmueUtils_PollInteractionModeDone) != 0) {
            break;
          }
          SCNF_WaitFrames_0000(1);
        }
        break;
    }
  }
  return;
}

static void script_176A(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t unused_F0[2];
  int32_t local_EC;
  int32_t local_E8;
  int32_t local_E4;
  int32_t local_E0;
  int32_t unused_D8[2];
  int32_t local_D4;
  int32_t local_D0;
  int32_t local_CC;
  int32_t unused_B4[6];
  int32_t local_B0;
  int32_t local_AC;
  int32_t local_A8;
  int32_t local_A4;
  int32_t local_A0;
  int32_t local_9C;
  int32_t local_98;
  int32_t local_94;
  int8_t local_93;
  uint8_t unused_92;
  int8_t local_91;
  uint8_t unused_90;
  Vector3 local_84;
  int32_t unused_80;
  float local_FF78[2];
  int32_t local_FF74;
  local_91 = 0;
  local_93 = 0;
  local_FC = Script_ScenDataUtil(&data_E93C, 4, arg0 * 8 + 1);
  local_CC = Script_ScenDataUtil(&data_E93C, 4, arg0 * 8 + 6);
  local_D0 = Script_ScenDataUtil(&data_E93C, 4, arg0 * 8 + 7);
  local_E4 = 0;
  if (Script_ScenDataUtil(&data_EEE8, 4, local_E4) != -1) {
    local_B0 = Script_ScenDataUtil(&data_EEE8, 4, local_E4);
    local_AC = Script_ScenDataUtil(&data_EEE8, 4, local_E4 + 1);
    local_A8 = Script_ScenDataUtil(&data_EEE8, 4, local_E4 + 2);
    local_A4 = Script_ScenDataUtil(&data_EEE8, 4, local_E4 + 3);
    local_E4 = 4;
    for (;;) {
      if ((Script_ScenDataUtil(&data_EEE8, 4, local_E4) == -1) & (Script_ScenDataUtil(&data_EEE8, 4, local_E4 + 1) == -1)) {
        local_B0 = 0;
        local_AC = 0;
        local_A8 = 0;
        local_A4 = 0;
        break;
      }
      if (Script_ScenDataUtil(&data_EEE8, 4, local_E4) == -1) {
        local_E4 = local_E4 + 1;
        local_B0 = Script_ScenDataUtil(&data_EEE8, 4, local_E4);
        local_AC = Script_ScenDataUtil(&data_EEE8, 4, local_E4 + 1);
        local_A8 = Script_ScenDataUtil(&data_EEE8, 4, local_E4 + 2);
        local_A4 = Script_ScenDataUtil(&data_EEE8, 4, local_E4 + 3);
        local_E4 = local_E4 + 4;
      }
      if (local_FC == Script_ScenDataUtil(&data_EEE8, 4, local_E4)) {
        break;
      }
      local_E4 = local_E4 + 1;
    }
  }
  if (task_430) {
    task_448 = 4;
    script_4658(local_FC);
  }
  if (task_424 == 0) {
    if (local_D0 != 0) {
      task_44C = *(int32_t *)(data_F024 + (local_D0 - 1) * 56);
      *(&task_44C + 1) = *(int32_t *)(data_F024 + ((local_D0 - 1) * 56 + 4));
      *(&task_44C + 2) = *(int32_t *)(data_F024 + ((local_D0 - 1) * 56 + 8));
      *(&task_44C + 3) = *(int32_t *)(data_F024 + ((local_D0 - 1) * 56 + 12));
      *(&task_44C + 4) = *(int32_t *)(data_F024 + ((local_D0 - 1) * 56 + 16));
      *(&task_44C + 5) = *(int32_t *)(data_F024 + ((local_D0 - 1) * 56 + 20));
      *(&task_44C + 6) = *(int32_t *)(data_F024 + ((local_D0 - 1) * 56 + 24));
      *(&task_44C + 7) = *(int32_t *)(data_F024 + ((local_D0 - 1) * 2 + 1) * 28);
      *(&task_44C + 8) = *(int32_t *)(data_F024 + (((local_D0 - 1) * 2 + 1) * 28 + 4));
      *(&task_44C + 9) = *(int32_t *)(data_F024 + (((local_D0 - 1) * 2 + 1) * 28 + 8));
      *(&task_44C + 10) = *(int32_t *)(data_F024 + (((local_D0 - 1) * 2 + 1) * 28 + 12));
      *(&task_44C + 11) = *(int32_t *)(data_F024 + (((local_D0 - 1) * 2 + 1) * 28 + 16));
      *(&task_44C + 12) = *(int32_t *)(data_F024 + (((local_D0 - 1) * 2 + 1) * 28 + 20));
      *(&task_44C + 13) = *(int32_t *)(data_F024 + (((local_D0 - 1) * 2 + 1) * 28 + 24));
      Script_InvestigationUtils(ShenmueUtils_LoadSpecialAssetEntry, &task_44C);
    }
    if (local_CC != 0) {
      arg1 = local_CC - 1;
    }
    Script_InvestigationUtils(ShenmueUtils_QueueSndCommand, local_B0, local_AC);
    Script_InvestigationUtils(ShenmueUtils_BeginInteractionMode, local_FC, arg1);
    local_E0 = local_E0 + 1;
    for (;;) {
      if (task_428 == 1) {
        script_2607(local_FC);
      }
      if (Script_InvestigationUtils(ShenmueUtils_PollInteractionModeDone) != 0) {
        break;
      }
      SCNF_WaitFrames_0000(1);
      local_E0 = local_E0 + 1;
      if (local_E0 > 30) {
        if (Script_GetControllerInput(1) == 0x400) {
          local_91 = 1;
        }
      }
    }
    local_D4 = local_FC;
    task_448 = 5;
    script_4658(local_FC);
    local_E4 = 0;
    for (;;) {
      local_EC = Script_ScenDataUtil(data_EE80, 4, local_E4 * 5);
      if (local_EC == -1) {
        break;
      }
      if (local_EC == task_40C) {
        local_E8 = Script_ScenDataUtil(data_EE80, 4, local_E4 * 5 + 1);
        local_F8 = Script_ScenDataUtil(data_EE80, 4, local_E4 * 5 + 2);
        local_FF74 = Script_ScenDataUtil(data_EE80, 4, local_E4 * 5 + 4);
        Script_fixoClearAttachment(local_E8);
        Script_MoveObjectToObject(local_E8, local_FC);
        Script_EVT_GetPosition(local_E8, -1, &task_3E8, 0);
        Script_mtwkGetSetRotation(local_E8, &task_3F4, 0);
        Script_MoveObjectToObject(local_E8, -1);
        local_E4 = -1;
        break;
      }
      local_E4 = local_E4 + 1;
    }
    local_E0 = 0;
    if (Script_ScenDataUtil(&data_EEE8, 4, local_E0) != -1) {
      local_A0 = Script_ScenDataUtil(&data_EEE8, 4, local_E0);
      local_9C = Script_ScenDataUtil(&data_EEE8, 4, local_E0 + 1);
      local_98 = Script_ScenDataUtil(&data_EEE8, 4, local_E0 + 2);
      local_94 = Script_ScenDataUtil(&data_EEE8, 4, local_E0 + 3);
      local_E0 = 4;
      for (;;) {
        if ((Script_ScenDataUtil(&data_EEE8, 4, local_E0) == -1) & (Script_ScenDataUtil(&data_EEE8, 4, local_E0 + 1) == -1)) {
          local_A0 = 0;
          local_9C = 0;
          local_98 = 0;
          local_94 = 0;
          break;
        }
        if (Script_ScenDataUtil(&data_EEE8, 4, local_E0) == -1) {
          local_E0 = local_E0 + 1;
          local_A0 = Script_ScenDataUtil(&data_EEE8, 4, local_E0);
          local_9C = Script_ScenDataUtil(&data_EEE8, 4, local_E0 + 1);
          local_98 = Script_ScenDataUtil(&data_EEE8, 4, local_E0 + 2);
          local_94 = Script_ScenDataUtil(&data_EEE8, 4, local_E0 + 3);
          local_E0 = local_E0 + 4;
        }
        if (local_E8 == Script_ScenDataUtil(&data_EEE8, 4, local_E0)) {
          break;
        }
        local_E0 = local_E0 + 1;
      }
    }
    local_E0 = 0;
    for (;;) {
      if (task_614 == local_E0) {
        break;
      }
      local_E0 = local_E0 + 1;
      if (task_484[local_E0] == local_E8) {
        Script_sprintf("このアイテムもってる");
        local_E4 = 0;
      }
    }
    if (local_E4 == -1) {
      if (Script_scenCheckCharaExist(local_E8)) {
        Script_sprintf("tansu naka = %d\012", Script_charaDisplay(local_E8, CDM_Query));
        if (Script_charaDisplay(local_E8, CDM_Query) == 0) {
          local_E4 = 0;
        }
      }
    }
    if (task_41C == 1) {
      local_F8 = 0;
    }
    if ((local_E4 == -1) & (task_42C >= 0)) {
      for (;;) {
        if (task_428 == 1) {
          script_2607(local_FC);
        }
        Script_InvestigationUtils(ShenmueUtils_UpdateSpecialAssetInput);
        if ((Script_GetControllerInput(0) & 0x400) | (local_91 == 1)) {
          Script_EVT_GetPosition(local_E8, -1, &local_84, 0);
          Script_ScreenToWorldAssign(&local_84, (void *)local_FF78);
          if (-!(*local_FF78 <= 80.0f) & -!(*local_FF78 >= 560.0f) & -!(local_FF78[1] <= 60.0f) & -!(local_FF78[1] >= 420.0f)) {
            if (local_F8 > -1) {
              break;
            }
          }
        }
        if (Script_GetControllerInput(0) & 0x200 & -(task_42C == 1)) {
          local_F8 = -1;
          break;
        }
        SCNF_WaitFrames_0000(1);
      }
      if (local_F8 != -1) {
        Script_InvestigationUtils(ShenmueUtils_QueueSndCommand, local_A0, local_9C);
        if (local_FF74 == 0) {
          Script_InvestigationUtils(ShenmueUtils_BeginTargetModeDefault, local_E8);
        } else {
          Script_InvestigationUtils(ShenmueUtils_BeginTargetModeByType, local_E8, local_FF74 - 1);
        }
        for (;;) {
          if (Script_InvestigationUtils(ShenmueUtils_UpdateStateMachine) != 0) {
            break;
          }
          SCNF_WaitFrames_0000(1);
        }
        if (task_428 == 1) {
          script_2607(local_FC);
        }
        task_448 = 1;
        script_4658(local_E8);
        if (task_42C >= 0) {
          for (;;) {
            if (task_428 == 1) {
              script_2607(local_FC);
            }
            if ((local_91 == 1) & (task_42C == 1)) {
              break;
            }
            if (Script_GetControllerInput(0) & 0x400 & -(task_42C == 1)) {
              break;
            }
            if (Script_GetControllerInput(0) & 0x200 & -(task_42C == 1)) {
              local_F8 = 0;
              break;
            }
            SCNF_WaitFrames_0000(1);
            if (task_418 == 0) {
              Script_InvestigationUtils(ShenmueUtils_UpdateInterpolatedSpeed);
            }
          }
          if (local_F8 == 0) {
            if (task_430) {
              task_448 = 3;
              script_4658(local_E8);
            }
            Script_InvestigationUtils(ShenmueUtils_QueueSndCommand, local_98, local_94);
            Script_InvestigationUtils(ShenmueUtils_RequestTransitionVariantA);
          } else {
            if (task_430) {
              task_448 = 2;
              script_4658(local_E8);
            }
            if (local_F8 & 0x8000000) {
              Script_GetCassetteFlags(local_F8, 1, 0, 1);
              SCNF_StartIconSystemMenu(local_F8 + 255);
            } else {
              Script_GetCassetteFlags(local_F8, 1, 0, 0);
              SCNF_StartIconSystemMenu(local_F8);
            }
            Script_InvestigationUtils(ShenmueUtils_RequestTransitionVariantB);
            task_614 = task_614 + 1;
            task_484[task_614] = local_E8;
            local_93 = 1;
          }
          for (;;) {
            if (Script_InvestigationUtils(ShenmueUtils_UpdateStateMachine) != 0) {
              break;
            }
            SCNF_WaitFrames_0000(1);
          }
        }
      }
      Script_fixoAttachEx(local_E8, local_FC, 0, &task_3E8, &task_3F4);
    } else {
      if (task_42C >= 0) {
        for (;;) {
          if (task_428 == 1) {
            script_2607(local_FC);
          }
          Script_InvestigationUtils(ShenmueUtils_UpdateSpecialAssetInput);
          if ((local_91 == 1) & (task_42C == 1)) {
            break;
          }
          if (Script_GetControllerInput(0) & 0x400 & -(task_42C == 1)) {
            break;
          }
          if (Script_GetControllerInput(0) & 0x200 & -(task_42C == 1)) {
            break;
          }
          SCNF_WaitFrames_0000(1);
        }
      }
    }
    if (task_42C >= 0) {
      Script_InvestigationUtils(ShenmueUtils_QueueSndCommand, local_A8, local_A4);
      Script_InvestigationUtils(ShenmueUtils_RequestInteractionModeExit);
      task_448 = 6;
      script_4658(local_D4);
      for (;;) {
        if (Script_InvestigationUtils(ShenmueUtils_PollInteractionModeDone) != 0) {
          break;
        }
        SCNF_WaitFrames_0000(1);
      }
      task_448 = 7;
      script_4658(local_D4);
      if (!(task_428 == 1)) goto L00219F;
      L00219F:;
      if (local_93 == 1) {
        Script_srchUtils(SRCH_SCRIPT_Reset);
        Script_SetGameStateTo(AUTH_None);
      }
    } else {
      task_42C = 1;
      Script_srchUtils(SRCH_SCRIPT_Reset);
      Script_SetGameStateTo(AUTH_None);
    }
  }
  return;
}

static void script_21CD(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  int32_t local_F0;
  int32_t local_EC;
  int32_t local_E8;
  int32_t local_E4;
  int8_t local_E3;
  int8_t local_E2;
  local_E3 = 0;
  local_FC = Script_ScenDataUtil(&data_E93C, 4, arg0 * 8 + 1);
  local_E4 = 0;
  if (Script_ScenDataUtil(&data_EEE8, 4, local_E4) != -1) {
    local_F4 = Script_ScenDataUtil(&data_EEE8, 4, local_E4);
    local_F0 = Script_ScenDataUtil(&data_EEE8, 4, local_E4 + 1);
    local_EC = Script_ScenDataUtil(&data_EEE8, 4, local_E4 + 2);
    local_E8 = Script_ScenDataUtil(&data_EEE8, 4, local_E4 + 3);
    local_E4 = 4;
    for (;;) {
      if ((Script_ScenDataUtil(&data_EEE8, 4, local_E4) == -1) & (Script_ScenDataUtil(&data_EEE8, 4, local_E4 + 1) == -1)) {
        local_F4 = 0;
        local_F0 = 0;
        local_EC = 0;
        local_E8 = 0;
        break;
      }
      if (Script_ScenDataUtil(&data_EEE8, 4, local_E4) == -1) {
        local_E4 = local_E4 + 1;
        local_F4 = Script_ScenDataUtil(&data_EEE8, 4, local_E4);
        local_F0 = Script_ScenDataUtil(&data_EEE8, 4, local_E4 + 1);
        local_EC = Script_ScenDataUtil(&data_EEE8, 4, local_E4 + 2);
        local_E8 = Script_ScenDataUtil(&data_EEE8, 4, local_E4 + 3);
        local_E4 = local_E4 + 4;
      }
      if (local_FC == Script_ScenDataUtil(&data_EEE8, 4, local_E4)) {
        break;
      }
      local_E4 = local_E4 + 1;
    }
  }
  if (Script_charaDisplay(local_FC, CDM_Query)) {
    Script_InvestigationUtils(ShenmueUtils_QueueSndCommand, local_F4, local_F0);
    if (Script_ScenDataUtil(&data_E93C, 4, arg0 * 8 + 7) == 0) {
      Script_InvestigationUtils(ShenmueUtils_BeginTargetModeDefault, local_FC);
    } else {
      Script_InvestigationUtils(ShenmueUtils_BeginTargetModeByType, local_FC, Script_ScenDataUtil(&data_E93C, 4, arg0 * 8 + 7) - 1);
    }
    local_E2 = 0;
    for (;;) {
      SCNF_WaitFrames_0000(1);
      if (Script_InvestigationUtils(ShenmueUtils_UpdateStateMachine) != 0) {
        break;
      }
      if (Script_GetControllerInput(1) == 0x400) {
        local_E2 = 1;
      }
    }
    if (task_428 == 1) {
      script_2607(local_FC);
    }
    task_448 = 1;
    script_4658(task_40C);
    local_F8 = Script_ScenDataUtil(&data_E93C, 4, arg0 * 8 + 6);
    for (;;) {
      if (task_428 == 1) {
        script_2607(local_FC);
      }
      SCNF_WaitFrames_0000(1);
      if (task_418 == 0) {
        Script_InvestigationUtils(ShenmueUtils_UpdateInterpolatedSpeed);
      }
      if ((local_E2 == 1) & (task_42C == 1)) {
        break;
      }
      if (Script_GetControllerInput(0) & 0x400 & -(task_42C == 1)) {
        break;
      }
      if (Script_GetControllerInput(0) & 0x200 & -(task_42C == 1)) {
        local_F8 = 0;
        break;
      }
    }
    if (task_41C == 1) {
      local_F8 = 0;
    }
    if (local_F8 == 0) {
      Script_InvestigationUtils(ShenmueUtils_QueueSndCommand, local_EC, local_E8);
      Script_InvestigationUtils(ShenmueUtils_RequestTransitionVariantA);
    } else {
      if (task_430) {
        task_448 = 2;
        script_4658(task_40C);
      }
      if (local_F8 & 0x8000000) {
        Script_GetCassetteFlags(local_F8, 1, 0, 1);
        SCNF_StartIconSystemMenu(local_F8 + 255);
      } else {
        Script_GetCassetteFlags(local_F8, 1, 0, 0);
        SCNF_StartIconSystemMenu(local_F8);
      }
      Script_InvestigationUtils(ShenmueUtils_RequestTransitionVariantB);
      task_614 = task_614 + 1;
      task_484[task_614] = local_FC;
      local_E3 = 1;
    }
    for (;;) {
      SCNF_WaitFrames_0000(1);
      if (Script_InvestigationUtils(ShenmueUtils_UpdateStateMachine) != 0) {
        break;
      }
    }
    if (!(task_428 == 1)) goto L00256A;
    L00256A:;
    if (local_E3 == 1) {
      Script_srchUtils(SRCH_SCRIPT_Reset);
      Script_SetGameStateTo(AUTH_None);
    }
    if (local_F8 == 0) {
      if (task_430) {
        task_448 = 3;
        script_4658(task_40C);
      }
    }
  }
  return;
}

static int32_t script_25A2(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  int16_t local_F6;
  local_FC = 0;
  local_F8 = 0;
  for (;;) {
    local_F6 = Script_ScenDataUtil(&data_E93C, 2, local_F8 * 16);
    if (local_F6 < 0) {
      break;
    }
    if (Script_ScenDataUtil(&data_E93C, 4, local_F8 * 8 + 1) == arg0) {
      local_FC = local_F8;
      break;
    }
    local_F8 = local_F8 + 1;
  }
  return local_FC;
}

static void script_2607(int32_t arg0) {
  Vector3 local_F4;
  int32_t unused_E8[3];
  Vector3 local_DC;
  Vector3 local_D0;
  Vector3 local_C4;
  int32_t unused_C0;
  int32_t local_B8[2];
  int32_t local_B4;
  int16_t local_B2;
  uint8_t unused_B0[2];
  local_B4 = script_25A2(arg0);
  local_B2 = Script_ScenDataUtil(&data_E93C, 2, local_B4 * 16 + 1);
  if (local_B2 < 0) {
    *(int32_t *)&local_F4.x = Script_ScenDataUtil(&data_E93C, 4, local_B4 * 8 + 3);
    *(int32_t *)&(&local_F4)->y = Script_ScenDataUtil(&data_E93C, 4, local_B4 * 8 + 4);
    *(int32_t *)&(&local_F4)->z = Script_ScenDataUtil(&data_E93C, 4, local_B4 * 8 + 5);
  } else {
    Script_EVT_GetPosition(arg0, -1, &local_F4, 0);
  }
  Script_GetAuthorState_And_SetCamPos(0, &local_DC);
  Script_GetAuthorState_And_SetCamPos(&local_D0, 0);
  local_B8[1] = Script_FloatUtils(ScriptMathOp_Atan2ReverseVector, (&local_D0)->x - (&local_DC)->x, (&local_D0)->z - (&local_DC)->z);
  local_B8[0] = Script_FloatUtils(ScriptMathOp_Atan2ReverseVector, (&local_D0)->z - (&local_DC)->z, (&local_D0)->y - (&local_DC)->y);
  local_C4.x = (float)0 - Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, local_B8[1]);
  (&local_C4)->y = (float)0 - Script_MathUtils(SCRIPT_MATH_COS_FIXED16, *(int32_t *)local_B8);
  (&local_C4)->z = (float)0 - Script_MathUtils(SCRIPT_MATH_COS_FIXED16, local_B8[1]);
  Script_lightSpotLightEnable(28, &local_D0, &local_C4);
  Script_LightUtils(28, 2, &data_F258);
  Script_LightUtils(28, 4, 0.85f);
  Script_LightUtils(28, 5, 7.0f);
  Script_LightUtils(28, 6, data_F268);
  Script_LightUtils(28, 10, 7500);
  return;
}

static void script_277E(void) {
  task_42C = 0;
  return;
}

static void script_2788(void) {
  task_42C = 1;
  return;
}

static void script_2792(void) {
  task_42C = -1;
  return;
}

static void script_279C(int32_t arg0) {
  task_428 = arg0;
  return;
}

static void script_27A6(int32_t arg0) {
  int32_t local_F4[3];
  int32_t unused_F0;
  int32_t local_E8[2];
  local_F4[0] = Script_ScenDataUtil(&data_E93C, 4, arg0 * 8 + 3);
  local_F4[1] = Script_ScenDataUtil(&data_E93C, 4, arg0 * 8 + 4);
  local_F4[2] = Script_ScenDataUtil(&data_E93C, 4, arg0 * 8 + 5);
  local_E8[1] = Script_ScenDataUtil(&data_E93C, 2, arg0 * 16 + 4);
  task_618 = Script_ScenDataUtil(&data_E93C, 4, arg0 * 8 + 7) - 1;
  *(&task_618 + 1) = 0;
  *(&task_618 + 2) = 0;
  *(&task_618 + 3) = 0;
  *(&task_618 + 4) = 0;
  *(&task_618 + 5) = local_E8[1];
  *(&task_618 + 6) = 0;
  *(&task_618 + 7) = -1;
  *(&task_618 + 8) = 0;
  *(&task_618 + 9) = 0;
  *(&task_618 + 10) = 0;
  *(&task_618 + 11) = 0;
  *(&task_618 + 12) = 0;
  *(&task_618 + 13) = 0;
  Script_InvestigationUtils(ShenmueUtils_LoadSpecialAssetEntry, &task_618);
  Script_InvestigationUtils(ShenmueUtils_BeginInteractionMode, (void *)local_F4, 60);
  for (;;) {
    if (Script_InvestigationUtils(ShenmueUtils_PollInteractionModeDone) != 0) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  for (;;) {
    Script_InvestigationUtils(ShenmueUtils_UpdateSpecialAssetInput);
    SCNF_WaitFrames_0000(1);
    if (Script_GetControllerInput(0) == 0x400) {
      break;
    }
    if (Script_GetControllerInput(0) == 0x200) {
      break;
    }
  }
  Script_InvestigationUtils(ShenmueUtils_RequestInteractionModeExit);
  for (;;) {
    if (Script_InvestigationUtils(ShenmueUtils_PollInteractionModeDone) != 0) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_srchUtils(SRCH_SCRIPT_Reset);
  return;
}

__attribute__((scn_string_bindings(6, data_F388, 7, data_F392)))
static int32_t script_294C(void) {
  int32_t local_FC;
  int32_t unused_F8;
  int32_t local_EC[3];
  int32_t local_E0[3];
  int32_t local_DC;
  int32_t local_D8;
  int32_t local_D4;
  int32_t unused_CC[2];
  int32_t local_C8;
  int32_t local_C4;
  task_334 = 0;
  task_330 = 0;
  local_EC[0] = 0;
  local_EC[1] = 0x3CF5C28F;
  local_EC[2] = 0x3CA3D70A;
  local_E0[0] = 0;
  local_E0[1] = 23665;
  local_E0[2] = 0x4000;
  task_32C = 0;
  task_324 = 0;
  task_328 = 0;
  Script_srchUtils(SRCH_SCRIPT_SetEntrySelectorFilter, 1);
  task_394[0] = 0;
  task_394[1] = 0;
  task_394[2] = 0;
  task_2F8 = 0;
  local_C8 = 0;
  for (;;) {
    task_338[local_C8] = 0;
    local_C8 = local_C8 + 1;
    if (local_C8 == 20) {
      break;
    }
  }
  task_38C = 0;
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 836)) {
    Script_charaDisplay(SHEN_NAME('MATI'), CDM_Hide);
  }
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_YDB1_BOOK_FOR_MASTER_CHEN)) {
    Script_charaDisplay(SHEN_NAME('BOOK'), CDM_Hide);
  }
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 844)) {
    Script_EVT_SetPosition(SHEN_NAME('IROU'), POS_XYZ, &data_F340);
  }
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 226)) {
    task_388 = 1;
  }
  Script_InvestigationUtils(ShenmueUtils_SetupOrSyncAnchors, SHEN_NAME('YKUL'), 0x52554B59);
  Script_InvestigationUtils(ShenmueUtils_SetSpecialAssetList, data_F1E4);
  script_10F7(1, 1, 0x4C554B59, 0x52554B59);
  script_1294();
  for (;;) {
    if (task_2E4 & -(task_250 == 0)) {
      Script_GetCameraInputDetails(15);
    }
    task_2D0 = -1;
    local_DC = Script_cassetteUtils();
    local_D8 = (local_DC & -0x10000) >> 16;
    local_D4 = local_DC & 0xFFFF;
    if ((local_D8 == 56) & (local_D4 == 2)) {
      if ((Script_srchUtils(SRCH_SCRIPT_UpdateAndPollSelection) == 7) & (task_2CC == 0)) {
        if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_YDB1_FLASHLIGHT_NEEDS_BULB)) {
        } else {
          script_AD92();
        }
      } else {
        task_3CC.x = 3.994f;
        task_3CC.y = 0.0f;
        task_3CC.z = 3.999f;
        if (script_60EF() == 1) {
          if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_YDB1_FLASHLIGHT_NEEDS_BULB)) {
          } else {
            script_AD92();
          }
        }
      }
    }
    if (task_250 == 0) {
      script_3B38();
    }
    if ((local_D8 == 54) & (local_D4 == 2)) {
      int32_t local_C0;
      local_C0 = Script_srchUtils(SRCH_SCRIPT_UpdateAndPollSelection);
      switch (local_C0) {
        case 0x31554F52:
          if (*task_304 == 0) {
            if (*task_304 != 2) {
              Script_ryoControl(PCCTL_DisablePlayerControl);
              script_B358(1);
              Script_ryoControl(RYO_RestoreControl);
              task_304[0] = 1;
              task_394[0] = 0;
              task_3A4 = 0;
            }
            goto L002BD2;
          }
          L002BD2:;
          break;
        case 0x32554F52:
          if (task_304[1] == 0) {
            if (task_304[1] != 2) {
              Script_ryoControl(PCCTL_DisablePlayerControl);
              script_B358(2);
              Script_ryoControl(RYO_RestoreControl);
              task_304[1] = 1;
              task_394[1] = 0;
              *(&task_3A4 + 1) = 0;
            }
            goto L002C33;
          }
          L002C33:;
          break;
        case 0x33554F52:
          if (task_304[2] == 0) {
            if (task_304[2] != 2) {
              Script_ryoControl(PCCTL_DisablePlayerControl);
              script_B358(3);
              Script_ryoControl(RYO_RestoreControl);
              task_304[2] = 1;
              task_394[2] = 0;
              *(&task_3A4 + 2) = 0;
            }
            goto L002C94;
          }
          L002C94:;
          break;
        case 0x34554F52:
          if (task_304[3] == 0) {
            if (task_304[3] != 2) {
              Script_ryoControl(PCCTL_DisablePlayerControl);
              script_B358(4);
              Script_ryoControl(RYO_RestoreControl);
              task_304[3] = 1;
              task_394[3] = 0;
              *(&task_3A4 + 3) = 0;
            }
            goto L002CF5;
          }
          L002CF5:;
          break;
      }
      if (Script_srchUtils(SRCH_SCRIPT_UpdateAndPollSelection) < 0) {
        Script_EVT_GetPosition(SHEN_NAME('ROU1'), -1, &task_3CC, 0);
        if (script_60EF() == 1) {
          if (*task_304 == 0) {
            Script_ryoControl(PCCTL_DisablePlayerControl);
            script_B358(1);
            Script_ryoControl(RYO_RestoreControl);
            task_304[0] = 1;
            task_394[0] = 0;
            task_3A4 = 0;
          }
        }
        Script_EVT_GetPosition(SHEN_NAME('ROU2'), -1, &task_3CC, 0);
        if (script_60EF() == 1) {
          if (task_304[1] == 0) {
            Script_ryoControl(PCCTL_DisablePlayerControl);
            script_B358(2);
            Script_ryoControl(RYO_RestoreControl);
            task_304[1] = 1;
            task_394[1] = 0;
            *(&task_3A4 + 1) = 0;
          }
        }
        Script_EVT_GetPosition(SHEN_NAME('ROU3'), -1, &task_3CC, 0);
        if (script_60EF() == 1) {
          if (task_304[2] == 0) {
            Script_ryoControl(PCCTL_DisablePlayerControl);
            script_B358(3);
            Script_ryoControl(RYO_RestoreControl);
            task_304[2] = 1;
            task_394[2] = 0;
            *(&task_3A4 + 2) = 0;
          }
        }
        Script_EVT_GetPosition(SHEN_NAME('ROU4'), -1, &task_3CC, 0);
        if (script_60EF() == 1) {
          if (task_304[3] == 0) {
            Script_ryoControl(PCCTL_DisablePlayerControl);
            script_B358(4);
            Script_ryoControl(RYO_RestoreControl);
            task_304[3] = 1;
            task_394[3] = 0;
            *(&task_3A4 + 3) = 0;
          }
        }
      }
    }
    if ((local_D8 == 55) & (local_D4 == 2)) {
      int32_t local_C0;
      local_C0 = Script_srchUtils(SRCH_SCRIPT_UpdateAndPollSelection);
      switch (local_C0) {
        case 0x31554F52:
          if (*task_304 == 1) {
            Script_ryoControl(PCCTL_DisablePlayerControl);
            script_B723(1);
            Script_ryoControl(RYO_RestoreControl);
            task_304[0] = 2;
            task_394[0] = 0;
            task_3A4 = 0;
          }
          break;
        case 0x32554F52:
          if (task_304[1] == 1) {
            Script_ryoControl(PCCTL_DisablePlayerControl);
            script_B723(2);
            Script_ryoControl(RYO_RestoreControl);
            task_304[1] = 2;
            task_394[1] = 0;
            *(&task_3A4 + 1) = 0;
          }
          break;
        case 0x33554F52:
          if (task_304[2] == 1) {
            Script_ryoControl(PCCTL_DisablePlayerControl);
            script_B723(3);
            Script_ryoControl(RYO_RestoreControl);
            task_304[2] = 2;
            task_394[2] = 0;
            *(&task_3A4 + 2) = 0;
          }
          break;
        case 0x34554F52:
          if (task_304[3] == 1) {
            Script_ryoControl(PCCTL_DisablePlayerControl);
            script_B723(4);
            Script_ryoControl(RYO_RestoreControl);
            task_304[3] = 2;
            task_394[3] = 0;
            *(&task_3A4 + 3) = 0;
          }
          break;
      }
      if (Script_srchUtils(SRCH_SCRIPT_UpdateAndPollSelection) < 0) {
        Script_EVT_GetPosition(SHEN_NAME('ROU1'), -1, &task_3CC, 0);
        if (script_60EF() == 1) {
          if ((local_D8 == 55) & (local_D4 == 2)) {
            if (*task_304 == 1) {
              Script_ryoControl(PCCTL_DisablePlayerControl);
              script_B723(1);
              Script_ryoControl(RYO_RestoreControl);
              task_304[0] = 2;
              task_394[0] = 0;
              task_3A4 = 0;
            }
          }
        }
        Script_EVT_GetPosition(SHEN_NAME('ROU2'), -1, &task_3CC, 0);
        if (script_60EF() == 1) {
          if ((local_D8 == 55) & (local_D4 == 2)) {
            if (task_304[1] == 1) {
              Script_ryoControl(PCCTL_DisablePlayerControl);
              script_B723(2);
              Script_ryoControl(RYO_RestoreControl);
              task_304[1] = 2;
              task_394[1] = 0;
              *(&task_3A4 + 1) = 0;
            }
          }
        }
        Script_EVT_GetPosition(SHEN_NAME('ROU3'), -1, &task_3CC, 0);
        if (script_60EF() == 1) {
          if ((local_D8 == 55) & (local_D4 == 2)) {
            if (task_304[2] == 1) {
              Script_ryoControl(PCCTL_DisablePlayerControl);
              script_B723(3);
              Script_ryoControl(RYO_RestoreControl);
              task_304[2] = 2;
              task_394[2] = 0;
              *(&task_3A4 + 2) = 0;
            }
          }
        }
        Script_EVT_GetPosition(SHEN_NAME('ROU4'), -1, &task_3CC, 0);
        if (script_60EF() == 1) {
          if ((local_D8 == 55) & (local_D4 == 2)) {
            if (task_304[3] == 1) {
              Script_ryoControl(PCCTL_DisablePlayerControl);
              script_B723(4);
              Script_ryoControl(RYO_RestoreControl);
              task_304[3] = 2;
              task_394[3] = 0;
              *(&task_3A4 + 3) = 0;
            }
          }
        }
      }
    }
    local_FC = script_13B4();
    if (local_FC == 0x5F4F4E4F) {
      if ((task_254 == 3) | ((task_254 == 2) & (task_2D4 == 1))) {
        Script_srchUtils(SRCH_SCRIPT_Reset);
        Script_lightSetSlotEnabled(28, 0);
        Script_SetGameStateTo(AUTH_None);
        Script_ryoControl(PCCTL_DisablePlayerControl);
        Script_motmToggleFlags(CHARA_AKIR, 2, 0x40);
        Script_motiSetAngFlags(CHARA_AKIR, 0);
        Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &data_F310);
        Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &data_F31C, 0);
        Script_ccowUtils(CHARA_AKIR, 2);
        Script_PlayCharaMotion(CHARA_AKIR, 4164, 70.0f, -1, -1, 1.5f, 0);
        if ((Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_TIP_WALL_DIFFERENT_COLOR) == 1) | Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_SPACE_BEHIND_WALL) | (task_320 == 1)) {
          Script_ShowUIMessageFromID("E1012A272");
        } else {
          Script_ShowUIMessageFromID("E1012A014");
        }
        for (;;) {
          task_258 = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_GetCharaMotionRemainingFramesScaled(CHARA_AKIR));
          if (task_258 == 120) {
            Script_executeAicaCommand(0x2505A9, 0, 0);
            Script_CharHandlers(CHARA_AKIR, 0, 0, 16);
            Script_fixoAttachEx(CHARA_ONO_, CHARA_AKIR, 18, &data_F2F8, &data_F304);
          }
          if (task_258 == 200) {
            break;
          }
          SCNF_WaitFrames_0000(1);
        }
        Script_motiSetAngFlags(CHARA_AKIR, 1);
        Script_motmToggleFlags(CHARA_AKIR, 1, 0x40);
        Script_fixoClearAttachment(CHARA_ONO_);
        Script_fixoAttachEx(CHARA_ONO_, CHARA_AKIR, 18, &data_F328, &data_F334);
        Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_MOVE_BACK_30, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        SCNF_WaitFrames_0000(30);
        Script_ccowUtils(CHARA_AKIR, 1);
        Script_ryoControl(RYO_RestoreControl);
        task_2CC = 1;
      } else {
        Script_srchUtils(SRCH_SCRIPT_Reset);
        Script_SetGameStateTo(AUTH_None);
        Script_ryoControl(PCCTL_DisablePlayerControl);
        Script_motmToggleFlags(CHARA_AKIR, 2, 0x40);
        Script_motiSetAngFlags(CHARA_AKIR, 0);
        Script_lightSetSlotEnabled(28, 0);
        Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &data_F310);
        Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &data_F31C, 0);
        Script_ccowUtils(CHARA_AKIR, 2);
        Script_PlayCharaMotion(CHARA_AKIR, 4163, 0.0f, -1, -1, 1.5f, 0);
        Script_ShowUIMessageFromID("E1012A271");
        for (;;) {
          task_258 = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_GetCharaMotionRemainingFramesScaled(CHARA_AKIR));
          if (task_258 == 47) {
            Script_executeAicaCommand(0x2505A9, 0, 0);
            Script_fixoAttachEx(CHARA_ONO_, CHARA_AKIR, 18, &data_F2F8, &data_F304);
          }
          if (task_258 == 148) {
            Script_executeAicaCommand(0x1105A9, 0, 0);
            Script_fixoClearAttachment(CHARA_ONO_);
          }
          if (task_258 == 180) {
            break;
          }
          SCNF_WaitFrames_0000(1);
        }
        Script_motiSetAngFlags(CHARA_AKIR, 1);
        Script_motmToggleFlags(CHARA_AKIR, 1, 0x40);
        Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_MOVE_BACK_30, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        SCNF_WaitFrames_0000(30);
        Script_ccowUtils(CHARA_AKIR, 1);
        Script_ryoControl(RYO_RestoreControl);
        Script_SetGameStateTo(AUTH_None);
      }
    } else {
      if (((task_254 == 3) & (task_2CC == 0)) | (-(task_2CC == 0) & -(task_254 == 2) & -(task_2D4 == 1))) {
        if (-(Script_GetControllerInput(1) == 0x400) & Script_IsPlayerAllowed()) {
          if (script_61A2() == 1) {
            SCNF_RequestIconNotification(0, 0, 0, 2);
            Script_srchUtils(SRCH_SCRIPT_SetExplicitTargetMapInfoId, 0x5F4F4E4F);
            local_FC = 0;
          }
        }
      }
    }
    if (-(Script_srchUtils(SRCH_SCRIPT_GetCurrentTargetEntryIndex) < 0) & -(local_D8 == 51) & -(local_D4 == 2)) {
      if (task_2E4 == 1) {
        task_2E4 = 0;
        script_D87D(0x52494B41, SHEN_NAME('LIGH'));
        Script_lightSetSlotEnabled(15, 0);
        script_279C(0);
        SCNF_WaitFrames_0000(1);
        Script_uiUtils(0, 0, 0);
        Script_GetBatteryLifePair(BATTERY_SET_DECREMENT_ENABLED, 0, 0);
      } else if (Script_GetBatteryLifePair(BATTERY_GET_PERCENT, 0) > 0) {
        task_2E4 = 1;
        Script_srchUtils(SRCH_SCRIPT_SetEntrySelectorFilter, 0);
        script_D789(CHARA_AKIR, SHEN_NAME('LIGH'));
        script_279C(1);
        Script_lightSetSlotEnabled(15, 1);
        Script_uiUtils(0, 26, 0);
        Script_CharHandlers(CHARA_AKIR, 1, 3, 16);
        Script_scenToggleCharaFlag(0, 100, 0);
        Script_GetBatteryLifePair(BATTERY_SET_DECREMENT_ENABLED, 0, 1);
        task_2F8 = 1;
      } else {
        int32_t local_C0;
        Script_ryoControl(PCCTL_DisablePlayerControl);
        local_C4 = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 1.0f));
        local_C0 = local_C4;
        switch (local_C0) {
          case 0:
            Script_ShowUIMessageFromID("E1012A288");
            break;
          case 1:
            Script_ShowUIMessageFromID("E1012A289");
            break;
        }
        while (Script_convGetIsActive(0)) {
          SCNF_WaitFrames_0000(1);
        }
        Script_ryoControl(RYO_RestoreControl);
      }
    }
    if ((Script_srchUtils(SRCH_SCRIPT_GetCurrentTargetEntryIndex) > 20) & (Script_srchUtils(SRCH_SCRIPT_GetCurrentTargetEntryIndex) < 24)) {
      if (task_394[Script_srchUtils(SRCH_SCRIPT_GetCurrentTargetEntryIndex) - 21] == 0) {
        for (;;) {
          if (Script_srchUtils(SRCH_SCRIPT_UpdateAndPollSelection)) {
            break;
          }
          SCNF_WaitFrames_0000(1);
        }
        Script_ryoControl(PCCTL_DisablePlayerControl);
        {
          int32_t local_C0;
          local_C0 = Script_srchUtils(SRCH_SCRIPT_UpdateAndPollSelection);
          switch (local_C0) {
            case 0x31554F52:
              task_394[0] = 1;
              local_C4 = *task_304;
              break;
            case 0x32554F52:
              task_394[1] = 1;
              local_C4 = task_304[1];
              break;
            case 0x33554F52:
              task_394[2] = 1;
              local_C4 = task_304[2];
              break;
            case 0x34554F52:
              task_394[3] = 1;
              local_C4 = task_304[3];
              break;
          }
        }
        {
          int32_t local_C0;
          local_C0 = Script_srchUtils(SRCH_SCRIPT_UpdateAndPollSelection);
          if ((local_C0 == 0x31554F52) | (local_C0 == 0x32554F52) | (local_C0 == 0x33554F52) | (local_C0 == 0x34554F52)) {
            local_C0 = local_C4;
            switch (local_C0) {
              case 0:
                local_C4 = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 1.0f));
                local_C0 = local_C4;
                switch (local_C0) {
                  case 0:
                    Script_ShowUIMessageFromID("E1012A112");
                    break;
                  case 1:
                    Script_ShowUIMessageFromID("E1012A198");
                    break;
                }
                while (Script_convGetIsActive(0)) {
                  SCNF_WaitFrames_0000(1);
                }
                break;
              case 1:
                local_C4 = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 1.0f));
                local_C0 = local_C4;
                switch (local_C0) {
                  case 0:
                    Script_ShowUIMessageFromID("E1012A135");
                    break;
                  case 1:
                    Script_ShowUIMessageFromID("E1012A066");
                    break;
                }
                while (Script_convGetIsActive(0)) {
                  SCNF_WaitFrames_0000(1);
                }
                break;
              case 2:
                local_C4 = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 1.0f));
                local_C0 = local_C4;
                switch (local_C0) {
                  case 0:
                    Script_ShowUIMessageFromID("E1012A190");
                    break;
                  case 1:
                    Script_ShowUIMessageFromID("E1012A192");
                    break;
                }
                while (Script_convGetIsActive(0)) {
                  SCNF_WaitFrames_0000(1);
                }
                break;
            }
            goto L0038D5;
          }
        }
        L0038D5:;
        Script_ryoControl(RYO_RestoreControl);
      }
    }
    (void)(task_2D4 == 0);
    task_318 = 0;
    if ((task_254 == 2) | ((task_254 == 3) & (task_250 == 0))) {
      if (local_FC == 0x594D4144) {
        if ((task_254 == 3) & (task_2CC == 0)) {
          script_4135(0);
        } else {
          Script_srchUtils(SRCH_SCRIPT_Reset);
          SCNF_WaitFrames_0000(1);
          task_318 = 1;
        }
      }
      if (-(Script_GetControllerInput(1) == 0x400) & -(task_2D4 == 1) & -(task_318 == 0)) {
        if ((-(task_254 == 2) | -(task_254 == 3)) & -(script_6225() == 1) & Script_IsPlayerAllowed()) {
          SCNF_RequestIconNotification(0, 0, 0, 2);
          Script_srchUtils(SRCH_SCRIPT_SetExplicitTargetMapInfoId, 0x594D4144);
          local_FC = 0;
        } else {
          if ((script_6225() == 1) & (Script_srchUtils(SRCH_SCRIPT_UpdateAndPollSelection) == -1)) {
            task_318 = 1;
            local_FC = 0;
          }
        }
      }
    }
    if (-(Script_GetControllerInput(1) == 0x400) & Script_IsPlayerAllowed()) {
      task_3CC.x = 0.0342f;
      task_3CC.y = 0.0f;
      task_3CC.z = 2.9097f;
      if (script_60EF() == 1) {
        if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_YDB1_FLASHLIGHT_NEEDS_BULB)) {
          script_CCC1();
        }
      }
    }
    if (-(local_FC == -1) & -(Script_GetControllerInput(1) == 0x400) & -(task_250 == 0) & -(Script_srchUtils(SRCH_SCRIPT_GetCurrentTargetEntryIndex) == -1) & Script_IsPlayerAllowed(0)) {
      int32_t local_C0;
      local_C0 = Script_mapEvUtils(0x4D450002, CHARA_AKIR);
      switch (local_C0) {
        case 0:
          script_4135(0);
          break;
        case 100:
          script_4135(100);
          break;
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_3A93(void) {
  for (;;) {
    Script_SetOwnedItemMenuFlagIfOwned(1, OwnedItem_UIITEMMATC, 0);
    Script_SetOwnedItemMenuFlagIfOwned(1, OwnedItem_UIITEMCAND, 0);
    Script_SetOwnedItemMenuFlagIfOwned(1, OwnedItem_UIITEMLIGH, 0);
    if ((Script_GetCurrentEntryPoint() == 3) | (Script_GetCurrentEntryPoint() == 5)) {
      if (Script_GetInputData(3, 1) == 0x400) {
        SAVE_Script_UpdateForSaveData(3, 0, 300);
      }
      if (Script_GetInputData(3, 1) == 0x200) {
        SAVE_Script_UpdateForSaveData(3, 0, 3000);
      }
      Script_PrintDebugString(3, 24, "FUKUSAN  frame = %d\012", SAVE_Script_UpdateForSaveData(4, 0));
      Script_PrintDebugString(3, 25, "KAITYUU nokori = %d\012", Script_GetBatteryLifePair(BATTERY_GET_PERCENT, 0));
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_3B38(void) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  local_FC = 0;
  local_F8 = 0;
  if (Script_GetBatteryLifePair(BATTERY_GET_PERCENT, 0) > 0) {
    Script_SetOwnedItemMenuFlagIfOwned(1, OwnedItem_UIITEMFLAS, 1);
  } else {
    Script_SetOwnedItemMenuFlagIfOwned(1, OwnedItem_UIITEMFLAS, 0);
  }
  if (Script_GetBatteryLifePair(BATTERY_GET_PERCENT, 0) < 13) {
    Script_SetOwnedItemMenuFlagIfOwned(1, OwnedItem_UIITEMCBAT, 1);
  }
  local_FC = Script_ScenDataUtil(&data_E93C, 4, Script_srchUtils(SRCH_SCRIPT_GetCurrentTargetEntryIndex) * 8 + 1);
  if (task_2CC == 0) {
    if (Script_srchUtils(SRCH_SCRIPT_GetCurrentTargetEntryIndex) >= 0) {
      local_F4 = local_FC;
      switch (local_F4) {
        case 0x31554F52:
          if (*task_304 == 1) {
            Script_SetOwnedItemMenuFlagIfOwned(1, OwnedItem_UIITEMMATC, 1);
          } else {
            if (*task_304 == 0) {
              Script_SetOwnedItemMenuFlagIfOwned(1, OwnedItem_UIITEMCAND, 1);
              goto L003C03;
            }
          }
          L003C03:;
          break;
        case 0x32554F52:
          if (task_304[1] == 1) {
            Script_SetOwnedItemMenuFlagIfOwned(1, OwnedItem_UIITEMMATC, 1);
          } else {
            if (task_304[1] == 0) {
              Script_SetOwnedItemMenuFlagIfOwned(1, OwnedItem_UIITEMCAND, 1);
              goto L003C48;
            }
          }
          L003C48:;
          break;
        case 0x33554F52:
          if (task_304[2] == 1) {
            Script_SetOwnedItemMenuFlagIfOwned(1, OwnedItem_UIITEMMATC, 1);
          } else {
            if (task_304[2] == 0) {
              Script_SetOwnedItemMenuFlagIfOwned(1, OwnedItem_UIITEMCAND, 1);
              goto L003C8D;
            }
          }
          L003C8D:;
          break;
        case 0x34554F52:
          if (task_304[3] == 1) {
            Script_SetOwnedItemMenuFlagIfOwned(1, OwnedItem_UIITEMMATC, 1);
          } else {
            if (task_304[3] == 0) {
              Script_SetOwnedItemMenuFlagIfOwned(1, OwnedItem_UIITEMCAND, 1);
              goto L003CD2;
            }
          }
          L003CD2:;
          break;
      }
    }
    Script_EVT_GetPosition(SHEN_NAME('ROU1'), -1, &task_3CC, 0);
    if ((script_60EF() == 1) & (task_420 == 0)) {
      if (*task_304 == 1) {
        local_FC = 0x31554F52;
        Script_SetOwnedItemMenuFlagIfOwned(1, OwnedItem_UIITEMMATC, 1);
      } else {
        if ((*task_304 == 0) & (task_420 == 0)) {
          local_FC = 0x31554F52;
          Script_SetOwnedItemMenuFlagIfOwned(1, OwnedItem_UIITEMCAND, 1);
          goto L003D42;
        }
      }
    }
    L003D42:;
    Script_EVT_GetPosition(SHEN_NAME('ROU2'), -1, &task_3CC, 0);
    if ((script_60EF() == 1) & (task_420 == 0)) {
      if (task_304[1] == 1) {
        local_FC = 0x32554F52;
        Script_SetOwnedItemMenuFlagIfOwned(1, OwnedItem_UIITEMMATC, 1);
      } else {
        if ((task_304[1] == 0) & (task_420 == 0)) {
          local_FC = 0x32554F52;
          Script_SetOwnedItemMenuFlagIfOwned(1, OwnedItem_UIITEMCAND, 1);
          goto L003DB5;
        }
      }
    }
    L003DB5:;
    Script_EVT_GetPosition(SHEN_NAME('ROU3'), -1, &task_3CC, 0);
    if ((script_60EF() == 1) & (task_420 == 0)) {
      if (task_304[2] == 1) {
        local_FC = 0x33554F52;
        Script_SetOwnedItemMenuFlagIfOwned(1, OwnedItem_UIITEMMATC, 1);
      } else {
        if ((task_304[2] == 0) & (task_420 == 0)) {
          local_FC = 0x33554F52;
          Script_SetOwnedItemMenuFlagIfOwned(1, OwnedItem_UIITEMCAND, 1);
          goto L003E28;
        }
      }
    }
    L003E28:;
    Script_EVT_GetPosition(SHEN_NAME('ROU4'), -1, &task_3CC, 0);
    if (script_60EF() == 1) {
      if ((task_304[3] == 1) & (task_420 == 0)) {
        local_FC = 0x34554F52;
        Script_SetOwnedItemMenuFlagIfOwned(1, OwnedItem_UIITEMMATC, 1);
      } else {
        if ((task_304[3] == 0) & (task_420 == 0)) {
          local_FC = 0x34554F52;
          Script_SetOwnedItemMenuFlagIfOwned(1, OwnedItem_UIITEMCAND, 1);
          goto L003E9B;
        }
      }
    }
    L003E9B:;
    task_3CC.x = 3.994f;
    task_3CC.y = 0.0f;
    task_3CC.z = 3.999f;
    if (script_60EF() == 1) {
      if (~Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_YDB1_FLASHLIGHT_NEEDS_BULB) & -(task_2CC == 0)) {
        Script_SetOwnedItemMenuFlagIfOwned(1, OwnedItem_UIITEMLIGH, 1);
      }
    }
  }
  if (Script_srchUtils(SRCH_SCRIPT_UpdateAndPollSelection) == 7) {
    if (~Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_YDB1_FLASHLIGHT_NEEDS_BULB) & -(task_2CC == 0)) {
      Script_SetOwnedItemMenuFlagIfOwned(1, OwnedItem_UIITEMLIGH, 1);
    }
  }
  if (Script_ScenDataUtil(&data_E93C, 4, Script_srchUtils(SRCH_SCRIPT_GetCurrentTargetEntryIndex) * 8 + 1) == 0x306B6E64) {
    if (~Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_YDB1_FLASHLIGHT_NEEDS_BULB)) {
      Script_SetOwnedItemMenuFlagIfOwned(1, OwnedItem_UIITEMLIGH, 1);
    }
  }
  if (task_2D4 == 1) {
    task_3CC.x = *(float *)&task_278;
    task_3CC.y = *(float *)(&task_278 + 1);
    task_3CC.z = *(float *)(&task_278 + 2);
    if (Script_ScenDataUtil(&data_E93C, 4, Script_srchUtils(SRCH_SCRIPT_GetCurrentTargetEntryIndex) * 8 + 1) == 0x594D4144) {
      if ((task_254 == 3) & (task_2CC == 1)) {
        local_F8 = 1;
      }
      if (task_254 == 2) {
        local_F8 = 1;
      }
    }
    if (script_6225() == 1) {
      if ((task_254 == 3) & (task_2CC == 1)) {
        local_F8 = 1;
      }
      if (task_254 == 2) {
        local_F8 = 1;
      }
    }
    if (task_2CC == 0) {
      if (script_61A2() == 1) {
        if ((task_254 == 3) | (task_254 == 2)) {
          local_F8 = 1;
        }
      }
      if (Script_ScenDataUtil(&data_E93C, 4, Script_srchUtils(SRCH_SCRIPT_GetCurrentTargetEntryIndex) * 8 + 1) == 0x5F4F4E4F) {
        if ((task_254 == 3) | (task_254 == 2)) {
          local_F8 = 1;
        }
      }
    }
  }
  if (-(task_2F0 == 0) & Script_IsPlayerAllowed(0)) {
    if ((task_3E4 == 0) & (local_F8 > 0)) {
      SCNF_RequestIconNotification(0x100, 0, 0, 1);
      task_3E4 = 1;
    }
    if (-(task_3E4 == 0) & (-(task_328 == 1) & (-(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_FLOOR_SCRATCHES) == 1) | -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_TIP_FLOOR_SCRATCHES) == 1)))) {
      SCNF_RequestIconNotification(0x100, 0, 0, 1);
      task_3E4 = 1;
    }
  } else {
    task_3E4 = 0;
    SCNF_RequestIconNotification(0, 0, 0, 2);
  }
  if ((local_F8 == 0) & (task_328 == 0)) {
    if (task_3E4 == 1) {
      task_3E4 = 0;
      SCNF_RequestIconNotification(0, 0, 0, 2);
    }
  }
  return;
}

__attribute__((scn_string_bindings(0, data_F3E4, 1, data_F3EE, 5, data_F416, 6, data_F420, 16, data_F484)))
static void script_4135(int32_t arg0) {
  Vector3i local_F4;
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  if (arg0 == 0) {
    if ((arg0 == 2) & (task_2CC == 1)) {
      int32_t local_F0;
      Script_ryoControl(PCCTL_DisablePlayerControl);
      Script_mtwkGetSetRotation(CHARA_AKIR, &local_F4, 0x42000000);
      Script_EVT_SetRotation(CHARA_AKIR, 0x78000000, &local_F4, 0);
      Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
      if (task_330 > 1) {
        task_330 = 0;
      }
      local_F0 = task_330;
      switch (local_F0) {
        case 0:
          Script_ShowUIMessageFromID("E1012A101");
          break;
        case 1:
          Script_ShowUIMessageFromID("E1012A103");
          break;
      }
      while (Script_convGetIsActive(0)) {
        SCNF_WaitFrames_0000(1);
      }
      Script_ryoControl(RYO_RestoreControl);
      task_330 = task_330 + 1;
      goto L004657;
    }
    {
      int32_t local_F0;
      local_F0 = task_254;
      if (local_F0 == 3) {
        if (task_2CC == 0) {
          Script_ryoControl(PCCTL_DisablePlayerControl);
          Script_mtwkGetSetRotation(CHARA_AKIR, &local_F4, 0x42000000);
          Script_EVT_SetRotation(CHARA_AKIR, 0x78000000, &local_F4, 0);
          Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
          if (task_330 > 2) {
            task_330 = 0;
          }
          local_F0 = task_330;
          switch (local_F0) {
            case 0:
              Script_ShowUIMessageFromID("E1012A167");
              break;
            case 1:
              Script_ShowUIMessageFromID("E1012A168");
              break;
            case 2:
              Script_ShowUIMessageFromID("E1012A169");
              break;
          }
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
          Script_ryoControl(RYO_RestoreControl);
        } else {
          Script_ryoControl(PCCTL_DisablePlayerControl);
          Script_mtwkGetSetRotation(CHARA_AKIR, &local_F4, 0x42000000);
          Script_EVT_SetRotation(CHARA_AKIR, 0x78000000, &local_F4, 0);
          Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
          if (task_330 > 1) {
            task_330 = 0;
          }
          local_F0 = task_330;
          switch (local_F0) {
            case 0:
              Script_ShowUIMessageFromID("E1012A101");
              break;
            case 1:
              Script_ShowUIMessageFromID("E1012A103");
              break;
          }
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
          Script_ryoControl(RYO_RestoreControl);
        }
        task_330 = task_330 + 1;
      } else if (local_F0 == 12) {
      } else if (local_F0 == 6) {
      } else {
        Script_ryoControl(PCCTL_DisablePlayerControl);
        Script_mtwkGetSetRotation(CHARA_AKIR, &local_F4, 0x42000000);
        Script_EVT_SetRotation(CHARA_AKIR, 0x78000000, &local_F4, 0);
        Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        if (task_330 > 13) {
          task_330 = 0;
        }
        local_F0 = task_330;
        switch (local_F0) {
          case 0:
            Script_ShowUIMessageFromID("E1012A092");
            break;
          case 1:
            Script_ShowUIMessageFromID("E1012A093");
            break;
          case 2:
            Script_ShowUIMessageFromID("E1012A094");
            break;
          case 3:
            Script_ShowUIMessageFromID("E1012A095");
            break;
          case 4:
            Script_ShowUIMessageFromID("E1012A096");
            break;
          case 5:
            Script_ShowUIMessageFromID("E1012A097");
            break;
          case 6:
            Script_ShowUIMessageFromID("E1012A098");
            break;
          case 7:
            Script_ShowUIMessageFromID("E1012A099");
            break;
          case 8:
            Script_ShowUIMessageFromID("E1012A100");
            break;
          case 10:
            Script_ShowUIMessageFromID("E1012A103");
            break;
          case 11:
            Script_ShowUIMessageFromID("E1012A104");
            break;
          case 12:
            Script_ShowUIMessageFromID("E1012A105");
            break;
          case 13:
            Script_ShowUIMessageFromID("E1012A106");
            break;
          case 14:
            Script_ShowUIMessageFromID("E1012A107");
            break;
        }
        while (Script_convGetIsActive(0)) {
          SCNF_WaitFrames_0000(1);
        }
        Script_ryoControl(RYO_RestoreControl);
        task_330 = task_330 + 1;
      }
    }
  } else {
    int32_t local_F0;
    local_F0 = task_254;
    if (local_F0 == 12) {
    } else if (local_F0 == 6) {
    } else {
      Script_ryoControl(PCCTL_DisablePlayerControl);
      Script_mtwkGetSetRotation(CHARA_AKIR, &local_F4, 0x42000000);
      Script_EVT_SetRotation(CHARA_AKIR, 0x78000000, &local_F4, 0);
      Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
      if (task_330 > 2) {
        task_330 = 0;
      }
      local_F0 = task_330;
      switch (local_F0) {
        case 0:
          Script_ShowUIMessageFromID("E1012A085");
          break;
        case 1:
          Script_ShowUIMessageFromID("E1012A084");
          break;
        case 2:
          Script_ShowUIMessageFromID("E1012A086");
          break;
      }
      task_330 = task_330 + 1;
      while (Script_convGetIsActive(0)) {
        SCNF_WaitFrames_0000(1);
      }
      Script_ryoControl(RYO_RestoreControl);
    }
  }
  L004657:;
  return;
}

__attribute__((scn_string_bindings(0, data_F504, 1, data_F50E, 2, data_F518, 12, data_F57C, 13, data_F586, 14, data_F590, 15, data_F59A, 20, data_F5CC, 21, data_F5D6, 22, data_F5E0, 23, data_F5EA, 24, data_F5F4, 25, data_F5FE, 26, data_F608, 27, data_F612, 28, data_F61C, 29, data_F626, 30, data_F630, 31, data_F63A, 32, data_F644, 33, data_F64E, 34, data_F658, 35, data_F662, 36, data_F66C, 37, data_F676, 38, data_F680, 39, data_F68A, 40, data_F694, 41, data_F69E, 42, data_F6A8, 43, data_F6B2, 44, data_F6BC, 45, data_F6C6, 46, data_F6D0, 47, data_F6DA, 48, data_F6E4, 49, data_F6EE, 50, data_F6F8, 51, data_F702, 52, data_F70C, 53, data_F716, 54, data_F720, 55, data_F72A, 56, data_F734, 57, data_F73E, 58, data_F748, 59, data_F752, 60, data_F75C, 61, data_F766, 62, data_F770, 63, data_F77A, 64, data_F784, 65, data_F78E, 66, data_F798, 67, data_F7A2, 68, data_F7AC, 69, data_F7B6, 70, data_F7C0, 71, data_F7CA, 72, data_F7D4, 73, data_F7DE, 74, data_F7E8, 75, data_F7F2, 76, data_F7FC, 85, data_F856, 86, data_F860, 87, data_F86A, 95, data_F8BA, 107, data_F932)))
static void script_4658(int32_t arg0) {
  int32_t unused_FC;
  Vector3 local_F0;
  Vector3i local_E4;
  int32_t local_E0;
  if (arg0 < 20) {
    if (task_338[arg0] == 1) {
      int32_t unused_D4[3];
      int32_t local_D0;
      local_E0 = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f));
      Script_ryoControl(PCCTL_DisablePlayerControl);
      local_D0 = local_E0;
      switch (local_D0) {
        case 0:
          Script_ShowUIMessageFromID("E1012A187");
          break;
        case 1:
          Script_ShowUIMessageFromID("E1012A188");
          break;
        case 2:
          Script_ShowUIMessageFromID("E1012A189");
          break;
      }
      while (Script_convGetIsActive(0)) {
        SCNF_WaitFrames_0000(1);
      }
      Script_ryoControl(RYO_RestoreControl);
    } else {
      int32_t unused_D4[3];
      int32_t local_D0;
      if (arg0 != 8) {
        if (task_448 == 8) {
          task_338[arg0] = 1;
        }
      }
      local_D0 = arg0;
      switch (local_D0) {
        case 3:
          if (task_448 == 8) {
            Script_ryoControl(PCCTL_DisablePlayerControl);
            while (Script_convGetIsActive(0)) {
              SCNF_WaitFrames_0000(1);
            }
            Script_ShowUIMessageFromID("E1012A019");
            while (Script_convGetIsActive(0)) {
              SCNF_WaitFrames_0000(1);
            }
            Script_ryoControl(RYO_RestoreControl);
          }
          break;
        case 4:
          if (task_448 == 8) {
            Script_ryoControl(PCCTL_DisablePlayerControl);
            Script_ShowUIMessageFromID("E1012A057");
            while (Script_convGetIsActive(0)) {
              SCNF_WaitFrames_0000(1);
            }
            Script_ShowUIMessageFromID("E1012A058");
            while (Script_convGetIsActive(0)) {
              SCNF_WaitFrames_0000(1);
            }
            Script_ryoControl(RYO_RestoreControl);
          }
          break;
        case 5:
          if (task_448 == 8) {
            Script_CharHandlers(CHARA_AKIR, 0, 0, 16);
            Script_ryoControl(PCCTL_DisablePlayerControl);
            Script_motmToggleFlags(CHARA_AKIR, 1, 0x80);
            Script_motmToggleFlags(CHARA_AKIR, 2, 0x40);
            Script_motiSetAngFlags(CHARA_AKIR, 0);
            Script_PlayCharaMotion(CHARA_AKIR, 4126, -1.0f, -1.0f, -1.0f, 1.0f, 0);
            for (;;) {
              if (Script_IsCharaMotionFinished(CHARA_AKIR)) {
                break;
              }
              Script_RotateNodeUtils(CHARA_AKIR, &data_E860, 0);
              SCNF_WaitFrames_0000(1);
            }
            Script_motiSetAngFlags(CHARA_AKIR, 1);
            Script_motmToggleFlags(CHARA_AKIR, 1, 0x40);
            Script_executeAicaCommand(198057, 0, 0);
            Script_motmToggleFlags(CHARA_AKIR, 1, 0x80);
            Script_PlayCharaMotion(CHARA_AKIR, 4125, -1.0f, -1.0f, -1.0f, 1.0f, 0);
            for (;;) {
              if (Script_IsCharaMotionFinished(CHARA_AKIR)) {
                break;
              }
              Script_RotateNodeUtils(CHARA_AKIR, &data_E860, 0);
              SCNF_WaitFrames_0000(1);
            }
            Script_ryoControl(RYO_RestoreControl);
            Script_CharHandlers(CHARA_AKIR, 0, 13, 16);
          }
          break;
        case 6:
          Script_CharHandlers(CHARA_AKIR, 0, 0, 16);
          Script_ryoControl(PCCTL_DisablePlayerControl);
          Script_motmToggleFlags(CHARA_AKIR, 1, 0x80);
          Script_motmToggleFlags(CHARA_AKIR, 2, 0x40);
          Script_motiSetAngFlags(CHARA_AKIR, 0);
          Script_PlayCharaMotion(CHARA_AKIR, 4126, -1.0f, -1.0f, -1.0f, 1.0f, 0);
          for (;;) {
            if (Script_IsCharaMotionFinished(CHARA_AKIR)) {
              break;
            }
            Script_RotateNodeUtils(CHARA_AKIR, &data_E860, 0);
            SCNF_WaitFrames_0000(1);
          }
          Script_motiSetAngFlags(CHARA_AKIR, 1);
          Script_motmToggleFlags(CHARA_AKIR, 1, 0x40);
          Script_executeAicaCommand(198057, 0, 0);
          Script_motmToggleFlags(CHARA_AKIR, 1, 0x80);
          Script_PlayCharaMotion(CHARA_AKIR, 4125, -1.0f, -1.0f, -1.0f, 1.0f, 0);
          for (;;) {
            if (Script_IsCharaMotionFinished(CHARA_AKIR)) {
              break;
            }
            Script_RotateNodeUtils(CHARA_AKIR, &data_E860, 0);
            SCNF_WaitFrames_0000(1);
          }
          Script_ryoControl(RYO_RestoreControl);
          Script_CharHandlers(CHARA_AKIR, 0, 13, 16);
          break;
        case 7:
          if (task_448 == 8) {
            if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_YDB1_FLASHLIGHT_NEEDS_BULB) == 0) {
              Script_ryoControl(PCCTL_DisablePlayerControl);
              Script_ShowUIMessageFromID("E1012A236");
              while (Script_convGetIsActive(0)) {
                SCNF_WaitFrames_0000(1);
              }
              Script_ryoControl(RYO_RestoreControl);
            }
          }
          break;
        case 8:
          if (task_448 == 8) {
            if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_YDB1_FLASHLIGHT_NEEDS_BULB)) {
              local_F0.x = 0.6872f;
              (&local_F0)->y = 0.0f;
              (&local_F0)->z = 3.3065f;
              local_E4.x = 0;
              (&local_E4)->y = 0x4000;
              (&local_E4)->z = 0;
              Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &local_F0);
              Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &local_E4, 0);
              script_CCC1();
            } else {
              Script_ryoControl(PCCTL_DisablePlayerControl);
              local_D0 = *(&task_3A4 + 5);
              switch (local_D0) {
                case 0:
                  Script_ShowUIMessageFromID("E1012A256");
                  *(&task_3A4 + 5) = 1;
                  break;
                case 1:
                  Script_ShowUIMessageFromID("E1012A257");
                  *(&task_3A4 + 5) = 0;
                  break;
              }
              while (Script_convGetIsActive(0)) {
                SCNF_WaitFrames_0000(1);
              }
              Script_ryoControl(RYO_RestoreControl);
            }
          }
          break;
        case 12:
          if (task_448 == 8) {
            Script_ryoControl(PCCTL_DisablePlayerControl);
            Script_ShowUIMessageFromID("E1012A061");
            while (Script_convGetIsActive(0)) {
              SCNF_WaitFrames_0000(1);
            }
            Script_ShowUIMessageFromID("E1012A062");
            while (Script_convGetIsActive(0)) {
              SCNF_WaitFrames_0000(1);
            }
            Script_ShowUIMessageFromID("E1012A110");
            while (Script_convGetIsActive(0)) {
              SCNF_WaitFrames_0000(1);
            }
            Script_ryoControl(RYO_RestoreControl);
          }
          break;
        case 14:
          if (task_448 == 8) {
            local_E0 = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f));
            Script_ryoControl(PCCTL_DisablePlayerControl);
            local_D0 = local_E0;
            switch (local_D0) {
              case 0:
                Script_ShowUIMessageFromID("E1012A187");
                break;
              case 1:
                Script_ShowUIMessageFromID("E1012A188");
                break;
              case 2:
                Script_ShowUIMessageFromID("E1012A189");
                break;
            }
            while (Script_convGetIsActive(0)) {
              SCNF_WaitFrames_0000(1);
            }
            Script_ryoControl(RYO_RestoreControl);
          }
          break;
        case 13:
          if ((task_448 == 8) & (task_2D4 == 0)) {
            if (task_334 == 0) {
              Script_ryoControl(PCCTL_DisablePlayerControl);
              local_F0.x = 6.8943f;
              (&local_F0)->y = 0.0f;
              (&local_F0)->z = 1.2971f;
              Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &local_F0);
              local_E4.x = 0;
              (&local_E4)->y = 47695;
              (&local_E4)->z = 0;
              Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &local_E4, 0);
              Script_srchUtils(SRCH_SCRIPT_Reset);
              Script_ShowUIMessageFromID("E1012A040");
              Script_seqEnterEvtCam(4000, 0, AUTH_None);
              while (Script_convGetIsActive(0)) {
                SCNF_WaitFrames_0000(1);
              }
              Script_ShowUIMessageFromID("E1012A182");
              while (Script_convGetIsActive(0)) {
                SCNF_WaitFrames_0000(1);
              }
              Script_EVT_GetPosition(SHEN_NAME('FUDA'), -1, &local_F0, 0);
              Script_SetLookPointDetails(CHARA_AKIR, 12, &local_F0, 1);
              Script_seqEnterEvtCam(4001, 0, AUTH_None);
              Script_ShowUIMessageFromID("E1012A183");
              while (Script_convGetIsActive(0)) {
                SCNF_WaitFrames_0000(1);
              }
              Script_lightSetSlotEnabled(28, 0);
              SCNF_WaitFrames_0000(15);
              Script_SetCameraFieldOfView(10922);
              Script_ryoControl(RYO_RestoreControl);
              Script_SetGameStateTo(AUTH_None);
              Script_SetCharaLookAtPosition(CHARA_AKIR, -13, 0, 0);
              task_334 = 1;
            } else {
              Script_ryoControl(PCCTL_DisablePlayerControl);
              Script_srchUtils(SRCH_SCRIPT_Reset);
              Script_seqEnterEvtCam(4000, 0, AUTH_None);
              if (task_334 == 1) {
                Script_ShowUIMessageFromID("E1012A184");
                task_334 = 2;
              } else {
                Script_ShowUIMessageFromID("E1012A185");
                task_334 = 1;
              }
              while (Script_convGetIsActive(0)) {
                SCNF_WaitFrames_0000(1);
              }
              SCNF_WaitFrames_0000(150);
              Script_ryoControl(RYO_RestoreControl);
              Script_SetCameraFieldOfView(10922);
              Script_SetGameStateTo(AUTH_None);
            }
          }
          break;
      }
    }
  } else {
    int32_t unused_D4[3];
    int32_t local_D0;
    local_D0 = arg0;
    switch (local_D0) {
      case 0x4F4B4148:
        if (task_448 == 8) {
          if ((Script_GetOwnedItemCount(8, 0) == 0) & (task_2CC == 0)) {
            Vector3i local_C4;
            script_9422();
            if (task_428 == 1) {
              Script_lightSetSlotEnabled(28, 0);
            }
            Script_lightSetSlotEnabled(15, 0);
            Script_srchUtils(SRCH_SCRIPT_Reset);
            Script_SetGameStateTo(AUTH_None);
            Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &data_F4D4);
            (&local_C4)->y = 0x8000;
            Script_EVT_SetRotation(CHARA_AKIR, MATRIX_XFORM_Y, &local_C4, 0);
            Script_seqEnterEvtCam(2100, 0, AUTH_None);
            script_B019(SHEN_NAME('FUTA'));
            script_B21F();
            Script_GetCassetteFlags(8, 1, 0, 0);
            SCNF_StartIconSystemMenu(8);
            Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_OLD_PHOTO_OF_DAD, 1);
            Script_charaDisplay(SHEN_NAME('SHAS'), CDM_Hide);
            Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &data_F4E0);
            Script_fixoClearAttachment(SHEN_NAME('SHAS'));
            Script_seqEnterEvtCam(2101, 0, AUTH_None);
            script_B1AD(arg0);
            script_94B2();
            local_F0.x = 6.07f;
            (&local_F0)->y = 0.877f;
            (&local_F0)->z = 8.1409f;
            local_E4.x = 0;
            (&local_E4)->y = 1195;
            (&local_E4)->z = 0;
            Script_EVT_SetRotation(SHEN_NAME('FUTA'), 0x38000000, &local_E4, 0);
            Script_EVT_SetPosition(SHEN_NAME('FUTA'), POS_XYZ, &local_F0);
            Script_charaDisplay(CHARA_ONO_, CDM_Show);
          } else {
            Script_ryoControl(PCCTL_DisablePlayerControl);
            local_E0 = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f));
            local_D0 = local_E0;
            switch (local_D0) {
              case 0:
                Script_ShowUIMessageFromID("E1012A187");
                break;
              case 1:
                Script_ShowUIMessageFromID("E1012A188");
                break;
              case 2:
                Script_ShowUIMessageFromID("E1012A189");
                break;
            }
            while (Script_convGetIsActive(0)) {
              SCNF_WaitFrames_0000(1);
            }
            Script_ryoControl(RYO_RestoreControl);
          }
        }
        break;
      case 0x31554F52:
        if (task_448 == 8) {
          Script_ryoControl(PCCTL_DisablePlayerControl);
          local_D0 = *task_304;
          switch (local_D0) {
            case 0:
              local_E0 = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 1.0f));
              local_D0 = *&task_3A4;
              switch (local_D0) {
                case 0:
                  Script_ShowUIMessageFromID("E1012A194");
                  task_3A4 = 1;
                  break;
                case 1:
                  Script_ShowUIMessageFromID("E1012A198");
                  task_3A4 = 2;
                  break;
                case 2:
                  Script_ShowUIMessageFromID("E1012A187");
                  task_3A4 = 3;
                  break;
                case 3:
                  Script_ShowUIMessageFromID("E1012A188");
                  task_3A4 = 4;
                  break;
                case 4:
                  Script_ShowUIMessageFromID("E1012A189");
                  task_3A4 = 2;
                  break;
              }
              break;
            case 1:
              local_D0 = *&task_3A4;
              switch (local_D0) {
                case 0:
                  Script_ShowUIMessageFromID("E1012A195");
                  task_3A4 = 1;
                  break;
                case 1:
                  Script_ShowUIMessageFromID("E1012A187");
                  task_3A4 = 2;
                  break;
                case 2:
                  Script_ShowUIMessageFromID("E1012A188");
                  task_3A4 = 3;
                  break;
                case 3:
                  Script_ShowUIMessageFromID("E1012A189");
                  task_3A4 = 1;
                  break;
              }
              break;
            case 2:
              if ((*(int16_t *)task_2FC < 0x4B00) & (*(int16_t *)task_2FC > 9600)) {
                Script_ShowUIMessageFromID("E1012A197");
              }
              if ((*(int16_t *)task_2FC < 9600) & (*(int16_t *)task_2FC > 4800)) {
                Script_ShowUIMessageFromID("E1012A193");
              }
              if (*(int16_t *)task_2FC < 4800) {
                Script_ShowUIMessageFromID("E1012A191");
              }
              break;
          }
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
          Script_ryoControl(RYO_RestoreControl);
        }
        break;
      case 0x32554F52:
        if (task_448 == 8) {
          Script_ryoControl(PCCTL_DisablePlayerControl);
          local_D0 = *task_304;
          switch (local_D0) {
            case 0:
              local_D0 = *(&task_3A4 + 1);
              switch (local_D0) {
                case 0:
                  Script_ShowUIMessageFromID("E1012A194");
                  *(&task_3A4 + 1) = 1;
                  break;
                case 1:
                  Script_ShowUIMessageFromID("E1012A198");
                  *(&task_3A4 + 1) = 2;
                  break;
                case 2:
                  Script_ShowUIMessageFromID("E1012A187");
                  *(&task_3A4 + 1) = 3;
                  break;
                case 3:
                  Script_ShowUIMessageFromID("E1012A188");
                  *(&task_3A4 + 1) = 4;
                  break;
                case 4:
                  Script_ShowUIMessageFromID("E1012A189");
                  *(&task_3A4 + 1) = 2;
                  break;
              }
              break;
            case 1:
              local_D0 = *(&task_3A4 + 1);
              switch (local_D0) {
                case 0:
                  Script_ShowUIMessageFromID("E1012A195");
                  *(&task_3A4 + 1) = 1;
                  break;
                case 1:
                  Script_ShowUIMessageFromID("E1012A187");
                  *(&task_3A4 + 1) = 2;
                  break;
                case 2:
                  Script_ShowUIMessageFromID("E1012A188");
                  *(&task_3A4 + 1) = 3;
                  break;
                case 3:
                  Script_ShowUIMessageFromID("E1012A189");
                  *(&task_3A4 + 1) = 1;
                  break;
              }
              break;
            case 2:
              if ((*(int16_t *)((int8_t *)task_2FC + 2) < 0x4B00) & (*(int16_t *)((int8_t *)task_2FC + 2) > 9600)) {
                Script_ShowUIMessageFromID("E1012A197");
              }
              if ((*(int16_t *)((int8_t *)task_2FC + 2) < 9600) & (*(int16_t *)((int8_t *)task_2FC + 2) > 4800)) {
                Script_ShowUIMessageFromID("E1012A193");
              }
              if (*(int16_t *)((int8_t *)task_2FC + 2) < 4800) {
                Script_ShowUIMessageFromID("E1012A191");
              }
              break;
          }
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
          Script_ryoControl(RYO_RestoreControl);
        }
        break;
      case 0x33554F52:
        if (task_448 == 8) {
          Script_ryoControl(PCCTL_DisablePlayerControl);
          local_D0 = task_304[2];
          switch (local_D0) {
            case 0:
              local_D0 = *(&task_3A4 + 2);
              switch (local_D0) {
                case 0:
                  Script_ShowUIMessageFromID("E1012A194");
                  *(&task_3A4 + 2) = 1;
                  break;
                case 1:
                  Script_ShowUIMessageFromID("E1012A198");
                  *(&task_3A4 + 2) = 2;
                  break;
                case 2:
                  Script_ShowUIMessageFromID("E1012A187");
                  *(&task_3A4 + 2) = 3;
                  break;
                case 3:
                  Script_ShowUIMessageFromID("E1012A188");
                  *(&task_3A4 + 2) = 4;
                  break;
                case 4:
                  Script_ShowUIMessageFromID("E1012A189");
                  *(&task_3A4 + 2) = 2;
                  break;
              }
              break;
            case 1:
              local_D0 = *(&task_3A4 + 2);
              switch (local_D0) {
                case 0:
                  Script_ShowUIMessageFromID("E1012A195");
                  *(&task_3A4 + 2) = 1;
                  break;
                case 1:
                  Script_ShowUIMessageFromID("E1012A187");
                  *(&task_3A4 + 2) = 2;
                  break;
                case 2:
                  Script_ShowUIMessageFromID("E1012A188");
                  *(&task_3A4 + 2) = 3;
                  break;
                case 3:
                  Script_ShowUIMessageFromID("E1012A189");
                  *(&task_3A4 + 2) = 1;
                  break;
              }
              break;
            case 2:
              if ((*(int16_t *)((int8_t *)task_2FC + 4) < 0x4B00) & (*(int16_t *)((int8_t *)task_2FC + 4) > 9600)) {
                Script_ShowUIMessageFromID("E1012A197");
              }
              if ((*(int16_t *)((int8_t *)task_2FC + 4) < 9600) & (*(int16_t *)((int8_t *)task_2FC + 4) > 4800)) {
                Script_ShowUIMessageFromID("E1012A193");
              }
              if (*(int16_t *)((int8_t *)task_2FC + 4) < 4800) {
                Script_ShowUIMessageFromID("E1012A191");
              }
              break;
          }
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
          Script_ryoControl(RYO_RestoreControl);
        }
        break;
      case 0x34554F52:
        if (task_448 == 8) {
          Script_ryoControl(PCCTL_DisablePlayerControl);
          local_D0 = task_304[3];
          switch (local_D0) {
            case 0:
              local_D0 = *(&task_3A4 + 3);
              switch (local_D0) {
                case 0:
                  Script_ShowUIMessageFromID("E1012A194");
                  *(&task_3A4 + 3) = 1;
                  break;
                case 1:
                  Script_ShowUIMessageFromID("E1012A198");
                  *(&task_3A4 + 3) = 2;
                  break;
                case 2:
                  Script_ShowUIMessageFromID("E1012A187");
                  *(&task_3A4 + 3) = 3;
                  break;
                case 3:
                  Script_ShowUIMessageFromID("E1012A188");
                  *(&task_3A4 + 3) = 4;
                  break;
                case 4:
                  Script_ShowUIMessageFromID("E1012A189");
                  *(&task_3A4 + 3) = 2;
                  break;
              }
              break;
            case 1:
              local_D0 = *(&task_3A4 + 3);
              switch (local_D0) {
                case 0:
                  Script_ShowUIMessageFromID("E1012A195");
                  *(&task_3A4 + 3) = 1;
                  break;
                case 1:
                  Script_ShowUIMessageFromID("E1012A187");
                  *(&task_3A4 + 3) = 2;
                  break;
                case 2:
                  Script_ShowUIMessageFromID("E1012A188");
                  *(&task_3A4 + 3) = 3;
                  break;
                case 3:
                  Script_ShowUIMessageFromID("E1012A189");
                  *(&task_3A4 + 3) = 1;
                  break;
              }
              break;
            case 2:
              if ((*(int16_t *)((int8_t *)task_2FC + 6) < 0x4B00) & (*(int16_t *)((int8_t *)task_2FC + 6) > 9600)) {
                Script_ShowUIMessageFromID("E1012A197");
              }
              if ((*(int16_t *)((int8_t *)task_2FC + 6) < 9600) & (*(int16_t *)((int8_t *)task_2FC + 6) > 4800)) {
                Script_ShowUIMessageFromID("E1012A193");
              }
              if (*(int16_t *)((int8_t *)task_2FC + 6) < 4800) {
                Script_ShowUIMessageFromID("E1012A191");
              }
              break;
          }
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
          Script_ryoControl(RYO_RestoreControl);
        }
        break;
      case 0x42345842:
        if (task_448 == 8) {
          if ((task_38C == 0) & (task_2CC == 0)) {
            if (task_428 == 1) {
              Script_lightSetSlotEnabled(28, 0);
            }
            Script_srchUtils(SRCH_SCRIPT_Reset);
            Script_SetGameStateTo(AUTH_None);
            script_C0FC();
            task_38C = 1;
          } else {
            Script_ryoControl(PCCTL_DisablePlayerControl);
            local_E0 = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f));
            local_D0 = local_E0;
            switch (local_D0) {
              case 0:
                Script_ShowUIMessageFromID("E1012A187");
                break;
              case 1:
                Script_ShowUIMessageFromID("E1012A188");
                break;
              case 2:
                Script_ShowUIMessageFromID("E1012A189");
                break;
            }
            while (Script_convGetIsActive(0)) {
              SCNF_WaitFrames_0000(1);
            }
            Script_ryoControl(RYO_RestoreControl);
          }
        }
        break;
      case 0x574E4544:
        if (task_448 == 8) {
          if ((task_388 == 0) & (task_2CC == 0)) {
            script_BC3F();
            task_388 = 1;
          } else {
            Script_ryoControl(PCCTL_DisablePlayerControl);
            local_E0 = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f));
            local_D0 = local_E0;
            switch (local_D0) {
              case 0:
                Script_ShowUIMessageFromID("E1012A187");
                break;
              case 1:
                Script_ShowUIMessageFromID("E1012A188");
                break;
              case 2:
                Script_ShowUIMessageFromID("E1012A189");
                break;
            }
            while (Script_convGetIsActive(0)) {
              SCNF_WaitFrames_0000(1);
            }
            Script_ryoControl(RYO_RestoreControl);
          }
        }
        break;
      case 0x414B4153:
        if (task_448 == 1) {
          Script_ShowUIMessageFromID("E1012A138");
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
          Script_ShowUIMessageFromID("E1012A139");
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
        }
        if (task_448 == 3) {
          Script_ryoControl(PCCTL_DisablePlayerControl);
          Script_ShowUIMessageFromID("E1012A140");
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
          Script_ryoControl(RYO_RestoreControl);
        }
        break;
      case 0x53494E4B:
        if (task_448 == 1) {
          Script_ShowUIMessageFromID("E1012A131");
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
        }
        break;
      case 0x59454B48:
        if (task_448 == 1) {
          Script_ShowUIMessageFromID("E1012A137");
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
        }
        break;
      case 0x454E414B:
        if (task_448 == 1) {
          Script_ShowUIMessageFromID("E1012A141");
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
        }
        if (task_448 == 2) {
          Script_ryoControl(PCCTL_DisablePlayerControl);
          Script_ShowUIMessageFromID("E1012A142");
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
          Script_ryoControl(RYO_RestoreControl);
        }
        break;
      case 0x414B554D:
        if (((task_448 == 8) & (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_TIP_FLOOR_SCRATCHES) == 1)) | (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_FLOOR_SCRATCHES) == 1)) {
          if (Script_GetControllerInput(1) == 0x400) {
            if (task_428 == 1) {
              Script_lightSetSlotEnabled(28, 0);
            }
            SCNF_RequestIconNotification(0, 0, 0, 2);
            task_2D4 = 1;
            Script_srchUtils(SRCH_SCRIPT_Reset);
            Script_SetGameStateTo(AUTH_None);
            script_C858();
          }
        }
        break;
      case 0x41524954:
        if (task_448 == 8) {
          Script_ryoControl(PCCTL_DisablePlayerControl);
          Script_ShowUIMessageFromID("E1012A268");
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
          Script_ShowUIMessageFromID("E1012A295");
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
          Script_ryoControl(RYO_RestoreControl);
        }
        break;
      case 0x32425554:
        if (task_448 == 1) {
          Script_ShowUIMessageFromID("E1012A144");
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
        }
        break;
      case 0x33425554:
        if (task_448 == 1) {
          Script_ShowUIMessageFromID("E1012A144");
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
        }
        break;
      case 0x494B414D:
        if (task_448 == 2) {
          Script_ryoControl(PCCTL_DisablePlayerControl);
          Script_ShowUIMessageFromID("E1012A079");
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
          Script_ShowUIMessageFromID("E1012A080");
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
          Script_ryoControl(RYO_RestoreControl);
          Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_YDB1_BOOK_FOR_MASTER_CHEN, 1);
        }
        break;
      case 0x324B4159:
        if (task_448 == 1) {
          Script_ShowUIMessageFromID("E1012A180");
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
          Script_ShowUIMessageFromID("E1012A298");
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
        }
        if (!(task_448 == 2)) goto L005CFC;
        L005CFC:;
        break;
      case 0x554B4159:
        if (task_448 == 1) {
          Script_ShowUIMessageFromID("E1012A290");
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
          Script_ShowUIMessageFromID("E1012A293");
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
          Script_ShowUIMessageFromID("E1012A294");
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
          Script_ShowUIMessageFromID("E1012A295");
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
        }
        if (!(task_448 == 2)) goto L005D97;
        L005D97:;
        break;
      case 0x30544E42:
        if (task_448 == 1) {
          Script_ShowUIMessageFromID("E1012A254");
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
        }
        break;
      case 0x4954414D:
        if (task_448 == 1) {
          Script_ShowUIMessageFromID("E1012A153");
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
        }
        if (task_448 == 2) {
          Script_GetCassetteFlags(55, 3, 0, 0);
          Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 836, 1);
        }
        break;
      case 0x4B4F4F42:
        if (task_448 == 1) {
          Script_ShowUIMessageFromID("E1012A299");
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
          Script_ShowUIMessageFromID("E1012A300");
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
        }
        if (task_448 == 2) {
          Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_YDB1_BOOK_FOR_MASTER_CHEN, 1);
        }
        break;
      case 0x4C4C4142:
        if (task_448 == 1) {
          Script_ShowUIMessageFromID("E1012A274");
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
          Script_ShowUIMessageFromID("E1012A275");
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
        }
        break;
      case 0x33534E54:
        if (task_448 == 5) {
          if (Script_GetOwnedItemCount(0, 0) == 0) {
            Script_ShowUIMessageFromID("E1012A158");
            while (Script_convGetIsActive(0)) {
              SCNF_WaitFrames_0000(1);
            }
          }
        }
        break;
      case 0x5A414B4B:
        if (task_448 == 1) {
          Script_ShowUIMessageFromID("E1012A159");
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
          Script_ShowUIMessageFromID("E1012A296");
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
          Script_ShowUIMessageFromID("E1012A297");
        }
        if (!(task_448 == 2)) goto L005F94;
        L005F94:;
        break;
      case 0x4B53554B:
        if (task_448 == 1) {
          Script_ShowUIMessageFromID("E1012A164");
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
        }
        break;
      case 0x554F5249:
        if (task_448 == 1) {
          Script_ShowUIMessageFromID("E1012A135");
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
        }
        if (task_448 == 1) {
          Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 844, 1);
        }
        break;
      case 0x53415A53:
        if (task_448 == 1) {
          Script_ShowUIMessageFromID("E1012A150");
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
        }
        break;
      case 0x30534E54:
        break;
      case 0x41445546:
        Script_ryoControl(PCCTL_DisablePlayerControl);
        local_D0 = *(&task_3A4 + 6);
        switch (local_D0) {
          case 0:
            Script_ShowUIMessageFromID("E1012A259");
            *(&task_3A4 + 6) = 1;
            break;
          case 1:
            Script_ShowUIMessageFromID("E1012A260");
            *(&task_3A4 + 6) = 0;
            break;
        }
        while (Script_convGetIsActive(0)) {
          SCNF_WaitFrames_0000(1);
        }
        Script_ryoControl(RYO_RestoreControl);
        break;
    }
  }
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static int32_t script_60EF(void) {
  Vector3 local_F4;
  int32_t unused_EC[2];
  int32_t local_E4[2];
  float local_E0;
  int32_t local_DC;
  local_DC = 0;
  if (task_2F0 == 0) {
    Script_EVT_GetPosition(CHARA_AKIR, -1, &local_F4, 0);
    task_3CC.y = (&local_F4)->y;
    local_E0 = Script_MathUtils(SCRIPT_MATH_TASK_VEC3_DISTANCE, &local_F4, &task_3CC);
    if (!(local_E0 >= 1.8f)) {
      local_E4[1] = Script_FloatUtils(ScriptMathOp_Atan2ReverseVector, (&task_3CC)->x - (&local_F4)->x, task_3CC.z - (&local_F4)->z);
      Script_mtwkGetSetRotation(CHARA_AKIR, &task_290, 0x2000000);
      if ((local_E4[1] + 0x3000 > task_290.y) & (local_E4[1] - 0x3000 < task_290.y)) {
        local_DC = 1;
      }
    }
  }
  return local_DC;
}

static int32_t script_61A2(void) {
  Vector3 local_F4;
  Vector3 local_E8;
  Vector3i local_DC;
  int32_t local_D8;
  local_E8.x = 0.3531f;
  (&local_E8)->y = 0.0f;
  (&local_E8)->z = 5.4435f;
  local_D8 = 0;
  Script_EVT_GetPosition(CHARA_AKIR, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  Script_mtwkGetSetRotation(CHARA_AKIR, &local_DC, 0x42000000);
  if (Script_CheckTargetWithinAngleRange(&local_E8, 1.5f, 0xC000, 12743, &local_F4)) {
    if (Script_CheckTargetWithinAngleRange(&local_F4, 1.4f, (&local_DC)->y, 7281, &local_E8)) {
      local_D8 = 1;
    }
  }
  return local_D8;
}

static int32_t script_6225(void) {
  Vector3 local_F4;
  Vector3 local_E8;
  Vector3i local_DC;
  int32_t local_D8;
  local_E8.x = 6.5205f;
  (&local_E8)->y = 0.2f;
  (&local_E8)->z = 0.0196f;
  local_D8 = 0;
  Script_EVT_GetPosition(CHARA_AKIR, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  Script_mtwkGetSetRotation(CHARA_AKIR, &local_DC, 0x42000000);
  if (Script_CheckTargetWithinAngleRange(&local_E8, 1.5f, 0x8000, 12743, &local_F4)) {
    if (Script_CheckTargetWithinAngleRange(&local_F4, 1.4f, (&local_DC)->y, 7281, &local_E8)) {
      local_D8 = 1;
    }
  }
  return local_D8;
}

static int32_t script_62AB(void) {
  for (;;) {
    Script_GetCameraInputDetails(15);
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_62C0(void) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  task_650 = 0;
  Script_SetCPAMFlags(CHARA_FUKU, 0);
  if (Script_GetOwnedItemCount(51, 0) == 0) {
    Script_scenCreateTask(script_7E8C, 0);
  }
  if (Script_GetCurrentEntryPoint() == 5) {
    local_F8 = 0;
    Script_GetCassetteFlags(55, 10, 0, 0);
    Script_GetCassetteFlags(54, 10, 0, 0);
    if (Script_GetOwnedItemCount(51, 0) == 0) {
      Script_GetCassetteFlags(51, 1, 0, 0);
    }
    Script_GetCassetteFlags(56, 1, 0, 0);
  } else {
    local_F8 = Script_GetCurrentEntryPoint();
  }
  SAVE_Script_UpdateForSaveData(0, 0);
  if (Script_GetCurrentEntryPoint() == 3) {
    SAVE_Script_UpdateForSaveData(3, 0, 300);
  } else {
    SAVE_Script_UpdateForSaveData(3, 0, (8 - local_F8 * 3) * 1800);
  }
  for (;;) {
    if (Script_GetOwnedItemCount(5, 0) == 0) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  local_FC = 0;
  for (;;) {
    if (Script_scenUtils(SCENARIO_CMD_GET_STEP_LUT, 21) == 3) {
      break;
    }
    for (;;) {
      if (Script_scenUtils(SCENARIO_CMD_GET_STEP_LUT, 21) == 2) {
        if (task_2D4 == 1) {
          if (Script_mapEvUtils(0x4D450002, CHARA_AKIR) != 100) {
            local_FC = SAVE_Script_UpdateForSaveData(4, 0);
          }
        }
      } else {
        if (Script_mapEvUtils(0x4D450002, CHARA_AKIR) != 100) {
          local_FC = SAVE_Script_UpdateForSaveData(4, 0);
        }
      }
      SCNF_WaitFrames_0000(1);
      if (-(local_FC < 0) & Script_IsPlayerAllowed()) {
        if (Script_SetTaskDataValue(CHARA_AKIR, &data_F95C, &data_F968, 0)) {
          Script_mtwkGetSetRotation(CHARA_AKIR, &task_3F4, 0);
          if ((task_3F4.y > 0x9B00) & (task_3F4.y < 0xE500)) {
            if (task_2F0 == 0) {
              if ((task_420 == 0) & (task_250 == 0)) {
                break;
              }
            }
          }
        }
      }
      if (task_650 == 1) {
        script_7ADA();
      }
    }
    local_F4 = Script_scenUtils(SCENARIO_CMD_GET_STEP_LUT, 21);
    if ((local_F4 == 0) | (local_F4 == 1)) {
      if (-(task_2D4 == 0) & (-(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_TIP_FLOOR_SCRATCHES) == 0) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_FLOOR_SCRATCHES) == 0))) {
        (void)((task_250 = -1) & 0x80);
        Script_charaDisplay(CHARA_FUKU, CDM_Show);
        script_6F93();
        Script_scenUtils(SCENARIO_CMD_SET_STEP_LUT, 21, 2);
        SAVE_Script_UpdateForSaveData(3, 0, (8 - local_F8 * 3) * 1800);
        local_FC = 0;
        (void)((task_250 = 0) & 0x80);
        Script_sprintf(data_F974);
      } else {
        Script_scenUtils(SCENARIO_CMD_SET_STEP_LUT, 21, 2);
      }
    } else {
      switch (local_F4) {
        case 2:
          if (task_254 == 2) {
            if (-(task_2D4 == 1) & -(task_2CC == 0) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_TIP_WALL_DIFFERENT_COLOR) == 0)) {
              (void)((task_250 = -1) & 0x80);
              Script_charaDisplay(CHARA_FUKU, CDM_Show);
              script_7583();
              Script_scenUtils(SCENARIO_CMD_SET_STEP_LUT, 21, 3);
              (void)((task_250 = 0) & 0x80);
              goto L0065B3;
            }
            L0065B3:;
          } else {
            Script_scenUtils(SCENARIO_CMD_SET_STEP_LUT, 21, 3);
          }
          break;
        case 3:
          goto L0065D5;
          break;
      }
    }
  }
  L0065D5:;
  return;
}

static void script_65D6(void) {
  Script_seqEnterEvtCam(5000, 0, AUTH_None);
  SCNF_WaitFrames_0000(125);
  Script_seqEnterEvtCam(5001, 0, AUTH_None);
  SCNF_WaitFrames_0000(220);
  Script_seqEnterEvtCam(5002, 0, AUTH_None);
  SCNF_WaitFrames_0000(60);
  Script_seqEnterEvtCam(5003, 0, AUTH_None);
  SCNF_WaitFrames_0000(120);
  Script_seqEnterEvtCam(5004, 0, AUTH_None);
  return;
}

__attribute__((scn_string_bindings(2, data_F9CC, 3, data_F9D6, 4, data_F9E0, 5, data_F9EA, 6, data_F9F4)))
static void script_6631(void) {
  {
    int32_t unused_F4[3];
    Vector3 local_E8;
    if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, 0x554B5546) & Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, 0x52494B41)) {
      Script_EVT_GetPosition(CHARA_AKIR, 4, &local_E8, ENTITY_XFORM_WORLD_SPACE);
      Script_SetLookPointDetails(CHARA_FUKU, 12, &local_E8, 1);
    }
    if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, 0x52494B41) & Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, 0x554B5546)) {
      Script_EVT_GetPosition(CHARA_FUKU, 4, &local_E8, ENTITY_XFORM_WORLD_SPACE);
      Script_SetLookPointDetails(CHARA_AKIR, 12, &local_E8, 1);
    }
  }
  SCNF_WaitFrames_0000(1);
  {
    int32_t unused_F4[3];
    int32_t local_F0;
    local_F0 = Script_scenUtils(SCENARIO_CMD_GET_STEP_LUT, 21);
    if ((local_F0 == 0) | (local_F0 == 1)) {
      Script_ShowUIMessageFromID("E1012B065");
      while (Script_convGetIsActive(0)) {
        SCNF_WaitFrames_0000(1);
      }
      Script_ShowUIMessageFromID("E1012B266");
      while (Script_convGetIsActive(0)) {
        SCNF_WaitFrames_0000(1);
      }
      Script_ShowUIMessageFromID("E1012A230");
      task_32C = 1;
    } else {
      if (local_F0 == 2) {
        Script_ShowUIMessageFromID("E1012B059");
        while (Script_convGetIsActive(0)) {
          SCNF_WaitFrames_0000(1);
        }
        Script_ShowUIMessageFromID("E1012B060");
        while (Script_convGetIsActive(0)) {
          SCNF_WaitFrames_0000(1);
        }
        Script_ShowUIMessageFromID("E1012A250");
        while (Script_convGetIsActive(0)) {
          SCNF_WaitFrames_0000(1);
        }
        Script_ShowUIMessageFromID("E1012A251");
        while (Script_convGetIsActive(0)) {
          SCNF_WaitFrames_0000(1);
        }
        Script_ShowUIMessageFromID(data_F9FE);
        while (Script_convGetIsActive(0)) {
          SCNF_WaitFrames_0000(1);
        }
        goto L0067CA;
      }
    }
  }
  L0067CA:;
  Script_SetCharaLookAtPosition(CHARA_FUKU, -1, 0, 0);
  Script_SetCharaLookAtPosition(CHARA_AKIR, -1, 0, 0);
  SCNF_WaitFrames_0000(1);
  Script_charaToggleMotEnabled(CHARA_FUKU, 1);
  SCNF_WaitFrames_0000(30);
  Script_PlayCharaMotion(CHARA_FUKU, MN_AKI_AKI_TURN180_WAD_R, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  SCNF_WaitFrames_0000(25);
  {
    int32_t unused_F0[4];
    Vector3i local_E4;
    (&local_E4)->y = 0x8000;
    Script_EVT_SetRotation(CHARA_FUKU, -0x70000000, &local_E4, 0);
  }
  Script_PlayCharaMotion(CHARA_FUKU, MN_AKI_AKI_WALK_UP1_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  SCNF_WaitFrames_0000(30);
  Script_charaDisplay(CHARA_FUKU, CDM_Hide);
  Script_EVT_SetPosition(CHARA_FUKU, POS_XYZ, &data_F994);
  return;
}

__attribute__((scn_string_bindings(1, data_FA62)))
static void script_6894(void) {
  int32_t unused_F4[3];
  int32_t local_E8[3];
  int32_t local_DC[3];
  Vector3 local_D0;
  local_E8[0] = 0x3F266666;
  local_E8[1] = 0;
  local_E8[2] = 0x40628F5C;
  local_DC[0] = 0;
  local_DC[1] = 48059;
  local_DC[2] = 0;
  Script_ccowUtils(CHARA_FUKU, 2);
  Script_motmToggleFlags(CHARA_AKIR, 2, 0x40);
  Script_motiSetAngFlags(CHARA_AKIR, 0);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  task_654 = Script_scenCreateTask(script_62AB, 0);
  script_0B9C(6100, CHARA_AKIR, CHARA_AKIR, -1);
  script_0B9C(6101, -1, -1, 135);
  script_0792(CHARA_AKIR, -1, 0x4091EB85, 0, 0x406C28F6, -1);
  script_0893(CHARA_AKIR, -1, 0x43610000, -1);
  script_0792(CHARA_AKIR, -1, 0x3FD851EC, 0, 0x4098A3D7, 135);
  script_0893(CHARA_AKIR, -1, 0x42200000, 135);
  script_0792(CHARA_FUKU, -1, 0x3F266666, 0, 0x40628F5C, -1);
  script_0893(CHARA_FUKU, -1, 0x43848000, -1);
  Script_ShowUIMessageFromID("E1012B038");
  Script_SetCPAMFlags(CHARA_FUKU, 0);
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, 0x52494B41) & Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, 0x554B5546)) {
    Script_EVT_GetPosition(CHARA_FUKU, 4, &local_D0, ENTITY_XFORM_WORLD_SPACE);
    Script_SetLookPointDetails(CHARA_AKIR, 12, &local_D0, 1);
  }
  SCNF_WaitFrames_0000(30);
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_motmToggleFlags(CHARA_AKIR, 1, 0x40);
  Script_ShowUIMessageFromID("E1012A242");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_PlayCharaMotion(CHARA_FUKU, 4142, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  SCNF_WaitFrames_0000(1);
  Script_ShowUIMessageFromID("E1012B002");
  Script_FaceEx(CHARA_FUKU, 1, 5, 4);
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, 0x52494B41) & Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, 0x554B5546)) {
    Script_EVT_GetPosition(CHARA_FUKU, 4, &local_D0, ENTITY_XFORM_WORLD_SPACE);
    Script_SetLookPointDetails(CHARA_AKIR, 12, &local_D0, 1);
  }
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_ShowUIMessageFromID("E1012B003");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_ShowUIMessageFromID("E1012A212");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, 0x554B5546) & Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, 0x52494B41)) {
    Script_EVT_GetPosition(CHARA_AKIR, 4, &local_D0, ENTITY_XFORM_WORLD_SPACE);
    Script_SetLookPointDetails(CHARA_FUKU, 12, &local_D0, 1);
  }
  Script_ShowUIMessageFromID("E1012B004");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_ShowUIMessageFromID(data_FA94);
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_SetCharaLookAtPosition(CHARA_FUKU, -1, 0, 0);
  Script_SetCharaLookAtPosition(CHARA_AKIR, -1, 0, 0);
  Script_SetCPAMFlags(CHARA_FUKU, 0);
  Script_PlayCharaMotion(CHARA_FUKU, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_SetCPAMFlags(CHARA_FUKU, 1);
  Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_FUKU_TIPS, 1);
  Script_motiSetAngFlags(CHARA_AKIR, 1);
  Script_motmToggleFlags(CHARA_AKIR, 1, 0x40);
  Script_srchControl((void *)task_654);
  return;
}

__attribute__((scn_string_bindings(1, data_FB22)))
static void script_6C55(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  Vector3 local_DC;
  Vector3 local_D0;
  script_9422();
  local_F4.x = 0.6543f;
  (&local_F4)->y = 0.0f;
  (&local_F4)->z = 3.5454f;
  local_E8.x = 0;
  (&local_E8)->y = 48059;
  (&local_E8)->z = 0;
  SCNF_RequestIconNotification(0, 0, 0, 2);
  task_654 = Script_scenCreateTask(script_62AB, 0);
  SCNF_WaitFrames_0000(1);
  Script_ccowUtils(CHARA_FUKU, 2);
  Script_motmToggleFlags(CHARA_AKIR, 2, 0x40);
  Script_motmToggleFlags(CHARA_FUKU, 2, 0x40);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_PlayCharaMotion(CHARA_FUKU, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_EVT_SetPosition(CHARA_FUKU, POS_XYZ, &local_F4);
  Script_EVT_SetRotation(CHARA_FUKU, 0x38000000, &local_E8, 0);
  SCNF_WaitFrames_0000(1);
  Script_ShowUIMessageFromID("E1012B067");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_motmToggleFlags(CHARA_AKIR, 1, 0x40);
  Script_motmToggleFlags(CHARA_FUKU, 1, 0x40);
  Script_SetCPAMFlags(CHARA_FUKU, 0);
  Script_seqEnterEvtCam(6100, CHARA_AKIR, CHARA_AKIR);
  Script_EVT_GetPosition(CHARA_AKIR, -1, &local_DC, 0);
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, 0x52494B41) & Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, 0x554B5546)) {
    Script_EVT_GetPosition(CHARA_FUKU, 4, &local_D0, ENTITY_XFORM_WORLD_SPACE);
    Script_SetLookPointDetails(CHARA_AKIR, 12, &local_D0, 1);
  }
  Script_scenCharaSubmitMoveTargetRequest(CHARA_AKIR, &local_DC, 11832, MN_AKI_AKI_STAND_L, 0);
  while (Script_IsMoveTargetRequestActive(CHARA_AKIR) != 0) {
    SCNF_WaitFrames_0000(1);
    Script_GetCameraInputDetails(15);
  }
  Script_ShowUIMessageFromID("E1012A242");
  Script_SetCharaLookAtPosition(CHARA_AKIR, -1, 0, 0);
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_seqEnterEvtCam(6101, 0, AUTH_None);
  Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &data_FAAC);
  SCNF_WaitFrames_0000(1);
  Script_ShowUIMessageFromID("E1012B068");
  Script_FaceEx(CHARA_FUKU, 1, 5, 4);
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, 0x52494B41) & Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, 0x554B5546)) {
    Script_EVT_GetPosition(CHARA_FUKU, 4, &local_D0, ENTITY_XFORM_WORLD_SPACE);
    Script_SetLookPointDetails(CHARA_AKIR, 12, &local_D0, 1);
  }
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, 0x554B5546) & Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, 0x52494B41)) {
    Script_EVT_GetPosition(CHARA_AKIR, 4, &local_D0, ENTITY_XFORM_WORLD_SPACE);
    Script_SetLookPointDetails(CHARA_FUKU, 12, &local_D0, 1);
  }
  Script_ShowUIMessageFromID("E1012A243");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_SetCharaLookAtPosition(CHARA_FUKU, -1, 0, 0);
  Script_SetCharaLookAtPosition(CHARA_AKIR, -1, 0, 0);
  Script_PlayCharaMotion(CHARA_FUKU, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_srchControl((void *)task_654);
  return;
}

static void script_6F93(void) {
  int32_t local_FC;
  script_9422();
  script_D789(CHARA_AKIR, SHEN_NAME('LIGH'));
  Script_lightSetSlotEnabled(15, 1);
  script_6894();
  task_660 = 0;
  for (;;) {
    if (task_420 == 0) {
      local_FC = task_660;
      switch (local_FC) {
        case 0:
          script_7017();
          task_660 = 1;
          break;
        case 1:
          script_6631();
          task_660 = 2;
          goto L007006;
          break;
      }
    }
  }
  L007006:;
  Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_TIP_FLOOR_SCRATCHES, 1);
  script_94B2();
  return;
}

__attribute__((scn_string_bindings(0, data_FB78)))
static void script_7017(void) {
  int32_t local_FC;
  Vector3 local_F0;
  int32_t local_E4[3];
  task_654 = Script_scenCreateTask(script_62AB, 0);
  local_FC = Script_scenCreateTask(script_7358, 1, 60);
  Script_FaceLookAtTargetUtils(CHARA_FUKU, 0);
  script_0B9C(6103, -1, -1, 60);
  script_0B9C(6104, -1, -1, 130);
  script_0B9C(6105, -1, -1, 310);
  script_0B9C(6106, -1, -1, 480);
  script_0B9C(6107, -1, -1, 570);
  script_96F1(CHARA_AKIR, 11, (int32_t)&data_FB6C, 130, 820);
  local_E4[0] = 0;
  local_E4[1] = 57708;
  local_E4[2] = 0;
  local_E4[1] = Script_EVT_SetRotation(CHARA_FUKU, 0x14000000, (const Vector3i *)local_E4, 0);
  for (;;) {
    SCNF_WaitFrames_0000(1);
    if (Script_IsCharaMotionFinished(CHARA_FUKU)) {
      Script_EVT_SetRotation(CHARA_FUKU, MATRIX_XFORM_Y, (const Vector3i *)local_E4, 0);
      Script_PlayCharaMotion(CHARA_FUKU, MN_AKI_AKI_STAND_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
      break;
    }
  }
  Script_PlayCharaMotion(CHARA_FUKU, MN_AKI_AKI_WALK_SLOW_K_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_LineWalkUtils(LNWK_SCRIPT_BEGIN_PATH_WALK, CHARA_FUKU, &data_FB40, 0, 0);
  while (Script_LineWalkUtils(LNWK_SCRIPT_SET_MOVE_SCALE_GET_CMD, CHARA_FUKU, 0.05f) != -1) {
    Script_GetCameraInputDetails(15);
    if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, 0x52494B41) & Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, 0x554B5546)) {
      Script_EVT_GetPosition(CHARA_FUKU, 4, (Vector3 *)local_E4, ENTITY_XFORM_WORLD_SPACE);
      Script_SetLookPointDetails(CHARA_AKIR, 12, (const Vector3 *)local_E4, 1);
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_motmSetBlendFrames(CHARA_FUKU, MOTM_BLEND_FRAMES_30);
  Script_PlayCharaMotion(CHARA_FUKU, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_ShowUIMessageFromID("E1012B051");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_SetCharaLookAtPosition(CHARA_AKIR, -1, 0, 0);
  Script_PlayCharaMotion(CHARA_FUKU, 4138, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  SCNF_WaitFrames_0000(60);
  Script_ShowUIMessageFromID("E1012B052");
  SCNF_WaitFrames_0000(10);
  Script_ccowUtils(CHARA_AKIR, 2);
  Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &data_FB54);
  SCNF_WaitFrames_0000(1);
  local_F0.x = 4.0274f;
  (&local_F0)->y = -0.5f;
  (&local_F0)->z = -0.2809f;
  Script_scenCharaSubmitMoveTargetRequest(CHARA_AKIR, &data_FB60, 4551, MN_AKI_AKI_STAND_LP, 0);
  L0072D1:;
  SCNF_WaitFrames_0000(1);
  if (!(Script_IsMoveTargetRequestActive(CHARA_AKIR) == 0)) goto L0072D1;
  Script_SetLookPointDetails(CHARA_AKIR, 12, &local_F0, 1);
  SCNF_WaitFrames_0000(30);
  Script_ShowUIMessageFromID("E1012A245");
  SCNF_WaitFrames_0000(150);
  Script_ShowUIMessageFromID("E1012A246");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  SCNF_WaitFrames_0000(110);
  Script_SetCharaLookAtPosition(CHARA_AKIR, -1, 0, 0);
  Script_srchControl((void *)local_FC);
  Script_srchControl((void *)task_654);
  return;
}

static int32_t script_7358(int32_t arg0) {
  Vector3 local_F4;
  Vector3 local_E8;
  Vector3i local_DC;
  if (arg0 >= 0) {
    SCNF_WaitFrames_0000(arg0);
  }
  Script_lightSetSlotEnabled(15, 0);
  Script_fixoAttachEx(SHEN_NAME('LIGH'), CHARA_AKIR, 12, &data_FBA0, &data_FBAC);
  for (;;) {
    Script_EVT_GetPosition(SHEN_NAME('LIGH'), -1, &local_E8, ENTITY_XFORM_WORLD_SPACE);
    Script_mtwkGetSetRotation(SHEN_NAME('LIGH'), &local_DC, 0x40000000);
    local_F4.x = (float)0 - Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_DC)->x);
    (&local_F4)->y = Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_DC)->y);
    (&local_F4)->z = Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_DC)->z);
    local_F4.x = 1.0f;
    (&local_F4)->y = 1.0f;
    (&local_F4)->z = 1.0f;
    Script_lightSpotLightEnable(28, &local_E8, &local_F4);
    Script_LightUtils(28, 2, &data_F258);
    Script_LightUtils(28, 4, 2.1f);
    Script_LightUtils(28, 5, 2.8f);
    Script_LightUtils(28, 6, data_F268);
    Script_LightUtils(28, 10, 7281);
    Script_lightSetSlotEnabled(28, 1);
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_7468(void) {
  Script_seqEnterEvtCam(3201, 0, AUTH_None);
  SCNF_WaitFrames_0000(225);
  Script_seqEnterEvtCam(3202, 0, AUTH_None);
  SCNF_WaitFrames_0000(275);
  Script_seqEnterEvtCam(3203, 0, AUTH_None);
  SCNF_WaitFrames_0000(175);
  Script_seqEnterEvtCam(3204, 0, AUTH_None);
  SCNF_WaitFrames_0000(275);
  return;
}

__attribute__((scn_string_bindings(0, data_FBB8)))
static int32_t script_74BB(void) {
  Vector3 local_F4;
  Script_seqEnterEvtCam(6100, CHARA_AKIR, CHARA_AKIR);
  SCNF_WaitFrames_0000(135);
  Script_seqEnterEvtCam(6116, 0, AUTH_None);
  SCNF_WaitFrames_0000(350);
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, 0x52494B41) & Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, 0x554B5546)) {
    Script_EVT_GetPosition(CHARA_FUKU, 4, &local_F4, ENTITY_XFORM_WORLD_SPACE);
    Script_SetLookPointDetails(CHARA_AKIR, 12, &local_F4, 1);
  }
  Script_seqEnterEvtCam(6111, 0, AUTH_None);
  Script_ShowUIMessageFromID("E1012B051");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_ShowUIMessageFromID(data_FBC2);
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_SetCharaLookAtPosition(CHARA_AKIR, -1, 0, 0);
  end:;
}

static void script_7583(void) {
  int32_t local_FC;
  script_9422();
  Script_SetCPAMFlags(CHARA_FUKU, 0);
  SCNF_WaitFrames_0000(1);
  script_D789(CHARA_AKIR, SHEN_NAME('LIGH'));
  Script_lightSetSlotEnabled(15, 1);
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_FUKU_TIPS)) {
    script_6C55();
  } else {
    Script_scenCreateTask(script_74BB, 0);
    script_6894();
  }
  Script_SetCPAMFlags(CHARA_FUKU, 0);
  task_660 = 0;
  for (;;) {
    if (task_420 == 0) {
      local_FC = task_660;
      if (local_FC == 0) {
        script_7642();
        task_660 = 1;
        break;
        goto L00761A;
      }
    }
    L00761A:;
    SCNF_WaitFrames_0000(1);
  }
  Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_TIP_WALL_DIFFERENT_COLOR, 1);
  Script_SetCPAMFlags(CHARA_FUKU, 1);
  script_94B2();
  return;
}

__attribute__((scn_string_bindings(1, data_FC32, 6, data_FC64, 7, data_FC6E, 8, data_FC78, 9, data_FC82)))
static void script_7642(void) {
  Vector3 local_F4;
  task_654 = Script_scenCreateTask(script_62AB, 0);
  Script_FaceLookAtTargetUtils(CHARA_FUKU, 0);
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_TIP_FLOOR_SCRATCHES) | Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_FLOOR_SCRATCHES)) {
    Script_EVT_SetPosition(CHARA_FUKU, POS_XYZ, &data_FC04);
    Script_EVT_SetRotation(CHARA_FUKU, 0x38000000, &data_FC10, 0);
    if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, 0x52494B41) & Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, 0x554B5546)) {
      Script_EVT_GetPosition(CHARA_FUKU, 4, &local_F4, ENTITY_XFORM_WORLD_SPACE);
      Script_SetLookPointDetails(CHARA_AKIR, 12, &local_F4, 1);
    }
    Script_seqEnterEvtCam(6111, 0, AUTH_None);
    Script_ShowUIMessageFromID("E1012B056");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_ShowUIMessageFromID("E1012A230");
    Script_SetCharaLookAtPosition(CHARA_AKIR, -1, 0, 0);
    Script_EVT_SetPosition(CHARA_FUKU, POS_XYZ, &data_FC04);
    Script_EVT_SetRotation(CHARA_FUKU, 0x38000000, &data_FC10, 0);
    SCNF_WaitFrames_0000(1);
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_ccowUtils(CHARA_AKIR, 2);
    Script_ccowUtils(CHARA_FUKU, 2);
    Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &data_FBE0);
    Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &data_FBEC, 0);
    Script_PlayCharaMotion(CHARA_FUKU, 4155, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_PlayCharaMotion(CHARA_AKIR, 4152, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_charaToggleMotEnabled(CHARA_FUKU, 1);
    Script_motmToggleFlags(CHARA_FUKU, 2, 8);
    SCNF_WaitFrames_0000(1);
    Script_seqEnterEvtCam(6115, 0, AUTH_None);
    SCNF_WaitFrames_0000(120);
    Script_seqEnterEvtCam(6112, 0, AUTH_None);
  } else {
    SCNF_WaitFrames_0000(100);
    Script_EVT_SetPosition(CHARA_FUKU, POS_XYZ, &data_FC04);
    Script_EVT_SetRotation(CHARA_FUKU, 0x38000000, &data_FC10, 0);
    SCNF_WaitFrames_0000(1);
    Script_SetCharaLookAtPosition(CHARA_AKIR, -1, 0, 0);
    Script_ccowUtils(CHARA_AKIR, 2);
    Script_ccowUtils(CHARA_FUKU, 2);
    Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &data_FBE0);
    Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &data_FBEC, 0);
    Script_PlayCharaMotion(CHARA_FUKU, 4155, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_PlayCharaMotion(CHARA_AKIR, 4152, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_charaToggleMotEnabled(CHARA_FUKU, 1);
    SCNF_WaitFrames_0000(1);
    Script_seqEnterEvtCam(6115, 0, AUTH_None);
    Script_motmToggleFlags(CHARA_FUKU, 2, 8);
    SCNF_WaitFrames_0000(120);
  }
  Script_ShowUIMessageFromID("E1012B057");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_ShowUIMessageFromID("E1012A248");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_ShowUIMessageFromID("E1012B058");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_ShowUIMessageFromID("E1012A249");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_seqEnterEvtCam(6113, 0, AUTH_None);
  Script_ShowUIMessageFromID("E1012B059");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, 0x52494B41) & Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, 0x554B5546)) {
    Script_EVT_GetPosition(CHARA_FUKU, 4, &local_F4, ENTITY_XFORM_WORLD_SPACE);
    Script_SetLookPointDetails(CHARA_AKIR, 12, &local_F4, 1);
  }
  Script_ShowUIMessageFromID("E1012A250");
  Script_seqEnterEvtCam(6114, 0, AUTH_None);
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_ShowUIMessageFromID("E1012B060");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_ShowUIMessageFromID("E1012A251");
  Script_SetCharaLookAtPosition(CHARA_AKIR, -1, 0, 0);
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_ShowUIMessageFromID("E1012B061");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  for (;;) {
    if (Script_IsCharaMotionFinished(CHARA_FUKU) != 0) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_EVT_SetPosition(CHARA_FUKU, POS_XYZ, &data_FC1C);
  Script_charaDisplay(CHARA_FUKU, CDM_Hide);
  return;
}

static void script_7ADA(void) {
  Vector3 local_F4;
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_FaceEx(CHARA_AKIR, 1, 0, 2);
  Script_ResetHandMotion(CHARA_AKIR, -1, 0, 0);
  Script_ResetHandMotion(CHARA_AKIR, -1, 1, 0);
  Script_shenClearFreeState(0);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_ShowUIMessageFromID("E1012A174");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &data_FC98);
  Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &data_FCE0, 0);
  Script_charaDisplay(CHARA_FUKU, CDM_Show);
  Script_EVT_SetPosition(CHARA_FUKU, POS_XYZ, &data_FCB0);
  Script_EVT_SetRotation(CHARA_FUKU, 0x38000000, &data_FCEC, 0);
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 1);
  Script_SetCharaRefBoardPos(CHARA_AKIR, &data_FD10);
  Script_charaSetRefboardFlagTo(CHARA_FUKU, 1);
  Script_SetCharaRefBoardPos(CHARA_FUKU, &data_FD10);
  Script_charaDisplay(SHEN_NAME('YKUR'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('YKUL'), CDM_Show);
  Script_EVT_SetPosition(SHEN_NAME('YKUR'), POS_XYZ, &data_FCD4);
  Script_EVT_SetRotation(SHEN_NAME('YKUR'), 0x38000000, &data_FD04, 0);
  Script_EVT_SetPosition(SHEN_NAME('YKUL'), POS_XYZ, &data_FCC8);
  Script_EVT_SetRotation(SHEN_NAME('YKUL'), 0x38000000, &data_FCF8, 0);
  Script_seqEnterEvtCam(9000, 0, AUTH_None);
  SCNF_WaitFrames_0000(1);
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, 0x554B5546) & Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, 0x52494B41)) {
    Script_EVT_GetPosition(CHARA_AKIR, 4, &local_F4, ENTITY_XFORM_WORLD_SPACE);
    Script_SetLookPointDetails(CHARA_FUKU, 12, &local_F4, 0);
  }
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, 0x52494B41) & Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, 0x554B5546)) {
    Script_EVT_GetPosition(CHARA_FUKU, 4, &local_F4, ENTITY_XFORM_WORLD_SPACE);
    Script_SetLookPointDetails(CHARA_AKIR, 12, &local_F4, 1);
  }
  Script_ShowUIMessageFromID("E1012B036");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &data_FCA4);
  Script_EVT_SetPosition(CHARA_FUKU, POS_XYZ, &data_FCBC);
  Script_seqEnterEvtCam(9001, 0, AUTH_None);
  Script_ShowUIMessageFromID("E1012A232");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_EVT_SetPosition(CHARA_FUKU, POS_XYZ, &data_FCB0);
  Script_seqEnterEvtCam(9002, 0, AUTH_None);
  Script_ShowUIMessageFromID("E1012B037");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_seqEnterEvtCam(9003, 0, AUTH_None);
  Script_ShowUIMessageFromID("E1012A233");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  task_658 = 10;
  task_650 = 0;
  Script_executeAicaCommand(2720, 1, 30);
  Script_executeAicaCommand(2720, 2, 30);
  Script_executeAicaCommand(2720, 3, 30);
  Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  (void)((task_250 = -1) & 0x20);
  Script_SetStats(STATS_PLAYER_CONTEXT_ACTION_MODE, 0);
  script_D87D(0x52494B41, SHEN_NAME('LIGH'));
  task_2E4 = 0;
  SCNF_WaitFrames_0000(1);
  Script_SetRyoMotionDetails(CHARA_AKIR);
  script_DC39();
  script_D06D(3, 0x4F4D4F4A, 0);
  Script_shenRequestGameModuleTransition(3, SHEN_NAME('JHD0'), 6);
  SCNF_WaitFrames_0000(1);
  return;
}

static int32_t script_7E8C(void) {
  int32_t local_FC;
  local_FC = 0;
  while ((Script_GetOwnedItemCount(51, 0) == 0) & (task_658 < 2)) {
    if (local_FC > 600) {
      script_7ADA();
    }
    if (Script_IsPlayerAllowed()) {
      local_FC = local_FC + 1;
    }
    SCNF_WaitFrames_0000(1);
  }
  return Script_srchControl(0);
}

static void script_7EDB(void) {
  int32_t unused_FC;
  GameDateTime local_F4;
  int8_t local_F3;
  int8_t local_F2;
  int8_t local_F1;
  uint8_t unused_EF[2];
  int8_t local_EE;
  int8_t local_ED;
  int8_t local_EC;
  int8_t local_EB;
  int8_t local_EA;
  int8_t local_E9;
  int8_t local_E8;
  int8_t local_E7;
  int8_t local_E6;
  int8_t local_DD[9];
  uint8_t unused_DC;
  int32_t local_D8;
  Script_ReturnTimeDate(&local_F4);
  local_EE = (&local_F4)->year;
  local_ED = (&local_F4)->month;
  local_EC = (&local_F4)->day;
  local_F3 = 0;
  local_D8 = local_ED;
  switch (local_D8) {
    case 12:
      while (local_F3 < 9) {
        local_DD[local_F3] = data_FD54[local_F3];
        local_F3 = local_F3 + 1;
      }
      break;
    case 1:
      while (local_F3 < 9) {
        local_DD[local_F3] = data_FD60[local_F3];
        local_F3 = local_F3 + 1;
      }
      break;
    case 2:
      while (local_F3 < 9) {
        local_DD[local_F3] = data_FD6C[local_F3];
        local_F3 = local_F3 + 1;
      }
      break;
    case 3:
      while (local_F3 < 9) {
        local_DD[local_F3] = data_FD78[local_F3];
        local_F3 = local_F3 + 1;
      }
      break;
  }
  local_F3 = 0;
  local_EB = 0;
  while (local_F3 < 9) {
    if (local_EC == local_DD[local_F3]) {
      local_EB = 1;
    }
    local_F3 = local_F3 + 1;
  }
  *(int8_t *)task_664 = Script_PlayerStatsUtils(PS_GetStepSlot, 0);
  *((int8_t *)task_664 + 1) = Script_PlayerStatsUtils(PS_GetStepSlot, 1);
  *((int8_t *)task_664 + 2) = Script_PlayerStatsUtils(PS_GetStepSlot, 2);
  *((int8_t *)task_664 + 3) = Script_PlayerStatsUtils(PS_GetStepSlot, 3);
  *((int8_t *)task_664 + 4) = Script_PlayerStatsUtils(PS_GetStepSlot, 4);
  *((int8_t *)task_664 + 5) = Script_PlayerStatsUtils(PS_GetStepSlot, 5);
  *((int8_t *)task_664 + 6) = Script_PlayerStatsUtils(PS_GetStepSlot, 6);
  *((int8_t *)task_664 + 7) = Script_PlayerStatsUtils(PS_GetStepSlot, 7);
  *((int8_t *)task_664 + 8) = Script_PlayerStatsUtils(PS_GetStepSlot, 8);
  *((int8_t *)task_664 + 9) = Script_PlayerStatsUtils(PS_GetStepSlot, 9);
  *((int8_t *)task_664 + 10) = Script_PlayerStatsUtils(PS_GetStepSlot, 10);
  *((int8_t *)task_664 + 11) = Script_PlayerStatsUtils(PS_GetStepSlot, 11);
  *((int8_t *)task_664 + 12) = Script_PlayerStatsUtils(PS_GetStepSlot, 12);
  *((int8_t *)task_664 + 13) = Script_PlayerStatsUtils(PS_GetStepSlot, 13);
  *((int8_t *)task_664 + 14) = Script_PlayerStatsUtils(PS_GetStepSlot, 14);
  *((int8_t *)task_664 + 15) = Script_PlayerStatsUtils(PS_GetStepSlot, 15);
  if (*(int8_t *)task_664 == 0) {
    *(int8_t *)task_664 = 8;
    Script_PlayerStatsUtils(PS_SetStepSlot, 0, *(int8_t *)task_664);
    *((int8_t *)task_664 + 1) = 4;
    Script_PlayerStatsUtils(PS_SetStepSlot, 1, *((int8_t *)task_664 + 1));
    *((int8_t *)task_664 + 2) = 7;
    Script_PlayerStatsUtils(PS_SetStepSlot, 2, *((int8_t *)task_664 + 2));
    *((int8_t *)task_664 + 3) = 9;
    Script_PlayerStatsUtils(PS_SetStepSlot, 3, *((int8_t *)task_664 + 3));
    *((int8_t *)task_664 + 4) = 6;
    Script_PlayerStatsUtils(PS_SetStepSlot, 4, *((int8_t *)task_664 + 4));
    *((int8_t *)task_664 + 5) = 5;
    Script_PlayerStatsUtils(PS_SetStepSlot, 5, *((int8_t *)task_664 + 5));
    *((int8_t *)task_664 + 6) = 2;
    Script_PlayerStatsUtils(PS_SetStepSlot, 6, *((int8_t *)task_664 + 6));
    *((int8_t *)task_664 + 7) = 5;
    Script_PlayerStatsUtils(PS_SetStepSlot, 7, *((int8_t *)task_664 + 7));
    *((int8_t *)task_664 + 8) = 7;
    Script_PlayerStatsUtils(PS_SetStepSlot, 8, *((int8_t *)task_664 + 8));
    *((int8_t *)task_664 + 9) = 3;
    Script_PlayerStatsUtils(PS_SetStepSlot, 9, *((int8_t *)task_664 + 9));
    *((int8_t *)task_664 + 10) = 2;
    Script_PlayerStatsUtils(PS_SetStepSlot, 10, *((int8_t *)task_664 + 10));
    *((int8_t *)task_664 + 11) = 5;
    Script_PlayerStatsUtils(PS_SetStepSlot, 11, *((int8_t *)task_664 + 11));
    *((int8_t *)task_664 + 12) = 6;
    Script_PlayerStatsUtils(PS_SetStepSlot, 12, *((int8_t *)task_664 + 12));
    *((int8_t *)task_664 + 13) = 3;
    Script_PlayerStatsUtils(PS_SetStepSlot, 13, *((int8_t *)task_664 + 13));
    *((int8_t *)task_664 + 14) = 2;
    Script_PlayerStatsUtils(PS_SetStepSlot, 14, *((int8_t *)task_664 + 14));
    *((int8_t *)task_664 + 15) = 4;
    Script_PlayerStatsUtils(PS_SetStepSlot, 15, *((int8_t *)task_664 + 15));
  }
  local_F3 = 0;
  local_F2 = 0;
  local_F1 = 0;
  while (1) {
    if ((*(int8_t *)task_664 == Script_PlayerStatsUtils(PS_GetStepSlot, 0)) | (*((int8_t *)task_664 + 1) == Script_PlayerStatsUtils(PS_GetStepSlot, 1)) | (*((int8_t *)task_664 + 2) == Script_PlayerStatsUtils(PS_GetStepSlot, 2)) | (*((int8_t *)task_664 + 3) == Script_PlayerStatsUtils(PS_GetStepSlot, 3)) | (*((int8_t *)task_664 + 4) == Script_PlayerStatsUtils(PS_GetStepSlot, 4)) | (*((int8_t *)task_664 + 5) == Script_PlayerStatsUtils(PS_GetStepSlot, 5))) {
      local_EA = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 6.0f);
      local_E9 = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 6.0f);
      local_E8 = *((int8_t *)task_664 + local_EA);
      local_E7 = *((int8_t *)task_664 + local_E9);
      *((int8_t *)task_664 + local_EA) = local_E7;
      *((int8_t *)task_664 + local_E9) = local_E8;
    } else {
      local_F3 = 1;
    }
    if ((*((int8_t *)task_664 + 6) == Script_PlayerStatsUtils(PS_GetStepSlot, 6)) | (*((int8_t *)task_664 + 7) == Script_PlayerStatsUtils(PS_GetStepSlot, 7)) | (*((int8_t *)task_664 + 8) == Script_PlayerStatsUtils(PS_GetStepSlot, 8)) | (*((int8_t *)task_664 + 9) == Script_PlayerStatsUtils(PS_GetStepSlot, 9))) {
      local_EA = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 4.0f);
      local_E9 = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 4.0f);
      local_E8 = *((int8_t *)task_664 + (local_EA + 6));
      local_E7 = *((int8_t *)task_664 + (local_E9 + 6));
      *((int8_t *)task_664 + (local_EA + 6)) = local_E7;
      *((int8_t *)task_664 + (local_E9 + 6)) = local_E8;
    } else {
      local_F2 = 1;
    }
    if ((*((int8_t *)task_664 + 10) == Script_PlayerStatsUtils(PS_GetStepSlot, 10)) | (*((int8_t *)task_664 + 11) == Script_PlayerStatsUtils(PS_GetStepSlot, 11)) | (*((int8_t *)task_664 + 12) == Script_PlayerStatsUtils(PS_GetStepSlot, 12)) | (*((int8_t *)task_664 + 13) == Script_PlayerStatsUtils(PS_GetStepSlot, 13))) {
      local_EA = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 4.0f);
      local_E9 = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 4.0f);
      local_E8 = *((int8_t *)task_664 + (local_EA + 10));
      local_E7 = *((int8_t *)task_664 + (local_E9 + 10));
      *((int8_t *)task_664 + (local_EA + 10)) = local_E7;
      *((int8_t *)task_664 + (local_E9 + 10)) = local_E8;
    } else {
      local_F1 = 1;
    }
    local_EA = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f);
    local_E9 = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f);
    local_E8 = *((int8_t *)task_664 + (local_EA + 14));
    local_E7 = *((int8_t *)task_664 + (local_E9 + 14));
    *((int8_t *)task_664 + (local_EA + 14)) = local_E7;
    *((int8_t *)task_664 + (local_E9 + 14)) = local_E8;
    if (-(local_F3 == 1) & -(local_F2 == 1) & -(local_F1 == 1)) {
      break;
    }
  }
  local_F3 = 0;
  local_E6 = 1;
  local_E8 = *(int8_t *)task_664;
  while (local_F3 < 16) {
    local_E7 = *((int8_t *)task_664 + local_F3);
    if (local_E8 < local_E7) {
      local_E8 = local_E7;
      local_E6 = local_F3 + 1;
    }
    local_F3 = local_F3 + 1;
  }
  Script_scenUtils(SCENARIO_CMD_SET_STEP_LUT, 11, local_E6);
  Script_PlayerStatsUtils(PS_SetStepSlot, 0, *(int8_t *)task_664);
  Script_PlayerStatsUtils(PS_SetStepSlot, 1, *((int8_t *)task_664 + 1));
  Script_PlayerStatsUtils(PS_SetStepSlot, 2, *((int8_t *)task_664 + 2));
  Script_PlayerStatsUtils(PS_SetStepSlot, 3, *((int8_t *)task_664 + 3));
  Script_PlayerStatsUtils(PS_SetStepSlot, 4, *((int8_t *)task_664 + 4));
  Script_PlayerStatsUtils(PS_SetStepSlot, 5, *((int8_t *)task_664 + 5));
  Script_PlayerStatsUtils(PS_SetStepSlot, 6, *((int8_t *)task_664 + 6));
  Script_PlayerStatsUtils(PS_SetStepSlot, 7, *((int8_t *)task_664 + 7));
  Script_PlayerStatsUtils(PS_SetStepSlot, 8, *((int8_t *)task_664 + 8));
  Script_PlayerStatsUtils(PS_SetStepSlot, 9, *((int8_t *)task_664 + 9));
  Script_PlayerStatsUtils(PS_SetStepSlot, 10, *((int8_t *)task_664 + 10));
  Script_PlayerStatsUtils(PS_SetStepSlot, 11, *((int8_t *)task_664 + 11));
  Script_PlayerStatsUtils(PS_SetStepSlot, 12, *((int8_t *)task_664 + 12));
  Script_PlayerStatsUtils(PS_SetStepSlot, 13, *((int8_t *)task_664 + 13));
  Script_PlayerStatsUtils(PS_SetStepSlot, 14, *((int8_t *)task_664 + 14));
  Script_PlayerStatsUtils(PS_SetStepSlot, 15, *((int8_t *)task_664 + 15));
  SCNF_WaitFrames_0000(1);
  Script_sprintf(" : %d %d %d %d %d %d : %d %d %d %d : %d %d %d %d : %d %d \012", *(int8_t *)task_664, *((int8_t *)task_664 + 1), *((int8_t *)task_664 + 2), *((int8_t *)task_664 + 3), *((int8_t *)task_664 + 4), *((int8_t *)task_664 + 5), *((int8_t *)task_664 + 6), *((int8_t *)task_664 + 7), *((int8_t *)task_664 + 8), *((int8_t *)task_664 + 9), *((int8_t *)task_664 + 10), *((int8_t *)task_664 + 11), *((int8_t *)task_664 + 12), *((int8_t *)task_664 + 13), *((int8_t *)task_664 + 14), *((int8_t *)task_664 + 15));
  Script_sprintf("lacky : %d\012", local_E6);
  return;
}

static void script_8753(void) {
  Script_SetupLightFromTable(0);
  Script_ProcessFogFromTable(0);
  Script_lightSetSlotEnabled(2, 0);
  Script_lightSetSlotEnabled(3, 0);
  task_2F0 = 1;
  Script_PlayerStatsUtils(PS_IncrementSpotVisitCount, 14);
  Script_charaSetClipAngle(SHEN_NAME('KINK'), 15.0f);
  task_278 = 0x40D00000;
  *(&task_278 + 1) = 0;
  *(&task_278 + 2) = 0;
  task_2D8 = 45;
  task_2DC = 315;
  if (Script_GetOwnedItemCount(39, 0)) {
    Script_GetCassetteFlags_0(39, 1, 0);
    Script_GetCassetteFlags(39, 1, 0, 0);
  }
  if (Script_GetOwnedItemCount(51, 0)) {
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 628, 1);
  }
  if (Script_GetOwnedItemCount(55, 0)) {
    Script_GetCassetteFlags_0(55, 1, 0);
    Script_GetCassetteFlags(55, 1, 0, 0);
  }
  if (Script_GetOwnedItemCount(0, 0)) {
    Script_GetCassetteFlags_0(0, 1, 0);
    Script_GetCassetteFlags(0, 1, 0, 0);
  }
  if (Script_GetOwnedItemCount(0, 0)) {
    Script_GetCassetteFlags_0(0, 1, 0);
    Script_GetCassetteFlags(0, 1, 0, 0);
  }
  if (Script_GetOwnedItemCount(55, 0) & Script_GetOwnedItemCount(54, 0)) {
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 635, 1);
  }
  if (Script_GetOwnedItemCount(52, 0)) {
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 638, 1);
  }
  return;
}

static int32_t script_8891(void) {
  int32_t unused_FF78[34];
  int32_t local_FF74;
  int32_t local_FF70;
  int32_t local_FF6C;
  int32_t local_FF68;
  int32_t local_FF64;
  int32_t local_FF60;
  Vector3 local_FF54;
  Vector3 local_FF48;
  int32_t local_FF44;
  int32_t unused_FF38[3];
  int32_t local_FF34;
  float local_FF30;
  int32_t unused_FF2C;
  GameDateTime local_FF24;
  local_FF44 = 0;
  Script_lightResetSlotToDefaults(15);
  Script_LightUtils(15, 0, 4);
  Script_LightUtils(15, 2, &data_FDC8);
  Script_LightUtils(15, 4, 1.0f);
  Script_LightUtils(15, 5, 7.0f);
  Script_LightUtils(15, 6, data_FDD8);
  Script_LightUtils(15, 10, 3640);
  Script_lightCreatePoint(6, &data_FE34);
  Script_lightCreatePoint(7, &data_FE60);
  Script_lightCreatePoint(8, &data_FE8C);
  Script_lightCreatePoint(9, &data_FEB8);
  Script_lightSetSlotEnabled(6, 0);
  Script_lightSetSlotEnabled(7, 0);
  Script_lightSetSlotEnabled(8, 0);
  Script_lightSetSlotEnabled(9, 0);
  if (*task_304 == 2) {
    Script_charaDisplay(SHEN_NAME('FIR1'), CDM_Show);
    Script_lightSetSlotEnabled(6, 1);
  }
  if (task_304[1] == 2) {
    Script_charaDisplay(SHEN_NAME('FIR2'), CDM_Show);
    Script_lightSetSlotEnabled(7, 1);
  }
  if (task_304[2] == 2) {
    Script_charaDisplay(SHEN_NAME('FIR3'), CDM_Show);
    Script_lightSetSlotEnabled(8, 1);
  }
  if (task_304[3] == 2) {
    Script_charaDisplay(SHEN_NAME('FIR5'), CDM_Show);
    Script_lightSetSlotEnabled(9, 1);
  }
  Script_lightSetGlobalSceneLighting(&data_FE04);
  task_2EC = Script_scenUtils(SCENARIO_CMD_GET_STEP_LUT, 22) * 1200;
  local_FF34 = 0x40E00000;
  local_FF54.x = 1.0f;
  (&local_FF54)->y = 1.0f;
  (&local_FF54)->z = 1.0f;
  local_FF70 = 0;
  for (;;) {
    (&local_FF54)->y = Script_MathUtils(SCRIPT_MATH_INT_TO_FLOAT, *(int16_t *)((int8_t *)task_2FC + (local_FF70 << 1))) / 240.0f;
    Script_hmdlApplyScale(*(&data_FF74 + local_FF70), &local_FF54);
    Script_EVT_GetPosition(*(&data_FF74 + local_FF70), -1, &local_FF48, 0);
    (&local_FF48)->y = (&local_FF48)->y + (&local_FF54)->y * 0.15f;
    Script_EVT_SetPosition(*(&data_FF84 + local_FF70), POS_XYZ, &local_FF48);
    if (task_304[local_FF70] == 0) {
      Script_charaDisplay(*(&data_FF74 + local_FF70), CDM_Hide);
    } else {
      if (task_304[local_FF70] == 1) {
        Script_charaDisplay(*(&data_FF74 + local_FF70), CDM_Show);
        goto L008ADD;
      }
    }
    L008ADD:;
    local_FF70 = local_FF70 + 1;
    if (local_FF70 == 4) {
      break;
    }
  }
  local_FF70 = 0;
  while (local_FF70 < 35) {
    *(&local_FF74 + local_FF70) = local_FF70 % 4;
    local_FF70 = local_FF70 + 1;
  }
  local_FF64 = 0;
  local_FF60 = 0;
  for (;;) {
    script_E207();
    Script_ReturnTimeDate(&local_FF24);
    if (((&local_FF24)->month == 4) & ((&local_FF24)->day == 15)) {
      (&local_FF24)->hours = 7;
      (&local_FF24)->minutes = 59;
      (&local_FF24)->seconds = 0;
      Script_SetNewDateTime(&local_FF24);
      Script_shenRequestGameModuleTransition(3, SHEN_NAME('JOMO'), 4);
      SCNF_WaitFrames_0000(1);
    } else {
      if (((&local_FF24)->dayOfWeek == 1) & (local_FF60 == 0)) {
        script_7EDB();
        local_FF60 = 1;
        goto L008BBD;
      }
    }
    L008BBD:;
    if ((*task_304 == 2) | (task_304[1] == 2) | (task_304[2] == 2) | (task_304[3] == 2) | Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 890)) {
      task_2F0 = 0;
      if ((task_2E4 == 1) | Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 890)) {
        Script_lightSetGlobalSceneLighting(&data_FE24);
        Script_srchUtils(SRCH_SCRIPT_SetEntrySelectorFilter, 0);
      } else {
        Script_lightSetGlobalSceneLighting(&data_FE14);
        Script_srchUtils(SRCH_SCRIPT_SetEntrySelectorFilter, 0);
      }
    } else {
      if (task_2E4 == 1) {
        Script_lightSetGlobalSceneLighting(&data_FE14);
        Script_srchUtils(SRCH_SCRIPT_SetEntrySelectorFilter, 0);
        task_2F0 = 0;
      } else {
        task_428 = 0;
        task_2F0 = 1;
        Script_lightSetGlobalSceneLighting(&data_FE04);
        Script_srchUtils(SRCH_SCRIPT_SetEntrySelectorFilter, 1);
      }
      if (task_250 == -1) {
        Script_lightSetGlobalSceneLighting(&data_FE24);
      }
    }
    {
      int32_t local_FF20;
      local_FF20 = local_FF64;
      switch (local_FF20) {
        case 0:
          local_FF6C = 0;
          local_FF68 = 17;
          local_FF64 = 1;
          break;
        case 1:
          local_FF6C = 17;
          local_FF68 = 35;
          local_FF64 = 0;
          break;
      }
    }
    local_FF70 = local_FF6C;
    while (local_FF70 < local_FF68) {
      Script_SetImageModuleTaskDataFlag(SHEN_NAME('FIR1'), *(&local_FF74 + local_FF70));
      Script_SetImageModuleTaskDataFlag(SHEN_NAME('FIR2'), *(&local_FF74 + local_FF70));
      Script_SetImageModuleTaskDataFlag(SHEN_NAME('FIR3'), *(&local_FF74 + local_FF70));
      Script_SetImageModuleTaskDataFlag(SHEN_NAME('FIR4'), *(&local_FF74 + local_FF70));
      Script_SetImageModuleTaskDataFlag(SHEN_NAME('FIR5'), *(&local_FF74 + local_FF70));
      *(&local_FF74 + local_FF70) = *(&local_FF74 + local_FF70) + 1;
      *(&local_FF74 + local_FF70) = *(&local_FF74 + local_FF70) % 6;
      local_FF70 = local_FF70 + 1;
    }
    SCNF_WaitFrames_0000(1);
    local_FF70 = 0;
    for (;;) {
      if (-(Script_GetBatteryLifePair(BATTERY_GET_PERCENT, 0) < 13) & -(Script_GetBatteryLifePair(BATTERY_GET_PERCENT, 0) > 0) & -(local_FF44 == 0) & -(task_2E4 == 1)) {
        if (Script_IsPlayerAllowed(0)) {
          if ((task_420 == 0) & (task_250 == 0)) {
            Script_ryoControl(PCCTL_DisablePlayerControl);
            Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
            local_FF44 = 1;
            Script_ShowUIMessageFromID("E1012A287");
            Script_ryoControl(RYO_RestoreControl);
          }
        }
      }
      if ((Script_GetBatteryLifePair(BATTERY_GET_PERCENT, 0) < 1) & (task_2E4 == 1)) {
        if ((task_420 == 0) & (task_250 == 0)) {
          if (Script_IsPlayerAllowed(0)) {
            script_9355();
          }
        }
      }
      if (task_2F0 == 1) {
        if (Script_IsPlayerAllowed(0) & -(task_250 == 0)) {
          if (Script_SetTaskDataValue(CHARA_AKIR, &data_FFA0, &data_FFAC, 0)) {
            script_91CC();
          }
        }
      }
      if (task_304[local_FF70] == 2) {
        int32_t local_FF20;
        local_FF30 = Script_MathUtils(SCRIPT_MATH_INT_TO_FLOAT, *(int16_t *)((int8_t *)task_2FC + (local_FF70 << 1)) - script_90CD(local_FF70)) / 240.0f;
        (&local_FF54)->y = local_FF30;
        if (!(local_FF30 >= 0.0f)) {
          task_304[local_FF70] = 0;
          *(int16_t *)((int8_t *)task_2FC + (local_FF70 << 1)) = 240;
          Script_charaDisplay(*(&data_FF84 + local_FF70), CDM_Hide);
          Script_charaDisplay(*(&data_FF74 + local_FF70), CDM_Hide);
          Script_lightSetSlotEnabled(local_FF70 + 6, 0);
          (&local_FF54)->y = 1.0f;
          local_FF30 = 1.0f;
        }
        local_FF20 = local_FF70;
        switch (local_FF20) {
          case 0:
            Script_hmdlApplyScale(SHEN_NAME('RO11'), &local_FF54);
            Script_EVT_GetPosition(SHEN_NAME('RO11'), -1, &local_FF48, 0);
            (&local_FF48)->y = (&local_FF48)->y + local_FF30 * 0.15f;
            Script_EVT_SetPosition(SHEN_NAME('FIR1'), POS_XYZ, &local_FF48);
            break;
          case 1:
            Script_hmdlApplyScale(SHEN_NAME('RO21'), &local_FF54);
            Script_EVT_GetPosition(SHEN_NAME('RO21'), -1, &local_FF48, 0);
            (&local_FF48)->y = (&local_FF48)->y + local_FF30 * 0.15f;
            Script_EVT_SetPosition(SHEN_NAME('FIR2'), POS_XYZ, &local_FF48);
            break;
          case 2:
            Script_hmdlApplyScale(SHEN_NAME('RO31'), &local_FF54);
            Script_EVT_GetPosition(SHEN_NAME('RO31'), -1, &local_FF48, 0);
            (&local_FF48)->y = (&local_FF48)->y + local_FF30 * 0.15f;
            Script_EVT_SetPosition(SHEN_NAME('FIR3'), POS_XYZ, &local_FF48);
            break;
          case 3:
            Script_hmdlApplyScale(SHEN_NAME('RO41'), &local_FF54);
            Script_EVT_GetPosition(SHEN_NAME('RO41'), -1, &local_FF48, 0);
            (&local_FF48)->y = (&local_FF48)->y + local_FF30 * 0.15f;
            Script_EVT_SetPosition(SHEN_NAME('FIR5'), POS_XYZ, &local_FF48);
            break;
        }
      }
      local_FF70 = local_FF70 + 1;
      if (local_FF70 == 4) {
        break;
      }
    }
  }
  end:;
}

static int32_t script_90CD(int32_t arg0) {
  int32_t unused_FC;
  GameDateTime local_F4;
  int32_t local_F0;
  Script_ReturnTimeDate(&local_F4);
  local_F0 = -1;
  if (task_304[arg0] == 2) {
    if ((&local_F4)->hours - *(int8_t *)&task_674[arg0][4] < 0) {
      (&local_F4)->hours = (&local_F4)->hours + 24;
    }
    (&local_F4)->hours = (&local_F4)->hours - *(int8_t *)&task_674[arg0][4];
    (&local_F4)->minutes = (&local_F4)->minutes - *(int8_t *)&task_674[arg0][5];
    local_F0 = (&local_F4)->hours * 60 + (&local_F4)->minutes;
  } else {
    Script_ReturnTimeDate(&local_F4);
    *(int8_t *)task_674[arg0] = (&local_F4)->year;
    *(int8_t *)&task_674[arg0][1] = (&local_F4)->month;
    *(int8_t *)&task_674[arg0][3] = (&local_F4)->dayOfWeek;
    *(int8_t *)&task_674[arg0][4] = (&local_F4)->hours;
    *(int8_t *)&task_674[arg0][5] = (&local_F4)->minutes;
  }
  return local_F0;
}

__attribute__((scn_string_bindings(0, data_FFE8)))
static void script_91CC(void) {
  Vector3i local_F4;
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_ShowUIMessageFromID("E1012A083");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_motmToggleFlags(CHARA_AKIR, 2, 0x40);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &data_FFC4);
  (&local_F4)->y = 0;
  Script_EVT_SetRotation(CHARA_AKIR, MATRIX_XFORM_Y, &local_F4, 0);
  SCNF_WaitFrames_0000(1);
  Script_scenToggleCharaFlag(0, 100, 1);
  if (task_2CC == 1) {
    task_2CC = 0;
    Script_fixoClearAttachment(CHARA_ONO_);
    Script_EVT_SetPosition(CHARA_ONO_, POS_XYZ, &data_FFD0);
    Script_EVT_SetRotation(CHARA_ONO_, 0x38000000, &data_FFDC, 0);
  }
  Script_seqEnterEvtCam(10, 0, AUTH_None);
  SCNF_WaitFrames_0000(1);
  Script_convSetCamInactiveFlag(1);
  Script_SetGameStateTo(AUTH_None);
  Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_motmToggleFlags(CHARA_AKIR, 1, 0x40);
  Script_ryoControl(RYO_RestoreControl);
  task_2F8 = 0;
  return;
}

static void script_9355(void) {
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  task_2E4 = 0;
  script_D87D(0x52494B41, SHEN_NAME('LIGH'));
  Script_lightSetSlotEnabled(15, 0);
  script_279C(0);
  Script_uiUtils(0, 0, 0);
  Script_GetBatteryLifePair(BATTERY_SET_DECREMENT_ENABLED, 0, 0);
  Script_ShowUIMessageFromID(data_FFF2);
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  if ((*task_304 == 2) | (task_304[1] == 2) | (task_304[2] == 2) | (task_304[3] == 2) | Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 890)) {
    Script_ryoControl(RYO_RestoreControl);
  } else {
    script_91CC();
  }
  return;
}

static void script_9422(void) {
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_FaceEx(CHARA_AKIR, 1, 0, 2);
  Script_ResetHandMotion(CHARA_AKIR, -1, 0, 0);
  Script_ResetHandMotion(CHARA_AKIR, -1, 1, 0);
  Script_shenClearFreeState(0);
  SCNF_RequestIconNotification(0, 0, 0, 2);
  Script_charaDisplay(CHARA_AKIR, CDM_Show);
  (void)((task_250 = -1) & 4);
  Script_ccowUtils(CHARA_AKIR, 2);
  Script_SetActionSelectorHUDElementVisibility(1);
  Script_srchUtils(SRCH_SCRIPT_Reset);
  if (task_2E4 == 1) {
    Script_GetBatteryLifePair(BATTERY_SET_DECREMENT_ENABLED, 0, 0);
  }
  return;
}

static void script_94B2(void) {
  Script_filmCheckOrSetTransitionUtil(15, 0, 0, 0, 0, 255, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_lightSetSlotEnabled(28, 0);
  Script_motiSetAngFlags(CHARA_AKIR, 0);
  Script_motmToggleFlags(CHARA_AKIR, 2, 0x40);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_ccowUtils(CHARA_AKIR, 1);
  Script_convSetCamInactiveFlag(1);
  Script_SetGameStateTo(AUTH_None);
  Script_SetActionSelectorHUDElementVisibility(0);
  if (task_2E4 == 1) {
    Script_GetBatteryLifePair(BATTERY_SET_DECREMENT_ENABLED, 0, 1);
    script_D789(CHARA_AKIR, SHEN_NAME('LIGH'));
    Script_lightSetSlotEnabled(15, 1);
  } else {
    script_D87D(0x52494B41, SHEN_NAME('LIGH'));
    Script_lightSetSlotEnabled(15, 0);
  }
  if (task_2CC == 1) {
    Script_CharHandlers(CHARA_AKIR, 0, 0, 16);
  }
  if (SAVE_Script_UpdateForSaveData(4, 0) < 0) {
    SAVE_Script_UpdateForSaveData(3, 0, 10);
  }
  Script_ryoControl(RYO_RestoreControl);
  Script_FaceEx(CHARA_AKIR, 0, 0, 2);
  Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
  Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
  Script_shenClearFreeState(1);
  SCNF_WaitFrames_0000(10);
  Script_motiSetAngFlags(CHARA_AKIR, 1);
  Script_motmToggleFlags(CHARA_AKIR, 1, 0x40);
  (void)((task_250 = 0) & 4);
  Script_filmCheckOrSetTransitionUtil(15, 255, 0, 0, 0, 0, 0, 0, 0);
  return;
}

static void script_9640(int32_t arg0, float arg1) {
  for (;;) {
    if (Script_GetCharaMotionRemainingFramesScaled(CHARA_AKIR) >= arg1) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_965F(int32_t arg0) {
  while (Script_IsCharaMotionFinished(arg0) == 0) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static int32_t script_967B(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  Vector3 local_F4;
  int32_t local_F0;
  local_F0 = 0;
  local_F4.x = *(float *)arg2;
  (&local_F4)->y = *(float *)(arg2 + 4);
  (&local_F4)->z = *(float *)(arg2 + 8);
  if (arg3 >= 0) {
    SCNF_WaitFrames_0000(arg3);
  }
  while (local_F0 < arg4) {
    local_F0 = local_F0 + 1;
    Script_motiSetAngFlags(arg0, 1);
    Script_SetMotnFlags(arg0, arg1, &local_F4, 0x78000000);
    SCNF_WaitFrames_0000(1);
  }
  return Script_srchControl(0);
}

static void script_96F1(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  Script_scenCreateTask(script_967B, 5, arg0, arg1, arg2, arg3, arg4);
  return;
}

static void script_9709(void) {
  Vector3i local_F4;
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_FaceEx(CHARA_AKIR, 1, 0, 2);
  Script_ResetHandMotion(CHARA_AKIR, -1, 0, 0);
  Script_ResetHandMotion(CHARA_AKIR, -1, 1, 0);
  Script_shenClearFreeState(0);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &data_10008);
  (&local_F4)->y = 0x4000;
  Script_EVT_SetRotation(CHARA_AKIR, MATRIX_XFORM_Y, &local_F4, 0);
  Script_filmCheckOrSetTransitionUtil(15, 255, 0, 0, 0, 0, 0, 0, 0);
  Script_PlayCharaMotion(CHARA_AKIR, 4162, 0.0f, -1, -1, 1.5f, 0);
  script_9640(0x52494B41, 90.0f);
  Script_executeAicaCommand(0x1105A9, 0, 0);
  Script_fixoClearAttachment(CHARA_ONO_);
  Script_EVT_SetPosition(CHARA_ONO_, POS_XYZ, &data_10014);
  Script_EVT_SetRotation(CHARA_ONO_, 0x38000000, &data_10020, 0);
  script_965F(CHARA_AKIR);
  Script_ryoControl(RYO_RestoreControl);
  Script_FaceEx(CHARA_AKIR, 0, 0, 2);
  Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
  Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
  Script_shenClearFreeState(1);
  Script_SetGameStateTo(AUTH_None);
  return;
}

__attribute__((scn_string_bindings(0, data_10044)))
static void script_9891(void) {
  int32_t unused_F8[2];
  SCNF_RequestIconNotification(0, 0, 0, 2);
  (void)((task_250 = 0) & 8);
  task_254 = 2;
  SCNF_WaitFrames_0000(1);
  Script_SetGameStateTo(AUTH_None);
  for (;;) {
    SCNF_WaitFrames_0000(1);
    if (-(task_318 == 1) & -(task_2F0 == 0) & Script_IsPlayerAllowed()) {
      break;
    }
  }
  if (task_2CC == 0) {
    script_9422();
    task_318 = 0;
    Script_ResetHandMotion(CHARA_AKIR, 1, 0, 411);
    task_258 = 0;
    task_284.x = 6.3f;
    task_284.y = 0.0f;
    task_284.z = 0.7f;
    Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &task_284);
    task_290.x = 0;
    task_290.y = 0;
    task_290.z = 0;
    Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &task_290, 0);
    script_D87D(0x52494B41, SHEN_NAME('LIGH'));
    Script_lightSetSlotEnabled(15, 0);
    Script_motiSetAngFlags(CHARA_AKIR, 0);
    Script_motmToggleFlags(CHARA_AKIR, 2, 0x40);
    Script_PlayCharaMotion(CHARA_AKIR, 4105, 78.0f, -1, -1, 1.3f, 0);
    Script_CharHandlers(CHARA_AKIR, 1, 0, 16);
    SCNF_WaitFrames_0000(1);
    Script_motiSetAngFlags(CHARA_AKIR, 1);
    Script_motmToggleFlags(CHARA_AKIR, 1, 0x40);
    Script_SetGameStateTo(AUTH_EventCam);
    Script_scenCreateTask(script_AA3A, 0);
    Script_ccowUtils(CHARA_AKIR, 2);
    Script_lightSpotLightEnable(28, &data_1002C, &data_10038);
    Script_LightUtils(28, 2, &data_F258);
    Script_LightUtils(28, 4, 2.1f);
    Script_LightUtils(28, 5, 2.8f);
    Script_LightUtils(28, 6, data_F268);
    Script_LightUtils(28, 10, 4187);
    Script_lightSetSlotEnabled(28, 1);
    Script_clmdSetFlags(CHARA_AKIR, 4, 1);
    script_9640(0x52494B41, 203.0f);
    Script_SetNewHandMotion(CHARA_AKIR, 0, 402, 0, 0, 0, 0, 1.0f, 60);
    script_9640(0x52494B41, 225.0f);
    Script_executeAicaCommand(132521, 0, 0);
    Script_ShowUIMessageFromID("E1012A040");
    script_9640(0x52494B41, 305.0f);
    Script_executeAicaCommand(198057, 0, 0);
    Script_ShowUIMessageFromID("E1012A041");
    script_9640(0x52494B41, 350.0f);
    Script_ShowUIMessageFromID("E1012A042");
    script_9640(0x52494B41, 385.0f);
    Script_executeAicaCommand(132521, 0, 0);
    script_9640(0x52494B41, 460.0f);
    Script_clmdSetFlags(CHARA_AKIR, 4, 0);
    script_9640(0x52494B41, 470.0f);
    Script_SetNewHandMotion(CHARA_AKIR, 0, 411, 0, 0, 0, 0, 1.0f, 60);
    script_965F(CHARA_AKIR);
    Script_CharHandlers(CHARA_AKIR, -1, 0, 0);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_SPACE_BEHIND_WALL, 1);
    script_94B2();
    Script_lightSetSlotEnabled(28, 0);
  }
  Script_scenCreateTask(script_9BB3, 0);
  return;
}

static int32_t script_9BB3(void) {
  int32_t local_FC;
  int32_t unused_F4[2];
  SCNF_RequestIconNotification(0, 0, 0, 2);
  task_254 = 3;
  local_FC = 0;
  Script_scenUtils(SCENARIO_CMD_SET_STEP_LUT, 26, 2);
  L009BDA:;
  (void)((task_250 = 0) & 4);
  for (;;) {
    if (-(task_318 == 1) & -(task_2CC == 1) & -(task_2F0 == 0) & Script_IsPlayerAllowed()) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  script_9422();
  local_FC = local_FC + 1;
  task_258 = 0;
  task_284.x = 6.2196f;
  task_284.y = 0.0f;
  task_284.z = 0.7367f;
  Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &task_284);
  task_290.x = 0;
  task_290.y = 51154;
  task_290.z = 0;
  Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &task_290, 0);
  task_25C = 11;
  script_D87D(0x52494B41, SHEN_NAME('LIGH'));
  Script_lightSetSlotEnabled(15, 0);
  Script_fixoAttachEx(CHARA_ONO_, CHARA_AKIR, 18, &data_10064, &data_10070);
  SCNF_WaitFrames_0000(1);
  script_D87D(0x52494B41, SHEN_NAME('LIGH'));
  script_9D35();
  script_9E2D();
  local_FC = 2;
  task_290.x = 0;
  task_290.y = 0;
  task_290.z = 0;
  Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &task_290, 0);
  if (!(local_FC >= 2)) goto L009BDA;
  Script_scenCreateTask(script_A0EA, 0);
  return task_2CC = 0;
}

static void script_9D35(void) {
  Script_lightSpotLightEnable(28, &data_1007C, &data_10088);
  Script_LightUtils(28, 2, &data_F258);
  Script_LightUtils(28, 4, 2.1f);
  Script_LightUtils(28, 5, 2.8f);
  Script_LightUtils(28, 6, data_F268);
  Script_LightUtils(28, 10, 4187);
  Script_lightSetSlotEnabled(28, 1);
  Script_scenCreateTask(script_AA64, 0);
  Script_PlayCharaMotion(CHARA_AKIR, 4108, 20.0f, -1, -1, 1.0f, 0);
  Script_ShowUIMessageFromID("E1012A043");
  SCNF_WaitFrames_0000(1);
  script_9640(0x52494B41, 45.0f);
  Script_executeAicaCommand(263593, 0, 0);
  script_9640(0x52494B41, 80.0f);
  Script_clmdSetFlags(CHARA_AKIR, 4, 1);
  script_9640(0x52494B41, 110.0f);
  Script_clmdSetFlags(CHARA_AKIR, 4, 0);
  script_965F(CHARA_AKIR);
  return;
}

static void script_9E2D(void) {
  int32_t local_FC;
  int32_t local_F8;
  local_FC = 0;
  task_258 = 0;
  Script_EVT_SetPosition(SHEN_NAME('DAMY'), POS_XYZ, &data_100A0);
  Script_charaDisplay(SHEN_NAME('KABE'), CDM_Show);
  Script_seqEnterEvtCam(1000, 0, AUTH_None);
  for (;;) {
    Script_charaDisplay(SHEN_NAME('KABE'), CDM_Show);
    local_F8 = local_FC;
    switch (local_F8) {
      case 0:
        Script_PlayCharaMotion(CHARA_AKIR, 4108, 0.0f, 56.0f, 0.0f, 1.0f, 0);
        break;
      case 1:
        Script_motmToggleFlags(CHARA_AKIR, 2, 0x40);
        SCNF_WaitFrames_0000(1);
        Script_seqEnterEvtCam(900, 0, AUTH_None);
        Script_PlayCharaMotion(CHARA_AKIR, 4108, 0.0f, 60.0f, 42.0f, 1.0f, 0);
        break;
      case 2:
        Script_seqEnterEvtCam(702, 0, AUTH_None);
        Script_GetDataFromFightProgram(CHARA_AKIR, 2);
        Script_PlayCharaMotion(CHARA_AKIR, 4115, 30.0f, 90.0f, 25.0f, 1.0f, 0);
        break;
      case 3:
        Script_GetDataFromFightProgram(CHARA_AKIR, 0);
        Script_charaDisplay(SHEN_NAME('KABE'), CDM_Hide);
        Script_PlayCharaMotion(CHARA_AKIR, 4115, 90.0f, -1.0f, 90.0f, 1.0f, 0);
        break;
      case 4:
        Script_charaDisplay(SHEN_NAME('KABE'), CDM_Hide);
        goto L00A0D5;
        break;
    }
    SCNF_WaitFrames_0000(1);
    if (local_FC == 0) {
      script_9640(0x52494B41, 30.0f);
      Script_ShowUIMessageFromID("E1012A045");
    }
    if (local_FC != 3) {
      script_9640(0x52494B41, 55.0f);
      Script_executeAicaCommand(329129, 0, 0);
    }
    if (local_FC == 2) {
      Script_charaDisplay(SHEN_NAME('KABE'), CDM_Hide);
      Script_scenCreateTask(script_E106, 1);
    }
    if (local_FC == 2) {
      script_9640(0x52494B41, 56.0f);
      Script_SetCamShake(0.23f, 0.23f, 0.9f);
      script_9640(0x52494B41, 75.0f);
      Script_scenCreateTask(script_AB96, 0);
    }
    if (local_FC == 3) {
      script_9640(0x52494B41, 60.0f);
      Script_executeAicaCommand(0x1005A9, 0, 0);
      script_9640(0x52494B41, 100.0f);
      Script_fixoClearAttachment(CHARA_ONO_);
      Script_scenCreateTask(script_ACD2, 0);
      Script_seqEnterEvtCam(703, 0, AUTH_None);
      script_9640(0x52494B41, 110.0f);
      Script_executeAicaCommand(984489, 0, 0);
    }
    script_965F(CHARA_AKIR);
    local_FC = local_FC + 1;
  }
  L00A0D5:;
  script_965F(CHARA_AKIR);
  Script_lightSetSlotEnabled(28, 0);
  return;
}

static int32_t script_A0EA(void) {
  int32_t unused_F8[2];
  int32_t local_F4;
  int32_t unused_F0;
  SCNF_RequestIconNotification(0, 0, 0, 2);
  local_F4 = 0;
  (void)((task_250 = 0) & 8);
  task_254 = 4;
  Script_EVT_SetPosition(SHEN_NAME('DAMY'), POS_XYZ, &data_100B8);
  Script_charaToggleMotEnabled(CHARA_AKIR, -1);
  Script_charaDisplay(SHEN_NAME('HKFT'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('HKHK'), CDM_Show);
  (void)((task_250 = -1) & 8);
  Script_charaDisplay(SHEN_NAME('DAMY'), CDM_Hide);
  Script_ResetHandMotion(CHARA_AKIR, 1, 1, 407);
  Script_ResetHandMotion(CHARA_AKIR, 1, 0, 407);
  Script_scenCreateTask(script_A992, 0);
  script_A18D();
  end:;
}

static void script_A18D(void) {
  int32_t unused_F4[3];
  Vector3 local_E8;
  Vector3i local_DC;
  Vector3 local_D0;
  Vector3 local_C4;
  int32_t unused_B8[3];
  Vector3i local_AC;
  int32_t local_A8;
  SCNF_RequestIconNotification(0, 0, 0, 2);
  (void)((task_250 = -1) & 8);
  Script_ccowUtils(CHARA_AKIR, 2);
  Script_ResetHandMotion(CHARA_AKIR, 1, 0, 0);
  Script_ResetHandMotion(CHARA_AKIR, 1, 1, 0);
  Script_lightSpotLightEnable(28, &data_1019C, &data_101A8);
  Script_LightUtils(28, 2, &data_F258);
  Script_LightUtils(28, 4, 2.1f);
  Script_LightUtils(28, 5, 2.8f);
  Script_LightUtils(28, 6, data_F268);
  Script_LightUtils(28, 10, 4187);
  Script_lightSetSlotEnabled(28, 1);
  local_E8.x = 6.202f;
  (&local_E8)->y = 0.0f;
  (&local_E8)->z = 0.821f;
  local_DC.x = 0;
  (&local_DC)->y = 0;
  (&local_DC)->z = 0;
  Script_EVT_SetPosition(SHEN_NAME('HKHK'), POS_XYZ, &local_E8);
  Script_EVT_SetRotation(SHEN_NAME('HKHK'), 0x38000000, &local_DC, 0);
  local_E8.x = 6.349f;
  (&local_E8)->y = -0.021f;
  (&local_E8)->z = 1.037f;
  local_DC.x = 0;
  (&local_DC)->y = 21293;
  (&local_DC)->z = 0;
  Script_EVT_SetPosition(SHEN_NAME('HKFT'), POS_XYZ, &local_E8);
  Script_EVT_SetRotation(SHEN_NAME('HKFT'), 0x38000000, &local_DC, 0);
  Script_charaDisplay(SHEN_NAME('HKFT'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('HKHK'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('HOKY'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('CH2K'), CDM_Show);
  script_D87D(0x52494B41, SHEN_NAME('LIGH'));
  Script_charaDisplay(SHEN_NAME('LIGH'), CDM_Show);
  Script_EVT_SetPosition(SHEN_NAME('LIGH'), POS_XYZ, &data_10154);
  Script_EVT_SetRotation(SHEN_NAME('LIGH'), 0x38000000, &data_10160, 0);
  Script_ccowUtils(CHARA_AKIR, 2);
  Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &data_100D0);
  Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &data_100C4, 0);
  Script_ShowUIMessageFromID("E1012A276");
  Script_PlayCharaMotion(CHARA_AKIR, 4146, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_SetNewHandMotion(CHARA_AKIR, 1, 407, 0, 0, 0, 0, 1.0f, 0);
  Script_SetNewHandMotion(CHARA_AKIR, 0, 407, 0, 0, 0, 0, 1.0f, 0);
  Script_charaSetClipAngle(SHEN_NAME('CH2K'), 15.0f);
  task_258 = 0;
  for (;;) {
    Script_GetCameraInputDetails(15);
    if (Script_IsCharaMotionFinished(CHARA_AKIR) != 0) {
      break;
    }
    local_A8 = task_258;
    switch (local_A8) {
      case 110:
        Script_tmnmUtils(SHEN_NAME('KINK'), 4157, 0.0f, 32767.0f, 0.0f, 1.0f, 0);
        Script_executeAicaCommand(591273, 0, 0);
        break;
      case 200:
        break;
      case 231:
        Script_SetNewHandMotion(CHARA_AKIR, 1, 413, 0, 0, 0, 0, 1.0f, 0);
        break;
      case 312:
        Script_SetNewHandMotion(CHARA_AKIR, 1, 411, 0, 0, 0, 0, 1.0f, 0);
        break;
      case 494:
        Script_SetNewHandMotion(CHARA_AKIR, 1, 407, 0, 0, 0, 0, 1.0f, 0);
        break;
      case 291:
        Script_SetNewHandMotion(CHARA_AKIR, 0, 413, 0, 0, 0, 0, 1.0f, 0);
        break;
      case 385:
        Script_SetNewHandMotion(CHARA_AKIR, 0, 411, 0, 0, 0, 0, 1.0f, 0);
        break;
      case 495:
        Script_SetNewHandMotion(CHARA_AKIR, 0, 407, 0, 0, 0, 0, 1.0f, 0);
        break;
      case 230:
        Script_tmnmUtils(SHEN_NAME('CH2K'), 4158, 0.0f, 32767.0f, 0.0f, 1.0f, 0);
        break;
      case 255:
        Script_ShowUIMessageFromID("E1012A278");
        Script_executeAicaCommand(0x3705A9, 0, 0);
        break;
      case 320:
        Script_executeAicaCommand(0x3705A9, 0, 0);
        break;
      case 390:
        break;
      case 405:
        Script_executeAicaCommand(0x2205A9, 0, 0);
        break;
      case 430:
        Script_executeAicaCommand(0x2405A9, 0, 0);
        break;
      case 470:
        break;
      case 500:
        Script_fixoAttachEx(SHEN_NAME('HOKY'), CHARA_AKIR, 18, &data_1013C, &data_10148);
        Script_charaDisplay(SHEN_NAME('HKFT'), CDM_Show);
        Script_charaDisplay(SHEN_NAME('HKHK'), CDM_Show);
        Script_charaDisplay(SHEN_NAME('CH2K'), CDM_Hide);
        Script_ShowUIMessageFromID("E1012A279");
        break;
    }
    if (task_258 > 600) {
      Script_fixoClearAttachment(SHEN_NAME('LIGH'));
      Script_mtwkGetSetRotation(SHEN_NAME('LIGH'), &local_AC, 0);
      Script_EVT_GetPosition(SHEN_NAME('LIGH'), -1, &local_D0, 0);
      local_C4.x = (float)0 - Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_AC)->x);
      (&local_C4)->y = Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_AC)->y);
      (&local_C4)->z = Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_AC)->z);
      Script_lightSpotLightEnable(28, &local_D0, &local_C4);
      Script_LightUtils(28, 2, &data_F258);
      Script_LightUtils(28, 4, 2.1f);
      Script_LightUtils(28, 5, 2.8f);
      Script_LightUtils(28, 6, data_F268);
      Script_LightUtils(28, 10, 7281);
      Script_lightSetSlotEnabled(28, 1);
      Script_fixoAttachEx(SHEN_NAME('LIGH'), CHARA_AKIR, 12, &data_10184, &data_10190);
    }
    task_258 = task_258 + 1;
    SCNF_WaitFrames_0000(1);
  }
  task_258 = 0;
  Script_ShowUIMessageFromID("E1012A081");
  for (;;) {
    task_258 = task_258 + 1;
    Script_GetCameraInputDetails(15);
    if (task_258 == 60) {
      SCNF_StartIconSystemMenu(5);
    }
    if (task_258 == 120) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &data_100DC, 0);
  Script_fixoAttachEx(SHEN_NAME('HOKY'), CHARA_AKIR, 18, &data_1013C, &data_10148);
  Script_motmToggleFlags(CHARA_AKIR, 1, 0x40);
  Script_PlayCharaMotion(CHARA_AKIR, 4145, 20.0f, 155.0f, -1, 1.0f, 0);
  task_258 = 0;
  for (;;) {
    Script_GetCameraInputDetails(15);
    if (Script_IsCharaMotionFinished(CHARA_AKIR) != 0) {
      break;
    }
    if (task_258 == 60) {
      Script_executeAicaCommand(0x3505A9, 0, 0);
      Script_fixoClearAttachment(SHEN_NAME('HOKY'));
      Script_charaDisplay(SHEN_NAME('HOKY'), CDM_Hide);
    }
    task_258 = task_258 + 1;
    SCNF_WaitFrames_0000(1);
  }
  Script_GetCassetteFlags(5, 1, 0, 0);
  Script_executeAicaCommand(2720, 1, 30);
  Script_executeAicaCommand(2720, 2, 30);
  Script_executeAicaCommand(2720, 3, 30);
  Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  script_D06D(3, 0x4F4D4F4A, 0);
  Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_YDB1_SECRET_BOX_OPENING_COMPLETE, 1);
  Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_YDB1_SECRET_BOX_OPENING_COMPLETE, 1);
  Script_charaDisplay(SHEN_NAME('HOKY'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('HKFT'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('HKHK'), CDM_Hide);
  Script_SetRyoMotionDetails(CHARA_AKIR);
  Script_fixoClearAttachment(SHEN_NAME('LIGH'));
  (void)((task_250 = 0) & 2);
  Script_scenUtils(SCENARIO_CMD_SET_STEP_LUT, 26, 4);
  Script_clmdSetFlags(CHARA_AKIR, 4, 0);
  task_254 = 6;
  (void)((task_250 = -1) & 2);
  Script_SetStats(STATS_PLAYER_CONTEXT_ACTION_MODE, 0);
  script_D87D(0x52494B41, SHEN_NAME('LIGH'));
  task_2E4 = 0;
  SCNF_WaitFrames_0000(1);
  Script_SetRyoMotionDetails(CHARA_AKIR);
  script_DC39();
  Script_shenRequestGameModuleTransition(3, SHEN_NAME('JHD0'), 6);
  SCNF_WaitFrames_0000(1);
  return;
}

static int32_t script_A992(void) {
  Script_seqEnterEvtCam(810, 0, AUTH_None);
  SCNF_WaitFrames_0000(160);
  Script_seqEnterEvtCam(805, 0, AUTH_None);
  SCNF_WaitFrames_0000(90);
  Script_seqEnterEvtCam(806, 0, AUTH_None);
  SCNF_WaitFrames_0000(150);
  Script_seqEnterEvtCam(807, 0, AUTH_None);
  SCNF_WaitFrames_0000(80);
  Script_seqEnterEvtCam(808, 0, AUTH_None);
  SCNF_WaitFrames_0000(360);
  Script_seqEnterEvtCam(809, 0, AUTH_None);
  SCNF_WaitFrames_0000(190);
  Script_convSetCamInactiveFlag(0);
  end:;
}

static void script_AA12(void) {
  Script_seqEnterEvtCam(10, 0, AUTH_None);
  SCNF_WaitFrames_0000(120);
  Script_seqEnterEvtCam(11, 0, AUTH_None);
  SCNF_WaitFrames_0000(195);
  return;
}

static int32_t script_AA3A(void) {
  Script_seqEnterEvtCam(600, 0, AUTH_None);
  SCNF_WaitFrames_0000(120);
  Script_seqEnterEvtCam(601, 0, AUTH_None);
  SCNF_WaitFrames_0000(310);
  end:;
}

static int32_t script_AA64(void) {
  Script_seqEnterEvtCam(1000, 0, AUTH_None);
  end:;
}

static int32_t script_AA72(int32_t arg0) {
  int32_t local_FC;
  Vector3 local_F0;
  Vector3 local_E4;
  Vector3 local_D8;
  Vector3 local_CC;
  local_E4.x = 0.0f;
  (&local_E4)->y = 0.0f;
  (&local_E4)->z = 0.0f;
  Script_EVT_GetPosition(CHARA_AKIR, -1, &local_F0, 0);
  if (arg0 == 1) {
    (&local_F0)->y = (&local_F0)->y + 0.7f;
  }
  local_FC = Script_EffectUtils(0, &local_F0, &local_E4, 10, 0.08f, 0, 0);
  Script_EffectUtils(8, local_FC, 1.25f, 1.25f);
  local_D8.x = 0.00078f;
  (&local_D8)->y = 0.0f;
  (&local_D8)->z = 0.00507f;
  local_CC.x = 0.00468f;
  (&local_CC)->y = 0.0f;
  (&local_CC)->z = 0.0f;
  Script_EffectUtils(6, local_FC, &local_D8, &local_CC);
  Script_EffectUtils(9, local_FC, 46, 49, 46);
  Script_EffectUtils(12, local_FC, 26);
  Script_EffectUtils(10, local_FC, 0.2246f);
  Script_EffectUtils(11, local_FC, 125);
  Script_EffectUtils(8, local_FC, 1.1718f, 1.0546f);
  Script_EffectUtils(1, local_FC);
  SCNF_WaitFrames_0000(150);
  Script_EffectUtils(2, local_FC);
  return Script_EffectUtils(3, local_FC);
}

static int32_t script_AB96(void) {
  int32_t local_FC;
  Vector3 local_F0;
  Vector3 local_E4;
  Vector3 local_D8;
  Vector3 local_CC;
  local_E4.x = 0.0f;
  (&local_E4)->y = 0.0f;
  (&local_E4)->z = 0.0f;
  Script_EVT_GetPosition(SHEN_NAME('KABE'), -1, &local_F0, 0);
  local_FC = Script_EffectUtils(0, &local_F0, &local_E4, 10, 0.08f, 0, 0);
  Script_EffectUtils(8, local_FC, 1.25f, 1.25f);
  local_D8.x = 0.00078f;
  (&local_D8)->y = 0.0f;
  (&local_D8)->z = 0.00507f;
  local_CC.x = 0.00468f;
  (&local_CC)->y = 0.0f;
  (&local_CC)->z = 0.0f;
  Script_EffectUtils(6, local_FC, &local_D8, &local_CC);
  Script_EffectUtils(9, local_FC, 46, 49, 46);
  Script_EffectUtils(12, local_FC, 26);
  Script_EffectUtils(10, local_FC, 0.2246f);
  Script_EffectUtils(11, local_FC, 125);
  Script_EffectUtils(8, local_FC, 1.1718f, 1.0546f);
  Script_EffectUtils(1, local_FC);
  SCNF_WaitFrames_0000(100);
  Script_EffectUtils(2, local_FC);
  return Script_EffectUtils(3, local_FC);
}

static void script_AC9D(void) {
  int32_t local_FC;
  local_FC = 0;
  L00ACA5:;
  Script_EVT_SetPosition(*(&data_101DC + local_FC), POS_XYZ, &data_10224);
  local_FC = local_FC + 1;
  if (!(local_FC >= 18)) goto L00ACA5;
  return;
}

__attribute__((scn_string_bindings(0, data_10230)))
static int32_t script_ACD2(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t local_E4;
  Vector3 local_D8;
  Script_EVT_GetPosition(CHARA_ONO_, -1, &local_D8, 0);
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 2, "MPK00");
  local_E4 = 200;
  L00AD03:;
  Script_InterpolateAndScaleAngles(0, local_E4, &local_F4, &local_E8, "DYN2");
  local_F4.x = (&local_F4)->x + (&local_D8)->x;
  (&local_F4)->y = (&local_F4)->y * (float)10;
  (&local_F4)->z = (&local_D8)->z - (&local_F4)->z * (float)40;
  Script_EVT_SetPosition(CHARA_ONO_, POS_XYZ, &local_F4);
  Script_EVT_SetRotation(CHARA_ONO_, 0x38000000, &local_E8, 0);
  local_E4 = local_E4 + 2;
  SCNF_WaitFrames_0000(1);
  if (!(local_E4 > 289)) goto L00AD03;
  return Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
}

static void script_AD92(void) {
  Vector3i local_F4;
  int32_t unused_DC[6];
  Vector3 local_D0;
  Vector3i local_C4;
  int32_t local_C0;
  int32_t local_BC;
  script_9422();
  Script_GetCassetteFlags_0(56, 1, 0);
  Script_mtwkGetSetRotation(0x306B6E64, &local_C4, 0);
  Script_EVT_GetPosition(0x306B6E64, -1, &local_D0, 0);
  Script_charaDisplay(0x306B6E64, CDM_Show);
  Script_charaDisplay(CHARA_ONO_, CDM_Hide);
  Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &data_10248);
  Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &data_10254, 0);
  Script_motiSetAngFlags(CHARA_AKIR, 0);
  Script_motmToggleFlags(CHARA_AKIR, 2, 0x40);
  Script_seqEnterEvtCam(6300, 0, AUTH_None);
  Script_ResetHandMotion(CHARA_AKIR, 1, 1, 812);
  Script_fixoAttachEx(0x306B6E64, CHARA_AKIR, 18, &data_10260, &data_1026C);
  local_F4.x = 0;
  (&local_F4)->z = 0;
  (&local_F4)->y = 0;
  Script_PlayCharaMotion(CHARA_AKIR, 4151, 163.0f, 186.0f, -1.0f, 1.0f, 0);
  task_258 = 0;
  for (;;) {
    if (task_258 == 24) {
      Script_fixoClearAttachment(0x306B6E64);
      break;
    }
    if (Script_IsCharaMotionFinished(CHARA_AKIR) != 0) {
      break;
    }
    task_258 = task_258 + 1;
    SCNF_WaitFrames_0000(1);
  }
  Script_motiSetAngFlags(CHARA_AKIR, 1);
  Script_motmToggleFlags(CHARA_AKIR, 1, 0x40);
  local_C0 = 0;
  for (;;) {
    local_BC = local_C0 % 30;
    if (local_BC < 20) {
      if (local_BC == 0) {
        Script_executeAicaCommand(0x2105A9, 0, 0);
      }
      local_F4.x = (&local_F4)->x + 455;
    } else {
      local_F4.x = (&local_F4)->x - 910;
    }
    local_C0 = local_C0 + 1;
    if (local_C0 == 120) {
      break;
    }
    Script_SetHandRootRotation(CHARA_AKIR, 1, 0x15, &local_F4);
    SCNF_WaitFrames_0000(1);
  }
  task_2E8 = 1;
  Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_YDB1_FLASHLIGHT_NEEDS_BULB, 1);
  Script_EVT_SetRotation(0x306B6E64, 0x38000000, &local_C4, 0);
  Script_EVT_SetPosition(0x306B6E64, POS_XYZ, &local_D0);
  Script_charaDisplay(CHARA_ONO_, CDM_Show);
  Script_CharHandlers(CHARA_AKIR, 0, 0, 16);
  Script_motiSetAngFlags(CHARA_AKIR, 0);
  Script_motmToggleFlags(CHARA_AKIR, 2, 0x40);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_convSetCamInactiveFlag(1);
  Script_SetGameStateTo(AUTH_None);
  Script_ShowUIMessageFromID(data_10278);
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  script_94B2();
  return;
}

static void script_B019(int32_t arg0) {
  int32_t local_FC;
  int32_t unused_F0[3];
  Vector3 local_E4;
  Script_lightSetSlotEnabled(15, 0);
  script_D87D(0x52494B41, SHEN_NAME('LIGH'));
  Script_lightSpotLightEnable(28, &data_102B4, &data_102C0);
  Script_LightUtils(28, 2, &data_F258);
  Script_LightUtils(28, 4, 2.1f);
  Script_LightUtils(28, 5, 2.8f);
  Script_LightUtils(28, 6, data_F268);
  Script_LightUtils(28, 10, 4187);
  Script_lightSetSlotEnabled(28, 1);
  Script_charaDisplay(CHARA_ONO_, CDM_Hide);
  Script_ccowUtils(CHARA_AKIR, 2);
  Script_ccowUtils(arg0, 2);
  Script_charaDisplay(SHEN_NAME('DOGI'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('SHAS'), CDM_Show);
  Script_motmToggleFlags(CHARA_AKIR, 2, 0x40);
  Script_motiSetAngFlags(CHARA_AKIR, 0);
  Script_PlayCharaMotion(CHARA_AKIR, 4100, 0.0f, -1, -1, 1.0f, 0);
  Script_EVT_GetPosition(SHEN_NAME('FUTA'), -1, &local_E4, 0);
  (&local_E4)->y = 0.45f;
  Script_EVT_SetPosition(SHEN_NAME('FUTA'), POS_XYZ, &local_E4);
  Script_tmnmUtils(SHEN_NAME('FUTA'), 4170, 0.0f, 32767.0f, 0.0f, 1.0f, 0);
  local_FC = 0;
  for (;;) {
    if (local_FC == 40) {
      Script_executeAicaCommand(0x2405A9, 0, 0);
    }
    if (local_FC == 86) {
      Script_executeAicaCommand(0x1105A9, 0, 0);
    }
    if (Script_IsCharaMotionFinished(CHARA_AKIR) != 0) {
      break;
    }
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  Script_motiSetAngFlags(CHARA_AKIR, 1);
  Script_motmToggleFlags(CHARA_AKIR, 1, 0x40);
  return;
}

static void script_B1AD(int32_t arg0) {
  int32_t unused_FC;
  Script_ccowUtils(CHARA_AKIR, 1);
  Script_ccowUtils(arg0, 1);
  Script_charaDisplay(SHEN_NAME('DOGI'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('SHAS'), CDM_Hide);
  Script_lightSetSlotEnabled(28, 0);
  Script_charaDisplay(CHARA_ONO_, CDM_Show);
  if (task_2E4 == 1) {
    Script_lightSetSlotEnabled(15, 1);
    script_D789(CHARA_AKIR, SHEN_NAME('LIGH'));
  }
  Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
  return;
}

static void script_B21F(void) {
  int32_t local_FC;
  Script_ResetHandMotion(CHARA_AKIR, 1, 1, 812);
  Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &data_102FC);
  {
    Vector3i local_F0;
    (&local_F0)->y = 0x4000;
    Script_EVT_SetRotation(CHARA_AKIR, MATRIX_XFORM_Y, &local_F0, 0);
  }
  Script_PlayCharaMotion(CHARA_AKIR, 4129, 245.0f, 305.0f, -1.0f, 1.0f, 0);
  Script_fixoAttachEx(SHEN_NAME('SHAS'), CHARA_AKIR, 18, &data_102E4, &data_102F0);
  Script_seqEnterEvtCam(2102, 0, AUTH_None);
  for (;;) {
    if (Script_IsCharaMotionFinished(CHARA_AKIR) != 0) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  {
    Vector3i local_F0;
    (&local_F0)->y = 0x8000;
    Script_EVT_SetRotation(CHARA_AKIR, MATRIX_XFORM_Y, &local_F0, 0);
  }
  local_FC = 0;
  Script_PlayCharaMotion(CHARA_AKIR, 4130, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  for (;;) {
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
    if (local_FC == 30) {
      Script_seqEnterEvtCam(2103, 0, AUTH_None);
      Script_ShowUIMessageFromID("E1012A074");
      break;
    }
  }
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_B358(int32_t arg0) {
  Vector3 local_F4;
  Vector3 local_E8;
  script_9422();
  Script_GetCassetteFlags_0(54, 1, 0);
  script_D87D(0x52494B41, SHEN_NAME('LIGH'));
  Script_lightSetSlotEnabled(15, 0);
  Script_motmToggleFlags(CHARA_AKIR, 2, 0x40);
  Script_motiSetAngFlags(CHARA_AKIR, 0);
  Script_charaDisplay(CHARA_AKIR, CDM_Show);
  Script_charaDisplay(SHEN_NAME('HROU'), CDM_Show);
  Script_ResetHandMotion(CHARA_AKIR, 1, 1, 812);
  {
    int32_t local_E4;
    local_E4 = arg0;
    switch (local_E4) {
      case 1:
        Script_seqEnterEvtCam(1204, 0, AUTH_None);
        Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &data_10314);
        Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &data_10344, 0);
        Script_PlayCharaMotion(CHARA_AKIR, 4161, 0.0f, 85.0f, -1, 1.0f, 0);
        Script_EVT_GetPosition(SHEN_NAME('ROU1'), -1, &local_E8, 0);
        Script_fixoAttachEx(SHEN_NAME('HROU'), CHARA_AKIR, 18, &data_10374, &data_103A4);
        script_96F1(CHARA_AKIR, 17, (int32_t)&data_103D4, 20, 45);
        break;
      case 2:
        Script_seqEnterEvtCam(1205, 0, AUTH_None);
        Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &data_10320);
        Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &data_10350, 0);
        Script_PlayCharaMotion(CHARA_AKIR, 4159, 0.0f, 85.0f, -1, 1.0f, 0);
        Script_EVT_GetPosition(SHEN_NAME('ROU2'), -1, &local_E8, 0);
        Script_fixoAttachEx(SHEN_NAME('HROU'), CHARA_AKIR, 18, &data_10380, &data_103B0);
        script_96F1(CHARA_AKIR, 17, (int32_t)&data_103E0, 30, 35);
        break;
      case 3:
        Script_seqEnterEvtCam(1206, 0, AUTH_None);
        Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &data_1032C);
        Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &data_1035C, 0);
        Script_PlayCharaMotion(CHARA_AKIR, 4161, 0.0f, 85.0f, -1, 1.0f, 0);
        Script_EVT_GetPosition(SHEN_NAME('ROU3'), -1, &local_E8, 0);
        Script_fixoAttachEx(SHEN_NAME('HROU'), CHARA_AKIR, 18, &data_1038C, &data_103BC);
        script_96F1(CHARA_AKIR, 17, (int32_t)&data_103EC, 25, 40);
        break;
      case 4:
        Script_seqEnterEvtCam(1207, 0, AUTH_None);
        Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &data_10338);
        Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &data_10368, 0);
        Script_PlayCharaMotion(CHARA_AKIR, 4160, 0.0f, 85.0f, -1, 1.0f, 0);
        Script_EVT_GetPosition(SHEN_NAME('ROU4'), -1, &local_E8, 0);
        Script_fixoAttachEx(SHEN_NAME('HROU'), CHARA_AKIR, 18, &data_10398, &data_103C8);
        script_96F1(CHARA_AKIR, 17, (int32_t)&data_103F8, 25, 40);
        break;
    }
  }
  (&local_E8)->y = 0.0f;
  Script_SetLookPointDetails(CHARA_AKIR, 1, &local_E8, 1);
  Script_charaDisplay(SHEN_NAME('HROU'), CDM_Show);
  task_258 = 0;
  SCNF_WaitFrames_0000(50);
  script_94B2();
  Script_fixoClearAttachment(SHEN_NAME('HROU'));
  Script_charaDisplay(SHEN_NAME('HROU'), CDM_Hide);
  {
    int32_t local_E4;
    local_E4 = arg0;
    switch (local_E4) {
      case 1:
        Script_charaDisplay(SHEN_NAME('RO11'), CDM_Show);
        break;
      case 2:
        Script_charaDisplay(SHEN_NAME('RO21'), CDM_Show);
        break;
      case 3:
        Script_charaDisplay(SHEN_NAME('RO31'), CDM_Show);
        break;
      case 4:
        Script_charaDisplay(SHEN_NAME('RO41'), CDM_Show);
        break;
    }
  }
  Script_EVT_GetPosition(CHARA_AKIR, -1, &local_F4, 0);
  (&local_F4)->y = 0.0f;
  Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &local_F4);
  Script_SetCharaLookAtPosition(CHARA_AKIR, -1, 0, 0);
  script_DC39();
  return;
}

static void script_B723(int32_t arg0) {
  Vector3 local_F4;
  script_9422();
  Script_GetCassetteFlags_0(55, 1, 0);
  script_D87D(0x52494B41, SHEN_NAME('LIGH'));
  Script_lightSetSlotEnabled(15, 0);
  Script_motmToggleFlags(CHARA_AKIR, 2, 0x40);
  Script_motiSetAngFlags(CHARA_AKIR, 0);
  Script_ResetHandMotion(CHARA_AKIR, 1, 1, 811);
  Script_ResetHandMotion(CHARA_AKIR, 1, 0, 812);
  {
    int32_t local_F0;
    local_F0 = arg0;
    switch (local_F0) {
      case 1:
        Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &data_1044C);
        Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &data_10488, 0);
        Script_EVT_GetPosition(SHEN_NAME('ROU1'), -1, &local_F4, 0);
        Script_seqEnterEvtCam(1200, 0, AUTH_None);
        Script_PlayCharaMotion(CHARA_AKIR, 4169, 0.0f, 215.0f, -1, 1.0f, 0);
        break;
      case 2:
        Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &data_10458);
        Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &data_10494, 0);
        Script_EVT_GetPosition(SHEN_NAME('ROU2'), -1, &local_F4, 0);
        Script_seqEnterEvtCam(1201, 0, AUTH_None);
        if (*(int16_t *)((int8_t *)task_2FC + 4) > 239) {
          Script_PlayCharaMotion(CHARA_AKIR, 4165, 0.0f, 215.0f, -1, 1.0f, 0);
        } else {
          Script_PlayCharaMotion(CHARA_AKIR, 4166, 0.0f, 215.0f, -1, 1.0f, 0);
        }
        break;
      case 3:
        Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &data_10464);
        Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &data_104A0, 0);
        Script_EVT_GetPosition(SHEN_NAME('ROU3'), -1, &local_F4, 0);
        Script_seqEnterEvtCam(1202, 0, AUTH_None);
        if (*(int16_t *)((int8_t *)task_2FC + 6) > 239) {
          Script_PlayCharaMotion(CHARA_AKIR, 4169, 0.0f, 215.0f, -1, 1.0f, 0);
        } else {
          Script_PlayCharaMotion(CHARA_AKIR, 4167, 0.0f, 215.0f, -1, 1.0f, 0);
        }
        break;
      case 4:
        Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &data_10470);
        Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &data_104AC, 0);
        Script_EVT_GetPosition(SHEN_NAME('ROU4'), -1, &local_F4, 0);
        Script_seqEnterEvtCam(1203, 0, AUTH_None);
        Script_PlayCharaMotion(CHARA_AKIR, 4168, 0.0f, 215.0f, -1, 1.0f, 0);
        break;
    }
  }
  (&local_F4)->y = 0.0f;
  Script_charaDisplay(SHEN_NAME('MAT2'), CDM_Show);
  Script_CharHandlers(CHARA_AKIR, 0, 3, 16);
  Script_charaDisplay(SHEN_NAME('MATB'), CDM_Show);
  Script_fixoAttachEx(SHEN_NAME('MAT2'), CHARA_AKIR, 12, &data_10404, &data_10410);
  Script_fixoAttachEx(SHEN_NAME('MATB'), CHARA_AKIR, 18, &data_10434, &data_10440);
  Script_SetLookPointDetails(CHARA_AKIR, 1, &local_F4, 1);
  task_258 = 0;
  for (;;) {
    int32_t local_F0;
    if (Script_IsCharaMotionFinished(CHARA_AKIR) != 0) {
      break;
    }
    local_F0 = task_258;
    switch (local_F0) {
      case 34:
        Script_executeAicaCommand(0x1905A9, 0, 0);
        break;
      case 44:
        Script_executeAicaCommand(0x1D05A9, 0, 0);
        Script_charaDisplay(SHEN_NAME('FIR4'), CDM_Show);
        Script_fixoAttachEx(SHEN_NAME('FIR4'), CHARA_AKIR, 18, &data_1047C, &data_104B8);
        local_F0 = arg0;
        switch (local_F0) {
          case 1:
            Script_lightSetSlotEnabled(6, 1);
            break;
          case 2:
            Script_lightSetSlotEnabled(7, 1);
            break;
          case 3:
            Script_lightSetSlotEnabled(8, 1);
            break;
          case 4:
            Script_lightSetSlotEnabled(9, 1);
            break;
        }
        break;
      case 46:
        Script_executeAicaCommand(0x3205A9, 0, 0);
        break;
      case 153:
        local_F0 = arg0;
        switch (local_F0) {
          case 1:
            Script_charaDisplay(SHEN_NAME('FIR1'), CDM_Show);
            break;
          case 2:
            Script_charaDisplay(SHEN_NAME('FIR2'), CDM_Show);
            break;
          case 3:
            Script_charaDisplay(SHEN_NAME('FIR3'), CDM_Show);
            break;
          case 4:
            Script_charaDisplay(SHEN_NAME('FIR5'), CDM_Show);
            break;
        }
        break;
    }
    if (-(*(int16_t *)((int8_t *)task_2FC + 2) == 240) & -(arg0 == 2) & -(task_258 > 100)) {
      Script_motiSetAngFlags(CHARA_AKIR, 1);
      Script_SetMotnFlags(CHARA_AKIR, 17, &data_104C4, 0x78000000);
    }
    task_258 = task_258 + 1;
    SCNF_WaitFrames_0000(1);
  }
  script_94B2();
  Script_charaDisplay(SHEN_NAME('FIR4'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('MAT2'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('MATB'), CDM_Hide);
  Script_fixoClearAttachment(SHEN_NAME('FIR4'));
  Script_fixoClearAttachment(SHEN_NAME('MAT2'));
  Script_fixoClearAttachment(SHEN_NAME('MATB'));
  Script_SetCharaLookAtPosition(CHARA_AKIR, -1, 0, 0);
  Script_EVT_GetPosition(CHARA_AKIR, -1, &task_3E8, 0);
  task_3E8.y = 0.0f;
  Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &task_3E8);
  script_DC39();
  script_90CD(arg0 - 1);
  return;
}

static void script_BC3F(void) {
  int32_t unused_FC;
  int32_t local_F4[2];
  int32_t local_F0;
  int32_t local_EC;
  Vector3 local_E0;
  int32_t local_DC;
  Vector3i local_D0;
  local_D0.x = 0;
  (&local_D0)->y = 0;
  script_9422();
  Script_ResetHandMotion(CHARA_AKIR, 1, 1, 411);
  Script_ResetHandMotion(CHARA_AKIR, 1, 0, 411);
  script_D87D(0x52494B41, SHEN_NAME('LIGH'));
  Script_lightSetSlotEnabled(15, 0);
  Script_charaDisplay(CHARA_ONO_, CDM_Hide);
  local_F4[0] = 0;
  local_F4[1] = 0;
  local_F0 = 0;
  Script_scenToggleVisibleFor(SHEN_NAME('YKUR'), 1);
  Script_scenToggleVisibleFor(SHEN_NAME('YKUL'), 1);
  task_388 = 2;
  local_EC = 0;
  Script_EVT_SetPosition(SHEN_NAME('LEAF'), POS_XYZ, &data_104DC);
  Script_InvestigationUtils(ShenmueUtils_BeginTargetModeDefault, SHEN_NAME('DENW'));
  for (;;) {
    if (Script_InvestigationUtils(ShenmueUtils_UpdateStateMachine) != 0) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  local_DC = 0;
  for (;;) {
    local_DC = local_DC + 1;
    if (local_DC == 20) {
      break;
    }
    Script_InvestigationUtils(ShenmueUtils_UpdateInterpolatedSpeed);
    SCNF_WaitFrames_0000(1);
  }
  Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &data_1050C);
  {
    int32_t unused_B0[8];
    Vector3i local_A4;
    (&local_A4)->y = 0xC000;
    Script_EVT_SetRotation(CHARA_AKIR, MATRIX_XFORM_Y, &local_A4, 0);
  }
  SCNF_WaitFrames_0000(1);
  Script_SetLookPointDetails(CHARA_AKIR, 12, &data_10530, 1);
  (&local_D0)->z = -0x8000;
  Script_charaPosRotUtils(SHEN_NAME('DENW'), 152, &data_1053C, &local_D0, 1);
  Script_ShowUIMessageFromID("E1012A119");
  Script_ccowUtils(CHARA_AKIR, 2);
  Script_lightSetSlotEnabled(28, 0);
  Script_motmToggleFlags(CHARA_AKIR, 2, 0x40);
  Script_motiSetAngFlags(CHARA_AKIR, 0);
  Script_seqEnterEvtCam(6000, 0, AUTH_None);
  Script_charaDisplay(CHARA_AKIR, CDM_Show);
  Script_PlayCharaMotion(CHARA_AKIR, 4129, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_charaDisplay(SHEN_NAME('YKUR'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('YKUL'), CDM_Hide);
  Script_fixoAttachEx(SHEN_NAME('DENW'), CHARA_AKIR, 12, &data_104F4, &data_10500);
  Script_EVT_GetPosition(SHEN_NAME('LEAF'), -1, &local_E0, 0);
  Script_charaDisplay(SHEN_NAME('LEAF'), CDM_Show);
  task_258 = 0;
  for (;;) {
    int32_t unused_B0[8];
    int32_t local_AC;
    if ((task_258 > 35) & (task_258 < 135)) {
      (&local_E0)->y = (&local_E0)->y + (*(float *)(&data_104E8 + 1) - (&data_104DC)->y) / (float)100;
      local_E0.x = (&local_E0)->x + (*(float *)&data_104E8 - (&data_104DC)->x) / (float)100;
      (&local_E0)->z = (&local_E0)->z + (*(float *)(&data_104E8 + 2) - (&data_104DC)->z) / (float)100;
    }
    if (Script_IsCharaMotionFinished(CHARA_AKIR) != 0) {
      break;
    }
    local_AC = task_258;
    switch (local_AC) {
      case 35:
        Script_ResetHandMotion(CHARA_AKIR, 1, 1, 402);
        Script_scenCreateTask(script_DD28, 0);
        break;
      case 80:
        Script_SetCharaLookAtPosition(CHARA_AKIR, -1, 0, 0);
        Script_seqEnterEvtCam(6001, 0, AUTH_None);
        break;
      case 150:
        Script_ResetHandMotion(CHARA_AKIR, 1, 1, 411);
        Script_seqEnterEvtCam(6002, 0, AUTH_None);
        break;
      case 250:
        Script_ResetHandMotion(CHARA_AKIR, 1, 1, 811);
        Script_fixoAttachEx(SHEN_NAME('LEAF'), CHARA_AKIR, 18, &data_10518, &data_10524);
        break;
    }
    task_258 = task_258 + 1;
    SCNF_WaitFrames_0000(1);
  }
  Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 226, 1);
  Script_motiSetAngFlags(CHARA_AKIR, 1);
  Script_motmToggleFlags(CHARA_AKIR, 1, 0x40);
  {
    int32_t unused_B0[8];
    Vector3i local_A4;
    (&local_A4)->y = 0;
    Script_EVT_SetRotation(CHARA_AKIR, MATRIX_XFORM_Y, &local_A4, 0);
  }
  Script_PlayCharaMotion(CHARA_AKIR, 4130, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_ShowUIMessageFromID("E1012A177");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_ShowUIMessageFromID("E1012A178");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_GetCassetteFlags(40, 1, 0, 0);
  SCNF_StartIconSystemMenu(40);
  (&local_D0)->z = 0x8000;
  Script_charaPosRotUtils(SHEN_NAME('DENW'), 152, &data_1053C, &local_D0, 1);
  Script_fixoClearAttachment(SHEN_NAME('DENW'));
  Script_charaDisplay(SHEN_NAME('YKUR'), CDM_Show);
  Script_lightSetSlotEnabled(28, 1);
  Script_InvestigationUtils(ShenmueUtils_SetupOrSyncAnchors, 0, 0);
  script_94B2();
  Script_charaDisplay(CHARA_ONO_, CDM_Show);
  Script_charaDisplay(SHEN_NAME('LEAF'), CDM_Hide);
  Script_scenToggleVisibleFor(SHEN_NAME('YKUR'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('YKUL'), 0);
  Script_lightSetSlotEnabled(28, 0);
  Script_srchUtils(SRCH_SCRIPT_Reset);
  return;
}

static void script_C0FC(void) {
  script_9422();
  Script_charaDisplay(CHARA_ONO_, CDM_Hide);
  Script_lightSpotLightEnable(28, &data_1061C, &data_10628);
  Script_LightUtils(28, 2, &data_F258);
  Script_LightUtils(28, 4, 2.1f);
  Script_LightUtils(28, 5, 2.8f);
  Script_LightUtils(28, 6, data_F268);
  Script_LightUtils(28, 10, 4187);
  Script_lightSetSlotEnabled(28, 1);
  Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &data_10568);
  Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &data_10574, 0);
  script_D87D(0x52494B41, SHEN_NAME('LIGH'));
  Script_lightSetSlotEnabled(15, 0);
  Script_ResetHandMotion(CHARA_AKIR, 1, 1, 411);
  Script_ResetHandMotion(CHARA_AKIR, 1, 0, 411);
  Script_scenCreateTask(script_C7F8, 0);
  task_258 = 0;
  Script_motmToggleFlags(CHARA_AKIR, 2, 0x40);
  Script_motiSetAngFlags(CHARA_AKIR, 0);
  Script_PlayCharaMotion(CHARA_AKIR, 4140, 0.0f, -1, -1, 1.0f, 0);
  script_D87D(0x52494B41, SHEN_NAME('LIGH'));
  Script_charaDisplay(0x746E6564, CDM_Show);
  Script_EVT_SetPosition(0x746E6564, POS_XYZ, &data_10604);
  Script_EVT_SetRotation(0x746E6564, 0x38000000, &data_10610, 0);
  for (;;) {
    Vector3 local_F4;
    int32_t local_F0;
    if (Script_IsCharaMotionFinished(CHARA_AKIR) != 0) {
      break;
    }
    local_F0 = task_258;
    switch (local_F0) {
      case 80:
        local_F4.x = 0.0f;
        (&local_F4)->z = 0.0f;
        (&local_F4)->y = 0.0f;
        Script_EVT_SetPosition(SHEN_NAME('BX4B'), POS_XYZ, &local_F4);
        Script_tmnmUtils(SHEN_NAME('BX4B'), 4171, 0.0f, 32767.0f, 0.0f, 1.0f, 0);
        break;
      case 92:
        Script_executeAicaCommand(0x2605A9, 0, 0);
        break;
      case 150:
        Script_executeAicaCommand(0x2205A9, 0, 0);
        break;
      case 195:
        Script_fixoAttachEx(SHEN_NAME('PIPE'), CHARA_AKIR, 18, &data_10580, &data_1058C);
        Script_executeAicaCommand(0x3305A9, 0, 0);
        break;
      case 307:
        Script_fixoClearAttachment(SHEN_NAME('PIPE'));
        Script_fixoAttachEx(SHEN_NAME('PIPE'), CHARA_AKIR, 12, &data_10598, &data_105A4);
        break;
      case 380:
        Script_executeAicaCommand(0x1505A9, 0, 0);
        Script_fixoClearAttachment(SHEN_NAME('PIPE'));
        Script_charaDisplay(SHEN_NAME('PIPE'), CDM_Hide);
        break;
      case 530:
        Script_fixoClearAttachment(SHEN_NAME('PIPE'));
        Script_charaDisplay(SHEN_NAME('PIPE'), CDM_Hide);
        Script_EVT_SetPosition(SHEN_NAME('PIPE'), POS_XYZ, &data_105C8);
        break;
    }
    task_258 = task_258 + 1;
    SCNF_WaitFrames_0000(1);
  }
  Script_motiSetAngFlags(CHARA_AKIR, 1);
  Script_motmToggleFlags(CHARA_AKIR, 1, 0x40);
  task_258 = 0;
  Script_PlayCharaMotion(CHARA_AKIR, 4133, -1, -1, -1, 1.0f, 0);
  {
    int32_t unused_F4[3];
    Vector3i local_E8;
    (&local_E8)->y = 0x8000;
    Script_EVT_SetRotation(CHARA_AKIR, MATRIX_XFORM_Y, &local_E8, 0);
  }
  Script_seqEnterEvtCam(1110, SHEN_NAME('BX4A'), SHEN_NAME('BX4A'));
  Script_EVT_SetPosition(SHEN_NAME('KAK1'), POS_XYZ, &data_105D4);
  Script_EVT_SetRotation(SHEN_NAME('KAK1'), 0x38000000, &data_105E0, 0);
  Script_charaDisplay(SHEN_NAME('KAK1'), CDM_Show);
  for (;;) {
    task_258 = task_258 + 1;
    if (task_258 == 150) {
      break;
    }
    if (task_258 == 50) {
      Script_ShowUIMessageFromID("E1012A208");
      while (Script_convGetIsActive(0)) {
        SCNF_WaitFrames_0000(1);
      }
      Script_ShowUIMessageFromID("E1012A209");
      SCNF_WaitFrames_0000(1);
      Script_seqEnterEvtCam(1111, SHEN_NAME('BX4A'), SHEN_NAME('BX4A'));
    }
    SCNF_WaitFrames_0000(1);
  }
  task_258 = 0;
  Script_PlayCharaMotion(CHARA_AKIR, 4137, -1, -1, -1, 1.0f, 0);
  for (;;) {
    if (task_258 == 27) {
      Script_seqEnterEvtCam(1120, SHEN_NAME('BX4A'), SHEN_NAME('BX4A'));
      Script_EVT_SetPosition(SHEN_NAME('KAK1'), POS_XYZ, &data_105EC);
      Script_EVT_SetRotation(SHEN_NAME('KAK1'), 0x38000000, &data_105F8, 0);
      Script_charaDisplay(SHEN_NAME('KAK1'), CDM_Hide);
    }
    if (Script_IsCharaMotionFinished(CHARA_AKIR) != 0) {
      break;
    }
    task_258 = task_258 + 1;
    SCNF_WaitFrames_0000(1);
  }
  task_258 = 0;
  Script_PlayCharaMotion(CHARA_AKIR, 4136, -1, -1, -1, 1.0f, 0);
  Script_executeAicaCommand(0x3805A9, 0, 0);
  for (;;) {
    task_258 = task_258 + 1;
    if (task_258 == 100) {
      Script_executeAicaCommand(0x3905A9, 0, 0);
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  task_258 = 0;
  Script_PlayCharaMotion(CHARA_AKIR, 4139, -1, -1, -1, 1.0f, 0);
  Script_fixoAttachEx(SHEN_NAME('PIPE'), CHARA_AKIR, 12, &data_105B0, &data_105BC);
  for (;;) {
    Vector3 local_F4;
    int32_t local_F0;
    if (Script_IsCharaMotionFinished(CHARA_AKIR) != 0) {
      break;
    }
    local_F0 = task_258;
    switch (local_F0) {
      case 20:
        Script_charaDisplay(SHEN_NAME('PIPE'), CDM_Show);
        break;
      case 62:
        Script_seqEnterEvtCam(1121, SHEN_NAME('BX4A'), SHEN_NAME('BX4A'));
        break;
      case 142:
        Script_fixoClearAttachment(SHEN_NAME('PIPE'));
        break;
      case 162:
        Script_seqEnterEvtCam(1122, SHEN_NAME('BX4A'), SHEN_NAME('BX4A'));
        break;
      case 182:
        local_F4.x = 0.0f;
        (&local_F4)->z = 0.0f;
        (&local_F4)->y = 0.0f;
        Script_EVT_SetPosition(SHEN_NAME('BX4B'), POS_XYZ, &local_F4);
        Script_executeAicaCommand(0x2205A9, 0, 0);
        Script_tmnmUtils(SHEN_NAME('BX4B'), 4172, 0.0f, 32767.0f, 0.0f, 1.0f, 0);
        break;
      case 230:
        Script_executeAicaCommand(0x2705A9, 0, 0);
        break;
      case 250:
        break;
      case 320:
        break;
    }
    task_258 = task_258 + 1;
    SCNF_WaitFrames_0000(1);
  }
  Script_CharHandlers(CHARA_AKIR, 0, 13, 16);
  Script_CharHandlers(CHARA_AKIR, 1, 13, 16);
  Script_EVT_SetPosition(0x746E6564, POS_XYZ, &data_105EC);
  Script_EVT_SetRotation(0x746E6564, 0x38000000, &data_105F8, 0);
  Script_charaDisplay(0x746E6564, CDM_Hide);
  Script_lightSetSlotEnabled(28, 0);
  Script_tmnmReset(SHEN_NAME('BX4B'));
  Script_charaDisplay(CHARA_ONO_, CDM_Show);
  {
    Vector3 local_F4;
    local_F4.x = 2.126f;
    (&local_F4)->y = 0.489f;
    (&local_F4)->z = 0.922f;
    Script_EVT_SetPosition(SHEN_NAME('BX4B'), POS_XYZ, &local_F4);
  }
  Script_CreateNPCImageModule(SHEN_NAME('BX4B'), 0, 0);
  Script_CreateNPCImageModule(SHEN_NAME('BX4B'), "HAKO_4B", 0);
  script_94B2();
  return;
}

static int32_t script_C7F8(void) {
  Script_seqEnterEvtCam(1106, SHEN_NAME('BX4A'), SHEN_NAME('BX4A'));
  SCNF_WaitFrames_0000(70);
  Script_seqEnterEvtCam(1107, SHEN_NAME('BX4A'), SHEN_NAME('BX4A'));
  SCNF_WaitFrames_0000(210);
  Script_seqEnterEvtCam(1108, SHEN_NAME('BX4A'), SHEN_NAME('BX4A'));
  SCNF_WaitFrames_0000(100);
  Script_seqEnterEvtCam(1109, SHEN_NAME('BX4A'), SHEN_NAME('BX4A'));
  end:;
}

static void script_C858(void) {
  int32_t unused_F4[3];
  Vector3 local_E8;
  Vector3 local_DC;
  Vector3 local_D0;
  Vector3 local_C4;
  Vector3 local_B8;
  Vector3 local_AC;
  Vector3 local_A0;
  Vector3 local_94;
  Vector3 local_88;
  Vector3 local_FF7C;
  Vector3 local_FF70;
  Vector3 local_FF64;
  script_9422();
  Script_filmCheckOrSetTransitionUtil(15, 0, 0, 0, 0, 255, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  script_D87D(0x52494B41, SHEN_NAME('LIGH'));
  Script_lightSetSlotEnabled(15, 0);
  Script_scenUtils(SCENARIO_CMD_SET_STEP_LUT, 26, 1);
  Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &data_10650);
  Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &data_1065C, 0);
  SCNF_WaitFrames_0000(1);
  Script_filmCheckOrSetTransitionUtil(15, 255, 0, 0, 0, 0, 0, 0, 0);
  Script_seqEnterEvtCam(1112, 0, AUTH_None);
  SCNF_WaitFrames_0000(1);
  task_258 = 120;
  Script_motmToggleFlags(CHARA_AKIR, 2, 0x40);
  Script_motiSetAngFlags(CHARA_AKIR, 0);
  Script_PlayCharaMotion(CHARA_AKIR, 4135, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  task_258 = 0;
  Script_EVT_GetPosition(SHEN_NAME('TANR'), -1, &local_D0, 0);
  Script_EVT_GetPosition(SHEN_NAME('BALL'), -1, &local_B8, 0);
  Script_EVT_GetPosition(SHEN_NAME('UBOX'), -1, &local_A0, 0);
  Script_EVT_GetPosition(SHEN_NAME('YAK2'), -1, &local_88, 0);
  Script_EVT_GetPosition(SHEN_NAME('FUDA'), -1, &local_FF70, 0);
  (&local_C4)->z = (&local_D0)->z;
  (&local_C4)->y = 0.0f;
  (&local_94)->z = (&local_A0)->z;
  (&local_94)->y = (&local_A0)->y;
  (&local_AC)->z = (&local_B8)->z;
  (&local_AC)->y = (&local_B8)->y;
  (&local_FF7C)->z = (&local_88)->z;
  (&local_FF7C)->y = (&local_88)->y;
  (&local_FF64)->y = (&local_FF70)->y;
  (&local_FF64)->z = (&local_FF70)->z;
  local_C4.x = 8.0f;
  Script_fixoAttachEx(SHEN_NAME('DAMY'), CHARA_AKIR, 18, &data_10668, &data_10674);
  task_258 = 0;
  for (;;) {
    if (Script_IsCharaMotionFinished(CHARA_AKIR) != 0) {
      break;
    }
    {
      int32_t unused_FF58[3];
      int32_t local_FF54;
      local_FF54 = task_258;
      if (local_FF54 == 90) {
        Script_seqEnterEvtCam(1116, 0, AUTH_None);
        goto L00CA80;
      }
    }
    L00CA80:;
    Script_motiSetAngFlags(CHARA_AKIR, 1);
    Script_motmToggleFlags(CHARA_AKIR, 1, 0x40);
    if (task_258 == 85) {
      Script_EVT_GetPosition(SHEN_NAME('DAMY'), -1, &local_E8, 0);
    }
    if ((task_258 > 84) & (task_258 < 348)) {
      Script_EVT_GetPosition(SHEN_NAME('DAMY'), -1, &local_DC, 0);
      local_DC.x = (&local_DC)->x - (&local_E8)->x;
      if (!((&local_DC)->x + (&local_D0)->x >= (&local_C4)->x)) {
        local_C4.x = (&local_D0)->x + (&local_DC)->x;
        local_AC.x = (&local_B8)->x + (&local_DC)->x;
        local_94.x = (&local_A0)->x + (&local_DC)->x;
        local_FF7C.x = (&local_88)->x + (&local_DC)->x;
        local_FF64.x = (&local_FF70)->x + (&local_DC)->x;
      }
      Script_EVT_SetPosition(SHEN_NAME('TANR'), POS_XYZ, &local_C4);
      Script_EVT_SetPosition(SHEN_NAME('BALL'), POS_XYZ, &local_AC);
      Script_EVT_SetPosition(SHEN_NAME('UBOX'), POS_XYZ, &local_94);
      Script_EVT_SetPosition(SHEN_NAME('YAK2'), POS_XYZ, &local_FF7C);
      Script_EVT_SetPosition(SHEN_NAME('FUDA'), POS_XYZ, &local_FF64);
    }
    {
      int32_t unused_FF58[3];
      int32_t local_FF54;
      local_FF54 = task_258;
      switch (local_FF54) {
        case 120:
          Script_executeAicaCommand(0x1A05A9, 0, 0);
          break;
        case 170:
          Script_executeAicaCommand(0x1A05A9, 0, 0);
          break;
        case 220:
          Script_executeAicaCommand(0x1A05A9, 0, 0);
          break;
        case 267:
          Script_executeAicaCommand(0x1A05A9, 0, 0);
          break;
        case 320:
          Script_executeAicaCommand(0x1A05A9, 0, 0);
          break;
      }
    }
    SCNF_WaitFrames_0000(1);
    task_258 = task_258 + 1;
  }
  Script_EVT_SetPosition(SHEN_NAME('MUKA'), POS_XYZ, &data_1068C);
  Script_fixoClearAttachment(SHEN_NAME('DAMY'));
  local_E8.x = 3.15f;
  (&local_E8)->y = 0.0f;
  (&local_E8)->z = 0.68f;
  Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_SHELF_MOVED, 1);
  Script_EVT_SetPosition(SHEN_NAME('DAMY'), POS_XYZ, &data_10680);
  script_94B2();
  Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &local_E8);
  if ((Script_GetCurrentEntryPoint() == 3) | (Script_GetCurrentEntryPoint() == 5)) {
    SAVE_Script_UpdateForSaveData(3, 0, 300);
  } else {
    SAVE_Script_UpdateForSaveData(3, 0, (8 - Script_GetCurrentEntryPoint() * 3) * 1800);
  }
  return;
}

static void script_CCC1(void) {
  Vector3 local_F4;
  int32_t local_E8[3];
  int32_t unused_E4;
  Script_ryoControl(PCCTL_DisablePlayerControl);
  local_F4.x = 0.6872f;
  (&local_F4)->y = 0.0f;
  (&local_F4)->z = 3.3065f;
  local_E8[0] = 0;
  local_E8[1] = 0x4000;
  local_E8[2] = 0;
  Script_srchUtils(SRCH_SCRIPT_Reset);
  Script_SetGameStateTo(AUTH_None);
  Script_scenCharaSubmitMoveTargetRequest(CHARA_AKIR, &local_F4, 0x4000, MN_AKI_AKI_STAND_L, 0);
  while (Script_IsMoveTargetRequestActive(CHARA_AKIR) != 0) {
    SCNF_WaitFrames_0000(1);
    Script_GetCameraInputDetails(15);
  }
  Script_PlayCharaMotion(CHARA_AKIR, 4147, 55.0f, 140.0f, -1, 1.5f, 0);
  script_9640(0x52494B41, 100.0f);
  Script_executeAicaCommand(0x2005A9, 0, 0);
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 890) == 0) {
    Script_lightSetSlotEnabled(2, 1);
  } else {
    Script_charaDisplay(0x316B6E64, CDM_Hide);
    Script_charaDisplay(0x306B6E64, CDM_Show);
    Script_lightSetSlotEnabled(2, 0);
    Script_lightSetSlotEnabled(3, 0);
  }
  script_965F(CHARA_AKIR);
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 890) == 0) {
    Script_charaDisplay(0x306B6E64, CDM_Hide);
    Script_charaDisplay(0x316B6E64, CDM_Show);
    Script_lightSetSlotEnabled(2, 1);
    Script_lightSetSlotEnabled(3, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 890, 1);
    task_2F0 = 0;
  } else {
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 890, 0);
  }
  Script_motiSetAngFlags(CHARA_AKIR, 1);
  Script_motmToggleFlags(CHARA_AKIR, 1, 0x40);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_ryoControl(RYO_RestoreControl);
  return;
}

static void script_CE68(void) {
  int32_t local_FC;
  int32_t local_F8;
  local_FC = 1;
  local_F8 = 0;
  for (;;) {
    if (local_FC % 6 == 0) {
      Script_lightSetSlotEnabled(2, 1);
    } else {
      Script_lightSetSlotEnabled(2, 0);
    }
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
    if (local_FC == 40) {
      break;
    }
  }
  return;
}

void script_CEB6(void) {
  Script_filmCheckOrSetTransitionUtil(0, 255, 0, 0, 0, 0, 0, 0, 0);
  Script_ryoSleepUtils(2, 1);
  SCNT_Enqueue();
  Script_MotStepUtil(CHARA_AKIR, 260);
  task_2F4 = Script_ryoSleepUtils(0);
  Script_SetMapFlags(0, 0);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 15.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 10.0f);
  Script_SetCameraFieldOfView(10922);
  task_318 = 0;
  task_2B8 = 0;
  task_2D4 = 0;
  task_2D0 = 0;
  task_2CC = 0;
  task_2E8 = 0;
  Script_cpctStateUtils(1, 2);
  Script_SetDynamicMapTransform(1, &data_E930, 0);
  Script_mapmSetCamFlags(1, 1, 0);
  Script_mapmSetCamFlags(1, 0, 0);
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_YDB1_SECRET_BOX_OPENING_COMPLETE)) {
    Script_SetShouldSleepFlag(0);
    script_D06D(3, 0x4F4D4F4A, 0);
    Script_SetShouldSleepFlag(1);
  } else {
    Script_SetShouldSleepFlag(0);
    script_D06D(3, 0x4F4D4F4A, 1);
    Script_SetShouldSleepFlag(1);
  }
  Script_charaDisplay(SHEN_NAME('FIR1'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('FIR2'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('FIR3'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('FIR5'), CDM_Hide);
  Script_motmToggleFlags(CHARA_AKIR, 2, 8);
  Script_ryoControl(PCCTL_SetCPCTFlag400, 1);
  Script_LoadVoiceFile(1, data_10698);
  Script_SetStats(STATS_PLAYER_CONTEXT_ACTION_MODE, 0);
  script_8753();
  Script_scenCreateTask(script_D14B, 1, CHARA_AKIR);
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_YDB1_WAKE_FROM_SLEEP_CUTSCENE_SEEN) == 0) {
    Script_scenUtils(SCENARIO_CMD_SET_STEP_LUT, 21, 0);
    Script_scenUtils(SCENARIO_CMD_SET_STEP_LUT, 22, 48);
    Script_scenUtils(SCENARIO_CMD_SET_STEP_LUT, 26, 0);
    Script_scenUtils(SCENARIO_CMD_SET_STEP_LUT, 20, 255);
  }
  if (task_2F4 != -1) {
    script_DA3B();
  }
  return;
}

static void script_D06D(int32_t arg0, int32_t arg1, int32_t arg2) {
  int32_t unused_FC;
  GameDateTime local_F4;
  int32_t local_D8[7];
  Script_ReturnTimeDate(&local_F4);
  local_D8[0] = 22;
  local_D8[1] = 30;
  if ((Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_DKTY_TUBA_CUTSCENE_COMPLETE) == 1) & (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_YDB1_SECRET_BOX_OPENING_COMPLETE) == 0)) {
    if (arg2) {
      if (((&local_F4)->hours >= 0) & ((&local_F4)->hours <= 4)) {
        local_D8[0] = 3;
        local_D8[1] = 30;
        local_D8[2] = 4;
        local_D8[3] = 0;
      } else {
        local_D8[0] = 27;
        local_D8[1] = 30;
        local_D8[2] = 28;
        local_D8[3] = 0;
      }
    } else {
      local_D8[0] = 22;
      local_D8[1] = 30;
      local_D8[2] = 23;
      local_D8[3] = 30;
    }
    local_D8[4] = arg0;
    local_D8[5] = arg1;
    local_D8[6] = 4;
    Script_SetHomeArea((const void *)local_D8);
  }
  return;
}

__attribute__((scn_string_bindings(0, data_10748, 3, data_10768)))
static int32_t script_D14B(int32_t arg0) {
  int32_t local_E8[6];
  Vector3i local_DC;
  int32_t unused_D8;
  Vector3 local_CC;
  int32_t unused_BC[4];
  int32_t local_B8;
  int32_t local_B4;
  task_658 = 0;
  task_2E4 = 0;
  task_320 = 0;
  task_25C = 0;
  local_B4 = 0;
  (void)((task_250 = 0) & 4);
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 2, "MPK00");
  Script_LoadHandMot(3, SHEN_NAME('YDB1'), "HMOT_DB1.BIN");
  Script_LoadHandMot(3, SHEN_NAME('YDB1'), "HMOT_STD.BIN");
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
  Script_facemotUtils(arg0, 1, 0);
  Script_charaDisplay(arg0, CDM_Show);
  Script_ryoSetChara(arg0);
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_clmdSetFlags(arg0, 2, 0);
  Script_facemotUtils(arg0, 0, 0);
  Script_FaceEx(arg0, 1, 0, 1);
  Script_FACE_LCG(arg0, 0, 10, 0);
  script_0FBF();
  Script_charaSetClipAngle(SHEN_NAME('TANR'), 15.0f);
  Script_charaSetClipAngle(SHEN_NAME('UBOX'), 15.0f);
  Script_charaSetClipAngle(SHEN_NAME('BALL'), 15.0f);
  Script_charaSetClipAngle(SHEN_NAME('FUDA'), 15.0f);
  local_E8[0] = 0x40FE2824;
  local_E8[1] = 0x3F98BE0E;
  local_E8[2] = 0x3FB91340;
  local_E8[3] = 0x3F800000;
  local_E8[4] = 0;
  local_E8[5] = 0;
  local_DC.x = 0;
  (&local_DC)->y = 0;
  (&local_DC)->z = 0;
  Script_EVT_GetPosition(CHARA_AKIR, -1, &local_CC, 0);
  if (task_2F4 != -1) {
    Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
    script_DDE3(CHARA_AKIR);
  } else {
    Script_ryoSleepUtils(1);
    script_E34E();
    Script_hmdlApplyScale(SHEN_NAME('UBOX'), &data_106AC);
    Script_charaSetClipAngle(SHEN_NAME('TANR'), 15.0f);
    if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_YDB1_FLASHLIGHT_NEEDS_BULB)) {
      Script_charaDisplay(0x306B6E64, CDM_Show);
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 890)) {
        Script_charaDisplay(0x316B6E64, CDM_Show);
        Script_lightSetSlotEnabled(2, 1);
        Script_lightSetSlotEnabled(3, 1);
        Script_scenToggleCharaFlag(0, 100, 0);
      }
    }
    Script_scenCreateTask(script_8891, 0);
    Script_scenCreateTask(script_294C, 0);
    Script_scenCreateTask(script_3A93, 0);
    script_D908();
    SCNF_WaitFrames_0000(30);
    Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
    Script_shenClearFreeState(1);
  }
  Script_CreateNPCImageModule(SHEN_NAME('DAMY'), 0, 0);
  if (task_2E4) {
    Script_srchUtils(SRCH_SCRIPT_SetEntrySelectorFilter, 0);
    task_2E4 = 1;
    script_279C(1);
    script_D789(CHARA_AKIR, SHEN_NAME('LIGH'));
    Script_lightSetSlotEnabled(15, 1);
    SCNF_WaitFrames_0000(1);
    Script_uiUtils(0, 26, 0);
    Script_CharHandlers(CHARA_AKIR, 1, 3, 16);
    Script_scenToggleCharaFlag(0, 100, 0);
    Script_GetBatteryLifePair(BATTERY_SET_DECREMENT_ENABLED, 0, 1);
  }
  if (task_2CC) {
    Script_CharHandlers(CHARA_AKIR, 0, 0, 16);
    Script_fixoAttachEx(CHARA_ONO_, CHARA_AKIR, 18, &data_10730, &data_1073C);
  }
  task_25C = 0;
  SCNF_WaitFrames_0000(1);
  Script_ryoControl(RYO_RestoreControl);
  local_B8 = 0;
  while (task_2B8 == 0) {
    if (Script_SetTaskDataValue(CHARA_AKIR, &data_106C4, &data_106D0, 0) & Script_IsPlayerAllowed()) {
      if (local_B4 == 1) {
        Script_SetShouldSleepFlag(0);
        Script_ryoControl(PCCTL_DisablePlayerControl);
        Script_FaceEx(CHARA_AKIR, 1, 0, 2);
        Script_ResetHandMotion(CHARA_AKIR, -1, 0, 0);
        Script_ResetHandMotion(CHARA_AKIR, -1, 1, 0);
        Script_shenClearFreeState(0);
        Script_executeAicaCommand(2720, 1, 30);
        Script_executeAicaCommand(2720, 2, 30);
        Script_executeAicaCommand(2720, 3, 30);
        Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
        while (Script_filmCheckOrSetTransitionUtil(-1)) {
          SCNF_WaitFrames_0000(1);
        }
        (void)((task_250 = -1) & 1);
        Script_SetStats(STATS_PLAYER_CONTEXT_ACTION_MODE, 0);
        script_D87D(0x52494B41, SHEN_NAME('LIGH'));
        task_2E4 = 0;
        SCNF_WaitFrames_0000(1);
        Script_SetRyoMotionDetails(arg0);
        script_DC39();
        script_D06D(3, 0x4F4D4F4A, 0);
        Script_shenRequestGameModuleTransition(3, SHEN_NAME('JHD0'), 6);
        SCNF_WaitFrames_0000(1);
      }
    } else {
      local_B4 = 1;
    }
    if (task_2CC == 1) {
      if (Script_SetTaskDataValue(CHARA_AKIR, &data_10718, &data_10724, 0) & Script_IsPlayerAllowed()) {
        script_9709();
        task_2CC = 0;
      }
    }
    if (task_2F0 == 1) {
      if (Script_SetTaskDataValue(CHARA_AKIR, &data_106DC, &data_106E8, 0)) {
        if (-(local_B8 == 0) & -(task_2F8 == 0) & Script_IsPlayerAllowed()) {
          Script_ryoControl(PCCTL_DisablePlayerControl);
          SCNF_WaitFrames_0000(1);
          if (task_2E4 == 0) {
            Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
            Script_ShowUIMessageFromID("E1012A083");
            while (Script_convGetIsActive(0)) {
              SCNF_WaitFrames_0000(1);
            }
            {
              Vector3i local_A8;
              (&local_A8)->y = 0x8000;
              Script_EVT_SetRotation(CHARA_AKIR, MATRIX_XFORM_Y, &local_A8, 0);
            }
            Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TURN180_WAD_R, -1.0f, -1.0f, -1.0f, 1.0f, 0);
            SCNF_WaitFrames_0000(20);
            {
              Vector3i local_A8;
              (&local_A8)->y = 0;
              Script_EVT_SetRotation(CHARA_AKIR, MATRIX_XFORM_Y, &local_A8, 0);
            }
            Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_WALK_UP1_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
            SCNF_WaitFrames_0000(30);
            Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
            local_B8 = 1;
            if (Script_GetOwnedItemCount(51, 0) == 0) {
              task_658 = task_658 + 1;
              if (task_658 == 2) {
                task_650 = 1;
                task_658 = task_658 + 1;
              }
            }
          }
          Script_ryoControl(RYO_RestoreControl);
        }
      } else {
        local_B8 = 0;
      }
    }
    if (Script_SetTaskDataValue(CHARA_AKIR, &data_10700, &data_1070C, 0)) {
      Script_EVT_ApplyRot((void *)local_E8, &local_DC);
    } else {
      Script_EVT_ApplyRot(0, &local_DC);
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_D789(int32_t arg0, int32_t arg1) {
  Vector3 local_F4;
  Vector3i local_E8;
  local_F4.x = 0.124f;
  (&local_F4)->y = 0.056f;
  (&local_F4)->z = 0.019f;
  local_E8.x = 13378;
  (&local_E8)->y = 17293;
  (&local_E8)->z = 3639;
  Script_fixoAttachEx(arg1, arg0, 12, &local_F4, &local_E8);
  Script_charaDisplay(arg1, CDM_Show);
  Script_SetStats(STATS_PLAYER_CONTEXT_ACTION_MODE, 3);
  Script_CharHandlers(arg0, 1, 0, 16);
  Script_CharHandlers(arg0, 0, 2, 16);
  task_2F0 = 0;
  SCNF_WaitFrames_0000(1);
  return;
}

static void script_D810(int32_t arg0, int32_t arg1) {
  Vector3 local_F4;
  Vector3i local_E8;
  Script_SetStats(STATS_PLAYER_CONTEXT_ACTION_MODE, 3);
  SCNF_WaitFrames_0000(1);
  local_F4.x = 0.124f;
  (&local_F4)->y = 0.056f;
  (&local_F4)->z = 0.019f;
  local_E8.x = 13378;
  (&local_E8)->y = 17293;
  (&local_E8)->z = 3639;
  Script_fixoAttachEx(arg1, arg0, 12, &local_F4, &local_E8);
  Script_charaDisplay(arg1, CDM_Show);
  task_2F0 = 0;
  return;
}

static void script_D87D(int32_t arg0, int32_t arg1) {
  int32_t unused_E8[6];
  Script_SetStats(STATS_PLAYER_CONTEXT_ACTION_MODE, 0);
  if (Script_charaDisplay(arg1, CDM_Query)) {
    Script_fixoClearAttachment(arg1);
  }
  Script_CharHandlers(CHARA_AKIR, 2, 0, 16);
  Script_CharHandlers(CHARA_AKIR, -1, 0, 0);
  Script_charaDisplay(arg1, CDM_Hide);
  return;
}

static void script_D8C1(void) {
  Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 255, 255, 255);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
    SCNF_WaitFrames_0000(2);
  }
  Script_SetCurrentScrollEnabled(0);
  Script_SetFogFlags(2, 0);
  return;
}

static void script_D908(void) {
  task_2D4 = 1;
  Script_charaDisplay(SHEN_NAME('MUKA'), CDM_Hide);
  task_254 = 12;
  Script_charaDisplay(SHEN_NAME('KABE'), CDM_Hide);
  Script_scenCreateTask(script_E106, 1);
  Script_EVT_SetRotation(SHEN_NAME('KINK'), 0x38000000, &data_107A4, 0);
  Script_EVT_SetPosition(SHEN_NAME('TANR'), POS_XYZ, &data_10774);
  Script_EVT_SetPosition(SHEN_NAME('YAK2'), POS_XYZ, &data_10780);
  Script_EVT_SetPosition(SHEN_NAME('UBOX'), POS_XYZ, &data_1078C);
  Script_EVT_SetPosition(SHEN_NAME('BALL'), POS_XYZ, &data_10798);
  Script_EVT_SetPosition(SHEN_NAME('FUDA'), POS_XYZ, &data_107E0);
  Script_EVT_SetRotation(SHEN_NAME('KINK'), 0x38000000, &data_107A4, 0);
  Script_EVT_SetPosition(SHEN_NAME('HKHK'), POS_XYZ, &data_107B0);
  Script_EVT_SetRotation(SHEN_NAME('HKHK'), 0x38000000, &data_107BC, 0);
  Script_EVT_SetPosition(SHEN_NAME('HKFT'), POS_XYZ, &data_107C8);
  Script_EVT_SetRotation(SHEN_NAME('HKFT'), 0x38000000, &data_107D4, 0);
  Script_EVT_SetPosition(CHARA_ONO_, POS_XYZ, &data_107F8);
  Script_EVT_SetRotation(CHARA_ONO_, 0x38000000, &data_10804, 0);
  return;
}

static void script_DA3B(void) {
  int32_t local_FC;
  local_FC = Script_scenUtils(SCENARIO_CMD_GET_STEP_LUT, 20);
  {
    int32_t local_F8;
    local_F8 = (local_FC & 0x30) >> 4;
    switch (local_F8) {
      case 0:
        *(int16_t *)task_2FC = 240;
        task_304[0] = 0;
        break;
      case 1:
        *(int16_t *)task_2FC = 240;
        task_304[0] = 1;
        break;
      case 2:
        *(int16_t *)task_2FC = 240;
        task_304[0] = 0;
        break;
      case 3:
        *(int16_t *)task_2FC = 240;
        task_304[0] = 1;
        break;
    }
  }
  {
    int32_t local_F8;
    local_F8 = (local_FC & 0xC) >> 2;
    switch (local_F8) {
      case 0:
        *(int16_t *)((int8_t *)task_2FC + 2) = 240;
        task_304[1] = 0;
        break;
      case 1:
        *(int16_t *)((int8_t *)task_2FC + 2) = 240;
        task_304[1] = 1;
        break;
      case 2:
        *(int16_t *)((int8_t *)task_2FC + 2) = 240;
        task_304[1] = 0;
        break;
      case 3:
        *(int16_t *)((int8_t *)task_2FC + 2) = 120;
        task_304[1] = 1;
        break;
    }
  }
  {
    int32_t local_F8;
    local_F8 = local_FC & 3;
    switch (local_F8) {
      case 0:
        *(int16_t *)((int8_t *)task_2FC + 4) = 240;
        task_304[2] = 0;
        break;
      case 1:
        *(int16_t *)((int8_t *)task_2FC + 4) = 240;
        task_304[2] = 1;
        break;
      case 2:
        *(int16_t *)((int8_t *)task_2FC + 4) = 240;
        task_304[2] = 0;
        break;
      case 3:
        *(int16_t *)((int8_t *)task_2FC + 4) = 60;
        task_304[2] = 1;
        break;
    }
  }
  {
    int32_t local_F8;
    local_F8 = (local_FC & 0xC0) >> 6;
    if (local_F8 == 1) {
      *(int16_t *)((int8_t *)task_2FC + 6) = 240;
      task_304[3] = 1;
    } else {
      if ((local_F8 == 0) | (local_F8 == 2) | (local_F8 == 3)) {
        *(int16_t *)((int8_t *)task_2FC + 6) = 240;
        task_304[3] = 0;
        goto L00DC38;
      }
    }
  }
  L00DC38:;
  return;
}

static void script_DC39(void) {
  int16_t local_FE;
  if ((*(int16_t *)task_2FC == 240) & (*task_304 == 1)) {
    local_FE = 48;
  } else {
    local_FE = 0;
  }
  if ((*(int16_t *)((int8_t *)task_2FC + 2) == 120) & (task_304[1] == 1)) {
    local_FE = local_FE | 0xC;
  } else {
    if (task_304[1] == 1) {
      local_FE = local_FE | 4;
      goto L00DC9D;
    }
  }
  L00DC9D:;
  if ((*(int16_t *)((int8_t *)task_2FC + 4) == 60) & (task_304[2] == 1)) {
    local_FE = local_FE | 3;
  } else {
    if (task_304[2] == 1) {
      local_FE = local_FE | 1;
      goto L00DCDC;
    }
  }
  L00DCDC:;
  if ((*(int16_t *)((int8_t *)task_2FC + 6) == 240) & (task_304[3] == 0)) {
    local_FE = local_FE | 0xC0;
  } else {
    if (task_304[3] == 1) {
      local_FE = local_FE | 0x40;
      goto L00DD1D;
    }
  }
  L00DD1D:;
  Script_scenUtils(SCENARIO_CMD_SET_STEP_LUT, 20, local_FE);
  return;
}

__attribute__((scn_string_bindings(0, data_10810)))
static int32_t script_DD28(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t local_E4;
  int32_t unused_D8[3];
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 2, "MPK00");
  local_E4 = 0;
  L00DD49:;
  Script_InterpolateAndScaleAngles(0, local_E4, &local_F4, &local_E8, "DYN3");
  local_F4.x = (&local_F4)->x * (float)10 + 7.0521f;
  (&local_F4)->y = (&local_F4)->y * (float)10 - 0.05f;
  (&local_F4)->z = (&local_F4)->z * (float)10 + 4.5825f;
  Script_EVT_SetPosition(SHEN_NAME('LEAF'), POS_XYZ, &local_F4);
  Script_EVT_SetRotation(SHEN_NAME('LEAF'), 0x38000000, &local_E8, 0);
  local_E4 = local_E4 + 1;
  SCNF_WaitFrames_0000(1);
  if (!(local_E4 > 154)) goto L00DD49;
  return Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
}

static void script_DDE3(int32_t arg0) {
  (void)((task_250 = -1) & 1);
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_ccowUtils(arg0, 2);
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_YDB1_WAKE_FROM_SLEEP_CUTSCENE_SEEN) == 0) {
    Script_seqEnterEvtCam(10, 0, AUTH_None);
  } else {
    Script_seqEnterEvtCam(15, 0, AUTH_None);
  }
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_YDB1_WAKE_FROM_SLEEP_CUTSCENE_SEEN) == 0) {
    Script_PlayCharaMotion(arg0, 4103, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  } else {
    Script_PlayCharaMotion(arg0, 4103, 0.0f, 82.0f, -1, 1.0f, 0);
  }
  for (;;) {
    int32_t local_FC;
    task_258 = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_GetCharaMotionRemainingFramesScaled(CHARA_AKIR));
    if (Script_IsCharaMotionFinished(CHARA_AKIR) != 0) {
      break;
    }
    local_FC = task_258;
    switch (local_FC) {
      case 5:
        if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_YDB1_WAKE_FROM_SLEEP_CUTSCENE_SEEN) == 0) {
          Script_ShowUIMessageFromID("E1012A001");
        } else {
          if (!(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_YDB1_SECRET_BOX_OPENING_COMPLETE) == 0)) goto L00DED9;
        }
        L00DED9:;
        break;
      case 6:
        Script_executeAicaCommand(0x2805A9, 0, 0);
        break;
      case 24:
        Script_executeAicaCommand(0x2805A9, 0, 0);
        break;
      case 70:
        Script_executeAicaCommand(0x3605A9, 0, 0);
        Script_scenCreateTask(script_AA72, 0);
        break;
      case 110:
        Script_seqEnterEvtCam(11, 0, AUTH_None);
        break;
      case 225:
        Script_executeAicaCommand(0x1205A9, 0, 0);
        break;
      case 235:
        Script_executeAicaCommand(0x1305A9, 0, 0);
        break;
      case 180:
        break;
    }
    SCNF_WaitFrames_0000(1);
  }
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  SCNF_WaitFrames_0000(1);
  Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_YDB1_WAKE_FROM_SLEEP_CUTSCENE_SEEN, 1);
  Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_YDB1_WAKE_FROM_SLEEP_CUTSCENE_SEEN, 1);
  (void)((task_250 = 0) & 0x10);
  Script_motmToggleFlags(CHARA_AKIR, 2, 0x40);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &data_10834);
  {
    Vector3i local_F4;
    (&local_F4)->y = 0x8000;
    Script_EVT_SetRotation(CHARA_AKIR, MATRIX_XFORM_Y, &local_F4, 0);
  }
  Script_convSetCamInactiveFlag(1);
  Script_SetGameStateTo(AUTH_None);
  SCNF_WaitFrames_0000(10);
  Script_motmToggleFlags(CHARA_AKIR, 1, 0x40);
  Script_ccowUtils(arg0, 1);
  Script_ryoControl(RYO_RestoreControl);
  Script_hmdlApplyScale(SHEN_NAME('UBOX'), &data_10828);
  Script_charaSetClipAngle(SHEN_NAME('TANR'), 15.0f);
  Script_charaSetClipAngle(SHEN_NAME('UBOX'), 15.0f);
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_YDB1_FLASHLIGHT_NEEDS_BULB)) {
    Script_charaDisplay(0x306B6E64, CDM_Show);
    if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 890)) {
      Script_charaDisplay(0x316B6E64, CDM_Show);
      Script_lightSetSlotEnabled(2, 1);
      Script_lightSetSlotEnabled(3, 1);
      Script_scenToggleCharaFlag(0, 100, 0);
    }
  }
  Script_scenCreateTask(script_8891, 0);
  Script_scenCreateTask(script_294C, 0);
  Script_scenCreateTask(script_3A93, 0);
  script_D908();
  script_E4BE();
  Script_shenClearFreeState(1);
  return;
}

__attribute__((scn_string_bindings(1, data_108A7)))
static int32_t script_E106(int32_t arg0) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t local_E4;
  local_E4 = 2;
  Script_sprintf("start\012");
  {
    int32_t local_E0;
    local_E0 = 0;
    L00E11D:;
    Script_charaDisplay(*(&data_10858 + local_E0), CDM_Show);
    local_E0 = local_E0 + 1;
    if (!(local_E0 >= 18)) goto L00E11D;
  }
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 2, "MPK00");
  L00E15A:;
  {
    int32_t local_E0;
    local_E0 = 0;
    L00E160:;
    Script_InterpolateAndScaleAngles(local_E0, local_E4, &local_F4, &local_E8, "DYN1");
    local_F4.x = (&local_F4)->x * (float)10;
    (&local_F4)->y = (&local_F4)->y * (float)10;
    (&local_F4)->z = (&local_F4)->z * (float)10;
    Script_EVT_SetPosition(*(&data_10858 + local_E0), POS_XYZ, &local_F4);
    Script_EVT_SetRotation(*(&data_10858 + local_E0), 0x38000000, &local_E8, 0);
    local_E0 = local_E0 + 1;
    if (!(local_E0 >= 18)) goto L00E160;
  }
  local_E4 = local_E4 + 2;
  SCNF_WaitFrames_0000(1);
  if (!(local_E4 > 65)) goto L00E15A;
  return Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
}

static void script_E207(void) {
  Script_ryoSleepUtils(3, 1, 0, task_2CC);
  Script_ryoSleepUtils(3, 1, 1, *(int16_t *)task_2FC);
  Script_ryoSleepUtils(3, 1, 2, *(int16_t *)((int8_t *)task_2FC + 2));
  Script_ryoSleepUtils(3, 1, 3, *(int16_t *)((int8_t *)task_2FC + 4));
  Script_ryoSleepUtils(3, 1, 4, *(int16_t *)((int8_t *)task_2FC + 6));
  Script_ryoSleepUtils(3, 1, 5, *task_304);
  Script_ryoSleepUtils(3, 1, 6, task_304[1]);
  Script_ryoSleepUtils(3, 1, 7, task_304[2]);
  Script_ryoSleepUtils(3, 1, 8, task_304[3]);
  Script_ryoSleepUtils(3, 1, 9, *(int8_t *)&task_674[0][4]);
  Script_ryoSleepUtils(3, 1, 10, *(int8_t *)&task_674[0][5]);
  Script_ryoSleepUtils(3, 1, 11, *(int8_t *)&task_674[1][4]);
  Script_ryoSleepUtils(3, 1, 12, *(int8_t *)&task_674[1][5]);
  Script_ryoSleepUtils(3, 1, 13, *(int8_t *)&task_674[2][4]);
  Script_ryoSleepUtils(3, 1, 14, *(int8_t *)&task_674[2][5]);
  Script_ryoSleepUtils(3, 1, 15, *(int8_t *)&task_674[3][4]);
  Script_ryoSleepUtils(3, 1, 16, *(int8_t *)&task_674[3][5]);
  Script_ryoSleepUtils(3, 1, 17, task_2E4);
  Script_ryoSleepUtils(3, 1, 18, task_320);
  return;
}

static void script_E34E(void) {
  task_2CC = Script_ryoSleepUtils(3, 0, 0);
  *(int16_t *)task_2FC = Script_ryoSleepUtils(3, 0, 1);
  *(int16_t *)((int8_t *)task_2FC + 2) = Script_ryoSleepUtils(3, 0, 2);
  *(int16_t *)((int8_t *)task_2FC + 4) = Script_ryoSleepUtils(3, 0, 3);
  *(int16_t *)((int8_t *)task_2FC + 6) = Script_ryoSleepUtils(3, 0, 4);
  task_304[0] = Script_ryoSleepUtils(3, 0, 5);
  task_304[1] = Script_ryoSleepUtils(3, 0, 6);
  task_304[2] = Script_ryoSleepUtils(3, 0, 7);
  task_304[3] = Script_ryoSleepUtils(3, 0, 8);
  *(int8_t *)&task_674[0][4] = Script_ryoSleepUtils(3, 0, 9);
  *(int8_t *)&task_674[0][5] = Script_ryoSleepUtils(3, 0, 10);
  *(int8_t *)&task_674[1][4] = Script_ryoSleepUtils(3, 0, 11);
  *(int8_t *)&task_674[1][5] = Script_ryoSleepUtils(3, 0, 12);
  *(int8_t *)&task_674[2][4] = Script_ryoSleepUtils(3, 0, 13);
  *(int8_t *)&task_674[2][5] = Script_ryoSleepUtils(3, 0, 14);
  *(int8_t *)&task_674[3][4] = Script_ryoSleepUtils(3, 0, 15);
  *(int8_t *)&task_674[3][5] = Script_ryoSleepUtils(3, 0, 16);
  task_2E4 = Script_ryoSleepUtils(3, 0, 17);
  task_320 = Script_ryoSleepUtils(3, 0, 18);
  return;
}

static void script_E4BE(void) {
  Script_scenToggleVisibleFor(SHEN_NAME('HN01'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('HN02'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('HN03'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('HN04'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('HN05'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('HN06'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('HN07'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('HN08'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('HN09'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('HN10'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('HN11'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('HN12'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('HN13'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('HN14'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('HN15'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('HN16'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('HN17'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('HN18'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('HOKY'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('SHAS'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('TUB2'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('TUB3'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('HIK1'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('HIK2'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('HIK3'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('YAKU'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('YAK2'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('BNT0'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('KKEY'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('DAMY'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('MKDE'), 0);
  Script_scenToggleVisibleFor(CHARA_ONO_, 0);
  Script_scenToggleVisibleFor(SHEN_NAME('BALL'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('BOOK'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('BX4A'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('BX8A'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('SAKA'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('DENW'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('LEAF'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('TIRA'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('KNIS'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('TNS0'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('TNS1'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('TNS2'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('TNS3'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('TNS4'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('TNS5'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('PIPE'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('UBOX'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('KKAZ'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('KUSK'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('MAK2'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('KAK1'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('ROU1'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('RO11'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('RO14'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('ROU2'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('RO21'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('RO24'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('ROU3'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('RO31'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('RO34'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('ROU4'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('RO41'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('RO44'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('IROU'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('HROU'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('MATB'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('FUDA'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('DOGI'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('SKDI'), 0);
  Script_scenToggleVisibleFor(0x316B6E64, 0);
  Script_scenToggleVisibleFor(0x306B6E64, 0);
  Script_scenToggleVisibleFor(SHEN_NAME('LIGH'), 0);
  Script_scenToggleVisibleFor(0x746E6564, 0);
  Script_scenToggleVisibleFor(SHEN_NAME('HKFT'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('HKHK'), 0);
  return;
}
