#pragma once

#include "scenario.h"
#include "motion/motion.h"
#include "scene/02/jhd0/m_01_fuk.h"

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
static int32_t script_387C(int32_t arg0);
static void script_389A(float arg0);
static int32_t script_38B9(int32_t arg0, int32_t arg1);
static void script_3931(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4);
static int32_t script_3EE3(int32_t arg0);
static void script_3FB4(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_3FC4(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_40C8(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_426A(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_43CB(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_452C(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_47CA(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_4898(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_48FF(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_4966(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_49AD(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_4AE3(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_53A4(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_54C2(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_5B87(int32_t arg0, int32_t arg1);
static void script_5C5D(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_6B70(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_6BDC(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_7096(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_7750(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_9030(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_9751(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_9EDB(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_9FB3(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_A42A(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_B3BD(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_B807(int32_t arg0, DSWKDoorEntry *arg1, int32_t *arg2, DoorResourceDescriptor *arg3, MapObjectRecord *arg4, int32_t arg5, int32_t arg6);
static void script_B993(DSWKDoorEntry *arg0, int32_t *arg1, DoorResourceDescriptor *arg2, MapObjectRecord *arg3, int32_t arg4);
static void script_B9C7(void);
static void script_BA30(int32_t arg0, int32_t arg1);
static void script_BACD(int32_t arg0, int32_t arg1);
static void script_BB07(void);
static void script_BB6A(void);
static void script_BBCD(int32_t arg0);
static void script_BC04(int32_t arg0);
static void script_BC3B(int32_t arg0);
static void script_BE7D(int32_t arg0);
static void script_BEC6(int32_t arg0);
static void script_BF0F(int32_t arg0, int32_t arg1);
static void script_BF85(int32_t arg0);
static void script_BF93(int32_t arg0);
static void script_BFA1(int32_t arg0);
static void script_C068(void);
static void script_C074(void);
static void script_C080(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5, int32_t arg6);
static int32_t script_C0C3(void);
static int8_t script_C1D4(int32_t arg0, int32_t arg1, int8_t arg2);
static void script_C26C(int32_t arg0);
static void script_C43B(int32_t arg0);
static int32_t script_C46C(int32_t arg0, int32_t arg1, int32_t arg2, float arg3, float arg4, int32_t arg5, int32_t arg6);
static void script_CB35(int32_t arg0, int32_t arg1);
static void script_CB67(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_CCD7(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_CE94(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_D048(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_D25E(int32_t arg0, int32_t arg1);
static void script_D328(int32_t arg0, int32_t arg1);
static void script_D389(int32_t arg0);
static void script_D52E(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_D5C6(int32_t arg0, int32_t arg1);
static void script_D6A8(int32_t arg0);
static void script_D6FF(int32_t arg0, float arg1, float arg2, float arg3);
static void script_D727(int32_t arg0, float arg1, float arg2, float arg3);
static void script_D77B(int32_t arg0, int32_t arg1, int32_t arg2);
static int32_t script_D7CB(int32_t arg0);
static void script_D872(int32_t arg0, int32_t arg1);
static int32_t script_D895(int32_t arg0);
static void script_D8C8(int32_t arg0, int32_t arg1);
static void script_D8EB(int32_t arg0, int32_t arg1);
static void script_D94A(int32_t arg0);
static void script_D95F(void);
static void script_D96E(void);
static void script_D97D(void);
static void script_D980(void);
static void script_D983(void);
static void script_D986(void);
static void script_D989(void);
static int32_t script_D996(void);
static void script_D9A2(void);
static int32_t script_D9B1(void);
static void script_D9C0(void);
static void script_D9C9(void);
static void script_D9D8(int32_t arg0);
static void script_DA65(int32_t arg0);
static void script_DAB5(void);
static void script_DAE3(int32_t arg0);
static void script_DB11(void);
static void script_DB45(int32_t arg0);
static int32_t script_DB79(int32_t arg0);
static void script_DB97(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_DBD1(int32_t arg0, int32_t arg1);
static int32_t script_DC05(void);
static void script_DCFD(int32_t arg0, int32_t arg1);
static int32_t script_DD2D(void);
static void script_DDBB(void);
static int32_t script_E386(void);
static void script_E59C(void);
static void script_E5C3(void);
static void script_E615(void);
static int32_t script_E668(void);
static void script_EC28(void);
static int32_t script_EC8D(void);
static void script_F323(void);
static int32_t script_F3E7(void);
static void script_F70F(void);
static int32_t script_F74C(void);
static void script_FA8C(void);
static int32_t script_FC04(void);
static int32_t script_FC76(void);
static void script_FCFB(void);
static void script_FD2F(void);
static void script_FD4A(void);
static void script_FD8D(void);
static void script_FDC1(void);
static void script_FDDC(int32_t arg0);
static void script_FE78(int32_t arg0);
static void script_FE98(int32_t arg0);
static void script_FEDE(void);
static int32_t script_10408(void);
static void script_105AA(void);
static void script_105EA(void);
static void script_10605(int32_t arg0);
static void script_106BE(int32_t arg0);
static void script_106E7(int32_t arg0);
static void script_1070A(int32_t arg0);
static void script_10750(void);
static void script_107A8(void);
static void script_10911(void);
static void script_10949(void);
static int32_t script_10977(int32_t arg0, int32_t arg1);
static int32_t script_10A07(void);
static void script_10B3C(void);
static void script_10C37(void);
static void script_10C3A(void);
static int32_t script_10F3A(void);
static int32_t script_10F63(void);
static int32_t script_11288(void);
static int32_t script_112EA(void);
static void script_11C39(void);
static void script_11C80(void);
static void script_11CEE(void);
static void script_11D65(void);
static int32_t script_11E03(int32_t arg0);
static void script_11E6C(void);
static void script_121DF(void);
static void script_12764(void);
static void script_12B83(void);
static void script_12D94(void);
static int32_t script_12F8B(void);
static void script_12FFD(void);
static void script_13271(void);
static int32_t script_13366(void);
static void script_133C4(void);
static void script_13421(void);
static int32_t script_1347E(void);
static void script_13568(void);
static int32_t script_13C3B(void);
static void script_13C96(int32_t arg0);
static void script_13EF6(void);
static int32_t script_13F31(void);
static void script_13F99(int32_t arg0);
static void script_141E5(int32_t arg0);
static void script_142DF(int32_t arg0);
static void script_143EC(int32_t arg0);
static void script_144E6(int32_t arg0);
static void script_145F3(void);
static int32_t script_14A50(void);
static void script_14A93(void);
static int32_t script_14D3B(void);
static int32_t script_14DAE(void);
static void script_151D6(void);
static void script_15275(void);
static void script_152B4(void);
static int32_t script_152DD(void);
static void script_15351(void);
static void script_15448(void);
static void script_155FF(void);
static void script_157DE(void);
static void script_15817(void);
static void script_158B4(void);
static void script_1592A(void);
static void script_159DE(void);
static int32_t script_15AA8(void);
static void script_15B77(void);
static void script_15C53(void);
static int32_t script_15C9A(void);
static void script_15ED8(void);
static void script_15F44(void);
static void script_15F85(void);
static void script_15FFC(void);
static int32_t script_1605F(void);
static void script_160B4(void);
static int32_t script_16190(void);
static void script_16213(void);
static void script_162E6(void);
static int32_t script_163B9(void);
static void script_16419(void);
static void script_1654D(void);
static int32_t script_1665E(void);
static int32_t script_1674C(void);
static void script_1684E(void);
static void script_169B9(void);
static void script_16A16(void);
static void script_16A8C(void);
static void script_16B19(void);
static int32_t script_16C74(void);
static void script_16C85(void);
static int32_t script_171EE(void);
static void script_17244(void);
static int32_t script_1725D(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_172AD(void);
static void script_173A6(void);
static void script_173FA(int32_t arg0);
static int32_t script_176A7(int32_t arg0, int32_t arg1);
static int32_t script_176B9(int32_t arg0, int32_t arg1);
static int32_t script_176E5(void);
static int32_t script_17871(void);
static int32_t script_17C10(void);
static int32_t script_17ED3(void);
static int32_t script_17F1C(void);
static void script_18090(void);
static int32_t script_18113(void);
static void script_18186(void);
static int32_t script_182D2(int32_t arg0);
static void script_1833B(void);
static int32_t script_183D9(void);
static int32_t script_18C9C(void);
static void script_18D08(void);
static void script_18D16(void);
static int32_t script_18E20(void);
static void script_19347(void);
static void script_19437(int32_t arg0);
static int32_t script_1959C(int32_t arg0, const void *arg1, float arg2);
static int32_t script_19616(void);
static void script_19648(int32_t arg0);
static void script_197E0(int32_t arg0);
static int32_t script_19872(void);
static void script_199AE(void);
static int32_t script_199F0(void);
static void script_19ACA(void);
static void script_19B01(int32_t arg0);
static void script_19B88(int32_t arg0);
static int32_t script_19BA8(void);
static void script_1A248(void);
static void script_1A8AA(void);
static void script_1A92D(void);
static void script_1B16E(void);
static int32_t script_1B8C1(int32_t arg0);
static int32_t script_1B91C(void);
static int32_t script_1BA75(void);
static void script_1BB1B(void);
static void script_1BB74(void);
static int32_t script_1BBAE(int32_t arg0);
static void script_1BBD1(void);
static int32_t script_1BBF1(void);
static void script_1BC3C(void);
static void script_1BC83(void);
static void script_1BCD6(void);
static void script_1BD32(void);
static void script_1BD79(void);
static void script_1BDD5(void);
static void script_1BE5D(void);
static void script_1BED5(void);
static void script_1BF12(void);
static int32_t script_1BF4F(int32_t arg0);
static void script_1BFC0(int32_t arg0);
static int32_t script_1C25A(int32_t arg0);
static void script_1C2A2(void);
static void script_1C2AA(void);
static void script_1C2BE(int8_t arg0);
static int32_t script_1C521(void);
static int8_t script_1C65B(int8_t arg0);
static void script_1C730(void);
static void script_1C7B3(int32_t arg0);
static void script_1CB8D(void);
static void script_1CBD8(void);
static void script_1CBF4(int32_t arg0, int32_t arg1);
static void script_1CC60(void);
static void script_1CD04(void);
static void script_1CDE7(void);
static void script_1D23B(void);
static void script_1D786(void);
static void script_1DB90(void);
static void script_1E0DB(void);
static void script_1E213(int32_t arg0);
static void script_1E21F(int32_t arg0);
static void script_1E22B(int32_t arg0);
static void script_1E337(int32_t arg0);
static void script_1E34E(int32_t arg0);
static void script_1E38F(int32_t arg0);
static void script_1E3D3(int32_t arg0);
static void script_1E451(void);
static void script_1E4D0(void);
static void script_1E5FD(void);
void script_1E600(void);
static int32_t script_1E9E2(int32_t arg0);
static int32_t script_1F198(void);
static void script_1F3F9(void);
static void script_1F459(void);
static void script_1F47A(void);
static void script_1F493(void);
static void script_1F4A9(int32_t arg0);
static void script_1F51F(void);
static void script_1F522(void);
static int32_t script_1F525(void);
static void script_1F6A8(void);
static void script_1FCAF(int32_t arg0);
static void script_1FD5A(int32_t arg0);
static void script_1FD78(float arg0);
static void script_1FD97(int32_t arg0);
static void script_1FDAC(int32_t arg0, int32_t arg1);
static int32_t script_1FE62(int32_t arg0);
static void script_1FE7A(int32_t arg0);
static void script_1FE90(int32_t arg0);
static void script_1FE9E(void);
static void script_1FF68(void);
static void script_1FF6B(void);
static void script_200B9(int32_t arg0);
static int32_t script_200CD(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_200EF(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_20135(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_20149(int32_t arg0);
static void script_20163(void);
static void script_2017E(int32_t arg0);
static void script_201AB(int32_t arg0, int32_t arg1);
static void script_201D2(void);
static void script_201DC(void);
static void script_201F7(void);
static void script_20201(int32_t arg0);
static int32_t script_20265(float arg0, float arg1, float arg2, float arg3);
static int32_t script_2028C(int32_t arg0, int32_t arg1);
static int32_t script_20304(int32_t arg0, int32_t arg1, int32_t arg2, const Vector3 *arg3);
static void script_203B0(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_20576(int32_t arg0);
static void script_20598(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_20673(void);
static void script_20697(int32_t arg0);
static void script_20757(void);
static void script_2075D(void);
static void script_20768(int32_t arg0);
static void script_20843(int32_t arg0, int32_t arg1);
static void script_208A8(int32_t arg0, int32_t arg1);
static void script_2090D(void);
static void script_20949(void);
static void script_20964(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_209D8(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_209FD(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_20A46(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_20AAC(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_20B74(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_20BCA(void);
static void script_20C1F(int32_t arg0);
static void script_20C79(const char *arg0, const char *arg1);

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
static DSWKDoorEntry *task_5C;
static int32_t *task_60;
static DoorResourceDescriptor *task_64;
static MapObjectRecord *task_68;
static DoorSlotState task_6C;
static int32_t task_78;
static int32_t task_7C[10];
static int32_t task_A4[10];
static int32_t task_CC[10];
static int32_t task_F4[10];
static int32_t task_11C[10];
static int32_t task_144[10];
static int32_t task_16C[10];
static int32_t task_194[10];
static int32_t task_1BC;
static int32_t task_1C0;
static Vector3 task_1C4;
static int32_t task_1D0;
static int32_t task_1D4;
static int32_t task_1D8;
static int8_t task_1DC;
static uint8_t task_1DD;
static uint16_t task_1DE;
static int8_t task_1E0;
static uint8_t task_1E1;
static uint16_t task_1E2;
static int32_t task_1E4;
static int32_t task_1E8;
static int8_t task_1EC;
static uint8_t task_1ED;
static uint16_t task_1EE;
static Vector3 task_1F0;
static int32_t task_1FC;
static int32_t task_200;
static int32_t task_204;
static int8_t task_208;
static uint8_t task_209;
static uint16_t task_20A;
static Vector3 task_20C;
static Vector3i task_218;
static int32_t task_224;
static int8_t task_228;
static int8_t task_229;
static int8_t task_22A;
static int8_t task_22B;
static int8_t task_22C;
static int8_t task_22D;
static int8_t task_22E;
static int8_t task_22F;
static int8_t task_230;
static int8_t task_231;
static int8_t task_232;
static int8_t task_233;
static int32_t task_234;
static int32_t task_238;
static int8_t task_23C;
static uint8_t task_23D;
static uint16_t task_23E;
static int8_t task_240;
static uint8_t task_241;
static uint16_t task_242;
static int32_t task_244;
static Vector3 task_248;
static Vector3i task_254;
static int32_t task_260;
static int32_t task_264;
static int32_t task_268;
static int32_t task_26C;
static int8_t task_270;
static int8_t task_271;
static int8_t task_272;
static int8_t task_273;
static int8_t task_274;
static int8_t task_275;
static int8_t task_276;
static int8_t task_277;
static int8_t task_278;
static int8_t task_279;
static int8_t task_27A;
static int8_t task_27B;
static int8_t task_27C;
static int8_t task_27D;
static int8_t task_27E;
static int8_t task_27F;
static int8_t task_280;
static int8_t task_281;
static int8_t task_282;
static int8_t task_283;
static int8_t task_284;
static uint8_t task_285;
static uint16_t task_286;
static int8_t task_288;
static uint8_t task_289;
static uint16_t task_28A;
static Vector3 task_28C;
static Vector3 task_298;
static int8_t task_2A4;
static uint8_t task_2A5;
static uint16_t task_2A6;
static int32_t task_2A8;
static int32_t task_2AC;
static int32_t task_2B0;
static int32_t task_2B4;
static int32_t task_2B8;
static int32_t task_2BC;
static int32_t task_2C0;
static int32_t task_2C4;
static int32_t task_2C8;
static int32_t task_2CC;
static int32_t task_2D0;
static int32_t task_2D4;
static int32_t task_2D8;
static int32_t task_2DC;
static int32_t task_2E0;
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
static int32_t task_310;
static int32_t task_314;
static int8_t task_318;
static uint8_t task_319;
static int16_t task_31A;
static int32_t task_31C;
static int32_t task_320;
static int32_t task_324;
static int32_t task_328;
static int32_t task_32C;
static int32_t task_330;
static int32_t task_334;
static int32_t task_338;
static int32_t task_33C;
static int32_t task_340;
static int32_t task_344;
static int32_t task_348;
static int32_t task_34C;
static int32_t task_350;
static int32_t task_354;
static int32_t task_358;
static int32_t task_35C;
static int32_t task_360;
static int32_t task_364;
static int32_t task_368;
static int32_t task_36C;
static int32_t task_370;
static int32_t task_374;
static int32_t task_378;
static int32_t task_37C;
static int32_t task_380;
static int32_t task_384;
static int32_t task_388;
static int32_t task_38C;
static int8_t task_390;
static int8_t task_391;
static int8_t task_392;
static int8_t task_393;
static int8_t task_394;
static uint8_t task_395;
static uint16_t task_396;
static int32_t task_398;
static int32_t task_39C;
static int8_t task_3A0[100][12];
static int8_t task_850[12];
static int8_t task_85C[10];
static int8_t task_866;
static int8_t task_867;
static int8_t task_868;
static uint8_t task_869;
static uint16_t task_86A;
static int32_t task_86C;
static int32_t task_870;
static int8_t task_874;
static uint8_t task_875;
static uint16_t task_876;
static int32_t task_878[10];
static int32_t task_8A0;
static int32_t task_8A4;
static const Vector3 data_20CCC = {.x = 1.0f, .y = 1.0f, .z = 1.0f};
static const int32_t data_20CD8[4] = {16777215, 16777215, 16777215, 16777215};
static const float data_20CE8[3] = {20.0f, 800.0f, 1.0f};
static const int32_t data_20CF4 = -1;
static const char data_20CF8[10] __attribute__((scn_string_literal)) = "Kaidan : ";
static const char data_20D02[32] __attribute__((scn_string_literal)) = "========== initializing kaidan\012";
static const char data_20D22[48] __attribute__((scn_string_literal)) = "please ENDOFKDATA use can't start shuki kaidan\012";
static const char data_20D52[] __attribute__((scn_string_literal)) = "Kaidan Function Error 1\n";
static const char data_20D6B[] __attribute__((scn_string_literal)) = "Kaidan Function Error 2\n";
static const char data_20D84[] __attribute__((scn_string_literal)) = "Kaidan Data Error\n";
static const char data_20D97[10] __attribute__((scn_string_literal)) = "Kaidan : ";
static const char data_20DA1[28] __attribute__((scn_string_literal)) = "wait ... shuki shuki shuki\012";
static const char data_20DBD[10] __attribute__((scn_string_literal)) = "Kaidan : ";
static const char data_20DC7[24] __attribute__((scn_string_literal)) = "KAIDAN : Warning coli!\012";
static const char data_20DDF[24] __attribute__((scn_string_literal)) = "KAIDAN : Warning coli!\012";
static const char data_20DF7[24] __attribute__((scn_string_literal)) = "KAIDAN : Warning coli!\012";
static const char data_20E0F[24] __attribute__((scn_string_literal)) = "KAIDAN : Warning coli!\012";
static const char data_20E27[24] __attribute__((scn_string_literal)) = "KAIDAN : Warning coli!\012";
static const char data_20E3F[24] __attribute__((scn_string_literal)) = "KAIDAN : Warning setY!\012";
static const char data_20E57[24] __attribute__((scn_string_literal)) = "KAIDAN : Warning setY!\012";
static const char data_20E6F[13] __attribute__((scn_string_literal)) = "Kaidan : ";
static DoorMotionSelectorBlock data_20E7C[38] = {
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
static ScnDoorMotionDescriptor data_2146C[72] = {
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
static uint32_t data_21E8C = 0xFFFFFFFF;
typedef struct Row_21E90 { float w0; float w1; float w2; float w3; int32_t w4; } Row_21E90;
static const Row_21E90 data_21E90[7] = {
  {0.0f, 0.0f, 0.0f, 0.0f, -1},
  {0.0f, 0.0f, 0.0f, 0.0f, -1},
  {0.0f, 0.0f, 0.0f, 0.0f, 1534},
  {26.0f, 55.0f, 45.0f, 75.0f, 1555},
  {61.0f, 146.0f, 45.0f, 50.0f, 1554},
  {59.0f, 124.0f, 45.0f, 70.0f, 1543},
  {33.0f, 49.0f, 45.0f, 50.0f, 1543}
};
static const float data_21F1C[4] = {33.0f, 49.0f, 50.0f, 50.0f};
static void (*data_21F2C)(int32_t, int32_t, int32_t) = script_3FB4;
static void (*const data_21F30)(int32_t, int32_t, int32_t, int32_t) = script_3FC4;
static void (*const data_21F34)(int32_t, int32_t, int32_t, int32_t) = script_3FC4;
static void (*const data_21F38)(int32_t, int32_t, int32_t, int32_t) = script_3FC4;
static void (*const data_21F3C)(int32_t, int32_t, int32_t, int32_t) = script_3FC4;
static void (*const data_21F40)(int32_t, int32_t, int32_t, int32_t) = script_3FC4;
static void (*const data_21F44)(int32_t, int32_t, int32_t, int32_t) = script_3FC4;
static void (*const data_21F48)(int32_t, int32_t, int32_t, int32_t) = script_3FC4;
static void (*const data_21F4C)(int32_t, int32_t, int32_t, int32_t) = script_3FC4;
static void (*const data_21F50)(int32_t, int32_t, int32_t, int32_t) = script_426A;
static void (*const data_21F54)(int32_t, int32_t, int32_t, int32_t) = script_43CB;
static void (*const data_21F58)(int32_t, int32_t, int32_t, int32_t) = script_4898;
static void (*const data_21F5C)(int32_t, int32_t, int32_t, int32_t) = script_48FF;
static void (*const data_21F60)(int32_t, int32_t, int32_t, int32_t) = script_40C8;
static void (*const data_21F64)(int32_t, int32_t, int32_t, int32_t) = script_40C8;
static void (*const data_21F68)(int32_t, int32_t, int32_t, int32_t) = script_452C;
static void (*const data_21F6C)(int32_t, int32_t, int32_t, int32_t) = script_47CA;
static void (*const data_21F70)(int32_t, int32_t, int32_t, int32_t) = script_4966;
static void (*const data_21F74)(int32_t, int32_t, int32_t, int32_t) = script_426A;
static void (*const data_21F78)(int32_t, int32_t, int32_t, int32_t) = script_43CB;
static void (*const data_21F7C)(int32_t, int32_t, int32_t, int32_t) = script_452C;
static void (*const data_21F80)(int32_t, int32_t, int32_t, int32_t) = script_426A;
static void (*const data_21F84)(int32_t, int32_t, int32_t, int32_t) = script_43CB;
static void (*const data_21F88)(int32_t, int32_t, int32_t, int32_t) = script_452C;
static void (*const data_21F8C)(int32_t, int32_t, int32_t, int32_t) = script_49AD;
static void (*const data_21F90)(int32_t, int32_t, int32_t, int32_t) = script_49AD;
static void (*const data_21F94)(int32_t, int32_t, int32_t, int32_t) = script_49AD;
static void (*const data_21F98)(int32_t, int32_t, int32_t, int32_t) = script_49AD;
static void (*const data_21F9C)(int32_t, int32_t, int32_t, int32_t) = script_4AE3;
static void (*const data_21FA0)(int32_t, int32_t, int32_t, int32_t) = script_53A4;
static void (*const data_21FA4)(int32_t, int32_t, int32_t, int32_t) = script_3FC4;
static void (*const data_21FA8)(int32_t, int32_t, int32_t, int32_t) = script_3FC4;
static void (*const data_21FAC)(int32_t, int32_t, int32_t, int32_t) = script_3FC4;
static void (*const data_21FB0)(int32_t, int32_t, int32_t, int32_t) = script_3FC4;
static void (*const data_21FB4)(int32_t, int32_t, int32_t, int32_t) = script_3FC4;
static void (*const data_21FB8)(int32_t, int32_t, int32_t, int32_t) = script_3FC4;
static void (*const data_21FBC)(int32_t, int32_t, int32_t, int32_t) = script_3FC4;
static void (*const data_21FC0)(int32_t, int32_t, int32_t, int32_t) = script_3FC4;
static const char data_21FC4[46] __attribute__((scn_string_literal)) = "Door Index = %d : call_type = %d : type = %d\012";
static const char data_21FF2[43] __attribute__((scn_string_literal)) = "Door Open Motion = DEB_AKESIME_FUSUMA_L_F\012";
static const char data_2201D[43] __attribute__((scn_string_literal)) = "Door Open Motion = DEB_AKESIME_FUSUMA_R_F\012";
static const char data_22048[42] __attribute__((scn_string_literal)) = "Door Open Motion = DEB_HIKISIME_DOOR_L_F\012";
static const char data_22072[42] __attribute__((scn_string_literal)) = "Door Open Motion = DEB_HIKISIME_DOOR_R_F\012";
static const char data_2209C[41] __attribute__((scn_string_literal)) = "Door Open Motion = DEB_OSISIME_DOOR_L_F\012";
static const char data_220C5[41] __attribute__((scn_string_literal)) = "Door Open Motion = DEB_OSISIME_DOOR_R_F\012";
static const char data_220EE[43] __attribute__((scn_string_literal)) = "Door Open Motion = GAK_AKESIME_FUSUMA_L_F\012";
static const char data_22119[43] __attribute__((scn_string_literal)) = "Door Open Motion = GAK_AKESIME_FUSUMA_R_F\012";
static const char data_22144[42] __attribute__((scn_string_literal)) = "Door Open Motion = GAK_HIKISIME_DOOR_L_F\012";
static const char data_2216E[42] __attribute__((scn_string_literal)) = "Door Open Motion = GAK_HIKISIME_DOOR_R_F\012";
static const char data_22198[41] __attribute__((scn_string_literal)) = "Door Open Motion = GAK_OSISIME_DOOR_L_F\012";
static const char data_221C1[41] __attribute__((scn_string_literal)) = "Door Open Motion = GAK_OSISIME_DOOR_R_F\012";
static const char data_221EA[43] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_AKESIME_FUSUMA_L_F\012";
static const char data_22215[43] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_AKESIME_FUSUMA_R_F\012";
static const char data_22240[42] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_HIKISIME_DOOR_L_F\012";
static const char data_2226A[42] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_HIKISIME_DOOR_R_F\012";
static const char data_22294[41] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_OSISIME_DOOR_L_F\012";
static const char data_222BD[41] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_OSISIME_DOOR_R_F\012";
static const char data_222E6[43] __attribute__((scn_string_literal)) = "Door Open Motion = KOD_AKESIME_FUSUMA_L_F\012";
static const char data_22311[43] __attribute__((scn_string_literal)) = "Door Open Motion = KOD_AKESIME_FUSUMA_R_F\012";
static const char data_2233C[42] __attribute__((scn_string_literal)) = "Door Open Motion = KOD_HIKISIME_DOOR_L_F\012";
static const char data_22366[42] __attribute__((scn_string_literal)) = "Door Open Motion = KOD_HIKISIME_DOOR_R_F\012";
static const char data_22390[41] __attribute__((scn_string_literal)) = "Door Open Motion = KOD_OSISIME_DOOR_L_F\012";
static const char data_223B9[41] __attribute__((scn_string_literal)) = "Door Open Motion = KOD_OSISIME_DOOR_R_F\012";
static const char data_223E2[43] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_AKESIME_FUSUMA_L_F\012";
static const char data_2240D[43] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_AKESIME_FUSUMA_R_F\012";
static const char data_22438[42] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_HIKISIME_DOOR_L_F\012";
static const char data_22462[42] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_HIKISIME_DOOR_R_F\012";
static const char data_2248C[41] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_OSISIME_DOOR_L_F\012";
static const char data_224B5[41] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_OSISIME_DOOR_R_F\012";
static const char data_224DE[43] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_AKESIME_FUSUMA_L_F\012";
static const char data_22509[43] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_AKESIME_FUSUMA_R_F\012";
static const char data_22534[42] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_HIKISIME_DOOR_L_F\012";
static const char data_2255E[42] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_HIKISIME_DOOR_R_F\012";
static const char data_22588[41] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_OSISIME_DOOR_L_F\012";
static const char data_225B1[41] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_OSISIME_DOOR_R_F\012";
static const char data_225DA[43] __attribute__((scn_string_literal)) = "Door Open Motion = SIN_AKESIME_FUSUMA_L_F\012";
static const char data_22605[43] __attribute__((scn_string_literal)) = "Door Open Motion = SIN_AKESIME_FUSUMA_R_F\012";
static const char data_22630[42] __attribute__((scn_string_literal)) = "Door Open Motion = SIN_HIKISIME_DOOR_L_F\012";
static const char data_2265A[42] __attribute__((scn_string_literal)) = "Door Open Motion = SIN_HIKISIME_DOOR_R_F\012";
static const char data_22684[41] __attribute__((scn_string_literal)) = "Door Open Motion = SIN_OSISIME_DOOR_L_F\012";
static const char data_226AD[41] __attribute__((scn_string_literal)) = "Door Open Motion = SIN_OSISIME_DOOR_R_F\012";
static const char data_226D6[43] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_AKESIME_FUSUMA_L_F\012";
static const char data_22701[43] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_AKESIME_FUSUMA_R_F\012";
static const char data_2272C[42] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_HIKISIME_DOOR_L_F\012";
static const char data_22756[42] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_HIKISIME_DOOR_R_F\012";
static const char data_22780[41] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_OSISIME_DOOR_L_F\012";
static const char data_227A9[41] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_OSISIME_DOOR_R_F\012";
static const char data_227D2[43] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_AKESIME_FUSUMA_L_F\012";
static const char data_227FD[43] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_AKESIME_FUSUMA_R_F\012";
static const char data_22828[42] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_HIKISIME_DOOR_L_F\012";
static const char data_22852[42] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_HIKISIME_DOOR_R_F\012";
static const char data_2287C[41] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_OSISIME_DOOR_L_F\012";
static const char data_228A5[41] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_OSISIME_DOOR_R_F\012";
static const char data_228CE[44] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_OSISIME_DOOR_AP_L_F\012";
static const char data_228FA[44] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_OSISIME_DOOR_AP_L_F\012";
static const char data_22926[44] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_OSISIME_DOOR_AP_L_F\012";
static const char data_22952[44] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_OSISIME_DOOR_AP_L_F\012";
static const char data_2297E[44] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_OSISIME_DOOR_AP_L_F\012";
static const char data_229AA[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_229C6[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_229E2[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_229FE[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_22A1A[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_22A36[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_22A52[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_22A6E[13] __attribute__((scn_string_literal)) = "open-move!!\012";
static const char data_22A7B[11] __attribute__((scn_string_literal)) = "M_MDOR.BIN";
static const char data_22A86[] __attribute__((scn_string_literal)) = "This Charactor Work is NULL!!\n";
static const char data_22AA5[22] __attribute__((scn_string_literal)) = "手動ドア開け番号指定\012";
static const char data_22ABB[34] __attribute__((scn_string_literal)) = "無限ループになりそうなので抜ける\012";
static const char data_22ADD[] __attribute__((scn_string_literal)) = "Don't make BOX Buffer!!\n";
static const char data_22AF6[] __attribute__((scn_string_literal)) = "Don't make BOX Buffer!!\n";
static const char data_22B0F[15] __attribute__((scn_string_literal)) = "Door System : ";
static const char data_22B1E[15] __attribute__((scn_string_literal)) = "Door System : ";
static const char data_22B2D[18] __attribute__((scn_string_literal)) = "Door Box is ON!!\012";
static const char data_22B3F[19] __attribute__((scn_string_literal)) = "Door Box is OFF!!\012";
static const char data_22B52[18] __attribute__((scn_string_literal)) = "Door Box is ON!!\012";
static const char data_22B64[20] __attribute__((scn_string_literal)) = "Door Box is OFF!!\012";
static const DSWKDoorEntry data_22B78[2] = {
  {.slot = 0, .flags = 0, .ctrl_mask_pair = 0x640064U, .coarse_metric = 0.0f, .selection_metric = 0.0f, .view_ang_delta = 0, .group = 0, .global1 = 0xFFFFFFFFU, .global2 = 0xFFFFFFFFU, .global3 = 0xFFFFFFFFU, .sndCommandId = 0xFFFFFFFFU, .global5 = 0xFFFFFFFFU, .global6 = 0xFFFFFFFFU},
  {.slot = 2, .flags = 0, .ctrl_mask_pair = 0x640064U, .coarse_metric = 0.0f, .selection_metric = 0.0f, .view_ang_delta = 0, .group = 0, .global1 = 0xFFFFFFFFU, .global2 = 0xFFFFFFFFU, .global3 = 0xFFFFFFFFU, .sndCommandId = 0xFFFFFFFFU, .global5 = 0xFFFFFFFFU, .global6 = 0xFFFFFFFFU}
};
static const int32_t data_22BE0[13] = {-1, 0, 6553700, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1};
static const uint8_t data_22C14[4] = {0};
static const int32_t data_22C18[3] = {-1, 1, -1};
static char data_22C24[] = "DR29_000";
static char data_22C2D[] = "GNKS2STG";
static const uint8_t data_22C36[2] = {0};
static char *const data_22C38 = data_22C24;
static const int32_t data_22C3C[3] = {29, 787115, 852651};
static char *const data_22C48 = data_22C2D;
static const int32_t data_22C4C[3] = {15, 524971, 590507};
static const MapObjectRecord data_22C58[2] = {
  {.kind = 2, .resourceIndex = 0, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-1.0f, 0.21f, 10.9f}, .yawFixed16 = 0},
  {.kind = 2, .resourceIndex = 1, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-3.24f, 0.1f, 0.02f}, .yawFixed16 = 0}
};
static const uint32_t data_22CA0[4] = {0x413042CU, 0x42F042DU, 0x4300432U, 0x3870431U};
static const char data_22CB0[20] __attribute__((scn_string_literal)) = "TUNED FreeTalk def\012";
static const char data_22CC4[932] = "TUNED FreeTalk def\012\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000V\325\377\377\000\000\000\000\000\000\000\000\302\326\377\377\000\000\000\000\260\005\000\000\000\000\000\000\000\000\000\000\000\000\000\000\232\271\377\377\000\000\000\000\000\000\000\000(\275\377\377\000\000\000\000\000\000\000\000.\270\377\377\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\\\320\377\377\000\000\000\000\000\000\000\000\344\270\377\377\000\000\000\000\000\000\000\000\224\276\377\377\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\310\321\377\377\000\000\000\000\000\000\000\000.\270\377\377\000\000\000\000\000\000\000\000\373\304\377\377\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\216\003\000\000\022\321\377\377\000\000\000\000x\367\377\377\224\276\377\377\000\000\000\000x\367\377\377\344\330\377\377\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\224\376\377\377~\322\377\377\240\364\377\377\224\376\377\377\357\356\377\377f\006\000\000\216\003\000\000\266\000\000\000\000\000\000\000\000\000\000\000m\301\377\377\000\000\000\000\266\000\000\000\261\245\377\377\000\000\000\000\"\002\000\000\014\266\377\377\000\000\000\000\000\000\000\000\274\373\377\377\000\000\000\000\000\000\000\000\000\300\377\377\000\000\000\000\000\000\000\000\253\252\377\377\000\000\000\000\000\000\000\000\302\266\377\377\000\000\000\000\000\000\000\000\336\375\377\377\000\000\000\000\000\000\000\000\027\254\377\377\000\000\000\000\000\000\000\000\315\254\377\377\000\000\000\000\000\000\000\000x\267\377\377\000\000\000\000\000\000\000\000(\375\377\377\000\000\000\000\216\003\000\000\302\266\377\377\000\000\000\000\266\000\000\000\302\266\377\377\000\000\000\000\\\360\377\377\344\270\377\377\000\000\000\000\000\000\000\000(\375\377\377\000\000\000\000\224\376\377\377?\311\377\377\240\364\377\377\224\376\377\377\261\345\377\377f\006\000\000\216\003\000\000\364\011\000\000\000\000\000\000\000\000\000\000x\267\377\377\000\000\000\000\266\000\000\000\274\233\377\377\000\000\000\000\"\002\000\000\027\254\377\377\000\000\000\000\000\000\000\000\310\361\377\377\000\000\000\000\000\000\000\000\014\266\377\377\000\000\000\000\000\000\000\000\267\240\377\377\000\000\000\000\000\000\000\000\315\254\377\377\000\000\000\000\000\000\000\000\352\363\377\377\000\000\000\000\000\000\000\000#\242\377\377\000\000\000\000\000\000\000\000\331\242\377\377\000\000\000\000\000\000\000\000\203\255\377\377\000\000\000\000\000\000\000\0004\363\377\377\000\000\000\000\216\003\000\000\315\254\377\377\000\000\000\000\266\000\000\000\315\254\377\377\000\000\000\000\\\360\377\377\357\256\377\377\000\000\000\000\000\000\000\0004\363\377\377";
static const int32_t data_23068[57] = {
  0, 0, -5461, 0, -2366, -1092, 0, -182, 1092, 0, -546, -6735, 0, 0, -4004, 0, 0, -2912, 0, -182, 1274, 0, 0, -13289, 0, 0, -5279, 0, 0, -8009, 0, 0,
  1456, 0, 182, -8738, 0, 0, -6007, 0, 0, -8738, 0, 182, 1638, 0, -728, -5279, 0, -1274, -6735, 0, -910, -8738, 0, 0, 1638
};
static const int32_t data_2314C[114] = {
  0, 0, -5461, 0, -2366, -1092, 0, -182, 1092, 0, -546, -13289, 0, 0, -5643, 0, 0, -8009, 0, -182, 1274, 0, 0, -11832, 0, 0, -5279, 0, 0, -8009, 0, 0,
  1456, 0, 182, -8738, 0, 0, -6007, 0, 0, -8738, 0, 182, 1638, 0, -728, -5279, 0, -1274, -6735, 0, -910, -8738, 0, 0, 1092, 0, 0, -5461, 0, -2366,
  -1092, 0, -182, 1092, 0, -546, -3640, 0, 0, -2366, 0, 0, -1456, 0, -182, 1274, 0, 0, -3822, 0, 0, -2730, 0, 0, -1456, 0, 0, 1456, 0, 182, -3822, 0,
  0, -2912, 0, 0, -1638, 0, 182, 1638, 0, -728, -3640, 0, -910, -2730, 0, -546, -1638, 0, 0, 1092
};
static const int32_t data_23314[114] = {
  0, 0, -5461, 0, -2366, -1092, 0, -182, 1092, 0, -546, -6735, 0, 0, -4004, 0, 0, -6553, 0, -182, 1274, 0, 0, -13289, 0, 0, -5279, 0, 0, -8009, 0, 0,
  1456, 0, 182, -8738, 0, 0, -6007, 0, 0, -8738, 0, 182, 1638, 0, -728, -5279, 0, -1274, -6735, 0, -910, -8738, 0, 0, 1638, 0, 0, -9102, 0, -2366,
  -2002, 0, -182, 910, 0, -546, -2366, 0, 0, -2002, 0, 0, -2912, 0, -182, 1274, 0, 0, -14199, 0, 0, -11468, 0, 0, -13653, 0, 0, 1456, 0, 182, -11286,
  0, 0, -12014, 0, 0, -12743, 0, 182, 1638, 0, -728, -10194, 0, -1274, -12196, 0, -910, -10922, -728, 546, 1638
};
static const int32_t data_234DC[228] = {
  0, 0, -1820, 0, -2366, -728, 0, -364, -1092, 0, -546, -11832, 0, 910, -6735, 0, 0, -6553, 0, -182, 1274, 0, 0, -13289, 0, 0, -6007, 0, 0, -10012, 0,
  0, 1092, 0, 182, -8738, 0, 0, -6735, 0, 0, -9830, 0, 182, 1638, 0, -728, -6553, 0, -1274, -7281, 0, -910, -10740, 0, 0, 1638, 0, 0, -5461, -2912,
  -2366, -364, -5279, 2548, -910, 0, -546, -8374, 0, 0, -9466, 0, 910, -4915, 0, -182, 546, 0, 0, -13289, 0, 0, -5279, 0, 364, -7645, 0, -182, 1456,
  0, 182, -8738, 0, 0, -7281, 0, 182, -6917, 0, -182, 1638, 0, -728, -5279, 0, -1274, -6735, 0, -182, -8009, 0, 0, 1638, -4733, 0, -8556, 0, -2366,
  -1092, -6189, 2912, 0, 0, -546, -10922, 0, 1820, -16384, 0, 0, -11468, 0, 182, 546, 0, 0, -10376, 0, 0, -15837, 0, 0, -12743, 0, -182, 728, 0, 182,
  -13835, 0, 0, -16201, 0, 364, -10922, 0, -182, 728, 0, -728, -9648, 0, -3458, -12379, 0, -728, -7463, 0, 0, 1092, 0, 0, -5461, 0, -2366, -1638, 0,
  -910, 1092, 0, -546, -3640, 0, 0, -2366, 0, 0, -1456, 0, -182, 1638, 0, 0, -910, 0, 0, -2730, 0, 0, -1274, 0, 0, 1820, 0, 182, -2730, 0, 0, -1820,
  0, 0, -1274, 0, 182, 2184, 0, -728, -3094, 0, -910, -2548, 0, 910, -1456, -546, 910, 2366
};
static const int32_t data_2386C[57] = {
  0, 0, -6005, 0, 0, -909, 0, 0, 0, 0, 0, -6368, 0, 1820, -8552, 0, 0, -4548, 0, 0, 0, 0, 0, -8552, 0, 0, -10372, 0, -545, -4002, 0, 0, 0, 0, 0,
  -7823, 0, 0, -12374, 0, -909, -2182, 0, 0, 0, 0, -1636, -10735, 0, -2001, -10372, 0, -1819, -727, 0, 0, 0
};
static const int32_t data_23950[57] = {
  0, 0, -7827, 0, -910, -4004, 0, -182, 546, 0, -546, -2730, 0, 0, -4551, 0, 0, -4369, 0, -182, 1820, 0, 0, -5825, 0, 0, -5825, 0, 0, -3458, 0, 0,
  1456, 0, 182, -7099, 0, 182, -5825, 0, 0, -4369, 0, 182, 1638, 0, -728, -6553, 0, -910, -7463, 0, -546, -2730, 0, 0, 1092
};
static const Vector4 data_23A34 = {.x = 1.0f, .y = 0.08f, .z = 0.08f, .w = 0.1f};
static const float data_23A44[3] = {0.1f, 360.0f, 0.92f};
static const int32_t data_23A50 = -16777160;
static const char data_23A54[] __attribute__((scn_string_literal)) = "scroll";
static const char data_23A5B[13] __attribute__((scn_string_literal)) = "SCROLL25.SPR";
static const Vector4 data_23A68 = {.x = 0.3f, .y = 0.3f, .z = 0.3f, .w = 0.3f};
static const float data_23A78[4] = {1.0f, 1.0f, 1.0f, 1.0f};
static const Vector4 data_23A88 = {.x = 0.3f, .y = 0.3f, .z = 0.3f, .w = 0.3f};
static const Vector4 data_23A98 = {.x = 1.0f, .y = 1.0f, .z = 1.0f, .w = 1.0f};
static const Vector4 data_23AA8 = {.x = 0.4f, .y = 0.4f, .z = 0.4f, .w = 0.4f};
static const Vector4 data_23AB8 = {.x = 0.3f, .y = 0.3f, .z = 0.3f, .w = 0.3f};
static const Vector4 data_23AC8 = {.x = 0.3f, .y = 0.3f, .z = 0.3f, .w = 0.3f};
static const Vector4 data_23AD8 = {.x = 0.3f, .y = 0.3f, .z = 0.3f, .w = 0.3f};
static const uint16_t data_23AE8[7] = {670, 672, 674, 677, 684, 682, 680};
static const char data_23AF6[24] __attribute__((scn_string_literal)) = "ふくちゃん読み込み失敗\012";
static const char data_23B0E[22] __attribute__((scn_string_literal)) = "いねさん読み込み失敗\012";
static const Vector3 data_23B24 = {.x = -11.2439f, .y = 2.9721f, .z = -22.947001f};
static const Vector3 data_23B30 = {.x = -11.242f, .y = 2.9721f, .z = -21.937f};
typedef struct Row_23B3C { float w0; float w1; float w2; int32_t w3; int32_t w4; int32_t w5; } Row_23B3C;
static const Row_23B3C data_23B3C[2] = {
  {-10.8f, 1.13f, -23.03f, 0, 16384, 0},
  {-10.5499f, 1.397f, -22.75f, 49152, 32768, 49152}
};
static const float data_23B6C[3] = {-10.5299f, 1.269f, -22.6462f};
static const Vector3 data_23B78 = {.x = -4.0f, .y = 0.0f, .z = -27.0f};
static const Vector3 data_23B84 = {.x = -9.4f, .y = 0.0f, .z = -18.2f};
static const uint32_t data_23B90 = 3;
static const float data_23B94[3] = {-6.7f, -22.5f, 1.0f};
static const int32_t data_23BA0 = 196608;
static const uint32_t data_23BA4 = 131072;
typedef struct Row_23BA8 { shen_name w0; uint32_t w1; int32_t w2; float w3; float w4; int32_t w5; int32_t w6; int32_t w7; } Row_23BA8;
static const Row_23BA8 data_23BA8[7] = {
  {SHEN_NAME('KHKS'), 0x80004000U, 0, 0.0f, 0.0f, 0, 0, 131072},
  {SHEN_NAME('GART'), 0x80008000U, 0, -0.3f, 0.4f, 0, 0, 131072},
  {SHEN_NAME('FIR1'), 0x0U, 0, 0.0f, 0.0f, 0, 0, 131072},
  {SHEN_NAME('FIR2'), 0x0U, 0, 0.0f, 0.0f, 0, 0, 131072},
  {SHEN_NAME('FIR3'), 0x0U, 0, 0.0f, 0.0f, 0, 0, 131072},
  {SHEN_NAME('FIR4'), 0x0U, 0, 0.0f, 0.0f, 0, 0, 131072},
  {SHEN_NAME('HAKO'), 0x0U, 0, 0.4f, 0.0f, 0, 0, 65535}
};
static const uint32_t data_23C88 = 131072;
typedef struct Row_23C8C { shen_name w0; uint32_t w1; int32_t w2; float w3; float w4; int32_t w5; int32_t w6; int32_t w7; } Row_23C8C;
static const Row_23C8C data_23C8C[13] = {
  {SHEN_NAME('KHKS'), 0x80004000U, 0, 0.0f, 0.0f, 0, 0, 131072},
  {SHEN_NAME('GART'), 0x80008000U, 0, -0.3f, 0.4f, 0, 0, 131072},
  {SHEN_NAME('FIR1'), 0x0U, 0, 0.0f, 0.0f, 0, 0, 131072},
  {SHEN_NAME('FIR2'), 0x0U, 0, 0.0f, 0.0f, 0, 0, 131072},
  {SHEN_NAME('FIR3'), 0x0U, 0, 0.0f, 0.0f, 0, 0, 131072},
  {SHEN_NAME('FIR4'), 0x0U, 0, 0.0f, 0.0f, 0, 0, 131072},
  {SHEN_NAME('HAKO'), 0x0U, 0, 0.4f, 0.0f, 0, 0, 0},
  {SHEN_NAME('KAK2'), 0x80004000U, 0, -0.7f, 0.0f, 0, 0, 0},
  {SHEN_NAME('KAK1'), 0x80004000U, 0, -0.7f, 0.0f, 0, 0, 0},
  {SHEN_NAME('KAK2'), 0x80004000U, 0, 0.0f, 0.0f, 0, 0, 0},
  {SHEN_NAME('KAK1'), 0x80004000U, 0, 0.0f, 0.0f, 0, 0, 0},
  {SHEN_NAME('KAK2'), 0x80004000U, 0, -1.2f, 0.0f, 0, 0, 0},
  {SHEN_NAME('KAK1'), 0x80004000U, 0, -1.2f, 0.0f, 0, 0, 65535}
};
static const uint32_t data_23E2C = 131072;
static const shen_name data_23E30 = SHEN_NAME('KHKS');
static const uint32_t data_23E34[6] = {0x80004000U, 0, 0, 0, 0, 0};
static const int32_t data_23E4C = 131072;
static const shen_name data_23E50 = SHEN_NAME('GART');
static const uint32_t data_23E54[2] = {0x80008000U, 0};
static const float data_23E5C[4] = {-0.3f, 0.4f, 0.0f, 0.0f};
static const int32_t data_23E6C = 131072;
static const shen_name data_23E70[7] = {SHEN_NAME('FIR1'), 0, 0, 0, 0, 0, 0};
static const int32_t data_23E8C = 131072;
static const shen_name data_23E90[7] = {SHEN_NAME('FIR2'), 0, 0, 0, 0, 0, 0};
static const int32_t data_23EAC = 131072;
static const shen_name data_23EB0[7] = {SHEN_NAME('FIR3'), 0, 0, 0, 0, 0, 0};
static const int32_t data_23ECC = 131072;
static const shen_name data_23ED0[7] = {SHEN_NAME('FIR4'), 0, 0, 0, 0, 0, 0};
static const int32_t data_23EEC = 131072;
static const shen_name data_23EF0[3] = {SHEN_NAME('HAKO'), 0, 0};
static const float data_23EFC[5] = {0.4f, 0.0f, 0.0f, 0.0f, 0.0f};
static const shen_name data_23F10 = SHEN_NAME('KAK2');
static const uint32_t data_23F14[2] = {0x80004000U, 0};
static const float data_23F1C[4] = {-0.7f, 0.0f, 0.0f, 0.0f};
static const int32_t data_23F2C[2] = {-65536, 10};
static const uint32_t data_23F34 = 0x80004000U;
static const float data_23F38[6] = {-11.299f, 2.3499f, -22.951f, 0.0f, 0.0f, 0.0f};
static const shen_name data_23F50 = SHEN_NAME('KAK2');
static const uint32_t data_23F54[7] = {0x80004000U, 0, 0, 0, 0, 0, 0};
static const shen_name data_23F70 = SHEN_NAME('KAK2');
static const uint32_t data_23F74[2] = {0x80004000U, 0};
static const float data_23F7C[4] = {-1.2f, 0.0f, 0.0f, 0.0f};
static const int32_t data_23F8C = 65535;
static const uint32_t data_23F90 = 131072;
static const shen_name data_23F94 = SHEN_NAME('KHKS');
static const uint32_t data_23F98[6] = {0x80004000U, 0, 0, 0, 0, 0};
static const int32_t data_23FB0 = 131072;
static const shen_name data_23FB4 = SHEN_NAME('GART');
static const uint32_t data_23FB8[2] = {0x80008000U, 0};
static const float data_23FC0[4] = {-0.3f, 0.4f, 0.0f, 0.0f};
static const int32_t data_23FD0 = 131072;
static const shen_name data_23FD4[7] = {SHEN_NAME('FIR1'), 0, 0, 0, 0, 0, 0};
static const int32_t data_23FF0 = 131072;
static const shen_name data_23FF4[7] = {SHEN_NAME('FIR2'), 0, 0, 0, 0, 0, 0};
static const int32_t data_24010 = 131072;
static const shen_name data_24014[7] = {SHEN_NAME('FIR3'), 0, 0, 0, 0, 0, 0};
static const int32_t data_24030 = 131072;
static const shen_name data_24034[7] = {SHEN_NAME('FIR4'), 0, 0, 0, 0, 0, 0};
static const int32_t data_24050 = 131072;
static const shen_name data_24054[3] = {SHEN_NAME('HAKO'), 0, 0};
static const float data_24060[4] = {0.4f, 0.0f, 0.0f, 0.0f};
static const int32_t data_24070[2] = {-65536, 11};
static const uint32_t data_24078 = 0x80004000U;
static const float data_2407C[6] = {-11.299f, 2.3499f, -21.94f, 0.0f, 0.0f, 0.0f};
static const shen_name data_24094 = SHEN_NAME('KAK1');
static const uint32_t data_24098[2] = {0x80004000U, 0};
static const float data_240A0[5] = {-0.7f, 0.0f, 0.0f, 0.0f, 0.0f};
static const shen_name data_240B4 = SHEN_NAME('KAK1');
static const uint32_t data_240B8[7] = {0x80004000U, 0, 0, 0, 0, 0, 0};
static const shen_name data_240D4 = SHEN_NAME('KAK1');
static const uint32_t data_240D8[2] = {0x80004000U, 0};
static const float data_240E0[4] = {-1.2f, 0.0f, 0.0f, 0.0f};
static const int32_t data_240F0 = 65535;
static const uint32_t data_240F4 = 131072;
static const shen_name data_240F8 = SHEN_NAME('KHKS');
static const uint32_t data_240FC[6] = {0x80004000U, 0, 0, 0, 0, 0};
static const int32_t data_24114 = 131072;
static const shen_name data_24118 = SHEN_NAME('GART');
static const uint32_t data_2411C[2] = {0x80008000U, 0};
static const float data_24124[4] = {-0.3f, 0.4f, 0.0f, 0.0f};
static const int32_t data_24134 = 131072;
static const shen_name data_24138[7] = {SHEN_NAME('FIR1'), 0, 0, 0, 0, 0, 0};
static const int32_t data_24154 = 131072;
static const shen_name data_24158[7] = {SHEN_NAME('FIR2'), 0, 0, 0, 0, 0, 0};
static const int32_t data_24174 = 131072;
static const shen_name data_24178[7] = {SHEN_NAME('FIR3'), 0, 0, 0, 0, 0, 0};
static const int32_t data_24194 = 131072;
static const shen_name data_24198[7] = {SHEN_NAME('FIR4'), 0, 0, 0, 0, 0, 0};
static const int32_t data_241B4 = 131072;
static const shen_name data_241B8[3] = {SHEN_NAME('HAKO'), 0, 0};
static const float data_241C4[4] = {0.4f, 0.0f, 0.0f, 0.0f};
static const int32_t data_241D4[2] = {-65536, 11};
static const uint32_t data_241DC = 0x80004000U;
static const float data_241E0[5] = {-11.299f, 2.3499f, -21.94f, 0.0f, 0.0f};
static const int32_t data_241F4[2] = {-65536, 10};
static const uint32_t data_241FC = 0x80004000U;
static const float data_24200[5] = {-11.299f, 2.3499f, -22.951f, 0.0f, 0.0f};
static const int32_t data_24214 = 65535;
static const uint32_t data_24218 = 131072;
typedef struct Row_2421C { shen_name w0; uint32_t w1; int32_t w2; float w3; float w4; int32_t w5; int32_t w6; int32_t w7; } Row_2421C;
static const Row_2421C data_2421C[14] = {
  {SHEN_NAME('KHKS'), 0x80004000U, 0, 0.0f, 0.0f, 0, 0, 131072},
  {SHEN_NAME('GART'), 0x80008000U, 0, -0.3f, 0.4f, 0, 0, 131072},
  {SHEN_NAME('FIR1'), 0x0U, 0, 0.0f, 0.0f, 0, 0, 131072},
  {SHEN_NAME('FIR2'), 0x0U, 0, 0.0f, 0.0f, 0, 0, 131072},
  {SHEN_NAME('FIR3'), 0x0U, 0, 0.0f, 0.0f, 0, 0, 131072},
  {SHEN_NAME('FIR4'), 0x0U, 0, 0.0f, 0.0f, 0, 0, 131072},
  {SHEN_NAME('HAKO'), 0x0U, 0, 0.4f, 0.0f, 0, 0, 0},
  {SHEN_NAME('KAK2'), 0x80004000U, 0, -0.7f, 0.0f, 0, 0, 0},
  {SHEN_NAME('KAK1'), 0x80004000U, 0, -0.7f, 0.0f, 0, 0, 0},
  {SHEN_NAME('KAK2'), 0x80004000U, 0, 0.0f, 0.0f, 0, 0, 0},
  {SHEN_NAME('KAK1'), 0x80004000U, 0, 0.0f, 0.0f, 0, 0, 0},
  {SHEN_NAME('KAK2'), 0x80004000U, 0, -1.2f, 0.0f, 0, 0, 0},
  {SHEN_NAME('KAK1'), 0x80004000U, 0, -1.2f, 0.0f, 0, 0, 131072},
  {SHEN_NAME('POK2'), 0x0U, 0, 0.0f, 0.0f, 0, 0, 65535}
};
static const uint32_t data_243DC = 131072;
static const shen_name data_243E0 = SHEN_NAME('KHKS');
static const uint32_t data_243E4[6] = {0x80004000U, 0, 0, 0, 0, 0};
static const int32_t data_243FC = 131072;
static const shen_name data_24400 = SHEN_NAME('GART');
static const uint32_t data_24404[2] = {0x80008000U, 0};
static const float data_2440C[4] = {-0.3f, 0.4f, 0.0f, 0.0f};
static const int32_t data_2441C = 131072;
static const shen_name data_24420[7] = {SHEN_NAME('FIR1'), 0, 0, 0, 0, 0, 0};
static const int32_t data_2443C = 131072;
static const shen_name data_24440[7] = {SHEN_NAME('FIR2'), 0, 0, 0, 0, 0, 0};
static const int32_t data_2445C = 131072;
static const shen_name data_24460[7] = {SHEN_NAME('FIR3'), 0, 0, 0, 0, 0, 0};
static const int32_t data_2447C = 131072;
static const shen_name data_24480[7] = {SHEN_NAME('FIR4'), 0, 0, 0, 0, 0, 0};
static const int32_t data_2449C = 131072;
static const shen_name data_244A0[3] = {SHEN_NAME('HAKO'), 0, 0};
static const float data_244AC[5] = {0.4f, 0.0f, 0.0f, 0.0f, 0.0f};
static const shen_name data_244C0 = SHEN_NAME('KAK2');
static const uint32_t data_244C4[2] = {0x80004000U, 0};
static const float data_244CC[4] = {-0.7f, 0.0f, 0.0f, 0.0f};
static const int32_t data_244DC[2] = {-65536, 10};
static const uint32_t data_244E4 = 0x80004000U;
static const float data_244E8[6] = {-11.299f, 2.3499f, -22.951f, 0.0f, 0.0f, 0.0f};
static const shen_name data_24500 = SHEN_NAME('KAK2');
static const uint32_t data_24504[7] = {0x80004000U, 0, 0, 0, 0, 0, 0};
static const shen_name data_24520 = SHEN_NAME('KAK2');
static const uint32_t data_24524[2] = {0x80004000U, 0};
static const float data_2452C[4] = {-1.2f, 0.0f, 0.0f, 0.0f};
static const int32_t data_2453C = 131072;
static const shen_name data_24540[7] = {SHEN_NAME('POK2'), 0, 0, 0, 0, 0, 0};
static const int32_t data_2455C = 65535;
static const uint32_t data_24560 = 131072;
static const shen_name data_24564 = SHEN_NAME('KHKS');
static const uint32_t data_24568[6] = {0x80004000U, 0, 0, 0, 0, 0};
static const int32_t data_24580 = 131072;
static const shen_name data_24584 = SHEN_NAME('GART');
static const uint32_t data_24588[2] = {0x80008000U, 0};
static const float data_24590[4] = {-0.3f, 0.4f, 0.0f, 0.0f};
static const int32_t data_245A0 = 131072;
static const shen_name data_245A4[7] = {SHEN_NAME('FIR1'), 0, 0, 0, 0, 0, 0};
static const int32_t data_245C0 = 131072;
static const shen_name data_245C4[7] = {SHEN_NAME('FIR2'), 0, 0, 0, 0, 0, 0};
static const int32_t data_245E0 = 131072;
static const shen_name data_245E4[7] = {SHEN_NAME('FIR3'), 0, 0, 0, 0, 0, 0};
static const int32_t data_24600 = 131072;
static const shen_name data_24604[7] = {SHEN_NAME('FIR4'), 0, 0, 0, 0, 0, 0};
static const int32_t data_24620 = 131072;
static const shen_name data_24624[3] = {SHEN_NAME('HAKO'), 0, 0};
static const float data_24630[4] = {0.4f, 0.0f, 0.0f, 0.0f};
static const int32_t data_24640[2] = {-65536, 11};
static const uint32_t data_24648 = 0x80004000U;
static const float data_2464C[6] = {-11.299f, 2.3499f, -21.94f, 0.0f, 0.0f, 0.0f};
static const shen_name data_24664 = SHEN_NAME('KAK1');
static const uint32_t data_24668[2] = {0x80004000U, 0};
static const float data_24670[5] = {-0.7f, 0.0f, 0.0f, 0.0f, 0.0f};
static const shen_name data_24684 = SHEN_NAME('KAK1');
static const uint32_t data_24688[7] = {0x80004000U, 0, 0, 0, 0, 0, 0};
static const shen_name data_246A4 = SHEN_NAME('KAK1');
static const uint32_t data_246A8[2] = {0x80004000U, 0};
static const float data_246B0[4] = {-1.2f, 0.0f, 0.0f, 0.0f};
static const int32_t data_246C0 = 131072;
static const shen_name data_246C4[7] = {SHEN_NAME('POK2'), 0, 0, 0, 0, 0, 0};
static const int32_t data_246E0 = 65535;
static const uint32_t data_246E4 = 131072;
static const shen_name data_246E8 = SHEN_NAME('KHKS');
static const uint32_t data_246EC[6] = {0x80004000U, 0, 0, 0, 0, 0};
static const int32_t data_24704 = 131072;
static const shen_name data_24708 = SHEN_NAME('GART');
static const uint32_t data_2470C[2] = {0x80008000U, 0};
static const float data_24714[4] = {-0.3f, 0.4f, 0.0f, 0.0f};
static const int32_t data_24724 = 131072;
static const shen_name data_24728[7] = {SHEN_NAME('FIR1'), 0, 0, 0, 0, 0, 0};
static const int32_t data_24744 = 131072;
static const shen_name data_24748[7] = {SHEN_NAME('FIR2'), 0, 0, 0, 0, 0, 0};
static const int32_t data_24764 = 131072;
static const shen_name data_24768[7] = {SHEN_NAME('FIR3'), 0, 0, 0, 0, 0, 0};
static const int32_t data_24784 = 131072;
static const shen_name data_24788[7] = {SHEN_NAME('FIR4'), 0, 0, 0, 0, 0, 0};
static const int32_t data_247A4 = 131072;
static const shen_name data_247A8[3] = {SHEN_NAME('HAKO'), 0, 0};
static const float data_247B4[4] = {0.4f, 0.0f, 0.0f, 0.0f};
static const int32_t data_247C4[2] = {-65536, 11};
static const uint32_t data_247CC = 0x80004000U;
static const float data_247D0[5] = {-11.299f, 2.3499f, -21.94f, 0.0f, 0.0f};
static const int32_t data_247E4[2] = {-65536, 10};
static const uint32_t data_247EC = 0x80004000U;
static const float data_247F0[5] = {-11.299f, 2.3499f, -22.951f, 0.0f, 0.0f};
static const int32_t data_24804 = 131072;
static const shen_name data_24808[7] = {SHEN_NAME('POK2'), 0, 0, 0, 0, 0, 0};
static const int32_t data_24824 = 65535;
static const Vector3 data_24828 = {.x = 0.055f, .y = 1.25f, .z = -0.5f};
static const Vector3i data_24834 = {.x = 0, .y = 0, .z = 49152};
static const Vector3 data_24840 = {.x = 0.025f, .y = 1.252f, .z = 0.5f};
static const Vector3i data_2484C = {.x = 16384, .y = 0, .z = 16384};
static const Vector3i data_24858 = {.x = 0, .y = 16384, .z = 0};
static const Vector3 data_24864 = {.x = -2.2635f, .y = 1.1315f, .z = -16.2188f};
static const Vector3 data_24870 = {.x = -11.2711f, .y = 1.1315f, .z = -28.659599f};
static const char data_2487C[10] __attribute__((scn_string_literal)) = "E1011A109";
static const char data_24886[10] __attribute__((scn_string_literal)) = "E1011A110";
static const char data_24890[10] __attribute__((scn_string_literal)) = "E1011A111";
static const char data_2489A[10] __attribute__((scn_string_literal)) = "E1011A112";
static const char data_248A4[10] __attribute__((scn_string_literal)) = "E1011A113";
static const char data_248AE[10] __attribute__((scn_string_literal)) = "E1011A107";
static const char data_248B8[12] __attribute__((scn_string_literal)) = "E1011A108";
static const Vector3 data_248C4 = {.x = -2.3772f, .y = 0.0f, .z = -23.4748f};
static const Vector3 data_248D0 = {.x = -0.7816f, .y = 0.0f, .z = -21.4021f};
static const Vector3 data_248DC = {.x = -10.995349f, .y = 0.0f, .z = -22.697281f};
static const Vector3 data_248E8 = {.x = -10.375824f, .y = 0.0f, .z = -23.385668f};
static const Vector3 data_248F4 = {.x = -10.995349f, .y = 0.0f, .z = -21.0f};
static const Vector3 data_24900 = {.x = -10.375824f, .y = 0.0f, .z = -22.25f};
static const Vector3 data_2490C = {.x = -10.6f, .y = 0.0f, .z = -24.2f};
static const Vector3 data_24918 = {.x = -8.85f, .y = 0.0f, .z = -26.1f};
static const Vector3 data_24924 = {.x = -14.8742f, .y = 0.0f, .z = -26.414801f};
static const Vector3 data_24930 = {.x = -11.35f, .y = 0.0f, .z = -24.0f};
static const float data_2493C[2] = {-14.8742f, 0.0f};
static const uint32_t data_24944 = 0xC1D35183U;
static const float data_24948[2] = {-11.4742f, 0.0f};
static const uint32_t data_24950 = 0xC1C4EB1CU;
static const float data_24954[24] = {
  -1.0f, 0.0f, -1.0f, 1.0f, 0.0f, 1.0f, -3.95f, 0.0f, -27.3f, -9.72f, 0.0f, -17.8f, -0.6f, 0.0f, -0.6f, 0.6f, 0.0f, 0.6f, -5.8f, 1.05f, -16.5f, -7.9f,
  1.0595f, -18.0f
};
static const Vector3 data_249B4 = {.x = -1.0f, .y = 0.0f, .z = -1.0f};
static const Vector3 data_249C0 = {.x = 1.0f, .y = 0.0f, .z = 1.0f};
static const Vector3 data_249CC = {.x = -2.33f, .y = 0.0f, .z = -23.76f};
static const Vector3 data_249D8 = {.x = -4.0f, .y = 0.0f, .z = -21.37f};
typedef struct Row_249E4 { float w0; int32_t w1; float w2; } Row_249E4;
static const Row_249E4 data_249E4[2] = {
  {-0.2f, 0, -0.2f},
  {0.2f, 0, 0.2f}
};
static const char data_249FC[8] __attribute__((scn_string_literal)) = "BROOM";
static const Vector3 data_24A04 = {.x = -10.5462f, .y = 1.1095f, .z = -22.7847f};
static const Vector3i data_24A10 = {.x = 0, .y = 15841, .z = 0};
static const Vector3 data_24A1C = {.x = 0.115f, .y = 0.119f, .z = -0.024f};
static const Vector3i data_24A28 = {.x = 65353, .y = 62984, .z = 546};
static const Vector3 data_24A34 = {.x = 0.093f, .y = 0.163f, .z = -0.013f};
static const Vector3i data_24A40 = {.x = 31676, .y = 2547, .z = 0};
static const Vector3 data_24A4C = {.x = 0.05f, .y = 1.25f, .z = -0.5f};
static const Vector3i data_24A58 = {.x = 0, .y = 32768, .z = 16384};
static const Vector3i data_24A64 = {.x = 0, .y = 0, .z = 16384};
static const float data_24A70[2] = {-10.8f, 1.1095f};
static const uint32_t data_24A78 = 0xC1C1353FU;
static const float data_24A7C[4] = {0.03f, 1.85f, 0.7f, 0.0f};
static const int32_t data_24A8C[2] = {16384, 0};
static const Vector3 data_24A94 = {.x = 0.096f, .y = 0.163f, .z = 0.011f};
static const Vector3i data_24AA0 = {.x = 32768, .y = 65171, .z = 64080};
static const char data_24AAC[10] __attribute__((scn_string_literal)) = "E1011A055";
static const char data_24AB6[10] __attribute__((scn_string_literal)) = "E1011A045";
static const Vector3 data_24AC0 = {.x = -10.6806f, .y = 1.102f, .z = -22.978901f};
static const Vector3i data_24ACC = {.x = 0, .y = 16384, .z = 0};
typedef struct Row_24AD8 { float w0; float w1; float w2; int32_t w3; int32_t w4; int32_t w5; } Row_24AD8;
static const Row_24AD8 data_24AD8[2] = {
  {0.115f, 0.119f, -0.024f, 65353, 62984, 546},
  {0.093f, 0.163f, -0.013f, 31676, 2547, 0}
};
static const Vector3 data_24B08 = {.x = 0.055f, .y = 1.25f, .z = -0.5f};
static const Vector3i data_24B14 = {.x = 0, .y = 0, .z = 49152};
static const int32_t data_24B20[3] = {0, 0, 16384};
static const float data_24B2C[2] = {-10.8f, 1.1095f};
static const uint32_t data_24B34 = 0xC1C1353FU;
static const float data_24B38[4] = {0.03f, 1.85f, 0.7f, 0.0f};
static const int32_t data_24B48[2] = {16384, 0};
static const float data_24B50[3] = {0.096f, 0.163f, 0.011f};
static const int32_t data_24B5C[3] = {32768, 65171, 64080};
static const char data_24B68[10] __attribute__((scn_string_literal)) = "E1011A045";
static const char data_24B72[10] __attribute__((scn_string_literal)) = "JINGURU01";
static const char data_24B7C[10] __attribute__((scn_string_literal)) = "JINGURU01";
static const char data_24B86[10] __attribute__((scn_string_literal)) = "JINGURU01";
static const uint32_t data_24B90 = 1;
static const float data_24B94[3] = {2.17f, -22.45f, 1.0f};
static const int32_t data_24BA0 = 2;
static const float data_24BA4[3] = {7.36f, -19.18f, 1.0f};
static const int32_t data_24BB0 = 3;
static const float data_24BB4[3] = {7.36f, -10.96f, 1.0f};
static const int32_t data_24BC0 = 196608;
static const float data_24BC4[3] = {0.096f, 0.163f, 0.011f};
static const int32_t data_24BD0[3] = {32768, 32768, 64080};
static const float data_24BDC[3] = {0.155f, 0.054f, -0.032f};
static const int32_t data_24BE8[3] = {15834, 15469, 35671};
static const Vector3 data_24BF4 = {.x = -10.7095f, .y = 1.1095f, .z = -22.887501f};
static const Vector3i data_24C00 = {.x = 0, .y = 16384, .z = 0};
static const Vector3 data_24C0C = {.x = -9.8f, .y = 1.0565f, .z = -24.112f};
static const Vector3i data_24C18 = {.x = 29191, .y = 50374, .z = 3289};
typedef struct Row_24C24 { float w0; float w1; float w2; float w3; int32_t w4; } Row_24C24;
static const Row_24C24 data_24C24[2] = {
  {-11.2439f, 2.9721f, -23.097f, 0.0f, 16384},
  {0.0f, -0.276f, 0.162f, -0.51f, 8372}
};
static const int32_t data_24C4C[2] = {0, 44059};
static const char data_24C54[10] __attribute__((scn_string_literal)) = "E1011A078";
static const char data_24C5E[10] __attribute__((scn_string_literal)) = "E1011A079";
static const Vector3 data_24C68 = {.x = -10.7095f, .y = 1.12075f, .z = -21.877501f};
static const Vector3i data_24C74 = {.x = 0, .y = 16384, .z = 0};
static const Vector3 data_24C80 = {.x = -9.8f, .y = 1.0565f, .z = -22.6f};
static const Vector3i data_24C8C = {.x = 29191, .y = 50374, .z = 3289};
typedef struct Row_24C98 { float w0; float w1; float w2; float w3; int32_t w4; } Row_24C98;
static const Row_24C98 data_24C98[2] = {
  {-11.2439f, 2.9721f, -23.097f, 0.0f, 16384},
  {0.0f, -0.276f, 0.162f, -0.51f, 8372}
};
static const int32_t data_24CC0[2] = {0, 44059};
static const char data_24CC8[6] __attribute__((scn_string_literal)) = "BROOM";
static const char data_24CCE[10] __attribute__((scn_string_literal)) = "E1011A103";
static const char data_24CD8[10] __attribute__((scn_string_literal)) = "E1011A104";
static const char data_24CE2[10] __attribute__((scn_string_literal)) = "E1011A105";
static const Vector3 data_24CEC = {.x = 0.1173f, .y = 0.0177f, .z = -0.0934f};
static const Vector3i data_24CF8 = {.x = 1063, .y = 949, .z = 275};
static const int32_t data_24D04[6] = {49152, 0, 16384, 0, 8008, 0};
static const float data_24D1C[3] = {-11.2741f, 2.3605f, -21.9512f};
static const Vector3 data_24D28 = {.x = 0.025f, .y = 1.252f, .z = 0.5f};
static const Vector3i data_24D34 = {.x = 16384, .y = 0, .z = 16384};
static const char data_24D40[10] __attribute__((scn_string_literal)) = "E1011A082";
static const char data_24D4A[10] __attribute__((scn_string_literal)) = "E1011A045";
static const uint32_t data_24D54 = 1;
static const float data_24D58[3] = {-12.14f, -23.0f, 0.1f};
static const int32_t data_24D64 = 196608;
static const char data_24D68[10] __attribute__((scn_string_literal)) = "E1011A116";
static const char data_24D72[10] __attribute__((scn_string_literal)) = "E1011A117";
static const char data_24D7C[12] __attribute__((scn_string_literal)) = "E1011A118";
static const uint32_t data_24D88 = 1;
static const uint32_t data_24D8C[2] = {0xC13E6006U, 0xC1B823FEU};
static const float data_24D94 = 0.1f;
static const int32_t data_24D98 = 2;
static const uint32_t data_24D9C[2] = {0xC13FDDF7U, 0xC1D24001U};
static const float data_24DA4 = 0.1f;
static const int32_t data_24DA8 = 3;
static const float data_24DAC[3] = {-12.6812f, -26.243f, 0.1f};
static const int32_t data_24DB8 = 196608;
static const uint32_t data_24DBC[6] = {0xC137472BU, 0, 0xC1BBAF1CU, 0xC1B37A6BU, 0, 0xC1B37A6BU};
static const Vector3 data_24DD4 = {.x = -14.8742f, .y = 0.0f, .z = -26.414801f};
static const Vector3 data_24DE0 = {.x = -11.4742f, .y = 0.0f, .z = -24.614799f};
static const float data_24DEC[4] = {0.03f, 1.85f, 0.7f, 0.0f};
static const int32_t data_24DFC[2] = {16384, 0};
static const float data_24E04[2] = {-10.8f, 1.1095f};
static const uint32_t data_24E0C = 0xC1C1353FU;
static const Vector3 data_24E10 = {.x = -10.163f, .y = 1.0765f, .z = -24.112f};
static const Vector3i data_24E1C = {.x = 29191, .y = 50374, .z = 3289};
static const Vector3 data_24E28 = {.x = -10.163f, .y = 1.0765f, .z = -22.6f};
static const Vector3i data_24E34 = {.x = 29191, .y = 50374, .z = 3289};
static const uint32_t data_24E40 = 1;
typedef struct Row_24E44 { float w0; float w1; float w2; int32_t w3; } Row_24E44;
static const Row_24E44 data_24E44[2] = {
  {-4.04f, -24.0f, 1.0f, 1},
  {-2.8f, -24.55f, 1.0f, 196608}
};
static const char data_24E64[36] = "E1011A023\000\000\000\246\012\366\300\262\235\207?,\324\210\301\000\000\000\000K\234";
static const Vector3 data_24E88 = {.x = 0.0922f, .y = 0.1018f, .z = 0.00806f};
static const Vector3i data_24E94 = {.x = 0, .y = 0, .z = 0};
static const Vector3 data_24EA0 = {.x = 0.0922f, .y = 0.1018f, .z = 0.00806f};
static const Vector3i data_24EAC = {.x = 0, .y = 0, .z = 0};
static const char data_24EB8[10] __attribute__((scn_string_literal)) = "E1011A031";
static const char data_24EC2[10] __attribute__((scn_string_literal)) = "E1011A101";
static const char data_24ECC[10] __attribute__((scn_string_literal)) = "E1011A031";
static const char data_24ED6[10] __attribute__((scn_string_literal)) = "E1011A026";
static const char data_24EE0[10] __attribute__((scn_string_literal)) = "E1011A027";
static const char data_24EEA[10] __attribute__((scn_string_literal)) = "E1011A028";
static const char data_24EF4[10] __attribute__((scn_string_literal)) = "E1011A029";
static const char data_24EFE[10] __attribute__((scn_string_literal)) = "E1011A030";
static const char data_24F08[10] __attribute__((scn_string_literal)) = "E1011A032";
static const char data_24F12[10] __attribute__((scn_string_literal)) = "E1011A033";
static const char data_24F1C[10] __attribute__((scn_string_literal)) = "E1011A074";
static const char data_24F26[10] __attribute__((scn_string_literal)) = "E1011A077";
static const char data_24F30[10] __attribute__((scn_string_literal)) = "E1011A076";
static const char data_24F3A[10] __attribute__((scn_string_literal)) = "E1011A017";
static const char data_24F44[10] __attribute__((scn_string_literal)) = "E1011A016";
static const char data_24F4E[10] __attribute__((scn_string_literal)) = "E1011A018";
static const char data_24F58[10] __attribute__((scn_string_literal)) = "E1011A007";
static const char data_24F62[10] __attribute__((scn_string_literal)) = "E1011A016";
static const char data_24F6C[10] __attribute__((scn_string_literal)) = "E1011A034";
static const char data_24F76[10] __attribute__((scn_string_literal)) = "E1011A035";
static const char data_24F80[10] __attribute__((scn_string_literal)) = "E1011A036";
static const char data_24F8A[10] __attribute__((scn_string_literal)) = "E1011A037";
static const char data_24F94[10] __attribute__((scn_string_literal)) = "E1011A034";
static const char data_24F9E[10] __attribute__((scn_string_literal)) = "E1011A038";
static const char data_24FA8[10] __attribute__((scn_string_literal)) = "E1011A039";
static const char data_24FB2[10] __attribute__((scn_string_literal)) = "E1011A040";
static const char data_24FBC[10] __attribute__((scn_string_literal)) = "E1011A041";
static const char data_24FC6[10] __attribute__((scn_string_literal)) = "E1011A042";
static const char data_24FD0[10] __attribute__((scn_string_literal)) = "E1011A007";
static const char data_24FDA[10] __attribute__((scn_string_literal)) = "E1011A008";
static const char data_24FE4[10] __attribute__((scn_string_literal)) = "E1011A034";
static const char data_24FEE[10] __attribute__((scn_string_literal)) = "E1011A035";
static const char data_24FF8[10] __attribute__((scn_string_literal)) = "E1011A082";
static const char data_25002[10] __attribute__((scn_string_literal)) = "E1011A045";
static const char data_2500C[10] __attribute__((scn_string_literal)) = "E1011A007";
static const char data_25016[10] __attribute__((scn_string_literal)) = "E1011A046";
static const char data_25020[10] __attribute__((scn_string_literal)) = "E1011A051";
static const char data_2502A[10] __attribute__((scn_string_literal)) = "E1011A052";
static const char data_25034[10] __attribute__((scn_string_literal)) = "E1011A053";
static const char data_2503E[10] __attribute__((scn_string_literal)) = "E1011A054";
static const char data_25048[10] __attribute__((scn_string_literal)) = "E1011A055";
static const char data_25052[10] __attribute__((scn_string_literal)) = "E1011A056";
static const char data_2505C[10] __attribute__((scn_string_literal)) = "E1011A057";
static const char data_25066[10] __attribute__((scn_string_literal)) = "E1011A047";
static const char data_25070[10] __attribute__((scn_string_literal)) = "E1011A045";
static const char data_2507A[10] __attribute__((scn_string_literal)) = "E1011A007";
static const char data_25084[10] __attribute__((scn_string_literal)) = "E1011A046";
static const char data_2508E[10] __attribute__((scn_string_literal)) = "E1011A048";
static const char data_25098[10] __attribute__((scn_string_literal)) = "E1011A049";
static const char data_250A2[10] __attribute__((scn_string_literal)) = "E1011A050";
static const char data_250AC[10] __attribute__((scn_string_literal)) = "E1011A070";
static const char data_250B6[10] __attribute__((scn_string_literal)) = "E1011A071";
static const char data_250C0[10] __attribute__((scn_string_literal)) = "E1011A072";
static const char data_250CA[10] __attribute__((scn_string_literal)) = "E1011A087";
static const char data_250D4[10] __attribute__((scn_string_literal)) = "E1011A017";
static const char data_250DE[10] __attribute__((scn_string_literal)) = "E1011A016";
static const char data_250E8[10] __attribute__((scn_string_literal)) = "E1011A018";
static const char data_250F2[10] __attribute__((scn_string_literal)) = "E1011A065";
static const char data_250FC[10] __attribute__((scn_string_literal)) = "E1011A007";
static const char data_25106[10] __attribute__((scn_string_literal)) = "E1011A063";
static const char data_25110[10] __attribute__((scn_string_literal)) = "E1011A017";
static const char data_2511A[10] __attribute__((scn_string_literal)) = "E1011A016";
static const char data_25124[10] __attribute__((scn_string_literal)) = "E1011A018";
static const char data_2512E[10] __attribute__((scn_string_literal)) = "E1011A065";
static const char data_25138[10] __attribute__((scn_string_literal)) = "E1011A007";
static const char data_25142[10] __attribute__((scn_string_literal)) = "E1011A016";
static const char data_2514C[10] __attribute__((scn_string_literal)) = "E1011A040";
static const char data_25156[10] __attribute__((scn_string_literal)) = "E1011A078";
static const char data_25160[10] __attribute__((scn_string_literal)) = "E1011A089";
static const char data_2516A[10] __attribute__((scn_string_literal)) = "E1011A081";
static const char data_25174[10] __attribute__((scn_string_literal)) = "E1011A090";
static const char data_2517E[10] __attribute__((scn_string_literal)) = "E1011A040";
static const char data_25188[10] __attribute__((scn_string_literal)) = "E1011A042";
static const char data_25192[10] __attribute__((scn_string_literal)) = "E1011A031";
static const char data_2519C[10] __attribute__((scn_string_literal)) = "E1011A091";
static const char data_251A6[10] __attribute__((scn_string_literal)) = "E1011A092";
static const char data_251B0[10] __attribute__((scn_string_literal)) = "E1011A093";
static const char data_251BA[10] __attribute__((scn_string_literal)) = "E1011A042";
static const char data_251C4[10] __attribute__((scn_string_literal)) = "E1011A094";
static const char data_251CE[10] __attribute__((scn_string_literal)) = "E1011A095";
static const char data_251D8[10] __attribute__((scn_string_literal)) = "E1011A096";
static const char data_251E2[10] __attribute__((scn_string_literal)) = "E1011A097";
static const char data_251EC[10] __attribute__((scn_string_literal)) = "E1011A098";
static const char data_251F6[10] __attribute__((scn_string_literal)) = "E1011A031";
static const Vector3 data_25200 = {.x = 0.332f, .y = 0.001f, .z = 0.223f};
static const Vector3i data_2520C = {.x = 0, .y = 182, .z = 10555};
static const char data_25218[10] __attribute__((scn_string_literal)) = "E1011A006";
static const char data_25222[10] __attribute__((scn_string_literal)) = "E1011A102";
static const uint32_t data_2522C = 0x3ECCCCCD;
typedef struct Row_25230 { float w0; float w1; int32_t w2; int32_t w3; int32_t w4; float w5; } Row_25230;
static const Row_25230 data_25230[2] = {
  {0.0f, -22.5f, 16384, 40192, 4, -0.55f},
  {1.0f, -22.5f, 49152, 39176, 3, 100000.0f}
};
static const uint8_t data_25260[8] = {0};
static const float data_25268[2] = {0.6f, 0.0f};
static const int32_t data_25270 = 1;
static const float data_25274 = 0.6f;
static const int32_t data_25278[10] = {1, 0, 0, 2, 1, 1, 2, 0, 2, 3};
static const char data_252A0[7] __attribute__((scn_string_literal)) = "SA1118";
static const char data_252A7[11] __attribute__((scn_string_literal)) = "SA1118A001";
static const char data_252B2[11] __attribute__((scn_string_literal)) = "SA1118A002";
static const char data_252BD[11] __attribute__((scn_string_literal)) = "SA1118A003";
static const char data_252C8[7] __attribute__((scn_string_literal)) = "SA1118";
static const char data_252CF[6] __attribute__((scn_string_literal)) = "YKB_M";
static const char data_252D5[27] = "YKC_M\000\000\001\000\000\000\315\314,\300\315\314\262\301\000\000\200?\000\000\003";
static const uint16_t data_252F0[8] = {670, 672, 674, 677, 684, 682, 680, 0};
static uint32_t data_25300 = 1;
static const int32_t data_25304[4] = {0, 0, 0, 196608};
static const char data_25314[40] = "frame_count = %d\012\000\000\000\001\000\000\000\232\231\031\300\000\000 @\000\000\200?\000\000\003";
static const Vector4 data_2533C = {.x = 1.0f, .y = 1.0f, .z = 1.0f, .w = 1.0f};
static const char data_2534C[13] __attribute__((scn_string_literal)) = "M_01_FUK.BIN";
static const char data_25359[6] __attribute__((scn_string_literal)) = "BROOM";
static const char data_2535F[11] __attribute__((scn_string_literal)) = "SA1037A001";
static const char data_2536A[11] __attribute__((scn_string_literal)) = "SA1037B001";
static const char data_25375[11] __attribute__((scn_string_literal)) = "SA1037A002";
static const char data_25380[11] __attribute__((scn_string_literal)) = "SA1037B002";
static const char data_2538B[11] __attribute__((scn_string_literal)) = "SA1037A003";
static const char data_25396[11] __attribute__((scn_string_literal)) = "SA1037B003";
static const char data_253A1[11] __attribute__((scn_string_literal)) = "SA1037A004";
static const char data_253AC[11] __attribute__((scn_string_literal)) = "SA1037B004";
static const char data_253B7[11] __attribute__((scn_string_literal)) = "SA1037A005";
static const char data_253C2[11] __attribute__((scn_string_literal)) = "SA1037B005";
static const char data_253CD[11] __attribute__((scn_string_literal)) = "SA1037A006";
static const char data_253D8[11] __attribute__((scn_string_literal)) = "SA1037B006";
static const char data_253E3[11] __attribute__((scn_string_literal)) = "SA1037A007";
static const char data_253EE[13] __attribute__((scn_string_literal)) = "a1_fukay.snd";
static const char data_253FB[11] __attribute__((scn_string_literal)) = "bgm085.snd";
static const char data_25406[16] __attribute__((scn_string_literal)) = "/scene/02/JHD0/";
static const char data_25416[5] __attribute__((scn_string_literal)) = "SORA";
static const char data_2541B[5] __attribute__((scn_string_literal)) = "SORA";
static const char data_25420[1] __attribute__((scn_string_literal)) = "";
static const char data_25421[5] __attribute__((scn_string_literal)) = "SORA";
static const char data_25426[1] __attribute__((scn_string_literal)) = "";
static const char data_25427[5] __attribute__((scn_string_literal)) = "SORA";
static const char data_2542C[11] __attribute__((scn_string_literal)) = "bgm077.snd";
static const char data_25437[13] __attribute__((scn_string_literal)) = "a1_ineko.snd";
static const char data_25444[6] __attribute__((scn_string_literal)) = "01MIR";
static const char data_2544A[16] __attribute__((scn_string_literal)) = "/scene/02/JHD0/";
static const char data_2545A[5] __attribute__((scn_string_literal)) = "INEK";
static const char data_2545F[5] __attribute__((scn_string_literal)) = "INEK";
static const char data_25464[1] __attribute__((scn_string_literal)) = "";
static const char data_25465[5] __attribute__((scn_string_literal)) = "INEK";
static const char data_2546A[13] __attribute__((scn_string_literal)) = "f1doujou.snd";
static const char data_25477[13] __attribute__((scn_string_literal)) = "bgm009.snd";
static const uint32_t data_25484 = SHEN_NAME('KOI0');
static const shen_name data_25488[4] = {SHEN_NAME('KOI1'), SHEN_NAME('KOI2'), SHEN_NAME('KOI3'), SHEN_NAME('KOI4')};
static const uint32_t data_25498 = SHEN_NAME('HAS0');
static const shen_name data_2549C[20] = {
  SHEN_NAME('HAS1'), SHEN_NAME('HAS2'), SHEN_NAME('HAS3'), SHEN_NAME('HAS4'), SHEN_NAME('HAS5'), SHEN_NAME('HAS6'), SHEN_NAME('HAS7'),
  SHEN_NAME('HAS8'), SHEN_NAME('HAS9'), SHEN_NAME('HASA'), SHEN_NAME('HASB'), SHEN_NAME('HASC'), SHEN_NAME('HASD'), SHEN_NAME('HASE'),
  SHEN_NAME('HASF'), SHEN_NAME('HAN0'), SHEN_NAME('HAN1'), SHEN_NAME('HAN2'), SHEN_NAME('HAN3'), SHEN_NAME('HAN4')
};
static const int32_t data_254EC = -1;
static const uint32_t data_254F0 = 1;
typedef struct Row_254F4 { float w0; float w1; float w2; int32_t w3; } Row_254F4;
static const Row_254F4 data_254F4[7] = {
  {9.2f, 7.2f, 0.4f, 1},
  {8.6f, 6.6f, 0.4f, 1},
  {6.6f, 6.8f, 0.4f, 1},
  {5.4f, 7.6f, 0.4f, 1},
  {5.4f, 8.6f, 0.4f, 1},
  {6.0f, 9.4f, 0.4f, 1},
  {8.2f, 9.2f, 0.4f, 196608}
};
static const uint32_t data_25564 = 1;
typedef struct Row_25568 { float w0; float w1; float w2; int32_t w3; } Row_25568;
static const Row_25568 data_25568[4] = {
  {9.2f, 8.2f, 0.6f, 1},
  {8.3f, 6.9f, 0.6f, 1},
  {7.3f, 7.1f, 0.8f, 1},
  {7.6f, 7.9f, 0.6f, 196608}
};
static const uint32_t data_255A8 = 1;
typedef struct Row_255AC { float w0; float w1; float w2; int32_t w3; } Row_255AC;
static const Row_255AC data_255AC[11] = {
  {5.8f, 8.5f, 0.4f, 1},
  {6.2f, 7.3f, 0.4f, 1},
  {7.1f, 7.8f, 0.4f, 1},
  {8.3f, 8.9f, 0.4f, 1},
  {9.5f, 8.0f, 0.4f, 1},
  {8.7f, 6.7f, 0.4f, 1},
  {6.7f, 6.5f, 0.4f, 1},
  {6.3f, 7.2f, 0.4f, 1},
  {7.1f, 7.9f, 0.4f, 1},
  {7.5f, 9.2f, 0.4f, 1},
  {6.3f, 9.6f, 0.4f, 196608}
};
static const uint32_t data_2565C = 1;
typedef struct Row_25660 { float w0; float w1; float w2; int32_t w3; } Row_25660;
static const Row_25660 data_25660[5] = {
  {6.5f, 9.2f, 0.6f, 1},
  {5.7f, 8.5f, 1.0f, 1},
  {6.3f, 7.4f, 1.0f, 1},
  {7.5f, 7.6f, 1.0f, 1},
  {7.5f, 8.8f, 1.0f, 196608}
};
static const uint32_t data_256B0 = 1;
typedef struct Row_256B4 { float w0; float w1; float w2; int32_t w3; } Row_256B4;
static const Row_256B4 data_256B4[6] = {
  {6.7f, 7.5f, 1.0f, 1},
  {7.1f, 6.6f, 1.0f, 1},
  {9.0f, 6.4f, 1.0f, 1},
  {10.1f, 7.8f, 1.0f, 1},
  {9.2f, 9.3f, 1.0f, 1},
  {7.7f, 8.2f, 1.0f, 196608}
};
static const Vector3 data_25714 = {.x = 9.3f, .y = -0.08f, .z = 11.0f};
static const char data_25720[13] __attribute__((scn_string_literal)) = "model/object";
static const char data_2572D[12] __attribute__((scn_string_literal)) = "trlake1.pvr";
static const char data_25739[13] __attribute__((scn_string_literal)) = "model/object";
static const char data_25746[13] __attribute__((scn_string_literal)) = "trsea025.mt6";
static const char data_25753[13] __attribute__((scn_string_literal)) = "model/object";
static const char data_25760[48] = "grid025.bin\000\001\000\000\000\232\231i\300ff\256\301\000\000\200?\002\000\000\000ff\226\300\000\000\244\301\000\000\200?\000\000\003";
static const int32_t data_25790[57] = {
  0, 0, -5461, 0, -2366, -1092, 0, -182, 1092, 0, 182, -3458, 0, 0, -2548, 0, 0, -3458, 0, -182, 1274, 0, 910, -910, 0, 0, -2366, 0, 0, -5097, 0, 0,
  1456, 0, 182, -1820, 0, 0, -3276, 0, 0, -4915, -182, -182, 1820, 0, -728, -2730, 0, -1274, -5461, 0, -910, -3640, 0, 0, 1638
};
static const Vector4 data_25874 = {.x = 0.3f, .y = 0.3f, .z = 0.3f, .w = 0.3f};
static const char data_25884[16] __attribute__((scn_string_literal)) = "/scene/02/JHD0/";
static const char data_25894[5] __attribute__((scn_string_literal)) = "KAKG";
static const char data_25899[5] __attribute__((scn_string_literal)) = "KAKG";
static const char data_2589E[1] __attribute__((scn_string_literal)) = "";
static const char data_2589F[5] __attribute__((scn_string_literal)) = "KAKG";
static const char data_258A4[1] __attribute__((scn_string_literal)) = "";
static const char data_258A5[11] __attribute__((scn_string_literal)) = "bgm012.snd";
static const char data_258B0[6] __attribute__((scn_string_literal)) = "YKF_M";
static const char data_258B6[6] __attribute__((scn_string_literal)) = "FUK_M";
static const char data_258BC[6] __attribute__((scn_string_literal)) = "YKC_M";
static const char data_258C2[10] __attribute__((scn_string_literal)) = "E1031A008";
static const char data_258CC[10] __attribute__((scn_string_literal)) = "E1031A009";
static const char data_258D6[10] __attribute__((scn_string_literal)) = "E1031A010";
static const char data_258E0[10] __attribute__((scn_string_literal)) = "E1031A011";
static const char data_258EA[10] __attribute__((scn_string_literal)) = "E1031B014";
static const char data_258F4[10] __attribute__((scn_string_literal)) = "E1031B015";
static const char data_258FE[5] __attribute__((scn_string_literal)) = "KAKG";
static const char data_25903[11] __attribute__((scn_string_literal)) = "bgm009.snd";
static const char data_2590E[6] __attribute__((scn_string_literal)) = "E1031";
static const char data_25914[6] __attribute__((scn_string_literal)) = "YKF_M";
static const char data_2591A[11] __attribute__((scn_string_literal)) = "M_FGT1.BIN";
static const char data_25925[10] __attribute__((scn_string_literal)) = "E1031B009";
static const char data_2592F[10] __attribute__((scn_string_literal)) = "E1031B007";
static const char data_25939[11] __attribute__((scn_string_literal)) = "E1031B008";
static const uint32_t data_25944 = 3;
static const float data_25948[3] = {-2.99f, -22.55f, 1.0f};
static const int32_t data_25954 = 196608;
static const char data_25958[6] __attribute__((scn_string_literal)) = "E1040";
static const char data_2595E[10] __attribute__((scn_string_literal)) = "E1040A028";
static const char data_25968[10] __attribute__((scn_string_literal)) = "E1040A029";
static const char data_25972[6] __attribute__((scn_string_literal)) = "E1040";
static const char data_25978[10] __attribute__((scn_string_literal)) = "E1040A030";
static const char data_25982[10] __attribute__((scn_string_literal)) = "E1040A031";
static const char data_2598C[10] __attribute__((scn_string_literal)) = "E1040A032";
static const char data_25996[6] __attribute__((scn_string_literal)) = "E1040";
static const Vector3 data_2599C = {.x = -7.13f, .y = 1.0f, .z = -24.79f};
static const char data_259A8[13] __attribute__((scn_string_literal)) = "e1shugyo.snd";
static const char data_259B5[10] __attribute__((scn_string_literal)) = "E1040A001";
static const char data_259BF[10] __attribute__((scn_string_literal)) = "E1040A003";
static const char data_259C9[10] __attribute__((scn_string_literal)) = "E1040A009";
static const char data_259D3[10] __attribute__((scn_string_literal)) = "E1040A010";
static const char data_259DD[10] __attribute__((scn_string_literal)) = "E1040A012";
static const char data_259E7[10] __attribute__((scn_string_literal)) = "E1040A013";
static const char data_259F1[10] __attribute__((scn_string_literal)) = "E1040A014";
static const char data_259FB[10] __attribute__((scn_string_literal)) = "E1040A018";
static const char data_25A05[10] __attribute__((scn_string_literal)) = "E1040A020";
static const char data_25A0F[10] __attribute__((scn_string_literal)) = "E1040A015";
static const char data_25A19[10] __attribute__((scn_string_literal)) = "E1040A016";
static const char data_25A23[10] __attribute__((scn_string_literal)) = "E1040A017";
static const char data_25A2D[6] __attribute__((scn_string_literal)) = "E1040";
static const char data_25A33[9] __attribute__((scn_string_literal)) = "E1040";
static const Vector3 data_25A3C = {.x = -7.15f, .y = 1.0f, .z = -20.68f};
static const char data_25A48[10] __attribute__((scn_string_literal)) = "YGB_B.MT5";
static const char data_25A52[10] __attribute__((scn_string_literal)) = "YKA_B.MT5";
static const char data_25A5C[16] __attribute__((scn_string_literal)) = "battle_1.snd";
static const Vector3 data_25A6C = {.x = -11.1987f, .y = 0.9f, .z = -16.3866f};
static const Vector3 data_25A78 = {.x = -3.5184f, .y = 1.2f, .z = -28.564899f};
static const uint32_t data_25A84 = 1;
static const float data_25A88[3] = {-8.0f, -22.5f, 1.0f};
static const int32_t data_25A94 = 196608;
static const char data_25A98[11] __attribute__((scn_string_literal)) = "bgm109.snd";
static const char data_25AA3[45] = "bgm009.snd\000\000\000\232\231aA\000\000\322B\315\314L=\000\000\000\377\000\000\000\000\000\000\000\000H:4\273H:4\273";
static const uint32_t data_25AD0 = 380;
static const int32_t data_25AD4[6] = {1, 390, 0, 1018, 0, -1};
static const uint32_t data_25AEC = 510;
static const int32_t data_25AF0[6] = {1, 525, 0, 1020, 0, -1};
static const uint32_t data_25B08 = 150;
static const int32_t data_25B0C[6] = {1, 160, 0, 1019, 0, -1};
static const uint32_t data_25B24 = 530;
static const int32_t data_25B28[6] = {1, 970, 0, 1014, 0, -1};
static const uint32_t data_25B40 = 911;
static const int32_t data_25B44[6] = {1, 155, 0, 1015, 0, -1};
static const char data_25B5C[7] __attribute__((scn_string_literal)) = "FREE02";
static const char data_25B63[7] __attribute__((scn_string_literal)) = "FREE03";
static const char data_25B6A[7] __attribute__((scn_string_literal)) = "FREE04";
static const char data_25B71[7] __attribute__((scn_string_literal)) = "FREE01";
static const uint32_t data_25B78 = 0x41C00000;
static const float data_25B7C[2] = {0.0f, -33.0f};
static const uint32_t data_25B84 = 0x41A00000;
static const float data_25B88[2] = {0.0f, 18.0f};
static const uint32_t data_25B90 = 0xC1B80000;
static const float data_25B94[2] = {0.0f, 13.0f};
static const uint32_t data_25B9C = 0x40E00000;
static const float data_25BA0[2] = {0.0f, 12.0f};
static const uint8_t data_25BA8[8] = {0};
static const float data_25BB0 = -35.0f;
static const uint8_t data_25BB4[8] = {0};
static const float data_25BBC = -35.0f;
static const uint32_t data_25BC0 = 0x42300000;
static const float data_25BC4[2] = {0.0f, -24.0f};
static const uint32_t data_25BCC = 0xC1A00000;
static const float data_25BD0[2] = {0.0f, 20.0f};
static const uint32_t data_25BD8 = 0x3F800000;
static const float data_25BDC[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.8f, 0.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint32_t data_25C60 = 0x3F800000;
static const float data_25C64[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.8f, 0.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint32_t data_25CE8 = 0x3F800000;
static const float data_25CEC[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.8f, 0.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint32_t data_25D70 = 0x3F800000;
static const float data_25D74[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint8_t data_25DF8[80] = {0};
typedef struct Row_25E48 { float w0; float w1; float w2; int32_t w3; } Row_25E48;
static const Row_25E48 data_25E48[3] = {
  {0.8f, 0.8f, 0.8f, 0},
  {0.0f, 0.0f, 0.0f, 0},
  {0.0f, 0.0f, 0.0f, 0}
};
static const int32_t data_25E78[2] = {0, 0};
static const uint8_t data_25E80[96] = {0};
static const float data_25EE0[10] = {1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f};
static const uint32_t data_25F08 = 0x3F800000;
static const float data_25F0C[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint32_t data_25F90 = 0x3F800000;
static const float data_25F94[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const char data_26018[12] __attribute__((scn_string_literal)) = "system1.snd";
static const char data_26024[11] __attribute__((scn_string_literal)) = "amb036.snd";
static const char data_2602F[11] __attribute__((scn_string_literal)) = "bgm085.snd";
static const char data_2603A[13] __attribute__((scn_string_literal)) = "a1_houha.snd";
static const char data_26047[13] __attribute__((scn_string_literal)) = "battle_1.snd";
static const char data_26054[12] __attribute__((scn_string_literal)) = "system1.snd";
static const char data_26060[11] __attribute__((scn_string_literal)) = "amb036.snd";
static const char data_2606B[11] __attribute__((scn_string_literal)) = "bgm009.snd";
static const char data_26076[13] __attribute__((scn_string_literal)) = "f1doujou.snd";
static const char data_26083[13] __attribute__((scn_string_literal)) = "battle_1.snd";
static const char data_26090[16] __attribute__((scn_string_literal)) = "/scene/02/JHD0/";
static const char data_260A0[5] __attribute__((scn_string_literal)) = "JHD0";
static const char data_260A5[13] __attribute__((scn_string_literal)) = "HMOT_YKA.BIN";
static const char data_260B2[16] __attribute__((scn_string_literal)) = "/scene/02/JHD0/";
static const char data_260C2[5] __attribute__((scn_string_literal)) = "MPSN";
static const char data_260C7[16] __attribute__((scn_string_literal)) = "/scene/02/JHD0/";
static const char data_260D7[5] __attribute__((scn_string_literal)) = "MPSU";
static const char data_260DC[16] __attribute__((scn_string_literal)) = "/scene/02/JHD0/";
static const char data_260EC[5] __attribute__((scn_string_literal)) = "HOUO";
static const char data_260F1[11] __attribute__((scn_string_literal)) = "M_MOBJ.BIN";
static const char data_260FC[14] __attribute__((scn_string_literal)) = "scene/02/JOMO";
static const char data_2610A[] __attribute__((scn_string_literal)) = "SCRTEST.BIN";
static const uint8_t data_26116[2] = {0};
static const Vector3 data_26118 = {.x = -0.78f, .y = 0.0f, .z = -16.476601f};
static const Vector3 data_26124 = {.x = -14.0f, .y = 0.0f, .z = -28.671101f};
static const char data_26130[6] __attribute__((scn_string_literal)) = "YKC_M";
static const char data_26136[6] __attribute__((scn_string_literal)) = "E1011";
static const char data_2613C[6] __attribute__((scn_string_literal)) = "01DIS";
static const char data_26142[6] __attribute__((scn_string_literal)) = "FUK_M";
static const char data_26148[5] __attribute__((scn_string_literal)) = "HOUO";
static const char data_2614D[1] __attribute__((scn_string_literal)) = "";
static const char data_2614E[5] __attribute__((scn_string_literal)) = "HOUO";
static const char data_26153[6] __attribute__((scn_string_literal)) = "YKB_M";
static const char data_26159[11] __attribute__((scn_string_literal)) = "bgm009.snd";
static const char data_26164[13] __attribute__((scn_string_literal)) = "f1doujou.snd";
static const char data_26171[7] __attribute__((scn_string_literal)) = "SA1037";
static const char data_26178[6] __attribute__((scn_string_literal)) = "FUB_M";
static const char data_2617E[6] __attribute__((scn_string_literal)) = "01SOR";
static const char data_26184[6] __attribute__((scn_string_literal)) = "FUB_M";
static const char data_2618A[6] __attribute__((scn_string_literal)) = "INE_M";
static const char data_26190[6] __attribute__((scn_string_literal)) = "YKB_M";
static const char data_26196[16] __attribute__((scn_string_literal)) = "/scene/02/JHD0/";
static const char data_261A6[5] __attribute__((scn_string_literal)) = "MPM2";
static const char data_261AB[16] __attribute__((scn_string_literal)) = "/scene/02/JHD0/";
static const char data_261BB[5] __attribute__((scn_string_literal)) = "MPM1";
static const char data_261C0[5] __attribute__((scn_string_literal)) = "MPM2";
static const char data_261C5[1] __attribute__((scn_string_literal)) = "";
static const char data_261C6[5] __attribute__((scn_string_literal)) = "MPM1";
static const char data_261CB[1] __attribute__((scn_string_literal)) = "";
static const char data_261CC[5] __attribute__((scn_string_literal)) = "MPM2";
static const char data_261D1[7] __attribute__((scn_string_literal)) = "MPM1";
static const uint32_t data_261D8 = 0xC17CCCCD;
static const float data_261DC[41] = {
  -13.7f, -15.9f, -30.6f, 2.0f, -30.6f, 2.0f, -13.7f, 0.0f, 7.0f, -6.75f, 3.0f, -22.5f, 9.0f, 3.89f, 0.83f, -5.75f, 0.83f, -6.08f, -11.99f, 3.88f,
  -12.0f, 0.0f, 2.75f, -0.75f, 1.4f, -5.0f, 6.0f, 1.02f, 9.46f, 0.91f, 12.5f, -3.0f, 12.5f, -3.0f, 9.5f, 0.0f, 2.5f, -0.85f, 1.25f, 11.0f, 3.0f
};
static const FogColourData data_26280 = {.red = 0.1f, .green = 360.0f, .blue = 1.14f, .colour = 0xFF17191BU};
static const Vector4 data_26290 = {.x = 1.0f, .y = 0.16f, .z = 0.12f, .w = 0.0f};
static const float data_262A0[4] = {1.0f, 0.3f, 0.3f, 0.3f};
static const Vector3 data_262B0 = {.x = 2.8079f, .y = 0.0f, .z = 9.2562f};
static const Vector3 data_262BC = {.x = 9.8262f, .y = 0.0f, .z = 2.1434f};
static const Vector3 data_262C8 = {.x = 12.9789f, .y = 0.0f, .z = -13.1704f};
static const Vector3 data_262D4 = {.x = -0.4989f, .y = 0.0f, .z = -30.642f};
static const Vector3 data_262E0 = {.x = -0.78f, .y = 0.0f, .z = -16.476601f};
static const Vector3 data_262EC = {.x = -14.0f, .y = 0.0f, .z = -28.671101f};
static const Vector3 data_262F8 = {.x = 9.25f, .y = 0.0f, .z = -30.33f};
static const Vector3 data_26304 = {.x = 6.5f, .y = 0.0f, .z = -27.0f};
static const char data_26310[16] __attribute__((scn_string_literal)) = "/scene/02/JHD0/";
static const char data_26320[5] __attribute__((scn_string_literal)) = "MPS1";
static const char data_26325[16] __attribute__((scn_string_literal)) = "/scene/02/JHD0/";
static const char data_26335[5] __attribute__((scn_string_literal)) = "MPS2";
static const char data_2633A[16] __attribute__((scn_string_literal)) = "/scene/02/JHD0/";
static const char data_2634A[5] __attribute__((scn_string_literal)) = "MPS3";
static const char data_2634F[16] __attribute__((scn_string_literal)) = "/scene/02/JHD0/";
static const char data_2635F[5] __attribute__((scn_string_literal)) = "MPS4";
static const char data_26364[16] __attribute__((scn_string_literal)) = "/scene/02/JHD0/";
static const char data_26374[5] __attribute__((scn_string_literal)) = "MPS4";
static const char data_26379[16] __attribute__((scn_string_literal)) = "/scene/02/JHD0/";
static const char data_26389[5] __attribute__((scn_string_literal)) = "MPS5";
static const char data_2638E[16] __attribute__((scn_string_literal)) = "/scene/02/JHD0/";
static const char data_2639E[5] __attribute__((scn_string_literal)) = "MPS5";
static const char data_263A3[16] __attribute__((scn_string_literal)) = "/scene/02/JHD0/";
static const char data_263B3[5] __attribute__((scn_string_literal)) = "MPS6";
static const char data_263B8[16] __attribute__((scn_string_literal)) = "/scene/02/JHD0/";
static const char data_263C8[5] __attribute__((scn_string_literal)) = "MPS6";
static const char data_263CD[16] __attribute__((scn_string_literal)) = "/scene/02/JHD0/";
static const char data_263DD[5] __attribute__((scn_string_literal)) = "MPS6";
static const char data_263E2[16] __attribute__((scn_string_literal)) = "/scene/02/JHD0/";
static const char data_263F2[38] = "MPS7\000\000\236\002\240\002\242\002\245\002\254\002\252\002\250\002\000\003\370\000\372\000\365\000\367\000\366\000\373\000\371\000\000\003";
static const char data_26418[61] __attribute__((scn_string_literal)) = "WAZA Warning : handle is not NULL, and create same handle !\012";
static const char data_26455[30] __attribute__((scn_string_literal)) = "POS POS_RYOU_0 = %f, %f, %f.\012";
static const char data_26473[30] __attribute__((scn_string_literal)) = "POS POS_KAIS_0 = %f, %f, %f.\012";
static const char data_26491[29] __attribute__((scn_string_literal)) = "define ANG_RYOU_0 DEG(%d.0)\012";
static const char data_264AE[29] __attribute__((scn_string_literal)) = "define ANG_KAIS_0 DEG(%d.0)\012";
static const char data_264CB[4] __attribute__((scn_string_literal)) = "%s\012";
static const char data_264CF[37] __attribute__((scn_string_literal)) = "Konnna Error ha Detekonai hazu : %d\012";
static const Vector3 data_264F4 = {.x = 1.0f, .y = 1.0f, .z = 1.0f};
static const Vector3i data_26500 = {.x = 0, .y = 0, .z = 0};
static const char data_2650C[14] __attribute__((scn_string_literal)) = "Entry Model!\012";
static const char data_2651A[38] __attribute__((scn_string_literal)) = "WZ ModoriMove Limit Over !!!!!!!!!!!\012";
static const char data_26540[30] __attribute__((scn_string_literal)) = "WazaDen: Load PackFile %s:%s\012";
static const char data_2655E[34] __attribute__((scn_string_literal)) = "WazaDen: Now Loading PackFile.\012";
