#pragma once

#include "scenario.h"
#include "motion/motion.h"
#include "scene/02/d000/m_tpin.h"

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
static void script_2657(void);
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
static void script_387C(void);
static void script_3885(void);
static void script_3894(int32_t arg0);
static void script_3921(int32_t arg0);
static void script_3971(void);
static void script_399F(int32_t arg0);
static void script_39CD(void);
static void script_3A01(int32_t arg0);
static void script_3A35(int32_t arg0);
static void script_3A53(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_3A8D(int32_t arg0, int32_t arg1);
static void script_3AC1(void);
static void script_3BB9(int32_t arg0, int32_t arg1);
static void script_3BE9(int32_t arg0);
static int32_t script_3C15(int32_t arg0, int32_t arg1);
static int8_t script_3C48(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_3D8A(int32_t arg0, int32_t arg1);
static int8_t script_3F53(int32_t arg0, int32_t arg1);
static int32_t script_4129(int32_t arg0, const char *arg1);
static void script_419E(const char *arg0);
static int32_t script_41BF(int32_t arg0, const char *arg1, int32_t arg2);
static int32_t script_4204(int32_t arg0, int32_t arg1);
static void script_4232(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_4246(int32_t arg0, int32_t arg1);
static int32_t script_4258(const char *arg0, int32_t arg1);
static void script_4289(int32_t arg0, int32_t arg1);
static int32_t script_429B(int32_t arg0, int32_t arg1, int32_t arg2, const Vector3 *arg3, const Vector3i *arg4, int32_t arg5);
static void script_42CE(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static int32_t script_42E8(int32_t arg0, int32_t arg1);
static void script_4310(int32_t arg0, int32_t arg1);
static void script_4322(int32_t arg0, int32_t arg1);
static int32_t script_4360(int32_t arg0, int32_t arg1, int32_t arg2, float arg3, int32_t arg4, int32_t arg5);
static int32_t script_43D1(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4);
static void script_4433(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4);
static void script_444B(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static int32_t script_4465(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_44A3(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_44B9(int32_t arg0, int32_t arg1, const Vector3i *arg2, int32_t arg3);
static void script_44EA(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_4500(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4);
static void script_4531(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4);
static int32_t script_4549(int32_t arg0, int32_t arg1, const Vector3 *arg2, int32_t arg3);
static void script_4596(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_45AC(int32_t arg0, int32_t arg1, const Vector3i *arg2, int32_t arg3);
static void script_45FB(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_4611(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_463F(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_4655(int32_t arg0);
static void script_4694(void);
static void script_46C0(int32_t arg0, int32_t arg1, int32_t arg2);
static int32_t script_47E6(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_4858(int32_t arg0, int32_t arg1, int32_t arg2);
static int32_t script_486C(int32_t arg0);
static int32_t script_4892(int32_t arg0);
static int32_t script_48B0(int32_t arg0, int32_t arg1);
static void script_48DE(int32_t arg0, int32_t arg1);
static void script_48F0(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_4A21(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_4A53(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_4B22(void);
static void script_4B5C(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_4CB9(int32_t arg0);
static void script_4D94(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_4E35(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_4F3E(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4);
static void script_5282(int32_t arg0, int32_t arg1);
static void script_5350(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_5641(int32_t arg0, int32_t arg1, int32_t arg2);
static int32_t script_592B(int32_t arg0);
static void script_5D8F(int32_t arg0);
static void script_5DAA(void);
static void script_5DBB(void);
static void script_5DC2(void);
static void script_5DCF(void);
static void script_5FF3(void);
static void script_625F(void);
static void script_64EF(void);
static void script_675B(int32_t arg0, int32_t arg1);
static void script_677C(void);
static void script_67A2(void);
static void script_67B4(void);
static void script_67DA(int32_t arg0);
static void script_680E(void);
static void script_6899(int32_t arg0);
static void script_6C58(void);
static void script_6CAC(void);
static void script_6DF3(int32_t arg0);
static void script_6E0E(int32_t arg0);
static void script_6E5F(int32_t arg0);
static void script_6EDC(void);
static void script_6EF1(void);
static void script_6EFA(void);
static void script_6EFD(int32_t arg0);
static void script_6F3D(int32_t arg0);
static int32_t script_6FAA(void);
static int32_t script_713F(void);
static int32_t script_7174(void);
static int32_t script_7205(void);
static int32_t script_72B2(void);
static int32_t script_7339(void);
static void script_741A(int32_t arg0, int32_t arg1);
static int32_t script_7489(int32_t arg0, int32_t arg1, float arg2);
static int32_t script_74F2(int32_t arg0, int32_t arg1, float arg2);
static void script_7564(int32_t arg0);
static void script_75C2(int32_t arg0);
static void script_7620(int32_t arg0);
static int32_t script_767E(int32_t arg0, int32_t arg1, float arg2, float arg3);
static void script_76F7(int32_t arg0, float arg1, float arg2, float arg3);
static void script_771F(int32_t arg0, float arg1, float arg2, float arg3);
static void script_7773(int32_t arg0, int32_t arg1, int32_t arg2);
static int32_t script_77C3(int32_t arg0);
static void script_786A(int32_t arg0, int32_t arg1);
static int32_t script_788D(int32_t arg0);
static void script_78C0(int32_t arg0, int32_t arg1);
static void script_78E3(int32_t arg0, int32_t arg1);
static void script_7942(int32_t arg0);
static void script_7957(void);
static void script_7966(void);
static void script_7975(void);
static void script_7978(void);
static void script_797B(void);
static void script_797E(void);
static void script_7981(void);
static int32_t script_798E(void);
static void script_799A(void);
static int32_t script_79A9(void);
static int32_t script_79B8(int32_t arg0);
static void script_79D6(float arg0);
static int32_t script_79F5(int32_t arg0, int32_t arg1);
static void script_7A6D(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4);
static int32_t script_801F(int32_t arg0);
static void script_80F0(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_8100(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_8204(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_83A6(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_8507(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_8668(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_8906(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_89D4(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_8A3B(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_8AA2(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_8AE9(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_8C1F(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_94E0(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_95FE(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_9CC3(int32_t arg0, int32_t arg1);
static void script_9D99(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_ACAC(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_AD18(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_B1D2(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_B88C(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_D16C(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_D88D(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_E017(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_E0EF(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_E566(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_F4F9(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_F943(int32_t arg0, DSWKDoorEntry *arg1, int32_t *arg2, DoorResourceDescriptor *arg3, MapObjectRecord *arg4, int32_t arg5, int32_t arg6);
static void script_FACF(DSWKDoorEntry *arg0, int32_t *arg1, DoorResourceDescriptor *arg2, MapObjectRecord *arg3, int32_t arg4);
static void script_FB03(void);
static void script_FB6C(int32_t arg0, int32_t arg1);
static void script_FC09(int32_t arg0, int32_t arg1);
static void script_FC43(void);
static void script_FCA6(void);
static void script_FD09(int32_t arg0);
static void script_FD40(int32_t arg0);
static void script_FD77(int32_t arg0);
static void script_FFB9(int32_t arg0);
static void script_10002(int32_t arg0);
static void script_1004B(int32_t arg0, int32_t arg1);
static void script_100C1(int32_t arg0);
static void script_100CF(int32_t arg0);
static void script_100DD(int32_t arg0);
static void script_101A4(void);
static void script_101B0(void);
static void script_101BC(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5, int32_t arg6);
static int32_t script_101FF(void);
static int8_t script_10310(int32_t arg0, int32_t arg1, int8_t arg2);
static void script_103A8(int32_t arg0);
static void script_10577(int32_t arg0);
static int32_t script_105A8(int32_t arg0, int32_t arg1, int32_t arg2, float arg3, float arg4, int32_t arg5, int32_t arg6);
static void script_10C71(int32_t arg0, int32_t arg1);
static void script_10CA3(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_10E13(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_10FD0(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_11184(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_1139A(int32_t arg0, int32_t arg1);
static void script_11464(int32_t arg0, int32_t arg1);
static void script_114C5(int32_t arg0);
static void script_1166A(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_11702(int32_t arg0, int32_t arg1);
static void script_117E4(int32_t arg0);
static void script_1183B(int32_t arg0, int32_t arg1);
static void script_1189A(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static int32_t script_118C1(float arg0, float arg1, float arg2, float arg3);
static int32_t script_11A5F(void);
static void script_11B26(const char *arg0, int32_t arg1);
static void script_11B73(void);
static int32_t script_11CAA(int32_t arg0);
static void script_11D23(void);
static int8_t script_11D85(void);
static void script_11DD0(int32_t arg0);
static void script_11E39(int32_t arg0);
static void script_11E64(void);
static int32_t script_11E8B(int32_t arg0);
static int8_t script_11ECD(int32_t arg0);
static void script_11FA5(void);
static void script_11FB2(void);
static void script_12487(void);
static void script_12FCE(void);
static void script_1308C(void);
static void script_130B1(void);
static void script_137F8(void);
static int8_t script_1587F(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int8_t script_15938(void);
static void script_15AEE(int32_t arg0);
static int8_t script_16C5F(void);
static void script_171AE(int32_t arg0);
static void script_1B06C(void);
static int8_t script_1BB34(void);
static int8_t script_1BB5C(void);
static void script_1BC77(int32_t arg0);
static int32_t script_1BD60(int32_t arg0, int32_t arg1);
static int32_t script_1BDC9(void);
static void script_1BDE4(int32_t arg0);
static void script_1BECD(int32_t arg0);
static void script_1C0BB(void);
static void script_1C0F2(int32_t arg0);
static int8_t script_1C1BA(void);
static void script_1CE89(void);
static void script_1D00F(void);
static void script_1D023(void);
static void script_1D06D(void);
static int32_t script_1D1B2(void);
static int32_t script_1D235(void);
static void script_1D961(void);
static void script_1D9D4(void);
static void script_1DA34(void);
static void script_1DAD2(void);
static void script_1DB6E(int32_t arg0);
static int16_t script_1DD0F(int8_t arg0, int8_t arg1);
static void script_1DEFA(int32_t arg0, int32_t arg1);
static int32_t script_1DF49(int32_t arg0, int32_t arg1);
static void script_1DF9C(int32_t arg0, int32_t arg1, int32_t arg2, float arg3);
static void script_1E1D3(void);
static int32_t script_1E33F(int32_t arg0);
static int32_t script_1E3A9(int32_t arg0);
static int32_t script_1E5DA(void);
static int32_t script_1E68A(void);
static int32_t script_1E72D(void);
static void script_1E7BF(void);
static void script_1E7EC(int32_t arg0);
static void script_1E8BE(void);
static void script_1EA65(void);
static void script_1EB3F(void);
static void script_1EC03(void);
static void script_1EC32(int32_t arg0);
static void script_1EDBB(int32_t arg0, int32_t arg1);
static int32_t script_1EE3C(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4);
static int32_t script_1EEF6(void);
static int32_t script_1F031(float arg0);
static int32_t script_1F101(void);
static int32_t script_1F1CA(void);
static int32_t script_1F274(void);
static int32_t script_1F2BE(void);
static void script_1F3B9(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_1F5AA(void);
static int32_t script_1F947(void);
static int32_t script_1FA39(int32_t arg0);
static int32_t script_1FAB9(int32_t arg0);
static void script_1FBEB(int32_t arg0);
static int32_t script_1FE57(void);
static void script_1FF2F(void);
static int32_t script_1FF32(int32_t arg0);
static int32_t script_20048(int16_t arg0);
static int32_t script_20171(void);
static void script_2019A(void);
static int32_t script_20204(void);
static int32_t script_202C4(void);
static int32_t script_205E3(void);
static void script_206C0(void);
static int32_t script_206D8(void);
static int32_t script_206EB(void);
static void script_207C6(void);
static int32_t script_208A1(int32_t arg0, int32_t arg1);
static void script_20A5A(int32_t arg0);
static void script_20AED(int32_t arg0, int32_t arg1);
static void script_20BA3(void);
static void script_20C1D(int32_t arg0);
static int32_t script_20D33(void);
static int32_t script_21275(int32_t arg0, const char *arg1, const char *arg2, ...);
static void script_212C7(int32_t arg0, Vector3 *arg1);
static void script_212FF(int32_t arg0, Vector3i *arg1);
static void script_21337(void);
static void script_21378(void);
static void script_213D8(void);
static void script_21483(void);
static void script_214D4(void);
static int32_t script_218F9(int32_t arg0, int32_t arg1);
static void script_21A42(int32_t arg0, int32_t arg1);
static int32_t script_21AD5(void);
static void script_21E51(void);
static int32_t script_21EF4(float arg0, float arg1);
static int32_t script_2202B(void);
static void script_2217C(int32_t arg0);
static void script_221D5(int32_t arg0);
static void script_2239E(void);
static void script_223A1(void);
static void script_22426(void);
static void script_224C4(void);
static void script_22519(void);
static int8_t script_225B9(void);
static void script_22600(void);
static void script_22668(void);
static void script_22674(void);
static void script_22680(int32_t arg0);
static void script_227B2(int32_t arg0, int32_t arg1);
static void script_228ED(void);
static void script_229A5(void);
static int32_t script_22A15(int32_t arg0);
static int32_t script_22CA2(int32_t arg0);
static int32_t script_22CF2(void);
static void script_22D0F(void);
static void script_22DB3(void);
static void script_22F16(void);
static void script_22FCD(void);
static void script_232BE(int32_t arg0);
static void script_2367A(int32_t arg0, int32_t arg1);
static void script_23721(int32_t arg0, int32_t arg1);
static void script_23785(int32_t arg0, int32_t arg1);
static void script_2382C(int32_t arg0, int32_t arg1);
static int32_t script_23890(int32_t arg0, int32_t arg1);
static int32_t script_23901(int32_t arg0, int32_t arg1);
static void script_23972(int32_t arg0, int32_t arg1);
static void script_23AB3(int32_t arg0, int32_t arg1);
static int32_t script_23B4E(int32_t arg0);
static int32_t script_23BA1(int32_t arg0);
static void script_23BFC(int32_t arg0);
static void script_23C7A(int32_t arg0);
static void script_23CDF(int32_t arg0, int32_t arg1);
static void script_23CFF(float arg0, float arg1, float arg2, int32_t arg3);
static void script_23D59(float arg0, float arg1, float arg2, int32_t arg3, int32_t arg4);
static void script_23DB2(int32_t arg0);
static void script_23E1D(int32_t arg0);
static void script_23E88(void);
static void script_23EB0(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_23F18(const char *arg0, int32_t arg1);
static int32_t script_23F44(int32_t arg0, int32_t arg1);
static void script_23F74(int32_t arg0);
static void script_24154(int32_t arg0);
static void script_24280(int32_t arg0);
static void script_24472(int32_t arg0);
static void script_246B8(void);
static int32_t script_24817(void);
static void script_2488F(void);
static void script_248B7(void);
static void script_248D6(int32_t arg0);
static int8_t script_2496C(int8_t arg0);
static int8_t script_24ADD(int32_t arg0);
static int8_t script_24D46(int32_t arg0);
static int8_t script_24E35(int32_t arg0);
static int8_t script_24FF1(void);
static int8_t script_2500B(void);
static int8_t script_250DA(void);
static void script_25175(int32_t arg0);
static int32_t script_25873(int32_t arg0, int32_t arg1);
static int32_t script_25885(int32_t arg0, int32_t arg1);
static void script_25ABF(void);
static int32_t script_25AE5(void);
static void script_25D20(void);
static void script_25D23(int32_t arg0);
static void script_261A4(int32_t arg0);
static void script_26399(void);
static void script_2645C(void);
static int32_t script_2646B(void);
static void script_264B8(void);
static int32_t script_26803(int32_t arg0);
static void script_269BD(void);
static void script_26A31(void);
static void script_26ADF(void);
static void script_26BA2(void);
static void script_26BC9(void);
static void script_26BF0(void);
static void script_26BFD(void);
static void script_26C05(void);
static void script_26C60(void);
static void script_26C84(void);
static void script_26CB1(void);
static void script_26F95(void);
static int32_t script_270FB(void);
static void script_272A4(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4);
static int32_t script_27317(int32_t arg0);
static void script_2770E(void);
static int8_t script_2773C(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_277BB(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_27938(void);
static void script_27B54(void);
static int32_t script_27B7D(void);
static void script_27BDC(void);
static void script_27D14(int32_t arg0);
static void script_27D20(int32_t arg0);
static void script_27D2C(int32_t arg0);
static void script_27E38(int32_t arg0);
static void script_27E4F(int32_t arg0);
static void script_27E90(int32_t arg0);
static void script_27ED4(int32_t arg0);
static void script_27F52(void);
static int32_t script_27FD1(int32_t arg0);
static int32_t script_28057(void);
static int32_t script_2817A(void);
static void script_28201(void);
static int32_t script_2825A(void);
static int32_t script_28294(int32_t arg0);
static void script_282B7(void);
static int32_t script_282D7(int32_t arg0);
static int32_t script_282FA(void);
static void script_28345(void);
static void script_2838C(void);
static void script_283DF(void);
static void script_2843B(void);
static void script_28482(void);
static void script_284DE(int8_t arg0);
static void script_285B4(void);
static void script_2862B(void);
static void script_28668(void);
static int32_t script_286A5(int32_t arg0);
static void script_28716(int32_t arg0);
static int32_t script_28979(int32_t arg0);
static void script_289C1(void);
static void script_289C9(void);
static void script_289DD(int8_t arg0);
static void script_28D11(void);
static void script_28D79(void);
static void script_28DDE(void);
static void script_28DE6(void);
static void script_28DE9(void);
static void script_28DF7(void);
static void script_28DFA(void);
static void script_28DFD(int32_t arg0);
static void script_28E2E(int32_t arg0);
static void script_28E5F(int32_t arg0);
static void script_28ECC(int32_t arg0);
static void script_28EFD(void);
static void script_28F50(void);
static int32_t script_28FA3(void);
static int32_t script_2919E(void);
static void script_292D5(void);
static void script_29324(void);
static void script_2934A(int32_t arg0);
static void script_293F0(int32_t arg0);
static void script_29410(int32_t arg0);
static void script_29456(void);
static int32_t script_29493(void);
static void script_296C8(void);
static void script_29708(void);
static void script_29723(int32_t arg0);
static void script_297DC(int32_t arg0);
static void script_29805(int32_t arg0);
static void script_29828(int32_t arg0);
static void script_2986E(void);
static void script_29CFA(void);
static void script_29D3D(void);
static void script_29E86(void);
static void script_29E95(void);
void script_29EA9(void);
static void script_29EB1(int32_t arg0);
static void script_29ECF(float arg0);
static void script_29EEE(int32_t arg0);
static void script_29F03(int32_t arg0, int32_t arg1);
static int32_t script_29FB9(int32_t arg0);
static void script_29FD1(int32_t arg0);
static void script_29FE7(int32_t arg0);
static void script_29FF5(void);
static void script_2A0BF(void);
static void script_2A0C2(void);
static void script_2A210(int32_t arg0);
static int32_t script_2A224(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_2A246(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_2A28C(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_2A2A0(int32_t arg0);
static void script_2A2BA(void);
static void script_2A2D5(int32_t arg0);
static void script_2A302(int32_t arg0, int32_t arg1);
static void script_2A329(void);
static void script_2A333(void);
static void script_2A34E(void);
static void script_2A358(int32_t arg0);
static int32_t script_2A3BC(float arg0, float arg1, float arg2, float arg3);
static int32_t script_2A3E3(int32_t arg0, int32_t arg1);
static int32_t script_2A45B(int32_t arg0, int32_t arg1, int32_t arg2, const Vector3 *arg3);
static void script_2A507(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_2A6CD(int32_t arg0);
static void script_2A6EF(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_2A7CA(void);
static void script_2A7EE(int32_t arg0);
static void script_2A8AE(void);
static void script_2A8B4(void);
static void script_2A8BF(int32_t arg0);
static void script_2A99A(int32_t arg0, int32_t arg1);
static void script_2A9FF(int32_t arg0, int32_t arg1);
static void script_2AA64(void);
static void script_2AAA0(void);
static void script_2AABB(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_2AB2F(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_2AB54(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_2AB9D(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_2AC03(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_2ACCB(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_2AD21(void);
static void script_2AD76(int32_t arg0);
static void script_2ADD0(const char *arg0, const char *arg1);
static void script_2AE20(int32_t arg0);
static void script_2AE45(int32_t arg0);
static int32_t script_2AE6A(int32_t arg0, int32_t arg1);
static void script_2AE8F(void);
static void script_2AE9B(void);
static void script_2AE9E(void);
static void script_2AEAA(void);
static void script_2AEB6(void);
static void script_2AEC2(void);
static int32_t script_2AECE(void);
static int32_t script_2AEDA(void);
static void script_2AEE6(void);
static int32_t script_2AEF2(void);
static int32_t script_2AEFE(void);
static void script_2AF0A(void);
static int32_t script_2AF16(void);
static int32_t script_2AF22(void);
static void script_2AF2E(void);
static void script_2AF36(void);
static int32_t script_2AF3E(void);
static int32_t script_2AF4A(void);
static int32_t script_2AF56(void);
static int32_t script_2AF62(void);
static void script_2AF6E(void);
static void script_2AF76(void);
static int32_t script_2AF82(void);
static int32_t script_2AFCE(void);
static int32_t script_2AFDA(void);
static int32_t script_2AFE6(void);
static int32_t script_2AFF2(void);
static void script_2AFFE(void);
static int32_t script_2B006(void);
static int32_t script_2B012(void);
static int32_t script_2B01E(void);
static int32_t script_2B02A(void);
static int32_t script_2B036(void);
static void script_2B042(void);
static void script_2B04E(void);
static void script_2B05A(void);
static void script_2B066(void);
static void script_2B072(void);
static void script_2B07E(void);
static int32_t script_2B08A(void);
static int32_t script_2B096(void);
static void script_2B0A2(void);
static void script_2B0AE(void);
static int32_t script_2B0BA(void);
static int32_t script_2B0C6(void);
static int32_t script_2B0D2(void);
static int32_t script_2B0DE(void);
static int32_t script_2B0EA(void);
static int32_t script_2B0F6(void);
static int32_t script_2B102(void);
static int32_t script_2B10E(void);
static int32_t script_2B11A(void);
static void script_2B126(void);
static void script_2B132(void);
static int32_t script_2B13E(void);
static int32_t script_2B14A(void);
static int32_t script_2B156(void);
static int32_t script_2B162(void);
static void script_2B16E(void);
static void script_2B17A(void);
static void script_2B186(int32_t arg0);
static void script_2B1B1(int32_t arg0);
static int32_t script_2B1DF(int32_t arg0);
static void script_2B1FC(float arg0, float arg1, float arg2, float arg3, float arg4, float arg5);
static int32_t script_2B233(int32_t arg0, float arg1, float arg2, float arg3, float arg4);
static void script_2B25A(int32_t arg0, float arg1, float arg2, float arg3, float arg4);
static void script_2B29F(int32_t arg0);
static void script_2B2BB(int32_t arg0, float arg1);
static int32_t script_2B2DF(int32_t arg0, float arg1);
static void script_2B2F5(int32_t arg0, float arg1);
static void script_2B31A(int32_t arg0, float arg1);
static int32_t script_2B347(int32_t arg0, float arg1);
static int32_t script_2B360(int32_t arg0, float arg1);
static void script_2B376(int32_t arg0, float arg1);
static int32_t script_2B39B(int32_t arg0, float arg1);
static void script_2B3B4(int32_t arg0, float arg1);
static int32_t script_2B3D9(int32_t arg0, float arg1, float arg2, int32_t arg3, int32_t arg4);
static int32_t script_2B4BA(int32_t arg0, float arg1, float arg2, int32_t arg3, int32_t arg4);
static void script_2B4E5(int32_t arg0, float arg1, float arg2, int32_t arg3, int32_t arg4);
static void script_2B57D(int32_t arg0, float arg1, float arg2, float arg3, int32_t arg4, int32_t arg5);
static void script_2B5F4(int32_t arg0, float arg1, float arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_2B640(int32_t arg0);
static void script_2B65F(int32_t arg0);
static int32_t script_2B67E(int32_t arg0, int32_t arg1, int32_t arg2);
static int32_t script_2B693(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_2B76B(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_2B803(int32_t arg0, int32_t arg1, float arg2);
static void script_2B84A(int32_t arg0);
static void script_2B879(int32_t arg0);
static int32_t script_2B8B1(int32_t arg0, int32_t arg1);
static void script_2B90A(int32_t arg0, float arg1, float arg2, int32_t arg3, int32_t arg4, int8_t arg5);
static void script_2B990(int32_t arg0);
static void script_2BAA1(void);
static void script_2BBAC(int32_t arg0);
static void script_2BBCF(int32_t arg0, int32_t arg1, void *arg2, int32_t arg3, const void *arg4);
static void script_2BC6C(int32_t arg0, int32_t arg1, void *arg2, int32_t arg3, const void *arg4);
static int32_t script_2BD09(int32_t arg0);
static int32_t script_2BDC5(int32_t arg0);
static int32_t script_2BE81(int32_t arg0);
static int32_t script_2BF5F(void);
static int32_t script_2BFCB(int32_t arg0);
static int32_t script_2C0A6(int32_t arg0);
static void script_2C0F3(int32_t arg0);
static void script_2C107(int32_t arg0);
static int32_t script_2C11B(int32_t arg0);
static void script_2C189(int32_t arg0, int32_t arg1);
static void script_2C1A9(const char *arg0, const char *arg1);
static void script_2C200(int32_t arg0, int32_t arg1, float arg2, float arg3);
static void script_2C272(void);
static void script_2C299(void);
static void script_2C2C0(void);
static int32_t script_2C303(const char *arg0, float arg1, float arg2, float arg3, int32_t arg4);
static void script_2C371(int32_t arg0, int32_t arg1);
static void script_2C3AB(int32_t arg0, int32_t arg1, int32_t arg2);
static int32_t script_2C3D1(int32_t arg0);
static int32_t script_2C719(int32_t arg0);
static void script_2C7FA(int32_t arg0, int32_t arg1);
static int32_t script_2C8B9(void);
static int32_t script_2CA32(void);
static int32_t script_2CAE5(void);
static int32_t script_2CD9A(int32_t arg0);
static int32_t script_2CE5F(void);
static void script_2D4C0(void);
static void script_2D4F2(void);
static int32_t script_2D515(int32_t arg0);
static int32_t script_2D7CD(int32_t arg0);
static int32_t script_2D80C(void);
static int32_t script_2D83B(int32_t arg0);
static int32_t script_2D9A9(int32_t arg0);
static int32_t script_2E1CD(int32_t arg0);
static int32_t script_2E36D(int32_t arg0);
static int32_t script_2E486(int32_t arg0);
static int32_t script_2E56D(int32_t arg0);
static int32_t script_2E6B7(int32_t arg0);
static int32_t script_2E7A5(void);
static int32_t script_2F9C7(void);
static int32_t script_2F9E7(void);
static int32_t script_2FA07(float arg0, float arg1, float arg2, float arg3);
static int32_t script_2FA2E(void);
static int32_t script_2FA51(void);
static int32_t script_2FA54(void);
static int32_t script_2FA57(void);
static int32_t script_2FA9A(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_2FAE2(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static int32_t script_2FAFC(int32_t arg0, float arg1, int32_t arg2);
static void script_2FB22(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_2FB36(void);
static void script_2FB75(int32_t arg0);
static int32_t script_2FC02(void);
static int32_t script_2FCDA(int32_t arg0);
static int32_t script_2FDF6(int32_t arg0);
static int32_t script_2FEFC(int32_t arg0);
static int32_t script_3017F(int32_t arg0);
static int32_t script_30277(int32_t arg0);
static int32_t script_3045D(int32_t arg0);
static void script_30559(int32_t arg0);
static int32_t script_30594(int32_t arg0);
static int32_t script_31192(int32_t arg0);
static int32_t script_3126E(int32_t arg0);
static int32_t script_31312(int32_t arg0);
static int32_t script_3140B(void);
static int32_t script_32EEA(int32_t arg0, float arg1);
static int32_t script_32F23(void);
static void script_32F4F(void);
static int32_t script_330E9(int32_t arg0, int32_t arg1);
static int32_t script_33588(void);
static int32_t script_335CC(void);
static int32_t script_3362C(void);
static void script_338DF(void);
static void script_3390B(void);
static void script_339E2(void);
static void script_33AB9(void);
static void script_33CA3(void);
static void script_33CAF(void);
static void script_33D06(void);
static void script_33D25(int32_t arg0, int32_t arg1);
static int32_t script_33D49(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4);
static int32_t script_33E20(int32_t arg0, int32_t arg1, int32_t arg2, float arg3, int32_t arg4, int32_t arg5);
static void script_33EB7(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4);
static void script_33F19(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_33F75(int32_t arg0, float arg1, int32_t arg2);
static void script_33FD2(int32_t arg0);
static int32_t script_33FEE(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, void *arg4, int32_t arg5);
static void script_34215(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_3424C(int32_t arg0);
static void script_3425C(void);
static void script_3427C(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_342B6(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_34302(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4);
static void script_3434B(int32_t arg0);
static int32_t script_3435E(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_343C1(int32_t arg0, int32_t arg1);
static int32_t script_34482(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_34570(void);
static void script_3465E(int32_t arg0, int32_t arg1);
static void script_34687(int32_t arg0, int32_t arg1, float arg2, float arg3, float arg4);
static void script_34716(int32_t arg0, int32_t arg1, const Vector3 *arg2);
static void script_34749(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static int32_t script_34763(int32_t arg0, int32_t arg1, float arg2, float arg3, float arg4, int32_t arg5);
static void script_3478E(int32_t arg0, int32_t arg1, float arg2, float arg3, float arg4, float arg5);
static void script_3480E(int32_t arg0, int32_t arg1, float arg2);
static void script_34862(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_34878(int32_t arg0, int32_t arg1, float arg2, int32_t arg3);
static void script_3489F(int32_t arg0, float arg1);
static void script_348C8(int32_t arg0, float arg1, float arg2, float arg3);
static void script_3491C(int32_t arg0, int32_t arg1, int32_t arg2);
static int32_t script_34930(int32_t arg0, float arg1, int32_t arg2);
static void script_34955(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4);
static int32_t script_3496D(int32_t arg0, float arg1, float arg2, float arg3, int32_t arg4);
static void script_34996(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4);
static int32_t script_349AE(int32_t arg0, float arg1, float arg2, float arg3, int32_t arg4);
static void script_34A03(int32_t arg0, int32_t arg1);
static void script_34AE8(int32_t arg0, int32_t arg1);
static void script_34BD1(int32_t arg0);
static void script_34BEF(void);
static void script_34C37(void);
static void script_34C52(void);
static void script_34C6C(const char *arg0, const char *arg1);
static void script_34CAA(const char *arg0);
static void script_34CCB(int32_t arg0);
static void script_34CE2(int32_t arg0);
static void script_34D68(int32_t arg0, int32_t arg1);
static int32_t script_34D8B(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_34FFE(int32_t arg0, int32_t arg1);
static int32_t script_35010(int32_t arg0, int32_t arg1);
static void script_351B1(int32_t arg0, int32_t arg1);
static int32_t script_351C3(int32_t arg0, int32_t arg1);
static void script_351E7(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_351FD(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_35237(int32_t arg0, int32_t arg1);
static void script_35248(int32_t arg0, int32_t arg1);
static int32_t script_35389(int32_t arg0, int32_t arg1);
static int32_t script_3568E(int32_t arg0, int32_t arg1);
static void script_35CDE(int32_t arg0, int32_t arg1, float arg2, int32_t arg3, const void *arg4);
static void script_35D54(int32_t arg0, float arg1, const void *arg2);
static void script_35DD6(int32_t arg0, float arg1);
static void script_35E56(int32_t arg0);
static int32_t script_35EA3(int32_t arg0, const void *arg1, int32_t arg2, float arg3);
static int32_t script_35F08(void);
static void script_3605E(int32_t arg0);
static int32_t script_3610A(void);
static void script_361E9(void);
static void script_36281(void);
static int32_t script_363A1(void);
static int32_t script_36486(void);
static void script_364EB(int32_t arg0);
static int32_t script_364FB(int32_t arg0);
static void script_3656A(int32_t arg0, int32_t arg1);
static void script_365B6(int32_t arg0, int32_t arg1, int32_t arg2);
static int32_t script_365CA(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_365F6(void);
static void script_3661D(void);
static void script_3674F(void);
static void script_3697F(void);
static int32_t script_36B0A(int32_t arg0, int32_t arg1);
static int32_t script_36C4D(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_36D2B(void);
static void script_36DAD(void);
static int32_t script_36E35(void);
static void script_36EAD(int32_t arg0);
static int32_t script_36F2B(int32_t arg0);
static void script_36F97(int32_t arg0, int32_t arg1);
static void script_36FAE(void);
static void script_36FC8(const char *arg0, const char *arg1);
static void script_36FEB(const char *arg0, const char *arg1);
static void script_3700F(const char *arg0);
static void script_37033(int32_t arg0, float arg1, float arg2, float arg3, int32_t arg4, int32_t arg5, int32_t arg6);
static void script_37082(void);
static void script_370A2(int32_t arg0, int32_t arg1, Vector3 *arg2, Vector3i *arg3, const void *arg4);
static void script_370CA(int32_t arg0, const char *arg1);
static void script_370F3(void);
static void script_3710D(int32_t arg0);
static void script_37135(void);
static void script_37153(void);
static void script_37186(void);
static void script_37193(int32_t arg0);
static void script_371D2(void);
static void script_37222(void);
static void script_37283(int32_t arg0);
static int32_t script_37344(void);

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
static int32_t task_6C;
static int32_t task_70;
static int32_t task_74;
static int32_t task_78;
static int32_t task_7C;
static int32_t task_80;
static int32_t task_84;
static int32_t task_88;
static int32_t task_8C;
static int32_t task_90[4];
static int32_t task_A0;
static float task_A4;
static float task_A8;
static float task_AC;
static int8_t task_B0;
static int8_t task_B1;
static int8_t task_B2;
static int8_t task_B3;
static int8_t task_B4;
static int8_t task_B5;
static int8_t task_B6;
static int8_t task_B7;
static int8_t task_B8;
static uint8_t task_B9;
static uint16_t task_BA;
static int8_t task_BC;
static uint8_t task_BD;
static uint16_t task_BE;
static int32_t task_C0;
static int32_t task_C4;
static GameDateTime task_C8;
static int32_t task_D0;
static Vector3 task_D4;
static Vector3i task_E0;
static Vector3i task_EC;
static int32_t task_F8;
static int32_t task_FC;
static int32_t task_100;
static int8_t task_104;
static int8_t task_105;
static int16_t task_106;
static int8_t task_108;
static int8_t task_109;
static int8_t task_10A;
static int8_t task_10B;
static int8_t task_10C;
static int8_t task_10D;
static int8_t task_10E;
static int8_t task_10F;
static int8_t task_110;
static int8_t task_111;
static int8_t task_112;
static int8_t task_113;
static int8_t task_114;
static int8_t task_115;
static int8_t task_116;
static uint8_t task_117;
static int32_t task_118;
static char *task_11C;
static int32_t task_120;
static int32_t task_124;
static int32_t task_128;
static int8_t task_12C;
static int8_t task_12D;
static int8_t task_12E;
static uint8_t task_12F;
static int32_t task_130;
static int32_t task_134;
static int32_t task_138;
static int32_t task_13C;
static int32_t task_140;
static int8_t task_144;
static uint8_t task_145;
static uint16_t task_146;
static DSWKDoorEntry *task_148;
static int32_t *task_14C;
static DoorResourceDescriptor *task_150;
static MapObjectRecord *task_154;
static DoorSlotState task_158;
static int32_t task_164;
static int32_t task_168[10];
static int32_t task_190[10];
static int32_t task_1B8[10];
static int32_t task_1E0[10];
static int32_t task_208[10];
static int32_t task_230[10];
static int32_t task_258[10];
static int32_t task_280[10];
static int32_t task_2A8;
static int32_t task_2AC;
static Vector3 task_2B0;
static int32_t task_2BC;
static int32_t task_2C0;
static int32_t task_2C4;
static int8_t task_2C8;
static uint8_t task_2C9;
static uint16_t task_2CA;
static int8_t task_2CC;
static uint8_t task_2CD;
static uint16_t task_2CE;
static int32_t task_2D0;
static int32_t task_2D4;
static int8_t task_2D8;
static uint8_t task_2D9;
static uint16_t task_2DA;
static Vector3 task_2DC;
static int32_t task_2E8;
static int32_t task_2EC;
static int32_t task_2F0;
static int32_t task_2F4;
static int32_t task_2F8;
static int32_t task_2FC;
static int32_t task_300;
static int32_t task_304;
static int32_t task_308;
static int32_t task_30C;
static int32_t task_310;
static int8_t task_314;
static int8_t task_315;
static int8_t task_316;
static int8_t task_317;
static int8_t task_318;
static uint8_t task_319;
static uint16_t task_31A;
static int8_t task_31C;
static uint8_t task_31D;
static uint16_t task_31E;
static int32_t task_320[23];
static int8_t task_37C;
static int8_t task_37D;
static int8_t task_37E;
static int8_t task_37F;
static int8_t task_380;
static uint8_t task_381;
static uint16_t task_382;
static int32_t task_384;
static int32_t task_388;
static int8_t task_38C;
static int8_t task_38D;
static int8_t task_38E;
static int8_t task_38F;
static int8_t task_390;
static uint8_t task_391;
static uint16_t task_392;
static int8_t task_394;
static uint8_t task_395;
static uint16_t task_396;
static float task_398;
static int32_t task_39C;
static int32_t task_3A0;
static int8_t task_3A4;
static uint8_t task_3A5;
static uint16_t task_3A6;
static int32_t task_3A8;
static int32_t task_3AC;
static int32_t task_3B0;
static int32_t task_3B4;
static int32_t task_3B8;
static int32_t task_3BC;
static int32_t task_3C0;
static int32_t task_3C4;
static int32_t task_3C8;
static int32_t task_3CC;
static int32_t task_3D0;
static int32_t task_3D4;
static int32_t task_3D8;
static int32_t task_3DC;
static int32_t task_3E0;
static int32_t task_3E4;
static int32_t task_3E8;
static int32_t task_3EC;
static int32_t task_3F0;
static int32_t task_3F4;
static int32_t task_3F8;
static int32_t task_3FC;
static int32_t task_400;
static int32_t task_404;
static int32_t task_408;
static int32_t task_40C;
static int32_t task_410;
static int32_t task_414;
static int32_t task_418;
static int8_t task_41C;
static int8_t task_41D;
static int8_t task_41E;
static int8_t task_41F;
static int8_t task_420;
static uint8_t task_421;
static uint16_t task_422;
static int32_t task_424;
static int32_t task_428;
static int8_t task_42C[100][12];
static int8_t task_8DC[12];
static int8_t task_8E8[10];
static int8_t task_8F2;
static int8_t task_8F3;
static int8_t task_8F4;
static uint8_t task_8F5;
static uint16_t task_8F6;
static int32_t task_8F8;
static int32_t task_8FC;
static int8_t task_900;
static uint8_t task_901;
static uint16_t task_902;
static int32_t task_904[10];
static int32_t task_92C;
static int32_t task_930;
static int32_t task_934;
static int32_t task_938;
static int32_t task_93C;
static int32_t task_940;
static int32_t task_944;
static int32_t task_948;
static int32_t task_94C;
static int32_t task_950;
static int32_t task_954[11];
static int32_t task_980[3];
static int32_t task_98C;
static int32_t task_990;
static int8_t task_994;
static int8_t task_995;
static uint8_t task_996;
static int8_t task_997;
static int8_t task_998;
static int8_t task_999;
static int8_t task_99A;
static int8_t task_99B;
static int8_t task_99C;
static int8_t task_99D;
static int8_t task_99E;
static int8_t task_99F;
static int32_t task_9A0[9];
static int8_t task_9C4;
static int8_t task_9C5;
static int8_t task_9C6;
static int8_t task_9C7;
static int8_t task_9C8;
static int8_t task_9C9;
static int8_t task_9CA;
static int8_t task_9CB;
static int8_t task_9CC;
static uint8_t task_9CD;
static uint16_t task_9CE;
static int8_t task_9D0;
static uint8_t task_9D1;
static uint16_t task_9D2;
static int32_t task_9D4;
static int32_t task_9D8;
static int32_t task_9DC;
static int32_t task_9E0;
static int32_t task_9E4;
static int8_t task_9E8;
static uint8_t task_9E9;
static uint16_t task_9EA;
static int8_t task_9EC;
static uint8_t task_9ED;
static uint16_t task_9EE;
static int32_t task_9F0[2];
static int32_t task_9F8[2];
static int32_t task_A00[2];
static int32_t task_A08[2];
static int32_t task_A10[2];
static int32_t task_A18;
static int32_t task_A1C[2];
static int32_t task_A24[2];
static int32_t task_A2C[2];
static int32_t task_A34[2];
static int32_t task_A3C;
static int32_t task_A40;
static int32_t task_A44;
static int32_t task_A48;
static int32_t task_A4C;
static int32_t task_A50;
static int32_t task_A54[2];
static float task_A5C[2][3];
static float task_A74[2];
static float task_A7C;
static float task_A80;
static float task_A84;
static float task_A88[2][3];
static int32_t task_AA0[2][3];
static float task_AB8[2];
static int32_t task_AC0[2];
static int32_t task_AC8;
static int32_t task_ACC;
static int8_t task_AD0;
static uint8_t task_AD1;
static uint16_t task_AD2;
static int8_t task_AD4[4];
static int8_t task_AD8;
static uint8_t task_AD9;
static uint16_t task_ADA;
static int32_t task_ADC;
static int32_t task_AE0[2];
static int32_t task_AE8[2];
static int32_t task_AF0[2];
static int32_t task_AF8[2];
static float task_B00[2];
static const Vector3 data_37398 = {.x = 1.0f, .y = 1.0f, .z = 1.0f};
static const int32_t data_373A4[4] = {16777215, 16777215, 16777215, 16777215};
static const float data_373B4[3] = {20.0f, 800.0f, 1.0f};
static const int32_t data_373C0 = -1;
static const char data_373C4[10] __attribute__((scn_string_literal)) = "Kaidan : ";
static const char data_373CE[32] __attribute__((scn_string_literal)) = "========== initializing kaidan\012";
static const char data_373EE[48] __attribute__((scn_string_literal)) = "please ENDOFKDATA use can't start shuki kaidan\012";
static const char data_3741E[] __attribute__((scn_string_literal)) = "Kaidan Function Error 1\n";
static const char data_37437[] __attribute__((scn_string_literal)) = "Kaidan Function Error 2\n";
static const char data_37450[] __attribute__((scn_string_literal)) = "Kaidan Data Error\n";
static const char data_37463[10] __attribute__((scn_string_literal)) = "Kaidan : ";
static const char data_3746D[28] __attribute__((scn_string_literal)) = "wait ... shuki shuki shuki\012";
static const char data_37489[10] __attribute__((scn_string_literal)) = "Kaidan : ";
static const char data_37493[24] __attribute__((scn_string_literal)) = "KAIDAN : Warning coli!\012";
static const char data_374AB[24] __attribute__((scn_string_literal)) = "KAIDAN : Warning coli!\012";
static const char data_374C3[24] __attribute__((scn_string_literal)) = "KAIDAN : Warning coli!\012";
static const char data_374DB[24] __attribute__((scn_string_literal)) = "KAIDAN : Warning coli!\012";
static const char data_374F3[24] __attribute__((scn_string_literal)) = "KAIDAN : Warning coli!\012";
static const char data_3750B[24] __attribute__((scn_string_literal)) = "KAIDAN : Warning setY!\012";
static const char data_37523[24] __attribute__((scn_string_literal)) = "KAIDAN : Warning setY!\012";
static const char data_3753B[241] __attribute__((scn_string_literal)) = "Kaidan : ";
static const int32_t data_3762C[171] = {
  0, 0, -10922, 0, 0, -10558, 0, 1456, 0, 0, 0, -18022, 0, 0, -17112, 0, 0, -18386, 0, 0, 0, 0, 0, -12196, 0, 0, -18204, 0, 0, -16748, 0, 0, 0, 0, 0,
  -11832, 0, 0, -18386, 0, 0, -15109, 0, 0, 0, 0, 910, -12014, 0, -2184, -16748, 0, -2184, -10012, 0, 0, 0, 0, -364, -11650, -2912, -364, -4369, 1638,
  910, 182, 0, 0, -16019, 0, 182, -23119, 0, 546, -18932, 0, 0, -1092, 0, 0, -16384, 0, 0, -21845, 0, 0, -18750, 0, 0, -546, 0, 0, -21481, 0, 0,
  -21299, 0, 0, -18568, 0, 0, -728, 0, 910, -18750, 0, 182, -18750, 0, -4004, -18204, 0, 0, -728, 0, -364, -14017, -2912, -364, -6735, 1638, 910,
  2548, 0, 0, -18568, 0, 182, -25668, 0, 546, -21481, 0, 0, -3640, 0, 0, -18932, 0, 0, -24393, 0, 0, -21299, 0, 0, -3094, 0, 0, -24029, 0, 0, -23847,
  0, 0, -21117, 0, 0, -3276, 0, 910, -21299, 0, 182, -21299, 0, -4004, -20753, 0, 0, -3276
};
static const int32_t data_378D8[114] = {
  0, 0, -5461, 0, -2366, -1092, 0, -182, 1092, 0, -546, -6735, 0, 0, -4004, 0, 0, -2912, 0, -182, 1274, 0, 0, -13289, 0, 0, -5279, 0, 0, -8009, 0, 0,
  1456, 0, 182, -8738, 0, 0, -6007, 0, 0, -8738, 0, 182, 1638, 0, -728, -5279, 0, -1274, -6735, 0, -910, -8738, 0, 0, 1638, 0, 0, -5461, 0, -2366,
  -1092, 0, -182, 1092, 0, -546, -13289, 0, 0, -5643, 0, 0, -8009, 0, -182, 1274, 0, 0, -11832, 0, 0, -5279, 0, 0, -8009, 0, 0, 1456, 0, 182, -8738,
  0, 0, -6007, 0, 0, -8738, 0, 182, 1638, 0, -728, -5279, 0, -1274, -6735, 0, -910, -8738, 0, 0, 1092
};
static const int32_t data_37AA0[399] = {
  0, 0, -5461, 0, -2366, -1092, 0, -182, 1092, 0, -546, -3640, 0, 0, -2366, 0, 0, -1456, 0, -182, 1274, 0, 0, -3822, 0, 0, -2730, 0, 0, -1456, 0, 0,
  1456, 0, 182, -3822, 0, 0, -2912, 0, 0, -1638, 0, 182, 1638, 0, -728, -3640, 0, -910, -2730, 0, -546, -1638, 0, 0, 1092, 0, 0, -5461, 0, -2366,
  -1092, 0, -182, 1092, 0, -546, -6735, 0, 0, -4004, 0, 0, -6553, 0, -182, 1274, 0, 0, -13289, 0, 0, -5279, 0, 0, -8009, 0, 0, 1456, 0, 182, -8738, 0,
  0, -6007, 0, 0, -8738, 0, 182, 1638, 0, -728, -5279, 0, -1274, -6735, 0, -910, -8738, 0, 0, 1638, 0, 0, -9102, 0, -2366, -2002, 0, -182, 910, 0,
  -546, -2366, 0, 0, -2002, 0, 0, -2912, 0, -182, 1274, 0, 0, -14199, 0, 0, -11468, 0, 0, -13653, 0, 0, 1456, 0, 182, -11286, 0, 0, -12014, 0, 0,
  -12743, 0, 182, 1638, 0, -728, -10194, 0, -1274, -12196, 0, -910, -10922, -728, 546, 1638, 0, 0, -1820, 0, -2366, -728, 0, -364, -1092, 0, -546,
  -11832, 0, 910, -6735, 0, 0, -6553, 0, -182, 1274, 0, 0, -13289, 0, 0, -6007, 0, 0, -10012, 0, 0, 1092, 0, 182, -8738, 0, 0, -6735, 0, 0, -9830, 0,
  182, 1638, 0, -728, -6553, 0, -1274, -7281, 0, -910, -10740, 0, 0, 1638, 0, 0, -5461, -2912, -2366, -364, -5279, 2548, -910, 0, -546, -8374, 0, 0,
  -9466, 0, 910, -4915, 0, -182, 546, 0, 0, -13289, 0, 0, -5279, 0, 364, -7645, 0, -182, 1456, 0, 182, -8738, 0, 0, -7281, 0, 182, -6917, 0, -182,
  1638, 0, -728, -5279, 0, -1274, -6735, 0, -182, -8009, 0, 0, 1638, -4733, 0, -8556, 0, -2366, -1092, -6189, 2912, 0, 0, -546, -10922, 0, 1820,
  -16384, 0, 0, -11468, 0, 182, 546, 0, 0, -10376, 0, 0, -15837, 0, 0, -12743, 0, -182, 728, 0, 182, -13835, 0, 0, -16201, 0, 364, -10922, 0, -182,
  728, 0, -728, -9648, 0, -3458, -12379, 0, -728, -7463, 0, 0, 1092, 0, 0, -5461, 0, -2366, -1638, 0, -910, 1092, 0, -546, -3640, 0, 0, -2366, 0, 0,
  -1456, 0, -182, 1638, 0, 0, -910, 0, 0, -2730, 0, 0, -1274, 0, 0, 1820, 0, 182, -2730, 0, 0, -1820, 0, 0, -1274, 0, 182, 2184, 0, -728, -3094, 0,
  -910, -2548, 0, 910, -1456, -546, 910, 2366
};
static const uint32_t data_380DC = SHEN_NAME('VM_0');
static const shen_name data_380E0[7] = {
  SHEN_NAME('VM_1'), SHEN_NAME('VM_2'), SHEN_NAME('VM_3'), SHEN_NAME('VM_4'), SHEN_NAME('VM_5'), SHEN_NAME('VM_6'), SHEN_NAME('VM_7')
};
static const char data_380FC[5] __attribute__((scn_string_literal)) = "SY11";
static const char data_38101[9] __attribute__((scn_string_literal)) = "SY11A006";
static const char data_3810A[9] __attribute__((scn_string_literal)) = "SY11A007";
static const char data_38113[9] __attribute__((scn_string_literal)) = "SY11A008";
static const char data_3811C[9] __attribute__((scn_string_literal)) = "SY11A002";
static const char data_38125[9] __attribute__((scn_string_literal)) = "SY11A003";
static const char data_3812E[9] __attribute__((scn_string_literal)) = "SY11A004";
static const char data_38137[9] __attribute__((scn_string_literal)) = "SY11A005";
static const char data_38140[9] __attribute__((scn_string_literal)) = "SY11A001";
static const char data_38149[9] __attribute__((scn_string_literal)) = "SY11A009";
static const char data_38152[9] __attribute__((scn_string_literal)) = "SY11A010";
static const char data_3815B[9] __attribute__((scn_string_literal)) = "SY11A011";
static const char data_38164[9] __attribute__((scn_string_literal)) = "SY11A009";
static const char data_3816D[5] __attribute__((scn_string_literal)) = "SY11";
static const char data_38172[5] __attribute__((scn_string_literal)) = "SY11";
static const char data_38177[9] __attribute__((scn_string_literal)) = "SY11A001";
static const char data_38180[5] __attribute__((scn_string_literal)) = "SY11";
static const char data_38185[5] __attribute__((scn_string_literal)) = "SY11";
static const char data_3818A[9] __attribute__((scn_string_literal)) = "SY11A009";
static const char data_38193[9] __attribute__((scn_string_literal)) = "SY11A010";
static const char data_3819C[9] __attribute__((scn_string_literal)) = "SY11A011";
static const char data_381A5[5] __attribute__((scn_string_literal)) = "SY11";
static const char data_381AA[18] __attribute__((scn_string_literal)) = "/scene/02/stream/";
static const char data_381BC[5] __attribute__((scn_string_literal)) = "%03d";
static const char data_381C1[] __attribute__((scn_string_literal)) = "ATRK";
static const uint8_t data_381C6[2] = {0};
static Vector3 data_381C8 = {.x = 0.0f, .y = -0.1f, .z = 0.0f};
static Vector3 data_381D4 = {.x = 0.0f, .y = 0.0f, .z = 0.0f};
static const char data_381E0[] __attribute__((scn_string_literal)) = "VEND";
static const char data_381E5[] __attribute__((scn_string_literal)) = "VEND";
static const char data_381EA[] __attribute__((scn_string_literal)) = "SPRT";
static const char data_381EF[7] __attribute__((scn_string_literal)) = "01JUCE";
static const char data_381F6[13] __attribute__((scn_string_literal)) = "HMOT_CAN.BIN";
static const char data_38203[] __attribute__((scn_string_literal)) = "VEND";
static const char data_38208[7] __attribute__((scn_string_literal)) = "01JUCE";
static const char data_3820F[26] __attribute__((scn_string_literal)) = "HandMot read by current!\012";
static const char data_38229[13] __attribute__((scn_string_literal)) = "HMOT_CAN.BIN";
static const char data_38236[7] __attribute__((scn_string_literal)) = "01JUCE";
static const char data_3823D[] __attribute__((scn_string_literal)) = "VEND";
static const uint8_t data_38242[2] = {0};
static const uint8_t data_38244[12] = {0};
static const char data_38250[29] __attribute__((scn_string_literal)) = "vendmachine exit is EV_STOP\012";
static const char data_3826D[41] __attribute__((scn_string_literal)) = "vendmachine exit is MONEY_LOCK -> %d,%d\012";
static const char data_38296[] __attribute__((scn_string_literal)) = "COKE";
static const char data_3829B[] __attribute__((scn_string_literal)) = "FATG";
static const char data_382A0[] __attribute__((scn_string_literal)) = "FATO";
static const char data_382A5[] __attribute__((scn_string_literal)) = "SPRT";
static const char data_382AA[] __attribute__((scn_string_literal)) = "CAFE";
static const uint8_t data_382AF[1] = {0};
static const uint32_t data_382B0 = 0x3D204C06;
static const float data_382B4[2] = {0.0f, 0.890018f};
static const uint8_t data_382BC[12] = {0};
static const uint8_t data_382C8[12] = {0};
static const uint32_t data_382D4 = 0x3DE978D5;
static const float data_382D8[2] = {0.002f, -0.036f};
static const uint32_t data_382E0 = 8192;
static const int32_t data_382E4[2] = {11377, 3640};
static const uint32_t data_382EC = 0x3D204C06;
static const float data_382F0[2] = {0.0f, 0.890018f};
static const uint8_t data_382F8[12] = {0};
static const uint32_t data_38304 = 4369;
static const int32_t data_38308[2] = {0, -2184};
static const char data_38310[] __attribute__((scn_string_literal)) = "01JUCEA011";
static const char data_3831B[] __attribute__((scn_string_literal)) = "01JUCEA012";
static const char data_38326[] __attribute__((scn_string_literal)) = "01JUCEA013";
static const char data_38331[] __attribute__((scn_string_literal)) = "01JUCEA002";
static const char data_3833C[] __attribute__((scn_string_literal)) = "01JUCEA003";
static const uint8_t data_38347[1] = {0};
static const uint32_t data_38348 = 0x3DE978D5;
static const float data_3834C[2] = {0.002f, -0.036f};
static const uint32_t data_38354 = 8192;
static const int32_t data_38358[2] = {11377, 3640};
static const uint32_t data_38360 = 0x3DCB04AB;
static const float data_38364[2] = {0.02044f, -0.04322f};
static const uint32_t data_3836C = 0xFFFFFE69;
static const int32_t data_38370[2] = {3331, -3464};
static const uint32_t data_38378 = 0x3D204C06;
static const float data_3837C[2] = {0.0f, 0.890018f};
static const uint8_t data_38384[12] = {0};
static const uint32_t data_38390 = 4369;
static const int32_t data_38394[2] = {0, -2184};
static const uint8_t data_3839C[12] = {0};
static const char data_383A8[] __attribute__((scn_string_literal)) = "01JUCEA002";
static const char data_383B3[] __attribute__((scn_string_literal)) = "01JUCEA003";
static const uint8_t pad_383BE[4] = {0};
static const uint8_t data_383C2[10] = {
  0x80, 0x3F, 0x00, 0x00, 0x80, 0x3F, 0x00, 0x00, 0x80, 0x3F
};
static const uint32_t data_383CC = 0xC2480000;
static const float data_383D0[2] = {0.0f, 10.0f};
static const uint32_t data_383D8 = 0x41A00000;
static const float data_383DC[2] = {0.0f, 110.0f};
static const uint32_t data_383E4 = 0xC2041FF3;
static const float data_383E8[2] = {0.0f, 52.1008f};
static const uint32_t data_383F0 = 0x41B3AA99;
static const uint32_t data_383F4[2] = {0, 0x420947FDU};
static const uint32_t data_383FC = 0x42201A1D;
static const float data_38400[2] = {0.0f, -2.1877f};
static const uint32_t data_38408 = 0xC2BD9F2E;
static const uint32_t data_3840C[2] = {0, 0x428A24A9U};
static const uint32_t data_38414 = 0x41D6BFB1;
static const uint32_t data_38418[2] = {0, 0x42CFC5AFU};
static const uint32_t data_38420 = 0xC1A00000;
static const float data_38424[2] = {0.0f, 110.0f};
static const uint32_t data_3842C = 0xC19181A3;
static const uint32_t data_38430[2] = {0, 0x421B20DFU};
static const uint32_t data_38438 = 0xC2C00000;
static const float data_3843C[2] = {0.0f, 78.0f};
static const uint32_t data_38444 = 0xC2880000;
static const float data_38448[2] = {0.0f, 53.0f};
static const uint32_t data_38450 = 0xC1A00000;
static const float data_38454[2] = {0.0f, 110.0f};
static const uint32_t data_3845C = 0xC2041FF3;
static const float data_38460[2] = {0.0f, 52.1008f};
static const uint32_t data_38468 = 0x3F4978D5;
static const float data_3846C[33] = {
  0.787f, 0.866f, 0.944f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.944f, 0.866f, 0.787f,
  0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f
};
static const uint8_t data_384F0[80] = {0};
typedef struct Row_38540 { float w0; float w1; float w2; int32_t w3; } Row_38540;
static const Row_38540 data_38540[3] = {
  {1.0f, 1.0f, 1.0f, 0},
  {0.0f, 0.0f, 0.0f, 0},
  {0.0f, 0.0f, 0.0f, 0}
};
static const int32_t data_38570[2] = {0, 0};
static const uint8_t data_38578[24] = {0};
static const float data_38590[28] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f
};
static const uint8_t data_38600[24] = {0};
static const float data_38618[28] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f
};
static const uint32_t data_38688 = 0x3F4978D5;
static const float data_3868C[33] = {
  0.787f, 0.866f, 0.944f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  0.944f, 0.866f, 0.866f, 0.866f, 0.866f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f
};
static const uint8_t data_38710[24] = {0};
static const float data_38728[28] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f
};
static const uint32_t data_38798 = 0x3F800000;
static const float data_3879C[33] = {
  0.944f, 0.866f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f,
  0.787f, 0.787f, 0.551f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint8_t data_38820[104] = {0};
static const float data_38888[8] = {1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f};
static const uint8_t data_388A8[96] = {0};
static const float data_38908[10] = {1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f};
static const uint32_t data_38930 = 0x3F266666;
static const float data_38934[33] = {
  0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f,
  0.708f, 0.787f, 0.866f, 0.944f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint32_t data_389B8 = 0x3F800000;
static const float data_389BC[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint32_t data_38A40 = 0x3F800000;
static const float data_38A44[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint32_t data_38AC8 = 0x3F800000;
static const float data_38ACC[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint8_t data_38B50[12] = {0};
static const float data_38B5C[31] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint32_t data_38BD8 = 0xC2480000;
static const float data_38BDC[2] = {0.0f, 10.0f};
static const uint32_t data_38BE4 = 0x4214CCCD;
static const float data_38BE8[2] = {0.0f, 6.6f};
static const uint32_t data_38BF0 = 0x41B5999A;
static const float data_38BF4[2] = {0.0f, 27.2f};
static const uint32_t data_38BFC = 0x41B3999A;
static const float data_38C00[2] = {0.0f, 34.32f};
static const uint32_t data_38C08 = 0x4220147B;
static const float data_38C0C[2] = {0.0f, -2.18f};
static const uint32_t data_38C14 = 0xC2BD9EB8;
static const float data_38C18[2] = {0.0f, 69.07f};
static const uint32_t data_38C20 = 0x41D6B852;
static const float data_38C24[2] = {0.0f, 103.88f};
static const uint32_t data_38C2C = 0xC0966666;
static const float data_38C30[2] = {0.0f, 30.6f};
static const uint32_t data_38C38 = 0xC19170A4;
static const float data_38C3C[2] = {0.0f, 38.78f};
static const uint32_t data_38C44 = 0xC2C00000;
static const float data_38C48[2] = {0.0f, 78.0f};
static const uint32_t data_38C50 = 0xC2840000;
static const float data_38C54[2] = {0.0f, 54.0f};
static const uint32_t data_38C5C = 0xC1FB0A3D;
static const float data_38C60[2] = {0.0f, 40.1f};
static const uint32_t data_38C68 = 0xC2C28F5C;
static const float data_38C6C[2] = {0.0f, 64.48f};
static const uint32_t data_38C74 = 0xC2F2FAE1;
static const float data_38C78[2] = {0.0f, 94.2f};
static const uint32_t data_38C80 = 0xC2BC75C3;
static const float data_38C84[2] = {0.0f, 85.91f};
static const uint32_t data_38C8C = 0x3F4978D5;
static const float data_38C90[33] = {
  0.787f, 0.866f, 0.944f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.944f, 0.866f, 0.787f,
  0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f
};
static const uint8_t data_38D14[16] = {0};
static const float data_38D24[30] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint8_t data_38D9C[16] = {0};
static const float data_38DAC[30] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint8_t data_38E24[24] = {0};
static const float data_38E3C[28] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f
};
static const uint32_t data_38EAC = 0x3F4978D5;
static const float data_38EB0[33] = {
  0.787f, 0.866f, 0.944f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  0.944f, 0.866f, 0.866f, 0.866f, 0.866f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f
};
static const uint8_t data_38F34[24] = {0};
static const float data_38F4C[28] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f
};
static const uint32_t data_38FBC = 0x3F800000;
static const float data_38FC0[33] = {
  0.944f, 0.866f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f,
  0.787f, 0.787f, 0.551f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint8_t data_39044[16] = {0};
static const float data_39054[30] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint8_t data_390CC[96] = {0};
static const float data_3912C[10] = {1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f};
static const uint32_t data_39154 = 0x3F266666;
static const float data_39158[33] = {
  0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f,
  0.708f, 0.787f, 0.866f, 0.944f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint32_t data_391DC = 0x3F800000;
static const float data_391E0[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint8_t data_39264[16] = {0};
static const float data_39274[30] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint8_t data_392EC[80] = {0};
static const float data_3933C[14] = {1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f};
static const uint8_t data_39374[80] = {0};
static const float data_393C4[14] = {1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f};
static const uint8_t data_393FC[80] = {0};
static const float data_3944C[14] = {1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f};
static const uint8_t data_39484[12] = {0};
static const float data_39490[31] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint32_t data_3950C = 0xC2480000;
static const float data_39510[2] = {0.0f, 10.0f};
static const uint32_t data_39518 = 0x41E80000;
static const shen_name data_3951C[2] = {0, SHEN_NAME('333A')};
static const uint32_t data_39524 = 0xC2041FF3;
static const float data_39528[2] = {0.0f, 52.1008f};
static const uint32_t data_39530 = 0x41B3AA99;
static const uint32_t data_39534[2] = {0, 0x420947FDU};
static const uint32_t data_3953C = 0x42201A1D;
static const float data_39540[2] = {0.0f, -2.1877f};
static const uint32_t data_39548 = 0xC2BD9F2E;
static const uint32_t data_3954C[2] = {0, 0x428A24A9U};
static const uint32_t data_39554 = 0x41D6BFB1;
static const uint32_t data_39558[2] = {0, 0x42CFC5AFU};
static const uint32_t data_39560 = 0xC1A00000;
static const float data_39564[2] = {0.0f, 110.0f};
static const uint32_t data_3956C = 0xC19181A3;
static const uint32_t data_39570[2] = {0, 0x421B20DFU};
static const uint32_t data_39578 = 0xC2C00000;
static const float data_3957C[2] = {0.0f, 78.0f};
static const uint32_t data_39584 = 0xC2880000;
static const float data_39588[2] = {0.0f, 53.0f};
static const uint32_t data_39590 = 0xC0B9999A;
static const float data_39594[2] = {0.0f, 26.2f};
static const uint32_t data_3959C = 0xC22E0000;
static const float data_395A0[2] = {0.0f, 46.0f};
static const uint32_t data_395A8 = 0xC2CE0000;
static const float data_395AC[2] = {0.0f, 80.0f};
static const uint32_t data_395B4 = 0xC2041FF3;
static const float data_395B8[2] = {0.0f, 52.1008f};
static const uint32_t data_395C0 = 0x3F4978D5;
static const float data_395C4[33] = {
  0.787f, 0.866f, 0.944f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.944f, 0.866f, 0.787f,
  0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f
};
static const uint8_t data_39648[16] = {0};
static const float data_39658[30] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint8_t data_396D0[24] = {0};
static const float data_396E8[28] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f
};
static const uint8_t data_39758[24] = {0};
static const float data_39770[28] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f
};
static const uint32_t data_397E0 = 0x3F4978D5;
static const float data_397E4[33] = {
  0.787f, 0.866f, 0.944f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  0.944f, 0.866f, 0.866f, 0.866f, 0.866f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f
};
static const uint8_t data_39868[24] = {0};
static const float data_39880[28] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f
};
static const uint32_t data_398F0 = 0x3F800000;
static const float data_398F4[33] = {
  0.944f, 0.866f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f,
  0.787f, 0.787f, 0.551f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint8_t data_39978[104] = {0};
static const float data_399E0[8] = {1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f};
static const uint8_t data_39A00[96] = {0};
static const float data_39A60[10] = {1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f};
static const uint32_t data_39A88 = 0x3F266666;
static const float data_39A8C[33] = {
  0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f,
  0.708f, 0.787f, 0.866f, 0.944f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint32_t data_39B10 = 0x3F800000;
static const float data_39B14[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint8_t data_39B98[16] = {0};
static const float data_39BA8[30] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint8_t data_39C20[16] = {0};
static const float data_39C30[30] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint8_t data_39CA8[80] = {0};
static const float data_39CF8[14] = {1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f};
static const uint32_t data_39D30 = 0x3F800000;
static const float data_39D34[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint8_t data_39DB8[12] = {0};
static const float data_39DC4[31] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint32_t data_39E40 = 0xC2480000;
static const float data_39E44[2] = {0.0f, 10.0f};
static const uint32_t data_39E4C = 0x41A00000;
static const float data_39E50[2] = {0.0f, 110.0f};
static const uint32_t data_39E58 = 0xC2041FF3;
static const float data_39E5C[2] = {0.0f, 52.1008f};
static const uint32_t data_39E64 = 0x41B3AA99;
static const uint32_t data_39E68[2] = {0, 0x420947FDU};
static const uint32_t data_39E70 = 0x42201A1D;
static const float data_39E74[2] = {0.0f, -2.1877f};
static const uint32_t data_39E7C = 0xC2BD9F2E;
static const uint32_t data_39E80[2] = {0, 0x428A24A9U};
static const uint32_t data_39E88 = 0x41D6BFB1;
static const uint32_t data_39E8C[2] = {0, 0x42CFC5AFU};
static const uint32_t data_39E94 = 0xC1A00000;
static const float data_39E98[2] = {0.0f, 110.0f};
static const uint32_t data_39EA0 = 0xC19181A3;
static const uint32_t data_39EA4[2] = {0, 0x421B20DFU};
static const uint32_t data_39EAC = 0xC2C00000;
static const float data_39EB0[2] = {0.0f, 78.0f};
static const uint32_t data_39EB8 = 0xC2880000;
static const float data_39EBC[2] = {0.0f, 53.0f};
static const uint32_t data_39EC4 = 0x40A66666;
static const float data_39EC8[2] = {0.0f, 27.0f};
static const uint32_t data_39ED0 = 0xC201999A;
static const float data_39ED4[2] = {0.0f, 44.5f};
static const uint32_t data_39EDC = 0xC2CE0000;
static const float data_39EE0[2] = {0.0f, 80.0f};
static const uint32_t data_39EE8 = 0xC2041FF3;
static const float data_39EEC[2] = {0.0f, 52.1008f};
static const uint32_t data_39EF4 = 0x3F4978D5;
static const float data_39EF8[33] = {
  0.787f, 0.866f, 0.944f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.944f, 0.866f, 0.787f,
  0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f
};
static const uint8_t data_39F7C[80] = {0};
typedef struct Row_39FCC { float w0; float w1; float w2; int32_t w3; } Row_39FCC;
static const Row_39FCC data_39FCC[3] = {
  {1.0f, 1.0f, 1.0f, 0},
  {0.0f, 0.0f, 0.0f, 0},
  {0.0f, 0.0f, 0.0f, 0}
};
static const int32_t data_39FFC[2] = {0, 0};
static const uint8_t data_3A004[24] = {0};
static const float data_3A01C[28] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f
};
static const uint8_t data_3A08C[24] = {0};
static const float data_3A0A4[28] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f
};
static const uint32_t data_3A114 = 0x3F4978D5;
static const float data_3A118[33] = {
  0.787f, 0.866f, 0.944f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  0.944f, 0.866f, 0.866f, 0.866f, 0.866f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f
};
static const uint8_t data_3A19C[24] = {0};
static const float data_3A1B4[28] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f
};
static const uint32_t data_3A224 = 0x3F800000;
static const float data_3A228[33] = {
  0.944f, 0.866f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f,
  0.787f, 0.787f, 0.551f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint8_t data_3A2AC[104] = {0};
static const float data_3A314[8] = {1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f};
static const uint8_t data_3A334[96] = {0};
static const float data_3A394[10] = {1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f};
static const uint32_t data_3A3BC = 0x3F266666;
static const float data_3A3C0[33] = {
  0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f,
  0.708f, 0.787f, 0.866f, 0.944f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint32_t data_3A444 = 0x3F800000;
static const float data_3A448[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint8_t data_3A4CC[16] = {0};
static const float data_3A4DC[30] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint8_t data_3A554[16] = {0};
static const float data_3A564[30] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint8_t data_3A5DC[80] = {0};
static const float data_3A62C[14] = {1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f};
static const uint32_t data_3A664 = 0x3F800000;
static const float data_3A668[67] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const int32_t data_3A774[57] = {
  0, 0, -10977, 0, 0, -4842, 0, 0, 0, 0, 0, -4531, 0, 0, -1820, 0, 0, -3877, 0, 0, 0, 0, 0, -8574, 0, 0, -16074, 0, 0, -4642, 0, 0, 0, 0, 0, -8610, 0,
  182, -16966, 0, 0, -6407, 0, 36, -54, 0, 0, -11450, 0, 0, -13562, 0, 0, -8974, 0, -819, -345
};
static const int32_t data_3A858[57] = {
  0, 0, -6005, 0, 0, -909, 0, 0, 0, 0, 0, -6368, 0, 1820, -8552, 0, 0, -4548, 0, 0, 0, 0, 0, -8552, 0, 0, -10372, 0, -545, -4002, 0, 0, 0, 0, 0,
  -7823, 0, 0, -12374, 0, -909, -2182, 0, 0, 0, 0, -1636, -10735, 0, -2001, -10372, 0, -1819, -727, 0, 0, 0
};
static const Vector4 data_3A93C = {.x = 1.0f, .y = 0.3f, .z = 0.3f, .w = 0.3f};
static const Vector3 data_3A94C = {.x = 38.2248f, .y = -10.0f, .z = 10.056f};
static const FogColourData data_3A958 = {.red = 20.0f, .green = 800.0f, .blue = 1.0f, .colour = 0xFF000020U};
static const Vector3 data_3A968 = {.x = -27.532101f, .y = 9.2975f, .z = 49.022202f};
static const Vector3 data_3A974 = {.x = -77.909798f, .y = 7.1224f, .z = 64.488998f};
static const Vector3 data_3A980 = {.x = 29.400999f, .y = 3.7988f, .z = 23.5072f};
static const Vector3 data_3A98C = {.x = 23.9245f, .y = 3.7983f, .z = 21.183201f};
static const Vector3 data_3A998 = {.x = -72.2313f, .y = 3.7998f, .z = 60.132702f};
static const Vector3 data_3A9A4 = {.x = -7.4819f, .y = 3.7965f, .z = 32.121201f};
static const Vector3i data_3A9B0 = {.x = 0, .y = 32768, .z = 0};
static const Vector3i data_3A9BC = {.x = 0, .y = -32768, .z = 0};
static const Vector3i data_3A9C8 = {.x = 0, .y = -21353, .z = 0};
static const Vector3i data_3A9D4 = {.x = 0, .y = 28061, .z = 0};
static const Vector3i data_3A9E0 = {.x = 0, .y = 19660, .z = 0};
static const Vector3i data_3A9EC = {.x = 0, .y = -27925, .z = 0};
typedef struct Row_3A9F8 { uint32_t w0; float w1; float w2; } Row_3A9F8;
static const Row_3A9F8 data_3A9F8[2] = {
  {0x42189B2DU, 0.90358f, 10.08923f},
  {0x4218DC1CU, 0.90821f, 10.06523f}
};
static const Vector3 data_3AA10 = {.x = 0.0f, .y = -10.0f, .z = 100.0f};
static const int32_t data_3AA1C[6] = {0, -24386, 0, 0, -19859, 0};
static const Vector3 data_3AA34 = {.x = 38.161098f, .y = 0.972f, .z = 10.1892f};
static const Vector3i data_3AA40 = {.x = 0, .y = 11666, .z = 0};
static const Vector3 data_3AA4C = {.x = 38.2248f, .y = 0.9712f, .z = 10.056f};
static const Vector3i data_3AA58 = {.x = 0, .y = 44220, .z = 0};
static const Vector3i data_3AA64 = {.x = 10740, .y = -910, .z = -10012};
static const Vector3i data_3AA70 = {.x = -4915, .y = -1820, .z = 3640};
static const uint16_t data_3AA7C[8] = {1068, 1043, 1069, 1071, 1074, 1072, 1073, 903};
static const char data_3AA8C[20] __attribute__((scn_string_literal)) = "TUNED FreeTalk def\012";
static const char data_3AAA0[20] __attribute__((scn_string_literal)) = "TUNED FreeTalk def\012";
static DoorMotionSelectorBlock data_3AAB4[38] = {
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
static ScnDoorMotionDescriptor data_3B0A4[72] = {
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
static uint32_t data_3BAC4 = 0xFFFFFFFF;
typedef struct Row_3BAC8 { float w0; float w1; float w2; float w3; int32_t w4; } Row_3BAC8;
static const Row_3BAC8 data_3BAC8[7] = {
  {0.0f, 0.0f, 0.0f, 0.0f, -1},
  {0.0f, 0.0f, 0.0f, 0.0f, -1},
  {0.0f, 0.0f, 0.0f, 0.0f, 1534},
  {26.0f, 55.0f, 45.0f, 75.0f, 1555},
  {61.0f, 146.0f, 45.0f, 50.0f, 1554},
  {59.0f, 124.0f, 45.0f, 70.0f, 1543},
  {33.0f, 49.0f, 45.0f, 50.0f, 1543}
};
static const float data_3BB54[4] = {33.0f, 49.0f, 50.0f, 50.0f};
static void (*data_3BB64)(int32_t, int32_t, int32_t) = script_80F0;
static void (*const data_3BB68)(int32_t, int32_t, int32_t, int32_t) = script_8100;
static void (*const data_3BB6C)(int32_t, int32_t, int32_t, int32_t) = script_8100;
static void (*const data_3BB70)(int32_t, int32_t, int32_t, int32_t) = script_8100;
static void (*const data_3BB74)(int32_t, int32_t, int32_t, int32_t) = script_8100;
static void (*const data_3BB78)(int32_t, int32_t, int32_t, int32_t) = script_8100;
static void (*const data_3BB7C)(int32_t, int32_t, int32_t, int32_t) = script_8100;
static void (*const data_3BB80)(int32_t, int32_t, int32_t, int32_t) = script_8100;
static void (*const data_3BB84)(int32_t, int32_t, int32_t, int32_t) = script_8100;
static void (*const data_3BB88)(int32_t, int32_t, int32_t, int32_t) = script_83A6;
static void (*const data_3BB8C)(int32_t, int32_t, int32_t, int32_t) = script_8507;
static void (*const data_3BB90)(int32_t, int32_t, int32_t, int32_t) = script_89D4;
static void (*const data_3BB94)(int32_t, int32_t, int32_t, int32_t) = script_8A3B;
static void (*const data_3BB98)(int32_t, int32_t, int32_t, int32_t) = script_8204;
static void (*const data_3BB9C)(int32_t, int32_t, int32_t, int32_t) = script_8204;
static void (*const data_3BBA0)(int32_t, int32_t, int32_t, int32_t) = script_8668;
static void (*const data_3BBA4)(int32_t, int32_t, int32_t, int32_t) = script_8906;
static void (*const data_3BBA8)(int32_t, int32_t, int32_t, int32_t) = script_8AA2;
static void (*const data_3BBAC)(int32_t, int32_t, int32_t, int32_t) = script_83A6;
static void (*const data_3BBB0)(int32_t, int32_t, int32_t, int32_t) = script_8507;
static void (*const data_3BBB4)(int32_t, int32_t, int32_t, int32_t) = script_8668;
static void (*const data_3BBB8)(int32_t, int32_t, int32_t, int32_t) = script_83A6;
static void (*const data_3BBBC)(int32_t, int32_t, int32_t, int32_t) = script_8507;
static void (*const data_3BBC0)(int32_t, int32_t, int32_t, int32_t) = script_8668;
static void (*const data_3BBC4)(int32_t, int32_t, int32_t, int32_t) = script_8AE9;
static void (*const data_3BBC8)(int32_t, int32_t, int32_t, int32_t) = script_8AE9;
static void (*const data_3BBCC)(int32_t, int32_t, int32_t, int32_t) = script_8AE9;
static void (*const data_3BBD0)(int32_t, int32_t, int32_t, int32_t) = script_8AE9;
static void (*const data_3BBD4)(int32_t, int32_t, int32_t, int32_t) = script_8C1F;
static void (*const data_3BBD8)(int32_t, int32_t, int32_t, int32_t) = script_94E0;
static void (*const data_3BBDC)(int32_t, int32_t, int32_t, int32_t) = script_8100;
static void (*const data_3BBE0)(int32_t, int32_t, int32_t, int32_t) = script_8100;
static void (*const data_3BBE4)(int32_t, int32_t, int32_t, int32_t) = script_8100;
static void (*const data_3BBE8)(int32_t, int32_t, int32_t, int32_t) = script_8100;
static void (*const data_3BBEC)(int32_t, int32_t, int32_t, int32_t) = script_8100;
static void (*const data_3BBF0)(int32_t, int32_t, int32_t, int32_t) = script_8100;
static void (*const data_3BBF4)(int32_t, int32_t, int32_t, int32_t) = script_8100;
static void (*const data_3BBF8)(int32_t, int32_t, int32_t, int32_t) = script_8100;
static const char data_3BBFC[46] __attribute__((scn_string_literal)) = "Door Index = %d : call_type = %d : type = %d\012";
static const char data_3BC2A[43] __attribute__((scn_string_literal)) = "Door Open Motion = DEB_AKESIME_FUSUMA_L_F\012";
static const char data_3BC55[43] __attribute__((scn_string_literal)) = "Door Open Motion = DEB_AKESIME_FUSUMA_R_F\012";
static const char data_3BC80[42] __attribute__((scn_string_literal)) = "Door Open Motion = DEB_HIKISIME_DOOR_L_F\012";
static const char data_3BCAA[42] __attribute__((scn_string_literal)) = "Door Open Motion = DEB_HIKISIME_DOOR_R_F\012";
static const char data_3BCD4[41] __attribute__((scn_string_literal)) = "Door Open Motion = DEB_OSISIME_DOOR_L_F\012";
static const char data_3BCFD[41] __attribute__((scn_string_literal)) = "Door Open Motion = DEB_OSISIME_DOOR_R_F\012";
static const char data_3BD26[43] __attribute__((scn_string_literal)) = "Door Open Motion = GAK_AKESIME_FUSUMA_L_F\012";
static const char data_3BD51[43] __attribute__((scn_string_literal)) = "Door Open Motion = GAK_AKESIME_FUSUMA_R_F\012";
static const char data_3BD7C[42] __attribute__((scn_string_literal)) = "Door Open Motion = GAK_HIKISIME_DOOR_L_F\012";
static const char data_3BDA6[42] __attribute__((scn_string_literal)) = "Door Open Motion = GAK_HIKISIME_DOOR_R_F\012";
static const char data_3BDD0[41] __attribute__((scn_string_literal)) = "Door Open Motion = GAK_OSISIME_DOOR_L_F\012";
static const char data_3BDF9[41] __attribute__((scn_string_literal)) = "Door Open Motion = GAK_OSISIME_DOOR_R_F\012";
static const char data_3BE22[43] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_AKESIME_FUSUMA_L_F\012";
static const char data_3BE4D[43] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_AKESIME_FUSUMA_R_F\012";
static const char data_3BE78[42] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_HIKISIME_DOOR_L_F\012";
static const char data_3BEA2[42] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_HIKISIME_DOOR_R_F\012";
static const char data_3BECC[41] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_OSISIME_DOOR_L_F\012";
static const char data_3BEF5[41] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_OSISIME_DOOR_R_F\012";
static const char data_3BF1E[43] __attribute__((scn_string_literal)) = "Door Open Motion = KOD_AKESIME_FUSUMA_L_F\012";
static const char data_3BF49[43] __attribute__((scn_string_literal)) = "Door Open Motion = KOD_AKESIME_FUSUMA_R_F\012";
static const char data_3BF74[42] __attribute__((scn_string_literal)) = "Door Open Motion = KOD_HIKISIME_DOOR_L_F\012";
static const char data_3BF9E[42] __attribute__((scn_string_literal)) = "Door Open Motion = KOD_HIKISIME_DOOR_R_F\012";
static const char data_3BFC8[41] __attribute__((scn_string_literal)) = "Door Open Motion = KOD_OSISIME_DOOR_L_F\012";
static const char data_3BFF1[41] __attribute__((scn_string_literal)) = "Door Open Motion = KOD_OSISIME_DOOR_R_F\012";
static const char data_3C01A[43] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_AKESIME_FUSUMA_L_F\012";
static const char data_3C045[43] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_AKESIME_FUSUMA_R_F\012";
static const char data_3C070[42] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_HIKISIME_DOOR_L_F\012";
static const char data_3C09A[42] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_HIKISIME_DOOR_R_F\012";
static const char data_3C0C4[41] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_OSISIME_DOOR_L_F\012";
static const char data_3C0ED[41] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_OSISIME_DOOR_R_F\012";
static const char data_3C116[43] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_AKESIME_FUSUMA_L_F\012";
static const char data_3C141[43] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_AKESIME_FUSUMA_R_F\012";
static const char data_3C16C[42] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_HIKISIME_DOOR_L_F\012";
static const char data_3C196[42] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_HIKISIME_DOOR_R_F\012";
static const char data_3C1C0[41] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_OSISIME_DOOR_L_F\012";
static const char data_3C1E9[41] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_OSISIME_DOOR_R_F\012";
static const char data_3C212[43] __attribute__((scn_string_literal)) = "Door Open Motion = SIN_AKESIME_FUSUMA_L_F\012";
static const char data_3C23D[43] __attribute__((scn_string_literal)) = "Door Open Motion = SIN_AKESIME_FUSUMA_R_F\012";
static const char data_3C268[42] __attribute__((scn_string_literal)) = "Door Open Motion = SIN_HIKISIME_DOOR_L_F\012";
static const char data_3C292[42] __attribute__((scn_string_literal)) = "Door Open Motion = SIN_HIKISIME_DOOR_R_F\012";
static const char data_3C2BC[41] __attribute__((scn_string_literal)) = "Door Open Motion = SIN_OSISIME_DOOR_L_F\012";
static const char data_3C2E5[41] __attribute__((scn_string_literal)) = "Door Open Motion = SIN_OSISIME_DOOR_R_F\012";
static const char data_3C30E[43] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_AKESIME_FUSUMA_L_F\012";
static const char data_3C339[43] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_AKESIME_FUSUMA_R_F\012";
static const char data_3C364[42] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_HIKISIME_DOOR_L_F\012";
static const char data_3C38E[42] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_HIKISIME_DOOR_R_F\012";
static const char data_3C3B8[41] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_OSISIME_DOOR_L_F\012";
static const char data_3C3E1[41] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_OSISIME_DOOR_R_F\012";
static const char data_3C40A[43] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_AKESIME_FUSUMA_L_F\012";
static const char data_3C435[43] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_AKESIME_FUSUMA_R_F\012";
static const char data_3C460[42] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_HIKISIME_DOOR_L_F\012";
static const char data_3C48A[42] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_HIKISIME_DOOR_R_F\012";
static const char data_3C4B4[41] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_OSISIME_DOOR_L_F\012";
static const char data_3C4DD[41] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_OSISIME_DOOR_R_F\012";
static const char data_3C506[44] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_OSISIME_DOOR_AP_L_F\012";
static const char data_3C532[44] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_OSISIME_DOOR_AP_L_F\012";
static const char data_3C55E[44] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_OSISIME_DOOR_AP_L_F\012";
static const char data_3C58A[44] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_OSISIME_DOOR_AP_L_F\012";
static const char data_3C5B6[44] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_OSISIME_DOOR_AP_L_F\012";
static const char data_3C5E2[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_3C5FE[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_3C61A[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_3C636[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_3C652[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_3C66E[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_3C68A[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_3C6A6[13] __attribute__((scn_string_literal)) = "open-move!!\012";
static const char data_3C6B3[11] __attribute__((scn_string_literal)) = "M_MDOR.BIN";
static const char data_3C6BE[] __attribute__((scn_string_literal)) = "This Charactor Work is NULL!!\n";
static const char data_3C6DD[22] __attribute__((scn_string_literal)) = "手動ドア開け番号指定\012";
static const char data_3C6F3[34] __attribute__((scn_string_literal)) = "無限ループになりそうなので抜ける\012";
static const char data_3C715[] __attribute__((scn_string_literal)) = "Don't make BOX Buffer!!\n";
static const char data_3C72E[] __attribute__((scn_string_literal)) = "Don't make BOX Buffer!!\n";
static const char data_3C747[15] __attribute__((scn_string_literal)) = "Door System : ";
static const char data_3C756[15] __attribute__((scn_string_literal)) = "Door System : ";
static const char data_3C765[18] __attribute__((scn_string_literal)) = "Door Box is ON!!\012";
static const char data_3C777[19] __attribute__((scn_string_literal)) = "Door Box is OFF!!\012";
static const char data_3C78A[18] __attribute__((scn_string_literal)) = "Door Box is ON!!\012";
static const char data_3C79C[20] __attribute__((scn_string_literal)) = "Door Box is OFF!!\012";
static const DSWKDoorEntry data_3C7B0[65] = {
  {.slot = 0, .flags = 0, .ctrl_mask_pair = 0x640064U, .coarse_metric = 0.0f, .selection_metric = 0.0f, .view_ang_delta = 0, .group = 0, .global1 = 0xFFFFFFFFU, .global2 = 0xFFFFFFFFU, .global3 = 0xFFFFFFFFU, .sndCommandId = 0xFFFFFFFFU, .global5 = 0xFFFFFFFFU, .global6 = 0xFFFFFFFFU},
  {.slot = 3, .flags = 0, .ctrl_mask_pair = 0x640064U, .coarse_metric = 0.0f, .selection_metric = 0.0f, .view_ang_delta = 0, .group = 0, .global1 = 0xFFFFFFFFU, .global2 = 0xFFFFFFFFU, .global3 = 0xFFFFFFFFU, .sndCommandId = 0xFFFFFFFFU, .global5 = 0xFFFFFFFFU, .global6 = 0xFFFFFFFFU},
  {.slot = 6, .flags = 0, .ctrl_mask_pair = 0x640064U, .coarse_metric = 0.0f, .selection_metric = 0.0f, .view_ang_delta = 0, .group = 0, .global1 = 0xFFFFFFFFU, .global2 = 0xFFFFFFFFU, .global3 = 0xFFFFFFFFU, .sndCommandId = 0xFFFFFFFFU, .global5 = 0xFFFFFFFFU, .global6 = 0xFFFFFFFFU},
  {.slot = 8, .flags = 0, .ctrl_mask_pair = 0x640064U, .coarse_metric = 0.0f, .selection_metric = 0.0f, .view_ang_delta = 0, .group = 0, .global1 = 0xFFFFFFFFU, .global2 = 0xFFFFFFFFU, .global3 = 0xFFFFFFFFU, .sndCommandId = 0xFFFFFFFFU, .global5 = 0xFFFFFFFFU, .global6 = 0xFFFFFFFFU},
  {.slot = 11, .flags = 0, .ctrl_mask_pair = 0x640064U, .coarse_metric = 0.0f, .selection_metric = 0.0f, .view_ang_delta = 0, .group = 0, .global1 = 0xFFFFFFFFU, .global2 = 0xFFFFFFFFU, .global3 = 0xFFFFFFFFU, .sndCommandId = 0xFFFFFFFFU, .global5 = 0xFFFFFFFFU, .global6 = 0xFFFFFFFFU},
  {.slot = 14, .flags = 0, .ctrl_mask_pair = 0x640064U, .coarse_metric = 0.0f, .selection_metric = 0.0f, .view_ang_delta = 0, .group = 0, .global1 = 0xFFFFFFFFU, .global2 = 0xFFFFFFFFU, .global3 = 0xFFFFFFFFU, .sndCommandId = 0xFFFFFFFFU, .global5 = 0xFFFFFFFFU, .global6 = 0xFFFFFFFFU},
  {.slot = 17, .flags = 0, .ctrl_mask_pair = 0x640064U, .coarse_metric = 0.0f, .selection_metric = 0.0f, .view_ang_delta = 0, .group = 0, .global1 = 0xFFFFFFFFU, .global2 = 0xFFFFFFFFU, .global3 = 0xFFFFFFFFU, .sndCommandId = 0xFFFFFFFFU, .global5 = 0xFFFFFFFFU, .global6 = 0xFFFFFFFFU},
  {.slot = 20, .flags = 0, .ctrl_mask_pair = 0x640064U, .coarse_metric = 0.0f, .selection_metric = 0.0f, .view_ang_delta = 0, .group = 0, .global1 = 0xFFFFFFFFU, .global2 = 0xFFFFFFFFU, .global3 = 0xFFFFFFFFU, .sndCommandId = 0xFFFFFFFFU, .global5 = 0xFFFFFFFFU, .global6 = 0xFFFFFFFFU},
  {.slot = 23, .flags = 0, .ctrl_mask_pair = 0x640064U, .coarse_metric = 0.0f, .selection_metric = 0.0f, .view_ang_delta = 0, .group = 0, .global1 = 0xFFFFFFFFU, .global2 = 0xFFFFFFFFU, .global3 = 0xFFFFFFFFU, .sndCommandId = 0xFFFFFFFFU, .global5 = 0xFFFFFFFFU, .global6 = 0xFFFFFFFFU},
  {.slot = 26, .flags = 0, .ctrl_mask_pair = 0x640064U, .coarse_metric = 0.0f, .selection_metric = 0.0f, .view_ang_delta = 0, .group = 0, .global1 = 0xFFFFFFFFU, .global2 = 0xFFFFFFFFU, .global3 = 0xFFFFFFFFU, .sndCommandId = 0xFFFFFFFFU, .global5 = 0xFFFFFFFFU, .global6 = 0xFFFFFFFFU},
  {.slot = 29, .flags = 0, .ctrl_mask_pair = 0x640064U, .coarse_metric = 0.0f, .selection_metric = 0.0f, .view_ang_delta = 0, .group = 0, .global1 = 0xFFFFFFFFU, .global2 = 0xFFFFFFFFU, .global3 = 0xFFFFFFFFU, .sndCommandId = 0xFFFFFFFFU, .global5 = 0xFFFFFFFFU, .global6 = 0xFFFFFFFFU},
  {.slot = 32, .flags = 0, .ctrl_mask_pair = 0x640064U, .coarse_metric = 0.0f, .selection_metric = 0.0f, .view_ang_delta = 0, .group = 0, .global1 = 0xFFFFFFFFU, .global2 = 0xFFFFFFFFU, .global3 = 0xFFFFFFFFU, .sndCommandId = 0xFFFFFFFFU, .global5 = 0xFFFFFFFFU, .global6 = 0xFFFFFFFFU},
  {.slot = 35, .flags = 0, .ctrl_mask_pair = 0x640064U, .coarse_metric = 0.0f, .selection_metric = 0.0f, .view_ang_delta = 0, .group = 0, .global1 = 0xFFFFFFFFU, .global2 = 0xFFFFFFFFU, .global3 = 0xFFFFFFFFU, .sndCommandId = 0xFFFFFFFFU, .global5 = 0xFFFFFFFFU, .global6 = 0xFFFFFFFFU},
  {.slot = 38, .flags = 0, .ctrl_mask_pair = 0x640064U, .coarse_metric = 0.0f, .selection_metric = 0.0f, .view_ang_delta = 0, .group = 0, .global1 = 0xFFFFFFFFU, .global2 = 0xFFFFFFFFU, .global3 = 0xFFFFFFFFU, .sndCommandId = 0xFFFFFFFFU, .global5 = 0xFFFFFFFFU, .global6 = 0xFFFFFFFFU},
  {.slot = 41, .flags = 0, .ctrl_mask_pair = 0x640064U, .coarse_metric = 0.0f, .selection_metric = 0.0f, .view_ang_delta = 0, .group = 0, .global1 = 0xFFFFFFFFU, .global2 = 0xFFFFFFFFU, .global3 = 0xFFFFFFFFU, .sndCommandId = 0xFFFFFFFFU, .global5 = 0xFFFFFFFFU, .global6 = 0xFFFFFFFFU},
  {.slot = 44, .flags = 0, .ctrl_mask_pair = 0x640064U, .coarse_metric = 0.0f, .selection_metric = 0.0f, .view_ang_delta = 0, .group = 0, .global1 = 0xFFFFFFFFU, .global2 = 0xFFFFFFFFU, .global3 = 0xFFFFFFFFU, .sndCommandId = 0xFFFFFFFFU, .global5 = 0xFFFFFFFFU, .global6 = 0xFFFFFFFFU},
  {.slot = 47, .flags = 0, .ctrl_mask_pair = 0x640064U, .coarse_metric = 0.0f, .selection_metric = 0.0f, .view_ang_delta = 0, .group = 0, .global1 = 0xFFFFFFFFU, .global2 = 0xFFFFFFFFU, .global3 = 0xFFFFFFFFU, .sndCommandId = 0xFFFFFFFFU, .global5 = 0xFFFFFFFFU, .global6 = 0xFFFFFFFFU},
  {.slot = 50, .flags = 0, .ctrl_mask_pair = 0x640064U, .coarse_metric = 0.0f, .selection_metric = 0.0f, .view_ang_delta = 0, .group = 0, .global1 = 0xFFFFFFFFU, .global2 = 0xFFFFFFFFU, .global3 = 0xFFFFFFFFU, .sndCommandId = 0xFFFFFFFFU, .global5 = 0xFFFFFFFFU, .global6 = 0xFFFFFFFFU},
  {.slot = 53, .flags = 0, .ctrl_mask_pair = 0x640064U, .coarse_metric = 0.0f, .selection_metric = 0.0f, .view_ang_delta = 0, .group = 0, .global1 = 0xFFFFFFFFU, .global2 = 0xFFFFFFFFU, .global3 = 0xFFFFFFFFU, .sndCommandId = 0xFFFFFFFFU, .global5 = 0xFFFFFFFFU, .global6 = 0xFFFFFFFFU},
  {.slot = 56, .flags = 0, .ctrl_mask_pair = 0x640064U, .coarse_metric = 0.0f, .selection_metric = 0.0f, .view_ang_delta = 0, .group = 0, .global1 = 0xFFFFFFFFU, .global2 = 0xFFFFFFFFU, .global3 = 0xFFFFFFFFU, .sndCommandId = 0xFFFFFFFFU, .global5 = 0xFFFFFFFFU, .global6 = 0xFFFFFFFFU},
  {.slot = 59, .flags = 0, .ctrl_mask_pair = 0x640064U, .coarse_metric = 0.0f, .selection_metric = 0.0f, .view_ang_delta = 0, .group = 0, .global1 = 0xFFFFFFFFU, .global2 = 0xFFFFFFFFU, .global3 = 0xFFFFFFFFU, .sndCommandId = 0xFFFFFFFFU, .global5 = 0xFFFFFFFFU, .global6 = 0xFFFFFFFFU},
  {.slot = 62, .flags = 0, .ctrl_mask_pair = 0x640064U, .coarse_metric = 0.0f, .selection_metric = 0.0f, .view_ang_delta = 0, .group = 0, .global1 = 0xFFFFFFFFU, .global2 = 0xFFFFFFFFU, .global3 = 0xFFFFFFFFU, .sndCommandId = 0xFFFFFFFFU, .global5 = 0xFFFFFFFFU, .global6 = 0xFFFFFFFFU},
  {.slot = 65, .flags = 0, .ctrl_mask_pair = 0x640064U, .coarse_metric = 0.0f, .selection_metric = 0.0f, .view_ang_delta = 0, .group = 0, .global1 = 0xFFFFFFFFU, .global2 = 0xFFFFFFFFU, .global3 = 0xFFFFFFFFU, .sndCommandId = 0xFFFFFFFFU, .global5 = 0xFFFFFFFFU, .global6 = 0xFFFFFFFFU},
  {.slot = 68, .flags = 0, .ctrl_mask_pair = 0x640064U, .coarse_metric = 0.0f, .selection_metric = 0.0f, .view_ang_delta = 0, .group = 0, .global1 = 0xFFFFFFFFU, .global2 = 0xFFFFFFFFU, .global3 = 0xFFFFFFFFU, .sndCommandId = 0xFFFFFFFFU, .global5 = 0xFFFFFFFFU, .global6 = 0xFFFFFFFFU},
  {.slot = 71, .flags = 0, .ctrl_mask_pair = 0x640064U, .coarse_metric = 0.0f, .selection_metric = 0.0f, .view_ang_delta = 0, .group = 0, .global1 = 0xFFFFFFFFU, .global2 = 0xFFFFFFFFU, .global3 = 0xFFFFFFFFU, .sndCommandId = 0xFFFFFFFFU, .global5 = 0xFFFFFFFFU, .global6 = 0xFFFFFFFFU},
  {.slot = 74, .flags = 0, .ctrl_mask_pair = 0x640064U, .coarse_metric = 0.0f, .selection_metric = 0.0f, .view_ang_delta = 0, .group = 0, .global1 = 0xFFFFFFFFU, .global2 = 0xFFFFFFFFU, .global3 = 0xFFFFFFFFU, .sndCommandId = 0xFFFFFFFFU, .global5 = 0xFFFFFFFFU, .global6 = 0xFFFFFFFFU},
  {.slot = 77, .flags = 0, .ctrl_mask_pair = 0x640064U, .coarse_metric = 0.0f, .selection_metric = 0.0f, .view_ang_delta = 0, .group = 0, .global1 = 0xFFFFFFFFU, .global2 = 0xFFFFFFFFU, .global3 = 0xFFFFFFFFU, .sndCommandId = 0xFFFFFFFFU, .global5 = 0xFFFFFFFFU, .global6 = 0xFFFFFFFFU},
  {.slot = 80, .flags = 0, .ctrl_mask_pair = 0x640064U, .coarse_metric = 0.0f, .selection_metric = 0.0f, .view_ang_delta = 0, .group = 0, .global1 = 0xFFFFFFFFU, .global2 = 0xFFFFFFFFU, .global3 = 0xFFFFFFFFU, .sndCommandId = 0xFFFFFFFFU, .global5 = 0xFFFFFFFFU, .global6 = 0xFFFFFFFFU},
  {.slot = 82, .flags = 0, .ctrl_mask_pair = 0x640064U, .coarse_metric = 0.0f, .selection_metric = 0.0f, .view_ang_delta = 0, .group = 0, .global1 = 0xFFFFFFFFU, .global2 = 0xFFFFFFFFU, .global3 = 0xFFFFFFFFU, .sndCommandId = 0xFFFFFFFFU, .global5 = 0xFFFFFFFFU, .global6 = 0xFFFFFFFFU},
  {.slot = 85, .flags = 0, .ctrl_mask_pair = 0x6E006EU, .coarse_metric = 0.0f, .selection_metric = 0.0f, .view_ang_delta = 0, .group = 0, .global1 = 0xFFFFFFFFU, .global2 = 0xFFFFFFFFU, .global3 = 0xFFFFFFFFU, .sndCommandId = 0xFFFFFFFFU, .global5 = 0xFFFFFFFFU, .global6 = 0xFFFFFFFFU},
  {.slot = 87, .flags = 0, .ctrl_mask_pair = 0x640064U, .coarse_metric = 0.0f, .selection_metric = 0.0f, .view_ang_delta = 0, .group = 0, .global1 = 0xFFFFFFFFU, .global2 = 0xFFFFFFFFU, .global3 = 0xFFFFFFFFU, .sndCommandId = 0xFFFFFFFFU, .global5 = 0xFFFFFFFFU, .global6 = 0xFFFFFFFFU},
  {.slot = 90, .flags = 0, .ctrl_mask_pair = 0x640064U, .coarse_metric = 0.0f, .selection_metric = 0.0f, .view_ang_delta = 0, .group = 0, .global1 = 0xFFFFFFFFU, .global2 = 0xFFFFFFFFU, .global3 = 0xFFFFFFFFU, .sndCommandId = 0xFFFFFFFFU, .global5 = 0xFFFFFFFFU, .global6 = 0xFFFFFFFFU},
  {.slot = 93, .flags = 0, .ctrl_mask_pair = 0x640064U, .coarse_metric = 0.0f, .selection_metric = 0.0f, .view_ang_delta = 0, .group = 0, .global1 = 0xFFFFFFFFU, .global2 = 0xFFFFFFFFU, .global3 = 0xFFFFFFFFU, .sndCommandId = 0xFFFFFFFFU, .global5 = 0xFFFFFFFFU, .global6 = 0xFFFFFFFFU},
  {.slot = 96, .flags = 0, .ctrl_mask_pair = 0x640064U, .coarse_metric = 0.0f, .selection_metric = 0.0f, .view_ang_delta = 0, .group = 0, .global1 = 0xFFFFFFFFU, .global2 = 0xFFFFFFFFU, .global3 = 0xFFFFFFFFU, .sndCommandId = 0xFFFFFFFFU, .global5 = 0xFFFFFFFFU, .global6 = 0xFFFFFFFFU},
  {.slot = 99, .flags = 0, .ctrl_mask_pair = 0x640064U, .coarse_metric = 0.0f, .selection_metric = 0.0f, .view_ang_delta = 0, .group = 0, .global1 = 0xFFFFFFFFU, .global2 = 0xFFFFFFFFU, .global3 = 0xFFFFFFFFU, .sndCommandId = 0xFFFFFFFFU, .global5 = 0xFFFFFFFFU, .global6 = 0xFFFFFFFFU},
  {.slot = 102, .flags = 0, .ctrl_mask_pair = 0x640064U, .coarse_metric = 0.0f, .selection_metric = 0.0f, .view_ang_delta = 0, .group = 0, .global1 = 0xFFFFFFFFU, .global2 = 0xFFFFFFFFU, .global3 = 0xFFFFFFFFU, .sndCommandId = 0xFFFFFFFFU, .global5 = 0xFFFFFFFFU, .global6 = 0xFFFFFFFFU},
  {.slot = 104, .flags = 0, .ctrl_mask_pair = 0x640064U, .coarse_metric = 0.0f, .selection_metric = 0.0f, .view_ang_delta = 0, .group = 0, .global1 = 0xFFFFFFFFU, .global2 = 0xFFFFFFFFU, .global3 = 0xFFFFFFFFU, .sndCommandId = 0xFFFFFFFFU, .global5 = 0xFFFFFFFFU, .global6 = 0xFFFFFFFFU},
  {.slot = 106, .flags = 0, .ctrl_mask_pair = 0x640064U, .coarse_metric = 0.0f, .selection_metric = 0.0f, .view_ang_delta = 0, .group = 0, .global1 = 0xFFFFFFFFU, .global2 = 0xFFFFFFFFU, .global3 = 0xFFFFFFFFU, .sndCommandId = 0xFFFFFFFFU, .global5 = 0xFFFFFFFFU, .global6 = 0xFFFFFFFFU},
  {.slot = 109, .flags = 0, .ctrl_mask_pair = 0x640064U, .coarse_metric = 0.0f, .selection_metric = 0.0f, .view_ang_delta = 0, .group = 0, .global1 = 0xFFFFFFFFU, .global2 = 0xFFFFFFFFU, .global3 = 0xFFFFFFFFU, .sndCommandId = 0xFFFFFFFFU, .global5 = 0xFFFFFFFFU, .global6 = 0xFFFFFFFFU},
  {.slot = 111, .flags = 0, .ctrl_mask_pair = 0x640064U, .coarse_metric = 0.0f, .selection_metric = 0.0f, .view_ang_delta = 0, .group = 0, .global1 = 0xFFFFFFFFU, .global2 = 0xFFFFFFFFU, .global3 = 0xFFFFFFFFU, .sndCommandId = 0xFFFFFFFFU, .global5 = 0xFFFFFFFFU, .global6 = 0xFFFFFFFFU},
  {.slot = 114, .flags = 0, .ctrl_mask_pair = 0x640064U, .coarse_metric = 0.0f, .selection_metric = 0.0f, .view_ang_delta = 0, .group = 0, .global1 = 0xFFFFFFFFU, .global2 = 0xFFFFFFFFU, .global3 = 0xFFFFFFFFU, .sndCommandId = 0xFFFFFFFFU, .global5 = 0xFFFFFFFFU, .global6 = 0xFFFFFFFFU},
  {.slot = 117, .flags = 0, .ctrl_mask_pair = 0x640064U, .coarse_metric = 0.0f, .selection_metric = 0.0f, .view_ang_delta = 0, .group = 0, .global1 = 0xFFFFFFFFU, .global2 = 0xFFFFFFFFU, .global3 = 0xFFFFFFFFU, .sndCommandId = 0xFFFFFFFFU, .global5 = 0xFFFFFFFFU, .global6 = 0xFFFFFFFFU},
  {.slot = 120, .flags = 0, .ctrl_mask_pair = 0x640064U, .coarse_metric = 0.0f, .selection_metric = 0.0f, .view_ang_delta = 0, .group = 0, .global1 = 0xFFFFFFFFU, .global2 = 0xFFFFFFFFU, .global3 = 0xFFFFFFFFU, .sndCommandId = 0xFFFFFFFFU, .global5 = 0xFFFFFFFFU, .global6 = 0xFFFFFFFFU},
  {.slot = 123, .flags = 0, .ctrl_mask_pair = 0x640064U, .coarse_metric = 0.0f, .selection_metric = 0.0f, .view_ang_delta = 0, .group = 0, .global1 = 0xFFFFFFFFU, .global2 = 0xFFFFFFFFU, .global3 = 0xFFFFFFFFU, .sndCommandId = 0xFFFFFFFFU, .global5 = 0xFFFFFFFFU, .global6 = 0xFFFFFFFFU},
  {.slot = 125, .flags = 0, .ctrl_mask_pair = 0x640064U, .coarse_metric = 0.0f, .selection_metric = 0.0f, .view_ang_delta = 0, .group = 0, .global1 = 0xFFFFFFFFU, .global2 = 0xFFFFFFFFU, .global3 = 0xFFFFFFFFU, .sndCommandId = 0xFFFFFFFFU, .global5 = 0xFFFFFFFFU, .global6 = 0xFFFFFFFFU},
  {.slot = 128, .flags = 0, .ctrl_mask_pair = 0x640064U, .coarse_metric = 0.0f, .selection_metric = 0.0f, .view_ang_delta = 0, .group = 0, .global1 = 0xFFFFFFFFU, .global2 = 0xFFFFFFFFU, .global3 = 0xFFFFFFFFU, .sndCommandId = 0xFFFFFFFFU, .global5 = 0xFFFFFFFFU, .global6 = 0xFFFFFFFFU},
  {.slot = 131, .flags = 0, .ctrl_mask_pair = 0x640064U, .coarse_metric = 0.0f, .selection_metric = 0.0f, .view_ang_delta = 0, .group = 0, .global1 = 0xFFFFFFFFU, .global2 = 0xFFFFFFFFU, .global3 = 0xFFFFFFFFU, .sndCommandId = 0xFFFFFFFFU, .global5 = 0xFFFFFFFFU, .global6 = 0xFFFFFFFFU},
  {.slot = 134, .flags = 0, .ctrl_mask_pair = 0x640064U, .coarse_metric = 0.0f, .selection_metric = 0.0f, .view_ang_delta = 0, .group = 0, .global1 = 0xFFFFFFFFU, .global2 = 0xFFFFFFFFU, .global3 = 0xFFFFFFFFU, .sndCommandId = 0xFFFFFFFFU, .global5 = 0xFFFFFFFFU, .global6 = 0xFFFFFFFFU},
  {.slot = 137, .flags = 0, .ctrl_mask_pair = 0x640064U, .coarse_metric = 0.0f, .selection_metric = 0.0f, .view_ang_delta = 0, .group = 0, .global1 = 0xFFFFFFFFU, .global2 = 0xFFFFFFFFU, .global3 = 0xFFFFFFFFU, .sndCommandId = 0xFFFFFFFFU, .global5 = 0xFFFFFFFFU, .global6 = 0xFFFFFFFFU},
  {.slot = 139, .flags = 0, .ctrl_mask_pair = 0x640064U, .coarse_metric = 0.0f, .selection_metric = 0.0f, .view_ang_delta = 0, .group = 0, .global1 = 0xFFFFFFFFU, .global2 = 0xFFFFFFFFU, .global3 = 0xFFFFFFFFU, .sndCommandId = 0xFFFFFFFFU, .global5 = 0xFFFFFFFFU, .global6 = 0xFFFFFFFFU},
  {.slot = 142, .flags = 0, .ctrl_mask_pair = 0x640064U, .coarse_metric = 0.0f, .selection_metric = 0.0f, .view_ang_delta = 0, .group = 0, .global1 = 0xFFFFFFFFU, .global2 = 0xFFFFFFFFU, .global3 = 0xFFFFFFFFU, .sndCommandId = 0xFFFFFFFFU, .global5 = 0xFFFFFFFFU, .global6 = 0xFFFFFFFFU},
  {.slot = 145, .flags = 0, .ctrl_mask_pair = 0x640064U, .coarse_metric = 0.0f, .selection_metric = 0.0f, .view_ang_delta = 0, .group = 0, .global1 = 0xFFFFFFFFU, .global2 = 0xFFFFFFFFU, .global3 = 0xFFFFFFFFU, .sndCommandId = 0xFFFFFFFFU, .global5 = 0xFFFFFFFFU, .global6 = 0xFFFFFFFFU},
  {.slot = 148, .flags = 0, .ctrl_mask_pair = 0x640064U, .coarse_metric = 0.0f, .selection_metric = 0.0f, .view_ang_delta = 0, .group = 0, .global1 = 0xFFFFFFFFU, .global2 = 0xFFFFFFFFU, .global3 = 0xFFFFFFFFU, .sndCommandId = 0xFFFFFFFFU, .global5 = 0xFFFFFFFFU, .global6 = 0xFFFFFFFFU},
  {.slot = 151, .flags = 0, .ctrl_mask_pair = 0x640064U, .coarse_metric = 0.0f, .selection_metric = 0.0f, .view_ang_delta = 0, .group = 0, .global1 = 0xFFFFFFFFU, .global2 = 0xFFFFFFFFU, .global3 = 0xFFFFFFFFU, .sndCommandId = 0xFFFFFFFFU, .global5 = 0xFFFFFFFFU, .global6 = 0xFFFFFFFFU},
  {.slot = 154, .flags = 0, .ctrl_mask_pair = 0x640064U, .coarse_metric = 0.0f, .selection_metric = 0.0f, .view_ang_delta = 0, .group = 0, .global1 = 0xFFFFFFFFU, .global2 = 0xFFFFFFFFU, .global3 = 0xFFFFFFFFU, .sndCommandId = 0xFFFFFFFFU, .global5 = 0xFFFFFFFFU, .global6 = 0xFFFFFFFFU},
  {.slot = 157, .flags = 0, .ctrl_mask_pair = 0x640064U, .coarse_metric = 0.0f, .selection_metric = 0.0f, .view_ang_delta = 0, .group = 0, .global1 = 0xFFFFFFFFU, .global2 = 0xFFFFFFFFU, .global3 = 0xFFFFFFFFU, .sndCommandId = 0xFFFFFFFFU, .global5 = 0xFFFFFFFFU, .global6 = 0xFFFFFFFFU},
  {.slot = 160, .flags = 0, .ctrl_mask_pair = 0x640064U, .coarse_metric = 0.0f, .selection_metric = 0.0f, .view_ang_delta = 0, .group = 0, .global1 = 0xFFFFFFFFU, .global2 = 0xFFFFFFFFU, .global3 = 0xFFFFFFFFU, .sndCommandId = 0xFFFFFFFFU, .global5 = 0xFFFFFFFFU, .global6 = 0xFFFFFFFFU},
  {.slot = 163, .flags = 0, .ctrl_mask_pair = 0x640064U, .coarse_metric = 0.0f, .selection_metric = 0.0f, .view_ang_delta = 0, .group = 0, .global1 = 0xFFFFFFFFU, .global2 = 0xFFFFFFFFU, .global3 = 0xFFFFFFFFU, .sndCommandId = 0xFFFFFFFFU, .global5 = 0xFFFFFFFFU, .global6 = 0xFFFFFFFFU},
  {.slot = 166, .flags = 0, .ctrl_mask_pair = 0x640064U, .coarse_metric = 0.0f, .selection_metric = 0.0f, .view_ang_delta = 0, .group = 0, .global1 = 0xFFFFFFFFU, .global2 = 0xFFFFFFFFU, .global3 = 0xFFFFFFFFU, .sndCommandId = 0xFFFFFFFFU, .global5 = 0xFFFFFFFFU, .global6 = 0xFFFFFFFFU},
  {.slot = 168, .flags = 0, .ctrl_mask_pair = 0x640064U, .coarse_metric = 0.0f, .selection_metric = 0.0f, .view_ang_delta = 0, .group = 0, .global1 = 0xFFFFFFFFU, .global2 = 0xFFFFFFFFU, .global3 = 0xFFFFFFFFU, .sndCommandId = 0xFFFFFFFFU, .global5 = 0xFFFFFFFFU, .global6 = 0xFFFFFFFFU},
  {.slot = 171, .flags = 0, .ctrl_mask_pair = 0x640064U, .coarse_metric = 0.0f, .selection_metric = 0.0f, .view_ang_delta = 0, .group = 0, .global1 = 0xFFFFFFFFU, .global2 = 0xFFFFFFFFU, .global3 = 0xFFFFFFFFU, .sndCommandId = 0xFFFFFFFFU, .global5 = 0xFFFFFFFFU, .global6 = 0xFFFFFFFFU},
  {.slot = 174, .flags = 0, .ctrl_mask_pair = 0x640064U, .coarse_metric = 0.0f, .selection_metric = 0.0f, .view_ang_delta = 0, .group = 0, .global1 = 0xFFFFFFFFU, .global2 = 0xFFFFFFFFU, .global3 = 0xFFFFFFFFU, .sndCommandId = 0xFFFFFFFFU, .global5 = 0xFFFFFFFFU, .global6 = 0xFFFFFFFFU},
  {.slot = 177, .flags = 0, .ctrl_mask_pair = 0x640064U, .coarse_metric = 0.0f, .selection_metric = 0.0f, .view_ang_delta = 0, .group = 0, .global1 = 0xFFFFFFFFU, .global2 = 0xFFFFFFFFU, .global3 = 0xFFFFFFFFU, .sndCommandId = 0xFFFFFFFFU, .global5 = 0xFFFFFFFFU, .global6 = 0xFFFFFFFFU},
  {.slot = 180, .flags = 0, .ctrl_mask_pair = 0x640064U, .coarse_metric = 0.0f, .selection_metric = 0.0f, .view_ang_delta = 0, .group = 0, .global1 = 0xFFFFFFFFU, .global2 = 0xFFFFFFFFU, .global3 = 0xFFFFFFFFU, .sndCommandId = 0xFFFFFFFFU, .global5 = 0xFFFFFFFFU, .global6 = 0xFFFFFFFFU},
  {.slot = 183, .flags = 0, .ctrl_mask_pair = 0x650065U, .coarse_metric = 0.0f, .selection_metric = 0.0f, .view_ang_delta = 0, .group = 0, .global1 = 0xFFFFFFFFU, .global2 = 0xFFFFFFFFU, .global3 = 0xFFFFFFFFU, .sndCommandId = 0xFFFFFFFFU, .global5 = 0xFFFFFFFFU, .global6 = 0xFFFFFFFFU}
};
static const int32_t data_3D4E4[13] = {-1, 0, 6553700, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1};
static const uint32_t data_3D518 = 98;
static const int32_t data_3D51C[184] = {
  99, -1, 78, 79, -1, 90, -1, 40, 41, -1, 60, 61, -1, 62, 63, -1, 64, 65, -1, 66, 67, -1, 68, 69, -1, 70, 71, -1, 72, 73, -1, 74, 75, -1, 42, 43, -1,
  44, 45, -1, 46, 47, -1, 48, 49, -1, 50, 51, -1, 86, 87, -1, 52, 53, -1, 54, 55, -1, 80, 81, -1, 56, 57, -1, 84, 85, -1, 58, 59, -1, 82, 83, -1, 21,
  22, -1, 19, 20, -1, 97, -1, 9, 10, -1, 91, -1, 5, 6, -1, 36, 37, -1, 33, 34, -1, 76, 77, -1, 116, 117, -1, 35, -1, 4, -1, 7, 8, -1, 32, -1, 106,
  107, -1, 30, 31, -1, 2, 3, -1, 92, 93, -1, 94, -1, 17, 18, -1, 15, 16, -1, 13, 14, -1, 11, 12, -1, 29, -1, 108, 109, -1, 110, 111, -1, 102, 103, -1,
  118, 119, -1, 0, 1, -1, 112, 113, -1, 95, 96, -1, 100, 101, -1, 25, 26, -1, 27, -1, 114, 115, -1, 38, 39, -1, 104, 105, -1, 23, 24, -1, 88, 89, -1,
  28, -1
};
static char data_3D7FC[] = "DR01_011";
static char data_3D805[] = "DR00_002";
static char data_3D80E[] = "DR05_001";
static char data_3D817[] = "DR02_012";
static char data_3D820[] = "DR02_016";
static char data_3D829[] = "DR00_003";
static char data_3D832[] = "DR02_014";
static char data_3D83B[] = "DR15_013";
static char data_3D844[] = "DR01_006";
static char data_3D84D[] = "DR01_007";
static char data_3D856[] = "DR13_010";
static char data_3D85F[] = "DR15_009";
static char data_3D868[] = "DR01_010";
static char data_3D871[] = "DR02_018";
static char data_3D87A[] = "DR17_002";
static char data_3D883[] = "DR13_009";
static char data_3D88C[] = "DR06_007";
static char data_3D895[] = "DR06_008";
static char data_3D89E[] = "DR15_011";
static char data_3D8A7[] = "DR02_013";
static char data_3D8B0[] = "DR13_012";
static char data_3D8B9[] = "DR01_018";
static char data_3D8C2[] = "DR13_008";
static char data_3D8CB[] = "DR02_017";
static char data_3D8D4[] = "DR13_007";
static char data_3D8DD[] = "DR12_001";
static char data_3D8E6[] = "DR15_014";
static char data_3D8EF[] = "DR28_001";
static char data_3D8F8[] = "DR15_010";
static char data_3D901[] = "DR02_019";
static char data_3D90A[] = "DR02_020";
static char data_3D913[] = "DR01_008";
static char data_3D91C[] = "DR01_009";
static char data_3D925[] = "DR15_012";
static char data_3D92E[] = "DR02_015";
static char data_3D937[] = "DR13_013";
static char *const data_3D940 = data_3D7FC;
static const int32_t data_3D944[3] = {1, 1311403, 1376939};
static char *const data_3D950 = data_3D805;
static const int32_t data_3D954[3] = {0, -1, -1};
static char *const data_3D960 = data_3D80E;
static const int32_t data_3D964[3] = {5, 1442475, 1508011};
static char *const data_3D970 = data_3D817;
static const int32_t data_3D974[3] = {2, 1573547, 1639083};
static char *const data_3D980 = data_3D820;
static const int32_t data_3D984[3] = {2, 1573547, 1639083};
static char *const data_3D990 = data_3D829;
static const int32_t data_3D994[3] = {0, -1, -1};
static char *const data_3D9A0 = data_3D832;
static const int32_t data_3D9A4[3] = {2, 1573547, 1639083};
static char *const data_3D9B0 = data_3D83B;
static const int32_t data_3D9B4[3] = {15, 1180331, 1245867};
static char *const data_3D9C0 = data_3D844;
static const int32_t data_3D9C4[3] = {1, 1311403, 1376939};
static char *const data_3D9D0 = data_3D84D;
static const int32_t data_3D9D4[3] = {36, 1311403, 1376939};
static char *const data_3D9E0 = data_3D856;
static const int32_t data_3D9E4[3] = {13, 1442475, 1508011};
static char *const data_3D9F0 = data_3D85F;
static const int32_t data_3D9F4[3] = {15, 1180331, 1245867};
static char *const data_3DA00 = data_3D868;
static const int32_t data_3DA04[3] = {1, 1311403, 1376939};
static char *const data_3DA10 = data_3D871;
static const int32_t data_3DA14[3] = {2, 1573547, 1639083};
static char *const data_3DA20 = data_3D87A;
static const int32_t data_3DA24[3] = {17, 656043, 721579};
static char *const data_3DA30 = data_3D883;
static const int32_t data_3DA34[3] = {13, 1442475, 1508011};
static char *const data_3DA40 = data_3D88C;
static const int32_t data_3DA44[3] = {6, 1442475, 1508011};
static char *const data_3DA50 = data_3D895;
static const int32_t data_3DA54[3] = {6, 1442475, 1508011};
static char *const data_3DA60 = data_3D89E;
static const int32_t data_3DA64[3] = {15, 1180331, 1245867};
static char *const data_3DA70 = data_3D8A7;
static const int32_t data_3DA74[3] = {2, 1573547, 1639083};
static char *const data_3DA80 = data_3D8B0;
static const int32_t data_3DA84[3] = {13, 1442475, 1508011};
static char *const data_3DA90 = data_3D8B9;
static const int32_t data_3DA94[3] = {36, 1311403, 1376939};
static char *const data_3DAA0 = data_3D8C2;
static const int32_t data_3DAA4[3] = {13, 1442475, 1508011};
static char *const data_3DAB0 = data_3D8CB;
static const int32_t data_3DAB4[3] = {2, 1573547, 1639083};
static char *const data_3DAC0 = data_3D8D4;
static const int32_t data_3DAC4[3] = {13, 1442475, 1508011};
static char *const data_3DAD0 = data_3D8DD;
static const int32_t data_3DAD4[3] = {12, 656043, 721579};
static char *const data_3DAE0 = data_3D8E6;
static const int32_t data_3DAE4[3] = {15, 1180331, 1245867};
static char *const data_3DAF0 = data_3D8EF;
static const int32_t data_3DAF4[3] = {28, 1049259, 1114795};
static char *const data_3DB00 = data_3D8F8;
static const int32_t data_3DB04[3] = {15, 1180331, 1245867};
static char *const data_3DB10 = data_3D901;
static const int32_t data_3DB14[3] = {2, 1573547, 1639083};
static char *const data_3DB20 = data_3D90A;
static const int32_t data_3DB24[3] = {2, 1573547, 1639083};
static char *const data_3DB30 = data_3D913;
static const int32_t data_3DB34[3] = {1, 1311403, 1376939};
static char *const data_3DB40 = data_3D91C;
static const int32_t data_3DB44[3] = {1, 1311403, 1376939};
static char *const data_3DB50 = data_3D925;
static const int32_t data_3DB54[3] = {15, 1180331, 1245867};
static char *const data_3DB60 = data_3D92E;
static const int32_t data_3DB64[3] = {2, 1573547, 1639083};
static char *const data_3DB70 = data_3D937;
static const int32_t data_3DB74[3] = {13, 1442475, 1508011};
static const MapObjectRecord data_3DB80[120] = {
  {.kind = 2, .resourceIndex = 0, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-122.646652f, 0.0724f, 82.192696f}, .yawFixed16 = 9393},
  {.kind = 1, .resourceIndex = 1, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-122.677994f, 0.072396f, 82.16787f}, .yawFixed16 = 9393},
  {.kind = 2, .resourceIndex = 2, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-10.311799f, 0.0724f, 24.0f}, .yawFixed16 = 0},
  {.kind = 1, .resourceIndex = 1, .scale = {0.45f, 1.0f, 1.0f}, .pos = {-9.8618f, 0.0724f, 24.0f}, .yawFixed16 = 0},
  {.kind = 2, .resourceIndex = 3, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-93.6605f, 0.0724f, 59.617001f}, .yawFixed16 = 2767},
  {.kind = 2, .resourceIndex = 4, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-126.362854f, -1.8f, 65.0345f}, .yawFixed16 = 25795},
  {.kind = 1, .resourceIndex = 5, .scale = {1.0f, 1.1f, 1.0f}, .pos = {-126.009689f, -1.8f, 65.313385f}, .yawFixed16 = 25795},
  {.kind = 1, .resourceIndex = 1, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-94.172798f, 0.0724f, 58.513298f}, .yawFixed16 = 2767},
  {.kind = 2, .resourceIndex = 6, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-93.738541f, 0.0724f, 58.395317f}, .yawFixed16 = 2767},
  {.kind = 1, .resourceIndex = 1, .scale = {0.75f, 1.0f, 1.0f}, .pos = {-95.345993f, 0.0724f, 67.7024f}, .yawFixed16 = -28216},
  {.kind = 2, .resourceIndex = 7, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-95.345993f, 0.0724f, 67.7024f}, .yawFixed16 = -28216},
  {.kind = 2, .resourceIndex = 8, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-99.812065f, 0.0724f, 99.813805f}, .yawFixed16 = -23301},
  {.kind = 1, .resourceIndex = 1, .scale = {0.5f, 1.0f, 1.0f}, .pos = {-100.089127f, 0.0724f, 100.168411f}, .yawFixed16 = -23301},
  {.kind = 1, .resourceIndex = 1, .scale = {0.5f, 1.0f, 1.0f}, .pos = {-100.089127f, 2.7724f, 100.168411f}, .yawFixed16 = -23301},
  {.kind = 2, .resourceIndex = 9, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-99.812103f, 2.7724f, 99.813797f}, .yawFixed16 = -23301},
  {.kind = 2, .resourceIndex = 8, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-103.136642f, 0.0724f, 104.068993f}, .yawFixed16 = -23301},
  {.kind = 1, .resourceIndex = 1, .scale = {0.5f, 1.0f, 1.0f}, .pos = {-103.413712f, 0.0724f, 104.423607f}, .yawFixed16 = -23301},
  {.kind = 1, .resourceIndex = 1, .scale = {0.5f, 1.0f, 1.0f}, .pos = {-103.413712f, 2.7724f, 104.423607f}, .yawFixed16 = -23301},
  {.kind = 2, .resourceIndex = 8, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-103.136002f, 2.7724f, 104.069f}, .yawFixed16 = -23301},
  {.kind = 1, .resourceIndex = 1, .scale = {0.675132f, 1.0f, 1.0f}, .pos = {-117.714317f, 0.0724f, 85.386909f}, .yawFixed16 = -23374},
  {.kind = 2, .resourceIndex = 0, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-117.434776f, 0.0724f, 85.034241f}, .yawFixed16 = -23374},
  {.kind = 1, .resourceIndex = 1, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-71.7677f, 0.0724f, 79.417603f}, .yawFixed16 = 0},
  {.kind = 2, .resourceIndex = 10, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-71.7677f, 0.0724f, 79.457603f}, .yawFixed16 = 0},
  {.kind = 1, .resourceIndex = 1, .scale = {0.75f, 1.0f, 1.0f}, .pos = {-74.390099f, 0.2505f, 68.9077f}, .yawFixed16 = -32767},
  {.kind = 2, .resourceIndex = 11, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-74.390099f, 0.2505f, 68.947693f}, .yawFixed16 = -32768},
  {.kind = 1, .resourceIndex = 1, .scale = {0.45f, 1.0f, 1.0f}, .pos = {38.7924f, 0.0724f, 8.474f}, .yawFixed16 = -21335},
  {.kind = 2, .resourceIndex = 12, .scale = {1.0f, 1.0f, 1.0f}, .pos = {38.998093f, 0.0724f, 8.073762f}, .yawFixed16 = -21335},
  {.kind = 2, .resourceIndex = 13, .scale = {1.0f, 1.0f, 1.0f}, .pos = {23.00841f, 0.0724f, 23.001627f}, .yawFixed16 = 11432},
  {.kind = 2, .resourceIndex = 14, .scale = {1.0f, 1.0f, 1.0f}, .pos = {22.358902f, 0.0724f, 32.833599f}, .yawFixed16 = 32768},
  {.kind = 2, .resourceIndex = 15, .scale = {1.0f, 1.0f, 1.0f}, .pos = {6.338201f, 0.0724f, 25.81f}, .yawFixed16 = 0},
  {.kind = 2, .resourceIndex = 16, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-5.409301f, 0.0724f, 25.8f}, .yawFixed16 = 0},
  {.kind = 1, .resourceIndex = 1, .scale = {0.5f, 1.0f, 1.0f}, .pos = {-5.8593f, 0.0724f, 25.8f}, .yawFixed16 = 0},
  {.kind = 2, .resourceIndex = 17, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-14.397702f, 0.0724f, 28.168093f}, .yawFixed16 = 7117},
  {.kind = 2, .resourceIndex = 2, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-52.868057f, 0.0724f, 40.178917f}, .yawFixed16 = 6462},
  {.kind = 1, .resourceIndex = 1, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-52.501701f, 0.0724f, 39.917599f}, .yawFixed16 = 6462},
  {.kind = 2, .resourceIndex = 18, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-56.760239f, 0.0724f, 44.021336f}, .yawFixed16 = 6462},
  {.kind = 2, .resourceIndex = 19, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-53.875557f, 0.0724f, 51.425114f}, .yawFixed16 = -26305},
  {.kind = 1, .resourceIndex = 1, .scale = {0.45f, 1.1f, 1.0f}, .pos = {-53.517578f, 0.0724f, 51.169655f}, .yawFixed16 = -26305},
  {.kind = 1, .resourceIndex = 1, .scale = {1.0f, 1.1f, 1.0f}, .pos = {-20.707703f, 0.0724f, 33.0093f}, .yawFixed16 = 7117},
  {.kind = 2, .resourceIndex = 20, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-20.708447f, 0.0724f, 33.008385f}, .yawFixed16 = 7117},
  {.kind = 2, .resourceIndex = 21, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-31.00709f, 0.0724f, 90.011246f}, .yawFixed16 = -16383},
  {.kind = 1, .resourceIndex = 1, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-31.007092f, 0.0724f, 90.461243f}, .yawFixed16 = -16383},
  {.kind = 2, .resourceIndex = 21, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-51.747093f, 2.7724f, 92.310478f}, .yawFixed16 = -11432},
  {.kind = 1, .resourceIndex = 1, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-51.541409f, 2.7724f, 92.710724f}, .yawFixed16 = -11432},
  {.kind = 2, .resourceIndex = 21, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-49.690144f, 0.0724f, 96.312889f}, .yawFixed16 = -11432},
  {.kind = 1, .resourceIndex = 1, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-49.484447f, 0.0724f, 96.713135f}, .yawFixed16 = -11432},
  {.kind = 2, .resourceIndex = 21, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-49.690144f, 2.7724f, 96.312889f}, .yawFixed16 = -11432},
  {.kind = 1, .resourceIndex = 1, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-49.484447f, 2.7724f, 96.713135f}, .yawFixed16 = -11432},
  {.kind = 2, .resourceIndex = 8, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-47.633221f, 0.0724f, 100.315262f}, .yawFixed16 = -11432},
  {.kind = 1, .resourceIndex = 1, .scale = {0.56f, 1.0f, 1.0f}, .pos = {-47.427532f, 0.0724f, 100.7155f}, .yawFixed16 = -11432},
  {.kind = 2, .resourceIndex = 8, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-47.633221f, 2.7724f, 100.315262f}, .yawFixed16 = -11432},
  {.kind = 1, .resourceIndex = 1, .scale = {0.56f, 1.0f, 1.0f}, .pos = {-47.427532f, 2.7724f, 100.7155f}, .yawFixed16 = -11432},
  {.kind = 2, .resourceIndex = 8, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-3.060143f, 0.0724f, 92.632942f}, .yawFixed16 = -23374},
  {.kind = 1, .resourceIndex = 1, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-3.339661f, 0.0724f, 92.985603f}, .yawFixed16 = -23374},
  {.kind = 2, .resourceIndex = 21, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-3.060143f, 2.7724f, 92.632942f}, .yawFixed16 = -23374},
  {.kind = 1, .resourceIndex = 1, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-3.339661f, 2.7724f, 92.985603f}, .yawFixed16 = -23374},
  {.kind = 2, .resourceIndex = 21, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-5.855344f, 0.0724f, 96.15963f}, .yawFixed16 = -23374},
  {.kind = 1, .resourceIndex = 1, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-6.134857f, 0.0724f, 96.512291f}, .yawFixed16 = -23374},
  {.kind = 2, .resourceIndex = 21, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-5.855344f, 2.7724f, 96.15963f}, .yawFixed16 = -23374},
  {.kind = 1, .resourceIndex = 1, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-6.134857f, 2.7724f, 96.512291f}, .yawFixed16 = -23374},
  {.kind = 2, .resourceIndex = 21, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-31.00709f, 2.7724f, 90.011246f}, .yawFixed16 = -16383},
  {.kind = 1, .resourceIndex = 1, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-31.007092f, 2.7724f, 90.461243f}, .yawFixed16 = -16383},
  {.kind = 2, .resourceIndex = 21, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-31.00709f, 2.7724f, 85.511566f}, .yawFixed16 = -16383},
  {.kind = 1, .resourceIndex = 1, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-31.007092f, 2.7724f, 85.961563f}, .yawFixed16 = -16383},
  {.kind = 2, .resourceIndex = 8, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-31.00709f, 0.0724f, 85.511566f}, .yawFixed16 = -16383},
  {.kind = 1, .resourceIndex = 1, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-31.007092f, 0.0724f, 85.961563f}, .yawFixed16 = -16383},
  {.kind = 2, .resourceIndex = 21, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-9.383801f, 0.0724f, 69.625267f}, .yawFixed16 = -4223},
  {.kind = 1, .resourceIndex = 1, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-8.970196f, 0.0724f, 69.802544f}, .yawFixed16 = -4223},
  {.kind = 2, .resourceIndex = 21, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-9.383801f, 2.7724f, 69.625267f}, .yawFixed16 = -4223},
  {.kind = 1, .resourceIndex = 1, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-8.970196f, 2.7724f, 69.802544f}, .yawFixed16 = -4223},
  {.kind = 2, .resourceIndex = 21, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-5.247641f, 2.7724f, 71.39801f}, .yawFixed16 = -4223},
  {.kind = 1, .resourceIndex = 1, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-4.83403f, 2.7724f, 71.575287f}, .yawFixed16 = -4223},
  {.kind = 2, .resourceIndex = 8, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-5.247641f, 0.0724f, 71.39801f}, .yawFixed16 = -4223},
  {.kind = 1, .resourceIndex = 1, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-4.83403f, 0.0724f, 71.575287f}, .yawFixed16 = -4223},
  {.kind = 2, .resourceIndex = 8, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-51.747093f, 0.0724f, 92.310478f}, .yawFixed16 = -11432},
  {.kind = 1, .resourceIndex = 1, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-51.541409f, 0.0724f, 92.710724f}, .yawFixed16 = -11432},
  {.kind = 1, .resourceIndex = 1, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-30.733089f, 0.0724f, 71.916801f}, .yawFixed16 = 0},
  {.kind = 2, .resourceIndex = 22, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-30.733089f, 0.0724f, 71.916801f}, .yawFixed16 = 0},
  {.kind = 1, .resourceIndex = 1, .scale = {1.0f, 1.2f, 1.0f}, .pos = {-40.69305f, 0.0724f, 74.185394f}, .yawFixed16 = 2949},
  {.kind = 2, .resourceIndex = 18, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-40.704201f, 0.0724f, 74.147003f}, .yawFixed16 = 2949},
  {.kind = 2, .resourceIndex = 21, .scale = {1.0f, 1.0f, 1.0f}, .pos = {42.01281f, 7.9474f, 89.753197f}, .yawFixed16 = -25122},
  {.kind = 1, .resourceIndex = 1, .scale = {1.0f, 1.0f, 1.0f}, .pos = {41.678394f, 7.9474f, 90.054306f}, .yawFixed16 = -25122},
  {.kind = 2, .resourceIndex = 21, .scale = {1.0f, 1.0f, 1.0f}, .pos = {38.668617f, 7.9474f, 92.764336f}, .yawFixed16 = -25122},
  {.kind = 1, .resourceIndex = 1, .scale = {1.0f, 1.0f, 1.0f}, .pos = {38.334198f, 7.9474f, 93.065445f}, .yawFixed16 = -25122},
  {.kind = 2, .resourceIndex = 21, .scale = {1.0f, 1.0f, 1.0f}, .pos = {38.668617f, 5.2474f, 92.764336f}, .yawFixed16 = -25122},
  {.kind = 1, .resourceIndex = 1, .scale = {1.0f, 1.0f, 1.0f}, .pos = {38.334198f, 5.2474f, 93.065445f}, .yawFixed16 = -25122},
  {.kind = 2, .resourceIndex = 8, .scale = {1.0f, 1.0f, 1.0f}, .pos = {42.01281f, 5.2474f, 89.753197f}, .yawFixed16 = -25122},
  {.kind = 1, .resourceIndex = 1, .scale = {1.0f, 1.0f, 1.0f}, .pos = {41.678394f, 5.2474f, 90.054306f}, .yawFixed16 = -25122},
  {.kind = 1, .resourceIndex = 5, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-105.975586f, 0.0724f, 93.001869f}, .yawFixed16 = 9393},
  {.kind = 2, .resourceIndex = 23, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-105.671112f, 0.0724f, 92.682182f}, .yawFixed16 = 9393},
  {.kind = 2, .resourceIndex = 24, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-97.059059f, 0.0734f, 61.789459f}, .yawFixed16 = 4551},
  {.kind = 2, .resourceIndex = 25, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-68.2295f, 0.08239f, 53.425629f}, .yawFixed16 = 13034},
  {.kind = 2, .resourceIndex = 19, .scale = {1.0f, 1.0f, 1.0f}, .pos = {14.29f, 0.07f, 35.618f}, .yawFixed16 = 32768},
  {.kind = 1, .resourceIndex = 1, .scale = {0.45f, 1.0f, 0.4f}, .pos = {14.74f, 0.0724f, 35.651798f}, .yawFixed16 = 32768},
  {.kind = 2, .resourceIndex = 6, .scale = {1.0f, 1.0f, 1.0f}, .pos = {14.29f, 0.07f, 32.956589f}, .yawFixed16 = 32768},
  {.kind = 1, .resourceIndex = 1, .scale = {0.75f, 1.0f, 1.0f}, .pos = {20.3689f, 0.08288f, 49.165401f}, .yawFixed16 = -16383},
  {.kind = 2, .resourceIndex = 26, .scale = {1.0f, 1.0f, 1.0f}, .pos = {20.368959f, 0.082879f, 49.165401f}, .yawFixed16 = -16383},
  {.kind = 2, .resourceIndex = 6, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-65.236916f, 0.072395f, 95.27832f}, .yawFixed16 = 16384},
  {.kind = 1, .resourceIndex = 5, .scale = {0.9f, 1.1f, 1.0f}, .pos = {-95.490257f, 0.0724f, 84.369667f}, .yawFixed16 = 4551},
  {.kind = 2, .resourceIndex = 27, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-95.490257f, 0.0724f, 84.369667f}, .yawFixed16 = 4551},
  {.kind = 1, .resourceIndex = 1, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-63.242302f, 0.0827f, 66.408524f}, .yawFixed16 = 49152},
  {.kind = 2, .resourceIndex = 28, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-63.242298f, 0.0827f, 66.408501f}, .yawFixed16 = -16384},
  {.kind = 2, .resourceIndex = 29, .scale = {1.0f, 1.0f, 1.0f}, .pos = {7.3624f, 0.072396f, 84.918999f}, .yawFixed16 = -6990},
  {.kind = 1, .resourceIndex = 1, .scale = {0.7f, 1.1f, 1.0f}, .pos = {7.0097f, 0.072396f, 84.639503f}, .yawFixed16 = -6990},
  {.kind = 1, .resourceIndex = 1, .scale = {0.62f, 1.0f, 1.0f}, .pos = {-18.885372f, 0.072397f, 72.516792f}, .yawFixed16 = 0},
  {.kind = 2, .resourceIndex = 30, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-18.435373f, 0.072397f, 72.516792f}, .yawFixed16 = 0},
  {.kind = 2, .resourceIndex = 31, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-111.771889f, 0.072397f, 69.613281f}, .yawFixed16 = 4551},
  {.kind = 1, .resourceIndex = 1, .scale = {0.55f, 1.0f, 1.0f}, .pos = {-111.364052f, 0.072397f, 69.423103f}, .yawFixed16 = 4551},
  {.kind = 1, .resourceIndex = 5, .scale = {0.8f, 1.2f, 1.0f}, .pos = {-123.806267f, 0.072396f, 95.76992f}, .yawFixed16 = 18513},
  {.kind = 2, .resourceIndex = 32, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-123.658234f, 0.072396f, 96.484764f}, .yawFixed16 = 18513},
  {.kind = 2, .resourceIndex = 33, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-111.496483f, 0.072392f, 103.089706f}, .yawFixed16 = 28489},
  {.kind = 1, .resourceIndex = 1, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-111.496498f, 0.072395f, 103.089699f}, .yawFixed16 = 28489},
  {.kind = 2, .resourceIndex = 23, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-124.66507f, 0.072396f, 88.421341f}, .yawFixed16 = 18513},
  {.kind = 1, .resourceIndex = 5, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-124.612915f, 0.072396f, 88.870094f}, .yawFixed16 = 18513},
  {.kind = 1, .resourceIndex = 5, .scale = {0.62f, 1.0f, 1.0f}, .pos = {-53.788616f, 0.072396f, 80.828423f}, .yawFixed16 = 5115},
  {.kind = 2, .resourceIndex = 34, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-53.391666f, 0.072396f, 80.616402f}, .yawFixed16 = 5115},
  {.kind = 1, .resourceIndex = 5, .scale = {1.0f, 1.2f, 1.0f}, .pos = {-115.437248f, 0.072395f, 101.619835f}, .yawFixed16 = 28489},
  {.kind = 2, .resourceIndex = 35, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-115.437263f, 0.072395f, 101.619865f}, .yawFixed16 = -4278},
  {.kind = 2, .resourceIndex = 2, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-36.584099f, 0.0724f, 35.1745f}, .yawFixed16 = 0},
  {.kind = 1, .resourceIndex = 1, .scale = {0.5f, 1.0f, 1.0f}, .pos = {-36.134102f, 0.0704f, 35.1745f}, .yawFixed16 = 0}
};
static const int32_t data_3EC60[57] = {
  0, 0, -8046, 0, 0, -1820, -13088, 4350, -2894, 0, 0, -9011, 0, 0, -18422, 0, 0, -9812, 0, 0, 0, 0, 0, -7518, 0, 0, -17385, 0, 0, -11559, 0, 0, 0, 0,
  0, -5734, 0, 0, -18058, 0, 0, -12233, 0, 0, 0, 0, 0, -7827, 0, 0, -16183, 0, 0, -13981, 0, -72, 0
};
static const int32_t data_3ED44[57] = {
  0, 0, -2712, 0, 0, -691, 0, 0, 0, 0, 0, -1092, 0, 0, -1438, 0, 0, -2748, 0, 0, 0, 0, 0, 364, 0, 0, -4132, 0, 0, -3531, 0, 0, 0, 0, 0, -2020, 0, 0,
  -6881, 0, 0, -5333, 0, 109, -200, 0, 0, -3986, 0, 0, -8829, 0, 0, -7245, 0, -764, -837
};
static const uint32_t data_3EE28 = 0x303131;
static const uint32_t data_3EE2C = 0x393131;
static const uint32_t data_3EE30 = 0x373731;
static const uint32_t data_3EE34 = 0x343031;
static const char data_3EE38[] __attribute__((scn_string_literal)) = "61564";
static const char data_3EE3E[] __attribute__((scn_string_literal)) = "37742";
static const char data_3EE44[] __attribute__((scn_string_literal)) = "37749";
static const char data_3EE4A[] __attribute__((scn_string_literal)) = "22550";
static const char data_3EE50[] __attribute__((scn_string_literal)) = "046861564";
static const char data_3EE5A[] __attribute__((scn_string_literal)) = "046837742";
static const char data_3EE64[] __attribute__((scn_string_literal)) = "046837749";
static const char data_3EE6E[] __attribute__((scn_string_literal)) = "046822550";
static const char data_3EE78[] __attribute__((scn_string_literal)) = "2?????";
static const char data_3EE7F[] __attribute__((scn_string_literal)) = "5?????";
static const char data_3EE86[] __attribute__((scn_string_literal)) = "7?????";
static const char data_3EE8D[] __attribute__((scn_string_literal)) = "8?????";
static const char data_3EE94[] __attribute__((scn_string_literal)) = "3?????";
static const char data_3EE9B[] __attribute__((scn_string_literal)) = "4?????";
static const char data_3EEA2[] __attribute__((scn_string_literal)) = "6?????";
static const char data_3EEA9[] __attribute__((scn_string_literal)) = "9?????";
static const char data_3EEB0[] __attribute__((scn_string_literal)) = "0?????????";
static const char data_3EEBB[] __attribute__((scn_string_literal)) = "1?????";
static const char data_3EEC2[6] __attribute__((scn_string_literal)) = "E1066";
static const char data_3EEC8[10] __attribute__((scn_string_literal)) = "E1066A012";
static const char data_3EED2[10] __attribute__((scn_string_literal)) = "E1066A013";
static const char data_3EEDC[10] __attribute__((scn_string_literal)) = "E1066A006";
static const char data_3EEE6[6] __attribute__((scn_string_literal)) = "E1066";
static const char data_3EEEC[12] __attribute__((scn_string_literal)) = "メモ帳消去\012";
static const char data_3EEF8[6] __attribute__((scn_string_literal)) = "E1032";
static const char data_3EEFE[10] __attribute__((scn_string_literal)) = "E1032A001";
static const char data_3EF08[10] __attribute__((scn_string_literal)) = "E1032A002";
static const char data_3EF12[10] __attribute__((scn_string_literal)) = "E1032A003";
static const char data_3EF1C[10] __attribute__((scn_string_literal)) = "E1032A004";
static const char data_3EF26[10] __attribute__((scn_string_literal)) = "E1032A005";
static const char data_3EF30[6] __attribute__((scn_string_literal)) = "E1032";
static const char data_3EF36[6] __attribute__((scn_string_literal)) = "E1032";
static const char data_3EF3C[10] __attribute__((scn_string_literal)) = "E1032A061";
static const char data_3EF46[10] __attribute__((scn_string_literal)) = "E1032A062";
static const char data_3EF50[10] __attribute__((scn_string_literal)) = "E1032A063";
static const char data_3EF5A[10] __attribute__((scn_string_literal)) = "E1032A064";
static const char data_3EF64[6] __attribute__((scn_string_literal)) = "E1032";
static const char data_3EF6A[6] __attribute__((scn_string_literal)) = "E1032";
static const char data_3EF70[6] __attribute__((scn_string_literal)) = "E1032";
static const char data_3EF76[6] __attribute__((scn_string_literal)) = "E1032";
static const char data_3EF7C[10] __attribute__((scn_string_literal)) = "E1032A031";
static const char data_3EF86[10] __attribute__((scn_string_literal)) = "E1032A029";
static const char data_3EF90[10] __attribute__((scn_string_literal)) = "E1032A032";
static const char data_3EF9A[10] __attribute__((scn_string_literal)) = "E1032A033";
static const char data_3EFA4[6] __attribute__((scn_string_literal)) = "E1032";
static const char data_3EFAA[10] __attribute__((scn_string_literal)) = "E1032A028";
static const char data_3EFB4[10] __attribute__((scn_string_literal)) = "E1032A029";
static const char data_3EFBE[10] __attribute__((scn_string_literal)) = "E1032A030";
static const char data_3EFC8[6] __attribute__((scn_string_literal)) = "E1032";
static const char data_3EFCE[10] __attribute__((scn_string_literal)) = "E1032G001";
static const char data_3EFD8[10] __attribute__((scn_string_literal)) = "E1032G002";
static const char data_3EFE2[6] __attribute__((scn_string_literal)) = "E1032";
static const char data_3EFE8[10] __attribute__((scn_string_literal)) = "E1032I001";
static const char data_3EFF2[10] __attribute__((scn_string_literal)) = "E1032A052";
static const char data_3EFFC[10] __attribute__((scn_string_literal)) = "E1032I007";
static const char data_3F006[10] __attribute__((scn_string_literal)) = "E1032I008";
static const char data_3F010[10] __attribute__((scn_string_literal)) = "E1032A097";
static const char data_3F01A[10] __attribute__((scn_string_literal)) = "E1032I009";
static const char data_3F024[10] __attribute__((scn_string_literal)) = "E1032I010";
static const char data_3F02E[10] __attribute__((scn_string_literal)) = "E1032A054";
static const char data_3F038[10] __attribute__((scn_string_literal)) = "E1032I011";
static const char data_3F042[10] __attribute__((scn_string_literal)) = "E1032A055";
static const char data_3F04C[10] __attribute__((scn_string_literal)) = "E1032I012";
static const char data_3F056[10] __attribute__((scn_string_literal)) = "E1032A056";
static const char data_3F060[10] __attribute__((scn_string_literal)) = "E1032I013";
static const char data_3F06A[10] __attribute__((scn_string_literal)) = "E1032A057";
static const char data_3F074[10] __attribute__((scn_string_literal)) = "E1032I011";
static const char data_3F07E[10] __attribute__((scn_string_literal)) = "E1032A059";
static const char data_3F088[10] __attribute__((scn_string_literal)) = "E1032I009";
static const char data_3F092[10] __attribute__((scn_string_literal)) = "E1032I010";
static const char data_3F09C[10] __attribute__((scn_string_literal)) = "E1032A054";
static const char data_3F0A6[10] __attribute__((scn_string_literal)) = "E1032I011";
static const char data_3F0B0[10] __attribute__((scn_string_literal)) = "E1032A060";
static const char data_3F0BA[10] __attribute__((scn_string_literal)) = "E1032I009";
static const char data_3F0C4[10] __attribute__((scn_string_literal)) = "E1032I010";
static const char data_3F0CE[10] __attribute__((scn_string_literal)) = "E1032A054";
static const char data_3F0D8[10] __attribute__((scn_string_literal)) = "E1032I011";
static const char data_3F0E2[10] __attribute__((scn_string_literal)) = "E1032I001";
static const char data_3F0EC[10] __attribute__((scn_string_literal)) = "E1032A007";
static const char data_3F0F6[10] __attribute__((scn_string_literal)) = "E1032A006";
static const char data_3F100[10] __attribute__((scn_string_literal)) = "E1032I002";
static const char data_3F10A[10] __attribute__((scn_string_literal)) = "E1032I003";
static const char data_3F114[10] __attribute__((scn_string_literal)) = "E1032I004";
static const char data_3F11E[10] __attribute__((scn_string_literal)) = "E1032I005";
static const char data_3F128[10] __attribute__((scn_string_literal)) = "E1032I006";
static const char data_3F132[10] __attribute__((scn_string_literal)) = "E1032A008";
static const char data_3F13C[10] __attribute__((scn_string_literal)) = "E1032A009";
static const char data_3F146[10] __attribute__((scn_string_literal)) = "E1032K001";
static const char data_3F150[10] __attribute__((scn_string_literal)) = "E1032K002";
static const char data_3F15A[10] __attribute__((scn_string_literal)) = "E1032K019";
static const char data_3F164[10] __attribute__((scn_string_literal)) = "E1032K020";
static const char data_3F16E[10] __attribute__((scn_string_literal)) = "E1032K022";
static const char data_3F178[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_3F182[10] __attribute__((scn_string_literal)) = "E1032K016";
static const char data_3F18C[10] __attribute__((scn_string_literal)) = "E1032K017";
static const char data_3F196[10] __attribute__((scn_string_literal)) = "E1032K018";
static const char data_3F1A0[10] __attribute__((scn_string_literal)) = "E1032K003";
static const char data_3F1AA[10] __attribute__((scn_string_literal)) = "E1032K016";
static const char data_3F1B4[10] __attribute__((scn_string_literal)) = "E1032K003";
static const char data_3F1BE[10] __attribute__((scn_string_literal)) = "E1032K017";
static const char data_3F1C8[10] __attribute__((scn_string_literal)) = "E1032K003";
static const char data_3F1D2[10] __attribute__((scn_string_literal)) = "E1032K018";
static const char data_3F1DC[10] __attribute__((scn_string_literal)) = "E1032K004";
static const char data_3F1E6[10] __attribute__((scn_string_literal)) = "E1032K016";
static const char data_3F1F0[10] __attribute__((scn_string_literal)) = "E1032K004";
static const char data_3F1FA[10] __attribute__((scn_string_literal)) = "E1032K017";
static const char data_3F204[10] __attribute__((scn_string_literal)) = "E1032K004";
static const char data_3F20E[10] __attribute__((scn_string_literal)) = "E1032K018";
static const char data_3F218[10] __attribute__((scn_string_literal)) = "E1032K005";
static const char data_3F222[10] __attribute__((scn_string_literal)) = "E1032K016";
static const char data_3F22C[10] __attribute__((scn_string_literal)) = "E1032K005";
static const char data_3F236[10] __attribute__((scn_string_literal)) = "E1032K017";
static const char data_3F240[10] __attribute__((scn_string_literal)) = "E1032K005";
static const char data_3F24A[10] __attribute__((scn_string_literal)) = "E1032K018";
static const char data_3F254[10] __attribute__((scn_string_literal)) = "E1032K006";
static const char data_3F25E[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_3F268[10] __attribute__((scn_string_literal)) = "E1032K006";
static const char data_3F272[10] __attribute__((scn_string_literal)) = "E1032K017";
static const char data_3F27C[10] __attribute__((scn_string_literal)) = "E1032K006";
static const char data_3F286[10] __attribute__((scn_string_literal)) = "E1032K018";
static const char data_3F290[10] __attribute__((scn_string_literal)) = "E1032K007";
static const char data_3F29A[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_3F2A4[10] __attribute__((scn_string_literal)) = "E1032K007";
static const char data_3F2AE[10] __attribute__((scn_string_literal)) = "E1032K017";
static const char data_3F2B8[10] __attribute__((scn_string_literal)) = "E1032K007";
static const char data_3F2C2[10] __attribute__((scn_string_literal)) = "E1032K018";
static const char data_3F2CC[10] __attribute__((scn_string_literal)) = "E1032K008";
static const char data_3F2D6[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_3F2E0[10] __attribute__((scn_string_literal)) = "E1032K008";
static const char data_3F2EA[10] __attribute__((scn_string_literal)) = "E1032K017";
static const char data_3F2F4[10] __attribute__((scn_string_literal)) = "E1032K008";
static const char data_3F2FE[10] __attribute__((scn_string_literal)) = "E1032K018";
static const char data_3F308[10] __attribute__((scn_string_literal)) = "E1032K009";
static const char data_3F312[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_3F31C[10] __attribute__((scn_string_literal)) = "E1032K009";
static const char data_3F326[10] __attribute__((scn_string_literal)) = "E1032K016";
static const char data_3F330[10] __attribute__((scn_string_literal)) = "E1032K009";
static const char data_3F33A[10] __attribute__((scn_string_literal)) = "E1032K018";
static const char data_3F344[10] __attribute__((scn_string_literal)) = "E1032K010";
static const char data_3F34E[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_3F358[10] __attribute__((scn_string_literal)) = "E1032K010";
static const char data_3F362[10] __attribute__((scn_string_literal)) = "E1032K016";
static const char data_3F36C[10] __attribute__((scn_string_literal)) = "E1032K010";
static const char data_3F376[10] __attribute__((scn_string_literal)) = "E1032K018";
static const char data_3F380[10] __attribute__((scn_string_literal)) = "E1032K011";
static const char data_3F38A[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_3F394[10] __attribute__((scn_string_literal)) = "E1032K011";
static const char data_3F39E[10] __attribute__((scn_string_literal)) = "E1032K016";
static const char data_3F3A8[10] __attribute__((scn_string_literal)) = "E1032K011";
static const char data_3F3B2[10] __attribute__((scn_string_literal)) = "E1032K018";
static const char data_3F3BC[10] __attribute__((scn_string_literal)) = "E1032K012";
static const char data_3F3C6[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_3F3D0[10] __attribute__((scn_string_literal)) = "E1032K012";
static const char data_3F3DA[10] __attribute__((scn_string_literal)) = "E1032K016";
static const char data_3F3E4[10] __attribute__((scn_string_literal)) = "E1032K012";
static const char data_3F3EE[10] __attribute__((scn_string_literal)) = "E1032K017";
static const char data_3F3F8[10] __attribute__((scn_string_literal)) = "E1032K013";
static const char data_3F402[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_3F40C[10] __attribute__((scn_string_literal)) = "E1032K013";
static const char data_3F416[10] __attribute__((scn_string_literal)) = "E1032K016";
static const char data_3F420[10] __attribute__((scn_string_literal)) = "E1032K013";
static const char data_3F42A[10] __attribute__((scn_string_literal)) = "E1032K017";
static const char data_3F434[10] __attribute__((scn_string_literal)) = "E1032K014";
static const char data_3F43E[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_3F448[10] __attribute__((scn_string_literal)) = "E1032K014";
static const char data_3F452[10] __attribute__((scn_string_literal)) = "E1032K016";
static const char data_3F45C[10] __attribute__((scn_string_literal)) = "E1032K014";
static const char data_3F466[10] __attribute__((scn_string_literal)) = "E1032K017";
static const char data_3F470[10] __attribute__((scn_string_literal)) = "E1032K004";
static const char data_3F47A[10] __attribute__((scn_string_literal)) = "E1032K007";
static const char data_3F484[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_3F48E[10] __attribute__((scn_string_literal)) = "E1032K004";
static const char data_3F498[10] __attribute__((scn_string_literal)) = "E1032K007";
static const char data_3F4A2[10] __attribute__((scn_string_literal)) = "E1032K017";
static const char data_3F4AC[10] __attribute__((scn_string_literal)) = "E1032K004";
static const char data_3F4B6[10] __attribute__((scn_string_literal)) = "E1032K006";
static const char data_3F4C0[10] __attribute__((scn_string_literal)) = "E1032K017";
static const char data_3F4CA[10] __attribute__((scn_string_literal)) = "E1032K004";
static const char data_3F4D4[10] __attribute__((scn_string_literal)) = "E1032K013";
static const char data_3F4DE[10] __attribute__((scn_string_literal)) = "E1032K017";
static const char data_3F4E8[10] __attribute__((scn_string_literal)) = "E1032K005";
static const char data_3F4F2[10] __attribute__((scn_string_literal)) = "E1032K007";
static const char data_3F4FC[10] __attribute__((scn_string_literal)) = "E1032K018";
static const char data_3F506[10] __attribute__((scn_string_literal)) = "E1032K007";
static const char data_3F510[10] __attribute__((scn_string_literal)) = "E1032K004";
static const char data_3F51A[10] __attribute__((scn_string_literal)) = "E1032K017";
static const char data_3F524[10] __attribute__((scn_string_literal)) = "E1032K008";
static const char data_3F52E[10] __attribute__((scn_string_literal)) = "E1032K010";
static const char data_3F538[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_3F542[10] __attribute__((scn_string_literal)) = "E1032K008";
static const char data_3F54C[10] __attribute__((scn_string_literal)) = "E1032K013";
static const char data_3F556[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_3F560[10] __attribute__((scn_string_literal)) = "E1032K010";
static const char data_3F56A[10] __attribute__((scn_string_literal)) = "E1032K005";
static const char data_3F574[10] __attribute__((scn_string_literal)) = "E1032K016";
static const char data_3F57E[10] __attribute__((scn_string_literal)) = "E1032K010";
static const char data_3F588[10] __attribute__((scn_string_literal)) = "E1032K004";
static const char data_3F592[10] __attribute__((scn_string_literal)) = "E1032K018";
static const char data_3F59C[10] __attribute__((scn_string_literal)) = "E1032K010";
static const char data_3F5A6[10] __attribute__((scn_string_literal)) = "E1032K007";
static const char data_3F5B0[10] __attribute__((scn_string_literal)) = "E1032K018";
static const char data_3F5BA[10] __attribute__((scn_string_literal)) = "E1032K011";
static const char data_3F5C4[10] __attribute__((scn_string_literal)) = "E1032K007";
static const char data_3F5CE[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_3F5D8[10] __attribute__((scn_string_literal)) = "E1032K013";
static const char data_3F5E2[10] __attribute__((scn_string_literal)) = "E1032K008";
static const char data_3F5EC[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_3F5F6[10] __attribute__((scn_string_literal)) = "E1032K010";
static const char data_3F600[10] __attribute__((scn_string_literal)) = "E1032K005";
static const char data_3F60A[10] __attribute__((scn_string_literal)) = "E1032K016";
static const char data_3F614[10] __attribute__((scn_string_literal)) = "E1032K004";
static const char data_3F61E[10] __attribute__((scn_string_literal)) = "E1032K010";
static const char data_3F628[10] __attribute__((scn_string_literal)) = "E1032K016";
static const char data_3F632[10] __attribute__((scn_string_literal)) = "E1032K004";
static const char data_3F63C[10] __attribute__((scn_string_literal)) = "E1032K008";
static const char data_3F646[10] __attribute__((scn_string_literal)) = "E1032K018";
static const char data_3F650[10] __attribute__((scn_string_literal)) = "E1032K003";
static const char data_3F65A[10] __attribute__((scn_string_literal)) = "E1032K007";
static const char data_3F664[10] __attribute__((scn_string_literal)) = "E1032K018";
static const char data_3F66E[10] __attribute__((scn_string_literal)) = "E1032K007";
static const char data_3F678[10] __attribute__((scn_string_literal)) = "E1032K010";
static const char data_3F682[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_3F68C[10] __attribute__((scn_string_literal)) = "E1032K008";
static const char data_3F696[10] __attribute__((scn_string_literal)) = "E1032K004";
static const char data_3F6A0[10] __attribute__((scn_string_literal)) = "E1032K018";
static const char data_3F6AA[10] __attribute__((scn_string_literal)) = "E1032K007";
static const char data_3F6B4[10] __attribute__((scn_string_literal)) = "E1032K010";
static const char data_3F6BE[10] __attribute__((scn_string_literal)) = "E1032K018";
static const char data_3F6C8[10] __attribute__((scn_string_literal)) = "E1032K004";
static const char data_3F6D2[10] __attribute__((scn_string_literal)) = "E1032K010";
static const char data_3F6DC[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_3F6E6[10] __attribute__((scn_string_literal)) = "E1032K004";
static const char data_3F6F0[10] __attribute__((scn_string_literal)) = "E1032K007";
static const char data_3F6FA[10] __attribute__((scn_string_literal)) = "E1032K017";
static const char data_3F704[10] __attribute__((scn_string_literal)) = "E1032K004";
static const char data_3F70E[10] __attribute__((scn_string_literal)) = "E1032K007";
static const char data_3F718[10] __attribute__((scn_string_literal)) = "E1032K018";
static const char data_3F722[10] __attribute__((scn_string_literal)) = "E1032K010";
static const char data_3F72C[10] __attribute__((scn_string_literal)) = "E1032K008";
static const char data_3F736[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_3F740[10] __attribute__((scn_string_literal)) = "E1032K013";
static const char data_3F74A[10] __attribute__((scn_string_literal)) = "E1032K007";
static const char data_3F754[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_3F75E[10] __attribute__((scn_string_literal)) = "E1032K004";
static const char data_3F768[10] __attribute__((scn_string_literal)) = "E1032K013";
static const char data_3F772[10] __attribute__((scn_string_literal)) = "E1032K016";
static const char data_3F77C[10] __attribute__((scn_string_literal)) = "E1032K014";
static const char data_3F786[10] __attribute__((scn_string_literal)) = "E1032K007";
static const char data_3F790[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_3F79A[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_3F7A4[10] __attribute__((scn_string_literal)) = "E1032L001";
static const char data_3F7AE[10] __attribute__((scn_string_literal)) = "E1032A098";
static const char data_3F7B8[10] __attribute__((scn_string_literal)) = "E1032L002";
static const char data_3F7C2[10] __attribute__((scn_string_literal)) = "E1032A099";
static const char data_3F7CC[10] __attribute__((scn_string_literal)) = "E1032L003";
static const char data_3F7D6[10] __attribute__((scn_string_literal)) = "E1032L004";
static const char data_3F7E0[10] __attribute__((scn_string_literal)) = "E1032L005";
static const char data_3F7EA[10] __attribute__((scn_string_literal)) = "E1032A100";
static const char data_3F7F4[10] __attribute__((scn_string_literal)) = "E1032L001";
static const char data_3F7FE[10] __attribute__((scn_string_literal)) = "E1032A101";
static const char data_3F808[10] __attribute__((scn_string_literal)) = "E1032L006";
static const char data_3F812[10] __attribute__((scn_string_literal)) = "E1032L007";
static const char data_3F81C[10] __attribute__((scn_string_literal)) = "E1032L008";
static const char data_3F826[10] __attribute__((scn_string_literal)) = "E1032L009";
static const char data_3F830[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_3F83A[10] __attribute__((scn_string_literal)) = "E1032A102";
static const char data_3F844[10] __attribute__((scn_string_literal)) = "E1032L010";
static const char data_3F84E[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_3F858[10] __attribute__((scn_string_literal)) = "E1032L011";
static const char data_3F862[10] __attribute__((scn_string_literal)) = "E1032A101";
static const char data_3F86C[10] __attribute__((scn_string_literal)) = "E1032L012";
static const char data_3F876[10] __attribute__((scn_string_literal)) = "E1032A103";
static const char data_3F880[10] __attribute__((scn_string_literal)) = "E1032L013";
static const char data_3F88A[10] __attribute__((scn_string_literal)) = "E1032L014";
static const char data_3F894[10] __attribute__((scn_string_literal)) = "E1032L015";
static const char data_3F89E[10] __attribute__((scn_string_literal)) = "E1032A103";
static const char data_3F8A8[10] __attribute__((scn_string_literal)) = "E1032L016";
static const char data_3F8B2[10] __attribute__((scn_string_literal)) = "E1032A104";
static const char data_3F8BC[10] __attribute__((scn_string_literal)) = "E1032A105";
static const char data_3F8C6[10] __attribute__((scn_string_literal)) = "E1032L017";
static const char data_3F8D0[10] __attribute__((scn_string_literal)) = "E1032A106";
static const char data_3F8DA[10] __attribute__((scn_string_literal)) = "E1032L018";
static const char data_3F8E4[10] __attribute__((scn_string_literal)) = "E1032A107";
static const char data_3F8EE[10] __attribute__((scn_string_literal)) = "E1032L019";
static const char data_3F8F8[10] __attribute__((scn_string_literal)) = "E1032A108";
static const char data_3F902[10] __attribute__((scn_string_literal)) = "E1032L020";
static const char data_3F90C[10] __attribute__((scn_string_literal)) = "E1032L021";
static const char data_3F916[10] __attribute__((scn_string_literal)) = "E1032A109";
static const char data_3F920[10] __attribute__((scn_string_literal)) = "E1032L022";
static const char data_3F92A[10] __attribute__((scn_string_literal)) = "E1032A110";
static const char data_3F934[10] __attribute__((scn_string_literal)) = "E1032L023";
static const char data_3F93E[10] __attribute__((scn_string_literal)) = "E1032M001";
static const char data_3F948[10] __attribute__((scn_string_literal)) = "E1032A111";
static const char data_3F952[10] __attribute__((scn_string_literal)) = "E1032M002";
static const char data_3F95C[10] __attribute__((scn_string_literal)) = "E1032A112";
static const char data_3F966[10] __attribute__((scn_string_literal)) = "E1032M003";
static const char data_3F970[10] __attribute__((scn_string_literal)) = "E1032A113";
static const char data_3F97A[10] __attribute__((scn_string_literal)) = "E1032M004";
static const char data_3F984[10] __attribute__((scn_string_literal)) = "E1032A104";
static const char data_3F98E[10] __attribute__((scn_string_literal)) = "E1032A114";
static const char data_3F998[10] __attribute__((scn_string_literal)) = "E1032M005";
static const char data_3F9A2[10] __attribute__((scn_string_literal)) = "E1032M006";
static const char data_3F9AC[10] __attribute__((scn_string_literal)) = "E1032A111";
static const char data_3F9B6[10] __attribute__((scn_string_literal)) = "E1032M002";
static const char data_3F9C0[10] __attribute__((scn_string_literal)) = "E1032A115";
static const char data_3F9CA[10] __attribute__((scn_string_literal)) = "E1032M018";
static const char data_3F9D4[10] __attribute__((scn_string_literal)) = "E1032A122";
static const char data_3F9DE[10] __attribute__((scn_string_literal)) = "E1032M019";
static const char data_3F9E8[10] __attribute__((scn_string_literal)) = "E1032A123";
static const char data_3F9F2[10] __attribute__((scn_string_literal)) = "E1032M019";
static const char data_3F9FC[10] __attribute__((scn_string_literal)) = "E1032M020";
static const char data_3FA06[10] __attribute__((scn_string_literal)) = "E1032A104";
static const char data_3FA10[10] __attribute__((scn_string_literal)) = "E1032A114";
static const char data_3FA1A[10] __attribute__((scn_string_literal)) = "E1032M010";
static const char data_3FA24[10] __attribute__((scn_string_literal)) = "E1032N001";
static const char data_3FA2E[10] __attribute__((scn_string_literal)) = "E1032A124";
static const char data_3FA38[10] __attribute__((scn_string_literal)) = "E1032A125";
static const char data_3FA42[10] __attribute__((scn_string_literal)) = "E1032N002";
static const char data_3FA4C[10] __attribute__((scn_string_literal)) = "E1032A126";
static const char data_3FA56[10] __attribute__((scn_string_literal)) = "E1032N003";
static const char data_3FA60[10] __attribute__((scn_string_literal)) = "E1032A127";
static const char data_3FA6A[10] __attribute__((scn_string_literal)) = "E1032A128";
static const char data_3FA74[10] __attribute__((scn_string_literal)) = "E1032N004";
static const char data_3FA7E[10] __attribute__((scn_string_literal)) = "E1032A129";
static const char data_3FA88[10] __attribute__((scn_string_literal)) = "E1032N005";
static const char data_3FA92[10] __attribute__((scn_string_literal)) = "E1032L011";
static const char data_3FA9C[10] __attribute__((scn_string_literal)) = "E1032A101";
static const char data_3FAA6[10] __attribute__((scn_string_literal)) = "E1032L019";
static const char data_3FAB0[10] __attribute__((scn_string_literal)) = "E1032A117";
static const char data_3FABA[10] __attribute__((scn_string_literal)) = "E1032L024";
static const char data_3FAC4[10] __attribute__((scn_string_literal)) = "E1032L025";
static const char data_3FACE[10] __attribute__((scn_string_literal)) = "E1032L026";
static const char data_3FAD8[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_3FAE2[10] __attribute__((scn_string_literal)) = "E1032A118";
static const char data_3FAEC[10] __attribute__((scn_string_literal)) = "E1032L023";
static const char data_3FAF6[10] __attribute__((scn_string_literal)) = "E1032M001";
static const char data_3FB00[10] __attribute__((scn_string_literal)) = "E1032A111";
static const char data_3FB0A[10] __attribute__((scn_string_literal)) = "E1032M002";
static const char data_3FB14[10] __attribute__((scn_string_literal)) = "E1032A112";
static const char data_3FB1E[10] __attribute__((scn_string_literal)) = "E1032M015";
static const char data_3FB28[10] __attribute__((scn_string_literal)) = "E1032A119";
static const char data_3FB32[10] __attribute__((scn_string_literal)) = "E1032M016";
static const char data_3FB3C[10] __attribute__((scn_string_literal)) = "E1032A104";
static const char data_3FB46[10] __attribute__((scn_string_literal)) = "E1032A118";
static const char data_3FB50[10] __attribute__((scn_string_literal)) = "E1032M017";
static const char data_3FB5A[10] __attribute__((scn_string_literal)) = "E1032O001";
static const char data_3FB64[10] __attribute__((scn_string_literal)) = "E1032A136";
static const char data_3FB6E[10] __attribute__((scn_string_literal)) = "E1032O002";
static const char data_3FB78[10] __attribute__((scn_string_literal)) = "E1032A137";
static const char data_3FB82[10] __attribute__((scn_string_literal)) = "E1032O007";
static const char data_3FB8C[10] __attribute__((scn_string_literal)) = "E1032A127";
static const char data_3FB96[10] __attribute__((scn_string_literal)) = "E1032O004";
static const char data_3FBA0[10] __attribute__((scn_string_literal)) = "E1032A138";
static const char data_3FBAA[10] __attribute__((scn_string_literal)) = "E1032O005";
static const char data_3FBB4[10] __attribute__((scn_string_literal)) = "E1032A139";
static const char data_3FBBE[10] __attribute__((scn_string_literal)) = "E1032O006";
static const char data_3FBC8[10] __attribute__((scn_string_literal)) = "E1032O001";
static const char data_3FBD2[10] __attribute__((scn_string_literal)) = "E1032A136";
static const char data_3FBDC[10] __attribute__((scn_string_literal)) = "E1032O002";
static const char data_3FBE6[10] __attribute__((scn_string_literal)) = "E1032A140";
static const char data_3FBF0[10] __attribute__((scn_string_literal)) = "E1032O007";
static const char data_3FBFA[10] __attribute__((scn_string_literal)) = "E1032A127";
static const char data_3FC04[10] __attribute__((scn_string_literal)) = "E1032O004";
static const char data_3FC0E[10] __attribute__((scn_string_literal)) = "E1032A138";
static const char data_3FC18[10] __attribute__((scn_string_literal)) = "E1032O005";
static const char data_3FC22[10] __attribute__((scn_string_literal)) = "E1032A139";
static const char data_3FC2C[10] __attribute__((scn_string_literal)) = "E1032O006";
static const char data_3FC36[10] __attribute__((scn_string_literal)) = "E1032P001";
static const char data_3FC40[10] __attribute__((scn_string_literal)) = "E1032A085";
static const char data_3FC4A[10] __attribute__((scn_string_literal)) = "E1032P002";
static const char data_3FC54[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_3FC5E[10] __attribute__((scn_string_literal)) = "E1032P003";
static const char data_3FC68[10] __attribute__((scn_string_literal)) = "E1032A141";
static const char data_3FC72[10] __attribute__((scn_string_literal)) = "E1032P004";
static const char data_3FC7C[10] __attribute__((scn_string_literal)) = "E1032A142";
static const char data_3FC86[10] __attribute__((scn_string_literal)) = "E1032P005";
static const char data_3FC90[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_3FC9A[10] __attribute__((scn_string_literal)) = "E1032P001";
static const char data_3FCA4[10] __attribute__((scn_string_literal)) = "E1032A085";
static const char data_3FCAE[10] __attribute__((scn_string_literal)) = "E1032P006";
static const char data_3FCB8[10] __attribute__((scn_string_literal)) = "E1032A143";
static const char data_3FCC2[10] __attribute__((scn_string_literal)) = "E1032P007";
static const char data_3FCCC[10] __attribute__((scn_string_literal)) = "E1032A144";
static const char data_3FCD6[10] __attribute__((scn_string_literal)) = "E1032P004";
static const char data_3FCE0[10] __attribute__((scn_string_literal)) = "E1032O008";
static const char data_3FCEA[10] __attribute__((scn_string_literal)) = "E1032P008";
static const char data_3FCF4[10] __attribute__((scn_string_literal)) = "E1032A068";
static const char data_3FCFE[10] __attribute__((scn_string_literal)) = "E1032P009";
static const char data_3FD08[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_3FD12[10] __attribute__((scn_string_literal)) = "E1032A085";
static const char data_3FD1C[10] __attribute__((scn_string_literal)) = "E1032P010";
static const char data_3FD26[10] __attribute__((scn_string_literal)) = "E1032A145";
static const char data_3FD30[10] __attribute__((scn_string_literal)) = "E1032P011";
static const char data_3FD3A[10] __attribute__((scn_string_literal)) = "E1032A146";
static const char data_3FD44[10] __attribute__((scn_string_literal)) = "E1032P012";
static const char data_3FD4E[10] __attribute__((scn_string_literal)) = "E1032A147";
static const char data_3FD58[10] __attribute__((scn_string_literal)) = "E1032P013";
static const char data_3FD62[10] __attribute__((scn_string_literal)) = "E1032A148";
static const char data_3FD6C[10] __attribute__((scn_string_literal)) = "E1032P014";
static const char data_3FD76[10] __attribute__((scn_string_literal)) = "E1032A149";
static const char data_3FD80[10] __attribute__((scn_string_literal)) = "E1032P015";
static const char data_3FD8A[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_3FD94[10] __attribute__((scn_string_literal)) = "E1032P016";
static const char data_3FD9E[10] __attribute__((scn_string_literal)) = "E1032A068";
static const char data_3FDA8[10] __attribute__((scn_string_literal)) = "E1032P017";
static const char data_3FDB2[10] __attribute__((scn_string_literal)) = "E1032A085";
static const char data_3FDBC[10] __attribute__((scn_string_literal)) = "E1032P018";
static const char data_3FDC6[10] __attribute__((scn_string_literal)) = "E1032A150";
static const char data_3FDD0[10] __attribute__((scn_string_literal)) = "E1032P019";
static const char data_3FDDA[10] __attribute__((scn_string_literal)) = "E1032A151";
static const char data_3FDE4[10] __attribute__((scn_string_literal)) = "E1032P004";
static const char data_3FDEE[10] __attribute__((scn_string_literal)) = "E1032P016";
static const char data_3FDF8[10] __attribute__((scn_string_literal)) = "E1032A068";
static const char data_3FE02[10] __attribute__((scn_string_literal)) = "E1032P017";
static const char data_3FE0C[10] __attribute__((scn_string_literal)) = "E1032P021";
static const char data_3FE16[10] __attribute__((scn_string_literal)) = "E1032A152";
static const char data_3FE20[10] __attribute__((scn_string_literal)) = "E1032P010";
static const char data_3FE2A[10] __attribute__((scn_string_literal)) = "E1032A153";
static const char data_3FE34[10] __attribute__((scn_string_literal)) = "E1032P022";
static const char data_3FE3E[10] __attribute__((scn_string_literal)) = "E1032A154";
static const char data_3FE48[10] __attribute__((scn_string_literal)) = "E1032P023";
static const char data_3FE52[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_3FE5C[10] __attribute__((scn_string_literal)) = "E1032P024";
static const char data_3FE66[10] __attribute__((scn_string_literal)) = "E1032A068";
static const char data_3FE70[10] __attribute__((scn_string_literal)) = "E1032P017";
static const char data_3FE7A[10] __attribute__((scn_string_literal)) = "E1032P001";
static const char data_3FE84[10] __attribute__((scn_string_literal)) = "E1032A085";
static const char data_3FE8E[10] __attribute__((scn_string_literal)) = "E1032P025";
static const char data_3FE98[10] __attribute__((scn_string_literal)) = "E1032A155";
static const char data_3FEA2[10] __attribute__((scn_string_literal)) = "E1032P004";
static const char data_3FEAC[10] __attribute__((scn_string_literal)) = "E1032A156";
static const char data_3FEB6[10] __attribute__((scn_string_literal)) = "E1032O008";
static const char data_3FEC0[10] __attribute__((scn_string_literal)) = "E1032P008";
static const char data_3FECA[10] __attribute__((scn_string_literal)) = "E1032A154";
static const char data_3FED4[10] __attribute__((scn_string_literal)) = "E1032P027";
static const char data_3FEDE[10] __attribute__((scn_string_literal)) = "E1032A068";
static const char data_3FEE8[10] __attribute__((scn_string_literal)) = "E1032P017";
static const char data_3FEF2[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_3FEFC[10] __attribute__((scn_string_literal)) = "E1032P021";
static const char data_3FF06[10] __attribute__((scn_string_literal)) = "E1032A152";
static const char data_3FF10[10] __attribute__((scn_string_literal)) = "E1032P028";
static const char data_3FF1A[10] __attribute__((scn_string_literal)) = "E1032A158";
static const char data_3FF24[10] __attribute__((scn_string_literal)) = "E1032P030";
static const char data_3FF2E[10] __attribute__((scn_string_literal)) = "E1032A159";
static const char data_3FF38[10] __attribute__((scn_string_literal)) = "E1032P066";
static const char data_3FF42[10] __attribute__((scn_string_literal)) = "E1032P016";
static const char data_3FF4C[10] __attribute__((scn_string_literal)) = "E1032A068";
static const char data_3FF56[10] __attribute__((scn_string_literal)) = "E1032P017";
static const char data_3FF60[10] __attribute__((scn_string_literal)) = "E1032P021";
static const char data_3FF6A[10] __attribute__((scn_string_literal)) = "E1032A152";
static const char data_3FF74[10] __attribute__((scn_string_literal)) = "E1032P032";
static const char data_3FF7E[10] __attribute__((scn_string_literal)) = "E1032A160";
static const char data_3FF88[10] __attribute__((scn_string_literal)) = "E1032P033";
static const char data_3FF92[10] __attribute__((scn_string_literal)) = "E1032P034";
static const char data_3FF9C[10] __attribute__((scn_string_literal)) = "E1032P080";
static const char data_3FFA6[10] __attribute__((scn_string_literal)) = "E1032P081";
static const char data_3FFB0[10] __attribute__((scn_string_literal)) = "E1032A068";
static const char data_3FFBA[10] __attribute__((scn_string_literal)) = "E1032P035";
static const char data_3FFC4[10] __attribute__((scn_string_literal)) = "E1032A141";
static const char data_3FFCE[10] __attribute__((scn_string_literal)) = "E1032P036";
static const char data_3FFD8[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_3FFE2[10] __attribute__((scn_string_literal)) = "E1032P021";
static const char data_3FFEC[10] __attribute__((scn_string_literal)) = "E1032A152";
static const char data_3FFF6[10] __attribute__((scn_string_literal)) = "E1032P025";
static const char data_40000[10] __attribute__((scn_string_literal)) = "E1032A160";
static const char data_4000A[10] __attribute__((scn_string_literal)) = "E1032P033";
static const char data_40014[10] __attribute__((scn_string_literal)) = "E1032P082";
static const char data_4001E[10] __attribute__((scn_string_literal)) = "E1032P083";
static const char data_40028[10] __attribute__((scn_string_literal)) = "E1032P084";
static const char data_40032[10] __attribute__((scn_string_literal)) = "E1032P085";
static const char data_4003C[10] __attribute__((scn_string_literal)) = "E1032A068";
static const char data_40046[10] __attribute__((scn_string_literal)) = "E1032P086";
static const char data_40050[10] __attribute__((scn_string_literal)) = "E1032P001";
static const char data_4005A[10] __attribute__((scn_string_literal)) = "E1032A164";
static const char data_40064[10] __attribute__((scn_string_literal)) = "E1032P018";
static const char data_4006E[10] __attribute__((scn_string_literal)) = "E1032A165";
static const char data_40078[10] __attribute__((scn_string_literal)) = "E1032P037";
static const char data_40082[10] __attribute__((scn_string_literal)) = "E1032A104";
static const char data_4008C[10] __attribute__((scn_string_literal)) = "E1032P120";
static const char data_40096[10] __attribute__((scn_string_literal)) = "E1032A173";
static const char data_400A0[10] __attribute__((scn_string_literal)) = "E1032O008";
static const char data_400AA[10] __attribute__((scn_string_literal)) = "E1032P008";
static const char data_400B4[10] __attribute__((scn_string_literal)) = "E1032A154";
static const char data_400BE[10] __attribute__((scn_string_literal)) = "E1032P027";
static const char data_400C8[10] __attribute__((scn_string_literal)) = "E1032A068";
static const char data_400D2[10] __attribute__((scn_string_literal)) = "E1032P017";
static const char data_400DC[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_400E6[10] __attribute__((scn_string_literal)) = "E1032P001";
static const char data_400F0[10] __attribute__((scn_string_literal)) = "E1032A164";
static const char data_400FA[10] __attribute__((scn_string_literal)) = "E1032P018";
static const char data_40104[10] __attribute__((scn_string_literal)) = "E1032A170";
static const char data_4010E[10] __attribute__((scn_string_literal)) = "E1032P044";
static const char data_40118[10] __attribute__((scn_string_literal)) = "E1032A200";
static const char data_40122[10] __attribute__((scn_string_literal)) = "E1032A172";
static const char data_4012C[10] __attribute__((scn_string_literal)) = "E1032P045";
static const char data_40136[10] __attribute__((scn_string_literal)) = "E1032A173";
static const char data_40140[10] __attribute__((scn_string_literal)) = "E1032P046";
static const char data_4014A[10] __attribute__((scn_string_literal)) = "E1032P047";
static const char data_40154[10] __attribute__((scn_string_literal)) = "E1032A201";
static const char data_4015E[10] __attribute__((scn_string_literal)) = "E1032P049";
static const char data_40168[10] __attribute__((scn_string_literal)) = "E1032A176";
static const char data_40172[10] __attribute__((scn_string_literal)) = "E1032P050";
static const char data_4017C[10] __attribute__((scn_string_literal)) = "E1032A177";
static const char data_40186[10] __attribute__((scn_string_literal)) = "E1032P051";
static const char data_40190[10] __attribute__((scn_string_literal)) = "E1032P001";
static const char data_4019A[10] __attribute__((scn_string_literal)) = "E1032A164";
static const char data_401A4[10] __attribute__((scn_string_literal)) = "E1032P018";
static const char data_401AE[10] __attribute__((scn_string_literal)) = "E1032A178";
static const char data_401B8[10] __attribute__((scn_string_literal)) = "E1032P004";
static const char data_401C2[10] __attribute__((scn_string_literal)) = "E1032P052";
static const char data_401CC[10] __attribute__((scn_string_literal)) = "E1032A179";
static const char data_401D6[10] __attribute__((scn_string_literal)) = "E1032A180";
static const char data_401E0[10] __attribute__((scn_string_literal)) = "E1032A181";
static const char data_401EA[10] __attribute__((scn_string_literal)) = "E1032P053";
static const char data_401F4[10] __attribute__((scn_string_literal)) = "E1032P087";
static const char data_401FE[10] __attribute__((scn_string_literal)) = "E1032P055";
static const char data_40208[10] __attribute__((scn_string_literal)) = "E1032A182";
static const char data_40212[10] __attribute__((scn_string_literal)) = "E1032P051";
static const char data_4021C[10] __attribute__((scn_string_literal)) = "E1032P001";
static const char data_40226[10] __attribute__((scn_string_literal)) = "E1032A164";
static const char data_40230[10] __attribute__((scn_string_literal)) = "E1032P018";
static const char data_4023A[10] __attribute__((scn_string_literal)) = "E1032A183";
static const char data_40244[10] __attribute__((scn_string_literal)) = "E1032P038";
static const char data_4024E[10] __attribute__((scn_string_literal)) = "E1032A184";
static const char data_40258[10] __attribute__((scn_string_literal)) = "E1032P056";
static const char data_40262[10] __attribute__((scn_string_literal)) = "E1032P088";
static const char data_4026C[10] __attribute__((scn_string_literal)) = "E1032P058";
static const char data_40276[10] __attribute__((scn_string_literal)) = "E1032A220";
static const char data_40280[10] __attribute__((scn_string_literal)) = "E1032A185";
static const char data_4028A[10] __attribute__((scn_string_literal)) = "E1032P059";
static const char data_40294[10] __attribute__((scn_string_literal)) = "E1032P001";
static const char data_4029E[10] __attribute__((scn_string_literal)) = "E1032A164";
static const char data_402A8[10] __attribute__((scn_string_literal)) = "E1032P018";
static const char data_402B2[10] __attribute__((scn_string_literal)) = "E1032P060";
static const char data_402BC[10] __attribute__((scn_string_literal)) = "E1032A186";
static const char data_402C6[10] __attribute__((scn_string_literal)) = "E1032P061";
static const char data_402D0[10] __attribute__((scn_string_literal)) = "E1032A187";
static const char data_402DA[10] __attribute__((scn_string_literal)) = "E1032P062";
static const char data_402E4[10] __attribute__((scn_string_literal)) = "E1032A188";
static const char data_402EE[10] __attribute__((scn_string_literal)) = "E1032P063";
static const char data_402F8[10] __attribute__((scn_string_literal)) = "E1032P064";
static const char data_40302[10] __attribute__((scn_string_literal)) = "E1032P065";
static const char data_4030C[10] __attribute__((scn_string_literal)) = "E1032A189";
static const char data_40316[10] __attribute__((scn_string_literal)) = "E1032P066";
static const char data_40320[10] __attribute__((scn_string_literal)) = "E1032A190";
static const char data_4032A[10] __attribute__((scn_string_literal)) = "E1032P001";
static const char data_40334[10] __attribute__((scn_string_literal)) = "E1032A152";
static const char data_4033E[10] __attribute__((scn_string_literal)) = "E1032P067";
static const char data_40348[10] __attribute__((scn_string_literal)) = "E1032A191";
static const char data_40352[10] __attribute__((scn_string_literal)) = "E1032A192";
static const char data_4035C[10] __attribute__((scn_string_literal)) = "E1032P068";
static const char data_40366[10] __attribute__((scn_string_literal)) = "E1032P069";
static const char data_40370[10] __attribute__((scn_string_literal)) = "E1032A193";
static const char data_4037A[10] __attribute__((scn_string_literal)) = "E1032P001";
static const char data_40384[10] __attribute__((scn_string_literal)) = "E1032A152";
static const char data_4038E[10] __attribute__((scn_string_literal)) = "E1032P070";
static const char data_40398[10] __attribute__((scn_string_literal)) = "E1032A194";
static const char data_403A2[10] __attribute__((scn_string_literal)) = "E1032P071";
static const char data_403AC[10] __attribute__((scn_string_literal)) = "E1032A195";
static const char data_403B6[10] __attribute__((scn_string_literal)) = "E1032P072";
static const char data_403C0[10] __attribute__((scn_string_literal)) = "E1032P073";
static const char data_403CA[10] __attribute__((scn_string_literal)) = "E1032A151";
static const char data_403D4[10] __attribute__((scn_string_literal)) = "E1032P074";
static const char data_403DE[10] __attribute__((scn_string_literal)) = "E1032P075";
static const char data_403E8[10] __attribute__((scn_string_literal)) = "E1032P089";
static const char data_403F2[10] __attribute__((scn_string_literal)) = "E1032P090";
static const char data_403FC[10] __attribute__((scn_string_literal)) = "E1032A196";
static const char data_40406[10] __attribute__((scn_string_literal)) = "E1032P079";
static const char data_40410[10] __attribute__((scn_string_literal)) = "E1032A197";
static const char data_4041A[10] __attribute__((scn_string_literal)) = "E1032P001";
static const char data_40424[10] __attribute__((scn_string_literal)) = "E1032A152";
static const char data_4042E[10] __attribute__((scn_string_literal)) = "E1032P010";
static const char data_40438[10] __attribute__((scn_string_literal)) = "E1032P091";
static const char data_40442[10] __attribute__((scn_string_literal)) = "E1032P092";
static const char data_4044C[10] __attribute__((scn_string_literal)) = "E1032A203";
static const char data_40456[10] __attribute__((scn_string_literal)) = "E1032P093";
static const char data_40460[10] __attribute__((scn_string_literal)) = "E1032P094";
static const char data_4046A[10] __attribute__((scn_string_literal)) = "E1032A204";
static const char data_40474[10] __attribute__((scn_string_literal)) = "E1032P095";
static const char data_4047E[10] __attribute__((scn_string_literal)) = "E1032P021";
static const char data_40488[10] __attribute__((scn_string_literal)) = "E1032A152";
static const char data_40492[10] __attribute__((scn_string_literal)) = "E1032P096";
static const char data_4049C[10] __attribute__((scn_string_literal)) = "E1032A104";
static const char data_404A6[10] __attribute__((scn_string_literal)) = "E1032P097";
static const char data_404B0[10] __attribute__((scn_string_literal)) = "E1032A103";
static const char data_404BA[10] __attribute__((scn_string_literal)) = "E1032P098";
static const char data_404C4[10] __attribute__((scn_string_literal)) = "E1032P099";
static const char data_404CE[10] __attribute__((scn_string_literal)) = "E1032P100";
static const char data_404D8[10] __attribute__((scn_string_literal)) = "E1032A206";
static const char data_404E2[10] __attribute__((scn_string_literal)) = "E1032P021";
static const char data_404EC[10] __attribute__((scn_string_literal)) = "E1032A152";
static const char data_404F6[10] __attribute__((scn_string_literal)) = "E1032P101";
static const char data_40500[10] __attribute__((scn_string_literal)) = "E1032A207";
static const char data_4050A[10] __attribute__((scn_string_literal)) = "E1032P102";
static const char data_40514[10] __attribute__((scn_string_literal)) = "E1032A208";
static const char data_4051E[10] __attribute__((scn_string_literal)) = "E1032P103";
static const char data_40528[10] __attribute__((scn_string_literal)) = "E1032A209";
static const char data_40532[10] __attribute__((scn_string_literal)) = "E1032P104";
static const char data_4053C[10] __attribute__((scn_string_literal)) = "E1032P105";
static const char data_40546[10] __attribute__((scn_string_literal)) = "E1032A210";
static const char data_40550[10] __attribute__((scn_string_literal)) = "E1032P106";
static const char data_4055A[10] __attribute__((scn_string_literal)) = "E1032A154";
static const char data_40564[10] __attribute__((scn_string_literal)) = "E1032P107";
static const char data_4056E[10] __attribute__((scn_string_literal)) = "E1032A204";
static const char data_40578[10] __attribute__((scn_string_literal)) = "E1032P108";
static const char data_40582[10] __attribute__((scn_string_literal)) = "E1032P109";
static const char data_4058C[10] __attribute__((scn_string_literal)) = "E1032A085";
static const char data_40596[10] __attribute__((scn_string_literal)) = "E1032P110";
static const char data_405A0[10] __attribute__((scn_string_literal)) = "E1032A211";
static const char data_405AA[10] __attribute__((scn_string_literal)) = "E1032P111";
static const char data_405B4[10] __attribute__((scn_string_literal)) = "E1032A104";
static const char data_405BE[10] __attribute__((scn_string_literal)) = "E1032P112";
static const char data_405C8[10] __attribute__((scn_string_literal)) = "E1032A212";
static const char data_405D2[10] __attribute__((scn_string_literal)) = "E1032P113";
static const char data_405DC[10] __attribute__((scn_string_literal)) = "E1032A158";
static const char data_405E6[10] __attribute__((scn_string_literal)) = "E1032P114";
static const char data_405F0[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_405FA[10] __attribute__((scn_string_literal)) = "E1032P115";
static const char data_40604[10] __attribute__((scn_string_literal)) = "E1032A213";
static const char data_4060E[10] __attribute__((scn_string_literal)) = "E1032P116";
static const char data_40618[10] __attribute__((scn_string_literal)) = "E1032P001";
static const char data_40622[10] __attribute__((scn_string_literal)) = "E1032A152";
static const char data_4062C[10] __attribute__((scn_string_literal)) = "E1032P117";
static const char data_40636[10] __attribute__((scn_string_literal)) = "E1032A214";
static const char data_40640[10] __attribute__((scn_string_literal)) = "E1032P118";
static const char data_4064A[10] __attribute__((scn_string_literal)) = "E1032A154";
static const char data_40654[10] __attribute__((scn_string_literal)) = "E1032P119";
static const char data_4065E[10] __attribute__((scn_string_literal)) = "E1032P120";
static const char data_40668[10] __attribute__((scn_string_literal)) = "E1032A215";
static const char data_40672[10] __attribute__((scn_string_literal)) = "E1032P121";
static const char data_4067C[10] __attribute__((scn_string_literal)) = "E1032A163";
static const char data_40686[10] __attribute__((scn_string_literal)) = "E1032P123";
static const char data_40690[10] __attribute__((scn_string_literal)) = "E1032A217";
static const char data_4069A[10] __attribute__((scn_string_literal)) = "E1032P001";
static const char data_406A4[10] __attribute__((scn_string_literal)) = "E1032A152";
static const char data_406AE[10] __attribute__((scn_string_literal)) = "E1032P010";
static const char data_406B8[10] __attribute__((scn_string_literal)) = "E1032P124";
static const char data_406C2[10] __attribute__((scn_string_literal)) = "E1032P125";
static const char data_406CC[10] __attribute__((scn_string_literal)) = "E1032A218";
static const char data_406D6[10] __attribute__((scn_string_literal)) = "E1032P126";
static const char data_406E0[10] __attribute__((scn_string_literal)) = "E1032A154";
static const char data_406EA[10] __attribute__((scn_string_literal)) = "E1032P127";
static const char data_406F4[10] __attribute__((scn_string_literal)) = "E1032A213";
static const char data_406FE[10] __attribute__((scn_string_literal)) = "E1032P128";
static const char data_40708[10] __attribute__((scn_string_literal)) = "E1032A204";
static const char data_40712[10] __attribute__((scn_string_literal)) = "E1032P116";
static const char data_4071C[10] __attribute__((scn_string_literal)) = "E1032P109";
static const char data_40726[10] __attribute__((scn_string_literal)) = "E1032A085";
static const char data_40730[10] __attribute__((scn_string_literal)) = "E1032P117";
static const char data_4073A[10] __attribute__((scn_string_literal)) = "E1032A214";
static const char data_40744[10] __attribute__((scn_string_literal)) = "E1032P129";
static const char data_4074E[10] __attribute__((scn_string_literal)) = "E1032P130";
static const char data_40758[10] __attribute__((scn_string_literal)) = "E1032P131";
static const char data_40762[10] __attribute__((scn_string_literal)) = "E1032A213";
static const char data_4076C[10] __attribute__((scn_string_literal)) = "E1032A219";
static const char data_40776[10] __attribute__((scn_string_literal)) = "E1032P132";
static const char data_40780[10] __attribute__((scn_string_literal)) = "E1032A153";
static const char data_4078A[10] __attribute__((scn_string_literal)) = "E1032P133";
static const char data_40794[10] __attribute__((scn_string_literal)) = "E1032A220";
static const char data_4079E[10] __attribute__((scn_string_literal)) = "E1032P134";
static const char data_407A8[10] __attribute__((scn_string_literal)) = "E1032A204";
static const char data_407B2[10] __attribute__((scn_string_literal)) = "E1032P116";
static const char data_407BC[10] __attribute__((scn_string_literal)) = "E1032A085";
static const char data_407C6[10] __attribute__((scn_string_literal)) = "E1032P010";
static const char data_407D0[10] __attribute__((scn_string_literal)) = "E1032A221";
static const char data_407DA[10] __attribute__((scn_string_literal)) = "E1032P135";
static const char data_407E4[10] __attribute__((scn_string_literal)) = "E1032P136";
static const char data_407EE[10] __attribute__((scn_string_literal)) = "E1032A222";
static const char data_407F8[10] __attribute__((scn_string_literal)) = "E1032P137";
static const char data_40802[10] __attribute__((scn_string_literal)) = "E1032P138";
static const char data_4080C[10] __attribute__((scn_string_literal)) = "E1032A223";
static const char data_40816[10] __attribute__((scn_string_literal)) = "E1032P139";
static const char data_40820[10] __attribute__((scn_string_literal)) = "E1032A224";
static const char data_4082A[10] __attribute__((scn_string_literal)) = "E1032P140";
static const char data_40834[10] __attribute__((scn_string_literal)) = "E1032A225";
static const char data_4083E[10] __attribute__((scn_string_literal)) = "E1032A226";
static const char data_40848[10] __attribute__((scn_string_literal)) = "E1032P004";
static const char data_40852[10] __attribute__((scn_string_literal)) = "E1032E001";
static const char data_4085C[10] __attribute__((scn_string_literal)) = "E1032E002";
static const char data_40866[10] __attribute__((scn_string_literal)) = "E1032E003";
static const char data_40870[10] __attribute__((scn_string_literal)) = "E1032A010";
static const char data_4087A[10] __attribute__((scn_string_literal)) = "E1032E004";
static const char data_40884[10] __attribute__((scn_string_literal)) = "E1032A011";
static const char data_4088E[10] __attribute__((scn_string_literal)) = "E1032E005";
static const char data_40898[10] __attribute__((scn_string_literal)) = "E1032A012";
static const char data_408A2[10] __attribute__((scn_string_literal)) = "E1032A013";
static const char data_408AC[10] __attribute__((scn_string_literal)) = "E1032E006";
static const char data_408B6[10] __attribute__((scn_string_literal)) = "E1032A014";
static const char data_408C0[10] __attribute__((scn_string_literal)) = "E1032A015";
static const char data_408CA[10] __attribute__((scn_string_literal)) = "E1032E007";
static const char data_408D4[10] __attribute__((scn_string_literal)) = "E1032A016";
static const char data_408DE[10] __attribute__((scn_string_literal)) = "E1032E008";
static const char data_408E8[10] __attribute__((scn_string_literal)) = "E1032E009";
static const char data_408F2[10] __attribute__((scn_string_literal)) = "E1032A017";
static const char data_408FC[10] __attribute__((scn_string_literal)) = "E1032E010";
static const char data_40906[10] __attribute__((scn_string_literal)) = "E1032E011";
static const char data_40910[10] __attribute__((scn_string_literal)) = "E1032E012";
static const char data_4091A[10] __attribute__((scn_string_literal)) = "E1032E013";
static const char data_40924[10] __attribute__((scn_string_literal)) = "E1032A018";
static const char data_4092E[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_40938[10] __attribute__((scn_string_literal)) = "E1032A020";
static const char data_40942[10] __attribute__((scn_string_literal)) = "E1032A021";
static const char data_4094C[10] __attribute__((scn_string_literal)) = "E1032F001";
static const char data_40956[10] __attribute__((scn_string_literal)) = "E1032F002";
static const char data_40960[10] __attribute__((scn_string_literal)) = "E1032F003";
static const char data_4096A[10] __attribute__((scn_string_literal)) = "E1032A022";
static const char data_40974[10] __attribute__((scn_string_literal)) = "E1032F004";
static const char data_4097E[10] __attribute__((scn_string_literal)) = "E1032A011";
static const char data_40988[10] __attribute__((scn_string_literal)) = "E1032F005";
static const char data_40992[10] __attribute__((scn_string_literal)) = "E1032A012";
static const char data_4099C[10] __attribute__((scn_string_literal)) = "E1032A013";
static const char data_409A6[10] __attribute__((scn_string_literal)) = "E1032F006";
static const char data_409B0[10] __attribute__((scn_string_literal)) = "E1032A023";
static const char data_409BA[10] __attribute__((scn_string_literal)) = "E1032A024";
static const char data_409C4[10] __attribute__((scn_string_literal)) = "E1032F007";
static const char data_409CE[10] __attribute__((scn_string_literal)) = "E1032A025";
static const char data_409D8[10] __attribute__((scn_string_literal)) = "E1032A026";
static const char data_409E2[10] __attribute__((scn_string_literal)) = "E1032F008";
static const char data_409EC[10] __attribute__((scn_string_literal)) = "E1032F009";
static const char data_409F6[10] __attribute__((scn_string_literal)) = "E1032F010";
static const char data_40A00[10] __attribute__((scn_string_literal)) = "E1032F011";
static const char data_40A0A[10] __attribute__((scn_string_literal)) = "E1032F012";
static const char data_40A14[10] __attribute__((scn_string_literal)) = "E1032F013";
static const char data_40A1E[10] __attribute__((scn_string_literal)) = "E1032F014";
static const char data_40A28[10] __attribute__((scn_string_literal)) = "E1032F015";
static const char data_40A32[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_40A3C[10] __attribute__((scn_string_literal)) = "E1032A018";
static const char data_40A46[10] __attribute__((scn_string_literal)) = "E1032A027";
static const char data_40A50[10] __attribute__((scn_string_literal)) = "E1032A034";
static const char data_40A5A[10] __attribute__((scn_string_literal)) = "E1032H005";
static const char data_40A64[10] __attribute__((scn_string_literal)) = "E1032H004";
static const char data_40A6E[10] __attribute__((scn_string_literal)) = "E1032H009";
static const char data_40A78[10] __attribute__((scn_string_literal)) = "E1032H006";
static const char data_40A82[10] __attribute__((scn_string_literal)) = "E1032A042";
static const char data_40A8C[10] __attribute__((scn_string_literal)) = "E1032A040";
static const char data_40A96[10] __attribute__((scn_string_literal)) = "E1032A043";
static const char data_40AA0[10] __attribute__((scn_string_literal)) = "E1032A041";
static const char data_40AAA[10] __attribute__((scn_string_literal)) = "E1032A045";
static const char data_40AB4[10] __attribute__((scn_string_literal)) = "E1032A046";
static const char data_40ABE[10] __attribute__((scn_string_literal)) = "E1032A047";
static const char data_40AC8[10] __attribute__((scn_string_literal)) = "E1032A046";
static const char data_40AD2[10] __attribute__((scn_string_literal)) = "E1032A047";
static const char data_40ADC[10] __attribute__((scn_string_literal)) = "E1032A044";
static const char data_40AE6[10] __attribute__((scn_string_literal)) = "E1032A051";
static const char data_40AF0[10] __attribute__((scn_string_literal)) = "E1032A039";
static const char data_40AFA[10] __attribute__((scn_string_literal)) = "E1032H007";
static const char data_40B04[10] __attribute__((scn_string_literal)) = "E1032A120";
static const char data_40B0E[10] __attribute__((scn_string_literal)) = "E1032H010";
static const char data_40B18[10] __attribute__((scn_string_literal)) = "E1032A066";
static const char data_40B22[10] __attribute__((scn_string_literal)) = "E1032H011";
static const char data_40B2C[10] __attribute__((scn_string_literal)) = "E1032J001";
static const char data_40B36[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_40B40[10] __attribute__((scn_string_literal)) = "E1032A131";
static const char data_40B4A[10] __attribute__((scn_string_literal)) = "E1032J010";
static const char data_40B54[10] __attribute__((scn_string_literal)) = "E1032J013";
static const char data_40B5E[10] __attribute__((scn_string_literal)) = "E1032J001";
static const char data_40B68[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_40B72[10] __attribute__((scn_string_literal)) = "E1032A134";
static const char data_40B7C[10] __attribute__((scn_string_literal)) = "E1032J015";
static const char data_40B86[10] __attribute__((scn_string_literal)) = "E1032A135";
static const char data_40B90[10] __attribute__((scn_string_literal)) = "E1032J001";
static const char data_40B9A[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_40BA4[10] __attribute__((scn_string_literal)) = "E1032J018";
static const char data_40BAE[10] __attribute__((scn_string_literal)) = "E1032A163";
static const char data_40BB8[10] __attribute__((scn_string_literal)) = "E1032J019";
static const char data_40BC2[10] __attribute__((scn_string_literal)) = "E1032J020";
static const char data_40BCC[10] __attribute__((scn_string_literal)) = "E1032A068";
static const char data_40BD6[10] __attribute__((scn_string_literal)) = "E1032J016";
static const char data_40BE0[10] __attribute__((scn_string_literal)) = "E1032A161";
static const char data_40BEA[10] __attribute__((scn_string_literal)) = "E1032J017";
static const char data_40BF4[10] __attribute__((scn_string_literal)) = "E1032A162";
static const char data_40BFE[10] __attribute__((scn_string_literal)) = "E1032J001";
static const char data_40C08[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_40C12[10] __attribute__((scn_string_literal)) = "E1032A131";
static const char data_40C1C[10] __attribute__((scn_string_literal)) = "E1032J010";
static const char data_40C26[10] __attribute__((scn_string_literal)) = "E1032A132";
static const char data_40C30[10] __attribute__((scn_string_literal)) = "E1032J011";
static const char data_40C3A[10] __attribute__((scn_string_literal)) = "E1032J012";
static const char data_40C44[10] __attribute__((scn_string_literal)) = "E1032H007";
static const char data_40C4E[10] __attribute__((scn_string_literal)) = "E1032A120";
static const char data_40C58[10] __attribute__((scn_string_literal)) = "E1032H010";
static const char data_40C62[10] __attribute__((scn_string_literal)) = "E1032A066";
static const char data_40C6C[10] __attribute__((scn_string_literal)) = "E1032H011";
static const char data_40C76[10] __attribute__((scn_string_literal)) = "E1032J001";
static const char data_40C80[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_40C8A[10] __attribute__((scn_string_literal)) = "E1032J018";
static const char data_40C94[10] __attribute__((scn_string_literal)) = "E1032A163";
static const char data_40C9E[10] __attribute__((scn_string_literal)) = "E1032J019";
static const char data_40CA8[10] __attribute__((scn_string_literal)) = "E1032J020";
static const char data_40CB2[10] __attribute__((scn_string_literal)) = "E1032A068";
static const char data_40CBC[10] __attribute__((scn_string_literal)) = "E1032J016";
static const char data_40CC6[10] __attribute__((scn_string_literal)) = "E1032A161";
static const char data_40CD0[10] __attribute__((scn_string_literal)) = "E1032J017";
static const char data_40CDA[10] __attribute__((scn_string_literal)) = "E1032A162";
static const char data_40CE4[10] __attribute__((scn_string_literal)) = "E1032J001";
static const char data_40CEE[10] __attribute__((scn_string_literal)) = "E1032A067";
static const char data_40CF8[10] __attribute__((scn_string_literal)) = "E1032J002";
static const char data_40D02[10] __attribute__((scn_string_literal)) = "E1032A068";
static const char data_40D0C[10] __attribute__((scn_string_literal)) = "E1032J001";
static const char data_40D16[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_40D20[10] __attribute__((scn_string_literal)) = "E1032J003";
static const char data_40D2A[10] __attribute__((scn_string_literal)) = "E1032A069";
static const char data_40D34[10] __attribute__((scn_string_literal)) = "E1032J004";
static const char data_40D3E[10] __attribute__((scn_string_literal)) = "E1032A070";
static const char data_40D48[10] __attribute__((scn_string_literal)) = "E1032H007";
static const char data_40D52[10] __attribute__((scn_string_literal)) = "E1032A048";
static const char data_40D5C[10] __attribute__((scn_string_literal)) = "E1032H008";
static const char data_40D66[10] __attribute__((scn_string_literal)) = "E1032A049";
static const char data_40D70[10] __attribute__((scn_string_literal)) = "E1032A050";
static const char data_40D7A[14] __attribute__((scn_string_literal)) = "M_TPIN.BIN";
static const int8_t data_40D88[16] = {12, 17, 22, 10, 19, 18, 11, 8, 4, 4, 17, 7, 6, 5, 4, 4};
static const int8_t data_40D98[16] = {12, 1, 2, 1, 3, 2, 2, 8, 4, 4, 17, 7, 6, 5, 4, 4};
static const uint32_t data_40DA8 = 0xA800A7;
static const int32_t data_40DAC[5] = {8847529, 8978568, 15073509, 15204583, 15335657};
static const uint32_t data_40DC0 = 0x440043;
static const int32_t data_40DC4[8] = {4587589, 4718663, 4849737, 4980811, 5111885, 5242959, 5374033, 85};
static const uint32_t data_40DE4 = 0x5C005B;
static const int32_t data_40DE8[10] = {6160477, 6291551, 6422625, 6553699, 6684773, 6815847, 6946921, 7077995, 7209069, 7340143};
static const uint32_t data_40E10 = 0x740073;
static const int32_t data_40E14[4] = {7798902, 7929976, 8061050, 8257660};
static const uint32_t data_40E24 = 0x720071;
static const int32_t data_40E28[9] = {10223750, 10354845, 10485919, 10616993, 10748067, 10879141, 11206826, 5767255, 180};
static const uint32_t data_40E4C = 0x810080;
static const int32_t data_40E50[8] = {8585346, 11403437, 11534511, 11665585, 11862195, 11993270, 12452024, 8650879};
static const uint32_t data_40E70 = 0xB900BC;
static const int32_t data_40E74[5] = {12255418, 13238473, 13500621, 12517579, 200};
static const uint32_t data_40E88 = 0x5A0059;
static const int32_t data_40E8C[3] = {5636268, 14221445, 5505107};
static const uint32_t data_40E98 = 0x7D0075;
static const int32_t data_40E9C = 12386442;
static const uint32_t data_40EA0 = 0xC600C5;
static const int32_t data_40EA4 = 13369543;
static const uint32_t data_40EA8 = 0x8C008B;
static const int32_t data_40EAC[8] = {9306253, 9437327, 9633937, 148, 9961623, 10092697, 9568411, 149};
static const uint32_t data_40ECC = 0xD700D6;
static const int32_t data_40ED0[3] = {14287064, 14876891, 228};
static const uint32_t data_40EDC = 0xDC00D3;
static const int32_t data_40EE0[2] = {13893845, 14549213};
static const uint32_t data_40EE8 = 0xC100C0;
static const int32_t data_40EEC[2] = {12779714, 196};
static const uint32_t data_40EF4 = 0xD000CF;
static const int32_t data_40EF8 = 13762769;
static const uint32_t data_40EFC = 0xE000DF;
static const int32_t data_40F00 = 14811361;
static const char data_40F04[169] = "%GACR016G.MT5\000%GACK66BG.MT5\000%GACK66RG.MT5\000%GACR0BNG.MT5\000%GACR0B2G.MT5\000%GACR0B3G.MT5\000%SPBK6REG.MT5\000%SPBK6BRG.MT5\000%SPBK6YEG.MT5\000%SAIK6WHG.MT5\000%SAIK6BKG.MT5\000%SAIK6REG.MT5";
static const char data_40FAD[315] = "%GACIAK1G.MT5\000%GACIAK2G.MT5\000%GACOJK1G.MT5\000%GACOJK2G.MT5\000%GACOSA1G.MT5\000%GACOSA2G.MT5\000%GACOLA1G.MT5\000%GACOLA2G.MT5\000%GACOPA1G.MT5\000%GACOPA2G.MT5\000%GACGWF1G.MT5\000%GACGWF2G.MT5\000%GACGJF1G.MT5\000%GACGJF2G.MT5\000%GACIKA1G.MT5\000%GACIKA2G.MT5\000%GACM401G.MT5\000%GACIAA1G.MT5\000%GACIAA2G.MT5\000D\000D\000D\000D\000D\000D\000D\000D\000D\000D\000%GACGJJ1G.MT5\000%GACGJJ2G.MT5";
static const char data_410E8[309] = "%GACIAKAG.MT5\000%GACIAKBG.MT5\000%GACGJKAG.MT5\000%GACGJKBG.MT5\000%GACOSAAG.MT5\000%GACOSABG.MT5\000%GACOLAAG.MT5\000%GACOLABG.MT5\000%GACOPAAG.MT5\000%GACOPABG.MT5\000%GACIWLAG.MT5\000%GACIWLBG.MT5\000%GACGJFAG.MT5\000%GACGJFBG.MT5\000%GACIKAAG.MT5\000%GACIKABG.MT5\000%GACGSNAG.MT5\000%GACGSNBG.MT5\000%GACILIAG.MT5\000%GACILIBG.MT5\000%GACIDUAG.MT5\000%GACIDUBG.MT5";
static const char data_4121D[167] = "%GACRSO1G.MT5\000%GACRSO2G.MT5\000%GACRTL1G.MT5\000%GACRNUCG.MT5\000%GACRBAKG.MT5\000%GACRBING.MT5\000%GACRFNGG.MT5\000%GACRAMYG.MT5\000%GACRESPG.MT5\000%GACREGGG.MT5\000D\000D\000D\000D\000D\000D\000%GACRFNNG.MT5";
static const char data_412C4[365] = "%GACRNT1G.MT5\000%GACRNT2G.MT5\000%GACRLOPG.MT5\000%GACRMYUG.MT5\000%GACRCKNG.MT5\000%GACRALFG.MT5\000%GACRRISG.MT5\000%GACRARXG.MT5\000%GACR0OPG.MT5\000%GACR0O2G.MT5\000%GACRPOPG.MT5\000%GACRCOBG.MT5\000%GACRIDAG.MT5\000%GACRSHPG.MT5\000%GACRMOBG.MT5\000%GACRROBG.MT5\000%GACK6TZG.MT5\000%GACRPANG.MT5\000%GACK6DYG.MT5\000D\000D\000D\000D\000D\000D\000D\000%GACRAXXG.MT5\000D\000D\000D\000D\000D\000D\000D\000%GACRRBBG.MT5\000%GACK6TSG.MT5\000%GACRPPNG.MT5\000%GACK6DSG.MT5";
static const char data_41431[505] = "%GACK6HHG.MT5\000%GACK6HYG.MT5\000%GACK6HBG.MT5\000%GACK6HGG.MT5\000%GACH5BKG.MT5\000%KAB02FSG.MT5\000%GACO1K2G.MT5\000%GACO1K3G.MT5\000%GACO1K7G.MT5\000%GACO1K8G.MT5\000%GACO1K9G.MT5\000%GACO1K1G.MT5\000%GACO1K4G.MT5\000%GACO1K5G.MT5\000%GACO1K6G.MT5\000%GACS5BSG.MT5\000%GACK6HOG.MT5\000%GACK6HKG.MT5\000%GACK6XHG.MT5\000%GACK6XYG.MT5\000%GACK6XBG.MT5\000%GACK6XGG.MT5\000%GACK6BKG.MT5\000%KABK6FSG.MT5\000%GACOKK2G.MT5\000%GACOKK3G.MT5\000%GACOKK7G.MT5\000%GACOKK8G.MT5\000%GACOKK9G.MT5\000%GACOKK1G.MT5\000%GACOKK4G.MT5\000%GACOKK5G.MT5\000%GACOKK6G.MT5\000%GACS5BBG.MT5\000%GACK6XOG.MT5\000%GACK6XKG.MT5";
static const char data_4162A[211] = "%GACH5TKG.MT5\000%GACGTRXG.MT5\000%GACO1T1G.MT5\000%GACO1T2G.MT5\000%GACO1KOG.MT5\000%GACO1KBG.MT5\000%GACO1KJG.MT5\000%GACH5UKG.MT5\000%GACH5IKG.MT5\000%GACO1FOG.MT5\000%GACGSHEG.MT5\000%GACH5TTG.MT5\000%GACGTXXG.MT5\000%GACOTT1G.MT5\000%GACOTT2G.MT5";
static const char data_416FD[113] = "%YKW0201G.MT5\000%YWT0201G.MT5\000%GACRBONG.MT5\000%GACRRENG.MT5\000%GACK6SPG.MT5\000%GACG360G.MT5\000%GACRDR1G.MT5\000%GACRDR2G.MT5";
static const char data_4176E[77] = "%GACRSSCG.MT5\000%GACRMETG.MT5\000%GACRGBIG.MT5\000%GACH5TRG.MT5\000D\000D\000D\000%GACH5RRG.MT5";
static const char data_417BB[57] = "%GACS5FRG.MT5\000%GACS5FBG.MT5\000%GACO1KUG.MT5\000%GACO1FEG.MT5";
static const char data_417F4[239] = "%GACODG1G.MT5\000%GACODG2G.MT5\000%GACODG3G.MT5\000%GACOCT2G.MT5\000%GACOCATG.MT5\000%GACO1SHG.MT5\000%GACOKSHG.MT5\000%GACO1IRG.MT5\000%GACOKIRG.MT5\000%GACO1HAG.MT5\000%GACOHA2G.MT5\000%GACOHA3G.MT5\000%GACOHA4G.MT5\000%GACO1YGG.MT5\000%GACO1USG.MT5\000%GACOOSHG.MT5\000%GACOOIRG.MT5";
static const char data_418E3[99] = "%GACM400G.MT5\000%GACGSPHG.MT5\000%GACS5HGG.MT5\000%GACGDT1G.MT5\000%GACS5EQG.MT5\000%SYOGSH0G.MT5\000%SYOGHG0G.MT5";
static const char data_41946[85] = "%GACGCRYG.MT5\000%GACGBIRG.MT5\000%GACH5SLG.MT5\000%GACH5JKG.MT5\000%SYOGMD1G.MT5\000%SYOGMD5G.MT5";
static const char data_4199B[71] = "%GACS5N1G.MT5\000%GACS5N2G.MT5\000%GACS5N3G.MT5\000%GACS5N4G.MT5\000%GACS5N5G.MT5";
static const char data_419E2[57] = "%GACGXBNG.MT5\000%GACGYBNG.MT5\000%GACGABNG.MT5\000%GACGBBNG.MT5";
static const char data_41A1B[57] = "%GACRMEGG.MT5\000%GACR0GGG.MT5\000%GACRSATG.MT5\000%GACRCASG.MT5";
static const uint32_t data_41A54 = SHEN_NAME('GCH0');
static const shen_name data_41A58[5] = {SHEN_NAME('GCH1'), SHEN_NAME('GCH2'), SHEN_NAME('GCH3'), SHEN_NAME('GCH4'), SHEN_NAME('GCH5')};
static const uint32_t data_41A6C = SHEN_NAME('HGC0');
static const shen_name data_41A70[5] = {SHEN_NAME('HGC1'), SHEN_NAME('HGC2'), SHEN_NAME('HGC3'), SHEN_NAME('HGC4'), SHEN_NAME('HGC5')};
static const uint32_t data_41A84 = SHEN_NAME('GBX0');
static const shen_name data_41A88[5] = {SHEN_NAME('GBX1'), SHEN_NAME('GBX2'), SHEN_NAME('GBX3'), SHEN_NAME('GBX4'), SHEN_NAME('GBX5')};
static const uint32_t data_41A9C = 0x41C40000;
typedef struct Row_41AA0 { int32_t w0; float w1; int32_t w2; int32_t w3; float w4; } Row_41AA0;
static const Row_41AA0 data_41AA0[5] = {
  {0, 32.16f, 32768, 516, 0.0f},
  {0, 0.0f, 0, 0, 0.0f},
  {0, 0.0f, 0, 0, 23.75f},
  {0, 32.16f, 32768, 770, 0.0f},
  {0, 0.0f, 0, 0, 0.0f}
};
static const int32_t data_41B04[4] = {0, 0, 0, 0};
static const Vector3 data_41B14 = {.x = 0.29f, .y = 0.0f, .z = 0.088f};
static const Vector3i data_41B20 = {.x = 0, .y = 0, .z = 0};
static const char data_41B2C[6] __attribute__((scn_string_literal)) = "E1066";
static const char data_41B32[13] __attribute__((scn_string_literal)) = "e1gachap.snd";
static const char data_41B3F[16] __attribute__((scn_string_literal)) = "/scene/02/D000/";
static const char data_41B4F[8] __attribute__((scn_string_literal)) = "HI_GACH";
static const char data_41B57[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_41B5C[6] __attribute__((scn_string_literal)) = "E1066";
static const char data_41B62[8] __attribute__((scn_string_literal)) = "HI_GACH";
static const char data_41B6A[10] __attribute__((scn_string_literal)) = "HI_GACH";
static const Vector4 data_41B74 = {.x = 0.0f, .y = 0.7f, .z = 0.7f, .w = 0.7f};
static const uint8_t data_41B84[4] = {0};
static const int32_t data_41B88[56] = {
  -546, -5870, 0, 2184, -3726, 0, 182, 0, 0, 0, -5184, 0, 71525, -12075, 0, -182, -5279, 0, -364, 910, 0, 1456, -2894, 0, 3458, -13352, 0, -910,
  -10327, 0, 0, 1820, 0, 0, -6863, 0, 1820, -16438, 0, -364, -10150, 0, 236, 291, 0, 0, -12105, 0, 0, -15539, 0, 0, -7249, 0, -819, -364
};
static const uint8_t data_41C68[4] = {0};
static const int32_t data_41C6C[56] = {
  -546, -5870, 0, 2184, -3726, 0, 182, 0, 0, 0, -5184, 0, 71525, -10922, 0, -182, -5279, 0, -364, 910, 0, 1456, -2894, 0, 3458, -13352, 0, -910,
  -10327, 0, 0, 1820, 0, 0, -6863, 0, 1820, -16438, 0, -364, -10150, 0, 236, 291, 0, 0, -12105, 0, 0, -15539, 0, 0, -7249, 0, -819, -364
};
static const uint8_t data_41D4C[4] = {0};
static const int32_t data_41D50[56] = {
  2912, -7507, 0, 6189, -4454, 0, -1456, 0, 0, 546, -8266, 0, 1292, -7099, 0, 910, -11468, 0, 728, 3276, 0, 1092, -11450, 0, 546, -10256, 0, 1456,
  -6504, 0, 546, 1638, 0, -1092, -12142, 0, -364, -16438, 0, -364, -10150, 0, -309, 291, 0, 1638, -11923, 0, -3276, -17359, 0, -2912, -10707, 0, -819,
  -364
};
static const uint8_t data_41E30[4] = {0};
static const int32_t data_41E34[56] = {
  1638, -7507, 0, 6189, -4454, 0, -1456, 0, 0, 546, -17731, 0, -709, -9466, 0, 728, -15473, 0, 182, 2548, 0, 1092, -13271, 0, -1092, -14625, 0, 1638,
  -11965, 0, 0, 1638, 0, -1092, -12142, 0, -364, -16438, 0, -364, -10150, 0, -309, 291, 0, 1638, -11923, 0, -3276, -17359, 0, -2912, -10707, 0, -819,
  -364
};
static const uint8_t data_41F14[4] = {0};
static const int32_t data_41F18[56] = {
  1638, -6779, 0, 6189, -4454, 0, -1456, 0, 0, 546, -17731, 0, -709, -9466, 0, 1274, -15473, 0, 546, 2548, 0, 1092, -13271, 0, -1092, -14625, 0, 1638,
  -11965, 0, -910, 1638, 0, -1092, -12142, 0, -364, -16438, 0, -364, -10150, 0, -309, 291, 0, 1638, -12105, 0, -3276, -17359, 0, -2912, -10889, 0,
  -819, -364
};
static const uint8_t data_41FF8[4] = {0};
static const int32_t data_41FFC[56] = {
  -2730, -9510, -3094, 4004, -1359, 0, 4004, -728, 0, 546, -6990, 0, 18, -4004, 2366, 1274, -11468, 0, 182, -182, 0, 1092, -13271, 0, -1092, -14443,
  0, 1638, -11965, 0, -910, 1638, 0, -1092, -12142, 0, -364, -16438, 0, -364, -10150, 0, -309, 291, 0, 1638, -12105, 0, -3276, -17359, 0, -2912,
  -10889, 0, -819, -364
};
static const uint8_t data_420DC[8] = {0};
static const int32_t data_420E4[55] = {
  -6005, 0, 4731, -6188, 0, 0, 0, 0, 0, -2727, 0, 0, -9097, 0, 0, -5457, 0, 0, 0, 0, 0, -8734, 0, -909, -7277, 0, 0, -7823, 0, 0, 0, 0, 1456, -4002,
  0, 0, -9462, 0, 726, -7824, 0, 0, 0, 0, 0, -12011, 0, -1091, -7461, 0, -1636, -9280, 0, 0, 0
};
static const uint8_t data_421C0[4] = {0};
static const int32_t data_421C4[56] = {
  -546, -5870, 0, 2184, -3726, 0, 182, 0, 0, 0, -5184, 0, 71525, -12075, 0, -182, -5279, 0, -364, 910, 0, 1456, -2894, 0, 3458, -13352, 0, -910,
  -10327, 0, 0, 1820, 0, 0, -6863, 0, 1820, -16438, 0, -364, -10150, 0, 236, 291, 0, 0, -12105, 0, 0, -15539, 0, 0, -7249, 0, -819, -364
};
static const uint8_t data_422A4[8] = {0};
static const int32_t data_422AC[55] = {
  -8046, 0, 0, -1820, -13088, 4350, -2894, 0, 0, -9011, 0, 0, -18422, 0, 0, -9812, 0, 0, 0, 0, 0, -7518, 0, 0, -17385, 0, 0, -11559, 0, 0, 0, 0, 0,
  -5734, 0, 0, -18058, 0, 0, -12233, 0, 0, 0, 0, 0, -7827, 0, 0, -16183, 0, 0, -13981, 0, -72, 0
};
static const Vector3 data_42388 = {.x = 0.067f, .y = 0.061f, .z = 0.0379f};
static const uint32_t data_42394 = 0x3DB851EC;
static const float data_42398[2] = {0.075f, 0.008f};
static const Vector3i data_423A0 = {.x = 13111, .y = 817, .z = 50684};
static const Vector3 data_423AC = {.x = 0.091f, .y = -0.087f, .z = 0.0671f};
static const Vector3i data_423B8 = {.x = 20024, .y = 65353, .z = 11832};
static const Vector3 data_423C4 = {.x = 0.0f, .y = 0.0f, .z = 0.0f};
static const Vector3i data_423D0 = {.x = 0, .y = 0, .z = 0};
static const Vector3 data_423DC = {.x = -0.042f, .y = 0.1735f, .z = 0.2062f};
static const char data_423E8[] __attribute__((scn_string_literal)) = "E1066A001";
static const char data_423F2[] __attribute__((scn_string_literal)) = "E1066A002";
static const char data_423FC[] __attribute__((scn_string_literal)) = "E1066A003";
static const char data_42406[] __attribute__((scn_string_literal)) = "E1066A004";
static const char data_42410[] __attribute__((scn_string_literal)) = "E1066A005";
static const char data_4241A[] __attribute__((scn_string_literal)) = "E1066A006";
static const char data_42424[] __attribute__((scn_string_literal)) = "E1066A007";
static const char data_4242E[] __attribute__((scn_string_literal)) = "E1066A008";
static const char data_42438[] __attribute__((scn_string_literal)) = "E1066A009";
static const char data_42442[] __attribute__((scn_string_literal)) = "E1066A010";
static const char data_4244C[] __attribute__((scn_string_literal)) = "E1066A011";
static const char data_42456[] __attribute__((scn_string_literal)) = "E1066A026";
static const char data_42460[] __attribute__((scn_string_literal)) = "E1066A027";
static const char data_4246A[] __attribute__((scn_string_literal)) = "E1066A019";
static const char data_42474[] __attribute__((scn_string_literal)) = "E1066A020";
static const char data_4247E[] __attribute__((scn_string_literal)) = "E1066A021";
static const char data_42488[] __attribute__((scn_string_literal)) = "E1066A014";
static const char data_42492[] __attribute__((scn_string_literal)) = "E1066A015";
static const char data_4249C[] __attribute__((scn_string_literal)) = "E1066A016";
static const char data_424A6[] __attribute__((scn_string_literal)) = "E1066A017";
static const char data_424B0[] __attribute__((scn_string_literal)) = "E1066A018";
static const char data_424BA[] __attribute__((scn_string_literal)) = "E1066A022";
static const char data_424C4[] __attribute__((scn_string_literal)) = "E1066A025";
static const char data_424CE[] __attribute__((scn_string_literal)) = "E1066A024";
static const char data_424D8[12] __attribute__((scn_string_literal)) = "E1066A023";
static const Vector3 data_424E4 = {.x = 0.337795f, .y = 0.596267f, .z = -0.181433f};
static const Vector3 data_424F0 = {.x = 0.091f, .y = -0.087f, .z = 0.0671f};
static const Vector3i data_424FC = {.x = 20024, .y = 65353, .z = 11832};
static const char data_42508[12] __attribute__((scn_string_literal)) = "JINGURU01";
static Vector3i data_42514 = {.x = 0, .y = -910, .z = 0};
static const uint8_t data_42520[20] = {0};
static const int32_t data_42534[9] = {-21173, 0, -1, 0, 0, 0, 0, 0, 0};
static const uint32_t data_42558 = 1;
static const int32_t data_4255C[13] = {0, 0, 0, 0, 16384, 0, -1, 0, 0, 0, 0, 0, 0};
static const uint32_t data_42590 = 2;
static const int32_t data_42594[13] = {0, 0, 0, 0, -32768, 0, -1, 0, 0, 0, 0, 0, 0};
static const uint32_t data_425C8 = 3;
static const int32_t data_425CC[13] = {0, 0, 0, 0, -4952, 0, -1, 0, 0, 0, 0, 0, 0};
static const uint32_t data_42600 = 4;
static const int32_t data_42604[13] = {0, 0, 0, 0, 32768, 0, -1, 0, 0, 0, 0, 0, 0};
static const uint32_t data_42638 = 5;
static const int32_t data_4263C[13] = {0, 0, 0, 0, 25675, 0, -1, 0, 0, 0, 0, 0, 0};
static const uint32_t data_42670 = 6;
static const int32_t data_42674[13] = {0, 0, 0, 0, 41320, 0, -1, 0, 0, 0, 0, 0, 0};
static const uint32_t data_426A8 = 7;
static const int32_t data_426AC[13] = {0, 0, 0, 0, -23276, 0, -1, 0, 0, 0, 0, 0, 0};
static char data_426E0[] = "$B01MP01G.MT5";
static const char str_426EE[] __attribute__((scn_string_literal)) = "$B01MP02G.MT5";
static const char str_426FC[] __attribute__((scn_string_literal)) = "$B01MP03G.MT5";
static const char str_4270A[] __attribute__((scn_string_literal)) = "$BSPK3T1G.MT5";
static const char str_42718[] __attribute__((scn_string_literal)) = "$B01MP04G.MT5";
static const char str_42726[] __attribute__((scn_string_literal)) = "$B01MP05G.MT5";
static const char str_42734[] __attribute__((scn_string_literal)) = "$B01MP06G.MT5";
static const char str_42742[] __attribute__((scn_string_literal)) = "$B01MP07G.MT5";
static const uint8_t pad_42750[4] = {0};
static const uint32_t data_42754 = 0xFFFF0000;
static const uint32_t data_42758[3] = {0, 0x8000A7D2U, 0x4200914EU};
static const float data_42764 = 1.1753f;
static const uint32_t data_42768[3] = {0x41B0C49CU, 0, 0};
static const int32_t data_42774[2] = {-65536, 1};
static const uint32_t data_4277C = 0x80004000U;
static const float data_42780[5] = {15.3191f, 1.1753f, 43.8f, 0.0f, 0.0f};
static const int32_t data_42794[2] = {-65536, 2};
static const uint32_t data_4279C = 0x80007E93U;
static const float data_427A0[5] = {-25.4825f, 1.1753f, 44.4692f, 0.0f, 0.0f};
static const int32_t data_427B4[2] = {-65536, 3};
static const uint32_t data_427BC[2] = {0x80008000U, 0xC27A2305U};
static const float data_427C4 = 1.1753f;
static const uint32_t data_427C8[3] = {0x42B4B70AU, 0, 0};
static const int32_t data_427D4[2] = {-65536, 4};
static const uint32_t data_427DC = 0x8000644BU;
static const float data_427E0[2] = {6.5485f, 1.1753f};
static const uint32_t data_427E8[3] = {0x42B7D879U, 0, 0};
static const int32_t data_427F4[2] = {-65536, 5};
static const uint32_t data_427FC[2] = {0x8000A168U, 0xC2DCF74CU};
static const float data_42804[4] = {1.1753f, 76.7537f, 0.0f, 0.0f};
static const int32_t data_42814[2] = {-65536, 6};
static const uint32_t data_4281C[2] = {0x8000A513U, 0xC2D5A6E9U};
static const float data_42824 = 1.1753f;
static const uint32_t data_42828[3] = {0x42CCDAEEU, 0, 0};
static const int32_t data_42834[2] = {-65536, 7};
static const uint32_t data_4283C[2] = {0x8000E71CU, 0x422A2E98U};
static const float data_42844[4] = {1.8f, 5.1289f, 0.0f, 0.0f};
static const int32_t data_42854[2] = {-65536, 8};
static const uint32_t data_4285C[2] = {0x8000E71CU, 0x422A2E98U};
static const float data_42864[4] = {1.2768f, 5.1289f, 0.0f, 0.0f};
static const int32_t data_42874[2] = {-65536, 133};
static const uint32_t data_4287C[2] = {0x8000B1C7U, 0x421A21CBU};
static const float data_42884[5] = {1.1f, 9.521f, 0.0f, 0.0f, 0.0f};
static const shen_name data_42898 = SHEN_NAME('TBK1');
static const uint32_t data_4289C[2] = {0x8000B1C7U, 0x38CB6C7AU};
static const float data_428A4[4] = {0.130716f, 0.002221f, 0.0f, 0.0f};
static const int32_t data_428B4[2] = {-65536, 100};
static const uint32_t data_428BC = 0x8000AB86U;
static const float data_428C0[5] = {37.43f, 1.58f, 10.49f, 0.0f, 0.0f};
static const int32_t data_428D4[2] = {-65536, 101};
static const uint32_t data_428DC = 0x80002DDDU;
static const float data_428E0[5] = {29.09f, 1.43f, 10.9f, 0.0f, 0.0f};
static const int32_t data_428F4[2] = {-65536, 102};
static const uint32_t data_428FC = 0x80002CFAU;
static const float data_42900[5] = {21.93f, 1.5f, 25.05f, 0.0f, 0.0f};
static const int32_t data_42914[2] = {-65536, 103};
static const uint32_t data_4291C = 0x80008003U;
static const float data_42920[5] = {20.56f, 1.62f, 32.69f, 0.0f, 0.0f};
static const int32_t data_42934[2] = {-65536, 104};
static const uint32_t data_4293C = 0x8000FE87U;
static const float data_42940[2] = {2.9916f, 1.0886f};
static const uint32_t data_42948[3] = {0x41D40866U, 0, 0};
static const int32_t data_42954[2] = {-65536, 105};
static const uint32_t data_4295C = 0x80007F34U;
static const float data_42960[5] = {3.53f, 1.2f, 32.65f, 0.0f, 0.0f};
static const int32_t data_42974[2] = {-65536, 106};
static const uint32_t data_4297C = 0x8000FD8BU;
static const float data_42980[5] = {-8.4f, 0.8f, 25.8f, 0.0f, 0.0f};
static const int32_t data_42994[2] = {-65536, 107};
static const uint32_t data_4299C = 0x80001A7EU;
static const float data_429A0[5] = {-15.36f, 1.08f, 28.93f, 0.0f, 0.0f};
static const int32_t data_429B4[2] = {-65536, 108};
static const uint32_t data_429BC = 0x8000196DU;
static const float data_429C0[5] = {-19.24f, 1.24f, 32.36f, 0.0f, 0.0f};
static const int32_t data_429D4[2] = {-65536, 109};
static const uint32_t data_429DC = 0x800016CAU;
static const float data_429E0[5] = {-53.15f, 1.33f, 41.13f, 0.0f, 0.0f};
static const int32_t data_429F4[2] = {-65536, 110};
static const uint32_t data_429FC = 0x80009AC2U;
static const float data_42A00[5] = {-51.9f, 1.41f, 50.03f, 0.0f, 0.0f};
static const int32_t data_42A14[2] = {-65536, 111};
static const uint32_t data_42A1C = 0x80001930U;
static const float data_42A20[5] = {-58.06f, 0.96f, 45.03f, 0.0f, 0.0f};
static const int32_t data_42A34[2] = {-65536, 112};
static const uint32_t data_42A3C = 0x8000C0A7U;
static const float data_42A40[5] = {-63.29f, 1.56f, 66.86f, 0.0f, 0.0f};
static const int32_t data_42A54[2] = {-65536, 113};
static const uint32_t data_42A5C[2] = {0x8000C2C5U, 0xC27CB4F1U};
static const float data_42A64 = 1.6068f;
static const uint32_t data_42A68[3] = {0x428F2EA5U, 0, 0};
static const int32_t data_42A74[2] = {-65536, 114};
static const uint32_t data_42A7C = 0x80008000U;
static const float data_42A80[5] = {-69.82f, 1.75f, 91.66f, 0.0f, 0.0f};
static const int32_t data_42A94[2] = {-65536, 115};
static const uint32_t data_42A9C = 0x8000148EU;
static const float data_42AA0[5] = {-54.42f, 1.56f, 81.22f, 0.0f, 0.0f};
static const int32_t data_42AB4[2] = {-65536, 116};
static const uint32_t data_42ABC = 0x80000C73U;
static const float data_42AC0[5] = {-41.95f, 1.19f, 74.58f, 0.0f, 0.0f};
static const int32_t data_42AD4[2] = {-65536, 117};
static const uint32_t data_42ADC = 0x8000FE68U;
static const float data_42AE0[5] = {-29.54f, 1.04f, 72.51f, 0.0f, 0.0f};
static const int32_t data_42AF4[2] = {-65536, 118};
static const uint32_t data_42AFC = 0x8000E21AU;
static const float data_42B00[5] = {6.33f, 1.59f, 84.17f, 0.0f, 0.0f};
static const int32_t data_42B14[2] = {-65536, 119};
static const uint32_t data_42B1C = 0x80000C96U;
static const float data_42B20[5] = {-93.08f, 1.53f, 61.81f, 0.0f, 0.0f};
static const int32_t data_42B34[2] = {-65536, 120};
static const uint32_t data_42B3C[2] = {0x800091EAU, 0xC2BC7909U};
static const float data_42B44 = 1.693f;
static const uint32_t data_42B48[3] = {0x42854DACU, 0, 0};
static const int32_t data_42B54[2] = {-65536, 121};
static const uint32_t data_42B5C = 0x800011C7U;
static const float data_42B60[5] = {-99.52f, 1.7f, 65.19f, 0.0f, 0.0f};
static const int32_t data_42B74[2] = {-65536, 122};
static const uint32_t data_42B7C = 0x800013A4U;
static const float data_42B80[5] = {-111.81f, 1.61f, 69.88f, 0.0f, 0.0f};
static const int32_t data_42B94[2] = {-65536, 123};
static const uint32_t data_42B9C = 0x80006046U;
static const float data_42BA0[5] = {-126.7f, -0.13f, 64.72f, 0.0f, 0.0f};
static const int32_t data_42BB4[2] = {-65536, 124};
static const uint32_t data_42BBC = 0x800047B5U;
static const float data_42BC0[5] = {-124.49f, 1.58f, 87.91f, 0.0f, 0.0f};
static const int32_t data_42BD4[2] = {-65536, 125};
static const uint32_t data_42BDC = 0x80004A32U;
static const float data_42BE0[5] = {-123.44f, 1.23f, 94.43f, 0.0f, 0.0f};
static const int32_t data_42BF4[2] = {-65536, 126};
static const uint32_t data_42BFC = 0x80006F49U;
static const float data_42C00[5] = {-114.06f, 1.31f, 101.77f, 0.0f, 0.0f};
static const int32_t data_42C14[2] = {-65536, 127};
static const uint32_t data_42C1C = 0x80006D47U;
static const float data_42C20[5] = {-109.53f, 1.32f, 102.17f, 0.0f, 0.0f};
static const int32_t data_42C34[2] = {-65536, 128};
static const uint32_t data_42C3C = 0x800026A0U;
static const float data_42C40[5] = {-106.34f, 1.64f, 93.77f, 0.0f, 0.0f};
static const int32_t data_42C54[2] = {-65536, 129};
static const uint32_t data_42C5C = 0x80001418U;
static const float data_42C60[5] = {-94.02f, 1.69f, 83.69f, 0.0f, 0.0f};
static const int32_t data_42C74[2] = {-65536, 130};
static const uint32_t data_42C7C = 0x80003BB6U;
static const float data_42C80[6] = {-69.96f, 1.88f, 79.5f, 0.0f, 0.0f, 0.0f};
static const shen_name data_42C98 = SHEN_NAME('GCH0');
static const float data_42C9C[7] = {40064.0f, 0.0f, 0.76f, 0.055f, 0.0f, 0.0f, 0.0f};
static const shen_name data_42CB8 = SHEN_NAME('GCH1');
static const float data_42CBC[7] = {40064.0f, 0.0f, 0.76f, 0.055f, 0.0f, 0.0f, 0.0f};
static const shen_name data_42CD8 = SHEN_NAME('GCH2');
static const float data_42CDC[7] = {40064.0f, 0.0f, 0.76f, 0.055f, 0.0f, 0.0f, 0.0f};
static const shen_name data_42CF8 = SHEN_NAME('GCH3');
static const float data_42CFC[7] = {40064.0f, 0.0f, 0.76f, 0.055f, 0.0f, 0.0f, 0.0f};
static const shen_name data_42D18 = SHEN_NAME('GCH4');
static const float data_42D1C[7] = {40064.0f, 0.0f, 0.76f, 0.055f, 0.0f, 0.0f, 0.0f};
static const shen_name data_42D38 = SHEN_NAME('GCH5');
static const float data_42D3C[6] = {40064.0f, 0.0f, 0.76f, 0.055f, 0.0f, 0.0f};
static const int32_t data_42D54[2] = {-65536, 131};
static const uint32_t data_42D5C = 0x8000C367U;
static const float data_42D60[5] = {20.06f, 1.61f, 47.94f, 0.0f, 0.0f};
static const int32_t data_42D74[2] = {-65536, 132};
static const uint32_t data_42D7C[2] = {0x800024C5U, 0xC2F61C1CU};
static const float data_42D84 = 1.2514f;
static const uint32_t data_42D88[3] = {0x42A6B8BBU, 0, 0};
static const int32_t data_42D94[2] = {-65535, 134};
static const uint32_t data_42D9C = 0x71C799F4U;
static const float data_42DA0[2] = {-18.28f, 1.17f};
static const uint32_t data_42DA8 = 0x421A0F5CU;
static const int32_t data_42DAC = -1;
static uint32_t data_42DB0 = 0x4200914E;
static const float data_42DB4 = 1.1753f;
static const uint32_t data_42DB8 = 0x41B0C49CU;
static uint32_t data_42DBC = 0x41751B09;
static const float data_42DC0[2] = {1.1753f, 43.8f};
static uint32_t data_42DC8 = 0xC1CBDC29;
static const float data_42DCC[2] = {1.1753f, 44.472f};
static uint32_t data_42DD4 = 0xC27A2305;
static const float data_42DD8 = 1.1753f;
static const uint32_t data_42DDC = 0x42B4B70AU;
static uint32_t data_42DE0 = 0x40D1851F;
static const float data_42DE4 = 1.1753f;
static const uint32_t data_42DE8 = 0x42B7D7F6U;
static uint32_t data_42DEC = 0xC2DCF7CF;
static const float data_42DF0 = 1.1753f;
static const uint32_t data_42DF4 = 0x429981D8U;
static uint32_t data_42DF8 = 0xC2D5A76D;
static const float data_42DFC = 1.1753f;
static const uint32_t data_42E00 = 0x42CCDAE1U;
static uint32_t data_42E04 = 0x422A2E98;
static const float data_42E08[2] = {1.2768f, 5.1289f};
static Vector3i data_42E10 = {.x = 0, .y = 14108, .z = 0};
static Vector3i data_42E1C = {.x = 0, .y = 30474, .z = 0};
static const float data_42E28[3] = {1.62f, 1.0f, 3.4f};
static Vector3i data_42E34 = {.x = 0, .y = 14108, .z = 0};
static Vector3i data_42E40 = {.x = 0, .y = -30474, .z = 0};
static Vector3i data_42E4C = {.x = 0, .y = -14108, .z = 0};
static Vector3i data_42E58 = {.x = 0, .y = 30474, .z = 0};
static const Vector3 data_42E64 = {.x = 3.1f, .y = 2.65f, .z = 8.0f};
static const Vector4 data_42E70 = {.x = 1.0f, .y = 0.4f, .z = 0.4f, .w = 0.4f};
static Vector3i data_42E80 = {.x = 2730, .y = 0, .z = 0};
static Vector3i data_42E8C = {.x = -2730, .y = 0, .z = 0};
static const char data_42E98[11] __attribute__((scn_string_literal)) = "SA1076A001";
static const char data_42EA3[11] __attribute__((scn_string_literal)) = "SA1076A013";
static const char data_42EAE[11] __attribute__((scn_string_literal)) = "SA1076A002";
static const char data_42EB9[11] __attribute__((scn_string_literal)) = "SA1076A014";
static const char data_42EC4[11] __attribute__((scn_string_literal)) = "SA1076A003";
static const char data_42ECF[11] __attribute__((scn_string_literal)) = "SA1076A015";
static const char data_42EDA[11] __attribute__((scn_string_literal)) = "SA1076A015";
static const char data_42EE5[11] __attribute__((scn_string_literal)) = "SA1076A016";
static const char data_42EF0[12] __attribute__((scn_string_literal)) = "SA1076A017";
static const Vector3 data_42EFC = {.x = 42.199902f, .y = 0.0f, .z = 5.9f};
static const Vector3 data_42F08 = {.x = 42.599899f, .y = 0.0f, .z = 6.8f};
static const Vector3 data_42F14 = {.x = 43.4921f, .y = 0.0f, .z = 5.1527f};
static const char data_42F20[] __attribute__((scn_string_literal)) = "BUSS";
static const char data_42F25[13] __attribute__((scn_string_literal)) = "a1_busno.snd";
static const char data_42F32[] __attribute__((scn_string_literal)) = "/scene/02/D000/";
static const char data_42F42[] __attribute__((scn_string_literal)) = "BUSS";
static const char data_42F47[13] __attribute__((scn_string_literal)) = "seqdata1.bin";
static const char data_42F54[5] __attribute__((scn_string_literal)) = "BUSS";
static const char data_42F59[13] __attribute__((scn_string_literal)) = "seqdata4.bin";
static const char data_42F66[5] __attribute__((scn_string_literal)) = "BUSS";
static const char data_42F6B[13] __attribute__((scn_string_literal)) = "a1_busno.snd";
static const char data_42F78[] __attribute__((scn_string_literal)) = "/scene/02/D000/";
static const char data_42F88[] __attribute__((scn_string_literal)) = "BUSS";
static const char data_42F8D[13] __attribute__((scn_string_literal)) = "seqdata2.bin";
static const char data_42F9A[5] __attribute__((scn_string_literal)) = "BUSS";
static const char data_42F9F[13] __attribute__((scn_string_literal)) = "seqdata5.bin";
static const char data_42FAC[252] = "BUSS\000\000\000\000\232\231\231>333?333?333?\000\000\000\000\000\000\000\000\\\360\377\377\000\000\000\000\000\000\000\000\336\375\377\377\000\000\000\000l\001\000\000\232\371\377\377\000\000\000\000\000\000\000\000m\341\377\377\000\000\000\000\000\000\000\0004\323\377\377\000\000\000\000\000\000\000\000\022\361\377\377\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\340\377\377\000\000\000\000\000\000\000\000\232\271\377\377\000\000\000\000\000\000\000\0004\363\377\377\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000m\341\377\377\000\000\000\000\000\000\000\000\232\271\377\377\000\000\000\000\000\000\000\000a\353\377\377\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\216\003\000\000\022\321\377\377\000\000\000\000x\367\377\377\224\276\377\377\000\000\000\000.\370\377\377\336\335\377\377\000\000\000\000\000\000\000\000l\001";
static const char data_430A8[6] __attribute__((scn_string_literal)) = "YKB_M";
static const char data_430AE[11] __attribute__((scn_string_literal)) = "SA1080A003";
static const char data_430B9[11] __attribute__((scn_string_literal)) = "SA1080A002";
static const char data_430C4[11] __attribute__((scn_string_literal)) = "SA1080A004";
static const char data_430CF[11] __attribute__((scn_string_literal)) = "SA1080A005";
static const char data_430DA[11] __attribute__((scn_string_literal)) = "SA1080A004";
static const char data_430E5[11] __attribute__((scn_string_literal)) = "SA1080A006";
static const char data_430F0[7] __attribute__((scn_string_literal)) = "SA1080";
static const char data_430F7[7] __attribute__((scn_string_literal)) = "SA1081";
static const char data_430FE[11] __attribute__((scn_string_literal)) = "SA1081A002";
static const char data_43109[11] __attribute__((scn_string_literal)) = "SA1081A003";
static const char data_43114[11] __attribute__((scn_string_literal)) = "SA1081A002";
static const char data_4311F[11] __attribute__((scn_string_literal)) = "SA1081A005";
static const char data_4312A[11] __attribute__((scn_string_literal)) = "SA1081A006";
static const char data_43135[7] __attribute__((scn_string_literal)) = "SA1081";
static const char data_4313C[7] __attribute__((scn_string_literal)) = "SA1088";
static const char data_43143[11] __attribute__((scn_string_literal)) = "SA1088A001";
static const char data_4314E[11] __attribute__((scn_string_literal)) = "SA1088A002";
static const char data_43159[11] __attribute__((scn_string_literal)) = "SA1088A003";
static const char data_43164[11] __attribute__((scn_string_literal)) = "SA1088A002";
static const char data_4316F[11] __attribute__((scn_string_literal)) = "SA1088A004";
static const char data_4317A[11] __attribute__((scn_string_literal)) = "SA1088A003";
static const char data_43185[11] __attribute__((scn_string_literal)) = "SA1088A005";
static const char data_43190[11] __attribute__((scn_string_literal)) = "SA1088A002";
static const char data_4319B[11] __attribute__((scn_string_literal)) = "SA1088A003";
static const char data_431A6[10] __attribute__((scn_string_literal)) = "SA1088";
static const uint32_t data_431B0 = 0x4192A3D7;
static const float data_431B4 = 0.225f;
static const uint32_t data_431B8 = 0x42738CCDU;
static const int32_t data_431BC[3] = {32768, 1048832, 8};
static const uint32_t data_431C8 = 0x41935A1DU;
static const float data_431CC[3] = {1.8f, 62.9902f, 0.0f};
static const int32_t data_431D8[2] = {264, 8};
static const float data_431E0[2] = {19.5203f, 2.025f};
static const uint32_t data_431E8 = 0x42822113U;
static const int32_t data_431EC[3] = {39321, 1048832, 15};
static const float data_431F8[2] = {22.1514f, 5.4f};
static const uint32_t data_43200 = 0x4288AB6BU;
static const int32_t data_43204[3] = {6553, 264, 15};
static const float data_43210[2] = {-120.7995f, -1.573f};
static const uint32_t data_43218 = 0x429CA0ECU;
static const int32_t data_4321C[3] = {42143, 1048832, 8};
static const uint32_t data_43228[3] = {0xC2EE475FU, 0, 0x429F3A93U};
static const int32_t data_43234[3] = {9375, 264, 8};
static const float data_43240[2] = {-101.9963f, 0.2967f};
static const uint32_t data_43248 = 0x42C74241U;
static const int32_t data_4324C[3] = {25486, 1048576, 12};
static const uint32_t data_43258 = 0xC2CFF724U;
static const float data_4325C = 2.75f;
static const uint32_t data_43260 = 0x42CC65A2U;
static const int32_t data_43264[3] = {58254, 8, 12};
static const uint32_t data_43270 = 0xC24CC20CU;
static const float data_43274[2] = {0.296f, 97.4198f};
static const int32_t data_4327C[3] = {37501, 1048576, 12};
static const uint32_t data_43288 = 0xC246BAADU;
static const float data_4328C = 2.7716f;
static const uint32_t data_43290 = 0x42C898EFU;
static const int32_t data_43294[3] = {4642, 9, 12};
static const float data_432A0[2] = {-32.9104f, 0.296f};
static const uint32_t data_432A8 = 0x42B06A4BU;
static const int32_t data_432AC[3] = {33314, 1048576, 12};
static const uint32_t data_432B8 = 0xC202E388U;
static const float data_432BC = 2.7716f;
static const uint32_t data_432C0 = 0x42B701D8U;
static const int32_t data_432C4[3] = {546, 8, 12};
static const float data_432D0[2] = {-7.6464f, 0.296f};
static const uint32_t data_432D8 = 0x4290DBF5U;
static const int32_t data_432DC[3] = {11468, 1048576, 12};
static const float data_432E8[3] = {-10.6061f, 2.7716f, 70.9646f};
static const int32_t data_432F4[3] = {44252, 8, 12};
static const float data_43300[2] = {-6.245f, 0.296f};
static const uint32_t data_43308 = 0x42BB1AA0U;
static const int32_t data_4330C[3] = {26594, 1048576, 12};
static const float data_43318[2] = {-8.1412f, 2.7716f};
static const uint32_t data_43320 = 0x42C083F1U;
static const int32_t data_43324[3] = {59346, 8, 12};
static const uint32_t data_43330 = 0x421AEF00U;
static const float data_43334[2] = {5.4742f, 90.1437f};
static const int32_t data_4333C[3] = {24576, 1048576, 12};
static const float data_43348[2] = {36.3997f, 7.9525f};
static const uint32_t data_43350 = 0x42B8F893U;
static const int32_t data_43354[3] = {57344, 8, 12};
static const float data_43360 = 100000.0f;
static const uint8_t data_43364[8] = {0};
typedef struct Row_4336C { float w0; int32_t w1; int32_t w2; } Row_4336C;
static const Row_4336C data_4336C[5] = {
  {1.0f, 0, 1},
  {1.0f, 0, 2},
  {1.0f, 0, 3},
  {1.0f, 0, 4},
  {0.5f, 0, 5}
};
static const float data_433A8 = 0.5f;
static const int32_t data_433AC = 3;
static const char data_433B0[34] __attribute__((scn_string_literal)) = "DOBUITA EVENT START START START!\012";
static const char data_433D2[20] __attribute__((scn_string_literal)) = "DOBUITA EVENT END!\012";
static const char data_433E6[26] __attribute__((scn_string_literal)) = "training check error!!!!\012";
static const char data_43400[67] __attribute__((scn_string_literal)) = "Daily practice %d: Check: in range, setting training flag to true\012";
static const char data_43443[109] __attribute__((scn_string_literal)) = "Daily practice %d: Check: in range, training flag already true ?? Failed before due to NPC's so check again\012";
static const char data_434B0[31] __attribute__((scn_string_literal)) = "VEND_MACHINE_CHECKER_ERROR!!!\012";
static const char data_434CF[11] __attribute__((scn_string_literal)) = "SA1089A001";
static const char data_434DA[11] __attribute__((scn_string_literal)) = "SA1089A002";
static const char data_434E5[11] __attribute__((scn_string_literal)) = "SA1089A003";
static const char data_434F0[11] __attribute__((scn_string_literal)) = "SA1095A001";
static const char data_434FB[11] __attribute__((scn_string_literal)) = "SA1095A002";
static const char data_43506[11] __attribute__((scn_string_literal)) = "SA1095A003";
static const char data_43511[11] __attribute__((scn_string_literal)) = "SA1119A001";
static const char data_4351C[11] __attribute__((scn_string_literal)) = "SA1119A002";
static const char data_43527[11] __attribute__((scn_string_literal)) = "SA1119A003";
static const char data_43532[11] __attribute__((scn_string_literal)) = "SA1120A001";
static const char data_4353D[11] __attribute__((scn_string_literal)) = "SA1120A002";
static const char data_43548[11] __attribute__((scn_string_literal)) = "SA1120A003";
static const char data_43553[11] __attribute__((scn_string_literal)) = "SA1121A001";
static const char data_4355E[11] __attribute__((scn_string_literal)) = "SA1121A002";
static const char data_43569[11] __attribute__((scn_string_literal)) = "SA1121A003";
static const char data_43574[11] __attribute__((scn_string_literal)) = "SA1122A004";
static const char data_4357F[11] __attribute__((scn_string_literal)) = "SA1122A005";
static const char data_4358A[11] __attribute__((scn_string_literal)) = "SA1122A006";
static const char data_43595[11] __attribute__((scn_string_literal)) = "SA1122A001";
static const char data_435A0[11] __attribute__((scn_string_literal)) = "SA1122A002";
static const char data_435AB[11] __attribute__((scn_string_literal)) = "SA1122A003";
static const char data_435B6[11] __attribute__((scn_string_literal)) = "SA1076A001";
static const char data_435C1[11] __attribute__((scn_string_literal)) = "SA1076A002";
static const char data_435CC[11] __attribute__((scn_string_literal)) = "SA1076A003";
static const char data_435D7[9] __attribute__((scn_string_literal)) = "SA1076";
static const uint32_t data_435E0 = 0xC1291EB8;
static const float data_435E4[386] = {
  27.06f, -10.59f, 21.45f, -7.08f, 21.45f, -7.28f, 27.04f, 0.0f, 4.9f, -8.89f, 1.0f, 25.68f, 1.0f, -64.45f, 70.33f, -54.16f, 70.41f, -57.28f, 73.43f,
  -64.49f, 73.58f, 0.0f, 3.4f, -60.6f, 1.0f, 72.0f, 3.0f, -69.57f, 91.23f, -65.16f, 91.25f, -65.16f, 96.3f, -69.55f, 96.3f, 0.0f, 3.3f, -67.2f, 1.0f,
  92.23f, 1.0f, 37.9f, 7.42f, 41.27f, 10.4f, 40.55f, 12.12f, 36.49f, 10.05f, -0.1f, 3.4f, 38.0f, 1.0f, 9.76f, 2.0f, 25.3f, 32.48f, 25.27f, 35.15f,
  19.42f, 35.3f, 19.3f, 32.49f, 0.0f, 3.0f, 22.39f, 1.0f, 33.82f, 2.0f, 43.07f, 4.48f, 44.36f, 1.95f, 51.58f, 5.53f, 50.26f, 8.16f, -0.1f, 3.5f,
  48.07f, 1.0f, 3.93f, 5.0f, 25.46f, 19.66f, 22.56f, 18.22f, 24.29f, 15.49f, 26.84f, 16.81f, -0.1f, 3.67f, 25.19f, 1.05f, 17.72f, 1.05f, 23.84f,
  22.59f, 21.78f, 26.59f, 19.57f, 25.42f, 21.6f, 21.42f, -0.1f, 3.5f, 21.67f, 1.24f, 24.03f, 1.24f, 7.69f, 27.01f, 2.14f, 27.1f, 2.15f, 23.28f, 7.57f,
  23.46f, -0.1f, 3.39f, 5.52f, 1.67f, 25.26f, 1.67f, -15.68f, 30.85f, -19.23f, 26.28f, -16.87f, 24.33f, -13.06f, 28.73f, -0.1f, 4.25f, -16.19f, 1.55f,
  27.56f, 2.0f, -24.57f, 30.94f, -20.08f, 27.22f, -16.58f, 31.68f, -20.96f, 35.28f, -0.1f, 3.23f, -20.63f, 2.67f, 31.18f, 2.67f, -43.15f, 45.49f,
  -24.35f, 45.53f, -24.33f, 59.73f, -43.1f, 59.74f, -0.1f, 3.37f, -39.6f, 1.95f, 47.54f, 1.95f, -49.87f, 40.63f, -52.83f, 43.11f, -55.9f, 38.4f,
  -52.2f, 36.68f, -0.1f, 3.37f, -51.83f, 1.17f, 40.86f, 1.17f, -66.47f, 50.01f, -68.57f, 56.32f, -71.71f, 55.3f, -69.63f, 48.97f, -0.1f, 3.83f,
  -69.09f, 1.66f, 52.65f, 1.66f, -94.89f, 63.91f, -98.94f, 65.8f, -100.91f, 61.57f, -96.84f, 59.71f, -0.1f, 3.07f, -97.89f, 2.24f, 62.75f, 2.24f,
  -98.85f, 68.5f, -97.6f, 71.17f, -101.9f, 73.11f, -103.11f, 70.43f, -0.1f, 3.42f, -100.46f, 1.03f, 70.45f, 1.03f, -114.79f, 80.12f, -111.59f, 77.25f,
  -110.39f, 78.21f, -113.02f, 81.52f, -0.1f, 3.67f, -112.44f, 1.0f, 79.27f, 1.0f, -122.48f, 93.48f, -121.87f, 96.42f, -126.4f, 97.36f, -126.99f,
  94.41f, -0.1f, 3.42f, -123.62f, 1.49f, 95.25f, 1.49f, -116.15f, 99.69f, -113.13f, 101.0f, -114.38f, 103.85f, -117.38f, 102.52f, -0.1f, 3.32f,
  -115.26f, 1.55f, 101.77f, 1.55f, -111.92f, 101.41f, -109.14f, 102.62f, -110.34f, 105.43f, -113.18f, 104.22f, -0.1f, 3.37f, -111.15f, 1.52f, 103.42f,
  1.52f, -95.67f, 88.61f, -92.56f, 87.28f, -91.53f, 89.52f, -94.22f, 91.27f, -0.1f, 3.67f, -93.67f, 1.0f, 88.7f, 1.0f, -85.45f, 84.08f, -84.02f,
  87.26f, -88.37f, 88.96f, -89.69f, 86.03f, -0.1f, 3.42f, -87.11f, 1.02f, 86.06f, 1.02f, -77.62f, 80.11f, -80.62f, 80.11f, -80.61f, 78.6f, -77.61f,
  78.6f, -0.1f, 3.67f, -79.14f, 1.0f, 79.36f, 1.0f, -55.62f, 82.92f, -58.23f, 78.35f, -54.48f, 76.52f, -52.57f, 81.27f, -0.1f, 3.77f, -54.84f, 1.54f,
  80.71f, 1.54f, -38.6f, 75.51f, -43.41f, 76.9f, -44.52f, 72.78f, -39.75f, 71.45f, -0.1f, 3.47f, -41.57f, 2.13f, 74.16f, 2.13f, -29.23f, 73.42f,
  -33.14f, 73.42f, -33.14f, 71.86f, -29.23f, 71.86f, -0.1f, 3.77f, -31.18f, 1.0f, 72.64f, 1.0f, 3.05f, 83.0f, 5.91f, 79.44f, 9.93f, 82.67f, 7.1f,
  86.22f, -0.1f, 4.33f, 6.53f, 2.25f, 82.84f, 2.25f, 43.66f, 86.22f, 50.37f, 93.65f, 41.84f, 101.37f, 34.12f, 92.72f, 5.14f
};
static const char str_43BEC[] __attribute__((scn_string_literal)) = "ff>A=\n\"B";
static const uint8_t data_43BF5[16] = {
  0x00, 0xB8, 0x40, 0x14, 0x2E, 0xB7, 0x42, 0x00, 0x00, 0x00, 0x41, 0x7B, 0x14, 0xDE, 0x41, 0xA4
};
static const uint8_t data_43C05[16] = {
  0x70, 0xEF, 0x41, 0x5C, 0x8F, 0xD6, 0x41, 0x1F, 0x85, 0xEB, 0x41, 0xE1, 0x7A, 0xE0, 0x41, 0x71
};
static const uint8_t data_43C15[16] = {
  0x3D, 0xD8, 0x41, 0x00, 0x00, 0xE8, 0x41, 0x7B, 0x14, 0xDC, 0x41, 0xCD, 0xCC, 0xCC, 0xBD, 0x5C
};
static const uint8_t data_43C25[16] = {
  0x8F, 0x52, 0x40, 0xAE, 0x47, 0xDF, 0x41, 0x00, 0x00, 0x80, 0x3F, 0x0A, 0xD7, 0xE3, 0x41, 0x00
};
static const uint8_t data_43C35[16] = {
  0x00, 0x80, 0x3F, 0xAE, 0x47, 0x39, 0x41, 0x52, 0xB8, 0xCE, 0x41, 0xAE, 0x47, 0x39, 0x41, 0x33
};
static const uint8_t data_43C45[16] = {
  0x33, 0xD7, 0x41, 0x8F, 0xC2, 0x0D, 0x41, 0x33, 0x33, 0xD7, 0x41, 0x8F, 0xC2, 0x0D, 0x41, 0xD7
};
static const uint8_t data_43C55[16] = {
  0xA3, 0xCE, 0x41, 0xCD, 0xCC, 0xCC, 0xBD, 0x71, 0x3D, 0x5A, 0x40, 0x29, 0x5C, 0x23, 0x41, 0x00
};
static const uint8_t data_43C65[16] = {
  0x00, 0x80, 0x3F, 0x48, 0xE1, 0xD2, 0x41, 0x00, 0x00, 0x80, 0x3F, 0x66, 0x66, 0x12, 0x41, 0x5C
};
static const uint8_t data_43C75[16] = {
  0x8F, 0xFE, 0x41, 0x66, 0x66, 0x12, 0x41, 0xE1, 0x7A, 0x03, 0x42, 0xCD, 0xCC, 0xBC, 0x40, 0xE1
};
static const uint8_t data_43C85[16] = {
  0x7A, 0x03, 0x42, 0xCD, 0xCC, 0xBC, 0x40, 0x5C, 0x8F, 0xFE, 0x41, 0xCD, 0xCC, 0xCC, 0xBD, 0x71
};
static const uint8_t data_43C95[16] = {
  0x3D, 0x92, 0x40, 0xC3, 0xF5, 0xF0, 0x40, 0x00, 0x00, 0x80, 0x3F, 0x29, 0x5C, 0x01, 0x42, 0x00
};
static const uint8_t data_43CA5[16] = {
  0x00, 0x80, 0x3F, 0x8F, 0xC2, 0x75, 0x3F, 0xCD, 0xCC, 0xD0, 0x41, 0x8F, 0xC2, 0x75, 0x3F, 0xAE
};
static const uint8_t data_43CB5[16] = {
  0x47, 0xD5, 0x41, 0x52, 0xB8, 0x7E, 0xC0, 0x29, 0x5C, 0xD5, 0x41, 0x52, 0xB8, 0x7E, 0xC0, 0xCD
};
static const uint8_t data_43CC5[16] = {
  0xCC, 0xD0, 0x41, 0xCD, 0xCC, 0xCC, 0xBD, 0xCD, 0xCC, 0x6C, 0x40, 0x5C, 0x8F, 0xC2, 0xBF, 0x00
};
static const uint8_t data_43CD5[16] = {
  0x00, 0x80, 0x3F, 0x3D, 0x0A, 0xD3, 0x41, 0x00, 0x00, 0x80, 0x3F, 0x0A, 0xD7, 0xC3, 0xC0, 0x7B
};
static const uint8_t data_43CE5[16] = {
  0x14, 0xFE, 0x41, 0x14, 0xAE, 0x47, 0xC0, 0x7B, 0x14, 0xFE, 0x41, 0x14, 0xAE, 0x47, 0xC0, 0xB8
};
static const uint8_t data_43CF5[16] = {
  0x1E, 0x05, 0x42, 0x0A, 0xD7, 0xC3, 0xC0, 0xB8, 0x1E, 0x05, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0x48
};
static const uint8_t data_43D05[16] = {
  0xE1, 0x6A, 0x40, 0x0A, 0xD7, 0x93, 0xC0, 0x00, 0x00, 0x80, 0x3F, 0x3D, 0x0A, 0x02, 0x42, 0x00
};
static const uint8_t data_43D15[16] = {
  0x00, 0x80, 0x3F, 0x71, 0x3D, 0xBE, 0xC1, 0x29, 0x5C, 0x0F, 0x42, 0x48, 0xE1, 0xB8, 0xC1, 0xD7
};
static const uint8_t data_43D25[16] = {
  0xA3, 0x12, 0x42, 0x8F, 0xC2, 0xC9, 0xC1, 0xE1, 0x7A, 0x19, 0x42, 0xB8, 0x1E, 0xCF, 0xC1, 0x71
};
static const uint8_t data_43D35[16] = {
  0x3D, 0x16, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0x85, 0xEB, 0x51, 0x40, 0x00, 0x00, 0xC4, 0xC1, 0x00
};
static const uint8_t data_43D45[16] = {
  0x00, 0x80, 0x3F, 0xA4, 0x70, 0x14, 0x42, 0x00, 0x00, 0x80, 0x3F, 0x14, 0xAE, 0x1D, 0xC2, 0xB8
};
static const uint8_t data_43D55[16] = {
  0x1E, 0x16, 0x42, 0xD7, 0xA3, 0x1D, 0xC2, 0xB8, 0x1E, 0x1C, 0x42, 0xAE, 0x47, 0x2D, 0xC2, 0xB8
};
static const uint8_t data_43D65[16] = {
  0x1E, 0x1C, 0x42, 0x71, 0x3D, 0x2D, 0xC2, 0xB8, 0x1E, 0x16, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0xAE
};
static const uint8_t data_43D75[16] = {
  0x47, 0x71, 0x40, 0xA4, 0x70, 0x25, 0xC2, 0x00, 0x00, 0x80, 0x3F, 0xB8, 0x1E, 0x19, 0x42, 0x00
};
static const uint8_t data_43D85[16] = {
  0x00, 0x80, 0x3F, 0xCD, 0xCC, 0x33, 0xC2, 0x33, 0x33, 0x17, 0x42, 0xCD, 0xCC, 0x33, 0xC2, 0x5C
};
static const uint8_t data_43D95[16] = {
  0x8F, 0x1C, 0x42, 0x0A, 0xD7, 0x3F, 0xC2, 0x1F, 0x85, 0x1C, 0x42, 0x66, 0x66, 0x3E, 0xC2, 0xB8
};
static const uint8_t data_43DA5[16] = {
  0x1E, 0x17, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0x14, 0xAE, 0x57, 0x40, 0x0A, 0xD7, 0x39, 0xC2, 0x00
};
static const uint8_t data_43DB5[16] = {
  0x00, 0x80, 0x3F, 0x0A, 0xD7, 0x19, 0x42, 0x00, 0x00, 0x80, 0x3F, 0x1F, 0x85, 0x3B, 0xC2, 0x29
};
static const uint8_t data_43DC5[16] = {
  0x5C, 0x33, 0x42, 0x00, 0x00, 0x38, 0xC2, 0x66, 0x66, 0x38, 0x42, 0x48, 0xE1, 0x42, 0xC2, 0x7B
};
static const uint8_t data_43DD5[16] = {
  0x14, 0x41, 0x42, 0xE1, 0x7A, 0x46, 0xC2, 0x33, 0x33, 0x3B, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0x71
};
static const uint8_t data_43DE5[16] = {
  0x3D, 0x92, 0x40, 0x8F, 0xC2, 0x3F, 0xC2, 0x00, 0x00, 0x80, 0x3F, 0x29, 0x5C, 0x39, 0x42, 0x00
};
static const uint8_t data_43DF5[16] = {
  0x00, 0x80, 0x3F, 0x85, 0xEB, 0x68, 0xC2, 0xEC, 0x51, 0x38, 0x42, 0x85, 0xEB, 0x6F, 0xC2, 0x52
};
static const uint8_t data_43E05[16] = {
  0xB8, 0x2E, 0x42, 0xC3, 0xF5, 0x62, 0xC2, 0x1F, 0x85, 0x25, 0x42, 0x33, 0x33, 0x5C, 0xC2, 0x29
};
static const uint8_t data_43E15[16] = {
  0x5C, 0x2F, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0x14, 0xAE, 0x77, 0x40, 0x00, 0x00, 0x66, 0xC2, 0x52
};
static const uint8_t data_43E25[16] = {
  0xB8, 0xBE, 0x3F, 0xC3, 0xF5, 0x2E, 0x42, 0x52, 0xB8, 0xBE, 0x3F, 0xD7, 0xA3, 0x77, 0xC2, 0x33
};
static const uint8_t data_43E35[16] = {
  0x33, 0x3C, 0x42, 0x33, 0x33, 0x75, 0xC2, 0xD7, 0xA3, 0x3F, 0x42, 0xF6, 0x28, 0x7E, 0xC2, 0xC3
};
static const uint8_t data_43E45[16] = {
  0xF5, 0x45, 0x42, 0xAE, 0x47, 0x80, 0xC2, 0x1F, 0x85, 0x42, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0x33
};
static const uint8_t data_43E55[16] = {
  0x33, 0x53, 0x40, 0x48, 0xE1, 0x7A, 0xC2, 0x00, 0x00, 0x80, 0x3F, 0x7B, 0x14, 0x41, 0x42, 0x00
};
static const uint8_t data_43E65[16] = {
  0x00, 0x80, 0x3F, 0x00, 0x00, 0x5E, 0xC2, 0x52, 0xB8, 0x4D, 0x42, 0x29, 0x5C, 0x5A, 0xC2, 0x48
};
static const uint8_t data_43E75[16] = {
  0xE1, 0x52, 0x42, 0xCD, 0xCC, 0x6A, 0xC2, 0x33, 0x33, 0x5F, 0x42, 0xD7, 0xA3, 0x6E, 0xC2, 0x52
};
static const uint8_t data_43E85[16] = {
  0xB8, 0x59, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0xCD, 0xCC, 0x6C, 0x40, 0x5C, 0x8F, 0x65, 0xC2, 0x00
};
static const uint8_t data_43E95[16] = {
  0x00, 0x80, 0x3F, 0x0A, 0xD7, 0x54, 0x42, 0x00, 0x00, 0x80, 0x3F, 0x8F, 0xC2, 0x7B, 0xC2, 0xB8
};
static const uint8_t data_43EA5[16] = {
  0x1E, 0x66, 0x42, 0x8F, 0xC2, 0x77, 0xC2, 0x5C, 0x8F, 0x67, 0x42, 0xB8, 0x1E, 0x7C, 0xC2, 0xC3
};
static const uint8_t data_43EB5[16] = {
  0xF5, 0x73, 0x42, 0x5C, 0x0F, 0x80, 0xC2, 0xA4, 0x70, 0x72, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0x71
};
static const uint8_t data_43EC5[16] = {
  0x3D, 0x92, 0x40, 0x85, 0xEB, 0x7B, 0xC2, 0x00, 0x00, 0x80, 0x3F, 0x00, 0x00, 0x6D, 0x42, 0x00
};
static const uint8_t data_43ED5[16] = {
  0x00, 0x80, 0x3F, 0xA4, 0xF0, 0x93, 0xC2, 0xF6, 0x28, 0x6D, 0x42, 0xA4, 0xF0, 0x93, 0xC2, 0x9A
};
static const uint8_t data_43EE5[16] = {
  0x99, 0x72, 0x42, 0xA4, 0xF0, 0x99, 0xC2, 0x9A, 0x99, 0x72, 0x42, 0xA4, 0xF0, 0x99, 0xC2, 0xF6
};
static const uint8_t data_43EF5[16] = {
  0x28, 0x6D, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0x14, 0xAE, 0x57, 0x40, 0xA4, 0xF0, 0x96, 0xC2, 0x00
};
static const uint8_t data_43F05[16] = {
  0x00, 0x80, 0x3F, 0x48, 0xE1, 0x6F, 0x42, 0x00, 0x00, 0x80, 0x3F, 0x8F, 0xC2, 0xA0, 0xC2, 0x5C
};
static const uint8_t data_43F15[16] = {
  0x8F, 0x6D, 0x42, 0xAE, 0xC7, 0xA0, 0xC2, 0x52, 0xB8, 0x71, 0x42, 0x33, 0x33, 0xA6, 0xC2, 0x52
};
static const uint8_t data_43F25[16] = {
  0xB8, 0x71, 0x42, 0x33, 0x33, 0xA6, 0xC2, 0x1F, 0x85, 0x6D, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0x85
};
static const uint8_t data_43F35[16] = {
  0xEB, 0x51, 0x40, 0xE1, 0x7A, 0xA3, 0xC2, 0x00, 0x00, 0x80, 0x3F, 0xD7, 0xA3, 0x6F, 0x42, 0x00
};
static const uint8_t data_43F45[16] = {
  0x00, 0x80, 0x3F, 0x33, 0x33, 0xCC, 0xC2, 0x52, 0x38, 0x82, 0x42, 0xEC, 0x51, 0xCB, 0xC2, 0xF6
};
static const uint8_t data_43F55[16] = {
  0x28, 0x84, 0x42, 0x0A, 0xD7, 0xD1, 0xC2, 0x33, 0x33, 0x87, 0x42, 0x71, 0xBD, 0xD2, 0xC2, 0x8F
};
static const uint8_t data_43F65[16] = {
  0x42, 0x85, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0xD7, 0xA3, 0x30, 0x40, 0x1F, 0x05, 0xCF, 0xC2, 0x00
};
static const uint8_t data_43F75[16] = {
  0x00, 0x80, 0x3F, 0x33, 0xB3, 0x84, 0x42, 0x00, 0x00, 0x80, 0x3F, 0x9A, 0x19, 0xC4, 0xC2, 0x29
};
static const uint8_t data_43F85[16] = {
  0xDC, 0x87, 0x42, 0x0A, 0xD7, 0xBA, 0xC2, 0x3D, 0x8A, 0x83, 0x42, 0x85, 0xEB, 0xB7, 0xC2, 0x85
};
static const uint8_t data_43F95[16] = {
  0xEB, 0x8B, 0x42, 0x14, 0x2E, 0xC1, 0xC2, 0x8F, 0x42, 0x8E, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0x71
};
static const uint8_t data_43FA5[16] = {
  0x3D, 0x82, 0x40, 0x1F, 0x05, 0xBE, 0xC2, 0x5C, 0x8F, 0xE2, 0x3F, 0x48, 0xE1, 0x88, 0x42, 0x5C
};
static const uint8_t data_43FB5[16] = {
  0x8F, 0xE2, 0x3F, 0x3D, 0x8A, 0xD4, 0xC2, 0xF6, 0x28, 0x89, 0x42, 0x48, 0xE1, 0xDE, 0xC2, 0x00
};
static const uint8_t data_43FC5[16] = {
  0x00, 0x8E, 0x42, 0xE1, 0x7A, 0xE2, 0xC2, 0x0A, 0x57, 0x86, 0x42, 0x1F, 0x05, 0xD8, 0xC2, 0x5C
};
static const uint8_t data_43FD5[16] = {
  0x8F, 0x81, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0x48, 0xE1, 0x6A, 0x40, 0xE1, 0x7A, 0xDB, 0xC2, 0x66
};
static const uint8_t data_43FE5[16] = {
  0x66, 0x06, 0x40, 0x8F, 0xC2, 0x87, 0x42, 0x66, 0x66, 0x06, 0x40, 0xCD, 0x4C, 0xF2, 0xC2, 0xA4
};
static const uint8_t data_43FF5[16] = {
  0x70, 0xA4, 0x42, 0xB8, 0x1E, 0xF7, 0xC2, 0xD7, 0xA3, 0xAA, 0x42, 0xD7, 0xA3, 0xFD, 0xC2, 0x71
};
static const uint8_t data_44005[16] = {
  0x3D, 0xA5, 0x42, 0x71, 0xBD, 0xF8, 0xC2, 0x0A, 0x57, 0x9F, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0x48
};
static const uint8_t data_44015[16] = {
  0xE1, 0x6A, 0x40, 0xC3, 0xF5, 0xF7, 0xC2, 0xB8, 0x1E, 0x05, 0x40, 0x85, 0xEB, 0xA4, 0x42, 0xB8
};
static const uint8_t data_44025[16] = {
  0x1E, 0x05, 0x40, 0xC3, 0xF5, 0xCF, 0xC2, 0x48, 0xE1, 0xB8, 0x42, 0x14, 0x2E, 0xD4, 0xC2, 0x33
};
static const uint8_t data_44035[16] = {
  0x33, 0xBE, 0x42, 0xD7, 0x23, 0xDA, 0xC2, 0xE1, 0x7A, 0xB9, 0x42, 0x48, 0xE1, 0xD5, 0xC2, 0x9A
};
static const uint8_t data_44045[16] = {
  0x19, 0xB4, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0x9A, 0x99, 0x79, 0x40, 0x3D, 0x0A, 0xD5, 0xC2, 0x85
};
static const uint8_t data_44055[16] = {
  0xEB, 0xF1, 0x3F, 0xF6, 0x28, 0xB9, 0x42, 0x85, 0xEB, 0xF1, 0x3F, 0xD7, 0xA3, 0xC7, 0xC2, 0x3D
};
static const uint8_t data_44065[16] = {
  0x0A, 0xAF, 0x42, 0x9A, 0x99, 0xCC, 0xC2, 0xEC, 0xD1, 0xB5, 0x42, 0x8F, 0x42, 0xD1, 0xC2, 0xCD
};
static const uint8_t data_44075[16] = {
  0x4C, 0xB2, 0x42, 0x52, 0x38, 0xCC, 0xC2, 0xCD, 0xCC, 0xAB, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0x48
};
static const uint8_t data_44085[16] = {
  0xE1, 0x6A, 0x40, 0x1F, 0x05, 0xCC, 0xC2, 0xB8, 0x1E, 0x85, 0x3F, 0x00, 0x00, 0xB1, 0x42, 0xB8
};
static const uint8_t data_44095[16] = {
  0x1E, 0x85, 0x3F, 0x1F, 0x85, 0xB4, 0xC2, 0x5C, 0x8F, 0xA2, 0x42, 0xB8, 0x9E, 0xB3, 0xC2, 0xE1
};
static const uint8_t data_440A5[16] = {
  0x7A, 0xA4, 0x42, 0x5C, 0x8F, 0xB8, 0xC2, 0xAE, 0xC7, 0xA6, 0x42, 0xC3, 0x75, 0xB9, 0xC2, 0x29
};
static const uint8_t data_440B5[16] = {
  0xDC, 0xA4, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0x85, 0xEB, 0x51, 0x40, 0x3D, 0x8A, 0xB6, 0xC2, 0x00
};
static const uint8_t data_440C5[16] = {
  0x00, 0x00, 0x40, 0xF6, 0xA8, 0xA4, 0x42, 0x00, 0x00, 0x00, 0x40, 0xE1, 0xFA, 0xA2, 0xC2, 0x00
};
static const uint8_t data_440D5[16] = {
  0x00, 0xA7, 0x42, 0xF6, 0x28, 0x9C, 0xC2, 0x00, 0x00, 0xA7, 0x42, 0xF6, 0x28, 0x9C, 0xC2, 0x3D
};
static const uint8_t data_440E5[16] = {
  0x8A, 0xA9, 0x42, 0xE1, 0xFA, 0xA2, 0xC2, 0x3D, 0x8A, 0xA9, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0x71
};
static const uint8_t data_440F5[16] = {
  0x3D, 0x92, 0x40, 0x5C, 0x8F, 0x9F, 0xC2, 0x00, 0x00, 0xC0, 0x3F, 0xAE, 0x47, 0xA8, 0x42, 0x00
};
static const uint8_t data_44105[16] = {
  0x00, 0xC0, 0x3F, 0xEC, 0x51, 0x90, 0xC2, 0x71, 0x3D, 0xA7, 0x42, 0xCD, 0x4C, 0x90, 0xC2, 0x0A
};
static const uint8_t data_44115[16] = {
  0x57, 0xA9, 0x42, 0x8F, 0xC2, 0x95, 0xC2, 0x0A, 0x57, 0xA9, 0x42, 0x8F, 0xC2, 0x95, 0xC2, 0x71
};
static const uint8_t data_44125[16] = {
  0x3D, 0xA7, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0x33, 0x33, 0x53, 0x40, 0x3D, 0x0A, 0x93, 0xC2, 0x00
};
static const uint8_t data_44135[16] = {
  0x00, 0x80, 0x3F, 0xCD, 0x4C, 0xA8, 0x42, 0x00, 0x00, 0x80, 0x3F, 0x52, 0xB8, 0x22, 0xC2, 0x52
};
static const uint8_t data_44145[16] = {
  0x38, 0xA1, 0x42, 0xD7, 0xA3, 0x15, 0xC2, 0x1F, 0x05, 0x9F, 0x42, 0xE1, 0x7A, 0x11, 0xC2, 0x29
};
static const uint8_t data_44155[16] = {
  0x5C, 0xA5, 0x42, 0x48, 0xE1, 0x1E, 0xC2, 0x5C, 0x8F, 0xA7, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0x71
};
static const uint8_t data_44165[16] = {
  0x3D, 0x92, 0x40, 0xE1, 0x7A, 0x1B, 0xC2, 0x00, 0x00, 0x80, 0x3F, 0x66, 0x66, 0xA1, 0x42, 0x00
};
static const uint8_t data_44175[16] = {
  0x00, 0x80, 0x3F, 0x52, 0xB8, 0x1E, 0xC0, 0x0A, 0x57, 0xA9, 0x42, 0xB8, 0x1E, 0x55, 0xC0, 0x00
};
static const uint8_t data_44185[16] = {
  0x80, 0xAB, 0x42, 0x8F, 0xC2, 0xB5, 0xC0, 0x71, 0xBD, 0xA7, 0x42, 0x5C, 0x8F, 0x9A, 0xC0, 0xB8
};
static const uint8_t data_44195[16] = {
  0x9E, 0xA5, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0x14, 0xAE, 0x57, 0x40, 0x85, 0xEB, 0x81, 0xC0, 0x00
};
static const uint8_t data_441A5[16] = {
  0x00, 0x80, 0x3F, 0x00, 0x80, 0xA8, 0x42, 0x00, 0x00, 0x80, 0x3F, 0x48, 0xE1, 0xEC, 0x41, 0x7B
};
static const uint8_t data_441B5[16] = {
  0x14, 0x0E, 0x41, 0x3D, 0x0A, 0xF7, 0x41, 0xE1, 0x7A, 0xCC, 0x40, 0x5C, 0x8F, 0xFE, 0x41, 0xF6
};
static const uint8_t data_441C5[16] = {
  0x28, 0xDC, 0x40, 0x66, 0x66, 0xF4, 0x41, 0x85, 0xEB, 0x15, 0x41, 0xCD, 0xCC, 0xCC, 0xBD, 0x85
};
static const uint8_t data_441D5[16] = {
  0xEB, 0x51, 0x40, 0x14, 0xAE, 0xF5, 0x41, 0x00, 0x00, 0x80, 0x3F, 0xF6, 0x28, 0xFC, 0x40, 0x00
};
static const uint8_t data_441E5[16] = {
  0x00, 0x80, 0x3F, 0x71, 0x3D, 0x5E, 0xC1, 0xAE, 0x47, 0x0E, 0x42, 0xC3, 0xF5, 0x38, 0xC1, 0x52
};
static const uint8_t data_441F5[16] = {
  0xB8, 0x06, 0x42, 0x3D, 0x0A, 0x2B, 0xC1, 0xB8, 0x1E, 0x0B, 0x42, 0xEC, 0x51, 0x50, 0xC1, 0x14
};
static const uint8_t data_44205[16] = {
  0xAE, 0x12, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0x14, 0xAE, 0x57, 0x40, 0xD7, 0xA3, 0x44, 0xC1, 0x00
};
static const uint8_t data_44215[16] = {
  0x00, 0x80, 0x3F, 0x52, 0xB8, 0x0C, 0x42, 0x00, 0x00, 0x80, 0x3F, 0x3D, 0x0A, 0xA3, 0xC2, 0xA4
};
static const uint8_t data_44225[16] = {
  0xF0, 0x82, 0x42, 0x3D, 0x0A, 0xA3, 0xC2, 0x5C, 0x8F, 0x7F, 0x42, 0x00, 0x00, 0x9D, 0xC2, 0x5C
};
static const uint8_t data_44235[16] = {
  0x8F, 0x7F, 0x42, 0x00, 0x00, 0x9D, 0xC2, 0xA4, 0xF0, 0x82, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0x48
};
static const uint8_t data_44245[16] = {
  0xE1, 0x6A, 0x40, 0x1F, 0x05, 0xA0, 0xC2, 0x00, 0x00, 0x80, 0x3F, 0x29, 0x5C, 0x81, 0x42, 0x00
};
static const uint8_t data_44255[16] = {
  0x00, 0x80, 0x3F, 0xA4, 0xF0, 0xAF, 0xC2, 0xCD, 0xCC, 0x75, 0x42, 0x1F, 0x85, 0xB0, 0xC2, 0xEC
};
static const uint8_t data_44265[16] = {
  0x51, 0x71, 0x42, 0x33, 0x33, 0xAA, 0xC2, 0x00, 0x00, 0x6E, 0x42, 0xF6, 0xA8, 0xA9, 0xC2, 0xEC
};
static const uint8_t data_44275[16] = {
  0x51, 0x72, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0x00, 0x00, 0x90, 0x40, 0x7B, 0x14, 0xAD, 0xC2, 0x00
};
static const uint8_t data_44285[16] = {
  0x00, 0x80, 0x3F, 0x0A, 0xD7, 0x71, 0x42, 0x00, 0x00, 0x80, 0x3F, 0x48, 0xE1, 0xA9, 0xC2, 0x8F
};
static const uint8_t data_44295[16] = {
  0x42, 0x80, 0x42, 0x48, 0xE1, 0xA8, 0xC2, 0xCD, 0xCC, 0x84, 0x42, 0x9A, 0x99, 0xAF, 0xC2, 0x66
};
static const uint8_t data_442A5[16] = {
  0x66, 0x86, 0x42, 0xF6, 0xA8, 0xB0, 0xC2, 0x48, 0xE1, 0x81, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0x71
};
static const uint8_t data_442B5[16] = {
  0x3D, 0x92, 0x40, 0x1F, 0x05, 0xAD, 0xC2, 0x00, 0x00, 0x80, 0x3F, 0xF6, 0x28, 0x82, 0x42, 0x00
};
static const uint8_t data_442C5[16] = {
  0x00, 0x80, 0x3F, 0x33, 0x33, 0xD7, 0xC2, 0xAE, 0x47, 0x91, 0x42, 0x8F, 0x42, 0xD1, 0xC2, 0x3D
};
static const uint8_t data_442D5[16] = {
  0x8A, 0x8E, 0x42, 0x7B, 0x14, 0xD0, 0xC2, 0x1F, 0x05, 0x91, 0x42, 0xC3, 0xF5, 0xD5, 0xC2, 0xAE
};
static const uint8_t data_442E5[16] = {
  0xC7, 0x93, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0x48, 0xE1, 0x92, 0x40, 0xB8, 0x9E, 0xD3, 0xC2, 0x00
};
static const uint8_t data_442F5[16] = {
  0x00, 0x80, 0x3F, 0xF6, 0x28, 0x91, 0x42, 0x00, 0x00, 0x80, 0x3F, 0x0A, 0x57, 0xFE, 0xC2, 0x1F
};
static const uint8_t data_44305[16] = {
  0x05, 0x80, 0x42, 0xA4, 0xF0, 0xF8, 0xC2, 0x0A, 0x57, 0x84, 0x42, 0x0A, 0xD7, 0xFC, 0xC2, 0x14
};
static const uint8_t data_44315[16] = {
  0x2E, 0x89, 0x42, 0x0A, 0x17, 0x01, 0xC3, 0x85, 0xEB, 0x84, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0x5C
};
static const uint8_t data_44325[16] = {
  0x8F, 0x12, 0x40, 0x7B, 0x94, 0xFD, 0xC2, 0x66, 0x66, 0xC6, 0x3F, 0xB8, 0x9E, 0x84, 0x42, 0x66
};
static const uint8_t data_44335[16] = {
  0x66, 0xC6, 0x3F, 0x66, 0x66, 0xE8, 0xC2, 0xAE, 0xC7, 0xA3, 0x42, 0x9A, 0x99, 0xE5, 0xC2, 0x8F
};
static const uint8_t data_44345[16] = {
  0xC2, 0xA5, 0x42, 0x71, 0x3D, 0xE9, 0xC2, 0x1F, 0x85, 0xAA, 0x42, 0x7B, 0x14, 0xEC, 0xC2, 0x00
};
static const uint8_t data_44355[16] = {
  0x80, 0xA8, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0xD7, 0xA3, 0x50, 0x40, 0x52, 0x38, 0xE9, 0xC2, 0x00
};
static const uint8_t data_44365[16] = {
  0x00, 0x80, 0x3F, 0xF6, 0xA8, 0xA6, 0x42, 0x00, 0x00, 0x80, 0x3F, 0xC3, 0x75, 0xF8, 0xC2, 0xB8
};
static const uint8_t data_44375[16] = {
  0x1E, 0xAE, 0x42, 0x5C, 0x0F, 0xF7, 0xC2, 0x8F, 0xC2, 0xB4, 0x42, 0x33, 0x33, 0xFB, 0xC2, 0xB8
};
static const uint8_t data_44385[16] = {
  0x9E, 0xB5, 0x42, 0x1F, 0x85, 0xFC, 0xC2, 0xC3, 0xF5, 0xAE, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0xAE
};
static const uint8_t data_44395[16] = {
  0x47, 0x51, 0x40, 0xEC, 0xD1, 0xF9, 0xC2, 0xAE, 0x47, 0x81, 0x3F, 0x29, 0xDC, 0xB1, 0x42, 0xAE
};
static const uint8_t data_443A5[16] = {
  0x47, 0x81, 0x3F, 0x29, 0x5C, 0xE2, 0xC2, 0x8F, 0xC2, 0xC2, 0x42, 0x0A, 0xD7, 0xDC, 0xC2, 0xF6
};
static const uint8_t data_443B5[16] = {
  0xA8, 0xC4, 0x42, 0xCD, 0x4C, 0xDD, 0xC2, 0x85, 0xEB, 0xC5, 0x42, 0xCD, 0xCC, 0xE2, 0xC2, 0x1F
};
static const uint8_t data_443C5[16] = {
  0x05, 0xC4, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0x8F, 0xC2, 0x45, 0x40, 0xEC, 0xD1, 0xDF, 0xC2, 0x00
};
static const uint8_t data_443D5[16] = {
  0x00, 0x80, 0x3F, 0x0A, 0x57, 0xC4, 0x42, 0x00, 0x00, 0x80, 0x3F, 0x0A, 0x57, 0xBC, 0xC2, 0x52
};
static const uint8_t data_443E5[16] = {
  0x38, 0xA8, 0x42, 0x5C, 0x0F, 0xC1, 0xC2, 0x85, 0x6B, 0xAA, 0x42, 0x71, 0x3D, 0xC3, 0xC2, 0x29
};
static const uint8_t data_443F5[16] = {
  0xDC, 0xA5, 0x42, 0xA4, 0x70, 0xBE, 0xC2, 0xD7, 0xA3, 0xA3, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0xA4
};
static const uint8_t data_44405[16] = {
  0x70, 0x4D, 0x40, 0x8F, 0xC2, 0xBF, 0xC2, 0xAE, 0x47, 0xA1, 0x3F, 0x1F, 0x05, 0xA7, 0x42, 0xAE
};
static const uint8_t data_44415[16] = {
  0x47, 0xA1, 0x3F, 0x0A, 0xD7, 0xAD, 0xC2, 0xD7, 0xA3, 0x9F, 0x42, 0x1F, 0x05, 0xA8, 0xC2, 0x71
};
static const uint8_t data_44425[16] = {
  0x3D, 0x9E, 0x42, 0x29, 0x5C, 0xA7, 0xC2, 0x29, 0xDC, 0xA0, 0x42, 0x71, 0x3D, 0xAD, 0xC2, 0x8F
};
static const uint8_t data_44435[16] = {
  0x42, 0xA2, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0x14, 0xAE, 0x57, 0x40, 0xB8, 0x9E, 0xAA, 0xC2, 0x00
};
static const uint8_t data_44445[16] = {
  0x00, 0x80, 0x3F, 0x71, 0x3D, 0xA0, 0x42, 0x00, 0x00, 0x80, 0x3F, 0x48, 0x61, 0x91, 0xC2, 0x5C
};
static const uint8_t data_44455[16] = {
  0x0F, 0xA0, 0x42, 0x48, 0x61, 0x91, 0xC2, 0xC3, 0xF5, 0x97, 0x42, 0xCD, 0x4C, 0x8D, 0xC2, 0xC3
};
static const uint8_t data_44465[16] = {
  0xF5, 0x97, 0x42, 0xCD, 0x4C, 0x8D, 0xC2, 0x5C, 0x0F, 0xA0, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0x48
};
static const uint8_t data_44475[16] = {
  0xE1, 0x8A, 0x40, 0x0A, 0x57, 0x8F, 0xC2, 0xC3, 0xF5, 0xA8, 0x3F, 0x66, 0x66, 0x9D, 0x42, 0xC3
};
static const uint8_t data_44485[16] = {
  0xF5, 0xA8, 0x3F, 0xCD, 0xCC, 0x80, 0xC2, 0x3D, 0x0A, 0x8B, 0x42, 0xCD, 0xCC, 0x80, 0xC2, 0x00
};
static const uint8_t data_44495[16] = {
  0x00, 0x80, 0x42, 0x7B, 0x14, 0x72, 0xC2, 0x00, 0x00, 0x80, 0x42, 0x7B, 0x14, 0x72, 0xC2, 0x1F
};
static const uint8_t data_444A5[16] = {
  0x05, 0x8B, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0x33, 0x33, 0x73, 0x40, 0x0A, 0xD7, 0x79, 0xC2, 0x3D
};
static const uint8_t data_444B5[16] = {
  0x0A, 0xF7, 0x3F, 0x1F, 0x85, 0x85, 0x42, 0x3D, 0x0A, 0xF7, 0x3F, 0xC3, 0xF5, 0x80, 0xC2, 0x85
};
static const uint8_t data_444C5[16] = {
  0xEB, 0x95, 0x42, 0x33, 0x33, 0x7C, 0xC2, 0x85, 0xEB, 0x95, 0x42, 0x33, 0x33, 0x7C, 0xC2, 0x85
};
static const uint8_t data_444D5[16] = {
  0xEB, 0x9B, 0x42, 0xC3, 0xF5, 0x80, 0xC2, 0x85, 0xEB, 0x9B, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0x14
};
static const uint8_t data_444E5[16] = {
  0xAE, 0x57, 0x40, 0x7B, 0x14, 0x7F, 0xC2, 0x00, 0x00, 0x80, 0x3F, 0x85, 0xEB, 0x98, 0x42, 0x00
};
static const uint8_t data_444F5[16] = {
  0x00, 0x80, 0x3F, 0x33, 0x33, 0x36, 0xC2, 0xF6, 0xA8, 0x99, 0x42, 0xE1, 0x7A, 0x3B, 0xC2, 0x7B
};
static const uint8_t data_44505[16] = {
  0x14, 0x9B, 0x42, 0x3D, 0x0A, 0x3E, 0xC2, 0xF6, 0xA8, 0x98, 0x42, 0x9A, 0x99, 0x38, 0xC2, 0x33
};
static const uint8_t data_44515[16] = {
  0x33, 0x97, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0xA4, 0x70, 0x3D, 0x40, 0x7B, 0x14, 0x3A, 0xC2, 0x00
};
static const uint8_t data_44525[16] = {
  0x00, 0x80, 0x3F, 0xD7, 0x23, 0x99, 0x42, 0x00, 0x00, 0x80, 0x3F, 0xC3, 0xF5, 0x19, 0xC2, 0x85
};
static const uint8_t data_44535[16] = {
  0xEB, 0x92, 0x42, 0x00, 0x00, 0x1B, 0xC2, 0xF6, 0x28, 0x91, 0x42, 0xA4, 0x70, 0x11, 0xC2, 0x52
};
static const uint8_t data_44545[16] = {
  0xB8, 0x8F, 0x42, 0x29, 0x5C, 0x10, 0xC2, 0x3D, 0x8A, 0x91, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0x71
};
static const uint8_t data_44555[16] = {
  0x3D, 0x5A, 0x40, 0x52, 0xB8, 0x15, 0xC2, 0x00, 0x00, 0x80, 0x3F, 0xEC, 0x51, 0x91, 0x42, 0x00
};
static const uint8_t data_44565[16] = {
  0x00, 0x80, 0x3F, 0x71, 0x3D, 0xA0, 0xC1, 0xD7, 0x23, 0x91, 0x42, 0x71, 0x3D, 0xA0, 0xC1, 0x85
};
static const uint8_t data_44575[16] = {
  0x6B, 0x8C, 0x42, 0xAE, 0x47, 0x8D, 0xC1, 0x85, 0x6B, 0x8C, 0x42, 0xAE, 0x47, 0x8D, 0xC1, 0xD7
};
static const uint8_t data_44585[16] = {
  0x23, 0x91, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0xE1, 0x7A, 0x64, 0x40, 0x52, 0xB8, 0x96, 0xC1, 0x3D
};
static const uint8_t data_44595[16] = {
  0x0A, 0x97, 0x3F, 0xAE, 0xC7, 0x8E, 0x42, 0x3D, 0x0A, 0x97, 0x3F, 0x0A, 0xD7, 0x85, 0x41, 0x71
};
static const uint8_t data_445A5[16] = {
  0xBD, 0xBA, 0x42, 0xCD, 0xCC, 0x74, 0x41, 0x3D, 0x8A, 0xB8, 0x42, 0x0A, 0xD7, 0x7F, 0x41, 0x5C
};
static const uint8_t data_445B5[16] = {
  0x8F, 0xB6, 0x42, 0xA4, 0x70, 0x8B, 0x41, 0x0A, 0xD7, 0xB8, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0x48
};
static const uint8_t data_445C5[16] = {
  0xE1, 0x3A, 0x40, 0xC3, 0xF5, 0x82, 0x41, 0x00, 0x00, 0x80, 0x3F, 0x14, 0xAE, 0xB8, 0x42, 0x00
};
static const uint8_t data_445D5[16] = {
  0x00, 0x80, 0x3F, 0x9A, 0x99, 0x99, 0x41, 0x3D, 0x0A, 0x23, 0x42, 0x7B, 0x14, 0xA6, 0x41, 0x3D
};
static const uint8_t data_445E5[16] = {
  0x0A, 0x23, 0x42, 0x7B, 0x14, 0xA6, 0x41, 0xD7, 0xA3, 0x32, 0x42, 0x9A, 0x99, 0x99, 0x41, 0xD7
};
static const uint8_t data_445F5[16] = {
  0xA3, 0x32, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0xAE, 0x47, 0x71, 0x40, 0x0A, 0xD7, 0x9F, 0x41, 0x00
};
static const uint8_t data_44605[16] = {
  0x00, 0x80, 0x3F, 0x0A, 0xD7, 0x2A, 0x42, 0x00, 0x00, 0x80, 0x3F, 0x9A, 0x99, 0x99, 0x41, 0x9A
};
static const uint8_t data_44615[16] = {
  0x99, 0x3B, 0x42, 0x8F, 0xC2, 0xB5, 0x41, 0x9A, 0x99, 0x3B, 0x42, 0x8F, 0xC2, 0xB5, 0x41, 0x0A
};
static const uint8_t data_44625[16] = {
  0xD7, 0x50, 0x42, 0x9A, 0x99, 0x99, 0x41, 0x0A, 0xD7, 0x50, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0x33
};
static const uint8_t data_44635[16] = {
  0x33, 0x73, 0x40, 0x14, 0xAE, 0xA7, 0x41, 0x52, 0xB8, 0xDE, 0x3F, 0x33, 0x33, 0x46, 0x42, 0x52
};
static const uint8_t data_44645[16] = {
  0xB8, 0xDE, 0x3F, 0xD7, 0xA3, 0x80, 0x41, 0xD7, 0xA3, 0x62, 0x42, 0x71, 0x3D, 0x66, 0x41, 0xD7
};
static const uint8_t data_44655[16] = {
  0xA3, 0x62, 0x42, 0x71, 0x3D, 0x66, 0x41, 0xD7, 0xA3, 0x56, 0x42, 0xD7, 0xA3, 0x80, 0x41, 0xD7
};
static const uint8_t data_44665[16] = {
  0xA3, 0x56, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0x48, 0xE1, 0x6A, 0x40, 0x0A, 0xD7, 0x73, 0x41, 0x00
};
static const uint8_t data_44675[16] = {
  0x00, 0x80, 0x3F, 0x14, 0xAE, 0x5C, 0x42, 0x00, 0x00, 0x80, 0x3F, 0x9A, 0x99, 0x99, 0x41, 0xD7
};
static const uint8_t data_44685[16] = {
  0xA3, 0x5C, 0x42, 0x29, 0x5C, 0xA7, 0x41, 0xD7, 0xA3, 0x5C, 0x42, 0x29, 0x5C, 0xA7, 0x41, 0x9A
};
static const uint8_t data_44695[16] = {
  0x99, 0x68, 0x42, 0x1F, 0x85, 0x99, 0x41, 0x9A, 0x99, 0x68, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0x48
};
static const uint8_t data_446A5[16] = {
  0xE1, 0x6A, 0x40, 0xE1, 0x7A, 0xA0, 0x41, 0x00, 0x00, 0x80, 0x3F, 0x9A, 0x99, 0x62, 0x42, 0x00
};
static const uint8_t data_446B5[16] = {
  0x00, 0x80, 0x3F, 0x9A, 0x99, 0xAD, 0x41, 0xB8, 0x1E, 0x92, 0x42, 0x85, 0xEB, 0xA7, 0x41, 0xCD
};
static const uint8_t data_446C5[16] = {
  0x4C, 0x93, 0x42, 0x52, 0xB8, 0x9E, 0x41, 0x3D, 0x8A, 0x90, 0x42, 0xEC, 0x51, 0xA4, 0x41, 0x29
};
static const uint8_t data_446D5[16] = {
  0x5C, 0x8F, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0x7B, 0x14, 0xFE, 0x40, 0xF6, 0x28, 0xA6, 0x41, 0x00
};
static const uint8_t data_446E5[16] = {
  0x00, 0x80, 0x3F, 0xEC, 0x51, 0x91, 0x42, 0x00, 0x00, 0x80, 0x3F, 0x00, 0x00, 0xC6, 0x41, 0x7B
};
static const uint8_t data_446F5[16] = {
  0x94, 0x96, 0x42, 0x29, 0x5C, 0xCF, 0x41, 0x0A, 0x57, 0x99, 0x42, 0x29, 0x5C, 0xC9, 0x41, 0x5C
};
static const uint8_t data_44705[16] = {
  0x8F, 0x9A, 0x42, 0x71, 0x3D, 0xC0, 0x41, 0xEC, 0xD1, 0x97, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0xAE
};
static const uint8_t data_44715[16] = {
  0x47, 0x01, 0x41, 0x8F, 0xC2, 0xC7, 0x41, 0x00, 0x00, 0x80, 0x3F, 0x7B, 0x94, 0x98, 0x42, 0x00
};
static const uint8_t data_44725[16] = {
  0x00, 0x80, 0x3F, 0xAE, 0x47, 0xE7, 0x41, 0x71, 0x3D, 0xA0, 0x42, 0x5C, 0x8F, 0xF0, 0x41, 0x00
};
static const uint8_t data_44735[16] = {
  0x00, 0xA3, 0x42, 0xD7, 0xA3, 0xEA, 0x41, 0x52, 0x38, 0xA4, 0x42, 0xA4, 0x70, 0xE1, 0x41, 0xE1
};
static const uint8_t data_44745[16] = {
  0x7A, 0xA1, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0xAE, 0x47, 0x01, 0x41, 0x3D, 0x0A, 0xE9, 0x41, 0x00
};
static const uint8_t data_44755[16] = {
  0x00, 0x80, 0x3F, 0x71, 0x3D, 0xA2, 0x42, 0x00, 0x00, 0x80, 0x3F, 0xC3, 0xF5, 0x1D, 0x42, 0x0A
};
static const uint8_t data_44765[16] = {
  0x57, 0xA1, 0x42, 0xD7, 0xA3, 0x22, 0x42, 0x9A, 0x19, 0xA4, 0x42, 0xD7, 0xA3, 0x1F, 0x42, 0x0A
};
static const uint8_t data_44775[16] = {
  0x57, 0xA5, 0x42, 0x7B, 0x14, 0x1B, 0x42, 0x9A, 0x99, 0xA2, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0xAE
};
static const uint8_t data_44785[16] = {
  0x47, 0x01, 0x41, 0x0A, 0xD7, 0x1E, 0x42, 0x00, 0x00, 0x80, 0x3F, 0x0A, 0x57, 0xA3, 0x42, 0x00
};
static const uint8_t data_44795[16] = {
  0x00, 0x80, 0x3F, 0x85, 0xEB, 0xEA, 0xC2, 0x1F, 0x05, 0xC7, 0x42, 0xAE, 0xC7, 0xEB, 0xC2, 0xE1
};
static const uint8_t data_447A5[16] = {
  0xFA, 0xC8, 0x42, 0x66, 0xE6, 0xF1, 0xC2, 0x66, 0x66, 0xC6, 0x42, 0x7B, 0x14, 0xF1, 0xC2, 0xEC
};
static const uint8_t data_447B5[16] = {
  0x51, 0xC4, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0x7B, 0x14, 0x7E, 0x40, 0x85, 0x6B, 0xEE, 0xC2, 0x00
};
static const uint8_t data_447C5[16] = {
  0x00, 0xC0, 0x3F, 0x14, 0xAE, 0xC6, 0x42, 0x00, 0x00, 0xC0, 0x3F, 0x14, 0xAE, 0xFB, 0xC1, 0xCD
};
static const uint8_t data_447D5[16] = {
  0xCC, 0xA0, 0x42, 0xA4, 0x70, 0xDB, 0xC1, 0xCD, 0xCC, 0xA0, 0x42, 0xA4, 0x70, 0xDB, 0xC1, 0x1F
};
static const uint8_t data_447E5[16] = {
  0x05, 0xA4, 0x42, 0x14, 0xAE, 0xFB, 0xC1, 0x1F, 0x05, 0xA4, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0x48
};
static const uint8_t data_447F5[16] = {
  0xE1, 0x6A, 0x40, 0x9A, 0x99, 0xEB, 0xC1, 0x00, 0x00, 0x80, 0x3F, 0x66, 0x66, 0xA2, 0x42, 0x00
};
static const uint8_t data_44805[16] = {
  0x00, 0x80, 0x3F, 0x71, 0x3D, 0x8A, 0x3E, 0x52, 0x38, 0xB6, 0x42, 0x71, 0x3D, 0xAA, 0x3F, 0x00
};
static const uint8_t data_44815[16] = {
  0x80, 0xB3, 0x42, 0x71, 0x3D, 0x92, 0x40, 0x14, 0xAE, 0xB8, 0x42, 0x00, 0x00, 0x60, 0x40, 0x66
};
static const uint8_t data_44825[16] = {
  0x66, 0xBB, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0x48, 0xE1, 0x6A, 0x40, 0x48, 0xE1, 0x1A, 0x40, 0x00
};
static const uint8_t data_44835[16] = {
  0x00, 0xC0, 0x3F, 0xC3, 0x75, 0xB7, 0x42, 0x00, 0x00, 0xC0, 0x3F, 0xF6, 0x28, 0x84, 0xC0, 0xE1
};
static const uint8_t data_44845[16] = {
  0x7A, 0x99, 0x42, 0x7B, 0x14, 0x3E, 0xC0, 0x9A, 0x19, 0x94, 0x42, 0x52, 0xB8, 0xBE, 0xBF, 0x48
};
static const uint8_t data_44855[16] = {
  0x61, 0x95, 0x42, 0xC3, 0xF5, 0x28, 0xC0, 0x8F, 0xC2, 0x9A, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0xAE
};
static const uint8_t data_44865[16] = {
  0x47, 0x51, 0x40, 0x0A, 0xD7, 0x33, 0xC0, 0x00, 0x00, 0x80, 0x3F, 0xA4, 0x70, 0x97, 0x42, 0x00
};
static const uint8_t data_44875[3] = {
  0x00, 0x80, 0x3F
};
static const uint32_t data_44878 = 0xC297999A;
static const float data_4487C[195] = {
  62.51f, -69.12f, 62.51f, -69.12f, 69.64f, -75.79f, 69.64f, -0.1f, 3.9f, -72.46f, 3.58f, 66.07f, 3.58f, 41.74f, 5.91f, 42.39f, 4.64f, 47.25f, 7.12f,
  46.6f, 8.4f, -0.1f, 3.62f, 44.5f, 1.5f, 6.52f, 1.5f, -37.12f, 34.88f, -32.29f, 34.91f, -32.3f, 40.65f, -37.12f, 40.65f, -0.1f, 3.5f, -34.93f, 1.0f,
  36.73f, 2.5f, 10.64f, 31.2f, 15.25f, 31.2f, 15.25f, 36.51f, 10.64f, 36.51f, -0.1f, 3.5f, 12.94f, 2.65f, 33.86f, 2.65f, 22.54f, 12.55f, 24.78f,
  8.17f, 30.2f, 10.95f, 27.94f, 15.33f, -0.1f, 3.71f, 26.37f, 3.06f, 11.75f, 3.06f, -16.99f, 35.91f, -14.6f, 38.79f, -17.15f, 40.85f, -19.52f, 37.92f,
  -0.1f, 3.67f, -17.16f, 1.0f, 39.31f, 3.0f, -89.11f, 61.66f, -94.2f, 63.0f, -95.36f, 58.5f, -90.35f, 57.16f, -0.1f, 3.86f, -92.24f, 2.16f, 60.2f,
  2.16f, 4.33f, 31.24f, 4.3f, 36.6f, -0.04f, 36.52f, 0.0f, 31.15f, -0.1f, 3.77f, 2.13f, 2.55f, 33.87f, 2.55f, -50.56f, 47.3f, -47.61f, 51.37f,
  -50.33f, 53.17f, -53.09f, 49.25f, 0.0f, 4.9f, -50.88f, 1.0f, 49.69f, 2.0f, -97.78f, 94.96f, -90.02f, 100.65f, -97.63f, 110.33f, -106.26f, 103.19f,
  -0.1f, 6.13f, -99.43f, 3.56f, 101.32f, 6.0f, -54.74f, 89.75f, -46.57f, 85.61f, -40.17f, 98.13f, -49.44f, 102.86f, -0.1f, 6.72f, -49.49f, 4.49f,
  95.14f, 6.0f, -32.84f, 81.87f, -22.75f, 81.9f, -22.79f, 93.4f, -34.09f, 93.4f, -0.1f, 6.72f, -30.17f, 4.57f, 87.82f, 6.0f, -8.43f, 60.75f, 2.28f,
  65.28f, -1.67f, 74.53f, -12.81f, 71.4f, -0.1f, 6.72f, -6.12f, 3.67f, 70.32f, 6.0f, -2.0f, 88.92f, 5.87f, 95.06f, -1.29f, 104.08f, -10.35f, 96.96f,
  -0.1f, 6.72f, -4.21f, 3.77f, 94.66f, 6.0f
};
static const char data_44B88[] __attribute__((scn_string_literal)) = "/scene/02/D000/";
static const char data_44B98[] __attribute__((scn_string_literal)) = "MAP1";
static const char data_44B9D[] __attribute__((scn_string_literal)) = "/scene/02/D000/";
static const char data_44BAD[] __attribute__((scn_string_literal)) = "MAP0";
static const char data_44BB2[] __attribute__((scn_string_literal)) = "/scene/02/D000/";
static const char data_44BC2[] __attribute__((scn_string_literal)) = "MAP3";
static const char data_44BC7[] __attribute__((scn_string_literal)) = "/scene/02/D000/";
static const char data_44BD7[] __attribute__((scn_string_literal)) = "MAP2";
static const char data_44BDC[11] __attribute__((scn_string_literal)) = "bgm109.snd";
static const char data_44BE7[] __attribute__((scn_string_literal)) = "/scene/02/D000/";
static const char data_44BF7[] __attribute__((scn_string_literal)) = "YORU";
static const char data_44BFC[13] __attribute__((scn_string_literal)) = "seqdata3.bin";
static const char data_44C09[5] __attribute__((scn_string_literal)) = "YORU";
static const char data_44C0E[13] __attribute__((scn_string_literal)) = "seqdata4.bin";
static const char data_44C1B[5] __attribute__((scn_string_literal)) = "YORU";
static const char data_44C20[13] __attribute__((scn_string_literal)) = "seqdata5.bin";
static const char data_44C2D[5] __attribute__((scn_string_literal)) = "YORU";
static const char data_44C32[] __attribute__((scn_string_literal)) = "YORU";
static const char data_44C37[11] __attribute__((scn_string_literal)) = "SA1091A004";
static const char data_44C42[11] __attribute__((scn_string_literal)) = "SA1091A005";
static const char data_44C4D[11] __attribute__((scn_string_literal)) = "SA1091A006";
static const uint32_t data_44C58 = 380;
static const int32_t data_44C5C[6] = {1, 390, 0, 1018, 0, -1};
static const uint32_t data_44C74 = 510;
static const int32_t data_44C78[6] = {1, 525, 0, 1020, 0, -1};
static const uint32_t data_44C90 = 150;
static const int32_t data_44C94[6] = {1, 160, 0, 1019, 0, -1};
static const uint32_t data_44CAC = 530;
static const int32_t data_44CB0[6] = {1, 970, 0, 1014, 0, -1};
static const uint32_t data_44CC8 = 911;
static const int32_t data_44CCC[6] = {1, 155, 0, 1015, 0, -1};
static const char data_44CE4[7] __attribute__((scn_string_literal)) = "FREE02";
static const char data_44CEB[7] __attribute__((scn_string_literal)) = "FREE03";
static const char data_44CF2[7] __attribute__((scn_string_literal)) = "FREE04";
static const char data_44CF9[7] __attribute__((scn_string_literal)) = "FREE01";
static const char data_44D00[6] __attribute__((scn_string_literal)) = "E1040";
static const char data_44D06[10] __attribute__((scn_string_literal)) = "E1040A028";
static const char data_44D10[10] __attribute__((scn_string_literal)) = "E1040A029";
static const char data_44D1A[6] __attribute__((scn_string_literal)) = "E1040";
static const char data_44D20[10] __attribute__((scn_string_literal)) = "E1040A030";
static const char data_44D2A[10] __attribute__((scn_string_literal)) = "E1040A031";
static const char data_44D34[10] __attribute__((scn_string_literal)) = "E1040A032";
static const char data_44D3E[6] __attribute__((scn_string_literal)) = "E1040";
static const Vector3 data_44D44 = {.x = 41.99f, .y = 0.0f, .z = 23.13f};
static const Vector3 data_44D50 = {.x = 9.6f, .y = 0.0f, .z = 47.17f};
static const char data_44D5C[13] __attribute__((scn_string_literal)) = "e1shugyo.snd";
static const char data_44D69[10] __attribute__((scn_string_literal)) = "E1040A001";
static const char data_44D73[10] __attribute__((scn_string_literal)) = "E1040A003";
static const char data_44D7D[10] __attribute__((scn_string_literal)) = "E1040A009";
static const char data_44D87[10] __attribute__((scn_string_literal)) = "E1040A010";
static const char data_44D91[10] __attribute__((scn_string_literal)) = "E1040A012";
static const char data_44D9B[10] __attribute__((scn_string_literal)) = "E1040A013";
static const char data_44DA5[10] __attribute__((scn_string_literal)) = "E1040A014";
static const char data_44DAF[10] __attribute__((scn_string_literal)) = "E1040A018";
static const char data_44DB9[10] __attribute__((scn_string_literal)) = "E1040A020";
static const char data_44DC3[10] __attribute__((scn_string_literal)) = "E1040A015";
static const char data_44DCD[10] __attribute__((scn_string_literal)) = "E1040A016";
static const char data_44DD7[10] __attribute__((scn_string_literal)) = "E1040A017";
static const char data_44DE1[6] __attribute__((scn_string_literal)) = "E1040";
static const char data_44DE7[9] __attribute__((scn_string_literal)) = "E1040";
static const Vector3 data_44DF0 = {.x = 40.7094f, .y = 0.0f, .z = 21.8232f};
static const Vector3 data_44DFC = {.x = 39.43f, .y = 0.0f, .z = 21.84f};
static const Vector3 data_44E08 = {.x = 12.35f, .y = 0.0f, .z = 47.05f};
static const char data_44E14[30] __attribute__((scn_string_literal)) = "training_npc_too_close:  set\012";
static const char data_44E32[20] __attribute__((scn_string_literal)) = "\012yamagisi ga iru!\012\012";
static const char data_44E46[15] __attribute__((scn_string_literal)) = "\012ryu ga iru!\012\012";
static const char data_44E55[55] __attribute__((scn_string_literal)) = "training_npc_too_close reset before training starts \012";
static void (*data_44E8C)(void) = script_28DF7;
static void (*const data_44E90)(void) = script_28DFA;
static void (*const data_44E94)(int32_t) = script_28DFD;
static void (*const data_44E98)(int32_t) = script_28E2E;
static void (*const data_44E9C)(int32_t) = script_28E5F;
static void (*const data_44EA0)(int32_t) = script_28ECC;
static const Vector4 data_44EA4 = {.x = 0.3f, .y = 0.3f, .z = 0.3f, .w = 0.3f};
static const Vector4 data_44EB4 = {.x = 0.3f, .y = 0.3f, .z = 0.3f, .w = 0.3f};
static const Vector4 data_44EC4 = {.x = 0.3f, .y = 0.3f, .z = 0.3f, .w = 0.3f};
static const Vector4 data_44ED4 = {.x = 0.3f, .y = 0.3f, .z = 0.3f, .w = 0.3f};
static const char data_44EE4[] __attribute__((scn_string_literal)) = "/scene/02/D000/";
static const uint32_t data_44EF4 = 0x474D4F;
static const char data_44EF8[11] __attribute__((scn_string_literal)) = "M_MOBJ.BIN";
static const char data_44F03[] __attribute__((scn_string_literal)) = "/scene/02/D000/";
static const char data_44F13[] __attribute__((scn_string_literal)) = "SHUKI";
static const char data_44F19[14] __attribute__((scn_string_literal)) = "scene/02/D000";
static const char data_44F27[] __attribute__((scn_string_literal)) = "SCRTEST.BIN";
static const char data_44F33[] __attribute__((scn_string_literal)) = "BUSS";
static const char data_44F38[7] __attribute__((scn_string_literal)) = "01JIMB";
static const char data_44F3F[7] __attribute__((scn_string_literal)) = "E1004a";
static const char data_44F46[6] __attribute__((scn_string_literal)) = "01JIM";
static const char data_44F4C[12] __attribute__((scn_string_literal)) = "system1.snd";
static const char data_44F58[11] __attribute__((scn_string_literal)) = "amb022.snd";
static const char data_44F63[7] __attribute__((scn_string_literal)) = "SELECT";
static const char data_44F6A[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_44F6F[13] __attribute__((scn_string_literal)) = "f1dobuit.snd";
static const char data_44F7C[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_44F81[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_44F86[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_44F8B[12] __attribute__((scn_string_literal)) = "system1.snd";
static const char data_44F97[11] __attribute__((scn_string_literal)) = "amb051.snd";
static const char data_44FA2[7] __attribute__((scn_string_literal)) = "SELECT";
static const char data_44FA9[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_44FAE[13] __attribute__((scn_string_literal)) = "f1dobuit.snd";
static const char data_44FBB[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_44FC0[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_44FC5[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_44FCA[12] __attribute__((scn_string_literal)) = "system1.snd";
static const char data_44FD6[11] __attribute__((scn_string_literal)) = "amb050.snd";
static const char data_44FE1[7] __attribute__((scn_string_literal)) = "SELECT";
static const char data_44FE8[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_44FED[13] __attribute__((scn_string_literal)) = "f1dobuit.snd";
static const char data_44FFA[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_44FFF[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_45004[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_45009[12] __attribute__((scn_string_literal)) = "system1.snd";
static const char data_45015[11] __attribute__((scn_string_literal)) = "amb052.snd";
static const char data_45020[7] __attribute__((scn_string_literal)) = "SELECT";
static const char data_45027[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_4502C[13] __attribute__((scn_string_literal)) = "f1dobuit.snd";
static const char data_45039[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_4503E[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_45043[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_45048[12] __attribute__((scn_string_literal)) = "system1.snd";
static const char data_45054[11] __attribute__((scn_string_literal)) = "amb021.snd";
static const char data_4505F[7] __attribute__((scn_string_literal)) = "SELECT";
static const char data_45066[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_4506B[13] __attribute__((scn_string_literal)) = "f1dobuit.snd";
static const char data_45078[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_4507D[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_45082[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_45087[] __attribute__((scn_string_literal)) = "SHUKI";
static const char data_4508D[] __attribute__((scn_string_literal)) = "/scene/02/D000/";
static const char data_4509D[] __attribute__((scn_string_literal)) = "SHUKI";
static const uint8_t data_450A3[16] = {
  0x00, 0x9E, 0x02, 0xA0, 0x02, 0xA2, 0x02, 0xA5, 0x02, 0xAC, 0x02, 0xAA, 0x02, 0xA8, 0x02, 0x00
};
static const uint8_t data_450B3[16] = {
  0x03, 0xF8, 0x00, 0xFA, 0x00, 0xF5, 0x00, 0xF7, 0x00, 0xF6, 0x00, 0xFB, 0x00, 0xF9, 0x00, 0x00
};
static const uint8_t data_450C3[1] = {
  0x03
};
static const char data_450C4[61] __attribute__((scn_string_literal)) = "WAZA Warning : handle is not NULL, and create same handle !\012";
static const char data_45101[30] __attribute__((scn_string_literal)) = "POS POS_RYOU_0 = %f, %f, %f.\012";
static const char data_4511F[30] __attribute__((scn_string_literal)) = "POS POS_KAIS_0 = %f, %f, %f.\012";
static const char data_4513D[29] __attribute__((scn_string_literal)) = "define ANG_RYOU_0 DEG(%d.0)\012";
static const char data_4515A[29] __attribute__((scn_string_literal)) = "define ANG_KAIS_0 DEG(%d.0)\012";
static const char data_45177[4] __attribute__((scn_string_literal)) = "%s\012";
static const char data_4517B[37] __attribute__((scn_string_literal)) = "Konnna Error ha Detekonai hazu : %d\012";
static const Vector3 data_451A0 = {.x = 1.0f, .y = 1.0f, .z = 1.0f};
static const Vector3i data_451AC = {.x = 0, .y = 0, .z = 0};
static const char data_451B8[14] __attribute__((scn_string_literal)) = "Entry Model!\012";
static const char data_451C6[38] __attribute__((scn_string_literal)) = "WZ ModoriMove Limit Over !!!!!!!!!!!\012";
static const char data_451EC[30] __attribute__((scn_string_literal)) = "WazaDen: Load PackFile %s:%s\012";
static const char data_4520A[34] __attribute__((scn_string_literal)) = "WazaDen: Now Loading PackFile.\012";
static const Vector3 data_4522C = {.x = 1.0f, .y = 1.0f, .z = 1.0f};
static const uint32_t data_45238 = SHEN_NAME('ONO_');
static const shen_name data_4523C[9] = {CHARA_AKSK, CHARA_HOND, CHARA_TONY, CHARA_KURI, CHARA_MRUA, CHARA_SUMI, CHARA_YOSE, CHARA_NORK, CHARA_YMGC};
static const int32_t data_45260 = -1;
static const char data_45264[] __attribute__((scn_string_literal)) = "HANA";
static const uint8_t data_45269[7] = {
  0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF
};
static const uint32_t data_45270 = SHEN_NAME('HISA');
static const shen_name data_45274 = CHARA_YURI;
static const int32_t data_45278 = -1;
static const uint32_t data_4527C = SHEN_NAME('BN05');
static const shen_name data_45280[9] = {
  SHEN_NAME('BN06'), SHEN_NAME('BN07'), SHEN_NAME('BN08'), SHEN_NAME('BN09'), SHEN_NAME('BN10'), SHEN_NAME('BN13'), SHEN_NAME('BN14'),
  SHEN_NAME('BN15'), SHEN_NAME('BN16')
};
static const int32_t data_452A4 = -1;
static const uint32_t data_452A8 = SHEN_NAME('CAR2');
static const shen_name data_452AC[2] = {SHEN_NAME('CAR3'), SHEN_NAME('CAR4')};
static const int32_t data_452B4 = -1;
static const char data_452B8[11] __attribute__((scn_string_literal)) = "E1004AA001";
static const char data_452C3[11] __attribute__((scn_string_literal)) = "E1004AB001";
static const char data_452CE[11] __attribute__((scn_string_literal)) = "E1004AA002";
static const char data_452D9[11] __attribute__((scn_string_literal)) = "E1004AB002";
static const char data_452E4[11] __attribute__((scn_string_literal)) = "E1004AA003";
static const char data_452EF[11] __attribute__((scn_string_literal)) = "E1004AA004";
static const char data_452FA[11] __attribute__((scn_string_literal)) = "E1004AC001";
static const char data_45305[11] __attribute__((scn_string_literal)) = "E1004AA005";
static const char data_45310[11] __attribute__((scn_string_literal)) = "E1004AA006";
static const char data_4531B[11] __attribute__((scn_string_literal)) = "E1004AB003";
static const char data_45326[10] __attribute__((scn_string_literal)) = "E1004A007";
static const char data_45330[11] __attribute__((scn_string_literal)) = "E1004AA007";
static const char data_4533B[11] __attribute__((scn_string_literal)) = "E1004AA009";
static const char data_45346[13] __attribute__((scn_string_literal)) = "「よっと！」";
static const char data_45353[15] __attribute__((scn_string_literal)) = "「おわっと！」";
static const char data_45362[11] __attribute__((scn_string_literal)) = "E1004AD001";
static const char data_4536D[11] __attribute__((scn_string_literal)) = "E1004AB004";
static const char data_45378[11] __attribute__((scn_string_literal)) = "E1004AA010";
static const char data_45383[11] __attribute__((scn_string_literal)) = "E1004AA011";
static const char data_4538E[23] __attribute__((scn_string_literal)) = "「クソ、ジミーの野郎」";
static const char data_453A5[11] __attribute__((scn_string_literal)) = "E1004AA012";
static const char data_453B0[11] __attribute__((scn_string_literal)) = "E1004AA013";
static const char data_453BB[11] __attribute__((scn_string_literal)) = "E1004AA014";
static const char data_453C6[11] __attribute__((scn_string_literal)) = "E1004AA015";
static const char data_453D1[11] __attribute__((scn_string_literal)) = "E1004AA016";
static const char data_453DC[11] __attribute__((scn_string_literal)) = "E1004AE002";
static const char data_453E7[11] __attribute__((scn_string_literal)) = "E1004AE001";
static const char data_453F2[11] __attribute__((scn_string_literal)) = "E1004AB005";
static const char data_453FD[11] __attribute__((scn_string_literal)) = "「フン！」";
static const char data_45408[11] __attribute__((scn_string_literal)) = "E1004AF001";
static const char data_45413[11] __attribute__((scn_string_literal)) = "E1004AA017";
static const char data_4541E[11] __attribute__((scn_string_literal)) = "E1004AG001";
static const char data_45429[11] __attribute__((scn_string_literal)) = "E1004AA034";
static const char data_45434[11] __attribute__((scn_string_literal)) = "E1004AG002";
static const char data_4543F[11] __attribute__((scn_string_literal)) = "E1004AH001";
static const char data_4544A[11] __attribute__((scn_string_literal)) = "E1004AH002";
static const char data_45455[11] __attribute__((scn_string_literal)) = "E1004AH003";
static const char data_45460[11] __attribute__((scn_string_literal)) = "E1004AA029";
static const char data_4546B[11] __attribute__((scn_string_literal)) = "E1004AB006";
static const char data_45476[11] __attribute__((scn_string_literal)) = "E1004AA031";
static const char data_45481[11] __attribute__((scn_string_literal)) = "E1004AN002";
static const char data_4548C[11] __attribute__((scn_string_literal)) = "E1004AN001";
static const char data_45497[11] __attribute__((scn_string_literal)) = "E1004AO002";
static const char data_454A2[11] __attribute__((scn_string_literal)) = "E1004AO004";
static const char data_454AD[11] __attribute__((scn_string_literal)) = "E1004AA027";
static const char data_454B8[11] __attribute__((scn_string_literal)) = "E1004AL002";
static const char data_454C3[11] __attribute__((scn_string_literal)) = "E1004AA037";
static const char data_454CE[11] __attribute__((scn_string_literal)) = "E1004AA018";
static const char data_454D9[11] __attribute__((scn_string_literal)) = "E1004AL001";
static const char data_454E4[11] __attribute__((scn_string_literal)) = "E1004AA028";
static const char data_454EF[11] __attribute__((scn_string_literal)) = "E1004AM001";
static const char data_454FA[11] __attribute__((scn_string_literal)) = "E1004AI002";
static const char data_45505[11] __attribute__((scn_string_literal)) = "E1004AI001";
static const char data_45510[11] __attribute__((scn_string_literal)) = "E1004AI003";
static const char data_4551B[11] __attribute__((scn_string_literal)) = "E1004AA022";
static const char data_45526[11] __attribute__((scn_string_literal)) = "E1004AA023";
static const char data_45531[11] __attribute__((scn_string_literal)) = "E1004AL003";
static const char data_4553C[11] __attribute__((scn_string_literal)) = "E1004AJ001";
static const char data_45547[11] __attribute__((scn_string_literal)) = "E1004AJ002";
static const char data_45552[11] __attribute__((scn_string_literal)) = "E1004AA040";
static const char data_4555D[11] __attribute__((scn_string_literal)) = "E1004AB009";
static const char data_45568[10] __attribute__((scn_string_literal)) = "QTE [%d]\012";
static const char data_45572[11] __attribute__((scn_string_literal)) = "check end\012";
static const char data_4557D[8] __attribute__((scn_string_literal)) = "CTERMO1";
static const char data_45585[11] __attribute__((scn_string_literal)) = "CTERMO1";
static const uint8_t data_45590[4] = {0};
static const int32_t data_45594[9] = {20, 15, 3, 10, 5, 14, 1, 13, 8};
static const char data_455B8[] __attribute__((scn_string_literal)) = "BIKE";
static const char data_455BD[] __attribute__((scn_string_literal)) = "BIKE";
static const uint8_t data_455C2[2] = {0};
static const uint32_t data_455C4 = 18;
static const int32_t data_455C8[6] = {0, 17, 1, 8, 12, 7};
static const char data_455E0[] __attribute__((scn_string_literal)) = "KANB";
static const char data_455E5[] __attribute__((scn_string_literal)) = "KANB";
static const uint8_t data_455EA[2] = {0};
static const uint8_t data_455EC[4] = {0};
static const int32_t data_455F0[33] = {
  1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33
};
static const char data_45674[] __attribute__((scn_string_literal)) = "APLE";
static const char data_45679[] __attribute__((scn_string_literal)) = "APLE";
static const uint8_t data_4567E[2] = {0};
static const uint32_t data_45680 = 23;
static const int32_t data_45684[33] = {
  18, 0, 11, 6, 20, 15, 22, 17, 3, 10, 5, 14, 2, 21, 16, 1, 13, 8, 24, 19, 12, 7, 9, 4, 28, 29, 30, 31, 32, 33, 25, 26, 27
};
static const char data_45708[] __attribute__((scn_string_literal)) = "APL2";
static const uint8_t data_4570D[3] = {0};
static const uint32_t data_45710 = 153;
static const int32_t data_45714[16] = {154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 169, 172, 175};
static const char data_45754[] __attribute__((scn_string_literal)) = "SOBA";
static const char data_45759[] __attribute__((scn_string_literal)) = "SOBA";
static const uint8_t data_4575E[2] = {0};
static const uint32_t data_45760 = 153;
static const int32_t data_45764[3] = {154, 155, 156};
static const char data_45770[] __attribute__((scn_string_literal)) = "HAKO";
static const uint8_t data_45775[3] = {0};
static const uint32_t data_45778 = 0x3A0039;
static const int32_t data_4577C[4] = {3670071, 4063291, 61, 1};
static const float data_4578C[3] = {-67.88f, 79.77f, 0.0f};
static const int32_t data_45798[2] = {196608, 1};
static const float data_457A0[3] = {-69.94f, 81.09f, 0.0f};
static const int32_t data_457AC = 196608;
static const uint32_t data_457B0 = 4;
typedef struct Row_457B4 { float w0; float w1; float w2; int32_t w3; } Row_457B4;
static const Row_457B4 data_457B4[6] = {
  {-85.53f, 82.61f, 1.0f, 5},
  {-90.83f, 84.61f, 1.0f, 6},
  {-98.78f, 89.06f, 1.0f, 7},
  {-107.03f, 99.61f, 1.0f, 8},
  {-109.43f, 100.61f, 1.0f, 9},
  {-114.5f, 98.9f, 0.0f, 196608}
};
static const uint32_t data_45814 = 6;
typedef struct Row_45818 { float w0; float w1; float w2; int32_t w3; } Row_45818;
static const Row_45818 data_45818[4] = {
  {-98.78f, 89.06f, 1.0f, 7},
  {-107.03f, 99.61f, 1.0f, 8},
  {-109.43f, 100.61f, 1.0f, 9},
  {-114.5f, 98.9f, 0.0f, 196608}
};
static const uint32_t data_45858 = 8;
typedef struct Row_4585C { float w0; float w1; float w2; int32_t w3; } Row_4585C;
static const Row_4585C data_4585C[2] = {
  {-109.43f, 100.61f, 1.0f, 9},
  {-114.5f, 98.9f, 0.0f, 196608}
};
static const char data_4587C[] __attribute__((scn_string_literal)) = "DR28_001";
static const uint8_t data_45885[3] = {0};
static const uint32_t data_45888 = 5;
typedef struct Row_4588C { float w0; float w1; float w2; int32_t w3; } Row_4588C;
static const Row_4588C data_4588C[7] = {
  {-93.0f, 86.71f, 1.0f, 6},
  {-98.0f, 88.91f, 1.0f, 7},
  {-105.6f, 98.61f, 1.0f, 8},
  {-108.1f, 100.01f, 1.0f, 9},
  {-113.4f, 98.91f, 1.0f, 10},
  {-119.3f, 96.41f, 1.0f, 11},
  {-120.8f, 90.91f, 1.0f, 196608}
};
static const uint32_t data_458FC = 0xC2EB147B;
static const float data_45900[9] = {97.88f, -110.35f, 101.7f, -108.15f, 101.5f, -106.73f, 100.5f, -104.83f, 97.8f};
static const char data_45924[11] __attribute__((scn_string_literal)) = "E1004AA017";
static const char data_4592F[18] __attribute__((scn_string_literal)) = "check count = %d\012";
static const char data_45941[12] __attribute__((scn_string_literal)) = "TE1004AG001";
static const char data_4594D[] __attribute__((scn_string_literal)) = "DR01_009";
static const char data_45956[10] __attribute__((scn_string_literal)) = "LEFT[%d]\012";
static const char data_45960[11] __attribute__((scn_string_literal)) = "RIGHT[%d]\012";
static const char data_4596B[] __attribute__((scn_string_literal)) = "DR02_017";
static const char data_45974[] __attribute__((scn_string_literal)) = "DR15_009";
static const char data_4597D[] __attribute__((scn_string_literal)) = "DR12_001";
static const uint8_t data_45986[2] = {0};
static const uint32_t data_45988 = 0xC226999A;
static const float data_4598C[4] = {38.85f, -50.85f, 46.15f, -56.35f};
static const shen_name data_4599C = SHEN_NAME('33JB');
static const char data_459A0[11] __attribute__((scn_string_literal)) = "E1004AO001";
static const char data_459AB[] __attribute__((scn_string_literal)) = "DR15_011";
static const char data_459B4[11] __attribute__((scn_string_literal)) = "E1004AA007";
static const char data_459BF[11] __attribute__((scn_string_literal)) = "E1004AO003";
static const char data_459CA[11] __attribute__((scn_string_literal)) = "E1004AA026";
static const char data_459D5[11] __attribute__((scn_string_literal)) = "E1004AA036";
static const char data_459E0[11] __attribute__((scn_string_literal)) = "E1004AA037";
static const char data_459EB[11] __attribute__((scn_string_literal)) = "E1004AO002";
static const char data_459F6[12] __attribute__((scn_string_literal)) = "UNDEL[%ld]\012";
static const char data_45A02[10] __attribute__((scn_string_literal)) = "DEL[%ld]\012";
static const char data_45A0C[13] __attribute__((scn_string_literal)) = "a1_jimse.snd";
static const char data_45A19[12] __attribute__((scn_string_literal)) = "UNDEL[%ld]\012";
static const char data_45A25[10] __attribute__((scn_string_literal)) = "DEL[%ld]\012";
static const char data_45A2F[8] __attribute__((scn_string_literal)) = "CTERMO2";
static const char data_45A37[] __attribute__((scn_string_literal)) = "/scene/02/D000";
static const char data_45A46[] __attribute__((scn_string_literal)) = "CTERMO3";
static const char data_45A4E[7] __attribute__((scn_string_literal)) = "CTERST";
static const char data_45A55[7] __attribute__((scn_string_literal)) = "CTERST";
static const char data_45A5C[9] __attribute__((scn_string_literal)) = "CTERBASE";
static const char data_45A65[9] __attribute__((scn_string_literal)) = "CTERBASE";
static const char data_45A6E[9] __attribute__((scn_string_literal)) = "CTERMISE";
static const char data_45A77[9] __attribute__((scn_string_literal)) = "CTERMISE";
static const char data_45A80[8] __attribute__((scn_string_literal)) = "CTERMO1";
static const char data_45A88[8] __attribute__((scn_string_literal)) = "CTERMO1";
static const char data_45A90[8] __attribute__((scn_string_literal)) = "CTERMO2";
static const char data_45A98[8] __attribute__((scn_string_literal)) = "CTERMO2";
static const char data_45AA0[8] __attribute__((scn_string_literal)) = "CTERMO3";
static const char data_45AA8[8] __attribute__((scn_string_literal)) = "CTERMO3";
static const char data_45AB0[11] __attribute__((scn_string_literal)) = "rim006.snd";
static const char data_45ABB[13] __attribute__((scn_string_literal)) = "a1_jimma.snd";
static const char data_45AC8[13] __attribute__((scn_string_literal)) = "e1chasej.snd";
static const char data_45AD5[6] __attribute__((scn_string_literal)) = "SHUKI";
static const char data_45ADB[6] __attribute__((scn_string_literal)) = "SHUKI";
static const char data_45AE1[4] __attribute__((scn_string_literal)) = "OMG";
static const char data_45AE5[4] __attribute__((scn_string_literal)) = "OMG";
static const char data_45AE9[] __attribute__((scn_string_literal)) = "/scene/02/D000";
static const char data_45AF8[] __attribute__((scn_string_literal)) = "CTERMO1";
static const char data_45B00[8] __attribute__((scn_string_literal)) = "CTERMO1";
static const char data_45B08[1] __attribute__((scn_string_literal)) = "";
static const char data_45B09[8] __attribute__((scn_string_literal)) = "CTERMO1";
static const char data_45B11[1] __attribute__((scn_string_literal)) = "";
static const char data_45B12[8] __attribute__((scn_string_literal)) = "CTERMO1";
static const char data_45B1A[1] __attribute__((scn_string_literal)) = "";
static const char data_45B1B[] __attribute__((scn_string_literal)) = "/scene/02/D000";
static const char data_45B2A[] __attribute__((scn_string_literal)) = "CTERMO2";
static const char data_45B32[] __attribute__((scn_string_literal)) = "/scene/02/D000";
static const char data_45B41[] __attribute__((scn_string_literal)) = "CTERST";
static const char data_45B48[] __attribute__((scn_string_literal)) = "/scene/02/D000";
static const char data_45B57[] __attribute__((scn_string_literal)) = "CTERBASE";
static const char data_45B60[] __attribute__((scn_string_literal)) = "/scene/02/D000";
static const char data_45B6F[] __attribute__((scn_string_literal)) = "CTERMISE";
static const char data_45B78[8] __attribute__((scn_string_literal)) = "CTERMO3";
static const char data_45B80[] __attribute__((scn_string_literal)) = "/scene/02/D000";
static const char data_45B8F[] __attribute__((scn_string_literal)) = "CTERMO2";
static const char data_45B97[7] __attribute__((scn_string_literal)) = "01JIMB";
static const char data_45B9E[7] __attribute__((scn_string_literal)) = "E1004a";
static const char data_45BA5[6] __attribute__((scn_string_literal)) = "01JIM";
static const char data_45BAB[13] __attribute__((scn_string_literal)) = "a1_jimsi.snd";
static const char data_45BB8[14] __attribute__((scn_string_literal)) = "scene/01/D000";
static const char data_45BC6[] __attribute__((scn_string_literal)) = "SCRTEST.BIN";
static const char data_45BD2[14] __attribute__((scn_string_literal)) = "scene/02/D000";
static const char data_45BE0[] __attribute__((scn_string_literal)) = "SCRTEST.BIN";
static const char data_45BEC[24] __attribute__((scn_string_literal)) = " Error : syuki init\012";
static const uint32_t data_45C04 = 256;
static const int32_t data_45C08[35] = {
  20, 14, 4, 3, -1, 47, -1, 1, -1, 3, 5, 512, 20, 14, 4, 4, -1, 149, -1, 7, -1, 9, 11, 64, 20, 16, 4, 0, -1, 0, -1, 13, -1, 15, 17
};
static const uint32_t data_45C94 = 1;
static const float data_45C98[3] = {-109.5f, 100.8f, 1.0f};
static const int32_t data_45CA4 = 196608;
static const uint32_t data_45CA8 = 152;
static const int32_t data_45CAC[23] = {153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175};
static const char data_45D08[10] __attribute__((scn_string_literal)) = "E1005A001";
static const char data_45D12[10] __attribute__((scn_string_literal)) = "E1005A002";
static const char data_45D1C[10] __attribute__((scn_string_literal)) = "E1005A003";
static const char data_45D26[10] __attribute__((scn_string_literal)) = "E1005A004";
static const char data_45D30[10] __attribute__((scn_string_literal)) = "E1005B001";
static const char data_45D3A[10] __attribute__((scn_string_literal)) = "E1005B002";
static const char data_45D44[10] __attribute__((scn_string_literal)) = "E1005B003";
static const char data_45D4E[10] __attribute__((scn_string_literal)) = "E1005B004";
static const char data_45D58[10] __attribute__((scn_string_literal)) = "E1005B005";
static const char data_45D62[10] __attribute__((scn_string_literal)) = "E1005B006";
static const char data_45D6C[10] __attribute__((scn_string_literal)) = "E1005B007";
static const char data_45D76[10] __attribute__((scn_string_literal)) = "E1005B008";
static const char data_45D80[10] __attribute__((scn_string_literal)) = "E1005B009";
static const char data_45D8A[10] __attribute__((scn_string_literal)) = "E1005B010";
static const char data_45D94[10] __attribute__((scn_string_literal)) = "E1005B011";
static const char data_45D9E[10] __attribute__((scn_string_literal)) = "E1005B012";
static const char data_45DA8[10] __attribute__((scn_string_literal)) = "E1005B013";
static const char data_45DB2[10] __attribute__((scn_string_literal)) = "E1005B014";
static const char data_45DBC[7] __attribute__((scn_string_literal)) = "開始０";
static const char data_45DC3[7] __attribute__((scn_string_literal)) = "成功０";
static const char data_45DCA[7] __attribute__((scn_string_literal)) = "失敗０";
static const char data_45DD1[7] __attribute__((scn_string_literal)) = "開始１";
static const char data_45DD8[7] __attribute__((scn_string_literal)) = "成功１";
static const char data_45DDF[7] __attribute__((scn_string_literal)) = "失敗１";
static const char data_45DE6[7] __attribute__((scn_string_literal)) = "開始２";
static const char data_45DED[7] __attribute__((scn_string_literal)) = "成功２";
static const char data_45DF4[7] __attribute__((scn_string_literal)) = "失敗２";
static const char data_45DFB[7] __attribute__((scn_string_literal)) = "開始３";
static const char data_45E02[7] __attribute__((scn_string_literal)) = "成功３";
static const char data_45E09[7] __attribute__((scn_string_literal)) = "失敗３";
static const char data_45E10[7] __attribute__((scn_string_literal)) = "開始４";
static const char data_45E17[7] __attribute__((scn_string_literal)) = "成功４";
static const char data_45E1E[7] __attribute__((scn_string_literal)) = "失敗４";
static const char data_45E25[7] __attribute__((scn_string_literal)) = "成功５";
static const char data_45E2C[7] __attribute__((scn_string_literal)) = "失敗５";
static const char data_45E33[] __attribute__((scn_string_literal)) = "DAT1";
static const int32_t data_45E38[3] = {0, 50426, 0};
static const Vector3 data_45E44 = {.x = -106.95f, .y = 0.0f, .z = 98.34f};
static const Vector3i data_45E50 = {.x = 0, .y = 27672, .z = 0};
static const Vector3 data_45E5C = {.x = -114.446602f, .y = 0.0f, .z = 99.066704f};
static const Vector3i data_45E68 = {.x = 0, .y = 44236, .z = 0};
static const char data_45E74[12] __attribute__((scn_string_literal)) = "system1.snd";
static const char data_45E80[11] __attribute__((scn_string_literal)) = "tsm002.snd";
static const char data_45E8B[13] __attribute__((scn_string_literal)) = "e1yoppar.snd";
static const char data_45E98[11] __attribute__((scn_string_literal)) = "qtebat.snd";
static const char data_45EA3[] __attribute__((scn_string_literal)) = "/scene/02/D000/";
static const char data_45EB3[] __attribute__((scn_string_literal)) = "DYQB";
static const char data_45EB8[8] __attribute__((scn_string_literal)) = "E1005";
static const Vector3 data_45EC0 = {.x = -107.0f, .y = 0.0f, .z = 99.7f};
static const Vector3i data_45ECC = {.x = 0, .y = 58600, .z = 0};
static const char data_45ED8[6] __attribute__((scn_string_literal)) = "E1005";
static const char data_45EDE[6] __attribute__((scn_string_literal)) = "DYQB";
static const Vector3 data_45EE4 = {.x = 0.0f, .y = 0.0f, .z = 0.0f};
static const Vector3i data_45EF0 = {.x = 0, .y = 0, .z = 0};
typedef struct Row_45EFC { int32_t w0; float w1; float w2; int32_t w3; } Row_45EFC;
static const Row_45EFC data_45EFC[2] = {
  {0, 1.0f, 0.0f, 0},
  {0, 2.0f, -2.0f, 0}
};
static const float data_45F1C[3] = {0.6f, 0.6f, 0.6f};
static const char data_45F28[4] __attribute__((scn_string_literal)) = "";
static const char data_45F2C[7] __attribute__((scn_string_literal)) = "SA1115";
static const char data_45F33[7] __attribute__((scn_string_literal)) = "SA1115";
static const char data_45F3A[11] __attribute__((scn_string_literal)) = "SA1115A001";
static const char data_45F45[11] __attribute__((scn_string_literal)) = "SA1115A002";
static const char data_45F50[12] __attribute__((scn_string_literal)) = "SA1115A003";
