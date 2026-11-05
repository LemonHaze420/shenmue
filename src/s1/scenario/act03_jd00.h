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
static int32_t script_5DCF(int32_t arg0);
static void script_5DED(float arg0);
static int32_t script_5E0C(int32_t arg0, int32_t arg1);
static void script_5E84(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4);
static int32_t script_6436(int32_t arg0);
static void script_6507(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_6517(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_661B(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_67BD(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_691E(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_6A7F(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_6D1D(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_6DEB(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_6E52(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_6EB9(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_6F00(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_7036(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_78F7(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_7A15(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_80DA(int32_t arg0, int32_t arg1);
static void script_81B0(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_90C3(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_912F(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_95E9(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_9CA3(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_B5A1(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_BCC2(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_C44C(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_C524(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_C99B(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_D92E(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_DD78(int32_t arg0, DSWKDoorEntry *arg1, int32_t *arg2, DoorResourceDescriptor *arg3, MapObjectRecord *arg4, int32_t arg5, int32_t arg6);
static void script_DF04(DSWKDoorEntry *arg0, int32_t *arg1, DoorResourceDescriptor *arg2, MapObjectRecord *arg3, int32_t arg4);
static void script_DF38(void);
static void script_DFA1(int32_t arg0, int32_t arg1);
static void script_E03E(int32_t arg0, int32_t arg1);
static void script_E078(void);
static void script_E0DB(void);
static void script_E13E(int32_t arg0);
static void script_E175(int32_t arg0);
static void script_E1AC(int32_t arg0);
static void script_E3EE(int32_t arg0);
static void script_E437(int32_t arg0);
static void script_E480(int32_t arg0, int32_t arg1);
static void script_E4F6(int32_t arg0);
static void script_E504(int32_t arg0);
static void script_E512(int32_t arg0);
static void script_E5D9(void);
static void script_E5E5(void);
static void script_E5F1(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5, int32_t arg6);
static int32_t script_E634(void);
static int8_t script_E745(int32_t arg0, int32_t arg1, int8_t arg2);
static void script_E7DD(int32_t arg0);
static void script_E9AC(int32_t arg0);
static int32_t script_E9DD(int32_t arg0, int32_t arg1, int32_t arg2, float arg3, float arg4, int32_t arg5, int32_t arg6);
static void script_F0A6(int32_t arg0, int32_t arg1);
static void script_F0D8(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_F248(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_F404(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_F5B8(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_F7CE(int32_t arg0, int32_t arg1);
static void script_F898(int32_t arg0, int32_t arg1);
static void script_F8F9(int32_t arg0);
static void script_FA9E(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_FB36(int32_t arg0, int32_t arg1);
static void script_FC18(int32_t arg0);
static void script_FC6F(int32_t arg0, float arg1, float arg2, float arg3);
static void script_FC97(int32_t arg0, float arg1, float arg2, float arg3);
static void script_FCEB(int32_t arg0, int32_t arg1, int32_t arg2);
static int32_t script_FD3B(int32_t arg0);
static void script_FDE2(int32_t arg0, int32_t arg1);
static int32_t script_FE05(int32_t arg0);
static void script_FE38(int32_t arg0, int32_t arg1);
static void script_FE5B(int32_t arg0, int32_t arg1);
static void script_FEBA(int32_t arg0);
static void script_FECF(void);
static void script_FEDE(void);
static void script_FEED(void);
static void script_FEF0(void);
static void script_FEF3(void);
static void script_FEF6(void);
static void script_FEF9(void);
static void script_FF06(void);
static void script_FF12(void);
static int32_t script_FF21(void);
static void script_FF30(int32_t arg0);
static void script_100B6(void);
static void script_1016B(int32_t arg0);
static void script_1018F(int32_t arg0);
static void script_101CE(int32_t arg0);
static void script_10254(void);
static void script_1029D(int32_t arg0);
static void script_102A8(int32_t arg0);
static int32_t script_10322(void);
static int32_t script_103AA(void);
static int32_t script_1041E(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_104B4(int32_t arg0);
static int16_t script_10655(int8_t arg0, int8_t arg1);
static void script_10840(int32_t arg0, int32_t arg1);
static int32_t script_1088F(int32_t arg0, int32_t arg1);
static void script_108E2(int32_t arg0, int32_t arg1, int32_t arg2, float arg3);
static void script_10B19(void);
static int32_t script_10CC2(int32_t arg0);
static int32_t script_10D2C(int32_t arg0);
static int32_t script_10F5D(void);
static int32_t script_1100D(void);
static int32_t script_110B0(void);
static void script_11142(void);
static void script_1116F(int32_t arg0);
static void script_11241(void);
static void script_113E8(void);
static void script_114C2(void);
static void script_11586(void);
static void script_115B5(int32_t arg0);
static void script_1173E(int32_t arg0, int32_t arg1);
static int32_t script_117BF(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4);
static int32_t script_11879(void);
static int32_t script_119B4(float arg0);
static int32_t script_11A84(void);
static int32_t script_11B4D(void);
static int32_t script_11BF7(void);
static int32_t script_11C41(void);
static void script_11D3C(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_11F2D(void);
static int32_t script_122C9(void);
static int32_t script_123BB(int32_t arg0);
static int32_t script_1243B(int32_t arg0);
static void script_1256D(int32_t arg0);
static int32_t script_127D9(void);
static void script_128B1(void);
static int32_t script_128B4(int32_t arg0);
static int32_t script_129CB(int16_t arg0);
static int32_t script_12AF5(void);
static void script_12B1E(void);
static int32_t script_12B88(void);
static int32_t script_12C48(void);
static int32_t script_12F67(void);
static void script_13044(void);
static int32_t script_1305C(void);
static int32_t script_1306F(void);
static void script_1314A(void);
static int32_t script_13225(int32_t arg0, int32_t arg1);
static void script_133DE(int32_t arg0);
static void script_13471(int32_t arg0, int32_t arg1);
static void script_13527(void);
static void script_135A1(int32_t arg0);
static int32_t script_136B7(void);
static int32_t script_13BE3(int32_t arg0, const char *arg1, const char *arg2, ...);
static void script_13C35(int32_t arg0, Vector3 *arg1);
static void script_13C6D(int32_t arg0, Vector3i *arg1);
static int32_t script_13CA5(void);
static void script_13E2B(int32_t arg0);
static int32_t script_13F38(int32_t arg0);
static int32_t script_13F93(void);
static int32_t script_140D5(void);
static void script_1417B(void);
static int32_t script_141D4(void);
static int32_t script_1420E(int32_t arg0);
static void script_14231(void);
static int32_t script_14251(int32_t arg0);
static void script_1429C(void);
static void script_142E3(void);
static void script_14336(void);
static void script_14392(void);
static void script_143D9(void);
static void script_14435(void);
static void script_14503(void);
static void script_14595(void);
static void script_145D2(void);
static int32_t script_1460F(int32_t arg0);
static void script_14680(int32_t arg0);
static int32_t script_1489B(void);
static void script_148E3(void);
static void script_148EB(void);
static void script_148FF(void);
static void script_14C01(int32_t arg0, int32_t arg1);
static void script_14C60(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static int32_t script_14C87(float arg0, float arg1, float arg2, float arg3);
static int32_t script_14E25(void);
static void script_14EEC(const char *arg0, int32_t arg1);
static void script_14F39(void);
static int32_t script_15070(int32_t arg0);
static void script_150E9(void);
static int8_t script_1514B(void);
static void script_15196(int32_t arg0);
static void script_151FF(int32_t arg0);
static void script_1522A(void);
static int32_t script_15251(int32_t arg0);
static int8_t script_15293(int32_t arg0);
static void script_1536B(void);
static void script_15378(void);
static void script_157E2(void);
static void script_16347(void);
static void script_16405(void);
static void script_1642A(void);
static void script_16B71(void);
static int8_t script_18BF8(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int8_t script_18CB1(void);
static void script_18E67(int32_t arg0);
static int8_t script_19FD8(void);
static void script_1A56D(int32_t arg0);
static void script_1E42B(void);
static int8_t script_1EEF3(void);
static int8_t script_1EF1B(void);
static void script_1F036(int32_t arg0);
static int32_t script_1F11F(int32_t arg0, int32_t arg1);
static int32_t script_1F188(void);
static void script_1F1A3(int32_t arg0);
static void script_1F28C(int32_t arg0);
static void script_1F47A(void);
static void script_1F4B1(int32_t arg0);
static int8_t script_1F579(void);
static void script_20256(void);
static void script_20319(void);
static void script_20641(void);
static void script_206C6(void);
static void script_20755(void);
static int32_t script_2096B(void);
static int32_t script_20A20(void);
static void script_213A2(void);
static void script_214A1(void);
static void script_21514(void);
static void script_216FD(void);
static void script_21711(void);
static void script_217D9(void);
static int32_t script_2191E(void);
static void script_219A1(void);
static void script_219F7(void);
static int32_t script_21BBB(void);
static int8_t script_21E15(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_21ECE(void);
static int32_t script_21F0A(void);
static void script_2219F(void);
static void script_221BE(void);
static void script_221CC(void);
static void script_221CF(int32_t arg0);
static void script_221F2(int32_t arg0);
static void script_22215(int32_t arg0);
static void script_22238(void);
static int32_t script_2229F(void);
static void script_22373(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_2240A(void);
static int32_t script_22432(void);
static int8_t script_2244C(void);
static int32_t script_2253C(void);
static int32_t script_22612(void);
static int32_t script_22727(void);
static int8_t script_22866(void);
static void script_228F5(int32_t arg0);
static int32_t script_22973(int32_t arg0, int32_t arg1);
static int32_t script_22985(int32_t arg0, int32_t arg1);
static void script_22A3C(void);
static int32_t script_22A62(void);
static void script_22C97(int32_t arg0);
static void script_22CD2(int32_t arg0);
static void script_22CFC(int32_t arg0);
static void script_22DD9(int32_t arg0);
static void script_22F42(void);
static void script_2304D(void);
static void script_2309B(int32_t arg0);
static void script_230D6(int32_t arg0);
static int32_t script_23100(void);
static void script_2313C(int32_t arg0);
static void script_23477(void);
static void script_235D5(void);
static int32_t script_23645(void);
static void script_23986(void);
static void script_23A34(void);
static void script_23A84(int32_t arg0);
static void script_23B20(int32_t arg0);
static void script_23B5E(int32_t arg0);
static void script_23B81(int32_t arg0);
static int32_t script_23BC7(void);
static int32_t script_23D2A(int32_t arg0, int32_t arg1);
static void script_23E0B(int32_t arg0);
void script_242A7(void);
static int32_t script_2464A(void);
static void script_2466F(void);
static void script_249CD(void);
static void script_24A05(void);
static void script_24B3D(int32_t arg0);
static void script_24B49(int32_t arg0);
static void script_24B55(int32_t arg0);
static void script_24C61(int32_t arg0);
static void script_24C78(int32_t arg0);
static void script_24CB9(int32_t arg0);
static void script_24CFD(int32_t arg0);
static void script_24D7B(void);
static void script_24DFA(void);
static void script_24E65(int32_t arg0, int32_t arg1);
static void script_24ECC(int32_t arg0, int32_t arg1);
static void script_24EF8(void);
static void script_24F33(void);
static void script_24F46(void);
static void script_24F52(void);
static void script_250EB(void);
static void script_25154(void);

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
static int32_t task_C8;
static char *task_CC;
static int32_t task_D0;
static int32_t task_D4;
static int32_t task_D8;
static int8_t task_DC;
static int8_t task_DD;
static int8_t task_DE;
static uint8_t task_DF;
static int32_t task_E0;
static int32_t task_E4;
static DSWKDoorEntry *task_E8;
static int32_t *task_EC;
static DoorResourceDescriptor *task_F0;
static MapObjectRecord *task_F4;
static DoorSlotState task_F8;
static int32_t task_104;
static int32_t task_108[10];
static int32_t task_130[10];
static int32_t task_158[10];
static int32_t task_180[10];
static int32_t task_1A8[10];
static int32_t task_1D0[10];
static int32_t task_1F8[10];
static int32_t task_220[10];
static int32_t task_248;
static int32_t task_24C;
static Vector3 task_250;
static int32_t task_25C;
static int32_t task_260;
static int32_t task_264;
static int8_t task_268;
static uint8_t task_269;
static uint16_t task_26A;
static int8_t task_26C;
static uint8_t task_26D;
static uint16_t task_26E;
static int32_t task_270;
static int32_t task_274;
static int8_t task_278;
static uint8_t task_279;
static uint16_t task_27A;
static Vector3 task_27C;
static Vector3 task_288;
static Vector3 task_294;
static int32_t task_2A0;
static int32_t task_2A4;
static int32_t task_2A8;
static int32_t task_2AC;
static int32_t task_2B0;
static int32_t task_2B4;
static Vector3 task_2B8;
static Vector3i task_2C4;
static int32_t task_2D0;
static int8_t task_2D4;
static uint8_t task_2D5;
static uint16_t task_2D6;
static int32_t task_2D8;
static int16_t task_2DC;
static int8_t task_2DE;
static uint8_t task_2DF;
static int32_t task_2E0;
static int32_t task_2E4;
static int32_t task_2E8;
static int8_t task_2EC;
static uint8_t task_2ED;
static uint16_t task_2EE;
static int32_t task_2F0;
static int8_t task_2F4;
static uint8_t task_2F5;
static uint16_t task_2F6;
static int32_t task_2F8;
static int32_t task_2FC;
static int8_t task_300;
static int8_t task_301;
static int8_t task_302;
static int8_t task_303;
static int32_t task_304;
static int32_t task_308;
static int32_t task_30C;
static int32_t task_310;
static int32_t task_314;
static int32_t task_318;
static int32_t task_31C;
static int32_t task_320;
static int32_t task_324;
static int32_t task_328;
static int8_t task_32C;
static int8_t task_32D;
static int8_t task_32E;
static int8_t task_32F;
static int8_t task_330;
static uint8_t task_331;
static uint16_t task_332;
static int8_t task_334;
static uint8_t task_335;
static uint16_t task_336;
static int32_t task_338[23];
static int8_t task_394;
static int8_t task_395;
static int8_t task_396;
static int8_t task_397;
static Vector3i task_398;
static Vector3i task_3A4;
static int32_t task_3B0;
static const float data_2518C[3] = {1.0f, 1.0f, 1.0f};
static const int32_t data_25198[4] = {16777215, 16777215, 16777215, 16777215};
static const float data_251A8[3] = {20.0f, 800.0f, 1.0f};
static const int32_t data_251B4 = -1;
static const char data_251B8[10] __attribute__((scn_string_literal)) = "Kaidan : ";
static const char data_251C2[32] __attribute__((scn_string_literal)) = "========== initializing kaidan\012";
static const char data_251E2[48] __attribute__((scn_string_literal)) = "please ENDOFKDATA use can't start shuki kaidan\012";
static const char data_25212[] __attribute__((scn_string_literal)) = "Kaidan Function Error 1\n";
static const char data_2522B[] __attribute__((scn_string_literal)) = "Kaidan Function Error 2\n";
static const char data_25244[] __attribute__((scn_string_literal)) = "Kaidan Data Error\n";
static const char data_25257[10] __attribute__((scn_string_literal)) = "Kaidan : ";
static const char data_25261[28] __attribute__((scn_string_literal)) = "wait ... shuki shuki shuki\012";
static const char data_2527D[10] __attribute__((scn_string_literal)) = "Kaidan : ";
static const char data_25287[24] __attribute__((scn_string_literal)) = "KAIDAN : Warning coli!\012";
static const char data_2529F[24] __attribute__((scn_string_literal)) = "KAIDAN : Warning coli!\012";
static const char data_252B7[24] __attribute__((scn_string_literal)) = "KAIDAN : Warning coli!\012";
static const char data_252CF[24] __attribute__((scn_string_literal)) = "KAIDAN : Warning coli!\012";
static const char data_252E7[24] __attribute__((scn_string_literal)) = "KAIDAN : Warning coli!\012";
static const char data_252FF[24] __attribute__((scn_string_literal)) = "KAIDAN : Warning setY!\012";
static const char data_25317[24] __attribute__((scn_string_literal)) = "KAIDAN : Warning setY!\012";
static const char data_2532F[925] = "Kaidan : \000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000V\325\377\377\000\000\000\000\000\000\000\000\302\326\377\377\000\000\000\000\260\005\000\000\000\000\000\000\000\000\000\000\000\000\000\000\232\271\377\377\000\000\000\000\000\000\000\000(\275\377\377\000\000\000\000\000\000\000\000.\270\377\377\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\\\320\377\377\000\000\000\000\000\000\000\000\344\270\377\377\000\000\000\000\000\000\000\000\224\276\377\377\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\310\321\377\377\000\000\000\000\000\000\000\000.\270\377\377\000\000\000\000\000\000\000\000\373\304\377\377\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\216\003\000\000\022\321\377\377\000\000\000\000x\367\377\377\224\276\377\377\000\000\000\000x\367\377\377\344\330\377\377\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\224\376\377\377~\322\377\377\240\364\377\377\224\376\377\377\357\356\377\377f\006\000\000\216\003\000\000\266\000\000\000\000\000\000\000\000\000\000\000m\301\377\377\000\000\000\000\266\000\000\000\261\245\377\377\000\000\000\000\"\002\000\000\014\266\377\377\000\000\000\000\000\000\000\000\274\373\377\377\000\000\000\000\000\000\000\000\000\300\377\377\000\000\000\000\000\000\000\000\253\252\377\377\000\000\000\000\000\000\000\000\302\266\377\377\000\000\000\000\000\000\000\000\336\375\377\377\000\000\000\000\000\000\000\000\027\254\377\377\000\000\000\000\000\000\000\000\315\254\377\377\000\000\000\000\000\000\000\000x\267\377\377\000\000\000\000\000\000\000\000(\375\377\377\000\000\000\000\216\003\000\000\302\266\377\377\000\000\000\000\266\000\000\000\302\266\377\377\000\000\000\000\\\360\377\377\344\270\377\377\000\000\000\000\000\000\000\000(\375\377\377\000\000\000\000\224\376\377\377?\311\377\377\240\364\377\377\224\376\377\377\261\345\377\377f\006\000\000\216\003\000\000\364\011\000\000\000\000\000\000\000\000\000\000x\267\377\377\000\000\000\000\266\000\000\000\274\233\377\377\000\000\000\000\"\002\000\000\027\254\377\377\000\000\000\000\000\000\000\000\310\361\377\377\000\000\000\000\000\000\000\000\014\266\377\377\000\000\000\000\000\000\000\000\267\240\377\377\000\000\000\000\000\000\000\000\315\254\377\377\000\000\000\000\000\000\000\000\352\363\377\377\000\000\000\000\000\000\000\000#\242\377\377\000\000\000\000\000\000\000\000\331\242\377\377\000\000\000\000\000\000\000\000\203\255\377\377\000\000\000\000\000\000\000\0004\363\377\377\000\000\000\000\216\003\000\000\315\254\377\377\000\000\000\000\266\000\000\000\315\254\377\377\000\000\000\000\\\360\377\377\357\256\377\377\000\000\000\000\000\000\000\0004\363\377\377";
static const int32_t data_256CC[114] = {
  0, 0, -5461, 0, -2366, -1092, 0, -182, 1092, 0, -546, -6735, 0, 0, -4004, 0, 0, -2912, 0, -182, 1274, 0, 0, -13289, 0, 0, -5279, 0, 0, -8009, 0, 0,
  1456, 0, 182, -8738, 0, 0, -6007, 0, 0, -8738, 0, 182, 1638, 0, -728, -5279, 0, -1274, -6735, 0, -910, -8738, 0, 0, 1638, 0, 0, -5461, 0, -2366,
  -1092, 0, -182, 1092, 0, -546, -13289, 0, 0, -5643, 0, 0, -8009, 0, -182, 1274, 0, 0, -11832, 0, 0, -5279, 0, 0, -8009, 0, 0, 1456, 0, 182, -8738,
  0, 0, -6007, 0, 0, -8738, 0, 182, 1638, 0, -728, -5279, 0, -1274, -6735, 0, -910, -8738, 0, 0, 1092
};
static const int32_t data_25894[57] = {
  0, 0, -5461, 0, -2366, -1092, 0, -182, 1092, 0, -546, -3640, 0, 0, -2366, 0, 0, -1456, 0, -182, 1274, 0, 0, -3822, 0, 0, -2730, 0, 0, -1456, 0, 0,
  1456, 0, 182, -3822, 0, 0, -2912, 0, 0, -1638, 0, 182, 1638, 0, -728, -3640, 0, -910, -2730, 0, -546, -1638, 0, 0, 1092
};
static const int32_t data_25978[342] = {
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
static const char data_25ED0[5] __attribute__((scn_string_literal)) = "SY11";
static const char data_25ED5[9] __attribute__((scn_string_literal)) = "SY11A006";
static const char data_25EDE[9] __attribute__((scn_string_literal)) = "SY11A007";
static const char data_25EE7[9] __attribute__((scn_string_literal)) = "SY11A008";
static const char data_25EF0[9] __attribute__((scn_string_literal)) = "SY11A002";
static const char data_25EF9[9] __attribute__((scn_string_literal)) = "SY11A003";
static const char data_25F02[9] __attribute__((scn_string_literal)) = "SY11A004";
static const char data_25F0B[9] __attribute__((scn_string_literal)) = "SY11A005";
static const char data_25F14[9] __attribute__((scn_string_literal)) = "SY11A001";
static const char data_25F1D[9] __attribute__((scn_string_literal)) = "SY11A009";
static const char data_25F26[9] __attribute__((scn_string_literal)) = "SY11A010";
static const char data_25F2F[9] __attribute__((scn_string_literal)) = "SY11A011";
static const char data_25F38[9] __attribute__((scn_string_literal)) = "SY11A009";
static const char data_25F41[5] __attribute__((scn_string_literal)) = "SY11";
static const char data_25F46[5] __attribute__((scn_string_literal)) = "SY11";
static const char data_25F4B[9] __attribute__((scn_string_literal)) = "SY11A001";
static const char data_25F54[5] __attribute__((scn_string_literal)) = "SY11";
static const char data_25F59[5] __attribute__((scn_string_literal)) = "SY11";
static const char data_25F5E[9] __attribute__((scn_string_literal)) = "SY11A009";
static const char data_25F67[9] __attribute__((scn_string_literal)) = "SY11A010";
static const char data_25F70[9] __attribute__((scn_string_literal)) = "SY11A011";
static const char data_25F79[7] __attribute__((scn_string_literal)) = "SY11";
static const uint32_t data_25F80 = SHEN_NAME('VM_0');
static const shen_name data_25F84[7] = {
  SHEN_NAME('VM_1'), SHEN_NAME('VM_2'), SHEN_NAME('VM_3'), SHEN_NAME('VM_4'), SHEN_NAME('VM_5'), SHEN_NAME('VM_6'), SHEN_NAME('VM_7')
};
static const char data_25FA0[18] __attribute__((scn_string_literal)) = "/scene/03/stream/";
static const char data_25FB2[5] __attribute__((scn_string_literal)) = "%03d";
static const char data_25FB7[] __attribute__((scn_string_literal)) = "ATRK";
static Vector3 data_25FBC = {.x = 0.0f, .y = -0.1f, .z = 0.0f};
static Vector3 data_25FC8 = {.x = 0.0f, .y = 0.0f, .z = 0.0f};
static const char data_25FD4[] __attribute__((scn_string_literal)) = "VEND";
static const char data_25FD9[] __attribute__((scn_string_literal)) = "VEND";
static const char data_25FDE[] __attribute__((scn_string_literal)) = "SPRT";
static const char data_25FE3[7] __attribute__((scn_string_literal)) = "01JUCE";
static const char data_25FEA[13] __attribute__((scn_string_literal)) = "HMOT_CAN.BIN";
static const char data_25FF7[] __attribute__((scn_string_literal)) = "VEND";
static const char data_25FFC[7] __attribute__((scn_string_literal)) = "01JUCE";
static const char data_26003[22] __attribute__((scn_string_literal)) = "HandMot read by VEND\012";
static const char data_26019[13] __attribute__((scn_string_literal)) = "HMOT_CAN.BIN";
static const char data_26026[7] __attribute__((scn_string_literal)) = "01JUCE";
static const char data_2602D[] __attribute__((scn_string_literal)) = "VEND";
static const uint8_t data_26032[2] = {0};
static const uint8_t data_26034[12] = {0};
static const char data_26040[29] __attribute__((scn_string_literal)) = "vendmachine exit is EV_STOP\012";
static const char data_2605D[41] __attribute__((scn_string_literal)) = "vendmachine exit is MONEY_LOCK -> %d,%d\012";
static const char data_26086[] __attribute__((scn_string_literal)) = "COKE";
static const char data_2608B[] __attribute__((scn_string_literal)) = "FATG";
static const char data_26090[] __attribute__((scn_string_literal)) = "FATO";
static const char data_26095[] __attribute__((scn_string_literal)) = "SPRT";
static const char data_2609A[] __attribute__((scn_string_literal)) = "CAFE";
static const uint8_t data_2609F[1] = {0};
static const uint32_t data_260A0 = 0x3D204C06;
static const float data_260A4[2] = {0.0f, 0.890018f};
static const uint8_t data_260AC[12] = {0};
static const uint8_t data_260B8[12] = {0};
static const uint32_t data_260C4 = 0x3DE978D5;
static const float data_260C8[2] = {0.002f, -0.036f};
static const uint32_t data_260D0 = 8192;
static const int32_t data_260D4[2] = {11377, 3640};
static const uint32_t data_260DC = 0x3D204C06;
static const float data_260E0[2] = {0.0f, 0.890018f};
static const uint8_t data_260E8[12] = {0};
static const uint32_t data_260F4 = 4369;
static const int32_t data_260F8[2] = {0, -2184};
static const char data_26100[] __attribute__((scn_string_literal)) = "01JUCEA011";
static const char data_2610B[] __attribute__((scn_string_literal)) = "01JUCEA012";
static const char data_26116[] __attribute__((scn_string_literal)) = "01JUCEA013";
static const char data_26121[] __attribute__((scn_string_literal)) = "01JUCEA002";
static const char data_2612C[] __attribute__((scn_string_literal)) = "01JUCEA003";
static const uint8_t data_26137[1] = {0};
static const uint32_t data_26138 = 0x3DE978D5;
static const float data_2613C[2] = {0.002f, -0.036f};
static const uint32_t data_26144 = 8192;
static const int32_t data_26148[2] = {11377, 3640};
static const uint32_t data_26150 = 0x3DCB04AB;
static const float data_26154[2] = {0.02044f, -0.04322f};
static const uint32_t data_2615C = 0xFFFFFE69;
static const int32_t data_26160[2] = {3331, -3464};
static const uint32_t data_26168 = 0x3D204C06;
static const float data_2616C[2] = {0.0f, 0.890018f};
static const uint8_t data_26174[12] = {0};
static const uint32_t data_26180 = 4369;
static const int32_t data_26184[2] = {0, -2184};
static const uint8_t data_2618C[12] = {0};
static const char data_26198[] __attribute__((scn_string_literal)) = "01JUCEA002";
static const char data_261A3[] __attribute__((scn_string_literal)) = "01JUCEA003";
static const uint8_t pad_261AE[4] = {0};
static const uint8_t data_261B2[10] = {
  0x80, 0x3F, 0x00, 0x00, 0x80, 0x3F, 0x00, 0x00, 0x80, 0x3F
};
static DoorMotionSelectorBlock data_261BC[38] = {
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
static ScnDoorMotionDescriptor data_267AC[72] = {
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
static uint32_t data_271CC = 0xFFFFFFFF;
typedef struct Row_271D0 { float w0; float w1; float w2; float w3; int32_t w4; } Row_271D0;
static const Row_271D0 data_271D0[7] = {
  {0.0f, 0.0f, 0.0f, 0.0f, -1},
  {0.0f, 0.0f, 0.0f, 0.0f, -1},
  {0.0f, 0.0f, 0.0f, 0.0f, 1534},
  {26.0f, 55.0f, 45.0f, 75.0f, 1555},
  {61.0f, 146.0f, 45.0f, 50.0f, 1554},
  {59.0f, 124.0f, 45.0f, 70.0f, 1543},
  {33.0f, 49.0f, 45.0f, 50.0f, 1543}
};
static const float data_2725C[4] = {33.0f, 49.0f, 50.0f, 50.0f};
static void (*data_2726C)(int32_t, int32_t, int32_t) = script_6507;
static void (*const data_27270)(int32_t, int32_t, int32_t, int32_t) = script_6517;
static void (*const data_27274)(int32_t, int32_t, int32_t, int32_t) = script_6517;
static void (*const data_27278)(int32_t, int32_t, int32_t, int32_t) = script_6517;
static void (*const data_2727C)(int32_t, int32_t, int32_t, int32_t) = script_6517;
static void (*const data_27280)(int32_t, int32_t, int32_t, int32_t) = script_6517;
static void (*const data_27284)(int32_t, int32_t, int32_t, int32_t) = script_6517;
static void (*const data_27288)(int32_t, int32_t, int32_t, int32_t) = script_6517;
static void (*const data_2728C)(int32_t, int32_t, int32_t, int32_t) = script_6517;
static void (*const data_27290)(int32_t, int32_t, int32_t, int32_t) = script_67BD;
static void (*const data_27294)(int32_t, int32_t, int32_t, int32_t) = script_691E;
static void (*const data_27298)(int32_t, int32_t, int32_t, int32_t) = script_6DEB;
static void (*const data_2729C)(int32_t, int32_t, int32_t, int32_t) = script_6E52;
static void (*const data_272A0)(int32_t, int32_t, int32_t, int32_t) = script_661B;
static void (*const data_272A4)(int32_t, int32_t, int32_t, int32_t) = script_661B;
static void (*const data_272A8)(int32_t, int32_t, int32_t, int32_t) = script_6A7F;
static void (*const data_272AC)(int32_t, int32_t, int32_t, int32_t) = script_6D1D;
static void (*const data_272B0)(int32_t, int32_t, int32_t, int32_t) = script_6EB9;
static void (*const data_272B4)(int32_t, int32_t, int32_t, int32_t) = script_67BD;
static void (*const data_272B8)(int32_t, int32_t, int32_t, int32_t) = script_691E;
static void (*const data_272BC)(int32_t, int32_t, int32_t, int32_t) = script_6A7F;
static void (*const data_272C0)(int32_t, int32_t, int32_t, int32_t) = script_67BD;
static void (*const data_272C4)(int32_t, int32_t, int32_t, int32_t) = script_691E;
static void (*const data_272C8)(int32_t, int32_t, int32_t, int32_t) = script_6A7F;
static void (*const data_272CC)(int32_t, int32_t, int32_t, int32_t) = script_6F00;
static void (*const data_272D0)(int32_t, int32_t, int32_t, int32_t) = script_6F00;
static void (*const data_272D4)(int32_t, int32_t, int32_t, int32_t) = script_6F00;
static void (*const data_272D8)(int32_t, int32_t, int32_t, int32_t) = script_6F00;
static void (*const data_272DC)(int32_t, int32_t, int32_t, int32_t) = script_7036;
static void (*const data_272E0)(int32_t, int32_t, int32_t, int32_t) = script_78F7;
static void (*const data_272E4)(int32_t, int32_t, int32_t, int32_t) = script_6517;
static void (*const data_272E8)(int32_t, int32_t, int32_t, int32_t) = script_6517;
static void (*const data_272EC)(int32_t, int32_t, int32_t, int32_t) = script_6517;
static void (*const data_272F0)(int32_t, int32_t, int32_t, int32_t) = script_6517;
static void (*const data_272F4)(int32_t, int32_t, int32_t, int32_t) = script_6517;
static void (*const data_272F8)(int32_t, int32_t, int32_t, int32_t) = script_6517;
static void (*const data_272FC)(int32_t, int32_t, int32_t, int32_t) = script_6517;
static void (*const data_27300)(int32_t, int32_t, int32_t, int32_t) = script_6517;
static const char data_27304[46] __attribute__((scn_string_literal)) = "Door Index = %d : call_type = %d : type = %d\012";
static const char data_27332[43] __attribute__((scn_string_literal)) = "Door Open Motion = DEB_AKESIME_FUSUMA_L_F\012";
static const char data_2735D[43] __attribute__((scn_string_literal)) = "Door Open Motion = DEB_AKESIME_FUSUMA_R_F\012";
static const char data_27388[42] __attribute__((scn_string_literal)) = "Door Open Motion = DEB_HIKISIME_DOOR_L_F\012";
static const char data_273B2[42] __attribute__((scn_string_literal)) = "Door Open Motion = DEB_HIKISIME_DOOR_R_F\012";
static const char data_273DC[41] __attribute__((scn_string_literal)) = "Door Open Motion = DEB_OSISIME_DOOR_L_F\012";
static const char data_27405[41] __attribute__((scn_string_literal)) = "Door Open Motion = DEB_OSISIME_DOOR_R_F\012";
static const char data_2742E[43] __attribute__((scn_string_literal)) = "Door Open Motion = GAK_AKESIME_FUSUMA_L_F\012";
static const char data_27459[43] __attribute__((scn_string_literal)) = "Door Open Motion = GAK_AKESIME_FUSUMA_R_F\012";
static const char data_27484[42] __attribute__((scn_string_literal)) = "Door Open Motion = GAK_HIKISIME_DOOR_L_F\012";
static const char data_274AE[42] __attribute__((scn_string_literal)) = "Door Open Motion = GAK_HIKISIME_DOOR_R_F\012";
static const char data_274D8[41] __attribute__((scn_string_literal)) = "Door Open Motion = GAK_OSISIME_DOOR_L_F\012";
static const char data_27501[41] __attribute__((scn_string_literal)) = "Door Open Motion = GAK_OSISIME_DOOR_R_F\012";
static const char data_2752A[43] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_AKESIME_FUSUMA_L_F\012";
static const char data_27555[43] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_AKESIME_FUSUMA_R_F\012";
static const char data_27580[42] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_HIKISIME_DOOR_L_F\012";
static const char data_275AA[42] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_HIKISIME_DOOR_R_F\012";
static const char data_275D4[41] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_OSISIME_DOOR_L_F\012";
static const char data_275FD[41] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_OSISIME_DOOR_R_F\012";
static const char data_27626[43] __attribute__((scn_string_literal)) = "Door Open Motion = KOD_AKESIME_FUSUMA_L_F\012";
static const char data_27651[43] __attribute__((scn_string_literal)) = "Door Open Motion = KOD_AKESIME_FUSUMA_R_F\012";
static const char data_2767C[42] __attribute__((scn_string_literal)) = "Door Open Motion = KOD_HIKISIME_DOOR_L_F\012";
static const char data_276A6[42] __attribute__((scn_string_literal)) = "Door Open Motion = KOD_HIKISIME_DOOR_R_F\012";
static const char data_276D0[41] __attribute__((scn_string_literal)) = "Door Open Motion = KOD_OSISIME_DOOR_L_F\012";
static const char data_276F9[41] __attribute__((scn_string_literal)) = "Door Open Motion = KOD_OSISIME_DOOR_R_F\012";
static const char data_27722[43] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_AKESIME_FUSUMA_L_F\012";
static const char data_2774D[43] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_AKESIME_FUSUMA_R_F\012";
static const char data_27778[42] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_HIKISIME_DOOR_L_F\012";
static const char data_277A2[42] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_HIKISIME_DOOR_R_F\012";
static const char data_277CC[41] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_OSISIME_DOOR_L_F\012";
static const char data_277F5[41] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_OSISIME_DOOR_R_F\012";
static const char data_2781E[43] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_AKESIME_FUSUMA_L_F\012";
static const char data_27849[43] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_AKESIME_FUSUMA_R_F\012";
static const char data_27874[42] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_HIKISIME_DOOR_L_F\012";
static const char data_2789E[42] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_HIKISIME_DOOR_R_F\012";
static const char data_278C8[41] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_OSISIME_DOOR_L_F\012";
static const char data_278F1[41] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_OSISIME_DOOR_R_F\012";
static const char data_2791A[43] __attribute__((scn_string_literal)) = "Door Open Motion = SIN_AKESIME_FUSUMA_L_F\012";
static const char data_27945[43] __attribute__((scn_string_literal)) = "Door Open Motion = SIN_AKESIME_FUSUMA_R_F\012";
static const char data_27970[42] __attribute__((scn_string_literal)) = "Door Open Motion = SIN_HIKISIME_DOOR_L_F\012";
static const char data_2799A[42] __attribute__((scn_string_literal)) = "Door Open Motion = SIN_HIKISIME_DOOR_R_F\012";
static const char data_279C4[41] __attribute__((scn_string_literal)) = "Door Open Motion = SIN_OSISIME_DOOR_L_F\012";
static const char data_279ED[41] __attribute__((scn_string_literal)) = "Door Open Motion = SIN_OSISIME_DOOR_R_F\012";
static const char data_27A16[43] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_AKESIME_FUSUMA_L_F\012";
static const char data_27A41[43] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_AKESIME_FUSUMA_R_F\012";
static const char data_27A6C[42] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_HIKISIME_DOOR_L_F\012";
static const char data_27A96[42] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_HIKISIME_DOOR_R_F\012";
static const char data_27AC0[41] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_OSISIME_DOOR_L_F\012";
static const char data_27AE9[41] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_OSISIME_DOOR_R_F\012";
static const char data_27B12[43] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_AKESIME_FUSUMA_L_F\012";
static const char data_27B3D[43] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_AKESIME_FUSUMA_R_F\012";
static const char data_27B68[42] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_HIKISIME_DOOR_L_F\012";
static const char data_27B92[42] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_HIKISIME_DOOR_R_F\012";
static const char data_27BBC[41] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_OSISIME_DOOR_L_F\012";
static const char data_27BE5[41] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_OSISIME_DOOR_R_F\012";
static const char data_27C0E[44] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_OSISIME_DOOR_AP_L_F\012";
static const char data_27C3A[44] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_OSISIME_DOOR_AP_L_F\012";
static const char data_27C66[44] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_OSISIME_DOOR_AP_L_F\012";
static const char data_27C92[44] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_OSISIME_DOOR_AP_L_F\012";
static const char data_27CBE[44] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_OSISIME_DOOR_AP_L_F\012";
static const char data_27CEA[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_27D06[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_27D22[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_27D3E[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_27D5A[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_27D76[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_27D92[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_27DAE[13] __attribute__((scn_string_literal)) = "open-move!!\012";
static const char data_27DBB[11] __attribute__((scn_string_literal)) = "M_MDOR.BIN";
static const char data_27DC6[] __attribute__((scn_string_literal)) = "This Charactor Work is NULL!!\n";
static const char data_27DE5[22] __attribute__((scn_string_literal)) = "手動ドア開け番号指定\012";
static const char data_27DFB[34] __attribute__((scn_string_literal)) = "無限ループになりそうなので抜ける\012";
static const char data_27E1D[] __attribute__((scn_string_literal)) = "Don't make BOX Buffer!!\n";
static const char data_27E36[] __attribute__((scn_string_literal)) = "Don't make BOX Buffer!!\n";
static const char data_27E4F[15] __attribute__((scn_string_literal)) = "Door System : ";
static const char data_27E5E[15] __attribute__((scn_string_literal)) = "Door System : ";
static const char data_27E6D[18] __attribute__((scn_string_literal)) = "Door Box is ON!!\012";
static const char data_27E7F[19] __attribute__((scn_string_literal)) = "Door Box is OFF!!\012";
static const char data_27E92[18] __attribute__((scn_string_literal)) = "Door Box is ON!!\012";
static const char data_27EA4[20] __attribute__((scn_string_literal)) = "Door Box is OFF!!\012";
static const DSWKDoorEntry data_27EB8[28] = {
  {.slot = 0, .flags = 0, .ctrl_mask_pair = 0x640064U, .coarse_metric = 0.0f, .selection_metric = 0.0f, .view_ang_delta = 0, .group = 0, .global1 = 0xFFFFFFFFU, .global2 = 0xFFFFFFFFU, .global3 = 0xFFFFFFFFU, .sndCommandId = 0xFFFFFFFFU, .global5 = 0xFFFFFFFFU, .global6 = 0xFFFFFFFFU},
  {.slot = 2, .flags = 0, .ctrl_mask_pair = 0x640064U, .coarse_metric = 0.0f, .selection_metric = 0.0f, .view_ang_delta = 0, .group = 0, .global1 = 0xFFFFFFFFU, .global2 = 0xFFFFFFFFU, .global3 = 0xFFFFFFFFU, .sndCommandId = 0xFFFFFFFFU, .global5 = 0xFFFFFFFFU, .global6 = 0xFFFFFFFFU},
  {.slot = 5, .flags = 0, .ctrl_mask_pair = 0x640064U, .coarse_metric = 0.0f, .selection_metric = 0.0f, .view_ang_delta = 0, .group = 0, .global1 = 0xFFFFFFFFU, .global2 = 0xFFFFFFFFU, .global3 = 0xFFFFFFFFU, .sndCommandId = 0xFFFFFFFFU, .global5 = 0xFFFFFFFFU, .global6 = 0xFFFFFFFFU},
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
  {.slot = 80, .flags = 0, .ctrl_mask_pair = 0x640064U, .coarse_metric = 0.0f, .selection_metric = 0.0f, .view_ang_delta = 0, .group = 0, .global1 = 0xFFFFFFFFU, .global2 = 0xFFFFFFFFU, .global3 = 0xFFFFFFFFU, .sndCommandId = 0xFFFFFFFFU, .global5 = 0xFFFFFFFFU, .global6 = 0xFFFFFFFFU}
};
static const int32_t data_28468[13] = {-1, 0, 6553700, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1};
static const uint8_t data_2849C[4] = {0};
static const int32_t data_284A0[82] = {
  -1, 45, 46, -1, 7, 8, -1, 53, 54, -1, 17, 18, -1, 39, 40, -1, 51, 52, -1, 37, 38, -1, 43, 44, -1, 1, 2, -1, 41, 42, -1, 35, 36, -1, 19, 20, -1, 3,
  4, -1, 33, 34, -1, 13, 14, -1, 11, 12, -1, 9, 10, -1, 23, 24, -1, 25, 26, -1, 29, 30, -1, 27, 28, -1, 21, 22, -1, 31, 32, -1, 5, 6, -1, 47, 48, -1,
  15, 16, -1, 49, 50, -1
};
static char data_285E8[] = "DR15_024";
static char data_285F1[] = "DR00_010";
static char data_285FA[] = "DR15_020";
static char data_28603[] = "DR00_011";
static char data_2860C[] = "DR02_023";
static char data_28615[] = "DR15_021";
static char data_2861E[] = "DR02_022";
static char data_28627[] = "DR00_005";
static char data_28630[] = "DR01_012";
static char data_28639[] = "DR01_014";
static char data_28642[] = "DR15_018";
static char data_2864B[] = "DR15_019";
static char data_28654[] = "DR03_001";
static char data_2865D[] = "DR01_013";
static char data_28666[] = "DR01_013";
static const uint8_t data_2866F[1] = {0};
static char *const data_28670 = data_285E8;
static const int32_t data_28674[3] = {15, 524971, 590507};
static char *const data_28680 = data_285F1;
static const int32_t data_28684[3] = {0, -1, -1};
static char *const data_28690 = data_285FA;
static const int32_t data_28694[3] = {15, 524971, 590507};
static char *const data_286A0 = data_28603;
static const int32_t data_286A4[3] = {0, -1, -1};
static char *const data_286B0 = data_2860C;
static const int32_t data_286B4[3] = {2, 1311403, 1376939};
static char *const data_286C0 = data_28615;
static const int32_t data_286C4[3] = {15, 524971, 590507};
static char *const data_286D0 = data_2861E;
static const int32_t data_286D4[3] = {2, 1311403, 1376939};
static char *const data_286E0 = data_28627;
static const int32_t data_286E4[3] = {0, -1, -1};
static char *const data_286F0 = data_28630;
static const int32_t data_286F4[3] = {1, 1311403, 1376939};
static char *const data_28700 = data_28639;
static const int32_t data_28704[3] = {1, 1311403, 1376939};
static char *const data_28710 = data_28642;
static const int32_t data_28714[3] = {15, 524971, 590507};
static char *const data_28720 = data_2864B;
static const int32_t data_28724[3] = {15, 524971, 590507};
static char *const data_28730 = data_28654;
static const int32_t data_28734[3] = {3, 1311403, 1376939};
static char *const data_28740 = data_2865D;
static const int32_t data_28744[3] = {1, 1311403, 1376939};
static char *const data_28750 = data_28666;
static const int32_t data_28754[3] = {37, 1311403, 1376939};
static const MapObjectRecord data_28760[55] = {
  {.kind = 2, .resourceIndex = 0, .scale = {1.0f, 1.0f, 1.0f}, .pos = {14.7395f, 0.005667f, 20.42877f}, .yawFixed16 = -16383},
  {.kind = 1, .resourceIndex = 1, .scale = {2.0f, 1.0f, 1.0f}, .pos = {8.283677f, 0.0f, -15.969709f}, .yawFixed16 = 16383},
  {.kind = 2, .resourceIndex = 2, .scale = {1.0f, 1.0f, 1.0f}, .pos = {8.283677f, 0.0f, -15.969709f}, .yawFixed16 = 16383},
  {.kind = 1, .resourceIndex = 3, .scale = {2.0f, 1.0f, 1.0f}, .pos = {9.188771f, -0.00216f, 10.971309f}, .yawFixed16 = 16383},
  {.kind = 2, .resourceIndex = 4, .scale = {1.0f, 1.0f, 1.0f}, .pos = {9.203772f, -0.00216f, 10.521309f}, .yawFixed16 = 16383},
  {.kind = 1, .resourceIndex = 1, .scale = {2.0f, 1.0f, 1.0f}, .pos = {-18.219503f, 1.575149f, -3.487819f}, .yawFixed16 = -2293},
  {.kind = 2, .resourceIndex = 5, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-18.219503f, 1.568384f, -3.487819f}, .yawFixed16 = -2293},
  {.kind = 1, .resourceIndex = 3, .scale = {1.0f, 1.0f, 1.0f}, .pos = {8.810897f, 0.0f, 17.1300011f}, .yawFixed16 = 16383},
  {.kind = 2, .resourceIndex = 6, .scale = {1.0f, 1.0f, 1.0f}, .pos = {8.795897f, 0.0f, 16.68f}, .yawFixed16 = 16383},
  {.kind = 1, .resourceIndex = 7, .scale = {1.0f, 1.0f, 1.0f}, .pos = {15.276f, 0.678079f, 25.6500015f}, .yawFixed16 = -16383},
  {.kind = 2, .resourceIndex = 8, .scale = {1.0f, 1.0f, 1.0f}, .pos = {15.291f, 0.678079f, 25.2f}, .yawFixed16 = -16383},
  {.kind = 2, .resourceIndex = 8, .scale = {1.0f, 1.0f, 1.0f}, .pos = {4.500243f, 0.0f, -2.036989f}, .yawFixed16 = 0},
  {.kind = 1, .resourceIndex = 7, .scale = {1.0f, 1.0f, 1.0f}, .pos = {4.950243f, 0.0f, -2.021989f}, .yawFixed16 = 0},
  {.kind = 1, .resourceIndex = 3, .scale = {1.0f, 1.0f, 1.0f}, .pos = {21.865557f, 0.0f, 34.958485f}, .yawFixed16 = -9712},
  {.kind = 2, .resourceIndex = 9, .scale = {1.0f, 1.0f, 1.0f}, .pos = {21.608976f, 0.0f, 34.588501f}, .yawFixed16 = -9712},
  {.kind = 1, .resourceIndex = 1, .scale = {2.0f, 1.0f, 1.0f}, .pos = {14.4621f, 0.0f, 37.933716f}, .yawFixed16 = 25082},
  {.kind = 2, .resourceIndex = 2, .scale = {1.0f, 1.0f, 1.0f}, .pos = {14.4621f, 0.0f, 37.933716f}, .yawFixed16 = 25082},
  {.kind = 1, .resourceIndex = 1, .scale = {2.0f, 1.0f, 1.0f}, .pos = {27.233486f, 0.000244f, 9.113459f}, .yawFixed16 = 31311},
  {.kind = 2, .resourceIndex = 10, .scale = {1.0f, 1.0f, 1.0f}, .pos = {27.233486f, 0.0f, 9.113459f}, .yawFixed16 = 31311},
  {.kind = 1, .resourceIndex = 1, .scale = {2.0f, 1.0f, 1.0f}, .pos = {-4.3878f, 0.007f, 23.706007f}, .yawFixed16 = 32767},
  {.kind = 2, .resourceIndex = 11, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-4.3878f, 0.007f, 23.676006f}, .yawFixed16 = 32767},
  {.kind = 1, .resourceIndex = 1, .scale = {2.0f, 1.0f, 1.0f}, .pos = {8.20000267f, 0.007f, 26.098f}, .yawFixed16 = 16383},
  {.kind = 2, .resourceIndex = 10, .scale = {1.0f, 1.0f, 1.0f}, .pos = {8.20000267f, 0.007f, 26.098f}, .yawFixed16 = 16383},
  {.kind = 1, .resourceIndex = 1, .scale = {2.0f, 1.0f, 1.0f}, .pos = {-11.320493f, 0.007f, 2.094432f}, .yawFixed16 = -2628},
  {.kind = 2, .resourceIndex = 10, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-11.320493f, 0.007f, 2.094432f}, .yawFixed16 = -2628},
  {.kind = 1, .resourceIndex = 1, .scale = {2.0f, 1.0f, 1.0f}, .pos = {-18.04855f, 1.127f, 9.051468f}, .yawFixed16 = 17840},
  {.kind = 2, .resourceIndex = 11, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-18.04855f, 1.127f, 9.051468f}, .yawFixed16 = 17840},
  {.kind = 1, .resourceIndex = 3, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-26.424046f, 2.935309f, 9.891591f}, .yawFixed16 = 17048},
  {.kind = 2, .resourceIndex = 12, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-26.410368f, 2.935309f, 10.341632f}, .yawFixed16 = 17048},
  {.kind = 1, .resourceIndex = 3, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-31.450916f, 2.888575f, -3.344818f}, .yawFixed16 = -18386},
  {.kind = 2, .resourceIndex = 9, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-31.379776f, 2.888575f, -3.789412f}, .yawFixed16 = -18386},
  {.kind = 1, .resourceIndex = 1, .scale = {1.006f, 1.0f, 1.0f}, .pos = {-6.90468f, -0.00139f, 9.739539f}, .yawFixed16 = -20662},
  {.kind = 2, .resourceIndex = 4, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-7.070361f, -0.00139f, 10.158197f}, .yawFixed16 = -20662},
  {.kind = 2, .resourceIndex = 12, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-22.070604f, 3.831154f, 27.319414f}, .yawFixed16 = -2652},
  {.kind = 1, .resourceIndex = 7, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-21.638857f, 3.831154f, 27.447159f}, .yawFixed16 = -2652},
  {.kind = 1, .resourceIndex = 7, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-18.506819f, 1.135557f, 27.224298f}, .yawFixed16 = -2652},
  {.kind = 2, .resourceIndex = 12, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-18.938568f, 1.135557f, 27.096556f}, .yawFixed16 = -2652},
  {.kind = 2, .resourceIndex = 12, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-18.938568f, 3.831154f, 27.096561f}, .yawFixed16 = -2652},
  {.kind = 1, .resourceIndex = 7, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-18.506819f, 3.831154f, 27.224304f}, .yawFixed16 = -2652},
  {.kind = 2, .resourceIndex = 12, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-22.070637f, 1.135557f, 27.31953f}, .yawFixed16 = -2652},
  {.kind = 1, .resourceIndex = 7, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-21.638887f, 1.135557f, 27.447271f}, .yawFixed16 = -2652},
  {.kind = 2, .resourceIndex = 12, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-20.38096f, 3.831154f, 35.568211f}, .yawFixed16 = 104},
  {.kind = 1, .resourceIndex = 7, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-19.930832f, 3.831154f, 35.578712f}, .yawFixed16 = 104},
  {.kind = 2, .resourceIndex = 12, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-17.405649f, 3.831154f, 34.564453f}, .yawFixed16 = 104},
  {.kind = 1, .resourceIndex = 7, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-16.955523f, 3.831154f, 34.574955f}, .yawFixed16 = 104},
  {.kind = 2, .resourceIndex = 12, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-20.380859f, 1.135557f, 35.568317f}, .yawFixed16 = 104},
  {.kind = 1, .resourceIndex = 7, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-19.930733f, 1.135557f, 35.578815f}, .yawFixed16 = 104},
  {.kind = 2, .resourceIndex = 12, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-17.405649f, 1.135557f, 34.564445f}, .yawFixed16 = 104},
  {.kind = 1, .resourceIndex = 7, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-16.955523f, 1.135557f, 34.574947f}, .yawFixed16 = 104},
  {.kind = 1, .resourceIndex = 3, .scale = {1.0f, 1.0f, 1.0f}, .pos = {5.810672f, 0.214537f, 1.138362f}, .yawFixed16 = 15479},
  {.kind = 2, .resourceIndex = 14, .scale = {1.0f, 1.0f, 1.0f}, .pos = {5.756743f, 0.214537f, 1.585371f}, .yawFixed16 = 15479},
  {.kind = 1, .resourceIndex = 3, .scale = {1.0f, 1.0f, 1.0f}, .pos = {4.164679f, 0.214555f, 33.155029f}, .yawFixed16 = 16383},
  {.kind = 2, .resourceIndex = 14, .scale = {1.0f, 1.0f, 1.0f}, .pos = {4.149679f, 0.214555f, 33.60503f}, .yawFixed16 = 16383},
  {.kind = 1, .resourceIndex = 3, .scale = {2.0f, 1.0f, 1.0f}, .pos = {19.680346f, 0.000031f, 8.925988f}, .yawFixed16 = 16383},
  {.kind = 2, .resourceIndex = 9, .scale = {1.0f, 1.0f, 1.0f}, .pos = {19.695345f, -0.000183f, 8.925988f}, .yawFixed16 = 16383}
};
static const uint32_t data_28F1C[4] = {0x413042CU, 0x42F042DU, 0x4300432U, 0x3870431U};
static const char data_28F2C[20] __attribute__((scn_string_literal)) = "TUNED FreeTalk def\012";
static const char data_28F40[20] __attribute__((scn_string_literal)) = "TUNED FreeTalk def\012";
static const char data_28F54[11] __attribute__((scn_string_literal)) = "bgm109.snd";
static const char data_28F5F[9] __attribute__((scn_string_literal)) = "SELECT";
static const Vector3 data_28F68 = {.x = 10.5122f, .y = 3.1989f, .z = -13.5867f};
static const Vector3 data_28F74 = {.x = 9.67f, .y = 3.1546f, .z = 6.2737f};
static const Vector3 data_28F80 = {.x = -5.8791f, .y = 3.1546f, .z = 21.8745f};
static const Vector3i data_28F8C = {.x = 65172, .y = 12599, .z = 65172};
static const Vector3i data_28F98 = {.x = 0, .y = 27285, .z = 0};
static const Vector3i data_28FA4 = {.x = 0, .y = -26868, .z = 0};
static const int8_t data_28FB0[16] = {12, 17, 22, 10, 19, 18, 11, 8, 4, 4, 17, 7, 6, 5, 4, 4};
static const int8_t data_28FC0[16] = {12, 1, 2, 1, 3, 2, 2, 8, 4, 4, 17, 7, 6, 5, 4, 4};
static const uint32_t data_28FD0 = 0xA800A7;
static const int32_t data_28FD4[5] = {8847529, 8978568, 15073509, 15204583, 15335657};
static const uint32_t data_28FE8 = 0x440043;
static const int32_t data_28FEC[8] = {4587589, 4718663, 4849737, 4980811, 5111885, 5242959, 5374033, 85};
static const uint32_t data_2900C = 0x5C005B;
static const int32_t data_29010[10] = {6160477, 6291551, 6422625, 6553699, 6684773, 6815847, 6946921, 7077995, 7209069, 7340143};
static const uint32_t data_29038 = 0x740073;
static const int32_t data_2903C[4] = {7798902, 7929976, 8061050, 8257660};
static const uint32_t data_2904C = 0x720071;
static const int32_t data_29050[9] = {10223750, 10354845, 10485919, 10616993, 10748067, 10879141, 11206826, 5767255, 180};
static const uint32_t data_29074 = 0x810080;
static const int32_t data_29078[8] = {8585346, 11403437, 11534511, 11665585, 11862195, 11993270, 12452024, 8650879};
static const uint32_t data_29098 = 0xB900BC;
static const int32_t data_2909C[5] = {12255418, 13238473, 13500621, 12517579, 200};
static const uint32_t data_290B0 = 0x5A0059;
static const int32_t data_290B4[3] = {5636268, 14221445, 5505107};
static const uint32_t data_290C0 = 0x7D0075;
static const int32_t data_290C4 = 12386442;
static const uint32_t data_290C8 = 0xC600C5;
static const int32_t data_290CC = 13369543;
static const uint32_t data_290D0 = 0x8C008B;
static const int32_t data_290D4[8] = {9306253, 9437327, 9633937, 148, 9961623, 10092697, 9568411, 149};
static const uint32_t data_290F4 = 0xD700D6;
static const int32_t data_290F8[3] = {14287064, 14876891, 228};
static const uint32_t data_29104 = 0xDC00D3;
static const int32_t data_29108[2] = {13893845, 14549213};
static const uint32_t data_29110 = 0xC100C0;
static const int32_t data_29114[2] = {12779714, 196};
static const uint32_t data_2911C = 0xD000CF;
static const int32_t data_29120 = 13762769;
static const uint32_t data_29124 = 0xE000DF;
static const int32_t data_29128 = 14811361;
static const char data_2912C[169] = "%GACR016G.MT5\000%GACK66BG.MT5\000%GACK66RG.MT5\000%GACR0BNG.MT5\000%GACR0B2G.MT5\000%GACR0B3G.MT5\000%SPBK6REG.MT5\000%SPBK6BRG.MT5\000%SPBK6YEG.MT5\000%SAIK6WHG.MT5\000%SAIK6BKG.MT5\000%SAIK6REG.MT5";
static const char data_291D5[315] = "%GACIAK1G.MT5\000%GACIAK2G.MT5\000%GACOJK1G.MT5\000%GACOJK2G.MT5\000%GACOSA1G.MT5\000%GACOSA2G.MT5\000%GACOLA1G.MT5\000%GACOLA2G.MT5\000%GACOPA1G.MT5\000%GACOPA2G.MT5\000%GACGWF1G.MT5\000%GACGWF2G.MT5\000%GACGJF1G.MT5\000%GACGJF2G.MT5\000%GACIKA1G.MT5\000%GACIKA2G.MT5\000%GACM401G.MT5\000%GACIAA1G.MT5\000%GACIAA2G.MT5\000D\000D\000D\000D\000D\000D\000D\000D\000D\000D\000%GACGJJ1G.MT5\000%GACGJJ2G.MT5";
static const char data_29310[309] = "%GACIAKAG.MT5\000%GACIAKBG.MT5\000%GACGJKAG.MT5\000%GACGJKBG.MT5\000%GACOSAAG.MT5\000%GACOSABG.MT5\000%GACOLAAG.MT5\000%GACOLABG.MT5\000%GACOPAAG.MT5\000%GACOPABG.MT5\000%GACIWLAG.MT5\000%GACIWLBG.MT5\000%GACGJFAG.MT5\000%GACGJFBG.MT5\000%GACIKAAG.MT5\000%GACIKABG.MT5\000%GACGSNAG.MT5\000%GACGSNBG.MT5\000%GACILIAG.MT5\000%GACILIBG.MT5\000%GACIDUAG.MT5\000%GACIDUBG.MT5";
static const char data_29445[167] = "%GACRSO1G.MT5\000%GACRSO2G.MT5\000%GACRTL1G.MT5\000%GACRNUCG.MT5\000%GACRBAKG.MT5\000%GACRBING.MT5\000%GACRFNGG.MT5\000%GACRAMYG.MT5\000%GACRESPG.MT5\000%GACREGGG.MT5\000D\000D\000D\000D\000D\000D\000%GACRFNNG.MT5";
static const char data_294EC[365] = "%GACRNT1G.MT5\000%GACRNT2G.MT5\000%GACRLOPG.MT5\000%GACRMYUG.MT5\000%GACRCKNG.MT5\000%GACRALFG.MT5\000%GACRRISG.MT5\000%GACRARXG.MT5\000%GACR0OPG.MT5\000%GACR0O2G.MT5\000%GACRPOPG.MT5\000%GACRCOBG.MT5\000%GACRIDAG.MT5\000%GACRSHPG.MT5\000%GACRMOBG.MT5\000%GACRROBG.MT5\000%GACK6TZG.MT5\000%GACRPANG.MT5\000%GACK6DYG.MT5\000D\000D\000D\000D\000D\000D\000D\000%GACRAXXG.MT5\000D\000D\000D\000D\000D\000D\000D\000%GACRRBBG.MT5\000%GACK6TSG.MT5\000%GACRPPNG.MT5\000%GACK6DSG.MT5";
static const char data_29659[505] = "%GACK6HHG.MT5\000%GACK6HYG.MT5\000%GACK6HBG.MT5\000%GACK6HGG.MT5\000%GACH5BKG.MT5\000%KAB02FSG.MT5\000%GACO1K2G.MT5\000%GACO1K3G.MT5\000%GACO1K7G.MT5\000%GACO1K8G.MT5\000%GACO1K9G.MT5\000%GACO1K1G.MT5\000%GACO1K4G.MT5\000%GACO1K5G.MT5\000%GACO1K6G.MT5\000%GACS5BSG.MT5\000%GACK6HOG.MT5\000%GACK6HKG.MT5\000%GACK6XHG.MT5\000%GACK6XYG.MT5\000%GACK6XBG.MT5\000%GACK6XGG.MT5\000%GACK6BKG.MT5\000%KABK6FSG.MT5\000%GACOKK2G.MT5\000%GACOKK3G.MT5\000%GACOKK7G.MT5\000%GACOKK8G.MT5\000%GACOKK9G.MT5\000%GACOKK1G.MT5\000%GACOKK4G.MT5\000%GACOKK5G.MT5\000%GACOKK6G.MT5\000%GACS5BBG.MT5\000%GACK6XOG.MT5\000%GACK6XKG.MT5";
static const char data_29852[211] = "%GACH5TKG.MT5\000%GACGTRXG.MT5\000%GACO1T1G.MT5\000%GACO1T2G.MT5\000%GACO1KOG.MT5\000%GACO1KBG.MT5\000%GACO1KJG.MT5\000%GACH5UKG.MT5\000%GACH5IKG.MT5\000%GACO1FOG.MT5\000%GACGSHEG.MT5\000%GACH5TTG.MT5\000%GACGTXXG.MT5\000%GACOTT1G.MT5\000%GACOTT2G.MT5";
static const char data_29925[113] = "%YKW0201G.MT5\000%YWT0201G.MT5\000%GACRBONG.MT5\000%GACRRENG.MT5\000%GACK6SPG.MT5\000%GACG360G.MT5\000%GACRDR1G.MT5\000%GACRDR2G.MT5";
static const char data_29996[77] = "%GACRSSCG.MT5\000%GACRMETG.MT5\000%GACRGBIG.MT5\000%GACH5TRG.MT5\000D\000D\000D\000%GACH5RRG.MT5";
static const char data_299E3[57] = "%GACS5FRG.MT5\000%GACS5FBG.MT5\000%GACO1KUG.MT5\000%GACO1FEG.MT5";
static const char data_29A1C[239] = "%GACODG1G.MT5\000%GACODG2G.MT5\000%GACODG3G.MT5\000%GACOCT2G.MT5\000%GACOCATG.MT5\000%GACO1SHG.MT5\000%GACOKSHG.MT5\000%GACO1IRG.MT5\000%GACOKIRG.MT5\000%GACO1HAG.MT5\000%GACOHA2G.MT5\000%GACOHA3G.MT5\000%GACOHA4G.MT5\000%GACO1YGG.MT5\000%GACO1USG.MT5\000%GACOOSHG.MT5\000%GACOOIRG.MT5";
static const char data_29B0B[99] = "%GACM400G.MT5\000%GACGSPHG.MT5\000%GACS5HGG.MT5\000%GACGDT1G.MT5\000%GACS5EQG.MT5\000%SYOGSH0G.MT5\000%SYOGHG0G.MT5";
static const char data_29B6E[85] = "%GACGCRYG.MT5\000%GACGBIRG.MT5\000%GACH5SLG.MT5\000%GACH5JKG.MT5\000%SYOGMD1G.MT5\000%SYOGMD5G.MT5";
static const char data_29BC3[71] = "%GACS5N1G.MT5\000%GACS5N2G.MT5\000%GACS5N3G.MT5\000%GACS5N4G.MT5\000%GACS5N5G.MT5";
static const char data_29C0A[57] = "%GACGXBNG.MT5\000%GACGYBNG.MT5\000%GACGABNG.MT5\000%GACGBBNG.MT5";
static const char data_29C43[57] = "%GACRMEGG.MT5\000%GACR0GGG.MT5\000%GACRSATG.MT5\000%GACRCASG.MT5";
static const uint32_t data_29C7C = SHEN_NAME('GCH0');
static const shen_name data_29C80[5] = {SHEN_NAME('GCH1'), SHEN_NAME('GCH2'), SHEN_NAME('GCH3'), SHEN_NAME('GCH4'), SHEN_NAME('GCH5')};
static const uint32_t data_29C94 = SHEN_NAME('HGC0');
static const shen_name data_29C98[5] = {SHEN_NAME('HGC1'), SHEN_NAME('HGC2'), SHEN_NAME('HGC3'), SHEN_NAME('HGC4'), SHEN_NAME('HGC5')};
static const uint32_t data_29CAC = SHEN_NAME('GBX0');
static const shen_name data_29CB0[5] = {SHEN_NAME('GBX1'), SHEN_NAME('GBX2'), SHEN_NAME('GBX3'), SHEN_NAME('GBX4'), SHEN_NAME('GBX5')};
static const uint8_t data_29CC4[20] = {0};
typedef struct Row_29CD8 { float w0; int32_t w1; float w2; int32_t w3; int32_t w4; } Row_29CD8;
static const Row_29CD8 data_29CD8[5] = {
  {13.8f, 0, 22.65f, 49152, 3},
  {13.8f, 0, 22.0f, 49152, 257},
  {0.0f, 0, 0.0f, 0, 0},
  {0.0f, 0, 0.0f, 0, 0},
  {0.0f, 0, 0.0f, 0, 0}
};
static const char data_29D3C[16] __attribute__((scn_string_literal)) = "/scene/03/JD00/";
static const char data_29D4C[8] __attribute__((scn_string_literal)) = "GACH";
static const Vector3 data_29D54 = {.x = 0.29f, .y = 0.0f, .z = 0.088f};
static const Vector3i data_29D60 = {.x = 0, .y = 0, .z = 0};
static const char data_29D6C[6] __attribute__((scn_string_literal)) = "E1066";
static const char data_29D72[13] __attribute__((scn_string_literal)) = "e1gachap.snd";
static const char data_29D7F[16] __attribute__((scn_string_literal)) = "/scene/03/JD00/";
static const char data_29D8F[8] __attribute__((scn_string_literal)) = "HI_GACH";
static const char data_29D97[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_29D9C[6] __attribute__((scn_string_literal)) = "E1066";
static const char data_29DA2[8] __attribute__((scn_string_literal)) = "HI_GACH";
static const char data_29DAA[10] __attribute__((scn_string_literal)) = "HI_GACH";
static const Vector4 data_29DB4 = {.x = 0.0f, .y = 0.7f, .z = 0.7f, .w = 0.7f};
static const uint8_t data_29DC4[4] = {0};
static const int32_t data_29DC8[56] = {
  -546, -5870, 0, 2184, -3726, 0, 182, 0, 0, 0, -5184, 0, 71525, -12075, 0, -182, -5279, 0, -364, 910, 0, 1456, -2894, 0, 3458, -13352, 0, -910,
  -10327, 0, 0, 1820, 0, 0, -6863, 0, 1820, -16438, 0, -364, -10150, 0, 236, 291, 0, 0, -12105, 0, 0, -15539, 0, 0, -7249, 0, -819, -364
};
static const uint8_t data_29EA8[4] = {0};
static const int32_t data_29EAC[56] = {
  -546, -5870, 0, 2184, -3726, 0, 182, 0, 0, 0, -5184, 0, 71525, -10922, 0, -182, -5279, 0, -364, 910, 0, 1456, -2894, 0, 3458, -13352, 0, -910,
  -10327, 0, 0, 1820, 0, 0, -6863, 0, 1820, -16438, 0, -364, -10150, 0, 236, 291, 0, 0, -12105, 0, 0, -15539, 0, 0, -7249, 0, -819, -364
};
static const uint8_t data_29F8C[4] = {0};
static const int32_t data_29F90[56] = {
  2912, -7507, 0, 6189, -4454, 0, -1456, 0, 0, 546, -8266, 0, 1292, -7099, 0, 910, -11468, 0, 728, 3276, 0, 1092, -11450, 0, 546, -10256, 0, 1456,
  -6504, 0, 546, 1638, 0, -1092, -12142, 0, -364, -16438, 0, -364, -10150, 0, -309, 291, 0, 1638, -11923, 0, -3276, -17359, 0, -2912, -10707, 0, -819,
  -364
};
static const uint8_t data_2A070[4] = {0};
static const int32_t data_2A074[56] = {
  1638, -7507, 0, 6189, -4454, 0, -1456, 0, 0, 546, -17731, 0, -709, -9466, 0, 728, -15473, 0, 182, 2548, 0, 1092, -13271, 0, -1092, -14625, 0, 1638,
  -11965, 0, 0, 1638, 0, -1092, -12142, 0, -364, -16438, 0, -364, -10150, 0, -309, 291, 0, 1638, -11923, 0, -3276, -17359, 0, -2912, -10707, 0, -819,
  -364
};
static const uint8_t data_2A154[4] = {0};
static const int32_t data_2A158[56] = {
  1638, -6779, 0, 6189, -4454, 0, -1456, 0, 0, 546, -17731, 0, -709, -9466, 0, 1274, -15473, 0, 546, 2548, 0, 1092, -13271, 0, -1092, -14625, 0, 1638,
  -11965, 0, -910, 1638, 0, -1092, -12142, 0, -364, -16438, 0, -364, -10150, 0, -309, 291, 0, 1638, -12105, 0, -3276, -17359, 0, -2912, -10889, 0,
  -819, -364
};
static const uint8_t data_2A238[4] = {0};
static const int32_t data_2A23C[56] = {
  -2730, -9510, -3094, 4004, -1359, 0, 4004, -728, 0, 546, -6990, 0, 18, -4004, 2366, 1274, -11468, 0, 182, -182, 0, 1092, -13271, 0, -1092, -14443,
  0, 1638, -11965, 0, -910, 1638, 0, -1092, -12142, 0, -364, -16438, 0, -364, -10150, 0, -309, 291, 0, 1638, -12105, 0, -3276, -17359, 0, -2912,
  -10889, 0, -819, -364
};
static const uint8_t data_2A31C[8] = {0};
static const int32_t data_2A324[55] = {
  -6005, 0, 4731, -6188, 0, 0, 0, 0, 0, -2727, 0, 0, -9097, 0, 0, -5457, 0, 0, 0, 0, 0, -8734, 0, -909, -7277, 0, 0, -7823, 0, 0, 0, 0, 1456, -4002,
  0, 0, -9462, 0, 726, -7824, 0, 0, 0, 0, 0, -12011, 0, -1091, -7461, 0, -1636, -9280, 0, 0, 0
};
static const uint8_t data_2A400[4] = {0};
static const int32_t data_2A404[56] = {
  -546, -5870, 0, 2184, -3726, 0, 182, 0, 0, 0, -5184, 0, 71525, -12075, 0, -182, -5279, 0, -364, 910, 0, 1456, -2894, 0, 3458, -13352, 0, -910,
  -10327, 0, 0, 1820, 0, 0, -6863, 0, 1820, -16438, 0, -364, -10150, 0, 236, 291, 0, 0, -12105, 0, 0, -15539, 0, 0, -7249, 0, -819, -364
};
static const uint8_t data_2A4E4[8] = {0};
static const int32_t data_2A4EC[55] = {
  -8046, 0, 0, -1820, -13088, 4350, -2894, 0, 0, -9011, 0, 0, -18422, 0, 0, -9812, 0, 0, 0, 0, 0, -7518, 0, 0, -17385, 0, 0, -11559, 0, 0, 0, 0, 0,
  -5734, 0, 0, -18058, 0, 0, -12233, 0, 0, 0, 0, 0, -7827, 0, 0, -16183, 0, 0, -13981, 0, -72, 0
};
static const Vector3 data_2A5C8 = {.x = 0.067f, .y = 0.061f, .z = 0.0379f};
static const uint32_t data_2A5D4 = 0x3DB851EC;
static const float data_2A5D8[2] = {0.075f, 0.008f};
static const Vector3i data_2A5E0 = {.x = 13111, .y = 817, .z = 50684};
static const Vector3 data_2A5EC = {.x = 0.091f, .y = -0.087f, .z = 0.0671f};
static const Vector3i data_2A5F8 = {.x = 20024, .y = 65353, .z = 11832};
static const Vector3 data_2A604 = {.x = 0.0f, .y = 0.0f, .z = 0.0f};
static const Vector3i data_2A610 = {.x = 0, .y = 0, .z = 0};
static const Vector3 data_2A61C = {.x = -0.042f, .y = 0.1735f, .z = 0.2062f};
static const char data_2A628[] __attribute__((scn_string_literal)) = "E1066A001";
static const char data_2A632[] __attribute__((scn_string_literal)) = "E1066A002";
static const char data_2A63C[] __attribute__((scn_string_literal)) = "E1066A003";
static const char data_2A646[] __attribute__((scn_string_literal)) = "E1066A004";
static const char data_2A650[] __attribute__((scn_string_literal)) = "E1066A005";
static const char data_2A65A[] __attribute__((scn_string_literal)) = "E1066A006";
static const char data_2A664[] __attribute__((scn_string_literal)) = "E1066A007";
static const char data_2A66E[] __attribute__((scn_string_literal)) = "E1066A008";
static const char data_2A678[] __attribute__((scn_string_literal)) = "E1066A009";
static const char data_2A682[] __attribute__((scn_string_literal)) = "E1066A010";
static const char data_2A68C[] __attribute__((scn_string_literal)) = "E1066A011";
static const char data_2A696[] __attribute__((scn_string_literal)) = "E1066A026";
static const char data_2A6A0[] __attribute__((scn_string_literal)) = "E1066A027";
static const char data_2A6AA[] __attribute__((scn_string_literal)) = "E1066A019";
static const char data_2A6B4[] __attribute__((scn_string_literal)) = "E1066A020";
static const char data_2A6BE[] __attribute__((scn_string_literal)) = "E1066A021";
static const char data_2A6C8[] __attribute__((scn_string_literal)) = "E1066A014";
static const char data_2A6D2[] __attribute__((scn_string_literal)) = "E1066A015";
static const char data_2A6DC[] __attribute__((scn_string_literal)) = "E1066A016";
static const char data_2A6E6[] __attribute__((scn_string_literal)) = "E1066A017";
static const char data_2A6F0[] __attribute__((scn_string_literal)) = "E1066A018";
static const char data_2A6FA[] __attribute__((scn_string_literal)) = "E1066A022";
static const char data_2A704[] __attribute__((scn_string_literal)) = "E1066A025";
static const char data_2A70E[] __attribute__((scn_string_literal)) = "E1066A024";
static const char data_2A718[12] __attribute__((scn_string_literal)) = "E1066A023";
static const Vector3 data_2A724 = {.x = 0.337795f, .y = 0.596267f, .z = -0.181433f};
static const Vector3 data_2A730 = {.x = 0.091f, .y = -0.087f, .z = 0.0671f};
static const Vector3i data_2A73C = {.x = 20024, .y = 65353, .z = 11832};
static const char data_2A748[12] __attribute__((scn_string_literal)) = "JINGURU01";
static const uint8_t data_2A754[28] = {0};
static const int32_t data_2A770[7] = {-1, 0, 0, 0, 0, 0, 0};
static char data_2A78C[] = "$B14A0M1G.MT5";
static const uint8_t data_2A79A[2] = {0};
static const uint32_t data_2A79C = 0xFFFF0000;
static const uint32_t data_2A7A0[2] = {0, 0x80000000U};
static const float data_2A7A8[5] = {15.0673f, 1.0992f, 2.0482f, 0.0f, 0.0f};
static const int32_t data_2A7BC[2] = {-65536, 101};
static const uint32_t data_2A7C4 = 0x71C74000U;
static const float data_2A7C8[5] = {9.5827f, 1.3422f, 1.3926f, 0.0f, 0.0f};
static const int32_t data_2A7DC[2] = {-65536, 102};
static const uint32_t data_2A7E4 = 0x71C74000U;
static const float data_2A7E8[5] = {9.5219f, 1.635f, 32.3657f, 0.0f, 0.0f};
static const int32_t data_2A7FC[2] = {-65536, 103};
static const uint32_t data_2A804 = 0x71C77C71U;
static const float data_2A808[5] = {28.2411f, 1.696f, 9.228f, 0.0f, 0.0f};
static const int32_t data_2A81C[2] = {-65536, 104};
static const uint32_t data_2A824[2] = {0x71C78000U, 0x41A56E2FU};
static const float data_2A82C[4] = {1.4047f, 6.9876f, 0.0f, 0.0f};
static const int32_t data_2A83C[2] = {-65536, 106};
static const uint32_t data_2A844 = 0x71C74000U;
static const float data_2A848[5] = {9.5741f, 1.5472f, -1.9451f, 0.0f, 0.0f};
static const int32_t data_2A85C[2] = {-65536, 107};
static const uint32_t data_2A864 = 0x71C74000U;
static const float data_2A868[5] = {9.4969f, 1.25f, 12.0913f, 0.0f, 0.0f};
static const int32_t data_2A87C[2] = {-65536, 109};
static const uint32_t data_2A884 = 0x71C74000U;
static const float data_2A888[5] = {8.1858f, 1.7775f, 27.101f, 0.0f, 0.0f};
static const int32_t data_2A89C[2] = {-65536, 110};
static const uint32_t data_2A8A4 = 0x71C7671CU;
static const float data_2A8A8[2] = {14.3068f, 1.5725f};
static const uint32_t data_2A8B0[3] = {0x420BDC43U, 0, 0};
static const int32_t data_2A8BC[2] = {-65536, 111};
static const uint32_t data_2A8C4 = 0x71C7E38EU;
static const float data_2A8C8[2] = {20.4664f, 1.6303f};
static const uint32_t data_2A8D0[3] = {0x420D7909U, 0, 0};
static const int32_t data_2A8DC[2] = {-65536, 112};
static const uint32_t data_2A8E4 = 0x71C70000U;
static const float data_2A8E8[5] = {13.6589f, 2.0333f, 27.6413f, 0.0f, 0.0f};
static const int32_t data_2A8FC[2] = {-65536, 116};
static const uint32_t data_2A904[2] = {0x71C7F60BU, 0xC20B4275U};
static const float data_2A90C[4] = {4.2982f, -2.5379f, 0.0f, 0.0f};
static const int32_t data_2A91C[2] = {-65536, 117};
static const uint32_t data_2A924[2] = {0x71C78000U, 0xC1C9F803U};
static const float data_2A92C[4] = {2.5835f, 4.8512f, 0.0f, 0.0f};
static const int32_t data_2A93C[2] = {-65536, 118};
static const uint32_t data_2A944 = 0x71C74000U;
static const float data_2A948[5] = {-14.4027f, 2.0075f, 6.531f, 0.0f, 0.0f};
static const int32_t data_2A95C[2] = {-65536, 114};
static const uint32_t data_2A964 = 0x71C7B1C7U;
static const float data_2A968[5] = {-8.3123f, 1.1153f, 9.8865f, 0.0f, 0.0f};
static const int32_t data_2A97C[2] = {-65536, 113};
static const uint32_t data_2A984 = 0x71C78000U;
static const float data_2A988[2] = {-3.5656f, 1.14f};
static const uint32_t data_2A990[3] = {0x41B453C3U, 0, 0};
static const int32_t data_2A99C[2] = {-65535, 123};
static const uint32_t data_2A9A4[2] = {0x71C7F555U, 0xC18F6F00U};
static const float data_2A9AC[4] = {3.139f, 27.362f, 0.0f, 0.0f};
static const int32_t data_2A9BC[2] = {-65535, 124};
static const uint32_t data_2A9C4[2] = {0x71C7F555U, 0xC1A8A95FU};
static const float data_2A9CC = 3.1365f;
static const uint32_t data_2A9D0[3] = {0x41DCBDD9U, 0, 0};
static const int32_t data_2A9DC[2] = {-65534, 125};
static const uint32_t data_2A9E4[2] = {0x71C7F555U, 0xC18F6F00U};
static const float data_2A9EC = 5.8396f;
static const uint32_t data_2A9F0[3] = {0x41DB020CU, 0, 0};
static const int32_t data_2A9FC[2] = {-65534, 126};
static const uint32_t data_2AA04[2] = {0x71C7F555U, 0xC1A8A95FU};
static const float data_2AA0C = 5.8396f;
static const uint32_t data_2AA10[3] = {0x41DCC04FU, 0, 0};
static const int32_t data_2AA1C[2] = {-65535, 119};
static const uint32_t data_2AA24[2] = {0x71C70000U, 0xC182E595U};
static const float data_2AA2C = 3.1365f;
static const uint32_t data_2AA30[3] = {0x420A4CCDU, 0, 0};
static const int32_t data_2AA3C[2] = {-65535, 120};
static const uint32_t data_2AA44[2] = {0x71C70000U, 0xC19ACE07U};
static const float data_2AA4C[4] = {3.1365f, 35.5826f, 0.0f, 0.0f};
static const int32_t data_2AA5C[2] = {-65534, 121};
static const uint32_t data_2AA64[2] = {0x71C70000U, 0xC182DD64U};
static const float data_2AA6C = 5.8365f;
static const uint32_t data_2AA70[3] = {0x420A578DU, 0, 0};
static const int32_t data_2AA7C[2] = {-65534, 122};
static const uint32_t data_2AA84 = 0x71C70000U;
static const float data_2AA88[2] = {-19.3489f, 5.8365f};
static const uint32_t data_2AA90[3] = {0x420E5C78U, 0, 0};
static const int32_t data_2AA9C[2] = {-65536, 112};
static const uint32_t data_2AAA4 = 0x71C7F1C7U;
static const float data_2AAA8[5] = {-10.4958f, 1.2717f, 3.1206f, 0.0f, 0.0f};
static const int32_t data_2AABC[2] = {-65536, 108};
static const uint32_t data_2AAC4 = 0x71C74000U;
static const float data_2AAC8[5] = {9.493f, 1.2451f, 16.1099f, 0.0f, 0.0f};
static const int32_t data_2AADC[2] = {-65536, 105};
static const uint32_t data_2AAE4 = 0x71C70000U;
static const float data_2AAE8[5] = {8.9036f, 1.55f, -16.9252f, 0.0f, 0.0f};
static const int32_t data_2AAFC[2] = {-65536, 115};
static const uint32_t data_2AB04[2] = {0x71C7F555U, 0xC18881A3U};
static const float data_2AB0C[4] = {2.971f, -3.2252f, 0.0f, 0.0f};
static const int32_t data_2AB1C[2] = {-65536, 131};
static const uint32_t data_2AB24 = 0x71C7E0B6U;
static const float data_2AB28[5] = {19.1329f, 1.23f, 34.5327f, 0.0f, 0.0f};
static const int32_t data_2AB3C[2] = {-65536, 131};
static const uint32_t data_2AB44 = 0x71C70000U;
static const float data_2AB48[5] = {2.1828f, 1.2508f, 18.9f, 0.0f, 0.0f};
static const int32_t data_2AB5C[2] = {-65536, 132};
static const uint32_t data_2AB64 = 0x71C7CFA4U;
static const float data_2AB68[5] = {-6.6538f, 1.2028f, 4.0641f, 0.0f, 0.0f};
static const int32_t data_2AB7C[2] = {-65536, 133};
static const uint32_t data_2AB84 = 0x71C7EAAAU;
static const float data_2AB88[5] = {13.7469f, 1.2147f, -8.3833f, 0.0f, 0.0f};
static const int32_t data_2AB9C[2] = {-65536, 135};
static const uint32_t data_2ABA4 = 0x71C7B8E3U;
static const float data_2ABA8[5] = {13.8298f, 1.2958f, 23.5861f, 0.0f, 0.0f};
static const int32_t data_2ABBC[2] = {-65536, 136};
static const uint32_t data_2ABC4 = 0x71C74000U;
static const float data_2ABC8[5] = {9.5274f, 1.4023f, 32.3564f, 0.0f, 0.0f};
static const int32_t data_2ABDC[2] = {-65536, 137};
static const uint32_t data_2ABE4 = 0x71C7C000U;
static const float data_2ABE8[2] = {14.7023f, 1.475f};
static const uint32_t data_2ABF0[4] = {0x41ADA29CU, 0, 0, 0};
static const shen_name data_2AC00 = SHEN_NAME('GCH0');
static const float data_2AC04[7] = {40128.0f, 0.0f, 0.76f, 0.055f, 0.0f, 0.0f, 0.0f};
static const shen_name data_2AC20 = SHEN_NAME('GCH1');
static const float data_2AC24[7] = {40128.0f, 0.0f, 0.76f, 0.055f, 0.0f, 0.0f, 0.0f};
static const shen_name data_2AC40 = SHEN_NAME('GCH2');
static const float data_2AC44[7] = {40128.0f, 0.0f, 0.76f, 0.055f, 0.0f, 0.0f, 0.0f};
static const shen_name data_2AC60 = SHEN_NAME('GCH3');
static const float data_2AC64[7] = {40128.0f, 0.0f, 0.76f, 0.055f, 0.0f, 0.0f, 0.0f};
static const shen_name data_2AC80 = SHEN_NAME('GCH4');
static const float data_2AC84[7] = {40128.0f, 0.0f, 0.76f, 0.055f, 0.0f, 0.0f, 0.0f};
static const shen_name data_2ACA0 = SHEN_NAME('GCH5');
static const float data_2ACA4[6] = {40128.0f, 0.0f, 0.76f, 0.055f, 0.0f, 0.0f};
static const int32_t data_2ACBC = 65535;
static uint32_t data_2ACC0 = 0x417113A9;
static const float data_2ACC4[2] = {1.1f, 2.0982f};
static const char data_2ACCC[6] __attribute__((scn_string_literal)) = "E1040";
static const char data_2ACD2[10] __attribute__((scn_string_literal)) = "E1040A028";
static const char data_2ACDC[10] __attribute__((scn_string_literal)) = "E1040A029";
static const char data_2ACE6[6] __attribute__((scn_string_literal)) = "E1040";
static const char data_2ACEC[10] __attribute__((scn_string_literal)) = "E1040A030";
static const char data_2ACF6[10] __attribute__((scn_string_literal)) = "E1040A031";
static const char data_2AD00[10] __attribute__((scn_string_literal)) = "E1040A032";
static const char data_2AD0A[6] __attribute__((scn_string_literal)) = "E1040";
static const Vector3 data_2AD10 = {.x = 21.6f, .y = 0.75f, .z = -6.18f};
static const char data_2AD1C[13] __attribute__((scn_string_literal)) = "e1shugyo.snd";
static const char data_2AD29[10] __attribute__((scn_string_literal)) = "E1040A001";
static const char data_2AD33[10] __attribute__((scn_string_literal)) = "E1040A003";
static const char data_2AD3D[10] __attribute__((scn_string_literal)) = "E1040A009";
static const char data_2AD47[10] __attribute__((scn_string_literal)) = "E1040A010";
static const char data_2AD51[10] __attribute__((scn_string_literal)) = "E1040A012";
static const char data_2AD5B[10] __attribute__((scn_string_literal)) = "E1040A013";
static const char data_2AD65[10] __attribute__((scn_string_literal)) = "E1040A014";
static const char data_2AD6F[10] __attribute__((scn_string_literal)) = "E1040A018";
static const char data_2AD79[10] __attribute__((scn_string_literal)) = "E1040A020";
static const char data_2AD83[10] __attribute__((scn_string_literal)) = "E1040A015";
static const char data_2AD8D[10] __attribute__((scn_string_literal)) = "E1040A016";
static const char data_2AD97[10] __attribute__((scn_string_literal)) = "E1040A017";
static const char data_2ADA1[6] __attribute__((scn_string_literal)) = "E1040";
static const char data_2ADA7[9] __attribute__((scn_string_literal)) = "E1040";
static const Vector3 data_2ADB0 = {.x = 19.9f, .y = 0.0f, .z = -8.2f};
static const Vector3 data_2ADBC = {.x = 23.28f, .y = 0.75f, .z = -9.38f};
static const char data_2ADC8[16] __attribute__((scn_string_literal)) = "\012 itou ga iru\012\012";
static const char data_2ADD8[18] __attribute__((scn_string_literal)) = "\012 mayumi ga iru\012\012";
static const char data_2ADEA[22] __attribute__((scn_string_literal)) = "\012 yamagisi ga iru\012\012";
static const int32_t data_2AE00[57] = {
  0, 0, -8046, 0, 0, -1820, -13088, 4350, -2894, 0, 0, -9011, 0, 0, -18422, 0, 0, -9812, 0, 0, 0, 0, 0, -7518, 0, 0, -17385, 0, 0, -11559, 0, 0, 0, 0,
  0, -5734, 0, 0, -18058, 0, 0, -12233, 0, 0, 0, 0, 0, -7827, 0, 0, -16183, 0, 0, -13981, 0, -72, 0
};
static const int32_t data_2AEE4[57] = {
  0, 0, -2712, 0, 0, -691, 0, 0, 0, 0, 0, -1092, 0, 0, -1438, 0, 0, -2748, 0, 0, 0, 0, 0, 364, 0, 0, -4132, 0, 0, -3531, 0, 0, 0, 0, 0, -2020, 0, 0,
  -6881, 0, 0, -5333, 0, 109, -200, 0, 0, -3986, 0, 0, -8829, 0, 0, -7245, 0, -764, -837
};
static const uint32_t data_2AFC8 = 0x303131;
static const uint32_t data_2AFCC = 0x393131;
static const uint32_t data_2AFD0 = 0x373731;
static const uint32_t data_2AFD4 = 0x343031;
static const char data_2AFD8[] __attribute__((scn_string_literal)) = "61564";
static const char data_2AFDE[] __attribute__((scn_string_literal)) = "37742";
static const char data_2AFE4[] __attribute__((scn_string_literal)) = "37749";
static const char data_2AFEA[] __attribute__((scn_string_literal)) = "22550";
static const char data_2AFF0[] __attribute__((scn_string_literal)) = "046861564";
static const char data_2AFFA[] __attribute__((scn_string_literal)) = "046837742";
static const char data_2B004[] __attribute__((scn_string_literal)) = "046837749";
static const char data_2B00E[] __attribute__((scn_string_literal)) = "046822550";
static const char data_2B018[] __attribute__((scn_string_literal)) = "2?????";
static const char data_2B01F[] __attribute__((scn_string_literal)) = "5?????";
static const char data_2B026[] __attribute__((scn_string_literal)) = "7?????";
static const char data_2B02D[] __attribute__((scn_string_literal)) = "8?????";
static const char data_2B034[] __attribute__((scn_string_literal)) = "3?????";
static const char data_2B03B[] __attribute__((scn_string_literal)) = "4?????";
static const char data_2B042[] __attribute__((scn_string_literal)) = "6?????";
static const char data_2B049[] __attribute__((scn_string_literal)) = "9?????";
static const char data_2B050[] __attribute__((scn_string_literal)) = "0?????????";
static const char data_2B05B[] __attribute__((scn_string_literal)) = "1?????";
static const char data_2B062[6] __attribute__((scn_string_literal)) = "E1066";
static const char data_2B068[10] __attribute__((scn_string_literal)) = "E1066A012";
static const char data_2B072[10] __attribute__((scn_string_literal)) = "E1066A013";
static const char data_2B07C[10] __attribute__((scn_string_literal)) = "E1066A006";
static const char data_2B086[6] __attribute__((scn_string_literal)) = "E1066";
static const char data_2B08C[12] __attribute__((scn_string_literal)) = "メモ帳消去\012";
static const char data_2B098[6] __attribute__((scn_string_literal)) = "E1032";
static const char data_2B09E[10] __attribute__((scn_string_literal)) = "E1032A001";
static const char data_2B0A8[10] __attribute__((scn_string_literal)) = "E1032A002";
static const char data_2B0B2[10] __attribute__((scn_string_literal)) = "E1032A003";
static const char data_2B0BC[10] __attribute__((scn_string_literal)) = "E1032A004";
static const char data_2B0C6[10] __attribute__((scn_string_literal)) = "E1032A005";
static const char data_2B0D0[6] __attribute__((scn_string_literal)) = "E1032";
static const char data_2B0D6[6] __attribute__((scn_string_literal)) = "E1032";
static const char data_2B0DC[10] __attribute__((scn_string_literal)) = "E1032A061";
static const char data_2B0E6[10] __attribute__((scn_string_literal)) = "E1032A062";
static const char data_2B0F0[10] __attribute__((scn_string_literal)) = "E1032A063";
static const char data_2B0FA[10] __attribute__((scn_string_literal)) = "E1032A064";
static const char data_2B104[6] __attribute__((scn_string_literal)) = "E1032";
static const char data_2B10A[6] __attribute__((scn_string_literal)) = "E1032";
static const char data_2B110[6] __attribute__((scn_string_literal)) = "E1032";
static const char data_2B116[6] __attribute__((scn_string_literal)) = "E1032";
static const char data_2B11C[10] __attribute__((scn_string_literal)) = "E1032A031";
static const char data_2B126[10] __attribute__((scn_string_literal)) = "E1032A029";
static const char data_2B130[10] __attribute__((scn_string_literal)) = "E1032A032";
static const char data_2B13A[10] __attribute__((scn_string_literal)) = "E1032A033";
static const char data_2B144[6] __attribute__((scn_string_literal)) = "E1032";
static const char data_2B14A[10] __attribute__((scn_string_literal)) = "E1032A028";
static const char data_2B154[10] __attribute__((scn_string_literal)) = "E1032A029";
static const char data_2B15E[10] __attribute__((scn_string_literal)) = "E1032A030";
static const char data_2B168[6] __attribute__((scn_string_literal)) = "E1032";
static const char data_2B16E[10] __attribute__((scn_string_literal)) = "E1032G001";
static const char data_2B178[10] __attribute__((scn_string_literal)) = "E1032G002";
static const char data_2B182[6] __attribute__((scn_string_literal)) = "E1032";
static const char data_2B188[10] __attribute__((scn_string_literal)) = "E1032I001";
static const char data_2B192[10] __attribute__((scn_string_literal)) = "E1032A052";
static const char data_2B19C[10] __attribute__((scn_string_literal)) = "E1032I007";
static const char data_2B1A6[10] __attribute__((scn_string_literal)) = "E1032I008";
static const char data_2B1B0[10] __attribute__((scn_string_literal)) = "E1032A097";
static const char data_2B1BA[10] __attribute__((scn_string_literal)) = "E1032I009";
static const char data_2B1C4[10] __attribute__((scn_string_literal)) = "E1032I010";
static const char data_2B1CE[10] __attribute__((scn_string_literal)) = "E1032A054";
static const char data_2B1D8[10] __attribute__((scn_string_literal)) = "E1032I011";
static const char data_2B1E2[10] __attribute__((scn_string_literal)) = "E1032A055";
static const char data_2B1EC[10] __attribute__((scn_string_literal)) = "E1032I012";
static const char data_2B1F6[10] __attribute__((scn_string_literal)) = "E1032A056";
static const char data_2B200[10] __attribute__((scn_string_literal)) = "E1032I013";
static const char data_2B20A[10] __attribute__((scn_string_literal)) = "E1032A057";
static const char data_2B214[10] __attribute__((scn_string_literal)) = "E1032I011";
static const char data_2B21E[10] __attribute__((scn_string_literal)) = "E1032A059";
static const char data_2B228[10] __attribute__((scn_string_literal)) = "E1032I009";
static const char data_2B232[10] __attribute__((scn_string_literal)) = "E1032I010";
static const char data_2B23C[10] __attribute__((scn_string_literal)) = "E1032A054";
static const char data_2B246[10] __attribute__((scn_string_literal)) = "E1032I011";
static const char data_2B250[10] __attribute__((scn_string_literal)) = "E1032A060";
static const char data_2B25A[10] __attribute__((scn_string_literal)) = "E1032I009";
static const char data_2B264[10] __attribute__((scn_string_literal)) = "E1032I010";
static const char data_2B26E[10] __attribute__((scn_string_literal)) = "E1032A054";
static const char data_2B278[10] __attribute__((scn_string_literal)) = "E1032I011";
static const char data_2B282[10] __attribute__((scn_string_literal)) = "E1032I001";
static const char data_2B28C[10] __attribute__((scn_string_literal)) = "E1032A007";
static const char data_2B296[10] __attribute__((scn_string_literal)) = "E1032A006";
static const char data_2B2A0[10] __attribute__((scn_string_literal)) = "E1032I002";
static const char data_2B2AA[10] __attribute__((scn_string_literal)) = "E1032I003";
static const char data_2B2B4[10] __attribute__((scn_string_literal)) = "E1032I004";
static const char data_2B2BE[10] __attribute__((scn_string_literal)) = "E1032I005";
static const char data_2B2C8[10] __attribute__((scn_string_literal)) = "E1032I006";
static const char data_2B2D2[10] __attribute__((scn_string_literal)) = "E1032A008";
static const char data_2B2DC[10] __attribute__((scn_string_literal)) = "E1032A009";
static const char data_2B2E6[10] __attribute__((scn_string_literal)) = "E1032K001";
static const char data_2B2F0[10] __attribute__((scn_string_literal)) = "E1032K002";
static const char data_2B2FA[10] __attribute__((scn_string_literal)) = "E1032K019";
static const char data_2B304[10] __attribute__((scn_string_literal)) = "E1032K020";
static const char data_2B30E[10] __attribute__((scn_string_literal)) = "E1032K022";
static const char data_2B318[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_2B322[10] __attribute__((scn_string_literal)) = "E1032K016";
static const char data_2B32C[10] __attribute__((scn_string_literal)) = "E1032K017";
static const char data_2B336[10] __attribute__((scn_string_literal)) = "E1032K018";
static const char data_2B340[10] __attribute__((scn_string_literal)) = "E1032K003";
static const char data_2B34A[10] __attribute__((scn_string_literal)) = "E1032K016";
static const char data_2B354[10] __attribute__((scn_string_literal)) = "E1032K003";
static const char data_2B35E[10] __attribute__((scn_string_literal)) = "E1032K017";
static const char data_2B368[10] __attribute__((scn_string_literal)) = "E1032K003";
static const char data_2B372[10] __attribute__((scn_string_literal)) = "E1032K018";
static const char data_2B37C[10] __attribute__((scn_string_literal)) = "E1032K004";
static const char data_2B386[10] __attribute__((scn_string_literal)) = "E1032K016";
static const char data_2B390[10] __attribute__((scn_string_literal)) = "E1032K004";
static const char data_2B39A[10] __attribute__((scn_string_literal)) = "E1032K017";
static const char data_2B3A4[10] __attribute__((scn_string_literal)) = "E1032K004";
static const char data_2B3AE[10] __attribute__((scn_string_literal)) = "E1032K018";
static const char data_2B3B8[10] __attribute__((scn_string_literal)) = "E1032K005";
static const char data_2B3C2[10] __attribute__((scn_string_literal)) = "E1032K016";
static const char data_2B3CC[10] __attribute__((scn_string_literal)) = "E1032K005";
static const char data_2B3D6[10] __attribute__((scn_string_literal)) = "E1032K017";
static const char data_2B3E0[10] __attribute__((scn_string_literal)) = "E1032K005";
static const char data_2B3EA[10] __attribute__((scn_string_literal)) = "E1032K018";
static const char data_2B3F4[10] __attribute__((scn_string_literal)) = "E1032K006";
static const char data_2B3FE[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_2B408[10] __attribute__((scn_string_literal)) = "E1032K006";
static const char data_2B412[10] __attribute__((scn_string_literal)) = "E1032K017";
static const char data_2B41C[10] __attribute__((scn_string_literal)) = "E1032K006";
static const char data_2B426[10] __attribute__((scn_string_literal)) = "E1032K018";
static const char data_2B430[10] __attribute__((scn_string_literal)) = "E1032K007";
static const char data_2B43A[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_2B444[10] __attribute__((scn_string_literal)) = "E1032K007";
static const char data_2B44E[10] __attribute__((scn_string_literal)) = "E1032K017";
static const char data_2B458[10] __attribute__((scn_string_literal)) = "E1032K007";
static const char data_2B462[10] __attribute__((scn_string_literal)) = "E1032K018";
static const char data_2B46C[10] __attribute__((scn_string_literal)) = "E1032K008";
static const char data_2B476[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_2B480[10] __attribute__((scn_string_literal)) = "E1032K008";
static const char data_2B48A[10] __attribute__((scn_string_literal)) = "E1032K017";
static const char data_2B494[10] __attribute__((scn_string_literal)) = "E1032K008";
static const char data_2B49E[10] __attribute__((scn_string_literal)) = "E1032K018";
static const char data_2B4A8[10] __attribute__((scn_string_literal)) = "E1032K009";
static const char data_2B4B2[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_2B4BC[10] __attribute__((scn_string_literal)) = "E1032K009";
static const char data_2B4C6[10] __attribute__((scn_string_literal)) = "E1032K016";
static const char data_2B4D0[10] __attribute__((scn_string_literal)) = "E1032K009";
static const char data_2B4DA[10] __attribute__((scn_string_literal)) = "E1032K018";
static const char data_2B4E4[10] __attribute__((scn_string_literal)) = "E1032K010";
static const char data_2B4EE[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_2B4F8[10] __attribute__((scn_string_literal)) = "E1032K010";
static const char data_2B502[10] __attribute__((scn_string_literal)) = "E1032K016";
static const char data_2B50C[10] __attribute__((scn_string_literal)) = "E1032K010";
static const char data_2B516[10] __attribute__((scn_string_literal)) = "E1032K018";
static const char data_2B520[10] __attribute__((scn_string_literal)) = "E1032K011";
static const char data_2B52A[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_2B534[10] __attribute__((scn_string_literal)) = "E1032K011";
static const char data_2B53E[10] __attribute__((scn_string_literal)) = "E1032K016";
static const char data_2B548[10] __attribute__((scn_string_literal)) = "E1032K011";
static const char data_2B552[10] __attribute__((scn_string_literal)) = "E1032K018";
static const char data_2B55C[10] __attribute__((scn_string_literal)) = "E1032K012";
static const char data_2B566[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_2B570[10] __attribute__((scn_string_literal)) = "E1032K012";
static const char data_2B57A[10] __attribute__((scn_string_literal)) = "E1032K016";
static const char data_2B584[10] __attribute__((scn_string_literal)) = "E1032K012";
static const char data_2B58E[10] __attribute__((scn_string_literal)) = "E1032K017";
static const char data_2B598[10] __attribute__((scn_string_literal)) = "E1032K013";
static const char data_2B5A2[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_2B5AC[10] __attribute__((scn_string_literal)) = "E1032K013";
static const char data_2B5B6[10] __attribute__((scn_string_literal)) = "E1032K016";
static const char data_2B5C0[10] __attribute__((scn_string_literal)) = "E1032K013";
static const char data_2B5CA[10] __attribute__((scn_string_literal)) = "E1032K017";
static const char data_2B5D4[10] __attribute__((scn_string_literal)) = "E1032K014";
static const char data_2B5DE[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_2B5E8[10] __attribute__((scn_string_literal)) = "E1032K014";
static const char data_2B5F2[10] __attribute__((scn_string_literal)) = "E1032K016";
static const char data_2B5FC[10] __attribute__((scn_string_literal)) = "E1032K014";
static const char data_2B606[10] __attribute__((scn_string_literal)) = "E1032K017";
static const char data_2B610[10] __attribute__((scn_string_literal)) = "E1032K004";
static const char data_2B61A[10] __attribute__((scn_string_literal)) = "E1032K007";
static const char data_2B624[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_2B62E[10] __attribute__((scn_string_literal)) = "E1032K004";
static const char data_2B638[10] __attribute__((scn_string_literal)) = "E1032K007";
static const char data_2B642[10] __attribute__((scn_string_literal)) = "E1032K017";
static const char data_2B64C[10] __attribute__((scn_string_literal)) = "E1032K004";
static const char data_2B656[10] __attribute__((scn_string_literal)) = "E1032K006";
static const char data_2B660[10] __attribute__((scn_string_literal)) = "E1032K017";
static const char data_2B66A[10] __attribute__((scn_string_literal)) = "E1032K004";
static const char data_2B674[10] __attribute__((scn_string_literal)) = "E1032K013";
static const char data_2B67E[10] __attribute__((scn_string_literal)) = "E1032K017";
static const char data_2B688[10] __attribute__((scn_string_literal)) = "E1032K005";
static const char data_2B692[10] __attribute__((scn_string_literal)) = "E1032K007";
static const char data_2B69C[10] __attribute__((scn_string_literal)) = "E1032K018";
static const char data_2B6A6[10] __attribute__((scn_string_literal)) = "E1032K007";
static const char data_2B6B0[10] __attribute__((scn_string_literal)) = "E1032K004";
static const char data_2B6BA[10] __attribute__((scn_string_literal)) = "E1032K017";
static const char data_2B6C4[10] __attribute__((scn_string_literal)) = "E1032K008";
static const char data_2B6CE[10] __attribute__((scn_string_literal)) = "E1032K010";
static const char data_2B6D8[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_2B6E2[10] __attribute__((scn_string_literal)) = "E1032K008";
static const char data_2B6EC[10] __attribute__((scn_string_literal)) = "E1032K013";
static const char data_2B6F6[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_2B700[10] __attribute__((scn_string_literal)) = "E1032K010";
static const char data_2B70A[10] __attribute__((scn_string_literal)) = "E1032K005";
static const char data_2B714[10] __attribute__((scn_string_literal)) = "E1032K016";
static const char data_2B71E[10] __attribute__((scn_string_literal)) = "E1032K010";
static const char data_2B728[10] __attribute__((scn_string_literal)) = "E1032K004";
static const char data_2B732[10] __attribute__((scn_string_literal)) = "E1032K018";
static const char data_2B73C[10] __attribute__((scn_string_literal)) = "E1032K010";
static const char data_2B746[10] __attribute__((scn_string_literal)) = "E1032K007";
static const char data_2B750[10] __attribute__((scn_string_literal)) = "E1032K018";
static const char data_2B75A[10] __attribute__((scn_string_literal)) = "E1032K011";
static const char data_2B764[10] __attribute__((scn_string_literal)) = "E1032K007";
static const char data_2B76E[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_2B778[10] __attribute__((scn_string_literal)) = "E1032K013";
static const char data_2B782[10] __attribute__((scn_string_literal)) = "E1032K008";
static const char data_2B78C[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_2B796[10] __attribute__((scn_string_literal)) = "E1032K010";
static const char data_2B7A0[10] __attribute__((scn_string_literal)) = "E1032K005";
static const char data_2B7AA[10] __attribute__((scn_string_literal)) = "E1032K016";
static const char data_2B7B4[10] __attribute__((scn_string_literal)) = "E1032K004";
static const char data_2B7BE[10] __attribute__((scn_string_literal)) = "E1032K010";
static const char data_2B7C8[10] __attribute__((scn_string_literal)) = "E1032K016";
static const char data_2B7D2[10] __attribute__((scn_string_literal)) = "E1032K004";
static const char data_2B7DC[10] __attribute__((scn_string_literal)) = "E1032K008";
static const char data_2B7E6[10] __attribute__((scn_string_literal)) = "E1032K018";
static const char data_2B7F0[10] __attribute__((scn_string_literal)) = "E1032K003";
static const char data_2B7FA[10] __attribute__((scn_string_literal)) = "E1032K007";
static const char data_2B804[10] __attribute__((scn_string_literal)) = "E1032K018";
static const char data_2B80E[10] __attribute__((scn_string_literal)) = "E1032K007";
static const char data_2B818[10] __attribute__((scn_string_literal)) = "E1032K010";
static const char data_2B822[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_2B82C[10] __attribute__((scn_string_literal)) = "E1032K008";
static const char data_2B836[10] __attribute__((scn_string_literal)) = "E1032K004";
static const char data_2B840[10] __attribute__((scn_string_literal)) = "E1032K018";
static const char data_2B84A[10] __attribute__((scn_string_literal)) = "E1032K007";
static const char data_2B854[10] __attribute__((scn_string_literal)) = "E1032K010";
static const char data_2B85E[10] __attribute__((scn_string_literal)) = "E1032K018";
static const char data_2B868[10] __attribute__((scn_string_literal)) = "E1032K004";
static const char data_2B872[10] __attribute__((scn_string_literal)) = "E1032K010";
static const char data_2B87C[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_2B886[10] __attribute__((scn_string_literal)) = "E1032K004";
static const char data_2B890[10] __attribute__((scn_string_literal)) = "E1032K007";
static const char data_2B89A[10] __attribute__((scn_string_literal)) = "E1032K017";
static const char data_2B8A4[10] __attribute__((scn_string_literal)) = "E1032K004";
static const char data_2B8AE[10] __attribute__((scn_string_literal)) = "E1032K007";
static const char data_2B8B8[10] __attribute__((scn_string_literal)) = "E1032K018";
static const char data_2B8C2[10] __attribute__((scn_string_literal)) = "E1032K010";
static const char data_2B8CC[10] __attribute__((scn_string_literal)) = "E1032K008";
static const char data_2B8D6[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_2B8E0[10] __attribute__((scn_string_literal)) = "E1032K013";
static const char data_2B8EA[10] __attribute__((scn_string_literal)) = "E1032K007";
static const char data_2B8F4[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_2B8FE[10] __attribute__((scn_string_literal)) = "E1032K004";
static const char data_2B908[10] __attribute__((scn_string_literal)) = "E1032K013";
static const char data_2B912[10] __attribute__((scn_string_literal)) = "E1032K016";
static const char data_2B91C[10] __attribute__((scn_string_literal)) = "E1032K014";
static const char data_2B926[10] __attribute__((scn_string_literal)) = "E1032K007";
static const char data_2B930[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_2B93A[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_2B944[10] __attribute__((scn_string_literal)) = "E1032L001";
static const char data_2B94E[10] __attribute__((scn_string_literal)) = "E1032A098";
static const char data_2B958[10] __attribute__((scn_string_literal)) = "E1032L002";
static const char data_2B962[10] __attribute__((scn_string_literal)) = "E1032A099";
static const char data_2B96C[10] __attribute__((scn_string_literal)) = "E1032L003";
static const char data_2B976[10] __attribute__((scn_string_literal)) = "E1032L004";
static const char data_2B980[10] __attribute__((scn_string_literal)) = "E1032L005";
static const char data_2B98A[10] __attribute__((scn_string_literal)) = "E1032A100";
static const char data_2B994[10] __attribute__((scn_string_literal)) = "E1032L001";
static const char data_2B99E[10] __attribute__((scn_string_literal)) = "E1032A101";
static const char data_2B9A8[10] __attribute__((scn_string_literal)) = "E1032L006";
static const char data_2B9B2[10] __attribute__((scn_string_literal)) = "E1032L007";
static const char data_2B9BC[10] __attribute__((scn_string_literal)) = "E1032L008";
static const char data_2B9C6[10] __attribute__((scn_string_literal)) = "E1032L009";
static const char data_2B9D0[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_2B9DA[10] __attribute__((scn_string_literal)) = "E1032A102";
static const char data_2B9E4[10] __attribute__((scn_string_literal)) = "E1032L010";
static const char data_2B9EE[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_2B9F8[10] __attribute__((scn_string_literal)) = "E1032L011";
static const char data_2BA02[10] __attribute__((scn_string_literal)) = "E1032A101";
static const char data_2BA0C[10] __attribute__((scn_string_literal)) = "E1032L012";
static const char data_2BA16[10] __attribute__((scn_string_literal)) = "E1032A103";
static const char data_2BA20[10] __attribute__((scn_string_literal)) = "E1032L013";
static const char data_2BA2A[10] __attribute__((scn_string_literal)) = "E1032L014";
static const char data_2BA34[10] __attribute__((scn_string_literal)) = "E1032L015";
static const char data_2BA3E[10] __attribute__((scn_string_literal)) = "E1032A103";
static const char data_2BA48[10] __attribute__((scn_string_literal)) = "E1032L016";
static const char data_2BA52[10] __attribute__((scn_string_literal)) = "E1032A104";
static const char data_2BA5C[10] __attribute__((scn_string_literal)) = "E1032A105";
static const char data_2BA66[10] __attribute__((scn_string_literal)) = "E1032L017";
static const char data_2BA70[10] __attribute__((scn_string_literal)) = "E1032A106";
static const char data_2BA7A[10] __attribute__((scn_string_literal)) = "E1032L018";
static const char data_2BA84[10] __attribute__((scn_string_literal)) = "E1032A107";
static const char data_2BA8E[10] __attribute__((scn_string_literal)) = "E1032L019";
static const char data_2BA98[10] __attribute__((scn_string_literal)) = "E1032A108";
static const char data_2BAA2[10] __attribute__((scn_string_literal)) = "E1032L020";
static const char data_2BAAC[10] __attribute__((scn_string_literal)) = "E1032L021";
static const char data_2BAB6[10] __attribute__((scn_string_literal)) = "E1032A109";
static const char data_2BAC0[10] __attribute__((scn_string_literal)) = "E1032L022";
static const char data_2BACA[10] __attribute__((scn_string_literal)) = "E1032A110";
static const char data_2BAD4[10] __attribute__((scn_string_literal)) = "E1032L023";
static const char data_2BADE[10] __attribute__((scn_string_literal)) = "E1032M001";
static const char data_2BAE8[10] __attribute__((scn_string_literal)) = "E1032A111";
static const char data_2BAF2[10] __attribute__((scn_string_literal)) = "E1032M002";
static const char data_2BAFC[10] __attribute__((scn_string_literal)) = "E1032A112";
static const char data_2BB06[10] __attribute__((scn_string_literal)) = "E1032M003";
static const char data_2BB10[10] __attribute__((scn_string_literal)) = "E1032A113";
static const char data_2BB1A[10] __attribute__((scn_string_literal)) = "E1032M004";
static const char data_2BB24[10] __attribute__((scn_string_literal)) = "E1032A104";
static const char data_2BB2E[10] __attribute__((scn_string_literal)) = "E1032A114";
static const char data_2BB38[10] __attribute__((scn_string_literal)) = "E1032M005";
static const char data_2BB42[10] __attribute__((scn_string_literal)) = "E1032M006";
static const char data_2BB4C[10] __attribute__((scn_string_literal)) = "E1032A111";
static const char data_2BB56[10] __attribute__((scn_string_literal)) = "E1032M002";
static const char data_2BB60[10] __attribute__((scn_string_literal)) = "E1032A115";
static const char data_2BB6A[10] __attribute__((scn_string_literal)) = "E1032M018";
static const char data_2BB74[10] __attribute__((scn_string_literal)) = "E1032A122";
static const char data_2BB7E[10] __attribute__((scn_string_literal)) = "E1032M019";
static const char data_2BB88[10] __attribute__((scn_string_literal)) = "E1032A123";
static const char data_2BB92[10] __attribute__((scn_string_literal)) = "E1032M019";
static const char data_2BB9C[10] __attribute__((scn_string_literal)) = "E1032M020";
static const char data_2BBA6[10] __attribute__((scn_string_literal)) = "E1032A104";
static const char data_2BBB0[10] __attribute__((scn_string_literal)) = "E1032A114";
static const char data_2BBBA[10] __attribute__((scn_string_literal)) = "E1032M010";
static const char data_2BBC4[10] __attribute__((scn_string_literal)) = "E1032N001";
static const char data_2BBCE[10] __attribute__((scn_string_literal)) = "E1032A124";
static const char data_2BBD8[10] __attribute__((scn_string_literal)) = "E1032A125";
static const char data_2BBE2[10] __attribute__((scn_string_literal)) = "E1032N002";
static const char data_2BBEC[10] __attribute__((scn_string_literal)) = "E1032A126";
static const char data_2BBF6[10] __attribute__((scn_string_literal)) = "E1032N003";
static const char data_2BC00[10] __attribute__((scn_string_literal)) = "E1032A127";
static const char data_2BC0A[10] __attribute__((scn_string_literal)) = "E1032A128";
static const char data_2BC14[10] __attribute__((scn_string_literal)) = "E1032N004";
static const char data_2BC1E[10] __attribute__((scn_string_literal)) = "E1032A129";
static const char data_2BC28[10] __attribute__((scn_string_literal)) = "E1032N005";
static const char data_2BC32[10] __attribute__((scn_string_literal)) = "E1032L011";
static const char data_2BC3C[10] __attribute__((scn_string_literal)) = "E1032A101";
static const char data_2BC46[10] __attribute__((scn_string_literal)) = "E1032L019";
static const char data_2BC50[10] __attribute__((scn_string_literal)) = "E1032A117";
static const char data_2BC5A[10] __attribute__((scn_string_literal)) = "E1032L024";
static const char data_2BC64[10] __attribute__((scn_string_literal)) = "E1032L025";
static const char data_2BC6E[10] __attribute__((scn_string_literal)) = "E1032L026";
static const char data_2BC78[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_2BC82[10] __attribute__((scn_string_literal)) = "E1032A118";
static const char data_2BC8C[10] __attribute__((scn_string_literal)) = "E1032L023";
static const char data_2BC96[10] __attribute__((scn_string_literal)) = "E1032M001";
static const char data_2BCA0[10] __attribute__((scn_string_literal)) = "E1032A111";
static const char data_2BCAA[10] __attribute__((scn_string_literal)) = "E1032M002";
static const char data_2BCB4[10] __attribute__((scn_string_literal)) = "E1032A112";
static const char data_2BCBE[10] __attribute__((scn_string_literal)) = "E1032M015";
static const char data_2BCC8[10] __attribute__((scn_string_literal)) = "E1032A119";
static const char data_2BCD2[10] __attribute__((scn_string_literal)) = "E1032M016";
static const char data_2BCDC[10] __attribute__((scn_string_literal)) = "E1032A104";
static const char data_2BCE6[10] __attribute__((scn_string_literal)) = "E1032A118";
static const char data_2BCF0[10] __attribute__((scn_string_literal)) = "E1032M017";
static const char data_2BCFA[10] __attribute__((scn_string_literal)) = "E1032O001";
static const char data_2BD04[10] __attribute__((scn_string_literal)) = "E1032A136";
static const char data_2BD0E[10] __attribute__((scn_string_literal)) = "E1032O002";
static const char data_2BD18[10] __attribute__((scn_string_literal)) = "E1032A137";
static const char data_2BD22[10] __attribute__((scn_string_literal)) = "E1032O007";
static const char data_2BD2C[10] __attribute__((scn_string_literal)) = "E1032A127";
static const char data_2BD36[10] __attribute__((scn_string_literal)) = "E1032O004";
static const char data_2BD40[10] __attribute__((scn_string_literal)) = "E1032A138";
static const char data_2BD4A[10] __attribute__((scn_string_literal)) = "E1032O005";
static const char data_2BD54[10] __attribute__((scn_string_literal)) = "E1032A139";
static const char data_2BD5E[10] __attribute__((scn_string_literal)) = "E1032O006";
static const char data_2BD68[10] __attribute__((scn_string_literal)) = "E1032O001";
static const char data_2BD72[10] __attribute__((scn_string_literal)) = "E1032A136";
static const char data_2BD7C[10] __attribute__((scn_string_literal)) = "E1032O002";
static const char data_2BD86[10] __attribute__((scn_string_literal)) = "E1032A140";
static const char data_2BD90[10] __attribute__((scn_string_literal)) = "E1032O007";
static const char data_2BD9A[10] __attribute__((scn_string_literal)) = "E1032A127";
static const char data_2BDA4[10] __attribute__((scn_string_literal)) = "E1032O004";
static const char data_2BDAE[10] __attribute__((scn_string_literal)) = "E1032A138";
static const char data_2BDB8[10] __attribute__((scn_string_literal)) = "E1032O005";
static const char data_2BDC2[10] __attribute__((scn_string_literal)) = "E1032A139";
static const char data_2BDCC[10] __attribute__((scn_string_literal)) = "E1032O006";
static const char data_2BDD6[10] __attribute__((scn_string_literal)) = "E1032P001";
static const char data_2BDE0[10] __attribute__((scn_string_literal)) = "E1032A085";
static const char data_2BDEA[10] __attribute__((scn_string_literal)) = "E1032P002";
static const char data_2BDF4[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_2BDFE[10] __attribute__((scn_string_literal)) = "E1032P003";
static const char data_2BE08[10] __attribute__((scn_string_literal)) = "E1032A141";
static const char data_2BE12[10] __attribute__((scn_string_literal)) = "E1032P004";
static const char data_2BE1C[10] __attribute__((scn_string_literal)) = "E1032A142";
static const char data_2BE26[10] __attribute__((scn_string_literal)) = "E1032P005";
static const char data_2BE30[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_2BE3A[10] __attribute__((scn_string_literal)) = "E1032P001";
static const char data_2BE44[10] __attribute__((scn_string_literal)) = "E1032A085";
static const char data_2BE4E[10] __attribute__((scn_string_literal)) = "E1032P006";
static const char data_2BE58[10] __attribute__((scn_string_literal)) = "E1032A143";
static const char data_2BE62[10] __attribute__((scn_string_literal)) = "E1032P007";
static const char data_2BE6C[10] __attribute__((scn_string_literal)) = "E1032A144";
static const char data_2BE76[10] __attribute__((scn_string_literal)) = "E1032P004";
static const char data_2BE80[10] __attribute__((scn_string_literal)) = "E1032O008";
static const char data_2BE8A[10] __attribute__((scn_string_literal)) = "E1032P008";
static const char data_2BE94[10] __attribute__((scn_string_literal)) = "E1032A068";
static const char data_2BE9E[10] __attribute__((scn_string_literal)) = "E1032P009";
static const char data_2BEA8[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_2BEB2[10] __attribute__((scn_string_literal)) = "E1032A085";
static const char data_2BEBC[10] __attribute__((scn_string_literal)) = "E1032P010";
static const char data_2BEC6[10] __attribute__((scn_string_literal)) = "E1032A145";
static const char data_2BED0[10] __attribute__((scn_string_literal)) = "E1032P011";
static const char data_2BEDA[10] __attribute__((scn_string_literal)) = "E1032A146";
static const char data_2BEE4[10] __attribute__((scn_string_literal)) = "E1032P012";
static const char data_2BEEE[10] __attribute__((scn_string_literal)) = "E1032A147";
static const char data_2BEF8[10] __attribute__((scn_string_literal)) = "E1032P013";
static const char data_2BF02[10] __attribute__((scn_string_literal)) = "E1032A148";
static const char data_2BF0C[10] __attribute__((scn_string_literal)) = "E1032P014";
static const char data_2BF16[10] __attribute__((scn_string_literal)) = "E1032A149";
static const char data_2BF20[10] __attribute__((scn_string_literal)) = "E1032P015";
static const char data_2BF2A[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_2BF34[10] __attribute__((scn_string_literal)) = "E1032P016";
static const char data_2BF3E[10] __attribute__((scn_string_literal)) = "E1032A068";
static const char data_2BF48[10] __attribute__((scn_string_literal)) = "E1032P017";
static const char data_2BF52[10] __attribute__((scn_string_literal)) = "E1032A085";
static const char data_2BF5C[10] __attribute__((scn_string_literal)) = "E1032P018";
static const char data_2BF66[10] __attribute__((scn_string_literal)) = "E1032A150";
static const char data_2BF70[10] __attribute__((scn_string_literal)) = "E1032P019";
static const char data_2BF7A[10] __attribute__((scn_string_literal)) = "E1032A151";
static const char data_2BF84[10] __attribute__((scn_string_literal)) = "E1032P004";
static const char data_2BF8E[10] __attribute__((scn_string_literal)) = "E1032P016";
static const char data_2BF98[10] __attribute__((scn_string_literal)) = "E1032A068";
static const char data_2BFA2[10] __attribute__((scn_string_literal)) = "E1032P017";
static const char data_2BFAC[10] __attribute__((scn_string_literal)) = "E1032P021";
static const char data_2BFB6[10] __attribute__((scn_string_literal)) = "E1032A152";
static const char data_2BFC0[10] __attribute__((scn_string_literal)) = "E1032P010";
static const char data_2BFCA[10] __attribute__((scn_string_literal)) = "E1032A153";
static const char data_2BFD4[10] __attribute__((scn_string_literal)) = "E1032P022";
static const char data_2BFDE[10] __attribute__((scn_string_literal)) = "E1032A154";
static const char data_2BFE8[10] __attribute__((scn_string_literal)) = "E1032P023";
static const char data_2BFF2[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_2BFFC[10] __attribute__((scn_string_literal)) = "E1032P024";
static const char data_2C006[10] __attribute__((scn_string_literal)) = "E1032A068";
static const char data_2C010[10] __attribute__((scn_string_literal)) = "E1032P017";
static const char data_2C01A[10] __attribute__((scn_string_literal)) = "E1032P001";
static const char data_2C024[10] __attribute__((scn_string_literal)) = "E1032A085";
static const char data_2C02E[10] __attribute__((scn_string_literal)) = "E1032P025";
static const char data_2C038[10] __attribute__((scn_string_literal)) = "E1032A155";
static const char data_2C042[10] __attribute__((scn_string_literal)) = "E1032P004";
static const char data_2C04C[10] __attribute__((scn_string_literal)) = "E1032A156";
static const char data_2C056[10] __attribute__((scn_string_literal)) = "E1032O008";
static const char data_2C060[10] __attribute__((scn_string_literal)) = "E1032P008";
static const char data_2C06A[10] __attribute__((scn_string_literal)) = "E1032A154";
static const char data_2C074[10] __attribute__((scn_string_literal)) = "E1032P027";
static const char data_2C07E[10] __attribute__((scn_string_literal)) = "E1032A068";
static const char data_2C088[10] __attribute__((scn_string_literal)) = "E1032P017";
static const char data_2C092[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_2C09C[10] __attribute__((scn_string_literal)) = "E1032P021";
static const char data_2C0A6[10] __attribute__((scn_string_literal)) = "E1032A152";
static const char data_2C0B0[10] __attribute__((scn_string_literal)) = "E1032P028";
static const char data_2C0BA[10] __attribute__((scn_string_literal)) = "E1032A158";
static const char data_2C0C4[10] __attribute__((scn_string_literal)) = "E1032P030";
static const char data_2C0CE[10] __attribute__((scn_string_literal)) = "E1032A159";
static const char data_2C0D8[10] __attribute__((scn_string_literal)) = "E1032P066";
static const char data_2C0E2[10] __attribute__((scn_string_literal)) = "E1032P016";
static const char data_2C0EC[10] __attribute__((scn_string_literal)) = "E1032A068";
static const char data_2C0F6[10] __attribute__((scn_string_literal)) = "E1032P017";
static const char data_2C100[10] __attribute__((scn_string_literal)) = "E1032P021";
static const char data_2C10A[10] __attribute__((scn_string_literal)) = "E1032A152";
static const char data_2C114[10] __attribute__((scn_string_literal)) = "E1032P032";
static const char data_2C11E[10] __attribute__((scn_string_literal)) = "E1032A160";
static const char data_2C128[10] __attribute__((scn_string_literal)) = "E1032P033";
static const char data_2C132[10] __attribute__((scn_string_literal)) = "E1032P034";
static const char data_2C13C[10] __attribute__((scn_string_literal)) = "E1032P080";
static const char data_2C146[10] __attribute__((scn_string_literal)) = "E1032P081";
static const char data_2C150[10] __attribute__((scn_string_literal)) = "E1032A068";
static const char data_2C15A[10] __attribute__((scn_string_literal)) = "E1032P035";
static const char data_2C164[10] __attribute__((scn_string_literal)) = "E1032A141";
static const char data_2C16E[10] __attribute__((scn_string_literal)) = "E1032P036";
static const char data_2C178[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_2C182[10] __attribute__((scn_string_literal)) = "E1032P021";
static const char data_2C18C[10] __attribute__((scn_string_literal)) = "E1032A152";
static const char data_2C196[10] __attribute__((scn_string_literal)) = "E1032P025";
static const char data_2C1A0[10] __attribute__((scn_string_literal)) = "E1032A160";
static const char data_2C1AA[10] __attribute__((scn_string_literal)) = "E1032P033";
static const char data_2C1B4[10] __attribute__((scn_string_literal)) = "E1032P082";
static const char data_2C1BE[10] __attribute__((scn_string_literal)) = "E1032P083";
static const char data_2C1C8[10] __attribute__((scn_string_literal)) = "E1032P084";
static const char data_2C1D2[10] __attribute__((scn_string_literal)) = "E1032P085";
static const char data_2C1DC[10] __attribute__((scn_string_literal)) = "E1032A068";
static const char data_2C1E6[10] __attribute__((scn_string_literal)) = "E1032P086";
static const char data_2C1F0[10] __attribute__((scn_string_literal)) = "E1032P001";
static const char data_2C1FA[10] __attribute__((scn_string_literal)) = "E1032A164";
static const char data_2C204[10] __attribute__((scn_string_literal)) = "E1032P018";
static const char data_2C20E[10] __attribute__((scn_string_literal)) = "E1032A165";
static const char data_2C218[10] __attribute__((scn_string_literal)) = "E1032P037";
static const char data_2C222[10] __attribute__((scn_string_literal)) = "E1032A104";
static const char data_2C22C[10] __attribute__((scn_string_literal)) = "E1032P120";
static const char data_2C236[10] __attribute__((scn_string_literal)) = "E1032A173";
static const char data_2C240[10] __attribute__((scn_string_literal)) = "E1032O008";
static const char data_2C24A[10] __attribute__((scn_string_literal)) = "E1032P008";
static const char data_2C254[10] __attribute__((scn_string_literal)) = "E1032A154";
static const char data_2C25E[10] __attribute__((scn_string_literal)) = "E1032P027";
static const char data_2C268[10] __attribute__((scn_string_literal)) = "E1032A068";
static const char data_2C272[10] __attribute__((scn_string_literal)) = "E1032P017";
static const char data_2C27C[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_2C286[10] __attribute__((scn_string_literal)) = "E1032P001";
static const char data_2C290[10] __attribute__((scn_string_literal)) = "E1032A164";
static const char data_2C29A[10] __attribute__((scn_string_literal)) = "E1032P018";
static const char data_2C2A4[10] __attribute__((scn_string_literal)) = "E1032A170";
static const char data_2C2AE[10] __attribute__((scn_string_literal)) = "E1032P044";
static const char data_2C2B8[10] __attribute__((scn_string_literal)) = "E1032A200";
static const char data_2C2C2[10] __attribute__((scn_string_literal)) = "E1032A172";
static const char data_2C2CC[10] __attribute__((scn_string_literal)) = "E1032P045";
static const char data_2C2D6[10] __attribute__((scn_string_literal)) = "E1032A173";
static const char data_2C2E0[10] __attribute__((scn_string_literal)) = "E1032P046";
static const char data_2C2EA[10] __attribute__((scn_string_literal)) = "E1032P047";
static const char data_2C2F4[10] __attribute__((scn_string_literal)) = "E1032A201";
static const char data_2C2FE[10] __attribute__((scn_string_literal)) = "E1032P049";
static const char data_2C308[10] __attribute__((scn_string_literal)) = "E1032A176";
static const char data_2C312[10] __attribute__((scn_string_literal)) = "E1032P050";
static const char data_2C31C[10] __attribute__((scn_string_literal)) = "E1032A177";
static const char data_2C326[10] __attribute__((scn_string_literal)) = "E1032P051";
static const char data_2C330[10] __attribute__((scn_string_literal)) = "E1032P001";
static const char data_2C33A[10] __attribute__((scn_string_literal)) = "E1032A164";
static const char data_2C344[10] __attribute__((scn_string_literal)) = "E1032P018";
static const char data_2C34E[10] __attribute__((scn_string_literal)) = "E1032A178";
static const char data_2C358[10] __attribute__((scn_string_literal)) = "E1032P004";
static const char data_2C362[10] __attribute__((scn_string_literal)) = "E1032P052";
static const char data_2C36C[10] __attribute__((scn_string_literal)) = "E1032A179";
static const char data_2C376[10] __attribute__((scn_string_literal)) = "E1032A180";
static const char data_2C380[10] __attribute__((scn_string_literal)) = "E1032A181";
static const char data_2C38A[10] __attribute__((scn_string_literal)) = "E1032P053";
static const char data_2C394[10] __attribute__((scn_string_literal)) = "E1032P087";
static const char data_2C39E[10] __attribute__((scn_string_literal)) = "E1032P055";
static const char data_2C3A8[10] __attribute__((scn_string_literal)) = "E1032A182";
static const char data_2C3B2[10] __attribute__((scn_string_literal)) = "E1032P051";
static const char data_2C3BC[10] __attribute__((scn_string_literal)) = "E1032P001";
static const char data_2C3C6[10] __attribute__((scn_string_literal)) = "E1032A164";
static const char data_2C3D0[10] __attribute__((scn_string_literal)) = "E1032P018";
static const char data_2C3DA[10] __attribute__((scn_string_literal)) = "E1032A183";
static const char data_2C3E4[10] __attribute__((scn_string_literal)) = "E1032P038";
static const char data_2C3EE[10] __attribute__((scn_string_literal)) = "E1032A184";
static const char data_2C3F8[10] __attribute__((scn_string_literal)) = "E1032P056";
static const char data_2C402[10] __attribute__((scn_string_literal)) = "E1032P088";
static const char data_2C40C[10] __attribute__((scn_string_literal)) = "E1032P058";
static const char data_2C416[10] __attribute__((scn_string_literal)) = "E1032A220";
static const char data_2C420[10] __attribute__((scn_string_literal)) = "E1032A185";
static const char data_2C42A[10] __attribute__((scn_string_literal)) = "E1032P059";
static const char data_2C434[10] __attribute__((scn_string_literal)) = "E1032P001";
static const char data_2C43E[10] __attribute__((scn_string_literal)) = "E1032A164";
static const char data_2C448[10] __attribute__((scn_string_literal)) = "E1032P018";
static const char data_2C452[10] __attribute__((scn_string_literal)) = "E1032P060";
static const char data_2C45C[10] __attribute__((scn_string_literal)) = "E1032A186";
static const char data_2C466[10] __attribute__((scn_string_literal)) = "E1032P061";
static const char data_2C470[10] __attribute__((scn_string_literal)) = "E1032A187";
static const char data_2C47A[10] __attribute__((scn_string_literal)) = "E1032P062";
static const char data_2C484[10] __attribute__((scn_string_literal)) = "E1032A188";
static const char data_2C48E[10] __attribute__((scn_string_literal)) = "E1032P063";
static const char data_2C498[10] __attribute__((scn_string_literal)) = "E1032P064";
static const char data_2C4A2[10] __attribute__((scn_string_literal)) = "E1032P065";
static const char data_2C4AC[10] __attribute__((scn_string_literal)) = "E1032A189";
static const char data_2C4B6[10] __attribute__((scn_string_literal)) = "E1032P066";
static const char data_2C4C0[10] __attribute__((scn_string_literal)) = "E1032A190";
static const char data_2C4CA[10] __attribute__((scn_string_literal)) = "E1032P001";
static const char data_2C4D4[10] __attribute__((scn_string_literal)) = "E1032A152";
static const char data_2C4DE[10] __attribute__((scn_string_literal)) = "E1032P067";
static const char data_2C4E8[10] __attribute__((scn_string_literal)) = "E1032A191";
static const char data_2C4F2[10] __attribute__((scn_string_literal)) = "E1032A192";
static const char data_2C4FC[10] __attribute__((scn_string_literal)) = "E1032P068";
static const char data_2C506[10] __attribute__((scn_string_literal)) = "E1032P069";
static const char data_2C510[10] __attribute__((scn_string_literal)) = "E1032A193";
static const char data_2C51A[10] __attribute__((scn_string_literal)) = "E1032P001";
static const char data_2C524[10] __attribute__((scn_string_literal)) = "E1032A152";
static const char data_2C52E[10] __attribute__((scn_string_literal)) = "E1032P070";
static const char data_2C538[10] __attribute__((scn_string_literal)) = "E1032A194";
static const char data_2C542[10] __attribute__((scn_string_literal)) = "E1032P071";
static const char data_2C54C[10] __attribute__((scn_string_literal)) = "E1032A195";
static const char data_2C556[10] __attribute__((scn_string_literal)) = "E1032P072";
static const char data_2C560[10] __attribute__((scn_string_literal)) = "E1032P073";
static const char data_2C56A[10] __attribute__((scn_string_literal)) = "E1032A151";
static const char data_2C574[10] __attribute__((scn_string_literal)) = "E1032P074";
static const char data_2C57E[10] __attribute__((scn_string_literal)) = "E1032P075";
static const char data_2C588[10] __attribute__((scn_string_literal)) = "E1032P089";
static const char data_2C592[10] __attribute__((scn_string_literal)) = "E1032P090";
static const char data_2C59C[10] __attribute__((scn_string_literal)) = "E1032A196";
static const char data_2C5A6[10] __attribute__((scn_string_literal)) = "E1032P079";
static const char data_2C5B0[10] __attribute__((scn_string_literal)) = "E1032A197";
static const char data_2C5BA[10] __attribute__((scn_string_literal)) = "E1032P001";
static const char data_2C5C4[10] __attribute__((scn_string_literal)) = "E1032A152";
static const char data_2C5CE[10] __attribute__((scn_string_literal)) = "E1032P010";
static const char data_2C5D8[10] __attribute__((scn_string_literal)) = "E1032P091";
static const char data_2C5E2[10] __attribute__((scn_string_literal)) = "E1032P092";
static const char data_2C5EC[10] __attribute__((scn_string_literal)) = "E1032A203";
static const char data_2C5F6[10] __attribute__((scn_string_literal)) = "E1032P093";
static const char data_2C600[10] __attribute__((scn_string_literal)) = "E1032P094";
static const char data_2C60A[10] __attribute__((scn_string_literal)) = "E1032A204";
static const char data_2C614[10] __attribute__((scn_string_literal)) = "E1032P095";
static const char data_2C61E[10] __attribute__((scn_string_literal)) = "E1032P021";
static const char data_2C628[10] __attribute__((scn_string_literal)) = "E1032A152";
static const char data_2C632[10] __attribute__((scn_string_literal)) = "E1032P096";
static const char data_2C63C[10] __attribute__((scn_string_literal)) = "E1032A104";
static const char data_2C646[10] __attribute__((scn_string_literal)) = "E1032P097";
static const char data_2C650[10] __attribute__((scn_string_literal)) = "E1032A103";
static const char data_2C65A[10] __attribute__((scn_string_literal)) = "E1032P098";
static const char data_2C664[10] __attribute__((scn_string_literal)) = "E1032P099";
static const char data_2C66E[10] __attribute__((scn_string_literal)) = "E1032P100";
static const char data_2C678[10] __attribute__((scn_string_literal)) = "E1032A206";
static const char data_2C682[10] __attribute__((scn_string_literal)) = "E1032P021";
static const char data_2C68C[10] __attribute__((scn_string_literal)) = "E1032A152";
static const char data_2C696[10] __attribute__((scn_string_literal)) = "E1032P101";
static const char data_2C6A0[10] __attribute__((scn_string_literal)) = "E1032A207";
static const char data_2C6AA[10] __attribute__((scn_string_literal)) = "E1032P102";
static const char data_2C6B4[10] __attribute__((scn_string_literal)) = "E1032A208";
static const char data_2C6BE[10] __attribute__((scn_string_literal)) = "E1032P103";
static const char data_2C6C8[10] __attribute__((scn_string_literal)) = "E1032A209";
static const char data_2C6D2[10] __attribute__((scn_string_literal)) = "E1032P104";
static const char data_2C6DC[10] __attribute__((scn_string_literal)) = "E1032P105";
static const char data_2C6E6[10] __attribute__((scn_string_literal)) = "E1032A210";
static const char data_2C6F0[10] __attribute__((scn_string_literal)) = "E1032P106";
static const char data_2C6FA[10] __attribute__((scn_string_literal)) = "E1032A154";
static const char data_2C704[10] __attribute__((scn_string_literal)) = "E1032P107";
static const char data_2C70E[10] __attribute__((scn_string_literal)) = "E1032A204";
static const char data_2C718[10] __attribute__((scn_string_literal)) = "E1032P108";
static const char data_2C722[10] __attribute__((scn_string_literal)) = "E1032P109";
static const char data_2C72C[10] __attribute__((scn_string_literal)) = "E1032A085";
static const char data_2C736[10] __attribute__((scn_string_literal)) = "E1032P110";
static const char data_2C740[10] __attribute__((scn_string_literal)) = "E1032A211";
static const char data_2C74A[10] __attribute__((scn_string_literal)) = "E1032P111";
static const char data_2C754[10] __attribute__((scn_string_literal)) = "E1032A104";
static const char data_2C75E[10] __attribute__((scn_string_literal)) = "E1032P112";
static const char data_2C768[10] __attribute__((scn_string_literal)) = "E1032A212";
static const char data_2C772[10] __attribute__((scn_string_literal)) = "E1032P113";
static const char data_2C77C[10] __attribute__((scn_string_literal)) = "E1032A158";
static const char data_2C786[10] __attribute__((scn_string_literal)) = "E1032P114";
static const char data_2C790[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_2C79A[10] __attribute__((scn_string_literal)) = "E1032P115";
static const char data_2C7A4[10] __attribute__((scn_string_literal)) = "E1032A213";
static const char data_2C7AE[10] __attribute__((scn_string_literal)) = "E1032P116";
static const char data_2C7B8[10] __attribute__((scn_string_literal)) = "E1032P001";
static const char data_2C7C2[10] __attribute__((scn_string_literal)) = "E1032A152";
static const char data_2C7CC[10] __attribute__((scn_string_literal)) = "E1032P117";
static const char data_2C7D6[10] __attribute__((scn_string_literal)) = "E1032A214";
static const char data_2C7E0[10] __attribute__((scn_string_literal)) = "E1032P118";
static const char data_2C7EA[10] __attribute__((scn_string_literal)) = "E1032A154";
static const char data_2C7F4[10] __attribute__((scn_string_literal)) = "E1032P119";
static const char data_2C7FE[10] __attribute__((scn_string_literal)) = "E1032P120";
static const char data_2C808[10] __attribute__((scn_string_literal)) = "E1032A215";
static const char data_2C812[10] __attribute__((scn_string_literal)) = "E1032P121";
static const char data_2C81C[10] __attribute__((scn_string_literal)) = "E1032A163";
static const char data_2C826[10] __attribute__((scn_string_literal)) = "E1032P123";
static const char data_2C830[10] __attribute__((scn_string_literal)) = "E1032A217";
static const char data_2C83A[10] __attribute__((scn_string_literal)) = "E1032P001";
static const char data_2C844[10] __attribute__((scn_string_literal)) = "E1032A152";
static const char data_2C84E[10] __attribute__((scn_string_literal)) = "E1032P010";
static const char data_2C858[10] __attribute__((scn_string_literal)) = "E1032P124";
static const char data_2C862[10] __attribute__((scn_string_literal)) = "E1032P125";
static const char data_2C86C[10] __attribute__((scn_string_literal)) = "E1032A218";
static const char data_2C876[10] __attribute__((scn_string_literal)) = "E1032P126";
static const char data_2C880[10] __attribute__((scn_string_literal)) = "E1032A154";
static const char data_2C88A[10] __attribute__((scn_string_literal)) = "E1032P127";
static const char data_2C894[10] __attribute__((scn_string_literal)) = "E1032A213";
static const char data_2C89E[10] __attribute__((scn_string_literal)) = "E1032P128";
static const char data_2C8A8[10] __attribute__((scn_string_literal)) = "E1032A204";
static const char data_2C8B2[10] __attribute__((scn_string_literal)) = "E1032P116";
static const char data_2C8BC[10] __attribute__((scn_string_literal)) = "E1032P109";
static const char data_2C8C6[10] __attribute__((scn_string_literal)) = "E1032A085";
static const char data_2C8D0[10] __attribute__((scn_string_literal)) = "E1032P117";
static const char data_2C8DA[10] __attribute__((scn_string_literal)) = "E1032A214";
static const char data_2C8E4[10] __attribute__((scn_string_literal)) = "E1032P129";
static const char data_2C8EE[10] __attribute__((scn_string_literal)) = "E1032P130";
static const char data_2C8F8[10] __attribute__((scn_string_literal)) = "E1032P131";
static const char data_2C902[10] __attribute__((scn_string_literal)) = "E1032A213";
static const char data_2C90C[10] __attribute__((scn_string_literal)) = "E1032A219";
static const char data_2C916[10] __attribute__((scn_string_literal)) = "E1032P132";
static const char data_2C920[10] __attribute__((scn_string_literal)) = "E1032A153";
static const char data_2C92A[10] __attribute__((scn_string_literal)) = "E1032P133";
static const char data_2C934[10] __attribute__((scn_string_literal)) = "E1032A220";
static const char data_2C93E[10] __attribute__((scn_string_literal)) = "E1032P134";
static const char data_2C948[10] __attribute__((scn_string_literal)) = "E1032A204";
static const char data_2C952[10] __attribute__((scn_string_literal)) = "E1032P116";
static const char data_2C95C[10] __attribute__((scn_string_literal)) = "E1032A085";
static const char data_2C966[10] __attribute__((scn_string_literal)) = "E1032P010";
static const char data_2C970[10] __attribute__((scn_string_literal)) = "E1032A221";
static const char data_2C97A[10] __attribute__((scn_string_literal)) = "E1032P135";
static const char data_2C984[10] __attribute__((scn_string_literal)) = "E1032P136";
static const char data_2C98E[10] __attribute__((scn_string_literal)) = "E1032A222";
static const char data_2C998[10] __attribute__((scn_string_literal)) = "E1032P137";
static const char data_2C9A2[10] __attribute__((scn_string_literal)) = "E1032P138";
static const char data_2C9AC[10] __attribute__((scn_string_literal)) = "E1032A223";
static const char data_2C9B6[10] __attribute__((scn_string_literal)) = "E1032P139";
static const char data_2C9C0[10] __attribute__((scn_string_literal)) = "E1032A224";
static const char data_2C9CA[10] __attribute__((scn_string_literal)) = "E1032P140";
static const char data_2C9D4[10] __attribute__((scn_string_literal)) = "E1032A225";
static const char data_2C9DE[10] __attribute__((scn_string_literal)) = "E1032A226";
static const char data_2C9E8[10] __attribute__((scn_string_literal)) = "E1032P004";
static const char data_2C9F2[10] __attribute__((scn_string_literal)) = "E1032E001";
static const char data_2C9FC[10] __attribute__((scn_string_literal)) = "E1032E002";
static const char data_2CA06[10] __attribute__((scn_string_literal)) = "E1032E003";
static const char data_2CA10[10] __attribute__((scn_string_literal)) = "E1032A010";
static const char data_2CA1A[10] __attribute__((scn_string_literal)) = "E1032E004";
static const char data_2CA24[10] __attribute__((scn_string_literal)) = "E1032A011";
static const char data_2CA2E[10] __attribute__((scn_string_literal)) = "E1032E005";
static const char data_2CA38[10] __attribute__((scn_string_literal)) = "E1032A012";
static const char data_2CA42[10] __attribute__((scn_string_literal)) = "E1032A013";
static const char data_2CA4C[10] __attribute__((scn_string_literal)) = "E1032E006";
static const char data_2CA56[10] __attribute__((scn_string_literal)) = "E1032A014";
static const char data_2CA60[10] __attribute__((scn_string_literal)) = "E1032A015";
static const char data_2CA6A[10] __attribute__((scn_string_literal)) = "E1032E007";
static const char data_2CA74[10] __attribute__((scn_string_literal)) = "E1032A016";
static const char data_2CA7E[10] __attribute__((scn_string_literal)) = "E1032E008";
static const char data_2CA88[10] __attribute__((scn_string_literal)) = "E1032E009";
static const char data_2CA92[10] __attribute__((scn_string_literal)) = "E1032A017";
static const char data_2CA9C[10] __attribute__((scn_string_literal)) = "E1032E010";
static const char data_2CAA6[10] __attribute__((scn_string_literal)) = "E1032E011";
static const char data_2CAB0[10] __attribute__((scn_string_literal)) = "E1032E012";
static const char data_2CABA[10] __attribute__((scn_string_literal)) = "E1032E013";
static const char data_2CAC4[10] __attribute__((scn_string_literal)) = "E1032A018";
static const char data_2CACE[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_2CAD8[10] __attribute__((scn_string_literal)) = "E1032A020";
static const char data_2CAE2[10] __attribute__((scn_string_literal)) = "E1032A021";
static const char data_2CAEC[10] __attribute__((scn_string_literal)) = "E1032F001";
static const char data_2CAF6[10] __attribute__((scn_string_literal)) = "E1032F002";
static const char data_2CB00[10] __attribute__((scn_string_literal)) = "E1032F003";
static const char data_2CB0A[10] __attribute__((scn_string_literal)) = "E1032A022";
static const char data_2CB14[10] __attribute__((scn_string_literal)) = "E1032F004";
static const char data_2CB1E[10] __attribute__((scn_string_literal)) = "E1032A011";
static const char data_2CB28[10] __attribute__((scn_string_literal)) = "E1032F005";
static const char data_2CB32[10] __attribute__((scn_string_literal)) = "E1032A012";
static const char data_2CB3C[10] __attribute__((scn_string_literal)) = "E1032A013";
static const char data_2CB46[10] __attribute__((scn_string_literal)) = "E1032F006";
static const char data_2CB50[10] __attribute__((scn_string_literal)) = "E1032A023";
static const char data_2CB5A[10] __attribute__((scn_string_literal)) = "E1032A024";
static const char data_2CB64[10] __attribute__((scn_string_literal)) = "E1032F007";
static const char data_2CB6E[10] __attribute__((scn_string_literal)) = "E1032A025";
static const char data_2CB78[10] __attribute__((scn_string_literal)) = "E1032A026";
static const char data_2CB82[10] __attribute__((scn_string_literal)) = "E1032F008";
static const char data_2CB8C[10] __attribute__((scn_string_literal)) = "E1032F009";
static const char data_2CB96[10] __attribute__((scn_string_literal)) = "E1032F010";
static const char data_2CBA0[10] __attribute__((scn_string_literal)) = "E1032F011";
static const char data_2CBAA[10] __attribute__((scn_string_literal)) = "E1032F012";
static const char data_2CBB4[10] __attribute__((scn_string_literal)) = "E1032F013";
static const char data_2CBBE[10] __attribute__((scn_string_literal)) = "E1032F014";
static const char data_2CBC8[10] __attribute__((scn_string_literal)) = "E1032F015";
static const char data_2CBD2[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_2CBDC[10] __attribute__((scn_string_literal)) = "E1032A018";
static const char data_2CBE6[10] __attribute__((scn_string_literal)) = "E1032A027";
static const char data_2CBF0[10] __attribute__((scn_string_literal)) = "E1032A034";
static const char data_2CBFA[10] __attribute__((scn_string_literal)) = "E1032H005";
static const char data_2CC04[10] __attribute__((scn_string_literal)) = "E1032H004";
static const char data_2CC0E[10] __attribute__((scn_string_literal)) = "E1032H009";
static const char data_2CC18[10] __attribute__((scn_string_literal)) = "E1032H006";
static const char data_2CC22[10] __attribute__((scn_string_literal)) = "E1032A042";
static const char data_2CC2C[10] __attribute__((scn_string_literal)) = "E1032A040";
static const char data_2CC36[10] __attribute__((scn_string_literal)) = "E1032A043";
static const char data_2CC40[10] __attribute__((scn_string_literal)) = "E1032A041";
static const char data_2CC4A[10] __attribute__((scn_string_literal)) = "E1032A045";
static const char data_2CC54[10] __attribute__((scn_string_literal)) = "E1032A046";
static const char data_2CC5E[10] __attribute__((scn_string_literal)) = "E1032A047";
static const char data_2CC68[10] __attribute__((scn_string_literal)) = "E1032A046";
static const char data_2CC72[10] __attribute__((scn_string_literal)) = "E1032A047";
static const char data_2CC7C[10] __attribute__((scn_string_literal)) = "E1032A044";
static const char data_2CC86[10] __attribute__((scn_string_literal)) = "E1032A051";
static const char data_2CC90[10] __attribute__((scn_string_literal)) = "E1032A039";
static const char data_2CC9A[10] __attribute__((scn_string_literal)) = "E1032H007";
static const char data_2CCA4[10] __attribute__((scn_string_literal)) = "E1032A120";
static const char data_2CCAE[10] __attribute__((scn_string_literal)) = "E1032H010";
static const char data_2CCB8[10] __attribute__((scn_string_literal)) = "E1032A066";
static const char data_2CCC2[10] __attribute__((scn_string_literal)) = "E1032H011";
static const char data_2CCCC[10] __attribute__((scn_string_literal)) = "E1032J001";
static const char data_2CCD6[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_2CCE0[10] __attribute__((scn_string_literal)) = "E1032A131";
static const char data_2CCEA[10] __attribute__((scn_string_literal)) = "E1032J010";
static const char data_2CCF4[10] __attribute__((scn_string_literal)) = "E1032J013";
static const char data_2CCFE[10] __attribute__((scn_string_literal)) = "E1032J001";
static const char data_2CD08[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_2CD12[10] __attribute__((scn_string_literal)) = "E1032A134";
static const char data_2CD1C[10] __attribute__((scn_string_literal)) = "E1032J015";
static const char data_2CD26[10] __attribute__((scn_string_literal)) = "E1032A135";
static const char data_2CD30[10] __attribute__((scn_string_literal)) = "E1032J001";
static const char data_2CD3A[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_2CD44[10] __attribute__((scn_string_literal)) = "E1032J018";
static const char data_2CD4E[10] __attribute__((scn_string_literal)) = "E1032A163";
static const char data_2CD58[10] __attribute__((scn_string_literal)) = "E1032J019";
static const char data_2CD62[10] __attribute__((scn_string_literal)) = "E1032J020";
static const char data_2CD6C[10] __attribute__((scn_string_literal)) = "E1032A068";
static const char data_2CD76[10] __attribute__((scn_string_literal)) = "E1032J016";
static const char data_2CD80[10] __attribute__((scn_string_literal)) = "E1032A161";
static const char data_2CD8A[10] __attribute__((scn_string_literal)) = "E1032J017";
static const char data_2CD94[10] __attribute__((scn_string_literal)) = "E1032A162";
static const char data_2CD9E[10] __attribute__((scn_string_literal)) = "E1032J001";
static const char data_2CDA8[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_2CDB2[10] __attribute__((scn_string_literal)) = "E1032A131";
static const char data_2CDBC[10] __attribute__((scn_string_literal)) = "E1032J010";
static const char data_2CDC6[10] __attribute__((scn_string_literal)) = "E1032A132";
static const char data_2CDD0[10] __attribute__((scn_string_literal)) = "E1032J011";
static const char data_2CDDA[10] __attribute__((scn_string_literal)) = "E1032J012";
static const char data_2CDE4[10] __attribute__((scn_string_literal)) = "E1032H007";
static const char data_2CDEE[10] __attribute__((scn_string_literal)) = "E1032A120";
static const char data_2CDF8[10] __attribute__((scn_string_literal)) = "E1032H010";
static const char data_2CE02[10] __attribute__((scn_string_literal)) = "E1032A066";
static const char data_2CE0C[10] __attribute__((scn_string_literal)) = "E1032H011";
static const char data_2CE16[10] __attribute__((scn_string_literal)) = "E1032J001";
static const char data_2CE20[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_2CE2A[10] __attribute__((scn_string_literal)) = "E1032J018";
static const char data_2CE34[10] __attribute__((scn_string_literal)) = "E1032A163";
static const char data_2CE3E[10] __attribute__((scn_string_literal)) = "E1032J019";
static const char data_2CE48[10] __attribute__((scn_string_literal)) = "E1032J020";
static const char data_2CE52[10] __attribute__((scn_string_literal)) = "E1032A068";
static const char data_2CE5C[10] __attribute__((scn_string_literal)) = "E1032J016";
static const char data_2CE66[10] __attribute__((scn_string_literal)) = "E1032A161";
static const char data_2CE70[10] __attribute__((scn_string_literal)) = "E1032J017";
static const char data_2CE7A[10] __attribute__((scn_string_literal)) = "E1032A162";
static const char data_2CE84[10] __attribute__((scn_string_literal)) = "E1032J001";
static const char data_2CE8E[10] __attribute__((scn_string_literal)) = "E1032A067";
static const char data_2CE98[10] __attribute__((scn_string_literal)) = "E1032J002";
static const char data_2CEA2[10] __attribute__((scn_string_literal)) = "E1032A068";
static const char data_2CEAC[10] __attribute__((scn_string_literal)) = "E1032J001";
static const char data_2CEB6[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_2CEC0[10] __attribute__((scn_string_literal)) = "E1032J003";
static const char data_2CECA[10] __attribute__((scn_string_literal)) = "E1032A069";
static const char data_2CED4[10] __attribute__((scn_string_literal)) = "E1032J004";
static const char data_2CEDE[10] __attribute__((scn_string_literal)) = "E1032A070";
static const char data_2CEE8[10] __attribute__((scn_string_literal)) = "E1032H007";
static const char data_2CEF2[10] __attribute__((scn_string_literal)) = "E1032A048";
static const char data_2CEFC[10] __attribute__((scn_string_literal)) = "E1032H008";
static const char data_2CF06[10] __attribute__((scn_string_literal)) = "E1032A049";
static const char data_2CF10[10] __attribute__((scn_string_literal)) = "E1032A050";
static const char data_2CF1A[16] __attribute__((scn_string_literal)) = "/scene/03/JD00/";
static const char data_2CF2A[5] __attribute__((scn_string_literal)) = "TBOX";
static const char data_2CF2F[5] __attribute__((scn_string_literal)) = "TBOX";
static const uint32_t data_2CF34 = 0xC18DE1B1;
static const float data_2CF38[2] = {0.225f, -0.0935f};
static const int32_t data_2CF40[3] = {63169, 1048832, 6};
static const float data_2CF4C[3] = {-17.4023f, 1.574f, -1.5519f};
static const int32_t data_2CF58[3] = {30401, 264, 6};
static const float data_2CF64[3] = {-10.4327f, 0.225f, 21.8722f};
static const int32_t data_2CF70[3] = {16384, 1048832, 5};
static const float data_2CF7C[2] = {-11.6312f, 1.1495f};
static const uint32_t data_2CF84 = 0x41AF1FBEU;
static const int32_t data_2CF88[3] = {49152, 264, 5};
static const float data_2CF94[3] = {-14.4036f, 0.225f, 7.0609f};
static const int32_t data_2CFA0[3] = {16384, 1048576, 5};
static const float data_2CFAC[3] = {-15.5966f, 1.0f, 7.0721f};
static const int32_t data_2CFB8[3] = {49152, 8, 5};
static const uint32_t data_2CFC4 = 0xC1C4F247U;
static const float data_2CFC8[2] = {1.2587f, 6.2984f};
static const int32_t data_2CFD0[3] = {32768, 1048576, 8};
static const uint32_t data_2CFDC = 0xC1C4E944U;
static const float data_2CFE0[3] = {3.0927f, 8.4006f, 0.0f};
static const int32_t data_2CFEC[2] = {8, 8};
static const float data_2CFF4[3] = {14.462f, 0.2094f, 28.3062f};
static const int32_t data_2D000[3] = {64261, 1048576, 3};
static const float data_2D00C[3] = {14.6576f, 0.6583f, 27.645f};
static const int32_t data_2D018[3] = {31493, 8, 3};
static const float data_2D024[2] = {-13.8104f, 1.3233f};
static const uint32_t data_2D02C = 0x41C3D8AEU;
static const int32_t data_2D030[3] = {30219, 1064960, 12};
static const float data_2D03C[2] = {-14.6088f, 3.8117f};
static const uint32_t data_2D044 = 0x41DD73EBU;
static const int32_t data_2D048[3] = {62987, 16392, 12};
static const float data_2D054[2] = {-13.0535f, 1.3156f};
static const uint32_t data_2D05C = 0x41F68D84U;
static const int32_t data_2D060[3] = {32768, 1064960, 12};
static const float data_2D06C[4] = {-13.0292f, 3.8062f, 34.1213f, 0.0f};
static const int32_t data_2D07C[2] = {16392, 12};
static const float data_2D084 = 100000.0f;
static const uint8_t data_2D088[8] = {0};
typedef struct Row_2D090 { float w0; int32_t w1; int32_t w2; } Row_2D090;
static const Row_2D090 data_2D090[3] = {
  {0.5f, 0, 1},
  {0.5f, 0, 2},
  {0.4f, 0, 3}
};
static const float data_2D0B4 = 0.4f;
static const int32_t data_2D0B8 = 3;
static const char data_2D0BC[16] __attribute__((scn_string_literal)) = "/scene/03/JD00/";
static const char data_2D0CC[5] __attribute__((scn_string_literal)) = "MAPY";
static const char data_2D0D1[16] __attribute__((scn_string_literal)) = "/scene/03/JD00/";
static const char data_2D0E1[5] __attribute__((scn_string_literal)) = "MAPN";
static const char data_2D0E6[16] __attribute__((scn_string_literal)) = "/scene/03/JD00/";
static const char data_2D0F6[5] __attribute__((scn_string_literal)) = "MAPS";
static const char data_2D0FB[16] __attribute__((scn_string_literal)) = "/scene/03/JD00/";
static const char data_2D10B[5] __attribute__((scn_string_literal)) = "JDYA";
static const char data_2D110[5] __attribute__((scn_string_literal)) = "JDYA";
static const char data_2D115[1] __attribute__((scn_string_literal)) = "";
static const char data_2D116[5] __attribute__((scn_string_literal)) = "JDYA";
static const char data_2D11B[1] __attribute__((scn_string_literal)) = "";
static const char data_2D11C[8] __attribute__((scn_string_literal)) = "JDYA";
static void (*data_2D124)(void) = script_221CC;
static void (*const data_2D128)(int32_t) = script_221CF;
static void (*const data_2D12C)(int32_t) = script_221F2;
static void (*const data_2D130)(int32_t) = script_22215;
static const Vector4 data_2D134 = {.x = 1.0f, .y = 0.4f, .z = 0.4f, .w = 0.4f};
static const Vector4 data_2D144 = {.x = 1.0f, .y = 0.6f, .z = 0.6f, .w = 0.6f};
static const char data_2D154[16] __attribute__((scn_string_literal)) = "/scene/03/JD00/";
static const char data_2D164[5] __attribute__((scn_string_literal)) = "DNOZ";
static const char data_2D169[5] __attribute__((scn_string_literal)) = "DNOZ";
static const char data_2D16E[1] __attribute__((scn_string_literal)) = "";
static const char data_2D16F[5] __attribute__((scn_string_literal)) = "DNOZ";
static const char data_2D174[1] __attribute__((scn_string_literal)) = "";
static const char data_2D175[7] __attribute__((scn_string_literal)) = "01FULB";
static const char data_2D17C[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_2D181[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_2D186[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_2D18B[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_2D190[11] __attribute__((scn_string_literal)) = "bgm068.snd";
static const char data_2D19B[13] __attribute__((scn_string_literal)) = "a1_nonam.snd";
static const FogColourData data_2D1A8 = {.red = 16.7f, .green = 780.0f, .blue = 1.0f, .colour = 0xFF0D0705U};
static const uint32_t data_2D1B8[4] = {0x320E102EU, 0x320E102EU, 0x64555462U, 0x64555462U};
static const Vector4 data_2D1C8 = {.x = 0.25f, .y = 0.09f, .z = 0.19f, .w = 0.31f};
static const Vector4 data_2D1D8 = {.x = 1.0f, .y = 0.8f, .z = 0.63f, .w = 0.4f};
static const float data_2D1E8[2] = {0.69f, 1.0f};
static const Vector4 data_2D1F0 = {.x = 1.0f, .y = 0.4f, .z = 0.4f, .w = 0.4f};
static const Vector4 data_2D200 = {.x = 1.0f, .y = 0.6f, .z = 0.6f, .w = 0.6f};
static const char data_2D210[] __attribute__((scn_string_literal)) = "sprite";
static const char data_2D217[13] __attribute__((scn_string_literal)) = "SCROLL27.SPR";
static const int32_t data_2D224[57] = {
  0, 0, -6189, 0, 1638, -3822, 0, 546, -546, 0, -1820, -12925, 0, 0, -9466, 0, -546, -4187, 0, 0, -1274, 0, 546, -14017, 0, 0, -14017, 0, 0, -4551, 0,
  0, -1092, 0, 728, -13289, 0, -1274, -11468, 0, 1638, -5825, 0, 0, -1092, 0, -1638, -9830, 0, -364, -6189, 0, -2002, -6007, 0, 182, -546
};
static const int32_t data_2D308[57] = {
  0, 0, -6189, 0, 1638, -3822, 0, 546, -546, 0, -1820, -14199, 0, 0, -16384, 0, 910, -8009, 0, -182, -546, 0, 546, -7827, 0, 0, -13471, 0, 0, -10376,
  0, 0, -182, 0, 728, -8738, 0, 0, -14199, 0, 0, -9284, 0, 0, -546, 0, -364, -7281, 0, 0, -11832, 0, -3458, -8738, 0, -364, 182
};
static const char data_2D3EC[6] __attribute__((scn_string_literal)) = "YKB_M";
static const char data_2D3F2[6] __attribute__((scn_string_literal)) = "YKG_M";
static const Vector4 data_2D3F8 = {.x = 0.3f, .y = 0.3f, .z = 0.3f, .w = 0.3f};
static const Vector4 data_2D408 = {.x = 0.3f, .y = 0.3f, .z = 0.3f, .w = 0.3f};
static const Vector4 data_2D418 = {.x = 0.3f, .y = 0.3f, .z = 0.3f, .w = 0.3f};
static uint32_t data_2D428 = 152;
static const int32_t data_2D42C = 153;
static const char data_2D430[5] __attribute__((scn_string_literal)) = "BIKE";
static const char data_2D435[] __attribute__((scn_string_literal)) = "DAT0";
static const uint8_t data_2D43A[2] = {0};
static const FogColourData data_2D43C = {.red = 0.1f, .green = 1055.0f, .blue = 0.41f, .colour = 0xFF1B214FU};
static const uint32_t data_2D44C[4] = {0xFFFFFFU, 0xFFFFFFU, 0x20AFB7E7U, 0x20AFB7E7U};
static const char data_2D45C[] __attribute__((scn_string_literal)) = "sprite";
static const char data_2D463[13] __attribute__((scn_string_literal)) = "SCROLL27.SPR";
static const char data_2D470[5] __attribute__((scn_string_literal)) = "IBIK";
static const char data_2D475[5] __attribute__((scn_string_literal)) = "IBIK";
static const char data_2D47A[16] __attribute__((scn_string_literal)) = "/scene/03/JD00/";
static const char data_2D48A[5] __attribute__((scn_string_literal)) = "BIKE";
static const char data_2D48F[5] __attribute__((scn_string_literal)) = "BIKE";
static const char data_2D494[1] __attribute__((scn_string_literal)) = "";
static const char data_2D495[7] __attribute__((scn_string_literal)) = "01BIKE";
static const char data_2D49C[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_2D4A1[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_2D4A6[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_2D4AB[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_2D4B0[11] __attribute__((scn_string_literal)) = "bgm200.snd";
static const char data_2D4BB[13] __attribute__((scn_string_literal)) = "a1_bikek.snd";
static const char data_2D4C8[11] __attribute__((scn_string_literal)) = "M_MOBJ.BIN";
static const char data_2D4D3[16] __attribute__((scn_string_literal)) = "/scene/03/JD00/";
static const char data_2D4E3[5] __attribute__((scn_string_literal)) = "JD00";
static const char data_2D4E8[16] __attribute__((scn_string_literal)) = "/scene/03/JD00/";
static const char data_2D4F8[5] __attribute__((scn_string_literal)) = "IBIK";
static const char data_2D4FD[14] __attribute__((scn_string_literal)) = "scene/03/D000";
static const char data_2D50B[] __attribute__((scn_string_literal)) = "SCRTEST.BIN";
static const uint8_t data_2D517[1] = {0};
static const uint32_t data_2D518 = 380;
static const int32_t data_2D51C[6] = {1, 390, 0, 1018, 0, -1};
static const uint32_t data_2D534 = 510;
static const int32_t data_2D538[6] = {1, 525, 0, 1020, 0, -1};
static const uint32_t data_2D550 = 150;
static const int32_t data_2D554[6] = {1, 160, 0, 1019, 0, -1};
static const uint32_t data_2D56C = 530;
static const int32_t data_2D570[6] = {1, 970, 0, 1014, 0, -1};
static const uint32_t data_2D588 = 911;
static const int32_t data_2D58C[6] = {1, 155, 0, 1015, 0, -1};
static const char data_2D5A4[7] __attribute__((scn_string_literal)) = "FREE02";
static const char data_2D5AB[7] __attribute__((scn_string_literal)) = "FREE03";
static const char data_2D5B2[7] __attribute__((scn_string_literal)) = "FREE04";
static const char data_2D5B9[7] __attribute__((scn_string_literal)) = "FREE01";
static const uint32_t data_2D5C0 = 0xC1F00000;
static const float data_2D5C4[2] = {0.0f, 30.0f};
static const uint32_t data_2D5CC = 0x40C00000;
static const float data_2D5D0[2] = {0.0f, 44.0f};
static const uint32_t data_2D5D8 = 0x41100000;
static const float data_2D5DC[2] = {0.0f, 35.0f};
static const uint32_t data_2D5E4 = 0x41E00000;
static const float data_2D5E8[2] = {0.0f, 45.0f};
static const uint32_t data_2D5F0 = 0xC2480000;
static const uint8_t pad_2D5F4[8] = {0};
static const uint32_t data_2D5FC = 0x40E00000;
static const float data_2D600[2] = {0.0f, -2.0f};
static const uint32_t data_2D608 = 0x41400000;
static const float data_2D60C[2] = {0.0f, -22.0f};
static const uint8_t data_2D614[12] = {0};
static const uint8_t data_2D620[12] = {0};
static const uint8_t data_2D62C[12] = {0};
static const uint8_t data_2D638[12] = {0};
static const uint32_t data_2D644 = 0x3F800000;
static const float data_2D648[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.8f, 0.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint32_t data_2D6CC = 0x3F800000;
static const float data_2D6D0[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.8f, 0.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint32_t data_2D754 = 0x3F800000;
static const float data_2D758[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.8f, 0.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint32_t data_2D7DC = 0x3F800000;
static const float data_2D7E0[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.8f, 0.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint32_t data_2D864 = 0x3F800000;
static const float data_2D868[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.8f, 0.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint32_t data_2D8EC = 0x3F800000;
static const float data_2D8F0[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.8f, 0.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint32_t data_2D974 = 0x3F800000;
static const float data_2D978[33] = {
  0.944f, 0.866f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 1.0f, 1.0f, 1.0f,
  0.8f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint8_t data_2D9FC[80] = {0};
typedef struct Row_2DA4C { float w0; float w1; float w2; int32_t w3; } Row_2DA4C;
static const Row_2DA4C data_2DA4C[3] = {
  {0.8f, 0.8f, 0.8f, 0},
  {0.0f, 0.0f, 0.0f, 0},
  {0.0f, 0.0f, 0.0f, 0}
};
static const int32_t data_2DA7C[2] = {0, 0};
static const uint8_t data_2DA84[96] = {0};
static const float data_2DAE4[10] = {0.8f, 0.8f, 0.8f, 0.8f, 0.8f, 0.8f, 0.8f, 0.8f, 0.8f, 0.8f};
static const uint32_t data_2DB0C = 0x3F800000;
static const float data_2DB10[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint32_t data_2DB94 = 0x3F800000;
static const float data_2DB98[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const char data_2DC1C[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_2DC21[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_2DC26[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_2DC2B[7] __attribute__((scn_string_literal)) = "SELECT";
static const char data_2DC32[14] __attribute__((scn_string_literal)) = "f1jutadn.snd";
static const uint32_t data_2DC40 = 0xC14170A4;
static const float data_2DC44[123] = {
  36.17f, -20.88f, 37.92f, -20.91f, 31.44f, -12.46f, 30.47f, 0.0f, 7.5f, -16.44f, 3.5f, 34.86f, 4.5f, -14.1f, 29.92f, -23.16f, 29.44f, -21.72f,
  23.58f, -13.13f, 24.21f, 0.0f, 7.5f, -17.94f, 3.5f, 26.67f, 4.5f, -21.26f, 7.45f, -16.64f, 6.81f, -16.22f, 9.79f, -20.83f, 10.43f, 0.0f, 4.26f,
  -18.73f, 2.12f, 8.62f, 1.5f, -25.96f, 8.81f, -25.84f, 10.81f, -28.21f, 10.97f, -28.33f, 8.97f, 1.93f, 5.9f, -26.09f, 3.93f, 9.89f, 2.0f, -33.09f,
  -11.92f, -23.99f, -10.08f, -25.55f, -0.33f, -35.09f, -2.31f, 1.8f, 8.49f, -29.43f, 4.73f, -6.16f, 4.7f, -19.71f, -2.82f, -15.43f, -8.92f, -13.51f,
  -8.48f, -15.02f, -1.77f, 0.57f, 4.86f, -15.94f, 2.57f, -3.23f, 2.5f, -12.19f, -0.23f, -9.49f, 0.5f, -10.42f, 3.13f, -12.52f, 2.42f, -1.0f, 2.95f,
  -11.11f, 1.07f, 1.16f, 1.5f, -5.74f, 5.17f, -3.55f, 6.11f, -6.66f, 12.87f, -8.86f, 11.92f, -1.0f, 3.22f, -6.2f, 1.19f, 9.02f, 1.3f, 0.01f, 24.91f,
  -5.85f, 24.91f, -5.85f, 23.14f, 0.01f, 23.14f, -1.0f, 2.99f, -2.91f, 1.0f
};
static const uint32_t data_2DE30 = 0x41C00419U;
static const float data_2DE34[128] = {
  2.5f, 1.22f, 22.82f, 3.06f, 22.82f, 3.06f, 24.38f, 1.22f, 24.38f, -1.0f, 3.1f, 2.15f, 1.0f, 23.6f, 1.0f, 5.17f, 27.96f, 5.17f, 22.75f, 9.74f,
  22.75f, 9.74f, 27.96f, -1.0f, 6.51f, 7.46f, 1.44f, 26.52f, 1.5f, 1.4f, 32.35f, 8.63f, 32.34f, 8.63f, 34.07f, 1.4f, 34.0f, -1.0f, 3.86f, 6.02f, 1.0f,
  33.21f, 2.5f, 8.64f, 40.58f, 13.25f, 35.48f, 16.11f, 38.07f, 11.5f, 43.17f, -1.0f, 3.3f, 12.38f, 1.92f, 39.33f, 3.5f, 22.06f, 36.09f, 20.86f,
  34.48f, 22.48f, 33.26f, 23.68f, 34.87f, -1.0f, 3.02f, 22.27f, 1.0f, 34.67f, 1.0f, 13.63f, 23.94f, 13.63f, 17.5f, 17.6f, 17.58f, 17.6f, 23.94f,
  -1.0f, 3.95f, 15.61f, 1.0f, 20.75f, 1.97f, 18.4f, 7.02f, 22.18f, 7.02f, 22.18f, 9.74f, 18.4f, 9.74f, -1.0f, 3.3f, 20.29f, 1.35f, 8.38f, 1.35f,
  23.86f, 11.68f, 24.53f, 7.0f, 29.56f, 7.7f, 28.89f, 12.39f, -1.0f, 3.3f, 26.71f, 2.35f, 9.69f, 2.5f, 6.93f, 16.18f, 9.35f, 16.18f, 9.35f, 18.02f,
  6.93f, 18.02f, -1.0f, 3.01f, 8.14f, 1.0f, 17.1f, 1.0f, 9.9f
};
static const shen_name data_2E034 = SHEN_NAME('33KA');
static const float data_2E038 = 4.64f;
static const shen_name data_2E03C = SHEN_NAME('33KA');
static const float data_2E040[38] = {
  4.64f, 7.09f, 9.9f, 7.09f, -1.0f, 5.86f, 7.27f, 1.0f, 9.9f, 2.62f, 4.87f, 1.89f, 5.03f, 0.1f, 7.43f, 0.32f, 7.26f, 2.11f, -1.21f, 3.05f, 6.15f,
  1.0f, 1.1f, 1.0f, 5.27f, -13.42f, 5.27f, -16.92f, 9.88f, -17.0f, 9.88f, -13.42f, -1.0f, 3.3f, 7.58f, 1.75f, -15.17f, 1.75f
};
