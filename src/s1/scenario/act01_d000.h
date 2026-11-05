#pragma once

#include "scenario.h"
#include "motion/motion.h"
#include "scene/01/d000/m_tpin.h"
#include "scene/01/yq14/m_yq14.h"

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
static int32_t script_25FC(int32_t arg0);
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
static void script_3C61(void);
static void script_3CDD(void);
static int32_t script_3CFD(void);
static void script_3DF7(int32_t arg0);
static void script_3E40(int32_t arg0);
static int32_t script_3E6C(int32_t arg0, int32_t arg1);
static int8_t script_3E9F(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_3FE1(int32_t arg0, int32_t arg1);
static int8_t script_41AA(int32_t arg0, int32_t arg1);
static int32_t script_4380(int32_t arg0, const char *arg1);
static void script_43F5(const char *arg0);
static int32_t script_4416(int32_t arg0, const char *arg1, int32_t arg2);
static int32_t script_445B(int32_t arg0, int32_t arg1);
static void script_4489(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_449D(int32_t arg0, int32_t arg1);
static int32_t script_44AF(const char *arg0, int32_t arg1);
static void script_44E0(int32_t arg0, int32_t arg1);
static int32_t script_44F2(int32_t arg0, int32_t arg1, int32_t arg2, const Vector3 *arg3, const Vector3i *arg4, int32_t arg5);
static void script_4525(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static int32_t script_453F(int32_t arg0, int32_t arg1);
static void script_4567(int32_t arg0, int32_t arg1);
static void script_4579(int32_t arg0, int32_t arg1);
static int32_t script_45B7(int32_t arg0, int32_t arg1, int32_t arg2, float arg3, int32_t arg4, int32_t arg5);
static int32_t script_4628(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4);
static void script_468A(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4);
static void script_46A2(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static int32_t script_46BC(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_46FA(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_4710(int32_t arg0, int32_t arg1, const Vector3i *arg2, int32_t arg3);
static void script_4741(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_4757(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4);
static void script_4788(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4);
static int32_t script_47A0(int32_t arg0, int32_t arg1, const Vector3 *arg2, int32_t arg3);
static void script_47ED(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_4803(int32_t arg0, int32_t arg1, const Vector3i *arg2, int32_t arg3);
static void script_4852(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_4868(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_4896(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_48AC(int32_t arg0);
static void script_48EB(void);
static void script_4917(int32_t arg0, int32_t arg1, int32_t arg2);
static int32_t script_4A3D(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_4AAF(int32_t arg0, int32_t arg1, int32_t arg2);
static int32_t script_4AC3(int32_t arg0);
static int32_t script_4AE9(int32_t arg0);
static int32_t script_4B07(int32_t arg0, int32_t arg1);
static void script_4B35(int32_t arg0, int32_t arg1);
static void script_4B47(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_4C78(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_4CAA(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_4D79(void);
static void script_4DB3(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_4F10(int32_t arg0);
static void script_4FEB(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_508C(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_5195(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4);
static void script_54D9(int32_t arg0, int32_t arg1);
static void script_55A7(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_5898(int32_t arg0, int32_t arg1, int32_t arg2);
static int32_t script_5B82(int32_t arg0);
static void script_5FE6(int32_t arg0);
static void script_6001(void);
static void script_6012(void);
static void script_6019(void);
static void script_6026(void);
static void script_6207(void);
static void script_6430(void);
static void script_667D(void);
static void script_68A6(int32_t arg0, int32_t arg1);
static void script_68C7(void);
static void script_68ED(void);
static void script_68FF(void);
static void script_6925(int32_t arg0);
static void script_6959(void);
static void script_69E4(int32_t arg0);
static void script_6DA3(void);
static void script_6DF7(void);
static void script_6F3E(int32_t arg0);
static void script_6F59(int32_t arg0);
static void script_6FAA(int32_t arg0);
static void script_7027(void);
static void script_703C(void);
static void script_7045(void);
static void script_7048(int32_t arg0);
static void script_7088(int32_t arg0);
static int32_t script_70F5(void);
static int32_t script_728A(void);
static int32_t script_72BF(void);
static int32_t script_7350(void);
static int32_t script_73FD(void);
static int32_t script_7484(void);
static void script_7565(int32_t arg0, int32_t arg1);
static int32_t script_75D4(int32_t arg0, int32_t arg1, float arg2);
static int32_t script_763D(int32_t arg0, int32_t arg1, float arg2);
static void script_76AF(int32_t arg0);
static void script_770D(int32_t arg0);
static void script_776B(int32_t arg0);
static int32_t script_77C9(int32_t arg0, int32_t arg1, float arg2, float arg3);
static void script_7842(int32_t arg0, float arg1, float arg2, float arg3);
static void script_786A(int32_t arg0, float arg1, float arg2, float arg3);
static void script_78BE(int32_t arg0, int32_t arg1, int32_t arg2);
static int32_t script_790E(int32_t arg0);
static void script_79B5(int32_t arg0, int32_t arg1);
static int32_t script_79D8(int32_t arg0);
static void script_7A0B(int32_t arg0, int32_t arg1);
static void script_7A2E(int32_t arg0, int32_t arg1);
static void script_7A8D(int32_t arg0);
static void script_7AA2(void);
static void script_7AB1(void);
static void script_7AC0(void);
static void script_7AC3(void);
static void script_7AC6(void);
static void script_7AC9(void);
static void script_7ACC(void);
static int32_t script_7AD9(void);
static void script_7AE5(void);
static int32_t script_7AF4(void);
static int32_t script_7B03(int32_t arg0);
static void script_7B21(float arg0);
static int32_t script_7B40(int32_t arg0, int32_t arg1);
static void script_7BB8(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4);
static int32_t script_816A(int32_t arg0);
static void script_823B(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_824B(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_834F(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_84F1(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_8652(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_87B3(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_8A51(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_8B1F(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_8B86(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_8BED(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_8C34(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_8D6A(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_962B(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_9749(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_9E0E(int32_t arg0, int32_t arg1);
static void script_9EE4(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_ADF7(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_AE63(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_B31D(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_B9D7(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_D2B7(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_D9D8(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_E162(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_E23A(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_E6B1(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_F644(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_FA8E(int32_t arg0, DSWKDoorEntry *arg1, int32_t *arg2, DoorResourceDescriptor *arg3, MapObjectRecord *arg4, int32_t arg5, int32_t arg6);
static void script_FC1A(DSWKDoorEntry *arg0, int32_t *arg1, DoorResourceDescriptor *arg2, MapObjectRecord *arg3, int32_t arg4);
static void script_FC4E(void);
static void script_FCB7(int32_t arg0, int32_t arg1);
static void script_FD54(int32_t arg0, int32_t arg1);
static void script_FD8E(void);
static void script_FDF1(void);
static void script_FE54(int32_t arg0);
static void script_FE8B(int32_t arg0);
static void script_FEC2(int32_t arg0);
static void script_10104(int32_t arg0);
static void script_1014D(int32_t arg0);
static void script_10196(int32_t arg0, int32_t arg1);
static void script_1020C(int32_t arg0);
static void script_1021A(int32_t arg0);
static void script_10228(int32_t arg0);
static void script_102EF(void);
static void script_102FB(void);
static void script_10307(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5, int32_t arg6);
static int32_t script_1034A(void);
static int8_t script_1045B(int32_t arg0, int32_t arg1, int8_t arg2);
static void script_104F3(int32_t arg0);
static void script_106C2(int32_t arg0);
static int32_t script_106F3(int32_t arg0, int32_t arg1, int32_t arg2, float arg3, float arg4, int32_t arg5, int32_t arg6);
static void script_10DBC(int32_t arg0, int32_t arg1);
static void script_10DEE(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_10F5E(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_1111A(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_112CE(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_114E4(int32_t arg0, int32_t arg1);
static void script_115AE(int32_t arg0, int32_t arg1);
static void script_1160F(int32_t arg0);
static void script_117B4(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_1184C(int32_t arg0, int32_t arg1);
static void script_1192E(int32_t arg0);
static void script_11985(int32_t arg0, int32_t arg1);
static void script_119E4(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static int32_t script_11A0B(float arg0, float arg1, float arg2, float arg3);
static int32_t script_11BA9(void);
static void script_11C70(const char *arg0, int32_t arg1);
static void script_11CBD(void);
static int32_t script_11DF4(int32_t arg0);
static void script_11E6D(void);
static int8_t script_11ECF(void);
static void script_11F1A(int32_t arg0);
static void script_11F83(int32_t arg0);
static void script_11FAE(void);
static int32_t script_11FD5(int32_t arg0);
static int8_t script_12017(int32_t arg0);
static void script_120EF(void);
static void script_120FC(void);
static void script_125D1(void);
static void script_13118(void);
static void script_131D6(void);
static void script_131FB(void);
static void script_13942(void);
static int8_t script_159C9(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int8_t script_15A82(void);
static void script_15C38(int32_t arg0);
static int8_t script_16DA9(void);
static void script_172F8(int32_t arg0);
static void script_1B1B6(void);
static int8_t script_1BC7E(void);
static int8_t script_1BCA6(void);
static void script_1BDC1(int32_t arg0);
static int32_t script_1BEAA(int32_t arg0, int32_t arg1);
static int32_t script_1BF13(void);
static void script_1BF2E(int32_t arg0);
static void script_1C017(int32_t arg0);
static void script_1C205(void);
static void script_1C23C(int32_t arg0);
static int8_t script_1C304(void);
static void script_1CFD3(void);
static void script_1D159(void);
static void script_1D16D(void);
static void script_1D1B7(void);
static int32_t script_1D2FC(void);
static int32_t script_1D37F(void);
static void script_1DAAB(void);
static void script_1DB1E(void);
static void script_1DB7E(void);
static void script_1DC1C(void);
static void script_1DCB8(int32_t arg0);
static int16_t script_1DE59(int8_t arg0, int8_t arg1);
static void script_1E044(int32_t arg0, int32_t arg1);
static int32_t script_1E093(int32_t arg0, int32_t arg1);
static void script_1E0E6(int32_t arg0, int32_t arg1, int32_t arg2, float arg3);
static void script_1E31D(void);
static int32_t script_1E489(int32_t arg0);
static int32_t script_1E4F3(int32_t arg0);
static int32_t script_1E724(void);
static int32_t script_1E7D4(void);
static int32_t script_1E877(void);
static void script_1E909(void);
static void script_1E936(int32_t arg0);
static void script_1EA08(void);
static void script_1EBAF(void);
static void script_1EC89(void);
static void script_1ED4D(void);
static void script_1ED7C(int32_t arg0);
static void script_1EF05(int32_t arg0, int32_t arg1);
static int32_t script_1EF86(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4);
static int32_t script_1F040(void);
static int32_t script_1F17B(float arg0);
static int32_t script_1F24B(void);
static int32_t script_1F314(void);
static int32_t script_1F3BE(void);
static int32_t script_1F408(void);
static void script_1F503(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_1F6F4(void);
static int32_t script_1FA90(void);
static int32_t script_1FB82(int32_t arg0);
static int32_t script_1FC02(int32_t arg0);
static void script_1FD34(int32_t arg0);
static int32_t script_1FFA0(void);
static void script_20078(void);
static int32_t script_2007B(int32_t arg0);
static int32_t script_20191(int16_t arg0);
static int32_t script_202BA(void);
static void script_202E3(void);
static int32_t script_2034D(void);
static int32_t script_2040D(void);
static int32_t script_2072C(void);
static void script_20809(void);
static int32_t script_20821(void);
static int32_t script_20834(void);
static void script_2090F(void);
static int32_t script_209EA(int32_t arg0, int32_t arg1);
static void script_20BA3(int32_t arg0);
static void script_20C36(int32_t arg0, int32_t arg1);
static void script_20CEC(void);
static void script_20D66(int32_t arg0);
static int32_t script_20E7C(void);
static int32_t script_213BE(int32_t arg0, const char *arg1, const char *arg2, ...);
static void script_21410(int32_t arg0, Vector3 *arg1);
static void script_21448(int32_t arg0, Vector3i *arg1);
static void script_21480(int32_t arg0, int32_t arg1);
static int32_t script_214BF(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_2154F(int32_t arg0, int32_t arg1);
static int32_t script_2158F(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_2165C(int32_t arg0, int32_t arg1);
static int32_t script_216EA(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4);
static int32_t script_21752(int32_t arg0, int32_t arg1, int32_t arg2);
static int32_t script_21799(int32_t arg0, int32_t arg1, int32_t arg2);
static int32_t script_21825(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_21887(int32_t arg0);
static void script_2197D(void);
static void script_21C70(void);
static void script_21D41(void);
static void script_21DB6(void);
static void script_21E2B(void);
static void script_21EA6(void);
static void script_21F27(void);
static void script_21FA8(void);
static void script_22016(void);
static void script_22072(void);
static void script_22111(void);
static void script_22165(void);
static void script_221C1(void);
static void script_2227A(void);
static void script_22290(int32_t arg0);
static void script_222CB(int32_t arg0);
static int32_t script_222F5(void);
static int32_t script_22446(void);
static int32_t script_2258E(void);
static void script_226D6(void);
static void script_22B3A(void);
static void script_22DB0(void);
static void script_22DF1(int32_t arg0);
static void script_2300E(int32_t arg0, int32_t arg1);
static void script_23080(void);
static int32_t script_231D5(void);
static int32_t script_232C0(void);
static void script_233AB(void);
static void script_23419(void);
static void script_23466(int32_t arg0);
static int32_t script_234D0(int32_t arg0);
static void script_23574(int32_t arg0);
static void script_23618(void);
static void script_23667(int32_t arg0);
static void script_236A0(void);
static int32_t script_23722(void);
static void script_2377A(int32_t arg0, int32_t arg1);
static void script_2380D(void);
static void script_23810(void);
static int32_t script_23813(void);
static int32_t script_23816(void);
static int32_t script_23819(void);
static int32_t script_2381C(void);
static void script_2381F(void);
static void script_23822(void);
static void script_23825(void);
static int32_t script_23828(void);
static int32_t script_2382B(void);
static int32_t script_2382E(void);
static int32_t script_23831(void);
static int32_t script_23834(void);
static int32_t script_23837(void);
static int32_t script_2383A(void);
static int32_t script_2383D(void);
static int32_t script_23840(void);
static int32_t script_23843(void);
static int32_t script_23846(void);
static int32_t script_23849(void);
static int32_t script_2384C(void);
static int32_t script_2384F(void);
static int32_t script_23852(void);
static int32_t script_23855(void);
static int32_t script_23858(void);
static int32_t script_2385B(void);
static int32_t script_2385E(void);
static int32_t script_23861(void);
static int32_t script_23864(void);
static int32_t script_23867(void);
static void script_2386A(void);
static int32_t script_2386D(void);
static int32_t script_23870(void);
static int32_t script_23873(void);
static int32_t script_23876(void);
static int32_t script_23879(void);
static int32_t script_2387C(void);
static int32_t script_2387F(void);
static int32_t script_23882(void);
static int32_t script_23885(void);
static int32_t script_23888(void);
static int32_t script_2388B(void);
static int32_t script_2388E(void);
static int32_t script_23891(void);
static int32_t script_23894(void);
static int32_t script_23897(void);
static int32_t script_2389A(void);
static int32_t script_2389D(void);
static int32_t script_238A0(void);
static int32_t script_238A3(void);
static int32_t script_238A6(void);
static int32_t script_238A9(void);
static int32_t script_238AC(void);
static int32_t script_238AF(void);
static int32_t script_238B2(void);
static int32_t script_238B5(void);
static int32_t script_238B8(void);
static void script_238BB(void);
static void script_238BE(void);
static int32_t script_238C1(void);
static int32_t script_238CD(void);
static int32_t script_238D9(void);
static int32_t script_238E5(void);
static int32_t script_238F1(void);
static int32_t script_238FD(void);
static void script_23909(void);
static int32_t script_23915(void);
static int32_t script_23921(void);
static int32_t script_2392D(void);
static int32_t script_23939(void);
static int32_t script_23945(void);
static int32_t script_23951(void);
static int32_t script_2395D(void);
static int32_t script_23969(void);
static int32_t script_23975(void);
static int32_t script_23981(void);
static int32_t script_2398D(void);
static int32_t script_23999(void);
static int32_t script_239A5(void);
static int32_t script_239B1(void);
static int32_t script_239BD(void);
static int32_t script_239C9(void);
static int32_t script_239D5(void);
static int32_t script_239E1(void);
static int32_t script_239ED(void);
static void script_239F9(void);
static int32_t script_239FC(void);
static int32_t script_23A08(void);
static int32_t script_23A14(void);
static int32_t script_23A20(void);
static int32_t script_23A2C(void);
static void script_23A38(void);
static int32_t script_23A44(void);
static int32_t script_23A50(void);
static int32_t script_23A5C(void);
static int32_t script_23A68(void);
static int32_t script_23A74(void);
static int32_t script_23A80(void);
static int32_t script_23A8C(void);
static int32_t script_23A98(void);
static int32_t script_23AA4(void);
static int32_t script_23AB0(void);
static int32_t script_23ABC(void);
static int32_t script_23AC8(void);
static int32_t script_23AD4(void);
static int32_t script_23AE0(void);
static int32_t script_23AEC(void);
static int32_t script_23AF8(void);
static int32_t script_23B04(void);
static int32_t script_23B10(void);
static int32_t script_23B1C(void);
static int32_t script_23B28(void);
static int32_t script_23B34(void);
static int32_t script_23B40(void);
static int32_t script_23B4C(void);
static int32_t script_23B58(void);
static int32_t script_23B64(void);
static int32_t script_23B70(void);
static int32_t script_23B7C(void);
static int32_t script_23B88(void);
static int32_t script_23B94(void);
static int32_t script_23BA0(void);
static int32_t script_23BAC(void);
static int32_t script_23BB8(void);
static int32_t script_23BC4(void);
static int32_t script_23BD0(void);
static int32_t script_23BDC(void);
static int32_t script_23BE8(void);
static int32_t script_23BF4(void);
static int32_t script_23C00(void);
static int32_t script_23C0C(void);
static int32_t script_23C18(void);
static int32_t script_23C24(int32_t arg0, int32_t arg1, int32_t arg2);
static int32_t script_23E7B(int32_t arg0, int32_t arg1);
static int32_t script_240C9(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_24794(int32_t arg0, int32_t arg1);
static int32_t script_24875(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_24A81(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_24B13(int32_t arg0, int32_t arg1, int32_t arg2);
static int32_t script_24D4C(int32_t arg0, int32_t arg1, int32_t arg2);
static int32_t script_25299(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4);
static int32_t script_25381(void);
static void script_253FA(void);
static void script_2599D(void);
static void script_259EC(void);
static int32_t script_25A3D(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5, int32_t arg6, int32_t arg7);
static void script_26012(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_26028(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_26058(int32_t arg0, int32_t arg1);
static int32_t script_2606A(int32_t arg0, int32_t arg1);
static void script_26088(int32_t arg0, int32_t arg1);
static int32_t script_2609A(int32_t arg0, int32_t arg1);
static void script_2620B(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static int32_t script_26225(int32_t arg0, int32_t arg1, int32_t arg2, float arg3, int32_t arg4, int32_t arg5, int32_t arg6);
static void script_2628D(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static int32_t script_262A7(int32_t arg0, int32_t arg1, float arg2, float arg3, float arg4, int32_t arg5);
static void script_262CC(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4);
static int32_t script_262E4(int32_t arg0, float arg1, float arg2, float arg3, int32_t arg4, int32_t arg5);
static void script_26352(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_26368(int32_t arg0, int32_t arg1, float arg2, int32_t arg3);
static void script_26389(int32_t arg0, int32_t arg1, float arg2, float arg3, float arg4);
static void script_2641B(int32_t arg0, int32_t arg1, float arg2);
static void script_2646F(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_26485(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_264BF(int32_t arg0, int32_t arg1);
static void script_264DA(int32_t arg0, int32_t arg1, int32_t arg2);
static int32_t script_264EE(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_2650D(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_26523(int32_t arg0, int32_t arg1, int32_t arg2, float arg3);
static void script_26580(int32_t arg0, int32_t arg1, int32_t arg2);
static int32_t script_26594(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_265B1(void);
static void script_265D9(void);
static void script_26999(void);
static void script_26C35(void);
static void script_26E23(void);
static void script_274FA(void);
static void script_27C19(void);
static void script_28443(void);
static void script_289CE(void);
static void script_28A0F(void);
static void script_28A6F(void);
static void script_28B1A(void);
static void script_28B6B(void);
static int32_t script_28F90(int32_t arg0, int32_t arg1);
static void script_290D9(int32_t arg0, int32_t arg1);
static int32_t script_2916C(void);
static void script_294E8(void);
static int32_t script_29522(float arg0, float arg1);
static int32_t script_29659(void);
static void script_297D5(int32_t arg0);
static void script_2982E(int32_t arg0);
static void script_299F7(void);
static void script_299FA(void);
static void script_29ADC(void);
static void script_29B7A(void);
static void script_29D90(void);
static void script_29DE5(void);
static void script_29E85(void);
static int32_t script_2A4E5(void);
static int32_t script_2A5E1(void);
static int8_t script_2A620(void);
static void script_2A667(void);
static void script_2A6CF(void);
static void script_2A6DB(void);
static void script_2A6E7(int32_t arg0);
static void script_2A819(int32_t arg0, int32_t arg1);
static void script_2A954(void);
static void script_2AA0C(void);
static int32_t script_2AA7C(int32_t arg0);
static int32_t script_2AD09(int32_t arg0);
static int32_t script_2AD59(void);
static void script_2AD76(void);
static void script_2AE1A(void);
static void script_2AF7D(void);
static void script_2B034(void);
static void script_2B325(int32_t arg0);
static void script_2B68A(int32_t arg0, int32_t arg1);
static void script_2B731(int32_t arg0, int32_t arg1);
static void script_2B795(int32_t arg0, int32_t arg1);
static void script_2B83C(int32_t arg0, int32_t arg1);
static int32_t script_2B8A0(int32_t arg0, int32_t arg1);
static int32_t script_2B911(int32_t arg0, int32_t arg1);
static void script_2B982(int32_t arg0, int32_t arg1);
static void script_2BAC3(int32_t arg0, int32_t arg1);
static int32_t script_2BB5E(int32_t arg0);
static int32_t script_2BBB1(int32_t arg0);
static void script_2BC0C(int32_t arg0);
static int32_t script_2BC8A(int32_t arg0);
static void script_2BCEF(int32_t arg0, int32_t arg1);
static void script_2BD0F(float arg0, float arg1, float arg2, int32_t arg3);
static void script_2BD69(float arg0, float arg1, float arg2, int32_t arg3, int32_t arg4);
static void script_2BDC2(int32_t arg0);
static void script_2BE2D(int32_t arg0);
static void script_2BE98(void);
static int32_t script_2BEC0(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_2BF28(const char *arg0, int32_t arg1);
static int32_t script_2BF54(int32_t arg0, int32_t arg1);
static void script_2BF84(int32_t arg0);
static void script_2C521(int32_t arg0);
static void script_2CB19(int32_t arg0);
static int32_t script_2CBF3(int32_t arg0);
static int32_t script_2CD22(int32_t arg0);
static void script_2CDF5(int32_t arg0);
static int32_t script_2D314(int32_t arg0, int32_t arg1);
static void script_2D332(int32_t arg0);
static void script_2D84B(int32_t arg0);
static void script_2DD64(int32_t arg0);
static void script_2DF44(int32_t arg0);
static void script_2E070(int32_t arg0);
static void script_2E262(int32_t arg0);
static void script_2E4A8(void);
static int32_t script_2E9EA(void);
static void script_2EA62(void);
static void script_2EA8A(void);
static void script_2EAA9(int32_t arg0);
static int8_t script_2EB3F(int8_t arg0);
static int8_t script_2ECB0(int32_t arg0);
static int8_t script_2EF19(int32_t arg0);
static int8_t script_2F008(int32_t arg0);
static int8_t script_2F1C4(void);
static int8_t script_2F1DE(void);
static int8_t script_2F281(void);
static int8_t script_2F329(void);
static int8_t script_2F3C2(void);
static int8_t script_2F4A5(void);
static int8_t script_2F505(void);
static int32_t script_2F587(void);
static void script_2F5EE(int32_t arg0);
static int32_t script_2FFAD(int32_t arg0, int32_t arg1);
static int32_t script_2FFBF(int32_t arg0, int32_t arg1);
static void script_3026B(void);
static int32_t script_30291(void);
static void script_304CC(void);
static void script_306DB(void);
static void script_307F5(int32_t arg0);
static void script_308FF(int32_t arg0);
static void script_30AF4(void);
static void script_30B23(void);
static int32_t script_30B32(int32_t arg0);
static void script_30B77(void);
static void script_30F3C(void);
static int32_t script_31287(int32_t arg0);
static void script_31441(void);
static void script_314B5(void);
static void script_31563(void);
static void script_31626(void);
static void script_3164D(void);
static void script_31674(void);
static void script_31681(void);
static void script_31689(void);
static void script_316E4(void);
static void script_31708(void);
static void script_31735(void);
static void script_31A19(void);
static int32_t script_31B7F(void);
static void script_31D28(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4);
static int32_t script_31D9B(int32_t arg0);
static void script_32192(void);
static int8_t script_321C0(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_3223F(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_323BC(void);
static void script_325D2(void);
static int32_t script_325FB(void);
static void script_3266E(void);
static void script_32718(void);
static void script_3274B(void);
static void script_32794(void);
static void script_327E5(void);
static void script_32886(void);
static void script_328E5(void);
static void script_329A7(void);
static void script_32A67(void);
static void script_32B9F(int32_t arg0);
static void script_32BAB(int32_t arg0);
static void script_32BB7(int32_t arg0);
static void script_32CC3(int32_t arg0);
static void script_32CDA(int32_t arg0);
static void script_32D1B(int32_t arg0);
static void script_32D5F(int32_t arg0);
static void script_32DDD(void);
static int32_t script_32E5C(int32_t arg0);
static int32_t script_32EE2(void);
static int32_t script_33005(void);
static void script_3308C(void);
static int32_t script_330E5(void);
static int32_t script_3311F(int32_t arg0);
static void script_33142(void);
static int32_t script_33162(int32_t arg0);
static int32_t script_33185(void);
static void script_331D0(void);
static void script_33217(void);
static void script_3326A(void);
static void script_332C6(void);
static void script_3330D(void);
static void script_33369(int8_t arg0);
static void script_3343F(void);
static void script_334B6(void);
static void script_334F3(void);
static int32_t script_33530(int32_t arg0);
static void script_335A1(int32_t arg0);
static int32_t script_33804(int32_t arg0);
static void script_3384C(void);
static void script_33854(void);
static void script_33868(int8_t arg0);
static void script_33B9C(void);
static void script_33DA9(void);
static void script_33DAC(void);
static void script_33E2D(void);
static void script_33E3B(void);
static void script_33E3E(void);
static void script_33E41(int32_t arg0);
static void script_33E72(int32_t arg0);
static void script_33EA3(int32_t arg0);
static void script_33ED4(int32_t arg0);
static void script_33F05(int32_t arg0);
static void script_34173(int32_t arg0);
static void script_343FA(void);
static void script_34627(void);
static void script_346D3(int32_t arg0);
static void script_3470E(int32_t arg0);
static int32_t script_34738(void);
static void script_347F8(void);
static void script_34858(void);
static void script_34DF1(int32_t arg0);
static void script_3500A(void);
static void script_35149(void);
static void script_351F0(void);
static void script_3597B(void);
static int32_t script_359D4(void);
static int32_t script_35ABC(void);
static int32_t script_35B55(void);
static void script_35BA3(void);
static int32_t script_363C2(void);
static int32_t script_363F1(void);
static void script_36420(void);
static void script_3644F(void);
static void script_364CA(void);
static int32_t script_36575(void);
static void script_36596(void);
static int32_t script_365A6(void);
static int32_t script_365DC(void);
static int32_t script_365FE(void);
static int32_t script_36612(void);
static int32_t script_36684(void);
static int32_t script_366B2(void);
static int32_t script_366C0(void);
static void script_366E7(int32_t arg0);
static void script_36723(int32_t arg0);
static void script_36818(int32_t arg0);
static void script_36DA9(int32_t arg0);
static void script_3705B(int32_t arg0);
static void script_37385(int32_t arg0);
static void script_3743A(int32_t arg0);
static void script_3749A(void);
static void script_3758B(void);
static void script_37612(int32_t arg0);
static void script_3764D(int32_t arg0);
static void script_37677(int32_t arg0);
static int32_t script_37713(int32_t arg0);
static void script_37804(int32_t arg0);
static void script_378FC(int32_t arg0);
static int32_t script_379DE(void);
static void script_37AF3(int32_t arg0);
static int32_t script_37CED(void);
static void script_37ED8(void);
static void script_381B2(void);
static int32_t script_38425(void);
static int32_t script_3846C(void);
static int32_t script_3849B(void);
static void script_384CA(void);
static void script_3895F(void);
static void script_3898E(void);
static void script_38AD7(void);
static void script_38AE6(void);
void script_38AFA(void);
static void script_38B02(int32_t arg0);
static void script_38B20(float arg0);
static void script_38B3F(int32_t arg0);
static void script_38B54(int32_t arg0, int32_t arg1);
static int32_t script_38C0A(int32_t arg0);
static void script_38C22(int32_t arg0);
static void script_38C38(int32_t arg0);
static void script_38C46(void);
static void script_38D10(void);
static void script_38D13(void);
static void script_38E61(int32_t arg0);
static int32_t script_38E75(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_38E97(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_38EDD(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_38EF1(int32_t arg0);
static void script_38F0B(void);
static int32_t script_38F26(int32_t arg0);
static void script_38F53(int32_t arg0, int32_t arg1);
static void script_38F7A(void);
static void script_38F84(void);
static void script_38F9F(void);
static void script_38FA9(int32_t arg0);
static int32_t script_3900D(float arg0, float arg1, float arg2, float arg3);
static int32_t script_39034(int32_t arg0, int32_t arg1);
static int32_t script_390AC(int32_t arg0, int32_t arg1, int32_t arg2, const Vector3 *arg3);
static void script_39158(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_3931E(int32_t arg0);
static void script_39340(int32_t arg0, int32_t arg1, int32_t arg2);
static int32_t script_3941B(void);
static void script_3943F(int32_t arg0);
static int32_t script_394FF(int32_t arg0, int32_t arg1);
static int32_t script_39505(int32_t arg0);
static int32_t script_39510(int32_t arg0);
static void script_395EB(int32_t arg0, int32_t arg1);
static void script_39650(int32_t arg0, int32_t arg1);
static void script_396B5(void);
static void script_396F1(void);
static void script_3970C(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_39780(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_397A5(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_397EE(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_39854(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_3991C(int32_t arg0, int32_t arg1, int32_t arg2);
static int32_t script_39972(void);
static void script_399C7(int32_t arg0);
static void script_39A21(const char *arg0, const char *arg1);
static int32_t script_39A71(int32_t arg0, int32_t arg1);
static int32_t script_39A95(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4);
static int32_t script_39B6C(int32_t arg0, int32_t arg1, int32_t arg2, float arg3, int32_t arg4, int32_t arg5);
static int32_t script_39C03(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4);
static int32_t script_39C65(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_39CC1(int32_t arg0, float arg1, int32_t arg2);
static void script_39D1E(int32_t arg0);
static int32_t script_39D3A(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, void *arg4, int32_t arg5);
static void script_39F61(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_39F98(int32_t arg0);
static void script_39FA8(void);
static void script_39FC8(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_3A002(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_3A04E(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4);
static void script_3A097(int32_t arg0);
static int32_t script_3A0AA(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_3A10D(int32_t arg0, int32_t arg1);
static int32_t script_3A1CE(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_3A2BC(void);
static void script_3A3AA(int32_t arg0, int32_t arg1);
static void script_3A3D3(int32_t arg0, int32_t arg1, float arg2, float arg3, float arg4);
static void script_3A462(int32_t arg0, int32_t arg1, const Vector3 *arg2);
static void script_3A495(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static int32_t script_3A4AF(int32_t arg0, int32_t arg1, float arg2, float arg3, float arg4, int32_t arg5);
static void script_3A4DA(int32_t arg0, int32_t arg1, float arg2, float arg3, float arg4, float arg5);
static void script_3A55A(int32_t arg0, int32_t arg1, float arg2);
static void script_3A5AE(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_3A5C4(int32_t arg0, int32_t arg1, float arg2, int32_t arg3);
static void script_3A5EB(int32_t arg0, float arg1);
static void script_3A614(int32_t arg0, float arg1, float arg2, float arg3);
static void script_3A668(int32_t arg0, int32_t arg1, int32_t arg2);
static int32_t script_3A67C(int32_t arg0, float arg1, int32_t arg2);
static void script_3A6A1(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4);
static int32_t script_3A6B9(int32_t arg0, float arg1, float arg2, float arg3, int32_t arg4);
static void script_3A6E2(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4);
static int32_t script_3A6FA(int32_t arg0, float arg1, float arg2, float arg3, int32_t arg4);
static void script_3A74F(int32_t arg0, int32_t arg1);
static void script_3A834(int32_t arg0, int32_t arg1);
static void script_3A91D(int32_t arg0);
static void script_3A93B(void);
static void script_3A983(void);
static void script_3A99E(void);
static void script_3A9B8(const char *arg0, const char *arg1);
static void script_3A9F6(const char *arg0);
static void script_3AA17(int32_t arg0);
static void script_3AA2E(int32_t arg0);
static void script_3AAB4(int32_t arg0, int32_t arg1);
static int32_t script_3AAD7(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_3AF28(int32_t arg0, int32_t arg1);
static int32_t script_3AF3A(int32_t arg0, int32_t arg1);
static void script_3B23A(int32_t arg0, int32_t arg1);
static int32_t script_3B24C(int32_t arg0, int32_t arg1);
static void script_3B270(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_3B286(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_3B2AA(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_3B2C0(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_3B300(int32_t arg0, int32_t arg1);
static void script_3B31B(int32_t arg0, int32_t arg1, int32_t arg2);
static int32_t script_3B32F(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_3B354(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4);
static int32_t script_3B36C(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4);
static void script_3B393(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5, int32_t arg6);
static int32_t script_3B3AF(int32_t arg0, int32_t arg1, int32_t arg2, float arg3, int32_t arg4, int32_t arg5, int32_t arg6);
static void script_3B450(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_3B466(float arg0, float arg1, float arg2, int32_t arg3);
static void script_3B48B(int32_t arg0, int32_t arg1);
static void script_3B49C(int32_t arg0, int32_t arg1);
static int32_t script_3B654(int32_t arg0, int32_t arg1);
static int32_t script_3B91C(int32_t arg0, int32_t arg1);
static void script_3C31A(int32_t arg0, int32_t arg1);
static void script_3C6CA(int32_t arg0, int32_t arg1);
static void script_3CC50(int32_t arg0, int32_t arg1);
static int32_t script_3CF55(int32_t arg0, int32_t arg1);
static void script_3CFAB(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4);
static int32_t script_3CFC3(int32_t arg0, int32_t arg1, int32_t arg2, float arg3, float arg4);
static void script_3D01C(int32_t arg0, int32_t arg1);
static void script_3D069(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_3D07F(int32_t arg0, int32_t arg1, int32_t arg2, float arg3);
static void script_3D0E2(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_3D109(void);
static void script_3D130(void);
static void script_3D330(void);
static void script_3D363(void);
static int32_t script_3D50C(int32_t arg0, int32_t arg1);
static int32_t script_3D6C3(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_3D786(int32_t arg0, int32_t arg1);
static int32_t script_3D7AA(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4);
static int32_t script_3D881(int32_t arg0, int32_t arg1, int32_t arg2, float arg3, int32_t arg4, int32_t arg5);
static int32_t script_3D918(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4);
static int32_t script_3D97A(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_3D9D6(int32_t arg0, float arg1, int32_t arg2);
static void script_3DA33(int32_t arg0);
static int32_t script_3DA4F(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, void *arg4, int32_t arg5);
static void script_3DC76(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_3DCAD(int32_t arg0);
static void script_3DCBD(void);
static void script_3DCDD(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_3DD17(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_3DD63(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4);
static void script_3DDAC(int32_t arg0);
static int32_t script_3DDBF(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_3DE22(int32_t arg0, int32_t arg1);
static int32_t script_3DEE3(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_3DFD1(void);
static void script_3E0BF(int32_t arg0, int32_t arg1);
static void script_3E0E8(int32_t arg0, int32_t arg1, float arg2, float arg3, float arg4);
static void script_3E177(int32_t arg0, int32_t arg1, const Vector3 *arg2);
static void script_3E1AA(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static int32_t script_3E1C4(int32_t arg0, int32_t arg1, float arg2, float arg3, float arg4, int32_t arg5);
static void script_3E1EF(int32_t arg0, int32_t arg1, float arg2, float arg3, float arg4, float arg5);
static void script_3E26F(int32_t arg0, int32_t arg1, float arg2);
static void script_3E2C3(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_3E2D9(int32_t arg0, int32_t arg1, float arg2, int32_t arg3);
static void script_3E300(int32_t arg0, float arg1);
static void script_3E329(int32_t arg0, float arg1, float arg2, float arg3);
static void script_3E37D(int32_t arg0, int32_t arg1, int32_t arg2);
static int32_t script_3E391(int32_t arg0, float arg1, int32_t arg2);
static void script_3E3B6(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4);
static int32_t script_3E3CE(int32_t arg0, float arg1, float arg2, float arg3, int32_t arg4);
static void script_3E3F7(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4);
static int32_t script_3E40F(int32_t arg0, float arg1, float arg2, float arg3, int32_t arg4);
static void script_3E464(int32_t arg0, int32_t arg1);
static void script_3E549(int32_t arg0, int32_t arg1);
static void script_3E632(int32_t arg0);
static void script_3E650(void);
static void script_3E698(void);
static void script_3E6B3(void);
static void script_3E6CD(const char *arg0, const char *arg1);
static void script_3E70B(const char *arg0);
static void script_3E72C(int32_t arg0);
static void script_3E743(int32_t arg0);
static void script_3E7C9(int32_t arg0, int32_t arg1);
static void script_3E7EC(void);
static int32_t script_3E89A(void);
static int32_t script_3E98F(int32_t arg0, int32_t arg1, int32_t arg2);
static int32_t script_3EE17(int32_t arg0, int32_t arg1, int32_t arg2);
static int32_t script_3EF3E(int32_t arg0, int32_t arg1, int32_t arg2);
static int32_t script_3F04F(int32_t arg0, int32_t arg1);
static int32_t script_3F148(int32_t arg0, int32_t arg1, int32_t arg2);
static int32_t script_3F250(int32_t arg0, int32_t arg1, float arg2, int32_t arg3, const void *arg4);
static void script_3F2BF(int32_t arg0, float arg1, const void *arg2, int32_t arg3);
static void script_3F341(int32_t arg0, float arg1);
static void script_3F3C1(int32_t arg0);
static int32_t script_3F40E(void);
static int32_t script_3F4D5(void);
static void script_3F505(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_3F51B(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_3F53F(int32_t arg0, int32_t arg1);
static int32_t script_3F551(int32_t arg0, int32_t arg1);
static void script_3F71C(int32_t arg0, int32_t arg1);
static void script_3F72D(int32_t arg0, int32_t arg1);
static void script_3F84C(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_3F862(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_3F89E(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4);
static int32_t script_3F8B6(int32_t arg0, int32_t arg1, float arg2, int32_t arg3, int32_t arg4);
static void script_3F931(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5, int32_t arg6);
static int32_t script_3F94D(int32_t arg0, int32_t arg1, int32_t arg2, float arg3, int32_t arg4, int32_t arg5, int32_t arg6);
static void script_3F9A3(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4);
static int32_t script_3F9BB(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4);
static void script_3F9E2(void);
static int32_t script_3FA32(int32_t arg0, int32_t arg1);
static int32_t script_3FDEB(int32_t arg0, int32_t arg1);
static void script_404BE(int32_t arg0);
static void script_404EA(void);
static void script_4056D(int32_t arg0);
static void script_40717(int32_t arg0);
static int32_t script_407C5(int32_t arg0, int32_t arg1);
static int32_t script_40968(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_40AED(int32_t arg0, int32_t arg1);
static int32_t script_40B4E(int32_t arg0, int32_t arg1, int32_t arg2);
static int32_t script_40C29(int32_t arg0, int32_t arg1);
static void script_40C82(int32_t arg0);
static int32_t script_40D00(int32_t arg0);
static void script_40D6C(int32_t arg0, int32_t arg1);
static void script_40D83(void);
static void script_40D9D(const char *arg0, const char *arg1);
static void script_40DC0(const char *arg0, const char *arg1);
static void script_40DE4(const char *arg0);
static void script_40E08(int32_t arg0, float arg1, float arg2, float arg3, int32_t arg4, int32_t arg5, int32_t arg6);
static void script_40E57(void);
static void script_40E77(int32_t arg0, int32_t arg1, Vector3 *arg2, Vector3i *arg3, const void *arg4);
static void script_40E9F(int32_t arg0, const char *arg1);
static void script_40EC8(void);
static void script_40EE2(int32_t arg0);
static void script_40F0A(void);
static int32_t script_40F28(void);
static void script_40F5B(void);
static void script_40F68(int32_t arg0);

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
static int8_t task_113;
static int8_t task_114;
static uint8_t task_115;
static uint16_t task_116;
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
static int32_t task_394;
static int32_t task_398;
static int8_t task_39C;
static uint8_t task_39D;
static uint16_t task_39E;
static int32_t task_3A0;
static int32_t task_3A4;
static int32_t task_3A8;
static int32_t task_3AC;
static int32_t task_3B0;
static int8_t task_3B4;
static uint8_t task_3B5;
static uint16_t task_3B6;
static int8_t task_3B8;
static uint8_t task_3B9;
static uint16_t task_3BA;
static int32_t task_3BC;
static int32_t task_3C0;
static int32_t task_3C4;
static int32_t task_3C8;
static GameDateTime task_3CC;
static int32_t task_3D4;
static int32_t task_3D8;
static int32_t task_3DC;
static int32_t task_3E0;
static int32_t task_3E4;
static uint8_t task_3E8;
static int8_t task_3E9;
static int8_t task_3EA;
static uint8_t task_3EB;
static Vector3 task_3EC;
static int8_t task_3F8;
static uint8_t task_3F9;
static uint16_t task_3FA;
static int32_t task_3FC;
static int32_t task_400;
static int8_t task_404;
static uint8_t task_405;
static uint16_t task_406;
static int8_t task_408;
static uint8_t task_409;
static uint16_t task_40A;
static int32_t task_40C;
static int32_t task_410;
static int32_t task_414;
static int32_t task_418;
static int32_t task_41C;
static int32_t task_420;
static int32_t task_424;
static int32_t task_428;
static int32_t task_42C;
static int32_t task_430;
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
static int8_t task_480;
static int8_t task_481;
static int8_t task_482;
static int8_t task_483;
static int8_t task_484;
static uint8_t task_485;
static uint16_t task_486;
static int32_t task_488;
static int32_t task_48C;
static int8_t task_490[100][12];
static int8_t task_940[12];
static int8_t task_94C[10];
static int8_t task_956;
static int8_t task_957;
static int8_t task_958;
static uint8_t task_959;
static uint16_t task_95A;
static int32_t task_95C;
static int32_t task_960;
static int8_t task_964;
static uint8_t task_965;
static uint16_t task_966;
static int32_t task_968[10];
static int32_t task_990;
static int32_t task_994;
static int32_t task_998;
static int32_t task_99C;
static GameDateTime task_9A0;
static int32_t task_9A8;
static int32_t task_9AC;
static int32_t task_9B0;
static int32_t task_9B4;
static int32_t task_9B8;
static int32_t task_9BC;
static int32_t task_9C0[3];
static int32_t task_9CC[3];
static int32_t task_9D8[3];
static int32_t task_9E4[3];
static int32_t task_9F0[3];
static int32_t task_9FC;
static int32_t task_A00[3];
static int32_t task_A0C[3];
static int32_t task_A18[3];
static int32_t task_A24[3];
static int32_t task_A30;
static int32_t task_A34;
static int32_t task_A38;
static int32_t task_A3C;
static int32_t task_A40;
static int32_t task_A44;
static int32_t task_A48[3];
static float task_A54[3][3];
static float task_A78[3];
static float task_A84;
static float task_A88;
static float task_A8C;
static float task_A90[3][3];
static int32_t task_AB4[3][3];
static float task_AD8[3];
static int32_t task_AE4[3];
static int32_t task_AF0;
static int32_t task_AF4;
static int32_t task_AF8;
static int32_t task_AFC;
static int32_t task_B00;
static int32_t task_B04;
static int32_t task_B08;
static int32_t task_B0C;
static int32_t task_B10;
static int32_t task_B14;
static int32_t task_B18[3];
static int32_t task_B24[3];
static int32_t task_B30[3];
static int32_t task_B3C[3];
static int32_t task_B48[3];
static int32_t task_B54;
static int32_t task_B58[3];
static int32_t task_B64[3];
static int32_t task_B70[3];
static int32_t task_B7C[3];
static int32_t task_B88;
static int32_t task_B8C;
static int32_t task_B90;
static int32_t task_B94;
static int32_t task_B98;
static int32_t task_B9C;
static int32_t task_BA0[3];
static float task_BAC[3][3];
static float task_BD0[3];
static float task_BDC;
static float task_BE0;
static float task_BE4;
static float task_BE8[3][3];
static int32_t task_C0C[3][3];
static float task_C30[3];
static int32_t task_C3C[3];
static int32_t task_C48;
static int32_t task_C4C;
static int32_t task_C50[3];
static int32_t task_C5C[3];
static int32_t task_C68[3];
static int32_t task_C74[3];
static float task_C80[3];
static int32_t task_C8C;
static const Vector3 data_40FA8 = {.x = 1.0f, .y = 1.0f, .z = 1.0f};
static const int32_t data_40FB4[4] = {16777215, 16777215, 16777215, 16777215};
static const float data_40FC4[3] = {20.0f, 800.0f, 1.0f};
static const int32_t data_40FD0 = -1;
static const char data_40FD4[10] __attribute__((scn_string_literal)) = "Kaidan : ";
static const char data_40FDE[32] __attribute__((scn_string_literal)) = "========== initializing kaidan\012";
static const char data_40FFE[48] __attribute__((scn_string_literal)) = "please ENDOFKDATA use can't start shuki kaidan\012";
static const char data_4102E[] __attribute__((scn_string_literal)) = "Kaidan Function Error 1\n";
static const char data_41047[] __attribute__((scn_string_literal)) = "Kaidan Function Error 2\n";
static const char data_41060[] __attribute__((scn_string_literal)) = "Kaidan Data Error\n";
static const char data_41073[10] __attribute__((scn_string_literal)) = "Kaidan : ";
static const char data_4107D[28] __attribute__((scn_string_literal)) = "wait ... shuki shuki shuki\012";
static const char data_41099[10] __attribute__((scn_string_literal)) = "Kaidan : ";
static const char data_410A3[24] __attribute__((scn_string_literal)) = "KAIDAN : Warning coli!\012";
static const char data_410BB[24] __attribute__((scn_string_literal)) = "KAIDAN : Warning coli!\012";
static const char data_410D3[24] __attribute__((scn_string_literal)) = "KAIDAN : Warning coli!\012";
static const char data_410EB[24] __attribute__((scn_string_literal)) = "KAIDAN : Warning coli!\012";
static const char data_41103[24] __attribute__((scn_string_literal)) = "KAIDAN : Warning coli!\012";
static const char data_4111B[24] __attribute__((scn_string_literal)) = "KAIDAN : Warning setY!\012";
static const char data_41133[24] __attribute__((scn_string_literal)) = "KAIDAN : Warning setY!\012";
static const char data_4114B[241] __attribute__((scn_string_literal)) = "Kaidan : ";
static const int32_t data_4123C[171] = {
  0, 0, -10922, 0, 0, -10558, 0, 1456, 0, 0, 0, -18022, 0, 0, -17112, 0, 0, -18386, 0, 0, 0, 0, 0, -12196, 0, 0, -18204, 0, 0, -16748, 0, 0, 0, 0, 0,
  -11832, 0, 0, -18386, 0, 0, -15109, 0, 0, 0, 0, 910, -12014, 0, -2184, -16748, 0, -2184, -10012, 0, 0, 0, 0, -364, -11650, -2912, -364, -4369, 1638,
  910, 182, 0, 0, -16019, 0, 182, -23119, 0, 546, -18932, 0, 0, -1092, 0, 0, -16384, 0, 0, -21845, 0, 0, -18750, 0, 0, -546, 0, 0, -21481, 0, 0,
  -21299, 0, 0, -18568, 0, 0, -728, 0, 910, -18750, 0, 182, -18750, 0, -4004, -18204, 0, 0, -728, 0, -364, -14017, -2912, -364, -6735, 1638, 910,
  2548, 0, 0, -18568, 0, 182, -25668, 0, 546, -21481, 0, 0, -3640, 0, 0, -18932, 0, 0, -24393, 0, 0, -21299, 0, 0, -3094, 0, 0, -24029, 0, 0, -23847,
  0, 0, -21117, 0, 0, -3276, 0, 910, -21299, 0, 182, -21299, 0, -4004, -20753, 0, 0, -3276
};
static const int32_t data_414E8[171] = {
  0, 0, -5461, 0, -2366, -1092, 0, -182, 1092, 0, -546, -6735, 0, 0, -4004, 0, 0, -2912, 0, -182, 1274, 0, 0, -13289, 0, 0, -5279, 0, 0, -8009, 0, 0,
  1456, 0, 182, -8738, 0, 0, -6007, 0, 0, -8738, 0, 182, 1638, 0, -728, -5279, 0, -1274, -6735, 0, -910, -8738, 0, 0, 1638, 0, 0, -5461, 0, -2366,
  -1092, 0, -182, 1092, 0, -546, -13289, 0, 0, -5643, 0, 0, -8009, 0, -182, 1274, 0, 0, -11832, 0, 0, -5279, 0, 0, -8009, 0, 0, 1456, 0, 182, -8738,
  0, 0, -6007, 0, 0, -8738, 0, 182, 1638, 0, -728, -5279, 0, -1274, -6735, 0, -910, -8738, 0, 0, 1092, 0, 0, -5461, 0, -2366, -1092, 0, -182, 1092, 0,
  -546, -3640, 0, 0, -2366, 0, 0, -1456, 0, -182, 1274, 0, 0, -3822, 0, 0, -2730, 0, 0, -1456, 0, 0, 1456, 0, 182, -3822, 0, 0, -2912, 0, 0, -1638, 0,
  182, 1638, 0, -728, -3640, 0, -910, -2730, 0, -546, -1638, 0, 0, 1092
};
static const int32_t data_41794[57] = {
  0, 0, -5461, 0, -2366, -1092, 0, -182, 1092, 0, -546, -6735, 0, 0, -4004, 0, 0, -6553, 0, -182, 1274, 0, 0, -13289, 0, 0, -5279, 0, 0, -8009, 0, 0,
  1456, 0, 182, -8738, 0, 0, -6007, 0, 0, -8738, 0, 182, 1638, 0, -728, -5279, 0, -1274, -6735, 0, -910, -8738, 0, 0, 1638
};
static const int32_t data_41878[285] = {
  0, 0, -9102, 0, -2366, -2002, 0, -182, 910, 0, -546, -2366, 0, 0, -2002, 0, 0, -2912, 0, -182, 1274, 0, 0, -14199, 0, 0, -11468, 0, 0, -13653, 0, 0,
  1456, 0, 182, -11286, 0, 0, -12014, 0, 0, -12743, 0, 182, 1638, 0, -728, -10194, 0, -1274, -12196, 0, -910, -10922, -728, 546, 1638, 0, 0, -1820, 0,
  -2366, -728, 0, -364, -1092, 0, -546, -11832, 0, 910, -6735, 0, 0, -6553, 0, -182, 1274, 0, 0, -13289, 0, 0, -6007, 0, 0, -10012, 0, 0, 1092, 0,
  182, -8738, 0, 0, -6735, 0, 0, -9830, 0, 182, 1638, 0, -728, -6553, 0, -1274, -7281, 0, -910, -10740, 0, 0, 1638, 0, 0, -5461, -2912, -2366, -364,
  -5279, 2548, -910, 0, -546, -8374, 0, 0, -9466, 0, 910, -4915, 0, -182, 546, 0, 0, -13289, 0, 0, -5279, 0, 364, -7645, 0, -182, 1456, 0, 182, -8738,
  0, 0, -7281, 0, 182, -6917, 0, -182, 1638, 0, -728, -5279, 0, -1274, -6735, 0, -182, -8009, 0, 0, 1638, -4733, 0, -8556, 0, -2366, -1092, -6189,
  2912, 0, 0, -546, -10922, 0, 1820, -16384, 0, 0, -11468, 0, 182, 546, 0, 0, -10376, 0, 0, -15837, 0, 0, -12743, 0, -182, 728, 0, 182, -13835, 0, 0,
  -16201, 0, 364, -10922, 0, -182, 728, 0, -728, -9648, 0, -3458, -12379, 0, -728, -7463, 0, 0, 1092, 0, 0, -5461, 0, -2366, -1638, 0, -910, 1092, 0,
  -546, -3640, 0, 0, -2366, 0, 0, -1456, 0, -182, 1638, 0, 0, -910, 0, 0, -2730, 0, 0, -1274, 0, 0, 1820, 0, 182, -2730, 0, 0, -1820, 0, 0, -1274, 0,
  182, 2184, 0, -728, -3094, 0, -910, -2548, 0, 910, -1456, -546, 910, 2366
};
static const int32_t data_41CEC[57] = {
  0, 0, -3640, 0, 0, -2366, -4369, 1820, -546, 0, 0, -5279, 0, 0, -6007, 0, 0, -2912, 0, 0, -910, 0, 0, -6553, 0, 0, -6735, 0, 0, -3640, 0, 0, -182,
  0, 0, -5461, 0, 0, -6007, 0, 0, -2366, 0, 0, 0, 0, 0, -5643, 0, 0, -4733, 0, 0, -2366, -1456, 0, 728
};
static const Vector4 data_41DD0 = {.x = 0.3f, .y = 0.3f, .z = 0.3f, .w = 0.3f};
static const uint32_t data_41DE0 = SHEN_NAME('VM_0');
static const shen_name data_41DE4[7] = {
  SHEN_NAME('VM_1'), SHEN_NAME('VM_2'), SHEN_NAME('VM_3'), SHEN_NAME('VM_4'), SHEN_NAME('VM_5'), SHEN_NAME('VM_6'), SHEN_NAME('VM_7')
};
static const char data_41E00[5] __attribute__((scn_string_literal)) = "SY11";
static const char data_41E05[9] __attribute__((scn_string_literal)) = "SY11A006";
static const char data_41E0E[9] __attribute__((scn_string_literal)) = "SY11A007";
static const char data_41E17[9] __attribute__((scn_string_literal)) = "SY11A008";
static const char data_41E20[9] __attribute__((scn_string_literal)) = "SY11A002";
static const char data_41E29[9] __attribute__((scn_string_literal)) = "SY11A003";
static const char data_41E32[9] __attribute__((scn_string_literal)) = "SY11A004";
static const char data_41E3B[9] __attribute__((scn_string_literal)) = "SY11A005";
static const char data_41E44[9] __attribute__((scn_string_literal)) = "SY11A001";
static const char data_41E4D[9] __attribute__((scn_string_literal)) = "SY11A009";
static const char data_41E56[9] __attribute__((scn_string_literal)) = "SY11A010";
static const char data_41E5F[9] __attribute__((scn_string_literal)) = "SY11A011";
static const char data_41E68[9] __attribute__((scn_string_literal)) = "SY11A009";
static const char data_41E71[5] __attribute__((scn_string_literal)) = "SY11";
static const char data_41E76[5] __attribute__((scn_string_literal)) = "SY11";
static const char data_41E7B[9] __attribute__((scn_string_literal)) = "SY11A001";
static const char data_41E84[5] __attribute__((scn_string_literal)) = "SY11";
static const char data_41E89[5] __attribute__((scn_string_literal)) = "SY11";
static const char data_41E8E[9] __attribute__((scn_string_literal)) = "SY11A009";
static const char data_41E97[9] __attribute__((scn_string_literal)) = "SY11A010";
static const char data_41EA0[9] __attribute__((scn_string_literal)) = "SY11A011";
static const char data_41EA9[5] __attribute__((scn_string_literal)) = "SY11";
static const char data_41EAE[18] __attribute__((scn_string_literal)) = "/scene/01/stream/";
static const char data_41EC0[5] __attribute__((scn_string_literal)) = "%03d";
static const char data_41EC5[] __attribute__((scn_string_literal)) = "ATRK";
static const uint8_t data_41ECA[2] = {0};
static Vector3 data_41ECC = {.x = 0.0f, .y = -0.1f, .z = 0.0f};
static Vector3 data_41ED8 = {.x = 0.0f, .y = 0.0f, .z = 0.0f};
static const char data_41EE4[] __attribute__((scn_string_literal)) = "VEND";
static const char data_41EE9[] __attribute__((scn_string_literal)) = "VEND";
static const char data_41EEE[] __attribute__((scn_string_literal)) = "SPRT";
static const char data_41EF3[7] __attribute__((scn_string_literal)) = "01JUCE";
static const char data_41EFA[13] __attribute__((scn_string_literal)) = "HMOT_CAN.BIN";
static const char data_41F07[] __attribute__((scn_string_literal)) = "VEND";
static const char data_41F0C[7] __attribute__((scn_string_literal)) = "01JUCE";
static const char data_41F13[26] __attribute__((scn_string_literal)) = "HandMot read by current!\012";
static const char data_41F2D[13] __attribute__((scn_string_literal)) = "HMOT_CAN.BIN";
static const char data_41F3A[7] __attribute__((scn_string_literal)) = "01JUCE";
static const char data_41F41[] __attribute__((scn_string_literal)) = "VEND";
static const uint8_t data_41F46[2] = {0};
static const uint8_t data_41F48[12] = {0};
static const char data_41F54[29] __attribute__((scn_string_literal)) = "vendmachine exit is EV_STOP\012";
static const char data_41F71[41] __attribute__((scn_string_literal)) = "vendmachine exit is MONEY_LOCK -> %d,%d\012";
static const char data_41F9A[] __attribute__((scn_string_literal)) = "COKE";
static const char data_41F9F[] __attribute__((scn_string_literal)) = "FATG";
static const char data_41FA4[] __attribute__((scn_string_literal)) = "FATO";
static const char data_41FA9[] __attribute__((scn_string_literal)) = "SPRT";
static const char data_41FAE[] __attribute__((scn_string_literal)) = "CAFE";
static const uint8_t data_41FB3[1] = {0};
static const uint32_t data_41FB4 = 0x3D204C06;
static const float data_41FB8[2] = {0.0f, 0.890018f};
static const uint8_t data_41FC0[12] = {0};
static const uint8_t data_41FCC[12] = {0};
static const uint32_t data_41FD8 = 0x3DE978D5;
static const float data_41FDC[2] = {0.002f, -0.036f};
static const uint32_t data_41FE4 = 8192;
static const int32_t data_41FE8[2] = {11377, 3640};
static const uint32_t data_41FF0 = 0x3D204C06;
static const float data_41FF4[2] = {0.0f, 0.890018f};
static const uint8_t data_41FFC[12] = {0};
static const uint32_t data_42008 = 4369;
static const int32_t data_4200C[2] = {0, -2184};
static const char data_42014[] __attribute__((scn_string_literal)) = "01JUCEA011";
static const char data_4201F[] __attribute__((scn_string_literal)) = "01JUCEA012";
static const char data_4202A[] __attribute__((scn_string_literal)) = "01JUCEA013";
static const char data_42035[] __attribute__((scn_string_literal)) = "01JUCEA002";
static const char data_42040[] __attribute__((scn_string_literal)) = "01JUCEA003";
static const uint8_t data_4204B[1] = {0};
static const uint32_t data_4204C = 0x3DE978D5;
static const float data_42050[2] = {0.002f, -0.036f};
static const uint32_t data_42058 = 8192;
static const int32_t data_4205C[2] = {11377, 3640};
static const uint32_t data_42064 = 0x3DCB04AB;
static const float data_42068[2] = {0.02044f, -0.04322f};
static const uint32_t data_42070 = 0xFFFFFE69;
static const int32_t data_42074[2] = {3331, -3464};
static const uint32_t data_4207C = 0x3D204C06;
static const float data_42080[2] = {0.0f, 0.890018f};
static const uint8_t data_42088[12] = {0};
static const uint32_t data_42094 = 4369;
static const int32_t data_42098[2] = {0, -2184};
static const uint8_t data_420A0[12] = {0};
static const char data_420AC[] __attribute__((scn_string_literal)) = "01JUCEA002";
static const char data_420B7[] __attribute__((scn_string_literal)) = "01JUCEA003";
static const uint8_t pad_420C2[4] = {0};
static const uint8_t data_420C6[10] = {
  0x80, 0x3F, 0x00, 0x00, 0x80, 0x3F, 0x00, 0x00, 0x80, 0x3F
};
static const uint32_t data_420D0 = 0xC2480000;
static const float data_420D4[2] = {0.0f, 10.0f};
static const uint32_t data_420DC = 0x41A00000;
static const float data_420E0[2] = {0.0f, 110.0f};
static const uint32_t data_420E8 = 0xC2041FF3;
static const float data_420EC[2] = {0.0f, 52.1008f};
static const uint32_t data_420F4 = 0x41B3AA99;
static const uint32_t data_420F8[2] = {0, 0x420947FDU};
static const uint32_t data_42100 = 0x42201A1D;
static const float data_42104[2] = {0.0f, -2.1877f};
static const uint32_t data_4210C = 0xC2BD9F2E;
static const uint32_t data_42110[2] = {0, 0x428A24A9U};
static const uint32_t data_42118 = 0x41D6BFB1;
static const uint32_t data_4211C[2] = {0, 0x42CFC5AFU};
static const uint32_t data_42124 = 0xC1A00000;
static const float data_42128[2] = {0.0f, 110.0f};
static const uint32_t data_42130 = 0xC19181A3;
static const uint32_t data_42134[2] = {0, 0x421B20DFU};
static const uint32_t data_4213C = 0xC2C00000;
static const float data_42140[2] = {0.0f, 78.0f};
static const uint32_t data_42148 = 0xC2880000;
static const float data_4214C[2] = {0.0f, 53.0f};
static const uint32_t data_42154 = 0xC1A00000;
static const float data_42158[2] = {0.0f, 110.0f};
static const uint32_t data_42160 = 0xC2041FF3;
static const float data_42164[2] = {0.0f, 52.1008f};
static const uint32_t data_4216C = 0x3F4978D5;
static const float data_42170[33] = {
  0.787f, 0.866f, 0.944f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.944f, 0.866f, 0.787f,
  0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f
};
static const uint8_t data_421F4[80] = {0};
typedef struct Row_42244 { float w0; float w1; float w2; int32_t w3; } Row_42244;
static const Row_42244 data_42244[3] = {
  {1.0f, 1.0f, 1.0f, 0},
  {0.0f, 0.0f, 0.0f, 0},
  {0.0f, 0.0f, 0.0f, 0}
};
static const int32_t data_42274[2] = {0, 0};
static const uint8_t data_4227C[24] = {0};
static const float data_42294[28] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f
};
static const uint8_t data_42304[24] = {0};
static const float data_4231C[28] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f
};
static const uint32_t data_4238C = 0x3F4978D5;
static const float data_42390[33] = {
  0.787f, 0.866f, 0.944f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  0.944f, 0.866f, 0.866f, 0.866f, 0.866f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f
};
static const uint8_t data_42414[24] = {0};
static const float data_4242C[28] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f
};
static const uint32_t data_4249C = 0x3F800000;
static const float data_424A0[33] = {
  0.944f, 0.866f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f,
  0.787f, 0.787f, 0.551f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint8_t data_42524[104] = {0};
static const float data_4258C[8] = {1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f};
static const uint8_t data_425AC[12] = {0};
static const float data_425B8[31] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint32_t data_42634 = 0x3F266666;
static const float data_42638[33] = {
  0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f,
  0.708f, 0.787f, 0.866f, 0.944f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint32_t data_426BC = 0x3F800000;
static const float data_426C0[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint32_t data_42744 = 0x3F800000;
static const float data_42748[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint32_t data_427CC = 0x3F800000;
static const float data_427D0[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint32_t data_42854 = 0xC2480000;
static const float data_42858[2] = {0.0f, 10.0f};
static const uint32_t data_42860 = 0x4214CCCD;
static const float data_42864[2] = {0.0f, 6.6f};
static const uint32_t data_4286C = 0x41B5999A;
static const float data_42870[2] = {0.0f, 27.2f};
static const uint32_t data_42878 = 0x41B3999A;
static const float data_4287C[2] = {0.0f, 34.32f};
static const uint32_t data_42884 = 0x4220147B;
static const float data_42888[2] = {0.0f, -2.18f};
static const uint32_t data_42890 = 0xC2BD9EB8;
static const float data_42894[2] = {0.0f, 69.07f};
static const uint32_t data_4289C = 0x41D6B852;
static const float data_428A0[2] = {0.0f, 103.88f};
static const uint32_t data_428A8 = 0xC0966666;
static const float data_428AC[2] = {0.0f, 30.6f};
static const uint32_t data_428B4 = 0xC19170A4;
static const float data_428B8[2] = {0.0f, 38.78f};
static const uint32_t data_428C0 = 0xC2C00000;
static const float data_428C4[2] = {0.0f, 78.0f};
static const uint32_t data_428CC = 0xC2840000;
static const float data_428D0[2] = {0.0f, 54.0f};
static const uint32_t data_428D8 = 0xC1FB0A3D;
static const float data_428DC[2] = {0.0f, 40.1f};
static const uint32_t data_428E4 = 0xC2C28F5C;
static const float data_428E8[2] = {0.0f, 64.48f};
static const uint32_t data_428F0 = 0xC2F2FAE1;
static const float data_428F4[2] = {0.0f, 94.2f};
static const uint32_t data_428FC = 0xC2BC75C3;
static const float data_42900[2] = {0.0f, 85.91f};
static const uint32_t data_42908 = 0x3F4978D5;
static const float data_4290C[33] = {
  0.787f, 0.866f, 0.944f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.944f, 0.866f, 0.787f,
  0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f
};
static const uint8_t data_42990[16] = {0};
static const float data_429A0[30] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint8_t data_42A18[16] = {0};
static const float data_42A28[30] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint8_t data_42AA0[24] = {0};
static const float data_42AB8[28] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f
};
static const uint32_t data_42B28 = 0x3F4978D5;
static const float data_42B2C[33] = {
  0.787f, 0.866f, 0.944f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  0.944f, 0.866f, 0.866f, 0.866f, 0.866f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f
};
static const uint8_t data_42BB0[24] = {0};
static const float data_42BC8[28] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f
};
static const uint32_t data_42C38 = 0x3F800000;
static const float data_42C3C[33] = {
  0.944f, 0.866f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f,
  0.787f, 0.787f, 0.551f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint8_t data_42CC0[16] = {0};
static const float data_42CD0[30] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint8_t data_42D48[8] = {0};
static const float data_42D50[32] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint32_t data_42DD0 = 0x3F266666;
static const float data_42DD4[33] = {
  0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f,
  0.708f, 0.787f, 0.866f, 0.944f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint32_t data_42E58 = 0x3F800000;
static const float data_42E5C[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint8_t data_42EE0[16] = {0};
static const float data_42EF0[30] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint8_t data_42F68[80] = {0};
static const float data_42FB8[14] = {1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f};
static const uint8_t data_42FF0[80] = {0};
static const float data_43040[14] = {1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f};
static const uint8_t data_43078[80] = {0};
static const float data_430C8[14] = {1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f};
static const uint32_t data_43100 = 0xC2480000;
static const float data_43104[2] = {0.0f, 10.0f};
static const uint32_t data_4310C = 0x41E80000;
static const shen_name data_43110[2] = {0, SHEN_NAME('333A')};
static const uint32_t data_43118 = 0xC2041FF3;
static const float data_4311C[2] = {0.0f, 52.1008f};
static const uint32_t data_43124 = 0x41B3AA99;
static const uint32_t data_43128[2] = {0, 0x420947FDU};
static const uint32_t data_43130 = 0x42201A1D;
static const float data_43134[2] = {0.0f, -2.1877f};
static const uint32_t data_4313C = 0xC2BD9F2E;
static const uint32_t data_43140[2] = {0, 0x428A24A9U};
static const uint32_t data_43148 = 0x41D6BFB1;
static const uint32_t data_4314C[2] = {0, 0x42CFC5AFU};
static const uint32_t data_43154 = 0xC1A00000;
static const float data_43158[2] = {0.0f, 110.0f};
static const uint32_t data_43160 = 0xC19181A3;
static const uint32_t data_43164[2] = {0, 0x421B20DFU};
static const uint32_t data_4316C = 0xC2C00000;
static const float data_43170[2] = {0.0f, 78.0f};
static const uint32_t data_43178 = 0xC2880000;
static const float data_4317C[2] = {0.0f, 53.0f};
static const uint32_t data_43184 = 0xC0B9999A;
static const float data_43188[2] = {0.0f, 26.2f};
static const uint32_t data_43190 = 0xC22E0000;
static const float data_43194[2] = {0.0f, 46.0f};
static const uint32_t data_4319C = 0xC2CE0000;
static const float data_431A0[2] = {0.0f, 80.0f};
static const uint32_t data_431A8 = 0xC2041FF3;
static const float data_431AC[2] = {0.0f, 52.1008f};
static const uint32_t data_431B4 = 0x3F4978D5;
static const float data_431B8[33] = {
  0.787f, 0.866f, 0.944f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.944f, 0.866f, 0.787f,
  0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f
};
static const uint8_t data_4323C[16] = {0};
static const float data_4324C[30] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint8_t data_432C4[24] = {0};
static const float data_432DC[28] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f
};
static const uint8_t data_4334C[24] = {0};
static const float data_43364[28] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f
};
static const uint32_t data_433D4 = 0x3F4978D5;
static const float data_433D8[33] = {
  0.787f, 0.866f, 0.944f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  0.944f, 0.866f, 0.866f, 0.866f, 0.866f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f
};
static const uint8_t data_4345C[24] = {0};
static const float data_43474[28] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f
};
static const uint32_t data_434E4 = 0x3F800000;
static const float data_434E8[33] = {
  0.944f, 0.866f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f,
  0.787f, 0.787f, 0.551f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint8_t data_4356C[104] = {0};
static const float data_435D4[8] = {1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f};
static const uint8_t data_435F4[8] = {0};
static const float data_435FC[32] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint32_t data_4367C = 0x3F266666;
static const float data_43680[33] = {
  0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f,
  0.708f, 0.787f, 0.866f, 0.944f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint32_t data_43704 = 0x3F800000;
static const float data_43708[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint8_t data_4378C[16] = {0};
static const float data_4379C[30] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint8_t data_43814[16] = {0};
static const float data_43824[30] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint8_t data_4389C[80] = {0};
static const float data_438EC[14] = {1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f};
static const uint32_t data_43924 = 0x3F800000;
static const float data_43928[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint32_t data_439AC = 0xC2480000;
static const float data_439B0[2] = {0.0f, 10.0f};
static const uint32_t data_439B8 = 0x41A00000;
static const float data_439BC[2] = {0.0f, 110.0f};
static const uint32_t data_439C4 = 0xC2041FF3;
static const float data_439C8[2] = {0.0f, 52.1008f};
static const uint32_t data_439D0 = 0x41B3AA99;
static const uint32_t data_439D4[2] = {0, 0x420947FDU};
static const uint32_t data_439DC = 0x42201A1D;
static const float data_439E0[2] = {0.0f, -2.1877f};
static const uint32_t data_439E8 = 0xC2BD9F2E;
static const uint32_t data_439EC[2] = {0, 0x428A24A9U};
static const uint32_t data_439F4 = 0x41D6BFB1;
static const uint32_t data_439F8[2] = {0, 0x42CFC5AFU};
static const uint32_t data_43A00 = 0xC1A00000;
static const float data_43A04[2] = {0.0f, 110.0f};
static const uint32_t data_43A0C = 0xC19181A3;
static const uint32_t data_43A10[2] = {0, 0x421B20DFU};
static const uint32_t data_43A18 = 0xC2C00000;
static const float data_43A1C[2] = {0.0f, 78.0f};
static const uint32_t data_43A24 = 0xC2880000;
static const float data_43A28[2] = {0.0f, 53.0f};
static const uint32_t data_43A30 = 0x40A66666;
static const float data_43A34[2] = {0.0f, 27.0f};
static const uint32_t data_43A3C = 0xC201999A;
static const float data_43A40[2] = {0.0f, 44.5f};
static const uint32_t data_43A48 = 0xC2CE0000;
static const float data_43A4C[2] = {0.0f, 80.0f};
static const uint32_t data_43A54 = 0xC2041FF3;
static const float data_43A58[2] = {0.0f, 52.1008f};
static const uint32_t data_43A60 = 0x3F4978D5;
static const float data_43A64[33] = {
  0.787f, 0.866f, 0.944f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.944f, 0.866f, 0.787f,
  0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f
};
static const uint8_t data_43AE8[80] = {0};
typedef struct Row_43B38 { float w0; float w1; float w2; int32_t w3; } Row_43B38;
static const Row_43B38 data_43B38[3] = {
  {1.0f, 1.0f, 1.0f, 0},
  {0.0f, 0.0f, 0.0f, 0},
  {0.0f, 0.0f, 0.0f, 0}
};
static const int32_t data_43B68[2] = {0, 0};
static const uint8_t data_43B70[24] = {0};
static const float data_43B88[28] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f
};
static const uint8_t data_43BF8[24] = {0};
static const float data_43C10[28] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f
};
static const uint32_t data_43C80 = 0x3F4978D5;
static const float data_43C84[33] = {
  0.787f, 0.866f, 0.944f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  0.944f, 0.866f, 0.866f, 0.866f, 0.866f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f
};
static const uint8_t data_43D08[24] = {0};
static const float data_43D20[28] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f
};
static const uint32_t data_43D90 = 0x3F800000;
static const float data_43D94[33] = {
  0.944f, 0.866f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f,
  0.787f, 0.787f, 0.551f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint8_t data_43E18[104] = {0};
static const float data_43E80[8] = {1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f};
static const uint8_t data_43EA0[8] = {0};
static const float data_43EA8[32] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint32_t data_43F28 = 0x3F266666;
static const float data_43F2C[33] = {
  0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f,
  0.708f, 0.787f, 0.866f, 0.944f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint32_t data_43FB0 = 0x3F800000;
static const float data_43FB4[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint8_t data_44038[16] = {0};
static const float data_44048[30] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint8_t data_440C0[16] = {0};
static const float data_440D0[30] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint8_t data_44148[80] = {0};
static const float data_44198[14] = {1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f};
static const uint32_t data_441D0 = 0x3F800000;
static const float data_441D4[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const int32_t data_44258[57] = {
  0, 0, -10977, 0, 0, -4842, 0, 0, 0, 0, 0, -4531, 0, 0, -1820, 0, 0, -3877, 0, 0, 0, 0, 0, -8574, 0, 0, -16074, 0, 0, -4642, 0, 0, 0, 0, 0, -8610, 0,
  182, -16966, 0, 0, -6407, 0, 36, -54, 0, 0, -11450, 0, 0, -13562, 0, 0, -8974, 0, -819, -345
};
static const int32_t data_4433C[57] = {
  0, 0, -6005, 0, 0, -909, 0, 0, 0, 0, 0, -6368, 0, 1820, -8552, 0, 0, -4548, 0, 0, 0, 0, 0, -8552, 0, 0, -10372, 0, -545, -4002, 0, 0, 0, 0, 0,
  -7823, 0, 0, -12374, 0, -909, -2182, 0, 0, 0, 0, -1636, -10735, 0, -2001, -10372, 0, -1819, -727, 0, 0, 0
};
static const Vector4 data_44420 = {.x = 1.0f, .y = 0.3f, .z = 0.3f, .w = 0.3f};
static const Vector3 data_44430 = {.x = 38.2248f, .y = -10.0f, .z = 10.056f};
static const FogColourData data_4443C = {.red = 20.0f, .green = 800.0f, .blue = 1.0f, .colour = 0xFF000020U};
static const Vector3 data_4444C = {.x = -27.532101f, .y = 9.2975f, .z = 49.022202f};
static const Vector3 data_44458 = {.x = -77.909798f, .y = 7.1224f, .z = 64.488998f};
static const Vector3 data_44464 = {.x = 29.400999f, .y = 3.7988f, .z = 23.5072f};
static const Vector3 data_44470 = {.x = 23.9245f, .y = 3.7983f, .z = 21.183201f};
static const Vector3 data_4447C = {.x = -72.2313f, .y = 3.7998f, .z = 60.132702f};
static const Vector3 data_44488 = {.x = -7.4819f, .y = 3.7965f, .z = 32.121201f};
static const Vector3i data_44494 = {.x = 0, .y = 32768, .z = 0};
static const Vector3i data_444A0 = {.x = 0, .y = -32768, .z = 0};
static const Vector3i data_444AC = {.x = 0, .y = -21353, .z = 0};
static const Vector3i data_444B8 = {.x = 0, .y = 28061, .z = 0};
static const Vector3i data_444C4 = {.x = 0, .y = 19660, .z = 0};
static const Vector3i data_444D0 = {.x = 0, .y = -27925, .z = 0};
typedef struct Row_444DC { uint32_t w0; float w1; float w2; } Row_444DC;
static const Row_444DC data_444DC[2] = {
  {0x42189B2DU, 0.90358f, 10.08923f},
  {0x4218DC1CU, 0.90821f, 10.06523f}
};
static const Vector3 data_444F4 = {.x = 0.0f, .y = -10.0f, .z = 100.0f};
static const int32_t data_44500[6] = {0, -24386, 0, 0, -19859, 0};
static const Vector3 data_44518 = {.x = 38.161098f, .y = 0.972f, .z = 10.1892f};
static const Vector3i data_44524 = {.x = 0, .y = 11666, .z = 0};
static const Vector3 data_44530 = {.x = 38.2248f, .y = 0.9712f, .z = 10.056f};
static const Vector3i data_4453C = {.x = 0, .y = 44220, .z = 0};
static const Vector3i data_44548 = {.x = 10740, .y = -910, .z = -10012};
static const Vector3i data_44554 = {.x = -4915, .y = -1820, .z = 3640};
static const uint16_t data_44560[8] = {1068, 1043, 1069, 1071, 1074, 1072, 1073, 903};
static const char data_44570[20] __attribute__((scn_string_literal)) = "TUNED FreeTalk def\012";
static const char data_44584[20] __attribute__((scn_string_literal)) = "TUNED FreeTalk def\012";
static DoorMotionSelectorBlock data_44598[38] = {
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
static ScnDoorMotionDescriptor data_44B88[72] = {
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
static uint32_t data_455A8 = 0xFFFFFFFF;
typedef struct Row_455AC { float w0; float w1; float w2; float w3; int32_t w4; } Row_455AC;
static const Row_455AC data_455AC[7] = {
  {0.0f, 0.0f, 0.0f, 0.0f, -1},
  {0.0f, 0.0f, 0.0f, 0.0f, -1},
  {0.0f, 0.0f, 0.0f, 0.0f, 1534},
  {26.0f, 55.0f, 45.0f, 75.0f, 1555},
  {61.0f, 146.0f, 45.0f, 50.0f, 1554},
  {59.0f, 124.0f, 45.0f, 70.0f, 1543},
  {33.0f, 49.0f, 45.0f, 50.0f, 1543}
};
static const float data_45638[4] = {33.0f, 49.0f, 50.0f, 50.0f};
static void (*data_45648)(int32_t, int32_t, int32_t) = script_823B;
static void (*const data_4564C)(int32_t, int32_t, int32_t, int32_t) = script_824B;
static void (*const data_45650)(int32_t, int32_t, int32_t, int32_t) = script_824B;
static void (*const data_45654)(int32_t, int32_t, int32_t, int32_t) = script_824B;
static void (*const data_45658)(int32_t, int32_t, int32_t, int32_t) = script_824B;
static void (*const data_4565C)(int32_t, int32_t, int32_t, int32_t) = script_824B;
static void (*const data_45660)(int32_t, int32_t, int32_t, int32_t) = script_824B;
static void (*const data_45664)(int32_t, int32_t, int32_t, int32_t) = script_824B;
static void (*const data_45668)(int32_t, int32_t, int32_t, int32_t) = script_824B;
static void (*const data_4566C)(int32_t, int32_t, int32_t, int32_t) = script_84F1;
static void (*const data_45670)(int32_t, int32_t, int32_t, int32_t) = script_8652;
static void (*const data_45674)(int32_t, int32_t, int32_t, int32_t) = script_8B1F;
static void (*const data_45678)(int32_t, int32_t, int32_t, int32_t) = script_8B86;
static void (*const data_4567C)(int32_t, int32_t, int32_t, int32_t) = script_834F;
static void (*const data_45680)(int32_t, int32_t, int32_t, int32_t) = script_834F;
static void (*const data_45684)(int32_t, int32_t, int32_t, int32_t) = script_87B3;
static void (*const data_45688)(int32_t, int32_t, int32_t, int32_t) = script_8A51;
static void (*const data_4568C)(int32_t, int32_t, int32_t, int32_t) = script_8BED;
static void (*const data_45690)(int32_t, int32_t, int32_t, int32_t) = script_84F1;
static void (*const data_45694)(int32_t, int32_t, int32_t, int32_t) = script_8652;
static void (*const data_45698)(int32_t, int32_t, int32_t, int32_t) = script_87B3;
static void (*const data_4569C)(int32_t, int32_t, int32_t, int32_t) = script_84F1;
static void (*const data_456A0)(int32_t, int32_t, int32_t, int32_t) = script_8652;
static void (*const data_456A4)(int32_t, int32_t, int32_t, int32_t) = script_87B3;
static void (*const data_456A8)(int32_t, int32_t, int32_t, int32_t) = script_8C34;
static void (*const data_456AC)(int32_t, int32_t, int32_t, int32_t) = script_8C34;
static void (*const data_456B0)(int32_t, int32_t, int32_t, int32_t) = script_8C34;
static void (*const data_456B4)(int32_t, int32_t, int32_t, int32_t) = script_8C34;
static void (*const data_456B8)(int32_t, int32_t, int32_t, int32_t) = script_8D6A;
static void (*const data_456BC)(int32_t, int32_t, int32_t, int32_t) = script_962B;
static void (*const data_456C0)(int32_t, int32_t, int32_t, int32_t) = script_824B;
static void (*const data_456C4)(int32_t, int32_t, int32_t, int32_t) = script_824B;
static void (*const data_456C8)(int32_t, int32_t, int32_t, int32_t) = script_824B;
static void (*const data_456CC)(int32_t, int32_t, int32_t, int32_t) = script_824B;
static void (*const data_456D0)(int32_t, int32_t, int32_t, int32_t) = script_824B;
static void (*const data_456D4)(int32_t, int32_t, int32_t, int32_t) = script_824B;
static void (*const data_456D8)(int32_t, int32_t, int32_t, int32_t) = script_824B;
static void (*const data_456DC)(int32_t, int32_t, int32_t, int32_t) = script_824B;
static const char data_456E0[46] __attribute__((scn_string_literal)) = "Door Index = %d : call_type = %d : type = %d\012";
static const char data_4570E[43] __attribute__((scn_string_literal)) = "Door Open Motion = DEB_AKESIME_FUSUMA_L_F\012";
static const char data_45739[43] __attribute__((scn_string_literal)) = "Door Open Motion = DEB_AKESIME_FUSUMA_R_F\012";
static const char data_45764[42] __attribute__((scn_string_literal)) = "Door Open Motion = DEB_HIKISIME_DOOR_L_F\012";
static const char data_4578E[42] __attribute__((scn_string_literal)) = "Door Open Motion = DEB_HIKISIME_DOOR_R_F\012";
static const char data_457B8[41] __attribute__((scn_string_literal)) = "Door Open Motion = DEB_OSISIME_DOOR_L_F\012";
static const char data_457E1[41] __attribute__((scn_string_literal)) = "Door Open Motion = DEB_OSISIME_DOOR_R_F\012";
static const char data_4580A[43] __attribute__((scn_string_literal)) = "Door Open Motion = GAK_AKESIME_FUSUMA_L_F\012";
static const char data_45835[43] __attribute__((scn_string_literal)) = "Door Open Motion = GAK_AKESIME_FUSUMA_R_F\012";
static const char data_45860[42] __attribute__((scn_string_literal)) = "Door Open Motion = GAK_HIKISIME_DOOR_L_F\012";
static const char data_4588A[42] __attribute__((scn_string_literal)) = "Door Open Motion = GAK_HIKISIME_DOOR_R_F\012";
static const char data_458B4[41] __attribute__((scn_string_literal)) = "Door Open Motion = GAK_OSISIME_DOOR_L_F\012";
static const char data_458DD[41] __attribute__((scn_string_literal)) = "Door Open Motion = GAK_OSISIME_DOOR_R_F\012";
static const char data_45906[43] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_AKESIME_FUSUMA_L_F\012";
static const char data_45931[43] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_AKESIME_FUSUMA_R_F\012";
static const char data_4595C[42] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_HIKISIME_DOOR_L_F\012";
static const char data_45986[42] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_HIKISIME_DOOR_R_F\012";
static const char data_459B0[41] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_OSISIME_DOOR_L_F\012";
static const char data_459D9[41] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_OSISIME_DOOR_R_F\012";
static const char data_45A02[43] __attribute__((scn_string_literal)) = "Door Open Motion = KOD_AKESIME_FUSUMA_L_F\012";
static const char data_45A2D[43] __attribute__((scn_string_literal)) = "Door Open Motion = KOD_AKESIME_FUSUMA_R_F\012";
static const char data_45A58[42] __attribute__((scn_string_literal)) = "Door Open Motion = KOD_HIKISIME_DOOR_L_F\012";
static const char data_45A82[42] __attribute__((scn_string_literal)) = "Door Open Motion = KOD_HIKISIME_DOOR_R_F\012";
static const char data_45AAC[41] __attribute__((scn_string_literal)) = "Door Open Motion = KOD_OSISIME_DOOR_L_F\012";
static const char data_45AD5[41] __attribute__((scn_string_literal)) = "Door Open Motion = KOD_OSISIME_DOOR_R_F\012";
static const char data_45AFE[43] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_AKESIME_FUSUMA_L_F\012";
static const char data_45B29[43] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_AKESIME_FUSUMA_R_F\012";
static const char data_45B54[42] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_HIKISIME_DOOR_L_F\012";
static const char data_45B7E[42] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_HIKISIME_DOOR_R_F\012";
static const char data_45BA8[41] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_OSISIME_DOOR_L_F\012";
static const char data_45BD1[41] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_OSISIME_DOOR_R_F\012";
static const char data_45BFA[43] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_AKESIME_FUSUMA_L_F\012";
static const char data_45C25[43] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_AKESIME_FUSUMA_R_F\012";
static const char data_45C50[42] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_HIKISIME_DOOR_L_F\012";
static const char data_45C7A[42] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_HIKISIME_DOOR_R_F\012";
static const char data_45CA4[41] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_OSISIME_DOOR_L_F\012";
static const char data_45CCD[41] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_OSISIME_DOOR_R_F\012";
static const char data_45CF6[43] __attribute__((scn_string_literal)) = "Door Open Motion = SIN_AKESIME_FUSUMA_L_F\012";
static const char data_45D21[43] __attribute__((scn_string_literal)) = "Door Open Motion = SIN_AKESIME_FUSUMA_R_F\012";
static const char data_45D4C[42] __attribute__((scn_string_literal)) = "Door Open Motion = SIN_HIKISIME_DOOR_L_F\012";
static const char data_45D76[42] __attribute__((scn_string_literal)) = "Door Open Motion = SIN_HIKISIME_DOOR_R_F\012";
static const char data_45DA0[41] __attribute__((scn_string_literal)) = "Door Open Motion = SIN_OSISIME_DOOR_L_F\012";
static const char data_45DC9[41] __attribute__((scn_string_literal)) = "Door Open Motion = SIN_OSISIME_DOOR_R_F\012";
static const char data_45DF2[43] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_AKESIME_FUSUMA_L_F\012";
static const char data_45E1D[43] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_AKESIME_FUSUMA_R_F\012";
static const char data_45E48[42] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_HIKISIME_DOOR_L_F\012";
static const char data_45E72[42] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_HIKISIME_DOOR_R_F\012";
static const char data_45E9C[41] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_OSISIME_DOOR_L_F\012";
static const char data_45EC5[41] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_OSISIME_DOOR_R_F\012";
static const char data_45EEE[43] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_AKESIME_FUSUMA_L_F\012";
static const char data_45F19[43] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_AKESIME_FUSUMA_R_F\012";
static const char data_45F44[42] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_HIKISIME_DOOR_L_F\012";
static const char data_45F6E[42] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_HIKISIME_DOOR_R_F\012";
static const char data_45F98[41] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_OSISIME_DOOR_L_F\012";
static const char data_45FC1[41] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_OSISIME_DOOR_R_F\012";
static const char data_45FEA[44] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_OSISIME_DOOR_AP_L_F\012";
static const char data_46016[44] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_OSISIME_DOOR_AP_L_F\012";
static const char data_46042[44] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_OSISIME_DOOR_AP_L_F\012";
static const char data_4606E[44] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_OSISIME_DOOR_AP_L_F\012";
static const char data_4609A[44] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_OSISIME_DOOR_AP_L_F\012";
static const char data_460C6[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_460E2[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_460FE[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_4611A[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_46136[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_46152[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_4616E[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_4618A[13] __attribute__((scn_string_literal)) = "open-move!!\012";
static const char data_46197[11] __attribute__((scn_string_literal)) = "M_MDOR.BIN";
static const char data_461A2[] __attribute__((scn_string_literal)) = "This Charactor Work is NULL!!\n";
static const char data_461C1[22] __attribute__((scn_string_literal)) = "手動ドア開け番号指定\012";
static const char data_461D7[34] __attribute__((scn_string_literal)) = "無限ループになりそうなので抜ける\012";
static const char data_461F9[] __attribute__((scn_string_literal)) = "Don't make BOX Buffer!!\n";
static const char data_46212[] __attribute__((scn_string_literal)) = "Don't make BOX Buffer!!\n";
static const char data_4622B[15] __attribute__((scn_string_literal)) = "Door System : ";
static const char data_4623A[15] __attribute__((scn_string_literal)) = "Door System : ";
static const char data_46249[18] __attribute__((scn_string_literal)) = "Door Box is ON!!\012";
static const char data_4625B[19] __attribute__((scn_string_literal)) = "Door Box is OFF!!\012";
static const char data_4626E[18] __attribute__((scn_string_literal)) = "Door Box is ON!!\012";
static const char data_46280[20] __attribute__((scn_string_literal)) = "Door Box is OFF!!\012";
static const DSWKDoorEntry data_46294[65] = {
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
static const int32_t data_46FC8[13] = {-1, 0, 6553700, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1};
static const uint32_t data_46FFC = 98;
static const int32_t data_47000[184] = {
  99, -1, 78, 79, -1, 90, -1, 40, 41, -1, 60, 61, -1, 62, 63, -1, 64, 65, -1, 66, 67, -1, 68, 69, -1, 70, 71, -1, 72, 73, -1, 74, 75, -1, 42, 43, -1,
  44, 45, -1, 46, 47, -1, 48, 49, -1, 50, 51, -1, 86, 87, -1, 52, 53, -1, 54, 55, -1, 80, 81, -1, 56, 57, -1, 84, 85, -1, 58, 59, -1, 82, 83, -1, 21,
  22, -1, 19, 20, -1, 97, -1, 9, 10, -1, 91, -1, 5, 6, -1, 36, 37, -1, 33, 34, -1, 76, 77, -1, 116, 117, -1, 35, -1, 4, -1, 7, 8, -1, 32, -1, 106,
  107, -1, 30, 31, -1, 2, 3, -1, 92, 93, -1, 94, -1, 17, 18, -1, 15, 16, -1, 13, 14, -1, 11, 12, -1, 29, -1, 108, 109, -1, 110, 111, -1, 102, 103, -1,
  118, 119, -1, 0, 1, -1, 112, 113, -1, 95, 96, -1, 100, 101, -1, 25, 26, -1, 27, -1, 114, 115, -1, 38, 39, -1, 104, 105, -1, 23, 24, -1, 88, 89, -1,
  28, -1
};
static char data_472E0[] = "DR01_011";
static char data_472E9[] = "DR00_002";
static char data_472F2[] = "DR05_001";
static char data_472FB[] = "DR02_012";
static char data_47304[] = "DR02_016";
static char data_4730D[] = "DR00_003";
static char data_47316[] = "DR02_014";
static char data_4731F[] = "DR15_013";
static char data_47328[] = "DR01_006";
static char data_47331[] = "DR01_007";
static char data_4733A[] = "DR13_010";
static char data_47343[] = "DR15_009";
static char data_4734C[] = "DR01_010";
static char data_47355[] = "DR02_018";
static char data_4735E[] = "DR17_002";
static char data_47367[] = "DR13_009";
static char data_47370[] = "DR06_007";
static char data_47379[] = "DR06_008";
static char data_47382[] = "DR15_011";
static char data_4738B[] = "DR02_013";
static char data_47394[] = "DR13_012";
static char data_4739D[] = "DR01_018";
static char data_473A6[] = "DR13_008";
static char data_473AF[] = "DR02_017";
static char data_473B8[] = "DR13_007";
static char data_473C1[] = "DR12_001";
static char data_473CA[] = "DR15_014";
static char data_473D3[] = "DR28_001";
static char data_473DC[] = "DR15_010";
static char data_473E5[] = "DR02_019";
static char data_473EE[] = "DR02_020";
static char data_473F7[] = "DR01_008";
static char data_47400[] = "DR01_009";
static char data_47409[] = "DR15_012";
static char data_47412[] = "DR02_015";
static char data_4741B[] = "DR13_013";
static char *const data_47424 = data_472E0;
static const int32_t data_47428[3] = {1, 1311403, 1376939};
static char *const data_47434 = data_472E9;
static const int32_t data_47438[3] = {0, -1, -1};
static char *const data_47444 = data_472F2;
static const int32_t data_47448[3] = {5, 1442475, 1508011};
static char *const data_47454 = data_472FB;
static const int32_t data_47458[3] = {2, 1573547, 1639083};
static char *const data_47464 = data_47304;
static const int32_t data_47468[3] = {2, 1573547, 1639083};
static char *const data_47474 = data_4730D;
static const int32_t data_47478[3] = {0, -1, -1};
static char *const data_47484 = data_47316;
static const int32_t data_47488[3] = {2, 1573547, 1639083};
static char *const data_47494 = data_4731F;
static const int32_t data_47498[3] = {15, 1180331, 1245867};
static char *const data_474A4 = data_47328;
static const int32_t data_474A8[3] = {1, 1311403, 1376939};
static char *const data_474B4 = data_47331;
static const int32_t data_474B8[3] = {36, 1311403, 1376939};
static char *const data_474C4 = data_4733A;
static const int32_t data_474C8[3] = {13, 1442475, 1508011};
static char *const data_474D4 = data_47343;
static const int32_t data_474D8[3] = {15, 1180331, 1245867};
static char *const data_474E4 = data_4734C;
static const int32_t data_474E8[3] = {1, 1311403, 1376939};
static char *const data_474F4 = data_47355;
static const int32_t data_474F8[3] = {2, 1573547, 1639083};
static char *const data_47504 = data_4735E;
static const int32_t data_47508[3] = {17, 656043, 721579};
static char *const data_47514 = data_47367;
static const int32_t data_47518[3] = {13, 1442475, 1508011};
static char *const data_47524 = data_47370;
static const int32_t data_47528[3] = {6, 1442475, 1508011};
static char *const data_47534 = data_47379;
static const int32_t data_47538[3] = {6, 1442475, 1508011};
static char *const data_47544 = data_47382;
static const int32_t data_47548[3] = {15, 1180331, 1245867};
static char *const data_47554 = data_4738B;
static const int32_t data_47558[3] = {2, 1573547, 1639083};
static char *const data_47564 = data_47394;
static const int32_t data_47568[3] = {13, 1442475, 1508011};
static char *const data_47574 = data_4739D;
static const int32_t data_47578[3] = {36, 1311403, 1376939};
static char *const data_47584 = data_473A6;
static const int32_t data_47588[3] = {13, 1442475, 1508011};
static char *const data_47594 = data_473AF;
static const int32_t data_47598[3] = {2, 1573547, 1639083};
static char *const data_475A4 = data_473B8;
static const int32_t data_475A8[3] = {13, 1442475, 1508011};
static char *const data_475B4 = data_473C1;
static const int32_t data_475B8[3] = {12, 656043, 721579};
static char *const data_475C4 = data_473CA;
static const int32_t data_475C8[3] = {15, 1180331, 1245867};
static char *const data_475D4 = data_473D3;
static const int32_t data_475D8[3] = {28, 1049259, 1114795};
static char *const data_475E4 = data_473DC;
static const int32_t data_475E8[3] = {15, 1180331, 1245867};
static char *const data_475F4 = data_473E5;
static const int32_t data_475F8[3] = {2, 1573547, 1639083};
static char *const data_47604 = data_473EE;
static const int32_t data_47608[3] = {2, 1573547, 1639083};
static char *const data_47614 = data_473F7;
static const int32_t data_47618[3] = {1, 1311403, 1376939};
static char *const data_47624 = data_47400;
static const int32_t data_47628[3] = {1, 1311403, 1376939};
static char *const data_47634 = data_47409;
static const int32_t data_47638[3] = {15, 1180331, 1245867};
static char *const data_47644 = data_47412;
static const int32_t data_47648[3] = {2, 1573547, 1639083};
static char *const data_47654 = data_4741B;
static const int32_t data_47658[3] = {13, 1442475, 1508011};
static const MapObjectRecord data_47664[120] = {
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
static const int32_t data_48744[57] = {
  0, 0, -8046, 0, 0, -1820, -13088, 4350, -2894, 0, 0, -9011, 0, 0, -18422, 0, 0, -9812, 0, 0, 0, 0, 0, -7518, 0, 0, -17385, 0, 0, -11559, 0, 0, 0, 0,
  0, -5734, 0, 0, -18058, 0, 0, -12233, 0, 0, 0, 0, 0, -7827, 0, 0, -16183, 0, 0, -13981, 0, -72, 0
};
static const int32_t data_48828[57] = {
  0, 0, -2712, 0, 0, -691, 0, 0, 0, 0, 0, -1092, 0, 0, -1438, 0, 0, -2748, 0, 0, 0, 0, 0, 364, 0, 0, -4132, 0, 0, -3531, 0, 0, 0, 0, 0, -2020, 0, 0,
  -6881, 0, 0, -5333, 0, 109, -200, 0, 0, -3986, 0, 0, -8829, 0, 0, -7245, 0, -764, -837
};
static const uint32_t data_4890C = 0x303131;
static const uint32_t data_48910 = 0x393131;
static const uint32_t data_48914 = 0x373731;
static const uint32_t data_48918 = 0x343031;
static const char data_4891C[] __attribute__((scn_string_literal)) = "61564";
static const char data_48922[] __attribute__((scn_string_literal)) = "37742";
static const char data_48928[] __attribute__((scn_string_literal)) = "37749";
static const char data_4892E[] __attribute__((scn_string_literal)) = "22550";
static const char data_48934[] __attribute__((scn_string_literal)) = "046861564";
static const char data_4893E[] __attribute__((scn_string_literal)) = "046837742";
static const char data_48948[] __attribute__((scn_string_literal)) = "046837749";
static const char data_48952[] __attribute__((scn_string_literal)) = "046822550";
static const char data_4895C[] __attribute__((scn_string_literal)) = "2?????";
static const char data_48963[] __attribute__((scn_string_literal)) = "5?????";
static const char data_4896A[] __attribute__((scn_string_literal)) = "7?????";
static const char data_48971[] __attribute__((scn_string_literal)) = "8?????";
static const char data_48978[] __attribute__((scn_string_literal)) = "3?????";
static const char data_4897F[] __attribute__((scn_string_literal)) = "4?????";
static const char data_48986[] __attribute__((scn_string_literal)) = "6?????";
static const char data_4898D[] __attribute__((scn_string_literal)) = "9?????";
static const char data_48994[] __attribute__((scn_string_literal)) = "0?????????";
static const char data_4899F[] __attribute__((scn_string_literal)) = "1?????";
static const char data_489A6[6] __attribute__((scn_string_literal)) = "E1066";
static const char data_489AC[10] __attribute__((scn_string_literal)) = "E1066A012";
static const char data_489B6[10] __attribute__((scn_string_literal)) = "E1066A013";
static const char data_489C0[10] __attribute__((scn_string_literal)) = "E1066A006";
static const char data_489CA[6] __attribute__((scn_string_literal)) = "E1066";
static const char data_489D0[12] __attribute__((scn_string_literal)) = "メモ帳消去\012";
static const char data_489DC[6] __attribute__((scn_string_literal)) = "E1032";
static const char data_489E2[10] __attribute__((scn_string_literal)) = "E1032A001";
static const char data_489EC[10] __attribute__((scn_string_literal)) = "E1032A002";
static const char data_489F6[10] __attribute__((scn_string_literal)) = "E1032A003";
static const char data_48A00[10] __attribute__((scn_string_literal)) = "E1032A004";
static const char data_48A0A[10] __attribute__((scn_string_literal)) = "E1032A005";
static const char data_48A14[6] __attribute__((scn_string_literal)) = "E1032";
static const char data_48A1A[6] __attribute__((scn_string_literal)) = "E1032";
static const char data_48A20[10] __attribute__((scn_string_literal)) = "E1032A061";
static const char data_48A2A[10] __attribute__((scn_string_literal)) = "E1032A062";
static const char data_48A34[10] __attribute__((scn_string_literal)) = "E1032A063";
static const char data_48A3E[10] __attribute__((scn_string_literal)) = "E1032A064";
static const char data_48A48[6] __attribute__((scn_string_literal)) = "E1032";
static const char data_48A4E[6] __attribute__((scn_string_literal)) = "E1032";
static const char data_48A54[6] __attribute__((scn_string_literal)) = "E1032";
static const char data_48A5A[6] __attribute__((scn_string_literal)) = "E1032";
static const char data_48A60[10] __attribute__((scn_string_literal)) = "E1032A031";
static const char data_48A6A[10] __attribute__((scn_string_literal)) = "E1032A029";
static const char data_48A74[10] __attribute__((scn_string_literal)) = "E1032A032";
static const char data_48A7E[10] __attribute__((scn_string_literal)) = "E1032A033";
static const char data_48A88[6] __attribute__((scn_string_literal)) = "E1032";
static const char data_48A8E[10] __attribute__((scn_string_literal)) = "E1032A028";
static const char data_48A98[10] __attribute__((scn_string_literal)) = "E1032A029";
static const char data_48AA2[10] __attribute__((scn_string_literal)) = "E1032A030";
static const char data_48AAC[6] __attribute__((scn_string_literal)) = "E1032";
static const char data_48AB2[10] __attribute__((scn_string_literal)) = "E1032G001";
static const char data_48ABC[10] __attribute__((scn_string_literal)) = "E1032G002";
static const char data_48AC6[6] __attribute__((scn_string_literal)) = "E1032";
static const char data_48ACC[10] __attribute__((scn_string_literal)) = "E1032I001";
static const char data_48AD6[10] __attribute__((scn_string_literal)) = "E1032A052";
static const char data_48AE0[10] __attribute__((scn_string_literal)) = "E1032I007";
static const char data_48AEA[10] __attribute__((scn_string_literal)) = "E1032I008";
static const char data_48AF4[10] __attribute__((scn_string_literal)) = "E1032A097";
static const char data_48AFE[10] __attribute__((scn_string_literal)) = "E1032I009";
static const char data_48B08[10] __attribute__((scn_string_literal)) = "E1032I010";
static const char data_48B12[10] __attribute__((scn_string_literal)) = "E1032A054";
static const char data_48B1C[10] __attribute__((scn_string_literal)) = "E1032I011";
static const char data_48B26[10] __attribute__((scn_string_literal)) = "E1032A055";
static const char data_48B30[10] __attribute__((scn_string_literal)) = "E1032I012";
static const char data_48B3A[10] __attribute__((scn_string_literal)) = "E1032A056";
static const char data_48B44[10] __attribute__((scn_string_literal)) = "E1032I013";
static const char data_48B4E[10] __attribute__((scn_string_literal)) = "E1032A057";
static const char data_48B58[10] __attribute__((scn_string_literal)) = "E1032I011";
static const char data_48B62[10] __attribute__((scn_string_literal)) = "E1032A059";
static const char data_48B6C[10] __attribute__((scn_string_literal)) = "E1032I009";
static const char data_48B76[10] __attribute__((scn_string_literal)) = "E1032I010";
static const char data_48B80[10] __attribute__((scn_string_literal)) = "E1032A054";
static const char data_48B8A[10] __attribute__((scn_string_literal)) = "E1032I011";
static const char data_48B94[10] __attribute__((scn_string_literal)) = "E1032A060";
static const char data_48B9E[10] __attribute__((scn_string_literal)) = "E1032I009";
static const char data_48BA8[10] __attribute__((scn_string_literal)) = "E1032I010";
static const char data_48BB2[10] __attribute__((scn_string_literal)) = "E1032A054";
static const char data_48BBC[10] __attribute__((scn_string_literal)) = "E1032I011";
static const char data_48BC6[10] __attribute__((scn_string_literal)) = "E1032I001";
static const char data_48BD0[10] __attribute__((scn_string_literal)) = "E1032A007";
static const char data_48BDA[10] __attribute__((scn_string_literal)) = "E1032A006";
static const char data_48BE4[10] __attribute__((scn_string_literal)) = "E1032I002";
static const char data_48BEE[10] __attribute__((scn_string_literal)) = "E1032I003";
static const char data_48BF8[10] __attribute__((scn_string_literal)) = "E1032I004";
static const char data_48C02[10] __attribute__((scn_string_literal)) = "E1032I005";
static const char data_48C0C[10] __attribute__((scn_string_literal)) = "E1032I006";
static const char data_48C16[10] __attribute__((scn_string_literal)) = "E1032A008";
static const char data_48C20[10] __attribute__((scn_string_literal)) = "E1032A009";
static const char data_48C2A[10] __attribute__((scn_string_literal)) = "E1032K001";
static const char data_48C34[10] __attribute__((scn_string_literal)) = "E1032K002";
static const char data_48C3E[10] __attribute__((scn_string_literal)) = "E1032K019";
static const char data_48C48[10] __attribute__((scn_string_literal)) = "E1032K020";
static const char data_48C52[10] __attribute__((scn_string_literal)) = "E1032K022";
static const char data_48C5C[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_48C66[10] __attribute__((scn_string_literal)) = "E1032K016";
static const char data_48C70[10] __attribute__((scn_string_literal)) = "E1032K017";
static const char data_48C7A[10] __attribute__((scn_string_literal)) = "E1032K018";
static const char data_48C84[10] __attribute__((scn_string_literal)) = "E1032K003";
static const char data_48C8E[10] __attribute__((scn_string_literal)) = "E1032K016";
static const char data_48C98[10] __attribute__((scn_string_literal)) = "E1032K003";
static const char data_48CA2[10] __attribute__((scn_string_literal)) = "E1032K017";
static const char data_48CAC[10] __attribute__((scn_string_literal)) = "E1032K003";
static const char data_48CB6[10] __attribute__((scn_string_literal)) = "E1032K018";
static const char data_48CC0[10] __attribute__((scn_string_literal)) = "E1032K004";
static const char data_48CCA[10] __attribute__((scn_string_literal)) = "E1032K016";
static const char data_48CD4[10] __attribute__((scn_string_literal)) = "E1032K004";
static const char data_48CDE[10] __attribute__((scn_string_literal)) = "E1032K017";
static const char data_48CE8[10] __attribute__((scn_string_literal)) = "E1032K004";
static const char data_48CF2[10] __attribute__((scn_string_literal)) = "E1032K018";
static const char data_48CFC[10] __attribute__((scn_string_literal)) = "E1032K005";
static const char data_48D06[10] __attribute__((scn_string_literal)) = "E1032K016";
static const char data_48D10[10] __attribute__((scn_string_literal)) = "E1032K005";
static const char data_48D1A[10] __attribute__((scn_string_literal)) = "E1032K017";
static const char data_48D24[10] __attribute__((scn_string_literal)) = "E1032K005";
static const char data_48D2E[10] __attribute__((scn_string_literal)) = "E1032K018";
static const char data_48D38[10] __attribute__((scn_string_literal)) = "E1032K006";
static const char data_48D42[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_48D4C[10] __attribute__((scn_string_literal)) = "E1032K006";
static const char data_48D56[10] __attribute__((scn_string_literal)) = "E1032K017";
static const char data_48D60[10] __attribute__((scn_string_literal)) = "E1032K006";
static const char data_48D6A[10] __attribute__((scn_string_literal)) = "E1032K018";
static const char data_48D74[10] __attribute__((scn_string_literal)) = "E1032K007";
static const char data_48D7E[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_48D88[10] __attribute__((scn_string_literal)) = "E1032K007";
static const char data_48D92[10] __attribute__((scn_string_literal)) = "E1032K017";
static const char data_48D9C[10] __attribute__((scn_string_literal)) = "E1032K007";
static const char data_48DA6[10] __attribute__((scn_string_literal)) = "E1032K018";
static const char data_48DB0[10] __attribute__((scn_string_literal)) = "E1032K008";
static const char data_48DBA[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_48DC4[10] __attribute__((scn_string_literal)) = "E1032K008";
static const char data_48DCE[10] __attribute__((scn_string_literal)) = "E1032K017";
static const char data_48DD8[10] __attribute__((scn_string_literal)) = "E1032K008";
static const char data_48DE2[10] __attribute__((scn_string_literal)) = "E1032K018";
static const char data_48DEC[10] __attribute__((scn_string_literal)) = "E1032K009";
static const char data_48DF6[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_48E00[10] __attribute__((scn_string_literal)) = "E1032K009";
static const char data_48E0A[10] __attribute__((scn_string_literal)) = "E1032K016";
static const char data_48E14[10] __attribute__((scn_string_literal)) = "E1032K009";
static const char data_48E1E[10] __attribute__((scn_string_literal)) = "E1032K018";
static const char data_48E28[10] __attribute__((scn_string_literal)) = "E1032K010";
static const char data_48E32[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_48E3C[10] __attribute__((scn_string_literal)) = "E1032K010";
static const char data_48E46[10] __attribute__((scn_string_literal)) = "E1032K016";
static const char data_48E50[10] __attribute__((scn_string_literal)) = "E1032K010";
static const char data_48E5A[10] __attribute__((scn_string_literal)) = "E1032K018";
static const char data_48E64[10] __attribute__((scn_string_literal)) = "E1032K011";
static const char data_48E6E[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_48E78[10] __attribute__((scn_string_literal)) = "E1032K011";
static const char data_48E82[10] __attribute__((scn_string_literal)) = "E1032K016";
static const char data_48E8C[10] __attribute__((scn_string_literal)) = "E1032K011";
static const char data_48E96[10] __attribute__((scn_string_literal)) = "E1032K018";
static const char data_48EA0[10] __attribute__((scn_string_literal)) = "E1032K012";
static const char data_48EAA[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_48EB4[10] __attribute__((scn_string_literal)) = "E1032K012";
static const char data_48EBE[10] __attribute__((scn_string_literal)) = "E1032K016";
static const char data_48EC8[10] __attribute__((scn_string_literal)) = "E1032K012";
static const char data_48ED2[10] __attribute__((scn_string_literal)) = "E1032K017";
static const char data_48EDC[10] __attribute__((scn_string_literal)) = "E1032K013";
static const char data_48EE6[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_48EF0[10] __attribute__((scn_string_literal)) = "E1032K013";
static const char data_48EFA[10] __attribute__((scn_string_literal)) = "E1032K016";
static const char data_48F04[10] __attribute__((scn_string_literal)) = "E1032K013";
static const char data_48F0E[10] __attribute__((scn_string_literal)) = "E1032K017";
static const char data_48F18[10] __attribute__((scn_string_literal)) = "E1032K014";
static const char data_48F22[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_48F2C[10] __attribute__((scn_string_literal)) = "E1032K014";
static const char data_48F36[10] __attribute__((scn_string_literal)) = "E1032K016";
static const char data_48F40[10] __attribute__((scn_string_literal)) = "E1032K014";
static const char data_48F4A[10] __attribute__((scn_string_literal)) = "E1032K017";
static const char data_48F54[10] __attribute__((scn_string_literal)) = "E1032K004";
static const char data_48F5E[10] __attribute__((scn_string_literal)) = "E1032K007";
static const char data_48F68[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_48F72[10] __attribute__((scn_string_literal)) = "E1032K004";
static const char data_48F7C[10] __attribute__((scn_string_literal)) = "E1032K007";
static const char data_48F86[10] __attribute__((scn_string_literal)) = "E1032K017";
static const char data_48F90[10] __attribute__((scn_string_literal)) = "E1032K004";
static const char data_48F9A[10] __attribute__((scn_string_literal)) = "E1032K006";
static const char data_48FA4[10] __attribute__((scn_string_literal)) = "E1032K017";
static const char data_48FAE[10] __attribute__((scn_string_literal)) = "E1032K004";
static const char data_48FB8[10] __attribute__((scn_string_literal)) = "E1032K013";
static const char data_48FC2[10] __attribute__((scn_string_literal)) = "E1032K017";
static const char data_48FCC[10] __attribute__((scn_string_literal)) = "E1032K005";
static const char data_48FD6[10] __attribute__((scn_string_literal)) = "E1032K007";
static const char data_48FE0[10] __attribute__((scn_string_literal)) = "E1032K018";
static const char data_48FEA[10] __attribute__((scn_string_literal)) = "E1032K007";
static const char data_48FF4[10] __attribute__((scn_string_literal)) = "E1032K004";
static const char data_48FFE[10] __attribute__((scn_string_literal)) = "E1032K017";
static const char data_49008[10] __attribute__((scn_string_literal)) = "E1032K008";
static const char data_49012[10] __attribute__((scn_string_literal)) = "E1032K010";
static const char data_4901C[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_49026[10] __attribute__((scn_string_literal)) = "E1032K008";
static const char data_49030[10] __attribute__((scn_string_literal)) = "E1032K013";
static const char data_4903A[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_49044[10] __attribute__((scn_string_literal)) = "E1032K010";
static const char data_4904E[10] __attribute__((scn_string_literal)) = "E1032K005";
static const char data_49058[10] __attribute__((scn_string_literal)) = "E1032K016";
static const char data_49062[10] __attribute__((scn_string_literal)) = "E1032K010";
static const char data_4906C[10] __attribute__((scn_string_literal)) = "E1032K004";
static const char data_49076[10] __attribute__((scn_string_literal)) = "E1032K018";
static const char data_49080[10] __attribute__((scn_string_literal)) = "E1032K010";
static const char data_4908A[10] __attribute__((scn_string_literal)) = "E1032K007";
static const char data_49094[10] __attribute__((scn_string_literal)) = "E1032K018";
static const char data_4909E[10] __attribute__((scn_string_literal)) = "E1032K011";
static const char data_490A8[10] __attribute__((scn_string_literal)) = "E1032K007";
static const char data_490B2[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_490BC[10] __attribute__((scn_string_literal)) = "E1032K013";
static const char data_490C6[10] __attribute__((scn_string_literal)) = "E1032K008";
static const char data_490D0[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_490DA[10] __attribute__((scn_string_literal)) = "E1032K010";
static const char data_490E4[10] __attribute__((scn_string_literal)) = "E1032K005";
static const char data_490EE[10] __attribute__((scn_string_literal)) = "E1032K016";
static const char data_490F8[10] __attribute__((scn_string_literal)) = "E1032K004";
static const char data_49102[10] __attribute__((scn_string_literal)) = "E1032K010";
static const char data_4910C[10] __attribute__((scn_string_literal)) = "E1032K016";
static const char data_49116[10] __attribute__((scn_string_literal)) = "E1032K004";
static const char data_49120[10] __attribute__((scn_string_literal)) = "E1032K008";
static const char data_4912A[10] __attribute__((scn_string_literal)) = "E1032K018";
static const char data_49134[10] __attribute__((scn_string_literal)) = "E1032K003";
static const char data_4913E[10] __attribute__((scn_string_literal)) = "E1032K007";
static const char data_49148[10] __attribute__((scn_string_literal)) = "E1032K018";
static const char data_49152[10] __attribute__((scn_string_literal)) = "E1032K007";
static const char data_4915C[10] __attribute__((scn_string_literal)) = "E1032K010";
static const char data_49166[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_49170[10] __attribute__((scn_string_literal)) = "E1032K008";
static const char data_4917A[10] __attribute__((scn_string_literal)) = "E1032K004";
static const char data_49184[10] __attribute__((scn_string_literal)) = "E1032K018";
static const char data_4918E[10] __attribute__((scn_string_literal)) = "E1032K007";
static const char data_49198[10] __attribute__((scn_string_literal)) = "E1032K010";
static const char data_491A2[10] __attribute__((scn_string_literal)) = "E1032K018";
static const char data_491AC[10] __attribute__((scn_string_literal)) = "E1032K004";
static const char data_491B6[10] __attribute__((scn_string_literal)) = "E1032K010";
static const char data_491C0[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_491CA[10] __attribute__((scn_string_literal)) = "E1032K004";
static const char data_491D4[10] __attribute__((scn_string_literal)) = "E1032K007";
static const char data_491DE[10] __attribute__((scn_string_literal)) = "E1032K017";
static const char data_491E8[10] __attribute__((scn_string_literal)) = "E1032K004";
static const char data_491F2[10] __attribute__((scn_string_literal)) = "E1032K007";
static const char data_491FC[10] __attribute__((scn_string_literal)) = "E1032K018";
static const char data_49206[10] __attribute__((scn_string_literal)) = "E1032K010";
static const char data_49210[10] __attribute__((scn_string_literal)) = "E1032K008";
static const char data_4921A[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_49224[10] __attribute__((scn_string_literal)) = "E1032K013";
static const char data_4922E[10] __attribute__((scn_string_literal)) = "E1032K007";
static const char data_49238[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_49242[10] __attribute__((scn_string_literal)) = "E1032K004";
static const char data_4924C[10] __attribute__((scn_string_literal)) = "E1032K013";
static const char data_49256[10] __attribute__((scn_string_literal)) = "E1032K016";
static const char data_49260[10] __attribute__((scn_string_literal)) = "E1032K014";
static const char data_4926A[10] __attribute__((scn_string_literal)) = "E1032K007";
static const char data_49274[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_4927E[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_49288[10] __attribute__((scn_string_literal)) = "E1032L001";
static const char data_49292[10] __attribute__((scn_string_literal)) = "E1032A098";
static const char data_4929C[10] __attribute__((scn_string_literal)) = "E1032L002";
static const char data_492A6[10] __attribute__((scn_string_literal)) = "E1032A099";
static const char data_492B0[10] __attribute__((scn_string_literal)) = "E1032L003";
static const char data_492BA[10] __attribute__((scn_string_literal)) = "E1032L004";
static const char data_492C4[10] __attribute__((scn_string_literal)) = "E1032L005";
static const char data_492CE[10] __attribute__((scn_string_literal)) = "E1032A100";
static const char data_492D8[10] __attribute__((scn_string_literal)) = "E1032L001";
static const char data_492E2[10] __attribute__((scn_string_literal)) = "E1032A101";
static const char data_492EC[10] __attribute__((scn_string_literal)) = "E1032L006";
static const char data_492F6[10] __attribute__((scn_string_literal)) = "E1032L007";
static const char data_49300[10] __attribute__((scn_string_literal)) = "E1032L008";
static const char data_4930A[10] __attribute__((scn_string_literal)) = "E1032L009";
static const char data_49314[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_4931E[10] __attribute__((scn_string_literal)) = "E1032A102";
static const char data_49328[10] __attribute__((scn_string_literal)) = "E1032L010";
static const char data_49332[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_4933C[10] __attribute__((scn_string_literal)) = "E1032L011";
static const char data_49346[10] __attribute__((scn_string_literal)) = "E1032A101";
static const char data_49350[10] __attribute__((scn_string_literal)) = "E1032L012";
static const char data_4935A[10] __attribute__((scn_string_literal)) = "E1032A103";
static const char data_49364[10] __attribute__((scn_string_literal)) = "E1032L013";
static const char data_4936E[10] __attribute__((scn_string_literal)) = "E1032L014";
static const char data_49378[10] __attribute__((scn_string_literal)) = "E1032L015";
static const char data_49382[10] __attribute__((scn_string_literal)) = "E1032A103";
static const char data_4938C[10] __attribute__((scn_string_literal)) = "E1032L016";
static const char data_49396[10] __attribute__((scn_string_literal)) = "E1032A104";
static const char data_493A0[10] __attribute__((scn_string_literal)) = "E1032A105";
static const char data_493AA[10] __attribute__((scn_string_literal)) = "E1032L017";
static const char data_493B4[10] __attribute__((scn_string_literal)) = "E1032A106";
static const char data_493BE[10] __attribute__((scn_string_literal)) = "E1032L018";
static const char data_493C8[10] __attribute__((scn_string_literal)) = "E1032A107";
static const char data_493D2[10] __attribute__((scn_string_literal)) = "E1032L019";
static const char data_493DC[10] __attribute__((scn_string_literal)) = "E1032A108";
static const char data_493E6[10] __attribute__((scn_string_literal)) = "E1032L020";
static const char data_493F0[10] __attribute__((scn_string_literal)) = "E1032L021";
static const char data_493FA[10] __attribute__((scn_string_literal)) = "E1032A109";
static const char data_49404[10] __attribute__((scn_string_literal)) = "E1032L022";
static const char data_4940E[10] __attribute__((scn_string_literal)) = "E1032A110";
static const char data_49418[10] __attribute__((scn_string_literal)) = "E1032L023";
static const char data_49422[10] __attribute__((scn_string_literal)) = "E1032M001";
static const char data_4942C[10] __attribute__((scn_string_literal)) = "E1032A111";
static const char data_49436[10] __attribute__((scn_string_literal)) = "E1032M002";
static const char data_49440[10] __attribute__((scn_string_literal)) = "E1032A112";
static const char data_4944A[10] __attribute__((scn_string_literal)) = "E1032M003";
static const char data_49454[10] __attribute__((scn_string_literal)) = "E1032A113";
static const char data_4945E[10] __attribute__((scn_string_literal)) = "E1032M004";
static const char data_49468[10] __attribute__((scn_string_literal)) = "E1032A104";
static const char data_49472[10] __attribute__((scn_string_literal)) = "E1032A114";
static const char data_4947C[10] __attribute__((scn_string_literal)) = "E1032M005";
static const char data_49486[10] __attribute__((scn_string_literal)) = "E1032M006";
static const char data_49490[10] __attribute__((scn_string_literal)) = "E1032A111";
static const char data_4949A[10] __attribute__((scn_string_literal)) = "E1032M002";
static const char data_494A4[10] __attribute__((scn_string_literal)) = "E1032A115";
static const char data_494AE[10] __attribute__((scn_string_literal)) = "E1032M018";
static const char data_494B8[10] __attribute__((scn_string_literal)) = "E1032A122";
static const char data_494C2[10] __attribute__((scn_string_literal)) = "E1032M019";
static const char data_494CC[10] __attribute__((scn_string_literal)) = "E1032A123";
static const char data_494D6[10] __attribute__((scn_string_literal)) = "E1032M019";
static const char data_494E0[10] __attribute__((scn_string_literal)) = "E1032M020";
static const char data_494EA[10] __attribute__((scn_string_literal)) = "E1032A104";
static const char data_494F4[10] __attribute__((scn_string_literal)) = "E1032A114";
static const char data_494FE[10] __attribute__((scn_string_literal)) = "E1032M010";
static const char data_49508[10] __attribute__((scn_string_literal)) = "E1032N001";
static const char data_49512[10] __attribute__((scn_string_literal)) = "E1032A124";
static const char data_4951C[10] __attribute__((scn_string_literal)) = "E1032A125";
static const char data_49526[10] __attribute__((scn_string_literal)) = "E1032N002";
static const char data_49530[10] __attribute__((scn_string_literal)) = "E1032A126";
static const char data_4953A[10] __attribute__((scn_string_literal)) = "E1032N003";
static const char data_49544[10] __attribute__((scn_string_literal)) = "E1032A127";
static const char data_4954E[10] __attribute__((scn_string_literal)) = "E1032A128";
static const char data_49558[10] __attribute__((scn_string_literal)) = "E1032N004";
static const char data_49562[10] __attribute__((scn_string_literal)) = "E1032A129";
static const char data_4956C[10] __attribute__((scn_string_literal)) = "E1032N005";
static const char data_49576[10] __attribute__((scn_string_literal)) = "E1032L011";
static const char data_49580[10] __attribute__((scn_string_literal)) = "E1032A101";
static const char data_4958A[10] __attribute__((scn_string_literal)) = "E1032L019";
static const char data_49594[10] __attribute__((scn_string_literal)) = "E1032A117";
static const char data_4959E[10] __attribute__((scn_string_literal)) = "E1032L024";
static const char data_495A8[10] __attribute__((scn_string_literal)) = "E1032L025";
static const char data_495B2[10] __attribute__((scn_string_literal)) = "E1032L026";
static const char data_495BC[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_495C6[10] __attribute__((scn_string_literal)) = "E1032A118";
static const char data_495D0[10] __attribute__((scn_string_literal)) = "E1032L023";
static const char data_495DA[10] __attribute__((scn_string_literal)) = "E1032M001";
static const char data_495E4[10] __attribute__((scn_string_literal)) = "E1032A111";
static const char data_495EE[10] __attribute__((scn_string_literal)) = "E1032M002";
static const char data_495F8[10] __attribute__((scn_string_literal)) = "E1032A112";
static const char data_49602[10] __attribute__((scn_string_literal)) = "E1032M015";
static const char data_4960C[10] __attribute__((scn_string_literal)) = "E1032A119";
static const char data_49616[10] __attribute__((scn_string_literal)) = "E1032M016";
static const char data_49620[10] __attribute__((scn_string_literal)) = "E1032A104";
static const char data_4962A[10] __attribute__((scn_string_literal)) = "E1032A118";
static const char data_49634[10] __attribute__((scn_string_literal)) = "E1032M017";
static const char data_4963E[10] __attribute__((scn_string_literal)) = "E1032O001";
static const char data_49648[10] __attribute__((scn_string_literal)) = "E1032A136";
static const char data_49652[10] __attribute__((scn_string_literal)) = "E1032O002";
static const char data_4965C[10] __attribute__((scn_string_literal)) = "E1032A137";
static const char data_49666[10] __attribute__((scn_string_literal)) = "E1032O007";
static const char data_49670[10] __attribute__((scn_string_literal)) = "E1032A127";
static const char data_4967A[10] __attribute__((scn_string_literal)) = "E1032O004";
static const char data_49684[10] __attribute__((scn_string_literal)) = "E1032A138";
static const char data_4968E[10] __attribute__((scn_string_literal)) = "E1032O005";
static const char data_49698[10] __attribute__((scn_string_literal)) = "E1032A139";
static const char data_496A2[10] __attribute__((scn_string_literal)) = "E1032O006";
static const char data_496AC[10] __attribute__((scn_string_literal)) = "E1032O001";
static const char data_496B6[10] __attribute__((scn_string_literal)) = "E1032A136";
static const char data_496C0[10] __attribute__((scn_string_literal)) = "E1032O002";
static const char data_496CA[10] __attribute__((scn_string_literal)) = "E1032A140";
static const char data_496D4[10] __attribute__((scn_string_literal)) = "E1032O007";
static const char data_496DE[10] __attribute__((scn_string_literal)) = "E1032A127";
static const char data_496E8[10] __attribute__((scn_string_literal)) = "E1032O004";
static const char data_496F2[10] __attribute__((scn_string_literal)) = "E1032A138";
static const char data_496FC[10] __attribute__((scn_string_literal)) = "E1032O005";
static const char data_49706[10] __attribute__((scn_string_literal)) = "E1032A139";
static const char data_49710[10] __attribute__((scn_string_literal)) = "E1032O006";
static const char data_4971A[10] __attribute__((scn_string_literal)) = "E1032P001";
static const char data_49724[10] __attribute__((scn_string_literal)) = "E1032A085";
static const char data_4972E[10] __attribute__((scn_string_literal)) = "E1032P002";
static const char data_49738[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_49742[10] __attribute__((scn_string_literal)) = "E1032P003";
static const char data_4974C[10] __attribute__((scn_string_literal)) = "E1032A141";
static const char data_49756[10] __attribute__((scn_string_literal)) = "E1032P004";
static const char data_49760[10] __attribute__((scn_string_literal)) = "E1032A142";
static const char data_4976A[10] __attribute__((scn_string_literal)) = "E1032P005";
static const char data_49774[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_4977E[10] __attribute__((scn_string_literal)) = "E1032P001";
static const char data_49788[10] __attribute__((scn_string_literal)) = "E1032A085";
static const char data_49792[10] __attribute__((scn_string_literal)) = "E1032P006";
static const char data_4979C[10] __attribute__((scn_string_literal)) = "E1032A143";
static const char data_497A6[10] __attribute__((scn_string_literal)) = "E1032P007";
static const char data_497B0[10] __attribute__((scn_string_literal)) = "E1032A144";
static const char data_497BA[10] __attribute__((scn_string_literal)) = "E1032P004";
static const char data_497C4[10] __attribute__((scn_string_literal)) = "E1032O008";
static const char data_497CE[10] __attribute__((scn_string_literal)) = "E1032P008";
static const char data_497D8[10] __attribute__((scn_string_literal)) = "E1032A068";
static const char data_497E2[10] __attribute__((scn_string_literal)) = "E1032P009";
static const char data_497EC[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_497F6[10] __attribute__((scn_string_literal)) = "E1032A085";
static const char data_49800[10] __attribute__((scn_string_literal)) = "E1032P010";
static const char data_4980A[10] __attribute__((scn_string_literal)) = "E1032A145";
static const char data_49814[10] __attribute__((scn_string_literal)) = "E1032P011";
static const char data_4981E[10] __attribute__((scn_string_literal)) = "E1032A146";
static const char data_49828[10] __attribute__((scn_string_literal)) = "E1032P012";
static const char data_49832[10] __attribute__((scn_string_literal)) = "E1032A147";
static const char data_4983C[10] __attribute__((scn_string_literal)) = "E1032P013";
static const char data_49846[10] __attribute__((scn_string_literal)) = "E1032A148";
static const char data_49850[10] __attribute__((scn_string_literal)) = "E1032P014";
static const char data_4985A[10] __attribute__((scn_string_literal)) = "E1032A149";
static const char data_49864[10] __attribute__((scn_string_literal)) = "E1032P015";
static const char data_4986E[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_49878[10] __attribute__((scn_string_literal)) = "E1032P016";
static const char data_49882[10] __attribute__((scn_string_literal)) = "E1032A068";
static const char data_4988C[10] __attribute__((scn_string_literal)) = "E1032P017";
static const char data_49896[10] __attribute__((scn_string_literal)) = "E1032A085";
static const char data_498A0[10] __attribute__((scn_string_literal)) = "E1032P018";
static const char data_498AA[10] __attribute__((scn_string_literal)) = "E1032A150";
static const char data_498B4[10] __attribute__((scn_string_literal)) = "E1032P019";
static const char data_498BE[10] __attribute__((scn_string_literal)) = "E1032A151";
static const char data_498C8[10] __attribute__((scn_string_literal)) = "E1032P004";
static const char data_498D2[10] __attribute__((scn_string_literal)) = "E1032P016";
static const char data_498DC[10] __attribute__((scn_string_literal)) = "E1032A068";
static const char data_498E6[10] __attribute__((scn_string_literal)) = "E1032P017";
static const char data_498F0[10] __attribute__((scn_string_literal)) = "E1032P021";
static const char data_498FA[10] __attribute__((scn_string_literal)) = "E1032A152";
static const char data_49904[10] __attribute__((scn_string_literal)) = "E1032P010";
static const char data_4990E[10] __attribute__((scn_string_literal)) = "E1032A153";
static const char data_49918[10] __attribute__((scn_string_literal)) = "E1032P022";
static const char data_49922[10] __attribute__((scn_string_literal)) = "E1032A154";
static const char data_4992C[10] __attribute__((scn_string_literal)) = "E1032P023";
static const char data_49936[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_49940[10] __attribute__((scn_string_literal)) = "E1032P024";
static const char data_4994A[10] __attribute__((scn_string_literal)) = "E1032A068";
static const char data_49954[10] __attribute__((scn_string_literal)) = "E1032P017";
static const char data_4995E[10] __attribute__((scn_string_literal)) = "E1032P001";
static const char data_49968[10] __attribute__((scn_string_literal)) = "E1032A085";
static const char data_49972[10] __attribute__((scn_string_literal)) = "E1032P025";
static const char data_4997C[10] __attribute__((scn_string_literal)) = "E1032A155";
static const char data_49986[10] __attribute__((scn_string_literal)) = "E1032P004";
static const char data_49990[10] __attribute__((scn_string_literal)) = "E1032A156";
static const char data_4999A[10] __attribute__((scn_string_literal)) = "E1032O008";
static const char data_499A4[10] __attribute__((scn_string_literal)) = "E1032P008";
static const char data_499AE[10] __attribute__((scn_string_literal)) = "E1032A154";
static const char data_499B8[10] __attribute__((scn_string_literal)) = "E1032P027";
static const char data_499C2[10] __attribute__((scn_string_literal)) = "E1032A068";
static const char data_499CC[10] __attribute__((scn_string_literal)) = "E1032P017";
static const char data_499D6[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_499E0[10] __attribute__((scn_string_literal)) = "E1032P021";
static const char data_499EA[10] __attribute__((scn_string_literal)) = "E1032A152";
static const char data_499F4[10] __attribute__((scn_string_literal)) = "E1032P028";
static const char data_499FE[10] __attribute__((scn_string_literal)) = "E1032A158";
static const char data_49A08[10] __attribute__((scn_string_literal)) = "E1032P030";
static const char data_49A12[10] __attribute__((scn_string_literal)) = "E1032A159";
static const char data_49A1C[10] __attribute__((scn_string_literal)) = "E1032P066";
static const char data_49A26[10] __attribute__((scn_string_literal)) = "E1032P016";
static const char data_49A30[10] __attribute__((scn_string_literal)) = "E1032A068";
static const char data_49A3A[10] __attribute__((scn_string_literal)) = "E1032P017";
static const char data_49A44[10] __attribute__((scn_string_literal)) = "E1032P021";
static const char data_49A4E[10] __attribute__((scn_string_literal)) = "E1032A152";
static const char data_49A58[10] __attribute__((scn_string_literal)) = "E1032P032";
static const char data_49A62[10] __attribute__((scn_string_literal)) = "E1032A160";
static const char data_49A6C[10] __attribute__((scn_string_literal)) = "E1032P033";
static const char data_49A76[10] __attribute__((scn_string_literal)) = "E1032P034";
static const char data_49A80[10] __attribute__((scn_string_literal)) = "E1032P080";
static const char data_49A8A[10] __attribute__((scn_string_literal)) = "E1032P081";
static const char data_49A94[10] __attribute__((scn_string_literal)) = "E1032A068";
static const char data_49A9E[10] __attribute__((scn_string_literal)) = "E1032P035";
static const char data_49AA8[10] __attribute__((scn_string_literal)) = "E1032A141";
static const char data_49AB2[10] __attribute__((scn_string_literal)) = "E1032P036";
static const char data_49ABC[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_49AC6[10] __attribute__((scn_string_literal)) = "E1032P021";
static const char data_49AD0[10] __attribute__((scn_string_literal)) = "E1032A152";
static const char data_49ADA[10] __attribute__((scn_string_literal)) = "E1032P025";
static const char data_49AE4[10] __attribute__((scn_string_literal)) = "E1032A160";
static const char data_49AEE[10] __attribute__((scn_string_literal)) = "E1032P033";
static const char data_49AF8[10] __attribute__((scn_string_literal)) = "E1032P082";
static const char data_49B02[10] __attribute__((scn_string_literal)) = "E1032P083";
static const char data_49B0C[10] __attribute__((scn_string_literal)) = "E1032P084";
static const char data_49B16[10] __attribute__((scn_string_literal)) = "E1032P085";
static const char data_49B20[10] __attribute__((scn_string_literal)) = "E1032A068";
static const char data_49B2A[10] __attribute__((scn_string_literal)) = "E1032P086";
static const char data_49B34[10] __attribute__((scn_string_literal)) = "E1032P001";
static const char data_49B3E[10] __attribute__((scn_string_literal)) = "E1032A164";
static const char data_49B48[10] __attribute__((scn_string_literal)) = "E1032P018";
static const char data_49B52[10] __attribute__((scn_string_literal)) = "E1032A165";
static const char data_49B5C[10] __attribute__((scn_string_literal)) = "E1032P037";
static const char data_49B66[10] __attribute__((scn_string_literal)) = "E1032A104";
static const char data_49B70[10] __attribute__((scn_string_literal)) = "E1032P120";
static const char data_49B7A[10] __attribute__((scn_string_literal)) = "E1032A173";
static const char data_49B84[10] __attribute__((scn_string_literal)) = "E1032O008";
static const char data_49B8E[10] __attribute__((scn_string_literal)) = "E1032P008";
static const char data_49B98[10] __attribute__((scn_string_literal)) = "E1032A154";
static const char data_49BA2[10] __attribute__((scn_string_literal)) = "E1032P027";
static const char data_49BAC[10] __attribute__((scn_string_literal)) = "E1032A068";
static const char data_49BB6[10] __attribute__((scn_string_literal)) = "E1032P017";
static const char data_49BC0[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_49BCA[10] __attribute__((scn_string_literal)) = "E1032P001";
static const char data_49BD4[10] __attribute__((scn_string_literal)) = "E1032A164";
static const char data_49BDE[10] __attribute__((scn_string_literal)) = "E1032P018";
static const char data_49BE8[10] __attribute__((scn_string_literal)) = "E1032A170";
static const char data_49BF2[10] __attribute__((scn_string_literal)) = "E1032P044";
static const char data_49BFC[10] __attribute__((scn_string_literal)) = "E1032A200";
static const char data_49C06[10] __attribute__((scn_string_literal)) = "E1032A172";
static const char data_49C10[10] __attribute__((scn_string_literal)) = "E1032P045";
static const char data_49C1A[10] __attribute__((scn_string_literal)) = "E1032A173";
static const char data_49C24[10] __attribute__((scn_string_literal)) = "E1032P046";
static const char data_49C2E[10] __attribute__((scn_string_literal)) = "E1032P047";
static const char data_49C38[10] __attribute__((scn_string_literal)) = "E1032A201";
static const char data_49C42[10] __attribute__((scn_string_literal)) = "E1032P049";
static const char data_49C4C[10] __attribute__((scn_string_literal)) = "E1032A176";
static const char data_49C56[10] __attribute__((scn_string_literal)) = "E1032P050";
static const char data_49C60[10] __attribute__((scn_string_literal)) = "E1032A177";
static const char data_49C6A[10] __attribute__((scn_string_literal)) = "E1032P051";
static const char data_49C74[10] __attribute__((scn_string_literal)) = "E1032P001";
static const char data_49C7E[10] __attribute__((scn_string_literal)) = "E1032A164";
static const char data_49C88[10] __attribute__((scn_string_literal)) = "E1032P018";
static const char data_49C92[10] __attribute__((scn_string_literal)) = "E1032A178";
static const char data_49C9C[10] __attribute__((scn_string_literal)) = "E1032P004";
static const char data_49CA6[10] __attribute__((scn_string_literal)) = "E1032P052";
static const char data_49CB0[10] __attribute__((scn_string_literal)) = "E1032A179";
static const char data_49CBA[10] __attribute__((scn_string_literal)) = "E1032A180";
static const char data_49CC4[10] __attribute__((scn_string_literal)) = "E1032A181";
static const char data_49CCE[10] __attribute__((scn_string_literal)) = "E1032P053";
static const char data_49CD8[10] __attribute__((scn_string_literal)) = "E1032P087";
static const char data_49CE2[10] __attribute__((scn_string_literal)) = "E1032P055";
static const char data_49CEC[10] __attribute__((scn_string_literal)) = "E1032A182";
static const char data_49CF6[10] __attribute__((scn_string_literal)) = "E1032P051";
static const char data_49D00[10] __attribute__((scn_string_literal)) = "E1032P001";
static const char data_49D0A[10] __attribute__((scn_string_literal)) = "E1032A164";
static const char data_49D14[10] __attribute__((scn_string_literal)) = "E1032P018";
static const char data_49D1E[10] __attribute__((scn_string_literal)) = "E1032A183";
static const char data_49D28[10] __attribute__((scn_string_literal)) = "E1032P038";
static const char data_49D32[10] __attribute__((scn_string_literal)) = "E1032A184";
static const char data_49D3C[10] __attribute__((scn_string_literal)) = "E1032P056";
static const char data_49D46[10] __attribute__((scn_string_literal)) = "E1032P088";
static const char data_49D50[10] __attribute__((scn_string_literal)) = "E1032P058";
static const char data_49D5A[10] __attribute__((scn_string_literal)) = "E1032A220";
static const char data_49D64[10] __attribute__((scn_string_literal)) = "E1032A185";
static const char data_49D6E[10] __attribute__((scn_string_literal)) = "E1032P059";
static const char data_49D78[10] __attribute__((scn_string_literal)) = "E1032P001";
static const char data_49D82[10] __attribute__((scn_string_literal)) = "E1032A164";
static const char data_49D8C[10] __attribute__((scn_string_literal)) = "E1032P018";
static const char data_49D96[10] __attribute__((scn_string_literal)) = "E1032P060";
static const char data_49DA0[10] __attribute__((scn_string_literal)) = "E1032A186";
static const char data_49DAA[10] __attribute__((scn_string_literal)) = "E1032P061";
static const char data_49DB4[10] __attribute__((scn_string_literal)) = "E1032A187";
static const char data_49DBE[10] __attribute__((scn_string_literal)) = "E1032P062";
static const char data_49DC8[10] __attribute__((scn_string_literal)) = "E1032A188";
static const char data_49DD2[10] __attribute__((scn_string_literal)) = "E1032P063";
static const char data_49DDC[10] __attribute__((scn_string_literal)) = "E1032P064";
static const char data_49DE6[10] __attribute__((scn_string_literal)) = "E1032P065";
static const char data_49DF0[10] __attribute__((scn_string_literal)) = "E1032A189";
static const char data_49DFA[10] __attribute__((scn_string_literal)) = "E1032P066";
static const char data_49E04[10] __attribute__((scn_string_literal)) = "E1032A190";
static const char data_49E0E[10] __attribute__((scn_string_literal)) = "E1032P001";
static const char data_49E18[10] __attribute__((scn_string_literal)) = "E1032A152";
static const char data_49E22[10] __attribute__((scn_string_literal)) = "E1032P067";
static const char data_49E2C[10] __attribute__((scn_string_literal)) = "E1032A191";
static const char data_49E36[10] __attribute__((scn_string_literal)) = "E1032A192";
static const char data_49E40[10] __attribute__((scn_string_literal)) = "E1032P068";
static const char data_49E4A[10] __attribute__((scn_string_literal)) = "E1032P069";
static const char data_49E54[10] __attribute__((scn_string_literal)) = "E1032A193";
static const char data_49E5E[10] __attribute__((scn_string_literal)) = "E1032P001";
static const char data_49E68[10] __attribute__((scn_string_literal)) = "E1032A152";
static const char data_49E72[10] __attribute__((scn_string_literal)) = "E1032P070";
static const char data_49E7C[10] __attribute__((scn_string_literal)) = "E1032A194";
static const char data_49E86[10] __attribute__((scn_string_literal)) = "E1032P071";
static const char data_49E90[10] __attribute__((scn_string_literal)) = "E1032A195";
static const char data_49E9A[10] __attribute__((scn_string_literal)) = "E1032P072";
static const char data_49EA4[10] __attribute__((scn_string_literal)) = "E1032P073";
static const char data_49EAE[10] __attribute__((scn_string_literal)) = "E1032A151";
static const char data_49EB8[10] __attribute__((scn_string_literal)) = "E1032P074";
static const char data_49EC2[10] __attribute__((scn_string_literal)) = "E1032P075";
static const char data_49ECC[10] __attribute__((scn_string_literal)) = "E1032P089";
static const char data_49ED6[10] __attribute__((scn_string_literal)) = "E1032P090";
static const char data_49EE0[10] __attribute__((scn_string_literal)) = "E1032A196";
static const char data_49EEA[10] __attribute__((scn_string_literal)) = "E1032P079";
static const char data_49EF4[10] __attribute__((scn_string_literal)) = "E1032A197";
static const char data_49EFE[10] __attribute__((scn_string_literal)) = "E1032P001";
static const char data_49F08[10] __attribute__((scn_string_literal)) = "E1032A152";
static const char data_49F12[10] __attribute__((scn_string_literal)) = "E1032P010";
static const char data_49F1C[10] __attribute__((scn_string_literal)) = "E1032P091";
static const char data_49F26[10] __attribute__((scn_string_literal)) = "E1032P092";
static const char data_49F30[10] __attribute__((scn_string_literal)) = "E1032A203";
static const char data_49F3A[10] __attribute__((scn_string_literal)) = "E1032P093";
static const char data_49F44[10] __attribute__((scn_string_literal)) = "E1032P094";
static const char data_49F4E[10] __attribute__((scn_string_literal)) = "E1032A204";
static const char data_49F58[10] __attribute__((scn_string_literal)) = "E1032P095";
static const char data_49F62[10] __attribute__((scn_string_literal)) = "E1032P021";
static const char data_49F6C[10] __attribute__((scn_string_literal)) = "E1032A152";
static const char data_49F76[10] __attribute__((scn_string_literal)) = "E1032P096";
static const char data_49F80[10] __attribute__((scn_string_literal)) = "E1032A104";
static const char data_49F8A[10] __attribute__((scn_string_literal)) = "E1032P097";
static const char data_49F94[10] __attribute__((scn_string_literal)) = "E1032A103";
static const char data_49F9E[10] __attribute__((scn_string_literal)) = "E1032P098";
static const char data_49FA8[10] __attribute__((scn_string_literal)) = "E1032P099";
static const char data_49FB2[10] __attribute__((scn_string_literal)) = "E1032P100";
static const char data_49FBC[10] __attribute__((scn_string_literal)) = "E1032A206";
static const char data_49FC6[10] __attribute__((scn_string_literal)) = "E1032P021";
static const char data_49FD0[10] __attribute__((scn_string_literal)) = "E1032A152";
static const char data_49FDA[10] __attribute__((scn_string_literal)) = "E1032P101";
static const char data_49FE4[10] __attribute__((scn_string_literal)) = "E1032A207";
static const char data_49FEE[10] __attribute__((scn_string_literal)) = "E1032P102";
static const char data_49FF8[10] __attribute__((scn_string_literal)) = "E1032A208";
static const char data_4A002[10] __attribute__((scn_string_literal)) = "E1032P103";
static const char data_4A00C[10] __attribute__((scn_string_literal)) = "E1032A209";
static const char data_4A016[10] __attribute__((scn_string_literal)) = "E1032P104";
static const char data_4A020[10] __attribute__((scn_string_literal)) = "E1032P105";
static const char data_4A02A[10] __attribute__((scn_string_literal)) = "E1032A210";
static const char data_4A034[10] __attribute__((scn_string_literal)) = "E1032P106";
static const char data_4A03E[10] __attribute__((scn_string_literal)) = "E1032A154";
static const char data_4A048[10] __attribute__((scn_string_literal)) = "E1032P107";
static const char data_4A052[10] __attribute__((scn_string_literal)) = "E1032A204";
static const char data_4A05C[10] __attribute__((scn_string_literal)) = "E1032P108";
static const char data_4A066[10] __attribute__((scn_string_literal)) = "E1032P109";
static const char data_4A070[10] __attribute__((scn_string_literal)) = "E1032A085";
static const char data_4A07A[10] __attribute__((scn_string_literal)) = "E1032P110";
static const char data_4A084[10] __attribute__((scn_string_literal)) = "E1032A211";
static const char data_4A08E[10] __attribute__((scn_string_literal)) = "E1032P111";
static const char data_4A098[10] __attribute__((scn_string_literal)) = "E1032A104";
static const char data_4A0A2[10] __attribute__((scn_string_literal)) = "E1032P112";
static const char data_4A0AC[10] __attribute__((scn_string_literal)) = "E1032A212";
static const char data_4A0B6[10] __attribute__((scn_string_literal)) = "E1032P113";
static const char data_4A0C0[10] __attribute__((scn_string_literal)) = "E1032A158";
static const char data_4A0CA[10] __attribute__((scn_string_literal)) = "E1032P114";
static const char data_4A0D4[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_4A0DE[10] __attribute__((scn_string_literal)) = "E1032P115";
static const char data_4A0E8[10] __attribute__((scn_string_literal)) = "E1032A213";
static const char data_4A0F2[10] __attribute__((scn_string_literal)) = "E1032P116";
static const char data_4A0FC[10] __attribute__((scn_string_literal)) = "E1032P001";
static const char data_4A106[10] __attribute__((scn_string_literal)) = "E1032A152";
static const char data_4A110[10] __attribute__((scn_string_literal)) = "E1032P117";
static const char data_4A11A[10] __attribute__((scn_string_literal)) = "E1032A214";
static const char data_4A124[10] __attribute__((scn_string_literal)) = "E1032P118";
static const char data_4A12E[10] __attribute__((scn_string_literal)) = "E1032A154";
static const char data_4A138[10] __attribute__((scn_string_literal)) = "E1032P119";
static const char data_4A142[10] __attribute__((scn_string_literal)) = "E1032P120";
static const char data_4A14C[10] __attribute__((scn_string_literal)) = "E1032A215";
static const char data_4A156[10] __attribute__((scn_string_literal)) = "E1032P121";
static const char data_4A160[10] __attribute__((scn_string_literal)) = "E1032A163";
static const char data_4A16A[10] __attribute__((scn_string_literal)) = "E1032P123";
static const char data_4A174[10] __attribute__((scn_string_literal)) = "E1032A217";
static const char data_4A17E[10] __attribute__((scn_string_literal)) = "E1032P001";
static const char data_4A188[10] __attribute__((scn_string_literal)) = "E1032A152";
static const char data_4A192[10] __attribute__((scn_string_literal)) = "E1032P010";
static const char data_4A19C[10] __attribute__((scn_string_literal)) = "E1032P124";
static const char data_4A1A6[10] __attribute__((scn_string_literal)) = "E1032P125";
static const char data_4A1B0[10] __attribute__((scn_string_literal)) = "E1032A218";
static const char data_4A1BA[10] __attribute__((scn_string_literal)) = "E1032P126";
static const char data_4A1C4[10] __attribute__((scn_string_literal)) = "E1032A154";
static const char data_4A1CE[10] __attribute__((scn_string_literal)) = "E1032P127";
static const char data_4A1D8[10] __attribute__((scn_string_literal)) = "E1032A213";
static const char data_4A1E2[10] __attribute__((scn_string_literal)) = "E1032P128";
static const char data_4A1EC[10] __attribute__((scn_string_literal)) = "E1032A204";
static const char data_4A1F6[10] __attribute__((scn_string_literal)) = "E1032P116";
static const char data_4A200[10] __attribute__((scn_string_literal)) = "E1032P109";
static const char data_4A20A[10] __attribute__((scn_string_literal)) = "E1032A085";
static const char data_4A214[10] __attribute__((scn_string_literal)) = "E1032P117";
static const char data_4A21E[10] __attribute__((scn_string_literal)) = "E1032A214";
static const char data_4A228[10] __attribute__((scn_string_literal)) = "E1032P129";
static const char data_4A232[10] __attribute__((scn_string_literal)) = "E1032P130";
static const char data_4A23C[10] __attribute__((scn_string_literal)) = "E1032P131";
static const char data_4A246[10] __attribute__((scn_string_literal)) = "E1032A213";
static const char data_4A250[10] __attribute__((scn_string_literal)) = "E1032A219";
static const char data_4A25A[10] __attribute__((scn_string_literal)) = "E1032P132";
static const char data_4A264[10] __attribute__((scn_string_literal)) = "E1032A153";
static const char data_4A26E[10] __attribute__((scn_string_literal)) = "E1032P133";
static const char data_4A278[10] __attribute__((scn_string_literal)) = "E1032A220";
static const char data_4A282[10] __attribute__((scn_string_literal)) = "E1032P134";
static const char data_4A28C[10] __attribute__((scn_string_literal)) = "E1032A204";
static const char data_4A296[10] __attribute__((scn_string_literal)) = "E1032P116";
static const char data_4A2A0[10] __attribute__((scn_string_literal)) = "E1032A085";
static const char data_4A2AA[10] __attribute__((scn_string_literal)) = "E1032P010";
static const char data_4A2B4[10] __attribute__((scn_string_literal)) = "E1032A221";
static const char data_4A2BE[10] __attribute__((scn_string_literal)) = "E1032P135";
static const char data_4A2C8[10] __attribute__((scn_string_literal)) = "E1032P136";
static const char data_4A2D2[10] __attribute__((scn_string_literal)) = "E1032A222";
static const char data_4A2DC[10] __attribute__((scn_string_literal)) = "E1032P137";
static const char data_4A2E6[10] __attribute__((scn_string_literal)) = "E1032P138";
static const char data_4A2F0[10] __attribute__((scn_string_literal)) = "E1032A223";
static const char data_4A2FA[10] __attribute__((scn_string_literal)) = "E1032P139";
static const char data_4A304[10] __attribute__((scn_string_literal)) = "E1032A224";
static const char data_4A30E[10] __attribute__((scn_string_literal)) = "E1032P140";
static const char data_4A318[10] __attribute__((scn_string_literal)) = "E1032A225";
static const char data_4A322[10] __attribute__((scn_string_literal)) = "E1032A226";
static const char data_4A32C[10] __attribute__((scn_string_literal)) = "E1032P004";
static const char data_4A336[10] __attribute__((scn_string_literal)) = "E1032E001";
static const char data_4A340[10] __attribute__((scn_string_literal)) = "E1032E002";
static const char data_4A34A[10] __attribute__((scn_string_literal)) = "E1032E003";
static const char data_4A354[10] __attribute__((scn_string_literal)) = "E1032A010";
static const char data_4A35E[10] __attribute__((scn_string_literal)) = "E1032E004";
static const char data_4A368[10] __attribute__((scn_string_literal)) = "E1032A011";
static const char data_4A372[10] __attribute__((scn_string_literal)) = "E1032E005";
static const char data_4A37C[10] __attribute__((scn_string_literal)) = "E1032A012";
static const char data_4A386[10] __attribute__((scn_string_literal)) = "E1032A013";
static const char data_4A390[10] __attribute__((scn_string_literal)) = "E1032E006";
static const char data_4A39A[10] __attribute__((scn_string_literal)) = "E1032A014";
static const char data_4A3A4[10] __attribute__((scn_string_literal)) = "E1032A015";
static const char data_4A3AE[10] __attribute__((scn_string_literal)) = "E1032E007";
static const char data_4A3B8[10] __attribute__((scn_string_literal)) = "E1032A016";
static const char data_4A3C2[10] __attribute__((scn_string_literal)) = "E1032E008";
static const char data_4A3CC[10] __attribute__((scn_string_literal)) = "E1032E009";
static const char data_4A3D6[10] __attribute__((scn_string_literal)) = "E1032A017";
static const char data_4A3E0[10] __attribute__((scn_string_literal)) = "E1032E010";
static const char data_4A3EA[10] __attribute__((scn_string_literal)) = "E1032E011";
static const char data_4A3F4[10] __attribute__((scn_string_literal)) = "E1032E012";
static const char data_4A3FE[10] __attribute__((scn_string_literal)) = "E1032E013";
static const char data_4A408[10] __attribute__((scn_string_literal)) = "E1032A018";
static const char data_4A412[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_4A41C[10] __attribute__((scn_string_literal)) = "E1032A020";
static const char data_4A426[10] __attribute__((scn_string_literal)) = "E1032A021";
static const char data_4A430[10] __attribute__((scn_string_literal)) = "E1032F001";
static const char data_4A43A[10] __attribute__((scn_string_literal)) = "E1032F002";
static const char data_4A444[10] __attribute__((scn_string_literal)) = "E1032F003";
static const char data_4A44E[10] __attribute__((scn_string_literal)) = "E1032A022";
static const char data_4A458[10] __attribute__((scn_string_literal)) = "E1032F004";
static const char data_4A462[10] __attribute__((scn_string_literal)) = "E1032A011";
static const char data_4A46C[10] __attribute__((scn_string_literal)) = "E1032F005";
static const char data_4A476[10] __attribute__((scn_string_literal)) = "E1032A012";
static const char data_4A480[10] __attribute__((scn_string_literal)) = "E1032A013";
static const char data_4A48A[10] __attribute__((scn_string_literal)) = "E1032F006";
static const char data_4A494[10] __attribute__((scn_string_literal)) = "E1032A023";
static const char data_4A49E[10] __attribute__((scn_string_literal)) = "E1032A024";
static const char data_4A4A8[10] __attribute__((scn_string_literal)) = "E1032F007";
static const char data_4A4B2[10] __attribute__((scn_string_literal)) = "E1032A025";
static const char data_4A4BC[10] __attribute__((scn_string_literal)) = "E1032A026";
static const char data_4A4C6[10] __attribute__((scn_string_literal)) = "E1032F008";
static const char data_4A4D0[10] __attribute__((scn_string_literal)) = "E1032F009";
static const char data_4A4DA[10] __attribute__((scn_string_literal)) = "E1032F010";
static const char data_4A4E4[10] __attribute__((scn_string_literal)) = "E1032F011";
static const char data_4A4EE[10] __attribute__((scn_string_literal)) = "E1032F012";
static const char data_4A4F8[10] __attribute__((scn_string_literal)) = "E1032F013";
static const char data_4A502[10] __attribute__((scn_string_literal)) = "E1032F014";
static const char data_4A50C[10] __attribute__((scn_string_literal)) = "E1032F015";
static const char data_4A516[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_4A520[10] __attribute__((scn_string_literal)) = "E1032A018";
static const char data_4A52A[10] __attribute__((scn_string_literal)) = "E1032A027";
static const char data_4A534[10] __attribute__((scn_string_literal)) = "E1032A034";
static const char data_4A53E[10] __attribute__((scn_string_literal)) = "E1032H005";
static const char data_4A548[10] __attribute__((scn_string_literal)) = "E1032H004";
static const char data_4A552[10] __attribute__((scn_string_literal)) = "E1032H009";
static const char data_4A55C[10] __attribute__((scn_string_literal)) = "E1032H006";
static const char data_4A566[10] __attribute__((scn_string_literal)) = "E1032A042";
static const char data_4A570[10] __attribute__((scn_string_literal)) = "E1032A040";
static const char data_4A57A[10] __attribute__((scn_string_literal)) = "E1032A043";
static const char data_4A584[10] __attribute__((scn_string_literal)) = "E1032A041";
static const char data_4A58E[10] __attribute__((scn_string_literal)) = "E1032A045";
static const char data_4A598[10] __attribute__((scn_string_literal)) = "E1032A046";
static const char data_4A5A2[10] __attribute__((scn_string_literal)) = "E1032A047";
static const char data_4A5AC[10] __attribute__((scn_string_literal)) = "E1032A046";
static const char data_4A5B6[10] __attribute__((scn_string_literal)) = "E1032A047";
static const char data_4A5C0[10] __attribute__((scn_string_literal)) = "E1032A044";
static const char data_4A5CA[10] __attribute__((scn_string_literal)) = "E1032A051";
static const char data_4A5D4[10] __attribute__((scn_string_literal)) = "E1032A039";
static const char data_4A5DE[10] __attribute__((scn_string_literal)) = "E1032H007";
static const char data_4A5E8[10] __attribute__((scn_string_literal)) = "E1032A120";
static const char data_4A5F2[10] __attribute__((scn_string_literal)) = "E1032H010";
static const char data_4A5FC[10] __attribute__((scn_string_literal)) = "E1032A066";
static const char data_4A606[10] __attribute__((scn_string_literal)) = "E1032H011";
static const char data_4A610[10] __attribute__((scn_string_literal)) = "E1032J001";
static const char data_4A61A[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_4A624[10] __attribute__((scn_string_literal)) = "E1032A131";
static const char data_4A62E[10] __attribute__((scn_string_literal)) = "E1032J010";
static const char data_4A638[10] __attribute__((scn_string_literal)) = "E1032J013";
static const char data_4A642[10] __attribute__((scn_string_literal)) = "E1032J001";
static const char data_4A64C[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_4A656[10] __attribute__((scn_string_literal)) = "E1032A134";
static const char data_4A660[10] __attribute__((scn_string_literal)) = "E1032J015";
static const char data_4A66A[10] __attribute__((scn_string_literal)) = "E1032A135";
static const char data_4A674[10] __attribute__((scn_string_literal)) = "E1032J001";
static const char data_4A67E[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_4A688[10] __attribute__((scn_string_literal)) = "E1032J018";
static const char data_4A692[10] __attribute__((scn_string_literal)) = "E1032A163";
static const char data_4A69C[10] __attribute__((scn_string_literal)) = "E1032J019";
static const char data_4A6A6[10] __attribute__((scn_string_literal)) = "E1032J020";
static const char data_4A6B0[10] __attribute__((scn_string_literal)) = "E1032A068";
static const char data_4A6BA[10] __attribute__((scn_string_literal)) = "E1032J016";
static const char data_4A6C4[10] __attribute__((scn_string_literal)) = "E1032A161";
static const char data_4A6CE[10] __attribute__((scn_string_literal)) = "E1032J017";
static const char data_4A6D8[10] __attribute__((scn_string_literal)) = "E1032A162";
static const char data_4A6E2[10] __attribute__((scn_string_literal)) = "E1032J001";
static const char data_4A6EC[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_4A6F6[10] __attribute__((scn_string_literal)) = "E1032A131";
static const char data_4A700[10] __attribute__((scn_string_literal)) = "E1032J010";
static const char data_4A70A[10] __attribute__((scn_string_literal)) = "E1032A132";
static const char data_4A714[10] __attribute__((scn_string_literal)) = "E1032J011";
static const char data_4A71E[10] __attribute__((scn_string_literal)) = "E1032J012";
static const char data_4A728[10] __attribute__((scn_string_literal)) = "E1032H007";
static const char data_4A732[10] __attribute__((scn_string_literal)) = "E1032A120";
static const char data_4A73C[10] __attribute__((scn_string_literal)) = "E1032H010";
static const char data_4A746[10] __attribute__((scn_string_literal)) = "E1032A066";
static const char data_4A750[10] __attribute__((scn_string_literal)) = "E1032H011";
static const char data_4A75A[10] __attribute__((scn_string_literal)) = "E1032J001";
static const char data_4A764[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_4A76E[10] __attribute__((scn_string_literal)) = "E1032J018";
static const char data_4A778[10] __attribute__((scn_string_literal)) = "E1032A163";
static const char data_4A782[10] __attribute__((scn_string_literal)) = "E1032J019";
static const char data_4A78C[10] __attribute__((scn_string_literal)) = "E1032J020";
static const char data_4A796[10] __attribute__((scn_string_literal)) = "E1032A068";
static const char data_4A7A0[10] __attribute__((scn_string_literal)) = "E1032J016";
static const char data_4A7AA[10] __attribute__((scn_string_literal)) = "E1032A161";
static const char data_4A7B4[10] __attribute__((scn_string_literal)) = "E1032J017";
static const char data_4A7BE[10] __attribute__((scn_string_literal)) = "E1032A162";
static const char data_4A7C8[10] __attribute__((scn_string_literal)) = "E1032J001";
static const char data_4A7D2[10] __attribute__((scn_string_literal)) = "E1032A067";
static const char data_4A7DC[10] __attribute__((scn_string_literal)) = "E1032J002";
static const char data_4A7E6[10] __attribute__((scn_string_literal)) = "E1032A068";
static const char data_4A7F0[10] __attribute__((scn_string_literal)) = "E1032J001";
static const char data_4A7FA[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_4A804[10] __attribute__((scn_string_literal)) = "E1032J003";
static const char data_4A80E[10] __attribute__((scn_string_literal)) = "E1032A069";
static const char data_4A818[10] __attribute__((scn_string_literal)) = "E1032J004";
static const char data_4A822[10] __attribute__((scn_string_literal)) = "E1032A070";
static const char data_4A82C[10] __attribute__((scn_string_literal)) = "E1032H007";
static const char data_4A836[10] __attribute__((scn_string_literal)) = "E1032A048";
static const char data_4A840[10] __attribute__((scn_string_literal)) = "E1032H008";
static const char data_4A84A[10] __attribute__((scn_string_literal)) = "E1032A049";
static const char data_4A854[10] __attribute__((scn_string_literal)) = "E1032A050";
static const char data_4A85E[14] __attribute__((scn_string_literal)) = "M_TPIN.BIN";
static const int8_t data_4A86C[16] = {12, 17, 22, 10, 19, 18, 11, 8, 4, 4, 17, 7, 6, 5, 4, 4};
static const int8_t data_4A87C[16] = {12, 1, 2, 1, 3, 2, 2, 8, 4, 4, 17, 7, 6, 5, 4, 4};
static const uint32_t data_4A88C = 0xA800A7;
static const int32_t data_4A890[5] = {8847529, 8978568, 15073509, 15204583, 15335657};
static const uint32_t data_4A8A4 = 0x440043;
static const int32_t data_4A8A8[8] = {4587589, 4718663, 4849737, 4980811, 5111885, 5242959, 5374033, 85};
static const uint32_t data_4A8C8 = 0x5C005B;
static const int32_t data_4A8CC[10] = {6160477, 6291551, 6422625, 6553699, 6684773, 6815847, 6946921, 7077995, 7209069, 7340143};
static const uint32_t data_4A8F4 = 0x740073;
static const int32_t data_4A8F8[4] = {7798902, 7929976, 8061050, 8257660};
static const uint32_t data_4A908 = 0x720071;
static const int32_t data_4A90C[9] = {10223750, 10354845, 10485919, 10616993, 10748067, 10879141, 11206826, 5767255, 180};
static const uint32_t data_4A930 = 0x810080;
static const int32_t data_4A934[8] = {8585346, 11403437, 11534511, 11665585, 11862195, 11993270, 12452024, 8650879};
static const uint32_t data_4A954 = 0xB900BC;
static const int32_t data_4A958[5] = {12255418, 13238473, 13500621, 12517579, 200};
static const uint32_t data_4A96C = 0x5A0059;
static const int32_t data_4A970[3] = {5636268, 14221445, 5505107};
static const uint32_t data_4A97C = 0x7D0075;
static const int32_t data_4A980 = 12386442;
static const uint32_t data_4A984 = 0xC600C5;
static const int32_t data_4A988 = 13369543;
static const uint32_t data_4A98C = 0x8C008B;
static const int32_t data_4A990[8] = {9306253, 9437327, 9633937, 148, 9961623, 10092697, 9568411, 149};
static const uint32_t data_4A9B0 = 0xD700D6;
static const int32_t data_4A9B4[3] = {14287064, 14876891, 228};
static const uint32_t data_4A9C0 = 0xDC00D3;
static const int32_t data_4A9C4[2] = {13893845, 14549213};
static const uint32_t data_4A9CC = 0xC100C0;
static const int32_t data_4A9D0[2] = {12779714, 196};
static const uint32_t data_4A9D8 = 0xD000CF;
static const int32_t data_4A9DC = 13762769;
static const uint32_t data_4A9E0 = 0xE000DF;
static const int32_t data_4A9E4 = 14811361;
static const char data_4A9E8[169] = "%GACR016G.MT5\000%GACK66BG.MT5\000%GACK66RG.MT5\000%GACR0BNG.MT5\000%GACR0B2G.MT5\000%GACR0B3G.MT5\000%SPBK6REG.MT5\000%SPBK6BRG.MT5\000%SPBK6YEG.MT5\000%SAIK6WHG.MT5\000%SAIK6BKG.MT5\000%SAIK6REG.MT5";
static const char data_4AA91[315] = "%GACIAK1G.MT5\000%GACIAK2G.MT5\000%GACOJK1G.MT5\000%GACOJK2G.MT5\000%GACOSA1G.MT5\000%GACOSA2G.MT5\000%GACOLA1G.MT5\000%GACOLA2G.MT5\000%GACOPA1G.MT5\000%GACOPA2G.MT5\000%GACGWF1G.MT5\000%GACGWF2G.MT5\000%GACGJF1G.MT5\000%GACGJF2G.MT5\000%GACIKA1G.MT5\000%GACIKA2G.MT5\000%GACM401G.MT5\000%GACIAA1G.MT5\000%GACIAA2G.MT5\000D\000D\000D\000D\000D\000D\000D\000D\000D\000D\000%GACGJJ1G.MT5\000%GACGJJ2G.MT5";
static const char data_4ABCC[309] = "%GACIAKAG.MT5\000%GACIAKBG.MT5\000%GACGJKAG.MT5\000%GACGJKBG.MT5\000%GACOSAAG.MT5\000%GACOSABG.MT5\000%GACOLAAG.MT5\000%GACOLABG.MT5\000%GACOPAAG.MT5\000%GACOPABG.MT5\000%GACIWLAG.MT5\000%GACIWLBG.MT5\000%GACGJFAG.MT5\000%GACGJFBG.MT5\000%GACIKAAG.MT5\000%GACIKABG.MT5\000%GACGSNAG.MT5\000%GACGSNBG.MT5\000%GACILIAG.MT5\000%GACILIBG.MT5\000%GACIDUAG.MT5\000%GACIDUBG.MT5";
static const char data_4AD01[167] = "%GACRSO1G.MT5\000%GACRSO2G.MT5\000%GACRTL1G.MT5\000%GACRNUCG.MT5\000%GACRBAKG.MT5\000%GACRBING.MT5\000%GACRFNGG.MT5\000%GACRAMYG.MT5\000%GACRESPG.MT5\000%GACREGGG.MT5\000D\000D\000D\000D\000D\000D\000%GACRFNNG.MT5";
static const char data_4ADA8[365] = "%GACRNT1G.MT5\000%GACRNT2G.MT5\000%GACRLOPG.MT5\000%GACRMYUG.MT5\000%GACRCKNG.MT5\000%GACRALFG.MT5\000%GACRRISG.MT5\000%GACRARXG.MT5\000%GACR0OPG.MT5\000%GACR0O2G.MT5\000%GACRPOPG.MT5\000%GACRCOBG.MT5\000%GACRIDAG.MT5\000%GACRSHPG.MT5\000%GACRMOBG.MT5\000%GACRROBG.MT5\000%GACK6TZG.MT5\000%GACRPANG.MT5\000%GACK6DYG.MT5\000D\000D\000D\000D\000D\000D\000D\000%GACRAXXG.MT5\000D\000D\000D\000D\000D\000D\000D\000%GACRRBBG.MT5\000%GACK6TSG.MT5\000%GACRPPNG.MT5\000%GACK6DSG.MT5";
static const char data_4AF15[505] = "%GACK6HHG.MT5\000%GACK6HYG.MT5\000%GACK6HBG.MT5\000%GACK6HGG.MT5\000%GACH5BKG.MT5\000%KAB02FSG.MT5\000%GACO1K2G.MT5\000%GACO1K3G.MT5\000%GACO1K7G.MT5\000%GACO1K8G.MT5\000%GACO1K9G.MT5\000%GACO1K1G.MT5\000%GACO1K4G.MT5\000%GACO1K5G.MT5\000%GACO1K6G.MT5\000%GACS5BSG.MT5\000%GACK6HOG.MT5\000%GACK6HKG.MT5\000%GACK6XHG.MT5\000%GACK6XYG.MT5\000%GACK6XBG.MT5\000%GACK6XGG.MT5\000%GACK6BKG.MT5\000%KABK6FSG.MT5\000%GACOKK2G.MT5\000%GACOKK3G.MT5\000%GACOKK7G.MT5\000%GACOKK8G.MT5\000%GACOKK9G.MT5\000%GACOKK1G.MT5\000%GACOKK4G.MT5\000%GACOKK5G.MT5\000%GACOKK6G.MT5\000%GACS5BBG.MT5\000%GACK6XOG.MT5\000%GACK6XKG.MT5";
static const char data_4B10E[211] = "%GACH5TKG.MT5\000%GACGTRXG.MT5\000%GACO1T1G.MT5\000%GACO1T2G.MT5\000%GACO1KOG.MT5\000%GACO1KBG.MT5\000%GACO1KJG.MT5\000%GACH5UKG.MT5\000%GACH5IKG.MT5\000%GACO1FOG.MT5\000%GACGSHEG.MT5\000%GACH5TTG.MT5\000%GACGTXXG.MT5\000%GACOTT1G.MT5\000%GACOTT2G.MT5";
static const char data_4B1E1[113] = "%YKW0201G.MT5\000%YWT0201G.MT5\000%GACRBONG.MT5\000%GACRRENG.MT5\000%GACK6SPG.MT5\000%GACG360G.MT5\000%GACRDR1G.MT5\000%GACRDR2G.MT5";
static const char data_4B252[77] = "%GACRSSCG.MT5\000%GACRMETG.MT5\000%GACRGBIG.MT5\000%GACH5TRG.MT5\000D\000D\000D\000%GACH5RRG.MT5";
static const char data_4B29F[57] = "%GACS5FRG.MT5\000%GACS5FBG.MT5\000%GACO1KUG.MT5\000%GACO1FEG.MT5";
static const char data_4B2D8[239] = "%GACODG1G.MT5\000%GACODG2G.MT5\000%GACODG3G.MT5\000%GACOCT2G.MT5\000%GACOCATG.MT5\000%GACO1SHG.MT5\000%GACOKSHG.MT5\000%GACO1IRG.MT5\000%GACOKIRG.MT5\000%GACO1HAG.MT5\000%GACOHA2G.MT5\000%GACOHA3G.MT5\000%GACOHA4G.MT5\000%GACO1YGG.MT5\000%GACO1USG.MT5\000%GACOOSHG.MT5\000%GACOOIRG.MT5";
static const char data_4B3C7[99] = "%GACM400G.MT5\000%GACGSPHG.MT5\000%GACS5HGG.MT5\000%GACGDT1G.MT5\000%GACS5EQG.MT5\000%SYOGSH0G.MT5\000%SYOGHG0G.MT5";
static const char data_4B42A[85] = "%GACGCRYG.MT5\000%GACGBIRG.MT5\000%GACH5SLG.MT5\000%GACH5JKG.MT5\000%SYOGMD1G.MT5\000%SYOGMD5G.MT5";
static const char data_4B47F[71] = "%GACS5N1G.MT5\000%GACS5N2G.MT5\000%GACS5N3G.MT5\000%GACS5N4G.MT5\000%GACS5N5G.MT5";
static const char data_4B4C6[57] = "%GACGXBNG.MT5\000%GACGYBNG.MT5\000%GACGABNG.MT5\000%GACGBBNG.MT5";
static const char data_4B4FF[57] = "%GACRMEGG.MT5\000%GACR0GGG.MT5\000%GACRSATG.MT5\000%GACRCASG.MT5";
static const uint32_t data_4B538 = SHEN_NAME('GCH0');
static const shen_name data_4B53C[5] = {SHEN_NAME('GCH1'), SHEN_NAME('GCH2'), SHEN_NAME('GCH3'), SHEN_NAME('GCH4'), SHEN_NAME('GCH5')};
static const uint32_t data_4B550 = SHEN_NAME('HGC0');
static const shen_name data_4B554[5] = {SHEN_NAME('HGC1'), SHEN_NAME('HGC2'), SHEN_NAME('HGC3'), SHEN_NAME('HGC4'), SHEN_NAME('HGC5')};
static const uint32_t data_4B568 = SHEN_NAME('GBX0');
static const shen_name data_4B56C[5] = {SHEN_NAME('GBX1'), SHEN_NAME('GBX2'), SHEN_NAME('GBX3'), SHEN_NAME('GBX4'), SHEN_NAME('GBX5')};
static const uint32_t data_4B580 = 0x41C40000;
typedef struct Row_4B584 { int32_t w0; float w1; int32_t w2; int32_t w3; float w4; } Row_4B584;
static const Row_4B584 data_4B584[5] = {
  {0, 32.16f, 32768, 516, 0.0f},
  {0, 0.0f, 0, 0, 0.0f},
  {0, 0.0f, 0, 0, 23.75f},
  {0, 32.16f, 32768, 770, 0.0f},
  {0, 0.0f, 0, 0, 0.0f}
};
static const int32_t data_4B5E8[4] = {0, 0, 0, 0};
static const Vector3 data_4B5F8 = {.x = 0.29f, .y = 0.0f, .z = 0.088f};
static const Vector3i data_4B604 = {.x = 0, .y = 0, .z = 0};
static const char data_4B610[6] __attribute__((scn_string_literal)) = "E1066";
static const char data_4B616[13] __attribute__((scn_string_literal)) = "e1gachap.snd";
static const char data_4B623[16] __attribute__((scn_string_literal)) = "/scene/01/D000/";
static const char data_4B633[8] __attribute__((scn_string_literal)) = "HI_GACH";
static const char data_4B63B[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_4B640[6] __attribute__((scn_string_literal)) = "E1066";
static const char data_4B646[8] __attribute__((scn_string_literal)) = "HI_GACH";
static const char data_4B64E[10] __attribute__((scn_string_literal)) = "HI_GACH";
static const Vector4 data_4B658 = {.x = 0.0f, .y = 0.7f, .z = 0.7f, .w = 0.7f};
static const uint8_t data_4B668[4] = {0};
static const int32_t data_4B66C[56] = {
  -546, -5870, 0, 2184, -3726, 0, 182, 0, 0, 0, -5184, 0, 71525, -12075, 0, -182, -5279, 0, -364, 910, 0, 1456, -2894, 0, 3458, -13352, 0, -910,
  -10327, 0, 0, 1820, 0, 0, -6863, 0, 1820, -16438, 0, -364, -10150, 0, 236, 291, 0, 0, -12105, 0, 0, -15539, 0, 0, -7249, 0, -819, -364
};
static const uint8_t data_4B74C[4] = {0};
static const int32_t data_4B750[56] = {
  -546, -5870, 0, 2184, -3726, 0, 182, 0, 0, 0, -5184, 0, 71525, -10922, 0, -182, -5279, 0, -364, 910, 0, 1456, -2894, 0, 3458, -13352, 0, -910,
  -10327, 0, 0, 1820, 0, 0, -6863, 0, 1820, -16438, 0, -364, -10150, 0, 236, 291, 0, 0, -12105, 0, 0, -15539, 0, 0, -7249, 0, -819, -364
};
static const uint8_t data_4B830[4] = {0};
static const int32_t data_4B834[56] = {
  2912, -7507, 0, 6189, -4454, 0, -1456, 0, 0, 546, -8266, 0, 1292, -7099, 0, 910, -11468, 0, 728, 3276, 0, 1092, -11450, 0, 546, -10256, 0, 1456,
  -6504, 0, 546, 1638, 0, -1092, -12142, 0, -364, -16438, 0, -364, -10150, 0, -309, 291, 0, 1638, -11923, 0, -3276, -17359, 0, -2912, -10707, 0, -819,
  -364
};
static const uint8_t data_4B914[4] = {0};
static const int32_t data_4B918[56] = {
  1638, -7507, 0, 6189, -4454, 0, -1456, 0, 0, 546, -17731, 0, -709, -9466, 0, 728, -15473, 0, 182, 2548, 0, 1092, -13271, 0, -1092, -14625, 0, 1638,
  -11965, 0, 0, 1638, 0, -1092, -12142, 0, -364, -16438, 0, -364, -10150, 0, -309, 291, 0, 1638, -11923, 0, -3276, -17359, 0, -2912, -10707, 0, -819,
  -364
};
static const uint8_t data_4B9F8[4] = {0};
static const int32_t data_4B9FC[56] = {
  1638, -6779, 0, 6189, -4454, 0, -1456, 0, 0, 546, -17731, 0, -709, -9466, 0, 1274, -15473, 0, 546, 2548, 0, 1092, -13271, 0, -1092, -14625, 0, 1638,
  -11965, 0, -910, 1638, 0, -1092, -12142, 0, -364, -16438, 0, -364, -10150, 0, -309, 291, 0, 1638, -12105, 0, -3276, -17359, 0, -2912, -10889, 0,
  -819, -364
};
static const uint8_t data_4BADC[4] = {0};
static const int32_t data_4BAE0[56] = {
  -2730, -9510, -3094, 4004, -1359, 0, 4004, -728, 0, 546, -6990, 0, 18, -4004, 2366, 1274, -11468, 0, 182, -182, 0, 1092, -13271, 0, -1092, -14443,
  0, 1638, -11965, 0, -910, 1638, 0, -1092, -12142, 0, -364, -16438, 0, -364, -10150, 0, -309, 291, 0, 1638, -12105, 0, -3276, -17359, 0, -2912,
  -10889, 0, -819, -364
};
static const uint8_t data_4BBC0[8] = {0};
static const int32_t data_4BBC8[55] = {
  -6005, 0, 4731, -6188, 0, 0, 0, 0, 0, -2727, 0, 0, -9097, 0, 0, -5457, 0, 0, 0, 0, 0, -8734, 0, -909, -7277, 0, 0, -7823, 0, 0, 0, 0, 1456, -4002,
  0, 0, -9462, 0, 726, -7824, 0, 0, 0, 0, 0, -12011, 0, -1091, -7461, 0, -1636, -9280, 0, 0, 0
};
static const uint8_t data_4BCA4[4] = {0};
static const int32_t data_4BCA8[56] = {
  -546, -5870, 0, 2184, -3726, 0, 182, 0, 0, 0, -5184, 0, 71525, -12075, 0, -182, -5279, 0, -364, 910, 0, 1456, -2894, 0, 3458, -13352, 0, -910,
  -10327, 0, 0, 1820, 0, 0, -6863, 0, 1820, -16438, 0, -364, -10150, 0, 236, 291, 0, 0, -12105, 0, 0, -15539, 0, 0, -7249, 0, -819, -364
};
static const uint8_t data_4BD88[8] = {0};
static const int32_t data_4BD90[55] = {
  -8046, 0, 0, -1820, -13088, 4350, -2894, 0, 0, -9011, 0, 0, -18422, 0, 0, -9812, 0, 0, 0, 0, 0, -7518, 0, 0, -17385, 0, 0, -11559, 0, 0, 0, 0, 0,
  -5734, 0, 0, -18058, 0, 0, -12233, 0, 0, 0, 0, 0, -7827, 0, 0, -16183, 0, 0, -13981, 0, -72, 0
};
static const Vector3 data_4BE6C = {.x = 0.067f, .y = 0.061f, .z = 0.0379f};
static const uint32_t data_4BE78 = 0x3DB851EC;
static const float data_4BE7C[2] = {0.075f, 0.008f};
static const Vector3i data_4BE84 = {.x = 13111, .y = 817, .z = 50684};
static const Vector3 data_4BE90 = {.x = 0.091f, .y = -0.087f, .z = 0.0671f};
static const Vector3i data_4BE9C = {.x = 20024, .y = 65353, .z = 11832};
static const Vector3 data_4BEA8 = {.x = 0.0f, .y = 0.0f, .z = 0.0f};
static const Vector3i data_4BEB4 = {.x = 0, .y = 0, .z = 0};
static const Vector3 data_4BEC0 = {.x = -0.042f, .y = 0.1735f, .z = 0.2062f};
static const char data_4BECC[] __attribute__((scn_string_literal)) = "E1066A001";
static const char data_4BED6[] __attribute__((scn_string_literal)) = "E1066A002";
static const char data_4BEE0[] __attribute__((scn_string_literal)) = "E1066A003";
static const char data_4BEEA[] __attribute__((scn_string_literal)) = "E1066A004";
static const char data_4BEF4[] __attribute__((scn_string_literal)) = "E1066A005";
static const char data_4BEFE[] __attribute__((scn_string_literal)) = "E1066A006";
static const char data_4BF08[] __attribute__((scn_string_literal)) = "E1066A007";
static const char data_4BF12[] __attribute__((scn_string_literal)) = "E1066A008";
static const char data_4BF1C[] __attribute__((scn_string_literal)) = "E1066A009";
static const char data_4BF26[] __attribute__((scn_string_literal)) = "E1066A010";
static const char data_4BF30[] __attribute__((scn_string_literal)) = "E1066A011";
static const char data_4BF3A[] __attribute__((scn_string_literal)) = "E1066A026";
static const char data_4BF44[] __attribute__((scn_string_literal)) = "E1066A027";
static const char data_4BF4E[] __attribute__((scn_string_literal)) = "E1066A019";
static const char data_4BF58[] __attribute__((scn_string_literal)) = "E1066A020";
static const char data_4BF62[] __attribute__((scn_string_literal)) = "E1066A021";
static const char data_4BF6C[] __attribute__((scn_string_literal)) = "E1066A014";
static const char data_4BF76[] __attribute__((scn_string_literal)) = "E1066A015";
static const char data_4BF80[] __attribute__((scn_string_literal)) = "E1066A016";
static const char data_4BF8A[] __attribute__((scn_string_literal)) = "E1066A017";
static const char data_4BF94[] __attribute__((scn_string_literal)) = "E1066A018";
static const char data_4BF9E[] __attribute__((scn_string_literal)) = "E1066A022";
static const char data_4BFA8[] __attribute__((scn_string_literal)) = "E1066A025";
static const char data_4BFB2[] __attribute__((scn_string_literal)) = "E1066A024";
static const char data_4BFBC[12] __attribute__((scn_string_literal)) = "E1066A023";
static const Vector3 data_4BFC8 = {.x = 0.337795f, .y = 0.596267f, .z = -0.181433f};
static const Vector3 data_4BFD4 = {.x = 0.091f, .y = -0.087f, .z = 0.0671f};
static const Vector3i data_4BFE0 = {.x = 20024, .y = 65353, .z = 11832};
static const char data_4BFEC[12] __attribute__((scn_string_literal)) = "JINGURU01";
static const uint16_t data_4BFF8[20] = {
  1068, 1043, 1069, 1071, 1074, 1072, 1073, 0, 52953, 16735, 52429, 15820, 11901, 16956, 14995, 16706, 52429, 15820, 7602, 16957
};
static const Vector3 data_4C020 = {.x = 13.9523f, .y = 0.1f, .z = 46.176701f};
static const Vector3 data_4C02C = {.x = 11.8471f, .y = 0.1f, .z = 48.298302f};
static const Vector3 data_4C038 = {.x = 13.7393f, .y = 0.1f, .z = 46.326801f};
static const float data_4C044[3] = {12.0095f, 0.1f, 48.326f};
static const Vector3 data_4C050 = {.x = 13.9365f, .y = 0.1f, .z = 47.3013f};
static const Vector3 data_4C05C = {.x = 12.0318f, .y = 0.1f, .z = 47.643398f};
static const Vector3 data_4C068 = {.x = 14.4428f, .y = 0.1f, .z = 47.1908f};
static const Vector3 data_4C074 = {.x = 12.0466f, .y = 0.1f, .z = 47.5914f};
static const Vector3 data_4C080 = {.x = 14.4431f, .y = 0.1f, .z = 47.190701f};
static const Vector3 data_4C08C = {.x = 12.0475f, .y = 0.1f, .z = 47.594898f};
static const Vector3 data_4C098 = {.x = 14.2992f, .y = 0.1f, .z = 47.118198f};
static const Vector3 data_4C0A4 = {.x = 11.9747f, .y = 0.1f, .z = 47.474499f};
static const int32_t data_4C0B0[114] = {
  0, -728, -3640, 0, -2366, 1820, 0, 3094, 546, 0, -546, -6735, 0, 0, -4004, 0, 0, -910, 0, -182, 1274, 0, 0, -13289, 0, 0, -5279, 0, 0, -4004, 0, 0,
  1456, 0, 182, -8738, 0, 0, -6007, 0, 0, -8738, 0, 182, 1638, 0, -728, -5279, 0, -1274, -6735, 0, -910, -8738, 0, 0, 1638, 0, 0, -5461, 0, -2366,
  -1092, 0, -182, 1092, 0, -546, -6735, 0, 0, -4004, 0, 0, -6553, 0, -182, 1274, 0, 0, -13289, 0, 0, -5279, 0, 0, -8009, 0, 0, 1456, 0, 182, -8738, 0,
  0, -6007, 0, 0, -8738, 0, 182, 1638, 0, -728, -5279, 0, -1274, -6735, 0, -910, -8738, 0, 0, 1638
};
static const int32_t data_4C278[57] = {
  0, 0, -5461, 0, -2366, -1092, 5825, -3094, 1092, 0, -546, -6735, 0, 0, -4004, 0, 0, -6553, 0, -182, 1274, 0, 0, -13289, 0, 0, -1456, 0, 0, -8009, 0,
  0, 1456, 0, 182, -8738, 0, 0, -182, 0, 0, -8738, 0, 182, 1638, 0, -728, -5279, 0, -1274, -182, 0, -910, -8738, 0, 0, 1638
};
static const Vector4 data_4C35C = {.x = 1.0f, .y = 0.4f, .z = 0.4f, .w = 0.4f};
static const Vector3 data_4C36C = {.x = 1000.0f, .y = 0.0f, .z = 0.0f};
static const Vector3 data_4C378 = {.x = 0.0f, .y = 0.0f, .z = 0.0f};
static const Vector3 data_4C384 = {.x = 0.0f, .y = 0.0f, .z = 0.0f};
static const Vector3 data_4C390 = {.x = 0.0f, .y = 0.0f, .z = 0.0f};
static const int32_t data_4C39C[57] = {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -3640, 0, 0, -3640, 0, 0, 0, 0, 0, 0, 0, 0, -3640, 0, 0, -3640, 0, 0, 0, 0, 0, 0, 0, 0, -3640, 0, 0, -3640, 0, 0,
  0, 0, 0, 0, 0, 0, -3640, 0, 0, -3640, 0, 0, 0, 0, 0, 0
};
static const uint32_t data_4C480 = 8198;
typedef struct Row_4C484 { int32_t w0; uint32_t w1; int32_t w2; } Row_4C484;
static const Row_4C484 data_4C484[16] = {
  {0, 0x0U, 0},
  {0, 0x80000000U, 0},
  {32769, 0x80020000U, 0},
  {32772, 0x80080000U, 0},
  {32784, 0xFE200000U, 0},
  {576, 0x2800000U, 0},
  {576, 0xFE200000U, 0},
  {32784, 0x80080000U, 0},
  {32772, 0x80020000U, 0},
  {32769, 0x80000000U, 0},
  {0, 0x0U, 0},
  {0, 0x0U, 0},
  {0, 0x0U, 0},
  {0, 0x0U, 0},
  {0, 0x0U, 0},
  {0, 0x0U, 0}
};
static const uint32_t data_4C544 = 8198;
static const uint32_t data_4C548[26] = {
  0, 0, 0, 0, 0, 0, 0, 0xF878300U, 0x8201F003U, 0x10020949U, 0x9498202U, 0x81040804U, 0xC8040932U, 0x7932F108U, 0x101024E9U, 0x24294084U, 0x40841020U,
  0x1010E429U, 0x4284084U, 0x7932F108U, 0x810404E8U, 0xE4090932U, 0x9498202U, 0x82012409U, 0x24090949U, 0xF878300U
};
static const int32_t data_4C5B0[22] = {15375, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
static const Vector4 data_4C608 = {.x = 1.0f, .y = 0.3f, .z = 0.3f, .w = 0.3f};
static const char data_4C618[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_4C61D[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_4C622[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_4C627[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_4C62C[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_4C631[11] __attribute__((scn_string_literal)) = "bgm085.snd";
static const char data_4C63C[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_4C641[13] __attribute__((scn_string_literal)) = "a1_yamaw.snd";
static const char data_4C64E[13] __attribute__((scn_string_literal)) = "battle_1.snd";
static const char data_4C65B[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_4C660[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_4C665[6] __attribute__((scn_string_literal)) = "E1020";
static const char data_4C66B[] __attribute__((scn_string_literal)) = "/scene/01/D000/";
static const char data_4C67B[] __attribute__((scn_string_literal)) = "D0W0";
static const char data_4C680[13] __attribute__((scn_string_literal)) = "seqdata1.bin";
static const char data_4C68D[5] __attribute__((scn_string_literal)) = "D0W0";
static const char data_4C692[13] __attribute__((scn_string_literal)) = "seqdata2.bin";
static const char data_4C69F[5] __attribute__((scn_string_literal)) = "D0W0";
static const char data_4C6A4[13] __attribute__((scn_string_literal)) = "seqdata3.bin";
static const char data_4C6B1[5] __attribute__((scn_string_literal)) = "D0W0";
static const char data_4C6B6[13] __attribute__((scn_string_literal)) = "seqdata4.bin";
static const char data_4C6C3[5] __attribute__((scn_string_literal)) = "D0W0";
static const char data_4C6C8[13] __attribute__((scn_string_literal)) = "seqdata5.bin";
static const char data_4C6D5[5] __attribute__((scn_string_literal)) = "D0W0";
static const char data_4C6DA[13] __attribute__((scn_string_literal)) = "seqdata6.bin";
static const char data_4C6E7[5] __attribute__((scn_string_literal)) = "D0W0";
static const char data_4C6EC[13] __attribute__((scn_string_literal)) = "seqdata7.bin";
static const char data_4C6F9[5] __attribute__((scn_string_literal)) = "D0W0";
static const char data_4C6FE[13] __attribute__((scn_string_literal)) = "seqdata8.bin";
static const char data_4C70B[5] __attribute__((scn_string_literal)) = "D0W0";
static const char data_4C710[13] __attribute__((scn_string_literal)) = "seqdata9.bin";
static const char data_4C71D[5] __attribute__((scn_string_literal)) = "D0W0";
static const char data_4C722[13] __attribute__((scn_string_literal)) = "seqdataA.bin";
static const char data_4C72F[5] __attribute__((scn_string_literal)) = "D0W0";
static const char data_4C734[13] __attribute__((scn_string_literal)) = "seqdataB.bin";
static const char data_4C741[5] __attribute__((scn_string_literal)) = "D0W0";
static const char data_4C746[13] __attribute__((scn_string_literal)) = "seqdataC.bin";
static const char data_4C753[5] __attribute__((scn_string_literal)) = "D0W0";
static const char data_4C758[6] __attribute__((scn_string_literal)) = "E1020";
static const char data_4C75E[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_4C763[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_4C768[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_4C76D[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_4C772[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_4C777[5] __attribute__((scn_string_literal)) = "D0W0";
static const char data_4C77C[28] __attribute__((scn_string_literal)) = "<<< WazaDenjyu Start >>>\012";
static const Vector3i data_4C798 = {.x = 0, .y = 4551, .z = 0};
static const Vector3i data_4C7A4 = {.x = 0, .y = 0, .z = 5461};
static const char data_4C7B0[15] __attribute__((scn_string_literal)) = "auth %d start\012";
static const char data_4C7BF[10] __attribute__((scn_string_literal)) = "E1020A008";
static const char data_4C7C9[10] __attribute__((scn_string_literal)) = "E1020A009";
static const char data_4C7D3[10] __attribute__((scn_string_literal)) = "E1020B026";
static const char data_4C7DD[10] __attribute__((scn_string_literal)) = "E1020B027";
static const char data_4C7E7[10] __attribute__((scn_string_literal)) = "E1020B028";
static const char data_4C7F1[10] __attribute__((scn_string_literal)) = "E1020B029";
static const char data_4C7FB[10] __attribute__((scn_string_literal)) = "E1020B033";
static const char data_4C805[10] __attribute__((scn_string_literal)) = "E1020B034";
static const char data_4C80F[10] __attribute__((scn_string_literal)) = "E1020B035";
static const char data_4C819[10] __attribute__((scn_string_literal)) = "E1020B036";
static const char data_4C823[10] __attribute__((scn_string_literal)) = "E1020B037";
static const char data_4C82D[10] __attribute__((scn_string_literal)) = "E1020B034";
static const char data_4C837[10] __attribute__((scn_string_literal)) = "E1020B038";
static const char data_4C841[10] __attribute__((scn_string_literal)) = "E1020B039";
static const char data_4C84B[10] __attribute__((scn_string_literal)) = "E1020B040";
static const char data_4C855[10] __attribute__((scn_string_literal)) = "E1020B041";
static const char data_4C85F[10] __attribute__((scn_string_literal)) = "E1020B042";
static const char data_4C869[10] __attribute__((scn_string_literal)) = "E1020B059";
static const char data_4C873[10] __attribute__((scn_string_literal)) = "E1020B060";
static const char data_4C87D[10] __attribute__((scn_string_literal)) = "E1020B061";
static const char data_4C887[10] __attribute__((scn_string_literal)) = "E1020B062";
static const char data_4C891[10] __attribute__((scn_string_literal)) = "E1020B056";
static const char data_4C89B[10] __attribute__((scn_string_literal)) = "E1020B063";
static const char data_4C8A5[10] __attribute__((scn_string_literal)) = "E1020B064";
static const char data_4C8AF[10] __attribute__((scn_string_literal)) = "E1020B065";
static const char data_4C8B9[10] __attribute__((scn_string_literal)) = "E1020B066";
static const char data_4C8C3[10] __attribute__((scn_string_literal)) = "E1020B067";
static const char data_4C8CD[10] __attribute__((scn_string_literal)) = "E1020B068";
static const char data_4C8D7[10] __attribute__((scn_string_literal)) = "E1020B069";
static const char data_4C8E1[10] __attribute__((scn_string_literal)) = "E1020B070";
static const char data_4C8EB[10] __attribute__((scn_string_literal)) = "E1020B071";
static const char data_4C8F5[10] __attribute__((scn_string_literal)) = "E1020B072";
static const char data_4C8FF[10] __attribute__((scn_string_literal)) = "E1020B073";
static const char data_4C909[10] __attribute__((scn_string_literal)) = "E1020B074";
static const char data_4C913[10] __attribute__((scn_string_literal)) = "E1020B090";
static const char data_4C91D[10] __attribute__((scn_string_literal)) = "E1020B091";
static const char data_4C927[10] __attribute__((scn_string_literal)) = "E1020B092";
static const char data_4C931[10] __attribute__((scn_string_literal)) = "E1020A008";
static const char data_4C93B[10] __attribute__((scn_string_literal)) = "E1020A018";
static const char data_4C945[10] __attribute__((scn_string_literal)) = "E1020B056";
static const char data_4C94F[10] __attribute__((scn_string_literal)) = "E1020B093";
static const char data_4C959[10] __attribute__((scn_string_literal)) = "E1020A019";
static const char data_4C963[10] __attribute__((scn_string_literal)) = "E1020B004";
static const char data_4C96D[10] __attribute__((scn_string_literal)) = "E1020B094";
static const char data_4C977[10] __attribute__((scn_string_literal)) = "E1020A010";
static const char data_4C981[10] __attribute__((scn_string_literal)) = "E1020A018";
static const char data_4C98B[10] __attribute__((scn_string_literal)) = "E1020B056";
static const char data_4C995[10] __attribute__((scn_string_literal)) = "E1020B057";
static const char data_4C99F[10] __attribute__((scn_string_literal)) = "E1020B058";
static const char data_4C9A9[10] __attribute__((scn_string_literal)) = "E1020B090";
static const char data_4C9B3[10] __attribute__((scn_string_literal)) = "E1020B098";
static const char data_4C9BD[10] __attribute__((scn_string_literal)) = "E1020A010";
static const char data_4C9C7[10] __attribute__((scn_string_literal)) = "E1020A018";
static const char data_4C9D1[10] __attribute__((scn_string_literal)) = "E1020B099";
static const char data_4C9DB[10] __attribute__((scn_string_literal)) = "E1020B103";
static const char data_4C9E5[10] __attribute__((scn_string_literal)) = "E1020A010";
static const char data_4C9EF[10] __attribute__((scn_string_literal)) = "E1020A018";
static const char data_4C9F9[10] __attribute__((scn_string_literal)) = "E1020B004";
static const char data_4CA03[10] __attribute__((scn_string_literal)) = "E1020B104";
static const char data_4CA0D[10] __attribute__((scn_string_literal)) = "E1020B058";
static const char data_4CA17[10] __attribute__((scn_string_literal)) = "E1020A021";
static const char data_4CA21[10] __attribute__((scn_string_literal)) = "E1020B026";
static const char data_4CA2B[10] __attribute__((scn_string_literal)) = "E1020B027";
static const char data_4CA35[10] __attribute__((scn_string_literal)) = "E1020B028";
static const char data_4CA3F[10] __attribute__((scn_string_literal)) = "E1020B029";
static const char data_4CA49[10] __attribute__((scn_string_literal)) = "E1020A024";
static const char data_4CA53[10] __attribute__((scn_string_literal)) = "E1020B004";
static const char data_4CA5D[10] __attribute__((scn_string_literal)) = "E1020B107";
static const char data_4CA67[10] __attribute__((scn_string_literal)) = "E1020B108";
static const char data_4CA71[10] __attribute__((scn_string_literal)) = "E1020A010";
static const char data_4CA7B[10] __attribute__((scn_string_literal)) = "E1020B075";
static const char data_4CA85[51] = "E1020B081\000\000\001\000\000\000\000\000$B\000\000\320A\000\000\200?\000\000\003\000\001\000\000\000\315\314\364A\000\000\210A\000\000\200?\000\000\003";
static const char data_4CAB8[10] __attribute__((scn_string_literal)) = "E1065A001";
static const char data_4CAC2[10] __attribute__((scn_string_literal)) = "E1065A002";
static const char data_4CACC[10] __attribute__((scn_string_literal)) = "E1065A003";
static const char data_4CAD6[10] __attribute__((scn_string_literal)) = "E1065A004";
static const char data_4CAE0[10] __attribute__((scn_string_literal)) = "E1065B001";
static const char data_4CAEA[10] __attribute__((scn_string_literal)) = "E1065B002";
static const char data_4CAF4[10] __attribute__((scn_string_literal)) = "E1065B003";
static const char data_4CAFE[10] __attribute__((scn_string_literal)) = "E1065B004";
static const char data_4CB08[10] __attribute__((scn_string_literal)) = "E1065B005";
static const char data_4CB12[10] __attribute__((scn_string_literal)) = "E1065B006";
static const char data_4CB1C[10] __attribute__((scn_string_literal)) = "E1065B007";
static const char data_4CB26[10] __attribute__((scn_string_literal)) = "E1065B008";
static const char data_4CB30[10] __attribute__((scn_string_literal)) = "E1065C001";
static const char data_4CB3A[10] __attribute__((scn_string_literal)) = "E1065C002";
static const char data_4CB44[10] __attribute__((scn_string_literal)) = "E1065C003";
static const char data_4CB4E[10] __attribute__((scn_string_literal)) = "E1065C004";
static const char data_4CB58[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_4CB5D[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_4CB62[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_4CB67[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_4CB6C[11] __attribute__((scn_string_literal)) = "bgm013.snd";
static const char data_4CB77[12] __attribute__((scn_string_literal)) = "e1enoki.snd";
static const char data_4CB83[13] __attribute__((scn_string_literal)) = "battle_1.snd";
static const char data_4CB90[] __attribute__((scn_string_literal)) = "/scene/01/D000/";
static const char data_4CBA0[] __attribute__((scn_string_literal)) = "DEFB";
static const char data_4CBA5[6] __attribute__((scn_string_literal)) = "E1065";
static const char data_4CBAB[5] __attribute__((scn_string_literal)) = "DEFB";
static const char data_4CBB0[8] __attribute__((scn_string_literal)) = "E1065";
static const Vector3 data_4CBB8 = {.x = 0.0f, .y = 5.25f, .z = 0.0f};
static const Vector3 data_4CBC4 = {.x = 0.0f, .y = 5.25f, .z = 0.0f};
static const Vector3 data_4CBD0 = {.x = 50.7f, .y = 5.25f, .z = 92.4f};
static const Vector3i data_4CBDC = {.x = 0, .y = 0, .z = 0};
static const float data_4CBE8[3] = {36.0f, 0.0724f, 19.69f};
static const char data_4CBF4[36] = "M_WARU.BIN\000\000\000\000 B\012\327\243=\000\000\240A";
static const Vector3 data_4CC18 = {.x = 0.0f, .y = 5.25f, .z = 0.0f};
static const int32_t data_4CC24[20] = {0, 5461, 0, 0, 56433, 0, 54613, 54667, 0, 54613, 0, 5461, 910, 0, 0, 0, 364, 0, 0, 0};
static const int32_t data_4CC74[10] = {0, 3640, 0, 0, 60074, 0, 60074, 61895, 0, 54613};
static const Vector3 data_4CC9C = {.x = 0.0f, .y = 5.25f, .z = 0.0f};
static Vector3i data_4CCA8 = {.x = 0, .y = -910, .z = 0};
static const uint8_t data_4CCB4[20] = {0};
static const int32_t data_4CCC8[9] = {-21173, 0, -1, 0, 0, 0, 0, 0, 0};
static const uint32_t data_4CCEC = 1;
static const int32_t data_4CCF0[13] = {0, 0, 0, 0, 16384, 0, -1, 0, 0, 0, 0, 0, 0};
static const uint32_t data_4CD24 = 2;
static const int32_t data_4CD28[13] = {0, 0, 0, 0, -32768, 0, -1, 0, 0, 0, 0, 0, 0};
static const uint32_t data_4CD5C = 3;
static const int32_t data_4CD60[13] = {0, 0, 0, 0, -4952, 0, -1, 0, 0, 0, 0, 0, 0};
static const uint32_t data_4CD94 = 4;
static const int32_t data_4CD98[13] = {0, 0, 0, 0, 32768, 0, -1, 0, 0, 0, 0, 0, 0};
static const uint32_t data_4CDCC = 5;
static const int32_t data_4CDD0[13] = {0, 0, 0, 0, 25675, 0, -1, 0, 0, 0, 0, 0, 0};
static const uint32_t data_4CE04 = 6;
static const int32_t data_4CE08[13] = {0, 0, 0, 0, 41320, 0, -1, 0, 0, 0, 0, 0, 0};
static const uint32_t data_4CE3C = 7;
static const int32_t data_4CE40[13] = {0, 0, 0, 0, -23276, 0, -1, 0, 0, 0, 0, 0, 0};
static char data_4CE74[] = "$B01MP01G.MT5";
static const char str_4CE82[] __attribute__((scn_string_literal)) = "$B01MP02G.MT5";
static const char str_4CE90[] __attribute__((scn_string_literal)) = "$B01MP03G.MT5";
static const char str_4CE9E[] __attribute__((scn_string_literal)) = "$BSPK3T1G.MT5";
static const char str_4CEAC[] __attribute__((scn_string_literal)) = "$B01MP04G.MT5";
static const char str_4CEBA[] __attribute__((scn_string_literal)) = "$B01MP05G.MT5";
static const char str_4CEC8[] __attribute__((scn_string_literal)) = "$B01MP06G.MT5";
static const char str_4CED6[] __attribute__((scn_string_literal)) = "$B01MP07G.MT5";
static const uint8_t pad_4CEE4[4] = {0};
static const uint32_t data_4CEE8 = 0xFFFF0000;
static const uint32_t data_4CEEC[3] = {0, 0x8000A7D2U, 0x4200914EU};
static const float data_4CEF8 = 1.1753f;
static const uint32_t data_4CEFC[3] = {0x41B0C49CU, 0, 0};
static const int32_t data_4CF08[2] = {-65536, 1};
static const uint32_t data_4CF10 = 0x80004000U;
static const float data_4CF14[5] = {15.3191f, 1.1753f, 43.8f, 0.0f, 0.0f};
static const int32_t data_4CF28[2] = {-65536, 2};
static const uint32_t data_4CF30 = 0x80007E93U;
static const float data_4CF34[5] = {-25.4825f, 1.1753f, 44.4692f, 0.0f, 0.0f};
static const int32_t data_4CF48[2] = {-65536, 3};
static const uint32_t data_4CF50[2] = {0x80008000U, 0xC27A2305U};
static const float data_4CF58 = 1.1753f;
static const uint32_t data_4CF5C[3] = {0x42B4B70AU, 0, 0};
static const int32_t data_4CF68[2] = {-65536, 4};
static const uint32_t data_4CF70 = 0x8000644BU;
static const float data_4CF74[2] = {6.5485f, 1.1753f};
static const uint32_t data_4CF7C[3] = {0x42B7D879U, 0, 0};
static const int32_t data_4CF88[2] = {-65536, 5};
static const uint32_t data_4CF90[2] = {0x8000A168U, 0xC2DCF74CU};
static const float data_4CF98[4] = {1.1753f, 76.7537f, 0.0f, 0.0f};
static const int32_t data_4CFA8[2] = {-65536, 6};
static const uint32_t data_4CFB0[2] = {0x8000A513U, 0xC2D5A6E9U};
static const float data_4CFB8 = 1.1753f;
static const uint32_t data_4CFBC[3] = {0x42CCDAEEU, 0, 0};
static const int32_t data_4CFC8[2] = {-65536, 7};
static const uint32_t data_4CFD0[2] = {0x8000E71CU, 0x422A2E98U};
static const float data_4CFD8[4] = {1.8f, 5.1289f, 0.0f, 0.0f};
static const int32_t data_4CFE8[2] = {-65536, 8};
static const uint32_t data_4CFF0[2] = {0x8000E71CU, 0x422A2E98U};
static const float data_4CFF8[4] = {1.2768f, 5.1289f, 0.0f, 0.0f};
static const int32_t data_4D008[2] = {-65536, 133};
static const uint32_t data_4D010[2] = {0x8000B1C7U, 0x421A21CBU};
static const float data_4D018[5] = {1.1f, 9.521f, 0.0f, 0.0f, 0.0f};
static const shen_name data_4D02C = SHEN_NAME('TBK1');
static const uint32_t data_4D030[2] = {0x8000B1C7U, 0x38CB6C7AU};
static const float data_4D038[4] = {0.130716f, 0.002221f, 0.0f, 0.0f};
static const int32_t data_4D048[2] = {-65536, 100};
static const uint32_t data_4D050 = 0x8000AB86U;
static const float data_4D054[5] = {37.43f, 1.58f, 10.49f, 0.0f, 0.0f};
static const int32_t data_4D068[2] = {-65536, 101};
static const uint32_t data_4D070 = 0x80002DDDU;
static const float data_4D074[5] = {29.09f, 1.43f, 10.9f, 0.0f, 0.0f};
static const int32_t data_4D088[2] = {-65536, 102};
static const uint32_t data_4D090 = 0x80002CFAU;
static const float data_4D094[5] = {21.93f, 1.5f, 25.05f, 0.0f, 0.0f};
static const int32_t data_4D0A8[2] = {-65536, 103};
static const uint32_t data_4D0B0 = 0x80008003U;
static const float data_4D0B4[5] = {20.56f, 1.62f, 32.69f, 0.0f, 0.0f};
static const int32_t data_4D0C8[2] = {-65536, 104};
static const uint32_t data_4D0D0 = 0x8000FE87U;
static const float data_4D0D4[2] = {2.9916f, 1.0886f};
static const uint32_t data_4D0DC[3] = {0x41D40866U, 0, 0};
static const int32_t data_4D0E8[2] = {-65536, 105};
static const uint32_t data_4D0F0 = 0x80007F34U;
static const float data_4D0F4[5] = {3.53f, 1.2f, 32.65f, 0.0f, 0.0f};
static const int32_t data_4D108[2] = {-65536, 106};
static const uint32_t data_4D110 = 0x8000FD8BU;
static const float data_4D114[5] = {-8.4f, 0.8f, 25.8f, 0.0f, 0.0f};
static const int32_t data_4D128[2] = {-65536, 107};
static const uint32_t data_4D130 = 0x80001A7EU;
static const float data_4D134[5] = {-15.36f, 1.08f, 28.93f, 0.0f, 0.0f};
static const int32_t data_4D148[2] = {-65536, 108};
static const uint32_t data_4D150 = 0x8000196DU;
static const float data_4D154[5] = {-19.24f, 1.24f, 32.36f, 0.0f, 0.0f};
static const int32_t data_4D168[2] = {-65536, 109};
static const uint32_t data_4D170 = 0x800016CAU;
static const float data_4D174[5] = {-53.15f, 1.33f, 41.13f, 0.0f, 0.0f};
static const int32_t data_4D188[2] = {-65536, 110};
static const uint32_t data_4D190 = 0x80009AC2U;
static const float data_4D194[5] = {-51.9f, 1.41f, 50.03f, 0.0f, 0.0f};
static const int32_t data_4D1A8[2] = {-65536, 111};
static const uint32_t data_4D1B0 = 0x80001930U;
static const float data_4D1B4[5] = {-58.06f, 0.96f, 45.03f, 0.0f, 0.0f};
static const int32_t data_4D1C8[2] = {-65536, 112};
static const uint32_t data_4D1D0 = 0x8000C0A7U;
static const float data_4D1D4[5] = {-63.29f, 1.56f, 66.86f, 0.0f, 0.0f};
static const int32_t data_4D1E8[2] = {-65536, 113};
static const uint32_t data_4D1F0[2] = {0x8000C2C5U, 0xC27CB4F1U};
static const float data_4D1F8 = 1.6068f;
static const uint32_t data_4D1FC[3] = {0x428F2EA5U, 0, 0};
static const int32_t data_4D208[2] = {-65536, 114};
static const uint32_t data_4D210 = 0x80008000U;
static const float data_4D214[5] = {-69.82f, 1.75f, 91.66f, 0.0f, 0.0f};
static const int32_t data_4D228[2] = {-65536, 115};
static const uint32_t data_4D230 = 0x8000148EU;
static const float data_4D234[5] = {-54.42f, 1.56f, 81.22f, 0.0f, 0.0f};
static const int32_t data_4D248[2] = {-65536, 116};
static const uint32_t data_4D250 = 0x80000C73U;
static const float data_4D254[5] = {-41.95f, 1.19f, 74.58f, 0.0f, 0.0f};
static const int32_t data_4D268[2] = {-65536, 117};
static const uint32_t data_4D270 = 0x8000FE68U;
static const float data_4D274[5] = {-29.54f, 1.04f, 72.51f, 0.0f, 0.0f};
static const int32_t data_4D288[2] = {-65536, 118};
static const uint32_t data_4D290 = 0x8000E21AU;
static const float data_4D294[5] = {6.33f, 1.59f, 84.17f, 0.0f, 0.0f};
static const int32_t data_4D2A8[2] = {-65536, 119};
static const uint32_t data_4D2B0 = 0x80000C96U;
static const float data_4D2B4[5] = {-93.08f, 1.53f, 61.81f, 0.0f, 0.0f};
static const int32_t data_4D2C8[2] = {-65536, 120};
static const uint32_t data_4D2D0[2] = {0x800091EAU, 0xC2BC7909U};
static const float data_4D2D8 = 1.693f;
static const uint32_t data_4D2DC[3] = {0x42854DACU, 0, 0};
static const int32_t data_4D2E8[2] = {-65536, 121};
static const uint32_t data_4D2F0 = 0x800011C7U;
static const float data_4D2F4[5] = {-99.52f, 1.7f, 65.19f, 0.0f, 0.0f};
static const int32_t data_4D308[2] = {-65536, 122};
static const uint32_t data_4D310 = 0x800013A4U;
static const float data_4D314[5] = {-111.81f, 1.61f, 69.88f, 0.0f, 0.0f};
static const int32_t data_4D328[2] = {-65536, 123};
static const uint32_t data_4D330 = 0x80006046U;
static const float data_4D334[5] = {-126.7f, -0.13f, 64.72f, 0.0f, 0.0f};
static const int32_t data_4D348[2] = {-65536, 124};
static const uint32_t data_4D350 = 0x800047B5U;
static const float data_4D354[5] = {-124.49f, 1.58f, 87.91f, 0.0f, 0.0f};
static const int32_t data_4D368[2] = {-65536, 125};
static const uint32_t data_4D370 = 0x80004A32U;
static const float data_4D374[5] = {-123.44f, 1.23f, 94.43f, 0.0f, 0.0f};
static const int32_t data_4D388[2] = {-65536, 126};
static const uint32_t data_4D390 = 0x80006F49U;
static const float data_4D394[5] = {-114.06f, 1.31f, 101.77f, 0.0f, 0.0f};
static const int32_t data_4D3A8[2] = {-65536, 127};
static const uint32_t data_4D3B0 = 0x80006D47U;
static const float data_4D3B4[5] = {-109.53f, 1.32f, 102.17f, 0.0f, 0.0f};
static const int32_t data_4D3C8[2] = {-65536, 128};
static const uint32_t data_4D3D0 = 0x800026A0U;
static const float data_4D3D4[5] = {-106.34f, 1.64f, 93.77f, 0.0f, 0.0f};
static const int32_t data_4D3E8[2] = {-65536, 129};
static const uint32_t data_4D3F0 = 0x80001418U;
static const float data_4D3F4[5] = {-94.02f, 1.69f, 83.69f, 0.0f, 0.0f};
static const int32_t data_4D408[2] = {-65536, 130};
static const uint32_t data_4D410 = 0x80003BB6U;
static const float data_4D414[6] = {-69.96f, 1.88f, 79.5f, 0.0f, 0.0f, 0.0f};
static const shen_name data_4D42C = SHEN_NAME('GCH0');
static const float data_4D430[7] = {40064.0f, 0.0f, 0.76f, 0.055f, 0.0f, 0.0f, 0.0f};
static const shen_name data_4D44C = SHEN_NAME('GCH1');
static const float data_4D450[7] = {40064.0f, 0.0f, 0.76f, 0.055f, 0.0f, 0.0f, 0.0f};
static const shen_name data_4D46C = SHEN_NAME('GCH2');
static const float data_4D470[7] = {40064.0f, 0.0f, 0.76f, 0.055f, 0.0f, 0.0f, 0.0f};
static const shen_name data_4D48C = SHEN_NAME('GCH3');
static const float data_4D490[7] = {40064.0f, 0.0f, 0.76f, 0.055f, 0.0f, 0.0f, 0.0f};
static const shen_name data_4D4AC = SHEN_NAME('GCH4');
static const float data_4D4B0[7] = {40064.0f, 0.0f, 0.76f, 0.055f, 0.0f, 0.0f, 0.0f};
static const shen_name data_4D4CC = SHEN_NAME('GCH5');
static const float data_4D4D0[6] = {40064.0f, 0.0f, 0.76f, 0.055f, 0.0f, 0.0f};
static const int32_t data_4D4E8[2] = {-65536, 131};
static const uint32_t data_4D4F0 = 0x8000C367U;
static const float data_4D4F4[5] = {20.06f, 1.61f, 47.94f, 0.0f, 0.0f};
static const int32_t data_4D508[2] = {-65536, 132};
static const uint32_t data_4D510[2] = {0x800024C5U, 0xC2F61C1CU};
static const float data_4D518 = 1.2514f;
static const uint32_t data_4D51C[3] = {0x42A6B8BBU, 0, 0};
static const int32_t data_4D528[2] = {-65535, 134};
static const uint32_t data_4D530 = 0x71C799F4U;
static const float data_4D534[2] = {-18.28f, 1.17f};
static const uint32_t data_4D53C = 0x421A0F5CU;
static const int32_t data_4D540 = -1;
static uint32_t data_4D544 = 0x4200914E;
static const float data_4D548 = 1.1753f;
static const uint32_t data_4D54C = 0x41B0C49CU;
static uint32_t data_4D550 = 0x41751B09;
static const float data_4D554[2] = {1.1753f, 43.8f};
static uint32_t data_4D55C = 0xC1CBDC29;
static const float data_4D560[2] = {1.1753f, 44.472f};
static uint32_t data_4D568 = 0xC27A2305;
static const float data_4D56C = 1.1753f;
static const uint32_t data_4D570 = 0x42B4B70AU;
static uint32_t data_4D574 = 0x40D1851F;
static const float data_4D578 = 1.1753f;
static const uint32_t data_4D57C = 0x42B7D7F6U;
static uint32_t data_4D580 = 0xC2DCF7CF;
static const float data_4D584 = 1.1753f;
static const uint32_t data_4D588 = 0x429981D8U;
static uint32_t data_4D58C = 0xC2D5A76D;
static const float data_4D590 = 1.1753f;
static const uint32_t data_4D594 = 0x42CCDAE1U;
static const Vector3 data_4D598 = {.x = 37.026901f, .y = 0.1227f, .z = 9.716f};
static const char data_4D5A4[11] __attribute__((scn_string_literal)) = "bgm029.snd";
static const char data_4D5AF[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_4D5B4[12] __attribute__((scn_string_literal)) = "a1_telp.snd";
static const char data_4D5C0[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_4D5C5[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_4D5CA[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_4D5CF[] __attribute__((scn_string_literal)) = "/scene/01/D000/";
static const char data_4D5DF[] __attribute__((scn_string_literal)) = "AUTH";
static const char data_4D5E4[13] __attribute__((scn_string_literal)) = "seqdata0.bin";
static const char data_4D5F1[5] __attribute__((scn_string_literal)) = "AUTH";
static const char data_4D5F6[6] __attribute__((scn_string_literal)) = "01TEL";
static const char data_4D5FC[5] __attribute__((scn_string_literal)) = "AUTH";
static const char data_4D601[7] __attribute__((scn_string_literal)) = "01TEL";
static uint32_t data_4D608 = 0x422A2E98;
static const float data_4D60C[2] = {1.2768f, 5.1289f};
static const int32_t data_4D614[57] = {
  0, 0, -6003, 0, 0, -908, 0, 0, 0, 0, 0, -3819, 0, 0, -7094, 0, 0, -4547, 0, 0, 0, 0, 0, -2909, 0, 0, -8550, 0, -544, -4001, 0, 0, 0, 0, 0, -4363, 0,
  0, -8004, 0, -908, -2180, 0, 0, 0, 0, -1634, -6366, 0, -2000, -4365, 0, -1818, -2182, 0, 0, 0
};
static const int32_t data_4D6F8[57] = {
  -3820, -1636, -4548, 6370, 4186, 4914, -9646, -364, 2730, 0, 0, -5641, 0, 1273, -9826, 0, 0, -6368, 0, 0, 2911, 0, 0, -1819, 0, -363, -12374, 0, 0,
  -6369, 0, 0, 2184, 0, 0, -4185, 0, 0, -11646, 0, -1273, -4913, 0, 0, 2184, 0, 0, -7825, 0, -3821, -8227, 0, -727, -6733, 0, 0, 2911
};
static const int32_t data_4D7DC[57] = {
  0, 0, -6002, 0, 3276, -907, 0, 3458, 546, 0, 0, -3819, 0, 0, -5091, 0, 2184, -8550, 0, -545, 546, 0, 0, -2908, 0, 0, -6911, 0, -543, -8733, 0, 0,
  727, 0, 0, -4362, 0, 0, -6183, 0, -907, -7275, 0, 0, 0, 0, -1633, -9095, 0, -1999, -4000, 0, -2727, -9461, 0, 0, 1637
};
static const int32_t data_4D8C0[57] = {
  2911, 0, -4910, 2366, -181, -3637, 5641, -363, 910, 0, 0, -1634, 0, 0, -7093, 0, 0, -1088, 0, 0, 0, 0, 0, -2908, 0, 0, -7093, 0, -543, 1095, 0, 0,
  0, 0, 0, -3270, 0, 0, -8004, 0, -907, 2916, 0, 0, 0, 0, -1633, -3818, 0, -1817, -4364, 0, -543, 2367, 0, 0, -181
};
static const char data_4D9A4[] __attribute__((scn_string_literal)) = "/scene/01/D000/";
static const char data_4D9B4[] __attribute__((scn_string_literal)) = "DESA";
static const char data_4D9B9[7] __attribute__((scn_string_literal)) = "SA1071";
static const char data_4D9C0[11] __attribute__((scn_string_literal)) = "SA1071A001";
static const char data_4D9CB[11] __attribute__((scn_string_literal)) = "SA1071A002";
static const char data_4D9D6[11] __attribute__((scn_string_literal)) = "SA1071A003";
static const char data_4D9E1[11] __attribute__((scn_string_literal)) = "SA1071A004";
static const char data_4D9EC[5] __attribute__((scn_string_literal)) = "DESA";
static const char data_4D9F1[7] __attribute__((scn_string_literal)) = "SA1071";
static Vector3i data_4D9F8 = {.x = 0, .y = 14108, .z = 0};
static Vector3i data_4DA04 = {.x = 0, .y = 30474, .z = 0};
static const float data_4DA10[3] = {1.62f, 1.0f, 3.4f};
static Vector3i data_4DA1C = {.x = 0, .y = 14108, .z = 0};
static Vector3i data_4DA28 = {.x = 0, .y = -30474, .z = 0};
static Vector3i data_4DA34 = {.x = 0, .y = -14108, .z = 0};
static Vector3i data_4DA40 = {.x = 0, .y = 30474, .z = 0};
static const Vector3 data_4DA4C = {.x = 3.1f, .y = 2.65f, .z = 8.0f};
static const Vector4 data_4DA58 = {.x = 1.0f, .y = 0.4f, .z = 0.4f, .w = 0.4f};
static Vector3i data_4DA68 = {.x = 2730, .y = 0, .z = 0};
static Vector3i data_4DA74 = {.x = -2730, .y = 0, .z = 0};
static const char data_4DA80[11] __attribute__((scn_string_literal)) = "SA1076A001";
static const char data_4DA8B[11] __attribute__((scn_string_literal)) = "SA1076A013";
static const char data_4DA96[11] __attribute__((scn_string_literal)) = "SA1076A002";
static const char data_4DAA1[11] __attribute__((scn_string_literal)) = "SA1076A014";
static const char data_4DAAC[11] __attribute__((scn_string_literal)) = "SA1076A003";
static const char data_4DAB7[11] __attribute__((scn_string_literal)) = "SA1076A015";
static const char data_4DAC2[11] __attribute__((scn_string_literal)) = "SA1076A015";
static const char data_4DACD[11] __attribute__((scn_string_literal)) = "SA1076A016";
static const char data_4DAD8[12] __attribute__((scn_string_literal)) = "SA1076A017";
static const Vector3 data_4DAE4 = {.x = 42.199902f, .y = 0.0f, .z = 5.9f};
static const Vector3 data_4DAF0 = {.x = 42.599899f, .y = 0.0f, .z = 6.8f};
static const Vector3 data_4DAFC = {.x = 43.4921f, .y = 0.0f, .z = 5.1527f};
static const char data_4DB08[] __attribute__((scn_string_literal)) = "BUSS";
static const char data_4DB0D[13] __attribute__((scn_string_literal)) = "a1_busno.snd";
static const char data_4DB1A[] __attribute__((scn_string_literal)) = "/scene/01/D000/";
static const char data_4DB2A[] __attribute__((scn_string_literal)) = "BUSS";
static const char data_4DB2F[13] __attribute__((scn_string_literal)) = "seqdata1.bin";
static const char data_4DB3C[5] __attribute__((scn_string_literal)) = "BUSS";
static const char data_4DB41[13] __attribute__((scn_string_literal)) = "seqdata4.bin";
static const char data_4DB4E[5] __attribute__((scn_string_literal)) = "BUSS";
static const char data_4DB53[13] __attribute__((scn_string_literal)) = "a1_busno.snd";
static const char data_4DB60[] __attribute__((scn_string_literal)) = "/scene/01/D000/";
static const char data_4DB70[] __attribute__((scn_string_literal)) = "BUSS";
static const char data_4DB75[13] __attribute__((scn_string_literal)) = "seqdata2.bin";
static const char data_4DB82[5] __attribute__((scn_string_literal)) = "BUSS";
static const char data_4DB87[13] __attribute__((scn_string_literal)) = "seqdata5.bin";
static const char data_4DB94[8] __attribute__((scn_string_literal)) = "BUSS";
static const Vector4 data_4DB9C = {.x = 0.3f, .y = 0.7f, .z = 0.7f, .w = 0.7f};
static const int32_t data_4DBAC[57] = {
  0, 0, -4004, 0, 0, -546, 0, 364, -1638, 0, 0, -7827, 0, 0, -11468, 0, 0, -3822, 0, 0, 0, 0, 0, -8192, 0, 0, -18022, 0, 0, -3276, 0, 0, 0, 0, 0,
  -7827, 0, 0, -18022, 0, 0, -5279, 0, 0, 0, 0, 910, -12014, 0, -2184, -16748, 0, -2002, -8738, 0, 0, 364
};
static const char data_4DC90[6] __attribute__((scn_string_literal)) = "YKB_M";
static const char data_4DC96[7] __attribute__((scn_string_literal)) = "SA1012";
static const char data_4DC9D[11] __attribute__((scn_string_literal)) = "SA1012A005";
static const char data_4DCA8[11] __attribute__((scn_string_literal)) = "SA1012A002";
static const char data_4DCB3[11] __attribute__((scn_string_literal)) = "SA1012A005";
static const char data_4DCBE[11] __attribute__((scn_string_literal)) = "SA1012A006";
static const char data_4DCC9[11] __attribute__((scn_string_literal)) = "SA1012A005";
static const char data_4DCD4[11] __attribute__((scn_string_literal)) = "SA1012A002";
static const char data_4DCDF[11] __attribute__((scn_string_literal)) = "SA1012A005";
static const char data_4DCEA[7] __attribute__((scn_string_literal)) = "SA1012";
static const char data_4DCF1[7] __attribute__((scn_string_literal)) = "SA1007";
static const char data_4DCF8[11] __attribute__((scn_string_literal)) = "SA1007A001";
static const char data_4DD03[11] __attribute__((scn_string_literal)) = "SA1007B001";
static const char data_4DD0E[11] __attribute__((scn_string_literal)) = "SA1007B002";
static const char data_4DD19[11] __attribute__((scn_string_literal)) = "SA1007A002";
static const char data_4DD24[11] __attribute__((scn_string_literal)) = "SA1007B003";
static const char data_4DD2F[11] __attribute__((scn_string_literal)) = "SA1007A003";
static const char data_4DD3A[11] __attribute__((scn_string_literal)) = "SA1007B004";
static const char data_4DD45[11] __attribute__((scn_string_literal)) = "SA1007B006";
static const char data_4DD50[11] __attribute__((scn_string_literal)) = "SA1007A004";
static const char data_4DD5B[11] __attribute__((scn_string_literal)) = "SA1007B003";
static const char data_4DD66[11] __attribute__((scn_string_literal)) = "SA1007A003";
static const char data_4DD71[11] __attribute__((scn_string_literal)) = "SA1007B004";
static const char data_4DD7C[11] __attribute__((scn_string_literal)) = "SA1007B006";
static const char data_4DD87[11] __attribute__((scn_string_literal)) = "SA1007A004";
static const char data_4DD92[7] __attribute__((scn_string_literal)) = "SA1007";
static const char data_4DD99[7] __attribute__((scn_string_literal)) = "SA1009";
static const char data_4DDA0[11] __attribute__((scn_string_literal)) = "SA1009B006";
static const char data_4DDAB[11] __attribute__((scn_string_literal)) = "SA1009B001";
static const char data_4DDB6[11] __attribute__((scn_string_literal)) = "SA1009A005";
static const char data_4DDC1[11] __attribute__((scn_string_literal)) = "SA1009B005";
static const char data_4DDCC[11] __attribute__((scn_string_literal)) = "SA1009B004";
static const char data_4DDD7[11] __attribute__((scn_string_literal)) = "SA1009A004";
static const char data_4DDE2[11] __attribute__((scn_string_literal)) = "SA1009B001";
static const char data_4DDED[11] __attribute__((scn_string_literal)) = "SA1009A001";
static const char data_4DDF8[11] __attribute__((scn_string_literal)) = "SA1009A002";
static const char data_4DE03[11] __attribute__((scn_string_literal)) = "SA1009B002";
static const char data_4DE0E[11] __attribute__((scn_string_literal)) = "SA1009B003";
static const char data_4DE19[11] __attribute__((scn_string_literal)) = "SA1009A003";
static const char data_4DE24[11] __attribute__((scn_string_literal)) = "SA1009B004";
static const char data_4DE2F[11] __attribute__((scn_string_literal)) = "SA1009A004";
static const char data_4DE3A[7] __attribute__((scn_string_literal)) = "SA1009";
static const char data_4DE41[7] __attribute__((scn_string_literal)) = "SA1083";
static const char data_4DE48[11] __attribute__((scn_string_literal)) = "SA1083B001";
static const char data_4DE53[11] __attribute__((scn_string_literal)) = "SA1083B002";
static const char data_4DE5E[11] __attribute__((scn_string_literal)) = "SA1083A001";
static const char data_4DE69[11] __attribute__((scn_string_literal)) = "SA1083B003";
static const char data_4DE74[11] __attribute__((scn_string_literal)) = "SA1083B004";
static const char data_4DE7F[11] __attribute__((scn_string_literal)) = "SA1083B005";
static const char data_4DE8A[11] __attribute__((scn_string_literal)) = "SA1083A001";
static const char data_4DE95[11] __attribute__((scn_string_literal)) = "SA1083B006";
static const char data_4DEA0[11] __attribute__((scn_string_literal)) = "SA1083B007";
static const char data_4DEAB[11] __attribute__((scn_string_literal)) = "SA1083B008";
static const char data_4DEB6[11] __attribute__((scn_string_literal)) = "SA1083A001";
static const char data_4DEC1[11] __attribute__((scn_string_literal)) = "SA1083B009";
static const char data_4DECC[11] __attribute__((scn_string_literal)) = "SA1083B010";
static const char data_4DED7[7] __attribute__((scn_string_literal)) = "SA1083";
static const char data_4DEDE[7] __attribute__((scn_string_literal)) = "SA1084";
static const char data_4DEE5[11] __attribute__((scn_string_literal)) = "SA1084B001";
static const char data_4DEF0[11] __attribute__((scn_string_literal)) = "SA1084B002";
static const char data_4DEFB[11] __attribute__((scn_string_literal)) = "SA1084A001";
static const char data_4DF06[11] __attribute__((scn_string_literal)) = "SA1084B003";
static const char data_4DF11[11] __attribute__((scn_string_literal)) = "SA1084B004";
static const char data_4DF1C[11] __attribute__((scn_string_literal)) = "SA1084B005";
static const char data_4DF27[11] __attribute__((scn_string_literal)) = "SA1084A001";
static const char data_4DF32[11] __attribute__((scn_string_literal)) = "SA1084B006";
static const char data_4DF3D[11] __attribute__((scn_string_literal)) = "SA1084B007";
static const char data_4DF48[11] __attribute__((scn_string_literal)) = "SA1084B008";
static const char data_4DF53[11] __attribute__((scn_string_literal)) = "SA1084A001";
static const char data_4DF5E[11] __attribute__((scn_string_literal)) = "SA1084B009";
static const char data_4DF69[11] __attribute__((scn_string_literal)) = "SA1084B010";
static const char data_4DF74[7] __attribute__((scn_string_literal)) = "SA1084";
static const char data_4DF7B[11] __attribute__((scn_string_literal)) = "SA1080A003";
static const char data_4DF86[11] __attribute__((scn_string_literal)) = "SA1080A002";
static const char data_4DF91[11] __attribute__((scn_string_literal)) = "SA1080A004";
static const char data_4DF9C[11] __attribute__((scn_string_literal)) = "SA1080A005";
static const char data_4DFA7[11] __attribute__((scn_string_literal)) = "SA1080A004";
static const char data_4DFB2[11] __attribute__((scn_string_literal)) = "SA1080A006";
static const char data_4DFBD[7] __attribute__((scn_string_literal)) = "SA1080";
static const char data_4DFC4[7] __attribute__((scn_string_literal)) = "SA1081";
static const char data_4DFCB[11] __attribute__((scn_string_literal)) = "SA1081A002";
static const char data_4DFD6[11] __attribute__((scn_string_literal)) = "SA1081A003";
static const char data_4DFE1[11] __attribute__((scn_string_literal)) = "SA1081A002";
static const char data_4DFEC[11] __attribute__((scn_string_literal)) = "SA1081A005";
static const char data_4DFF7[11] __attribute__((scn_string_literal)) = "SA1081A006";
static const char data_4E002[7] __attribute__((scn_string_literal)) = "SA1081";
static const char data_4E009[7] __attribute__((scn_string_literal)) = "SA1088";
static const char data_4E010[11] __attribute__((scn_string_literal)) = "SA1088A001";
static const char data_4E01B[11] __attribute__((scn_string_literal)) = "SA1088A002";
static const char data_4E026[11] __attribute__((scn_string_literal)) = "SA1088A003";
static const char data_4E031[11] __attribute__((scn_string_literal)) = "SA1088A002";
static const char data_4E03C[11] __attribute__((scn_string_literal)) = "SA1088A004";
static const char data_4E047[11] __attribute__((scn_string_literal)) = "SA1088A003";
static const char data_4E052[11] __attribute__((scn_string_literal)) = "SA1088A005";
static const char data_4E05D[11] __attribute__((scn_string_literal)) = "SA1088A002";
static const char data_4E068[11] __attribute__((scn_string_literal)) = "SA1088A003";
static const char data_4E073[9] __attribute__((scn_string_literal)) = "SA1088";
static const uint32_t data_4E07C = 0x4192A3D7;
static const float data_4E080 = 0.225f;
static const uint32_t data_4E084 = 0x42738CCDU;
static const int32_t data_4E088[3] = {32768, 1048832, 8};
static const uint32_t data_4E094 = 0x41935A1DU;
static const float data_4E098[3] = {1.8f, 62.9902f, 0.0f};
static const int32_t data_4E0A4[2] = {264, 8};
static const float data_4E0AC[2] = {19.5203f, 2.025f};
static const uint32_t data_4E0B4 = 0x42822113U;
static const int32_t data_4E0B8[3] = {39321, 1048832, 15};
static const float data_4E0C4[2] = {22.1514f, 5.4f};
static const uint32_t data_4E0CC = 0x4288AB6BU;
static const int32_t data_4E0D0[3] = {6553, 264, 15};
static const float data_4E0DC[2] = {-120.7995f, -1.573f};
static const uint32_t data_4E0E4 = 0x429CA0ECU;
static const int32_t data_4E0E8[3] = {42143, 1048832, 8};
static const uint32_t data_4E0F4[3] = {0xC2EE475FU, 0, 0x429F3A93U};
static const int32_t data_4E100[3] = {9375, 264, 8};
static const float data_4E10C[2] = {-101.9963f, 0.2967f};
static const uint32_t data_4E114 = 0x42C74241U;
static const int32_t data_4E118[3] = {25486, 1048576, 12};
static const uint32_t data_4E124 = 0xC2CFF724U;
static const float data_4E128 = 2.75f;
static const uint32_t data_4E12C = 0x42CC65A2U;
static const int32_t data_4E130[3] = {58254, 8, 12};
static const uint32_t data_4E13C = 0xC24CC20CU;
static const float data_4E140[2] = {0.296f, 97.4198f};
static const int32_t data_4E148[3] = {37501, 1048576, 12};
static const uint32_t data_4E154 = 0xC246BAADU;
static const float data_4E158 = 2.7716f;
static const uint32_t data_4E15C = 0x42C898EFU;
static const int32_t data_4E160[3] = {4642, 9, 12};
static const float data_4E16C[2] = {-32.9104f, 0.296f};
static const uint32_t data_4E174 = 0x42B06A4BU;
static const int32_t data_4E178[3] = {33314, 1048576, 12};
static const uint32_t data_4E184 = 0xC202E388U;
static const float data_4E188 = 2.7716f;
static const uint32_t data_4E18C = 0x42B701D8U;
static const int32_t data_4E190[3] = {546, 8, 12};
static const float data_4E19C[2] = {-7.6464f, 0.296f};
static const uint32_t data_4E1A4 = 0x4290DBF5U;
static const int32_t data_4E1A8[3] = {11468, 1048576, 12};
static const float data_4E1B4[3] = {-10.6061f, 2.7716f, 70.9646f};
static const int32_t data_4E1C0[3] = {44252, 8, 12};
static const float data_4E1CC[2] = {-6.245f, 0.296f};
static const uint32_t data_4E1D4 = 0x42BB1AA0U;
static const int32_t data_4E1D8[3] = {26594, 1048576, 12};
static const float data_4E1E4[2] = {-8.1412f, 2.7716f};
static const uint32_t data_4E1EC = 0x42C083F1U;
static const int32_t data_4E1F0[3] = {59346, 8, 12};
static const uint32_t data_4E1FC = 0x421AEF00U;
static const float data_4E200[2] = {5.4742f, 90.1437f};
static const int32_t data_4E208[3] = {24576, 1048576, 12};
static const float data_4E214[2] = {36.3997f, 7.9525f};
static const uint32_t data_4E21C = 0x42B8F893U;
static const int32_t data_4E220[3] = {57344, 8, 12};
static const float data_4E22C = 100000.0f;
static const uint8_t data_4E230[8] = {0};
typedef struct Row_4E238 { float w0; int32_t w1; int32_t w2; } Row_4E238;
static const Row_4E238 data_4E238[5] = {
  {1.0f, 0, 1},
  {1.0f, 0, 2},
  {1.0f, 0, 3},
  {1.0f, 0, 4},
  {0.5f, 0, 5}
};
static const float data_4E274 = 0.5f;
static const int32_t data_4E278 = 3;
static const char data_4E27C[34] __attribute__((scn_string_literal)) = "DOBUITA EVENT START START START!\012";
static const char data_4E29E[20] __attribute__((scn_string_literal)) = "DOBUITA EVENT END!\012";
static const char data_4E2B2[26] __attribute__((scn_string_literal)) = "training check error!!!!\012";
static const char data_4E2CC[67] __attribute__((scn_string_literal)) = "Daily practice %d: Check: in range, setting training flag to true\012";
static const char data_4E30F[109] __attribute__((scn_string_literal)) = "Daily practice %d: Check: in range, training flag already true ?? Failed before due to NPC's so check again\012";
static const char data_4E37C[31] __attribute__((scn_string_literal)) = "VEND_MACHINE_CHECKER_ERROR!!!\012";
static const char data_4E39B[9] __attribute__((scn_string_literal)) = "DR15_014";
static const char data_4E3A4[9] __attribute__((scn_string_literal)) = "DR15_010";
static const char data_4E3AD[9] __attribute__((scn_string_literal)) = "DR15_009";
static const char data_4E3B6[] __attribute__((scn_string_literal)) = "/scene/01/D000/";
static const char data_4E3C6[] __attribute__((scn_string_literal)) = "YQ14";
static const char data_4E3CB[13] __attribute__((scn_string_literal)) = "seqdata1.bin";
static const char data_4E3D8[5] __attribute__((scn_string_literal)) = "YQ14";
static const char data_4E3DD[13] __attribute__((scn_string_literal)) = "seqdata2.bin";
static const char data_4E3EA[5] __attribute__((scn_string_literal)) = "YQ14";
static const char data_4E3EF[5] __attribute__((scn_string_literal)) = "YQ14";
static const char data_4E3F4[11] __attribute__((scn_string_literal)) = "SA1086A001";
static const char data_4E3FF[11] __attribute__((scn_string_literal)) = "SA1086A002";
static const char data_4E40A[11] __attribute__((scn_string_literal)) = "SA1076A001";
static const char data_4E415[11] __attribute__((scn_string_literal)) = "SA1076A002";
static const char data_4E420[11] __attribute__((scn_string_literal)) = "SA1076A003";
static const char data_4E42B[7] __attribute__((scn_string_literal)) = "SA1076";
static const char data_4E432[11] __attribute__((scn_string_literal)) = "bgm018.snd";
static const char data_4E43D[12] __attribute__((scn_string_literal)) = "a1_yobi.snd";
static const char data_4E449[16] __attribute__((scn_string_literal)) = "/scene/01/D000/";
static const char data_4E459[5] __attribute__((scn_string_literal)) = "YBHN";
static const char data_4E45E[13] __attribute__((scn_string_literal)) = "seqdata0.bin";
static const char data_4E46B[5] __attribute__((scn_string_literal)) = "YBHN";
static const char data_4E470[9] __attribute__((scn_string_literal)) = "DR15_012";
static const char data_4E479[7] __attribute__((scn_string_literal)) = "YBHN";
static const uint32_t data_4E480 = 0xC1291EB8;
static const float data_4E484[386] = {
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
static const char str_4EA8C[] __attribute__((scn_string_literal)) = "ff>A=\n\"B";
static const uint8_t data_4EA95[16] = {
  0x00, 0xB8, 0x40, 0x14, 0x2E, 0xB7, 0x42, 0x00, 0x00, 0x00, 0x41, 0x7B, 0x14, 0xDE, 0x41, 0xA4
};
static const uint8_t data_4EAA5[16] = {
  0x70, 0xEF, 0x41, 0x5C, 0x8F, 0xD6, 0x41, 0x1F, 0x85, 0xEB, 0x41, 0xE1, 0x7A, 0xE0, 0x41, 0x71
};
static const uint8_t data_4EAB5[16] = {
  0x3D, 0xD8, 0x41, 0x00, 0x00, 0xE8, 0x41, 0x7B, 0x14, 0xDC, 0x41, 0xCD, 0xCC, 0xCC, 0xBD, 0x5C
};
static const uint8_t data_4EAC5[16] = {
  0x8F, 0x52, 0x40, 0xAE, 0x47, 0xDF, 0x41, 0x00, 0x00, 0x80, 0x3F, 0x0A, 0xD7, 0xE3, 0x41, 0x00
};
static const uint8_t data_4EAD5[16] = {
  0x00, 0x80, 0x3F, 0xAE, 0x47, 0x39, 0x41, 0x52, 0xB8, 0xCE, 0x41, 0xAE, 0x47, 0x39, 0x41, 0x33
};
static const uint8_t data_4EAE5[16] = {
  0x33, 0xD7, 0x41, 0x8F, 0xC2, 0x0D, 0x41, 0x33, 0x33, 0xD7, 0x41, 0x8F, 0xC2, 0x0D, 0x41, 0xD7
};
static const uint8_t data_4EAF5[16] = {
  0xA3, 0xCE, 0x41, 0xCD, 0xCC, 0xCC, 0xBD, 0x71, 0x3D, 0x5A, 0x40, 0x29, 0x5C, 0x23, 0x41, 0x00
};
static const uint8_t data_4EB05[16] = {
  0x00, 0x80, 0x3F, 0x48, 0xE1, 0xD2, 0x41, 0x00, 0x00, 0x80, 0x3F, 0x66, 0x66, 0x12, 0x41, 0x5C
};
static const uint8_t data_4EB15[16] = {
  0x8F, 0xFE, 0x41, 0x66, 0x66, 0x12, 0x41, 0xE1, 0x7A, 0x03, 0x42, 0xCD, 0xCC, 0xBC, 0x40, 0xE1
};
static const uint8_t data_4EB25[16] = {
  0x7A, 0x03, 0x42, 0xCD, 0xCC, 0xBC, 0x40, 0x5C, 0x8F, 0xFE, 0x41, 0xCD, 0xCC, 0xCC, 0xBD, 0x71
};
static const uint8_t data_4EB35[16] = {
  0x3D, 0x92, 0x40, 0xC3, 0xF5, 0xF0, 0x40, 0x00, 0x00, 0x80, 0x3F, 0x29, 0x5C, 0x01, 0x42, 0x00
};
static const uint8_t data_4EB45[16] = {
  0x00, 0x80, 0x3F, 0x8F, 0xC2, 0x75, 0x3F, 0xCD, 0xCC, 0xD0, 0x41, 0x8F, 0xC2, 0x75, 0x3F, 0xAE
};
static const uint8_t data_4EB55[16] = {
  0x47, 0xD5, 0x41, 0x52, 0xB8, 0x7E, 0xC0, 0x29, 0x5C, 0xD5, 0x41, 0x52, 0xB8, 0x7E, 0xC0, 0xCD
};
static const uint8_t data_4EB65[16] = {
  0xCC, 0xD0, 0x41, 0xCD, 0xCC, 0xCC, 0xBD, 0xCD, 0xCC, 0x6C, 0x40, 0x5C, 0x8F, 0xC2, 0xBF, 0x00
};
static const uint8_t data_4EB75[16] = {
  0x00, 0x80, 0x3F, 0x3D, 0x0A, 0xD3, 0x41, 0x00, 0x00, 0x80, 0x3F, 0x0A, 0xD7, 0xC3, 0xC0, 0x7B
};
static const uint8_t data_4EB85[16] = {
  0x14, 0xFE, 0x41, 0x14, 0xAE, 0x47, 0xC0, 0x7B, 0x14, 0xFE, 0x41, 0x14, 0xAE, 0x47, 0xC0, 0xB8
};
static const uint8_t data_4EB95[16] = {
  0x1E, 0x05, 0x42, 0x0A, 0xD7, 0xC3, 0xC0, 0xB8, 0x1E, 0x05, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0x48
};
static const uint8_t data_4EBA5[16] = {
  0xE1, 0x6A, 0x40, 0x0A, 0xD7, 0x93, 0xC0, 0x00, 0x00, 0x80, 0x3F, 0x3D, 0x0A, 0x02, 0x42, 0x00
};
static const uint8_t data_4EBB5[16] = {
  0x00, 0x80, 0x3F, 0x71, 0x3D, 0xBE, 0xC1, 0x29, 0x5C, 0x0F, 0x42, 0x48, 0xE1, 0xB8, 0xC1, 0xD7
};
static const uint8_t data_4EBC5[16] = {
  0xA3, 0x12, 0x42, 0x8F, 0xC2, 0xC9, 0xC1, 0xE1, 0x7A, 0x19, 0x42, 0xB8, 0x1E, 0xCF, 0xC1, 0x71
};
static const uint8_t data_4EBD5[16] = {
  0x3D, 0x16, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0x85, 0xEB, 0x51, 0x40, 0x00, 0x00, 0xC4, 0xC1, 0x00
};
static const uint8_t data_4EBE5[16] = {
  0x00, 0x80, 0x3F, 0xA4, 0x70, 0x14, 0x42, 0x00, 0x00, 0x80, 0x3F, 0x14, 0xAE, 0x1D, 0xC2, 0xB8
};
static const uint8_t data_4EBF5[16] = {
  0x1E, 0x16, 0x42, 0xD7, 0xA3, 0x1D, 0xC2, 0xB8, 0x1E, 0x1C, 0x42, 0xAE, 0x47, 0x2D, 0xC2, 0xB8
};
static const uint8_t data_4EC05[16] = {
  0x1E, 0x1C, 0x42, 0x71, 0x3D, 0x2D, 0xC2, 0xB8, 0x1E, 0x16, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0xAE
};
static const uint8_t data_4EC15[16] = {
  0x47, 0x71, 0x40, 0xA4, 0x70, 0x25, 0xC2, 0x00, 0x00, 0x80, 0x3F, 0xB8, 0x1E, 0x19, 0x42, 0x00
};
static const uint8_t data_4EC25[16] = {
  0x00, 0x80, 0x3F, 0xCD, 0xCC, 0x33, 0xC2, 0x33, 0x33, 0x17, 0x42, 0xCD, 0xCC, 0x33, 0xC2, 0x5C
};
static const uint8_t data_4EC35[16] = {
  0x8F, 0x1C, 0x42, 0x0A, 0xD7, 0x3F, 0xC2, 0x1F, 0x85, 0x1C, 0x42, 0x66, 0x66, 0x3E, 0xC2, 0xB8
};
static const uint8_t data_4EC45[16] = {
  0x1E, 0x17, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0x14, 0xAE, 0x57, 0x40, 0x0A, 0xD7, 0x39, 0xC2, 0x00
};
static const uint8_t data_4EC55[16] = {
  0x00, 0x80, 0x3F, 0x0A, 0xD7, 0x19, 0x42, 0x00, 0x00, 0x80, 0x3F, 0x1F, 0x85, 0x3B, 0xC2, 0x29
};
static const uint8_t data_4EC65[16] = {
  0x5C, 0x33, 0x42, 0x00, 0x00, 0x38, 0xC2, 0x66, 0x66, 0x38, 0x42, 0x48, 0xE1, 0x42, 0xC2, 0x7B
};
static const uint8_t data_4EC75[16] = {
  0x14, 0x41, 0x42, 0xE1, 0x7A, 0x46, 0xC2, 0x33, 0x33, 0x3B, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0x71
};
static const uint8_t data_4EC85[16] = {
  0x3D, 0x92, 0x40, 0x8F, 0xC2, 0x3F, 0xC2, 0x00, 0x00, 0x80, 0x3F, 0x29, 0x5C, 0x39, 0x42, 0x00
};
static const uint8_t data_4EC95[16] = {
  0x00, 0x80, 0x3F, 0x85, 0xEB, 0x68, 0xC2, 0xEC, 0x51, 0x38, 0x42, 0x85, 0xEB, 0x6F, 0xC2, 0x52
};
static const uint8_t data_4ECA5[16] = {
  0xB8, 0x2E, 0x42, 0xC3, 0xF5, 0x62, 0xC2, 0x1F, 0x85, 0x25, 0x42, 0x33, 0x33, 0x5C, 0xC2, 0x29
};
static const uint8_t data_4ECB5[16] = {
  0x5C, 0x2F, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0x14, 0xAE, 0x77, 0x40, 0x00, 0x00, 0x66, 0xC2, 0x52
};
static const uint8_t data_4ECC5[16] = {
  0xB8, 0xBE, 0x3F, 0xC3, 0xF5, 0x2E, 0x42, 0x52, 0xB8, 0xBE, 0x3F, 0xD7, 0xA3, 0x77, 0xC2, 0x33
};
static const uint8_t data_4ECD5[16] = {
  0x33, 0x3C, 0x42, 0x33, 0x33, 0x75, 0xC2, 0xD7, 0xA3, 0x3F, 0x42, 0xF6, 0x28, 0x7E, 0xC2, 0xC3
};
static const uint8_t data_4ECE5[16] = {
  0xF5, 0x45, 0x42, 0xAE, 0x47, 0x80, 0xC2, 0x1F, 0x85, 0x42, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0x33
};
static const uint8_t data_4ECF5[16] = {
  0x33, 0x53, 0x40, 0x48, 0xE1, 0x7A, 0xC2, 0x00, 0x00, 0x80, 0x3F, 0x7B, 0x14, 0x41, 0x42, 0x00
};
static const uint8_t data_4ED05[16] = {
  0x00, 0x80, 0x3F, 0x00, 0x00, 0x5E, 0xC2, 0x52, 0xB8, 0x4D, 0x42, 0x29, 0x5C, 0x5A, 0xC2, 0x48
};
static const uint8_t data_4ED15[16] = {
  0xE1, 0x52, 0x42, 0xCD, 0xCC, 0x6A, 0xC2, 0x33, 0x33, 0x5F, 0x42, 0xD7, 0xA3, 0x6E, 0xC2, 0x52
};
static const uint8_t data_4ED25[16] = {
  0xB8, 0x59, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0xCD, 0xCC, 0x6C, 0x40, 0x5C, 0x8F, 0x65, 0xC2, 0x00
};
static const uint8_t data_4ED35[16] = {
  0x00, 0x80, 0x3F, 0x0A, 0xD7, 0x54, 0x42, 0x00, 0x00, 0x80, 0x3F, 0x8F, 0xC2, 0x7B, 0xC2, 0xB8
};
static const uint8_t data_4ED45[16] = {
  0x1E, 0x66, 0x42, 0x8F, 0xC2, 0x77, 0xC2, 0x5C, 0x8F, 0x67, 0x42, 0xB8, 0x1E, 0x7C, 0xC2, 0xC3
};
static const uint8_t data_4ED55[16] = {
  0xF5, 0x73, 0x42, 0x5C, 0x0F, 0x80, 0xC2, 0xA4, 0x70, 0x72, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0x71
};
static const uint8_t data_4ED65[16] = {
  0x3D, 0x92, 0x40, 0x85, 0xEB, 0x7B, 0xC2, 0x00, 0x00, 0x80, 0x3F, 0x00, 0x00, 0x6D, 0x42, 0x00
};
static const uint8_t data_4ED75[16] = {
  0x00, 0x80, 0x3F, 0xA4, 0xF0, 0x93, 0xC2, 0xF6, 0x28, 0x6D, 0x42, 0xA4, 0xF0, 0x93, 0xC2, 0x9A
};
static const uint8_t data_4ED85[16] = {
  0x99, 0x72, 0x42, 0xA4, 0xF0, 0x99, 0xC2, 0x9A, 0x99, 0x72, 0x42, 0xA4, 0xF0, 0x99, 0xC2, 0xF6
};
static const uint8_t data_4ED95[16] = {
  0x28, 0x6D, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0x14, 0xAE, 0x57, 0x40, 0xA4, 0xF0, 0x96, 0xC2, 0x00
};
static const uint8_t data_4EDA5[16] = {
  0x00, 0x80, 0x3F, 0x48, 0xE1, 0x6F, 0x42, 0x00, 0x00, 0x80, 0x3F, 0x8F, 0xC2, 0xA0, 0xC2, 0x5C
};
static const uint8_t data_4EDB5[16] = {
  0x8F, 0x6D, 0x42, 0xAE, 0xC7, 0xA0, 0xC2, 0x52, 0xB8, 0x71, 0x42, 0x33, 0x33, 0xA6, 0xC2, 0x52
};
static const uint8_t data_4EDC5[16] = {
  0xB8, 0x71, 0x42, 0x33, 0x33, 0xA6, 0xC2, 0x1F, 0x85, 0x6D, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0x85
};
static const uint8_t data_4EDD5[16] = {
  0xEB, 0x51, 0x40, 0xE1, 0x7A, 0xA3, 0xC2, 0x00, 0x00, 0x80, 0x3F, 0xD7, 0xA3, 0x6F, 0x42, 0x00
};
static const uint8_t data_4EDE5[16] = {
  0x00, 0x80, 0x3F, 0x33, 0x33, 0xCC, 0xC2, 0x52, 0x38, 0x82, 0x42, 0xEC, 0x51, 0xCB, 0xC2, 0xF6
};
static const uint8_t data_4EDF5[16] = {
  0x28, 0x84, 0x42, 0x0A, 0xD7, 0xD1, 0xC2, 0x33, 0x33, 0x87, 0x42, 0x71, 0xBD, 0xD2, 0xC2, 0x8F
};
static const uint8_t data_4EE05[16] = {
  0x42, 0x85, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0xD7, 0xA3, 0x30, 0x40, 0x1F, 0x05, 0xCF, 0xC2, 0x00
};
static const uint8_t data_4EE15[16] = {
  0x00, 0x80, 0x3F, 0x33, 0xB3, 0x84, 0x42, 0x00, 0x00, 0x80, 0x3F, 0x9A, 0x19, 0xC4, 0xC2, 0x29
};
static const uint8_t data_4EE25[16] = {
  0xDC, 0x87, 0x42, 0x0A, 0xD7, 0xBA, 0xC2, 0x3D, 0x8A, 0x83, 0x42, 0x85, 0xEB, 0xB7, 0xC2, 0x85
};
static const uint8_t data_4EE35[16] = {
  0xEB, 0x8B, 0x42, 0x14, 0x2E, 0xC1, 0xC2, 0x8F, 0x42, 0x8E, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0x71
};
static const uint8_t data_4EE45[16] = {
  0x3D, 0x82, 0x40, 0x1F, 0x05, 0xBE, 0xC2, 0x5C, 0x8F, 0xE2, 0x3F, 0x48, 0xE1, 0x88, 0x42, 0x5C
};
static const uint8_t data_4EE55[16] = {
  0x8F, 0xE2, 0x3F, 0x3D, 0x8A, 0xD4, 0xC2, 0xF6, 0x28, 0x89, 0x42, 0x48, 0xE1, 0xDE, 0xC2, 0x00
};
static const uint8_t data_4EE65[16] = {
  0x00, 0x8E, 0x42, 0xE1, 0x7A, 0xE2, 0xC2, 0x0A, 0x57, 0x86, 0x42, 0x1F, 0x05, 0xD8, 0xC2, 0x5C
};
static const uint8_t data_4EE75[16] = {
  0x8F, 0x81, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0x48, 0xE1, 0x6A, 0x40, 0xE1, 0x7A, 0xDB, 0xC2, 0x66
};
static const uint8_t data_4EE85[16] = {
  0x66, 0x06, 0x40, 0x8F, 0xC2, 0x87, 0x42, 0x66, 0x66, 0x06, 0x40, 0xCD, 0x4C, 0xF2, 0xC2, 0xA4
};
static const uint8_t data_4EE95[16] = {
  0x70, 0xA4, 0x42, 0xB8, 0x1E, 0xF7, 0xC2, 0xD7, 0xA3, 0xAA, 0x42, 0xD7, 0xA3, 0xFD, 0xC2, 0x71
};
static const uint8_t data_4EEA5[16] = {
  0x3D, 0xA5, 0x42, 0x71, 0xBD, 0xF8, 0xC2, 0x0A, 0x57, 0x9F, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0x48
};
static const uint8_t data_4EEB5[16] = {
  0xE1, 0x6A, 0x40, 0xC3, 0xF5, 0xF7, 0xC2, 0xB8, 0x1E, 0x05, 0x40, 0x85, 0xEB, 0xA4, 0x42, 0xB8
};
static const uint8_t data_4EEC5[16] = {
  0x1E, 0x05, 0x40, 0xC3, 0xF5, 0xCF, 0xC2, 0x48, 0xE1, 0xB8, 0x42, 0x14, 0x2E, 0xD4, 0xC2, 0x33
};
static const uint8_t data_4EED5[16] = {
  0x33, 0xBE, 0x42, 0xD7, 0x23, 0xDA, 0xC2, 0xE1, 0x7A, 0xB9, 0x42, 0x48, 0xE1, 0xD5, 0xC2, 0x9A
};
static const uint8_t data_4EEE5[16] = {
  0x19, 0xB4, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0x9A, 0x99, 0x79, 0x40, 0x3D, 0x0A, 0xD5, 0xC2, 0x85
};
static const uint8_t data_4EEF5[16] = {
  0xEB, 0xF1, 0x3F, 0xF6, 0x28, 0xB9, 0x42, 0x85, 0xEB, 0xF1, 0x3F, 0xD7, 0xA3, 0xC7, 0xC2, 0x3D
};
static const uint8_t data_4EF05[16] = {
  0x0A, 0xAF, 0x42, 0x9A, 0x99, 0xCC, 0xC2, 0xEC, 0xD1, 0xB5, 0x42, 0x8F, 0x42, 0xD1, 0xC2, 0xCD
};
static const uint8_t data_4EF15[16] = {
  0x4C, 0xB2, 0x42, 0x52, 0x38, 0xCC, 0xC2, 0xCD, 0xCC, 0xAB, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0x48
};
static const uint8_t data_4EF25[16] = {
  0xE1, 0x6A, 0x40, 0x1F, 0x05, 0xCC, 0xC2, 0xB8, 0x1E, 0x85, 0x3F, 0x00, 0x00, 0xB1, 0x42, 0xB8
};
static const uint8_t data_4EF35[16] = {
  0x1E, 0x85, 0x3F, 0x1F, 0x85, 0xB4, 0xC2, 0x5C, 0x8F, 0xA2, 0x42, 0xB8, 0x9E, 0xB3, 0xC2, 0xE1
};
static const uint8_t data_4EF45[16] = {
  0x7A, 0xA4, 0x42, 0x5C, 0x8F, 0xB8, 0xC2, 0xAE, 0xC7, 0xA6, 0x42, 0xC3, 0x75, 0xB9, 0xC2, 0x29
};
static const uint8_t data_4EF55[16] = {
  0xDC, 0xA4, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0x85, 0xEB, 0x51, 0x40, 0x3D, 0x8A, 0xB6, 0xC2, 0x00
};
static const uint8_t data_4EF65[16] = {
  0x00, 0x00, 0x40, 0xF6, 0xA8, 0xA4, 0x42, 0x00, 0x00, 0x00, 0x40, 0xE1, 0xFA, 0xA2, 0xC2, 0x00
};
static const uint8_t data_4EF75[16] = {
  0x00, 0xA7, 0x42, 0xF6, 0x28, 0x9C, 0xC2, 0x00, 0x00, 0xA7, 0x42, 0xF6, 0x28, 0x9C, 0xC2, 0x3D
};
static const uint8_t data_4EF85[16] = {
  0x8A, 0xA9, 0x42, 0xE1, 0xFA, 0xA2, 0xC2, 0x3D, 0x8A, 0xA9, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0x71
};
static const uint8_t data_4EF95[16] = {
  0x3D, 0x92, 0x40, 0x5C, 0x8F, 0x9F, 0xC2, 0x00, 0x00, 0xC0, 0x3F, 0xAE, 0x47, 0xA8, 0x42, 0x00
};
static const uint8_t data_4EFA5[16] = {
  0x00, 0xC0, 0x3F, 0xEC, 0x51, 0x90, 0xC2, 0x71, 0x3D, 0xA7, 0x42, 0xCD, 0x4C, 0x90, 0xC2, 0x0A
};
static const uint8_t data_4EFB5[16] = {
  0x57, 0xA9, 0x42, 0x8F, 0xC2, 0x95, 0xC2, 0x0A, 0x57, 0xA9, 0x42, 0x8F, 0xC2, 0x95, 0xC2, 0x71
};
static const uint8_t data_4EFC5[16] = {
  0x3D, 0xA7, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0x33, 0x33, 0x53, 0x40, 0x3D, 0x0A, 0x93, 0xC2, 0x00
};
static const uint8_t data_4EFD5[16] = {
  0x00, 0x80, 0x3F, 0xCD, 0x4C, 0xA8, 0x42, 0x00, 0x00, 0x80, 0x3F, 0x52, 0xB8, 0x22, 0xC2, 0x52
};
static const uint8_t data_4EFE5[16] = {
  0x38, 0xA1, 0x42, 0xD7, 0xA3, 0x15, 0xC2, 0x1F, 0x05, 0x9F, 0x42, 0xE1, 0x7A, 0x11, 0xC2, 0x29
};
static const uint8_t data_4EFF5[16] = {
  0x5C, 0xA5, 0x42, 0x48, 0xE1, 0x1E, 0xC2, 0x5C, 0x8F, 0xA7, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0x71
};
static const uint8_t data_4F005[16] = {
  0x3D, 0x92, 0x40, 0xE1, 0x7A, 0x1B, 0xC2, 0x00, 0x00, 0x80, 0x3F, 0x66, 0x66, 0xA1, 0x42, 0x00
};
static const uint8_t data_4F015[16] = {
  0x00, 0x80, 0x3F, 0x52, 0xB8, 0x1E, 0xC0, 0x0A, 0x57, 0xA9, 0x42, 0xB8, 0x1E, 0x55, 0xC0, 0x00
};
static const uint8_t data_4F025[16] = {
  0x80, 0xAB, 0x42, 0x8F, 0xC2, 0xB5, 0xC0, 0x71, 0xBD, 0xA7, 0x42, 0x5C, 0x8F, 0x9A, 0xC0, 0xB8
};
static const uint8_t data_4F035[16] = {
  0x9E, 0xA5, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0x14, 0xAE, 0x57, 0x40, 0x85, 0xEB, 0x81, 0xC0, 0x00
};
static const uint8_t data_4F045[16] = {
  0x00, 0x80, 0x3F, 0x00, 0x80, 0xA8, 0x42, 0x00, 0x00, 0x80, 0x3F, 0x48, 0xE1, 0xEC, 0x41, 0x7B
};
static const uint8_t data_4F055[16] = {
  0x14, 0x0E, 0x41, 0x3D, 0x0A, 0xF7, 0x41, 0xE1, 0x7A, 0xCC, 0x40, 0x5C, 0x8F, 0xFE, 0x41, 0xF6
};
static const uint8_t data_4F065[16] = {
  0x28, 0xDC, 0x40, 0x66, 0x66, 0xF4, 0x41, 0x85, 0xEB, 0x15, 0x41, 0xCD, 0xCC, 0xCC, 0xBD, 0x85
};
static const uint8_t data_4F075[16] = {
  0xEB, 0x51, 0x40, 0x14, 0xAE, 0xF5, 0x41, 0x00, 0x00, 0x80, 0x3F, 0xF6, 0x28, 0xFC, 0x40, 0x00
};
static const uint8_t data_4F085[16] = {
  0x00, 0x80, 0x3F, 0x71, 0x3D, 0x5E, 0xC1, 0xAE, 0x47, 0x0E, 0x42, 0xC3, 0xF5, 0x38, 0xC1, 0x52
};
static const uint8_t data_4F095[16] = {
  0xB8, 0x06, 0x42, 0x3D, 0x0A, 0x2B, 0xC1, 0xB8, 0x1E, 0x0B, 0x42, 0xEC, 0x51, 0x50, 0xC1, 0x14
};
static const uint8_t data_4F0A5[16] = {
  0xAE, 0x12, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0x14, 0xAE, 0x57, 0x40, 0xD7, 0xA3, 0x44, 0xC1, 0x00
};
static const uint8_t data_4F0B5[16] = {
  0x00, 0x80, 0x3F, 0x52, 0xB8, 0x0C, 0x42, 0x00, 0x00, 0x80, 0x3F, 0x3D, 0x0A, 0xA3, 0xC2, 0xA4
};
static const uint8_t data_4F0C5[16] = {
  0xF0, 0x82, 0x42, 0x3D, 0x0A, 0xA3, 0xC2, 0x5C, 0x8F, 0x7F, 0x42, 0x00, 0x00, 0x9D, 0xC2, 0x5C
};
static const uint8_t data_4F0D5[16] = {
  0x8F, 0x7F, 0x42, 0x00, 0x00, 0x9D, 0xC2, 0xA4, 0xF0, 0x82, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0x48
};
static const uint8_t data_4F0E5[16] = {
  0xE1, 0x6A, 0x40, 0x1F, 0x05, 0xA0, 0xC2, 0x00, 0x00, 0x80, 0x3F, 0x29, 0x5C, 0x81, 0x42, 0x00
};
static const uint8_t data_4F0F5[16] = {
  0x00, 0x80, 0x3F, 0xA4, 0xF0, 0xAF, 0xC2, 0xCD, 0xCC, 0x75, 0x42, 0x1F, 0x85, 0xB0, 0xC2, 0xEC
};
static const uint8_t data_4F105[16] = {
  0x51, 0x71, 0x42, 0x33, 0x33, 0xAA, 0xC2, 0x00, 0x00, 0x6E, 0x42, 0xF6, 0xA8, 0xA9, 0xC2, 0xEC
};
static const uint8_t data_4F115[16] = {
  0x51, 0x72, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0x00, 0x00, 0x90, 0x40, 0x7B, 0x14, 0xAD, 0xC2, 0x00
};
static const uint8_t data_4F125[16] = {
  0x00, 0x80, 0x3F, 0x0A, 0xD7, 0x71, 0x42, 0x00, 0x00, 0x80, 0x3F, 0x48, 0xE1, 0xA9, 0xC2, 0x8F
};
static const uint8_t data_4F135[16] = {
  0x42, 0x80, 0x42, 0x48, 0xE1, 0xA8, 0xC2, 0xCD, 0xCC, 0x84, 0x42, 0x9A, 0x99, 0xAF, 0xC2, 0x66
};
static const uint8_t data_4F145[16] = {
  0x66, 0x86, 0x42, 0xF6, 0xA8, 0xB0, 0xC2, 0x48, 0xE1, 0x81, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0x71
};
static const uint8_t data_4F155[16] = {
  0x3D, 0x92, 0x40, 0x1F, 0x05, 0xAD, 0xC2, 0x00, 0x00, 0x80, 0x3F, 0xF6, 0x28, 0x82, 0x42, 0x00
};
static const uint8_t data_4F165[16] = {
  0x00, 0x80, 0x3F, 0x33, 0x33, 0xD7, 0xC2, 0xAE, 0x47, 0x91, 0x42, 0x8F, 0x42, 0xD1, 0xC2, 0x3D
};
static const uint8_t data_4F175[16] = {
  0x8A, 0x8E, 0x42, 0x7B, 0x14, 0xD0, 0xC2, 0x1F, 0x05, 0x91, 0x42, 0xC3, 0xF5, 0xD5, 0xC2, 0xAE
};
static const uint8_t data_4F185[16] = {
  0xC7, 0x93, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0x48, 0xE1, 0x92, 0x40, 0xB8, 0x9E, 0xD3, 0xC2, 0x00
};
static const uint8_t data_4F195[16] = {
  0x00, 0x80, 0x3F, 0xF6, 0x28, 0x91, 0x42, 0x00, 0x00, 0x80, 0x3F, 0x0A, 0x57, 0xFE, 0xC2, 0x1F
};
static const uint8_t data_4F1A5[16] = {
  0x05, 0x80, 0x42, 0xA4, 0xF0, 0xF8, 0xC2, 0x0A, 0x57, 0x84, 0x42, 0x0A, 0xD7, 0xFC, 0xC2, 0x14
};
static const uint8_t data_4F1B5[16] = {
  0x2E, 0x89, 0x42, 0x0A, 0x17, 0x01, 0xC3, 0x85, 0xEB, 0x84, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0x5C
};
static const uint8_t data_4F1C5[16] = {
  0x8F, 0x12, 0x40, 0x7B, 0x94, 0xFD, 0xC2, 0x66, 0x66, 0xC6, 0x3F, 0xB8, 0x9E, 0x84, 0x42, 0x66
};
static const uint8_t data_4F1D5[16] = {
  0x66, 0xC6, 0x3F, 0x66, 0x66, 0xE8, 0xC2, 0xAE, 0xC7, 0xA3, 0x42, 0x9A, 0x99, 0xE5, 0xC2, 0x8F
};
static const uint8_t data_4F1E5[16] = {
  0xC2, 0xA5, 0x42, 0x71, 0x3D, 0xE9, 0xC2, 0x1F, 0x85, 0xAA, 0x42, 0x7B, 0x14, 0xEC, 0xC2, 0x00
};
static const uint8_t data_4F1F5[16] = {
  0x80, 0xA8, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0xD7, 0xA3, 0x50, 0x40, 0x52, 0x38, 0xE9, 0xC2, 0x00
};
static const uint8_t data_4F205[16] = {
  0x00, 0x80, 0x3F, 0xF6, 0xA8, 0xA6, 0x42, 0x00, 0x00, 0x80, 0x3F, 0xC3, 0x75, 0xF8, 0xC2, 0xB8
};
static const uint8_t data_4F215[16] = {
  0x1E, 0xAE, 0x42, 0x5C, 0x0F, 0xF7, 0xC2, 0x8F, 0xC2, 0xB4, 0x42, 0x33, 0x33, 0xFB, 0xC2, 0xB8
};
static const uint8_t data_4F225[16] = {
  0x9E, 0xB5, 0x42, 0x1F, 0x85, 0xFC, 0xC2, 0xC3, 0xF5, 0xAE, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0xAE
};
static const uint8_t data_4F235[16] = {
  0x47, 0x51, 0x40, 0xEC, 0xD1, 0xF9, 0xC2, 0xAE, 0x47, 0x81, 0x3F, 0x29, 0xDC, 0xB1, 0x42, 0xAE
};
static const uint8_t data_4F245[16] = {
  0x47, 0x81, 0x3F, 0x29, 0x5C, 0xE2, 0xC2, 0x8F, 0xC2, 0xC2, 0x42, 0x0A, 0xD7, 0xDC, 0xC2, 0xF6
};
static const uint8_t data_4F255[16] = {
  0xA8, 0xC4, 0x42, 0xCD, 0x4C, 0xDD, 0xC2, 0x85, 0xEB, 0xC5, 0x42, 0xCD, 0xCC, 0xE2, 0xC2, 0x1F
};
static const uint8_t data_4F265[16] = {
  0x05, 0xC4, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0x8F, 0xC2, 0x45, 0x40, 0xEC, 0xD1, 0xDF, 0xC2, 0x00
};
static const uint8_t data_4F275[16] = {
  0x00, 0x80, 0x3F, 0x0A, 0x57, 0xC4, 0x42, 0x00, 0x00, 0x80, 0x3F, 0x0A, 0x57, 0xBC, 0xC2, 0x52
};
static const uint8_t data_4F285[16] = {
  0x38, 0xA8, 0x42, 0x5C, 0x0F, 0xC1, 0xC2, 0x85, 0x6B, 0xAA, 0x42, 0x71, 0x3D, 0xC3, 0xC2, 0x29
};
static const uint8_t data_4F295[16] = {
  0xDC, 0xA5, 0x42, 0xA4, 0x70, 0xBE, 0xC2, 0xD7, 0xA3, 0xA3, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0xA4
};
static const uint8_t data_4F2A5[16] = {
  0x70, 0x4D, 0x40, 0x8F, 0xC2, 0xBF, 0xC2, 0xAE, 0x47, 0xA1, 0x3F, 0x1F, 0x05, 0xA7, 0x42, 0xAE
};
static const uint8_t data_4F2B5[16] = {
  0x47, 0xA1, 0x3F, 0x0A, 0xD7, 0xAD, 0xC2, 0xD7, 0xA3, 0x9F, 0x42, 0x1F, 0x05, 0xA8, 0xC2, 0x71
};
static const uint8_t data_4F2C5[16] = {
  0x3D, 0x9E, 0x42, 0x29, 0x5C, 0xA7, 0xC2, 0x29, 0xDC, 0xA0, 0x42, 0x71, 0x3D, 0xAD, 0xC2, 0x8F
};
static const uint8_t data_4F2D5[16] = {
  0x42, 0xA2, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0x14, 0xAE, 0x57, 0x40, 0xB8, 0x9E, 0xAA, 0xC2, 0x00
};
static const uint8_t data_4F2E5[16] = {
  0x00, 0x80, 0x3F, 0x71, 0x3D, 0xA0, 0x42, 0x00, 0x00, 0x80, 0x3F, 0x48, 0x61, 0x91, 0xC2, 0x5C
};
static const uint8_t data_4F2F5[16] = {
  0x0F, 0xA0, 0x42, 0x48, 0x61, 0x91, 0xC2, 0xC3, 0xF5, 0x97, 0x42, 0xCD, 0x4C, 0x8D, 0xC2, 0xC3
};
static const uint8_t data_4F305[16] = {
  0xF5, 0x97, 0x42, 0xCD, 0x4C, 0x8D, 0xC2, 0x5C, 0x0F, 0xA0, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0x48
};
static const uint8_t data_4F315[16] = {
  0xE1, 0x8A, 0x40, 0x0A, 0x57, 0x8F, 0xC2, 0xC3, 0xF5, 0xA8, 0x3F, 0x66, 0x66, 0x9D, 0x42, 0xC3
};
static const uint8_t data_4F325[16] = {
  0xF5, 0xA8, 0x3F, 0xCD, 0xCC, 0x80, 0xC2, 0x3D, 0x0A, 0x8B, 0x42, 0xCD, 0xCC, 0x80, 0xC2, 0x00
};
static const uint8_t data_4F335[16] = {
  0x00, 0x80, 0x42, 0x7B, 0x14, 0x72, 0xC2, 0x00, 0x00, 0x80, 0x42, 0x7B, 0x14, 0x72, 0xC2, 0x1F
};
static const uint8_t data_4F345[16] = {
  0x05, 0x8B, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0x33, 0x33, 0x73, 0x40, 0x0A, 0xD7, 0x79, 0xC2, 0x3D
};
static const uint8_t data_4F355[16] = {
  0x0A, 0xF7, 0x3F, 0x1F, 0x85, 0x85, 0x42, 0x3D, 0x0A, 0xF7, 0x3F, 0xC3, 0xF5, 0x80, 0xC2, 0x85
};
static const uint8_t data_4F365[16] = {
  0xEB, 0x95, 0x42, 0x33, 0x33, 0x7C, 0xC2, 0x85, 0xEB, 0x95, 0x42, 0x33, 0x33, 0x7C, 0xC2, 0x85
};
static const uint8_t data_4F375[16] = {
  0xEB, 0x9B, 0x42, 0xC3, 0xF5, 0x80, 0xC2, 0x85, 0xEB, 0x9B, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0x14
};
static const uint8_t data_4F385[16] = {
  0xAE, 0x57, 0x40, 0x7B, 0x14, 0x7F, 0xC2, 0x00, 0x00, 0x80, 0x3F, 0x85, 0xEB, 0x98, 0x42, 0x00
};
static const uint8_t data_4F395[16] = {
  0x00, 0x80, 0x3F, 0x33, 0x33, 0x36, 0xC2, 0xF6, 0xA8, 0x99, 0x42, 0xE1, 0x7A, 0x3B, 0xC2, 0x7B
};
static const uint8_t data_4F3A5[16] = {
  0x14, 0x9B, 0x42, 0x3D, 0x0A, 0x3E, 0xC2, 0xF6, 0xA8, 0x98, 0x42, 0x9A, 0x99, 0x38, 0xC2, 0x33
};
static const uint8_t data_4F3B5[16] = {
  0x33, 0x97, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0xA4, 0x70, 0x3D, 0x40, 0x7B, 0x14, 0x3A, 0xC2, 0x00
};
static const uint8_t data_4F3C5[16] = {
  0x00, 0x80, 0x3F, 0xD7, 0x23, 0x99, 0x42, 0x00, 0x00, 0x80, 0x3F, 0xC3, 0xF5, 0x19, 0xC2, 0x85
};
static const uint8_t data_4F3D5[16] = {
  0xEB, 0x92, 0x42, 0x00, 0x00, 0x1B, 0xC2, 0xF6, 0x28, 0x91, 0x42, 0xA4, 0x70, 0x11, 0xC2, 0x52
};
static const uint8_t data_4F3E5[16] = {
  0xB8, 0x8F, 0x42, 0x29, 0x5C, 0x10, 0xC2, 0x3D, 0x8A, 0x91, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0x71
};
static const uint8_t data_4F3F5[16] = {
  0x3D, 0x5A, 0x40, 0x52, 0xB8, 0x15, 0xC2, 0x00, 0x00, 0x80, 0x3F, 0xEC, 0x51, 0x91, 0x42, 0x00
};
static const uint8_t data_4F405[16] = {
  0x00, 0x80, 0x3F, 0x71, 0x3D, 0xA0, 0xC1, 0xD7, 0x23, 0x91, 0x42, 0x71, 0x3D, 0xA0, 0xC1, 0x85
};
static const uint8_t data_4F415[16] = {
  0x6B, 0x8C, 0x42, 0xAE, 0x47, 0x8D, 0xC1, 0x85, 0x6B, 0x8C, 0x42, 0xAE, 0x47, 0x8D, 0xC1, 0xD7
};
static const uint8_t data_4F425[16] = {
  0x23, 0x91, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0xE1, 0x7A, 0x64, 0x40, 0x52, 0xB8, 0x96, 0xC1, 0x3D
};
static const uint8_t data_4F435[16] = {
  0x0A, 0x97, 0x3F, 0xAE, 0xC7, 0x8E, 0x42, 0x3D, 0x0A, 0x97, 0x3F, 0x0A, 0xD7, 0x85, 0x41, 0x71
};
static const uint8_t data_4F445[16] = {
  0xBD, 0xBA, 0x42, 0xCD, 0xCC, 0x74, 0x41, 0x3D, 0x8A, 0xB8, 0x42, 0x0A, 0xD7, 0x7F, 0x41, 0x5C
};
static const uint8_t data_4F455[16] = {
  0x8F, 0xB6, 0x42, 0xA4, 0x70, 0x8B, 0x41, 0x0A, 0xD7, 0xB8, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0x48
};
static const uint8_t data_4F465[16] = {
  0xE1, 0x3A, 0x40, 0xC3, 0xF5, 0x82, 0x41, 0x00, 0x00, 0x80, 0x3F, 0x14, 0xAE, 0xB8, 0x42, 0x00
};
static const uint8_t data_4F475[16] = {
  0x00, 0x80, 0x3F, 0x9A, 0x99, 0x99, 0x41, 0x3D, 0x0A, 0x23, 0x42, 0x7B, 0x14, 0xA6, 0x41, 0x3D
};
static const uint8_t data_4F485[16] = {
  0x0A, 0x23, 0x42, 0x7B, 0x14, 0xA6, 0x41, 0xD7, 0xA3, 0x32, 0x42, 0x9A, 0x99, 0x99, 0x41, 0xD7
};
static const uint8_t data_4F495[16] = {
  0xA3, 0x32, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0xAE, 0x47, 0x71, 0x40, 0x0A, 0xD7, 0x9F, 0x41, 0x00
};
static const uint8_t data_4F4A5[16] = {
  0x00, 0x80, 0x3F, 0x0A, 0xD7, 0x2A, 0x42, 0x00, 0x00, 0x80, 0x3F, 0x9A, 0x99, 0x99, 0x41, 0x9A
};
static const uint8_t data_4F4B5[16] = {
  0x99, 0x3B, 0x42, 0x8F, 0xC2, 0xB5, 0x41, 0x9A, 0x99, 0x3B, 0x42, 0x8F, 0xC2, 0xB5, 0x41, 0x0A
};
static const uint8_t data_4F4C5[16] = {
  0xD7, 0x50, 0x42, 0x9A, 0x99, 0x99, 0x41, 0x0A, 0xD7, 0x50, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0x33
};
static const uint8_t data_4F4D5[16] = {
  0x33, 0x73, 0x40, 0x14, 0xAE, 0xA7, 0x41, 0x52, 0xB8, 0xDE, 0x3F, 0x33, 0x33, 0x46, 0x42, 0x52
};
static const uint8_t data_4F4E5[16] = {
  0xB8, 0xDE, 0x3F, 0xD7, 0xA3, 0x80, 0x41, 0xD7, 0xA3, 0x62, 0x42, 0x71, 0x3D, 0x66, 0x41, 0xD7
};
static const uint8_t data_4F4F5[16] = {
  0xA3, 0x62, 0x42, 0x71, 0x3D, 0x66, 0x41, 0xD7, 0xA3, 0x56, 0x42, 0xD7, 0xA3, 0x80, 0x41, 0xD7
};
static const uint8_t data_4F505[16] = {
  0xA3, 0x56, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0x48, 0xE1, 0x6A, 0x40, 0x0A, 0xD7, 0x73, 0x41, 0x00
};
static const uint8_t data_4F515[16] = {
  0x00, 0x80, 0x3F, 0x14, 0xAE, 0x5C, 0x42, 0x00, 0x00, 0x80, 0x3F, 0x9A, 0x99, 0x99, 0x41, 0xD7
};
static const uint8_t data_4F525[16] = {
  0xA3, 0x5C, 0x42, 0x29, 0x5C, 0xA7, 0x41, 0xD7, 0xA3, 0x5C, 0x42, 0x29, 0x5C, 0xA7, 0x41, 0x9A
};
static const uint8_t data_4F535[16] = {
  0x99, 0x68, 0x42, 0x1F, 0x85, 0x99, 0x41, 0x9A, 0x99, 0x68, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0x48
};
static const uint8_t data_4F545[16] = {
  0xE1, 0x6A, 0x40, 0xE1, 0x7A, 0xA0, 0x41, 0x00, 0x00, 0x80, 0x3F, 0x9A, 0x99, 0x62, 0x42, 0x00
};
static const uint8_t data_4F555[16] = {
  0x00, 0x80, 0x3F, 0x9A, 0x99, 0xAD, 0x41, 0xB8, 0x1E, 0x92, 0x42, 0x85, 0xEB, 0xA7, 0x41, 0xCD
};
static const uint8_t data_4F565[16] = {
  0x4C, 0x93, 0x42, 0x52, 0xB8, 0x9E, 0x41, 0x3D, 0x8A, 0x90, 0x42, 0xEC, 0x51, 0xA4, 0x41, 0x29
};
static const uint8_t data_4F575[16] = {
  0x5C, 0x8F, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0x7B, 0x14, 0xFE, 0x40, 0xF6, 0x28, 0xA6, 0x41, 0x00
};
static const uint8_t data_4F585[16] = {
  0x00, 0x80, 0x3F, 0xEC, 0x51, 0x91, 0x42, 0x00, 0x00, 0x80, 0x3F, 0x00, 0x00, 0xC6, 0x41, 0x7B
};
static const uint8_t data_4F595[16] = {
  0x94, 0x96, 0x42, 0x29, 0x5C, 0xCF, 0x41, 0x0A, 0x57, 0x99, 0x42, 0x29, 0x5C, 0xC9, 0x41, 0x5C
};
static const uint8_t data_4F5A5[16] = {
  0x8F, 0x9A, 0x42, 0x71, 0x3D, 0xC0, 0x41, 0xEC, 0xD1, 0x97, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0xAE
};
static const uint8_t data_4F5B5[16] = {
  0x47, 0x01, 0x41, 0x8F, 0xC2, 0xC7, 0x41, 0x00, 0x00, 0x80, 0x3F, 0x7B, 0x94, 0x98, 0x42, 0x00
};
static const uint8_t data_4F5C5[16] = {
  0x00, 0x80, 0x3F, 0xAE, 0x47, 0xE7, 0x41, 0x71, 0x3D, 0xA0, 0x42, 0x5C, 0x8F, 0xF0, 0x41, 0x00
};
static const uint8_t data_4F5D5[16] = {
  0x00, 0xA3, 0x42, 0xD7, 0xA3, 0xEA, 0x41, 0x52, 0x38, 0xA4, 0x42, 0xA4, 0x70, 0xE1, 0x41, 0xE1
};
static const uint8_t data_4F5E5[16] = {
  0x7A, 0xA1, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0xAE, 0x47, 0x01, 0x41, 0x3D, 0x0A, 0xE9, 0x41, 0x00
};
static const uint8_t data_4F5F5[16] = {
  0x00, 0x80, 0x3F, 0x71, 0x3D, 0xA2, 0x42, 0x00, 0x00, 0x80, 0x3F, 0xC3, 0xF5, 0x1D, 0x42, 0x0A
};
static const uint8_t data_4F605[16] = {
  0x57, 0xA1, 0x42, 0xD7, 0xA3, 0x22, 0x42, 0x9A, 0x19, 0xA4, 0x42, 0xD7, 0xA3, 0x1F, 0x42, 0x0A
};
static const uint8_t data_4F615[16] = {
  0x57, 0xA5, 0x42, 0x7B, 0x14, 0x1B, 0x42, 0x9A, 0x99, 0xA2, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0xAE
};
static const uint8_t data_4F625[16] = {
  0x47, 0x01, 0x41, 0x0A, 0xD7, 0x1E, 0x42, 0x00, 0x00, 0x80, 0x3F, 0x0A, 0x57, 0xA3, 0x42, 0x00
};
static const uint8_t data_4F635[16] = {
  0x00, 0x80, 0x3F, 0x85, 0xEB, 0xEA, 0xC2, 0x1F, 0x05, 0xC7, 0x42, 0xAE, 0xC7, 0xEB, 0xC2, 0xE1
};
static const uint8_t data_4F645[16] = {
  0xFA, 0xC8, 0x42, 0x66, 0xE6, 0xF1, 0xC2, 0x66, 0x66, 0xC6, 0x42, 0x7B, 0x14, 0xF1, 0xC2, 0xEC
};
static const uint8_t data_4F655[16] = {
  0x51, 0xC4, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0x7B, 0x14, 0x7E, 0x40, 0x85, 0x6B, 0xEE, 0xC2, 0x00
};
static const uint8_t data_4F665[16] = {
  0x00, 0xC0, 0x3F, 0x14, 0xAE, 0xC6, 0x42, 0x00, 0x00, 0xC0, 0x3F, 0x14, 0xAE, 0xFB, 0xC1, 0xCD
};
static const uint8_t data_4F675[16] = {
  0xCC, 0xA0, 0x42, 0xA4, 0x70, 0xDB, 0xC1, 0xCD, 0xCC, 0xA0, 0x42, 0xA4, 0x70, 0xDB, 0xC1, 0x1F
};
static const uint8_t data_4F685[16] = {
  0x05, 0xA4, 0x42, 0x14, 0xAE, 0xFB, 0xC1, 0x1F, 0x05, 0xA4, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0x48
};
static const uint8_t data_4F695[16] = {
  0xE1, 0x6A, 0x40, 0x9A, 0x99, 0xEB, 0xC1, 0x00, 0x00, 0x80, 0x3F, 0x66, 0x66, 0xA2, 0x42, 0x00
};
static const uint8_t data_4F6A5[16] = {
  0x00, 0x80, 0x3F, 0x71, 0x3D, 0x8A, 0x3E, 0x52, 0x38, 0xB6, 0x42, 0x71, 0x3D, 0xAA, 0x3F, 0x00
};
static const uint8_t data_4F6B5[16] = {
  0x80, 0xB3, 0x42, 0x71, 0x3D, 0x92, 0x40, 0x14, 0xAE, 0xB8, 0x42, 0x00, 0x00, 0x60, 0x40, 0x66
};
static const uint8_t data_4F6C5[16] = {
  0x66, 0xBB, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0x48, 0xE1, 0x6A, 0x40, 0x48, 0xE1, 0x1A, 0x40, 0x00
};
static const uint8_t data_4F6D5[16] = {
  0x00, 0xC0, 0x3F, 0xC3, 0x75, 0xB7, 0x42, 0x00, 0x00, 0xC0, 0x3F, 0xF6, 0x28, 0x84, 0xC0, 0xE1
};
static const uint8_t data_4F6E5[16] = {
  0x7A, 0x99, 0x42, 0x7B, 0x14, 0x3E, 0xC0, 0x9A, 0x19, 0x94, 0x42, 0x52, 0xB8, 0xBE, 0xBF, 0x48
};
static const uint8_t data_4F6F5[16] = {
  0x61, 0x95, 0x42, 0xC3, 0xF5, 0x28, 0xC0, 0x8F, 0xC2, 0x9A, 0x42, 0xCD, 0xCC, 0xCC, 0xBD, 0xAE
};
static const uint8_t data_4F705[16] = {
  0x47, 0x51, 0x40, 0x0A, 0xD7, 0x33, 0xC0, 0x00, 0x00, 0x80, 0x3F, 0xA4, 0x70, 0x97, 0x42, 0x00
};
static const uint8_t data_4F715[3] = {
  0x00, 0x80, 0x3F
};
static const uint32_t data_4F718 = 0xC297999A;
static const float data_4F71C[195] = {
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
static const char data_4FA28[] __attribute__((scn_string_literal)) = "/scene/01/D000/";
static const char data_4FA38[] __attribute__((scn_string_literal)) = "MAP1";
static const char data_4FA3D[] __attribute__((scn_string_literal)) = "/scene/01/D000/";
static const char data_4FA4D[] __attribute__((scn_string_literal)) = "MAP0";
static const char data_4FA52[] __attribute__((scn_string_literal)) = "/scene/01/D000/";
static const char data_4FA62[] __attribute__((scn_string_literal)) = "MAP3";
static const char data_4FA67[] __attribute__((scn_string_literal)) = "/scene/01/D000/";
static const char data_4FA77[] __attribute__((scn_string_literal)) = "MAP2";
static const char data_4FA7C[11] __attribute__((scn_string_literal)) = "bgm109.snd";
static const char data_4FA87[] __attribute__((scn_string_literal)) = "/scene/01/D000/";
static const char data_4FA97[] __attribute__((scn_string_literal)) = "YORU";
static const char data_4FA9C[13] __attribute__((scn_string_literal)) = "seqdata3.bin";
static const char data_4FAA9[5] __attribute__((scn_string_literal)) = "YORU";
static const char data_4FAAE[13] __attribute__((scn_string_literal)) = "seqdata4.bin";
static const char data_4FABB[5] __attribute__((scn_string_literal)) = "YORU";
static const char data_4FAC0[13] __attribute__((scn_string_literal)) = "seqdata5.bin";
static const char data_4FACD[5] __attribute__((scn_string_literal)) = "YORU";
static const char data_4FAD2[] __attribute__((scn_string_literal)) = "YORU";
static const char data_4FAD7[11] __attribute__((scn_string_literal)) = "SA1091A004";
static const char data_4FAE2[11] __attribute__((scn_string_literal)) = "SA1091A005";
static const char data_4FAED[11] __attribute__((scn_string_literal)) = "SA1091A006";
static const char data_4FAF8[12] __attribute__((scn_string_literal)) = "F1030B001";
static const uint32_t data_4FB04 = 1;
static const float data_4FB08[3] = {-122.1577f, 77.2416f, 1.0f};
static const int32_t data_4FB14 = 2;
static const uint32_t data_4FB18 = 0xC2F2506FU;
static const float data_4FB1C[2] = {78.0056f, 1.0f};
static const int32_t data_4FB24[2] = {196608, 1};
static const float data_4FB2C[3] = {-122.1577f, 77.2416f, 1.0f};
static const int32_t data_4FB38 = 2;
static const uint32_t data_4FB3C[2] = {0xC2F2AF83U, 0x429BC7E3U};
static const float data_4FB44 = 1.0f;
static const int32_t data_4FB48 = 196608;
static const char data_4FB4C[14] __attribute__((scn_string_literal)) = "HATO_STOPPER\012";
static const char data_4FB5A[18] __attribute__((scn_string_literal)) = "HATO_STOPPER_END\012";
static const char data_4FB6C[10] __attribute__((scn_string_literal)) = "SA_START\012";
static const char data_4FB76[10] __attribute__((scn_string_literal)) = "SA_END\012";
static const uint32_t data_4FB80 = 380;
static const int32_t data_4FB84[6] = {1, 390, 0, 1018, 0, -1};
static const uint32_t data_4FB9C = 510;
static const int32_t data_4FBA0[6] = {1, 525, 0, 1020, 0, -1};
static const uint32_t data_4FBB8 = 150;
static const int32_t data_4FBBC[6] = {1, 160, 0, 1019, 0, -1};
static const uint32_t data_4FBD4 = 530;
static const int32_t data_4FBD8[6] = {1, 970, 0, 1014, 0, -1};
static const uint32_t data_4FBF0 = 911;
static const int32_t data_4FBF4[6] = {1, 155, 0, 1015, 0, -1};
static const char data_4FC0C[7] __attribute__((scn_string_literal)) = "FREE02";
static const char data_4FC13[7] __attribute__((scn_string_literal)) = "FREE03";
static const char data_4FC1A[7] __attribute__((scn_string_literal)) = "FREE04";
static const char data_4FC21[7] __attribute__((scn_string_literal)) = "FREE01";
static const char data_4FC28[6] __attribute__((scn_string_literal)) = "E1040";
static const char data_4FC2E[10] __attribute__((scn_string_literal)) = "E1040A028";
static const char data_4FC38[10] __attribute__((scn_string_literal)) = "E1040A029";
static const char data_4FC42[6] __attribute__((scn_string_literal)) = "E1040";
static const char data_4FC48[10] __attribute__((scn_string_literal)) = "E1040A030";
static const char data_4FC52[10] __attribute__((scn_string_literal)) = "E1040A031";
static const char data_4FC5C[10] __attribute__((scn_string_literal)) = "E1040A032";
static const char data_4FC66[6] __attribute__((scn_string_literal)) = "E1040";
static const Vector3 data_4FC6C = {.x = 41.99f, .y = 0.0f, .z = 23.13f};
static const Vector3 data_4FC78 = {.x = 9.6f, .y = 0.0f, .z = 47.17f};
static const char data_4FC84[13] __attribute__((scn_string_literal)) = "e1shugyo.snd";
static const char data_4FC91[10] __attribute__((scn_string_literal)) = "E1040A001";
static const char data_4FC9B[10] __attribute__((scn_string_literal)) = "E1040A003";
static const char data_4FCA5[10] __attribute__((scn_string_literal)) = "E1040A009";
static const char data_4FCAF[10] __attribute__((scn_string_literal)) = "E1040A010";
static const char data_4FCB9[10] __attribute__((scn_string_literal)) = "E1040A012";
static const char data_4FCC3[10] __attribute__((scn_string_literal)) = "E1040A013";
static const char data_4FCCD[10] __attribute__((scn_string_literal)) = "E1040A014";
static const char data_4FCD7[10] __attribute__((scn_string_literal)) = "E1040A018";
static const char data_4FCE1[10] __attribute__((scn_string_literal)) = "E1040A020";
static const char data_4FCEB[10] __attribute__((scn_string_literal)) = "E1040A015";
static const char data_4FCF5[10] __attribute__((scn_string_literal)) = "E1040A016";
static const char data_4FCFF[10] __attribute__((scn_string_literal)) = "E1040A017";
static const char data_4FD09[6] __attribute__((scn_string_literal)) = "E1040";
static const char data_4FD0F[9] __attribute__((scn_string_literal)) = "E1040";
static const Vector3 data_4FD18 = {.x = 40.7094f, .y = 0.0f, .z = 21.8232f};
static const Vector3 data_4FD24 = {.x = 39.43f, .y = 0.0f, .z = 21.84f};
static const Vector3 data_4FD30 = {.x = 12.35f, .y = 0.0f, .z = 47.05f};
static const char data_4FD3C[30] __attribute__((scn_string_literal)) = "training_npc_too_close:  set\012";
static const char data_4FD5A[20] __attribute__((scn_string_literal)) = "\012yamagisi ga iru!\012\012";
static const char data_4FD6E[15] __attribute__((scn_string_literal)) = "\012ryu ga iru!\012\012";
static const char data_4FD7D[54] __attribute__((scn_string_literal)) = "training_npc_too_close reset before training starts \012";
static const char data_4FDB3[12] __attribute__((scn_string_literal)) = "lock_start\012";
static const char data_4FDBF[17] __attribute__((scn_string_literal)) = "ykhi_hdl = %x\012";
static void (*data_4FDD0)(void) = script_33E3B;
static void (*const data_4FDD4)(void) = script_33E3E;
static void (*const data_4FDD8)(int32_t) = script_33E41;
static void (*const data_4FDDC)(int32_t) = script_33E72;
static void (*const data_4FDE0)(int32_t) = script_33EA3;
static void (*const data_4FDE4)(int32_t) = script_33ED4;
static const int32_t data_4FDE8[57] = {
  0, 0, -3640, 0, 0, -2366, -4369, 1820, -546, 0, 0, -5279, 0, 0, -6007, 0, 0, -2912, 0, 0, -910, 0, 0, -6553, 0, 0, -6735, 0, 0, -3640, 0, 0, -182,
  0, 0, -5461, 0, 0, -6007, 0, 0, -2366, 0, 0, 0, 0, 0, -5643, 0, 0, -4733, 0, 0, -2366, -1456, 0, 728
};
static const Vector4 data_4FECC = {.x = 1.0f, .y = 0.3f, .z = 0.3f, .w = 0.3f};
static const Vector3i data_4FEDC = {.x = 0, .y = 49152, .z = 0};
static const Vector3 data_4FEE8 = {.x = 7.0f, .y = 0.0f, .z = 28.54f};
static const uint32_t data_4FEF4 = 1;
static const float data_4FEF8[3] = {20.48f, 28.54f, 1.0f};
static const int32_t data_4FF04 = 196608;
static const uint32_t data_4FF08 = 0xFA00F8;
static const int32_t data_4FF0C[3] = {16187637, 16449782, 249};
static const Vector3 data_4FF18 = {.x = 22.95f, .y = 0.0f, .z = 31.02f};
static const Vector3i data_4FF24 = {.x = 0, .y = 7281, .z = 0};
static const char data_4FF30[11] __attribute__((scn_string_literal)) = "bgm013.snd";
static const char data_4FF3B[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_4FF40[13] __attribute__((scn_string_literal)) = "a1_senfk.snd";
static const char data_4FF4D[13] __attribute__((scn_string_literal)) = "battle_1.snd";
static const char data_4FF5A[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_4FF5F[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_4FF64[6] __attribute__((scn_string_literal)) = "01REV";
static const char data_4FF6A[7] __attribute__((scn_string_literal)) = "DRAUTH";
static const char data_4FF71[] __attribute__((scn_string_literal)) = "/scene/01/D000/";
static const char data_4FF81[] __attribute__((scn_string_literal)) = "DRAUTH";
static const char data_4FF88[6] __attribute__((scn_string_literal)) = "01REV";
static const char data_4FF8E[7] __attribute__((scn_string_literal)) = "DRAUTH";
static const char data_4FF95[1] __attribute__((scn_string_literal)) = "";
static const char data_4FF96[7] __attribute__((scn_string_literal)) = "DRAUTH";
static const char data_4FF9D[1] __attribute__((scn_string_literal)) = "";
static const char data_4FF9E[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_4FFA3[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_4FFA8[13] __attribute__((scn_string_literal)) = "e1fuksyu.snd";
static const char data_4FFB5[] __attribute__((scn_string_literal)) = "/scene/01/D000/";
static const char data_4FFC5[] __attribute__((scn_string_literal)) = "DRSA";
static const char data_4FFCA[6] __attribute__((scn_string_literal)) = "E1068";
static const char data_4FFD0[6] __attribute__((scn_string_literal)) = "E1068";
static const char data_4FFD6[6] __attribute__((scn_string_literal)) = "DRSA";
static const Vector3 data_4FFDC = {.x = 10000.0f, .y = 10000.0f, .z = 10000.0f};
static const Vector3 data_4FFE8 = {.x = 37.361f, .y = 0.08f, .z = 18.253f};
static const Vector3 data_4FFF4 = {.x = 34.845001f, .y = 0.08f, .z = 18.173f};
static const Vector3 data_50000 = {.x = 38.498001f, .y = 0.08f, .z = 21.765068f};
static const Vector3 data_5000C = {.x = 34.776001f, .y = 0.08f, .z = 20.25f};
static const float data_50018[3] = {35.1f, 0.04f, 21.63f};
static const Vector3 data_50024 = {.x = 35.6399f, .y = 0.08f, .z = 21.827101f};
static const Vector3 data_50030 = {.x = 40.2188f, .y = 0.08f, .z = 22.0f};
static const char data_5003C[12] __attribute__((scn_string_literal)) = "E1068B009";
static const Vector3 data_50048 = {.x = 0.0f, .y = 0.0f, .z = 0.0f};
static const uint8_t data_50054[12] = {0};
static const uint32_t data_50060[3] = {0x42200000U, 0x3DA3D70AU, 0x41A00000U};
static const Vector3 data_5006C = {.x = 0.51869f, .y = 0.08f, .z = -1.00386f};
static const Vector3i data_50078 = {.x = 0, .y = 16384, .z = 0};
static const Vector3 data_50084 = {.x = 40.7f, .y = 1.0f, .z = 18.75f};
static const int32_t data_50090[10] = {0, 5461, 0, 0, 54613, 0, 0, 53703, 0, 63715};
static const int32_t data_500B8[10] = {0, 61348, 0, 3640, 5461, 0, 0, 5461, 0, 12743};
static const Vector3 data_500E0 = {.x = 40.3913f, .y = 1.38f, .z = 19.1728f};
static const Vector3 data_500EC = {.x = 40.705002f, .y = 0.9754f, .z = 18.932301f};
static const Vector3 data_500F8 = {.x = 40.5f, .y = 1.431f, .z = 18.85f};
static const char data_50104[10] __attribute__((scn_string_literal)) = "E1068A001";
static const char data_5010E[10] __attribute__((scn_string_literal)) = "E1068B001";
static const char data_50118[10] __attribute__((scn_string_literal)) = "E1068A002";
static const char data_50122[10] __attribute__((scn_string_literal)) = "E1068B004";
static const char data_5012C[10] __attribute__((scn_string_literal)) = "E1068B005";
static const char data_50136[10] __attribute__((scn_string_literal)) = "E1068B006";
static const char data_50140[10] __attribute__((scn_string_literal)) = "E1068A003";
static const char data_5014A[10] __attribute__((scn_string_literal)) = "E1068B007";
static const char data_50154[88] = "E1068B008\000\000\00033\243@\000\000HD{\024\256>U\026\024\377\000\000\000\000\000\000\000\000\\@Z\242\\@Z\242\270\036\205>\232\231\231>\232\231\231>\232\231\231>\000\000\200?33\263>\354Q\270>\024\256\307>)\\\217\276\256Ga\277\024\256\307\276";
static const Vector4 data_501AC = {.x = 1.0f, .y = 0.2f, .z = 0.2f, .w = 0.2f};
static const int32_t data_501BC[57] = {
  0, 0, -5461, 0, -2366, -1092, 0, -182, 1092, 0, -546, -6735, 0, 0, -6189, 0, 0, -6553, 0, -182, 3822, 0, 0, -13289, 0, 0, -7099, 0, 0, -8556, 0, 0,
  2912, 0, 182, -8738, 0, 0, -6007, 0, 0, -8738, 0, 182, 1638, 0, -728, -5279, 0, -1274, -6735, 0, -910, -8738, 0, 0, 1638
};
static const int32_t data_502A0[57] = {
  0, 0, -5461, 0, 5461, -1274, 0, -182, 1092, 0, -546, -6735, 0, 0, -4369, 0, 0, -5279, 0, -182, 3822, 0, 0, -13289, 0, -182, -4004, 0, 0, -6735, 0,
  0, 3640, 0, 182, -8738, 0, -546, -7827, 0, -546, -3822, 0, 182, 2730, 0, -728, -5279, 0, -1274, -6007, 0, -910, -3822, 0, 546, 3094
};
static const int32_t data_50384[57] = {
  0, 0, -5461, 0, 546, 0, 0, -182, 1092, 0, -546, -6735, 0, 0, -4004, 0, 0, -6553, 0, -182, 1274, 0, 0, -4915, 0, 0, -5279, 0, 0, -8009, 0, 0, 1456,
  0, 182, -8738, 0, 0, -6007, 0, 0, -8738, 0, 182, 1638, 0, -728, -5279, 0, -1274, -6735, 0, -910, -8738, 0, 0, 1638
};
static const Vector4 data_50468 = {.x = 1.0f, .y = 0.3f, .z = 0.3f, .w = 0.3f};
static const char data_50478[] __attribute__((scn_string_literal)) = "/scene/01/D000/";
static const char data_50488[] __attribute__((scn_string_literal)) = "DJHN_01";
static const char data_50490[] __attribute__((scn_string_literal)) = "/scene/01/D000/";
static const char data_504A0[] __attribute__((scn_string_literal)) = "DJHN_02";
static const char data_504A8[] __attribute__((scn_string_literal)) = "/scene/01/D000/";
static const char data_504B8[] __attribute__((scn_string_literal)) = "DJHN_03";
static const char data_504C0[] __attribute__((scn_string_literal)) = "/scene/01/D000/";
static const char data_504D0[] __attribute__((scn_string_literal)) = "DJHN_04";
static const char data_504D8[] __attribute__((scn_string_literal)) = "/scene/01/D000/";
static const char data_504E8[] __attribute__((scn_string_literal)) = "DJHN_05";
static const char data_504F0[] __attribute__((scn_string_literal)) = "/scene/01/D000/";
static const char data_50500[] __attribute__((scn_string_literal)) = "DJHN_06";
static const char data_50508[] __attribute__((scn_string_literal)) = "/scene/01/D000/";
static const char data_50518[] __attribute__((scn_string_literal)) = "DJHN_07";
static const char data_50520[] __attribute__((scn_string_literal)) = "/scene/01/D000/";
static const char data_50530[] __attribute__((scn_string_literal)) = "DJHN_MOT";
static const char data_50539[13] __attribute__((scn_string_literal)) = "seqdata1.bin";
static const char data_50546[8] __attribute__((scn_string_literal)) = "DJHN_01";
static const char data_5054E[13] __attribute__((scn_string_literal)) = "seqdata2.bin";
static const char data_5055B[8] __attribute__((scn_string_literal)) = "DJHN_02";
static const char data_50563[13] __attribute__((scn_string_literal)) = "seqdata3.bin";
static const char data_50570[8] __attribute__((scn_string_literal)) = "DJHN_03";
static const char data_50578[13] __attribute__((scn_string_literal)) = "seqdata4.bin";
static const char data_50585[8] __attribute__((scn_string_literal)) = "DJHN_04";
static const char data_5058D[13] __attribute__((scn_string_literal)) = "seqdata5.bin";
static const char data_5059A[8] __attribute__((scn_string_literal)) = "DJHN_05";
static const char data_505A2[13] __attribute__((scn_string_literal)) = "seqdata6.bin";
static const char data_505AF[8] __attribute__((scn_string_literal)) = "DJHN_06";
static const char data_505B7[13] __attribute__((scn_string_literal)) = "seqdata7.bin";
static const char data_505C4[8] __attribute__((scn_string_literal)) = "DJHN_07";
static const char data_505CC[8] __attribute__((scn_string_literal)) = "DJHN_01";
static const char data_505D4[8] __attribute__((scn_string_literal)) = "DJHN_02";
static const char data_505DC[8] __attribute__((scn_string_literal)) = "DJHN_03";
static const char data_505E4[8] __attribute__((scn_string_literal)) = "DJHN_04";
static const char data_505EC[8] __attribute__((scn_string_literal)) = "DJHN_05";
static const char data_505F4[8] __attribute__((scn_string_literal)) = "DJHN_06";
static const char data_505FC[8] __attribute__((scn_string_literal)) = "DJHN_07";
static const char data_50604[9] __attribute__((scn_string_literal)) = "DJHN_MOT";
static const char data_5060D[13] __attribute__((scn_string_literal)) = "a1_yanji.snd";
static const char data_5061A[8] __attribute__((scn_string_literal)) = "01JUCEA";
static const char data_50622[8] __attribute__((scn_string_literal)) = "01JUCEA";
static const char data_5062A[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_5062F[5] __attribute__((scn_string_literal)) = "VMSA";
static const char data_50634[16] __attribute__((scn_string_literal)) = "ykhi free = %x\012";
static const uint8_t data_50644[12] = {0};
static const char data_50650[] __attribute__((scn_string_literal)) = "COKE";
static const char data_50655[] __attribute__((scn_string_literal)) = "FATG";
static const char data_5065A[] __attribute__((scn_string_literal)) = "FATO";
static const char data_5065F[] __attribute__((scn_string_literal)) = "SPRT";
static const char data_50664[] __attribute__((scn_string_literal)) = "CAFE";
static const char data_50669[] __attribute__((scn_string_literal)) = "/scene/01/D000/";
static const char data_50679[] __attribute__((scn_string_literal)) = "VMSA";
static const char data_5067E[] __attribute__((scn_string_literal)) = "/scene/01/D000/";
static const uint8_t data_5068E[4] = {
  0x4F, 0x4D, 0x47, 0x00
};
static const char data_50692[11] __attribute__((scn_string_literal)) = "M_MOBJ.BIN";
static const char data_5069D[] __attribute__((scn_string_literal)) = "/scene/01/D000/";
static const char data_506AD[] __attribute__((scn_string_literal)) = "SHUKI";
static const char data_506B3[14] __attribute__((scn_string_literal)) = "scene/01/D000";
static const char data_506C1[] __attribute__((scn_string_literal)) = "SCRTEST.BIN";
static const char data_506CD[] __attribute__((scn_string_literal)) = "BUSS";
static const char data_506D2[6] __attribute__((scn_string_literal)) = "01YOB";
static const char data_506D8[12] __attribute__((scn_string_literal)) = "system1.snd";
static const char data_506E4[11] __attribute__((scn_string_literal)) = "amb022.snd";
static const char data_506EF[7] __attribute__((scn_string_literal)) = "SELECT";
static const char data_506F6[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_506FB[13] __attribute__((scn_string_literal)) = "f1dobuit.snd";
static const char data_50708[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_5070D[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_50712[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_50717[12] __attribute__((scn_string_literal)) = "system1.snd";
static const char data_50723[11] __attribute__((scn_string_literal)) = "amb051.snd";
static const char data_5072E[7] __attribute__((scn_string_literal)) = "SELECT";
static const char data_50735[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_5073A[13] __attribute__((scn_string_literal)) = "f1dobuit.snd";
static const char data_50747[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_5074C[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_50751[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_50756[12] __attribute__((scn_string_literal)) = "system1.snd";
static const char data_50762[11] __attribute__((scn_string_literal)) = "amb050.snd";
static const char data_5076D[7] __attribute__((scn_string_literal)) = "SELECT";
static const char data_50774[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_50779[13] __attribute__((scn_string_literal)) = "f1dobuit.snd";
static const char data_50786[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_5078B[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_50790[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_50795[12] __attribute__((scn_string_literal)) = "system1.snd";
static const char data_507A1[11] __attribute__((scn_string_literal)) = "amb052.snd";
static const char data_507AC[7] __attribute__((scn_string_literal)) = "SELECT";
static const char data_507B3[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_507B8[13] __attribute__((scn_string_literal)) = "f1dobuit.snd";
static const char data_507C5[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_507CA[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_507CF[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_507D4[12] __attribute__((scn_string_literal)) = "system1.snd";
static const char data_507E0[11] __attribute__((scn_string_literal)) = "amb021.snd";
static const char data_507EB[7] __attribute__((scn_string_literal)) = "SELECT";
static const char data_507F2[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_507F7[13] __attribute__((scn_string_literal)) = "f1dobuit.snd";
static const char data_50804[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_50809[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_5080E[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_50813[] __attribute__((scn_string_literal)) = "SHUKI";
static const char data_50819[] __attribute__((scn_string_literal)) = "/scene/01/D000/";
static const char data_50829[] __attribute__((scn_string_literal)) = "SHUKI";
static const uint8_t data_5082F[16] = {
  0x00, 0x9E, 0x02, 0xA0, 0x02, 0xA2, 0x02, 0xA5, 0x02, 0xAC, 0x02, 0xAA, 0x02, 0xA8, 0x02, 0x00
};
static const uint8_t data_5083F[16] = {
  0x03, 0xF8, 0x00, 0xFA, 0x00, 0xF5, 0x00, 0xF7, 0x00, 0xF6, 0x00, 0xFB, 0x00, 0xF9, 0x00, 0x00
};
static const uint8_t data_5084F[1] = {
  0x03
};
static const char data_50850[61] __attribute__((scn_string_literal)) = "WAZA Warning : handle is not NULL, and create same handle !\012";
static const char data_5088D[30] __attribute__((scn_string_literal)) = "POS POS_RYOU_0 = %f, %f, %f.\012";
static const char data_508AB[30] __attribute__((scn_string_literal)) = "POS POS_KAIS_0 = %f, %f, %f.\012";
static const char data_508C9[29] __attribute__((scn_string_literal)) = "define ANG_RYOU_0 DEG(%d.0)\012";
static const char data_508E6[29] __attribute__((scn_string_literal)) = "define ANG_KAIS_0 DEG(%d.0)\012";
static const char data_50903[4] __attribute__((scn_string_literal)) = "%s\012";
static const char data_50907[37] __attribute__((scn_string_literal)) = "Konnna Error ha Detekonai hazu : %d\012";
static const Vector3 data_5092C = {.x = 1.0f, .y = 1.0f, .z = 1.0f};
static const Vector3i data_50938 = {.x = 0, .y = 0, .z = 0};
static const char data_50944[14] __attribute__((scn_string_literal)) = "Entry Model!\012";
static const char data_50952[38] __attribute__((scn_string_literal)) = "WZ ModoriMove Limit Over !!!!!!!!!!!\012";
static const char data_50978[30] __attribute__((scn_string_literal)) = "WazaDen: Load PackFile %s:%s\012";
static const char data_50996[34] __attribute__((scn_string_literal)) = "WazaDen: Now Loading PackFile.\012";
static const uint32_t data_509B8 = 64;
static const int32_t data_509BC[48] = {
  20, 16, 4, 0, -1, -1, -1, 1, -1, 5, 3, 256, 20, 16, 4, 0, -1, -1, -1, 1, -1, 7, 3, 512, 20, 16, 4, 0, -1, -1, -1, 9, -1, 11, 3, 256, 20, 16, 4, 0,
  -1, -1, 18, -1, 13, 15, 17, 1
};
static const float data_50A7C[3] = {-113.1f, 99.6f, 1.0f};
static const int32_t data_50A88 = 196608;
static const char data_50A8C[10] __attribute__((scn_string_literal)) = "E1064A008";
static const char data_50A96[10] __attribute__((scn_string_literal)) = "E1064A009";
static const char data_50AA0[10] __attribute__((scn_string_literal)) = "E1064A010";
static const char data_50AAA[10] __attribute__((scn_string_literal)) = "E1064A011";
static const char data_50AB4[10] __attribute__((scn_string_literal)) = "E1064B007";
static const char data_50ABE[10] __attribute__((scn_string_literal)) = "E1064B008";
static const char data_50AC8[10] __attribute__((scn_string_literal)) = "E1064B009";
static const char data_50AD2[10] __attribute__((scn_string_literal)) = "E1064B010";
static const char data_50ADC[10] __attribute__((scn_string_literal)) = "E1064B011";
static const char data_50AE6[10] __attribute__((scn_string_literal)) = "E1064B012";
static const char data_50AF0[10] __attribute__((scn_string_literal)) = "E1064B013";
static const char data_50AFA[10] __attribute__((scn_string_literal)) = "E1064B014";
static const char data_50B04[10] __attribute__((scn_string_literal)) = "E1064B015";
static const char data_50B0E[10] __attribute__((scn_string_literal)) = "E1064B016";
static const char data_50B18[10] __attribute__((scn_string_literal)) = "E1064B018";
static const char data_50B22[10] __attribute__((scn_string_literal)) = "E1064B019";
static const char data_50B2C[10] __attribute__((scn_string_literal)) = "E1064B020";
static const char data_50B36[10] __attribute__((scn_string_literal)) = "E1064B021";
static const char data_50B40[10] __attribute__((scn_string_literal)) = "E1064B022";
static const char data_50B4A[10] __attribute__((scn_string_literal)) = "E1064B023";
static const char data_50B54[10] __attribute__((scn_string_literal)) = "E1064B024";
static const char data_50B5E[10] __attribute__((scn_string_literal)) = "E1064C008";
static const char data_50B68[10] __attribute__((scn_string_literal)) = "E1064C010";
static const char data_50B72[10] __attribute__((scn_string_literal)) = "E1064C011";
static const char data_50B7C[10] __attribute__((scn_string_literal)) = "E1064C012";
static const char data_50B86[10] __attribute__((scn_string_literal)) = "E1064C013";
static const char data_50B90[10] __attribute__((scn_string_literal)) = "E1064C014";
static const char data_50B9A[10] __attribute__((scn_string_literal)) = "E1064C015";
static const char data_50BA4[10] __attribute__((scn_string_literal)) = "E1064C016";
static const char data_50BAE[10] __attribute__((scn_string_literal)) = "E1064C017";
static const char data_50BB8[10] __attribute__((scn_string_literal)) = "E1064C018";
static const char data_50BC2[10] __attribute__((scn_string_literal)) = "E1064C019";
static const char data_50BCC[10] __attribute__((scn_string_literal)) = "E1064C020";
static const char data_50BD6[10] __attribute__((scn_string_literal)) = "E1064C021";
static const char data_50BE0[10] __attribute__((scn_string_literal)) = "E1064C022";
static const char data_50BEA[7] __attribute__((scn_string_literal)) = "開始０";
static const char data_50BF1[7] __attribute__((scn_string_literal)) = "成功０";
static const char data_50BF8[7] __attribute__((scn_string_literal)) = "失敗０";
static const char data_50BFF[7] __attribute__((scn_string_literal)) = "開始１";
static const char data_50C06[7] __attribute__((scn_string_literal)) = "成功１";
static const char data_50C0D[7] __attribute__((scn_string_literal)) = "失敗１";
static const char data_50C14[7] __attribute__((scn_string_literal)) = "開始２";
static const char data_50C1B[7] __attribute__((scn_string_literal)) = "成功２";
static const char data_50C22[7] __attribute__((scn_string_literal)) = "失敗２";
static const char data_50C29[7] __attribute__((scn_string_literal)) = "開始３";
static const char data_50C30[7] __attribute__((scn_string_literal)) = "成功３";
static const char data_50C37[7] __attribute__((scn_string_literal)) = "失敗３";
static const char data_50C3E[7] __attribute__((scn_string_literal)) = "開始４";
static const char data_50C45[7] __attribute__((scn_string_literal)) = "成功４";
static const char data_50C4C[7] __attribute__((scn_string_literal)) = "失敗４";
static const char data_50C53[7] __attribute__((scn_string_literal)) = "開始５";
static const char data_50C5A[7] __attribute__((scn_string_literal)) = "成功５";
static const char data_50C61[7] __attribute__((scn_string_literal)) = "失敗５";
static const char data_50C68[7] __attribute__((scn_string_literal)) = "開始６";
static const char data_50C6F[7] __attribute__((scn_string_literal)) = "成功６";
static const char data_50C76[7] __attribute__((scn_string_literal)) = "失敗６";
static const char data_50C7D[9] __attribute__((scn_string_literal)) = "オート１";
static const char data_50C86[9] __attribute__((scn_string_literal)) = "オート２";
static const char data_50C8F[9] __attribute__((scn_string_literal)) = "オート３";
static const Vector3 data_50C98 = {.x = -68.2295f, .y = 0.08239f, .z = 53.425629f};
static const Vector3i data_50CA4 = {.x = 0, .y = 13034, .z = 0};
static const char data_50CB0[9] __attribute__((scn_string_literal)) = "DR12_001";
static const char data_50CB9[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_50CBE[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_50CC3[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_50CC8[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_50CCD[12] __attribute__((scn_string_literal)) = "system1.snd";
static const char data_50CD9[11] __attribute__((scn_string_literal)) = "bgm067.snd";
static const char data_50CE4[11] __attribute__((scn_string_literal)) = "qtebat.snd";
static const char data_50CEF[] __attribute__((scn_string_literal)) = "/scene/01/D000/";
static const char data_50CFF[] __attribute__((scn_string_literal)) = "DEQB";
static const char data_50D04[6] __attribute__((scn_string_literal)) = "E1064";
static const char data_50D0A[21] __attribute__((scn_string_literal)) = "/scene/01/D000/pack/";
static const char data_50D1F[5] __attribute__((scn_string_literal)) = "DUMY";
static const char data_50D24[6] __attribute__((scn_string_literal)) = "E1064";
static const char data_50D2A[6] __attribute__((scn_string_literal)) = "DEQB";
static const Vector3 data_50D30 = {.x = 0.0f, .y = 0.0f, .z = 0.0f};
static const Vector3i data_50D3C = {.x = 0, .y = 0, .z = 0};
typedef struct Row_50D48 { int32_t w0; float w1; } Row_50D48;
static const Row_50D48 data_50D48[3] = {
  {0, 1.0f},
  {0, 0.0f},
  {0, 2.0f}
};
static const float data_50D60 = -2.0f;
static const Vector3 data_50D64 = {.x = -168.0f, .y = 0.0f, .z = -53.8f};
static const Vector3i data_50D70 = {.x = 0, .y = 16384, .z = 0};
static const uint32_t data_50D7C = 256;
static const int32_t data_50D80[47] = {
  20, 20, 4, 2, -1, -1, -1, 1, -1, 3, 5, 512, 20, 20, 4, 3, -1, 19, -1, -1, -1, 7, 9, 512, 20, 20, 4, 5, -1, 15, -1, -1, -1, 13, 11, 512, 20, 20, 4,
  0, -1, -1, -1, -1, -1, 15, 17
};
static const uint8_t data_50E3C[4] = {0};
static const int32_t data_50E40[7] = {6, 3, 5, 2, 1, 7, 4};
static const uint8_t data_50E5C[4] = {0};
static const int32_t data_50E60[11] = {11, 6, 3, 10, 5, 2, 1, 8, 7, 9, 4};
static const uint8_t data_50E8C[4] = {0};
static const int32_t data_50E90[36] = {
  1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36
};
static const char data_50F20[5] __attribute__((scn_string_literal)) = "YQ14";
static const char data_50F25[] __attribute__((scn_string_literal)) = "YDBD";
static const char data_50F2A[5] __attribute__((scn_string_literal)) = "YQ14";
static const char data_50F2F[] __attribute__((scn_string_literal)) = "BC0D";
static const char data_50F34[5] __attribute__((scn_string_literal)) = "YQ14";
static const char data_50F39[] __attribute__((scn_string_literal)) = "BC1D";
static const char data_50F3E[5] __attribute__((scn_string_literal)) = "YQ14";
static const char data_50F43[] __attribute__((scn_string_literal)) = "KADY";
static const Vector4 data_50F48 = {.x = 1.0f, .y = 0.4f, .z = 0.4f, .w = 0.4f};
static const uint32_t data_50F58 = 0xC2ED0000;
static const float data_50F5C[3] = {-1.75f, 69.0f, 1.0f};
static const int32_t data_50F68 = 1;
static const float data_50F6C[4] = {-123.2f, -1.75f, 65.5f, 1.0f};
static const int32_t data_50F7C[2] = {2, -11062528};
static const uint32_t data_50F84 = 0xC2EE0000;
static const float data_50F88[3] = {-1.75f, 69.7174f, 0.0f};
static const int32_t data_50F94 = 1;
static const float data_50F98[4] = {-118.5f, -1.75f, 69.0f, 0.5f};
static const int32_t data_50FA8 = 1;
static const float data_50FAC[4] = {-123.2f, -1.75f, 65.5f, 1.0f};
static const int32_t data_50FBC[2] = {2, -11062528};
static const uint32_t data_50FC4 = 0xC2EE9439;
static const float data_50FC8 = -1.75f;
static const uint32_t data_50FCC[2] = {0x428C9340U, 0};
static const int32_t data_50FD4 = 1;
static const float data_50FD8[4] = {-118.5f, -1.75f, 69.0f, 1.0f};
static const int32_t data_50FE8 = 1;
static const float data_50FEC[4] = {-123.2f, -1.75f, 65.5f, 1.0f};
static const int32_t data_50FFC[2] = {2, -11062528};
static const uint32_t data_51004 = 0xC2EF8858U;
static const float data_51008[3] = {-1.75f, 71.4291f, 0.0f};
static const int32_t data_51014 = 1;
static const float data_51018[4] = {-118.5f, -1.75f, 69.0f, 1.0f};
static const int32_t data_51028 = 1;
static const float data_5102C[4] = {-123.2f, -1.75f, 65.5f, 1.0f};
static const int32_t data_5103C[2] = {2, -11062528};
static const char data_51044[10] __attribute__((scn_string_literal)) = "E1002B001";
static const char data_5104E[10] __attribute__((scn_string_literal)) = "E1002B002";
static const char data_51058[10] __attribute__((scn_string_literal)) = "E1002B003";
static const char data_51062[10] __attribute__((scn_string_literal)) = "E1002B004";
static const char data_5106C[10] __attribute__((scn_string_literal)) = "E1002B005";
static const char data_51076[10] __attribute__((scn_string_literal)) = "E1002B006";
static const char data_51080[10] __attribute__((scn_string_literal)) = "E1002B007";
static const char data_5108A[10] __attribute__((scn_string_literal)) = "E1002B008";
static const char data_51094[10] __attribute__((scn_string_literal)) = "E1002B009";
static const char data_5109E[10] __attribute__((scn_string_literal)) = "E1002B010";
static const char data_510A8[10] __attribute__((scn_string_literal)) = "E1002C001";
static const char data_510B2[10] __attribute__((scn_string_literal)) = "E1002C002";
static const char data_510BC[10] __attribute__((scn_string_literal)) = "E1002C003";
static const char data_510C6[10] __attribute__((scn_string_literal)) = "E1002C004";
static const char data_510D0[10] __attribute__((scn_string_literal)) = "E1002C005";
static const char data_510DA[10] __attribute__((scn_string_literal)) = "E1002C006";
static const char data_510E4[10] __attribute__((scn_string_literal)) = "E1002C007";
static const char data_510EE[10] __attribute__((scn_string_literal)) = "E1002C008";
static const char data_510F8[10] __attribute__((scn_string_literal)) = "E1002C009";
static const char data_51102[10] __attribute__((scn_string_literal)) = "E1002D001";
static const char data_5110C[7] __attribute__((scn_string_literal)) = "開始０";
static const char data_51113[7] __attribute__((scn_string_literal)) = "成功０";
static const char data_5111A[7] __attribute__((scn_string_literal)) = "失敗０";
static const char data_51121[7] __attribute__((scn_string_literal)) = "開始１";
static const char data_51128[7] __attribute__((scn_string_literal)) = "成功１";
static const char data_5112F[7] __attribute__((scn_string_literal)) = "失敗１";
static const char data_51136[7] __attribute__((scn_string_literal)) = "開始２";
static const char data_5113D[7] __attribute__((scn_string_literal)) = "成功２";
static const char data_51144[7] __attribute__((scn_string_literal)) = "失敗２";
static const char data_5114B[7] __attribute__((scn_string_literal)) = "開始３";
static const char data_51152[7] __attribute__((scn_string_literal)) = "成功３";
static const char data_51159[7] __attribute__((scn_string_literal)) = "失敗３";
static const char data_51160[7] __attribute__((scn_string_literal)) = "開始４";
static const char data_51167[7] __attribute__((scn_string_literal)) = "成功４";
static const char data_5116E[7] __attribute__((scn_string_literal)) = "失敗４";
static const char data_51175[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_5117A[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_5117F[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_51184[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_51189[11] __attribute__((scn_string_literal)) = "bgm067.snd";
static const char data_51194[12] __attribute__((scn_string_literal)) = "n1014_4.snd";
static const char data_511A0[13] __attribute__((scn_string_literal)) = "e1senqte.snd";
static const char data_511AD[23] = "qtebat.snd\000\000\000\000\000ﾍﾌLｽ";
static const char data_511C4[6] __attribute__((scn_string_literal)) = "E1002";
static const char data_511CA[7] __attribute__((scn_string_literal)) = "A01114";
static const char data_511D1[11] __attribute__((scn_string_literal)) = "M_YQ14.BIN";
static const char data_511DC[12] __attribute__((scn_string_literal)) = "M_01114.BIN";
static const char data_511E8[7] __attribute__((scn_string_literal)) = "A01114";
static const char data_511EF[9] __attribute__((scn_string_literal)) = "E1002";
static const char data_511F8[4] __attribute__((scn_string_literal)) = "";
