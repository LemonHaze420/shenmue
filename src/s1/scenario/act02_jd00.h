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
static int32_t script_FF06(void);
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
static void script_13DE7(int32_t arg0);
static int32_t script_13EF4(int32_t arg0);
static int32_t script_13F4F(void);
static int32_t script_14091(void);
static void script_14137(void);
static int32_t script_14190(void);
static int32_t script_141CA(int32_t arg0);
static void script_141ED(void);
static int32_t script_1420D(int32_t arg0);
static void script_14258(void);
static void script_1429F(void);
static void script_142F2(void);
static void script_1434E(void);
static void script_14395(void);
static void script_143F1(void);
static void script_144BF(void);
static void script_14551(void);
static void script_1458E(void);
static int32_t script_145CB(int32_t arg0);
static void script_1463C(int32_t arg0);
static int32_t script_14857(void);
static void script_1489F(void);
static void script_148A7(void);
static void script_148BB(void);
static void script_14BBD(int32_t arg0, int32_t arg1);
static void script_14C1C(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static int32_t script_14C43(float arg0, float arg1, float arg2, float arg3);
static int32_t script_14DE1(void);
static void script_14EA8(const char *arg0, int32_t arg1);
static void script_14EF5(void);
static int32_t script_1502C(int32_t arg0);
static void script_150A5(void);
static int8_t script_15107(void);
static void script_15152(int32_t arg0);
static void script_151BB(int32_t arg0);
static void script_151E6(void);
static int32_t script_1520D(int32_t arg0);
static int8_t script_1524F(int32_t arg0);
static void script_15327(void);
static void script_15334(void);
static void script_1579E(void);
static void script_16303(void);
static void script_163C1(void);
static void script_163E6(void);
static void script_16B2D(void);
static int8_t script_18BB4(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int8_t script_18C6D(void);
static void script_18E23(int32_t arg0);
static int8_t script_19F94(void);
static void script_1A529(int32_t arg0);
static void script_1E3E7(void);
static int8_t script_1EEAF(void);
static int8_t script_1EED7(void);
static void script_1EFF2(int32_t arg0);
static int32_t script_1F0DB(int32_t arg0, int32_t arg1);
static int32_t script_1F144(void);
static void script_1F15F(int32_t arg0);
static void script_1F248(int32_t arg0);
static void script_1F436(void);
static void script_1F46D(int32_t arg0);
static int8_t script_1F535(void);
static void script_20212(void);
static void script_202D5(void);
static void script_205FD(void);
static void script_20682(void);
static void script_20711(void);
static int32_t script_20927(void);
static int32_t script_209DC(void);
static void script_2135E(void);
static void script_2145D(void);
static void script_214D0(void);
static void script_216B9(void);
static void script_216CD(void);
static void script_21795(void);
static int32_t script_218DA(void);
static void script_2195D(void);
static void script_219B3(void);
static int32_t script_21B77(void);
static int8_t script_21DD1(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_21E8A(void);
static int32_t script_21EC6(void);
static void script_2215B(void);
static void script_2217A(void);
static void script_22188(void);
static void script_2218B(int32_t arg0);
static void script_221AE(int32_t arg0);
static void script_221D1(int32_t arg0);
static void script_221F4(void);
static int32_t script_2225B(void);
static void script_2232F(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_223C6(void);
static int32_t script_223EE(void);
static int8_t script_22408(void);
static int32_t script_224F8(void);
static int32_t script_225CE(void);
static int32_t script_226E3(void);
static void script_22806(void);
static int32_t script_228FC(void);
static void script_22A43(int32_t arg0);
static int32_t script_22AC1(int32_t arg0, int32_t arg1);
static int32_t script_22AD3(int32_t arg0, int32_t arg1);
static void script_22B8A(void);
static int32_t script_22BB0(void);
static void script_22DE1(int32_t arg0);
static void script_22E1C(int32_t arg0);
static void script_22E46(int32_t arg0);
static void script_22F23(int32_t arg0);
static void script_2308C(void);
static void script_23197(void);
static void script_231E5(int32_t arg0);
static void script_23220(int32_t arg0);
static int32_t script_2324A(void);
static void script_23286(int32_t arg0);
static void script_235C1(void);
static void script_2371F(int32_t arg0);
static void script_23764(int32_t arg0);
static void script_23814(void);
static void script_238B8(void);
static void script_238DF(int32_t arg0);
static int32_t script_2394A(void);
static void script_23F47(int32_t arg0);
static void script_23FAE(void);
static void script_2439B(void);
static int32_t script_24E45(void);
static void script_24F4A(int32_t arg0);
static void script_24FC4(void);
static void script_2500F(void);
static int32_t script_25036(void);
static int32_t script_2521B(void);
static int32_t script_25563(void);
void script_255CA(void);
static int32_t script_25909(void);
static void script_25933(void);
static void script_25C91(void);
static void script_25CC9(void);
static void script_25E01(int32_t arg0);
static void script_25E0D(int32_t arg0);
static void script_25E19(int32_t arg0);
static void script_25F25(int32_t arg0);
static void script_25F3C(int32_t arg0);
static void script_25F7D(int32_t arg0);
static void script_25FC1(int32_t arg0);
static void script_2603F(void);
static void script_260BE(void);
static void script_26129(int32_t arg0, int32_t arg1);
static void script_26190(int32_t arg0, int32_t arg1);
static void script_261BC(void);
static void script_261F7(void);
static void script_2620A(void);
static void script_26216(void);
static void script_263AF(void);
static void script_26418(void);

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
static int32_t task_3B0;
static int32_t task_3B4;
static const float data_26450[3] = {1.0f, 1.0f, 1.0f};
static const int32_t data_2645C[4] = {16777215, 16777215, 16777215, 16777215};
static const float data_2646C[3] = {20.0f, 800.0f, 1.0f};
static const int32_t data_26478 = -1;
static const char data_2647C[10] __attribute__((scn_string_literal)) = "Kaidan : ";
static const char data_26486[32] __attribute__((scn_string_literal)) = "========== initializing kaidan\012";
static const char data_264A6[48] __attribute__((scn_string_literal)) = "please ENDOFKDATA use can't start shuki kaidan\012";
static const char data_264D6[] __attribute__((scn_string_literal)) = "Kaidan Function Error 1\n";
static const char data_264EF[] __attribute__((scn_string_literal)) = "Kaidan Function Error 2\n";
static const char data_26508[] __attribute__((scn_string_literal)) = "Kaidan Data Error\n";
static const char data_2651B[10] __attribute__((scn_string_literal)) = "Kaidan : ";
static const char data_26525[28] __attribute__((scn_string_literal)) = "wait ... shuki shuki shuki\012";
static const char data_26541[10] __attribute__((scn_string_literal)) = "Kaidan : ";
static const char data_2654B[24] __attribute__((scn_string_literal)) = "KAIDAN : Warning coli!\012";
static const char data_26563[24] __attribute__((scn_string_literal)) = "KAIDAN : Warning coli!\012";
static const char data_2657B[24] __attribute__((scn_string_literal)) = "KAIDAN : Warning coli!\012";
static const char data_26593[24] __attribute__((scn_string_literal)) = "KAIDAN : Warning coli!\012";
static const char data_265AB[24] __attribute__((scn_string_literal)) = "KAIDAN : Warning coli!\012";
static const char data_265C3[24] __attribute__((scn_string_literal)) = "KAIDAN : Warning setY!\012";
static const char data_265DB[24] __attribute__((scn_string_literal)) = "KAIDAN : Warning setY!\012";
static const char data_265F3[1609] = "Kaidan : \000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000V\325\377\377\000\000\000\000\000\000\000\000\302\326\377\377\000\000\000\000\260\005\000\000\000\000\000\000\000\000\000\000\000\000\000\000\232\271\377\377\000\000\000\000\000\000\000\000(\275\377\377\000\000\000\000\000\000\000\000.\270\377\377\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\\\320\377\377\000\000\000\000\000\000\000\000\344\270\377\377\000\000\000\000\000\000\000\000\224\276\377\377\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\310\321\377\377\000\000\000\000\000\000\000\000.\270\377\377\000\000\000\000\000\000\000\000\373\304\377\377\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\216\003\000\000\022\321\377\377\000\000\000\000x\367\377\377\224\276\377\377\000\000\000\000x\367\377\377\344\330\377\377\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\224\376\377\377~\322\377\377\240\364\377\377\224\376\377\377\357\356\377\377f\006\000\000\216\003\000\000\266\000\000\000\000\000\000\000\000\000\000\000m\301\377\377\000\000\000\000\266\000\000\000\261\245\377\377\000\000\000\000\"\002\000\000\014\266\377\377\000\000\000\000\000\000\000\000\274\373\377\377\000\000\000\000\000\000\000\000\000\300\377\377\000\000\000\000\000\000\000\000\253\252\377\377\000\000\000\000\000\000\000\000\302\266\377\377\000\000\000\000\000\000\000\000\336\375\377\377\000\000\000\000\000\000\000\000\027\254\377\377\000\000\000\000\000\000\000\000\315\254\377\377\000\000\000\000\000\000\000\000x\267\377\377\000\000\000\000\000\000\000\000(\375\377\377\000\000\000\000\216\003\000\000\302\266\377\377\000\000\000\000\266\000\000\000\302\266\377\377\000\000\000\000\\\360\377\377\344\270\377\377\000\000\000\000\000\000\000\000(\375\377\377\000\000\000\000\224\376\377\377?\311\377\377\240\364\377\377\224\376\377\377\261\345\377\377f\006\000\000\216\003\000\000\364\011\000\000\000\000\000\000\000\000\000\000x\267\377\377\000\000\000\000\266\000\000\000\274\233\377\377\000\000\000\000\"\002\000\000\027\254\377\377\000\000\000\000\000\000\000\000\310\361\377\377\000\000\000\000\000\000\000\000\014\266\377\377\000\000\000\000\000\000\000\000\267\240\377\377\000\000\000\000\000\000\000\000\315\254\377\377\000\000\000\000\000\000\000\000\352\363\377\377\000\000\000\000\000\000\000\000#\242\377\377\000\000\000\000\000\000\000\000\331\242\377\377\000\000\000\000\000\000\000\000\203\255\377\377\000\000\000\000\000\000\000\0004\363\377\377\000\000\000\000\216\003\000\000\315\254\377\377\000\000\000\000\266\000\000\000\315\254\377\377\000\000\000\000\\\360\377\377\357\256\377\377\000\000\000\000\000\000\000\0004\363\377\377\000\000\000\000\000\000\000\000\253\352\377\377\000\000\000\000\302\366\377\377\274\373\377\377\000\000\000\000J\377\377\377D\004\000\000\000\000\000\000\336\375\377\377\261\345\377\377\000\000\000\000\000\000\000\000\\\360\377\377\000\000\000\000\000\000\000\000\240\364\377\377\000\000\000\000J\377\377\377\372\004\000\000\000\000\000\000\000\000\000\000\027\314\377\377\000\000\000\000\000\000\000\000a\353\377\377\000\000\000\000\000\000\000\000\267\340\377\377\000\000\000\000\000\000\000\000\260\005\000\000\000\000\000\000\266\000\000\000\336\335\377\377\000\000\000\000\000\000\000\000\211\350\377\377\000\000\000\000\000\000\000\000\336\335\377\377\000\000\000\000\266\000\000\000f\006\000\000\000\000\000\000(\375\377\377a\353\377\377\000\000\000\000\006\373\377\377\261\345\377\377\000\000\000\000r\374\377\377\336\335\377\377\000\000\000\000\000\000\000\000f\006\000\000\000\000\000\000\000\000\000\000\253\352\377\377\000\000\000\000\302\366\377\377\274\373\377\377\000\000\000\000J\377\377\377D\004\000\000\000\000\000\000\336\375\377\377\027\314\377\377\000\000\000\000\000\000\000\000\365\351\377\377\000\000\000\000\000\000\000\000\267\340\377\377\000\000\000\000J\377\377\377\372\004\000\000\000\000\000\000\000\000\000\000\310\321\377\377\000\000\000\000\000\000\000\000a\353\377\377\000\000\000\000\000\000\000\000\267\340\377\377\000\000\000\000\000\000\000\000\260\005\000\000\000\000\000\000\266\000\000\000\336\335\377\377\000\000\000\000\000\000\000\000\211\350\377\377\000\000\000\000\000\000\000\000\336\335\377\377\000\000\000\000\266\000\000\000f\006\000\000\000\000\000\000(\375\377\377a\353\377\377\000\000\000\000\006\373\377\377\261\345\377\377\000\000\000\000r\374\377\377\336\335\377\377\000\000\000\000\000\000\000\000D\004\000\000\000\000\000\000\000\000\000\000\253\352\377\377\000\000\000\000\302\366\377\377\274\373\377\377\000\000\000\000J\377\377\377D\004\000\000\000\000\000\000\336\375\377\377\310\361\377\377\000\000\000\000\000\000\000\000\302\366\377\377\000\000\000\000\000\000\000\000P\372\377\377\000\000\000\000J\377\377\377\372\004\000\000\000\000\000\000\000\000\000\000\022\361\377\377\000\000\000\000\000\000\000\000V\365\377\377\000\000\000\000\000\000\000\000P\372\377\377\000\000\000\000\000\000\000\000\260\005\000\000\000\000\000\000\266\000\000\000\022\361\377\377\000\000\000\000\000\000\000\000\240\364\377\377\000\000\000\000\000\000\000\000\232\371\377\377\000\000\000\000\266\000\000\000f\006\000\000\000\000\000\000(\375\377\377\310\361\377\377\000\000\000\000r\374\377\377V\365\377\377\000\000\000\000\336\375\377\377\232\371\377\377\000\000\000\000\000\000\000\000D\004";
static const int32_t data_26C3C[342] = {
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
static const char data_27194[5] __attribute__((scn_string_literal)) = "SY11";
static const char data_27199[9] __attribute__((scn_string_literal)) = "SY11A006";
static const char data_271A2[9] __attribute__((scn_string_literal)) = "SY11A007";
static const char data_271AB[9] __attribute__((scn_string_literal)) = "SY11A008";
static const char data_271B4[9] __attribute__((scn_string_literal)) = "SY11A002";
static const char data_271BD[9] __attribute__((scn_string_literal)) = "SY11A003";
static const char data_271C6[9] __attribute__((scn_string_literal)) = "SY11A004";
static const char data_271CF[9] __attribute__((scn_string_literal)) = "SY11A005";
static const char data_271D8[9] __attribute__((scn_string_literal)) = "SY11A001";
static const char data_271E1[9] __attribute__((scn_string_literal)) = "SY11A009";
static const char data_271EA[9] __attribute__((scn_string_literal)) = "SY11A010";
static const char data_271F3[9] __attribute__((scn_string_literal)) = "SY11A011";
static const char data_271FC[9] __attribute__((scn_string_literal)) = "SY11A009";
static const char data_27205[5] __attribute__((scn_string_literal)) = "SY11";
static const char data_2720A[5] __attribute__((scn_string_literal)) = "SY11";
static const char data_2720F[9] __attribute__((scn_string_literal)) = "SY11A001";
static const char data_27218[5] __attribute__((scn_string_literal)) = "SY11";
static const char data_2721D[5] __attribute__((scn_string_literal)) = "SY11";
static const char data_27222[9] __attribute__((scn_string_literal)) = "SY11A009";
static const char data_2722B[9] __attribute__((scn_string_literal)) = "SY11A010";
static const char data_27234[9] __attribute__((scn_string_literal)) = "SY11A011";
static const char data_2723D[7] __attribute__((scn_string_literal)) = "SY11";
static const uint32_t data_27244 = SHEN_NAME('VM_0');
static const shen_name data_27248[7] = {
  SHEN_NAME('VM_1'), SHEN_NAME('VM_2'), SHEN_NAME('VM_3'), SHEN_NAME('VM_4'), SHEN_NAME('VM_5'), SHEN_NAME('VM_6'), SHEN_NAME('VM_7')
};
static const char data_27264[18] __attribute__((scn_string_literal)) = "/scene/02/stream/";
static const char data_27276[5] __attribute__((scn_string_literal)) = "%03d";
static const char data_2727B[] __attribute__((scn_string_literal)) = "ATRK";
static Vector3 data_27280 = {.x = 0.0f, .y = -0.1f, .z = 0.0f};
static Vector3 data_2728C = {.x = 0.0f, .y = 0.0f, .z = 0.0f};
static const char data_27298[] __attribute__((scn_string_literal)) = "VEND";
static const char data_2729D[] __attribute__((scn_string_literal)) = "VEND";
static const char data_272A2[] __attribute__((scn_string_literal)) = "SPRT";
static const char data_272A7[7] __attribute__((scn_string_literal)) = "01JUCE";
static const char data_272AE[13] __attribute__((scn_string_literal)) = "HMOT_CAN.BIN";
static const char data_272BB[] __attribute__((scn_string_literal)) = "VEND";
static const char data_272C0[7] __attribute__((scn_string_literal)) = "01JUCE";
static const char data_272C7[22] __attribute__((scn_string_literal)) = "HandMot read by VEND\012";
static const char data_272DD[13] __attribute__((scn_string_literal)) = "HMOT_CAN.BIN";
static const char data_272EA[7] __attribute__((scn_string_literal)) = "01JUCE";
static const char data_272F1[] __attribute__((scn_string_literal)) = "VEND";
static const uint8_t data_272F6[2] = {0};
static const uint8_t data_272F8[12] = {0};
static const char data_27304[29] __attribute__((scn_string_literal)) = "vendmachine exit is EV_STOP\012";
static const char data_27321[41] __attribute__((scn_string_literal)) = "vendmachine exit is MONEY_LOCK -> %d,%d\012";
static const char data_2734A[] __attribute__((scn_string_literal)) = "COKE";
static const char data_2734F[] __attribute__((scn_string_literal)) = "FATG";
static const char data_27354[] __attribute__((scn_string_literal)) = "FATO";
static const char data_27359[] __attribute__((scn_string_literal)) = "SPRT";
static const char data_2735E[] __attribute__((scn_string_literal)) = "CAFE";
static const uint8_t data_27363[1] = {0};
static const uint32_t data_27364 = 0x3D204C06;
static const float data_27368[2] = {0.0f, 0.890018f};
static const uint8_t data_27370[12] = {0};
static const uint8_t data_2737C[12] = {0};
static const uint32_t data_27388 = 0x3DE978D5;
static const float data_2738C[2] = {0.002f, -0.036f};
static const uint32_t data_27394 = 8192;
static const int32_t data_27398[2] = {11377, 3640};
static const uint32_t data_273A0 = 0x3D204C06;
static const float data_273A4[2] = {0.0f, 0.890018f};
static const uint8_t data_273AC[12] = {0};
static const uint32_t data_273B8 = 4369;
static const int32_t data_273BC[2] = {0, -2184};
static const char data_273C4[] __attribute__((scn_string_literal)) = "01JUCEA011";
static const char data_273CF[] __attribute__((scn_string_literal)) = "01JUCEA012";
static const char data_273DA[] __attribute__((scn_string_literal)) = "01JUCEA013";
static const char data_273E5[] __attribute__((scn_string_literal)) = "01JUCEA002";
static const char data_273F0[] __attribute__((scn_string_literal)) = "01JUCEA003";
static const uint8_t data_273FB[1] = {0};
static const uint32_t data_273FC = 0x3DE978D5;
static const float data_27400[2] = {0.002f, -0.036f};
static const uint32_t data_27408 = 8192;
static const int32_t data_2740C[2] = {11377, 3640};
static const uint32_t data_27414 = 0x3DCB04AB;
static const float data_27418[2] = {0.02044f, -0.04322f};
static const uint32_t data_27420 = 0xFFFFFE69;
static const int32_t data_27424[2] = {3331, -3464};
static const uint32_t data_2742C = 0x3D204C06;
static const float data_27430[2] = {0.0f, 0.890018f};
static const uint8_t data_27438[12] = {0};
static const uint32_t data_27444 = 4369;
static const int32_t data_27448[2] = {0, -2184};
static const uint8_t data_27450[12] = {0};
static const char data_2745C[] __attribute__((scn_string_literal)) = "01JUCEA002";
static const char data_27467[] __attribute__((scn_string_literal)) = "01JUCEA003";
static const uint8_t pad_27472[4] = {0};
static const uint8_t data_27476[10] = {
  0x80, 0x3F, 0x00, 0x00, 0x80, 0x3F, 0x00, 0x00, 0x80, 0x3F
};
static DoorMotionSelectorBlock data_27480[38] = {
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
static ScnDoorMotionDescriptor data_27A70[72] = {
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
static uint32_t data_28490 = 0xFFFFFFFF;
typedef struct Row_28494 { float w0; float w1; float w2; float w3; int32_t w4; } Row_28494;
static const Row_28494 data_28494[7] = {
  {0.0f, 0.0f, 0.0f, 0.0f, -1},
  {0.0f, 0.0f, 0.0f, 0.0f, -1},
  {0.0f, 0.0f, 0.0f, 0.0f, 1534},
  {26.0f, 55.0f, 45.0f, 75.0f, 1555},
  {61.0f, 146.0f, 45.0f, 50.0f, 1554},
  {59.0f, 124.0f, 45.0f, 70.0f, 1543},
  {33.0f, 49.0f, 45.0f, 50.0f, 1543}
};
static const float data_28520[4] = {33.0f, 49.0f, 50.0f, 50.0f};
static void (*data_28530)(int32_t, int32_t, int32_t) = script_6507;
static void (*const data_28534)(int32_t, int32_t, int32_t, int32_t) = script_6517;
static void (*const data_28538)(int32_t, int32_t, int32_t, int32_t) = script_6517;
static void (*const data_2853C)(int32_t, int32_t, int32_t, int32_t) = script_6517;
static void (*const data_28540)(int32_t, int32_t, int32_t, int32_t) = script_6517;
static void (*const data_28544)(int32_t, int32_t, int32_t, int32_t) = script_6517;
static void (*const data_28548)(int32_t, int32_t, int32_t, int32_t) = script_6517;
static void (*const data_2854C)(int32_t, int32_t, int32_t, int32_t) = script_6517;
static void (*const data_28550)(int32_t, int32_t, int32_t, int32_t) = script_6517;
static void (*const data_28554)(int32_t, int32_t, int32_t, int32_t) = script_67BD;
static void (*const data_28558)(int32_t, int32_t, int32_t, int32_t) = script_691E;
static void (*const data_2855C)(int32_t, int32_t, int32_t, int32_t) = script_6DEB;
static void (*const data_28560)(int32_t, int32_t, int32_t, int32_t) = script_6E52;
static void (*const data_28564)(int32_t, int32_t, int32_t, int32_t) = script_661B;
static void (*const data_28568)(int32_t, int32_t, int32_t, int32_t) = script_661B;
static void (*const data_2856C)(int32_t, int32_t, int32_t, int32_t) = script_6A7F;
static void (*const data_28570)(int32_t, int32_t, int32_t, int32_t) = script_6D1D;
static void (*const data_28574)(int32_t, int32_t, int32_t, int32_t) = script_6EB9;
static void (*const data_28578)(int32_t, int32_t, int32_t, int32_t) = script_67BD;
static void (*const data_2857C)(int32_t, int32_t, int32_t, int32_t) = script_691E;
static void (*const data_28580)(int32_t, int32_t, int32_t, int32_t) = script_6A7F;
static void (*const data_28584)(int32_t, int32_t, int32_t, int32_t) = script_67BD;
static void (*const data_28588)(int32_t, int32_t, int32_t, int32_t) = script_691E;
static void (*const data_2858C)(int32_t, int32_t, int32_t, int32_t) = script_6A7F;
static void (*const data_28590)(int32_t, int32_t, int32_t, int32_t) = script_6F00;
static void (*const data_28594)(int32_t, int32_t, int32_t, int32_t) = script_6F00;
static void (*const data_28598)(int32_t, int32_t, int32_t, int32_t) = script_6F00;
static void (*const data_2859C)(int32_t, int32_t, int32_t, int32_t) = script_6F00;
static void (*const data_285A0)(int32_t, int32_t, int32_t, int32_t) = script_7036;
static void (*const data_285A4)(int32_t, int32_t, int32_t, int32_t) = script_78F7;
static void (*const data_285A8)(int32_t, int32_t, int32_t, int32_t) = script_6517;
static void (*const data_285AC)(int32_t, int32_t, int32_t, int32_t) = script_6517;
static void (*const data_285B0)(int32_t, int32_t, int32_t, int32_t) = script_6517;
static void (*const data_285B4)(int32_t, int32_t, int32_t, int32_t) = script_6517;
static void (*const data_285B8)(int32_t, int32_t, int32_t, int32_t) = script_6517;
static void (*const data_285BC)(int32_t, int32_t, int32_t, int32_t) = script_6517;
static void (*const data_285C0)(int32_t, int32_t, int32_t, int32_t) = script_6517;
static void (*const data_285C4)(int32_t, int32_t, int32_t, int32_t) = script_6517;
static const char data_285C8[46] __attribute__((scn_string_literal)) = "Door Index = %d : call_type = %d : type = %d\012";
static const char data_285F6[43] __attribute__((scn_string_literal)) = "Door Open Motion = DEB_AKESIME_FUSUMA_L_F\012";
static const char data_28621[43] __attribute__((scn_string_literal)) = "Door Open Motion = DEB_AKESIME_FUSUMA_R_F\012";
static const char data_2864C[42] __attribute__((scn_string_literal)) = "Door Open Motion = DEB_HIKISIME_DOOR_L_F\012";
static const char data_28676[42] __attribute__((scn_string_literal)) = "Door Open Motion = DEB_HIKISIME_DOOR_R_F\012";
static const char data_286A0[41] __attribute__((scn_string_literal)) = "Door Open Motion = DEB_OSISIME_DOOR_L_F\012";
static const char data_286C9[41] __attribute__((scn_string_literal)) = "Door Open Motion = DEB_OSISIME_DOOR_R_F\012";
static const char data_286F2[43] __attribute__((scn_string_literal)) = "Door Open Motion = GAK_AKESIME_FUSUMA_L_F\012";
static const char data_2871D[43] __attribute__((scn_string_literal)) = "Door Open Motion = GAK_AKESIME_FUSUMA_R_F\012";
static const char data_28748[42] __attribute__((scn_string_literal)) = "Door Open Motion = GAK_HIKISIME_DOOR_L_F\012";
static const char data_28772[42] __attribute__((scn_string_literal)) = "Door Open Motion = GAK_HIKISIME_DOOR_R_F\012";
static const char data_2879C[41] __attribute__((scn_string_literal)) = "Door Open Motion = GAK_OSISIME_DOOR_L_F\012";
static const char data_287C5[41] __attribute__((scn_string_literal)) = "Door Open Motion = GAK_OSISIME_DOOR_R_F\012";
static const char data_287EE[43] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_AKESIME_FUSUMA_L_F\012";
static const char data_28819[43] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_AKESIME_FUSUMA_R_F\012";
static const char data_28844[42] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_HIKISIME_DOOR_L_F\012";
static const char data_2886E[42] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_HIKISIME_DOOR_R_F\012";
static const char data_28898[41] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_OSISIME_DOOR_L_F\012";
static const char data_288C1[41] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_OSISIME_DOOR_R_F\012";
static const char data_288EA[43] __attribute__((scn_string_literal)) = "Door Open Motion = KOD_AKESIME_FUSUMA_L_F\012";
static const char data_28915[43] __attribute__((scn_string_literal)) = "Door Open Motion = KOD_AKESIME_FUSUMA_R_F\012";
static const char data_28940[42] __attribute__((scn_string_literal)) = "Door Open Motion = KOD_HIKISIME_DOOR_L_F\012";
static const char data_2896A[42] __attribute__((scn_string_literal)) = "Door Open Motion = KOD_HIKISIME_DOOR_R_F\012";
static const char data_28994[41] __attribute__((scn_string_literal)) = "Door Open Motion = KOD_OSISIME_DOOR_L_F\012";
static const char data_289BD[41] __attribute__((scn_string_literal)) = "Door Open Motion = KOD_OSISIME_DOOR_R_F\012";
static const char data_289E6[43] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_AKESIME_FUSUMA_L_F\012";
static const char data_28A11[43] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_AKESIME_FUSUMA_R_F\012";
static const char data_28A3C[42] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_HIKISIME_DOOR_L_F\012";
static const char data_28A66[42] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_HIKISIME_DOOR_R_F\012";
static const char data_28A90[41] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_OSISIME_DOOR_L_F\012";
static const char data_28AB9[41] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_OSISIME_DOOR_R_F\012";
static const char data_28AE2[43] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_AKESIME_FUSUMA_L_F\012";
static const char data_28B0D[43] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_AKESIME_FUSUMA_R_F\012";
static const char data_28B38[42] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_HIKISIME_DOOR_L_F\012";
static const char data_28B62[42] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_HIKISIME_DOOR_R_F\012";
static const char data_28B8C[41] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_OSISIME_DOOR_L_F\012";
static const char data_28BB5[41] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_OSISIME_DOOR_R_F\012";
static const char data_28BDE[43] __attribute__((scn_string_literal)) = "Door Open Motion = SIN_AKESIME_FUSUMA_L_F\012";
static const char data_28C09[43] __attribute__((scn_string_literal)) = "Door Open Motion = SIN_AKESIME_FUSUMA_R_F\012";
static const char data_28C34[42] __attribute__((scn_string_literal)) = "Door Open Motion = SIN_HIKISIME_DOOR_L_F\012";
static const char data_28C5E[42] __attribute__((scn_string_literal)) = "Door Open Motion = SIN_HIKISIME_DOOR_R_F\012";
static const char data_28C88[41] __attribute__((scn_string_literal)) = "Door Open Motion = SIN_OSISIME_DOOR_L_F\012";
static const char data_28CB1[41] __attribute__((scn_string_literal)) = "Door Open Motion = SIN_OSISIME_DOOR_R_F\012";
static const char data_28CDA[43] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_AKESIME_FUSUMA_L_F\012";
static const char data_28D05[43] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_AKESIME_FUSUMA_R_F\012";
static const char data_28D30[42] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_HIKISIME_DOOR_L_F\012";
static const char data_28D5A[42] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_HIKISIME_DOOR_R_F\012";
static const char data_28D84[41] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_OSISIME_DOOR_L_F\012";
static const char data_28DAD[41] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_OSISIME_DOOR_R_F\012";
static const char data_28DD6[43] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_AKESIME_FUSUMA_L_F\012";
static const char data_28E01[43] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_AKESIME_FUSUMA_R_F\012";
static const char data_28E2C[42] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_HIKISIME_DOOR_L_F\012";
static const char data_28E56[42] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_HIKISIME_DOOR_R_F\012";
static const char data_28E80[41] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_OSISIME_DOOR_L_F\012";
static const char data_28EA9[41] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_OSISIME_DOOR_R_F\012";
static const char data_28ED2[44] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_OSISIME_DOOR_AP_L_F\012";
static const char data_28EFE[44] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_OSISIME_DOOR_AP_L_F\012";
static const char data_28F2A[44] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_OSISIME_DOOR_AP_L_F\012";
static const char data_28F56[44] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_OSISIME_DOOR_AP_L_F\012";
static const char data_28F82[44] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_OSISIME_DOOR_AP_L_F\012";
static const char data_28FAE[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_28FCA[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_28FE6[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_29002[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_2901E[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_2903A[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_29056[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_29072[13] __attribute__((scn_string_literal)) = "open-move!!\012";
static const char data_2907F[11] __attribute__((scn_string_literal)) = "M_MDOR.BIN";
static const char data_2908A[] __attribute__((scn_string_literal)) = "This Charactor Work is NULL!!\n";
static const char data_290A9[22] __attribute__((scn_string_literal)) = "手動ドア開け番号指定\012";
static const char data_290BF[34] __attribute__((scn_string_literal)) = "無限ループになりそうなので抜ける\012";
static const char data_290E1[] __attribute__((scn_string_literal)) = "Don't make BOX Buffer!!\n";
static const char data_290FA[] __attribute__((scn_string_literal)) = "Don't make BOX Buffer!!\n";
static const char data_29113[15] __attribute__((scn_string_literal)) = "Door System : ";
static const char data_29122[15] __attribute__((scn_string_literal)) = "Door System : ";
static const char data_29131[18] __attribute__((scn_string_literal)) = "Door Box is ON!!\012";
static const char data_29143[19] __attribute__((scn_string_literal)) = "Door Box is OFF!!\012";
static const char data_29156[18] __attribute__((scn_string_literal)) = "Door Box is ON!!\012";
static const char data_29168[20] __attribute__((scn_string_literal)) = "Door Box is OFF!!\012";
static const DSWKDoorEntry data_2917C[28] = {
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
static const int32_t data_2972C[13] = {-1, 0, 6553700, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1};
static const uint8_t data_29760[4] = {0};
static const int32_t data_29764[82] = {
  -1, 45, 46, -1, 7, 8, -1, 53, 54, -1, 17, 18, -1, 39, 40, -1, 51, 52, -1, 37, 38, -1, 43, 44, -1, 1, 2, -1, 41, 42, -1, 35, 36, -1, 19, 20, -1, 3,
  4, -1, 33, 34, -1, 13, 14, -1, 11, 12, -1, 9, 10, -1, 23, 24, -1, 25, 26, -1, 29, 30, -1, 27, 28, -1, 21, 22, -1, 31, 32, -1, 5, 6, -1, 47, 48, -1,
  15, 16, -1, 49, 50, -1
};
static char data_298AC[] = "DR15_024";
static char data_298B5[] = "DR00_010";
static char data_298BE[] = "DR15_020";
static char data_298C7[] = "DR00_011";
static char data_298D0[] = "DR02_023";
static char data_298D9[] = "DR15_021";
static char data_298E2[] = "DR02_022";
static char data_298EB[] = "DR00_005";
static char data_298F4[] = "DR01_012";
static char data_298FD[] = "DR01_014";
static char data_29906[] = "DR15_018";
static char data_2990F[] = "DR15_019";
static char data_29918[] = "DR03_001";
static char data_29921[] = "DR01_013";
static char data_2992A[] = "DR01_013";
static const uint8_t data_29933[1] = {0};
static char *const data_29934 = data_298AC;
static const int32_t data_29938[3] = {15, 524971, 590507};
static char *const data_29944 = data_298B5;
static const int32_t data_29948[3] = {0, -1, -1};
static char *const data_29954 = data_298BE;
static const int32_t data_29958[3] = {15, 524971, 590507};
static char *const data_29964 = data_298C7;
static const int32_t data_29968[3] = {0, -1, -1};
static char *const data_29974 = data_298D0;
static const int32_t data_29978[3] = {2, 1311403, 1376939};
static char *const data_29984 = data_298D9;
static const int32_t data_29988[3] = {15, 524971, 590507};
static char *const data_29994 = data_298E2;
static const int32_t data_29998[3] = {2, 1311403, 1376939};
static char *const data_299A4 = data_298EB;
static const int32_t data_299A8[3] = {0, -1, -1};
static char *const data_299B4 = data_298F4;
static const int32_t data_299B8[3] = {1, 1311403, 1376939};
static char *const data_299C4 = data_298FD;
static const int32_t data_299C8[3] = {1, 1311403, 1376939};
static char *const data_299D4 = data_29906;
static const int32_t data_299D8[3] = {15, 524971, 590507};
static char *const data_299E4 = data_2990F;
static const int32_t data_299E8[3] = {15, 524971, 590507};
static char *const data_299F4 = data_29918;
static const int32_t data_299F8[3] = {3, 1311403, 1376939};
static char *const data_29A04 = data_29921;
static const int32_t data_29A08[3] = {1, 1311403, 1376939};
static char *const data_29A14 = data_2992A;
static const int32_t data_29A18[3] = {37, 1311403, 1376939};
static const MapObjectRecord data_29A24[55] = {
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
static const uint32_t data_2A1E0[4] = {0x413042CU, 0x42F042DU, 0x4300432U, 0x3870431U};
static const char data_2A1F0[20] __attribute__((scn_string_literal)) = "TUNED FreeTalk def\012";
static const char data_2A204[20] __attribute__((scn_string_literal)) = "TUNED FreeTalk def\012";
static const char data_2A218[11] __attribute__((scn_string_literal)) = "bgm109.snd";
static const char data_2A223[9] __attribute__((scn_string_literal)) = "SELECT";
static const Vector3 data_2A22C = {.x = 10.5122f, .y = 3.1989f, .z = -13.5867f};
static const Vector3 data_2A238 = {.x = 9.67f, .y = 3.1546f, .z = 6.2737f};
static const Vector3 data_2A244 = {.x = -5.8791f, .y = 3.1546f, .z = 21.8745f};
static const Vector3i data_2A250 = {.x = 65172, .y = 12599, .z = 65172};
static const Vector3i data_2A25C = {.x = 0, .y = 27285, .z = 0};
static const Vector3i data_2A268 = {.x = 0, .y = -26868, .z = 0};
static const int8_t data_2A274[16] = {12, 17, 22, 10, 19, 18, 11, 8, 4, 4, 17, 7, 6, 5, 4, 4};
static const int8_t data_2A284[16] = {12, 1, 2, 1, 3, 2, 2, 8, 4, 4, 17, 7, 6, 5, 4, 4};
static const uint32_t data_2A294 = 0xA800A7;
static const int32_t data_2A298[5] = {8847529, 8978568, 15073509, 15204583, 15335657};
static const uint32_t data_2A2AC = 0x440043;
static const int32_t data_2A2B0[8] = {4587589, 4718663, 4849737, 4980811, 5111885, 5242959, 5374033, 85};
static const uint32_t data_2A2D0 = 0x5C005B;
static const int32_t data_2A2D4[10] = {6160477, 6291551, 6422625, 6553699, 6684773, 6815847, 6946921, 7077995, 7209069, 7340143};
static const uint32_t data_2A2FC = 0x740073;
static const int32_t data_2A300[4] = {7798902, 7929976, 8061050, 8257660};
static const uint32_t data_2A310 = 0x720071;
static const int32_t data_2A314[9] = {10223750, 10354845, 10485919, 10616993, 10748067, 10879141, 11206826, 5767255, 180};
static const uint32_t data_2A338 = 0x810080;
static const int32_t data_2A33C[8] = {8585346, 11403437, 11534511, 11665585, 11862195, 11993270, 12452024, 8650879};
static const uint32_t data_2A35C = 0xB900BC;
static const int32_t data_2A360[5] = {12255418, 13238473, 13500621, 12517579, 200};
static const uint32_t data_2A374 = 0x5A0059;
static const int32_t data_2A378[3] = {5636268, 14221445, 5505107};
static const uint32_t data_2A384 = 0x7D0075;
static const int32_t data_2A388 = 12386442;
static const uint32_t data_2A38C = 0xC600C5;
static const int32_t data_2A390 = 13369543;
static const uint32_t data_2A394 = 0x8C008B;
static const int32_t data_2A398[8] = {9306253, 9437327, 9633937, 148, 9961623, 10092697, 9568411, 149};
static const uint32_t data_2A3B8 = 0xD700D6;
static const int32_t data_2A3BC[3] = {14287064, 14876891, 228};
static const uint32_t data_2A3C8 = 0xDC00D3;
static const int32_t data_2A3CC[2] = {13893845, 14549213};
static const uint32_t data_2A3D4 = 0xC100C0;
static const int32_t data_2A3D8[2] = {12779714, 196};
static const uint32_t data_2A3E0 = 0xD000CF;
static const int32_t data_2A3E4 = 13762769;
static const uint32_t data_2A3E8 = 0xE000DF;
static const int32_t data_2A3EC = 14811361;
static const char data_2A3F0[169] = "%GACR016G.MT5\000%GACK66BG.MT5\000%GACK66RG.MT5\000%GACR0BNG.MT5\000%GACR0B2G.MT5\000%GACR0B3G.MT5\000%SPBK6REG.MT5\000%SPBK6BRG.MT5\000%SPBK6YEG.MT5\000%SAIK6WHG.MT5\000%SAIK6BKG.MT5\000%SAIK6REG.MT5";
static const char data_2A499[315] = "%GACIAK1G.MT5\000%GACIAK2G.MT5\000%GACOJK1G.MT5\000%GACOJK2G.MT5\000%GACOSA1G.MT5\000%GACOSA2G.MT5\000%GACOLA1G.MT5\000%GACOLA2G.MT5\000%GACOPA1G.MT5\000%GACOPA2G.MT5\000%GACGWF1G.MT5\000%GACGWF2G.MT5\000%GACGJF1G.MT5\000%GACGJF2G.MT5\000%GACIKA1G.MT5\000%GACIKA2G.MT5\000%GACM401G.MT5\000%GACIAA1G.MT5\000%GACIAA2G.MT5\000D\000D\000D\000D\000D\000D\000D\000D\000D\000D\000%GACGJJ1G.MT5\000%GACGJJ2G.MT5";
static const char data_2A5D4[309] = "%GACIAKAG.MT5\000%GACIAKBG.MT5\000%GACGJKAG.MT5\000%GACGJKBG.MT5\000%GACOSAAG.MT5\000%GACOSABG.MT5\000%GACOLAAG.MT5\000%GACOLABG.MT5\000%GACOPAAG.MT5\000%GACOPABG.MT5\000%GACIWLAG.MT5\000%GACIWLBG.MT5\000%GACGJFAG.MT5\000%GACGJFBG.MT5\000%GACIKAAG.MT5\000%GACIKABG.MT5\000%GACGSNAG.MT5\000%GACGSNBG.MT5\000%GACILIAG.MT5\000%GACILIBG.MT5\000%GACIDUAG.MT5\000%GACIDUBG.MT5";
static const char data_2A709[167] = "%GACRSO1G.MT5\000%GACRSO2G.MT5\000%GACRTL1G.MT5\000%GACRNUCG.MT5\000%GACRBAKG.MT5\000%GACRBING.MT5\000%GACRFNGG.MT5\000%GACRAMYG.MT5\000%GACRESPG.MT5\000%GACREGGG.MT5\000D\000D\000D\000D\000D\000D\000%GACRFNNG.MT5";
static const char data_2A7B0[365] = "%GACRNT1G.MT5\000%GACRNT2G.MT5\000%GACRLOPG.MT5\000%GACRMYUG.MT5\000%GACRCKNG.MT5\000%GACRALFG.MT5\000%GACRRISG.MT5\000%GACRARXG.MT5\000%GACR0OPG.MT5\000%GACR0O2G.MT5\000%GACRPOPG.MT5\000%GACRCOBG.MT5\000%GACRIDAG.MT5\000%GACRSHPG.MT5\000%GACRMOBG.MT5\000%GACRROBG.MT5\000%GACK6TZG.MT5\000%GACRPANG.MT5\000%GACK6DYG.MT5\000D\000D\000D\000D\000D\000D\000D\000%GACRAXXG.MT5\000D\000D\000D\000D\000D\000D\000D\000%GACRRBBG.MT5\000%GACK6TSG.MT5\000%GACRPPNG.MT5\000%GACK6DSG.MT5";
static const char data_2A91D[505] = "%GACK6HHG.MT5\000%GACK6HYG.MT5\000%GACK6HBG.MT5\000%GACK6HGG.MT5\000%GACH5BKG.MT5\000%KAB02FSG.MT5\000%GACO1K2G.MT5\000%GACO1K3G.MT5\000%GACO1K7G.MT5\000%GACO1K8G.MT5\000%GACO1K9G.MT5\000%GACO1K1G.MT5\000%GACO1K4G.MT5\000%GACO1K5G.MT5\000%GACO1K6G.MT5\000%GACS5BSG.MT5\000%GACK6HOG.MT5\000%GACK6HKG.MT5\000%GACK6XHG.MT5\000%GACK6XYG.MT5\000%GACK6XBG.MT5\000%GACK6XGG.MT5\000%GACK6BKG.MT5\000%KABK6FSG.MT5\000%GACOKK2G.MT5\000%GACOKK3G.MT5\000%GACOKK7G.MT5\000%GACOKK8G.MT5\000%GACOKK9G.MT5\000%GACOKK1G.MT5\000%GACOKK4G.MT5\000%GACOKK5G.MT5\000%GACOKK6G.MT5\000%GACS5BBG.MT5\000%GACK6XOG.MT5\000%GACK6XKG.MT5";
static const char data_2AB16[211] = "%GACH5TKG.MT5\000%GACGTRXG.MT5\000%GACO1T1G.MT5\000%GACO1T2G.MT5\000%GACO1KOG.MT5\000%GACO1KBG.MT5\000%GACO1KJG.MT5\000%GACH5UKG.MT5\000%GACH5IKG.MT5\000%GACO1FOG.MT5\000%GACGSHEG.MT5\000%GACH5TTG.MT5\000%GACGTXXG.MT5\000%GACOTT1G.MT5\000%GACOTT2G.MT5";
static const char data_2ABE9[113] = "%YKW0201G.MT5\000%YWT0201G.MT5\000%GACRBONG.MT5\000%GACRRENG.MT5\000%GACK6SPG.MT5\000%GACG360G.MT5\000%GACRDR1G.MT5\000%GACRDR2G.MT5";
static const char data_2AC5A[77] = "%GACRSSCG.MT5\000%GACRMETG.MT5\000%GACRGBIG.MT5\000%GACH5TRG.MT5\000D\000D\000D\000%GACH5RRG.MT5";
static const char data_2ACA7[57] = "%GACS5FRG.MT5\000%GACS5FBG.MT5\000%GACO1KUG.MT5\000%GACO1FEG.MT5";
static const char data_2ACE0[239] = "%GACODG1G.MT5\000%GACODG2G.MT5\000%GACODG3G.MT5\000%GACOCT2G.MT5\000%GACOCATG.MT5\000%GACO1SHG.MT5\000%GACOKSHG.MT5\000%GACO1IRG.MT5\000%GACOKIRG.MT5\000%GACO1HAG.MT5\000%GACOHA2G.MT5\000%GACOHA3G.MT5\000%GACOHA4G.MT5\000%GACO1YGG.MT5\000%GACO1USG.MT5\000%GACOOSHG.MT5\000%GACOOIRG.MT5";
static const char data_2ADCF[99] = "%GACM400G.MT5\000%GACGSPHG.MT5\000%GACS5HGG.MT5\000%GACGDT1G.MT5\000%GACS5EQG.MT5\000%SYOGSH0G.MT5\000%SYOGHG0G.MT5";
static const char data_2AE32[85] = "%GACGCRYG.MT5\000%GACGBIRG.MT5\000%GACH5SLG.MT5\000%GACH5JKG.MT5\000%SYOGMD1G.MT5\000%SYOGMD5G.MT5";
static const char data_2AE87[71] = "%GACS5N1G.MT5\000%GACS5N2G.MT5\000%GACS5N3G.MT5\000%GACS5N4G.MT5\000%GACS5N5G.MT5";
static const char data_2AECE[57] = "%GACGXBNG.MT5\000%GACGYBNG.MT5\000%GACGABNG.MT5\000%GACGBBNG.MT5";
static const char data_2AF07[57] = "%GACRMEGG.MT5\000%GACR0GGG.MT5\000%GACRSATG.MT5\000%GACRCASG.MT5";
static const uint32_t data_2AF40 = SHEN_NAME('GCH0');
static const shen_name data_2AF44[5] = {SHEN_NAME('GCH1'), SHEN_NAME('GCH2'), SHEN_NAME('GCH3'), SHEN_NAME('GCH4'), SHEN_NAME('GCH5')};
static const uint32_t data_2AF58 = SHEN_NAME('HGC0');
static const shen_name data_2AF5C[5] = {SHEN_NAME('HGC1'), SHEN_NAME('HGC2'), SHEN_NAME('HGC3'), SHEN_NAME('HGC4'), SHEN_NAME('HGC5')};
static const uint32_t data_2AF70 = SHEN_NAME('GBX0');
static const shen_name data_2AF74[5] = {SHEN_NAME('GBX1'), SHEN_NAME('GBX2'), SHEN_NAME('GBX3'), SHEN_NAME('GBX4'), SHEN_NAME('GBX5')};
static const uint8_t data_2AF88[20] = {0};
typedef struct Row_2AF9C { float w0; int32_t w1; float w2; int32_t w3; int32_t w4; } Row_2AF9C;
static const Row_2AF9C data_2AF9C[5] = {
  {13.8f, 0, 22.65f, 49152, 3},
  {13.8f, 0, 22.0f, 49152, 257},
  {0.0f, 0, 0.0f, 0, 0},
  {0.0f, 0, 0.0f, 0, 0},
  {0.0f, 0, 0.0f, 0, 0}
};
static const char data_2B000[16] __attribute__((scn_string_literal)) = "/scene/02/JD00/";
static const char data_2B010[8] __attribute__((scn_string_literal)) = "GACH";
static const Vector3 data_2B018 = {.x = 0.29f, .y = 0.0f, .z = 0.088f};
static const Vector3i data_2B024 = {.x = 0, .y = 0, .z = 0};
static const char data_2B030[6] __attribute__((scn_string_literal)) = "E1066";
static const char data_2B036[13] __attribute__((scn_string_literal)) = "e1gachap.snd";
static const char data_2B043[16] __attribute__((scn_string_literal)) = "/scene/02/JD00/";
static const char data_2B053[8] __attribute__((scn_string_literal)) = "HI_GACH";
static const char data_2B05B[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_2B060[6] __attribute__((scn_string_literal)) = "E1066";
static const char data_2B066[8] __attribute__((scn_string_literal)) = "HI_GACH";
static const char data_2B06E[10] __attribute__((scn_string_literal)) = "HI_GACH";
static const Vector4 data_2B078 = {.x = 0.0f, .y = 0.7f, .z = 0.7f, .w = 0.7f};
static const uint8_t data_2B088[4] = {0};
static const int32_t data_2B08C[56] = {
  -546, -5870, 0, 2184, -3726, 0, 182, 0, 0, 0, -5184, 0, 71525, -12075, 0, -182, -5279, 0, -364, 910, 0, 1456, -2894, 0, 3458, -13352, 0, -910,
  -10327, 0, 0, 1820, 0, 0, -6863, 0, 1820, -16438, 0, -364, -10150, 0, 236, 291, 0, 0, -12105, 0, 0, -15539, 0, 0, -7249, 0, -819, -364
};
static const uint8_t data_2B16C[4] = {0};
static const int32_t data_2B170[56] = {
  -546, -5870, 0, 2184, -3726, 0, 182, 0, 0, 0, -5184, 0, 71525, -10922, 0, -182, -5279, 0, -364, 910, 0, 1456, -2894, 0, 3458, -13352, 0, -910,
  -10327, 0, 0, 1820, 0, 0, -6863, 0, 1820, -16438, 0, -364, -10150, 0, 236, 291, 0, 0, -12105, 0, 0, -15539, 0, 0, -7249, 0, -819, -364
};
static const uint8_t data_2B250[4] = {0};
static const int32_t data_2B254[56] = {
  2912, -7507, 0, 6189, -4454, 0, -1456, 0, 0, 546, -8266, 0, 1292, -7099, 0, 910, -11468, 0, 728, 3276, 0, 1092, -11450, 0, 546, -10256, 0, 1456,
  -6504, 0, 546, 1638, 0, -1092, -12142, 0, -364, -16438, 0, -364, -10150, 0, -309, 291, 0, 1638, -11923, 0, -3276, -17359, 0, -2912, -10707, 0, -819,
  -364
};
static const uint8_t data_2B334[4] = {0};
static const int32_t data_2B338[56] = {
  1638, -7507, 0, 6189, -4454, 0, -1456, 0, 0, 546, -17731, 0, -709, -9466, 0, 728, -15473, 0, 182, 2548, 0, 1092, -13271, 0, -1092, -14625, 0, 1638,
  -11965, 0, 0, 1638, 0, -1092, -12142, 0, -364, -16438, 0, -364, -10150, 0, -309, 291, 0, 1638, -11923, 0, -3276, -17359, 0, -2912, -10707, 0, -819,
  -364
};
static const uint8_t data_2B418[4] = {0};
static const int32_t data_2B41C[56] = {
  1638, -6779, 0, 6189, -4454, 0, -1456, 0, 0, 546, -17731, 0, -709, -9466, 0, 1274, -15473, 0, 546, 2548, 0, 1092, -13271, 0, -1092, -14625, 0, 1638,
  -11965, 0, -910, 1638, 0, -1092, -12142, 0, -364, -16438, 0, -364, -10150, 0, -309, 291, 0, 1638, -12105, 0, -3276, -17359, 0, -2912, -10889, 0,
  -819, -364
};
static const uint8_t data_2B4FC[4] = {0};
static const int32_t data_2B500[56] = {
  -2730, -9510, -3094, 4004, -1359, 0, 4004, -728, 0, 546, -6990, 0, 18, -4004, 2366, 1274, -11468, 0, 182, -182, 0, 1092, -13271, 0, -1092, -14443,
  0, 1638, -11965, 0, -910, 1638, 0, -1092, -12142, 0, -364, -16438, 0, -364, -10150, 0, -309, 291, 0, 1638, -12105, 0, -3276, -17359, 0, -2912,
  -10889, 0, -819, -364
};
static const uint8_t data_2B5E0[8] = {0};
static const int32_t data_2B5E8[55] = {
  -6005, 0, 4731, -6188, 0, 0, 0, 0, 0, -2727, 0, 0, -9097, 0, 0, -5457, 0, 0, 0, 0, 0, -8734, 0, -909, -7277, 0, 0, -7823, 0, 0, 0, 0, 1456, -4002,
  0, 0, -9462, 0, 726, -7824, 0, 0, 0, 0, 0, -12011, 0, -1091, -7461, 0, -1636, -9280, 0, 0, 0
};
static const uint8_t data_2B6C4[4] = {0};
static const int32_t data_2B6C8[56] = {
  -546, -5870, 0, 2184, -3726, 0, 182, 0, 0, 0, -5184, 0, 71525, -12075, 0, -182, -5279, 0, -364, 910, 0, 1456, -2894, 0, 3458, -13352, 0, -910,
  -10327, 0, 0, 1820, 0, 0, -6863, 0, 1820, -16438, 0, -364, -10150, 0, 236, 291, 0, 0, -12105, 0, 0, -15539, 0, 0, -7249, 0, -819, -364
};
static const uint8_t data_2B7A8[8] = {0};
static const int32_t data_2B7B0[55] = {
  -8046, 0, 0, -1820, -13088, 4350, -2894, 0, 0, -9011, 0, 0, -18422, 0, 0, -9812, 0, 0, 0, 0, 0, -7518, 0, 0, -17385, 0, 0, -11559, 0, 0, 0, 0, 0,
  -5734, 0, 0, -18058, 0, 0, -12233, 0, 0, 0, 0, 0, -7827, 0, 0, -16183, 0, 0, -13981, 0, -72, 0
};
static const Vector3 data_2B88C = {.x = 0.067f, .y = 0.061f, .z = 0.0379f};
static const uint32_t data_2B898 = 0x3DB851EC;
static const float data_2B89C[2] = {0.075f, 0.008f};
static const Vector3i data_2B8A4 = {.x = 13111, .y = 817, .z = 50684};
static const Vector3 data_2B8B0 = {.x = 0.091f, .y = -0.087f, .z = 0.0671f};
static const Vector3i data_2B8BC = {.x = 20024, .y = 65353, .z = 11832};
static const Vector3 data_2B8C8 = {.x = 0.0f, .y = 0.0f, .z = 0.0f};
static const Vector3i data_2B8D4 = {.x = 0, .y = 0, .z = 0};
static const Vector3 data_2B8E0 = {.x = -0.042f, .y = 0.1735f, .z = 0.2062f};
static const char data_2B8EC[] __attribute__((scn_string_literal)) = "E1066A001";
static const char data_2B8F6[] __attribute__((scn_string_literal)) = "E1066A002";
static const char data_2B900[] __attribute__((scn_string_literal)) = "E1066A003";
static const char data_2B90A[] __attribute__((scn_string_literal)) = "E1066A004";
static const char data_2B914[] __attribute__((scn_string_literal)) = "E1066A005";
static const char data_2B91E[] __attribute__((scn_string_literal)) = "E1066A006";
static const char data_2B928[] __attribute__((scn_string_literal)) = "E1066A007";
static const char data_2B932[] __attribute__((scn_string_literal)) = "E1066A008";
static const char data_2B93C[] __attribute__((scn_string_literal)) = "E1066A009";
static const char data_2B946[] __attribute__((scn_string_literal)) = "E1066A010";
static const char data_2B950[] __attribute__((scn_string_literal)) = "E1066A011";
static const char data_2B95A[] __attribute__((scn_string_literal)) = "E1066A026";
static const char data_2B964[] __attribute__((scn_string_literal)) = "E1066A027";
static const char data_2B96E[] __attribute__((scn_string_literal)) = "E1066A019";
static const char data_2B978[] __attribute__((scn_string_literal)) = "E1066A020";
static const char data_2B982[] __attribute__((scn_string_literal)) = "E1066A021";
static const char data_2B98C[] __attribute__((scn_string_literal)) = "E1066A014";
static const char data_2B996[] __attribute__((scn_string_literal)) = "E1066A015";
static const char data_2B9A0[] __attribute__((scn_string_literal)) = "E1066A016";
static const char data_2B9AA[] __attribute__((scn_string_literal)) = "E1066A017";
static const char data_2B9B4[] __attribute__((scn_string_literal)) = "E1066A018";
static const char data_2B9BE[] __attribute__((scn_string_literal)) = "E1066A022";
static const char data_2B9C8[] __attribute__((scn_string_literal)) = "E1066A025";
static const char data_2B9D2[] __attribute__((scn_string_literal)) = "E1066A024";
static const char data_2B9DC[12] __attribute__((scn_string_literal)) = "E1066A023";
static const Vector3 data_2B9E8 = {.x = 0.337795f, .y = 0.596267f, .z = -0.181433f};
static const Vector3 data_2B9F4 = {.x = 0.091f, .y = -0.087f, .z = 0.0671f};
static const Vector3i data_2BA00 = {.x = 20024, .y = 65353, .z = 11832};
static const char data_2BA0C[12] __attribute__((scn_string_literal)) = "JINGURU01";
static const uint8_t data_2BA18[28] = {0};
static const int32_t data_2BA34[7] = {-1, 0, 0, 0, 0, 0, 0};
static char data_2BA50[] = "$B14A0M1G.MT5";
static const uint8_t data_2BA5E[2] = {0};
static const uint32_t data_2BA60 = 0xFFFF0000;
static const uint32_t data_2BA64[2] = {0, 0x80000000U};
static const float data_2BA6C[5] = {15.0673f, 1.0992f, 2.0482f, 0.0f, 0.0f};
static const int32_t data_2BA80[2] = {-65536, 101};
static const uint32_t data_2BA88 = 0x71C74000U;
static const float data_2BA8C[5] = {9.5827f, 1.3422f, 1.3926f, 0.0f, 0.0f};
static const int32_t data_2BAA0[2] = {-65536, 102};
static const uint32_t data_2BAA8 = 0x71C74000U;
static const float data_2BAAC[5] = {9.5219f, 1.635f, 32.3657f, 0.0f, 0.0f};
static const int32_t data_2BAC0[2] = {-65536, 103};
static const uint32_t data_2BAC8 = 0x71C77C71U;
static const float data_2BACC[5] = {28.2411f, 1.696f, 9.228f, 0.0f, 0.0f};
static const int32_t data_2BAE0[2] = {-65536, 104};
static const uint32_t data_2BAE8[2] = {0x71C78000U, 0x41A56E2FU};
static const float data_2BAF0[4] = {1.4047f, 6.9876f, 0.0f, 0.0f};
static const int32_t data_2BB00[2] = {-65536, 106};
static const uint32_t data_2BB08 = 0x71C74000U;
static const float data_2BB0C[5] = {9.5741f, 1.5472f, -1.9451f, 0.0f, 0.0f};
static const int32_t data_2BB20[2] = {-65536, 107};
static const uint32_t data_2BB28 = 0x71C74000U;
static const float data_2BB2C[5] = {9.4969f, 1.25f, 12.0913f, 0.0f, 0.0f};
static const int32_t data_2BB40[2] = {-65536, 109};
static const uint32_t data_2BB48 = 0x71C74000U;
static const float data_2BB4C[5] = {8.1858f, 1.7775f, 27.101f, 0.0f, 0.0f};
static const int32_t data_2BB60[2] = {-65536, 110};
static const uint32_t data_2BB68 = 0x71C7671CU;
static const float data_2BB6C[2] = {14.3068f, 1.5725f};
static const uint32_t data_2BB74[3] = {0x420BDC43U, 0, 0};
static const int32_t data_2BB80[2] = {-65536, 111};
static const uint32_t data_2BB88 = 0x71C7E38EU;
static const float data_2BB8C[2] = {20.4664f, 1.6303f};
static const uint32_t data_2BB94[3] = {0x420D7909U, 0, 0};
static const int32_t data_2BBA0[2] = {-65536, 112};
static const uint32_t data_2BBA8 = 0x71C70000U;
static const float data_2BBAC[5] = {13.6589f, 2.0333f, 27.6413f, 0.0f, 0.0f};
static const int32_t data_2BBC0[2] = {-65536, 116};
static const uint32_t data_2BBC8[2] = {0x71C7F60BU, 0xC20B4275U};
static const float data_2BBD0[4] = {4.2982f, -2.5379f, 0.0f, 0.0f};
static const int32_t data_2BBE0[2] = {-65536, 117};
static const uint32_t data_2BBE8[2] = {0x71C78000U, 0xC1C9F803U};
static const float data_2BBF0[4] = {2.5835f, 4.8512f, 0.0f, 0.0f};
static const int32_t data_2BC00[2] = {-65536, 118};
static const uint32_t data_2BC08 = 0x71C74000U;
static const float data_2BC0C[5] = {-14.4027f, 2.0075f, 6.531f, 0.0f, 0.0f};
static const int32_t data_2BC20[2] = {-65536, 114};
static const uint32_t data_2BC28 = 0x71C7B1C7U;
static const float data_2BC2C[5] = {-8.3123f, 1.1153f, 9.8865f, 0.0f, 0.0f};
static const int32_t data_2BC40[2] = {-65536, 113};
static const uint32_t data_2BC48 = 0x71C78000U;
static const float data_2BC4C[2] = {-3.5656f, 1.14f};
static const uint32_t data_2BC54[3] = {0x41B453C3U, 0, 0};
static const int32_t data_2BC60[2] = {-65535, 123};
static const uint32_t data_2BC68[2] = {0x71C7F555U, 0xC18F6F00U};
static const float data_2BC70[4] = {3.139f, 27.362f, 0.0f, 0.0f};
static const int32_t data_2BC80[2] = {-65535, 124};
static const uint32_t data_2BC88[2] = {0x71C7F555U, 0xC1A8A95FU};
static const float data_2BC90 = 3.1365f;
static const uint32_t data_2BC94[3] = {0x41DCBDD9U, 0, 0};
static const int32_t data_2BCA0[2] = {-65534, 125};
static const uint32_t data_2BCA8[2] = {0x71C7F555U, 0xC18F6F00U};
static const float data_2BCB0 = 5.8396f;
static const uint32_t data_2BCB4[3] = {0x41DB020CU, 0, 0};
static const int32_t data_2BCC0[2] = {-65534, 126};
static const uint32_t data_2BCC8[2] = {0x71C7F555U, 0xC1A8A95FU};
static const float data_2BCD0 = 5.8396f;
static const uint32_t data_2BCD4[3] = {0x41DCC04FU, 0, 0};
static const int32_t data_2BCE0[2] = {-65535, 119};
static const uint32_t data_2BCE8[2] = {0x71C70000U, 0xC182E595U};
static const float data_2BCF0 = 3.1365f;
static const uint32_t data_2BCF4[3] = {0x420A4CCDU, 0, 0};
static const int32_t data_2BD00[2] = {-65535, 120};
static const uint32_t data_2BD08[2] = {0x71C70000U, 0xC19ACE07U};
static const float data_2BD10[4] = {3.1365f, 35.5826f, 0.0f, 0.0f};
static const int32_t data_2BD20[2] = {-65534, 121};
static const uint32_t data_2BD28[2] = {0x71C70000U, 0xC182DD64U};
static const float data_2BD30 = 5.8365f;
static const uint32_t data_2BD34[3] = {0x420A578DU, 0, 0};
static const int32_t data_2BD40[2] = {-65534, 122};
static const uint32_t data_2BD48 = 0x71C70000U;
static const float data_2BD4C[2] = {-19.3489f, 5.8365f};
static const uint32_t data_2BD54[3] = {0x420E5C78U, 0, 0};
static const int32_t data_2BD60[2] = {-65536, 112};
static const uint32_t data_2BD68 = 0x71C7F1C7U;
static const float data_2BD6C[5] = {-10.4958f, 1.2717f, 3.1206f, 0.0f, 0.0f};
static const int32_t data_2BD80[2] = {-65536, 108};
static const uint32_t data_2BD88 = 0x71C74000U;
static const float data_2BD8C[5] = {9.493f, 1.2451f, 16.1099f, 0.0f, 0.0f};
static const int32_t data_2BDA0[2] = {-65536, 105};
static const uint32_t data_2BDA8 = 0x71C70000U;
static const float data_2BDAC[5] = {8.9036f, 1.55f, -16.9252f, 0.0f, 0.0f};
static const int32_t data_2BDC0[2] = {-65536, 115};
static const uint32_t data_2BDC8[2] = {0x71C7F555U, 0xC18881A3U};
static const float data_2BDD0[4] = {2.971f, -3.2252f, 0.0f, 0.0f};
static const int32_t data_2BDE0[2] = {-65536, 131};
static const uint32_t data_2BDE8 = 0x71C7E0B6U;
static const float data_2BDEC[5] = {19.1329f, 1.23f, 34.5327f, 0.0f, 0.0f};
static const int32_t data_2BE00[2] = {-65536, 131};
static const uint32_t data_2BE08 = 0x71C70000U;
static const float data_2BE0C[5] = {2.1828f, 1.2508f, 18.9f, 0.0f, 0.0f};
static const int32_t data_2BE20[2] = {-65536, 132};
static const uint32_t data_2BE28 = 0x71C7CFA4U;
static const float data_2BE2C[5] = {-6.6538f, 1.2028f, 4.0641f, 0.0f, 0.0f};
static const int32_t data_2BE40[2] = {-65536, 133};
static const uint32_t data_2BE48 = 0x71C7EAAAU;
static const float data_2BE4C[5] = {13.7469f, 1.2147f, -8.3833f, 0.0f, 0.0f};
static const int32_t data_2BE60[2] = {-65536, 135};
static const uint32_t data_2BE68 = 0x71C7B8E3U;
static const float data_2BE6C[5] = {13.8298f, 1.2958f, 23.5861f, 0.0f, 0.0f};
static const int32_t data_2BE80[2] = {-65536, 136};
static const uint32_t data_2BE88 = 0x71C74000U;
static const float data_2BE8C[5] = {9.5274f, 1.4023f, 32.3564f, 0.0f, 0.0f};
static const int32_t data_2BEA0[2] = {-65536, 137};
static const uint32_t data_2BEA8 = 0x71C7C000U;
static const float data_2BEAC[2] = {14.7023f, 1.475f};
static const uint32_t data_2BEB4[4] = {0x41ADA29CU, 0, 0, 0};
static const shen_name data_2BEC4 = SHEN_NAME('GCH0');
static const float data_2BEC8[7] = {40128.0f, 0.0f, 0.76f, 0.055f, 0.0f, 0.0f, 0.0f};
static const shen_name data_2BEE4 = SHEN_NAME('GCH1');
static const float data_2BEE8[7] = {40128.0f, 0.0f, 0.76f, 0.055f, 0.0f, 0.0f, 0.0f};
static const shen_name data_2BF04 = SHEN_NAME('GCH2');
static const float data_2BF08[7] = {40128.0f, 0.0f, 0.76f, 0.055f, 0.0f, 0.0f, 0.0f};
static const shen_name data_2BF24 = SHEN_NAME('GCH3');
static const float data_2BF28[7] = {40128.0f, 0.0f, 0.76f, 0.055f, 0.0f, 0.0f, 0.0f};
static const shen_name data_2BF44 = SHEN_NAME('GCH4');
static const float data_2BF48[7] = {40128.0f, 0.0f, 0.76f, 0.055f, 0.0f, 0.0f, 0.0f};
static const shen_name data_2BF64 = SHEN_NAME('GCH5');
static const float data_2BF68[6] = {40128.0f, 0.0f, 0.76f, 0.055f, 0.0f, 0.0f};
static const int32_t data_2BF80 = 65535;
static uint32_t data_2BF84 = 0x417113A9;
static const float data_2BF88[2] = {1.1f, 2.0982f};
static const char data_2BF90[6] __attribute__((scn_string_literal)) = "E1040";
static const char data_2BF96[10] __attribute__((scn_string_literal)) = "E1040A028";
static const char data_2BFA0[10] __attribute__((scn_string_literal)) = "E1040A029";
static const char data_2BFAA[6] __attribute__((scn_string_literal)) = "E1040";
static const char data_2BFB0[10] __attribute__((scn_string_literal)) = "E1040A030";
static const char data_2BFBA[10] __attribute__((scn_string_literal)) = "E1040A031";
static const char data_2BFC4[10] __attribute__((scn_string_literal)) = "E1040A032";
static const char data_2BFCE[6] __attribute__((scn_string_literal)) = "E1040";
static const Vector3 data_2BFD4 = {.x = 21.6f, .y = 0.75f, .z = -6.18f};
static const char data_2BFE0[13] __attribute__((scn_string_literal)) = "e1shugyo.snd";
static const char data_2BFED[10] __attribute__((scn_string_literal)) = "E1040A001";
static const char data_2BFF7[10] __attribute__((scn_string_literal)) = "E1040A003";
static const char data_2C001[10] __attribute__((scn_string_literal)) = "E1040A009";
static const char data_2C00B[10] __attribute__((scn_string_literal)) = "E1040A010";
static const char data_2C015[10] __attribute__((scn_string_literal)) = "E1040A012";
static const char data_2C01F[10] __attribute__((scn_string_literal)) = "E1040A013";
static const char data_2C029[10] __attribute__((scn_string_literal)) = "E1040A014";
static const char data_2C033[10] __attribute__((scn_string_literal)) = "E1040A018";
static const char data_2C03D[10] __attribute__((scn_string_literal)) = "E1040A020";
static const char data_2C047[10] __attribute__((scn_string_literal)) = "E1040A015";
static const char data_2C051[10] __attribute__((scn_string_literal)) = "E1040A016";
static const char data_2C05B[10] __attribute__((scn_string_literal)) = "E1040A017";
static const char data_2C065[6] __attribute__((scn_string_literal)) = "E1040";
static const char data_2C06B[9] __attribute__((scn_string_literal)) = "E1040";
static const Vector3 data_2C074 = {.x = 19.9f, .y = 0.0f, .z = -8.2f};
static const Vector3 data_2C080 = {.x = 23.28f, .y = 0.75f, .z = -9.38f};
static const char data_2C08C[16] __attribute__((scn_string_literal)) = "\012 itou ga iru\012\012";
static const char data_2C09C[18] __attribute__((scn_string_literal)) = "\012 mayumi ga iru\012\012";
static const char data_2C0AE[22] __attribute__((scn_string_literal)) = "\012 yamagisi ga iru\012\012";
static const int32_t data_2C0C4[57] = {
  0, 0, -8046, 0, 0, -1820, -13088, 4350, -2894, 0, 0, -9011, 0, 0, -18422, 0, 0, -9812, 0, 0, 0, 0, 0, -7518, 0, 0, -17385, 0, 0, -11559, 0, 0, 0, 0,
  0, -5734, 0, 0, -18058, 0, 0, -12233, 0, 0, 0, 0, 0, -7827, 0, 0, -16183, 0, 0, -13981, 0, -72, 0
};
static const int32_t data_2C1A8[57] = {
  0, 0, -2712, 0, 0, -691, 0, 0, 0, 0, 0, -1092, 0, 0, -1438, 0, 0, -2748, 0, 0, 0, 0, 0, 364, 0, 0, -4132, 0, 0, -3531, 0, 0, 0, 0, 0, -2020, 0, 0,
  -6881, 0, 0, -5333, 0, 109, -200, 0, 0, -3986, 0, 0, -8829, 0, 0, -7245, 0, -764, -837
};
static const uint32_t data_2C28C = 0x303131;
static const uint32_t data_2C290 = 0x393131;
static const uint32_t data_2C294 = 0x373731;
static const uint32_t data_2C298 = 0x343031;
static const char data_2C29C[] __attribute__((scn_string_literal)) = "61564";
static const char data_2C2A2[] __attribute__((scn_string_literal)) = "37742";
static const char data_2C2A8[] __attribute__((scn_string_literal)) = "37749";
static const char data_2C2AE[] __attribute__((scn_string_literal)) = "22550";
static const char data_2C2B4[] __attribute__((scn_string_literal)) = "046861564";
static const char data_2C2BE[] __attribute__((scn_string_literal)) = "046837742";
static const char data_2C2C8[] __attribute__((scn_string_literal)) = "046837749";
static const char data_2C2D2[] __attribute__((scn_string_literal)) = "046822550";
static const char data_2C2DC[] __attribute__((scn_string_literal)) = "2?????";
static const char data_2C2E3[] __attribute__((scn_string_literal)) = "5?????";
static const char data_2C2EA[] __attribute__((scn_string_literal)) = "7?????";
static const char data_2C2F1[] __attribute__((scn_string_literal)) = "8?????";
static const char data_2C2F8[] __attribute__((scn_string_literal)) = "3?????";
static const char data_2C2FF[] __attribute__((scn_string_literal)) = "4?????";
static const char data_2C306[] __attribute__((scn_string_literal)) = "6?????";
static const char data_2C30D[] __attribute__((scn_string_literal)) = "9?????";
static const char data_2C314[] __attribute__((scn_string_literal)) = "0?????????";
static const char data_2C31F[] __attribute__((scn_string_literal)) = "1?????";
static const char data_2C326[6] __attribute__((scn_string_literal)) = "E1066";
static const char data_2C32C[10] __attribute__((scn_string_literal)) = "E1066A012";
static const char data_2C336[10] __attribute__((scn_string_literal)) = "E1066A013";
static const char data_2C340[10] __attribute__((scn_string_literal)) = "E1066A006";
static const char data_2C34A[6] __attribute__((scn_string_literal)) = "E1066";
static const char data_2C350[12] __attribute__((scn_string_literal)) = "メモ帳消去\012";
static const char data_2C35C[6] __attribute__((scn_string_literal)) = "E1032";
static const char data_2C362[10] __attribute__((scn_string_literal)) = "E1032A001";
static const char data_2C36C[10] __attribute__((scn_string_literal)) = "E1032A002";
static const char data_2C376[10] __attribute__((scn_string_literal)) = "E1032A003";
static const char data_2C380[10] __attribute__((scn_string_literal)) = "E1032A004";
static const char data_2C38A[10] __attribute__((scn_string_literal)) = "E1032A005";
static const char data_2C394[6] __attribute__((scn_string_literal)) = "E1032";
static const char data_2C39A[6] __attribute__((scn_string_literal)) = "E1032";
static const char data_2C3A0[10] __attribute__((scn_string_literal)) = "E1032A061";
static const char data_2C3AA[10] __attribute__((scn_string_literal)) = "E1032A062";
static const char data_2C3B4[10] __attribute__((scn_string_literal)) = "E1032A063";
static const char data_2C3BE[10] __attribute__((scn_string_literal)) = "E1032A064";
static const char data_2C3C8[6] __attribute__((scn_string_literal)) = "E1032";
static const char data_2C3CE[6] __attribute__((scn_string_literal)) = "E1032";
static const char data_2C3D4[6] __attribute__((scn_string_literal)) = "E1032";
static const char data_2C3DA[6] __attribute__((scn_string_literal)) = "E1032";
static const char data_2C3E0[10] __attribute__((scn_string_literal)) = "E1032A031";
static const char data_2C3EA[10] __attribute__((scn_string_literal)) = "E1032A029";
static const char data_2C3F4[10] __attribute__((scn_string_literal)) = "E1032A032";
static const char data_2C3FE[10] __attribute__((scn_string_literal)) = "E1032A033";
static const char data_2C408[6] __attribute__((scn_string_literal)) = "E1032";
static const char data_2C40E[10] __attribute__((scn_string_literal)) = "E1032A028";
static const char data_2C418[10] __attribute__((scn_string_literal)) = "E1032A029";
static const char data_2C422[10] __attribute__((scn_string_literal)) = "E1032A030";
static const char data_2C42C[6] __attribute__((scn_string_literal)) = "E1032";
static const char data_2C432[10] __attribute__((scn_string_literal)) = "E1032G001";
static const char data_2C43C[10] __attribute__((scn_string_literal)) = "E1032G002";
static const char data_2C446[6] __attribute__((scn_string_literal)) = "E1032";
static const char data_2C44C[10] __attribute__((scn_string_literal)) = "E1032I001";
static const char data_2C456[10] __attribute__((scn_string_literal)) = "E1032A052";
static const char data_2C460[10] __attribute__((scn_string_literal)) = "E1032I007";
static const char data_2C46A[10] __attribute__((scn_string_literal)) = "E1032I008";
static const char data_2C474[10] __attribute__((scn_string_literal)) = "E1032A097";
static const char data_2C47E[10] __attribute__((scn_string_literal)) = "E1032I009";
static const char data_2C488[10] __attribute__((scn_string_literal)) = "E1032I010";
static const char data_2C492[10] __attribute__((scn_string_literal)) = "E1032A054";
static const char data_2C49C[10] __attribute__((scn_string_literal)) = "E1032I011";
static const char data_2C4A6[10] __attribute__((scn_string_literal)) = "E1032A055";
static const char data_2C4B0[10] __attribute__((scn_string_literal)) = "E1032I012";
static const char data_2C4BA[10] __attribute__((scn_string_literal)) = "E1032A056";
static const char data_2C4C4[10] __attribute__((scn_string_literal)) = "E1032I013";
static const char data_2C4CE[10] __attribute__((scn_string_literal)) = "E1032A057";
static const char data_2C4D8[10] __attribute__((scn_string_literal)) = "E1032I011";
static const char data_2C4E2[10] __attribute__((scn_string_literal)) = "E1032A059";
static const char data_2C4EC[10] __attribute__((scn_string_literal)) = "E1032I009";
static const char data_2C4F6[10] __attribute__((scn_string_literal)) = "E1032I010";
static const char data_2C500[10] __attribute__((scn_string_literal)) = "E1032A054";
static const char data_2C50A[10] __attribute__((scn_string_literal)) = "E1032I011";
static const char data_2C514[10] __attribute__((scn_string_literal)) = "E1032A060";
static const char data_2C51E[10] __attribute__((scn_string_literal)) = "E1032I009";
static const char data_2C528[10] __attribute__((scn_string_literal)) = "E1032I010";
static const char data_2C532[10] __attribute__((scn_string_literal)) = "E1032A054";
static const char data_2C53C[10] __attribute__((scn_string_literal)) = "E1032I011";
static const char data_2C546[10] __attribute__((scn_string_literal)) = "E1032I001";
static const char data_2C550[10] __attribute__((scn_string_literal)) = "E1032A007";
static const char data_2C55A[10] __attribute__((scn_string_literal)) = "E1032A006";
static const char data_2C564[10] __attribute__((scn_string_literal)) = "E1032I002";
static const char data_2C56E[10] __attribute__((scn_string_literal)) = "E1032I003";
static const char data_2C578[10] __attribute__((scn_string_literal)) = "E1032I004";
static const char data_2C582[10] __attribute__((scn_string_literal)) = "E1032I005";
static const char data_2C58C[10] __attribute__((scn_string_literal)) = "E1032I006";
static const char data_2C596[10] __attribute__((scn_string_literal)) = "E1032A008";
static const char data_2C5A0[10] __attribute__((scn_string_literal)) = "E1032A009";
static const char data_2C5AA[10] __attribute__((scn_string_literal)) = "E1032K001";
static const char data_2C5B4[10] __attribute__((scn_string_literal)) = "E1032K002";
static const char data_2C5BE[10] __attribute__((scn_string_literal)) = "E1032K019";
static const char data_2C5C8[10] __attribute__((scn_string_literal)) = "E1032K020";
static const char data_2C5D2[10] __attribute__((scn_string_literal)) = "E1032K022";
static const char data_2C5DC[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_2C5E6[10] __attribute__((scn_string_literal)) = "E1032K016";
static const char data_2C5F0[10] __attribute__((scn_string_literal)) = "E1032K017";
static const char data_2C5FA[10] __attribute__((scn_string_literal)) = "E1032K018";
static const char data_2C604[10] __attribute__((scn_string_literal)) = "E1032K003";
static const char data_2C60E[10] __attribute__((scn_string_literal)) = "E1032K016";
static const char data_2C618[10] __attribute__((scn_string_literal)) = "E1032K003";
static const char data_2C622[10] __attribute__((scn_string_literal)) = "E1032K017";
static const char data_2C62C[10] __attribute__((scn_string_literal)) = "E1032K003";
static const char data_2C636[10] __attribute__((scn_string_literal)) = "E1032K018";
static const char data_2C640[10] __attribute__((scn_string_literal)) = "E1032K004";
static const char data_2C64A[10] __attribute__((scn_string_literal)) = "E1032K016";
static const char data_2C654[10] __attribute__((scn_string_literal)) = "E1032K004";
static const char data_2C65E[10] __attribute__((scn_string_literal)) = "E1032K017";
static const char data_2C668[10] __attribute__((scn_string_literal)) = "E1032K004";
static const char data_2C672[10] __attribute__((scn_string_literal)) = "E1032K018";
static const char data_2C67C[10] __attribute__((scn_string_literal)) = "E1032K005";
static const char data_2C686[10] __attribute__((scn_string_literal)) = "E1032K016";
static const char data_2C690[10] __attribute__((scn_string_literal)) = "E1032K005";
static const char data_2C69A[10] __attribute__((scn_string_literal)) = "E1032K017";
static const char data_2C6A4[10] __attribute__((scn_string_literal)) = "E1032K005";
static const char data_2C6AE[10] __attribute__((scn_string_literal)) = "E1032K018";
static const char data_2C6B8[10] __attribute__((scn_string_literal)) = "E1032K006";
static const char data_2C6C2[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_2C6CC[10] __attribute__((scn_string_literal)) = "E1032K006";
static const char data_2C6D6[10] __attribute__((scn_string_literal)) = "E1032K017";
static const char data_2C6E0[10] __attribute__((scn_string_literal)) = "E1032K006";
static const char data_2C6EA[10] __attribute__((scn_string_literal)) = "E1032K018";
static const char data_2C6F4[10] __attribute__((scn_string_literal)) = "E1032K007";
static const char data_2C6FE[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_2C708[10] __attribute__((scn_string_literal)) = "E1032K007";
static const char data_2C712[10] __attribute__((scn_string_literal)) = "E1032K017";
static const char data_2C71C[10] __attribute__((scn_string_literal)) = "E1032K007";
static const char data_2C726[10] __attribute__((scn_string_literal)) = "E1032K018";
static const char data_2C730[10] __attribute__((scn_string_literal)) = "E1032K008";
static const char data_2C73A[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_2C744[10] __attribute__((scn_string_literal)) = "E1032K008";
static const char data_2C74E[10] __attribute__((scn_string_literal)) = "E1032K017";
static const char data_2C758[10] __attribute__((scn_string_literal)) = "E1032K008";
static const char data_2C762[10] __attribute__((scn_string_literal)) = "E1032K018";
static const char data_2C76C[10] __attribute__((scn_string_literal)) = "E1032K009";
static const char data_2C776[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_2C780[10] __attribute__((scn_string_literal)) = "E1032K009";
static const char data_2C78A[10] __attribute__((scn_string_literal)) = "E1032K016";
static const char data_2C794[10] __attribute__((scn_string_literal)) = "E1032K009";
static const char data_2C79E[10] __attribute__((scn_string_literal)) = "E1032K018";
static const char data_2C7A8[10] __attribute__((scn_string_literal)) = "E1032K010";
static const char data_2C7B2[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_2C7BC[10] __attribute__((scn_string_literal)) = "E1032K010";
static const char data_2C7C6[10] __attribute__((scn_string_literal)) = "E1032K016";
static const char data_2C7D0[10] __attribute__((scn_string_literal)) = "E1032K010";
static const char data_2C7DA[10] __attribute__((scn_string_literal)) = "E1032K018";
static const char data_2C7E4[10] __attribute__((scn_string_literal)) = "E1032K011";
static const char data_2C7EE[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_2C7F8[10] __attribute__((scn_string_literal)) = "E1032K011";
static const char data_2C802[10] __attribute__((scn_string_literal)) = "E1032K016";
static const char data_2C80C[10] __attribute__((scn_string_literal)) = "E1032K011";
static const char data_2C816[10] __attribute__((scn_string_literal)) = "E1032K018";
static const char data_2C820[10] __attribute__((scn_string_literal)) = "E1032K012";
static const char data_2C82A[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_2C834[10] __attribute__((scn_string_literal)) = "E1032K012";
static const char data_2C83E[10] __attribute__((scn_string_literal)) = "E1032K016";
static const char data_2C848[10] __attribute__((scn_string_literal)) = "E1032K012";
static const char data_2C852[10] __attribute__((scn_string_literal)) = "E1032K017";
static const char data_2C85C[10] __attribute__((scn_string_literal)) = "E1032K013";
static const char data_2C866[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_2C870[10] __attribute__((scn_string_literal)) = "E1032K013";
static const char data_2C87A[10] __attribute__((scn_string_literal)) = "E1032K016";
static const char data_2C884[10] __attribute__((scn_string_literal)) = "E1032K013";
static const char data_2C88E[10] __attribute__((scn_string_literal)) = "E1032K017";
static const char data_2C898[10] __attribute__((scn_string_literal)) = "E1032K014";
static const char data_2C8A2[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_2C8AC[10] __attribute__((scn_string_literal)) = "E1032K014";
static const char data_2C8B6[10] __attribute__((scn_string_literal)) = "E1032K016";
static const char data_2C8C0[10] __attribute__((scn_string_literal)) = "E1032K014";
static const char data_2C8CA[10] __attribute__((scn_string_literal)) = "E1032K017";
static const char data_2C8D4[10] __attribute__((scn_string_literal)) = "E1032K004";
static const char data_2C8DE[10] __attribute__((scn_string_literal)) = "E1032K007";
static const char data_2C8E8[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_2C8F2[10] __attribute__((scn_string_literal)) = "E1032K004";
static const char data_2C8FC[10] __attribute__((scn_string_literal)) = "E1032K007";
static const char data_2C906[10] __attribute__((scn_string_literal)) = "E1032K017";
static const char data_2C910[10] __attribute__((scn_string_literal)) = "E1032K004";
static const char data_2C91A[10] __attribute__((scn_string_literal)) = "E1032K006";
static const char data_2C924[10] __attribute__((scn_string_literal)) = "E1032K017";
static const char data_2C92E[10] __attribute__((scn_string_literal)) = "E1032K004";
static const char data_2C938[10] __attribute__((scn_string_literal)) = "E1032K013";
static const char data_2C942[10] __attribute__((scn_string_literal)) = "E1032K017";
static const char data_2C94C[10] __attribute__((scn_string_literal)) = "E1032K005";
static const char data_2C956[10] __attribute__((scn_string_literal)) = "E1032K007";
static const char data_2C960[10] __attribute__((scn_string_literal)) = "E1032K018";
static const char data_2C96A[10] __attribute__((scn_string_literal)) = "E1032K007";
static const char data_2C974[10] __attribute__((scn_string_literal)) = "E1032K004";
static const char data_2C97E[10] __attribute__((scn_string_literal)) = "E1032K017";
static const char data_2C988[10] __attribute__((scn_string_literal)) = "E1032K008";
static const char data_2C992[10] __attribute__((scn_string_literal)) = "E1032K010";
static const char data_2C99C[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_2C9A6[10] __attribute__((scn_string_literal)) = "E1032K008";
static const char data_2C9B0[10] __attribute__((scn_string_literal)) = "E1032K013";
static const char data_2C9BA[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_2C9C4[10] __attribute__((scn_string_literal)) = "E1032K010";
static const char data_2C9CE[10] __attribute__((scn_string_literal)) = "E1032K005";
static const char data_2C9D8[10] __attribute__((scn_string_literal)) = "E1032K016";
static const char data_2C9E2[10] __attribute__((scn_string_literal)) = "E1032K010";
static const char data_2C9EC[10] __attribute__((scn_string_literal)) = "E1032K004";
static const char data_2C9F6[10] __attribute__((scn_string_literal)) = "E1032K018";
static const char data_2CA00[10] __attribute__((scn_string_literal)) = "E1032K010";
static const char data_2CA0A[10] __attribute__((scn_string_literal)) = "E1032K007";
static const char data_2CA14[10] __attribute__((scn_string_literal)) = "E1032K018";
static const char data_2CA1E[10] __attribute__((scn_string_literal)) = "E1032K011";
static const char data_2CA28[10] __attribute__((scn_string_literal)) = "E1032K007";
static const char data_2CA32[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_2CA3C[10] __attribute__((scn_string_literal)) = "E1032K013";
static const char data_2CA46[10] __attribute__((scn_string_literal)) = "E1032K008";
static const char data_2CA50[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_2CA5A[10] __attribute__((scn_string_literal)) = "E1032K010";
static const char data_2CA64[10] __attribute__((scn_string_literal)) = "E1032K005";
static const char data_2CA6E[10] __attribute__((scn_string_literal)) = "E1032K016";
static const char data_2CA78[10] __attribute__((scn_string_literal)) = "E1032K004";
static const char data_2CA82[10] __attribute__((scn_string_literal)) = "E1032K010";
static const char data_2CA8C[10] __attribute__((scn_string_literal)) = "E1032K016";
static const char data_2CA96[10] __attribute__((scn_string_literal)) = "E1032K004";
static const char data_2CAA0[10] __attribute__((scn_string_literal)) = "E1032K008";
static const char data_2CAAA[10] __attribute__((scn_string_literal)) = "E1032K018";
static const char data_2CAB4[10] __attribute__((scn_string_literal)) = "E1032K003";
static const char data_2CABE[10] __attribute__((scn_string_literal)) = "E1032K007";
static const char data_2CAC8[10] __attribute__((scn_string_literal)) = "E1032K018";
static const char data_2CAD2[10] __attribute__((scn_string_literal)) = "E1032K007";
static const char data_2CADC[10] __attribute__((scn_string_literal)) = "E1032K010";
static const char data_2CAE6[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_2CAF0[10] __attribute__((scn_string_literal)) = "E1032K008";
static const char data_2CAFA[10] __attribute__((scn_string_literal)) = "E1032K004";
static const char data_2CB04[10] __attribute__((scn_string_literal)) = "E1032K018";
static const char data_2CB0E[10] __attribute__((scn_string_literal)) = "E1032K007";
static const char data_2CB18[10] __attribute__((scn_string_literal)) = "E1032K010";
static const char data_2CB22[10] __attribute__((scn_string_literal)) = "E1032K018";
static const char data_2CB2C[10] __attribute__((scn_string_literal)) = "E1032K004";
static const char data_2CB36[10] __attribute__((scn_string_literal)) = "E1032K010";
static const char data_2CB40[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_2CB4A[10] __attribute__((scn_string_literal)) = "E1032K004";
static const char data_2CB54[10] __attribute__((scn_string_literal)) = "E1032K007";
static const char data_2CB5E[10] __attribute__((scn_string_literal)) = "E1032K017";
static const char data_2CB68[10] __attribute__((scn_string_literal)) = "E1032K004";
static const char data_2CB72[10] __attribute__((scn_string_literal)) = "E1032K007";
static const char data_2CB7C[10] __attribute__((scn_string_literal)) = "E1032K018";
static const char data_2CB86[10] __attribute__((scn_string_literal)) = "E1032K010";
static const char data_2CB90[10] __attribute__((scn_string_literal)) = "E1032K008";
static const char data_2CB9A[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_2CBA4[10] __attribute__((scn_string_literal)) = "E1032K013";
static const char data_2CBAE[10] __attribute__((scn_string_literal)) = "E1032K007";
static const char data_2CBB8[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_2CBC2[10] __attribute__((scn_string_literal)) = "E1032K004";
static const char data_2CBCC[10] __attribute__((scn_string_literal)) = "E1032K013";
static const char data_2CBD6[10] __attribute__((scn_string_literal)) = "E1032K016";
static const char data_2CBE0[10] __attribute__((scn_string_literal)) = "E1032K014";
static const char data_2CBEA[10] __attribute__((scn_string_literal)) = "E1032K007";
static const char data_2CBF4[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_2CBFE[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_2CC08[10] __attribute__((scn_string_literal)) = "E1032L001";
static const char data_2CC12[10] __attribute__((scn_string_literal)) = "E1032A098";
static const char data_2CC1C[10] __attribute__((scn_string_literal)) = "E1032L002";
static const char data_2CC26[10] __attribute__((scn_string_literal)) = "E1032A099";
static const char data_2CC30[10] __attribute__((scn_string_literal)) = "E1032L003";
static const char data_2CC3A[10] __attribute__((scn_string_literal)) = "E1032L004";
static const char data_2CC44[10] __attribute__((scn_string_literal)) = "E1032L005";
static const char data_2CC4E[10] __attribute__((scn_string_literal)) = "E1032A100";
static const char data_2CC58[10] __attribute__((scn_string_literal)) = "E1032L001";
static const char data_2CC62[10] __attribute__((scn_string_literal)) = "E1032A101";
static const char data_2CC6C[10] __attribute__((scn_string_literal)) = "E1032L006";
static const char data_2CC76[10] __attribute__((scn_string_literal)) = "E1032L007";
static const char data_2CC80[10] __attribute__((scn_string_literal)) = "E1032L008";
static const char data_2CC8A[10] __attribute__((scn_string_literal)) = "E1032L009";
static const char data_2CC94[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_2CC9E[10] __attribute__((scn_string_literal)) = "E1032A102";
static const char data_2CCA8[10] __attribute__((scn_string_literal)) = "E1032L010";
static const char data_2CCB2[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_2CCBC[10] __attribute__((scn_string_literal)) = "E1032L011";
static const char data_2CCC6[10] __attribute__((scn_string_literal)) = "E1032A101";
static const char data_2CCD0[10] __attribute__((scn_string_literal)) = "E1032L012";
static const char data_2CCDA[10] __attribute__((scn_string_literal)) = "E1032A103";
static const char data_2CCE4[10] __attribute__((scn_string_literal)) = "E1032L013";
static const char data_2CCEE[10] __attribute__((scn_string_literal)) = "E1032L014";
static const char data_2CCF8[10] __attribute__((scn_string_literal)) = "E1032L015";
static const char data_2CD02[10] __attribute__((scn_string_literal)) = "E1032A103";
static const char data_2CD0C[10] __attribute__((scn_string_literal)) = "E1032L016";
static const char data_2CD16[10] __attribute__((scn_string_literal)) = "E1032A104";
static const char data_2CD20[10] __attribute__((scn_string_literal)) = "E1032A105";
static const char data_2CD2A[10] __attribute__((scn_string_literal)) = "E1032L017";
static const char data_2CD34[10] __attribute__((scn_string_literal)) = "E1032A106";
static const char data_2CD3E[10] __attribute__((scn_string_literal)) = "E1032L018";
static const char data_2CD48[10] __attribute__((scn_string_literal)) = "E1032A107";
static const char data_2CD52[10] __attribute__((scn_string_literal)) = "E1032L019";
static const char data_2CD5C[10] __attribute__((scn_string_literal)) = "E1032A108";
static const char data_2CD66[10] __attribute__((scn_string_literal)) = "E1032L020";
static const char data_2CD70[10] __attribute__((scn_string_literal)) = "E1032L021";
static const char data_2CD7A[10] __attribute__((scn_string_literal)) = "E1032A109";
static const char data_2CD84[10] __attribute__((scn_string_literal)) = "E1032L022";
static const char data_2CD8E[10] __attribute__((scn_string_literal)) = "E1032A110";
static const char data_2CD98[10] __attribute__((scn_string_literal)) = "E1032L023";
static const char data_2CDA2[10] __attribute__((scn_string_literal)) = "E1032M001";
static const char data_2CDAC[10] __attribute__((scn_string_literal)) = "E1032A111";
static const char data_2CDB6[10] __attribute__((scn_string_literal)) = "E1032M002";
static const char data_2CDC0[10] __attribute__((scn_string_literal)) = "E1032A112";
static const char data_2CDCA[10] __attribute__((scn_string_literal)) = "E1032M003";
static const char data_2CDD4[10] __attribute__((scn_string_literal)) = "E1032A113";
static const char data_2CDDE[10] __attribute__((scn_string_literal)) = "E1032M004";
static const char data_2CDE8[10] __attribute__((scn_string_literal)) = "E1032A104";
static const char data_2CDF2[10] __attribute__((scn_string_literal)) = "E1032A114";
static const char data_2CDFC[10] __attribute__((scn_string_literal)) = "E1032M005";
static const char data_2CE06[10] __attribute__((scn_string_literal)) = "E1032M006";
static const char data_2CE10[10] __attribute__((scn_string_literal)) = "E1032A111";
static const char data_2CE1A[10] __attribute__((scn_string_literal)) = "E1032M002";
static const char data_2CE24[10] __attribute__((scn_string_literal)) = "E1032A115";
static const char data_2CE2E[10] __attribute__((scn_string_literal)) = "E1032M018";
static const char data_2CE38[10] __attribute__((scn_string_literal)) = "E1032A122";
static const char data_2CE42[10] __attribute__((scn_string_literal)) = "E1032M019";
static const char data_2CE4C[10] __attribute__((scn_string_literal)) = "E1032A123";
static const char data_2CE56[10] __attribute__((scn_string_literal)) = "E1032M019";
static const char data_2CE60[10] __attribute__((scn_string_literal)) = "E1032M020";
static const char data_2CE6A[10] __attribute__((scn_string_literal)) = "E1032A104";
static const char data_2CE74[10] __attribute__((scn_string_literal)) = "E1032A114";
static const char data_2CE7E[10] __attribute__((scn_string_literal)) = "E1032M010";
static const char data_2CE88[10] __attribute__((scn_string_literal)) = "E1032N001";
static const char data_2CE92[10] __attribute__((scn_string_literal)) = "E1032A124";
static const char data_2CE9C[10] __attribute__((scn_string_literal)) = "E1032A125";
static const char data_2CEA6[10] __attribute__((scn_string_literal)) = "E1032N002";
static const char data_2CEB0[10] __attribute__((scn_string_literal)) = "E1032A126";
static const char data_2CEBA[10] __attribute__((scn_string_literal)) = "E1032N003";
static const char data_2CEC4[10] __attribute__((scn_string_literal)) = "E1032A127";
static const char data_2CECE[10] __attribute__((scn_string_literal)) = "E1032A128";
static const char data_2CED8[10] __attribute__((scn_string_literal)) = "E1032N004";
static const char data_2CEE2[10] __attribute__((scn_string_literal)) = "E1032A129";
static const char data_2CEEC[10] __attribute__((scn_string_literal)) = "E1032N005";
static const char data_2CEF6[10] __attribute__((scn_string_literal)) = "E1032L011";
static const char data_2CF00[10] __attribute__((scn_string_literal)) = "E1032A101";
static const char data_2CF0A[10] __attribute__((scn_string_literal)) = "E1032L019";
static const char data_2CF14[10] __attribute__((scn_string_literal)) = "E1032A117";
static const char data_2CF1E[10] __attribute__((scn_string_literal)) = "E1032L024";
static const char data_2CF28[10] __attribute__((scn_string_literal)) = "E1032L025";
static const char data_2CF32[10] __attribute__((scn_string_literal)) = "E1032L026";
static const char data_2CF3C[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_2CF46[10] __attribute__((scn_string_literal)) = "E1032A118";
static const char data_2CF50[10] __attribute__((scn_string_literal)) = "E1032L023";
static const char data_2CF5A[10] __attribute__((scn_string_literal)) = "E1032M001";
static const char data_2CF64[10] __attribute__((scn_string_literal)) = "E1032A111";
static const char data_2CF6E[10] __attribute__((scn_string_literal)) = "E1032M002";
static const char data_2CF78[10] __attribute__((scn_string_literal)) = "E1032A112";
static const char data_2CF82[10] __attribute__((scn_string_literal)) = "E1032M015";
static const char data_2CF8C[10] __attribute__((scn_string_literal)) = "E1032A119";
static const char data_2CF96[10] __attribute__((scn_string_literal)) = "E1032M016";
static const char data_2CFA0[10] __attribute__((scn_string_literal)) = "E1032A104";
static const char data_2CFAA[10] __attribute__((scn_string_literal)) = "E1032A118";
static const char data_2CFB4[10] __attribute__((scn_string_literal)) = "E1032M017";
static const char data_2CFBE[10] __attribute__((scn_string_literal)) = "E1032O001";
static const char data_2CFC8[10] __attribute__((scn_string_literal)) = "E1032A136";
static const char data_2CFD2[10] __attribute__((scn_string_literal)) = "E1032O002";
static const char data_2CFDC[10] __attribute__((scn_string_literal)) = "E1032A137";
static const char data_2CFE6[10] __attribute__((scn_string_literal)) = "E1032O007";
static const char data_2CFF0[10] __attribute__((scn_string_literal)) = "E1032A127";
static const char data_2CFFA[10] __attribute__((scn_string_literal)) = "E1032O004";
static const char data_2D004[10] __attribute__((scn_string_literal)) = "E1032A138";
static const char data_2D00E[10] __attribute__((scn_string_literal)) = "E1032O005";
static const char data_2D018[10] __attribute__((scn_string_literal)) = "E1032A139";
static const char data_2D022[10] __attribute__((scn_string_literal)) = "E1032O006";
static const char data_2D02C[10] __attribute__((scn_string_literal)) = "E1032O001";
static const char data_2D036[10] __attribute__((scn_string_literal)) = "E1032A136";
static const char data_2D040[10] __attribute__((scn_string_literal)) = "E1032O002";
static const char data_2D04A[10] __attribute__((scn_string_literal)) = "E1032A140";
static const char data_2D054[10] __attribute__((scn_string_literal)) = "E1032O007";
static const char data_2D05E[10] __attribute__((scn_string_literal)) = "E1032A127";
static const char data_2D068[10] __attribute__((scn_string_literal)) = "E1032O004";
static const char data_2D072[10] __attribute__((scn_string_literal)) = "E1032A138";
static const char data_2D07C[10] __attribute__((scn_string_literal)) = "E1032O005";
static const char data_2D086[10] __attribute__((scn_string_literal)) = "E1032A139";
static const char data_2D090[10] __attribute__((scn_string_literal)) = "E1032O006";
static const char data_2D09A[10] __attribute__((scn_string_literal)) = "E1032P001";
static const char data_2D0A4[10] __attribute__((scn_string_literal)) = "E1032A085";
static const char data_2D0AE[10] __attribute__((scn_string_literal)) = "E1032P002";
static const char data_2D0B8[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_2D0C2[10] __attribute__((scn_string_literal)) = "E1032P003";
static const char data_2D0CC[10] __attribute__((scn_string_literal)) = "E1032A141";
static const char data_2D0D6[10] __attribute__((scn_string_literal)) = "E1032P004";
static const char data_2D0E0[10] __attribute__((scn_string_literal)) = "E1032A142";
static const char data_2D0EA[10] __attribute__((scn_string_literal)) = "E1032P005";
static const char data_2D0F4[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_2D0FE[10] __attribute__((scn_string_literal)) = "E1032P001";
static const char data_2D108[10] __attribute__((scn_string_literal)) = "E1032A085";
static const char data_2D112[10] __attribute__((scn_string_literal)) = "E1032P006";
static const char data_2D11C[10] __attribute__((scn_string_literal)) = "E1032A143";
static const char data_2D126[10] __attribute__((scn_string_literal)) = "E1032P007";
static const char data_2D130[10] __attribute__((scn_string_literal)) = "E1032A144";
static const char data_2D13A[10] __attribute__((scn_string_literal)) = "E1032P004";
static const char data_2D144[10] __attribute__((scn_string_literal)) = "E1032O008";
static const char data_2D14E[10] __attribute__((scn_string_literal)) = "E1032P008";
static const char data_2D158[10] __attribute__((scn_string_literal)) = "E1032A068";
static const char data_2D162[10] __attribute__((scn_string_literal)) = "E1032P009";
static const char data_2D16C[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_2D176[10] __attribute__((scn_string_literal)) = "E1032A085";
static const char data_2D180[10] __attribute__((scn_string_literal)) = "E1032P010";
static const char data_2D18A[10] __attribute__((scn_string_literal)) = "E1032A145";
static const char data_2D194[10] __attribute__((scn_string_literal)) = "E1032P011";
static const char data_2D19E[10] __attribute__((scn_string_literal)) = "E1032A146";
static const char data_2D1A8[10] __attribute__((scn_string_literal)) = "E1032P012";
static const char data_2D1B2[10] __attribute__((scn_string_literal)) = "E1032A147";
static const char data_2D1BC[10] __attribute__((scn_string_literal)) = "E1032P013";
static const char data_2D1C6[10] __attribute__((scn_string_literal)) = "E1032A148";
static const char data_2D1D0[10] __attribute__((scn_string_literal)) = "E1032P014";
static const char data_2D1DA[10] __attribute__((scn_string_literal)) = "E1032A149";
static const char data_2D1E4[10] __attribute__((scn_string_literal)) = "E1032P015";
static const char data_2D1EE[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_2D1F8[10] __attribute__((scn_string_literal)) = "E1032P016";
static const char data_2D202[10] __attribute__((scn_string_literal)) = "E1032A068";
static const char data_2D20C[10] __attribute__((scn_string_literal)) = "E1032P017";
static const char data_2D216[10] __attribute__((scn_string_literal)) = "E1032A085";
static const char data_2D220[10] __attribute__((scn_string_literal)) = "E1032P018";
static const char data_2D22A[10] __attribute__((scn_string_literal)) = "E1032A150";
static const char data_2D234[10] __attribute__((scn_string_literal)) = "E1032P019";
static const char data_2D23E[10] __attribute__((scn_string_literal)) = "E1032A151";
static const char data_2D248[10] __attribute__((scn_string_literal)) = "E1032P004";
static const char data_2D252[10] __attribute__((scn_string_literal)) = "E1032P016";
static const char data_2D25C[10] __attribute__((scn_string_literal)) = "E1032A068";
static const char data_2D266[10] __attribute__((scn_string_literal)) = "E1032P017";
static const char data_2D270[10] __attribute__((scn_string_literal)) = "E1032P021";
static const char data_2D27A[10] __attribute__((scn_string_literal)) = "E1032A152";
static const char data_2D284[10] __attribute__((scn_string_literal)) = "E1032P010";
static const char data_2D28E[10] __attribute__((scn_string_literal)) = "E1032A153";
static const char data_2D298[10] __attribute__((scn_string_literal)) = "E1032P022";
static const char data_2D2A2[10] __attribute__((scn_string_literal)) = "E1032A154";
static const char data_2D2AC[10] __attribute__((scn_string_literal)) = "E1032P023";
static const char data_2D2B6[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_2D2C0[10] __attribute__((scn_string_literal)) = "E1032P024";
static const char data_2D2CA[10] __attribute__((scn_string_literal)) = "E1032A068";
static const char data_2D2D4[10] __attribute__((scn_string_literal)) = "E1032P017";
static const char data_2D2DE[10] __attribute__((scn_string_literal)) = "E1032P001";
static const char data_2D2E8[10] __attribute__((scn_string_literal)) = "E1032A085";
static const char data_2D2F2[10] __attribute__((scn_string_literal)) = "E1032P025";
static const char data_2D2FC[10] __attribute__((scn_string_literal)) = "E1032A155";
static const char data_2D306[10] __attribute__((scn_string_literal)) = "E1032P004";
static const char data_2D310[10] __attribute__((scn_string_literal)) = "E1032A156";
static const char data_2D31A[10] __attribute__((scn_string_literal)) = "E1032O008";
static const char data_2D324[10] __attribute__((scn_string_literal)) = "E1032P008";
static const char data_2D32E[10] __attribute__((scn_string_literal)) = "E1032A154";
static const char data_2D338[10] __attribute__((scn_string_literal)) = "E1032P027";
static const char data_2D342[10] __attribute__((scn_string_literal)) = "E1032A068";
static const char data_2D34C[10] __attribute__((scn_string_literal)) = "E1032P017";
static const char data_2D356[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_2D360[10] __attribute__((scn_string_literal)) = "E1032P021";
static const char data_2D36A[10] __attribute__((scn_string_literal)) = "E1032A152";
static const char data_2D374[10] __attribute__((scn_string_literal)) = "E1032P028";
static const char data_2D37E[10] __attribute__((scn_string_literal)) = "E1032A158";
static const char data_2D388[10] __attribute__((scn_string_literal)) = "E1032P030";
static const char data_2D392[10] __attribute__((scn_string_literal)) = "E1032A159";
static const char data_2D39C[10] __attribute__((scn_string_literal)) = "E1032P066";
static const char data_2D3A6[10] __attribute__((scn_string_literal)) = "E1032P016";
static const char data_2D3B0[10] __attribute__((scn_string_literal)) = "E1032A068";
static const char data_2D3BA[10] __attribute__((scn_string_literal)) = "E1032P017";
static const char data_2D3C4[10] __attribute__((scn_string_literal)) = "E1032P021";
static const char data_2D3CE[10] __attribute__((scn_string_literal)) = "E1032A152";
static const char data_2D3D8[10] __attribute__((scn_string_literal)) = "E1032P032";
static const char data_2D3E2[10] __attribute__((scn_string_literal)) = "E1032A160";
static const char data_2D3EC[10] __attribute__((scn_string_literal)) = "E1032P033";
static const char data_2D3F6[10] __attribute__((scn_string_literal)) = "E1032P034";
static const char data_2D400[10] __attribute__((scn_string_literal)) = "E1032P080";
static const char data_2D40A[10] __attribute__((scn_string_literal)) = "E1032P081";
static const char data_2D414[10] __attribute__((scn_string_literal)) = "E1032A068";
static const char data_2D41E[10] __attribute__((scn_string_literal)) = "E1032P035";
static const char data_2D428[10] __attribute__((scn_string_literal)) = "E1032A141";
static const char data_2D432[10] __attribute__((scn_string_literal)) = "E1032P036";
static const char data_2D43C[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_2D446[10] __attribute__((scn_string_literal)) = "E1032P021";
static const char data_2D450[10] __attribute__((scn_string_literal)) = "E1032A152";
static const char data_2D45A[10] __attribute__((scn_string_literal)) = "E1032P025";
static const char data_2D464[10] __attribute__((scn_string_literal)) = "E1032A160";
static const char data_2D46E[10] __attribute__((scn_string_literal)) = "E1032P033";
static const char data_2D478[10] __attribute__((scn_string_literal)) = "E1032P082";
static const char data_2D482[10] __attribute__((scn_string_literal)) = "E1032P083";
static const char data_2D48C[10] __attribute__((scn_string_literal)) = "E1032P084";
static const char data_2D496[10] __attribute__((scn_string_literal)) = "E1032P085";
static const char data_2D4A0[10] __attribute__((scn_string_literal)) = "E1032A068";
static const char data_2D4AA[10] __attribute__((scn_string_literal)) = "E1032P086";
static const char data_2D4B4[10] __attribute__((scn_string_literal)) = "E1032P001";
static const char data_2D4BE[10] __attribute__((scn_string_literal)) = "E1032A164";
static const char data_2D4C8[10] __attribute__((scn_string_literal)) = "E1032P018";
static const char data_2D4D2[10] __attribute__((scn_string_literal)) = "E1032A165";
static const char data_2D4DC[10] __attribute__((scn_string_literal)) = "E1032P037";
static const char data_2D4E6[10] __attribute__((scn_string_literal)) = "E1032A104";
static const char data_2D4F0[10] __attribute__((scn_string_literal)) = "E1032P120";
static const char data_2D4FA[10] __attribute__((scn_string_literal)) = "E1032A173";
static const char data_2D504[10] __attribute__((scn_string_literal)) = "E1032O008";
static const char data_2D50E[10] __attribute__((scn_string_literal)) = "E1032P008";
static const char data_2D518[10] __attribute__((scn_string_literal)) = "E1032A154";
static const char data_2D522[10] __attribute__((scn_string_literal)) = "E1032P027";
static const char data_2D52C[10] __attribute__((scn_string_literal)) = "E1032A068";
static const char data_2D536[10] __attribute__((scn_string_literal)) = "E1032P017";
static const char data_2D540[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_2D54A[10] __attribute__((scn_string_literal)) = "E1032P001";
static const char data_2D554[10] __attribute__((scn_string_literal)) = "E1032A164";
static const char data_2D55E[10] __attribute__((scn_string_literal)) = "E1032P018";
static const char data_2D568[10] __attribute__((scn_string_literal)) = "E1032A170";
static const char data_2D572[10] __attribute__((scn_string_literal)) = "E1032P044";
static const char data_2D57C[10] __attribute__((scn_string_literal)) = "E1032A200";
static const char data_2D586[10] __attribute__((scn_string_literal)) = "E1032A172";
static const char data_2D590[10] __attribute__((scn_string_literal)) = "E1032P045";
static const char data_2D59A[10] __attribute__((scn_string_literal)) = "E1032A173";
static const char data_2D5A4[10] __attribute__((scn_string_literal)) = "E1032P046";
static const char data_2D5AE[10] __attribute__((scn_string_literal)) = "E1032P047";
static const char data_2D5B8[10] __attribute__((scn_string_literal)) = "E1032A201";
static const char data_2D5C2[10] __attribute__((scn_string_literal)) = "E1032P049";
static const char data_2D5CC[10] __attribute__((scn_string_literal)) = "E1032A176";
static const char data_2D5D6[10] __attribute__((scn_string_literal)) = "E1032P050";
static const char data_2D5E0[10] __attribute__((scn_string_literal)) = "E1032A177";
static const char data_2D5EA[10] __attribute__((scn_string_literal)) = "E1032P051";
static const char data_2D5F4[10] __attribute__((scn_string_literal)) = "E1032P001";
static const char data_2D5FE[10] __attribute__((scn_string_literal)) = "E1032A164";
static const char data_2D608[10] __attribute__((scn_string_literal)) = "E1032P018";
static const char data_2D612[10] __attribute__((scn_string_literal)) = "E1032A178";
static const char data_2D61C[10] __attribute__((scn_string_literal)) = "E1032P004";
static const char data_2D626[10] __attribute__((scn_string_literal)) = "E1032P052";
static const char data_2D630[10] __attribute__((scn_string_literal)) = "E1032A179";
static const char data_2D63A[10] __attribute__((scn_string_literal)) = "E1032A180";
static const char data_2D644[10] __attribute__((scn_string_literal)) = "E1032A181";
static const char data_2D64E[10] __attribute__((scn_string_literal)) = "E1032P053";
static const char data_2D658[10] __attribute__((scn_string_literal)) = "E1032P087";
static const char data_2D662[10] __attribute__((scn_string_literal)) = "E1032P055";
static const char data_2D66C[10] __attribute__((scn_string_literal)) = "E1032A182";
static const char data_2D676[10] __attribute__((scn_string_literal)) = "E1032P051";
static const char data_2D680[10] __attribute__((scn_string_literal)) = "E1032P001";
static const char data_2D68A[10] __attribute__((scn_string_literal)) = "E1032A164";
static const char data_2D694[10] __attribute__((scn_string_literal)) = "E1032P018";
static const char data_2D69E[10] __attribute__((scn_string_literal)) = "E1032A183";
static const char data_2D6A8[10] __attribute__((scn_string_literal)) = "E1032P038";
static const char data_2D6B2[10] __attribute__((scn_string_literal)) = "E1032A184";
static const char data_2D6BC[10] __attribute__((scn_string_literal)) = "E1032P056";
static const char data_2D6C6[10] __attribute__((scn_string_literal)) = "E1032P088";
static const char data_2D6D0[10] __attribute__((scn_string_literal)) = "E1032P058";
static const char data_2D6DA[10] __attribute__((scn_string_literal)) = "E1032A220";
static const char data_2D6E4[10] __attribute__((scn_string_literal)) = "E1032A185";
static const char data_2D6EE[10] __attribute__((scn_string_literal)) = "E1032P059";
static const char data_2D6F8[10] __attribute__((scn_string_literal)) = "E1032P001";
static const char data_2D702[10] __attribute__((scn_string_literal)) = "E1032A164";
static const char data_2D70C[10] __attribute__((scn_string_literal)) = "E1032P018";
static const char data_2D716[10] __attribute__((scn_string_literal)) = "E1032P060";
static const char data_2D720[10] __attribute__((scn_string_literal)) = "E1032A186";
static const char data_2D72A[10] __attribute__((scn_string_literal)) = "E1032P061";
static const char data_2D734[10] __attribute__((scn_string_literal)) = "E1032A187";
static const char data_2D73E[10] __attribute__((scn_string_literal)) = "E1032P062";
static const char data_2D748[10] __attribute__((scn_string_literal)) = "E1032A188";
static const char data_2D752[10] __attribute__((scn_string_literal)) = "E1032P063";
static const char data_2D75C[10] __attribute__((scn_string_literal)) = "E1032P064";
static const char data_2D766[10] __attribute__((scn_string_literal)) = "E1032P065";
static const char data_2D770[10] __attribute__((scn_string_literal)) = "E1032A189";
static const char data_2D77A[10] __attribute__((scn_string_literal)) = "E1032P066";
static const char data_2D784[10] __attribute__((scn_string_literal)) = "E1032A190";
static const char data_2D78E[10] __attribute__((scn_string_literal)) = "E1032P001";
static const char data_2D798[10] __attribute__((scn_string_literal)) = "E1032A152";
static const char data_2D7A2[10] __attribute__((scn_string_literal)) = "E1032P067";
static const char data_2D7AC[10] __attribute__((scn_string_literal)) = "E1032A191";
static const char data_2D7B6[10] __attribute__((scn_string_literal)) = "E1032A192";
static const char data_2D7C0[10] __attribute__((scn_string_literal)) = "E1032P068";
static const char data_2D7CA[10] __attribute__((scn_string_literal)) = "E1032P069";
static const char data_2D7D4[10] __attribute__((scn_string_literal)) = "E1032A193";
static const char data_2D7DE[10] __attribute__((scn_string_literal)) = "E1032P001";
static const char data_2D7E8[10] __attribute__((scn_string_literal)) = "E1032A152";
static const char data_2D7F2[10] __attribute__((scn_string_literal)) = "E1032P070";
static const char data_2D7FC[10] __attribute__((scn_string_literal)) = "E1032A194";
static const char data_2D806[10] __attribute__((scn_string_literal)) = "E1032P071";
static const char data_2D810[10] __attribute__((scn_string_literal)) = "E1032A195";
static const char data_2D81A[10] __attribute__((scn_string_literal)) = "E1032P072";
static const char data_2D824[10] __attribute__((scn_string_literal)) = "E1032P073";
static const char data_2D82E[10] __attribute__((scn_string_literal)) = "E1032A151";
static const char data_2D838[10] __attribute__((scn_string_literal)) = "E1032P074";
static const char data_2D842[10] __attribute__((scn_string_literal)) = "E1032P075";
static const char data_2D84C[10] __attribute__((scn_string_literal)) = "E1032P089";
static const char data_2D856[10] __attribute__((scn_string_literal)) = "E1032P090";
static const char data_2D860[10] __attribute__((scn_string_literal)) = "E1032A196";
static const char data_2D86A[10] __attribute__((scn_string_literal)) = "E1032P079";
static const char data_2D874[10] __attribute__((scn_string_literal)) = "E1032A197";
static const char data_2D87E[10] __attribute__((scn_string_literal)) = "E1032P001";
static const char data_2D888[10] __attribute__((scn_string_literal)) = "E1032A152";
static const char data_2D892[10] __attribute__((scn_string_literal)) = "E1032P010";
static const char data_2D89C[10] __attribute__((scn_string_literal)) = "E1032P091";
static const char data_2D8A6[10] __attribute__((scn_string_literal)) = "E1032P092";
static const char data_2D8B0[10] __attribute__((scn_string_literal)) = "E1032A203";
static const char data_2D8BA[10] __attribute__((scn_string_literal)) = "E1032P093";
static const char data_2D8C4[10] __attribute__((scn_string_literal)) = "E1032P094";
static const char data_2D8CE[10] __attribute__((scn_string_literal)) = "E1032A204";
static const char data_2D8D8[10] __attribute__((scn_string_literal)) = "E1032P095";
static const char data_2D8E2[10] __attribute__((scn_string_literal)) = "E1032P021";
static const char data_2D8EC[10] __attribute__((scn_string_literal)) = "E1032A152";
static const char data_2D8F6[10] __attribute__((scn_string_literal)) = "E1032P096";
static const char data_2D900[10] __attribute__((scn_string_literal)) = "E1032A104";
static const char data_2D90A[10] __attribute__((scn_string_literal)) = "E1032P097";
static const char data_2D914[10] __attribute__((scn_string_literal)) = "E1032A103";
static const char data_2D91E[10] __attribute__((scn_string_literal)) = "E1032P098";
static const char data_2D928[10] __attribute__((scn_string_literal)) = "E1032P099";
static const char data_2D932[10] __attribute__((scn_string_literal)) = "E1032P100";
static const char data_2D93C[10] __attribute__((scn_string_literal)) = "E1032A206";
static const char data_2D946[10] __attribute__((scn_string_literal)) = "E1032P021";
static const char data_2D950[10] __attribute__((scn_string_literal)) = "E1032A152";
static const char data_2D95A[10] __attribute__((scn_string_literal)) = "E1032P101";
static const char data_2D964[10] __attribute__((scn_string_literal)) = "E1032A207";
static const char data_2D96E[10] __attribute__((scn_string_literal)) = "E1032P102";
static const char data_2D978[10] __attribute__((scn_string_literal)) = "E1032A208";
static const char data_2D982[10] __attribute__((scn_string_literal)) = "E1032P103";
static const char data_2D98C[10] __attribute__((scn_string_literal)) = "E1032A209";
static const char data_2D996[10] __attribute__((scn_string_literal)) = "E1032P104";
static const char data_2D9A0[10] __attribute__((scn_string_literal)) = "E1032P105";
static const char data_2D9AA[10] __attribute__((scn_string_literal)) = "E1032A210";
static const char data_2D9B4[10] __attribute__((scn_string_literal)) = "E1032P106";
static const char data_2D9BE[10] __attribute__((scn_string_literal)) = "E1032A154";
static const char data_2D9C8[10] __attribute__((scn_string_literal)) = "E1032P107";
static const char data_2D9D2[10] __attribute__((scn_string_literal)) = "E1032A204";
static const char data_2D9DC[10] __attribute__((scn_string_literal)) = "E1032P108";
static const char data_2D9E6[10] __attribute__((scn_string_literal)) = "E1032P109";
static const char data_2D9F0[10] __attribute__((scn_string_literal)) = "E1032A085";
static const char data_2D9FA[10] __attribute__((scn_string_literal)) = "E1032P110";
static const char data_2DA04[10] __attribute__((scn_string_literal)) = "E1032A211";
static const char data_2DA0E[10] __attribute__((scn_string_literal)) = "E1032P111";
static const char data_2DA18[10] __attribute__((scn_string_literal)) = "E1032A104";
static const char data_2DA22[10] __attribute__((scn_string_literal)) = "E1032P112";
static const char data_2DA2C[10] __attribute__((scn_string_literal)) = "E1032A212";
static const char data_2DA36[10] __attribute__((scn_string_literal)) = "E1032P113";
static const char data_2DA40[10] __attribute__((scn_string_literal)) = "E1032A158";
static const char data_2DA4A[10] __attribute__((scn_string_literal)) = "E1032P114";
static const char data_2DA54[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_2DA5E[10] __attribute__((scn_string_literal)) = "E1032P115";
static const char data_2DA68[10] __attribute__((scn_string_literal)) = "E1032A213";
static const char data_2DA72[10] __attribute__((scn_string_literal)) = "E1032P116";
static const char data_2DA7C[10] __attribute__((scn_string_literal)) = "E1032P001";
static const char data_2DA86[10] __attribute__((scn_string_literal)) = "E1032A152";
static const char data_2DA90[10] __attribute__((scn_string_literal)) = "E1032P117";
static const char data_2DA9A[10] __attribute__((scn_string_literal)) = "E1032A214";
static const char data_2DAA4[10] __attribute__((scn_string_literal)) = "E1032P118";
static const char data_2DAAE[10] __attribute__((scn_string_literal)) = "E1032A154";
static const char data_2DAB8[10] __attribute__((scn_string_literal)) = "E1032P119";
static const char data_2DAC2[10] __attribute__((scn_string_literal)) = "E1032P120";
static const char data_2DACC[10] __attribute__((scn_string_literal)) = "E1032A215";
static const char data_2DAD6[10] __attribute__((scn_string_literal)) = "E1032P121";
static const char data_2DAE0[10] __attribute__((scn_string_literal)) = "E1032A163";
static const char data_2DAEA[10] __attribute__((scn_string_literal)) = "E1032P123";
static const char data_2DAF4[10] __attribute__((scn_string_literal)) = "E1032A217";
static const char data_2DAFE[10] __attribute__((scn_string_literal)) = "E1032P001";
static const char data_2DB08[10] __attribute__((scn_string_literal)) = "E1032A152";
static const char data_2DB12[10] __attribute__((scn_string_literal)) = "E1032P010";
static const char data_2DB1C[10] __attribute__((scn_string_literal)) = "E1032P124";
static const char data_2DB26[10] __attribute__((scn_string_literal)) = "E1032P125";
static const char data_2DB30[10] __attribute__((scn_string_literal)) = "E1032A218";
static const char data_2DB3A[10] __attribute__((scn_string_literal)) = "E1032P126";
static const char data_2DB44[10] __attribute__((scn_string_literal)) = "E1032A154";
static const char data_2DB4E[10] __attribute__((scn_string_literal)) = "E1032P127";
static const char data_2DB58[10] __attribute__((scn_string_literal)) = "E1032A213";
static const char data_2DB62[10] __attribute__((scn_string_literal)) = "E1032P128";
static const char data_2DB6C[10] __attribute__((scn_string_literal)) = "E1032A204";
static const char data_2DB76[10] __attribute__((scn_string_literal)) = "E1032P116";
static const char data_2DB80[10] __attribute__((scn_string_literal)) = "E1032P109";
static const char data_2DB8A[10] __attribute__((scn_string_literal)) = "E1032A085";
static const char data_2DB94[10] __attribute__((scn_string_literal)) = "E1032P117";
static const char data_2DB9E[10] __attribute__((scn_string_literal)) = "E1032A214";
static const char data_2DBA8[10] __attribute__((scn_string_literal)) = "E1032P129";
static const char data_2DBB2[10] __attribute__((scn_string_literal)) = "E1032P130";
static const char data_2DBBC[10] __attribute__((scn_string_literal)) = "E1032P131";
static const char data_2DBC6[10] __attribute__((scn_string_literal)) = "E1032A213";
static const char data_2DBD0[10] __attribute__((scn_string_literal)) = "E1032A219";
static const char data_2DBDA[10] __attribute__((scn_string_literal)) = "E1032P132";
static const char data_2DBE4[10] __attribute__((scn_string_literal)) = "E1032A153";
static const char data_2DBEE[10] __attribute__((scn_string_literal)) = "E1032P133";
static const char data_2DBF8[10] __attribute__((scn_string_literal)) = "E1032A220";
static const char data_2DC02[10] __attribute__((scn_string_literal)) = "E1032P134";
static const char data_2DC0C[10] __attribute__((scn_string_literal)) = "E1032A204";
static const char data_2DC16[10] __attribute__((scn_string_literal)) = "E1032P116";
static const char data_2DC20[10] __attribute__((scn_string_literal)) = "E1032A085";
static const char data_2DC2A[10] __attribute__((scn_string_literal)) = "E1032P010";
static const char data_2DC34[10] __attribute__((scn_string_literal)) = "E1032A221";
static const char data_2DC3E[10] __attribute__((scn_string_literal)) = "E1032P135";
static const char data_2DC48[10] __attribute__((scn_string_literal)) = "E1032P136";
static const char data_2DC52[10] __attribute__((scn_string_literal)) = "E1032A222";
static const char data_2DC5C[10] __attribute__((scn_string_literal)) = "E1032P137";
static const char data_2DC66[10] __attribute__((scn_string_literal)) = "E1032P138";
static const char data_2DC70[10] __attribute__((scn_string_literal)) = "E1032A223";
static const char data_2DC7A[10] __attribute__((scn_string_literal)) = "E1032P139";
static const char data_2DC84[10] __attribute__((scn_string_literal)) = "E1032A224";
static const char data_2DC8E[10] __attribute__((scn_string_literal)) = "E1032P140";
static const char data_2DC98[10] __attribute__((scn_string_literal)) = "E1032A225";
static const char data_2DCA2[10] __attribute__((scn_string_literal)) = "E1032A226";
static const char data_2DCAC[10] __attribute__((scn_string_literal)) = "E1032P004";
static const char data_2DCB6[10] __attribute__((scn_string_literal)) = "E1032E001";
static const char data_2DCC0[10] __attribute__((scn_string_literal)) = "E1032E002";
static const char data_2DCCA[10] __attribute__((scn_string_literal)) = "E1032E003";
static const char data_2DCD4[10] __attribute__((scn_string_literal)) = "E1032A010";
static const char data_2DCDE[10] __attribute__((scn_string_literal)) = "E1032E004";
static const char data_2DCE8[10] __attribute__((scn_string_literal)) = "E1032A011";
static const char data_2DCF2[10] __attribute__((scn_string_literal)) = "E1032E005";
static const char data_2DCFC[10] __attribute__((scn_string_literal)) = "E1032A012";
static const char data_2DD06[10] __attribute__((scn_string_literal)) = "E1032A013";
static const char data_2DD10[10] __attribute__((scn_string_literal)) = "E1032E006";
static const char data_2DD1A[10] __attribute__((scn_string_literal)) = "E1032A014";
static const char data_2DD24[10] __attribute__((scn_string_literal)) = "E1032A015";
static const char data_2DD2E[10] __attribute__((scn_string_literal)) = "E1032E007";
static const char data_2DD38[10] __attribute__((scn_string_literal)) = "E1032A016";
static const char data_2DD42[10] __attribute__((scn_string_literal)) = "E1032E008";
static const char data_2DD4C[10] __attribute__((scn_string_literal)) = "E1032E009";
static const char data_2DD56[10] __attribute__((scn_string_literal)) = "E1032A017";
static const char data_2DD60[10] __attribute__((scn_string_literal)) = "E1032E010";
static const char data_2DD6A[10] __attribute__((scn_string_literal)) = "E1032E011";
static const char data_2DD74[10] __attribute__((scn_string_literal)) = "E1032E012";
static const char data_2DD7E[10] __attribute__((scn_string_literal)) = "E1032E013";
static const char data_2DD88[10] __attribute__((scn_string_literal)) = "E1032A018";
static const char data_2DD92[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_2DD9C[10] __attribute__((scn_string_literal)) = "E1032A020";
static const char data_2DDA6[10] __attribute__((scn_string_literal)) = "E1032A021";
static const char data_2DDB0[10] __attribute__((scn_string_literal)) = "E1032F001";
static const char data_2DDBA[10] __attribute__((scn_string_literal)) = "E1032F002";
static const char data_2DDC4[10] __attribute__((scn_string_literal)) = "E1032F003";
static const char data_2DDCE[10] __attribute__((scn_string_literal)) = "E1032A022";
static const char data_2DDD8[10] __attribute__((scn_string_literal)) = "E1032F004";
static const char data_2DDE2[10] __attribute__((scn_string_literal)) = "E1032A011";
static const char data_2DDEC[10] __attribute__((scn_string_literal)) = "E1032F005";
static const char data_2DDF6[10] __attribute__((scn_string_literal)) = "E1032A012";
static const char data_2DE00[10] __attribute__((scn_string_literal)) = "E1032A013";
static const char data_2DE0A[10] __attribute__((scn_string_literal)) = "E1032F006";
static const char data_2DE14[10] __attribute__((scn_string_literal)) = "E1032A023";
static const char data_2DE1E[10] __attribute__((scn_string_literal)) = "E1032A024";
static const char data_2DE28[10] __attribute__((scn_string_literal)) = "E1032F007";
static const char data_2DE32[10] __attribute__((scn_string_literal)) = "E1032A025";
static const char data_2DE3C[10] __attribute__((scn_string_literal)) = "E1032A026";
static const char data_2DE46[10] __attribute__((scn_string_literal)) = "E1032F008";
static const char data_2DE50[10] __attribute__((scn_string_literal)) = "E1032F009";
static const char data_2DE5A[10] __attribute__((scn_string_literal)) = "E1032F010";
static const char data_2DE64[10] __attribute__((scn_string_literal)) = "E1032F011";
static const char data_2DE6E[10] __attribute__((scn_string_literal)) = "E1032F012";
static const char data_2DE78[10] __attribute__((scn_string_literal)) = "E1032F013";
static const char data_2DE82[10] __attribute__((scn_string_literal)) = "E1032F014";
static const char data_2DE8C[10] __attribute__((scn_string_literal)) = "E1032F015";
static const char data_2DE96[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_2DEA0[10] __attribute__((scn_string_literal)) = "E1032A018";
static const char data_2DEAA[10] __attribute__((scn_string_literal)) = "E1032A027";
static const char data_2DEB4[10] __attribute__((scn_string_literal)) = "E1032A034";
static const char data_2DEBE[10] __attribute__((scn_string_literal)) = "E1032H005";
static const char data_2DEC8[10] __attribute__((scn_string_literal)) = "E1032H004";
static const char data_2DED2[10] __attribute__((scn_string_literal)) = "E1032H009";
static const char data_2DEDC[10] __attribute__((scn_string_literal)) = "E1032H006";
static const char data_2DEE6[10] __attribute__((scn_string_literal)) = "E1032A042";
static const char data_2DEF0[10] __attribute__((scn_string_literal)) = "E1032A040";
static const char data_2DEFA[10] __attribute__((scn_string_literal)) = "E1032A043";
static const char data_2DF04[10] __attribute__((scn_string_literal)) = "E1032A041";
static const char data_2DF0E[10] __attribute__((scn_string_literal)) = "E1032A045";
static const char data_2DF18[10] __attribute__((scn_string_literal)) = "E1032A046";
static const char data_2DF22[10] __attribute__((scn_string_literal)) = "E1032A047";
static const char data_2DF2C[10] __attribute__((scn_string_literal)) = "E1032A046";
static const char data_2DF36[10] __attribute__((scn_string_literal)) = "E1032A047";
static const char data_2DF40[10] __attribute__((scn_string_literal)) = "E1032A044";
static const char data_2DF4A[10] __attribute__((scn_string_literal)) = "E1032A051";
static const char data_2DF54[10] __attribute__((scn_string_literal)) = "E1032A039";
static const char data_2DF5E[10] __attribute__((scn_string_literal)) = "E1032H007";
static const char data_2DF68[10] __attribute__((scn_string_literal)) = "E1032A120";
static const char data_2DF72[10] __attribute__((scn_string_literal)) = "E1032H010";
static const char data_2DF7C[10] __attribute__((scn_string_literal)) = "E1032A066";
static const char data_2DF86[10] __attribute__((scn_string_literal)) = "E1032H011";
static const char data_2DF90[10] __attribute__((scn_string_literal)) = "E1032J001";
static const char data_2DF9A[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_2DFA4[10] __attribute__((scn_string_literal)) = "E1032A131";
static const char data_2DFAE[10] __attribute__((scn_string_literal)) = "E1032J010";
static const char data_2DFB8[10] __attribute__((scn_string_literal)) = "E1032J013";
static const char data_2DFC2[10] __attribute__((scn_string_literal)) = "E1032J001";
static const char data_2DFCC[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_2DFD6[10] __attribute__((scn_string_literal)) = "E1032A134";
static const char data_2DFE0[10] __attribute__((scn_string_literal)) = "E1032J015";
static const char data_2DFEA[10] __attribute__((scn_string_literal)) = "E1032A135";
static const char data_2DFF4[10] __attribute__((scn_string_literal)) = "E1032J001";
static const char data_2DFFE[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_2E008[10] __attribute__((scn_string_literal)) = "E1032J018";
static const char data_2E012[10] __attribute__((scn_string_literal)) = "E1032A163";
static const char data_2E01C[10] __attribute__((scn_string_literal)) = "E1032J019";
static const char data_2E026[10] __attribute__((scn_string_literal)) = "E1032J020";
static const char data_2E030[10] __attribute__((scn_string_literal)) = "E1032A068";
static const char data_2E03A[10] __attribute__((scn_string_literal)) = "E1032J016";
static const char data_2E044[10] __attribute__((scn_string_literal)) = "E1032A161";
static const char data_2E04E[10] __attribute__((scn_string_literal)) = "E1032J017";
static const char data_2E058[10] __attribute__((scn_string_literal)) = "E1032A162";
static const char data_2E062[10] __attribute__((scn_string_literal)) = "E1032J001";
static const char data_2E06C[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_2E076[10] __attribute__((scn_string_literal)) = "E1032A131";
static const char data_2E080[10] __attribute__((scn_string_literal)) = "E1032J010";
static const char data_2E08A[10] __attribute__((scn_string_literal)) = "E1032A132";
static const char data_2E094[10] __attribute__((scn_string_literal)) = "E1032J011";
static const char data_2E09E[10] __attribute__((scn_string_literal)) = "E1032J012";
static const char data_2E0A8[10] __attribute__((scn_string_literal)) = "E1032H007";
static const char data_2E0B2[10] __attribute__((scn_string_literal)) = "E1032A120";
static const char data_2E0BC[10] __attribute__((scn_string_literal)) = "E1032H010";
static const char data_2E0C6[10] __attribute__((scn_string_literal)) = "E1032A066";
static const char data_2E0D0[10] __attribute__((scn_string_literal)) = "E1032H011";
static const char data_2E0DA[10] __attribute__((scn_string_literal)) = "E1032J001";
static const char data_2E0E4[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_2E0EE[10] __attribute__((scn_string_literal)) = "E1032J018";
static const char data_2E0F8[10] __attribute__((scn_string_literal)) = "E1032A163";
static const char data_2E102[10] __attribute__((scn_string_literal)) = "E1032J019";
static const char data_2E10C[10] __attribute__((scn_string_literal)) = "E1032J020";
static const char data_2E116[10] __attribute__((scn_string_literal)) = "E1032A068";
static const char data_2E120[10] __attribute__((scn_string_literal)) = "E1032J016";
static const char data_2E12A[10] __attribute__((scn_string_literal)) = "E1032A161";
static const char data_2E134[10] __attribute__((scn_string_literal)) = "E1032J017";
static const char data_2E13E[10] __attribute__((scn_string_literal)) = "E1032A162";
static const char data_2E148[10] __attribute__((scn_string_literal)) = "E1032J001";
static const char data_2E152[10] __attribute__((scn_string_literal)) = "E1032A067";
static const char data_2E15C[10] __attribute__((scn_string_literal)) = "E1032J002";
static const char data_2E166[10] __attribute__((scn_string_literal)) = "E1032A068";
static const char data_2E170[10] __attribute__((scn_string_literal)) = "E1032J001";
static const char data_2E17A[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_2E184[10] __attribute__((scn_string_literal)) = "E1032J003";
static const char data_2E18E[10] __attribute__((scn_string_literal)) = "E1032A069";
static const char data_2E198[10] __attribute__((scn_string_literal)) = "E1032J004";
static const char data_2E1A2[10] __attribute__((scn_string_literal)) = "E1032A070";
static const char data_2E1AC[10] __attribute__((scn_string_literal)) = "E1032H007";
static const char data_2E1B6[10] __attribute__((scn_string_literal)) = "E1032A048";
static const char data_2E1C0[10] __attribute__((scn_string_literal)) = "E1032H008";
static const char data_2E1CA[10] __attribute__((scn_string_literal)) = "E1032A049";
static const char data_2E1D4[10] __attribute__((scn_string_literal)) = "E1032A050";
static const char data_2E1DE[16] __attribute__((scn_string_literal)) = "/scene/02/JD00/";
static const char data_2E1EE[5] __attribute__((scn_string_literal)) = "TBOX";
static const char data_2E1F3[5] __attribute__((scn_string_literal)) = "TBOX";
static const uint32_t data_2E1F8 = 0xC18DE1B1;
static const float data_2E1FC[2] = {0.225f, -0.0935f};
static const int32_t data_2E204[3] = {63169, 1048832, 6};
static const float data_2E210[3] = {-17.4023f, 1.574f, -1.5519f};
static const int32_t data_2E21C[3] = {30401, 264, 6};
static const float data_2E228[3] = {-10.4327f, 0.225f, 21.8722f};
static const int32_t data_2E234[3] = {16384, 1048832, 5};
static const float data_2E240[2] = {-11.6312f, 1.1495f};
static const uint32_t data_2E248 = 0x41AF1FBEU;
static const int32_t data_2E24C[3] = {49152, 264, 5};
static const float data_2E258[3] = {-14.4036f, 0.225f, 7.0609f};
static const int32_t data_2E264[3] = {16384, 1048576, 5};
static const float data_2E270[3] = {-15.5966f, 1.0f, 7.0721f};
static const int32_t data_2E27C[3] = {49152, 8, 5};
static const uint32_t data_2E288 = 0xC1C4F247U;
static const float data_2E28C[2] = {1.2587f, 6.2984f};
static const int32_t data_2E294[3] = {32768, 1048576, 8};
static const uint32_t data_2E2A0 = 0xC1C4E944U;
static const float data_2E2A4[3] = {3.0927f, 8.4006f, 0.0f};
static const int32_t data_2E2B0[2] = {8, 8};
static const float data_2E2B8[3] = {14.462f, 0.2094f, 28.3062f};
static const int32_t data_2E2C4[3] = {64261, 1048576, 3};
static const float data_2E2D0[3] = {14.6576f, 0.6583f, 27.645f};
static const int32_t data_2E2DC[3] = {31493, 8, 3};
static const float data_2E2E8[2] = {-13.8104f, 1.3233f};
static const uint32_t data_2E2F0 = 0x41C3D8AEU;
static const int32_t data_2E2F4[3] = {30219, 1064960, 12};
static const float data_2E300[2] = {-14.6088f, 3.8117f};
static const uint32_t data_2E308 = 0x41DD73EBU;
static const int32_t data_2E30C[3] = {62987, 16392, 12};
static const float data_2E318[2] = {-13.0535f, 1.3156f};
static const uint32_t data_2E320 = 0x41F68D84U;
static const int32_t data_2E324[3] = {32768, 1064960, 12};
static const float data_2E330[4] = {-13.0292f, 3.8062f, 34.1213f, 0.0f};
static const int32_t data_2E340[2] = {16392, 12};
static const float data_2E348 = 100000.0f;
static const uint8_t data_2E34C[8] = {0};
typedef struct Row_2E354 { float w0; int32_t w1; int32_t w2; } Row_2E354;
static const Row_2E354 data_2E354[3] = {
  {0.5f, 0, 1},
  {0.5f, 0, 2},
  {0.4f, 0, 3}
};
static const float data_2E378 = 0.4f;
static const int32_t data_2E37C = 3;
static const char data_2E380[16] __attribute__((scn_string_literal)) = "/scene/02/JD00/";
static const char data_2E390[5] __attribute__((scn_string_literal)) = "MAPY";
static const char data_2E395[16] __attribute__((scn_string_literal)) = "/scene/02/JD00/";
static const char data_2E3A5[5] __attribute__((scn_string_literal)) = "MAPN";
static const char data_2E3AA[16] __attribute__((scn_string_literal)) = "/scene/02/JD00/";
static const char data_2E3BA[5] __attribute__((scn_string_literal)) = "MAPS";
static const char data_2E3BF[16] __attribute__((scn_string_literal)) = "/scene/02/JD00/";
static const char data_2E3CF[5] __attribute__((scn_string_literal)) = "JDYA";
static const char data_2E3D4[5] __attribute__((scn_string_literal)) = "JDYA";
static const char data_2E3D9[1] __attribute__((scn_string_literal)) = "";
static const char data_2E3DA[5] __attribute__((scn_string_literal)) = "JDYA";
static const char data_2E3DF[1] __attribute__((scn_string_literal)) = "";
static const char data_2E3E0[8] __attribute__((scn_string_literal)) = "JDYA";
static void (*data_2E3E8)(void) = script_22188;
static void (*const data_2E3EC)(int32_t) = script_2218B;
static void (*const data_2E3F0)(int32_t) = script_221AE;
static void (*const data_2E3F4)(int32_t) = script_221D1;
static const Vector4 data_2E3F8 = {.x = 1.0f, .y = 0.4f, .z = 0.4f, .w = 0.4f};
static const Vector4 data_2E408 = {.x = 1.0f, .y = 0.6f, .z = 0.6f, .w = 0.6f};
static const char data_2E418[16] __attribute__((scn_string_literal)) = "/scene/02/JD00/";
static const char data_2E428[5] __attribute__((scn_string_literal)) = "DNOZ";
static const char data_2E42D[5] __attribute__((scn_string_literal)) = "DNOZ";
static const char data_2E432[1] __attribute__((scn_string_literal)) = "";
static const char data_2E433[5] __attribute__((scn_string_literal)) = "DNOZ";
static const char data_2E438[1] __attribute__((scn_string_literal)) = "";
static const char data_2E439[7] __attribute__((scn_string_literal)) = "01FULB";
static const char data_2E440[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_2E445[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_2E44A[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_2E44F[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_2E454[11] __attribute__((scn_string_literal)) = "bgm068.snd";
static const char data_2E45F[13] __attribute__((scn_string_literal)) = "a1_nonam.snd";
static const FogColourData data_2E46C = {.red = 16.7f, .green = 780.0f, .blue = 1.0f, .colour = 0xFF0D0705U};
static const uint32_t data_2E47C[4] = {0x320E102EU, 0x320E102EU, 0x64555462U, 0x64555462U};
static const Vector4 data_2E48C = {.x = 0.25f, .y = 0.09f, .z = 0.19f, .w = 0.31f};
static const Vector4 data_2E49C = {.x = 1.0f, .y = 0.8f, .z = 0.63f, .w = 0.4f};
static const float data_2E4AC[2] = {0.69f, 1.0f};
static const Vector4 data_2E4B4 = {.x = 1.0f, .y = 0.4f, .z = 0.4f, .w = 0.4f};
static const Vector4 data_2E4C4 = {.x = 1.0f, .y = 0.6f, .z = 0.6f, .w = 0.6f};
static const char data_2E4D4[] __attribute__((scn_string_literal)) = "sprite";
static const char data_2E4DB[241] __attribute__((scn_string_literal)) = "SCROLL27.SPR";
static const int32_t data_2E5CC[228] = {
  0, 0, -3640, 0, 0, -2366, -4369, 1820, -546, 0, 0, -5279, 0, 0, -6007, 0, 0, -2912, 0, 0, -910, 0, 0, -6553, 0, 0, -6735, 0, 0, -3640, 0, 0, -182,
  0, 0, -5461, 0, 0, -6007, 0, 0, -2366, 0, 0, 0, 0, 0, -5643, 0, 0, -4733, 0, 0, -2366, -1456, 0, 728, 0, 0, -9466, 0, -546, -2912, 0, 3094, -2366,
  0, 0, -18386, 0, 0, -19296, 0, 1092, -14381, 0, 0, 0, 0, 0, -16748, 0, 0, -17658, 0, 0, -15109, 0, 0, 0, 0, 0, -16566, 0, 0, -18204, 0, 0, -14381,
  0, 0, 0, 0, 0, -19478, 0, 0, -18386, 0, -4004, -12925, 0, 0, -546, 0, 0, -3276, 0, 0, 0, 0, 0, 0, 0, 0, -6553, 0, 0, -4369, 0, -546, -3458, 0, 0, 0,
  0, 0, -5461, 0, 0, -5643, 0, 0, -3458, 0, 0, 0, 0, 0, -6007, 0, 0, -4551, 0, 546, -2548, 0, 0, 0, 0, 0, -6189, 0, -1092, -3094, 0, 0, -2548, 0, 0,
  0, 0, 0, -6007, 0, 0, -728, 0, 0, 0, 0, 0, -5097, 0, 0, -2184, 0, 0, -2002, 0, 0, 0, 0, 0, -5825, 0, 0, -2912, 0, 0, -728, 0, 0, 0, 0, 0, -4915, 0,
  0, -4187, 0, 0, -1274, 0, 0, 0, 0, 0, -6553, 0, -364, -4369, 0, -546, -1456, 0, 0, 0
};
static const int32_t data_2E95C[57] = {
  0, 0, 0, 0, 0, 0, 0, 3458, 0, 0, 0, 0, 0, 0, -2912, 0, 0, 0, 0, 0, 1456, 0, 0, 0, 0, 0, -2184, 0, 0, 0, 0, 0, 1092, 0, 0, 0, 0, 0, -1820, 0, 0, 0,
  0, 0, 910, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};
static const int32_t data_2EA40[57] = {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1638, 0, 0, -2548, 0, 0, 1092, 0, 0, 0, 0, 0, -2002, 0, 0, -910, 0, 0, 546, 0, 0, 0, 0, 0, -1820, 0, 0,
  -728, 0, 0, 546, 0, 0, 0, 0, 0, -1820, 0, 0, -728, 0, 0, 364
};
static const Vector4 data_2EB24 = {.x = 0.3f, .y = 0.3f, .z = 0.3f, .w = 0.3f};
static const Vector3 data_2EB34 = {.x = 0.153f, .y = 0.0016f, .z = -0.023f};
static const Vector3i data_2EB40 = {.x = 49152, .y = 0, .z = 0};
static const Vector3 data_2EB4C = {.x = 0.14f, .y = 0.002f, .z = -0.023f};
static const Vector3i data_2EB58 = {.x = 49152, .y = 0, .z = 0};
static const int32_t data_2EB64[10] = {0, 2730, 1820, 3640, 0, 0, 0, 0, 0, 0};
static const int32_t data_2EB8C[10] = {0, 0, 0, 3640, 64443, 0, 0, 60074, 0, 0};
static const Vector3i data_2EBB4 = {.x = 0, .y = 364, .z = 5279};
static const char data_2EBC0[16] __attribute__((scn_string_literal)) = "/scene/02/JD00/";
static const char data_2EBD0[5] __attribute__((scn_string_literal)) = "INYO";
static const char data_2EBD5[5] __attribute__((scn_string_literal)) = "INYO";
static const char data_2EBDA[1] __attribute__((scn_string_literal)) = "";
static const char data_2EBDB[6] __attribute__((scn_string_literal)) = "01INY";
static const char data_2EBE1[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_2EBE6[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_2EBEB[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_2EBF0[11] __attribute__((scn_string_literal)) = "amb019.snd";
static const char data_2EBFB[11] __attribute__((scn_string_literal)) = "bgm052.snd";
static const char data_2EC06[13] __attribute__((scn_string_literal)) = "a1_inyou.snd";
static const char data_2EC13[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_2EC18[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_2EC1D[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_2EC22[11] __attribute__((scn_string_literal)) = "amb019.snd";
static const char data_2EC2D[7] __attribute__((scn_string_literal)) = "SELECT";
static const char data_2EC34[13] __attribute__((scn_string_literal)) = "f1jutadn.snd";
static const char data_2EC41[7] __attribute__((scn_string_literal)) = "INYO";
static const int32_t data_2EC48[57] = {
  0, 0, -3276, 0, 0, 0, 0, 0, 0, 0, 0, -6553, 0, 0, -4369, 0, -546, -3458, 0, 0, 0, 0, 0, -5461, 0, 0, -5643, 0, 0, -3458, 0, 0, 0, 0, 0, -6007, 0, 0,
  -4551, 0, 546, -2548, 0, 0, 0, 0, 0, -6189, 0, -1092, -3094, 0, 0, -2548, 0, 0, 0
};
static const int32_t data_2ED2C[57] = {
  0, 0, -6007, 0, 0, -728, 0, 0, 0, 0, 0, -5097, 0, 0, -2184, 0, 0, -2002, 0, 0, 0, 0, 0, -5825, 0, 0, -2912, 0, 0, -728, 0, 0, 0, 0, 0, -4915, 0, 0,
  -4187, 0, 0, -1274, 0, 0, 0, 0, 0, -6553, 0, -364, -4369, 0, -546, -1456, 0, 0, 0
};
static const Vector4 data_2EE10 = {.x = 1.0f, .y = 0.5f, .z = 0.5f, .w = 0.5f};
static const char data_2EE20[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_2EE25[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_2EE2A[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_2EE2F[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_2EE34[11] __attribute__((scn_string_literal)) = "bgm116.snd";
static const char data_2EE3F[13] __attribute__((scn_string_literal)) = "a1_taike.snd";
static const char data_2EE4C[16] __attribute__((scn_string_literal)) = "/scene/02/JD00/";
static const char data_2EE5C[5] __attribute__((scn_string_literal)) = "KSBT";
static const char data_2EE61[6] __attribute__((scn_string_literal)) = "A0167";
static const char data_2EE67[5] __attribute__((scn_string_literal)) = "KSBT";
static const char data_2EE6C[1] __attribute__((scn_string_literal)) = "";
static const char data_2EE6D[5] __attribute__((scn_string_literal)) = "KSBT";
static const char data_2EE72[1] __attribute__((scn_string_literal)) = "";
static const char data_2EE73[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_2EE78[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_2EE7D[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_2EE82[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_2EE87[11] __attribute__((scn_string_literal)) = "bgm013.snd";
static const char data_2EE92[13] __attribute__((scn_string_literal)) = "e1kishou.snd";
static const char data_2EE9F[13] __attribute__((scn_string_literal)) = "battle_1.snd";
static const char data_2EEAC[11] __attribute__((scn_string_literal)) = "M_FGT1.BIN";
static const char data_2EEB7[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_2EEBC[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_2EEC1[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_2EEC6[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_2EECB[11] __attribute__((scn_string_literal)) = "bgm024.snd";
static const char data_2EED6[13] __attribute__((scn_string_literal)) = "a1_bdygd.snd";
static const char data_2EEE3[5] __attribute__((scn_string_literal)) = "KSBT";
static const char data_2EEE8[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_2EEED[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_2EEF2[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_2EEF7[7] __attribute__((scn_string_literal)) = "SELECT";
static const char data_2EEFE[13] __attribute__((scn_string_literal)) = "f1jutadn.snd";
static const char data_2EF0B[9] __attribute__((scn_string_literal)) = "A0167";
static const Vector3 data_2EF14 = {.x = -7.54f, .y = 1.6f, .z = 26.38f};
static const Vector3i data_2EF20 = {.x = 0, .y = 0, .z = -4369};
static const int32_t data_2EF2C[10] = {0, 1274, 0, 1820, 7463, 0, 7645, 7463, 0, 7463};
static const char data_2EF54[11] __attribute__((scn_string_literal)) = "tsm001.snd";
static const char data_2EF5F[11] __attribute__((scn_string_literal)) = "M_MOBJ.BIN";
static const char data_2EF6A[16] __attribute__((scn_string_literal)) = "/scene/02/JD00/";
static const char data_2EF7A[5] __attribute__((scn_string_literal)) = "JD00";
static const char data_2EF7F[14] __attribute__((scn_string_literal)) = "scene/02/D000";
static const char data_2EF8D[] __attribute__((scn_string_literal)) = "SCRTEST.BIN";
static const uint8_t data_2EF99[3] = {0};
static const uint32_t data_2EF9C = 380;
static const int32_t data_2EFA0[6] = {1, 390, 0, 1018, 0, -1};
static const uint32_t data_2EFB8 = 510;
static const int32_t data_2EFBC[6] = {1, 525, 0, 1020, 0, -1};
static const uint32_t data_2EFD4 = 150;
static const int32_t data_2EFD8[6] = {1, 160, 0, 1019, 0, -1};
static const uint32_t data_2EFF0 = 530;
static const int32_t data_2EFF4[6] = {1, 970, 0, 1014, 0, -1};
static const uint32_t data_2F00C = 911;
static const int32_t data_2F010[6] = {1, 155, 0, 1015, 0, -1};
static const char data_2F028[7] __attribute__((scn_string_literal)) = "FREE02";
static const char data_2F02F[7] __attribute__((scn_string_literal)) = "FREE03";
static const char data_2F036[7] __attribute__((scn_string_literal)) = "FREE04";
static const char data_2F03D[7] __attribute__((scn_string_literal)) = "FREE01";
static const uint32_t data_2F044 = 0xC1F00000;
static const float data_2F048[2] = {0.0f, 30.0f};
static const uint32_t data_2F050 = 0x40C00000;
static const float data_2F054[2] = {0.0f, 44.0f};
static const uint32_t data_2F05C = 0x41100000;
static const float data_2F060[2] = {0.0f, 35.0f};
static const uint32_t data_2F068 = 0x41E00000;
static const float data_2F06C[2] = {0.0f, 45.0f};
static const uint32_t data_2F074 = 0xC2480000;
static const uint8_t pad_2F078[8] = {0};
static const uint32_t data_2F080 = 0x40E00000;
static const float data_2F084[2] = {0.0f, -2.0f};
static const uint32_t data_2F08C = 0x41400000;
static const float data_2F090[2] = {0.0f, -22.0f};
static const uint8_t data_2F098[12] = {0};
static const uint8_t data_2F0A4[12] = {0};
static const uint8_t data_2F0B0[12] = {0};
static const uint8_t data_2F0BC[12] = {0};
static const uint32_t data_2F0C8 = 0x3F800000;
static const float data_2F0CC[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.8f, 0.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint32_t data_2F150 = 0x3F800000;
static const float data_2F154[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.8f, 0.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint32_t data_2F1D8 = 0x3F800000;
static const float data_2F1DC[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.8f, 0.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint32_t data_2F260 = 0x3F800000;
static const float data_2F264[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.8f, 0.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint32_t data_2F2E8 = 0x3F800000;
static const float data_2F2EC[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.8f, 0.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint32_t data_2F370 = 0x3F800000;
static const float data_2F374[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.8f, 0.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint32_t data_2F3F8 = 0x3F800000;
static const float data_2F3FC[33] = {
  0.944f, 0.866f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 1.0f, 1.0f, 1.0f,
  0.8f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint8_t data_2F480[80] = {0};
typedef struct Row_2F4D0 { float w0; float w1; float w2; int32_t w3; } Row_2F4D0;
static const Row_2F4D0 data_2F4D0[3] = {
  {0.8f, 0.8f, 0.8f, 0},
  {0.0f, 0.0f, 0.0f, 0},
  {0.0f, 0.0f, 0.0f, 0}
};
static const int32_t data_2F500[2] = {0, 0};
static const uint8_t data_2F508[96] = {0};
static const float data_2F568[10] = {0.8f, 0.8f, 0.8f, 0.8f, 0.8f, 0.8f, 0.8f, 0.8f, 0.8f, 0.8f};
static const uint32_t data_2F590 = 0x3F800000;
static const float data_2F594[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint32_t data_2F618 = 0x3F800000;
static const float data_2F61C[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const char data_2F6A0[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_2F6A5[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_2F6AA[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_2F6AF[7] __attribute__((scn_string_literal)) = "SELECT";
static const char data_2F6B6[14] __attribute__((scn_string_literal)) = "f1jutadn.snd";
static const uint32_t data_2F6C4 = 0xC14170A4;
static const float data_2F6C8[123] = {
  36.17f, -20.88f, 37.92f, -20.91f, 31.44f, -12.46f, 30.47f, 0.0f, 7.5f, -16.44f, 3.5f, 34.86f, 4.5f, -14.1f, 29.92f, -23.16f, 29.44f, -21.72f,
  23.58f, -13.13f, 24.21f, 0.0f, 7.5f, -17.94f, 3.5f, 26.67f, 4.5f, -21.26f, 7.45f, -16.64f, 6.81f, -16.22f, 9.79f, -20.83f, 10.43f, 0.0f, 4.26f,
  -18.73f, 2.12f, 8.62f, 1.5f, -25.96f, 8.81f, -25.84f, 10.81f, -28.21f, 10.97f, -28.33f, 8.97f, 1.93f, 5.9f, -26.09f, 3.93f, 9.89f, 2.0f, -33.09f,
  -11.92f, -23.99f, -10.08f, -25.55f, -0.33f, -35.09f, -2.31f, 1.8f, 8.49f, -29.43f, 4.73f, -6.16f, 4.7f, -19.71f, -2.82f, -15.43f, -8.92f, -13.51f,
  -8.48f, -15.02f, -1.77f, 0.57f, 4.86f, -15.94f, 2.57f, -3.23f, 2.5f, -12.19f, -0.23f, -9.49f, 0.5f, -10.42f, 3.13f, -12.52f, 2.42f, -1.0f, 2.95f,
  -11.11f, 1.07f, 1.16f, 1.5f, -5.74f, 5.17f, -3.55f, 6.11f, -6.66f, 12.87f, -8.86f, 11.92f, -1.0f, 3.22f, -6.2f, 1.19f, 9.02f, 1.3f, 0.01f, 24.91f,
  -5.85f, 24.91f, -5.85f, 23.14f, 0.01f, 23.14f, -1.0f, 2.99f, -2.91f, 1.0f
};
static const uint32_t data_2F8B4 = 0x41C00419U;
static const float data_2F8B8[128] = {
  2.5f, 1.22f, 22.82f, 3.06f, 22.82f, 3.06f, 24.38f, 1.22f, 24.38f, -1.0f, 3.1f, 2.15f, 1.0f, 23.6f, 1.0f, 5.17f, 27.96f, 5.17f, 22.75f, 9.74f,
  22.75f, 9.74f, 27.96f, -1.0f, 6.51f, 7.46f, 1.44f, 26.52f, 1.5f, 1.4f, 32.35f, 8.63f, 32.34f, 8.63f, 34.07f, 1.4f, 34.0f, -1.0f, 3.86f, 6.02f, 1.0f,
  33.21f, 2.5f, 8.64f, 40.58f, 13.25f, 35.48f, 16.11f, 38.07f, 11.5f, 43.17f, -1.0f, 3.3f, 12.38f, 1.92f, 39.33f, 3.5f, 22.06f, 36.09f, 20.86f,
  34.48f, 22.48f, 33.26f, 23.68f, 34.87f, -1.0f, 3.02f, 22.27f, 1.0f, 34.67f, 1.0f, 13.63f, 23.94f, 13.63f, 17.5f, 17.6f, 17.58f, 17.6f, 23.94f,
  -1.0f, 3.95f, 15.61f, 1.0f, 20.75f, 1.97f, 18.4f, 7.02f, 22.18f, 7.02f, 22.18f, 9.74f, 18.4f, 9.74f, -1.0f, 3.3f, 20.29f, 1.35f, 8.38f, 1.35f,
  23.86f, 11.68f, 24.53f, 7.0f, 29.56f, 7.7f, 28.89f, 12.39f, -1.0f, 3.3f, 26.71f, 2.35f, 9.69f, 2.5f, 6.93f, 16.18f, 9.35f, 16.18f, 9.35f, 18.02f,
  6.93f, 18.02f, -1.0f, 3.01f, 8.14f, 1.0f, 17.1f, 1.0f, 9.9f
};
static const shen_name data_2FAB8 = SHEN_NAME('33KA');
static const float data_2FABC = 4.64f;
static const shen_name data_2FAC0 = SHEN_NAME('33KA');
static const float data_2FAC4[38] = {
  4.64f, 7.09f, 9.9f, 7.09f, -1.0f, 5.86f, 7.27f, 1.0f, 9.9f, 2.62f, 4.87f, 1.89f, 5.03f, 0.1f, 7.43f, 0.32f, 7.26f, 2.11f, -1.21f, 3.05f, 6.15f,
  1.0f, 1.1f, 1.0f, 5.27f, -13.42f, 5.27f, -16.92f, 9.88f, -17.0f, 9.88f, -13.42f, -1.0f, 3.3f, 7.58f, 1.75f, -15.17f, 1.75f
};
