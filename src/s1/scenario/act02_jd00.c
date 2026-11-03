#include "act02_jd00.h"

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

__attribute__((scn_string_bindings(0, data_2647C)))
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
    while (*(&task_78 + arg5) & 4) {
      SCNF_WaitFrames_0000(1);
    }
    arg2 = Script_KaidanUtils(CAM_STAIR_QUERY_GUIDE_ENTRY, arg0, task_6C, (const void *)task_70, (const void *)task_74, task_8C);
    if (arg2 >= 0) {
      Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_2);
      Script_ccowUtils(arg0, 9);
      if (*(&task_78 + arg5) & 0x800) {
        Script_scenUtils(SCENARIO_CMD_INIT_SCNT);
      }
      (void)((local_CA = 0) & 1);
      local_E4 = arg2 * 6;
      *(int32_t *)&local_F4[1] = Script_ScenDataUtil((void *)task_70, 4, local_E4 + 1);
      *(int32_t *)&local_F4[0] = Script_ScenDataUtil((void *)task_70, 4, local_E4);
      *(int32_t *)&local_F4[2] = Script_ScenDataUtil((void *)task_70, 4, local_E4 + 2);
      local_DC = Script_ScenDataUtil((void *)task_70, 4, local_E4 + 4);
      local_E8[1] = Script_ScenDataUtil((void *)task_70, 4, local_E4 + 3);
      local_E0 = Script_ScenDataUtil((void *)task_70, 4, local_E4 + 5);
      local_D8 = Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, local_E8[1]);
      local_D4 = Script_MathUtils(SCRIPT_MATH_COS_FIXED16, local_E8[1]);
      script_20A3(arg0, arg5, arg2);
      *(&task_84 + arg5) = arg2 + 1;
      *((int8_t *)&*(int32_t *)&task_B0 + arg5) = 3;
      script_2D34(arg0, arg5, local_DC);
      Script_CharHandlers(arg0, 2, 0, 16);
      (void)((local_CD = -((local_CB = 0) != 0)) & 1);
      if (local_DC & 0xF00 & 0x400) {
        script_0A25(1, arg2);
        if (local_DC & 0xF000 & 0x1000) {
          (void)((task_B8 = -1) & 1);
          while (task_B8 & 8) {
            SCNF_WaitFrames_0000(1);
          }
        }
      }
      if (local_DC & 0x100) {
        (void)((local_CA = -1) & 1);
      }
      if (*(&task_78 + arg5) & 0x400) {
        Script_MotStepUtil(arg0, 0);
      }
      if ((local_DC & 0xF) <= 4) {
        Script_ccowSetFlag80(arg0, 1);
        if ((*(&task_78 + arg5) & 8) == 0) {
          if (local_DC & 0x1000000) {
            Script_scenCreateTask(script_24C5, 2, arg0, 1);
          } else {
            Script_scenCreateTask(script_24C5, 2, arg0, 1);
          }
        }
        if (local_DC & 0x2000) {
          task_B5 = 0;
        } else {
          uint8_t unused_C8[2];
          int32_t local_C4;
          local_C4 = *(&task_7C + arg5);
          switch (local_C4) {
            case 0:
              task_B5 = 0;
              break;
            case 1:
              task_B5 = 1;
              break;
            case 3:
              task_B5 = 1;
              break;
            case 2:
              if (Script_ryoControl(PCCTL_QueryIsRunningRequest)) {
                task_B5 = 1;
              } else {
                task_B5 = 0;
              }
              break;
          }
        }
        if (task_B5) {
          local_F4[0] = *local_F4 + local_D8 * -0.1f;
          local_F4[2] = local_F4[2] + local_D4 * -0.1f;
        } else {
          local_F4[0] = *local_F4 + local_D8 * 0.16f;
          local_F4[2] = local_F4[2] + local_D4 * 0.16f;
        }
        local_CF = script_1B07(arg0, local_E8[1], *local_F4, local_F4[2], arg5, local_DC, local_E0);
        if (local_CA & 4) {
          Script_KaidanUtils(CAM_STAIR_APPLY_GUIDE_FACING, arg0, arg2, (const void *)task_70);
        }
        (void)((local_CC = -1) & 1);
        Script_ccowSetFlag80(arg0, 2);
        Script_ccowUnknown(arg0, 1, 3.0f);
      } else {
        if ((local_DC & 0xF) <= 12) {
          if ((*(&task_78 + arg5) & 8) == 0) {
            Script_scenCreateTask(script_24C5, 2, arg0, 0);
          }
          if (local_DC & 0x2000) {
            task_B5 = 0;
          } else {
            uint8_t unused_C8[2];
            int32_t local_C4;
            local_C4 = *(&task_7C + arg5);
            switch (local_C4) {
              case 0:
                task_B5 = 0;
                break;
              case 1:
                task_B5 = 1;
                break;
              case 3:
                task_B5 = 1;
                break;
              case 2:
                if (Script_ryoControl(PCCTL_QueryIsRunningRequest)) {
                  task_B5 = 1;
                } else {
                  task_B5 = 0;
                }
                break;
            }
          }
          if (task_B5) {
            local_F4[0] = *local_F4 + local_D8 * -0.25f;
            local_F4[2] = local_F4[2] + local_D4 * -0.25f;
          } else {
            local_F4[0] = *local_F4 + local_D8 * 0.15f;
            local_F4[2] = local_F4[2] + local_D4 * 0.15f;
          }
          local_CF = local_E0 - script_1D96(arg0, local_E8[1], *local_F4, local_F4[2], arg5, local_DC, local_E0);
          if (local_CA & 4) {
            Script_KaidanUtils(CAM_STAIR_APPLY_GUIDE_FACING, arg0, arg2, (const void *)task_70);
          }
          Script_ccowSetFlag80(arg0, 2);
          Script_ccowUnknown(arg0, 1, 3.0f);
          (void)((local_CC = 0) & 0x80);
          goto L000525;
        }
      }
      L000525:;
      if ((local_E0 == 1) & ((local_DC & 0xF) <= 4)) {
        if (*((int8_t *)&*(int32_t *)&task_B3 + arg5) == 1) {
          Script_motmToggleFlags(arg0, 1, 0x80);
        }
        if (task_B5) {
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
          local_CE = task_B5;
          if (local_D0) {
            local_D0 = 0;
          } else if (local_DC & 0x2000) {
            task_B5 = 0;
          } else {
            uint8_t unused_C8[2];
            int32_t local_C4;
            local_C4 = *(&task_7C + arg5);
            switch (local_C4) {
              case 0:
                task_B5 = 0;
                break;
              case 1:
                task_B5 = 1;
                break;
              case 3:
                task_B5 = 1;
                break;
              case 2:
                if (Script_ryoControl(PCCTL_QueryIsRunningRequest)) {
                  task_B5 = 1;
                } else {
                  task_B5 = 0;
                }
                break;
            }
          }
          if (local_CB & 8) {
          } else {
            if (*(&task_78 + arg5) & 0x100) {
              if (*(&task_78 + arg5) & 2) {
                if ((Script_GetControllerInput(0) & 0x2000) > 0) {
                  (void)((local_CD = -1) & 4);
                }
              }
            }
            if (local_CC & 0x10) {
              local_CF = local_CF + script_0E0C(arg0, local_CF, local_E0, arg5, local_DC, arg2, local_CE - task_B5);
            } else {
              local_CF = local_CF - script_12FC(arg0, local_CF, arg5, local_DC, arg2, local_CE - task_B5);
            }
            if ((local_CF > 0) & (local_CF < local_E0)) {
              if (!(local_CD & 0x20)) goto L000767;
            }
          }
          L000767:;
        }
        if (local_D0) {
          if (((*(&task_78 + arg5) & 0x10) == 0) & ((local_DC & 0xF) < 2)) {
            if (*((int8_t *)&*(int32_t *)&task_B3 + arg5) == 1) {
              Script_motmToggleFlags(arg0, 1, 0x80);
            }
            if (task_B5) {
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
      if ((task_B6 == 1) | (task_B6 == 2)) {
        task_B6 = 3;
      }
      if ((*(&task_78 + arg5) & 2) > 0) {
        if (task_B5) {
          Script_SetCharaMotionPlaybackRate(arg0, 1.2f);
        } else {
          Script_SetCharaMotionPlaybackRate(arg0, 1.3f);
        }
        if (task_BC & 0x10) {
          Script_sprintf("Kaidan : ");
          Script_ryoControl(RYO_RestoreControl);
          (void)((task_BC = 0) & 4);
        }
        script_2E09(arg0, arg5, local_DC);
      }
      Script_ccowUnknown(arg0, 2, 0.07f);
      Script_ccowUtils(arg0, 10);
      if (*((int8_t *)&*(int32_t *)&task_B1 + arg5) != 2) {
        *((int8_t *)&*(int32_t *)&task_B1 + arg5) = 0;
      }
      *((int8_t *)&*(int32_t *)&task_B2 + arg5) = 0;
      *((int8_t *)&*(int32_t *)&task_B0 + arg5) = 0;
      *(&task_84 + arg5) = -1;
      if (*(&task_78 + arg5) & 0x800) {
        Script_scenUtils(SCENARIO_CMD_UPDATE_TIMER);
      }
    } else {
      (void)((task_BC = 0) & 1);
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_0932(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
  int32_t local_FC;
  int32_t local_F8;
  *(&task_78 + arg5) = arg4;
  *(&task_80 + arg5) = arg0;
  *(&task_84 + arg5) = -1;
  *(&task_7C + arg5) = arg1;
  *((int8_t *)&*(int32_t *)&task_B1 + arg5) = 0;
  *((int8_t *)&*(int32_t *)&task_B2 + arg5) = 0;
  Script_sprintf("========== initializing kaidan\012");
  if (arg3) {
    (void)((task_B7 = 0) & 1);
    task_8C = 0x4000;
    task_B4 = 1;
    task_70 = arg3;
    (void)((task_BC = 0) & 1);
    if (arg2) {
      Script_sprintf("please ENDOFKDATA use can't start shuki kaidan\012");
      task_6C = arg2;
    } else {
      Script_CycleManagerUtils(CYCLE_CMD_SET_GLOBAL_CYCLE_PTR, task_70, &task_84);
      task_6C = local_F8 = 0;
      while ((local_FC = Script_ScenDataUtil((void *)task_70, 4, local_F8)) != 0x47C35000) {
        task_6C = task_6C + 1;
        local_F8 = local_F8 + 6;
      }
    }
  } else {
    task_B4 = task_B4 + 1;
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
  while ((local_F8 = Script_ScenDataUtil((void *)task_74, 4, local_FC)) != 3) {
    local_F0 = local_F8;
    if (local_F0 == 0) {
      local_FC = local_FC + 3;
    } else if (local_F0 == 1) {
      if (arg0) {
        if (Script_ScenDataUtil((void *)task_74, 4, local_FC + 1) == arg1) {
          local_F4 = Script_ScenDataUtil((void *)task_74, 4, local_FC + 2);
          if (local_F4 < 4) {
            local_F4 = task_90[local_F4];
            break;
          } else {
            Script_PreserveAccumulator_012C(0, "Kaidan Function Error 1\012");
          }
        }
      }
      local_FC = local_FC + 3;
    } else if (local_F0 == 2) {
      if (arg0 == 0) {
        if (Script_ScenDataUtil((void *)task_74, 4, local_FC + 1) == arg1) {
          local_F4 = Script_ScenDataUtil((void *)task_74, 4, local_FC + 2);
          if (local_F4 < 4) {
            local_F4 = task_90[local_F4];
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
    task_B5 = 0;
  } else {
    local_F8 = *(&task_7C + arg1);
    switch (local_F8) {
      case 0:
        task_B5 = 0;
        break;
      case 1:
        task_B5 = 1;
        break;
      case 3:
        task_B5 = 1;
        break;
      case 2:
        if (Script_ryoControl(PCCTL_QueryIsRunningRequest)) {
          task_B5 = 1;
        } else {
          task_B5 = 0;
        }
        break;
    }
  }
  if (task_B5) {
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
    task_B5 = 0;
  } else {
    local_F8 = *(&task_7C + arg1);
    switch (local_F8) {
      case 0:
        task_B5 = 0;
        break;
      case 1:
        task_B5 = 1;
        break;
      case 3:
        task_B5 = 1;
        break;
      case 2:
        if (Script_ryoControl(PCCTL_QueryIsRunningRequest)) {
          task_B5 = 1;
        } else {
          task_B5 = 0;
        }
        break;
    }
  }
  if (task_B5) {
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
  task_A0 = 0;
  if (task_B5) {
    local_FF = 4;
    if (arg1 < arg2 - 3) {
      Script_motmToggleFlags(arg0, 2, 0x40);
      if (arg6) {
        if (*((int8_t *)&*(int32_t *)&task_B3 + arg3) == 1) {
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
      if ((*(&task_78 + arg3) & 8) == 0) {
        task_B6 = 2;
      }
      script_2DB9(arg0, arg3, arg4);
      if (arg4 & 0xF00 & 0x800) {
        script_0A25(0, arg5);
        if (arg4 & 0xF000 & 0x1000) {
          (void)((task_B8 = -1) & 1);
          while (task_B8 & 8) {
            SCNF_WaitFrames_0000(1);
          }
        }
      } else if (arg2 <= arg1 + 2) {
        if (*((int8_t *)&*(int32_t *)&task_B3 + arg3) == 0) {
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
          if (*((int8_t *)&*(int32_t *)&task_B3 + arg3) == 1) {
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
        if (*((int8_t *)&*(int32_t *)&task_B3 + arg3) == 0) {
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
      if ((*(&task_78 + arg3) & 8) == 0) {
        task_B6 = 3;
      }
    }
  } else {
    local_FF = 2;
    if (arg1 < arg2 - 1) {
      if (arg6) {
        if (*((int8_t *)&*(int32_t *)&task_B3 + arg3) == 1) {
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
      if ((*(&task_78 + arg3) & 8) == 0) {
        task_B6 = 2;
      }
      script_2DB9(arg0, arg3, arg4);
      if (arg6) {
        if (*((int8_t *)&*(int32_t *)&task_B3 + arg3) == 1) {
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
          (void)((task_B8 = -1) & 2);
          while (task_B8 & 8) {
            SCNF_WaitFrames_0000(1);
          }
        }
      } else {
        if (arg2 == arg1) {
          if (*((int8_t *)&*(int32_t *)&task_B3 + arg3) == 0) {
            Script_motmToggleFlags(arg0, 1, 0x80);
          }
        } else {
          if (*((int8_t *)&*(int32_t *)&task_B3 + arg3) == 1) {
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
      if ((*(&task_78 + arg3) & 8) == 0) {
        task_B6 = 3;
      }
    }
  }
  return local_FF;
}

static int8_t script_12FC(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
  int8_t local_FF;
  if (task_B5) {
    local_FF = 4;
    if (arg1 >= local_FF) {
      if (arg5) {
        if (*((int8_t *)&*(int32_t *)&task_B3 + arg2) == 1) {
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
      if (*((int8_t *)&*(int32_t *)&task_B1 + arg2) != 2) {
        *((int8_t *)&*(int32_t *)&task_B1 + arg2) = 0;
      }
      switch (arg1) {
        case 1:
          if (arg5) {
            if (*((int8_t *)&*(int32_t *)&task_B3 + arg2) == 1) {
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
            if (*((int8_t *)&*(int32_t *)&task_B3 + arg2) == 1) {
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
            if (*((int8_t *)&*(int32_t *)&task_B3 + arg2) == 1) {
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
      if ((*(&task_78 + arg2) & 8) == 0) {
        task_B6 = 2;
      }
      script_2DB9(arg0, arg2, arg3);
      if (arg3 & 0xF00 & 0x800) {
        script_0A25(0, arg4);
        if (arg3 & 0xF000 & 0x1000) {
          (void)((task_B8 = -1) & 1);
          while (task_B8 & 8) {
            SCNF_WaitFrames_0000(1);
          }
        }
      } else {
        if (arg1 <= 2) {
          if (*((int8_t *)&*(int32_t *)&task_B3 + arg2) == 1) {
            Script_motmToggleFlags(arg0, 1, 0x80);
          }
        } else {
          if (*((int8_t *)&*(int32_t *)&task_B3 + arg2) == 0) {
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
      if ((*(&task_78 + arg2) & 8) == 0) {
        task_B6 = 3;
      }
    }
  } else {
    local_FF = 2;
    if (arg1 >= local_FF) {
      if (arg5) {
        if (*((int8_t *)&*(int32_t *)&task_B3 + arg2) == 1) {
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
      if (*((int8_t *)&*(int32_t *)&task_B1 + arg2) != 2) {
        *((int8_t *)&*(int32_t *)&task_B1 + arg2) = 0;
      }
      if (arg1) {
        if (arg5) {
          if (*((int8_t *)&*(int32_t *)&task_B3 + arg2) == 1) {
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
      if ((*(&task_78 + arg2) & 8) == 0) {
        task_B6 = 2;
      }
      if (arg3 & 0xF00 & 0x800) {
        script_0A25(0, arg4);
        if (arg3 & 0xF000 & 0x1000) {
          (void)((task_B8 = -1) & 1);
          while (task_B8 & 8) {
            SCNF_WaitFrames_0000(1);
          }
        }
        script_2DB9(arg0, arg2, arg3);
      } else {
        if (arg1) {
          if (*((int8_t *)&*(int32_t *)&task_B3 + arg2) == 1) {
            Script_motmToggleFlags(arg0, 1, 0x80);
          }
        } else {
          if (*((int8_t *)&*(int32_t *)&task_B3 + arg2) == 0) {
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
      if ((*(&task_78 + arg2) & 8) == 0) {
        task_B6 = 3;
      }
    }
  }
  return local_FF;
}

static void script_18C8(int32_t arg0, int32_t arg1, float arg2, float arg3, int32_t arg4, int32_t arg5) {
  Vector3 local_F4;
  float local_E8[3];
  if (arg5 & 0x2000) {
    task_B5 = 0;
  } else {
    int32_t unused_D4[5];
    int32_t local_D0;
    local_D0 = *(&task_7C + arg4);
    switch (local_D0) {
      case 0:
        task_B5 = 0;
        break;
      case 1:
        task_B5 = 1;
        break;
      case 3:
        task_B5 = 1;
        break;
      case 2:
        if (Script_ryoControl(PCCTL_QueryIsRunningRequest)) {
          task_B5 = 1;
        } else {
          task_B5 = 0;
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
  if (task_B5) {
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
  task_A0 = 0;
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
  if (task_B5) {
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
      task_A0 = -0x80000000;
      *((int8_t *)&*(int32_t *)&task_B3 + arg4) = 1;
    } else {
      *((int8_t *)&*(int32_t *)&task_B3 + arg4) = 0;
    }
    Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
    if (arg6 > 1) {
      Script_SetCharaMoveTargetEx(arg0, &local_F4, arg1, task_A0 | 0x2CB, 0, -1.0f, 2);
      local_B8 = 2;
    } else {
      Script_SetCharaMoveTargetEx(arg0, &local_F4, arg1, task_A0 | 0x2CB, 0, -1.0f, 2);
      local_B8 = arg6;
    }
  } else {
    local_F4.x = arg2;
    (&local_F4)->z = arg3;
    Script_EVT_GetPosition(arg0, 26, &local_DC, ENTITY_XFORM_WORLD_SPACE);
    Script_EVT_GetPosition(arg0, 33, &local_D0, ENTITY_XFORM_WORLD_SPACE);
    if (!((&local_DC)->y >= (&local_D0)->y)) {
      task_A0 = -0x80000000;
      *((int8_t *)&*(int32_t *)&task_B3 + arg4) = 1;
    } else {
      *((int8_t *)&*(int32_t *)&task_B3 + arg4) = 0;
    }
    Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
    if (arg6 > 1) {
      Script_SetCharaMoveTargetEx(arg0, &local_F4, arg1, task_A0 | 0x2C6, 0, -1.0f, 2);
      local_B8 = 1;
    } else {
      Script_SetCharaMoveTargetEx(arg0, &local_F4, arg1, task_A0 | 0x2C6, 0, -1.0f, 2);
      local_B8 = arg6;
    }
  }
  while (Script_SetCharaXMPTMode(arg0, 2) != 0) {
    SCNF_WaitFrames_0000(1);
  }
  return local_B8;
}

__attribute__((scn_string_bindings(0, data_2651B)))
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
  task_A0 = 0;
  if (task_B5) {
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
      task_A0 = -0x80000000;
      *((int8_t *)&*(int32_t *)&task_B3 + arg4) = 1;
    } else {
      *((int8_t *)&*(int32_t *)&task_B3 + arg4) = 0;
    }
    Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
    if (arg6 > 1) {
      Script_SetCharaMoveTargetEx(arg0, &local_F4, arg1, task_A0 | 0x2F9 | 0x30000, 0, -1.0f, 2);
      local_B8 = 0;
    } else {
      Script_SetCharaMoveTargetEx(arg0, &local_F4, arg1, task_A0 | 0x2F9 | 0x30000, 0, -1.0f, 2);
      local_B8 = arg6;
    }
  } else {
    local_F4.x = arg2;
    (&local_F4)->z = arg3;
    Script_EVT_GetPosition(arg0, 26, &local_DC, ENTITY_XFORM_WORLD_SPACE);
    Script_EVT_GetPosition(arg0, 33, &local_D0, ENTITY_XFORM_WORLD_SPACE);
    if (!((&local_DC)->y >= (&local_D0)->y)) {
      task_A0 = -0x80000000;
      *((int8_t *)&*(int32_t *)&task_B3 + arg4) = 1;
    } else {
      *((int8_t *)&*(int32_t *)&task_B3 + arg4) = 0;
    }
    Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
    if (arg6 > 1) {
      Script_SetCharaMoveTargetEx(arg0, &local_F4, arg1, task_A0 | 0x2C7 | 0x90000, 0, -1.0f, 2);
      local_B8 = 0;
    } else {
      Script_SetCharaMoveTargetEx(arg0, &local_F4, arg1, task_A0 | 0x2C7 | 0x90000, 0, -1.0f, 2);
      local_B8 = arg6;
    }
  }
  while (Script_SetCharaXMPTMode(arg0, 2) != 0) {
    SCNF_WaitFrames_0000(1);
  }
  Script_ccowUtils(arg0, 1);
  script_2D34(arg0, arg4, arg5);
  if (*(&task_78 + arg4) & 2) {
    if (Script_ryoControl(PCCTL_QueryShouldRun)) {
      Script_sprintf("Kaidan : ");
      Script_ryoControl(PCCTL_DisablePlayerControl);
      (void)((task_BC = -1) & 4);
    }
    Script_ccowUtils(arg0, 6);
  }
  return local_B8;
}

__attribute__((scn_string_bindings(1, data_26541)))
static void script_20A3(int32_t arg0, int32_t arg1, int32_t arg2) {
  int32_t local_FC;
  int32_t local_F8;
  int8_t local_F7;
  (void)((local_F7 = 0) & 4);
  arg2 = arg2 & -2;
  local_F8 = Script_ScenDataUtil((void *)task_70, 4, arg2 * 6 + 4);
  local_FC = Script_ScenDataUtil((void *)task_70, 4, (arg2 + 1) * 6 + 4);
  local_FC = local_FC | local_F8;
  if (local_FC & -0x80000000) {
    while (local_FC & -0x80000000) {
      if (local_F7 & 0x80) {
        if (task_C0) {
        } else {
          Script_sprintf("wait ... shuki shuki shuki\012");
        }
      } else {
        if (*(&task_78 + arg1) & 2) {
          if (Script_ryoControl(PCCTL_QueryShouldRun)) {
            Script_sprintf("Kaidan : ");
            Script_ryoControl(PCCTL_DisablePlayerControl);
            (void)((task_BC = -1) & 4);
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
      task_C0 = (task_C0 + 1) & 0xF;
      local_F8 = Script_ScenDataUtil((void *)task_70, 4, arg2 * 6 + 4);
      local_FC = Script_ScenDataUtil((void *)task_70, 4, (arg2 + 1) * 6 + 4);
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
  if (~task_B7) {
    (void)((task_B7 = -1) & 1);
    task_A4 = Script_SetCameraSettings(CAM_UTILS_SET_DISTANCE_FROM_PLAYER, -1.0f);
    task_A8 = Script_SetCameraSettings(CAM_UTILS_SET_HEIGHT_ABOVE_PLAYER, -1.0f);
    task_AC = Script_SetCameraSettings(CAM_UTILS_SET_MAX_Y_ANGLE, -1.0f);
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
    local_F4 = task_B6;
    if ((local_F4 == 0) | (local_F4 == 2) | (local_F4 == 4)) {
      break;
      goto L0023A3;
    }
    L0023A3:;
    local_F8 = task_AC + (arg0 - task_AC) * (float)local_FC / 10.0f;
    Script_SetCameraSettings(CAM_UTILS_SET_MAX_Y_ANGLE, local_F8);
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  {
    int32_t local_F4;
    local_F4 = task_B6;
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
    local_F4 = task_B6;
    if ((local_F4 == 0) | (local_F4 == 2) | (local_F4 == 4)) {
      break;
      goto L002432;
    }
    L002432:;
    local_F8 = task_A4 + (arg0 - task_A4) * (float)local_FC / 50.0f;
    Script_SetCameraSettings(CAM_UTILS_SET_DISTANCE_FROM_PLAYER, local_F8);
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  {
    int32_t local_F4;
    local_F4 = task_B6;
    if ((local_F4 == 0) | (local_F4 == 2) | (local_F4 == 4)) {
    } else {
      Script_SetCameraSettings(CAM_UTILS_SET_DISTANCE_FROM_PLAYER, arg0);
    }
  }
  return;
}

static void script_248E(void) {
  Script_SetCameraSettings(6, 7);
  Script_SetCameraSettings(CAM_UTILS_SET_DISTANCE_FROM_PLAYER, task_A4);
  Script_SetCameraSettings(CAM_UTILS_SET_HEIGHT_ABOVE_PLAYER, task_A8);
  Script_SetCameraSettings(CAM_UTILS_SET_MAX_Y_ANGLE, task_AC);
  Script_KaidanUtils(CAM_STAIR_RESET_RETRACTION_LIMIT);
  (void)((task_B7 = 0) & 2);
  return;
}

static int32_t script_24C5(int32_t arg0, int32_t arg1) {
  Vector3 local_F4;
  Vector3 local_E8;
  int8_t local_E7;
  int8_t local_E6;
  task_88 = arg0;
  task_B6 = 1;
  local_E7 = -1;
  (void)((local_E6 = 0) & 2);
  Script_SetGameStateTo(AUTH_None);
  while (task_88 == arg0) {
    if (Script_GetAuthorState_And_SetCamPos(&local_F4, &local_E8) != 17) {
      if (task_B6 != local_E7) {
        uint8_t unused_E4[2];
        int32_t local_E0;
        local_E0 = task_B6;
        if (local_E0 == 1) {
          (void)((local_E6 = -1) & 2);
          script_22D1(arg1);
        } else if (local_E0 == 2) {
          script_248E();
        } else if ((local_E0 == 0) | (local_E0 == 3)) {
          Script_SetCameraSettings(6, 7);
          task_88 = 0;
        } else {
          if (local_E0 == 4) goto L002572;
        }
        L002572:;
        local_E7 = task_B6;
      }
    } else {
      if (task_B6 != local_E7) {
        uint8_t unused_E4[2];
        int32_t local_E0;
        local_E0 = task_B6;
        if ((local_E0 == 0) | (local_E0 == 3)) {
          Script_SetCameraSettings(6, 7);
          task_88 = 0;
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
  if (task_B6 != 0) {
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
  while (local_FF < task_B4) {
    if (arg0 == *(&task_80 + local_FF)) {
      local_F8 = *(&task_84 + local_FF);
    }
    local_FF = local_FF + 1;
  }
  (void)local_F8;
  return;
}

static void script_2643(int32_t arg0) {
  task_74 = arg0;
  return;
}

static void script_264D(int32_t arg0) {
  task_8C = arg0;
  return;
}

static void script_2657(void) {
  (void)task_8C;
  return;
}

__attribute__((scn_string_bindings(0, data_2654B)))
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

__attribute__((scn_string_bindings(0, data_26563)))
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

__attribute__((scn_string_bindings(0, data_2657B)))
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

__attribute__((scn_string_bindings(0, data_26593)))
static void script_28EF(int32_t arg0, int32_t arg1) {
  Vector3 local_F4;
  Vector3i local_E8;
  float local_E4;
  float local_E0;
  float local_DC;
  if (*((int8_t *)&*(int32_t *)&task_B2 + arg1) == 0) {
    *((int8_t *)&*(int32_t *)&task_B2 + arg1) = 1;
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

__attribute__((scn_string_bindings(0, data_265AB)))
static void script_29E5(int32_t arg0, int32_t arg1) {
  Vector3 local_F4;
  Vector3i local_E8;
  float local_E4;
  float local_E0;
  float local_DC;
  if (*((int8_t *)&*(int32_t *)&task_B2 + arg1) == 0) {
    *((int8_t *)&*(int32_t *)&task_B2 + arg1) = 1;
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

__attribute__((scn_string_bindings(0, data_265C3)))
static int32_t script_2ADB(int32_t arg0, int32_t arg1) {
  float local_FC;
  float local_F8;
  if (*((int8_t *)&*(int32_t *)&task_B1 + arg1)) {
    Script_sprintf("KAIDAN : Warning setY!\012");
  } else {
    *((int8_t *)&*(int32_t *)&task_B1 + arg1) = 1;
    if (task_B5) {
      local_FC = local_F8 = -0.001f;
    } else {
      local_FC = local_F8 = 0.0f;
    }
    while (*((int8_t *)&*(int32_t *)&task_B1 + arg1) == 1) {
      if (task_B5) {
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
    if (*((int8_t *)&*(int32_t *)&task_B1 + arg1) != 2) {
      for (;;) {
        if (local_FC >= 0.0f) {
          break;
        }
        if (*((int8_t *)&*(int32_t *)&task_B1 + arg1) == 2) {
          *((int8_t *)&*(int32_t *)&task_B1 + arg1) = 0;
          Script_KaidanSetY(arg0, 0.0f);
          break;
        }
        local_FC = local_FC + 0.004f;
        Script_KaidanSetY(arg0, local_FC);
        SCNF_WaitFrames_0000(1);
      }
    } else {
      *((int8_t *)&*(int32_t *)&task_B1 + arg1) = 0;
      Script_KaidanSetY(arg0, 0.0f);
    }
  }
  end:;
}

__attribute__((scn_string_bindings(0, data_265DB)))
static int32_t script_2C03(int32_t arg0, int32_t arg1) {
  float local_FC;
  float local_F8;
  if (*((int8_t *)&*(int32_t *)&task_B1 + arg1)) {
    Script_sprintf("KAIDAN : Warning setY!\012");
  } else {
    *((int8_t *)&*(int32_t *)&task_B1 + arg1) = 1;
    if (task_B5) {
      local_FC = local_F8 = 0.16f;
    } else {
      local_FC = local_F8 = 0.08f;
    }
    while (*((int8_t *)&*(int32_t *)&task_B1 + arg1) == 1) {
      if (task_B5) {
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
    if (*((int8_t *)&*(int32_t *)&task_B1 + arg1) != 2) {
      for (;;) {
        if (local_FC <= 0.0f) {
          break;
        }
        if (*((int8_t *)&*(int32_t *)&task_B1 + arg1) == 2) {
          *((int8_t *)&*(int32_t *)&task_B1 + arg1) = 0;
          Script_KaidanSetY(arg0, 0.0f);
          break;
        }
        local_FC = local_FC - 0.004f;
        Script_KaidanSetY(arg0, local_FC);
        SCNF_WaitFrames_0000(1);
      }
    } else {
      *((int8_t *)&*(int32_t *)&task_B1 + arg1) = 0;
      Script_KaidanSetY(arg0, 0.0f);
    }
  }
  end:;
}

static void script_2D34(int32_t arg0, int32_t arg1, int32_t arg2) {
  if ((*(&task_78 + arg1) & 2) > 0) {
    if (Script_ryoControl(PCCTL_QueryShouldRun)) {
      Script_sprintf(data_265F3);
      Script_ryoControl(PCCTL_DisablePlayerControl);
      (void)((task_BC = -1) & 4);
    }
    Script_ccowUtils(arg0, 6);
  }
  if (*(&task_78 + arg1) & 0x20) {
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
  if (*(&task_78 + arg1) & 0x20) {
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
  if (*(&task_78 + arg1) & 0x20) {
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

static void script_387C(void) {
  Script_ryoControl(PCCTL_DisablePlayerControl);
  return;
}

static void script_3885(void) {
  Script_ryoControl(RYO_RestoreControl);
  Script_SetGameStateTo(AUTH_None);
  return;
}

static void script_3894(int32_t arg0) {
  Script_ccowUtils(arg0, 2);
  if (0) {
    Script_charaDisplay(arg0, CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(arg0, CDM_Show);
      goto L0038C0;
    }
  }
  L0038C0:;
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

static void script_3921(int32_t arg0) {
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

static void script_3971(void) {
  Script_filmCheckOrSetTransitionUtil(80, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_399F(int32_t arg0) {
  Script_filmCheckOrSetTransitionUtil(arg0, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_39CD(void) {
  Script_filmCheckOrSetTransitionUtil(80, 255, 255, 255, 255, 0, 255, 255, 255);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_3A01(int32_t arg0) {
  Script_filmCheckOrSetTransitionUtil(arg0, 255, 255, 255, 255, 0, 255, 255, 255);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_3A35(int32_t arg0) {
  Script_filmCheckOrSetTransitionUtil(arg0, 255, 255, 255, 255, 150, 68, 50, 0);
  return;
}

static void script_3A53(int32_t arg0, int32_t arg1, int32_t arg2) {
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

static void script_3A8D(int32_t arg0, int32_t arg1) {
  while (arg1 > 0) {
    Script_SetFightProgramStateBytes(arg0, arg1, 1);
    arg1 = arg1 - 1;
    SCNF_WaitFrames_0000(1);
  }
  Script_GetDataFromFightProgram(arg0, 0);
  return;
}

static void script_3AC1(void) {
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

static void script_3BB9(int32_t arg0, int32_t arg1) {
  Script_FACE_LCG(arg0, 1, arg1, 0);
  SCNF_WaitFrames_0000(arg1 + 1);
  Script_FACE_LCG(arg0, 2, arg1, 0);
  SCNF_WaitFrames_0000(arg1);
  return;
}

static void script_3BE9(int32_t arg0) {
  int8_t local_FF;
  int32_t local_F8;
  local_FF = 0;
  local_F8 = Script_shenGetStats(STATS_CASH);
  if (local_F8 - arg0 < 69000) {
    local_FF = 1;
  }
  (void)local_FF;
  return;
}

static int32_t script_3C15(int32_t arg0, int32_t arg1) {
  int8_t local_FF;
  int32_t local_F8;
  (void)((local_FF = 0) & 4);
  local_F8 = Script_shenGetStats(STATS_CASH);
  if (local_F8 - arg1 >= arg0) {
    (void)((local_FF = -1) & 4);
  }
  return local_FF & 0x80;
}

static int8_t script_3C48(int32_t arg0, int32_t arg1, int32_t arg2) {
  int8_t local_FF;
  int32_t local_F8;
  int32_t local_F4;
  int8_t local_F3;
  int32_t local_EC;
  int32_t local_E8;
  local_F8 = 0;
  local_F4 = 0;
  local_FF = 0;
  local_EC = Script_StepUtils(GET_STEP_HIGHER, 0) * 100 + Script_StepUtils(GET_STEP_LOWER, 0);
  local_E8 = arg0;
  if ((local_E8 == 0) | (local_E8 == 1)) {
    if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_SPEND_BUDGET_BONUS_ACTIVE)) {
      local_F4 = 320;
    }
  } else {
    if (local_E8 == 2) {
      local_F4 = 160;
      goto L003CB6;
    }
  }
  L003CB6:;
  local_F8 = local_F8 + local_F4;
  if ((Script_StepUtils(GET_STEP_HIGHER, 0) == 17) & (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_D000_VENDING_LOCK_CLEARED) == 0)) {
    local_F8 = local_F8 + 69000;
    (void)((local_F3 = 0) & 1);
  } else if ((local_EC >= 2202) & (local_EC <= 2204)) {
    (void)((local_F3 = 0) & 1);
    local_F8 = local_F4 = 0;
    local_FF = 4;
  } else {
    (void)((local_F3 = -(script_3C15(local_F8, arg1) != 0)) & 0x10);
  }
  if (~local_F3) {
    if (local_F4 & -(local_F8 == local_F4)) {
      local_FF = 2;
    } else if (local_F8 > local_F4) {
      local_FF = 3;
    } else {
      if (local_FF == 0) {
        local_FF = 1;
      }
    }
    if (arg2) {
      script_3D8A(local_FF, arg0);
    }
  }
  return local_FF;
}

__attribute__((scn_string_bindings(0, data_27194, 8, data_271D8, 9, data_271E1, 10, data_271EA, 11, data_271F3, 12, data_271FC, 13, data_27205)))
static void script_3D8A(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  if (arg0 == 1) {
  } else {
    Script_LoadVoiceFile(1, "SY11");
    local_F4 = arg0;
    switch (local_F4) {
      case 2:
        if (arg1 == 2) {
          local_F8 = Script_FloatUtils(ScriptMathOp_RandomIntBelow, 3);
          local_F4 = local_F8;
          switch (local_F4) {
            case 0:
              local_FC = Script_ShowUIMessageFromID("SY11A006");
              break;
            case 1:
              local_FC = Script_ShowUIMessageFromID("SY11A007");
              break;
            case 2:
              local_FC = Script_ShowUIMessageFromID("SY11A008");
              break;
          }
        } else {
          local_F8 = Script_FloatUtils(ScriptMathOp_RandomIntBelow, 4);
          local_F4 = local_F8;
          switch (local_F4) {
            case 0:
              local_FC = Script_ShowUIMessageFromID("SY11A002");
              break;
            case 1:
              local_FC = Script_ShowUIMessageFromID("SY11A003");
              break;
            case 2:
              local_FC = Script_ShowUIMessageFromID("SY11A004");
              break;
            case 3:
              local_FC = Script_ShowUIMessageFromID("SY11A005");
              break;
          }
        }
        break;
      case 3:
        local_FC = Script_ShowUIMessageFromID("SY11A001");
        break;
      case 4:
        local_F4 = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f);
        if (local_F4 == 1) {
          local_FC = Script_ShowUIMessageFromID("SY11A009");
        } else if (local_F4 == 2) {
          local_FC = Script_ShowUIMessageFromID("SY11A010");
        } else {
          local_FC = Script_ShowUIMessageFromID("SY11A011");
        }
        break;
      case 5:
        local_FC = Script_ShowUIMessageFromID("SY11A009");
        break;
    }
    while (Script_convGetIsActive((const void *)local_FC)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_LoadVoiceFile(0, "SY11");
  }
  return;
}

__attribute__((scn_string_bindings(0, data_2720A, 1, data_2720F, 2, data_27218, 3, data_2721D, 4, data_27222, 5, data_2722B, 6, data_27234, 7, data_2723D)))
static int8_t script_3F53(int32_t arg0, int32_t arg1) {
  int8_t local_FF;
  int8_t local_FE;
  int32_t local_F8;
  int32_t local_F4;
  int32_t local_F0;
  local_FE = 1;
  local_FF = 0;
  local_F4 = Script_StepUtils(GET_STEP_HIGHER, 0) * 100 + Script_StepUtils(GET_STEP_LOWER, 0);
  if ((Script_StepUtils(GET_STEP_HIGHER, 0) == 17) & (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_D000_VENDING_LOCK_CLEARED) == 0)) {
    local_FE = 0;
    if (arg1) {
      if (Script_ryoControl(PCCTL_QueryShouldRun)) {
        Script_ryoControl(PCCTL_DisablePlayerControl);
        Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        local_FF = 1;
      }
      Script_LoadVoiceFile(1, "SY11");
      local_F8 = Script_ShowUIMessageFromID("SY11A001");
      while (Script_convGetIsActive((const void *)local_F8)) {
        SCNF_WaitFrames_0000(1);
      }
      Script_LoadVoiceFile(0, "SY11");
    }
  }
  if ((local_F4 >= 2202) & (local_F4 <= 2204)) {
    local_FE = 0;
    if (arg1) {
      if (Script_ryoControl(PCCTL_QueryShouldRun)) {
        Script_ryoControl(PCCTL_DisablePlayerControl);
        Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        local_FF = 1;
      }
      Script_LoadVoiceFile(1, "SY11");
      local_F0 = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f);
      if (local_F0 == 1) {
        local_F8 = Script_ShowUIMessageFromID("SY11A009");
      } else if (local_F0 == 2) {
        local_F8 = Script_ShowUIMessageFromID("SY11A010");
      } else {
        local_F8 = Script_ShowUIMessageFromID("SY11A011");
      }
      while (Script_convGetIsActive((const void *)local_F8)) {
        SCNF_WaitFrames_0000(1);
      }
      Script_LoadVoiceFile(0, "SY11");
    }
  }
  if (arg1) {
    if (local_FF == 1) {
      Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
      Script_ryoControl(RYO_RestoreControl);
    }
  }
  return local_FE;
}

static int32_t script_4129(int32_t arg0, const char *arg1) {
  task_DE = 0;
  for (;;) {
    if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0) != 0) {
      break;
    }
    if (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 5, arg1) == 0) {
      while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, "/scene/02/stream/", arg1) == 0) {
        SCNF_WaitFrames_0000(1);
      }
      while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2)) {
        SCNF_WaitFrames_0000(1);
      }
    }
  }
  task_DE = 1;
}

static void script_419E(const char *arg0) {
  if (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 5, arg0) != 0) {
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, arg0);
  }
  return;
}

static int32_t script_41BF(int32_t arg0, const char *arg1, int32_t arg2) {
  Script_LoadSND(3, 0, 0, 0);
  Script_LoadSND(1, arg0, arg1, arg2);
  while (Script_LoadSND(2, 0, 0, 0)) {
    SCNF_WaitFrames_0000(1);
  }
  return Script_LoadSND(4, 0, 0, 0);
}

static int32_t script_4204(int32_t arg0, int32_t arg1) {
  if (arg1 > 0) {
    SCNF_WaitFrames_0000(arg1);
  }
  if (task_DC == 0) {
    Script_executeAicaCommand(arg0, 0, 0);
  }
  return Script_srchControl(0);
}

static void script_4232(int32_t arg0, int32_t arg1, int32_t arg2) {
  Script_scenCreateTask(script_41BF, 3, arg0, arg1, arg2);
  return;
}

static void script_4246(int32_t arg0, int32_t arg1) {
  Script_scenCreateTask(script_4204, 2, arg0, arg1);
  return;
}

static int32_t script_4258(const char *arg0, int32_t arg1) {
  if (arg1 > 0) {
    SCNF_WaitFrames_0000(arg1);
  }
  if (task_DC == 0) {
    Script_StartOrFreeSEQ(0);
    Script_ShowUIMessageFromID(arg0);
  }
  return Script_srchControl(0);
}

static void script_4289(int32_t arg0, int32_t arg1) {
  Script_scenCreateTask(script_4258, 2, arg0, arg1);
  return;
}

static int32_t script_429B(int32_t arg0, int32_t arg1, int32_t arg2, const Vector3 *arg3, const Vector3i *arg4, int32_t arg5) {
  if (arg5 >= 0) {
    SCNF_WaitFrames_0000(arg5);
  }
  if (task_DC == 0) {
    Script_fixoAttachEx(arg0, arg1, arg2, arg3, arg4);
  }
  return Script_srchControl(0);
}

static void script_42CE(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
  Script_scenCreateTask(script_429B, 6, arg0, arg1, arg2, arg3, arg4, arg5);
  return;
}

static int32_t script_42E8(int32_t arg0, int32_t arg1) {
  if (arg1 >= 0) {
    SCNF_WaitFrames_0000(arg1);
  }
  Script_fixoClearAttachment(arg0);
  Script_charaDisplay(arg0, CDM_Hide);
  return Script_srchControl(0);
}

static void script_4310(int32_t arg0, int32_t arg1) {
  Script_scenCreateTask(script_42E8, 2, arg0, arg1);
  return;
}

static void script_4322(int32_t arg0, int32_t arg1) {
  Vector3i local_F4;
  while (Script_IsCharaMotionFinished(arg0) == 0) {
    SCNF_WaitFrames_0000(1);
  }
  if (arg1) {
    Script_mtwkGetSetRotation(arg0, &local_F4, 0x2000000);
    Script_EVT_SetRotation(arg0, 0x78000000, &local_F4, 0);
  }
  return;
}

static int32_t script_4360(int32_t arg0, int32_t arg1, int32_t arg2, float arg3, int32_t arg4, int32_t arg5) {
  if (arg5 >= 0) {
    SCNF_WaitFrames_0000(arg5);
  }
  if (task_DC == 0) {
    if (arg4 & -0x80000000) {
      Script_motmToggleFlags(arg0, 2, 0x40);
    } else {
      Script_motmToggleFlags(arg0, 1, 0x40);
    }
    if (!(arg3 >= 0.1f)) {
      arg3 = -1.0f;
    }
    Script_PlayCharaMotion(arg0, arg1, arg2, ((scn3_bits){.f = arg3}).i, arg2, 1.0f, 0);
  }
  return Script_srchControl(0);
}

static int32_t script_43D1(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  if (arg4 >= 0) {
    SCNF_WaitFrames_0000(arg4);
  }
  if (task_DC == 0) {
    if (arg3 & -0x80000000) {
      Script_motmToggleFlags(arg0, 2, 0x40);
    } else {
      Script_motmToggleFlags(arg0, 1, 0x40);
    }
    Script_PlayCharaMotion(arg0, arg1, arg2, -1.0f, -1.0f, 1.0f, 0);
  }
  return Script_srchControl(0);
}

static void script_4433(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  Script_scenCreateTask(script_43D1, 5, arg0, arg1, arg2, arg3, arg4);
  return;
}

static void script_444B(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
  Script_scenCreateTask(script_4360, 6, arg0, arg1, arg2, arg3, arg4, arg5);
  return;
}

static int32_t script_4465(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  if (arg3 >= 0) {
    SCNF_WaitFrames_0000(arg3);
  }
  if (task_DC == 0) {
    Script_SetNewHandMotion(arg0, arg1, arg2, 0, 0, 0, 0, 1.0f, 0);
  }
  return Script_srchControl(0);
}

static void script_44A3(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Script_scenCreateTask(script_4465, 4, arg0, arg1, arg2, arg3);
  return;
}

static int32_t script_44B9(int32_t arg0, int32_t arg1, const Vector3i *arg2, int32_t arg3) {
  if (arg3 >= 0) {
    SCNF_WaitFrames_0000(arg3);
  }
  if (task_DC == 0) {
    Script_SetHandRootRotation(arg0, arg1, 0x15, arg2);
  }
  return Script_srchControl(0);
}

static void script_44EA(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Script_scenCreateTask(script_44B9, 4, arg0, arg1, arg2, arg3);
  return;
}

static int32_t script_4500(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  if (arg4 >= 0) {
    SCNF_WaitFrames_0000(arg4);
  }
  if (task_DC == 0) {
    Script_FACE(arg0, arg1, arg2, arg3);
  }
  return Script_srchControl(0);
}

static void script_4531(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  Script_scenCreateTask(script_4500, 5, arg0, arg1, arg2, arg3, arg4);
  return;
}

static int32_t script_4549(int32_t arg0, int32_t arg1, const Vector3 *arg2, int32_t arg3) {
  int32_t local_FC;
  if (arg3 >= 0) {
    SCNF_WaitFrames_0000(arg3);
  }
  if (task_DC == 0) {
    local_FC = Script_CEWPRelated(arg0);
    Script_MoveObjectToObject(arg0, arg1);
    Script_EVT_SetPosition(arg0, POS_XYZ, arg2);
    Script_MoveObjectToObject(arg0, local_FC);
  }
  return Script_srchControl(0);
}

static void script_4596(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Script_scenCreateTask(script_4549, 4, arg0, arg1, arg2, arg3);
  return;
}

static int32_t script_45AC(int32_t arg0, int32_t arg1, const Vector3i *arg2, int32_t arg3) {
  int32_t local_FC;
  if (arg3 >= 0) {
    SCNF_WaitFrames_0000(arg3);
  }
  if (task_DC == 0) {
    local_FC = Script_CEWPRelated(arg0);
    Script_MoveObjectToObject(arg0, arg1);
    Script_EVT_SetRotation(arg0, 0x38000000, arg2, 0);
    Script_MoveObjectToObject(arg0, local_FC);
  }
  return Script_srchControl(0);
}

static void script_45FB(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Script_scenCreateTask(script_45AC, 4, arg0, arg1, arg2, arg3);
  return;
}

static int32_t script_4611(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  if (arg3 >= 0) {
    SCNF_WaitFrames_0000(arg3);
  }
  if (task_DC == 0) {
    Script_seqEnterEvtCam(arg0, arg1, arg2);
  }
  return Script_srchControl(0);
}

static void script_463F(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Script_scenCreateTask(script_4611, 4, arg0, arg1, arg2, arg3);
  return;
}

static void script_4655(int32_t arg0) {
  Vector4 local_F0;
  local_F0.x = 0.0f;
  (&local_F0)->y = 0.2f;
  (&local_F0)->z = 0.2f;
  (&local_F0)->w = 0.2f;
  Script_SetCharaRefBoardPos(arg0, &local_F0);
  Script_charaSetRefboardFlagTo(arg0, 1);
  return;
}

static void script_4694(void) {
  int32_t local_FC;
  local_FC = 0;
  for (;;) {
    Script_PrintDebugString(2, 4, "%03d", local_FC);
    SCNF_WaitFrames_0000(1);
    local_FC = local_FC + 1;
  }
  return;
}

static void script_46C0(int32_t arg0, int32_t arg1, int32_t arg2) {
  int8_t local_FF;
  int32_t local_F8;
  local_FF = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 100.0f);
  if (~(arg2 & 2)) {
    if (local_FF > 74) {
      task_D0 = 5;
      task_CC = (char *)data_2727B;
      arg1 = (int32_t)task_CC;
    }
  }
  local_F8 = task_D0;
  switch (local_F8) {
    case 0:
      Script_PlayerStatsUtils(PS_SetIndexedU8Table0Slot, 0, Script_PlayerStatsUtils(PS_GetIndexedU8Table0Slot, 0) + 1);
      break;
    case 1:
      Script_PlayerStatsUtils(PS_SetIndexedU8Table0Slot, 1, Script_PlayerStatsUtils(PS_GetIndexedU8Table0Slot, 1) + 1);
      break;
    case 2:
      Script_PlayerStatsUtils(PS_SetIndexedU8Table0Slot, 2, Script_PlayerStatsUtils(PS_GetIndexedU8Table0Slot, 2) + 1);
      break;
    case 3:
      Script_PlayerStatsUtils(PS_SetIndexedU8Table0Slot, 3, Script_PlayerStatsUtils(PS_GetIndexedU8Table0Slot, 3) + 1);
      break;
    case 4:
      Script_PlayerStatsUtils(PS_SetIndexedU8Table0Slot, 4, Script_PlayerStatsUtils(PS_GetIndexedU8Table0Slot, 4) + 1);
      break;
    case 5:
      Script_PlayerStatsUtils(PS_SetIndexedU8Table0Slot, 5, Script_PlayerStatsUtils(PS_GetIndexedU8Table0Slot, 5) + 1);
      break;
  }
  script_4129(arg0, (const char *)arg1);
  script_4655(arg0);
  return;
}

static int32_t script_47E6(int32_t arg0, int32_t arg1, int32_t arg2) {
  if (arg2 > 0) {
    SCNF_WaitFrames_0000(arg2);
  }
  if (arg1 == 0) {
    Script_charaPosRotUtils(arg0, 152, &data_27280, 0, 0);
    Script_charaPosRotUtils(arg0, 153, &data_2728C, 0, 0);
  } else {
    Script_charaPosRotUtils(arg0, 152, &data_2728C, 0, 0);
    Script_charaPosRotUtils(arg0, 153, &data_27280, 0, 0);
  }
  return Script_srchControl(0);
}

static void script_4858(int32_t arg0, int32_t arg1, int32_t arg2) {
  Script_scenCreateTask(script_47E6, 3, arg0, arg1, arg2);
  return;
}

static int32_t script_486C(int32_t arg0) {
  SCNF_WaitFrames_0000(73);
  SCNF_SetCashValue(-100, 1);
  arg0 = arg0 - 100;
  Script_SetStats(STATS_CASH, arg0);
  end:;
}

static int32_t script_4892(int32_t arg0) {
  SCNF_SetCashValue(100, 1);
  arg0 = arg0 + 100;
  Script_SetStats(STATS_CASH, arg0);
  end:;
}

static int32_t script_48B0(int32_t arg0, int32_t arg1) {
  if (arg1 > 0) {
    SCNF_WaitFrames_0000(arg1);
  }
  SCNF_StartIconSystemMenu(arg0);
  Script_GetCassetteFlags(arg0, 1, 0, 0);
  return Script_srchControl(0);
}

static void script_48DE(int32_t arg0, int32_t arg1) {
  Script_scenCreateTask(script_48B0, 2, arg0, arg1);
  return;
}

__attribute__((scn_string_bindings(0, data_27298)))
static void script_48F0(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int8_t local_FF;
  uint8_t unused_FC[3];
  int32_t local_F8;
  int32_t local_F4;
  int32_t unused_F0;
  GameDateTime local_E8;
  int32_t local_E4;
  local_FF = 0;
  local_F8 = Script_shenGetStats(STATS_CASH);
  Script_ReturnTimeDate(&local_E8);
  if (Script_scenUtils(SCENARIO_CMD_QUERY_FOCUS_OR_TIME_LIMIT) == 0) {
    if (Script_IsPlayerAllowed(0)) {
      if (local_F8 > 99) {
        script_4CB9(arg0);
        Script_scenCreateTask(script_486C, 1, local_F8);
        Script_scenCreateTask(script_4129, 2, arg2, "VEND");
        script_5282(arg0, arg1);
        script_4D94(arg0, arg1, arg2, arg3);
        local_F4 = script_4F3E(arg0, arg1, arg2, arg3, 1);
        local_E4 = local_F4;
        switch (local_E4) {
          case 13:
            if (task_D0 == 5) {
              script_5350(arg0, arg1, 1);
            } else {
              script_5641(arg0, arg1, 1);
            }
            break;
          case 16:
            local_F8 = Script_shenGetStats(STATS_CASH);
            Script_scenCreateTask(script_4892, 1, local_F8);
            break;
        }
        script_4E35(arg0, arg1, arg2, arg3);
      } else {
        if (Script_ryoControl(PCCTL_QueryShouldRun)) {
          local_FF = 1;
          Script_ryoControl(PCCTL_DisablePlayerControl);
        }
        script_3D8A(2, 0);
        if (local_FF == 1) {
          Script_ryoControl(RYO_RestoreControl);
        }
      }
    }
  }
  return;
}

static void script_4A21(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  script_4B5C(arg0, arg1, arg2, arg3);
  script_4F3E(arg0, arg1, arg2, arg3, 0);
  script_4E35(arg0, arg1, arg2, arg3);
  return;
}

__attribute__((scn_string_bindings(0, data_2729D)))
static void script_4A53(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  local_FC = Script_shenGetStats(STATS_CASH);
  if (Script_scenUtils(SCENARIO_CMD_GET_SCEN_FLAG100) == 0) {
    if (local_FC > 99) {
      script_4CB9(arg0);
      Script_scenCreateTask(script_486C, 1, local_FC);
      Script_scenCreateTask(script_4129, 2, arg2, "VEND");
      script_5282(arg0, arg1);
      script_4D94(arg0, arg1, arg2, arg3);
      local_F8 = script_4F3E(arg0, arg1, arg2, arg3, 1);
      local_F4 = local_F8;
      switch (local_F4) {
        case 13:
          script_5641(arg0, arg1, 1);
          break;
        case 16:
          local_FC = Script_shenGetStats(STATS_CASH);
          Script_scenCreateTask(script_4892, 1, local_FC);
          break;
      }
      script_4E35(arg0, arg1, arg2, arg3);
    }
  }
  return;
}

static void script_4B22(void) {
  task_C8 = 0x314E4143;
  task_D0 = 3;
  task_CC = (char *)data_272A2;
  task_DC = 0;
  task_DD = 0;
  task_DE = 0;
  task_E4 = 0;
  return;
}

__attribute__((scn_string_bindings(0, data_272A7, 1, data_272AE, 2, data_272BB)))
static void script_4B5C(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Vector3i local_F4;
  Vector3 local_E8;
  script_4B22();
  Script_ryoControl(PCCTL_UpdateRyoNavYaw, arg0);
  Script_charaSetRefboardFlagTo(arg0, 1);
  Script_uiUtils(0, 25, 0);
  Script_FaceEx(arg0, 1, 0, 2);
  task_D4 = Script_ryoControl(PCCTL_QueryShouldRun);
  if (task_D4) {
    Script_ryoControl(PCCTL_DisablePlayerControl);
    Script_PlayCharaMotion(arg0, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_scenUtils(SCENARIO_CMD_INIT_SCNT);
    Script_ccowUtils(arg0, 2);
    Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
    Script_CycleManagerUtils(CYCLE_CMD_PROCESS_SND);
  }
  Script_LoadVoiceFile(1, "01JUCE");
  task_E4 = Script_LoadHandMot(2, SHEN_NAME('VEND'), "HMOT_CAN.BIN");
  Script_scenCreateTask(script_4129, 2, arg2, "VEND");
  while (task_DE != 1) {
    SCNF_WaitFrames_0000(1);
  }
  Script_facemotUtils(arg3, 1, 0);
  Script_ResetHandMotion(arg3, 1, 1, 0);
  Script_charaDisplay(arg1, CDM_Hide);
  Script_ccowUtils(arg1, 2);
  Script_EVT_GetPosition(arg1, -1, &local_E8, ENTITY_XFORM_WORLD_SPACE);
  Script_mtwkGetSetRotation(arg1, &local_F4, 0x40000000);
  Script_EVT_SetPosition(arg2, 0x78000000, &local_E8);
  Script_EVT_SetRotation(arg2, 0x78000000, &local_F4, 0);
  Script_ResetHandMotion(arg0, 1, 1, 0);
  Script_ResetHandMotion(arg0, 1, 0, 0);
  script_4655(arg3);
  Script_InitMOTMGlobalState(arg0, arg2, arg3);
  return;
}

__attribute__((scn_string_bindings(0, data_272C0, 2, data_272DD)))
static void script_4CB9(int32_t arg0) {
  script_4B22();
  Script_SetCPAMFlags(arg0, 0);
  Script_FaceEx(arg0, 1, 0, 2);
  Script_charaSetRefboardFlagTo(arg0, 1);
  Script_uiUtils(0, 25, 0);
  task_D4 = Script_ryoControl(PCCTL_QueryShouldRun);
  if (task_D4) {
    Script_ryoControl(PCCTL_DisablePlayerControl);
    Script_PlayCharaMotion(arg0, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  }
  if (Script_scenUtils(SCENARIO_CMD_GET_SCEN_FLAG100) == 0) {
    Script_scenUtils(SCENARIO_CMD_INIT_SCNT);
  }
  Script_ccowUtils(arg0, 2);
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
  Script_CycleManagerUtils(CYCLE_CMD_PROCESS_SND);
  Script_LoadVoiceFile(1, "01JUCE");
  Script_sprintf("HandMot read by VEND\012");
  task_E4 = Script_LoadHandMot(2, SHEN_NAME('VEND'), "HMOT_CAN.BIN");
  Script_ResetHandMotion(arg0, 1, 1, 0);
  Script_ResetHandMotion(arg0, 1, 0, 0);
  return;
}

static void script_4D94(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Vector3 local_F4;
  Vector3i local_E8;
  while (task_DE != 1) {
    SCNF_WaitFrames_0000(1);
  }
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
  Script_facemotUtils(arg3, 1, 0);
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
  Script_ResetHandMotion(arg3, 1, 1, 0);
  script_4655(arg3);
  Script_charaDisplay(arg1, CDM_Hide);
  Script_ccowUtils(arg1, 2);
  Script_EVT_GetPosition(arg1, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  Script_mtwkGetSetRotation(arg1, &local_E8, 0x40000000);
  Script_EVT_SetPosition(arg2, 0x78000000, &local_F4);
  Script_EVT_SetRotation(arg2, 0x78000000, &local_E8, 0);
  Script_InitMOTMGlobalState(arg0, arg2, arg3);
  return;
}

__attribute__((scn_string_bindings(0, data_272EA, 1, data_272F1)))
static void script_4E35(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Vector3i local_F4;
  Script_charaSetRefboardFlagTo(arg3, 0);
  Script_ResetHandMotion(arg3, 0, 1, 0);
  Script_faceUpdateFor(arg3, 1, 0);
  SCNF_WaitFrames_0000(1);
  Script_ResetHandMotion(arg0, 0, 1, 0);
  Script_ResetHandMotion(arg0, 0, 0, 0);
  if (task_E4 != 0) {
    Script_FreeTaskData(task_E4);
  }
  Script_LoadVoiceFile(0, "01JUCE");
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 1);
  Script_ccowUtils(arg0, 1);
  Script_SetGameStateTo(AUTH_None);
  Script_scenUtils(SCENARIO_CMD_UPDATE_TIMER);
  Script_mtwkGetSetRotation(arg0, &local_F4, 0x2000000);
  Script_EVT_SetRotation(arg0, 0x78000000, &local_F4, 0);
  if (task_D4) {
    Script_PlayCharaMotion(arg0, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_ryoControl(RYO_RestoreControl);
  }
  SCNF_WaitFrames_0000(1);
  script_419E("VEND");
  Script_FaceEx(arg0, 0, 0, 2);
  Script_charaSetRefboardFlagTo(arg0, 2);
  Script_SetCPAMFlags(arg0, 1);
  task_DD = 14;
  Script_shenClearFreeState(1);
  return;
}

static int32_t script_4F3E(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  int32_t unused_E8[6];
  int32_t local_E4;
  int32_t local_E0;
  int32_t local_DC;
  int32_t local_D8;
  int8_t local_D7;
  uint8_t unused_D4[3];
  int32_t local_D0;
  local_DC = 0;
  local_D8 = 0;
  local_E4 = 1;
  local_E0 = -1;
  local_D7 = 0;
  Script_sicnUtils(0, 37, 1);
  local_D8 = Script_shenGetStats(STATS_CASH);
  local_D8 = local_D8 + 100;
  Script_SetStats(STATS_CASH, local_D8);
  local_D7 = script_3C48(0, 100, 0);
  local_D8 = Script_shenGetStats(STATS_CASH);
  local_D8 = local_D8 - 100;
  Script_SetStats(STATS_CASH, local_D8);
  if (local_D7 > 0) {
    Script_shenClearFreeState(0);
  }
  for (;;) {
    if (local_DC > 1) {
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_EV_STOP_ACTIVE) == 1) {
        if (local_E4 < 3) {
          Script_sprintf("vendmachine exit is EV_STOP\012");
          Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_EV_STOP_ACTIVE, 0);
          local_E4 = 16;
        }
      } else {
        if (local_E0 >= 1) {
          if (local_D7 > 0) {
            script_3C48(0, 100, 1);
            Script_sprintf("vendmachine exit is MONEY_LOCK -> %d,%d\012", local_D7, local_DC);
            local_E4 = 16;
          }
        }
      }
    }
    if (local_E0 != local_E4) {
      local_E0 = local_E4;
      task_DD = local_E4;
      local_D0 = local_E4;
      if (local_D0 == 1) {
        script_44EA(arg3, 1, (int32_t)data_272F8, -1);
        script_44A3(arg3, 1, 542, -1);
      } else if (local_D0 == 3) {
        task_DD = 1;
        Script_shenClearFreeState(0);
        Script_sicnUtils(1, 0, 0);
      } else if (local_D0 == 16) {
        Script_sicnUtils(1, 0, 0);
      } else if ((local_D0 == 2) | (local_D0 == 4)) {
        task_DD = 1;
      } else {
        switch (local_D0) {
          case 5:
            script_4246(0x3005A9, -1);
            break;
          case 6:
            script_4246(0x3405A9, 5);
            break;
          case 8:
            task_CC = (char *)data_2734A;
            task_D0 = 0;
            if (arg4 & 1) {
              script_46C0(task_C8, (int32_t)task_CC, arg4);
            }
            break;
          case 9:
            task_CC = (char *)data_2734F;
            task_D0 = 1;
            if (arg4 & 1) {
              script_46C0(task_C8, (int32_t)task_CC, arg4);
            }
            break;
          case 10:
            task_CC = (char *)data_27354;
            task_D0 = 2;
            if (arg4 & 1) {
              script_46C0(task_C8, (int32_t)task_CC, arg4);
            }
            break;
          case 11:
            task_CC = (char *)data_27359;
            task_D0 = 3;
            if (arg4 & 1) {
              script_46C0(task_C8, (int32_t)task_CC, arg4);
            }
            break;
          case 12:
            task_CC = (char *)data_2735E;
            task_D0 = 4;
            if (arg4 & 1) {
              script_46C0(task_C8, (int32_t)task_CC, arg4);
            }
            break;
        }
      }
    }
    local_E4 = Script_UpdateCharaSelectionScene(arg0, arg2, arg3, CHARA_SELECT_IDLE);
    if ((local_E0 == 13) | (local_E0 == 14) | (local_E0 == 15) | (local_E0 == 16)) {
      break;
    }
    local_DC = local_DC + 1;
    if (local_D7 < 1) {
      SCNF_WaitFrames_0000(1);
    }
  }
  Script_charaDisplay(arg1, CDM_Show);
  Script_charaDisplay(arg2, CDM_Hide);
  Script_ccowUtils(arg1, 1);
  Script_charaDisplay(arg3, CDM_Hide);
  return local_E0;
}

static void script_5282(int32_t arg0, int32_t arg1) {
  Script_shenClearFreeState(0);
  script_4596(arg0, arg1, (int32_t)&data_27364, -1);
  script_45FB(arg0, arg1, (int32_t)data_27370, -1);
  script_44EA(arg0, 1, (int32_t)data_2737C, -1);
  script_44EA(arg0, 0, (int32_t)data_2737C, -1);
  script_44A3(arg0, 1, 541, -1);
  script_4433(arg0, 712, 0, -0x80000000, -1);
  script_4246(0x3105A9, 125);
  script_463F(4013, arg1, arg1, -1);
  SCNF_WaitFrames_0000(10);
  script_4322(arg0, 0);
  Script_PlayCharaMotion(arg0, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_shenClearFreeState(1);
  return;
}

__attribute__((scn_string_bindings(3, data_273E5, 4, data_273F0)))
static void script_5350(int32_t arg0, int32_t arg1, int32_t arg2) {
  int32_t local_FC;
  if (arg2 == 0) {
    script_4129(task_C8, task_CC);
  }
  script_42CE(task_C8, arg0, 18, (int32_t)&data_27388, (int32_t)&data_27394, 82);
  script_4858(task_C8, 1, 0);
  script_4596(arg0, arg1, (int32_t)&data_273A0, -1);
  script_45FB(arg0, arg1, (int32_t)data_273AC, -1);
  script_44EA(arg0, 1, (int32_t)&data_273B8, -1);
  script_44A3(arg0, 1, 545, -1);
  script_44A3(arg0, 0, 546, -1);
  script_4433(arg0, 45059, 0, -0x80000000, -1);
  script_4246(0x3905A9, 80);
  script_463F(4019, arg1, arg1, -1);
  SCNF_WaitFrames_0000(10);
  script_4322(arg0, 0);
  local_FC = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f);
  if (local_FC == 0) {
    script_4289((int32_t)"01JUCEA011", -1);
  } else if (local_FC == 1) {
    script_4289((int32_t)"01JUCEA012", -1);
  } else {
    script_4289((int32_t)"01JUCEA013", -1);
  }
  SCNF_WaitFrames_0000(90);
  script_4531(arg0, 10, 5, 16, 180);
  script_4531(arg0, 9, 5, 16, 251);
  script_4531(arg0, 4, 5, 16, 351);
  script_4531(arg0, 1, 4, 16, 391);
  script_4531(arg0, 9, 5, 16, 426);
  script_4531(arg0, 10, 1, 16, 521);
  script_4531(arg0, 1, 2, 16, 545);
  script_4531(arg0, 0, 0, 16, 596);
  script_4246(0x3505A9, 108);
  script_4858(task_C8, 0, 108);
  script_444B(arg0, 45058, 0, 0x441C0000, 0, -1);
  script_4246(0x3205A9, 210);
  script_4246(0x3305A9, 330);
  script_4246(0x3205A9, 470);
  script_4246(0x3305A9, 500);
  script_4289((int32_t)"01JUCEA002", 550);
  script_4289((int32_t)"01JUCEA003", 400);
  script_463F(4015, arg1, arg1, -1);
  script_463F(4016, arg1, arg1, 575);
  SCNF_WaitFrames_0000(10);
  script_4322(arg0, 1);
  Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_16);
  script_4433(arg0, 45061, 0x42100000, 0, -1);
  script_4310(task_C8, 30);
  script_48DE(66, 32);
  SCNF_WaitFrames_0000(10);
  script_4322(arg0, 0);
  Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_5);
  script_419E(task_CC);
  Script_shenClearFreeState(1);
  return;
}

__attribute__((scn_string_bindings(0, data_2745C, 1, data_27467)))
static void script_5641(int32_t arg0, int32_t arg1, int32_t arg2) {
  if (arg2 == 0) {
    script_4129(task_C8, task_CC);
  }
  if (task_D0 == 4) {
    script_42CE(task_C8, arg0, 18, (int32_t)&data_27414, (int32_t)&data_27420, 82);
    script_4596(arg0, arg1, (int32_t)&data_2742C, -1);
    script_45FB(arg0, arg1, (int32_t)data_27438, -1);
    script_44EA(arg0, 1, (int32_t)data_27450, -1);
    script_44A3(arg0, 1, 544, -1);
  } else {
    script_42CE(task_C8, arg0, 18, (int32_t)&data_273FC, (int32_t)&data_27408, 82);
    script_4596(arg0, arg1, (int32_t)&data_2742C, -1);
    script_45FB(arg0, arg1, (int32_t)data_27438, -1);
    script_44EA(arg0, 1, (int32_t)&data_27444, -1);
    script_44A3(arg0, 1, 545, -1);
  }
  script_4858(task_C8, 1, 0);
  script_44A3(arg0, 0, 546, -1);
  script_4433(arg0, 45059, 0, -0x80000000, -1);
  script_4246(0x3905A9, 80);
  script_4246(0x3505A9, 290);
  script_463F(4014, arg1, arg1, -1);
  script_4858(task_C8, 0, 290);
  SCNF_WaitFrames_0000(10);
  script_4322(arg0, 0);
  script_4531(arg0, 10, 5, 16, 180);
  script_4531(arg0, 9, 5, 16, 251);
  script_4531(arg0, 4, 5, 16, 351);
  script_4531(arg0, 1, 4, 16, 391);
  script_4531(arg0, 9, 5, 16, 426);
  script_4531(arg0, 10, 1, 16, 521);
  script_4531(arg0, 1, 2, 16, 545);
  script_4531(arg0, 0, 0, 16, 596);
  if (task_D0 == 4) {
    script_4433(arg0, 45060, 0, 0, -1);
  } else {
    script_4433(arg0, 45058, 0, 0, -1);
  }
  script_4246(0x3205A9, 210);
  script_4246(0x3305A9, 330);
  script_4246(0x3205A9, 470);
  script_4246(0x3305A9, 500);
  script_4246(0x3805A9, 690);
  script_4289((int32_t)"01JUCEA002", 550);
  script_4289((int32_t)"01JUCEA003", 400);
  script_463F(4015, arg1, arg1, -1);
  script_463F(4016, arg1, arg1, 575);
  script_4310(task_C8, 690);
  SCNF_WaitFrames_0000(10);
  script_4322(arg0, 0);
  script_419E(task_CC);
  Script_shenClearFreeState(1);
  return;
}

static int32_t script_592B(int32_t arg0) {
  int8_t local_FF;
  int8_t local_FE;
  int8_t local_FD;
  int8_t local_FC;
  int8_t local_FB;
  int8_t local_FA;
  int8_t local_F9;
  int8_t local_F8;
  int32_t local_F4;
  int32_t local_F0;
  float local_90[8][3];
  Vector3 local_84;
  float local_FF24[8][3];
  int32_t local_FEC4[8][3];
  Vector3i local_FEB8;
  (void)((local_FF = 0) & 1);
  (void)((local_FE = 0) & 1);
  (void)((local_FD = 0) & 1);
  (void)((local_FC = 0) & 1);
  (void)((local_FB = 0) & 1);
  (void)((local_FA = 0) & 1);
  local_F9 = 0;
  local_F8 = 0;
  local_F4 = 0;
  while (local_F4 < 8) {
    if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, *(&data_27244 + local_F4))) {
      Script_EVT_GetPosition(*(&data_27244 + local_F4), -1, (Vector3 *)local_90[local_F4], ENTITY_XFORM_WORLD_SPACE);
      Script_mtwkGetSetRotation(*(&data_27244 + local_F4), (Vector3i *)local_FEC4[local_F4], 0x40000000);
      local_FF24[local_F4][0] = local_90[local_F4][0] + Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, local_FEC4[local_F4][1]) * 0.3f;
      local_FF24[local_F4][1] = local_90[local_F4][1];
      local_FF24[local_F4][2] = local_90[local_F4][2] + Script_MathUtils(SCRIPT_MATH_COS_FIXED16, local_FEC4[local_F4][1]) * 0.3f;
    }
    local_F4 = local_F4 + 1;
  }
  for (;;) {
    local_F9 = -1;
    (void)((local_FB = 0) & 2);
    Script_EVT_GetPosition(arg0, -1, &local_84, ENTITY_XFORM_WORLD_SPACE);
    Script_mtwkGetSetRotation(arg0, &local_FEB8, 0x42000000);
    local_84.x = (&local_84)->x + Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_FEB8)->y) * 0.3f;
    (&local_84)->z = (&local_84)->z + Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_FEB8)->y) * 0.3f;
    local_F0 = 0;
    while (local_F0 < 8) {
      if (Script_CheckTargetWithinAngleRange((const Vector3 *)local_FF24[local_F0], 1.4f, local_FEC4[local_F0][1] + 0x8000, 12743, &local_84) != 0) {
        if (Script_CheckTargetWithinAngleRange(&local_84, 1.4f, (&local_FEB8)->y, 5461, (const Vector3 *)local_FF24[local_F0]) != 0) {
          local_F9 = local_F0;
          break;
        }
      }
      local_F0 = local_F0 + 1;
    }
    if (Script_shenGetStats(STATS_CASH) < 100) {
      local_F8 = 1;
    } else {
      local_F8 = Script_ryoControl(PCCTL_GetTatehiza);
    }
    if (local_FA == -1) {
      if (Script_scenUtils(SCENARIO_CMD_GET_SCEN_FLAG100) == 0) {
        (void)((local_FA = 0) & 0x80);
      }
    }
    if ((local_F9 != -1) & (local_FA == 0)) {
      if (Script_GetControllerInput(1) & 0x400) {
        if (local_FB == 0) {
          (void)((local_FB = -1) & 0x80);
        }
        if (Script_scenUtils(SCENARIO_CMD_GET_SCEN_FLAG100)) {
          (void)((local_FA = -1) & 0x80);
        }
      } else {
        (void)((local_FB = 0) & 0x80);
      }
      if (local_FA != -1) {
        if (local_FD == 0) {
          Script_uiUtils(0, 25, 0);
          (void)((local_FD = -1) & 0x80);
        }
      } else {
        if (local_FD == -1) {
          Script_uiUtils(0, 0, 0);
        }
        (void)((local_FD = 0) & 0x80);
      }
      if (local_FE == 0) {
        Script_sicnUtils(0, 1, 1);
        (void)((local_FE = -1) & 0x80);
      }
      if (local_F8 == 0) {
        if (-(local_FB == -1) & -(local_FC == 0) & -(local_FA != -1)) {
          (void)((local_FC = -1) & 0x80);
        }
        if (local_FC == 0) {
          if (local_FF == 0) {
            SCNF_RequestIconNotification(0x100, 0, 0, 1);
            (void)((local_FF = -1) & 0x80);
          }
        } else {
          if (local_FF == -1) {
            SCNF_RequestIconNotification(0, 0, 0, 2);
          }
          (void)((local_FF = 0) & 0x80);
        }
      } else {
        if (local_FF == -1) {
          SCNF_RequestIconNotification(0, 0, 0, 2);
        }
        (void)((local_FF = 0) & 0x80);
        (void)((local_FC = 0) & 0x80);
      }
    } else {
      if (local_FE == -1) {
        Script_sicnUtils(1, 0, 0);
        (void)((local_FE = 0) & 0x80);
      }
      if (local_FD == -1) {
        Script_uiUtils(0, 0, 0);
        (void)((local_FD = 0) & 0x80);
      }
      if (local_FF == -1) {
        SCNF_RequestIconNotification(0, 0, 0, 2);
        (void)((local_FF = 0) & 0x80);
      }
    }
    if ((local_FB == -1) & (local_FC == -1)) {
      script_4A53(arg0, *(&data_27244 + local_F9), SHEN_NAME('JIHN'), SHEN_NAME('HAND'));
      (void)((local_FB = 0) & 0x80);
      (void)((local_FC = 0) & 0x80);
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_5D8F(int32_t arg0) {
  script_4B22();
  task_D8 = Script_scenCreateTask(script_592B, 1, arg0);
  return;
}

static void script_5DAA(void) {
  Script_srchControl((void *)task_D8);
  task_D8 = 0;
  return;
}

static void script_5DBB(void) {
  (void)task_DD;
  return;
}

static void script_5DC2(void) {
  Script_ExitCharaSelector();
  task_DC = 1;
  return;
}

static int32_t script_5DCF(int32_t arg0) {
  if (arg0 < 0) {
    return -arg0;
  } else {
    return arg0;
  }
}

static void script_5DED(float arg0) {
  if (!(arg0 >= 0.0f)) {
    (void)((float)0 - arg0);
    return;
  } else {
    (void)arg0;
    return;
  }
}

static int32_t script_5E0C(int32_t arg0, int32_t arg1) {
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

static void script_5E84(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  int32_t local_FC;
  if (task_248 != arg0) {
    Script_sprintf("Door Index = %d : call_type = %d : type = %d\012", task_130[arg2], arg3, arg4);
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

static int32_t script_6436(int32_t arg0) {
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

static void script_6507(int32_t arg0, int32_t arg1, int32_t arg2) {
  script_F248(arg0, arg2, 0);
  return;
}

static void script_6517(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  float local_F8;
  float local_F4;
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
    local_FC = script_E9DD(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
    if ((local_FC == 1) | (local_FC == 2)) {
      local_FC = 1;
    } else {
      local_FC = 0;
    }
    *(int32_t *)&local_F8 = Script_ScenDataUtil(data_27A70, 5, Script_ScenDataUtil(data_27480, 1, arg3 * 40 + task_180[arg2] * 4 + local_FC) * 9 + 5);
    *(int32_t *)&local_F4 = Script_ScenDataUtil(data_27A70, 5, Script_ScenDataUtil(data_27480, 1, arg3 * 40 + task_180[arg2] * 4 + local_FC) * 9 + 6);
    local_FC = script_E9DD(arg0, arg1, 5, local_F8 / 100.0f, local_F4 / 100.0f, arg3, -1);
    script_9CA3(arg0, arg1, arg2, arg3, local_FC, 0);
  }
  script_F248(arg0, arg2, 0);
  return;
}

static void script_661B(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  int32_t local_F8;
  float local_F4;
  float local_F0;
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
    local_FC = script_E9DD(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
    *(int32_t *)&local_F4 = Script_ScenDataUtil(data_27A70, 5, Script_ScenDataUtil(data_27480, 1, arg3 * 40 + task_180[arg2] * 4 + local_FC) * 9 + 5);
    local_F4 = 90.0f - local_F4;
    *(int32_t *)&local_F0 = Script_ScenDataUtil(data_27A70, 5, Script_ScenDataUtil(data_27480, 1, arg3 * 40 + task_180[arg2] * 4 + local_FC) * 9 + 6);
    if ((Script_ScenDataUtil(task_E8, 4, task_130[arg2] * 13 + 1) & 0x40) | (Script_ScenDataUtil(task_E8, 4, task_130[arg2] * 13 + 1) & 0x80)) {
      if (Script_ScenDataUtil(task_E8, 4, task_130[arg2] * 13 + 1) & 0x40) {
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
    local_FC = script_E9DD(arg0, arg1, local_F8, local_F4 / (float)100, local_F0 / (float)100, arg3, task_130[arg2]);
    script_9CA3(arg0, arg1, arg2, arg3, local_FC, 1);
  }
  script_F248(arg0, arg2, 0);
  return;
}

static void script_67BD(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  float local_F8;
  float local_F4;
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
    local_FC = script_E9DD(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
    if ((local_FC == 1) | (local_FC == 2)) {
      local_FC = 1;
    } else {
      local_FC = 0;
    }
    *(int32_t *)&local_F8 = Script_ScenDataUtil(data_27A70, 5, Script_ScenDataUtil(data_27480, 1, arg3 * 40 + task_180[arg2] * 4 + local_FC) * 9 + 5);
    *(int32_t *)&local_F4 = Script_ScenDataUtil(data_27A70, 5, Script_ScenDataUtil(data_27480, 1, arg3 * 40 + task_180[arg2] * 4 + local_FC) * 9 + 6);
    local_FC = script_E9DD(arg0, arg1, 5, local_F8 / 100.0f, local_F4 / 100.0f, arg3, -1);
    if (local_FC == 1) {
      local_FC = 2;
    } else {
      local_FC = 3;
    }
    if ((Script_ScenDataUtil(task_E8, 4, task_130[arg2] * 13 + 1) & 4) == 0) {
      script_81B0(arg0, arg1, arg2, arg3, local_FC, 0);
    } else {
      if (arg0 == task_248) {
        script_B5A1(arg0, arg1, arg2, arg3, local_FC, 0);
      }
    }
  }
  script_F248(arg0, arg2, 2);
  return;
}

static void script_691E(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  float local_F8;
  float local_F4;
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
    local_FC = script_E9DD(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
    if ((local_FC == 1) | (local_FC == 2)) {
      local_FC = 1;
    } else {
      local_FC = 0;
    }
    *(int32_t *)&local_F8 = Script_ScenDataUtil(data_27A70, 5, Script_ScenDataUtil(data_27480, 1, arg3 * 40 + task_180[arg2] * 4 + local_FC) * 9 + 5);
    *(int32_t *)&local_F4 = Script_ScenDataUtil(data_27A70, 5, Script_ScenDataUtil(data_27480, 1, arg3 * 40 + task_180[arg2] * 4 + local_FC) * 9 + 6);
    local_FC = script_E9DD(arg0, arg1, 5, local_F8 / 100.0f, local_F4 / 100.0f, arg3, -1);
    if (local_FC == 1) {
      local_FC = 1;
    } else {
      local_FC = 0;
    }
    if ((Script_ScenDataUtil(task_E8, 4, task_130[arg2] * 13 + 1) & 4) == 0) {
      script_81B0(arg0, arg1, arg2, arg3, local_FC, 0);
    } else {
      if (arg0 == task_248) {
        script_B5A1(arg0, arg1, arg2, arg3, local_FC, 0);
      }
    }
  }
  script_F248(arg0, arg2, 2);
  return;
}

static void script_6A7F(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  int32_t local_F8;
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
    if ((Script_ScenDataUtil(task_E8, 4, task_130[arg2] * 13 + 1) & 4) == 0) {
      float local_F4;
      float local_F0;
      local_FC = script_E9DD(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
      *(int32_t *)&local_F4 = Script_ScenDataUtil(data_27A70, 5, Script_ScenDataUtil(data_27480, 1, arg3 * 40 + task_180[arg2] * 4 + local_FC) * 9 + 5);
      *(int32_t *)&local_F0 = Script_ScenDataUtil(data_27A70, 5, Script_ScenDataUtil(data_27480, 1, arg3 * 40 + task_180[arg2] * 4 + local_FC) * 9 + 6);
      if ((Script_ScenDataUtil(task_E8, 4, task_130[arg2] * 13 + 1) & 0x40) | (Script_ScenDataUtil(task_E8, 4, task_130[arg2] * 13 + 1) & 0x80)) {
        if (Script_ScenDataUtil(task_E8, 4, task_130[arg2] * 13 + 1) & 0x40) {
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
      local_FC = script_E9DD(arg0, arg1, local_F8, local_F4 / 100.0f, local_F0 / 100.0f, arg3, task_130[arg2]);
      script_81B0(arg0, arg1, arg2, arg3, local_FC, 1);
    } else {
      if (arg0 == task_248) {
        float local_F4;
        float local_F0;
        local_FC = script_E9DD(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
        *(int32_t *)&local_F4 = Script_ScenDataUtil(data_27A70, 5, Script_ScenDataUtil(data_27480, 1, arg3 * 40 + task_180[arg2] * 4 + local_FC) * 9 + 5);
        *(int32_t *)&local_F0 = Script_ScenDataUtil(data_27A70, 5, Script_ScenDataUtil(data_27480, 1, arg3 * 40 + task_180[arg2] * 4 + local_FC) * 9 + 6);
        local_FC = script_E9DD(arg0, arg1, 6, local_F4 / 100.0f, local_F0 / 100.0f, arg3, task_130[arg2]);
        script_B5A1(arg0, arg1, arg2, arg3, local_FC, 1);
      }
    }
  }
  script_F248(arg0, arg2, 2);
  return;
}

static void script_6D1D(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  float local_F8;
  float local_F4;
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
    local_FC = script_E9DD(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
    *(int32_t *)&local_F8 = Script_ScenDataUtil(data_27A70, 5, Script_ScenDataUtil(data_27480, 1, arg3 * 40 + task_180[arg2] * 4 + local_FC) * 9 + 5);
    *(int32_t *)&local_F4 = Script_ScenDataUtil(data_27A70, 5, Script_ScenDataUtil(data_27480, 1, arg3 * 40 + task_180[arg2] * 4 + local_FC) * 9 + 6);
    script_E9DD(arg0, arg1, 0, local_F8 / 100.0f, local_F4 / 100.0f, arg3, -1);
  }
  script_F248(arg0, arg2, 0);
  return;
}

static void script_6DEB(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
    local_FC = script_E9DD(arg0, arg1, 4, 0.5f, 0.5f, arg3, -1);
    if (local_FC == 1) {
      local_FC = 2;
    } else {
      local_FC = 3;
    }
    script_912F(arg0, arg1, arg2, arg3, local_FC, 1);
  }
  script_F248(arg0, arg2, 1);
  return;
}

static void script_6E52(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
    local_FC = script_E9DD(arg0, arg1, 4, 0.5f, 0.5f, arg3, -1);
    if (local_FC == 1) {
      local_FC = 1;
    } else {
      local_FC = 0;
    }
    script_912F(arg0, arg1, arg2, arg3, local_FC, 1);
  }
  script_F248(arg0, arg2, 1);
  return;
}

static void script_6EB9(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
    script_E9DD(arg0, arg1, 4, 0.0f, 0.5f, arg3, -1);
    script_912F(arg0, arg1, arg2, arg3, 4, 1);
  }
  script_F248(arg0, arg2, 1);
  return;
}

static void script_6F00(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  float local_F8;
  float local_F4;
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
    local_FC = script_E9DD(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
    if ((local_FC == 1) | (local_FC == 2)) {
      local_FC = 1;
    } else {
      local_FC = 0;
    }
    *(int32_t *)&local_F8 = Script_ScenDataUtil(data_27A70, 5, Script_ScenDataUtil(data_27480, 1, arg3 * 40 + task_180[arg2] * 4 + local_FC) * 9 + 5);
    *(int32_t *)&local_F4 = Script_ScenDataUtil(data_27A70, 5, Script_ScenDataUtil(data_27480, 1, arg3 * 40 + task_180[arg2] * 4 + local_FC) * 9 + 6);
    if (local_FC == 0) {
      local_FC = script_E9DD(arg0, arg1, 5, local_F8 / 100.0f, local_F4 / 100.0f, arg3, -1);
    } else {
      local_FC = script_E9DD(arg0, arg1, 5, local_F8 / 100.0f, local_F4 / 100.0f, arg3, -1);
    }
    script_95E9(arg0, arg1, arg2, arg3, local_FC, 0);
  }
  script_F248(arg0, arg2, 2);
  return;
}

static void script_7036(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
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
  if (arg0 == task_248) {
    if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
      local_FF7F = script_E9DD(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
      *(int32_t *)&local_FF78 = Script_ScenDataUtil(data_27A70, 5, Script_ScenDataUtil(data_27480, 1, arg3 * 40 + task_180[arg2] * 4 + local_FF7F) * 9 + 5);
      *(int32_t *)&local_FF74 = Script_ScenDataUtil(data_27A70, 5, Script_ScenDataUtil(data_27480, 1, arg3 * 40 + task_180[arg2] * 4 + local_FF7F) * 9 + 6);
      local_FF7F = script_E9DD(arg0, arg1, 5, local_FF78 / (float)100, local_FF74 / (float)100, arg3, -1);
      script_F0A6(arg0, 0);
      *(int32_t *)&local_94 = Script_ScenDataUtil(data_27A70, 5, Script_ScenDataUtil(data_27480, 1, arg3 * 40 + task_180[arg2] * 4 + local_FF7F) * 9 + 1);
      *(int32_t *)&local_90 = Script_ScenDataUtil(data_27A70, 5, Script_ScenDataUtil(data_27480, 1, arg3 * 40 + task_180[arg2] * 4 + local_FF7F) * 9 + 2);
      local_8C = 0.0f;
      local_FC = Script_ScenDataUtil(data_27A70, 4, Script_ScenDataUtil(data_27480, 1, arg3 * 40 + task_180[arg2] * 4 + local_FF7F) * 9);
      local_FC = local_FC | ((Script_FloatUtils(ScriptMathOp_FloatToInt, local_94 - 3.0f) << 16) & 0x7FFF0000);
      local_9C.x = (&task_250)->x;
      (&local_9C)->y = task_250.y;
      (&local_9C)->z = task_250.z;
      Script_charaPosRotUtils(arg1, 12, &local_CC, &local_E4, 2);
      Script_charaPosRotUtils(arg1, 7, &local_C0, &local_D8, 2);
      Script_ryoControl(PCCTL_StartCameraFacingInterpolation, 0);
      if (arg0 == task_248) {
        Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
        Script_scenCharaSubmitMoveTargetRequest(arg0, &task_250, *(&task_25C + 1), local_FC, 0);
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
        Script_SetCharaMoveTarget(arg0, &task_250, *(&task_25C + 1), local_FC);
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
              local_FF6C = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_130[arg2], 2);
              local_FF6C = Script_ScenDataUtil(task_F0, 4, Script_ScenDataUtil(task_F4, 4, local_FF6C * 9 + 1) * 4 + 2);
              if (local_FF6C != -1) {
                if (arg0 == task_248) {
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
              local_FF6C = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_130[arg2], 2);
              local_FF6C = Script_ScenDataUtil(task_F0, 4, Script_ScenDataUtil(task_F4, 4, local_FF6C * 9 + 1) * 4 + 2);
              if (local_FF6C != -1) {
                if (arg0 == task_248) {
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
      script_F0A6(arg0, 1);
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
        local_FF6C = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_130[arg2], 2);
        local_FF6C = Script_ScenDataUtil(task_F0, 4, Script_ScenDataUtil(task_F4, 4, local_FF6C * 9 + 1) * 4 + 3);
        if (local_FF6C != -1) {
          if (arg0 == task_248) {
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
    script_F248(arg0, arg2, 0);
  } else {
    script_661B(arg0, arg1, arg2, arg3);
  }
  return;
}

static void script_78F7(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  float local_F8;
  int32_t local_F4;
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
    local_FC = script_E9DD(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
    *(int32_t *)&local_F8 = Script_ScenDataUtil(data_27A70, 5, Script_ScenDataUtil(data_27480, 1, arg3 * 40 + task_180[arg2] * 4 + local_FC) * 9 + 5);
    local_F8 = 98.0f - local_F8;
    local_F4 = Script_ScenDataUtil(data_27A70, 5, Script_ScenDataUtil(data_27480, 1, arg3 * 40 + task_180[arg2] * 4 + local_FC) * 9 + 6);
    if ((local_FC == 0) | (local_FC == 3)) {
      local_FC = script_E9DD(arg0, arg1, 4, 0.34736f, 0.73184f, arg3, -1);
    } else {
      local_FC = script_E9DD(arg0, arg1, 4, 0.4f, 0.712f, arg3, -1);
    }
    script_BCC2(arg0, arg1, arg2, arg3, local_FC, 0);
  }
  script_F248(arg0, arg2, 0);
  return;
}

static int32_t script_7A15(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
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
    local_F4 = Script_ScenDataUtil(task_E8, 4, task_130[arg2] * 13 + 6);
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
        local_FC = script_E9DD(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
        local_FC = script_E9DD(arg0, arg1, 7, 0.45f, 0.0f, arg3, -1);
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
              goto L007BD1;
            }
          }
          L007BD1:;
        } else {
          local_D4 = 0.45f;
        }
        Script_EVT_GetPosition(arg1, -1, &local_E4, 0);
        Script_mtwkGetSetRotation(arg1, &local_D8, 0);
        local_E4.x = (&local_E4)->x + local_D4 * Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_D8)->y & 0xFFFF);
        (&local_E4)->z = (&local_E4)->z - local_D4 * Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_D8)->y & 0xFFFF);
        *(&task_25C + 1) = Script_FloatUtils(ScriptMathOp_Atan2ReverseVector, (&local_E4)->x - (&task_250)->x, (&local_E4)->z - task_250.z);
        script_C44C(arg0, arg1, arg2, arg3, local_FC, local_F0);
      }
    } else if ((local_BC == 3) | (local_BC == 4) | (local_BC == 5) | (local_BC == 6)) {
      *(int32_t *)&local_D0 = Script_ScenDataUtil(&data_28490, 5, Script_ScenDataUtil(task_E8, 4, task_130[arg2] * 13 + 6) * 5 + 3);
      *(int32_t *)&local_CC = Script_ScenDataUtil(&data_28490, 5, Script_ScenDataUtil(task_E8, 4, task_130[arg2] * 13 + 6) * 5 + 4);
      local_FC = script_E9DD(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
      local_BC = arg3;
      if ((local_BC == 13) | (local_BC == 14) | (local_BC == 15) | (local_BC == 16) | (local_BC == 20) | (local_BC == 23) | (local_BC == 28) | (local_BC == 29)) {
        if ((Script_ScenDataUtil(task_E8, 4, task_130[arg2] * 13 + 1) & 0x40) | (Script_ScenDataUtil(task_E8, 4, task_130[arg2] * 13 + 1) & 0x80)) {
          if (Script_ScenDataUtil(task_E8, 4, task_130[arg2] * 13 + 1) & 0x40) {
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
      local_FC = script_E9DD(arg0, arg1, local_F8, local_D0 / (float)100, local_CC / (float)100, arg3, task_130[arg2]);
      script_C524(arg0, arg1, arg2, arg3, local_FC, 0);
    } else {
      if (local_BC == 7) {
        local_FC = script_E9DD(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
        local_BC = arg3;
        if ((local_BC == 13) | (local_BC == 14) | (local_BC == 15) | (local_BC == 16) | (local_BC == 20) | (local_BC == 23) | (local_BC == 28) | (local_BC == 29)) {
          if ((Script_ScenDataUtil(task_E8, 4, task_130[arg2] * 13 + 1) & 0x40) | (Script_ScenDataUtil(task_E8, 4, task_130[arg2] * 13 + 1) & 0x80)) {
            if (Script_ScenDataUtil(task_E8, 4, task_130[arg2] * 13 + 1) & 0x40) {
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
        if (Script_CycleManagerUtils(CYCLE_CMD_IS_DOOR_OR_MOBJ_TYPE2, Script_ScenDataUtil(task_E8, 4, task_130[arg2] * 13 + 12)) == 1) {
          *(int32_t *)&local_D0 = Script_ScenDataUtil(&data_28490, 5, Script_ScenDataUtil(task_E8, 4, task_130[arg2] * 13 + 6) * 5 + 3);
          *(int32_t *)&local_CC = Script_ScenDataUtil(&data_28490, 5, Script_ScenDataUtil(task_E8, 4, task_130[arg2] * 13 + 6) * 5 + 4);
          local_FC = script_E9DD(arg0, arg1, local_F8, local_D0 / (float)100, local_CC / (float)100, arg3, task_130[arg2]);
          Script_scenUtils(SCENARIO_CMD_INIT_SCNT);
          script_C99B(arg0, arg1, arg2, arg3, local_FC, 0);
          Script_scenUtils(SCENARIO_CMD_UPDATE_TIMER);
        } else {
          *(int32_t *)&local_D0 = Script_ScenDataUtil(&data_28490, 5, Script_ScenDataUtil(task_E8, 4, task_130[arg2] * 13 + 6) * 5 + 3);
          *(int32_t *)&local_CC = Script_ScenDataUtil(&data_28490, 5, Script_ScenDataUtil(task_E8, 4, task_130[arg2] * 13 + 6) * 5 + 4);
          local_FC = script_E9DD(arg0, arg1, local_F8, local_D0 / (float)100, local_CC / (float)100, arg3, task_130[arg2]);
          script_C524(arg0, arg1, arg2, arg3, local_FC, 0);
        }
        goto L0080CC;
      }
    }
  }
  L0080CC:;
  script_F248(arg0, arg2, 0);
  end:;
}

static void script_80DA(int32_t arg0, int32_t arg1) {
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
    Script_SetCharaMoveTarget(arg0, &local_F4, (&local_E8)->y, script_6436(arg1));
  }
  return;
}

__attribute__((scn_string_bindings(0, data_28FAE)))
static void script_81B0(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
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
  *(int32_t *)&local_98 = Script_ScenDataUtil(data_27A70, 5, Script_ScenDataUtil(data_27480, 1, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9 + 1);
  *(int32_t *)&local_94 = Script_ScenDataUtil(data_27A70, 5, Script_ScenDataUtil(data_27480, 1, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9 + 2);
  *(int32_t *)&local_90 = Script_ScenDataUtil(data_27A70, 5, Script_ScenDataUtil(data_27480, 1, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9 + 3);
  *(int32_t *)&local_8C = Script_ScenDataUtil(data_27A70, 5, Script_ScenDataUtil(data_27480, 1, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9 + 4);
  *(int32_t *)&local_88 = Script_ScenDataUtil(data_27A70, 5, Script_ScenDataUtil(data_27480, 1, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9 + 7);
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
      local_FF78 = Script_ScenDataUtil(data_27A70, 4, Script_ScenDataUtil(data_27480, 1, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9);
      local_FF7C = 18;
      local_80 = 18;
      local_A4 = 1.0f;
    } else {
      if (arg0 == task_248) {
        local_FF78 = Script_ScenDataUtil(data_27A70, 4, Script_ScenDataUtil(data_27480, 1, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9) | -0x80000000;
        local_FF7C = 12;
        local_80 = 12;
      } else {
        local_FF78 = Script_ScenDataUtil(data_27A70, 4, Script_ScenDataUtil(data_27480, 1, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9);
        local_FF7C = 18;
        local_80 = 18;
      }
      local_A4 = -1.0f;
    }
    script_5E84(arg0, local_FF78, arg2, arg3, arg4);
    local_FF78 = local_FF78 | ((Script_FloatUtils(ScriptMathOp_FloatToInt, local_98 - 3.0f) << 16) & 0x7FFF0000);
    script_F0A6(arg0, 0);
    local_C0.x = (&task_250)->x;
    (&local_C0)->y = task_250.y;
    (&local_C0)->z = task_250.z;
    Script_ryoControl(PCCTL_StartCameraFacingInterpolation, 0);
    Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
    Script_SetCharaMoveTarget(arg0, &task_250, *(&task_25C + 1), local_FF78);
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
    if (task_248 == arg0) {
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
          script_80DA(arg0, task_180[arg2]);
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
            local_FF64 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_130[arg2], 2);
            local_FF64 = Script_ScenDataUtil(task_F0, 4, Script_ScenDataUtil(task_F4, 4, local_FF64 * 9 + 1) * 4 + 2);
            if (local_FF64 != -1) {
              if (arg0 == task_248) {
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
                goto L008738;
              }
            }
            L008738:;
          } else {
            if ((local_FF64 == 1) | (local_FF64 == 3)) {
              if (!(local_B4 > 0.0f)) {
                local_B4 = 0.0f;
              } else {
                if (!(local_B4 < 0.75f)) {
                  local_B4 = 0.75f;
                  goto L008776;
                }
              }
              L008776:;
              goto L008779;
            }
          }
        }
      }
      L008779:;
      if (task_248 == arg0) {
        if (!(local_B8 <= local_94) & !(local_B8 >= local_90)) {
          local_86 = 0;
        }
      } else {
        if (!(local_B8 <= local_94) & !(local_B8 > local_88)) {
          *(int32_t *)&local_9C = Script_ScenDataUtil(data_27A70, 5, Script_ScenDataUtil(data_27480, 1, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9 + 8);
          local_B4 = local_B4 + local_9C * local_A4;
        }
      }
      if (!(local_B8 < local_90) & !(local_B8 >= local_90 + 1.5f)) {
        if (local_84 == 1) {
          int32_t local_FF64;
          local_FF64 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_130[arg2], 2);
          local_FF64 = Script_ScenDataUtil(task_F0, 4, Script_ScenDataUtil(task_F4, 4, local_FF64 * 9 + 1) * 4 + 3);
          if (local_FF64 != -1) {
            if (arg0 == task_248) {
              Script_executeAicaCommand(local_FF64, 0, 0);
            } else {
              Script_Execute3DSoundCommand_Wrapper(local_FF64, 4, &local_C0, 5.0f, 20.0f, 0);
            }
          }
          local_84 = 2;
        }
        if (task_248 != arg0) {
          if (local_FF74 == 0) {
            local_AC = local_B4 / (local_8C - local_90);
            local_FF74 = 1;
          }
        }
      }
      if (!(local_B8 < local_90) & !(local_B8 > local_8C)) {
        Script_EVT_GetPosition(arg0, local_FF7C, &local_F0, ENTITY_XFORM_MOTION_SPACE);
        if (task_248 == arg0) {
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
                goto L008945;
              }
            }
            L008945:;
          } else {
            if ((local_FF64 == 1) | (local_FF64 == 3)) {
              if (!(local_B4 < 0.75f)) {
                local_B4 = 0.75f;
              } else {
                if (!(local_B4 > 0.0f)) {
                  local_B4 = 0.0f;
                  goto L008983;
                }
              }
              L008983:;
              goto L008986;
            }
          }
          L008986:;
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
              goto L0089DF;
            }
          }
        }
      }
      L0089DF:;
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
      if (arg0 == task_248) {
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
  script_F0A6(arg0, 1);
  {
    int32_t local_FF78;
    int32_t unused_FF68[4];
    int32_t local_FF64;
    local_FF78 = task_130[arg2];
    local_FF64 = arg3;
    if ((local_FF64 == 9) | (local_FF64 == 10) | (local_FF64 == 15)) {
      if (arg5) {
        Script_charaPosRotUtils(arg1, 12, &local_D8, 0, 0);
        Script_charaPosRotUtils(arg1, 7, &local_CC, 0, 0);
      } else {
        Script_EVT_SetPosition(arg1, POS_XYZ, &local_D8);
      }
      if (task_248 == arg0) {
        local_80 = (Script_ScenDataUtil(task_E8, 4, local_FF78 * 13 + 2) >> 16) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_80, 1);
        local_80 = Script_ScenDataUtil(task_E8, 4, local_FF78 * 13 + 2) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_80, 1);
      }
    } else if ((local_FF64 == 18) | (local_FF64 == 19) | (local_FF64 == 21) | (local_FF64 == 22)) {
      if (task_248 == arg0) {
        local_80 = (Script_ScenDataUtil(task_E8, 4, local_FF78 * 13 + 2) >> 16) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_80, 1);
        {
          int32_t local_FF60;
          local_FF60 = Script_ScenDataUtil(task_E8, 4, local_FF78 * 13 + 1);
          local_FF60 = local_FF60 | 8;
          Script_ScenDataUtil(task_E8, 14, local_FF78 * 13 + 1, local_FF60);
        }
        {
          int32_t local_FF60;
          local_FF60 = Script_ScenDataUtil(task_E8, 4, local_FF78 * 13 + 1);
          local_FF60 = local_FF60 | 0x10;
          Script_ScenDataUtil(task_E8, 14, local_FF78 * 13 + 1, local_FF60);
        }
        {
          int32_t local_FF60;
          local_FF60 = Script_ScenDataUtil(task_E8, 4, local_FF78 * 13 + 1);
          local_FF60 = local_FF60 | 2;
          Script_ScenDataUtil(task_E8, 14, local_FF78 * 13 + 1, local_FF60);
        }
      } else {
        if (arg5) {
          Script_charaPosRotUtils(arg1, 12, &local_D8, 0, 0);
          Script_charaPosRotUtils(arg1, 7, &local_CC, 0, 0);
        } else {
          Script_EVT_SetPosition(arg1, POS_XYZ, &local_D8);
        }
        local_80 = (Script_ScenDataUtil(task_E8, 4, local_FF78 * 13 + 2) >> 16) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_80, 1);
        local_80 = Script_ScenDataUtil(task_E8, 4, local_FF78 * 13 + 2) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_80, 1);
      }
    } else {
      if ((local_FF64 == 20) | (local_FF64 == 23)) {
        if (task_248 == arg0) {
          if ((arg4 == 0) | (arg4 == 1)) {
            {
              int32_t local_FF60;
              local_FF60 = Script_ScenDataUtil(task_E8, 4, local_FF78 * 13 + 1);
              local_FF60 = local_FF60 | 0x10;
              Script_ScenDataUtil(task_E8, 14, local_FF78 * 13 + 1, local_FF60);
            }
            {
              int32_t local_FF60;
              local_FF60 = Script_ScenDataUtil(task_E8, 4, local_FF78 * 13 + 1);
              local_FF60 = local_FF60 | 4;
              Script_ScenDataUtil(task_E8, 14, local_FF78 * 13 + 1, local_FF60);
            }
          } else {
            {
              int32_t local_FF60;
              local_FF60 = Script_ScenDataUtil(task_E8, 4, local_FF78 * 13 + 1);
              local_FF60 = local_FF60 | 8;
              Script_ScenDataUtil(task_E8, 14, local_FF78 * 13 + 1, local_FF60);
            }
            {
              int32_t local_FF60;
              local_FF60 = Script_ScenDataUtil(task_E8, 4, local_FF78 * 13 + 1);
              local_FF60 = local_FF60 | 4;
              Script_ScenDataUtil(task_E8, 14, local_FF78 * 13 + 1, local_FF60);
            }
          }
        } else {
          if (arg5) {
            Script_charaPosRotUtils(arg1, 12, &local_D8, 0, 0);
            Script_charaPosRotUtils(arg1, 7, &local_CC, 0, 0);
          } else {
            Script_EVT_SetPosition(arg1, POS_XYZ, &local_D8);
          }
          local_80 = (Script_ScenDataUtil(task_E8, 4, local_FF78 * 13 + 2) >> 16) & 0xFFFF;
          Script_scenToggleCharaFlag(0, local_80, 1);
          local_80 = Script_ScenDataUtil(task_E8, 4, local_FF78 * 13 + 2) & 0xFFFF;
          Script_scenToggleCharaFlag(0, local_80, 1);
        }
        goto L0090C2;
      }
    }
  }
  L0090C2:;
  return;
}

static void script_90C3(int32_t arg0, int32_t arg1, int32_t arg2) {
  int32_t local_FC;
  if (task_248 == arg0) {
    local_FC = task_130[arg1];
    {
      int32_t local_F8;
      local_F8 = Script_ScenDataUtil(task_E8, 4, local_FC * 13 + 2) & 0xFFFF;
      Script_scenToggleCharaFlag(0, local_F8, arg2);
    }
    {
      int32_t local_F8;
      local_F8 = (Script_ScenDataUtil(task_E8, 4, local_FC * 13 + 2) >> 16) & 0xFFFF;
      Script_scenToggleCharaFlag(0, local_F8, arg2);
    }
  }
  return;
}

static void script_912F(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
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
  local_AC.x = (&task_250)->x;
  (&local_AC)->y = task_250.y;
  (&local_AC)->z = task_250.z;
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
    local_80 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_130[arg2], 2);
    local_80 = Script_ScenDataUtil(task_F0, 4, Script_ScenDataUtil(task_F4, 4, local_80 * 9 + 1) * 4 + 2);
    if (local_80 != -1) {
      if (arg0 == task_248) {
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
        task_158[arg2] = arg0;
        local_98 = -local_98;
        local_84 = 0;
      }
      local_8C = 0;
    } else {
      if ((local_93 == 1) & (local_8C == 0)) {
        int32_t local_80;
        local_80 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_130[arg2], 2);
        local_80 = Script_ScenDataUtil(task_F0, 4, Script_ScenDataUtil(task_F4, 4, local_80 * 9 + 1) * 4 + 3);
        if (local_80 != -1) {
          if (arg0 == task_248) {
            Script_executeAicaCommand(local_80, 0, 0);
          } else {
            Script_Execute3DSoundCommand_Wrapper(local_80, 4, &local_AC, 5.0f, 20.0f, 0);
          }
        }
        local_93 = 0;
        local_8C = 1;
        local_98 = -local_98;
        local_84 = 0;
        task_158[arg2] = 0;
      }
    }
    local_88 = task_130[arg2];
    local_88 = Script_ScenDataUtil(task_E8, 4, local_88 * 13 + 2) & 0xFFFF;
    local_88 = local_88 + ((local_9C - 0x4000) >> 12);
    if (local_84 != local_88) {
      Script_scenToggleCharaFlag(0, local_88, local_8C);
      local_84 = local_88;
    }
    local_9C = local_9C - local_98;
    if (local_9C >= 0xC000) {
      local_9C = 0xC000;
      local_88 = task_130[arg2];
      local_88 = Script_ScenDataUtil(task_E8, 4, local_88 * 13 + 2) & 0xFFFF;
      local_88 = local_88 + 8;
      Script_scenToggleCharaFlag(0, local_88, local_8C);
      break;
    }
    if (local_9C <= 0x4000) {
      local_9C = 0x4000;
      if (local_93 == 0) {
        task_158[arg2] = 0;
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
          goto L009599;
        }
      }
    }
    L009599:;
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

__attribute__((scn_string_bindings(0, data_28FCA)))
static void script_95E9(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
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
          local_FC = Script_ScenDataUtil(data_27A70, 4, Script_ScenDataUtil(data_27480, 1, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9);
          break;
        case 0:
          local_B7 = 0;
          local_F8 = 12;
          local_F0 = 0x4000;
          local_B8 = 1.0f;
          local_FC = Script_ScenDataUtil(data_27A70, 4, Script_ScenDataUtil(data_27480, 1, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9) | -0x80000000;
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
          local_FC = Script_ScenDataUtil(data_27A70, 4, Script_ScenDataUtil(data_27480, 1, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9) | -0x80000000;
          break;
        case 0:
          local_B7 = 1;
          local_F8 = 18;
          local_F0 = 0xC000;
          local_B8 = 1.0f;
          local_FC = Script_ScenDataUtil(data_27A70, 4, Script_ScenDataUtil(data_27480, 1, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9);
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
          local_FC = Script_ScenDataUtil(data_27A70, 4, Script_ScenDataUtil(data_27480, 1, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9) | -0x80000000;
          break;
        case 0:
          local_B7 = 0;
          local_F8 = 18;
          local_F0 = 0xC000;
          local_B8 = -1.0f;
          local_FC = Script_ScenDataUtil(data_27A70, 4, Script_ScenDataUtil(data_27480, 1, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9);
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
          local_FC = Script_ScenDataUtil(data_27A70, 4, Script_ScenDataUtil(data_27480, 1, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9);
          break;
        case 0:
          local_B7 = 1;
          local_F8 = 12;
          local_F0 = 0x4000;
          local_B8 = -1.0f;
          local_FC = Script_ScenDataUtil(data_27A70, 4, Script_ScenDataUtil(data_27480, 1, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9) | -0x80000000;
          break;
      }
      break;
  }
  *(int32_t *)&local_C4 = Script_ScenDataUtil(data_27A70, 5, Script_ScenDataUtil(data_27480, 1, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9 + 1);
  *(int32_t *)&local_C0 = Script_ScenDataUtil(data_27A70, 5, Script_ScenDataUtil(data_27480, 1, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9 + 2);
  local_FC = local_FC | ((Script_FloatUtils(ScriptMathOp_FloatToInt, local_C4 - 3.0f) << 16) & 0x7FFF0000);
  local_BC = 0.0f;
  script_F0A6(arg0, 0);
  local_CC.x = (&task_250)->x;
  (&local_CC)->y = task_250.y;
  (&local_CC)->z = task_250.z;
  Script_ryoControl(PCCTL_StartCameraFacingInterpolation, 0);
  Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
  Script_SetCharaMoveTarget(arg0, &task_250, *(&task_25C + 1), local_FC);
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
        local_A0 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_130[arg2], 2);
        local_A0 = Script_ScenDataUtil(task_F0, 4, Script_ScenDataUtil(task_F4, 4, local_A0 * 9 + 1) * 4 + 2);
        if (local_A0 != -1) {
          if (arg0 == task_248) {
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
  script_F0A6(arg0, 1);
  local_FC = task_130[arg2];
  {
    int32_t local_A0;
    local_A0 = Script_ScenDataUtil(task_E8, 4, local_FC * 13 + 1);
    local_A0 = local_A0 | 8;
    Script_ScenDataUtil(task_E8, 14, local_FC * 13 + 1, local_A0);
  }
  {
    int32_t local_A0;
    local_A0 = Script_ScenDataUtil(task_E8, 4, local_FC * 13 + 1);
    local_A0 = local_A0 | 0x10;
    Script_ScenDataUtil(task_E8, 14, local_FC * 13 + 1, local_A0);
  }
  {
    int32_t local_A0;
    local_A0 = Script_ScenDataUtil(task_E8, 4, local_FC * 13 + 1);
    local_A0 = local_A0 | 4;
    Script_ScenDataUtil(task_E8, 14, local_FC * 13 + 1, local_A0);
  }
  return;
}

__attribute__((scn_string_bindings(0, data_28FE6)))
static void script_9CA3(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
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
      if (arg0 == task_248) {
        local_FF5C = arg4;
        switch (local_FF5C) {
          case 1:
            local_CF = 1;
            local_A4 = Script_ScenDataUtil(data_27A70, 4, Script_ScenDataUtil(data_27480, 1, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9);
            local_C0 = 1.0f;
            local_A0 = 18;
            local_9C = 18;
            local_90 = 15473;
            local_8C = 17294;
            break;
          case 0:
            local_CF = 0;
            local_A4 = Script_ScenDataUtil(data_27A70, 4, Script_ScenDataUtil(data_27480, 1, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9) | -0x80000000;
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
            local_A4 = Script_ScenDataUtil(data_27A70, 4, Script_ScenDataUtil(data_27480, 1, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9);
            local_C0 = 1.0f;
            local_A0 = 18;
            local_9C = 18;
            local_90 = 13653;
            break;
          case 0:
            local_CF = 0;
            local_A4 = Script_ScenDataUtil(data_27A70, 4, Script_ScenDataUtil(data_27480, 1, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9);
            local_C0 = 1.0f;
            local_A0 = 18;
            local_9C = 18;
            local_90 = 19842;
            break;
        }
      }
    } else if ((local_FF5C == 1) | (local_FF5C == 7) | (local_FF5C == 30) | (local_FF5C == 36) | (local_FF5C == 37)) {
      if (arg0 == task_248) {
        local_FF5C = arg4;
        switch (local_FF5C) {
          case 1:
            local_CF = 0;
            local_A4 = Script_ScenDataUtil(data_27A70, 4, Script_ScenDataUtil(data_27480, 1, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9) | -0x80000000;
            local_C0 = 1.0f;
            local_A0 = 12;
            local_9C = 18;
            local_90 = 50062;
            local_8C = 48241;
            break;
          case 0:
            local_CF = 1;
            local_A4 = Script_ScenDataUtil(data_27A70, 4, Script_ScenDataUtil(data_27480, 1, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9);
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
            local_A4 = Script_ScenDataUtil(data_27A70, 4, Script_ScenDataUtil(data_27480, 1, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9);
            local_C0 = 1.0f;
            local_A0 = 18;
            local_9C = 18;
            if (task_180[arg2] == 4) {
              local_90 = 50062;
            } else {
              local_90 = 51882;
            }
            break;
          case 0:
            local_CF = 1;
            local_A4 = Script_ScenDataUtil(data_27A70, 4, Script_ScenDataUtil(data_27480, 1, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9);
            local_C0 = 1.0f;
            local_A0 = 18;
            local_9C = 18;
            if (task_180[arg2] == 4) {
              local_90 = 46421;
            } else {
              local_90 = 45511;
            }
            break;
        }
      }
    } else if ((local_FF5C == 3) | (local_FF5C == 5) | (local_FF5C == 32) | (local_FF5C == 34)) {
      if (arg0 == task_248) {
        local_FF5C = arg4;
        switch (local_FF5C) {
          case 1:
            local_CF = 1;
            local_A4 = Script_ScenDataUtil(data_27A70, 4, Script_ScenDataUtil(data_27480, 1, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9) | -0x80000000;
            local_C0 = -1.0f;
            local_A0 = 12;
            local_9C = 12;
            local_90 = 50062;
            local_8C = 48241;
            break;
          case 0:
            local_CF = 0;
            local_A4 = Script_ScenDataUtil(data_27A70, 4, Script_ScenDataUtil(data_27480, 1, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9);
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
            local_A4 = Script_ScenDataUtil(data_27A70, 4, Script_ScenDataUtil(data_27480, 1, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9);
            local_C0 = -1.0f;
            local_A0 = 18;
            local_9C = 18;
            local_90 = 51882;
            break;
          case 0:
            local_CF = 0;
            local_A4 = Script_ScenDataUtil(data_27A70, 4, Script_ScenDataUtil(data_27480, 1, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9);
            local_C0 = -1.0f;
            local_A0 = 18;
            local_9C = 18;
            local_90 = 45511;
            break;
        }
      }
    } else if ((local_FF5C == 2) | (local_FF5C == 8) | (local_FF5C == 31)) {
      if (arg0 == task_248) {
        local_FF5C = arg4;
        switch (local_FF5C) {
          case 1:
            local_CF = 0;
            local_A4 = Script_ScenDataUtil(data_27A70, 4, Script_ScenDataUtil(data_27480, 1, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9);
            local_C0 = -1.0f;
            local_A0 = 18;
            local_9C = 12;
            local_90 = 15473;
            local_8C = 17294;
            break;
          case 0:
            local_CF = 1;
            local_A4 = Script_ScenDataUtil(data_27A70, 4, Script_ScenDataUtil(data_27480, 1, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9) | -0x80000000;
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
            local_A4 = Script_ScenDataUtil(data_27A70, 4, Script_ScenDataUtil(data_27480, 1, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9);
            local_C0 = -1.0f;
            local_A0 = 18;
            local_9C = 18;
            local_90 = 13653;
            break;
          case 0:
            local_CF = 1;
            local_A4 = Script_ScenDataUtil(data_27A70, 4, Script_ScenDataUtil(data_27480, 1, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9);
            local_C0 = -1.0f;
            local_A0 = 18;
            local_9C = 18;
            local_90 = 19842;
            break;
        }
      }
    } else {
      if ((local_FF5C == 13) | (local_FF5C == 14) | (local_FF5C == 28)) {
        if (arg0 == task_248) {
          local_FF5C = arg4;
          switch (local_FF5C) {
            case 0:
              local_CF = 0;
              local_A4 = Script_ScenDataUtil(data_27A70, 4, Script_ScenDataUtil(data_27480, 1, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9);
              local_C0 = -1.0f;
              local_A0 = 18;
              local_9C = 12;
              local_90 = 48241;
              local_8C = 50062;
              break;
            case 1:
              local_CF = 1;
              local_A4 = Script_ScenDataUtil(data_27A70, 4, Script_ScenDataUtil(data_27480, 1, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9) | -0x80000000;
              local_C0 = -1.0f;
              local_A0 = 12;
              local_9C = 12;
              local_90 = 50062;
              local_8C = 48241;
              break;
            case 2:
              local_CF = 1;
              local_A4 = Script_ScenDataUtil(data_27A70, 4, Script_ScenDataUtil(data_27480, 1, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9);
              local_C0 = 1.0f;
              local_A0 = 18;
              local_9C = 18;
              local_90 = 15473;
              local_8C = 17294;
              break;
            case 3:
              local_CF = 0;
              local_A4 = Script_ScenDataUtil(data_27A70, 4, Script_ScenDataUtil(data_27480, 1, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9) | -0x80000000;
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
              local_A4 = Script_ScenDataUtil(data_27A70, 4, Script_ScenDataUtil(data_27480, 1, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9);
              local_C0 = -1.0f;
              local_A0 = 18;
              local_9C = 18;
              local_90 = 45511;
              break;
            case 1:
              local_CF = 1;
              local_A4 = Script_ScenDataUtil(data_27A70, 4, Script_ScenDataUtil(data_27480, 1, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9);
              local_C0 = -1.0f;
              local_A0 = 18;
              local_9C = 18;
              local_90 = 51882;
              break;
            case 2:
              local_CF = 1;
              local_A4 = Script_ScenDataUtil(data_27A70, 4, Script_ScenDataUtil(data_27480, 1, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9);
              local_C0 = 1.0f;
              local_A0 = 18;
              local_9C = 18;
              if (task_180[arg2] == 4) {
                local_90 = 14199;
              } else {
                local_90 = 13653;
              }
              break;
            case 3:
              local_CF = 0;
              local_A4 = Script_ScenDataUtil(data_27A70, 4, Script_ScenDataUtil(data_27480, 1, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9);
              local_C0 = 1.0f;
              local_A0 = 18;
              local_9C = 18;
              if (task_180[arg2] == 4) {
                local_90 = 18022;
              } else {
                local_90 = 19842;
              }
              break;
          }
        }
        local_CE = 1;
        goto L00A896;
      }
    }
  }
  L00A896:;
  (void)local_90;
  *(int32_t *)&local_B8 = Script_ScenDataUtil(data_27A70, 5, Script_ScenDataUtil(data_27480, 1, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9 + 1);
  *(int32_t *)&local_B4 = Script_ScenDataUtil(data_27A70, 5, Script_ScenDataUtil(data_27480, 1, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9 + 2);
  *(int32_t *)&local_B0 = Script_ScenDataUtil(data_27A70, 5, Script_ScenDataUtil(data_27480, 1, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9 + 3);
  *(int32_t *)&local_AC = Script_ScenDataUtil(data_27A70, 5, Script_ScenDataUtil(data_27480, 1, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9 + 4);
  *(int32_t *)&local_A8 = Script_ScenDataUtil(data_27A70, 5, Script_ScenDataUtil(data_27480, 1, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9 + 7);
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
  script_5E84(arg0, local_A4, arg2, arg3, arg4);
  local_A4 = local_A4 | ((Script_FloatUtils(ScriptMathOp_FloatToInt, local_B8 - 3.0f) << 16) & 0x7FFF0000);
  script_F0A6(arg0, 0);
  local_D0.x = (&task_250)->x;
  (&local_D0)->y = task_250.y;
  (&local_D0)->z = task_250.z;
  Script_ryoControl(PCCTL_StartCameraFacingInterpolation, 0);
  Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
  Script_SetCharaMoveTarget(arg0, &task_250, *(&task_25C + 1), local_A4);
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
    if (arg0 == task_248) {
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
      if (arg0 == task_248) {
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
            if ((task_130[arg2] == 15) & (arg4 == 0)) {
              local_FF50 = 0;
            }
            if (local_FF50) {
              script_80DA(arg0, task_180[arg2]);
            }
          }
        }
      }
      if (!(local_C4 > local_B4)) {
        if (!(local_C4 < local_B8) & !(local_C4 > local_B4)) {
          if (arg0 == task_248) {
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
          local_FF50 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_130[arg2], 2);
          local_FF50 = Script_ScenDataUtil(task_F0, 4, Script_ScenDataUtil(task_F4, 4, local_FF50 * 9 + 1) * 4 + 2);
          if (local_FF50 != -1) {
            if (arg0 == task_248) {
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
        local_FF60 = script_5E0C((&local_FF64)->y & 0xFFFF, local_88 & 0xFFFF);
        if (local_FF60 > 910) {
          local_88 = (&local_FF64)->y + 910;
        } else {
          if (local_FF60 < -910) {
            local_88 = (&local_FF64)->y - 910;
            goto L00AEB9;
          }
        }
        L00AEB9:;
        if (!(local_C4 >= local_B4)) {
          if (!(local_C0 <= 0.0f)) {
            if (script_5E0C(local_FF70, local_88) > 0) {
              local_88 = local_FF70;
            }
          } else {
            if (script_5E0C(local_FF70, local_88) < 0) {
              local_88 = local_FF70;
            }
          }
        }
      }
      if (arg0 != task_248) {
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
          local_FF74 = (float)script_5DCF(script_5E0C(local_FF70, (&local_FF48)->y) * 360 / 0x10000);
        }
        if (!(local_C4 <= local_B4) & !(local_C4 > local_A8)) {
          *(int32_t *)&local_BC = Script_ScenDataUtil(data_27A70, 5, Script_ScenDataUtil(data_27480, 1, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9 + 8);
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
          if (arg0 != task_248) {
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
        local_FF50 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_130[arg2], 2);
        local_FF50 = Script_ScenDataUtil(task_F0, 4, Script_ScenDataUtil(task_F4, 4, local_FF50 * 9 + 1) * 4 + 3);
        if (local_FF50 != -1) {
          if (arg0 == task_248) {
            Script_executeAicaCommand(local_FF50, 0, 0);
          } else {
            Script_Execute3DSoundCommand_Wrapper(local_FF50, 4, &local_D0, 5.0f, 20.0f, 0);
          }
        }
        local_A4 = task_130[arg2];
        if (arg0 == task_248) {
          if (task_248 == arg0) {
            local_A0 = (Script_ScenDataUtil(task_E8, 4, local_A4 * 13 + 2) >> 16) & 0xFFFF;
            Script_scenToggleCharaFlag(0, local_A0, 1);
            local_A0 = Script_ScenDataUtil(task_E8, 4, local_A4 * 13 + 2) & 0xFFFF;
            Script_scenToggleCharaFlag(0, local_A0, 1);
          }
        }
      }
      if (!(local_AC <= 0.0f)) {
        if (!(local_C4 <= local_AC)) {
          if (arg0 == task_248) {
            local_FF5C = arg3;
            if ((local_FF5C == 30) | (local_FF5C == 31) | (local_FF5C == 32) | (local_FF5C == 33) | (local_FF5C == 34) | (local_FF5C == 35)) {
              local_FF78 = local_FF78 - 0.06f;
              if (!(local_FF78 >= 0.3f)) {
                local_FF78 = 0.3f;
              }
              DSWK_SetActorOpacityAndDrawCallback(arg1, local_FF78);
              goto L00B365;
            }
          }
        }
      }
      L00B365:;
      if (!(local_C4 < local_B0) & !(local_C4 > local_AC)) {
        if (arg0 == task_248) {
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
      if (arg0 != task_248) {
        if (!(local_C4 < local_AC)) {
          break;
        }
        local_C4 = local_C4 + 1.0f;
      }
      SCNF_WaitFrames_0000(1);
    }
  }
  script_F0A6(arg0, 1);
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
  if (arg0 == task_248) {
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
      goto L00B5A0;
    }
  }
  L00B5A0:;
  return;
}

__attribute__((scn_string_bindings(0, data_29002)))
static void script_B5A1(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
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
    script_F0A6(arg0, 0);
    local_C4.x = (&task_250)->x;
    (&local_C4)->y = task_250.y;
    (&local_C4)->z = task_250.z;
    Script_ryoControl(PCCTL_StartCameraFacingInterpolation, 0);
    Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
    Script_SetCharaMoveTarget(arg0, &task_250, *(&task_25C + 1), local_94);
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
          local_84 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_130[arg2], 2);
          local_84 = Script_ScenDataUtil(task_F0, 4, Script_ScenDataUtil(task_F4, 4, local_84 * 9 + 1) * 4 + 3);
          if (local_84 != -1) {
            if (arg0 == task_248) {
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
                goto L00B8C5;
              }
            }
            L00B8C5:;
          } else {
            if ((local_84 == 1) | (local_84 == 3)) {
              if (!(local_B8 > -0.75f)) {
                local_B8 = -0.75f;
              } else {
                if (!(local_B8 < 0.0f)) {
                  local_B8 = 0.0f;
                  goto L00B901;
                }
              }
              L00B901:;
              goto L00B904;
            }
          }
        }
      }
      L00B904:;
      if (!(local_BC <= local_A4)) {
        int32_t local_84;
        local_84 = arg4;
        if ((local_84 == 0) | (local_84 == 2)) {
          local_B8 = 0.75f;
        } else {
          if ((local_84 == 1) | (local_84 == 3)) {
            local_B8 = -0.75f;
            goto L00B949;
          }
        }
      }
      L00B949:;
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
  script_F0A6(arg0, 1);
  {
    int32_t local_94;
    local_94 = task_130[arg2];
    local_9C = Script_ScenDataUtil(task_E8, 4, local_94 * 13 + 2) & 0xFFFF;
    Script_scenToggleCharaFlag(0, local_9C, 1);
    local_9C = (Script_ScenDataUtil(task_E8, 4, local_94 * 13 + 2) >> 16) & 0xFFFF;
    Script_scenToggleCharaFlag(0, local_9C, 1);
    {
      int32_t unused_88[3];
      int32_t local_84;
      local_84 = Script_ScenDataUtil(task_E8, 4, local_94 * 13 + 1);
      local_84 = local_84 & -9;
      Script_ScenDataUtil(task_E8, 14, local_94 * 13 + 1, local_84);
    }
    {
      int32_t unused_88[3];
      int32_t local_84;
      local_84 = Script_ScenDataUtil(task_E8, 4, local_94 * 13 + 1);
      local_84 = local_84 & -17;
      Script_ScenDataUtil(task_E8, 14, local_94 * 13 + 1, local_84);
    }
    {
      int32_t unused_88[3];
      int32_t local_84;
      local_84 = Script_ScenDataUtil(task_E8, 4, local_94 * 13 + 1);
      local_84 = local_84 & -5;
      Script_ScenDataUtil(task_E8, 14, local_94 * 13 + 1, local_84);
    }
  }
  return;
}

__attribute__((scn_string_bindings(0, data_2901E)))
static void script_BCC2(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
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
      local_8C = Script_ScenDataUtil(data_27A70, 4, Script_ScenDataUtil(data_27480, 1, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9);
      local_A0 = -1.0f;
      local_88 = 18;
      local_84 = 18;
      local_FF78 = 46421;
      local_FF74 = 48787;
      break;
    case 1:
      local_AF = 1;
      local_8C = Script_ScenDataUtil(data_27A70, 4, Script_ScenDataUtil(data_27480, 1, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9) | -0x80000000;
      local_A0 = -1.0f;
      local_88 = 12;
      local_84 = 12;
      local_FF78 = 50426;
      local_FF74 = 48969;
      break;
    case 2:
      local_AF = 1;
      local_8C = Script_ScenDataUtil(data_27A70, 4, Script_ScenDataUtil(data_27480, 1, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9);
      local_A0 = 1.0f;
      local_88 = 18;
      local_84 = 18;
      local_FF78 = 15109;
      local_FF74 = 16566;
      break;
    case 3:
      local_AF = 0;
      local_8C = Script_ScenDataUtil(data_27A70, 4, Script_ScenDataUtil(data_27480, 1, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9) | -0x80000000;
      local_A0 = 1.0f;
      local_88 = 12;
      local_84 = 12;
      local_FF78 = 19114;
      local_FF74 = 16748;
      break;
  }
  (void)local_FF78;
  *(int32_t *)&local_9C = Script_ScenDataUtil(data_27A70, 5, Script_ScenDataUtil(data_27480, 1, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9 + 1);
  *(int32_t *)&local_98 = Script_ScenDataUtil(data_27A70, 5, Script_ScenDataUtil(data_27480, 1, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9 + 2);
  *(int32_t *)&local_94 = Script_ScenDataUtil(data_27A70, 5, Script_ScenDataUtil(data_27480, 1, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9 + 3);
  *(int32_t *)&local_90 = Script_ScenDataUtil(data_27A70, 5, Script_ScenDataUtil(data_27480, 1, arg3 * 40 + task_180[arg2] * 4 + arg4) * 9 + 4);
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
  script_F0A6(arg0, 0);
  local_B0.x = (&task_250)->x;
  (&local_B0)->y = task_250.y;
  (&local_B0)->z = task_250.z;
  Script_ryoControl(PCCTL_StartCameraFacingInterpolation, 0);
  Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
  Script_SetCharaMoveTarget(arg0, &task_250, *(&task_25C + 1), local_8C);
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
        local_FF64 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_130[arg2], 2);
        local_FF64 = Script_ScenDataUtil(task_F0, 4, Script_ScenDataUtil(task_F4, 4, local_FF64 * 9 + 1) * 4 + 2);
        if (local_FF64 != -1) {
          if (arg0 == task_248) {
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
        local_FF64 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_130[arg2], 2);
        local_FF64 = Script_ScenDataUtil(task_F0, 4, Script_ScenDataUtil(task_F4, 4, local_FF64 * 9 + 1) * 4 + 3);
        if (local_FF64 != -1) {
          if (arg0 == task_248) {
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
  script_F0A6(arg0, 1);
  local_F4.x = 0;
  (&local_F4)->z = 0;
  (&local_F4)->y = local_80;
  Script_charaPosRotUtils(arg1, 12, 0, &local_F4, 0);
  (&local_F4)->y = local_FF7C;
  Script_charaPosRotUtils(arg1, 7, 0, &local_F4, 0);
  return;
}

static void script_C44C(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
  int32_t local_FC;
  int32_t local_F8;
  Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
  Script_SetCharaMoveTarget(arg0, &task_250, *(&task_25C + 1), MN_AKI_AKI_BASE);
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
        local_FC = Script_ScenDataUtil(task_E8, 4, task_130[arg2] * 13 + 7);
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

__attribute__((scn_string_bindings(0, data_2903A)))
static void script_C524(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
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
  local_D4 = Script_ScenDataUtil(task_E8, 4, task_130[arg2] * 13 + 6);
  local_CC = arg3;
  if ((local_CC == 1) | (local_CC == 3) | (local_CC == 5) | (local_CC == 7) | (local_CC == 9) | (local_CC == 18) | (local_CC == 21) | (local_CC == 25) | (local_CC == 27) | (local_CC == 36) | (local_CC == 37)) {
    local_CC = arg4;
    switch (local_CC) {
      case 1:
        local_E0 = Script_ScenDataUtil(&data_28490, 4, local_D4 * 5) | -0x80000000;
        break;
      case 0:
        local_E0 = Script_ScenDataUtil(&data_28490, 4, local_D4 * 5);
        break;
    }
  } else if ((local_CC == 2) | (local_CC == 4) | (local_CC == 6) | (local_CC == 8) | (local_CC == 10) | (local_CC == 19) | (local_CC == 22) | (local_CC == 24) | (local_CC == 26) | (local_CC == 28)) {
    local_CC = arg4;
    switch (local_CC) {
      case 1:
        local_E0 = Script_ScenDataUtil(&data_28490, 4, local_D4 * 5);
        break;
      case 0:
        local_E0 = Script_ScenDataUtil(&data_28490, 4, local_D4 * 5) | -0x80000000;
        break;
    }
  } else {
    if ((local_CC == 13) | (local_CC == 14) | (local_CC == 15) | (local_CC == 16) | (local_CC == 20) | (local_CC == 23) | (local_CC == 29)) {
      local_CC = arg4;
      switch (local_CC) {
        case 0:
          local_E0 = Script_ScenDataUtil(&data_28490, 4, local_D4 * 5);
          break;
        case 1:
          local_E0 = Script_ScenDataUtil(&data_28490, 4, local_D4 * 5) | -0x80000000;
          break;
        case 2:
          local_E0 = Script_ScenDataUtil(&data_28490, 4, local_D4 * 5);
          break;
        case 3:
          local_E0 = Script_ScenDataUtil(&data_28490, 4, local_D4 * 5) | -0x80000000;
          break;
      }
      goto L00C745;
    }
  }
  L00C745:;
  *(int32_t *)&local_EC = Script_ScenDataUtil(&data_28490, 5, local_D4 * 5 + 1);
  *(int32_t *)&local_E8 = Script_ScenDataUtil(&data_28490, 5, local_D4 * 5 + 2);
  local_E0 = local_E0 | ((Script_FloatUtils(ScriptMathOp_FloatToInt, local_EC - 3.0f) << 16) & 0x7FFF0000);
  local_F4.x = (&task_250)->x;
  (&local_F4)->y = task_250.y;
  (&local_F4)->z = task_250.z;
  Script_ryoControl(PCCTL_StartCameraFacingInterpolation, 0);
  Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
  Script_SetCharaMoveTarget(arg0, &task_250, *(&task_25C + 1), local_E0);
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
      local_C8 = Script_ScenDataUtil(task_E8, 4, task_130[arg2] * 13 + 9);
      if (local_C8 != -1) {
        if (arg0 == task_248) {
          Script_executeAicaCommand(local_C8, 0, 0);
        } else {
          Script_Execute3DSoundCommand_Wrapper(local_C8, 4, &local_F4, 5.0f, 20.0f, 0);
        }
      }
    }
    if (!(local_F0 < local_E8) & !(local_F0 >= local_E8 + 1.0f)) {
      if ((local_D4 == 6) | (local_D4 == 7)) {
        int32_t local_C8;
        local_C8 = Script_ScenDataUtil(task_E8, 4, task_130[arg2] * 13 + 9);
        if (local_C8 != -1) {
          if (arg0 == task_248) {
            Script_executeAicaCommand(local_C8, 0, 0);
          } else {
            Script_Execute3DSoundCommand_Wrapper(local_C8, 4, &local_F4, 5.0f, 20.0f, 0);
          }
        }
      }
      if (local_DC == 0) {
        local_DC = 1;
        local_D8 = Script_ScenDataUtil(task_E8, 4, task_130[arg2] * 13 + 7);
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

__attribute__((scn_string_bindings(0, data_29056)))
static void script_C99B(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
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
  task_274 = 0;
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
        goto L00CCA7;
      }
    }
  }
  L00CCA7:;
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
  local_B8.x = (&task_250)->x;
  (&local_B8)->y = task_250.y;
  (&local_B8)->z = task_250.z;
  Script_ryoControl(PCCTL_StartCameraFacingInterpolation, 0);
  Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
  Script_SetCharaMoveTarget(arg0, &task_250, *(&task_25C + 1), local_90);
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
        local_FF40 = Script_ScenDataUtil(task_E8, 4, task_130[arg2] * 13 + 10);
        if (local_FF40 != -1) {
          if (arg0 == task_248) {
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
      local_FF40 = Script_ScenDataUtil(task_E8, 4, task_130[arg2] * 13 + 9);
      if (local_FF40 != -1) {
        if (arg0 == task_248) {
          Script_executeAicaCommand(local_FF40, 0, 0);
        } else {
          Script_Execute3DSoundCommand_Wrapper(local_FF40, 4, &local_B8, 5.0f, 20.0f, 0);
        }
      }
    }
    if (!(local_A0 < local_94) & !(local_A0 >= local_94 + 1.0f)) {
      int32_t unused_FF44;
      int32_t local_FF40;
      local_FF40 = Script_ScenDataUtil(task_E8, 4, task_130[arg2] * 13 + 9);
      if (local_FF40 != -1) {
        if (arg0 == task_248) {
          Script_executeAicaCommand(local_FF40, 0, 0);
        } else {
          Script_Execute3DSoundCommand_Wrapper(local_FF40, 4, &local_B8, 5.0f, 20.0f, 0);
        }
      }
      if (local_FF68 == 0) {
        local_FF68 = 1;
        local_FF64 = Script_ScenDataUtil(task_E8, 4, task_130[arg2] * 13 + 8);
        Script_ConversationUtils(1, local_FF74);
        Script_EnqueueSCNT(1, 0x524F4F44, local_FF64);
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  local_A4 = 0.0f;
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0) != 0) {
    local_FF6C = 0;
    L00D772:;
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
      if (!(local_FF6C >= 20)) goto L00D772;
    }
    {
      int32_t unused_FF44;
      int32_t local_FF40;
      local_FF40 = Script_ScenDataUtil(task_E8, 4, task_130[arg2] * 13 + 11);
      if (local_FF40 != -1) {
        if (arg0 == task_248) {
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

static void script_D92E(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
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
    if (arg0 != task_248) {
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
      Script_SetCharaMoveTarget(arg0, &local_E0, local_BC[1], script_6436(arg5));
    }
  }
  return;
}

static void script_DD78(int32_t arg0, DSWKDoorEntry *arg1, int32_t *arg2, DoorResourceDescriptor *arg3, MapObjectRecord *arg4, int32_t arg5, int32_t arg6) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t unused_F4;
  task_E8 = arg1;
  task_EC = arg2;
  task_F0 = arg3;
  task_F4 = arg4;
  task_F8.mode = 0;
  task_F8.id = 0;
  task_F8.entry_idx = 0;
  task_248 = arg0;
  (void)((task_26C = -1) & 1);
  task_268 = 0;
  Script_motiLoad(Script_GetSceneID(), SHEN_NAME('FREE'), "M_MDOR.BIN");
  local_F8 = 0;
  L00DDE2:;
  local_FC = Script_AddCharacterHandler(1, 0, local_F8 * 0x1000000 + 0x30726F64);
  Script_PreserveAccumulator_012C(local_FC, "This Charactor Work is NULL!!\012");
  DSWK_AllocDoorMapcRenderState(local_FC);
  Script_scenToggleVisibleFor(local_FC, 0);
  task_1D0[local_F8] = local_FC;
  task_108[local_F8] = -1;
  task_130[local_F8] = -1;
  task_158[local_F8] = 0;
  task_180[local_F8] = 0;
  task_1A8[local_F8] = 0;
  task_1F8[local_F8] = 0;
  local_F8 = local_F8 + 1;
  if (!(local_F8 >= 10)) goto L00DDE2;
  local_F8 = 0;
  for (;;) {
    if (Script_ScenDataUtil(task_E8, 4, local_F8 * 13) == -1) {
      break;
    }
    Script_ScenDataUtil(task_E8, 14, local_F8 * 13 + 1, 0);
    local_F8 = local_F8 + 1;
  }
  Script_EnqueueSCNT(0);
  DSWK_BindTablesAndInitState(arg1, arg2, arg3, arg4, &task_F8, task_108, task_130, (shen_name *)task_158, task_180, arg0, arg5, data_27480, data_27A70, (shen_name *)task_1D0);
  Script_scenCreateTask(script_E634, 0);
  return;
}

static void script_DF04(DSWKDoorEntry *arg0, int32_t *arg1, DoorResourceDescriptor *arg2, MapObjectRecord *arg3, int32_t arg4) {
  task_E8 = arg0;
  task_EC = arg1;
  task_F0 = arg2;
  task_F4 = arg3;
  DSWK_RebindDoorSystemTables(arg0, arg1, arg2, arg3, arg4);
  task_268 = 1;
  return;
}

static void script_DF38(void) {
  int32_t local_FC;
  local_FC = 0;
  L00DF40:;
  task_108[local_FC] = -1;
  task_130[local_FC] = -1;
  task_158[local_FC] = 0;
  task_180[local_FC] = 0;
  task_1A8[local_FC] = 0;
  task_1F8[local_FC] = 0;
  local_FC = local_FC + 1;
  if (!(local_FC >= 10)) goto L00DF40;
  return;
}

static void script_DFA1(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  local_F4 = 0;
  for (;;) {
    Script_sprintf("手動ドア開け番号指定\012");
    if (task_248 == arg0) {
      Script_EVT_GetPosition(arg0, -1, &task_27C, 0);
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

static void script_E03E(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  Vector3 local_F0;
  if (task_248 == arg0) {
    Script_EVT_GetPosition(arg0, -1, &task_27C, 0);
  }
  Script_EVT_GetPosition(arg0, -1, &local_F0, 0);
  local_FC = DSWK_DoorUtils(DOOR_CMD_FIND_CLOSEST_ELEMENT, arg0, &local_F0, arg1);
  (void)local_FC;
  return;
}

static void script_E078(void) {
  int32_t local_FC;
  int32_t local_F8;
  local_FC = 0;
  for (;;) {
    if (Script_ScenDataUtil(task_E8, 4, local_FC * 13) == -1) {
      break;
    }
    local_F8 = Script_ScenDataUtil(task_E8, 4, local_FC * 13 + 1);
    local_F8 = local_F8 | 2;
    Script_ScenDataUtil(task_E8, 14, local_FC * 13 + 1, local_F8);
    local_FC = local_FC + 1;
  }
  return;
}

static void script_E0DB(void) {
  int32_t local_FC;
  int32_t local_F8;
  local_FC = 0;
  for (;;) {
    if (Script_ScenDataUtil(task_E8, 4, local_FC * 13) == -1) {
      break;
    }
    local_F8 = Script_ScenDataUtil(task_E8, 4, local_FC * 13 + 1);
    local_F8 = local_F8 & -3;
    Script_ScenDataUtil(task_E8, 14, local_FC * 13 + 1, local_F8);
    local_FC = local_FC + 1;
  }
  return;
}

static void script_E13E(int32_t arg0) {
  int32_t local_FC;
  local_FC = Script_ScenDataUtil(task_E8, 4, arg0 * 13 + 1);
  local_FC = local_FC | 2;
  Script_ScenDataUtil(task_E8, 14, arg0 * 13 + 1, local_FC);
  return;
}

static void script_E175(int32_t arg0) {
  int32_t local_FC;
  local_FC = Script_ScenDataUtil(task_E8, 4, arg0 * 13 + 1);
  local_FC = local_FC & -3;
  Script_ScenDataUtil(task_E8, 14, arg0 * 13 + 1, local_FC);
  return;
}

static void script_E1AC(int32_t arg0) {
  Vector3 local_F4;
  int32_t local_F0;
  int32_t local_EC;
  local_F0 = 0;
  for (;;) {
    if (task_130[local_F0] == arg0) {
      local_EC = task_1D0[local_F0];
      if (Script_shenGetNode(local_EC, 7)) {
        if (Script_shenGetNode(local_EC, 5) & Script_shenGetNode(local_EC, 10)) {
          if (Script_ScenDataUtil(task_E8, 4, task_130[local_F0] * 13 + 1) & 0x100) {
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
          if (Script_ScenDataUtil(task_E8, 4, task_130[local_F0] * 13 + 1) & 0x100) {
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
        if (Script_ScenDataUtil(task_E8, 4, task_130[local_F0] * 13 + 1) & 0x20) {
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

static void script_E3EE(int32_t arg0) {
  int32_t local_FC;
  local_FC = Script_ScenDataUtil(task_E8, 4, arg0 * 13 + 1);
  local_FC = local_FC | 0x20;
  Script_ScenDataUtil(task_E8, 14, arg0 * 13 + 1, local_FC);
  script_E4F6(arg0);
  script_E1AC(arg0);
  return;
}

static void script_E437(int32_t arg0) {
  int32_t local_FC;
  local_FC = Script_ScenDataUtil(task_E8, 4, arg0 * 13 + 1);
  local_FC = local_FC & -33;
  Script_ScenDataUtil(task_E8, 14, arg0 * 13 + 1, local_FC);
  script_E504(arg0);
  script_E1AC(arg0);
  return;
}

static void script_E480(int32_t arg0, int32_t arg1) {
  if (arg1) {
    int32_t local_FC;
    local_FC = Script_ScenDataUtil(task_E8, 4, arg0 * 13 + 1);
    local_FC = local_FC & -257;
    Script_ScenDataUtil(task_E8, 14, arg0 * 13 + 1, local_FC);
  } else {
    int32_t local_FC;
    local_FC = Script_ScenDataUtil(task_E8, 4, arg0 * 13 + 1);
    local_FC = local_FC | 0x100;
    Script_ScenDataUtil(task_E8, 14, arg0 * 13 + 1, local_FC);
  }
  return;
}

static void script_E4F6(int32_t arg0) {
  script_E480(arg0, 0);
  return;
}

static void script_E504(int32_t arg0) {
  script_E480(arg0, 1);
  return;
}

static void script_E512(int32_t arg0) {
  int32_t local_FC;
  local_FC = 0;
  for (;;) {
    if (Script_ScenDataUtil(task_E8, 4, local_FC * 13) == -1) {
      break;
    }
    if (arg0) {
      int32_t local_F8;
      local_F8 = Script_ScenDataUtil(task_E8, 4, local_FC * 13 + 1);
      local_F8 = local_F8 & -65;
      local_F8 = local_F8 & -129;
      local_F8 = local_F8 | 0x40;
      Script_ScenDataUtil(task_E8, 14, local_FC * 13 + 1, local_F8);
    } else {
      int32_t local_F8;
      local_F8 = Script_ScenDataUtil(task_E8, 4, local_FC * 13 + 1);
      local_F8 = local_F8 & -65;
      local_F8 = local_F8 & -129;
      local_F8 = local_F8 | 0x80;
      Script_ScenDataUtil(task_E8, 14, local_FC * 13 + 1, local_F8);
    }
    local_FC = local_FC + 1;
  }
  return;
}

static void script_E5D9(void) {
  script_E512(1);
  return;
}

static void script_E5E5(void) {
  script_E512(0);
  return;
}

static void script_E5F1(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5, int32_t arg6) {
  int32_t local_FC;
  local_FC = 0;
  for (;;) {
    if (Script_ScenDataUtil(task_E8, 4, local_FC * 13) == -1) {
      break;
    }
    DSWK_AssignDoorEntryConfig(local_FC, arg0, arg1, arg2, arg3, arg4, arg5, arg6);
    local_FC = local_FC + 1;
  }
  return;
}

static int32_t script_E634(void) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  SCNF_WaitFrames_0000(1);
  SCNF_WaitFrames_0000(1);
  SCNF_WaitFrames_0000(1);
  local_F4 = -1;
  local_F8 = -1;
  for (;;) {
    if (task_268 == 0) {
      local_F4 = DSWK_DoorUtils(DOOR_CMD_FIND_LOWEST_FLOAT_INDEX, 0x3FD851EA);
      if (-(local_F4 != -1) & -(task_108[local_F4] == 1) & Script_IsPlayerAllowed(0) & -((&task_F8)->mode == 0)) {
        local_FC = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_130[local_F4], 2);
        local_F8 = Script_ScenDataUtil(task_F0, 4, Script_ScenDataUtil(task_F4, 4, local_FC * 9 + 1) * 4 + 1);
        script_F404(1, local_FC, local_F4, local_F8);
        if (script_F5B8(local_FC, local_F4, local_F8)) {
          local_F4 = -1;
          local_F8 = -1;
        }
      } else {
        script_F404(0, local_FC, local_F4, local_F8);
        local_F4 = -1;
        local_F8 = -1;
      }
      script_F7CE(local_F4, local_F8);
    } else {
      task_268 = 0;
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int8_t script_E745(int32_t arg0, int32_t arg1, int8_t arg2) {
  Vector3 local_F4;
  Vector3 local_E8;
  Vector3i local_DC;
  int32_t local_D8;
  arg2 = 0;
  Script_EVT_GetPosition(task_248, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  Script_mtwkGetSetRotation(task_248, &local_DC, 0x42000000);
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

__attribute__((scn_string_bindings(0, data_290E1, 1, data_290FA)))
static void script_E7DD(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  Vector3 local_EC;
  Vector3i local_E0;
  local_F8 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_130[arg0], 1);
  if (local_F8 != -1) {
    local_FC = Script_AddCharacterHandler(1, 0, arg0 * 0x1000000 + 0x30786F62);
    if (local_FC == 0) {
      Script_PreserveAccumulator_012C(local_FC, "Don't make BOX Buffer!!\012");
      task_220[arg0] = -1;
    } else if (local_FC == arg0 * 0x1000000 + 0x30786F62) {
      task_220[arg0] = local_FC;
      *(int32_t *)&local_EC.x = Script_ScenDataUtil(task_F4, 4, local_F8 * 9 + 5);
      *(int32_t *)&(&local_EC)->y = Script_ScenDataUtil(task_F4, 4, local_F8 * 9 + 6);
      *(int32_t *)&(&local_EC)->z = Script_ScenDataUtil(task_F4, 4, local_F8 * 9 + 7);
      Script_EVT_SetPosition(local_FC, POS_XYZ, &local_EC);
      local_E0.x = 0;
      (&local_E0)->y = Script_ScenDataUtil(task_F4, 4, local_F8 * 9 + 8);
      (&local_E0)->z = 0;
      Script_EVT_SetRotation(local_FC, 0x38000000, &local_E0, 0);
      *(int32_t *)&local_EC.x = Script_ScenDataUtil(task_F4, 4, local_F8 * 9 + 2);
      *(int32_t *)&(&local_EC)->y = Script_ScenDataUtil(task_F4, 4, local_F8 * 9 + 3);
      *(int32_t *)&(&local_EC)->z = Script_ScenDataUtil(task_F4, 4, local_F8 * 9 + 4);
      Script_hmdlApplyScale(local_FC, &local_EC);
      Script_CreateNPCImageModule(local_FC, (const char *)Script_ScenDataUtil(task_F0, 8, Script_ScenDataUtil(task_F4, 4, local_F8 * 9 + 1) * 4), 0);
      Script_charaDisplay(local_FC, CDM_Show);
      Script_charaVisUtils(local_FC, 1);
    } else {
      Script_PreserveAccumulator_012C(local_FC, "Don't make BOX Buffer!!\012");
      task_220[arg0] = -1;
    }
  } else {
    task_220[arg0] = -1;
  }
  return;
}

static void script_E9AC(int32_t arg0) {
  if (task_220[arg0] != -1) {
    Script_SetMapControlTaskFlag(task_220[arg0], 1);
    task_220[arg0] = -1;
  }
  return;
}

static int32_t script_E9DD(int32_t arg0, int32_t arg1, int32_t arg2, float arg3, float arg4, int32_t arg5, int32_t arg6) {
  Vector3 local_F4;
  Vector3 local_E8;
  float local_DC[3];
  Vector3i local_D0;
  Vector3i local_C4;
  int32_t local_C0;
  int32_t local_BC;
  if (task_248 == arg0) {
    local_F4.x = (&task_27C)->x;
    (&local_F4)->y = task_27C.y;
    (&local_F4)->z = task_27C.z;
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
        if ((Script_ScenDataUtil(task_E8, 4, arg6 * 13 + 1) & 8) | (Script_ScenDataUtil(task_E8, 4, arg6 * 13 + 1) & 0x40)) {
          local_C0 = local_C0 ^ 3;
        }
      } else {
        if ((Script_ScenDataUtil(task_E8, 4, arg6 * 13 + 1) & 0x10) | (Script_ScenDataUtil(task_E8, 4, arg6 * 13 + 1) & 0x80)) {
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
          if ((local_B4 == 1) | (local_B4 == 2)) goto L00EBC8;
        }
        L00EBC8:;
      } else if ((local_B4 == 2) | (local_B4 == 4) | (local_B4 == 6) | (local_B4 == 8) | (local_B4 == 10) | (local_B4 == 12) | (local_B4 == 24) | (local_B4 == 26) | (local_B4 == 31) | (local_B4 == 33) | (local_B4 == 35)) {
        local_B4 = local_C0;
        if ((local_B4 == 2) | (local_B4 == 1)) {
          arg3 = (float)0 - arg3;
        } else {
          if ((local_B4 == 3) | (local_B4 == 0)) {
            arg3 = (float)0 - arg3;
            arg4 = (float)0 - arg4;
            (&local_C4)->y = (&local_C4)->y + 0x8000;
            goto L00EC6B;
          }
        }
        L00EC6B:;
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
            goto L00EE17;
          }
        }
      }
    }
    L00EE17:;
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
          task_250.x = (&local_F4)->x;
          task_250.y = (&local_F4)->y;
          task_250.z = (&local_F4)->z;
          task_25C = (&local_C4)->x;
          *(&task_25C + 1) = (&local_C4)->y;
          *(&task_25C + 2) = (&local_C4)->z;
          break;
        case 5:
          task_250.x = (&local_F4)->x;
          task_250.y = (&local_F4)->y;
          task_250.z = (&local_F4)->z;
          task_25C = (&local_C4)->x;
          *(&task_25C + 1) = (&local_C4)->y;
          *(&task_25C + 2) = (&local_C4)->z;
          if ((local_C0 == 1) | (local_C0 == 2)) {
            local_C0 = 1;
          } else {
            local_C0 = 0;
          }
          break;
        case 7:
          Script_EVT_GetPosition(arg0, -1, &task_250, 0);
          task_25C = (&local_C4)->x;
          *(&task_25C + 1) = (&local_C4)->y;
          *(&task_25C + 2) = (&local_C4)->z;
          break;
      }
    }
  }
  return local_C0;
}

static void script_F0A6(int32_t arg0, int32_t arg1) {
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
    if (task_248 == arg0) {
      if (arg1) {
        Script_ccowUtils(arg0, 3);
      } else {
        Script_ccowUtils(arg0, 4);
      }
    }
  }
  return;
}

static void script_F0D8(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  int32_t local_F8;
  local_F8 = script_E9DD(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
  if (task_248 != arg0) {
    if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
      Script_charaToggleMotEnabled(arg0, 1);
    }
  } else if ((local_F8 == 0) | (local_F8 == 1)) {
    if ((Script_ScenDataUtil(task_E8, 4, arg2 * 13 + 1) & 8) == 0) {
      if ((Script_ScenDataUtil(task_E8, 4, arg2 * 13 + 1) & 0x40) != 0) {
        local_FC = Script_ScenDataUtil(task_E8, 4, arg2 * 13 + 2) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_FC, 0);
      } else {
        local_FC = (Script_ScenDataUtil(task_E8, 4, arg2 * 13 + 2) >> 16) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_FC, 0);
      }
    }
  } else {
    if ((Script_ScenDataUtil(task_E8, 4, arg2 * 13 + 1) & 0x10) == 0) {
      if ((Script_ScenDataUtil(task_E8, 4, arg2 * 13 + 1) & 0x80) != 0) {
        local_FC = (Script_ScenDataUtil(task_E8, 4, arg2 * 13 + 2) >> 16) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_FC, 0);
      } else {
        local_FC = Script_ScenDataUtil(task_E8, 4, arg2 * 13 + 2) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_FC, 0);
      }
    }
  }
  return;
}

__attribute__((scn_string_bindings(0, data_29113)))
static void script_F248(int32_t arg0, int32_t arg1, int32_t arg2) {
  int32_t local_FC;
  if (task_248 == arg0) {
    task_F8.mode = 0;
    task_F8.id = 0;
    if ((arg2 == 0) | (arg2 == 2)) {
      Script_PlayCharaMotion(task_248, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
      if (task_26C & 2) {
        Script_sprintf("Door System : ");
        Script_ryoControl(RYO_RestoreControl);
      }
      Script_shenClearFreeState(1);
      if (arg2 != 2) {
        int32_t local_F8;
        local_F8 = task_130[arg1];
        local_FC = Script_ScenDataUtil(task_E8, 4, local_F8 * 13 + 2) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_FC, 1);
        local_FC = (Script_ScenDataUtil(task_E8, 4, local_F8 * 13 + 2) >> 16) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_FC, 1);
      }
    }
  } else {
    if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
      Script_charaToggleMotEnabled(arg0, 0);
    }
    if ((arg2 == 0) | (arg2 == 2)) {
      int32_t local_F8;
      local_F8 = task_130[arg1];
      local_FC = Script_ScenDataUtil(task_E8, 4, local_F8 * 13 + 2) & 0xFFFF;
      Script_scenToggleCharaFlag(0, local_FC, 1);
      local_FC = (Script_ScenDataUtil(task_E8, 4, local_F8 * 13 + 2) >> 16) & 0xFFFF;
      Script_scenToggleCharaFlag(0, local_FC, 1);
    }
  }
  task_108[arg1] = 1;
  task_1F8[arg1] = 0;
  task_1A8[arg1] = 0;
  task_158[arg1] = 0;
  if (task_220[arg1] != -1) {
    Script_SetMapControlTaskFlag(task_220[arg1], 1);
    task_220[arg1] = -1;
  }
  return;
}

static void script_F404(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  Vector3 local_E8;
  local_FC = 0;
  if (arg0 == 1) {
    local_F4 = arg3;
    if ((local_F4 == 17) | (local_F4 == 11) | (local_F4 == 12)) {
      local_FC = 0;
    } else if ((Script_ScenDataUtil(task_E8, 4, task_130[arg2] * 13 + 1) & 2) == 0) {
      if ((Script_ScenDataUtil(task_E8, 4, task_130[arg2] * 13 + 1) & 4) == 0) {
        local_FC = script_E745(arg2, arg3, local_FC);
      } else {
        local_F4 = arg3;
        if ((local_F4 == 21) | (local_F4 == 22) | (local_F4 == 23)) {
          local_FC = script_E745(arg2, arg3, local_FC);
          goto L00F4C8;
        }
      }
      L00F4C8:;
    } else {
      if ((Script_ScenDataUtil(task_E8, 4, task_130[arg2] * 13 + 1) & 4) == 0) {
        local_FC = script_E745(arg2, arg3, local_FC);
        local_F8 = Script_ScenDataUtil(task_E8, 4, task_130[arg2] * 13 + 6);
        if (local_F8 == 0) {
          local_FC = 0;
        } else {
          if (local_F8 >= 2) {
            Script_EVT_GetPosition(task_248, -1, &local_E8, 0);
            if (Script_CycleManagerUtils(CYCLE_CMD_COUNT_CYCLES_IN_RADIUS, &local_E8, 1.5f)) {
              local_FC = 0;
            }
            goto L00F563;
          }
        }
      }
    }
  }
  L00F563:;
  if (task_1F8[arg2]) {
    local_FC = 0;
  }
  Script_scenUtils(SCENARIO_CMD_SET_HELP_PROMPT_OVERRIDE, local_FC);
  if ((local_FC ^ task_278) & local_FC) {
    Script_sicnUtils(0, 4, 1);
  }
  if ((local_FC ^ task_278) & task_278) {
    Script_sicnUtils(2, 4, 0);
  }
  task_278 = local_FC;
  return;
}

__attribute__((scn_string_bindings(0, data_29122)))
static int32_t script_F5B8(int32_t arg0, int32_t arg1, int32_t arg2) {
  int32_t local_FC;
  int32_t local_F8;
  Vector3 local_EC;
  local_FC = 1;
  local_F8 = arg2;
  if ((local_F8 == 17) | (local_F8 == 11) | (local_F8 == 12)) {
    if (((Script_ScenDataUtil(task_E8, 4, task_130[arg1] * 13 + 1) & 2) == 0) & ((Script_ScenDataUtil(task_E8, 4, task_130[arg1] * 13 + 1) & 4) == 0)) {
      if (task_1F8[arg1] == 0) {
        task_108[arg1] = 4;
        local_FC = 0;
      }
    }
  } else {
    if (Script_GetControllerInput(1) == 0x400) {
      if (-(task_158[arg1] != task_248) & task_278) {
        if ((Script_ScenDataUtil(task_E8, 4, task_130[arg1] * 13 + 1) & 2) == 0) {
          task_108[arg1] = 3;
          task_158[arg1] = task_248;
          task_180[arg1] = 0;
          task_F8.mode = 1;
          task_F8.entry_idx = task_130[arg1];
          local_FC = 0;
        } else {
          if (Script_ScenDataUtil(task_E8, 4, task_130[arg1] * 13 + 6) >= 2) {
            Script_EVT_GetPosition(task_248, -1, &local_EC, 0);
            if (Script_CycleManagerUtils(CYCLE_CMD_COUNT_CYCLES_IN_RADIUS, &local_EC, 1.5f) == 0) {
              task_108[arg1] = 6;
              task_158[arg1] = task_248;
              task_180[arg1] = 0;
              task_F8.mode = 0;
              task_F8.entry_idx = task_130[arg1];
              local_FC = 0;
            }
            goto L00F76C;
          }
        }
        L00F76C:;
        if (local_FC == 0) {
          Script_EVT_GetPosition(task_248, -1, &task_27C, 0);
          Script_ryoControl(PCCTL_RequestNewMode, 4);
          Script_sprintf("Door System : ");
          Script_shenClearFreeState(0);
          Script_ryoControl(PCCTL_DisablePlayerControl);
          task_F8.id = task_248;
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

static void script_F7CE(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  int32_t local_F8;
  local_FC = 0;
  L00F7D6:;
  local_F8 = task_108[local_FC];
  switch (local_F8) {
    case 6:
      if (!(arg0 != -1)) goto L00F7F6;
      L00F7F6:;
      script_F898(local_FC, arg1);
      break;
    case 5:
      if (!(arg0 != -1)) goto L00F816;
      L00F816:;
      script_F8F9(local_FC);
      break;
    case 4:
      if (!(arg0 != -1)) goto L00F834;
      L00F834:;
      script_FA9E(local_FC, arg0, arg1);
      break;
    case 3:
      if (!(arg0 != -1)) goto L00F856;
      L00F856:;
      script_FB36(local_FC, arg1);
      break;
    case 2:
      script_FC18(local_FC);
      break;
    case 1:
      break;
  }
  local_FC = local_FC + 1;
  if (!(local_FC >= 10)) goto L00F7D6;
  return;
}

static void script_F898(int32_t arg0, int32_t arg1) {
  if (task_1F8[arg0] == 0) {
    task_1F8[arg0] = Script_scenCreateTask(script_7A15, 4, task_158[arg0], task_1D0[arg0], arg0, arg1);
    task_1A8[arg0] = task_158[arg0];
    script_E7DD(arg0);
  }
  return;
}

__attribute__((scn_string_bindings(0, data_29131, 1, data_29143)))
static void script_F8F9(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, task_158[arg0])) {
    if (task_1F8[arg0] == 0) {
      if (task_158[arg0] == task_248) {
        task_F8.mode = 1;
        task_F8.entry_idx = task_130[arg0];
        goto L00F94E;
      }
      L00F94E:;
      local_FC = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_130[arg0], 2);
      local_F8 = Script_ScenDataUtil(task_F0, 4, Script_ScenDataUtil(task_F4, 4, local_FC * 9 + 1) * 4 + 1);
      script_F0D8(task_158[arg0], task_1D0[arg0], task_130[arg0], local_F8);
      task_1F8[arg0] = Script_scenCreateTask(Script_ScenDataUtil(&data_28530, 8, local_F8), 4, task_158[arg0], task_1D0[arg0], arg0, local_F8);
      task_1A8[arg0] = task_158[arg0];
      if ((Script_ScenDataUtil(task_E8, 4, task_130[arg0] * 13 + 1) & 0x20000) == 0) {
        Script_sprintf("Door Box is ON!!\012");
        script_E7DD(arg0);
      } else {
        Script_sprintf("Door Box is OFF!!\012");
        task_220[arg0] = -1;
      }
    }
  } else {
    if (task_108[arg0] != 5) {
      task_108[arg0] = 1;
      task_1F8[arg0] = 0;
      task_1A8[arg0] = 0;
      task_158[arg0] = 0;
    }
  }
  return;
}

static void script_FA9E(int32_t arg0, int32_t arg1, int32_t arg2) {
  if ((task_1F8[arg0] == 0) & (arg2 >= 0)) {
    task_158[arg0] = task_248;
    task_F8.mode = 2;
    task_F8.entry_idx = task_130[arg0];
    task_1F8[arg0] = Script_scenCreateTask(Script_ScenDataUtil(&data_28530, 8, arg2), 4, task_158[arg0], task_1D0[arg0], arg0, arg2);
    task_1A8[arg0] = task_158[arg0];
    script_E7DD(arg0);
  }
  return;
}

__attribute__((scn_string_bindings(0, data_29156, 1, data_29168)))
static void script_FB36(int32_t arg0, int32_t arg1) {
  if ((task_1F8[arg0] == 0) & (arg1 >= 0)) {
    script_F0D8(task_158[arg0], task_1D0[arg0], task_130[arg0], arg1);
    task_1F8[arg0] = Script_scenCreateTask(Script_ScenDataUtil(&data_28530, 8, arg1), 4, task_158[arg0], task_1D0[arg0], arg0, arg1);
    task_1A8[arg0] = task_158[arg0];
    if ((Script_ScenDataUtil(task_E8, 4, task_130[arg0] * 13 + 1) & 0x20000) == 0) {
      Script_sprintf("Door Box is ON!!\012");
      script_E7DD(arg0);
    } else {
      Script_sprintf("Door Box is OFF!!\012");
      task_220[arg0] = -1;
    }
  }
  return;
}

static void script_FC18(int32_t arg0) {
  int32_t local_FC;
  task_1F8[arg0] = 0;
  task_1A8[arg0] = 0;
  task_108[arg0] = 1;
  local_FC = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_130[arg0], 2);
  if (local_FC != -1) {
    task_158[arg0] = 0;
  }
  return;
}

static void script_FC6F(int32_t arg0, float arg1, float arg2, float arg3) {
  Vector3 local_F4;
  local_F4.x = arg1;
  (&local_F4)->y = arg2;
  (&local_F4)->z = arg3;
  Script_EVT_SetPosition(arg0, POS_XYZ, &local_F4);
  return;
}

static void script_FC97(int32_t arg0, float arg1, float arg2, float arg3) {
  Vector3i local_F4;
  local_F4.x = (int32_t)(arg1 / 360.0f * (float)0x10000);
  (&local_F4)->y = (int32_t)(arg2 / 360.0f * (float)0x10000);
  (&local_F4)->z = (int32_t)(arg3 / 360.0f * (float)0x10000);
  Script_EVT_SetRotation(arg0, 0x38000000, &local_F4, 0);
  return;
}

static void script_FCEB(int32_t arg0, int32_t arg1, int32_t arg2) {
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

static int32_t script_FD3B(int32_t arg0) {
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

static void script_FDE2(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  local_FC = script_FD3B(arg0);
  if (arg1 > local_FC) {
    (void)0;
  } else {
    (void)-1;
  }
  return;
}

static int32_t script_FE05(int32_t arg0) {
  int32_t unused_FC;
  GameDateTime local_F4;
  int32_t local_F0;
  int32_t local_EC;
  Script_ReturnTimeDate(&local_F4);
  local_F0 = (&local_F4)->hours * 60 + (&local_F4)->minutes;
  local_EC = Script_scenUtils(SCENARIO_CMD_GET_STEP_LUT, arg0) * 6;
  return local_F0 - local_EC;
}

static void script_FE38(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  local_FC = script_FE05(arg0);
  if (arg1 > local_FC) {
    (void)0;
  } else {
    (void)-1;
  }
  return;
}

static void script_FE5B(int32_t arg0, int32_t arg1) {
  int8_t local_F4[12];
  int32_t local_F0;
  local_F0 = script_FE05(arg0);
  local_F4[4] = local_F0 / 60;
  local_F4[5] = local_F0 % 60;
  if ((local_F4[4] < *(int8_t *)(arg1 + 4)) | ((local_F4[4] == *(int8_t *)(arg1 + 4)) & (local_F4[5] < *(int8_t *)(arg1 + 5)))) {
    (void)0;
  } else {
    (void)-1;
  }
  return;
}

static void script_FEBA(int32_t arg0) {
  int32_t local_F8[2];
  local_F8[0] = arg0;
  local_F8[1] = 0;
  (void)local_F8;
  return;
}

__attribute__((scn_string_bindings(0, data_2A1F0)))
static void script_FECF(void) {
  Script_sprintf("TUNED FreeTalk def\012");
  SCNT_Enqueue();
  return;
}

__attribute__((scn_string_bindings(0, data_2A204)))
static void script_FEDE(void) {
  Script_sprintf("TUNED FreeTalk def\012");
  SCNT_Enqueue();
  return;
}

static void script_FEED(void) {
  return;
}

static void script_FEF0(void) {
  return;
}

static void script_FEF3(void) {
  return;
}

static void script_FEF6(void) {
  return;
}

static void script_FEF9(void) {
  (void)((task_2EC = 0) & 0x20);
  return;
}

static int32_t script_FF06(void) {
  return task_2E0 = Script_freeTalkGetCurrentChara();
}

static void script_FF12(void) {
  *(&task_2E0 + 1) = Script_GetFreeTalkStatus();
  return;
}

static int32_t script_FF21(void) {
  return *(&task_2E0 + 2) = Script_freeTalkGetAndResetState();
}

__attribute__((scn_string_bindings(1, data_2A223)))
static void script_FF30(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  task_2F0 = arg0;
  Script_ryoControl(PCCTL_DisablePlayerControl);
  script_100B6();
  local_F8 = arg0;
  switch (local_F8) {
    case 2:
      Script_scenCreateTask(script_10322, 0);
      Script_shenSeqUtils(0);
      while (Script_shenSeqUtils(-1)) {
        SCNF_WaitFrames_0000(1);
      }
      break;
    case 3:
      Script_scenCreateTask(script_103AA, 0);
      Script_shenSeqUtils(1);
      while (Script_shenSeqUtils(-1)) {
        SCNF_WaitFrames_0000(1);
      }
      break;
    case 4:
      Script_LoadSND(0, 2, "bgm109.snd", 0);
      Script_executeAicaCommand(16296, 0, 0);
      Script_executeAicaCommand(1184, 2, 96);
      local_FC = Script_scenCreateTask(script_10322, 0);
      Script_shenSeqUtils(0);
      while (Script_shenSeqUtils(-1)) {
        SCNF_WaitFrames_0000(1);
      }
      Script_srchControl((void *)local_FC);
      if (Script_shenSeqUtils(-7) == 0) {
        local_FC = Script_scenCreateTask(script_103AA, 0);
        Script_shenSeqUtils(1);
        while (Script_shenSeqUtils(-1)) {
          SCNF_WaitFrames_0000(1);
        }
        Script_srchControl((void *)local_FC);
      }
      Script_executeAicaCommand(2720, 2, 20);
      Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
      while (Script_filmCheckOrSetTransitionUtil(-1)) {
        SCNF_WaitFrames_0000(1);
      }
      break;
  }
  Script_set_val_to_1(2, 0);
  Script_LoadSND(0, 2, "SELECT", 0);
  Script_executeAicaCommand(16296, 0, 0);
  Script_executeAicaCommand(1184, 2, 94);
  script_10254();
  return;
}

static void script_100B6(void) {
  script_101CE(CHARA_AKIR);
  if (task_2F0 == 4) {
    script_1016B(SHEN_NAME('GT01'));
    script_1016B(SHEN_NAME('GT02'));
    script_1016B(SHEN_NAME('GT03'));
    Script_EVT_SetPosition(SHEN_NAME('GT01'), POS_XYZ, &data_2A22C);
    Script_EVT_SetPosition(SHEN_NAME('GT02'), POS_XYZ, &data_2A238);
    Script_EVT_SetPosition(SHEN_NAME('GT03'), POS_XYZ, &data_2A244);
    Script_EVT_SetRotation(SHEN_NAME('GT01'), 0x38000000, &data_2A250, 0);
    Script_EVT_SetRotation(SHEN_NAME('GT02'), 0x38000000, &data_2A25C, 0);
    Script_EVT_SetRotation(SHEN_NAME('GT03'), 0x38000000, &data_2A268, 0);
  }
  return;
}

static void script_1016B(int32_t arg0) {
  Script_charaDisplay(arg0, CDM_Show);
  Script_ccowSetFlag80(arg0, 0);
  Script_ccowUtils(arg0, 2);
  Script_scenToggleActiveFor(arg0, 0);
  return;
}

static void script_1018F(int32_t arg0) {
  Script_charaDisplay(arg0, CDM_Show);
  Script_motmToggleFlags(arg0, 2, 0x40);
  Script_ccowSetFlag80(arg0, 0);
  Script_ccowUtils(arg0, 2);
  Script_CharHandlers(arg0, 1, 8, 16);
  Script_CharHandlers(arg0, 0, 8, 16);
  return;
}

static void script_101CE(int32_t arg0) {
  Script_charaDisplay(arg0, CDM_Show);
  Script_motmToggleFlags(arg0, 2, 0x40);
  Script_ccowSetFlag80(arg0, 0);
  Script_ccowUtils(arg0, 2);
  Script_facemotUtils(arg0, 0, 0);
  Script_FaceEx(arg0, 1, 0, 2);
  Script_FACE_LCG(arg0, 0, 6, 0);
  Script_CharHandlers(arg0, 1, 8, 16);
  Script_CharHandlers(arg0, 0, 8, 16);
  Script_FaceLookAtTargetUtils(arg0, 0);
  Script_charaVisUtils(arg0, 1);
  Script_SetCPAMFlags(arg0, 0);
  Script_SetMotnFlag(arg0, -1);
  return;
}

static void script_10254(void) {
  script_102A8(CHARA_AKIR);
  if (task_2F0 == 4) {
    script_1029D(SHEN_NAME('GT01'));
    script_1029D(SHEN_NAME('GT02'));
    script_1029D(SHEN_NAME('GT03'));
  }
  Script_SetGameStateTo(AUTH_None);
  Script_ryoControl(RYO_RestoreControl);
  return;
}

static void script_1029D(int32_t arg0) {
  Script_charaDisplay(arg0, CDM_Hide);
  return;
}

static void script_102A8(int32_t arg0) {
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
  return;
}

static int32_t script_10322(void) {
  int16_t local_FE;
  local_FE = 0;
  while (1) {
    if (Script_shenSeqUtils(-1)) {
      switch (local_FE) {
        case 0:
          Script_lightSetSlotEnabled(8, 0);
          Script_charaDisplay(SHEN_NAME('GT01'), CDM_Show);
          Script_scenCreateTask(script_1041E, 3, SHEN_NAME('GT01'), 7, 40);
          break;
        case 50:
          Script_scenCreateTask(script_1041E, 3, SHEN_NAME('GT02'), 8, 30);
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

static int32_t script_103AA(void) {
  int16_t local_FE;
  local_FE = 0;
  while (1) {
    if (Script_shenSeqUtils(-1)) {
      switch (local_FE) {
        case 0:
          Script_lightSetSlotEnabled(10, 0);
          Script_charaDisplay(SHEN_NAME('GT03'), CDM_Show);
          break;
        case 50:
          Script_scenCreateTask(script_1041E, 3, SHEN_NAME('GT03'), 10, 70);
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

static int32_t script_1041E(int32_t arg0, int32_t arg1, int32_t arg2) {
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

static void script_104B4(int32_t arg0) {
  int32_t local_FC;
  if (arg0 != -1) {
    task_2F4 = arg0;
  }
  local_FC = task_2F4;
  if (local_FC == 0) {
    Script_gachaponUtils(0, data_2A3F0);
  } else if (local_FC == 1) {
    Script_gachaponUtils(0, data_2A499);
  } else if (local_FC == 2) {
    Script_gachaponUtils(0, data_2A5D4);
  } else if (local_FC == 3) {
    Script_gachaponUtils(0, data_2A709);
  } else if (local_FC == 4) {
    Script_gachaponUtils(0, data_2A7B0);
  } else if (local_FC == 5) {
    Script_gachaponUtils(0, data_2A91D);
  } else if (local_FC == 6) {
    Script_gachaponUtils(0, data_2AB16);
  } else if (local_FC == 7) {
    Script_gachaponUtils(0, data_2ABE9);
  } else if (local_FC == 8) {
    Script_gachaponUtils(0, data_2AC5A);
  } else if (local_FC == 9) {
    Script_gachaponUtils(0, data_2ACA7);
  } else if (local_FC == 10) {
    Script_gachaponUtils(0, data_2ACE0);
  } else if (local_FC == 11) {
    Script_gachaponUtils(0, data_2ADCF);
  } else if (local_FC == 12) {
    Script_gachaponUtils(0, data_2AE32);
  } else if (local_FC == 13) {
    Script_gachaponUtils(0, data_2AE87);
  } else if (local_FC == 14) {
    Script_gachaponUtils(0, data_2AECE);
  } else if (local_FC == 15) {
    Script_gachaponUtils(0, data_2AF07);
  } else {
    task_2F4 = -1;
  }
  return;
}

static int16_t script_10655(int8_t arg0, int8_t arg1) {
  int16_t local_FE;
  int8_t local_FD;
  int32_t local_F8;
  local_FE = -1;
  if (arg0 == -1) {
    local_FD = task_2F4;
  } else {
    local_FD = arg0;
  }
  local_F8 = local_FD;
  switch (local_F8) {
    case 0:
      local_FE = *(int16_t *)((int8_t *)&data_2A294 + (arg1 << 1));
      break;
    case 1:
      local_FE = *(int16_t *)((int8_t *)&data_2A2AC + (arg1 << 1));
      break;
    case 2:
      local_FE = *(int16_t *)((int8_t *)&data_2A2D0 + (arg1 << 1));
      break;
    case 3:
      local_FE = *(int16_t *)((int8_t *)&data_2A2FC + (arg1 << 1));
      break;
    case 4:
      local_FE = *(int16_t *)((int8_t *)&data_2A310 + (arg1 << 1));
      break;
    case 5:
      local_FE = *(int16_t *)((int8_t *)&data_2A338 + (arg1 << 1));
      break;
    case 6:
      local_FE = *(int16_t *)((int8_t *)&data_2A35C + (arg1 << 1));
      break;
    case 7:
      local_FE = *(int16_t *)((int8_t *)&data_2A374 + (arg1 << 1));
      break;
    case 8:
      local_FE = *(int16_t *)((int8_t *)&data_2A384 + (arg1 << 1));
      break;
    case 9:
      local_FE = *(int16_t *)((int8_t *)&data_2A38C + (arg1 << 1));
      break;
    case 10:
      local_FE = *(int16_t *)((int8_t *)&data_2A394 + (arg1 << 1));
      break;
    case 11:
      local_FE = *(int16_t *)((int8_t *)&data_2A3B8 + (arg1 << 1));
      break;
    case 12:
      local_FE = *(int16_t *)((int8_t *)&data_2A3C8 + (arg1 << 1));
      break;
    case 13:
      local_FE = *(int16_t *)((int8_t *)&data_2A3D4 + (arg1 << 1));
      break;
    case 14:
      local_FE = *(int16_t *)((int8_t *)&data_2A3E0 + (arg1 << 1));
      break;
    case 15:
      local_FE = *(int16_t *)((int8_t *)&data_2A3E8 + (arg1 << 1));
      break;
  }
  return local_FE;
}

static void script_10840(int32_t arg0, int32_t arg1) {
  for (;;) {
    *(int32_t *)arg0 = Script_FloatUtils(ScriptMathOp_RandomIntBelow, 16);
    if (data_2A274[*(int32_t *)arg0] - data_2A284[*(int32_t *)arg0]) {
      break;
    }
  }
  *(int32_t *)arg1 = Script_FloatUtils(ScriptMathOp_RandomIntBelow, data_2A274[*(int32_t *)arg0] - data_2A284[*(int32_t *)arg0]);
  return;
}

static int32_t script_1088F(int32_t arg0, int32_t arg1) {
  if (arg0 == 0) {
    (void)0;
  } else if (arg0 == 14) {
    (void)0;
  } else if (arg1 < data_2A274[arg0] - data_2A284[arg0]) {
    (void)1;
  } else if (arg0 == 15) {
    (void)3;
  } else {
    (void)2;
  }
  end:;
}

static void script_108E2(int32_t arg0, int32_t arg1, int32_t arg2, float arg3) {
  Vector3 local_F4;
  int8_t local_F3;
  int8_t local_F2;
  int32_t local_EC;
  (void)((local_F3 = 0) & 1);
  if (!(arg3 >= 0.0f)) {
    (void)((local_F2 = -1) & 1);
    arg3 = arg3 * -1.0f;
  } else {
    (void)((local_F2 = 0) & 1);
  }
  local_EC = arg1;
  switch (local_EC) {
    case 1:
      local_EC = arg2;
      if ((local_EC == 0) | (local_EC == 1) | (local_EC == 12) | (local_EC == 13)) {
        (void)((local_F3 = -1) & 1);
        goto L010953;
      }
      L010953:;
      break;
    case 3:
      local_EC = arg2;
      if (local_EC == 6) {
        (void)((local_F3 = -1) & 1);
        goto L01097A;
      }
      L01097A:;
      break;
    case 4:
      local_EC = arg2;
      if ((local_EC == 7) | (local_EC == 15) | (local_EC == 16) | (local_EC == 17) | (local_EC == 18)) {
        (void)((local_F3 = -1) & 1);
        goto L0109B9;
      }
      L0109B9:;
      break;
    case 5:
      (void)((local_F3 = -1) & 1);
      break;
    case 6:
      local_EC = arg2;
      if ((local_EC == 0) | (local_EC == 1) | (local_EC == 2) | (local_EC == 3)) {
        (void)((local_F3 = -1) & 1);
        goto L010A07;
      }
      L010A07:;
      break;
    case 8:
      local_EC = arg2;
      if (local_EC == 3) {
        (void)((local_F3 = -1) & 1);
        goto L010A2E;
      }
      L010A2E:;
      break;
  }
  local_F4.x = (&local_F4)->y = (&local_F4)->z = 1.0f;
  if (local_F3 & 8) {
    arg2 = arg2 + data_2A274[arg1];
    local_F4.x = (&local_F4)->x * arg3;
    (&local_F4)->y = (&local_F4)->y * arg3;
    (&local_F4)->z = (&local_F4)->z * arg3;
  } else {
    local_F4.x = (&local_F4)->x * arg3 * 0.33333f;
    (&local_F4)->y = (&local_F4)->y * arg3 * 0.33333f;
    (&local_F4)->z = (&local_F4)->z * arg3 * 0.33333f;
  }
  if (local_F2 & 4) {
    script_104B4(arg1);
    Script_gachaponUtils(5, arg0, arg2);
  } else {
    script_104B4(arg1);
    Script_gachaponUtils(1, arg0, arg2);
  }
  if ((((scn3_bits){.f = (&local_F4)->x}).i != 0x3F800000) & (((scn3_bits){.f = (&local_F4)->x}).i != 0)) {
    Script_hmdlApplyScale(arg0, &local_F4);
  }
  return;
}

__attribute__((scn_string_bindings(0, data_2B000)))
static void script_10B19(void) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  Vector3 local_E8;
  Vector3i local_DC;
  int32_t local_D8;
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, "/scene/02/JD00/", "GACH") == 0) {
    SCNF_WaitFrames_0000(1);
  }
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2)) {
    SCNF_WaitFrames_0000(1);
  }
  local_F8 = 6;
  local_FC = 0;
  while (local_FC < 6) {
    local_F4 = 1 << local_FC;
    if (local_F8 & local_F4) {
      local_D8 = *(&data_2AF40 + local_FC);
      local_E8.x = *(float *)(data_2AF88 + local_FC * 20);
      (&local_E8)->y = *(float *)(data_2AF88 + (local_FC * 20 + 4));
      (&local_E8)->z = *(float *)(data_2AF88 + (local_FC * 20 + 8));
      local_DC.x = 0;
      (&local_DC)->y = *(int32_t *)(data_2AF88 + (local_FC * 20 + 12));
      (&local_DC)->z = 0;
      Script_EVT_SetPosition(local_D8, POS_XYZ, &local_E8);
      Script_EVT_SetRotation(local_D8, 0x38000000, &local_DC, 0);
      Script_charaDisplay(local_D8, CDM_Show);
      Script_SetOrGetWOCCValue(local_D8, *((int8_t *)data_2AF88 + (local_FC * 20 + 16)));
      Script_MoveObjectToObject(*(&data_2AF70 + local_FC), *(&data_2AF40 + local_FC));
      Script_EVT_SetPosition(*(&data_2AF70 + local_FC), POS_XYZ, &data_2B018);
      Script_EVT_SetRotation(*(&data_2AF70 + local_FC), 0x38000000, &data_2B024, 0);
      Script_charaDisplay(*(&data_2AF70 + local_FC), CDM_Show);
      goto L010C90;
    }
    L010C90:;
    local_FC = local_FC + 1;
  }
  task_2F8 = 0;
  (void)((task_300 = 0) & 2);
  (void)((task_301 = 0) & 2);
  (void)((task_302 = 0) & 2);
  return;
}

static int32_t script_10CC2(int32_t arg0) {
  Vector3 local_F4;
  int32_t unused_E8[3];
  Vector3 local_DC;
  int8_t local_DB;
  uint8_t unused_D8[3];
  float local_D4;
  Script_EVT_GetPosition(arg0, -1, &local_F4, 0);
  Script_EVT_GetPosition(CHARA_AKIR, -1, &local_DC, 0);
  local_D4 = Script_MathUtils(SCRIPT_MATH_TASK_VEC3_DISTANCE, &local_F4, &local_DC);
  if (!(local_D4 >= 1.5f)) {
    (void)((local_DB = -1) & 0x80);
  } else {
    (void)((local_DB = 0) & 0x80);
  }
  if (task_300 & 4) {
    (void)((local_DB = 0) & 0x80);
  }
  return local_DB & 8;
}

static int32_t script_10D2C(int32_t arg0) {
  Vector3 local_F4;
  Vector3 local_E8;
  int8_t local_E7;
  int8_t local_E6;
  int8_t local_E5;
  uint8_t unused_E4;
  int32_t local_E0;
  float local_DC;
  float local_D8;
  (void)((local_E7 = 0) & 0x80);
  (void)((local_E6 = -((local_E5 = 0) != 0)) & 0x10);
  local_F4.x = 0.5f;
  (&local_F4)->y = 0.0f;
  (&local_F4)->z = 1.5f;
  local_E8.x = -0.5f;
  (&local_E8)->y = 0.0f;
  (&local_E8)->z = -0.15f;
  local_E0 = 0;
  while (local_E0 < 6) {
    int32_t local_D4;
    local_D4 = 6;
    if (local_D4 & (1 << local_E0)) {
      int32_t local_D0;
      local_D0 = *(&data_2AF40 + local_E0);
      if (local_D0 != arg0) {
        Vector3 local_C4;
        Script_MoveObjectToObject(local_D0, arg0);
        Script_EVT_GetPosition(local_D0, -1, &local_C4, 0);
        Script_MoveObjectToObject(local_D0, -1);
        {
          float local_C0;
          local_C0 = Script_MathUtils(SCRIPT_MATH_VEC2_LENGTH, (&local_C4)->x, (&local_C4)->z) / 2.0f;
          if (!(local_C0 >= 0.5f)) {
            if (!((&local_C4)->x <= (float)0)) {
              if (~local_E5) {
                (void)((local_E5 = -1) & 1);
                local_D8 = local_C0;
              } else {
                if (!((&local_C4)->x >= local_D8)) {
                  local_D8 = local_C0;
                }
              }
            } else if (~local_E6) {
              (void)((local_E6 = -1) & 1);
              local_DC = (float)0 - local_C0;
            } else {
              if (!((&local_C4)->x <= local_DC)) {
                local_DC = (float)0 - local_C0;
              }
            }
          }
        }
      }
    }
    local_E0 = local_E0 + 1;
  }
  if (local_E5 & 0x20) {
    local_F4.x = local_D8;
  }
  if (local_E6 & 0x40) {
    local_E8.x = local_DC;
  }
  script_13C35(arg0, &local_F4);
  script_13C35(arg0, &local_E8);
  if (Script_SetTaskDataValue(CHARA_AKIR, &local_F4, &local_E8, 0)) {
    Vector3 local_CC;
    Vector3 local_C0;
    Vector3i local_B4;
    Script_EVT_GetPosition(arg0, -1, &local_C0, ENTITY_XFORM_WORLD_SPACE);
    Script_EVT_GetPosition(CHARA_AKIR, -1, &local_CC, ENTITY_XFORM_WORLD_SPACE);
    Script_mtwkGetSetRotation(CHARA_AKIR, &local_B4, 0x42000000);
    local_CC.x = (&local_CC)->x + Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_B4)->y) * 0.3f;
    (&local_CC)->y = (&local_CC)->y;
    (&local_CC)->z = (&local_CC)->z + Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_B4)->y) * 0.3f;
    if (Script_CheckTargetWithinAngleRange(&local_CC, 1.5f, (&local_B4)->y, 5461, &local_C0)) {
      (void)((local_E7 = -1) & 0x10);
    }
  }
  return local_E7 & 0x80;
}

static int32_t script_10F5D(void) {
  int16_t local_FE;
  int32_t local_F8;
  int32_t local_F4;
  int32_t local_F0;
  local_F8 = 0;
  local_FE = 0;
  while (local_FE < 6) {
    local_F4 = 6;
    if (local_F4 & (1 << local_FE)) {
      local_F0 = *(&data_2AF40 + local_FE);
      if (script_10CC2(local_F0)) {
        Script_scenToggleVisibleFor(local_F0, 1);
        Script_scenToggleVisibleFor(*(&data_2AF70 + local_FE), 1);
      } else {
        Script_scenToggleVisibleFor(local_F0, 0);
        Script_scenToggleVisibleFor(*(&data_2AF70 + local_FE), 0);
      }
      if (script_10D2C(local_F0)) {
        local_F8 = local_F0;
        task_2F8 = local_F0;
      }
    }
    local_FE = local_FE + 1;
  }
  task_2F8 = local_F8;
  return local_F8;
}

static int32_t script_1100D(void) {
  int32_t local_FC;
  int8_t local_FB;
  local_FC = 0;
  (void)((local_FB = 0) & 2);
  if (Script_IsPlayerAllowed(0)) {
    if (script_10F5D()) {
      (void)((local_FB = -1) & 2);
      if (Script_GetControllerInput(1) == 0x400) {
        local_FC = 1;
      }
    }
  }
  if (local_FB & 8) {
    if (~task_301) {
      (void)((task_301 = -1) & 2);
      Script_sicnUtils(0, 5, 1);
      Script_uiUtils(0, 6, 0);
    }
  } else {
    if (task_301 & 8) {
      (void)((task_301 = 0) & 2);
      Script_sicnUtils(1, 0, 0);
      Script_uiUtils(0, 0, 0);
    }
  }
  return local_FC;
}

__attribute__((scn_string_bindings(0, data_2B030, 2, data_2B043, 3, data_2B053)))
static int32_t script_110B0(void) {
  Script_LoadVoiceFile(1, "E1066");
  Script_LoadSND(3, 0, 0, 0);
  Script_LoadSND(1, 7, "e1gachap.snd", 0x4000);
  while (Script_LoadSND(2, 0, 0, 0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_LoadSND(4, 0, 0, 0);
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, "/scene/02/JD00/", "HI_GACH") == 0) {
    SCNF_WaitFrames_0000(1);
  }
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2)) {
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

__attribute__((scn_string_bindings(0, data_2B05B, 1, data_2B060, 2, data_2B066)))
static void script_11142(void) {
  Script_LoadSND(0, 7, "FREE", 0);
  Script_LoadVoiceFile(0, "E1066");
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "HI_GACH");
  return;
}

__attribute__((scn_string_bindings(0, data_2B06E)))
static void script_1116F(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  int32_t local_F0;
  int32_t local_EC;
  Vector3 local_E0;
  Vector3i local_D4;
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 5, "HI_GACH") == 0) {
    SCNF_WaitFrames_0000(1);
  }
  local_F8 = 6;
  local_FC = 0;
  while (local_FC < 6) {
    local_F4 = 1 << local_FC;
    if (local_F8 & local_F4) {
      local_F0 = *(&data_2AF40 + local_FC);
      local_EC = *(&data_2AF58 + local_FC);
      Script_EVT_GetPosition(local_F0, -1, &local_E0, 0);
      Script_mtwkGetSetRotation(local_F0, &local_D4, 0);
      if (arg0) {
        Script_EVT_SetPosition(local_EC, POS_XYZ, &local_E0);
        Script_EVT_SetRotation(local_EC, 0x38000000, &local_D4, 0);
        Script_charaDisplay(local_EC, CDM_Show);
        Script_charaDisplay(local_F0, CDM_Hide);
      } else {
        Script_charaDisplay(local_F0, CDM_Show);
        Script_charaDisplay(local_EC, CDM_Hide);
      }
    }
    local_FC = local_FC + 1;
  }
  return;
}

static void script_11241(void) {
  Vector3i local_F4;
  (void)((task_300 = -1) & 8);
  Script_scenCreateTask(script_110B0, 0);
  Script_FaceEx(CHARA_AKIR, 1, 0, 2);
  Script_charaSetRenderOpacity(CHARA_AKIR, 1.0f);
  Script_SetCPAMFlags(CHARA_AKIR, 0);
  Script_scenUtils(SCENARIO_CMD_INIT_SCNT);
  Script_srchUtils(SRCH_SCRIPT_Reset);
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_motmToggleFlags(CHARA_AKIR, 2, 0x40);
  Script_mtwkGetSetRotation(CHARA_AKIR, &local_F4, 0x42000000);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_EVT_SetRotation(CHARA_AKIR, 0x78000000, &local_F4, 0);
  Script_motmToggleFlags(CHARA_AKIR, 1, 0x40);
  Script_motmSetBlendFrames(CHARA_AKIR, MOTM_BLEND_FRAMES_15);
  Script_sicnUtils(1, 0, 0);
  (void)((task_301 = 0) & 8);
  Script_ccowUtils(CHARA_AKIR, 2);
  SCNF_WaitFrames_0000(1);
  Script_uiUtils(0, 6, 0);
  Script_SetCharaRefBoardPos(CHARA_AKIR, &data_2B078);
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 1);
  Script_scenToggleVisibleFor(SHEN_NAME('DAMY'), 1);
  Script_charaDisplay(SHEN_NAME('DAMY'), CDM_Hide);
  Script_scenToggleVisibleFor(SHEN_NAME('GCPG'), 1);
  Script_charaDisplay(SHEN_NAME('GCPG'), CDM_Hide);
  Script_scenToggleVisibleFor(SHEN_NAME('GCPB'), 1);
  Script_charaDisplay(SHEN_NAME('GCPB'), CDM_Hide);
  Script_scenToggleVisibleFor(SHEN_NAME('GCPR'), 1);
  Script_charaDisplay(SHEN_NAME('GCPR'), CDM_Hide);
  Script_scenToggleVisibleFor(SHEN_NAME('GCPY'), 1);
  Script_charaDisplay(SHEN_NAME('GCPY'), CDM_Hide);
  Script_scenToggleVisibleFor(SHEN_NAME('GABS'), 1);
  Script_charaDisplay(SHEN_NAME('GABS'), CDM_Hide);
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
  Script_CycleManagerUtils(CYCLE_CMD_PROCESS_SND_EXTRA);
  return;
}

static void script_113E8(void) {
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 2);
  Script_motmSetBlendFrames(CHARA_AKIR, MOTM_BLEND_FRAMES_5);
  script_11142();
  Script_SetGameStateTo(AUTH_None);
  Script_ccowUtils(CHARA_AKIR, 1);
  Script_ryoControl(RYO_RestoreControl);
  Script_scenUtils(SCENARIO_CMD_UPDATE_TIMER);
  Script_uiUtils(0, 0, 0);
  (void)((task_300 = 0) & 8);
  Script_scenToggleVisibleFor(SHEN_NAME('DAMY'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('GCPG'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('GCPB'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('GCPR'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('GCPY'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('GABS'), 0);
  Script_FaceEx(CHARA_AKIR, 0, 0, 2);
  Script_SetCPAMFlags(CHARA_AKIR, 1);
  Script_shenClearFreeState(1);
  (void)((task_300 = 0) & 8);
  Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_EV_STOP_ACTIVE, 0);
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 1);
  return;
}

static void script_114C2(void) {
  int32_t local_F4[3];
  int32_t local_F0;
  int32_t local_EC;
  int32_t local_E8;
  Script_ResetHandMotion(CHARA_AKIR, 1, 1, 0);
  Script_ResetHandMotion(CHARA_AKIR, 1, 0, 0);
  script_11D3C(CHARA_AKIR, 1, 8, 1);
  script_11D3C(CHARA_AKIR, 0, 8, 1);
  local_F4[0] = local_F4[1] = local_F4[2] = 0;
  local_E8 = 0;
  while (local_E8 < 6) {
    if (task_2F8 == *(&data_2AF40 + local_E8)) {
      local_EC = *((int8_t *)data_2AF88 + (local_E8 * 20 + 17));
      break;
    }
    local_E8 = local_E8 + 1;
  }
  local_F0 = Script_PlayerStatsUtils(PS_GetIndexedU16Table3Slot, local_EC);
  Script_PlayerStatsUtils(PS_SetIndexedU16Table3Slot, local_EC, local_F0 + 1);
  Script_InitializeOsag(CHARA_AKIR);
  return;
}

static void script_11586(void) {
  Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
  Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
  Script_gachaponUtils(2, 0x4849454B);
  return;
}

static void script_115B5(int32_t arg0) {
  Vector3 local_F4;
  Vector3 local_E8;
  float local_DC[3];
  Vector3i local_D0;
  int32_t local_CC;
  Script_EVT_GetPosition(arg0, -1, &local_F4, 0);
  (&local_F4)->y = (&local_F4)->y + 0.6f;
  Script_SetCharaLookAtPosition(CHARA_AKIR, 1, &local_F4, 0);
  SCNF_WaitFrames_0000(1);
  while (Script_IsCharaMotionFinished(CHARA_AKIR) == 0) {
    SCNF_WaitFrames_0000(1);
  }
  local_F4.x = -0.05f;
  (&local_F4)->y = 0.0f;
  (&local_F4)->z = 0.65f;
  script_13C35(arg0, &local_F4);
  Script_mtwkGetSetRotation(arg0, &local_D0, 0);
  Script_scenCharaSubmitMoveTargetRequest(CHARA_AKIR, &local_F4, (&local_D0)->y, MN_AKI_AKI_STAND_LP, 0);
  while (Script_IsMoveTargetRequestActive(CHARA_AKIR)) {
    SCNF_WaitFrames_0000(1);
  }
  SCNF_WaitFrames_0000(1);
  local_F4.x = -0.0511f;
  (&local_F4)->y = 0.0f;
  (&local_F4)->z = 0.650771f;
  script_13C35(task_2F8, &local_F4);
  Script_EVT_GetPosition(CHARA_AKIR, -1, &local_E8, 0);
  local_DC[0] = (&local_F4)->x - (&local_E8)->x;
  local_DC[1] = 0.0f;
  local_DC[2] = (&local_F4)->z - (&local_E8)->z;
  local_CC = 10;
  while (local_CC) {
    local_E8.x = (&local_E8)->x + *local_DC / 10.0f;
    (&local_E8)->z = (&local_E8)->z + local_DC[2] / 10.0f;
    Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &local_E8);
    local_CC = local_CC - 1;
    SCNF_WaitFrames_0000(1);
  }
  Script_SetCharaLookAtPosition(CHARA_AKIR, -1, 0, 0);
  return;
}

static void script_1173E(int32_t arg0, int32_t arg1) {
  Vector3 local_F4;
  Vector3i local_E8;
  if (arg1) {
    local_F4.x = -0.109743f;
    (&local_F4)->y = 0.0f;
    (&local_F4)->z = 0.860585f;
  } else {
    local_F4.x = -0.0511f;
    (&local_F4)->y = 0.0f;
    (&local_F4)->z = 0.650771f;
  }
  script_13C35(task_2F8, &local_F4);
  Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &local_F4);
  Script_mtwkGetSetRotation(task_2F8, &local_E8, 0);
  Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &local_E8, 0);
  return;
}

static int32_t script_117BF(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  Vector3i local_F4;
  Vector3i local_E8;
  Vector3i local_DC;
  int32_t local_D8;
  local_DC.x = arg1;
  (&local_DC)->y = arg2;
  (&local_DC)->z = arg3;
  Script_handSetFor(CHARA_AKIR, arg0, &local_F4);
  local_D8 = 0;
  while (local_D8 < arg4) {
    local_E8.x = (&local_F4)->x + (arg1 - (&local_F4)->x) * local_D8 / arg4;
    (&local_E8)->y = (&local_F4)->y + (arg2 - (&local_F4)->y) * local_D8 / arg4;
    (&local_E8)->z = (&local_F4)->z + (arg3 - (&local_F4)->z) * local_D8 / arg4;
    Script_SetHandRootRotation(CHARA_AKIR, arg0, 0x15, &local_E8);
    local_D8 = local_D8 + 1;
    SCNF_WaitFrames_0000(1);
  }
  Script_SetHandRootRotation(CHARA_AKIR, arg0, 0x15, &local_DC);
  end:;
}

static int32_t script_11879(void) {
  float local_FC;
  int8_t local_FB;
  int8_t local_FA;
  int8_t local_F9;
  int8_t local_F8;
  int8_t local_F7;
  int8_t local_F6;
  (void)((local_FB = -((local_FA = -((local_F9 = 0) != 0)) != 0)) & 1);
  (void)((local_F8 = -((local_F7 = 0) != 0)) & 0x10);
  (void)((local_F6 = 0) & 1);
  for (;;) {
    local_FC = Script_GetCharaMotionRemainingFramesScaled(CHARA_AKIR);
    if (-!(local_FC < 100.0f) & ~local_FB) {
      (void)((local_FB = -1) & 2);
      Script_scenCreateTask(script_119B4, 1, local_FC - 100.0f);
    }
    if (-!(local_FC < 170.0f) & ~local_FA) {
      (void)((local_FA = -1) & 2);
      Script_scenCreateTask(script_11A84, 0);
    }
    if (-!(local_FC < 215.0f) & ~local_F9) {
      (void)((local_F9 = -1) & 2);
      Script_scenCreateTask(script_11B4D, 0);
    }
    if (-!(local_FC < 280.0f) & ~local_F8) {
      (void)((local_F8 = -1) & 2);
      Script_scenCreateTask(script_11BF7, 0);
    }
    if (-!(local_FC < 300.0f) & ~local_F7) {
      (void)((local_F7 = -1) & 2);
      Script_scenCreateTask(script_11C41, 0);
    }
    if (-!(local_FC < 310.0f) & ~local_F6) {
      (void)((local_F6 = -1) & 2);
      Script_scenCreateTask(script_11F2D, 0);
    }
    if (local_F6 & 0x40) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  return Script_srchControl(0);
}

static int32_t script_119B4(float arg0) {
  int32_t local_F4[3];
  int32_t local_F0;
  if (!(40.0f - arg0 <= 0.0f)) {
    local_F0 = Script_FloatUtils(ScriptMathOp_FloatToInt, 40.0f - arg0);
    script_11D3C(CHARA_AKIR, 1, 0, local_F0);
    local_F4[0] = 0;
    local_F4[1] = -5643;
    local_F4[2] = 728;
    script_117BF(1, *(int32_t *)local_F4, local_F4[1], local_F4[2], local_F0);
  }
  script_11D3C(CHARA_AKIR, 1, 1, 16);
  local_F4[1] = -4915;
  script_117BF(1, *(int32_t *)local_F4, local_F4[1], local_F4[2], 8);
  local_F4[1] = -5643;
  script_117BF(1, *(int32_t *)local_F4, local_F4[1], local_F4[2], 8);
  Script_scenCreateTask(script_12B88, 0);
  end:;
}

static int32_t script_11A84(void) {
  int32_t local_F4[3];
  script_11D3C(CHARA_AKIR, 1, 2, 40);
  local_F4[0] = -7827;
  local_F4[1] = 4187;
  local_F4[2] = 4187;
  script_117BF(1, *(int32_t *)local_F4, local_F4[1], local_F4[2], 8);
  local_F4[0] = -7827;
  local_F4[1] = 3458;
  local_F4[2] = 4187;
  script_117BF(1, *(int32_t *)local_F4, local_F4[1], local_F4[2], 8);
  Script_scenCreateTask(script_1243B, 1, 20);
  local_F4[0] = 6735;
  local_F4[1] = 2821;
  local_F4[2] = 1456;
  script_117BF(1, *(int32_t *)local_F4, local_F4[1], local_F4[2], 16);
  SCNF_WaitFrames_0000(4);
  end:;
}

static int32_t script_11B4D(void) {
  int32_t local_F4[3];
  script_11D3C(CHARA_AKIR, 1, 3, 40);
  local_F4[0] = -7827;
  local_F4[1] = 3458;
  local_F4[2] = 4187;
  script_117BF(1, *(int32_t *)local_F4, local_F4[1], local_F4[2], 24);
  Script_scenCreateTask(script_1243B, 1, 14);
  local_F4[0] = 6735;
  local_F4[1] = 2821;
  local_F4[2] = 1456;
  script_117BF(1, *(int32_t *)local_F4, local_F4[1], local_F4[2], 16);
  Script_scenCreateTask(script_12C48, 0);
  SCNF_WaitFrames_0000(4);
  Script_scenCreateTask(script_12F67, 0);
  end:;
}

static int32_t script_11BF7(void) {
  int32_t local_F4[3];
  script_11D3C(CHARA_AKIR, 1, 4, 20);
  local_F4[0] = 2548;
  local_F4[1] = -3822;
  local_F4[2] = -2184;
  script_117BF(1, *(int32_t *)local_F4, local_F4[1], local_F4[2], 20);
  end:;
}

static int32_t script_11C41(void) {
  int32_t local_F4[3];
  script_11D3C(CHARA_AKIR, 1, 5, 20);
  local_F4[0] = -364;
  local_F4[1] = -2730;
  local_F4[2] = -182;
  script_117BF(1, *(int32_t *)local_F4, local_F4[1], local_F4[2], 20);
  local_F4[0] = -546;
  local_F4[1] = -1274;
  local_F4[2] = 2730;
  Script_scenCreateTask(script_117BF, 5, 1, *(int32_t *)local_F4, local_F4[1], local_F4[2], 40);
  local_F4[0] = -7645;
  local_F4[1] = 3822;
  local_F4[2] = 910;
  local_F4[0] = -2419;
  local_F4[1] = 4176;
  local_F4[2] = 5642;
  Script_scenCreateTask(script_117BF, 5, 0, *(int32_t *)local_F4, local_F4[1], local_F4[2], 30);
  script_11D3C(CHARA_AKIR, 1, 6, 10);
  script_11D3C(CHARA_AKIR, 0, 6, 10);
  end:;
}

static void script_11D3C(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t unused_FF20[56];
  int32_t local_FF1C;
  int32_t local_FF18;
  int32_t local_FF14;
  local_FF18 = -1;
  local_FF14 = arg2;
  switch (local_FF14) {
    case 0:
      local_FF18 = (int32_t)data_2B088;
      break;
    case 1:
      local_FF18 = (int32_t)data_2B16C;
      break;
    case 2:
      local_FF18 = (int32_t)data_2B250;
      break;
    case 3:
      local_FF18 = (int32_t)data_2B334;
      break;
    case 4:
      local_FF18 = (int32_t)data_2B418;
      break;
    case 5:
      local_FF18 = (int32_t)data_2B4FC;
      break;
    case 6:
      local_FF18 = (int32_t)data_2B5E0;
      break;
    case 7:
      local_FF18 = (int32_t)data_2B6C4;
      break;
    case 8:
      local_FF18 = (int32_t)data_2B7A8;
      break;
    case 9:
      {
        int32_t local_FF10;
        local_FF10 = 0;
        while (local_FF10 < 57) {
          *(&local_FF1C + local_FF10) = *(int32_t *)(data_2B4FC + (local_FF10 << 2));
          local_FF10 = local_FF10 + 1;
        }
        *(&local_FF1C + 3) = -5643;
        *(&local_FF1C + 4) = 0x2000;
        *(&local_FF1C + 14) = -6917;
        *(&local_FF1C + 17) = -4551;
        local_FF18 = (int32_t)&local_FF1C;
        break;
      }
    case 10:
      {
        int32_t local_FF10;
        local_FF10 = 0;
        while (local_FF10 < 57) {
          *(&local_FF1C + local_FF10) = *(int32_t *)(data_2B4FC + (local_FF10 << 2));
          local_FF10 = local_FF10 + 1;
        }
        *(&local_FF1C + 3) = -5643;
        *(&local_FF1C + 4) = 1820;
        *(&local_FF1C + 14) = -6917;
        *(&local_FF1C + 17) = -13107;
        local_FF18 = (int32_t)&local_FF1C;
        break;
      }
  }
  if ((local_FF18 != -1) & (local_FF18 != 0)) {
    Script_handSetFloatsFor(arg0, arg1, (const int32_t *)local_FF18, arg3);
  }
  return;
}

static int32_t script_11F2D(void) {
  Script_seqEnterEvtCam(7008, task_2F8, task_2F8);
  {
    Vector3 local_F4;
    Vector3i local_E8;
    local_F4.x = 0.126f;
    (&local_F4)->y = 0.035f;
    (&local_F4)->z = -0.056f;
    local_E8.x = 0x4000;
    (&local_E8)->y = 0;
    (&local_E8)->z = 0;
    Script_fixoAttachEx(SHEN_NAME('GABS'), CHARA_AKIR, 18, &local_F4, &local_E8);
    Script_fixoAttachEx(task_2FC, CHARA_AKIR, 18, &local_F4, &local_E8);
  }
  SCNF_WaitFrames_0000(1);
  for (;;) {
    if (Script_GetCharaMotionRemainingFramesScaled(CHARA_AKIR) >= 340.0f) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  {
    Vector3 local_F4;
    Vector3i local_E8;
    local_F4.x = 0.0f;
    (&local_F4)->y = 0.0f;
    (&local_F4)->z = 0.0f;
    local_E8.x = 0x4000;
    (&local_E8)->y = 0x4000;
    (&local_E8)->z = 0x4000;
    Script_fixoAttachEx(SHEN_NAME('KEIH'), task_2FC, 18, &local_F4, &local_E8);
  }
  Script_charaDisplay(SHEN_NAME('KEIH'), CDM_Show);
  SCNF_WaitFrames_0000(1);
  for (;;) {
    if (Script_GetCharaMotionRemainingFramesScaled(CHARA_AKIR) >= 360.0f) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_executeAicaCommand(132265, 0, 0);
  {
    Vector3 local_F4;
    Vector3i local_E8;
    local_F4.x = 0.09f;
    (&local_F4)->y = 0.075f;
    (&local_F4)->z = 0.008f;
    local_E8.x = 13111;
    (&local_E8)->y = 817;
    (&local_E8)->z = 50684;
    Script_fixoAttachEx(task_2FC, CHARA_AKIR, 12, &local_F4, &local_E8);
  }
  SCNF_WaitFrames_0000(1);
  for (;;) {
    if (Script_GetCharaMotionRemainingFramesScaled(CHARA_AKIR) >= 410.0f) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_fixoClearAttachment(SHEN_NAME('GABS'));
  Script_scenCreateTask(script_123BB, 1, SHEN_NAME('GABS'));
  SCNF_WaitFrames_0000(1);
  for (;;) {
    if (Script_GetCharaMotionRemainingFramesScaled(CHARA_AKIR) >= 430.0f) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  script_11D3C(CHARA_AKIR, 1, 5, 20);
  {
    int32_t unused_F4[3];
    int32_t local_E8[3];
    local_E8[0] = 909;
    local_E8[1] = -1657;
    local_E8[2] = 1431;
    Script_scenCreateTask(script_117BF, 5, 1, *(int32_t *)local_E8, local_E8[1], local_E8[2], 20);
  }
  {
    int32_t unused_F4[3];
    int32_t local_E8[3];
    local_E8[0] = -38;
    local_E8[1] = 476;
    local_E8[2] = 9888;
    Script_scenCreateTask(script_117BF, 5, 0, *(int32_t *)local_E8, local_E8[1], local_E8[2], 20);
  }
  Script_scenCreateTask(script_122C9, 0);
  SCNF_WaitFrames_0000(1);
  for (;;) {
    if (Script_GetCharaMotionRemainingFramesScaled(CHARA_AKIR) >= 450.0f) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_seqEnterEvtCam(7009, task_2F8, task_2F8);
  Script_fixoClearAttachment(SHEN_NAME('KEIH'));
  {
    Vector3 local_F4;
    Vector3i local_E8;
    local_F4.x = 0.11f;
    (&local_F4)->y = 0.055f;
    (&local_F4)->z = -0.065f;
    local_E8.x = 29664;
    (&local_E8)->y = 55344;
    (&local_E8)->z = 35490;
    Script_fixoAttachEx(SHEN_NAME('KEIH'), CHARA_AKIR, 18, &local_F4, &local_E8);
  }
  if (task_303 & 1) {
    int32_t unused_E8[6];
    Vector4 local_D8;
    local_D8.x = (&local_D8)->y = (&local_D8)->z = (&local_D8)->w = 0.35f;
    Script_charaSetRefboardFlagTo(SHEN_NAME('KEIH'), 1);
    Script_SetCharaRefBoardPos(SHEN_NAME('KEIH'), &local_D8);
  }
  SCNF_WaitFrames_0000(1);
  for (;;) {
    if (Script_GetCharaMotionRemainingFramesScaled(CHARA_AKIR) >= 503.0f) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_fixoClearAttachment(task_2FC);
  Script_scenCreateTask(script_123BB, 1, task_2FC);
  SCNF_WaitFrames_0000(1);
  while (Script_IsCharaMotionFinished(CHARA_AKIR) == 0) {
    SCNF_WaitFrames_0000(1);
  }
  Script_fixoClearAttachment(SHEN_NAME('KEIH'));
  end:;
}

static int32_t script_122C9(void) {
  Vector3 local_F4;
  int32_t local_F0;
  local_F4.x = *(float *)&data_2B898;
  (&local_F4)->y = *(float *)(&data_2B898 + 1);
  (&local_F4)->z = *(float *)(&data_2B898 + 2);
  local_F0 = 20;
  while (local_F0) {
    local_F4.x = (&local_F4)->x + ((&data_2B88C)->x - *(float *)&data_2B898) / 20.0f;
    (&local_F4)->y = (&local_F4)->y + ((&data_2B88C)->y - *(float *)(&data_2B898 + 1)) / 20.0f;
    (&local_F4)->z = (&local_F4)->z + ((&data_2B88C)->z - *(float *)(&data_2B898 + 2)) / 20.0f;
    Script_fixoClearAttachment(task_2FC);
    Script_fixoAttachEx(task_2FC, CHARA_AKIR, 12, &local_F4, &data_2B8A4);
    local_F0 = local_F0 - 1;
    SCNF_WaitFrames_0000(1);
  }
  Script_fixoAttachEx(task_2FC, CHARA_AKIR, 12, &data_2B88C, &data_2B8A4);
  return Script_srchControl(0);
}

static int32_t script_123BB(int32_t arg0) {
  Vector3 local_F4;
  float local_F0;
  float local_EC;
  int32_t unused_E8;
  Script_EVT_GetPosition(arg0, -1, &local_F4, 0);
  local_EC = 0.0108888894f;
  local_F0 = 0.0f;
  for (;;) {
    if ((&local_F4)->y <= 0.0f) {
      break;
    }
    Script_EVT_SetPosition(arg0, POS_XYZ, &local_F4);
    local_F0 = local_F0 + local_EC;
    (&local_F4)->y = (&local_F4)->y - local_F0;
    SCNF_WaitFrames_0000(1);
  }
  Script_executeAicaCommand(328873, 0, 0);
  Script_charaDisplay(arg0, CDM_Hide);
  Script_scenToggleVisibleFor(arg0, 0);
  return Script_srchControl(0);
}

static int32_t script_1243B(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  {
    Vector3i local_EC;
    Vector3 local_E0;
    float local_DC;
    local_EC.x = 0;
    (&local_EC)->y = 0;
    (&local_EC)->z = -0x8000 / arg0;
    local_FC = 0;
    while (local_FC < 6) {
      if (task_2F8 == *(&data_2AF40 + local_FC)) {
        local_F8 = *(&data_2AF58 + local_FC);
        break;
      }
      local_FC = local_FC + 1;
    }
    Script_charaPosRotUtils(local_F8, 153, &local_E0, 0, 2);
    local_DC = (&local_E0)->y;
    local_FC = 0;
    L0124BB:;
    if (local_FC == arg0 * 6 / 10) {
      Script_executeAicaCommand(66729, 0, 0);
    }
    Script_charaPosRotUtils(local_F8, 152, 0, &local_EC, 1);
    (&local_E0)->y = (&local_E0)->y + (Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 0.003f) - 0.0015f);
    Script_charaPosRotUtils(local_F8, 153, &local_E0, 0, 0);
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
    if (!(local_FC >= arg0)) goto L0124BB;
    (&local_E0)->y = local_DC;
    Script_charaPosRotUtils(local_F8, 153, &local_E0, 0, 0);
  }
  {
    Vector3i local_EC;
    local_EC.x = (&local_EC)->y = (&local_EC)->z = 0;
    Script_charaPosRotUtils(local_F8, 152, 0, &local_EC, 0);
  }
  end:;
}

static void script_1256D(int32_t arg0) {
  int32_t unused_F4[3];
  Vector3i local_E8;
  int32_t local_E4;
  int32_t local_E0;
  int32_t local_DC;
  int32_t unused_D0[3];
  Vector3i local_C4;
  Script_MoveObjectToObject(SHEN_NAME('KEIH'), task_2F8);
  (&local_E8)->y = (&local_E8)->z = 0;
  local_E8.x = 0;
  Script_EVT_SetRotation(SHEN_NAME('KEIH'), 0x38000000, &local_E8, 0);
  Script_MoveObjectToObject(SHEN_NAME('KEIH'), -1);
  Script_MoveObjectToObject(SHEN_NAME('YKUR'), 0x4849454B);
  Script_EVT_SetPosition(SHEN_NAME('YKUR'), POS_XYZ, &data_2B8B0);
  Script_EVT_SetRotation(SHEN_NAME('YKUR'), 0x38000000, &data_2B8BC, 0);
  Script_MoveObjectToObject(SHEN_NAME('YKUR'), -1);
  Script_charaDisplay(CHARA_AKIR, CDM_Hide);
  Script_charaDisplay(SHEN_NAME('YKUR'), CDM_Show);
  Script_gachaponUtils(3, 0x4849454B, &data_2B8E0);
  SCNF_WaitFrames_0000(1);
  local_E0 = Script_scenCreateTask(script_127D9, 0);
  Script_RotateAllAxisWrapper(SHEN_NAME('YKUR'), 1, &local_E8);
  Script_sicnUtils(0, 5, 1);
  Script_shenClearFreeState(1);
  local_E4 = 0;
  while (1) {
    local_DC = Script_GetControllerInput(0);
    if (local_DC & 0x6000) {
      if (local_E4 > -20) {
        local_E4 = local_E4 - 1;
        local_E8.x = (&local_E8)->x - 546;
      }
    } else {
      if (local_DC & 0x9000) {
        if (local_E4 < 20) {
          local_E4 = local_E4 + 1;
          local_E8.x = (&local_E8)->x + 546;
        }
        goto L0126BA;
      }
    }
    L0126BA:;
    Script_SetHandEulerRotationMasked(SHEN_NAME('YKUR'), 1, 0x15, &local_E8);
    Script_fixoClearAttachment(SHEN_NAME('KEIH'));
    Script_fixoSetAttachmentOffset(SHEN_NAME('KEIH'), 0, &data_2B8B0, &data_2B8BC);
    local_C4.x = 0;
    (&local_C4)->y = -(&local_E8)->x;
    (&local_C4)->z = 0;
    Script_fixoAttachEx(SHEN_NAME('KEIH'), SHEN_NAME('YKUR'), 18, &data_2B8C8, &local_C4);
    SCNF_WaitFrames_0000(1);
    if (local_DC & 0x400) {
      if (~task_302) {
        break;
      }
    }
    if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_EV_STOP_ACTIVE) == 1) {
      break;
    }
  }
  Script_sicnUtils(1, 0, 0);
  Script_shenClearFreeState(0);
  Script_gachaponUtils(4, 0x4849454B);
  Script_srchControl((void *)local_E0);
  script_135A1(arg0);
  Script_SetHandEulerRotationMasked(SHEN_NAME('YKUR'), 1, 0x15, &data_2B8D4);
  Script_charaDisplay(SHEN_NAME('KEIH'), CDM_Hide);
  Script_MoveObjectToObject(SHEN_NAME('YKUR'), -1);
  Script_scenToggleVisibleFor(SHEN_NAME('YKUR'), 0);
  Script_charaDisplay(SHEN_NAME('YKUR'), CDM_Hide);
  Script_ResetHandMotion(SHEN_NAME('YKUR'), 0, 1, 0);
  Script_charaDisplay(CHARA_AKIR, CDM_Show);
  return;
}

static int32_t script_127D9(void) {
  Vector3 local_F4;
  int32_t local_E8[3];
  Vector3 local_DC;
  Script_EVT_GetPosition(SHEN_NAME('YKUR'), -1, &local_F4, 0);
  local_E8[0] = local_E8[1] = local_E8[2] = 0;
  for (;;) {
    local_E8[0] = *(int32_t *)local_E8 + 364;
    local_E8[1] = local_E8[1] + 546;
    local_E8[2] = local_E8[2] + 728;
    local_DC.x = (&local_F4)->x + Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, *(int32_t *)local_E8 & 0xFFFF) * 0.005f;
    (&local_DC)->y = (&local_F4)->y + Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, local_E8[1] & 0xFFFF) * 0.005f;
    (&local_DC)->z = (&local_F4)->z + Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, local_E8[2] & 0xFFFF) * 0.005f;
    Script_EVT_SetPosition(SHEN_NAME('YKUR'), POS_XYZ, &local_DC);
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_128B1(void) {
  return;
}

__attribute__((scn_string_bindings(5, data_2B91E)))
static int32_t script_128B4(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  while (task_302 & 0x80) {
    SCNF_WaitFrames_0000(1);
  }
  (void)((task_302 = -1) & 0x10);
  local_F8 = arg0;
  switch (local_F8) {
    case 0:
      local_FC = script_13BE3(3, "E1066A001", "E1066A002", "E1066A003");
      break;
    case 1:
      local_FC = script_13BE3(2, "E1066A004", "E1066A005");
      break;
    case 2:
      local_FC = script_13BE3(3, "E1066A006", "E1066A007", "E1066A008");
      break;
    case 3:
      local_FC = script_13BE3(3, "E1066A009", "E1066A010", "E1066A011");
      break;
    case 4:
      local_FC = script_13BE3(2, "E1066A026", "E1066A027");
      break;
  }
  if (local_FC) {
    while (Script_convGetIsActive((const void *)local_FC)) {
      SCNF_WaitFrames_0000(1);
    }
  }
  (void)((task_302 = 0) & 4);
  return Script_srchControl(0);
}

static int32_t script_129CB(int16_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  int32_t local_F0;
  int32_t local_EC;
  int32_t local_E8;
  while (task_302 & 0x80) {
    SCNF_WaitFrames_0000(1);
  }
  (void)((task_302 = -1) & 4);
  local_F8 = task_2F4;
  local_F4 = script_10655(local_F8, arg0);
  local_EC = script_1088F(local_F8, arg0);
  local_F0 = Script_GetOwnedItemCount(local_F4, 0);
  local_FC = 0;
  if (local_F0) {
    local_FC = script_13BE3(3, "E1066A019", "E1066A020", "E1066A021");
  } else {
    local_E8 = local_EC;
    if (local_E8 == 0) {
      local_FC = script_13BE3(5, "E1066A014", "E1066A015", "E1066A016", "E1066A017", "E1066A018");
    } else if (local_E8 == 1) {
      local_FC = script_13BE3(3, "E1066A022", "E1066A025", "E1066A024");
    } else {
      if ((local_E8 == 2) | (local_E8 == 3)) {
        local_FC = Script_ShowUIMessageFromID("E1066A023");
        goto L012ACF;
      }
    }
  }
  L012ACF:;
  if (local_FC) {
    while (Script_convGetIsActive((const void *)local_FC)) {
      SCNF_WaitFrames_0000(1);
    }
  }
  return (task_302 = 0) & 0x10;
}

static int32_t script_12AF5(void) {
  int32_t local_FC;
  local_FC = Script_shenGetStats(STATS_CASH);
  SCNF_SetCashValue(-100, 1);
  Script_SetStats(STATS_CASH, local_FC - 100);
  return Script_srchControl(0);
}

static void script_12B1E(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  local_F4.x = 0.132f;
  (&local_F4)->y = 0.041f;
  (&local_F4)->z = 0.018f;
  local_E8.x = 19114;
  (&local_E8)->y = 14563;
  (&local_E8)->z = 0;
  Script_scenToggleVisibleFor(0x6E496F63, 1);
  Script_charaDisplay(0x6E496F63, CDM_Show);
  Script_fixoAttachEx(0x6E496F63, CHARA_AKIR, 18, &local_F4, &local_E8);
  return;
}

static int32_t script_12B88(void) {
  Vector3 local_F4;
  int32_t unused_E8[3];
  float local_E4;
  float local_E0;
  Script_fixoClearAttachment(0x6E496F63);
  Script_MoveObjectToObject(0x6E496F63, task_2F8);
  Script_EVT_GetPosition(0x6E496F63, -1, &local_F4, 0);
  local_E4 = (&local_F4)->y - 0.05f;
  local_E0 = 0.002f;
  for (;;) {
    if ((&local_F4)->y < local_E4) {
      break;
    }
    local_F4.x = (&local_F4)->x + 0.001f;
    (&local_F4)->y = (&local_F4)->y - local_E0;
    local_F4.x = (&local_F4)->x + 0.002f;
    local_E0 = local_E0 + 0.002f;
    Script_EVT_SetPosition(0x6E496F63, POS_XYZ, &local_F4);
    SCNF_WaitFrames_0000(1);
  }
  Script_scenToggleVisibleFor(0x6E496F63, 0);
  Script_charaDisplay(0x6E496F63, CDM_Hide);
  Script_executeAicaCommand(1193, 0, 0);
  end:;
}

static int32_t script_12C48(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  Vector3i local_DC;
  int32_t local_D8;
  int32_t unused_CC[3];
  float local_C8;
  int32_t unused_C4;
  float local_C0;
  int32_t local_BC;
  int32_t local_B8;
  Script_executeAicaCommand(197801, 0, 0);
  local_D8 = Script_FloatUtils(ScriptMathOp_RandomIntBelow, 4);
  local_B8 = local_D8;
  if (local_B8 == 0) {
    task_2FC = 0x47504347;
  } else if (local_B8 == 1) {
    task_2FC = 0x42504347;
  } else if (local_B8 == 2) {
    task_2FC = 0x52504347;
  } else {
    task_2FC = 0x59504347;
  }
  Script_EVT_GetPosition(task_2F8, -1, &local_F4, 0);
  Script_mtwkGetSetRotation(task_2F8, &local_E8, 0);
  local_F4.x = (&local_F4)->x + (Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_E8)->y) * 0.2f + ((float)0 - Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_E8)->y)) * -0.1f);
  (&local_F4)->y = (&local_F4)->y + 0.5f;
  (&local_F4)->z = (&local_F4)->z + (Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_E8)->y) * 0.2f + Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_E8)->y) * -0.1f);
  Script_EVT_SetPosition(task_2FC, POS_XYZ, &local_F4);
  Script_EVT_SetPosition(SHEN_NAME('GABS'), POS_XYZ, &local_F4);
  Script_charaDisplay(task_2FC, CDM_Show);
  Script_charaDisplay(SHEN_NAME('GABS'), CDM_Show);
  local_E8.x = (&local_E8)->y = (&local_E8)->z = 0;
  local_DC.x = 728;
  (&local_DC)->y = 546;
  (&local_DC)->y = 364;
  local_C8 = (&local_F4)->y - 0.08f;
  local_C0 = 0.001f;
  for (;;) {
    if (local_C8 > (&local_F4)->y) {
      break;
    }
    (&local_F4)->y = (&local_F4)->y - local_C0;
    local_C0 = local_C0 + 0.001f;
    local_E8.x = (&local_E8)->x + (&local_DC)->x;
    (&local_E8)->y = (&local_E8)->y + (&local_DC)->y;
    (&local_E8)->z = (&local_E8)->z + (&local_DC)->z;
    Script_EVT_SetPosition(task_2FC, POS_XYZ, &local_F4);
    Script_EVT_SetPosition(SHEN_NAME('GABS'), POS_XYZ, &local_F4);
    Script_EVT_SetRotation(task_2FC, 0x38000000, &local_E8, 0);
    Script_EVT_SetRotation(SHEN_NAME('GABS'), 0x38000000, &local_E8, 0);
    SCNF_WaitFrames_0000(1);
  }
  local_E8.x = 910;
  (&local_E8)->y = 1274;
  (&local_E8)->z = 0x8000;
  Script_mtwkGetSetRotation(task_2FC, &local_DC, 0);
  *(float *)&local_DC.x = (float)((&local_E8)->x - (&local_DC)->x) / 10.0f;
  *(float *)&(&local_DC)->y = (float)((&local_E8)->y - (&local_DC)->y) / 10.0f;
  *(float *)&(&local_DC)->z = (float)((&local_E8)->z - (&local_DC)->z) / 10.0f;
  local_BC = 10;
  while (local_BC) {
    local_E8.x = (&local_E8)->x + (&local_DC)->x;
    (&local_E8)->y = (&local_E8)->y + (&local_DC)->y;
    (&local_E8)->z = (&local_E8)->z + (&local_DC)->z;
    script_13C6D(task_2F8, &local_E8);
    Script_EVT_SetRotation(task_2FC, 0x38000000, &local_E8, 0);
    Script_EVT_SetRotation(SHEN_NAME('GABS'), 0x38000000, &local_E8, 0);
    local_BC = local_BC - 1;
    SCNF_WaitFrames_0000(1);
  }
  Script_executeAicaCommand(263337, 0, 0);
  end:;
}

static int32_t script_12F67(void) {
  Vector3 local_F4;
  Script_MoveObjectToObject(CHARA_AKIR, task_2F8);
  Script_EVT_GetPosition(CHARA_AKIR, -1, &local_F4, 0);
  Script_MoveObjectToObject(CHARA_AKIR, -1);
  {
    int32_t local_F0;
    local_F0 = 15;
    while (local_F0) {
      (&local_F4)->z = (&local_F4)->z + 0.002f;
      Script_MoveObjectToObject(CHARA_AKIR, task_2F8);
      Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &local_F4);
      Script_MoveObjectToObject(CHARA_AKIR, -1);
      local_F0 = local_F0 - 1;
      SCNF_WaitFrames_0000(1);
    }
  }
  {
    int32_t local_F0;
    local_F0 = 15;
    while (local_F0) {
      (&local_F4)->z = (&local_F4)->z - 0.002f;
      Script_MoveObjectToObject(CHARA_AKIR, task_2F8);
      Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &local_F4);
      Script_MoveObjectToObject(CHARA_AKIR, -1);
      local_F0 = local_F0 - 1;
      SCNF_WaitFrames_0000(1);
    }
  }
  end:;
}

static void script_13044(void) {
  Script_SetMotnFlags(CHARA_AKIR, 17, &data_2B9E8, 0x39000000);
  return;
}

static int32_t script_1305C(void) {
  for (;;) {
    script_13044();
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_1306F(void) {
  Script_scenToggleVisibleFor(SHEN_NAME('YKUR'), 1);
  Script_ResetHandMotion(SHEN_NAME('YKUR'), 1, 1, 0);
  Script_charaDisplay(SHEN_NAME('YKUR'), CDM_Hide);
  Script_SetHandMotionTargetChara(SHEN_NAME('YKUR'), 1, 0);
  script_11D3C(SHEN_NAME('YKUR'), 1, 9, 1);
  SCNF_WaitFrames_0000(1);
  Script_SetHandMotionTargetChara(SHEN_NAME('YKUR'), 1, SHEN_NAME('KEIH'));
  script_11D3C(SHEN_NAME('YKUR'), 1, 10, 1);
  L0130E3:;
  Script_MoveObjectToObject(SHEN_NAME('YKUR'), 0x4849454B);
  Script_EVT_SetPosition(SHEN_NAME('YKUR'), POS_XYZ, &data_2B9F4);
  Script_EVT_SetRotation(SHEN_NAME('YKUR'), 0x38000000, &data_2BA00, 0);
  Script_MoveObjectToObject(SHEN_NAME('YKUR'), -1);
  SCNF_WaitFrames_0000(1);
  if (!Script_GetHandControlBit3(SHEN_NAME('YKUR'), 1)) goto L0130E3;
  Script_SetHandMotionTargetChara(SHEN_NAME('YKUR'), 1, 0);
  end:;
}

static void script_1314A(void) {
  int32_t unused_F4[3];
  int32_t local_F0;
  int32_t local_EC;
  local_EC = 0;
  local_F0 = task_2F8;
  script_11241();
  Script_sicnUtils(0, 1, 1);
  for (;;) {
    if (script_13225(local_F0, local_EC)) {
      Script_shenClearFreeState(0);
      Script_sicnUtils(2, 1, 0);
      script_13471(local_F0, local_EC);
    } else {
      Script_shenClearFreeState(0);
      Script_sicnUtils(2, 1, 0);
      if (local_EC) {
        script_1116F(0);
        Script_charaSetRefboardFlagTo(CHARA_AKIR, 2);
        Script_motmSetBlendFrames(CHARA_AKIR, MOTM_BLEND_FRAMES_30);
        Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        Script_SetGameStateTo(AUTH_None);
        SCNF_WaitFrames_0000(30);
      }
      L013200:;
      SCNF_WaitFrames_0000(1);
      if (!~task_302) goto L013200;
      break;
    }
    local_EC = local_EC + 1;
  }
  script_113E8();
  return;
}

static int32_t script_13225(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  int32_t local_F8;
  Vector3 local_EC;
  int32_t local_E8;
  int32_t local_E4;
  local_F8 = 0;
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_EV_STOP_ACTIVE) == 1) {
    (void)0;
  } else {
    script_1173E(arg0, arg1);
    Script_EVT_GetPosition(arg0, -1, &local_EC, 0);
    (&local_EC)->y = (&local_EC)->y + 0.6f;
    Script_SetCharaLookAtPosition(CHARA_AKIR, 1, &local_EC, 0);
    if (arg1 == 0) {
      Script_scenCreateTask(script_128B4, 1, 0);
      Script_seqEnterEvtCam(7000, arg0, arg0);
    } else {
      local_F8 = Script_scenCreateTask(script_1305C, 0);
      Script_scenCreateTask(script_128B4, 1, 4);
      Script_seqEnterEvtCam(7010, arg0, arg0);
    }
    L0132C9:;
    SCNF_WaitFrames_0000(1);
    if (!~task_302) goto L0132C9;
    if (arg1 == 0) {
      local_FC = SCNF_RequestActionSelectorFromParams_0008(0xFFFF, 40, 41, 0xFFFF, 0x7FFF, 0xFFFF, ACTION_SELECTOR_MODE_STANDARD_TIMEOUT_CANCEL);
    } else {
      local_FC = SCNF_RequestActionSelectorFromParams_0008(0xFFFF, 4, 34, 0xFFFF, 0x7FFF, 0xFFFF, ACTION_SELECTOR_MODE_STANDARD_TIMEOUT_CANCEL);
    }
    Script_SetCharaLookAtPosition(CHARA_AKIR, -1, 0, 0);
    if ((local_FC == 40) | (local_FC == 4)) {
      local_E8 = script_3C48(1, 100, 1);
      local_E4 = local_E8;
      if (local_E4 == 0) {
        if (arg1 == 0) {
          Script_scenCreateTask(script_128B4, 1, 1);
        }
        local_FC = 1;
      } else if (local_E4 == 1) {
        Script_scenCreateTask(script_128B4, 1, 2);
        SCNF_SetCashValue(0, 1);
        local_FC = 0;
      } else {
        local_FC = 0;
      }
    } else {
      Script_scenCreateTask(script_128B4, 1, 3);
      local_FC = 0;
    }
    if (local_F8) {
      Script_srchControl((void *)local_F8);
    }
    (void)local_FC;
  }
  end:;
}

static void script_133DE(int32_t arg0) {
  int32_t local_FC;
  if (arg0 == 0) {
    local_FC = -0x40800000;
  } else {
    local_FC = 0x42FA0000;
    Script_motmSetBlendFrames(CHARA_AKIR, MOTM_BLEND_FRAMES_10);
  }
  script_11D3C(CHARA_AKIR, 1, 7, 1);
  script_11D3C(CHARA_AKIR, 0, 8, 1);
  Script_PlayCharaMotion(CHARA_AKIR, 4353, -1.0f, -1.0f, local_FC, 0.0f, 0);
  SCNF_WaitFrames_0000(1);
  Script_SetCharaMotionPlaybackRate(CHARA_AKIR, 1.25f);
  script_12B1E();
  Script_scenCreateTask(script_11879, 0);
  return;
}

static void script_13471(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  Script_shenClearFreeState(0);
  script_114C2();
  local_FC = script_136B7();
  if (arg1 == 0) {
    Script_seqEnterEvtCam(7006, arg0, arg0);
    script_1116F(1);
  } else {
    Script_seqEnterEvtCam(7011, arg0, arg0);
  }
  script_133DE(arg1);
  SCNF_WaitFrames_0000(1);
  Script_scenCreateTask(script_12AF5, 0);
  Script_scenCreateTask(script_1306F, 0);
  SCNF_WaitFrames_0000(1);
  while (Script_IsCharaMotionFinished(CHARA_AKIR) == 0) {
    SCNF_WaitFrames_0000(1);
  }
  Script_scenCreateTask(script_129CB, 1, local_FC);
  Script_shenClearFreeState(1);
  script_1256D(local_FC);
  script_11586();
  script_13527();
  return;
}

static void script_13527(void) {
  Vector3 local_F4;
  Script_motmToggleFlags(CHARA_AKIR, 2, 0x40);
  Script_PlayCharaMotion(CHARA_AKIR, 4353, -1.0f, -1.0f, 135.0f, 0.0f, 0);
  script_13044();
  Script_EVT_GetPosition(task_2F8, -1, &local_F4, 0);
  (&local_F4)->y = (&local_F4)->y + 0.6f;
  Script_seqEnterEvtCam(7005, CHARA_AKIR, CHARA_AKIR);
  SCNF_WaitFrames_0000(1);
  Script_motmToggleFlags(CHARA_AKIR, 1, 0x40);
  return;
}

static void script_135A1(int32_t arg0) {
  Vector3 local_F4;
  Vector3 local_E8;
  float local_DC[3];
  Vector3i local_D0;
  int32_t unused_CC;
  int32_t local_C8;
  int32_t local_C4;
  Script_mtwkGetSetRotation(CHARA_AKIR, &local_D0, 0x40000000);
  Script_EVT_GetPosition(SHEN_NAME('YKUR'), -1, &local_E8, ENTITY_XFORM_WORLD_SPACE);
  local_DC[0] = Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_D0)->y) * 0.3f / 30.0f;
  local_DC[1] = 0.0f;
  local_DC[2] = Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_D0)->y) * 0.3f / 30.0f;
  local_C4 = 0;
  while (local_C4 < 30) {
    local_F4.x = (&local_E8)->x + *local_DC * (float)local_C4;
    (&local_F4)->y = (&local_E8)->y + local_DC[1] * (float)local_C4;
    (&local_F4)->z = (&local_E8)->z + local_DC[2] * (float)local_C4;
    Script_EVT_SetPosition(SHEN_NAME('YKUR'), 0x78000000, &local_F4);
    local_C4 = local_C4 + 1;
    SCNF_WaitFrames_0000(1);
  }
  local_C8 = script_10655(-1, arg0);
  Script_ProcessSubtitle("JINGURU01", 0);
  Script_GetCassetteFlags(local_C8, 1, 0, 0);
  SCNF_StartIconSystemMenu(local_C8);
  Script_fixoClearAttachment(SHEN_NAME('KEIH'));
  return;
}

static int32_t script_136B7(void) {
  float local_FC;
  float local_F8;
  int32_t local_F4;
  int32_t local_F0;
  int32_t local_EC;
  int32_t unused_E8;
  GameDateTime local_E0;
  int32_t local_DC;
  int32_t local_D8;
  int32_t local_D4;
  local_F0 = Script_SetOrGetWOCCValue(task_2F8, -1);
  local_FC = Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 1.0f);
  local_EC = 2;
  if (!(local_FC >= 0.333333313f)) {
    local_EC = 0;
  } else {
    local_FC = local_FC - 0.333333313f;
    if (!(local_FC >= 0.05f)) {
      if (data_2A284[local_F0] != 0) {
        local_EC = 3;
      }
    } else {
      local_FC = local_FC - 0.05f;
      if (!(local_FC >= 0.0f)) {
        local_EC = 5;
      } else {
        (void)local_FC;
        if (!(local_FC >= 0.0f)) {
          local_EC = 6;
        } else {
          (void)local_FC;
          if (!(local_FC >= 0.0f)) {
            local_EC = 7;
          } else {
            (void)local_FC;
            if (!(local_FC >= 0.0f)) {
              local_EC = 8;
            } else {
              (void)local_FC;
              if (!(local_FC >= 0.0f)) {
                local_EC = 9;
              }
            }
          }
        }
      }
    }
  }
  if (-(local_EC == 5) & (-(local_F0 == 1) | -(local_F0 == 2))) {
    local_EC = 2;
  }
  if ((-(local_EC == 7) | -(local_EC == 6)) & -(local_F0 != 2)) {
    local_EC = 2;
  }
  if ((-(local_EC == 9) | -(local_EC == 8)) & -(local_F0 != 1)) {
    local_EC = 2;
  }
  local_DC = local_EC;
  switch (local_DC) {
    case 0:
      local_F0 = 0;
      local_F4 = Script_FloatUtils(ScriptMathOp_RandomIntBelow, data_2A274[local_F0]);
      break;
    case 1:
      local_F0 = 14;
      local_F4 = Script_FloatUtils(ScriptMathOp_RandomIntBelow, data_2A274[local_F0]);
      break;
    case 2:
      local_F4 = Script_FloatUtils(ScriptMathOp_RandomIntBelow, data_2A274[local_F0] - data_2A284[local_F0]);
      break;
    case 3:
      local_D8 = data_2A274[local_F0] - data_2A284[local_F0] - 1;
      local_D4 = 0;
      while (local_D4 < data_2A284[local_F0]) {
        if (Script_GetOwnedItemCount(script_10655(local_F0, local_D8 + local_D4), 0) == 0) {
          if (!(Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 1.0f) >= 0.666666687f)) {
            break;
          }
        }
        local_D4 = local_D4 + 1;
      }
      if (local_D4 < data_2A284[local_F0]) {
        local_F4 = data_2A274[local_F0] - local_D4 - 1;
      } else {
        local_D8 = Script_FloatUtils(ScriptMathOp_RandomIntBelow, data_2A284[local_F0]) + 1;
        local_F4 = data_2A274[local_F0] - local_D8;
      }
      break;
    case 4:
      local_F0 = 15;
      local_D8 = 0;
      while (local_D8 < data_2A274[local_F0]) {
        if (Script_GetOwnedItemCount(script_10655(local_F0, local_D8), 0) == 0) {
          if (!(Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 1.0f) >= 0.333333343f)) {
            break;
          }
        }
        local_D8 = local_D8 + 1;
      }
      if (local_D8 < data_2A274[local_F0]) {
        local_F4 = local_D8;
      } else {
        local_F4 = Script_FloatUtils(ScriptMathOp_RandomIntBelow, data_2A274[local_F0]);
      }
      break;
    case 5:
      local_DC = local_F0;
      switch (local_DC) {
        case 3:
          local_F0 = 8;
          local_DC = Script_FloatUtils(ScriptMathOp_RandomIntBelow, 2);
          switch (local_DC) {
            case 0:
              local_F4 = 1;
              break;
            case 1:
              local_F4 = 0;
              break;
          }
          break;
        case 4:
          local_F0 = 7;
          local_DC = Script_FloatUtils(ScriptMathOp_RandomIntBelow, 5);
          switch (local_DC) {
            case 0:
              local_F4 = 2;
              break;
            case 1:
              local_F4 = 3;
              break;
            case 2:
              local_F4 = 4;
              break;
            case 3:
              local_F4 = 5;
              break;
            case 4:
              local_F0 = 8;
              local_F4 = 2;
              break;
          }
          break;
        case 5:
          local_F0 = 8;
          local_F4 = 3;
          break;
        case 6:
          local_F0 = 9;
          local_DC = Script_FloatUtils(ScriptMathOp_RandomIntBelow, 4);
          switch (local_DC) {
            case 0:
              local_F4 = 0;
              break;
            case 1:
              local_F4 = 1;
              break;
            case 2:
              local_F4 = 2;
              break;
            case 3:
              local_F4 = 3;
              break;
          }
          break;
      }
      break;
    case 6:
      local_F0 = 7;
      local_F4 = 1;
      break;
    case 7:
      local_F0 = 7;
      local_F4 = 0;
      break;
    case 8:
      local_F0 = 7;
      local_F4 = 6;
      break;
    case 9:
      local_F0 = 7;
      local_F4 = 7;
      break;
  }
  local_F8 = 1.0f;
  Script_ReturnTimeDate(&local_E0);
  if ((&local_E0)->hours >= 17) {
    local_F8 = -1.0f;
  } else {
    local_F8 = 1.0f;
  }
  if (!(local_F8 >= 0.0f)) {
    (void)((task_303 = -1) & 2);
  } else {
    (void)((task_303 = 0) & 2);
  }
  script_108E2(SHEN_NAME('KEIH'), local_F0, local_F4, local_F8);
  Script_charaDisplay(SHEN_NAME('KEIH'), CDM_Hide);
  return local_F4;
}

static int32_t script_13BE3(int32_t arg0, const char *arg1, const char *arg2, ...) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  local_FC = Script_FloatUtils(ScriptMathOp_RandomIntBelow, arg0);
  local_F4 = local_FC;
  if (local_F4 == 1) {
    local_F8 = Script_ShowUIMessageFromID(arg2);
  } else if (local_F4 == 2) {
    {
      va_list arguments0;
      va_start(arguments0, arg2);
      local_F8 = Script_ShowUIMessageFromID(va_arg(arguments0, const char *));
      va_end(arguments0);
    }
  } else {
    local_F8 = Script_ShowUIMessageFromID(arg1);
  }
  return local_F8;
}

static void script_13C35(int32_t arg0, Vector3 *arg1) {
  Script_MoveObjectToObject(SHEN_NAME('DAMY'), arg0);
  Script_EVT_SetPosition(SHEN_NAME('DAMY'), POS_XYZ, arg1);
  Script_MoveObjectToObject(SHEN_NAME('DAMY'), -1);
  Script_EVT_GetPosition(SHEN_NAME('DAMY'), -1, arg1, 0);
  return;
}

static void script_13C6D(int32_t arg0, Vector3i *arg1) {
  int32_t unused_F4[3];
  Script_MoveObjectToObject(SHEN_NAME('DAMY'), arg0);
  Script_EVT_SetRotation(SHEN_NAME('DAMY'), 0x38000000, arg1, 0);
  Script_MoveObjectToObject(SHEN_NAME('DAMY'), -1);
  Script_mtwkGetSetRotation(SHEN_NAME('DAMY'), arg1, 0);
  return;
}

static int32_t script_13CA5(void) {
  int32_t local_FC;
  int32_t local_F8;
  Vector3 local_EC;
  Vector3 local_E0;
  int32_t local_DC;
  Script_InvestigationUtils(ShenmueUtils_SetupOrSyncAnchors, SHEN_NAME('YKUL'), 0x52554B59);
  Script_InvestigationUtils(ShenmueUtils_SetSpecialAssetList, data_2BA50);
  Script_srchUtils(SRCH_SCRIPT_InitEntryTableFromTaskOffset, &data_2BA60);
  local_EC.x = -13.0396f;
  (&local_EC)->y = 0.0f;
  (&local_EC)->z = 22.571501f;
  local_E0.x = -21.3396f;
  (&local_E0)->y = 0.0f;
  (&local_E0)->z = 37.871498f;
  for (;;) {
    local_FC = Script_srchUtils(-1);
    local_F8 = Script_srchUtils(SRCH_SCRIPT_GetCurrentTargetEntryIndex);
    if (local_F8 == 0) {
      script_13DE7(1);
    }
    if (Script_SetTaskDataValue(CHARA_AKIR, &local_EC, &local_E0, 0)) {
      if (!(task_2B8.y >= 3.7f)) {
        Script_srchUtils(SRCH_SCRIPT_SetEntrySelectorFilter, 1);
      } else {
        Script_srchUtils(SRCH_SCRIPT_SetEntrySelectorFilter, 2);
      }
    } else {
      Script_srchUtils(SRCH_SCRIPT_SetEntrySelectorFilter, -1);
    }
    if ((local_FC > 100) & (local_FC < 130)) {
    } else {
      local_DC = local_FC;
      if ((local_DC == 0x30484347) | (local_DC == 0x31484347) | (local_DC == 0x32484347) | (local_DC == 0x33484347) | (local_DC == 0x34484347) | (local_DC == 0x35484347)) {
        task_2F8 = local_FC;
        script_1314A();
        goto L013DDB;
      }
    }
    L013DDB:;
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_13DE7(int32_t arg0) {
  if ((Script_IsPlayerAllowed(0) & -(arg0 == 0)) | (arg0 == 1)) {
    Script_scenUtils(SCENARIO_CMD_INIT_SCNT);
    Script_ryoControl(PCCTL_DisablePlayerControl);
    Script_ryoControl(PCCTL_UpdateRyoNavYaw, 0x52494B41);
    Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_sicnUtils(1, 0, 0);
    *((int8_t *)&*(int32_t *)&task_2DC + 1) = 0;
    Script_InvestigationUtils(ShenmueUtils_LoadSpecialAssetEntry, data_2BA18);
    Script_InvestigationUtils(ShenmueUtils_BeginInteractionMode, &data_2BF84, 60);
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
    Script_scenUtils(SCENARIO_CMD_UPDATE_TIMER);
    Script_ryoControl(RYO_RestoreControl);
  }
  return;
}

static int32_t script_13EF4(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  local_FC = 0;
  while (SCN_GetIsFighting() != 2) {
    SCNF_WaitFrames_0000(1);
    if (Script_GetFightingStats(-1, 2) == 0) {
      local_FC = local_FC + 1;
    }
  }
  local_F8 = Script_PlayerStatsUtils(PS_GetShugyoCourseActiveFramesSlot, 1);
  local_F8 = local_F8 + local_FC;
  return Script_PlayerStatsUtils(PS_SetShugyoCourseActiveFramesSlot, 1, local_F8);
}

__attribute__((scn_string_bindings(0, data_2BF90, 3, data_2BFAA, 7, data_2BFCE)))
static int32_t script_13F4F(void) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  int32_t unused_F0;
  GameDateTime local_E8;
  int32_t local_E4;
  local_F4 = 0;
  local_FC = Script_StepUtils(GET_STEP_HIGHER, 0) * 100 + Script_StepUtils(GET_STEP_LOWER, 0);
  Script_ReturnTimeDate(&local_E8);
  local_F8 = (&local_E8)->hours * 100 + (&local_E8)->minutes;
  local_E4 = local_FC;
  if (local_E4 == 1901) {
    if (local_F8 > 1100) {
      Script_LoadVoiceFile(1, "E1040");
      if ((int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f) > 0) {
        Script_ShowUIMessageFromID("E1040A028");
      } else {
        Script_ShowUIMessageFromID("E1040A029");
      }
      local_F4 = 1;
    }
  } else {
    if ((local_E4 == 2202) | (local_E4 == 2203)) {
      Script_LoadVoiceFile(1, "E1040");
      local_E4 = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f);
      if (local_E4 == 0) {
        Script_ShowUIMessageFromID("E1040A030");
      } else if (local_E4 == 1) {
        Script_ShowUIMessageFromID("E1040A031");
      } else {
        Script_ShowUIMessageFromID("E1040A032");
      }
      local_F4 = 1;
      goto L01404F;
    }
  }
  L01404F:;
  if (local_F4) {
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_LoadVoiceFile(0, "E1040");
  }
  if ((local_F8 >= 2200) | (local_F8 <= 829)) {
    local_F4 = 1;
  }
  return local_F4;
}

static int32_t script_14091(void) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  int32_t unused_F0;
  GameDateTime local_E8;
  int32_t local_E4;
  local_F4 = 0;
  local_FC = Script_StepUtils(GET_STEP_HIGHER, 0) * 100 + Script_StepUtils(GET_STEP_LOWER, 0);
  Script_ReturnTimeDate(&local_E8);
  local_F8 = (&local_E8)->hours * 100 + (&local_E8)->minutes;
  local_E4 = local_FC;
  switch (local_E4) {
    case 602:
      if (local_F8 > 1430) {
        local_F4 = 1;
      }
      break;
    case 1901:
      if (local_F8 > 1200) {
        local_F4 = 1;
      }
      break;
  }
  if (local_F8 >= 2200) {
    local_F4 = 1;
  }
  if (~local_F4 & -(local_F8 == 1900)) {
    local_F4 = 1;
  }
  return local_F4;
}

static void script_14137(void) {
  int32_t local_FC;
  local_FC = 0;
  Script_wazaUtils(MOVE_UI_CMD_OPEN_MOVES_SCREEN, 0, 0, 0);
  for (;;) {
    SCNF_WaitFrames_0000(1);
    if (Script_GetControllerInput(1) == 0x200) {
      break;
    }
    if (local_FC < 1200) {
      local_FC = local_FC + 1;
    } else {
      break;
    }
  }
  Script_wazaUtils(MOVE_UI_CMD_REQUEST_CLOSE_MOVES_SCREEN, 0, 0, 0);
  return;
}

static int32_t script_14190(void) {
  Script_seqEnterEvtCam(2900, CHARA_AKIR, CHARA_AKIR);
  Script_shenClearFreeState(0);
  for (;;) {
    SCNF_WaitFrames_0000(1);
    if (Script_IsInEventCam() == 0) {
      break;
    }
  }
  Script_shenClearFreeState(1);
  end:;
}

static int32_t script_141CA(int32_t arg0) {
  Script_EVT_SetPosition(arg0, 0x78000000, &data_2BFD4);
  Script_charaDisplay(arg0, CDM_Hide);
  return Script_ccowUtils(arg0, 2);
}

static void script_141ED(void) {
  SCNF_WaitFrames_0000(2);
  while (Script_IsInEventCam() != 0) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static int32_t script_1420D(int32_t arg0) {
  Script_LoadSND(3, 0, 0, 0);
  Script_LoadSND(1, 5, "e1shugyo.snd", 0x8000);
  while (Script_LoadSND(2, 0, 0, 0)) {
    SCNF_WaitFrames_0000(1);
  }
  return Script_LoadSND(4, 0, 0, 0);
}

static void script_14258(void) {
  int32_t local_FC;
  local_FC = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f);
  if (local_FC == 0) {
    Script_ShowUIMessageFromID("E1040A001");
  } else {
    Script_ShowUIMessageFromID("E1040A003");
  }
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_1429F(void) {
  int32_t local_FC;
  local_FC = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f);
  switch (local_FC) {
    case 0:
      Script_ShowUIMessageFromID("E1040A009");
      break;
    case 1:
      Script_ShowUIMessageFromID("E1040A010");
      break;
  }
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_142F2(void) {
  int32_t local_FC;
  local_FC = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f);
  if (local_FC == 0) {
    Script_ShowUIMessageFromID("E1040A012");
  } else if (local_FC == 1) {
    Script_ShowUIMessageFromID("E1040A013");
  } else {
    Script_ShowUIMessageFromID("E1040A014");
  }
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_1434E(void) {
  int32_t local_FC;
  local_FC = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f);
  if (local_FC == 0) {
    Script_ShowUIMessageFromID("E1040A018");
  } else {
    Script_ShowUIMessageFromID("E1040A020");
  }
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_14395(void) {
  int32_t local_FC;
  local_FC = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f);
  if (local_FC == 0) {
    Script_ShowUIMessageFromID("E1040A015");
  } else if (local_FC == 1) {
    Script_ShowUIMessageFromID("E1040A016");
  } else {
    Script_ShowUIMessageFromID("E1040A017");
  }
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

__attribute__((scn_string_bindings(0, data_2C065)))
static void script_143F1(void) {
  int32_t unused_F0[4];
  if (Script_scenToggleVisibleFor(SHEN_NAME('LUV2'), -1) == 0) {
    Script_scenToggleVisibleFor(SHEN_NAME('LUV2'), 1);
  }
  Script_motmToggleFlags(CHARA_AKIR, 2, 0x40);
  Script_SetCPAMFlags(CHARA_AKIR, 0);
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_ryoControl(PCCTL_UpdateRyoNavYaw, 0x52494B41);
  Script_LoadVoiceFile(1, "E1040");
  Script_scenCreateTask(script_141CA, 1, SHEN_NAME('LUV2'));
  Script_FaceEx(CHARA_AKIR, 1, 0, 1);
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 1);
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
  Script_CycleManagerUtils(CYCLE_CMD_PROCESS_SND_EXTRA);
  Script_scenCreateTask(script_1420D, 1, 1);
  Script_scenCreateTask(script_14190, 0);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_MIMAWASU_K, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  return;
}

__attribute__((scn_string_bindings(0, data_2C06B)))
static void script_144BF(void) {
  Script_vfToggleBattleState(scnPostResultsSetup);
  while (SCN_GetIsFighting() != 2) {
    SCNF_WaitFrames_0000(1);
  }
  Script_shenClearFreeState(1);
  if (Script_scenToggleVisibleFor(SHEN_NAME('LUV2'), -1)) {
    Script_scenToggleVisibleFor(SHEN_NAME('LUV2'), 0);
  }
  Script_LoadVoiceFile(0, "E1040");
  while (Script_GetConversationDetails(CONV_DETAILS_MODE_GET_GAME_STATE, 0) != 7) {
    SCNF_WaitFrames_0000(1);
  }
  Script_motmToggleFlags(CHARA_AKIR, 1, 0x40);
  Script_FaceEx(CHARA_AKIR, 0, 0, 1);
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 2);
  return;
}

static void script_14551(void) {
  int32_t local_FC;
  local_FC = 0;
  while (Script_GetFightingStats(-3, 2)) {
    if (local_FC > 1200) {
      Script_vfToggleBattleState(scnExitConfirmPending);
      break;
    }
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_1458E(void) {
  int32_t local_FC;
  local_FC = 0;
  while (Script_GetFightingStats(-4, 2)) {
    if (local_FC > 300) {
      Script_vfToggleBattleState(scnExitConfirmPending);
      break;
    }
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static int32_t script_145CB(int32_t arg0) {
  int32_t local_FC;
  int8_t local_FB;
  int8_t local_FA;
  int8_t local_F9;
  int32_t local_F4;
  local_F4 = arg0;
  if (local_F4 == 4) {
    local_FB = 58;
    local_FA = 34;
    local_F9 = local_FB;
  } else {
    local_FB = 37;
    local_FA = 34;
    local_F9 = local_FA;
  }
  if (SCNF_RequestActionSelectorFromParams_0008(0xFFFF, local_FB, local_FA, 0xFFFF, 300, 0xFFFF, ACTION_SELECTOR_MODE_STANDARD_TIMEOUT_CANCEL) == local_F9) {
    local_FC = 1;
  } else {
    local_FC = -1;
  }
  return local_FC;
}

static void script_1463C(int32_t arg0) {
  Vector3i local_F4;
  Script_uiUtils(0, 15, 0);
  Script_vfSetup(0, 20, 20, 0, 0);
  Script_vfStageAssignRingBoundary(&data_2C074, 6.0f);
  task_0 = 0x52494B41;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 0;
  *(&task_0 + 3) = 0;
  *(&task_0 + 4) = 5;
  *(&task_0 + 5) = 0;
  *(&task_0 + 6) = 0;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0;
  Script_vfUtils(&task_0);
  task_0 = 0x3256554C;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 5;
  *(&task_0 + 5) = 21;
  *(&task_0 + 6) = 0;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(SHEN_NAME('LUV2'), VF_FORMATION_APPLY_FLAGS, 0x200000, 0x800);
  Script_shenClearFreeState(0);
  Script_filmCheckOrSetTransitionUtil(15, 0, 0, 0, 0, 255, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_FaceEx(CHARA_AKIR, 0, 0, 1);
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 2);
  Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &data_2C080);
  (&local_F4)->y = 0x7000;
  Script_EVT_SetRotation(CHARA_AKIR, MATRIX_XFORM_Y, &local_F4, 0);
  SCNF_WaitFrames_0000(1);
  Script_seqEnterEvtCam(2903, CHARA_AKIR, CHARA_AKIR);
  SCNF_WaitFrames_0000(1);
  script_141ED();
  SCN_Enqueue_VFGI(2, 99, 1.0f, 4, -1);
  Script_filmCheckOrSetTransitionUtil(15, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_shenClearFreeState(1);
  Script_scenCreateTask(script_13EF4, 1, arg0);
  return;
}

static int32_t script_14857(void) {
  int32_t local_FC;
  local_FC = 0;
  if (Script_GetFightingStats(-1, 2) == 1) {
    local_FC = 1;
  }
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_EV_STOP_ACTIVE) == 1) {
    local_FC = 2;
  }
  if (script_14091()) {
    local_FC = 2;
  }
  return local_FC;
}

static void script_1489F(void) {
  script_1434E();
  return;
}

static void script_148A7(void) {
  SCN_Enqueue_VFGI(2, 99, 1.0f, 99, -1);
  return;
}

static void script_148BB(void) {
  int8_t local_FF;
  int8_t local_FE;
  int32_t local_F8;
  int32_t local_F4;
  local_F8 = Script_StepUtils(GET_STEP_HIGHER, 0) * 100 + Script_StepUtils(GET_STEP_LOWER, 0);
  local_FE = 0;
  local_FF = 0;
  if (~Script_IsPlayerAllowed(0)) {
    local_FE = 1;
  }
  if (Script_CycleManagerUtils(CYCLE_CMD_GET_NEAR_NPC_COUNT_CLAMPED) > 0) {
    local_FE = 1;
  } else {
    if ((local_F8 > 100) & (local_F8 < 103)) {
      if (Script_scenCheckCharaExist(SHEN_NAME('YJHI'))) {
        Script_sprintf("\012 itou ga iru\012\012");
        local_FE = 1;
      }
      if (Script_scenCheckCharaExist(CHARA_MAYM)) {
        Script_sprintf("\012 mayumi ga iru\012\012");
        local_FE = 1;
      }
    }
    if (-(local_F8 > 1600) & -(local_F8 < 1902) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_SECRET_OF_YIN_YANG) == 0)) {
      if (Script_scenCheckCharaExist(CHARA_YAMA)) {
        Script_sprintf("\012 yamagisi ga iru\012\012");
        local_FE = 1;
      }
    }
  }
  if (script_13F4F()) {
    local_FE = 1;
  }
  if (local_FE == 0) {
    script_143F1();
    local_FF = 1;
  }
  for (;;) {
    script_14551();
    script_1458E();
    if (local_FE == 1) {
      break;
      local_FF = 14;
    }
    SCNF_WaitFrames_0000(1);
    local_F4 = local_FF;
    if (local_F4 == 1) {
      script_14258();
      local_FF = 2;
    } else if (local_F4 == 2) {
      script_1429F();
      local_FF = 4;
    } else if ((local_F4 == 3) | (local_F4 == 13) | (local_F4 == 14)) {
      break;
    } else if ((local_F4 == 4) | (local_F4 == 12)) {
      local_FF = local_FF + script_145CB(local_FF);
    } else if (local_F4 == 5) {
      local_FE = 2;
      script_142F2();
      script_1463C(0);
      local_FF = 6;
    } else if (local_F4 == 6) {
      local_FF = local_FF + script_14857();
    } else if ((local_F4 == 7) | (local_F4 == 8)) {
      local_FF = local_FF - 7 + 9;
    } else {
      switch (local_F4) {
        case 9:
          if (Script_GetFightingStats(-1, 2) == 1) {
            script_1489F();
            local_FF = 12;
          }
          break;
        case 10:
          if (Script_GetFightingStats(-1, 2) == 1) {
            script_1489F();
            local_FF = 14;
          }
          break;
        case 11:
          script_148A7();
          local_FF = 6;
          break;
      }
    }
  }
  if (local_FE != 1) {
    script_144BF();
  }
  if (local_FE == 2) {
    script_14137();
    Script_PlayerStatsUtils(PS_SetShugyoCourseClearCountSlot, 1, Script_PlayerStatsUtils(PS_GetShugyoCourseClearCountSlot, 1) + 1);
  }
  if (local_FE != 1) {
    Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_CharHandlers(CHARA_AKIR, -1, 0, 0);
    Script_SetGameStateTo(AUTH_None);
    Script_SetCPAMFlags(CHARA_AKIR, 1);
    Script_scenUtils(SCENARIO_CMD_UPDATE_TIMER);
    Script_shenClearFreeState(1);
    Script_ryoControl(RYO_RestoreControl);
    SCNF_WaitFrames_0000(1);
    Script_uiUtils(0, 0, 0);
    Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 1);
  }
  return;
}

static void script_14BBD(int32_t arg0, int32_t arg1) {
  task_304 = arg0;
  script_151BB(arg1);
  task_318 = 9999;
  task_31C = 9999;
  task_320 = 0;
  task_324 = 0;
  task_32D = 0;
  task_330 = 0;
  script_14EF5();
  task_334 = 0;
  task_308 = 0x52554B59;
  task_30C = 0x4C554B59;
  return;
}

static void script_14C1C(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
  script_14BBD(arg0, arg1);
  task_328 = Script_scenCreateTask(script_14C43, 4, arg2, arg3, arg4, arg5);
  return;
}

static int32_t script_14C43(float arg0, float arg1, float arg2, float arg3) {
  Vector3 local_F4;
  Vector3i local_E8;
  Vector3 local_DC;
  float local_D8;
  int8_t local_D7;
  local_F4.x = arg0;
  (&local_F4)->y = arg1;
  (&local_F4)->z = arg2;
  local_D8 = arg3 + 180.0f;
  local_D7 = 0;
  task_32E = 0;
  task_32F = 0;
  for (;;) {
    Script_EVT_GetPosition(CHARA_AKIR, -1, &local_DC, ENTITY_XFORM_WORLD_SPACE);
    Script_mtwkGetSetRotation(CHARA_AKIR, &local_E8, 0x42000000);
    if (Script_CheckTargetWithinAngleRange(&local_F4, 2.0f, (int32_t)(local_D8 / 360.0f * (float)0x10000), 12743, &local_DC) & Script_CheckTargetWithinAngleRange(&local_DC, 2.0f, (&local_E8)->y, 12743, &local_F4)) {
      if (Script_IsPlayerAllowed(0) & -(task_32D == 0)) {
        if (local_D7 == 0) {
          Script_uiUtils(0, 9, 0);
          Script_sicnUtils(0, 5, 1);
          local_D7 = 1;
        }
        if (Script_GetControllerInput(1) == 0x400) {
          if ((local_D7 == 1) & (MMTK_ScriptMemoToggle(2, 0) == 0)) {
            Script_uiUtils(0, 0, 0);
            Script_sicnUtils(1, 0, 0);
            local_D7 = 0;
          }
          task_32E = 1;
          script_15327();
          task_32E = 0;
        }
      } else {
        if ((local_D7 == 1) & (MMTK_ScriptMemoToggle(2, 0) == 0)) {
          Script_uiUtils(0, 0, 0);
          Script_sicnUtils(1, 0, 0);
          local_D7 = 0;
        }
      }
    } else {
      if ((local_D7 == 1) & (MMTK_ScriptMemoToggle(2, 0) == 0)) {
        Script_uiUtils(0, 0, 0);
        Script_sicnUtils(1, 0, 0);
        local_D7 = 0;
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_14DE1(void) {
  {
    int32_t local_FC;
    local_FC = 0;
    task_32C = 0;
    for (;;) {
      if ((Script_GetControllerInput(1) == 0x400) | (Script_GetControllerInput(1) == 0x200)) {
        task_32C = 1;
        Script_TELMUtils(TELM_CMD_RequestHangUp, task_304);
        break;
      }
      local_FC = local_FC + 1;
      if (local_FC == 300) {
        task_32C = 1;
        Script_TELMUtils(TELM_CMD_RequestHangUp, task_304);
        break;
      }
      SCNF_WaitFrames_0000(1);
    }
  }
  if (task_324) {
    int32_t local_FC;
    local_FC = 0;
    for (;;) {
      if ((task_324 == 0) | (local_FC < -200)) {
        break;
      }
      local_FC = local_FC - 5;
      Script_SetSequenceAudioVolume(0, (void *)task_324, local_FC);
      SCNF_WaitFrames_0000(1);
    }
    if (task_324) {
      Script_StartOrFreeSEQ((void *)task_324);
    }
  }
  return Script_srchControl(0);
}

static void script_14EA8(const char *arg0, int32_t arg1) {
  task_338[arg1] = Script_TELMUtils(TELM_CMD_RegisterPhoneNumber, task_304, arg0);
  if (task_310 == arg1) {
    task_318 = task_338[arg1];
  }
  if (task_314 == arg1) {
    task_31C = task_338[arg1];
  }
  return;
}

static void script_14EF5(void) {
  script_14EA8((const char *)&data_2C28C, 0);
  script_14EA8((const char *)&data_2C290, 1);
  script_14EA8((const char *)&data_2C294, 2);
  script_14EA8((const char *)&data_2C298, 7);
  script_14EA8("61564", 3);
  script_14EA8("37742", 4);
  script_14EA8("37749", 5);
  script_14EA8("22550", 6);
  script_14EA8("046861564", 8);
  script_14EA8("046837742", 9);
  script_14EA8("046837749", 10);
  script_14EA8("046822550", 11);
  script_14EA8("2?????", 12);
  script_14EA8("5?????", 13);
  script_14EA8("7?????", 14);
  script_14EA8("8?????", 15);
  script_14EA8("3?????", 16);
  script_14EA8("4?????", 17);
  script_14EA8("6?????", 18);
  script_14EA8("9?????", 19);
  script_14EA8("0?????????", 20);
  script_14EA8("1?????", 21);
  return;
}

static int32_t script_1502C(int32_t arg0) {
  if (arg0 > 0) {
    if ((arg0 == task_318) | (arg0 == task_31C)) {
      Script_scenCreateTask(script_14DE1, 0);
      for (;;) {
        Script_executeAicaCommand(394667, 0, 0);
        SCNF_WaitFrames_0000(15);
        Script_executeAicaCommand(460203, 0, 0);
        SCNF_WaitFrames_0000(15);
        if (task_32C) {
          break;
        }
        SCNF_WaitFrames_0000(1);
      }
      arg0 = Script_TELMUtils(TELM_CMD_GetActiveResult, task_304);
    }
  }
  return arg0;
}

static void script_150A5(void) {
  if (task_32D == 0) {
    if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_TOKI_TEGS_CUTSCENE_COMPLETE) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_PHONE_I_BRANCH_UNLOCKED) == 0)) {
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 185, 1);
    }
    task_32D = script_15107();
    if (task_32D == 0) {
      task_32D = task_32D + 1;
      if (task_32E) {
        task_32F = 1;
      }
    }
  }
  return;
}

static int8_t script_15107(void) {
  int32_t local_FC;
  int8_t local_FB;
  local_FC = Script_shenGetStats(STATS_CASH);
  local_FB = 0;
  if ((Script_StepUtils(GET_STEP_HIGHER, 0) == 17) & (Script_StepUtils(GET_STEP_LOWER, 0) <= 5)) {
    local_FB = -1;
  }
  if (local_FC < 10) {
    local_FB = -15;
  }
  return local_FB;
}

__attribute__((scn_string_bindings(0, data_2C326, 3, data_2C340, 4, data_2C34A)))
static void script_15152(int32_t arg0) {
  int32_t local_FC;
  Script_LoadVoiceFile(1, "E1066");
  local_FC = arg0;
  if (local_FC == -1) {
    Script_ShowUIMessageFromID("E1066A012");
  } else if (local_FC == -10) {
    Script_ShowUIMessageFromID("E1066A013");
  } else {
    Script_ShowUIMessageFromID("E1066A006");
  }
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_LoadVoiceFile(0, "E1066");
  return;
}

static void script_151BB(int32_t arg0) {
  int32_t local_FC;
  task_310 = arg0;
  local_FC = arg0;
  if (local_FC == 4) {
    task_314 = 9;
  } else {
    task_314 = 9999;
  }
  return;
}

static void script_151E6(void) {
  Script_convSetCamInactiveFlag(0);
  if ((task_310 == 9998) | (task_310 == 9997)) {
    SCNF_SetCashValue(10, 1);
  }
  return;
}

static int32_t script_1520D(int32_t arg0) {
  int32_t unused_FC;
  GameDateTime local_F4;
  Script_ReturnTimeDate(&local_F4);
  if ((&local_F4)->hours <= 4) {
    arg0 = -1;
  } else {
    if (((&local_F4)->hours >= 23) & ((&local_F4)->minutes >= 30)) {
      arg0 = -1;
      goto L01524B;
    }
  }
  L01524B:;
  return arg0;
}

static int8_t script_1524F(int32_t arg0) {
  int8_t local_FF;
  int8_t local_FE;
  local_FF = 0;
  local_FE = 1;
  for (;;) {
    Script_executeAicaCommand(263595, 0, 0);
    {
      int8_t local_FD;
      local_FD = 0;
      for (;;) {
        if (Script_GetControllerInput(1) == 0x200) {
          local_FE = 0;
          break;
        }
        local_FD = local_FD + 1;
        if (local_FD > 45) {
          break;
        }
        SCNF_WaitFrames_0000(1);
      }
    }
    Script_executeAicaCommand(329131, 0, 0);
    if (local_FE == 0) {
      break;
    }
    {
      int8_t local_FD;
      local_FD = 0;
      for (;;) {
        if (Script_GetControllerInput(1) == 0x200) {
          local_FE = 0;
          break;
        }
        local_FD = local_FD + 1;
        if (local_FD > 45) {
          break;
        }
        SCNF_WaitFrames_0000(1);
      }
    }
    if (local_FE == 0) {
      break;
    }
    local_FF = local_FF + 1;
    if (local_FF == arg0) {
      break;
    }
  }
  return local_FE;
}

static void script_15327(void) {
  script_150A5();
  script_15334();
  return;
}

static void script_15334(void) {
  int32_t unused_F4[3];
  Vector4 local_E4;
  int32_t local_E0;
  int32_t local_DC;
  float local_D8;
  int32_t unused_D4;
  int8_t local_D3;
  uint8_t unused_D0[3];
  if (task_32D == 1) {
    Script_scenToggleActiveFor(CHARA_AKIR, 0);
    Script_ryoControl(PCCTL_UpdateRyoNavYaw, 0x52494B41);
    Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_ryoControl(PCCTL_DisablePlayerControl);
    Script_ccowUtils(CHARA_AKIR, 2);
    Script_scenToggleVisibleFor(task_304, 1);
    if (0) {
      Script_charaDisplay(task_30C, CDM_Hide);
    } else {
      if (-1) {
        Script_charaDisplay(task_30C, CDM_Show);
        goto L0153B8;
      }
    }
    L0153B8:;
    Script_scenToggleVisibleFor(task_30C, 1);
    if (0) {
      Script_charaDisplay(task_308, CDM_Hide);
    } else {
      if (-1) {
        Script_charaDisplay(task_308, CDM_Show);
        goto L0153E6;
      }
    }
    L0153E6:;
    Script_scenToggleVisibleFor(task_308, 1);
    Script_InvestigationUtils(ShenmueUtils_SetupOrSyncAnchors, task_30C, task_308);
    local_D8 = Script_charaSetClipAngle(task_304, -1.0f);
    Script_charaSetClipAngle(task_304, 1.0f);
    SCNF_WaitFrames_0000(1);
    script_2135E();
    script_16303();
    script_1579E();
    local_E0 = 0;
    local_DC = 0;
    task_396 = 0;
    for (;;) {
      if (MMTK_ScriptMemoToggle(2, 0) == 0) {
        if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_EV_STOP_ACTIVE)) {
          task_396 = 1;
          Script_sicnUtils(1, 0, 0);
          Script_uiUtils(0, 0, 0);
          break;
        } else if (Script_GetControllerInput(1) == 0x400) {
          local_D3 = script_15107();
          if (local_D3) {
            script_15152(local_D3);
            Script_sicnUtils(1, 0, 0);
            Script_uiUtils(0, 0, 0);
            break;
          } else {
            Script_sicnUtils(1, 0, 0);
            script_1579E();
            local_E0 = 0;
            local_DC = 0;
          }
        } else if ((Script_GetControllerInput(1) == 0x200) | (Script_GetControllerInput(1) == 0x2000) | (local_E0 > 1800)) {
          Script_sicnUtils(1, 0, 0);
          Script_uiUtils(0, 0, 0);
          break;
        } else {
          if (Script_GetControllerInput(1) == 4) {
            MMTK_ScriptMemoToggle(0, 0);
            goto L01553F;
          }
        }
        L01553F:;
        local_E0 = local_E0 + 1;
      } else {
        if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_EV_STOP_ACTIVE)) {
          MMTK_ScriptMemoToggle(1, 0);
          goto L015563;
        }
      }
      L015563:;
      SCNF_WaitFrames_0000(1);
    }
    if ((task_310 == 9998) | (task_310 == 4)) {
      script_216CD();
      SCNF_WaitFrames_0000(1);
      SCNF_WaitFrames_0000(1);
      SCNF_WaitFrames_0000(1);
      SCNF_WaitFrames_0000(1);
      Script_convSetCamInactiveFlag(0);
      Script_SetCPAMFlags(CHARA_AKIR, 1);
      Script_charaSetRefboardFlagTo(task_308, 0);
      Script_charaSetRefboardFlagTo(task_30C, 0);
      Script_charaSetRefboardFlagTo(CHARA_AKIR, 2);
      Script_SetGameStateTo(AUTH_None);
      Script_SetCameraFieldOfView(10922);
      Script_scenToggleActiveFor(CHARA_AKIR, 1);
    }
    script_2145D();
    Script_ccowUtils(CHARA_AKIR, 1);
    Script_ryoControl(RYO_RestoreControl);
    Script_charaSetRefboardFlagTo(task_308, 0);
    Script_charaSetRefboardFlagTo(task_30C, 0);
    Script_charaSetRefboardFlagTo(CHARA_AKIR, 2);
    Script_charaSetClipAngle(task_304, local_D8);
    Script_scenToggleVisibleFor(task_304, 0);
    if (-1) {
      Script_charaDisplay(task_308, CDM_Hide);
    } else {
      if (0) {
        Script_charaDisplay(task_308, CDM_Show);
        goto L01565E;
      }
    }
    L01565E:;
    Script_scenToggleVisibleFor(task_308, 0);
    if (-1) {
      Script_charaDisplay(task_30C, CDM_Hide);
    } else {
      if (0) {
        Script_charaDisplay(task_30C, CDM_Show);
        goto L01568C;
      }
    }
    L01568C:;
    Script_scenToggleVisibleFor(task_30C, 0);
    task_32D = 0;
    task_32F = 0;
  } else {
    if ((task_32D == -1) | (task_32D == -10) | (task_32D == -15)) {
      Script_ryoControl(PCCTL_DisablePlayerControl);
      Script_ryoControl(PCCTL_UpdateRyoNavYaw, 0x52494B41);
      Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
      Script_ccowUtils(CHARA_AKIR, 2);
      Script_charaSetRefboardFlagTo(CHARA_AKIR, 1);
      local_E4.x = 0.8f;
      (&local_E4)->y = 0.8f;
      (&local_E4)->z = 0.8f;
      (&local_E4)->w = 0.8f;
      Script_SetCharaRefBoardPos(CHARA_AKIR, &local_E4);
      script_2135E();
      script_15152(task_32D);
      script_2145D();
      Script_ccowUtils(CHARA_AKIR, 1);
      Script_motmSetBlendFrames(CHARA_AKIR, MOTM_BLEND_FRAMES_5);
      Script_charaSetRefboardFlagTo(CHARA_AKIR, 2);
      Script_ryoControl(RYO_RestoreControl);
      Script_convSetCamInactiveFlag(1);
      Script_SetGameStateTo(AUTH_None);
      task_32D = 0;
      task_32F = 0;
      goto L01579D;
    }
  }
  L01579D:;
  return;
}

__attribute__((scn_string_bindings(1, data_2C35C, 7, data_2C394, 8, data_2C39A, 13, data_2C3C8, 14, data_2C3CE, 15, data_2C3D4, 16, data_2C3DA, 18, data_2C3EA, 21, data_2C408, 23, data_2C418, 25, data_2C42C, 28, data_2C446)))
static void script_1579E(void) {
  int32_t unused_FC;
  GameDateTime local_F4;
  int32_t local_F0;
  int32_t local_EC;
  int8_t local_EB;
  int8_t local_EA;
  int8_t local_E9;
  int8_t local_E8;
  int8_t local_E7;
  int8_t local_E6;
  int8_t local_E5;
  int8_t local_E4;
  Vector4 local_D4;
  local_D4.x = 0.8f;
  (&local_D4)->y = 0.8f;
  (&local_D4)->z = 0.8f;
  (&local_D4)->w = 0.8f;
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 1);
  Script_SetCharaRefBoardPos(CHARA_AKIR, &local_D4);
  local_EC = Script_StepUtils(GET_STEP_HIGHER, 0) * 10 + Script_StepUtils(GET_STEP_LOWER, 0);
  Script_TELMUtils(TELM_CMD_LoadHandMotionAndSetup, task_304);
  local_E4 = 0;
  task_397 = 0;
  for (;;) {
    task_394 = Script_TELMUtils(TELM_CMD_GetActiveResult, task_304);
    task_394 = script_1502C(task_394);
    Script_ReturnTimeDate(&local_F4);
    if (MMTK_ScriptMemoToggle(2, 0) & Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_EV_STOP_ACTIVE)) {
      Script_sprintf("メモ帳消去\012");
      MMTK_ScriptMemoToggle(1, 0);
    }
    if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_EV_STOP_ACTIVE) & -(task_394 != -4) & -(task_394 != 0) & -(local_E4 == 0)) {
      local_E4 = 1;
      Script_TELMUtils(TELM_CMD_RequestHangUp, task_304);
      task_394 = Script_TELMUtils(TELM_CMD_GetActiveResult, task_304);
    }
    if ((task_394 == *task_338) | (task_394 == task_338[1])) {
      Script_TELMUtils(TELM_CMD_RequestHangUp, task_304);
      SCNF_WaitFrames_0000(1);
      Script_LoadVoiceFile(1, "E1032");
      script_163C1();
      if (task_394 == *task_338) {
        int32_t local_D0;
        local_D0 = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f));
        if (local_D0 == 0) {
          task_324 = Script_ShowUIMessageFromID("E1032A001");
          while (Script_convGetIsActive((const void *)task_324)) {
            SCNF_WaitFrames_0000(1);
          }
          task_324 = 0;
        } else if (local_D0 == 1) {
          task_324 = Script_ShowUIMessageFromID("E1032A002");
          while (Script_convGetIsActive((const void *)task_324)) {
            SCNF_WaitFrames_0000(1);
          }
          task_324 = 0;
        } else {
          task_324 = Script_ShowUIMessageFromID("E1032A003");
          while (Script_convGetIsActive((const void *)task_324)) {
            SCNF_WaitFrames_0000(1);
          }
          task_324 = 0;
        }
      } else {
        int32_t local_D0;
        local_D0 = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f));
        if (local_D0 == 0) {
          task_324 = Script_ShowUIMessageFromID("E1032A004");
          while (Script_convGetIsActive((const void *)task_324)) {
            SCNF_WaitFrames_0000(1);
          }
          task_324 = 0;
        } else {
          task_324 = Script_ShowUIMessageFromID("E1032A005");
          while (Script_convGetIsActive((const void *)task_324)) {
            SCNF_WaitFrames_0000(1);
          }
          task_324 = 0;
        }
      }
      Script_LoadVoiceFile(0, "E1032");
      script_151E6();
    }
    if ((task_394 == task_338[7]) | (task_394 == task_338[2]) | (task_394 == task_338[3]) | (task_394 == task_338[8]) | (task_394 == task_338[4]) | (task_394 == task_338[9]) | (task_394 == task_338[5]) | (task_394 == task_338[10]) | (task_394 == task_338[6]) | (task_394 == task_338[11])) {
      local_EB = 2;
      local_EA = 1;
      if (task_394 == task_338[7]) {
        if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_PHONE_I_BRANCH_UNLOCKED) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_PHONE_I_BRANCH_DONE) == 0) & Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_OPERATOR_ADDRESS_LOOKUP_REFUSED)) {
          int32_t local_D0;
          Script_TELMUtils(TELM_CMD_RequestHangUp, task_304);
          SCNF_WaitFrames_0000(1);
          Script_LoadVoiceFile(1, "E1032");
          local_D0 = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 4.0f));
          if (local_D0 == 0) {
            task_324 = Script_ShowUIMessageFromID("E1032A061");
            while (Script_convGetIsActive((const void *)task_324)) {
              SCNF_WaitFrames_0000(1);
            }
            task_324 = 0;
          } else if (local_D0 == 1) {
            task_324 = Script_ShowUIMessageFromID("E1032A062");
            while (Script_convGetIsActive((const void *)task_324)) {
              SCNF_WaitFrames_0000(1);
            }
            task_324 = 0;
          } else if (local_D0 == 2) {
            task_324 = Script_ShowUIMessageFromID("E1032A063");
            while (Script_convGetIsActive((const void *)task_324)) {
              SCNF_WaitFrames_0000(1);
            }
            task_324 = 0;
          } else {
            task_324 = Script_ShowUIMessageFromID("E1032A064");
            while (Script_convGetIsActive((const void *)task_324)) {
              SCNF_WaitFrames_0000(1);
            }
            task_324 = 0;
          }
          Script_LoadVoiceFile(0, "E1032");
          script_151E6();
          local_EA = 0;
        }
      }
      if (task_394 == task_338[2]) {
        local_EB = 1;
      }
      if ((task_394 == task_338[4]) | (task_394 == task_338[9])) {
        local_E9 = script_1520D(local_E9);
        if (local_E9 == -1) {
          task_394 = -10;
          local_EA = 0;
        }
      }
      if ((task_394 == task_338[3]) | (task_394 == task_338[8])) {
        local_F0 = Script_StepUtils(GET_STEP_HIGHER, 0);
        if ((Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_TOKI_TEGS_CUTSCENE_COMPLETE) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_PHONE_I_BRANCH_UNLOCKED) == 0)) | (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_MS08_SCENE_EXIT_COMPLETE) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_MS8A_POST_MS08_FOLLOWUP_COMPLETE) == 0)) | (local_F0 >= 16)) {
          local_E6 = script_1520D(local_E6);
          if (local_E6 == -1) {
            task_394 = -10;
            local_EA = 0;
          }
        } else {
          task_394 = -10;
          local_EA = 0;
        }
        if ((local_EC >= 131) & (local_EC <= 154)) {
          task_394 = -10;
          local_EA = 0;
        }
      }
      if ((task_394 == task_338[5]) | (task_394 == task_338[10])) {
        local_E8 = script_18C6D();
        switch (local_E8) {
          case -1:
            task_394 = -10;
            local_EA = 0;
            break;
          case -2:
            task_394 = -11;
            local_EA = 0;
            break;
        }
      }
      if ((task_394 == task_338[6]) | (task_394 == task_338[11])) {
        local_E7 = script_19F94();
        local_E7 = script_1520D(local_E7);
        if (local_E7 == -1) {
          task_394 = -10;
          local_EA = 0;
        }
      }
      if (local_EA) {
        if (script_1524F(local_EB)) {
          Script_FaceEx(CHARA_AKIR, 1, 0, 2);
          Script_shenClearFreeState(0);
          Script_LoadVoiceFile(1, "E1032");
          script_163C1();
          script_214D0();
          local_E5 = 0;
          if (task_394 == task_338[7]) {
            script_163E6();
          }
          if (task_394 == task_338[2]) {
            script_16B2D();
          }
          if ((task_394 == task_338[5]) | (task_394 == task_338[10])) {
            script_18E23(local_E8);
          }
          if ((task_394 == task_338[6]) | (task_394 == task_338[11])) {
            script_1A529(local_E7);
          }
          if ((task_394 == task_338[3]) | (task_394 == task_338[8])) {
            local_E5 = script_1EEAF();
          }
          if ((task_394 == task_338[4]) | (task_394 == task_338[9])) {
            script_1E3E7();
          }
          script_21795();
          if (local_E5) {
            script_1F436();
            Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_PHONE_I_BRANCH_UNLOCKED, 1);
          }
          Script_LoadVoiceFile(0, "E1032");
          script_216B9();
          Script_FaceEx(CHARA_AKIR, 0, 0, 2);
          Script_shenClearFreeState(1);
          task_394 = 0;
        } else {
          Script_TELMUtils(TELM_CMD_RequestHangUp, task_304);
          script_151E6();
        }
      }
    }
    if (task_394 == -10) {
      SCNF_WaitFrames_0000(1);
      Script_LoadVoiceFile(1, "E1032");
      script_163C1();
      if (script_1524F(6)) {
        int32_t local_D0;
        local_D0 = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 5.0f));
        if (local_D0 == 0) {
        } else if (local_D0 == 1) {
          task_324 = Script_ShowUIMessageFromID("E1032A031");
          while (Script_convGetIsActive((const void *)task_324)) {
            SCNF_WaitFrames_0000(1);
          }
          task_324 = 0;
        } else if (local_D0 == 2) {
          task_324 = Script_ShowUIMessageFromID("E1032A029");
          while (Script_convGetIsActive((const void *)task_324)) {
            SCNF_WaitFrames_0000(1);
          }
          task_324 = 0;
        } else if (local_D0 == 3) {
          task_324 = Script_ShowUIMessageFromID("E1032A032");
          while (Script_convGetIsActive((const void *)task_324)) {
            SCNF_WaitFrames_0000(1);
          }
          task_324 = 0;
        } else {
          task_324 = Script_ShowUIMessageFromID("E1032A033");
          while (Script_convGetIsActive((const void *)task_324)) {
            SCNF_WaitFrames_0000(1);
          }
          task_324 = 0;
        }
      }
      Script_LoadVoiceFile(0, "E1032");
      Script_TELMUtils(TELM_CMD_RequestHangUp, task_304);
      script_151E6();
    }
    if (task_394 == -11) {
      int32_t local_D0;
      Script_scenCreateTask(script_14DE1, 0);
      script_163C1();
      for (;;) {
        Script_executeAicaCommand(394667, 0, 0);
        SCNF_WaitFrames_0000(15);
        Script_executeAicaCommand(460203, 0, 0);
        SCNF_WaitFrames_0000(15);
        if (task_32C) {
          break;
        }
        SCNF_WaitFrames_0000(1);
      }
      local_D0 = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 4.0f));
      if (local_D0 == 0) {
      } else if (local_D0 == 1) {
        task_324 = Script_ShowUIMessageFromID("E1032A028");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
      } else if (local_D0 == 2) {
        task_324 = Script_ShowUIMessageFromID("E1032A029");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
      } else {
        task_324 = Script_ShowUIMessageFromID("E1032A030");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
      }
      task_394 = Script_TELMUtils(TELM_CMD_GetActiveResult, task_304);
      script_151E6();
    }
    if ((task_394 == task_338[12]) | (task_394 == task_338[13]) | (task_394 == task_338[14]) | (task_394 == task_338[15]) | (task_394 == task_338[16]) | (task_394 == task_338[17]) | (task_394 == task_338[18]) | (task_394 == task_338[19]) | (task_394 == task_338[20]) | (task_394 == task_338[21])) {
      SCNF_WaitFrames_0000(1);
      Script_LoadVoiceFile(1, "E1032");
      script_163C1();
      Script_scenCreateTask(script_14DE1, 0);
      for (;;) {
        task_324 = Script_ShowUIMessageFromID("E1032G001");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
        if (task_32C) {
          break;
        }
        task_324 = Script_ShowUIMessageFromID("E1032G002");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
        if (task_32C) {
          break;
        }
        SCNF_WaitFrames_0000(1);
      }
      Script_LoadVoiceFile(0, "E1032");
      task_394 = Script_TELMUtils(TELM_CMD_GetActiveResult, task_304);
      script_151E6();
    }
    if (task_394 == 0) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_seqEnterEvtCam(6219, task_304, task_304);
  script_16303();
  Script_sicnUtils(0, 5, 1);
  Script_uiUtils(0, 9, 0);
  SCNF_WaitFrames_0000(1);
  Script_ryoControl(PCCTL_DisablePlayerControl);
  return;
}

static void script_16303(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  Vector3i local_DC;
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_EVT_GetPosition(task_304, -1, &local_F4, 0);
  Script_mtwkGetSetRotation(task_304, &local_E8, 0);
  local_F4.x = (&local_F4)->x + Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_E8)->y) * 0.75f;
  (&local_F4)->y = -0.3f;
  (&local_F4)->z = (&local_F4)->z + Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_E8)->y) * 0.75f;
  Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &local_F4);
  (&local_DC)->y = (&local_E8)->y;
  Script_EVT_SetRotation(CHARA_AKIR, MATRIX_XFORM_Y, &local_DC, 0);
  Script_InitializeOsag(CHARA_AKIR);
  return;
}

static void script_163C1(void) {
  int32_t local_FC;
  local_FC = Script_PlayerStatsUtils(PS_GetStrayCatSceneVisitCountSlot, 2);
  local_FC = local_FC + 1;
  Script_PlayerStatsUtils(PS_SetStrayCatSceneVisitCountSlot, 2, local_FC);
  return;
}

__attribute__((scn_string_bindings(0, data_2C44C, 5, data_2C47E, 6, data_2C488, 7, data_2C492, 8, data_2C49C, 14, data_2C4D8, 16, data_2C4EC, 17, data_2C4F6, 18, data_2C500, 19, data_2C50A, 21, data_2C51E, 22, data_2C528, 23, data_2C532, 24, data_2C53C, 25, data_2C546)))
static void script_163E6(void) {
  int32_t local_FC;
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_PHONE_I_BRANCH_UNLOCKED) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_PHONE_I_BRANCH_DONE) == 0)) {
    int32_t local_F8;
    task_320 = 81;
    Script_scenCreateTask(script_209DC, 0);
    task_324 = Script_ShowUIMessageFromID("E1032I001");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032A052");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032I007");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032I008");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    local_FC = SCNF_RequestActionSelectorFromParams_0008(27, 28, 29, 30, 600, 0xFFFF, ACTION_SELECTOR_MODE_STANDARD);
    task_320 = 82;
    local_F8 = local_FC;
    switch (local_F8) {
      case 27:
        task_324 = Script_ShowUIMessageFromID("E1032A097");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
        task_324 = Script_ShowUIMessageFromID("E1032I009");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
        task_324 = Script_ShowUIMessageFromID("E1032I010");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
        task_324 = Script_ShowUIMessageFromID("E1032A054");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
        task_324 = Script_ShowUIMessageFromID("E1032I011");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
        break;
      case 28:
        task_324 = Script_ShowUIMessageFromID("E1032A055");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
        task_324 = Script_ShowUIMessageFromID("E1032I012");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
        task_324 = Script_ShowUIMessageFromID("E1032A056");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
        task_324 = Script_ShowUIMessageFromID("E1032I013");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
        task_324 = Script_ShowUIMessageFromID("E1032A057");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
        task_324 = Script_ShowUIMessageFromID("E1032I011");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
        Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_PHONE_I_BRANCH_DONE, 1);
        break;
      case 29:
        task_324 = Script_ShowUIMessageFromID("E1032A059");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
        task_324 = Script_ShowUIMessageFromID("E1032I009");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
        task_324 = Script_ShowUIMessageFromID("E1032I010");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
        task_324 = Script_ShowUIMessageFromID("E1032A054");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
        task_324 = Script_ShowUIMessageFromID("E1032I011");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
        break;
      case 30:
        task_324 = Script_ShowUIMessageFromID("E1032A060");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
        task_324 = Script_ShowUIMessageFromID("E1032I009");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
        task_324 = Script_ShowUIMessageFromID("E1032I010");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
        task_324 = Script_ShowUIMessageFromID("E1032A054");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
        task_324 = Script_ShowUIMessageFromID("E1032I011");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
        break;
    }
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_OPERATOR_ADDRESS_LOOKUP_REFUSED, 1);
  } else {
    task_320 = 80;
    Script_scenCreateTask(script_209DC, 0);
    task_324 = Script_ShowUIMessageFromID("E1032I001");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    {
      int32_t local_F8;
      local_F8 = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f));
      if (local_F8 == 0) {
        task_324 = Script_ShowUIMessageFromID("E1032A007");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
      } else {
        task_324 = Script_ShowUIMessageFromID("E1032A006");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
      }
    }
    {
      int32_t local_F8;
      local_F8 = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 5.0f));
      if (local_F8 == 0) {
        task_324 = Script_ShowUIMessageFromID("E1032I002");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
      } else if (local_F8 == 1) {
        task_324 = Script_ShowUIMessageFromID("E1032I003");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
      } else if (local_F8 == 2) {
        task_324 = Script_ShowUIMessageFromID("E1032I004");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
      } else if (local_F8 == 3) {
        task_324 = Script_ShowUIMessageFromID("E1032I005");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
      } else {
        task_324 = Script_ShowUIMessageFromID("E1032I006");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
      }
    }
    {
      int32_t local_F8;
      local_F8 = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f));
      if (local_F8 == 0) {
        task_324 = Script_ShowUIMessageFromID("E1032A008");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
      } else {
        task_324 = Script_ShowUIMessageFromID("E1032A009");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
      }
    }
  }
  return;
}

__attribute__((scn_string_bindings(5, data_2C5DC, 6, data_2C5E6, 7, data_2C5F0, 8, data_2C5FA, 9, data_2C604, 10, data_2C60E, 11, data_2C618, 12, data_2C622, 13, data_2C62C, 14, data_2C636, 15, data_2C640, 16, data_2C64A, 17, data_2C654, 18, data_2C65E, 19, data_2C668, 20, data_2C672, 21, data_2C67C, 22, data_2C686, 23, data_2C690, 24, data_2C69A, 25, data_2C6A4, 26, data_2C6AE, 27, data_2C6B8, 28, data_2C6C2, 29, data_2C6CC, 30, data_2C6D6, 31, data_2C6E0, 32, data_2C6EA, 33, data_2C6F4, 34, data_2C6FE, 35, data_2C708, 36, data_2C712, 37, data_2C71C, 38, data_2C726, 39, data_2C730, 40, data_2C73A, 41, data_2C744, 42, data_2C74E, 43, data_2C758, 44, data_2C762, 45, data_2C76C, 46, data_2C776, 47, data_2C780, 48, data_2C78A, 49, data_2C794, 50, data_2C79E, 51, data_2C7A8, 52, data_2C7B2, 53, data_2C7BC, 54, data_2C7C6, 55, data_2C7D0, 56, data_2C7DA, 57, data_2C7E4, 58, data_2C7EE, 59, data_2C7F8, 60, data_2C802, 61, data_2C80C, 62, data_2C816, 63, data_2C820, 64, data_2C82A, 65, data_2C834, 66, data_2C83E, 67, data_2C848, 68, data_2C852, 69, data_2C85C, 70, data_2C866, 71, data_2C870, 72, data_2C87A, 73, data_2C884, 74, data_2C88E, 75, data_2C898, 76, data_2C8A2, 77, data_2C8AC, 78, data_2C8B6, 79, data_2C8C0, 80, data_2C8CA, 81, data_2C8D4, 82, data_2C8DE, 83, data_2C8E8, 84, data_2C8F2, 85, data_2C8FC, 86, data_2C906, 87, data_2C910, 88, data_2C91A, 89, data_2C924, 90, data_2C92E, 91, data_2C938, 92, data_2C942, 93, data_2C94C, 94, data_2C956, 95, data_2C960, 96, data_2C96A, 97, data_2C974, 98, data_2C97E, 99, data_2C988, 100, data_2C992, 101, data_2C99C, 102, data_2C9A6, 103, data_2C9B0, 104, data_2C9BA, 105, data_2C9C4, 106, data_2C9CE, 107, data_2C9D8, 108, data_2C9E2, 109, data_2C9EC, 110, data_2C9F6, 111, data_2CA00, 112, data_2CA0A, 113, data_2CA14, 114, data_2CA1E, 115, data_2CA28, 116, data_2CA32, 117, data_2CA3C, 118, data_2CA46, 119, data_2CA50, 120, data_2CA5A, 121, data_2CA64, 122, data_2CA6E, 123, data_2CA78, 124, data_2CA82, 125, data_2CA8C, 126, data_2CA96, 127, data_2CAA0, 128, data_2CAAA, 129, data_2CAB4, 130, data_2CABE, 131, data_2CAC8, 132, data_2CAD2, 133, data_2CADC, 134, data_2CAE6, 135, data_2CAF0, 136, data_2CAFA, 137, data_2CB04, 138, data_2CB0E, 139, data_2CB18, 140, data_2CB22, 141, data_2CB2C, 142, data_2CB36, 143, data_2CB40, 144, data_2CB4A, 145, data_2CB54, 146, data_2CB5E, 147, data_2CB68, 148, data_2CB72, 149, data_2CB7C, 150, data_2CB86, 151, data_2CB90, 152, data_2CB9A, 153, data_2CBA4, 154, data_2CBAE, 155, data_2CBB8, 156, data_2CBC2, 157, data_2CBCC, 158, data_2CBD6, 159, data_2CBE0, 160, data_2CBEA, 161, data_2CBF4, 162, data_2CBFE)))
static void script_16B2D(void) {
  int32_t unused_FC;
  GameDateTime local_F4;
  int32_t local_F0;
  int32_t local_EC;
  task_320 = 70;
  Script_scenCreateTask(script_209DC, 0);
  task_324 = Script_ShowUIMessageFromID("E1032K001");
  while (Script_convGetIsActive((const void *)task_324)) {
    SCNF_WaitFrames_0000(1);
  }
  task_324 = 0;
  task_324 = Script_ShowUIMessageFromID("E1032K002");
  while (Script_convGetIsActive((const void *)task_324)) {
    SCNF_WaitFrames_0000(1);
  }
  task_324 = 0;
  task_324 = Script_ShowUIMessageFromID("E1032K019");
  while (Script_convGetIsActive((const void *)task_324)) {
    SCNF_WaitFrames_0000(1);
  }
  task_324 = 0;
  task_324 = Script_ShowUIMessageFromID("E1032K020");
  while (Script_convGetIsActive((const void *)task_324)) {
    SCNF_WaitFrames_0000(1);
  }
  task_324 = 0;
  task_324 = Script_ShowUIMessageFromID("E1032K022");
  while (Script_convGetIsActive((const void *)task_324)) {
    SCNF_WaitFrames_0000(1);
  }
  task_324 = 0;
  Script_ReturnTimeDate(&local_F4);
  if (((&local_F4)->month == 1) | ((&local_F4)->month == 3) | ((&local_F4)->month == 5) | ((&local_F4)->month == 7) | ((&local_F4)->month == 8) | ((&local_F4)->month == 10) | ((&local_F4)->month == 12)) {
    if ((&local_F4)->day >= 31) {
      if ((&local_F4)->month == 12) {
        local_F4.year = (&local_F4)->year + 1;
        (&local_F4)->month = 1;
      } else {
        (&local_F4)->month = (&local_F4)->month + 1;
      }
      (&local_F4)->day = 1;
    } else {
      (&local_F4)->day = (&local_F4)->day + 1;
    }
  } else if ((&local_F4)->month == 2) {
    if ((&local_F4)->day >= 28) {
      (&local_F4)->month = 3;
      (&local_F4)->day = 1;
    } else {
      (&local_F4)->day = (&local_F4)->day + 1;
    }
  } else if ((&local_F4)->day >= 30) {
    (&local_F4)->month = (&local_F4)->month + 1;
    (&local_F4)->day = 1;
  } else {
    (&local_F4)->day = (&local_F4)->day + 1;
  }
  local_F0 = Script_WeatherUtils_0(WeatherDataUtil_LookupFCASDateCode, (&local_F4)->year, (&local_F4)->month, (&local_F4)->day);
  local_EC = local_F0;
  if (local_EC == 1) {
    task_324 = Script_ShowUIMessageFromID("E1032K015");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  } else if (local_EC == 2) {
    task_324 = Script_ShowUIMessageFromID("E1032K016");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  } else if (local_EC == 3) {
    task_324 = Script_ShowUIMessageFromID("E1032K017");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  } else if (local_EC == 4) {
    task_324 = Script_ShowUIMessageFromID("E1032K018");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  } else if (local_EC == 5) {
    task_324 = Script_ShowUIMessageFromID("E1032K003");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K016");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  } else if (local_EC == 6) {
    task_324 = Script_ShowUIMessageFromID("E1032K003");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K017");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  } else if (local_EC == 7) {
    task_324 = Script_ShowUIMessageFromID("E1032K003");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K018");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  } else if (local_EC == 8) {
    task_324 = Script_ShowUIMessageFromID("E1032K004");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K016");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  } else if (local_EC == 9) {
    task_324 = Script_ShowUIMessageFromID("E1032K004");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K017");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  } else if (local_EC == 10) {
    task_324 = Script_ShowUIMessageFromID("E1032K004");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K018");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  } else if (local_EC == 11) {
    task_324 = Script_ShowUIMessageFromID("E1032K005");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K016");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  } else if (local_EC == 12) {
    task_324 = Script_ShowUIMessageFromID("E1032K005");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K017");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  } else if (local_EC == 13) {
    task_324 = Script_ShowUIMessageFromID("E1032K005");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K018");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  } else if (local_EC == 14) {
    task_324 = Script_ShowUIMessageFromID("E1032K006");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K015");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  } else if (local_EC == 15) {
    task_324 = Script_ShowUIMessageFromID("E1032K006");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K017");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  } else if (local_EC == 16) {
    task_324 = Script_ShowUIMessageFromID("E1032K006");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K018");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  } else if (local_EC == 17) {
    task_324 = Script_ShowUIMessageFromID("E1032K007");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K015");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  } else if (local_EC == 18) {
    task_324 = Script_ShowUIMessageFromID("E1032K007");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K017");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  } else if (local_EC == 19) {
    task_324 = Script_ShowUIMessageFromID("E1032K007");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K018");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  } else if (local_EC == 20) {
    task_324 = Script_ShowUIMessageFromID("E1032K008");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K015");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  } else if (local_EC == 21) {
    task_324 = Script_ShowUIMessageFromID("E1032K008");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K017");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  } else if (local_EC == 22) {
    task_324 = Script_ShowUIMessageFromID("E1032K008");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K018");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  } else if (local_EC == 23) {
    task_324 = Script_ShowUIMessageFromID("E1032K009");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K015");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  } else if (local_EC == 24) {
    task_324 = Script_ShowUIMessageFromID("E1032K009");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K016");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  } else if (local_EC == 25) {
    task_324 = Script_ShowUIMessageFromID("E1032K009");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K018");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  } else if (local_EC == 26) {
    task_324 = Script_ShowUIMessageFromID("E1032K010");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K015");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  } else if (local_EC == 27) {
    task_324 = Script_ShowUIMessageFromID("E1032K010");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K016");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  } else if (local_EC == 28) {
    task_324 = Script_ShowUIMessageFromID("E1032K010");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K018");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  } else if (local_EC == 29) {
    task_324 = Script_ShowUIMessageFromID("E1032K011");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K015");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  } else if (local_EC == 30) {
    task_324 = Script_ShowUIMessageFromID("E1032K011");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K016");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  } else if (local_EC == 31) {
    task_324 = Script_ShowUIMessageFromID("E1032K011");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K018");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  } else if (local_EC == 32) {
    task_324 = Script_ShowUIMessageFromID("E1032K012");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K015");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  } else if (local_EC == 33) {
    task_324 = Script_ShowUIMessageFromID("E1032K012");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K016");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  } else if (local_EC == 34) {
    task_324 = Script_ShowUIMessageFromID("E1032K012");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K017");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  } else if (local_EC == 35) {
    task_324 = Script_ShowUIMessageFromID("E1032K013");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K015");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  } else if (local_EC == 36) {
    task_324 = Script_ShowUIMessageFromID("E1032K013");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K016");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  } else if (local_EC == 37) {
    task_324 = Script_ShowUIMessageFromID("E1032K013");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K017");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  } else if (local_EC == 38) {
    task_324 = Script_ShowUIMessageFromID("E1032K014");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K015");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  } else if (local_EC == 39) {
    task_324 = Script_ShowUIMessageFromID("E1032K014");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K016");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  } else if (local_EC == 40) {
    task_324 = Script_ShowUIMessageFromID("E1032K014");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K017");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  } else if (local_EC == 41) {
    task_324 = Script_ShowUIMessageFromID("E1032K004");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K007");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K015");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  } else if (local_EC == 42) {
    task_324 = Script_ShowUIMessageFromID("E1032K004");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K007");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K017");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  } else if (local_EC == 43) {
    task_324 = Script_ShowUIMessageFromID("E1032K004");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K006");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K017");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  } else if (local_EC == 44) {
    task_324 = Script_ShowUIMessageFromID("E1032K004");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K013");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K017");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  } else if (local_EC == 45) {
    task_324 = Script_ShowUIMessageFromID("E1032K005");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K007");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K018");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  } else if (local_EC == 46) {
    task_324 = Script_ShowUIMessageFromID("E1032K007");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K004");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K017");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  } else if (local_EC == 47) {
    task_324 = Script_ShowUIMessageFromID("E1032K008");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K010");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K015");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  } else if (local_EC == 48) {
    task_324 = Script_ShowUIMessageFromID("E1032K008");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K013");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K015");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  } else if (local_EC == 54) {
    task_324 = Script_ShowUIMessageFromID("E1032K010");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K005");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K016");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  } else if (local_EC == 50) {
    task_324 = Script_ShowUIMessageFromID("E1032K010");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K004");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K018");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  } else if (local_EC == 51) {
    task_324 = Script_ShowUIMessageFromID("E1032K010");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K007");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K018");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  } else if (local_EC == 52) {
    task_324 = Script_ShowUIMessageFromID("E1032K011");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K007");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K015");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  } else if (local_EC == 53) {
    task_324 = Script_ShowUIMessageFromID("E1032K013");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K008");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K015");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  } else if (local_EC == 54) {
    task_324 = Script_ShowUIMessageFromID("E1032K010");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K005");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K016");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  } else if (local_EC == 55) {
    task_324 = Script_ShowUIMessageFromID("E1032K004");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K010");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K016");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  } else if (local_EC == 56) {
    task_324 = Script_ShowUIMessageFromID("E1032K004");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K008");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K018");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  } else if (local_EC == 57) {
    task_324 = Script_ShowUIMessageFromID("E1032K003");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K007");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K018");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  } else if (local_EC == 58) {
    task_324 = Script_ShowUIMessageFromID("E1032K007");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K010");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K015");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  } else if (local_EC == 59) {
    task_324 = Script_ShowUIMessageFromID("E1032K008");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K004");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K018");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  } else if (local_EC == 60) {
    task_324 = Script_ShowUIMessageFromID("E1032K007");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K010");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K018");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  } else if (local_EC == 61) {
    task_324 = Script_ShowUIMessageFromID("E1032K004");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K010");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K015");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  } else if (local_EC == 62) {
    task_324 = Script_ShowUIMessageFromID("E1032K004");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K007");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K017");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  } else if (local_EC == 63) {
    task_324 = Script_ShowUIMessageFromID("E1032K004");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K007");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K018");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  } else if (local_EC == 64) {
    task_324 = Script_ShowUIMessageFromID("E1032K010");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K008");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K015");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  } else if (local_EC == 65) {
    task_324 = Script_ShowUIMessageFromID("E1032K013");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K007");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K015");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  } else if (local_EC == 66) {
    task_324 = Script_ShowUIMessageFromID("E1032K004");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K013");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K016");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  } else if (local_EC == 67) {
    task_324 = Script_ShowUIMessageFromID("E1032K014");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K007");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032K015");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  } else {
    task_324 = Script_ShowUIMessageFromID("E1032K015");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  }
  return;
}

static int8_t script_18BB4(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
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
  return local_E7;
}

static int8_t script_18C6D(void) {
  int32_t unused_FC;
  GameDateTime local_F4;
  int32_t local_F0;
  int8_t local_EF;
  int8_t local_EE;
  int8_t local_ED;
  int8_t local_EC;
  int32_t local_E8;
  Script_ReturnTimeDate(&local_F4);
  local_F0 = Script_StepUtils(GET_STEP_HIGHER, 0);
  if (script_18BB4(8, 30, 15, 10)) {
    local_EE = 0;
  } else if (script_18BB4(15, 10, 20, 30)) {
    local_EE = 1;
  } else {
    local_EE = 2;
  }
  if ((&local_F4)->day & 1) {
    local_ED = 0;
  } else {
    local_ED = 1;
  }
  local_EF = 0;
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_NOZOMI_KIDNAPPED) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOZOMI_RESCUE_RUSH_ACTIVE) == 0)) {
    if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 950) == 0) {
      local_EF = 7;
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 950, 1);
    } else {
      local_EF = -2;
    }
  } else {
    if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_PROMISED_TERRY_TO_FIGHT_CHEN_GUI_ZHANG)) {
      if ((local_EE == 2) & (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NAOYUKI_MOTORCYCLE_THANKS_CALL) == 0)) {
        local_EF = 8;
        Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NAOYUKI_MOTORCYCLE_THANKS_CALL, 1);
      }
      goto L018D62;
    }
  }
  L018D62:;
  if (local_EF == 0) {
    local_EF = script_1520D(local_EF);
  }
  if (local_EF == 0) {
    local_E8 = local_EE;
    if (local_E8 == 0) {
      if (local_ED == 0) {
        local_EF = 5;
      } else {
        local_EF = 6;
      }
    } else if (local_E8 == 1) {
      if (local_ED == 0) {
        local_EF = -1;
      } else {
        local_EF = -2;
      }
    } else {
      local_EC = (&local_F4)->day & 3;
      local_E8 = local_EC;
      if (local_E8 == 1) {
        local_EF = 1;
      } else if (local_E8 == 2) {
        if (local_F0 >= 11) {
          local_EF = 2;
        } else {
          local_EF = 3;
        }
      } else {
        local_EF = 4;
      }
    }
  }
  return local_EF;
}

__attribute__((scn_string_bindings(0, data_2CC08, 8, data_2CC58, 9, data_2CC62, 14, data_2CC94, 17, data_2CCB2, 18, data_2CCBC, 19, data_2CCC6, 21, data_2CCDA, 25, data_2CD02, 27, data_2CD16, 33, data_2CD52, 40, data_2CD98, 41, data_2CDA2, 42, data_2CDAC, 43, data_2CDB6, 44, data_2CDC0, 48, data_2CDE8, 49, data_2CDF2, 52, data_2CE10, 53, data_2CE1A, 57, data_2CE42, 59, data_2CE56, 61, data_2CE6A, 62, data_2CE74, 70, data_2CEC4, 75, data_2CEF6, 76, data_2CF00, 77, data_2CF0A, 82, data_2CF3C, 83, data_2CF46, 84, data_2CF50, 85, data_2CF5A, 86, data_2CF64, 87, data_2CF6E, 88, data_2CF78, 92, data_2CFA0, 93, data_2CFAA)))
static void script_18E23(int32_t arg0) {
  int32_t local_FC;
  local_FC = arg0;
  switch (local_FC) {
    case 1:
      task_320 = 170;
      Script_scenCreateTask(script_209DC, 0);
      task_324 = Script_ShowUIMessageFromID("E1032L001");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A098");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032L002");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A099");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032L003");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032L004");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032L005");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A100");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      break;
    case 2:
      task_320 = 180;
      Script_scenCreateTask(script_209DC, 0);
      task_324 = Script_ShowUIMessageFromID("E1032L001");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A101");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032L006");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032L007");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032L008");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032L009");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A019");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A102");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032L010");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A019");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      break;
    case 3:
      task_320 = 190;
      Script_scenCreateTask(script_209DC, 0);
      task_324 = Script_ShowUIMessageFromID("E1032L011");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A101");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032L012");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A103");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032L013");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032L014");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032L015");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_320 = 191;
      task_324 = Script_ShowUIMessageFromID("E1032A103");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032L016");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A104");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A105");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032L017");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A106");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      break;
    case 4:
      task_320 = 200;
      Script_scenCreateTask(script_209DC, 0);
      task_324 = Script_ShowUIMessageFromID("E1032L018");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A107");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032L019");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A108");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032L020");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032L021");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A109");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032L022");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A110");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032L023");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      break;
    case 5:
      task_320 = 210;
      Script_scenCreateTask(script_209DC, 0);
      task_324 = Script_ShowUIMessageFromID("E1032M001");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A111");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032M002");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A112");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032M003");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A113");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032M004");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A104");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A114");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032M005");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      break;
    case 6:
      task_320 = 220;
      Script_scenCreateTask(script_209DC, 0);
      task_324 = Script_ShowUIMessageFromID("E1032M006");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A111");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032M002");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A115");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032M018");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A122");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032M019");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A123");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032M019");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032M020");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A104");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A114");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032M010");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      break;
    case 7:
      task_320 = 230;
      Script_scenCreateTask(script_209DC, 0);
      task_324 = Script_ShowUIMessageFromID("E1032N001");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A124");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A125");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032N002");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A126");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032N003");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_320 = 231;
      task_324 = Script_ShowUIMessageFromID("E1032A127");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A128");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032N004");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A129");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032N005");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      break;
    case 8:
      task_320 = 240;
      Script_scenCreateTask(script_209DC, 0);
      task_324 = Script_ShowUIMessageFromID("E1032L011");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A101");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032L019");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A117");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032L024");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032L025");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032L026");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A019");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A118");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032L023");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      break;
    case 9:
      task_324 = Script_ShowUIMessageFromID("E1032M001");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A111");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032M002");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A112");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032M015");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A119");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032M016");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A104");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A118");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032M017");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      break;
  }
  return;
}

static int8_t script_19F94(void) {
  int32_t unused_FC;
  GameDateTime local_F4;
  int32_t local_F0;
  int8_t local_EF;
  int8_t local_EE;
  uint8_t unused_EC[2];
  int32_t local_E8;
  Script_ReturnTimeDate(&local_F4);
  local_F0 = Script_StepUtils(GET_STEP_HIGHER, 0) * 10 + Script_StepUtils(GET_STEP_LOWER, 0);
  local_EF = 0;
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_FIND_TERRY) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_PROMISED_TERRY_TO_FIGHT_CHEN_GUI_ZHANG) == 0)) {
    local_EF = -1;
  } else if ((Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JOMO_DNOZ_EVENT_DONE) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_DKTY_TUBA_CUTSCENE_COMPLETE) == 0) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JOMO_POST_DNOZ_ROUTE_ACTIVE) == 0)) | (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JOMO_01BNK_BANK_EVENT_ACTIVE) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_NOZOMI_CRIED) == 0) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_FIND_TERRY) == 0))) {
    if (script_18BB4(8, 30, 21, 0)) {
      local_EF = 1;
    } else {
      local_EF = 2;
    }
  } else if ((local_F0 == 13) | (local_F0 == 174)) {
    if (script_18BB4(8, 30, 22, 0)) {
      local_EF = 11;
    } else {
      local_EF = 10;
    }
  } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_NOZOMI_CRIED) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NOZOMI_SORRY_ABOUT_OTHER_DAY_CALL) == 0)) {
    local_EF = 3;
  } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JD00_JUTADN_EVENT_ACTIVE) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_D000_YBHN_EVENT_DONE) == 0) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 946) == 0)) {
    local_EF = 6;
  } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_D000_YBHN_EVENT_DONE) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JU00_STRAY_CAT_STAGE5_COMPLETE) == 0) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JU00_HRSK_CAT_FOLLOWUP_COMPLETE) == 0) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JU00_MEGUMI_KEEPS_CAT) == 0) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JOMO_POST_DNOZ_ROUTE_ACTIVE) == 0) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JOMO_POST_0172_SCENE2_PROMPT_SHOWN) == 0) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 955) == 0) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 537) == 0)) {
    local_EF = 12;
  } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JOMO_POST_DNOZ_ROUTE_ACTIVE) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JOMO_POST_0172_SCENE2_PROMPT_SHOWN) == 0) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 955) == 0) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JU00_STRAY_CAT_STAGE5_COMPLETE) == 0) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JU00_HRSK_CAT_FOLLOWUP_COMPLETE) == 0) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JU00_MEGUMI_KEEPS_CAT) == 0) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NOZOMI_SPRUNG_ON_YOU_CALL) == 0)) {
    local_EF = 7;
  } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JU00_STRAY_CAT_STAGE5_COMPLETE) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JU00_HRSK_CAT_FOLLOWUP_COMPLETE) == 0) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JU00_MEGUMI_KEEPS_CAT) == 0) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JOMO_POST_0172_SCENE2_PROMPT_SHOWN) == 0) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 955) == 0) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NOZOMI_SHRINE_KITTEN_CALL) == 0) & -(local_F0 >= 162)) {
    local_EF = 13;
  } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JU00_HRSK_CAT_FOLLOWUP_COMPLETE) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JU00_MEGUMI_KEEPS_CAT) == 0) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JOMO_POST_0172_SCENE2_PROMPT_SHOWN) == 0) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 955) == 0) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NOZOMI_KITTEN_RECOVERED_CALL) == 0) & -(local_F0 >= 201)) {
    local_EF = 14;
  } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JOMO_POST_0172_SCENE2_PROMPT_SHOWN) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 955) == 0) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NOZOMI_FLOWERS_THANKS_CALL) == 0)) {
    local_EF = 4;
  } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 955) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_FIND_TERRY) == 0) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NOZOMI_STAY_AWAY_FROM_HARBOR_CALL) == 0)) {
    local_EF = 16;
  } else {
    if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_PROMISED_TERRY_TO_FIGHT_CHEN_GUI_ZHANG) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NOZOMI_AFTER_RESCUE_CALL) == 0)) {
      local_EF = 5;
      goto L01A30D;
    }
  }
  L01A30D:;
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_FIND_TERRY) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_PROMISED_TERRY_TO_FIGHT_CHEN_GUI_ZHANG) == 0)) {
    local_EF = -1;
  } else if (script_18BB4(8, 30, 21, 0)) {
    local_EF = 1;
  } else {
    local_EF = 2;
  }
  if (local_EF == 0) {
    if ((local_F0 <= 13) & (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NOZOMI_TALK_TO_INE_CALL) == 0)) {
      local_EF = 17;
    } else if (-(local_F0 >= 21) & -(local_F0 <= 102) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NOZOMI_PARENTS_IN_VANCOUVER_CALL) == 0)) {
      local_EF = 18;
    } else if (-(local_F0 >= 61) & -(local_F0 <= 102) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NOZOMI_CALL_NAOYUKI_AND_ICHIRO_CALL) == 0)) {
      local_EF = 19;
    } else if (-(local_F0 >= 111) & -(local_F0 <= 193) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NOZOMI_LAPIS_FORTUNETELLER_CALL) == 0)) {
      local_EF = 20;
    } else if (-(local_F0 >= 141) & -(local_F0 <= 193) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NOZOMI_FIRST_MEETING_CALL) == 0)) {
      local_EF = 22;
    } else if (-(local_F0 >= 171) & -(local_F0 <= 193) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NOZOMI_NURSERY_TEACHER_CALL) == 0)) {
      local_EF = 23;
    } else if (-(local_F0 >= 211) & -(local_F0 <= 218) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NOZOMI_FEELS_WATCHED_CALL) == 0)) {
      local_EF = 24;
    } else if (-(local_F0 >= 201) & -(local_F0 <= 241) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NOZOMI_TRIP_INVITATION_CALL) == 0)) {
      local_EF = 25;
    } else {
      if (-(local_F0 >= 215) & -(local_F0 <= 241) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NOZOMI_RELAY_RACE_PHOTO_CALL) == 0)) {
        local_EF = 21;
        goto L01A4B7;
      }
    }
  }
  L01A4B7:;
  if (local_EF == 0) {
    local_EE = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 4.0f));
    local_E8 = local_EE;
    switch (local_E8) {
      case 0:
        local_EF = 8;
        break;
      case 1:
        local_EF = 9;
        break;
      case 2:
        local_EF = 10;
        break;
      case 3:
        local_EF = 26;
        break;
    }
  }
  return local_EF;
}

__attribute__((scn_string_bindings(0, data_2CFBE, 1, data_2CFC8, 2, data_2CFD2, 4, data_2CFE6, 5, data_2CFF0, 6, data_2CFFA, 7, data_2D004, 8, data_2D00E, 9, data_2D018, 10, data_2D022, 11, data_2D02C, 12, data_2D036, 13, data_2D040, 15, data_2D054, 16, data_2D05E, 17, data_2D068, 18, data_2D072, 19, data_2D07C, 20, data_2D086, 21, data_2D090, 22, data_2D09A, 23, data_2D0A4, 25, data_2D0B8, 27, data_2D0CC, 28, data_2D0D6, 31, data_2D0F4, 32, data_2D0FE, 33, data_2D108, 38, data_2D13A, 39, data_2D144, 40, data_2D14E, 41, data_2D158, 43, data_2D16C, 44, data_2D176, 45, data_2D180, 56, data_2D1EE, 57, data_2D1F8, 58, data_2D202, 59, data_2D20C, 60, data_2D216, 61, data_2D220, 64, data_2D23E, 65, data_2D248, 66, data_2D252, 67, data_2D25C, 68, data_2D266, 69, data_2D270, 70, data_2D27A, 71, data_2D284, 72, data_2D28E, 74, data_2D2A2, 76, data_2D2B6, 78, data_2D2CA, 79, data_2D2D4, 80, data_2D2DE, 81, data_2D2E8, 82, data_2D2F2, 84, data_2D306, 86, data_2D31A, 87, data_2D324, 88, data_2D32E, 89, data_2D338, 90, data_2D342, 91, data_2D34C, 92, data_2D356, 93, data_2D360, 94, data_2D36A, 96, data_2D37E, 99, data_2D39C, 100, data_2D3A6, 101, data_2D3B0, 102, data_2D3BA, 103, data_2D3C4, 104, data_2D3CE, 106, data_2D3E2, 107, data_2D3EC, 111, data_2D414, 113, data_2D428, 115, data_2D43C, 116, data_2D446, 117, data_2D450, 118, data_2D45A, 119, data_2D464, 120, data_2D46E, 125, data_2D4A0, 127, data_2D4B4, 128, data_2D4BE, 129, data_2D4C8, 132, data_2D4E6, 133, data_2D4F0, 134, data_2D4FA, 135, data_2D504, 136, data_2D50E, 137, data_2D518, 138, data_2D522, 139, data_2D52C, 140, data_2D536, 141, data_2D540, 142, data_2D54A, 143, data_2D554, 144, data_2D55E, 150, data_2D59A, 158, data_2D5EA, 159, data_2D5F4, 160, data_2D5FE, 161, data_2D608, 163, data_2D61C, 172, data_2D676, 173, data_2D680, 174, data_2D68A, 175, data_2D694, 182, data_2D6DA, 185, data_2D6F8, 186, data_2D702, 187, data_2D70C, 198, data_2D77A, 200, data_2D78E, 201, data_2D798, 208, data_2D7DE, 209, data_2D7E8, 216, data_2D82E, 224, data_2D87E, 225, data_2D888, 226, data_2D892, 232, data_2D8CE, 234, data_2D8E2, 235, data_2D8EC, 237, data_2D900, 239, data_2D914, 244, data_2D946, 245, data_2D950, 256, data_2D9BE, 258, data_2D9D2, 260, data_2D9E6, 261, data_2D9F0, 265, data_2DA18, 269, data_2DA40, 271, data_2DA54, 273, data_2DA68, 274, data_2DA72, 275, data_2DA7C, 276, data_2DA86, 277, data_2DA90, 278, data_2DA9A, 280, data_2DAAE, 282, data_2DAC2, 285, data_2DAE0, 288, data_2DAFE, 289, data_2DB08, 290, data_2DB12, 295, data_2DB44, 297, data_2DB58, 299, data_2DB6C, 300, data_2DB76, 301, data_2DB80, 302, data_2DB8A, 303, data_2DB94, 304, data_2DB9E, 308, data_2DBC6, 311, data_2DBE4, 313, data_2DBF8, 315, data_2DC0C, 316, data_2DC16, 317, data_2DC20, 318, data_2DC2A, 331, data_2DCAC)))
static void script_1A529(int32_t arg0) {
  int8_t local_FF;
  int32_t local_F8;
  int32_t local_F4;
  local_F8 = Script_StepUtils(GET_STEP_HIGHER, 0) * 10 + Script_StepUtils(GET_STEP_LOWER, 0);
  local_F4 = arg0;
  switch (local_F4) {
    case 1:
      task_320 = 90;
      Script_scenCreateTask(script_209DC, 0);
      task_324 = Script_ShowUIMessageFromID("E1032O001");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A136");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032O002");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A137");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032O007");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A127");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032O004");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A138");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032O005");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A139");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032O006");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      break;
    case 2:
      task_320 = 90;
      Script_scenCreateTask(script_209DC, 0);
      task_324 = Script_ShowUIMessageFromID("E1032O001");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A136");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032O002");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A140");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032O007");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A127");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032O004");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A138");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032O005");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A139");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032O006");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      break;
    case 3:
      task_320 = 90;
      Script_scenCreateTask(script_209DC, 0);
      task_324 = Script_ShowUIMessageFromID("E1032P001");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A085");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P002");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A019");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P003");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A141");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P004");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A142");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P005");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A019");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NOZOMI_SORRY_ABOUT_OTHER_DAY_CALL, 1);
      break;
    case 4:
      task_320 = 100;
      Script_scenCreateTask(script_209DC, 0);
      task_324 = Script_ShowUIMessageFromID("E1032P001");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A085");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P006");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A143");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P007");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A144");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P004");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032O008");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_320 = 101;
      task_324 = Script_ShowUIMessageFromID("E1032P008");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A068");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P009");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A019");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NOZOMI_FLOWERS_THANKS_CALL, 1);
      break;
    case 5:
      task_320 = 110;
      Script_scenCreateTask(script_209DC, 0);
      task_324 = Script_ShowUIMessageFromID("E1032A085");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P010");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A145");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P011");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A146");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P012");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A147");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P013");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A148");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P014");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      SCNF_WaitFrames_0000(60);
      task_320 = 111;
      task_324 = Script_ShowUIMessageFromID("E1032A149");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P015");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A019");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P016");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A068");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P017");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NOZOMI_AFTER_RESCUE_CALL, 1);
      break;
    case 6:
      task_320 = 120;
      Script_scenCreateTask(script_209DC, 0);
      task_324 = Script_ShowUIMessageFromID("E1032A085");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P018");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A150");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P019");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A151");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P004");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P016");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A068");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P017");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 946, 1);
      break;
    case 7:
      task_320 = 130;
      Script_scenCreateTask(script_209DC, 0);
      task_324 = Script_ShowUIMessageFromID("E1032P021");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A152");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P010");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      SCNF_WaitFrames_0000(60);
      task_320 = 131;
      task_324 = Script_ShowUIMessageFromID("E1032A153");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P022");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A154");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P023");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A019");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P024");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A068");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P017");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NOZOMI_SPRUNG_ON_YOU_CALL, 1);
      break;
    case 8:
      task_320 = 140;
      Script_scenCreateTask(script_209DC, 0);
      task_324 = Script_ShowUIMessageFromID("E1032P001");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A085");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P025");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A155");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P004");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A156");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032O008");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P008");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_320 = 141;
      task_324 = Script_ShowUIMessageFromID("E1032A154");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P027");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A068");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P017");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A019");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      break;
    case 9:
      task_320 = 150;
      Script_scenCreateTask(script_209DC, 0);
      task_324 = Script_ShowUIMessageFromID("E1032P021");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A152");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P028");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A158");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P030");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_320 = 151;
      task_324 = Script_ShowUIMessageFromID("E1032A159");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P066");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P016");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A068");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P017");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      break;
    case 10:
      task_320 = 160;
      Script_scenCreateTask(script_209DC, 0);
      task_324 = Script_ShowUIMessageFromID("E1032P021");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A152");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P032");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A160");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P033");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      SCNF_WaitFrames_0000(60);
      task_320 = 161;
      if ((local_F8 == 13) | (local_F8 == 174)) {
        task_324 = Script_ShowUIMessageFromID("E1032P034");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
      } else {
        local_FF = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f));
        local_F4 = local_FF;
        switch (local_F4) {
          case 0:
            task_324 = Script_ShowUIMessageFromID("E1032P080");
            while (Script_convGetIsActive((const void *)task_324)) {
              SCNF_WaitFrames_0000(1);
            }
            task_324 = 0;
            break;
          case 1:
            task_324 = Script_ShowUIMessageFromID("E1032P081");
            while (Script_convGetIsActive((const void *)task_324)) {
              SCNF_WaitFrames_0000(1);
            }
            task_324 = 0;
            break;
        }
      }
      task_324 = Script_ShowUIMessageFromID("E1032A068");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P035");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A141");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P036");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A019");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      break;
    case 11:
      task_320 = 162;
      Script_scenCreateTask(script_209DC, 0);
      task_324 = Script_ShowUIMessageFromID("E1032P021");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A152");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P025");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A160");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P033");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      SCNF_WaitFrames_0000(60);
      task_320 = 163;
      local_FF = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f));
      local_F4 = local_FF;
      switch (local_F4) {
        case 0:
          task_324 = Script_ShowUIMessageFromID("E1032P082");
          while (Script_convGetIsActive((const void *)task_324)) {
            SCNF_WaitFrames_0000(1);
          }
          task_324 = 0;
          break;
        case 1:
          task_324 = Script_ShowUIMessageFromID("E1032P083");
          while (Script_convGetIsActive((const void *)task_324)) {
            SCNF_WaitFrames_0000(1);
          }
          task_324 = 0;
          break;
        case 2:
          task_324 = Script_ShowUIMessageFromID("E1032P084");
          while (Script_convGetIsActive((const void *)task_324)) {
            SCNF_WaitFrames_0000(1);
          }
          task_324 = 0;
          task_324 = Script_ShowUIMessageFromID("E1032P085");
          while (Script_convGetIsActive((const void *)task_324)) {
            SCNF_WaitFrames_0000(1);
          }
          task_324 = 0;
          break;
      }
      task_324 = Script_ShowUIMessageFromID("E1032A068");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P086");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      break;
    case 12:
      task_320 = 370;
      Script_scenCreateTask(script_209DC, 0);
      task_324 = Script_ShowUIMessageFromID("E1032P001");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A164");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P018");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A165");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P037");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A104");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P120");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A173");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032O008");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P008");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_320 = 371;
      task_324 = Script_ShowUIMessageFromID("E1032A154");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P027");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A068");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P017");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A019");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 537, 1);
      break;
    case 13:
      task_320 = 380;
      Script_scenCreateTask(script_209DC, 0);
      task_324 = Script_ShowUIMessageFromID("E1032P001");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A164");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P018");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A170");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P044");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A200");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A172");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P045");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A173");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P046");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P047");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_320 = 381;
      task_324 = Script_ShowUIMessageFromID("E1032A201");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P049");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A176");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P050");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A177");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P051");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NOZOMI_SHRINE_KITTEN_CALL, 1);
      break;
    case 14:
      task_320 = 390;
      Script_scenCreateTask(script_209DC, 0);
      task_324 = Script_ShowUIMessageFromID("E1032P001");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A164");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P018");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A178");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P004");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P052");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A179");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A180");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A181");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_320 = 391;
      task_324 = Script_ShowUIMessageFromID("E1032P053");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P087");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P055");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A182");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P051");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NOZOMI_KITTEN_RECOVERED_CALL, 1);
      break;
    case 15:
      task_320 = 400;
      Script_scenCreateTask(script_209DC, 0);
      task_324 = Script_ShowUIMessageFromID("E1032P001");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A164");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P018");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A183");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P038");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A184");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P056");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P088");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P058");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A220");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A185");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P059");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NOZOMI_KEEP_KITTEN_CALL, 1);
      break;
    case 16:
      task_320 = 410;
      Script_scenCreateTask(script_209DC, 0);
      task_324 = Script_ShowUIMessageFromID("E1032P001");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A164");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P018");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P060");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A186");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P061");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A187");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P062");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_320 = 411;
      task_324 = Script_ShowUIMessageFromID("E1032A188");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P063");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      SCNF_WaitFrames_0000(40);
      task_324 = Script_ShowUIMessageFromID("E1032P064");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P065");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A189");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P066");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A190");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NOZOMI_STAY_AWAY_FROM_HARBOR_CALL, 1);
      break;
    case 17:
      task_320 = 270;
      Script_scenCreateTask(script_209DC, 0);
      task_324 = Script_ShowUIMessageFromID("E1032P001");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A152");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P067");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A191");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A192");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P068");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P069");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A193");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NOZOMI_TALK_TO_INE_CALL, 1);
      break;
    case 24:
      task_320 = 280;
      Script_scenCreateTask(script_209DC, 0);
      task_324 = Script_ShowUIMessageFromID("E1032P001");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A152");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P070");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A194");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P071");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A195");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P072");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P073");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A151");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P074");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P075");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P089");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P090");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_320 = 281;
      task_324 = Script_ShowUIMessageFromID("E1032A196");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P079");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A197");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NOZOMI_FEELS_WATCHED_CALL, 1);
      break;
    case 21:
      task_320 = 290;
      Script_scenCreateTask(script_209DC, 0);
      task_324 = Script_ShowUIMessageFromID("E1032P001");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A152");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P010");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P091");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P092");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A203");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P093");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      SCNF_WaitFrames_0000(60);
      task_324 = Script_ShowUIMessageFromID("E1032P094");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A204");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P095");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NOZOMI_RELAY_RACE_PHOTO_CALL, 1);
      break;
    case 26:
      task_320 = 300;
      Script_scenCreateTask(script_209DC, 0);
      task_324 = Script_ShowUIMessageFromID("E1032P021");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A152");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P096");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A104");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P097");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A103");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_320 = 301;
      local_FF = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f));
      local_F4 = local_FF;
      switch (local_F4) {
        case 0:
          task_324 = Script_ShowUIMessageFromID("E1032P098");
          while (Script_convGetIsActive((const void *)task_324)) {
            SCNF_WaitFrames_0000(1);
          }
          task_324 = 0;
          break;
        case 1:
          task_324 = Script_ShowUIMessageFromID("E1032P099");
          while (Script_convGetIsActive((const void *)task_324)) {
            SCNF_WaitFrames_0000(1);
          }
          task_324 = 0;
          break;
        case 2:
          task_324 = Script_ShowUIMessageFromID("E1032P100");
          while (Script_convGetIsActive((const void *)task_324)) {
            SCNF_WaitFrames_0000(1);
          }
          task_324 = 0;
          break;
      }
      task_324 = Script_ShowUIMessageFromID("E1032A206");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      break;
    case 18:
      task_320 = 310;
      Script_scenCreateTask(script_209DC, 0);
      task_324 = Script_ShowUIMessageFromID("E1032P021");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A152");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P101");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A207");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P102");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A208");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P103");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_320 = 311;
      task_324 = Script_ShowUIMessageFromID("E1032A209");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P104");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P105");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A210");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P106");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A154");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P107");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A204");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P108");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NOZOMI_PARENTS_IN_VANCOUVER_CALL, 1);
      break;
    case 19:
      task_320 = 320;
      Script_scenCreateTask(script_209DC, 0);
      task_324 = Script_ShowUIMessageFromID("E1032P109");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A085");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P110");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A211");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P111");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A104");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P112");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_320 = 321;
      task_324 = Script_ShowUIMessageFromID("E1032A212");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P113");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A158");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P114");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A019");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P115");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A213");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P116");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NOZOMI_CALL_NAOYUKI_AND_ICHIRO_CALL, 1);
      break;
    case 25:
      task_320 = 330;
      Script_scenCreateTask(script_209DC, 0);
      task_324 = Script_ShowUIMessageFromID("E1032P001");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A152");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P117");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A214");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P118");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A154");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P119");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P120");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_320 = 331;
      task_324 = Script_ShowUIMessageFromID("E1032A215");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P121");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A163");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P123");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A217");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NOZOMI_TRIP_INVITATION_CALL, 1);
      break;
    case 20:
      task_320 = 340;
      Script_scenCreateTask(script_209DC, 0);
      task_324 = Script_ShowUIMessageFromID("E1032P001");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A152");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P010");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P124");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P125");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A218");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P126");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A154");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P127");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A213");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P128");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A204");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P116");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NOZOMI_LAPIS_FORTUNETELLER_CALL, 1);
      break;
    case 23:
      task_320 = 350;
      Script_scenCreateTask(script_209DC, 0);
      task_324 = Script_ShowUIMessageFromID("E1032P109");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A085");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P117");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A214");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P129");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P130");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P131");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A213");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A219");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P132");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      SCNF_WaitFrames_0000(60);
      task_320 = 351;
      task_324 = Script_ShowUIMessageFromID("E1032A153");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P133");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A220");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P134");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A204");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P116");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NOZOMI_NURSERY_TEACHER_CALL, 1);
      break;
    case 22:
      task_320 = 360;
      Script_scenCreateTask(script_209DC, 0);
      task_324 = Script_ShowUIMessageFromID("E1032A085");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P010");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A221");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P135");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P136");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A222");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P137");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P138");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A223");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P139");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_320 = 361;
      task_324 = Script_ShowUIMessageFromID("E1032A224");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      SCNF_WaitFrames_0000(60);
      task_324 = Script_ShowUIMessageFromID("E1032P140");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A225");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A226");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032P004");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_NOZOMI_FIRST_MEETING_CALL, 1);
      break;
  }
  return;
}

__attribute__((scn_string_bindings(5, data_2DCE8, 7, data_2DCFC, 8, data_2DD06, 21, data_2DD88, 22, data_2DD92, 30, data_2DDE2, 32, data_2DDF6, 33, data_2DE00, 48, data_2DE96, 49, data_2DEA0)))
static void script_1E3E7(void) {
  if (Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f))) {
    task_320 = 250;
    Script_scenCreateTask(script_209DC, 0);
    {
      int32_t local_FC;
      local_FC = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f));
      if (local_FC == 0) {
        task_324 = Script_ShowUIMessageFromID("E1032E001");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
      } else if (local_FC == 1) {
        task_324 = Script_ShowUIMessageFromID("E1032E002");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
      } else {
        task_324 = Script_ShowUIMessageFromID("E1032E003");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
      }
    }
    task_324 = Script_ShowUIMessageFromID("E1032A010");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032E004");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    {
      int32_t local_FC;
      local_FC = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f));
      switch (local_FC) {
        case 0:
          task_324 = Script_ShowUIMessageFromID("E1032A011");
          while (Script_convGetIsActive((const void *)task_324)) {
            SCNF_WaitFrames_0000(1);
          }
          task_324 = 0;
          task_324 = Script_ShowUIMessageFromID("E1032E005");
          while (Script_convGetIsActive((const void *)task_324)) {
            SCNF_WaitFrames_0000(1);
          }
          task_324 = 0;
          task_324 = Script_ShowUIMessageFromID("E1032A012");
          while (Script_convGetIsActive((const void *)task_324)) {
            SCNF_WaitFrames_0000(1);
          }
          task_324 = 0;
          break;
        case 1:
          task_324 = Script_ShowUIMessageFromID("E1032A013");
          while (Script_convGetIsActive((const void *)task_324)) {
            SCNF_WaitFrames_0000(1);
          }
          task_324 = 0;
          task_324 = Script_ShowUIMessageFromID("E1032E006");
          while (Script_convGetIsActive((const void *)task_324)) {
            SCNF_WaitFrames_0000(1);
          }
          task_324 = 0;
          task_324 = Script_ShowUIMessageFromID("E1032A014");
          while (Script_convGetIsActive((const void *)task_324)) {
            SCNF_WaitFrames_0000(1);
          }
          task_324 = 0;
          break;
        case 2:
          task_324 = Script_ShowUIMessageFromID("E1032A015");
          while (Script_convGetIsActive((const void *)task_324)) {
            SCNF_WaitFrames_0000(1);
          }
          task_324 = 0;
          task_324 = Script_ShowUIMessageFromID("E1032E007");
          while (Script_convGetIsActive((const void *)task_324)) {
            SCNF_WaitFrames_0000(1);
          }
          task_324 = 0;
          task_324 = Script_ShowUIMessageFromID("E1032A016");
          while (Script_convGetIsActive((const void *)task_324)) {
            SCNF_WaitFrames_0000(1);
          }
          task_324 = 0;
          break;
      }
    }
    task_324 = Script_ShowUIMessageFromID("E1032E008");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_320 = 251;
    task_324 = Script_ShowUIMessageFromID("E1032E009");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032A017");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032E010");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    {
      int32_t local_FC;
      local_FC = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f));
      if (local_FC == 0) {
        task_324 = Script_ShowUIMessageFromID("E1032E011");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
      } else if (local_FC == 1) {
        task_324 = Script_ShowUIMessageFromID("E1032E012");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
      } else {
        task_324 = Script_ShowUIMessageFromID("E1032E013");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
      }
    }
    {
      int32_t local_FC;
      local_FC = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f));
      if (local_FC == 0) {
        task_324 = Script_ShowUIMessageFromID("E1032A018");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
      } else if (local_FC == 1) {
        task_324 = Script_ShowUIMessageFromID("E1032A019");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
      } else {
        task_324 = Script_ShowUIMessageFromID("E1032A020");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
      }
    }
    task_324 = Script_ShowUIMessageFromID("E1032A021");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  } else {
    task_320 = 260;
    Script_scenCreateTask(script_209DC, 0);
    {
      int32_t local_FC;
      local_FC = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f));
      if (local_FC == 0) {
        task_324 = Script_ShowUIMessageFromID("E1032F001");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
      } else if (local_FC == 1) {
        task_324 = Script_ShowUIMessageFromID("E1032F002");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
      } else {
        task_324 = Script_ShowUIMessageFromID("E1032F003");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
      }
    }
    task_324 = Script_ShowUIMessageFromID("E1032A022");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032F004");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    {
      int32_t local_FC;
      local_FC = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f));
      switch (local_FC) {
        case 0:
          task_324 = Script_ShowUIMessageFromID("E1032A011");
          while (Script_convGetIsActive((const void *)task_324)) {
            SCNF_WaitFrames_0000(1);
          }
          task_324 = 0;
          task_324 = Script_ShowUIMessageFromID("E1032F005");
          while (Script_convGetIsActive((const void *)task_324)) {
            SCNF_WaitFrames_0000(1);
          }
          task_324 = 0;
          task_324 = Script_ShowUIMessageFromID("E1032A012");
          while (Script_convGetIsActive((const void *)task_324)) {
            SCNF_WaitFrames_0000(1);
          }
          task_324 = 0;
          break;
        case 1:
          task_324 = Script_ShowUIMessageFromID("E1032A013");
          while (Script_convGetIsActive((const void *)task_324)) {
            SCNF_WaitFrames_0000(1);
          }
          task_324 = 0;
          task_324 = Script_ShowUIMessageFromID("E1032F006");
          while (Script_convGetIsActive((const void *)task_324)) {
            SCNF_WaitFrames_0000(1);
          }
          task_324 = 0;
          task_324 = Script_ShowUIMessageFromID("E1032A023");
          while (Script_convGetIsActive((const void *)task_324)) {
            SCNF_WaitFrames_0000(1);
          }
          task_324 = 0;
          break;
        case 2:
          task_324 = Script_ShowUIMessageFromID("E1032A024");
          while (Script_convGetIsActive((const void *)task_324)) {
            SCNF_WaitFrames_0000(1);
          }
          task_324 = 0;
          task_324 = Script_ShowUIMessageFromID("E1032F007");
          while (Script_convGetIsActive((const void *)task_324)) {
            SCNF_WaitFrames_0000(1);
          }
          task_324 = 0;
          task_324 = Script_ShowUIMessageFromID("E1032A025");
          while (Script_convGetIsActive((const void *)task_324)) {
            SCNF_WaitFrames_0000(1);
          }
          task_324 = 0;
          task_324 = Script_ShowUIMessageFromID("E1032A026");
          while (Script_convGetIsActive((const void *)task_324)) {
            SCNF_WaitFrames_0000(1);
          }
          task_324 = 0;
          break;
      }
    }
    task_320 = 261;
    task_324 = Script_ShowUIMessageFromID("E1032F008");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032F009");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    {
      int32_t local_FC;
      local_FC = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f));
      if (local_FC == 0) {
        task_324 = Script_ShowUIMessageFromID("E1032F010");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
      } else if (local_FC == 1) {
        task_324 = Script_ShowUIMessageFromID("E1032F011");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
      } else {
        task_324 = Script_ShowUIMessageFromID("E1032F012");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
      }
    }
    {
      int32_t local_FC;
      local_FC = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f));
      if (local_FC == 0) {
        task_324 = Script_ShowUIMessageFromID("E1032F013");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
      } else if (local_FC == 1) {
        task_324 = Script_ShowUIMessageFromID("E1032F014");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
      } else {
        task_324 = Script_ShowUIMessageFromID("E1032F015");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
      }
    }
    {
      int32_t local_FC;
      local_FC = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f));
      if (local_FC == 0) {
        task_324 = Script_ShowUIMessageFromID("E1032A019");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
      } else {
        task_324 = Script_ShowUIMessageFromID("E1032A018");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
      }
    }
    task_324 = Script_ShowUIMessageFromID("E1032A027");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  }
  return;
}

static int8_t script_1EEAF(void) {
  int8_t local_FF;
  local_FF = 0;
  task_320 = 55;
  Script_scenCreateTask(script_209DC, 0);
  local_FF = script_1EED7();
  return local_FF;
}

static int8_t script_1EED7(void) {
  int32_t local_FC;
  int32_t local_F8;
  int8_t local_F7;
  local_F7 = 0;
  task_324 = Script_ShowUIMessageFromID("E1032A034");
  while (Script_convGetIsActive((const void *)task_324)) {
    SCNF_WaitFrames_0000(1);
  }
  task_324 = 0;
  if (Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f))) {
    local_F8 = 0;
  } else {
    local_F8 = 2;
  }
  script_1EFF2(local_F8);
  local_FC = script_1F144();
  script_1F15F(local_FC);
  if (script_1F0DB(local_F8, local_FC)) {
    if (local_F8 == 0) {
      local_F8 = 2;
    } else {
      local_F8 = 0;
    }
    task_320 = 56;
    Script_mapmSetCamFlags(0, 0);
    Script_charaVisUtils(CHARA_AKIR, 1);
    script_1EFF2(local_F8);
    local_FC = script_1F144();
    script_1F15F(local_FC);
    Script_mapmSetCamFlags(0, 1);
    Script_charaVisUtils(CHARA_AKIR, 0);
    if (script_1F0DB(local_F8, local_FC)) {
      local_F7 = script_1F535();
    } else {
      script_1F248(local_FC);
    }
  } else {
    script_1F248(local_FC);
  }
  return local_F7;
}

static void script_1EFF2(int32_t arg0) {
  int32_t local_FC;
  local_FC = arg0;
  switch (local_FC) {
    case 0:
      task_324 = Script_ShowUIMessageFromID("E1032H005");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      break;
    case 1:
      task_324 = Script_ShowUIMessageFromID("E1032H004");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      break;
    case 2:
      task_324 = Script_ShowUIMessageFromID("E1032H009");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      break;
    case 3:
      task_324 = Script_ShowUIMessageFromID("E1032H006");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      break;
  }
  return;
}

static int32_t script_1F0DB(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  int32_t local_F8;
  local_FC = -1;
  local_F8 = arg1;
  switch (local_F8) {
    case 5:
      local_FC = 1;
      break;
    case 6:
      local_FC = 2;
      break;
    case 7:
      local_FC = 3;
      break;
    case 8:
      local_FC = 0;
      break;
  }
  if (arg0 == local_FC) {
    (void)1;
  } else {
    (void)0;
  }
  end:;
}

static int32_t script_1F144(void) {
  return SCNF_RequestActionSelectorFromParams_0008(7, 8, 6, 5, 300, 0xFFFF, ACTION_SELECTOR_MODE_STANDARD);
}

static void script_1F15F(int32_t arg0) {
  int32_t local_FC;
  local_FC = arg0;
  switch (local_FC) {
    case 5:
      task_324 = Script_ShowUIMessageFromID("E1032A042");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      break;
    case 6:
      task_324 = Script_ShowUIMessageFromID("E1032A040");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      break;
    case 7:
      task_324 = Script_ShowUIMessageFromID("E1032A043");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      break;
    case 8:
      task_324 = Script_ShowUIMessageFromID("E1032A041");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      break;
  }
  return;
}

__attribute__((scn_string_bindings(1, data_2DF18, 2, data_2DF22, 3, data_2DF2C, 4, data_2DF36)))
static void script_1F248(int32_t arg0) {
  int32_t local_FC;
  task_320 = 58;
  script_1F46D(1);
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 714)) {
    if ((arg0 == 5) | (arg0 == 6) | (arg0 == 7) | (arg0 == 8)) {
      int32_t local_F8;
      local_F8 = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 4.0f));
      if (local_F8 == 0) {
      } else if (local_F8 == 1) {
        task_324 = Script_ShowUIMessageFromID("E1032A045");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
      } else if (local_F8 == 2) {
        task_324 = Script_ShowUIMessageFromID("E1032A046");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
      } else {
        task_324 = Script_ShowUIMessageFromID("E1032A047");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
      }
    } else {
      int32_t local_F8;
      local_F8 = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f));
      if (local_F8 == 0) {
      } else if (local_F8 == 1) {
        task_324 = Script_ShowUIMessageFromID("E1032A046");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
      } else {
        task_324 = Script_ShowUIMessageFromID("E1032A047");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
      }
    }
  } else {
    task_324 = Script_ShowUIMessageFromID("E1032A044");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  }
  Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 714, 1);
  if (Script_StepUtils(GET_STEP_HIGHER, 0) == 9) {
    local_FC = Script_scenUtils(SCENARIO_CMD_GET_STEP_LUT, 9);
    local_FC = local_FC + 1;
    Script_scenUtils(SCENARIO_CMD_SET_STEP_LUT, 9, local_FC);
  }
  return;
}

static void script_1F436(void) {
  task_324 = Script_ShowUIMessageFromID("E1032A051");
  while (Script_convGetIsActive((const void *)task_324)) {
    SCNF_WaitFrames_0000(1);
  }
  task_324 = 0;
  SCNF_WaitFrames_0000(10);
  return;
}

static void script_1F46D(int32_t arg0) {
  Script_executeAicaCommand(525739, 0, 0);
  SCNF_WaitFrames_0000(10);
  if (arg0) {
    task_324 = Script_ShowUIMessageFromID("E1032A039");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
  }
  Script_executeAicaCommand(394667, 0, 0);
  SCNF_WaitFrames_0000(15);
  Script_executeAicaCommand(460203, 0, 0);
  SCNF_WaitFrames_0000(15);
  Script_executeAicaCommand(394667, 0, 0);
  SCNF_WaitFrames_0000(15);
  Script_executeAicaCommand(460203, 0, 0);
  SCNF_WaitFrames_0000(15);
  Script_executeAicaCommand(394667, 0, 0);
  SCNF_WaitFrames_0000(15);
  Script_executeAicaCommand(460203, 0, 0);
  SCNF_WaitFrames_0000(15);
  return;
}

__attribute__((scn_string_bindings(0, data_2DF5E, 1, data_2DF68, 2, data_2DF72, 3, data_2DF7C, 4, data_2DF86, 5, data_2DF90, 6, data_2DF9A, 7, data_2DFA4, 8, data_2DFAE, 10, data_2DFC2, 11, data_2DFCC, 15, data_2DFF4, 16, data_2DFFE, 17, data_2E008, 18, data_2E012, 19, data_2E01C, 20, data_2E026, 21, data_2E030, 22, data_2E03A, 23, data_2E044, 24, data_2E04E, 25, data_2E058, 26, data_2E062, 27, data_2E06C, 28, data_2E076, 29, data_2E080, 33, data_2E0A8, 34, data_2E0B2, 35, data_2E0BC, 36, data_2E0C6, 37, data_2E0D0, 38, data_2E0DA, 39, data_2E0E4, 40, data_2E0EE, 41, data_2E0F8, 42, data_2E102, 43, data_2E10C, 44, data_2E116, 45, data_2E120, 46, data_2E12A, 47, data_2E134, 48, data_2E13E, 49, data_2E148, 52, data_2E166, 53, data_2E170, 54, data_2E17A, 59, data_2E1AC)))
static int8_t script_1F535(void) {
  int32_t local_FC;
  int32_t local_F8;
  int8_t local_F7;
  int32_t local_F0;
  local_F8 = Script_StepUtils(GET_STEP_HIGHER, 0) * 10 + Script_StepUtils(GET_STEP_LOWER, 0);
  local_F7 = 0;
  task_320 = 57;
  local_FC = Script_StepUtils(GET_STEP_HIGHER, 0);
  if (local_FC >= 17) {
    task_324 = Script_ShowUIMessageFromID("E1032H007");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032A120");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032H010");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032A066");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032H011");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    SCNF_WaitFrames_0000(60);
    if (((local_F8 >= 170) & (local_F8 <= 193)) | ((local_F8 >= 220) & (local_F8 <= 241))) {
      local_F0 = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f));
      if (local_F0 == 0) {
        task_324 = Script_ShowUIMessageFromID("E1032J001");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
        task_324 = Script_ShowUIMessageFromID("E1032A019");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
        task_324 = Script_ShowUIMessageFromID("E1032A131");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
        task_324 = Script_ShowUIMessageFromID("E1032J010");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
        task_324 = Script_ShowUIMessageFromID("E1032J013");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
      } else {
        task_324 = Script_ShowUIMessageFromID("E1032J001");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
        task_324 = Script_ShowUIMessageFromID("E1032A019");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
        task_324 = Script_ShowUIMessageFromID("E1032A134");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
        task_324 = Script_ShowUIMessageFromID("E1032J015");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
        task_324 = Script_ShowUIMessageFromID("E1032A135");
        while (Script_convGetIsActive((const void *)task_324)) {
          SCNF_WaitFrames_0000(1);
        }
        task_324 = 0;
      }
      script_1F46D(0);
    } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 546)) {
      task_324 = Script_ShowUIMessageFromID("E1032J001");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A019");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032J018");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A163");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032J019");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032J020");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A068");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      Script_executeAicaCommand(525739, 0, 0);
      SCNF_WaitFrames_0000(10);
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_CHEN_RESIDENCE_CALLED, 1);
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 546, 0);
    } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_CHEN_RESIDENCE_CALLED)) {
      task_324 = Script_ShowUIMessageFromID("E1032J016");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A161");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032J017");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A162");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      Script_executeAicaCommand(525739, 0, 0);
      SCNF_WaitFrames_0000(10);
    } else {
      task_324 = Script_ShowUIMessageFromID("E1032J001");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A019");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A131");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032J010");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A132");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032J011");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032J012");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_CHEN_RESIDENCE_CALLED, 1);
    }
  } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_MS08_SCENE_EXIT_COMPLETE) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_MS8A_POST_MS08_FOLLOWUP_COMPLETE) == 0)) {
    task_320 = 64;
    task_324 = Script_ShowUIMessageFromID("E1032H007");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032A120");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032H010");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032A066");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032H011");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    SCNF_WaitFrames_0000(60);
    if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 546)) {
      task_320 = 65;
      task_324 = Script_ShowUIMessageFromID("E1032J001");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A019");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032J018");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A163");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032J019");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032J020");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A068");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      Script_executeAicaCommand(525739, 0, 0);
      SCNF_WaitFrames_0000(10);
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_CHEN_RESIDENCE_CALLED, 1);
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 546, 0);
    } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_CHEN_RESIDENCE_CALLED)) {
      task_320 = 66;
      task_324 = Script_ShowUIMessageFromID("E1032J016");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A161");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032J017");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A162");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      Script_executeAicaCommand(525739, 0, 0);
      SCNF_WaitFrames_0000(10);
    } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_YDB1_SECRET_BOX_OPENING_COMPLETE)) {
      task_324 = Script_ShowUIMessageFromID("E1032J001");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A067");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032J002");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A068");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_E1032_CHEN_RESIDENCE_CALLED, 1);
    } else {
      task_324 = Script_ShowUIMessageFromID("E1032J001");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A019");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032J003");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A069");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032J004");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
      task_324 = Script_ShowUIMessageFromID("E1032A070");
      while (Script_convGetIsActive((const void *)task_324)) {
        SCNF_WaitFrames_0000(1);
      }
      task_324 = 0;
    }
  } else {
    local_F7 = 1;
    task_324 = Script_ShowUIMessageFromID("E1032H007");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032A048");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032H008");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_320 = 63;
    task_324 = Script_ShowUIMessageFromID("E1032A049");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    task_324 = Script_ShowUIMessageFromID("E1032A050");
    while (Script_convGetIsActive((const void *)task_324)) {
      SCNF_WaitFrames_0000(1);
    }
    task_324 = 0;
    script_1F46D(1);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 714, 1);
  }
  return local_F7;
}

static void script_20212(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  Script_EVT_GetPosition(SHEN_NAME('TBOX'), -1, &local_F4, 0);
  Script_mtwkGetSetRotation(SHEN_NAME('TBOX'), &local_E8, 0);
  (&local_E8)->y = (&local_E8)->y + -1820;
  local_F4.x = (&local_F4)->x + Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_E8)->y) * 1.0f;
  (&local_F4)->z = (&local_F4)->z + Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_E8)->y) * 1.0f;
  Script_motmToggleFlags(CHARA_AKIR, 1, 0x40);
  Script_motmSetBlendFrames(CHARA_AKIR, MOTM_BLEND_FRAMES_30);
  Script_charaToggleMotEnabled(CHARA_AKIR, 1);
  Script_scenCharaSubmitMoveTargetRequest(CHARA_AKIR, &local_F4, (&local_E8)->y, MN_AKI_AKI_STAND_LP, 0);
  while (Script_IsMoveTargetRequestActive(CHARA_AKIR) != 0) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_202D5(void) {
  Vector3i local_F4;
  Vector3i local_E8;
  Vector3 local_DC;
  Vector3i local_D0;
  Vector3i local_C4;
  script_216CD();
  Script_InitializeOsag(CHARA_AKIR);
  Script_motmSetBlendFrames(CHARA_AKIR, MOTM_BLEND_FRAMES_5);
  Script_PlayCharaMotion(CHARA_AKIR, 32001, 23.0f, 53.0f, -1.0f, 1.0f, 0);
  Script_motmToggleFlags(CHARA_AKIR, 2, 0x40);
  task_320 = 62;
  Script_scenCreateTask(script_209DC, 0);
  for (;;) {
    if (Script_IsCharaMotionFinished(CHARA_AKIR)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_executeAicaCommand(263851, 0, 0);
  local_F4.x = 0;
  (&local_F4)->y = 1307;
  (&local_F4)->z = 0;
  local_E8.x = 0;
  (&local_E8)->y = -601;
  (&local_E8)->z = 0;
  Script_PlayCharaMotion(CHARA_AKIR, 32001, 53.0f, 56.0f, -1.0f, 0.2f, 0);
  for (;;) {
    Script_charaPosRotUtils(SHEN_NAME('TBOX'), 16, 0, &local_E8, 1);
    Script_charaPosRotUtils(SHEN_NAME('TBOX'), 21, 0, &local_F4, 1);
    if (Script_IsCharaMotionFinished(CHARA_AKIR)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  local_F4.x = 0;
  (&local_F4)->y = 490;
  (&local_F4)->z = 0;
  local_E8.x = 0;
  (&local_E8)->y = -225;
  (&local_E8)->z = 0;
  Script_PlayCharaMotion(CHARA_AKIR, 32001, 56.0f, 63.0f, -1.0f, 0.5f, 0);
  for (;;) {
    Script_charaPosRotUtils(SHEN_NAME('TBOX'), 16, 0, &local_E8, 1);
    Script_charaPosRotUtils(SHEN_NAME('TBOX'), 21, 0, &local_F4, 1);
    if (Script_IsCharaMotionFinished(CHARA_AKIR)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_PlayCharaMotion(CHARA_AKIR, 32001, 63.0f, 90.0f, -1.0f, 1.0f, 0);
  for (;;) {
    if (Script_IsCharaMotionFinished(CHARA_AKIR)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_motmToggleFlags(CHARA_AKIR, 2, 0x40);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_EVT_GetPosition(SHEN_NAME('TBOX'), -1, &local_DC, 0);
  Script_mtwkGetSetRotation(SHEN_NAME('TBOX'), &local_D0, 0);
  local_DC.x = (&local_DC)->x + Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_D0)->y) * 1.5f;
  (&local_DC)->z = (&local_DC)->z + Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_D0)->y) * 1.5f;
  Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &local_DC);
  (&local_D0)->y = (&local_D0)->y + 0x8000;
  (&local_C4)->y = (&local_D0)->y;
  Script_EVT_SetRotation(CHARA_AKIR, MATRIX_XFORM_Y, &local_C4, 0);
  Script_charaSetRefboardFlagTo(task_308, 0);
  Script_charaSetRefboardFlagTo(task_30C, 0);
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 2);
  Script_scenToggleActiveFor(CHARA_AKIR, 1);
  Script_charaPosRotUtils(SHEN_NAME('TBOX'), 21, 0, &task_398, 0);
  Script_charaPosRotUtils(SHEN_NAME('TBOX'), 16, 0, &task_3A4, 0);
  task_320 = -1;
  Script_SetCPAMFlags(CHARA_AKIR, 0);
  Script_SetCameraFieldOfView(10922);
  Script_convSetCamInactiveFlag(1);
  Script_SetGameStateTo(AUTH_None);
  return;
}

static void script_205FD(void) {
  Script_seqEnterEvtCam(6220, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
  script_20212();
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_motmSetBlendFrames(CHARA_AKIR, MOTM_BLEND_FRAMES_30);
  Script_motmToggleFlags(CHARA_AKIR, 1, 0x40);
  Script_PlayCharaMotion(CHARA_AKIR, 32002, 10.0f, 50.0f, -1.0f, 1.0f, 0);
  for (;;) {
    if (Script_IsCharaMotionFinished(CHARA_AKIR)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_20682(void) {
  Script_PlayCharaMotion(CHARA_AKIR, 32002, 20.0f, 50.0f, -1.0f, 1.0f, 0);
  Script_motmToggleFlags(CHARA_AKIR, 1, 0x20);
  for (;;) {
    if (Script_IsCharaMotionFinished(CHARA_AKIR)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_motmToggleFlags(CHARA_AKIR, 2, 0x20);
  Script_motmSetBlendFrames(CHARA_AKIR, MOTM_BLEND_FRAMES_10);
  SCNF_WaitFrames_0000(10);
  return;
}

static void script_20711(void) {
  Vector3i local_F4;
  Vector3i local_E8;
  int32_t local_E4;
  Script_seqEnterEvtCam(6220, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
  Script_ryoControl(PCCTL_DisablePlayerControl);
  script_20212();
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_ccowUtils(CHARA_AKIR, 2);
  Script_motmSetBlendFrames(CHARA_AKIR, MOTM_BLEND_FRAMES_5);
  Script_motmToggleFlags(CHARA_AKIR, 2, 4);
  Script_PlayCharaMotion(CHARA_AKIR, 32002, 25.0f, 51.0f, -1.0f, 1.0f, 0);
  for (;;) {
    if (Script_IsCharaMotionFinished(CHARA_AKIR)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_PlayCharaMotion(CHARA_AKIR, 32002, 51.0f, 85.0f, -1.0f, 1.0f, 0);
  Script_charaPosRotUtils(SHEN_NAME('TBOX'), 21, 0, &task_398, 2);
  Script_charaPosRotUtils(SHEN_NAME('TBOX'), 16, 0, &task_3A4, 2);
  local_F4.x = 0;
  (&local_F4)->y = 713;
  (&local_F4)->z = 0;
  local_E8.x = 0;
  (&local_E8)->y = -328;
  (&local_E8)->z = 0;
  Script_executeAicaCommand(263851, 0, 0);
  local_E4 = Script_scenCreateTask(script_20927, 0);
  for (;;) {
    Script_charaPosRotUtils(SHEN_NAME('TBOX'), 16, 0, &local_E8, 1);
    Script_charaPosRotUtils(SHEN_NAME('TBOX'), 21, 0, &local_F4, 1);
    if (Script_IsCharaMotionFinished(CHARA_AKIR)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_PlayCharaMotion(CHARA_AKIR, 32002, 85.0f, 95.0f, -1.0f, 1.0f, 0);
  for (;;) {
    if (Script_IsCharaMotionFinished(CHARA_AKIR)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_charaPosRotUtils(SHEN_NAME('TBOX'), 21, 0, &task_398, 0);
  Script_charaPosRotUtils(SHEN_NAME('TBOX'), 16, 0, &task_3A4, 0);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_srchControl((void *)local_E4);
  Script_motiSetAngFlags(CHARA_AKIR, 0);
  Script_seqEnterEvtCam(6219, task_304, task_304);
  SCNF_WaitFrames_0000(1);
  return;
}

static int32_t script_20927(void) {
  Vector3i local_F4;
  Vector3i local_E8;
  Vector3 local_DC;
  Vector3 local_D0;
  int32_t local_CC;
  Script_motiSetAngFlags(CHARA_AKIR, 1);
  for (;;) {
    Script_charaPosRotUtils(SHEN_NAME('TBOX'), 21, &local_DC, &local_F4, 2);
    Script_charaPosRotUtils(SHEN_NAME('TBOX'), 16, &local_D0, &local_E8, 2);
    Script_charaSetupXForm(SHEN_NAME('TBOX'), 21, &local_DC);
    local_CC = (&local_F4)->y + (&local_E8)->y + 51882;
    local_DC.x = (&local_DC)->x + Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, local_CC) * 0.6f;
    (&local_DC)->y = 1.2f;
    (&local_DC)->z = (&local_DC)->z + Script_MathUtils(SCRIPT_MATH_COS_FIXED16, local_CC) * 0.6f;
    Script_SetMotnFlags(CHARA_AKIR, 17, &local_DC, 0x38000000);
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_209DC(void) {
  int32_t local_FC;
  for (;;) {
    local_FC = task_320;
    switch (local_FC) {
      case -1:
        goto L021350;
        break;
      case 0:
        break;
      case 51:
        Script_seqEnterEvtCam(6222, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 53:
        Script_seqEnterEvtCam(6222, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 54:
        Script_seqEnterEvtCam(6222, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 55:
        Script_seqEnterEvtCam(6223, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 56:
        Script_seqEnterEvtCam(6224, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 57:
        Script_seqEnterEvtCam(6225, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 58:
        Script_seqEnterEvtCam(6226, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 62:
        Script_seqEnterEvtCam(6221, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 63:
        Script_seqEnterEvtCam(6227, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 64:
        Script_seqEnterEvtCam(6232, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 65:
        Script_seqEnterEvtCam(6233, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 66:
        Script_seqEnterEvtCam(6230, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 70:
        local_FC = task_330;
        switch (local_FC) {
          case 0:
            Script_seqEnterEvtCam(6228, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
            break;
          case 1:
            Script_seqEnterEvtCam(6229, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
            break;
          case 2:
            Script_seqEnterEvtCam(6230, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
            break;
          case 3:
            Script_seqEnterEvtCam(6231, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
            break;
          case 4:
            Script_seqEnterEvtCam(6232, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
            break;
          case 5:
            Script_seqEnterEvtCam(6233, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
            break;
          case 6:
            Script_seqEnterEvtCam(6234, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
            break;
        }
        task_330 = task_330 + 1;
        if (task_330 > 6) {
          task_330 = 0;
        }
        break;
      case 80:
        Script_seqEnterEvtCam(6232, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 81:
        Script_seqEnterEvtCam(6232, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 82:
        Script_seqEnterEvtCam(6234, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 90:
        Script_seqEnterEvtCam(6233, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 100:
        Script_seqEnterEvtCam(6234, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 101:
        Script_seqEnterEvtCam(6232, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 110:
        Script_seqEnterEvtCam(6231, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 111:
        Script_seqEnterEvtCam(6232, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 120:
        Script_seqEnterEvtCam(6230, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 130:
        Script_seqEnterEvtCam(6228, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 131:
        Script_seqEnterEvtCam(6230, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 140:
        Script_seqEnterEvtCam(6228, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 141:
        Script_seqEnterEvtCam(6229, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 150:
        Script_seqEnterEvtCam(6234, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 151:
        Script_seqEnterEvtCam(6232, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 160:
        Script_seqEnterEvtCam(6228, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 161:
        Script_seqEnterEvtCam(6230, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 162:
        Script_seqEnterEvtCam(6228, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 163:
        Script_seqEnterEvtCam(6230, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 170:
        Script_seqEnterEvtCam(6234, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 180:
        Script_seqEnterEvtCam(6233, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 190:
        Script_seqEnterEvtCam(6228, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 191:
        Script_seqEnterEvtCam(6229, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 200:
        Script_seqEnterEvtCam(6229, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 210:
        Script_seqEnterEvtCam(6231, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 220:
        Script_seqEnterEvtCam(6233, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 230:
        Script_seqEnterEvtCam(6234, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 231:
        Script_seqEnterEvtCam(6232, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 240:
        Script_seqEnterEvtCam(6230, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 250:
        Script_seqEnterEvtCam(6231, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 251:
        Script_seqEnterEvtCam(6232, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 260:
        Script_seqEnterEvtCam(6228, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 261:
        Script_seqEnterEvtCam(6230, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 270:
        Script_seqEnterEvtCam(6234, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 280:
        Script_seqEnterEvtCam(6231, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 281:
        Script_seqEnterEvtCam(6232, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 290:
        Script_seqEnterEvtCam(6230, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 300:
        Script_seqEnterEvtCam(6228, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 301:
        Script_seqEnterEvtCam(6230, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 310:
        Script_seqEnterEvtCam(6234, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 311:
        Script_seqEnterEvtCam(6232, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 320:
        Script_seqEnterEvtCam(6234, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 321:
        Script_seqEnterEvtCam(6232, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 330:
        Script_seqEnterEvtCam(6228, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 331:
        Script_seqEnterEvtCam(6229, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 340:
        Script_seqEnterEvtCam(6230, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 350:
        Script_seqEnterEvtCam(6231, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 351:
        Script_seqEnterEvtCam(6232, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 360:
        Script_seqEnterEvtCam(6228, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 361:
        Script_seqEnterEvtCam(6229, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 370:
        Script_seqEnterEvtCam(6231, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 371:
        Script_seqEnterEvtCam(6232, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 380:
        Script_seqEnterEvtCam(6228, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 381:
        Script_seqEnterEvtCam(6229, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 390:
        Script_seqEnterEvtCam(6234, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 391:
        Script_seqEnterEvtCam(6232, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 400:
        Script_seqEnterEvtCam(6233, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 410:
        Script_seqEnterEvtCam(6228, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
      case 411:
        Script_seqEnterEvtCam(6230, SHEN_NAME('TBOX'), SHEN_NAME('TBOX'));
        break;
    }
    if (task_320) {
      task_320 = 0;
    }
    SCNF_WaitFrames_0000(1);
  }
  L021350:;
  task_320 = 0;
  return Script_srchControl(0);
}

__attribute__((scn_string_bindings(0, data_2E1DE, 1, data_2E1EE)))
static void script_2135E(void) {
  Vector4 local_F0;
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
  Script_CycleManagerUtils(CYCLE_CMD_PROCESS_SND);
  Script_scenUtils(SCENARIO_CMD_INIT_SCNT);
  SCNF_WaitFrames_0000(1);
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, "/scene/02/JD00/", "TBOX") == 0) {
    SCNF_WaitFrames_0000(1);
  }
  SCNF_WaitFrames_0000(1);
  Script_FaceEx(CHARA_AKIR, 1, 0, 2);
  Script_shenClearFreeState(0);
  local_F0.x = 0.8f;
  (&local_F0)->y = 0.8f;
  (&local_F0)->z = 0.8f;
  (&local_F0)->w = 0.8f;
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 1);
  Script_charaSetRefboardFlagTo(task_308, 1);
  Script_charaSetRefboardFlagTo(task_30C, 1);
  Script_SetCharaRefBoardPos(CHARA_AKIR, &local_F0);
  Script_SetCharaRefBoardPos(task_308, &local_F0);
  Script_SetCharaRefBoardPos(task_30C, &local_F0);
  if (task_32D < 0) {
    script_205FD();
  } else {
    script_20711();
  }
  Script_FaceEx(CHARA_AKIR, 0, 0, 2);
  Script_shenClearFreeState(1);
  return;
}

__attribute__((scn_string_bindings(0, data_2E1F3)))
static void script_2145D(void) {
  Script_FaceEx(CHARA_AKIR, 1, 0, 2);
  Script_shenClearFreeState(0);
  if (task_32D < 0) {
    script_20682();
  } else {
    script_202D5();
  }
  SCNF_WaitFrames_0000(1);
  Script_FaceEx(CHARA_AKIR, 0, 0, 2);
  Script_shenClearFreeState(1);
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "TBOX");
  Script_scenUtils(SCENARIO_CMD_UPDATE_TIMER);
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 1);
  SCNF_WaitFrames_0000(10);
  return;
}

static void script_214D0(void) {
  Script_SetCPAMFlags(CHARA_AKIR, 0);
  {
    Vector3 local_F4;
    Vector3i local_E8;
    Script_EVT_GetPosition(task_304, -1, &local_F4, 0);
    Script_mtwkGetSetRotation(task_304, &local_E8, 0);
    local_F4.x = (&local_F4)->x + Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_E8)->y) * 0.52f;
    (&local_F4)->y = 0.0f;
    (&local_F4)->z = (&local_F4)->z + Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_E8)->y) * 0.52f;
    Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &local_F4);
    {
      Vector3i local_DC;
      (&local_DC)->y = (&local_E8)->y - 1820;
      Script_EVT_SetRotation(CHARA_AKIR, MATRIX_XFORM_Y, &local_DC, 0);
    }
  }
  Script_InitializeOsag(CHARA_AKIR);
  Script_motmToggleFlags(CHARA_AKIR, 2, 0x40);
  Script_PlayCharaMotion(CHARA_AKIR, 32003, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_charaDisplay(CHARA_AKIR, CDM_Hide);
  Script_ResetHandMotion(CHARA_AKIR, 1, 0, 0);
  Script_handSetFloatsFor(CHARA_AKIR, 0, data_2C0C4, 1);
  Script_ConfigHandMotionIK(CHARA_AKIR, 0, 0, 0, 1);
  Script_ResetHandMotion(CHARA_AKIR, 1, 1, 0);
  Script_handSetFloatsFor(CHARA_AKIR, 1, data_2C0C4, 1);
  Script_ConfigHandMotionIK(CHARA_AKIR, 1, 0, 0, 1);
  Script_scenCreateTask(script_218DA, 0);
  SCNF_WaitFrames_0000(1);
  Script_TELMUtils(TELM_CMD_Cleanup, task_304);
  Script_TELMUtils(TELM_CMD_SetTargetTaskAndMode, task_304, 0x52494B41, 3);
  {
    Vector3 local_F4;
    Vector3i local_E8;
    local_F4.x = -0.03f;
    (&local_F4)->y = -0.01f;
    (&local_F4)->z = -0.02f;
    local_E8.x = 59162;
    (&local_E8)->y = 7824;
    (&local_E8)->z = 62441;
    Script_TELMUtils(TELM_CMD_SetCustomReceiverOffset, task_304, &local_F4);
    Script_TELMUtils(TELM_CMD_SetCustomReceiverEuler, task_304, &local_E8);
  }
  Script_charaDisplay(CHARA_AKIR, CDM_Show);
  Script_executeAicaCommand(591275, 0, 0);
  return;
}

static void script_216B9(void) {
  task_320 = -1;
  Script_TELMUtils(TELM_CMD_Cleanup, task_304);
  return;
}

static void script_216CD(void) {
  Vector3 local_F4;
  Script_EVT_GetPosition(task_304, -1, &local_F4, 0);
  {
    Vector3i local_E8;
    Script_mtwkGetSetRotation(task_304, &local_E8, 0);
    local_F4.x = (&local_F4)->x + Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_E8)->y) * 0.5f;
    (&local_F4)->y = 0.0f;
    (&local_F4)->z = (&local_F4)->z + Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_E8)->y) * 0.5f;
  }
  Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &local_F4);
  {
    Vector3i local_E8;
    Vector3i local_DC;
    Script_mtwkGetSetRotation(SHEN_NAME('TBOX'), &local_E8, 0);
    (&local_DC)->y = (&local_E8)->y;
    Script_EVT_SetRotation(CHARA_AKIR, MATRIX_XFORM_Y, &local_DC, 0);
  }
  Script_InitializeOsag(CHARA_AKIR);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  return;
}

static void script_21795(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  task_395 = 1;
  Script_motmToggleFlags(CHARA_AKIR, 2, 4);
  Script_motmToggleFlags(CHARA_AKIR, 1, 0x40);
  Script_motmSetBlendFrames(CHARA_AKIR, MOTM_BLEND_FRAMES_20);
  Script_PlayCharaMotion(CHARA_AKIR, 32004, -1.0f, 55.0f, -1.0f, 1.5f, 0);
  for (;;) {
    if (Script_IsCharaMotionFinished(CHARA_AKIR)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_handSetFloatsFor(CHARA_AKIR, 0, data_2C1A8, 5);
  Script_TELMUtils(TELM_CMD_ResetPeerState, task_304);
  Script_executeAicaCommand(67243, 0, 0);
  Script_PlayCharaMotion(CHARA_AKIR, 32004, 55.0f, -1.0f, -1.0f, 1.5f, 0);
  Script_motmSetBlendFrames(CHARA_AKIR, MOTM_BLEND_FRAMES_5);
  for (;;) {
    if (Script_IsCharaMotionFinished(CHARA_AKIR)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
  Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
  local_F4.x = 0.0f;
  (&local_F4)->y = 0.0f;
  (&local_F4)->z = 0.0f;
  local_E8.x = 0;
  (&local_E8)->y = 0;
  (&local_E8)->z = 0;
  Script_TELMUtils(TELM_CMD_SetCustomReceiverOffset, task_304, &local_F4);
  Script_TELMUtils(TELM_CMD_SetCustomReceiverEuler, task_304, &local_E8);
  return;
}

static int32_t script_218DA(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  local_F4.x = -0.164926f;
  (&local_F4)->y = 1.545938f;
  (&local_F4)->z = -0.138711f;
  local_E8.x = -8009;
  (&local_E8)->y = 4187;
  (&local_E8)->z = -2184;
  task_395 = 0;
  for (;;) {
    if (task_395) {
      break;
    }
    Script_SetHandRootRotation(CHARA_AKIR, 0, 0x15, &local_E8);
    Script_SetMotnFlags(CHARA_AKIR, 11, &local_F4, 0x39000000);
    SCNF_WaitFrames_0000(1);
  }
  return Script_srchControl(0);
}

static void script_2195D(void) {
  Vector3i local_F4;
  local_F4.x = 0;
  (&local_F4)->y = -0x4000;
  (&local_F4)->z = 0;
  Script_charaPosRotUtils(SHEN_NAME('ITMN'), 7, 0, &local_F4, 0);
  (&local_F4)->y = 0x4000;
  Script_charaPosRotUtils(SHEN_NAME('ITMN'), 12, 0, &local_F4, 0);
  Script_scenToggleVisibleFor(SHEN_NAME('ITMN'), 0);
  return;
}

__attribute__((scn_string_bindings(0, data_2E380, 2, data_2E395, 4, data_2E3AA)))
static void script_219B3(void) {
  if (Script_WeatherUtils_0(WeatherDataUtil_QueryCurrentFCASFlag, 3)) {
    while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, "/scene/02/JD00/", "MAPY") == 0) {
      SCNF_WaitFrames_0000(1);
    }
  } else {
    while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, "/scene/02/JD00/", "MAPN") == 0) {
      SCNF_WaitFrames_0000(1);
    }
  }
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2)) {
    SCNF_WaitFrames_0000(1);
  }
  if (Script_WeatherUtils_0(WeatherDataUtil_QueryCurrentFCASFlag, 5)) {
    while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, "/scene/02/JD00/", "MAPS") == 0) {
      SCNF_WaitFrames_0000(1);
    }
    while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2)) {
      SCNF_WaitFrames_0000(1);
    }
  }
  {
    Vector3 local_F4;
    Vector3i local_E8;
    Script_charaPosRotUtils(SHEN_NAME('YAMN'), 7, &local_F4, 0, 2);
    local_F4.x = (&local_F4)->x - 0.05f;
    local_E8.x = 0;
    (&local_E8)->y = -0x4000;
    (&local_E8)->z = 0;
    Script_charaPosRotUtils(SHEN_NAME('YAMN'), 7, &local_F4, &local_E8, 0);
  }
  {
    Vector3 local_F4;
    Vector3i local_E8;
    Script_charaPosRotUtils(SHEN_NAME('YAMN'), 12, &local_F4, 0, 2);
    local_F4.x = (&local_F4)->x + 0.05f;
    local_E8.x = 0;
    (&local_E8)->y = 0x4000;
    (&local_E8)->z = 0;
    Script_charaPosRotUtils(SHEN_NAME('YAMN'), 12, &local_F4, &local_E8, 0);
  }
  Script_ccowUtils(SHEN_NAME('YAMN'), 2);
  Script_scenToggleVisibleFor(SHEN_NAME('YAMN'), 0);
  {
    Vector3 local_F4;
    local_F4.x = 1.2f;
    (&local_F4)->y = 1.0f;
    (&local_F4)->z = 1.0f;
    Script_hmdlApplyScale(SHEN_NAME('ITMN'), &local_F4);
  }
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_NOZOMI_KIDNAPPED) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOZOMI_RESCUE_RUSH_ACTIVE) == 0)) {
    Script_ccowUtils(SHEN_NAME('ITMN'), 1);
  } else {
    script_2195D();
  }
  return;
}

static int32_t script_21B77(void) {
  int32_t unused_FC;
  GameDateTime local_F4;
  int8_t local_F3;
  int8_t local_F2;
  int8_t local_F1;
  int8_t local_F0;
  Script_shenSetLightingMode(1);
  local_F1 = -1;
  local_F3 = -1;
  local_F2 = 0;
  local_F0 = 0;
  Script_mapmSetRenderFlag(3, 0);
  Script_mapmSetRenderFlag(5, 0);
  Script_mapmSetRenderFlag(7, 0);
  SCNF_WaitFrames_0000(1);
  for (;;) {
    Script_ReturnTimeDate(&local_F4);
    if ((&local_F4)->minutes != local_F3) {
      if ((-((&local_F4)->hours >= 19) | (-((&local_F4)->hours >= 0) & -((&local_F4)->hours <= 4))) & -(local_F1 != 0)) {
        if (local_F2 == 18) {
          local_F1 = 0;
          Script_scenCreateTask(script_21EC6, 0);
        } else {
          local_F1 = 0;
          Script_charaDisplay(SHEN_NAME('TAXI'), CDM_Hide);
          if (-1) {
            Script_charaDisplay(SHEN_NAME('TAXI'), CDM_Hide);
          } else {
            if (0) {
              Script_charaDisplay(SHEN_NAME('TAXI'), CDM_Show);
              goto L021C4A;
            }
          }
          L021C4A:;
          Script_scenToggleVisibleFor(SHEN_NAME('TAXI'), 0);
          script_2215B();
        }
      }
      if (local_F0 < 3) {
        if (local_F0 == 0) {
          if (((&local_F4)->hours >= 18) & ((&local_F4)->minutes >= 30)) {
            Script_mapmSetRenderFlag(3, 1);
            Script_mapmSetRenderFlag(2, 0);
            local_F0 = 1;
          }
        }
        if (local_F0 == 1) {
          if (((&local_F4)->hours >= 19) & ((&local_F4)->minutes >= 0)) {
            Script_mapmSetRenderFlag(5, 1);
            Script_mapmSetRenderFlag(4, 0);
            local_F0 = 2;
          }
        }
        if (local_F0 == 2) {
          if (((&local_F4)->hours >= 19) & ((&local_F4)->minutes >= 30)) {
            Script_mapmSetRenderFlag(7, 1);
            Script_mapmSetRenderFlag(6, 0);
            local_F0 = 3;
          }
        }
        if ((&local_F4)->hours >= 20) {
          Script_mapmSetRenderFlag(3, 1);
          Script_mapmSetRenderFlag(5, 1);
          Script_mapmSetRenderFlag(7, 1);
          Script_mapmSetRenderFlag(2, 0);
          Script_mapmSetRenderFlag(4, 0);
          Script_mapmSetRenderFlag(6, 0);
          local_F0 = 3;
        }
      }
      if (local_F0 == 3) {
        if (((&local_F4)->hours >= 0) & ((&local_F4)->hours <= 4)) {
          Script_mapmSetRenderFlag(3, 0);
          Script_mapmSetRenderFlag(5, 0);
          Script_mapmSetRenderFlag(7, 0);
          Script_mapmSetRenderFlag(2, 1);
          Script_mapmSetRenderFlag(4, 1);
          Script_mapmSetRenderFlag(6, 1);
          local_F0 = 4;
        }
      }
      local_F3 = (&local_F4)->minutes;
      local_F2 = (&local_F4)->hours;
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int8_t script_21DD1(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
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
  return local_E7;
}

static int32_t script_21E8A(void) {
  SCNF_WaitFrames_0000(1);
  Script_filmCheckOrSetTransitionUtil(1, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  return Script_srchControl(0);
}

__attribute__((scn_string_bindings(0, data_2E3BF, 1, data_2E3CF, 2, data_2E3D4, 3, data_2E3D9, 4, data_2E3DA, 5, data_2E3DF, 6, data_2E3E0)))
static int32_t script_21EC6(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  for (;;) {
    if (Script_ryoControl(PCCTL_QueryShouldRun) & -(Script_scenUtils(SCENARIO_CMD_GET_SCEN_FLAG100) == 0) & -(task_300 == 0) & -(task_32D == 0) & Script_IsPlayerAllowed(0)) {
      Script_ryoControl(PCCTL_DisablePlayerControl);
      Script_FaceEx(CHARA_AKIR, 1, 0, 2);
      Script_ResetHandMotion(CHARA_AKIR, -1, 0, 0);
      Script_ResetHandMotion(CHARA_AKIR, -1, 1, 0);
      Script_shenClearFreeState(0);
      Script_scenUtils(SCENARIO_CMD_INIT_SCNT);
      script_261F7();
      SCNF_WaitFrames_0000(1);
      Script_EVT_GetPosition(CHARA_AKIR, -1, &local_F4, 0);
      Script_mtwkGetSetRotation(CHARA_AKIR, &local_E8, 0);
      Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
      while (Script_filmCheckOrSetTransitionUtil(-1)) {
        SCNF_WaitFrames_0000(1);
      }
      Script_charaDisplay(SHEN_NAME('TAXI'), CDM_Hide);
      if (-1) {
        Script_charaDisplay(SHEN_NAME('TAXI'), CDM_Hide);
      } else {
        if (0) {
          Script_charaDisplay(SHEN_NAME('TAXI'), CDM_Show);
          goto L021FBE;
        }
      }
      L021FBE:;
      Script_scenToggleVisibleFor(SHEN_NAME('TAXI'), 0);
      Script_SetupLightFromTable(1);
      while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2)) {
        SCNF_WaitFrames_0000(1);
      }
      while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, "/scene/02/JD00/", "JDYA") == 0) {
        SCNF_WaitFrames_0000(1);
      }
      while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2)) {
        SCNF_WaitFrames_0000(1);
      }
      Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
      Script_AuthorUtils(0, 0, "JDYA", "");
      Script_AuthorUtils(0, 1, "JDYA", "");
      Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
      Script_scenCreateTask(script_21E8A, 0);
      script_FF30(4);
      Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &local_E8, 0);
      Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &local_F4);
      Script_convSetCamInactiveFlag(1);
      Script_SetGameStateTo(AUTH_None);
      Script_lightResetSlotToDefaults(1);
      script_2215B();
      script_2620A();
      Script_scenUtils(SCENARIO_CMD_UPDATE_TIMER);
      script_261BC();
      Script_AuthorUtils(AUTH_FREE, 0);
      Script_AuthorUtils(AUTH_FREE, 1);
      Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "JDYA");
      Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
      while (Script_filmCheckOrSetTransitionUtil(-1)) {
        SCNF_WaitFrames_0000(1);
      }
      Script_ryoControl(RYO_RestoreControl);
      Script_FaceEx(CHARA_AKIR, 0, 0, 2);
      Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
      Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
      Script_shenClearFreeState(1);
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  return Script_srchControl(0);
}

static void script_2215B(void) {
  Script_SetupLightFromTable(0);
  Script_PREP_Related(-1, 1);
  Script_SetCharaFlags(CHARA_AKIR, 0);
  return;
}

static void script_2217A(void) {
  Script_scenSetMapEvTbl((uint32_t *)&data_2E3E8, 4);
  return;
}

static void script_22188(void) {
  return;
}

static void script_2218B(int32_t arg0) {
  if (arg0 == 0x52494B41) {
    task_2DE = 3;
    Script_ryoSleepUtils(3, 1, 0, 3);
  }
  return;
}

static void script_221AE(int32_t arg0) {
  if (arg0 == 0x52494B41) {
    task_2DE = 1;
    Script_ryoSleepUtils(3, 1, 0, 1);
  }
  return;
}

static void script_221D1(int32_t arg0) {
  if (arg0 == 0x52494B41) {
    task_2DE = 2;
    Script_ryoSleepUtils(3, 1, 0, 2);
  }
  return;
}

static void script_221F4(void) {
  int32_t unused_FC;
  if (script_21DD1(9, 0, 19, 0)) {
    if (Script_ScenDataUtil(task_E8, 4, 1) & 2) {
      script_E175(0);
      script_E3EE(0);
    }
  } else {
    if ((Script_ScenDataUtil(task_E8, 4, 1) & 2) == 0) {
      script_E13E(0);
      script_E437(0);
    }
  }
  return;
}

static int32_t script_2225B(void) {
  float local_FC;
  int32_t local_F8;
  local_FC = Script_SetCameraSettings(CAM_UTILS_SET_DISTANCE_FROM_PLAYER, -1.0f);
  while (1) {
    local_F8 = *&task_84;
    if ((local_F8 == 11) | (local_F8 == 13)) {
      while (task_B6 != 1) {
        SCNF_WaitFrames_0000(1);
      }
      Script_SetCameraSettings(CAM_UTILS_SET_DISTANCE_FROM_PLAYER, 1.5f);
      task_A4 = 1.5f;
      task_B0 = *(int8_t *)&*(int32_t *)&task_B0 | 1;
      for (;;) {
        if (!*(int8_t *)&*(int32_t *)&task_B0) {
          break;
        }
        SCNF_WaitFrames_0000(1);
      }
    } else {
      if ((local_F8 == 12) | (local_F8 == 14)) {
        while (task_B6 != 1) {
          SCNF_WaitFrames_0000(1);
        }
        task_A4 = local_FC;
        task_B0 = *(int8_t *)&*(int32_t *)&task_B0 | 1;
        for (;;) {
          if (!*(int8_t *)&*(int32_t *)&task_B0) {
            break;
          }
          SCNF_WaitFrames_0000(1);
        }
        goto L022323;
      }
    }
    L022323:;
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_2232F(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  local_FC = arg3;
  if (local_FC == 1) {
    task_7C = 0;
    goto L02234A;
  }
  L02234A:;
  Script_ryoControl(PCCTL_RequestNewMode, 3);
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_shenClearFreeState(0);
  Script_executeAicaCommand(2720, 1, 30);
  Script_executeAicaCommand(2720, 2, 30);
  Script_executeAicaCommand(2720, 3, 30);
  Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_ryoControl(PCCTL_RequestNewMode, 0);
  Script_shenRequestGameModuleTransition(arg0, arg1, arg2);
  SCNF_WaitFrames_0000(1);
  return;
}

static int32_t script_223C6(void) {
  int32_t local_FC;
  local_FC = 0;
  if ((&task_F8)->mode) {
    if (task_F8.entry_idx == 0) {
      local_FC = 2;
    }
  }
  return local_FC;
}

static int32_t script_223EE(void) {
  int32_t local_FC;
  local_FC = 0;
  if (script_1100D()) {
    local_FC = 3;
  }
  return local_FC;
}

static int8_t script_22408(void) {
  int8_t local_FF;
  local_FF = 0;
  task_288.x = 21.4f;
  task_288.y = 0.0f;
  task_288.z = -8.46f;
  if (task_2D4 == 1) {
    if (!(Script_MathUtils(SCRIPT_MATH_TASK_VEC3_DISTANCE, &task_2B8, &task_288) <= 10.0f)) {
      task_2D4 = 0;
      Script_ryoSleepUtils(3, 1, 1, 0);
    }
  }
  if ((Script_CycleManagerUtils(CYCLE_CMD_COUNT_CYCLES_IN_RADIUS, &task_288, 10.0f) == 0) & (task_2D4 == 0)) {
    task_288.x = 19.213699f;
    task_288.y = 0.0f;
    task_288.z = -7.6756f;
    task_294.x = 27.6138f;
    task_294.y = 0.0f;
    task_294.z = -10.7757f;
    if (Script_SetTaskDataValue(CHARA_AKIR, &task_288, &task_294, 0)) {
      task_2D4 = 1;
      Script_ryoSleepUtils(3, 1, 1, 1);
      local_FF = 6;
    }
  }
  return local_FF;
}

static int32_t script_224F8(void) {
  int32_t local_FC;
  local_FC = 0;
  task_288.x = 14.0555f;
  task_288.y = 0.0f;
  task_288.z = 18.608601f;
  if (Script_CheckTargetWithinAngleRange(&task_288, 1.8f, 0x4000, 12743, &task_2B8) & -(task_2C4.y < 63715) & -(task_2C4.y > 34588)) {
    if (*(int8_t *)&*(int32_t *)&task_2DC == 0) {
      Script_sicnUtils(0, 5, 1);
      *(int8_t *)&task_2DC = 1;
      Script_uiUtils(0, 25, 0);
    }
    if (Script_GetControllerInput(1) == 0x400) {
      local_FC = 7;
    }
  } else {
    if (*(int8_t *)&*(int32_t *)&task_2DC) {
      Script_sicnUtils(1, 0, 0);
      SCNF_RequestIconNotification(0, 0, 0, 2);
      *(int8_t *)&task_2DC = 0;
      Script_uiUtils(0, 0, 0);
    }
  }
  return local_FC;
}

static int32_t script_225CE(void) {
  int32_t local_FC;
  local_FC = 0;
  task_288.x = 15.0673f;
  task_288.y = 0.0f;
  task_288.z = 2.0482f;
  task_294.x = (&task_2B8)->x + Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, task_2C4.y) * 0.3f;
  task_294.y = task_2B8.y;
  task_294.z = task_2B8.z + Script_MathUtils(SCRIPT_MATH_COS_FIXED16, task_2C4.y) * 0.3f;
  if (Script_CheckTargetWithinAngleRange(&task_294, 1.4f, task_2C4.y, 7281, &task_288)) {
    if (*((int8_t *)&*(int32_t *)&task_2DC + 1) == 0) {
      Script_sicnUtils(0, 4, 1);
      *((int8_t *)&*(int32_t *)&task_2DC + 1) = 1;
    }
    if (Script_GetControllerInput(1) == 0x400) {
      local_FC = 5;
      if (*((int8_t *)&*(int32_t *)&task_2DC + 1)) {
        Script_sicnUtils(1, 0, 0);
        *((int8_t *)&*(int32_t *)&task_2DC + 1) = 0;
      }
    }
  } else {
    if (*((int8_t *)&*(int32_t *)&task_2DC + 1)) {
      Script_sicnUtils(1, 0, 0);
      *((int8_t *)&*(int32_t *)&task_2DC + 1) = 0;
    }
  }
  return local_FC;
}

static int32_t script_226E3(void) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t unused_F4;
  GameDateTime local_EC;
  local_F8 = 0;
  task_288.x = 14.78f;
  task_288.y = 0.0f;
  task_288.z = 7.57f;
  task_294.x = 8.73f;
  task_294.y = 0.0f;
  task_294.z = 0.64f;
  local_FC = Script_StepUtils(GET_STEP_LOWER, 0) + Script_StepUtils(GET_STEP_HIGHER, 0) * 100;
  if ((Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_SA1037_SEQUENCE_COMPLETE) == 0) & (Script_GetOwnedItemCount(4, 0) == 1)) {
    local_F8 = 0;
  } else {
    if (((local_FC >= 801) & (local_FC <= 1703)) | ((local_FC >= 1705) & (local_FC <= 2108))) {
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_FIND_TERRY) == 0) {
        if (Script_SetTaskDataValue(CHARA_AKIR, &task_288, &task_294, 0) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_NOZOMI_CRIED) == 0)) {
          Script_ReturnTimeDate(&local_EC);
          if (script_21DD1(19, 0, 23, 29) & ((-((&local_EC)->month == 12) & -((&local_EC)->day >= 26)) | -((&local_EC)->month < 5))) {
            local_F8 = 4;
          }
        }
      }
    }
  }
  return local_F8;
}

static void script_22806(void) {
  int32_t unused_FC;
  GameDateTime local_F4;
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_SECRET_OF_YIN_YANG) == 0) {
    if (((Script_StepUtils(GET_STEP_HIGHER, 0) == 19) & (Script_StepUtils(GET_STEP_LOWER, 0) == 1)) | ((Script_StepUtils(GET_STEP_HIGHER, 0) >= 16) & (Script_StepUtils(GET_STEP_HIGHER, 0) < 19))) {
      task_288.x = 16.0f;
      task_288.y = 0.0f;
      task_288.z = -2.0f;
      task_294.x = 27.0f;
      task_294.y = 0.0f;
      task_294.z = -18.0f;
      if (Script_SetTaskDataValue(CHARA_AKIR, &task_288, &task_294, 0)) {
        if (Script_scenUtils(SCENARIO_CMD_QUERY_ACTIVE_RUNTIME_OBJECT, 0)) {
          if (script_FF06() == 0x414D4159) {
            Script_ReturnTimeDate(&local_F4);
            if ((((&local_F4)->hours == 8) & ((&local_F4)->minutes >= 30)) | (((&local_F4)->hours > 8) & ((&local_F4)->hours < 19))) {
              script_23FAE();
            }
          }
        }
      }
    }
  }
  return;
}

static int32_t script_228FC(void) {
  int32_t local_FC;
  Vector3i local_F0;
  int32_t local_EC;
  local_FC = 0;
  Script_mtwkGetSetRotation(CHARA_AKIR, &local_F0, 0);
  task_288.x = -4.581f;
  task_288.y = 0.0f;
  task_288.z = 18.6539f;
  task_294.x = -5.681f;
  task_294.y = 0.0f;
  task_294.z = 22.6539f;
  task_2A0.x = -5.481f;
  task_2A0.y = 0.0f;
  task_2A0.z = 18.6539f;
  task_2AC.x = -10.081f;
  task_2AC.y = 0.0f;
  task_2AC.z = 20.0539f;
  if ((Script_SetTaskDataValue(CHARA_AKIR, &task_288, &task_294, 0) & -((&local_F0)->y > 0) & -((&local_F0)->y < 0x8000)) | (Script_SetTaskDataValue(CHARA_AKIR, &task_2A0, &task_2AC, 0) & -((&local_F0)->y > 0x4000) & -((&local_F0)->y < 0xC000))) {
    local_EC = Script_StepUtils(GET_STEP_HIGHER, 0) * 10 + Script_StepUtils(GET_STEP_LOWER, 0);
    if (-(local_EC >= 131) & -(local_EC <= 154) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JD00_TAIKE_EVENT_DONE) == 0) & script_21DD1(8, 30, 22, 59)) {
      local_FC = 9;
    }
  }
  return local_FC;
}

static void script_22A43(int32_t arg0) {
  int32_t local_FC;
  local_FC = arg0;
  if (local_FC == 2) {
    SCNF_WaitFrames_0000(30);
    Script_executeAicaCommand(2720, 1, 30);
    Script_executeAicaCommand(2720, 2, 30);
    Script_executeAicaCommand(2720, 3, 30);
    Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
    while (Script_filmCheckOrSetTransitionUtil(-1)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_shenRequestGameModuleTransition(2, SHEN_NAME('JABE'), 0);
    SCNF_WaitFrames_0000(1);
    goto L022AC0;
  }
  L022AC0:;
  return;
}

static int32_t script_22AC1(int32_t arg0, int32_t arg1) {
  if (arg0) {
    (void)arg0;
  } else {
    (void)arg1;
  }
  end:;
}

static int32_t script_22AD3(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  int32_t local_F8;
  local_FC = 0;
  local_F8 = script_22AC1(arg0, arg1);
  switch (local_F8) {
    case 2:
      local_FC = script_223C6();
      break;
    case 6:
      local_FC = script_22408();
      break;
    case 4:
      local_FC = script_226E3();
      break;
    case 9:
      local_FC = script_228FC();
      break;
  }
  if (Script_IsPlayerAllowed(0)) {
    if (local_FC == 0) {
      local_FC = script_223EE();
    }
    if (local_FC == 0) {
      local_FC = script_224F8();
    }
    if (local_FC == 0) {
      local_FC = script_225CE();
    }
  }
  return local_FC;
}

static void script_22B8A(void) {
  int32_t local_FC;
  local_FC = Script_UpdateTaskCollisionCache(0, 1, &task_2B8);
  if (local_FC != -1) {
    Script_PlayerStatsUtils(PS_IncrementSpotVisitCount, local_FC);
  }
  return;
}

static int32_t script_22BB0(void) {
  int8_t local_FF;
  int8_t local_FE;
  task_2D4 = 0;
  task_2DE = 0;
  local_FF = 0;
  for (;;) {
    if (local_FF == 2) {
      break;
    }
    *((int8_t *)&*(int32_t *)&task_2DC + local_FF) = 0;
    local_FF = local_FF + 1;
  }
  local_FF = local_FE = 1;
  for (;;) {
    Script_EVT_GetPosition(CHARA_AKIR, -1, &task_2B8, ENTITY_XFORM_WORLD_SPACE);
    Script_mtwkGetSetRotation(CHARA_AKIR, &task_2C4, 0x42000000);
    script_22B8A();
    if (Script_IsPlayerAllowed(0)) {
      uint8_t unused_FC[2];
      int32_t local_F8;
      local_F8 = task_2DE;
      switch (local_F8) {
        case 3:
          script_2232F(2, SHEN_NAME('JU00'), 0, 0);
          break;
        case 1:
          script_2232F(2, SHEN_NAME('D000'), 1, 0);
          break;
        case 2:
          script_2232F(2, SHEN_NAME('D000'), 11, 0);
          break;
      }
    }
    if ((local_FE == 2) | Script_IsPlayerAllowed(0)) {
      local_FF = script_22AD3(local_FF, local_FE);
    } else {
      local_FF = 0;
    }
    local_FE = local_FE + 1;
    if (local_FE == 10) {
      local_FE = 2;
    }
    if ((local_FE == 3) | (local_FE == 7) | (local_FE == 5)) {
      local_FE = local_FE + 1;
      if (local_FE == 10) {
        local_FE = 2;
      }
    }
    {
      uint8_t unused_FC[2];
      int32_t local_F8;
      local_F8 = local_FF;
      switch (local_F8) {
        case 2:
          script_22A43(local_FF);
          break;
        case 6:
          script_148BB();
          break;
        case 7:
          if (script_3F53(100, 1)) {
            script_48F0(CHARA_AKIR, SHEN_NAME('VM_0'), SHEN_NAME('JIHN'), SHEN_NAME('HAND'));
          }
          if (*(int8_t *)&*(int32_t *)&task_2DC) {
            Script_sicnUtils(1, 0, 0);
            *(int8_t *)&task_2DC = 0;
            Script_uiUtils(0, 0, 0);
          }
          break;
        case 3:
          script_1314A();
          break;
        case 5:
          script_13DE7(0);
          break;
        case 4:
          script_23286(1);
          break;
        case 9:
          script_2439B();
          break;
      }
    }
    if (Script_ryoControl(PCCTL_QueryShouldRun)) {
      script_221F4();
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_22DE1(int32_t arg0) {
  Script_ResetHandMotion(arg0, 1, 1, 0);
  Script_ResetHandMotion(arg0, 1, 0, 0);
  Script_handSetFloatsFor(arg0, 1, data_26C3C, 8);
  Script_handSetFloatsFor(arg0, 0, data_26C3C, 8);
  return;
}

static void script_22E1C(int32_t arg0) {
  Script_ResetHandMotion(arg0, 0, 1, 0);
  Script_ResetHandMotion(arg0, 0, 0, 0);
  Script_CharHandlers(arg0, 2, 8, 16);
  return;
}

static void script_22E46(int32_t arg0) {
  int32_t local_FC;
  int32_t unused_F0[3];
  int32_t local_EC;
  Script_executeAicaCommand(19112, 0, 0);
  Script_executeAicaCommand(1184, 2, 100);
  script_387C();
  script_2308C();
  local_FC = 0;
  Script_shenSeqUtils(arg0);
  while (Script_shenSeqUtils(-1)) {
    local_EC = local_FC;
    switch (local_EC) {
      case 0:
        Script_FACE(CHARA_HRSK, 3, 0, 16);
        break;
      case 400:
        Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 25.0f);
        break;
      case 950:
        Script_FACE(CHARA_HRSK, 0, 0, 16);
        break;
      case 1472:
        Script_FACE(CHARA_HRSK, 2, 0, 16);
        break;
      case 1560:
        Script_FACE(CHARA_HRSK, 3, 0, 16);
        break;
    }
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_22F23(int32_t arg0) {
  int32_t local_FC;
  Vector3 local_F0;
  int32_t local_EC;
  if (Script_shenSeqUtils(-7) == 0) {
    script_387C();
    script_2308C();
    local_FC = 0;
    Script_shenSeqUtils(arg0);
    while (Script_shenSeqUtils(-1)) {
      local_EC = local_FC;
      switch (local_EC) {
        case 0:
          Script_FACE(CHARA_HRSK, 3, 0, 16);
          break;
        case 143:
          script_22E1C(CHARA_AKIR);
          script_22E1C(CHARA_HRSK);
          break;
        case 570:
          Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 30.0f);
          Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 20.0f);
          Script_scenToggleActiveFor(CHARA_AKIR, 0);
          Script_scenToggleActiveFor(CHARA_HRSK, 0);
          break;
        case 1000:
          Script_FACE_LCG(CHARA_HRSK, 1, 16, 0);
          break;
        case 1080:
          Script_FACE(CHARA_HRSK, 0, 0, 16);
          break;
        case 1162:
          Script_FaceLookAtTargetUtils(CHARA_AKIR, 0);
          break;
        case 1500:
          goto L02307B;
          break;
      }
      if ((local_FC >= 896) & (local_FC <= 1161)) {
        Script_EVT_GetPosition(CHARA_HRSK, 18, &local_F0, 0);
        Script_FaceLookAtTargetUtils(CHARA_AKIR, 2, &local_F0, 8);
      }
      local_FC = local_FC + 1;
      SCNF_WaitFrames_0000(1);
    }
    L02307B:;
    Script_FACE_LCG(CHARA_HRSK, 0, 0, 0);
  }
  return;
}

static void script_2308C(void) {
  script_3894(CHARA_AKIR);
  script_3894(CHARA_HRSK);
  Script_SetMaxInterpSpeed(0.4f);
  Script_SetCharaRefBoardPos(CHARA_AKIR, &data_2E3F8);
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 1);
  Script_SetCharaRefBoardPos(CHARA_HRSK, &data_2E408);
  Script_charaSetRefboardFlagTo(CHARA_HRSK, 1);
  Script_facemotUtils(CHARA_AKIR, 1, 0);
  Script_facemotUtils(CHARA_HRSK, 1, 0);
  script_22DE1(CHARA_AKIR);
  script_22DE1(CHARA_HRSK);
  Script_SetMotnFlag(CHARA_AKIR, -1);
  Script_SetMotnFlag(CHARA_HRSK, -1);
  Script_SetShadowFlags(CHARA_AKIR, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_SetShadowFlags(CHARA_HRSK, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_SetCharaFlags(CHARA_AKIR, 4);
  Script_SetCharaFlags(CHARA_HRSK, 4);
  Script_SetShadowFlags(CHARA_AKIR, SCRIPT_SHADOW_UPDATE_LIGHT_MASK, 1);
  Script_SetShadowFlags(CHARA_HRSK, SCRIPT_SHADOW_UPDATE_LIGHT_MASK, 1);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 60.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 45.0f);
  return;
}

static void script_23197(void) {
  script_23220(CHARA_AKIR);
  Script_SetCharaFlags(CHARA_HRSK, 0);
  script_3921(CHARA_AKIR);
  script_3921(CHARA_HRSK);
  Script_FaceEx(CHARA_AKIR, 1, 0, 2);
  Script_SetCharaFlags(CHARA_AKIR, 4);
  return;
}

static void script_231E5(int32_t arg0) {
  Script_ResetHandMotion(arg0, 1, 1, 0);
  Script_ResetHandMotion(arg0, 1, 0, 0);
  Script_handSetFloatsFor(arg0, 1, data_26C3C, 8);
  Script_handSetFloatsFor(arg0, 0, data_26C3C, 8);
  return;
}

static void script_23220(int32_t arg0) {
  Script_ResetHandMotion(arg0, 0, 1, 0);
  Script_ResetHandMotion(arg0, 0, 0, 0);
  Script_CharHandlers(arg0, 2, 8, 16);
  return;
}

static int32_t script_2324A(void) {
  SCNF_WaitFrames_0000(1);
  Script_filmCheckOrSetTransitionUtil(15, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  return Script_srchControl(0);
}

__attribute__((scn_string_bindings(0, data_2E418, 1, data_2E428, 2, data_2E42D, 3, data_2E432, 4, data_2E433, 5, data_2E438, 7, data_2E440, 8, data_2E445, 9, data_2E44A, 10, data_2E44F)))
static void script_23286(int32_t arg0) {
  int32_t local_FC;
  int32_t unused_F8;
  GameDateTime local_F0;
  local_FC = 0;
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_FaceEx(CHARA_AKIR, 1, 0, 2);
  Script_ResetHandMotion(CHARA_AKIR, -1, 0, 0);
  Script_ResetHandMotion(CHARA_AKIR, -1, 1, 0);
  Script_shenClearFreeState(0);
  Script_WeatherUtils_0(WeatherDataUtil_SetInterpolatedWeatherOverride, 1, 0, 400, 0);
  if (arg0) {
    Script_filmCheckOrSetTransitionUtil(10, 0, 0, 0, 0, 255, 0, 0, 0);
    while (Script_filmCheckOrSetTransitionUtil(-1)) {
      SCNF_WaitFrames_0000(1);
    }
  }
  Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_NOZOMI_CRIED, 1);
  Script_SetShouldSleepFlag(0);
  script_261F7();
  Script_scenUtils(SCENARIO_CMD_INIT_SCNT);
  Script_srchControl((void *)task_2D0);
  Script_srchControl((void *)task_328);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('YAMN'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('YAMN'), CDM_Show);
      goto L02335D;
    }
  }
  L02335D:;
  Script_scenToggleVisibleFor(SHEN_NAME('YAMN'), 0);
  Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT, 0x4B535248, 1);
  local_FC = Script_CycleManagerUtils(CYCLE_CMD_GET_CYCLE_PTR_BY_CHARA_ID, 0x4B535248);
  while (local_FC == 0) {
    SCNF_WaitFrames_0000(1);
    local_FC = Script_CycleManagerUtils(CYCLE_CMD_GET_CYCLE_PTR_BY_CHARA_ID, 0x4B535248);
  }
  while (Script_CycleManagerUtils(CYCLE_CMD_GET_NEXT_TASK_POINTER, local_FC) == 0) {
    SCNF_WaitFrames_0000(1);
  }
  Script_motmGetCharaFlags(CHARA_HRSK, 0, 0);
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, "/scene/02/JD00/", "DNOZ") == 0) {
    SCNF_WaitFrames_0000(1);
  }
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
  Script_AuthorUtils(0, 0, "DNOZ", "");
  Script_AuthorUtils(0, 1, "DNOZ", "");
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
  script_235C1();
  Script_LoadVoiceFile(1, "01FULB");
  Script_LoadSND(0, 4, "FREE", 0);
  Script_LoadSND(0, 3, "FREE", 0);
  Script_LoadSND(0, 2, "FREE", 0);
  Script_LoadSND(0, 1, "FREE", 0);
  Script_LoadSND(0, 2, "bgm068.snd", 0);
  Script_LoadSND(0, 4, "a1_nonam.snd", 0);
  Script_executeAicaCommand(19112, 0, 0);
  Script_executeAicaCommand(1184, 2, 100);
  if (arg0 == 0) {
    Script_cdttLoadScrUtils(1, 0);
  }
  Script_scenCreateTask(script_2324A, 0);
  script_22E46(0);
  script_22F23(1);
  Script_executeAicaCommand(2720, 1, 30);
  Script_executeAicaCommand(2720, 2, 30);
  Script_executeAicaCommand(2720, 3, 30);
  Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  SCNF_WaitFrames_0000(1);
  Script_ReturnTimeDate(&local_F0);
  if ((((&local_F0)->hours < 23) & ((&local_F0)->hours > 5)) | (((&local_F0)->hours == 23) & ((&local_F0)->minutes < 30))) {
    (&local_F0)->hours = 23;
    (&local_F0)->minutes = 30;
    (&local_F0)->seconds = 0;
    Script_SetNewDateTime(&local_F0);
  }
  Script_shenRequestGameModuleTransition(2, SHEN_NAME('JOMO'), 3);
  SCNF_WaitFrames_0000(1);
  return;
}

static void script_235C1(void) {
  Vector3 local_F4;
  Script_PREP_Related(-1, 0);
  Script_WeatherTaskUtil(ToggleSceneLightingFlags, 0);
  Script_WeatherTaskUtil(ToggleFogTask, 0);
  Script_WeatherTaskUtil(AndNotInstructionFlags, 0xC);
  Script_SetVector4ByOffset(&data_2E4B4);
  Script_SetCharaRefBoardPos(CHARA_HRSK, &data_2E4C4);
  Script_charaSetRefboardFlagTo(CHARA_HRSK, 1);
  Script_SetFogColour(&data_2E46C);
  Script_SetFogFlags(0, data_2E47C);
  Script_shenSetLightingMode(2);
  Script_lightSetGlobalSceneLighting(&data_2E48C);
  local_F4.x = 20.61f;
  (&local_F4)->y = 3.39f;
  (&local_F4)->z = -4.08f;
  Script_lightEnablePointLightAt(1, &local_F4);
  Script_LightUtils(1, 2, &data_2E49C);
  Script_LightUtils(1, 4, 1.0f);
  Script_LightUtils(1, 5, 3.69f);
  Script_LightUtils(1, 6, data_2E4AC);
  Script_lightSetSlotEnabled(1, 1);
  Script_mapmSetRenderFlag(0, 1);
  Script_mapmSetRenderFlag(1, 1);
  Script_mapmSetRenderFlag(2, 0);
  Script_mapmSetRenderFlag(3, 1);
  Script_mapmSetRenderFlag(4, 0);
  Script_mapmSetRenderFlag(5, 1);
  Script_mapmSetRenderFlag(6, 0);
  Script_mapmSetRenderFlag(7, 0);
  Script_mapmSetRenderFlag(8, 1);
  Script_mapmSetRenderFlag(9, 1);
  Script_SpriteUtils(Scroll_Read1, (int32_t)"sprite", "SCROLL27.SPR");
  Script_mapmSetCamFlags(0, 0);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 25.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 25.0f);
  return;
}

static void script_2371F(int32_t arg0) {
  Script_ccowUtils(arg0, 2);
  Script_ccowSetFlag80(arg0, 1);
  Script_motmToggleFlags(arg0, 2, 0x40);
  Script_SetMotnFlag(arg0, -1);
  Script_SetCPAMFlags(arg0, 0);
  Script_SetCharaFlags(arg0, 4);
  Script_CharHandlers(arg0, 2, 8, 16);
  return;
}

static void script_23764(int32_t arg0) {
  Script_ccowUtils(arg0, 2);
  Script_ccowSetFlag80(arg0, 1);
  Script_motmToggleFlags(arg0, 2, 0x40);
  Script_SetMotnFlag(arg0, -1);
  Script_SetCPAMFlags(arg0, 0);
  Script_SetCharaFlags(arg0, 4);
  Script_motmGetCharaFlags(arg0, 0, 0);
  Script_facemotUtils(arg0, 0, 0);
  Script_facemotUtils(arg0, 1, 0);
  Script_FaceEx(arg0, 1, 0, 1);
  Script_ResetHandMotion(arg0, 1, 1, 0);
  Script_ResetHandMotion(arg0, 1, 0, 0);
  Script_handSetFloatsFor(arg0, 1, data_2E5CC, 1);
  Script_handSetFloatsFor(arg0, 0, data_2E5CC, 1);
  Script_FACE_LCG(arg0, 0, 6, 0);
  Script_FaceLookAtTargetUtils(arg0, 0);
  return;
}

static void script_23814(void) {
  Script_SetShadowFlags(0, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_SetShadowFlags(CHARA_AKIR, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_SetShadowFlags(CHARA_AKIR, SCRIPT_SHADOW_UPDATE_LIGHT_MASK, 1);
  Script_SetCharaRefBoardPos(CHARA_AKIR, &data_2EB24);
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 1);
  Script_SetShadowFlags(CHARA_YAMA, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_SetShadowFlags(CHARA_YAMA, SCRIPT_SHADOW_UPDATE_LIGHT_MASK, 1);
  Script_SetCharaRefBoardPos(CHARA_YAMA, &data_2EB24);
  Script_charaSetRefboardFlagTo(CHARA_YAMA, 1);
  Script_SetCharaRefBoardPos(SHEN_NAME('PNXM'), &data_2EB24);
  Script_charaSetRefboardFlagTo(SHEN_NAME('PNXM'), 1);
  Script_SetVector4ByOffset(&data_2EB24);
  (void)1;
  return;
}

static void script_238B8(void) {
  script_23764(CHARA_AKIR);
  script_23764(CHARA_YAMA);
  script_23814();
  task_3B0 = 0;
  return;
}

static void script_238DF(int32_t arg0) {
  Script_executeAicaCommand(17832, 0, 0);
  Script_executeAicaCommand(1184, 2, 105);
  script_238B8();
  Script_filmCheckOrSetTransitionUtil(20, 255, 0, 0, 0, 0, 0, 0, 0);
  SCNF_WaitFrames_0000(1);
  Script_shenSeqUtils(arg0);
  while (Script_shenSeqUtils(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_charaSetRefboardFlagTo(SHEN_NAME('PNXM'), 0);
  Script_set_val_to_1(2, 0);
  return;
}

static int32_t script_2394A(void) {
  Vector3 local_F4;
  for (;;) {
    {
      int32_t local_F0;
      local_F0 = task_3B0;
      switch (local_F0) {
        case 1:
          Script_FACE(CHARA_AKIR, 0, 0, 0);
          break;
        case 1929:
          Script_FACE(CHARA_AKIR, 2, 0, 30);
          break;
        case 2261:
          Script_FACE(CHARA_AKIR, 3, 0, 30);
          break;
        case 2453:
          Script_FACE(CHARA_AKIR, 0, 0, 30);
          break;
      }
    }
    if ((task_3B0 > 2661) & (task_3B0 < 2692)) {
      Script_EVT_GetPosition(SHEN_NAME('PNXM'), -1, &local_F4, 0);
      Script_FaceLookAtTargetUtils(CHARA_AKIR, 2, &local_F4, 8);
    } else {
      Script_EVT_GetPosition(CHARA_YAMA, 5, &local_F4, ENTITY_XFORM_WORLD_SPACE);
      Script_FaceLookAtTargetUtils(CHARA_AKIR, 2, &local_F4, 8);
    }
    {
      int32_t local_F0;
      local_F0 = task_3B0;
      switch (local_F0) {
        case 407:
          Script_ResetHandMotion(CHARA_AKIR, 1, 1, 0);
          Script_ResetHandMotion(CHARA_AKIR, 1, 0, 0);
          Script_handSetFloatsFor(CHARA_AKIR, 1, data_2E5CC, 0);
          Script_handSetFloatsFor(CHARA_AKIR, 0, data_2E5CC, 0);
          break;
        case 556:
          Script_charaDisplay(SHEN_NAME('PNXM'), CDM_Show);
          Script_fixoAttachEx(SHEN_NAME('PNXM'), CHARA_AKIR, 18, &data_2EB34, &data_2EB40);
          Script_handSetFloatsFor(CHARA_AKIR, 1, data_2EA40, 0);
          break;
        case 678:
          Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
          Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
          break;
        case 1591:
          Script_ResetHandMotion(CHARA_AKIR, 1, 1, 0);
          Script_ResetHandMotion(CHARA_AKIR, 1, 0, 0);
          Script_handSetFloatsFor(CHARA_AKIR, 1, data_2E5CC, 0);
          Script_handSetFloatsFor(CHARA_AKIR, 0, data_2E5CC, 0);
          break;
        case 1928:
          Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
          Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
          break;
        case 2080:
          Script_ResetHandMotion(CHARA_AKIR, 1, 1, 0);
          Script_ResetHandMotion(CHARA_AKIR, 1, 0, 0);
          Script_handSetFloatsFor(CHARA_AKIR, 1, data_2E5CC, 0);
          Script_handSetFloatsFor(CHARA_AKIR, 0, data_2E5CC, 0);
          break;
        case 2335:
          Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
          break;
        case 2336:
          Script_handSetFloatsFor(CHARA_AKIR, 1, data_2EA40, 8);
          Script_fixoAttachEx(SHEN_NAME('PNXM'), CHARA_AKIR, 18, &data_2EB34, &data_2EB40);
          break;
        case 2642:
          Script_charaDisplay(SHEN_NAME('PNXM'), CDM_Hide);
          Script_handSetFloatsFor(CHARA_AKIR, 1, data_2E5CC, 0);
          Script_ResetHandMotion(CHARA_AKIR, 1, 0, 0);
          Script_handSetFloatsFor(CHARA_AKIR, 0, data_2E5CC, 0);
          break;
        case 2743:
          break;
      }
    }
    {
      int32_t local_F0;
      local_F0 = task_3B0;
      switch (local_F0) {
        case 60:
          Script_ResetHandMotion(CHARA_YAMA, 1, 1, 0);
          Script_ResetHandMotion(CHARA_YAMA, 1, 0, 0);
          Script_SetHandRootRotation(CHARA_YAMA, 1, 0x15, &data_2EBB4);
          Script_handSetFloatsFor(CHARA_YAMA, 1, data_2E95C, 0);
          Script_handSetFloatsFor(CHARA_YAMA, 0, data_2E95C, 0);
          break;
        case 407:
          Script_ResetHandMotion(CHARA_YAMA, 0, 1, 0);
          Script_ResetHandMotion(CHARA_YAMA, 0, 0, 0);
          Script_ReadModelFormat7(CHARA_YAMA, 1, data_2EB64, 0);
          break;
        case 678:
          Script_fixoAttachEx(SHEN_NAME('PNXM'), CHARA_YAMA, 18, &data_2EB4C, &data_2EB58);
          Script_ResetHandMotion(CHARA_YAMA, 1, 1, 0);
          Script_ResetHandMotion(CHARA_YAMA, 1, 0, 0);
          Script_handSetFloatsFor(CHARA_YAMA, 1, data_2EA40, 8);
          Script_handSetFloatsFor(CHARA_YAMA, 0, data_2E95C, 0);
          break;
        case 838:
          Script_ResetHandMotion(CHARA_YAMA, 0, 1, 0);
          Script_CharHandlers(CHARA_YAMA, 0, 11, 16);
          Script_ResetHandMotion(CHARA_YAMA, 0, 0, 0);
          Script_ReadModelFormat7(CHARA_YAMA, 1, data_2EB8C, 0);
          break;
        case 2613:
          Script_ResetHandMotion(CHARA_YAMA, 1, 1, 0);
          Script_ResetHandMotion(CHARA_YAMA, 1, 0, 0);
          Script_handSetFloatsFor(CHARA_YAMA, 1, data_2E95C, 0);
          Script_handSetFloatsFor(CHARA_YAMA, 0, data_2E95C, 0);
          break;
        case 2400:
          Script_ResetHandMotion(CHARA_YAMA, 0, 1, 0);
          Script_ResetHandMotion(CHARA_YAMA, 0, 0, 0);
          break;
        case 2740:
          Script_filmCheckOrSetTransitionUtil(20, 0, 0, 0, 0, 255, 0, 0, 0);
          break;
      }
    }
    {
      int32_t local_F0;
      local_F0 = task_3B0;
      switch (local_F0) {
        case 1:
          Script_FACE(CHARA_YAMA, 10, 0, 0);
          Script_FACE_LCG(CHARA_YAMA, 1, 0, 0);
          break;
        case 60:
          Script_FACE_LCG(CHARA_YAMA, 0, 0, 0);
          break;
        case 100:
          Script_FACE(CHARA_YAMA, 1, 0, 40);
          break;
        case 678:
          Script_FACE(CHARA_YAMA, 7, 0, 0);
          break;
        case 1592:
          Script_FACE(CHARA_YAMA, 0, 0, 0);
          break;
        case 1795:
          Script_FACE(CHARA_YAMA, 2, 0, 30);
          break;
      }
    }
    task_3B0 = task_3B0 + 1;
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_23F47(int32_t arg0) {
  Script_ccowUtils(arg0, 1);
  Script_ccowSetFlag80(arg0, 2);
  Script_motmToggleFlags(arg0, 1, 0x40);
  Script_FaceEx(arg0, 0, 0, 2);
  Script_FaceLookAtTargetUtils(arg0, 0);
  Script_CharHandlers(arg0, -1, 0, 0);
  Script_SetCPAMFlags(arg0, 1);
  Script_PlayCharaMotion(arg0, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  return;
}

__attribute__((scn_string_bindings(0, data_2EBC0, 1, data_2EBD0, 2, data_2EBD5, 3, data_2EBDA, 5, data_2EBE1, 6, data_2EBE6, 7, data_2EBEB, 8, data_2EBF0, 11, data_2EC13, 12, data_2EC18, 13, data_2EC1D, 14, data_2EC22, 15, data_2EC2D, 16, data_2EC34, 17, data_2EC41)))
static void script_23FAE(void) {
  int32_t local_FC;
  int32_t local_F8;
  local_FC = 0;
  Script_WeatherUtils_0(WeatherDataUtil_SetInterpolatedWeatherOverride, 1, 0, 400, 0);
  Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_SECRET_OF_YIN_YANG, 1);
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_FaceEx(CHARA_AKIR, 1, 0, 2);
  Script_ResetHandMotion(CHARA_AKIR, -1, 0, 0);
  Script_ResetHandMotion(CHARA_AKIR, -1, 1, 0);
  Script_shenClearFreeState(0);
  Script_scenUtils(SCENARIO_CMD_INIT_SCNT);
  Script_filmCheckOrSetTransitionUtil(20, 0, 0, 0, 0, 255, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  script_261F7();
  Script_SetCharaLookAtPosition(CHARA_AKIR, -20, 0, 0);
  Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT, 0x414D4159, 1);
  local_FC = Script_CycleManagerUtils(CYCLE_CMD_GET_CYCLE_PTR_BY_CHARA_ID, 0x414D4159);
  while (local_FC == 0) {
    SCNF_WaitFrames_0000(1);
    local_FC = Script_CycleManagerUtils(CYCLE_CMD_GET_CYCLE_PTR_BY_CHARA_ID, 0x414D4159);
  }
  while (Script_CycleManagerUtils(CYCLE_CMD_GET_NEXT_TASK_POINTER, local_FC) == 0) {
    SCNF_WaitFrames_0000(1);
  }
  Script_motmGetCharaFlags(CHARA_YAMA, 0, 0);
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, "/scene/02/JD00/", "INYO") == 0) {
    SCNF_WaitFrames_0000(1);
  }
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
  Script_AuthorUtils(0, 0, "INYO", "");
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
  Script_LoadVoiceFile(1, "01INY");
  Script_LoadSND(0, 4, "FREE", 0);
  Script_LoadSND(0, 2, "FREE", 0);
  Script_LoadSND(0, 1, "FREE", 0);
  Script_LoadSND(0, 1, "amb019.snd", 0);
  Script_LoadSND(0, 2, "bgm052.snd", 0);
  Script_LoadSND(0, 4, "a1_inyou.snd", 0);
  Script_executeAicaCommand(17832, 0, 0);
  Script_executeAicaCommand(1184, 2, 110);
  script_26216();
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 50.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 50.0f);
  local_F8 = Script_scenCreateTask(script_2394A, 0);
  script_238DF(0);
  Script_executeAicaCommand(2720, 1, 30);
  Script_executeAicaCommand(2720, 2, 30);
  Script_executeAicaCommand(2720, 3, 30);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  script_23F47(CHARA_AKIR);
  Script_srchControl((void *)local_F8);
  if (local_FC) {
    Script_CycleManagerUtils(CYCLE_CMD_LOAD_OR_RESET_RUNNING_PROGRAM, local_FC);
  }
  Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT, 0x414D4159, 0);
  Script_LoadSND(0, 4, "FREE", 0);
  Script_LoadSND(0, 2, "FREE", 0);
  Script_LoadSND(0, 1, "FREE", 0);
  Script_LoadSND(0, 1, "amb019.snd", 0);
  Script_LoadSND(0, 2, "SELECT", 0);
  Script_LoadSND(0, 4, "f1jutadn.snd", 0);
  Script_executeAicaCommand(16296, 0, 0);
  Script_executeAicaCommand(1184, 2, 100);
  script_26216();
  Script_AuthorUtils(AUTH_FREE, 0);
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "INYO");
  script_2620A();
  Script_scenUtils(SCENARIO_CMD_UPDATE_TIMER);
  Script_convSetCamInactiveFlag(1);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 65.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 17.0f);
  Script_SetGameStateTo(AUTH_None);
  Script_CycleManagerUtils(CYCLE_CMD_RESET_LOAD_SETTLE);
  for (;;) {
    if (Script_CycleManagerUtils(CYCLE_CMD_TICK_LOAD_SETTLE_COUNT) == 0) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_filmCheckOrSetTransitionUtil(20, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_WeatherUtils_0(WeatherDataUtil_SetInterpolatedWeatherOverride, 1, 1, 400, 0);
  Script_ryoControl(RYO_RestoreControl);
  Script_FaceEx(CHARA_AKIR, 0, 0, 2);
  Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
  Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
  Script_shenClearFreeState(1);
  return;
}

__attribute__((scn_string_bindings(0, data_2EE20, 1, data_2EE25, 2, data_2EE2A, 3, data_2EE2F, 6, data_2EE4C, 7, data_2EE5C, 8, data_2EE61, 9, data_2EE67, 10, data_2EE6C, 11, data_2EE6D, 12, data_2EE72, 13, data_2EE73, 14, data_2EE78, 15, data_2EE7D, 16, data_2EE82, 21, data_2EEB7, 22, data_2EEBC, 23, data_2EEC1, 24, data_2EEC6, 27, data_2EEE3, 28, data_2EEE8, 29, data_2EEED, 30, data_2EEF2, 31, data_2EEF7, 32, data_2EEFE, 33, data_2EF0B)))
static void script_2439B(void) {
  int32_t unused_E8[6];
  int8_t local_E7;
  uint8_t unused_E4[3];
  int32_t local_E0;
  int32_t local_DC;
  int32_t local_D8;
  int32_t local_D4;
  Script_scenUtils(SCENARIO_CMD_INIT_SCNT);
  Script_SetActionSelectorHUDElementVisibility(1);
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_FaceEx(CHARA_AKIR, 1, 0, 2);
  Script_ResetHandMotion(CHARA_AKIR, -1, 0, 0);
  Script_ResetHandMotion(CHARA_AKIR, -1, 1, 0);
  Script_shenClearFreeState(0);
  Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_JD00_TAIKE_EVENT_DONE, 1);
  Script_filmCheckOrSetTransitionUtil(60, 0, 0, 0, 0, 255, 0, 0, 0);
  Script_scenCreateTask(script_24E45, 0);
  Script_executeAicaCommand(2720, 2, 64);
  SCNF_WaitFrames_0000(60);
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
  Script_CycleManagerUtils(CYCLE_CMD_PROCESS_SND_EXTRA);
  Script_LoadSND(0, 5, "FREE", 0);
  Script_LoadSND(0, 4, "FREE", 0);
  Script_LoadSND(0, 3, "FREE", 0);
  Script_LoadSND(0, 2, "FREE", 0);
  Script_LoadSND(0, 2, "bgm116.snd", 0);
  Script_executeAicaCommand(16296, 0, 0);
  Script_executeAicaCommand(1184, 2, 86);
  Script_LoadSND(0, 4, "a1_taike.snd", 0);
  Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT, 0x5953494B, 1);
  local_E0 = Script_CycleManagerUtils(CYCLE_CMD_GET_CYCLE_PTR_BY_CHARA_ID, 0x5953494B);
  while (local_E0 == 0) {
    SCNF_WaitFrames_0000(1);
    local_E0 = Script_CycleManagerUtils(CYCLE_CMD_GET_CYCLE_PTR_BY_CHARA_ID, 0x5953494B);
  }
  while (Script_CycleManagerUtils(CYCLE_CMD_GET_NEXT_TASK_POINTER, local_E0) == 0) {
    SCNF_WaitFrames_0000(1);
  }
  Script_motmGetCharaFlags(CHARA_KISY, 0, 0);
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 10, -1);
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, "/scene/02/JD00/", "KSBT") == 0) {
    SCNF_WaitFrames_0000(1);
  }
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 10, 0);
  script_24FC4();
  Script_LoadVoiceFile(1, "A0167");
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
  Script_AuthorUtils(0, 0, "KSBT", "");
  Script_AuthorUtils(0, 1, "KSBT", "");
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
  SCNF_WaitFrames_0000(1);
  Script_executeAicaCommand(2720, 1, 64);
  Script_charaDisplay(CHARA_AKIR, CDM_Show);
  local_E7 = 0;
  Script_SetCPAMFlags(CHARA_KISY, 0);
  Script_scenToggleVisibleFor(CHARA_KISY, 1);
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 1);
  Script_SetCharaRefBoardPos(CHARA_AKIR, &data_2EE10);
  Script_charaSetRefboardFlagTo(CHARA_KISY, 1);
  Script_SetCharaRefBoardPos(CHARA_KISY, &data_2EE10);
  Script_filmCheckOrSetTransitionUtil(60, 255, 0, 0, 0, 0, 0, 0, 0);
  script_24F4A(0);
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 1);
  Script_SetCharaRefBoardPos(CHARA_AKIR, &data_2EE10);
  Script_charaSetRefboardFlagTo(CHARA_KISY, 1);
  Script_SetCharaRefBoardPos(CHARA_KISY, &data_2EE10);
  Script_executeAicaCommand(2720, 2, 30);
  Script_SetGameStateTo(AUTH_None);
  Script_SetCPAMFlags(CHARA_KISY, 1);
  Script_ryoControl(RYO_RestoreControl);
  Script_FaceEx(CHARA_AKIR, 0, 0, 2);
  Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
  Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
  Script_shenClearFreeState(1);
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_LoadSND(0, 5, "FREE", 0);
  Script_LoadSND(0, 4, "FREE", 0);
  Script_LoadSND(0, 3, "FREE", 0);
  Script_LoadSND(0, 2, "FREE", 0);
  Script_LoadSND(0, 2, "bgm013.snd", 0);
  Script_executeAicaCommand(9640, 0, 0);
  Script_executeAicaCommand(1184, 2, 104);
  Script_LoadSND(0, 4, "e1kishou.snd", 0);
  Script_LoadSND(0, 5, "battle_1.snd", 0);
  local_D8 = Script_motiLoad(2, SHEN_NAME('JD00'), "M_FGT1.BIN");
  Script_vfSetup(0, 20, 20, 0, 0);
  {
    Vector3 local_C8;
    Script_EVT_GetPosition(CHARA_AKIR, -1, &local_C8, 0);
    Script_vfStageAssignRingBoundary(&local_C8, 5.0f);
  }
  Script_ryoSetChara(CHARA_AKIR);
  SCNF_WaitFrames_0000(1);
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
  Script_vfConfigureFormationGroup(CHARA_AKIR, VF_FORMATION_APPLY_FLAGS, VF_HP_TEAM_BASED, 49);
  task_0 = 0x5953494B;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 200;
  *(&task_0 + 5) = 20;
  *(&task_0 + 6) = 0;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(CHARA_KISY, VF_FORMATION_APPLY_FLAGS, 0x200000, 0x800);
  Script_vfConfigureFormationGroup(CHARA_KISY, VF_FORMATION_APPLY_FLAGS, VF_HP_TEAM_BASED, 49);
  Script_vfConfigureFormationGroup(CHARA_AKIR, VF_FORMATION_APPLY_FLAGS, VF_SfxEventPending, 50);
  Script_vfConfigureFormationGroup(CHARA_KISY, VF_FORMATION_APPLY_FLAGS, VF_SfxEventPending, 50);
  SCN_Enqueue_VFGI(2, 99, 1.0f, 0, -1);
  local_E7 = 1;
  local_D4 = 7200;
  for (;;) {
    int32_t local_D0;
    if (Script_CheckTaskStatus(CHARA_AKIR)) {
      local_E7 = 3;
    }
    if (Script_CheckTaskStatus(CHARA_KISY)) {
      local_E7 = 4;
    }
    local_D0 = local_E7;
    if ((local_D0 == 3) | (local_D0 == 4)) {
      break;
      goto L02495B;
    }
    L02495B:;
    SCNF_WaitFrames_0000(1);
    local_D4 = local_D4 - 1;
    if (local_D4 <= 0) {
      break;
    }
  }
  Script_executeAicaCommand(2720, 2, 20);
  SCNF_WaitFrames_0000(10);
  Script_filmCheckOrSetTransitionUtil(20, 0, 0, 0, 0, 255, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_vfToggleBattleState(scnPostBattleDelay);
  for (;;) {
    if (SCN_GetIsFighting() == 2) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_FaceEx(CHARA_AKIR, 1, 0, 2);
  Script_ResetHandMotion(CHARA_AKIR, -1, 0, 0);
  Script_ResetHandMotion(CHARA_AKIR, -1, 1, 0);
  Script_shenClearFreeState(0);
  Script_motmToggleFlags(CHARA_KISY, 2, 0x80);
  Script_motmToggleFlags(CHARA_AKIR, 2, 0x80);
  Script_SetCPAMFlags(CHARA_KISY, 0);
  Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT, 0x49414843, 1);
  local_DC = Script_CycleManagerUtils(CYCLE_CMD_GET_CYCLE_PTR_BY_CHARA_ID, 0x49414843);
  while (local_DC == 0) {
    SCNF_WaitFrames_0000(1);
    local_DC = Script_CycleManagerUtils(CYCLE_CMD_GET_CYCLE_PTR_BY_CHARA_ID, 0x49414843);
  }
  while (Script_CycleManagerUtils(CYCLE_CMD_GET_NEXT_TASK_POINTER, local_DC) == 0) {
    SCNF_WaitFrames_0000(1);
  }
  Script_motmGetCharaFlags(CHARA_CHAI, 0, 0);
  SCNF_WaitFrames_0000(1);
  Script_LoadSND(0, 5, "FREE", 0);
  Script_LoadSND(0, 4, "FREE", 0);
  Script_LoadSND(0, 3, "FREE", 0);
  Script_LoadSND(0, 2, "FREE", 0);
  Script_LoadSND(0, 2, "bgm024.snd", 0);
  Script_executeAicaCommand(12456, 0, 0);
  Script_executeAicaCommand(1184, 2, 86);
  Script_LoadSND(0, 4, "a1_bdygd.snd", 0);
  Script_scenToggleVisibleFor(CHARA_CHAI, 1);
  script_101CE(CHARA_CHAI);
  Script_charaVisUtils(CHARA_CHAI, 1);
  Script_SetCharaFlags(CHARA_CHAI, 4);
  Script_filmCheckOrSetTransitionUtil(60, 255, 0, 0, 0, 0, 0, 0, 0);
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 1);
  Script_SetCharaRefBoardPos(CHARA_AKIR, &data_2EE10);
  Script_charaSetRefboardFlagTo(CHARA_KISY, 1);
  Script_SetCharaRefBoardPos(CHARA_KISY, &data_2EE10);
  Script_charaSetRefboardFlagTo(CHARA_CHAI, 1);
  Script_SetCharaRefBoardPos(CHARA_CHAI, &data_2EE10);
  Script_motmGetCharaFlags(CHARA_KISY, 0, 0);
  Script_SetMotnFlag(CHARA_KISY, -1);
  script_24F4A(1);
  Script_executeAicaCommand(2720, 2, 30);
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 2);
  Script_charaSetRefboardFlagTo(CHARA_KISY, 2);
  Script_charaSetRefboardFlagTo(CHARA_CHAI, 2);
  Script_mapmSetCamFlags(0, 1);
  Script_SetGameStateTo(AUTH_None);
  Script_SetCameraSettings(5, 6, 1, 0);
  SCNF_WaitFrames_0000(1);
  script_2500F();
  SCNF_WaitFrames_0000(1);
  Script_CycleManagerUtils(CYCLE_CMD_LOAD_OR_RESET_RUNNING_PROGRAM, local_DC);
  Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT, 0x49414843, 0);
  Script_CycleManagerUtils(CYCLE_CMD_LOAD_OR_RESET_RUNNING_PROGRAM, local_E0);
  Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT, 0x5953494B, 0);
  Script_AuthorUtils(AUTH_FREE, 0);
  Script_AuthorUtils(AUTH_FREE, 1);
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "KSBT");
  SCNF_WaitFrames_0000(1);
  SCNF_WaitFrames_0000(30);
  Script_LoadSND(0, 4, "FREE", 0);
  Script_LoadSND(0, 3, "FREE", 0);
  Script_LoadSND(0, 2, "FREE", 0);
  Script_LoadSND(0, 2, "SELECT", 0);
  Script_LoadSND(0, 4, "f1jutadn.snd", 0);
  Script_executeAicaCommand(16296, 0, 0);
  Script_executeAicaCommand(1184, 2, 100);
  Script_executeAicaCommand(2720, 1, 120);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 1);
  Script_scenUtils(SCENARIO_CMD_UPDATE_TIMER);
  Script_SetActionSelectorHUDElementVisibility(0);
  Script_ReallocateUNFO(local_D8);
  Script_LoadVoiceFile(0, "A0167");
  task_0 = -6;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 0;
  *(&task_0 + 3) = 0;
  *(&task_0 + 4) = 0;
  *(&task_0 + 5) = 5;
  *(&task_0 + 6) = 0;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0;
  Script_vfUtils(&task_0);
  Script_CycleManagerUtils(CYCLE_CMD_RESET_LOAD_SETTLE);
  for (;;) {
    if (Script_CycleManagerUtils(CYCLE_CMD_TICK_LOAD_SETTLE_COUNT) == 0) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_filmCheckOrSetTransitionUtil(60, 255, 0, 0, 0, 0, 0, 0, 0);
  Script_ryoControl(RYO_RestoreControl);
  Script_FaceEx(CHARA_AKIR, 0, 0, 2);
  Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
  Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
  Script_shenClearFreeState(1);
  return;
}

static int32_t script_24E45(void) {
  int32_t local_FC;
  Vector3i local_F0;
  local_FC = 60;
  if (Script_scenCharaGetMotAnimId(CHARA_AKIR) != 1374) {
    Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_WALK_SP2_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  }
  Script_SetCharaMotionPlaybackRate(CHARA_AKIR, 0.7f);
  Script_charaToggleMotEnabled(CHARA_AKIR, 1);
  while (local_FC > 0) {
    Script_mtwkGetSetRotation(CHARA_AKIR, &local_F0, 0);
    (&local_F0)->y = (&local_F0)->y & 0xFFFF;
    if ((&local_F0)->y > 0x8000) {
      Vector3i local_E4;
      (&local_E4)->y = -127;
      Script_EVT_SetRotation(CHARA_AKIR, -0x70000000, &local_E4, 0);
    } else {
      Vector3i local_E4;
      (&local_E4)->y = 127;
      Script_EVT_SetRotation(CHARA_AKIR, -0x70000000, &local_E4, 0);
    }
    SCNF_WaitFrames_0000(1);
    local_FC = local_FC - 1;
  }
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  return Script_srchControl(0);
}

static void script_24F4A(int32_t arg0) {
  int32_t unused_FC;
  int32_t local_F8;
  task_2F0 = arg0;
  local_F8 = arg0;
  switch (local_F8) {
    case 0:
      Script_scenCreateTask(script_25036, 0);
      Script_shenSeqUtils(arg0);
      while (Script_shenSeqUtils(-1)) {
        SCNF_WaitFrames_0000(1);
      }
      break;
    case 1:
      Script_scenCreateTask(script_2521B, 0);
      Script_shenSeqUtils(arg0);
      while (Script_shenSeqUtils(-1)) {
        SCNF_WaitFrames_0000(1);
      }
      break;
  }
  Script_set_val_to_1(2, 0);
  return;
}

static void script_24FC4(void) {
  script_101CE(CHARA_AKIR);
  script_101CE(CHARA_KISY);
  (void)1;
  Script_facemotUtils(CHARA_KISY, 1, 0);
  Script_ResetHandMotion(CHARA_KISY, 0, 0, 0);
  Script_ResetHandMotion(CHARA_KISY, 0, 1, 0);
  return;
}

static void script_2500F(void) {
  script_102A8(CHARA_AKIR);
  script_102A8(CHARA_KISY);
  script_102A8(CHARA_CHAI);
  return;
}

static int32_t script_25036(void) {
  int16_t local_FE;
  uint8_t unused_F0[14];
  local_FE = 0;
  while (1) {
    if (Script_shenSeqUtils(-1)) {
      switch (local_FE) {
        case 186:
          Script_FaceLookAtTargetUtils(CHARA_AKIR, 2, &data_2EF14, 2);
          break;
        case 220:
          Script_FaceLookAtTargetUtils(CHARA_AKIR, 0);
          break;
      }
      switch (local_FE) {
        case 0:
          Script_ResetHandMotion(CHARA_KISY, 0, 1, 0);
          Script_ResetHandMotion(CHARA_KISY, 0, 0, 0);
          Script_CharHandlers(CHARA_KISY, 0, 8, 16);
          Script_CharHandlers(CHARA_KISY, 1, 8, 16);
          Script_CharHandlers(CHARA_AKIR, 0, 8, 16);
          Script_CharHandlers(CHARA_AKIR, 1, 8, 16);
          break;
        case 235:
          Script_CharHandlers(CHARA_AKIR, 0, 0, 16);
          Script_CharHandlers(CHARA_AKIR, 1, 0, 16);
          break;
        case 550:
          Script_ResetHandMotion(CHARA_KISY, 1, 1, 0);
          Script_ResetHandMotion(CHARA_KISY, 1, 0, 0);
          break;
        case 580:
          Script_handSetFloatsFor(CHARA_KISY, 1, data_2ED2C, 50);
          Script_SetHandRootRotation(CHARA_KISY, 0, 0x15, &data_2EF20);
          Script_handSetFloatsFor(CHARA_KISY, 0, data_2EC48, 50);
          break;
      }
      switch (local_FE) {
        case 0:
          Script_FACE(CHARA_AKIR, 0, 0, 0);
          Script_SetMotnFlag(CHARA_AKIR, -1);
          break;
        case 200:
          Script_FACE(CHARA_AKIR, 2, 0, 30);
          break;
        case 587:
          Script_FACE(CHARA_AKIR, 0, 0, 30);
          Script_executeAicaCommand(2720, 2, 64);
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

static int32_t script_2521B(void) {
  int16_t local_FE;
  uint8_t unused_FC[2];
  Vector3 local_F0;
  int32_t unused_E4[3];
  local_FE = 0;
  while (1) {
    if (Script_shenSeqUtils(-1)) {
      switch (local_FE) {
        case 0:
          Script_ResetHandMotion(CHARA_KISY, 0, 1, 0);
          Script_ReadModelFormat7(CHARA_KISY, 0, data_2EF2C, 10);
          Script_ResetHandMotion(CHARA_KISY, 0, 0, 0);
          Script_CharHandlers(CHARA_KISY, 1, 0, 16);
          Script_CharHandlers(CHARA_AKIR, 0, 0, 16);
          Script_CharHandlers(CHARA_AKIR, 1, 0, 16);
          break;
        case 50:
          Script_CharHandlers(CHARA_KISY, 0, 8, 16);
          Script_CharHandlers(CHARA_KISY, 1, 8, 16);
          break;
        case 330:
          Script_CharHandlers(CHARA_AKIR, 0, 8, 16);
          Script_CharHandlers(CHARA_AKIR, 1, 8, 16);
          break;
      }
      if (local_FE == 4) {
        Script_mapmSetCamFlags(0, 0);
      }
      if ((local_FE >= 760) & (local_FE <= 1141)) {
        Script_EVT_GetPosition(CHARA_KISY, 5, &local_F0, 0);
        Script_SetLookPointDetails(CHARA_AKIR, 0x18000, &local_F0, 0);
        Script_FaceLookAtTargetUtils(CHARA_AKIR, 2, &local_F0, 1);
      } else {
        if (local_FE == 1142) {
          Script_SetCharaLookAtPosition(CHARA_AKIR, -98312, 0, 0);
          Script_FaceLookAtTargetUtils(CHARA_AKIR, 0);
          Script_CharHandlers(CHARA_AKIR, 0, 0, 16);
          Script_CharHandlers(CHARA_AKIR, 1, 0, 16);
          goto L02539D;
        }
      }
      L02539D:;
      switch (local_FE) {
        case 65:
          Script_FACE(CHARA_KISY, 1, 4, 10);
          break;
        case 198:
          Script_FACE(CHARA_KISY, 0, 0, 0);
          break;
        case 360:
          Script_FACE(CHARA_AKIR, 2, 0, 0);
          break;
        case 655:
          Script_FACE_LCG(CHARA_AKIR, 2, 0, 0);
          break;
        case 660:
          Script_FACE(CHARA_AKIR, 7, 0, 20);
          break;
        case 730:
          Script_FACE_LCG(CHARA_AKIR, 0, 6, 0);
          Script_FACE(CHARA_AKIR, 0, 0, 30);
          break;
        case 1060:
          Script_FACE(CHARA_AKIR, 2, 0, 20);
          break;
        case 1150:
          Script_FACE(CHARA_AKIR, 0, 0, 30);
          break;
        case 1550:
          Script_executeAicaCommand(2720, 2, 30);
          break;
        case 1580:
          Script_scenCreateTask(script_25563, 0);
          break;
        case 1760:
          Script_FACE(CHARA_CHAI, 4, 4, 30);
          break;
      }
      switch (local_FE) {
        case 1900:
          Script_GetDataFromFightProgram(CHARA_CHAI, 1);
          Script_SetFightProgramStateBytes(CHARA_CHAI, 2, 1);
          break;
        case 1940:
          Script_GetDataFromFightProgram(CHARA_CHAI, 0);
          break;
        case 2040:
          Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
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

static int32_t script_25563(void) {
  Script_LoadSND(3, 0, 0, 0);
  Script_LoadSND(1, 2, "tsm001.snd", 0x8000);
  while (Script_LoadSND(2, 0, 0, 0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_LoadSND(4, 0, 0, 0);
  Script_executeAicaCommand(3496, 0, 0);
  Script_executeAicaCommand(1184, 2, 86);
  return Script_srchControl(0);
}

__attribute__((scn_string_bindings(1, data_2EF6A)))
void script_255CA(void) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  Script_filmCheckOrSetTransitionUtil(0, 255, 0, 0, 0, 0, 0, 0, 0);
  Script_SetCameraSettings(5, 6, 1, 0);
  Script_motiLoad(Script_GetSceneID(), SHEN_NAME('FREE'), "M_MOBJ.BIN");
  local_F8 = Script_ryoSleepUtils(0);
  script_26216();
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 65.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 17.0f);
  Script_SetMapFlags(0, 1);
  Script_PlayerStatsUtils(PS_IncrementSpotVisitCount, 19);
  script_2217A();
  if ((Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_NOZOMI_KIDNAPPED) == 1) & (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOZOMI_RESCUE_RUSH_ACTIVE) == 0)) {
    Script_SetShouldSleepFlag(0);
  }
  local_FC = Script_GetCurrentEntryPoint();
  SCNF_WaitFrames_0000(1);
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, "/scene/02/JD00/", "JD00") == 0) {
    SCNF_WaitFrames_0000(1);
  }
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2)) {
    SCNF_WaitFrames_0000(1);
  }
  script_219B3();
  script_10B19();
  if (local_FC == 20) {
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_JD00_ENTRY20_TIMED_EVENT_DONE, 1);
    SCNF_WaitFrames_0000(1);
  }
  script_DD78(CHARA_AKIR, (DSWKDoorEntry *)data_2917C, (int32_t *)data_29760, (DoorResourceDescriptor *)&data_29934, (MapObjectRecord *)data_29A24, 10, 0);
  SCNF_WaitFrames_0000(1);
  Script_EVT_GetPosition(CHARA_AKIR, -1, &task_2B8, ENTITY_XFORM_WORLD_SPACE);
  Script_mtwkGetSetRotation(CHARA_AKIR, &task_2C4, 0x42000000);
  SCNF_WaitFrames_0000(1);
  task_2D8 = Script_scenCreateTask(script_21B77, 0);
  script_25E0D(CHARA_AKIR);
  task_2D0 = Script_scenCreateTask(script_0030, 6, CHARA_AKIR, 2, 0, &data_2E1F8, 2, 0);
  script_2643((int32_t)data_2E34C);
  Script_scenCreateTask(script_25909, 0);
  Script_scenCreateTask(script_22BB0, 0);
  Script_scenCreateTask(script_13CA5, 0);
  Script_scenCreateTask(script_2225B, 0);
  script_14C1C(0x4C455442, 9997, -0x3F5E84B6, 0, 0x4192C189, 0);
  script_26418();
  Script_MotStepUtil(CHARA_AKIR, 269);
  DSWK_DoorUtils(DOOR_CMD_SET_STATE_1);
  script_25933();
  script_E078();
  if (script_21DD1(9, 0, 19, 0)) {
    script_E175(0);
  }
  script_25C91();
  local_F4 = local_FC;
  switch (local_F4) {
    case 30:
      SCNF_WaitFrames_0000(1);
      SCNF_WaitFrames_0000(1);
      script_23286(0);
      break;
    case 10:
      Script_cdttLoadScrUtils(1, 0);
      script_23FAE();
      break;
  }
  if (local_F8 != 0) {
    Script_ryoSleepUtils(1);
    task_2DE = Script_ryoSleepUtils(3, 0, 0);
    task_2D4 = Script_ryoSleepUtils(3, 0, 1);
  } else {
    Script_ryoSleepUtils(3, 1, 0, 0);
    Script_ryoSleepUtils(3, 1, 1, 0);
  }
  Script_ryoSleepUtils(2, 1);
  Script_CycleManagerUtils(CYCLE_CMD_RESET_LOAD_SETTLE);
  for (;;) {
    if (Script_CycleManagerUtils(CYCLE_CMD_TICK_LOAD_SETTLE_COUNT) == 0) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_cdttLoadScrUtils(1, 0);
  Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_shenClearFreeState(1);
  Script_ryoControl(RYO_RestoreControl);
  return;
}

static int32_t script_25909(void) {
  script_FECF();
  for (;;) {
    script_22806();
    if (script_FF21() != -1) {
      script_FEF9();
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_25933(void) {
  script_E5F1(6, 0, -1, 0x2102AB, -1, -1, -1);
  DSWK_AssignDoorEntryConfig(0, 5, 4, -1, 0x2502AB, -1, -1, -1);
  DSWK_AssignDoorEntryConfig(4, 6, 1, -1, 0x2002AB, -1, -1, -1);
  DSWK_AssignDoorEntryConfig(3, 7, 1, 6, 0x2202AB, 0x2702AB, 0x2602AB, 0x444E4F4B);
  DSWK_AssignDoorEntryConfig(9, 6, 1, -1, 0x2002AB, -1, -1, -1);
  DSWK_AssignDoorEntryConfig(16, 7, 1, 6, 0x2202AB, 0x2702AB, 0x2602AB, 0x4E454154);
  DSWK_AssignDoorEntryConfig(27, 6, 1, -1, 0x2102AB, -1, -1, -1);
  DSWK_AssignDoorEntryConfig(13, 7, 1, 3, 0x2202AB, 0x2702AB, 0x2602AB, 0x41544F4B);
  DSWK_AssignDoorEntryConfig(2, 6, 1, -1, 0x2202AB, -1, -1, -1);
  DSWK_AssignDoorEntryConfig(22, 6, 1, -1, 0x2002AB, -1, -1, -1);
  DSWK_AssignDoorEntryConfig(6, 7, 1, 3, 0x2102AB, 0x2902AB, 0x2802AB, 0x55534159);
  DSWK_AssignDoorEntryConfig(26, 2, 8, -1, -1, -1, -1, -1);
  DSWK_AssignDoorEntryConfig(15, 6, 1, -1, 0x2202AB, -1, -1, -1);
  DSWK_AssignDoorEntryConfig(17, 6, 1, -1, 0x2202AB, -1, -1, -1);
  DSWK_AssignDoorEntryConfig(12, 6, 1, -1, 0x2002AB, -1, -1, -1);
  {
    int32_t local_FC;
    local_FC = Script_ScenDataUtil(task_E8, 4, 157);
    local_FC = local_FC & -65;
    local_FC = local_FC & -129;
    local_FC = local_FC | 0x40;
    Script_ScenDataUtil(task_E8, 14, 157, local_FC);
  }
  DSWK_AssignDoorEntryConfig(23, 7, 1, 4, 0x2202AB, 0x2702AB, 0x2602AB, 0x53484B54);
  DSWK_AssignDoorEntryConfig(18, 6, 1, -1, 0x2002AB, -1, -1, -1);
  DSWK_AssignDoorEntryConfig(24, 6, 1, -1, 0x2002AB, -1, -1, -1);
  {
    int32_t local_FC;
    local_FC = Script_ScenDataUtil(task_E8, 4, 313);
    local_FC = local_FC & -65;
    local_FC = local_FC & -129;
    local_FC = local_FC | 0x40;
    Script_ScenDataUtil(task_E8, 14, 313, local_FC);
  }
  DSWK_AssignDoorEntryConfig(20, 6, 1, -1, 0x2202AB, -1, -1, -1);
  DSWK_AssignDoorEntryConfig(21, 6, 1, -1, 0x2202AB, -1, -1, -1);
  DSWK_AssignDoorEntryConfig(19, 6, 1, -1, 0x2002AB, -1, -1, -1);
  DSWK_AssignDoorEntryConfig(11, 7, 1, 5, 0x2202AB, 0x2702AB, 0x2602AB, 0x57474455);
  DSWK_AssignDoorEntryConfig(5, 6, 1, -1, 0x2202AB, -1, -1, -1);
  DSWK_AssignDoorEntryConfig(7, 6, 1, -1, 0x2202AB, -1, -1, -1);
  DSWK_AssignDoorEntryConfig(14, 7, 1, 2, 0x2202AB, 0x2702AB, 0x2602AB, 0x524F4F44);
  DSWK_AssignDoorEntryConfig(25, 6, 1, -1, 0x2202AB, -1, -1, -1);
  DSWK_AssignDoorEntryConfig(1, 7, 1, 5, 0x2202AB, 0x2702AB, 0x2602AB, 0x524F4F44);
  DSWK_AssignDoorEntryConfig(8, 6, 1, -1, 0x2202AB, -1, -1, -1);
  DSWK_AssignDoorEntryConfig(10, 6, 1, -1, 0x2202AB, -1, -1, -1);
  return;
}

static void script_25C91(void) {
  Script_CycleManagerUtils(CYCLE_CMD_SETUP, "scene/02/D000", (int32_t)"SCRTEST.BIN", 0x3030444A);
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
  SCNF_WaitFrames_0000(1);
  script_260BE();
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 1);
  return;
}

static void script_25CC9(void) {
  int32_t local_FC;
  local_FC = Script_GetSceneID();
  switch (local_FC) {
    case 1:
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_ENTRY2_TIMED_EVENT_DONE) == 0) {
        Script_TimedTransitionUtils(TIMED_TRANSITION_REGISTER, 1, 0x3044484A, 2, &data_2F00C, 0xD5A6C3, TimedTransitionUpdateMemoItem150);
      }
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JD00_ENTRY20_TIMED_EVENT_DONE) == 0) {
        Script_TimedTransitionUtils(TIMED_TRANSITION_REGISTER, 1, 0x3030444A, 20, &data_2EFD4, 0xDA3AA3, 0);
      }
      break;
    case 2:
      if ((Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_GMCT_ROUTE_STARTED) == 0) | (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_GMCT_ROUTE_ADVANCED) == 0)) {
        Script_TimedTransitionUtils(TIMED_TRANSITION_REGISTER, 2, 0x54434D47, 1, &data_2EF9C, 0xB7420F, 0);
      }
      break;
    case 3:
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOZOMI_RESCUE_RUSH_ACTIVE) == 0) {
        Script_TimedTransitionUtils(TIMED_TRANSITION_REGISTER, 3, 0x4F4D4F4A, 20, &data_2EFB8, 0x2DC9FE, TimedTransitionRollbackToPreviousDay2330);
      }
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_FIRED_FROM_JOB) & ~Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 970)) {
        if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_MFSY_ENTRY42_TIMED_EVENT_DONE) == 0) {
          Script_TimedTransitionUtils(TIMED_TRANSITION_REGISTER, 3, 0x5953464D, 42, &data_2EFF0, 0xBBB817, 0);
        }
      }
      break;
  }
  return;
}

static void script_25E01(int32_t arg0) {
  script_25E19(arg0);
  return;
}

static void script_25E0D(int32_t arg0) {
  script_25E19(arg0);
  return;
}

static void script_25E19(int32_t arg0) {
  if (Script_StepUtils(GET_STEP_HIGHER, 0) == 22) {
    int32_t local_FC;
    local_FC = Script_StepUtils(GET_STEP_LOWER, 0);
    if ((local_FC == 2) | (local_FC == 3)) {
      Script_SetShouldSleepFlag(0);
      goto L025E52;
    }
  }
  L025E52:;
  Script_ryoSetChara(arg0);
  Script_GetSetWeatherSettings(WEATHER_SETTINGS_SET_DISABLE_WEATHER, 1);
  Script_WeatherUtils_0(WeatherDataUtil_ApplyCurrentWeatherOrEnableDateTimeRead, 0);
  Script_WeatherTaskUtil(InitTaskData);
  Script_facemotUtils(arg0, 1, 0);
  Script_facemotUtils(arg0, 0, 0);
  Script_FaceEx(arg0, 0, 0, 2);
  Script_CharHandlers(arg0, -1, 0, 0);
  script_25CC9();
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

static void script_25F25(int32_t arg0) {
  script_25E0D(arg0);
  for (;;) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_25F3C(int32_t arg0) {
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

static void script_25F7D(int32_t arg0) {
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

static void script_25FC1(int32_t arg0) {
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

static void script_2603F(void) {
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_PROGRESS_STEP_28_REACHED)) {
    script_25FC1(40);
  } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_PROGRESS_STEP_1E_REACHED)) {
    script_25FC1(30);
  } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_PROGRESS_STEP_14_REACHED)) {
    script_25FC1(20);
  } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JU00_CATA1_NBO_BRANCH_CHOSEN)) {
    script_25FC1(10);
  } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_PROGRESS_STEP05_SA1116_DOJO_DONE)) {
    script_25FC1(5);
  } else {
    script_25FC1(0);
  }
  return;
}

static void script_260BE(void) {
  int8_t local_FF;
  local_FF = 0;
  for (;;) {
    if (local_FF == 1) {
      break;
    }
    *(&task_3B4 + local_FF) = 0;
    local_FF = local_FF + 1;
  }
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_DBYO_DJAZ_DHQB_CHAIN_ACTIVE) == 0) {
    Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT, 0x494B4B52, 1);
  }
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JU00_HRSK_CAT_FOLLOWUP_COMPLETE) == 0) {
    Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT, 0x4D544143, 1);
  }
  return;
}

static void script_26129(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT, arg0, 1);
  local_FC = Script_CycleManagerUtils(CYCLE_CMD_GET_CYCLE_PTR_BY_CHARA_ID, arg0);
  while (local_FC == 0) {
    SCNF_WaitFrames_0000(1);
    local_FC = Script_CycleManagerUtils(CYCLE_CMD_GET_CYCLE_PTR_BY_CHARA_ID, arg0);
  }
  while (Script_CycleManagerUtils(CYCLE_CMD_GET_NEXT_TASK_POINTER, local_FC) == 0) {
    SCNF_WaitFrames_0000(1);
  }
  *(&task_3B4 + arg1) = local_FC;
  return;
}

static void script_26190(int32_t arg0, int32_t arg1) {
  Script_CycleManagerUtils(CYCLE_CMD_LOAD_OR_RESET_RUNNING_PROGRAM, *(&task_3B4 + arg1));
  Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT, arg0, 0);
  *(&task_3B4 + arg1) = 0;
  return;
}

static void script_261BC(void) {
  int32_t unused_FC;
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
  SCNF_WaitFrames_0000(1);
  if (*&task_3B4) {
    script_26190(0x414D4159, 0);
  }
  SCNF_WaitFrames_0000(1);
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 1);
  return;
}

static void script_261F7(void) {
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
  Script_CycleManagerUtils(CYCLE_CMD_PROCESS_SND_EXTRA);
  return;
}

static void script_2620A(void) {
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 1);
  return;
}

static void script_26216(void) {
  Script_ambSoundUtils(1, 0, &data_2F044, 12.0f, 30.0f, 0.9f, 2, &data_2F0C8);
  Script_ambSoundUtils(2, 0, &data_2F050, 10.0f, 30.0f, 0.9f, 2, &data_2F150);
  Script_ambSoundUtils(3, 0, &data_2F05C, 5.0f, 20.0f, 0.9f, 2, &data_2F1D8);
  Script_ambSoundUtils(4, 0, &data_2F068, 10.0f, 25.0f, 0.9f, 2, &data_2F260);
  Script_ambSoundUtils(5, 0, &data_2F074, 22.0f, 30.0f, 0.9f, 2, &data_2F2E8);
  Script_ambSoundUtils(6, 0, &data_2F080, 5.0f, 20.0f, 0.9f, 2, &data_2F370);
  Script_ambSoundUtils(7, 0, &data_2F08C, 20.0f, 30.0f, 0.9f, 2, &data_2F3F8);
  Script_ambSoundUtils(8, 1, data_2F098, 50.0f, 30.0f, 1.0f, 0, data_2F480);
  Script_ambSoundUtils(9, 1, data_2F0A4, 50.0f, 30.0f, 1.0f, 0, data_2F508);
  Script_ambSoundUtils(10, 1, data_2F0B0, 50.0f, 30.0f, 1.0f, 0, &data_2F590);
  Script_ambSoundUtils(15, 1, data_2F0BC, 200.0f, 50.0f, 1.0f, 3, &data_2F618);
  Script_Ambient1SoundUtils(0, 23976);
  return;
}

__attribute__((scn_string_bindings(0, data_2F6A0, 1, data_2F6A5, 2, data_2F6AA, 3, data_2F6AF, 4, data_2F6B6)))
static void script_263AF(void) {
  Script_LoadSND(0, 4, "FREE", 0);
  Script_LoadSND(0, 3, "FREE", 0);
  Script_LoadSND(0, 2, "FREE", 0);
  Script_LoadSND(0, 2, "SELECT", 0);
  Script_LoadSND(0, 4, "f1jutadn.snd", 0);
  Script_executeAicaCommand(16296, 0, 0);
  Script_executeAicaCommand(1184, 2, 100);
  return;
}

static void script_26418(void) {
  int32_t local_FC;
  Script_WeatherUtils(WeatherScript_LoadWccpClipSet0FromTaskOffset, &data_2F6C4, 21);
  local_FC = 0;
  while (local_FC < 21) {
    Script_WeatherUtils(WeatherScript_SetWccpClipSet0Entry, local_FC, 1);
    local_FC = local_FC + 1;
  }
  return;
}
