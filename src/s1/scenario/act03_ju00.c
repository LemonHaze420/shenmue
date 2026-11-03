#include "act03_ju00.h"

static int32_t script_0030(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_0932(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_0A25(int32_t arg0, int32_t arg1);
static void script_0B64(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_0D13(int32_t arg0, int32_t arg1, int32_t arg2);
static int8_t script_0E0C(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5, int32_t arg6);
static int8_t script_12FC(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_18C8(int32_t arg0, int32_t arg1, float arg2, float arg3, int32_t arg4, int32_t arg5);
static int32_t script_1B07(int32_t arg0, int32_t arg1, float arg2, float arg3, int32_t arg4, int32_t arg5, int32_t arg6);
static int32_t script_1D96(int32_t arg0, int32_t arg1, float arg2, float arg3, int32_t arg4, int32_t arg5, int32_t arg6);
static void script_20A3(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_22D1(int32_t arg0);
static void script_2370(float arg0);
static void script_23FF(float arg0);
static void script_248E(void);
static int32_t script_24C5(int32_t arg0, int32_t arg1);
static void script_25FC(int32_t arg0);
static void script_2643(int32_t arg0);
static void script_264D(int32_t arg0);
static int32_t script_2657(void);
static void script_265E(int32_t arg0);
static void script_2739(int32_t arg0);
static void script_2814(int32_t arg0);
static void script_28EF(int32_t arg0, int32_t arg1);
static void script_29E5(int32_t arg0, int32_t arg1);
static int32_t script_2ADB(int32_t arg0, int32_t arg1);
static int32_t script_2C03(int32_t arg0, int32_t arg1);
static void script_2D34(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_2DB9(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_2E09(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_2E58(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_31EE(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_387C(int32_t arg0, float arg1, float arg2, float arg3);
static void script_38A4(int32_t arg0, float arg1, float arg2, float arg3);
static void script_38F8(int32_t arg0, int32_t arg1, int32_t arg2);
static int32_t script_3948(int32_t arg0);
static void script_39EF(int32_t arg0, int32_t arg1);
static int32_t script_3A12(int32_t arg0);
static void script_3A45(int32_t arg0, int32_t arg1);
static void script_3A68(int32_t arg0, int32_t arg1);
static void script_3AC7(int32_t arg0);
static void script_3ADC(void);
static void script_3AEB(void);
static void script_3AFA(void);
static void script_3AFD(void);
static void script_3B00(void);
static void script_3B03(void);
static void script_3B06(void);
static void script_3B13(void);
static void script_3B1F(void);
static int32_t script_3B2E(void);
static int32_t script_3B3D(int32_t arg0);
static void script_3B5B(float arg0);
static int32_t script_3B7A(int32_t arg0, int32_t arg1);
static void script_3BF2(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4);
static int32_t script_41A4(int32_t arg0);
static void script_4275(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_4285(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_4389(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_452B(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_468C(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_47ED(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_4A8B(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_4B59(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_4BC0(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_4C27(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_4C6E(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_4DA4(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_5665(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_5783(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_5E48(int32_t arg0, int32_t arg1);
static void script_5F1E(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_6E31(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_6E9D(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_7357(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_7A11(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_92F1(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_9A12(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_A19C(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_A274(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_A6EB(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_B67E(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_BAC8(int32_t arg0, DSWKDoorEntry *arg1, int32_t *arg2, DoorResourceDescriptor *arg3, MapObjectRecord *arg4, int32_t arg5, int32_t arg6);
static void script_BC54(DSWKDoorEntry *arg0, int32_t *arg1, DoorResourceDescriptor *arg2, MapObjectRecord *arg3, int32_t arg4);
static void script_BC88(void);
static void script_BCF1(int32_t arg0, int32_t arg1);
static void script_BD8E(int32_t arg0, int32_t arg1);
static void script_BDC8(void);
static void script_BE2B(void);
static void script_BE8E(int32_t arg0);
static void script_BEC5(int32_t arg0);
static void script_BEFC(int32_t arg0);
static void script_C13E(int32_t arg0);
static void script_C187(int32_t arg0);
static void script_C1D0(int32_t arg0, int32_t arg1);
static void script_C246(int32_t arg0);
static void script_C254(int32_t arg0);
static void script_C262(int32_t arg0);
static void script_C329(void);
static void script_C335(void);
static void script_C341(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5, int32_t arg6);
static int32_t script_C384(void);
static int8_t script_C495(int32_t arg0, int32_t arg1, int8_t arg2);
static void script_C52D(int32_t arg0);
static void script_C6FC(int32_t arg0);
static int32_t script_C72D(int32_t arg0, int32_t arg1, int32_t arg2, float arg3, float arg4, int32_t arg5, int32_t arg6);
static void script_CDF6(int32_t arg0, int32_t arg1);
static void script_CE28(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_CF98(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_D154(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_D308(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_D51E(int32_t arg0, int32_t arg1);
static void script_D5E8(int32_t arg0, int32_t arg1);
static void script_D649(int32_t arg0);
static void script_D7EE(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_D886(int32_t arg0, int32_t arg1);
static void script_D968(int32_t arg0);
static void script_D9BF(void);
static void script_D9CD(void);
static void script_D9D0(int32_t arg0);
static void script_D9FB(void);
static void script_DA2F(void);
static void script_DBA4(void);
static void script_DBAD(void);
static void script_DBBC(int32_t arg0);
static void script_DC49(int32_t arg0);
static void script_DC99(void);
static void script_DCC7(int32_t arg0);
static void script_DCF5(void);
static void script_DD29(int32_t arg0);
static void script_DD5D(int32_t arg0);
static void script_DD7B(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_DDB5(int32_t arg0, int32_t arg1);
static void script_DDE9(void);
static void script_DEE1(int32_t arg0, int32_t arg1);
static void script_DF11(void);
static void script_ED5E(void);
static void script_F78A(void);
static void script_FB75(void);
static void script_FF79(void);
static void script_1001F(void);
static void script_10176(void);
static void script_10224(void);
static int32_t script_10296(int32_t arg0);
static int32_t script_102CA(int32_t arg0, int32_t arg1, float arg2, float arg3);
static int32_t script_1031A(void);
static void script_1034F(int32_t arg0);
static void script_103BB(int32_t arg0);
static void script_104DC(int32_t arg0);
static void script_1055F(int32_t arg0);
static void script_1059B(int32_t arg0);
static void script_10637(void);
static void script_10661(void);
static int32_t script_106C6(void);
static int32_t script_106FF(void);
static void script_1078E(void);
static void script_10796(void);
static void script_107FC(void);
static void script_107FF(void);
static void script_10802(void);
static void script_10805(void);
static void script_10808(void);
static void script_10882(void);
static void script_10EA5(void);
static void script_10F76(void);
static void script_11366(void);
static void script_119FC(void);
static void script_11CE0(void);
static void script_11D6F(void);
static void script_11EC4(void);
static int32_t script_11EED(void);
static void script_11F55(void);
static void script_120B7(void);
static void script_12146(void);
static void script_12241(void);
static void script_122B3(void);
static void script_12B7A(void);
static void script_12D89(void);
static void script_12E68(void);
static void script_12F47(int32_t arg0, int32_t arg1);
static void script_14E33(int32_t arg0);
static void script_14F0F(void);
static void script_14F12(const char *arg0);
static void script_14F21(float arg0, const char *arg1, const char *arg2, const char *arg3, ...);
static void script_14FB0(void);
static void script_14FD1(void);
static int32_t script_1505B(int32_t arg0, int32_t arg1);
static int32_t script_151EA(int32_t arg0, int32_t arg1);
static int32_t script_152BD(int32_t arg0, int32_t arg1);
static int32_t script_153E0(int32_t arg0, int32_t arg1);
static void script_15539(void);
static void script_15572(void);
static void script_1558D(void);
static void script_157DE(void);
static void script_15855(void);
static void script_16635(void);
static void script_166A7(void);
static void script_166CC(void);
static void script_166E6(void);
static void script_166F2(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_167AB(int32_t arg0);
static void script_16E20(float arg0, float arg1);
static void script_16E3F(void);
static int32_t script_17171(int32_t arg0);
static void script_171F5(int32_t arg0, int32_t arg1);
static float script_1721B(int32_t arg0, float arg1, float arg2, float arg3, float arg4);
static int32_t script_17266(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4);
static void script_17308(int32_t arg0);
static void script_17335(int32_t arg0);
static int32_t script_17362(int32_t arg0);
static void script_17396(int32_t arg0, float arg1, float arg2, float arg3, float arg4);
static void script_17598(int32_t arg0);
static void script_17667(int32_t arg0);
static void script_1773E(int32_t arg0);
static void script_180D2(int32_t arg0);
static void script_180DE(int32_t arg0);
static void script_181F4(int32_t arg0);
static void script_18273(int32_t arg0);
static void script_1836A(int32_t arg0);
static void script_1843E(int32_t arg0);
static void script_184FE(int32_t arg0);
static void script_18564(int32_t arg0);
static void script_185DE(int32_t arg0);
static void script_18665(int32_t arg0);
static void script_1875C(int32_t arg0);
static void script_1883D(int32_t arg0);
static void script_1891E(int32_t arg0);
static void script_189F5(int32_t arg0);
static void script_18AE2(int32_t arg0);
static void script_18BCF(int32_t arg0);
static void script_18D43(int32_t arg0);
static int32_t script_18D63(int32_t arg0);
static int32_t script_18E1F(int32_t arg0);
static void script_18EAC(void);
static void script_18EEB(int32_t arg0);
static int32_t script_18F05(void);
static void script_18F14(void);
static int32_t script_18F6F(int32_t arg0);
static int32_t script_19007(int32_t arg0);
static int32_t script_192C6(int32_t arg0);
static void script_194A5(int32_t arg0);
static int32_t script_19B0D(int32_t arg0);
static int32_t script_19E89(int32_t arg0);
static int32_t script_19F93(int32_t arg0);
static int32_t script_1AA8E(int32_t arg0);
static int32_t script_1AB0F(int32_t arg0, int32_t arg1, float arg2);
static void script_1B6B7(void);
static int32_t script_1B6EF(void);
static void script_1B71F(void);
static void script_1B72E(int32_t arg0);
static void script_1B85C(void);
static void script_1B87A(void);
static void script_1B8EA(int32_t arg0);
static void script_1B917(void);
static void script_1B93E(int32_t arg0);
static int32_t script_1B949(void);
static int32_t script_1B99B(void);
static int32_t script_1BA0F(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_1BAA5(void);
static void script_1BC5B(void);
static void script_1BD3F(void);
static void script_1BDA5(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int8_t script_1BE2E(void);
static void script_1BEF1(int32_t arg0);
static int32_t script_1BF0B(int32_t arg0, int32_t arg1);
static int32_t script_1BF1D(int32_t arg0, int32_t arg1);
static int32_t script_1BF49(void);
static void script_1BFE5(const char *arg0, const char *arg1);
static int32_t script_1C031(void);
static void script_1C32B(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_1C3E4(void);
static int32_t script_1C40B(void);
static int32_t script_1C443(void);
static void script_1C47F(void);
static int32_t script_1C857(void);
static void script_1CBB2(void);
static int32_t script_1CBF1(void);
static void script_1CDF6(void);
static void script_1CF25(void);
static void script_1D10E(int32_t arg0);
static void script_1D174(int32_t arg0);
static void script_1D1DA(int32_t arg0);
static void script_1D240(int32_t arg0);
static void script_1D2A6(void);
static void script_1D2F3(int32_t arg0);
static void script_1D3B2(int32_t arg0);
static void script_1D3FD(void);
static int32_t script_1D473(void);
static void script_1D57E(void);
static void script_1D6B6(int32_t arg0);
static void script_1D6C2(int32_t arg0);
static void script_1D6CE(int32_t arg0);
static void script_1D7DA(int32_t arg0);
static void script_1D7F1(int32_t arg0);
static void script_1D832(int32_t arg0);
static void script_1D876(int32_t arg0);
static void script_1D8F4(void);
static void script_1D973(int32_t arg0);
void script_1D9B3(void);

static int32_t task_0;
static int32_t task_4;
static int32_t task_8;
static int32_t task_C;
static int32_t task_10;
static int32_t task_14;
static int32_t task_18;
static int32_t task_1C;
static int32_t task_20;
static int32_t task_24[4];
static int32_t task_34;
static float task_38;
static float task_3C;
static float task_40;
static int8_t task_44;
static int8_t task_45;
static int8_t task_46;
static int8_t task_47;
static int8_t task_48;
static int8_t task_49;
static int8_t task_4A;
static int8_t task_4B;
static int8_t task_4C;
static uint8_t task_4D;
static uint16_t task_4E;
static int8_t task_50;
static uint8_t task_51;
static uint16_t task_52;
static int32_t task_54;
static int32_t task_58;
static int32_t task_5C;
static int32_t task_60;
static int32_t task_64;
static int8_t task_68;
static uint8_t task_69;
static uint16_t task_6A;
static DSWKDoorEntry *task_6C;
static int32_t *task_70;
static DoorResourceDescriptor *task_74;
static MapObjectRecord *task_78;
static DoorSlotState task_7C;
static int32_t task_88;
static int32_t task_8C[10];
static int32_t task_B4[10];
static int32_t task_DC[10];
static int32_t task_104[10];
static int32_t task_12C[10];
static int32_t task_154[10];
static int32_t task_17C[10];
static int32_t task_1A4[10];
static int32_t task_1CC;
static int32_t task_1D0;
static Vector3 task_1D4;
static int32_t task_1E0;
static int32_t task_1E4;
static int32_t task_1E8;
static int8_t task_1EC;
static uint8_t task_1ED;
static uint16_t task_1EE;
static int8_t task_1F0;
static uint8_t task_1F1;
static uint16_t task_1F2;
static int32_t task_1F4;
static int32_t task_1F8;
static int8_t task_1FC;
static uint8_t task_1FD;
static uint16_t task_1FE;
static Vector3 task_200;
static int32_t task_20C[6];
static int32_t task_224[6];
static int8_t task_23C;
static uint8_t task_23D;
static uint16_t task_23E;
static int32_t task_240;
static int8_t task_244;
static uint8_t task_245;
static uint16_t task_246;
static int32_t task_248;
static int32_t task_24C;
static int32_t task_250;
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
static int32_t task_284;
static int32_t task_288;
static int8_t task_28C;
static int8_t task_28D;
static int8_t task_28E;
static int8_t task_28F;
static int8_t task_290;
static int8_t task_291;
static int8_t task_292;
static int8_t task_293;
static int8_t task_294;
static uint8_t task_295;
static int8_t task_296;
static int8_t task_297;
static int8_t task_298;
static uint8_t task_299;
static uint16_t task_29A;
static Vector3 task_29C;
static Vector3i task_2A8;
static int32_t task_2B4;
static int32_t task_2B8;
static int8_t task_2BC;
static int8_t task_2BD;
static int8_t task_2BE;
static int8_t task_2BF;
static int8_t task_2C0;
static uint8_t task_2C1;
static int8_t task_2C2;
static uint8_t task_2C3;
static int32_t task_2C4;
static const float data_1DB8C[3] = {1.0f, 1.0f, 1.0f};
static const int32_t data_1DB98[4] = {16777215, 16777215, 16777215, 16777215};
static const float data_1DBA8[3] = {20.0f, 800.0f, 1.0f};
static const int32_t data_1DBB4 = -1;
static const char data_1DBB8[10] __attribute__((scn_string_literal)) = "Kaidan : ";
static const char data_1DBC2[32] __attribute__((scn_string_literal)) = "========== initializing kaidan\012";
static const char data_1DBE2[48] __attribute__((scn_string_literal)) = "please ENDOFKDATA use can't start shuki kaidan\012";
static const char data_1DC12[] __attribute__((scn_string_literal)) = "Kaidan Function Error 1\n";
static const char data_1DC2B[] __attribute__((scn_string_literal)) = "Kaidan Function Error 2\n";
static const char data_1DC44[] __attribute__((scn_string_literal)) = "Kaidan Data Error\n";
static const char data_1DC57[10] __attribute__((scn_string_literal)) = "Kaidan : ";
static const char data_1DC61[28] __attribute__((scn_string_literal)) = "wait ... shuki shuki shuki\012";
static const char data_1DC7D[10] __attribute__((scn_string_literal)) = "Kaidan : ";
static const char data_1DC87[24] __attribute__((scn_string_literal)) = "KAIDAN : Warning coli!\012";
static const char data_1DC9F[24] __attribute__((scn_string_literal)) = "KAIDAN : Warning coli!\012";
static const char data_1DCB7[24] __attribute__((scn_string_literal)) = "KAIDAN : Warning coli!\012";
static const char data_1DCCF[24] __attribute__((scn_string_literal)) = "KAIDAN : Warning coli!\012";
static const char data_1DCE7[24] __attribute__((scn_string_literal)) = "KAIDAN : Warning coli!\012";
static const char data_1DCFF[24] __attribute__((scn_string_literal)) = "KAIDAN : Warning setY!\012";
static const char data_1DD17[24] __attribute__((scn_string_literal)) = "KAIDAN : Warning setY!\012";
static const char data_1DD2F[29] = "Kaidan : \000\000\000\000,\004\023\004-\004/\0042\0040\0041\004\207\003";
static const char data_1DD4C[20] __attribute__((scn_string_literal)) = "TUNED FreeTalk def\012";
static const char data_1DD60[20] __attribute__((scn_string_literal)) = "TUNED FreeTalk def\012";
static DoorMotionSelectorBlock data_1DD74[38] = {
  {.motion_desc_index = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
  {.motion_desc_index = {7, 6, 6, 7, 15, 14, 14, 14, 21, 20, 20, 20, 27, 26, 26, 26, 33, 32, 32, 32, 39, 38, 38, 38, 45, 44, 44, 44, 51, 50, 50, 50, 57, 56, 56, 56, 63, 62, 62, 62}},
  {.motion_desc_index = {7, 6, 6, 7, 16, 13, 13, 13, 22, 19, 19, 19, 28, 25, 25, 25, 34, 31, 31, 31, 40, 37, 37, 37, 46, 43, 43, 43, 52, 49, 49, 49, 58, 55, 55, 55, 64, 61, 61, 61}},
  {.motion_desc_index = {6, 7, 7, 6, 13, 16, 16, 16, 19, 22, 22, 22, 25, 28, 28, 28, 31, 34, 34, 34, 37, 40, 40, 40, 43, 46, 46, 46, 49, 52, 52, 52, 55, 58, 58, 58, 61, 64, 64, 64}},
  {.motion_desc_index = {6, 7, 7, 6, 14, 15, 15, 15, 20, 21, 21, 21, 26, 27, 27, 27, 32, 33, 33, 33, 38, 39, 39, 39, 44, 45, 45, 45, 50, 51, 51, 51, 56, 57, 57, 57, 62, 63, 63, 63}},
  {.motion_desc_index = {6, 7, 7, 6, 13, 16, 16, 16, 19, 22, 22, 22, 25, 28, 28, 28, 31, 34, 34, 34, 37, 40, 40, 40, 43, 46, 46, 46, 49, 52, 52, 52, 55, 58, 58, 58, 61, 64, 64, 64}},
  {.motion_desc_index = {6, 7, 7, 6, 14, 15, 15, 15, 20, 21, 21, 21, 26, 27, 27, 27, 32, 33, 33, 33, 38, 39, 39, 39, 44, 45, 45, 45, 50, 51, 51, 51, 56, 57, 57, 57, 62, 63, 63, 63}},
  {.motion_desc_index = {7, 6, 6, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
  {.motion_desc_index = {7, 6, 6, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
  {.motion_desc_index = {1, 1, 1, 1, 11, 12, 12, 12, 17, 18, 18, 18, 23, 24, 24, 24, 29, 30, 30, 30, 35, 36, 36, 36, 41, 42, 42, 42, 47, 48, 48, 48, 53, 54, 54, 54, 59, 60, 60, 60}},
  {.motion_desc_index = {1, 1, 1, 1, 12, 11, 11, 11, 18, 17, 17, 17, 24, 23, 23, 23, 30, 29, 29, 29, 36, 35, 35, 35, 42, 41, 41, 41, 48, 47, 47, 47, 54, 53, 53, 53, 60, 59, 59, 59}},
  {.motion_desc_index = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
  {.motion_desc_index = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
  {.motion_desc_index = {6, 7, 7, 6, 13, 16, 15, 14, 19, 22, 21, 20, 25, 28, 27, 26, 31, 34, 33, 32, 37, 40, 39, 38, 43, 46, 45, 44, 49, 52, 51, 50, 55, 58, 57, 56, 61, 64, 63, 62}},
  {.motion_desc_index = {6, 7, 7, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
  {.motion_desc_index = {1, 1, 1, 1, 12, 11, 12, 11, 18, 17, 18, 17, 24, 23, 24, 23, 30, 29, 30, 29, 36, 35, 36, 35, 42, 41, 42, 41, 48, 47, 48, 47, 54, 53, 54, 53, 60, 59, 60, 59}},
  {.motion_desc_index = {0, 0, 0, 0, 11, 12, 11, 12, 17, 18, 17, 18, 23, 24, 23, 24, 29, 30, 29, 30, 35, 36, 35, 36, 41, 42, 41, 42, 47, 48, 47, 48, 53, 54, 53, 54, 59, 60, 59, 60}},
  {.motion_desc_index = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
  {.motion_desc_index = {2, 2, 2, 2, 11, 12, 12, 12, 17, 18, 18, 18, 23, 24, 24, 24, 29, 30, 30, 30, 35, 36, 36, 36, 41, 42, 42, 42, 47, 48, 48, 48, 53, 54, 54, 54, 59, 60, 60, 60}},
  {.motion_desc_index = {2, 2, 2, 2, 12, 11, 11, 11, 18, 17, 17, 17, 24, 23, 23, 23, 30, 29, 29, 29, 36, 35, 35, 35, 42, 41, 41, 41, 48, 47, 47, 47, 54, 53, 53, 53, 60, 59, 59, 59}},
  {.motion_desc_index = {2, 2, 2, 2, 12, 11, 12, 11, 18, 17, 18, 17, 24, 23, 24, 23, 30, 29, 30, 29, 36, 35, 36, 35, 42, 41, 42, 41, 48, 47, 48, 47, 54, 53, 54, 53, 60, 59, 60, 59}},
  {.motion_desc_index = {3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
  {.motion_desc_index = {3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
  {.motion_desc_index = {3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
  {.motion_desc_index = {4, 5, 5, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
  {.motion_desc_index = {4, 5, 5, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
  {.motion_desc_index = {5, 4, 4, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
  {.motion_desc_index = {5, 4, 4, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
  {.motion_desc_index = {8, 8, 8, 8, 13, 16, 15, 14, 19, 22, 21, 20, 25, 28, 27, 26, 31, 34, 33, 32, 37, 40, 39, 38, 43, 46, 45, 44, 49, 52, 51, 50, 55, 58, 57, 56, 61, 64, 63, 62}},
  {.motion_desc_index = {9, 10, 10, 9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
  {.motion_desc_index = {7, 6, 6, 7, 15, 14, 14, 14, 21, 20, 20, 20, 27, 26, 26, 26, 33, 32, 32, 32, 39, 38, 38, 38, 45, 44, 44, 44, 51, 50, 50, 50, 57, 56, 56, 56, 63, 62, 62, 62}},
  {.motion_desc_index = {7, 6, 6, 7, 16, 13, 13, 13, 22, 19, 19, 19, 28, 25, 25, 25, 34, 31, 31, 31, 40, 37, 37, 37, 46, 43, 43, 43, 52, 49, 49, 49, 58, 55, 55, 55, 64, 61, 61, 61}},
  {.motion_desc_index = {6, 7, 7, 6, 13, 16, 16, 16, 19, 22, 22, 22, 25, 28, 28, 28, 31, 34, 34, 34, 37, 40, 40, 40, 43, 46, 46, 46, 49, 52, 52, 52, 55, 58, 58, 58, 61, 64, 64, 64}},
  {.motion_desc_index = {6, 7, 7, 6, 14, 15, 15, 15, 20, 21, 21, 21, 26, 27, 27, 27, 32, 33, 33, 33, 38, 39, 39, 39, 44, 45, 45, 45, 50, 51, 51, 51, 56, 57, 57, 57, 62, 63, 63, 63}},
  {.motion_desc_index = {6, 7, 7, 6, 13, 16, 16, 16, 19, 22, 22, 22, 25, 28, 28, 28, 31, 34, 34, 34, 37, 40, 40, 40, 43, 46, 46, 46, 49, 52, 52, 52, 55, 58, 58, 58, 61, 64, 64, 64}},
  {.motion_desc_index = {6, 7, 7, 6, 14, 15, 15, 15, 20, 21, 21, 21, 26, 27, 27, 27, 32, 33, 33, 33, 38, 39, 39, 39, 44, 45, 45, 45, 50, 51, 51, 51, 56, 57, 57, 57, 62, 63, 63, 63}},
  {.motion_desc_index = {7, 6, 6, 7, 15, 14, 14, 14, 21, 20, 20, 20, 65, 26, 26, 26, 33, 32, 32, 32, 66, 38, 38, 38, 67, 44, 44, 44, 51, 50, 50, 50, 68, 56, 56, 56, 69, 62, 62, 62}},
  {.motion_desc_index = {7, 6, 6, 7, 15, 14, 14, 14, 21, 70, 20, 20, 65, 26, 26, 26, 33, 71, 32, 32, 66, 38, 38, 38, 67, 44, 44, 44, 51, 50, 50, 50, 68, 56, 56, 56, 69, 62, 62, 62}}
};
static ScnDoorMotionDescriptor data_1E364[72] = {
  {.motion_id = -1, .frame_open_sfx_begin = 0.0f, .frame_phase1_end = 0.0f, .frame_close_sfx_begin = 0.0f, .frame_motion_end = 0.0f, .approach_off_x = 0.0f, .approach_off_z = 0.0f, .npc_tail_end_frame = 0.0f, .npc_tail_delta_per_frame = 0.0f},
  {.motion_id = 631, .frame_open_sfx_begin = 19.0f, .frame_phase1_end = 52.0f, .frame_close_sfx_begin = 96.0f, .frame_motion_end = 132.0f, .approach_off_x = 45.0f, .approach_off_z = 50.0f, .npc_tail_end_frame = 0.0f, .npc_tail_delta_per_frame = 0.0f},
  {.motion_id = 637, .frame_open_sfx_begin = 19.0f, .frame_phase1_end = 53.0f, .frame_close_sfx_begin = 0.0f, .frame_motion_end = 0.0f, .approach_off_x = 45.0f, .approach_off_z = 50.0f, .npc_tail_end_frame = 0.0f, .npc_tail_delta_per_frame = 0.0f},
  {.motion_id = 636, .frame_open_sfx_begin = 30.0f, .frame_phase1_end = 64.0f, .frame_close_sfx_begin = 0.0f, .frame_motion_end = 0.0f, .approach_off_x = 45.0f, .approach_off_z = 50.0f, .npc_tail_end_frame = 0.0f, .npc_tail_delta_per_frame = 0.0f},
  {.motion_id = 628, .frame_open_sfx_begin = 19.0f, .frame_phase1_end = 47.0f, .frame_close_sfx_begin = 0.0f, .frame_motion_end = 0.0f, .approach_off_x = 45.0f, .approach_off_z = 80.0f, .npc_tail_end_frame = 0.0f, .npc_tail_delta_per_frame = 0.0f},
  {.motion_id = 54, .frame_open_sfx_begin = 33.0f, .frame_phase1_end = 70.0f, .frame_close_sfx_begin = 0.0f, .frame_motion_end = 0.0f, .approach_off_x = 50.0f, .approach_off_z = 80.0f, .npc_tail_end_frame = 0.0f, .npc_tail_delta_per_frame = 0.0f},
  {.motion_id = 646, .frame_open_sfx_begin = 19.0f, .frame_phase1_end = 45.0f, .frame_close_sfx_begin = 106.0f, .frame_motion_end = 141.0f, .approach_off_x = 45.0f, .approach_off_z = 80.0f, .npc_tail_end_frame = 0.0f, .npc_tail_delta_per_frame = 0.0f},
  {.motion_id = 649, .frame_open_sfx_begin = 30.0f, .frame_phase1_end = 73.0f, .frame_close_sfx_begin = 108.0f, .frame_motion_end = 135.0f, .approach_off_x = 45.0f, .approach_off_z = 50.0f, .npc_tail_end_frame = 0.0f, .npc_tail_delta_per_frame = 0.0f},
  {.motion_id = 1365, .frame_open_sfx_begin = 30.0f, .frame_phase1_end = 73.0f, .frame_close_sfx_begin = 0.0f, .frame_motion_end = 0.0f, .approach_off_x = 0.0f, .approach_off_z = 89.5f, .npc_tail_end_frame = 0.0f, .npc_tail_delta_per_frame = 0.0f},
  {.motion_id = 1548, .frame_open_sfx_begin = 34.0f, .frame_phase1_end = 88.0f, .frame_close_sfx_begin = 125.0f, .frame_motion_end = 185.0f, .approach_off_x = 63.264f, .approach_off_z = 73.183998f, .npc_tail_end_frame = 0.0f, .npc_tail_delta_per_frame = 0.0f},
  {.motion_id = 1549, .frame_open_sfx_begin = 28.0f, .frame_phase1_end = 73.0f, .frame_close_sfx_begin = 108.0f, .frame_motion_end = 163.0f, .approach_off_x = 58.0f, .approach_off_z = 71.2f, .npc_tail_end_frame = 0.0f, .npc_tail_delta_per_frame = 0.0f},
  {.motion_id = 40731, .frame_open_sfx_begin = 21.0f, .frame_phase1_end = 45.0f, .frame_close_sfx_begin = 95.0f, .frame_motion_end = 125.0f, .approach_off_x = 45.0f, .approach_off_z = 56.0f, .npc_tail_end_frame = 51.0f, .npc_tail_delta_per_frame = 0.025f},
  {.motion_id = 40732, .frame_open_sfx_begin = 20.0f, .frame_phase1_end = 43.0f, .frame_close_sfx_begin = 107.0f, .frame_motion_end = 135.0f, .approach_off_x = 45.0f, .approach_off_z = 56.5f, .npc_tail_end_frame = 48.0f, .npc_tail_delta_per_frame = 0.04f},
  {.motion_id = 40705, .frame_open_sfx_begin = 21.0f, .frame_phase1_end = 35.0f, .frame_close_sfx_begin = 102.0f, .frame_motion_end = 126.0f, .approach_off_x = 48.08f, .approach_off_z = 83.538002f, .npc_tail_end_frame = 65.0f, .npc_tail_delta_per_frame = 1.7136f},
  {.motion_id = 40706, .frame_open_sfx_begin = 41.0f, .frame_phase1_end = 61.0f, .frame_close_sfx_begin = 140.0f, .frame_motion_end = 182.0f, .approach_off_x = 48.978001f, .approach_off_z = 82.223f, .npc_tail_end_frame = 93.0f, .npc_tail_delta_per_frame = 2.4333f},
  {.motion_id = 40733, .frame_open_sfx_begin = 33.0f, .frame_phase1_end = 59.0f, .frame_close_sfx_begin = 105.0f, .frame_motion_end = 136.0f, .approach_off_x = 34.54f, .approach_off_z = 86.077003f, .npc_tail_end_frame = 64.0f, .npc_tail_delta_per_frame = 1.7972f},
  {.motion_id = 40707, .frame_open_sfx_begin = 48.0f, .frame_phase1_end = 88.0f, .frame_close_sfx_begin = 134.0f, .frame_motion_end = 193.0f, .approach_off_x = 50.158001f, .approach_off_z = 51.297001f, .npc_tail_end_frame = 107.0f, .npc_tail_delta_per_frame = 1.206f},
  {.motion_id = 40734, .frame_open_sfx_begin = 22.0f, .frame_phase1_end = 45.0f, .frame_close_sfx_begin = 100.0f, .frame_motion_end = 130.0f, .approach_off_x = 60.0f, .approach_off_z = 55.0f, .npc_tail_end_frame = 52.0f, .npc_tail_delta_per_frame = 0.0183f},
  {.motion_id = 40735, .frame_open_sfx_begin = 20.0f, .frame_phase1_end = 37.0f, .frame_close_sfx_begin = 114.0f, .frame_motion_end = 136.0f, .approach_off_x = 68.5f, .approach_off_z = 60.0f, .npc_tail_end_frame = 42.0f, .npc_tail_delta_per_frame = 0.0798f},
  {.motion_id = 40708, .frame_open_sfx_begin = 23.0f, .frame_phase1_end = 44.0f, .frame_close_sfx_begin = 118.0f, .frame_motion_end = 149.0f, .approach_off_x = 81.0f, .approach_off_z = 58.0f, .npc_tail_end_frame = 64.0f, .npc_tail_delta_per_frame = 1.2643f},
  {.motion_id = 40709, .frame_open_sfx_begin = 51.0f, .frame_phase1_end = 64.0f, .frame_close_sfx_begin = 140.0f, .frame_motion_end = 175.0f, .approach_off_x = 76.0f, .approach_off_z = 80.0f, .npc_tail_end_frame = 101.0f, .npc_tail_delta_per_frame = 1.7926f},
  {.motion_id = 40736, .frame_open_sfx_begin = 35.0f, .frame_phase1_end = 70.0f, .frame_close_sfx_begin = 133.0f, .frame_motion_end = 169.0f, .approach_off_x = 62.75f, .approach_off_z = 59.0f, .npc_tail_end_frame = 70.0f, .npc_tail_delta_per_frame = 1.9468f},
  {.motion_id = 40710, .frame_open_sfx_begin = 33.0f, .frame_phase1_end = 58.0f, .frame_close_sfx_begin = 96.0f, .frame_motion_end = 142.0f, .approach_off_x = 55.0f, .approach_off_z = 69.0f, .npc_tail_end_frame = 79.0f, .npc_tail_delta_per_frame = 2.28709984f},
  {.motion_id = 40737, .frame_open_sfx_begin = 24.0f, .frame_phase1_end = 51.0f, .frame_close_sfx_begin = 89.0f, .frame_motion_end = 123.0f, .approach_off_x = 54.0f, .approach_off_z = 56.5f, .npc_tail_end_frame = 58.0f, .npc_tail_delta_per_frame = 0.0249f},
  {.motion_id = 40738, .frame_open_sfx_begin = 25.0f, .frame_phase1_end = 43.0f, .frame_close_sfx_begin = 94.0f, .frame_motion_end = 123.0f, .approach_off_x = 48.3f, .approach_off_z = 58.0f, .npc_tail_end_frame = 54.0f, .npc_tail_delta_per_frame = 0.0236f},
  {.motion_id = 40711, .frame_open_sfx_begin = 20.0f, .frame_phase1_end = 38.0f, .frame_close_sfx_begin = 95.0f, .frame_motion_end = 125.0f, .approach_off_x = 47.703999f, .approach_off_z = 76.788002f, .npc_tail_end_frame = 60.0f, .npc_tail_delta_per_frame = 1.9167f},
  {.motion_id = 40712, .frame_open_sfx_begin = 45.0f, .frame_phase1_end = 71.0f, .frame_close_sfx_begin = 165.0f, .frame_motion_end = 217.0f, .approach_off_x = 53.195f, .approach_off_z = 88.927002f, .npc_tail_end_frame = 97.0f, .npc_tail_delta_per_frame = 1.4981f},
  {.motion_id = 40739, .frame_open_sfx_begin = 46.0f, .frame_phase1_end = 95.0f, .frame_close_sfx_begin = 150.0f, .frame_motion_end = 200.0f, .approach_off_x = 39.535f, .approach_off_z = 89.264999f, .npc_tail_end_frame = 99.0f, .npc_tail_delta_per_frame = 0.5833f},
  {.motion_id = 40713, .frame_open_sfx_begin = 34.0f, .frame_phase1_end = 110.0f, .frame_close_sfx_begin = 167.0f, .frame_motion_end = 202.0f, .approach_off_x = 48.733002f, .approach_off_z = 59.613998f, .npc_tail_end_frame = 130.0f, .npc_tail_delta_per_frame = 0.75f},
  {.motion_id = 40740, .frame_open_sfx_begin = 22.0f, .frame_phase1_end = 41.0f, .frame_close_sfx_begin = 85.0f, .frame_motion_end = 110.0f, .approach_off_x = 63.0f, .approach_off_z = 47.0f, .npc_tail_end_frame = 47.0f, .npc_tail_delta_per_frame = 0.028f},
  {.motion_id = 40741, .frame_open_sfx_begin = 23.0f, .frame_phase1_end = 41.0f, .frame_close_sfx_begin = 90.0f, .frame_motion_end = 114.0f, .approach_off_x = 73.0f, .approach_off_z = 46.0f, .npc_tail_end_frame = 47.0f, .npc_tail_delta_per_frame = 0.0289f},
  {.motion_id = 40714, .frame_open_sfx_begin = 22.0f, .frame_phase1_end = 36.0f, .frame_close_sfx_begin = 105.0f, .frame_motion_end = 132.0f, .approach_off_x = 67.096001f, .approach_off_z = 56.924f, .npc_tail_end_frame = 59.0f, .npc_tail_delta_per_frame = 1.6836f},
  {.motion_id = 40715, .frame_open_sfx_begin = 45.0f, .frame_phase1_end = 59.0f, .frame_close_sfx_begin = 140.0f, .frame_motion_end = 180.0f, .approach_off_x = 91.0f, .approach_off_z = 60.0f, .npc_tail_end_frame = 95.0f, .npc_tail_delta_per_frame = 1.8126f},
  {.motion_id = 40742, .frame_open_sfx_begin = 23.0f, .frame_phase1_end = 67.0f, .frame_close_sfx_begin = 105.0f, .frame_motion_end = 162.0f, .approach_off_x = 50.1f, .approach_off_z = 54.112f, .npc_tail_end_frame = 80.0f, .npc_tail_delta_per_frame = 0.3366f},
  {.motion_id = 40716, .frame_open_sfx_begin = 32.0f, .frame_phase1_end = 60.0f, .frame_close_sfx_begin = 96.0f, .frame_motion_end = 150.0f, .approach_off_x = 62.0f, .approach_off_z = 63.0f, .npc_tail_end_frame = 86.0f, .npc_tail_delta_per_frame = 1.39560008f},
  {.motion_id = 40743, .frame_open_sfx_begin = 21.0f, .frame_phase1_end = 48.0f, .frame_close_sfx_begin = 80.0f, .frame_motion_end = 113.0f, .approach_off_x = 49.0f, .approach_off_z = 57.5f, .npc_tail_end_frame = 54.0f, .npc_tail_delta_per_frame = 0.0223f},
  {.motion_id = 40744, .frame_open_sfx_begin = 24.0f, .frame_phase1_end = 47.0f, .frame_close_sfx_begin = 94.0f, .frame_motion_end = 120.0f, .approach_off_x = 49.0f, .approach_off_z = 58.5f, .npc_tail_end_frame = 50.0f, .npc_tail_delta_per_frame = 0.0327f},
  {.motion_id = 40717, .frame_open_sfx_begin = 20.0f, .frame_phase1_end = 39.0f, .frame_close_sfx_begin = 98.0f, .frame_motion_end = 125.0f, .approach_off_x = 51.0f, .approach_off_z = 80.081001f, .npc_tail_end_frame = 47.0f, .npc_tail_delta_per_frame = 1.9319f},
  {.motion_id = 40718, .frame_open_sfx_begin = 35.0f, .frame_phase1_end = 55.0f, .frame_close_sfx_begin = 110.0f, .frame_motion_end = 152.0f, .approach_off_x = 51.819f, .approach_off_z = 82.554001f, .npc_tail_end_frame = 87.0f, .npc_tail_delta_per_frame = 2.2963f},
  {.motion_id = 40745, .frame_open_sfx_begin = 33.0f, .frame_phase1_end = 64.0f, .frame_close_sfx_begin = 100.0f, .frame_motion_end = 138.0f, .approach_off_x = 39.535f, .approach_off_z = 95.652f, .npc_tail_end_frame = 71.0f, .npc_tail_delta_per_frame = 1.1599f},
  {.motion_id = 40719, .frame_open_sfx_begin = 43.0f, .frame_phase1_end = 73.0f, .frame_close_sfx_begin = 100.0f, .frame_motion_end = 135.0f, .approach_off_x = 50.158001f, .approach_off_z = 59.491001f, .npc_tail_end_frame = 88.0f, .npc_tail_delta_per_frame = 1.0387f},
  {.motion_id = 40746, .frame_open_sfx_begin = 21.0f, .frame_phase1_end = 41.0f, .frame_close_sfx_begin = 84.0f, .frame_motion_end = 106.0f, .approach_off_x = 45.0f, .approach_off_z = 54.0f, .npc_tail_end_frame = 43.0f, .npc_tail_delta_per_frame = 0.03f},
  {.motion_id = 40747, .frame_open_sfx_begin = 51.0f, .frame_phase1_end = 76.0f, .frame_close_sfx_begin = 156.0f, .frame_motion_end = 184.0f, .approach_off_x = 51.0f, .approach_off_z = 52.5f, .npc_tail_end_frame = 79.0f, .npc_tail_delta_per_frame = 0.0283f},
  {.motion_id = 40720, .frame_open_sfx_begin = 25.0f, .frame_phase1_end = 48.0f, .frame_close_sfx_begin = 100.0f, .frame_motion_end = 129.0f, .approach_off_x = 56.070999f, .approach_off_z = 70.459f, .npc_tail_end_frame = 64.0f, .npc_tail_delta_per_frame = 1.4962f},
  {.motion_id = 40721, .frame_open_sfx_begin = 64.0f, .frame_phase1_end = 99.0f, .frame_close_sfx_begin = 160.0f, .frame_motion_end = 200.0f, .approach_off_x = 43.352001f, .approach_off_z = 106.598999f, .npc_tail_end_frame = 131.0f, .npc_tail_delta_per_frame = 2.0784f},
  {.motion_id = 40748, .frame_open_sfx_begin = 38.0f, .frame_phase1_end = 66.0f, .frame_close_sfx_begin = 102.0f, .frame_motion_end = 133.0f, .approach_off_x = 52.025002f, .approach_off_z = 100.740997f, .npc_tail_end_frame = 69.0f, .npc_tail_delta_per_frame = 0.7168f},
  {.motion_id = 40722, .frame_open_sfx_begin = 47.0f, .frame_phase1_end = 108.0f, .frame_close_sfx_begin = 153.0f, .frame_motion_end = 200.0f, .approach_off_x = 53.535f, .approach_off_z = 90.637001f, .npc_tail_end_frame = 115.0f, .npc_tail_delta_per_frame = 0.0565f},
  {.motion_id = 40749, .frame_open_sfx_begin = 22.0f, .frame_phase1_end = 42.0f, .frame_close_sfx_begin = 84.0f, .frame_motion_end = 105.0f, .approach_off_x = 51.0f, .approach_off_z = 44.5f, .npc_tail_end_frame = 43.0f, .npc_tail_delta_per_frame = 0.02f},
  {.motion_id = 40750, .frame_open_sfx_begin = 39.0f, .frame_phase1_end = 67.0f, .frame_close_sfx_begin = 140.0f, .frame_motion_end = 176.0f, .approach_off_x = 47.0f, .approach_off_z = 54.1f, .npc_tail_end_frame = 75.0f, .npc_tail_delta_per_frame = 0.025f},
  {.motion_id = 40723, .frame_open_sfx_begin = 22.0f, .frame_phase1_end = 46.0f, .frame_close_sfx_begin = 110.0f, .frame_motion_end = 135.0f, .approach_off_x = 60.0f, .approach_off_z = 68.424004f, .npc_tail_end_frame = 67.0f, .npc_tail_delta_per_frame = 1.7189f},
  {.motion_id = 40724, .frame_open_sfx_begin = 39.0f, .frame_phase1_end = 68.0f, .frame_close_sfx_begin = 124.0f, .frame_motion_end = 173.0f, .approach_off_x = 54.183998f, .approach_off_z = 76.428001f, .npc_tail_end_frame = 98.0f, .npc_tail_delta_per_frame = 1.9291f},
  {.motion_id = 40751, .frame_open_sfx_begin = 38.0f, .frame_phase1_end = 70.0f, .frame_close_sfx_begin = 102.0f, .frame_motion_end = 134.0f, .approach_off_x = 45.0f, .approach_off_z = 96.102997f, .npc_tail_end_frame = 70.0f, .npc_tail_delta_per_frame = 0.0f},
  {.motion_id = 40725, .frame_open_sfx_begin = 40.0f, .frame_phase1_end = 84.0f, .frame_close_sfx_begin = 130.0f, .frame_motion_end = 190.0f, .approach_off_x = 50.0f, .approach_off_z = 81.102997f, .npc_tail_end_frame = 110.0f, .npc_tail_delta_per_frame = 1.0671f},
  {.motion_id = 40757, .frame_open_sfx_begin = 21.0f, .frame_phase1_end = 45.0f, .frame_close_sfx_begin = 84.0f, .frame_motion_end = 109.0f, .approach_off_x = 48.5f, .approach_off_z = 44.5f, .npc_tail_end_frame = 46.0f, .npc_tail_delta_per_frame = 0.01f},
  {.motion_id = 40752, .frame_open_sfx_begin = 39.0f, .frame_phase1_end = 67.0f, .frame_close_sfx_begin = 110.0f, .frame_motion_end = 146.0f, .approach_off_x = 49.5f, .approach_off_z = 42.5f, .npc_tail_end_frame = 75.0f, .npc_tail_delta_per_frame = 0.0119f},
  {.motion_id = 40726, .frame_open_sfx_begin = 20.0f, .frame_phase1_end = 44.0f, .frame_close_sfx_begin = 110.0f, .frame_motion_end = 136.0f, .approach_off_x = 60.0f, .approach_off_z = 68.424004f, .npc_tail_end_frame = 56.0f, .npc_tail_delta_per_frame = 0.7448f},
  {.motion_id = 40727, .frame_open_sfx_begin = 42.0f, .frame_phase1_end = 66.0f, .frame_close_sfx_begin = 127.0f, .frame_motion_end = 185.0f, .approach_off_x = 54.183998f, .approach_off_z = 81.102997f, .npc_tail_end_frame = 100.0f, .npc_tail_delta_per_frame = 1.8151f},
  {.motion_id = 40753, .frame_open_sfx_begin = 36.0f, .frame_phase1_end = 57.0f, .frame_close_sfx_begin = 85.0f, .frame_motion_end = 111.0f, .approach_off_x = 30.0f, .approach_off_z = 81.102997f, .npc_tail_end_frame = 62.0f, .npc_tail_delta_per_frame = 1.421f},
  {.motion_id = 40728, .frame_open_sfx_begin = 37.0f, .frame_phase1_end = 87.0f, .frame_close_sfx_begin = 130.0f, .frame_motion_end = 194.0f, .approach_off_x = 50.0f, .approach_off_z = 71.102997f, .npc_tail_end_frame = 101.0f, .npc_tail_delta_per_frame = 0.7448f},
  {.motion_id = 40758, .frame_open_sfx_begin = 21.0f, .frame_phase1_end = 52.0f, .frame_close_sfx_begin = 90.0f, .frame_motion_end = 123.0f, .approach_off_x = 47.553001f, .approach_off_z = 52.632999f, .npc_tail_end_frame = 54.0f, .npc_tail_delta_per_frame = 0.01f},
  {.motion_id = 40754, .frame_open_sfx_begin = 24.0f, .frame_phase1_end = 47.0f, .frame_close_sfx_begin = 104.0f, .frame_motion_end = 133.0f, .approach_off_x = 48.0f, .approach_off_z = 52.132999f, .npc_tail_end_frame = 53.0f, .npc_tail_delta_per_frame = 0.0247f},
  {.motion_id = 40729, .frame_open_sfx_begin = 18.0f, .frame_phase1_end = 42.0f, .frame_close_sfx_begin = 108.0f, .frame_motion_end = 138.0f, .approach_off_x = 45.0f, .approach_off_z = 78.463997f, .npc_tail_end_frame = 68.0f, .npc_tail_delta_per_frame = 1.3195f},
  {.motion_id = 40730, .frame_open_sfx_begin = 35.0f, .frame_phase1_end = 56.0f, .frame_close_sfx_begin = 143.0f, .frame_motion_end = 183.0f, .approach_off_x = 55.4f, .approach_off_z = 84.542999f, .npc_tail_end_frame = 85.0f, .npc_tail_delta_per_frame = 1.9986f},
  {.motion_id = 40755, .frame_open_sfx_begin = 32.0f, .frame_phase1_end = 65.0f, .frame_close_sfx_begin = 110.0f, .frame_motion_end = 144.0f, .approach_off_x = 41.187f, .approach_off_z = 95.0f, .npc_tail_end_frame = 66.0f, .npc_tail_delta_per_frame = 0.2855f},
  {.motion_id = 40756, .frame_open_sfx_begin = 44.0f, .frame_phase1_end = 75.0f, .frame_close_sfx_begin = 105.0f, .frame_motion_end = 151.0f, .approach_off_x = 50.629002f, .approach_off_z = 56.424f, .npc_tail_end_frame = 90.0f, .npc_tail_delta_per_frame = 1.2961f},
  {.motion_id = 40759, .frame_open_sfx_begin = 55.0f, .frame_phase1_end = 90.0f, .frame_close_sfx_begin = 150.0f, .frame_motion_end = 180.0f, .approach_off_x = 30.0f, .approach_off_z = 80.0f, .npc_tail_end_frame = 90.0f, .npc_tail_delta_per_frame = 0.0f},
  {.motion_id = 40760, .frame_open_sfx_begin = 29.0f, .frame_phase1_end = 60.0f, .frame_close_sfx_begin = 71.0f, .frame_motion_end = 110.0f, .approach_off_x = 47.535f, .approach_off_z = 75.652f, .npc_tail_end_frame = 71.0f, .npc_tail_delta_per_frame = 0.1295f},
  {.motion_id = 40761, .frame_open_sfx_begin = 29.0f, .frame_phase1_end = 66.0f, .frame_close_sfx_begin = 104.0f, .frame_motion_end = 200.0f, .approach_off_x = 52.025002f, .approach_off_z = 80.740997f, .npc_tail_end_frame = 67.0f, .npc_tail_delta_per_frame = 0.0231f},
  {.motion_id = 40762, .frame_open_sfx_begin = 36.0f, .frame_phase1_end = 62.0f, .frame_close_sfx_begin = 73.0f, .frame_motion_end = 114.0f, .approach_off_x = 45.0f, .approach_off_z = 71.102997f, .npc_tail_end_frame = 62.0f, .npc_tail_delta_per_frame = 0.0f},
  {.motion_id = 40763, .frame_open_sfx_begin = 32.0f, .frame_phase1_end = 60.0f, .frame_close_sfx_begin = 78.0f, .frame_motion_end = 120.0f, .approach_off_x = 44.284f, .approach_off_z = 81.165001f, .npc_tail_end_frame = 60.0f, .npc_tail_delta_per_frame = 0.0f},
  {.motion_id = 40764, .frame_open_sfx_begin = 52.0f, .frame_phase1_end = 74.0f, .frame_close_sfx_begin = 180.0f, .frame_motion_end = 225.0f, .approach_off_x = 45.0f, .approach_off_z = 65.0f, .npc_tail_end_frame = 95.0f, .npc_tail_delta_per_frame = 2.0f},
  {.motion_id = 40765, .frame_open_sfx_begin = 52.0f, .frame_phase1_end = 76.0f, .frame_close_sfx_begin = 190.0f, .frame_motion_end = 240.0f, .approach_off_x = 45.0f, .approach_off_z = 60.0f, .npc_tail_end_frame = 98.0f, .npc_tail_delta_per_frame = 1.7727f}
};
static uint32_t data_1ED84 = 0xFFFFFFFF;
typedef struct Row_1ED88 { float w0; float w1; float w2; float w3; int32_t w4; } Row_1ED88;
static const Row_1ED88 data_1ED88[7] = {
  {0.0f, 0.0f, 0.0f, 0.0f, -1},
  {0.0f, 0.0f, 0.0f, 0.0f, -1},
  {0.0f, 0.0f, 0.0f, 0.0f, 1534},
  {26.0f, 55.0f, 45.0f, 75.0f, 1555},
  {61.0f, 146.0f, 45.0f, 50.0f, 1554},
  {59.0f, 124.0f, 45.0f, 70.0f, 1543},
  {33.0f, 49.0f, 45.0f, 50.0f, 1543}
};
static const float data_1EE14[4] = {33.0f, 49.0f, 50.0f, 50.0f};
static void (*data_1EE24)(int32_t, int32_t, int32_t) = script_4275;
static void (*const data_1EE28)(int32_t, int32_t, int32_t, int32_t) = script_4285;
static void (*const data_1EE2C)(int32_t, int32_t, int32_t, int32_t) = script_4285;
static void (*const data_1EE30)(int32_t, int32_t, int32_t, int32_t) = script_4285;
static void (*const data_1EE34)(int32_t, int32_t, int32_t, int32_t) = script_4285;
static void (*const data_1EE38)(int32_t, int32_t, int32_t, int32_t) = script_4285;
static void (*const data_1EE3C)(int32_t, int32_t, int32_t, int32_t) = script_4285;
static void (*const data_1EE40)(int32_t, int32_t, int32_t, int32_t) = script_4285;
static void (*const data_1EE44)(int32_t, int32_t, int32_t, int32_t) = script_4285;
static void (*const data_1EE48)(int32_t, int32_t, int32_t, int32_t) = script_452B;
static void (*const data_1EE4C)(int32_t, int32_t, int32_t, int32_t) = script_468C;
static void (*const data_1EE50)(int32_t, int32_t, int32_t, int32_t) = script_4B59;
static void (*const data_1EE54)(int32_t, int32_t, int32_t, int32_t) = script_4BC0;
static void (*const data_1EE58)(int32_t, int32_t, int32_t, int32_t) = script_4389;
static void (*const data_1EE5C)(int32_t, int32_t, int32_t, int32_t) = script_4389;
static void (*const data_1EE60)(int32_t, int32_t, int32_t, int32_t) = script_47ED;
static void (*const data_1EE64)(int32_t, int32_t, int32_t, int32_t) = script_4A8B;
static void (*const data_1EE68)(int32_t, int32_t, int32_t, int32_t) = script_4C27;
static void (*const data_1EE6C)(int32_t, int32_t, int32_t, int32_t) = script_452B;
static void (*const data_1EE70)(int32_t, int32_t, int32_t, int32_t) = script_468C;
static void (*const data_1EE74)(int32_t, int32_t, int32_t, int32_t) = script_47ED;
static void (*const data_1EE78)(int32_t, int32_t, int32_t, int32_t) = script_452B;
static void (*const data_1EE7C)(int32_t, int32_t, int32_t, int32_t) = script_468C;
static void (*const data_1EE80)(int32_t, int32_t, int32_t, int32_t) = script_47ED;
static void (*const data_1EE84)(int32_t, int32_t, int32_t, int32_t) = script_4C6E;
static void (*const data_1EE88)(int32_t, int32_t, int32_t, int32_t) = script_4C6E;
static void (*const data_1EE8C)(int32_t, int32_t, int32_t, int32_t) = script_4C6E;
static void (*const data_1EE90)(int32_t, int32_t, int32_t, int32_t) = script_4C6E;
static void (*const data_1EE94)(int32_t, int32_t, int32_t, int32_t) = script_4DA4;
static void (*const data_1EE98)(int32_t, int32_t, int32_t, int32_t) = script_5665;
static void (*const data_1EE9C)(int32_t, int32_t, int32_t, int32_t) = script_4285;
static void (*const data_1EEA0)(int32_t, int32_t, int32_t, int32_t) = script_4285;
static void (*const data_1EEA4)(int32_t, int32_t, int32_t, int32_t) = script_4285;
static void (*const data_1EEA8)(int32_t, int32_t, int32_t, int32_t) = script_4285;
static void (*const data_1EEAC)(int32_t, int32_t, int32_t, int32_t) = script_4285;
static void (*const data_1EEB0)(int32_t, int32_t, int32_t, int32_t) = script_4285;
static void (*const data_1EEB4)(int32_t, int32_t, int32_t, int32_t) = script_4285;
static void (*const data_1EEB8)(int32_t, int32_t, int32_t, int32_t) = script_4285;
static const char data_1EEBC[46] __attribute__((scn_string_literal)) = "Door Index = %d : call_type = %d : type = %d\012";
static const char data_1EEEA[43] __attribute__((scn_string_literal)) = "Door Open Motion = DEB_AKESIME_FUSUMA_L_F\012";
static const char data_1EF15[43] __attribute__((scn_string_literal)) = "Door Open Motion = DEB_AKESIME_FUSUMA_R_F\012";
static const char data_1EF40[42] __attribute__((scn_string_literal)) = "Door Open Motion = DEB_HIKISIME_DOOR_L_F\012";
static const char data_1EF6A[42] __attribute__((scn_string_literal)) = "Door Open Motion = DEB_HIKISIME_DOOR_R_F\012";
static const char data_1EF94[41] __attribute__((scn_string_literal)) = "Door Open Motion = DEB_OSISIME_DOOR_L_F\012";
static const char data_1EFBD[41] __attribute__((scn_string_literal)) = "Door Open Motion = DEB_OSISIME_DOOR_R_F\012";
static const char data_1EFE6[43] __attribute__((scn_string_literal)) = "Door Open Motion = GAK_AKESIME_FUSUMA_L_F\012";
static const char data_1F011[43] __attribute__((scn_string_literal)) = "Door Open Motion = GAK_AKESIME_FUSUMA_R_F\012";
static const char data_1F03C[42] __attribute__((scn_string_literal)) = "Door Open Motion = GAK_HIKISIME_DOOR_L_F\012";
static const char data_1F066[42] __attribute__((scn_string_literal)) = "Door Open Motion = GAK_HIKISIME_DOOR_R_F\012";
static const char data_1F090[41] __attribute__((scn_string_literal)) = "Door Open Motion = GAK_OSISIME_DOOR_L_F\012";
static const char data_1F0B9[41] __attribute__((scn_string_literal)) = "Door Open Motion = GAK_OSISIME_DOOR_R_F\012";
static const char data_1F0E2[43] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_AKESIME_FUSUMA_L_F\012";
static const char data_1F10D[43] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_AKESIME_FUSUMA_R_F\012";
static const char data_1F138[42] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_HIKISIME_DOOR_L_F\012";
static const char data_1F162[42] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_HIKISIME_DOOR_R_F\012";
static const char data_1F18C[41] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_OSISIME_DOOR_L_F\012";
static const char data_1F1B5[41] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_OSISIME_DOOR_R_F\012";
static const char data_1F1DE[43] __attribute__((scn_string_literal)) = "Door Open Motion = KOD_AKESIME_FUSUMA_L_F\012";
static const char data_1F209[43] __attribute__((scn_string_literal)) = "Door Open Motion = KOD_AKESIME_FUSUMA_R_F\012";
static const char data_1F234[42] __attribute__((scn_string_literal)) = "Door Open Motion = KOD_HIKISIME_DOOR_L_F\012";
static const char data_1F25E[42] __attribute__((scn_string_literal)) = "Door Open Motion = KOD_HIKISIME_DOOR_R_F\012";
static const char data_1F288[41] __attribute__((scn_string_literal)) = "Door Open Motion = KOD_OSISIME_DOOR_L_F\012";
static const char data_1F2B1[41] __attribute__((scn_string_literal)) = "Door Open Motion = KOD_OSISIME_DOOR_R_F\012";
static const char data_1F2DA[43] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_AKESIME_FUSUMA_L_F\012";
static const char data_1F305[43] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_AKESIME_FUSUMA_R_F\012";
static const char data_1F330[42] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_HIKISIME_DOOR_L_F\012";
static const char data_1F35A[42] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_HIKISIME_DOOR_R_F\012";
static const char data_1F384[41] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_OSISIME_DOOR_L_F\012";
static const char data_1F3AD[41] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_OSISIME_DOOR_R_F\012";
static const char data_1F3D6[43] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_AKESIME_FUSUMA_L_F\012";
static const char data_1F401[43] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_AKESIME_FUSUMA_R_F\012";
static const char data_1F42C[42] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_HIKISIME_DOOR_L_F\012";
static const char data_1F456[42] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_HIKISIME_DOOR_R_F\012";
static const char data_1F480[41] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_OSISIME_DOOR_L_F\012";
static const char data_1F4A9[41] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_OSISIME_DOOR_R_F\012";
static const char data_1F4D2[43] __attribute__((scn_string_literal)) = "Door Open Motion = SIN_AKESIME_FUSUMA_L_F\012";
static const char data_1F4FD[43] __attribute__((scn_string_literal)) = "Door Open Motion = SIN_AKESIME_FUSUMA_R_F\012";
static const char data_1F528[42] __attribute__((scn_string_literal)) = "Door Open Motion = SIN_HIKISIME_DOOR_L_F\012";
static const char data_1F552[42] __attribute__((scn_string_literal)) = "Door Open Motion = SIN_HIKISIME_DOOR_R_F\012";
static const char data_1F57C[41] __attribute__((scn_string_literal)) = "Door Open Motion = SIN_OSISIME_DOOR_L_F\012";
static const char data_1F5A5[41] __attribute__((scn_string_literal)) = "Door Open Motion = SIN_OSISIME_DOOR_R_F\012";
static const char data_1F5CE[43] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_AKESIME_FUSUMA_L_F\012";
static const char data_1F5F9[43] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_AKESIME_FUSUMA_R_F\012";
static const char data_1F624[42] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_HIKISIME_DOOR_L_F\012";
static const char data_1F64E[42] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_HIKISIME_DOOR_R_F\012";
static const char data_1F678[41] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_OSISIME_DOOR_L_F\012";
static const char data_1F6A1[41] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_OSISIME_DOOR_R_F\012";
static const char data_1F6CA[43] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_AKESIME_FUSUMA_L_F\012";
static const char data_1F6F5[43] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_AKESIME_FUSUMA_R_F\012";
static const char data_1F720[42] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_HIKISIME_DOOR_L_F\012";
static const char data_1F74A[42] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_HIKISIME_DOOR_R_F\012";
static const char data_1F774[41] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_OSISIME_DOOR_L_F\012";
static const char data_1F79D[41] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_OSISIME_DOOR_R_F\012";
static const char data_1F7C6[44] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_OSISIME_DOOR_AP_L_F\012";
static const char data_1F7F2[44] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_OSISIME_DOOR_AP_L_F\012";
static const char data_1F81E[44] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_OSISIME_DOOR_AP_L_F\012";
static const char data_1F84A[44] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_OSISIME_DOOR_AP_L_F\012";
static const char data_1F876[44] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_OSISIME_DOOR_AP_L_F\012";
static const char data_1F8A2[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_1F8BE[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_1F8DA[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_1F8F6[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_1F912[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_1F92E[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_1F94A[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_1F966[13] __attribute__((scn_string_literal)) = "open-move!!\012";
static const char data_1F973[11] __attribute__((scn_string_literal)) = "M_MDOR.BIN";
static const char data_1F97E[] __attribute__((scn_string_literal)) = "This Charactor Work is NULL!!\n";
static const char data_1F99D[22] __attribute__((scn_string_literal)) = "手動ドア開け番号指定\012";
static const char data_1F9B3[34] __attribute__((scn_string_literal)) = "無限ループになりそうなので抜ける\012";
static const char data_1F9D5[] __attribute__((scn_string_literal)) = "Don't make BOX Buffer!!\n";
static const char data_1F9EE[] __attribute__((scn_string_literal)) = "Don't make BOX Buffer!!\n";
static const char data_1FA07[15] __attribute__((scn_string_literal)) = "Door System : ";
static const char data_1FA16[15] __attribute__((scn_string_literal)) = "Door System : ";
static const char data_1FA25[18] __attribute__((scn_string_literal)) = "Door Box is ON!!\012";
static const char data_1FA37[19] __attribute__((scn_string_literal)) = "Door Box is OFF!!\012";
static const char data_1FA4A[18] __attribute__((scn_string_literal)) = "Door Box is ON!!\012";
static const char data_1FA5C[20] __attribute__((scn_string_literal)) = "Door Box is OFF!!\012";
static const DSWKDoorEntry data_1FA70[10] = {
  {.slot = 0, .flags = 0, .ctrl_mask_pair = 0x640064U, .coarse_metric = 0.0f, .selection_metric = 0.0f, .view_ang_delta = 0, .group = 0, .global1 = 0xFFFFFFFFU, .global2 = 0xFFFFFFFFU, .global3 = 0xFFFFFFFFU, .sndCommandId = 0xFFFFFFFFU, .global5 = 0xFFFFFFFFU, .global6 = 0xFFFFFFFFU},
  {.slot = 3, .flags = 0, .ctrl_mask_pair = 0x640064U, .coarse_metric = 0.0f, .selection_metric = 0.0f, .view_ang_delta = 0, .group = 0, .global1 = 0xFFFFFFFFU, .global2 = 0xFFFFFFFFU, .global3 = 0xFFFFFFFFU, .sndCommandId = 0xFFFFFFFFU, .global5 = 0xFFFFFFFFU, .global6 = 0xFFFFFFFFU},
  {.slot = 5, .flags = 0, .ctrl_mask_pair = 0x640064U, .coarse_metric = 0.0f, .selection_metric = 0.0f, .view_ang_delta = 0, .group = 0, .global1 = 0xFFFFFFFFU, .global2 = 0xFFFFFFFFU, .global3 = 0xFFFFFFFFU, .sndCommandId = 0xFFFFFFFFU, .global5 = 0xFFFFFFFFU, .global6 = 0xFFFFFFFFU},
  {.slot = 8, .flags = 0, .ctrl_mask_pair = 0x640064U, .coarse_metric = 0.0f, .selection_metric = 0.0f, .view_ang_delta = 0, .group = 0, .global1 = 0xFFFFFFFFU, .global2 = 0xFFFFFFFFU, .global3 = 0xFFFFFFFFU, .sndCommandId = 0xFFFFFFFFU, .global5 = 0xFFFFFFFFU, .global6 = 0xFFFFFFFFU},
  {.slot = 11, .flags = 0, .ctrl_mask_pair = 0x640064U, .coarse_metric = 0.0f, .selection_metric = 0.0f, .view_ang_delta = 0, .group = 0, .global1 = 0xFFFFFFFFU, .global2 = 0xFFFFFFFFU, .global3 = 0xFFFFFFFFU, .sndCommandId = 0xFFFFFFFFU, .global5 = 0xFFFFFFFFU, .global6 = 0xFFFFFFFFU},
  {.slot = 14, .flags = 0, .ctrl_mask_pair = 0x640064U, .coarse_metric = 0.0f, .selection_metric = 0.0f, .view_ang_delta = 0, .group = 0, .global1 = 0xFFFFFFFFU, .global2 = 0xFFFFFFFFU, .global3 = 0xFFFFFFFFU, .sndCommandId = 0xFFFFFFFFU, .global5 = 0xFFFFFFFFU, .global6 = 0xFFFFFFFFU},
  {.slot = 17, .flags = 0, .ctrl_mask_pair = 0x640064U, .coarse_metric = 0.0f, .selection_metric = 0.0f, .view_ang_delta = 0, .group = 0, .global1 = 0xFFFFFFFFU, .global2 = 0xFFFFFFFFU, .global3 = 0xFFFFFFFFU, .sndCommandId = 0xFFFFFFFFU, .global5 = 0xFFFFFFFFU, .global6 = 0xFFFFFFFFU},
  {.slot = 20, .flags = 0, .ctrl_mask_pair = 0x640064U, .coarse_metric = 0.0f, .selection_metric = 0.0f, .view_ang_delta = 0, .group = 0, .global1 = 0xFFFFFFFFU, .global2 = 0xFFFFFFFFU, .global3 = 0xFFFFFFFFU, .sndCommandId = 0xFFFFFFFFU, .global5 = 0xFFFFFFFFU, .global6 = 0xFFFFFFFFU},
  {.slot = 23, .flags = 0, .ctrl_mask_pair = 0x640064U, .coarse_metric = 0.0f, .selection_metric = 0.0f, .view_ang_delta = 0, .group = 0, .global1 = 0xFFFFFFFFU, .global2 = 0xFFFFFFFFU, .global3 = 0xFFFFFFFFU, .sndCommandId = 0xFFFFFFFFU, .global5 = 0xFFFFFFFFU, .global6 = 0xFFFFFFFFU},
  {.slot = 26, .flags = 0, .ctrl_mask_pair = 0x640064U, .coarse_metric = 0.0f, .selection_metric = 0.0f, .view_ang_delta = 0, .group = 0, .global1 = 0xFFFFFFFFU, .global2 = 0xFFFFFFFFU, .global3 = 0xFFFFFFFFU, .sndCommandId = 0xFFFFFFFFU, .global5 = 0xFFFFFFFFU, .global6 = 0xFFFFFFFFU}
};
static const int32_t data_1FC78[13] = {-1, 0, 6553700, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1};
static const uint32_t data_1FCAC = 8;
static const int32_t data_1FCB0[28] = {9, -1, 18, -1, 10, 11, -1, 16, 17, -1, 12, 13, -1, 6, 7, -1, 4, 5, -1, 14, 15, -1, 0, 1, -1, 2, 3, -1};
static char data_1FD20[] = "DR03_001";
static char data_1FD29[] = "DR00_005";
static char data_1FD32[] = "DR00_010";
static char data_1FD3B[] = "DR15_023";
static char data_1FD44[] = "DR15_022";
static char data_1FD4D[] = "DR15_019";
static char data_1FD56[] = "DR29_000";
static const uint8_t data_1FD5F[1] = {0};
static char *const data_1FD60 = data_1FD20;
static const int32_t data_1FD64[3] = {3, 1311403, 1311403};
static char *const data_1FD70 = data_1FD29;
static const int32_t data_1FD74[3] = {0, -1, -1};
static char *const data_1FD80 = data_1FD32;
static const int32_t data_1FD84[3] = {0, -1, -1};
static char *const data_1FD90 = data_1FD3B;
static const int32_t data_1FD94[3] = {15, 524971, 590507};
static char *const data_1FDA0 = data_1FD44;
static const int32_t data_1FDA4[3] = {15, 524971, 590507};
static char *const data_1FDB0 = data_1FD4D;
static const int32_t data_1FDB4[3] = {15, 524971, 590507};
static char *const data_1FDC0 = data_1FD56;
static const int32_t data_1FDC4[3] = {29, 787115, 852651};
static const MapObjectRecord data_1FDD0[19] = {
  {.kind = 2, .resourceIndex = 0, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-31.66427f, 5.73816f, 54.10421f}, .yawFixed16 = 32767},
  {.kind = 1, .resourceIndex = 1, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-32.114269f, 5.73816f, 54.089211f}, .yawFixed16 = 32767},
  {.kind = 1, .resourceIndex = 2, .scale = {2.0f, 1.0f, 1.0f}, .pos = {-24.212303f, 0.910839f, 72.7369f}, .yawFixed16 = -24407},
  {.kind = 2, .resourceIndex = 3, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-24.245352f, 0.859881f, 72.704903f}, .yawFixed16 = -24407},
  {.kind = 1, .resourceIndex = 2, .scale = {2.0f, 1.0f, 1.0f}, .pos = {-28.821768f, 0.961581f, 64.784927f}, .yawFixed16 = -2821},
  {.kind = 2, .resourceIndex = 4, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-28.821768f, 0.961581f, 64.784927f}, .yawFixed16 = -2821},
  {.kind = 1, .resourceIndex = 2, .scale = {2.0f, 1.0f, 1.0f}, .pos = {-48.615913f, 1.922171f, 52.863117f}, .yawFixed16 = 20714},
  {.kind = 2, .resourceIndex = 4, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-48.615913f, 1.903116f, 52.863117f}, .yawFixed16 = 20714},
  {.kind = 1, .resourceIndex = 2, .scale = {2.0f, 1.0f, 1.0f}, .pos = {-47.738293f, 1.926357f, 63.262829f}, .yawFixed16 = 8100},
  {.kind = 2, .resourceIndex = 3, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-47.706051f, 1.926357f, 63.295639f}, .yawFixed16 = 8100},
  {.kind = 1, .resourceIndex = 2, .scale = {2.0f, 1.0f, 1.0f}, .pos = {-42.962852f, 1.924001f, 48.054562f}, .yawFixed16 = 2586},
  {.kind = 2, .resourceIndex = 5, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-42.951557f, 1.924001f, 48.099155f}, .yawFixed16 = 2586},
  {.kind = 2, .resourceIndex = 0, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-34.594669f, 5.738156f, 55.182808f}, .yawFixed16 = 32767},
  {.kind = 1, .resourceIndex = 1, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-35.04467f, 5.738156f, 55.167809f}, .yawFixed16 = 32767},
  {.kind = 2, .resourceIndex = 0, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-34.594719f, 3.066169f, 55.182808f}, .yawFixed16 = 32767},
  {.kind = 1, .resourceIndex = 1, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-35.04472f, 3.066169f, 55.167809f}, .yawFixed16 = 32767},
  {.kind = 2, .resourceIndex = 0, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-31.664251f, 3.066169f, 54.10424f}, .yawFixed16 = 32767},
  {.kind = 1, .resourceIndex = 1, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-32.11425f, 3.066169f, 54.089241f}, .yawFixed16 = 32767},
  {.kind = 2, .resourceIndex = 6, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-53.360699f, 7.8159f, 100.2145f}, .yawFixed16 = 22321}
};
static void (*data_2007C)(void) = script_D9CD;
static void (*const data_20080)(int32_t) = script_D9D0;
static const uint32_t data_20084 = 0xC2680000;
static const float data_20088[2] = {0.0f, 85.0f};
static const uint32_t data_20090 = 0xC2180000;
static const float data_20094[2] = {0.0f, 107.0f};
static const uint32_t data_2009C = 0xC23C0000;
static const float data_200A0[2] = {0.0f, 22.0f};
static const uint32_t data_200A8 = 0xC1500000;
static const float data_200AC[2] = {0.0f, 26.0f};
static const uint32_t data_200B4 = 0xC2040000;
static const float data_200B8[2] = {0.0f, 34.0f};
static const uint32_t data_200C0 = 0xC0A00000;
static const float data_200C4[2] = {0.0f, 43.0f};
static const uint32_t data_200CC = 0xC2200000;
static const float data_200D0[2] = {0.0f, 20.0f};
static const uint8_t data_200D8[8] = {0};
static const float data_200E0 = 100.0f;
static const uint8_t data_200E4[8] = {0};
static const float data_200EC = 100.0f;
static const uint8_t data_200F0[8] = {0};
static const float data_200F8 = 50.0f;
static const uint32_t data_200FC = 0x3F800000;
static const float data_20100[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.8f, 0.7f, 0.5f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint32_t data_20184 = 0x3F800000;
static const float data_20188[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.8f, 0.7f, 0.5f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint32_t data_2020C = 0x3F800000;
static const float data_20210[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.8f, 0.7f, 0.5f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint32_t data_20294 = 0x3F800000;
static const float data_20298[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.8f, 0.7f, 0.5f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint32_t data_2031C = 0x3F800000;
static const float data_20320[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.8f, 0.7f, 0.5f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint32_t data_203A4 = 0x3F800000;
static const float data_203A8[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.8f, 0.7f, 0.5f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint8_t data_2042C[80] = {0};
typedef struct Row_2047C { float w0; float w1; float w2; int32_t w3; } Row_2047C;
static const Row_2047C data_2047C[3] = {
  {1.0f, 1.0f, 1.0f, 0},
  {0.0f, 0.0f, 0.0f, 0},
  {0.0f, 0.0f, 0.0f, 0}
};
static const int32_t data_204AC[2] = {0, 0};
static const uint8_t data_204B4[96] = {0};
static const float data_20514[10] = {0.8f, 0.8f, 0.8f, 0.8f, 0.8f, 0.8f, 0.8f, 0.8f, 0.8f, 0.8f};
static const uint32_t data_2053C = 0x3F800000;
static const float data_20540[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint32_t data_205C4 = 0x3F800000;
static const float data_205C8[92] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const int32_t data_20738[340] = {
  -10922, 0, 0, -10558, 0, 1456, 0, 0, 0, -18022, 0, 0, -17112, 0, 0, -18386, 0, 0, 0, 0, 0, -12196, 0, 0, -18204, 0, 0, -16748, 0, 0, 0, 0, 0,
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
  -1638, 0, 0, 1092
};
static const int32_t data_20C88[342] = {
  0, 0, -5461, 0, -2366, -1092, 0, -182, 1092, 0, -546, -6735, 0, 0, -4004, 0, 0, -6553, 0, -182, 1274, 0, 0, -13289, 0, 0, -5279, 0, 0, -8009, 0, 0,
  1456, 0, 182, -8738, 0, 0, -6007, 0, 0, -8738, 0, 182, 1638, 0, -728, -5279, 0, -1274, -6735, 0, -910, -8738, 0, 0, 1638, 0, 0, -9102, 0, -2366,
  -2002, 0, -182, 910, 0, -546, -2366, 0, 0, -2002, 0, 0, -2912, 0, -182, 1274, 0, 0, -14199, 0, 0, -11468, 0, 0, -13653, 0, 0, 1456, 0, 182, -11286,
  0, 0, -12014, 0, 0, -12743, 0, 182, 1638, 0, -728, -10194, 0, -1274, -12196, 0, -910, -10922, -728, 546, 1638, 0, 0, -1820, 0, -2366, -728, 0, -364,
  -1092, 0, -546, -11832, 0, 910, -6735, 0, 0, -6553, 0, -182, 1274, 0, 0, -13289, 0, 0, -6007, 0, 0, -10012, 0, 0, 1092, 0, 182, -8738, 0, 0, -6735,
  0, 0, -9830, 0, 182, 1638, 0, -728, -6553, 0, -1274, -7281, 0, -910, -10740, 0, 0, 1638, 0, 0, -5461, -2912, -2366, -364, -5279, 2548, -910, 0,
  -546, -8374, 0, 0, -9466, 0, 910, -4915, 0, -182, 546, 0, 0, -13289, 0, 0, -5279, 0, 364, -7645, 0, -182, 1456, 0, 182, -8738, 0, 0, -7281, 0, 182,
  -6917, 0, -182, 1638, 0, -728, -5279, 0, -1274, -6735, 0, -182, -8009, 0, 0, 1638, -4733, 0, -8556, 0, -2366, -1092, -6189, 2912, 0, 0, -546,
  -10922, 0, 1820, -16384, 0, 0, -11468, 0, 182, 546, 0, 0, -10376, 0, 0, -15837, 0, 0, -12743, 0, -182, 728, 0, 182, -13835, 0, 0, -16201, 0, 364,
  -10922, 0, -182, 728, 0, -728, -9648, 0, -3458, -12379, 0, -728, -7463, 0, 0, 1092, 0, 0, -5461, 0, -2366, -1638, 0, -910, 1092, 0, -546, -3640, 0,
  0, -2366, 0, 0, -1456, 0, -182, 1638, 0, 0, -910, 0, 0, -2730, 0, 0, -1274, 0, 0, 1820, 0, 182, -2730, 0, 0, -1820, 0, 0, -1274, 0, 182, 2184, 0,
  -728, -3094, 0, -910, -2548, 0, 910, -1456, -546, 910, 2366
};
static const int32_t data_211E0[57] = {
  0, 0, -7281, 0, 0, -4004, 0, 546, 182, 0, 0, -18204, 0, 0, -16930, 0, 0, -12379, 0, 0, 0, 0, 0, -12196, 0, 0, -18204, 0, 0, -13289, 0, 0, 0, 0, 0,
  -11832, 0, 0, -18386, 0, 0, -13835, 0, 0, 0, 0, 910, -12014, 0, -2184, -16748, 0, -2002, -13835, 0, 0, 364
};
static const int32_t data_212C4[57] = {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -8009, 0, 0, -5825, 0, 0, -3458, 0, 0, 0, 0, 0, -6553, 0, 0, -7281, 0, 0, -3640, 0, 0, 0, 0, 0, -4369, 0, 0, -7281,
  0, 0, -3640, 0, 0, 0, 0, 0, -7281, 0, 0, -5825, -2184, 0, -5825, -2912, 0, 0
};
static const int32_t data_213A8[57] = {
  0, 0, 0, 0, 0, 0, 0, -1638, 1456, 0, 0, 0, 0, 182, -5097, 0, 0, -3458, 0, 0, 0, 0, 0, 0, 0, 0, -4733, 0, 0, -3640, 0, 0, 0, 0, 0, -910, 0, 0, -6917,
  0, 0, -1820, 0, 0, 0, 0, 0, -1820, 0, 0, -6735, 0, -1456, -2002, 0, 0, 0
};
static const Vector3i data_2148C = {.x = -1274, .y = 910, .z = -1820};
static const int32_t data_21498[261] = {
  0, 0, -1820, 0, -182, -182, 0, 182, -2366, 0, -546, -8738, 0, 910, -10740, 0, 0, -4369, 0, -182, 1274, 0, 0, -15473, 0, 0, -19114, 0, 0, -11468, 0,
  0, 1092, 0, 182, -15109, 0, 0, -19114, 0, 0, -14927, 0, 182, 1638, 0, -182, -16566, 0, -1274, -20206, 0, -1638, -15291, 0, 0, 1274, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, -8009, 0, 0, -5825, 0, 0, -3458, 0, 0, 0, 0, 0, -6553, 0, 0, -7281, 0, 0, -3640, 0, 0, 0, 0, 0, -4369, 0, 0, -7281, 0, 0, -3640,
  0, 0, 0, 0, 0, -7281, 0, 0, -5825, -2184, 0, -5825, -2912, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1638, 1456, 0, 0, 0, 0, 182, -5097, 0, 0, -3458, 0, 0, 0, 0,
  0, 0, 0, 0, -4733, 0, 0, -3640, 0, 0, 0, 0, 0, -910, 0, 0, -6917, 0, 0, -1820, 0, 0, 0, 0, 0, -1820, 0, 0, -6735, 0, -1456, -2002, 0, 0, 0, 5643, 0,
  0, 0, 2548, 0, 6189, 2366, 0, 8374, 57344, 0, 0, 0, 63351, 0, 63169, 62987, 0, 62623, 3276, 62805, 63715, 0, 0, 0, 0, 0, 0, 0, 57344, 1456, 63715,
  0, 0, 0, 0, 0, 0, 0, 8192, 0, 0, 0, 4004, 0, 0, 5825, 0, 0, 57344, 1456, 63715, 0, 0, 0, 0, 0, 0, 0, 0, 61895, 0, 0, 1274, 0, 4187, 3094, 0, 3458,
  57344, 1456, 63715, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1820, 0, 4915, 2912, 0, 3822
};
static const int32_t data_218AC[20] = {57344, 0, 0, 0, 62259, 0, 63169, 60802, 0, 60620, 0, 0, 0, 0, 1820, 0, 4915, 2912, 0, 3822};
static const int32_t data_218FC[10] = {57344, 0, 0, 0, 62805, 0, 61713, 62259, 0, 62805};
static const int32_t data_21924[10] = {4369, 2548, 0, 0, 2548, 0, 4004, 2730, 0, 4915};
static const int32_t data_2194C[10] = {58982, 56251, 0, 0, 62805, 0, 61713, 62259, 0, 62805};
static const int32_t data_21974[10] = {4369, 61531, 0, 0, 2548, 0, 4004, 2730, 0, 4915};
static const int32_t data_2199C[20] = {60984, 61713, 0, 0, 62805, 0, 61713, 62259, 0, 62805, 4369, 61531, 0, 0, 2548, 0, 4004, 2730, 0, 4915};
static const int32_t data_219EC[10] = {62077, 0, 0, 0, 64261, 0, 61531, 62805, 0, 61531};
static const int32_t data_21A14[20] = {0, 62987, 0, 0, 3458, 0, 6007, 4004, 0, 3276, 57344, 1456, 63715, 0, 0, 0, 0, 0, 0, 0};
static const int32_t data_21A64[20] = {3276, 62805, 63715, 0, 0, 0, 0, 0, 0, 0, 57344, 1456, 63715, 0, 0, 0, 0, 0, 0, 0};
static const int32_t data_21AB4[20] = {0, 62805, 0, 0, 4915, 0, 4551, 5279, 0, 4551, 57344, 1456, 63715, 0, 0, 0, 0, 0, 0, 0};
static const int32_t data_21B04[10] = {64261, 62259, 0, 0, 3458, 0, 6189, 2002, 0, 8738};
static const int32_t data_21B2C[10] = {0, 5461, 0, 0, 0, 0, 0, 0, 0, 0};
static const int32_t data_21B54[20] = {0, 0, 0, 0, 1820, 0, 4915, 2912, 0, 3822, 57344, 0, 0, 0, 62259, 0, 63169, 60802, 0, 60620};
static const int32_t data_21BA4[20] = {0, 58982, 0, 0, 4369, 0, 0, 3094, 0, 0, 57344, 0, 0, 0, 62259, 0, 63169, 60802, 0, 60620};
static const FogColourData data_21BF4 = {.red = 14.1f, .green = 800.0f, .blue = 0.48f, .colour = 0x7F9FD1FFU};
static const uint32_t data_21C04[4] = {0, 0, 0xBB00663CU, 0xBB00663CU};
static const FogColourData data_21C14 = {.red = 14.1f, .green = 800.0f, .blue = 0.48f, .colour = 0x7F9FD1FFU};
static const uint32_t data_21C24[61] = {
  0, 0, 0xBB00663CU, 0xBB00663CU, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};
static const int32_t data_21D18[228] = {
  0, 0, -3640, 0, 0, -2366, -4369, 1820, -546, 0, 0, -5279, 0, 0, -6007, 0, 0, -2912, 0, 0, -910, 0, 0, -6553, 0, 0, -6735, 0, 0, -3640, 0, 0, -182,
  0, 0, -5461, 0, 0, -6007, 0, 0, -2366, 0, 0, 0, 0, 0, -5643, 0, 0, -4733, 0, 0, -2366, -1456, 0, 728, 0, 0, -9466, 0, -546, -2912, 0, 3094, -2366,
  0, 0, -18386, 0, 0, -19296, 0, 1092, -14381, 0, 0, 0, 0, 0, -16748, 0, 0, -17658, 0, 0, -15109, 0, 0, 0, 0, 0, -16566, 0, 0, -18204, 0, 0, -14381,
  0, 0, 0, 0, 0, -19478, 0, 0, -18386, 0, -4004, -12925, 0, 0, -546, 0, 0, -3276, 0, 0, 0, 0, 0, 0, 0, 0, -6553, 0, 0, -4369, 0, -546, -3458, 0, 0, 0,
  0, 0, -5461, 0, 0, -5643, 0, 0, -3458, 0, 0, 0, 0, 0, -6007, 0, 0, -4551, 0, 546, -2548, 0, 0, 0, 0, 0, -6189, 0, -1092, -3094, 0, 0, -2548, 0, 0,
  0, 0, 0, -6007, 0, 0, -728, 0, 0, 0, 0, 0, -5097, 0, 0, -2184, 0, 0, -2002, 0, 0, 0, 0, 0, -5825, 0, 0, -2912, 0, 0, -728, 0, 0, 0, 0, 0, -4915, 0,
  0, -4187, 0, 0, -1274, 0, 0, 0, 0, 0, -6553, 0, -364, -4369, 0, -546, -1456, 0, 0, 0
};
static const char data_220A8[6] __attribute__((scn_string_literal)) = "01CJP";
static const char data_220AE[13] __attribute__((scn_string_literal)) = "seqdata0.bin";
static const char data_220BB[6] __attribute__((scn_string_literal)) = "CATA3";
static const char data_220C1[13] __attribute__((scn_string_literal)) = "seqdata1.bin";
static const char data_220CE[6] __attribute__((scn_string_literal)) = "CATA3";
static const char data_220D4[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_220D9[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_220DE[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_220E3[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_220E8[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_220ED[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_220F2[11] __attribute__((scn_string_literal)) = "bgm051.snd";
static const char data_220FD[13] __attribute__((scn_string_literal)) = "a1_nekot.snd";
static const char data_2210A[6] __attribute__((scn_string_literal)) = "01CJP";
static const int32_t data_22110[57] = {
  0, 0, -1638, 0, 0, -2184, 0, 0, 0, 0, 0, -3276, 0, 0, -6553, 0, 0, -6553, 0, 0, 0, 0, 0, -4551, 0, 0, -6553, 0, 0, -6917, 0, 0, 0, 0, 0, -4733, 0,
  0, -5461, 0, 0, -6735, 0, 0, 0, 0, 0, -8738, 0, -2912, -7463, 0, -1092, -4733, 0, 0, 0
};
static const Vector3i data_221F4 = {.x = -2730, .y = 2548, .z = -1638};
static const float data_22200[4] = {0.3f, 0.3f, 0.3f, 0.3f};
static const Vector4 data_22210 = {.x = 0.0f, .y = 0.4f, .z = 0.4f, .w = 0.4f};
static const char data_22220[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_22225[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_2222A[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_2222F[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_22234[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_22239[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_2223E[7] __attribute__((scn_string_literal)) = "SELECT";
static const char data_22245[13] __attribute__((scn_string_literal)) = "f1jutaup.snd";
static const char data_22252[6] __attribute__((scn_string_literal)) = "CATA3";
static const char data_22258[] __attribute__((scn_string_literal)) = "E1041A159";
static const char data_22262[] __attribute__((scn_string_literal)) = "E1041A160";
static const char data_2226C[] __attribute__((scn_string_literal)) = "E1041A161";
static const char data_22276[] __attribute__((scn_string_literal)) = "E1041A118";
static const char data_22280[] __attribute__((scn_string_literal)) = "E1041A119";
static const char data_2228A[] __attribute__((scn_string_literal)) = "E1041A120";
static const char data_22294[] __attribute__((scn_string_literal)) = "E1041A118";
static const char data_2229E[] __attribute__((scn_string_literal)) = "E1041A119";
static const char data_222A8[] __attribute__((scn_string_literal)) = "E1041A120";
static const char data_222B2[] __attribute__((scn_string_literal)) = "E1041A118";
static const char data_222BC[] __attribute__((scn_string_literal)) = "E1041A119";
static const char data_222C6[] __attribute__((scn_string_literal)) = "E1041A120";
static const char data_222D0[] __attribute__((scn_string_literal)) = "E1041A118";
static const char data_222DA[] __attribute__((scn_string_literal)) = "E1041A119";
static const char data_222E4[] __attribute__((scn_string_literal)) = "E1041A120";
static const char data_222EE[] __attribute__((scn_string_literal)) = "E1041A118";
static const char data_222F8[] __attribute__((scn_string_literal)) = "E1041A119";
static const char data_22302[] __attribute__((scn_string_literal)) = "E1041A120";
static const char data_2230C[] __attribute__((scn_string_literal)) = "E1041D007";
static const char data_22316[] __attribute__((scn_string_literal)) = "E1041E011";
static const char data_22320[] __attribute__((scn_string_literal)) = "E1041A027";
static const char data_2232A[] __attribute__((scn_string_literal)) = "E1041D010";
static const char data_22334[] __attribute__((scn_string_literal)) = "E1041A112";
static const char data_2233E[] __attribute__((scn_string_literal)) = "E1041E002";
static const char data_22348[] __attribute__((scn_string_literal)) = "E1041D002";
static const char data_22352[] __attribute__((scn_string_literal)) = "E1041E008";
static const char data_2235C[] __attribute__((scn_string_literal)) = "E1041D008";
static const char data_22366[] __attribute__((scn_string_literal)) = "E1041A113";
static const char data_22370[] __attribute__((scn_string_literal)) = "E1041D001";
static const char data_2237A[] __attribute__((scn_string_literal)) = "E1041E011";
static const char data_22384[] __attribute__((scn_string_literal)) = "E1041A027";
static const char data_2238E[] __attribute__((scn_string_literal)) = "E1041D010";
static const char data_22398[] __attribute__((scn_string_literal)) = "E1041A138";
static const char data_223A2[] __attribute__((scn_string_literal)) = "E1041E002";
static const char data_223AC[] __attribute__((scn_string_literal)) = "E1041D002";
static const char data_223B6[] __attribute__((scn_string_literal)) = "E1041E012";
static const char data_223C0[] __attribute__((scn_string_literal)) = "E1041D003";
static const char data_223CA[] __attribute__((scn_string_literal)) = "E1041A029";
static const char data_223D4[] __attribute__((scn_string_literal)) = "E1041C011";
static const char data_223DE[] __attribute__((scn_string_literal)) = "E1041A021";
static const char data_223E8[] __attribute__((scn_string_literal)) = "E1041C002";
static const char data_223F2[] __attribute__((scn_string_literal)) = "E1041C012";
static const char data_223FC[] __attribute__((scn_string_literal)) = "E1041A111";
static const char data_22406[] __attribute__((scn_string_literal)) = "E1041C004";
static const char data_22410[] __attribute__((scn_string_literal)) = "E1041A166";
static const char data_2241A[] __attribute__((scn_string_literal)) = "E1041C013";
static const char data_22424[] __attribute__((scn_string_literal)) = "E1041A023";
static const char data_2242E[] __attribute__((scn_string_literal)) = "E1041C015";
static const char data_22438[] __attribute__((scn_string_literal)) = "E1041C001";
static const char data_22442[] __attribute__((scn_string_literal)) = "E1041A021";
static const char data_2244C[] __attribute__((scn_string_literal)) = "E1041C002";
static const char data_22456[] __attribute__((scn_string_literal)) = "E1041C003";
static const char data_22460[] __attribute__((scn_string_literal)) = "E1041A135";
static const char data_2246A[] __attribute__((scn_string_literal)) = "E1041C004";
static const char data_22474[] __attribute__((scn_string_literal)) = "E1041A131";
static const char data_2247E[] __attribute__((scn_string_literal)) = "E1041C005";
static const char data_22488[] __attribute__((scn_string_literal)) = "E1041A023";
static const char data_22492[] __attribute__((scn_string_literal)) = "E1041D010";
static const char data_2249C[] __attribute__((scn_string_literal)) = "E1041B047";
static const char data_224A6[] __attribute__((scn_string_literal)) = "E1041A165";
static const char data_224B0[] __attribute__((scn_string_literal)) = "E1041B051";
static const char data_224BA[] __attribute__((scn_string_literal)) = "E1041B048";
static const char data_224C4[] __attribute__((scn_string_literal)) = "E1041A014";
static const char data_224CE[] __attribute__((scn_string_literal)) = "E1041B052";
static const char data_224D8[] __attribute__((scn_string_literal)) = "E1041A166";
static const char data_224E2[] __attribute__((scn_string_literal)) = "E1041B053";
static const char data_224EC[] __attribute__((scn_string_literal)) = "E1041A132";
static const char data_224F6[] __attribute__((scn_string_literal)) = "E1041B054";
static const char data_22500[] __attribute__((scn_string_literal)) = "E1041B001";
static const char data_2250A[] __attribute__((scn_string_literal)) = "E1041A130";
static const char data_22514[] __attribute__((scn_string_literal)) = "E1041B051";
static const char data_2251E[] __attribute__((scn_string_literal)) = "E1041B003";
static const char data_22528[] __attribute__((scn_string_literal)) = "E1041A014";
static const char data_22532[] __attribute__((scn_string_literal)) = "E1041B052";
static const char data_2253C[] __attribute__((scn_string_literal)) = "E1041A131";
static const char data_22546[] __attribute__((scn_string_literal)) = "E1041B053";
static const char data_22550[] __attribute__((scn_string_literal)) = "E1041A132";
static const char data_2255A[] __attribute__((scn_string_literal)) = "E1041B054";
static const char data_22564[] __attribute__((scn_string_literal)) = "E1041A122";
static const char data_2256E[] __attribute__((scn_string_literal)) = "E1041A123";
static const char data_22578[] __attribute__((scn_string_literal)) = "E1041A162";
static const char data_22582[] __attribute__((scn_string_literal)) = "E1041A122";
static const char data_2258C[] __attribute__((scn_string_literal)) = "E1041A123";
static const char data_22596[] __attribute__((scn_string_literal)) = "E1041A124";
static const Vector4 data_225A0 = {.x = 0.0f, .y = 0.3f, .z = 0.3f, .w = 0.3f};
static const Vector4 data_225B0 = {.x = 0.0f, .y = 0.3f, .z = 0.3f, .w = 0.3f};
static const char data_225C0[] __attribute__((scn_string_literal)) = "E1041A139";
static const char data_225CA[] __attribute__((scn_string_literal)) = "E1041A140";
static const char data_225D4[] __attribute__((scn_string_literal)) = "E1041A141";
static const char data_225DE[] __attribute__((scn_string_literal)) = "E1041A142";
static const char data_225E8[] __attribute__((scn_string_literal)) = "E1041A046";
static const char data_225F2[] __attribute__((scn_string_literal)) = "E1041A145";
static const char data_225FC[] __attribute__((scn_string_literal)) = "E1041A146";
static const char data_22606[] __attribute__((scn_string_literal)) = "E1041A049";
static const char data_22610[] __attribute__((scn_string_literal)) = "E1041A147";
static const char data_2261A[] __attribute__((scn_string_literal)) = "E1041A148";
static const char data_22624[] __attribute__((scn_string_literal)) = "E1041A040";
static const char data_2262E[] __attribute__((scn_string_literal)) = "E1041A143";
static const char data_22638[] __attribute__((scn_string_literal)) = "E1041A144";
static const char data_22642[] __attribute__((scn_string_literal)) = "E1041A037";
static const char data_2264C[] __attribute__((scn_string_literal)) = "E1041A054";
static const char data_22656[] __attribute__((scn_string_literal)) = "E1041A139";
static const char data_22660[] __attribute__((scn_string_literal)) = "E1041A121";
static const char data_2266A[] __attribute__((scn_string_literal)) = "E1041A149";
static const char data_22674[] __attribute__((scn_string_literal)) = "E1041A150";
static const char data_2267E[] __attribute__((scn_string_literal)) = "E1041A151";
static const uint16_t data_22688[7] = {1068, 1043, 1069, 1076, 1074, 1072, 1073};
static const char data_22696[] __attribute__((scn_string_literal)) = "E1041A116";
static const char data_226A0[] __attribute__((scn_string_literal)) = "E1041A170";
static const char data_226AA[] __attribute__((scn_string_literal)) = "E1041A171";
static const char data_226B4[] __attribute__((scn_string_literal)) = "E1041A172";
static const uint8_t data_226BE[2] = {0};
static const Vector4 data_226C0 = {.x = 0.0f, .y = 0.3f, .z = 0.3f, .w = 0.3f};
static const uint16_t data_226D0[8] = {1068, 1043, 1069, 1076, 1074, 1072, 1073, 0};
static const uint16_t data_226E0[7] = {4368, 4368, 4368, 4368, 4368, 4368, 4368};
static const char data_226EE[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_226F3[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_226F8[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_226FD[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_22702[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_22707[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_2270C[11] __attribute__((scn_string_literal)) = "bgm018.snd";
static const char data_22717[13] __attribute__((scn_string_literal)) = "f1jutaup.snd";
static const char data_22724[] __attribute__((scn_string_literal)) = "E1041A168";
static const char data_2272E[] __attribute__((scn_string_literal)) = "E1041F001";
static const char data_22738[] __attribute__((scn_string_literal)) = "E1041A169";
static const char data_22742[] __attribute__((scn_string_literal)) = "E1041F002";
static const char data_2274C[] __attribute__((scn_string_literal)) = "E1041A081";
static const char data_22756[] __attribute__((scn_string_literal)) = "E1041F008";
static const char data_22760[] __attribute__((scn_string_literal)) = "E1041F004";
static const char data_2276A[] __attribute__((scn_string_literal)) = "E1041B023";
static const char data_22774[] __attribute__((scn_string_literal)) = "E1041B026";
static const char data_2277E[] __attribute__((scn_string_literal)) = "E1041B027";
static const char data_22788[] __attribute__((scn_string_literal)) = "E1041B024";
static const char data_22792[] __attribute__((scn_string_literal)) = "E1041B028";
static const char data_2279C[] __attribute__((scn_string_literal)) = "E1041F005";
static const char data_227A6[] __attribute__((scn_string_literal)) = "E1041B060";
static const char data_227B0[] __attribute__((scn_string_literal)) = "E1041F006";
static const char data_227BA[] __attribute__((scn_string_literal)) = "E1041B061";
static const char data_227C4[] __attribute__((scn_string_literal)) = "E1041A158";
static const char data_227CE[] __attribute__((scn_string_literal)) = "E1041F009";
static const char data_227D8[] __attribute__((scn_string_literal)) = "E1041A081";
static const char data_227E2[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_227E7[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_227EC[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_227F1[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_227F6[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_227FB[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_22800[7] __attribute__((scn_string_literal)) = "SELECT";
static const char data_22807[13] __attribute__((scn_string_literal)) = "f1jutaup.snd";
static const char data_22814[19] __attribute__((scn_string_literal)) = "EVSN: Read Failed\012";
static const char data_22827[6] __attribute__((scn_string_literal)) = "E1041";
static const char data_2282D[13] __attribute__((scn_string_literal)) = "HMOT_CAT.BIN";
static const char data_2283A[] __attribute__((scn_string_literal)) = "/scene/03/JU00/";
static const char data_2284A[] __attribute__((scn_string_literal)) = "CATA3";
static const Vector4 data_22850 = {.x = 0.0f, .y = 0.3f, .z = 0.3f, .w = 0.3f};
static const char data_22860[4] __attribute__((scn_string_literal)) = "%f";
static const uint32_t data_22864 = 0xFFFF0000;
static const uint8_t pad_22868[4] = {0};
static const uint32_t data_2286C = 0x40008F16;
static const uint32_t data_22870 = 0xC20FF958;
static const uint32_t data_22874 = 0x4084B852;
static const uint32_t data_22878 = 0x424F7E77;
static const uint8_t pad_2287C[8] = {0};
static const uint32_t data_22884 = 0xFFFF0000;
static const uint32_t data_22888 = 1;
static const uint32_t data_2288C = 0x400080E4;
static const uint32_t data_22890 = 0xC20EBA44;
static const uint32_t data_22894 = 0x408E5F07;
static const uint32_t data_22898 = 0x425C491D;
static const uint8_t pad_2289C[8] = {0};
static const uint32_t data_228A4 = 0xFFFF0000;
static const uint32_t data_228A8 = 2;
static const uint32_t data_228AC = 0x400082DC;
static const uint32_t data_228B0 = 0xC202C659;
static const uint32_t data_228B4 = 0x408E9AD4;
static const uint32_t data_228B8 = 0x4257547B;
static const uint8_t pad_228BC[8] = {0};
static const uint32_t data_228C4 = 0xFFFF0000;
static const uint32_t data_228C8 = 3;
static const uint32_t data_228CC = 0x40008008;
static const uint32_t data_228D0 = 0xC20EB141;
static const uint32_t data_228D4 = 0x40E4E979;
static const uint32_t data_228D8 = 0x425C97DC;
static const uint8_t pad_228DC[8] = {0};
static const uint32_t data_228E4 = 0xFFFF0000;
static const uint32_t data_228E8 = 4;
static const uint32_t data_228EC = 0x4000813A;
static const uint32_t data_228F0 = 0xC202C16F;
static const uint32_t data_228F4 = 0x40E4E979;
static const uint32_t data_228F8 = 0x42583DF4;
static const uint8_t pad_228FC[8] = {0};
static const uint32_t data_22904 = 0xFFFF0000;
static const uint32_t data_22908 = 5;
static const uint32_t data_2290C = 0x40000F3D;
static const uint32_t data_22910 = 0xC22F2E2F;
static const uint32_t data_22914 = 0x404B27BB;
static const uint32_t data_22918 = 0x4243E738;
static const uint8_t pad_2291C[8] = {0};
static const uint32_t data_22924 = 0xFFFF0000;
static const uint32_t data_22928 = 6;
static const uint32_t data_2292C = 0x40005228;
static const uint32_t data_22930 = 0xC2409931;
static const uint32_t data_22934 = 0x4056FEC5;
static const uint32_t data_22938 = 0x4257FB99;
static const uint8_t pad_2293C[8] = {0};
static const uint32_t data_22944 = 0xFFFF0000;
static const uint32_t data_22948 = 8;
static const uint32_t data_2294C = 0x8000071C;
static const uint32_t data_22950 = 0xC23311B7;
static const uint32_t data_22954 = 0x40066666;
static const uint32_t data_22958 = 0x4283AF4F;
static const uint8_t pad_2295C[8] = {0};
static const uint32_t data_22964 = 0xFFFF0000;
static const uint32_t data_22968 = 9;
static const uint32_t data_2296C = 0x8000871C;
static const uint32_t data_22970 = 0xC23311B7;
static const uint32_t data_22974 = 0x40066666;
static const uint32_t data_22978 = 0x4283AF4F;
static const uint8_t pad_2297C[8] = {0};
static const uint32_t data_22984 = 0xFFFF0000;
static const uint32_t data_22988 = 7;
static const uint32_t data_2298C = 0x4000207E;
static const uint32_t data_22990 = 0xC242A4DD;
static const uint32_t data_22994 = 0x405EB9F5;
static const uint32_t data_22998 = 0x42805D98;
static const uint8_t pad_2299C[8] = {0};
static const uint32_t data_229A4 = 0xFFFF0000;
static const uint32_t data_229A8 = 10;
static const uint32_t data_229AC = 0x4000F178;
static const uint32_t data_229B0 = 0xC1DCDABA;
static const uint32_t data_229B4 = 0x40269C78;
static const uint32_t data_229B8 = 0x42823D49;
static const uint8_t pad_229BC[8] = {0};
static const uint32_t data_229C4 = 0xFFFF0000;
static const uint32_t data_229C8 = 11;
static const uint32_t data_229CC = 0x4000A098;
static const uint32_t data_229D0 = 0xC1C852F2;
static const uint32_t data_229D4 = 0x401B0A3D;
static const uint32_t data_229D8 = 0x4292FD56;
static const uint8_t pad_229DC[8] = {0};
static const uint32_t data_229E4 = 0xFFFF0000;
static const uint32_t data_229E8 = 12;
static const uint32_t data_229EC = 0x4000871C;
static const uint32_t data_229F0 = 0xC1C7F247;
static const uint32_t data_229F4 = 0x3FB33333;
static const uint32_t data_229F8 = 0x4296D439;
static const uint8_t pad_229FC[8] = {0};
static const uint32_t data_22A04 = 0xFFFF0000;
static const uint32_t data_22A08 = 13;
static const uint32_t data_22A0C = 0x40002CB6;
static const uint32_t data_22A10 = 0xC213AE14;
static const uint32_t data_22A14 = 0x408EBFB1;
static const uint32_t data_22A18 = 0x421E0189;
static const uint8_t pad_22A1C[8] = {0};
static const uint32_t data_22A24 = 0xFFFF0000;
static const uint32_t data_22A28 = 14;
static const uint32_t data_22A2C = 0x400000DC;
static const uint32_t data_22A30 = 0xC21E8ADB;
static const uint32_t data_22A34 = 0x408EBFB1;
static const uint32_t data_22A38 = 0x4224860B;
static const uint8_t pad_22A3C[8] = {0};
static const uint32_t data_22A44 = 0xFFFF0000;
static const uint32_t data_22A48 = 15;
static const uint32_t data_22A4C = 0x4000157B;
static const uint32_t data_22A50 = 0xC21C8BE1;
static const uint32_t data_22A54 = 0x4073E282;
static const uint32_t data_22A58 = 0x422AA3BD;
static const uint8_t pad_22A5C[8] = {0};
static const uint32_t data_22A64 = 0xFFFF0000;
static const uint32_t data_22A68 = 16;
static const uint32_t data_22A6C = 0x4000157B;
static const uint32_t data_22A70 = 0xC2200396;
static const uint32_t data_22A74 = 0x40ACCCCD;
static const uint32_t data_22A78 = 0x42147838;
static const uint8_t pad_22A7C[8] = {0};
static const uint32_t data_22A84 = 257;
static const uint32_t data_22A88 = SHEN_NAME('CATM');
static const uint8_t pad_22A8C[24] = {0};
static const uint8_t data_22AA4[2] = {
  0xFF, 0xFF
};
static const char data_22AA6[11] __attribute__((scn_string_literal)) = "bgm109.snd";
static const char data_22AB1[7] __attribute__((scn_string_literal)) = "SELECT";
static const FogColourData data_22AB8 = {.red = 20.0f, .green = 800.0f, .blue = 0.63f, .colour = 0xFF0F0004U};
static const uint32_t data_22AC8[4] = {0x805F10A0U, 0x805F10A0U, 0, 0};
static const Vector3 data_22AD8 = {.x = -31.1651f, .y = 5.086f, .z = 47.664902f};
static const Vector3 data_22AE4 = {.x = -39.887001f, .y = 5.096f, .z = 65.281998f};
static const Vector3i data_22AF0 = {.x = 65353, .y = 37501, .z = 0};
static const Vector3i data_22AFC = {.x = 65353, .y = 24393, .z = 0};
static const char data_22B08[16] __attribute__((scn_string_literal)) = "/scene/03/JU00/";
static const char data_22B18[5] __attribute__((scn_string_literal)) = "MAPZ";
static const char data_22B1D[16] __attribute__((scn_string_literal)) = "/scene/03/JU00/";
static const char data_22B2D[5] __attribute__((scn_string_literal)) = "MAPN";
static const char data_22B32[16] __attribute__((scn_string_literal)) = "/scene/03/JU00/";
static const char data_22B42[5] __attribute__((scn_string_literal)) = "MAPS";
static const char data_22B47[16] __attribute__((scn_string_literal)) = "/scene/03/JU00/";
static const char data_22B57[9] __attribute__((scn_string_literal)) = "SHUKI";
static const uint32_t data_22B60 = 0xC247B958;
static const float data_22B64 = 7.0f;
static const uint32_t data_22B68 = 0x42C403FEU;
static const int32_t data_22B6C[3] = {22499, 33024, 3};
static const uint32_t data_22B78 = 0xC249BF48U;
static const float data_22B7C = 7.6498f;
static const uint32_t data_22B80 = 0x42C4A3B0U;
static const int32_t data_22B84[3] = {55267, 33032, 3};
static const float data_22B90[2] = {-29.5634f, 2.27f};
static const uint32_t data_22B98 = 0x428D201AU;
static const int32_t data_22B9C[3] = {16748, 256, 4};
static const uint32_t data_22BA8 = 0xC1F3A95FU;
static const float data_22BAC = 2.95f;
static const uint32_t data_22BB0 = 0x428D204FU;
static const int32_t data_22BB4[3] = {49516, 264, 4};
static const uint32_t data_22BC0 = 0xC1E88E22U;
static const float data_22BC4 = 1.15f;
static const uint32_t data_22BC8 = 0x428679C1U;
static const int32_t data_22BCC[3] = {35498, 256, 5};
static const float data_22BD8[2] = {-28.8409f, 2.05f};
static const uint32_t data_22BE0 = 0x4288D6FDU;
static const int32_t data_22BE4[3] = {2730, 264, 5};
static const uint32_t data_22BF0 = 0xC20E57C2U;
static const float data_22BF4 = 2.1f;
static const uint32_t data_22BF8 = 0x42328A58U;
static const int32_t data_22BFC[3] = {5279, 8448, 3};
static const float data_22C08[2] = {-35.8936f, 2.6f};
static const uint32_t data_22C10 = 0x423093DEU;
static const int32_t data_22C14[3] = {38065, 8456, 3};
static const uint32_t data_22C20 = 0xC2148B44U;
static const float data_22C24 = 2.135f;
static const uint32_t data_22C28 = 0x4250B382U;
static const int32_t data_22C2C[3] = {36044, 256, 5};
static const uint32_t data_22C38 = 0xC2130A23U;
static const float data_22C3C[2] = {3.0506f, 53.3214f};
static const int32_t data_22C44[3] = {3276, 264, 5};
static const float data_22C50[3] = {-37.6507f, 2.7961f, 41.2066f};
static const int32_t data_22C5C[3] = {5461, 8194, 2};
static const uint32_t data_22C68 = 0xC217B368U;
static const float data_22C6C = 3.242f;
static const uint32_t data_22C70 = 0x4222D7A8U;
static const int32_t data_22C74[3] = {38229, 8202, 3};
static const uint32_t data_22C80 = 0xC219AD91U;
static const float data_22C84 = 3.4781f;
static const uint32_t data_22C88 = 0x421F6E2FU;
static const int32_t data_22C8C[3] = {5461, 40962, 3};
static const uint32_t data_22C98 = 0xC21B229CU;
static const float data_22C9C = 3.9411f;
static const uint32_t data_22CA0 = 0x421CEC3DU;
static const int32_t data_22CA4[3] = {38229, 24586, 3};
static const uint32_t data_22CB0 = 0xC21B4E70U;
static const float data_22CB4 = 3.24f;
static const uint32_t data_22CB8 = 0x426C8A3DU;
static const int32_t data_22CBC[3] = {364, 16384, 12};
static const float data_22CC8[2] = {-38.9445f, 5.71f};
static const uint32_t data_22CD0 = 0x425F53A9U;
static const int32_t data_22CD4[3] = {33241, 16392, 12};
static const float data_22CE0 = 100000.0f;
static const uint8_t data_22CE4[8] = {0};
typedef struct Row_22CEC { float w0; int32_t w1; int32_t w2; } Row_22CEC;
static const Row_22CEC data_22CEC[9] = {
  {1.0f, 0, 1},
  {1.0f, 0, 2},
  {0.5f, 0, 3},
  {0.7f, 0, 4},
  {0.7f, 0, 5},
  {0.7f, 0, 6},
  {1.6f, 0, 7},
  {1.6f, 0, 8},
  {0.3f, 0, 9}
};
static const float data_22D58 = 0.3f;
static const int32_t data_22D5C = 3;
static const char data_22D60[] __attribute__((scn_string_literal)) = "/scene/03/JU00/";
static const char data_22D70[] __attribute__((scn_string_literal)) = "JYUU";
static const char data_22D75[5] __attribute__((scn_string_literal)) = "JYUU";
static const char data_22D7A[1] __attribute__((scn_string_literal)) = "";
static const char data_22D7B[5] __attribute__((scn_string_literal)) = "JYUU";
static const char data_22D80[1] __attribute__((scn_string_literal)) = "";
static const char data_22D81[11] __attribute__((scn_string_literal)) = "bgm109.snd";
static const char data_22D8C[7] __attribute__((scn_string_literal)) = "SELECT";
static const char data_22D93[5] __attribute__((scn_string_literal)) = "JYUU";
static const uint32_t data_22D98 = 0xC182147B;
static const float data_22D9C[139] = {
  74.84f, -20.47f, 79.12f, -26.0f, 73.74f, -21.79f, 69.46f, 0.91f, 3.66f, -23.66f, 1.91f, 73.72f, 1.64f, -19.91f, 68.24f, -25.88f, 69.79f, -26.83f,
  65.72f, -20.86f, 64.17f, 0.91f, 4.43f, -24.75f, 1.91f, 67.39f, 1.64f, -26.7f, 64.43f, -27.13f, 66.01f, -30.98f, 64.91f, -30.5f, 63.35f, 0.91f,
  4.43f, -28.84f, 1.91f, 64.67f, 1.0f, -45.96f, 62.26f, -50.25f, 66.46f, -55.64f, 60.95f, -51.35f, 56.74f, 1.92f, 4.73f, -48.29f, 2.92f, 62.23f,
  2.04f, -47.14f, 54.41f, -49.22f, 55.39f, -50.88f, 51.75f, -48.79f, 50.77f, 1.92f, 5.54f, -48.72f, 2.92f, 52.82f, 1.0f, -40.75f, 48.1f, -44.92f,
  49.17f, -45.55f, 46.72f, -41.39f, 45.65f, 1.92f, 4.63f, -43.04f, 2.92f, 47.87f, 1.0f, -35.45f, 38.22f, -33.96f, 37.84f, -33.39f, 40.06f, -34.84f,
  40.47f, 2.56f, 5.45f, -34.99f, 3.6f, 39.38f, 1.1f, -40.2f, 53.86f, -31.11f, 51.78f, -30.97f, 58.22f, -39.4f, 59.48f, 3.04f, 9.51f, -36.32f, 5.04f,
  55.97f, 3.34f, -53.14f, 97.59f, -50.87f, 101.14f, -53.4f, 102.75f, -55.65f, 99.2f, 7.75f, 11.47f, -52.0f, 9.25f, 99.36f, 1.6f, -40.13f, 38.61f,
  -41.11f, 36.89f, -39.64f, 36.07f, -38.66f, 37.8f, 3.95f, 7.15f, -39.37f, 5.5f, 38.19f, 1.6f
};
static const uint32_t data_22FC8 = 380;
static const int32_t data_22FCC[6] = {1, 390, 0, 1018, 0, -1};
static const uint32_t data_22FE4 = 510;
static const int32_t data_22FE8[6] = {1, 525, 0, 1020, 0, -1};
static const uint32_t data_23000 = 150;
static const int32_t data_23004[6] = {1, 160, 0, 1019, 0, -1};
static const uint32_t data_2301C = 530;
static const int32_t data_23020[6] = {1, 970, 0, 1014, 0, -1};
static const uint32_t data_23038 = 911;
static const int32_t data_2303C[6] = {1, 155, 0, 1015, 0, -1};
static const char data_23054[7] __attribute__((scn_string_literal)) = "FREE02";
static const char data_2305B[7] __attribute__((scn_string_literal)) = "FREE03";
static const char data_23062[7] __attribute__((scn_string_literal)) = "FREE04";
static const char data_23069[7] __attribute__((scn_string_literal)) = "FREE01";
static const char data_23070[11] __attribute__((scn_string_literal)) = "M_MOBJ.BIN";
static const char data_2307B[14] __attribute__((scn_string_literal)) = "scene/03/D000";
static const char data_23089[] __attribute__((scn_string_literal)) = "SCRTEST.BIN";
static const uint8_t data_23095[3] = {0};

__attribute__((scn_string_bindings(0, data_1DBB8)))
static int32_t script_0030(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
  float local_F4[3];
  int32_t local_E8[3];
  int32_t local_E4;
  int32_t local_E0;
  int32_t local_DC;
  float local_D8;
  float local_D4;
  int32_t local_D0;
  int8_t local_CF;
  int8_t local_CE;
  int8_t local_CD;
  int8_t local_CC;
  int8_t local_CB;
  int8_t local_CA;
  script_0932(arg0, arg1, arg2, arg3, arg4, arg5);
  local_E8[0] = local_E8[2] = 0;
  while (1) {
    while (*(&task_C + arg5) & 4) {
      SCNF_WaitFrames_0000(1);
    }
    arg2 = Script_KaidanUtils(CAM_STAIR_QUERY_GUIDE_ENTRY, arg0, task_0, (const void *)task_4, (const void *)task_8, task_20);
    if (arg2 >= 0) {
      Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_2);
      Script_ccowUtils(arg0, 9);
      if (*(&task_C + arg5) & 0x800) {
        Script_scenUtils(SCENARIO_CMD_INIT_SCNT);
      }
      (void)((local_CA = 0) & 1);
      local_E4 = arg2 * 6;
      *(int32_t *)&local_F4[1] = Script_ScenDataUtil((void *)task_4, 4, local_E4 + 1);
      *(int32_t *)&local_F4[0] = Script_ScenDataUtil((void *)task_4, 4, local_E4);
      *(int32_t *)&local_F4[2] = Script_ScenDataUtil((void *)task_4, 4, local_E4 + 2);
      local_DC = Script_ScenDataUtil((void *)task_4, 4, local_E4 + 4);
      local_E8[1] = Script_ScenDataUtil((void *)task_4, 4, local_E4 + 3);
      local_E0 = Script_ScenDataUtil((void *)task_4, 4, local_E4 + 5);
      local_D8 = Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, local_E8[1]);
      local_D4 = Script_MathUtils(SCRIPT_MATH_COS_FIXED16, local_E8[1]);
      script_20A3(arg0, arg5, arg2);
      *(&task_18 + arg5) = arg2 + 1;
      *((int8_t *)&*(int32_t *)&task_44 + arg5) = 3;
      script_2D34(arg0, arg5, local_DC);
      Script_CharHandlers(arg0, 2, 0, 16);
      (void)((local_CD = -((local_CB = 0) != 0)) & 1);
      if (local_DC & 0xF00 & 0x400) {
        script_0A25(1, arg2);
        if (local_DC & 0xF000 & 0x1000) {
          (void)((task_4C = -1) & 1);
          while (task_4C & 2) {
            SCNF_WaitFrames_0000(1);
          }
        }
      }
      if (local_DC & 0x100) {
        (void)((local_CA = -1) & 1);
      }
      if (*(&task_C + arg5) & 0x400) {
        Script_MotStepUtil(arg0, 0);
      }
      if ((local_DC & 0xF) <= 4) {
        Script_ccowSetFlag80(arg0, 1);
        if ((*(&task_C + arg5) & 8) == 0) {
          if (local_DC & 0x1000000) {
            Script_scenCreateTask(script_24C5, 2, arg0, 1);
          } else {
            Script_scenCreateTask(script_24C5, 2, arg0, 1);
          }
        }
        if (local_DC & 0x2000) {
          task_49 = 0;
        } else {
          uint8_t unused_C8[2];
          int32_t local_C4;
          local_C4 = *(&task_10 + arg5);
          switch (local_C4) {
            case 0:
              task_49 = 0;
              break;
            case 1:
              task_49 = 1;
              break;
            case 3:
              task_49 = 1;
              break;
            case 2:
              if (Script_ryoControl(PCCTL_QueryIsRunningRequest)) {
                task_49 = 1;
              } else {
                task_49 = 0;
              }
              break;
          }
        }
        if (task_49) {
          local_F4[0] = *local_F4 + local_D8 * -0.1f;
          local_F4[2] = local_F4[2] + local_D4 * -0.1f;
        } else {
          local_F4[0] = *local_F4 + local_D8 * 0.16f;
          local_F4[2] = local_F4[2] + local_D4 * 0.16f;
        }
        local_CF = script_1B07(arg0, local_E8[1], *local_F4, local_F4[2], arg5, local_DC, local_E0);
        if (local_CA & 4) {
          Script_KaidanUtils(CAM_STAIR_APPLY_GUIDE_FACING, arg0, arg2, (const void *)task_4);
        }
        (void)((local_CC = -1) & 1);
        Script_ccowSetFlag80(arg0, 2);
        Script_ccowUnknown(arg0, 1, 3.0f);
      } else {
        if ((local_DC & 0xF) <= 12) {
          if ((*(&task_C + arg5) & 8) == 0) {
            Script_scenCreateTask(script_24C5, 2, arg0, 0);
          }
          if (local_DC & 0x2000) {
            task_49 = 0;
          } else {
            uint8_t unused_C8[2];
            int32_t local_C4;
            local_C4 = *(&task_10 + arg5);
            switch (local_C4) {
              case 0:
                task_49 = 0;
                break;
              case 1:
                task_49 = 1;
                break;
              case 3:
                task_49 = 1;
                break;
              case 2:
                if (Script_ryoControl(PCCTL_QueryIsRunningRequest)) {
                  task_49 = 1;
                } else {
                  task_49 = 0;
                }
                break;
            }
          }
          if (task_49) {
            local_F4[0] = *local_F4 + local_D8 * -0.25f;
            local_F4[2] = local_F4[2] + local_D4 * -0.25f;
          } else {
            local_F4[0] = *local_F4 + local_D8 * 0.15f;
            local_F4[2] = local_F4[2] + local_D4 * 0.15f;
          }
          local_CF = local_E0 - script_1D96(arg0, local_E8[1], *local_F4, local_F4[2], arg5, local_DC, local_E0);
          if (local_CA & 4) {
            Script_KaidanUtils(CAM_STAIR_APPLY_GUIDE_FACING, arg0, arg2, (const void *)task_4);
          }
          Script_ccowSetFlag80(arg0, 2);
          Script_ccowUnknown(arg0, 1, 3.0f);
          (void)((local_CC = 0) & 0x80);
          goto L000525;
        }
      }
      L000525:;
      if ((local_E0 == 1) & ((local_DC & 0xF) <= 4)) {
        if (*((int8_t *)&*(int32_t *)&task_47 + arg5) == 1) {
          Script_motmToggleFlags(arg0, 1, 0x80);
        }
        if (task_49) {
          Script_PlayCharaMotion(arg0, MN_AKI_AKI_RUN_MID_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
          SCNF_WaitFrames_0000(1);
          for (;;) {
            if (Script_GetCharaMotionRemainingFramesScaled(arg0) >= 10.0f) {
              break;
            }
            SCNF_WaitFrames_0000(1);
          }
        } else {
          Script_PlayCharaMotion(arg0, MN_AKI_AKI_WALK_SP2_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
          SCNF_WaitFrames_0000(1);
          for (;;) {
            if (Script_GetCharaMotionRemainingFramesScaled(arg0) >= 20.0f) {
              break;
            }
            SCNF_WaitFrames_0000(1);
          }
        }
      } else {
        local_D0 = 1;
        if (local_CC & 0x10) {
          Script_scenCreateTask(script_2ADB, 2, arg0, arg5);
        } else {
          Script_scenCreateTask(script_2C03, 2, arg0, arg5);
        }
        while ((local_CF >= 0) & (local_CF <= local_E0)) {
          local_CE = task_49;
          if (local_D0) {
            local_D0 = 0;
          } else if (local_DC & 0x2000) {
            task_49 = 0;
          } else {
            uint8_t unused_C8[2];
            int32_t local_C4;
            local_C4 = *(&task_10 + arg5);
            switch (local_C4) {
              case 0:
                task_49 = 0;
                break;
              case 1:
                task_49 = 1;
                break;
              case 3:
                task_49 = 1;
                break;
              case 2:
                if (Script_ryoControl(PCCTL_QueryIsRunningRequest)) {
                  task_49 = 1;
                } else {
                  task_49 = 0;
                }
                break;
            }
          }
          if (local_CB & 8) {
          } else {
            if (*(&task_C + arg5) & 0x100) {
              if (*(&task_C + arg5) & 2) {
                if ((Script_GetControllerInput(0) & 0x2000) > 0) {
                  (void)((local_CD = -1) & 4);
                }
              }
            }
            if (local_CC & 0x10) {
              local_CF = local_CF + script_0E0C(arg0, local_CF, local_E0, arg5, local_DC, arg2, local_CE - task_49);
            } else {
              local_CF = local_CF - script_12FC(arg0, local_CF, arg5, local_DC, arg2, local_CE - task_49);
            }
            if ((local_CF > 0) & (local_CF < local_E0)) {
              if (!(local_CD & 0x20)) goto L000767;
            }
          }
          L000767:;
        }
        if (local_D0) {
          if (((*(&task_C + arg5) & 0x10) == 0) & ((local_DC & 0xF) < 2)) {
            if (*((int8_t *)&*(int32_t *)&task_47 + arg5) == 1) {
              Script_motmToggleFlags(arg0, 1, 0x80);
            }
            if (task_49) {
              Script_PlayCharaMotion(arg0, MN_AKI_AKI_RUN_MID_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
              SCNF_WaitFrames_0000(1);
              for (;;) {
                if (Script_GetCharaMotionRemainingFramesScaled(arg0) >= 10.0f) {
                  break;
                }
                SCNF_WaitFrames_0000(1);
              }
            } else {
              Script_PlayCharaMotion(arg0, MN_AKI_AKI_WALK_SP2_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
              SCNF_WaitFrames_0000(1);
              for (;;) {
                if (Script_GetCharaMotionRemainingFramesScaled(arg0) >= 20.0f) {
                  break;
                }
                SCNF_WaitFrames_0000(1);
              }
            }
          }
        }
      }
      if ((task_4A == 1) | (task_4A == 2)) {
        task_4A = 3;
      }
      if ((*(&task_C + arg5) & 2) > 0) {
        if (task_49) {
          Script_SetCharaMotionPlaybackRate(arg0, 1.2f);
        } else {
          Script_SetCharaMotionPlaybackRate(arg0, 1.3f);
        }
        if (task_50 & 4) {
          Script_sprintf("Kaidan : ");
          Script_ryoControl(RYO_RestoreControl);
          (void)((task_50 = 0) & 4);
        }
        script_2E09(arg0, arg5, local_DC);
      }
      Script_ccowUnknown(arg0, 2, 0.07f);
      Script_ccowUtils(arg0, 10);
      if (*((int8_t *)&*(int32_t *)&task_45 + arg5) != 2) {
        *((int8_t *)&*(int32_t *)&task_45 + arg5) = 0;
      }
      *((int8_t *)&*(int32_t *)&task_46 + arg5) = 0;
      *((int8_t *)&*(int32_t *)&task_44 + arg5) = 0;
      *(&task_18 + arg5) = -1;
      if (*(&task_C + arg5) & 0x800) {
        Script_scenUtils(SCENARIO_CMD_UPDATE_TIMER);
      }
    } else {
      (void)((task_50 = 0) & 1);
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_0932(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
  int32_t local_FC;
  int32_t local_F8;
  *(&task_C + arg5) = arg4;
  *(&task_14 + arg5) = arg0;
  *(&task_18 + arg5) = -1;
  *(&task_10 + arg5) = arg1;
  *((int8_t *)&*(int32_t *)&task_45 + arg5) = 0;
  *((int8_t *)&*(int32_t *)&task_46 + arg5) = 0;
  Script_sprintf("========== initializing kaidan\012");
  if (arg3) {
    (void)((task_4B = 0) & 1);
    task_20 = 0x4000;
    task_48 = 1;
    task_4 = arg3;
    (void)((task_50 = 0) & 1);
    if (arg2) {
      Script_sprintf("please ENDOFKDATA use can't start shuki kaidan\012");
      task_0 = arg2;
    } else {
      Script_CycleManagerUtils(CYCLE_CMD_SET_GLOBAL_CYCLE_PTR, task_4, &task_18);
      task_0 = local_F8 = 0;
      while ((local_FC = Script_ScenDataUtil((void *)task_4, 4, local_F8)) != 0x47C35000) {
        task_0 = task_0 + 1;
        local_F8 = local_F8 + 6;
      }
    }
  } else {
    task_48 = task_48 + 1;
  }
  return;
}

static void script_0A25(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  int32_t local_F0;
  local_F4 = 0;
  local_FC = 0;
  while ((local_F8 = Script_ScenDataUtil((void *)task_8, 4, local_FC)) != 3) {
    local_F0 = local_F8;
    if (local_F0 == 0) {
      local_FC = local_FC + 3;
    } else if (local_F0 == 1) {
      if (arg0) {
        if (Script_ScenDataUtil((void *)task_8, 4, local_FC + 1) == arg1) {
          local_F4 = Script_ScenDataUtil((void *)task_8, 4, local_FC + 2);
          if (local_F4 < 4) {
            local_F4 = task_24[local_F4];
            break;
          } else {
            Script_PreserveAccumulator_012C(0, "Kaidan Function Error 1\012");
          }
        }
      }
      local_FC = local_FC + 3;
    } else if (local_F0 == 2) {
      if (arg0 == 0) {
        if (Script_ScenDataUtil((void *)task_8, 4, local_FC + 1) == arg1) {
          local_F4 = Script_ScenDataUtil((void *)task_8, 4, local_FC + 2);
          if (local_F4 < 4) {
            local_F4 = task_24[local_F4];
            break;
          } else {
            Script_PreserveAccumulator_012C(0, "Kaidan Function Error 2\012");
          }
        }
      }
      local_FC = local_FC + 3;
    } else {
      Script_PreserveAccumulator_012C(0, "Kaidan Data Error\012");
    }
  }
  if (local_F4) {
    Script_scenCreateTask(local_F4, 0);
  }
  return;
}

static void script_0B64(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int8_t local_FF;
  int32_t local_F8;
  if (arg2 & 0x2000) {
    task_49 = 0;
  } else {
    local_F8 = *(&task_10 + arg1);
    switch (local_F8) {
      case 0:
        task_49 = 0;
        break;
      case 1:
        task_49 = 1;
        break;
      case 3:
        task_49 = 1;
        break;
      case 2:
        if (Script_ryoControl(PCCTL_QueryIsRunningRequest)) {
          task_49 = 1;
        } else {
          task_49 = 0;
        }
        break;
    }
  }
  if (task_49) {
    if (arg3 > 1) {
      Script_PlayCharaMotion(arg0, MN_AKI_AKI_NOBORU_KAIDAN_2STEP_SONOBA_LP, -1.0f, -1.0f, 9.0f, 1.0f, 0);
      local_FF = 2;
      if (arg2 & 0x100000) {
        script_2739(arg0);
      } else {
        SCNF_WaitFrames_0000(1);
        while (Script_IsCharaMotionFinished(arg0) == 0) {
          SCNF_WaitFrames_0000(1);
        }
      }
    } else {
      Script_PlayCharaMotion(arg0, MN_AKI_AKI_NOBORU_KAIDAN_2STEP_SONOBA_LP, 0.0f, 8.0f, 1.0f, 1.0f, 0);
      local_FF = arg3;
      SCNF_WaitFrames_0000(1);
      while (Script_IsCharaMotionFinished(arg0) == 0) {
        SCNF_WaitFrames_0000(1);
      }
    }
  } else {
    if (arg3 > 1) {
      Script_PlayCharaMotion(arg0, MN_AKI_AKI_NOBORU_KAIDAN_SONOBA_LP, 0.0f, 29.0f, 5.0f, 1.0f, 0);
      local_FF = 1;
    } else {
      Script_PlayCharaMotion(arg0, MN_AKI_AKI_NOBORU_KAIDAN_SONOBA_LP, 0.0f, 18.0f, 5.0f, 1.0f, 0);
      local_FF = arg3;
    }
    SCNF_WaitFrames_0000(1);
    while (Script_IsCharaMotionFinished(arg0) == 0) {
      SCNF_WaitFrames_0000(1);
    }
  }
  (void)local_FF;
  return;
}

static void script_0D13(int32_t arg0, int32_t arg1, int32_t arg2) {
  int8_t local_FF;
  int32_t local_F8;
  if (arg2 & 0x2000) {
    task_49 = 0;
  } else {
    local_F8 = *(&task_10 + arg1);
    switch (local_F8) {
      case 0:
        task_49 = 0;
        break;
      case 1:
        task_49 = 1;
        break;
      case 3:
        task_49 = 1;
        break;
      case 2:
        if (Script_ryoControl(PCCTL_QueryIsRunningRequest)) {
          task_49 = 1;
        } else {
          task_49 = 0;
        }
        break;
    }
  }
  if (task_49) {
    local_FF = 0;
    Script_PlayCharaMotion(arg0, MN_AKI_AKI_ORIRU_KAIDAN_2STEP_SONOBA_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  } else {
    Script_PlayCharaMotion(arg0, MN_AKI_AKI_ORIRU_KAIDAN_SONOBA_LP, 0.0f, 29.0f, 18.0f, 1.0f, 0);
    local_FF = 0;
    SCNF_WaitFrames_0000(1);
    while (Script_IsCharaMotionFinished(arg0) == 0) {
      SCNF_WaitFrames_0000(1);
    }
  }
  (void)local_FF;
  return;
}

static int8_t script_0E0C(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5, int32_t arg6) {
  int8_t local_FF;
  uint8_t unused_F0[15];
  task_34 = 0;
  if (task_49) {
    local_FF = 4;
    if (arg1 < arg2 - 3) {
      Script_motmToggleFlags(arg0, 2, 0x40);
      if (arg6) {
        if (*((int8_t *)&*(int32_t *)&task_47 + arg3) == 1) {
          Script_motmToggleFlags(arg0, 1, 0x80);
        }
        Script_PlayCharaMotion(arg0, MN_AKI_AKI_NOBORU_KAIDAN_2STEP_SONOBA_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
      }
      if (arg4 & 0x100000) {
        script_2814(arg0);
      } else {
        SCNF_WaitFrames_0000(1);
        while (Script_IsCharaMotionFinished(arg0) == 0) {
          SCNF_WaitFrames_0000(1);
        }
      }
      Script_motmToggleFlags(arg0, 1, 0x40);
    } else {
      if ((*(&task_C + arg3) & 8) == 0) {
        task_4A = 2;
      }
      script_2DB9(arg0, arg3, arg4);
      if (arg4 & 0xF00 & 0x800) {
        script_0A25(0, arg5);
        if (arg4 & 0xF000 & 0x1000) {
          (void)((task_4C = -1) & 1);
          while (task_4C & 2) {
            SCNF_WaitFrames_0000(1);
          }
        }
      } else if (arg2 <= arg1 + 2) {
        if (*((int8_t *)&*(int32_t *)&task_47 + arg3) == 0) {
          Script_motmToggleFlags(arg0, 1, 0x80);
        }
        Script_PlayCharaMotion(arg0, MN_AKI_AKI_RUN_MID_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        SCNF_WaitFrames_0000(1);
        for (;;) {
          if (Script_GetCharaMotionRemainingFramesScaled(arg0) >= 8.0f) {
            break;
          }
          SCNF_WaitFrames_0000(1);
        }
      } else {
        if (arg6) {
          if (*((int8_t *)&*(int32_t *)&task_47 + arg3) == 1) {
            Script_motmToggleFlags(arg0, 1, 0x80);
          }
          Script_PlayCharaMotion(arg0, MN_AKI_AKI_NOBORU_KAIDAN_2STEP_SONOBA_LP, -1.0f, 5.0f, -1.0f, 1.0f, 0);
          SCNF_WaitFrames_0000(1);
          while (Script_IsCharaMotionFinished(arg0) == 0) {
            SCNF_WaitFrames_0000(1);
          }
        } else {
          SCNF_WaitFrames_0000(1);
          for (;;) {
            if (Script_GetCharaMotionRemainingFramesScaled(arg0) >= 5.0f) {
              break;
            }
            SCNF_WaitFrames_0000(1);
          }
        }
        if (*((int8_t *)&*(int32_t *)&task_47 + arg3) == 0) {
          Script_motmToggleFlags(arg0, 1, 0x80);
        }
        Script_PlayCharaMotion(arg0, MN_AKI_AKI_RUN_MID_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        SCNF_WaitFrames_0000(1);
        for (;;) {
          if (Script_GetCharaMotionRemainingFramesScaled(arg0) >= 8.0f) {
            break;
          }
          SCNF_WaitFrames_0000(1);
        }
      }
      if ((*(&task_C + arg3) & 8) == 0) {
        task_4A = 3;
      }
    }
  } else {
    local_FF = 2;
    if (arg1 < arg2 - 1) {
      if (arg6) {
        if (*((int8_t *)&*(int32_t *)&task_47 + arg3) == 1) {
          Script_motmToggleFlags(arg0, 1, 0x80);
        }
        Script_PlayCharaMotion(arg0, MN_AKI_AKI_NOBORU_KAIDAN_SONOBA_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
      }
      if (arg4 & 0x100000) {
        script_265E(arg0);
      } else {
        SCNF_WaitFrames_0000(1);
        while (Script_IsCharaMotionFinished(arg0) == 0) {
          SCNF_WaitFrames_0000(1);
        }
      }
    } else {
      Script_ccowSetFlag80(arg0, 1);
      if ((*(&task_C + arg3) & 8) == 0) {
        task_4A = 2;
      }
      script_2DB9(arg0, arg3, arg4);
      if (arg6) {
        if (*((int8_t *)&*(int32_t *)&task_47 + arg3) == 1) {
          Script_motmToggleFlags(arg0, 1, 0x80);
        }
        Script_PlayCharaMotion(arg0, MN_AKI_AKI_NOBORU_KAIDAN_SONOBA_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
      }
      if (arg2 != arg1) {
        SCNF_WaitFrames_0000(1);
        for (;;) {
          if (Script_GetCharaMotionRemainingFramesScaled(arg0) >= 13.0f) {
            break;
          }
          SCNF_WaitFrames_0000(1);
        }
      } else {
        SCNF_WaitFrames_0000(1);
        for (;;) {
          if (Script_GetCharaMotionRemainingFramesScaled(arg0) >= 3.0f) {
            break;
          }
          SCNF_WaitFrames_0000(1);
        }
      }
      Script_ccowSetFlag80(arg0, 2);
      if (arg4 & 0xF00 & 0x800) {
        script_0A25(0, arg5);
        if (arg4 & 0xF000 & 0x1000) {
          (void)((task_4C = -1) & 2);
          while (task_4C & 2) {
            SCNF_WaitFrames_0000(1);
          }
        }
      } else {
        if (arg2 == arg1) {
          if (*((int8_t *)&*(int32_t *)&task_47 + arg3) == 0) {
            Script_motmToggleFlags(arg0, 1, 0x80);
          }
        } else {
          if (*((int8_t *)&*(int32_t *)&task_47 + arg3) == 1) {
            Script_motmToggleFlags(arg0, 1, 0x80);
          }
        }
        Script_PlayCharaMotion(arg0, MN_AKI_AKI_WALK_SP2_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        SCNF_WaitFrames_0000(1);
        for (;;) {
          if (Script_GetCharaMotionRemainingFramesScaled(arg0) >= 25.0f) {
            break;
          }
          SCNF_WaitFrames_0000(1);
        }
      }
      if ((*(&task_C + arg3) & 8) == 0) {
        task_4A = 3;
      }
    }
  }
  return local_FF;
}

static int8_t script_12FC(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
  int8_t local_FF;
  if (task_49) {
    local_FF = 4;
    if (arg1 >= local_FF) {
      if (arg5) {
        if (*((int8_t *)&*(int32_t *)&task_47 + arg2) == 1) {
          Script_motmToggleFlags(arg0, 1, 0x80);
        }
        Script_PlayCharaMotion(arg0, MN_AKI_AKI_ORIRU_KAIDAN_2STEP_SONOBA_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
      }
      if (arg3 & 0x100000) {
        script_29E5(arg0, arg2);
      } else {
        SCNF_WaitFrames_0000(1);
        while (Script_IsCharaMotionFinished(arg0) == 0) {
          SCNF_WaitFrames_0000(1);
        }
      }
    } else {
      if (*((int8_t *)&*(int32_t *)&task_45 + arg2) != 2) {
        *((int8_t *)&*(int32_t *)&task_45 + arg2) = 0;
      }
      switch (arg1) {
        case 1:
          if (arg5) {
            if (*((int8_t *)&*(int32_t *)&task_47 + arg2) == 1) {
              Script_motmToggleFlags(arg0, 1, 0x80);
            }
            Script_PlayCharaMotion(arg0, MN_AKI_AKI_ORIRU_KAIDAN_2STEP_SONOBA_LP, 0.0f, 5.0f, 0.0f, 1.0f, 0);
            SCNF_WaitFrames_0000(1);
            while (Script_IsCharaMotionFinished(arg0) == 0) {
              SCNF_WaitFrames_0000(1);
            }
          } else {
            SCNF_WaitFrames_0000(1);
            for (;;) {
              if (Script_GetCharaMotionRemainingFramesScaled(arg0) >= 5.0f) {
                break;
              }
              SCNF_WaitFrames_0000(1);
            }
          }
          break;
        case 2:
          if (arg5) {
            if (*((int8_t *)&*(int32_t *)&task_47 + arg2) == 1) {
              Script_motmToggleFlags(arg0, 1, 0x80);
            }
            Script_PlayCharaMotion(arg0, MN_AKI_AKI_ORIRU_KAIDAN_2STEP_SONOBA_LP, 0.0f, 7.0f, 0.0f, 1.0f, 0);
            SCNF_WaitFrames_0000(1);
            while (Script_IsCharaMotionFinished(arg0) == 0) {
              SCNF_WaitFrames_0000(1);
            }
          } else {
            SCNF_WaitFrames_0000(1);
            for (;;) {
              if (Script_GetCharaMotionRemainingFramesScaled(arg0) >= 7.0f) {
                break;
              }
              SCNF_WaitFrames_0000(1);
            }
          }
          break;
        case 3:
          if (arg5) {
            if (*((int8_t *)&*(int32_t *)&task_47 + arg2) == 1) {
              Script_motmToggleFlags(arg0, 1, 0x80);
            }
            Script_PlayCharaMotion(arg0, MN_AKI_AKI_ORIRU_KAIDAN_2STEP_SONOBA_LP, 0.0f, 9.0f, 0.0f, 1.0f, 0);
            SCNF_WaitFrames_0000(1);
            while (Script_IsCharaMotionFinished(arg0) == 0) {
              SCNF_WaitFrames_0000(1);
            }
          } else {
            SCNF_WaitFrames_0000(1);
            for (;;) {
              if (Script_GetCharaMotionRemainingFramesScaled(arg0) >= 9.0f) {
                break;
              }
              SCNF_WaitFrames_0000(1);
            }
          }
          break;
      }
      if ((*(&task_C + arg2) & 8) == 0) {
        task_4A = 2;
      }
      script_2DB9(arg0, arg2, arg3);
      if (arg3 & 0xF00 & 0x800) {
        script_0A25(0, arg4);
        if (arg3 & 0xF000 & 0x1000) {
          (void)((task_4C = -1) & 1);
          while (task_4C & 2) {
            SCNF_WaitFrames_0000(1);
          }
        }
      } else {
        if (arg1 <= 2) {
          if (*((int8_t *)&*(int32_t *)&task_47 + arg2) == 1) {
            Script_motmToggleFlags(arg0, 1, 0x80);
          }
        } else {
          if (*((int8_t *)&*(int32_t *)&task_47 + arg2) == 0) {
            Script_motmToggleFlags(arg0, 1, 0x80);
          }
        }
        Script_PlayCharaMotion(arg0, MN_AKI_AKI_RUN_MID_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        SCNF_WaitFrames_0000(1);
        for (;;) {
          if (Script_GetCharaMotionRemainingFramesScaled(arg0) >= 8.0f) {
            break;
          }
          SCNF_WaitFrames_0000(1);
        }
      }
      if ((*(&task_C + arg2) & 8) == 0) {
        task_4A = 3;
      }
    }
  } else {
    local_FF = 2;
    if (arg1 >= local_FF) {
      if (arg5) {
        if (*((int8_t *)&*(int32_t *)&task_47 + arg2) == 1) {
          Script_motmToggleFlags(arg0, 1, 0x80);
        }
        Script_PlayCharaMotion(arg0, MN_AKI_AKI_ORIRU_KAIDAN_SONOBA_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
      }
      if (arg3 & 0x100000) {
        script_28EF(arg0, arg2);
      } else {
        SCNF_WaitFrames_0000(1);
        while (Script_IsCharaMotionFinished(arg0) == 0) {
          SCNF_WaitFrames_0000(1);
        }
      }
    } else {
      if (*((int8_t *)&*(int32_t *)&task_45 + arg2) != 2) {
        *((int8_t *)&*(int32_t *)&task_45 + arg2) = 0;
      }
      if (arg1) {
        if (arg5) {
          if (*((int8_t *)&*(int32_t *)&task_47 + arg2) == 1) {
            Script_motmToggleFlags(arg0, 1, 0x80);
          }
          Script_PlayCharaMotion(arg0, MN_AKI_AKI_ORIRU_KAIDAN_SONOBA_LP, 0.0f, 15.0f, 0.0f, 1.0f, 0);
          SCNF_WaitFrames_0000(1);
          while (Script_IsCharaMotionFinished(arg0) == 0) {
            SCNF_WaitFrames_0000(1);
          }
        } else {
          SCNF_WaitFrames_0000(1);
          for (;;) {
            if (Script_GetCharaMotionRemainingFramesScaled(arg0) >= 15.0f) {
              break;
            }
            SCNF_WaitFrames_0000(1);
          }
        }
      }
      if ((*(&task_C + arg2) & 8) == 0) {
        task_4A = 2;
      }
      if (arg3 & 0xF00 & 0x800) {
        script_0A25(0, arg4);
        if (arg3 & 0xF000 & 0x1000) {
          (void)((task_4C = -1) & 1);
          while (task_4C & 2) {
            SCNF_WaitFrames_0000(1);
          }
        }
        script_2DB9(arg0, arg2, arg3);
      } else {
        if (arg1) {
          if (*((int8_t *)&*(int32_t *)&task_47 + arg2) == 1) {
            Script_motmToggleFlags(arg0, 1, 0x80);
          }
        } else {
          if (*((int8_t *)&*(int32_t *)&task_47 + arg2) == 0) {
            Script_motmToggleFlags(arg0, 1, 0x80);
          }
        }
        Script_PlayCharaMotion(arg0, MN_AKI_AKI_WALK_SP2_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        SCNF_WaitFrames_0000(1);
        for (;;) {
          if (Script_GetCharaMotionRemainingFramesScaled(arg0) >= 10.0f) {
            break;
          }
          SCNF_WaitFrames_0000(1);
        }
        script_2DB9(arg0, arg2, arg3);
        SCNF_WaitFrames_0000(1);
        for (;;) {
          if (Script_GetCharaMotionRemainingFramesScaled(arg0) >= 20.0f) {
            break;
          }
          SCNF_WaitFrames_0000(1);
        }
      }
      if ((*(&task_C + arg2) & 8) == 0) {
        task_4A = 3;
      }
    }
  }
  return local_FF;
}

static void script_18C8(int32_t arg0, int32_t arg1, float arg2, float arg3, int32_t arg4, int32_t arg5) {
  Vector3 local_F4;
  float local_E8[3];
  if (arg5 & 0x2000) {
    task_49 = 0;
  } else {
    int32_t unused_D4[5];
    int32_t local_D0;
    local_D0 = *(&task_10 + arg4);
    switch (local_D0) {
      case 0:
        task_49 = 0;
        break;
      case 1:
        task_49 = 1;
        break;
      case 3:
        task_49 = 1;
        break;
      case 2:
        if (Script_ryoControl(PCCTL_QueryIsRunningRequest)) {
          task_49 = 1;
        } else {
          task_49 = 0;
        }
        break;
    }
  }
  Script_EVT_GetPosition(arg0, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  Script_charaToggleMotEnabled(arg0, 0);
  if (arg5 & 0x100) {
    int32_t unused_E4;
    float local_E0;
    float local_DC;
    float local_D8;
    local_E0 = Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, arg1);
    local_DC = Script_MathUtils(SCRIPT_MATH_COS_FIXED16, arg1);
    local_D8 = (&local_F4)->x - arg2;
    {
      float local_D4;
      local_D4 = (&local_F4)->z - arg3;
      local_D8 = local_D8 * local_DC - local_D4 * local_E0;
    }
    {
      float local_D4;
      local_D4 = 0.0f;
      arg1 = 0x10000 - arg1;
      local_E0 = Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, arg1);
      local_DC = Script_MathUtils(SCRIPT_MATH_COS_FIXED16, arg1);
      local_E8[0] = local_D8 * local_DC - local_D4 * local_E0;
      local_E8[2] = local_D8 * local_E0 + local_D4 * local_DC;
    }
    arg2 = *local_E8 + arg2;
    arg3 = local_E8[2] + arg3;
  }
  if (task_49) {
    float local_E4;
    local_E4 = 3.0f;
    arg2 = arg2 - (&local_F4)->x;
    arg2 = arg2 / local_E4;
    arg3 = arg3 - (&local_F4)->z;
    arg3 = arg3 / local_E4;
    Script_PlayCharaMotion(arg0, MN_AKI_AKI_RUN_MID_LP, 0.0f, 10.0f, 7.0f, 1.0f, 0);
  } else {
    float local_E4;
    local_E4 = 6.0f;
    arg2 = arg2 - (&local_F4)->x;
    arg2 = arg2 / local_E4;
    arg3 = arg3 - (&local_F4)->z;
    arg3 = arg3 / local_E4;
    Script_PlayCharaMotion(arg0, MN_AKI_AKI_WALK_SP2_LP, 0.0f, 8.0f, 2.0f, 1.0f, 0);
  }
  SCNF_WaitFrames_0000(1);
  while (Script_IsCharaMotionFinished(arg0) == 0) {
    local_F4.x = (&local_F4)->x + arg2;
    (&local_F4)->z = (&local_F4)->z + arg3;
    Script_EVT_SetPosition(arg0, 0x78000000, &local_F4);
    SCNF_WaitFrames_0000(1);
  }
  Script_charaToggleMotEnabled(arg0, 1);
  return;
}

static int32_t script_1B07(int32_t arg0, int32_t arg1, float arg2, float arg3, int32_t arg4, int32_t arg5, int32_t arg6) {
  Vector3 local_F4;
  float local_E8[3];
  Vector3 local_DC;
  Vector3 local_D0;
  float local_CC;
  float local_C8;
  int32_t local_C4;
  float local_C0;
  float local_BC;
  int32_t local_B8;
  task_34 = 0;
  Script_EVT_GetPosition(arg0, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  if (arg5 & 0x100) {
    local_CC = Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, arg1);
    local_C8 = Script_MathUtils(SCRIPT_MATH_COS_FIXED16, arg1);
    local_C0 = (&local_F4)->x - arg2;
    local_BC = (&local_F4)->z - arg3;
    local_C0 = local_C0 * local_C8 - local_BC * local_CC;
    local_BC = 0.0f;
    local_C4 = 0x10000 - arg1;
    local_CC = Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, local_C4);
    local_C8 = Script_MathUtils(SCRIPT_MATH_COS_FIXED16, local_C4);
    local_E8[0] = local_C0 * local_C8 - local_BC * local_CC;
    local_E8[2] = local_C0 * local_CC + local_BC * local_C8;
    arg2 = *local_E8 + arg2;
    arg3 = local_E8[2] + arg3;
  }
  if (task_49) {
    Script_motmToggleFlags(arg0, 2, 0x40);
    if (Script_motmUtilsEx(13, arg0, 0) != 0) {
      SCNF_WaitFrames_0000(1);
    }
    Script_motmToggleFlags(arg0, 1, 0x40);
    local_F4.x = arg2;
    (&local_F4)->z = arg3;
    Script_EVT_GetPosition(arg0, 26, &local_DC, ENTITY_XFORM_WORLD_SPACE);
    Script_EVT_GetPosition(arg0, 33, &local_D0, ENTITY_XFORM_WORLD_SPACE);
    if (!((&local_DC)->y >= (&local_D0)->y)) {
      task_34 = -0x80000000;
      *((int8_t *)&*(int32_t *)&task_47 + arg4) = 1;
    } else {
      *((int8_t *)&*(int32_t *)&task_47 + arg4) = 0;
    }
    Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
    if (arg6 > 1) {
      Script_SetCharaMoveTargetEx(arg0, &local_F4, arg1, task_34 | 0x2CB, 0, -1.0f, 2);
      local_B8 = 2;
    } else {
      Script_SetCharaMoveTargetEx(arg0, &local_F4, arg1, task_34 | 0x2CB, 0, -1.0f, 2);
      local_B8 = arg6;
    }
  } else {
    local_F4.x = arg2;
    (&local_F4)->z = arg3;
    Script_EVT_GetPosition(arg0, 26, &local_DC, ENTITY_XFORM_WORLD_SPACE);
    Script_EVT_GetPosition(arg0, 33, &local_D0, ENTITY_XFORM_WORLD_SPACE);
    if (!((&local_DC)->y >= (&local_D0)->y)) {
      task_34 = -0x80000000;
      *((int8_t *)&*(int32_t *)&task_47 + arg4) = 1;
    } else {
      *((int8_t *)&*(int32_t *)&task_47 + arg4) = 0;
    }
    Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
    if (arg6 > 1) {
      Script_SetCharaMoveTargetEx(arg0, &local_F4, arg1, task_34 | 0x2C6, 0, -1.0f, 2);
      local_B8 = 1;
    } else {
      Script_SetCharaMoveTargetEx(arg0, &local_F4, arg1, task_34 | 0x2C6, 0, -1.0f, 2);
      local_B8 = arg6;
    }
  }
  while (Script_SetCharaXMPTMode(arg0, 2) != 0) {
    SCNF_WaitFrames_0000(1);
  }
  return local_B8;
}

__attribute__((scn_string_bindings(0, data_1DC57)))
static int32_t script_1D96(int32_t arg0, int32_t arg1, float arg2, float arg3, int32_t arg4, int32_t arg5, int32_t arg6) {
  Vector3 local_F4;
  float local_E8[3];
  Vector3 local_DC;
  Vector3 local_D0;
  float local_CC;
  float local_C8;
  int32_t local_C4;
  float local_C0;
  float local_BC;
  int32_t local_B8;
  Script_EVT_GetPosition(arg0, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  Script_ccowUtils(arg0, 2);
  if (arg5 & 0x100) {
    local_CC = Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, arg1);
    local_C8 = Script_MathUtils(SCRIPT_MATH_COS_FIXED16, arg1);
    local_C0 = (&local_F4)->x - arg2;
    local_BC = (&local_F4)->z - arg3;
    local_C0 = local_C0 * local_C8 - local_BC * local_CC;
    local_BC = 0.0f;
    local_C4 = 0x10000 - arg1;
    local_CC = Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, local_C4);
    local_C8 = Script_MathUtils(SCRIPT_MATH_COS_FIXED16, local_C4);
    local_E8[0] = local_C0 * local_C8 - local_BC * local_CC;
    local_E8[2] = local_C0 * local_CC + local_BC * local_C8;
    arg2 = *local_E8 + arg2;
    arg3 = local_E8[2] + arg3;
  }
  task_34 = 0;
  if (task_49) {
    Script_motmToggleFlags(arg0, 2, 0x40);
    if (Script_motmUtilsEx(13, arg0, 0) != 0) {
      SCNF_WaitFrames_0000(1);
    }
    Script_motmToggleFlags(arg0, 1, 0x40);
    local_F4.x = arg2;
    (&local_F4)->z = arg3;
    Script_EVT_GetPosition(arg0, 26, &local_DC, ENTITY_XFORM_WORLD_SPACE);
    Script_EVT_GetPosition(arg0, 33, &local_D0, ENTITY_XFORM_WORLD_SPACE);
    Script_motmUtilsEx(14, arg0, 0x3F800000);
    if (!((&local_DC)->y >= (&local_D0)->y)) {
      task_34 = -0x80000000;
      *((int8_t *)&*(int32_t *)&task_47 + arg4) = 1;
    } else {
      *((int8_t *)&*(int32_t *)&task_47 + arg4) = 0;
    }
    Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
    if (arg6 > 1) {
      Script_SetCharaMoveTargetEx(arg0, &local_F4, arg1, task_34 | 0x2F9 | 0x30000, 0, -1.0f, 2);
      local_B8 = 0;
    } else {
      Script_SetCharaMoveTargetEx(arg0, &local_F4, arg1, task_34 | 0x2F9 | 0x30000, 0, -1.0f, 2);
      local_B8 = arg6;
    }
  } else {
    local_F4.x = arg2;
    (&local_F4)->z = arg3;
    Script_EVT_GetPosition(arg0, 26, &local_DC, ENTITY_XFORM_WORLD_SPACE);
    Script_EVT_GetPosition(arg0, 33, &local_D0, ENTITY_XFORM_WORLD_SPACE);
    if (!((&local_DC)->y >= (&local_D0)->y)) {
      task_34 = -0x80000000;
      *((int8_t *)&*(int32_t *)&task_47 + arg4) = 1;
    } else {
      *((int8_t *)&*(int32_t *)&task_47 + arg4) = 0;
    }
    Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
    if (arg6 > 1) {
      Script_SetCharaMoveTargetEx(arg0, &local_F4, arg1, task_34 | 0x2C7 | 0x90000, 0, -1.0f, 2);
      local_B8 = 0;
    } else {
      Script_SetCharaMoveTargetEx(arg0, &local_F4, arg1, task_34 | 0x2C7 | 0x90000, 0, -1.0f, 2);
      local_B8 = arg6;
    }
  }
  while (Script_SetCharaXMPTMode(arg0, 2) != 0) {
    SCNF_WaitFrames_0000(1);
  }
  Script_ccowUtils(arg0, 1);
  script_2D34(arg0, arg4, arg5);
  if (*(&task_C + arg4) & 2) {
    if (Script_ryoControl(PCCTL_QueryShouldRun)) {
      Script_sprintf("Kaidan : ");
      Script_ryoControl(PCCTL_DisablePlayerControl);
      (void)((task_50 = -1) & 4);
    }
    Script_ccowUtils(arg0, 6);
  }
  return local_B8;
}

__attribute__((scn_string_bindings(1, data_1DC7D)))
static void script_20A3(int32_t arg0, int32_t arg1, int32_t arg2) {
  int32_t local_FC;
  int32_t local_F8;
  int8_t local_F7;
  (void)((local_F7 = 0) & 4);
  arg2 = arg2 & -2;
  local_F8 = Script_ScenDataUtil((void *)task_4, 4, arg2 * 6 + 4);
  local_FC = Script_ScenDataUtil((void *)task_4, 4, (arg2 + 1) * 6 + 4);
  local_FC = local_FC | local_F8;
  if (local_FC & -0x80000000) {
    while (local_FC & -0x80000000) {
      if (local_F7 & 0x80) {
        if (task_54) {
        } else {
          Script_sprintf("wait ... shuki shuki shuki\012");
        }
      } else {
        if (*(&task_C + arg1) & 2) {
          if (Script_ryoControl(PCCTL_QueryShouldRun)) {
            Script_sprintf("Kaidan : ");
            Script_ryoControl(PCCTL_DisablePlayerControl);
            (void)((task_50 = -1) & 4);
          }
          Script_ccowUtils(arg0, 6);
        }
        Script_charaToggleMotEnabled(arg0, 0);
        if (Script_ryoControl(PCCTL_QueryIsRunningRequest)) {
          Script_PlayCharaMotion(arg0, MN_AKI_AKI_RUN_EN, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        } else {
          Script_PlayCharaMotion(arg0, MN_AKI_AKI_WALK_EN, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        }
        SCNF_WaitFrames_0000(1);
        while (Script_IsCharaMotionFinished(arg0) == 0) {
          SCNF_WaitFrames_0000(1);
        }
        Script_charaToggleMotEnabled(arg0, 1);
        Script_PlayCharaMotion(arg0, MN_AKI_AKI_MOVE_B_45, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        Script_SetCharaMotionPlaybackRate(arg0, 1.2f);
        SCNF_WaitFrames_0000(1);
        while (Script_IsCharaMotionFinished(arg0) == 0) {
          SCNF_WaitFrames_0000(1);
        }
        (void)((local_F7 = -1) & 4);
      }
      task_54 = (task_54 + 1) & 0xF;
      local_F8 = Script_ScenDataUtil((void *)task_4, 4, arg2 * 6 + 4);
      local_FC = Script_ScenDataUtil((void *)task_4, 4, (arg2 + 1) * 6 + 4);
      local_FC = local_FC | local_F8;
      SCNF_WaitFrames_0000(1);
    }
    Script_PlayCharaMotion(arg0, MN_AKI_AKI_WALK_ST, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_motmToggleFlags(arg0, 1, 0x80);
    SCNF_WaitFrames_0000(1);
    while (Script_IsCharaMotionFinished(arg0) == 0) {
      SCNF_WaitFrames_0000(1);
    }
  }
  return;
}

static void script_22D1(int32_t arg0) {
  if (~task_4B) {
    (void)((task_4B = -1) & 1);
    task_38 = Script_SetCameraSettings(CAM_UTILS_SET_DISTANCE_FROM_PLAYER, -1.0f);
    task_3C = Script_SetCameraSettings(CAM_UTILS_SET_HEIGHT_ABOVE_PLAYER, -1.0f);
    task_40 = Script_SetCameraSettings(CAM_UTILS_SET_MAX_Y_ANGLE, -1.0f);
  }
  Script_SetCameraSettings(6, 4);
  Script_convSetCamInactiveFlag(0);
  if (arg0) {
    Script_SetCameraSettings(CAM_UTILS_SET_DISTANCE_FROM_PLAYER, 3.1f);
    Script_SetCameraSettings(CAM_UTILS_SET_HEIGHT_ABOVE_PLAYER, 0.0f);
    Script_SetCameraSettings(CAM_UTILS_SET_MAX_Y_ANGLE, 1.0f);
  } else {
    Script_SetCameraSettings(CAM_UTILS_SET_DISTANCE_FROM_PLAYER, 2.0f);
    Script_SetCameraSettings(CAM_UTILS_SET_HEIGHT_ABOVE_PLAYER, 2.0f);
    Script_SetCameraSettings(CAM_UTILS_SET_MAX_Y_ANGLE, 1.0f);
  }
  return;
}

static void script_2370(float arg0) {
  int32_t local_FC;
  float local_F8;
  local_FC = 0;
  while (local_FC < 10) {
    int32_t local_F4;
    local_F4 = task_4A;
    if ((local_F4 == 0) | (local_F4 == 2) | (local_F4 == 4)) {
      break;
      goto L0023A3;
    }
    L0023A3:;
    local_F8 = task_40 + (arg0 - task_40) * (float)local_FC / 10.0f;
    Script_SetCameraSettings(CAM_UTILS_SET_MAX_Y_ANGLE, local_F8);
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  {
    int32_t local_F4;
    local_F4 = task_4A;
    if ((local_F4 == 0) | (local_F4 == 2) | (local_F4 == 4)) {
    } else {
      Script_SetCameraSettings(CAM_UTILS_SET_MAX_Y_ANGLE, arg0);
    }
  }
  return;
}

static void script_23FF(float arg0) {
  int32_t local_FC;
  float local_F8;
  local_FC = 0;
  while (local_FC < 50) {
    int32_t local_F4;
    local_F4 = task_4A;
    if ((local_F4 == 0) | (local_F4 == 2) | (local_F4 == 4)) {
      break;
      goto L002432;
    }
    L002432:;
    local_F8 = task_38 + (arg0 - task_38) * (float)local_FC / 50.0f;
    Script_SetCameraSettings(CAM_UTILS_SET_DISTANCE_FROM_PLAYER, local_F8);
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  {
    int32_t local_F4;
    local_F4 = task_4A;
    if ((local_F4 == 0) | (local_F4 == 2) | (local_F4 == 4)) {
    } else {
      Script_SetCameraSettings(CAM_UTILS_SET_DISTANCE_FROM_PLAYER, arg0);
    }
  }
  return;
}

static void script_248E(void) {
  Script_SetCameraSettings(6, 7);
  Script_SetCameraSettings(CAM_UTILS_SET_DISTANCE_FROM_PLAYER, task_38);
  Script_SetCameraSettings(CAM_UTILS_SET_HEIGHT_ABOVE_PLAYER, task_3C);
  Script_SetCameraSettings(CAM_UTILS_SET_MAX_Y_ANGLE, task_40);
  Script_KaidanUtils(CAM_STAIR_RESET_RETRACTION_LIMIT);
  (void)((task_4B = 0) & 2);
  return;
}

static int32_t script_24C5(int32_t arg0, int32_t arg1) {
  Vector3 local_F4;
  Vector3 local_E8;
  int8_t local_E7;
  int8_t local_E6;
  task_1C = arg0;
  task_4A = 1;
  local_E7 = -1;
  (void)((local_E6 = 0) & 2);
  Script_SetGameStateTo(AUTH_None);
  while (task_1C == arg0) {
    if (Script_GetAuthorState_And_SetCamPos(&local_F4, &local_E8) != 17) {
      if (task_4A != local_E7) {
        uint8_t unused_E4[2];
        int32_t local_E0;
        local_E0 = task_4A;
        if (local_E0 == 1) {
          (void)((local_E6 = -1) & 2);
          script_22D1(arg1);
        } else if (local_E0 == 2) {
          script_248E();
        } else if ((local_E0 == 0) | (local_E0 == 3)) {
          Script_SetCameraSettings(6, 7);
          task_1C = 0;
        } else {
          if (local_E0 == 4) goto L002572;
        }
        L002572:;
        local_E7 = task_4A;
      }
    } else {
      if (task_4A != local_E7) {
        uint8_t unused_E4[2];
        int32_t local_E0;
        local_E0 = task_4A;
        if ((local_E0 == 0) | (local_E0 == 3)) {
          Script_SetCameraSettings(6, 7);
          task_1C = 0;
        } else if ((local_E0 == 1) | (local_E0 == 2)) {
          Script_SetGameStateTo(AUTH_FirstPerson);
        } else {
          if (local_E0 == 4) goto L0025D2;
        }
        L0025D2:;
        local_E7 = 4;
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  if (task_4A != 0) {
    if (local_E6 & 0x40) {
      script_248E();
    }
  }
  end:;
}

static void script_25FC(int32_t arg0) {
  int8_t local_FF;
  int32_t local_F8;
  local_F8 = -1;
  local_FF = 0;
  while (local_FF < task_48) {
    if (arg0 == *(&task_14 + local_FF)) {
      local_F8 = *(&task_18 + local_FF);
    }
    local_FF = local_FF + 1;
  }
  (void)local_F8;
  return;
}

static void script_2643(int32_t arg0) {
  task_8 = arg0;
  return;
}

static void script_264D(int32_t arg0) {
  task_20 = arg0;
  return;
}

static int32_t script_2657(void) {
  return task_20;
}

__attribute__((scn_string_bindings(0, data_1DC87)))
static void script_265E(int32_t arg0) {
  Vector3 local_F4;
  Vector3i local_E8;
  float local_E4;
  float local_E0;
  float local_DC;
  Script_EVT_GetPosition(arg0, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  Script_mtwkGetSetRotation(arg0, &local_E8, 0);
  local_E4 = Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_E8)->y);
  local_E0 = Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_E8)->y);
  SCNF_WaitFrames_0000(1);
  while (Script_IsCharaMotionFinished(arg0) == 0) {
    SCNF_WaitFrames_0000(1);
  }
  local_F4.x = (&local_F4)->x - local_E4 * 0.6f;
  (&local_F4)->y = (&local_F4)->y + 0.45f;
  (&local_F4)->z = (&local_F4)->z - local_E0 * 0.6f;
  local_DC = Script_GetHighestCollisionBelow(arg0, &local_F4);
  if (!(local_DC < (&local_F4)->y - 0.1f)) {
    (&local_F4)->y = local_DC;
    Script_EVT_SetPosition(arg0, 0x78000000, &local_F4);
  } else {
    Script_sprintf("KAIDAN : Warning coli!\012");
  }
  return;
}

__attribute__((scn_string_bindings(0, data_1DC9F)))
static void script_2739(int32_t arg0) {
  Vector3 local_F4;
  Vector3i local_E8;
  float local_E4;
  float local_E0;
  float local_DC;
  Script_EVT_GetPosition(arg0, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  Script_mtwkGetSetRotation(arg0, &local_E8, 0);
  local_E4 = Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_E8)->y);
  local_E0 = Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_E8)->y);
  SCNF_WaitFrames_0000(1);
  while (Script_IsCharaMotionFinished(arg0) == 0) {
    SCNF_WaitFrames_0000(1);
  }
  local_F4.x = (&local_F4)->x - local_E4 * 0.480000019f;
  (&local_F4)->y = (&local_F4)->y + 0.359999985f;
  (&local_F4)->z = (&local_F4)->z - local_E0 * 0.480000019f;
  local_DC = Script_GetHighestCollisionBelow(arg0, &local_F4);
  if (!(local_DC < (&local_F4)->y - 0.1f)) {
    (&local_F4)->y = local_DC;
    Script_EVT_SetPosition(arg0, 0x78000000, &local_F4);
  } else {
    Script_sprintf("KAIDAN : Warning coli!\012");
  }
  return;
}

__attribute__((scn_string_bindings(0, data_1DCB7)))
static void script_2814(int32_t arg0) {
  Vector3 local_F4;
  Vector3i local_E8;
  float local_E4;
  float local_E0;
  float local_DC;
  Script_EVT_GetPosition(arg0, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  Script_mtwkGetSetRotation(arg0, &local_E8, 0);
  local_E4 = Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_E8)->y);
  local_E0 = Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_E8)->y);
  SCNF_WaitFrames_0000(1);
  while (Script_IsCharaMotionFinished(arg0) == 0) {
    SCNF_WaitFrames_0000(1);
  }
  local_F4.x = (&local_F4)->x - local_E4 * 1.2f;
  (&local_F4)->y = (&local_F4)->y + 0.9f;
  (&local_F4)->z = (&local_F4)->z - local_E0 * 1.2f;
  local_DC = Script_GetHighestCollisionBelow(arg0, &local_F4);
  if (!(local_DC < (&local_F4)->y - 0.1f)) {
    (&local_F4)->y = local_DC;
    Script_EVT_SetPosition(arg0, 0x78000000, &local_F4);
  } else {
    Script_sprintf("KAIDAN : Warning coli!\012");
  }
  return;
}

__attribute__((scn_string_bindings(0, data_1DCCF)))
static void script_28EF(int32_t arg0, int32_t arg1) {
  Vector3 local_F4;
  Vector3i local_E8;
  float local_E4;
  float local_E0;
  float local_DC;
  if (*((int8_t *)&*(int32_t *)&task_46 + arg1) == 0) {
    *((int8_t *)&*(int32_t *)&task_46 + arg1) = 1;
  } else {
    Script_EVT_GetPosition(arg0, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
    Script_mtwkGetSetRotation(arg0, &local_E8, 0);
    local_E4 = Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_E8)->y);
    local_E0 = Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_E8)->y);
    SCNF_WaitFrames_0000(1);
    while (Script_IsCharaMotionFinished(arg0) == 0) {
      SCNF_WaitFrames_0000(1);
    }
    local_F4.x = (&local_F4)->x - local_E4 * 0.6f;
    (&local_F4)->y = (&local_F4)->y - 0.45f;
    (&local_F4)->z = (&local_F4)->z - local_E0 * 0.6f;
    local_DC = Script_GetHighestCollisionBelow(arg0, &local_F4);
    if (!(local_DC < (&local_F4)->y - 0.1f)) {
      (&local_F4)->y = local_DC;
      Script_EVT_SetPosition(arg0, 0x78000000, &local_F4);
    } else {
      Script_sprintf("KAIDAN : Warning coli!\012");
    }
  }
  return;
}

__attribute__((scn_string_bindings(0, data_1DCE7)))
static void script_29E5(int32_t arg0, int32_t arg1) {
  Vector3 local_F4;
  Vector3i local_E8;
  float local_E4;
  float local_E0;
  float local_DC;
  if (*((int8_t *)&*(int32_t *)&task_46 + arg1) == 0) {
    *((int8_t *)&*(int32_t *)&task_46 + arg1) = 1;
  } else {
    Script_EVT_GetPosition(arg0, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
    Script_mtwkGetSetRotation(arg0, &local_E8, 0);
    local_E4 = Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_E8)->y);
    local_E0 = Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_E8)->y);
    SCNF_WaitFrames_0000(1);
    while (Script_IsCharaMotionFinished(arg0) == 0) {
      SCNF_WaitFrames_0000(1);
    }
    local_F4.x = (&local_F4)->x - local_E4 * 1.2f;
    (&local_F4)->y = (&local_F4)->y - 0.9f;
    (&local_F4)->z = (&local_F4)->z - local_E0 * 1.2f;
    local_DC = Script_GetHighestCollisionBelow(arg0, &local_F4);
    if (!(local_DC < (&local_F4)->y - 0.1f)) {
      (&local_F4)->y = local_DC;
      Script_EVT_SetPosition(arg0, 0x78000000, &local_F4);
    } else {
      Script_sprintf("KAIDAN : Warning coli!\012");
    }
  }
  return;
}

__attribute__((scn_string_bindings(0, data_1DCFF)))
static int32_t script_2ADB(int32_t arg0, int32_t arg1) {
  float local_FC;
  float local_F8;
  if (*((int8_t *)&*(int32_t *)&task_45 + arg1)) {
    Script_sprintf("KAIDAN : Warning setY!\012");
  } else {
    *((int8_t *)&*(int32_t *)&task_45 + arg1) = 1;
    if (task_49) {
      local_FC = local_F8 = -0.001f;
    } else {
      local_FC = local_F8 = 0.0f;
    }
    while (*((int8_t *)&*(int32_t *)&task_45 + arg1) == 1) {
      if (task_49) {
        if (!(local_F8 <= -0.16f)) {
          local_FC = local_F8 - 0.001f;
        } else {
          local_FC = -0.001f;
        }
      } else if (!(local_F8 >= 0.0f)) {
        local_FC = local_F8 + 0.001f;
      } else {
        local_FC = 0.0f;
      }
      Script_KaidanSetY(arg0, local_FC);
      local_F8 = local_FC;
      SCNF_WaitFrames_0000(1);
    }
    if (*((int8_t *)&*(int32_t *)&task_45 + arg1) != 2) {
      for (;;) {
        if (local_FC >= 0.0f) {
          break;
        }
        if (*((int8_t *)&*(int32_t *)&task_45 + arg1) == 2) {
          *((int8_t *)&*(int32_t *)&task_45 + arg1) = 0;
          Script_KaidanSetY(arg0, 0.0f);
          break;
        }
        local_FC = local_FC + 0.004f;
        Script_KaidanSetY(arg0, local_FC);
        SCNF_WaitFrames_0000(1);
      }
    } else {
      *((int8_t *)&*(int32_t *)&task_45 + arg1) = 0;
      Script_KaidanSetY(arg0, 0.0f);
    }
  }
  end:;
}

__attribute__((scn_string_bindings(0, data_1DD17)))
static int32_t script_2C03(int32_t arg0, int32_t arg1) {
  float local_FC;
  float local_F8;
  if (*((int8_t *)&*(int32_t *)&task_45 + arg1)) {
    Script_sprintf("KAIDAN : Warning setY!\012");
  } else {
    *((int8_t *)&*(int32_t *)&task_45 + arg1) = 1;
    if (task_49) {
      local_FC = local_F8 = 0.16f;
    } else {
      local_FC = local_F8 = 0.08f;
    }
    while (*((int8_t *)&*(int32_t *)&task_45 + arg1) == 1) {
      if (task_49) {
        if (!(local_F8 >= 0.16f)) {
          local_FC = local_F8 + 0.004f;
        } else {
          local_FC = 0.16f;
        }
      } else if (!(local_F8 <= 0.08f)) {
        local_FC = local_F8 - 0.004f;
      } else {
        local_FC = 0.08f;
      }
      Script_KaidanSetY(arg0, local_FC);
      local_F8 = local_FC;
      SCNF_WaitFrames_0000(1);
    }
    if (*((int8_t *)&*(int32_t *)&task_45 + arg1) != 2) {
      for (;;) {
        if (local_FC <= 0.0f) {
          break;
        }
        if (*((int8_t *)&*(int32_t *)&task_45 + arg1) == 2) {
          *((int8_t *)&*(int32_t *)&task_45 + arg1) = 0;
          Script_KaidanSetY(arg0, 0.0f);
          break;
        }
        local_FC = local_FC - 0.004f;
        Script_KaidanSetY(arg0, local_FC);
        SCNF_WaitFrames_0000(1);
      }
    } else {
      *((int8_t *)&*(int32_t *)&task_45 + arg1) = 0;
      Script_KaidanSetY(arg0, 0.0f);
    }
  }
  end:;
}

static void script_2D34(int32_t arg0, int32_t arg1, int32_t arg2) {
  if ((*(&task_C + arg1) & 2) > 0) {
    if (Script_ryoControl(PCCTL_QueryShouldRun)) {
      Script_sprintf(data_1DD2F);
      Script_ryoControl(PCCTL_DisablePlayerControl);
      (void)((task_50 = -1) & 4);
    }
    Script_ccowUtils(arg0, 6);
  }
  if (*(&task_C + arg1) & 0x20) {
    Script_ccowUtils(arg0, 4);
  } else {
    if ((arg2 & 0x100) | (arg2 & 0x4000) | ((arg2 & 0x8000) > 0)) {
      Script_ccowUtils(arg0, 4);
      goto L002DB8;
    }
  }
  L002DB8:;
  return;
}

static void script_2DB9(int32_t arg0, int32_t arg1, int32_t arg2) {
  if (*(&task_C + arg1) & 0x20) {
    Script_ccowUtils(arg0, 3);
  } else if ((arg2 & 0x8000) > 0) {
  } else {
    if ((arg2 & 0x100) | ((arg2 & 0x4000) > 0)) {
      Script_ccowUtils(arg0, 3);
      goto L002E08;
    }
  }
  L002E08:;
  return;
}

static void script_2E09(int32_t arg0, int32_t arg1, int32_t arg2) {
  Script_ccowUtils(arg0, 5);
  if (*(&task_C + arg1) & 0x20) {
    Script_ccowUtils(arg0, 3);
  } else {
    if ((arg2 & 0x100) | (arg2 & 0x4000) | ((arg2 & 0x8000) > 0)) {
      Script_ccowUtils(arg0, 3);
      goto L002E57;
    }
  }
  L002E57:;
  return;
}

static void script_2E58(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t local_E4;
  int32_t local_E0;
  local_E0 = arg3;
  switch (local_E0) {
    case 0:
      local_F4.x = 0.1f;
      (&local_F4)->y = 0.0f;
      (&local_F4)->z = -0.01f;
      local_E8.x = 0;
      (&local_E8)->y = 64443;
      (&local_E8)->z = 0;
      local_E4 = 18;
      break;
    case 1:
      local_F4.x = 0.163f;
      (&local_F4)->y = 0.015f;
      (&local_F4)->z = 0.014f;
      local_E8.x = 0;
      (&local_E8)->y = 0x8000;
      (&local_E8)->z = 0;
      local_E4 = 18;
      break;
    case 2:
      local_F4.x = 0.064f;
      (&local_F4)->y = -0.071f;
      (&local_F4)->z = -0.366f;
      local_E8.x = 22570;
      (&local_E8)->y = 47422;
      (&local_E8)->z = 50974;
      local_E4 = 18;
      break;
    case 3:
      local_F4.x = 0.1f;
      (&local_F4)->y = 0.0f;
      (&local_F4)->z = -0.036f;
      local_E8.x = 0;
      (&local_E8)->y = 0;
      (&local_E8)->z = 0;
      local_E4 = 18;
      break;
    case 4:
      local_F4.x = 0.563f;
      (&local_F4)->y = -0.18f;
      (&local_F4)->z = -0.033f;
      local_E8.x = 0;
      (&local_E8)->y = 0;
      (&local_E8)->z = 0x3000;
      local_E4 = 18;
      break;
    case 5:
      local_F4.x = 0.129f;
      (&local_F4)->y = -0.037f;
      (&local_F4)->z = -0.233f;
      local_E8.x = 18313;
      (&local_E8)->y = 46148;
      (&local_E8)->z = 56615;
      local_E4 = 18;
      break;
    case 6:
      local_F4.x = 0.143f;
      (&local_F4)->y = 0.024f;
      (&local_F4)->z = 0.01f;
      local_E8.x = 10920;
      (&local_E8)->y = 11829;
      (&local_E8)->z = 4550;
      local_E4 = 18;
      break;
    case 7:
      local_F4.x = -0.066f;
      (&local_F4)->y = -1.05f;
      (&local_F4)->z = -0.413f;
      local_E8.x = 5367;
      (&local_E8)->y = 0x2000;
      (&local_E8)->z = 2274;
      local_E4 = 18;
      break;
    case 8:
      local_F4.x = 0.135f;
      (&local_F4)->y = 0.0f;
      (&local_F4)->z = -0.384f;
      local_E8.x = 46789;
      (&local_E8)->y = 49970;
      (&local_E8)->z = 20929;
      local_E4 = 18;
      break;
    case 9:
      local_F4.x = 0.1f;
      (&local_F4)->y = 0.0f;
      (&local_F4)->z = -0.02f;
      local_E8.x = 0;
      (&local_E8)->y = 0;
      (&local_E8)->z = 0;
      local_E4 = 18;
      break;
  }
  if (Script_motnUtils(2, arg0)) {
    Script_motnUtils(1, arg0, 0);
  }
  SCNF_WaitFrames_0000(1);
  Script_motmToggleFlags(arg0, 2, 0x40);
  Script_motmGetCharaFlags(arg0, 0, 0);
  Script_PlayCharaMotion(arg0, arg2, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  SCNF_WaitFrames_0000(1);
  Script_fixoAttachEx(arg1, arg0, local_E4, &local_F4, &local_E8);
  Script_motmGetCharaFlags(arg0, 2, 0);
  if (Script_motnUtils(2, arg0)) {
    Script_motnUtils(1, arg0, 1);
  }
  return;
}

static void script_31EE(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t local_E4;
  int32_t local_E0;
  local_E0 = arg3;
  switch (local_E0) {
    case 0:
      local_F4.x = 0.066f;
      (&local_F4)->y = 0.0f;
      (&local_F4)->z = -0.016f;
      local_E8.x = 0xC000;
      (&local_E8)->y = 0;
      (&local_E8)->z = 0xD000;
      local_E4 = 18;
      break;
    case 1:
      local_F4.x = 0.071f;
      (&local_F4)->y = -0.9f;
      (&local_F4)->z = -0.197f;
      local_E8.x = 1820;
      (&local_E8)->y = -64350;
      (&local_E8)->z = 364;
      local_E4 = 18;
      break;
    case 2:
      local_F4.x = 0.147f;
      (&local_F4)->y = -0.86f;
      (&local_F4)->z = -0.35f;
      local_E8.x = 3913;
      (&local_E8)->y = 0;
      (&local_E8)->z = 910;
      local_E4 = 18;
      break;
    case 3:
      local_F4.x = -0.29f;
      (&local_F4)->y = -1.095f;
      (&local_F4)->z = -0.123f;
      local_E8.x = 43146;
      (&local_E8)->y = 47880;
      (&local_E8)->z = 19113;
      local_E4 = 18;
      break;
    case 4:
      local_F4.x = 0.113f;
      (&local_F4)->y = 0.0f;
      (&local_F4)->z = -0.035f;
      local_E8.x = 182;
      (&local_E8)->y = 62441;
      (&local_E8)->z = 65171;
      local_E4 = 18;
      break;
    case 5:
      local_F4.x = 0.088f;
      (&local_F4)->y = 0.036f;
      (&local_F4)->z = -0.048f;
      local_E8.x = 5279;
      (&local_E8)->y = 37523;
      (&local_E8)->z = 5825;
      local_E4 = 18;
      break;
    case 6:
      local_F4.x = 0.103f;
      (&local_F4)->y = 0.012f;
      (&local_F4)->z = 0.021f;
      local_E8.x = 59892;
      (&local_E8)->y = 3845;
      (&local_E8)->z = 364;
      local_E4 = 12;
      break;
    case 7:
      local_F4.x = 0.1136f;
      (&local_F4)->y = 0.0f;
      (&local_F4)->z = 0.013f;
      local_E8.x = 44540;
      (&local_E8)->y = 43679;
      (&local_E8)->z = 13666;
      local_E4 = 18;
      break;
    case 8:
      local_F4.x = 0.131f;
      (&local_F4)->y = -0.045f;
      (&local_F4)->z = -0.375f;
      local_E8.x = 18204;
      (&local_E8)->y = 47058;
      (&local_E8)->z = 58166;
      local_E4 = 18;
      break;
    case 9:
      local_F4.x = 0.07f;
      (&local_F4)->y = 0.01f;
      (&local_F4)->z = -0.02f;
      local_E8.x = 0x4000;
      (&local_E8)->y = 0xC000;
      (&local_E8)->z = 0xA000;
      local_E4 = 18;
      break;
    case 10:
      local_F4.x = 0.049f;
      (&local_F4)->y = 0.001f;
      (&local_F4)->z = -0.011f;
      local_E8.x = 64169;
      (&local_E8)->y = 0;
      (&local_E8)->z = 0;
      local_E4 = 18;
      break;
    case 11:
      local_F4.x = 0.1f;
      (&local_F4)->y = 0.0f;
      (&local_F4)->z = -0.02f;
      local_E8.x = 0;
      (&local_E8)->y = 0;
      (&local_E8)->z = 0;
      local_E4 = 18;
      break;
    case 12:
      local_F4.x = 0.114f;
      (&local_F4)->y = -0.018f;
      (&local_F4)->z = -0.055f;
      local_E8.x = 0x1000;
      (&local_E8)->y = 0;
      (&local_E8)->z = 0;
      local_E4 = 18;
      break;
    case 13:
      local_F4.x = 0.257f;
      (&local_F4)->y = -0.072f;
      (&local_F4)->z = 0.064f;
      local_E8.x = 0xF000;
      (&local_E8)->y = 0xA000;
      (&local_E8)->z = 0x2000;
      local_E4 = 18;
      break;
    case 14:
      local_F4.x = 0.061f;
      (&local_F4)->y = 0.015f;
      (&local_F4)->z = 0.026f;
      local_E8.x = 19842;
      (&local_E8)->y = 61986;
      (&local_E8)->z = 55977;
      local_E4 = 12;
      break;
    case 15:
      local_F4.x = 0.2f;
      (&local_F4)->y = 0.0f;
      (&local_F4)->z = 0.03f;
      local_E8.x = 0xC000;
      (&local_E8)->y = 30947;
      (&local_E8)->z = 0x4000;
      local_E4 = 12;
      break;
    case 16:
      local_F4.x = 0.089f;
      (&local_F4)->y = 0.016f;
      (&local_F4)->z = -0.017f;
      local_E8.x = 0x4000;
      (&local_E8)->y = 0;
      (&local_E8)->z = 0;
      local_E4 = 18;
      break;
    case 17:
      local_F4.x = 0.11f;
      (&local_F4)->y = 0.055f;
      (&local_F4)->z = -0.005f;
      local_E8.x = 47336;
      (&local_E8)->y = 18232;
      (&local_E8)->z = 59134;
      local_E4 = 18;
      break;
    case 18:
      local_F4.x = 0.132f;
      (&local_F4)->y = 0.002f;
      (&local_F4)->z = -0.019f;
      local_E8.x = 36416;
      (&local_E8)->y = 33701;
      (&local_E8)->z = 47303;
      local_E4 = 18;
      break;
    case 19:
      local_F4.x = 0.134f;
      (&local_F4)->y = -0.033f;
      (&local_F4)->z = -0.036f;
      local_E8.x = 0;
      (&local_E8)->y = 41859;
      (&local_E8)->z = 0;
      local_E4 = 18;
      break;
  }
  Script_motmToggleFlags(arg0, 2, 0x40);
  Script_motmGetCharaFlags(arg0, 0, 0);
  Script_PlayCharaMotion(arg0, arg2, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  SCNF_WaitFrames_0000(1);
  Script_fixoAttachEx(arg1, arg0, local_E4, &local_F4, &local_E8);
  Script_motmGetCharaFlags(arg0, 2, 0);
  return;
}

static void script_387C(int32_t arg0, float arg1, float arg2, float arg3) {
  Vector3 local_F4;
  local_F4.x = arg1;
  (&local_F4)->y = arg2;
  (&local_F4)->z = arg3;
  Script_EVT_SetPosition(arg0, POS_XYZ, &local_F4);
  return;
}

static void script_38A4(int32_t arg0, float arg1, float arg2, float arg3) {
  Vector3i local_F4;
  local_F4.x = (int32_t)(arg1 / 360.0f * (float)0x10000);
  (&local_F4)->y = (int32_t)(arg2 / 360.0f * (float)0x10000);
  (&local_F4)->z = (int32_t)(arg3 / 360.0f * (float)0x10000);
  Script_EVT_SetRotation(arg0, 0x38000000, &local_F4, 0);
  return;
}

static void script_38F8(int32_t arg0, int32_t arg1, int32_t arg2) {
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

static int32_t script_3948(int32_t arg0) {
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

static void script_39EF(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  local_FC = script_3948(arg0);
  if (arg1 > local_FC) {
    (void)0;
  } else {
    (void)-1;
  }
  return;
}

static int32_t script_3A12(int32_t arg0) {
  int32_t unused_FC;
  GameDateTime local_F4;
  int32_t local_F0;
  int32_t local_EC;
  Script_ReturnTimeDate(&local_F4);
  local_F0 = (&local_F4)->hours * 60 + (&local_F4)->minutes;
  local_EC = Script_scenUtils(SCENARIO_CMD_GET_STEP_LUT, arg0) * 6;
  return local_F0 - local_EC;
}

static void script_3A45(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  local_FC = script_3A12(arg0);
  if (arg1 > local_FC) {
    (void)0;
  } else {
    (void)-1;
  }
  return;
}

static void script_3A68(int32_t arg0, int32_t arg1) {
  int8_t local_F4[12];
  int32_t local_F0;
  local_F0 = script_3A12(arg0);
  local_F4[4] = local_F0 / 60;
  local_F4[5] = local_F0 % 60;
  if ((local_F4[4] < *(int8_t *)(arg1 + 4)) | ((local_F4[4] == *(int8_t *)(arg1 + 4)) & (local_F4[5] < *(int8_t *)(arg1 + 5)))) {
    (void)0;
  } else {
    (void)-1;
  }
  return;
}

static void script_3AC7(int32_t arg0) {
  int32_t local_F8[2];
  local_F8[0] = arg0;
  local_F8[1] = 0;
  (void)local_F8;
  return;
}

__attribute__((scn_string_bindings(0, data_1DD4C)))
static void script_3ADC(void) {
  Script_sprintf("TUNED FreeTalk def\012");
  SCNT_Enqueue();
  return;
}

__attribute__((scn_string_bindings(0, data_1DD60)))
static void script_3AEB(void) {
  Script_sprintf("TUNED FreeTalk def\012");
  SCNT_Enqueue();
  return;
}

static void script_3AFA(void) {
  return;
}

static void script_3AFD(void) {
  return;
}

static void script_3B00(void) {
  return;
}

static void script_3B03(void) {
  return;
}

static void script_3B06(void) {
  (void)((task_68 = 0) & 0x20);
  return;
}

static void script_3B13(void) {
  task_5C = Script_freeTalkGetCurrentChara();
  return;
}

static void script_3B1F(void) {
  *(&task_5C + 1) = Script_GetFreeTalkStatus();
  return;
}

static int32_t script_3B2E(void) {
  return *(&task_5C + 2) = Script_freeTalkGetAndResetState();
}

static int32_t script_3B3D(int32_t arg0) {
  if (arg0 < 0) {
    return -arg0;
  } else {
    return arg0;
  }
}

static void script_3B5B(float arg0) {
  if (!(arg0 >= 0.0f)) {
    (void)((float)0 - arg0);
    return;
  } else {
    (void)arg0;
    return;
  }
}

static int32_t script_3B7A(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  local_FC = arg1 - arg0;
  local_F8 = arg0 - arg1;
  if ((local_F8 & 0xFFFF) < (local_FC & 0xFFFF)) {
    if (arg0 > arg1) {
      local_F4 = local_F8 * -1;
    } else {
      local_F4 = (local_F8 & 0xFFFF) * -1;
    }
  } else if (arg0 > arg1) {
    local_F4 = local_FC & 0xFFFF;
  } else {
    local_F4 = local_FC & 0xFFFF;
  }
  return local_F4;
}

static void script_3BF2(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  int32_t local_FC;
  if (task_1CC != arg0) {
    Script_sprintf("Door Index = %d : call_type = %d : type = %d\012", task_B4[arg2], arg3, arg4);
    local_FC = arg1;
    switch (local_FC) {
      case 40731:
        Script_sprintf("Door Open Motion = DEB_AKESIME_FUSUMA_L_F\012");
        break;
      case 40732:
        Script_sprintf("Door Open Motion = DEB_AKESIME_FUSUMA_R_F\012");
        break;
      case 40705:
        Script_sprintf("Door Open Motion = DEB_HIKISIME_DOOR_L_F\012");
        break;
      case 40706:
        Script_sprintf("Door Open Motion = DEB_HIKISIME_DOOR_R_F\012");
        break;
      case 40733:
        Script_sprintf("Door Open Motion = DEB_OSISIME_DOOR_L_F\012");
        break;
      case 40707:
        Script_sprintf("Door Open Motion = DEB_OSISIME_DOOR_R_F\012");
        break;
      case 40734:
        Script_sprintf("Door Open Motion = GAK_AKESIME_FUSUMA_L_F\012");
        break;
      case 40735:
        Script_sprintf("Door Open Motion = GAK_AKESIME_FUSUMA_R_F\012");
        break;
      case 40708:
        Script_sprintf("Door Open Motion = GAK_HIKISIME_DOOR_L_F\012");
        break;
      case 40709:
        Script_sprintf("Door Open Motion = GAK_HIKISIME_DOOR_R_F\012");
        break;
      case 40736:
        Script_sprintf("Door Open Motion = GAK_OSISIME_DOOR_L_F\012");
        break;
      case 40710:
        Script_sprintf("Door Open Motion = GAK_OSISIME_DOOR_R_F\012");
        break;
      case 40737:
        Script_sprintf("Door Open Motion = JIJ_AKESIME_FUSUMA_L_F\012");
        break;
      case 40738:
        Script_sprintf("Door Open Motion = JIJ_AKESIME_FUSUMA_R_F\012");
        break;
      case 40711:
        Script_sprintf("Door Open Motion = JIJ_HIKISIME_DOOR_L_F\012");
        break;
      case 40712:
        Script_sprintf("Door Open Motion = JIJ_HIKISIME_DOOR_R_F\012");
        break;
      case 40739:
        Script_sprintf("Door Open Motion = JIJ_OSISIME_DOOR_L_F\012");
        break;
      case 40713:
        Script_sprintf("Door Open Motion = JIJ_OSISIME_DOOR_R_F\012");
        break;
      case 40740:
        Script_sprintf("Door Open Motion = KOD_AKESIME_FUSUMA_L_F\012");
        break;
      case 40741:
        Script_sprintf("Door Open Motion = KOD_AKESIME_FUSUMA_R_F\012");
        break;
      case 40714:
        Script_sprintf("Door Open Motion = KOD_HIKISIME_DOOR_L_F\012");
        break;
      case 40715:
        Script_sprintf("Door Open Motion = KOD_HIKISIME_DOOR_R_F\012");
        break;
      case 40742:
        Script_sprintf("Door Open Motion = KOD_OSISIME_DOOR_L_F\012");
        break;
      case 40716:
        Script_sprintf("Door Open Motion = KOD_OSISIME_DOOR_R_F\012");
        break;
      case 40743:
        Script_sprintf("Door Open Motion = OTH_AKESIME_FUSUMA_L_F\012");
        break;
      case 40744:
        Script_sprintf("Door Open Motion = OTH_AKESIME_FUSUMA_R_F\012");
        break;
      case 40717:
        Script_sprintf("Door Open Motion = OTH_HIKISIME_DOOR_L_F\012");
        break;
      case 40718:
        Script_sprintf("Door Open Motion = OTH_HIKISIME_DOOR_R_F\012");
        break;
      case 40745:
        Script_sprintf("Door Open Motion = OTH_OSISIME_DOOR_L_F\012");
        break;
      case 40719:
        Script_sprintf("Door Open Motion = OTH_OSISIME_DOOR_R_F\012");
        break;
      case 40746:
        Script_sprintf("Door Open Motion = PNW_AKESIME_FUSUMA_L_F\012");
        break;
      case 40747:
        Script_sprintf("Door Open Motion = PNW_AKESIME_FUSUMA_R_F\012");
        break;
      case 40720:
        Script_sprintf("Door Open Motion = PNW_HIKISIME_DOOR_L_F\012");
        break;
      case 40721:
        Script_sprintf("Door Open Motion = PNW_HIKISIME_DOOR_R_F\012");
        break;
      case 40748:
        Script_sprintf("Door Open Motion = PNW_OSISIME_DOOR_L_F\012");
        break;
      case 40722:
        Script_sprintf("Door Open Motion = PNW_OSISIME_DOOR_R_F\012");
        break;
      case 40749:
        Script_sprintf("Door Open Motion = SIN_AKESIME_FUSUMA_L_F\012");
        break;
      case 40750:
        Script_sprintf("Door Open Motion = SIN_AKESIME_FUSUMA_R_F\012");
        break;
      case 40723:
        Script_sprintf("Door Open Motion = SIN_HIKISIME_DOOR_L_F\012");
        break;
      case 40724:
        Script_sprintf("Door Open Motion = SIN_HIKISIME_DOOR_R_F\012");
        break;
      case 40751:
        Script_sprintf("Door Open Motion = SIN_OSISIME_DOOR_L_F\012");
        break;
      case 40725:
        Script_sprintf("Door Open Motion = SIN_OSISIME_DOOR_R_F\012");
        break;
      case 40757:
        Script_sprintf("Door Open Motion = SYP_AKESIME_FUSUMA_L_F\012");
        break;
      case 40752:
        Script_sprintf("Door Open Motion = SYP_AKESIME_FUSUMA_R_F\012");
        break;
      case 40726:
        Script_sprintf("Door Open Motion = SYP_HIKISIME_DOOR_L_F\012");
        break;
      case 40727:
        Script_sprintf("Door Open Motion = SYP_HIKISIME_DOOR_R_F\012");
        break;
      case 40753:
        Script_sprintf("Door Open Motion = SYP_OSISIME_DOOR_L_F\012");
        break;
      case 40728:
        Script_sprintf("Door Open Motion = SYP_OSISIME_DOOR_R_F\012");
        break;
      case 40758:
        Script_sprintf("Door Open Motion = YKI_AKESIME_FUSUMA_L_F\012");
        break;
      case 40754:
        Script_sprintf("Door Open Motion = YKI_AKESIME_FUSUMA_R_F\012");
        break;
      case 40729:
        Script_sprintf("Door Open Motion = YKI_HIKISIME_DOOR_L_F\012");
        break;
      case 40730:
        Script_sprintf("Door Open Motion = YKI_HIKISIME_DOOR_R_F\012");
        break;
      case 40755:
        Script_sprintf("Door Open Motion = YKI_OSISIME_DOOR_L_F\012");
        break;
      case 40756:
        Script_sprintf("Door Open Motion = YKI_OSISIME_DOOR_R_F\012");
        break;
      case 40759:
        Script_sprintf("Door Open Motion = JIJ_OSISIME_DOOR_AP_L_F\012");
        break;
      case 40760:
        Script_sprintf("Door Open Motion = OTH_OSISIME_DOOR_AP_L_F\012");
        break;
      case 40761:
        Script_sprintf("Door Open Motion = PNW_OSISIME_DOOR_AP_L_F\012");
        break;
      case 40762:
        Script_sprintf("Door Open Motion = SYP_OSISIME_DOOR_AP_L_F\012");
        break;
      case 40763:
        Script_sprintf("Door Open Motion = YKI_OSISIME_DOOR_AP_L_F\012");
        break;
    }
  }
  return;
}

static int32_t script_41A4(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  local_F8 = arg0;
  if (local_F8 == 0) {
    local_FC = 594;
  } else if (local_F8 == 1) {
    local_FC = 595;
  } else if (local_F8 == 2) {
    local_FC = 596;
  } else if (local_F8 == 3) {
    local_FC = 598;
  } else if (local_F8 == 4) {
    local_FC = 599;
  } else if (local_F8 == 5) {
    local_FC = 600;
  } else if (local_F8 == 6) {
    local_FC = 1000;
  } else if (local_F8 == 7) {
    local_FC = 656;
  } else if (local_F8 == 8) {
    local_FC = 601;
  } else if (local_F8 == 9) {
    local_FC = 594;
  } else {
    local_FC = 594;
  }
  return local_FC;
}

static void script_4275(int32_t arg0, int32_t arg1, int32_t arg2) {
  script_CF98(arg0, arg2, 0);
  return;
}

static void script_4285(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  float local_F8;
  float local_F4;
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
    local_FC = script_C72D(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
    if ((local_FC == 1) | (local_FC == 2)) {
      local_FC = 1;
    } else {
      local_FC = 0;
    }
    *(int32_t *)&local_F8 = Script_ScenDataUtil(data_1E364, 5, Script_ScenDataUtil(data_1DD74, 1, arg3 * 40 + task_104[arg2] * 4 + local_FC) * 9 + 5);
    *(int32_t *)&local_F4 = Script_ScenDataUtil(data_1E364, 5, Script_ScenDataUtil(data_1DD74, 1, arg3 * 40 + task_104[arg2] * 4 + local_FC) * 9 + 6);
    local_FC = script_C72D(arg0, arg1, 5, local_F8 / 100.0f, local_F4 / 100.0f, arg3, -1);
    script_7A11(arg0, arg1, arg2, arg3, local_FC, 0);
  }
  script_CF98(arg0, arg2, 0);
  return;
}

static void script_4389(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  int32_t local_F8;
  float local_F4;
  float local_F0;
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
    local_FC = script_C72D(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
    *(int32_t *)&local_F4 = Script_ScenDataUtil(data_1E364, 5, Script_ScenDataUtil(data_1DD74, 1, arg3 * 40 + task_104[arg2] * 4 + local_FC) * 9 + 5);
    local_F4 = 90.0f - local_F4;
    *(int32_t *)&local_F0 = Script_ScenDataUtil(data_1E364, 5, Script_ScenDataUtil(data_1DD74, 1, arg3 * 40 + task_104[arg2] * 4 + local_FC) * 9 + 6);
    if ((Script_ScenDataUtil(task_6C, 4, task_B4[arg2] * 13 + 1) & 0x40) | (Script_ScenDataUtil(task_6C, 4, task_B4[arg2] * 13 + 1) & 0x80)) {
      if (Script_ScenDataUtil(task_6C, 4, task_B4[arg2] * 13 + 1) & 0x40) {
        if ((local_FC == 0) | (local_FC == 1)) {
          local_F8 = 6;
        } else {
          local_F8 = 4;
        }
      } else if ((local_FC == 2) | (local_FC == 3)) {
        local_F8 = 6;
      } else {
        local_F8 = 4;
      }
    } else {
      local_F8 = 4;
    }
    local_FC = script_C72D(arg0, arg1, local_F8, local_F4 / (float)100, local_F0 / (float)100, arg3, task_B4[arg2]);
    script_7A11(arg0, arg1, arg2, arg3, local_FC, 1);
  }
  script_CF98(arg0, arg2, 0);
  return;
}

static void script_452B(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  float local_F8;
  float local_F4;
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
    local_FC = script_C72D(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
    if ((local_FC == 1) | (local_FC == 2)) {
      local_FC = 1;
    } else {
      local_FC = 0;
    }
    *(int32_t *)&local_F8 = Script_ScenDataUtil(data_1E364, 5, Script_ScenDataUtil(data_1DD74, 1, arg3 * 40 + task_104[arg2] * 4 + local_FC) * 9 + 5);
    *(int32_t *)&local_F4 = Script_ScenDataUtil(data_1E364, 5, Script_ScenDataUtil(data_1DD74, 1, arg3 * 40 + task_104[arg2] * 4 + local_FC) * 9 + 6);
    local_FC = script_C72D(arg0, arg1, 5, local_F8 / 100.0f, local_F4 / 100.0f, arg3, -1);
    if (local_FC == 1) {
      local_FC = 2;
    } else {
      local_FC = 3;
    }
    if ((Script_ScenDataUtil(task_6C, 4, task_B4[arg2] * 13 + 1) & 4) == 0) {
      script_5F1E(arg0, arg1, arg2, arg3, local_FC, 0);
    } else {
      if (arg0 == task_1CC) {
        script_92F1(arg0, arg1, arg2, arg3, local_FC, 0);
      }
    }
  }
  script_CF98(arg0, arg2, 2);
  return;
}

static void script_468C(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  float local_F8;
  float local_F4;
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
    local_FC = script_C72D(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
    if ((local_FC == 1) | (local_FC == 2)) {
      local_FC = 1;
    } else {
      local_FC = 0;
    }
    *(int32_t *)&local_F8 = Script_ScenDataUtil(data_1E364, 5, Script_ScenDataUtil(data_1DD74, 1, arg3 * 40 + task_104[arg2] * 4 + local_FC) * 9 + 5);
    *(int32_t *)&local_F4 = Script_ScenDataUtil(data_1E364, 5, Script_ScenDataUtil(data_1DD74, 1, arg3 * 40 + task_104[arg2] * 4 + local_FC) * 9 + 6);
    local_FC = script_C72D(arg0, arg1, 5, local_F8 / 100.0f, local_F4 / 100.0f, arg3, -1);
    if (local_FC == 1) {
      local_FC = 1;
    } else {
      local_FC = 0;
    }
    if ((Script_ScenDataUtil(task_6C, 4, task_B4[arg2] * 13 + 1) & 4) == 0) {
      script_5F1E(arg0, arg1, arg2, arg3, local_FC, 0);
    } else {
      if (arg0 == task_1CC) {
        script_92F1(arg0, arg1, arg2, arg3, local_FC, 0);
      }
    }
  }
  script_CF98(arg0, arg2, 2);
  return;
}

static void script_47ED(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  int32_t local_F8;
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
    if ((Script_ScenDataUtil(task_6C, 4, task_B4[arg2] * 13 + 1) & 4) == 0) {
      float local_F4;
      float local_F0;
      local_FC = script_C72D(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
      *(int32_t *)&local_F4 = Script_ScenDataUtil(data_1E364, 5, Script_ScenDataUtil(data_1DD74, 1, arg3 * 40 + task_104[arg2] * 4 + local_FC) * 9 + 5);
      *(int32_t *)&local_F0 = Script_ScenDataUtil(data_1E364, 5, Script_ScenDataUtil(data_1DD74, 1, arg3 * 40 + task_104[arg2] * 4 + local_FC) * 9 + 6);
      if ((Script_ScenDataUtil(task_6C, 4, task_B4[arg2] * 13 + 1) & 0x40) | (Script_ScenDataUtil(task_6C, 4, task_B4[arg2] * 13 + 1) & 0x80)) {
        if (Script_ScenDataUtil(task_6C, 4, task_B4[arg2] * 13 + 1) & 0x40) {
          if ((local_FC == 0) | (local_FC == 1)) {
            local_F8 = 6;
          } else {
            local_F8 = 4;
          }
        } else if ((local_FC == 2) | (local_FC == 3)) {
          local_F8 = 6;
        } else {
          local_F8 = 4;
        }
      } else {
        local_F8 = 4;
      }
      local_FC = script_C72D(arg0, arg1, local_F8, local_F4 / 100.0f, local_F0 / 100.0f, arg3, task_B4[arg2]);
      script_5F1E(arg0, arg1, arg2, arg3, local_FC, 1);
    } else {
      if (arg0 == task_1CC) {
        float local_F4;
        float local_F0;
        local_FC = script_C72D(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
        *(int32_t *)&local_F4 = Script_ScenDataUtil(data_1E364, 5, Script_ScenDataUtil(data_1DD74, 1, arg3 * 40 + task_104[arg2] * 4 + local_FC) * 9 + 5);
        *(int32_t *)&local_F0 = Script_ScenDataUtil(data_1E364, 5, Script_ScenDataUtil(data_1DD74, 1, arg3 * 40 + task_104[arg2] * 4 + local_FC) * 9 + 6);
        local_FC = script_C72D(arg0, arg1, 6, local_F4 / 100.0f, local_F0 / 100.0f, arg3, task_B4[arg2]);
        script_92F1(arg0, arg1, arg2, arg3, local_FC, 1);
      }
    }
  }
  script_CF98(arg0, arg2, 2);
  return;
}

static void script_4A8B(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  float local_F8;
  float local_F4;
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
    local_FC = script_C72D(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
    *(int32_t *)&local_F8 = Script_ScenDataUtil(data_1E364, 5, Script_ScenDataUtil(data_1DD74, 1, arg3 * 40 + task_104[arg2] * 4 + local_FC) * 9 + 5);
    *(int32_t *)&local_F4 = Script_ScenDataUtil(data_1E364, 5, Script_ScenDataUtil(data_1DD74, 1, arg3 * 40 + task_104[arg2] * 4 + local_FC) * 9 + 6);
    script_C72D(arg0, arg1, 0, local_F8 / 100.0f, local_F4 / 100.0f, arg3, -1);
  }
  script_CF98(arg0, arg2, 0);
  return;
}

static void script_4B59(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
    local_FC = script_C72D(arg0, arg1, 4, 0.5f, 0.5f, arg3, -1);
    if (local_FC == 1) {
      local_FC = 2;
    } else {
      local_FC = 3;
    }
    script_6E9D(arg0, arg1, arg2, arg3, local_FC, 1);
  }
  script_CF98(arg0, arg2, 1);
  return;
}

static void script_4BC0(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
    local_FC = script_C72D(arg0, arg1, 4, 0.5f, 0.5f, arg3, -1);
    if (local_FC == 1) {
      local_FC = 1;
    } else {
      local_FC = 0;
    }
    script_6E9D(arg0, arg1, arg2, arg3, local_FC, 1);
  }
  script_CF98(arg0, arg2, 1);
  return;
}

static void script_4C27(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
    script_C72D(arg0, arg1, 4, 0.0f, 0.5f, arg3, -1);
    script_6E9D(arg0, arg1, arg2, arg3, 4, 1);
  }
  script_CF98(arg0, arg2, 1);
  return;
}

static void script_4C6E(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  float local_F8;
  float local_F4;
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
    local_FC = script_C72D(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
    if ((local_FC == 1) | (local_FC == 2)) {
      local_FC = 1;
    } else {
      local_FC = 0;
    }
    *(int32_t *)&local_F8 = Script_ScenDataUtil(data_1E364, 5, Script_ScenDataUtil(data_1DD74, 1, arg3 * 40 + task_104[arg2] * 4 + local_FC) * 9 + 5);
    *(int32_t *)&local_F4 = Script_ScenDataUtil(data_1E364, 5, Script_ScenDataUtil(data_1DD74, 1, arg3 * 40 + task_104[arg2] * 4 + local_FC) * 9 + 6);
    if (local_FC == 0) {
      local_FC = script_C72D(arg0, arg1, 5, local_F8 / 100.0f, local_F4 / 100.0f, arg3, -1);
    } else {
      local_FC = script_C72D(arg0, arg1, 5, local_F8 / 100.0f, local_F4 / 100.0f, arg3, -1);
    }
    script_7357(arg0, arg1, arg2, arg3, local_FC, 0);
  }
  script_CF98(arg0, arg2, 2);
  return;
}

static void script_4DA4(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  Vector3i local_F0;
  Vector3i local_E4;
  Vector3i local_D8;
  Vector3 local_CC;
  Vector3 local_C0;
  Vector3 local_B4;
  Vector3 local_A8;
  Vector3 local_9C;
  float local_98;
  float local_94;
  float local_90;
  float local_8C;
  int32_t local_88;
  int32_t local_84;
  int32_t unused_80;
  int8_t local_FF7F;
  uint8_t unused_FF7C[3];
  float local_FF78;
  float local_FF74;
  if (arg0 == task_1CC) {
    if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
      local_FF7F = script_C72D(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
      *(int32_t *)&local_FF78 = Script_ScenDataUtil(data_1E364, 5, Script_ScenDataUtil(data_1DD74, 1, arg3 * 40 + task_104[arg2] * 4 + local_FF7F) * 9 + 5);
      *(int32_t *)&local_FF74 = Script_ScenDataUtil(data_1E364, 5, Script_ScenDataUtil(data_1DD74, 1, arg3 * 40 + task_104[arg2] * 4 + local_FF7F) * 9 + 6);
      local_FF7F = script_C72D(arg0, arg1, 5, local_FF78 / (float)100, local_FF74 / (float)100, arg3, -1);
      script_CDF6(arg0, 0);
      *(int32_t *)&local_94 = Script_ScenDataUtil(data_1E364, 5, Script_ScenDataUtil(data_1DD74, 1, arg3 * 40 + task_104[arg2] * 4 + local_FF7F) * 9 + 1);
      *(int32_t *)&local_90 = Script_ScenDataUtil(data_1E364, 5, Script_ScenDataUtil(data_1DD74, 1, arg3 * 40 + task_104[arg2] * 4 + local_FF7F) * 9 + 2);
      local_8C = 0.0f;
      local_FC = Script_ScenDataUtil(data_1E364, 4, Script_ScenDataUtil(data_1DD74, 1, arg3 * 40 + task_104[arg2] * 4 + local_FF7F) * 9);
      local_FC = local_FC | ((Script_FloatUtils(ScriptMathOp_FloatToInt, local_94 - 3.0f) << 16) & 0x7FFF0000);
      local_9C.x = (&task_1D4)->x;
      (&local_9C)->y = task_1D4.y;
      (&local_9C)->z = task_1D4.z;
      Script_charaPosRotUtils(arg1, 12, &local_CC, &local_E4, 2);
      Script_charaPosRotUtils(arg1, 7, &local_C0, &local_D8, 2);
      Script_ryoControl(PCCTL_StartCameraFacingInterpolation, 0);
      if (arg0 == task_1CC) {
        Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
        Script_scenCharaSubmitMoveTargetRequest(arg0, &task_1D4, *(&task_1E0 + 1), local_FC, 0);
        for (;;) {
          if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0) == 0) {
            break;
          }
          if (Script_IsMoveTargetRequestActive(arg0) == 0) {
            break;
          }
          SCNF_WaitFrames_0000(1);
        }
      } else {
        Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
        Script_SetCharaMoveTarget(arg0, &task_1D4, *(&task_1E0 + 1), local_FC);
        for (;;) {
          if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0) == 0) {
            break;
          }
          if (Script_GetCharaMoveStatus(arg0) == 0) {
            break;
          }
          SCNF_WaitFrames_0000(1);
        }
      }
      for (;;) {
        if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0) == 0) {
          break;
        }
        if (Script_IsCharaMotionFinished(arg0)) {
          break;
        }
        local_98 = Script_GetCharaMotionRemainingFramesScaled(arg0);
        Script_ReadMOTNPositionAtFrame(arg0, local_FC, local_98, &local_A8);
        if (!(local_98 < local_94) & !(local_98 > local_90)) {
          if (local_FF7F == 1) {
            if (!(local_98 < local_94) & !(local_98 >= local_94 + 1.0f)) {
              int32_t unused_FF70;
              int32_t local_FF6C;
              local_FF6C = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_B4[arg2], 2);
              local_FF6C = Script_ScenDataUtil(task_74, 4, Script_ScenDataUtil(task_78, 4, local_FF6C * 9 + 1) * 4 + 2);
              if (local_FF6C != -1) {
                if (arg0 == task_1CC) {
                  Script_executeAicaCommand(local_FF6C, 0, 0);
                } else {
                  Script_Execute3DSoundCommand_Wrapper(local_FF6C, 4, &local_9C, 5.0f, 20.0f, 0);
                }
              }
            }
            Script_EVT_GetPosition(arg0, 12, &local_B4, ENTITY_XFORM_MOTION_SPACE);
            (&local_B4)->z = (&local_B4)->z + (&local_A8)->z;
            (&local_F0)->y = Script_FloatUtils(ScriptMathOp_Atan2ReverseVector, (&local_C0)->x - (&local_B4)->x, (&local_C0)->z - (&local_B4)->z);
            (&local_F0)->y = (&local_F0)->y - (int32_t)((local_8C + 95.0f) / 360.0f * (float)0x10000);
            local_F0.x = (&local_D8)->x;
            (&local_F0)->z = (&local_D8)->z;
            Script_charaPosRotUtils(arg1, 7, 0, &local_F0, 0);
            Script_EVT_GetPosition(arg0, 18, &local_B4, ENTITY_XFORM_MOTION_SPACE);
            (&local_B4)->z = (&local_B4)->z + (&local_A8)->z;
            (&local_F0)->y = Script_FloatUtils(ScriptMathOp_Atan2ReverseVector, (&local_CC)->x - (&local_B4)->x, (&local_CC)->z - (&local_B4)->z);
            (&local_F0)->y = (&local_F0)->y + (int32_t)((local_8C + 95.0f) / 360.0f * (float)0x10000);
            local_F0.x = (&local_E4)->x;
            (&local_F0)->z = (&local_E4)->z;
            Script_charaPosRotUtils(arg1, 12, 0, &local_F0, 0);
          } else {
            if (!(local_98 < local_94) & !(local_98 >= local_94 + 1.0f)) {
              int32_t unused_FF70;
              int32_t local_FF6C;
              local_FF6C = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_B4[arg2], 2);
              local_FF6C = Script_ScenDataUtil(task_74, 4, Script_ScenDataUtil(task_78, 4, local_FF6C * 9 + 1) * 4 + 2);
              if (local_FF6C != -1) {
                if (arg0 == task_1CC) {
                  Script_executeAicaCommand(local_FF6C, 0, 0);
                } else {
                  Script_Execute3DSoundCommand_Wrapper(local_FF6C, 4, &local_9C, 5.0f, 20.0f, 0);
                }
              }
            }
            Script_EVT_GetPosition(arg0, 12, &local_B4, ENTITY_XFORM_MOTION_SPACE);
            (&local_B4)->z = (&local_B4)->z + (&local_A8)->z;
            (&local_F0)->y = Script_FloatUtils(ScriptMathOp_Atan2ReverseVector, (&local_C0)->x - (&local_B4)->x, (&local_C0)->z - (&local_B4)->z);
            (&local_F0)->y = -((&local_F0)->y - (int32_t)((local_8C + 95.0f) / 360.0f * (float)0x10000));
            local_F0.x = (&local_D8)->x;
            (&local_F0)->z = (&local_D8)->z;
            Script_charaPosRotUtils(arg1, 7, 0, &local_F0, 0);
            Script_EVT_GetPosition(arg0, 18, &local_B4, ENTITY_XFORM_MOTION_SPACE);
            (&local_B4)->z = (&local_B4)->z + (&local_A8)->z;
            (&local_F0)->y = Script_FloatUtils(ScriptMathOp_Atan2ReverseVector, (&local_CC)->x - (&local_B4)->x, (&local_CC)->z - (&local_B4)->z);
            (&local_F0)->y = -((&local_F0)->y + (int32_t)((local_8C + 95.0f) / 360.0f * (float)0x10000));
            local_F0.x = (&local_E4)->x;
            (&local_F0)->z = (&local_E4)->z;
            Script_charaPosRotUtils(arg1, 12, 0, &local_F0, 0);
          }
        }
        if (!(local_98 < local_90 + 10.0f)) {
          break;
        }
        SCNF_WaitFrames_0000(1);
      }
      script_CDF6(arg0, 1);
      local_8C = Script_MathUtils(SCRIPT_MATH_INT_TO_FLOAT, (&local_F0)->y);
      local_8C = local_8C / 182.044449f;
      if (!(local_8C <= 180.0f)) {
        local_8C = 360.0f - local_8C;
      }
      if (!(local_8C >= -180.0f)) {
        local_8C = local_8C + 360.0f;
      }
      local_88 = 0x4000;
      local_84 = 0;
      local_90 = 1.0f;
      local_FC = 0;
      {
        int32_t unused_FF70;
        int32_t local_FF6C;
        local_FF6C = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_B4[arg2], 2);
        local_FF6C = Script_ScenDataUtil(task_74, 4, Script_ScenDataUtil(task_78, 4, local_FF6C * 9 + 1) * 4 + 3);
        if (local_FF6C != -1) {
          if (arg0 == task_1CC) {
            Script_executeAicaCommand(local_FF6C, 0, 0);
          } else {
            Script_Execute3DSoundCommand_Wrapper(local_FF6C, 4, &local_9C, 5.0f, 20.0f, 0);
          }
        }
      }
      for (;;) {
        local_94 = local_8C * Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, local_88) * local_90;
        local_90 = Script_MathUtils(SCRIPT_MATH_COS_FIXED16, local_84);
        if (local_FC == 0) {
          local_88 = local_88 + 0x200;
          if (local_88 >= 0xC000) {
            local_FC = 1;
          }
        } else {
          local_88 = local_88 - 0x200;
          if (local_88 <= 0x4000) {
            local_FC = 0;
          }
        }
        local_84 = local_84 + 128;
        if (local_FF7F == 1) {
          (&local_F0)->y = -(int32_t)(((float)0 - local_94) / 360.0f * (float)0x10000);
          Script_charaPosRotUtils(arg1, 7, 0, &local_F0, 0);
          (&local_F0)->y = (int32_t)(((float)0 - local_94) / 360.0f * (float)0x10000);
          Script_charaPosRotUtils(arg1, 12, 0, &local_F0, 0);
        } else {
          (&local_F0)->y = (int32_t)(((float)0 - local_94) / 360.0f * (float)0x10000);
          Script_charaPosRotUtils(arg1, 7, 0, &local_F0, 0);
          (&local_F0)->y = -(int32_t)(((float)0 - local_94) / 360.0f * (float)0x10000);
          Script_charaPosRotUtils(arg1, 12, 0, &local_F0, 0);
        }
        SCNF_WaitFrames_0000(1);
        if (local_84 >= 0x4000) {
          break;
        }
      }
    }
    script_CF98(arg0, arg2, 0);
  } else {
    script_4389(arg0, arg1, arg2, arg3);
  }
  return;
}

static void script_5665(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  float local_F8;
  int32_t local_F4;
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
    local_FC = script_C72D(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
    *(int32_t *)&local_F8 = Script_ScenDataUtil(data_1E364, 5, Script_ScenDataUtil(data_1DD74, 1, arg3 * 40 + task_104[arg2] * 4 + local_FC) * 9 + 5);
    local_F8 = 98.0f - local_F8;
    local_F4 = Script_ScenDataUtil(data_1E364, 5, Script_ScenDataUtil(data_1DD74, 1, arg3 * 40 + task_104[arg2] * 4 + local_FC) * 9 + 6);
    if ((local_FC == 0) | (local_FC == 3)) {
      local_FC = script_C72D(arg0, arg1, 4, 0.34736f, 0.73184f, arg3, -1);
    } else {
      local_FC = script_C72D(arg0, arg1, 4, 0.4f, 0.712f, arg3, -1);
    }
    script_9A12(arg0, arg1, arg2, arg3, local_FC, 0);
  }
  script_CF98(arg0, arg2, 0);
  return;
}

static int32_t script_5783(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  int32_t local_F0;
  Vector3 local_E4;
  Vector3i local_D8;
  float local_D4;
  float local_D0;
  float local_CC;
  int32_t unused_C8;
  GameDateTime local_C0;
  int32_t local_BC;
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
    local_F4 = Script_ScenDataUtil(task_6C, 4, task_B4[arg2] * 13 + 6);
    if ((local_F4 == 6) | (local_F4 == 7)) {
      Script_ReturnTimeDate(&local_C0);
      if (((&local_C0)->hours >= 22) | ((&local_C0)->hours <= 4)) {
        local_F0 = 1;
        local_F4 = 2;
      } else {
        local_F0 = 0;
      }
    } else {
      local_F0 = 0;
    }
    local_BC = local_F4;
    if (local_BC == 2) {
      local_BC = arg3;
      if ((local_BC == 17) | (local_BC == 11) | (local_BC == 12)) {
      } else {
        local_FC = script_C72D(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
        local_FC = script_C72D(arg0, arg1, 7, 0.45f, 0.0f, arg3, -1);
        local_BC = arg3;
        if ((local_BC == 2) | (local_BC == 4) | (local_BC == 6) | (local_BC == 8) | (local_BC == 10) | (local_BC == 12) | (local_BC == 24) | (local_BC == 26) | (local_BC == 31) | (local_BC == 33) | (local_BC == 35)) {
          local_D4 = -0.45f;
        } else if ((local_BC == 13) | (local_BC == 14) | (local_BC == 15) | (local_BC == 16) | (local_BC == 17) | (local_BC == 18) | (local_BC == 19) | (local_BC == 20) | (local_BC == 21) | (local_BC == 22) | (local_BC == 23) | (local_BC == 29)) {
          local_BC = local_FC;
          if ((local_BC == 0) | (local_BC == 1)) {
            local_D4 = -0.45f;
          } else {
            if ((local_BC == 2) | (local_BC == 3)) {
              local_D4 = 0.45f;
              goto L00593F;
            }
          }
          L00593F:;
        } else {
          local_D4 = 0.45f;
        }
        Script_EVT_GetPosition(arg1, -1, &local_E4, 0);
        Script_mtwkGetSetRotation(arg1, &local_D8, 0);
        local_E4.x = (&local_E4)->x + local_D4 * Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_D8)->y & 0xFFFF);
        (&local_E4)->z = (&local_E4)->z - local_D4 * Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_D8)->y & 0xFFFF);
        *(&task_1E0 + 1) = Script_FloatUtils(ScriptMathOp_Atan2ReverseVector, (&local_E4)->x - (&task_1D4)->x, (&local_E4)->z - task_1D4.z);
        script_A19C(arg0, arg1, arg2, arg3, local_FC, local_F0);
      }
    } else if ((local_BC == 3) | (local_BC == 4) | (local_BC == 5) | (local_BC == 6)) {
      *(int32_t *)&local_D0 = Script_ScenDataUtil(&data_1ED84, 5, Script_ScenDataUtil(task_6C, 4, task_B4[arg2] * 13 + 6) * 5 + 3);
      *(int32_t *)&local_CC = Script_ScenDataUtil(&data_1ED84, 5, Script_ScenDataUtil(task_6C, 4, task_B4[arg2] * 13 + 6) * 5 + 4);
      local_FC = script_C72D(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
      local_BC = arg3;
      if ((local_BC == 13) | (local_BC == 14) | (local_BC == 15) | (local_BC == 16) | (local_BC == 20) | (local_BC == 23) | (local_BC == 28) | (local_BC == 29)) {
        if ((Script_ScenDataUtil(task_6C, 4, task_B4[arg2] * 13 + 1) & 0x40) | (Script_ScenDataUtil(task_6C, 4, task_B4[arg2] * 13 + 1) & 0x80)) {
          if (Script_ScenDataUtil(task_6C, 4, task_B4[arg2] * 13 + 1) & 0x40) {
            if ((local_FC == 0) | (local_FC == 1)) {
              local_F8 = 6;
            } else {
              local_F8 = 4;
            }
          } else if ((local_FC == 2) | (local_FC == 3)) {
            local_F8 = 6;
          } else {
            local_F8 = 4;
          }
        } else {
          local_F8 = 4;
        }
      } else {
        local_F8 = 5;
      }
      local_FC = script_C72D(arg0, arg1, local_F8, local_D0 / (float)100, local_CC / (float)100, arg3, task_B4[arg2]);
      script_A274(arg0, arg1, arg2, arg3, local_FC, 0);
    } else {
      if (local_BC == 7) {
        local_FC = script_C72D(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
        local_BC = arg3;
        if ((local_BC == 13) | (local_BC == 14) | (local_BC == 15) | (local_BC == 16) | (local_BC == 20) | (local_BC == 23) | (local_BC == 28) | (local_BC == 29)) {
          if ((Script_ScenDataUtil(task_6C, 4, task_B4[arg2] * 13 + 1) & 0x40) | (Script_ScenDataUtil(task_6C, 4, task_B4[arg2] * 13 + 1) & 0x80)) {
            if (Script_ScenDataUtil(task_6C, 4, task_B4[arg2] * 13 + 1) & 0x40) {
              if ((local_FC == 0) | (local_FC == 1)) {
                local_F8 = 6;
              } else {
                local_F8 = 4;
              }
            } else if ((local_FC == 2) | (local_FC == 3)) {
              local_F8 = 6;
            } else {
              local_F8 = 4;
            }
          } else {
            local_F8 = 4;
          }
        } else {
          local_F8 = 5;
        }
        if (Script_CycleManagerUtils(CYCLE_CMD_IS_DOOR_OR_MOBJ_TYPE2, Script_ScenDataUtil(task_6C, 4, task_B4[arg2] * 13 + 12)) == 1) {
          *(int32_t *)&local_D0 = Script_ScenDataUtil(&data_1ED84, 5, Script_ScenDataUtil(task_6C, 4, task_B4[arg2] * 13 + 6) * 5 + 3);
          *(int32_t *)&local_CC = Script_ScenDataUtil(&data_1ED84, 5, Script_ScenDataUtil(task_6C, 4, task_B4[arg2] * 13 + 6) * 5 + 4);
          local_FC = script_C72D(arg0, arg1, local_F8, local_D0 / (float)100, local_CC / (float)100, arg3, task_B4[arg2]);
          Script_scenUtils(SCENARIO_CMD_INIT_SCNT);
          script_A6EB(arg0, arg1, arg2, arg3, local_FC, 0);
          Script_scenUtils(SCENARIO_CMD_UPDATE_TIMER);
        } else {
          *(int32_t *)&local_D0 = Script_ScenDataUtil(&data_1ED84, 5, Script_ScenDataUtil(task_6C, 4, task_B4[arg2] * 13 + 6) * 5 + 3);
          *(int32_t *)&local_CC = Script_ScenDataUtil(&data_1ED84, 5, Script_ScenDataUtil(task_6C, 4, task_B4[arg2] * 13 + 6) * 5 + 4);
          local_FC = script_C72D(arg0, arg1, local_F8, local_D0 / (float)100, local_CC / (float)100, arg3, task_B4[arg2]);
          script_A274(arg0, arg1, arg2, arg3, local_FC, 0);
        }
        goto L005E3A;
      }
    }
  }
  L005E3A:;
  script_CF98(arg0, arg2, 0);
  end:;
}

static void script_5E48(int32_t arg0, int32_t arg1) {
  Vector3 local_F4;
  Vector3i local_E8;
  Script_EVT_GetPosition(arg0, -1, &local_F4, 0);
  Script_mtwkGetSetRotation(arg0, &local_E8, 0x42000000);
  if ((arg1 == 5) | (arg1 == 6)) {
    local_F4.x = (&local_F4)->x - Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_E8)->y) * 0.4f;
    (&local_F4)->z = (&local_F4)->z - Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_E8)->y) * 0.4f;
  } else {
    local_F4.x = (&local_F4)->x - Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_E8)->y) * 0.3f;
    (&local_F4)->z = (&local_F4)->z - Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_E8)->y) * 0.3f;
  }
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
    Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
    Script_SetCharaMoveTarget(arg0, &local_F4, (&local_E8)->y, script_41A4(arg1));
  }
  return;
}

__attribute__((scn_string_bindings(0, data_1F8A2)))
static void script_5F1E(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
  int8_t local_FF;
  uint8_t unused_FC[3];
  Vector3 local_F0;
  Vector3 local_E4;
  Vector3 local_D8;
  Vector3 local_CC;
  Vector3 local_C0;
  float local_BC;
  float local_B8;
  float local_B4;
  float local_B0;
  float local_AC;
  int32_t local_A8;
  float local_A4;
  int32_t local_A0;
  float local_9C;
  float local_98;
  float local_94;
  float local_90;
  float local_8C;
  float local_88;
  int16_t local_86;
  int16_t local_84;
  int32_t local_80;
  int32_t local_FF7C;
  *(int32_t *)&local_98 = Script_ScenDataUtil(data_1E364, 5, Script_ScenDataUtil(data_1DD74, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9 + 1);
  *(int32_t *)&local_94 = Script_ScenDataUtil(data_1E364, 5, Script_ScenDataUtil(data_1DD74, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9 + 2);
  *(int32_t *)&local_90 = Script_ScenDataUtil(data_1E364, 5, Script_ScenDataUtil(data_1DD74, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9 + 3);
  *(int32_t *)&local_8C = Script_ScenDataUtil(data_1E364, 5, Script_ScenDataUtil(data_1DD74, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9 + 4);
  *(int32_t *)&local_88 = Script_ScenDataUtil(data_1E364, 5, Script_ScenDataUtil(data_1DD74, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9 + 7);
  if (arg5) {
    Script_charaPosRotUtils(arg1, 12, &local_D8, 0, 2);
    Script_charaPosRotUtils(arg1, 7, &local_CC, 0, 2);
  } else {
    Script_EVT_GetPosition(arg1, -1, &local_D8, 0);
    Script_EVT_GetPosition(arg1, -1, &local_CC, 0);
  }
  local_F0.x = 0.0f;
  (&local_F0)->y = 0.0f;
  (&local_F0)->z = 0.0f;
  local_E4.x = 0.0f;
  (&local_E4)->y = 0.0f;
  (&local_E4)->z = 0.0f;
  local_BC = 0.0f;
  local_86 = 0;
  {
    int32_t local_FF78;
    int32_t local_FF74;
    Vector3i local_FF68;
    local_FF74 = 0;
    local_A0 = 0x3F800000;
    local_B4 = 0.0f;
    local_B0 = 0.0f;
    local_AC = 0.0f;
    local_A8 = 0;
    Script_mtwkGetSetRotation(arg1, &local_FF68, 0);
    local_84 = 0;
    if ((arg4 == 1) | (arg4 == 3)) {
      local_FF78 = Script_ScenDataUtil(data_1E364, 4, Script_ScenDataUtil(data_1DD74, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9);
      local_FF7C = 18;
      local_80 = 18;
      local_A4 = 1.0f;
    } else {
      if (arg0 == task_1CC) {
        local_FF78 = Script_ScenDataUtil(data_1E364, 4, Script_ScenDataUtil(data_1DD74, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9) | -0x80000000;
        local_FF7C = 12;
        local_80 = 12;
      } else {
        local_FF78 = Script_ScenDataUtil(data_1E364, 4, Script_ScenDataUtil(data_1DD74, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9);
        local_FF7C = 18;
        local_80 = 18;
      }
      local_A4 = -1.0f;
    }
    script_3BF2(arg0, local_FF78, arg2, arg3, arg4);
    local_FF78 = local_FF78 | ((Script_FloatUtils(ScriptMathOp_FloatToInt, local_98 - 3.0f) << 16) & 0x7FFF0000);
    script_CDF6(arg0, 0);
    local_C0.x = (&task_1D4)->x;
    (&local_C0)->y = task_1D4.y;
    (&local_C0)->z = task_1D4.z;
    Script_ryoControl(PCCTL_StartCameraFacingInterpolation, 0);
    Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
    Script_SetCharaMoveTarget(arg0, &task_1D4, *(&task_1E0 + 1), local_FF78);
    for (;;) {
      if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0) == 0) {
        break;
      }
      if (Script_GetCharaMoveStatus(arg0) == 0) {
        break;
      }
      SCNF_WaitFrames_0000(1);
    }
    local_FF78 = local_FF78 & 0xFFFF;
    if (task_1CC == arg0) {
      Script_SetCharaMotionPlaybackRate(arg0, 1.3f);
      local_FF = 0;
    } else {
      local_FF = 1;
    }
    if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
      local_B8 = Script_GetCharaMotionRemainingFramesScaled(arg0);
    }
    for (;;) {
      if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0) == 0) {
        Script_sprintf("This Motion don't Finish!!\012");
        break;
      }
      if (local_FF) {
        if (!(local_B8 <= local_90)) {
          local_FF = 0;
          script_5E48(arg0, task_104[arg2]);
        }
      }
      Script_ReadMOTNPositionAtFrame(arg0, local_FF78, local_B8, &local_E4);
      local_E4.x = (&local_E4)->x * local_A4;
      if (!(local_B8 >= local_98)) {
        local_86 = 0;
      }
      if (!(local_B8 < local_98) & !(local_B8 > local_94)) {
        if (!(local_B8 < local_98) & !(local_B8 >= local_98 + 1.5f)) {
          if (local_84 == 0) {
            int32_t local_FF64;
            local_FF64 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_B4[arg2], 2);
            local_FF64 = Script_ScenDataUtil(task_74, 4, Script_ScenDataUtil(task_78, 4, local_FF64 * 9 + 1) * 4 + 2);
            if (local_FF64 != -1) {
              if (arg0 == task_1CC) {
                Script_executeAicaCommand(local_FF64, 0, 0);
              } else {
                Script_Execute3DSoundCommand_Wrapper(local_FF64, 4, &local_C0, 5.0f, 20.0f, 0);
              }
            }
            local_84 = 1;
          }
        }
        Script_EVT_GetPosition(arg0, local_80, &local_F0, ENTITY_XFORM_MOTION_SPACE);
        if (local_86 == 1) {
          local_B0 = (&local_E4)->x;
          local_BC = (&local_F0)->x;
        }
        local_B4 = (&local_E4)->x + ((&local_F0)->x - local_BC);
        local_B4 = local_B4 - local_B0;
        local_B4 = local_B4 * 1.28f;
        {
          int32_t local_FF64;
          local_FF64 = arg4;
          if ((local_FF64 == 0) | (local_FF64 == 2)) {
            if (!(local_B4 < 0.0f)) {
              local_B4 = 0.0f;
            } else {
              if (!(local_B4 > -0.75f)) {
                local_B4 = -0.75f;
                goto L0064A6;
              }
            }
            L0064A6:;
          } else {
            if ((local_FF64 == 1) | (local_FF64 == 3)) {
              if (!(local_B4 > 0.0f)) {
                local_B4 = 0.0f;
              } else {
                if (!(local_B4 < 0.75f)) {
                  local_B4 = 0.75f;
                  goto L0064E4;
                }
              }
              L0064E4:;
              goto L0064E7;
            }
          }
        }
      }
      L0064E7:;
      if (task_1CC == arg0) {
        if (!(local_B8 <= local_94) & !(local_B8 >= local_90)) {
          local_86 = 0;
        }
      } else {
        if (!(local_B8 <= local_94) & !(local_B8 > local_88)) {
          *(int32_t *)&local_9C = Script_ScenDataUtil(data_1E364, 5, Script_ScenDataUtil(data_1DD74, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9 + 8);
          local_B4 = local_B4 + local_9C * local_A4;
        }
      }
      if (!(local_B8 < local_90) & !(local_B8 >= local_90 + 1.5f)) {
        if (local_84 == 1) {
          int32_t local_FF64;
          local_FF64 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_B4[arg2], 2);
          local_FF64 = Script_ScenDataUtil(task_74, 4, Script_ScenDataUtil(task_78, 4, local_FF64 * 9 + 1) * 4 + 3);
          if (local_FF64 != -1) {
            if (arg0 == task_1CC) {
              Script_executeAicaCommand(local_FF64, 0, 0);
            } else {
              Script_Execute3DSoundCommand_Wrapper(local_FF64, 4, &local_C0, 5.0f, 20.0f, 0);
            }
          }
          local_84 = 2;
        }
        if (task_1CC != arg0) {
          if (local_FF74 == 0) {
            local_AC = local_B4 / (local_8C - local_90);
            local_FF74 = 1;
          }
        }
      }
      if (!(local_B8 < local_90) & !(local_B8 > local_8C)) {
        Script_EVT_GetPosition(arg0, local_FF7C, &local_F0, ENTITY_XFORM_MOTION_SPACE);
        if (task_1CC == arg0) {
          int32_t local_FF64;
          local_B4 = (&local_E4)->x + ((&local_F0)->x - local_BC);
          local_B4 = local_B4 - local_B0;
          local_B4 = local_B4 * 1.28f;
          local_FF64 = arg4;
          if ((local_FF64 == 0) | (local_FF64 == 2)) {
            if (!(local_B4 > -0.75f)) {
              local_B4 = -0.75f;
            } else {
              if (!(local_B4 < 0.0f)) {
                local_B4 = 0.0f;
                goto L0066B3;
              }
            }
            L0066B3:;
          } else {
            if ((local_FF64 == 1) | (local_FF64 == 3)) {
              if (!(local_B4 < 0.75f)) {
                local_B4 = 0.75f;
              } else {
                if (!(local_B4 > 0.0f)) {
                  local_B4 = 0.0f;
                  goto L0066F1;
                }
              }
              L0066F1:;
              goto L0066F4;
            }
          }
          L0066F4:;
        } else {
          int32_t local_FF64;
          local_B4 = local_B4 - local_AC;
          local_FF64 = arg4;
          if ((local_FF64 == 0) | (local_FF64 == 2)) {
            if (!(local_B4 < 0.0f)) {
              local_B4 = 0.0f;
            }
          } else {
            if ((local_FF64 == 1) | (local_FF64 == 3)) {
              if (!(local_B4 > 0.0f)) {
                local_B4 = 0.0f;
              }
              goto L00674D;
            }
          }
        }
      }
      L00674D:;
      if (!(local_8C <= 0.0f) & !(local_B8 <= local_8C)) {
        local_B4 = local_B4 - 0.01f;
        if (!(local_B4 > 0.0f)) {
          local_B4 = 0.0f;
        }
      }
      {
        int32_t local_FF64;
        local_FF64 = arg4;
        switch (local_FF64) {
          case 0:
            local_F0.x = (&local_CC)->x;
            (&local_F0)->y = (&local_CC)->y;
            (&local_F0)->z = (&local_CC)->z;
            if (arg5) {
              local_F0.x = (&local_CC)->x - local_B4;
              Script_charaPosRotUtils(arg1, 7, &local_F0, 0, 0);
            } else {
              local_F0.x = (&local_CC)->x + ((float)0 - local_B4) * Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_FF68)->y);
              (&local_F0)->z = (&local_CC)->z + local_B4 * Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_FF68)->y);
              Script_EVT_SetPosition(arg1, POS_XYZ, &local_F0);
            }
            break;
          case 1:
            local_F0.x = (&local_CC)->x;
            (&local_F0)->y = (&local_CC)->y;
            (&local_F0)->z = (&local_CC)->z;
            if (arg5) {
              local_F0.x = (&local_CC)->x + local_B4;
              Script_charaPosRotUtils(arg1, 7, &local_F0, 0, 0);
            } else {
              local_F0.x = (&local_CC)->x - ((float)0 - local_B4) * Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_FF68)->y);
              (&local_F0)->z = (&local_CC)->z - local_B4 * Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_FF68)->y);
              Script_EVT_SetPosition(arg1, POS_XYZ, &local_F0);
            }
            break;
          case 2:
            local_F0.x = (&local_D8)->x;
            (&local_F0)->y = (&local_D8)->y;
            (&local_F0)->z = (&local_D8)->z;
            if (arg5) {
              local_F0.x = (&local_D8)->x + local_B4;
              Script_charaPosRotUtils(arg1, 12, &local_F0, 0, 0);
            } else {
              local_F0.x = (&local_D8)->x - ((float)0 - local_B4) * Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_FF68)->y);
              (&local_F0)->z = (&local_D8)->z - local_B4 * Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_FF68)->y);
              Script_EVT_SetPosition(arg1, POS_XYZ, &local_F0);
            }
            break;
          case 3:
            local_F0.x = (&local_D8)->x;
            (&local_F0)->y = (&local_D8)->y;
            (&local_F0)->z = (&local_D8)->z;
            if (arg5) {
              local_F0.x = (&local_D8)->x - local_B4;
              Script_charaPosRotUtils(arg1, 12, &local_F0, 0, 0);
            } else {
              local_F0.x = (&local_D8)->x + ((float)0 - local_B4) * Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_FF68)->y);
              (&local_F0)->z = (&local_D8)->z + local_B4 * Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_FF68)->y);
              Script_EVT_SetPosition(arg1, POS_XYZ, &local_F0);
            }
            break;
        }
      }
      local_86 = local_86 + 1;
      if (arg0 == task_1CC) {
        local_B8 = Script_GetCharaMotionRemainingFramesScaled(arg0);
        if (Script_IsCharaMotionFinished(arg0)) {
          break;
        }
      } else {
        if (!(local_B8 <= local_8C)) {
          break;
        }
        local_B8 = local_B8 + 1.0f;
      }
      SCNF_WaitFrames_0000(1);
    }
  }
  script_CDF6(arg0, 1);
  {
    int32_t local_FF78;
    int32_t unused_FF68[4];
    int32_t local_FF64;
    local_FF78 = task_B4[arg2];
    local_FF64 = arg3;
    if ((local_FF64 == 9) | (local_FF64 == 10) | (local_FF64 == 15)) {
      if (arg5) {
        Script_charaPosRotUtils(arg1, 12, &local_D8, 0, 0);
        Script_charaPosRotUtils(arg1, 7, &local_CC, 0, 0);
      } else {
        Script_EVT_SetPosition(arg1, POS_XYZ, &local_D8);
      }
      if (task_1CC == arg0) {
        local_80 = (Script_ScenDataUtil(task_6C, 4, local_FF78 * 13 + 2) >> 16) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_80, 1);
        local_80 = Script_ScenDataUtil(task_6C, 4, local_FF78 * 13 + 2) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_80, 1);
      }
    } else if ((local_FF64 == 18) | (local_FF64 == 19) | (local_FF64 == 21) | (local_FF64 == 22)) {
      if (task_1CC == arg0) {
        local_80 = (Script_ScenDataUtil(task_6C, 4, local_FF78 * 13 + 2) >> 16) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_80, 1);
        {
          int32_t local_FF60;
          local_FF60 = Script_ScenDataUtil(task_6C, 4, local_FF78 * 13 + 1);
          local_FF60 = local_FF60 | 8;
          Script_ScenDataUtil(task_6C, 14, local_FF78 * 13 + 1, local_FF60);
        }
        {
          int32_t local_FF60;
          local_FF60 = Script_ScenDataUtil(task_6C, 4, local_FF78 * 13 + 1);
          local_FF60 = local_FF60 | 0x10;
          Script_ScenDataUtil(task_6C, 14, local_FF78 * 13 + 1, local_FF60);
        }
        {
          int32_t local_FF60;
          local_FF60 = Script_ScenDataUtil(task_6C, 4, local_FF78 * 13 + 1);
          local_FF60 = local_FF60 | 2;
          Script_ScenDataUtil(task_6C, 14, local_FF78 * 13 + 1, local_FF60);
        }
      } else {
        if (arg5) {
          Script_charaPosRotUtils(arg1, 12, &local_D8, 0, 0);
          Script_charaPosRotUtils(arg1, 7, &local_CC, 0, 0);
        } else {
          Script_EVT_SetPosition(arg1, POS_XYZ, &local_D8);
        }
        local_80 = (Script_ScenDataUtil(task_6C, 4, local_FF78 * 13 + 2) >> 16) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_80, 1);
        local_80 = Script_ScenDataUtil(task_6C, 4, local_FF78 * 13 + 2) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_80, 1);
      }
    } else {
      if ((local_FF64 == 20) | (local_FF64 == 23)) {
        if (task_1CC == arg0) {
          if ((arg4 == 0) | (arg4 == 1)) {
            {
              int32_t local_FF60;
              local_FF60 = Script_ScenDataUtil(task_6C, 4, local_FF78 * 13 + 1);
              local_FF60 = local_FF60 | 0x10;
              Script_ScenDataUtil(task_6C, 14, local_FF78 * 13 + 1, local_FF60);
            }
            {
              int32_t local_FF60;
              local_FF60 = Script_ScenDataUtil(task_6C, 4, local_FF78 * 13 + 1);
              local_FF60 = local_FF60 | 4;
              Script_ScenDataUtil(task_6C, 14, local_FF78 * 13 + 1, local_FF60);
            }
          } else {
            {
              int32_t local_FF60;
              local_FF60 = Script_ScenDataUtil(task_6C, 4, local_FF78 * 13 + 1);
              local_FF60 = local_FF60 | 8;
              Script_ScenDataUtil(task_6C, 14, local_FF78 * 13 + 1, local_FF60);
            }
            {
              int32_t local_FF60;
              local_FF60 = Script_ScenDataUtil(task_6C, 4, local_FF78 * 13 + 1);
              local_FF60 = local_FF60 | 4;
              Script_ScenDataUtil(task_6C, 14, local_FF78 * 13 + 1, local_FF60);
            }
          }
        } else {
          if (arg5) {
            Script_charaPosRotUtils(arg1, 12, &local_D8, 0, 0);
            Script_charaPosRotUtils(arg1, 7, &local_CC, 0, 0);
          } else {
            Script_EVT_SetPosition(arg1, POS_XYZ, &local_D8);
          }
          local_80 = (Script_ScenDataUtil(task_6C, 4, local_FF78 * 13 + 2) >> 16) & 0xFFFF;
          Script_scenToggleCharaFlag(0, local_80, 1);
          local_80 = Script_ScenDataUtil(task_6C, 4, local_FF78 * 13 + 2) & 0xFFFF;
          Script_scenToggleCharaFlag(0, local_80, 1);
        }
        goto L006E30;
      }
    }
  }
  L006E30:;
  return;
}

static void script_6E31(int32_t arg0, int32_t arg1, int32_t arg2) {
  int32_t local_FC;
  if (task_1CC == arg0) {
    local_FC = task_B4[arg1];
    {
      int32_t local_F8;
      local_F8 = Script_ScenDataUtil(task_6C, 4, local_FC * 13 + 2) & 0xFFFF;
      Script_scenToggleCharaFlag(0, local_F8, arg2);
    }
    {
      int32_t local_F8;
      local_F8 = (Script_ScenDataUtil(task_6C, 4, local_FC * 13 + 2) >> 16) & 0xFFFF;
      Script_scenToggleCharaFlag(0, local_F8, arg2);
    }
  }
  return;
}

static void script_6E9D(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
  Vector3 local_F4;
  Vector3 local_E8;
  Vector3 local_DC;
  Vector3 local_D0;
  Vector3 local_C4;
  Vector3 local_B8;
  Vector3 local_AC;
  Vector3i local_A0;
  int32_t local_9C;
  int32_t local_98;
  float local_94;
  int8_t local_93;
  int8_t local_92;
  uint8_t unused_90[2];
  int32_t local_8C;
  int32_t local_88;
  int32_t local_84;
  if (arg4 == 4) {
    Script_charaPosRotUtils(arg1, 12, &local_C4, 0, 2);
    Script_charaPosRotUtils(arg1, 7, &local_B8, 0, 2);
  } else {
    Script_EVT_GetPosition(arg1, -1, &local_C4, 0);
    Script_EVT_GetPosition(arg1, -1, &local_B8, 0);
  }
  local_AC.x = (&task_1D4)->x;
  (&local_AC)->y = task_1D4.y;
  (&local_AC)->z = task_1D4.z;
  local_DC.x = (&local_C4)->x;
  (&local_DC)->y = (&local_C4)->y;
  (&local_DC)->z = (&local_C4)->z;
  local_D0.x = (&local_B8)->x;
  (&local_D0)->y = (&local_B8)->y;
  (&local_D0)->z = (&local_B8)->z;
  local_9C = 0xC000;
  local_93 = 0;
  local_98 = 0x300;
  Script_EVT_GetPosition(arg1, -1, &local_F4, 0);
  Script_mtwkGetSetRotation(arg1, &local_A0, 0);
  {
    int32_t local_80;
    local_80 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_B4[arg2], 2);
    local_80 = Script_ScenDataUtil(task_74, 4, Script_ScenDataUtil(task_78, 4, local_80 * 9 + 1) * 4 + 2);
    if (local_80 != -1) {
      if (arg0 == task_1CC) {
        Script_executeAicaCommand(local_80, 0, 0);
      } else {
        Script_Execute3DSoundCommand_Wrapper(local_80, 4, &local_AC, 5.0f, 20.0f, 0);
      }
    }
  }
  local_88 = 0;
  local_8C = 0;
  local_84 = 0;
  for (;;) {
    local_92 = 0;
    if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
      Script_EVT_GetPosition(arg0, -1, &local_E8, 0);
      if (Script_CheckTargetWithinAngleRange(&local_F4, 1.5f, (&local_A0)->y, 0x10000, &local_E8)) {
        local_92 = 1;
      }
    }
    if (local_92) {
      if (local_8C) {
        task_DC[arg2] = arg0;
        local_98 = -local_98;
        local_84 = 0;
      }
      local_8C = 0;
    } else {
      if ((local_93 == 1) & (local_8C == 0)) {
        int32_t local_80;
        local_80 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_B4[arg2], 2);
        local_80 = Script_ScenDataUtil(task_74, 4, Script_ScenDataUtil(task_78, 4, local_80 * 9 + 1) * 4 + 3);
        if (local_80 != -1) {
          if (arg0 == task_1CC) {
            Script_executeAicaCommand(local_80, 0, 0);
          } else {
            Script_Execute3DSoundCommand_Wrapper(local_80, 4, &local_AC, 5.0f, 20.0f, 0);
          }
        }
        local_93 = 0;
        local_8C = 1;
        local_98 = -local_98;
        local_84 = 0;
        task_DC[arg2] = 0;
      }
    }
    local_88 = task_B4[arg2];
    local_88 = Script_ScenDataUtil(task_6C, 4, local_88 * 13 + 2) & 0xFFFF;
    local_88 = local_88 + ((local_9C - 0x4000) >> 12);
    if (local_84 != local_88) {
      Script_scenToggleCharaFlag(0, local_88, local_8C);
      local_84 = local_88;
    }
    local_9C = local_9C - local_98;
    if (local_9C >= 0xC000) {
      local_9C = 0xC000;
      local_88 = task_B4[arg2];
      local_88 = Script_ScenDataUtil(task_6C, 4, local_88 * 13 + 2) & 0xFFFF;
      local_88 = local_88 + 8;
      Script_scenToggleCharaFlag(0, local_88, local_8C);
      break;
    }
    if (local_9C <= 0x4000) {
      local_9C = 0x4000;
      if (local_93 == 0) {
        task_DC[arg2] = 0;
        local_93 = 1;
      }
    }
    local_94 = (Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, local_9C) + 1.0f) * 0.45f;
    {
      int32_t local_80;
      local_80 = arg4;
      if (local_80 == 4) {
        local_DC.x = (&local_C4)->x + local_94;
        local_D0.x = (&local_B8)->x - local_94;
        Script_charaPosRotUtils(arg1, 12, &local_DC, 0, 0);
        Script_charaPosRotUtils(arg1, 7, &local_D0, 0, 0);
      } else if ((local_80 == 0) | (local_80 == 1)) {
        local_DC.x = (&local_C4)->x - ((float)0 - local_94) * Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_A0)->y);
        (&local_DC)->z = (&local_C4)->z - local_94 * Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_A0)->y);
        Script_EVT_SetPosition(arg1, POS_XYZ, &local_DC);
      } else {
        if ((local_80 == 2) | (local_80 == 3)) {
          local_D0.x = (&local_B8)->x + ((float)0 - local_94) * Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_A0)->y);
          (&local_D0)->z = (&local_B8)->z + local_94 * Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_A0)->y);
          Script_EVT_SetPosition(arg1, POS_XYZ, &local_D0);
          goto L007307;
        }
      }
    }
    L007307:;
    SCNF_WaitFrames_0000(1);
  }
  if (arg4 == 4) {
    Script_charaPosRotUtils(arg1, 12, &local_C4, 0, 0);
    Script_charaPosRotUtils(arg1, 7, &local_B8, 0, 0);
  } else {
    Script_EVT_SetPosition(arg1, POS_XYZ, &local_C4);
    Script_EVT_SetPosition(arg1, POS_XYZ, &local_B8);
  }
  return;
}

__attribute__((scn_string_bindings(0, data_1F8BE)))
static void script_7357(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  int32_t local_F0;
  Vector3 local_E4;
  Vector3 local_D8;
  Vector3 local_CC;
  float local_C8;
  float local_C4;
  float local_C0;
  float local_BC;
  float local_B8;
  int8_t local_B7;
  uint8_t unused_B4[3];
  Vector3i local_A8;
  int32_t local_A4;
  local_A4 = arg3;
  switch (local_A4) {
    case 24:
      local_A4 = arg4;
      switch (local_A4) {
        case 1:
          local_B7 = 1;
          local_F8 = 18;
          local_F0 = 0x4000;
          local_B8 = 1.0f;
          local_FC = Script_ScenDataUtil(data_1E364, 4, Script_ScenDataUtil(data_1DD74, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9);
          break;
        case 0:
          local_B7 = 0;
          local_F8 = 12;
          local_F0 = 0x4000;
          local_B8 = 1.0f;
          local_FC = Script_ScenDataUtil(data_1E364, 4, Script_ScenDataUtil(data_1DD74, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9) | -0x80000000;
          break;
      }
      break;
    case 27:
      local_A4 = arg4;
      switch (local_A4) {
        case 1:
          local_B7 = 0;
          local_F8 = 12;
          local_F0 = 0xC000;
          local_B8 = 1.0f;
          local_FC = Script_ScenDataUtil(data_1E364, 4, Script_ScenDataUtil(data_1DD74, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9) | -0x80000000;
          break;
        case 0:
          local_B7 = 1;
          local_F8 = 18;
          local_F0 = 0xC000;
          local_B8 = 1.0f;
          local_FC = Script_ScenDataUtil(data_1E364, 4, Script_ScenDataUtil(data_1DD74, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9);
          break;
      }
      break;
    case 25:
      local_A4 = arg4;
      switch (local_A4) {
        case 1:
          local_B7 = 1;
          local_F8 = 12;
          local_F0 = 0xC000;
          local_B8 = -1.0f;
          local_FC = Script_ScenDataUtil(data_1E364, 4, Script_ScenDataUtil(data_1DD74, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9) | -0x80000000;
          break;
        case 0:
          local_B7 = 0;
          local_F8 = 18;
          local_F0 = 0xC000;
          local_B8 = -1.0f;
          local_FC = Script_ScenDataUtil(data_1E364, 4, Script_ScenDataUtil(data_1DD74, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9);
          break;
      }
      break;
    case 26:
      local_A4 = arg4;
      switch (local_A4) {
        case 1:
          local_B7 = 0;
          local_F8 = 18;
          local_F0 = 0x4000;
          local_B8 = -1.0f;
          local_FC = Script_ScenDataUtil(data_1E364, 4, Script_ScenDataUtil(data_1DD74, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9);
          break;
        case 0:
          local_B7 = 1;
          local_F8 = 12;
          local_F0 = 0x4000;
          local_B8 = -1.0f;
          local_FC = Script_ScenDataUtil(data_1E364, 4, Script_ScenDataUtil(data_1DD74, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9) | -0x80000000;
          break;
      }
      break;
  }
  *(int32_t *)&local_C4 = Script_ScenDataUtil(data_1E364, 5, Script_ScenDataUtil(data_1DD74, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9 + 1);
  *(int32_t *)&local_C0 = Script_ScenDataUtil(data_1E364, 5, Script_ScenDataUtil(data_1DD74, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9 + 2);
  local_FC = local_FC | ((Script_FloatUtils(ScriptMathOp_FloatToInt, local_C4 - 3.0f) << 16) & 0x7FFF0000);
  local_BC = 0.0f;
  script_CDF6(arg0, 0);
  local_CC.x = (&task_1D4)->x;
  (&local_CC)->y = task_1D4.y;
  (&local_CC)->z = task_1D4.z;
  Script_ryoControl(PCCTL_StartCameraFacingInterpolation, 0);
  Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
  Script_SetCharaMoveTarget(arg0, &task_1D4, *(&task_1E0 + 1), local_FC);
  for (;;) {
    if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0) == 0) {
      break;
    }
    if (Script_GetCharaMoveStatus(arg0) == 0) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  local_FC = local_FC & 0xFFFF;
  Script_EVT_GetPosition(arg1, -1, &local_E4, 0);
  Script_mtwkGetSetRotation(arg1, &local_A8, 0);
  local_F4 = (&local_A8)->y;
  for (;;) {
    if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0) == 0) {
      Script_sprintf("This Motion don't Finish!!\012");
      break;
    }
    if (Script_IsCharaMotionFinished(arg0)) {
      break;
    }
    local_C8 = Script_GetCharaMotionRemainingFramesScaled(arg0);
    if (!(local_C8 < local_C4) & !(local_C8 > local_C0)) {
      if (local_B7 == 1) {
        local_BC = local_BC - 0.3f;
      }
      if (!(local_C8 < local_C4) & !(local_C8 >= local_C4 + 1.0f)) {
        int32_t local_A0;
        local_A0 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_B4[arg2], 2);
        local_A0 = Script_ScenDataUtil(task_74, 4, Script_ScenDataUtil(task_78, 4, local_A0 * 9 + 1) * 4 + 2);
        if (local_A0 != -1) {
          if (arg0 == task_1CC) {
            Script_executeAicaCommand(local_A0, 0, 0);
          } else {
            Script_Execute3DSoundCommand_Wrapper(local_A0, 4, &local_CC, 5.0f, 20.0f, 0);
          }
        }
      }
      Script_EVT_GetPosition(arg0, local_F8, &local_D8, ENTITY_XFORM_WORLD_SPACE);
      local_F4 = Script_FloatUtils(ScriptMathOp_Atan2ReverseVector, (&local_E4)->x - (&local_D8)->x, (&local_E4)->z - (&local_D8)->z);
      local_F4 = local_F4 + local_F0 + (int32_t)(local_BC * local_B8 / 360.0f * (float)0x10000);
    }
    {
      Vector3i local_98;
      (&local_98)->y = local_F4;
      Script_EVT_SetRotation(arg1, MATRIX_XFORM_Y, &local_98, 0);
    }
    SCNF_WaitFrames_0000(1);
  }
  script_CDF6(arg0, 1);
  local_FC = task_B4[arg2];
  {
    int32_t local_A0;
    local_A0 = Script_ScenDataUtil(task_6C, 4, local_FC * 13 + 1);
    local_A0 = local_A0 | 8;
    Script_ScenDataUtil(task_6C, 14, local_FC * 13 + 1, local_A0);
  }
  {
    int32_t local_A0;
    local_A0 = Script_ScenDataUtil(task_6C, 4, local_FC * 13 + 1);
    local_A0 = local_A0 | 0x10;
    Script_ScenDataUtil(task_6C, 14, local_FC * 13 + 1, local_A0);
  }
  {
    int32_t local_A0;
    local_A0 = Script_ScenDataUtil(task_6C, 4, local_FC * 13 + 1);
    local_A0 = local_A0 | 4;
    Script_ScenDataUtil(task_6C, 14, local_FC * 13 + 1, local_A0);
  }
  return;
}

__attribute__((scn_string_bindings(0, data_1F8DA)))
static void script_7A11(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
  Vector3 local_F4;
  Vector3 local_E8;
  int32_t unused_DC[3];
  Vector3 local_D0;
  int8_t local_CF;
  int8_t local_CE;
  uint8_t unused_CC[2];
  float local_C8;
  float local_C4;
  float local_C0;
  float local_BC;
  float local_B8;
  float local_B4;
  float local_B0;
  float local_AC;
  float local_A8;
  int32_t local_A4;
  int32_t local_A0;
  int32_t local_9C;
  int32_t local_98;
  int32_t local_94;
  int32_t local_90;
  int32_t local_8C;
  int32_t local_88;
  int32_t local_84;
  int32_t local_80;
  int32_t local_FF7C;
  float local_FF78;
  float local_FF74;
  int32_t local_FF70;
  Vector3i local_FF64;
  int32_t local_FF60;
  local_CE = 0;
  local_80 = 0;
  local_FF78 = 1.0f;
  local_FF74 = 0.0f;
  {
    int32_t local_FF5C;
    local_FF5C = arg3;
    if ((local_FF5C == 4) | (local_FF5C == 6) | (local_FF5C == 33) | (local_FF5C == 35)) {
      if (arg0 == task_1CC) {
        local_FF5C = arg4;
        switch (local_FF5C) {
          case 1:
            local_CF = 1;
            local_A4 = Script_ScenDataUtil(data_1E364, 4, Script_ScenDataUtil(data_1DD74, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9);
            local_C0 = 1.0f;
            local_A0 = 18;
            local_9C = 18;
            local_90 = 15473;
            local_8C = 17294;
            break;
          case 0:
            local_CF = 0;
            local_A4 = Script_ScenDataUtil(data_1E364, 4, Script_ScenDataUtil(data_1DD74, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9) | -0x80000000;
            local_C0 = 1.0f;
            local_A0 = 12;
            local_9C = 18;
            local_90 = 17294;
            local_8C = 15473;
            break;
        }
      } else {
        local_FF5C = arg4;
        switch (local_FF5C) {
          case 1:
            local_CF = 1;
            local_A4 = Script_ScenDataUtil(data_1E364, 4, Script_ScenDataUtil(data_1DD74, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9);
            local_C0 = 1.0f;
            local_A0 = 18;
            local_9C = 18;
            local_90 = 13653;
            break;
          case 0:
            local_CF = 0;
            local_A4 = Script_ScenDataUtil(data_1E364, 4, Script_ScenDataUtil(data_1DD74, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9);
            local_C0 = 1.0f;
            local_A0 = 18;
            local_9C = 18;
            local_90 = 19842;
            break;
        }
      }
    } else if ((local_FF5C == 1) | (local_FF5C == 7) | (local_FF5C == 30) | (local_FF5C == 36) | (local_FF5C == 37)) {
      if (arg0 == task_1CC) {
        local_FF5C = arg4;
        switch (local_FF5C) {
          case 1:
            local_CF = 0;
            local_A4 = Script_ScenDataUtil(data_1E364, 4, Script_ScenDataUtil(data_1DD74, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9) | -0x80000000;
            local_C0 = 1.0f;
            local_A0 = 12;
            local_9C = 18;
            local_90 = 50062;
            local_8C = 48241;
            break;
          case 0:
            local_CF = 1;
            local_A4 = Script_ScenDataUtil(data_1E364, 4, Script_ScenDataUtil(data_1DD74, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9);
            local_C0 = 1.0f;
            local_A0 = 18;
            local_9C = 18;
            local_90 = 48241;
            local_8C = 50062;
            break;
        }
      } else {
        local_FF5C = arg4;
        switch (local_FF5C) {
          case 1:
            local_CF = 0;
            local_A4 = Script_ScenDataUtil(data_1E364, 4, Script_ScenDataUtil(data_1DD74, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9);
            local_C0 = 1.0f;
            local_A0 = 18;
            local_9C = 18;
            if (task_104[arg2] == 4) {
              local_90 = 50062;
            } else {
              local_90 = 51882;
            }
            break;
          case 0:
            local_CF = 1;
            local_A4 = Script_ScenDataUtil(data_1E364, 4, Script_ScenDataUtil(data_1DD74, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9);
            local_C0 = 1.0f;
            local_A0 = 18;
            local_9C = 18;
            if (task_104[arg2] == 4) {
              local_90 = 46421;
            } else {
              local_90 = 45511;
            }
            break;
        }
      }
    } else if ((local_FF5C == 3) | (local_FF5C == 5) | (local_FF5C == 32) | (local_FF5C == 34)) {
      if (arg0 == task_1CC) {
        local_FF5C = arg4;
        switch (local_FF5C) {
          case 1:
            local_CF = 1;
            local_A4 = Script_ScenDataUtil(data_1E364, 4, Script_ScenDataUtil(data_1DD74, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9) | -0x80000000;
            local_C0 = -1.0f;
            local_A0 = 12;
            local_9C = 12;
            local_90 = 50062;
            local_8C = 48241;
            break;
          case 0:
            local_CF = 0;
            local_A4 = Script_ScenDataUtil(data_1E364, 4, Script_ScenDataUtil(data_1DD74, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9);
            local_C0 = -1.0f;
            local_A0 = 18;
            local_9C = 12;
            local_90 = 48241;
            local_8C = 50062;
            break;
        }
      } else {
        local_FF5C = arg4;
        switch (local_FF5C) {
          case 1:
            local_CF = 1;
            local_A4 = Script_ScenDataUtil(data_1E364, 4, Script_ScenDataUtil(data_1DD74, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9);
            local_C0 = -1.0f;
            local_A0 = 18;
            local_9C = 18;
            local_90 = 51882;
            break;
          case 0:
            local_CF = 0;
            local_A4 = Script_ScenDataUtil(data_1E364, 4, Script_ScenDataUtil(data_1DD74, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9);
            local_C0 = -1.0f;
            local_A0 = 18;
            local_9C = 18;
            local_90 = 45511;
            break;
        }
      }
    } else if ((local_FF5C == 2) | (local_FF5C == 8) | (local_FF5C == 31)) {
      if (arg0 == task_1CC) {
        local_FF5C = arg4;
        switch (local_FF5C) {
          case 1:
            local_CF = 0;
            local_A4 = Script_ScenDataUtil(data_1E364, 4, Script_ScenDataUtil(data_1DD74, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9);
            local_C0 = -1.0f;
            local_A0 = 18;
            local_9C = 12;
            local_90 = 15473;
            local_8C = 17294;
            break;
          case 0:
            local_CF = 1;
            local_A4 = Script_ScenDataUtil(data_1E364, 4, Script_ScenDataUtil(data_1DD74, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9) | -0x80000000;
            local_C0 = -1.0f;
            local_A0 = 12;
            local_9C = 12;
            local_90 = 17294;
            local_8C = 15473;
            break;
        }
      } else {
        local_FF5C = arg4;
        switch (local_FF5C) {
          case 1:
            local_CF = 0;
            local_A4 = Script_ScenDataUtil(data_1E364, 4, Script_ScenDataUtil(data_1DD74, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9);
            local_C0 = -1.0f;
            local_A0 = 18;
            local_9C = 18;
            local_90 = 13653;
            break;
          case 0:
            local_CF = 1;
            local_A4 = Script_ScenDataUtil(data_1E364, 4, Script_ScenDataUtil(data_1DD74, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9);
            local_C0 = -1.0f;
            local_A0 = 18;
            local_9C = 18;
            local_90 = 19842;
            break;
        }
      }
    } else {
      if ((local_FF5C == 13) | (local_FF5C == 14) | (local_FF5C == 28)) {
        if (arg0 == task_1CC) {
          local_FF5C = arg4;
          switch (local_FF5C) {
            case 0:
              local_CF = 0;
              local_A4 = Script_ScenDataUtil(data_1E364, 4, Script_ScenDataUtil(data_1DD74, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9);
              local_C0 = -1.0f;
              local_A0 = 18;
              local_9C = 12;
              local_90 = 48241;
              local_8C = 50062;
              break;
            case 1:
              local_CF = 1;
              local_A4 = Script_ScenDataUtil(data_1E364, 4, Script_ScenDataUtil(data_1DD74, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9) | -0x80000000;
              local_C0 = -1.0f;
              local_A0 = 12;
              local_9C = 12;
              local_90 = 50062;
              local_8C = 48241;
              break;
            case 2:
              local_CF = 1;
              local_A4 = Script_ScenDataUtil(data_1E364, 4, Script_ScenDataUtil(data_1DD74, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9);
              local_C0 = 1.0f;
              local_A0 = 18;
              local_9C = 18;
              local_90 = 15473;
              local_8C = 17294;
              break;
            case 3:
              local_CF = 0;
              local_A4 = Script_ScenDataUtil(data_1E364, 4, Script_ScenDataUtil(data_1DD74, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9) | -0x80000000;
              local_C0 = 1.0f;
              local_A0 = 12;
              local_9C = 18;
              local_90 = 17294;
              local_8C = 15473;
              break;
          }
        } else {
          local_FF5C = arg4;
          switch (local_FF5C) {
            case 0:
              local_CF = 0;
              local_A4 = Script_ScenDataUtil(data_1E364, 4, Script_ScenDataUtil(data_1DD74, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9);
              local_C0 = -1.0f;
              local_A0 = 18;
              local_9C = 18;
              local_90 = 45511;
              break;
            case 1:
              local_CF = 1;
              local_A4 = Script_ScenDataUtil(data_1E364, 4, Script_ScenDataUtil(data_1DD74, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9);
              local_C0 = -1.0f;
              local_A0 = 18;
              local_9C = 18;
              local_90 = 51882;
              break;
            case 2:
              local_CF = 1;
              local_A4 = Script_ScenDataUtil(data_1E364, 4, Script_ScenDataUtil(data_1DD74, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9);
              local_C0 = 1.0f;
              local_A0 = 18;
              local_9C = 18;
              if (task_104[arg2] == 4) {
                local_90 = 14199;
              } else {
                local_90 = 13653;
              }
              break;
            case 3:
              local_CF = 0;
              local_A4 = Script_ScenDataUtil(data_1E364, 4, Script_ScenDataUtil(data_1DD74, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9);
              local_C0 = 1.0f;
              local_A0 = 18;
              local_9C = 18;
              if (task_104[arg2] == 4) {
                local_90 = 18022;
              } else {
                local_90 = 19842;
              }
              break;
          }
        }
        local_CE = 1;
        goto L008604;
      }
    }
  }
  L008604:;
  (void)local_90;
  *(int32_t *)&local_B8 = Script_ScenDataUtil(data_1E364, 5, Script_ScenDataUtil(data_1DD74, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9 + 1);
  *(int32_t *)&local_B4 = Script_ScenDataUtil(data_1E364, 5, Script_ScenDataUtil(data_1DD74, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9 + 2);
  *(int32_t *)&local_B0 = Script_ScenDataUtil(data_1E364, 5, Script_ScenDataUtil(data_1DD74, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9 + 3);
  *(int32_t *)&local_AC = Script_ScenDataUtil(data_1E364, 5, Script_ScenDataUtil(data_1DD74, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9 + 4);
  *(int32_t *)&local_A8 = Script_ScenDataUtil(data_1E364, 5, Script_ScenDataUtil(data_1DD74, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9 + 7);
  local_C8 = 0.0f;
  local_FF7C = 0;
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg1)) {
    if (local_CE) {
      {
        int32_t unused_FF5C;
        Vector3i local_FF50;
        Script_charaPosRotUtils(arg1, 12, 0, &local_FF50, 2);
        local_98 = (&local_FF50)->y;
      }
      {
        int32_t unused_FF5C;
        Vector3i local_FF50;
        Script_charaPosRotUtils(arg1, 7, 0, &local_FF50, 2);
        local_94 = (&local_FF50)->y;
      }
      if ((arg4 == 2) | (arg4 == 3)) {
        local_88 = local_98;
        Script_charaPosRotUtils(arg1, 12, &local_F4, 0, 2);
      } else {
        local_88 = local_94;
        Script_charaPosRotUtils(arg1, 7, &local_F4, 0, 2);
      }
      local_FF70 = local_88;
      {
        int32_t unused_FF5C;
        Vector3i local_FF50;
        Vector3 local_FF44;
        Script_EVT_GetPosition(arg1, -1, &local_FF44, 0);
        Script_mtwkGetSetRotation(arg1, &local_FF50, 0);
        local_84 = (&local_FF50)->y;
        {
          float local_FF40;
          float local_FF3C;
          local_FF3C = (&local_F4)->z * Script_MathUtils(SCRIPT_MATH_COS_FIXED16, local_84) - (&local_F4)->x * Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, local_84);
          local_FF40 = (&local_F4)->z * Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, local_84) + (&local_F4)->x * Script_MathUtils(SCRIPT_MATH_COS_FIXED16, local_84);
          local_F4.x = local_FF40 + (&local_FF44)->x;
          (&local_F4)->y = (&local_F4)->y + (&local_FF44)->y;
          (&local_F4)->z = local_FF3C + (&local_FF44)->z;
        }
      }
    } else {
      int32_t unused_FF5C;
      Vector3i local_FF50;
      Script_mtwkGetSetRotation(arg1, &local_FF50, 0);
      local_98 = (&local_FF50)->y;
      local_94 = (&local_FF50)->y;
      local_88 = (&local_FF50)->y;
      local_FF70 = (&local_FF50)->y;
      Script_EVT_GetPosition(arg1, -1, &local_F4, 0);
      local_84 = 0;
    }
  }
  script_3BF2(arg0, local_A4, arg2, arg3, arg4);
  local_A4 = local_A4 | ((Script_FloatUtils(ScriptMathOp_FloatToInt, local_B8 - 3.0f) << 16) & 0x7FFF0000);
  script_CDF6(arg0, 0);
  local_D0.x = (&task_1D4)->x;
  (&local_D0)->y = task_1D4.y;
  (&local_D0)->z = task_1D4.z;
  Script_ryoControl(PCCTL_StartCameraFacingInterpolation, 0);
  Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
  Script_SetCharaMoveTarget(arg0, &task_1D4, *(&task_1E0 + 1), local_A4);
  for (;;) {
    if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0) == 0) {
      break;
    }
    if (Script_GetCharaMoveStatus(arg0) == 0) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  local_A4 = local_A4 & 0xFFFF;
  {
    int32_t local_FF5C;
    int8_t local_FF5B;
    if (arg0 == task_1CC) {
      (void)((local_FF5B = 0) & 0x80);
    } else {
      (void)((local_FF5B = -1) & 0x80);
    }
    if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
      local_C4 = Script_GetCharaMotionRemainingFramesScaled(arg0);
    }
    for (;;) {
      if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0) == 0) {
        Script_sprintf("This Motion don't Finish!!\012");
        break;
      }
      if (arg0 == task_1CC) {
        if (Script_IsCharaMotionFinished(arg0)) {
          break;
        }
        local_C4 = Script_GetCharaMotionRemainingFramesScaled(arg0);
      }
      if (local_FF5B & 8) {
        if (!(local_C4 <= local_B0)) {
          (void)((local_FF5B = 0) & 1);
          {
            uint8_t unused_FF54[7];
            int32_t local_FF50;
            local_FF50 = 1;
            if (local_FF50) {
              script_5E48(arg0, task_104[arg2]);
            }
          }
        }
      }
      if (!(local_C4 > local_B4)) {
        if (!(local_C4 < local_B8) & !(local_C4 > local_B4)) {
          if (arg0 == task_1CC) {
            if (local_CF == 1) {
              local_C8 = local_C8 - 0.165526f;
            } else {
              local_C8 = local_C8 - 0.145973f;
            }
          }
        }
        if (!(local_C4 < local_B8) & !(local_C4 >= local_B8 + 1.0f)) {
          uint8_t unused_FF54[7];
          int32_t local_FF50;
          local_FF50 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_B4[arg2], 2);
          local_FF50 = Script_ScenDataUtil(task_74, 4, Script_ScenDataUtil(task_78, 4, local_FF50 * 9 + 1) * 4 + 2);
          if (local_FF50 != -1) {
            if (arg0 == task_1CC) {
              Script_executeAicaCommand(local_FF50, 0, 0);
            } else {
              Script_Execute3DSoundCommand_Wrapper(local_FF50, 4, &local_D0, 5.0f, 20.0f, 0);
            }
          }
        }
        Script_EVT_GetPosition(arg0, local_A0, &local_E8, ENTITY_XFORM_WORLD_SPACE);
        local_88 = Script_FloatUtils(ScriptMathOp_Atan2ReverseVector, (&local_F4)->x - (&local_E8)->x, (&local_F4)->z - (&local_E8)->z);
        local_88 = local_88 - local_84;
        local_88 = local_88 + local_90 + (int32_t)(local_C8 * local_C0 / 360.0f * (float)0x10000);
        if (local_CE) {
          if ((arg4 == 2) | (arg4 == 3)) {
            Script_charaPosRotUtils(arg1, 12, 0, &local_FF64, 2);
          } else {
            Script_charaPosRotUtils(arg1, 7, 0, &local_FF64, 2);
          }
        } else {
          Script_mtwkGetSetRotation(arg1, &local_FF64, 0);
        }
        if ((local_CF == 0) & !(local_C4 >= local_B8)) {
          local_88 = (&local_FF64)->y;
        }
        local_FF60 = script_3B7A((&local_FF64)->y & 0xFFFF, local_88 & 0xFFFF);
        if (local_FF60 > 910) {
          local_88 = (&local_FF64)->y + 910;
        } else {
          if (local_FF60 < -910) {
            local_88 = (&local_FF64)->y - 910;
            goto L008C09;
          }
        }
        L008C09:;
        if (!(local_C4 >= local_B4)) {
          if (!(local_C0 <= 0.0f)) {
            if (script_3B7A(local_FF70, local_88) > 0) {
              local_88 = local_FF70;
            }
          } else {
            if (script_3B7A(local_FF70, local_88) < 0) {
              local_88 = local_FF70;
            }
          }
        }
      }
      if (arg0 != task_1CC) {
        if (!(local_B4 > local_C4) & !(local_C4 >= local_B4 + 1.0f)) {
          uint8_t unused_FF54[7];
          Vector3i local_FF48;
          if (local_CE) {
            if ((arg4 == 2) | (arg4 == 3)) {
              Script_charaPosRotUtils(arg1, 12, 0, &local_FF48, 2);
            } else {
              Script_charaPosRotUtils(arg1, 7, 0, &local_FF48, 2);
            }
          } else {
            Script_mtwkGetSetRotation(arg1, &local_FF48, 0);
          }
          local_FF74 = (float)script_3B3D(script_3B7A(local_FF70, (&local_FF48)->y) * 360 / 0x10000);
        }
        if (!(local_C4 <= local_B4) & !(local_C4 > local_A8)) {
          *(int32_t *)&local_BC = Script_ScenDataUtil(data_1E364, 5, Script_ScenDataUtil(data_1DD74, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9 + 8);
          if (!(local_FF74 < 90.0f)) {
            local_BC = 0.0f;
          } else {
            local_FF74 = local_FF74 + local_BC;
            if (!(local_FF74 <= 90.0f)) {
              local_BC = local_FF74 - 90.0f;
            }
          }
          local_88 = local_88 - (int32_t)(local_BC * local_C0 / 360.0f * (float)0x10000);
        }
      }
      if (!(local_C4 < local_B0) & !(local_C4 >= local_B0 + 1.0f)) {
        if (local_80 == 0) {
          if (arg0 != task_1CC) {
            uint8_t unused_FF54[7];
            Vector3i local_FF48;
            int32_t local_FF44;
            if (local_CE) {
              if ((arg4 == 2) | (arg4 == 3)) {
                Script_charaPosRotUtils(arg1, 12, 0, &local_FF48, 2);
              } else {
                Script_charaPosRotUtils(arg1, 7, 0, &local_FF48, 2);
              }
            } else {
              Script_mtwkGetSetRotation(arg1, &local_FF48, 0);
            }
            if (local_CE) {
              if ((arg4 == 2) | (arg4 == 3)) {
                if (!((float)(((&local_FF48)->y - local_98) & 0xFFFF) * 360.0f / (float)0x10000 <= 180.0f)) {
                  local_FF44 = local_98 + 0x10000 - (&local_FF48)->y;
                } else {
                  local_FF44 = (&local_FF48)->y - local_98;
                }
              } else if (!((float)(((&local_FF48)->y - local_94) & 0xFFFF) * 360.0f / (float)0x10000 <= 180.0f)) {
                local_FF44 = local_94 + 0x10000 - (&local_FF48)->y;
              } else {
                local_FF44 = (&local_FF48)->y - local_94;
              }
            } else if (!((float)(((&local_FF48)->y - local_98) & 0xFFFF) * 360.0f / (float)0x10000 <= 180.0f)) {
              local_FF44 = local_98 + 0x10000 - (&local_FF48)->y;
            } else {
              local_FF44 = (&local_FF48)->y - local_98;
            }
            local_FF7C = (int32_t)((float)(local_FF44 & 0xFFFF) * 360.0f / (float)0x10000 / (local_AC - local_B0) * local_C0 / 360.0f * (float)0x10000);
            local_80 = 1;
          }
        }
      }
      if (!(local_C4 < local_AC) & !(local_C4 >= local_AC + 1.0f)) {
        uint8_t unused_FF54[7];
        int32_t local_FF50;
        local_FF50 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_B4[arg2], 2);
        local_FF50 = Script_ScenDataUtil(task_74, 4, Script_ScenDataUtil(task_78, 4, local_FF50 * 9 + 1) * 4 + 3);
        if (local_FF50 != -1) {
          if (arg0 == task_1CC) {
            Script_executeAicaCommand(local_FF50, 0, 0);
          } else {
            Script_Execute3DSoundCommand_Wrapper(local_FF50, 4, &local_D0, 5.0f, 20.0f, 0);
          }
        }
        local_A4 = task_B4[arg2];
        if (arg0 == task_1CC) {
          if (task_1CC == arg0) {
            local_A0 = (Script_ScenDataUtil(task_6C, 4, local_A4 * 13 + 2) >> 16) & 0xFFFF;
            Script_scenToggleCharaFlag(0, local_A0, 1);
            local_A0 = Script_ScenDataUtil(task_6C, 4, local_A4 * 13 + 2) & 0xFFFF;
            Script_scenToggleCharaFlag(0, local_A0, 1);
          }
        }
      }
      if (!(local_AC <= 0.0f)) {
        if (!(local_C4 <= local_AC)) {
          if (arg0 == task_1CC) {
            local_FF5C = arg3;
            if ((local_FF5C == 30) | (local_FF5C == 31) | (local_FF5C == 32) | (local_FF5C == 33) | (local_FF5C == 34) | (local_FF5C == 35)) {
              local_FF78 = local_FF78 - 0.06f;
              if (!(local_FF78 >= 0.3f)) {
                local_FF78 = 0.3f;
              }
              DSWK_SetActorOpacityAndDrawCallback(arg1, local_FF78);
              goto L0090B5;
            }
          }
        }
      }
      L0090B5:;
      if (!(local_C4 < local_B0) & !(local_C4 > local_AC)) {
        if (arg0 == task_1CC) {
          if (local_CF == 0) {
            local_C8 = local_C8 - 0.11939f;
          } else {
            local_C8 = local_C8 + 2.381415f;
          }
          Script_EVT_GetPosition(arg0, local_9C, &local_E8, ENTITY_XFORM_WORLD_SPACE);
          local_88 = Script_FloatUtils(ScriptMathOp_Atan2ReverseVector, (&local_F4)->x - (&local_E8)->x, (&local_F4)->z - (&local_E8)->z);
          local_88 = local_88 - local_84;
          local_88 = local_88 + local_8C + (int32_t)(local_C8 * local_C0 / 360.0f * (float)0x10000);
        } else {
          local_88 = local_88 + local_FF7C;
        }
      }
      if (local_CE) {
        uint8_t unused_FF54[7];
        Vector3i local_FF48;
        local_FF48.x = 0;
        (&local_FF48)->z = 0;
        if ((arg4 == 2) | (arg4 == 3)) {
          (&local_FF48)->y = local_88;
          Script_charaPosRotUtils(arg1, 12, 0, &local_FF48, 0);
        } else {
          (&local_FF48)->y = local_88;
          Script_charaPosRotUtils(arg1, 7, 0, &local_FF48, 0);
        }
      } else {
        uint8_t unused_FF54[7];
        Vector3i local_FF48;
        (&local_FF48)->y = local_88;
        Script_EVT_SetRotation(arg1, MATRIX_XFORM_Y, &local_FF48, 0);
      }
      if (arg0 != task_1CC) {
        if (!(local_C4 < local_AC)) {
          break;
        }
        local_C4 = local_C4 + 1.0f;
      }
      SCNF_WaitFrames_0000(1);
    }
  }
  script_CDF6(arg0, 1);
  if (local_CE) {
    int32_t unused_FF5C;
    Vector3i local_FF50;
    local_FF50.x = 0;
    (&local_FF50)->z = 0;
    (&local_FF50)->y = local_98;
    Script_charaPosRotUtils(arg1, 12, 0, &local_FF50, 0);
    (&local_FF50)->y = local_94;
    Script_charaPosRotUtils(arg1, 7, 0, &local_FF50, 0);
  } else {
    {
      int32_t unused_FF5C;
      Vector3i local_FF50;
      (&local_FF50)->y = local_98;
      Script_EVT_SetRotation(arg1, MATRIX_XFORM_Y, &local_FF50, 0);
    }
    {
      int32_t unused_FF5C;
      Vector3i local_FF50;
      (&local_FF50)->y = local_94;
      Script_EVT_SetRotation(arg1, MATRIX_XFORM_Y, &local_FF50, 0);
    }
  }
  if (arg0 == task_1CC) {
    int32_t local_FF5C;
    local_FF5C = arg3;
    if ((local_FF5C == 30) | (local_FF5C == 31) | (local_FF5C == 32) | (local_FF5C == 33) | (local_FF5C == 34) | (local_FF5C == 35)) {
      for (;;) {
        if (Script_CamCtrl(CamCtrlCmd_CheckPosRotCollision) != -1) {
          break;
        }
        SCNF_WaitFrames_0000(1);
      }
      DSWK_SetActorOpacityAndDrawCallback(arg1, 1.0f);
      goto L0092F0;
    }
  }
  L0092F0:;
  return;
}

__attribute__((scn_string_bindings(0, data_1F8F6)))
static void script_92F1(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
  Vector3 local_F4;
  Vector3 local_E8;
  Vector3 local_DC;
  Vector3 local_D0;
  Vector3 local_C4;
  float local_C0;
  float local_BC;
  float local_B8;
  float local_B4;
  int32_t local_B0;
  float local_AC;
  float local_A8;
  float local_A4;
  int16_t local_A2;
  uint8_t unused_A0[2];
  int32_t local_9C;
  int32_t local_98;
  local_A8 = 30.0f;
  local_A4 = 60.0f;
  if (arg5) {
    Script_charaPosRotUtils(arg1, 12, &local_DC, 0, 2);
    Script_charaPosRotUtils(arg1, 7, &local_D0, 0, 2);
  } else {
    Script_EVT_GetPosition(arg1, -1, &local_DC, 0);
    Script_EVT_GetPosition(arg1, -1, &local_D0, 0);
  }
  local_F4.x = 0.0f;
  (&local_F4)->y = 0.0f;
  (&local_F4)->z = 0.0f;
  local_E8.x = 0.0f;
  (&local_E8)->y = 0.0f;
  (&local_E8)->z = 0.0f;
  local_C0 = 0.0f;
  local_A2 = 0;
  local_B8 = 0.0f;
  local_B4 = 0.0f;
  local_B0 = 0;
  {
    int32_t local_94;
    Vector3i local_88;
    Script_mtwkGetSetRotation(arg1, &local_88, 0);
    if ((arg4 == 1) | (arg4 == 3)) {
      local_94 = 1395;
      local_98 = 18;
      local_9C = 18;
      local_AC = 1.0f;
    } else {
      local_94 = -0x7FFFFA8D;
      local_98 = 12;
      local_9C = 12;
      local_AC = -1.0f;
    }
    local_94 = local_94 | ((Script_FloatUtils(ScriptMathOp_FloatToInt, local_A8 - 3.0f) << 16) & 0x7FFF0000);
    script_CDF6(arg0, 0);
    local_C4.x = (&task_1D4)->x;
    (&local_C4)->y = task_1D4.y;
    (&local_C4)->z = task_1D4.z;
    Script_ryoControl(PCCTL_StartCameraFacingInterpolation, 0);
    Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
    Script_SetCharaMoveTarget(arg0, &task_1D4, *(&task_1E0 + 1), local_94);
    for (;;) {
      if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0) == 0) {
        break;
      }
      if (Script_GetCharaMoveStatus(arg0) == 0) {
        break;
      }
      SCNF_WaitFrames_0000(1);
    }
    local_94 = local_94 & 0xFFFF;
    for (;;) {
      if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0) == 0) {
        Script_sprintf("This Motion don't Finish!!\012");
        break;
      }
      if (Script_IsCharaMotionFinished(arg0)) {
        break;
      }
      local_BC = Script_GetCharaMotionRemainingFramesScaled(arg0);
      Script_ReadMOTNPositionAtFrame(arg0, local_94, local_BC, &local_E8);
      local_E8.x = (&local_E8)->x * local_AC;
      if (!(local_BC >= local_A8)) {
        local_A2 = 0;
      }
      if (!(local_BC < local_A8) & !(local_BC > local_A4)) {
        if (!(local_BC < local_A8) & !(local_BC >= local_A8 + 1.0f)) {
          int32_t local_84;
          local_84 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_B4[arg2], 2);
          local_84 = Script_ScenDataUtil(task_74, 4, Script_ScenDataUtil(task_78, 4, local_84 * 9 + 1) * 4 + 3);
          if (local_84 != -1) {
            if (arg0 == task_1CC) {
              Script_executeAicaCommand(local_84, 0, 0);
            } else {
              Script_Execute3DSoundCommand_Wrapper(local_84, 4, &local_C4, 5.0f, 20.0f, 0);
            }
          }
        }
        Script_EVT_GetPosition(arg0, local_9C, &local_F4, ENTITY_XFORM_MOTION_SPACE);
        if (local_A2 == 1) {
          local_B4 = (&local_E8)->x;
          local_C0 = (&local_F4)->x;
        }
        local_B8 = (&local_E8)->x + ((&local_F4)->x - local_C0);
        local_B8 = local_B8 - local_B4;
        local_B8 = local_B8 * 1.28f;
        {
          int32_t local_84;
          local_84 = arg4;
          if ((local_84 == 0) | (local_84 == 2)) {
            if (!(local_B8 < 0.75f)) {
              local_B8 = 0.75f;
            } else {
              if (!(local_B8 > 0.0f)) {
                local_B8 = 0.0f;
                goto L009615;
              }
            }
            L009615:;
          } else {
            if ((local_84 == 1) | (local_84 == 3)) {
              if (!(local_B8 > -0.75f)) {
                local_B8 = -0.75f;
              } else {
                if (!(local_B8 < 0.0f)) {
                  local_B8 = 0.0f;
                  goto L009651;
                }
              }
              L009651:;
              goto L009654;
            }
          }
        }
      }
      L009654:;
      if (!(local_BC <= local_A4)) {
        int32_t local_84;
        local_84 = arg4;
        if ((local_84 == 0) | (local_84 == 2)) {
          local_B8 = 0.75f;
        } else {
          if ((local_84 == 1) | (local_84 == 3)) {
            local_B8 = -0.75f;
            goto L009699;
          }
        }
      }
      L009699:;
      {
        int32_t local_84;
        local_84 = arg4;
        switch (local_84) {
          case 0:
            local_F4.x = (&local_D0)->x;
            (&local_F4)->y = (&local_D0)->y;
            (&local_F4)->z = (&local_D0)->z;
            if (arg5) {
              local_F4.x = (&local_D0)->x - local_B8;
              Script_charaPosRotUtils(arg1, 7, &local_F4, 0, 0);
            } else {
              local_F4.x = (&local_D0)->x + ((float)0 - local_B8) * Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_88)->y);
              (&local_F4)->z = (&local_D0)->z + local_B8 * Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_88)->y);
              Script_EVT_SetPosition(arg1, POS_XYZ, &local_F4);
            }
            break;
          case 1:
            local_F4.x = (&local_D0)->x;
            (&local_F4)->y = (&local_D0)->y;
            (&local_F4)->z = (&local_D0)->z;
            if (arg5) {
              local_F4.x = (&local_D0)->x + local_B8;
              Script_charaPosRotUtils(arg1, 7, &local_F4, 0, 0);
            } else {
              local_F4.x = (&local_D0)->x - ((float)0 - local_B8) * Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_88)->y);
              (&local_F4)->z = (&local_D0)->z - local_B8 * Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_88)->y);
              Script_EVT_SetPosition(arg1, POS_XYZ, &local_F4);
            }
            break;
          case 2:
            local_F4.x = (&local_DC)->x;
            (&local_F4)->y = (&local_DC)->y;
            (&local_F4)->z = (&local_DC)->z;
            if (arg5) {
              local_F4.x = (&local_DC)->x + local_B8;
              Script_charaPosRotUtils(arg1, 12, &local_F4, 0, 0);
            } else {
              local_F4.x = (&local_DC)->x - ((float)0 - local_B8) * Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_88)->y);
              (&local_F4)->z = (&local_DC)->z - local_B8 * Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_88)->y);
              Script_EVT_SetPosition(arg1, POS_XYZ, &local_F4);
            }
            break;
          case 3:
            local_F4.x = (&local_DC)->x;
            (&local_F4)->y = (&local_DC)->y;
            (&local_F4)->z = (&local_DC)->z;
            if (arg5) {
              local_F4.x = (&local_DC)->x - local_B8;
              Script_charaPosRotUtils(arg1, 12, &local_F4, 0, 0);
            } else {
              local_F4.x = (&local_DC)->x + ((float)0 - local_B8) * Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_88)->y);
              (&local_F4)->z = (&local_DC)->z + local_B8 * Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_88)->y);
              Script_EVT_SetPosition(arg1, POS_XYZ, &local_F4);
            }
            break;
        }
      }
      local_A2 = local_A2 + 1;
      SCNF_WaitFrames_0000(1);
    }
  }
  script_CDF6(arg0, 1);
  {
    int32_t local_94;
    local_94 = task_B4[arg2];
    local_9C = Script_ScenDataUtil(task_6C, 4, local_94 * 13 + 2) & 0xFFFF;
    Script_scenToggleCharaFlag(0, local_9C, 1);
    local_9C = (Script_ScenDataUtil(task_6C, 4, local_94 * 13 + 2) >> 16) & 0xFFFF;
    Script_scenToggleCharaFlag(0, local_9C, 1);
    {
      int32_t unused_88[3];
      int32_t local_84;
      local_84 = Script_ScenDataUtil(task_6C, 4, local_94 * 13 + 1);
      local_84 = local_84 & -9;
      Script_ScenDataUtil(task_6C, 14, local_94 * 13 + 1, local_84);
    }
    {
      int32_t unused_88[3];
      int32_t local_84;
      local_84 = Script_ScenDataUtil(task_6C, 4, local_94 * 13 + 1);
      local_84 = local_84 & -17;
      Script_ScenDataUtil(task_6C, 14, local_94 * 13 + 1, local_84);
    }
    {
      int32_t unused_88[3];
      int32_t local_84;
      local_84 = Script_ScenDataUtil(task_6C, 4, local_94 * 13 + 1);
      local_84 = local_84 & -5;
      Script_ScenDataUtil(task_6C, 14, local_94 * 13 + 1, local_84);
    }
  }
  return;
}

__attribute__((scn_string_bindings(0, data_1F912)))
static void script_9A12(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
  Vector3i local_F4;
  Vector3 local_E8;
  float local_E4;
  float local_E0;
  Vector3 local_D4;
  Vector3 local_C8;
  int32_t unused_BC[3];
  Vector3 local_B0;
  int8_t local_AF;
  uint8_t unused_AC[3];
  float local_A8;
  float local_A4;
  float local_A0;
  float local_9C;
  float local_98;
  float local_94;
  float local_90;
  int32_t local_8C;
  int32_t local_88;
  int32_t local_84;
  int32_t local_80;
  int32_t local_FF7C;
  int32_t local_FF78;
  int32_t local_FF74;
  int32_t local_FF70;
  int32_t local_FF6C;
  int32_t local_FF68;
  local_FF68 = arg4;
  switch (local_FF68) {
    case 0:
      local_AF = 0;
      local_8C = Script_ScenDataUtil(data_1E364, 4, Script_ScenDataUtil(data_1DD74, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9);
      local_A0 = -1.0f;
      local_88 = 18;
      local_84 = 18;
      local_FF78 = 46421;
      local_FF74 = 48787;
      break;
    case 1:
      local_AF = 1;
      local_8C = Script_ScenDataUtil(data_1E364, 4, Script_ScenDataUtil(data_1DD74, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9) | -0x80000000;
      local_A0 = -1.0f;
      local_88 = 12;
      local_84 = 12;
      local_FF78 = 50426;
      local_FF74 = 48969;
      break;
    case 2:
      local_AF = 1;
      local_8C = Script_ScenDataUtil(data_1E364, 4, Script_ScenDataUtil(data_1DD74, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9);
      local_A0 = 1.0f;
      local_88 = 18;
      local_84 = 18;
      local_FF78 = 15109;
      local_FF74 = 16566;
      break;
    case 3:
      local_AF = 0;
      local_8C = Script_ScenDataUtil(data_1E364, 4, Script_ScenDataUtil(data_1DD74, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9) | -0x80000000;
      local_A0 = 1.0f;
      local_88 = 12;
      local_84 = 12;
      local_FF78 = 19114;
      local_FF74 = 16748;
      break;
  }
  (void)local_FF78;
  *(int32_t *)&local_9C = Script_ScenDataUtil(data_1E364, 5, Script_ScenDataUtil(data_1DD74, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9 + 1);
  *(int32_t *)&local_98 = Script_ScenDataUtil(data_1E364, 5, Script_ScenDataUtil(data_1DD74, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9 + 2);
  *(int32_t *)&local_94 = Script_ScenDataUtil(data_1E364, 5, Script_ScenDataUtil(data_1DD74, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9 + 3);
  *(int32_t *)&local_90 = Script_ScenDataUtil(data_1E364, 5, Script_ScenDataUtil(data_1DD74, 1, arg3 * 40 + task_104[arg2] * 4 + arg4) * 9 + 4);
  local_A8 = 0.0f;
  Script_charaPosRotUtils(arg1, 12, 0, &local_F4, 2);
  local_80 = (&local_F4)->y;
  Script_charaPosRotUtils(arg1, 7, 0, &local_F4, 2);
  local_FF7C = (&local_F4)->y;
  if ((arg4 == 2) | (arg4 == 3)) {
    local_FF70 = local_80;
    Script_charaPosRotUtils(arg1, 12, &local_D4, 0, 2);
  } else {
    local_FF70 = local_FF7C;
    Script_charaPosRotUtils(arg1, 7, &local_D4, 0, 2);
  }
  Script_EVT_GetPosition(arg1, -1, &local_E8, 0);
  Script_mtwkGetSetRotation(arg1, &local_F4, 0);
  local_FF6C = (&local_F4)->y;
  local_E0 = (&local_D4)->z * Script_MathUtils(SCRIPT_MATH_COS_FIXED16, local_FF6C) - (&local_D4)->x * Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, local_FF6C);
  local_E4 = (&local_D4)->z * Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, local_FF6C) + (&local_D4)->x * Script_MathUtils(SCRIPT_MATH_COS_FIXED16, local_FF6C);
  local_D4.x = local_E4 + (&local_E8)->x;
  (&local_D4)->y = (&local_D4)->y + (&local_E8)->y;
  (&local_D4)->z = local_E0 + (&local_E8)->z;
  local_8C = local_8C | ((Script_FloatUtils(ScriptMathOp_FloatToInt, local_9C - 3.0f) << 16) & 0x7FFF0000);
  script_CDF6(arg0, 0);
  local_B0.x = (&task_1D4)->x;
  (&local_B0)->y = task_1D4.y;
  (&local_B0)->z = task_1D4.z;
  Script_ryoControl(PCCTL_StartCameraFacingInterpolation, 0);
  Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
  Script_SetCharaMoveTarget(arg0, &task_1D4, *(&task_1E0 + 1), local_8C);
  for (;;) {
    if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0) == 0) {
      break;
    }
    if (Script_GetCharaMoveStatus(arg0) == 0) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  local_8C = local_8C & 0xFFFF;
  for (;;) {
    if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0) == 0) {
      Script_sprintf("This Motion don't Finish!!\012");
      break;
    }
    if (Script_IsCharaMotionFinished(arg0)) {
      break;
    }
    local_A4 = Script_GetCharaMotionRemainingFramesScaled(arg0);
    if (!(local_A4 < local_9C) & !(local_A4 > local_98)) {
      if (local_AF == 1) {
      } else {
        local_A8 = local_A8 - 0.208874f;
      }
      if (!(local_A4 < local_9C) & !(local_A4 >= local_9C + 1.0f)) {
        int32_t local_FF64;
        local_FF64 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_B4[arg2], 2);
        local_FF64 = Script_ScenDataUtil(task_74, 4, Script_ScenDataUtil(task_78, 4, local_FF64 * 9 + 1) * 4 + 2);
        if (local_FF64 != -1) {
          if (arg0 == task_1CC) {
            Script_executeAicaCommand(local_FF64, 0, 0);
          } else {
            Script_Execute3DSoundCommand_Wrapper(local_FF64, 4, &local_B0, 5.0f, 20.0f, 0);
          }
        }
      }
      Script_EVT_GetPosition(arg0, local_88, &local_C8, ENTITY_XFORM_WORLD_SPACE);
      local_FF70 = Script_FloatUtils(ScriptMathOp_Atan2ReverseVector, (&local_D4)->x - (&local_C8)->x, (&local_D4)->z - (&local_C8)->z);
      local_FF70 = local_FF70 - local_FF6C;
      local_FF70 = local_FF70 + local_FF78 + (int32_t)(local_A8 * local_A0 / 360.0f * (float)0x10000);
    }
    if (!(local_A4 < local_94) & !(local_A4 > local_90)) {
      if (local_AF == 0) {
      } else {
        local_A8 = local_A8 + 0.12844f;
      }
      if (!(local_A4 < local_90) & !(local_A4 >= local_90 + 1.0f)) {
        int32_t local_FF64;
        local_FF64 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_B4[arg2], 2);
        local_FF64 = Script_ScenDataUtil(task_74, 4, Script_ScenDataUtil(task_78, 4, local_FF64 * 9 + 1) * 4 + 3);
        if (local_FF64 != -1) {
          if (arg0 == task_1CC) {
            Script_executeAicaCommand(local_FF64, 0, 0);
          } else {
            Script_Execute3DSoundCommand_Wrapper(local_FF64, 4, &local_B0, 5.0f, 20.0f, 0);
          }
        }
      }
      Script_EVT_GetPosition(arg0, local_84, &local_C8, ENTITY_XFORM_WORLD_SPACE);
      local_FF70 = Script_FloatUtils(ScriptMathOp_Atan2ReverseVector, (&local_D4)->x - (&local_C8)->x, (&local_D4)->z - (&local_C8)->z);
      local_FF70 = local_FF70 - local_FF6C;
      local_FF70 = local_FF70 + local_FF74 + (int32_t)(local_A8 * local_A0 / 360.0f * (float)0x10000);
    }
    local_F4.x = 0;
    (&local_F4)->z = 0;
    if ((arg4 == 2) | (arg4 == 3)) {
      (&local_F4)->y = local_FF70;
      Script_charaPosRotUtils(arg1, 12, 0, &local_F4, 0);
    } else {
      (&local_F4)->y = local_FF70;
      Script_charaPosRotUtils(arg1, 7, 0, &local_F4, 0);
    }
    SCNF_WaitFrames_0000(1);
  }
  script_CDF6(arg0, 1);
  local_F4.x = 0;
  (&local_F4)->z = 0;
  (&local_F4)->y = local_80;
  Script_charaPosRotUtils(arg1, 12, 0, &local_F4, 0);
  (&local_F4)->y = local_FF7C;
  Script_charaPosRotUtils(arg1, 7, 0, &local_F4, 0);
  return;
}

static void script_A19C(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
  int32_t local_FC;
  int32_t local_F8;
  Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
  Script_SetCharaMoveTarget(arg0, &task_1D4, *(&task_1E0 + 1), MN_AKI_AKI_BASE);
  for (;;) {
    if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0) == 0) {
      break;
    }
    if (Script_GetCharaMoveStatus(arg0) == 0) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  local_F8 = 0;
  for (;;) {
    if (local_F8 == 0) {
      local_F8 = 1;
      if (arg5 != 0) {
        local_FC = 11;
      } else {
        local_FC = Script_ScenDataUtil(task_6C, 4, task_B4[arg2] * 13 + 7);
      }
      if (local_FC == 0) {
        SCNF_WaitFrames_0000(45);
      }
      Script_EnqueueSCNT(1, 0x47524F44, local_FC);
    } else {
      if (Script_EnqueueSCNT(2) == 0) {
        break;
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  return;
}

__attribute__((scn_string_bindings(0, data_1F92E)))
static void script_A274(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
  Vector3 local_F4;
  float local_F0;
  float local_EC;
  float local_E8;
  int32_t unused_E4;
  int32_t local_E0;
  int32_t local_DC;
  int32_t local_D8;
  int32_t local_D4;
  int32_t unused_D0;
  int32_t local_CC;
  local_DC = 0;
  local_D4 = Script_ScenDataUtil(task_6C, 4, task_B4[arg2] * 13 + 6);
  local_CC = arg3;
  if ((local_CC == 1) | (local_CC == 3) | (local_CC == 5) | (local_CC == 7) | (local_CC == 9) | (local_CC == 18) | (local_CC == 21) | (local_CC == 25) | (local_CC == 27) | (local_CC == 36) | (local_CC == 37)) {
    local_CC = arg4;
    switch (local_CC) {
      case 1:
        local_E0 = Script_ScenDataUtil(&data_1ED84, 4, local_D4 * 5) | -0x80000000;
        break;
      case 0:
        local_E0 = Script_ScenDataUtil(&data_1ED84, 4, local_D4 * 5);
        break;
    }
  } else if ((local_CC == 2) | (local_CC == 4) | (local_CC == 6) | (local_CC == 8) | (local_CC == 10) | (local_CC == 19) | (local_CC == 22) | (local_CC == 24) | (local_CC == 26) | (local_CC == 28)) {
    local_CC = arg4;
    switch (local_CC) {
      case 1:
        local_E0 = Script_ScenDataUtil(&data_1ED84, 4, local_D4 * 5);
        break;
      case 0:
        local_E0 = Script_ScenDataUtil(&data_1ED84, 4, local_D4 * 5) | -0x80000000;
        break;
    }
  } else {
    if ((local_CC == 13) | (local_CC == 14) | (local_CC == 15) | (local_CC == 16) | (local_CC == 20) | (local_CC == 23) | (local_CC == 29)) {
      local_CC = arg4;
      switch (local_CC) {
        case 0:
          local_E0 = Script_ScenDataUtil(&data_1ED84, 4, local_D4 * 5);
          break;
        case 1:
          local_E0 = Script_ScenDataUtil(&data_1ED84, 4, local_D4 * 5) | -0x80000000;
          break;
        case 2:
          local_E0 = Script_ScenDataUtil(&data_1ED84, 4, local_D4 * 5);
          break;
        case 3:
          local_E0 = Script_ScenDataUtil(&data_1ED84, 4, local_D4 * 5) | -0x80000000;
          break;
      }
      goto L00A495;
    }
  }
  L00A495:;
  *(int32_t *)&local_EC = Script_ScenDataUtil(&data_1ED84, 5, local_D4 * 5 + 1);
  *(int32_t *)&local_E8 = Script_ScenDataUtil(&data_1ED84, 5, local_D4 * 5 + 2);
  local_E0 = local_E0 | ((Script_FloatUtils(ScriptMathOp_FloatToInt, local_EC - 3.0f) << 16) & 0x7FFF0000);
  local_F4.x = (&task_1D4)->x;
  (&local_F4)->y = task_1D4.y;
  (&local_F4)->z = task_1D4.z;
  Script_ryoControl(PCCTL_StartCameraFacingInterpolation, 0);
  Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
  Script_SetCharaMoveTarget(arg0, &task_1D4, *(&task_1E0 + 1), local_E0);
  for (;;) {
    if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0) == 0) {
      break;
    }
    if (Script_GetCharaMoveStatus(arg0) == 0) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  for (;;) {
    if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0) == 0) {
      Script_sprintf("This Motion don't Finish!!\012");
      break;
    }
    if (Script_IsCharaMotionFinished(arg0)) {
      if (local_DC == 1) {
        if (Script_EnqueueSCNT(2) == 0) {
          break;
        }
      }
    }
    local_F0 = Script_GetCharaMotionRemainingFramesScaled(arg0);
    if (!(local_F0 < local_EC) & !(local_F0 >= local_EC + 1.0f)) {
      int32_t local_C8;
      local_C8 = Script_ScenDataUtil(task_6C, 4, task_B4[arg2] * 13 + 9);
      if (local_C8 != -1) {
        if (arg0 == task_1CC) {
          Script_executeAicaCommand(local_C8, 0, 0);
        } else {
          Script_Execute3DSoundCommand_Wrapper(local_C8, 4, &local_F4, 5.0f, 20.0f, 0);
        }
      }
    }
    if (!(local_F0 < local_E8) & !(local_F0 >= local_E8 + 1.0f)) {
      if ((local_D4 == 6) | (local_D4 == 7)) {
        int32_t local_C8;
        local_C8 = Script_ScenDataUtil(task_6C, 4, task_B4[arg2] * 13 + 9);
        if (local_C8 != -1) {
          if (arg0 == task_1CC) {
            Script_executeAicaCommand(local_C8, 0, 0);
          } else {
            Script_Execute3DSoundCommand_Wrapper(local_C8, 4, &local_F4, 5.0f, 20.0f, 0);
          }
        }
      }
      if (local_DC == 0) {
        local_DC = 1;
        local_D8 = Script_ScenDataUtil(task_6C, 4, task_B4[arg2] * 13 + 7);
        if (local_D8 == 0) {
          SCNF_WaitFrames_0000(45);
        }
        Script_EnqueueSCNT(1, 0x47524F44, local_D8);
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  return;
}

__attribute__((scn_string_bindings(0, data_1F94A)))
static void script_A6EB(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
  Vector3 local_F4;
  int32_t unused_E8[3];
  float local_DC[3];
  Vector3 local_D0;
  float local_C4[3];
  Vector3 local_B8;
  int32_t unused_AC[3];
  int8_t local_AB;
  int8_t local_AA;
  uint8_t unused_A8[2];
  float local_A4;
  float local_A0;
  float local_9C;
  float local_98;
  float local_94;
  int32_t local_90;
  int32_t local_8C;
  int32_t unused_88;
  int32_t local_84;
  int32_t local_80;
  int32_t local_FF7C;
  int32_t local_FF78;
  int32_t local_FF74;
  int32_t unused_FF70;
  int32_t local_FF6C;
  int32_t local_FF68;
  int32_t local_FF64;
  int32_t local_FF60;
  int32_t local_FF5C;
  int32_t local_FF58;
  int32_t unused_FF54;
  Vector3i local_FF48;
  local_AA = 0;
  local_FF5C = 0;
  local_FF58 = 0;
  task_1F8 = 0;
  local_90 = 1543;
  Script_EVT_GetPosition(arg1, -1, &local_D0, 0);
  Script_mtwkGetSetRotation(arg1, &local_FF48, 0);
  local_DC[0] = 0.0f;
  local_DC[1] = 0.0f;
  local_DC[2] = 0.0f;
  {
    int32_t local_FF44;
    local_FF44 = arg3;
    if ((local_FF44 == 4) | (local_FF44 == 6) | (local_FF44 == 33) | (local_FF44 == 35)) {
      local_FF44 = arg4;
      switch (local_FF44) {
        case 1:
          local_AB = 1;
          local_9C = 1.0f;
          local_FF74 = 0;
          break;
        case 0:
          local_AB = 0;
          local_90 = local_90 | -0x80000000;
          local_9C = 1.0f;
          local_FF74 = 1;
          break;
      }
    } else if ((local_FF44 == 1) | (local_FF44 == 7) | (local_FF44 == 30) | (local_FF44 == 36) | (local_FF44 == 37)) {
      local_FF44 = arg4;
      switch (local_FF44) {
        case 1:
          local_AB = 0;
          local_90 = local_90 | -0x80000000;
          local_9C = 1.0f;
          local_FF74 = 1;
          break;
        case 0:
          local_AB = 1;
          local_9C = 1.0f;
          local_FF74 = 0;
          break;
      }
    } else if ((local_FF44 == 3) | (local_FF44 == 5) | (local_FF44 == 32) | (local_FF44 == 34)) {
      local_FF44 = arg4;
      switch (local_FF44) {
        case 1:
          local_AB = 1;
          local_90 = local_90 | -0x80000000;
          local_9C = -1.0f;
          local_FF74 = 1;
          break;
        case 0:
          local_AB = 0;
          local_9C = -1.0f;
          local_FF74 = 0;
          break;
      }
    } else if ((local_FF44 == 2) | (local_FF44 == 8) | (local_FF44 == 31)) {
      local_FF44 = arg4;
      switch (local_FF44) {
        case 1:
          local_AB = 0;
          local_9C = -1.0f;
          local_FF74 = 0;
          break;
        case 0:
          local_AB = 1;
          local_90 = local_90 | -0x80000000;
          local_9C = -1.0f;
          local_FF74 = 1;
          break;
      }
    } else {
      if ((local_FF44 == 13) | (local_FF44 == 14)) {
        local_FF44 = arg4;
        switch (local_FF44) {
          case 0:
            local_AB = 0;
            local_9C = -1.0f;
            local_FF74 = 0;
            break;
          case 1:
            local_AB = 1;
            local_90 = local_90 | -0x80000000;
            local_9C = -1.0f;
            local_FF74 = 1;
            break;
          case 2:
            local_AB = 1;
            local_9C = 1.0f;
            local_FF74 = 0;
            break;
          case 3:
            local_AB = 0;
            local_90 = local_90 | -0x80000000;
            local_9C = 1.0f;
            local_FF74 = 1;
            break;
        }
        local_AA = 1;
        goto L00A9F7;
      }
    }
  }
  L00A9F7:;
  if (local_AA) {
    {
      int32_t unused_FF44;
      Vector3i local_FF38;
      Script_charaPosRotUtils(arg1, 12, 0, &local_FF38, 2);
      local_84 = (&local_FF38)->y;
    }
    {
      int32_t unused_FF44;
      Vector3i local_FF38;
      Script_charaPosRotUtils(arg1, 7, 0, &local_FF38, 2);
      local_80 = (&local_FF38)->y;
    }
    if ((arg4 == 2) | (arg4 == 3)) {
      local_FF7C = local_84;
      Script_charaPosRotUtils(arg1, 12, &local_F4, 0, 2);
    } else {
      local_FF7C = local_80;
      Script_charaPosRotUtils(arg1, 7, &local_F4, 0, 2);
    }
    {
      int32_t unused_FF44;
      Vector3i local_FF38;
      Vector3 local_FF2C;
      Script_EVT_GetPosition(arg1, -1, &local_FF2C, 0);
      Script_mtwkGetSetRotation(arg1, &local_FF38, 0);
      local_FF78 = (&local_FF38)->y;
      {
        float local_FF28;
        float local_FF24;
        local_FF24 = (&local_F4)->z * Script_MathUtils(SCRIPT_MATH_COS_FIXED16, local_FF78) - (&local_F4)->x * Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, local_FF78);
        local_FF28 = (&local_F4)->z * Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, local_FF78) + (&local_F4)->x * Script_MathUtils(SCRIPT_MATH_COS_FIXED16, local_FF78);
        local_F4.x = local_FF28 + (&local_FF2C)->x;
        (&local_F4)->y = (&local_F4)->y + (&local_FF2C)->y;
        (&local_F4)->z = local_FF24 + (&local_FF2C)->z;
      }
    }
  } else {
    int32_t unused_FF44;
    Vector3i local_FF38;
    Script_mtwkGetSetRotation(arg1, &local_FF38, 0);
    local_84 = (&local_FF38)->y;
    local_80 = (&local_FF38)->y;
    local_FF7C = (&local_FF38)->y;
    Script_EVT_GetPosition(arg1, -1, &local_F4, 0);
    local_FF78 = 0;
  }
  local_98 = 33.0f;
  local_94 = 49.0f;
  local_B8.x = (&task_1D4)->x;
  (&local_B8)->y = task_1D4.y;
  (&local_B8)->z = task_1D4.z;
  Script_ryoControl(PCCTL_StartCameraFacingInterpolation, 0);
  Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
  Script_SetCharaMoveTarget(arg0, &task_1D4, *(&task_1E0 + 1), local_90);
  for (;;) {
    if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0) == 0) {
      break;
    }
    if (Script_GetCharaMoveStatus(arg0) == 0) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT, 0x524F4F44, 1);
  while ((local_8C = Script_CycleManagerUtils(CYCLE_CMD_GET_CYCLE_PTR_BY_CHARA_ID, 0x524F4F44)) == 0) {
    SCNF_WaitFrames_0000(1);
  }
  for (;;) {
    if (Script_CycleManagerUtils(CYCLE_CMD_GET_NEXT_TASK_POINTER, local_8C) == 1) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_motmGetCharaFlags(SHEN_NAME('DOOR'), 0, 0);
  Script_PlayCharaMotion(SHEN_NAME('DOOR'), MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_charaDisplay(SHEN_NAME('DOOR'), CDM_Show);
  {
    int32_t local_FF44;
    local_FF44 = arg3;
    if ((local_FF44 == 0) | (local_FF44 == 1) | (local_FF44 == 3) | (local_FF44 == 5) | (local_FF44 == 7) | (local_FF44 == 9) | (local_FF44 == 11) | (local_FF44 == 25) | (local_FF44 == 27) | (local_FF44 == 30) | (local_FF44 == 32) | (local_FF44 == 34) | (local_FF44 == 36) | (local_FF44 == 37)) {
      local_FF44 = arg4;
      switch (local_FF44) {
        case 0:
          local_DC[0] = 0.45f;
          local_DC[2] = 0.5f;
          break;
        case 1:
          local_DC[0] = -0.45f;
          local_DC[2] = 0.5f;
          (&local_FF48)->y = (&local_FF48)->y + 0x8000;
          break;
        case 2:
          local_DC[0] = 0.45f;
          local_DC[2] = 0.5f;
          (&local_FF48)->y = (&local_FF48)->y + 0x8000;
          break;
        case 3:
          local_DC[0] = -0.45f;
          local_DC[2] = 0.5f;
          break;
      }
    } else if ((local_FF44 == 2) | (local_FF44 == 4) | (local_FF44 == 6) | (local_FF44 == 8) | (local_FF44 == 10) | (local_FF44 == 12) | (local_FF44 == 24) | (local_FF44 == 26) | (local_FF44 == 31) | (local_FF44 == 33) | (local_FF44 == 35)) {
      local_FF44 = arg4;
      switch (local_FF44) {
        case 0:
          local_DC[0] = -0.45f;
          local_DC[2] = 0.5f;
          break;
        case 1:
          local_DC[0] = 0.45f;
          local_DC[2] = 0.5f;
          (&local_FF48)->y = (&local_FF48)->y + 0x8000;
          break;
        case 2:
          local_DC[0] = -0.45f;
          local_DC[2] = 0.5f;
          (&local_FF48)->y = (&local_FF48)->y + 0x8000;
          break;
        case 3:
          local_DC[0] = 0.45f;
          local_DC[2] = 0.5f;
          break;
      }
    } else if ((local_FF44 == 13) | (local_FF44 == 14) | (local_FF44 == 15) | (local_FF44 == 16) | (local_FF44 == 17) | (local_FF44 == 18) | (local_FF44 == 19) | (local_FF44 == 20) | (local_FF44 == 21) | (local_FF44 == 22) | (local_FF44 == 23) | (local_FF44 == 29)) {
      local_FF44 = arg4;
      switch (local_FF44) {
        case 0:
          local_DC[0] = -0.45f;
          local_DC[2] = 0.5f;
          break;
        case 1:
          local_DC[0] = 0.45f;
          local_DC[2] = 0.5f;
          (&local_FF48)->y = (&local_FF48)->y + 0x8000;
          break;
        case 2:
          local_DC[0] = -0.45f;
          local_DC[2] = 0.5f;
          (&local_FF48)->y = (&local_FF48)->y + 0x8000;
          break;
        case 3:
          local_DC[0] = 0.45f;
          local_DC[2] = 0.5f;
          break;
      }
    } else if (local_FF44 == 28) {
      local_FF44 = arg4;
      switch (local_FF44) {
        case 0:
          local_DC[0] = -0.45f;
          local_DC[2] = -0.5f;
          (&local_FF48)->y = (&local_FF48)->y + 0x8000;
          break;
        case 1:
          local_DC[0] = -0.45f;
          local_DC[2] = 0.5f;
          break;
        case 2:
          local_DC[0] = 0.45f;
          local_DC[2] = 0.5f;
          break;
        case 3:
          local_DC[0] = 0.45f;
          local_DC[2] = -0.5f;
          (&local_FF48)->y = (&local_FF48)->y + 0x8000;
          break;
      }
    } else {
      local_FF44 = arg4;
      switch (local_FF44) {
        case 0:
          local_DC[0] = -0.45f;
          local_DC[2] = -0.5f;
          (&local_FF48)->y = (&local_FF48)->y + 0x8000;
          break;
        case 1:
          local_DC[0] = -0.45f;
          local_DC[2] = 0.5f;
          break;
        case 2:
          local_DC[0] = 0.45f;
          local_DC[2] = 0.5f;
          break;
        case 3:
          local_DC[0] = 0.45f;
          local_DC[2] = -0.5f;
          (&local_FF48)->y = (&local_FF48)->y + 0x8000;
          break;
      }
    }
  }
  local_FF60 = (&local_FF48)->y & 0xFFFF;
  local_C4[2] = local_DC[2] * Script_MathUtils(SCRIPT_MATH_COS_FIXED16, local_FF60) - *local_DC * Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, local_FF60);
  local_C4[0] = local_DC[2] * Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, local_FF60) + *local_DC * Script_MathUtils(SCRIPT_MATH_COS_FIXED16, local_FF60);
  local_D0.x = (&local_F4)->x + *local_C4;
  (&local_D0)->y = (&local_F4)->y;
  (&local_D0)->z = (&local_F4)->z + local_C4[2];
  Script_EVT_SetPosition(SHEN_NAME('DOOR'), POS_XYZ, &local_D0);
  Script_EVT_SetRotation(SHEN_NAME('DOOR'), 0x38000000, &local_FF48, 0);
  local_A4 = 0.0f;
  local_FF68 = 0;
  for (;;) {
    if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0) == 0) {
      Script_sprintf("This Motion don't Finish!!\012");
      break;
    }
    if (Script_IsCharaMotionFinished(arg0)) {
      if (local_FF68 == 1) {
        if (Script_EnqueueSCNT(2) == 0) {
          break;
        }
      }
    }
    if (local_FF68 == 1) {
      if (local_FF5C == 0) {
        int32_t unused_FF44;
        int32_t local_FF40;
        SCNF_WaitFrames_0000(45);
        local_FF40 = Script_ScenDataUtil(task_6C, 4, task_B4[arg2] * 13 + 10);
        if (local_FF40 != -1) {
          if (arg0 == task_1CC) {
            Script_executeAicaCommand(local_FF40, 0, 0);
          } else {
            Script_Execute3DSoundCommand_Wrapper(local_FF40, 4, &local_B8, 5.0f, 20.0f, 0);
          }
        }
        local_FF5C = 1;
      } else {
        if (local_FF58 <= 20) {
          local_A4 = local_A4 - 0.05f;
          local_FF7C = local_FF7C + (int32_t)(local_A4 * local_9C / 360.0f * (float)0x10000);
          if (local_AA) {
            int32_t unused_FF44;
            Vector3i local_FF38;
            local_FF38.x = 0;
            (&local_FF38)->z = 0;
            if ((arg4 == 2) | (arg4 == 3)) {
              (&local_FF38)->y = local_FF7C;
              Script_charaPosRotUtils(arg1, 12, 0, &local_FF38, 0);
            } else {
              (&local_FF38)->y = local_FF7C;
              Script_charaPosRotUtils(arg1, 7, 0, &local_FF38, 0);
            }
          } else {
            int32_t unused_FF44;
            Vector3i local_FF38;
            (&local_FF38)->y = local_FF7C;
            Script_EVT_SetRotation(arg1, MATRIX_XFORM_Y, &local_FF38, 0);
          }
          local_FF58 = local_FF58 + 1;
        }
      }
    }
    local_A0 = Script_GetCharaMotionRemainingFramesScaled(arg0);
    if (!(local_A0 < local_98) & !(local_A0 >= local_98 + 1.0f)) {
      int32_t unused_FF44;
      int32_t local_FF40;
      local_FF40 = Script_ScenDataUtil(task_6C, 4, task_B4[arg2] * 13 + 9);
      if (local_FF40 != -1) {
        if (arg0 == task_1CC) {
          Script_executeAicaCommand(local_FF40, 0, 0);
        } else {
          Script_Execute3DSoundCommand_Wrapper(local_FF40, 4, &local_B8, 5.0f, 20.0f, 0);
        }
      }
    }
    if (!(local_A0 < local_94) & !(local_A0 >= local_94 + 1.0f)) {
      int32_t unused_FF44;
      int32_t local_FF40;
      local_FF40 = Script_ScenDataUtil(task_6C, 4, task_B4[arg2] * 13 + 9);
      if (local_FF40 != -1) {
        if (arg0 == task_1CC) {
          Script_executeAicaCommand(local_FF40, 0, 0);
        } else {
          Script_Execute3DSoundCommand_Wrapper(local_FF40, 4, &local_B8, 5.0f, 20.0f, 0);
        }
      }
      if (local_FF68 == 0) {
        local_FF68 = 1;
        local_FF64 = Script_ScenDataUtil(task_6C, 4, task_B4[arg2] * 13 + 8);
        Script_ConversationUtils(1, local_FF74);
        Script_EnqueueSCNT(1, 0x524F4F44, local_FF64);
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  local_A4 = 0.0f;
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0) != 0) {
    local_FF6C = 0;
    L00B4C2:;
    if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0) == 0) {
    } else {
      local_A4 = local_A4 + 0.05f;
      local_FF7C = local_FF7C + (int32_t)(local_A4 * local_9C / 360.0f * (float)0x10000);
      if (local_AA) {
        int32_t unused_FF44;
        Vector3i local_FF38;
        local_FF38.x = 0;
        (&local_FF38)->z = 0;
        if ((arg4 == 2) | (arg4 == 3)) {
          (&local_FF38)->y = local_FF7C;
          Script_charaPosRotUtils(arg1, 12, 0, &local_FF38, 0);
        } else {
          (&local_FF38)->y = local_FF7C;
          Script_charaPosRotUtils(arg1, 7, 0, &local_FF38, 0);
        }
      } else {
        int32_t unused_FF44;
        Vector3i local_FF38;
        (&local_FF38)->y = local_FF7C;
        Script_EVT_SetRotation(arg1, MATRIX_XFORM_Y, &local_FF38, 0);
      }
      SCNF_WaitFrames_0000(1);
      local_FF6C = local_FF6C + 1;
      if (!(local_FF6C >= 20)) goto L00B4C2;
    }
    {
      int32_t unused_FF44;
      int32_t local_FF40;
      local_FF40 = Script_ScenDataUtil(task_6C, 4, task_B4[arg2] * 13 + 11);
      if (local_FF40 != -1) {
        if (arg0 == task_1CC) {
          Script_executeAicaCommand(local_FF40, 0, 0);
        } else {
          Script_Execute3DSoundCommand_Wrapper(local_FF40, 4, &local_B8, 5.0f, 20.0f, 0);
        }
      }
    }
  }
  if (local_AA) {
    int32_t unused_FF44;
    Vector3i local_FF38;
    local_FF38.x = 0;
    (&local_FF38)->z = 0;
    (&local_FF38)->y = local_84;
    Script_charaPosRotUtils(arg1, 12, 0, &local_FF38, 0);
    (&local_FF38)->y = local_80;
    Script_charaPosRotUtils(arg1, 7, 0, &local_FF38, 0);
  } else {
    {
      int32_t unused_FF44;
      Vector3i local_FF38;
      (&local_FF38)->y = local_84;
      Script_EVT_SetRotation(arg1, MATRIX_XFORM_Y, &local_FF38, 0);
    }
    {
      int32_t unused_FF44;
      Vector3i local_FF38;
      (&local_FF38)->y = local_80;
      Script_EVT_SetRotation(arg1, MATRIX_XFORM_Y, &local_FF38, 0);
    }
  }
  Script_CycleManagerUtils(CYCLE_CMD_LOAD_OR_RESET_RUNNING_PROGRAM, local_8C);
  return;
}

static void script_B67E(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
  int32_t local_FC;
  int32_t unused_F8;
  float local_EC[3];
  Vector3 local_E0;
  float local_D4[3];
  Vector3 local_C8;
  int32_t unused_C4;
  int32_t local_BC[2];
  Vector3i local_B0;
  int32_t local_AC;
  Script_sprintf("open-move!!\012");
  local_BC[1] = arg4;
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0) != 0) {
    Script_EVT_GetPosition(arg1, -1, &local_E0, 0);
    Script_EVT_GetPosition(arg0, -1, &local_C8, 0);
    Script_mtwkGetSetRotation(arg0, &local_B0, 0);
    local_EC[0] = 0.0f;
    local_EC[1] = 0.0f;
    local_EC[2] = 0.0f;
    if (arg0 != task_1CC) {
      local_AC = arg2;
      if ((local_AC == 0) | (local_AC == 1) | (local_AC == 3) | (local_AC == 5) | (local_AC == 7) | (local_AC == 9) | (local_AC == 11) | (local_AC == 25) | (local_AC == 27) | (local_AC == 30) | (local_AC == 32) | (local_AC == 34) | (local_AC == 36) | (local_AC == 37)) {
        local_AC = arg3;
        switch (local_AC) {
          case 0:
            local_EC[0] = 0.45f;
            local_EC[2] = 1.0f;
            break;
          case 1:
            local_EC[0] = 0.45f;
            local_EC[2] = -1.0f;
            break;
          case 2:
            local_EC[0] = 0.45f;
            local_EC[2] = -1.0f;
            break;
          case 3:
            local_EC[0] = 0.45f;
            local_EC[2] = 1.0f;
            break;
        }
      } else if ((local_AC == 2) | (local_AC == 4) | (local_AC == 6) | (local_AC == 8) | (local_AC == 10) | (local_AC == 12) | (local_AC == 24) | (local_AC == 26) | (local_AC == 31) | (local_AC == 33) | (local_AC == 35)) {
        local_AC = arg3;
        switch (local_AC) {
          case 0:
            local_EC[0] = -0.45f;
            local_EC[2] = 1.0f;
            break;
          case 1:
            local_EC[0] = -0.45f;
            local_EC[2] = -1.0f;
            break;
          case 2:
            local_EC[0] = 0.45f;
            local_EC[2] = -1.0f;
            break;
          case 3:
            local_EC[0] = 0.45f;
            local_EC[2] = 1.0f;
            break;
        }
      } else if ((local_AC == 13) | (local_AC == 14) | (local_AC == 15) | (local_AC == 16) | (local_AC == 17) | (local_AC == 18) | (local_AC == 19) | (local_AC == 20) | (local_AC == 21) | (local_AC == 22) | (local_AC == 23) | (local_AC == 29) | (local_AC == 28)) {
        local_AC = arg3;
        switch (local_AC) {
          case 0:
            local_EC[0] = 0.45f;
            local_EC[2] = 1.0f;
            break;
          case 1:
            local_EC[0] = 0.45f;
            local_EC[2] = -1.0f;
            break;
          case 2:
            local_EC[0] = -0.45f;
            local_EC[2] = -1.0f;
            break;
          case 3:
            local_EC[0] = -0.45f;
            local_EC[2] = 1.0f;
            break;
        }
      } else {
        local_AC = arg3;
        switch (local_AC) {
          case 0:
            local_EC[0] = 0.45f;
            local_EC[2] = 1.0f;
            break;
          case 1:
            local_EC[0] = 0.45f;
            local_EC[2] = -1.0f;
            break;
          case 2:
            local_EC[0] = -0.45f;
            local_EC[2] = -1.0f;
            break;
          case 3:
            local_EC[0] = -0.45f;
            local_EC[2] = 1.0f;
            break;
        }
      }
      local_FC = local_BC[1] & 0xFFFF;
      local_D4[2] = local_EC[2] * Script_MathUtils(SCRIPT_MATH_COS_FIXED16, local_FC) - *local_EC * Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, local_FC);
      local_D4[0] = local_EC[2] * Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, local_FC) + *local_EC * Script_MathUtils(SCRIPT_MATH_COS_FIXED16, local_FC);
      local_E0.x = (&local_E0)->x + *local_D4;
      (&local_E0)->z = (&local_E0)->z + local_D4[2];
      if ((arg3 == 0) | (arg3 == 3)) {
        local_BC[1] = local_BC[1] + 0x8000;
      }
      Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
      Script_SetCharaMoveTarget(arg0, &local_E0, local_BC[1], script_41A4(arg5));
    }
  }
  return;
}

static void script_BAC8(int32_t arg0, DSWKDoorEntry *arg1, int32_t *arg2, DoorResourceDescriptor *arg3, MapObjectRecord *arg4, int32_t arg5, int32_t arg6) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t unused_F4;
  task_6C = arg1;
  task_70 = arg2;
  task_74 = arg3;
  task_78 = arg4;
  task_7C.mode = 0;
  task_7C.id = 0;
  task_7C.entry_idx = 0;
  task_1CC = arg0;
  (void)((task_1F0 = -1) & 1);
  task_1EC = 0;
  Script_motiLoad(Script_GetSceneID(), SHEN_NAME('FREE'), "M_MDOR.BIN");
  local_F8 = 0;
  L00BB32:;
  local_FC = Script_AddCharacterHandler(1, 0, local_F8 * 0x1000000 + 0x30726F64);
  Script_PreserveAccumulator_012C(local_FC, "This Charactor Work is NULL!!\012");
  DSWK_AllocDoorMapcRenderState(local_FC);
  Script_scenToggleVisibleFor(local_FC, 0);
  task_154[local_F8] = local_FC;
  task_8C[local_F8] = -1;
  task_B4[local_F8] = -1;
  task_DC[local_F8] = 0;
  task_104[local_F8] = 0;
  task_12C[local_F8] = 0;
  task_17C[local_F8] = 0;
  local_F8 = local_F8 + 1;
  if (!(local_F8 >= 10)) goto L00BB32;
  local_F8 = 0;
  for (;;) {
    if (Script_ScenDataUtil(task_6C, 4, local_F8 * 13) == -1) {
      break;
    }
    Script_ScenDataUtil(task_6C, 14, local_F8 * 13 + 1, 0);
    local_F8 = local_F8 + 1;
  }
  Script_EnqueueSCNT(0);
  DSWK_BindTablesAndInitState(arg1, arg2, arg3, arg4, &task_7C, task_8C, task_B4, (shen_name *)task_DC, task_104, arg0, arg5, data_1DD74, data_1E364, (shen_name *)task_154);
  Script_scenCreateTask(script_C384, 0);
  return;
}

static void script_BC54(DSWKDoorEntry *arg0, int32_t *arg1, DoorResourceDescriptor *arg2, MapObjectRecord *arg3, int32_t arg4) {
  task_6C = arg0;
  task_70 = arg1;
  task_74 = arg2;
  task_78 = arg3;
  DSWK_RebindDoorSystemTables(arg0, arg1, arg2, arg3, arg4);
  task_1EC = 1;
  return;
}

static void script_BC88(void) {
  int32_t local_FC;
  local_FC = 0;
  L00BC90:;
  task_8C[local_FC] = -1;
  task_B4[local_FC] = -1;
  task_DC[local_FC] = 0;
  task_104[local_FC] = 0;
  task_12C[local_FC] = 0;
  task_17C[local_FC] = 0;
  local_FC = local_FC + 1;
  if (!(local_FC >= 10)) goto L00BC90;
  return;
}

static void script_BCF1(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  local_F4 = 0;
  for (;;) {
    Script_sprintf("手動ドア開け番号指定\012");
    if (task_1CC == arg0) {
      Script_EVT_GetPosition(arg0, -1, &task_200, 0);
    }
    local_F8 = DSWK_DoorUtils(DOOR_CMD_ALLOC_ENTRY_SLOT, arg0, arg1);
    if (local_F8 != -1) {
      for (;;) {
        local_FC = DSWK_DoorUtils(DOOR_CMD_TYPE13, local_F8);
        if (local_FC != 2) {
          break;
        }
        SCNF_WaitFrames_0000(1);
      }
      if (local_FC != -2) {
        break;
      }
      local_F4 = local_F4 + 1;
      if (local_F4 >= 120) {
        Script_sprintf("無限ループになりそうなので抜ける\012");
        break;
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  (void)local_FC;
  return;
}

static void script_BD8E(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  Vector3 local_F0;
  if (task_1CC == arg0) {
    Script_EVT_GetPosition(arg0, -1, &task_200, 0);
  }
  Script_EVT_GetPosition(arg0, -1, &local_F0, 0);
  local_FC = DSWK_DoorUtils(DOOR_CMD_FIND_CLOSEST_ELEMENT, arg0, &local_F0, arg1);
  (void)local_FC;
  return;
}

static void script_BDC8(void) {
  int32_t local_FC;
  int32_t local_F8;
  local_FC = 0;
  for (;;) {
    if (Script_ScenDataUtil(task_6C, 4, local_FC * 13) == -1) {
      break;
    }
    local_F8 = Script_ScenDataUtil(task_6C, 4, local_FC * 13 + 1);
    local_F8 = local_F8 | 2;
    Script_ScenDataUtil(task_6C, 14, local_FC * 13 + 1, local_F8);
    local_FC = local_FC + 1;
  }
  return;
}

static void script_BE2B(void) {
  int32_t local_FC;
  int32_t local_F8;
  local_FC = 0;
  for (;;) {
    if (Script_ScenDataUtil(task_6C, 4, local_FC * 13) == -1) {
      break;
    }
    local_F8 = Script_ScenDataUtil(task_6C, 4, local_FC * 13 + 1);
    local_F8 = local_F8 & -3;
    Script_ScenDataUtil(task_6C, 14, local_FC * 13 + 1, local_F8);
    local_FC = local_FC + 1;
  }
  return;
}

static void script_BE8E(int32_t arg0) {
  int32_t local_FC;
  local_FC = Script_ScenDataUtil(task_6C, 4, arg0 * 13 + 1);
  local_FC = local_FC | 2;
  Script_ScenDataUtil(task_6C, 14, arg0 * 13 + 1, local_FC);
  return;
}

static void script_BEC5(int32_t arg0) {
  int32_t local_FC;
  local_FC = Script_ScenDataUtil(task_6C, 4, arg0 * 13 + 1);
  local_FC = local_FC & -3;
  Script_ScenDataUtil(task_6C, 14, arg0 * 13 + 1, local_FC);
  return;
}

static void script_BEFC(int32_t arg0) {
  Vector3 local_F4;
  int32_t local_F0;
  int32_t local_EC;
  local_F0 = 0;
  for (;;) {
    if (task_B4[local_F0] == arg0) {
      local_EC = task_154[local_F0];
      if (Script_shenGetNode(local_EC, 7)) {
        if (Script_shenGetNode(local_EC, 5) & Script_shenGetNode(local_EC, 10)) {
          if (Script_ScenDataUtil(task_6C, 4, task_B4[local_F0] * 13 + 1) & 0x100) {
            local_F4.x = 0.0f;
            (&local_F4)->y = 0.0f;
            (&local_F4)->z = 0.0f;
            Script_charaPosRotUtils(local_EC, 10, &local_F4, 0, 0);
            Script_charaPosRotUtils(local_EC, 5, &local_F4, 0, 0);
          } else {
            local_F4.x = 0.0f;
            (&local_F4)->y = -1200.0f;
            (&local_F4)->z = 0.0f;
            Script_charaPosRotUtils(local_EC, 10, &local_F4, 0, 0);
            Script_charaPosRotUtils(local_EC, 5, &local_F4, 0, 0);
          }
        }
      } else {
        if (Script_shenGetNode(local_EC, 10)) {
          if (Script_ScenDataUtil(task_6C, 4, task_B4[local_F0] * 13 + 1) & 0x100) {
            local_F4.x = 0.0f;
            (&local_F4)->y = 0.0f;
            (&local_F4)->z = 0.0f;
            Script_charaPosRotUtils(local_EC, 10, &local_F4, 0, 0);
          } else {
            local_F4.x = 0.0f;
            (&local_F4)->y = -1200.0f;
            (&local_F4)->z = 0.0f;
            Script_charaPosRotUtils(local_EC, 10, &local_F4, 0, 0);
          }
        }
      }
      if (Script_shenGetNode(local_EC, 8) & Script_shenGetNode(local_EC, 13)) {
        if (Script_ScenDataUtil(task_6C, 4, task_B4[local_F0] * 13 + 1) & 0x20) {
          local_F4.x = 0.0f;
          (&local_F4)->y = 0.0f;
          (&local_F4)->z = 0.0f;
          Script_charaPosRotUtils(local_EC, 13, &local_F4, 0, 0);
          (&local_F4)->y = -1000.0f;
          Script_charaPosRotUtils(local_EC, 8, &local_F4, 0, 0);
        } else {
          local_F4.x = 0.0f;
          (&local_F4)->y = -1000.0f;
          (&local_F4)->z = 0.0f;
          Script_charaPosRotUtils(local_EC, 13, &local_F4, 0, 0);
          (&local_F4)->y = 0.0f;
          Script_charaPosRotUtils(local_EC, 8, &local_F4, 0, 0);
        }
      }
    }
    local_F0 = local_F0 + 1;
    if (local_F0 == 10) {
      break;
    }
  }
  return;
}

static void script_C13E(int32_t arg0) {
  int32_t local_FC;
  local_FC = Script_ScenDataUtil(task_6C, 4, arg0 * 13 + 1);
  local_FC = local_FC | 0x20;
  Script_ScenDataUtil(task_6C, 14, arg0 * 13 + 1, local_FC);
  script_C246(arg0);
  script_BEFC(arg0);
  return;
}

static void script_C187(int32_t arg0) {
  int32_t local_FC;
  local_FC = Script_ScenDataUtil(task_6C, 4, arg0 * 13 + 1);
  local_FC = local_FC & -33;
  Script_ScenDataUtil(task_6C, 14, arg0 * 13 + 1, local_FC);
  script_C254(arg0);
  script_BEFC(arg0);
  return;
}

static void script_C1D0(int32_t arg0, int32_t arg1) {
  if (arg1) {
    int32_t local_FC;
    local_FC = Script_ScenDataUtil(task_6C, 4, arg0 * 13 + 1);
    local_FC = local_FC & -257;
    Script_ScenDataUtil(task_6C, 14, arg0 * 13 + 1, local_FC);
  } else {
    int32_t local_FC;
    local_FC = Script_ScenDataUtil(task_6C, 4, arg0 * 13 + 1);
    local_FC = local_FC | 0x100;
    Script_ScenDataUtil(task_6C, 14, arg0 * 13 + 1, local_FC);
  }
  return;
}

static void script_C246(int32_t arg0) {
  script_C1D0(arg0, 0);
  return;
}

static void script_C254(int32_t arg0) {
  script_C1D0(arg0, 1);
  return;
}

static void script_C262(int32_t arg0) {
  int32_t local_FC;
  local_FC = 0;
  for (;;) {
    if (Script_ScenDataUtil(task_6C, 4, local_FC * 13) == -1) {
      break;
    }
    if (arg0) {
      int32_t local_F8;
      local_F8 = Script_ScenDataUtil(task_6C, 4, local_FC * 13 + 1);
      local_F8 = local_F8 & -65;
      local_F8 = local_F8 & -129;
      local_F8 = local_F8 | 0x40;
      Script_ScenDataUtil(task_6C, 14, local_FC * 13 + 1, local_F8);
    } else {
      int32_t local_F8;
      local_F8 = Script_ScenDataUtil(task_6C, 4, local_FC * 13 + 1);
      local_F8 = local_F8 & -65;
      local_F8 = local_F8 & -129;
      local_F8 = local_F8 | 0x80;
      Script_ScenDataUtil(task_6C, 14, local_FC * 13 + 1, local_F8);
    }
    local_FC = local_FC + 1;
  }
  return;
}

static void script_C329(void) {
  script_C262(1);
  return;
}

static void script_C335(void) {
  script_C262(0);
  return;
}

static void script_C341(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5, int32_t arg6) {
  int32_t local_FC;
  local_FC = 0;
  for (;;) {
    if (Script_ScenDataUtil(task_6C, 4, local_FC * 13) == -1) {
      break;
    }
    DSWK_AssignDoorEntryConfig(local_FC, arg0, arg1, arg2, arg3, arg4, arg5, arg6);
    local_FC = local_FC + 1;
  }
  return;
}

static int32_t script_C384(void) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  SCNF_WaitFrames_0000(1);
  SCNF_WaitFrames_0000(1);
  SCNF_WaitFrames_0000(1);
  local_F4 = -1;
  local_F8 = -1;
  for (;;) {
    if (task_1EC == 0) {
      local_F4 = DSWK_DoorUtils(DOOR_CMD_FIND_LOWEST_FLOAT_INDEX, 0x3FD851EA);
      if (-(local_F4 != -1) & -(task_8C[local_F4] == 1) & Script_IsPlayerAllowed(0) & -((&task_7C)->mode == 0)) {
        local_FC = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_B4[local_F4], 2);
        local_F8 = Script_ScenDataUtil(task_74, 4, Script_ScenDataUtil(task_78, 4, local_FC * 9 + 1) * 4 + 1);
        script_D154(1, local_FC, local_F4, local_F8);
        if (script_D308(local_FC, local_F4, local_F8)) {
          local_F4 = -1;
          local_F8 = -1;
        }
      } else {
        script_D154(0, local_FC, local_F4, local_F8);
        local_F4 = -1;
        local_F8 = -1;
      }
      script_D51E(local_F4, local_F8);
    } else {
      task_1EC = 0;
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int8_t script_C495(int32_t arg0, int32_t arg1, int8_t arg2) {
  Vector3 local_F4;
  Vector3 local_E8;
  Vector3i local_DC;
  int32_t local_D8;
  arg2 = 0;
  Script_EVT_GetPosition(task_1CC, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  Script_mtwkGetSetRotation(task_1CC, &local_DC, 0x42000000);
  local_E8.x = (&local_F4)->x;
  (&local_E8)->y = (&local_F4)->y;
  (&local_E8)->z = (&local_F4)->z;
  if (DSWK_DoorUtils(DOOR_CMD_CALC_VECTOR_FROM_ANGLE, arg0, &local_E8, &local_D8)) {
    local_D8 = ((&local_DC)->y - local_D8) & 0xFFFF;
    if ((local_D8 <= 21845) | (local_D8 >= 43691)) {
      if (DSWK_DoorUtils(DOOR_CMD_TEST_LINE_INTERSECTION, &local_F4, &local_E8) == 0) {
        arg2 = 1;
      }
    }
  }
  return arg2;
}

__attribute__((scn_string_bindings(0, data_1F9D5, 1, data_1F9EE)))
static void script_C52D(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  Vector3 local_EC;
  Vector3i local_E0;
  local_F8 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_B4[arg0], 1);
  if (local_F8 != -1) {
    local_FC = Script_AddCharacterHandler(1, 0, arg0 * 0x1000000 + 0x30786F62);
    if (local_FC == 0) {
      Script_PreserveAccumulator_012C(local_FC, "Don't make BOX Buffer!!\012");
      task_1A4[arg0] = -1;
    } else if (local_FC == arg0 * 0x1000000 + 0x30786F62) {
      task_1A4[arg0] = local_FC;
      *(int32_t *)&local_EC.x = Script_ScenDataUtil(task_78, 4, local_F8 * 9 + 5);
      *(int32_t *)&(&local_EC)->y = Script_ScenDataUtil(task_78, 4, local_F8 * 9 + 6);
      *(int32_t *)&(&local_EC)->z = Script_ScenDataUtil(task_78, 4, local_F8 * 9 + 7);
      Script_EVT_SetPosition(local_FC, POS_XYZ, &local_EC);
      local_E0.x = 0;
      (&local_E0)->y = Script_ScenDataUtil(task_78, 4, local_F8 * 9 + 8);
      (&local_E0)->z = 0;
      Script_EVT_SetRotation(local_FC, 0x38000000, &local_E0, 0);
      *(int32_t *)&local_EC.x = Script_ScenDataUtil(task_78, 4, local_F8 * 9 + 2);
      *(int32_t *)&(&local_EC)->y = Script_ScenDataUtil(task_78, 4, local_F8 * 9 + 3);
      *(int32_t *)&(&local_EC)->z = Script_ScenDataUtil(task_78, 4, local_F8 * 9 + 4);
      Script_hmdlApplyScale(local_FC, &local_EC);
      Script_CreateNPCImageModule(local_FC, (const char *)Script_ScenDataUtil(task_74, 8, Script_ScenDataUtil(task_78, 4, local_F8 * 9 + 1) * 4), 0);
      Script_charaDisplay(local_FC, CDM_Show);
      Script_charaVisUtils(local_FC, 1);
    } else {
      Script_PreserveAccumulator_012C(local_FC, "Don't make BOX Buffer!!\012");
      task_1A4[arg0] = -1;
    }
  } else {
    task_1A4[arg0] = -1;
  }
  return;
}

static void script_C6FC(int32_t arg0) {
  if (task_1A4[arg0] != -1) {
    Script_SetMapControlTaskFlag(task_1A4[arg0], 1);
    task_1A4[arg0] = -1;
  }
  return;
}

static int32_t script_C72D(int32_t arg0, int32_t arg1, int32_t arg2, float arg3, float arg4, int32_t arg5, int32_t arg6) {
  Vector3 local_F4;
  Vector3 local_E8;
  float local_DC[3];
  Vector3i local_D0;
  Vector3i local_C4;
  int32_t local_C0;
  int32_t local_BC;
  if (task_1CC == arg0) {
    local_F4.x = (&task_200)->x;
    (&local_F4)->y = task_200.y;
    (&local_F4)->z = task_200.z;
  } else {
    Script_EVT_GetPosition(arg0, -1, &local_F4, 0);
  }
  Script_EVT_GetPosition(arg1, -1, &local_E8, 0);
  Script_mtwkGetSetRotation(arg1, &local_D0, 0);
  local_C4.x = (&local_D0)->x;
  (&local_C4)->y = (&local_D0)->y;
  (&local_C4)->z = (&local_D0)->z;
  local_C0 = Script_FloatUtils(ScriptMathOp_Atan2ReverseVector, (&local_F4)->x - (&local_E8)->x, (&local_F4)->z - (&local_E8)->z);
  local_C0 = ((local_C0 - (&local_D0)->y) >> 14) & 3;
  if (arg2 != 99) {
    if (arg2 == 6) {
      if ((local_C0 == 0) | (local_C0 == 1)) {
        if ((Script_ScenDataUtil(task_6C, 4, arg6 * 13 + 1) & 8) | (Script_ScenDataUtil(task_6C, 4, arg6 * 13 + 1) & 0x40)) {
          local_C0 = local_C0 ^ 3;
        }
      } else {
        if ((Script_ScenDataUtil(task_6C, 4, arg6 * 13 + 1) & 0x10) | (Script_ScenDataUtil(task_6C, 4, arg6 * 13 + 1) & 0x80)) {
          local_C0 = local_C0 ^ 3;
        }
      }
      arg2 = 4;
    }
    {
      int32_t unused_B8;
      int32_t local_B4;
      local_B4 = arg5;
      if ((local_B4 == 0) | (local_B4 == 1) | (local_B4 == 3) | (local_B4 == 5) | (local_B4 == 7) | (local_B4 == 9) | (local_B4 == 11) | (local_B4 == 25) | (local_B4 == 27) | (local_B4 == 30) | (local_B4 == 32) | (local_B4 == 34) | (local_B4 == 36) | (local_B4 == 37)) {
        local_B4 = local_C0;
        if ((local_B4 == 0) | (local_B4 == 3)) {
          arg4 = (float)0 - arg4;
          (&local_C4)->y = (&local_C4)->y + 0x8000;
        } else {
          if ((local_B4 == 1) | (local_B4 == 2)) goto L00C918;
        }
        L00C918:;
      } else if ((local_B4 == 2) | (local_B4 == 4) | (local_B4 == 6) | (local_B4 == 8) | (local_B4 == 10) | (local_B4 == 12) | (local_B4 == 24) | (local_B4 == 26) | (local_B4 == 31) | (local_B4 == 33) | (local_B4 == 35)) {
        local_B4 = local_C0;
        if ((local_B4 == 2) | (local_B4 == 1)) {
          arg3 = (float)0 - arg3;
        } else {
          if ((local_B4 == 3) | (local_B4 == 0)) {
            arg3 = (float)0 - arg3;
            arg4 = (float)0 - arg4;
            (&local_C4)->y = (&local_C4)->y + 0x8000;
            goto L00C9BB;
          }
        }
        L00C9BB:;
      } else if ((local_B4 == 13) | (local_B4 == 14) | (local_B4 == 15) | (local_B4 == 16) | (local_B4 == 17) | (local_B4 == 18) | (local_B4 == 19) | (local_B4 == 20) | (local_B4 == 21) | (local_B4 == 22) | (local_B4 == 23) | (local_B4 == 29)) {
        local_B4 = local_C0;
        switch (local_B4) {
          case 0:
            arg3 = (float)0 - arg3;
            arg4 = (float)0 - arg4;
            (&local_C4)->y = (&local_C4)->y + 0x8000;
            break;
          case 1:
            arg3 = (float)0 - arg3;
            break;
          case 2:
            break;
          case 3:
            arg4 = (float)0 - arg4;
            (&local_C4)->y = (&local_C4)->y + 0x8000;
            break;
        }
      } else if (local_B4 == 28) {
        local_B4 = local_C0;
        switch (local_B4) {
          case 0:
            arg3 = (float)0 - arg3;
            arg4 = (float)0 - arg4;
            (&local_C4)->y = (&local_C4)->y + 0x8000;
            break;
          case 1:
            arg3 = (float)0 - arg3;
            break;
          case 2:
            break;
          case 3:
            arg4 = (float)0 - arg4;
            (&local_C4)->y = (&local_C4)->y + 0x8000;
            break;
        }
      } else {
        local_B4 = local_C0;
        if ((local_B4 == 2) | (local_B4 == 1)) {
        } else {
          if ((local_B4 == 3) | (local_B4 == 0)) {
            arg4 = (float)0 - arg4;
            (&local_C4)->y = (&local_C4)->y + 0x8000;
            goto L00CB67;
          }
        }
      }
    }
    L00CB67:;
    local_BC = (&local_D0)->y & 0xFFFF;
    local_DC[2] = arg4 * Script_MathUtils(SCRIPT_MATH_COS_FIXED16, local_BC) - arg3 * Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, local_BC);
    local_DC[0] = arg4 * Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, local_BC) + arg3 * Script_MathUtils(SCRIPT_MATH_COS_FIXED16, local_BC);
    if (arg2 == -1) {
      int32_t unused_B4[2];
      Vector3 local_A8;
      int32_t local_A4;
      local_A8.x = (&local_E8)->x + *local_DC;
      (&local_A8)->y = (&local_E8)->y;
      (&local_A8)->z = (&local_E8)->z + local_DC[2];
      local_A4 = DSWK_DoorUtils(DOOR_CMD_TEST_LINE_INTERSECTION, &local_F4, &local_A8);
      if ((local_C0 == 1) | (local_C0 == 2)) {
        local_C0 = 1;
      } else {
        local_C0 = 0;
      }
      if (local_A4) {
        local_C0 = -1;
      }
    } else {
      int32_t unused_B8;
      int32_t local_B4;
      local_F4.x = (&local_E8)->x + *local_DC;
      (&local_F4)->y = (&local_E8)->y;
      (&local_F4)->z = (&local_E8)->z + local_DC[2];
      local_B4 = arg2;
      switch (local_B4) {
        case 0:
          Script_EVT_SetPosition(arg0, POS_XYZ, &local_F4);
          Script_EVT_SetRotation(arg0, 0x38000000, &local_C4, 0);
          break;
        case 2:
          Script_EVT_SetPosition(arg0, POS_XYZ, &local_F4);
          Script_EVT_SetRotation(arg0, 0x38000000, &local_C4, 0);
          if ((local_C0 == 1) | (local_C0 == 2)) {
            local_C0 = 1;
          } else {
            local_C0 = 0;
          }
          break;
        case 3:
          if ((local_C0 == 1) | (local_C0 == 2)) {
            local_C0 = 1;
          } else {
            local_C0 = 0;
          }
          break;
        case 4:
          task_1D4.x = (&local_F4)->x;
          task_1D4.y = (&local_F4)->y;
          task_1D4.z = (&local_F4)->z;
          task_1E0 = (&local_C4)->x;
          *(&task_1E0 + 1) = (&local_C4)->y;
          *(&task_1E0 + 2) = (&local_C4)->z;
          break;
        case 5:
          task_1D4.x = (&local_F4)->x;
          task_1D4.y = (&local_F4)->y;
          task_1D4.z = (&local_F4)->z;
          task_1E0 = (&local_C4)->x;
          *(&task_1E0 + 1) = (&local_C4)->y;
          *(&task_1E0 + 2) = (&local_C4)->z;
          if ((local_C0 == 1) | (local_C0 == 2)) {
            local_C0 = 1;
          } else {
            local_C0 = 0;
          }
          break;
        case 7:
          Script_EVT_GetPosition(arg0, -1, &task_1D4, 0);
          task_1E0 = (&local_C4)->x;
          *(&task_1E0 + 1) = (&local_C4)->y;
          *(&task_1E0 + 2) = (&local_C4)->z;
          break;
      }
    }
  }
  return local_C0;
}

static void script_CDF6(int32_t arg0, int32_t arg1) {
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
    if (task_1CC == arg0) {
      if (arg1) {
        Script_ccowUtils(arg0, 3);
      } else {
        Script_ccowUtils(arg0, 4);
      }
    }
  }
  return;
}

static void script_CE28(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  int32_t local_F8;
  local_F8 = script_C72D(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
  if (task_1CC != arg0) {
    if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
      Script_charaToggleMotEnabled(arg0, 1);
    }
  } else if ((local_F8 == 0) | (local_F8 == 1)) {
    if ((Script_ScenDataUtil(task_6C, 4, arg2 * 13 + 1) & 8) == 0) {
      if ((Script_ScenDataUtil(task_6C, 4, arg2 * 13 + 1) & 0x40) != 0) {
        local_FC = Script_ScenDataUtil(task_6C, 4, arg2 * 13 + 2) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_FC, 0);
      } else {
        local_FC = (Script_ScenDataUtil(task_6C, 4, arg2 * 13 + 2) >> 16) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_FC, 0);
      }
    }
  } else {
    if ((Script_ScenDataUtil(task_6C, 4, arg2 * 13 + 1) & 0x10) == 0) {
      if ((Script_ScenDataUtil(task_6C, 4, arg2 * 13 + 1) & 0x80) != 0) {
        local_FC = (Script_ScenDataUtil(task_6C, 4, arg2 * 13 + 2) >> 16) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_FC, 0);
      } else {
        local_FC = Script_ScenDataUtil(task_6C, 4, arg2 * 13 + 2) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_FC, 0);
      }
    }
  }
  return;
}

__attribute__((scn_string_bindings(0, data_1FA07)))
static void script_CF98(int32_t arg0, int32_t arg1, int32_t arg2) {
  int32_t local_FC;
  if (task_1CC == arg0) {
    task_7C.mode = 0;
    task_7C.id = 0;
    if ((arg2 == 0) | (arg2 == 2)) {
      Script_PlayCharaMotion(task_1CC, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
      if (task_1F0 & 4) {
        Script_sprintf("Door System : ");
        Script_ryoControl(RYO_RestoreControl);
      }
      Script_shenClearFreeState(1);
      if (arg2 != 2) {
        int32_t local_F8;
        local_F8 = task_B4[arg1];
        local_FC = Script_ScenDataUtil(task_6C, 4, local_F8 * 13 + 2) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_FC, 1);
        local_FC = (Script_ScenDataUtil(task_6C, 4, local_F8 * 13 + 2) >> 16) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_FC, 1);
      }
    }
  } else {
    if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
      Script_charaToggleMotEnabled(arg0, 0);
    }
    if ((arg2 == 0) | (arg2 == 2)) {
      int32_t local_F8;
      local_F8 = task_B4[arg1];
      local_FC = Script_ScenDataUtil(task_6C, 4, local_F8 * 13 + 2) & 0xFFFF;
      Script_scenToggleCharaFlag(0, local_FC, 1);
      local_FC = (Script_ScenDataUtil(task_6C, 4, local_F8 * 13 + 2) >> 16) & 0xFFFF;
      Script_scenToggleCharaFlag(0, local_FC, 1);
    }
  }
  task_8C[arg1] = 1;
  task_17C[arg1] = 0;
  task_12C[arg1] = 0;
  task_DC[arg1] = 0;
  if (task_1A4[arg1] != -1) {
    Script_SetMapControlTaskFlag(task_1A4[arg1], 1);
    task_1A4[arg1] = -1;
  }
  return;
}

static void script_D154(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  Vector3 local_E8;
  local_FC = 0;
  if (arg0 == 1) {
    local_F4 = arg3;
    if ((local_F4 == 17) | (local_F4 == 11) | (local_F4 == 12)) {
      local_FC = 0;
    } else if ((Script_ScenDataUtil(task_6C, 4, task_B4[arg2] * 13 + 1) & 2) == 0) {
      if ((Script_ScenDataUtil(task_6C, 4, task_B4[arg2] * 13 + 1) & 4) == 0) {
        local_FC = script_C495(arg2, arg3, local_FC);
      } else {
        local_F4 = arg3;
        if ((local_F4 == 21) | (local_F4 == 22) | (local_F4 == 23)) {
          local_FC = script_C495(arg2, arg3, local_FC);
          goto L00D218;
        }
      }
      L00D218:;
    } else {
      if ((Script_ScenDataUtil(task_6C, 4, task_B4[arg2] * 13 + 1) & 4) == 0) {
        local_FC = script_C495(arg2, arg3, local_FC);
        local_F8 = Script_ScenDataUtil(task_6C, 4, task_B4[arg2] * 13 + 6);
        if (local_F8 == 0) {
          local_FC = 0;
        } else {
          if (local_F8 >= 2) {
            Script_EVT_GetPosition(task_1CC, -1, &local_E8, 0);
            if (Script_CycleManagerUtils(CYCLE_CMD_COUNT_CYCLES_IN_RADIUS, &local_E8, 1.5f)) {
              local_FC = 0;
            }
            goto L00D2B3;
          }
        }
      }
    }
  }
  L00D2B3:;
  if (task_17C[arg2]) {
    local_FC = 0;
  }
  Script_scenUtils(SCENARIO_CMD_SET_HELP_PROMPT_OVERRIDE, local_FC);
  if ((local_FC ^ task_1FC) & local_FC) {
    Script_sicnUtils(0, 4, 1);
  }
  if ((local_FC ^ task_1FC) & task_1FC) {
    Script_sicnUtils(2, 4, 0);
  }
  task_1FC = local_FC;
  return;
}

__attribute__((scn_string_bindings(0, data_1FA16)))
static int32_t script_D308(int32_t arg0, int32_t arg1, int32_t arg2) {
  int32_t local_FC;
  int32_t local_F8;
  Vector3 local_EC;
  local_FC = 1;
  local_F8 = arg2;
  if ((local_F8 == 17) | (local_F8 == 11) | (local_F8 == 12)) {
    if (((Script_ScenDataUtil(task_6C, 4, task_B4[arg1] * 13 + 1) & 2) == 0) & ((Script_ScenDataUtil(task_6C, 4, task_B4[arg1] * 13 + 1) & 4) == 0)) {
      if (task_17C[arg1] == 0) {
        task_8C[arg1] = 4;
        local_FC = 0;
      }
    }
  } else {
    if (Script_GetControllerInput(1) == 0x400) {
      if (-(task_DC[arg1] != task_1CC) & task_1FC) {
        if ((Script_ScenDataUtil(task_6C, 4, task_B4[arg1] * 13 + 1) & 2) == 0) {
          task_8C[arg1] = 3;
          task_DC[arg1] = task_1CC;
          task_104[arg1] = 0;
          task_7C.mode = 1;
          task_7C.entry_idx = task_B4[arg1];
          local_FC = 0;
        } else {
          if (Script_ScenDataUtil(task_6C, 4, task_B4[arg1] * 13 + 6) >= 2) {
            Script_EVT_GetPosition(task_1CC, -1, &local_EC, 0);
            if (Script_CycleManagerUtils(CYCLE_CMD_COUNT_CYCLES_IN_RADIUS, &local_EC, 1.5f) == 0) {
              task_8C[arg1] = 6;
              task_DC[arg1] = task_1CC;
              task_104[arg1] = 0;
              task_7C.mode = 0;
              task_7C.entry_idx = task_B4[arg1];
              local_FC = 0;
            }
            goto L00D4BC;
          }
        }
        L00D4BC:;
        if (local_FC == 0) {
          Script_EVT_GetPosition(task_1CC, -1, &task_200, 0);
          Script_ryoControl(PCCTL_RequestNewMode, 4);
          Script_sprintf("Door System : ");
          Script_shenClearFreeState(0);
          Script_ryoControl(PCCTL_DisablePlayerControl);
          task_7C.id = task_1CC;
          while (Script_ryoControl(PCCTL_QueryRequestIdle) == 0) {
            SCNF_WaitFrames_0000(1);
          }
          Script_ryoControl(PCCTL_RequestNewMode, 0);
        }
      }
    }
  }
  end:;
}

static void script_D51E(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  int32_t local_F8;
  local_FC = 0;
  L00D526:;
  local_F8 = task_8C[local_FC];
  switch (local_F8) {
    case 6:
      if (!(arg0 != -1)) goto L00D546;
      L00D546:;
      script_D5E8(local_FC, arg1);
      break;
    case 5:
      if (!(arg0 != -1)) goto L00D566;
      L00D566:;
      script_D649(local_FC);
      break;
    case 4:
      if (!(arg0 != -1)) goto L00D584;
      L00D584:;
      script_D7EE(local_FC, arg0, arg1);
      break;
    case 3:
      if (!(arg0 != -1)) goto L00D5A6;
      L00D5A6:;
      script_D886(local_FC, arg1);
      break;
    case 2:
      script_D968(local_FC);
      break;
    case 1:
      break;
  }
  local_FC = local_FC + 1;
  if (!(local_FC >= 10)) goto L00D526;
  return;
}

static void script_D5E8(int32_t arg0, int32_t arg1) {
  if (task_17C[arg0] == 0) {
    task_17C[arg0] = Script_scenCreateTask(script_5783, 4, task_DC[arg0], task_154[arg0], arg0, arg1);
    task_12C[arg0] = task_DC[arg0];
    script_C52D(arg0);
  }
  return;
}

__attribute__((scn_string_bindings(0, data_1FA25, 1, data_1FA37)))
static void script_D649(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, task_DC[arg0])) {
    if (task_17C[arg0] == 0) {
      if (task_DC[arg0] == task_1CC) {
        task_7C.mode = 1;
        task_7C.entry_idx = task_B4[arg0];
        goto L00D69E;
      }
      L00D69E:;
      local_FC = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_B4[arg0], 2);
      local_F8 = Script_ScenDataUtil(task_74, 4, Script_ScenDataUtil(task_78, 4, local_FC * 9 + 1) * 4 + 1);
      script_CE28(task_DC[arg0], task_154[arg0], task_B4[arg0], local_F8);
      task_17C[arg0] = Script_scenCreateTask(Script_ScenDataUtil(&data_1EE24, 8, local_F8), 4, task_DC[arg0], task_154[arg0], arg0, local_F8);
      task_12C[arg0] = task_DC[arg0];
      if ((Script_ScenDataUtil(task_6C, 4, task_B4[arg0] * 13 + 1) & 0x20000) == 0) {
        Script_sprintf("Door Box is ON!!\012");
        script_C52D(arg0);
      } else {
        Script_sprintf("Door Box is OFF!!\012");
        task_1A4[arg0] = -1;
      }
    }
  } else {
    if (task_8C[arg0] != 5) {
      task_8C[arg0] = 1;
      task_17C[arg0] = 0;
      task_12C[arg0] = 0;
      task_DC[arg0] = 0;
    }
  }
  return;
}

static void script_D7EE(int32_t arg0, int32_t arg1, int32_t arg2) {
  if ((task_17C[arg0] == 0) & (arg2 >= 0)) {
    task_DC[arg0] = task_1CC;
    task_7C.mode = 2;
    task_7C.entry_idx = task_B4[arg0];
    task_17C[arg0] = Script_scenCreateTask(Script_ScenDataUtil(&data_1EE24, 8, arg2), 4, task_DC[arg0], task_154[arg0], arg0, arg2);
    task_12C[arg0] = task_DC[arg0];
    script_C52D(arg0);
  }
  return;
}

__attribute__((scn_string_bindings(0, data_1FA4A, 1, data_1FA5C)))
static void script_D886(int32_t arg0, int32_t arg1) {
  if ((task_17C[arg0] == 0) & (arg1 >= 0)) {
    script_CE28(task_DC[arg0], task_154[arg0], task_B4[arg0], arg1);
    task_17C[arg0] = Script_scenCreateTask(Script_ScenDataUtil(&data_1EE24, 8, arg1), 4, task_DC[arg0], task_154[arg0], arg0, arg1);
    task_12C[arg0] = task_DC[arg0];
    if ((Script_ScenDataUtil(task_6C, 4, task_B4[arg0] * 13 + 1) & 0x20000) == 0) {
      Script_sprintf("Door Box is ON!!\012");
      script_C52D(arg0);
    } else {
      Script_sprintf("Door Box is OFF!!\012");
      task_1A4[arg0] = -1;
    }
  }
  return;
}

static void script_D968(int32_t arg0) {
  int32_t local_FC;
  task_17C[arg0] = 0;
  task_12C[arg0] = 0;
  task_8C[arg0] = 1;
  local_FC = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_B4[arg0], 2);
  if (local_FC != -1) {
    task_DC[arg0] = 0;
  }
  return;
}

static void script_D9BF(void) {
  Script_scenSetMapEvTbl((uint32_t *)&data_2007C, 2);
  return;
}

static void script_D9CD(void) {
  return;
}

static void script_D9D0(int32_t arg0) {
  if (arg0 == 0x52494B41) {
    if (Script_IsPlayerAllowed(0)) {
      script_1BDA5(3, SHEN_NAME('JD00'), 2, 1);
    }
  }
  return;
}

static void script_D9FB(void) {
  Vector3 local_F4;
  int32_t local_F0;
  Script_EVT_GetPosition(CHARA_AKIR, -1, &local_F4, 0);
  local_F0 = Script_UpdateTaskCollisionCache(0, 1, &local_F4);
  if (local_F0 != -1) {
    Script_PlayerStatsUtils(PS_IncrementSpotVisitCount, local_F0);
  }
  return;
}

static void script_DA2F(void) {
  Script_ambSoundUtils(1, 0, &data_20084, 10.0f, 30.0f, 0.8f, 2, &data_200FC);
  Script_ambSoundUtils(2, 0, &data_20090, 10.0f, 30.0f, 0.8f, 2, &data_20184);
  Script_ambSoundUtils(3, 0, &data_2009C, 20.0f, 30.0f, 0.8f, 2, &data_2020C);
  Script_ambSoundUtils(4, 0, &data_200A8, 15.0f, 30.0f, 0.8f, 2, &data_20294);
  Script_ambSoundUtils(5, 0, &data_200B4, 10.0f, 30.0f, 0.8f, 2, &data_2031C);
  Script_ambSoundUtils(6, 0, &data_200C0, 15.0f, 30.0f, 0.8f, 2, &data_203A4);
  Script_ambSoundUtils(7, 1, &data_200CC, 50.0f, 30.0f, 0.8f, 0, data_2042C);
  Script_ambSoundUtils(8, 1, data_200D8, 50.0f, 30.0f, 0.8f, 0, data_204B4);
  Script_ambSoundUtils(9, 1, data_200E4, 50.0f, 30.0f, 0.8f, 0, &data_2053C);
  Script_ambSoundUtils(15, 1, data_200F0, 200.0f, 50.0f, 1.0f, 3, &data_205C4);
  Script_Ambient1SoundUtils(0, 23976);
  return;
}

static void script_DBA4(void) {
  Script_ryoControl(PCCTL_DisablePlayerControl);
  return;
}

static void script_DBAD(void) {
  Script_ryoControl(RYO_RestoreControl);
  Script_SetGameStateTo(AUTH_None);
  return;
}

static void script_DBBC(int32_t arg0) {
  Script_ccowUtils(arg0, 2);
  if (0) {
    Script_charaDisplay(arg0, CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(arg0, CDM_Show);
      goto L00DBE8;
    }
  }
  L00DBE8:;
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

static void script_DC49(int32_t arg0) {
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

static void script_DC99(void) {
  Script_filmCheckOrSetTransitionUtil(80, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_DCC7(int32_t arg0) {
  Script_filmCheckOrSetTransitionUtil(arg0, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_DCF5(void) {
  Script_filmCheckOrSetTransitionUtil(80, 255, 255, 255, 255, 0, 255, 255, 255);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_DD29(int32_t arg0) {
  Script_filmCheckOrSetTransitionUtil(arg0, 255, 255, 255, 255, 0, 255, 255, 255);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_DD5D(int32_t arg0) {
  Script_filmCheckOrSetTransitionUtil(arg0, 255, 255, 255, 255, 150, 68, 50, 0);
  return;
}

static void script_DD7B(int32_t arg0, int32_t arg1, int32_t arg2) {
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

static void script_DDB5(int32_t arg0, int32_t arg1) {
  while (arg1 > 0) {
    Script_SetFightProgramStateBytes(arg0, arg1, 1);
    arg1 = arg1 - 1;
    SCNF_WaitFrames_0000(1);
  }
  Script_GetDataFromFightProgram(arg0, 0);
  return;
}

static void script_DDE9(void) {
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

static void script_DEE1(int32_t arg0, int32_t arg1) {
  Script_FACE_LCG(arg0, 1, arg1, 0);
  SCNF_WaitFrames_0000(arg1 + 1);
  Script_FACE_LCG(arg0, 2, arg1, 0);
  SCNF_WaitFrames_0000(arg1);
  return;
}

static void script_DF11(void) {
  int32_t local_FC;
  int32_t unused_F8;
  Vector3 local_EC;
  Vector3 local_E0;
  Vector3i local_D4;
  int32_t local_C8[3];
  Vector3i local_BC;
  Vector3i local_B0;
  int32_t local_AC;
  local_EC.x = (&local_EC)->y = (&local_EC)->z = 0.0f;
  local_E0.x = (&local_E0)->y = (&local_E0)->z = 0.0f;
  script_DBA4();
  script_FF79();
  Script_shenSeqUtils(0);
  local_FC = 0;
  local_C8[0] = -1274;
  local_C8[1] = 910;
  local_C8[2] = -1820;
  local_BC.x = 0;
  (&local_BC)->y = 0;
  (&local_BC)->z = 0;
  local_B0.x = 0;
  (&local_B0)->y = 0;
  (&local_B0)->z = 0;
  while (Script_shenSeqUtils(-1)) {
    local_AC = local_FC;
    switch (local_AC) {
      case 0:
        Script_executeAicaCommand(17576, 0, 0);
        Script_executeAicaCommand(1184, 2, 78);
        Script_executeAicaCommand(1184, 4, 127);
        Script_executeAicaCommand(1184, 1, 88);
        Script_SetMaxInterpSpeed(0.0f);
        Script_SetMotnFlag(CHARA_MEGM, -1);
        Script_EVT_GetPosition(CHARA_MEGM, -1, &local_EC, 0);
        Script_ResetHandMotion(CHARA_AKIR, 1, 1, 0);
        Script_ResetHandMotion(CHARA_AKIR, 1, 0, 0);
        Script_handSetFloatsFor(CHARA_AKIR, 1, data_211E0, 1);
        Script_handSetFloatsFor(CHARA_AKIR, 0, data_211E0, 1);
        Script_SetHandRootRotation(CHARA_AKIR, 1, 0x15, &data_2148C);
        Script_SetHandRootRotation(CHARA_AKIR, 0, 0x15, &data_2148C);
        Script_FACE(CHARA_AKIR, 1, 0, 1);
        local_EC.x = -39.451801f;
        (&local_EC)->y = 4.31740046f;
        (&local_EC)->z = 38.2202f;
        local_D4.x = 0;
        (&local_D4)->y = 0;
        (&local_D4)->z = 0;
        Script_EVT_SetPosition(SHEN_NAME('NBO1'), POS_XYZ, &local_EC);
        Script_EVT_SetRotation(SHEN_NAME('NBO1'), 0x38000000, &local_D4, 0);
        local_EC.x = -39.436699f;
        (&local_EC)->z = 38.2463f;
        (&local_D4)->y = 3640;
        Script_EVT_SetPosition(SHEN_NAME('NBO2'), POS_XYZ, &local_EC);
        Script_EVT_SetRotation(SHEN_NAME('NBO2'), 0x38000000, &local_D4, 0);
        local_EC.x = -39.455601f;
        (&local_EC)->z = 38.214401f;
        (&local_D4)->y = 7281;
        Script_EVT_SetPosition(SHEN_NAME('NBO3'), POS_XYZ, &local_EC);
        Script_EVT_SetRotation(SHEN_NAME('NBO3'), 0x38000000, &local_D4, 0);
        local_EC.x = -39.452801f;
        (&local_EC)->z = 38.220299f;
        (&local_D4)->y = 1092;
        Script_EVT_SetPosition(SHEN_NAME('NBO4'), POS_XYZ, &local_EC);
        Script_EVT_SetRotation(SHEN_NAME('NBO4'), 0x38000000, &local_D4, 0);
        local_EC.x = -39.451801f;
        (&local_EC)->z = 38.2202f;
        (&local_D4)->y = 1456;
        Script_EVT_SetPosition(SHEN_NAME('NBO5'), POS_XYZ, &local_EC);
        Script_EVT_SetRotation(SHEN_NAME('NBO5'), 0x38000000, &local_D4, 0);
        local_EC.x = -39.347f;
        (&local_EC)->y = 4.3074f;
        (&local_EC)->z = 38.145f;
        Script_EVT_SetPosition(SHEN_NAME('ABRG'), POS_XYZ, &local_EC);
        local_E0.x = -34.724899f;
        (&local_E0)->y = 3.6616f;
        (&local_E0)->z = 39.666599f;
        local_EC.x = -34.724899f;
        (&local_EC)->y = 3.6616f;
        (&local_EC)->z = 39.666599f;
        break;
      case 600:
        Script_FACE(CHARA_AKIR, 0, 0, 10);
        Script_EVT_GetPosition(CHARA_MEGM, -1, &local_EC, 0);
        local_E0.x = (&local_EC)->x - 0.51f;
        (&local_E0)->y = (&local_EC)->y + 1.07f;
        (&local_E0)->z = (&local_EC)->z - 0.11f;
        Script_handSetFloatsFor(CHARA_AKIR, 1, data_212C4, 100);
        Script_handSetFloatsFor(CHARA_AKIR, 0, data_213A8, 100);
        break;
      case 650:
        Script_EVT_GetPosition(CHARA_CATM, -1, &local_EC, 0);
        local_EC.x = (&local_EC)->x - 0.13f;
        (&local_EC)->y = (&local_EC)->y + 0.09f;
        (&local_EC)->z = (&local_EC)->z + 0.02f;
        break;
      case 720:
        Script_EVT_GetPosition(CHARA_MEGM, -1, &local_EC, 0);
        (&local_EC)->y = (&local_EC)->y + 0.5f;
        break;
      case 970:
        Script_EVT_GetPosition(CHARA_CATM, -1, &local_EC, 0);
        local_EC.x = (&local_EC)->x - 0.13f;
        (&local_EC)->y = (&local_EC)->y + 0.09f;
        (&local_EC)->z = (&local_EC)->z + 0.02f;
        break;
      case 1100:
        Script_FACE(CHARA_AKIR, 0, 0, 10);
        break;
      case 1300:
        Script_EVT_GetPosition(CHARA_MEGM, -1, &local_EC, 0);
        (&local_EC)->y = (&local_EC)->y + (float)1;
        break;
      case 1380:
        Script_FACE(CHARA_AKIR, 2, 0, 15);
        break;
      case 1400:
        Script_FaceLookAtTargetUtils(CHARA_AKIR, 0);
        break;
      case 1675:
        Script_FACE(CHARA_AKIR, 0, 0, 25);
        break;
      case 1700:
        Script_EVT_GetPosition(CHARA_MEGM, -1, &local_EC, 0);
        local_EC.x = (&local_EC)->x + 0.15f;
        (&local_EC)->y = (&local_EC)->y + 0.85f;
        (&local_EC)->z = (&local_EC)->z - 0.24f;
        break;
      case 1780:
        local_EC.x = -34.049198f;
        (&local_EC)->y = 3.0052f;
        (&local_EC)->z = 38.827202f;
        break;
      case 1840:
        Script_EVT_GetPosition(CHARA_MEGM, -1, &local_EC, 0);
        local_EC.x = (&local_EC)->x + 0.15f;
        (&local_EC)->y = (&local_EC)->y + 0.85f;
        (&local_EC)->z = (&local_EC)->z - 0.24f;
        break;
      case 2157:
        Script_FACE(CHARA_AKIR, 0, 0, 10);
        break;
      case 2167:
        Script_EVT_GetPosition(CHARA_MEGM, -1, &local_EC, 0);
        local_EC.x = -34.049198f;
        (&local_EC)->y = 3.0052f;
        (&local_EC)->z = 38.827202f;
        break;
      case 2262:
        Script_EVT_GetPosition(CHARA_MEGM, -1, &local_EC, 0);
        local_EC.x = (&local_EC)->x + 0.15f;
        (&local_EC)->y = (&local_EC)->y + 0.6f;
        (&local_EC)->z = (&local_EC)->z - 0.24f;
        break;
      case 2447:
        Script_EVT_GetPosition(CHARA_CATM, -1, &local_EC, 0);
        local_EC.x = (&local_EC)->x - 0.13f;
        (&local_EC)->y = (&local_EC)->y + 0.09f;
        (&local_EC)->z = (&local_EC)->z + 0.02f;
        break;
      case 2767:
        Script_FaceLookAtTargetUtils(CHARA_AKIR, 0);
        break;
      case 3017:
        Script_handSetFloatsFor(CHARA_AKIR, 1, data_211E0, 50);
        Script_handSetFloatsFor(CHARA_AKIR, 0, data_211E0, 50);
        break;
      case 1340:
        Script_SetCharaFlags(CHARA_AKIR, 4);
        Script_SetCharaFlags(CHARA_MEGM, 4);
        break;
      case 490:
        Script_SetCharaFlags(CHARA_AKIR, 0);
        Script_SetCharaFlags(CHARA_MEGM, 0);
        break;
      case 2:
        Script_FACE(CHARA_MEGM, 0, 0, 15);
        break;
      case 120:
        Script_FACE(CHARA_MEGM, 1, 0, 20);
        break;
      case 320:
        Script_FACE(CHARA_MEGM, 0, 0, 20);
        break;
      case 340:
        break;
      case 491:
        Script_FACE(CHARA_MEGM, 1, 0, 15);
        break;
      case 644:
        break;
      case 1156:
        Script_FACE(CHARA_MEGM, 0, 0, 10);
        break;
      case 1701:
        break;
      case 2748:
        Script_FACE(CHARA_MEGM, 0, 0, 10);
        break;
      case 3:
        Script_ReadModelFormat7(CHARA_MEGM, 1, data_218FC, 10);
        break;
      case 560:
        Script_ReadModelFormat7(CHARA_MEGM, 0, data_21924, 10);
        Script_ReadModelFormat7(CHARA_MEGM, 1, data_2194C, 90);
        break;
      case 780:
        Script_ReadModelFormat7(CHARA_MEGM, 1, data_2199C, 40);
        break;
      case 990:
        Script_ReadModelFormat7(CHARA_MEGM, 0, data_21974, 40);
        break;
      case 1200:
        Script_CharHandlers(CHARA_MEGM, 1, 8, 16);
        Script_CharHandlers(CHARA_MEGM, 0, 8, 16);
        break;
      case 1900:
        Script_ReadModelFormat7(CHARA_MEGM, 0, data_21BA4, 40);
        break;
      case 1950:
        Script_ReadModelFormat7(CHARA_MEGM, 0, data_21974, 20);
        break;
      case 2427:
        Script_ReadModelFormat7(CHARA_MEGM, 1, data_219EC, 40);
        break;
      case 3020:
        break;
    }
    if (!((&local_EC)->x - 0.1f <= (&local_E0)->x)) {
      local_E0.x = (&local_E0)->x + 0.1f;
    } else if (!((&local_EC)->x - 0.01f <= (&local_E0)->x)) {
      local_E0.x = (&local_E0)->x + 0.01f;
    } else {
      if (!((&local_EC)->x - 0.001f <= (&local_E0)->x)) {
        local_E0.x = (&local_E0)->x + 0.001f;
        goto L00E8F4;
      }
    }
    L00E8F4:;
    if (!((&local_EC)->x + 0.1f >= (&local_E0)->x)) {
      local_E0.x = (&local_E0)->x - 0.1f;
    } else if (!((&local_EC)->x + 0.01f >= (&local_E0)->x)) {
      local_E0.x = (&local_E0)->x - 0.01f;
    } else {
      if (!((&local_EC)->x + 0.001f >= (&local_E0)->x)) {
        local_E0.x = (&local_E0)->x - 0.001f;
        goto L00E957;
      }
    }
    L00E957:;
    if (!((&local_EC)->y - 0.1f <= (&local_E0)->y)) {
      (&local_E0)->y = (&local_E0)->y + 0.1f;
    } else if (!((&local_EC)->y - 0.01f <= (&local_E0)->y)) {
      (&local_E0)->y = (&local_E0)->y + 0.01f;
    } else {
      if (!((&local_EC)->y - 0.001f <= (&local_E0)->y)) {
        (&local_E0)->y = (&local_E0)->y + 0.001f;
        goto L00E9DE;
      }
    }
    L00E9DE:;
    if (!((&local_EC)->y + 0.1f >= (&local_E0)->y)) {
      (&local_E0)->y = (&local_E0)->y - 0.1f;
    } else if (!((&local_EC)->y + 0.01f >= (&local_E0)->y)) {
      (&local_E0)->y = (&local_E0)->y - 0.01f;
    } else {
      if (!((&local_EC)->y + 0.001f >= (&local_E0)->y)) {
        (&local_E0)->y = (&local_E0)->y - 0.001f;
        goto L00EA65;
      }
    }
    L00EA65:;
    if (!((&local_EC)->z - 0.1f <= (&local_E0)->z)) {
      (&local_E0)->z = (&local_E0)->z + 0.1f;
    } else if (!((&local_EC)->z - 0.01f <= (&local_E0)->z)) {
      (&local_E0)->z = (&local_E0)->z + 0.01f;
    } else {
      if (!((&local_EC)->z - 0.001f <= (&local_E0)->z)) {
        (&local_E0)->z = (&local_E0)->z + 0.001f;
        goto L00EAEC;
      }
    }
    L00EAEC:;
    if (!((&local_EC)->z + 0.1f >= (&local_E0)->z)) {
      (&local_E0)->z = (&local_E0)->z - 0.1f;
    } else if (!((&local_EC)->z + 0.01f >= (&local_E0)->z)) {
      (&local_E0)->z = (&local_E0)->z - 0.01f;
    } else {
      if (!((&local_EC)->z + 0.001f >= (&local_E0)->z)) {
        (&local_E0)->z = (&local_E0)->z - 0.001f;
        goto L00EB73;
      }
    }
    L00EB73:;
    if ((local_FC > 1400) & (local_FC < 1700)) {
      Script_FaceLookAtTargetUtils(CHARA_AKIR, 0);
    } else if (local_FC > 2767) {
      Script_FaceLookAtTargetUtils(CHARA_AKIR, 0);
    } else {
      Script_FaceLookAtTargetUtils(CHARA_AKIR, 2, &local_E0, 1);
    }
    local_FC = local_FC + 1;
    if (local_FC == 650) {
      local_C8[0] = -728;
      local_C8[1] = -2184;
      local_C8[2] = -3276;
    }
    if (local_FC == 730) {
      local_C8[0] = -728;
      local_C8[1] = -1274;
      local_C8[2] = -3276;
    }
    if (*(int32_t *)local_C8 - 182 > (&local_BC)->x) {
      local_BC.x = (&local_BC)->x + 182;
    }
    if (*(int32_t *)local_C8 + 182 < (&local_BC)->x) {
      local_BC.x = (&local_BC)->x - 182;
    }
    if (local_C8[1] - 182 > (&local_BC)->y) {
      (&local_BC)->y = (&local_BC)->y + 182;
    }
    if (local_C8[1] + 182 < (&local_BC)->y) {
      (&local_BC)->y = (&local_BC)->y - 182;
    }
    if (local_C8[2] - 182 > (&local_BC)->z) {
      (&local_BC)->z = (&local_BC)->z + 182;
    }
    if (local_C8[2] + 182 < (&local_BC)->z) {
      (&local_BC)->z = (&local_BC)->z - 182;
    }
    Script_SetHandRootRotation(CHARA_AKIR, 1, 0x15, &local_BC);
    if ((local_FC > 700) & ((&local_B0)->z <= 3640)) {
      Script_handSetFor(CHARA_AKIR, 0, &local_B0);
      local_B0.x = (&local_B0)->x + 145;
      (&local_B0)->y = 0;
      (&local_B0)->z = (&local_B0)->z + 182;
      Script_SetHandRootRotation(CHARA_AKIR, 0, 0x15, &local_B0);
    }
    SCNF_WaitFrames_0000(1);
  }
  script_10176();
  Script_SetMaxInterpSpeed(0.5f);
  return;
}

static void script_ED5E(void) {
  int32_t local_FC;
  int32_t unused_F8;
  Vector3 local_EC;
  Vector3 local_E0;
  int32_t unused_D4[3];
  Vector3i local_C8;
  Vector3 local_BC;
  int32_t local_B8;
  local_EC.x = (&local_EC)->y = (&local_EC)->z = 0.0f;
  local_BC.x = (&local_BC)->y = (&local_BC)->z = 0.0f;
  script_DBA4();
  script_FF79();
  Script_shenSeqUtils(0);
  local_FC = 0;
  while (Script_shenSeqUtils(-1)) {
    local_B8 = local_FC;
    switch (local_B8) {
      case 1:
        Script_clmdSetFlags(CHARA_MEGM, 8, 1);
        Script_InitializeOsag(CHARA_MEGM);
        Script_SetMaxInterpSpeed(0.0f);
        Script_SetMotnFlag(CHARA_MEGM, -1);
        Script_FACE(CHARA_AKIR, 1, 0, 1);
        Script_ResetHandMotion(CHARA_AKIR, 1, 1, 0);
        Script_ResetHandMotion(CHARA_AKIR, 1, 0, 0);
        Script_handSetFloatsFor(CHARA_AKIR, 1, data_21498, 1);
        Script_handSetFloatsFor(CHARA_AKIR, 0, data_20C88, 1);
        local_C8.x = 47149;
        (&local_C8)->y = 0;
        (&local_C8)->z = 11650;
        local_BC.x = 0.116f;
        (&local_BC)->y = 0.016f;
        (&local_BC)->z = -0.03f;
        Script_fixoAttachEx(SHEN_NAME('NBO1'), CHARA_AKIR, 18, &local_BC, &local_C8);
        (&local_BC)->z = -0.02f;
        Script_fixoAttachEx(SHEN_NAME('NBO2'), CHARA_AKIR, 18, &local_BC, &local_C8);
        (&local_BC)->y = 0.01f;
        (&local_BC)->z = -0.01f;
        Script_fixoAttachEx(SHEN_NAME('NBO3'), CHARA_AKIR, 18, &local_BC, &local_C8);
        Script_EVT_GetPosition(CHARA_CATM, -1, &local_EC, 0);
        local_EC.x = (&local_EC)->x - 0.13f;
        (&local_EC)->y = (&local_EC)->y + 0.09f;
        (&local_EC)->z = (&local_EC)->z + 0.02f;
        local_E0.x = (&local_EC)->x;
        (&local_E0)->y = (&local_EC)->y;
        (&local_E0)->z = (&local_EC)->z;
        Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 25.0f);
        Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 25.0f);
        Script_mapmSetCamFlags(0, 0);
        break;
      case 120:
        Script_handSetFloatsFor(CHARA_AKIR, 0, data_21498, 20);
        Script_handSetFloatsFor(CHARA_AKIR, 1, data_20C88, 20);
        break;
      case 140:
        local_C8.x = 47149;
        (&local_C8)->y = 0;
        (&local_C8)->z = 11650;
        local_BC.x = 0.116f;
        (&local_BC)->y = 0.016f;
        (&local_BC)->z = 0.03f;
        Script_fixoAttachEx(SHEN_NAME('NBO1'), CHARA_AKIR, 12, &local_BC, &local_C8);
        (&local_BC)->z = -0.02f;
        Script_fixoAttachEx(SHEN_NAME('NBO2'), CHARA_AKIR, 12, &local_BC, &local_C8);
        (&local_BC)->y = 0.01f;
        (&local_BC)->z = -0.01f;
        Script_fixoAttachEx(SHEN_NAME('NBO3'), CHARA_AKIR, 12, &local_BC, &local_C8);
        break;
      case 190:
        Script_handSetFloatsFor(CHARA_AKIR, 0, data_213A8, 100);
        break;
      case 270:
        Script_fixoClearAttachment(SHEN_NAME('NBO1'));
        Script_fixoClearAttachment(SHEN_NAME('NBO2'));
        Script_fixoClearAttachment(SHEN_NAME('NBO3'));
        break;
      case 370:
        Script_FACE(CHARA_AKIR, 0, 0, 25);
        break;
      case 535:
        Script_FACE(CHARA_AKIR, 3, 0, 20);
        break;
      case 640:
        Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 40.0f);
        Script_FACE(CHARA_AKIR, 0, 0, 25);
        break;
      case 790:
        Script_FACE(CHARA_AKIR, 2, 0, 25);
        break;
      case 1165:
        Script_FACE(CHARA_AKIR, 0, 0, 25);
        Script_mapmSetCamFlags(0, 1);
        break;
      case 1540:
        Script_handSetFloatsFor(CHARA_AKIR, 1, data_211E0, 60);
        Script_handSetFloatsFor(CHARA_AKIR, 0, data_211E0, 60);
        break;
      case 380:
        Script_FaceLookAtTargetUtils(CHARA_AKIR, 0);
        break;
      case 710:
        Script_EVT_GetPosition(CHARA_MEGM, -1, &local_EC, 0);
        (&local_EC)->y = (&local_EC)->y + (float)1;
        break;
      case 770:
        Script_FaceLookAtTargetUtils(CHARA_AKIR, 0);
        break;
      case 1343:
        local_EC.x = -34.088799f;
        (&local_EC)->y = 3.3207f;
        (&local_EC)->z = 38.786301f;
        break;
      case 1388:
        Script_EVT_GetPosition(CHARA_MEGM, -1, &local_EC, 0);
        (&local_EC)->y = (&local_EC)->y + (float)1;
        break;
      case 1690:
        Script_FACE(CHARA_MEGM, 0, 0, 15);
        local_EC.x = -33.894699f;
        (&local_EC)->y = 3.7952f;
        (&local_EC)->z = 39.349098f;
        Script_FaceLookAtTargetUtils(CHARA_AKIR, 2, &local_EC, 10);
        break;
      case 2180:
        Script_FaceLookAtTargetUtils(CHARA_AKIR, 0);
        break;
      case 2:
        Script_FACE(CHARA_MEGM, 0, 0, 10);
        break;
      case 440:
        Script_FACE(CHARA_MEGM, 1, 0, 10);
        break;
      case 551:
        Script_FACE(CHARA_MEGM, 2, 0, 10);
        break;
      case 1161:
        Script_FACE(CHARA_MEGM, 0, 0, 10);
        break;
      case 1200:
        Script_FACE(CHARA_MEGM, 1, 0, 30);
        break;
      case 290:
        Script_ReadModelFormat7(CHARA_MEGM, 0, data_21A64, 20);
        break;
      case 110:
        Script_ReadModelFormat7(CHARA_MEGM, 0, data_21A14, 40);
        break;
      case 890:
        Script_ReadModelFormat7(CHARA_MEGM, 0, data_21AB4, 20);
        break;
      case 1430:
        Script_ReadModelFormat7(CHARA_MEGM, 0, data_21A64, 20);
        break;
      case 1460:
        Script_ReadModelFormat7(CHARA_MEGM, 0, data_21AB4, 20);
        break;
      case 855:
        Script_SetCharaFlags(CHARA_AKIR, 0);
        Script_SetCharaFlags(CHARA_MEGM, 0);
        break;
      case 1070:
        Script_SetCharaFlags(CHARA_AKIR, 4);
        Script_SetCharaFlags(CHARA_MEGM, 4);
        break;
      case 2330:
        Script_executeAicaCommand(2720, 2, 60);
        Script_filmCheckOrSetTransitionUtil(60, 0, 0, 0, 0, 255, 0, 0, 0);
        while (Script_filmCheckOrSetTransitionUtil(-1)) {
          SCNF_WaitFrames_0000(1);
        }
        break;
    }
    if (!((&local_EC)->x - 0.1f <= (&local_E0)->x)) {
      local_E0.x = (&local_E0)->x + 0.1f;
    } else if (!((&local_EC)->x - 0.01f <= (&local_E0)->x)) {
      local_E0.x = (&local_E0)->x + 0.01f;
    } else {
      if (!((&local_EC)->x - 0.001f <= (&local_E0)->x)) {
        local_E0.x = (&local_E0)->x + 0.001f;
        goto L00F48F;
      }
    }
    L00F48F:;
    if (!((&local_EC)->x + 0.1f >= (&local_E0)->x)) {
      local_E0.x = (&local_E0)->x - 0.1f;
    } else if (!((&local_EC)->x + 0.01f >= (&local_E0)->x)) {
      local_E0.x = (&local_E0)->x - 0.01f;
    } else {
      if (!((&local_EC)->x + 0.001f >= (&local_E0)->x)) {
        local_E0.x = (&local_E0)->x - 0.001f;
        goto L00F4F2;
      }
    }
    L00F4F2:;
    if (!((&local_EC)->y - 0.1f <= (&local_E0)->y)) {
      (&local_E0)->y = (&local_E0)->y + 0.1f;
    } else if (!((&local_EC)->y - 0.01f <= (&local_E0)->y)) {
      (&local_E0)->y = (&local_E0)->y + 0.01f;
    } else {
      if (!((&local_EC)->y - 0.001f <= (&local_E0)->y)) {
        (&local_E0)->y = (&local_E0)->y + 0.001f;
        goto L00F579;
      }
    }
    L00F579:;
    if (!((&local_EC)->y + 0.1f >= (&local_E0)->y)) {
      (&local_E0)->y = (&local_E0)->y - 0.1f;
    } else if (!((&local_EC)->y + 0.01f >= (&local_E0)->y)) {
      (&local_E0)->y = (&local_E0)->y - 0.01f;
    } else {
      if (!((&local_EC)->y + 0.001f >= (&local_E0)->y)) {
        (&local_E0)->y = (&local_E0)->y - 0.001f;
        goto L00F600;
      }
    }
    L00F600:;
    if (!((&local_EC)->z - 0.1f <= (&local_E0)->z)) {
      (&local_E0)->z = (&local_E0)->z + 0.1f;
    } else if (!((&local_EC)->z - 0.01f <= (&local_E0)->z)) {
      (&local_E0)->z = (&local_E0)->z + 0.01f;
    } else {
      if (!((&local_EC)->z - 0.001f <= (&local_E0)->z)) {
        (&local_E0)->z = (&local_E0)->z + 0.001f;
        goto L00F687;
      }
    }
    L00F687:;
    if (!((&local_EC)->z + 0.1f >= (&local_E0)->z)) {
      (&local_E0)->z = (&local_E0)->z - 0.1f;
    } else if (!((&local_EC)->z + 0.01f >= (&local_E0)->z)) {
      (&local_E0)->z = (&local_E0)->z - 0.01f;
    } else {
      if (!((&local_EC)->z + 0.001f >= (&local_E0)->z)) {
        (&local_E0)->z = (&local_E0)->z - 0.001f;
        goto L00F70E;
      }
    }
    L00F70E:;
    if ((local_FC > 690) & (local_FC < 770)) {
      Script_FaceLookAtTargetUtils(CHARA_AKIR, 2, &local_E0, 1);
    }
    if ((local_FC > 854) & (local_FC < 1650)) {
      Script_FaceLookAtTargetUtils(CHARA_AKIR, 2, &local_E0, 1);
    }
    if (local_FC == 180) {
      Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 24.0f);
      Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 24.0f);
    }
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  script_10176();
  return;
}

static void script_F78A(void) {
  int32_t local_FC;
  int32_t unused_F8;
  Vector3 local_EC;
  int32_t unused_E0[3];
  Vector3i local_D4;
  int32_t unused_C8[3];
  Vector3 local_BC;
  Vector3i local_B0;
  int32_t local_AC;
  script_DBA4();
  script_FF79();
  Script_shenSeqUtils(0);
  local_FC = 0;
  while (Script_shenSeqUtils(-1)) {
    local_AC = local_FC;
    switch (local_AC) {
      case 1:
        Script_SetMaxInterpSpeed(0.0f);
        Script_clmdSetFlags(CHARA_MEGM, 8, 1);
        Script_InitializeOsag(CHARA_MEGM);
        Script_SetMotnFlag(CHARA_MEGM, -1);
        Script_SetMotnFlag(CHARA_CATM, -1);
        Script_FACE(CHARA_AKIR, 1, 0, 1);
        Script_ResetHandMotion(CHARA_AKIR, 1, 0, 0);
        Script_ResetHandMotion(CHARA_AKIR, 1, 1, 0);
        Script_handSetFloatsFor(CHARA_AKIR, 0, data_20C88, 1);
        Script_handSetFloatsFor(CHARA_AKIR, 1, data_21498, 1);
        local_B0.x = 2876;
        (&local_B0)->y = 4437;
        (&local_B0)->z = 13742;
        local_BC.x = 0.137f;
        (&local_BC)->y = 0.033f;
        (&local_BC)->z = -0.058f;
        Script_fixoAttachEx(SHEN_NAME('ABRG'), CHARA_AKIR, 18, &local_BC, &local_B0);
        Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 19.0f);
        Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 19.0f);
        Script_mapmSetCamFlags(0, 0);
        break;
      case 120:
        Script_fixoClearAttachment(SHEN_NAME('ABRG'));
        local_D4.x = 32857;
        (&local_D4)->y = 17472;
        (&local_D4)->z = 32314;
        local_EC.x = -34.591999f;
        (&local_EC)->y = 2.6087f;
        (&local_EC)->z = 39.194302f;
        Script_EVT_SetPosition(SHEN_NAME('ABRG'), POS_XYZ, &local_EC);
        Script_EVT_SetRotation(SHEN_NAME('ABRG'), 0x38000000, &local_D4, 0);
        Script_handSetFloatsFor(CHARA_AKIR, 1, data_212C4, 20);
        break;
      case 600:
        Script_handSetFloatsFor(CHARA_AKIR, 1, data_211E0, 60);
        Script_handSetFloatsFor(CHARA_AKIR, 0, data_211E0, 60);
        break;
      case 970:
        break;
      case 2:
        Script_FACE(CHARA_MEGM, 0, 0, 10);
        break;
      case 361:
        Script_FACE(CHARA_MEGM, 0, 0, 10);
        break;
      case 440:
        Script_FACE(CHARA_MEGM, 7, 0, 10);
        break;
      case 580:
        Script_FACE(CHARA_MEGM, 1, 0, 20);
        break;
      case 690:
        local_EC.x = -33.894699f;
        (&local_EC)->y = 3.7952f;
        (&local_EC)->z = 39.349098f;
        Script_FaceLookAtTargetUtils(CHARA_AKIR, 2, &local_EC, 10);
        break;
      case 8:
        Script_ReadModelFormat7(CHARA_MEGM, 0, data_21B04, 20);
        break;
      case 140:
        Script_ReadModelFormat7(CHARA_MEGM, 0, data_21B54, 20);
        break;
      case 150:
        Script_CharHandlers(CHARA_MEGM, 1, 1, 16);
        break;
      case 605:
        Script_ReadModelFormat7(CHARA_MEGM, 1, data_21B2C, 8);
        break;
      case 614:
        Script_ReadModelFormat7(CHARA_MEGM, 1, data_218AC, 10);
        break;
      case 800:
        Script_CharHandlers(CHARA_MEGM, 1, 8, 16);
        break;
      case 1300:
        Script_executeAicaCommand(2720, 2, 60);
        Script_filmCheckOrSetTransitionUtil(60, 0, 0, 0, 0, 255, 0, 0, 0);
        while (Script_filmCheckOrSetTransitionUtil(-1)) {
          SCNF_WaitFrames_0000(1);
        }
        break;
    }
    if (!(local_FC == 60)) goto L00FB32;
    L00FB32:;
    if (local_FC == 1060) {
      Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 20.0f);
      Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 20.0f);
    }
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  Script_mapmSetCamFlags(0, 1);
  script_10176();
  return;
}

static void script_FB75(void) {
  int32_t local_FC;
  int32_t unused_B0[19];
  int32_t local_AC;
  script_1001F();
  Script_shenSeqUtils(0);
  local_FC = 0;
  while (Script_shenSeqUtils(-1)) {
    local_AC = local_FC;
    switch (local_AC) {
      case 1:
        Script_scenCreateTask(script_10296, 1, 60);
        Script_executeAicaCommand(17576, 0, 0);
        Script_executeAicaCommand(1184, 2, 70);
        Script_executeAicaCommand(1184, 4, 250);
        Script_executeAicaCommand(1184, 1, 88);
        Script_ResetHandMotion(CHARA_AKIR, 1, 0, 0);
        Script_ResetHandMotion(CHARA_AKIR, 1, 1, 0);
        Script_handSetFloatsFor(CHARA_AKIR, 1, data_211E0, 1);
        Script_handSetFloatsFor(CHARA_AKIR, 0, data_211E0, 1);
        Script_CharHandlers(CHARA_YASU, 1, 0, 16);
        Script_CharHandlers(CHARA_YASU, 0, 0, 16);
        Script_CharHandlers(CHARA_KAYO, 1, 2, 16);
        Script_CharHandlers(CHARA_KAYO, 0, 2, 16);
        Script_FaceLookAtTargetUtils(CHARA_CATM, 0);
        break;
      case 50:
        Script_handSetFloatsFor(CHARA_AKIR, 0, data_20C88, 40);
        Script_handSetFloatsFor(CHARA_AKIR, 1, data_20C88, 40);
        break;
      case 449:
        Script_SetCameraFlags(0);
        break;
      case 470:
        Script_handSetFloatsFor(CHARA_AKIR, 1, data_211E0, 40);
        Script_handSetFloatsFor(CHARA_AKIR, 0, data_211E0, 40);
        break;
      case 700:
        Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
        Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
        Script_InitializeOsag(CHARA_KAYO);
        break;
      case 949:
        Script_SetCharaFlags(CHARA_YASU, 7);
        Script_SetCharaFlags(CHARA_KAYO, 7);
        break;
      case 840:
        Script_CharHandlers(CHARA_AKIR, 1, 2, 16);
        Script_CharHandlers(CHARA_AKIR, 0, 2, 16);
        break;
      case 515:
        Script_SetCameraFlags(1);
        break;
      case 595:
        Script_CharHandlers(CHARA_AKIR, 1, 0, 16);
        Script_CharHandlers(CHARA_AKIR, 0, 0, 16);
        Script_SetCharaFlags(CHARA_AKIR, 101, 1, 2.59f);
        Script_SetCharaFlags(CHARA_YASU, 101, 1, 2.59f);
        Script_SetCharaFlags(CHARA_KAYO, 101, 1, 2.59f);
        break;
      case 596:
        Script_SetCharaFlags(CHARA_AKIR, 0);
        Script_SetCharaFlags(CHARA_YASU, 0);
        Script_SetCharaFlags(CHARA_KAYO, 0);
        break;
      case 1050:
        Script_SetCharaFlags(CHARA_AKIR, 4);
        break;
      case 738:
        Script_CharHandlers(CHARA_YASU, 0, 8, 16);
        break;
      case 1180:
        Script_CharHandlers(CHARA_YASU, 1, 2, 16);
        Script_CharHandlers(CHARA_YASU, 0, 2, 16);
        break;
      case 1240:
        Script_CharHandlers(CHARA_YASU, 1, 0, 16);
        Script_CharHandlers(CHARA_YASU, 0, 0, 16);
        break;
      case 1190:
        Script_CharHandlers(CHARA_AKIR, 1, 0, 16);
        Script_CharHandlers(CHARA_AKIR, 0, 0, 16);
        break;
      case 1620:
        Script_executeAicaCommand(2720, 2, 80);
        break;
      case 560:
        Script_FACE(CHARA_CATM, 0, 5, 20);
        break;
      case 960:
        Script_FACE(CHARA_CATM, 0, 0, 20);
        break;
      case 1450:
        Script_FACE(CHARA_CATM, 0, 3, 20);
        break;
      case 1621:
        Script_FACE(CHARA_CATM, 0, 0, 20);
        break;
    }
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  script_10224();
  return;
}

static void script_FF79(void) {
  Vector3 local_F4;
  Script_SetFogColour(&data_21BF4);
  Script_SetFogFlags(0, data_21C04);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 23.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 23.0f);
  Script_SetCPAMFlags(CHARA_CATM, 0);
  script_DBBC(CHARA_AKIR);
  script_DBBC(CHARA_MEGM);
  Script_SetMotnFlag(CHARA_MEGM, -1);
  Script_SetCharaFlags(CHARA_AKIR, 4);
  Script_SetCharaFlags(CHARA_MEGM, 4);
  local_F4.x = -39.995499f;
  (&local_F4)->y = 2.6f;
  (&local_F4)->z = 39.7691f;
  Script_EVT_SetPosition(SHEN_NAME('BNB1'), POS_XYZ, &local_F4);
  return;
}

static void script_1001F(void) {
  int32_t unused_E8[6];
  Vector4 local_D8;
  local_D8.x = 0.4f;
  (&local_D8)->y = 0.4f;
  (&local_D8)->z = 0.4f;
  (&local_D8)->w = 0.4f;
  Script_SetFogColour(&data_21C14);
  Script_SetFogFlags(0, data_21C24);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 23.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 23.0f);
  Script_FACE_LCG(CHARA_CATM, 0, 6, 0);
  Script_SetCPAMFlags(CHARA_CATM, 0);
  script_DBBC(CHARA_AKIR);
  script_DBBC(CHARA_YASU);
  script_DBBC(CHARA_KAYO);
  Script_SetCharaFlags(CHARA_AKIR, 4);
  Script_SetCharaFlags(CHARA_YASU, 7);
  Script_SetCharaFlags(CHARA_CATM, 7);
  Script_SetCharaFlags(CHARA_KAYO, 7);
  Script_SetCharaFlags(CHARA_CATM, 101, 1, 2.59f);
  Script_SetCharaFlags(CHARA_YASU, 101, 1, 1.92f);
  Script_SetCharaFlags(CHARA_KAYO, 101, 1, 1.92f);
  Script_SetCharaRefBoardPos(CHARA_YASU, &local_D8);
  Script_SetCharaRefBoardPos(CHARA_AKIR, &local_D8);
  Script_SetCharaRefBoardPos(CHARA_KAYO, &local_D8);
  Script_SetMotnFlag(CHARA_AKIR, -1);
  Script_SetMotnFlag(CHARA_CATM, -1);
  Script_SetMotnFlag(CHARA_KAYO, -1);
  Script_SetMotnFlag(CHARA_YASU, -1);
  return;
}

static void script_10176(void) {
  Vector3 local_F4;
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 70.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 70.0f);
  script_DC49(CHARA_AKIR);
  Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
  Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
  script_DC49(CHARA_MEGM);
  local_F4.x = 0.0f;
  (&local_F4)->y = 0.0f;
  (&local_F4)->z = 0.0f;
  Script_EVT_SetPosition(SHEN_NAME('BNB1'), POS_XYZ, &local_F4);
  Script_EVT_SetPosition(SHEN_NAME('BNB2'), POS_XYZ, &local_F4);
  Script_EVT_SetPosition(SHEN_NAME('BNB3'), POS_XYZ, &local_F4);
  Script_SetCPAMFlags(CHARA_CATM, 1);
  Script_mapmSetCamFlags(0, 1);
  return;
}

static void script_10224(void) {
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 70.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 70.0f);
  script_DC49(CHARA_AKIR);
  Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
  Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
  script_DC49(CHARA_YASU);
  script_DC49(CHARA_KAYO);
  Script_SetCPAMFlags(CHARA_CATM, 1);
  Script_mapmSetCamFlags(0, 1);
  return;
}

static int32_t script_10296(int32_t arg0) {
  Script_filmCheckOrSetTransitionUtil(arg0, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  return Script_srchControl(0);
}

static int32_t script_102CA(int32_t arg0, int32_t arg1, float arg2, float arg3) {
  int32_t local_FC;
  Vector3 local_F0;
  Vector3 local_E4;
  Vector3i local_D8;
  Script_EVT_GetPosition(arg0, -1, &local_F0, 0);
  Script_EVT_GetPosition(arg1, -1, &local_E4, 0);
  Script_mtwkGetSetRotation(arg0, &local_D8, 0x2000000);
  local_FC = Script_CheckTargetWithinAngleRange(&local_F0, arg2, (&local_D8)->y, (int32_t)(arg3 / 360.0f * (float)0x10000), &local_E4);
  return local_FC;
}

static int32_t script_1031A(void) {
  int32_t unused_FC;
  GameDateTime local_F4;
  Script_ReturnTimeDate(&local_F4);
  task_270 = Script_WeatherUtils_0(WeatherDataUtil_GetWeatherClassForDateTime, (&local_F4)->year, (&local_F4)->month, (&local_F4)->day, (&local_F4)->hours, (&local_F4)->minutes);
  return task_270;
}

static void script_1034F(int32_t arg0) {
  if ((Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JU00_HRSK_CAT_FOLLOWUP_PENDING) == 1) & (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JU00_HRSK_CAT_FOLLOWUP_RESOLVED) == 0)) {
    if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, 0x4D47454D) == 1) {
      Script_scenToggleActiveFor(CHARA_CATM, 0);
    } else {
      Script_SetCharaFlags(arg0, 1);
      Script_SetCharaFlags(arg0, 11);
    }
  } else {
    Script_SetCharaFlags(arg0, 1);
    Script_SetCharaFlags(arg0, 11);
  }
  return;
}

__attribute__((scn_string_bindings(0, data_220A8, 2, data_220BB, 4, data_220CE, 5, data_220D4, 6, data_220D9, 7, data_220DE, 8, data_220E3, 9, data_220E8, 10, data_220ED, 13, data_2210A)))
static void script_103BB(int32_t arg0) {
  int32_t unused_FC;
  Script_LoadVoiceFile(1, "01CJP");
  Script_AuthorUtils(0, 0, "seqdata0.bin", "CATA3");
  Script_AuthorUtils(0, 1, "seqdata1.bin", "CATA3");
  Script_LoadSND_Wrapper(0, 0, "FREE", "FREE", "FREE", "FREE", "FREE", "FREE");
  Script_LoadSND_Wrapper(0, 0, "bgm051.snd", 0, "a1_nekot.snd", 0, 0, 0);
  script_DA2F();
  Script_scenCreateTask(script_17362, 1, 60);
  script_104DC(0);
  script_104DC(1);
  Script_PlayCharaMotion(CHARA_MEGM, 4356, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_PlayCharaMotion(CHARA_HRSK, 0xFFFF, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_LoadVoiceFile(0, "01CJP");
  Script_AuthorUtils(AUTH_FREE, 0);
  Script_AuthorUtils(AUTH_FREE, 1);
  return;
}

static void script_104DC(int32_t arg0) {
  script_10661();
  if (arg0 == 0) {
    Script_executeAicaCommand(17576, 0, 0);
    Script_executeAicaCommand(1184, 2, 110);
    Script_charaDisplay(CHARA_HRSK, CDM_Hide);
    task_2C4 = Script_scenCreateTask(script_106C6, 0);
  } else {
    Script_charaDisplay(CHARA_HRSK, CDM_Show);
    task_2C4 = Script_scenCreateTask(script_106FF, 0);
  }
  Script_shenSeqUtils(arg0);
  while (Script_shenSeqUtils(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_srchControl((void *)task_2C4);
  return;
}

static void script_1055F(int32_t arg0) {
  Script_ccowUtils(arg0, 2);
  Script_ccowSetFlag80(arg0, 1);
  Script_motmToggleFlags(arg0, 2, 0x40);
  Script_SetMotnFlag(arg0, -1);
  Script_SetCPAMFlags(arg0, 0);
  Script_CharHandlers(arg0, 2, 8, 16);
  return;
}

static void script_1059B(int32_t arg0) {
  Script_ccowUtils(arg0, 2);
  Script_ccowSetFlag80(arg0, 1);
  Script_motmToggleFlags(arg0, 2, 0x40);
  Script_SetMotnFlag(arg0, -1);
  Script_SetCPAMFlags(arg0, 0);
  Script_SetCharaFlags(arg0, 4);
  Script_motmGetCharaFlags(arg0, 0, 0);
  Script_FaceEx(arg0, 1, 0, 1);
  Script_ResetHandMotion(arg0, 1, 1, 0);
  Script_ResetHandMotion(arg0, 1, 0, 0);
  Script_handSetFloatsFor(arg0, 1, data_21D18, 1);
  Script_handSetFloatsFor(arg0, 0, data_21D18, 1);
  Script_FACE_LCG(arg0, 0, 6, 0);
  Script_FaceLookAtTargetUtils(arg0, 0);
  return;
}

static void script_10637(void) {
  Script_SetShadowFlags(0, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_SetShadowFlags(CHARA_AKIR, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_SetShadowFlags(CHARA_AKIR, SCRIPT_SHADOW_UPDATE_LIGHT_MASK, 1);
  return;
}

static void script_10661(void) {
  script_1059B(CHARA_AKIR);
  script_1059B(CHARA_HRSK);
  script_1059B(CHARA_MEGM);
  script_1055F(CHARA_CATM);
  Script_SetCharaFlags(CHARA_CATM, 8);
  Script_ccowSetFlag80(SHEN_NAME('DRUG'), 1);
  script_10637();
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 70.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 70.0f);
  return;
}

static int32_t script_106C6(void) {
  int32_t local_FC;
  local_FC = 0;
  for (;;) {
    if (local_FC == 0) {
      Script_SetCharaFlags(CHARA_CATM, 101, 1, 3.264f);
    }
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_106FF(void) {
  int32_t local_FC;
  local_FC = 0;
  for (;;) {
    switch (local_FC) {
      case 0:
        Script_SetCharaFlags(CHARA_CATM, 101, 1, 2.6137f);
        Script_handSetFloatsFor(CHARA_HRSK, 0, data_22110, 0);
        Script_SetHandRootRotation(CHARA_HRSK, 0, 0x15, &data_221F4);
        break;
      case 830:
        Script_executeAicaCommand(2720, 1, 30);
        Script_executeAicaCommand(2720, 2, 30);
        Script_executeAicaCommand(2720, 3, 30);
        break;
    }
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_1078E(void) {
  script_10796();
  return;
}

static void script_10796(void) {
  int32_t local_FC;
  local_FC = Script_StepUtils(GET_STEP_HIGHER, 0);
  switch (local_FC) {
    case 24:
      script_107FC();
      break;
    case 23:
      script_107FF();
      break;
    case 22:
      script_10802();
      break;
    case 21:
      script_10805();
      break;
    case 20:
      script_10808();
      break;
  }
  return;
}

static void script_107FC(void) {
  return;
}

static void script_107FF(void) {
  return;
}

static void script_10802(void) {
  return;
}

static void script_10805(void) {
  return;
}

static void script_10808(void) {
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JU00_MEGUMI_KEEPS_CAT) == 0) {
    if ((script_1031A() != 2) & (script_1031A() != 3)) {
      if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, 0x4D47454D) == 1) {
        if (Script_IsPlayerAllowed(0)) {
          script_10882();
        }
      }
    }
  }
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JU00_CATA1_INTRO_COMPLETE) == 1) {
    script_10EA5();
  }
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JU00_CATA1_INTRO_COMPLETE) == 1) {
    script_12241();
  }
  return;
}

__attribute__((scn_string_bindings(0, data_22220, 1, data_22225, 2, data_2222A, 3, data_2222F, 4, data_22234, 5, data_22239, 6, data_2223E, 7, data_22245, 8, data_22252)))
static void script_10882(void) {
  Vector3 local_F4;
  int32_t local_E8[3];
  int32_t local_DC[3];
  int32_t local_D0[3];
  int32_t local_C4[3];
  int32_t local_C0;
  int32_t local_BC;
  local_E8[0] = -0x3DFB7780;
  local_E8[1] = 0;
  local_E8[2] = 0x4233C553;
  local_DC[0] = -0x3DED2426;
  local_DC[1] = 0;
  local_DC[2] = 0x423AD100;
  local_D0[0] = -0x3DEB2C08;
  local_D0[1] = 0;
  local_D0[2] = 0x42376CC0;
  local_C4[0] = -0x3DF92B6B;
  local_C4[1] = 0;
  local_C4[2] = 0x422F9759;
  if (script_17266(CHARA_AKIR, (int32_t)local_E8, (int32_t)local_DC, (int32_t)local_D0, (int32_t)local_C4) == 1) {
    Script_ryoControl(PCCTL_DisablePlayerControl);
    Script_FaceEx(CHARA_AKIR, 1, 0, 2);
    Script_ResetHandMotion(CHARA_AKIR, -1, 0, 0);
    Script_ResetHandMotion(CHARA_AKIR, -1, 1, 0);
    Script_shenClearFreeState(0);
    Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    task_28E = 1;
    if (1) {
      task_C = *&task_C | 4;
    } else {
      task_C = *&task_C & -5;
    }
    Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
    while (Script_filmCheckOrSetTransitionUtil(-1)) {
      SCNF_WaitFrames_0000(1);
    }
    script_17335(1);
    Script_FaceEx(CHARA_CATM, 0, 0, 1);
    task_288 = 0;
    script_166CC();
    local_BC = script_17171(0x494B4B52);
    local_C0 = script_17171(0x4B535248);
    Script_facemotUtils(CHARA_HRSK, 0, 0);
    Script_facemotUtils(CHARA_HRSK, 1, 0);
    Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 25.0f);
    Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 25.0f);
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
    Script_SetCharaRefBoardPos(CHARA_AKIR, &data_22210);
    Script_charaSetRefboardFlagTo(CHARA_AKIR, 1);
    Script_SetCharaRefBoardPos(CHARA_CATM, &data_22210);
    Script_charaSetRefboardFlagTo(CHARA_CATM, 1);
    Script_SetCharaRefBoardPos(CHARA_HRSK, &data_22210);
    Script_charaSetRefboardFlagTo(CHARA_HRSK, 1);
    Script_SetCharaRefBoardPos(CHARA_MEGM, &data_22210);
    Script_charaSetRefboardFlagTo(CHARA_MEGM, 1);
    script_103BB(0x52494B41);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_JU00_MEGUMI_KEEPS_CAT, 1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 792, 1);
    Script_charaDisplay(CHARA_HRSK, CDM_Hide);
    Script_charaDisplay(SHEN_NAME('DRUG'), CDM_Hide);
    Script_charaDisplay(CHARA_CATM, CDM_Show);
    Script_ccowUtils(CHARA_AKIR, 1);
    Script_ccowUtils(CHARA_MEGM, 1);
    Script_ccowSetFlag80(CHARA_AKIR, 2);
    Script_ccowSetFlag80(CHARA_MEGM, 2);
    Script_motmToggleFlags(CHARA_AKIR, 1, 0x40);
    Script_motmToggleFlags(CHARA_MEGM, 1, 0x40);
    Script_SetCPAMFlags(CHARA_AKIR, 1);
    Script_SetCPAMFlags(CHARA_MEGM, 1);
    Script_SetCPAMFlags(CHARA_CATM, 1);
    Script_FaceEx(CHARA_MEGM, 0, 0, 2);
    if (0) {
      task_C = *&task_C | 4;
    } else {
      task_C = *&task_C & -5;
    }
    Script_LoadSND_Wrapper(0, 0, "FREE", "FREE", "FREE", "FREE", "FREE", "FREE");
    Script_LoadSND_Wrapper(0, 0, "SELECT", 0, "f1jutaup.snd", 0, 0, 0);
    Script_executeAicaCommand(16296, 0, 0);
    Script_executeAicaCommand(1184, 2, 100);
    Script_ccowUtils(SHEN_NAME('BOX1'), 2);
    Script_ccowUtils(CHARA_CATM, 2);
    Script_ccowUtils(CHARA_CATM, 5);
    Script_ccowUtils(CHARA_CATM, 4);
    Script_charaSetRefboardFlagTo(CHARA_AKIR, 2);
    Script_charaSetRefboardFlagTo(CHARA_CATM, 2);
    Script_charaSetRefboardFlagTo(CHARA_HRSK, 2);
    Script_charaSetRefboardFlagTo(CHARA_MEGM, 2);
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "CATA3");
    Script_charaToggleMotEnabled(CHARA_CATM, 1);
    if (task_27C == -1) {
      task_27C = Script_scenCreateTask(script_19F93, 1, CHARA_CATM);
    }
    task_294 = 3;
    script_171F5(0x4B535248, local_C0);
    script_171F5(0x494B4B52, local_BC);
    script_166E6();
    Script_SetCharaFlags(CHARA_CATM, 11);
    Script_SetCharaFlags(CHARA_CATM, 101, 1, 2.577f);
    Script_WeatherUtils_0(WeatherDataUtil_SetInterpolatedWeatherOverride, 1, 1, 150, 0);
    Script_WeatherUtils_0(WeatherDataUtil_SetInterpolatedWeatherOverride, 2, 1, 150, 0);
    Script_FaceEx(CHARA_CATM, 1, 0, 1);
    Script_SetCameraSettings(5, 6, 1, 0);
    Script_convSetCamInactiveFlag(1);
    task_28E = 0;
    script_387C(SHEN_NAME('BOX1'), -34.6698f, 2.6f, 38.565601f);
    Script_EVT_GetPosition(SHEN_NAME('BOX1'), -1, &local_F4, 0);
    local_F4.x = (&local_F4)->x + -0.1638f;
    (&local_F4)->y = (&local_F4)->y;
    (&local_F4)->z = (&local_F4)->z + 1.0116f;
    script_387C(CHARA_CATM, (&local_F4)->x, (&local_F4)->y, (&local_F4)->z);
    script_38A4(CHARA_CATM, 0.0f, 121.0f, 0.0f);
    Script_EVT_GetPosition(CHARA_CATM, -1, &task_29C, ENTITY_XFORM_WORLD_SPACE);
    Script_mtwkGetSetRotation(CHARA_CATM, &task_2A8, 0x40000000);
    script_387C(CHARA_MEGM, -34.000198f, 2.591f, 39.365601f);
    script_38A4(CHARA_MEGM, 0.0f, 58.9856f, 0.0f);
    Script_EVT_GetPosition(CHARA_CATM, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
    Script_SetCharaLookAtPosition(CHARA_MEGM, 98320, &local_F4, 1);
    script_38A4(CHARA_AKIR, 0.0f, 205.103806f, 0.0f);
    Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 70.0f);
    Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 70.0f);
    Script_SetCharaFlags(CHARA_AKIR, 4);
    Script_SetCharaFlags(CHARA_AKIR, 101, 0);
    Script_ryoControl(RYO_RestoreControl);
    Script_FaceEx(CHARA_AKIR, 0, 0, 2);
    Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
    Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
    Script_shenClearFreeState(1);
    script_167AB(0);
  }
  return;
}

static void script_10EA5(void) {
  if (task_294 != 4) {
    if (script_102CA(CHARA_AKIR, CHARA_CATM, 2.5f, 57.5f)) {
      Script_SetOwnedItemMenuFlagIfOwned(1, OwnedItem_UIITEMMILK, 1);
      Script_SetOwnedItemMenuFlagIfOwned(1, OwnedItem_UIITEMDRIE, 1);
      Script_SetOwnedItemMenuFlagIfOwned(1, OwnedItem_UIITEMSALA, 1);
      Script_SetOwnedItemMenuFlagIfOwned(1, OwnedItem_UIITEMCANN, 1);
      Script_SetOwnedItemMenuFlagIfOwned(1, OwnedItem_UIITEMSLIC, 1);
      Script_SetOwnedItemMenuFlagIfOwned(1, OwnedItem_UIITEMSQUI, 1);
      if (Script_IsPlayerAllowed(0)) {
        script_10F76();
      } else {
        script_166A7();
        if (Script_srchUtils(SRCH_SCRIPT_GetCurrentTargetEntryIndex) == 17) {
          script_10F76();
        }
      }
    } else {
      Script_SetOwnedItemMenuFlagIfOwned(1, OwnedItem_UIITEMMILK, 0);
      Script_SetOwnedItemMenuFlagIfOwned(1, OwnedItem_UIITEMDRIE, 0);
      Script_SetOwnedItemMenuFlagIfOwned(1, OwnedItem_UIITEMSALA, 0);
      Script_SetOwnedItemMenuFlagIfOwned(1, OwnedItem_UIITEMCANN, 0);
      Script_SetOwnedItemMenuFlagIfOwned(1, OwnedItem_UIITEMSLIC, 0);
      Script_SetOwnedItemMenuFlagIfOwned(1, OwnedItem_UIITEMSQUI, 0);
    }
  }
  return;
}

__attribute__((scn_string_bindings(3, data_22276, 4, data_22280, 5, data_2228A, 6, data_22294, 7, data_2229E, 8, data_222A8, 9, data_222B2, 10, data_222BC, 11, data_222C6, 12, data_222D0, 13, data_222DA, 14, data_222E4, 15, data_222EE, 16, data_222F8, 17, data_22302)))
static void script_10F76(void) {
  int32_t unused_E8[6];
  task_2B8 = Script_cassetteUtils();
  task_2B8 = (task_2B8 & -0x10000) >> 16;
  switch (task_2B8) {
    case 59:
      script_17308(25);
      script_119FC();
      Script_charaDisplay(SHEN_NAME('MLKS'), CDM_Show);
      script_14F21(3.0f, "E1041A159", "E1041A160", "E1041A161");
      script_14FB0();
      if (task_278 == -1) {
        task_278 = Script_scenCreateTask(script_1AB0F, 3, CHARA_CATM, 32, 0x3F800000);
      }
      Script_executeAicaCommand(460201, 0, 0);
      script_11EC4();
      script_18EEB(CHARA_CATM);
      task_288 = 0;
      Script_executeAicaCommand(525737, 0, 0);
      Script_charaDisplay(SHEN_NAME('MLKS'), CDM_Hide);
      script_11F55();
      Script_GetCassetteFlags_0(59, 1, 0);
      break;
    case 60:
      script_17308(15);
      script_119FC();
      Script_charaDisplay(SHEN_NAME('NBSI'), CDM_Show);
      if (task_278 == -1) {
        task_278 = Script_scenCreateTask(script_1AB0F, 3, CHARA_CATM, 33, 0x3F800000);
      }
      script_14F21(3.0f, "E1041A118", "E1041A119", "E1041A120");
      script_14FB0();
      script_11EC4();
      script_18EEB(CHARA_CATM);
      task_288 = 0;
      Script_charaDisplay(SHEN_NAME('NBSI'), CDM_Hide);
      script_11F55();
      Script_GetCassetteFlags_0(60, 1, 0);
      break;
    case 62:
      script_17308(3);
      script_119FC();
      Script_charaDisplay(SHEN_NAME('SRMI'), CDM_Show);
      if (task_278 == -1) {
        task_278 = Script_scenCreateTask(script_1AB0F, 3, CHARA_CATM, 33, 0x3F800000);
      }
      script_14F21(3.0f, "E1041A118", "E1041A119", "E1041A120");
      script_14FB0();
      script_11EC4();
      script_18EEB(CHARA_CATM);
      task_288 = 0;
      Script_charaDisplay(SHEN_NAME('SRMI'), CDM_Hide);
      script_11F55();
      Script_GetCassetteFlags_0(62, 1, 0);
      break;
    case 61:
      script_17308(45);
      script_119FC();
      Script_charaDisplay(SHEN_NAME('TNKN'), CDM_Show);
      if (task_278 == -1) {
        task_278 = Script_scenCreateTask(script_1AB0F, 3, CHARA_CATM, 33, 0x3F800000);
      }
      script_14F21(3.0f, "E1041A118", "E1041A119", "E1041A120");
      script_14FB0();
      script_11EC4();
      script_18EEB(CHARA_CATM);
      task_288 = 0;
      Script_charaDisplay(SHEN_NAME('TNKN'), CDM_Hide);
      script_11F55();
      Script_GetCassetteFlags_0(61, 1, 0);
      break;
    case 58:
      script_17308(120);
      script_119FC();
      Script_charaDisplay(SHEN_NAME('MAGR'), CDM_Show);
      if (task_278 == -1) {
        task_278 = Script_scenCreateTask(script_1AB0F, 3, CHARA_CATM, 33, 0x3F800000);
      }
      script_14F21(3.0f, "E1041A118", "E1041A119", "E1041A120");
      script_14FB0();
      script_11EC4();
      script_18EEB(CHARA_CATM);
      task_288 = 0;
      Script_charaDisplay(SHEN_NAME('MAGR'), CDM_Hide);
      script_11F55();
      Script_GetCassetteFlags_0(58, 1, 0);
      break;
    case 57:
      script_17308(3);
      script_119FC();
      Script_charaDisplay(SHEN_NAME('IKAS'), CDM_Show);
      if (task_278 == -1) {
        task_278 = Script_scenCreateTask(script_1AB0F, 3, CHARA_CATM, 33, 0x3F800000);
      }
      script_14F21(3.0f, "E1041A118", "E1041A119", "E1041A120");
      script_14FB0();
      script_11EC4();
      script_18EEB(CHARA_CATM);
      task_288 = 0;
      Script_charaDisplay(SHEN_NAME('IKAS'), CDM_Hide);
      script_11F55();
      Script_GetCassetteFlags_0(57, 1, 0);
      break;
  }
  return;
}

__attribute__((scn_string_bindings(1, data_22316, 2, data_22320, 3, data_2232A, 5, data_2233E, 6, data_22348, 11, data_2237A, 12, data_22384, 13, data_2238E, 15, data_223A2, 16, data_223AC, 21, data_223DE, 22, data_223E8, 25, data_22406, 26, data_22410, 28, data_22424, 31, data_22442, 32, data_2244C, 35, data_2246A, 36, data_22474, 38, data_22488, 39, data_22492, 42, data_224B0, 44, data_224C4, 45, data_224CE, 46, data_224D8, 47, data_224E2, 48, data_224EC, 49, data_224F6, 52, data_22514, 54, data_22528, 55, data_22532, 56, data_2253C, 57, data_22546, 58, data_22550, 59, data_2255A, 60, data_22564, 61, data_2256E, 63, data_22582, 64, data_2258C)))
static void script_11366(void) {
  int8_t local_FF;
  Script_charaDisplay(CHARA_AKIR, CDM_Show);
  local_FF = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f));
  if (local_FF == 0) {
    script_167AB(9);
  } else {
    script_167AB(11);
  }
  if (-(task_2C0 != 1) & -(Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, 0x55534159) == 1) & -(Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, 0x4F59414B) == 1)) {
    if (task_2B8 == 59) {
      uint8_t unused_FC[3];
      int32_t local_F8;
      local_FF = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f));
      local_F8 = local_FF;
      switch (local_F8) {
        case 0:
          script_14F12("E1041D007");
          script_14FB0();
          script_14F12("E1041E011");
          script_14FB0();
          script_14F12("E1041A027");
          script_14FB0();
          script_14F12("E1041D010");
          script_14FB0();
          break;
        case 1:
          script_14F12("E1041A112");
          script_14FB0();
          script_14F12("E1041E002");
          script_14FB0();
          script_14F12("E1041D002");
          script_14FB0();
          break;
        case 2:
          script_14F12("E1041E008");
          script_14FB0();
          script_14F12("E1041D008");
          script_14FB0();
          script_14F12("E1041A113");
          script_14FB0();
          break;
      }
    } else {
      uint8_t unused_FC[3];
      int32_t local_F8;
      local_FF = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f));
      local_F8 = local_FF;
      switch (local_F8) {
        case 0:
          script_14F12("E1041D001");
          script_14FB0();
          script_14F12("E1041E011");
          script_14FB0();
          script_14F12("E1041A027");
          script_14FB0();
          script_14F12("E1041D010");
          script_14FB0();
          break;
        case 1:
          script_14F12("E1041A138");
          script_14FB0();
          script_14F12("E1041E002");
          script_14FB0();
          script_14F12("E1041D002");
          script_14FB0();
          break;
        case 2:
          script_14F12("E1041E012");
          script_14FB0();
          script_14F12("E1041D003");
          script_14FB0();
          script_14F12("E1041A029");
          script_14FB0();
          break;
      }
    }
  } else if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, 0x41544F4B) == 1) {
    if (task_2B8 == 59) {
      uint8_t unused_FC[3];
      int32_t local_F8;
      local_FF = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f));
      local_F8 = local_FF;
      switch (local_F8) {
        case 0:
          script_14F12("E1041C011");
          script_14FB0();
          script_14F12("E1041A021");
          script_14FB0();
          script_14F12("E1041C002");
          script_14FB0();
          break;
        case 1:
          script_14F12("E1041C012");
          script_14FB0();
          script_14F12("E1041A111");
          script_14FB0();
          script_14F12("E1041C004");
          script_14FB0();
          break;
        case 2:
          script_14F12("E1041A166");
          script_14FB0();
          script_14F12("E1041C013");
          script_14FB0();
          script_14F12("E1041A023");
          script_14FB0();
          script_14F12("E1041C015");
          script_14FB0();
          break;
      }
    } else {
      uint8_t unused_FC[3];
      int32_t local_F8;
      local_FF = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f));
      local_F8 = local_FF;
      switch (local_F8) {
        case 0:
          script_14F12("E1041C001");
          script_14FB0();
          script_14F12("E1041A021");
          script_14FB0();
          script_14F12("E1041C002");
          script_14FB0();
          break;
        case 1:
          script_14F12("E1041C003");
          script_14FB0();
          script_14F12("E1041A135");
          script_14FB0();
          script_14F12("E1041C004");
          script_14FB0();
          break;
        case 2:
          script_14F12("E1041A131");
          script_14FB0();
          script_14F12("E1041C005");
          script_14FB0();
          script_14F12("E1041A023");
          script_14FB0();
          script_14F12("E1041D010");
          script_14FB0();
          break;
      }
    }
  } else if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, 0x4D47454D) == 1) {
    if (task_2B8 == 59) {
      uint8_t unused_FC[3];
      int32_t local_F8;
      local_FF = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f));
      local_F8 = local_FF;
      switch (local_F8) {
        case 0:
          script_14F12("E1041B047");
          script_14FB0();
          script_14F12("E1041A165");
          script_14FB0();
          script_14F12("E1041B051");
          script_14FB0();
          break;
        case 1:
          script_14F12("E1041B048");
          script_14FB0();
          script_14F12("E1041A014");
          script_14FB0();
          script_14F12("E1041B052");
          script_14FB0();
          break;
        case 2:
          script_14F12("E1041A166");
          script_14FB0();
          script_14F12("E1041B053");
          script_14FB0();
          script_14F12("E1041A132");
          script_14FB0();
          script_14F12("E1041B054");
          script_14FB0();
          break;
      }
    } else {
      uint8_t unused_FC[3];
      int32_t local_F8;
      local_FF = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f));
      local_F8 = local_FF;
      switch (local_F8) {
        case 0:
          script_14F12("E1041B001");
          script_14FB0();
          script_14F12("E1041A130");
          script_14FB0();
          script_14F12("E1041B051");
          script_14FB0();
          break;
        case 1:
          script_14F12("E1041B003");
          script_14FB0();
          script_14F12("E1041A014");
          script_14FB0();
          script_14F12("E1041B052");
          script_14FB0();
          break;
        case 2:
          script_14F12("E1041A131");
          script_14FB0();
          script_14F12("E1041B053");
          script_14FB0();
          script_14F12("E1041A132");
          script_14FB0();
          script_14F12("E1041B054");
          script_14FB0();
          break;
      }
    }
  } else if (task_2B8 == 59) {
    script_14F21(3.0f, "E1041A122", "E1041A123", "E1041A162");
    script_14FB0();
  } else {
    script_14F21(3.0f, "E1041A122", "E1041A123", "E1041A124");
    script_14FB0();
  }
  return;
}

static void script_119FC(void) {
  Vector3 local_F4;
  Vector3 local_E8;
  Vector3 local_DC;
  int32_t unused_D0[3];
  int8_t local_CF;
  uint8_t unused_CC[3];
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_scenUtils(SCENARIO_CMD_INIT_SCNT);
  SCNF_RequestIconNotification(0, 0, 0, 2);
  Script_shenClearFreeState(0);
  Script_srchUtils(SRCH_SCRIPT_Reset);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 25.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 25.0f);
  task_28E = 1;
  if (task_27C != -1) {
    Script_SetTaskExecutionSuspended((void *)task_27C, 1);
  }
  Script_FaceEx(CHARA_AKIR, 1, 0, 2);
  task_288 = 0;
  script_17335(1);
  Script_SetCPAMFlags(CHARA_CATM, 0);
  Script_SetCPAMFlags(CHARA_AKIR, 0);
  Script_charaSetRenderOpacity(CHARA_AKIR, 1.0f);
  Script_charaToggleMotEnabled(CHARA_CATM, 1);
  Script_motmToggleFlags(CHARA_CATM, 2, 0x40);
  Script_motmToggleFlags(CHARA_AKIR, 2, 0x40);
  script_38A4(CHARA_CATM, 0.0f, 148.0f, 0.0f);
  Script_EVT_GetPosition(CHARA_CATM, -1, &local_F4, 0);
  script_387C(SHEN_NAME('DCAT'), (&local_F4)->x, (&local_F4)->y, (&local_F4)->z);
  script_38A4(SHEN_NAME('DCAT'), 0.0f, 148.0f, 0.0f);
  Script_PlayCharaMotion(CHARA_CATM, 60716, 0.0f, 60.0f, 0.0f, 0.0f, 0);
  script_11D6F();
  script_11CE0();
  Script_ryoControl(PCCTL_UpdateRyoNavYaw, 0x52494B41);
  Script_EVT_GetPosition(CHARA_AKIR, 5, &local_E8, 0);
  Script_EVT_GetPosition(CHARA_AKIR, 27, &local_DC, 0);
  (&local_E8)->y = (&local_E8)->y - (&local_DC)->y;
  if (!((&local_E8)->y < 1.0f)) {
    Script_scenCreateTask(script_1505B, 2, CHARA_AKIR, 1);
    local_CF = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f));
    if (local_CF == 0) {
      script_167AB(8);
    } else {
      script_167AB(10);
    }
    Script_executeAicaCommand(0x5805A9, 0, 0);
    Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_SIT_TATEHIZA_ST, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    script_18EEB(CHARA_AKIR);
  }
  Script_MoveObjectToObject(SHEN_NAME('YKUR'), 0x54414344);
  script_387C(SHEN_NAME('YKUR'), -0.13f, 0.194f, -0.216f);
  script_38A4(SHEN_NAME('YKUR'), 336.00589f, 356.001007f, 14.9963f);
  Script_charaDisplay(SHEN_NAME('YKUR'), CDM_Show);
  Script_scenCreateTask(script_11EED, 0);
  Script_SetNewHandMotion(SHEN_NAME('YKUR'), 1, 66, 0, 0, 0, 0, 1.0f, 0);
  Script_PlayCharaMotion(CHARA_CATM, 60723, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_SIT_TATEHIZA_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_charaDisplay(CHARA_AKIR, CDM_Hide);
  task_2BC = 0;
  script_167AB(1);
  return;
}

static void script_11CE0(void) {
  Script_scenToggleActiveFor(CHARA_AKIR, 0);
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, 0x4D47454D) == 1) {
    Script_scenToggleActiveFor(CHARA_MEGM, 0);
  }
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, 0x41544F4B) == 1) {
    Script_scenToggleActiveFor(CHARA_KOTA, 0);
  }
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, 0x55534159) == 1) {
    Script_scenToggleActiveFor(CHARA_YASU, 0);
  }
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, 0x4F59414B) == 1) {
    Script_scenToggleActiveFor(CHARA_KAYO, 0);
  }
  return;
}

static void script_11D6F(void) {
  Script_SetCharaRefBoardPos(CHARA_AKIR, &data_225A0);
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 1);
  Script_SetCharaRefBoardPos(CHARA_CATM, &data_225A0);
  Script_charaSetRefboardFlagTo(CHARA_CATM, 1);
  Script_charaSetRefboardFlagTo(SHEN_NAME('BOX1'), 1);
  Script_charaSetRefboardFlagTo(SHEN_NAME('MLKS'), 1);
  Script_charaSetRefboardFlagTo(SHEN_NAME('MAGR'), 1);
  Script_charaSetRefboardFlagTo(SHEN_NAME('NBSI'), 1);
  Script_charaSetRefboardFlagTo(SHEN_NAME('TNKN'), 1);
  Script_charaSetRefboardFlagTo(SHEN_NAME('SRMI'), 1);
  Script_charaSetRefboardFlagTo(SHEN_NAME('IKAS'), 1);
  Script_charaSetRefboardFlagTo(SHEN_NAME('YKUR'), 1);
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, 0x4D47454D) == 1) {
    Script_SetCharaRefBoardPos(CHARA_MEGM, &data_225A0);
    Script_charaSetRefboardFlagTo(CHARA_MEGM, 1);
  }
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, 0x41544F4B) == 1) {
    Script_SetCharaRefBoardPos(CHARA_KOTA, &data_225A0);
    Script_charaSetRefboardFlagTo(CHARA_KOTA, 1);
  }
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, 0x55534159) == 1) {
    Script_SetCharaRefBoardPos(CHARA_YASU, &data_225A0);
    Script_charaSetRefboardFlagTo(CHARA_YASU, 1);
  }
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, 0x4F59414B) == 1) {
    Script_SetCharaRefBoardPos(CHARA_KAYO, &data_225A0);
    Script_charaSetRefboardFlagTo(CHARA_KAYO, 1);
  }
  return;
}

static void script_11EC4(void) {
  for (;;) {
    if (((scn3_bits){.f = Script_GetCharaMotionRemainingFramesScaled(CHARA_CATM)}).i == ((scn3_bits){.f = (float)170}).i) {
      script_11366();
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static int32_t script_11EED(void) {
  Vector3 local_F4;
  int8_t local_F3;
  local_F4.x = -0.01f;
  (&local_F4)->y = 0.02f;
  (&local_F4)->z = 0.0f;
  local_F3 = 0;
  for (;;) {
    if (local_F3 == 30) {
      break;
    }
    Script_EVT_SetPosition(SHEN_NAME('YKUR'), -0x48000000, &local_F4);
    local_F3 = local_F3 + 1;
    SCNF_WaitFrames_0000(1);
  }
  Script_charaDisplay(SHEN_NAME('YKUR'), CDM_Hide);
  return Script_srchControl(0);
}

static void script_11F55(void) {
  Script_motmToggleFlags(CHARA_CATM, 1, 0x40);
  Script_motmSetBlendFrames(CHARA_CATM, MOTM_BLEND_FRAMES_39);
  Script_EVT_SetPosition(CHARA_CATM, 0x78000000, &task_29C);
  Script_EVT_SetRotation(CHARA_CATM, 0x78000000, &task_2A8, 0);
  if (task_27C != -1) {
    Script_SetTaskExecutionSuspended((void *)task_27C, 0);
  }
  Script_SetCPAMFlags(CHARA_CATM, 1);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 70.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 70.0f);
  script_12146();
  script_120B7();
  script_167AB(0);
  Script_executeAicaCommand(0x5905A9, 0, 0);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_SIT_TATEHIZA_EN, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  script_18EEB(CHARA_AKIR);
  Script_motmToggleFlags(CHARA_AKIR, 1, 0x40);
  Script_motmToggleFlags(CHARA_CATM, 1, 0x40);
  Script_motmSetBlendFrames(CHARA_CATM, MOTM_BLEND_FRAMES_5);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  script_387C(SHEN_NAME('BOX1'), -34.6698f, 2.6f, 38.565601f);
  Script_FaceEx(CHARA_AKIR, 0, 0, 2);
  Script_SetCPAMFlags(CHARA_AKIR, 1);
  task_28E = 0;
  Script_shenClearFreeState(1);
  Script_ryoControl(RYO_RestoreControl);
  Script_scenUtils(SCENARIO_CMD_UPDATE_TIMER);
  SCNF_WaitFrames_0000(1);
  return;
}

static void script_120B7(void) {
  Script_scenToggleActiveFor(CHARA_AKIR, 1);
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, 0x4D47454D) == 1) {
    Script_scenToggleActiveFor(CHARA_MEGM, 1);
  }
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, 0x41544F4B) == 1) {
    Script_scenToggleActiveFor(CHARA_KOTA, 1);
  }
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, 0x55534159) == 1) {
    Script_scenToggleActiveFor(CHARA_YASU, 1);
  }
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, 0x4F59414B) == 1) {
    Script_scenToggleActiveFor(CHARA_KAYO, 1);
  }
  return;
}

static void script_12146(void) {
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 2);
  Script_charaSetRefboardFlagTo(CHARA_CATM, 2);
  Script_charaSetRefboardFlagTo(SHEN_NAME('BOX1'), 0);
  Script_charaSetRefboardFlagTo(SHEN_NAME('MLKS'), 0);
  Script_charaSetRefboardFlagTo(SHEN_NAME('MAGR'), 0);
  Script_charaSetRefboardFlagTo(SHEN_NAME('NBSI'), 0);
  Script_charaSetRefboardFlagTo(SHEN_NAME('TNKN'), 0);
  Script_charaSetRefboardFlagTo(SHEN_NAME('SRMI'), 0);
  Script_charaSetRefboardFlagTo(SHEN_NAME('IKAS'), 0);
  Script_charaSetRefboardFlagTo(SHEN_NAME('YKUR'), 0);
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, 0x4D47454D) == 1) {
    Script_charaSetRefboardFlagTo(CHARA_MEGM, 2);
  }
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, 0x41544F4B) == 1) {
    Script_charaSetRefboardFlagTo(CHARA_KOTA, 2);
  }
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, 0x55534159) == 1) {
    Script_charaSetRefboardFlagTo(CHARA_YASU, 2);
  }
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, 0x4F59414B) == 1) {
    Script_charaSetRefboardFlagTo(CHARA_KAYO, 2);
  }
  return;
}

static void script_12241(void) {
  if (script_102CA(CHARA_AKIR, CHARA_CATM, 1.5f, 40.0f)) {
    script_16635();
    if (Script_IsPlayerAllowed(0)) {
      if (Script_GetControllerInput(1) == 0x400) {
        script_122B3();
      }
    } else {
      if (Script_srchUtils(SRCH_SCRIPT_GetCurrentTargetEntryIndex) == 17) {
        if (Script_GetControllerInput(1) == 0x400) {
          script_122B3();
        }
      }
    }
  } else {
    script_166A7();
  }
  return;
}

__attribute__((scn_string_bindings(0, data_225C0, 15, data_22656)))
static void script_122B3(void) {
  Vector3 local_F4;
  Script_scenUtils(SCENARIO_CMD_INIT_SCNT);
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_shenClearFreeState(0);
  Script_srchUtils(SRCH_SCRIPT_Reset);
  Script_SetCameraSettings(5, 6, 1, 0);
  Script_convSetCamInactiveFlag(1);
  script_167AB(0);
  script_14FD1();
  task_28E = 1;
  script_17335(1);
  script_166A7();
  Script_FaceEx(CHARA_AKIR, 1, 0, 2);
  Script_FACE(CHARA_AKIR, 0, 0, 0);
  Script_SetMotnFlag(CHARA_AKIR, -1);
  script_12D89();
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 1);
  Script_charaSetRefboardFlagTo(CHARA_CATM, 1);
  Script_charaSetRefboardFlagTo(SHEN_NAME('BOX1'), 1);
  Script_SetCharaRefBoardPos(CHARA_AKIR, &data_225B0);
  Script_SetCharaRefBoardPos(CHARA_CATM, &data_225B0);
  Script_ccowUtils(CHARA_AKIR, 2);
  Script_ccowUtils(SHEN_NAME('BOX1'), 2);
  Script_ccowUtils(CHARA_CATM, 2);
  Script_scenToggleActiveFor(CHARA_AKIR, 0);
  Script_ccowSetFlag80(CHARA_AKIR, 1);
  script_387C(SHEN_NAME('BOX1'), -34.6698f, 2.6f, 38.565601f);
  if (task_296 == 0) {
    Vector3 local_E8;
    Script_EVT_GetPosition(SHEN_NAME('BOX1'), -1, &local_E8, 0);
    local_E8.x = (&local_E8)->x + -0.1638f;
    (&local_E8)->y = (&local_E8)->y;
    (&local_E8)->z = (&local_E8)->z + 1.0116f;
    script_387C(CHARA_CATM, (&local_E8)->x, (&local_E8)->y, (&local_E8)->z);
    script_38A4(CHARA_CATM, 0.0f, 121.0f, 0.0f);
  }
  {
    Vector3 local_E8;
    Script_EVT_GetPosition(CHARA_CATM, -1, &local_E8, 0);
    if (task_296 == 1) {
      script_387C(CHARA_AKIR, -34.875099f, 2.591f, 39.712898f);
      Script_EVT_SetFacingTowardPoint(CHARA_AKIR, 0x38000000, &local_E8, 0);
    } else {
      script_387C(CHARA_AKIR, -35.1f, 2.591f, 40.310902f);
      Script_EVT_SetFacingTowardPoint(CHARA_AKIR, 0x38000000, &local_E8, 0);
    }
  }
  Script_EVT_GetPosition(CHARA_AKIR, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  {
    Vector3 local_E8;
    Vector3 local_DC;
    Vector3i local_D0;
    Script_mtwkGetSetRotation(CHARA_AKIR, &local_D0, 0x40000000);
    Script_motmGetCharaFlags(CHARA_AKIR, 0, 0);
    Script_motmGetCharaFlags(CHARA_CATM, 0, 0);
    Script_motmToggleFlags(CHARA_AKIR, 2, 0x40);
    Script_motmToggleFlags(CHARA_CATM, 2, 0x40);
    Script_charaToggleMotEnabled(CHARA_AKIR, 1);
    Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_SIT_TATEHIZA_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_SetCPAMFlags(CHARA_CATM, 0);
    for (;;) {
      Vector3i local_C4;
      int32_t local_C0;
      if (task_296 == 1) {
        script_167AB(61);
      } else {
        script_167AB(3);
      }
      Script_shenClearFreeState(1);
      local_C0 = SCNF_RequestActionSelectorFromParams_0008(31, 32, 33, 34, 600, 0xFFFF, ACTION_SELECTOR_MODE_STANDARD_TIMEOUT_CANCEL);
      Script_shenClearFreeState(0);
      if (local_C0 == 31) {
        if (task_27C != -1) {
          Script_SetTaskExecutionSuspended((void *)task_27C, 1);
        }
        if (task_294 != 4) {
          task_288 = 0;
        }
        script_17308(1);
        Script_EVT_GetPosition(CHARA_CATM, -1, &local_E8, ENTITY_XFORM_WORLD_SPACE);
        {
          Script_mtwkGetSetRotation(CHARA_CATM, &local_C4, 0x40000000);
          if (task_296 == 0) {
            script_387C(SHEN_NAME('BOX1'), -34.6698f, 2.6f, 38.565601f);
            Script_EVT_GetPosition(SHEN_NAME('BOX1'), -1, &local_DC, 0);
            local_DC.x = (&local_DC)->x + -0.1638f;
            (&local_DC)->y = (&local_DC)->y;
            (&local_DC)->z = (&local_DC)->z + 1.0116f;
            script_387C(CHARA_CATM, (&local_DC)->x, (&local_DC)->y, (&local_DC)->z);
            script_38A4(CHARA_CATM, 0.0f, 121.0f, 0.0f);
          }
          Script_InitializeOsag(CHARA_AKIR);
          Script_ResetHandMotion(CHARA_AKIR, 1, 1, 0);
          Script_charaToggleMotEnabled(CHARA_AKIR, 0);
          Script_MoveObjectToObject(CHARA_AKIR, 0x4D544143);
          script_12F47(CHARA_AKIR, CHARA_CATM);
          Script_MoveObjectToObject(CHARA_AKIR, -1);
          script_14F21(4.0f, "E1041A139", "E1041A140", "E1041A141", "E1041A142");
          SCNF_WaitFrames_0000(76);
          Script_EVT_SetPosition(CHARA_CATM, 0x78000000, &local_E8);
          Script_EVT_SetRotation(CHARA_CATM, 0x78000000, &local_C4, 0);
        }
        Script_EVT_SetPosition(CHARA_AKIR, 0x78000000, &local_F4);
        {
          Script_EVT_SetRotation(CHARA_AKIR, 0x78000000, &local_D0, 0);
        }
        Script_charaToggleMotEnabled(CHARA_AKIR, 1);
        Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_SIT_TATEHIZA_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
        if (task_294 != 4) {
          task_288 = 0;
        }
        if (task_27C != -1) {
          Script_SetTaskExecutionSuspended((void *)task_27C, 0);
        }
      } else if (local_C0 == 32) {
        Script_charaDisplay(CHARA_AKIR, CDM_Hide);
        Script_SetCameraFlags(-1);
        Script_camSetRotationStepSize(5);
        if (task_296 == 1) {
          script_167AB(59);
        } else {
          script_167AB(53);
        }
        for (;;) {
          if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_EV_STOP_ACTIVE) == 1) {
            break;
          }
          if (Script_GetControllerInput(1) == 0x200) {
            break;
          }
          if (Script_GetCurrentFrameCount() == 1800) {
            Script_AllocSpline(600);
          }
          SCNF_WaitFrames_0000(1);
        }
        Script_SetCameraFlags(0);
        Script_charaDisplay(CHARA_AKIR, CDM_Show);
      } else if (local_C0 == 33) {
        if (task_296 == 0) {
          script_387C(SHEN_NAME('BOX1'), -34.6698f, 2.6f, 38.565601f);
          Script_EVT_GetPosition(SHEN_NAME('BOX1'), -1, &local_DC, 0);
          local_DC.x = (&local_DC)->x + -0.1638f;
          (&local_DC)->y = (&local_DC)->y;
          (&local_DC)->z = (&local_DC)->z + 1.0116f;
          script_387C(CHARA_CATM, (&local_DC)->x, (&local_DC)->y, (&local_DC)->z);
          script_38A4(CHARA_CATM, 0.0f, 121.0f, 0.0f);
        }
        if (task_27C != -1) {
          Script_SetTaskExecutionSuspended((void *)task_27C, 1);
        }
        if (task_294 != 4) {
          if (task_296 == 1) {
            Script_scenCreateTask(script_151EA, 2, CHARA_CATM, 1);
          } else {
            Script_scenCreateTask(script_151EA, 2, CHARA_CATM, 4);
          }
          task_288 = 0;
        }
        script_17308(1);
        Script_charaDisplay(CHARA_AKIR, CDM_Hide);
        switch (task_294) {
          case 1:
            Script_PlayCharaMotion(CHARA_CATM, 60681, -1.0f, -1.0f, -1.0f, 1.0f, 0);
            break;
          case 2:
            Script_PlayCharaMotion(CHARA_CATM, 60716, -1.0f, -1.0f, -1.0f, 1.0f, 0);
            break;
          case 3:
            Script_PlayCharaMotion(CHARA_CATM, 60698, -1.0f, -1.0f, -1.0f, 1.0f, 0);
            break;
          case 4:
            break;
        }
        if (task_296 == 1) {
          script_167AB(60);
        } else {
          script_167AB(58);
        }
        if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JU00_STRAY_CAT_STAGE3_COMPLETE) == 1) {
          if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JU00_CAT_NAMED_CHIBI) == 1) {
            script_14F21(3.0f, "E1041A046", "E1041A145", "E1041A146");
          } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JU00_CAT_NAMED_TAMA) == 1) {
            script_14F21(3.0f, "E1041A049", "E1041A147", "E1041A148");
          } else {
            script_14F21(3.0f, "E1041A040", "E1041A143", "E1041A144");
          }
        } else {
          script_14F21(3.0f, "E1041A037", "E1041A054", "E1041A139");
        }
        script_14FB0();
        if (task_294 != 4) {
          if (task_278 == -1) {
            task_278 = Script_scenCreateTask(script_1AB0F, 3, CHARA_CATM, 30, 0x3F800000);
          }
          Script_executeAicaCommand(66985, 0, 0);
          SCNF_WaitFrames_0000(30);
          script_14F12("E1041A121");
          script_14FB0();
          script_14F21(3.0f, "E1041A149", "E1041A150", "E1041A151");
          script_14FB0();
          task_288 = 0;
        }
        Script_charaDisplay(CHARA_AKIR, CDM_Show);
        task_291 = 0;
        task_2BD = 2;
        if (task_27C != -1) {
          Script_SetTaskExecutionSuspended((void *)task_27C, 0);
        }
      } else {
        script_12B7A();
        break;
      }
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_EV_STOP_ACTIVE) == 1) {
        script_12B7A();
        Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_EV_STOP_ACTIVE, 0);
        break;
      }
    }
  }
  return;
}

static void script_12B7A(void) {
  Vector3 local_F4;
  script_12E68();
  Script_scenToggleActiveFor(CHARA_AKIR, 1);
  Script_ccowSetFlag80(CHARA_AKIR, 2);
  Script_ccowUtils(CHARA_AKIR, 1);
  if (task_296 == 1) {
    Script_ccowUtils(CHARA_CATM, 2);
    Script_ccowUtils(SHEN_NAME('BOX1'), 1);
  } else {
    Script_ccowUtils(CHARA_CATM, 2);
    Script_ccowUtils(CHARA_CATM, 5);
    Script_ccowUtils(CHARA_CATM, 4);
    Script_ccowUtils(SHEN_NAME('BOX1'), 2);
  }
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 2);
  Script_charaSetRefboardFlagTo(CHARA_CATM, 2);
  Script_charaSetRefboardFlagTo(SHEN_NAME('BOX1'), 0);
  Script_FaceEx(CHARA_AKIR, 0, 0, 0);
  Script_SetMotnFlag(CHARA_AKIR, 0);
  script_387C(SHEN_NAME('BOX1'), -34.6698f, 2.6f, 38.565601f);
  if (task_296 == 0) {
    Script_EVT_GetPosition(SHEN_NAME('BOX1'), -1, &local_F4, 0);
    local_F4.x = (&local_F4)->x + -0.1638f;
    (&local_F4)->y = (&local_F4)->y;
    (&local_F4)->z = (&local_F4)->z + 1.0116f;
    script_387C(CHARA_CATM, (&local_F4)->x, (&local_F4)->y, (&local_F4)->z);
    script_38A4(CHARA_CATM, 0.0f, 121.0f, 0.0f);
  }
  Script_SetCPAMFlags(CHARA_CATM, 1);
  Script_SetCameraSettings(5, 6, 1, 0);
  Script_convSetCamInactiveFlag(1);
  script_167AB(0);
  Script_motmGetCharaFlags(CHARA_AKIR, 3, 0);
  Script_motmGetCharaFlags(CHARA_CATM, 3, 0);
  Script_charaToggleMotEnabled(CHARA_AKIR, 1);
  Script_charaToggleMotEnabled(CHARA_CATM, 1);
  Script_motmToggleFlags(CHARA_AKIR, 1, 0x40);
  Script_motmToggleFlags(CHARA_CATM, 1, 0x40);
  Script_executeAicaCommand(0x5905A9, 0, 0);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_SIT_TATEHIZA_EN, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  script_18EEB(CHARA_AKIR);
  task_28E = 0;
  task_2BC = 0;
  Script_shenClearFreeState(1);
  script_15572();
  return;
}

static void script_12D89(void) {
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, 0x4D47454D) == 1) {
    Script_scenToggleActiveFor(CHARA_MEGM, 0);
    Script_ccowUtils(CHARA_MEGM, 2);
    Script_SetCPAMFlags(CHARA_MEGM, 0);
  }
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, 0x41544F4B) == 1) {
    Script_scenToggleActiveFor(CHARA_KOTA, 0);
    Script_ccowUtils(CHARA_KOTA, 2);
    Script_SetCPAMFlags(CHARA_KOTA, 0);
  }
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, 0x55534159) == 1) {
    Script_scenToggleActiveFor(CHARA_YASU, 0);
    Script_ccowUtils(CHARA_YASU, 2);
    Script_SetCPAMFlags(CHARA_YASU, 0);
  }
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, 0x4F59414B) == 1) {
    Script_scenToggleActiveFor(CHARA_KAYO, 0);
    Script_ccowUtils(CHARA_KAYO, 2);
    Script_SetCPAMFlags(CHARA_KAYO, 0);
  }
  return;
}

static void script_12E68(void) {
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, 0x4D47454D) == 1) {
    Script_scenToggleActiveFor(CHARA_MEGM, 1);
    Script_ccowUtils(CHARA_MEGM, 1);
    Script_SetCPAMFlags(CHARA_MEGM, 1);
  }
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, 0x41544F4B) == 1) {
    Script_scenToggleActiveFor(CHARA_KOTA, 1);
    Script_ccowUtils(CHARA_KOTA, 1);
    Script_SetCPAMFlags(CHARA_KOTA, 1);
  }
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, 0x55534159) == 1) {
    Script_scenToggleActiveFor(CHARA_YASU, 1);
    Script_ccowUtils(CHARA_YASU, 1);
    Script_SetCPAMFlags(CHARA_YASU, 1);
  }
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, 0x4F59414B) == 1) {
    Script_scenToggleActiveFor(CHARA_KAYO, 1);
    Script_ccowUtils(CHARA_KAYO, 1);
    Script_SetCPAMFlags(CHARA_KAYO, 1);
  }
  return;
}

static void script_12F47(int32_t arg0, int32_t arg1) {
  float local_F4[3];
  switch (task_294) {
    case 1:
      {
        int32_t unused_F0;
        int32_t local_EC;
        local_EC = Script_StepUtils(GET_STEP_HIGHER, 0);
        switch (local_EC) {
          case 0:
            local_F4[0] = 0.0f;
            local_F4[1] = 0.0f;
            local_F4[2] = 0.0f;
            break;
          case 1:
            local_F4[0] = 0.0f;
            local_F4[1] = 0.0f;
            local_F4[2] = 0.0f;
            break;
          case 2:
            local_F4[0] = 0.0f;
            local_F4[1] = 0.003f;
            local_F4[2] = 0.0f;
            break;
          case 3:
            local_F4[0] = 0.0f;
            local_F4[1] = 0.006f;
            local_F4[2] = 0.0f;
            break;
          case 4:
            local_F4[0] = 0.0f;
            local_F4[1] = 0.009f;
            local_F4[2] = 0.0f;
            break;
          case 5:
            local_F4[0] = 0.0f;
            local_F4[1] = 0.012f;
            local_F4[2] = 0.0f;
            break;
          case 6:
            local_F4[0] = 0.0f;
            local_F4[1] = 0.015f;
            local_F4[2] = 0.0f;
            break;
          case 7:
            local_F4[0] = 0.0f;
            local_F4[1] = 0.018f;
            local_F4[2] = 0.0f;
            break;
          case 8:
            local_F4[0] = 0.0f;
            local_F4[1] = 0.021f;
            local_F4[2] = 0.0f;
            break;
          case 9:
            local_F4[0] = 0.0f;
            local_F4[1] = 0.024f;
            local_F4[2] = 0.0f;
            break;
          case 10:
            local_F4[0] = 0.0f;
            local_F4[1] = 0.027f;
            local_F4[2] = 0.0f;
            break;
          case 11:
            local_F4[0] = 0.0f;
            local_F4[1] = 0.03f;
            local_F4[2] = 0.0f;
            break;
          case 12:
            local_F4[0] = 0.0f;
            local_F4[1] = 0.03f;
            local_F4[2] = 0.0f;
            break;
          case 13:
            local_F4[0] = 0.0f;
            local_F4[1] = 0.03f;
            local_F4[2] = 0.0f;
            break;
          case 14:
            local_F4[0] = 0.0f;
            local_F4[1] = 0.033f;
            local_F4[2] = 0.0f;
            break;
          case 15:
            local_F4[0] = 0.0f;
            local_F4[1] = 0.033f;
            local_F4[2] = 0.0f;
            break;
          case 16:
            local_F4[0] = 0.0f;
            local_F4[1] = 0.033f;
            local_F4[2] = 0.0f;
            break;
          case 17:
            local_F4[0] = 0.0f;
            local_F4[1] = 0.036f;
            local_F4[2] = 0.0f;
            break;
          case 18:
            local_F4[0] = 0.0f;
            local_F4[1] = 0.039f;
            local_F4[2] = 0.0f;
            break;
          case 19:
            local_F4[0] = 0.0f;
            local_F4[1] = 0.042f;
            local_F4[2] = 0.0f;
            break;
          case 20:
            local_F4[0] = 0.0f;
            local_F4[1] = 0.048f;
            local_F4[2] = 0.0f;
            break;
          case 21:
            local_F4[0] = 0.0f;
            local_F4[1] = 0.048f;
            local_F4[2] = 0.0f;
            break;
          case 22:
            local_F4[0] = 0.0f;
            local_F4[1] = 0.048f;
            local_F4[2] = 0.0f;
            break;
          case 23:
            local_F4[0] = 0.0f;
            local_F4[1] = 0.048f;
            local_F4[2] = 0.0f;
            break;
          case 24:
            local_F4[0] = 0.0f;
            local_F4[1] = 0.05f;
            local_F4[2] = 0.0f;
            break;
        }
        local_F4[0] = *local_F4 + -0.65f;
        local_F4[1] = local_F4[1] + -0.12f;
        local_F4[2] = local_F4[2] + -0.16f;
        script_387C(arg0, *local_F4, local_F4[1], local_F4[2]);
        script_38A4(arg0, 0.0f, 264.995697f, 0.0f);
        Script_PlayCharaMotion(arg0, 4367, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        Script_SetNewHandMotion(arg0, 1, 832, 0, 0, 0, 0, 1.0f, 0);
        Script_PlayCharaMotion(arg1, 60683, 0.0f, 77.0f, 0.0f, 1.0f, 0);
        if (task_278 == -1) {
          task_278 = Script_scenCreateTask(script_1AB0F, 3, arg1, 25, 0x3F800000);
        }
        if (task_296 == 1) {
          script_167AB(44);
        } else {
          script_167AB(56);
        }
        break;
      }
    case 2:
      {
        if (Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f)) == 0) {
          int32_t unused_F0;
          int32_t local_EC;
          local_EC = Script_StepUtils(GET_STEP_HIGHER, 0);
          switch (local_EC) {
            case 0:
              local_F4[0] = 0.0f;
              local_F4[1] = 0.0f;
              local_F4[2] = 0.0f;
              break;
            case 1:
              local_F4[0] = 0.0f;
              local_F4[1] = 0.0f;
              local_F4[2] = 0.0f;
              break;
            case 2:
              local_F4[0] = 0.0f;
              local_F4[1] = 0.003f;
              local_F4[2] = 0.0f;
              break;
            case 3:
              local_F4[0] = 0.0f;
              local_F4[1] = 0.006f;
              local_F4[2] = 0.0f;
              break;
            case 4:
              local_F4[0] = 0.0f;
              local_F4[1] = 0.009f;
              local_F4[2] = 0.0f;
              break;
            case 5:
              local_F4[0] = 0.0f;
              local_F4[1] = 0.012f;
              local_F4[2] = 0.0f;
              break;
            case 6:
              local_F4[0] = 0.0f;
              local_F4[1] = 0.015f;
              local_F4[2] = 0.0f;
              break;
            case 7:
              local_F4[0] = 0.0f;
              local_F4[1] = 0.018f;
              local_F4[2] = 0.0f;
              break;
            case 8:
              local_F4[0] = 0.0f;
              local_F4[1] = 0.021f;
              local_F4[2] = 0.0f;
              break;
            case 9:
              local_F4[0] = 0.0f;
              local_F4[1] = 0.024f;
              local_F4[2] = 0.0f;
              break;
            case 10:
              local_F4[0] = 0.0f;
              local_F4[1] = 0.027f;
              local_F4[2] = 0.0f;
              break;
            case 11:
              local_F4[0] = 0.0f;
              local_F4[1] = 0.03f;
              local_F4[2] = 0.0f;
              break;
            case 12:
              local_F4[0] = 0.0f;
              local_F4[1] = 0.03f;
              local_F4[2] = 0.0f;
              break;
            case 13:
              local_F4[0] = 0.0f;
              local_F4[1] = 0.03f;
              local_F4[2] = 0.0f;
              break;
            case 14:
              local_F4[0] = 0.0f;
              local_F4[1] = 0.033f;
              local_F4[2] = 0.0f;
              break;
            case 15:
              local_F4[0] = 0.0f;
              local_F4[1] = 0.033f;
              local_F4[2] = 0.0f;
              break;
            case 16:
              local_F4[0] = 0.0f;
              local_F4[1] = 0.033f;
              local_F4[2] = 0.0f;
              break;
            case 17:
              local_F4[0] = 0.0f;
              local_F4[1] = 0.036f;
              local_F4[2] = 0.0f;
              break;
            case 18:
              local_F4[0] = 0.0f;
              local_F4[1] = 0.039f;
              local_F4[2] = 0.0f;
              break;
            case 19:
              local_F4[0] = 0.0f;
              local_F4[1] = 0.042f;
              local_F4[2] = 0.0f;
              break;
            case 20:
              local_F4[0] = 0.0f;
              local_F4[1] = 0.048f;
              local_F4[2] = 0.0f;
              break;
            case 21:
              local_F4[0] = 0.0f;
              local_F4[1] = 0.048f;
              local_F4[2] = 0.0f;
              break;
            case 22:
              local_F4[0] = 0.0f;
              local_F4[1] = 0.048f;
              local_F4[2] = 0.0f;
              break;
            case 23:
              local_F4[0] = 0.0f;
              local_F4[1] = 0.048f;
              local_F4[2] = 0.0f;
              break;
            case 24:
              local_F4[0] = 0.0f;
              local_F4[1] = 0.05f;
              local_F4[2] = 0.0f;
              break;
          }
          local_F4[0] = *local_F4 + -0.63f;
          local_F4[1] = local_F4[1] + -0.11f;
          local_F4[2] = local_F4[2] + -0.15f;
          script_387C(arg0, *local_F4, local_F4[1], local_F4[2]);
          script_38A4(arg0, 0.0f, 264.995697f, 0.0f);
          Script_PlayCharaMotion(arg0, 4367, -1.0f, -1.0f, -1.0f, 1.0f, 0);
          Script_SetNewHandMotion(arg0, 1, 832, 0, 0, 0, 0, 1.0f, 0);
          Script_PlayCharaMotion(arg1, 60718, 0.0f, 77.0f, 0.0f, 1.0f, 0);
          if (task_278 == -1) {
            task_278 = Script_scenCreateTask(script_1AB0F, 3, arg1, 17, 0x3F800000);
          }
          if (task_296 == 1) {
            script_167AB(44);
          } else {
            script_167AB(54);
          }
        } else {
          int32_t unused_F0;
          int32_t local_EC;
          local_EC = Script_StepUtils(GET_STEP_HIGHER, 0);
          switch (local_EC) {
            case 0:
              local_F4[0] = 0.0f;
              local_F4[1] = -0.01f;
              local_F4[2] = 0.0f;
              break;
            case 1:
              local_F4[0] = 0.0f;
              local_F4[1] = -0.01f;
              local_F4[2] = 0.0f;
              break;
            case 2:
              local_F4[0] = -0.0006f;
              local_F4[1] = -0.00849999953f;
              local_F4[2] = -0.0036f;
              break;
            case 3:
              local_F4[0] = -0.0012f;
              local_F4[1] = -0.00699999975f;
              local_F4[2] = -0.0072f;
              break;
            case 4:
              local_F4[0] = -0.0018f;
              local_F4[1] = -0.0055f;
              local_F4[2] = -0.0108f;
              break;
            case 5:
              local_F4[0] = -0.0024f;
              local_F4[1] = -0.00399999972f;
              local_F4[2] = -0.0144f;
              break;
            case 6:
              local_F4[0] = -0.003f;
              local_F4[1] = -0.0025f;
              local_F4[2] = -0.018f;
              break;
            case 7:
              local_F4[0] = -0.0036f;
              local_F4[1] = -0.00100000016f;
              local_F4[2] = -0.0216f;
              break;
            case 8:
              local_F4[0] = -0.0042f;
              local_F4[1] = 0.000500000082f;
              local_F4[2] = -0.0252f;
              break;
            case 9:
              local_F4[0] = -0.0048f;
              local_F4[1] = 0.00200000033f;
              local_F4[2] = -0.0288f;
              break;
            case 10:
              local_F4[0] = -0.0054f;
              local_F4[1] = 0.00350000057f;
              local_F4[2] = -0.0324f;
              break;
            case 11:
              local_F4[0] = -0.006f;
              local_F4[1] = 0.005f;
              local_F4[2] = -0.036f;
              break;
            case 12:
              local_F4[0] = -0.006f;
              local_F4[1] = 0.005f;
              local_F4[2] = -0.036f;
              break;
            case 13:
              local_F4[0] = -0.006f;
              local_F4[1] = 0.005f;
              local_F4[2] = -0.036f;
              break;
            case 14:
              local_F4[0] = -0.0066f;
              local_F4[1] = 0.0065f;
              local_F4[2] = -0.0396f;
              break;
            case 15:
              local_F4[0] = -0.0066f;
              local_F4[1] = 0.0065f;
              local_F4[2] = -0.0396f;
              break;
            case 16:
              local_F4[0] = -0.0066f;
              local_F4[1] = 0.0065f;
              local_F4[2] = -0.0396f;
              break;
            case 17:
              local_F4[0] = -0.0072f;
              local_F4[1] = 0.00799999945f;
              local_F4[2] = -0.0432f;
              break;
            case 18:
              local_F4[0] = -0.0078f;
              local_F4[1] = 0.00950000063f;
              local_F4[2] = -0.0468f;
              break;
            case 19:
              local_F4[0] = -0.0084f;
              local_F4[1] = 0.011f;
              local_F4[2] = -0.0504f;
              break;
            case 20:
              local_F4[0] = -0.0096f;
              local_F4[1] = 0.014f;
              local_F4[2] = -0.0576f;
              break;
            case 21:
              local_F4[0] = -0.0096f;
              local_F4[1] = 0.014f;
              local_F4[2] = -0.0576f;
              break;
            case 22:
              local_F4[0] = -0.0096f;
              local_F4[1] = 0.014f;
              local_F4[2] = -0.0576f;
              break;
            case 23:
              local_F4[0] = -0.0096f;
              local_F4[1] = 0.014f;
              local_F4[2] = -0.0576f;
              break;
            case 24:
              local_F4[0] = -0.01f;
              local_F4[1] = 0.0150000006f;
              local_F4[2] = -0.06f;
              break;
          }
          local_F4[0] = *local_F4 + -0.27f;
          local_F4[1] = local_F4[1] + -0.06f;
          local_F4[2] = local_F4[2] + -0.65f;
          script_387C(arg0, *local_F4, local_F4[1], local_F4[2]);
          script_38A4(arg0, 0.0f, 221.0065f, 0.0f);
          Script_PlayCharaMotion(arg0, 4365, 1.0f, 239.0f, 1.0f, 1.0f, 0);
          Script_motmToggleFlags(arg0, 1, 0x80);
          Script_SetNewHandMotion(arg0, 1, 831, 0, 0, 0, 0, 1.0f, 0);
          Script_PlayCharaMotion(arg1, 60712, 0.0f, 77.0f, 0.0f, 1.0f, 0);
          if (task_278 == -1) {
            task_278 = Script_scenCreateTask(script_1AB0F, 3, arg1, 14, 0x3F800000);
          }
          if (task_296 == 1) {
            script_167AB(44);
          } else {
            script_167AB(55);
          }
        }
        break;
      }
    case 3:
      {
        if (Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f)) == 0) {
          int32_t unused_F0;
          int32_t local_EC;
          local_EC = Script_StepUtils(GET_STEP_HIGHER, 0);
          switch (local_EC) {
            case 0:
              local_F4[0] = 0.0f;
              local_F4[1] = 0.0f;
              local_F4[2] = 0.0f;
              break;
            case 1:
              local_F4[0] = 0.0f;
              local_F4[1] = 0.0f;
              local_F4[2] = 0.0f;
              break;
            case 2:
              local_F4[0] = 0.0012f;
              local_F4[1] = 0.0048f;
              local_F4[2] = -0.0042f;
              break;
            case 3:
              local_F4[0] = 0.0024f;
              local_F4[1] = 0.0096f;
              local_F4[2] = -0.0084f;
              break;
            case 4:
              local_F4[0] = 0.0036f;
              local_F4[1] = 0.0144f;
              local_F4[2] = -0.0126f;
              break;
            case 5:
              local_F4[0] = 0.0048f;
              local_F4[1] = 0.0192f;
              local_F4[2] = -0.0168f;
              break;
            case 6:
              local_F4[0] = 0.006f;
              local_F4[1] = 0.024f;
              local_F4[2] = -0.021f;
              break;
            case 7:
              local_F4[0] = 0.0072f;
              local_F4[1] = 0.0288f;
              local_F4[2] = -0.0252f;
              break;
            case 8:
              local_F4[0] = 0.0084f;
              local_F4[1] = 0.0336f;
              local_F4[2] = -0.0294f;
              break;
            case 9:
              local_F4[0] = 0.0096f;
              local_F4[1] = 0.0384f;
              local_F4[2] = -0.0336f;
              break;
            case 10:
              local_F4[0] = 0.0108f;
              local_F4[1] = 0.0432f;
              local_F4[2] = -0.0378f;
              break;
            case 11:
              local_F4[0] = 0.012f;
              local_F4[1] = 0.048f;
              local_F4[2] = -0.042f;
              break;
            case 12:
              local_F4[0] = 0.012f;
              local_F4[1] = 0.048f;
              local_F4[2] = -0.042f;
              break;
            case 13:
              local_F4[0] = 0.012f;
              local_F4[1] = 0.048f;
              local_F4[2] = -0.042f;
              break;
            case 14:
              local_F4[0] = 0.0132f;
              local_F4[1] = 0.0528f;
              local_F4[2] = -0.0462f;
              break;
            case 15:
              local_F4[0] = 0.0132f;
              local_F4[1] = 0.0528f;
              local_F4[2] = -0.0462f;
              break;
            case 16:
              local_F4[0] = 0.0132f;
              local_F4[1] = 0.0528f;
              local_F4[2] = -0.0462f;
              break;
            case 17:
              local_F4[0] = 0.0144f;
              local_F4[1] = 0.0576f;
              local_F4[2] = -0.0504f;
              break;
            case 18:
              local_F4[0] = 0.0156f;
              local_F4[1] = 0.0624f;
              local_F4[2] = -0.0546f;
              break;
            case 19:
              local_F4[0] = 0.0168f;
              local_F4[1] = 0.0672f;
              local_F4[2] = -0.0588f;
              break;
            case 20:
              local_F4[0] = 0.0192f;
              local_F4[1] = 0.0768f;
              local_F4[2] = -0.0672f;
              break;
            case 21:
              local_F4[0] = 0.0192f;
              local_F4[1] = 0.0768f;
              local_F4[2] = -0.0672f;
              break;
            case 22:
              local_F4[0] = 0.0192f;
              local_F4[1] = 0.0768f;
              local_F4[2] = -0.0672f;
              break;
            case 23:
              local_F4[0] = 0.0192f;
              local_F4[1] = 0.0768f;
              local_F4[2] = -0.0672f;
              break;
            case 24:
              local_F4[0] = 0.02f;
              local_F4[1] = 0.08f;
              local_F4[2] = -0.07f;
              break;
          }
          local_F4[0] = *local_F4 + -0.26f;
          local_F4[1] = local_F4[1] + -0.02f;
          local_F4[2] = local_F4[2] + -0.61f;
          script_387C(arg0, *local_F4, local_F4[1], local_F4[2]);
          script_38A4(arg0, 0.0f, 221.0065f, 0.0f);
          Script_PlayCharaMotion(arg0, 4366, 1.0f, 179.0f, 1.0f, 1.0f, 0);
          Script_motmToggleFlags(arg0, 1, 0x80);
          Script_SetNewHandMotion(arg0, 1, 832, 0, 0, 0, 0, 1.0f, 0);
          Script_PlayCharaMotion(arg1, 60700, 0.0f, 77.0f, 0.0f, 1.0f, 0);
          if (task_278 == -1) {
            task_278 = Script_scenCreateTask(script_1AB0F, 3, arg1, 8, 0x3F800000);
          }
          if (task_296 == 1) {
            script_167AB(44);
          } else {
            script_167AB(52);
          }
        } else {
          int32_t unused_F0;
          int32_t local_EC;
          local_EC = Script_StepUtils(GET_STEP_HIGHER, 0);
          switch (local_EC) {
            case 0:
              local_F4[0] = 0.0f;
              local_F4[1] = 0.04f;
              local_F4[2] = 0.0f;
              break;
            case 1:
              local_F4[0] = 0.0f;
              local_F4[1] = 0.04f;
              local_F4[2] = 0.0f;
              break;
            case 2:
              local_F4[0] = -0.0012f;
              local_F4[1] = 0.0429999977f;
              local_F4[2] = -0.0036f;
              break;
            case 3:
              local_F4[0] = -0.0024f;
              local_F4[1] = 0.046f;
              local_F4[2] = -0.0072f;
              break;
            case 4:
              local_F4[0] = -0.0036f;
              local_F4[1] = 0.049f;
              local_F4[2] = -0.0108f;
              break;
            case 5:
              local_F4[0] = -0.0048f;
              local_F4[1] = 0.052f;
              local_F4[2] = -0.0144f;
              break;
            case 6:
              local_F4[0] = -0.006f;
              local_F4[1] = 0.055f;
              local_F4[2] = -0.018f;
              break;
            case 7:
              local_F4[0] = -0.0072f;
              local_F4[1] = 0.058f;
              local_F4[2] = -0.0216f;
              break;
            case 8:
              local_F4[0] = -0.0084f;
              local_F4[1] = 0.060999997f;
              local_F4[2] = -0.0252f;
              break;
            case 9:
              local_F4[0] = -0.0096f;
              local_F4[1] = 0.0639999956f;
              local_F4[2] = -0.0288f;
              break;
            case 10:
              local_F4[0] = -0.0108f;
              local_F4[1] = 0.067f;
              local_F4[2] = -0.0324f;
              break;
            case 11:
              local_F4[0] = -0.012f;
              local_F4[1] = 0.07f;
              local_F4[2] = -0.036f;
              break;
            case 12:
              local_F4[0] = -0.012f;
              local_F4[1] = 0.07f;
              local_F4[2] = -0.036f;
              break;
            case 13:
              local_F4[0] = -0.012f;
              local_F4[1] = 0.07f;
              local_F4[2] = -0.036f;
              break;
            case 14:
              local_F4[0] = -0.0132f;
              local_F4[1] = 0.073f;
              local_F4[2] = -0.0396f;
              break;
            case 15:
              local_F4[0] = -0.0132f;
              local_F4[1] = 0.073f;
              local_F4[2] = -0.0396f;
              break;
            case 16:
              local_F4[0] = -0.0132f;
              local_F4[1] = 0.073f;
              local_F4[2] = -0.0396f;
              break;
            case 17:
              local_F4[0] = -0.0144f;
              local_F4[1] = 0.076f;
              local_F4[2] = -0.0432f;
              break;
            case 18:
              local_F4[0] = -0.0156f;
              local_F4[1] = 0.0789999962f;
              local_F4[2] = -0.0468f;
              break;
            case 19:
              local_F4[0] = -0.0168f;
              local_F4[1] = 0.082f;
              local_F4[2] = -0.0504f;
              break;
            case 20:
              local_F4[0] = -0.0192f;
              local_F4[1] = 0.088f;
              local_F4[2] = -0.0576f;
              break;
            case 21:
              local_F4[0] = -0.0192f;
              local_F4[1] = 0.088f;
              local_F4[2] = -0.0576f;
              break;
            case 22:
              local_F4[0] = -0.0192f;
              local_F4[1] = 0.088f;
              local_F4[2] = -0.0576f;
              break;
            case 23:
              local_F4[0] = -0.0192f;
              local_F4[1] = 0.088f;
              local_F4[2] = -0.0576f;
              break;
            case 24:
              local_F4[0] = -0.02f;
              local_F4[1] = 0.09f;
              local_F4[2] = -0.06f;
              break;
          }
          local_F4[0] = *local_F4 + -0.27f;
          local_F4[1] = local_F4[1] + -0.02f;
          local_F4[2] = local_F4[2] + -0.65f;
          script_387C(arg0, *local_F4, local_F4[1], local_F4[2]);
          script_38A4(arg0, 0.0f, 221.0065f, 0.0f);
          Script_PlayCharaMotion(arg0, 4365, 1.0f, 239.0f, 1.0f, 1.0f, 0);
          Script_motmToggleFlags(arg0, 1, 0x80);
          Script_SetNewHandMotion(arg0, 1, 831, 0, 0, 0, 0, 1.0f, 0);
          Script_PlayCharaMotion(arg1, 60695, 0.0f, 77.0f, 0.0f, 1.0f, 0);
          if (task_278 == -1) {
            task_278 = Script_scenCreateTask(script_1AB0F, 3, arg1, 4, 0x3F800000);
          }
          if (task_296 == 1) {
            script_167AB(44);
          } else {
            script_167AB(52);
          }
        }
        break;
      }
    case 4:
      {
        int32_t unused_F0;
        int32_t local_EC;
        local_EC = Script_StepUtils(GET_STEP_HIGHER, 0);
        switch (local_EC) {
          case 0:
            local_F4[0] = 0.0f;
            local_F4[1] = 0.0f;
            local_F4[2] = 0.0f;
            break;
          case 1:
            local_F4[0] = 0.0f;
            local_F4[1] = 0.0f;
            local_F4[2] = 0.0f;
            break;
          case 2:
            local_F4[0] = 0.0f;
            local_F4[1] = 0.003f;
            local_F4[2] = 0.0f;
            break;
          case 3:
            local_F4[0] = 0.0f;
            local_F4[1] = 0.006f;
            local_F4[2] = 0.0f;
            break;
          case 4:
            local_F4[0] = 0.0f;
            local_F4[1] = 0.009f;
            local_F4[2] = 0.0f;
            break;
          case 5:
            local_F4[0] = 0.0f;
            local_F4[1] = 0.012f;
            local_F4[2] = 0.0f;
            break;
          case 6:
            local_F4[0] = 0.0f;
            local_F4[1] = 0.015f;
            local_F4[2] = 0.0f;
            break;
          case 7:
            local_F4[0] = 0.0f;
            local_F4[1] = 0.018f;
            local_F4[2] = 0.0f;
            break;
          case 8:
            local_F4[0] = 0.0f;
            local_F4[1] = 0.021f;
            local_F4[2] = 0.0f;
            break;
          case 9:
            local_F4[0] = 0.0f;
            local_F4[1] = 0.024f;
            local_F4[2] = 0.0f;
            break;
          case 10:
            local_F4[0] = 0.0f;
            local_F4[1] = 0.027f;
            local_F4[2] = 0.0f;
            break;
          case 11:
            local_F4[0] = 0.0f;
            local_F4[1] = 0.03f;
            local_F4[2] = 0.0f;
            break;
          case 12:
            local_F4[0] = 0.0f;
            local_F4[1] = 0.03f;
            local_F4[2] = 0.0f;
            break;
          case 13:
            local_F4[0] = 0.0f;
            local_F4[1] = 0.03f;
            local_F4[2] = 0.0f;
            break;
          case 14:
            local_F4[0] = 0.0f;
            local_F4[1] = 0.033f;
            local_F4[2] = 0.0f;
            break;
          case 15:
            local_F4[0] = 0.0f;
            local_F4[1] = 0.033f;
            local_F4[2] = 0.0f;
            break;
          case 16:
            local_F4[0] = 0.0f;
            local_F4[1] = 0.033f;
            local_F4[2] = 0.0f;
            break;
          case 17:
            local_F4[0] = 0.0f;
            local_F4[1] = 0.036f;
            local_F4[2] = 0.0f;
            break;
          case 18:
            local_F4[0] = 0.0f;
            local_F4[1] = 0.039f;
            local_F4[2] = 0.0f;
            break;
          case 19:
            local_F4[0] = 0.0f;
            local_F4[1] = 0.042f;
            local_F4[2] = 0.0f;
            break;
          case 20:
            local_F4[0] = 0.0f;
            local_F4[1] = 0.048f;
            local_F4[2] = 0.0f;
            break;
          case 21:
            local_F4[0] = 0.0f;
            local_F4[1] = 0.048f;
            local_F4[2] = 0.0f;
            break;
          case 22:
            local_F4[0] = 0.0f;
            local_F4[1] = 0.048f;
            local_F4[2] = 0.0f;
            break;
          case 23:
            local_F4[0] = 0.0f;
            local_F4[1] = 0.048f;
            local_F4[2] = 0.0f;
            break;
          case 24:
            local_F4[0] = 0.0f;
            local_F4[1] = 0.05f;
            local_F4[2] = 0.0f;
            break;
        }
        local_F4[0] = *local_F4 + -0.65f;
        local_F4[1] = local_F4[1] + -0.12f;
        local_F4[2] = local_F4[2] + -0.16f;
        script_387C(arg0, *local_F4, local_F4[1], local_F4[2]);
        script_38A4(arg0, 0.0f, 264.995697f, 0.0f);
        Script_PlayCharaMotion(arg0, 4367, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        Script_SetNewHandMotion(CHARA_AKIR, 1, 832, 0, 0, 0, 0, 1.0f, 0);
        if (task_278 == -1) {
          task_278 = Script_scenCreateTask(script_1AB0F, 3, arg1, 27, 0x3F800000);
        }
        if (task_296 == 1) {
          script_167AB(44);
        } else {
          script_167AB(56);
        }
        break;
      }
  }
  return;
}

static void script_14E33(int32_t arg0) {
  if (task_294 == 1) {
    Script_PlayCharaMotion(arg0, 60683, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  } else if (task_294 == 2) {
    Script_PlayCharaMotion(arg0, 60718, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  } else if (task_294 == 3) {
    Script_PlayCharaMotion(arg0, 60700, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  } else if (task_294 == 4) {
    Script_PlayCharaMotion(arg0, 60683, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  } else {
    Script_PlayCharaMotion(arg0, 60718, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  }
  return;
}

static void script_14F0F(void) {
  int32_t unused_FC;
  return;
}

static void script_14F12(const char *arg0) {
  task_2B4 = Script_ShowUIMessageFromID(arg0);
  return;
}

static void script_14F21(float arg0, const char *arg1, const char *arg2, const char *arg3, ...) {
  int8_t local_FF;
  local_FF = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, arg0));
  switch (local_FF) {
    case 0:
      task_2B4 = Script_ShowUIMessageFromID(arg1);
      break;
    case 1:
      task_2B4 = Script_ShowUIMessageFromID(arg2);
      break;
    case 2:
      task_2B4 = Script_ShowUIMessageFromID(arg3);
      break;
    case 3:
      {
        va_list arguments0;
        va_start(arguments0, arg3);
        task_2B4 = Script_ShowUIMessageFromID(va_arg(arguments0, const char *));
        va_end(arguments0);
      }
      break;
    case 4:
      {
        va_list arguments1;
        va_start(arguments1, arg3);
        (void)va_arg(arguments1, const char *);
        task_2B4 = Script_ShowUIMessageFromID(va_arg(arguments1, const char *));
        va_end(arguments1);
      }
      break;
  }
  return;
}

static void script_14FB0(void) {
  SCNF_WaitFrames_0000(1);
  while (Script_convGetIsActive((const void *)task_2B4)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_14FD1(void) {
  Vector3 local_F4;
  Vector3 local_E8;
  int32_t unused_E4;
  float local_DC[2];
  Script_EVT_GetPosition(CHARA_AKIR, 5, &local_F4, 0);
  Script_EVT_GetPosition(CHARA_AKIR, 27, &local_E8, 0);
  local_DC[1] = (&local_F4)->y - (&local_E8)->y;
  if (!(local_DC[1] < 1.0f)) {
    Script_executeAicaCommand(0x5805A9, 0, 0);
    Script_ryoControl(PCCTL_UpdateRyoNavYaw, 0x52494B41);
    Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_SIT_TATEHIZA_ST, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    script_18EEB(CHARA_AKIR);
  }
  return;
}

static int32_t script_1505B(int32_t arg0, int32_t arg1) {
  Vector3 local_F4;
  int32_t local_F0;
  Script_FaceLookAtTargetUtils(arg0, 0);
  task_2BC = arg1;
  for (;;) {
    local_F0 = task_2BC;
    switch (local_F0) {
      case 0:
        goto L0151CE;
        break;
      case 1:
        Script_EVT_GetPosition(CHARA_CATM, 6, &local_F4, 0);
        Script_SetLookPointDetails(arg0, 1, &local_F4, 0);
        Script_FaceLookAtTargetUtils(arg0, 2, &local_F4, 5);
        break;
      case 2:
        Script_EVT_GetPosition(SHEN_NAME('ABRG'), -1, &local_F4, 0);
        Script_SetLookPointDetails(arg0, 1, &local_F4, 0);
        Script_FaceLookAtTargetUtils(arg0, 2, &local_F4, 5);
        break;
      case 3:
        Script_EVT_GetPosition(CHARA_MEGM, 6, &local_F4, 0);
        Script_SetLookPointDetails(arg0, 1, &local_F4, 0);
        Script_FaceLookAtTargetUtils(arg0, 2, &local_F4, 5);
        break;
      case 4:
        SCNF_WaitFrames_0000(30);
        Script_EVT_GetPosition(SHEN_NAME('BOX1'), -1, &local_F4, 0);
        Script_SetLookPointDetails(arg0, 1, &local_F4, 0);
        Script_FaceLookAtTargetUtils(arg0, 2, &local_F4, 5);
        break;
      case 5:
        Script_EVT_GetPosition(CHARA_HRSK, 6, &local_F4, 0);
        Script_SetLookPointDetails(arg0, 1, &local_F4, 0);
        Script_FaceLookAtTargetUtils(arg0, 2, &local_F4, 5);
        break;
      case 6:
        Script_GetAuthorState_And_SetCamPos(&local_F4, 0);
        Script_SetLookPointDetails(arg0, 1, &local_F4, 0);
        Script_FaceLookAtTargetUtils(arg0, 2, &local_F4, 5);
        break;
    }
    SCNF_WaitFrames_0000(1);
  }
  L0151CE:;
  Script_SetCharaLookAtPosition(arg0, -1, 0, 0);
  Script_FaceLookAtTargetUtils(arg0, 0);
  return Script_srchControl(0);
}

static int32_t script_151EA(int32_t arg0, int32_t arg1) {
  Vector3 local_F4;
  int32_t local_F0;
  task_2BD = arg1;
  Script_FaceLookAtTargetUtils(arg0, 0);
  for (;;) {
    local_F0 = task_2BD;
    switch (local_F0) {
      case 0:
        Script_SetCharaLookAtPosition(arg0, -2, 0, 0);
        goto L0152B6;
        break;
      case 1:
        Script_EVT_GetPosition(CHARA_AKIR, 5, &local_F4, 0);
        Script_SetLookPointDetails(arg0, 1, &local_F4, 0);
        break;
      case 2:
        Script_SetCharaLookAtPosition(arg0, -3, 0, 0);
        goto L0152B6;
        break;
      case 3:
        Script_EVT_GetPosition(CHARA_MEGM, 6, &local_F4, 0);
        Script_SetLookPointDetails(arg0, 1, &local_F4, 0);
        break;
      case 4:
        Script_GetAuthorState_And_SetCamPos(&local_F4, 0);
        Script_SetLookPointDetails(arg0, 1, &local_F4, 0);
        break;
    }
    SCNF_WaitFrames_0000(1);
  }
  L0152B6:;
  return Script_srchControl(0);
}

static int32_t script_152BD(int32_t arg0, int32_t arg1) {
  Vector3 local_F4;
  int32_t local_F0;
  Script_FaceLookAtTargetUtils(arg0, 0);
  task_2BE = arg1;
  for (;;) {
    local_F0 = task_2BE;
    switch (local_F0) {
      case 0:
        goto L0153C4;
        break;
      case 1:
        Script_EVT_GetPosition(CHARA_AKIR, 6, &local_F4, 0);
        Script_SetLookPointDetails(arg0, 1, &local_F4, 0);
        Script_FaceLookAtTargetUtils(arg0, 2, &local_F4, 15);
        break;
      case 2:
        Script_EVT_GetPosition(CHARA_CATM, 6, &local_F4, 0);
        Script_SetLookPointDetails(arg0, 1, &local_F4, 0);
        Script_FaceLookAtTargetUtils(arg0, 2, &local_F4, 15);
        break;
      case 3:
        Script_EVT_GetPosition(SHEN_NAME('BOX1'), -1, &local_F4, 0);
        Script_SetLookPointDetails(arg0, 1, &local_F4, 2);
        Script_FaceLookAtTargetUtils(arg0, 2, &local_F4, 5);
        break;
      case 4:
        Script_EVT_GetPosition(CHARA_HRSK, -1, &local_F4, 0);
        Script_SetLookPointDetails(arg0, 1, &local_F4, 0);
        Script_FaceLookAtTargetUtils(arg0, 2, &local_F4, 5);
        break;
    }
    SCNF_WaitFrames_0000(1);
  }
  L0153C4:;
  Script_SetCharaLookAtPosition(arg0, -1, 0, 0);
  Script_FaceLookAtTargetUtils(arg0, 0);
  return Script_srchControl(0);
}

static int32_t script_153E0(int32_t arg0, int32_t arg1) {
  Vector3 local_F4;
  int32_t local_F0;
  Script_FaceLookAtTargetUtils(arg0, 0);
  task_2BF = arg1;
  for (;;) {
    local_F0 = task_2BF;
    switch (local_F0) {
      case 0:
        goto L01551D;
        break;
      case 1:
        Script_EVT_GetPosition(CHARA_AKIR, 6, &local_F4, 0);
        Script_SetLookPointDetails(arg0, 1, &local_F4, 0);
        Script_FaceLookAtTargetUtils(arg0, 2, &local_F4, 15);
        break;
      case 2:
        Script_EVT_GetPosition(CHARA_CATM, 6, &local_F4, 0);
        Script_SetLookPointDetails(arg0, 1, &local_F4, 0);
        Script_FaceLookAtTargetUtils(arg0, 2, &local_F4, 15);
        break;
      case 3:
        Script_EVT_GetPosition(CHARA_MEGM, -1, &local_F4, 0);
        Script_SetLookPointDetails(arg0, 1, &local_F4, 0);
        Script_FaceLookAtTargetUtils(arg0, 2, &local_F4, 15);
        break;
      case 4:
        local_F4.x = -27.006901f;
        (&local_F4)->y = 2.345f;
        (&local_F4)->z = 73.822998f;
        Script_SetLookPointDetails(arg0, 98334, &local_F4, 0);
        local_F4.x = -30.289801f;
        (&local_F4)->y = 0.1407f;
        (&local_F4)->z = 74.325897f;
        Script_FaceLookAtTargetUtils(arg0, 2, &local_F4, 45);
        break;
    }
    SCNF_WaitFrames_0000(1);
  }
  L01551D:;
  Script_SetCharaLookAtPosition(arg0, -1, 0, 0);
  Script_FaceLookAtTargetUtils(arg0, 0);
  return Script_srchControl(0);
}

static void script_15539(void) {
  Script_scenUtils(SCENARIO_CMD_INIT_SCNT);
  Script_SetCPAMFlags(CHARA_CATM, 0);
  SCNF_WaitFrames_0000(1);
  Script_ryoControl(PCCTL_DisablePlayerControl);
  SCNF_RequestIconNotification(0, 0, 0, 2);
  SCNF_WaitFrames_0000(1);
  return;
}

static void script_15572(void) {
  Script_SetCPAMFlags(CHARA_CATM, 1);
  Script_ryoControl(RYO_RestoreControl);
  Script_scenUtils(SCENARIO_CMD_UPDATE_TIMER);
  return;
}

static void script_1558D(void) {
  Vector3 local_F4;
  int32_t local_E8[3];
  int32_t local_DC[3];
  int32_t local_D0[3];
  int32_t local_C4[3];
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, 0x4D47454D) == 1) {
    if (Script_IsPlayerAllowed(0)) {
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JU00_HRSK_CAT_FOLLOWUP_PENDING) == 1) {
        if (task_2C2 != 1) {
          local_E8[0] = -0x3DFE24C3;
          local_E8[1] = 0;
          local_E8[2] = 0x428FCFEC;
          local_DC[0] = -0x3DEC077A;
          local_DC[1] = 0;
          local_DC[2] = 0x428FAADB;
          local_D0[0] = -0x3DEBC588;
          local_D0[1] = 0;
          local_D0[2] = 0x428C5E28;
          local_C4[0] = -0x3DFF049C;
          local_C4[1] = 0;
          local_C4[2] = 0x428AF2B0;
          if (script_17266(CHARA_AKIR, (int32_t)local_E8, (int32_t)local_DC, (int32_t)local_D0, (int32_t)local_C4) == 1) {
            Script_ryoControl(PCCTL_DisablePlayerControl);
            Script_FaceEx(CHARA_AKIR, 1, 0, 2);
            Script_ResetHandMotion(CHARA_AKIR, -1, 0, 0);
            Script_ResetHandMotion(CHARA_AKIR, -1, 1, 0);
            Script_shenClearFreeState(0);
            SCNF_WaitFrames_0000(30);
            script_14F12("E1041A116");
            Script_scenCreateTask(script_1505B, 2, CHARA_AKIR, 1);
            script_14FB0();
            Script_motmToggleFlags(CHARA_AKIR, 1, 0x40);
            Script_motmSetBlendFrames(CHARA_AKIR, MOTM_BLEND_FRAMES_10);
            Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
            SCNF_WaitFrames_0000(15);
            script_167AB(57);
            Script_EVT_GetPosition(CHARA_CATM, -1, &local_F4, 0);
            SCNF_CHAR_SCNFApplyFacingTowardPoint(CHARA_AKIR, &local_F4, data_22688, 0.0f, 0, 0);
            script_14F21(3.0f, "E1041A170", "E1041A171", "E1041A172", 0, 0);
            script_14FB0();
            SCNF_WaitFrames_0000(30);
            task_2BC = 0;
            task_2C2 = 1;
            script_166E6();
            Script_ryoControl(RYO_RestoreControl);
            Script_FaceEx(CHARA_AKIR, 0, 0, 2);
            Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
            Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
            Script_shenClearFreeState(1);
            SCNF_WaitFrames_0000(1);
            SCNF_WaitFrames_0000(1);
            Script_SetCameraSettings(5, 6, 1, 0);
            Script_convSetCamInactiveFlag(1);
            script_167AB(0);
          }
        }
      }
    }
  }
  return;
}

static void script_157DE(void) {
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, 0x4D47454D) == 1) {
    if (script_102CA(CHARA_AKIR, CHARA_CATM, 2.0f, 50.0f)) {
      script_16635();
      if (Script_GetControllerInput(1) == 0x400) {
        if (Script_IsPlayerAllowed(0)) {
          script_15855();
        } else {
          if (Script_srchUtils(SRCH_SCRIPT_GetCurrentTargetEntryIndex) == 17) {
            script_15855();
          }
        }
      }
    } else {
      script_166A7();
    }
  }
  return;
}

__attribute__((scn_string_bindings(0, data_226EE, 1, data_226F3, 2, data_226F8, 3, data_226FD, 4, data_22702, 5, data_22707, 7, data_22717, 12, data_2274C, 26, data_227D8, 27, data_227E2, 28, data_227E7, 29, data_227EC, 30, data_227F1, 31, data_227F6, 32, data_227FB, 33, data_22800, 34, data_22807)))
static void script_15855(void) {
  Vector3 local_F4;
  int32_t unused_E8[3];
  Vector3 local_DC;
  int32_t unused_D8;
  int32_t local_D4;
  int32_t local_D0;
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 25.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 25.0f);
  Script_ryoControl(PCCTL_UpdateRyoNavYaw, 0x52494B41);
  if (task_27C != -1) {
    Script_srchControl((void *)task_27C);
    task_27C = -1;
  }
  Script_srchUtils(SRCH_SCRIPT_Reset);
  Script_SetCameraSettings(5, 6, 1, 0);
  Script_convSetCamInactiveFlag(1);
  task_288 = 0;
  script_167AB(0);
  SCNF_WaitFrames_0000(1);
  if (task_284 != -1) {
    Script_srchControl((void *)task_284);
    task_284 = -1;
  }
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_FaceEx(CHARA_AKIR, 1, 0, 2);
  Script_ResetHandMotion(CHARA_AKIR, -1, 0, 0);
  Script_ResetHandMotion(CHARA_AKIR, -1, 1, 0);
  Script_shenClearFreeState(0);
  script_166CC();
  local_D0 = script_17171(0x494B4B52);
  local_D4 = script_17171(0x4B535248);
  Script_motmToggleFlags(CHARA_CATM, 2, 0x40);
  Script_motmToggleFlags(CHARA_AKIR, 2, 0x40);
  Script_motmToggleFlags(CHARA_HRSK, 2, 0x40);
  Script_ccowUtils(CHARA_HRSK, 2);
  SCNF_RequestIconNotification(0, 0, 0, 2);
  Script_scenUtils(SCENARIO_CMD_INIT_SCNT);
  Script_PlayCharaMotion(CHARA_CATM, 60698, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_scenCreateTask(script_1505B, 2, CHARA_AKIR, 1);
  Script_scenCreateTask(script_151EA, 2, CHARA_CATM, 1);
  script_14FD1();
  Script_SetCharaRefBoardPos(CHARA_AKIR, &data_226C0);
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 1);
  Script_SetCharaRefBoardPos(CHARA_MEGM, &data_226C0);
  Script_charaSetRefboardFlagTo(CHARA_MEGM, 1);
  Script_SetCharaRefBoardPos(CHARA_CATM, &data_226C0);
  Script_charaSetRefboardFlagTo(CHARA_CATM, 1);
  Script_charaSetRefboardFlagTo(SHEN_NAME('BOX1'), 1);
  Script_SetCharaRefBoardPos(CHARA_HRSK, &data_226C0);
  Script_charaSetRefboardFlagTo(CHARA_HRSK, 1);
  Script_LoadSND_Wrapper(0, 0, "FREE", "FREE", "FREE", "FREE", "FREE", "FREE");
  Script_LoadSND_Wrapper(0, 0, "bgm018.snd", 0, "f1jutaup.snd", 0, 0, 0);
  Script_executeAicaCommand(76456, 0, 0);
  Script_executeAicaCommand(1184, 2, 110);
  script_387C(CHARA_AKIR, -25.203199f, 0.91f, 74.430496f);
  script_38A4(CHARA_AKIR, 0.0f, 190.192398f, 0.0f);
  script_167AB(12);
  script_14F12("E1041A168");
  script_14FB0();
  if (task_278 == -1) {
    task_278 = Script_scenCreateTask(script_1AB0F, 3, CHARA_CATM, 30, 0x3F800000);
  }
  Script_executeAicaCommand(66985, 0, 0);
  SCNF_WaitFrames_0000(30);
  Script_charaToggleMotEnabled(CHARA_CATM, 1);
  task_288 = 0;
  Script_facemotUtils(CHARA_HRSK, 0, 0);
  Script_SetupClothData(CHARA_HRSK, 6);
  Script_SetCPAMFlags(CHARA_HRSK, 0);
  Script_SetMotnFlag(CHARA_AKIR, 3);
  Script_SetMotnFlag(CHARA_HRSK, -1);
  script_387C(CHARA_HRSK, -28.171301f, 2.051f, 71.117104f);
  script_38A4(CHARA_HRSK, 0.0f, 202.192398f, 0.0f);
  Script_PlayCharaMotion(CHARA_HRSK, 4363, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_FaceEx(CHARA_HRSK, 1, 0, 1);
  Script_charaDisplay(CHARA_HRSK, CDM_Show);
  Script_scenCreateTask(script_153E0, 2, CHARA_HRSK, 1);
  script_167AB(47);
  script_14F12("E1041F001");
  script_14FB0();
  Script_motmToggleFlags(CHARA_AKIR, 1, 0x40);
  Script_motmSetBlendFrames(CHARA_AKIR, MOTM_BLEND_FRAMES_10);
  task_2BC = 5;
  Script_executeAicaCommand(0x5905A9, 0, 0);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_SIT_TATEHIZA_EN, 0.0f, 30.0f, 0.0f, 1.0f, 0);
  script_18EEB(CHARA_AKIR);
  Script_EVT_GetPosition(CHARA_HRSK, -1, &local_F4, 0);
  SCNF_CHAR_SCNFApplyFacingTowardPoint(CHARA_AKIR, &local_F4, data_226D0, 0.0f, 0, 0);
  script_167AB(45);
  script_14F12("E1041A169");
  script_14FB0();
  script_14F12("E1041F002");
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  script_14FB0();
  script_167AB(46);
  script_14F12("E1041A081");
  script_14FB0();
  SCNF_WaitFrames_0000(30);
  task_2BF = 4;
  script_167AB(48);
  script_14F12("E1041F008");
  script_14FB0();
  SCNF_WaitFrames_0000(60);
  task_2BD = 2;
  Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_scenToggleActiveFor(CHARA_CATM, 1);
  Script_SetCharaFlags(CHARA_CATM, 101, 1, 2.577f);
  task_2BC = 3;
  task_2BF = 3;
  script_387C(CHARA_AKIR, -37.083f, 2.591f, 42.6157f);
  script_38A4(CHARA_AKIR, 0.0f, 306.0f, 0.0f);
  script_387C(CHARA_HRSK, -36.003201f, 2.591f, 43.3321f);
  script_38A4(CHARA_HRSK, 0.0f, 325.0f, 0.0f);
  script_387C(CHARA_CATM, -35.679001f, 2.591f, 42.819599f);
  script_38A4(CHARA_CATM, 0.0f, 315.0f, 0.0f);
  script_387C(CHARA_MEGM, -34.008099f, 2.591f, 38.865601f);
  script_38A4(CHARA_MEGM, 0.0f, 58.9856f, 0.0f);
  Script_motmToggleFlags(CHARA_HRSK, 1, 0x20);
  Script_PlayCharaMotion(CHARA_HRSK, 4361, 0.0f, 55.0f, 0.0f, 0.0f, 0);
  Script_PlayCharaMotion(CHARA_CATM, 60698, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  script_167AB(13);
  Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_PlayCharaMotion(CHARA_CATM, 60727, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  script_18EEB(CHARA_CATM);
  Script_PlayCharaMotion(CHARA_CATM, 60729, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  script_18EEB(CHARA_CATM);
  Script_PlayCharaMotion(CHARA_CATM, 60673, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  script_18EEB(CHARA_CATM);
  script_14F12("E1041F004");
  script_14FB0();
  script_387C(CHARA_CATM, -34.382999f, 2.596f, 39.5158f);
  script_38A4(CHARA_CATM, 0.0f, 330.0f, 0.0f);
  Script_EVT_GetPosition(CHARA_CATM, -1, &local_F4, 0);
  Script_PlayCharaMotion(CHARA_CATM, 60698, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  script_167AB(15);
  script_14F12("E1041B023");
  Script_scenCreateTask(script_151EA, 2, CHARA_CATM, 3);
  Script_scenCreateTask(script_152BD, 2, CHARA_MEGM, 2);
  SCNF_CHAR_SCNFApplyFacingTowardPoint(CHARA_MEGM, &local_F4, data_226E0, 0.0f, 0, 0);
  Script_motmSetBlendFrames(CHARA_MEGM, MOTM_BLEND_FRAMES_20);
  Script_PlayCharaMotion(CHARA_MEGM, 4359, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  script_18EEB(CHARA_CATM);
  Script_PlayCharaMotion(CHARA_MEGM, 4358, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  script_14FB0();
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JU00_CAT_NAMED_CHIBI) == 1) {
    script_14F12("E1041B026");
  } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JU00_CAT_NAMED_TAMA) == 1) {
    script_14F12("E1041B027");
  } else {
    script_14F12("E1041B024");
  }
  script_14FB0();
  script_167AB(28);
  script_14F12("E1041B028");
  script_14FB0();
  script_167AB(29);
  script_14F12("E1041F005");
  script_14FB0();
  Script_PlayCharaMotion(CHARA_MEGM, 4357, 0.0f, 94.0f, 0.0f, 1.5f, 0);
  task_2BE = 4;
  script_167AB(30);
  script_14F12("E1041B060");
  script_14FB0();
  Script_motmToggleFlags(CHARA_HRSK, 1, 0x20);
  Script_PlayCharaMotion(CHARA_HRSK, 4361, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  script_167AB(49);
  task_2BF = 1;
  script_14F12("E1041F006");
  SCNF_WaitFrames_0000(90);
  task_2BF = 3;
  script_14FB0();
  task_2BE = 1;
  script_167AB(50);
  script_14F12("E1041B061");
  script_14FB0();
  script_14F12("E1041A158");
  script_14FB0();
  task_2BF = 1;
  task_2BC = 5;
  script_167AB(51);
  script_14F12("E1041F009");
  script_14FB0();
  task_2BF = 0;
  Script_motmToggleFlags(CHARA_HRSK, 2, 0x20);
  Script_motmToggleFlags(CHARA_HRSK, 1, 0x40);
  Script_charaToggleMotEnabled(CHARA_HRSK, 1);
  Script_motmSetBlendFrames(CHARA_HRSK, MOTM_BLEND_FRAMES_20);
  Script_PlayCharaMotion(CHARA_HRSK, 4369, 0.0f, 326.0f, 250.0f, 1.0f, 0);
  script_18EEB(CHARA_HRSK);
  script_14F12("E1041A081");
  Script_PlayCharaMotion(CHARA_HRSK, MN_AKI_AKI_WALK_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_executeAicaCommand(2720, 2, 90);
  SCNF_WaitFrames_0000(90);
  Script_ccowUtils(SHEN_NAME('BOX1'), 2);
  script_17335(1);
  script_17308(200);
  Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_JU00_HRSK_CAT_FOLLOWUP_COMPLETE, 1);
  Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 792, 1);
  Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_JU00_HRSK_CAT_FOLLOWUP_RESOLVED, 1);
  Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_KITTEN_FOUND, 1);
  Script_motmSetBlendFrames(CHARA_MEGM, MOTM_BLEND_FRAMES_5);
  Script_motmToggleFlags(CHARA_CATM, 1, 0x40);
  Script_motmToggleFlags(CHARA_AKIR, 1, 0x40);
  task_2BC = 0;
  task_2BE = 0;
  task_2BD = 2;
  Script_PlayCharaMotion(CHARA_HRSK, 0xFFFF, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_charaDisplay(CHARA_HRSK, CDM_Hide);
  Script_scenToggleActiveFor(CHARA_CATM, 1);
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 2);
  Script_charaSetRefboardFlagTo(CHARA_MEGM, 2);
  Script_charaSetRefboardFlagTo(CHARA_CATM, 2);
  Script_charaSetRefboardFlagTo(CHARA_HRSK, 2);
  Script_charaSetRefboardFlagTo(SHEN_NAME('BOX1'), 0);
  task_294 = 3;
  if (task_27C == -1) {
    task_27C = Script_scenCreateTask(script_19F93, 1, CHARA_CATM);
  }
  script_171F5(0x4B535248, local_D4);
  script_171F5(0x494B4B52, local_D0);
  script_166E6();
  Script_scenUtils(SCENARIO_CMD_SET_CHARA_RADIUS_SQUARED, 0x4D47454D, 2.0f);
  task_28C = 0;
  Script_LoadSND_Wrapper(0, 0, "FREE", "FREE", "FREE", "FREE", "FREE", "FREE");
  Script_LoadSND_Wrapper(0, 0, "SELECT", 0, "f1jutaup.snd", 0, 0, 0);
  Script_executeAicaCommand(16296, 0, 0);
  Script_executeAicaCommand(1184, 2, 100);
  Script_ccowUtils(CHARA_CATM, 2);
  Script_ccowUtils(CHARA_CATM, 5);
  Script_ccowUtils(CHARA_CATM, 4);
  Script_WeatherUtils_0(WeatherDataUtil_SetInterpolatedWeatherOverride, 1, 1, 400, 0);
  Script_scenUtils(SCENARIO_CMD_UPDATE_TIMER);
  Script_SetCameraSettings(5, 6, 1, 0);
  Script_convSetCamInactiveFlag(1);
  script_387C(SHEN_NAME('BOX1'), -34.6698f, 2.6f, 38.565601f);
  Script_EVT_GetPosition(SHEN_NAME('BOX1'), -1, &local_DC, 0);
  local_DC.x = (&local_DC)->x + -0.1638f;
  (&local_DC)->y = (&local_DC)->y;
  (&local_DC)->z = (&local_DC)->z + 1.0116f;
  script_387C(CHARA_CATM, (&local_DC)->x, (&local_DC)->y, (&local_DC)->z);
  script_38A4(CHARA_CATM, 0.0f, 121.0f, 0.0f);
  Script_EVT_GetPosition(CHARA_CATM, -1, &task_29C, ENTITY_XFORM_WORLD_SPACE);
  Script_mtwkGetSetRotation(CHARA_CATM, &task_2A8, 0x40000000);
  script_387C(CHARA_MEGM, -34.000198f, 2.591f, 39.365601f);
  script_38A4(CHARA_MEGM, 0.0f, 58.9856f, 0.0f);
  Script_PlayCharaMotion(CHARA_MEGM, 4356, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_EVT_GetPosition(CHARA_CATM, -1, &local_DC, ENTITY_XFORM_WORLD_SPACE);
  Script_SetCharaLookAtPosition(CHARA_MEGM, 98320, &local_DC, 1);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 70.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 70.0f);
  Script_ryoControl(RYO_RestoreControl);
  Script_FaceEx(CHARA_AKIR, 0, 0, 2);
  Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
  Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
  Script_shenClearFreeState(1);
  script_167AB(0);
  return;
}

static void script_16635(void) {
  if (task_298 == 1) {
    if (Script_GetConversationDetails(CONV_DETAILS_MODE_GET_GAME_STATE, 0) == 0) {
      SCNF_RequestIconNotification(0, 0, 0, 2);
      task_298 = 0;
    }
    if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_EV_STOP_ACTIVE) == 1) {
      SCNF_RequestIconNotification(0, 0, 0, 2);
      task_298 = 0;
    }
  } else {
    SCNF_RequestIconNotification(0x100, 0, 0, 1);
    task_298 = 1;
  }
  return;
}

static void script_166A7(void) {
  if (task_298 == 0) {
  } else {
    SCNF_RequestIconNotification(0, 0, 0, 2);
    task_298 = 0;
  }
  return;
}

static void script_166CC(void) {
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
  Script_CycleManagerUtils(CYCLE_CMD_PROCESS_SND);
  Script_CycleManagerUtils(CYCLE_CMD_PROCESS_SND_EXTRA);
  return;
}

static void script_166E6(void) {
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 1);
  return;
}

static void script_166F2(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t unused_FC;
  GameDateTime local_F4;
  int32_t local_F0;
  int32_t local_EC;
  int32_t local_E8;
  int8_t local_E7;
  uint8_t unused_E4[3];
  Script_ReturnTimeDate(&local_F4);
  if (arg0 == arg2) {
    if (arg1 < arg3) {
      local_E7 = 1;
    } else {
      local_E7 = 0;
    }
  } else if (arg0 < arg2) {
    local_E7 = 1;
  } else {
    local_E7 = 0;
  }
  local_F0 = arg0 * 100 + arg1;
  local_EC = arg2 * 100 + arg3;
  local_E8 = (&local_F4)->hours * 100 + (&local_F4)->minutes;
  if (local_E7) {
    if ((local_E8 >= local_F0) & (local_E8 < local_EC)) {
      local_E7 = 1;
    } else {
      local_E7 = 0;
    }
  } else if ((local_E8 >= local_F0) | (local_E8 < local_EC)) {
    local_E7 = 1;
  } else {
    local_E7 = 0;
  }
  (void)local_E7;
  return;
}

static void script_167AB(int32_t arg0) {
  int32_t local_FC;
  Script_CamCtrl(CamCtrlCmd_Reset);
  local_FC = arg0;
  switch (local_FC) {
    case 0:
      Script_SetGameStateTo(AUTH_None);
      break;
    case 1:
      Script_seqEnterEvtCam(1500, SHEN_NAME('DCAT'), SHEN_NAME('DCAT'));
      break;
    case 2:
      Script_seqEnterEvtCam(1501, CHARA_CATM, CHARA_CATM);
      break;
    case 3:
      Script_seqEnterEvtCam(1502, 0, AUTH_None);
      break;
    case 4:
      Script_seqEnterEvtCam(1503, CHARA_CATM, CHARA_CATM);
      break;
    case 5:
      Script_seqEnterEvtCam(1504, SHEN_NAME('NBOX'), SHEN_NAME('NBOX'));
      break;
    case 6:
      Script_seqEnterEvtCam(1505, CHARA_AKIR, CHARA_AKIR);
      break;
    case 7:
      Script_seqEnterEvtCam(1506, 0, AUTH_None);
      break;
    case 8:
      Script_seqEnterEvtCam(1507, CHARA_AKIR, CHARA_AKIR);
      break;
    case 9:
      Script_seqEnterEvtCam(1508, 0, AUTH_None);
      break;
    case 10:
      Script_seqEnterEvtCam(1509, CHARA_AKIR, CHARA_AKIR);
      break;
    case 11:
      Script_seqEnterEvtCam(1510, 0, AUTH_None);
      break;
    case 12:
      Script_seqEnterEvtCam(1511, 0, AUTH_None);
      break;
    case 13:
      Script_seqEnterEvtCam(1512, 0, AUTH_None);
      break;
    case 14:
      Script_seqEnterEvtCam(1513, SHEN_NAME('BOX1'), SHEN_NAME('BOX1'));
      break;
    case 15:
      Script_seqEnterEvtCam(1514, 0, AUTH_None);
      break;
    case 16:
      Script_seqEnterEvtCam(1515, SHEN_NAME('BOX1'), SHEN_NAME('BOX1'));
      break;
    case 17:
      Script_seqEnterEvtCam(1516, SHEN_NAME('BOX1'), SHEN_NAME('BOX1'));
      break;
    case 18:
      Script_seqEnterEvtCam(1517, SHEN_NAME('BOX1'), SHEN_NAME('BOX1'));
      break;
    case 19:
      Script_seqEnterEvtCam(1518, SHEN_NAME('BOX1'), SHEN_NAME('BOX1'));
      break;
    case 20:
      Script_seqEnterEvtCam(1519, SHEN_NAME('BOX1'), SHEN_NAME('BOX1'));
      break;
    case 21:
      Script_seqEnterEvtCam(1520, SHEN_NAME('BOX1'), SHEN_NAME('BOX1'));
      break;
    case 22:
      Script_seqEnterEvtCam(1521, SHEN_NAME('BOX1'), SHEN_NAME('BOX1'));
      break;
    case 23:
      Script_seqEnterEvtCam(1522, SHEN_NAME('BOX1'), SHEN_NAME('BOX1'));
      break;
    case 24:
      Script_seqEnterEvtCam(1523, SHEN_NAME('BOX1'), SHEN_NAME('BOX1'));
      break;
    case 25:
      Script_seqEnterEvtCam(1524, SHEN_NAME('BOX1'), SHEN_NAME('BOX1'));
      break;
    case 26:
      Script_seqEnterEvtCam(1525, SHEN_NAME('BOX1'), SHEN_NAME('BOX1'));
      break;
    case 27:
      Script_seqEnterEvtCam(1526, 0, AUTH_None);
      break;
    case 28:
      Script_seqEnterEvtCam(1527, 0, AUTH_None);
      break;
    case 29:
      Script_seqEnterEvtCam(1528, 0, AUTH_None);
      break;
    case 30:
      Script_seqEnterEvtCam(1529, 0, AUTH_None);
      break;
    case 31:
      Script_seqEnterEvtCam(1530, SHEN_NAME('DAKI'), SHEN_NAME('DAKI'));
      break;
    case 32:
      Script_seqEnterEvtCam(1531, SHEN_NAME('NBOX'), SHEN_NAME('NBOX'));
      break;
    case 33:
      Script_seqEnterEvtCam(1532, SHEN_NAME('NBOX'), SHEN_NAME('NBOX'));
      break;
    case 34:
      Script_seqEnterEvtCam(1533, SHEN_NAME('BOX1'), SHEN_NAME('BOX1'));
      break;
    case 35:
      Script_seqEnterEvtCam(1534, SHEN_NAME('BOX1'), SHEN_NAME('BOX1'));
      break;
    case 36:
      Script_seqEnterEvtCam(1535, SHEN_NAME('BOX1'), SHEN_NAME('BOX1'));
      break;
    case 37:
      Script_seqEnterEvtCam(0x600, SHEN_NAME('BOX1'), SHEN_NAME('BOX1'));
      break;
    case 38:
      Script_seqEnterEvtCam(1537, SHEN_NAME('BOX1'), SHEN_NAME('BOX1'));
      break;
    case 39:
      Script_seqEnterEvtCam(1538, SHEN_NAME('BOX1'), SHEN_NAME('BOX1'));
      break;
    case 40:
      Script_seqEnterEvtCam(1539, SHEN_NAME('BOX1'), SHEN_NAME('BOX1'));
      break;
    case 41:
      Script_seqEnterEvtCam(1540, SHEN_NAME('BOX1'), SHEN_NAME('BOX1'));
      break;
    case 42:
      Script_seqEnterEvtCam(1541, SHEN_NAME('BOX1'), SHEN_NAME('BOX1'));
      break;
    case 43:
      Script_seqEnterEvtCam(1542, SHEN_NAME('BOX1'), SHEN_NAME('BOX1'));
      break;
    case 44:
      Script_seqEnterEvtCam(1543, CHARA_CATM, CHARA_CATM);
      break;
    case 45:
      Script_seqEnterEvtCam(1544, 0, AUTH_None);
      break;
    case 46:
      Script_seqEnterEvtCam(1545, 0, AUTH_None);
      break;
    case 47:
      Script_seqEnterEvtCam(1546, 0, AUTH_None);
      break;
    case 48:
      Script_seqEnterEvtCam(1547, 0, AUTH_None);
      break;
    case 49:
      Script_seqEnterEvtCam(1548, 0, AUTH_None);
      break;
    case 50:
      Script_seqEnterEvtCam(1549, 0, AUTH_None);
      break;
    case 51:
      Script_seqEnterEvtCam(1550, 0, AUTH_None);
      break;
    case 52:
      Script_seqEnterEvtCam(1551, 0, AUTH_None);
      break;
    case 53:
      Script_seqEnterEvtCam(1552, 0, AUTH_None);
      break;
    case 54:
      Script_seqEnterEvtCam(1553, CHARA_CATM, CHARA_CATM);
      break;
    case 55:
      Script_seqEnterEvtCam(1554, CHARA_CATM, CHARA_CATM);
      break;
    case 56:
      Script_seqEnterEvtCam(1555, CHARA_CATM, CHARA_CATM);
      break;
    case 57:
      Script_seqEnterEvtCam(1556, 0, AUTH_None);
      break;
    case 58:
      Script_seqEnterEvtCam(1557, 0, AUTH_None);
      break;
    case 59:
      Script_seqEnterEvtCam(1558, 0, AUTH_None);
      break;
    case 60:
      Script_seqEnterEvtCam(1559, 0, AUTH_None);
      break;
    case 61:
      Script_seqEnterEvtCam(1560, 0, AUTH_None);
      break;
  }
  return;
}

static void script_16E20(float arg0, float arg1) {
  Script_GetSetMotionScale(CHARA_CATM, 1, arg0);
  Script_GetSetMotionScale(CHARA_CATM, 3, arg1);
  return;
}

static void script_16E3F(void) {
  float local_FC;
  int32_t local_F8;
  local_F8 = Script_StepUtils(GET_STEP_HIGHER, 0);
  switch (local_F8) {
    case 0:
      local_FC = 0.0f;
      break;
    case 1:
      local_FC = 0.0f;
      break;
    case 2:
      local_FC = -0.005f;
      break;
    case 3:
      local_FC = -0.008f;
      break;
    case 4:
      local_FC = -0.011f;
      break;
    case 5:
      local_FC = -0.014f;
      break;
    case 6:
      local_FC = -0.0169999991f;
      break;
    case 7:
      local_FC = -0.02f;
      break;
    case 8:
      local_FC = -0.023f;
      break;
    case 9:
      local_FC = -0.026f;
      break;
    case 10:
      local_FC = -0.029000001f;
      break;
    case 11:
      local_FC = -0.0319999978f;
      break;
    case 12:
      local_FC = -0.0319999978f;
      break;
    case 13:
      local_FC = -0.0319999978f;
      break;
    case 14:
      local_FC = -0.035f;
      break;
    case 15:
      local_FC = -0.035f;
      break;
    case 16:
      local_FC = -0.035f;
      break;
    case 17:
      local_FC = -0.038f;
      break;
    case 18:
      local_FC = -0.041f;
      break;
    case 19:
      local_FC = -0.044f;
      break;
    case 20:
      local_FC = -0.05f;
      break;
    case 21:
      local_FC = -0.05f;
      break;
    case 22:
      local_FC = -0.05f;
      break;
    case 23:
      local_FC = -0.05f;
      break;
    case 24:
      local_FC = -0.052f;
      break;
  }
  script_387C(SHEN_NAME('IKAS'), -0.003f, 0.005f, local_FC + -0.107f);
  script_38A4(SHEN_NAME('IKAS'), 90.0f, 90.0f, 0.0f);
  script_387C(SHEN_NAME('MAGR'), 0.0f, 0.012f, local_FC + -0.1f);
  script_38A4(SHEN_NAME('MAGR'), 0.0f, 90.0f, 0.0f);
  script_387C(SHEN_NAME('MLKS'), 0.0f, 0.005f, local_FC + -0.14f);
  script_38A4(SHEN_NAME('MLKS'), 0.0f, 0.0f, 0.0f);
  script_387C(SHEN_NAME('NBSI'), 0.0f, 0.0f, local_FC + -0.1f);
  script_38A4(SHEN_NAME('NBSI'), 0.0f, 0.0f, 0.0f);
  script_387C(SHEN_NAME('SRMI'), 0.025f, 0.002f, local_FC + -0.1f);
  script_38A4(SHEN_NAME('SRMI'), 0.0f, 90.0f, 0.0f);
  script_387C(SHEN_NAME('TNKN'), 0.0f, 0.0f, local_FC + -0.1f);
  script_38A4(SHEN_NAME('TNKN'), 0.0f, 0.0f, 0.0f);
  return;
}

static int32_t script_17171(int32_t arg0) {
  int32_t local_FC;
  local_FC = 0;
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
  local_FC = Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT_INTERNAL_HUMAN, arg0);
  if (local_FC == 0) {
    Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT, arg0, 1);
    local_FC = Script_CycleManagerUtils(CYCLE_CMD_GET_CYCLE_PTR_BY_CHARA_ID, arg0);
    while (local_FC == 0) {
      Script_sprintf("EVSN: Read Failed\012");
      SCNF_WaitFrames_0000(1);
      local_FC = Script_CycleManagerUtils(CYCLE_CMD_GET_CYCLE_PTR_BY_CHARA_ID, arg0);
    }
    L0171DD:;
    SCNF_WaitFrames_0000(1);
    if (!Script_CycleManagerUtils(CYCLE_CMD_GET_NEXT_TASK_POINTER, local_FC)) goto L0171DD;
  }
  return local_FC;
}

static void script_171F5(int32_t arg0, int32_t arg1) {
  if (arg1 != 0) {
    Script_CycleManagerUtils(CYCLE_CMD_LOAD_OR_RESET_RUNNING_PROGRAM, arg1);
  }
  Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT, arg0, 0);
  arg1 = 0;
  return;
}

static float script_1721B(int32_t arg0, float arg1, float arg2, float arg3, float arg4) {
  float local_FC;
  float local_F8;
  float local_F4;
  float local_F0;
  Vector3 local_E4;
  Script_EVT_GetPosition(arg0, -1, &local_E4, ENTITY_XFORM_WORLD_SPACE);
  local_F4 = arg3 - arg1;
  local_FC = (float)0 - (arg4 - arg2);
  local_F8 = (&local_E4)->x - arg1;
  local_F0 = (&local_E4)->z - arg2;
  return local_FC * local_F8 + local_F4 * local_F0;
}

static int32_t script_17266(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  int32_t local_FC;
  local_FC = 0;
  if (!((float)((scn3_bits){.f = script_1721B(arg0, *(float *)arg1, *(float *)(arg1 + 8), *(float *)arg2, *(float *)(arg2 + 8))}).i < 0.0f)) {
    if (!((float)((scn3_bits){.f = script_1721B(arg0, *(float *)arg2, *(float *)(arg2 + 8), *(float *)arg3, *(float *)(arg3 + 8))}).i < 0.0f)) {
      if (!((float)((scn3_bits){.f = script_1721B(arg0, *(float *)arg3, *(float *)(arg3 + 8), *(float *)arg4, *(float *)(arg4 + 8))}).i < 0.0f)) {
        if (!((float)((scn3_bits){.f = script_1721B(arg0, *(float *)arg4, *(float *)(arg4 + 8), *(float *)arg1, *(float *)(arg1 + 8))}).i < 0.0f)) {
          local_FC = 1;
        }
      }
    }
  }
  return local_FC;
}

static void script_17308(int32_t arg0) {
  int32_t local_FC;
  local_FC = Script_PlayerStatsUtils(PS_GetStrayCatAffinityPoints);
  if (local_FC <= 60000) {
    local_FC = local_FC + arg0;
  }
  Script_PlayerStatsUtils(PS_SetStrayCatAffinityPoints, local_FC);
  return;
}

static void script_17335(int32_t arg0) {
  int32_t local_FC;
  local_FC = Script_PlayerStatsUtils(PS_GetStrayCatInteractionCount);
  if (local_FC <= 60000) {
    local_FC = local_FC + arg0;
  }
  Script_PlayerStatsUtils(PS_SetStrayCatInteractionCount, local_FC);
  return;
}

static int32_t script_17362(int32_t arg0) {
  Script_filmCheckOrSetTransitionUtil(arg0, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  return Script_srchControl(0);
}

static void script_17396(int32_t arg0, float arg1, float arg2, float arg3, float arg4) {
  float local_F4[3];
  float local_E8[3];
  float local_DC[3];
  float local_D0[3];
  Vector3 local_C4;
  float local_B8[3];
  Vector3i local_AC;
  float local_A8;
  float local_A4;
  float local_A0;
  int32_t unused_98[2];
  float local_94;
  float local_90;
  float local_8C;
  int32_t unused_84[2];
  float local_80;
  float local_FF7C;
  float local_FF78;
  float local_FF74;
  int32_t unused_FF6C[2];
  int8_t local_FF6B;
  uint8_t unused_FF68[3];
  local_E8[0] = arg1;
  local_E8[1] = 0.0f;
  local_E8[2] = arg2;
  local_DC[0] = arg3;
  local_DC[1] = 0.0f;
  local_DC[2] = arg4;
  local_FF6B = 0;
  Script_EVT_GetPosition(arg0, -1, &local_C4, 0);
  Script_mtwkGetSetRotation(arg0, &local_AC, 0);
  local_B8[0] = (&local_C4)->x - Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_AC)->y) * 0.8f;
  local_B8[2] = (&local_C4)->z - Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_AC)->y) * 0.8f;
  local_D0[0] = (&local_C4)->x - *local_B8;
  local_D0[2] = (&local_C4)->z - local_B8[2];
  local_8C = Script_MathUtils(SCRIPT_MATH_VEC2_LENGTH, *local_D0, local_D0[2]);
  local_F4[0] = *local_E8 - *local_DC;
  local_F4[2] = local_E8[2] - local_DC[2];
  local_A0 = Script_MathUtils(SCRIPT_MATH_VEC2_LENGTH, *local_F4, local_F4[2]);
  if (!(local_8C >= local_A0)) {
    if (!((&local_C4)->x >= *local_B8)) {
      local_FF78 = (&local_C4)->x;
      local_FF74 = *local_B8;
    } else {
      local_FF78 = *local_B8;
      local_FF74 = (&local_C4)->x;
    }
  } else if (!(*local_E8 >= local_DC[2])) {
    local_FF78 = *local_E8;
    local_FF74 = *local_DC;
  } else {
    local_FF78 = *local_DC;
    local_FF74 = *local_E8;
  }
  local_A8 = local_F4[2] / *local_F4;
  local_A4 = local_E8[2] - local_A8 * *local_E8;
  if (!(*local_D0 <= -1.00000004e-36f) & !(*local_D0 >= 1.00000004e-36f)) {
    local_80 = *local_D0;
    local_FF7C = local_80 * local_A8 + local_A4;
  } else {
    local_94 = local_D0[2] / *local_D0;
    local_90 = (&local_C4)->z - local_94 * (&local_C4)->x;
    local_80 = (local_A4 - local_90) / (local_94 - local_A8);
    local_FF7C = local_80 * local_94 + local_90;
  }
  if (!(local_FF78 >= local_80) & !(local_80 >= local_FF74)) {
    local_FF6B = 1;
  }
  (void)local_FF6B;
  return;
}

__attribute__((scn_string_bindings(2, data_2283A, 3, data_2284A)))
static void script_17598(int32_t arg0) {
  task_274 = -1;
  task_278 = -1;
  task_27C = -1;
  task_280 = -1;
  task_284 = -1;
  Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 792, 0);
  Script_LoadVoiceFile(1, "E1041");
  Script_LoadHandMot(3, SHEN_NAME('JU00'), "HMOT_CAT.BIN");
  script_18F14();
  task_291 = 0;
  task_28F = 0;
  task_28E = 0;
  task_296 = 0;
  task_298 = 0;
  task_28C = 0;
  task_292 = 0;
  script_1034F(arg0);
  Script_facemotUtils(arg0, 0, 0);
  Script_FaceEx(arg0, 1, 0, 1);
  Script_PlayCharaMotion(arg0, 60716, 0.0f, 60.0f, 0.0f, 1.5f, 0);
  script_1BFE5("/scene/03/JU00/", "CATA3");
  return;
}

static void script_17667(int32_t arg0) {
  script_387C(SHEN_NAME('BOX1'), -34.6698f, 2.6f, 38.565601f);
  Script_ccowSetFlag80(arg0, 1);
  if (task_27C != -1) {
    Script_srchControl((void *)task_27C);
    task_27C = -1;
  }
  if (task_280 != -1) {
    Script_srchControl((void *)task_280);
    task_280 = -1;
  }
  Script_FACE(arg0, 0, 0, 0);
  Script_FaceLookAtTargetUtils(arg0, 0);
  script_1773E(arg0);
  Script_SetCPAMFlags(arg0, 0);
  Script_MoveObjectToObject(SHEN_NAME('MLKS'), 0x54414344);
  Script_MoveObjectToObject(SHEN_NAME('MAGR'), 0x54414344);
  Script_MoveObjectToObject(SHEN_NAME('NBSI'), 0x54414344);
  Script_MoveObjectToObject(SHEN_NAME('TNKN'), 0x54414344);
  Script_MoveObjectToObject(SHEN_NAME('SRMI'), 0x54414344);
  Script_MoveObjectToObject(SHEN_NAME('IKAS'), 0x54414344);
  script_16E3F();
  return;
}

static void script_1773E(int32_t arg0) {
  int32_t local_FC;
  local_FC = Script_StepUtils(GET_STEP_HIGHER, 0);
  switch (local_FC) {
    case 24:
      script_16E20(1.5f, 1.4f);
      script_184FE(arg0);
      break;
    case 23:
      script_16E20(1.48f, 1.37f);
      if (-(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JU00_CATA1_INTRO_COMPLETE) == 1) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JU00_STRAY_CAT_STAGE2_COMPLETE) == 1) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JU00_STRAY_CAT_STAGE3_COMPLETE) == 1) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JU00_STRAY_CAT_STAGE4_COMPLETE) == 1) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JU00_STRAY_CAT_STAGE5_COMPLETE) == 1) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JU00_HRSK_CAT_FOLLOWUP_COMPLETE) == 1) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JU00_MEGUMI_KEEPS_CAT) == 1) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JU00_STRAY_CAT_STAGE7_COMPLETE) == 1) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 776) == 1)) {
        if (Script_PlayerStatsUtils(PS_GetStrayCatAffinityPoints) >= 2500) {
          script_180D2(arg0);
        } else {
          script_180DE(arg0);
        }
      } else if (Script_PlayerStatsUtils(PS_GetStrayCatAffinityPoints) >= 1000) {
        script_180DE(arg0);
      } else {
        script_181F4(arg0);
      }
      break;
    case 22:
      script_16E20(1.48f, 1.37f);
      script_184FE(arg0);
      break;
    case 21:
      script_16E20(1.48f, 1.37f);
      script_184FE(arg0);
      break;
    case 20:
      script_16E20(1.48f, 1.37f);
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 776) == 1) {
        script_18564(arg0);
      } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JU00_MEGUMI_KEEPS_CAT) == 0) {
        if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, 0x4D47454D) == 1) {
          if ((script_1031A() != 2) & (script_1031A() != 3)) {
            script_1836A(arg0);
          } else {
            script_18665(arg0);
          }
        } else {
          script_18665(arg0);
        }
      } else {
        script_18665(arg0);
      }
      break;
    case 19:
      script_16E20(1.42f, 1.32f);
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JU00_HRSK_CAT_FOLLOWUP_COMPLETE) == 0) {
        if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, 0x4D47454D) == 1) {
          if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JU00_HRSK_CAT_FOLLOWUP_PENDING) == 0) {
            if (script_1031A() != 2) {
              script_185DE(arg0);
            } else {
              script_18665(arg0);
            }
          } else {
            script_1843E(arg0);
          }
        } else {
          script_18665(arg0);
        }
      } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JU00_HRSK_CAT_FOLLOWUP_COMPLETE) == 1) {
        script_18665(arg0);
      } else {
        script_18665(arg0);
      }
      break;
    case 18:
      script_16E20(1.39f, 1.3f);
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JU00_HRSK_CAT_FOLLOWUP_COMPLETE) == 0) {
        if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, 0x4D47454D) == 1) {
          if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JU00_HRSK_CAT_FOLLOWUP_PENDING) == 0) {
            if (script_1031A() != 2) {
              script_185DE(arg0);
            } else {
              script_18665(arg0);
            }
          } else {
            script_1843E(arg0);
          }
        } else {
          script_18665(arg0);
        }
      } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JU00_HRSK_CAT_FOLLOWUP_COMPLETE) == 1) {
        script_18665(arg0);
      } else {
        script_18665(arg0);
      }
      break;
    case 17:
      script_16E20(1.36f, 1.27f);
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JU00_HRSK_CAT_FOLLOWUP_COMPLETE) == 0) {
        if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, 0x4D47454D) == 1) {
          if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JU00_HRSK_CAT_FOLLOWUP_PENDING) == 0) {
            if (script_1031A() != 2) {
              script_185DE(arg0);
            } else {
              script_18665(arg0);
            }
          } else {
            script_1843E(arg0);
          }
        } else {
          script_18665(arg0);
        }
      } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JU00_HRSK_CAT_FOLLOWUP_COMPLETE) == 1) {
        script_18665(arg0);
      } else {
        script_18665(arg0);
      }
      break;
    case 16:
      script_16E20(1.33f, 1.24f);
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JU00_STRAY_CAT_STAGE5_COMPLETE) == 1) {
        script_18273(arg0);
      } else {
        script_18665(arg0);
      }
      break;
    case 15:
      script_16E20(1.33f, 1.24f);
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JU00_STRAY_CAT_STAGE5_COMPLETE) == 1) {
        script_18273(arg0);
      } else {
        script_18665(arg0);
      }
      break;
    case 14:
      script_16E20(1.33f, 1.24f);
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JU00_STRAY_CAT_STAGE5_COMPLETE) == 1) {
        script_18273(arg0);
      } else {
        script_18665(arg0);
      }
      break;
    case 13:
      script_16E20(1.3f, 1.21f);
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JU00_STRAY_CAT_STAGE4_COMPLETE) == 1) {
        script_1891E(arg0);
      } else {
        script_1875C(arg0);
      }
      break;
    case 12:
      script_16E20(1.3f, 1.21f);
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JU00_STRAY_CAT_STAGE4_COMPLETE) == 1) {
        script_1891E(arg0);
      } else {
        script_1875C(arg0);
      }
      break;
    case 11:
      script_16E20(1.3f, 1.21f);
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JU00_STRAY_CAT_STAGE4_COMPLETE) == 1) {
        script_1891E(arg0);
      } else {
        script_1875C(arg0);
      }
      break;
    case 10:
      script_16E20(1.27f, 1.18f);
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JU00_STRAY_CAT_STAGE7_COMPLETE) == 1) {
        script_1883D(arg0);
      } else {
        script_1883D(arg0);
      }
      break;
    case 9:
      script_16E20(1.24f, 1.16f);
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JU00_STRAY_CAT_STAGE7_COMPLETE) == 1) {
        script_1883D(arg0);
      } else {
        script_1883D(arg0);
      }
      break;
    case 8:
      script_16E20(1.21f, 1.14f);
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JU00_STRAY_CAT_STAGE7_COMPLETE) == 1) {
        script_1883D(arg0);
      } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JU00_STRAY_CAT_STAGE3_COMPLETE) == 1) {
        script_1883D(arg0);
      } else {
        script_189F5(arg0);
      }
      break;
    case 7:
      script_16E20(1.18f, 1.12f);
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JU00_STRAY_CAT_STAGE7_COMPLETE) == 1) {
        script_1883D(arg0);
      } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JU00_STRAY_CAT_STAGE3_COMPLETE) == 1) {
        script_1883D(arg0);
      } else {
        script_189F5(arg0);
      }
      break;
    case 6:
      script_16E20(1.15f, 1.1f);
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JU00_STRAY_CAT_STAGE3_COMPLETE) == 1) {
        script_189F5(arg0);
      } else {
        script_189F5(arg0);
      }
      break;
    case 5:
      script_16E20(1.12f, 1.08f);
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JU00_STRAY_CAT_STAGE3_COMPLETE) == 1) {
        script_189F5(arg0);
      } else {
        script_189F5(arg0);
      }
      break;
    case 4:
      script_16E20(1.09f, 1.06f);
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JU00_STRAY_CAT_STAGE2_COMPLETE) == 1) {
        script_1891E(arg0);
      } else {
        script_189F5(arg0);
      }
      break;
    case 3:
      script_16E20(1.06f, 1.04f);
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JU00_STRAY_CAT_STAGE2_COMPLETE) == 1) {
        script_1891E(arg0);
      } else {
        script_189F5(arg0);
      }
      break;
    case 2:
      script_16E20(1.03f, 1.02f);
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JU00_STRAY_CAT_STAGE2_COMPLETE) == 1) {
        script_1891E(arg0);
      } else {
        script_189F5(arg0);
      }
      break;
    case 1:
      script_16E20(1.0f, 1.0f);
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JU00_STRAY_CAT_STAGE2_COMPLETE) == 1) {
        script_1891E(arg0);
      } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JU00_CATA1_INTRO_COMPLETE) == 1) {
        script_189F5(arg0);
      } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JU00_CATA1_INTRO_COMPLETE) == 0) {
        if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, 0x4D47454D) == 0) {
          script_184FE(arg0);
        } else {
          script_18AE2(arg0);
        }
      } else {
        script_18AE2(arg0);
      }
      break;
    case 0:
      script_16E20(1.0f, 1.0f);
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JU00_STRAY_CAT_STAGE2_COMPLETE) == 1) {
        script_1891E(arg0);
      } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JU00_CATA1_INTRO_COMPLETE) == 1) {
        script_189F5(arg0);
      } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JU00_CATA1_INTRO_COMPLETE) == 0) {
        if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, 0x4D47454D) == 0) {
          script_184FE(arg0);
        } else {
          script_18AE2(arg0);
        }
      } else {
        script_18AE2(arg0);
      }
      break;
  }
  return;
}

static void script_180D2(int32_t arg0) {
  script_184FE(arg0);
  return;
}

static void script_180DE(int32_t arg0) {
  Script_SetCharaRefBoardPos(arg0, &data_22850);
  Script_charaSetRefboardFlagTo(arg0, 1);
  Script_ccowUtils(arg0, 2);
  script_387C(arg0, -39.666698f, 7.009f, 37.2467f);
  script_38A4(arg0, 0.0f, 196.0f, 0.0f);
  Script_PlayCharaMotion(arg0, 60698, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_scenToggleActiveFor(arg0, 0);
  script_17171(0x41544143);
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 1);
  Script_GetSetMotionScale(SHEN_NAME('CATA'), 1, 0.65f);
  Script_GetSetMotionScale(SHEN_NAME('CATA'), 3, 0.65f);
  Script_ccowUtils(SHEN_NAME('CATA'), 2);
  script_387C(SHEN_NAME('CATA'), -39.8316f, 7.009f, 37.333599f);
  script_38A4(SHEN_NAME('CATA'), 0.0f, 223.0f, 0.0f);
  Script_PlayCharaMotion(SHEN_NAME('CATA'), 60698, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_scenToggleActiveFor(SHEN_NAME('CATA'), 0);
  return;
}

static void script_181F4(int32_t arg0) {
  Script_ccowUtils(arg0, 2);
  script_387C(arg0, -36.7477f, 5.294f, 41.556599f);
  script_38A4(arg0, 0.0f, 178.0f, 0.0f);
  Script_PlayCharaMotion(arg0, 60698, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_scenToggleActiveFor(CHARA_CATM, 0);
  if (task_280 == -1) {
    task_280 = Script_scenCreateTask(script_1AA8E, 1, arg0);
  }
  return;
}

static void script_18273(int32_t arg0) {
  Vector3 local_F4;
  Script_ccowUtils(arg0, 2);
  Script_ccowUtils(arg0, 5);
  Script_ccowUtils(arg0, 4);
  Script_EVT_GetPosition(SHEN_NAME('BOX1'), -1, &local_F4, 0);
  local_F4.x = (&local_F4)->x + -0.1638f;
  (&local_F4)->y = (&local_F4)->y;
  (&local_F4)->z = (&local_F4)->z + 1.0116f;
  script_387C(arg0, (&local_F4)->x, (&local_F4)->y, (&local_F4)->z);
  script_38A4(arg0, 0.0f, 121.0f, 0.0f);
  Script_EVT_GetPosition(arg0, -1, &task_29C, ENTITY_XFORM_WORLD_SPACE);
  Script_mtwkGetSetRotation(arg0, &task_2A8, 0x40000000);
  if (task_27C == -1) {
    task_27C = Script_scenCreateTask(script_19E89, 1, arg0);
  }
  task_294 = 3;
  Script_SetCharaFlags(arg0, 101, 1, 2.577f);
  task_296 = 0;
  if (task_280 == -1) {
    task_280 = Script_scenCreateTask(script_1AA8E, 1, arg0);
  }
  return;
}

static void script_1836A(int32_t arg0) {
  int32_t unused_F4[3];
  Script_ccowUtils(arg0, 2);
  script_387C(arg0, -38.724899f, 6.283f, 37.789902f);
  script_38A4(arg0, 0.0f, 309.951813f, 0.0f);
  task_28E = 1;
  Script_PlayCharaMotion(arg0, 60675, 0.0f, 271.0f, 0.0f, 0.0f, 0);
  script_387C(CHARA_MEGM, -35.598999f, 2.591f, 39.761501f);
  script_38A4(CHARA_MEGM, 0.0f, 58.9856f, 0.0f);
  Script_PlayCharaMotion(CHARA_MEGM, 4356, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  task_294 = 0;
  Script_WeatherUtils_0(WeatherDataUtil_SetInterpolatedWeatherOverride, 1, 0, 150, 0);
  Script_WeatherUtils_0(WeatherDataUtil_SetInterpolatedWeatherOverride, 2, 0, 150, 0);
  task_296 = 0;
  return;
}

static void script_1843E(int32_t arg0) {
  Vector3 local_F4;
  Script_ccowUtils(arg0, 2);
  Script_EVT_GetPosition(SHEN_NAME('BOX1'), -1, &local_F4, 0);
  script_387C(arg0, -24.1693f, 0.92f, 75.0616f);
  script_38A4(arg0, 0.0f, 45.0f, 0.0f);
  if (task_27C == -1) {
    task_27C = Script_scenCreateTask(script_19B0D, 1, arg0);
  }
  task_294 = 3;
  Script_SetCharaFlags(arg0, 101, 1, 0.91f);
  Script_scenUtils(SCENARIO_CMD_SET_CHARA_RADIUS_SQUARED, 0x4D47454D, 0.0f);
  task_28C = 1;
  Script_WeatherUtils_0(WeatherDataUtil_SetInterpolatedWeatherOverride, 1, 0, 400, 0);
  task_296 = 0;
  if (task_280 == -1) {
    task_280 = Script_scenCreateTask(script_1AA8E, 1, arg0);
  }
  return;
}

static void script_184FE(int32_t arg0) {
  Script_ccowUtils(arg0, 2);
  Script_charaDisplay(SHEN_NAME('BOX1'), CDM_Hide);
  Script_charaDisplay(arg0, CDM_Hide);
  script_387C(SHEN_NAME('BOX1'), 1000.169312f, 0.91f, 75.0616f);
  script_387C(arg0, 1000.169312f, 0.92f, 75.0616f);
  task_28E = 1;
  task_294 = 0;
  task_296 = 0;
  return;
}

static void script_18564(int32_t arg0) {
  Script_ccowUtils(arg0, 2);
  Script_charaDisplay(SHEN_NAME('BOX1'), CDM_Hide);
  Script_charaDisplay(arg0, CDM_Hide);
  script_387C(SHEN_NAME('BOX1'), 1000.169312f, 0.91f, 75.0616f);
  script_387C(arg0, 1000.169312f, 0.92f, 75.0616f);
  task_28E = 1;
  task_294 = 0;
  task_28C = 1;
  Script_scenUtils(SCENARIO_CMD_SET_CHARA_RADIUS_SQUARED, 0x4D47454D, 0.0f);
  task_296 = 0;
  return;
}

static void script_185DE(int32_t arg0) {
  Script_ccowUtils(arg0, 2);
  Script_charaDisplay(arg0, CDM_Hide);
  script_387C(arg0, -24.1693f, 0.92f, 75.0616f);
  script_38A4(arg0, 0.0f, 45.0f, 0.0f);
  task_28E = 1;
  task_294 = 0;
  Script_PlayCharaMotion(arg0, 60698, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  task_28C = 1;
  Script_scenUtils(SCENARIO_CMD_SET_CHARA_RADIUS_SQUARED, 0x4D47454D, 0.0f);
  task_296 = 0;
  return;
}

static void script_18665(int32_t arg0) {
  Vector3 local_F4;
  Script_ccowUtils(arg0, 2);
  Script_ccowUtils(arg0, 5);
  Script_ccowUtils(arg0, 4);
  Script_EVT_GetPosition(SHEN_NAME('BOX1'), -1, &local_F4, 0);
  local_F4.x = (&local_F4)->x + -0.1638f;
  (&local_F4)->y = (&local_F4)->y;
  (&local_F4)->z = (&local_F4)->z + 1.0116f;
  script_387C(arg0, (&local_F4)->x, (&local_F4)->y, (&local_F4)->z);
  script_38A4(arg0, 0.0f, 121.0f, 0.0f);
  Script_EVT_GetPosition(arg0, -1, &task_29C, ENTITY_XFORM_WORLD_SPACE);
  Script_mtwkGetSetRotation(arg0, &task_2A8, 0x40000000);
  if (task_27C == -1) {
    task_27C = Script_scenCreateTask(script_19F93, 1, arg0);
  }
  task_294 = 3;
  Script_SetCharaFlags(arg0, 101, 1, 2.577f);
  task_296 = 0;
  if (task_280 == -1) {
    task_280 = Script_scenCreateTask(script_1AA8E, 1, arg0);
  }
  return;
}

static void script_1875C(int32_t arg0) {
  Vector3 local_F4;
  Script_ccowUtils(arg0, 2);
  Script_EVT_GetPosition(SHEN_NAME('BOX1'), -1, &local_F4, 0);
  local_F4.x = (&local_F4)->x;
  (&local_F4)->y = (&local_F4)->y + 0.01f;
  (&local_F4)->z = (&local_F4)->z;
  script_387C(arg0, (&local_F4)->x, (&local_F4)->y, (&local_F4)->z);
  script_38A4(arg0, 0.0f, 121.0f, 0.0f);
  Script_EVT_GetPosition(arg0, -1, &task_29C, ENTITY_XFORM_WORLD_SPACE);
  Script_mtwkGetSetRotation(arg0, &task_2A8, 0x40000000);
  if (task_27C == -1) {
    task_27C = Script_scenCreateTask(script_19B0D, 1, arg0);
  }
  task_294 = 3;
  Script_SetCharaFlags(arg0, 101, 1, 2.595f);
  task_296 = 1;
  if (task_280 == -1) {
    task_280 = Script_scenCreateTask(script_1AA8E, 1, arg0);
  }
  return;
}

static void script_1883D(int32_t arg0) {
  Vector3 local_F4;
  Script_ccowUtils(arg0, 2);
  Script_EVT_GetPosition(SHEN_NAME('BOX1'), -1, &local_F4, 0);
  local_F4.x = (&local_F4)->x;
  (&local_F4)->y = (&local_F4)->y + 0.01f;
  (&local_F4)->z = (&local_F4)->z;
  script_387C(arg0, (&local_F4)->x, (&local_F4)->y, (&local_F4)->z);
  script_38A4(arg0, 0.0f, 121.0f, 0.0f);
  Script_EVT_GetPosition(arg0, -1, &task_29C, ENTITY_XFORM_WORLD_SPACE);
  Script_mtwkGetSetRotation(arg0, &task_2A8, 0x40000000);
  if (task_27C == -1) {
    task_27C = Script_scenCreateTask(script_19007, 1, arg0);
  }
  task_294 = 2;
  Script_SetCharaFlags(arg0, 101, 1, 2.595f);
  task_296 = 1;
  if (task_280 == -1) {
    task_280 = Script_scenCreateTask(script_1AA8E, 1, arg0);
  }
  return;
}

static void script_1891E(int32_t arg0) {
  Vector3 local_F4;
  Script_ccowUtils(arg0, 2);
  Script_EVT_GetPosition(SHEN_NAME('BOX1'), -1, &local_F4, 0);
  local_F4.x = (&local_F4)->x - 0.001f;
  (&local_F4)->y = (&local_F4)->y + 0.015f;
  (&local_F4)->z = (&local_F4)->z - 0.046f;
  script_387C(arg0, (&local_F4)->x, (&local_F4)->y, (&local_F4)->z);
  script_38A4(arg0, 0.0f, 121.0f, 0.0f);
  Script_EVT_GetPosition(arg0, -1, &task_29C, ENTITY_XFORM_WORLD_SPACE);
  Script_mtwkGetSetRotation(arg0, &task_2A8, 0x40000000);
  task_28F = 1;
  task_28E = 1;
  if (task_27C == -1) {
    task_27C = Script_scenCreateTask(script_192C6, 1, arg0);
  }
  Script_SetCharaFlags(arg0, 101, 1, 2.595f);
  task_296 = 1;
  return;
}

static void script_189F5(int32_t arg0) {
  Vector3 local_F4;
  Script_ccowUtils(arg0, 2);
  Script_EVT_GetPosition(SHEN_NAME('BOX1'), -1, &local_F4, 0);
  local_F4.x = (&local_F4)->x - 0.001f;
  (&local_F4)->y = (&local_F4)->y + 0.015f;
  (&local_F4)->z = (&local_F4)->z - 0.046f;
  script_387C(arg0, (&local_F4)->x, (&local_F4)->y, (&local_F4)->z);
  script_38A4(arg0, 0.0f, 121.0f, 0.0f);
  Script_EVT_GetPosition(arg0, -1, &task_29C, ENTITY_XFORM_WORLD_SPACE);
  Script_mtwkGetSetRotation(arg0, &task_2A8, 0x40000000);
  if (task_27C == -1) {
    task_27C = Script_scenCreateTask(script_19007, 1, arg0);
  }
  task_294 = 1;
  Script_SetCharaFlags(arg0, 101, 1, 2.595f);
  task_296 = 1;
  if (task_280 == -1) {
    task_280 = Script_scenCreateTask(script_1AA8E, 1, arg0);
  }
  return;
}

static void script_18AE2(int32_t arg0) {
  Vector3 local_F4;
  Script_ccowUtils(arg0, 2);
  Script_EVT_GetPosition(SHEN_NAME('BOX1'), -1, &local_F4, 0);
  local_F4.x = (&local_F4)->x - 0.001f;
  (&local_F4)->y = (&local_F4)->y + 0.015f;
  (&local_F4)->z = (&local_F4)->z - 0.046f;
  script_387C(arg0, (&local_F4)->x, (&local_F4)->y, (&local_F4)->z);
  script_38A4(arg0, 0.0f, 121.0f, 0.0f);
  Script_EVT_GetPosition(arg0, -1, &task_29C, ENTITY_XFORM_WORLD_SPACE);
  Script_mtwkGetSetRotation(arg0, &task_2A8, 0x40000000);
  if (task_27C == -1) {
    task_27C = Script_scenCreateTask(script_18F6F, 1, arg0);
  }
  task_294 = 1;
  Script_SetCharaFlags(arg0, 101, 1, 2.595f);
  task_296 = 1;
  if (task_280 == -1) {
    task_280 = Script_scenCreateTask(script_1AA8E, 1, arg0);
  }
  return;
}

static void script_18BCF(int32_t arg0) {
  int32_t local_F4[3];
  int32_t local_E8[3];
  int32_t local_DC[3];
  int32_t local_D0[3];
  int32_t local_C4[3];
  int32_t local_B8[3];
  int32_t local_AC[3];
  int32_t local_A0[3];
  local_F4[0] = -0x3E2D59B4;
  local_F4[1] = 0;
  local_F4[2] = 0x423A8C15;
  local_E8[0] = -0x3DD9A0DF;
  local_E8[1] = 0;
  local_E8[2] = 0x4259A320;
  local_DC[0] = -0x3DBC5F70;
  local_DC[1] = 0;
  local_DC[2] = 0x421F648F;
  local_D0[0] = -0x3DF33DA5;
  local_D0[1] = 0;
  local_D0[2] = 0x420375F7;
  local_C4[0] = -0x3DF73AAD;
  local_C4[1] = 0;
  local_C4[2] = 0x4215AF00;
  local_B8[0] = -0x3E060347;
  local_B8[1] = 0;
  local_B8[2] = 0x422B0F42;
  local_AC[0] = -0x3DE85FD9;
  local_AC[1] = 0;
  local_AC[2] = 0x42398BE1;
  local_A0[0] = -0x3DD9D014;
  local_A0[1] = 0;
  local_A0[2] = 0x4221E24E;
  if (script_17266(CHARA_AKIR, (int32_t)local_F4, (int32_t)local_E8, (int32_t)local_DC, (int32_t)local_D0) == 1) {
    if (task_274 == -1) {
      task_274 = Script_scenCreateTask(script_18D63, 1, arg0);
    }
    if (script_17266(CHARA_AKIR, (int32_t)local_C4, (int32_t)local_B8, (int32_t)local_AC, (int32_t)local_A0) != 1) {
      task_290 = 1;
    } else {
      task_290 = 0;
    }
  } else {
    if (task_274 != -1) {
      Script_srchControl((void *)task_274);
      task_274 = -1;
    }
  }
  return;
}

static void script_18D43(int32_t arg0) {
  if (task_284 == -1) {
    task_284 = Script_scenCreateTask(script_18E1F, 1, arg0);
  }
  return;
}

static int32_t script_18D63(int32_t arg0) {
  Vector3 local_F4;
  int32_t local_F0;
  local_F0 = 0;
  for (;;) {
    if (task_28D == 1) {
      break;
    }
    if (task_28F == 1) {
      for (;;) {
        if (task_28F == 0) {
          break;
        }
        SCNF_WaitFrames_0000(1);
      }
    }
    if (task_28E != 1) {
      if (task_290 == 1) {
        if (local_F0 == 0) {
          Script_EVT_GetPosition(arg0, -1, &local_F4, 0);
          Script_Execute3DSoundCommand_Wrapper(66985, 4, &local_F4, 0.0f, 30.0f, 0);
          local_F0 = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 90.0f));
          local_F0 = local_F0 + 120;
        }
        local_F0 = local_F0 - 1;
      }
      goto L018E06;
    }
    L018E06:;
    SCNF_WaitFrames_0000(1);
  }
  Script_srchControl(0);
  return task_274 = -1;
}

static int32_t script_18E1F(int32_t arg0) {
  Vector3 local_F4;
  int32_t local_F0;
  local_F0 = 0;
  for (;;) {
    if (task_28E != 1) {
      if (task_290 == 1) {
        if (local_F0 == 0) {
          Script_EVT_GetPosition(arg0, -1, &local_F4, 0);
          Script_Execute3DSoundCommand_Wrapper(66985, 4, &local_F4, 0.0f, 30.0f, 0);
          local_F0 = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 90.0f));
          local_F0 = local_F0 + 120;
        }
        local_F0 = local_F0 - 1;
      }
      goto L018E93;
    }
    L018E93:;
    SCNF_WaitFrames_0000(1);
  }
  Script_srchControl(0);
  return task_284 = -1;
}

static void script_18EAC(void) {
  int32_t local_FC;
  int8_t local_FB;
  local_FB = 0;
  Script_WeatherUtils(WeatherScript_StoreRainIntensityToOffset, (float *)&local_FC);
  if (local_FC == 0) {
    Script_WeatherUtils(WeatherScript_StoreSnowIntensityToOffset, (float *)&local_FC);
    if (local_FC != 0) {
      local_FB = 2;
    }
  } else {
    local_FB = 1;
  }
  (void)local_FB;
  return;
}

static void script_18EEB(int32_t arg0) {
  for (;;) {
    if (Script_IsCharaMotionFinished(arg0)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static int32_t script_18F05(void) {
  task_293 = Script_GetCurrentEntryPoint();
  return task_293;
}

static void script_18F14(void) {
  int32_t unused_FC;
  GameDateTime local_F4;
  int8_t local_F3;
  uint8_t unused_F0[3];
  if ((Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JU00_HRSK_CAT_FOLLOWUP_PENDING) == 1) & (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JU00_HRSK_CAT_FOLLOWUP_RESOLVED) == 0)) {
    Script_ReturnTimeDate(&local_F4);
    local_F3 = Script_scenUtils(SCENARIO_CMD_GET_STEP_LUT, 17);
    if (local_F3 == (&local_F4)->day) {
    } else {
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_JU00_HRSK_CAT_FOLLOWUP_COMPLETE, 1);
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_JU00_HRSK_CAT_FOLLOWUP_RESOLVED, 1);
    }
  }
  return;
}

static int32_t script_18F6F(int32_t arg0) {
  task_294 = 1;
  Script_charaToggleMotEnabled(arg0, 1);
  Script_SetMotnFlag(arg0, -1);
  Script_PlayCharaMotion(arg0, 60681, 0.0f, 60.0f, 0.0f, 1.5f, 0);
  for (;;) {
    if (task_291 == 1) {
      Script_SetMotnFlag(arg0, 0);
      task_291 = 0;
      break;
    }
    if (Script_scenCharaGetMotAnimId(arg0) != 60681) {
      Script_PlayCharaMotion(arg0, 60681, 0.0f, 60.0f, 0.0f, 1.5f, 0);
    }
    SCNF_WaitFrames_0000(1);
  }
  task_27C = -1;
  return Script_srchControl(0);
}

static int32_t script_19007(int32_t arg0) {
  int8_t local_FF;
  task_294 = 1;
  Script_charaToggleMotEnabled(arg0, 1);
  Script_SetMotnFlag(arg0, -1);
  for (;;) {
    if (task_291 == 1) {
      Script_SetMotnFlag(arg0, 0);
      task_291 = 0;
      break;
    }
    local_FF = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 9.0f));
    if (local_FF == 0) {
      Script_PlayCharaMotion(arg0, 60681, 0.0f, 60.0f, 0.0f, 1.5f, 0);
    } else if (local_FF == 1) {
      Script_PlayCharaMotion(arg0, 60682, 0.0f, 165.0f, 0.0f, 1.5f, 0);
      if (task_278 == -1) {
        task_278 = Script_scenCreateTask(script_1AB0F, 3, arg0, 24, 0x3FC00000);
      }
    } else if (local_FF == 2) {
      Script_PlayCharaMotion(arg0, 60691, 0.0f, 120.0f, 0.0f, 1.5f, 0);
      if (task_278 == -1) {
        task_278 = Script_scenCreateTask(script_1AB0F, 3, arg0, 29, 0x3FC00000);
      }
    } else if (local_FF == 3) {
      Script_PlayCharaMotion(arg0, 60680, 0.0f, 120.0f, 0.0f, 1.5f, 0);
      if (task_278 == -1) {
        task_278 = Script_scenCreateTask(script_1AB0F, 3, arg0, 23, 0x3FC00000);
      }
    } else if (local_FF == 4) {
      Script_PlayCharaMotion(arg0, 60688, 0.0f, 60.0f, 0.0f, 1.5f, 0);
    } else if (local_FF == 5) {
      Script_PlayCharaMotion(arg0, 60687, 0.0f, 130.0f, 0.0f, 1.5f, 0);
    } else if (local_FF == 6) {
      Script_PlayCharaMotion(arg0, 60678, 0.0f, 160.0f, 0.0f, 1.5f, 0);
      if (task_278 == -1) {
        task_278 = Script_scenCreateTask(script_1AB0F, 3, arg0, 21, 0x3FC00000);
      }
    } else if (local_FF == 7) {
      Script_PlayCharaMotion(arg0, 60689, 0.0f, 60.0f, 0.0f, 1.5f, 0);
    } else if (local_FF == 8) {
      Script_PlayCharaMotion(arg0, 60677, 0.0f, 220.0f, 0.0f, 1.5f, 0);
      if (task_278 == -1) {
        task_278 = Script_scenCreateTask(script_1AB0F, 3, arg0, 20, 0x3FC00000);
      }
    } else {
      Script_PlayCharaMotion(arg0, 60681, 0.0f, 60.0f, 0.0f, 1.5f, 0);
    }
    for (;;) {
      if (Script_IsCharaMotionFinished(arg0)) {
        task_288 = 0;
        break;
      }
      if (task_291 == 1) {
        task_288 = 0;
        break;
      }
      SCNF_WaitFrames_0000(1);
    }
  }
  task_27C = -1;
  return Script_srchControl(0);
}

static int32_t script_192C6(int32_t arg0) {
  int32_t local_FC;
  int8_t local_FB;
  int8_t local_FA;
  int8_t local_F9;
  int32_t local_F4;
  local_FB = 0;
  local_FA = 0;
  local_F9 = 0;
  local_FC = 0;
  task_294 = 4;
  Script_charaToggleMotEnabled(arg0, 1);
  Script_SetMotnFlag(arg0, -1);
  if (task_278 == -1) {
    task_278 = Script_scenCreateTask(script_1AB0F, 3, arg0, 28, 0x3F800000);
  }
  for (;;) {
    if (task_291 == 1) {
      task_291 = 0;
      Script_charaToggleMotEnabled(arg0, 0);
      Script_SetMotnFlag(arg0, 0);
      task_288 = 0;
      break;
    }
    local_FB = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f));
    local_F4 = local_FA;
    switch (local_F4) {
      case 0:
        if (local_FC < 150) {
          local_FB = 1;
          local_FC = local_FC + 1;
        }
        if (local_FB == 0) {
          Script_PlayCharaMotion(arg0, 60741, 0.0f, 38.0f, 0.0f, 1.0f, 0);
          local_FC = 0;
          local_FA = 1;
        } else {
          Script_PlayCharaMotion(arg0, 60685, 0.0f, 60.0f, 0.0f, 1.0f, 0);
        }
        break;
      case 1:
        if (local_FC < 150) {
          local_FB = 1;
          local_FC = local_FC + 1;
        }
        if (local_FB == 0) {
          Script_motmToggleFlags(arg0, 1, 0x20);
          Script_PlayCharaMotion(arg0, 60741, 0.0f, 38.0f, 0.0f, 1.0f, 0);
          local_FC = 0;
          local_FA = 0;
        } else {
          Script_PlayCharaMotion(arg0, 60686, 0.0f, 60.0f, 0.0f, 1.0f, 0);
        }
        break;
    }
    for (;;) {
      if (Script_IsCharaMotionFinished(arg0)) {
        Script_motmToggleFlags(arg0, 2, 0x20);
        break;
      }
      if (task_291 == 1) {
        task_288 = 0;
        break;
      }
      SCNF_WaitFrames_0000(1);
    }
  }
  Script_motmToggleFlags(arg0, 2, 0x40);
  task_27C = -1;
  return Script_srchControl(0);
}

static void script_194A5(int32_t arg0) {
  int8_t local_FF;
  int8_t local_FE;
  int8_t local_FD;
  local_FE = 0;
  local_FD = 0;
  Script_charaToggleMotEnabled(arg0, 1);
  Script_SetMotnFlag(arg0, -1);
  for (;;) {
    if (task_291 == 1) {
      Script_motmToggleFlags(arg0, 2, 0x20);
      Script_charaToggleMotEnabled(arg0, 0);
      Script_SetMotnFlag(arg0, 0);
      task_291 = 0;
      break;
    }
    local_FF = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 10.0f));
    {
      uint8_t unused_FC;
      int32_t local_F8;
      local_F8 = local_FE;
      switch (local_F8) {
        case 1:
          task_294 = 1;
          if (local_FF == 0) {
            Script_motmToggleFlags(arg0, 1, 0x20);
            Script_PlayCharaMotion(arg0, 60730, 0.0f, 30.0f, 0.0f, 1.5f, 0);
            local_FD = 1;
          } else if (local_FF == 1) {
            Script_PlayCharaMotion(arg0, 60681, 0.0f, 60.0f, 0.0f, 1.5f, 0);
          } else if (local_FF == 2) {
            Script_PlayCharaMotion(arg0, 60682, 0.0f, 165.0f, 0.0f, 1.5f, 0);
            if (task_278 == -1) {
              task_278 = Script_scenCreateTask(script_1AB0F, 3, arg0, 24, 0x3FC00000);
            }
          } else if (local_FF == 3) {
            Script_PlayCharaMotion(arg0, 60691, 0.0f, 120.0f, 0.0f, 1.5f, 0);
            if (task_278 == -1) {
              task_278 = Script_scenCreateTask(script_1AB0F, 3, arg0, 29, 0x3FC00000);
            }
          } else if (local_FF == 4) {
            Script_PlayCharaMotion(arg0, 60680, 0.0f, 120.0f, 0.0f, 1.5f, 0);
            if (task_278 == -1) {
              task_278 = Script_scenCreateTask(script_1AB0F, 3, arg0, 23, 0x3FC00000);
            }
          } else if (local_FF == 5) {
            Script_PlayCharaMotion(arg0, 60688, 0.0f, 60.0f, 0.0f, 1.5f, 0);
          } else if (local_FF == 6) {
            Script_PlayCharaMotion(arg0, 60687, 0.0f, 130.0f, 0.0f, 1.5f, 0);
          } else if (local_FF == 7) {
            Script_PlayCharaMotion(arg0, 60678, 0.0f, 160.0f, 0.0f, 1.5f, 0);
            if (task_278 == -1) {
              task_278 = Script_scenCreateTask(script_1AB0F, 3, arg0, 21, 0x3FC00000);
            }
          } else if (local_FF == 8) {
            Script_PlayCharaMotion(arg0, 60689, 0.0f, 60.0f, 0.0f, 1.5f, 0);
          } else if (local_FF == 9) {
            Script_PlayCharaMotion(arg0, 60677, 0.0f, 220.0f, 0.0f, 1.5f, 0);
            if (task_278 == -1) {
              task_278 = Script_scenCreateTask(script_1AB0F, 3, arg0, 20, 0x3FC00000);
            }
          } else {
            Script_PlayCharaMotion(arg0, 60681, 0.0f, 60.0f, 0.0f, 1.5f, 0);
          }
          break;
        case 2:
          task_294 = 2;
          if (local_FF == 0) {
            Script_PlayCharaMotion(arg0, 60730, 0.0f, 30.0f, 0.0f, 1.5f, 0);
            local_FD = 2;
          } else if (local_FF == 1) {
            Script_PlayCharaMotion(arg0, 60716, 0.0f, 60.0f, 0.0f, 1.5f, 0);
          } else if (local_FF == 2) {
            Script_PlayCharaMotion(arg0, 60717, 0.0f, 150.0f, 0.0f, 1.5f, 0);
            if (task_278 == -1) {
              task_278 = Script_scenCreateTask(script_1AB0F, 3, arg0, 16, 0x3FC00000);
            }
          } else if (local_FF == 3) {
            Script_PlayCharaMotion(arg0, 60713, 0.0f, 120.0f, 0.0f, 1.5f, 0);
            if (task_278 == -1) {
              task_278 = Script_scenCreateTask(script_1AB0F, 3, arg0, 15, 0x3FC00000);
            }
          } else if (local_FF == 4) {
            Script_PlayCharaMotion(arg0, 60725, 0.0f, 120.0f, 0.0f, 1.5f, 0);
            if (task_278 == -1) {
              task_278 = Script_scenCreateTask(script_1AB0F, 3, arg0, 19, 0x3FC00000);
            }
          } else if (local_FF == 5) {
            Script_PlayCharaMotion(arg0, 60720, 0.0f, 60.0f, 0.0f, 1.5f, 0);
          } else if (local_FF == 6) {
            Script_PlayCharaMotion(arg0, 60719, 0.0f, 144.0f, 0.0f, 1.5f, 0);
            if (task_278 == -1) {
              task_278 = Script_scenCreateTask(script_1AB0F, 3, arg0, 18, 0x3FC00000);
            }
          } else if (local_FF == 7) {
            Script_PlayCharaMotion(arg0, 60716, 0.0f, 60.0f, 0.0f, 1.5f, 0);
          } else if (local_FF == 8) {
            Script_PlayCharaMotion(arg0, 60721, 0.0f, 60.0f, 0.0f, 1.5f, 0);
          } else if (local_FF == 9) {
            Script_PlayCharaMotion(arg0, 60711, 0.0f, 150.0f, 0.0f, 1.5f, 0);
            if (task_278 == -1) {
              task_278 = Script_scenCreateTask(script_1AB0F, 3, arg0, 13, 0x3FC00000);
            }
          } else {
            Script_PlayCharaMotion(arg0, 60716, 0.0f, 60.0f, 0.0f, 1.5f, 0);
          }
          break;
      }
    }
    for (;;) {
      if (Script_IsCharaMotionFinished(arg0)) {
        Script_motmToggleFlags(arg0, 2, 0x20);
        task_288 = 0;
        break;
      }
      if (task_291 == 1) {
        Script_motmToggleFlags(arg0, 2, 0x20);
        task_288 = 0;
        break;
      }
      SCNF_WaitFrames_0000(1);
    }
    switch (local_FD) {
      case 1:
        local_FE = 2;
        break;
      case 2:
        local_FE = 1;
        break;
    }
    local_FD = 0;
    {
      uint8_t unused_FC;
      int32_t local_F8;
      local_F8 = local_FE;
      switch (local_F8) {
        case 1:
          if (local_FF == 1) {
          } else {
            Script_PlayCharaMotion(arg0, 60681, 0.0f, 60.0f, 0.0f, 1.5f, 0);
          }
          break;
        case 2:
          if ((local_FF == 1) | (local_FF == 7)) {
          } else {
            Script_PlayCharaMotion(arg0, 60716, 0.0f, 60.0f, 0.0f, 1.5f, 0);
          }
          break;
      }
    }
    for (;;) {
      if (Script_IsCharaMotionFinished(arg0)) {
        break;
      }
      if (task_291 == 1) {
        break;
      }
      SCNF_WaitFrames_0000(1);
    }
  }
  task_27C = -1;
  Script_srchControl(0);
  return;
}

static int32_t script_19B0D(int32_t arg0) {
  int8_t local_FF;
  task_294 = 3;
  Script_charaToggleMotEnabled(arg0, 1);
  Script_SetMotnFlag(arg0, -1);
  for (;;) {
    if (task_291 == 1) {
      task_291 = 0;
      Script_SetMotnFlag(arg0, 0);
      break;
    }
    local_FF = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f));
    if (local_FF == 0) {
      Script_PlayCharaMotion(arg0, 60698, 0.0f, 60.0f, 0.0f, 1.5f, 0);
    } else if (local_FF == 1) {
      Script_PlayCharaMotion(arg0, 60699, 0.0f, 136.0f, 0.0f, 1.5f, 0);
      if (task_278 == -1) {
        task_278 = Script_scenCreateTask(script_1AB0F, 3, arg0, 7, 0x3FC00000);
      }
    } else if (local_FF == 2) {
      Script_PlayCharaMotion(arg0, 60708, 0.0f, 120.0f, 0.0f, 1.5f, 0);
      if (task_278 == -1) {
        task_278 = Script_scenCreateTask(script_1AB0F, 3, arg0, 12, 0x3FC00000);
      }
    } else if (local_FF == 3) {
      Script_PlayCharaMotion(arg0, 60706, 0.0f, 210.0f, 0.0f, 1.5f, 0);
    } else if (local_FF == 4) {
      Script_PlayCharaMotion(arg0, 60701, 0.0f, 194.0f, 0.0f, 1.5f, 0);
      if (task_278 == -1) {
        task_278 = Script_scenCreateTask(script_1AB0F, 3, arg0, 9, 0x3FC00000);
      }
    } else if (local_FF == 5) {
      Script_PlayCharaMotion(arg0, 60696, 0.0f, 120.0f, 0.0f, 1.5f, 0);
      if (task_278 == -1) {
        task_278 = Script_scenCreateTask(script_1AB0F, 3, arg0, 5, 0x3FC00000);
      }
    } else if (local_FF == 6) {
      Script_PlayCharaMotion(arg0, 60704, 0.0f, 60.0f, 0.0f, 1.5f, 0);
    } else if (local_FF == 7) {
      Script_PlayCharaMotion(arg0, 60692, 0.0f, 200.0f, 0.0f, 1.5f, 0);
      if (task_278 == -1) {
        task_278 = Script_scenCreateTask(script_1AB0F, 3, arg0, 1, 0x3FC00000);
      }
    } else if (local_FF == 8) {
      Script_PlayCharaMotion(arg0, 60702, 0.0f, 185.0f, 0.0f, 1.5f, 0);
      if (task_278 == -1) {
        task_278 = Script_scenCreateTask(script_1AB0F, 3, arg0, 10, 0x3FC00000);
      }
    } else if (local_FF == 9) {
      Script_PlayCharaMotion(arg0, 60705, 0.0f, 120.0f, 0.0f, 1.5f, 0);
    } else if (local_FF == 10) {
      Script_PlayCharaMotion(arg0, 60703, 0.0f, 200.0f, 0.0f, 1.5f, 0);
      if (task_278 == -1) {
        task_278 = Script_scenCreateTask(script_1AB0F, 3, arg0, 11, 0x3FC00000);
      }
    } else if (local_FF == 11) {
      Script_PlayCharaMotion(arg0, 60707, 0.0f, 200.0f, 0.0f, 1.5f, 0);
    } else {
      Script_PlayCharaMotion(arg0, 60698, 0.0f, 60.0f, 0.0f, 1.5f, 0);
    }
    for (;;) {
      if (Script_IsCharaMotionFinished(arg0)) {
        task_288 = 0;
        break;
      }
      if (task_291 == 1) {
        task_288 = 0;
        break;
      }
      SCNF_WaitFrames_0000(1);
    }
  }
  task_27C = -1;
  return Script_srchControl(0);
}

static int32_t script_19E89(int32_t arg0) {
  Script_SetMotnFlag(arg0, -1);
  Script_charaToggleMotEnabled(arg0, 1);
  task_294 = 3;
  for (;;) {
    if (task_291 == 1) {
      task_291 = 0;
      Script_SetMotnFlag(arg0, 0);
      break;
    }
    Script_PlayCharaMotion(arg0, 60701, 0.0f, 194.0f, 0.0f, 1.5f, 0);
    if (task_278 == -1) {
      task_278 = Script_scenCreateTask(script_1AB0F, 3, arg0, 9, 0x3FC00000);
    }
    for (;;) {
      if (Script_scenCharaGetMotAnimId(arg0) != 60701) {
        Script_PlayCharaMotion(arg0, 60701, 0.0f, 194.0f, 0.0f, 1.5f, 0);
        if (task_278 == -1) {
          task_278 = Script_scenCreateTask(script_1AB0F, 3, arg0, 9, 0x3FC00000);
        }
      }
      if (Script_IsCharaMotionFinished(arg0)) {
        task_288 = 0;
        break;
      }
      if (task_291 == 1) {
        task_288 = 0;
        break;
      }
      SCNF_WaitFrames_0000(1);
    }
  }
  task_27C = -1;
  return Script_srchControl(0);
}

static int32_t script_19F93(int32_t arg0) {
  int8_t local_FF;
  int8_t local_FE;
  int8_t local_FD;
  local_FE = 0;
  local_FD = 0;
  Script_charaToggleMotEnabled(arg0, 1);
  Script_SetMotnFlag(arg0, -1);
  for (;;) {
    if (task_291 == 1) {
      Script_motmToggleFlags(arg0, 2, 0x20);
      Script_charaToggleMotEnabled(arg0, 0);
      Script_SetMotnFlag(arg0, 0);
      task_291 = 0;
      break;
    }
    local_FF = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 13.0f));
    {
      uint8_t unused_FC;
      int32_t local_F8;
      local_F8 = local_FE;
      switch (local_F8) {
        case 0:
          task_294 = 1;
          if (local_FF == 0) {
            Script_motmToggleFlags(arg0, 1, 0x20);
            Script_PlayCharaMotion(arg0, 60730, 0.0f, 59.0f, 0.0f, 1.5f, 0);
            local_FD = 4;
          } else if (local_FF == 1) {
            Script_PlayCharaMotion(arg0, 60681, 0.0f, 59.0f, 0.0f, 1.5f, 0);
          } else if (local_FF == 2) {
            Script_PlayCharaMotion(arg0, 60682, 0.0f, 165.0f, 0.0f, 1.5f, 0);
            if (task_278 == -1) {
              task_278 = Script_scenCreateTask(script_1AB0F, 3, arg0, 24, 0x3FC00000);
            }
          } else if (local_FF == 3) {
            Script_PlayCharaMotion(arg0, 60691, 0.0f, 120.0f, 0.0f, 1.5f, 0);
            if (task_278 == -1) {
              task_278 = Script_scenCreateTask(script_1AB0F, 3, arg0, 29, 0x3FC00000);
            }
          } else if (local_FF == 4) {
            Script_PlayCharaMotion(arg0, 60680, 0.0f, 120.0f, 0.0f, 1.5f, 0);
            if (task_278 == -1) {
              task_278 = Script_scenCreateTask(script_1AB0F, 3, arg0, 23, 0x3FC00000);
            }
          } else if (local_FF == 5) {
            Script_PlayCharaMotion(arg0, 60681, 0.0f, 60.0f, 0.0f, 1.5f, 0);
          } else if (local_FF == 6) {
            Script_PlayCharaMotion(arg0, 60681, 0.0f, 60.0f, 0.0f, 1.5f, 0);
          } else if (local_FF == 7) {
            Script_PlayCharaMotion(arg0, 60688, 0.0f, 60.0f, 0.0f, 1.5f, 0);
          } else if (local_FF == 8) {
            Script_PlayCharaMotion(arg0, 60687, 0.0f, 130.0f, 0.0f, 1.5f, 0);
          } else if (local_FF == 9) {
            Script_PlayCharaMotion(arg0, 60678, 0.0f, 160.0f, 0.0f, 1.5f, 0);
            if (task_278 == -1) {
              task_278 = Script_scenCreateTask(script_1AB0F, 3, arg0, 21, 0x3FC00000);
            }
          } else if (local_FF == 10) {
            Script_PlayCharaMotion(arg0, 60689, 0.0f, 60.0f, 0.0f, 1.5f, 0);
          } else if (local_FF == 11) {
            Script_PlayCharaMotion(arg0, 60677, 0.0f, 220.0f, 0.0f, 1.5f, 0);
            if (task_278 == -1) {
              task_278 = Script_scenCreateTask(script_1AB0F, 3, arg0, 20, 0x3FC00000);
            }
          } else if (local_FF == 12) {
            Script_PlayCharaMotion(arg0, 60681, 0.0f, 60.0f, 0.0f, 1.5f, 0);
          } else {
            Script_PlayCharaMotion(arg0, 60681, 0.0f, 60.0f, 0.0f, 1.5f, 0);
          }
          break;
        case 1:
          task_294 = 2;
          if (local_FF == 0) {
            Script_motmToggleFlags(arg0, 1, 0x20);
            Script_PlayCharaMotion(arg0, 60728, 0.0f, 30.0f, 0.0f, 1.5f, 0);
            local_FD = 3;
          } else if (local_FF == 1) {
            Script_PlayCharaMotion(arg0, 60730, 0.0f, 30.0f, 0.0f, 1.5f, 0);
            local_FD = 2;
          } else if (local_FF == 2) {
            Script_PlayCharaMotion(arg0, 60716, 0.0f, 60.0f, 0.0f, 1.5f, 0);
          } else if (local_FF == 3) {
            Script_PlayCharaMotion(arg0, 60717, 0.0f, 150.0f, 0.0f, 1.5f, 0);
            if (task_278 == -1) {
              task_278 = Script_scenCreateTask(script_1AB0F, 3, arg0, 16, 0x3FC00000);
            }
          } else if (local_FF == 4) {
            Script_PlayCharaMotion(arg0, 60713, 0.0f, 120.0f, 0.0f, 1.5f, 0);
            if (task_278 == -1) {
              task_278 = Script_scenCreateTask(script_1AB0F, 3, arg0, 15, 0x3FC00000);
            }
          } else if (local_FF == 5) {
            Script_PlayCharaMotion(arg0, 60725, 0.0f, 120.0f, 0.0f, 1.5f, 0);
            if (task_278 == -1) {
              task_278 = Script_scenCreateTask(script_1AB0F, 3, arg0, 19, 0x3FC00000);
            }
          } else if (local_FF == 6) {
            Script_PlayCharaMotion(arg0, 60716, 0.0f, 60.0f, 0.0f, 1.5f, 0);
          } else if (local_FF == 7) {
            Script_PlayCharaMotion(arg0, 60720, 0.0f, 60.0f, 0.0f, 1.5f, 0);
          } else if (local_FF == 8) {
            Script_PlayCharaMotion(arg0, 60719, 0.0f, 144.0f, 0.0f, 1.5f, 0);
            if (task_278 == -1) {
              task_278 = Script_scenCreateTask(script_1AB0F, 3, arg0, 18, 0x3FC00000);
            }
          } else if (local_FF == 9) {
            Script_PlayCharaMotion(arg0, 60716, 0.0f, 60.0f, 0.0f, 1.5f, 0);
          } else if (local_FF == 10) {
            Script_PlayCharaMotion(arg0, 60721, 0.0f, 60.0f, 0.0f, 1.5f, 0);
          } else if (local_FF == 11) {
            Script_PlayCharaMotion(arg0, 60711, 0.0f, 150.0f, 0.0f, 1.5f, 0);
            if (task_278 == -1) {
              task_278 = Script_scenCreateTask(script_1AB0F, 3, arg0, 13, 0x3FC00000);
            }
          } else if (local_FF == 12) {
            Script_PlayCharaMotion(arg0, 60716, 0.0f, 60.0f, 0.0f, 1.5f, 0);
          } else {
            Script_PlayCharaMotion(arg0, 60716, 0.0f, 60.0f, 0.0f, 1.5f, 0);
          }
          break;
        case 2:
          task_294 = 3;
          if (local_FF == 0) {
            Script_PlayCharaMotion(arg0, 60728, 0.0f, 30.0f, 0.0f, 1.5f, 0);
            local_FD = 1;
          } else if (local_FF == 1) {
            Script_PlayCharaMotion(arg0, 60698, 0.0f, 60.0f, 0.0f, 1.5f, 0);
          } else if (local_FF == 2) {
            Script_PlayCharaMotion(arg0, 60699, 0.0f, 136.0f, 0.0f, 1.5f, 0);
            if (task_278 == -1) {
              task_278 = Script_scenCreateTask(script_1AB0F, 3, arg0, 7, 0x3FC00000);
            }
          } else if (local_FF == 3) {
            Script_PlayCharaMotion(arg0, 60708, 0.0f, 120.0f, 0.0f, 1.5f, 0);
            if (task_278 == -1) {
              task_278 = Script_scenCreateTask(script_1AB0F, 3, arg0, 12, 0x3FC00000);
            }
          } else if (local_FF == 4) {
            Script_PlayCharaMotion(arg0, 60706, 0.0f, 210.0f, 0.0f, 1.5f, 0);
          } else if (local_FF == 5) {
            Script_PlayCharaMotion(arg0, 60701, 0.0f, 194.0f, 0.0f, 1.5f, 0);
            if (task_278 == -1) {
              task_278 = Script_scenCreateTask(script_1AB0F, 3, arg0, 9, 0x3FC00000);
            }
          } else if (local_FF == 6) {
            Script_PlayCharaMotion(arg0, 60696, 0.0f, 120.0f, 0.0f, 1.5f, 0);
            if (task_278 == -1) {
              task_278 = Script_scenCreateTask(script_1AB0F, 3, arg0, 5, 0x3FC00000);
            }
          } else if (local_FF == 7) {
            Script_PlayCharaMotion(arg0, 60704, 0.0f, 60.0f, 0.0f, 1.5f, 0);
          } else if (local_FF == 8) {
            Script_PlayCharaMotion(arg0, 60692, 0.0f, 200.0f, 0.0f, 1.5f, 0);
            if (task_278 == -1) {
              task_278 = Script_scenCreateTask(script_1AB0F, 3, arg0, 1, 0x3FC00000);
            }
          } else if (local_FF == 9) {
            Script_PlayCharaMotion(arg0, 60702, 0.0f, 185.0f, 0.0f, 1.5f, 0);
            if (task_278 == -1) {
              task_278 = Script_scenCreateTask(script_1AB0F, 3, arg0, 10, 0x3FC00000);
            }
          } else if (local_FF == 10) {
            Script_PlayCharaMotion(arg0, 60705, 0.0f, 120.0f, 0.0f, 1.5f, 0);
          } else if (local_FF == 11) {
            Script_PlayCharaMotion(arg0, 60703, 0.0f, 200.0f, 0.0f, 1.5f, 0);
            if (task_278 == -1) {
              task_278 = Script_scenCreateTask(script_1AB0F, 3, arg0, 11, 0x3FC00000);
            }
          } else if (local_FF == 12) {
            Script_PlayCharaMotion(arg0, 60707, 0.0f, 200.0f, 0.0f, 1.5f, 0);
          } else {
            Script_PlayCharaMotion(arg0, 60698, 0.0f, 60.0f, 0.0f, 1.5f, 0);
          }
          break;
      }
    }
    for (;;) {
      if (Script_IsCharaMotionFinished(arg0)) {
        Script_motmToggleFlags(arg0, 2, 0x20);
        task_288 = 0;
        break;
      }
      if (task_291 == 1) {
        Script_motmToggleFlags(arg0, 2, 0x20);
        break;
      }
      SCNF_WaitFrames_0000(1);
    }
    switch (local_FD) {
      case 1:
        local_FE = 1;
        break;
      case 2:
        local_FE = 0;
        break;
      case 3:
        local_FE = 2;
        break;
      case 4:
        local_FE = 1;
        break;
    }
    local_FD = 0;
    {
      uint8_t unused_FC;
      int32_t local_F8;
      local_F8 = local_FE;
      switch (local_F8) {
        case 0:
          if ((local_FF == 1) | (local_FF == 5) | (local_FF == 6) | (local_FF == 12)) {
          } else {
            Script_PlayCharaMotion(arg0, 60681, 0.0f, 60.0f, 0.0f, 1.5f, 0);
          }
          break;
        case 1:
          if ((local_FF == 2) | (local_FF == 6) | (local_FF == 9) | (local_FF == 12)) {
          } else {
            Script_PlayCharaMotion(arg0, 60716, 0.0f, 60.0f, 0.0f, 1.5f, 0);
          }
          break;
        case 2:
          if (local_FF == 1) {
          } else {
            Script_PlayCharaMotion(arg0, 60698, 0.0f, 60.0f, 0.0f, 1.5f, 0);
          }
          break;
      }
    }
    for (;;) {
      if (Script_IsCharaMotionFinished(arg0)) {
        break;
      }
      if (task_291 == 1) {
        break;
      }
      SCNF_WaitFrames_0000(1);
    }
  }
  task_27C = -1;
  return Script_srchControl(0);
}

static int32_t script_1AA8E(int32_t arg0) {
  int32_t local_FC;
  Script_FACE_LCG(arg0, 2, 1, 0);
  for (;;) {
    if (task_297 == 1) {
      break;
    }
    local_FC = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 400.0f)) + 400;
    SCNF_WaitFrames_0000(local_FC);
    Script_FACE_LCG(arg0, 1, 2, 0);
    SCNF_WaitFrames_0000(2);
    Script_FACE_LCG(arg0, 2, 2, 0);
    SCNF_WaitFrames_0000(2);
  }
  Script_srchControl(0);
  task_280 = -1;
  task_297 = 0;
}

static int32_t script_1AB0F(int32_t arg0, int32_t arg1, float arg2) {
  int32_t local_FC;
  float local_F8;
  float local_F4;
  int32_t local_F0;
  task_288 = arg1;
  local_FC = 0;
  local_F8 = 0.0f;
  local_F4 = arg2;
  for (;;) {
    local_F0 = task_288;
    switch (local_F0) {
      case 0:
        goto L01B68F;
        break;
      case 1:
        switch (local_FC) {
          case 1:
            Script_FACE_LCG(arg0, 1, 20, 0);
            Script_FACE(arg0, 0, 4, 50);
            break;
          case 150:
            Script_FACE(arg0, 0, 0, 50);
            break;
          case 180:
            Script_FACE_LCG(arg0, 2, 20, 0);
            break;
        }
        break;
      case 2:
        switch (local_FC) {
          case 25:
            Script_FACE_LCG(arg0, 1, 5, 0);
            break;
          case 100:
            Script_FACE_LCG(arg0, 2, 5, 0);
            break;
        }
        break;
      case 3:
        switch (local_FC) {
          case 20:
            Script_FACE(arg0, 0, 3, 10);
            break;
          case 150:
            Script_FACE(arg0, 0, 0, 10);
            break;
        }
        break;
      case 4:
        switch (local_FC) {
          case 20:
            Script_FACE_LCG(arg0, 1, 20, 0);
            break;
          case 200:
            Script_FACE_LCG(arg0, 2, 20, 0);
            break;
        }
        break;
      case 5:
        switch (local_FC) {
          case 10:
            Script_FACE(arg0, 0, 5, 5);
            break;
          case 25:
            Script_FACE(arg0, 0, 0, 5);
            break;
        }
        break;
      case 6:
        switch (local_FC) {
          case 1:
            Script_FACE(arg0, 0, 5, 20);
            break;
          case 220:
            Script_FACE(arg0, 0, 0, 18);
            break;
        }
        break;
      case 7:
        switch (local_FC) {
          case 20:
            Script_FACE(arg0, 0, 5, 5);
            break;
          case 35:
            Script_FACE(arg0, 0, 0, 5);
            break;
        }
        break;
      case 8:
        switch (local_FC) {
          case 20:
            Script_FACE_LCG(arg0, 1, 10, 0);
            break;
          case 140:
            Script_FACE_LCG(arg0, 2, 10, 0);
            break;
        }
        break;
      case 9:
        switch (local_FC) {
          case 25:
            Script_FACE(arg0, 0, 3, 5);
            break;
          case 165:
            Script_FACE(arg0, 0, 0, 5);
            break;
        }
        break;
      case 10:
        switch (local_FC) {
          case 10:
            Script_FACE(arg0, 0, 5, 5);
            break;
          case 25:
            Script_FACE(arg0, 0, 0, 5);
            break;
        }
        break;
      case 11:
        switch (local_FC) {
          case 50:
            Script_FACE_LCG(arg0, 1, 20, 0);
            break;
          case 160:
            Script_FACE_LCG(arg0, 2, 20, 0);
            break;
        }
        break;
      case 12:
        switch (local_FC) {
          case 30:
            Script_FACE(arg0, 0, 5, 10);
            break;
          case 40:
            Script_FACE(arg0, 0, 0, 10);
            break;
        }
        break;
      case 13:
        switch (local_FC) {
          case 1:
            Script_FACE_LCG(arg0, 1, 20, 0);
            Script_FACE(arg0, 0, 4, 30);
            break;
          case 120:
            Script_FACE(arg0, 0, 0, 30);
            break;
          case 130:
            Script_FACE_LCG(arg0, 2, 20, 0);
            break;
        }
        break;
      case 14:
        switch (local_FC) {
          case 20:
            Script_FACE_LCG(arg0, 1, 20, 0);
            break;
          case 150:
            Script_FACE_LCG(arg0, 2, 20, 0);
            break;
        }
        break;
      case 15:
        switch (local_FC) {
          case 15:
            Script_FACE(arg0, 0, 5, 5);
            break;
          case 30:
            Script_FACE(arg0, 0, 0, 5);
            break;
        }
        break;
      case 16:
        switch (local_FC) {
          case 10:
            Script_FACE(arg0, 0, 5, 5);
            break;
          case 25:
            Script_FACE(arg0, 0, 0, 5);
            break;
        }
        break;
      case 17:
        switch (local_FC) {
          case 10:
            Script_FACE_LCG(arg0, 1, 10, 0);
            break;
          case 160:
            Script_FACE_LCG(arg0, 2, 10, 0);
            break;
        }
        break;
      case 18:
        switch (local_FC) {
          case 15:
            Script_FACE(arg0, 0, 5, 5);
            break;
          case 30:
            Script_FACE(arg0, 0, 0, 5);
            break;
        }
        break;
      case 19:
        switch (local_FC) {
          case 20:
            Script_FACE(arg0, 0, 5, 10);
            break;
          case 30:
            Script_FACE(arg0, 0, 0, 10);
            break;
        }
        break;
      case 20:
        switch (local_FC) {
          case 1:
            Script_FACE_LCG(arg0, 1, 20, 0);
            Script_FACE(arg0, 0, 4, 60);
            break;
          case 160:
            Script_FACE(arg0, 0, 0, 60);
            break;
          case 180:
            Script_FACE_LCG(arg0, 2, 40, 0);
            break;
        }
        break;
      case 21:
        switch (local_FC) {
          case 15:
            Script_FACE(arg0, 0, 3, 5);
            break;
          case 140:
            Script_FACE(arg0, 0, 0, 5);
            break;
        }
        break;
      case 22:
        switch (local_FC) {
          case 20:
            Script_FACE_LCG(arg0, 1, 20, 0);
            break;
          case 150:
            Script_FACE_LCG(arg0, 2, 20, 0);
            break;
        }
        break;
      case 23:
        switch (local_FC) {
          case 15:
            Script_FACE(arg0, 0, 5, 5);
            break;
          case 30:
            Script_FACE(arg0, 0, 0, 5);
            break;
        }
        break;
      case 24:
        switch (local_FC) {
          case 40:
            Script_FACE(arg0, 0, 5, 10);
            break;
          case 110:
            Script_FACE(arg0, 0, 0, 10);
            break;
        }
        break;
      case 25:
        switch (local_FC) {
          case 10:
            Script_FACE_LCG(arg0, 1, 10, 0);
            break;
          case 160:
            Script_FACE_LCG(arg0, 2, 10, 0);
            break;
        }
        break;
      case 26:
        switch (local_FC) {
          case 15:
            Script_FACE(arg0, 0, 3, 5);
            break;
          case 145:
            Script_FACE(arg0, 0, 0, 5);
            break;
        }
        break;
      case 27:
        if (local_FC == 1) {
          Script_FACE_LCG(arg0, 1, 1, 0);
        }
        break;
      case 28:
        if (local_FC == 1) {
          Script_FACE_LCG(arg0, 1, 1, 0);
        }
        break;
      case 29:
        switch (local_FC) {
          case 30:
            Script_FACE(arg0, 0, 5, 5);
            break;
          case 45:
            Script_FACE(arg0, 0, 0, 5);
            break;
        }
        break;
      case 30:
        switch (local_FC) {
          case 1:
            Script_FACE(arg0, 0, 1, 10);
            break;
          case 30:
            Script_FACE(arg0, 0, 0, 10);
            break;
        }
        break;
      case 31:
        switch (local_FC) {
          case 1:
            Script_FACE(arg0, 0, 2, 10);
            break;
          case 30:
            Script_FACE(arg0, 0, 0, 10);
            break;
        }
        break;
      case 32:
        switch (local_FC) {
          case 1:
            Script_FACE(arg0, 0, 3, 5);
            break;
          case 165:
            Script_FACE(arg0, 0, 0, 5);
            break;
        }
        break;
      case 33:
        switch (local_FC) {
          case 120:
            Script_FACE(arg0, 0, 1, 5);
            break;
          case 125:
            Script_FACE(arg0, 0, 0, 5);
            break;
          case 130:
            Script_FACE(arg0, 0, 1, 5);
            break;
          case 135:
            Script_FACE(arg0, 0, 0, 5);
            break;
          case 140:
            Script_FACE(arg0, 0, 1, 5);
            break;
          case 145:
            Script_FACE(arg0, 0, 0, 5);
            break;
          case 150:
            Script_FACE(arg0, 0, 1, 5);
            break;
          case 155:
            Script_FACE(arg0, 0, 0, 5);
            break;
          case 160:
            Script_FACE(arg0, 0, 1, 5);
            break;
          case 165:
            Script_FACE(arg0, 0, 0, 5);
            break;
          case 170:
            Script_FACE(arg0, 0, 1, 5);
            break;
          case 175:
            Script_FACE(arg0, 0, 0, 5);
            break;
          case 180:
            Script_FACE(arg0, 0, 1, 5);
            break;
          case 185:
            Script_FACE(arg0, 0, 0, 5);
            break;
          case 190:
            Script_FACE(arg0, 0, 1, 5);
            break;
          case 195:
            Script_FACE(arg0, 0, 0, 5);
            break;
          case 200:
            Script_FACE(arg0, 0, 1, 5);
            break;
          case 205:
            Script_FACE(arg0, 0, 0, 5);
            break;
          case 210:
            Script_FACE(arg0, 0, 1, 5);
            break;
          case 215:
            Script_FACE(arg0, 0, 0, 5);
            break;
          case 220:
            Script_FACE(arg0, 0, 1, 5);
            break;
          case 225:
            Script_FACE(arg0, 0, 0, 5);
            break;
          case 230:
            Script_FACE(arg0, 0, 1, 5);
            break;
          case 235:
            Script_FACE(arg0, 0, 0, 5);
            break;
          case 240:
            Script_FACE(arg0, 0, 1, 5);
            break;
          case 245:
            Script_FACE(arg0, 0, 0, 5);
            break;
          case 250:
            Script_FACE(arg0, 0, 1, 5);
            break;
          case 255:
            Script_FACE(arg0, 0, 0, 5);
            break;
          case 260:
            Script_FACE(arg0, 0, 1, 5);
            break;
          case 265:
            Script_FACE(arg0, 0, 0, 5);
            break;
        }
        break;
    }
    if (Script_FloatUtils(ScriptMathOp_FloatToInt, local_F8) == local_FC) {
      local_F8 = local_F8 + local_F4;
      SCNF_WaitFrames_0000(1);
    }
    local_FC = local_FC + 1;
  }
  L01B68F:;
  Script_FACE(arg0, 0, 0, 0);
  Script_FACE_LCG(arg0, 2, 0, 0);
  task_278 = -1;
  return Script_srchControl(0);
}

static void script_1B6B7(void) {
  Vector3 local_F4;
  float local_F0;
  for (;;) {
    Script_EVT_GetPosition(CHARA_CATM, -1, &local_F4, 0);
    local_F0 = Script_GetHighestIntersectionAtLocalPoint(&local_F4);
    Script_PrintDebugString(10, 27, "%f", local_F0);
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static int32_t script_1B6EF(void) {
  int32_t local_FC;
  int32_t local_F8;
  script_1B71F();
  for (;;) {
    local_FC = Script_srchUtils(-1);
    local_F8 = Script_srchUtils(SRCH_SCRIPT_GetCurrentTargetEntryIndex);
    SCNF_WaitFrames_0000(1);
  }
  script_1B71F();
  end:;
}

static void script_1B71F(void) {
  Script_srchUtils(SRCH_SCRIPT_InitEntryTableFromTaskOffset, &data_22864);
  return;
}

__attribute__((scn_string_bindings(0, data_22AA6, 1, data_22AB1)))
static void script_1B72E(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_LoadSND(0, 2, "bgm109.snd", 0);
  Script_executeAicaCommand(16296, 0, 0);
  Script_executeAicaCommand(1184, 2, 96);
  local_F8 = arg0;
  switch (local_F8) {
    case 3:
      Script_scenCreateTask(script_1B949, 0);
      Script_shenSeqUtils(arg0);
      while (Script_shenSeqUtils(-1)) {
        SCNF_WaitFrames_0000(1);
      }
      break;
    case 4:
      Script_scenCreateTask(script_1B99B, 0);
      Script_shenSeqUtils(arg0);
      while (Script_shenSeqUtils(-1)) {
        SCNF_WaitFrames_0000(1);
      }
      break;
    case 5:
      local_FC = Script_scenCreateTask(script_1B949, 0);
      Script_shenSeqUtils(3);
      while (Script_shenSeqUtils(-1)) {
        SCNF_WaitFrames_0000(1);
      }
      Script_srchControl((void *)local_FC);
      local_FC = Script_scenCreateTask(script_1B99B, 0);
      Script_shenSeqUtils(4);
      while (Script_shenSeqUtils(-1)) {
        SCNF_WaitFrames_0000(1);
      }
      Script_srchControl((void *)local_FC);
      break;
  }
  Script_LoadSND(0, 2, "SELECT", 0);
  Script_executeAicaCommand(16296, 0, 0);
  Script_executeAicaCommand(1184, 2, 100);
  script_1B917();
  return;
}

static void script_1B85C(void) {
  Script_SetFogFlags(0, data_22AC8);
  Script_SetFogColour(&data_22AB8);
  Script_SetupLightFromTable(1);
  return;
}

static void script_1B87A(void) {
  script_1B85C();
  Script_EVT_SetPosition(SHEN_NAME('GT01'), POS_XYZ, &data_22AD8);
  Script_EVT_SetRotation(SHEN_NAME('GT01'), 0x38000000, &data_22AF0, 0);
  Script_EVT_SetPosition(SHEN_NAME('GT02'), POS_XYZ, &data_22AE4);
  Script_EVT_SetRotation(SHEN_NAME('GT02'), 0x38000000, &data_22AFC, 0);
  script_1B8EA(SHEN_NAME('GT01'));
  script_1B8EA(SHEN_NAME('GT02'));
  return;
}

static void script_1B8EA(int32_t arg0) {
  Script_charaDisplay(arg0, CDM_Show);
  Script_ccowSetFlag80(arg0, 0);
  Script_ccowUtils(arg0, 2);
  Script_scenToggleActiveFor(arg0, 0);
  Script_SetCPAMFlags(arg0, 0);
  return;
}

static void script_1B917(void) {
  script_1B93E(SHEN_NAME('GT01'));
  script_1B93E(SHEN_NAME('GT02'));
  Script_SetGameStateTo(AUTH_None);
  Script_ryoControl(RYO_RestoreControl);
  return;
}

static void script_1B93E(int32_t arg0) {
  Script_charaDisplay(arg0, CDM_Hide);
  return;
}

static int32_t script_1B949(void) {
  int16_t local_FE;
  local_FE = 0;
  while (1) {
    if (Script_shenSeqUtils(-1)) {
      if (local_FE == 0) {
        Script_scenCreateTask(script_1BA0F, 3, SHEN_NAME('GT01'), 6, 40);
      }
      local_FE = local_FE + 1;
    } else {
      Script_srchControl(0);
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_1B99B(void) {
  int16_t local_FE;
  local_FE = 0;
  while (1) {
    if (Script_shenSeqUtils(-1)) {
      switch (local_FE) {
        case 0:
          Script_lightSetSlotEnabled(3, 0);
          Script_charaDisplay(SHEN_NAME('GT02'), CDM_Show);
          break;
        case 50:
          Script_scenCreateTask(script_1BA0F, 3, SHEN_NAME('GT02'), 3, 35);
          break;
      }
      local_FE = local_FE + 1;
    } else {
      Script_srchControl(0);
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_1BA0F(int32_t arg0, int32_t arg1, int32_t arg2) {
  float local_FC;
  int16_t local_FA;
  local_FA = 0;
  while (1) {
    if (Script_shenSeqUtils(-1)) {
      if (local_FA <= arg2) {
        local_FC = Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 1.2f);
        if (!(local_FC >= 1.0f)) {
          Script_charaDisplay(arg0, CDM_Show);
          Script_lightSetSlotEnabled(arg1, 0);
        } else {
          Script_charaDisplay(arg0, CDM_Hide);
          Script_lightSetSlotEnabled(arg1, 1);
        }
      } else {
        Script_charaDisplay(arg0, CDM_Hide);
        Script_lightSetSlotEnabled(arg1, 1);
        Script_srchControl(0);
      }
    } else {
      Script_srchControl(0);
    }
    local_FA = local_FA + 1;
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_1BAA5(void) {
  script_D9BF();
  Script_scenCreateTask(script_1D473, 0);
  script_3ADC();
  DSWK_AssignDoorEntryConfig(2, 6, 0, -1, 0x2002AB, -1, -1, -1);
  DSWK_AssignDoorEntryConfig(5, 6, 0, -1, 0x2002AB, -1, -1, -1);
  DSWK_AssignDoorEntryConfig(0, 6, 0, -1, 0x2002AB, -1, -1, -1);
  DSWK_AssignDoorEntryConfig(6, 6, 0, -1, 0x2002AB, -1, -1, -1);
  DSWK_AssignDoorEntryConfig(9, 6, 0, -1, 0x2002AB, -1, -1, -1);
  DSWK_AssignDoorEntryConfig(3, 7, 1, 4, 0x2202AB, 0x2702AB, 0x2602AB, 0x414B4952);
  DSWK_AssignDoorEntryConfig(8, 7, 1, 5, 0x2202AB, 0x2702AB, 0x2602AB, 0x4D4D5559);
  DSWK_AssignDoorEntryConfig(4, 7, 1, 5, 0x2202AB, 0x2702AB, 0x2602AB, 0x554B5953);
  DSWK_AssignDoorEntryConfig(7, 6, 0, -1, 0x2202AB, -1, -1, -1);
  script_BEC5(1);
  {
    int32_t local_FC;
    local_FC = Script_ScenDataUtil(task_6C, 4, 27);
    local_FC = local_FC & -65;
    local_FC = local_FC & -129;
    local_FC = local_FC | 0x40;
    Script_ScenDataUtil(task_6C, 14, 27, local_FC);
  }
  {
    int32_t local_FC;
    local_FC = Script_ScenDataUtil(task_6C, 4, 66);
    local_FC = local_FC & -65;
    local_FC = local_FC & -129;
    local_FC = local_FC | 0x80;
    Script_ScenDataUtil(task_6C, 14, 66, local_FC);
  }
  task_293 = script_18F05();
  Script_ryoSleepUtils(2, 1);
  script_18F14();
  script_1CBB2();
  Script_PlayerStatsUtils(PS_IncrementSpotVisitCount, 15);
  Script_facemotUtils(CHARA_AKIR, 1, 0);
  return;
}

__attribute__((scn_string_bindings(0, data_22B08, 2, data_22B1D, 4, data_22B32, 6, data_22B47)))
static void script_1BC5B(void) {
  script_DA2F();
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
  if (Script_WeatherUtils_0(WeatherDataUtil_QueryCurrentFCASFlag, 3)) {
    while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, "/scene/03/JU00/", "MAPZ") == 0) {
      SCNF_WaitFrames_0000(1);
    }
  } else {
    while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, "/scene/03/JU00/", "MAPN") == 0) {
      SCNF_WaitFrames_0000(1);
    }
  }
  if (Script_WeatherUtils_0(WeatherDataUtil_QueryCurrentFCASFlag, 5)) {
    while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, "/scene/03/JU00/", "MAPS") == 0) {
      SCNF_WaitFrames_0000(1);
    }
  }
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, "/scene/03/JU00/", "SHUKI") == 0) {
    SCNF_WaitFrames_0000(1);
  }
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 1);
  return;
}

static void script_1BD3F(void) {
  Script_scenCreateTask(script_0030, 6, CHARA_AKIR, 2, 0, &data_22B60, 2, 0);
  script_2643((int32_t)data_22CE4);
  Script_scenCreateTask(script_1CBF1, 0);
  script_1D3FD();
  Script_scenCreateTask(script_1C031, 0);
  SCNF_WaitFrames_0000(1);
  task_244 = script_1C40B();
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 2);
  return;
}

static void script_1BDA5(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  local_FC = arg3;
  if (local_FC == 0) {
    SCNF_WaitFrames_0000(90);
    goto L01BDC1;
  }
  L01BDC1:;
  Script_executeAicaCommand(2720, 1, 30);
  Script_executeAicaCommand(2720, 2, 30);
  Script_executeAicaCommand(2720, 3, 30);
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_ryoControl(PCCTL_RequestNewMode, 0);
  Script_shenRequestGameModuleTransition(arg0, arg1, arg2);
  SCNF_WaitFrames_0000(1);
  return;
}

static int8_t script_1BE2E(void) {
  int8_t local_FF;
  int32_t local_F0[3];
  int32_t local_E4[3];
  int32_t local_D8[3];
  int32_t local_CC[3];
  local_FF = 0;
  local_F0[0] = -0x3DE83AFB;
  local_F0[1] = 0;
  local_F0[2] = 0x425512F2;
  local_E4[0] = -0x3DD1F6C9;
  local_E4[1] = 0;
  local_E4[2] = 0x42152ACE;
  local_D8[0] = -0x3DF6BE0E;
  local_D8[1] = 0;
  local_D8[2] = 0x42053007;
  local_CC[0] = -0x3E246E98;
  local_CC[1] = 0;
  local_CC[2] = 0x42445D7E;
  if (script_17266(CHARA_AKIR, (int32_t)local_F0, (int32_t)local_E4, (int32_t)local_D8, (int32_t)local_CC) == 1) {
    task_28D = 0;
    script_18BCF(CHARA_CATM);
    local_FF = 1;
  } else {
    task_28D = 1;
  }
  return local_FF;
}

static void script_1BEF1(int32_t arg0) {
  int32_t local_FC;
  local_FC = arg0;
  if (local_FC == 1) {
    script_1078E();
    goto L01BF0A;
  }
  L01BF0A:;
  return;
}

static int32_t script_1BF0B(int32_t arg0, int32_t arg1) {
  if (arg0) {
    (void)arg0;
  } else {
    (void)arg1;
  }
  end:;
}

static int32_t script_1BF1D(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  local_FC = script_1BF0B(arg0, arg1);
  if (local_FC == 1) {
    arg0 = script_1BE2E();
    goto L01BF45;
  }
  L01BF45:;
  return arg0;
}

static int32_t script_1BF49(void) {
  int8_t local_FF;
  int8_t local_FE;
  int32_t local_F8;
  local_FE = 1;
  local_FF = 0;
  local_FF = script_1BF1D(local_FF, local_FE);
  for (;;) {
    script_D9FB();
    local_FF = script_1BF1D(local_FF, local_FE);
    if (local_FE == 1) {
      local_FE = 1;
    } else {
      local_FE = local_FE + 1;
    }
    script_1BEF1(local_FF);
    if ((&task_7C)->mode) {
      local_F8 = task_7C.entry_idx;
      if (local_F8 == 1) {
        SCNF_WaitFrames_0000(90);
        script_1BDA5(3, SHEN_NAME('JHD0'), 1, 1);
        goto L01BFD9;
      }
    }
    L01BFD9:;
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_1BFE5(const char *arg0, const char *arg1) {
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, arg0, arg1) == 0) {
    SCNF_WaitFrames_0000(1);
  }
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 1);
  return;
}

static int32_t script_1C031(void) {
  int32_t unused_FC;
  GameDateTime local_F4;
  int32_t local_F0;
  int8_t local_EF;
  int8_t local_EE;
  uint8_t unused_EC[2];
  int32_t local_E8;
  local_EF = -1;
  local_F0 = 0;
  local_EE = 0;
  SCNF_WaitFrames_0000(1);
  for (;;) {
    Script_ReturnTimeDate(&local_F4);
    local_F0 = (&local_F4)->hours * 100 + (&local_F4)->minutes;
    local_E8 = local_EE;
    switch (local_E8) {
      case 0:
        if (((&local_F4)->hours >= 19) | (local_F0 <= 600)) {
          local_EE = 4;
          Script_SetupLightFromTable(0);
          Script_mapmSetRenderFlag(2, 0);
          Script_mapmSetRenderFlag(4, 0);
          Script_mapmSetRenderFlag(6, 0);
          Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 45.0f);
          Script_PREP_Related(-1, 1);
          Script_SetCharaFlags(CHARA_AKIR, 0);
        } else {
          local_EE = 1;
          if (task_244 < 2) {
            Script_mapmSetRenderFlag(3, 0);
          } else {
            Script_mapmSetRenderFlag(2, 0);
          }
          Script_mapmSetRenderFlag(5, 0);
          Script_mapmSetRenderFlag(7, 0);
          Script_shenSetLightingMode(1);
          Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 70.0f);
          Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 10.0f);
        }
        break;
      case 1:
        if (local_F0 >= 1726) {
          Script_mapmSetRenderFlag(7, 1);
          Script_mapmSetRenderFlag(6, 0);
          local_EE = 2;
        }
        break;
      case 2:
        if (local_F0 >= 1809) {
          Script_mapmSetRenderFlag(5, 1);
          Script_mapmSetRenderFlag(4, 0);
          local_EE = 3;
        }
        break;
      case 3:
        if (local_F0 >= 1900) {
          local_EE = 4;
          script_1C47F();
          Script_PREP_Related(-1, 1);
          Script_SetCharaFlags(CHARA_AKIR, 0);
          Script_ryoSleepUtils(3, 1, 0, 98);
        }
        break;
      case 4:
        if ((local_F0 >= 2337) | (local_F0 <= 600)) {
          Script_mapmSetRenderFlag(4, 1);
          Script_mapmSetRenderFlag(5, 0);
          local_EE = 5;
        }
        break;
      case 5:
        if ((local_F0 <= 600) & (local_F0 >= 28)) {
          Script_mapmSetRenderFlag(6, 1);
          Script_mapmSetRenderFlag(7, 0);
          local_EE = 6;
        }
        break;
      case 6:
        if ((local_F0 >= 600) & ((&local_F4)->minutes >= 0)) {
          local_EE = 1;
          if ((Script_IsPlayerAllowed(0) != 0) & (Script_ryoControl(PCCTL_QueryShouldRun) != 0)) {
            Script_srchUtils(SRCH_SCRIPT_Reset);
            Script_ryoControl(PCCTL_DisablePlayerControl);
            Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
            while (Script_filmCheckOrSetTransitionUtil(-1)) {
              SCNF_WaitFrames_0000(1);
            }
            Script_PREP_Related(-1, 0);
            Script_mapmSetRenderFlag(2, 1);
            Script_mapmSetRenderFlag(3, 0);
            Script_shenSetLightingMode(1);
            Script_SetCharaFlags(CHARA_AKIR, 4);
            Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 70.0f);
            Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 10.0f);
            Script_ryoControl(RYO_RestoreControl);
            Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
            while (Script_filmCheckOrSetTransitionUtil(-1)) {
              SCNF_WaitFrames_0000(1);
            }
          }
        }
        break;
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_1C32B(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t unused_FC;
  GameDateTime local_F4;
  int32_t local_F0;
  int32_t local_EC;
  int32_t local_E8;
  int8_t local_E7;
  uint8_t unused_E4[3];
  Script_ReturnTimeDate(&local_F4);
  if (arg0 == arg2) {
    if (arg1 < arg3) {
      local_E7 = 1;
    } else {
      local_E7 = 0;
    }
  } else if (arg0 < arg2) {
    local_E7 = 1;
  } else {
    local_E7 = 0;
  }
  local_F0 = arg0 * 100 + arg1;
  local_EC = arg2 * 100 + arg3;
  local_E8 = (&local_F4)->hours * 100 + (&local_F4)->minutes;
  if (local_E7) {
    if ((local_E8 >= local_F0) & (local_E8 < local_EC)) {
      local_E7 = 1;
    } else {
      local_E7 = 0;
    }
  } else if ((local_E8 >= local_F0) | (local_E8 < local_EC)) {
    local_E7 = 1;
  } else {
    local_E7 = 0;
  }
  (void)local_E7;
  return;
}

static void script_1C3E4(void) {
  if ((int32_t)script_2657 == 10922) {
    script_264D(0x4000);
  } else {
    script_264D(10922);
  }
  return;
}

static int32_t script_1C40B(void) {
  int32_t unused_FC;
  GameDateTime local_F4;
  int8_t local_F3;
  uint8_t unused_F0[3];
  Script_ReturnTimeDate(&local_F4);
  SCNF_WaitFrames_0000(1);
  local_F3 = Script_WeatherUtils_0(WeatherDataUtil_GetWeatherClassForDateTime, (&local_F4)->year, (&local_F4)->month, (&local_F4)->day, (&local_F4)->hours, (&local_F4)->minutes);
}

static int32_t script_1C443(void) {
  SCNF_WaitFrames_0000(1);
  Script_filmCheckOrSetTransitionUtil(1, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  return Script_srchControl(0);
}

__attribute__((scn_string_bindings(0, data_22D60, 1, data_22D70, 2, data_22D75, 3, data_22D7A, 4, data_22D7B, 5, data_22D80, 6, data_22D81, 7, data_22D8C, 8, data_22D93)))
static void script_1C47F(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  script_1BFE5("/scene/03/JU00/", "JYUU");
  for (;;) {
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_EV_STOP_ACTIVE, 1);
    if ((Script_IsPlayerAllowed(0) != 0) & (Script_ryoControl(PCCTL_QueryShouldRun) != 0)) {
      Script_scenUtils(SCENARIO_CMD_INIT_SCNT);
      Script_srchUtils(SRCH_SCRIPT_Reset);
      Script_ryoControl(PCCTL_DisablePlayerControl);
      Script_executeAicaCommand(2720, 1, 30);
      Script_executeAicaCommand(2720, 2, 30);
      Script_executeAicaCommand(2720, 3, 30);
      Script_filmCheckOrSetTransitionUtil(60, 0, 0, 0, 0, 255, 0, 0, 0);
      while (Script_filmCheckOrSetTransitionUtil(-1)) {
        SCNF_WaitFrames_0000(1);
      }
      SCNF_RequestIconNotification(0, 0, 0, 2);
      if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, *task_20C) == 1) {
        task_2BE = 0;
        SCNF_WaitFrames_0000(1);
        script_1D3B2(0);
      } else if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, task_20C[2]) == 1) {
        script_1D3B2(2);
        script_1D3B2(3);
      } else {
        if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, task_20C[1]) == 1) {
          script_1D3B2(1);
          goto L01C59F;
        }
      }
      L01C59F:;
      Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
      Script_CycleManagerUtils(CYCLE_CMD_PROCESS_SND_EXTRA);
      script_17667(CHARA_CATM);
      Script_mapmSetRenderFlag(3, 1);
      Script_mapmSetRenderFlag(2, 0);
      Script_mapmSetRenderFlag(3, 1);
      Script_mapmSetRenderFlag(5, 1);
      Script_mapmSetRenderFlag(7, 1);
      Script_mapmSetRenderFlag(2, 0);
      Script_mapmSetRenderFlag(4, 0);
      Script_mapmSetRenderFlag(6, 0);
      Script_mtwkGetSetRotation(CHARA_AKIR, &local_E8, 0);
      Script_EVT_GetPosition(CHARA_AKIR, -1, &local_F4, 0);
      Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
      Script_AuthorUtils(0, 0, "JYUU", "");
      Script_AuthorUtils(0, 1, "JYUU", "");
      script_1B87A();
      Script_LoadSND(0, 2, "bgm109.snd", 0);
      Script_executeAicaCommand(16296, 0, 0);
      Script_executeAicaCommand(1184, 2, 96);
      Script_scenCreateTask(script_1C443, 0);
      {
        int32_t local_E4;
        local_E4 = Script_scenCreateTask(script_1B949, 0);
        Script_shenSeqUtils(0);
        while (Script_shenSeqUtils(-1)) {
          SCNF_WaitFrames_0000(1);
        }
        Script_srchControl((void *)local_E4);
      }
      if (Script_shenSeqUtils(-7) == 0) {
        int32_t local_E4;
        local_E4 = Script_scenCreateTask(script_1B99B, 0);
        Script_shenSeqUtils(1);
        while (Script_shenSeqUtils(-1)) {
          SCNF_WaitFrames_0000(1);
        }
        Script_srchControl((void *)local_E4);
      }
      Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
      Script_executeAicaCommand(2720, 1, 30);
      Script_executeAicaCommand(2720, 2, 30);
      Script_executeAicaCommand(2720, 3, 30);
      Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
      while (Script_filmCheckOrSetTransitionUtil(-1)) {
        SCNF_WaitFrames_0000(1);
      }
      Script_charaDisplay(SHEN_NAME('GT01'), CDM_Hide);
      Script_charaDisplay(SHEN_NAME('GT02'), CDM_Hide);
      Script_lightResetSlotToDefaults(1);
      Script_executeAicaCommand(2720, 1, 30);
      Script_executeAicaCommand(2720, 2, 30);
      Script_executeAicaCommand(2720, 3, 30);
      Script_SetupLightFromTable(0);
      Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 45.0f);
      Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &local_F4);
      Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &local_E8, 0);
      Script_SetGameStateTo(AUTH_None);
      Script_convSetCamInactiveFlag(-1);
      Script_LoadSND(0, 2, "SELECT", 0);
      Script_executeAicaCommand(16296, 0, 0);
      Script_executeAicaCommand(1184, 2, 100);
      Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
      while (Script_filmCheckOrSetTransitionUtil(-1)) {
        SCNF_WaitFrames_0000(1);
      }
      Script_ryoControl(RYO_RestoreControl);
      Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 1);
      Script_scenUtils(SCENARIO_CMD_UPDATE_TIMER);
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_EV_STOP_ACTIVE, 0);
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "JYUU");
  return;
}

static int32_t script_1C857(void) {
  float local_FC;
  int32_t unused_F8;
  GameDateTime local_F0;
  Script_ReturnTimeDate(&local_F0);
  SCNF_WaitFrames_0000(1);
  Script_WeatherUtils_0(WeatherDataUtil_StoreUnknownWeatherFloatForDateTime, (&local_F0)->year, (&local_F0)->month, (&local_F0)->day, (&local_F0)->hours, (&local_F0)->minutes, &local_FC);
  Script_charaDisplay(SHEN_NAME('NBR1'), CDM_Show);
  Script_ccowSetFlag80(SHEN_NAME('NBR1'), 1);
  Script_scenToggleActiveFor(SHEN_NAME('NBR1'), 0);
  Script_charaDisplay(SHEN_NAME('NBR2'), CDM_Show);
  Script_ccowSetFlag80(SHEN_NAME('NBR2'), 1);
  Script_scenToggleActiveFor(SHEN_NAME('NBR2'), 0);
  Script_charaDisplay(SHEN_NAME('NBR3'), CDM_Show);
  Script_ccowSetFlag80(SHEN_NAME('NBR3'), 1);
  Script_scenToggleActiveFor(SHEN_NAME('NBR3'), 0);
  Script_charaDisplay(SHEN_NAME('NBR4'), CDM_Show);
  Script_ccowSetFlag80(SHEN_NAME('NBR4'), 1);
  Script_scenToggleActiveFor(SHEN_NAME('NBR4'), 0);
  SCNF_WaitFrames_0000(1);
  Script_tmnmUtils(SHEN_NAME('NBR1'), 1486, 0.0f, 32767.0f, 0.0f, 1.0f, 0);
  Script_tmnmUtils(SHEN_NAME('NBR2'), 1486, 0.0f, 32767.0f, 0.0f, 1.0f, 0);
  Script_tmnmUtils(SHEN_NAME('NBR3'), 1486, 0.0f, 32767.0f, 0.0f, 1.0f, 0);
  Script_tmnmUtils(SHEN_NAME('NBR4'), 1486, 0.0f, 32767.0f, 0.0f, 1.0f, 0);
  Script_tmnmUtils(SHEN_NAME('NBR1'), 0, -1.0f, -1.0f, -1.0f, 32767.0f, 4);
  Script_tmnmUtils(SHEN_NAME('NBR1'), 0, -1.0f, -1.0f, -1.0f, 1.0f, -1);
  Script_tmnmUtils(SHEN_NAME('NBR3'), 0, -1.0f, -1.0f, -1.0f, 32767.0f, 4);
  Script_tmnmUtils(SHEN_NAME('NBR3'), 0, -1.0f, -1.0f, -1.0f, 0.5f, -1);
  Script_tmnmUtils(SHEN_NAME('NBR2'), 0, -1.0f, -1.0f, -1.0f, 32767.0f, 4);
  Script_tmnmUtils(SHEN_NAME('NBR2'), 0, -1.0f, -1.0f, -1.0f, 0.75f, -1);
  Script_tmnmUtils(SHEN_NAME('NBR4'), 0, -1.0f, -1.0f, -1.0f, 32767.0f, 4);
  Script_tmnmUtils(SHEN_NAME('NBR4'), 0, -1.0f, -1.0f, -1.0f, 0.25f, -1);
  SCNF_WaitFrames_0000(100);
  Script_tmnmUtils(SHEN_NAME('NBR1'), 0, -1.0f, -1.0f, -1.0f, 32767.0f, 4);
  Script_tmnmUtils(SHEN_NAME('NBR1'), 0, -1.0f, -1.0f, -1.0f, local_FC, -1);
  Script_tmnmUtils(SHEN_NAME('NBR3'), 0, -1.0f, -1.0f, -1.0f, 32767.0f, 4);
  Script_tmnmUtils(SHEN_NAME('NBR3'), 0, -1.0f, -1.0f, -1.0f, local_FC, -1);
  Script_tmnmUtils(SHEN_NAME('NBR2'), 0, -1.0f, -1.0f, -1.0f, 32767.0f, 4);
  Script_tmnmUtils(SHEN_NAME('NBR2'), 0, -1.0f, -1.0f, -1.0f, local_FC, -1);
  Script_tmnmUtils(SHEN_NAME('NBR4'), 0, -1.0f, -1.0f, -1.0f, 32767.0f, 4);
  Script_tmnmUtils(SHEN_NAME('NBR4'), 0, -1.0f, -1.0f, -1.0f, local_FC, -1);
  return Script_srchControl(0);
}

static void script_1CBB2(void) {
  int32_t local_FC;
  local_FC = 0;
  Script_WeatherUtils(WeatherScript_LoadWccpClipSet0FromTaskOffset, &data_22D98, 10);
  while (local_FC < 10) {
    Script_WeatherUtils(WeatherScript_SetWccpClipSet0Entry, local_FC, 1);
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static int32_t script_1CBF1(void) {
  Vector3 local_F4;
  float local_F0;
  int8_t local_EF;
  int32_t local_E8;
  local_F0 = Script_SetCameraSettings(CAM_UTILS_SET_DISTANCE_FROM_PLAYER, -1.0f);
  while (1) {
    (void)((local_EF = 0) & 1);
    Script_EVT_GetPosition(CHARA_AKIR, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
    (&local_F4)->y = (&local_F4)->x * 0.1658f + 55.76f;
    if (!((&local_F4)->z >= (&local_F4)->y)) {
      (&local_F4)->y = (&local_F4)->x * -0.5538f + 25.2817f;
      if (!((&local_F4)->z <= (&local_F4)->y)) {
        (void)((local_EF = -1) & 0x10);
      }
    }
    if (local_EF & 0x80) {
      script_264D(5461);
    } else {
      script_264D(0x4000);
    }
    local_E8 = *&task_18;
    switch (local_E8) {
      case 11:
        while (*&task_18 == 11) {
          while (task_4A != 1) {
            SCNF_WaitFrames_0000(1);
          }
          Script_SetCameraSettings(CAM_UTILS_SET_DISTANCE_FROM_PLAYER, 2.5f);
          task_38 = 2.5f;
          task_44 = *(int8_t *)&*(int32_t *)&task_44 | 1;
          for (;;) {
            if (!*(int8_t *)&*(int32_t *)&task_44) {
              break;
            }
            SCNF_WaitFrames_0000(1);
          }
        }
        break;
      case 12:
        while (task_4A != 1) {
          SCNF_WaitFrames_0000(1);
        }
        task_38 = local_F0;
        task_44 = *(int8_t *)&*(int32_t *)&task_44 | 1;
        for (;;) {
          if (!*(int8_t *)&*(int32_t *)&task_44) {
            break;
          }
          SCNF_WaitFrames_0000(1);
        }
        break;
      case 15:
        while (task_4A != 1) {
          SCNF_WaitFrames_0000(1);
        }
        Script_SetCameraSettings(CAM_UTILS_SET_DISTANCE_FROM_PLAYER, 1.5f);
        task_38 = 1.5f;
        task_44 = *(int8_t *)&*(int32_t *)&task_44 | 1;
        for (;;) {
          if (!*(int8_t *)&*(int32_t *)&task_44) {
            break;
          }
          SCNF_WaitFrames_0000(1);
        }
        break;
      case 16:
        while (task_4A != 1) {
          SCNF_WaitFrames_0000(1);
        }
        task_38 = local_F0;
        task_44 = *(int8_t *)&*(int32_t *)&task_44 | 1;
        for (;;) {
          if (!*(int8_t *)&*(int32_t *)&task_44) {
            break;
          }
          SCNF_WaitFrames_0000(1);
        }
        break;
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_1CDF6(void) {
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, 0x4D544143) == 1) {
    if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, *task_20C) == 1) {
      Vector3 local_F4;
      Script_EVT_GetPosition(CHARA_CATM, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
      Script_EVT_SetFacingTowardPoint(*task_20C, 0x38000000, &local_F4, 0);
      Script_SetCharaLookAtPosition(*task_20C, 98320, &local_F4, 1);
    } else if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, task_20C[1]) == 1) {
      Vector3 local_F4;
      Script_EVT_GetPosition(CHARA_CATM, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
      Script_EVT_SetFacingTowardPoint(task_20C[1], 0x38000000, &local_F4, 0);
      Script_SetCharaLookAtPosition(task_20C[1], 98320, &local_F4, 1);
    } else {
      if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, task_20C[2]) == 1) {
        Vector3 local_F4;
        Script_EVT_GetPosition(CHARA_CATM, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
        Script_EVT_SetFacingTowardPoint(task_20C[2], 0x38000000, &local_F4, 0);
        Script_SetCharaLookAtPosition(task_20C[2], 98320, &local_F4, 1);
        Script_EVT_SetFacingTowardPoint(task_20C[3], 0x38000000, &local_F4, 0);
        Script_SetCharaLookAtPosition(task_20C[3], 98320, &local_F4, 1);
        goto L01CF24;
      }
    }
  }
  L01CF24:;
  return;
}

static void script_1CF25(void) {
  int32_t unused_FC;
  GameDateTime local_F4;
  int32_t local_F0;
  int32_t local_EC;
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
  task_240 = 0;
  if (task_23C != 0) {
    task_240 = Script_ryoSleepUtils(3, 0, 0);
    local_F0 = Script_ryoSleepUtils(3, 0, 1);
    local_F0 = local_F0 * 100;
    local_F0 = local_F0 + Script_ryoSleepUtils(3, 0, 2);
  } else {
    script_1D2A6();
    Script_ReturnTimeDate(&local_F4);
    local_F0 = (&local_F4)->hours * 100 + (&local_F4)->minutes;
    Script_ryoSleepUtils(3, 1, 1, (&local_F4)->hours);
    Script_ryoSleepUtils(3, 1, 2, (&local_F4)->minutes);
    Script_ryoSleepUtils(3, 1, 0, task_240);
  }
  local_EC = task_240;
  if (local_EC == 0) {
    if ((local_F0 > 800) & (local_F0 < 1859)) {
      script_1D1DA(0);
    }
  } else if ((local_EC == 1) | (local_EC == 5)) {
    if ((local_F0 > 800) & (local_F0 < 1800)) {
      script_1D1DA(0);
    }
  } else {
    switch (local_EC) {
      case 2:
        if ((local_F0 > 830) & (local_F0 < 1529)) {
          script_1D1DA(0);
        }
        if ((local_F0 > 1530) & (local_F0 < 1800)) {
          script_1D10E(1);
        }
        break;
      case 3:
        if ((local_F0 > 830) & (local_F0 < 1359)) {
          script_1D1DA(0);
        } else if ((local_F0 > 1400) & (local_F0 < 1559)) {
          script_1D174(2);
          script_1D1DA(3);
        } else {
          if ((local_F0 > 1600) & (local_F0 < 1800)) {
            script_1D10E(1);
            goto L01D0D2;
          }
        }
        L01D0D2:;
        break;
      case 4:
        if ((local_F0 > 830) & (local_F0 < 1800)) {
          script_1D10E(2);
          script_1D240(3);
        }
        break;
    }
  }
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 1);
  return;
}

static void script_1D10E(int32_t arg0) {
  int32_t unused_F4[3];
  script_1D2F3(arg0);
  script_387C(task_20C[arg0], -34.000198f, 2.591f, 39.365601f);
  Script_PlayCharaMotion(task_20C[arg0], MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_CycleManagerUtils(CYCLE_CMD_SET_RUNNING_PROGRAM_FIELD_B8, task_20C[arg0], 0);
  return;
}

static void script_1D174(int32_t arg0) {
  int32_t unused_F4[3];
  script_1D2F3(arg0);
  script_387C(task_20C[arg0], -34.000301f, 2.591f, 40.105701f);
  Script_PlayCharaMotion(task_20C[arg0], MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_CycleManagerUtils(CYCLE_CMD_SET_RUNNING_PROGRAM_FIELD_B8, task_20C[arg0], 0);
  return;
}

static void script_1D1DA(int32_t arg0) {
  int32_t unused_F4[3];
  script_1D2F3(arg0);
  script_387C(task_20C[arg0], -34.000198f, 2.591f, 39.365601f);
  Script_PlayCharaMotion(task_20C[arg0], 4356, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_CycleManagerUtils(CYCLE_CMD_SET_RUNNING_PROGRAM_FIELD_B8, task_20C[arg0], 0);
  return;
}

static void script_1D240(int32_t arg0) {
  int32_t unused_F4[3];
  script_1D2F3(arg0);
  script_387C(task_20C[arg0], -35.100201f, 2.591f, 37.738602f);
  Script_PlayCharaMotion(task_20C[arg0], 4356, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_CycleManagerUtils(CYCLE_CMD_SET_RUNNING_PROGRAM_FIELD_B8, task_20C[arg0], 0);
  return;
}

static void script_1D2A6(void) {
  int8_t local_FF;
  local_FF = Script_StepUtils(GET_STEP_HIGHER, 0);
  if (local_FF <= 20) {
    if ((task_244 < 2) & (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 776) == 0)) {
      task_240 = 1;
    } else {
      task_240 = 98;
    }
  } else {
    task_240 = 98;
  }
  return;
}

static void script_1D2F3(int32_t arg0) {
  task_224[arg0] = 0;
  Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT, task_20C[arg0], 1);
  SCNF_WaitFrames_0000(1);
  task_224[arg0] = Script_CycleManagerUtils(CYCLE_CMD_GET_CYCLE_PTR_BY_CHARA_ID, task_20C[arg0]);
  SCNF_WaitFrames_0000(1);
  while (task_224[arg0] == 0) {
    SCNF_WaitFrames_0000(1);
    task_224[arg0] = Script_CycleManagerUtils(CYCLE_CMD_GET_CYCLE_PTR_BY_CHARA_ID, task_20C[arg0]);
  }
  while (Script_CycleManagerUtils(CYCLE_CMD_GET_NEXT_TASK_POINTER, task_224[arg0]) == 0) {
    SCNF_WaitFrames_0000(1);
  }
  Script_motmGetCharaFlags(task_20C[arg0], 0, 0);
  return;
}

static void script_1D3B2(int32_t arg0) {
  Script_CycleManagerUtils(CYCLE_CMD_LOAD_OR_RESET_RUNNING_PROGRAM, task_224[arg0]);
  Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT, task_20C[arg0], 0);
  while (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, task_20C[arg0]) == -1) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_1D3FD(void) {
  int8_t local_FF;
  local_FF = 0;
  task_20C[0] = 0x4D47454D;
  task_20C[1] = 0x41544F4B;
  task_20C[2] = 0x55534159;
  task_20C[3] = 0x4F59414B;
  task_20C[5] = 0x494B4B52;
  task_20C[4] = 0x41595454;
  while (local_FF < 6) {
    task_224[local_FF] = 0;
    local_FF = local_FF + 1;
  }
  return;
}

static int32_t script_1D473(void) {
  int32_t unused_F4[3];
  int32_t local_F0;
  float local_EC;
  int8_t local_EB;
  uint8_t unused_E8[3];
  int32_t local_E4;
  for (;;) {
    local_F0 = script_3B2E();
    local_E4 = local_F0;
    if (local_E4 == 130) {
      if (Script_EnqueueSCNT(2) == 0) {
        if (local_EB == 0) {
          (void)((local_EB = -1) & 2);
          local_EC = Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f);
          if (!(local_EC >= 1.0f)) {
            Script_seqEnterEvtCam(1600, 0, AUTH_None);
          } else {
            if (!(local_EC >= 2.0f)) {
              Script_seqEnterEvtCam(1601, 0, AUTH_None);
              goto L01D4F4;
            }
          }
        }
      }
      L01D4F4:;
      script_3B06();
    } else if (local_E4 == 131) {
      while (Script_ConversationUtils(2) == 0) {
        SCNF_WaitFrames_0000(1);
      }
      (void)((local_EB = 0) & 1);
      if (Script_ryoControl(PCCTL_QueryShouldRun) == 0) {
        Script_SetGameStateTo(AUTH_Scripted);
      }
      script_3B06();
    } else if (local_E4 == 132) {
      Script_executeAicaCommand(66985, 0, 0);
      script_3B06();
    } else {
      if (local_F0 != -1) {
        script_3B06();
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_1D57E(void) {
  int32_t local_FC;
  local_FC = Script_GetSceneID();
  switch (local_FC) {
    case 1:
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_ENTRY2_TIMED_EVENT_DONE) == 0) {
        Script_TimedTransitionUtils(TIMED_TRANSITION_REGISTER, 1, 0x3044484A, 2, &data_23038, 0xD5A6C3, TimedTransitionUpdateMemoItem150);
      }
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JD00_ENTRY20_TIMED_EVENT_DONE) == 0) {
        Script_TimedTransitionUtils(TIMED_TRANSITION_REGISTER, 1, 0x3030444A, 20, &data_23000, 0xDA3AA3, 0);
      }
      break;
    case 2:
      if ((Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_GMCT_ROUTE_STARTED) == 0) | (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_GMCT_ROUTE_ADVANCED) == 0)) {
        Script_TimedTransitionUtils(TIMED_TRANSITION_REGISTER, 2, 0x54434D47, 1, &data_22FC8, 0xB7420F, 0);
      }
      break;
    case 3:
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOZOMI_RESCUE_RUSH_ACTIVE) == 0) {
        Script_TimedTransitionUtils(TIMED_TRANSITION_REGISTER, 3, 0x4F4D4F4A, 20, &data_22FE4, 0x2DC9FE, TimedTransitionRollbackToPreviousDay2330);
      }
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_FIRED_FROM_JOB) & ~Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 970)) {
        if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_MFSY_ENTRY42_TIMED_EVENT_DONE) == 0) {
          Script_TimedTransitionUtils(TIMED_TRANSITION_REGISTER, 3, 0x5953464D, 42, &data_2301C, 0xBBB817, 0);
        }
      }
      break;
  }
  return;
}

static void script_1D6B6(int32_t arg0) {
  script_1D6CE(arg0);
  return;
}

static void script_1D6C2(int32_t arg0) {
  script_1D6CE(arg0);
  return;
}

static void script_1D6CE(int32_t arg0) {
  if (Script_StepUtils(GET_STEP_HIGHER, 0) == 22) {
    int32_t local_FC;
    local_FC = Script_StepUtils(GET_STEP_LOWER, 0);
    if ((local_FC == 2) | (local_FC == 3)) {
      Script_SetShouldSleepFlag(0);
      goto L01D707;
    }
  }
  L01D707:;
  Script_ryoSetChara(arg0);
  Script_GetSetWeatherSettings(WEATHER_SETTINGS_SET_DISABLE_WEATHER, 1);
  Script_WeatherUtils_0(WeatherDataUtil_ApplyCurrentWeatherOrEnableDateTimeRead, 0);
  Script_WeatherTaskUtil(InitTaskData);
  Script_facemotUtils(arg0, 1, 0);
  Script_facemotUtils(arg0, 0, 0);
  Script_FaceEx(arg0, 0, 0, 2);
  Script_CharHandlers(arg0, -1, 0, 0);
  script_1D57E();
  Script_SetCharaFlags(arg0, 4);
  Script_charaDisplay(arg0, CDM_Show);
  {
    int32_t local_FC;
    local_FC = Script_GetSceneID();
    if (local_FC == 2) {
      Script_LoadVoiceFile(1, "FREE02");
    } else if (local_FC == 3) {
      Script_LoadVoiceFile(1, "FREE03");
    } else if (local_FC == 4) {
      Script_LoadVoiceFile(1, "FREE04");
    } else {
      Script_LoadVoiceFile(1, "FREE01");
    }
  }
  Script_CPCTRelated(arg0, 32, 89);
  Script_clmdSetFlags(arg0, 8, 1);
  return;
}

static void script_1D7DA(int32_t arg0) {
  script_1D6C2(arg0);
  for (;;) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_1D7F1(int32_t arg0) {
  Vector3i local_F4;
  Script_mtwkGetSetRotation(CHARA_AKIR, &local_F4, 0);
  (&local_F4)->y = ((&local_F4)->y - arg0 + 10922) & 0xFFFF;
  if ((&local_F4)->y < 21845) {
    (void)-1;
  } else {
    (void)0;
  }
  return;
}

static void script_1D832(int32_t arg0) {
  Vector3i local_F4;
  Script_mtwkGetSetRotation(CHARA_AKIR, &local_F4, 0x42000000);
  (&local_F4)->y = ((&local_F4)->y - arg0 + 10922) & 0xFFFF;
  if ((&local_F4)->y < 21845) {
    (void)-1;
  } else {
    (void)0;
  }
  return;
}

static void script_1D876(int32_t arg0) {
  int32_t local_FC;
  local_FC = arg0;
  if ((local_FC == 0) | (local_FC == 5)) {
    Script_executeAicaCommand(5288, 0, 0);
  } else if (local_FC == 10) {
    Script_executeAicaCommand(70824, 0, 0);
  } else if (local_FC == 20) {
    Script_executeAicaCommand(136360, 0, 0);
  } else if (local_FC == 30) {
    Script_executeAicaCommand(201896, 0, 0);
  } else {
    Script_executeAicaCommand(267432, 0, 0);
  }
  return;
}

static void script_1D8F4(void) {
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_PROGRESS_STEP_28_REACHED)) {
    script_1D876(40);
  } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_PROGRESS_STEP_1E_REACHED)) {
    script_1D876(30);
  } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_PROGRESS_STEP_14_REACHED)) {
    script_1D876(20);
  } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JU00_CATA1_NBO_BRANCH_CHOSEN)) {
    script_1D876(10);
  } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_PROGRESS_STEP05_SA1116_DOJO_DONE)) {
    script_1D876(5);
  } else {
    script_1D876(0);
  }
  return;
}

static void script_1D973(int32_t arg0) {
  script_387C(arg0, -1000.0f, 0.0f, 0.0f);
  if (-1) {
    Script_charaDisplay(arg0, CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(arg0, CDM_Show);
      goto L01D9A9;
    }
  }
  L01D9A9:;
  Script_scenToggleVisibleFor(arg0, 0);
  return;
}

void script_1D9B3(void) {
  Script_filmCheckOrSetTransitionUtil(0, 255, 0, 0, 0, 0, 0, 0, 0);
  task_23C = Script_ryoSleepUtils(0);
  if ((Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_NOZOMI_KIDNAPPED) == 1) & (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOZOMI_RESCUE_RUSH_ACTIVE) == 0)) {
    Script_SetShouldSleepFlag(0);
  }
  Script_SetCameraSettings(5, 6, 1, 0);
  Script_SetMapFlags(0, 1);
  script_1D6C2(CHARA_AKIR);
  script_1BD3F();
  script_BAC8(CHARA_AKIR, (DSWKDoorEntry *)data_1FA70, (int32_t *)&data_1FCAC, (DoorResourceDescriptor *)&data_1FD60, (MapObjectRecord *)data_1FDD0, 5, 0);
  DSWK_DoorUtils(DOOR_CMD_SET_STATE_1);
  script_BDC8();
  script_1BC5B();
  Script_motiLoad(3, SHEN_NAME('FREE'), "M_MOBJ.BIN");
  Script_scenCreateTask(script_1BF49, 0);
  SCNF_WaitFrames_0000(1);
  Script_scenCreateTask(script_1B6EF, 0);
  Script_scenCreateTask(script_1C857, 0);
  Script_CycleManagerUtils(CYCLE_CMD_SETUP, "scene/03/D000", (int32_t)"SCRTEST.BIN", 0x3030554A);
  Script_CycleManagerUtils(CYCLE_CMD_RESET_LOAD_SETTLE);
  for (;;) {
    if (Script_CycleManagerUtils(CYCLE_CMD_TICK_LOAD_SETTLE_COUNT) == 0) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  script_17598(CHARA_CATM);
  if (task_23C == 0) {
    Script_cdttLoadScrUtils(1, 0);
    Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
    while (Script_filmCheckOrSetTransitionUtil(-1)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_ryoControl(RYO_RestoreControl);
  }
  script_1BAA5();
  Script_shenClearFreeState(1);
  script_1CF25();
  script_17667(CHARA_CATM);
  script_1CDF6();
  if (task_23C != 0) {
    Script_ryoSleepUtils(1);
    Script_cdttLoadScrUtils(1, 0);
    Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
    while (Script_filmCheckOrSetTransitionUtil(-1)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_ryoControl(RYO_RestoreControl);
  }
  return;
}
