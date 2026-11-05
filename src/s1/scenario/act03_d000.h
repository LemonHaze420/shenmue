#pragma once

#include "scenario.h"
#include "motion/motion.h"
#include "scene/03/d000/m_tpin.h"

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
static void script_5FB0(void);
static void script_61D9(void);
static void script_6426(void);
static void script_664F(int32_t arg0, int32_t arg1);
static void script_6670(void);
static void script_6696(void);
static void script_66A8(void);
static void script_66CE(int32_t arg0);
static void script_6702(void);
static void script_678D(int32_t arg0);
static void script_6B4C(void);
static void script_6BA0(void);
static void script_6CE7(int32_t arg0);
static void script_6D02(int32_t arg0);
static void script_6D53(int32_t arg0);
static void script_6DD0(void);
static void script_6DE5(void);
static void script_6DEE(void);
static void script_6DF1(int32_t arg0);
static void script_6E31(int32_t arg0);
static int32_t script_6E9E(void);
static int32_t script_7033(void);
static int32_t script_7068(void);
static int32_t script_70F9(void);
static int32_t script_71A6(void);
static int32_t script_722D(void);
static void script_730E(int32_t arg0, int32_t arg1);
static int32_t script_737D(int32_t arg0, int32_t arg1, float arg2);
static int32_t script_73E6(int32_t arg0, int32_t arg1, float arg2);
static void script_7458(int32_t arg0);
static void script_74B6(int32_t arg0);
static void script_7514(int32_t arg0);
static int32_t script_7572(int32_t arg0, int32_t arg1, float arg2, float arg3);
static void script_75EB(int32_t arg0, float arg1, float arg2, float arg3);
static void script_7613(int32_t arg0, float arg1, float arg2, float arg3);
static void script_7667(int32_t arg0, int32_t arg1, int32_t arg2);
static int32_t script_76B7(int32_t arg0);
static void script_775E(int32_t arg0, int32_t arg1);
static int32_t script_7781(int32_t arg0);
static void script_77B4(int32_t arg0, int32_t arg1);
static void script_77D7(int32_t arg0, int32_t arg1);
static void script_7836(int32_t arg0);
static void script_784B(void);
static void script_785A(void);
static void script_7869(void);
static void script_786C(void);
static void script_786F(void);
static void script_7872(void);
static void script_7875(void);
static int32_t script_7882(void);
static void script_788E(void);
static int32_t script_789D(void);
static int32_t script_78AC(int32_t arg0);
static void script_78CA(float arg0);
static int32_t script_78E9(int32_t arg0, int32_t arg1);
static void script_7961(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4);
static int32_t script_7F13(int32_t arg0);
static void script_7FE4(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_7FF4(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_80F8(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_829A(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_83FB(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_855C(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_87FA(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_88C8(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_892F(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_8996(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_89DD(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_8B13(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_93D4(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_94F2(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_9BB7(int32_t arg0, int32_t arg1);
static void script_9C8D(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_ABA0(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_AC0C(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_B0C6(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_B780(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_D060(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_D781(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_DF0B(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_DFE3(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_E45A(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_F3ED(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_F837(int32_t arg0, DSWKDoorEntry *arg1, int32_t *arg2, DoorResourceDescriptor *arg3, MapObjectRecord *arg4, int32_t arg5, int32_t arg6);
static void script_F9C3(DSWKDoorEntry *arg0, int32_t *arg1, DoorResourceDescriptor *arg2, MapObjectRecord *arg3, int32_t arg4);
static void script_F9F7(void);
static void script_FA60(int32_t arg0, int32_t arg1);
static void script_FAFD(int32_t arg0, int32_t arg1);
static void script_FB37(void);
static void script_FB9A(void);
static void script_FBFD(int32_t arg0);
static void script_FC34(int32_t arg0);
static void script_FC6B(int32_t arg0);
static void script_FEAD(int32_t arg0);
static void script_FEF6(int32_t arg0);
static void script_FF3F(int32_t arg0, int32_t arg1);
static void script_FFB5(int32_t arg0);
static void script_FFC3(int32_t arg0);
static void script_FFD1(int32_t arg0);
static void script_10098(void);
static void script_100A4(void);
static void script_100B0(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5, int32_t arg6);
static int32_t script_100F3(void);
static int8_t script_10204(int32_t arg0, int32_t arg1, int8_t arg2);
static void script_1029C(int32_t arg0);
static void script_1046B(int32_t arg0);
static int32_t script_1049C(int32_t arg0, int32_t arg1, int32_t arg2, float arg3, float arg4, int32_t arg5, int32_t arg6);
static void script_10B65(int32_t arg0, int32_t arg1);
static void script_10B97(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_10D07(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_10EC3(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_11077(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_1128D(int32_t arg0, int32_t arg1);
static void script_11357(int32_t arg0, int32_t arg1);
static void script_113B8(int32_t arg0);
static void script_1155D(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_115F5(int32_t arg0, int32_t arg1);
static void script_116D7(int32_t arg0);
static void script_1172E(int32_t arg0, int32_t arg1);
static void script_1178D(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static int32_t script_117B4(float arg0, float arg1, float arg2, float arg3);
static int32_t script_11952(void);
static void script_11A19(const char *arg0, int32_t arg1);
static void script_11A66(void);
static int32_t script_11B9D(int32_t arg0);
static void script_11C16(void);
static int8_t script_11C78(void);
static void script_11CC3(int32_t arg0);
static void script_11D2C(int32_t arg0);
static void script_11D57(void);
static int32_t script_11D7E(int32_t arg0);
static int8_t script_11DC0(int32_t arg0);
static void script_11E98(void);
static void script_11EA5(void);
static void script_1237A(void);
static void script_12EC1(void);
static void script_12F7F(void);
static void script_12FA4(void);
static void script_136EB(void);
static int8_t script_15772(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int8_t script_1582B(void);
static void script_159E1(int32_t arg0);
static int8_t script_16B52(void);
static void script_170A1(int32_t arg0);
static void script_1AF5F(void);
static int8_t script_1BA27(void);
static int8_t script_1BA4F(void);
static void script_1BB6A(int32_t arg0);
static int32_t script_1BC53(int32_t arg0, int32_t arg1);
static int32_t script_1BCBC(void);
static void script_1BCD7(int32_t arg0);
static void script_1BDC0(int32_t arg0);
static void script_1BFAE(void);
static void script_1BFE5(int32_t arg0);
static int8_t script_1C0AD(void);
static void script_1CD7C(void);
static void script_1CF02(void);
static void script_1CF16(void);
static void script_1CF60(void);
static int32_t script_1D0A5(void);
static int32_t script_1D128(void);
static void script_1D854(void);
static void script_1D8C7(void);
static void script_1D927(void);
static void script_1D9C5(void);
static void script_1DA61(int32_t arg0);
static int16_t script_1DC02(int8_t arg0, int8_t arg1);
static void script_1DDED(int32_t arg0, int32_t arg1);
static int32_t script_1DE3C(int32_t arg0, int32_t arg1);
static void script_1DE8F(int32_t arg0, int32_t arg1, int32_t arg2, float arg3);
static void script_1E0C6(void);
static int32_t script_1E232(int32_t arg0);
static int32_t script_1E29C(int32_t arg0);
static int32_t script_1E4CD(void);
static int32_t script_1E57D(void);
static int32_t script_1E620(void);
static void script_1E6B2(void);
static void script_1E6DF(int32_t arg0);
static void script_1E7B1(void);
static void script_1E958(void);
static void script_1EA32(void);
static void script_1EAF6(void);
static void script_1EB25(int32_t arg0);
static void script_1ECAE(int32_t arg0, int32_t arg1);
static int32_t script_1ED2F(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4);
static int32_t script_1EDE9(void);
static int32_t script_1EF24(float arg0);
static int32_t script_1EFF4(void);
static int32_t script_1F0BD(void);
static int32_t script_1F167(void);
static int32_t script_1F1B1(void);
static void script_1F2AC(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_1F49D(void);
static int32_t script_1F839(void);
static int32_t script_1F92B(int32_t arg0);
static int32_t script_1F9AB(int32_t arg0);
static void script_1FADD(int32_t arg0);
static int32_t script_1FD49(void);
static void script_1FE21(void);
static int32_t script_1FE24(int32_t arg0);
static int32_t script_1FF3A(int16_t arg0);
static int32_t script_20063(void);
static void script_2008C(void);
static int32_t script_200F6(void);
static int32_t script_201B6(void);
static int32_t script_204D5(void);
static void script_205B2(void);
static int32_t script_205CA(void);
static int32_t script_205DD(void);
static void script_206B8(void);
static int32_t script_20793(int32_t arg0, int32_t arg1);
static void script_2094C(int32_t arg0);
static void script_209DF(int32_t arg0, int32_t arg1);
static void script_20A95(void);
static void script_20B0F(int32_t arg0);
static int32_t script_20C25(void);
static int32_t script_21167(int32_t arg0, const char *arg1, const char *arg2, ...);
static void script_211B9(int32_t arg0, Vector3 *arg1);
static void script_211F1(int32_t arg0, Vector3i *arg1);
static void script_21229(void);
static void script_2126A(void);
static void script_212CA(void);
static void script_21375(void);
static void script_213C6(void);
static int32_t script_217EB(int32_t arg0, int32_t arg1);
static void script_21934(int32_t arg0, int32_t arg1);
static int32_t script_219C7(void);
static void script_21D43(void);
static int32_t script_21DE3(float arg0, float arg1);
static int32_t script_21F1A(void);
static void script_2206B(int32_t arg0);
static void script_220C4(int32_t arg0);
static void script_2228D(void);
static void script_22290(void);
static void script_22315(void);
static void script_223B3(void);
static void script_22408(void);
static int8_t script_224A8(void);
static void script_224EF(void);
static void script_22557(void);
static void script_22563(void);
static void script_2256F(int32_t arg0);
static void script_226A1(int32_t arg0, int32_t arg1);
static void script_227DC(void);
static void script_22894(void);
static int32_t script_22904(int32_t arg0);
static int32_t script_22B91(int32_t arg0);
static int32_t script_22BE1(void);
static void script_22BFE(void);
static void script_22CA2(void);
static void script_22E05(void);
static void script_22EBC(void);
static void script_231AD(int32_t arg0);
static void script_23569(int32_t arg0, int32_t arg1);
static void script_23610(int32_t arg0, int32_t arg1);
static void script_23674(int32_t arg0, int32_t arg1);
static void script_2371B(int32_t arg0, int32_t arg1);
static int32_t script_2377F(int32_t arg0, int32_t arg1);
static int32_t script_237F0(int32_t arg0, int32_t arg1);
static void script_23861(int32_t arg0, int32_t arg1);
static void script_239A2(int32_t arg0, int32_t arg1);
static int32_t script_23A3D(int32_t arg0);
static int32_t script_23A90(int32_t arg0);
static void script_23AEB(int32_t arg0);
static int32_t script_23B69(int32_t arg0);
static void script_23BCE(int32_t arg0, int32_t arg1);
static void script_23BEE(float arg0, float arg1, float arg2, int32_t arg3);
static void script_23C48(float arg0, float arg1, float arg2, int32_t arg3, int32_t arg4);
static void script_23CA1(int32_t arg0);
static void script_23D0C(int32_t arg0);
static void script_23D77(void);
static int32_t script_23D9F(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_23E07(const char *arg0, int32_t arg1);
static int32_t script_23E33(int32_t arg0, int32_t arg1);
static void script_23E63(int32_t arg0);
static int32_t script_2467A(void);
static void script_246B2(int32_t arg0);
static void script_24892(int32_t arg0);
static void script_249BE(int32_t arg0);
static void script_24BB0(int32_t arg0);
static void script_24DF6(void);
static int32_t script_24FB1(void);
static void script_25029(void);
static void script_25051(void);
static void script_25071(int32_t arg0);
static int8_t script_25108(int8_t arg0);
static int8_t script_25279(int32_t arg0);
static int8_t script_254E2(int32_t arg0);
static int8_t script_255D1(int32_t arg0);
static int8_t script_2578D(void);
static void script_257A7(int32_t arg0);
static int32_t script_25DF1(int32_t arg0, int32_t arg1);
static int32_t script_25E03(int32_t arg0, int32_t arg1);
static void script_26011(void);
static int32_t script_26037(void);
static void script_26272(void);
static void script_26275(int32_t arg0);
static void script_2636F(int32_t arg0);
static void script_26564(void);
static void script_2659E(void);
static void script_265AD(void);
static int32_t script_268F8(int32_t arg0);
static void script_26AB2(void);
static void script_26B26(void);
static void script_26BD4(void);
static void script_26C97(void);
static void script_26CBE(void);
static void script_26CE5(void);
static void script_26CF2(void);
static void script_26CFA(void);
static void script_26D55(void);
static void script_26D79(void);
static void script_26DA6(void);
static void script_2708A(void);
static int32_t script_271F0(void);
static void script_27399(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4);
static int32_t script_2740C(int32_t arg0);
static void script_27803(void);
static int8_t script_27831(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_278B0(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_27A2D(void);
static void script_27C43(void);
static int32_t script_27C6C(void);
static void script_27CCB(void);
static void script_27E03(int32_t arg0);
static void script_27E0F(int32_t arg0);
static void script_27E1B(int32_t arg0);
static void script_27F27(int32_t arg0);
static void script_27F3E(int32_t arg0);
static void script_27F7F(int32_t arg0);
static void script_27FC3(int32_t arg0);
static void script_28041(void);
static int32_t script_280C0(int32_t arg0);
static int32_t script_28146(void);
static int32_t script_28269(void);
static void script_282F0(void);
static int32_t script_28349(void);
static int32_t script_28383(int32_t arg0);
static void script_283A6(void);
static int32_t script_283C6(int32_t arg0);
static int32_t script_283E9(void);
static void script_28434(void);
static void script_2847B(void);
static void script_284CE(void);
static void script_2852A(void);
static void script_28571(void);
static void script_285CD(int8_t arg0);
static void script_286A3(void);
static void script_2871A(void);
static void script_28757(void);
static int32_t script_28794(int32_t arg0);
static void script_28805(int32_t arg0);
static int32_t script_28A68(int32_t arg0);
static void script_28AB0(void);
static void script_28AB8(void);
static void script_28ACC(int8_t arg0);
static void script_28E00(void);
static void script_28E68(void);
static void script_28E89(void);
static void script_28E91(void);
static void script_28E94(void);
static void script_28EA2(void);
static void script_28EA5(void);
static void script_28EA8(int32_t arg0);
static void script_28ED9(int32_t arg0);
static void script_28F0A(int32_t arg0);
static void script_28F3B(int32_t arg0);
static void script_28F6C(void);
static void script_293AC(void);
static void script_293E2(void);
static void script_2952B(void);
static void script_2953A(void);
void script_2954E(void);
static void script_29556(int32_t arg0);
static void script_29574(float arg0);
static void script_29593(int32_t arg0);
static void script_295A8(int32_t arg0, int32_t arg1);
static int32_t script_2965E(int32_t arg0);
static void script_29676(int32_t arg0);
static void script_2968C(int32_t arg0);
static void script_2969A(void);
static void script_29764(void);
static void script_29767(void);
static void script_298B5(int32_t arg0);
static int32_t script_298C9(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_298EB(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_29931(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_29945(int32_t arg0);
static void script_2995F(void);
static void script_2997A(int32_t arg0);
static void script_299A7(int32_t arg0, int32_t arg1);
static void script_299CE(void);
static void script_299D8(void);
static void script_299F3(void);
static void script_299FD(int32_t arg0);
static int32_t script_29A61(float arg0, float arg1, float arg2, float arg3);
static int32_t script_29A88(int32_t arg0, int32_t arg1);
static int32_t script_29B00(int32_t arg0, int32_t arg1, int32_t arg2, const Vector3 *arg3);
static void script_29BAC(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_29D72(int32_t arg0);
static void script_29D94(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_29E6F(void);
static void script_29E93(int32_t arg0);
static void script_29F53(void);
static void script_29F59(void);
static void script_29F64(int32_t arg0);
static void script_2A03F(int32_t arg0, int32_t arg1);
static void script_2A0A4(int32_t arg0, int32_t arg1);
static void script_2A109(void);
static void script_2A145(void);
static void script_2A160(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_2A1D4(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_2A1F9(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_2A242(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_2A2A8(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_2A370(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_2A3C6(void);
static void script_2A41B(int32_t arg0);
static void script_2A475(const char *arg0, const char *arg1);
static void script_2A4C5(int32_t arg0);
static int32_t script_2A543(int32_t arg0);
static void script_2A5AF(int32_t arg0, int32_t arg1);
static void script_2A5C6(void);
static void script_2A5E0(const char *arg0, const char *arg1);
static void script_2A603(const char *arg0, const char *arg1);
static void script_2A627(const char *arg0);
static void script_2A64B(int32_t arg0, float arg1, float arg2, float arg3, int32_t arg4, int32_t arg5, int32_t arg6);
static void script_2A69A(void);
static void script_2A6BA(int32_t arg0, int32_t arg1, Vector3 *arg2, Vector3i *arg3, const void *arg4);
static void script_2A6E2(int32_t arg0, const char *arg1);
static void script_2A70B(void);
static void script_2A725(int32_t arg0);
static void script_2A74D(void);
static void script_2A76B(void);
static void script_2A79E(void);
static void script_2A7AB(int32_t arg0);

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
static int8_t task_100;
static int8_t task_101;
static int16_t task_102;
static int8_t task_104;
static int8_t task_105;
static int8_t task_106;
static int8_t task_107;
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
static uint8_t task_113;
static int32_t task_114;
static char *task_118;
static int32_t task_11C;
static int32_t task_120;
static int32_t task_124;
static int8_t task_128;
static int8_t task_129;
static int8_t task_12A;
static uint8_t task_12B;
static int32_t task_12C;
static int32_t task_130;
static int32_t task_134;
static int32_t task_138;
static int32_t task_13C;
static int8_t task_140;
static uint8_t task_141;
static uint16_t task_142;
static DSWKDoorEntry *task_144;
static int32_t *task_148;
static DoorResourceDescriptor *task_14C;
static MapObjectRecord *task_150;
static DoorSlotState task_154;
static int32_t task_160;
static int32_t task_164[10];
static int32_t task_18C[10];
static int32_t task_1B4[10];
static int32_t task_1DC[10];
static int32_t task_204[10];
static int32_t task_22C[10];
static int32_t task_254[10];
static int32_t task_27C[10];
static int32_t task_2A4;
static int32_t task_2A8;
static Vector3 task_2AC;
static int32_t task_2B8;
static int32_t task_2BC;
static int32_t task_2C0;
static int8_t task_2C4;
static uint8_t task_2C5;
static uint16_t task_2C6;
static int8_t task_2C8;
static uint8_t task_2C9;
static uint16_t task_2CA;
static int32_t task_2CC;
static int32_t task_2D0;
static int8_t task_2D4;
static uint8_t task_2D5;
static uint16_t task_2D6;
static Vector3 task_2D8;
static int32_t task_2E4;
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
static int8_t task_310;
static int8_t task_311;
static int8_t task_312;
static int8_t task_313;
static int8_t task_314;
static uint8_t task_315;
static uint16_t task_316;
static int8_t task_318;
static uint8_t task_319;
static uint16_t task_31A;
static int32_t task_31C[23];
static int8_t task_378;
static int8_t task_379;
static int8_t task_37A;
static int8_t task_37B;
static int8_t task_37C;
static uint8_t task_37D;
static uint16_t task_37E;
static int32_t task_380;
static int32_t task_384;
static int8_t task_388;
static int8_t task_389;
static int8_t task_38A;
static int8_t task_38B;
static int8_t task_38C;
static uint8_t task_38D;
static uint16_t task_38E;
static int8_t task_390;
static uint8_t task_391;
static uint16_t task_392;
static float task_394;
static int32_t task_398;
static int32_t task_39C;
static int8_t task_3A0;
static uint8_t task_3A1;
static uint16_t task_3A2;
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
static int8_t task_418;
static int8_t task_419;
static int8_t task_41A;
static int8_t task_41B;
static int8_t task_41C;
static uint8_t task_41D;
static uint16_t task_41E;
static int32_t task_420;
static int32_t task_424;
static int8_t task_428[100][12];
static int8_t task_8D8[12];
static int8_t task_8E4[10];
static int8_t task_8EE;
static int8_t task_8EF;
static int8_t task_8F0;
static uint8_t task_8F1;
static uint16_t task_8F2;
static int32_t task_8F4;
static int32_t task_8F8;
static int8_t task_8FC;
static uint8_t task_8FD;
static uint16_t task_8FE;
static int32_t task_900[10];
static int32_t task_928;
static int32_t task_92C;
static const Vector3 data_2A7EC = {.x = 1.0f, .y = 1.0f, .z = 1.0f};
static const int32_t data_2A7F8[4] = {16777215, 16777215, 16777215, 16777215};
static const float data_2A808[3] = {20.0f, 800.0f, 1.0f};
static const int32_t data_2A814 = -1;
static const char data_2A818[10] __attribute__((scn_string_literal)) = "Kaidan : ";
static const char data_2A822[32] __attribute__((scn_string_literal)) = "========== initializing kaidan\012";
static const char data_2A842[48] __attribute__((scn_string_literal)) = "please ENDOFKDATA use can't start shuki kaidan\012";
static const char data_2A872[] __attribute__((scn_string_literal)) = "Kaidan Function Error 1\n";
static const char data_2A88B[] __attribute__((scn_string_literal)) = "Kaidan Function Error 2\n";
static const char data_2A8A4[] __attribute__((scn_string_literal)) = "Kaidan Data Error\n";
static const char data_2A8B7[10] __attribute__((scn_string_literal)) = "Kaidan : ";
static const char data_2A8C1[28] __attribute__((scn_string_literal)) = "wait ... shuki shuki shuki\012";
static const char data_2A8DD[10] __attribute__((scn_string_literal)) = "Kaidan : ";
static const char data_2A8E7[24] __attribute__((scn_string_literal)) = "KAIDAN : Warning coli!\012";
static const char data_2A8FF[24] __attribute__((scn_string_literal)) = "KAIDAN : Warning coli!\012";
static const char data_2A917[24] __attribute__((scn_string_literal)) = "KAIDAN : Warning coli!\012";
static const char data_2A92F[24] __attribute__((scn_string_literal)) = "KAIDAN : Warning coli!\012";
static const char data_2A947[24] __attribute__((scn_string_literal)) = "KAIDAN : Warning coli!\012";
static const char data_2A95F[24] __attribute__((scn_string_literal)) = "KAIDAN : Warning setY!\012";
static const char data_2A977[24] __attribute__((scn_string_literal)) = "KAIDAN : Warning setY!\012";
static const char data_2A98F[2977] = "Kaidan : \000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000V\325\377\377\000\000\000\000\000\000\000\000\302\326\377\377\000\000\000\000\260\005\000\000\000\000\000\000\000\000\000\000\000\000\000\000\232\271\377\377\000\000\000\000\000\000\000\000(\275\377\377\000\000\000\000\000\000\000\000.\270\377\377\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\\\320\377\377\000\000\000\000\000\000\000\000\344\270\377\377\000\000\000\000\000\000\000\000\224\276\377\377\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\310\321\377\377\000\000\000\000\000\000\000\000.\270\377\377\000\000\000\000\000\000\000\000\373\304\377\377\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\216\003\000\000\022\321\377\377\000\000\000\000x\367\377\377\224\276\377\377\000\000\000\000x\367\377\377\344\330\377\377\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\224\376\377\377~\322\377\377\240\364\377\377\224\376\377\377\357\356\377\377f\006\000\000\216\003\000\000\266\000\000\000\000\000\000\000\000\000\000\000m\301\377\377\000\000\000\000\266\000\000\000\261\245\377\377\000\000\000\000\"\002\000\000\014\266\377\377\000\000\000\000\000\000\000\000\274\373\377\377\000\000\000\000\000\000\000\000\000\300\377\377\000\000\000\000\000\000\000\000\253\252\377\377\000\000\000\000\000\000\000\000\302\266\377\377\000\000\000\000\000\000\000\000\336\375\377\377\000\000\000\000\000\000\000\000\027\254\377\377\000\000\000\000\000\000\000\000\315\254\377\377\000\000\000\000\000\000\000\000x\267\377\377\000\000\000\000\000\000\000\000(\375\377\377\000\000\000\000\216\003\000\000\302\266\377\377\000\000\000\000\266\000\000\000\302\266\377\377\000\000\000\000\\\360\377\377\344\270\377\377\000\000\000\000\000\000\000\000(\375\377\377\000\000\000\000\224\376\377\377?\311\377\377\240\364\377\377\224\376\377\377\261\345\377\377f\006\000\000\216\003\000\000\364\011\000\000\000\000\000\000\000\000\000\000x\267\377\377\000\000\000\000\266\000\000\000\274\233\377\377\000\000\000\000\"\002\000\000\027\254\377\377\000\000\000\000\000\000\000\000\310\361\377\377\000\000\000\000\000\000\000\000\014\266\377\377\000\000\000\000\000\000\000\000\267\240\377\377\000\000\000\000\000\000\000\000\315\254\377\377\000\000\000\000\000\000\000\000\352\363\377\377\000\000\000\000\000\000\000\000#\242\377\377\000\000\000\000\000\000\000\000\331\242\377\377\000\000\000\000\000\000\000\000\203\255\377\377\000\000\000\000\000\000\000\0004\363\377\377\000\000\000\000\216\003\000\000\315\254\377\377\000\000\000\000\266\000\000\000\315\254\377\377\000\000\000\000\\\360\377\377\357\256\377\377\000\000\000\000\000\000\000\0004\363\377\377\000\000\000\000\000\000\000\000\253\352\377\377\000\000\000\000\302\366\377\377\274\373\377\377\000\000\000\000J\377\377\377D\004\000\000\000\000\000\000\336\375\377\377\261\345\377\377\000\000\000\000\000\000\000\000\\\360\377\377\000\000\000\000\000\000\000\000\240\364\377\377\000\000\000\000J\377\377\377\372\004\000\000\000\000\000\000\000\000\000\000\027\314\377\377\000\000\000\000\000\000\000\000a\353\377\377\000\000\000\000\000\000\000\000\267\340\377\377\000\000\000\000\000\000\000\000\260\005\000\000\000\000\000\000\266\000\000\000\336\335\377\377\000\000\000\000\000\000\000\000\211\350\377\377\000\000\000\000\000\000\000\000\336\335\377\377\000\000\000\000\266\000\000\000f\006\000\000\000\000\000\000(\375\377\377a\353\377\377\000\000\000\000\006\373\377\377\261\345\377\377\000\000\000\000r\374\377\377\336\335\377\377\000\000\000\000\000\000\000\000f\006\000\000\000\000\000\000\000\000\000\000\253\352\377\377\000\000\000\000\302\366\377\377\274\373\377\377\000\000\000\000J\377\377\377D\004\000\000\000\000\000\000\336\375\377\377\027\314\377\377\000\000\000\000\000\000\000\000\365\351\377\377\000\000\000\000\000\000\000\000\267\340\377\377\000\000\000\000J\377\377\377\372\004\000\000\000\000\000\000\000\000\000\000\310\321\377\377\000\000\000\000\000\000\000\000a\353\377\377\000\000\000\000\000\000\000\000\267\340\377\377\000\000\000\000\000\000\000\000\260\005\000\000\000\000\000\000\266\000\000\000\336\335\377\377\000\000\000\000\000\000\000\000\211\350\377\377\000\000\000\000\000\000\000\000\336\335\377\377\000\000\000\000\266\000\000\000f\006\000\000\000\000\000\000(\375\377\377a\353\377\377\000\000\000\000\006\373\377\377\261\345\377\377\000\000\000\000r\374\377\377\336\335\377\377\000\000\000\000\000\000\000\000D\004\000\000\000\000\000\000\000\000\000\000\253\352\377\377\000\000\000\000\302\366\377\377\274\373\377\377\000\000\000\000J\377\377\377D\004\000\000\000\000\000\000\336\375\377\377\310\361\377\377\000\000\000\000\000\000\000\000\302\366\377\377\000\000\000\000\000\000\000\000P\372\377\377\000\000\000\000J\377\377\377\372\004\000\000\000\000\000\000\000\000\000\000\022\361\377\377\000\000\000\000\000\000\000\000V\365\377\377\000\000\000\000\000\000\000\000P\372\377\377\000\000\000\000\000\000\000\000\260\005\000\000\000\000\000\000\266\000\000\000\022\361\377\377\000\000\000\000\000\000\000\000\240\364\377\377\000\000\000\000\000\000\000\000\232\371\377\377\000\000\000\000\266\000\000\000f\006\000\000\000\000\000\000(\375\377\377\310\361\377\377\000\000\000\000r\374\377\377V\365\377\377\000\000\000\000\336\375\377\377\232\371\377\377\000\000\000\000\000\000\000\000D\004\000\000\000\000\000\000\000\000\000\000\253\352\377\377\000\000\000\000\302\366\377\377\274\373\377\377\000\000\000\000J\377\377\377D\004\000\000\000\000\000\000\336\375\377\377\261\345\377\377\000\000\000\000\000\000\000\000\\\360\377\377\000\000\000\000\000\000\000\000g\346\377\377\000\000\000\000J\377\377\377\372\004\000\000\000\000\000\000\000\000\000\000\027\314\377\377\000\000\000\000\000\000\000\000a\353\377\377\000\000\000\000\000\000\000\000\267\340\377\377\000\000\000\000\000\000\000\000\260\005\000\000\000\000\000\000\266\000\000\000\336\335\377\377\000\000\000\000\000\000\000\000\211\350\377\377\000\000\000\000\000\000\000\000\336\335\377\377\000\000\000\000\266\000\000\000f\006\000\000\000\000\000\000(\375\377\377a\353\377\377\000\000\000\000\006\373\377\377\261\345\377\377\000\000\000\000r\374\377\377\336\335\377\377\000\000\000\000\000\000\000\000f\006\000\000\000\000\000\000\000\000\000\000r\334\377\377\000\000\000\000\302\366\377\377.\370\377\377\000\000\000\000J\377\377\377\216\003\000\000\000\000\000\000\336\375\377\377\302\366\377\377\000\000\000\000\000\000\000\000.\370\377\377\000\000\000\000\000\000\000\000\240\364\377\377\000\000\000\000J\377\377\377\372\004\000\000\000\000\000\000\000\000\000\000\211\310\377\377\000\000\000\000\000\000\000\0004\323\377\377\000\000\000\000\000\000\000\000\253\312\377\377\000\000\000\000\000\000\000\000\260\005\000\000\000\000\000\000\266\000\000\000\352\323\377\377\000\000\000\000\000\000\000\000\022\321\377\377\000\000\000\000\000\000\000\0009\316\377\377\000\000\000\000\266\000\000\000f\006\000\000\000\000\000\000(\375\377\377.\330\377\377\000\000\000\000\006\373\377\377\\\320\377\377\000\000\000\000r\374\377\377V\325\377\377(\375\377\377\"\002\000\000f\006\000\000\000\000\000\000\000\000\000\000\344\370\377\377\000\000\000\000\302\366\377\377(\375\377\377\000\000\000\000\224\376\377\377\274\373\377\377\000\000\000\000\336\375\377\377\310\321\377\377\000\000\000\000\216\003\000\000\261\345\377\377\000\000\000\000\000\000\000\000g\346\377\377\000\000\000\000J\377\377\377\372\004\000\000\000\000\000\000\000\000\000\000\027\314\377\377\000\000\000\000\000\000\000\000\211\350\377\377\000\000\000\000\000\000\000\000\344\330\377\377\000\000\000\000\000\000\000\000D\004\000\000\000\000\000\000\266\000\000\000\336\335\377\377\000\000\000\000\000\000\000\000\261\345\377\377\000\000\000\000\000\000\000\000\232\331\377\377\000\000\000\000\266\000\000\000f\006\000\000\000\000\000\000(\375\377\377g\346\377\377\000\000\000\000\006\373\377\377\217\343\377\377\000\000\000\000r\374\377\377\014\326\377\377\000\000\000\000\000\000\000\000f\006\000\000\000\000\000\000\000\000\000\000\253\352\377\377\240\364\377\377\302\366\377\377\224\376\377\377a\353\377\377\364\011\000\000r\374\377\377\000\000\000\000\336\375\377\377J\337\377\377\000\000\000\000\000\000\000\000\006\333\377\377\000\000\000\000\216\003\000\000\315\354\377\377\000\000\000\000J\377\377\377\"\002\000\000\000\000\000\000\000\000\000\000\027\314\377\377\000\000\000\000\000\000\000\000a\353\377\377\000\000\000\000l\001\000\000#\342\377\377\000\000\000\000J\377\377\377\260\005\000\000\000\000\000\000\266\000\000\000\336\335\377\377\000\000\000\000\000\000\000\000\217\343\377\377\000\000\000\000\266\000\000\000\373\344\377\377\000\000\000\000J\377\377\377f\006\000\000\000\000\000\000(\375\377\377a\353\377\377\000\000\000\000\006\373\377\377\261\345\377\377\000\000\000\000J\377\377\377\267\340\377\377\000\000\000\000\000\000\000\000f\006\000\000\203\355\377\377\000\000\000\000\224\336\377\377\000\000\000\000\302\366\377\377\274\373\377\377\323\347\377\377`\013\000\000\000\000\000\000\000\000\000\000\336\375\377\377V\325\377\377\000\000\000\000\034\007\000\000\000\300\377\377\000\000\000\000\000\000\000\0004\323\377\377\000\000\000\000\266\000\000\000\"\002\000\000\000\000\000\000\000\000\000\000x\327\377\377\000\000\000\000\000\000\000\000#\302\377\377\000\000\000\000\000\000\000\0009\316\377\377\000\000\000\000J\377\377\377\330\002\000\000\000\000\000\000\266\000\000\000\365\311\377\377\000\000\000\000\000\000\000\000\267\300\377\377\000\000\000\000l\001\000\000V\325\377\377\000\000\000\000J\377\377\377\330\002\000\000\000\000\000\000(\375\377\377P\332\377\377\000\000\000\000~\362\377\377\245\317\377\377\000\000\000\000(\375\377\377\331\342\377\377\000\000\000\000\000\000\000\000D\004\000\000\000\000\000\000\000\000\000\000\253\352\377\377\000\000\000\000\302\366\377\377\232\371\377\377\000\000\000\000r\374\377\377D\004\000\000\000\000\000\000\336\375\377\377\310\361\377\377\000\000\000\000\000\000\000\000\302\366\377\377\000\000\000\000\000\000\000\000P\372\377\377\000\000\000\000J\377\377\377f\006\000\000\000\000\000\000\000\000\000\000r\374\377\377\000\000\000\000\000\000\000\000V\365\377\377\000\000\000\000\000\000\000\000\006\373\377\377\000\000\000\000\000\000\000\000\034\007\000\000\000\000\000\000\266\000\000\000V\365\377\377\000\000\000\000\000\000\000\000\344\370\377\377\000\000\000\000\000\000\000\000\006\373\377\377\000\000\000\000\266\000\000\000\210\010\000\000\000\000\000\000(\375\377\377\352\363\377\377\000\000\000\000r\374\377\377\014\366\377\377\000\000\000\000\216\003\000\000P\372\377\377\336\375\377\377\216\003\000\000>\011";
static const uint32_t data_2B530 = SHEN_NAME('VM_0');
static const shen_name data_2B534[7] = {
  SHEN_NAME('VM_1'), SHEN_NAME('VM_2'), SHEN_NAME('VM_3'), SHEN_NAME('VM_4'), SHEN_NAME('VM_5'), SHEN_NAME('VM_6'), SHEN_NAME('VM_7')
};
static const char data_2B550[5] __attribute__((scn_string_literal)) = "SY11";
static const char data_2B555[9] __attribute__((scn_string_literal)) = "SY11A006";
static const char data_2B55E[9] __attribute__((scn_string_literal)) = "SY11A007";
static const char data_2B567[9] __attribute__((scn_string_literal)) = "SY11A008";
static const char data_2B570[9] __attribute__((scn_string_literal)) = "SY11A002";
static const char data_2B579[9] __attribute__((scn_string_literal)) = "SY11A003";
static const char data_2B582[9] __attribute__((scn_string_literal)) = "SY11A004";
static const char data_2B58B[9] __attribute__((scn_string_literal)) = "SY11A005";
static const char data_2B594[9] __attribute__((scn_string_literal)) = "SY11A001";
static const char data_2B59D[9] __attribute__((scn_string_literal)) = "SY11A009";
static const char data_2B5A6[9] __attribute__((scn_string_literal)) = "SY11A010";
static const char data_2B5AF[9] __attribute__((scn_string_literal)) = "SY11A011";
static const char data_2B5B8[9] __attribute__((scn_string_literal)) = "SY11A009";
static const char data_2B5C1[5] __attribute__((scn_string_literal)) = "SY11";
static const char data_2B5C6[5] __attribute__((scn_string_literal)) = "SY11";
static const char data_2B5CB[9] __attribute__((scn_string_literal)) = "SY11A001";
static const char data_2B5D4[5] __attribute__((scn_string_literal)) = "SY11";
static const char data_2B5D9[5] __attribute__((scn_string_literal)) = "SY11";
static const char data_2B5DE[9] __attribute__((scn_string_literal)) = "SY11A009";
static const char data_2B5E7[9] __attribute__((scn_string_literal)) = "SY11A010";
static const char data_2B5F0[9] __attribute__((scn_string_literal)) = "SY11A011";
static const char data_2B5F9[5] __attribute__((scn_string_literal)) = "SY11";
static const char data_2B5FE[18] __attribute__((scn_string_literal)) = "/scene/03/stream/";
static const char data_2B610[5] __attribute__((scn_string_literal)) = "%03d";
static const char data_2B615[] __attribute__((scn_string_literal)) = "ATRK";
static const uint8_t data_2B61A[2] = {0};
static Vector3 data_2B61C = {.x = 0.0f, .y = -0.1f, .z = 0.0f};
static Vector3 data_2B628 = {.x = 0.0f, .y = 0.0f, .z = 0.0f};
static const char data_2B634[] __attribute__((scn_string_literal)) = "VEND";
static const char data_2B639[] __attribute__((scn_string_literal)) = "VEND";
static const char data_2B63E[] __attribute__((scn_string_literal)) = "SPRT";
static const char data_2B643[7] __attribute__((scn_string_literal)) = "01JUCE";
static const char data_2B64A[13] __attribute__((scn_string_literal)) = "HMOT_CAN.BIN";
static const char data_2B657[] __attribute__((scn_string_literal)) = "VEND";
static const char data_2B65C[7] __attribute__((scn_string_literal)) = "01JUCE";
static const char data_2B663[26] __attribute__((scn_string_literal)) = "HandMot read by current!\012";
static const char data_2B67D[13] __attribute__((scn_string_literal)) = "HMOT_CAN.BIN";
static const char data_2B68A[7] __attribute__((scn_string_literal)) = "01JUCE";
static const char data_2B691[] __attribute__((scn_string_literal)) = "VEND";
static const uint8_t data_2B696[2] = {0};
static const uint8_t data_2B698[12] = {0};
static const char data_2B6A4[29] __attribute__((scn_string_literal)) = "vendmachine exit is EV_STOP\012";
static const char data_2B6C1[41] __attribute__((scn_string_literal)) = "vendmachine exit is MONEY_LOCK -> %d,%d\012";
static const char data_2B6EA[] __attribute__((scn_string_literal)) = "COKE";
static const char data_2B6EF[] __attribute__((scn_string_literal)) = "FATG";
static const char data_2B6F4[] __attribute__((scn_string_literal)) = "FATO";
static const char data_2B6F9[] __attribute__((scn_string_literal)) = "SPRT";
static const char data_2B6FE[] __attribute__((scn_string_literal)) = "CAFE";
static const uint8_t data_2B703[1] = {0};
static const uint32_t data_2B704 = 0x3D204C06;
static const float data_2B708[2] = {0.0f, 0.890018f};
static const uint8_t data_2B710[12] = {0};
static const uint8_t data_2B71C[12] = {0};
static const uint32_t data_2B728 = 0x3DE978D5;
static const float data_2B72C[2] = {0.002f, -0.036f};
static const uint32_t data_2B734 = 8192;
static const int32_t data_2B738[2] = {11377, 3640};
static const uint32_t data_2B740 = 0x3D204C06;
static const float data_2B744[2] = {0.0f, 0.890018f};
static const uint8_t data_2B74C[12] = {0};
static const uint32_t data_2B758 = 4369;
static const int32_t data_2B75C[2] = {0, -2184};
static const char data_2B764[] __attribute__((scn_string_literal)) = "01JUCEA011";
static const char data_2B76F[] __attribute__((scn_string_literal)) = "01JUCEA012";
static const char data_2B77A[] __attribute__((scn_string_literal)) = "01JUCEA013";
static const char data_2B785[] __attribute__((scn_string_literal)) = "01JUCEA002";
static const char data_2B790[] __attribute__((scn_string_literal)) = "01JUCEA003";
static const uint8_t data_2B79B[1] = {0};
static const uint32_t data_2B79C = 0x3DE978D5;
static const float data_2B7A0[2] = {0.002f, -0.036f};
static const uint32_t data_2B7A8 = 8192;
static const int32_t data_2B7AC[2] = {11377, 3640};
static const uint32_t data_2B7B4 = 0x3DCB04AB;
static const float data_2B7B8[2] = {0.02044f, -0.04322f};
static const uint32_t data_2B7C0 = 0xFFFFFE69;
static const int32_t data_2B7C4[2] = {3331, -3464};
static const uint32_t data_2B7CC = 0x3D204C06;
static const float data_2B7D0[2] = {0.0f, 0.890018f};
static const uint8_t data_2B7D8[12] = {0};
static const uint32_t data_2B7E4 = 4369;
static const int32_t data_2B7E8[2] = {0, -2184};
static const uint8_t data_2B7F0[12] = {0};
static const char data_2B7FC[] __attribute__((scn_string_literal)) = "01JUCEA002";
static const char data_2B807[] __attribute__((scn_string_literal)) = "01JUCEA003";
static const uint8_t pad_2B812[4] = {0};
static const uint8_t data_2B816[10] = {
  0x80, 0x3F, 0x00, 0x00, 0x80, 0x3F, 0x00, 0x00, 0x80, 0x3F
};
static const uint32_t data_2B820 = 0xC2480000;
static const float data_2B824[2] = {0.0f, 10.0f};
static const uint32_t data_2B82C = 0x41A00000;
static const float data_2B830[2] = {0.0f, 110.0f};
static const uint32_t data_2B838 = 0xC2041FF3;
static const float data_2B83C[2] = {0.0f, 52.1008f};
static const uint32_t data_2B844 = 0x41B3AA99;
static const uint32_t data_2B848[2] = {0, 0x420947FDU};
static const uint32_t data_2B850 = 0x42201A1D;
static const float data_2B854[2] = {0.0f, -2.1877f};
static const uint32_t data_2B85C = 0xC2BD9F2E;
static const uint32_t data_2B860[2] = {0, 0x428A24A9U};
static const uint32_t data_2B868 = 0x41D6BFB1;
static const uint32_t data_2B86C[2] = {0, 0x42CFC5AFU};
static const uint32_t data_2B874 = 0xC1A00000;
static const float data_2B878[2] = {0.0f, 110.0f};
static const uint32_t data_2B880 = 0xC19181A3;
static const uint32_t data_2B884[2] = {0, 0x421B20DFU};
static const uint32_t data_2B88C = 0xC2C00000;
static const float data_2B890[2] = {0.0f, 78.0f};
static const uint32_t data_2B898 = 0xC2880000;
static const float data_2B89C[2] = {0.0f, 53.0f};
static const uint32_t data_2B8A4 = 0xC1A00000;
static const float data_2B8A8[2] = {0.0f, 110.0f};
static const uint32_t data_2B8B0 = 0xC2041FF3;
static const float data_2B8B4[2] = {0.0f, 52.1008f};
static const uint32_t data_2B8BC = 0x3F4978D5;
static const float data_2B8C0[33] = {
  0.787f, 0.866f, 0.944f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.944f, 0.866f, 0.787f,
  0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f
};
static const uint8_t data_2B944[80] = {0};
typedef struct Row_2B994 { float w0; float w1; float w2; int32_t w3; } Row_2B994;
static const Row_2B994 data_2B994[3] = {
  {1.0f, 1.0f, 1.0f, 0},
  {0.0f, 0.0f, 0.0f, 0},
  {0.0f, 0.0f, 0.0f, 0}
};
static const int32_t data_2B9C4[2] = {0, 0};
static const uint8_t data_2B9CC[24] = {0};
static const float data_2B9E4[28] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f
};
static const uint8_t data_2BA54[24] = {0};
static const float data_2BA6C[28] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f
};
static const uint32_t data_2BADC = 0x3F4978D5;
static const float data_2BAE0[33] = {
  0.787f, 0.866f, 0.944f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  0.944f, 0.866f, 0.866f, 0.866f, 0.866f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f
};
static const uint8_t data_2BB64[24] = {0};
static const float data_2BB7C[28] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f
};
static const uint32_t data_2BBEC = 0x3F800000;
static const float data_2BBF0[33] = {
  0.944f, 0.866f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f,
  0.787f, 0.787f, 0.551f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint8_t data_2BC74[104] = {0};
static const float data_2BCDC[8] = {1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f};
static const uint8_t data_2BCFC[96] = {0};
static const float data_2BD5C[10] = {1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f};
static const uint32_t data_2BD84 = 0x3F266666;
static const float data_2BD88[33] = {
  0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f,
  0.708f, 0.787f, 0.866f, 0.944f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint32_t data_2BE0C = 0x3F800000;
static const float data_2BE10[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint32_t data_2BE94 = 0x3F800000;
static const float data_2BE98[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint32_t data_2BF1C = 0x3F800000;
static const float data_2BF20[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint32_t data_2BFA4 = 0xC2480000;
static const float data_2BFA8[2] = {0.0f, 10.0f};
static const uint32_t data_2BFB0 = 0x4214CCCD;
static const float data_2BFB4[2] = {0.0f, 6.6f};
static const uint32_t data_2BFBC = 0x41B5999A;
static const float data_2BFC0[2] = {0.0f, 27.2f};
static const uint32_t data_2BFC8 = 0x41B3999A;
static const float data_2BFCC[2] = {0.0f, 34.32f};
static const uint32_t data_2BFD4 = 0x4220147B;
static const float data_2BFD8[2] = {0.0f, -2.18f};
static const uint32_t data_2BFE0 = 0xC2BD9EB8;
static const float data_2BFE4[2] = {0.0f, 69.07f};
static const uint32_t data_2BFEC = 0x41D6B852;
static const float data_2BFF0[2] = {0.0f, 103.88f};
static const uint32_t data_2BFF8 = 0xC0966666;
static const float data_2BFFC[2] = {0.0f, 30.6f};
static const uint32_t data_2C004 = 0xC19170A4;
static const float data_2C008[2] = {0.0f, 38.78f};
static const uint32_t data_2C010 = 0xC2C00000;
static const float data_2C014[2] = {0.0f, 78.0f};
static const uint32_t data_2C01C = 0xC2840000;
static const float data_2C020[2] = {0.0f, 54.0f};
static const uint32_t data_2C028 = 0xC1FB0A3D;
static const float data_2C02C[2] = {0.0f, 40.1f};
static const uint32_t data_2C034 = 0xC2C28F5C;
static const float data_2C038[2] = {0.0f, 64.48f};
static const uint32_t data_2C040 = 0xC2F2FAE1;
static const float data_2C044[2] = {0.0f, 94.2f};
static const uint32_t data_2C04C = 0xC2BC75C3;
static const float data_2C050[2] = {0.0f, 85.91f};
static const uint32_t data_2C058 = 0x3F4978D5;
static const float data_2C05C[33] = {
  0.787f, 0.866f, 0.944f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.944f, 0.866f, 0.787f,
  0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f
};
static const uint8_t data_2C0E0[16] = {0};
static const float data_2C0F0[30] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint8_t data_2C168[16] = {0};
static const float data_2C178[30] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint8_t data_2C1F0[24] = {0};
static const float data_2C208[28] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f
};
static const uint32_t data_2C278 = 0x3F4978D5;
static const float data_2C27C[33] = {
  0.787f, 0.866f, 0.944f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  0.944f, 0.866f, 0.866f, 0.866f, 0.866f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f
};
static const uint8_t data_2C300[24] = {0};
static const float data_2C318[28] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f
};
static const uint32_t data_2C388 = 0x3F800000;
static const float data_2C38C[33] = {
  0.944f, 0.866f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f,
  0.787f, 0.787f, 0.551f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint8_t data_2C410[16] = {0};
static const float data_2C420[30] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint8_t data_2C498[96] = {0};
static const float data_2C4F8[10] = {1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f};
static const uint32_t data_2C520 = 0x3F266666;
static const float data_2C524[33] = {
  0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f,
  0.708f, 0.787f, 0.866f, 0.944f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint32_t data_2C5A8 = 0x3F800000;
static const float data_2C5AC[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint8_t data_2C630[16] = {0};
static const float data_2C640[30] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint8_t data_2C6B8[80] = {0};
static const float data_2C708[14] = {1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f};
static const uint8_t data_2C740[80] = {0};
static const float data_2C790[14] = {1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f};
static const uint8_t data_2C7C8[80] = {0};
static const float data_2C818[14] = {1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f};
static const uint32_t data_2C850 = 0xC2480000;
static const float data_2C854[2] = {0.0f, 10.0f};
static const uint32_t data_2C85C = 0x41E80000;
static const shen_name data_2C860[2] = {0, SHEN_NAME('333A')};
static const uint32_t data_2C868 = 0xC2041FF3;
static const float data_2C86C[2] = {0.0f, 52.1008f};
static const uint32_t data_2C874 = 0x41B3AA99;
static const uint32_t data_2C878[2] = {0, 0x420947FDU};
static const uint32_t data_2C880 = 0x42201A1D;
static const float data_2C884[2] = {0.0f, -2.1877f};
static const uint32_t data_2C88C = 0xC2BD9F2E;
static const uint32_t data_2C890[2] = {0, 0x428A24A9U};
static const uint32_t data_2C898 = 0x41D6BFB1;
static const uint32_t data_2C89C[2] = {0, 0x42CFC5AFU};
static const uint32_t data_2C8A4 = 0xC1A00000;
static const float data_2C8A8[2] = {0.0f, 110.0f};
static const uint32_t data_2C8B0 = 0xC19181A3;
static const uint32_t data_2C8B4[2] = {0, 0x421B20DFU};
static const uint32_t data_2C8BC = 0xC2C00000;
static const float data_2C8C0[2] = {0.0f, 78.0f};
static const uint32_t data_2C8C8 = 0xC2880000;
static const float data_2C8CC[2] = {0.0f, 53.0f};
static const uint32_t data_2C8D4 = 0xC0B9999A;
static const float data_2C8D8[2] = {0.0f, 26.2f};
static const uint32_t data_2C8E0 = 0xC22E0000;
static const float data_2C8E4[2] = {0.0f, 46.0f};
static const uint32_t data_2C8EC = 0xC2CE0000;
static const float data_2C8F0[2] = {0.0f, 80.0f};
static const uint32_t data_2C8F8 = 0xC2041FF3;
static const float data_2C8FC[2] = {0.0f, 52.1008f};
static const uint32_t data_2C904 = 0x3F4978D5;
static const float data_2C908[33] = {
  0.787f, 0.866f, 0.944f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.944f, 0.866f, 0.787f,
  0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f
};
static const uint8_t data_2C98C[16] = {0};
static const float data_2C99C[30] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint8_t data_2CA14[24] = {0};
static const float data_2CA2C[28] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f
};
static const uint8_t data_2CA9C[24] = {0};
static const float data_2CAB4[28] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f
};
static const uint32_t data_2CB24 = 0x3F4978D5;
static const float data_2CB28[33] = {
  0.787f, 0.866f, 0.944f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  0.944f, 0.866f, 0.866f, 0.866f, 0.866f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f
};
static const uint8_t data_2CBAC[24] = {0};
static const float data_2CBC4[28] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f
};
static const uint32_t data_2CC34 = 0x3F800000;
static const float data_2CC38[33] = {
  0.944f, 0.866f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f,
  0.787f, 0.787f, 0.551f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint8_t data_2CCBC[104] = {0};
static const float data_2CD24[8] = {1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f};
static const uint8_t data_2CD44[96] = {0};
static const float data_2CDA4[10] = {1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f};
static const uint32_t data_2CDCC = 0x3F266666;
static const float data_2CDD0[33] = {
  0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f,
  0.708f, 0.787f, 0.866f, 0.944f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint32_t data_2CE54 = 0x3F800000;
static const float data_2CE58[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint8_t data_2CEDC[16] = {0};
static const float data_2CEEC[30] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint8_t data_2CF64[16] = {0};
static const float data_2CF74[30] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint8_t data_2CFEC[80] = {0};
static const float data_2D03C[14] = {1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f};
static const uint32_t data_2D074 = 0x3F800000;
static const float data_2D078[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint32_t data_2D0FC = 0xC2480000;
static const float data_2D100[2] = {0.0f, 10.0f};
static const uint32_t data_2D108 = 0x41A00000;
static const float data_2D10C[2] = {0.0f, 110.0f};
static const uint32_t data_2D114 = 0xC2041FF3;
static const float data_2D118[2] = {0.0f, 52.1008f};
static const uint32_t data_2D120 = 0x41B3AA99;
static const uint32_t data_2D124[2] = {0, 0x420947FDU};
static const uint32_t data_2D12C = 0x42201A1D;
static const float data_2D130[2] = {0.0f, -2.1877f};
static const uint32_t data_2D138 = 0xC2BD9F2E;
static const uint32_t data_2D13C[2] = {0, 0x428A24A9U};
static const uint32_t data_2D144 = 0x41D6BFB1;
static const uint32_t data_2D148[2] = {0, 0x42CFC5AFU};
static const uint32_t data_2D150 = 0xC1A00000;
static const float data_2D154[2] = {0.0f, 110.0f};
static const uint32_t data_2D15C = 0xC19181A3;
static const uint32_t data_2D160[2] = {0, 0x421B20DFU};
static const uint32_t data_2D168 = 0xC2C00000;
static const float data_2D16C[2] = {0.0f, 78.0f};
static const uint32_t data_2D174 = 0xC2880000;
static const float data_2D178[2] = {0.0f, 53.0f};
static const uint32_t data_2D180 = 0x40A66666;
static const float data_2D184[2] = {0.0f, 27.0f};
static const uint32_t data_2D18C = 0xC201999A;
static const float data_2D190[2] = {0.0f, 44.5f};
static const uint32_t data_2D198 = 0xC2CE0000;
static const float data_2D19C[2] = {0.0f, 80.0f};
static const uint32_t data_2D1A4 = 0xC2041FF3;
static const float data_2D1A8[2] = {0.0f, 52.1008f};
static const uint32_t data_2D1B0 = 0x3F4978D5;
static const float data_2D1B4[33] = {
  0.787f, 0.866f, 0.944f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.944f, 0.866f, 0.787f,
  0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f
};
static const uint8_t data_2D238[80] = {0};
typedef struct Row_2D288 { float w0; float w1; float w2; int32_t w3; } Row_2D288;
static const Row_2D288 data_2D288[3] = {
  {1.0f, 1.0f, 1.0f, 0},
  {0.0f, 0.0f, 0.0f, 0},
  {0.0f, 0.0f, 0.0f, 0}
};
static const int32_t data_2D2B8[2] = {0, 0};
static const uint8_t data_2D2C0[24] = {0};
static const float data_2D2D8[28] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f
};
static const uint8_t data_2D348[24] = {0};
static const float data_2D360[28] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f
};
static const uint32_t data_2D3D0 = 0x3F4978D5;
static const float data_2D3D4[33] = {
  0.787f, 0.866f, 0.944f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  0.944f, 0.866f, 0.866f, 0.866f, 0.866f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f
};
static const uint8_t data_2D458[24] = {0};
static const float data_2D470[28] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f
};
static const uint32_t data_2D4E0 = 0x3F800000;
static const float data_2D4E4[33] = {
  0.944f, 0.866f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f,
  0.787f, 0.787f, 0.551f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint8_t data_2D568[104] = {0};
static const float data_2D5D0[8] = {1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f};
static const uint8_t data_2D5F0[96] = {0};
static const float data_2D650[10] = {1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f};
static const uint32_t data_2D678 = 0x3F266666;
static const float data_2D67C[33] = {
  0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f,
  0.708f, 0.787f, 0.866f, 0.944f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint32_t data_2D700 = 0x3F800000;
static const float data_2D704[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint8_t data_2D788[16] = {0};
static const float data_2D798[30] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint8_t data_2D810[16] = {0};
static const float data_2D820[30] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint8_t data_2D898[80] = {0};
static const float data_2D8E8[14] = {1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f};
static const uint32_t data_2D920 = 0x3F800000;
static const float data_2D924[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const int32_t data_2D9A8[57] = {
  0, 0, -10977, 0, 0, -4842, 0, 0, 0, 0, 0, -4531, 0, 0, -1820, 0, 0, -3877, 0, 0, 0, 0, 0, -8574, 0, 0, -16074, 0, 0, -4642, 0, 0, 0, 0, 0, -8610, 0,
  182, -16966, 0, 0, -6407, 0, 36, -54, 0, 0, -11450, 0, 0, -13562, 0, 0, -8974, 0, -819, -345
};
static const int32_t data_2DA8C[57] = {
  0, 0, -6005, 0, 0, -909, 0, 0, 0, 0, 0, -6368, 0, 1820, -8552, 0, 0, -4548, 0, 0, 0, 0, 0, -8552, 0, 0, -10372, 0, -545, -4002, 0, 0, 0, 0, 0,
  -7823, 0, 0, -12374, 0, -909, -2182, 0, 0, 0, 0, -1636, -10735, 0, -2001, -10372, 0, -1819, -727, 0, 0, 0
};
static const Vector4 data_2DB70 = {.x = 1.0f, .y = 0.3f, .z = 0.3f, .w = 0.3f};
static const Vector3 data_2DB80 = {.x = 38.2248f, .y = -10.0f, .z = 10.056f};
static const FogColourData data_2DB8C = {.red = 20.0f, .green = 800.0f, .blue = 1.0f, .colour = 0xFF000020U};
static const Vector3 data_2DB9C = {.x = -27.532101f, .y = 9.2975f, .z = 49.022202f};
static const Vector3 data_2DBA8 = {.x = -77.909798f, .y = 7.1224f, .z = 64.488998f};
static const Vector3 data_2DBB4 = {.x = 29.400999f, .y = 3.7988f, .z = 23.5072f};
static const Vector3 data_2DBC0 = {.x = 23.9245f, .y = 3.7983f, .z = 21.183201f};
static const Vector3 data_2DBCC = {.x = -72.2313f, .y = 3.7998f, .z = 60.132702f};
static const Vector3 data_2DBD8 = {.x = -7.4819f, .y = 3.7965f, .z = 32.121201f};
static const Vector3i data_2DBE4 = {.x = 0, .y = 32768, .z = 0};
static const Vector3i data_2DBF0 = {.x = 0, .y = -32768, .z = 0};
static const Vector3i data_2DBFC = {.x = 0, .y = -21353, .z = 0};
static const Vector3i data_2DC08 = {.x = 0, .y = 28061, .z = 0};
static const Vector3i data_2DC14 = {.x = 0, .y = 19660, .z = 0};
static const Vector3i data_2DC20 = {.x = 0, .y = -27925, .z = 0};
typedef struct Row_2DC2C { uint32_t w0; float w1; float w2; } Row_2DC2C;
static const Row_2DC2C data_2DC2C[2] = {
  {0x42189B2DU, 0.90358f, 10.08923f},
  {0x4218DC1CU, 0.90821f, 10.06523f}
};
static const Vector3 data_2DC44 = {.x = 0.0f, .y = -10.0f, .z = 100.0f};
static const int32_t data_2DC50[6] = {0, -24386, 0, 0, -19859, 0};
static const Vector3 data_2DC68 = {.x = 38.161098f, .y = 0.972f, .z = 10.1892f};
static const Vector3i data_2DC74 = {.x = 0, .y = 11666, .z = 0};
static const Vector3 data_2DC80 = {.x = 38.2248f, .y = 0.9712f, .z = 10.056f};
static const Vector3i data_2DC8C = {.x = 0, .y = 44220, .z = 0};
static const Vector3i data_2DC98 = {.x = 10740, .y = -910, .z = -10012};
static const Vector3i data_2DCA4 = {.x = -4915, .y = -1820, .z = 3640};
static const uint16_t data_2DCB0[8] = {1068, 1043, 1069, 1071, 1074, 1072, 1073, 903};
static const char data_2DCC0[20] __attribute__((scn_string_literal)) = "TUNED FreeTalk def\012";
static const char data_2DCD4[20] __attribute__((scn_string_literal)) = "TUNED FreeTalk def\012";
static DoorMotionSelectorBlock data_2DCE8[38] = {
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
static ScnDoorMotionDescriptor data_2E2D8[72] = {
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
static uint32_t data_2ECF8 = 0xFFFFFFFF;
typedef struct Row_2ECFC { float w0; float w1; float w2; float w3; int32_t w4; } Row_2ECFC;
static const Row_2ECFC data_2ECFC[7] = {
  {0.0f, 0.0f, 0.0f, 0.0f, -1},
  {0.0f, 0.0f, 0.0f, 0.0f, -1},
  {0.0f, 0.0f, 0.0f, 0.0f, 1534},
  {26.0f, 55.0f, 45.0f, 75.0f, 1555},
  {61.0f, 146.0f, 45.0f, 50.0f, 1554},
  {59.0f, 124.0f, 45.0f, 70.0f, 1543},
  {33.0f, 49.0f, 45.0f, 50.0f, 1543}
};
static const float data_2ED88[4] = {33.0f, 49.0f, 50.0f, 50.0f};
static void (*data_2ED98)(int32_t, int32_t, int32_t) = script_7FE4;
static void (*const data_2ED9C)(int32_t, int32_t, int32_t, int32_t) = script_7FF4;
static void (*const data_2EDA0)(int32_t, int32_t, int32_t, int32_t) = script_7FF4;
static void (*const data_2EDA4)(int32_t, int32_t, int32_t, int32_t) = script_7FF4;
static void (*const data_2EDA8)(int32_t, int32_t, int32_t, int32_t) = script_7FF4;
static void (*const data_2EDAC)(int32_t, int32_t, int32_t, int32_t) = script_7FF4;
static void (*const data_2EDB0)(int32_t, int32_t, int32_t, int32_t) = script_7FF4;
static void (*const data_2EDB4)(int32_t, int32_t, int32_t, int32_t) = script_7FF4;
static void (*const data_2EDB8)(int32_t, int32_t, int32_t, int32_t) = script_7FF4;
static void (*const data_2EDBC)(int32_t, int32_t, int32_t, int32_t) = script_829A;
static void (*const data_2EDC0)(int32_t, int32_t, int32_t, int32_t) = script_83FB;
static void (*const data_2EDC4)(int32_t, int32_t, int32_t, int32_t) = script_88C8;
static void (*const data_2EDC8)(int32_t, int32_t, int32_t, int32_t) = script_892F;
static void (*const data_2EDCC)(int32_t, int32_t, int32_t, int32_t) = script_80F8;
static void (*const data_2EDD0)(int32_t, int32_t, int32_t, int32_t) = script_80F8;
static void (*const data_2EDD4)(int32_t, int32_t, int32_t, int32_t) = script_855C;
static void (*const data_2EDD8)(int32_t, int32_t, int32_t, int32_t) = script_87FA;
static void (*const data_2EDDC)(int32_t, int32_t, int32_t, int32_t) = script_8996;
static void (*const data_2EDE0)(int32_t, int32_t, int32_t, int32_t) = script_829A;
static void (*const data_2EDE4)(int32_t, int32_t, int32_t, int32_t) = script_83FB;
static void (*const data_2EDE8)(int32_t, int32_t, int32_t, int32_t) = script_855C;
static void (*const data_2EDEC)(int32_t, int32_t, int32_t, int32_t) = script_829A;
static void (*const data_2EDF0)(int32_t, int32_t, int32_t, int32_t) = script_83FB;
static void (*const data_2EDF4)(int32_t, int32_t, int32_t, int32_t) = script_855C;
static void (*const data_2EDF8)(int32_t, int32_t, int32_t, int32_t) = script_89DD;
static void (*const data_2EDFC)(int32_t, int32_t, int32_t, int32_t) = script_89DD;
static void (*const data_2EE00)(int32_t, int32_t, int32_t, int32_t) = script_89DD;
static void (*const data_2EE04)(int32_t, int32_t, int32_t, int32_t) = script_89DD;
static void (*const data_2EE08)(int32_t, int32_t, int32_t, int32_t) = script_8B13;
static void (*const data_2EE0C)(int32_t, int32_t, int32_t, int32_t) = script_93D4;
static void (*const data_2EE10)(int32_t, int32_t, int32_t, int32_t) = script_7FF4;
static void (*const data_2EE14)(int32_t, int32_t, int32_t, int32_t) = script_7FF4;
static void (*const data_2EE18)(int32_t, int32_t, int32_t, int32_t) = script_7FF4;
static void (*const data_2EE1C)(int32_t, int32_t, int32_t, int32_t) = script_7FF4;
static void (*const data_2EE20)(int32_t, int32_t, int32_t, int32_t) = script_7FF4;
static void (*const data_2EE24)(int32_t, int32_t, int32_t, int32_t) = script_7FF4;
static void (*const data_2EE28)(int32_t, int32_t, int32_t, int32_t) = script_7FF4;
static void (*const data_2EE2C)(int32_t, int32_t, int32_t, int32_t) = script_7FF4;
static const char data_2EE30[46] __attribute__((scn_string_literal)) = "Door Index = %d : call_type = %d : type = %d\012";
static const char data_2EE5E[43] __attribute__((scn_string_literal)) = "Door Open Motion = DEB_AKESIME_FUSUMA_L_F\012";
static const char data_2EE89[43] __attribute__((scn_string_literal)) = "Door Open Motion = DEB_AKESIME_FUSUMA_R_F\012";
static const char data_2EEB4[42] __attribute__((scn_string_literal)) = "Door Open Motion = DEB_HIKISIME_DOOR_L_F\012";
static const char data_2EEDE[42] __attribute__((scn_string_literal)) = "Door Open Motion = DEB_HIKISIME_DOOR_R_F\012";
static const char data_2EF08[41] __attribute__((scn_string_literal)) = "Door Open Motion = DEB_OSISIME_DOOR_L_F\012";
static const char data_2EF31[41] __attribute__((scn_string_literal)) = "Door Open Motion = DEB_OSISIME_DOOR_R_F\012";
static const char data_2EF5A[43] __attribute__((scn_string_literal)) = "Door Open Motion = GAK_AKESIME_FUSUMA_L_F\012";
static const char data_2EF85[43] __attribute__((scn_string_literal)) = "Door Open Motion = GAK_AKESIME_FUSUMA_R_F\012";
static const char data_2EFB0[42] __attribute__((scn_string_literal)) = "Door Open Motion = GAK_HIKISIME_DOOR_L_F\012";
static const char data_2EFDA[42] __attribute__((scn_string_literal)) = "Door Open Motion = GAK_HIKISIME_DOOR_R_F\012";
static const char data_2F004[41] __attribute__((scn_string_literal)) = "Door Open Motion = GAK_OSISIME_DOOR_L_F\012";
static const char data_2F02D[41] __attribute__((scn_string_literal)) = "Door Open Motion = GAK_OSISIME_DOOR_R_F\012";
static const char data_2F056[43] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_AKESIME_FUSUMA_L_F\012";
static const char data_2F081[43] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_AKESIME_FUSUMA_R_F\012";
static const char data_2F0AC[42] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_HIKISIME_DOOR_L_F\012";
static const char data_2F0D6[42] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_HIKISIME_DOOR_R_F\012";
static const char data_2F100[41] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_OSISIME_DOOR_L_F\012";
static const char data_2F129[41] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_OSISIME_DOOR_R_F\012";
static const char data_2F152[43] __attribute__((scn_string_literal)) = "Door Open Motion = KOD_AKESIME_FUSUMA_L_F\012";
static const char data_2F17D[43] __attribute__((scn_string_literal)) = "Door Open Motion = KOD_AKESIME_FUSUMA_R_F\012";
static const char data_2F1A8[42] __attribute__((scn_string_literal)) = "Door Open Motion = KOD_HIKISIME_DOOR_L_F\012";
static const char data_2F1D2[42] __attribute__((scn_string_literal)) = "Door Open Motion = KOD_HIKISIME_DOOR_R_F\012";
static const char data_2F1FC[41] __attribute__((scn_string_literal)) = "Door Open Motion = KOD_OSISIME_DOOR_L_F\012";
static const char data_2F225[41] __attribute__((scn_string_literal)) = "Door Open Motion = KOD_OSISIME_DOOR_R_F\012";
static const char data_2F24E[43] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_AKESIME_FUSUMA_L_F\012";
static const char data_2F279[43] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_AKESIME_FUSUMA_R_F\012";
static const char data_2F2A4[42] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_HIKISIME_DOOR_L_F\012";
static const char data_2F2CE[42] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_HIKISIME_DOOR_R_F\012";
static const char data_2F2F8[41] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_OSISIME_DOOR_L_F\012";
static const char data_2F321[41] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_OSISIME_DOOR_R_F\012";
static const char data_2F34A[43] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_AKESIME_FUSUMA_L_F\012";
static const char data_2F375[43] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_AKESIME_FUSUMA_R_F\012";
static const char data_2F3A0[42] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_HIKISIME_DOOR_L_F\012";
static const char data_2F3CA[42] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_HIKISIME_DOOR_R_F\012";
static const char data_2F3F4[41] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_OSISIME_DOOR_L_F\012";
static const char data_2F41D[41] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_OSISIME_DOOR_R_F\012";
static const char data_2F446[43] __attribute__((scn_string_literal)) = "Door Open Motion = SIN_AKESIME_FUSUMA_L_F\012";
static const char data_2F471[43] __attribute__((scn_string_literal)) = "Door Open Motion = SIN_AKESIME_FUSUMA_R_F\012";
static const char data_2F49C[42] __attribute__((scn_string_literal)) = "Door Open Motion = SIN_HIKISIME_DOOR_L_F\012";
static const char data_2F4C6[42] __attribute__((scn_string_literal)) = "Door Open Motion = SIN_HIKISIME_DOOR_R_F\012";
static const char data_2F4F0[41] __attribute__((scn_string_literal)) = "Door Open Motion = SIN_OSISIME_DOOR_L_F\012";
static const char data_2F519[41] __attribute__((scn_string_literal)) = "Door Open Motion = SIN_OSISIME_DOOR_R_F\012";
static const char data_2F542[43] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_AKESIME_FUSUMA_L_F\012";
static const char data_2F56D[43] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_AKESIME_FUSUMA_R_F\012";
static const char data_2F598[42] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_HIKISIME_DOOR_L_F\012";
static const char data_2F5C2[42] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_HIKISIME_DOOR_R_F\012";
static const char data_2F5EC[41] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_OSISIME_DOOR_L_F\012";
static const char data_2F615[41] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_OSISIME_DOOR_R_F\012";
static const char data_2F63E[43] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_AKESIME_FUSUMA_L_F\012";
static const char data_2F669[43] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_AKESIME_FUSUMA_R_F\012";
static const char data_2F694[42] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_HIKISIME_DOOR_L_F\012";
static const char data_2F6BE[42] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_HIKISIME_DOOR_R_F\012";
static const char data_2F6E8[41] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_OSISIME_DOOR_L_F\012";
static const char data_2F711[41] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_OSISIME_DOOR_R_F\012";
static const char data_2F73A[44] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_OSISIME_DOOR_AP_L_F\012";
static const char data_2F766[44] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_OSISIME_DOOR_AP_L_F\012";
static const char data_2F792[44] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_OSISIME_DOOR_AP_L_F\012";
static const char data_2F7BE[44] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_OSISIME_DOOR_AP_L_F\012";
static const char data_2F7EA[44] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_OSISIME_DOOR_AP_L_F\012";
static const char data_2F816[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_2F832[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_2F84E[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_2F86A[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_2F886[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_2F8A2[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_2F8BE[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_2F8DA[13] __attribute__((scn_string_literal)) = "open-move!!\012";
static const char data_2F8E7[11] __attribute__((scn_string_literal)) = "M_MDOR.BIN";
static const char data_2F8F2[] __attribute__((scn_string_literal)) = "This Charactor Work is NULL!!\n";
static const char data_2F911[22] __attribute__((scn_string_literal)) = "手動ドア開け番号指定\012";
static const char data_2F927[34] __attribute__((scn_string_literal)) = "無限ループになりそうなので抜ける\012";
static const char data_2F949[] __attribute__((scn_string_literal)) = "Don't make BOX Buffer!!\n";
static const char data_2F962[] __attribute__((scn_string_literal)) = "Don't make BOX Buffer!!\n";
static const char data_2F97B[15] __attribute__((scn_string_literal)) = "Door System : ";
static const char data_2F98A[15] __attribute__((scn_string_literal)) = "Door System : ";
static const char data_2F999[18] __attribute__((scn_string_literal)) = "Door Box is ON!!\012";
static const char data_2F9AB[19] __attribute__((scn_string_literal)) = "Door Box is OFF!!\012";
static const char data_2F9BE[18] __attribute__((scn_string_literal)) = "Door Box is ON!!\012";
static const char data_2F9D0[20] __attribute__((scn_string_literal)) = "Door Box is OFF!!\012";
static const DSWKDoorEntry data_2F9E4[65] = {
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
static const int32_t data_30718[13] = {-1, 0, 6553700, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1};
static const uint32_t data_3074C = 98;
static const int32_t data_30750[184] = {
  99, -1, 78, 79, -1, 90, -1, 40, 41, -1, 60, 61, -1, 62, 63, -1, 64, 65, -1, 66, 67, -1, 68, 69, -1, 70, 71, -1, 72, 73, -1, 74, 75, -1, 42, 43, -1,
  44, 45, -1, 46, 47, -1, 48, 49, -1, 50, 51, -1, 86, 87, -1, 52, 53, -1, 54, 55, -1, 80, 81, -1, 56, 57, -1, 84, 85, -1, 58, 59, -1, 82, 83, -1, 21,
  22, -1, 19, 20, -1, 97, -1, 9, 10, -1, 91, -1, 5, 6, -1, 36, 37, -1, 33, 34, -1, 76, 77, -1, 116, 117, -1, 35, -1, 4, -1, 7, 8, -1, 32, -1, 106,
  107, -1, 30, 31, -1, 2, 3, -1, 92, 93, -1, 94, -1, 17, 18, -1, 15, 16, -1, 13, 14, -1, 11, 12, -1, 29, -1, 108, 109, -1, 110, 111, -1, 102, 103, -1,
  118, 119, -1, 0, 1, -1, 112, 113, -1, 95, 96, -1, 100, 101, -1, 25, 26, -1, 27, -1, 114, 115, -1, 38, 39, -1, 104, 105, -1, 23, 24, -1, 88, 89, -1,
  28, -1
};
static char data_30A30[] = "DR01_011";
static char data_30A39[] = "DR00_002";
static char data_30A42[] = "DR05_001";
static char data_30A4B[] = "DR02_012";
static char data_30A54[] = "DR02_016";
static char data_30A5D[] = "DR00_003";
static char data_30A66[] = "DR02_014";
static char data_30A6F[] = "DR15_013";
static char data_30A78[] = "DR01_006";
static char data_30A81[] = "DR01_007";
static char data_30A8A[] = "DR13_010";
static char data_30A93[] = "DR15_009";
static char data_30A9C[] = "DR01_010";
static char data_30AA5[] = "DR02_018";
static char data_30AAE[] = "DR17_002";
static char data_30AB7[] = "DR13_009";
static char data_30AC0[] = "DR06_007";
static char data_30AC9[] = "DR06_008";
static char data_30AD2[] = "DR15_011";
static char data_30ADB[] = "DR02_013";
static char data_30AE4[] = "DR13_012";
static char data_30AED[] = "DR01_018";
static char data_30AF6[] = "DR13_008";
static char data_30AFF[] = "DR02_017";
static char data_30B08[] = "DR13_007";
static char data_30B11[] = "DR12_001";
static char data_30B1A[] = "DR15_014";
static char data_30B23[] = "DR28_001";
static char data_30B2C[] = "DR15_010";
static char data_30B35[] = "DR02_019";
static char data_30B3E[] = "DR02_020";
static char data_30B47[] = "DR01_008";
static char data_30B50[] = "DR01_009";
static char data_30B59[] = "DR15_012";
static char data_30B62[] = "DR02_015";
static char data_30B6B[] = "DR13_013";
static char *const data_30B74 = data_30A30;
static const int32_t data_30B78[3] = {1, 1311403, 1376939};
static char *const data_30B84 = data_30A39;
static const int32_t data_30B88[3] = {0, -1, -1};
static char *const data_30B94 = data_30A42;
static const int32_t data_30B98[3] = {5, 1442475, 1508011};
static char *const data_30BA4 = data_30A4B;
static const int32_t data_30BA8[3] = {2, 1573547, 1639083};
static char *const data_30BB4 = data_30A54;
static const int32_t data_30BB8[3] = {2, 1573547, 1639083};
static char *const data_30BC4 = data_30A5D;
static const int32_t data_30BC8[3] = {0, -1, -1};
static char *const data_30BD4 = data_30A66;
static const int32_t data_30BD8[3] = {2, 1573547, 1639083};
static char *const data_30BE4 = data_30A6F;
static const int32_t data_30BE8[3] = {15, 1180331, 1245867};
static char *const data_30BF4 = data_30A78;
static const int32_t data_30BF8[3] = {1, 1311403, 1376939};
static char *const data_30C04 = data_30A81;
static const int32_t data_30C08[3] = {36, 1311403, 1376939};
static char *const data_30C14 = data_30A8A;
static const int32_t data_30C18[3] = {13, 1442475, 1508011};
static char *const data_30C24 = data_30A93;
static const int32_t data_30C28[3] = {15, 1180331, 1245867};
static char *const data_30C34 = data_30A9C;
static const int32_t data_30C38[3] = {1, 1311403, 1376939};
static char *const data_30C44 = data_30AA5;
static const int32_t data_30C48[3] = {2, 1573547, 1639083};
static char *const data_30C54 = data_30AAE;
static const int32_t data_30C58[3] = {17, 656043, 721579};
static char *const data_30C64 = data_30AB7;
static const int32_t data_30C68[3] = {13, 1442475, 1508011};
static char *const data_30C74 = data_30AC0;
static const int32_t data_30C78[3] = {6, 1442475, 1508011};
static char *const data_30C84 = data_30AC9;
static const int32_t data_30C88[3] = {6, 1442475, 1508011};
static char *const data_30C94 = data_30AD2;
static const int32_t data_30C98[3] = {15, 1180331, 1245867};
static char *const data_30CA4 = data_30ADB;
static const int32_t data_30CA8[3] = {2, 1573547, 1639083};
static char *const data_30CB4 = data_30AE4;
static const int32_t data_30CB8[3] = {13, 1442475, 1508011};
static char *const data_30CC4 = data_30AED;
static const int32_t data_30CC8[3] = {36, 1311403, 1376939};
static char *const data_30CD4 = data_30AF6;
static const int32_t data_30CD8[3] = {13, 1442475, 1508011};
static char *const data_30CE4 = data_30AFF;
static const int32_t data_30CE8[3] = {2, 1573547, 1639083};
static char *const data_30CF4 = data_30B08;
static const int32_t data_30CF8[3] = {13, 1442475, 1508011};
static char *const data_30D04 = data_30B11;
static const int32_t data_30D08[3] = {12, 656043, 721579};
static char *const data_30D14 = data_30B1A;
static const int32_t data_30D18[3] = {15, 1180331, 1245867};
static char *const data_30D24 = data_30B23;
static const int32_t data_30D28[3] = {28, 1049259, 1114795};
static char *const data_30D34 = data_30B2C;
static const int32_t data_30D38[3] = {15, 1180331, 1245867};
static char *const data_30D44 = data_30B35;
static const int32_t data_30D48[3] = {2, 1573547, 1639083};
static char *const data_30D54 = data_30B3E;
static const int32_t data_30D58[3] = {2, 1573547, 1639083};
static char *const data_30D64 = data_30B47;
static const int32_t data_30D68[3] = {1, 1311403, 1376939};
static char *const data_30D74 = data_30B50;
static const int32_t data_30D78[3] = {1, 1311403, 1376939};
static char *const data_30D84 = data_30B59;
static const int32_t data_30D88[3] = {15, 1180331, 1245867};
static char *const data_30D94 = data_30B62;
static const int32_t data_30D98[3] = {2, 1573547, 1639083};
static char *const data_30DA4 = data_30B6B;
static const int32_t data_30DA8[3] = {13, 1442475, 1508011};
static const MapObjectRecord data_30DB4[120] = {
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
static const int32_t data_31E94[57] = {
  0, 0, -8046, 0, 0, -1820, -13088, 4350, -2894, 0, 0, -9011, 0, 0, -18422, 0, 0, -9812, 0, 0, 0, 0, 0, -7518, 0, 0, -17385, 0, 0, -11559, 0, 0, 0, 0,
  0, -5734, 0, 0, -18058, 0, 0, -12233, 0, 0, 0, 0, 0, -7827, 0, 0, -16183, 0, 0, -13981, 0, -72, 0
};
static const int32_t data_31F78[57] = {
  0, 0, -2712, 0, 0, -691, 0, 0, 0, 0, 0, -1092, 0, 0, -1438, 0, 0, -2748, 0, 0, 0, 0, 0, 364, 0, 0, -4132, 0, 0, -3531, 0, 0, 0, 0, 0, -2020, 0, 0,
  -6881, 0, 0, -5333, 0, 109, -200, 0, 0, -3986, 0, 0, -8829, 0, 0, -7245, 0, -764, -837
};
static const uint32_t data_3205C = 0x303131;
static const uint32_t data_32060 = 0x393131;
static const uint32_t data_32064 = 0x373731;
static const uint32_t data_32068 = 0x343031;
static const char data_3206C[] __attribute__((scn_string_literal)) = "61564";
static const char data_32072[] __attribute__((scn_string_literal)) = "37742";
static const char data_32078[] __attribute__((scn_string_literal)) = "37749";
static const char data_3207E[] __attribute__((scn_string_literal)) = "22550";
static const char data_32084[] __attribute__((scn_string_literal)) = "046861564";
static const char data_3208E[] __attribute__((scn_string_literal)) = "046837742";
static const char data_32098[] __attribute__((scn_string_literal)) = "046837749";
static const char data_320A2[] __attribute__((scn_string_literal)) = "046822550";
static const char data_320AC[] __attribute__((scn_string_literal)) = "2?????";
static const char data_320B3[] __attribute__((scn_string_literal)) = "5?????";
static const char data_320BA[] __attribute__((scn_string_literal)) = "7?????";
static const char data_320C1[] __attribute__((scn_string_literal)) = "8?????";
static const char data_320C8[] __attribute__((scn_string_literal)) = "3?????";
static const char data_320CF[] __attribute__((scn_string_literal)) = "4?????";
static const char data_320D6[] __attribute__((scn_string_literal)) = "6?????";
static const char data_320DD[] __attribute__((scn_string_literal)) = "9?????";
static const char data_320E4[] __attribute__((scn_string_literal)) = "0?????????";
static const char data_320EF[] __attribute__((scn_string_literal)) = "1?????";
static const char data_320F6[6] __attribute__((scn_string_literal)) = "E1066";
static const char data_320FC[10] __attribute__((scn_string_literal)) = "E1066A012";
static const char data_32106[10] __attribute__((scn_string_literal)) = "E1066A013";
static const char data_32110[10] __attribute__((scn_string_literal)) = "E1066A006";
static const char data_3211A[6] __attribute__((scn_string_literal)) = "E1066";
static const char data_32120[12] __attribute__((scn_string_literal)) = "メモ帳消去\012";
static const char data_3212C[6] __attribute__((scn_string_literal)) = "E1032";
static const char data_32132[10] __attribute__((scn_string_literal)) = "E1032A001";
static const char data_3213C[10] __attribute__((scn_string_literal)) = "E1032A002";
static const char data_32146[10] __attribute__((scn_string_literal)) = "E1032A003";
static const char data_32150[10] __attribute__((scn_string_literal)) = "E1032A004";
static const char data_3215A[10] __attribute__((scn_string_literal)) = "E1032A005";
static const char data_32164[6] __attribute__((scn_string_literal)) = "E1032";
static const char data_3216A[6] __attribute__((scn_string_literal)) = "E1032";
static const char data_32170[10] __attribute__((scn_string_literal)) = "E1032A061";
static const char data_3217A[10] __attribute__((scn_string_literal)) = "E1032A062";
static const char data_32184[10] __attribute__((scn_string_literal)) = "E1032A063";
static const char data_3218E[10] __attribute__((scn_string_literal)) = "E1032A064";
static const char data_32198[6] __attribute__((scn_string_literal)) = "E1032";
static const char data_3219E[6] __attribute__((scn_string_literal)) = "E1032";
static const char data_321A4[6] __attribute__((scn_string_literal)) = "E1032";
static const char data_321AA[6] __attribute__((scn_string_literal)) = "E1032";
static const char data_321B0[10] __attribute__((scn_string_literal)) = "E1032A031";
static const char data_321BA[10] __attribute__((scn_string_literal)) = "E1032A029";
static const char data_321C4[10] __attribute__((scn_string_literal)) = "E1032A032";
static const char data_321CE[10] __attribute__((scn_string_literal)) = "E1032A033";
static const char data_321D8[6] __attribute__((scn_string_literal)) = "E1032";
static const char data_321DE[10] __attribute__((scn_string_literal)) = "E1032A028";
static const char data_321E8[10] __attribute__((scn_string_literal)) = "E1032A029";
static const char data_321F2[10] __attribute__((scn_string_literal)) = "E1032A030";
static const char data_321FC[6] __attribute__((scn_string_literal)) = "E1032";
static const char data_32202[10] __attribute__((scn_string_literal)) = "E1032G001";
static const char data_3220C[10] __attribute__((scn_string_literal)) = "E1032G002";
static const char data_32216[6] __attribute__((scn_string_literal)) = "E1032";
static const char data_3221C[10] __attribute__((scn_string_literal)) = "E1032I001";
static const char data_32226[10] __attribute__((scn_string_literal)) = "E1032A052";
static const char data_32230[10] __attribute__((scn_string_literal)) = "E1032I007";
static const char data_3223A[10] __attribute__((scn_string_literal)) = "E1032I008";
static const char data_32244[10] __attribute__((scn_string_literal)) = "E1032A097";
static const char data_3224E[10] __attribute__((scn_string_literal)) = "E1032I009";
static const char data_32258[10] __attribute__((scn_string_literal)) = "E1032I010";
static const char data_32262[10] __attribute__((scn_string_literal)) = "E1032A054";
static const char data_3226C[10] __attribute__((scn_string_literal)) = "E1032I011";
static const char data_32276[10] __attribute__((scn_string_literal)) = "E1032A055";
static const char data_32280[10] __attribute__((scn_string_literal)) = "E1032I012";
static const char data_3228A[10] __attribute__((scn_string_literal)) = "E1032A056";
static const char data_32294[10] __attribute__((scn_string_literal)) = "E1032I013";
static const char data_3229E[10] __attribute__((scn_string_literal)) = "E1032A057";
static const char data_322A8[10] __attribute__((scn_string_literal)) = "E1032I011";
static const char data_322B2[10] __attribute__((scn_string_literal)) = "E1032A059";
static const char data_322BC[10] __attribute__((scn_string_literal)) = "E1032I009";
static const char data_322C6[10] __attribute__((scn_string_literal)) = "E1032I010";
static const char data_322D0[10] __attribute__((scn_string_literal)) = "E1032A054";
static const char data_322DA[10] __attribute__((scn_string_literal)) = "E1032I011";
static const char data_322E4[10] __attribute__((scn_string_literal)) = "E1032A060";
static const char data_322EE[10] __attribute__((scn_string_literal)) = "E1032I009";
static const char data_322F8[10] __attribute__((scn_string_literal)) = "E1032I010";
static const char data_32302[10] __attribute__((scn_string_literal)) = "E1032A054";
static const char data_3230C[10] __attribute__((scn_string_literal)) = "E1032I011";
static const char data_32316[10] __attribute__((scn_string_literal)) = "E1032I001";
static const char data_32320[10] __attribute__((scn_string_literal)) = "E1032A007";
static const char data_3232A[10] __attribute__((scn_string_literal)) = "E1032A006";
static const char data_32334[10] __attribute__((scn_string_literal)) = "E1032I002";
static const char data_3233E[10] __attribute__((scn_string_literal)) = "E1032I003";
static const char data_32348[10] __attribute__((scn_string_literal)) = "E1032I004";
static const char data_32352[10] __attribute__((scn_string_literal)) = "E1032I005";
static const char data_3235C[10] __attribute__((scn_string_literal)) = "E1032I006";
static const char data_32366[10] __attribute__((scn_string_literal)) = "E1032A008";
static const char data_32370[10] __attribute__((scn_string_literal)) = "E1032A009";
static const char data_3237A[10] __attribute__((scn_string_literal)) = "E1032K001";
static const char data_32384[10] __attribute__((scn_string_literal)) = "E1032K002";
static const char data_3238E[10] __attribute__((scn_string_literal)) = "E1032K019";
static const char data_32398[10] __attribute__((scn_string_literal)) = "E1032K020";
static const char data_323A2[10] __attribute__((scn_string_literal)) = "E1032K022";
static const char data_323AC[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_323B6[10] __attribute__((scn_string_literal)) = "E1032K016";
static const char data_323C0[10] __attribute__((scn_string_literal)) = "E1032K017";
static const char data_323CA[10] __attribute__((scn_string_literal)) = "E1032K018";
static const char data_323D4[10] __attribute__((scn_string_literal)) = "E1032K003";
static const char data_323DE[10] __attribute__((scn_string_literal)) = "E1032K016";
static const char data_323E8[10] __attribute__((scn_string_literal)) = "E1032K003";
static const char data_323F2[10] __attribute__((scn_string_literal)) = "E1032K017";
static const char data_323FC[10] __attribute__((scn_string_literal)) = "E1032K003";
static const char data_32406[10] __attribute__((scn_string_literal)) = "E1032K018";
static const char data_32410[10] __attribute__((scn_string_literal)) = "E1032K004";
static const char data_3241A[10] __attribute__((scn_string_literal)) = "E1032K016";
static const char data_32424[10] __attribute__((scn_string_literal)) = "E1032K004";
static const char data_3242E[10] __attribute__((scn_string_literal)) = "E1032K017";
static const char data_32438[10] __attribute__((scn_string_literal)) = "E1032K004";
static const char data_32442[10] __attribute__((scn_string_literal)) = "E1032K018";
static const char data_3244C[10] __attribute__((scn_string_literal)) = "E1032K005";
static const char data_32456[10] __attribute__((scn_string_literal)) = "E1032K016";
static const char data_32460[10] __attribute__((scn_string_literal)) = "E1032K005";
static const char data_3246A[10] __attribute__((scn_string_literal)) = "E1032K017";
static const char data_32474[10] __attribute__((scn_string_literal)) = "E1032K005";
static const char data_3247E[10] __attribute__((scn_string_literal)) = "E1032K018";
static const char data_32488[10] __attribute__((scn_string_literal)) = "E1032K006";
static const char data_32492[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_3249C[10] __attribute__((scn_string_literal)) = "E1032K006";
static const char data_324A6[10] __attribute__((scn_string_literal)) = "E1032K017";
static const char data_324B0[10] __attribute__((scn_string_literal)) = "E1032K006";
static const char data_324BA[10] __attribute__((scn_string_literal)) = "E1032K018";
static const char data_324C4[10] __attribute__((scn_string_literal)) = "E1032K007";
static const char data_324CE[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_324D8[10] __attribute__((scn_string_literal)) = "E1032K007";
static const char data_324E2[10] __attribute__((scn_string_literal)) = "E1032K017";
static const char data_324EC[10] __attribute__((scn_string_literal)) = "E1032K007";
static const char data_324F6[10] __attribute__((scn_string_literal)) = "E1032K018";
static const char data_32500[10] __attribute__((scn_string_literal)) = "E1032K008";
static const char data_3250A[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_32514[10] __attribute__((scn_string_literal)) = "E1032K008";
static const char data_3251E[10] __attribute__((scn_string_literal)) = "E1032K017";
static const char data_32528[10] __attribute__((scn_string_literal)) = "E1032K008";
static const char data_32532[10] __attribute__((scn_string_literal)) = "E1032K018";
static const char data_3253C[10] __attribute__((scn_string_literal)) = "E1032K009";
static const char data_32546[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_32550[10] __attribute__((scn_string_literal)) = "E1032K009";
static const char data_3255A[10] __attribute__((scn_string_literal)) = "E1032K016";
static const char data_32564[10] __attribute__((scn_string_literal)) = "E1032K009";
static const char data_3256E[10] __attribute__((scn_string_literal)) = "E1032K018";
static const char data_32578[10] __attribute__((scn_string_literal)) = "E1032K010";
static const char data_32582[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_3258C[10] __attribute__((scn_string_literal)) = "E1032K010";
static const char data_32596[10] __attribute__((scn_string_literal)) = "E1032K016";
static const char data_325A0[10] __attribute__((scn_string_literal)) = "E1032K010";
static const char data_325AA[10] __attribute__((scn_string_literal)) = "E1032K018";
static const char data_325B4[10] __attribute__((scn_string_literal)) = "E1032K011";
static const char data_325BE[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_325C8[10] __attribute__((scn_string_literal)) = "E1032K011";
static const char data_325D2[10] __attribute__((scn_string_literal)) = "E1032K016";
static const char data_325DC[10] __attribute__((scn_string_literal)) = "E1032K011";
static const char data_325E6[10] __attribute__((scn_string_literal)) = "E1032K018";
static const char data_325F0[10] __attribute__((scn_string_literal)) = "E1032K012";
static const char data_325FA[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_32604[10] __attribute__((scn_string_literal)) = "E1032K012";
static const char data_3260E[10] __attribute__((scn_string_literal)) = "E1032K016";
static const char data_32618[10] __attribute__((scn_string_literal)) = "E1032K012";
static const char data_32622[10] __attribute__((scn_string_literal)) = "E1032K017";
static const char data_3262C[10] __attribute__((scn_string_literal)) = "E1032K013";
static const char data_32636[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_32640[10] __attribute__((scn_string_literal)) = "E1032K013";
static const char data_3264A[10] __attribute__((scn_string_literal)) = "E1032K016";
static const char data_32654[10] __attribute__((scn_string_literal)) = "E1032K013";
static const char data_3265E[10] __attribute__((scn_string_literal)) = "E1032K017";
static const char data_32668[10] __attribute__((scn_string_literal)) = "E1032K014";
static const char data_32672[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_3267C[10] __attribute__((scn_string_literal)) = "E1032K014";
static const char data_32686[10] __attribute__((scn_string_literal)) = "E1032K016";
static const char data_32690[10] __attribute__((scn_string_literal)) = "E1032K014";
static const char data_3269A[10] __attribute__((scn_string_literal)) = "E1032K017";
static const char data_326A4[10] __attribute__((scn_string_literal)) = "E1032K004";
static const char data_326AE[10] __attribute__((scn_string_literal)) = "E1032K007";
static const char data_326B8[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_326C2[10] __attribute__((scn_string_literal)) = "E1032K004";
static const char data_326CC[10] __attribute__((scn_string_literal)) = "E1032K007";
static const char data_326D6[10] __attribute__((scn_string_literal)) = "E1032K017";
static const char data_326E0[10] __attribute__((scn_string_literal)) = "E1032K004";
static const char data_326EA[10] __attribute__((scn_string_literal)) = "E1032K006";
static const char data_326F4[10] __attribute__((scn_string_literal)) = "E1032K017";
static const char data_326FE[10] __attribute__((scn_string_literal)) = "E1032K004";
static const char data_32708[10] __attribute__((scn_string_literal)) = "E1032K013";
static const char data_32712[10] __attribute__((scn_string_literal)) = "E1032K017";
static const char data_3271C[10] __attribute__((scn_string_literal)) = "E1032K005";
static const char data_32726[10] __attribute__((scn_string_literal)) = "E1032K007";
static const char data_32730[10] __attribute__((scn_string_literal)) = "E1032K018";
static const char data_3273A[10] __attribute__((scn_string_literal)) = "E1032K007";
static const char data_32744[10] __attribute__((scn_string_literal)) = "E1032K004";
static const char data_3274E[10] __attribute__((scn_string_literal)) = "E1032K017";
static const char data_32758[10] __attribute__((scn_string_literal)) = "E1032K008";
static const char data_32762[10] __attribute__((scn_string_literal)) = "E1032K010";
static const char data_3276C[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_32776[10] __attribute__((scn_string_literal)) = "E1032K008";
static const char data_32780[10] __attribute__((scn_string_literal)) = "E1032K013";
static const char data_3278A[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_32794[10] __attribute__((scn_string_literal)) = "E1032K010";
static const char data_3279E[10] __attribute__((scn_string_literal)) = "E1032K005";
static const char data_327A8[10] __attribute__((scn_string_literal)) = "E1032K016";
static const char data_327B2[10] __attribute__((scn_string_literal)) = "E1032K010";
static const char data_327BC[10] __attribute__((scn_string_literal)) = "E1032K004";
static const char data_327C6[10] __attribute__((scn_string_literal)) = "E1032K018";
static const char data_327D0[10] __attribute__((scn_string_literal)) = "E1032K010";
static const char data_327DA[10] __attribute__((scn_string_literal)) = "E1032K007";
static const char data_327E4[10] __attribute__((scn_string_literal)) = "E1032K018";
static const char data_327EE[10] __attribute__((scn_string_literal)) = "E1032K011";
static const char data_327F8[10] __attribute__((scn_string_literal)) = "E1032K007";
static const char data_32802[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_3280C[10] __attribute__((scn_string_literal)) = "E1032K013";
static const char data_32816[10] __attribute__((scn_string_literal)) = "E1032K008";
static const char data_32820[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_3282A[10] __attribute__((scn_string_literal)) = "E1032K010";
static const char data_32834[10] __attribute__((scn_string_literal)) = "E1032K005";
static const char data_3283E[10] __attribute__((scn_string_literal)) = "E1032K016";
static const char data_32848[10] __attribute__((scn_string_literal)) = "E1032K004";
static const char data_32852[10] __attribute__((scn_string_literal)) = "E1032K010";
static const char data_3285C[10] __attribute__((scn_string_literal)) = "E1032K016";
static const char data_32866[10] __attribute__((scn_string_literal)) = "E1032K004";
static const char data_32870[10] __attribute__((scn_string_literal)) = "E1032K008";
static const char data_3287A[10] __attribute__((scn_string_literal)) = "E1032K018";
static const char data_32884[10] __attribute__((scn_string_literal)) = "E1032K003";
static const char data_3288E[10] __attribute__((scn_string_literal)) = "E1032K007";
static const char data_32898[10] __attribute__((scn_string_literal)) = "E1032K018";
static const char data_328A2[10] __attribute__((scn_string_literal)) = "E1032K007";
static const char data_328AC[10] __attribute__((scn_string_literal)) = "E1032K010";
static const char data_328B6[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_328C0[10] __attribute__((scn_string_literal)) = "E1032K008";
static const char data_328CA[10] __attribute__((scn_string_literal)) = "E1032K004";
static const char data_328D4[10] __attribute__((scn_string_literal)) = "E1032K018";
static const char data_328DE[10] __attribute__((scn_string_literal)) = "E1032K007";
static const char data_328E8[10] __attribute__((scn_string_literal)) = "E1032K010";
static const char data_328F2[10] __attribute__((scn_string_literal)) = "E1032K018";
static const char data_328FC[10] __attribute__((scn_string_literal)) = "E1032K004";
static const char data_32906[10] __attribute__((scn_string_literal)) = "E1032K010";
static const char data_32910[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_3291A[10] __attribute__((scn_string_literal)) = "E1032K004";
static const char data_32924[10] __attribute__((scn_string_literal)) = "E1032K007";
static const char data_3292E[10] __attribute__((scn_string_literal)) = "E1032K017";
static const char data_32938[10] __attribute__((scn_string_literal)) = "E1032K004";
static const char data_32942[10] __attribute__((scn_string_literal)) = "E1032K007";
static const char data_3294C[10] __attribute__((scn_string_literal)) = "E1032K018";
static const char data_32956[10] __attribute__((scn_string_literal)) = "E1032K010";
static const char data_32960[10] __attribute__((scn_string_literal)) = "E1032K008";
static const char data_3296A[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_32974[10] __attribute__((scn_string_literal)) = "E1032K013";
static const char data_3297E[10] __attribute__((scn_string_literal)) = "E1032K007";
static const char data_32988[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_32992[10] __attribute__((scn_string_literal)) = "E1032K004";
static const char data_3299C[10] __attribute__((scn_string_literal)) = "E1032K013";
static const char data_329A6[10] __attribute__((scn_string_literal)) = "E1032K016";
static const char data_329B0[10] __attribute__((scn_string_literal)) = "E1032K014";
static const char data_329BA[10] __attribute__((scn_string_literal)) = "E1032K007";
static const char data_329C4[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_329CE[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_329D8[10] __attribute__((scn_string_literal)) = "E1032L001";
static const char data_329E2[10] __attribute__((scn_string_literal)) = "E1032A098";
static const char data_329EC[10] __attribute__((scn_string_literal)) = "E1032L002";
static const char data_329F6[10] __attribute__((scn_string_literal)) = "E1032A099";
static const char data_32A00[10] __attribute__((scn_string_literal)) = "E1032L003";
static const char data_32A0A[10] __attribute__((scn_string_literal)) = "E1032L004";
static const char data_32A14[10] __attribute__((scn_string_literal)) = "E1032L005";
static const char data_32A1E[10] __attribute__((scn_string_literal)) = "E1032A100";
static const char data_32A28[10] __attribute__((scn_string_literal)) = "E1032L001";
static const char data_32A32[10] __attribute__((scn_string_literal)) = "E1032A101";
static const char data_32A3C[10] __attribute__((scn_string_literal)) = "E1032L006";
static const char data_32A46[10] __attribute__((scn_string_literal)) = "E1032L007";
static const char data_32A50[10] __attribute__((scn_string_literal)) = "E1032L008";
static const char data_32A5A[10] __attribute__((scn_string_literal)) = "E1032L009";
static const char data_32A64[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_32A6E[10] __attribute__((scn_string_literal)) = "E1032A102";
static const char data_32A78[10] __attribute__((scn_string_literal)) = "E1032L010";
static const char data_32A82[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_32A8C[10] __attribute__((scn_string_literal)) = "E1032L011";
static const char data_32A96[10] __attribute__((scn_string_literal)) = "E1032A101";
static const char data_32AA0[10] __attribute__((scn_string_literal)) = "E1032L012";
static const char data_32AAA[10] __attribute__((scn_string_literal)) = "E1032A103";
static const char data_32AB4[10] __attribute__((scn_string_literal)) = "E1032L013";
static const char data_32ABE[10] __attribute__((scn_string_literal)) = "E1032L014";
static const char data_32AC8[10] __attribute__((scn_string_literal)) = "E1032L015";
static const char data_32AD2[10] __attribute__((scn_string_literal)) = "E1032A103";
static const char data_32ADC[10] __attribute__((scn_string_literal)) = "E1032L016";
static const char data_32AE6[10] __attribute__((scn_string_literal)) = "E1032A104";
static const char data_32AF0[10] __attribute__((scn_string_literal)) = "E1032A105";
static const char data_32AFA[10] __attribute__((scn_string_literal)) = "E1032L017";
static const char data_32B04[10] __attribute__((scn_string_literal)) = "E1032A106";
static const char data_32B0E[10] __attribute__((scn_string_literal)) = "E1032L018";
static const char data_32B18[10] __attribute__((scn_string_literal)) = "E1032A107";
static const char data_32B22[10] __attribute__((scn_string_literal)) = "E1032L019";
static const char data_32B2C[10] __attribute__((scn_string_literal)) = "E1032A108";
static const char data_32B36[10] __attribute__((scn_string_literal)) = "E1032L020";
static const char data_32B40[10] __attribute__((scn_string_literal)) = "E1032L021";
static const char data_32B4A[10] __attribute__((scn_string_literal)) = "E1032A109";
static const char data_32B54[10] __attribute__((scn_string_literal)) = "E1032L022";
static const char data_32B5E[10] __attribute__((scn_string_literal)) = "E1032A110";
static const char data_32B68[10] __attribute__((scn_string_literal)) = "E1032L023";
static const char data_32B72[10] __attribute__((scn_string_literal)) = "E1032M001";
static const char data_32B7C[10] __attribute__((scn_string_literal)) = "E1032A111";
static const char data_32B86[10] __attribute__((scn_string_literal)) = "E1032M002";
static const char data_32B90[10] __attribute__((scn_string_literal)) = "E1032A112";
static const char data_32B9A[10] __attribute__((scn_string_literal)) = "E1032M003";
static const char data_32BA4[10] __attribute__((scn_string_literal)) = "E1032A113";
static const char data_32BAE[10] __attribute__((scn_string_literal)) = "E1032M004";
static const char data_32BB8[10] __attribute__((scn_string_literal)) = "E1032A104";
static const char data_32BC2[10] __attribute__((scn_string_literal)) = "E1032A114";
static const char data_32BCC[10] __attribute__((scn_string_literal)) = "E1032M005";
static const char data_32BD6[10] __attribute__((scn_string_literal)) = "E1032M006";
static const char data_32BE0[10] __attribute__((scn_string_literal)) = "E1032A111";
static const char data_32BEA[10] __attribute__((scn_string_literal)) = "E1032M002";
static const char data_32BF4[10] __attribute__((scn_string_literal)) = "E1032A115";
static const char data_32BFE[10] __attribute__((scn_string_literal)) = "E1032M018";
static const char data_32C08[10] __attribute__((scn_string_literal)) = "E1032A122";
static const char data_32C12[10] __attribute__((scn_string_literal)) = "E1032M019";
static const char data_32C1C[10] __attribute__((scn_string_literal)) = "E1032A123";
static const char data_32C26[10] __attribute__((scn_string_literal)) = "E1032M019";
static const char data_32C30[10] __attribute__((scn_string_literal)) = "E1032M020";
static const char data_32C3A[10] __attribute__((scn_string_literal)) = "E1032A104";
static const char data_32C44[10] __attribute__((scn_string_literal)) = "E1032A114";
static const char data_32C4E[10] __attribute__((scn_string_literal)) = "E1032M010";
static const char data_32C58[10] __attribute__((scn_string_literal)) = "E1032N001";
static const char data_32C62[10] __attribute__((scn_string_literal)) = "E1032A124";
static const char data_32C6C[10] __attribute__((scn_string_literal)) = "E1032A125";
static const char data_32C76[10] __attribute__((scn_string_literal)) = "E1032N002";
static const char data_32C80[10] __attribute__((scn_string_literal)) = "E1032A126";
static const char data_32C8A[10] __attribute__((scn_string_literal)) = "E1032N003";
static const char data_32C94[10] __attribute__((scn_string_literal)) = "E1032A127";
static const char data_32C9E[10] __attribute__((scn_string_literal)) = "E1032A128";
static const char data_32CA8[10] __attribute__((scn_string_literal)) = "E1032N004";
static const char data_32CB2[10] __attribute__((scn_string_literal)) = "E1032A129";
static const char data_32CBC[10] __attribute__((scn_string_literal)) = "E1032N005";
static const char data_32CC6[10] __attribute__((scn_string_literal)) = "E1032L011";
static const char data_32CD0[10] __attribute__((scn_string_literal)) = "E1032A101";
static const char data_32CDA[10] __attribute__((scn_string_literal)) = "E1032L019";
static const char data_32CE4[10] __attribute__((scn_string_literal)) = "E1032A117";
static const char data_32CEE[10] __attribute__((scn_string_literal)) = "E1032L024";
static const char data_32CF8[10] __attribute__((scn_string_literal)) = "E1032L025";
static const char data_32D02[10] __attribute__((scn_string_literal)) = "E1032L026";
static const char data_32D0C[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_32D16[10] __attribute__((scn_string_literal)) = "E1032A118";
static const char data_32D20[10] __attribute__((scn_string_literal)) = "E1032L023";
static const char data_32D2A[10] __attribute__((scn_string_literal)) = "E1032M001";
static const char data_32D34[10] __attribute__((scn_string_literal)) = "E1032A111";
static const char data_32D3E[10] __attribute__((scn_string_literal)) = "E1032M002";
static const char data_32D48[10] __attribute__((scn_string_literal)) = "E1032A112";
static const char data_32D52[10] __attribute__((scn_string_literal)) = "E1032M015";
static const char data_32D5C[10] __attribute__((scn_string_literal)) = "E1032A119";
static const char data_32D66[10] __attribute__((scn_string_literal)) = "E1032M016";
static const char data_32D70[10] __attribute__((scn_string_literal)) = "E1032A104";
static const char data_32D7A[10] __attribute__((scn_string_literal)) = "E1032A118";
static const char data_32D84[10] __attribute__((scn_string_literal)) = "E1032M017";
static const char data_32D8E[10] __attribute__((scn_string_literal)) = "E1032O001";
static const char data_32D98[10] __attribute__((scn_string_literal)) = "E1032A136";
static const char data_32DA2[10] __attribute__((scn_string_literal)) = "E1032O002";
static const char data_32DAC[10] __attribute__((scn_string_literal)) = "E1032A137";
static const char data_32DB6[10] __attribute__((scn_string_literal)) = "E1032O007";
static const char data_32DC0[10] __attribute__((scn_string_literal)) = "E1032A127";
static const char data_32DCA[10] __attribute__((scn_string_literal)) = "E1032O004";
static const char data_32DD4[10] __attribute__((scn_string_literal)) = "E1032A138";
static const char data_32DDE[10] __attribute__((scn_string_literal)) = "E1032O005";
static const char data_32DE8[10] __attribute__((scn_string_literal)) = "E1032A139";
static const char data_32DF2[10] __attribute__((scn_string_literal)) = "E1032O006";
static const char data_32DFC[10] __attribute__((scn_string_literal)) = "E1032O001";
static const char data_32E06[10] __attribute__((scn_string_literal)) = "E1032A136";
static const char data_32E10[10] __attribute__((scn_string_literal)) = "E1032O002";
static const char data_32E1A[10] __attribute__((scn_string_literal)) = "E1032A140";
static const char data_32E24[10] __attribute__((scn_string_literal)) = "E1032O007";
static const char data_32E2E[10] __attribute__((scn_string_literal)) = "E1032A127";
static const char data_32E38[10] __attribute__((scn_string_literal)) = "E1032O004";
static const char data_32E42[10] __attribute__((scn_string_literal)) = "E1032A138";
static const char data_32E4C[10] __attribute__((scn_string_literal)) = "E1032O005";
static const char data_32E56[10] __attribute__((scn_string_literal)) = "E1032A139";
static const char data_32E60[10] __attribute__((scn_string_literal)) = "E1032O006";
static const char data_32E6A[10] __attribute__((scn_string_literal)) = "E1032P001";
static const char data_32E74[10] __attribute__((scn_string_literal)) = "E1032A085";
static const char data_32E7E[10] __attribute__((scn_string_literal)) = "E1032P002";
static const char data_32E88[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_32E92[10] __attribute__((scn_string_literal)) = "E1032P003";
static const char data_32E9C[10] __attribute__((scn_string_literal)) = "E1032A141";
static const char data_32EA6[10] __attribute__((scn_string_literal)) = "E1032P004";
static const char data_32EB0[10] __attribute__((scn_string_literal)) = "E1032A142";
static const char data_32EBA[10] __attribute__((scn_string_literal)) = "E1032P005";
static const char data_32EC4[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_32ECE[10] __attribute__((scn_string_literal)) = "E1032P001";
static const char data_32ED8[10] __attribute__((scn_string_literal)) = "E1032A085";
static const char data_32EE2[10] __attribute__((scn_string_literal)) = "E1032P006";
static const char data_32EEC[10] __attribute__((scn_string_literal)) = "E1032A143";
static const char data_32EF6[10] __attribute__((scn_string_literal)) = "E1032P007";
static const char data_32F00[10] __attribute__((scn_string_literal)) = "E1032A144";
static const char data_32F0A[10] __attribute__((scn_string_literal)) = "E1032P004";
static const char data_32F14[10] __attribute__((scn_string_literal)) = "E1032O008";
static const char data_32F1E[10] __attribute__((scn_string_literal)) = "E1032P008";
static const char data_32F28[10] __attribute__((scn_string_literal)) = "E1032A068";
static const char data_32F32[10] __attribute__((scn_string_literal)) = "E1032P009";
static const char data_32F3C[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_32F46[10] __attribute__((scn_string_literal)) = "E1032A085";
static const char data_32F50[10] __attribute__((scn_string_literal)) = "E1032P010";
static const char data_32F5A[10] __attribute__((scn_string_literal)) = "E1032A145";
static const char data_32F64[10] __attribute__((scn_string_literal)) = "E1032P011";
static const char data_32F6E[10] __attribute__((scn_string_literal)) = "E1032A146";
static const char data_32F78[10] __attribute__((scn_string_literal)) = "E1032P012";
static const char data_32F82[10] __attribute__((scn_string_literal)) = "E1032A147";
static const char data_32F8C[10] __attribute__((scn_string_literal)) = "E1032P013";
static const char data_32F96[10] __attribute__((scn_string_literal)) = "E1032A148";
static const char data_32FA0[10] __attribute__((scn_string_literal)) = "E1032P014";
static const char data_32FAA[10] __attribute__((scn_string_literal)) = "E1032A149";
static const char data_32FB4[10] __attribute__((scn_string_literal)) = "E1032P015";
static const char data_32FBE[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_32FC8[10] __attribute__((scn_string_literal)) = "E1032P016";
static const char data_32FD2[10] __attribute__((scn_string_literal)) = "E1032A068";
static const char data_32FDC[10] __attribute__((scn_string_literal)) = "E1032P017";
static const char data_32FE6[10] __attribute__((scn_string_literal)) = "E1032A085";
static const char data_32FF0[10] __attribute__((scn_string_literal)) = "E1032P018";
static const char data_32FFA[10] __attribute__((scn_string_literal)) = "E1032A150";
static const char data_33004[10] __attribute__((scn_string_literal)) = "E1032P019";
static const char data_3300E[10] __attribute__((scn_string_literal)) = "E1032A151";
static const char data_33018[10] __attribute__((scn_string_literal)) = "E1032P004";
static const char data_33022[10] __attribute__((scn_string_literal)) = "E1032P016";
static const char data_3302C[10] __attribute__((scn_string_literal)) = "E1032A068";
static const char data_33036[10] __attribute__((scn_string_literal)) = "E1032P017";
static const char data_33040[10] __attribute__((scn_string_literal)) = "E1032P021";
static const char data_3304A[10] __attribute__((scn_string_literal)) = "E1032A152";
static const char data_33054[10] __attribute__((scn_string_literal)) = "E1032P010";
static const char data_3305E[10] __attribute__((scn_string_literal)) = "E1032A153";
static const char data_33068[10] __attribute__((scn_string_literal)) = "E1032P022";
static const char data_33072[10] __attribute__((scn_string_literal)) = "E1032A154";
static const char data_3307C[10] __attribute__((scn_string_literal)) = "E1032P023";
static const char data_33086[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_33090[10] __attribute__((scn_string_literal)) = "E1032P024";
static const char data_3309A[10] __attribute__((scn_string_literal)) = "E1032A068";
static const char data_330A4[10] __attribute__((scn_string_literal)) = "E1032P017";
static const char data_330AE[10] __attribute__((scn_string_literal)) = "E1032P001";
static const char data_330B8[10] __attribute__((scn_string_literal)) = "E1032A085";
static const char data_330C2[10] __attribute__((scn_string_literal)) = "E1032P025";
static const char data_330CC[10] __attribute__((scn_string_literal)) = "E1032A155";
static const char data_330D6[10] __attribute__((scn_string_literal)) = "E1032P004";
static const char data_330E0[10] __attribute__((scn_string_literal)) = "E1032A156";
static const char data_330EA[10] __attribute__((scn_string_literal)) = "E1032O008";
static const char data_330F4[10] __attribute__((scn_string_literal)) = "E1032P008";
static const char data_330FE[10] __attribute__((scn_string_literal)) = "E1032A154";
static const char data_33108[10] __attribute__((scn_string_literal)) = "E1032P027";
static const char data_33112[10] __attribute__((scn_string_literal)) = "E1032A068";
static const char data_3311C[10] __attribute__((scn_string_literal)) = "E1032P017";
static const char data_33126[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_33130[10] __attribute__((scn_string_literal)) = "E1032P021";
static const char data_3313A[10] __attribute__((scn_string_literal)) = "E1032A152";
static const char data_33144[10] __attribute__((scn_string_literal)) = "E1032P028";
static const char data_3314E[10] __attribute__((scn_string_literal)) = "E1032A158";
static const char data_33158[10] __attribute__((scn_string_literal)) = "E1032P030";
static const char data_33162[10] __attribute__((scn_string_literal)) = "E1032A159";
static const char data_3316C[10] __attribute__((scn_string_literal)) = "E1032P066";
static const char data_33176[10] __attribute__((scn_string_literal)) = "E1032P016";
static const char data_33180[10] __attribute__((scn_string_literal)) = "E1032A068";
static const char data_3318A[10] __attribute__((scn_string_literal)) = "E1032P017";
static const char data_33194[10] __attribute__((scn_string_literal)) = "E1032P021";
static const char data_3319E[10] __attribute__((scn_string_literal)) = "E1032A152";
static const char data_331A8[10] __attribute__((scn_string_literal)) = "E1032P032";
static const char data_331B2[10] __attribute__((scn_string_literal)) = "E1032A160";
static const char data_331BC[10] __attribute__((scn_string_literal)) = "E1032P033";
static const char data_331C6[10] __attribute__((scn_string_literal)) = "E1032P034";
static const char data_331D0[10] __attribute__((scn_string_literal)) = "E1032P080";
static const char data_331DA[10] __attribute__((scn_string_literal)) = "E1032P081";
static const char data_331E4[10] __attribute__((scn_string_literal)) = "E1032A068";
static const char data_331EE[10] __attribute__((scn_string_literal)) = "E1032P035";
static const char data_331F8[10] __attribute__((scn_string_literal)) = "E1032A141";
static const char data_33202[10] __attribute__((scn_string_literal)) = "E1032P036";
static const char data_3320C[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_33216[10] __attribute__((scn_string_literal)) = "E1032P021";
static const char data_33220[10] __attribute__((scn_string_literal)) = "E1032A152";
static const char data_3322A[10] __attribute__((scn_string_literal)) = "E1032P025";
static const char data_33234[10] __attribute__((scn_string_literal)) = "E1032A160";
static const char data_3323E[10] __attribute__((scn_string_literal)) = "E1032P033";
static const char data_33248[10] __attribute__((scn_string_literal)) = "E1032P082";
static const char data_33252[10] __attribute__((scn_string_literal)) = "E1032P083";
static const char data_3325C[10] __attribute__((scn_string_literal)) = "E1032P084";
static const char data_33266[10] __attribute__((scn_string_literal)) = "E1032P085";
static const char data_33270[10] __attribute__((scn_string_literal)) = "E1032A068";
static const char data_3327A[10] __attribute__((scn_string_literal)) = "E1032P086";
static const char data_33284[10] __attribute__((scn_string_literal)) = "E1032P001";
static const char data_3328E[10] __attribute__((scn_string_literal)) = "E1032A164";
static const char data_33298[10] __attribute__((scn_string_literal)) = "E1032P018";
static const char data_332A2[10] __attribute__((scn_string_literal)) = "E1032A165";
static const char data_332AC[10] __attribute__((scn_string_literal)) = "E1032P037";
static const char data_332B6[10] __attribute__((scn_string_literal)) = "E1032A104";
static const char data_332C0[10] __attribute__((scn_string_literal)) = "E1032P120";
static const char data_332CA[10] __attribute__((scn_string_literal)) = "E1032A173";
static const char data_332D4[10] __attribute__((scn_string_literal)) = "E1032O008";
static const char data_332DE[10] __attribute__((scn_string_literal)) = "E1032P008";
static const char data_332E8[10] __attribute__((scn_string_literal)) = "E1032A154";
static const char data_332F2[10] __attribute__((scn_string_literal)) = "E1032P027";
static const char data_332FC[10] __attribute__((scn_string_literal)) = "E1032A068";
static const char data_33306[10] __attribute__((scn_string_literal)) = "E1032P017";
static const char data_33310[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_3331A[10] __attribute__((scn_string_literal)) = "E1032P001";
static const char data_33324[10] __attribute__((scn_string_literal)) = "E1032A164";
static const char data_3332E[10] __attribute__((scn_string_literal)) = "E1032P018";
static const char data_33338[10] __attribute__((scn_string_literal)) = "E1032A170";
static const char data_33342[10] __attribute__((scn_string_literal)) = "E1032P044";
static const char data_3334C[10] __attribute__((scn_string_literal)) = "E1032A200";
static const char data_33356[10] __attribute__((scn_string_literal)) = "E1032A172";
static const char data_33360[10] __attribute__((scn_string_literal)) = "E1032P045";
static const char data_3336A[10] __attribute__((scn_string_literal)) = "E1032A173";
static const char data_33374[10] __attribute__((scn_string_literal)) = "E1032P046";
static const char data_3337E[10] __attribute__((scn_string_literal)) = "E1032P047";
static const char data_33388[10] __attribute__((scn_string_literal)) = "E1032A201";
static const char data_33392[10] __attribute__((scn_string_literal)) = "E1032P049";
static const char data_3339C[10] __attribute__((scn_string_literal)) = "E1032A176";
static const char data_333A6[10] __attribute__((scn_string_literal)) = "E1032P050";
static const char data_333B0[10] __attribute__((scn_string_literal)) = "E1032A177";
static const char data_333BA[10] __attribute__((scn_string_literal)) = "E1032P051";
static const char data_333C4[10] __attribute__((scn_string_literal)) = "E1032P001";
static const char data_333CE[10] __attribute__((scn_string_literal)) = "E1032A164";
static const char data_333D8[10] __attribute__((scn_string_literal)) = "E1032P018";
static const char data_333E2[10] __attribute__((scn_string_literal)) = "E1032A178";
static const char data_333EC[10] __attribute__((scn_string_literal)) = "E1032P004";
static const char data_333F6[10] __attribute__((scn_string_literal)) = "E1032P052";
static const char data_33400[10] __attribute__((scn_string_literal)) = "E1032A179";
static const char data_3340A[10] __attribute__((scn_string_literal)) = "E1032A180";
static const char data_33414[10] __attribute__((scn_string_literal)) = "E1032A181";
static const char data_3341E[10] __attribute__((scn_string_literal)) = "E1032P053";
static const char data_33428[10] __attribute__((scn_string_literal)) = "E1032P087";
static const char data_33432[10] __attribute__((scn_string_literal)) = "E1032P055";
static const char data_3343C[10] __attribute__((scn_string_literal)) = "E1032A182";
static const char data_33446[10] __attribute__((scn_string_literal)) = "E1032P051";
static const char data_33450[10] __attribute__((scn_string_literal)) = "E1032P001";
static const char data_3345A[10] __attribute__((scn_string_literal)) = "E1032A164";
static const char data_33464[10] __attribute__((scn_string_literal)) = "E1032P018";
static const char data_3346E[10] __attribute__((scn_string_literal)) = "E1032A183";
static const char data_33478[10] __attribute__((scn_string_literal)) = "E1032P038";
static const char data_33482[10] __attribute__((scn_string_literal)) = "E1032A184";
static const char data_3348C[10] __attribute__((scn_string_literal)) = "E1032P056";
static const char data_33496[10] __attribute__((scn_string_literal)) = "E1032P088";
static const char data_334A0[10] __attribute__((scn_string_literal)) = "E1032P058";
static const char data_334AA[10] __attribute__((scn_string_literal)) = "E1032A220";
static const char data_334B4[10] __attribute__((scn_string_literal)) = "E1032A185";
static const char data_334BE[10] __attribute__((scn_string_literal)) = "E1032P059";
static const char data_334C8[10] __attribute__((scn_string_literal)) = "E1032P001";
static const char data_334D2[10] __attribute__((scn_string_literal)) = "E1032A164";
static const char data_334DC[10] __attribute__((scn_string_literal)) = "E1032P018";
static const char data_334E6[10] __attribute__((scn_string_literal)) = "E1032P060";
static const char data_334F0[10] __attribute__((scn_string_literal)) = "E1032A186";
static const char data_334FA[10] __attribute__((scn_string_literal)) = "E1032P061";
static const char data_33504[10] __attribute__((scn_string_literal)) = "E1032A187";
static const char data_3350E[10] __attribute__((scn_string_literal)) = "E1032P062";
static const char data_33518[10] __attribute__((scn_string_literal)) = "E1032A188";
static const char data_33522[10] __attribute__((scn_string_literal)) = "E1032P063";
static const char data_3352C[10] __attribute__((scn_string_literal)) = "E1032P064";
static const char data_33536[10] __attribute__((scn_string_literal)) = "E1032P065";
static const char data_33540[10] __attribute__((scn_string_literal)) = "E1032A189";
static const char data_3354A[10] __attribute__((scn_string_literal)) = "E1032P066";
static const char data_33554[10] __attribute__((scn_string_literal)) = "E1032A190";
static const char data_3355E[10] __attribute__((scn_string_literal)) = "E1032P001";
static const char data_33568[10] __attribute__((scn_string_literal)) = "E1032A152";
static const char data_33572[10] __attribute__((scn_string_literal)) = "E1032P067";
static const char data_3357C[10] __attribute__((scn_string_literal)) = "E1032A191";
static const char data_33586[10] __attribute__((scn_string_literal)) = "E1032A192";
static const char data_33590[10] __attribute__((scn_string_literal)) = "E1032P068";
static const char data_3359A[10] __attribute__((scn_string_literal)) = "E1032P069";
static const char data_335A4[10] __attribute__((scn_string_literal)) = "E1032A193";
static const char data_335AE[10] __attribute__((scn_string_literal)) = "E1032P001";
static const char data_335B8[10] __attribute__((scn_string_literal)) = "E1032A152";
static const char data_335C2[10] __attribute__((scn_string_literal)) = "E1032P070";
static const char data_335CC[10] __attribute__((scn_string_literal)) = "E1032A194";
static const char data_335D6[10] __attribute__((scn_string_literal)) = "E1032P071";
static const char data_335E0[10] __attribute__((scn_string_literal)) = "E1032A195";
static const char data_335EA[10] __attribute__((scn_string_literal)) = "E1032P072";
static const char data_335F4[10] __attribute__((scn_string_literal)) = "E1032P073";
static const char data_335FE[10] __attribute__((scn_string_literal)) = "E1032A151";
static const char data_33608[10] __attribute__((scn_string_literal)) = "E1032P074";
static const char data_33612[10] __attribute__((scn_string_literal)) = "E1032P075";
static const char data_3361C[10] __attribute__((scn_string_literal)) = "E1032P089";
static const char data_33626[10] __attribute__((scn_string_literal)) = "E1032P090";
static const char data_33630[10] __attribute__((scn_string_literal)) = "E1032A196";
static const char data_3363A[10] __attribute__((scn_string_literal)) = "E1032P079";
static const char data_33644[10] __attribute__((scn_string_literal)) = "E1032A197";
static const char data_3364E[10] __attribute__((scn_string_literal)) = "E1032P001";
static const char data_33658[10] __attribute__((scn_string_literal)) = "E1032A152";
static const char data_33662[10] __attribute__((scn_string_literal)) = "E1032P010";
static const char data_3366C[10] __attribute__((scn_string_literal)) = "E1032P091";
static const char data_33676[10] __attribute__((scn_string_literal)) = "E1032P092";
static const char data_33680[10] __attribute__((scn_string_literal)) = "E1032A203";
static const char data_3368A[10] __attribute__((scn_string_literal)) = "E1032P093";
static const char data_33694[10] __attribute__((scn_string_literal)) = "E1032P094";
static const char data_3369E[10] __attribute__((scn_string_literal)) = "E1032A204";
static const char data_336A8[10] __attribute__((scn_string_literal)) = "E1032P095";
static const char data_336B2[10] __attribute__((scn_string_literal)) = "E1032P021";
static const char data_336BC[10] __attribute__((scn_string_literal)) = "E1032A152";
static const char data_336C6[10] __attribute__((scn_string_literal)) = "E1032P096";
static const char data_336D0[10] __attribute__((scn_string_literal)) = "E1032A104";
static const char data_336DA[10] __attribute__((scn_string_literal)) = "E1032P097";
static const char data_336E4[10] __attribute__((scn_string_literal)) = "E1032A103";
static const char data_336EE[10] __attribute__((scn_string_literal)) = "E1032P098";
static const char data_336F8[10] __attribute__((scn_string_literal)) = "E1032P099";
static const char data_33702[10] __attribute__((scn_string_literal)) = "E1032P100";
static const char data_3370C[10] __attribute__((scn_string_literal)) = "E1032A206";
static const char data_33716[10] __attribute__((scn_string_literal)) = "E1032P021";
static const char data_33720[10] __attribute__((scn_string_literal)) = "E1032A152";
static const char data_3372A[10] __attribute__((scn_string_literal)) = "E1032P101";
static const char data_33734[10] __attribute__((scn_string_literal)) = "E1032A207";
static const char data_3373E[10] __attribute__((scn_string_literal)) = "E1032P102";
static const char data_33748[10] __attribute__((scn_string_literal)) = "E1032A208";
static const char data_33752[10] __attribute__((scn_string_literal)) = "E1032P103";
static const char data_3375C[10] __attribute__((scn_string_literal)) = "E1032A209";
static const char data_33766[10] __attribute__((scn_string_literal)) = "E1032P104";
static const char data_33770[10] __attribute__((scn_string_literal)) = "E1032P105";
static const char data_3377A[10] __attribute__((scn_string_literal)) = "E1032A210";
static const char data_33784[10] __attribute__((scn_string_literal)) = "E1032P106";
static const char data_3378E[10] __attribute__((scn_string_literal)) = "E1032A154";
static const char data_33798[10] __attribute__((scn_string_literal)) = "E1032P107";
static const char data_337A2[10] __attribute__((scn_string_literal)) = "E1032A204";
static const char data_337AC[10] __attribute__((scn_string_literal)) = "E1032P108";
static const char data_337B6[10] __attribute__((scn_string_literal)) = "E1032P109";
static const char data_337C0[10] __attribute__((scn_string_literal)) = "E1032A085";
static const char data_337CA[10] __attribute__((scn_string_literal)) = "E1032P110";
static const char data_337D4[10] __attribute__((scn_string_literal)) = "E1032A211";
static const char data_337DE[10] __attribute__((scn_string_literal)) = "E1032P111";
static const char data_337E8[10] __attribute__((scn_string_literal)) = "E1032A104";
static const char data_337F2[10] __attribute__((scn_string_literal)) = "E1032P112";
static const char data_337FC[10] __attribute__((scn_string_literal)) = "E1032A212";
static const char data_33806[10] __attribute__((scn_string_literal)) = "E1032P113";
static const char data_33810[10] __attribute__((scn_string_literal)) = "E1032A158";
static const char data_3381A[10] __attribute__((scn_string_literal)) = "E1032P114";
static const char data_33824[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_3382E[10] __attribute__((scn_string_literal)) = "E1032P115";
static const char data_33838[10] __attribute__((scn_string_literal)) = "E1032A213";
static const char data_33842[10] __attribute__((scn_string_literal)) = "E1032P116";
static const char data_3384C[10] __attribute__((scn_string_literal)) = "E1032P001";
static const char data_33856[10] __attribute__((scn_string_literal)) = "E1032A152";
static const char data_33860[10] __attribute__((scn_string_literal)) = "E1032P117";
static const char data_3386A[10] __attribute__((scn_string_literal)) = "E1032A214";
static const char data_33874[10] __attribute__((scn_string_literal)) = "E1032P118";
static const char data_3387E[10] __attribute__((scn_string_literal)) = "E1032A154";
static const char data_33888[10] __attribute__((scn_string_literal)) = "E1032P119";
static const char data_33892[10] __attribute__((scn_string_literal)) = "E1032P120";
static const char data_3389C[10] __attribute__((scn_string_literal)) = "E1032A215";
static const char data_338A6[10] __attribute__((scn_string_literal)) = "E1032P121";
static const char data_338B0[10] __attribute__((scn_string_literal)) = "E1032A163";
static const char data_338BA[10] __attribute__((scn_string_literal)) = "E1032P123";
static const char data_338C4[10] __attribute__((scn_string_literal)) = "E1032A217";
static const char data_338CE[10] __attribute__((scn_string_literal)) = "E1032P001";
static const char data_338D8[10] __attribute__((scn_string_literal)) = "E1032A152";
static const char data_338E2[10] __attribute__((scn_string_literal)) = "E1032P010";
static const char data_338EC[10] __attribute__((scn_string_literal)) = "E1032P124";
static const char data_338F6[10] __attribute__((scn_string_literal)) = "E1032P125";
static const char data_33900[10] __attribute__((scn_string_literal)) = "E1032A218";
static const char data_3390A[10] __attribute__((scn_string_literal)) = "E1032P126";
static const char data_33914[10] __attribute__((scn_string_literal)) = "E1032A154";
static const char data_3391E[10] __attribute__((scn_string_literal)) = "E1032P127";
static const char data_33928[10] __attribute__((scn_string_literal)) = "E1032A213";
static const char data_33932[10] __attribute__((scn_string_literal)) = "E1032P128";
static const char data_3393C[10] __attribute__((scn_string_literal)) = "E1032A204";
static const char data_33946[10] __attribute__((scn_string_literal)) = "E1032P116";
static const char data_33950[10] __attribute__((scn_string_literal)) = "E1032P109";
static const char data_3395A[10] __attribute__((scn_string_literal)) = "E1032A085";
static const char data_33964[10] __attribute__((scn_string_literal)) = "E1032P117";
static const char data_3396E[10] __attribute__((scn_string_literal)) = "E1032A214";
static const char data_33978[10] __attribute__((scn_string_literal)) = "E1032P129";
static const char data_33982[10] __attribute__((scn_string_literal)) = "E1032P130";
static const char data_3398C[10] __attribute__((scn_string_literal)) = "E1032P131";
static const char data_33996[10] __attribute__((scn_string_literal)) = "E1032A213";
static const char data_339A0[10] __attribute__((scn_string_literal)) = "E1032A219";
static const char data_339AA[10] __attribute__((scn_string_literal)) = "E1032P132";
static const char data_339B4[10] __attribute__((scn_string_literal)) = "E1032A153";
static const char data_339BE[10] __attribute__((scn_string_literal)) = "E1032P133";
static const char data_339C8[10] __attribute__((scn_string_literal)) = "E1032A220";
static const char data_339D2[10] __attribute__((scn_string_literal)) = "E1032P134";
static const char data_339DC[10] __attribute__((scn_string_literal)) = "E1032A204";
static const char data_339E6[10] __attribute__((scn_string_literal)) = "E1032P116";
static const char data_339F0[10] __attribute__((scn_string_literal)) = "E1032A085";
static const char data_339FA[10] __attribute__((scn_string_literal)) = "E1032P010";
static const char data_33A04[10] __attribute__((scn_string_literal)) = "E1032A221";
static const char data_33A0E[10] __attribute__((scn_string_literal)) = "E1032P135";
static const char data_33A18[10] __attribute__((scn_string_literal)) = "E1032P136";
static const char data_33A22[10] __attribute__((scn_string_literal)) = "E1032A222";
static const char data_33A2C[10] __attribute__((scn_string_literal)) = "E1032P137";
static const char data_33A36[10] __attribute__((scn_string_literal)) = "E1032P138";
static const char data_33A40[10] __attribute__((scn_string_literal)) = "E1032A223";
static const char data_33A4A[10] __attribute__((scn_string_literal)) = "E1032P139";
static const char data_33A54[10] __attribute__((scn_string_literal)) = "E1032A224";
static const char data_33A5E[10] __attribute__((scn_string_literal)) = "E1032P140";
static const char data_33A68[10] __attribute__((scn_string_literal)) = "E1032A225";
static const char data_33A72[10] __attribute__((scn_string_literal)) = "E1032A226";
static const char data_33A7C[10] __attribute__((scn_string_literal)) = "E1032P004";
static const char data_33A86[10] __attribute__((scn_string_literal)) = "E1032E001";
static const char data_33A90[10] __attribute__((scn_string_literal)) = "E1032E002";
static const char data_33A9A[10] __attribute__((scn_string_literal)) = "E1032E003";
static const char data_33AA4[10] __attribute__((scn_string_literal)) = "E1032A010";
static const char data_33AAE[10] __attribute__((scn_string_literal)) = "E1032E004";
static const char data_33AB8[10] __attribute__((scn_string_literal)) = "E1032A011";
static const char data_33AC2[10] __attribute__((scn_string_literal)) = "E1032E005";
static const char data_33ACC[10] __attribute__((scn_string_literal)) = "E1032A012";
static const char data_33AD6[10] __attribute__((scn_string_literal)) = "E1032A013";
static const char data_33AE0[10] __attribute__((scn_string_literal)) = "E1032E006";
static const char data_33AEA[10] __attribute__((scn_string_literal)) = "E1032A014";
static const char data_33AF4[10] __attribute__((scn_string_literal)) = "E1032A015";
static const char data_33AFE[10] __attribute__((scn_string_literal)) = "E1032E007";
static const char data_33B08[10] __attribute__((scn_string_literal)) = "E1032A016";
static const char data_33B12[10] __attribute__((scn_string_literal)) = "E1032E008";
static const char data_33B1C[10] __attribute__((scn_string_literal)) = "E1032E009";
static const char data_33B26[10] __attribute__((scn_string_literal)) = "E1032A017";
static const char data_33B30[10] __attribute__((scn_string_literal)) = "E1032E010";
static const char data_33B3A[10] __attribute__((scn_string_literal)) = "E1032E011";
static const char data_33B44[10] __attribute__((scn_string_literal)) = "E1032E012";
static const char data_33B4E[10] __attribute__((scn_string_literal)) = "E1032E013";
static const char data_33B58[10] __attribute__((scn_string_literal)) = "E1032A018";
static const char data_33B62[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_33B6C[10] __attribute__((scn_string_literal)) = "E1032A020";
static const char data_33B76[10] __attribute__((scn_string_literal)) = "E1032A021";
static const char data_33B80[10] __attribute__((scn_string_literal)) = "E1032F001";
static const char data_33B8A[10] __attribute__((scn_string_literal)) = "E1032F002";
static const char data_33B94[10] __attribute__((scn_string_literal)) = "E1032F003";
static const char data_33B9E[10] __attribute__((scn_string_literal)) = "E1032A022";
static const char data_33BA8[10] __attribute__((scn_string_literal)) = "E1032F004";
static const char data_33BB2[10] __attribute__((scn_string_literal)) = "E1032A011";
static const char data_33BBC[10] __attribute__((scn_string_literal)) = "E1032F005";
static const char data_33BC6[10] __attribute__((scn_string_literal)) = "E1032A012";
static const char data_33BD0[10] __attribute__((scn_string_literal)) = "E1032A013";
static const char data_33BDA[10] __attribute__((scn_string_literal)) = "E1032F006";
static const char data_33BE4[10] __attribute__((scn_string_literal)) = "E1032A023";
static const char data_33BEE[10] __attribute__((scn_string_literal)) = "E1032A024";
static const char data_33BF8[10] __attribute__((scn_string_literal)) = "E1032F007";
static const char data_33C02[10] __attribute__((scn_string_literal)) = "E1032A025";
static const char data_33C0C[10] __attribute__((scn_string_literal)) = "E1032A026";
static const char data_33C16[10] __attribute__((scn_string_literal)) = "E1032F008";
static const char data_33C20[10] __attribute__((scn_string_literal)) = "E1032F009";
static const char data_33C2A[10] __attribute__((scn_string_literal)) = "E1032F010";
static const char data_33C34[10] __attribute__((scn_string_literal)) = "E1032F011";
static const char data_33C3E[10] __attribute__((scn_string_literal)) = "E1032F012";
static const char data_33C48[10] __attribute__((scn_string_literal)) = "E1032F013";
static const char data_33C52[10] __attribute__((scn_string_literal)) = "E1032F014";
static const char data_33C5C[10] __attribute__((scn_string_literal)) = "E1032F015";
static const char data_33C66[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_33C70[10] __attribute__((scn_string_literal)) = "E1032A018";
static const char data_33C7A[10] __attribute__((scn_string_literal)) = "E1032A027";
static const char data_33C84[10] __attribute__((scn_string_literal)) = "E1032A034";
static const char data_33C8E[10] __attribute__((scn_string_literal)) = "E1032H005";
static const char data_33C98[10] __attribute__((scn_string_literal)) = "E1032H004";
static const char data_33CA2[10] __attribute__((scn_string_literal)) = "E1032H009";
static const char data_33CAC[10] __attribute__((scn_string_literal)) = "E1032H006";
static const char data_33CB6[10] __attribute__((scn_string_literal)) = "E1032A042";
static const char data_33CC0[10] __attribute__((scn_string_literal)) = "E1032A040";
static const char data_33CCA[10] __attribute__((scn_string_literal)) = "E1032A043";
static const char data_33CD4[10] __attribute__((scn_string_literal)) = "E1032A041";
static const char data_33CDE[10] __attribute__((scn_string_literal)) = "E1032A045";
static const char data_33CE8[10] __attribute__((scn_string_literal)) = "E1032A046";
static const char data_33CF2[10] __attribute__((scn_string_literal)) = "E1032A047";
static const char data_33CFC[10] __attribute__((scn_string_literal)) = "E1032A046";
static const char data_33D06[10] __attribute__((scn_string_literal)) = "E1032A047";
static const char data_33D10[10] __attribute__((scn_string_literal)) = "E1032A044";
static const char data_33D1A[10] __attribute__((scn_string_literal)) = "E1032A051";
static const char data_33D24[10] __attribute__((scn_string_literal)) = "E1032A039";
static const char data_33D2E[10] __attribute__((scn_string_literal)) = "E1032H007";
static const char data_33D38[10] __attribute__((scn_string_literal)) = "E1032A120";
static const char data_33D42[10] __attribute__((scn_string_literal)) = "E1032H010";
static const char data_33D4C[10] __attribute__((scn_string_literal)) = "E1032A066";
static const char data_33D56[10] __attribute__((scn_string_literal)) = "E1032H011";
static const char data_33D60[10] __attribute__((scn_string_literal)) = "E1032J001";
static const char data_33D6A[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_33D74[10] __attribute__((scn_string_literal)) = "E1032A131";
static const char data_33D7E[10] __attribute__((scn_string_literal)) = "E1032J010";
static const char data_33D88[10] __attribute__((scn_string_literal)) = "E1032J013";
static const char data_33D92[10] __attribute__((scn_string_literal)) = "E1032J001";
static const char data_33D9C[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_33DA6[10] __attribute__((scn_string_literal)) = "E1032A134";
static const char data_33DB0[10] __attribute__((scn_string_literal)) = "E1032J015";
static const char data_33DBA[10] __attribute__((scn_string_literal)) = "E1032A135";
static const char data_33DC4[10] __attribute__((scn_string_literal)) = "E1032J001";
static const char data_33DCE[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_33DD8[10] __attribute__((scn_string_literal)) = "E1032J018";
static const char data_33DE2[10] __attribute__((scn_string_literal)) = "E1032A163";
static const char data_33DEC[10] __attribute__((scn_string_literal)) = "E1032J019";
static const char data_33DF6[10] __attribute__((scn_string_literal)) = "E1032J020";
static const char data_33E00[10] __attribute__((scn_string_literal)) = "E1032A068";
static const char data_33E0A[10] __attribute__((scn_string_literal)) = "E1032J016";
static const char data_33E14[10] __attribute__((scn_string_literal)) = "E1032A161";
static const char data_33E1E[10] __attribute__((scn_string_literal)) = "E1032J017";
static const char data_33E28[10] __attribute__((scn_string_literal)) = "E1032A162";
static const char data_33E32[10] __attribute__((scn_string_literal)) = "E1032J001";
static const char data_33E3C[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_33E46[10] __attribute__((scn_string_literal)) = "E1032A131";
static const char data_33E50[10] __attribute__((scn_string_literal)) = "E1032J010";
static const char data_33E5A[10] __attribute__((scn_string_literal)) = "E1032A132";
static const char data_33E64[10] __attribute__((scn_string_literal)) = "E1032J011";
static const char data_33E6E[10] __attribute__((scn_string_literal)) = "E1032J012";
static const char data_33E78[10] __attribute__((scn_string_literal)) = "E1032H007";
static const char data_33E82[10] __attribute__((scn_string_literal)) = "E1032A120";
static const char data_33E8C[10] __attribute__((scn_string_literal)) = "E1032H010";
static const char data_33E96[10] __attribute__((scn_string_literal)) = "E1032A066";
static const char data_33EA0[10] __attribute__((scn_string_literal)) = "E1032H011";
static const char data_33EAA[10] __attribute__((scn_string_literal)) = "E1032J001";
static const char data_33EB4[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_33EBE[10] __attribute__((scn_string_literal)) = "E1032J018";
static const char data_33EC8[10] __attribute__((scn_string_literal)) = "E1032A163";
static const char data_33ED2[10] __attribute__((scn_string_literal)) = "E1032J019";
static const char data_33EDC[10] __attribute__((scn_string_literal)) = "E1032J020";
static const char data_33EE6[10] __attribute__((scn_string_literal)) = "E1032A068";
static const char data_33EF0[10] __attribute__((scn_string_literal)) = "E1032J016";
static const char data_33EFA[10] __attribute__((scn_string_literal)) = "E1032A161";
static const char data_33F04[10] __attribute__((scn_string_literal)) = "E1032J017";
static const char data_33F0E[10] __attribute__((scn_string_literal)) = "E1032A162";
static const char data_33F18[10] __attribute__((scn_string_literal)) = "E1032J001";
static const char data_33F22[10] __attribute__((scn_string_literal)) = "E1032A067";
static const char data_33F2C[10] __attribute__((scn_string_literal)) = "E1032J002";
static const char data_33F36[10] __attribute__((scn_string_literal)) = "E1032A068";
static const char data_33F40[10] __attribute__((scn_string_literal)) = "E1032J001";
static const char data_33F4A[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_33F54[10] __attribute__((scn_string_literal)) = "E1032J003";
static const char data_33F5E[10] __attribute__((scn_string_literal)) = "E1032A069";
static const char data_33F68[10] __attribute__((scn_string_literal)) = "E1032J004";
static const char data_33F72[10] __attribute__((scn_string_literal)) = "E1032A070";
static const char data_33F7C[10] __attribute__((scn_string_literal)) = "E1032H007";
static const char data_33F86[10] __attribute__((scn_string_literal)) = "E1032A048";
static const char data_33F90[10] __attribute__((scn_string_literal)) = "E1032H008";
static const char data_33F9A[10] __attribute__((scn_string_literal)) = "E1032A049";
static const char data_33FA4[10] __attribute__((scn_string_literal)) = "E1032A050";
static const char data_33FAE[14] __attribute__((scn_string_literal)) = "M_TPIN.BIN";
static const int8_t data_33FBC[16] = {12, 17, 22, 10, 19, 18, 11, 8, 4, 4, 17, 7, 6, 5, 4, 4};
static const int8_t data_33FCC[16] = {12, 1, 2, 1, 3, 2, 2, 8, 4, 4, 17, 7, 6, 5, 4, 4};
static const uint32_t data_33FDC = 0xA800A7;
static const int32_t data_33FE0[5] = {8847529, 8978568, 15073509, 15204583, 15335657};
static const uint32_t data_33FF4 = 0x440043;
static const int32_t data_33FF8[8] = {4587589, 4718663, 4849737, 4980811, 5111885, 5242959, 5374033, 85};
static const uint32_t data_34018 = 0x5C005B;
static const int32_t data_3401C[10] = {6160477, 6291551, 6422625, 6553699, 6684773, 6815847, 6946921, 7077995, 7209069, 7340143};
static const uint32_t data_34044 = 0x740073;
static const int32_t data_34048[4] = {7798902, 7929976, 8061050, 8257660};
static const uint32_t data_34058 = 0x720071;
static const int32_t data_3405C[9] = {10223750, 10354845, 10485919, 10616993, 10748067, 10879141, 11206826, 5767255, 180};
static const uint32_t data_34080 = 0x810080;
static const int32_t data_34084[8] = {8585346, 11403437, 11534511, 11665585, 11862195, 11993270, 12452024, 8650879};
static const uint32_t data_340A4 = 0xB900BC;
static const int32_t data_340A8[5] = {12255418, 13238473, 13500621, 12517579, 200};
static const uint32_t data_340BC = 0x5A0059;
static const int32_t data_340C0[3] = {5636268, 14221445, 5505107};
static const uint32_t data_340CC = 0x7D0075;
static const int32_t data_340D0 = 12386442;
static const uint32_t data_340D4 = 0xC600C5;
static const int32_t data_340D8 = 13369543;
static const uint32_t data_340DC = 0x8C008B;
static const int32_t data_340E0[8] = {9306253, 9437327, 9633937, 148, 9961623, 10092697, 9568411, 149};
static const uint32_t data_34100 = 0xD700D6;
static const int32_t data_34104[3] = {14287064, 14876891, 228};
static const uint32_t data_34110 = 0xDC00D3;
static const int32_t data_34114[2] = {13893845, 14549213};
static const uint32_t data_3411C = 0xC100C0;
static const int32_t data_34120[2] = {12779714, 196};
static const uint32_t data_34128 = 0xD000CF;
static const int32_t data_3412C = 13762769;
static const uint32_t data_34130 = 0xE000DF;
static const int32_t data_34134 = 14811361;
static const char data_34138[169] = "%GACR016G.MT5\000%GACK66BG.MT5\000%GACK66RG.MT5\000%GACR0BNG.MT5\000%GACR0B2G.MT5\000%GACR0B3G.MT5\000%SPBK6REG.MT5\000%SPBK6BRG.MT5\000%SPBK6YEG.MT5\000%SAIK6WHG.MT5\000%SAIK6BKG.MT5\000%SAIK6REG.MT5";
static const char data_341E1[315] = "%GACIAK1G.MT5\000%GACIAK2G.MT5\000%GACOJK1G.MT5\000%GACOJK2G.MT5\000%GACOSA1G.MT5\000%GACOSA2G.MT5\000%GACOLA1G.MT5\000%GACOLA2G.MT5\000%GACOPA1G.MT5\000%GACOPA2G.MT5\000%GACGWF1G.MT5\000%GACGWF2G.MT5\000%GACGJF1G.MT5\000%GACGJF2G.MT5\000%GACIKA1G.MT5\000%GACIKA2G.MT5\000%GACM401G.MT5\000%GACIAA1G.MT5\000%GACIAA2G.MT5\000D\000D\000D\000D\000D\000D\000D\000D\000D\000D\000%GACGJJ1G.MT5\000%GACGJJ2G.MT5";
static const char data_3431C[309] = "%GACIAKAG.MT5\000%GACIAKBG.MT5\000%GACGJKAG.MT5\000%GACGJKBG.MT5\000%GACOSAAG.MT5\000%GACOSABG.MT5\000%GACOLAAG.MT5\000%GACOLABG.MT5\000%GACOPAAG.MT5\000%GACOPABG.MT5\000%GACIWLAG.MT5\000%GACIWLBG.MT5\000%GACGJFAG.MT5\000%GACGJFBG.MT5\000%GACIKAAG.MT5\000%GACIKABG.MT5\000%GACGSNAG.MT5\000%GACGSNBG.MT5\000%GACILIAG.MT5\000%GACILIBG.MT5\000%GACIDUAG.MT5\000%GACIDUBG.MT5";
static const char data_34451[167] = "%GACRSO1G.MT5\000%GACRSO2G.MT5\000%GACRTL1G.MT5\000%GACRNUCG.MT5\000%GACRBAKG.MT5\000%GACRBING.MT5\000%GACRFNGG.MT5\000%GACRAMYG.MT5\000%GACRESPG.MT5\000%GACREGGG.MT5\000D\000D\000D\000D\000D\000D\000%GACRFNNG.MT5";
static const char data_344F8[365] = "%GACRNT1G.MT5\000%GACRNT2G.MT5\000%GACRLOPG.MT5\000%GACRMYUG.MT5\000%GACRCKNG.MT5\000%GACRALFG.MT5\000%GACRRISG.MT5\000%GACRARXG.MT5\000%GACR0OPG.MT5\000%GACR0O2G.MT5\000%GACRPOPG.MT5\000%GACRCOBG.MT5\000%GACRIDAG.MT5\000%GACRSHPG.MT5\000%GACRMOBG.MT5\000%GACRROBG.MT5\000%GACK6TZG.MT5\000%GACRPANG.MT5\000%GACK6DYG.MT5\000D\000D\000D\000D\000D\000D\000D\000%GACRAXXG.MT5\000D\000D\000D\000D\000D\000D\000D\000%GACRRBBG.MT5\000%GACK6TSG.MT5\000%GACRPPNG.MT5\000%GACK6DSG.MT5";
static const char data_34665[505] = "%GACK6HHG.MT5\000%GACK6HYG.MT5\000%GACK6HBG.MT5\000%GACK6HGG.MT5\000%GACH5BKG.MT5\000%KAB02FSG.MT5\000%GACO1K2G.MT5\000%GACO1K3G.MT5\000%GACO1K7G.MT5\000%GACO1K8G.MT5\000%GACO1K9G.MT5\000%GACO1K1G.MT5\000%GACO1K4G.MT5\000%GACO1K5G.MT5\000%GACO1K6G.MT5\000%GACS5BSG.MT5\000%GACK6HOG.MT5\000%GACK6HKG.MT5\000%GACK6XHG.MT5\000%GACK6XYG.MT5\000%GACK6XBG.MT5\000%GACK6XGG.MT5\000%GACK6BKG.MT5\000%KABK6FSG.MT5\000%GACOKK2G.MT5\000%GACOKK3G.MT5\000%GACOKK7G.MT5\000%GACOKK8G.MT5\000%GACOKK9G.MT5\000%GACOKK1G.MT5\000%GACOKK4G.MT5\000%GACOKK5G.MT5\000%GACOKK6G.MT5\000%GACS5BBG.MT5\000%GACK6XOG.MT5\000%GACK6XKG.MT5";
static const char data_3485E[211] = "%GACH5TKG.MT5\000%GACGTRXG.MT5\000%GACO1T1G.MT5\000%GACO1T2G.MT5\000%GACO1KOG.MT5\000%GACO1KBG.MT5\000%GACO1KJG.MT5\000%GACH5UKG.MT5\000%GACH5IKG.MT5\000%GACO1FOG.MT5\000%GACGSHEG.MT5\000%GACH5TTG.MT5\000%GACGTXXG.MT5\000%GACOTT1G.MT5\000%GACOTT2G.MT5";
static const char data_34931[113] = "%YKW0201G.MT5\000%YWT0201G.MT5\000%GACRBONG.MT5\000%GACRRENG.MT5\000%GACK6SPG.MT5\000%GACG360G.MT5\000%GACRDR1G.MT5\000%GACRDR2G.MT5";
static const char data_349A2[77] = "%GACRSSCG.MT5\000%GACRMETG.MT5\000%GACRGBIG.MT5\000%GACH5TRG.MT5\000D\000D\000D\000%GACH5RRG.MT5";
static const char data_349EF[57] = "%GACS5FRG.MT5\000%GACS5FBG.MT5\000%GACO1KUG.MT5\000%GACO1FEG.MT5";
static const char data_34A28[239] = "%GACODG1G.MT5\000%GACODG2G.MT5\000%GACODG3G.MT5\000%GACOCT2G.MT5\000%GACOCATG.MT5\000%GACO1SHG.MT5\000%GACOKSHG.MT5\000%GACO1IRG.MT5\000%GACOKIRG.MT5\000%GACO1HAG.MT5\000%GACOHA2G.MT5\000%GACOHA3G.MT5\000%GACOHA4G.MT5\000%GACO1YGG.MT5\000%GACO1USG.MT5\000%GACOOSHG.MT5\000%GACOOIRG.MT5";
static const char data_34B17[99] = "%GACM400G.MT5\000%GACGSPHG.MT5\000%GACS5HGG.MT5\000%GACGDT1G.MT5\000%GACS5EQG.MT5\000%SYOGSH0G.MT5\000%SYOGHG0G.MT5";
static const char data_34B7A[85] = "%GACGCRYG.MT5\000%GACGBIRG.MT5\000%GACH5SLG.MT5\000%GACH5JKG.MT5\000%SYOGMD1G.MT5\000%SYOGMD5G.MT5";
static const char data_34BCF[71] = "%GACS5N1G.MT5\000%GACS5N2G.MT5\000%GACS5N3G.MT5\000%GACS5N4G.MT5\000%GACS5N5G.MT5";
static const char data_34C16[57] = "%GACGXBNG.MT5\000%GACGYBNG.MT5\000%GACGABNG.MT5\000%GACGBBNG.MT5";
static const char data_34C4F[57] = "%GACRMEGG.MT5\000%GACR0GGG.MT5\000%GACRSATG.MT5\000%GACRCASG.MT5";
static const uint32_t data_34C88 = SHEN_NAME('GCH0');
static const shen_name data_34C8C[5] = {SHEN_NAME('GCH1'), SHEN_NAME('GCH2'), SHEN_NAME('GCH3'), SHEN_NAME('GCH4'), SHEN_NAME('GCH5')};
static const uint32_t data_34CA0 = SHEN_NAME('HGC0');
static const shen_name data_34CA4[5] = {SHEN_NAME('HGC1'), SHEN_NAME('HGC2'), SHEN_NAME('HGC3'), SHEN_NAME('HGC4'), SHEN_NAME('HGC5')};
static const uint32_t data_34CB8 = SHEN_NAME('GBX0');
static const shen_name data_34CBC[5] = {SHEN_NAME('GBX1'), SHEN_NAME('GBX2'), SHEN_NAME('GBX3'), SHEN_NAME('GBX4'), SHEN_NAME('GBX5')};
static const uint32_t data_34CD0 = 0x41C40000;
typedef struct Row_34CD4 { int32_t w0; float w1; int32_t w2; int32_t w3; float w4; } Row_34CD4;
static const Row_34CD4 data_34CD4[5] = {
  {0, 32.16f, 32768, 516, 0.0f},
  {0, 0.0f, 0, 0, 0.0f},
  {0, 0.0f, 0, 0, 23.75f},
  {0, 32.16f, 32768, 770, 0.0f},
  {0, 0.0f, 0, 0, 0.0f}
};
static const int32_t data_34D38[4] = {0, 0, 0, 0};
static const Vector3 data_34D48 = {.x = 0.29f, .y = 0.0f, .z = 0.088f};
static const Vector3i data_34D54 = {.x = 0, .y = 0, .z = 0};
static const char data_34D60[6] __attribute__((scn_string_literal)) = "E1066";
static const char data_34D66[13] __attribute__((scn_string_literal)) = "e1gachap.snd";
static const char data_34D73[16] __attribute__((scn_string_literal)) = "/scene/03/D000/";
static const char data_34D83[8] __attribute__((scn_string_literal)) = "HI_GACH";
static const char data_34D8B[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_34D90[6] __attribute__((scn_string_literal)) = "E1066";
static const char data_34D96[8] __attribute__((scn_string_literal)) = "HI_GACH";
static const char data_34D9E[10] __attribute__((scn_string_literal)) = "HI_GACH";
static const Vector4 data_34DA8 = {.x = 0.0f, .y = 0.7f, .z = 0.7f, .w = 0.7f};
static const uint8_t data_34DB8[4] = {0};
static const int32_t data_34DBC[56] = {
  -546, -5870, 0, 2184, -3726, 0, 182, 0, 0, 0, -5184, 0, 71525, -12075, 0, -182, -5279, 0, -364, 910, 0, 1456, -2894, 0, 3458, -13352, 0, -910,
  -10327, 0, 0, 1820, 0, 0, -6863, 0, 1820, -16438, 0, -364, -10150, 0, 236, 291, 0, 0, -12105, 0, 0, -15539, 0, 0, -7249, 0, -819, -364
};
static const uint8_t data_34E9C[4] = {0};
static const int32_t data_34EA0[56] = {
  -546, -5870, 0, 2184, -3726, 0, 182, 0, 0, 0, -5184, 0, 71525, -10922, 0, -182, -5279, 0, -364, 910, 0, 1456, -2894, 0, 3458, -13352, 0, -910,
  -10327, 0, 0, 1820, 0, 0, -6863, 0, 1820, -16438, 0, -364, -10150, 0, 236, 291, 0, 0, -12105, 0, 0, -15539, 0, 0, -7249, 0, -819, -364
};
static const uint8_t data_34F80[4] = {0};
static const int32_t data_34F84[56] = {
  2912, -7507, 0, 6189, -4454, 0, -1456, 0, 0, 546, -8266, 0, 1292, -7099, 0, 910, -11468, 0, 728, 3276, 0, 1092, -11450, 0, 546, -10256, 0, 1456,
  -6504, 0, 546, 1638, 0, -1092, -12142, 0, -364, -16438, 0, -364, -10150, 0, -309, 291, 0, 1638, -11923, 0, -3276, -17359, 0, -2912, -10707, 0, -819,
  -364
};
static const uint8_t data_35064[4] = {0};
static const int32_t data_35068[56] = {
  1638, -7507, 0, 6189, -4454, 0, -1456, 0, 0, 546, -17731, 0, -709, -9466, 0, 728, -15473, 0, 182, 2548, 0, 1092, -13271, 0, -1092, -14625, 0, 1638,
  -11965, 0, 0, 1638, 0, -1092, -12142, 0, -364, -16438, 0, -364, -10150, 0, -309, 291, 0, 1638, -11923, 0, -3276, -17359, 0, -2912, -10707, 0, -819,
  -364
};
static const uint8_t data_35148[4] = {0};
static const int32_t data_3514C[56] = {
  1638, -6779, 0, 6189, -4454, 0, -1456, 0, 0, 546, -17731, 0, -709, -9466, 0, 1274, -15473, 0, 546, 2548, 0, 1092, -13271, 0, -1092, -14625, 0, 1638,
  -11965, 0, -910, 1638, 0, -1092, -12142, 0, -364, -16438, 0, -364, -10150, 0, -309, 291, 0, 1638, -12105, 0, -3276, -17359, 0, -2912, -10889, 0,
  -819, -364
};
static const uint8_t data_3522C[4] = {0};
static const int32_t data_35230[56] = {
  -2730, -9510, -3094, 4004, -1359, 0, 4004, -728, 0, 546, -6990, 0, 18, -4004, 2366, 1274, -11468, 0, 182, -182, 0, 1092, -13271, 0, -1092, -14443,
  0, 1638, -11965, 0, -910, 1638, 0, -1092, -12142, 0, -364, -16438, 0, -364, -10150, 0, -309, 291, 0, 1638, -12105, 0, -3276, -17359, 0, -2912,
  -10889, 0, -819, -364
};
static const uint8_t data_35310[8] = {0};
static const int32_t data_35318[55] = {
  -6005, 0, 4731, -6188, 0, 0, 0, 0, 0, -2727, 0, 0, -9097, 0, 0, -5457, 0, 0, 0, 0, 0, -8734, 0, -909, -7277, 0, 0, -7823, 0, 0, 0, 0, 1456, -4002,
  0, 0, -9462, 0, 726, -7824, 0, 0, 0, 0, 0, -12011, 0, -1091, -7461, 0, -1636, -9280, 0, 0, 0
};
static const uint8_t data_353F4[4] = {0};
static const int32_t data_353F8[56] = {
  -546, -5870, 0, 2184, -3726, 0, 182, 0, 0, 0, -5184, 0, 71525, -12075, 0, -182, -5279, 0, -364, 910, 0, 1456, -2894, 0, 3458, -13352, 0, -910,
  -10327, 0, 0, 1820, 0, 0, -6863, 0, 1820, -16438, 0, -364, -10150, 0, 236, 291, 0, 0, -12105, 0, 0, -15539, 0, 0, -7249, 0, -819, -364
};
static const uint8_t data_354D8[8] = {0};
static const int32_t data_354E0[55] = {
  -8046, 0, 0, -1820, -13088, 4350, -2894, 0, 0, -9011, 0, 0, -18422, 0, 0, -9812, 0, 0, 0, 0, 0, -7518, 0, 0, -17385, 0, 0, -11559, 0, 0, 0, 0, 0,
  -5734, 0, 0, -18058, 0, 0, -12233, 0, 0, 0, 0, 0, -7827, 0, 0, -16183, 0, 0, -13981, 0, -72, 0
};
static const Vector3 data_355BC = {.x = 0.067f, .y = 0.061f, .z = 0.0379f};
static const uint32_t data_355C8 = 0x3DB851EC;
static const float data_355CC[2] = {0.075f, 0.008f};
static const Vector3i data_355D4 = {.x = 13111, .y = 817, .z = 50684};
static const Vector3 data_355E0 = {.x = 0.091f, .y = -0.087f, .z = 0.0671f};
static const Vector3i data_355EC = {.x = 20024, .y = 65353, .z = 11832};
static const Vector3 data_355F8 = {.x = 0.0f, .y = 0.0f, .z = 0.0f};
static const Vector3i data_35604 = {.x = 0, .y = 0, .z = 0};
static const Vector3 data_35610 = {.x = -0.042f, .y = 0.1735f, .z = 0.2062f};
static const char data_3561C[] __attribute__((scn_string_literal)) = "E1066A001";
static const char data_35626[] __attribute__((scn_string_literal)) = "E1066A002";
static const char data_35630[] __attribute__((scn_string_literal)) = "E1066A003";
static const char data_3563A[] __attribute__((scn_string_literal)) = "E1066A004";
static const char data_35644[] __attribute__((scn_string_literal)) = "E1066A005";
static const char data_3564E[] __attribute__((scn_string_literal)) = "E1066A006";
static const char data_35658[] __attribute__((scn_string_literal)) = "E1066A007";
static const char data_35662[] __attribute__((scn_string_literal)) = "E1066A008";
static const char data_3566C[] __attribute__((scn_string_literal)) = "E1066A009";
static const char data_35676[] __attribute__((scn_string_literal)) = "E1066A010";
static const char data_35680[] __attribute__((scn_string_literal)) = "E1066A011";
static const char data_3568A[] __attribute__((scn_string_literal)) = "E1066A026";
static const char data_35694[] __attribute__((scn_string_literal)) = "E1066A027";
static const char data_3569E[] __attribute__((scn_string_literal)) = "E1066A019";
static const char data_356A8[] __attribute__((scn_string_literal)) = "E1066A020";
static const char data_356B2[] __attribute__((scn_string_literal)) = "E1066A021";
static const char data_356BC[] __attribute__((scn_string_literal)) = "E1066A014";
static const char data_356C6[] __attribute__((scn_string_literal)) = "E1066A015";
static const char data_356D0[] __attribute__((scn_string_literal)) = "E1066A016";
static const char data_356DA[] __attribute__((scn_string_literal)) = "E1066A017";
static const char data_356E4[] __attribute__((scn_string_literal)) = "E1066A018";
static const char data_356EE[] __attribute__((scn_string_literal)) = "E1066A022";
static const char data_356F8[] __attribute__((scn_string_literal)) = "E1066A025";
static const char data_35702[] __attribute__((scn_string_literal)) = "E1066A024";
static const char data_3570C[12] __attribute__((scn_string_literal)) = "E1066A023";
static const Vector3 data_35718 = {.x = 0.337795f, .y = 0.596267f, .z = -0.181433f};
static const Vector3 data_35724 = {.x = 0.091f, .y = -0.087f, .z = 0.0671f};
static const Vector3i data_35730 = {.x = 20024, .y = 65353, .z = 11832};
static const char data_3573C[12] __attribute__((scn_string_literal)) = "JINGURU01";
static Vector3i data_35748 = {.x = 0, .y = -910, .z = 0};
static const uint8_t data_35754[20] = {0};
static const int32_t data_35768[9] = {-21173, 0, -1, 0, 0, 0, 0, 0, 0};
static const uint32_t data_3578C = 1;
static const int32_t data_35790[13] = {0, 0, 0, 0, 16384, 0, -1, 0, 0, 0, 0, 0, 0};
static const uint32_t data_357C4 = 2;
static const int32_t data_357C8[13] = {0, 0, 0, 0, -32768, 0, -1, 0, 0, 0, 0, 0, 0};
static const uint32_t data_357FC = 3;
static const int32_t data_35800[13] = {0, 0, 0, 0, -4952, 0, -1, 0, 0, 0, 0, 0, 0};
static const uint32_t data_35834 = 4;
static const int32_t data_35838[13] = {0, 0, 0, 0, 32768, 0, -1, 0, 0, 0, 0, 0, 0};
static const uint32_t data_3586C = 5;
static const int32_t data_35870[13] = {0, 0, 0, 0, 25675, 0, -1, 0, 0, 0, 0, 0, 0};
static const uint32_t data_358A4 = 6;
static const int32_t data_358A8[13] = {0, 0, 0, 0, 41320, 0, -1, 0, 0, 0, 0, 0, 0};
static const uint32_t data_358DC = 7;
static const int32_t data_358E0[13] = {0, 0, 0, 0, -23276, 0, -1, 0, 0, 0, 0, 0, 0};
static char data_35914[] = "$B01MP01G.MT5";
static const char str_35922[] __attribute__((scn_string_literal)) = "$B01MP02G.MT5";
static const char str_35930[] __attribute__((scn_string_literal)) = "$B01MP03G.MT5";
static const char str_3593E[] __attribute__((scn_string_literal)) = "$BSPK3T1G.MT5";
static const char str_3594C[] __attribute__((scn_string_literal)) = "$B01MP04G.MT5";
static const char str_3595A[] __attribute__((scn_string_literal)) = "$B01MP05G.MT5";
static const char str_35968[] __attribute__((scn_string_literal)) = "$B01MP06G.MT5";
static const char str_35976[] __attribute__((scn_string_literal)) = "$B01MP07G.MT5";
static const uint8_t pad_35984[4] = {0};
static const uint32_t data_35988 = 0xFFFF0000;
static const uint32_t data_3598C[3] = {0, 0x8000A7D2U, 0x4200914EU};
static const float data_35998 = 1.1753f;
static const uint32_t data_3599C[3] = {0x41B0C49CU, 0, 0};
static const int32_t data_359A8[2] = {-65536, 1};
static const uint32_t data_359B0 = 0x80004000U;
static const float data_359B4[5] = {15.3191f, 1.1753f, 43.8f, 0.0f, 0.0f};
static const int32_t data_359C8[2] = {-65536, 2};
static const uint32_t data_359D0 = 0x80007E93U;
static const float data_359D4[5] = {-25.4825f, 1.1753f, 44.4692f, 0.0f, 0.0f};
static const int32_t data_359E8[2] = {-65536, 3};
static const uint32_t data_359F0[2] = {0x80008000U, 0xC27A2305U};
static const float data_359F8 = 1.1753f;
static const uint32_t data_359FC[3] = {0x42B4B70AU, 0, 0};
static const int32_t data_35A08[2] = {-65536, 4};
static const uint32_t data_35A10 = 0x8000644BU;
static const float data_35A14[2] = {6.5485f, 1.1753f};
static const uint32_t data_35A1C[3] = {0x42B7D879U, 0, 0};
static const int32_t data_35A28[2] = {-65536, 5};
static const uint32_t data_35A30[2] = {0x8000A168U, 0xC2DCF74CU};
static const float data_35A38[4] = {1.1753f, 76.7537f, 0.0f, 0.0f};
static const int32_t data_35A48[2] = {-65536, 6};
static const uint32_t data_35A50[2] = {0x8000A513U, 0xC2D5A6E9U};
static const float data_35A58 = 1.1753f;
static const uint32_t data_35A5C[3] = {0x42CCDAEEU, 0, 0};
static const int32_t data_35A68[2] = {-65536, 7};
static const uint32_t data_35A70[2] = {0x8000E71CU, 0x422A2E98U};
static const float data_35A78[4] = {1.8f, 5.1289f, 0.0f, 0.0f};
static const int32_t data_35A88[2] = {-65536, 8};
static const uint32_t data_35A90[2] = {0x8000E71CU, 0x422A2E98U};
static const float data_35A98[4] = {1.2768f, 5.1289f, 0.0f, 0.0f};
static const int32_t data_35AA8[2] = {-65536, 133};
static const uint32_t data_35AB0[2] = {0x8000B1C7U, 0x421A21CBU};
static const float data_35AB8[5] = {1.1f, 9.521f, 0.0f, 0.0f, 0.0f};
static const shen_name data_35ACC = SHEN_NAME('TBK1');
static const uint32_t data_35AD0[2] = {0x8000B1C7U, 0x38CB6C7AU};
static const float data_35AD8[4] = {0.130716f, 0.002221f, 0.0f, 0.0f};
static const int32_t data_35AE8[2] = {-65536, 100};
static const uint32_t data_35AF0 = 0x8000AB86U;
static const float data_35AF4[5] = {37.43f, 1.58f, 10.49f, 0.0f, 0.0f};
static const int32_t data_35B08[2] = {-65536, 101};
static const uint32_t data_35B10 = 0x80002DDDU;
static const float data_35B14[5] = {29.09f, 1.43f, 10.9f, 0.0f, 0.0f};
static const int32_t data_35B28[2] = {-65536, 102};
static const uint32_t data_35B30 = 0x80002CFAU;
static const float data_35B34[5] = {21.93f, 1.5f, 25.05f, 0.0f, 0.0f};
static const int32_t data_35B48[2] = {-65536, 103};
static const uint32_t data_35B50 = 0x80008003U;
static const float data_35B54[5] = {20.56f, 1.62f, 32.69f, 0.0f, 0.0f};
static const int32_t data_35B68[2] = {-65536, 104};
static const uint32_t data_35B70 = 0x8000FE87U;
static const float data_35B74[2] = {2.9916f, 1.0886f};
static const uint32_t data_35B7C[3] = {0x41D40866U, 0, 0};
static const int32_t data_35B88[2] = {-65536, 105};
static const uint32_t data_35B90 = 0x80007F34U;
static const float data_35B94[5] = {3.53f, 1.2f, 32.65f, 0.0f, 0.0f};
static const int32_t data_35BA8[2] = {-65536, 106};
static const uint32_t data_35BB0 = 0x8000FD8BU;
static const float data_35BB4[5] = {-8.4f, 0.8f, 25.8f, 0.0f, 0.0f};
static const int32_t data_35BC8[2] = {-65536, 107};
static const uint32_t data_35BD0 = 0x80001A7EU;
static const float data_35BD4[5] = {-15.36f, 1.08f, 28.93f, 0.0f, 0.0f};
static const int32_t data_35BE8[2] = {-65536, 108};
static const uint32_t data_35BF0 = 0x8000196DU;
static const float data_35BF4[5] = {-19.24f, 1.24f, 32.36f, 0.0f, 0.0f};
static const int32_t data_35C08[2] = {-65536, 109};
static const uint32_t data_35C10 = 0x800016CAU;
static const float data_35C14[5] = {-53.15f, 1.33f, 41.13f, 0.0f, 0.0f};
static const int32_t data_35C28[2] = {-65536, 110};
static const uint32_t data_35C30 = 0x80009AC2U;
static const float data_35C34[5] = {-51.9f, 1.41f, 50.03f, 0.0f, 0.0f};
static const int32_t data_35C48[2] = {-65536, 111};
static const uint32_t data_35C50 = 0x80001930U;
static const float data_35C54[5] = {-58.06f, 0.96f, 45.03f, 0.0f, 0.0f};
static const int32_t data_35C68[2] = {-65536, 112};
static const uint32_t data_35C70 = 0x8000C0A7U;
static const float data_35C74[5] = {-63.29f, 1.56f, 66.86f, 0.0f, 0.0f};
static const int32_t data_35C88[2] = {-65536, 113};
static const uint32_t data_35C90[2] = {0x8000C2C5U, 0xC27CB4F1U};
static const float data_35C98 = 1.6068f;
static const uint32_t data_35C9C[3] = {0x428F2EA5U, 0, 0};
static const int32_t data_35CA8[2] = {-65536, 114};
static const uint32_t data_35CB0 = 0x80008000U;
static const float data_35CB4[5] = {-69.82f, 1.75f, 91.66f, 0.0f, 0.0f};
static const int32_t data_35CC8[2] = {-65536, 115};
static const uint32_t data_35CD0 = 0x8000148EU;
static const float data_35CD4[5] = {-54.42f, 1.56f, 81.22f, 0.0f, 0.0f};
static const int32_t data_35CE8[2] = {-65536, 116};
static const uint32_t data_35CF0 = 0x80000C73U;
static const float data_35CF4[5] = {-41.95f, 1.19f, 74.58f, 0.0f, 0.0f};
static const int32_t data_35D08[2] = {-65536, 117};
static const uint32_t data_35D10 = 0x8000FE68U;
static const float data_35D14[5] = {-29.54f, 1.04f, 72.51f, 0.0f, 0.0f};
static const int32_t data_35D28[2] = {-65536, 118};
static const uint32_t data_35D30 = 0x8000E21AU;
static const float data_35D34[5] = {6.33f, 1.59f, 84.17f, 0.0f, 0.0f};
static const int32_t data_35D48[2] = {-65536, 119};
static const uint32_t data_35D50 = 0x80000C96U;
static const float data_35D54[5] = {-93.08f, 1.53f, 61.81f, 0.0f, 0.0f};
static const int32_t data_35D68[2] = {-65536, 120};
static const uint32_t data_35D70[2] = {0x800091EAU, 0xC2BC7909U};
static const float data_35D78 = 1.693f;
static const uint32_t data_35D7C[3] = {0x42854DACU, 0, 0};
static const int32_t data_35D88[2] = {-65536, 121};
static const uint32_t data_35D90 = 0x800011C7U;
static const float data_35D94[5] = {-99.52f, 1.7f, 65.19f, 0.0f, 0.0f};
static const int32_t data_35DA8[2] = {-65536, 122};
static const uint32_t data_35DB0 = 0x800013A4U;
static const float data_35DB4[5] = {-111.81f, 1.61f, 69.88f, 0.0f, 0.0f};
static const int32_t data_35DC8[2] = {-65536, 123};
static const uint32_t data_35DD0 = 0x80006046U;
static const float data_35DD4[5] = {-126.7f, -0.13f, 64.72f, 0.0f, 0.0f};
static const int32_t data_35DE8[2] = {-65536, 124};
static const uint32_t data_35DF0 = 0x800047B5U;
static const float data_35DF4[5] = {-124.49f, 1.58f, 87.91f, 0.0f, 0.0f};
static const int32_t data_35E08[2] = {-65536, 125};
static const uint32_t data_35E10 = 0x80004A32U;
static const float data_35E14[5] = {-123.44f, 1.23f, 94.43f, 0.0f, 0.0f};
static const int32_t data_35E28[2] = {-65536, 126};
static const uint32_t data_35E30 = 0x80006F49U;
static const float data_35E34[5] = {-114.06f, 1.31f, 101.77f, 0.0f, 0.0f};
static const int32_t data_35E48[2] = {-65536, 127};
static const uint32_t data_35E50 = 0x80006D47U;
static const float data_35E54[5] = {-109.53f, 1.32f, 102.17f, 0.0f, 0.0f};
static const int32_t data_35E68[2] = {-65536, 128};
static const uint32_t data_35E70 = 0x800026A0U;
static const float data_35E74[5] = {-106.34f, 1.64f, 93.77f, 0.0f, 0.0f};
static const int32_t data_35E88[2] = {-65536, 129};
static const uint32_t data_35E90 = 0x80001418U;
static const float data_35E94[5] = {-94.02f, 1.69f, 83.69f, 0.0f, 0.0f};
static const int32_t data_35EA8[2] = {-65536, 130};
static const uint32_t data_35EB0 = 0x80003BB6U;
static const float data_35EB4[6] = {-69.96f, 1.88f, 79.5f, 0.0f, 0.0f, 0.0f};
static const shen_name data_35ECC = SHEN_NAME('GCH0');
static const float data_35ED0[7] = {40064.0f, 0.0f, 0.76f, 0.055f, 0.0f, 0.0f, 0.0f};
static const shen_name data_35EEC = SHEN_NAME('GCH1');
static const float data_35EF0[7] = {40064.0f, 0.0f, 0.76f, 0.055f, 0.0f, 0.0f, 0.0f};
static const shen_name data_35F0C = SHEN_NAME('GCH2');
static const float data_35F10[7] = {40064.0f, 0.0f, 0.76f, 0.055f, 0.0f, 0.0f, 0.0f};
static const shen_name data_35F2C = SHEN_NAME('GCH3');
static const float data_35F30[7] = {40064.0f, 0.0f, 0.76f, 0.055f, 0.0f, 0.0f, 0.0f};
static const shen_name data_35F4C = SHEN_NAME('GCH4');
static const float data_35F50[7] = {40064.0f, 0.0f, 0.76f, 0.055f, 0.0f, 0.0f, 0.0f};
static const shen_name data_35F6C = SHEN_NAME('GCH5');
static const float data_35F70[6] = {40064.0f, 0.0f, 0.76f, 0.055f, 0.0f, 0.0f};
static const int32_t data_35F88[2] = {-65536, 131};
static const uint32_t data_35F90 = 0x8000C367U;
static const float data_35F94[5] = {20.06f, 1.61f, 47.94f, 0.0f, 0.0f};
static const int32_t data_35FA8[2] = {-65536, 132};
static const uint32_t data_35FB0[2] = {0x800024C5U, 0xC2F61C1CU};
static const float data_35FB8 = 1.2514f;
static const uint32_t data_35FBC[3] = {0x42A6B8BBU, 0, 0};
static const int32_t data_35FC8[2] = {-65535, 134};
static const uint32_t data_35FD0 = 0x71C799F4U;
static const float data_35FD4[2] = {-18.28f, 1.17f};
static const uint32_t data_35FDC = 0x421A0F5CU;
static const int32_t data_35FE0 = -1;
static uint32_t data_35FE4 = 0x4200914E;
static const float data_35FE8 = 1.1753f;
static const uint32_t data_35FEC = 0x41B0C49CU;
static uint32_t data_35FF0 = 0x41751B09;
static const float data_35FF4[2] = {1.1753f, 43.8f};
static uint32_t data_35FFC = 0xC1CBDC29;
static const float data_36000[2] = {1.1753f, 44.472f};
static uint32_t data_36008 = 0xC27A2305;
static const float data_3600C = 1.1753f;
static const uint32_t data_36010 = 0x42B4B70AU;
static uint32_t data_36014 = 0x40D1851F;
static const float data_36018 = 1.1753f;
static const uint32_t data_3601C = 0x42B7D7F6U;
static uint32_t data_36020 = 0xC2DCF7CF;
static const float data_36024 = 1.1753f;
static const uint32_t data_36028 = 0x429981D8U;
static uint32_t data_3602C = 0xC2D5A76D;
static const float data_36030 = 1.1753f;
static const uint32_t data_36034 = 0x42CCDAE1U;
static uint32_t data_36038 = 0x422A2E98;
static const float data_3603C[2] = {1.2768f, 5.1289f};
static Vector3i data_36044 = {.x = 0, .y = 14108, .z = 0};
static Vector3i data_36050 = {.x = 0, .y = 30474, .z = 0};
static const float data_3605C[3] = {1.62f, 1.0f, 3.4f};
static Vector3i data_36068 = {.x = 0, .y = 14108, .z = 0};
static Vector3i data_36074 = {.x = 0, .y = -30474, .z = 0};
static Vector3i data_36080 = {.x = 0, .y = -14108, .z = 0};
static Vector3i data_3608C = {.x = 0, .y = 30474, .z = 0};
static const Vector3 data_36098 = {.x = 3.1f, .y = 2.65f, .z = 8.0f};
static const Vector4 data_360A4 = {.x = 1.0f, .y = 0.4f, .z = 0.4f, .w = 0.4f};
static Vector3i data_360B4 = {.x = 2730, .y = 0, .z = 0};
static Vector3i data_360C0 = {.x = -2730, .y = 0, .z = 0};
static const char data_360CC[11] __attribute__((scn_string_literal)) = "SA1076A001";
static const char data_360D7[11] __attribute__((scn_string_literal)) = "SA1076A013";
static const char data_360E2[11] __attribute__((scn_string_literal)) = "SA1076A002";
static const char data_360ED[11] __attribute__((scn_string_literal)) = "SA1076A014";
static const char data_360F8[11] __attribute__((scn_string_literal)) = "SA1076A003";
static const char data_36103[11] __attribute__((scn_string_literal)) = "SA1076A015";
static const char data_3610E[11] __attribute__((scn_string_literal)) = "SA1076A015";
static const char data_36119[11] __attribute__((scn_string_literal)) = "SA1076A016";
static const char data_36124[12] __attribute__((scn_string_literal)) = "SA1076A017";
static const Vector3 data_36130 = {.x = 42.199902f, .y = 0.0f, .z = 5.9f};
static const Vector3 data_3613C = {.x = 42.599899f, .y = 0.0f, .z = 6.8f};
static const Vector3 data_36148 = {.x = 43.4921f, .y = 0.0f, .z = 5.1527f};
static const char data_36154[] __attribute__((scn_string_literal)) = "BUSS";
static const char data_36159[13] __attribute__((scn_string_literal)) = "a1_busno.snd";
static const char data_36166[] __attribute__((scn_string_literal)) = "/scene/03/D000/";
static const char data_36176[] __attribute__((scn_string_literal)) = "BUSS";
static const char data_3617B[13] __attribute__((scn_string_literal)) = "seqdata1.bin";
static const char data_36188[5] __attribute__((scn_string_literal)) = "BUSS";
static const char data_3618D[13] __attribute__((scn_string_literal)) = "seqdata4.bin";
static const char data_3619A[5] __attribute__((scn_string_literal)) = "BUSS";
static const char data_3619F[13] __attribute__((scn_string_literal)) = "a1_busno.snd";
static const char data_361AC[] __attribute__((scn_string_literal)) = "/scene/03/D000/";
static const char data_361BC[] __attribute__((scn_string_literal)) = "BUSS";
static const char data_361C1[13] __attribute__((scn_string_literal)) = "seqdata2.bin";
static const char data_361CE[5] __attribute__((scn_string_literal)) = "BUSS";
static const char data_361D3[13] __attribute__((scn_string_literal)) = "seqdata5.bin";
static const char data_361E0[8] __attribute__((scn_string_literal)) = "BUSS";
static const Vector4 data_361E8 = {.x = 0.3f, .y = 0.7f, .z = 0.7f, .w = 0.7f};
static const int32_t data_361F8[57] = {
  0, 0, -4004, 0, 0, -546, 0, 364, -1638, 0, 0, -7827, 0, 0, -11468, 0, 0, -3822, 0, 0, 0, 0, 0, -8192, 0, 0, -18022, 0, 0, -3276, 0, 0, 0, 0, 0,
  -7827, 0, 0, -18022, 0, 0, -5279, 0, 0, 0, 0, 910, -12014, 0, -2184, -16748, 0, -2002, -8738, 0, 0, 364
};
static const char data_362DC[6] __attribute__((scn_string_literal)) = "YKB_M";
static const char data_362E2[7] __attribute__((scn_string_literal)) = "SA1053";
static const char data_362E9[11] __attribute__((scn_string_literal)) = "SA1053A001";
static const char data_362F4[11] __attribute__((scn_string_literal)) = "SA1053A002";
static const char data_362FF[11] __attribute__((scn_string_literal)) = "SA1053B007";
static const char data_3630A[] __attribute__((scn_string_literal)) = "SA1053B001";
static const char data_36315[11] __attribute__((scn_string_literal)) = "SA1053A003";
static const char data_36320[11] __attribute__((scn_string_literal)) = "SA1053B002";
static const char data_3632B[11] __attribute__((scn_string_literal)) = "SA1053B003";
static const char data_36336[11] __attribute__((scn_string_literal)) = "SA1053A004";
static const char data_36341[11] __attribute__((scn_string_literal)) = "SA1053B004";
static const char data_3634C[11] __attribute__((scn_string_literal)) = "SA1053B005";
static const char data_36357[11] __attribute__((scn_string_literal)) = "SA1053B006";
static const char data_36362[11] __attribute__((scn_string_literal)) = "SA1053A005";
static const char data_3636D[7] __attribute__((scn_string_literal)) = "SA1053";
static const char data_36374[11] __attribute__((scn_string_literal)) = "SA1080A003";
static const char data_3637F[11] __attribute__((scn_string_literal)) = "SA1080A002";
static const char data_3638A[11] __attribute__((scn_string_literal)) = "SA1080A004";
static const char data_36395[11] __attribute__((scn_string_literal)) = "SA1080A005";
static const char data_363A0[11] __attribute__((scn_string_literal)) = "SA1080A004";
static const char data_363AB[11] __attribute__((scn_string_literal)) = "SA1080A006";
static const char data_363B6[7] __attribute__((scn_string_literal)) = "SA1080";
static const char data_363BD[7] __attribute__((scn_string_literal)) = "SA1081";
static const char data_363C4[11] __attribute__((scn_string_literal)) = "SA1081A002";
static const char data_363CF[11] __attribute__((scn_string_literal)) = "SA1081A003";
static const char data_363DA[11] __attribute__((scn_string_literal)) = "SA1081A002";
static const char data_363E5[11] __attribute__((scn_string_literal)) = "SA1081A005";
static const char data_363F0[11] __attribute__((scn_string_literal)) = "SA1081A006";
static const char data_363FB[7] __attribute__((scn_string_literal)) = "SA1081";
static const char data_36402[7] __attribute__((scn_string_literal)) = "SA1088";
static const char data_36409[11] __attribute__((scn_string_literal)) = "SA1088A001";
static const char data_36414[11] __attribute__((scn_string_literal)) = "SA1088A002";
static const char data_3641F[11] __attribute__((scn_string_literal)) = "SA1088A003";
static const char data_3642A[11] __attribute__((scn_string_literal)) = "SA1088A002";
static const char data_36435[11] __attribute__((scn_string_literal)) = "SA1088A004";
static const char data_36440[11] __attribute__((scn_string_literal)) = "SA1088A003";
static const char data_3644B[11] __attribute__((scn_string_literal)) = "SA1088A005";
static const char data_36456[11] __attribute__((scn_string_literal)) = "SA1088A002";
static const char data_36461[11] __attribute__((scn_string_literal)) = "SA1088A003";
static const char data_3646C[8] __attribute__((scn_string_literal)) = "SA1088";
static const uint32_t data_36474 = 0x4192A3D7;
static const float data_36478 = 0.225f;
static const uint32_t data_3647C = 0x42738CCDU;
static const int32_t data_36480[3] = {32768, 1048832, 8};
static const uint32_t data_3648C = 0x41935A1DU;
static const float data_36490[3] = {1.8f, 62.9902f, 0.0f};
static const int32_t data_3649C[2] = {264, 8};
static const float data_364A4[2] = {19.5203f, 2.025f};
static const uint32_t data_364AC = 0x42822113U;
static const int32_t data_364B0[3] = {39321, 1048832, 15};
static const float data_364BC[2] = {22.1514f, 5.4f};
static const uint32_t data_364C4 = 0x4288AB6BU;
static const int32_t data_364C8[3] = {6553, 264, 15};
static const float data_364D4[2] = {-120.7995f, -1.573f};
static const uint32_t data_364DC = 0x429CA0ECU;
static const int32_t data_364E0[3] = {42143, 1048832, 8};
static const uint32_t data_364EC[3] = {0xC2EE475FU, 0, 0x429F3A93U};
static const int32_t data_364F8[3] = {9375, 264, 8};
static const float data_36504[2] = {-101.9963f, 0.2967f};
static const uint32_t data_3650C = 0x42C74241U;
static const int32_t data_36510[3] = {25486, 1048576, 12};
static const uint32_t data_3651C = 0xC2CFF724U;
static const float data_36520 = 2.75f;
static const uint32_t data_36524 = 0x42CC65A2U;
static const int32_t data_36528[3] = {58254, 8, 12};
static const uint32_t data_36534 = 0xC24CC20CU;
static const float data_36538[2] = {0.296f, 97.4198f};
static const int32_t data_36540[3] = {37501, 1048576, 12};
static const uint32_t data_3654C = 0xC246BAADU;
static const float data_36550 = 2.7716f;
static const uint32_t data_36554 = 0x42C898EFU;
static const int32_t data_36558[3] = {4642, 9, 12};
static const float data_36564[2] = {-32.9104f, 0.296f};
static const uint32_t data_3656C = 0x42B06A4BU;
static const int32_t data_36570[3] = {33314, 1048576, 12};
static const uint32_t data_3657C = 0xC202E388U;
static const float data_36580 = 2.7716f;
static const uint32_t data_36584 = 0x42B701D8U;
static const int32_t data_36588[3] = {546, 8, 12};
static const float data_36594[2] = {-7.6464f, 0.296f};
static const uint32_t data_3659C = 0x4290DBF5U;
static const int32_t data_365A0[3] = {11468, 1048576, 12};
static const float data_365AC[3] = {-10.6061f, 2.7716f, 70.9646f};
static const int32_t data_365B8[3] = {44252, 8, 12};
static const float data_365C4[2] = {-6.245f, 0.296f};
static const uint32_t data_365CC = 0x42BB1AA0U;
static const int32_t data_365D0[3] = {26594, 1048576, 12};
static const float data_365DC[2] = {-8.1412f, 2.7716f};
static const uint32_t data_365E4 = 0x42C083F1U;
static const int32_t data_365E8[3] = {59346, 8, 12};
static const uint32_t data_365F4 = 0x421AEF00U;
static const float data_365F8[2] = {5.4742f, 90.1437f};
static const int32_t data_36600[3] = {24576, 1048576, 12};
static const float data_3660C[2] = {36.3997f, 7.9525f};
static const uint32_t data_36614 = 0x42B8F893U;
static const int32_t data_36618[3] = {57344, 8, 12};
static const float data_36624 = 100000.0f;
static const uint8_t data_36628[8] = {0};
typedef struct Row_36630 { float w0; int32_t w1; int32_t w2; } Row_36630;
static const Row_36630 data_36630[5] = {
  {1.0f, 0, 1},
  {1.0f, 0, 2},
  {1.0f, 0, 3},
  {1.0f, 0, 4},
  {0.5f, 0, 5}
};
static const float data_3666C = 0.5f;
static const int32_t data_36670 = 3;
static const char data_36674[34] __attribute__((scn_string_literal)) = "DOBUITA EVENT START START START!\012";
static const char data_36696[20] __attribute__((scn_string_literal)) = "DOBUITA EVENT END!\012";
static const char data_366AA[26] __attribute__((scn_string_literal)) = "training check error!!!!\012";
static const char data_366C4[67] __attribute__((scn_string_literal)) = "Daily practice %d: Check: in range, setting training flag to true\012";
static const char data_36707[109] __attribute__((scn_string_literal)) = "Daily practice %d: Check: in range, training flag already true ?? Failed before due to NPC's so check again\012";
static const char data_36774[31] __attribute__((scn_string_literal)) = "VEND_MACHINE_CHECKER_ERROR!!!\012";
static const char data_36793[11] __attribute__((scn_string_literal)) = "SA1101A020";
static const char data_3679E[11] __attribute__((scn_string_literal)) = "SA1076A001";
static const char data_367A9[11] __attribute__((scn_string_literal)) = "SA1076A002";
static const char data_367B4[11] __attribute__((scn_string_literal)) = "SA1076A003";
static const char data_367BF[9] __attribute__((scn_string_literal)) = "SA1076";
static const uint32_t data_367C8 = 0xC1291EB8;
static const float data_367CC[386] = {
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
static const char str_36DD4[] __attribute__((scn_string_literal)) = "ff>A=\n\"B";
static const uint8_t data_36DDD[16] = {
  0x00, 0xB8, 0x40, 0x14, 0x2E, 0xB7, 0x42, 0x00, 0x00, 0x00, 0x41, 0x7B, 0x14, 0xDE, 0x41, 0xA4
};
static const uint8_t data_36DED[16] = {
  0x70, 0xEF, 0x41, 0x5C, 0x8F, 0xD6, 0x41, 0x1F, 0x85, 0xEB, 0x41, 0xE1, 0x7A, 0xE0, 0x41, 0x71
};
static const uint8_t data_36DFD[16] = {
  0x3D, 0xD8, 0x41, 0x00, 0x00, 0xE8, 0x41, 0x7B, 0x14, 0xDC, 0x41, 0xCD, 0xCC, 0xCC, 0xBD, 0x5C
};
static const uint8_t data_36E0D[16] = {
  0x8F, 0x52, 0x40, 0xAE, 0x47, 0xDF, 0x41, 0x00, 0x00, 0x80, 0x3F, 0x0A, 0xD7, 0xE3, 0x41, 0x00
};
static const uint8_t data_36E1D[16] = {
  0x00, 0x80, 0x3F, 0xAE, 0x47, 0x39, 0x41, 0x52, 0xB8, 0xCE, 0x41, 0xAE, 0x47, 0x39, 0x41, 0x33
};
static const uint8_t data_36E2D[16] = {
  0x33, 0xD7, 0x41, 0x8F, 0xC2, 0x0D, 0x41, 0x33, 0x33, 0xD7, 0x41, 0x8F, 0xC2, 0x0D, 0x41, 0xD7
};
static const uint8_t data_36E3D[16] = {
  0xA3, 0xCE, 0x41, 0xCD, 0xCC, 0xCC, 0xBD, 0x71, 0x3D, 0x5A, 0x40, 0x29, 0x5C, 0x23, 0x41, 0x00
};
static const uint8_t data_36E4D[16] = {
  0x00, 0x80, 0x3F, 0x48, 0xE1, 0xD2, 0x41, 0x00, 0x00, 0x80, 0x3F, 0x66, 0x66, 0x12, 0x41, 0x5C
};
static const uint8_t data_36E5D[16] = {
  0x8F, 0xFE, 0x41, 0x66, 0x66, 0x12, 0x41, 0xE1, 0x7A, 0x03, 0x42, 0xCD, 0xCC, 0xBC, 0x40, 0xE1
};
static const uint8_t data_36E6D[16] = {
  0x7A, 0x03, 0x42, 0xCD, 0xCC, 0xBC, 0x40, 0x5C, 0x8F, 0xFE, 0x41, 0xCD, 0xCC, 0xCC, 0xBD, 0x71
};
static const uint8_t data_36E7D[16] = {
  0x3D, 0x92, 0x40, 0xC3, 0xF5, 0xF0, 0x40, 0x00, 0x00, 0x80, 0x3F, 0x29, 0x5C, 0x01, 0x42, 0x00
};
static const uint8_t data_36E8D[16] = {
  0x00, 0x80, 0x3F, 0x8F, 0xC2, 0x75, 0x3F, 0xCD, 0xCC, 0xD0, 0x41, 0x8F, 0xC2, 0x75, 0x3F, 0xAE
};
static const uint8_t data_36E9D[16] = {
  0x47, 0xD5, 0x41, 0x52, 0xB8, 0x7E, 0xC0, 0x29, 0x5C, 0xD5, 0x41, 0x52, 0xB8, 0x7E, 0xC0, 0xCD
};
static const uint8_t data_36EAD[16] = {
  0xCC, 0xD0, 0x41, 0xCD, 0xCC, 0xCC, 0xBD, 0xCD, 0xCC, 0x6C, 0x40, 0x5C, 0x8F, 0xC2, 0xBF, 0x00
};
static const uint8_t data_36EBD[16] = {
  0x00, 0x80, 0x3F, 0x3D, 0x0A, 0xD3, 0x41, 0x00, 0x00, 0x80, 0x3F, 0x0A, 0xD7, 0xC3, 0xC0, 0x7B
};
static const uint8_t data_36ECD[16] = {
  0x14, 0xFE, 0x41, 0x14, 0xAE, 0x47, 0xC0, 0x7B, 0x14, 0xFE, 0x41, 0x14, 0xAE, 0x47, 0xC0, 0xB8
};
static const uint8_t data_36EDD[16] = {
  0x1E, 0x05, 0x42, 0x0A, 0xD7, 0xC3, 0xC0, 0xB8, 0x1E, 0x05, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0x48
};
static const uint8_t data_36EED[16] = {
  0xE1, 0x6A, 0x40, 0x0A, 0xD7, 0x93, 0xC0, 0x00, 0x00, 0x80, 0x3F, 0x3D, 0x0A, 0x02, 0x42, 0x00
};
static const uint8_t data_36EFD[16] = {
  0x00, 0x80, 0x3F, 0x71, 0x3D, 0xBE, 0xC1, 0x29, 0x5C, 0x0F, 0x42, 0x48, 0xE1, 0xB8, 0xC1, 0xD7
};
static const uint8_t data_36F0D[16] = {
  0xA3, 0x12, 0x42, 0x8F, 0xC2, 0xC9, 0xC1, 0xE1, 0x7A, 0x19, 0x42, 0xB8, 0x1E, 0xCF, 0xC1, 0x71
};
static const uint8_t data_36F1D[16] = {
  0x3D, 0x16, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0x85, 0xEB, 0x51, 0x40, 0x00, 0x00, 0xC4, 0xC1, 0x00
};
static const uint8_t data_36F2D[16] = {
  0x00, 0x80, 0x3F, 0xA4, 0x70, 0x14, 0x42, 0x00, 0x00, 0x80, 0x3F, 0x14, 0xAE, 0x1D, 0xC2, 0xB8
};
static const uint8_t data_36F3D[16] = {
  0x1E, 0x16, 0x42, 0xD7, 0xA3, 0x1D, 0xC2, 0xB8, 0x1E, 0x1C, 0x42, 0xAE, 0x47, 0x2D, 0xC2, 0xB8
};
static const uint8_t data_36F4D[16] = {
  0x1E, 0x1C, 0x42, 0x71, 0x3D, 0x2D, 0xC2, 0xB8, 0x1E, 0x16, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0xAE
};
static const uint8_t data_36F5D[16] = {
  0x47, 0x71, 0x40, 0xA4, 0x70, 0x25, 0xC2, 0x00, 0x00, 0x80, 0x3F, 0xB8, 0x1E, 0x19, 0x42, 0x00
};
static const uint8_t data_36F6D[16] = {
  0x00, 0x80, 0x3F, 0xCD, 0xCC, 0x33, 0xC2, 0x33, 0x33, 0x17, 0x42, 0xCD, 0xCC, 0x33, 0xC2, 0x5C
};
static const uint8_t data_36F7D[16] = {
  0x8F, 0x1C, 0x42, 0x0A, 0xD7, 0x3F, 0xC2, 0x1F, 0x85, 0x1C, 0x42, 0x66, 0x66, 0x3E, 0xC2, 0xB8
};
static const uint8_t data_36F8D[16] = {
  0x1E, 0x17, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0x14, 0xAE, 0x57, 0x40, 0x0A, 0xD7, 0x39, 0xC2, 0x00
};
static const uint8_t data_36F9D[16] = {
  0x00, 0x80, 0x3F, 0x0A, 0xD7, 0x19, 0x42, 0x00, 0x00, 0x80, 0x3F, 0x1F, 0x85, 0x3B, 0xC2, 0x29
};
static const uint8_t data_36FAD[16] = {
  0x5C, 0x33, 0x42, 0x00, 0x00, 0x38, 0xC2, 0x66, 0x66, 0x38, 0x42, 0x48, 0xE1, 0x42, 0xC2, 0x7B
};
static const uint8_t data_36FBD[16] = {
  0x14, 0x41, 0x42, 0xE1, 0x7A, 0x46, 0xC2, 0x33, 0x33, 0x3B, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0x71
};
static const uint8_t data_36FCD[16] = {
  0x3D, 0x92, 0x40, 0x8F, 0xC2, 0x3F, 0xC2, 0x00, 0x00, 0x80, 0x3F, 0x29, 0x5C, 0x39, 0x42, 0x00
};
static const uint8_t data_36FDD[16] = {
  0x00, 0x80, 0x3F, 0x85, 0xEB, 0x68, 0xC2, 0xEC, 0x51, 0x38, 0x42, 0x85, 0xEB, 0x6F, 0xC2, 0x52
};
static const uint8_t data_36FED[16] = {
  0xB8, 0x2E, 0x42, 0xC3, 0xF5, 0x62, 0xC2, 0x1F, 0x85, 0x25, 0x42, 0x33, 0x33, 0x5C, 0xC2, 0x29
};
static const uint8_t data_36FFD[16] = {
  0x5C, 0x2F, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0x14, 0xAE, 0x77, 0x40, 0x00, 0x00, 0x66, 0xC2, 0x52
};
static const uint8_t data_3700D[16] = {
  0xB8, 0xBE, 0x3F, 0xC3, 0xF5, 0x2E, 0x42, 0x52, 0xB8, 0xBE, 0x3F, 0xD7, 0xA3, 0x77, 0xC2, 0x33
};
static const uint8_t data_3701D[16] = {
  0x33, 0x3C, 0x42, 0x33, 0x33, 0x75, 0xC2, 0xD7, 0xA3, 0x3F, 0x42, 0xF6, 0x28, 0x7E, 0xC2, 0xC3
};
static const uint8_t data_3702D[16] = {
  0xF5, 0x45, 0x42, 0xAE, 0x47, 0x80, 0xC2, 0x1F, 0x85, 0x42, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0x33
};
static const uint8_t data_3703D[16] = {
  0x33, 0x53, 0x40, 0x48, 0xE1, 0x7A, 0xC2, 0x00, 0x00, 0x80, 0x3F, 0x7B, 0x14, 0x41, 0x42, 0x00
};
static const uint8_t data_3704D[16] = {
  0x00, 0x80, 0x3F, 0x00, 0x00, 0x5E, 0xC2, 0x52, 0xB8, 0x4D, 0x42, 0x29, 0x5C, 0x5A, 0xC2, 0x48
};
static const uint8_t data_3705D[16] = {
  0xE1, 0x52, 0x42, 0xCD, 0xCC, 0x6A, 0xC2, 0x33, 0x33, 0x5F, 0x42, 0xD7, 0xA3, 0x6E, 0xC2, 0x52
};
static const uint8_t data_3706D[16] = {
  0xB8, 0x59, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0xCD, 0xCC, 0x6C, 0x40, 0x5C, 0x8F, 0x65, 0xC2, 0x00
};
static const uint8_t data_3707D[16] = {
  0x00, 0x80, 0x3F, 0x0A, 0xD7, 0x54, 0x42, 0x00, 0x00, 0x80, 0x3F, 0x8F, 0xC2, 0x7B, 0xC2, 0xB8
};
static const uint8_t data_3708D[16] = {
  0x1E, 0x66, 0x42, 0x8F, 0xC2, 0x77, 0xC2, 0x5C, 0x8F, 0x67, 0x42, 0xB8, 0x1E, 0x7C, 0xC2, 0xC3
};
static const uint8_t data_3709D[16] = {
  0xF5, 0x73, 0x42, 0x5C, 0x0F, 0x80, 0xC2, 0xA4, 0x70, 0x72, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0x71
};
static const uint8_t data_370AD[16] = {
  0x3D, 0x92, 0x40, 0x85, 0xEB, 0x7B, 0xC2, 0x00, 0x00, 0x80, 0x3F, 0x00, 0x00, 0x6D, 0x42, 0x00
};
static const uint8_t data_370BD[16] = {
  0x00, 0x80, 0x3F, 0xA4, 0xF0, 0x93, 0xC2, 0xF6, 0x28, 0x6D, 0x42, 0xA4, 0xF0, 0x93, 0xC2, 0x9A
};
static const uint8_t data_370CD[16] = {
  0x99, 0x72, 0x42, 0xA4, 0xF0, 0x99, 0xC2, 0x9A, 0x99, 0x72, 0x42, 0xA4, 0xF0, 0x99, 0xC2, 0xF6
};
static const uint8_t data_370DD[16] = {
  0x28, 0x6D, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0x14, 0xAE, 0x57, 0x40, 0xA4, 0xF0, 0x96, 0xC2, 0x00
};
static const uint8_t data_370ED[16] = {
  0x00, 0x80, 0x3F, 0x48, 0xE1, 0x6F, 0x42, 0x00, 0x00, 0x80, 0x3F, 0x8F, 0xC2, 0xA0, 0xC2, 0x5C
};
static const uint8_t data_370FD[16] = {
  0x8F, 0x6D, 0x42, 0xAE, 0xC7, 0xA0, 0xC2, 0x52, 0xB8, 0x71, 0x42, 0x33, 0x33, 0xA6, 0xC2, 0x52
};
static const uint8_t data_3710D[16] = {
  0xB8, 0x71, 0x42, 0x33, 0x33, 0xA6, 0xC2, 0x1F, 0x85, 0x6D, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0x85
};
static const uint8_t data_3711D[16] = {
  0xEB, 0x51, 0x40, 0xE1, 0x7A, 0xA3, 0xC2, 0x00, 0x00, 0x80, 0x3F, 0xD7, 0xA3, 0x6F, 0x42, 0x00
};
static const uint8_t data_3712D[16] = {
  0x00, 0x80, 0x3F, 0x33, 0x33, 0xCC, 0xC2, 0x52, 0x38, 0x82, 0x42, 0xEC, 0x51, 0xCB, 0xC2, 0xF6
};
static const uint8_t data_3713D[16] = {
  0x28, 0x84, 0x42, 0x0A, 0xD7, 0xD1, 0xC2, 0x33, 0x33, 0x87, 0x42, 0x71, 0xBD, 0xD2, 0xC2, 0x8F
};
static const uint8_t data_3714D[16] = {
  0x42, 0x85, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0xD7, 0xA3, 0x30, 0x40, 0x1F, 0x05, 0xCF, 0xC2, 0x00
};
static const uint8_t data_3715D[16] = {
  0x00, 0x80, 0x3F, 0x33, 0xB3, 0x84, 0x42, 0x00, 0x00, 0x80, 0x3F, 0x9A, 0x19, 0xC4, 0xC2, 0x29
};
static const uint8_t data_3716D[16] = {
  0xDC, 0x87, 0x42, 0x0A, 0xD7, 0xBA, 0xC2, 0x3D, 0x8A, 0x83, 0x42, 0x85, 0xEB, 0xB7, 0xC2, 0x85
};
static const uint8_t data_3717D[16] = {
  0xEB, 0x8B, 0x42, 0x14, 0x2E, 0xC1, 0xC2, 0x8F, 0x42, 0x8E, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0x71
};
static const uint8_t data_3718D[16] = {
  0x3D, 0x82, 0x40, 0x1F, 0x05, 0xBE, 0xC2, 0x5C, 0x8F, 0xE2, 0x3F, 0x48, 0xE1, 0x88, 0x42, 0x5C
};
static const uint8_t data_3719D[16] = {
  0x8F, 0xE2, 0x3F, 0x3D, 0x8A, 0xD4, 0xC2, 0xF6, 0x28, 0x89, 0x42, 0x48, 0xE1, 0xDE, 0xC2, 0x00
};
static const uint8_t data_371AD[16] = {
  0x00, 0x8E, 0x42, 0xE1, 0x7A, 0xE2, 0xC2, 0x0A, 0x57, 0x86, 0x42, 0x1F, 0x05, 0xD8, 0xC2, 0x5C
};
static const uint8_t data_371BD[16] = {
  0x8F, 0x81, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0x48, 0xE1, 0x6A, 0x40, 0xE1, 0x7A, 0xDB, 0xC2, 0x66
};
static const uint8_t data_371CD[16] = {
  0x66, 0x06, 0x40, 0x8F, 0xC2, 0x87, 0x42, 0x66, 0x66, 0x06, 0x40, 0xCD, 0x4C, 0xF2, 0xC2, 0xA4
};
static const uint8_t data_371DD[16] = {
  0x70, 0xA4, 0x42, 0xB8, 0x1E, 0xF7, 0xC2, 0xD7, 0xA3, 0xAA, 0x42, 0xD7, 0xA3, 0xFD, 0xC2, 0x71
};
static const uint8_t data_371ED[16] = {
  0x3D, 0xA5, 0x42, 0x71, 0xBD, 0xF8, 0xC2, 0x0A, 0x57, 0x9F, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0x48
};
static const uint8_t data_371FD[16] = {
  0xE1, 0x6A, 0x40, 0xC3, 0xF5, 0xF7, 0xC2, 0xB8, 0x1E, 0x05, 0x40, 0x85, 0xEB, 0xA4, 0x42, 0xB8
};
static const uint8_t data_3720D[16] = {
  0x1E, 0x05, 0x40, 0xC3, 0xF5, 0xCF, 0xC2, 0x48, 0xE1, 0xB8, 0x42, 0x14, 0x2E, 0xD4, 0xC2, 0x33
};
static const uint8_t data_3721D[16] = {
  0x33, 0xBE, 0x42, 0xD7, 0x23, 0xDA, 0xC2, 0xE1, 0x7A, 0xB9, 0x42, 0x48, 0xE1, 0xD5, 0xC2, 0x9A
};
static const uint8_t data_3722D[16] = {
  0x19, 0xB4, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0x9A, 0x99, 0x79, 0x40, 0x3D, 0x0A, 0xD5, 0xC2, 0x85
};
static const uint8_t data_3723D[16] = {
  0xEB, 0xF1, 0x3F, 0xF6, 0x28, 0xB9, 0x42, 0x85, 0xEB, 0xF1, 0x3F, 0xD7, 0xA3, 0xC7, 0xC2, 0x3D
};
static const uint8_t data_3724D[16] = {
  0x0A, 0xAF, 0x42, 0x9A, 0x99, 0xCC, 0xC2, 0xEC, 0xD1, 0xB5, 0x42, 0x8F, 0x42, 0xD1, 0xC2, 0xCD
};
static const uint8_t data_3725D[16] = {
  0x4C, 0xB2, 0x42, 0x52, 0x38, 0xCC, 0xC2, 0xCD, 0xCC, 0xAB, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0x48
};
static const uint8_t data_3726D[16] = {
  0xE1, 0x6A, 0x40, 0x1F, 0x05, 0xCC, 0xC2, 0xB8, 0x1E, 0x85, 0x3F, 0x00, 0x00, 0xB1, 0x42, 0xB8
};
static const uint8_t data_3727D[16] = {
  0x1E, 0x85, 0x3F, 0x1F, 0x85, 0xB4, 0xC2, 0x5C, 0x8F, 0xA2, 0x42, 0xB8, 0x9E, 0xB3, 0xC2, 0xE1
};
static const uint8_t data_3728D[16] = {
  0x7A, 0xA4, 0x42, 0x5C, 0x8F, 0xB8, 0xC2, 0xAE, 0xC7, 0xA6, 0x42, 0xC3, 0x75, 0xB9, 0xC2, 0x29
};
static const uint8_t data_3729D[16] = {
  0xDC, 0xA4, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0x85, 0xEB, 0x51, 0x40, 0x3D, 0x8A, 0xB6, 0xC2, 0x00
};
static const uint8_t data_372AD[16] = {
  0x00, 0x00, 0x40, 0xF6, 0xA8, 0xA4, 0x42, 0x00, 0x00, 0x00, 0x40, 0xE1, 0xFA, 0xA2, 0xC2, 0x00
};
static const uint8_t data_372BD[16] = {
  0x00, 0xA7, 0x42, 0xF6, 0x28, 0x9C, 0xC2, 0x00, 0x00, 0xA7, 0x42, 0xF6, 0x28, 0x9C, 0xC2, 0x3D
};
static const uint8_t data_372CD[16] = {
  0x8A, 0xA9, 0x42, 0xE1, 0xFA, 0xA2, 0xC2, 0x3D, 0x8A, 0xA9, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0x71
};
static const uint8_t data_372DD[16] = {
  0x3D, 0x92, 0x40, 0x5C, 0x8F, 0x9F, 0xC2, 0x00, 0x00, 0xC0, 0x3F, 0xAE, 0x47, 0xA8, 0x42, 0x00
};
static const uint8_t data_372ED[16] = {
  0x00, 0xC0, 0x3F, 0xEC, 0x51, 0x90, 0xC2, 0x71, 0x3D, 0xA7, 0x42, 0xCD, 0x4C, 0x90, 0xC2, 0x0A
};
static const uint8_t data_372FD[16] = {
  0x57, 0xA9, 0x42, 0x8F, 0xC2, 0x95, 0xC2, 0x0A, 0x57, 0xA9, 0x42, 0x8F, 0xC2, 0x95, 0xC2, 0x71
};
static const uint8_t data_3730D[16] = {
  0x3D, 0xA7, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0x33, 0x33, 0x53, 0x40, 0x3D, 0x0A, 0x93, 0xC2, 0x00
};
static const uint8_t data_3731D[16] = {
  0x00, 0x80, 0x3F, 0xCD, 0x4C, 0xA8, 0x42, 0x00, 0x00, 0x80, 0x3F, 0x52, 0xB8, 0x22, 0xC2, 0x52
};
static const uint8_t data_3732D[16] = {
  0x38, 0xA1, 0x42, 0xD7, 0xA3, 0x15, 0xC2, 0x1F, 0x05, 0x9F, 0x42, 0xE1, 0x7A, 0x11, 0xC2, 0x29
};
static const uint8_t data_3733D[16] = {
  0x5C, 0xA5, 0x42, 0x48, 0xE1, 0x1E, 0xC2, 0x5C, 0x8F, 0xA7, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0x71
};
static const uint8_t data_3734D[16] = {
  0x3D, 0x92, 0x40, 0xE1, 0x7A, 0x1B, 0xC2, 0x00, 0x00, 0x80, 0x3F, 0x66, 0x66, 0xA1, 0x42, 0x00
};
static const uint8_t data_3735D[16] = {
  0x00, 0x80, 0x3F, 0x52, 0xB8, 0x1E, 0xC0, 0x0A, 0x57, 0xA9, 0x42, 0xB8, 0x1E, 0x55, 0xC0, 0x00
};
static const uint8_t data_3736D[16] = {
  0x80, 0xAB, 0x42, 0x8F, 0xC2, 0xB5, 0xC0, 0x71, 0xBD, 0xA7, 0x42, 0x5C, 0x8F, 0x9A, 0xC0, 0xB8
};
static const uint8_t data_3737D[16] = {
  0x9E, 0xA5, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0x14, 0xAE, 0x57, 0x40, 0x85, 0xEB, 0x81, 0xC0, 0x00
};
static const uint8_t data_3738D[16] = {
  0x00, 0x80, 0x3F, 0x00, 0x80, 0xA8, 0x42, 0x00, 0x00, 0x80, 0x3F, 0x48, 0xE1, 0xEC, 0x41, 0x7B
};
static const uint8_t data_3739D[16] = {
  0x14, 0x0E, 0x41, 0x3D, 0x0A, 0xF7, 0x41, 0xE1, 0x7A, 0xCC, 0x40, 0x5C, 0x8F, 0xFE, 0x41, 0xF6
};
static const uint8_t data_373AD[16] = {
  0x28, 0xDC, 0x40, 0x66, 0x66, 0xF4, 0x41, 0x85, 0xEB, 0x15, 0x41, 0xCD, 0xCC, 0xCC, 0xBD, 0x85
};
static const uint8_t data_373BD[16] = {
  0xEB, 0x51, 0x40, 0x14, 0xAE, 0xF5, 0x41, 0x00, 0x00, 0x80, 0x3F, 0xF6, 0x28, 0xFC, 0x40, 0x00
};
static const uint8_t data_373CD[16] = {
  0x00, 0x80, 0x3F, 0x71, 0x3D, 0x5E, 0xC1, 0xAE, 0x47, 0x0E, 0x42, 0xC3, 0xF5, 0x38, 0xC1, 0x52
};
static const uint8_t data_373DD[16] = {
  0xB8, 0x06, 0x42, 0x3D, 0x0A, 0x2B, 0xC1, 0xB8, 0x1E, 0x0B, 0x42, 0xEC, 0x51, 0x50, 0xC1, 0x14
};
static const uint8_t data_373ED[16] = {
  0xAE, 0x12, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0x14, 0xAE, 0x57, 0x40, 0xD7, 0xA3, 0x44, 0xC1, 0x00
};
static const uint8_t data_373FD[16] = {
  0x00, 0x80, 0x3F, 0x52, 0xB8, 0x0C, 0x42, 0x00, 0x00, 0x80, 0x3F, 0x3D, 0x0A, 0xA3, 0xC2, 0xA4
};
static const uint8_t data_3740D[16] = {
  0xF0, 0x82, 0x42, 0x3D, 0x0A, 0xA3, 0xC2, 0x5C, 0x8F, 0x7F, 0x42, 0x00, 0x00, 0x9D, 0xC2, 0x5C
};
static const uint8_t data_3741D[16] = {
  0x8F, 0x7F, 0x42, 0x00, 0x00, 0x9D, 0xC2, 0xA4, 0xF0, 0x82, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0x48
};
static const uint8_t data_3742D[16] = {
  0xE1, 0x6A, 0x40, 0x1F, 0x05, 0xA0, 0xC2, 0x00, 0x00, 0x80, 0x3F, 0x29, 0x5C, 0x81, 0x42, 0x00
};
static const uint8_t data_3743D[16] = {
  0x00, 0x80, 0x3F, 0xA4, 0xF0, 0xAF, 0xC2, 0xCD, 0xCC, 0x75, 0x42, 0x1F, 0x85, 0xB0, 0xC2, 0xEC
};
static const uint8_t data_3744D[16] = {
  0x51, 0x71, 0x42, 0x33, 0x33, 0xAA, 0xC2, 0x00, 0x00, 0x6E, 0x42, 0xF6, 0xA8, 0xA9, 0xC2, 0xEC
};
static const uint8_t data_3745D[16] = {
  0x51, 0x72, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0x00, 0x00, 0x90, 0x40, 0x7B, 0x14, 0xAD, 0xC2, 0x00
};
static const uint8_t data_3746D[16] = {
  0x00, 0x80, 0x3F, 0x0A, 0xD7, 0x71, 0x42, 0x00, 0x00, 0x80, 0x3F, 0x48, 0xE1, 0xA9, 0xC2, 0x8F
};
static const uint8_t data_3747D[16] = {
  0x42, 0x80, 0x42, 0x48, 0xE1, 0xA8, 0xC2, 0xCD, 0xCC, 0x84, 0x42, 0x9A, 0x99, 0xAF, 0xC2, 0x66
};
static const uint8_t data_3748D[16] = {
  0x66, 0x86, 0x42, 0xF6, 0xA8, 0xB0, 0xC2, 0x48, 0xE1, 0x81, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0x71
};
static const uint8_t data_3749D[16] = {
  0x3D, 0x92, 0x40, 0x1F, 0x05, 0xAD, 0xC2, 0x00, 0x00, 0x80, 0x3F, 0xF6, 0x28, 0x82, 0x42, 0x00
};
static const uint8_t data_374AD[16] = {
  0x00, 0x80, 0x3F, 0x33, 0x33, 0xD7, 0xC2, 0xAE, 0x47, 0x91, 0x42, 0x8F, 0x42, 0xD1, 0xC2, 0x3D
};
static const uint8_t data_374BD[16] = {
  0x8A, 0x8E, 0x42, 0x7B, 0x14, 0xD0, 0xC2, 0x1F, 0x05, 0x91, 0x42, 0xC3, 0xF5, 0xD5, 0xC2, 0xAE
};
static const uint8_t data_374CD[16] = {
  0xC7, 0x93, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0x48, 0xE1, 0x92, 0x40, 0xB8, 0x9E, 0xD3, 0xC2, 0x00
};
static const uint8_t data_374DD[16] = {
  0x00, 0x80, 0x3F, 0xF6, 0x28, 0x91, 0x42, 0x00, 0x00, 0x80, 0x3F, 0x0A, 0x57, 0xFE, 0xC2, 0x1F
};
static const uint8_t data_374ED[16] = {
  0x05, 0x80, 0x42, 0xA4, 0xF0, 0xF8, 0xC2, 0x0A, 0x57, 0x84, 0x42, 0x0A, 0xD7, 0xFC, 0xC2, 0x14
};
static const uint8_t data_374FD[16] = {
  0x2E, 0x89, 0x42, 0x0A, 0x17, 0x01, 0xC3, 0x85, 0xEB, 0x84, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0x5C
};
static const uint8_t data_3750D[16] = {
  0x8F, 0x12, 0x40, 0x7B, 0x94, 0xFD, 0xC2, 0x66, 0x66, 0xC6, 0x3F, 0xB8, 0x9E, 0x84, 0x42, 0x66
};
static const uint8_t data_3751D[16] = {
  0x66, 0xC6, 0x3F, 0x66, 0x66, 0xE8, 0xC2, 0xAE, 0xC7, 0xA3, 0x42, 0x9A, 0x99, 0xE5, 0xC2, 0x8F
};
static const uint8_t data_3752D[16] = {
  0xC2, 0xA5, 0x42, 0x71, 0x3D, 0xE9, 0xC2, 0x1F, 0x85, 0xAA, 0x42, 0x7B, 0x14, 0xEC, 0xC2, 0x00
};
static const uint8_t data_3753D[16] = {
  0x80, 0xA8, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0xD7, 0xA3, 0x50, 0x40, 0x52, 0x38, 0xE9, 0xC2, 0x00
};
static const uint8_t data_3754D[16] = {
  0x00, 0x80, 0x3F, 0xF6, 0xA8, 0xA6, 0x42, 0x00, 0x00, 0x80, 0x3F, 0xC3, 0x75, 0xF8, 0xC2, 0xB8
};
static const uint8_t data_3755D[16] = {
  0x1E, 0xAE, 0x42, 0x5C, 0x0F, 0xF7, 0xC2, 0x8F, 0xC2, 0xB4, 0x42, 0x33, 0x33, 0xFB, 0xC2, 0xB8
};
static const uint8_t data_3756D[16] = {
  0x9E, 0xB5, 0x42, 0x1F, 0x85, 0xFC, 0xC2, 0xC3, 0xF5, 0xAE, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0xAE
};
static const uint8_t data_3757D[16] = {
  0x47, 0x51, 0x40, 0xEC, 0xD1, 0xF9, 0xC2, 0xAE, 0x47, 0x81, 0x3F, 0x29, 0xDC, 0xB1, 0x42, 0xAE
};
static const uint8_t data_3758D[16] = {
  0x47, 0x81, 0x3F, 0x29, 0x5C, 0xE2, 0xC2, 0x8F, 0xC2, 0xC2, 0x42, 0x0A, 0xD7, 0xDC, 0xC2, 0xF6
};
static const uint8_t data_3759D[16] = {
  0xA8, 0xC4, 0x42, 0xCD, 0x4C, 0xDD, 0xC2, 0x85, 0xEB, 0xC5, 0x42, 0xCD, 0xCC, 0xE2, 0xC2, 0x1F
};
static const uint8_t data_375AD[16] = {
  0x05, 0xC4, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0x8F, 0xC2, 0x45, 0x40, 0xEC, 0xD1, 0xDF, 0xC2, 0x00
};
static const uint8_t data_375BD[16] = {
  0x00, 0x80, 0x3F, 0x0A, 0x57, 0xC4, 0x42, 0x00, 0x00, 0x80, 0x3F, 0x0A, 0x57, 0xBC, 0xC2, 0x52
};
static const uint8_t data_375CD[16] = {
  0x38, 0xA8, 0x42, 0x5C, 0x0F, 0xC1, 0xC2, 0x85, 0x6B, 0xAA, 0x42, 0x71, 0x3D, 0xC3, 0xC2, 0x29
};
static const uint8_t data_375DD[16] = {
  0xDC, 0xA5, 0x42, 0xA4, 0x70, 0xBE, 0xC2, 0xD7, 0xA3, 0xA3, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0xA4
};
static const uint8_t data_375ED[16] = {
  0x70, 0x4D, 0x40, 0x8F, 0xC2, 0xBF, 0xC2, 0xAE, 0x47, 0xA1, 0x3F, 0x1F, 0x05, 0xA7, 0x42, 0xAE
};
static const uint8_t data_375FD[16] = {
  0x47, 0xA1, 0x3F, 0x0A, 0xD7, 0xAD, 0xC2, 0xD7, 0xA3, 0x9F, 0x42, 0x1F, 0x05, 0xA8, 0xC2, 0x71
};
static const uint8_t data_3760D[16] = {
  0x3D, 0x9E, 0x42, 0x29, 0x5C, 0xA7, 0xC2, 0x29, 0xDC, 0xA0, 0x42, 0x71, 0x3D, 0xAD, 0xC2, 0x8F
};
static const uint8_t data_3761D[16] = {
  0x42, 0xA2, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0x14, 0xAE, 0x57, 0x40, 0xB8, 0x9E, 0xAA, 0xC2, 0x00
};
static const uint8_t data_3762D[16] = {
  0x00, 0x80, 0x3F, 0x71, 0x3D, 0xA0, 0x42, 0x00, 0x00, 0x80, 0x3F, 0x48, 0x61, 0x91, 0xC2, 0x5C
};
static const uint8_t data_3763D[16] = {
  0x0F, 0xA0, 0x42, 0x48, 0x61, 0x91, 0xC2, 0xC3, 0xF5, 0x97, 0x42, 0xCD, 0x4C, 0x8D, 0xC2, 0xC3
};
static const uint8_t data_3764D[16] = {
  0xF5, 0x97, 0x42, 0xCD, 0x4C, 0x8D, 0xC2, 0x5C, 0x0F, 0xA0, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0x48
};
static const uint8_t data_3765D[16] = {
  0xE1, 0x8A, 0x40, 0x0A, 0x57, 0x8F, 0xC2, 0xC3, 0xF5, 0xA8, 0x3F, 0x66, 0x66, 0x9D, 0x42, 0xC3
};
static const uint8_t data_3766D[16] = {
  0xF5, 0xA8, 0x3F, 0xCD, 0xCC, 0x80, 0xC2, 0x3D, 0x0A, 0x8B, 0x42, 0xCD, 0xCC, 0x80, 0xC2, 0x00
};
static const uint8_t data_3767D[16] = {
  0x00, 0x80, 0x42, 0x7B, 0x14, 0x72, 0xC2, 0x00, 0x00, 0x80, 0x42, 0x7B, 0x14, 0x72, 0xC2, 0x1F
};
static const uint8_t data_3768D[16] = {
  0x05, 0x8B, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0x33, 0x33, 0x73, 0x40, 0x0A, 0xD7, 0x79, 0xC2, 0x3D
};
static const uint8_t data_3769D[16] = {
  0x0A, 0xF7, 0x3F, 0x1F, 0x85, 0x85, 0x42, 0x3D, 0x0A, 0xF7, 0x3F, 0xC3, 0xF5, 0x80, 0xC2, 0x85
};
static const uint8_t data_376AD[16] = {
  0xEB, 0x95, 0x42, 0x33, 0x33, 0x7C, 0xC2, 0x85, 0xEB, 0x95, 0x42, 0x33, 0x33, 0x7C, 0xC2, 0x85
};
static const uint8_t data_376BD[16] = {
  0xEB, 0x9B, 0x42, 0xC3, 0xF5, 0x80, 0xC2, 0x85, 0xEB, 0x9B, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0x14
};
static const uint8_t data_376CD[16] = {
  0xAE, 0x57, 0x40, 0x7B, 0x14, 0x7F, 0xC2, 0x00, 0x00, 0x80, 0x3F, 0x85, 0xEB, 0x98, 0x42, 0x00
};
static const uint8_t data_376DD[16] = {
  0x00, 0x80, 0x3F, 0x33, 0x33, 0x36, 0xC2, 0xF6, 0xA8, 0x99, 0x42, 0xE1, 0x7A, 0x3B, 0xC2, 0x7B
};
static const uint8_t data_376ED[16] = {
  0x14, 0x9B, 0x42, 0x3D, 0x0A, 0x3E, 0xC2, 0xF6, 0xA8, 0x98, 0x42, 0x9A, 0x99, 0x38, 0xC2, 0x33
};
static const uint8_t data_376FD[16] = {
  0x33, 0x97, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0xA4, 0x70, 0x3D, 0x40, 0x7B, 0x14, 0x3A, 0xC2, 0x00
};
static const uint8_t data_3770D[16] = {
  0x00, 0x80, 0x3F, 0xD7, 0x23, 0x99, 0x42, 0x00, 0x00, 0x80, 0x3F, 0xC3, 0xF5, 0x19, 0xC2, 0x85
};
static const uint8_t data_3771D[16] = {
  0xEB, 0x92, 0x42, 0x00, 0x00, 0x1B, 0xC2, 0xF6, 0x28, 0x91, 0x42, 0xA4, 0x70, 0x11, 0xC2, 0x52
};
static const uint8_t data_3772D[16] = {
  0xB8, 0x8F, 0x42, 0x29, 0x5C, 0x10, 0xC2, 0x3D, 0x8A, 0x91, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0x71
};
static const uint8_t data_3773D[16] = {
  0x3D, 0x5A, 0x40, 0x52, 0xB8, 0x15, 0xC2, 0x00, 0x00, 0x80, 0x3F, 0xEC, 0x51, 0x91, 0x42, 0x00
};
static const uint8_t data_3774D[16] = {
  0x00, 0x80, 0x3F, 0x71, 0x3D, 0xA0, 0xC1, 0xD7, 0x23, 0x91, 0x42, 0x71, 0x3D, 0xA0, 0xC1, 0x85
};
static const uint8_t data_3775D[16] = {
  0x6B, 0x8C, 0x42, 0xAE, 0x47, 0x8D, 0xC1, 0x85, 0x6B, 0x8C, 0x42, 0xAE, 0x47, 0x8D, 0xC1, 0xD7
};
static const uint8_t data_3776D[16] = {
  0x23, 0x91, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0xE1, 0x7A, 0x64, 0x40, 0x52, 0xB8, 0x96, 0xC1, 0x3D
};
static const uint8_t data_3777D[16] = {
  0x0A, 0x97, 0x3F, 0xAE, 0xC7, 0x8E, 0x42, 0x3D, 0x0A, 0x97, 0x3F, 0x0A, 0xD7, 0x85, 0x41, 0x71
};
static const uint8_t data_3778D[16] = {
  0xBD, 0xBA, 0x42, 0xCD, 0xCC, 0x74, 0x41, 0x3D, 0x8A, 0xB8, 0x42, 0x0A, 0xD7, 0x7F, 0x41, 0x5C
};
static const uint8_t data_3779D[16] = {
  0x8F, 0xB6, 0x42, 0xA4, 0x70, 0x8B, 0x41, 0x0A, 0xD7, 0xB8, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0x48
};
static const uint8_t data_377AD[16] = {
  0xE1, 0x3A, 0x40, 0xC3, 0xF5, 0x82, 0x41, 0x00, 0x00, 0x80, 0x3F, 0x14, 0xAE, 0xB8, 0x42, 0x00
};
static const uint8_t data_377BD[16] = {
  0x00, 0x80, 0x3F, 0x9A, 0x99, 0x99, 0x41, 0x3D, 0x0A, 0x23, 0x42, 0x7B, 0x14, 0xA6, 0x41, 0x3D
};
static const uint8_t data_377CD[16] = {
  0x0A, 0x23, 0x42, 0x7B, 0x14, 0xA6, 0x41, 0xD7, 0xA3, 0x32, 0x42, 0x9A, 0x99, 0x99, 0x41, 0xD7
};
static const uint8_t data_377DD[16] = {
  0xA3, 0x32, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0xAE, 0x47, 0x71, 0x40, 0x0A, 0xD7, 0x9F, 0x41, 0x00
};
static const uint8_t data_377ED[16] = {
  0x00, 0x80, 0x3F, 0x0A, 0xD7, 0x2A, 0x42, 0x00, 0x00, 0x80, 0x3F, 0x9A, 0x99, 0x99, 0x41, 0x9A
};
static const uint8_t data_377FD[16] = {
  0x99, 0x3B, 0x42, 0x8F, 0xC2, 0xB5, 0x41, 0x9A, 0x99, 0x3B, 0x42, 0x8F, 0xC2, 0xB5, 0x41, 0x0A
};
static const uint8_t data_3780D[16] = {
  0xD7, 0x50, 0x42, 0x9A, 0x99, 0x99, 0x41, 0x0A, 0xD7, 0x50, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0x33
};
static const uint8_t data_3781D[16] = {
  0x33, 0x73, 0x40, 0x14, 0xAE, 0xA7, 0x41, 0x52, 0xB8, 0xDE, 0x3F, 0x33, 0x33, 0x46, 0x42, 0x52
};
static const uint8_t data_3782D[16] = {
  0xB8, 0xDE, 0x3F, 0xD7, 0xA3, 0x80, 0x41, 0xD7, 0xA3, 0x62, 0x42, 0x71, 0x3D, 0x66, 0x41, 0xD7
};
static const uint8_t data_3783D[16] = {
  0xA3, 0x62, 0x42, 0x71, 0x3D, 0x66, 0x41, 0xD7, 0xA3, 0x56, 0x42, 0xD7, 0xA3, 0x80, 0x41, 0xD7
};
static const uint8_t data_3784D[16] = {
  0xA3, 0x56, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0x48, 0xE1, 0x6A, 0x40, 0x0A, 0xD7, 0x73, 0x41, 0x00
};
static const uint8_t data_3785D[16] = {
  0x00, 0x80, 0x3F, 0x14, 0xAE, 0x5C, 0x42, 0x00, 0x00, 0x80, 0x3F, 0x9A, 0x99, 0x99, 0x41, 0xD7
};
static const uint8_t data_3786D[16] = {
  0xA3, 0x5C, 0x42, 0x29, 0x5C, 0xA7, 0x41, 0xD7, 0xA3, 0x5C, 0x42, 0x29, 0x5C, 0xA7, 0x41, 0x9A
};
static const uint8_t data_3787D[16] = {
  0x99, 0x68, 0x42, 0x1F, 0x85, 0x99, 0x41, 0x9A, 0x99, 0x68, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0x48
};
static const uint8_t data_3788D[16] = {
  0xE1, 0x6A, 0x40, 0xE1, 0x7A, 0xA0, 0x41, 0x00, 0x00, 0x80, 0x3F, 0x9A, 0x99, 0x62, 0x42, 0x00
};
static const uint8_t data_3789D[16] = {
  0x00, 0x80, 0x3F, 0x9A, 0x99, 0xAD, 0x41, 0xB8, 0x1E, 0x92, 0x42, 0x85, 0xEB, 0xA7, 0x41, 0xCD
};
static const uint8_t data_378AD[16] = {
  0x4C, 0x93, 0x42, 0x52, 0xB8, 0x9E, 0x41, 0x3D, 0x8A, 0x90, 0x42, 0xEC, 0x51, 0xA4, 0x41, 0x29
};
static const uint8_t data_378BD[16] = {
  0x5C, 0x8F, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0x7B, 0x14, 0xFE, 0x40, 0xF6, 0x28, 0xA6, 0x41, 0x00
};
static const uint8_t data_378CD[16] = {
  0x00, 0x80, 0x3F, 0xEC, 0x51, 0x91, 0x42, 0x00, 0x00, 0x80, 0x3F, 0x00, 0x00, 0xC6, 0x41, 0x7B
};
static const uint8_t data_378DD[16] = {
  0x94, 0x96, 0x42, 0x29, 0x5C, 0xCF, 0x41, 0x0A, 0x57, 0x99, 0x42, 0x29, 0x5C, 0xC9, 0x41, 0x5C
};
static const uint8_t data_378ED[16] = {
  0x8F, 0x9A, 0x42, 0x71, 0x3D, 0xC0, 0x41, 0xEC, 0xD1, 0x97, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0xAE
};
static const uint8_t data_378FD[16] = {
  0x47, 0x01, 0x41, 0x8F, 0xC2, 0xC7, 0x41, 0x00, 0x00, 0x80, 0x3F, 0x7B, 0x94, 0x98, 0x42, 0x00
};
static const uint8_t data_3790D[16] = {
  0x00, 0x80, 0x3F, 0xAE, 0x47, 0xE7, 0x41, 0x71, 0x3D, 0xA0, 0x42, 0x5C, 0x8F, 0xF0, 0x41, 0x00
};
static const uint8_t data_3791D[16] = {
  0x00, 0xA3, 0x42, 0xD7, 0xA3, 0xEA, 0x41, 0x52, 0x38, 0xA4, 0x42, 0xA4, 0x70, 0xE1, 0x41, 0xE1
};
static const uint8_t data_3792D[16] = {
  0x7A, 0xA1, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0xAE, 0x47, 0x01, 0x41, 0x3D, 0x0A, 0xE9, 0x41, 0x00
};
static const uint8_t data_3793D[16] = {
  0x00, 0x80, 0x3F, 0x71, 0x3D, 0xA2, 0x42, 0x00, 0x00, 0x80, 0x3F, 0xC3, 0xF5, 0x1D, 0x42, 0x0A
};
static const uint8_t data_3794D[16] = {
  0x57, 0xA1, 0x42, 0xD7, 0xA3, 0x22, 0x42, 0x9A, 0x19, 0xA4, 0x42, 0xD7, 0xA3, 0x1F, 0x42, 0x0A
};
static const uint8_t data_3795D[16] = {
  0x57, 0xA5, 0x42, 0x7B, 0x14, 0x1B, 0x42, 0x9A, 0x99, 0xA2, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0xAE
};
static const uint8_t data_3796D[16] = {
  0x47, 0x01, 0x41, 0x0A, 0xD7, 0x1E, 0x42, 0x00, 0x00, 0x80, 0x3F, 0x0A, 0x57, 0xA3, 0x42, 0x00
};
static const uint8_t data_3797D[16] = {
  0x00, 0x80, 0x3F, 0x85, 0xEB, 0xEA, 0xC2, 0x1F, 0x05, 0xC7, 0x42, 0xAE, 0xC7, 0xEB, 0xC2, 0xE1
};
static const uint8_t data_3798D[16] = {
  0xFA, 0xC8, 0x42, 0x66, 0xE6, 0xF1, 0xC2, 0x66, 0x66, 0xC6, 0x42, 0x7B, 0x14, 0xF1, 0xC2, 0xEC
};
static const uint8_t data_3799D[16] = {
  0x51, 0xC4, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0x7B, 0x14, 0x7E, 0x40, 0x85, 0x6B, 0xEE, 0xC2, 0x00
};
static const uint8_t data_379AD[16] = {
  0x00, 0xC0, 0x3F, 0x14, 0xAE, 0xC6, 0x42, 0x00, 0x00, 0xC0, 0x3F, 0x14, 0xAE, 0xFB, 0xC1, 0xCD
};
static const uint8_t data_379BD[16] = {
  0xCC, 0xA0, 0x42, 0xA4, 0x70, 0xDB, 0xC1, 0xCD, 0xCC, 0xA0, 0x42, 0xA4, 0x70, 0xDB, 0xC1, 0x1F
};
static const uint8_t data_379CD[16] = {
  0x05, 0xA4, 0x42, 0x14, 0xAE, 0xFB, 0xC1, 0x1F, 0x05, 0xA4, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0x48
};
static const uint8_t data_379DD[16] = {
  0xE1, 0x6A, 0x40, 0x9A, 0x99, 0xEB, 0xC1, 0x00, 0x00, 0x80, 0x3F, 0x66, 0x66, 0xA2, 0x42, 0x00
};
static const uint8_t data_379ED[16] = {
  0x00, 0x80, 0x3F, 0x71, 0x3D, 0x8A, 0x3E, 0x52, 0x38, 0xB6, 0x42, 0x71, 0x3D, 0xAA, 0x3F, 0x00
};
static const uint8_t data_379FD[16] = {
  0x80, 0xB3, 0x42, 0x71, 0x3D, 0x92, 0x40, 0x14, 0xAE, 0xB8, 0x42, 0x00, 0x00, 0x60, 0x40, 0x66
};
static const uint8_t data_37A0D[16] = {
  0x66, 0xBB, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0x48, 0xE1, 0x6A, 0x40, 0x48, 0xE1, 0x1A, 0x40, 0x00
};
static const uint8_t data_37A1D[16] = {
  0x00, 0xC0, 0x3F, 0xC3, 0x75, 0xB7, 0x42, 0x00, 0x00, 0xC0, 0x3F, 0xF6, 0x28, 0x84, 0xC0, 0xE1
};
static const uint8_t data_37A2D[16] = {
  0x7A, 0x99, 0x42, 0x7B, 0x14, 0x3E, 0xC0, 0x9A, 0x19, 0x94, 0x42, 0x52, 0xB8, 0xBE, 0xBF, 0x48
};
static const uint8_t data_37A3D[16] = {
  0x61, 0x95, 0x42, 0xC3, 0xF5, 0x28, 0xC0, 0x8F, 0xC2, 0x9A, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0xAE
};
static const uint8_t data_37A4D[16] = {
  0x47, 0x51, 0x40, 0x0A, 0xD7, 0x33, 0xC0, 0x00, 0x00, 0x80, 0x3F, 0xA4, 0x70, 0x97, 0x42, 0x00
};
static const uint8_t data_37A5D[3] = {
  0x00, 0x80, 0x3F
};
static const uint32_t data_37A60 = 0xC297999A;
static const float data_37A64[195] = {
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
static const char data_37D70[] __attribute__((scn_string_literal)) = "/scene/03/D000/";
static const char data_37D80[] __attribute__((scn_string_literal)) = "MAP1";
static const char data_37D85[] __attribute__((scn_string_literal)) = "/scene/03/D000/";
static const char data_37D95[] __attribute__((scn_string_literal)) = "MAP0";
static const char data_37D9A[] __attribute__((scn_string_literal)) = "/scene/03/D000/";
static const char data_37DAA[] __attribute__((scn_string_literal)) = "MAP3";
static const char data_37DAF[] __attribute__((scn_string_literal)) = "/scene/03/D000/";
static const char data_37DBF[] __attribute__((scn_string_literal)) = "MAP2";
static const char data_37DC4[11] __attribute__((scn_string_literal)) = "bgm109.snd";
static const char data_37DCF[] __attribute__((scn_string_literal)) = "/scene/03/D000/";
static const char data_37DDF[] __attribute__((scn_string_literal)) = "YORU";
static const char data_37DE4[13] __attribute__((scn_string_literal)) = "seqdata3.bin";
static const char data_37DF1[5] __attribute__((scn_string_literal)) = "YORU";
static const char data_37DF6[13] __attribute__((scn_string_literal)) = "seqdata4.bin";
static const char data_37E03[5] __attribute__((scn_string_literal)) = "YORU";
static const char data_37E08[13] __attribute__((scn_string_literal)) = "seqdata5.bin";
static const char data_37E15[5] __attribute__((scn_string_literal)) = "YORU";
static const char data_37E1A[] __attribute__((scn_string_literal)) = "YORU";
static const char data_37E1F[11] __attribute__((scn_string_literal)) = "SA1091A004";
static const char data_37E2A[11] __attribute__((scn_string_literal)) = "SA1091A005";
static const char data_37E35[11] __attribute__((scn_string_literal)) = "SA1091A006";
static const uint32_t data_37E40 = 380;
static const int32_t data_37E44[6] = {1, 390, 0, 1018, 0, -1};
static const uint32_t data_37E5C = 510;
static const int32_t data_37E60[6] = {1, 525, 0, 1020, 0, -1};
static const uint32_t data_37E78 = 150;
static const int32_t data_37E7C[6] = {1, 160, 0, 1019, 0, -1};
static const uint32_t data_37E94 = 530;
static const int32_t data_37E98[6] = {1, 970, 0, 1014, 0, -1};
static const uint32_t data_37EB0 = 911;
static const int32_t data_37EB4[6] = {1, 155, 0, 1015, 0, -1};
static const char data_37ECC[7] __attribute__((scn_string_literal)) = "FREE02";
static const char data_37ED3[7] __attribute__((scn_string_literal)) = "FREE03";
static const char data_37EDA[7] __attribute__((scn_string_literal)) = "FREE04";
static const char data_37EE1[7] __attribute__((scn_string_literal)) = "FREE01";
static const char data_37EE8[6] __attribute__((scn_string_literal)) = "E1040";
static const char data_37EEE[10] __attribute__((scn_string_literal)) = "E1040A028";
static const char data_37EF8[10] __attribute__((scn_string_literal)) = "E1040A029";
static const char data_37F02[6] __attribute__((scn_string_literal)) = "E1040";
static const char data_37F08[10] __attribute__((scn_string_literal)) = "E1040A030";
static const char data_37F12[10] __attribute__((scn_string_literal)) = "E1040A031";
static const char data_37F1C[10] __attribute__((scn_string_literal)) = "E1040A032";
static const char data_37F26[6] __attribute__((scn_string_literal)) = "E1040";
static const Vector3 data_37F2C = {.x = 41.99f, .y = 0.0f, .z = 23.13f};
static const Vector3 data_37F38 = {.x = 9.6f, .y = 0.0f, .z = 47.17f};
static const char data_37F44[13] __attribute__((scn_string_literal)) = "e1shugyo.snd";
static const char data_37F51[10] __attribute__((scn_string_literal)) = "E1040A001";
static const char data_37F5B[10] __attribute__((scn_string_literal)) = "E1040A003";
static const char data_37F65[10] __attribute__((scn_string_literal)) = "E1040A009";
static const char data_37F6F[10] __attribute__((scn_string_literal)) = "E1040A010";
static const char data_37F79[10] __attribute__((scn_string_literal)) = "E1040A012";
static const char data_37F83[10] __attribute__((scn_string_literal)) = "E1040A013";
static const char data_37F8D[10] __attribute__((scn_string_literal)) = "E1040A014";
static const char data_37F97[10] __attribute__((scn_string_literal)) = "E1040A018";
static const char data_37FA1[10] __attribute__((scn_string_literal)) = "E1040A020";
static const char data_37FAB[10] __attribute__((scn_string_literal)) = "E1040A015";
static const char data_37FB5[10] __attribute__((scn_string_literal)) = "E1040A016";
static const char data_37FBF[10] __attribute__((scn_string_literal)) = "E1040A017";
static const char data_37FC9[6] __attribute__((scn_string_literal)) = "E1040";
static const char data_37FCF[9] __attribute__((scn_string_literal)) = "E1040";
static const Vector3 data_37FD8 = {.x = 40.7094f, .y = 0.0f, .z = 21.8232f};
static const Vector3 data_37FE4 = {.x = 39.43f, .y = 0.0f, .z = 21.84f};
static const Vector3 data_37FF0 = {.x = 12.35f, .y = 0.0f, .z = 47.05f};
static const char data_37FFC[30] __attribute__((scn_string_literal)) = "training_npc_too_close:  set\012";
static const char data_3801A[20] __attribute__((scn_string_literal)) = "\012yamagisi ga iru!\012\012";
static const char data_3802E[15] __attribute__((scn_string_literal)) = "\012ryu ga iru!\012\012";
static const char data_3803D[55] __attribute__((scn_string_literal)) = "training_npc_too_close reset before training starts \012";
static void (*data_38074)(void) = script_28EA2;
static void (*const data_38078)(void) = script_28EA5;
static void (*const data_3807C)(int32_t) = script_28EA8;
static void (*const data_38080)(int32_t) = script_28ED9;
static void (*const data_38084)(int32_t) = script_28F0A;
static void (*const data_38088)(int32_t) = script_28F3B;
static const char data_3808C[] __attribute__((scn_string_literal)) = "/scene/03/D000/";
static const uint32_t data_3809C = 0x474D4F;
static const char data_380A0[11] __attribute__((scn_string_literal)) = "M_MOBJ.BIN";
static const char data_380AB[] __attribute__((scn_string_literal)) = "/scene/03/D000/";
static const char data_380BB[] __attribute__((scn_string_literal)) = "SHUKI";
static const char data_380C1[14] __attribute__((scn_string_literal)) = "scene/03/D000";
static const char data_380CF[] __attribute__((scn_string_literal)) = "SCRTEST.BIN";
static const char data_380DB[] __attribute__((scn_string_literal)) = "BUSS";
static const char data_380E0[7] __attribute__((scn_string_literal)) = "SA1053";
static const char data_380E7[12] __attribute__((scn_string_literal)) = "system1.snd";
static const char data_380F3[11] __attribute__((scn_string_literal)) = "amb022.snd";
static const char data_380FE[7] __attribute__((scn_string_literal)) = "SELECT";
static const char data_38105[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_3810A[13] __attribute__((scn_string_literal)) = "f1dobuit.snd";
static const char data_38117[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_3811C[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_38121[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_38126[12] __attribute__((scn_string_literal)) = "system1.snd";
static const char data_38132[11] __attribute__((scn_string_literal)) = "amb051.snd";
static const char data_3813D[7] __attribute__((scn_string_literal)) = "SELECT";
static const char data_38144[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_38149[13] __attribute__((scn_string_literal)) = "f1dobuit.snd";
static const char data_38156[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_3815B[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_38160[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_38165[12] __attribute__((scn_string_literal)) = "system1.snd";
static const char data_38171[11] __attribute__((scn_string_literal)) = "amb050.snd";
static const char data_3817C[7] __attribute__((scn_string_literal)) = "SELECT";
static const char data_38183[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_38188[13] __attribute__((scn_string_literal)) = "f1dobuit.snd";
static const char data_38195[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_3819A[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_3819F[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_381A4[12] __attribute__((scn_string_literal)) = "system1.snd";
static const char data_381B0[11] __attribute__((scn_string_literal)) = "amb052.snd";
static const char data_381BB[7] __attribute__((scn_string_literal)) = "SELECT";
static const char data_381C2[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_381C7[13] __attribute__((scn_string_literal)) = "f1dobuit.snd";
static const char data_381D4[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_381D9[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_381DE[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_381E3[12] __attribute__((scn_string_literal)) = "system1.snd";
static const char data_381EF[11] __attribute__((scn_string_literal)) = "amb021.snd";
static const char data_381FA[7] __attribute__((scn_string_literal)) = "SELECT";
static const char data_38201[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_38206[13] __attribute__((scn_string_literal)) = "f1dobuit.snd";
static const char data_38213[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_38218[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_3821D[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_38222[] __attribute__((scn_string_literal)) = "SHUKI";
static const char data_38228[] __attribute__((scn_string_literal)) = "/scene/03/D000/";
static const char data_38238[] __attribute__((scn_string_literal)) = "SHUKI";
static const uint8_t data_3823E[16] = {
  0x00, 0x00, 0x9E, 0x02, 0xA0, 0x02, 0xA2, 0x02, 0xA5, 0x02, 0xAC, 0x02, 0xAA, 0x02, 0xA8, 0x02
};
static const uint8_t data_3824E[16] = {
  0x00, 0x03, 0xF8, 0x00, 0xFA, 0x00, 0xF5, 0x00, 0xF7, 0x00, 0xF6, 0x00, 0xFB, 0x00, 0xF9, 0x00
};
static const uint8_t data_3825E[2] = {
  0x00, 0x03
};
static const char data_38260[61] __attribute__((scn_string_literal)) = "WAZA Warning : handle is not NULL, and create same handle !\012";
static const char data_3829D[30] __attribute__((scn_string_literal)) = "POS POS_RYOU_0 = %f, %f, %f.\012";
static const char data_382BB[30] __attribute__((scn_string_literal)) = "POS POS_KAIS_0 = %f, %f, %f.\012";
static const char data_382D9[29] __attribute__((scn_string_literal)) = "define ANG_RYOU_0 DEG(%d.0)\012";
static const char data_382F6[29] __attribute__((scn_string_literal)) = "define ANG_KAIS_0 DEG(%d.0)\012";
static const char data_38313[4] __attribute__((scn_string_literal)) = "%s\012";
static const char data_38317[37] __attribute__((scn_string_literal)) = "Konnna Error ha Detekonai hazu : %d\012";
static const Vector3 data_3833C = {.x = 1.0f, .y = 1.0f, .z = 1.0f};
static const Vector3i data_38348 = {.x = 0, .y = 0, .z = 0};
static const char data_38354[14] __attribute__((scn_string_literal)) = "Entry Model!\012";
static const char data_38362[38] __attribute__((scn_string_literal)) = "WZ ModoriMove Limit Over !!!!!!!!!!!\012";
static const char data_38388[30] __attribute__((scn_string_literal)) = "WazaDen: Load PackFile %s:%s\012";
static const char data_383A6[34] __attribute__((scn_string_literal)) = "WazaDen: Now Loading PackFile.\012";
static const char data_383C8[4] __attribute__((scn_string_literal)) = "";
