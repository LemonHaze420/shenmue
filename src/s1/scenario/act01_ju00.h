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
static void script_DF11(int32_t arg0);
static void script_ED5E(int32_t arg0);
static void script_F78A(int32_t arg0);
static void script_FB75(void);
static void script_FF79(void);
static void script_1001F(void);
static void script_10176(void);
static void script_10224(void);
static int32_t script_10296(int32_t arg0);
static int32_t script_102CA(int32_t arg0, int32_t arg1, float arg2, float arg3);
static int32_t script_1031A(void);
static void script_1034F(int32_t arg0);
static void script_103BB(void);
static void script_103C3(void);
static void script_1048F(void);
static void script_104BE(void);
static void script_104ED(void);
static void script_1051C(void);
static void script_1054B(void);
static void script_1057A(void);
static void script_105A9(void);
static void script_105D8(void);
static void script_106A9(void);
static void script_10A99(void);
static void script_1112F(void);
static void script_11413(void);
static void script_114A2(void);
static void script_115F7(void);
static int32_t script_11620(void);
static void script_11688(void);
static void script_117EA(void);
static void script_11879(void);
static void script_11974(void);
static void script_119A3(void);
static void script_119D2(void);
static void script_11A3F(void);
static void script_11AAC(void);
static void script_123AA(void);
static void script_124AC(void);
static void script_125F3(void);
static void script_12665(void);
static void script_12F2C(void);
static void script_1313B(void);
static void script_1321A(void);
static void script_132F9(int32_t arg0, int32_t arg1);
static void script_151E5(int32_t arg0);
static void script_152C1(void);
static void script_152C4(const char *arg0);
static void script_152D3(float arg0, const char *arg1, const char *arg2, const char *arg3, ...);
static void script_15362(void);
static void script_15383(void);
static int32_t script_1540D(int32_t arg0, int32_t arg1);
static int32_t script_1559C(int32_t arg0, int32_t arg1);
static int32_t script_1566F(int32_t arg0, int32_t arg1);
static int32_t script_15792(int32_t arg0, int32_t arg1);
static void script_158EB(void);
static void script_15924(void);
static void script_1593F(void);
static void script_15B90(void);
static void script_15C07(void);
static void script_169E7(void);
static void script_16A59(void);
static void script_16A7E(void);
static void script_16A98(void);
static void script_16AA4(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_16B5D(int32_t arg0);
static void script_171D2(float arg0, float arg1);
static void script_171F1(void);
static int32_t script_17523(int32_t arg0);
static void script_175A7(int32_t arg0, int32_t arg1);
static float script_175CD(int32_t arg0, float arg1, float arg2, float arg3, float arg4);
static int32_t script_17618(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4);
static void script_176BA(int32_t arg0);
static void script_176E7(int32_t arg0);
static void script_17714(int32_t arg0);
static void script_17748(int32_t arg0, float arg1, float arg2, float arg3, float arg4);
static void script_1794A(int32_t arg0);
static void script_17A19(int32_t arg0);
static void script_17AF0(int32_t arg0);
static void script_18484(int32_t arg0);
static void script_18490(int32_t arg0);
static void script_185A6(int32_t arg0);
static void script_18625(int32_t arg0);
static void script_1871C(int32_t arg0);
static void script_187F0(int32_t arg0);
static void script_188B0(int32_t arg0);
static void script_18916(int32_t arg0);
static void script_18990(int32_t arg0);
static void script_18A17(int32_t arg0);
static void script_18B0E(int32_t arg0);
static void script_18BEF(int32_t arg0);
static void script_18CD0(int32_t arg0);
static void script_18DA7(int32_t arg0);
static void script_18E94(int32_t arg0);
static void script_18F81(int32_t arg0);
static void script_190F5(int32_t arg0);
static int32_t script_19115(int32_t arg0);
static int32_t script_191D1(int32_t arg0);
static void script_1925E(void);
static void script_1929D(int32_t arg0);
static int32_t script_192B7(void);
static void script_192C6(void);
static int32_t script_19321(int32_t arg0);
static int32_t script_193B9(int32_t arg0);
static int32_t script_19678(int32_t arg0);
static void script_19857(int32_t arg0);
static int32_t script_19EBF(int32_t arg0);
static int32_t script_1A23B(int32_t arg0);
static int32_t script_1A345(int32_t arg0);
static int32_t script_1AE40(int32_t arg0);
static int32_t script_1AEC1(int32_t arg0, int32_t arg1, float arg2);
static void script_1BA69(void);
static int32_t script_1BAA1(void);
static void script_1BAD1(void);
static void script_1BAE0(int32_t arg0);
static void script_1BC0E(void);
static void script_1BC2C(void);
static void script_1BC9C(int32_t arg0);
static void script_1BCC9(void);
static void script_1BCF0(int32_t arg0);
static int32_t script_1BCFB(void);
static int32_t script_1BD4D(void);
static int32_t script_1BDC1(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_1BE57(void);
static void script_1C00D(void);
static void script_1C0FB(void);
static void script_1C166(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int8_t script_1C1EF(void);
static int8_t script_1C283(void);
static void script_1C346(int32_t arg0);
static int32_t script_1C371(int32_t arg0, int32_t arg1);
static int32_t script_1C383(int32_t arg0, int32_t arg1);
static int32_t script_1C3C5(void);
static void script_1C461(const char *arg0, const char *arg1);
static int32_t script_1C4B7(void);
static int8_t script_1C7B1(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_1C86A(void);
static void script_1C891(void);
static int32_t script_1CA57(void);
static int32_t script_1CA8F(void);
static void script_1CACB(void);
static int32_t script_1CE94(void);
static void script_1D1EF(void);
static int32_t script_1D22E(void);
static void script_1D433(void);
static void script_1D562(void);
static void script_1D874(int32_t arg0);
static void script_1D8DA(int32_t arg0);
static void script_1D940(int32_t arg0);
static void script_1D9A6(int32_t arg0);
static void script_1DA0C(void);
static void script_1DAAE(int32_t arg0);
static void script_1DB6D(int32_t arg0);
static void script_1DBB8(void);
static int32_t script_1DC2E(void);
static void script_1DD45(void);
static void script_1DF1F(void);
static void script_1E021(int32_t arg0);
static void script_1E1E2(int32_t arg0);
static void script_1E211(void);
static int32_t script_1E3B0(void);
static int32_t script_1ECE1(void);
static void script_1ECFD(int32_t arg0);
static void script_1F2C9(int32_t arg0);
static void script_1F895(int32_t arg0);
static void script_1FFA4(int32_t arg0);
static int32_t script_20376(int32_t arg0, int32_t arg1, int32_t arg2);
static int32_t script_203FF(int32_t arg0, float arg1, int32_t arg2);
static void script_2047A(void);
static void script_205B2(int32_t arg0);
static void script_205BE(int32_t arg0);
static void script_205CA(int32_t arg0);
static void script_206D6(int32_t arg0);
static void script_206ED(int32_t arg0);
static void script_2072E(int32_t arg0);
static void script_20772(int32_t arg0);
static void script_207F0(void);
static void script_2086F(int32_t arg0);
void script_208AF(void);

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
static int8_t task_24C;
static int8_t task_24D;
static int8_t task_24E;
static int8_t task_24F;
static Vector3 task_250;
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
static int32_t task_28C;
static int8_t task_290;
static int8_t task_291;
static int8_t task_292;
static int8_t task_293;
static int8_t task_294;
static int8_t task_295;
static int8_t task_296;
static int8_t task_297;
static int8_t task_298;
static uint8_t task_299;
static int8_t task_29A;
static int8_t task_29B;
static int8_t task_29C;
static uint8_t task_29D;
static uint16_t task_29E;
static Vector3 task_2A0;
static Vector3i task_2AC;
static int32_t task_2B8;
static int32_t task_2BC;
static int8_t task_2C0;
static int8_t task_2C1;
static int8_t task_2C2;
static int8_t task_2C3;
static int8_t task_2C4;
static uint8_t task_2C5;
static int8_t task_2C6;
static uint8_t task_2C7;
static const float data_20A88[3] = {1.0f, 1.0f, 1.0f};
static const int32_t data_20A94[4] = {16777215, 16777215, 16777215, 16777215};
static const float data_20AA4[3] = {20.0f, 800.0f, 1.0f};
static const int32_t data_20AB0 = -1;
static const char data_20AB4[10] __attribute__((scn_string_literal)) = "Kaidan : ";
static const char data_20ABE[32] __attribute__((scn_string_literal)) = "========== initializing kaidan\012";
static const char data_20ADE[48] __attribute__((scn_string_literal)) = "please ENDOFKDATA use can't start shuki kaidan\012";
static const char data_20B0E[] __attribute__((scn_string_literal)) = "Kaidan Function Error 1\n";
static const char data_20B27[] __attribute__((scn_string_literal)) = "Kaidan Function Error 2\n";
static const char data_20B40[] __attribute__((scn_string_literal)) = "Kaidan Data Error\n";
static const char data_20B53[10] __attribute__((scn_string_literal)) = "Kaidan : ";
static const char data_20B5D[28] __attribute__((scn_string_literal)) = "wait ... shuki shuki shuki\012";
static const char data_20B79[10] __attribute__((scn_string_literal)) = "Kaidan : ";
static const char data_20B83[24] __attribute__((scn_string_literal)) = "KAIDAN : Warning coli!\012";
static const char data_20B9B[24] __attribute__((scn_string_literal)) = "KAIDAN : Warning coli!\012";
static const char data_20BB3[24] __attribute__((scn_string_literal)) = "KAIDAN : Warning coli!\012";
static const char data_20BCB[24] __attribute__((scn_string_literal)) = "KAIDAN : Warning coli!\012";
static const char data_20BE3[24] __attribute__((scn_string_literal)) = "KAIDAN : Warning coli!\012";
static const char data_20BFB[24] __attribute__((scn_string_literal)) = "KAIDAN : Warning setY!\012";
static const char data_20C13[24] __attribute__((scn_string_literal)) = "KAIDAN : Warning setY!\012";
static const char data_20C2B[29] = "Kaidan : \000\000\000\000,\004\023\004-\004/\0042\0040\0041\004\207\003";
static const char data_20C48[20] __attribute__((scn_string_literal)) = "TUNED FreeTalk def\012";
static const char data_20C5C[20] __attribute__((scn_string_literal)) = "TUNED FreeTalk def\012";
static DoorMotionSelectorBlock data_20C70[38] = {
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
static ScnDoorMotionDescriptor data_21260[72] = {
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
static uint32_t data_21C80 = 0xFFFFFFFF;
typedef struct Row_21C84 { float w0; float w1; float w2; float w3; int32_t w4; } Row_21C84;
static const Row_21C84 data_21C84[7] = {
  {0.0f, 0.0f, 0.0f, 0.0f, -1},
  {0.0f, 0.0f, 0.0f, 0.0f, -1},
  {0.0f, 0.0f, 0.0f, 0.0f, 1534},
  {26.0f, 55.0f, 45.0f, 75.0f, 1555},
  {61.0f, 146.0f, 45.0f, 50.0f, 1554},
  {59.0f, 124.0f, 45.0f, 70.0f, 1543},
  {33.0f, 49.0f, 45.0f, 50.0f, 1543}
};
static const float data_21D10[4] = {33.0f, 49.0f, 50.0f, 50.0f};
static void (*data_21D20)(int32_t, int32_t, int32_t) = script_4275;
static void (*const data_21D24)(int32_t, int32_t, int32_t, int32_t) = script_4285;
static void (*const data_21D28)(int32_t, int32_t, int32_t, int32_t) = script_4285;
static void (*const data_21D2C)(int32_t, int32_t, int32_t, int32_t) = script_4285;
static void (*const data_21D30)(int32_t, int32_t, int32_t, int32_t) = script_4285;
static void (*const data_21D34)(int32_t, int32_t, int32_t, int32_t) = script_4285;
static void (*const data_21D38)(int32_t, int32_t, int32_t, int32_t) = script_4285;
static void (*const data_21D3C)(int32_t, int32_t, int32_t, int32_t) = script_4285;
static void (*const data_21D40)(int32_t, int32_t, int32_t, int32_t) = script_4285;
static void (*const data_21D44)(int32_t, int32_t, int32_t, int32_t) = script_452B;
static void (*const data_21D48)(int32_t, int32_t, int32_t, int32_t) = script_468C;
static void (*const data_21D4C)(int32_t, int32_t, int32_t, int32_t) = script_4B59;
static void (*const data_21D50)(int32_t, int32_t, int32_t, int32_t) = script_4BC0;
static void (*const data_21D54)(int32_t, int32_t, int32_t, int32_t) = script_4389;
static void (*const data_21D58)(int32_t, int32_t, int32_t, int32_t) = script_4389;
static void (*const data_21D5C)(int32_t, int32_t, int32_t, int32_t) = script_47ED;
static void (*const data_21D60)(int32_t, int32_t, int32_t, int32_t) = script_4A8B;
static void (*const data_21D64)(int32_t, int32_t, int32_t, int32_t) = script_4C27;
static void (*const data_21D68)(int32_t, int32_t, int32_t, int32_t) = script_452B;
static void (*const data_21D6C)(int32_t, int32_t, int32_t, int32_t) = script_468C;
static void (*const data_21D70)(int32_t, int32_t, int32_t, int32_t) = script_47ED;
static void (*const data_21D74)(int32_t, int32_t, int32_t, int32_t) = script_452B;
static void (*const data_21D78)(int32_t, int32_t, int32_t, int32_t) = script_468C;
static void (*const data_21D7C)(int32_t, int32_t, int32_t, int32_t) = script_47ED;
static void (*const data_21D80)(int32_t, int32_t, int32_t, int32_t) = script_4C6E;
static void (*const data_21D84)(int32_t, int32_t, int32_t, int32_t) = script_4C6E;
static void (*const data_21D88)(int32_t, int32_t, int32_t, int32_t) = script_4C6E;
static void (*const data_21D8C)(int32_t, int32_t, int32_t, int32_t) = script_4C6E;
static void (*const data_21D90)(int32_t, int32_t, int32_t, int32_t) = script_4DA4;
static void (*const data_21D94)(int32_t, int32_t, int32_t, int32_t) = script_5665;
static void (*const data_21D98)(int32_t, int32_t, int32_t, int32_t) = script_4285;
static void (*const data_21D9C)(int32_t, int32_t, int32_t, int32_t) = script_4285;
static void (*const data_21DA0)(int32_t, int32_t, int32_t, int32_t) = script_4285;
static void (*const data_21DA4)(int32_t, int32_t, int32_t, int32_t) = script_4285;
static void (*const data_21DA8)(int32_t, int32_t, int32_t, int32_t) = script_4285;
static void (*const data_21DAC)(int32_t, int32_t, int32_t, int32_t) = script_4285;
static void (*const data_21DB0)(int32_t, int32_t, int32_t, int32_t) = script_4285;
static void (*const data_21DB4)(int32_t, int32_t, int32_t, int32_t) = script_4285;
static const char data_21DB8[46] __attribute__((scn_string_literal)) = "Door Index = %d : call_type = %d : type = %d\012";
static const char data_21DE6[43] __attribute__((scn_string_literal)) = "Door Open Motion = DEB_AKESIME_FUSUMA_L_F\012";
static const char data_21E11[43] __attribute__((scn_string_literal)) = "Door Open Motion = DEB_AKESIME_FUSUMA_R_F\012";
static const char data_21E3C[42] __attribute__((scn_string_literal)) = "Door Open Motion = DEB_HIKISIME_DOOR_L_F\012";
static const char data_21E66[42] __attribute__((scn_string_literal)) = "Door Open Motion = DEB_HIKISIME_DOOR_R_F\012";
static const char data_21E90[41] __attribute__((scn_string_literal)) = "Door Open Motion = DEB_OSISIME_DOOR_L_F\012";
static const char data_21EB9[41] __attribute__((scn_string_literal)) = "Door Open Motion = DEB_OSISIME_DOOR_R_F\012";
static const char data_21EE2[43] __attribute__((scn_string_literal)) = "Door Open Motion = GAK_AKESIME_FUSUMA_L_F\012";
static const char data_21F0D[43] __attribute__((scn_string_literal)) = "Door Open Motion = GAK_AKESIME_FUSUMA_R_F\012";
static const char data_21F38[42] __attribute__((scn_string_literal)) = "Door Open Motion = GAK_HIKISIME_DOOR_L_F\012";
static const char data_21F62[42] __attribute__((scn_string_literal)) = "Door Open Motion = GAK_HIKISIME_DOOR_R_F\012";
static const char data_21F8C[41] __attribute__((scn_string_literal)) = "Door Open Motion = GAK_OSISIME_DOOR_L_F\012";
static const char data_21FB5[41] __attribute__((scn_string_literal)) = "Door Open Motion = GAK_OSISIME_DOOR_R_F\012";
static const char data_21FDE[43] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_AKESIME_FUSUMA_L_F\012";
static const char data_22009[43] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_AKESIME_FUSUMA_R_F\012";
static const char data_22034[42] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_HIKISIME_DOOR_L_F\012";
static const char data_2205E[42] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_HIKISIME_DOOR_R_F\012";
static const char data_22088[41] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_OSISIME_DOOR_L_F\012";
static const char data_220B1[41] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_OSISIME_DOOR_R_F\012";
static const char data_220DA[43] __attribute__((scn_string_literal)) = "Door Open Motion = KOD_AKESIME_FUSUMA_L_F\012";
static const char data_22105[43] __attribute__((scn_string_literal)) = "Door Open Motion = KOD_AKESIME_FUSUMA_R_F\012";
static const char data_22130[42] __attribute__((scn_string_literal)) = "Door Open Motion = KOD_HIKISIME_DOOR_L_F\012";
static const char data_2215A[42] __attribute__((scn_string_literal)) = "Door Open Motion = KOD_HIKISIME_DOOR_R_F\012";
static const char data_22184[41] __attribute__((scn_string_literal)) = "Door Open Motion = KOD_OSISIME_DOOR_L_F\012";
static const char data_221AD[41] __attribute__((scn_string_literal)) = "Door Open Motion = KOD_OSISIME_DOOR_R_F\012";
static const char data_221D6[43] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_AKESIME_FUSUMA_L_F\012";
static const char data_22201[43] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_AKESIME_FUSUMA_R_F\012";
static const char data_2222C[42] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_HIKISIME_DOOR_L_F\012";
static const char data_22256[42] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_HIKISIME_DOOR_R_F\012";
static const char data_22280[41] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_OSISIME_DOOR_L_F\012";
static const char data_222A9[41] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_OSISIME_DOOR_R_F\012";
static const char data_222D2[43] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_AKESIME_FUSUMA_L_F\012";
static const char data_222FD[43] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_AKESIME_FUSUMA_R_F\012";
static const char data_22328[42] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_HIKISIME_DOOR_L_F\012";
static const char data_22352[42] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_HIKISIME_DOOR_R_F\012";
static const char data_2237C[41] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_OSISIME_DOOR_L_F\012";
static const char data_223A5[41] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_OSISIME_DOOR_R_F\012";
static const char data_223CE[43] __attribute__((scn_string_literal)) = "Door Open Motion = SIN_AKESIME_FUSUMA_L_F\012";
static const char data_223F9[43] __attribute__((scn_string_literal)) = "Door Open Motion = SIN_AKESIME_FUSUMA_R_F\012";
static const char data_22424[42] __attribute__((scn_string_literal)) = "Door Open Motion = SIN_HIKISIME_DOOR_L_F\012";
static const char data_2244E[42] __attribute__((scn_string_literal)) = "Door Open Motion = SIN_HIKISIME_DOOR_R_F\012";
static const char data_22478[41] __attribute__((scn_string_literal)) = "Door Open Motion = SIN_OSISIME_DOOR_L_F\012";
static const char data_224A1[41] __attribute__((scn_string_literal)) = "Door Open Motion = SIN_OSISIME_DOOR_R_F\012";
static const char data_224CA[43] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_AKESIME_FUSUMA_L_F\012";
static const char data_224F5[43] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_AKESIME_FUSUMA_R_F\012";
static const char data_22520[42] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_HIKISIME_DOOR_L_F\012";
static const char data_2254A[42] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_HIKISIME_DOOR_R_F\012";
static const char data_22574[41] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_OSISIME_DOOR_L_F\012";
static const char data_2259D[41] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_OSISIME_DOOR_R_F\012";
static const char data_225C6[43] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_AKESIME_FUSUMA_L_F\012";
static const char data_225F1[43] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_AKESIME_FUSUMA_R_F\012";
static const char data_2261C[42] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_HIKISIME_DOOR_L_F\012";
static const char data_22646[42] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_HIKISIME_DOOR_R_F\012";
static const char data_22670[41] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_OSISIME_DOOR_L_F\012";
static const char data_22699[41] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_OSISIME_DOOR_R_F\012";
static const char data_226C2[44] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_OSISIME_DOOR_AP_L_F\012";
static const char data_226EE[44] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_OSISIME_DOOR_AP_L_F\012";
static const char data_2271A[44] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_OSISIME_DOOR_AP_L_F\012";
static const char data_22746[44] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_OSISIME_DOOR_AP_L_F\012";
static const char data_22772[44] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_OSISIME_DOOR_AP_L_F\012";
static const char data_2279E[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_227BA[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_227D6[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_227F2[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_2280E[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_2282A[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_22846[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_22862[13] __attribute__((scn_string_literal)) = "open-move!!\012";
static const char data_2286F[11] __attribute__((scn_string_literal)) = "M_MDOR.BIN";
static const char data_2287A[] __attribute__((scn_string_literal)) = "This Charactor Work is NULL!!\n";
static const char data_22899[22] __attribute__((scn_string_literal)) = "手動ドア開け番号指定\012";
static const char data_228AF[34] __attribute__((scn_string_literal)) = "無限ループになりそうなので抜ける\012";
static const char data_228D1[] __attribute__((scn_string_literal)) = "Don't make BOX Buffer!!\n";
static const char data_228EA[] __attribute__((scn_string_literal)) = "Don't make BOX Buffer!!\n";
static const char data_22903[15] __attribute__((scn_string_literal)) = "Door System : ";
static const char data_22912[15] __attribute__((scn_string_literal)) = "Door System : ";
static const char data_22921[18] __attribute__((scn_string_literal)) = "Door Box is ON!!\012";
static const char data_22933[19] __attribute__((scn_string_literal)) = "Door Box is OFF!!\012";
static const char data_22946[18] __attribute__((scn_string_literal)) = "Door Box is ON!!\012";
static const char data_22958[20] __attribute__((scn_string_literal)) = "Door Box is OFF!!\012";
static const DSWKDoorEntry data_2296C[10] = {
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
static const int32_t data_22B74[13] = {-1, 0, 6553700, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1};
static const uint32_t data_22BA8 = 8;
static const int32_t data_22BAC[28] = {9, -1, 18, -1, 10, 11, -1, 16, 17, -1, 12, 13, -1, 6, 7, -1, 4, 5, -1, 14, 15, -1, 0, 1, -1, 2, 3, -1};
static char data_22C1C[] = "DR03_001";
static char data_22C25[] = "DR00_005";
static char data_22C2E[] = "DR00_010";
static char data_22C37[] = "DR15_023";
static char data_22C40[] = "DR15_022";
static char data_22C49[] = "DR15_019";
static char data_22C52[] = "DR29_000";
static const uint8_t data_22C5B[1] = {0};
static char *const data_22C5C = data_22C1C;
static const int32_t data_22C60[3] = {3, 1311403, 1311403};
static char *const data_22C6C = data_22C25;
static const int32_t data_22C70[3] = {0, -1, -1};
static char *const data_22C7C = data_22C2E;
static const int32_t data_22C80[3] = {0, -1, -1};
static char *const data_22C8C = data_22C37;
static const int32_t data_22C90[3] = {15, 524971, 590507};
static char *const data_22C9C = data_22C40;
static const int32_t data_22CA0[3] = {15, 524971, 590507};
static char *const data_22CAC = data_22C49;
static const int32_t data_22CB0[3] = {15, 524971, 590507};
static char *const data_22CBC = data_22C52;
static const int32_t data_22CC0[3] = {29, 787115, 852651};
static const MapObjectRecord data_22CCC[19] = {
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
static void (*data_22F78)(void) = script_D9CD;
static void (*const data_22F7C)(int32_t) = script_D9D0;
static const uint32_t data_22F80 = 0xC2680000;
static const float data_22F84[2] = {0.0f, 85.0f};
static const uint32_t data_22F8C = 0xC2180000;
static const float data_22F90[2] = {0.0f, 107.0f};
static const uint32_t data_22F98 = 0xC23C0000;
static const float data_22F9C[2] = {0.0f, 22.0f};
static const uint32_t data_22FA4 = 0xC1500000;
static const float data_22FA8[2] = {0.0f, 26.0f};
static const uint32_t data_22FB0 = 0xC2040000;
static const float data_22FB4[2] = {0.0f, 34.0f};
static const uint32_t data_22FBC = 0xC0A00000;
static const float data_22FC0[2] = {0.0f, 43.0f};
static const uint32_t data_22FC8 = 0xC2200000;
static const float data_22FCC[2] = {0.0f, 20.0f};
static const uint8_t data_22FD4[8] = {0};
static const float data_22FDC = 100.0f;
static const uint8_t data_22FE0[8] = {0};
static const float data_22FE8 = 100.0f;
static const uint8_t data_22FEC[8] = {0};
static const float data_22FF4 = 50.0f;
static const uint32_t data_22FF8 = 0x3F800000;
static const float data_22FFC[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.8f, 0.7f, 0.5f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint32_t data_23080 = 0x3F800000;
static const float data_23084[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.8f, 0.7f, 0.5f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint32_t data_23108 = 0x3F800000;
static const float data_2310C[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.8f, 0.7f, 0.5f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint32_t data_23190 = 0x3F800000;
static const float data_23194[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.8f, 0.7f, 0.5f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint32_t data_23218 = 0x3F800000;
static const float data_2321C[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.8f, 0.7f, 0.5f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint32_t data_232A0 = 0x3F800000;
static const float data_232A4[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.8f, 0.7f, 0.5f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint8_t data_23328[80] = {0};
typedef struct Row_23378 { float w0; float w1; float w2; int32_t w3; } Row_23378;
static const Row_23378 data_23378[3] = {
  {1.0f, 1.0f, 1.0f, 0},
  {0.0f, 0.0f, 0.0f, 0},
  {0.0f, 0.0f, 0.0f, 0}
};
static const int32_t data_233A8[2] = {0, 0};
static const uint8_t data_233B0[96] = {0};
static const float data_23410[10] = {0.8f, 0.8f, 0.8f, 0.8f, 0.8f, 0.8f, 0.8f, 0.8f, 0.8f, 0.8f};
static const uint32_t data_23438 = 0x3F800000;
static const float data_2343C[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint32_t data_234C0 = 0x3F800000;
static const float data_234C4[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const int32_t data_23548[407] = {
  -1, -1, -1, -1, -1, -1, -1, 65535, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -10922, 0, 0, -10558, 0, 1456, 0, 0, 0, -18022, 0, 0, -17112, 0, 0, -18386, 0, 0, 0,
  0, 0, -12196, 0, 0, -18204, 0, 0, -16748, 0, 0, 0, 0, 0, -11832, 0, 0, -18386, 0, 0, -15109, 0, 0, 0, 0, 910, -12014, 0, -2184, -16748, 0, -2184,
  -10012, 0, 0, 0, 0, -364, -11650, -2912, -364, -4369, 1638, 910, 182, 0, 0, -16019, 0, 182, -23119, 0, 546, -18932, 0, 0, -1092, 0, 0, -16384, 0, 0,
  -21845, 0, 0, -18750, 0, 0, -546, 0, 0, -21481, 0, 0, -21299, 0, 0, -18568, 0, 0, -728, 0, 910, -18750, 0, 182, -18750, 0, -4004, -18204, 0, 0,
  -728, 0, -364, -14017, -2912, -364, -6735, 1638, 910, 2548, 0, 0, -18568, 0, 182, -25668, 0, 546, -21481, 0, 0, -3640, 0, 0, -18932, 0, 0, -24393,
  0, 0, -21299, 0, 0, -3094, 0, 0, -24029, 0, 0, -23847, 0, 0, -21117, 0, 0, -3276, 0, 910, -21299, 0, 182, -21299, 0, -4004, -20753, 0, 0, -3276, 0,
  0, -5461, 0, -2366, -1092, 0, -182, 1092, 0, -546, -6735, 0, 0, -4004, 0, 0, -2912, 0, -182, 1274, 0, 0, -13289, 0, 0, -5279, 0, 0, -8009, 0, 0,
  1456, 0, 182, -8738, 0, 0, -6007, 0, 0, -8738, 0, 182, 1638, 0, -728, -5279, 0, -1274, -6735, 0, -910, -8738, 0, 0, 1638, 0, 0, -5461, 0, -2366,
  -1092, 0, -182, 1092, 0, -546, -13289, 0, 0, -5643, 0, 0, -8009, 0, -182, 1274, 0, 0, -11832, 0, 0, -5279, 0, 0, -8009, 0, 0, 1456, 0, 182, -8738,
  0, 0, -6007, 0, 0, -8738, 0, 182, 1638, 0, -728, -5279, 0, -1274, -6735, 0, -910, -8738, 0, 0, 1092, 0, 0, -5461, 0, -2366, -1092, 0, -182, 1092, 0,
  -546, -3640, 0, 0, -2366, 0, 0, -1456, 0, -182, 1274, 0, 0, -3822, 0, 0, -2730, 0, 0, -1456, 0, 0, 1456, 0, 182, -3822, 0, 0, -2912, 0, 0, -1638, 0,
  182, 1638, 0, -728, -3640, 0, -910, -2730, 0, -546, -1638, 0, 0, 1092
};
static const int32_t data_23BA4[342] = {
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
static const int32_t data_240FC[57] = {
  0, 0, -7281, 0, 0, -4004, 0, 546, 182, 0, 0, -18204, 0, 0, -16930, 0, 0, -12379, 0, 0, 0, 0, 0, -12196, 0, 0, -18204, 0, 0, -13289, 0, 0, 0, 0, 0,
  -11832, 0, 0, -18386, 0, 0, -13835, 0, 0, 0, 0, 910, -12014, 0, -2184, -16748, 0, -2002, -13835, 0, 0, 364
};
static const int32_t data_241E0[57] = {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -8009, 0, 0, -5825, 0, 0, -3458, 0, 0, 0, 0, 0, -6553, 0, 0, -7281, 0, 0, -3640, 0, 0, 0, 0, 0, -4369, 0, 0, -7281,
  0, 0, -3640, 0, 0, 0, 0, 0, -7281, 0, 0, -5825, -2184, 0, -5825, -2912, 0, 0
};
static const int32_t data_242C4[57] = {
  0, 0, 0, 0, 0, 0, 0, -1638, 1456, 0, 0, 0, 0, 182, -5097, 0, 0, -3458, 0, 0, 0, 0, 0, 0, 0, 0, -4733, 0, 0, -3640, 0, 0, 0, 0, 0, -910, 0, 0, -6917,
  0, 0, -1820, 0, 0, 0, 0, 0, -1820, 0, 0, -6735, 0, -1456, -2002, 0, 0, 0
};
static const Vector3i data_243A8 = {.x = -1274, .y = 910, .z = -1820};
static const int32_t data_243B4[261] = {
  0, 0, -1820, 0, -182, -182, 0, 182, -2366, 0, -546, -8738, 0, 910, -10740, 0, 0, -4369, 0, -182, 1274, 0, 0, -15473, 0, 0, -19114, 0, 0, -11468, 0,
  0, 1092, 0, 182, -15109, 0, 0, -19114, 0, 0, -14927, 0, 182, 1638, 0, -182, -16566, 0, -1274, -20206, 0, -1638, -15291, 0, 0, 1274, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, -8009, 0, 0, -5825, 0, 0, -3458, 0, 0, 0, 0, 0, -6553, 0, 0, -7281, 0, 0, -3640, 0, 0, 0, 0, 0, -4369, 0, 0, -7281, 0, 0, -3640,
  0, 0, 0, 0, 0, -7281, 0, 0, -5825, -2184, 0, -5825, -2912, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1638, 1456, 0, 0, 0, 0, 182, -5097, 0, 0, -3458, 0, 0, 0, 0,
  0, 0, 0, 0, -4733, 0, 0, -3640, 0, 0, 0, 0, 0, -910, 0, 0, -6917, 0, 0, -1820, 0, 0, 0, 0, 0, -1820, 0, 0, -6735, 0, -1456, -2002, 0, 0, 0, 5643, 0,
  0, 0, 2548, 0, 6189, 2366, 0, 8374, 57344, 0, 0, 0, 63351, 0, 63169, 62987, 0, 62623, 3276, 62805, 63715, 0, 0, 0, 0, 0, 0, 0, 57344, 1456, 63715,
  0, 0, 0, 0, 0, 0, 0, 8192, 0, 0, 0, 4004, 0, 0, 5825, 0, 0, 57344, 1456, 63715, 0, 0, 0, 0, 0, 0, 0, 0, 61895, 0, 0, 1274, 0, 4187, 3094, 0, 3458,
  57344, 1456, 63715, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1820, 0, 4915, 2912, 0, 3822
};
static const int32_t data_247C8[20] = {57344, 0, 0, 0, 62259, 0, 63169, 60802, 0, 60620, 0, 0, 0, 0, 1820, 0, 4915, 2912, 0, 3822};
static const int32_t data_24818[10] = {57344, 0, 0, 0, 62805, 0, 61713, 62259, 0, 62805};
static const int32_t data_24840[10] = {4369, 2548, 0, 0, 2548, 0, 4004, 2730, 0, 4915};
static const int32_t data_24868[10] = {58982, 56251, 0, 0, 62805, 0, 61713, 62259, 0, 62805};
static const int32_t data_24890[10] = {4369, 61531, 0, 0, 2548, 0, 4004, 2730, 0, 4915};
static const int32_t data_248B8[20] = {60984, 61713, 0, 0, 62805, 0, 61713, 62259, 0, 62805, 4369, 61531, 0, 0, 2548, 0, 4004, 2730, 0, 4915};
static const int32_t data_24908[10] = {62077, 0, 0, 0, 64261, 0, 61531, 62805, 0, 61531};
static const int32_t data_24930[20] = {0, 62987, 0, 0, 3458, 0, 6007, 4004, 0, 3276, 57344, 1456, 63715, 0, 0, 0, 0, 0, 0, 0};
static const int32_t data_24980[20] = {3276, 62805, 63715, 0, 0, 0, 0, 0, 0, 0, 57344, 1456, 63715, 0, 0, 0, 0, 0, 0, 0};
static const int32_t data_249D0[20] = {0, 62805, 0, 0, 4915, 0, 4551, 5279, 0, 4551, 57344, 1456, 63715, 0, 0, 0, 0, 0, 0, 0};
static const int32_t data_24A20[10] = {64261, 62259, 0, 0, 3458, 0, 6189, 2002, 0, 8738};
static const int32_t data_24A48[10] = {0, 5461, 0, 0, 0, 0, 0, 0, 0, 0};
static const int32_t data_24A70[20] = {0, 0, 0, 0, 1820, 0, 4915, 2912, 0, 3822, 57344, 0, 0, 0, 62259, 0, 63169, 60802, 0, 60620};
static const int32_t data_24AC0[20] = {0, 58982, 0, 0, 4369, 0, 0, 3094, 0, 0, 57344, 0, 0, 0, 62259, 0, 63169, 60802, 0, 60620};
static const FogColourData data_24B10 = {.red = 14.1f, .green = 800.0f, .blue = 0.48f, .colour = 0x7F9FD1FFU};
static const uint32_t data_24B20[4] = {0, 0, 0xBB00663CU, 0xBB00663CU};
static const FogColourData data_24B30 = {.red = 14.1f, .green = 800.0f, .blue = 0.48f, .colour = 0x7F9FD1FFU};
static const uint32_t data_24B40[4] = {0, 0, 0xBB00663CU, 0xBB00663CU};
static const char data_24B50[] __attribute__((scn_string_literal)) = "E1041A159";
static const char data_24B5A[] __attribute__((scn_string_literal)) = "E1041A160";
static const char data_24B64[] __attribute__((scn_string_literal)) = "E1041A161";
static const char data_24B6E[] __attribute__((scn_string_literal)) = "E1041A118";
static const char data_24B78[] __attribute__((scn_string_literal)) = "E1041A119";
static const char data_24B82[] __attribute__((scn_string_literal)) = "E1041A120";
static const char data_24B8C[] __attribute__((scn_string_literal)) = "E1041A118";
static const char data_24B96[] __attribute__((scn_string_literal)) = "E1041A119";
static const char data_24BA0[] __attribute__((scn_string_literal)) = "E1041A120";
static const char data_24BAA[] __attribute__((scn_string_literal)) = "E1041A118";
static const char data_24BB4[] __attribute__((scn_string_literal)) = "E1041A119";
static const char data_24BBE[] __attribute__((scn_string_literal)) = "E1041A120";
static const char data_24BC8[] __attribute__((scn_string_literal)) = "E1041A118";
static const char data_24BD2[] __attribute__((scn_string_literal)) = "E1041A119";
static const char data_24BDC[] __attribute__((scn_string_literal)) = "E1041A120";
static const char data_24BE6[] __attribute__((scn_string_literal)) = "E1041A118";
static const char data_24BF0[] __attribute__((scn_string_literal)) = "E1041A119";
static const char data_24BFA[] __attribute__((scn_string_literal)) = "E1041A120";
static const char data_24C04[] __attribute__((scn_string_literal)) = "E1041D007";
static const char data_24C0E[] __attribute__((scn_string_literal)) = "E1041E011";
static const char data_24C18[] __attribute__((scn_string_literal)) = "E1041A027";
static const char data_24C22[] __attribute__((scn_string_literal)) = "E1041D010";
static const char data_24C2C[] __attribute__((scn_string_literal)) = "E1041A112";
static const char data_24C36[] __attribute__((scn_string_literal)) = "E1041E002";
static const char data_24C40[] __attribute__((scn_string_literal)) = "E1041D002";
static const char data_24C4A[] __attribute__((scn_string_literal)) = "E1041E008";
static const char data_24C54[] __attribute__((scn_string_literal)) = "E1041D008";
static const char data_24C5E[] __attribute__((scn_string_literal)) = "E1041A113";
static const char data_24C68[] __attribute__((scn_string_literal)) = "E1041D001";
static const char data_24C72[] __attribute__((scn_string_literal)) = "E1041E011";
static const char data_24C7C[] __attribute__((scn_string_literal)) = "E1041A027";
static const char data_24C86[] __attribute__((scn_string_literal)) = "E1041D010";
static const char data_24C90[] __attribute__((scn_string_literal)) = "E1041A138";
static const char data_24C9A[] __attribute__((scn_string_literal)) = "E1041E002";
static const char data_24CA4[] __attribute__((scn_string_literal)) = "E1041D002";
static const char data_24CAE[] __attribute__((scn_string_literal)) = "E1041E012";
static const char data_24CB8[] __attribute__((scn_string_literal)) = "E1041D003";
static const char data_24CC2[] __attribute__((scn_string_literal)) = "E1041A029";
static const char data_24CCC[] __attribute__((scn_string_literal)) = "E1041C011";
static const char data_24CD6[] __attribute__((scn_string_literal)) = "E1041A021";
static const char data_24CE0[] __attribute__((scn_string_literal)) = "E1041C002";
static const char data_24CEA[] __attribute__((scn_string_literal)) = "E1041C012";
static const char data_24CF4[] __attribute__((scn_string_literal)) = "E1041A111";
static const char data_24CFE[] __attribute__((scn_string_literal)) = "E1041C004";
static const char data_24D08[] __attribute__((scn_string_literal)) = "E1041A166";
static const char data_24D12[] __attribute__((scn_string_literal)) = "E1041C013";
static const char data_24D1C[] __attribute__((scn_string_literal)) = "E1041A023";
static const char data_24D26[] __attribute__((scn_string_literal)) = "E1041C015";
static const char data_24D30[] __attribute__((scn_string_literal)) = "E1041C001";
static const char data_24D3A[] __attribute__((scn_string_literal)) = "E1041A021";
static const char data_24D44[] __attribute__((scn_string_literal)) = "E1041C002";
static const char data_24D4E[] __attribute__((scn_string_literal)) = "E1041C003";
static const char data_24D58[] __attribute__((scn_string_literal)) = "E1041A135";
static const char data_24D62[] __attribute__((scn_string_literal)) = "E1041C004";
static const char data_24D6C[] __attribute__((scn_string_literal)) = "E1041A131";
static const char data_24D76[] __attribute__((scn_string_literal)) = "E1041C005";
static const char data_24D80[] __attribute__((scn_string_literal)) = "E1041A023";
static const char data_24D8A[] __attribute__((scn_string_literal)) = "E1041D010";
static const char data_24D94[] __attribute__((scn_string_literal)) = "E1041B047";
static const char data_24D9E[] __attribute__((scn_string_literal)) = "E1041A165";
static const char data_24DA8[] __attribute__((scn_string_literal)) = "E1041B051";
static const char data_24DB2[] __attribute__((scn_string_literal)) = "E1041B048";
static const char data_24DBC[] __attribute__((scn_string_literal)) = "E1041A014";
static const char data_24DC6[] __attribute__((scn_string_literal)) = "E1041B052";
static const char data_24DD0[] __attribute__((scn_string_literal)) = "E1041A166";
static const char data_24DDA[] __attribute__((scn_string_literal)) = "E1041B053";
static const char data_24DE4[] __attribute__((scn_string_literal)) = "E1041A132";
static const char data_24DEE[] __attribute__((scn_string_literal)) = "E1041B054";
static const char data_24DF8[] __attribute__((scn_string_literal)) = "E1041B001";
static const char data_24E02[] __attribute__((scn_string_literal)) = "E1041A130";
static const char data_24E0C[] __attribute__((scn_string_literal)) = "E1041B051";
static const char data_24E16[] __attribute__((scn_string_literal)) = "E1041B003";
static const char data_24E20[] __attribute__((scn_string_literal)) = "E1041A014";
static const char data_24E2A[] __attribute__((scn_string_literal)) = "E1041B052";
static const char data_24E34[] __attribute__((scn_string_literal)) = "E1041A131";
static const char data_24E3E[] __attribute__((scn_string_literal)) = "E1041B053";
static const char data_24E48[] __attribute__((scn_string_literal)) = "E1041A132";
static const char data_24E52[] __attribute__((scn_string_literal)) = "E1041B054";
static const char data_24E5C[] __attribute__((scn_string_literal)) = "E1041A122";
static const char data_24E66[] __attribute__((scn_string_literal)) = "E1041A123";
static const char data_24E70[] __attribute__((scn_string_literal)) = "E1041A162";
static const char data_24E7A[] __attribute__((scn_string_literal)) = "E1041A122";
static const char data_24E84[] __attribute__((scn_string_literal)) = "E1041A123";
static const char data_24E8E[] __attribute__((scn_string_literal)) = "E1041A124";
static const Vector4 data_24E98 = {.x = 0.0f, .y = 0.3f, .z = 0.3f, .w = 0.3f};
static const Vector4 data_24EA8 = {.x = 0.0f, .y = 0.3f, .z = 0.3f, .w = 0.3f};
static const char data_24EB8[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_24EBD[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_24EC2[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_24EC7[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_24ECC[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_24ED1[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_24ED6[11] __attribute__((scn_string_literal)) = "bgm051.snd";
static const char data_24EE1[13] __attribute__((scn_string_literal)) = "a1_neko1.snd";
static const char data_24EEE[7] __attribute__((scn_string_literal)) = "01CAT1";
static const char data_24EF5[13] __attribute__((scn_string_literal)) = "seqdata0.bin";
static const char data_24F02[6] __attribute__((scn_string_literal)) = "CATA1";
static const char data_24F08[13] __attribute__((scn_string_literal)) = "seqdata1.bin";
static const char data_24F15[6] __attribute__((scn_string_literal)) = "CATA1";
static const char data_24F1B[13] __attribute__((scn_string_literal)) = "seqdata2.bin";
static const char data_24F28[6] __attribute__((scn_string_literal)) = "CATA1";
static const char data_24F2E[7] __attribute__((scn_string_literal)) = "01CAT1";
static const char data_24F35[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_24F3A[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_24F3F[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_24F44[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_24F49[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_24F4E[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_24F53[7] __attribute__((scn_string_literal)) = "SELECT";
static const char data_24F5A[13] __attribute__((scn_string_literal)) = "f1jutaup.snd";
static const char data_24F67[6] __attribute__((scn_string_literal)) = "CATA1";
static const char data_24F6D[] __attribute__((scn_string_literal)) = "E1041A116";
static const uint8_t data_24F77[1] = {0};
static const Vector4 data_24F78 = {.x = 0.0f, .y = 0.3f, .z = 0.3f, .w = 0.3f};
static const char data_24F88[] __attribute__((scn_string_literal)) = "E1041A139";
static const char data_24F92[] __attribute__((scn_string_literal)) = "E1041A140";
static const char data_24F9C[] __attribute__((scn_string_literal)) = "E1041A141";
static const char data_24FA6[] __attribute__((scn_string_literal)) = "E1041A142";
static const char data_24FB0[] __attribute__((scn_string_literal)) = "E1041A046";
static const char data_24FBA[] __attribute__((scn_string_literal)) = "E1041A145";
static const char data_24FC4[] __attribute__((scn_string_literal)) = "E1041A146";
static const char data_24FCE[] __attribute__((scn_string_literal)) = "E1041A049";
static const char data_24FD8[] __attribute__((scn_string_literal)) = "E1041A147";
static const char data_24FE2[] __attribute__((scn_string_literal)) = "E1041A148";
static const char data_24FEC[] __attribute__((scn_string_literal)) = "E1041A040";
static const char data_24FF6[] __attribute__((scn_string_literal)) = "E1041A143";
static const char data_25000[] __attribute__((scn_string_literal)) = "E1041A144";
static const char data_2500A[] __attribute__((scn_string_literal)) = "E1041A037";
static const char data_25014[] __attribute__((scn_string_literal)) = "E1041A054";
static const char data_2501E[] __attribute__((scn_string_literal)) = "E1041A139";
static const char data_25028[] __attribute__((scn_string_literal)) = "E1041A121";
static const char data_25032[] __attribute__((scn_string_literal)) = "E1041A149";
static const char data_2503C[] __attribute__((scn_string_literal)) = "E1041A150";
static const char data_25046[] __attribute__((scn_string_literal)) = "E1041A151";
static const uint16_t data_25050[7] = {1068, 1043, 1069, 1076, 1074, 1072, 1073};
static const char data_2505E[] __attribute__((scn_string_literal)) = "E1041A116";
static const char data_25068[] __attribute__((scn_string_literal)) = "E1041A170";
static const char data_25072[] __attribute__((scn_string_literal)) = "E1041A171";
static const char data_2507C[] __attribute__((scn_string_literal)) = "E1041A172";
static const uint8_t data_25086[2] = {0};
static const Vector4 data_25088 = {.x = 0.0f, .y = 0.3f, .z = 0.3f, .w = 0.3f};
static const uint16_t data_25098[8] = {1068, 1043, 1069, 1076, 1074, 1072, 1073, 0};
static const uint16_t data_250A8[7] = {4368, 4368, 4368, 4368, 4368, 4368, 4368};
static const char data_250B6[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_250BB[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_250C0[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_250C5[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_250CA[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_250CF[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_250D4[11] __attribute__((scn_string_literal)) = "bgm018.snd";
static const char data_250DF[13] __attribute__((scn_string_literal)) = "f1jutaup.snd";
static const char data_250EC[] __attribute__((scn_string_literal)) = "E1041A168";
static const char data_250F6[] __attribute__((scn_string_literal)) = "E1041F001";
static const char data_25100[] __attribute__((scn_string_literal)) = "E1041A169";
static const char data_2510A[] __attribute__((scn_string_literal)) = "E1041F002";
static const char data_25114[] __attribute__((scn_string_literal)) = "E1041A081";
static const char data_2511E[] __attribute__((scn_string_literal)) = "E1041F008";
static const char data_25128[] __attribute__((scn_string_literal)) = "E1041F004";
static const char data_25132[] __attribute__((scn_string_literal)) = "E1041B023";
static const char data_2513C[] __attribute__((scn_string_literal)) = "E1041B026";
static const char data_25146[] __attribute__((scn_string_literal)) = "E1041B027";
static const char data_25150[] __attribute__((scn_string_literal)) = "E1041B024";
static const char data_2515A[] __attribute__((scn_string_literal)) = "E1041B028";
static const char data_25164[] __attribute__((scn_string_literal)) = "E1041F005";
static const char data_2516E[] __attribute__((scn_string_literal)) = "E1041B060";
static const char data_25178[] __attribute__((scn_string_literal)) = "E1041F006";
static const char data_25182[] __attribute__((scn_string_literal)) = "E1041B061";
static const char data_2518C[] __attribute__((scn_string_literal)) = "E1041A158";
static const char data_25196[] __attribute__((scn_string_literal)) = "E1041F009";
static const char data_251A0[] __attribute__((scn_string_literal)) = "E1041A081";
static const char data_251AA[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_251AF[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_251B4[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_251B9[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_251BE[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_251C3[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_251C8[7] __attribute__((scn_string_literal)) = "SELECT";
static const char data_251CF[13] __attribute__((scn_string_literal)) = "f1jutaup.snd";
static const char data_251DC[19] __attribute__((scn_string_literal)) = "EVSN: Read Failed\012";
static const char data_251EF[6] __attribute__((scn_string_literal)) = "E1041";
static const char data_251F5[13] __attribute__((scn_string_literal)) = "HMOT_CAT.BIN";
static const char data_25202[] __attribute__((scn_string_literal)) = "/scene/01/JU00/";
static const char data_25212[] __attribute__((scn_string_literal)) = "CATA1";
static const Vector4 data_25218 = {.x = 0.0f, .y = 0.3f, .z = 0.3f, .w = 0.3f};
static const char data_25228[4] __attribute__((scn_string_literal)) = "%f";
static const uint32_t data_2522C = 0xFFFF0000;
static const uint8_t pad_25230[4] = {0};
static const uint32_t data_25234 = 0x40008F16;
static const uint32_t data_25238 = 0xC20FF958;
static const uint32_t data_2523C = 0x4084B852;
static const uint32_t data_25240 = 0x424F7E77;
static const uint8_t pad_25244[8] = {0};
static const uint32_t data_2524C = 0xFFFF0000;
static const uint32_t data_25250 = 1;
static const uint32_t data_25254 = 0x400080E4;
static const uint32_t data_25258 = 0xC20EBA44;
static const uint32_t data_2525C = 0x408E5F07;
static const uint32_t data_25260 = 0x425C491D;
static const uint8_t pad_25264[8] = {0};
static const uint32_t data_2526C = 0xFFFF0000;
static const uint32_t data_25270 = 2;
static const uint32_t data_25274 = 0x400082DC;
static const uint32_t data_25278 = 0xC202C659;
static const uint32_t data_2527C = 0x408E9AD4;
static const uint32_t data_25280 = 0x4257547B;
static const uint8_t pad_25284[8] = {0};
static const uint32_t data_2528C = 0xFFFF0000;
static const uint32_t data_25290 = 3;
static const uint32_t data_25294 = 0x40008008;
static const uint32_t data_25298 = 0xC20EB141;
static const uint32_t data_2529C = 0x40E4E979;
static const uint32_t data_252A0 = 0x425C97DC;
static const uint8_t pad_252A4[8] = {0};
static const uint32_t data_252AC = 0xFFFF0000;
static const uint32_t data_252B0 = 4;
static const uint32_t data_252B4 = 0x4000813A;
static const uint32_t data_252B8 = 0xC202C16F;
static const uint32_t data_252BC = 0x40E4E979;
static const uint32_t data_252C0 = 0x42583DF4;
static const uint8_t pad_252C4[8] = {0};
static const uint32_t data_252CC = 0xFFFF0000;
static const uint32_t data_252D0 = 5;
static const uint32_t data_252D4 = 0x40000F3D;
static const uint32_t data_252D8 = 0xC22F2E2F;
static const uint32_t data_252DC = 0x404B27BB;
static const uint32_t data_252E0 = 0x4243E738;
static const uint8_t pad_252E4[8] = {0};
static const uint32_t data_252EC = 0xFFFF0000;
static const uint32_t data_252F0 = 6;
static const uint32_t data_252F4 = 0x40005228;
static const uint32_t data_252F8 = 0xC2409931;
static const uint32_t data_252FC = 0x4056FEC5;
static const uint32_t data_25300 = 0x4257FB99;
static const uint8_t pad_25304[8] = {0};
static const uint32_t data_2530C = 0xFFFF0000;
static const uint32_t data_25310 = 8;
static const uint32_t data_25314 = 0x8000071C;
static const uint32_t data_25318 = 0xC23311B7;
static const uint32_t data_2531C = 0x40066666;
static const uint32_t data_25320 = 0x4283AF4F;
static const uint8_t pad_25324[8] = {0};
static const uint32_t data_2532C = 0xFFFF0000;
static const uint32_t data_25330 = 9;
static const uint32_t data_25334 = 0x8000871C;
static const uint32_t data_25338 = 0xC23311B7;
static const uint32_t data_2533C = 0x40066666;
static const uint32_t data_25340 = 0x4283AF4F;
static const uint8_t pad_25344[8] = {0};
static const uint32_t data_2534C = 0xFFFF0000;
static const uint32_t data_25350 = 7;
static const uint32_t data_25354 = 0x4000207E;
static const uint32_t data_25358 = 0xC242A4DD;
static const uint32_t data_2535C = 0x405EB9F5;
static const uint32_t data_25360 = 0x42805D98;
static const uint8_t pad_25364[8] = {0};
static const uint32_t data_2536C = 0xFFFF0000;
static const uint32_t data_25370 = 10;
static const uint32_t data_25374 = 0x4000F178;
static const uint32_t data_25378 = 0xC1DCDABA;
static const uint32_t data_2537C = 0x40269C78;
static const uint32_t data_25380 = 0x42823D49;
static const uint8_t pad_25384[8] = {0};
static const uint32_t data_2538C = 0xFFFF0000;
static const uint32_t data_25390 = 11;
static const uint32_t data_25394 = 0x4000A098;
static const uint32_t data_25398 = 0xC1C852F2;
static const uint32_t data_2539C = 0x401B0A3D;
static const uint32_t data_253A0 = 0x4292FD56;
static const uint8_t pad_253A4[8] = {0};
static const uint32_t data_253AC = 0xFFFF0000;
static const uint32_t data_253B0 = 12;
static const uint32_t data_253B4 = 0x4000871C;
static const uint32_t data_253B8 = 0xC1C7F247;
static const uint32_t data_253BC = 0x3FB33333;
static const uint32_t data_253C0 = 0x4296D439;
static const uint8_t pad_253C4[8] = {0};
static const uint32_t data_253CC = 0xFFFF0000;
static const uint32_t data_253D0 = 13;
static const uint32_t data_253D4 = 0x40002CB6;
static const uint32_t data_253D8 = 0xC213AE14;
static const uint32_t data_253DC = 0x408EBFB1;
static const uint32_t data_253E0 = 0x421E0189;
static const uint8_t pad_253E4[8] = {0};
static const uint32_t data_253EC = 0xFFFF0000;
static const uint32_t data_253F0 = 14;
static const uint32_t data_253F4 = 0x400000DC;
static const uint32_t data_253F8 = 0xC21E8ADB;
static const uint32_t data_253FC = 0x408EBFB1;
static const uint32_t data_25400 = 0x4224860B;
static const uint8_t pad_25404[8] = {0};
static const uint32_t data_2540C = 0xFFFF0000;
static const uint32_t data_25410 = 15;
static const uint32_t data_25414 = 0x4000157B;
static const uint32_t data_25418 = 0xC21C8BE1;
static const uint32_t data_2541C = 0x4073E282;
static const uint32_t data_25420 = 0x422AA3BD;
static const uint8_t pad_25424[8] = {0};
static const uint32_t data_2542C = 0xFFFF0000;
static const uint32_t data_25430 = 16;
static const uint32_t data_25434 = 0x4000157B;
static const uint32_t data_25438 = 0xC2200396;
static const uint32_t data_2543C = 0x40ACCCCD;
static const uint32_t data_25440 = 0x42147838;
static const uint8_t pad_25444[8] = {0};
static const uint32_t data_2544C = 257;
static const uint32_t data_25450 = SHEN_NAME('CATM');
static const uint8_t pad_25454[28] = {0};
static const uint32_t data_25470 = SHEN_NAME('RAKK');
static const uint8_t pad_25474[24] = {0};
static const uint8_t data_2548C[2] = {
  0xFF, 0xFF
};
static const char data_2548E[11] __attribute__((scn_string_literal)) = "bgm109.snd";
static const char data_25499[7] __attribute__((scn_string_literal)) = "SELECT";
static const FogColourData data_254A0 = {.red = 20.0f, .green = 800.0f, .blue = 0.63f, .colour = 0xFF0F0004U};
static const uint32_t data_254B0[4] = {0x805F10A0U, 0x805F10A0U, 0, 0};
static const Vector3 data_254C0 = {.x = -31.1651f, .y = 5.086f, .z = 47.664902f};
static const Vector3 data_254CC = {.x = -39.887001f, .y = 5.096f, .z = 65.281998f};
static const Vector3i data_254D8 = {.x = 65353, .y = 37501, .z = 0};
static const Vector3i data_254E4 = {.x = 65353, .y = 24393, .z = 0};
static const char data_254F0[16] __attribute__((scn_string_literal)) = "/scene/01/JU00/";
static const char data_25500[5] __attribute__((scn_string_literal)) = "MAPZ";
static const char data_25505[16] __attribute__((scn_string_literal)) = "/scene/01/JU00/";
static const char data_25515[5] __attribute__((scn_string_literal)) = "MAPN";
static const char data_2551A[16] __attribute__((scn_string_literal)) = "/scene/01/JU00/";
static const char data_2552A[5] __attribute__((scn_string_literal)) = "MAPS";
static const char data_2552F[16] __attribute__((scn_string_literal)) = "/scene/01/JU00/";
static const char data_2553F[9] __attribute__((scn_string_literal)) = "SHUKI";
static const uint32_t data_25548 = 0xC247B958;
static const float data_2554C = 7.0f;
static const uint32_t data_25550 = 0x42C403FEU;
static const int32_t data_25554[3] = {22499, 33024, 3};
static const uint32_t data_25560 = 0xC249BF48U;
static const float data_25564 = 7.6498f;
static const uint32_t data_25568 = 0x42C4A3B0U;
static const int32_t data_2556C[3] = {55267, 33032, 3};
static const float data_25578[2] = {-29.5634f, 2.27f};
static const uint32_t data_25580 = 0x428D201AU;
static const int32_t data_25584[3] = {16748, 256, 4};
static const uint32_t data_25590 = 0xC1F3A95FU;
static const float data_25594 = 2.95f;
static const uint32_t data_25598 = 0x428D204FU;
static const int32_t data_2559C[3] = {49516, 264, 4};
static const uint32_t data_255A8 = 0xC1E88E22U;
static const float data_255AC = 1.15f;
static const uint32_t data_255B0 = 0x428679C1U;
static const int32_t data_255B4[3] = {35498, 256, 5};
static const float data_255C0[2] = {-28.8409f, 2.05f};
static const uint32_t data_255C8 = 0x4288D6FDU;
static const int32_t data_255CC[3] = {2730, 264, 5};
static const uint32_t data_255D8 = 0xC20E57C2U;
static const float data_255DC = 2.1f;
static const uint32_t data_255E0 = 0x42328A58U;
static const int32_t data_255E4[3] = {5279, 8448, 3};
static const float data_255F0[2] = {-35.8936f, 2.6f};
static const uint32_t data_255F8 = 0x423093DEU;
static const int32_t data_255FC[3] = {38065, 8456, 3};
static const uint32_t data_25608 = 0xC2148B44U;
static const float data_2560C = 2.135f;
static const uint32_t data_25610 = 0x4250B382U;
static const int32_t data_25614[3] = {36044, 256, 5};
static const uint32_t data_25620 = 0xC2130A23U;
static const float data_25624[2] = {3.0506f, 53.3214f};
static const int32_t data_2562C[3] = {3276, 264, 5};
static const float data_25638[3] = {-37.6507f, 2.7961f, 41.2066f};
static const int32_t data_25644[3] = {5461, 8194, 2};
static const uint32_t data_25650 = 0xC217B368U;
static const float data_25654 = 3.242f;
static const uint32_t data_25658 = 0x4222D7A8U;
static const int32_t data_2565C[3] = {38229, 8202, 3};
static const uint32_t data_25668 = 0xC219AD91U;
static const float data_2566C = 3.4781f;
static const uint32_t data_25670 = 0x421F6E2FU;
static const int32_t data_25674[3] = {5461, 40962, 3};
static const uint32_t data_25680 = 0xC21B229CU;
static const float data_25684 = 3.9411f;
static const uint32_t data_25688 = 0x421CEC3DU;
static const int32_t data_2568C[3] = {38229, 24586, 3};
static const uint32_t data_25698 = 0xC21B4E70U;
static const float data_2569C = 3.24f;
static const uint32_t data_256A0 = 0x426C8A3DU;
static const int32_t data_256A4[3] = {364, 16384, 12};
static const float data_256B0[2] = {-38.9445f, 5.71f};
static const uint32_t data_256B8 = 0x425F53A9U;
static const int32_t data_256BC[3] = {33241, 16392, 12};
static const float data_256C8 = 100000.0f;
static const uint8_t data_256CC[8] = {0};
typedef struct Row_256D4 { float w0; int32_t w1; int32_t w2; } Row_256D4;
static const Row_256D4 data_256D4[9] = {
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
static const float data_25740 = 0.3f;
static const int32_t data_25744 = 3;
static const char data_25748[] __attribute__((scn_string_literal)) = "/scene/01/JU00/";
static const char data_25758[] __attribute__((scn_string_literal)) = "JQSO";
static const char data_2575D[7] __attribute__((scn_string_literal)) = "E1014A";
static const char data_25764[13] __attribute__((scn_string_literal)) = "e1sbaqte.snd";
static const char data_25771[] __attribute__((scn_string_literal)) = "/scene/01/JU00/";
static const char data_25781[] __attribute__((scn_string_literal)) = "RAKK";
static const char data_25786[] __attribute__((scn_string_literal)) = "/scene/01/JU00/";
static const char data_25796[] __attribute__((scn_string_literal)) = "RAKK";
static const char data_2579B[] __attribute__((scn_string_literal)) = "/scene/01/JU00/";
static const char data_257AB[] __attribute__((scn_string_literal)) = "JYUU";
static const char data_257B0[5] __attribute__((scn_string_literal)) = "JYUU";
static const char data_257B5[1] __attribute__((scn_string_literal)) = "";
static const char data_257B6[5] __attribute__((scn_string_literal)) = "JYUU";
static const char data_257BB[1] __attribute__((scn_string_literal)) = "";
static const char data_257BC[11] __attribute__((scn_string_literal)) = "bgm109.snd";
static const char data_257C7[7] __attribute__((scn_string_literal)) = "SELECT";
static const char data_257CE[6] __attribute__((scn_string_literal)) = "JYUU";
static const uint32_t data_257D4 = 0xC182147B;
static const float data_257D8[139] = {
  74.84f, -20.47f, 79.12f, -26.0f, 73.74f, -21.79f, 69.46f, 0.91f, 3.66f, -23.66f, 1.91f, 73.72f, 1.64f, -19.91f, 68.24f, -25.88f, 69.79f, -26.83f,
  65.72f, -20.86f, 64.17f, 0.91f, 4.43f, -24.75f, 1.91f, 67.39f, 1.64f, -26.7f, 64.43f, -27.13f, 66.01f, -30.98f, 64.91f, -30.5f, 63.35f, 0.91f,
  4.43f, -28.84f, 1.91f, 64.67f, 1.0f, -45.96f, 62.26f, -50.25f, 66.46f, -55.64f, 60.95f, -51.35f, 56.74f, 1.92f, 4.73f, -48.29f, 2.92f, 62.23f,
  2.04f, -47.14f, 54.41f, -49.22f, 55.39f, -50.88f, 51.75f, -48.79f, 50.77f, 1.92f, 5.54f, -48.72f, 2.92f, 52.82f, 1.0f, -40.75f, 48.1f, -44.92f,
  49.17f, -45.55f, 46.72f, -41.39f, 45.65f, 1.92f, 4.63f, -43.04f, 2.92f, 47.87f, 1.0f, -35.45f, 38.22f, -33.96f, 37.84f, -33.39f, 40.06f, -34.84f,
  40.47f, 2.56f, 5.45f, -34.99f, 3.6f, 39.38f, 1.1f, -40.2f, 53.86f, -31.11f, 51.78f, -30.97f, 58.22f, -39.4f, 59.48f, 3.04f, 9.51f, -36.32f, 5.04f,
  55.97f, 3.34f, -53.14f, 97.59f, -50.87f, 101.14f, -53.4f, 102.75f, -55.65f, 99.2f, 7.75f, 11.47f, -52.0f, 9.25f, 99.36f, 1.6f, -40.13f, 38.61f,
  -41.11f, 36.89f, -39.64f, 36.07f, -38.66f, 37.8f, 3.95f, 7.15f, -39.37f, 5.5f, 38.19f, 1.6f
};
static FaceLipCue data_25A04 = {.viseme = 5, .field02 = 2, .duration = 31, .field06 = 0};
static const int32_t data_25A0C[3] = {131072, 1, 65535};
static const uint32_t data_25A18 = 0xFA00F8;
static const int32_t data_25A1C[3] = {16187637, 16449782, 249};
static const uint32_t data_25A28 = 327681;
static const uint32_t data_25A2C[2] = {0xC20E182BU, 0x42884539U};
static const float data_25A34 = 1.0f;
static const int32_t data_25A38 = 327682;
static const float data_25A3C = -37.6236f;
static const uint32_t data_25A40 = 0x4282AB9FU;
static const float data_25A44 = 1.0f;
static const int32_t data_25A48 = 327683;
static const float data_25A4C[3] = {-45.4235f, 56.1353f, 1.0f};
static const int32_t data_25A58 = 327684;
static const uint32_t data_25A5C[2] = {0xC2367E77U, 0x42568AA6U};
static const float data_25A64 = 1.0f;
static const int32_t data_25A68 = 327685;
static const uint32_t data_25A6C = 0xC2321810U;
static const float data_25A70[2] = {52.5354f, 0.1f};
static const int32_t data_25A78 = 196608;
static const char data_25A7C[5] __attribute__((scn_string_literal)) = "JQSO";
static const char data_25A81[5] __attribute__((scn_string_literal)) = "JQSO";
static const char data_25A86[] __attribute__((scn_string_literal)) = "DAT0";
static const char data_25A8B[5] __attribute__((scn_string_literal)) = "JQSO";
static const char data_25A90[5] __attribute__((scn_string_literal)) = "JQSO";
static const char data_25A95[] __attribute__((scn_string_literal)) = "DAT0";
static const char data_25A9A[5] __attribute__((scn_string_literal)) = "JQSO";
static const char data_25A9F[5] __attribute__((scn_string_literal)) = "JQSO";
static const char data_25AA4[] __attribute__((scn_string_literal)) = "DAT0";
static const char data_25AA9[5] __attribute__((scn_string_literal)) = "JQSO";
static const char data_25AAE[5] __attribute__((scn_string_literal)) = "JQSO";
static const char data_25AB3[] __attribute__((scn_string_literal)) = "DAT1";
static const char data_25AB8[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_25ABD[5] __attribute__((scn_string_literal)) = "JQSO";
static const char data_25AC2[11] __attribute__((scn_string_literal)) = "E1014AA002";
static const char data_25ACD[11] __attribute__((scn_string_literal)) = "E1014AA003";
static const char data_25AD8[11] __attribute__((scn_string_literal)) = "E1014AA004";
static const char data_25AE3[11] __attribute__((scn_string_literal)) = "E1014AA008";
static const char data_25AEE[11] __attribute__((scn_string_literal)) = "E1014AB001";
static const char data_25AF9[11] __attribute__((scn_string_literal)) = "E1014AB002";
static const char data_25B04[11] __attribute__((scn_string_literal)) = "E1014AB003";
static const char data_25B0F[11] __attribute__((scn_string_literal)) = "E1014AB004";
static const char data_25B1A[11] __attribute__((scn_string_literal)) = "E1014AB003";
static const char data_25B25[11] __attribute__((scn_string_literal)) = "E1014AC001";
static const char data_25B30[11] __attribute__((scn_string_literal)) = "E1014AC002";
static const char data_25B3B[11] __attribute__((scn_string_literal)) = "E1014AC015";
static const char data_25B46[11] __attribute__((scn_string_literal)) = "E1014AC016";
static const char data_25B51[11] __attribute__((scn_string_literal)) = "E1014AC009";
static const char data_25B5C[11] __attribute__((scn_string_literal)) = "E1014AC014";
static const char data_25B67[11] __attribute__((scn_string_literal)) = "E1014AD002";
static const char data_25B72[14] __attribute__((scn_string_literal)) = "E1014AD006";
static const uint32_t data_25B80 = 380;
static const int32_t data_25B84[6] = {1, 390, 0, 1018, 0, -1};
static const uint32_t data_25B9C = 510;
static const int32_t data_25BA0[6] = {1, 525, 0, 1020, 0, -1};
static const uint32_t data_25BB8 = 150;
static const int32_t data_25BBC[6] = {1, 160, 0, 1019, 0, -1};
static const uint32_t data_25BD4 = 530;
static const int32_t data_25BD8[6] = {1, 970, 0, 1014, 0, -1};
static const uint32_t data_25BF0 = 911;
static const int32_t data_25BF4[6] = {1, 155, 0, 1015, 0, -1};
static const char data_25C0C[7] __attribute__((scn_string_literal)) = "FREE02";
static const char data_25C13[7] __attribute__((scn_string_literal)) = "FREE03";
static const char data_25C1A[7] __attribute__((scn_string_literal)) = "FREE04";
static const char data_25C21[7] __attribute__((scn_string_literal)) = "FREE01";
static const char data_25C28[11] __attribute__((scn_string_literal)) = "M_MOBJ.BIN";
static const char data_25C33[14] __attribute__((scn_string_literal)) = "scene/01/D000";
static const char data_25C41[] __attribute__((scn_string_literal)) = "SCRTEST.BIN";
static const uint8_t data_25C4D[3] = {0};
