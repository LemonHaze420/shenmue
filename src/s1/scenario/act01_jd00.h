#pragma once

#include "scenario.h"
#include "motion/motion.h"
#include "scene/01/jd00/m_fgt1.h"

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
static int32_t script_FE5B(int32_t arg0, int32_t arg1);
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
static void script_13E9B(int32_t arg0);
static int32_t script_13FA8(int32_t arg0);
static int32_t script_14003(void);
static int32_t script_14145(void);
static void script_141EB(void);
static int32_t script_14244(void);
static int32_t script_1427E(int32_t arg0);
static void script_142A1(void);
static int32_t script_142C1(int32_t arg0);
static void script_1430C(void);
static void script_14353(void);
static void script_143A6(void);
static void script_14402(void);
static void script_14449(void);
static void script_144A5(void);
static void script_14573(void);
static void script_14605(void);
static void script_14642(void);
static int32_t script_1467F(int32_t arg0);
static void script_146F0(int32_t arg0);
static int32_t script_1490B(void);
static void script_14953(void);
static void script_1495B(void);
static void script_1496F(void);
static void script_14C71(int32_t arg0, int32_t arg1);
static void script_14CD0(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static int32_t script_14CF7(float arg0, float arg1, float arg2, float arg3);
static int32_t script_14E95(void);
static void script_14F5C(const char *arg0, int32_t arg1);
static void script_14FA9(void);
static int32_t script_150E0(int32_t arg0);
static void script_15159(void);
static int8_t script_151BB(void);
static void script_15206(int32_t arg0);
static void script_1526F(int32_t arg0);
static void script_1529A(void);
static int32_t script_152C1(int32_t arg0);
static int8_t script_15303(int32_t arg0);
static void script_153DB(void);
static void script_153E8(void);
static void script_15852(void);
static void script_163B7(void);
static void script_16475(void);
static void script_1649A(void);
static void script_16BE1(void);
static int8_t script_18C68(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int8_t script_18D21(void);
static void script_18ED7(int32_t arg0);
static int8_t script_1A048(void);
static void script_1A5DD(int32_t arg0);
static void script_1E49B(void);
static int8_t script_1EF63(void);
static int8_t script_1EF8B(void);
static void script_1F0A6(int32_t arg0);
static int32_t script_1F18F(int32_t arg0, int32_t arg1);
static int32_t script_1F1F8(void);
static void script_1F213(int32_t arg0);
static void script_1F2FC(int32_t arg0);
static void script_1F4EA(void);
static void script_1F521(int32_t arg0);
static int8_t script_1F5E9(void);
static void script_202C6(void);
static void script_20389(void);
static void script_206B1(void);
static void script_20736(void);
static void script_207C5(void);
static int32_t script_209DB(void);
static int32_t script_20A90(void);
static void script_21412(void);
static void script_21511(void);
static void script_21584(void);
static void script_2176D(void);
static void script_21781(void);
static void script_21849(void);
static int32_t script_2198E(void);
static void script_21A11(int32_t arg0);
static void script_21CA6(void);
static void script_21CFC(void);
static int32_t script_21EC0(void);
static int8_t script_2211A(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_221D3(void);
static int32_t script_2220F(void);
static void script_224A4(void);
static void script_224C3(void);
static void script_224D1(void);
static void script_224D4(int32_t arg0);
static void script_224F7(int32_t arg0);
static void script_2251A(int32_t arg0);
static void script_2253D(void);
static void script_225A4(void);
static int32_t script_225C0(void);
static void script_22694(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_2272B(void);
static int32_t script_22753(void);
static int8_t script_2276D(void);
static int32_t script_22878(void);
static int32_t script_2294E(void);
static int32_t script_22A63(void);
static int32_t script_22B86(void);
static int32_t script_22C53(void);
static int32_t script_22CE1(void);
static void script_22E75(int32_t arg0);
static int32_t script_22EF3(int32_t arg0, int32_t arg1);
static int32_t script_22F05(int32_t arg0, int32_t arg1);
static void script_22FFE(void);
static int32_t script_23024(void);
static void script_2331E(int32_t arg0);
static void script_23359(int32_t arg0);
static void script_23383(int32_t arg0);
static void script_23460(int32_t arg0);
static void script_235C9(void);
static void script_236D4(void);
static void script_23722(int32_t arg0);
static void script_2375D(int32_t arg0);
static int32_t script_23787(void);
static void script_237C3(int32_t arg0);
static void script_23AFE(void);
static void script_23C5C(int32_t arg0, int32_t arg1);
static void script_23CC3(int32_t arg0, int32_t arg1);
static int32_t script_23CEF(int32_t arg0);
static int32_t script_23D9A(int32_t arg0);
static int32_t script_23E7F(int32_t arg0);
static void script_23F9B(void);
static void script_2402C(void);
static void script_240F1(void);
static void script_241F5(void);
static int32_t script_24241(int32_t arg0);
static int8_t script_24604(void);
static void script_2465B(int32_t arg0);
static void script_2473A(void);
static void script_247F5(void);
static int32_t script_2489D(int32_t arg0);
static int32_t script_249EB(int32_t arg0);
static int32_t script_24B02(int32_t arg0);
static void script_24C6D(int32_t arg0, int32_t arg1);
static int32_t script_24E5F(int32_t arg0);
static int32_t script_24F1B(int32_t arg0);
static void script_24FA8(void);
static int32_t script_2519A(int32_t arg0);
static int32_t script_251E3(int32_t arg0);
static int32_t script_2526D(int32_t arg0);
static int32_t script_255AC(int32_t arg0);
static int8_t script_25937(void);
static int32_t script_25979(int32_t arg0, int32_t arg1);
static void script_25AE7(void);
static void script_25AEA(int32_t arg0);
static void script_25C86(int32_t arg0);
static int32_t script_2704B(void);
static int32_t script_27139(void);
static int32_t script_271CA(void);
static int32_t script_27261(void);
static int32_t script_27297(void);
static void script_274B6(void);
static int32_t script_27865(void);
static int32_t script_282B8(void);
static int32_t script_29B97(void);
static void script_29BDD(int32_t arg0);
static void script_2A1E2(void);
static int32_t script_2A27E(void);
static int32_t script_2A2C0(void);
static int32_t script_2A887(void);
static int32_t script_2AD2B(void);
static int32_t script_2AF9D(void);
static int32_t script_2B3F8(void);
static int32_t script_2B4BF(void);
static void script_2B5B3(int32_t arg0);
static int32_t script_2B671(void);
static int32_t script_2BB7A(void);
static int32_t script_2BBDE(void);
static int32_t script_2BC86(void);
static int32_t script_2BD94(void);
static void script_2BDAA(void);
static void script_2BE13(int32_t arg0);
static int32_t script_2BF86(void);
static void script_2BFA7(int32_t arg0);
static int32_t script_2BFBE(void);
static int32_t script_2C206(void);
static void script_2C3C6(int32_t arg0);
static void script_2C423(int32_t arg0, int32_t arg1);
static int32_t script_2C44B(void);
static int32_t script_2C6DE(void);
static int32_t script_2C798(void);
static int32_t script_2C825(void);
static void script_2C8B2(int32_t arg0);
static void script_2C928(int32_t arg0);
static void script_2CBE9(int32_t arg0);
static void script_2CE36(void);
static void script_2D14B(void);
static void script_2D391(int32_t arg0);
static void script_2D3CC(int32_t arg0);
static void script_2D3F6(int32_t arg0, float arg1, float arg2, float arg3, float arg4);
static int32_t script_2D444(int32_t arg0);
static void script_2D4C8(int32_t arg0, int32_t arg1);
static void script_2D4EE(int32_t arg0);
static void script_2D520(void);
static void script_2D781(void);
static void script_2D9F5(void);
static int32_t script_2E40E(void);
static void script_2E591(const char *arg0, const char *arg1);
static void script_2E5D3(void);
static void script_2E6C8(void);
static void script_2E7A6(void);
static void script_2E8D1(void);
void script_2E9D1(void);
static int32_t script_2ED1E(void);
static void script_2ED43(void);
static void script_2F0A1(void);
static void script_2F0D9(void);
static void script_2F211(int32_t arg0);
static void script_2F21D(int32_t arg0);
static void script_2F229(int32_t arg0);
static void script_2F335(int32_t arg0);
static void script_2F34C(int32_t arg0);
static void script_2F38D(int32_t arg0);
static void script_2F3D1(int32_t arg0);
static void script_2F44F(void);
static void script_2F4CE(void);
static void script_2F539(int32_t arg0, int32_t arg1);
static void script_2F5A0(int32_t arg0, int32_t arg1);
static void script_2F5CC(void);
static void script_2F607(void);
static void script_2F61A(void);
static void script_2F626(void);
static void script_2F7BF(void);
static void script_2F828(void);

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
static Vector3 task_2A0;
static Vector3 task_2AC;
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
static int8_t task_3B0;
static uint8_t task_3B1;
static uint16_t task_3B2;
static int8_t task_3B4;
static uint8_t task_3B5;
static uint16_t task_3B6;
static int8_t task_3B8;
static uint8_t task_3B9;
static uint16_t task_3BA;
static int32_t task_3BC;
static int32_t task_3C0;
static GameDateTime task_3C4;
static int32_t task_3CC;
static int32_t task_3D0;
static int32_t task_3D4[3];
static int32_t task_3E0;
static int32_t task_3E4;
static int32_t task_3E8;
static float task_3EC;
static int32_t task_3F0;
static int32_t task_3F4;
static int8_t task_3F8;
static int8_t task_3F9;
static int8_t task_3FA;
static int8_t task_3FB;
static int32_t task_3FC;
static int32_t task_400;
static Vector3 task_404;
static Vector3 task_410;
static float task_41C;
static int32_t task_420;
static int32_t task_424;
static Vector3 task_428;
static float task_434;
static int32_t task_438;
static int32_t task_43C;
static Vector3 task_440;
static float task_44C;
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
static int32_t task_484;
static int32_t task_488;
static int32_t task_48C;
static int32_t task_490;
static int32_t task_494;
static int32_t task_498;
static int32_t task_49C;
static int32_t task_4A0;
static int32_t task_4A4;
static int32_t task_4A8;
static int32_t task_4AC;
static int32_t task_4B0;
static int32_t task_4B4;
static int32_t task_4B8;
static GameDateTime task_4BC;
static int32_t task_4C4;
static int32_t task_4C8;
static int32_t task_4CC;
static int32_t task_4D0;
static int32_t task_4D4;
static int8_t task_4D8;
static uint8_t task_4D9;
static uint16_t task_4DA;
static int32_t task_4DC;
static int32_t task_4E0;
static int32_t task_4E4;
static int32_t task_4E8;
static float task_4EC;
static float task_4F0;
static int8_t task_4F4;
static uint8_t task_4F5;
static uint16_t task_4F6;
static int32_t task_4F8;
static const Vector3 data_2F860 = {.x = 1.0f, .y = 1.0f, .z = 1.0f};
static const int32_t data_2F86C[4] = {16777215, 16777215, 16777215, 16777215};
static const float data_2F87C[3] = {20.0f, 800.0f, 1.0f};
static const int32_t data_2F888 = -1;
static const char data_2F88C[10] __attribute__((scn_string_literal)) = "Kaidan : ";
static const char data_2F896[32] __attribute__((scn_string_literal)) = "========== initializing kaidan\012";
static const char data_2F8B6[48] __attribute__((scn_string_literal)) = "please ENDOFKDATA use can't start shuki kaidan\012";
static const char data_2F8E6[] __attribute__((scn_string_literal)) = "Kaidan Function Error 1\n";
static const char data_2F8FF[] __attribute__((scn_string_literal)) = "Kaidan Function Error 2\n";
static const char data_2F918[] __attribute__((scn_string_literal)) = "Kaidan Data Error\n";
static const char data_2F92B[10] __attribute__((scn_string_literal)) = "Kaidan : ";
static const char data_2F935[28] __attribute__((scn_string_literal)) = "wait ... shuki shuki shuki\012";
static const char data_2F951[10] __attribute__((scn_string_literal)) = "Kaidan : ";
static const char data_2F95B[24] __attribute__((scn_string_literal)) = "KAIDAN : Warning coli!\012";
static const char data_2F973[24] __attribute__((scn_string_literal)) = "KAIDAN : Warning coli!\012";
static const char data_2F98B[24] __attribute__((scn_string_literal)) = "KAIDAN : Warning coli!\012";
static const char data_2F9A3[24] __attribute__((scn_string_literal)) = "KAIDAN : Warning coli!\012";
static const char data_2F9BB[24] __attribute__((scn_string_literal)) = "KAIDAN : Warning coli!\012";
static const char data_2F9D3[24] __attribute__((scn_string_literal)) = "KAIDAN : Warning setY!\012";
static const char data_2F9EB[24] __attribute__((scn_string_literal)) = "KAIDAN : Warning setY!\012";
static const char data_2FA03[241] __attribute__((scn_string_literal)) = "Kaidan : ";
static const int32_t data_2FAF4[342] = {
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
  -1638, 0, 0, 1092
};
static const int32_t data_3004C[342] = {
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
static const char data_305A4[5] __attribute__((scn_string_literal)) = "SY11";
static const char data_305A9[9] __attribute__((scn_string_literal)) = "SY11A006";
static const char data_305B2[9] __attribute__((scn_string_literal)) = "SY11A007";
static const char data_305BB[9] __attribute__((scn_string_literal)) = "SY11A008";
static const char data_305C4[9] __attribute__((scn_string_literal)) = "SY11A002";
static const char data_305CD[9] __attribute__((scn_string_literal)) = "SY11A003";
static const char data_305D6[9] __attribute__((scn_string_literal)) = "SY11A004";
static const char data_305DF[9] __attribute__((scn_string_literal)) = "SY11A005";
static const char data_305E8[9] __attribute__((scn_string_literal)) = "SY11A001";
static const char data_305F1[9] __attribute__((scn_string_literal)) = "SY11A009";
static const char data_305FA[9] __attribute__((scn_string_literal)) = "SY11A010";
static const char data_30603[9] __attribute__((scn_string_literal)) = "SY11A011";
static const char data_3060C[9] __attribute__((scn_string_literal)) = "SY11A009";
static const char data_30615[5] __attribute__((scn_string_literal)) = "SY11";
static const char data_3061A[5] __attribute__((scn_string_literal)) = "SY11";
static const char data_3061F[9] __attribute__((scn_string_literal)) = "SY11A001";
static const char data_30628[5] __attribute__((scn_string_literal)) = "SY11";
static const char data_3062D[5] __attribute__((scn_string_literal)) = "SY11";
static const char data_30632[9] __attribute__((scn_string_literal)) = "SY11A009";
static const char data_3063B[9] __attribute__((scn_string_literal)) = "SY11A010";
static const char data_30644[9] __attribute__((scn_string_literal)) = "SY11A011";
static const char data_3064D[7] __attribute__((scn_string_literal)) = "SY11";
static const uint32_t data_30654 = SHEN_NAME('VM_0');
static const shen_name data_30658[7] = {
  SHEN_NAME('VM_1'), SHEN_NAME('VM_2'), SHEN_NAME('VM_3'), SHEN_NAME('VM_4'), SHEN_NAME('VM_5'), SHEN_NAME('VM_6'), SHEN_NAME('VM_7')
};
static const char data_30674[18] __attribute__((scn_string_literal)) = "/scene/01/stream/";
static const char data_30686[5] __attribute__((scn_string_literal)) = "%03d";
static const char data_3068B[] __attribute__((scn_string_literal)) = "ATRK";
static Vector3 data_30690 = {.x = 0.0f, .y = -0.1f, .z = 0.0f};
static Vector3 data_3069C = {.x = 0.0f, .y = 0.0f, .z = 0.0f};
static const char data_306A8[] __attribute__((scn_string_literal)) = "VEND";
static const char data_306AD[] __attribute__((scn_string_literal)) = "VEND";
static const char data_306B2[] __attribute__((scn_string_literal)) = "SPRT";
static const char data_306B7[7] __attribute__((scn_string_literal)) = "01JUCE";
static const char data_306BE[13] __attribute__((scn_string_literal)) = "HMOT_CAN.BIN";
static const char data_306CB[] __attribute__((scn_string_literal)) = "VEND";
static const char data_306D0[7] __attribute__((scn_string_literal)) = "01JUCE";
static const char data_306D7[22] __attribute__((scn_string_literal)) = "HandMot read by VEND\012";
static const char data_306ED[13] __attribute__((scn_string_literal)) = "HMOT_CAN.BIN";
static const char data_306FA[7] __attribute__((scn_string_literal)) = "01JUCE";
static const char data_30701[] __attribute__((scn_string_literal)) = "VEND";
static const uint8_t data_30706[2] = {0};
static const uint8_t data_30708[12] = {0};
static const char data_30714[29] __attribute__((scn_string_literal)) = "vendmachine exit is EV_STOP\012";
static const char data_30731[41] __attribute__((scn_string_literal)) = "vendmachine exit is MONEY_LOCK -> %d,%d\012";
static const char data_3075A[] __attribute__((scn_string_literal)) = "COKE";
static const char data_3075F[] __attribute__((scn_string_literal)) = "FATG";
static const char data_30764[] __attribute__((scn_string_literal)) = "FATO";
static const char data_30769[] __attribute__((scn_string_literal)) = "SPRT";
static const char data_3076E[] __attribute__((scn_string_literal)) = "CAFE";
static const uint8_t data_30773[1] = {0};
static const uint32_t data_30774 = 0x3D204C06;
static const float data_30778[2] = {0.0f, 0.890018f};
static const uint8_t data_30780[12] = {0};
static const uint8_t data_3078C[12] = {0};
static const uint32_t data_30798 = 0x3DE978D5;
static const float data_3079C[2] = {0.002f, -0.036f};
static const uint32_t data_307A4 = 8192;
static const int32_t data_307A8[2] = {11377, 3640};
static const uint32_t data_307B0 = 0x3D204C06;
static const float data_307B4[2] = {0.0f, 0.890018f};
static const uint8_t data_307BC[12] = {0};
static const uint32_t data_307C8 = 4369;
static const int32_t data_307CC[2] = {0, -2184};
static const char data_307D4[] __attribute__((scn_string_literal)) = "01JUCEA011";
static const char data_307DF[] __attribute__((scn_string_literal)) = "01JUCEA012";
static const char data_307EA[] __attribute__((scn_string_literal)) = "01JUCEA013";
static const char data_307F5[] __attribute__((scn_string_literal)) = "01JUCEA002";
static const char data_30800[] __attribute__((scn_string_literal)) = "01JUCEA003";
static const uint8_t data_3080B[1] = {0};
static const uint32_t data_3080C = 0x3DE978D5;
static const float data_30810[2] = {0.002f, -0.036f};
static const uint32_t data_30818 = 8192;
static const int32_t data_3081C[2] = {11377, 3640};
static const uint32_t data_30824 = 0x3DCB04AB;
static const float data_30828[2] = {0.02044f, -0.04322f};
static const uint32_t data_30830 = 0xFFFFFE69;
static const int32_t data_30834[2] = {3331, -3464};
static const uint32_t data_3083C = 0x3D204C06;
static const float data_30840[2] = {0.0f, 0.890018f};
static const uint8_t data_30848[12] = {0};
static const uint32_t data_30854 = 4369;
static const int32_t data_30858[2] = {0, -2184};
static const uint8_t data_30860[12] = {0};
static const char data_3086C[] __attribute__((scn_string_literal)) = "01JUCEA002";
static const char data_30877[] __attribute__((scn_string_literal)) = "01JUCEA003";
static const uint8_t pad_30882[4] = {0};
static const uint8_t data_30886[10] = {
  0x80, 0x3F, 0x00, 0x00, 0x80, 0x3F, 0x00, 0x00, 0x80, 0x3F
};
static DoorMotionSelectorBlock data_30890[38] = {
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
static ScnDoorMotionDescriptor data_30E80[72] = {
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
static uint32_t data_318A0 = 0xFFFFFFFF;
typedef struct Row_318A4 { float w0; float w1; float w2; float w3; int32_t w4; } Row_318A4;
static const Row_318A4 data_318A4[7] = {
  {0.0f, 0.0f, 0.0f, 0.0f, -1},
  {0.0f, 0.0f, 0.0f, 0.0f, -1},
  {0.0f, 0.0f, 0.0f, 0.0f, 1534},
  {26.0f, 55.0f, 45.0f, 75.0f, 1555},
  {61.0f, 146.0f, 45.0f, 50.0f, 1554},
  {59.0f, 124.0f, 45.0f, 70.0f, 1543},
  {33.0f, 49.0f, 45.0f, 50.0f, 1543}
};
static const float data_31930[4] = {33.0f, 49.0f, 50.0f, 50.0f};
static void (*data_31940)(int32_t, int32_t, int32_t) = script_6507;
static void (*const data_31944)(int32_t, int32_t, int32_t, int32_t) = script_6517;
static void (*const data_31948)(int32_t, int32_t, int32_t, int32_t) = script_6517;
static void (*const data_3194C)(int32_t, int32_t, int32_t, int32_t) = script_6517;
static void (*const data_31950)(int32_t, int32_t, int32_t, int32_t) = script_6517;
static void (*const data_31954)(int32_t, int32_t, int32_t, int32_t) = script_6517;
static void (*const data_31958)(int32_t, int32_t, int32_t, int32_t) = script_6517;
static void (*const data_3195C)(int32_t, int32_t, int32_t, int32_t) = script_6517;
static void (*const data_31960)(int32_t, int32_t, int32_t, int32_t) = script_6517;
static void (*const data_31964)(int32_t, int32_t, int32_t, int32_t) = script_67BD;
static void (*const data_31968)(int32_t, int32_t, int32_t, int32_t) = script_691E;
static void (*const data_3196C)(int32_t, int32_t, int32_t, int32_t) = script_6DEB;
static void (*const data_31970)(int32_t, int32_t, int32_t, int32_t) = script_6E52;
static void (*const data_31974)(int32_t, int32_t, int32_t, int32_t) = script_661B;
static void (*const data_31978)(int32_t, int32_t, int32_t, int32_t) = script_661B;
static void (*const data_3197C)(int32_t, int32_t, int32_t, int32_t) = script_6A7F;
static void (*const data_31980)(int32_t, int32_t, int32_t, int32_t) = script_6D1D;
static void (*const data_31984)(int32_t, int32_t, int32_t, int32_t) = script_6EB9;
static void (*const data_31988)(int32_t, int32_t, int32_t, int32_t) = script_67BD;
static void (*const data_3198C)(int32_t, int32_t, int32_t, int32_t) = script_691E;
static void (*const data_31990)(int32_t, int32_t, int32_t, int32_t) = script_6A7F;
static void (*const data_31994)(int32_t, int32_t, int32_t, int32_t) = script_67BD;
static void (*const data_31998)(int32_t, int32_t, int32_t, int32_t) = script_691E;
static void (*const data_3199C)(int32_t, int32_t, int32_t, int32_t) = script_6A7F;
static void (*const data_319A0)(int32_t, int32_t, int32_t, int32_t) = script_6F00;
static void (*const data_319A4)(int32_t, int32_t, int32_t, int32_t) = script_6F00;
static void (*const data_319A8)(int32_t, int32_t, int32_t, int32_t) = script_6F00;
static void (*const data_319AC)(int32_t, int32_t, int32_t, int32_t) = script_6F00;
static void (*const data_319B0)(int32_t, int32_t, int32_t, int32_t) = script_7036;
static void (*const data_319B4)(int32_t, int32_t, int32_t, int32_t) = script_78F7;
static void (*const data_319B8)(int32_t, int32_t, int32_t, int32_t) = script_6517;
static void (*const data_319BC)(int32_t, int32_t, int32_t, int32_t) = script_6517;
static void (*const data_319C0)(int32_t, int32_t, int32_t, int32_t) = script_6517;
static void (*const data_319C4)(int32_t, int32_t, int32_t, int32_t) = script_6517;
static void (*const data_319C8)(int32_t, int32_t, int32_t, int32_t) = script_6517;
static void (*const data_319CC)(int32_t, int32_t, int32_t, int32_t) = script_6517;
static void (*const data_319D0)(int32_t, int32_t, int32_t, int32_t) = script_6517;
static void (*const data_319D4)(int32_t, int32_t, int32_t, int32_t) = script_6517;
static const char data_319D8[46] __attribute__((scn_string_literal)) = "Door Index = %d : call_type = %d : type = %d\012";
static const char data_31A06[43] __attribute__((scn_string_literal)) = "Door Open Motion = DEB_AKESIME_FUSUMA_L_F\012";
static const char data_31A31[43] __attribute__((scn_string_literal)) = "Door Open Motion = DEB_AKESIME_FUSUMA_R_F\012";
static const char data_31A5C[42] __attribute__((scn_string_literal)) = "Door Open Motion = DEB_HIKISIME_DOOR_L_F\012";
static const char data_31A86[42] __attribute__((scn_string_literal)) = "Door Open Motion = DEB_HIKISIME_DOOR_R_F\012";
static const char data_31AB0[41] __attribute__((scn_string_literal)) = "Door Open Motion = DEB_OSISIME_DOOR_L_F\012";
static const char data_31AD9[41] __attribute__((scn_string_literal)) = "Door Open Motion = DEB_OSISIME_DOOR_R_F\012";
static const char data_31B02[43] __attribute__((scn_string_literal)) = "Door Open Motion = GAK_AKESIME_FUSUMA_L_F\012";
static const char data_31B2D[43] __attribute__((scn_string_literal)) = "Door Open Motion = GAK_AKESIME_FUSUMA_R_F\012";
static const char data_31B58[42] __attribute__((scn_string_literal)) = "Door Open Motion = GAK_HIKISIME_DOOR_L_F\012";
static const char data_31B82[42] __attribute__((scn_string_literal)) = "Door Open Motion = GAK_HIKISIME_DOOR_R_F\012";
static const char data_31BAC[41] __attribute__((scn_string_literal)) = "Door Open Motion = GAK_OSISIME_DOOR_L_F\012";
static const char data_31BD5[41] __attribute__((scn_string_literal)) = "Door Open Motion = GAK_OSISIME_DOOR_R_F\012";
static const char data_31BFE[43] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_AKESIME_FUSUMA_L_F\012";
static const char data_31C29[43] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_AKESIME_FUSUMA_R_F\012";
static const char data_31C54[42] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_HIKISIME_DOOR_L_F\012";
static const char data_31C7E[42] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_HIKISIME_DOOR_R_F\012";
static const char data_31CA8[41] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_OSISIME_DOOR_L_F\012";
static const char data_31CD1[41] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_OSISIME_DOOR_R_F\012";
static const char data_31CFA[43] __attribute__((scn_string_literal)) = "Door Open Motion = KOD_AKESIME_FUSUMA_L_F\012";
static const char data_31D25[43] __attribute__((scn_string_literal)) = "Door Open Motion = KOD_AKESIME_FUSUMA_R_F\012";
static const char data_31D50[42] __attribute__((scn_string_literal)) = "Door Open Motion = KOD_HIKISIME_DOOR_L_F\012";
static const char data_31D7A[42] __attribute__((scn_string_literal)) = "Door Open Motion = KOD_HIKISIME_DOOR_R_F\012";
static const char data_31DA4[41] __attribute__((scn_string_literal)) = "Door Open Motion = KOD_OSISIME_DOOR_L_F\012";
static const char data_31DCD[41] __attribute__((scn_string_literal)) = "Door Open Motion = KOD_OSISIME_DOOR_R_F\012";
static const char data_31DF6[43] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_AKESIME_FUSUMA_L_F\012";
static const char data_31E21[43] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_AKESIME_FUSUMA_R_F\012";
static const char data_31E4C[42] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_HIKISIME_DOOR_L_F\012";
static const char data_31E76[42] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_HIKISIME_DOOR_R_F\012";
static const char data_31EA0[41] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_OSISIME_DOOR_L_F\012";
static const char data_31EC9[41] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_OSISIME_DOOR_R_F\012";
static const char data_31EF2[43] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_AKESIME_FUSUMA_L_F\012";
static const char data_31F1D[43] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_AKESIME_FUSUMA_R_F\012";
static const char data_31F48[42] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_HIKISIME_DOOR_L_F\012";
static const char data_31F72[42] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_HIKISIME_DOOR_R_F\012";
static const char data_31F9C[41] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_OSISIME_DOOR_L_F\012";
static const char data_31FC5[41] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_OSISIME_DOOR_R_F\012";
static const char data_31FEE[43] __attribute__((scn_string_literal)) = "Door Open Motion = SIN_AKESIME_FUSUMA_L_F\012";
static const char data_32019[43] __attribute__((scn_string_literal)) = "Door Open Motion = SIN_AKESIME_FUSUMA_R_F\012";
static const char data_32044[42] __attribute__((scn_string_literal)) = "Door Open Motion = SIN_HIKISIME_DOOR_L_F\012";
static const char data_3206E[42] __attribute__((scn_string_literal)) = "Door Open Motion = SIN_HIKISIME_DOOR_R_F\012";
static const char data_32098[41] __attribute__((scn_string_literal)) = "Door Open Motion = SIN_OSISIME_DOOR_L_F\012";
static const char data_320C1[41] __attribute__((scn_string_literal)) = "Door Open Motion = SIN_OSISIME_DOOR_R_F\012";
static const char data_320EA[43] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_AKESIME_FUSUMA_L_F\012";
static const char data_32115[43] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_AKESIME_FUSUMA_R_F\012";
static const char data_32140[42] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_HIKISIME_DOOR_L_F\012";
static const char data_3216A[42] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_HIKISIME_DOOR_R_F\012";
static const char data_32194[41] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_OSISIME_DOOR_L_F\012";
static const char data_321BD[41] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_OSISIME_DOOR_R_F\012";
static const char data_321E6[43] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_AKESIME_FUSUMA_L_F\012";
static const char data_32211[43] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_AKESIME_FUSUMA_R_F\012";
static const char data_3223C[42] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_HIKISIME_DOOR_L_F\012";
static const char data_32266[42] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_HIKISIME_DOOR_R_F\012";
static const char data_32290[41] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_OSISIME_DOOR_L_F\012";
static const char data_322B9[41] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_OSISIME_DOOR_R_F\012";
static const char data_322E2[44] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_OSISIME_DOOR_AP_L_F\012";
static const char data_3230E[44] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_OSISIME_DOOR_AP_L_F\012";
static const char data_3233A[44] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_OSISIME_DOOR_AP_L_F\012";
static const char data_32366[44] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_OSISIME_DOOR_AP_L_F\012";
static const char data_32392[44] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_OSISIME_DOOR_AP_L_F\012";
static const char data_323BE[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_323DA[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_323F6[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_32412[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_3242E[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_3244A[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_32466[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_32482[13] __attribute__((scn_string_literal)) = "open-move!!\012";
static const char data_3248F[11] __attribute__((scn_string_literal)) = "M_MDOR.BIN";
static const char data_3249A[] __attribute__((scn_string_literal)) = "This Charactor Work is NULL!!\n";
static const char data_324B9[22] __attribute__((scn_string_literal)) = "手動ドア開け番号指定\012";
static const char data_324CF[34] __attribute__((scn_string_literal)) = "無限ループになりそうなので抜ける\012";
static const char data_324F1[] __attribute__((scn_string_literal)) = "Don't make BOX Buffer!!\n";
static const char data_3250A[] __attribute__((scn_string_literal)) = "Don't make BOX Buffer!!\n";
static const char data_32523[15] __attribute__((scn_string_literal)) = "Door System : ";
static const char data_32532[15] __attribute__((scn_string_literal)) = "Door System : ";
static const char data_32541[18] __attribute__((scn_string_literal)) = "Door Box is ON!!\012";
static const char data_32553[19] __attribute__((scn_string_literal)) = "Door Box is OFF!!\012";
static const char data_32566[18] __attribute__((scn_string_literal)) = "Door Box is ON!!\012";
static const char data_32578[20] __attribute__((scn_string_literal)) = "Door Box is OFF!!\012";
static const DSWKDoorEntry data_3258C[28] = {
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
static const int32_t data_32B3C[13] = {-1, 0, 6553700, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1};
static const uint8_t data_32B70[4] = {0};
static const int32_t data_32B74[82] = {
  -1, 45, 46, -1, 7, 8, -1, 53, 54, -1, 17, 18, -1, 39, 40, -1, 51, 52, -1, 37, 38, -1, 43, 44, -1, 1, 2, -1, 41, 42, -1, 35, 36, -1, 19, 20, -1, 3,
  4, -1, 33, 34, -1, 13, 14, -1, 11, 12, -1, 9, 10, -1, 23, 24, -1, 25, 26, -1, 29, 30, -1, 27, 28, -1, 21, 22, -1, 31, 32, -1, 5, 6, -1, 47, 48, -1,
  15, 16, -1, 49, 50, -1
};
static char data_32CBC[] = "DR15_024";
static char data_32CC5[] = "DR00_010";
static char data_32CCE[] = "DR15_020";
static char data_32CD7[] = "DR00_011";
static char data_32CE0[] = "DR02_023";
static char data_32CE9[] = "DR15_021";
static char data_32CF2[] = "DR02_022";
static char data_32CFB[] = "DR00_005";
static char data_32D04[] = "DR01_012";
static char data_32D0D[] = "DR01_014";
static char data_32D16[] = "DR15_018";
static char data_32D1F[] = "DR15_019";
static char data_32D28[] = "DR03_001";
static char data_32D31[] = "DR01_013";
static char data_32D3A[] = "DR01_013";
static const uint8_t data_32D43[1] = {0};
static char *const data_32D44 = data_32CBC;
static const int32_t data_32D48[3] = {15, 524971, 590507};
static char *const data_32D54 = data_32CC5;
static const int32_t data_32D58[3] = {0, -1, -1};
static char *const data_32D64 = data_32CCE;
static const int32_t data_32D68[3] = {15, 524971, 590507};
static char *const data_32D74 = data_32CD7;
static const int32_t data_32D78[3] = {0, -1, -1};
static char *const data_32D84 = data_32CE0;
static const int32_t data_32D88[3] = {2, 1311403, 1376939};
static char *const data_32D94 = data_32CE9;
static const int32_t data_32D98[3] = {15, 524971, 590507};
static char *const data_32DA4 = data_32CF2;
static const int32_t data_32DA8[3] = {2, 1311403, 1376939};
static char *const data_32DB4 = data_32CFB;
static const int32_t data_32DB8[3] = {0, -1, -1};
static char *const data_32DC4 = data_32D04;
static const int32_t data_32DC8[3] = {1, 1311403, 1376939};
static char *const data_32DD4 = data_32D0D;
static const int32_t data_32DD8[3] = {1, 1311403, 1376939};
static char *const data_32DE4 = data_32D16;
static const int32_t data_32DE8[3] = {15, 524971, 590507};
static char *const data_32DF4 = data_32D1F;
static const int32_t data_32DF8[3] = {15, 524971, 590507};
static char *const data_32E04 = data_32D28;
static const int32_t data_32E08[3] = {3, 1311403, 1376939};
static char *const data_32E14 = data_32D31;
static const int32_t data_32E18[3] = {1, 1311403, 1376939};
static char *const data_32E24 = data_32D3A;
static const int32_t data_32E28[3] = {37, 1311403, 1376939};
static const MapObjectRecord data_32E34[55] = {
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
static const uint16_t data_335F0[8] = {1068, 1043, 1069, 1071, 1074, 1072, 1073, 903};
static const char data_33600[20] __attribute__((scn_string_literal)) = "TUNED FreeTalk def\012";
static const char data_33614[20] __attribute__((scn_string_literal)) = "TUNED FreeTalk def\012";
static const char data_33628[11] __attribute__((scn_string_literal)) = "bgm109.snd";
static const char data_33633[9] __attribute__((scn_string_literal)) = "SELECT";
static const Vector3 data_3363C = {.x = 10.5122f, .y = 3.1989f, .z = -13.5867f};
static const Vector3 data_33648 = {.x = 9.67f, .y = 3.1546f, .z = 6.2737f};
static const Vector3 data_33654 = {.x = -5.8791f, .y = 3.1546f, .z = 21.8745f};
static const Vector3i data_33660 = {.x = 65172, .y = 12599, .z = 65172};
static const Vector3i data_3366C = {.x = 0, .y = 27285, .z = 0};
static const Vector3i data_33678 = {.x = 0, .y = -26868, .z = 0};
static const int8_t data_33684[16] = {12, 17, 22, 10, 19, 18, 11, 8, 4, 4, 17, 7, 6, 5, 4, 4};
static const int8_t data_33694[16] = {12, 1, 2, 1, 3, 2, 2, 8, 4, 4, 17, 7, 6, 5, 4, 4};
static const uint32_t data_336A4 = 0xA800A7;
static const int32_t data_336A8[5] = {8847529, 8978568, 15073509, 15204583, 15335657};
static const uint32_t data_336BC = 0x440043;
static const int32_t data_336C0[8] = {4587589, 4718663, 4849737, 4980811, 5111885, 5242959, 5374033, 85};
static const uint32_t data_336E0 = 0x5C005B;
static const int32_t data_336E4[10] = {6160477, 6291551, 6422625, 6553699, 6684773, 6815847, 6946921, 7077995, 7209069, 7340143};
static const uint32_t data_3370C = 0x740073;
static const int32_t data_33710[4] = {7798902, 7929976, 8061050, 8257660};
static const uint32_t data_33720 = 0x720071;
static const int32_t data_33724[9] = {10223750, 10354845, 10485919, 10616993, 10748067, 10879141, 11206826, 5767255, 180};
static const uint32_t data_33748 = 0x810080;
static const int32_t data_3374C[8] = {8585346, 11403437, 11534511, 11665585, 11862195, 11993270, 12452024, 8650879};
static const uint32_t data_3376C = 0xB900BC;
static const int32_t data_33770[5] = {12255418, 13238473, 13500621, 12517579, 200};
static const uint32_t data_33784 = 0x5A0059;
static const int32_t data_33788[3] = {5636268, 14221445, 5505107};
static const uint32_t data_33794 = 0x7D0075;
static const int32_t data_33798 = 12386442;
static const uint32_t data_3379C = 0xC600C5;
static const int32_t data_337A0 = 13369543;
static const uint32_t data_337A4 = 0x8C008B;
static const int32_t data_337A8[8] = {9306253, 9437327, 9633937, 148, 9961623, 10092697, 9568411, 149};
static const uint32_t data_337C8 = 0xD700D6;
static const int32_t data_337CC[3] = {14287064, 14876891, 228};
static const uint32_t data_337D8 = 0xDC00D3;
static const int32_t data_337DC[2] = {13893845, 14549213};
static const uint32_t data_337E4 = 0xC100C0;
static const int32_t data_337E8[2] = {12779714, 196};
static const uint32_t data_337F0 = 0xD000CF;
static const int32_t data_337F4 = 13762769;
static const uint32_t data_337F8 = 0xE000DF;
static const int32_t data_337FC = 14811361;
static const char data_33800[169] = "%GACR016G.MT5\000%GACK66BG.MT5\000%GACK66RG.MT5\000%GACR0BNG.MT5\000%GACR0B2G.MT5\000%GACR0B3G.MT5\000%SPBK6REG.MT5\000%SPBK6BRG.MT5\000%SPBK6YEG.MT5\000%SAIK6WHG.MT5\000%SAIK6BKG.MT5\000%SAIK6REG.MT5";
static const char data_338A9[315] = "%GACIAK1G.MT5\000%GACIAK2G.MT5\000%GACOJK1G.MT5\000%GACOJK2G.MT5\000%GACOSA1G.MT5\000%GACOSA2G.MT5\000%GACOLA1G.MT5\000%GACOLA2G.MT5\000%GACOPA1G.MT5\000%GACOPA2G.MT5\000%GACGWF1G.MT5\000%GACGWF2G.MT5\000%GACGJF1G.MT5\000%GACGJF2G.MT5\000%GACIKA1G.MT5\000%GACIKA2G.MT5\000%GACM401G.MT5\000%GACIAA1G.MT5\000%GACIAA2G.MT5\000D\000D\000D\000D\000D\000D\000D\000D\000D\000D\000%GACGJJ1G.MT5\000%GACGJJ2G.MT5";
static const char data_339E4[309] = "%GACIAKAG.MT5\000%GACIAKBG.MT5\000%GACGJKAG.MT5\000%GACGJKBG.MT5\000%GACOSAAG.MT5\000%GACOSABG.MT5\000%GACOLAAG.MT5\000%GACOLABG.MT5\000%GACOPAAG.MT5\000%GACOPABG.MT5\000%GACIWLAG.MT5\000%GACIWLBG.MT5\000%GACGJFAG.MT5\000%GACGJFBG.MT5\000%GACIKAAG.MT5\000%GACIKABG.MT5\000%GACGSNAG.MT5\000%GACGSNBG.MT5\000%GACILIAG.MT5\000%GACILIBG.MT5\000%GACIDUAG.MT5\000%GACIDUBG.MT5";
static const char data_33B19[167] = "%GACRSO1G.MT5\000%GACRSO2G.MT5\000%GACRTL1G.MT5\000%GACRNUCG.MT5\000%GACRBAKG.MT5\000%GACRBING.MT5\000%GACRFNGG.MT5\000%GACRAMYG.MT5\000%GACRESPG.MT5\000%GACREGGG.MT5\000D\000D\000D\000D\000D\000D\000%GACRFNNG.MT5";
static const char data_33BC0[365] = "%GACRNT1G.MT5\000%GACRNT2G.MT5\000%GACRLOPG.MT5\000%GACRMYUG.MT5\000%GACRCKNG.MT5\000%GACRALFG.MT5\000%GACRRISG.MT5\000%GACRARXG.MT5\000%GACR0OPG.MT5\000%GACR0O2G.MT5\000%GACRPOPG.MT5\000%GACRCOBG.MT5\000%GACRIDAG.MT5\000%GACRSHPG.MT5\000%GACRMOBG.MT5\000%GACRROBG.MT5\000%GACK6TZG.MT5\000%GACRPANG.MT5\000%GACK6DYG.MT5\000D\000D\000D\000D\000D\000D\000D\000%GACRAXXG.MT5\000D\000D\000D\000D\000D\000D\000D\000%GACRRBBG.MT5\000%GACK6TSG.MT5\000%GACRPPNG.MT5\000%GACK6DSG.MT5";
static const char data_33D2D[505] = "%GACK6HHG.MT5\000%GACK6HYG.MT5\000%GACK6HBG.MT5\000%GACK6HGG.MT5\000%GACH5BKG.MT5\000%KAB02FSG.MT5\000%GACO1K2G.MT5\000%GACO1K3G.MT5\000%GACO1K7G.MT5\000%GACO1K8G.MT5\000%GACO1K9G.MT5\000%GACO1K1G.MT5\000%GACO1K4G.MT5\000%GACO1K5G.MT5\000%GACO1K6G.MT5\000%GACS5BSG.MT5\000%GACK6HOG.MT5\000%GACK6HKG.MT5\000%GACK6XHG.MT5\000%GACK6XYG.MT5\000%GACK6XBG.MT5\000%GACK6XGG.MT5\000%GACK6BKG.MT5\000%KABK6FSG.MT5\000%GACOKK2G.MT5\000%GACOKK3G.MT5\000%GACOKK7G.MT5\000%GACOKK8G.MT5\000%GACOKK9G.MT5\000%GACOKK1G.MT5\000%GACOKK4G.MT5\000%GACOKK5G.MT5\000%GACOKK6G.MT5\000%GACS5BBG.MT5\000%GACK6XOG.MT5\000%GACK6XKG.MT5";
static const char data_33F26[211] = "%GACH5TKG.MT5\000%GACGTRXG.MT5\000%GACO1T1G.MT5\000%GACO1T2G.MT5\000%GACO1KOG.MT5\000%GACO1KBG.MT5\000%GACO1KJG.MT5\000%GACH5UKG.MT5\000%GACH5IKG.MT5\000%GACO1FOG.MT5\000%GACGSHEG.MT5\000%GACH5TTG.MT5\000%GACGTXXG.MT5\000%GACOTT1G.MT5\000%GACOTT2G.MT5";
static const char data_33FF9[113] = "%YKW0201G.MT5\000%YWT0201G.MT5\000%GACRBONG.MT5\000%GACRRENG.MT5\000%GACK6SPG.MT5\000%GACG360G.MT5\000%GACRDR1G.MT5\000%GACRDR2G.MT5";
static const char data_3406A[77] = "%GACRSSCG.MT5\000%GACRMETG.MT5\000%GACRGBIG.MT5\000%GACH5TRG.MT5\000D\000D\000D\000%GACH5RRG.MT5";
static const char data_340B7[57] = "%GACS5FRG.MT5\000%GACS5FBG.MT5\000%GACO1KUG.MT5\000%GACO1FEG.MT5";
static const char data_340F0[239] = "%GACODG1G.MT5\000%GACODG2G.MT5\000%GACODG3G.MT5\000%GACOCT2G.MT5\000%GACOCATG.MT5\000%GACO1SHG.MT5\000%GACOKSHG.MT5\000%GACO1IRG.MT5\000%GACOKIRG.MT5\000%GACO1HAG.MT5\000%GACOHA2G.MT5\000%GACOHA3G.MT5\000%GACOHA4G.MT5\000%GACO1YGG.MT5\000%GACO1USG.MT5\000%GACOOSHG.MT5\000%GACOOIRG.MT5";
static const char data_341DF[99] = "%GACM400G.MT5\000%GACGSPHG.MT5\000%GACS5HGG.MT5\000%GACGDT1G.MT5\000%GACS5EQG.MT5\000%SYOGSH0G.MT5\000%SYOGHG0G.MT5";
static const char data_34242[85] = "%GACGCRYG.MT5\000%GACGBIRG.MT5\000%GACH5SLG.MT5\000%GACH5JKG.MT5\000%SYOGMD1G.MT5\000%SYOGMD5G.MT5";
static const char data_34297[71] = "%GACS5N1G.MT5\000%GACS5N2G.MT5\000%GACS5N3G.MT5\000%GACS5N4G.MT5\000%GACS5N5G.MT5";
static const char data_342DE[57] = "%GACGXBNG.MT5\000%GACGYBNG.MT5\000%GACGABNG.MT5\000%GACGBBNG.MT5";
static const char data_34317[57] = "%GACRMEGG.MT5\000%GACR0GGG.MT5\000%GACRSATG.MT5\000%GACRCASG.MT5";
static const uint32_t data_34350 = SHEN_NAME('GCH0');
static const shen_name data_34354[5] = {SHEN_NAME('GCH1'), SHEN_NAME('GCH2'), SHEN_NAME('GCH3'), SHEN_NAME('GCH4'), SHEN_NAME('GCH5')};
static const uint32_t data_34368 = SHEN_NAME('HGC0');
static const shen_name data_3436C[5] = {SHEN_NAME('HGC1'), SHEN_NAME('HGC2'), SHEN_NAME('HGC3'), SHEN_NAME('HGC4'), SHEN_NAME('HGC5')};
static const uint32_t data_34380 = SHEN_NAME('GBX0');
static const shen_name data_34384[5] = {SHEN_NAME('GBX1'), SHEN_NAME('GBX2'), SHEN_NAME('GBX3'), SHEN_NAME('GBX4'), SHEN_NAME('GBX5')};
static const uint8_t data_34398[20] = {0};
typedef struct Row_343AC { float w0; int32_t w1; float w2; int32_t w3; int32_t w4; } Row_343AC;
static const Row_343AC data_343AC[5] = {
  {13.8f, 0, 22.65f, 49152, 3},
  {13.8f, 0, 22.0f, 49152, 257},
  {0.0f, 0, 0.0f, 0, 0},
  {0.0f, 0, 0.0f, 0, 0},
  {0.0f, 0, 0.0f, 0, 0}
};
static const char data_34410[16] __attribute__((scn_string_literal)) = "/scene/01/JD00/";
static const char data_34420[8] __attribute__((scn_string_literal)) = "GACH";
static const Vector3 data_34428 = {.x = 0.29f, .y = 0.0f, .z = 0.088f};
static const Vector3i data_34434 = {.x = 0, .y = 0, .z = 0};
static const char data_34440[6] __attribute__((scn_string_literal)) = "E1066";
static const char data_34446[13] __attribute__((scn_string_literal)) = "e1gachap.snd";
static const char data_34453[16] __attribute__((scn_string_literal)) = "/scene/01/JD00/";
static const char data_34463[8] __attribute__((scn_string_literal)) = "HI_GACH";
static const char data_3446B[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_34470[6] __attribute__((scn_string_literal)) = "E1066";
static const char data_34476[8] __attribute__((scn_string_literal)) = "HI_GACH";
static const char data_3447E[10] __attribute__((scn_string_literal)) = "HI_GACH";
static const Vector4 data_34488 = {.x = 0.0f, .y = 0.7f, .z = 0.7f, .w = 0.7f};
static const uint8_t data_34498[4] = {0};
static const int32_t data_3449C[56] = {
  -546, -5870, 0, 2184, -3726, 0, 182, 0, 0, 0, -5184, 0, 71525, -12075, 0, -182, -5279, 0, -364, 910, 0, 1456, -2894, 0, 3458, -13352, 0, -910,
  -10327, 0, 0, 1820, 0, 0, -6863, 0, 1820, -16438, 0, -364, -10150, 0, 236, 291, 0, 0, -12105, 0, 0, -15539, 0, 0, -7249, 0, -819, -364
};
static const uint8_t data_3457C[4] = {0};
static const int32_t data_34580[56] = {
  -546, -5870, 0, 2184, -3726, 0, 182, 0, 0, 0, -5184, 0, 71525, -10922, 0, -182, -5279, 0, -364, 910, 0, 1456, -2894, 0, 3458, -13352, 0, -910,
  -10327, 0, 0, 1820, 0, 0, -6863, 0, 1820, -16438, 0, -364, -10150, 0, 236, 291, 0, 0, -12105, 0, 0, -15539, 0, 0, -7249, 0, -819, -364
};
static const uint8_t data_34660[4] = {0};
static const int32_t data_34664[56] = {
  2912, -7507, 0, 6189, -4454, 0, -1456, 0, 0, 546, -8266, 0, 1292, -7099, 0, 910, -11468, 0, 728, 3276, 0, 1092, -11450, 0, 546, -10256, 0, 1456,
  -6504, 0, 546, 1638, 0, -1092, -12142, 0, -364, -16438, 0, -364, -10150, 0, -309, 291, 0, 1638, -11923, 0, -3276, -17359, 0, -2912, -10707, 0, -819,
  -364
};
static const uint8_t data_34744[4] = {0};
static const int32_t data_34748[56] = {
  1638, -7507, 0, 6189, -4454, 0, -1456, 0, 0, 546, -17731, 0, -709, -9466, 0, 728, -15473, 0, 182, 2548, 0, 1092, -13271, 0, -1092, -14625, 0, 1638,
  -11965, 0, 0, 1638, 0, -1092, -12142, 0, -364, -16438, 0, -364, -10150, 0, -309, 291, 0, 1638, -11923, 0, -3276, -17359, 0, -2912, -10707, 0, -819,
  -364
};
static const uint8_t data_34828[4] = {0};
static const int32_t data_3482C[56] = {
  1638, -6779, 0, 6189, -4454, 0, -1456, 0, 0, 546, -17731, 0, -709, -9466, 0, 1274, -15473, 0, 546, 2548, 0, 1092, -13271, 0, -1092, -14625, 0, 1638,
  -11965, 0, -910, 1638, 0, -1092, -12142, 0, -364, -16438, 0, -364, -10150, 0, -309, 291, 0, 1638, -12105, 0, -3276, -17359, 0, -2912, -10889, 0,
  -819, -364
};
static const uint8_t data_3490C[4] = {0};
static const int32_t data_34910[56] = {
  -2730, -9510, -3094, 4004, -1359, 0, 4004, -728, 0, 546, -6990, 0, 18, -4004, 2366, 1274, -11468, 0, 182, -182, 0, 1092, -13271, 0, -1092, -14443,
  0, 1638, -11965, 0, -910, 1638, 0, -1092, -12142, 0, -364, -16438, 0, -364, -10150, 0, -309, 291, 0, 1638, -12105, 0, -3276, -17359, 0, -2912,
  -10889, 0, -819, -364
};
static const uint8_t data_349F0[8] = {0};
static const int32_t data_349F8[55] = {
  -6005, 0, 4731, -6188, 0, 0, 0, 0, 0, -2727, 0, 0, -9097, 0, 0, -5457, 0, 0, 0, 0, 0, -8734, 0, -909, -7277, 0, 0, -7823, 0, 0, 0, 0, 1456, -4002,
  0, 0, -9462, 0, 726, -7824, 0, 0, 0, 0, 0, -12011, 0, -1091, -7461, 0, -1636, -9280, 0, 0, 0
};
static const uint8_t data_34AD4[4] = {0};
static const int32_t data_34AD8[56] = {
  -546, -5870, 0, 2184, -3726, 0, 182, 0, 0, 0, -5184, 0, 71525, -12075, 0, -182, -5279, 0, -364, 910, 0, 1456, -2894, 0, 3458, -13352, 0, -910,
  -10327, 0, 0, 1820, 0, 0, -6863, 0, 1820, -16438, 0, -364, -10150, 0, 236, 291, 0, 0, -12105, 0, 0, -15539, 0, 0, -7249, 0, -819, -364
};
static const uint8_t data_34BB8[8] = {0};
static const int32_t data_34BC0[55] = {
  -8046, 0, 0, -1820, -13088, 4350, -2894, 0, 0, -9011, 0, 0, -18422, 0, 0, -9812, 0, 0, 0, 0, 0, -7518, 0, 0, -17385, 0, 0, -11559, 0, 0, 0, 0, 0,
  -5734, 0, 0, -18058, 0, 0, -12233, 0, 0, 0, 0, 0, -7827, 0, 0, -16183, 0, 0, -13981, 0, -72, 0
};
static const Vector3 data_34C9C = {.x = 0.067f, .y = 0.061f, .z = 0.0379f};
static const uint32_t data_34CA8 = 0x3DB851EC;
static const float data_34CAC[2] = {0.075f, 0.008f};
static const Vector3i data_34CB4 = {.x = 13111, .y = 817, .z = 50684};
static const Vector3 data_34CC0 = {.x = 0.091f, .y = -0.087f, .z = 0.0671f};
static const Vector3i data_34CCC = {.x = 20024, .y = 65353, .z = 11832};
static const Vector3 data_34CD8 = {.x = 0.0f, .y = 0.0f, .z = 0.0f};
static const Vector3i data_34CE4 = {.x = 0, .y = 0, .z = 0};
static const Vector3 data_34CF0 = {.x = -0.042f, .y = 0.1735f, .z = 0.2062f};
static const char data_34CFC[] __attribute__((scn_string_literal)) = "E1066A001";
static const char data_34D06[] __attribute__((scn_string_literal)) = "E1066A002";
static const char data_34D10[] __attribute__((scn_string_literal)) = "E1066A003";
static const char data_34D1A[] __attribute__((scn_string_literal)) = "E1066A004";
static const char data_34D24[] __attribute__((scn_string_literal)) = "E1066A005";
static const char data_34D2E[] __attribute__((scn_string_literal)) = "E1066A006";
static const char data_34D38[] __attribute__((scn_string_literal)) = "E1066A007";
static const char data_34D42[] __attribute__((scn_string_literal)) = "E1066A008";
static const char data_34D4C[] __attribute__((scn_string_literal)) = "E1066A009";
static const char data_34D56[] __attribute__((scn_string_literal)) = "E1066A010";
static const char data_34D60[] __attribute__((scn_string_literal)) = "E1066A011";
static const char data_34D6A[] __attribute__((scn_string_literal)) = "E1066A026";
static const char data_34D74[] __attribute__((scn_string_literal)) = "E1066A027";
static const char data_34D7E[] __attribute__((scn_string_literal)) = "E1066A019";
static const char data_34D88[] __attribute__((scn_string_literal)) = "E1066A020";
static const char data_34D92[] __attribute__((scn_string_literal)) = "E1066A021";
static const char data_34D9C[] __attribute__((scn_string_literal)) = "E1066A014";
static const char data_34DA6[] __attribute__((scn_string_literal)) = "E1066A015";
static const char data_34DB0[] __attribute__((scn_string_literal)) = "E1066A016";
static const char data_34DBA[] __attribute__((scn_string_literal)) = "E1066A017";
static const char data_34DC4[] __attribute__((scn_string_literal)) = "E1066A018";
static const char data_34DCE[] __attribute__((scn_string_literal)) = "E1066A022";
static const char data_34DD8[] __attribute__((scn_string_literal)) = "E1066A025";
static const char data_34DE2[] __attribute__((scn_string_literal)) = "E1066A024";
static const char data_34DEC[12] __attribute__((scn_string_literal)) = "E1066A023";
static const Vector3 data_34DF8 = {.x = 0.337795f, .y = 0.596267f, .z = -0.181433f};
static const Vector3 data_34E04 = {.x = 0.091f, .y = -0.087f, .z = 0.0671f};
static const Vector3i data_34E10 = {.x = 20024, .y = 65353, .z = 11832};
static const char data_34E1C[12] __attribute__((scn_string_literal)) = "JINGURU01";
static const uint8_t data_34E28[28] = {0};
static const int32_t data_34E44[7] = {-1, 0, 0, 0, 0, 0, 0};
static char data_34E60[] = "$B14A0M1G.MT5";
static const uint8_t data_34E6E[2] = {0};
static const uint32_t data_34E70 = 0xFFFF0000;
static const uint32_t data_34E74[2] = {0, 0x80000000U};
static const float data_34E7C[5] = {15.0673f, 1.0992f, 2.0482f, 0.0f, 0.0f};
static const int32_t data_34E90[2] = {-65536, 101};
static const uint32_t data_34E98 = 0x71C74000U;
static const float data_34E9C[5] = {9.5827f, 1.3422f, 1.3926f, 0.0f, 0.0f};
static const int32_t data_34EB0[2] = {-65536, 102};
static const uint32_t data_34EB8 = 0x71C74000U;
static const float data_34EBC[5] = {9.5219f, 1.635f, 32.3657f, 0.0f, 0.0f};
static const int32_t data_34ED0[2] = {-65536, 103};
static const uint32_t data_34ED8 = 0x71C77C71U;
static const float data_34EDC[5] = {28.2411f, 1.696f, 9.228f, 0.0f, 0.0f};
static const int32_t data_34EF0[2] = {-65536, 104};
static const uint32_t data_34EF8[2] = {0x71C78000U, 0x41A56E2FU};
static const float data_34F00[4] = {1.4047f, 6.9876f, 0.0f, 0.0f};
static const int32_t data_34F10[2] = {-65536, 106};
static const uint32_t data_34F18 = 0x71C74000U;
static const float data_34F1C[5] = {9.5741f, 1.5472f, -1.9451f, 0.0f, 0.0f};
static const int32_t data_34F30[2] = {-65536, 107};
static const uint32_t data_34F38 = 0x71C74000U;
static const float data_34F3C[5] = {9.4969f, 1.25f, 12.0913f, 0.0f, 0.0f};
static const int32_t data_34F50[2] = {-65536, 109};
static const uint32_t data_34F58 = 0x71C74000U;
static const float data_34F5C[5] = {8.1858f, 1.7775f, 27.101f, 0.0f, 0.0f};
static const int32_t data_34F70[2] = {-65536, 110};
static const uint32_t data_34F78 = 0x71C7671CU;
static const float data_34F7C[2] = {14.3068f, 1.5725f};
static const uint32_t data_34F84[3] = {0x420BDC43U, 0, 0};
static const int32_t data_34F90[2] = {-65536, 111};
static const uint32_t data_34F98 = 0x71C7E38EU;
static const float data_34F9C[2] = {20.4664f, 1.6303f};
static const uint32_t data_34FA4[3] = {0x420D7909U, 0, 0};
static const int32_t data_34FB0[2] = {-65536, 112};
static const uint32_t data_34FB8 = 0x71C70000U;
static const float data_34FBC[5] = {13.6589f, 2.0333f, 27.6413f, 0.0f, 0.0f};
static const int32_t data_34FD0[2] = {-65536, 116};
static const uint32_t data_34FD8[2] = {0x71C7F60BU, 0xC20B4275U};
static const float data_34FE0[4] = {4.2982f, -2.5379f, 0.0f, 0.0f};
static const int32_t data_34FF0[2] = {-65536, 117};
static const uint32_t data_34FF8[2] = {0x71C78000U, 0xC1C9F803U};
static const float data_35000[4] = {2.5835f, 4.8512f, 0.0f, 0.0f};
static const int32_t data_35010[2] = {-65536, 118};
static const uint32_t data_35018 = 0x71C74000U;
static const float data_3501C[5] = {-14.4027f, 2.0075f, 6.531f, 0.0f, 0.0f};
static const int32_t data_35030[2] = {-65536, 114};
static const uint32_t data_35038 = 0x71C7B1C7U;
static const float data_3503C[5] = {-8.3123f, 1.1153f, 9.8865f, 0.0f, 0.0f};
static const int32_t data_35050[2] = {-65536, 113};
static const uint32_t data_35058 = 0x71C78000U;
static const float data_3505C[2] = {-3.5656f, 1.14f};
static const uint32_t data_35064[3] = {0x41B453C3U, 0, 0};
static const int32_t data_35070[2] = {-65535, 123};
static const uint32_t data_35078[2] = {0x71C7F555U, 0xC18F6F00U};
static const float data_35080[4] = {3.139f, 27.362f, 0.0f, 0.0f};
static const int32_t data_35090[2] = {-65535, 124};
static const uint32_t data_35098[2] = {0x71C7F555U, 0xC1A8A95FU};
static const float data_350A0 = 3.1365f;
static const uint32_t data_350A4[3] = {0x41DCBDD9U, 0, 0};
static const int32_t data_350B0[2] = {-65534, 125};
static const uint32_t data_350B8[2] = {0x71C7F555U, 0xC18F6F00U};
static const float data_350C0 = 5.8396f;
static const uint32_t data_350C4[3] = {0x41DB020CU, 0, 0};
static const int32_t data_350D0[2] = {-65534, 126};
static const uint32_t data_350D8[2] = {0x71C7F555U, 0xC1A8A95FU};
static const float data_350E0 = 5.8396f;
static const uint32_t data_350E4[3] = {0x41DCC04FU, 0, 0};
static const int32_t data_350F0[2] = {-65535, 119};
static const uint32_t data_350F8[2] = {0x71C70000U, 0xC182E595U};
static const float data_35100 = 3.1365f;
static const uint32_t data_35104[3] = {0x420A4CCDU, 0, 0};
static const int32_t data_35110[2] = {-65535, 120};
static const uint32_t data_35118[2] = {0x71C70000U, 0xC19ACE07U};
static const float data_35120[4] = {3.1365f, 35.5826f, 0.0f, 0.0f};
static const int32_t data_35130[2] = {-65534, 121};
static const uint32_t data_35138[2] = {0x71C70000U, 0xC182DD64U};
static const float data_35140 = 5.8365f;
static const uint32_t data_35144[3] = {0x420A578DU, 0, 0};
static const int32_t data_35150[2] = {-65534, 122};
static const uint32_t data_35158 = 0x71C70000U;
static const float data_3515C[2] = {-19.3489f, 5.8365f};
static const uint32_t data_35164[3] = {0x420E5C78U, 0, 0};
static const int32_t data_35170[2] = {-65536, 112};
static const uint32_t data_35178 = 0x71C7F1C7U;
static const float data_3517C[5] = {-10.4958f, 1.2717f, 3.1206f, 0.0f, 0.0f};
static const int32_t data_35190[2] = {-65536, 108};
static const uint32_t data_35198 = 0x71C74000U;
static const float data_3519C[5] = {9.493f, 1.2451f, 16.1099f, 0.0f, 0.0f};
static const int32_t data_351B0[2] = {-65536, 105};
static const uint32_t data_351B8 = 0x71C70000U;
static const float data_351BC[5] = {8.9036f, 1.55f, -16.9252f, 0.0f, 0.0f};
static const int32_t data_351D0[2] = {-65536, 115};
static const uint32_t data_351D8[2] = {0x71C7F555U, 0xC18881A3U};
static const float data_351E0[4] = {2.971f, -3.2252f, 0.0f, 0.0f};
static const int32_t data_351F0[2] = {-65536, 131};
static const uint32_t data_351F8 = 0x71C7E0B6U;
static const float data_351FC[5] = {19.1329f, 1.23f, 34.5327f, 0.0f, 0.0f};
static const int32_t data_35210[2] = {-65536, 131};
static const uint32_t data_35218 = 0x71C70000U;
static const float data_3521C[5] = {2.1828f, 1.2508f, 18.9f, 0.0f, 0.0f};
static const int32_t data_35230[2] = {-65536, 132};
static const uint32_t data_35238 = 0x71C7CFA4U;
static const float data_3523C[5] = {-6.6538f, 1.2028f, 4.0641f, 0.0f, 0.0f};
static const int32_t data_35250[2] = {-65536, 133};
static const uint32_t data_35258 = 0x71C7EAAAU;
static const float data_3525C[5] = {13.7469f, 1.2147f, -8.3833f, 0.0f, 0.0f};
static const int32_t data_35270[2] = {-65536, 135};
static const uint32_t data_35278 = 0x71C7B8E3U;
static const float data_3527C[5] = {13.8298f, 1.2958f, 23.5861f, 0.0f, 0.0f};
static const int32_t data_35290[2] = {-65536, 136};
static const uint32_t data_35298 = 0x71C74000U;
static const float data_3529C[5] = {9.5274f, 1.4023f, 32.3564f, 0.0f, 0.0f};
static const int32_t data_352B0[2] = {-65536, 137};
static const uint32_t data_352B8 = 0x71C7C000U;
static const float data_352BC[2] = {14.7023f, 1.475f};
static const uint32_t data_352C4[4] = {0x41ADA29CU, 0, 0, 0};
static const shen_name data_352D4 = SHEN_NAME('GCH0');
static const float data_352D8[7] = {40128.0f, 0.0f, 0.76f, 0.055f, 0.0f, 0.0f, 0.0f};
static const shen_name data_352F4 = SHEN_NAME('GCH1');
static const float data_352F8[7] = {40128.0f, 0.0f, 0.76f, 0.055f, 0.0f, 0.0f, 0.0f};
static const shen_name data_35314 = SHEN_NAME('GCH2');
static const float data_35318[7] = {40128.0f, 0.0f, 0.76f, 0.055f, 0.0f, 0.0f, 0.0f};
static const shen_name data_35334 = SHEN_NAME('GCH3');
static const float data_35338[7] = {40128.0f, 0.0f, 0.76f, 0.055f, 0.0f, 0.0f, 0.0f};
static const shen_name data_35354 = SHEN_NAME('GCH4');
static const float data_35358[7] = {40128.0f, 0.0f, 0.76f, 0.055f, 0.0f, 0.0f, 0.0f};
static const shen_name data_35374 = SHEN_NAME('GCH5');
static const float data_35378[6] = {40128.0f, 0.0f, 0.76f, 0.055f, 0.0f, 0.0f};
static const int32_t data_35390 = 65535;
static uint32_t data_35394 = 0x417113A9;
static const float data_35398[2] = {1.1f, 2.0982f};
static const char data_353A0[6] __attribute__((scn_string_literal)) = "E1040";
static const char data_353A6[10] __attribute__((scn_string_literal)) = "E1040A028";
static const char data_353B0[10] __attribute__((scn_string_literal)) = "E1040A029";
static const char data_353BA[6] __attribute__((scn_string_literal)) = "E1040";
static const char data_353C0[10] __attribute__((scn_string_literal)) = "E1040A030";
static const char data_353CA[10] __attribute__((scn_string_literal)) = "E1040A031";
static const char data_353D4[10] __attribute__((scn_string_literal)) = "E1040A032";
static const char data_353DE[6] __attribute__((scn_string_literal)) = "E1040";
static const Vector3 data_353E4 = {.x = 21.6f, .y = 0.75f, .z = -6.18f};
static const char data_353F0[13] __attribute__((scn_string_literal)) = "e1shugyo.snd";
static const char data_353FD[10] __attribute__((scn_string_literal)) = "E1040A001";
static const char data_35407[10] __attribute__((scn_string_literal)) = "E1040A003";
static const char data_35411[10] __attribute__((scn_string_literal)) = "E1040A009";
static const char data_3541B[10] __attribute__((scn_string_literal)) = "E1040A010";
static const char data_35425[10] __attribute__((scn_string_literal)) = "E1040A012";
static const char data_3542F[10] __attribute__((scn_string_literal)) = "E1040A013";
static const char data_35439[10] __attribute__((scn_string_literal)) = "E1040A014";
static const char data_35443[10] __attribute__((scn_string_literal)) = "E1040A018";
static const char data_3544D[10] __attribute__((scn_string_literal)) = "E1040A020";
static const char data_35457[10] __attribute__((scn_string_literal)) = "E1040A015";
static const char data_35461[10] __attribute__((scn_string_literal)) = "E1040A016";
static const char data_3546B[10] __attribute__((scn_string_literal)) = "E1040A017";
static const char data_35475[6] __attribute__((scn_string_literal)) = "E1040";
static const char data_3547B[9] __attribute__((scn_string_literal)) = "E1040";
static const Vector3 data_35484 = {.x = 19.9f, .y = 0.0f, .z = -8.2f};
static const Vector3 data_35490 = {.x = 23.28f, .y = 0.75f, .z = -9.38f};
static const char data_3549C[16] __attribute__((scn_string_literal)) = "\012 itou ga iru\012\012";
static const char data_354AC[18] __attribute__((scn_string_literal)) = "\012 mayumi ga iru\012\012";
static const char data_354BE[22] __attribute__((scn_string_literal)) = "\012 yamagisi ga iru\012\012";
static const int32_t data_354D4[57] = {
  0, 0, -8046, 0, 0, -1820, -13088, 4350, -2894, 0, 0, -9011, 0, 0, -18422, 0, 0, -9812, 0, 0, 0, 0, 0, -7518, 0, 0, -17385, 0, 0, -11559, 0, 0, 0, 0,
  0, -5734, 0, 0, -18058, 0, 0, -12233, 0, 0, 0, 0, 0, -7827, 0, 0, -16183, 0, 0, -13981, 0, -72, 0
};
static const int32_t data_355B8[57] = {
  0, 0, -2712, 0, 0, -691, 0, 0, 0, 0, 0, -1092, 0, 0, -1438, 0, 0, -2748, 0, 0, 0, 0, 0, 364, 0, 0, -4132, 0, 0, -3531, 0, 0, 0, 0, 0, -2020, 0, 0,
  -6881, 0, 0, -5333, 0, 109, -200, 0, 0, -3986, 0, 0, -8829, 0, 0, -7245, 0, -764, -837
};
static const uint32_t data_3569C = 0x303131;
static const uint32_t data_356A0 = 0x393131;
static const uint32_t data_356A4 = 0x373731;
static const uint32_t data_356A8 = 0x343031;
static const char data_356AC[] __attribute__((scn_string_literal)) = "61564";
static const char data_356B2[] __attribute__((scn_string_literal)) = "37742";
static const char data_356B8[] __attribute__((scn_string_literal)) = "37749";
static const char data_356BE[] __attribute__((scn_string_literal)) = "22550";
static const char data_356C4[] __attribute__((scn_string_literal)) = "046861564";
static const char data_356CE[] __attribute__((scn_string_literal)) = "046837742";
static const char data_356D8[] __attribute__((scn_string_literal)) = "046837749";
static const char data_356E2[] __attribute__((scn_string_literal)) = "046822550";
static const char data_356EC[] __attribute__((scn_string_literal)) = "2?????";
static const char data_356F3[] __attribute__((scn_string_literal)) = "5?????";
static const char data_356FA[] __attribute__((scn_string_literal)) = "7?????";
static const char data_35701[] __attribute__((scn_string_literal)) = "8?????";
static const char data_35708[] __attribute__((scn_string_literal)) = "3?????";
static const char data_3570F[] __attribute__((scn_string_literal)) = "4?????";
static const char data_35716[] __attribute__((scn_string_literal)) = "6?????";
static const char data_3571D[] __attribute__((scn_string_literal)) = "9?????";
static const char data_35724[] __attribute__((scn_string_literal)) = "0?????????";
static const char data_3572F[] __attribute__((scn_string_literal)) = "1?????";
static const char data_35736[6] __attribute__((scn_string_literal)) = "E1066";
static const char data_3573C[10] __attribute__((scn_string_literal)) = "E1066A012";
static const char data_35746[10] __attribute__((scn_string_literal)) = "E1066A013";
static const char data_35750[10] __attribute__((scn_string_literal)) = "E1066A006";
static const char data_3575A[6] __attribute__((scn_string_literal)) = "E1066";
static const char data_35760[12] __attribute__((scn_string_literal)) = "メモ帳消去\012";
static const char data_3576C[6] __attribute__((scn_string_literal)) = "E1032";
static const char data_35772[10] __attribute__((scn_string_literal)) = "E1032A001";
static const char data_3577C[10] __attribute__((scn_string_literal)) = "E1032A002";
static const char data_35786[10] __attribute__((scn_string_literal)) = "E1032A003";
static const char data_35790[10] __attribute__((scn_string_literal)) = "E1032A004";
static const char data_3579A[10] __attribute__((scn_string_literal)) = "E1032A005";
static const char data_357A4[6] __attribute__((scn_string_literal)) = "E1032";
static const char data_357AA[6] __attribute__((scn_string_literal)) = "E1032";
static const char data_357B0[10] __attribute__((scn_string_literal)) = "E1032A061";
static const char data_357BA[10] __attribute__((scn_string_literal)) = "E1032A062";
static const char data_357C4[10] __attribute__((scn_string_literal)) = "E1032A063";
static const char data_357CE[10] __attribute__((scn_string_literal)) = "E1032A064";
static const char data_357D8[6] __attribute__((scn_string_literal)) = "E1032";
static const char data_357DE[6] __attribute__((scn_string_literal)) = "E1032";
static const char data_357E4[6] __attribute__((scn_string_literal)) = "E1032";
static const char data_357EA[6] __attribute__((scn_string_literal)) = "E1032";
static const char data_357F0[10] __attribute__((scn_string_literal)) = "E1032A031";
static const char data_357FA[10] __attribute__((scn_string_literal)) = "E1032A029";
static const char data_35804[10] __attribute__((scn_string_literal)) = "E1032A032";
static const char data_3580E[10] __attribute__((scn_string_literal)) = "E1032A033";
static const char data_35818[6] __attribute__((scn_string_literal)) = "E1032";
static const char data_3581E[10] __attribute__((scn_string_literal)) = "E1032A028";
static const char data_35828[10] __attribute__((scn_string_literal)) = "E1032A029";
static const char data_35832[10] __attribute__((scn_string_literal)) = "E1032A030";
static const char data_3583C[6] __attribute__((scn_string_literal)) = "E1032";
static const char data_35842[10] __attribute__((scn_string_literal)) = "E1032G001";
static const char data_3584C[10] __attribute__((scn_string_literal)) = "E1032G002";
static const char data_35856[6] __attribute__((scn_string_literal)) = "E1032";
static const char data_3585C[10] __attribute__((scn_string_literal)) = "E1032I001";
static const char data_35866[10] __attribute__((scn_string_literal)) = "E1032A052";
static const char data_35870[10] __attribute__((scn_string_literal)) = "E1032I007";
static const char data_3587A[10] __attribute__((scn_string_literal)) = "E1032I008";
static const char data_35884[10] __attribute__((scn_string_literal)) = "E1032A097";
static const char data_3588E[10] __attribute__((scn_string_literal)) = "E1032I009";
static const char data_35898[10] __attribute__((scn_string_literal)) = "E1032I010";
static const char data_358A2[10] __attribute__((scn_string_literal)) = "E1032A054";
static const char data_358AC[10] __attribute__((scn_string_literal)) = "E1032I011";
static const char data_358B6[10] __attribute__((scn_string_literal)) = "E1032A055";
static const char data_358C0[10] __attribute__((scn_string_literal)) = "E1032I012";
static const char data_358CA[10] __attribute__((scn_string_literal)) = "E1032A056";
static const char data_358D4[10] __attribute__((scn_string_literal)) = "E1032I013";
static const char data_358DE[10] __attribute__((scn_string_literal)) = "E1032A057";
static const char data_358E8[10] __attribute__((scn_string_literal)) = "E1032I011";
static const char data_358F2[10] __attribute__((scn_string_literal)) = "E1032A059";
static const char data_358FC[10] __attribute__((scn_string_literal)) = "E1032I009";
static const char data_35906[10] __attribute__((scn_string_literal)) = "E1032I010";
static const char data_35910[10] __attribute__((scn_string_literal)) = "E1032A054";
static const char data_3591A[10] __attribute__((scn_string_literal)) = "E1032I011";
static const char data_35924[10] __attribute__((scn_string_literal)) = "E1032A060";
static const char data_3592E[10] __attribute__((scn_string_literal)) = "E1032I009";
static const char data_35938[10] __attribute__((scn_string_literal)) = "E1032I010";
static const char data_35942[10] __attribute__((scn_string_literal)) = "E1032A054";
static const char data_3594C[10] __attribute__((scn_string_literal)) = "E1032I011";
static const char data_35956[10] __attribute__((scn_string_literal)) = "E1032I001";
static const char data_35960[10] __attribute__((scn_string_literal)) = "E1032A007";
static const char data_3596A[10] __attribute__((scn_string_literal)) = "E1032A006";
static const char data_35974[10] __attribute__((scn_string_literal)) = "E1032I002";
static const char data_3597E[10] __attribute__((scn_string_literal)) = "E1032I003";
static const char data_35988[10] __attribute__((scn_string_literal)) = "E1032I004";
static const char data_35992[10] __attribute__((scn_string_literal)) = "E1032I005";
static const char data_3599C[10] __attribute__((scn_string_literal)) = "E1032I006";
static const char data_359A6[10] __attribute__((scn_string_literal)) = "E1032A008";
static const char data_359B0[10] __attribute__((scn_string_literal)) = "E1032A009";
static const char data_359BA[10] __attribute__((scn_string_literal)) = "E1032K001";
static const char data_359C4[10] __attribute__((scn_string_literal)) = "E1032K002";
static const char data_359CE[10] __attribute__((scn_string_literal)) = "E1032K019";
static const char data_359D8[10] __attribute__((scn_string_literal)) = "E1032K020";
static const char data_359E2[10] __attribute__((scn_string_literal)) = "E1032K022";
static const char data_359EC[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_359F6[10] __attribute__((scn_string_literal)) = "E1032K016";
static const char data_35A00[10] __attribute__((scn_string_literal)) = "E1032K017";
static const char data_35A0A[10] __attribute__((scn_string_literal)) = "E1032K018";
static const char data_35A14[10] __attribute__((scn_string_literal)) = "E1032K003";
static const char data_35A1E[10] __attribute__((scn_string_literal)) = "E1032K016";
static const char data_35A28[10] __attribute__((scn_string_literal)) = "E1032K003";
static const char data_35A32[10] __attribute__((scn_string_literal)) = "E1032K017";
static const char data_35A3C[10] __attribute__((scn_string_literal)) = "E1032K003";
static const char data_35A46[10] __attribute__((scn_string_literal)) = "E1032K018";
static const char data_35A50[10] __attribute__((scn_string_literal)) = "E1032K004";
static const char data_35A5A[10] __attribute__((scn_string_literal)) = "E1032K016";
static const char data_35A64[10] __attribute__((scn_string_literal)) = "E1032K004";
static const char data_35A6E[10] __attribute__((scn_string_literal)) = "E1032K017";
static const char data_35A78[10] __attribute__((scn_string_literal)) = "E1032K004";
static const char data_35A82[10] __attribute__((scn_string_literal)) = "E1032K018";
static const char data_35A8C[10] __attribute__((scn_string_literal)) = "E1032K005";
static const char data_35A96[10] __attribute__((scn_string_literal)) = "E1032K016";
static const char data_35AA0[10] __attribute__((scn_string_literal)) = "E1032K005";
static const char data_35AAA[10] __attribute__((scn_string_literal)) = "E1032K017";
static const char data_35AB4[10] __attribute__((scn_string_literal)) = "E1032K005";
static const char data_35ABE[10] __attribute__((scn_string_literal)) = "E1032K018";
static const char data_35AC8[10] __attribute__((scn_string_literal)) = "E1032K006";
static const char data_35AD2[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_35ADC[10] __attribute__((scn_string_literal)) = "E1032K006";
static const char data_35AE6[10] __attribute__((scn_string_literal)) = "E1032K017";
static const char data_35AF0[10] __attribute__((scn_string_literal)) = "E1032K006";
static const char data_35AFA[10] __attribute__((scn_string_literal)) = "E1032K018";
static const char data_35B04[10] __attribute__((scn_string_literal)) = "E1032K007";
static const char data_35B0E[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_35B18[10] __attribute__((scn_string_literal)) = "E1032K007";
static const char data_35B22[10] __attribute__((scn_string_literal)) = "E1032K017";
static const char data_35B2C[10] __attribute__((scn_string_literal)) = "E1032K007";
static const char data_35B36[10] __attribute__((scn_string_literal)) = "E1032K018";
static const char data_35B40[10] __attribute__((scn_string_literal)) = "E1032K008";
static const char data_35B4A[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_35B54[10] __attribute__((scn_string_literal)) = "E1032K008";
static const char data_35B5E[10] __attribute__((scn_string_literal)) = "E1032K017";
static const char data_35B68[10] __attribute__((scn_string_literal)) = "E1032K008";
static const char data_35B72[10] __attribute__((scn_string_literal)) = "E1032K018";
static const char data_35B7C[10] __attribute__((scn_string_literal)) = "E1032K009";
static const char data_35B86[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_35B90[10] __attribute__((scn_string_literal)) = "E1032K009";
static const char data_35B9A[10] __attribute__((scn_string_literal)) = "E1032K016";
static const char data_35BA4[10] __attribute__((scn_string_literal)) = "E1032K009";
static const char data_35BAE[10] __attribute__((scn_string_literal)) = "E1032K018";
static const char data_35BB8[10] __attribute__((scn_string_literal)) = "E1032K010";
static const char data_35BC2[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_35BCC[10] __attribute__((scn_string_literal)) = "E1032K010";
static const char data_35BD6[10] __attribute__((scn_string_literal)) = "E1032K016";
static const char data_35BE0[10] __attribute__((scn_string_literal)) = "E1032K010";
static const char data_35BEA[10] __attribute__((scn_string_literal)) = "E1032K018";
static const char data_35BF4[10] __attribute__((scn_string_literal)) = "E1032K011";
static const char data_35BFE[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_35C08[10] __attribute__((scn_string_literal)) = "E1032K011";
static const char data_35C12[10] __attribute__((scn_string_literal)) = "E1032K016";
static const char data_35C1C[10] __attribute__((scn_string_literal)) = "E1032K011";
static const char data_35C26[10] __attribute__((scn_string_literal)) = "E1032K018";
static const char data_35C30[10] __attribute__((scn_string_literal)) = "E1032K012";
static const char data_35C3A[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_35C44[10] __attribute__((scn_string_literal)) = "E1032K012";
static const char data_35C4E[10] __attribute__((scn_string_literal)) = "E1032K016";
static const char data_35C58[10] __attribute__((scn_string_literal)) = "E1032K012";
static const char data_35C62[10] __attribute__((scn_string_literal)) = "E1032K017";
static const char data_35C6C[10] __attribute__((scn_string_literal)) = "E1032K013";
static const char data_35C76[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_35C80[10] __attribute__((scn_string_literal)) = "E1032K013";
static const char data_35C8A[10] __attribute__((scn_string_literal)) = "E1032K016";
static const char data_35C94[10] __attribute__((scn_string_literal)) = "E1032K013";
static const char data_35C9E[10] __attribute__((scn_string_literal)) = "E1032K017";
static const char data_35CA8[10] __attribute__((scn_string_literal)) = "E1032K014";
static const char data_35CB2[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_35CBC[10] __attribute__((scn_string_literal)) = "E1032K014";
static const char data_35CC6[10] __attribute__((scn_string_literal)) = "E1032K016";
static const char data_35CD0[10] __attribute__((scn_string_literal)) = "E1032K014";
static const char data_35CDA[10] __attribute__((scn_string_literal)) = "E1032K017";
static const char data_35CE4[10] __attribute__((scn_string_literal)) = "E1032K004";
static const char data_35CEE[10] __attribute__((scn_string_literal)) = "E1032K007";
static const char data_35CF8[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_35D02[10] __attribute__((scn_string_literal)) = "E1032K004";
static const char data_35D0C[10] __attribute__((scn_string_literal)) = "E1032K007";
static const char data_35D16[10] __attribute__((scn_string_literal)) = "E1032K017";
static const char data_35D20[10] __attribute__((scn_string_literal)) = "E1032K004";
static const char data_35D2A[10] __attribute__((scn_string_literal)) = "E1032K006";
static const char data_35D34[10] __attribute__((scn_string_literal)) = "E1032K017";
static const char data_35D3E[10] __attribute__((scn_string_literal)) = "E1032K004";
static const char data_35D48[10] __attribute__((scn_string_literal)) = "E1032K013";
static const char data_35D52[10] __attribute__((scn_string_literal)) = "E1032K017";
static const char data_35D5C[10] __attribute__((scn_string_literal)) = "E1032K005";
static const char data_35D66[10] __attribute__((scn_string_literal)) = "E1032K007";
static const char data_35D70[10] __attribute__((scn_string_literal)) = "E1032K018";
static const char data_35D7A[10] __attribute__((scn_string_literal)) = "E1032K007";
static const char data_35D84[10] __attribute__((scn_string_literal)) = "E1032K004";
static const char data_35D8E[10] __attribute__((scn_string_literal)) = "E1032K017";
static const char data_35D98[10] __attribute__((scn_string_literal)) = "E1032K008";
static const char data_35DA2[10] __attribute__((scn_string_literal)) = "E1032K010";
static const char data_35DAC[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_35DB6[10] __attribute__((scn_string_literal)) = "E1032K008";
static const char data_35DC0[10] __attribute__((scn_string_literal)) = "E1032K013";
static const char data_35DCA[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_35DD4[10] __attribute__((scn_string_literal)) = "E1032K010";
static const char data_35DDE[10] __attribute__((scn_string_literal)) = "E1032K005";
static const char data_35DE8[10] __attribute__((scn_string_literal)) = "E1032K016";
static const char data_35DF2[10] __attribute__((scn_string_literal)) = "E1032K010";
static const char data_35DFC[10] __attribute__((scn_string_literal)) = "E1032K004";
static const char data_35E06[10] __attribute__((scn_string_literal)) = "E1032K018";
static const char data_35E10[10] __attribute__((scn_string_literal)) = "E1032K010";
static const char data_35E1A[10] __attribute__((scn_string_literal)) = "E1032K007";
static const char data_35E24[10] __attribute__((scn_string_literal)) = "E1032K018";
static const char data_35E2E[10] __attribute__((scn_string_literal)) = "E1032K011";
static const char data_35E38[10] __attribute__((scn_string_literal)) = "E1032K007";
static const char data_35E42[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_35E4C[10] __attribute__((scn_string_literal)) = "E1032K013";
static const char data_35E56[10] __attribute__((scn_string_literal)) = "E1032K008";
static const char data_35E60[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_35E6A[10] __attribute__((scn_string_literal)) = "E1032K010";
static const char data_35E74[10] __attribute__((scn_string_literal)) = "E1032K005";
static const char data_35E7E[10] __attribute__((scn_string_literal)) = "E1032K016";
static const char data_35E88[10] __attribute__((scn_string_literal)) = "E1032K004";
static const char data_35E92[10] __attribute__((scn_string_literal)) = "E1032K010";
static const char data_35E9C[10] __attribute__((scn_string_literal)) = "E1032K016";
static const char data_35EA6[10] __attribute__((scn_string_literal)) = "E1032K004";
static const char data_35EB0[10] __attribute__((scn_string_literal)) = "E1032K008";
static const char data_35EBA[10] __attribute__((scn_string_literal)) = "E1032K018";
static const char data_35EC4[10] __attribute__((scn_string_literal)) = "E1032K003";
static const char data_35ECE[10] __attribute__((scn_string_literal)) = "E1032K007";
static const char data_35ED8[10] __attribute__((scn_string_literal)) = "E1032K018";
static const char data_35EE2[10] __attribute__((scn_string_literal)) = "E1032K007";
static const char data_35EEC[10] __attribute__((scn_string_literal)) = "E1032K010";
static const char data_35EF6[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_35F00[10] __attribute__((scn_string_literal)) = "E1032K008";
static const char data_35F0A[10] __attribute__((scn_string_literal)) = "E1032K004";
static const char data_35F14[10] __attribute__((scn_string_literal)) = "E1032K018";
static const char data_35F1E[10] __attribute__((scn_string_literal)) = "E1032K007";
static const char data_35F28[10] __attribute__((scn_string_literal)) = "E1032K010";
static const char data_35F32[10] __attribute__((scn_string_literal)) = "E1032K018";
static const char data_35F3C[10] __attribute__((scn_string_literal)) = "E1032K004";
static const char data_35F46[10] __attribute__((scn_string_literal)) = "E1032K010";
static const char data_35F50[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_35F5A[10] __attribute__((scn_string_literal)) = "E1032K004";
static const char data_35F64[10] __attribute__((scn_string_literal)) = "E1032K007";
static const char data_35F6E[10] __attribute__((scn_string_literal)) = "E1032K017";
static const char data_35F78[10] __attribute__((scn_string_literal)) = "E1032K004";
static const char data_35F82[10] __attribute__((scn_string_literal)) = "E1032K007";
static const char data_35F8C[10] __attribute__((scn_string_literal)) = "E1032K018";
static const char data_35F96[10] __attribute__((scn_string_literal)) = "E1032K010";
static const char data_35FA0[10] __attribute__((scn_string_literal)) = "E1032K008";
static const char data_35FAA[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_35FB4[10] __attribute__((scn_string_literal)) = "E1032K013";
static const char data_35FBE[10] __attribute__((scn_string_literal)) = "E1032K007";
static const char data_35FC8[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_35FD2[10] __attribute__((scn_string_literal)) = "E1032K004";
static const char data_35FDC[10] __attribute__((scn_string_literal)) = "E1032K013";
static const char data_35FE6[10] __attribute__((scn_string_literal)) = "E1032K016";
static const char data_35FF0[10] __attribute__((scn_string_literal)) = "E1032K014";
static const char data_35FFA[10] __attribute__((scn_string_literal)) = "E1032K007";
static const char data_36004[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_3600E[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_36018[10] __attribute__((scn_string_literal)) = "E1032L001";
static const char data_36022[10] __attribute__((scn_string_literal)) = "E1032A098";
static const char data_3602C[10] __attribute__((scn_string_literal)) = "E1032L002";
static const char data_36036[10] __attribute__((scn_string_literal)) = "E1032A099";
static const char data_36040[10] __attribute__((scn_string_literal)) = "E1032L003";
static const char data_3604A[10] __attribute__((scn_string_literal)) = "E1032L004";
static const char data_36054[10] __attribute__((scn_string_literal)) = "E1032L005";
static const char data_3605E[10] __attribute__((scn_string_literal)) = "E1032A100";
static const char data_36068[10] __attribute__((scn_string_literal)) = "E1032L001";
static const char data_36072[10] __attribute__((scn_string_literal)) = "E1032A101";
static const char data_3607C[10] __attribute__((scn_string_literal)) = "E1032L006";
static const char data_36086[10] __attribute__((scn_string_literal)) = "E1032L007";
static const char data_36090[10] __attribute__((scn_string_literal)) = "E1032L008";
static const char data_3609A[10] __attribute__((scn_string_literal)) = "E1032L009";
static const char data_360A4[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_360AE[10] __attribute__((scn_string_literal)) = "E1032A102";
static const char data_360B8[10] __attribute__((scn_string_literal)) = "E1032L010";
static const char data_360C2[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_360CC[10] __attribute__((scn_string_literal)) = "E1032L011";
static const char data_360D6[10] __attribute__((scn_string_literal)) = "E1032A101";
static const char data_360E0[10] __attribute__((scn_string_literal)) = "E1032L012";
static const char data_360EA[10] __attribute__((scn_string_literal)) = "E1032A103";
static const char data_360F4[10] __attribute__((scn_string_literal)) = "E1032L013";
static const char data_360FE[10] __attribute__((scn_string_literal)) = "E1032L014";
static const char data_36108[10] __attribute__((scn_string_literal)) = "E1032L015";
static const char data_36112[10] __attribute__((scn_string_literal)) = "E1032A103";
static const char data_3611C[10] __attribute__((scn_string_literal)) = "E1032L016";
static const char data_36126[10] __attribute__((scn_string_literal)) = "E1032A104";
static const char data_36130[10] __attribute__((scn_string_literal)) = "E1032A105";
static const char data_3613A[10] __attribute__((scn_string_literal)) = "E1032L017";
static const char data_36144[10] __attribute__((scn_string_literal)) = "E1032A106";
static const char data_3614E[10] __attribute__((scn_string_literal)) = "E1032L018";
static const char data_36158[10] __attribute__((scn_string_literal)) = "E1032A107";
static const char data_36162[10] __attribute__((scn_string_literal)) = "E1032L019";
static const char data_3616C[10] __attribute__((scn_string_literal)) = "E1032A108";
static const char data_36176[10] __attribute__((scn_string_literal)) = "E1032L020";
static const char data_36180[10] __attribute__((scn_string_literal)) = "E1032L021";
static const char data_3618A[10] __attribute__((scn_string_literal)) = "E1032A109";
static const char data_36194[10] __attribute__((scn_string_literal)) = "E1032L022";
static const char data_3619E[10] __attribute__((scn_string_literal)) = "E1032A110";
static const char data_361A8[10] __attribute__((scn_string_literal)) = "E1032L023";
static const char data_361B2[10] __attribute__((scn_string_literal)) = "E1032M001";
static const char data_361BC[10] __attribute__((scn_string_literal)) = "E1032A111";
static const char data_361C6[10] __attribute__((scn_string_literal)) = "E1032M002";
static const char data_361D0[10] __attribute__((scn_string_literal)) = "E1032A112";
static const char data_361DA[10] __attribute__((scn_string_literal)) = "E1032M003";
static const char data_361E4[10] __attribute__((scn_string_literal)) = "E1032A113";
static const char data_361EE[10] __attribute__((scn_string_literal)) = "E1032M004";
static const char data_361F8[10] __attribute__((scn_string_literal)) = "E1032A104";
static const char data_36202[10] __attribute__((scn_string_literal)) = "E1032A114";
static const char data_3620C[10] __attribute__((scn_string_literal)) = "E1032M005";
static const char data_36216[10] __attribute__((scn_string_literal)) = "E1032M006";
static const char data_36220[10] __attribute__((scn_string_literal)) = "E1032A111";
static const char data_3622A[10] __attribute__((scn_string_literal)) = "E1032M002";
static const char data_36234[10] __attribute__((scn_string_literal)) = "E1032A115";
static const char data_3623E[10] __attribute__((scn_string_literal)) = "E1032M018";
static const char data_36248[10] __attribute__((scn_string_literal)) = "E1032A122";
static const char data_36252[10] __attribute__((scn_string_literal)) = "E1032M019";
static const char data_3625C[10] __attribute__((scn_string_literal)) = "E1032A123";
static const char data_36266[10] __attribute__((scn_string_literal)) = "E1032M019";
static const char data_36270[10] __attribute__((scn_string_literal)) = "E1032M020";
static const char data_3627A[10] __attribute__((scn_string_literal)) = "E1032A104";
static const char data_36284[10] __attribute__((scn_string_literal)) = "E1032A114";
static const char data_3628E[10] __attribute__((scn_string_literal)) = "E1032M010";
static const char data_36298[10] __attribute__((scn_string_literal)) = "E1032N001";
static const char data_362A2[10] __attribute__((scn_string_literal)) = "E1032A124";
static const char data_362AC[10] __attribute__((scn_string_literal)) = "E1032A125";
static const char data_362B6[10] __attribute__((scn_string_literal)) = "E1032N002";
static const char data_362C0[10] __attribute__((scn_string_literal)) = "E1032A126";
static const char data_362CA[10] __attribute__((scn_string_literal)) = "E1032N003";
static const char data_362D4[10] __attribute__((scn_string_literal)) = "E1032A127";
static const char data_362DE[10] __attribute__((scn_string_literal)) = "E1032A128";
static const char data_362E8[10] __attribute__((scn_string_literal)) = "E1032N004";
static const char data_362F2[10] __attribute__((scn_string_literal)) = "E1032A129";
static const char data_362FC[10] __attribute__((scn_string_literal)) = "E1032N005";
static const char data_36306[10] __attribute__((scn_string_literal)) = "E1032L011";
static const char data_36310[10] __attribute__((scn_string_literal)) = "E1032A101";
static const char data_3631A[10] __attribute__((scn_string_literal)) = "E1032L019";
static const char data_36324[10] __attribute__((scn_string_literal)) = "E1032A117";
static const char data_3632E[10] __attribute__((scn_string_literal)) = "E1032L024";
static const char data_36338[10] __attribute__((scn_string_literal)) = "E1032L025";
static const char data_36342[10] __attribute__((scn_string_literal)) = "E1032L026";
static const char data_3634C[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_36356[10] __attribute__((scn_string_literal)) = "E1032A118";
static const char data_36360[10] __attribute__((scn_string_literal)) = "E1032L023";
static const char data_3636A[10] __attribute__((scn_string_literal)) = "E1032M001";
static const char data_36374[10] __attribute__((scn_string_literal)) = "E1032A111";
static const char data_3637E[10] __attribute__((scn_string_literal)) = "E1032M002";
static const char data_36388[10] __attribute__((scn_string_literal)) = "E1032A112";
static const char data_36392[10] __attribute__((scn_string_literal)) = "E1032M015";
static const char data_3639C[10] __attribute__((scn_string_literal)) = "E1032A119";
static const char data_363A6[10] __attribute__((scn_string_literal)) = "E1032M016";
static const char data_363B0[10] __attribute__((scn_string_literal)) = "E1032A104";
static const char data_363BA[10] __attribute__((scn_string_literal)) = "E1032A118";
static const char data_363C4[10] __attribute__((scn_string_literal)) = "E1032M017";
static const char data_363CE[10] __attribute__((scn_string_literal)) = "E1032O001";
static const char data_363D8[10] __attribute__((scn_string_literal)) = "E1032A136";
static const char data_363E2[10] __attribute__((scn_string_literal)) = "E1032O002";
static const char data_363EC[10] __attribute__((scn_string_literal)) = "E1032A137";
static const char data_363F6[10] __attribute__((scn_string_literal)) = "E1032O007";
static const char data_36400[10] __attribute__((scn_string_literal)) = "E1032A127";
static const char data_3640A[10] __attribute__((scn_string_literal)) = "E1032O004";
static const char data_36414[10] __attribute__((scn_string_literal)) = "E1032A138";
static const char data_3641E[10] __attribute__((scn_string_literal)) = "E1032O005";
static const char data_36428[10] __attribute__((scn_string_literal)) = "E1032A139";
static const char data_36432[10] __attribute__((scn_string_literal)) = "E1032O006";
static const char data_3643C[10] __attribute__((scn_string_literal)) = "E1032O001";
static const char data_36446[10] __attribute__((scn_string_literal)) = "E1032A136";
static const char data_36450[10] __attribute__((scn_string_literal)) = "E1032O002";
static const char data_3645A[10] __attribute__((scn_string_literal)) = "E1032A140";
static const char data_36464[10] __attribute__((scn_string_literal)) = "E1032O007";
static const char data_3646E[10] __attribute__((scn_string_literal)) = "E1032A127";
static const char data_36478[10] __attribute__((scn_string_literal)) = "E1032O004";
static const char data_36482[10] __attribute__((scn_string_literal)) = "E1032A138";
static const char data_3648C[10] __attribute__((scn_string_literal)) = "E1032O005";
static const char data_36496[10] __attribute__((scn_string_literal)) = "E1032A139";
static const char data_364A0[10] __attribute__((scn_string_literal)) = "E1032O006";
static const char data_364AA[10] __attribute__((scn_string_literal)) = "E1032P001";
static const char data_364B4[10] __attribute__((scn_string_literal)) = "E1032A085";
static const char data_364BE[10] __attribute__((scn_string_literal)) = "E1032P002";
static const char data_364C8[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_364D2[10] __attribute__((scn_string_literal)) = "E1032P003";
static const char data_364DC[10] __attribute__((scn_string_literal)) = "E1032A141";
static const char data_364E6[10] __attribute__((scn_string_literal)) = "E1032P004";
static const char data_364F0[10] __attribute__((scn_string_literal)) = "E1032A142";
static const char data_364FA[10] __attribute__((scn_string_literal)) = "E1032P005";
static const char data_36504[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_3650E[10] __attribute__((scn_string_literal)) = "E1032P001";
static const char data_36518[10] __attribute__((scn_string_literal)) = "E1032A085";
static const char data_36522[10] __attribute__((scn_string_literal)) = "E1032P006";
static const char data_3652C[10] __attribute__((scn_string_literal)) = "E1032A143";
static const char data_36536[10] __attribute__((scn_string_literal)) = "E1032P007";
static const char data_36540[10] __attribute__((scn_string_literal)) = "E1032A144";
static const char data_3654A[10] __attribute__((scn_string_literal)) = "E1032P004";
static const char data_36554[10] __attribute__((scn_string_literal)) = "E1032O008";
static const char data_3655E[10] __attribute__((scn_string_literal)) = "E1032P008";
static const char data_36568[10] __attribute__((scn_string_literal)) = "E1032A068";
static const char data_36572[10] __attribute__((scn_string_literal)) = "E1032P009";
static const char data_3657C[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_36586[10] __attribute__((scn_string_literal)) = "E1032A085";
static const char data_36590[10] __attribute__((scn_string_literal)) = "E1032P010";
static const char data_3659A[10] __attribute__((scn_string_literal)) = "E1032A145";
static const char data_365A4[10] __attribute__((scn_string_literal)) = "E1032P011";
static const char data_365AE[10] __attribute__((scn_string_literal)) = "E1032A146";
static const char data_365B8[10] __attribute__((scn_string_literal)) = "E1032P012";
static const char data_365C2[10] __attribute__((scn_string_literal)) = "E1032A147";
static const char data_365CC[10] __attribute__((scn_string_literal)) = "E1032P013";
static const char data_365D6[10] __attribute__((scn_string_literal)) = "E1032A148";
static const char data_365E0[10] __attribute__((scn_string_literal)) = "E1032P014";
static const char data_365EA[10] __attribute__((scn_string_literal)) = "E1032A149";
static const char data_365F4[10] __attribute__((scn_string_literal)) = "E1032P015";
static const char data_365FE[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_36608[10] __attribute__((scn_string_literal)) = "E1032P016";
static const char data_36612[10] __attribute__((scn_string_literal)) = "E1032A068";
static const char data_3661C[10] __attribute__((scn_string_literal)) = "E1032P017";
static const char data_36626[10] __attribute__((scn_string_literal)) = "E1032A085";
static const char data_36630[10] __attribute__((scn_string_literal)) = "E1032P018";
static const char data_3663A[10] __attribute__((scn_string_literal)) = "E1032A150";
static const char data_36644[10] __attribute__((scn_string_literal)) = "E1032P019";
static const char data_3664E[10] __attribute__((scn_string_literal)) = "E1032A151";
static const char data_36658[10] __attribute__((scn_string_literal)) = "E1032P004";
static const char data_36662[10] __attribute__((scn_string_literal)) = "E1032P016";
static const char data_3666C[10] __attribute__((scn_string_literal)) = "E1032A068";
static const char data_36676[10] __attribute__((scn_string_literal)) = "E1032P017";
static const char data_36680[10] __attribute__((scn_string_literal)) = "E1032P021";
static const char data_3668A[10] __attribute__((scn_string_literal)) = "E1032A152";
static const char data_36694[10] __attribute__((scn_string_literal)) = "E1032P010";
static const char data_3669E[10] __attribute__((scn_string_literal)) = "E1032A153";
static const char data_366A8[10] __attribute__((scn_string_literal)) = "E1032P022";
static const char data_366B2[10] __attribute__((scn_string_literal)) = "E1032A154";
static const char data_366BC[10] __attribute__((scn_string_literal)) = "E1032P023";
static const char data_366C6[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_366D0[10] __attribute__((scn_string_literal)) = "E1032P024";
static const char data_366DA[10] __attribute__((scn_string_literal)) = "E1032A068";
static const char data_366E4[10] __attribute__((scn_string_literal)) = "E1032P017";
static const char data_366EE[10] __attribute__((scn_string_literal)) = "E1032P001";
static const char data_366F8[10] __attribute__((scn_string_literal)) = "E1032A085";
static const char data_36702[10] __attribute__((scn_string_literal)) = "E1032P025";
static const char data_3670C[10] __attribute__((scn_string_literal)) = "E1032A155";
static const char data_36716[10] __attribute__((scn_string_literal)) = "E1032P004";
static const char data_36720[10] __attribute__((scn_string_literal)) = "E1032A156";
static const char data_3672A[10] __attribute__((scn_string_literal)) = "E1032O008";
static const char data_36734[10] __attribute__((scn_string_literal)) = "E1032P008";
static const char data_3673E[10] __attribute__((scn_string_literal)) = "E1032A154";
static const char data_36748[10] __attribute__((scn_string_literal)) = "E1032P027";
static const char data_36752[10] __attribute__((scn_string_literal)) = "E1032A068";
static const char data_3675C[10] __attribute__((scn_string_literal)) = "E1032P017";
static const char data_36766[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_36770[10] __attribute__((scn_string_literal)) = "E1032P021";
static const char data_3677A[10] __attribute__((scn_string_literal)) = "E1032A152";
static const char data_36784[10] __attribute__((scn_string_literal)) = "E1032P028";
static const char data_3678E[10] __attribute__((scn_string_literal)) = "E1032A158";
static const char data_36798[10] __attribute__((scn_string_literal)) = "E1032P030";
static const char data_367A2[10] __attribute__((scn_string_literal)) = "E1032A159";
static const char data_367AC[10] __attribute__((scn_string_literal)) = "E1032P066";
static const char data_367B6[10] __attribute__((scn_string_literal)) = "E1032P016";
static const char data_367C0[10] __attribute__((scn_string_literal)) = "E1032A068";
static const char data_367CA[10] __attribute__((scn_string_literal)) = "E1032P017";
static const char data_367D4[10] __attribute__((scn_string_literal)) = "E1032P021";
static const char data_367DE[10] __attribute__((scn_string_literal)) = "E1032A152";
static const char data_367E8[10] __attribute__((scn_string_literal)) = "E1032P032";
static const char data_367F2[10] __attribute__((scn_string_literal)) = "E1032A160";
static const char data_367FC[10] __attribute__((scn_string_literal)) = "E1032P033";
static const char data_36806[10] __attribute__((scn_string_literal)) = "E1032P034";
static const char data_36810[10] __attribute__((scn_string_literal)) = "E1032P080";
static const char data_3681A[10] __attribute__((scn_string_literal)) = "E1032P081";
static const char data_36824[10] __attribute__((scn_string_literal)) = "E1032A068";
static const char data_3682E[10] __attribute__((scn_string_literal)) = "E1032P035";
static const char data_36838[10] __attribute__((scn_string_literal)) = "E1032A141";
static const char data_36842[10] __attribute__((scn_string_literal)) = "E1032P036";
static const char data_3684C[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_36856[10] __attribute__((scn_string_literal)) = "E1032P021";
static const char data_36860[10] __attribute__((scn_string_literal)) = "E1032A152";
static const char data_3686A[10] __attribute__((scn_string_literal)) = "E1032P025";
static const char data_36874[10] __attribute__((scn_string_literal)) = "E1032A160";
static const char data_3687E[10] __attribute__((scn_string_literal)) = "E1032P033";
static const char data_36888[10] __attribute__((scn_string_literal)) = "E1032P082";
static const char data_36892[10] __attribute__((scn_string_literal)) = "E1032P083";
static const char data_3689C[10] __attribute__((scn_string_literal)) = "E1032P084";
static const char data_368A6[10] __attribute__((scn_string_literal)) = "E1032P085";
static const char data_368B0[10] __attribute__((scn_string_literal)) = "E1032A068";
static const char data_368BA[10] __attribute__((scn_string_literal)) = "E1032P086";
static const char data_368C4[10] __attribute__((scn_string_literal)) = "E1032P001";
static const char data_368CE[10] __attribute__((scn_string_literal)) = "E1032A164";
static const char data_368D8[10] __attribute__((scn_string_literal)) = "E1032P018";
static const char data_368E2[10] __attribute__((scn_string_literal)) = "E1032A165";
static const char data_368EC[10] __attribute__((scn_string_literal)) = "E1032P037";
static const char data_368F6[10] __attribute__((scn_string_literal)) = "E1032A104";
static const char data_36900[10] __attribute__((scn_string_literal)) = "E1032P120";
static const char data_3690A[10] __attribute__((scn_string_literal)) = "E1032A173";
static const char data_36914[10] __attribute__((scn_string_literal)) = "E1032O008";
static const char data_3691E[10] __attribute__((scn_string_literal)) = "E1032P008";
static const char data_36928[10] __attribute__((scn_string_literal)) = "E1032A154";
static const char data_36932[10] __attribute__((scn_string_literal)) = "E1032P027";
static const char data_3693C[10] __attribute__((scn_string_literal)) = "E1032A068";
static const char data_36946[10] __attribute__((scn_string_literal)) = "E1032P017";
static const char data_36950[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_3695A[10] __attribute__((scn_string_literal)) = "E1032P001";
static const char data_36964[10] __attribute__((scn_string_literal)) = "E1032A164";
static const char data_3696E[10] __attribute__((scn_string_literal)) = "E1032P018";
static const char data_36978[10] __attribute__((scn_string_literal)) = "E1032A170";
static const char data_36982[10] __attribute__((scn_string_literal)) = "E1032P044";
static const char data_3698C[10] __attribute__((scn_string_literal)) = "E1032A200";
static const char data_36996[10] __attribute__((scn_string_literal)) = "E1032A172";
static const char data_369A0[10] __attribute__((scn_string_literal)) = "E1032P045";
static const char data_369AA[10] __attribute__((scn_string_literal)) = "E1032A173";
static const char data_369B4[10] __attribute__((scn_string_literal)) = "E1032P046";
static const char data_369BE[10] __attribute__((scn_string_literal)) = "E1032P047";
static const char data_369C8[10] __attribute__((scn_string_literal)) = "E1032A201";
static const char data_369D2[10] __attribute__((scn_string_literal)) = "E1032P049";
static const char data_369DC[10] __attribute__((scn_string_literal)) = "E1032A176";
static const char data_369E6[10] __attribute__((scn_string_literal)) = "E1032P050";
static const char data_369F0[10] __attribute__((scn_string_literal)) = "E1032A177";
static const char data_369FA[10] __attribute__((scn_string_literal)) = "E1032P051";
static const char data_36A04[10] __attribute__((scn_string_literal)) = "E1032P001";
static const char data_36A0E[10] __attribute__((scn_string_literal)) = "E1032A164";
static const char data_36A18[10] __attribute__((scn_string_literal)) = "E1032P018";
static const char data_36A22[10] __attribute__((scn_string_literal)) = "E1032A178";
static const char data_36A2C[10] __attribute__((scn_string_literal)) = "E1032P004";
static const char data_36A36[10] __attribute__((scn_string_literal)) = "E1032P052";
static const char data_36A40[10] __attribute__((scn_string_literal)) = "E1032A179";
static const char data_36A4A[10] __attribute__((scn_string_literal)) = "E1032A180";
static const char data_36A54[10] __attribute__((scn_string_literal)) = "E1032A181";
static const char data_36A5E[10] __attribute__((scn_string_literal)) = "E1032P053";
static const char data_36A68[10] __attribute__((scn_string_literal)) = "E1032P087";
static const char data_36A72[10] __attribute__((scn_string_literal)) = "E1032P055";
static const char data_36A7C[10] __attribute__((scn_string_literal)) = "E1032A182";
static const char data_36A86[10] __attribute__((scn_string_literal)) = "E1032P051";
static const char data_36A90[10] __attribute__((scn_string_literal)) = "E1032P001";
static const char data_36A9A[10] __attribute__((scn_string_literal)) = "E1032A164";
static const char data_36AA4[10] __attribute__((scn_string_literal)) = "E1032P018";
static const char data_36AAE[10] __attribute__((scn_string_literal)) = "E1032A183";
static const char data_36AB8[10] __attribute__((scn_string_literal)) = "E1032P038";
static const char data_36AC2[10] __attribute__((scn_string_literal)) = "E1032A184";
static const char data_36ACC[10] __attribute__((scn_string_literal)) = "E1032P056";
static const char data_36AD6[10] __attribute__((scn_string_literal)) = "E1032P088";
static const char data_36AE0[10] __attribute__((scn_string_literal)) = "E1032P058";
static const char data_36AEA[10] __attribute__((scn_string_literal)) = "E1032A220";
static const char data_36AF4[10] __attribute__((scn_string_literal)) = "E1032A185";
static const char data_36AFE[10] __attribute__((scn_string_literal)) = "E1032P059";
static const char data_36B08[10] __attribute__((scn_string_literal)) = "E1032P001";
static const char data_36B12[10] __attribute__((scn_string_literal)) = "E1032A164";
static const char data_36B1C[10] __attribute__((scn_string_literal)) = "E1032P018";
static const char data_36B26[10] __attribute__((scn_string_literal)) = "E1032P060";
static const char data_36B30[10] __attribute__((scn_string_literal)) = "E1032A186";
static const char data_36B3A[10] __attribute__((scn_string_literal)) = "E1032P061";
static const char data_36B44[10] __attribute__((scn_string_literal)) = "E1032A187";
static const char data_36B4E[10] __attribute__((scn_string_literal)) = "E1032P062";
static const char data_36B58[10] __attribute__((scn_string_literal)) = "E1032A188";
static const char data_36B62[10] __attribute__((scn_string_literal)) = "E1032P063";
static const char data_36B6C[10] __attribute__((scn_string_literal)) = "E1032P064";
static const char data_36B76[10] __attribute__((scn_string_literal)) = "E1032P065";
static const char data_36B80[10] __attribute__((scn_string_literal)) = "E1032A189";
static const char data_36B8A[10] __attribute__((scn_string_literal)) = "E1032P066";
static const char data_36B94[10] __attribute__((scn_string_literal)) = "E1032A190";
static const char data_36B9E[10] __attribute__((scn_string_literal)) = "E1032P001";
static const char data_36BA8[10] __attribute__((scn_string_literal)) = "E1032A152";
static const char data_36BB2[10] __attribute__((scn_string_literal)) = "E1032P067";
static const char data_36BBC[10] __attribute__((scn_string_literal)) = "E1032A191";
static const char data_36BC6[10] __attribute__((scn_string_literal)) = "E1032A192";
static const char data_36BD0[10] __attribute__((scn_string_literal)) = "E1032P068";
static const char data_36BDA[10] __attribute__((scn_string_literal)) = "E1032P069";
static const char data_36BE4[10] __attribute__((scn_string_literal)) = "E1032A193";
static const char data_36BEE[10] __attribute__((scn_string_literal)) = "E1032P001";
static const char data_36BF8[10] __attribute__((scn_string_literal)) = "E1032A152";
static const char data_36C02[10] __attribute__((scn_string_literal)) = "E1032P070";
static const char data_36C0C[10] __attribute__((scn_string_literal)) = "E1032A194";
static const char data_36C16[10] __attribute__((scn_string_literal)) = "E1032P071";
static const char data_36C20[10] __attribute__((scn_string_literal)) = "E1032A195";
static const char data_36C2A[10] __attribute__((scn_string_literal)) = "E1032P072";
static const char data_36C34[10] __attribute__((scn_string_literal)) = "E1032P073";
static const char data_36C3E[10] __attribute__((scn_string_literal)) = "E1032A151";
static const char data_36C48[10] __attribute__((scn_string_literal)) = "E1032P074";
static const char data_36C52[10] __attribute__((scn_string_literal)) = "E1032P075";
static const char data_36C5C[10] __attribute__((scn_string_literal)) = "E1032P089";
static const char data_36C66[10] __attribute__((scn_string_literal)) = "E1032P090";
static const char data_36C70[10] __attribute__((scn_string_literal)) = "E1032A196";
static const char data_36C7A[10] __attribute__((scn_string_literal)) = "E1032P079";
static const char data_36C84[10] __attribute__((scn_string_literal)) = "E1032A197";
static const char data_36C8E[10] __attribute__((scn_string_literal)) = "E1032P001";
static const char data_36C98[10] __attribute__((scn_string_literal)) = "E1032A152";
static const char data_36CA2[10] __attribute__((scn_string_literal)) = "E1032P010";
static const char data_36CAC[10] __attribute__((scn_string_literal)) = "E1032P091";
static const char data_36CB6[10] __attribute__((scn_string_literal)) = "E1032P092";
static const char data_36CC0[10] __attribute__((scn_string_literal)) = "E1032A203";
static const char data_36CCA[10] __attribute__((scn_string_literal)) = "E1032P093";
static const char data_36CD4[10] __attribute__((scn_string_literal)) = "E1032P094";
static const char data_36CDE[10] __attribute__((scn_string_literal)) = "E1032A204";
static const char data_36CE8[10] __attribute__((scn_string_literal)) = "E1032P095";
static const char data_36CF2[10] __attribute__((scn_string_literal)) = "E1032P021";
static const char data_36CFC[10] __attribute__((scn_string_literal)) = "E1032A152";
static const char data_36D06[10] __attribute__((scn_string_literal)) = "E1032P096";
static const char data_36D10[10] __attribute__((scn_string_literal)) = "E1032A104";
static const char data_36D1A[10] __attribute__((scn_string_literal)) = "E1032P097";
static const char data_36D24[10] __attribute__((scn_string_literal)) = "E1032A103";
static const char data_36D2E[10] __attribute__((scn_string_literal)) = "E1032P098";
static const char data_36D38[10] __attribute__((scn_string_literal)) = "E1032P099";
static const char data_36D42[10] __attribute__((scn_string_literal)) = "E1032P100";
static const char data_36D4C[10] __attribute__((scn_string_literal)) = "E1032A206";
static const char data_36D56[10] __attribute__((scn_string_literal)) = "E1032P021";
static const char data_36D60[10] __attribute__((scn_string_literal)) = "E1032A152";
static const char data_36D6A[10] __attribute__((scn_string_literal)) = "E1032P101";
static const char data_36D74[10] __attribute__((scn_string_literal)) = "E1032A207";
static const char data_36D7E[10] __attribute__((scn_string_literal)) = "E1032P102";
static const char data_36D88[10] __attribute__((scn_string_literal)) = "E1032A208";
static const char data_36D92[10] __attribute__((scn_string_literal)) = "E1032P103";
static const char data_36D9C[10] __attribute__((scn_string_literal)) = "E1032A209";
static const char data_36DA6[10] __attribute__((scn_string_literal)) = "E1032P104";
static const char data_36DB0[10] __attribute__((scn_string_literal)) = "E1032P105";
static const char data_36DBA[10] __attribute__((scn_string_literal)) = "E1032A210";
static const char data_36DC4[10] __attribute__((scn_string_literal)) = "E1032P106";
static const char data_36DCE[10] __attribute__((scn_string_literal)) = "E1032A154";
static const char data_36DD8[10] __attribute__((scn_string_literal)) = "E1032P107";
static const char data_36DE2[10] __attribute__((scn_string_literal)) = "E1032A204";
static const char data_36DEC[10] __attribute__((scn_string_literal)) = "E1032P108";
static const char data_36DF6[10] __attribute__((scn_string_literal)) = "E1032P109";
static const char data_36E00[10] __attribute__((scn_string_literal)) = "E1032A085";
static const char data_36E0A[10] __attribute__((scn_string_literal)) = "E1032P110";
static const char data_36E14[10] __attribute__((scn_string_literal)) = "E1032A211";
static const char data_36E1E[10] __attribute__((scn_string_literal)) = "E1032P111";
static const char data_36E28[10] __attribute__((scn_string_literal)) = "E1032A104";
static const char data_36E32[10] __attribute__((scn_string_literal)) = "E1032P112";
static const char data_36E3C[10] __attribute__((scn_string_literal)) = "E1032A212";
static const char data_36E46[10] __attribute__((scn_string_literal)) = "E1032P113";
static const char data_36E50[10] __attribute__((scn_string_literal)) = "E1032A158";
static const char data_36E5A[10] __attribute__((scn_string_literal)) = "E1032P114";
static const char data_36E64[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_36E6E[10] __attribute__((scn_string_literal)) = "E1032P115";
static const char data_36E78[10] __attribute__((scn_string_literal)) = "E1032A213";
static const char data_36E82[10] __attribute__((scn_string_literal)) = "E1032P116";
static const char data_36E8C[10] __attribute__((scn_string_literal)) = "E1032P001";
static const char data_36E96[10] __attribute__((scn_string_literal)) = "E1032A152";
static const char data_36EA0[10] __attribute__((scn_string_literal)) = "E1032P117";
static const char data_36EAA[10] __attribute__((scn_string_literal)) = "E1032A214";
static const char data_36EB4[10] __attribute__((scn_string_literal)) = "E1032P118";
static const char data_36EBE[10] __attribute__((scn_string_literal)) = "E1032A154";
static const char data_36EC8[10] __attribute__((scn_string_literal)) = "E1032P119";
static const char data_36ED2[10] __attribute__((scn_string_literal)) = "E1032P120";
static const char data_36EDC[10] __attribute__((scn_string_literal)) = "E1032A215";
static const char data_36EE6[10] __attribute__((scn_string_literal)) = "E1032P121";
static const char data_36EF0[10] __attribute__((scn_string_literal)) = "E1032A163";
static const char data_36EFA[10] __attribute__((scn_string_literal)) = "E1032P123";
static const char data_36F04[10] __attribute__((scn_string_literal)) = "E1032A217";
static const char data_36F0E[10] __attribute__((scn_string_literal)) = "E1032P001";
static const char data_36F18[10] __attribute__((scn_string_literal)) = "E1032A152";
static const char data_36F22[10] __attribute__((scn_string_literal)) = "E1032P010";
static const char data_36F2C[10] __attribute__((scn_string_literal)) = "E1032P124";
static const char data_36F36[10] __attribute__((scn_string_literal)) = "E1032P125";
static const char data_36F40[10] __attribute__((scn_string_literal)) = "E1032A218";
static const char data_36F4A[10] __attribute__((scn_string_literal)) = "E1032P126";
static const char data_36F54[10] __attribute__((scn_string_literal)) = "E1032A154";
static const char data_36F5E[10] __attribute__((scn_string_literal)) = "E1032P127";
static const char data_36F68[10] __attribute__((scn_string_literal)) = "E1032A213";
static const char data_36F72[10] __attribute__((scn_string_literal)) = "E1032P128";
static const char data_36F7C[10] __attribute__((scn_string_literal)) = "E1032A204";
static const char data_36F86[10] __attribute__((scn_string_literal)) = "E1032P116";
static const char data_36F90[10] __attribute__((scn_string_literal)) = "E1032P109";
static const char data_36F9A[10] __attribute__((scn_string_literal)) = "E1032A085";
static const char data_36FA4[10] __attribute__((scn_string_literal)) = "E1032P117";
static const char data_36FAE[10] __attribute__((scn_string_literal)) = "E1032A214";
static const char data_36FB8[10] __attribute__((scn_string_literal)) = "E1032P129";
static const char data_36FC2[10] __attribute__((scn_string_literal)) = "E1032P130";
static const char data_36FCC[10] __attribute__((scn_string_literal)) = "E1032P131";
static const char data_36FD6[10] __attribute__((scn_string_literal)) = "E1032A213";
static const char data_36FE0[10] __attribute__((scn_string_literal)) = "E1032A219";
static const char data_36FEA[10] __attribute__((scn_string_literal)) = "E1032P132";
static const char data_36FF4[10] __attribute__((scn_string_literal)) = "E1032A153";
static const char data_36FFE[10] __attribute__((scn_string_literal)) = "E1032P133";
static const char data_37008[10] __attribute__((scn_string_literal)) = "E1032A220";
static const char data_37012[10] __attribute__((scn_string_literal)) = "E1032P134";
static const char data_3701C[10] __attribute__((scn_string_literal)) = "E1032A204";
static const char data_37026[10] __attribute__((scn_string_literal)) = "E1032P116";
static const char data_37030[10] __attribute__((scn_string_literal)) = "E1032A085";
static const char data_3703A[10] __attribute__((scn_string_literal)) = "E1032P010";
static const char data_37044[10] __attribute__((scn_string_literal)) = "E1032A221";
static const char data_3704E[10] __attribute__((scn_string_literal)) = "E1032P135";
static const char data_37058[10] __attribute__((scn_string_literal)) = "E1032P136";
static const char data_37062[10] __attribute__((scn_string_literal)) = "E1032A222";
static const char data_3706C[10] __attribute__((scn_string_literal)) = "E1032P137";
static const char data_37076[10] __attribute__((scn_string_literal)) = "E1032P138";
static const char data_37080[10] __attribute__((scn_string_literal)) = "E1032A223";
static const char data_3708A[10] __attribute__((scn_string_literal)) = "E1032P139";
static const char data_37094[10] __attribute__((scn_string_literal)) = "E1032A224";
static const char data_3709E[10] __attribute__((scn_string_literal)) = "E1032P140";
static const char data_370A8[10] __attribute__((scn_string_literal)) = "E1032A225";
static const char data_370B2[10] __attribute__((scn_string_literal)) = "E1032A226";
static const char data_370BC[10] __attribute__((scn_string_literal)) = "E1032P004";
static const char data_370C6[10] __attribute__((scn_string_literal)) = "E1032E001";
static const char data_370D0[10] __attribute__((scn_string_literal)) = "E1032E002";
static const char data_370DA[10] __attribute__((scn_string_literal)) = "E1032E003";
static const char data_370E4[10] __attribute__((scn_string_literal)) = "E1032A010";
static const char data_370EE[10] __attribute__((scn_string_literal)) = "E1032E004";
static const char data_370F8[10] __attribute__((scn_string_literal)) = "E1032A011";
static const char data_37102[10] __attribute__((scn_string_literal)) = "E1032E005";
static const char data_3710C[10] __attribute__((scn_string_literal)) = "E1032A012";
static const char data_37116[10] __attribute__((scn_string_literal)) = "E1032A013";
static const char data_37120[10] __attribute__((scn_string_literal)) = "E1032E006";
static const char data_3712A[10] __attribute__((scn_string_literal)) = "E1032A014";
static const char data_37134[10] __attribute__((scn_string_literal)) = "E1032A015";
static const char data_3713E[10] __attribute__((scn_string_literal)) = "E1032E007";
static const char data_37148[10] __attribute__((scn_string_literal)) = "E1032A016";
static const char data_37152[10] __attribute__((scn_string_literal)) = "E1032E008";
static const char data_3715C[10] __attribute__((scn_string_literal)) = "E1032E009";
static const char data_37166[10] __attribute__((scn_string_literal)) = "E1032A017";
static const char data_37170[10] __attribute__((scn_string_literal)) = "E1032E010";
static const char data_3717A[10] __attribute__((scn_string_literal)) = "E1032E011";
static const char data_37184[10] __attribute__((scn_string_literal)) = "E1032E012";
static const char data_3718E[10] __attribute__((scn_string_literal)) = "E1032E013";
static const char data_37198[10] __attribute__((scn_string_literal)) = "E1032A018";
static const char data_371A2[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_371AC[10] __attribute__((scn_string_literal)) = "E1032A020";
static const char data_371B6[10] __attribute__((scn_string_literal)) = "E1032A021";
static const char data_371C0[10] __attribute__((scn_string_literal)) = "E1032F001";
static const char data_371CA[10] __attribute__((scn_string_literal)) = "E1032F002";
static const char data_371D4[10] __attribute__((scn_string_literal)) = "E1032F003";
static const char data_371DE[10] __attribute__((scn_string_literal)) = "E1032A022";
static const char data_371E8[10] __attribute__((scn_string_literal)) = "E1032F004";
static const char data_371F2[10] __attribute__((scn_string_literal)) = "E1032A011";
static const char data_371FC[10] __attribute__((scn_string_literal)) = "E1032F005";
static const char data_37206[10] __attribute__((scn_string_literal)) = "E1032A012";
static const char data_37210[10] __attribute__((scn_string_literal)) = "E1032A013";
static const char data_3721A[10] __attribute__((scn_string_literal)) = "E1032F006";
static const char data_37224[10] __attribute__((scn_string_literal)) = "E1032A023";
static const char data_3722E[10] __attribute__((scn_string_literal)) = "E1032A024";
static const char data_37238[10] __attribute__((scn_string_literal)) = "E1032F007";
static const char data_37242[10] __attribute__((scn_string_literal)) = "E1032A025";
static const char data_3724C[10] __attribute__((scn_string_literal)) = "E1032A026";
static const char data_37256[10] __attribute__((scn_string_literal)) = "E1032F008";
static const char data_37260[10] __attribute__((scn_string_literal)) = "E1032F009";
static const char data_3726A[10] __attribute__((scn_string_literal)) = "E1032F010";
static const char data_37274[10] __attribute__((scn_string_literal)) = "E1032F011";
static const char data_3727E[10] __attribute__((scn_string_literal)) = "E1032F012";
static const char data_37288[10] __attribute__((scn_string_literal)) = "E1032F013";
static const char data_37292[10] __attribute__((scn_string_literal)) = "E1032F014";
static const char data_3729C[10] __attribute__((scn_string_literal)) = "E1032F015";
static const char data_372A6[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_372B0[10] __attribute__((scn_string_literal)) = "E1032A018";
static const char data_372BA[10] __attribute__((scn_string_literal)) = "E1032A027";
static const char data_372C4[10] __attribute__((scn_string_literal)) = "E1032A034";
static const char data_372CE[10] __attribute__((scn_string_literal)) = "E1032H005";
static const char data_372D8[10] __attribute__((scn_string_literal)) = "E1032H004";
static const char data_372E2[10] __attribute__((scn_string_literal)) = "E1032H009";
static const char data_372EC[10] __attribute__((scn_string_literal)) = "E1032H006";
static const char data_372F6[10] __attribute__((scn_string_literal)) = "E1032A042";
static const char data_37300[10] __attribute__((scn_string_literal)) = "E1032A040";
static const char data_3730A[10] __attribute__((scn_string_literal)) = "E1032A043";
static const char data_37314[10] __attribute__((scn_string_literal)) = "E1032A041";
static const char data_3731E[10] __attribute__((scn_string_literal)) = "E1032A045";
static const char data_37328[10] __attribute__((scn_string_literal)) = "E1032A046";
static const char data_37332[10] __attribute__((scn_string_literal)) = "E1032A047";
static const char data_3733C[10] __attribute__((scn_string_literal)) = "E1032A046";
static const char data_37346[10] __attribute__((scn_string_literal)) = "E1032A047";
static const char data_37350[10] __attribute__((scn_string_literal)) = "E1032A044";
static const char data_3735A[10] __attribute__((scn_string_literal)) = "E1032A051";
static const char data_37364[10] __attribute__((scn_string_literal)) = "E1032A039";
static const char data_3736E[10] __attribute__((scn_string_literal)) = "E1032H007";
static const char data_37378[10] __attribute__((scn_string_literal)) = "E1032A120";
static const char data_37382[10] __attribute__((scn_string_literal)) = "E1032H010";
static const char data_3738C[10] __attribute__((scn_string_literal)) = "E1032A066";
static const char data_37396[10] __attribute__((scn_string_literal)) = "E1032H011";
static const char data_373A0[10] __attribute__((scn_string_literal)) = "E1032J001";
static const char data_373AA[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_373B4[10] __attribute__((scn_string_literal)) = "E1032A131";
static const char data_373BE[10] __attribute__((scn_string_literal)) = "E1032J010";
static const char data_373C8[10] __attribute__((scn_string_literal)) = "E1032J013";
static const char data_373D2[10] __attribute__((scn_string_literal)) = "E1032J001";
static const char data_373DC[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_373E6[10] __attribute__((scn_string_literal)) = "E1032A134";
static const char data_373F0[10] __attribute__((scn_string_literal)) = "E1032J015";
static const char data_373FA[10] __attribute__((scn_string_literal)) = "E1032A135";
static const char data_37404[10] __attribute__((scn_string_literal)) = "E1032J001";
static const char data_3740E[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_37418[10] __attribute__((scn_string_literal)) = "E1032J018";
static const char data_37422[10] __attribute__((scn_string_literal)) = "E1032A163";
static const char data_3742C[10] __attribute__((scn_string_literal)) = "E1032J019";
static const char data_37436[10] __attribute__((scn_string_literal)) = "E1032J020";
static const char data_37440[10] __attribute__((scn_string_literal)) = "E1032A068";
static const char data_3744A[10] __attribute__((scn_string_literal)) = "E1032J016";
static const char data_37454[10] __attribute__((scn_string_literal)) = "E1032A161";
static const char data_3745E[10] __attribute__((scn_string_literal)) = "E1032J017";
static const char data_37468[10] __attribute__((scn_string_literal)) = "E1032A162";
static const char data_37472[10] __attribute__((scn_string_literal)) = "E1032J001";
static const char data_3747C[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_37486[10] __attribute__((scn_string_literal)) = "E1032A131";
static const char data_37490[10] __attribute__((scn_string_literal)) = "E1032J010";
static const char data_3749A[10] __attribute__((scn_string_literal)) = "E1032A132";
static const char data_374A4[10] __attribute__((scn_string_literal)) = "E1032J011";
static const char data_374AE[10] __attribute__((scn_string_literal)) = "E1032J012";
static const char data_374B8[10] __attribute__((scn_string_literal)) = "E1032H007";
static const char data_374C2[10] __attribute__((scn_string_literal)) = "E1032A120";
static const char data_374CC[10] __attribute__((scn_string_literal)) = "E1032H010";
static const char data_374D6[10] __attribute__((scn_string_literal)) = "E1032A066";
static const char data_374E0[10] __attribute__((scn_string_literal)) = "E1032H011";
static const char data_374EA[10] __attribute__((scn_string_literal)) = "E1032J001";
static const char data_374F4[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_374FE[10] __attribute__((scn_string_literal)) = "E1032J018";
static const char data_37508[10] __attribute__((scn_string_literal)) = "E1032A163";
static const char data_37512[10] __attribute__((scn_string_literal)) = "E1032J019";
static const char data_3751C[10] __attribute__((scn_string_literal)) = "E1032J020";
static const char data_37526[10] __attribute__((scn_string_literal)) = "E1032A068";
static const char data_37530[10] __attribute__((scn_string_literal)) = "E1032J016";
static const char data_3753A[10] __attribute__((scn_string_literal)) = "E1032A161";
static const char data_37544[10] __attribute__((scn_string_literal)) = "E1032J017";
static const char data_3754E[10] __attribute__((scn_string_literal)) = "E1032A162";
static const char data_37558[10] __attribute__((scn_string_literal)) = "E1032J001";
static const char data_37562[10] __attribute__((scn_string_literal)) = "E1032A067";
static const char data_3756C[10] __attribute__((scn_string_literal)) = "E1032J002";
static const char data_37576[10] __attribute__((scn_string_literal)) = "E1032A068";
static const char data_37580[10] __attribute__((scn_string_literal)) = "E1032J001";
static const char data_3758A[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_37594[10] __attribute__((scn_string_literal)) = "E1032J003";
static const char data_3759E[10] __attribute__((scn_string_literal)) = "E1032A069";
static const char data_375A8[10] __attribute__((scn_string_literal)) = "E1032J004";
static const char data_375B2[10] __attribute__((scn_string_literal)) = "E1032A070";
static const char data_375BC[10] __attribute__((scn_string_literal)) = "E1032H007";
static const char data_375C6[10] __attribute__((scn_string_literal)) = "E1032A048";
static const char data_375D0[10] __attribute__((scn_string_literal)) = "E1032H008";
static const char data_375DA[10] __attribute__((scn_string_literal)) = "E1032A049";
static const char data_375E4[10] __attribute__((scn_string_literal)) = "E1032A050";
static const char data_375EE[16] __attribute__((scn_string_literal)) = "/scene/01/JD00/";
static const char data_375FE[5] __attribute__((scn_string_literal)) = "TBOX";
static const char data_37603[5] __attribute__((scn_string_literal)) = "TBOX";
static const char data_37608[7] __attribute__((scn_string_literal)) = "SA1087";
static const char data_3760F[11] __attribute__((scn_string_literal)) = "SA1087A001";
static const char data_3761A[11] __attribute__((scn_string_literal)) = "SA1087A002";
static const char data_37625[11] __attribute__((scn_string_literal)) = "SA1087A003";
static const char data_37630[11] __attribute__((scn_string_literal)) = "SA1087A004";
static const char data_3763B[11] __attribute__((scn_string_literal)) = "SA1087A005";
static const char data_37646[11] __attribute__((scn_string_literal)) = "SA1087A006";
static const char data_37651[7] __attribute__((scn_string_literal)) = "SA1087";
static const uint32_t data_37658 = 0xC18DE1B1;
static const float data_3765C[2] = {0.225f, -0.0935f};
static const int32_t data_37664[3] = {63169, 1048832, 6};
static const float data_37670[3] = {-17.4023f, 1.574f, -1.5519f};
static const int32_t data_3767C[3] = {30401, 264, 6};
static const float data_37688[3] = {-10.4327f, 0.225f, 21.8722f};
static const int32_t data_37694[3] = {16384, 1048832, 5};
static const float data_376A0[2] = {-11.6312f, 1.1495f};
static const uint32_t data_376A8 = 0x41AF1FBEU;
static const int32_t data_376AC[3] = {49152, 264, 5};
static const float data_376B8[3] = {-14.4036f, 0.225f, 7.0609f};
static const int32_t data_376C4[3] = {16384, 1048576, 5};
static const float data_376D0[3] = {-15.5966f, 1.0f, 7.0721f};
static const int32_t data_376DC[3] = {49152, 8, 5};
static const uint32_t data_376E8 = 0xC1C4F247U;
static const float data_376EC[2] = {1.2587f, 6.2984f};
static const int32_t data_376F4[3] = {32768, 1048576, 8};
static const uint32_t data_37700 = 0xC1C4E944U;
static const float data_37704[3] = {3.0927f, 8.4006f, 0.0f};
static const int32_t data_37710[2] = {8, 8};
static const float data_37718[3] = {14.462f, 0.2094f, 28.3062f};
static const int32_t data_37724[3] = {64261, 1048576, 3};
static const float data_37730[3] = {14.6576f, 0.6583f, 27.645f};
static const int32_t data_3773C[3] = {31493, 8, 3};
static const float data_37748[2] = {-13.8104f, 1.3233f};
static const uint32_t data_37750 = 0x41C3D8AEU;
static const int32_t data_37754[3] = {30219, 1064960, 12};
static const float data_37760[2] = {-14.6088f, 3.8117f};
static const uint32_t data_37768 = 0x41DD73EBU;
static const int32_t data_3776C[3] = {62987, 16392, 12};
static const float data_37778[2] = {-13.0535f, 1.3156f};
static const uint32_t data_37780 = 0x41F68D84U;
static const int32_t data_37784[3] = {32768, 1064960, 12};
static const float data_37790[4] = {-13.0292f, 3.8062f, 34.1213f, 0.0f};
static const int32_t data_377A0[2] = {16392, 12};
static const float data_377A8 = 100000.0f;
static const uint8_t data_377AC[8] = {0};
typedef struct Row_377B4 { float w0; int32_t w1; int32_t w2; } Row_377B4;
static const Row_377B4 data_377B4[3] = {
  {0.5f, 0, 1},
  {0.5f, 0, 2},
  {0.4f, 0, 3}
};
static const float data_377D8 = 0.4f;
static const int32_t data_377DC = 3;
static const char data_377E0[16] __attribute__((scn_string_literal)) = "/scene/01/JD00/";
static const char data_377F0[5] __attribute__((scn_string_literal)) = "MAPY";
static const char data_377F5[16] __attribute__((scn_string_literal)) = "/scene/01/JD00/";
static const char data_37805[5] __attribute__((scn_string_literal)) = "MAPN";
static const char data_3780A[16] __attribute__((scn_string_literal)) = "/scene/01/JD00/";
static const char data_3781A[5] __attribute__((scn_string_literal)) = "MAPS";
static const char data_3781F[16] __attribute__((scn_string_literal)) = "/scene/01/JD00/";
static const char data_3782F[5] __attribute__((scn_string_literal)) = "JDYA";
static const char data_37834[5] __attribute__((scn_string_literal)) = "JDYA";
static const char data_37839[1] __attribute__((scn_string_literal)) = "";
static const char data_3783A[5] __attribute__((scn_string_literal)) = "JDYA";
static const char data_3783F[1] __attribute__((scn_string_literal)) = "";
static const char data_37840[8] __attribute__((scn_string_literal)) = "JDYA";
static void (*data_37848)(void) = script_224D1;
static void (*const data_3784C)(int32_t) = script_224D4;
static void (*const data_37850)(int32_t) = script_224F7;
static void (*const data_37854)(int32_t) = script_2251A;
static const Vector4 data_37858 = {.x = 1.0f, .y = 0.4f, .z = 0.4f, .w = 0.4f};
static const Vector4 data_37868 = {.x = 1.0f, .y = 0.6f, .z = 0.6f, .w = 0.6f};
static const char data_37878[16] __attribute__((scn_string_literal)) = "/scene/01/JD00/";
static const char data_37888[5] __attribute__((scn_string_literal)) = "DNOZ";
static const char data_3788D[5] __attribute__((scn_string_literal)) = "DNOZ";
static const char data_37892[1] __attribute__((scn_string_literal)) = "";
static const char data_37893[5] __attribute__((scn_string_literal)) = "DNOZ";
static const char data_37898[1] __attribute__((scn_string_literal)) = "";
static const char data_37899[7] __attribute__((scn_string_literal)) = "01FULB";
static const char data_378A0[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_378A5[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_378AA[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_378AF[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_378B4[11] __attribute__((scn_string_literal)) = "bgm068.snd";
static const char data_378BF[13] __attribute__((scn_string_literal)) = "a1_nonam.snd";
static const FogColourData data_378CC = {.red = 16.7f, .green = 780.0f, .blue = 1.0f, .colour = 0xFF0D0705U};
static const uint32_t data_378DC[4] = {0x320E102EU, 0x320E102EU, 0x64555462U, 0x64555462U};
static const Vector4 data_378EC = {.x = 0.25f, .y = 0.09f, .z = 0.19f, .w = 0.31f};
static const Vector4 data_378FC = {.x = 1.0f, .y = 0.8f, .z = 0.63f, .w = 0.4f};
static const float data_3790C[2] = {0.69f, 1.0f};
static const Vector4 data_37914 = {.x = 1.0f, .y = 0.4f, .z = 0.4f, .w = 0.4f};
static const Vector4 data_37924 = {.x = 1.0f, .y = 0.6f, .z = 0.6f, .w = 0.6f};
static const char data_37934[] __attribute__((scn_string_literal)) = "sprite";
static const char data_3793B[13] __attribute__((scn_string_literal)) = "SCROLL27.SPR";
static const char data_37948[16] __attribute__((scn_string_literal)) = "/scene/01/JD00/";
static const char data_37958[5] __attribute__((scn_string_literal)) = "KAME";
static const char data_3795D[7] __attribute__((scn_string_literal)) = "E1044";
static const Vector3 data_37964 = {.x = 19.0f, .y = 0.0f, .z = -9.6f};
static const char data_37970[6] __attribute__((scn_string_literal)) = "E1044";
static const char data_37976[6] __attribute__((scn_string_literal)) = "KAME";
static const Vector3 data_3797C = {.x = 9.5f, .y = 0.0f, .z = -2.3f};
static const Vector3 data_37988 = {.x = 26.0f, .y = 0.0f, .z = -19.0f};
static char data_37994[] = "E1044A006";
static char data_3799E[] = "E1044A007";
static char data_379A8[] = "E1044A008";
static const uint8_t data_379B2[2] = {0};
static char *data_379B4 = data_37994;
static char *const data_379B8 = data_3799E;
static char *const data_379BC = data_379A8;
static const char data_379C0[10] __attribute__((scn_string_literal)) = "JINGURU01";
static const char data_379CA[10] __attribute__((scn_string_literal)) = "E1044A011";
static const char data_379D4[10] __attribute__((scn_string_literal)) = "E1044A009";
static const char data_379DE[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_379E3[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_379E8[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_379ED[11] __attribute__((scn_string_literal)) = "bgm203.snd";
static const uint32_t data_379F8 = 1;
typedef struct Row_379FC { float w0; float w1; float w2; int32_t w3; } Row_379FC;
static const Row_379FC data_379FC[3] = {
  {9.7f, 20.5f, 0.0f, 2},
  {11.0f, 21.5f, 1.0f, 3},
  {11.0f, 26.0f, 0.0f, 196608}
};
static const char data_37A2C[10] __attribute__((scn_string_literal)) = "E1044B001";
static const char data_37A36[10] __attribute__((scn_string_literal)) = "E1044A001";
static const char data_37A40[10] __attribute__((scn_string_literal)) = "E1044B002";
static const char data_37A4A[10] __attribute__((scn_string_literal)) = "E1044A002";
static const char data_37A54[10] __attribute__((scn_string_literal)) = "E1044B003";
static const char data_37A5E[10] __attribute__((scn_string_literal)) = "E1044B004";
static const char data_37A68[10] __attribute__((scn_string_literal)) = "E1044A004";
static const char data_37A72[10] __attribute__((scn_string_literal)) = "E1044A005";
static const char data_37A7C[10] __attribute__((scn_string_literal)) = "E1044B006";
static const char data_37A86[10] __attribute__((scn_string_literal)) = "E1044A012";
static const char data_37A90[10] __attribute__((scn_string_literal)) = "E1044B007";
static const char data_37A9A[10] __attribute__((scn_string_literal)) = "E1044A013";
static const char data_37AA4[10] __attribute__((scn_string_literal)) = "E1044B008";
static const char data_37AAE[10] __attribute__((scn_string_literal)) = "E1044A014";
static const char data_37AB8[10] __attribute__((scn_string_literal)) = "E1044B009";
static const char data_37AC2[10] __attribute__((scn_string_literal)) = "E1044B010";
static const char data_37ACC[10] __attribute__((scn_string_literal)) = "E1044B011";
static const char data_37AD6[10] __attribute__((scn_string_literal)) = "E1044A015";
static const char data_37AE0[10] __attribute__((scn_string_literal)) = "E1044A016";
static const char data_37AEA[10] __attribute__((scn_string_literal)) = "E1044B012";
static const char data_37AF4[12] __attribute__((scn_string_literal)) = "E1044B013";
static const uint32_t data_37B00 = 1;
static const float data_37B04[3] = {-12.36f, 16.81f, 1.0f};
static const int32_t data_37B10 = 196608;
static const char data_37B14[6] __attribute__((scn_string_literal)) = "E1049";
static const char data_37B1A[5] __attribute__((scn_string_literal)) = "KOJI";
static const char data_37B1F[9] __attribute__((scn_string_literal)) = "E1049";
static const Vector3 data_37B28 = {.x = 1.0f, .y = 1.0f, .z = 1.0f};
static const float data_37B34[3] = {1.0f, 1.0f, 0.05f};
static const char data_37B40[5] __attribute__((scn_string_literal)) = "KOJI";
static const char data_37B45[5] __attribute__((scn_string_literal)) = "KOJI";
static const char data_37B4A[] __attribute__((scn_string_literal)) = "DATD";
static const char data_37B4F[5] __attribute__((scn_string_literal)) = "KOJI";
static const char data_37B54[5] __attribute__((scn_string_literal)) = "KOJI";
static const char data_37B59[] __attribute__((scn_string_literal)) = "DATD";
static const char data_37B5E[] __attribute__((scn_string_literal)) = "DATD";
static const char data_37B63[5] __attribute__((scn_string_literal)) = "KOJI";
static const char data_37B68[5] __attribute__((scn_string_literal)) = "KOJI";
static const char data_37B6D[] __attribute__((scn_string_literal)) = "DATB";
static const char data_37B72[5] __attribute__((scn_string_literal)) = "KOJI";
static const char data_37B77[5] __attribute__((scn_string_literal)) = "KOJI";
static const char data_37B7C[] __attribute__((scn_string_literal)) = "DATC";
static const char data_37B81[5] __attribute__((scn_string_literal)) = "KOJI";
static const char data_37B86[5] __attribute__((scn_string_literal)) = "KOJI";
static const char data_37B8B[] __attribute__((scn_string_literal)) = "DATC";
static const char data_37B90[5] __attribute__((scn_string_literal)) = "KOJI";
static const char data_37B95[5] __attribute__((scn_string_literal)) = "KOJI";
static const char data_37B9A[] __attribute__((scn_string_literal)) = "DATD";
static const char data_37B9F[5] __attribute__((scn_string_literal)) = "KOJI";
static const char data_37BA4[5] __attribute__((scn_string_literal)) = "KOJI";
static const char data_37BA9[] __attribute__((scn_string_literal)) = "DATD";
static const char data_37BAE[5] __attribute__((scn_string_literal)) = "KOJI";
static const char data_37BB3[5] __attribute__((scn_string_literal)) = "KOJI";
static const char data_37BB8[] __attribute__((scn_string_literal)) = "DATD";
static const char data_37BBD[10] __attribute__((scn_string_literal)) = "E1049B006";
static const char data_37BC7[10] __attribute__((scn_string_literal)) = "E1049A001";
static const char data_37BD1[10] __attribute__((scn_string_literal)) = "E1049B007";
static const char data_37BDB[10] __attribute__((scn_string_literal)) = "E1049B002";
static const char data_37BE5[10] __attribute__((scn_string_literal)) = "E1049A002";
static const char data_37BEF[10] __attribute__((scn_string_literal)) = "E1049B003";
static const char data_37BF9[10] __attribute__((scn_string_literal)) = "E1049A009";
static const char data_37C03[10] __attribute__((scn_string_literal)) = "E1049C011";
static const char data_37C0D[10] __attribute__((scn_string_literal)) = "E1049A010";
static const char data_37C17[10] __attribute__((scn_string_literal)) = "E1049C012";
static const char data_37C21[10] __attribute__((scn_string_literal)) = "E1049C005";
static const char data_37C2B[10] __attribute__((scn_string_literal)) = "E1049A006";
static const char data_37C35[10] __attribute__((scn_string_literal)) = "E1049B008";
static const char data_37C3F[10] __attribute__((scn_string_literal)) = "E1049C006";
static const char data_37C49[10] __attribute__((scn_string_literal)) = "E1049A007";
static const char data_37C53[10] __attribute__((scn_string_literal)) = "E1049C008";
static const char data_37C5D[10] __attribute__((scn_string_literal)) = "E1049C008";
static const char data_37C67[10] __attribute__((scn_string_literal)) = "E1049C008";
static const char data_37C71[10] __attribute__((scn_string_literal)) = "E1049C010";
static const char data_37C7B[10] __attribute__((scn_string_literal)) = "E1049A008";
static const char data_37C85[10] __attribute__((scn_string_literal)) = "E1049B009";
static const char data_37C8F[10] __attribute__((scn_string_literal)) = "E1049A011";
static const char data_37C99[10] __attribute__((scn_string_literal)) = "E1049B010";
static const char data_37CA3[10] __attribute__((scn_string_literal)) = "E1049B011";
static const char data_37CAD[10] __attribute__((scn_string_literal)) = "E1049A012";
static const char data_37CB7[10] __attribute__((scn_string_literal)) = "E1049B012";
static const char data_37CC1[10] __attribute__((scn_string_literal)) = "E1049A013";
static const char data_37CCB[10] __attribute__((scn_string_literal)) = "E1049B013";
static const char data_37CD5[10] __attribute__((scn_string_literal)) = "E1049C009";
static const char data_37CDF[10] __attribute__((scn_string_literal)) = "E1049C014";
static const char data_37CE9[5] __attribute__((scn_string_literal)) = "KOJI";
static const char data_37CEE[5] __attribute__((scn_string_literal)) = "KOJI";
static const char data_37CF3[] __attribute__((scn_string_literal)) = "DATA";
static const char data_37CF8[16] __attribute__((scn_string_literal)) = "/scene/01/JD00/";
static const char data_37D08[5] __attribute__((scn_string_literal)) = "KOJI";
static const char data_37D0D[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_37D12[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_37D17[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_37D1C[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_37D21[11] __attribute__((scn_string_literal)) = "bgm013.snd";
static const char data_37D2C[13] __attribute__((scn_string_literal)) = "e1koujiq.snd";
static const char data_37D39[13] __attribute__((scn_string_literal)) = "battle_1.snd";
static const char data_37D46[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_37D4B[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_37D50[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_37D55[7] __attribute__((scn_string_literal)) = "SELECT";
static const char data_37D5C[16] __attribute__((scn_string_literal)) = "f1jutadn.snd";
static const uint32_t data_37D6C = 0x3D9374BC;
static const float data_37D70[2] = {0.027f, -0.1417f};
static const uint32_t data_37D78 = 0x3C1374BC;
static const float data_37D7C[2] = {0.067f, -0.117f};
static const Vector3i data_37D84 = {.x = 0, .y = 0, .z = 0};
static const char data_37D90[9] __attribute__((scn_string_literal)) = "DR15_019";
static const char data_37D99[11] __attribute__((scn_string_literal)) = "DR15_018";
static const Vector4 data_37DA4 = {.x = 1.0f, .y = 0.3f, .z = 0.3f, .w = 0.3f};
static const char data_37DB4[6] __attribute__((scn_string_literal)) = "01NVE";
static const char data_37DBA[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_37DBF[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_37DC4[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_37DC9[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_37DCE[11] __attribute__((scn_string_literal)) = "bgm001.snd";
static const char data_37DD9[13] __attribute__((scn_string_literal)) = "a1_nzmvs.snd";
static const char data_37DE6[13] __attribute__((scn_string_literal)) = "battle_1.snd";
static const char data_37DF3[] __attribute__((scn_string_literal)) = "/scene/01/JD00/";
static const char data_37E03[] __attribute__((scn_string_literal)) = "EVSN";
static const char data_37E08[5] __attribute__((scn_string_literal)) = "EVSN";
static const char data_37E0D[6] __attribute__((scn_string_literal)) = "01NVE";
static const char data_37E13[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_37E18[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_37E1D[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_37E22[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_37E27[7] __attribute__((scn_string_literal)) = "SELECT";
static const char data_37E2E[14] __attribute__((scn_string_literal)) = "f1jutadn.snd";
static const Vector3 data_37E3C = {.x = 20.2f, .y = 0.0f, .z = -8.5f};
static const char data_37E48[11] __attribute__((scn_string_literal)) = "M_FGT1.BIN";
static const char data_37E53[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_37E58[11] __attribute__((scn_string_literal)) = "bgm013.snd";
static const char data_37E63[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_37E68[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_37E6D[11] __attribute__((scn_string_literal)) = "bgm001.snd";
static const char data_37E78[13] __attribute__((scn_string_literal)) = "a1_nzmvs.snd";
static const char data_37E85[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_37E8A[14] __attribute__((scn_string_literal)) = "bgm013.snd";
static const uint32_t data_37E98 = 2;
static const int32_t data_37E9C[7] = {2, 512, 4, 1024, 1024, 1024, -1};
static const char data_37EB8[5] __attribute__((scn_string_literal)) = "EVSN";
static const char data_37EBD[5] __attribute__((scn_string_literal)) = "EVSN";
static const char data_37EC2[5] __attribute__((scn_string_literal)) = "EVSN";
static const char data_37EC7[1] __attribute__((scn_string_literal)) = "";
static const char data_37EC8[5] __attribute__((scn_string_literal)) = "EVSN";
static const char data_37ECD[5] __attribute__((scn_string_literal)) = "EVSN";
static const char data_37ED2[5] __attribute__((scn_string_literal)) = "EVSN";
static const char data_37ED7[1] __attribute__((scn_string_literal)) = "";
static const char data_37ED8[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_37EDD[11] __attribute__((scn_string_literal)) = "bgm001.snd";
static const char data_37EE8[5] __attribute__((scn_string_literal)) = "EVSN";
static const char data_37EED[5] __attribute__((scn_string_literal)) = "EVSN";
static const char data_37EF2[5] __attribute__((scn_string_literal)) = "EVSN";
static const char data_37EF7[1] __attribute__((scn_string_literal)) = "";
static const char data_37EF8[5] __attribute__((scn_string_literal)) = "EVSN";
static const char data_37EFD[5] __attribute__((scn_string_literal)) = "EVSN";
static const char data_37F02[5] __attribute__((scn_string_literal)) = "EVSN";
static const char data_37F07[1] __attribute__((scn_string_literal)) = "";
static const char data_37F08[11] __attribute__((scn_string_literal)) = "M_MOBJ.BIN";
static const char data_37F13[16] __attribute__((scn_string_literal)) = "/scene/01/JD00/";
static const char data_37F23[5] __attribute__((scn_string_literal)) = "JD00";
static const char data_37F28[14] __attribute__((scn_string_literal)) = "scene/01/D000";
static const char data_37F36[] __attribute__((scn_string_literal)) = "SCRTEST.BIN";
static const uint8_t data_37F42[2] = {0};
static const uint32_t data_37F44 = 380;
static const int32_t data_37F48[6] = {1, 390, 0, 1018, 0, -1};
static const uint32_t data_37F60 = 510;
static const int32_t data_37F64[6] = {1, 525, 0, 1020, 0, -1};
static const uint32_t data_37F7C = 150;
static const int32_t data_37F80[6] = {1, 160, 0, 1019, 0, -1};
static const uint32_t data_37F98 = 530;
static const int32_t data_37F9C[6] = {1, 970, 0, 1014, 0, -1};
static const uint32_t data_37FB4 = 911;
static const int32_t data_37FB8[6] = {1, 155, 0, 1015, 0, -1};
static const char data_37FD0[7] __attribute__((scn_string_literal)) = "FREE02";
static const char data_37FD7[7] __attribute__((scn_string_literal)) = "FREE03";
static const char data_37FDE[7] __attribute__((scn_string_literal)) = "FREE04";
static const char data_37FE5[7] __attribute__((scn_string_literal)) = "FREE01";
static const uint32_t data_37FEC = 0xC1F00000;
static const float data_37FF0[2] = {0.0f, 30.0f};
static const uint32_t data_37FF8 = 0x40C00000;
static const float data_37FFC[2] = {0.0f, 44.0f};
static const uint32_t data_38004 = 0x41100000;
static const float data_38008[2] = {0.0f, 35.0f};
static const uint32_t data_38010 = 0x41E00000;
static const float data_38014[2] = {0.0f, 45.0f};
static const uint32_t data_3801C = 0xC2480000;
static const uint8_t pad_38020[8] = {0};
static const uint32_t data_38028 = 0x40E00000;
static const float data_3802C[2] = {0.0f, -2.0f};
static const uint32_t data_38034 = 0x41400000;
static const float data_38038[2] = {0.0f, -22.0f};
static const uint8_t data_38040[12] = {0};
static const uint8_t data_3804C[12] = {0};
static const uint8_t data_38058[12] = {0};
static const uint8_t data_38064[12] = {0};
static const uint32_t data_38070 = 0x3F800000;
static const float data_38074[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.8f, 0.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint32_t data_380F8 = 0x3F800000;
static const float data_380FC[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.8f, 0.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint32_t data_38180 = 0x3F800000;
static const float data_38184[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.8f, 0.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint32_t data_38208 = 0x3F800000;
static const float data_3820C[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.8f, 0.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint32_t data_38290 = 0x3F800000;
static const float data_38294[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.8f, 0.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint32_t data_38318 = 0x3F800000;
static const float data_3831C[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.8f, 0.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint32_t data_383A0 = 0x3F800000;
static const float data_383A4[33] = {
  0.944f, 0.866f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 1.0f, 1.0f, 1.0f,
  0.8f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint8_t data_38428[80] = {0};
typedef struct Row_38478 { float w0; float w1; float w2; int32_t w3; } Row_38478;
static const Row_38478 data_38478[3] = {
  {0.8f, 0.8f, 0.8f, 0},
  {0.0f, 0.0f, 0.0f, 0},
  {0.0f, 0.0f, 0.0f, 0}
};
static const int32_t data_384A8[2] = {0, 0};
static const uint8_t data_384B0[96] = {0};
static const float data_38510[10] = {0.8f, 0.8f, 0.8f, 0.8f, 0.8f, 0.8f, 0.8f, 0.8f, 0.8f, 0.8f};
static const uint32_t data_38538 = 0x3F800000;
static const float data_3853C[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint32_t data_385C0 = 0x3F800000;
static const float data_385C4[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const char data_38648[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_3864D[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_38652[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_38657[7] __attribute__((scn_string_literal)) = "SELECT";
static const char data_3865E[14] __attribute__((scn_string_literal)) = "f1jutadn.snd";
static const uint32_t data_3866C = 0xC14170A4;
static const float data_38670[123] = {
  36.17f, -20.88f, 37.92f, -20.91f, 31.44f, -12.46f, 30.47f, 0.0f, 7.5f, -16.44f, 3.5f, 34.86f, 4.5f, -14.1f, 29.92f, -23.16f, 29.44f, -21.72f,
  23.58f, -13.13f, 24.21f, 0.0f, 7.5f, -17.94f, 3.5f, 26.67f, 4.5f, -21.26f, 7.45f, -16.64f, 6.81f, -16.22f, 9.79f, -20.83f, 10.43f, 0.0f, 4.26f,
  -18.73f, 2.12f, 8.62f, 1.5f, -25.96f, 8.81f, -25.84f, 10.81f, -28.21f, 10.97f, -28.33f, 8.97f, 1.93f, 5.9f, -26.09f, 3.93f, 9.89f, 2.0f, -33.09f,
  -11.92f, -23.99f, -10.08f, -25.55f, -0.33f, -35.09f, -2.31f, 1.8f, 8.49f, -29.43f, 4.73f, -6.16f, 4.7f, -19.71f, -2.82f, -15.43f, -8.92f, -13.51f,
  -8.48f, -15.02f, -1.77f, 0.57f, 4.86f, -15.94f, 2.57f, -3.23f, 2.5f, -12.19f, -0.23f, -9.49f, 0.5f, -10.42f, 3.13f, -12.52f, 2.42f, -1.0f, 2.95f,
  -11.11f, 1.07f, 1.16f, 1.5f, -5.74f, 5.17f, -3.55f, 6.11f, -6.66f, 12.87f, -8.86f, 11.92f, -1.0f, 3.22f, -6.2f, 1.19f, 9.02f, 1.3f, 0.01f, 24.91f,
  -5.85f, 24.91f, -5.85f, 23.14f, 0.01f, 23.14f, -1.0f, 2.99f, -2.91f, 1.0f
};
static const uint32_t data_3885C = 0x41C00419U;
static const float data_38860[128] = {
  2.5f, 1.22f, 22.82f, 3.06f, 22.82f, 3.06f, 24.38f, 1.22f, 24.38f, -1.0f, 3.1f, 2.15f, 1.0f, 23.6f, 1.0f, 5.17f, 27.96f, 5.17f, 22.75f, 9.74f,
  22.75f, 9.74f, 27.96f, -1.0f, 6.51f, 7.46f, 1.44f, 26.52f, 1.5f, 1.4f, 32.35f, 8.63f, 32.34f, 8.63f, 34.07f, 1.4f, 34.0f, -1.0f, 3.86f, 6.02f, 1.0f,
  33.21f, 2.5f, 8.64f, 40.58f, 13.25f, 35.48f, 16.11f, 38.07f, 11.5f, 43.17f, -1.0f, 3.3f, 12.38f, 1.92f, 39.33f, 3.5f, 22.06f, 36.09f, 20.86f,
  34.48f, 22.48f, 33.26f, 23.68f, 34.87f, -1.0f, 3.02f, 22.27f, 1.0f, 34.67f, 1.0f, 13.63f, 23.94f, 13.63f, 17.5f, 17.6f, 17.58f, 17.6f, 23.94f,
  -1.0f, 3.95f, 15.61f, 1.0f, 20.75f, 1.97f, 18.4f, 7.02f, 22.18f, 7.02f, 22.18f, 9.74f, 18.4f, 9.74f, -1.0f, 3.3f, 20.29f, 1.35f, 8.38f, 1.35f,
  23.86f, 11.68f, 24.53f, 7.0f, 29.56f, 7.7f, 28.89f, 12.39f, -1.0f, 3.3f, 26.71f, 2.35f, 9.69f, 2.5f, 6.93f, 16.18f, 9.35f, 16.18f, 9.35f, 18.02f,
  6.93f, 18.02f, -1.0f, 3.01f, 8.14f, 1.0f, 17.1f, 1.0f, 9.9f
};
static const shen_name data_38A60 = SHEN_NAME('33KA');
static const float data_38A64 = 4.64f;
static const shen_name data_38A68 = SHEN_NAME('33KA');
static const float data_38A6C[38] = {
  4.64f, 7.09f, 9.9f, 7.09f, -1.0f, 5.86f, 7.27f, 1.0f, 9.9f, 2.62f, 4.87f, 1.89f, 5.03f, 0.1f, 7.43f, 0.32f, 7.26f, 2.11f, -1.21f, 3.05f, 6.15f,
  1.0f, 1.1f, 1.0f, 5.27f, -13.42f, 5.27f, -16.92f, 9.88f, -17.0f, 9.88f, -13.42f, -1.0f, 3.3f, 7.58f, 1.75f, -15.17f, 1.75f
};
