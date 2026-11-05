#pragma once

#include "scenario.h"
#include "motion/motion.h"

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
