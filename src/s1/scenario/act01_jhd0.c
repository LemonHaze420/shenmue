#include "act01_jhd0.h"

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
static void script_FF36(void);
static void script_1009F(void);
static void script_100D7(void);
static int32_t script_10105(int32_t arg0, int32_t arg1);
static int32_t script_10195(void);
static void script_102CA(void);
static void script_103C5(void);
static void script_103C8(void);
static int32_t script_106C8(void);
static int32_t script_106F1(void);
static int32_t script_10A16(void);
static int32_t script_10A78(void);
static void script_113C7(void);
static void script_1140E(void);
static void script_1147C(void);
static void script_114F3(void);
static int32_t script_11591(int32_t arg0);
static void script_115FA(void);
static void script_1196D(void);
static void script_11EF2(void);
static void script_12311(void);
static void script_12522(void);
static int32_t script_12719(void);
static void script_1278B(void);
static void script_129FF(void);
static int32_t script_12AF4(void);
static void script_12B52(void);
static void script_12BAF(void);
static int32_t script_12C0C(void);
static void script_12CF6(void);
static int32_t script_133C9(void);
static void script_13424(int32_t arg0);
static void script_13684(void);
static int32_t script_136BF(void);
static void script_13727(int32_t arg0);
static void script_13973(int32_t arg0);
static void script_13A6D(int32_t arg0);
static void script_13B7A(int32_t arg0);
static void script_13C74(int32_t arg0);
static void script_13D81(void);
static int32_t script_141DE(void);
static void script_14221(void);
static int32_t script_144C9(void);
static int32_t script_1453C(void);
static void script_14964(void);
static void script_14A03(void);
static void script_14A42(void);
static int32_t script_14A6B(void);
static void script_14ADF(void);
static void script_14BD6(void);
static void script_14D8D(void);
static void script_14F6C(void);
static void script_14FA5(void);
static void script_15042(void);
static void script_150B8(void);
static void script_1516C(void);
static int32_t script_15236(void);
static void script_15305(void);
static void script_153E1(void);
static int32_t script_15428(void);
static void script_15666(void);
static void script_156D2(void);
static void script_15713(void);
static void script_1578A(void);
static int32_t script_157ED(void);
static void script_15842(void);
static int32_t script_1591E(void);
static void script_159A1(void);
static void script_15A74(void);
static int32_t script_15B47(void);
static void script_15BA7(void);
static void script_15CDB(void);
static int32_t script_15DEC(void);
static int32_t script_15EDA(void);
static void script_15FDC(void);
static void script_16147(void);
static void script_161A4(void);
static void script_1621A(void);
static void script_162A7(void);
static int32_t script_16402(void);
static void script_16413(void);
static int32_t script_1697C(void);
static void script_169D2(void);
static int32_t script_169EB(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_16A3B(void);
static void script_16B1E(void);
static void script_16B72(void);
static void script_16EA6(void);
static void script_17029(void);
static void script_170A2(int32_t arg0);
static int32_t script_1747B(int32_t arg0, int32_t arg1);
static int32_t script_1748D(int32_t arg0, int32_t arg1);
static int32_t script_174D0(void);
static int32_t script_1765C(void);
static int32_t script_179FB(void);
static int32_t script_17CBE(void);
static int32_t script_17D07(void);
static void script_17E7B(void);
static int32_t script_17EFE(void);
static void script_17F71(void);
static int32_t script_180BD(void);
static void script_18129(void);
static int32_t script_18137(void);
static void script_1872C(void);
static void script_18836(void);
static void script_18926(int32_t arg0);
static int32_t script_18A8B(int32_t arg0, const void *arg1, float arg2);
static int32_t script_18B05(void);
static void script_18B37(int32_t arg0);
static void script_18CCF(int32_t arg0);
static int32_t script_18D61(void);
static int32_t script_18E9D(void);
static int32_t script_18F66(void);
static void script_18FC0(void);
static int32_t script_1901A(void);
static void script_198BE(void);
static int32_t script_19900(void);
static void script_199DA(void);
static void script_19A11(int32_t arg0);
static void script_19A98(int32_t arg0);
static int32_t script_19AB8(void);
static void script_1A163(void);
static void script_1A86E(void);
static void script_1A8F1(void);
static void script_1B175(void);
static void script_1B8E3(int32_t arg0, int32_t arg1);
static int32_t script_1B922(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_1B9B2(int32_t arg0, int32_t arg1);
static int32_t script_1B9F2(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_1BABF(int32_t arg0, int32_t arg1);
static int32_t script_1BB4D(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4);
static int32_t script_1BBB5(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_1BBFC(int32_t arg0, int32_t arg1, int32_t arg2);
static int32_t script_1BC88(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_1BCEA(int32_t arg0);
static void script_1BD8B(void);
static void script_1BEAD(void);
static void script_1C036(void);
static void script_1C0A0(void);
static void script_1C165(void);
static void script_1C1F7(void);
static void script_1C2CB(void);
static void script_1C3F3(void);
static void script_1C54E(void);
static void script_1C5CB(void);
static void script_1C63E(int32_t arg0);
static void script_1C679(int32_t arg0);
static void script_1C6A3(void);
static void script_1CBC9(void);
static int32_t script_1CE80(void);
static void script_1CEE1(int32_t arg0);
static void script_1D086(int32_t arg0, int32_t arg1);
static int32_t script_1D0F8(void);
static int32_t script_1D1DE(void);
static void script_1D29A(void);
static void script_1D34B(int32_t arg0);
static void script_1D3B5(void);
static int32_t script_1D3F2(int32_t arg0);
static void script_1D484(void);
static void script_1D509(void);
static void script_1D53A(int32_t arg0);
static void script_1D573(void);
static void script_1D698(void);
static int32_t script_1D73E(void);
static int32_t script_1D7B7(void);
static void script_1D862(void);
static void script_1D865(void);
static int32_t script_1D868(void);
static int32_t script_1D86B(void);
static int32_t script_1D86E(void);
static int32_t script_1D871(void);
static int32_t script_1D874(void);
static int32_t script_1D877(void);
static int32_t script_1D87A(void);
static int32_t script_1D87D(void);
static int32_t script_1D880(void);
static int32_t script_1D883(void);
static int32_t script_1D886(void);
static int32_t script_1D889(void);
static int32_t script_1D88C(void);
static int32_t script_1D88F(void);
static int32_t script_1D892(void);
static int32_t script_1D895(void);
static int32_t script_1D898(void);
static void script_1D89B(void);
static int32_t script_1D8A3(void);
static int32_t script_1D8A6(void);
static int32_t script_1D8A9(void);
static void script_1D8AC(void);
static int32_t script_1D8AF(void);
static int32_t script_1D8B2(void);
static int32_t script_1D8B5(void);
static int32_t script_1D8B8(void);
static int32_t script_1D8BB(void);
static int32_t script_1D8BE(void);
static int32_t script_1D8C1(void);
static int32_t script_1D8C4(void);
static int32_t script_1D8C7(void);
static int32_t script_1D8CA(void);
static int32_t script_1D8CD(void);
static int32_t script_1D8D0(void);
static int32_t script_1D8D3(void);
static int32_t script_1D8D6(void);
static int32_t script_1D8D9(void);
static void script_1D8DC(void);
static void script_1D8E4(void);
static int32_t script_1D8EC(void);
static int32_t script_1D8EF(void);
static int32_t script_1D8F2(void);
static int32_t script_1D8F5(void);
static int32_t script_1D8F8(void);
static void script_1D8FB(void);
static void script_1D907(void);
static int32_t script_1D913(void);
static int32_t script_1D91F(void);
static int32_t script_1D92B(void);
static int32_t script_1D937(void);
static int32_t script_1D943(void);
static int32_t script_1D94F(void);
static int32_t script_1D95B(void);
static int32_t script_1D967(void);
static int32_t script_1D973(void);
static int32_t script_1D97F(void);
static int32_t script_1D98B(void);
static int32_t script_1D997(void);
static int32_t script_1D9A3(void);
static int32_t script_1D9AF(void);
static int32_t script_1D9BB(void);
static int32_t script_1D9C7(void);
static int32_t script_1D9D3(void);
static void script_1D9DF(void);
static int32_t script_1D9E2(void);
static int32_t script_1D9EE(void);
static int32_t script_1D9FA(void);
static void script_1DA06(void);
static int32_t script_1DA12(void);
static int32_t script_1DA1E(void);
static int32_t script_1DA2A(void);
static int32_t script_1DA36(void);
static int32_t script_1DA42(void);
static int32_t script_1DA4E(void);
static int32_t script_1DA5A(void);
static int32_t script_1DA66(void);
static int32_t script_1DA72(void);
static int32_t script_1DA7E(void);
static int32_t script_1DA8A(void);
static int32_t script_1DA96(void);
static int32_t script_1DAA2(void);
static int32_t script_1DAAE(void);
static int32_t script_1DABA(void);
static void script_1DAC6(void);
static void script_1DAC9(void);
static int32_t script_1DACC(void);
static int32_t script_1DAD8(void);
static int32_t script_1DAE4(void);
static int32_t script_1DAF0(void);
static int32_t script_1DAFC(void);
static void script_1DB08(void);
static int32_t script_1DC6A(int32_t arg0, int32_t arg1, int32_t arg2);
static int32_t script_1DDF6(int32_t arg0, int32_t arg1);
static int32_t script_1DEB6(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_1E07D(int32_t arg0, int32_t arg1);
static int32_t script_1E0A5(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_1E165(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_1E1AB(int32_t arg0, int32_t arg1, int32_t arg2);
static int32_t script_1E3D8(int32_t arg0, int32_t arg1, int32_t arg2);
static int32_t script_1E8CD(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4);
static void script_1E98F(int32_t arg0);
static void script_1EFE9(void);
static void script_1F06A(void);
static int32_t script_1F0C2(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5, int32_t arg6, int32_t arg7);
static int32_t script_1F6C3(int32_t arg0);
static int32_t script_1F71E(void);
static int32_t script_1F877(void);
static void script_1F91D(void);
static void script_1F976(void);
static int32_t script_1F9B0(int32_t arg0);
static void script_1F9D3(void);
static int32_t script_1F9F3(void);
static void script_1FA3E(void);
static void script_1FA85(void);
static void script_1FAD8(void);
static void script_1FB34(void);
static void script_1FB7B(void);
static void script_1FBD7(void);
static void script_1FC5F(void);
static void script_1FCD7(void);
static void script_1FD14(void);
static int32_t script_1FD51(int32_t arg0);
static void script_1FDC2(int32_t arg0);
static int32_t script_2005C(int32_t arg0);
static void script_200A4(void);
static void script_200AC(void);
static void script_200C0(int8_t arg0);
static int32_t script_20323(void);
static int8_t script_2045D(int8_t arg0);
static void script_20532(void);
static void script_205B5(int32_t arg0);
static void script_2098F(void);
static void script_209DA(void);
static void script_209F6(int32_t arg0, int32_t arg1);
static void script_20A62(void);
static void script_20B06(void);
static void script_20BE9(void);
static void script_2103D(void);
static void script_21588(void);
static void script_21992(void);
static void script_21EDD(void);
static void script_22015(int32_t arg0);
static void script_22021(int32_t arg0);
static void script_2202D(int32_t arg0);
static void script_22139(int32_t arg0);
static void script_22150(int32_t arg0);
static void script_22191(int32_t arg0);
static void script_221D5(int32_t arg0);
static void script_22253(void);
static void script_222D2(void);
void script_223FF(void);
static int32_t script_22929(int32_t arg0);
static int32_t script_22F34(void);
static void script_231E5(void);
static void script_23231(void);
static void script_2325E(void);
static void script_232AC(void);
static void script_23302(void);
static void script_2334B(void);
static void script_233AE(void);
static void script_237B7(int32_t arg0);
static void script_2382D(void);
static void script_23859(void);
static int32_t script_23876(void);
static void script_239F9(void);
static void script_24000(int32_t arg0);
static void script_240AB(int32_t arg0);
static void script_240C9(float arg0);
static void script_240E8(int32_t arg0);
static void script_240FD(int32_t arg0, int32_t arg1);
static int32_t script_241B3(int32_t arg0);
static void script_241CB(int32_t arg0);
static void script_241E1(int32_t arg0);
static void script_241EF(void);
static void script_242B9(void);
static void script_242BC(void);
static void script_2440A(int32_t arg0);
static int32_t script_2441E(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_24440(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_24486(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_2449A(int32_t arg0);
static void script_244B4(void);
static int32_t script_244CF(int32_t arg0);
static void script_244FC(int32_t arg0, int32_t arg1);
static void script_24523(void);
static void script_2452D(void);
static void script_24548(void);
static void script_24552(int32_t arg0);
static int32_t script_245B6(float arg0, float arg1, float arg2, float arg3);
static int32_t script_245DD(int32_t arg0, int32_t arg1);
static int32_t script_24655(int32_t arg0, int32_t arg1, int32_t arg2, const Vector3 *arg3);
static void script_24701(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_248C7(int32_t arg0);
static void script_248E9(int32_t arg0, int32_t arg1, int32_t arg2);
static int32_t script_249C4(void);
static void script_249E8(int32_t arg0);
static int32_t script_24AA8(int32_t arg0, int32_t arg1);
static int32_t script_24AAE(int32_t arg0);
static int32_t script_24AB9(int32_t arg0);
static int32_t script_24B94(int32_t arg0, int32_t arg1);
static void script_24BF9(int32_t arg0, int32_t arg1);
static void script_24C5E(void);
static void script_24C9A(void);
static void script_24CB5(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_24D29(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_24D4E(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_24D97(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_24DFD(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_24EC5(int32_t arg0, int32_t arg1, int32_t arg2);
static int32_t script_24F1B(void);
static void script_24F70(int32_t arg0);
static void script_24FCA(const char *arg0, const char *arg1);

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
static int8_t task_234;
static uint8_t task_235;
static uint16_t task_236;
static int32_t task_238;
static int32_t task_23C;
static int8_t task_240;
static uint8_t task_241;
static uint16_t task_242;
static int8_t task_244;
static uint8_t task_245;
static uint16_t task_246;
static int32_t task_248;
static Vector3 task_24C;
static Vector3i task_258;
static int32_t task_264;
static int32_t task_268;
static int32_t task_26C;
static int32_t task_270;
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
static int8_t task_285;
static int8_t task_286;
static int8_t task_287;
static int8_t task_288;
static uint8_t task_289;
static uint16_t task_28A;
static int8_t task_28C;
static uint8_t task_28D;
static uint16_t task_28E;
static Vector3 task_290;
static Vector3 task_29C;
static int8_t task_2A8;
static uint8_t task_2A9;
static uint16_t task_2AA;
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
static int32_t task_318;
static int32_t task_31C;
static int8_t task_320;
static uint8_t task_321;
static int16_t task_322;
static int8_t task_324;
static uint8_t task_325;
static uint16_t task_326;
static int32_t task_328;
static int32_t task_32C;
static int32_t task_330;
static int32_t task_334;
static int32_t task_338;
static int32_t task_33C;
static int32_t task_340;
static int32_t task_344;
static int8_t task_348;
static uint8_t task_349;
static uint16_t task_34A;
static int8_t task_34C;
static uint8_t task_34D;
static uint16_t task_34E;
static int32_t task_350;
static int32_t task_354;
static int32_t task_358;
static int32_t task_35C[3];
static int8_t task_368;
static uint8_t task_369;
static uint16_t task_36A;
static int32_t task_36C;
static int32_t task_370;
static int32_t task_374;
static int32_t task_378;
static int32_t task_37C;
static int32_t task_380;
static int32_t task_384;
static int32_t task_388;
static int32_t task_38C;
static int32_t task_390;
static int32_t task_394;
static int32_t task_398;
static int32_t task_39C;
static int32_t task_3A0;
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
static int8_t task_3DC;
static int8_t task_3DD;
static int8_t task_3DE;
static int8_t task_3DF;
static int8_t task_3E0;
static uint8_t task_3E1;
static uint16_t task_3E2;
static int32_t task_3E4;
static int32_t task_3E8;
static int8_t task_3EC[100][12];
static int8_t task_89C[12];
static int8_t task_8A8[10];
static int8_t task_8B2;
static int8_t task_8B3;
static int8_t task_8B4;
static uint8_t task_8B5;
static uint16_t task_8B6;
static int32_t task_8B8;
static int32_t task_8BC;
static int8_t task_8C0;
static uint8_t task_8C1;
static uint16_t task_8C2;
static int32_t task_8C4[10];
static int32_t task_8EC;
static int32_t task_8F0;
static const Vector3 data_2501C = {.x = 1.0f, .y = 1.0f, .z = 1.0f};
static const int32_t data_25028[4] = {16777215, 16777215, 16777215, 16777215};
static const float data_25038[3] = {20.0f, 800.0f, 1.0f};
static const int32_t data_25044 = -1;
static const char data_25048[10] __attribute__((scn_string_literal)) = "Kaidan : ";
static const char data_25052[32] __attribute__((scn_string_literal)) = "========== initializing kaidan\012";
static const char data_25072[48] __attribute__((scn_string_literal)) = "please ENDOFKDATA use can't start shuki kaidan\012";
static const char data_250A2[] __attribute__((scn_string_literal)) = "Kaidan Function Error 1\n";
static const char data_250BB[] __attribute__((scn_string_literal)) = "Kaidan Function Error 2\n";
static const char data_250D4[] __attribute__((scn_string_literal)) = "Kaidan Data Error\n";
static const char data_250E7[10] __attribute__((scn_string_literal)) = "Kaidan : ";
static const char data_250F1[28] __attribute__((scn_string_literal)) = "wait ... shuki shuki shuki\012";
static const char data_2510D[10] __attribute__((scn_string_literal)) = "Kaidan : ";
static const char data_25117[24] __attribute__((scn_string_literal)) = "KAIDAN : Warning coli!\012";
static const char data_2512F[24] __attribute__((scn_string_literal)) = "KAIDAN : Warning coli!\012";
static const char data_25147[24] __attribute__((scn_string_literal)) = "KAIDAN : Warning coli!\012";
static const char data_2515F[24] __attribute__((scn_string_literal)) = "KAIDAN : Warning coli!\012";
static const char data_25177[24] __attribute__((scn_string_literal)) = "KAIDAN : Warning coli!\012";
static const char data_2518F[24] __attribute__((scn_string_literal)) = "KAIDAN : Warning setY!\012";
static const char data_251A7[24] __attribute__((scn_string_literal)) = "KAIDAN : Warning setY!\012";
static const char data_251BF[13] __attribute__((scn_string_literal)) = "Kaidan : ";
static DoorMotionSelectorBlock data_251CC[38] = {
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
static ScnDoorMotionDescriptor data_257BC[72] = {
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
static uint32_t data_261DC = 0xFFFFFFFF;
typedef struct Row_261E0 { float w0; float w1; float w2; float w3; int32_t w4; } Row_261E0;
static const Row_261E0 data_261E0[7] = {
  {0.0f, 0.0f, 0.0f, 0.0f, -1},
  {0.0f, 0.0f, 0.0f, 0.0f, -1},
  {0.0f, 0.0f, 0.0f, 0.0f, 1534},
  {26.0f, 55.0f, 45.0f, 75.0f, 1555},
  {61.0f, 146.0f, 45.0f, 50.0f, 1554},
  {59.0f, 124.0f, 45.0f, 70.0f, 1543},
  {33.0f, 49.0f, 45.0f, 50.0f, 1543}
};
static const float data_2626C[4] = {33.0f, 49.0f, 50.0f, 50.0f};
static void (*data_2627C)(int32_t, int32_t, int32_t) = script_3FB4;
static void (*const data_26280)(int32_t, int32_t, int32_t, int32_t) = script_3FC4;
static void (*const data_26284)(int32_t, int32_t, int32_t, int32_t) = script_3FC4;
static void (*const data_26288)(int32_t, int32_t, int32_t, int32_t) = script_3FC4;
static void (*const data_2628C)(int32_t, int32_t, int32_t, int32_t) = script_3FC4;
static void (*const data_26290)(int32_t, int32_t, int32_t, int32_t) = script_3FC4;
static void (*const data_26294)(int32_t, int32_t, int32_t, int32_t) = script_3FC4;
static void (*const data_26298)(int32_t, int32_t, int32_t, int32_t) = script_3FC4;
static void (*const data_2629C)(int32_t, int32_t, int32_t, int32_t) = script_3FC4;
static void (*const data_262A0)(int32_t, int32_t, int32_t, int32_t) = script_426A;
static void (*const data_262A4)(int32_t, int32_t, int32_t, int32_t) = script_43CB;
static void (*const data_262A8)(int32_t, int32_t, int32_t, int32_t) = script_4898;
static void (*const data_262AC)(int32_t, int32_t, int32_t, int32_t) = script_48FF;
static void (*const data_262B0)(int32_t, int32_t, int32_t, int32_t) = script_40C8;
static void (*const data_262B4)(int32_t, int32_t, int32_t, int32_t) = script_40C8;
static void (*const data_262B8)(int32_t, int32_t, int32_t, int32_t) = script_452C;
static void (*const data_262BC)(int32_t, int32_t, int32_t, int32_t) = script_47CA;
static void (*const data_262C0)(int32_t, int32_t, int32_t, int32_t) = script_4966;
static void (*const data_262C4)(int32_t, int32_t, int32_t, int32_t) = script_426A;
static void (*const data_262C8)(int32_t, int32_t, int32_t, int32_t) = script_43CB;
static void (*const data_262CC)(int32_t, int32_t, int32_t, int32_t) = script_452C;
static void (*const data_262D0)(int32_t, int32_t, int32_t, int32_t) = script_426A;
static void (*const data_262D4)(int32_t, int32_t, int32_t, int32_t) = script_43CB;
static void (*const data_262D8)(int32_t, int32_t, int32_t, int32_t) = script_452C;
static void (*const data_262DC)(int32_t, int32_t, int32_t, int32_t) = script_49AD;
static void (*const data_262E0)(int32_t, int32_t, int32_t, int32_t) = script_49AD;
static void (*const data_262E4)(int32_t, int32_t, int32_t, int32_t) = script_49AD;
static void (*const data_262E8)(int32_t, int32_t, int32_t, int32_t) = script_49AD;
static void (*const data_262EC)(int32_t, int32_t, int32_t, int32_t) = script_4AE3;
static void (*const data_262F0)(int32_t, int32_t, int32_t, int32_t) = script_53A4;
static void (*const data_262F4)(int32_t, int32_t, int32_t, int32_t) = script_3FC4;
static void (*const data_262F8)(int32_t, int32_t, int32_t, int32_t) = script_3FC4;
static void (*const data_262FC)(int32_t, int32_t, int32_t, int32_t) = script_3FC4;
static void (*const data_26300)(int32_t, int32_t, int32_t, int32_t) = script_3FC4;
static void (*const data_26304)(int32_t, int32_t, int32_t, int32_t) = script_3FC4;
static void (*const data_26308)(int32_t, int32_t, int32_t, int32_t) = script_3FC4;
static void (*const data_2630C)(int32_t, int32_t, int32_t, int32_t) = script_3FC4;
static void (*const data_26310)(int32_t, int32_t, int32_t, int32_t) = script_3FC4;
static const char data_26314[46] __attribute__((scn_string_literal)) = "Door Index = %d : call_type = %d : type = %d\012";
static const char data_26342[43] __attribute__((scn_string_literal)) = "Door Open Motion = DEB_AKESIME_FUSUMA_L_F\012";
static const char data_2636D[43] __attribute__((scn_string_literal)) = "Door Open Motion = DEB_AKESIME_FUSUMA_R_F\012";
static const char data_26398[42] __attribute__((scn_string_literal)) = "Door Open Motion = DEB_HIKISIME_DOOR_L_F\012";
static const char data_263C2[42] __attribute__((scn_string_literal)) = "Door Open Motion = DEB_HIKISIME_DOOR_R_F\012";
static const char data_263EC[41] __attribute__((scn_string_literal)) = "Door Open Motion = DEB_OSISIME_DOOR_L_F\012";
static const char data_26415[41] __attribute__((scn_string_literal)) = "Door Open Motion = DEB_OSISIME_DOOR_R_F\012";
static const char data_2643E[43] __attribute__((scn_string_literal)) = "Door Open Motion = GAK_AKESIME_FUSUMA_L_F\012";
static const char data_26469[43] __attribute__((scn_string_literal)) = "Door Open Motion = GAK_AKESIME_FUSUMA_R_F\012";
static const char data_26494[42] __attribute__((scn_string_literal)) = "Door Open Motion = GAK_HIKISIME_DOOR_L_F\012";
static const char data_264BE[42] __attribute__((scn_string_literal)) = "Door Open Motion = GAK_HIKISIME_DOOR_R_F\012";
static const char data_264E8[41] __attribute__((scn_string_literal)) = "Door Open Motion = GAK_OSISIME_DOOR_L_F\012";
static const char data_26511[41] __attribute__((scn_string_literal)) = "Door Open Motion = GAK_OSISIME_DOOR_R_F\012";
static const char data_2653A[43] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_AKESIME_FUSUMA_L_F\012";
static const char data_26565[43] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_AKESIME_FUSUMA_R_F\012";
static const char data_26590[42] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_HIKISIME_DOOR_L_F\012";
static const char data_265BA[42] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_HIKISIME_DOOR_R_F\012";
static const char data_265E4[41] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_OSISIME_DOOR_L_F\012";
static const char data_2660D[41] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_OSISIME_DOOR_R_F\012";
static const char data_26636[43] __attribute__((scn_string_literal)) = "Door Open Motion = KOD_AKESIME_FUSUMA_L_F\012";
static const char data_26661[43] __attribute__((scn_string_literal)) = "Door Open Motion = KOD_AKESIME_FUSUMA_R_F\012";
static const char data_2668C[42] __attribute__((scn_string_literal)) = "Door Open Motion = KOD_HIKISIME_DOOR_L_F\012";
static const char data_266B6[42] __attribute__((scn_string_literal)) = "Door Open Motion = KOD_HIKISIME_DOOR_R_F\012";
static const char data_266E0[41] __attribute__((scn_string_literal)) = "Door Open Motion = KOD_OSISIME_DOOR_L_F\012";
static const char data_26709[41] __attribute__((scn_string_literal)) = "Door Open Motion = KOD_OSISIME_DOOR_R_F\012";
static const char data_26732[43] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_AKESIME_FUSUMA_L_F\012";
static const char data_2675D[43] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_AKESIME_FUSUMA_R_F\012";
static const char data_26788[42] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_HIKISIME_DOOR_L_F\012";
static const char data_267B2[42] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_HIKISIME_DOOR_R_F\012";
static const char data_267DC[41] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_OSISIME_DOOR_L_F\012";
static const char data_26805[41] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_OSISIME_DOOR_R_F\012";
static const char data_2682E[43] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_AKESIME_FUSUMA_L_F\012";
static const char data_26859[43] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_AKESIME_FUSUMA_R_F\012";
static const char data_26884[42] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_HIKISIME_DOOR_L_F\012";
static const char data_268AE[42] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_HIKISIME_DOOR_R_F\012";
static const char data_268D8[41] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_OSISIME_DOOR_L_F\012";
static const char data_26901[41] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_OSISIME_DOOR_R_F\012";
static const char data_2692A[43] __attribute__((scn_string_literal)) = "Door Open Motion = SIN_AKESIME_FUSUMA_L_F\012";
static const char data_26955[43] __attribute__((scn_string_literal)) = "Door Open Motion = SIN_AKESIME_FUSUMA_R_F\012";
static const char data_26980[42] __attribute__((scn_string_literal)) = "Door Open Motion = SIN_HIKISIME_DOOR_L_F\012";
static const char data_269AA[42] __attribute__((scn_string_literal)) = "Door Open Motion = SIN_HIKISIME_DOOR_R_F\012";
static const char data_269D4[41] __attribute__((scn_string_literal)) = "Door Open Motion = SIN_OSISIME_DOOR_L_F\012";
static const char data_269FD[41] __attribute__((scn_string_literal)) = "Door Open Motion = SIN_OSISIME_DOOR_R_F\012";
static const char data_26A26[43] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_AKESIME_FUSUMA_L_F\012";
static const char data_26A51[43] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_AKESIME_FUSUMA_R_F\012";
static const char data_26A7C[42] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_HIKISIME_DOOR_L_F\012";
static const char data_26AA6[42] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_HIKISIME_DOOR_R_F\012";
static const char data_26AD0[41] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_OSISIME_DOOR_L_F\012";
static const char data_26AF9[41] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_OSISIME_DOOR_R_F\012";
static const char data_26B22[43] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_AKESIME_FUSUMA_L_F\012";
static const char data_26B4D[43] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_AKESIME_FUSUMA_R_F\012";
static const char data_26B78[42] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_HIKISIME_DOOR_L_F\012";
static const char data_26BA2[42] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_HIKISIME_DOOR_R_F\012";
static const char data_26BCC[41] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_OSISIME_DOOR_L_F\012";
static const char data_26BF5[41] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_OSISIME_DOOR_R_F\012";
static const char data_26C1E[44] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_OSISIME_DOOR_AP_L_F\012";
static const char data_26C4A[44] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_OSISIME_DOOR_AP_L_F\012";
static const char data_26C76[44] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_OSISIME_DOOR_AP_L_F\012";
static const char data_26CA2[44] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_OSISIME_DOOR_AP_L_F\012";
static const char data_26CCE[44] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_OSISIME_DOOR_AP_L_F\012";
static const char data_26CFA[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_26D16[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_26D32[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_26D4E[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_26D6A[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_26D86[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_26DA2[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_26DBE[13] __attribute__((scn_string_literal)) = "open-move!!\012";
static const char data_26DCB[11] __attribute__((scn_string_literal)) = "M_MDOR.BIN";
static const char data_26DD6[] __attribute__((scn_string_literal)) = "This Charactor Work is NULL!!\n";
static const char data_26DF5[22] __attribute__((scn_string_literal)) = "手動ドア開け番号指定\012";
static const char data_26E0B[34] __attribute__((scn_string_literal)) = "無限ループになりそうなので抜ける\012";
static const char data_26E2D[] __attribute__((scn_string_literal)) = "Don't make BOX Buffer!!\n";
static const char data_26E46[] __attribute__((scn_string_literal)) = "Don't make BOX Buffer!!\n";
static const char data_26E5F[15] __attribute__((scn_string_literal)) = "Door System : ";
static const char data_26E6E[15] __attribute__((scn_string_literal)) = "Door System : ";
static const char data_26E7D[18] __attribute__((scn_string_literal)) = "Door Box is ON!!\012";
static const char data_26E8F[19] __attribute__((scn_string_literal)) = "Door Box is OFF!!\012";
static const char data_26EA2[18] __attribute__((scn_string_literal)) = "Door Box is ON!!\012";
static const char data_26EB4[20] __attribute__((scn_string_literal)) = "Door Box is OFF!!\012";
static const DSWKDoorEntry data_26EC8[2] = {
  {.slot = 0, .flags = 0, .ctrl_mask_pair = 0x640064U, .coarse_metric = 0.0f, .selection_metric = 0.0f, .view_ang_delta = 0, .group = 0, .global1 = 0xFFFFFFFFU, .global2 = 0xFFFFFFFFU, .global3 = 0xFFFFFFFFU, .sndCommandId = 0xFFFFFFFFU, .global5 = 0xFFFFFFFFU, .global6 = 0xFFFFFFFFU},
  {.slot = 2, .flags = 0, .ctrl_mask_pair = 0x640064U, .coarse_metric = 0.0f, .selection_metric = 0.0f, .view_ang_delta = 0, .group = 0, .global1 = 0xFFFFFFFFU, .global2 = 0xFFFFFFFFU, .global3 = 0xFFFFFFFFU, .sndCommandId = 0xFFFFFFFFU, .global5 = 0xFFFFFFFFU, .global6 = 0xFFFFFFFFU}
};
static const int32_t data_26F30[13] = {-1, 0, 6553700, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1};
static const uint8_t data_26F64[4] = {0};
static const int32_t data_26F68[3] = {-1, 1, -1};
static char data_26F74[] = "DR29_000";
static char data_26F7D[] = "GNKS2STG";
static const uint8_t data_26F86[2] = {0};
static char *const data_26F88 = data_26F74;
static const int32_t data_26F8C[3] = {29, 787115, 852651};
static char *const data_26F98 = data_26F7D;
static const int32_t data_26F9C[3] = {15, 524971, 590507};
static const MapObjectRecord data_26FA8[2] = {
  {.kind = 2, .resourceIndex = 0, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-1.0f, 0.21f, 10.9f}, .yawFixed16 = 0},
  {.kind = 2, .resourceIndex = 1, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-3.24f, 0.1f, 0.02f}, .yawFixed16 = 0}
};
static const uint32_t data_26FF0[4] = {0x413042CU, 0x42F042DU, 0x4300432U, 0x3870431U};
static const char data_27000[20] __attribute__((scn_string_literal)) = "TUNED FreeTalk def\012";
static const char data_27014[248] __attribute__((scn_string_literal)) = "TUNED FreeTalk def\012";
static const int32_t data_2710C[171] = {
  0, 0, -10922, 0, 0, -10558, 0, 1456, 0, 0, 0, -18022, 0, 0, -17112, 0, 0, -18386, 0, 0, 0, 0, 0, -12196, 0, 0, -18204, 0, 0, -16748, 0, 0, 0, 0, 0,
  -11832, 0, 0, -18386, 0, 0, -15109, 0, 0, 0, 0, 910, -12014, 0, -2184, -16748, 0, -2184, -10012, 0, 0, 0, 0, -364, -11650, -2912, -364, -4369, 1638,
  910, 182, 0, 0, -16019, 0, 182, -23119, 0, 546, -18932, 0, 0, -1092, 0, 0, -16384, 0, 0, -21845, 0, 0, -18750, 0, 0, -546, 0, 0, -21481, 0, 0,
  -21299, 0, 0, -18568, 0, 0, -728, 0, 910, -18750, 0, 182, -18750, 0, -4004, -18204, 0, 0, -728, 0, -364, -14017, -2912, -364, -6735, 1638, 910,
  2548, 0, 0, -18568, 0, 182, -25668, 0, 546, -21481, 0, 0, -3640, 0, 0, -18932, 0, 0, -24393, 0, 0, -21299, 0, 0, -3094, 0, 0, -24029, 0, 0, -23847,
  0, 0, -21117, 0, 0, -3276, 0, 910, -21299, 0, 182, -21299, 0, -4004, -20753, 0, 0, -3276
};
static const int32_t data_273B8[57] = {
  0, 0, -5461, 0, -2366, -1092, 0, -182, 1092, 0, -546, -6735, 0, 0, -4004, 0, 0, -2912, 0, -182, 1274, 0, 0, -13289, 0, 0, -5279, 0, 0, -8009, 0, 0,
  1456, 0, 182, -8738, 0, 0, -6007, 0, 0, -8738, 0, 182, 1638, 0, -728, -5279, 0, -1274, -6735, 0, -910, -8738, 0, 0, 1638
};
static const int32_t data_2749C[114] = {
  0, 0, -5461, 0, -2366, -1092, 0, -182, 1092, 0, -546, -13289, 0, 0, -5643, 0, 0, -8009, 0, -182, 1274, 0, 0, -11832, 0, 0, -5279, 0, 0, -8009, 0, 0,
  1456, 0, 182, -8738, 0, 0, -6007, 0, 0, -8738, 0, 182, 1638, 0, -728, -5279, 0, -1274, -6735, 0, -910, -8738, 0, 0, 1092, 0, 0, -5461, 0, -2366,
  -1092, 0, -182, 1092, 0, -546, -3640, 0, 0, -2366, 0, 0, -1456, 0, -182, 1274, 0, 0, -3822, 0, 0, -2730, 0, 0, -1456, 0, 0, 1456, 0, 182, -3822, 0,
  0, -2912, 0, 0, -1638, 0, 182, 1638, 0, -728, -3640, 0, -910, -2730, 0, -546, -1638, 0, 0, 1092
};
static const int32_t data_27664[114] = {
  0, 0, -5461, 0, -2366, -1092, 0, -182, 1092, 0, -546, -6735, 0, 0, -4004, 0, 0, -6553, 0, -182, 1274, 0, 0, -13289, 0, 0, -5279, 0, 0, -8009, 0, 0,
  1456, 0, 182, -8738, 0, 0, -6007, 0, 0, -8738, 0, 182, 1638, 0, -728, -5279, 0, -1274, -6735, 0, -910, -8738, 0, 0, 1638, 0, 0, -9102, 0, -2366,
  -2002, 0, -182, 910, 0, -546, -2366, 0, 0, -2002, 0, 0, -2912, 0, -182, 1274, 0, 0, -14199, 0, 0, -11468, 0, 0, -13653, 0, 0, 1456, 0, 182, -11286,
  0, 0, -12014, 0, 0, -12743, 0, 182, 1638, 0, -728, -10194, 0, -1274, -12196, 0, -910, -10922, -728, 546, 1638
};
static const int32_t data_2782C[228] = {
  0, 0, -1820, 0, -2366, -728, 0, -364, -1092, 0, -546, -11832, 0, 910, -6735, 0, 0, -6553, 0, -182, 1274, 0, 0, -13289, 0, 0, -6007, 0, 0, -10012, 0,
  0, 1092, 0, 182, -8738, 0, 0, -6735, 0, 0, -9830, 0, 182, 1638, 0, -728, -6553, 0, -1274, -7281, 0, -910, -10740, 0, 0, 1638, 0, 0, -5461, -2912,
  -2366, -364, -5279, 2548, -910, 0, -546, -8374, 0, 0, -9466, 0, 910, -4915, 0, -182, 546, 0, 0, -13289, 0, 0, -5279, 0, 364, -7645, 0, -182, 1456,
  0, 182, -8738, 0, 0, -7281, 0, 182, -6917, 0, -182, 1638, 0, -728, -5279, 0, -1274, -6735, 0, -182, -8009, 0, 0, 1638, -4733, 0, -8556, 0, -2366,
  -1092, -6189, 2912, 0, 0, -546, -10922, 0, 1820, -16384, 0, 0, -11468, 0, 182, 546, 0, 0, -10376, 0, 0, -15837, 0, 0, -12743, 0, -182, 728, 0, 182,
  -13835, 0, 0, -16201, 0, 364, -10922, 0, -182, 728, 0, -728, -9648, 0, -3458, -12379, 0, -728, -7463, 0, 0, 1092, 0, 0, -5461, 0, -2366, -1638, 0,
  -910, 1092, 0, -546, -3640, 0, 0, -2366, 0, 0, -1456, 0, -182, 1638, 0, 0, -910, 0, 0, -2730, 0, 0, -1274, 0, 0, 1820, 0, 182, -2730, 0, 0, -1820,
  0, 0, -1274, 0, 182, 2184, 0, -728, -3094, 0, -910, -2548, 0, 910, -1456, -546, 910, 2366
};
static const int32_t data_27BBC[57] = {
  0, 0, -6005, 0, 0, -909, 0, 0, 0, 0, 0, -6368, 0, 1820, -8552, 0, 0, -4548, 0, 0, 0, 0, 0, -8552, 0, 0, -10372, 0, -545, -4002, 0, 0, 0, 0, 0,
  -7823, 0, 0, -12374, 0, -909, -2182, 0, 0, 0, 0, -1636, -10735, 0, -2001, -10372, 0, -1819, -727, 0, 0, 0
};
static const int32_t data_27CA0[57] = {
  0, 0, -7827, 0, -910, -4004, 0, -182, 546, 0, -546, -2730, 0, 0, -4551, 0, 0, -4369, 0, -182, 1820, 0, 0, -5825, 0, 0, -5825, 0, 0, -3458, 0, 0,
  1456, 0, 182, -7099, 0, 182, -5825, 0, 0, -4369, 0, 182, 1638, 0, -728, -6553, 0, -910, -7463, 0, -546, -2730, 0, 0, 1092
};
static const Vector4 data_27D84 = {.x = 1.0f, .y = 0.08f, .z = 0.08f, .w = 0.1f};
static const float data_27D94[3] = {0.1f, 360.0f, 0.92f};
static const int32_t data_27DA0 = -16777160;
static const char data_27DA4[] __attribute__((scn_string_literal)) = "scroll";
static const char data_27DAB[13] __attribute__((scn_string_literal)) = "SCROLL25.SPR";
static const Vector4 data_27DB8 = {.x = 0.3f, .y = 0.3f, .z = 0.3f, .w = 0.3f};
static const float data_27DC8[4] = {1.0f, 1.0f, 1.0f, 1.0f};
static const Vector4 data_27DD8 = {.x = 0.3f, .y = 0.3f, .z = 0.3f, .w = 0.3f};
static const Vector4 data_27DE8 = {.x = 1.0f, .y = 1.0f, .z = 1.0f, .w = 1.0f};
static const Vector4 data_27DF8 = {.x = 0.4f, .y = 0.4f, .z = 0.4f, .w = 0.4f};
static const Vector4 data_27E08 = {.x = 0.3f, .y = 0.3f, .z = 0.3f, .w = 0.3f};
static const uint16_t data_27E18[7] = {670, 672, 674, 677, 684, 682, 680};
static const char data_27E26[24] __attribute__((scn_string_literal)) = "ふくちゃん読み込み失敗\012";
static const char data_27E3E[22] __attribute__((scn_string_literal)) = "いねさん読み込み失敗\012";
static const Vector3 data_27E54 = {.x = -11.2439f, .y = 2.9721f, .z = -22.947001f};
static const Vector3 data_27E60 = {.x = -11.242f, .y = 2.9721f, .z = -21.937f};
typedef struct Row_27E6C { float w0; float w1; float w2; int32_t w3; int32_t w4; int32_t w5; } Row_27E6C;
static const Row_27E6C data_27E6C[2] = {
  {-10.8f, 1.13f, -23.03f, 0, 16384, 0},
  {-10.5499f, 1.397f, -22.75f, 49152, 32768, 49152}
};
static const float data_27E9C[3] = {-10.5299f, 1.269f, -22.6462f};
static const Vector3 data_27EA8 = {.x = -4.0f, .y = 0.0f, .z = -27.0f};
static const Vector3 data_27EB4 = {.x = -9.4f, .y = 0.0f, .z = -18.2f};
static const uint32_t data_27EC0 = 3;
static const float data_27EC4[3] = {-6.7f, -22.5f, 1.0f};
static const int32_t data_27ED0 = 196608;
static const uint32_t data_27ED4 = 131072;
typedef struct Row_27ED8 { shen_name w0; uint32_t w1; int32_t w2; float w3; float w4; int32_t w5; int32_t w6; int32_t w7; } Row_27ED8;
static const Row_27ED8 data_27ED8[7] = {
  {SHEN_NAME('KHKS'), 0x80004000U, 0, 0.0f, 0.0f, 0, 0, 131072},
  {SHEN_NAME('GART'), 0x80008000U, 0, -0.3f, 0.4f, 0, 0, 131072},
  {SHEN_NAME('FIR1'), 0x0U, 0, 0.0f, 0.0f, 0, 0, 131072},
  {SHEN_NAME('FIR2'), 0x0U, 0, 0.0f, 0.0f, 0, 0, 131072},
  {SHEN_NAME('FIR3'), 0x0U, 0, 0.0f, 0.0f, 0, 0, 131072},
  {SHEN_NAME('FIR4'), 0x0U, 0, 0.0f, 0.0f, 0, 0, 131072},
  {SHEN_NAME('HAKO'), 0x0U, 0, 0.4f, 0.0f, 0, 0, 65535}
};
static const uint32_t data_27FB8 = 131072;
typedef struct Row_27FBC { shen_name w0; uint32_t w1; int32_t w2; float w3; float w4; int32_t w5; int32_t w6; int32_t w7; } Row_27FBC;
static const Row_27FBC data_27FBC[13] = {
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
static const uint32_t data_2815C = 131072;
static const shen_name data_28160 = SHEN_NAME('KHKS');
static const uint32_t data_28164[6] = {0x80004000U, 0, 0, 0, 0, 0};
static const int32_t data_2817C = 131072;
static const shen_name data_28180 = SHEN_NAME('GART');
static const uint32_t data_28184[2] = {0x80008000U, 0};
static const float data_2818C[4] = {-0.3f, 0.4f, 0.0f, 0.0f};
static const int32_t data_2819C = 131072;
static const shen_name data_281A0[7] = {SHEN_NAME('FIR1'), 0, 0, 0, 0, 0, 0};
static const int32_t data_281BC = 131072;
static const shen_name data_281C0[7] = {SHEN_NAME('FIR2'), 0, 0, 0, 0, 0, 0};
static const int32_t data_281DC = 131072;
static const shen_name data_281E0[7] = {SHEN_NAME('FIR3'), 0, 0, 0, 0, 0, 0};
static const int32_t data_281FC = 131072;
static const shen_name data_28200[7] = {SHEN_NAME('FIR4'), 0, 0, 0, 0, 0, 0};
static const int32_t data_2821C = 131072;
static const shen_name data_28220[3] = {SHEN_NAME('HAKO'), 0, 0};
static const float data_2822C[5] = {0.4f, 0.0f, 0.0f, 0.0f, 0.0f};
static const shen_name data_28240 = SHEN_NAME('KAK2');
static const uint32_t data_28244[2] = {0x80004000U, 0};
static const float data_2824C[4] = {-0.7f, 0.0f, 0.0f, 0.0f};
static const int32_t data_2825C[2] = {-65536, 10};
static const uint32_t data_28264 = 0x80004000U;
static const float data_28268[6] = {-11.299f, 2.3499f, -22.951f, 0.0f, 0.0f, 0.0f};
static const shen_name data_28280 = SHEN_NAME('KAK2');
static const uint32_t data_28284[7] = {0x80004000U, 0, 0, 0, 0, 0, 0};
static const shen_name data_282A0 = SHEN_NAME('KAK2');
static const uint32_t data_282A4[2] = {0x80004000U, 0};
static const float data_282AC[4] = {-1.2f, 0.0f, 0.0f, 0.0f};
static const int32_t data_282BC = 65535;
static const uint32_t data_282C0 = 131072;
static const shen_name data_282C4 = SHEN_NAME('KHKS');
static const uint32_t data_282C8[6] = {0x80004000U, 0, 0, 0, 0, 0};
static const int32_t data_282E0 = 131072;
static const shen_name data_282E4 = SHEN_NAME('GART');
static const uint32_t data_282E8[2] = {0x80008000U, 0};
static const float data_282F0[4] = {-0.3f, 0.4f, 0.0f, 0.0f};
static const int32_t data_28300 = 131072;
static const shen_name data_28304[7] = {SHEN_NAME('FIR1'), 0, 0, 0, 0, 0, 0};
static const int32_t data_28320 = 131072;
static const shen_name data_28324[7] = {SHEN_NAME('FIR2'), 0, 0, 0, 0, 0, 0};
static const int32_t data_28340 = 131072;
static const shen_name data_28344[7] = {SHEN_NAME('FIR3'), 0, 0, 0, 0, 0, 0};
static const int32_t data_28360 = 131072;
static const shen_name data_28364[7] = {SHEN_NAME('FIR4'), 0, 0, 0, 0, 0, 0};
static const int32_t data_28380 = 131072;
static const shen_name data_28384[3] = {SHEN_NAME('HAKO'), 0, 0};
static const float data_28390[4] = {0.4f, 0.0f, 0.0f, 0.0f};
static const int32_t data_283A0[2] = {-65536, 11};
static const uint32_t data_283A8 = 0x80004000U;
static const float data_283AC[6] = {-11.299f, 2.3499f, -21.94f, 0.0f, 0.0f, 0.0f};
static const shen_name data_283C4 = SHEN_NAME('KAK1');
static const uint32_t data_283C8[2] = {0x80004000U, 0};
static const float data_283D0[5] = {-0.7f, 0.0f, 0.0f, 0.0f, 0.0f};
static const shen_name data_283E4 = SHEN_NAME('KAK1');
static const uint32_t data_283E8[7] = {0x80004000U, 0, 0, 0, 0, 0, 0};
static const shen_name data_28404 = SHEN_NAME('KAK1');
static const uint32_t data_28408[2] = {0x80004000U, 0};
static const float data_28410[4] = {-1.2f, 0.0f, 0.0f, 0.0f};
static const int32_t data_28420 = 65535;
static const uint32_t data_28424 = 131072;
static const shen_name data_28428 = SHEN_NAME('KHKS');
static const uint32_t data_2842C[6] = {0x80004000U, 0, 0, 0, 0, 0};
static const int32_t data_28444 = 131072;
static const shen_name data_28448 = SHEN_NAME('GART');
static const uint32_t data_2844C[2] = {0x80008000U, 0};
static const float data_28454[4] = {-0.3f, 0.4f, 0.0f, 0.0f};
static const int32_t data_28464 = 131072;
static const shen_name data_28468[7] = {SHEN_NAME('FIR1'), 0, 0, 0, 0, 0, 0};
static const int32_t data_28484 = 131072;
static const shen_name data_28488[7] = {SHEN_NAME('FIR2'), 0, 0, 0, 0, 0, 0};
static const int32_t data_284A4 = 131072;
static const shen_name data_284A8[7] = {SHEN_NAME('FIR3'), 0, 0, 0, 0, 0, 0};
static const int32_t data_284C4 = 131072;
static const shen_name data_284C8[7] = {SHEN_NAME('FIR4'), 0, 0, 0, 0, 0, 0};
static const int32_t data_284E4 = 131072;
static const shen_name data_284E8[3] = {SHEN_NAME('HAKO'), 0, 0};
static const float data_284F4[4] = {0.4f, 0.0f, 0.0f, 0.0f};
static const int32_t data_28504[2] = {-65536, 11};
static const uint32_t data_2850C = 0x80004000U;
static const float data_28510[5] = {-11.299f, 2.3499f, -21.94f, 0.0f, 0.0f};
static const int32_t data_28524[2] = {-65536, 10};
static const uint32_t data_2852C = 0x80004000U;
static const float data_28530[5] = {-11.299f, 2.3499f, -22.951f, 0.0f, 0.0f};
static const int32_t data_28544 = 65535;
static const uint32_t data_28548 = 131072;
typedef struct Row_2854C { shen_name w0; uint32_t w1; int32_t w2; float w3; float w4; int32_t w5; int32_t w6; int32_t w7; } Row_2854C;
static const Row_2854C data_2854C[14] = {
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
static const uint32_t data_2870C = 131072;
static const shen_name data_28710 = SHEN_NAME('KHKS');
static const uint32_t data_28714[6] = {0x80004000U, 0, 0, 0, 0, 0};
static const int32_t data_2872C = 131072;
static const shen_name data_28730 = SHEN_NAME('GART');
static const uint32_t data_28734[2] = {0x80008000U, 0};
static const float data_2873C[4] = {-0.3f, 0.4f, 0.0f, 0.0f};
static const int32_t data_2874C = 131072;
static const shen_name data_28750[7] = {SHEN_NAME('FIR1'), 0, 0, 0, 0, 0, 0};
static const int32_t data_2876C = 131072;
static const shen_name data_28770[7] = {SHEN_NAME('FIR2'), 0, 0, 0, 0, 0, 0};
static const int32_t data_2878C = 131072;
static const shen_name data_28790[7] = {SHEN_NAME('FIR3'), 0, 0, 0, 0, 0, 0};
static const int32_t data_287AC = 131072;
static const shen_name data_287B0[7] = {SHEN_NAME('FIR4'), 0, 0, 0, 0, 0, 0};
static const int32_t data_287CC = 131072;
static const shen_name data_287D0[3] = {SHEN_NAME('HAKO'), 0, 0};
static const float data_287DC[5] = {0.4f, 0.0f, 0.0f, 0.0f, 0.0f};
static const shen_name data_287F0 = SHEN_NAME('KAK2');
static const uint32_t data_287F4[2] = {0x80004000U, 0};
static const float data_287FC[4] = {-0.7f, 0.0f, 0.0f, 0.0f};
static const int32_t data_2880C[2] = {-65536, 10};
static const uint32_t data_28814 = 0x80004000U;
static const float data_28818[6] = {-11.299f, 2.3499f, -22.951f, 0.0f, 0.0f, 0.0f};
static const shen_name data_28830 = SHEN_NAME('KAK2');
static const uint32_t data_28834[7] = {0x80004000U, 0, 0, 0, 0, 0, 0};
static const shen_name data_28850 = SHEN_NAME('KAK2');
static const uint32_t data_28854[2] = {0x80004000U, 0};
static const float data_2885C[4] = {-1.2f, 0.0f, 0.0f, 0.0f};
static const int32_t data_2886C = 131072;
static const shen_name data_28870[7] = {SHEN_NAME('POK2'), 0, 0, 0, 0, 0, 0};
static const int32_t data_2888C = 65535;
static const uint32_t data_28890 = 131072;
static const shen_name data_28894 = SHEN_NAME('KHKS');
static const uint32_t data_28898[6] = {0x80004000U, 0, 0, 0, 0, 0};
static const int32_t data_288B0 = 131072;
static const shen_name data_288B4 = SHEN_NAME('GART');
static const uint32_t data_288B8[2] = {0x80008000U, 0};
static const float data_288C0[4] = {-0.3f, 0.4f, 0.0f, 0.0f};
static const int32_t data_288D0 = 131072;
static const shen_name data_288D4[7] = {SHEN_NAME('FIR1'), 0, 0, 0, 0, 0, 0};
static const int32_t data_288F0 = 131072;
static const shen_name data_288F4[7] = {SHEN_NAME('FIR2'), 0, 0, 0, 0, 0, 0};
static const int32_t data_28910 = 131072;
static const shen_name data_28914[7] = {SHEN_NAME('FIR3'), 0, 0, 0, 0, 0, 0};
static const int32_t data_28930 = 131072;
static const shen_name data_28934[7] = {SHEN_NAME('FIR4'), 0, 0, 0, 0, 0, 0};
static const int32_t data_28950 = 131072;
static const shen_name data_28954[3] = {SHEN_NAME('HAKO'), 0, 0};
static const float data_28960[4] = {0.4f, 0.0f, 0.0f, 0.0f};
static const int32_t data_28970[2] = {-65536, 11};
static const uint32_t data_28978 = 0x80004000U;
static const float data_2897C[6] = {-11.299f, 2.3499f, -21.94f, 0.0f, 0.0f, 0.0f};
static const shen_name data_28994 = SHEN_NAME('KAK1');
static const uint32_t data_28998[2] = {0x80004000U, 0};
static const float data_289A0[5] = {-0.7f, 0.0f, 0.0f, 0.0f, 0.0f};
static const shen_name data_289B4 = SHEN_NAME('KAK1');
static const uint32_t data_289B8[7] = {0x80004000U, 0, 0, 0, 0, 0, 0};
static const shen_name data_289D4 = SHEN_NAME('KAK1');
static const uint32_t data_289D8[2] = {0x80004000U, 0};
static const float data_289E0[4] = {-1.2f, 0.0f, 0.0f, 0.0f};
static const int32_t data_289F0 = 131072;
static const shen_name data_289F4[7] = {SHEN_NAME('POK2'), 0, 0, 0, 0, 0, 0};
static const int32_t data_28A10 = 65535;
static const uint32_t data_28A14 = 131072;
static const shen_name data_28A18 = SHEN_NAME('KHKS');
static const uint32_t data_28A1C[6] = {0x80004000U, 0, 0, 0, 0, 0};
static const int32_t data_28A34 = 131072;
static const shen_name data_28A38 = SHEN_NAME('GART');
static const uint32_t data_28A3C[2] = {0x80008000U, 0};
static const float data_28A44[4] = {-0.3f, 0.4f, 0.0f, 0.0f};
static const int32_t data_28A54 = 131072;
static const shen_name data_28A58[7] = {SHEN_NAME('FIR1'), 0, 0, 0, 0, 0, 0};
static const int32_t data_28A74 = 131072;
static const shen_name data_28A78[7] = {SHEN_NAME('FIR2'), 0, 0, 0, 0, 0, 0};
static const int32_t data_28A94 = 131072;
static const shen_name data_28A98[7] = {SHEN_NAME('FIR3'), 0, 0, 0, 0, 0, 0};
static const int32_t data_28AB4 = 131072;
static const shen_name data_28AB8[7] = {SHEN_NAME('FIR4'), 0, 0, 0, 0, 0, 0};
static const int32_t data_28AD4 = 131072;
static const shen_name data_28AD8[3] = {SHEN_NAME('HAKO'), 0, 0};
static const float data_28AE4[4] = {0.4f, 0.0f, 0.0f, 0.0f};
static const int32_t data_28AF4[2] = {-65536, 11};
static const uint32_t data_28AFC = 0x80004000U;
static const float data_28B00[5] = {-11.299f, 2.3499f, -21.94f, 0.0f, 0.0f};
static const int32_t data_28B14[2] = {-65536, 10};
static const uint32_t data_28B1C = 0x80004000U;
static const float data_28B20[5] = {-11.299f, 2.3499f, -22.951f, 0.0f, 0.0f};
static const int32_t data_28B34 = 131072;
static const shen_name data_28B38[7] = {SHEN_NAME('POK2'), 0, 0, 0, 0, 0, 0};
static const int32_t data_28B54 = 65535;
static const Vector3 data_28B58 = {.x = 0.055f, .y = 1.25f, .z = -0.5f};
static const Vector3i data_28B64 = {.x = 0, .y = 0, .z = 49152};
static const Vector3 data_28B70 = {.x = 0.025f, .y = 1.252f, .z = 0.5f};
static const Vector3i data_28B7C = {.x = 16384, .y = 0, .z = 16384};
static const Vector3i data_28B88 = {.x = 0, .y = 16384, .z = 0};
static const Vector3 data_28B94 = {.x = -2.2635f, .y = 1.1315f, .z = -16.2188f};
static const Vector3 data_28BA0 = {.x = -11.2711f, .y = 1.1315f, .z = -28.659599f};
static const char data_28BAC[10] __attribute__((scn_string_literal)) = "E1011A109";
static const char data_28BB6[10] __attribute__((scn_string_literal)) = "E1011A110";
static const char data_28BC0[10] __attribute__((scn_string_literal)) = "E1011A111";
static const char data_28BCA[10] __attribute__((scn_string_literal)) = "E1011A112";
static const char data_28BD4[10] __attribute__((scn_string_literal)) = "E1011A113";
static const char data_28BDE[10] __attribute__((scn_string_literal)) = "E1011A107";
static const char data_28BE8[12] __attribute__((scn_string_literal)) = "E1011A108";
static const Vector3 data_28BF4 = {.x = -2.3772f, .y = 0.0f, .z = -23.4748f};
static const Vector3 data_28C00 = {.x = -0.7816f, .y = 0.0f, .z = -21.4021f};
static const Vector3 data_28C0C = {.x = -10.995349f, .y = 0.0f, .z = -22.697281f};
static const Vector3 data_28C18 = {.x = -10.375824f, .y = 0.0f, .z = -23.385668f};
static const Vector3 data_28C24 = {.x = -10.995349f, .y = 0.0f, .z = -21.0f};
static const Vector3 data_28C30 = {.x = -10.375824f, .y = 0.0f, .z = -22.25f};
static const Vector3 data_28C3C = {.x = -10.6f, .y = 0.0f, .z = -24.2f};
static const Vector3 data_28C48 = {.x = -8.85f, .y = 0.0f, .z = -26.1f};
static const Vector3 data_28C54 = {.x = -14.8742f, .y = 0.0f, .z = -26.414801f};
static const Vector3 data_28C60 = {.x = -11.35f, .y = 0.0f, .z = -24.0f};
static const float data_28C6C[2] = {-14.8742f, 0.0f};
static const uint32_t data_28C74 = 0xC1D35183U;
static const float data_28C78[2] = {-11.4742f, 0.0f};
static const uint32_t data_28C80 = 0xC1C4EB1CU;
static const float data_28C84[24] = {
  -1.0f, 0.0f, -1.0f, 1.0f, 0.0f, 1.0f, -3.95f, 0.0f, -27.3f, -9.72f, 0.0f, -17.8f, -0.6f, 0.0f, -0.6f, 0.6f, 0.0f, 0.6f, -5.8f, 1.05f, -16.5f, -7.9f,
  1.0595f, -18.0f
};
static const Vector3 data_28CE4 = {.x = -1.0f, .y = 0.0f, .z = -1.0f};
static const Vector3 data_28CF0 = {.x = 1.0f, .y = 0.0f, .z = 1.0f};
static const Vector3 data_28CFC = {.x = -2.33f, .y = 0.0f, .z = -23.76f};
static const Vector3 data_28D08 = {.x = -4.0f, .y = 0.0f, .z = -21.37f};
typedef struct Row_28D14 { float w0; int32_t w1; float w2; } Row_28D14;
static const Row_28D14 data_28D14[2] = {
  {-0.2f, 0, -0.2f},
  {0.2f, 0, 0.2f}
};
static const char data_28D2C[8] __attribute__((scn_string_literal)) = "BROOM";
static const Vector3 data_28D34 = {.x = -10.5462f, .y = 1.1095f, .z = -22.7847f};
static const Vector3i data_28D40 = {.x = 0, .y = 15841, .z = 0};
static const Vector3 data_28D4C = {.x = 0.115f, .y = 0.119f, .z = -0.024f};
static const Vector3i data_28D58 = {.x = 65353, .y = 62984, .z = 546};
static const Vector3 data_28D64 = {.x = 0.093f, .y = 0.163f, .z = -0.013f};
static const Vector3i data_28D70 = {.x = 31676, .y = 2547, .z = 0};
static const Vector3 data_28D7C = {.x = 0.05f, .y = 1.25f, .z = -0.5f};
static const Vector3i data_28D88 = {.x = 0, .y = 32768, .z = 16384};
static const Vector3i data_28D94 = {.x = 0, .y = 0, .z = 16384};
static const float data_28DA0[2] = {-10.8f, 1.1095f};
static const uint32_t data_28DA8 = 0xC1C1353FU;
static const float data_28DAC[4] = {0.03f, 1.85f, 0.7f, 0.0f};
static const int32_t data_28DBC[2] = {16384, 0};
static const Vector3 data_28DC4 = {.x = 0.096f, .y = 0.163f, .z = 0.011f};
static const Vector3i data_28DD0 = {.x = 32768, .y = 65171, .z = 64080};
static const char data_28DDC[10] __attribute__((scn_string_literal)) = "E1011A055";
static const char data_28DE6[10] __attribute__((scn_string_literal)) = "E1011A045";
static const Vector3 data_28DF0 = {.x = -10.6806f, .y = 1.102f, .z = -22.978901f};
static const Vector3i data_28DFC = {.x = 0, .y = 16384, .z = 0};
typedef struct Row_28E08 { float w0; float w1; float w2; int32_t w3; int32_t w4; int32_t w5; } Row_28E08;
static const Row_28E08 data_28E08[2] = {
  {0.115f, 0.119f, -0.024f, 65353, 62984, 546},
  {0.093f, 0.163f, -0.013f, 31676, 2547, 0}
};
static const Vector3 data_28E38 = {.x = 0.055f, .y = 1.25f, .z = -0.5f};
static const Vector3i data_28E44 = {.x = 0, .y = 0, .z = 49152};
static const int32_t data_28E50[3] = {0, 0, 16384};
static const float data_28E5C[2] = {-10.8f, 1.1095f};
static const uint32_t data_28E64 = 0xC1C1353FU;
static const float data_28E68[4] = {0.03f, 1.85f, 0.7f, 0.0f};
static const int32_t data_28E78[2] = {16384, 0};
static const float data_28E80[3] = {0.096f, 0.163f, 0.011f};
static const int32_t data_28E8C[3] = {32768, 65171, 64080};
static const char data_28E98[10] __attribute__((scn_string_literal)) = "E1011A045";
static const char data_28EA2[10] __attribute__((scn_string_literal)) = "JINGURU01";
static const char data_28EAC[10] __attribute__((scn_string_literal)) = "JINGURU01";
static const char data_28EB6[10] __attribute__((scn_string_literal)) = "JINGURU01";
static const uint32_t data_28EC0 = 1;
static const float data_28EC4[3] = {2.17f, -22.45f, 1.0f};
static const int32_t data_28ED0 = 2;
static const float data_28ED4[3] = {7.36f, -19.18f, 1.0f};
static const int32_t data_28EE0 = 3;
static const float data_28EE4[3] = {7.36f, -10.96f, 1.0f};
static const int32_t data_28EF0 = 196608;
static const float data_28EF4[3] = {0.096f, 0.163f, 0.011f};
static const int32_t data_28F00[3] = {32768, 32768, 64080};
static const float data_28F0C[3] = {0.155f, 0.054f, -0.032f};
static const int32_t data_28F18[3] = {15834, 15469, 35671};
static const Vector3 data_28F24 = {.x = -10.7095f, .y = 1.1095f, .z = -22.887501f};
static const Vector3i data_28F30 = {.x = 0, .y = 16384, .z = 0};
static const Vector3 data_28F3C = {.x = -9.8f, .y = 1.0565f, .z = -24.112f};
static const Vector3i data_28F48 = {.x = 29191, .y = 50374, .z = 3289};
typedef struct Row_28F54 { float w0; float w1; float w2; float w3; int32_t w4; } Row_28F54;
static const Row_28F54 data_28F54[2] = {
  {-11.2439f, 2.9721f, -23.097f, 0.0f, 16384},
  {0.0f, -0.276f, 0.162f, -0.51f, 8372}
};
static const int32_t data_28F7C[2] = {0, 44059};
static const char data_28F84[10] __attribute__((scn_string_literal)) = "E1011A078";
static const char data_28F8E[10] __attribute__((scn_string_literal)) = "E1011A079";
static const Vector3 data_28F98 = {.x = -10.7095f, .y = 1.12075f, .z = -21.877501f};
static const Vector3i data_28FA4 = {.x = 0, .y = 16384, .z = 0};
static const Vector3 data_28FB0 = {.x = -9.8f, .y = 1.0565f, .z = -22.6f};
static const Vector3i data_28FBC = {.x = 29191, .y = 50374, .z = 3289};
typedef struct Row_28FC8 { float w0; float w1; float w2; float w3; int32_t w4; } Row_28FC8;
static const Row_28FC8 data_28FC8[2] = {
  {-11.2439f, 2.9721f, -23.097f, 0.0f, 16384},
  {0.0f, -0.276f, 0.162f, -0.51f, 8372}
};
static const int32_t data_28FF0[2] = {0, 44059};
static const char data_28FF8[6] __attribute__((scn_string_literal)) = "BROOM";
static const char data_28FFE[10] __attribute__((scn_string_literal)) = "E1011A103";
static const char data_29008[10] __attribute__((scn_string_literal)) = "E1011A104";
static const char data_29012[10] __attribute__((scn_string_literal)) = "E1011A105";
static const Vector3 data_2901C = {.x = 0.1173f, .y = 0.0177f, .z = -0.0934f};
static const Vector3i data_29028 = {.x = 1063, .y = 949, .z = 275};
static const int32_t data_29034[6] = {49152, 0, 16384, 0, 8008, 0};
static const float data_2904C[3] = {-11.2741f, 2.3605f, -21.9512f};
static const Vector3 data_29058 = {.x = 0.025f, .y = 1.252f, .z = 0.5f};
static const Vector3i data_29064 = {.x = 16384, .y = 0, .z = 16384};
static const char data_29070[10] __attribute__((scn_string_literal)) = "E1011A082";
static const char data_2907A[10] __attribute__((scn_string_literal)) = "E1011A045";
static const uint32_t data_29084 = 1;
static const float data_29088[3] = {-12.14f, -23.0f, 0.1f};
static const int32_t data_29094 = 196608;
static const char data_29098[10] __attribute__((scn_string_literal)) = "E1011A116";
static const char data_290A2[10] __attribute__((scn_string_literal)) = "E1011A117";
static const char data_290AC[12] __attribute__((scn_string_literal)) = "E1011A118";
static const uint32_t data_290B8 = 1;
static const uint32_t data_290BC[2] = {0xC13E6006U, 0xC1B823FEU};
static const float data_290C4 = 0.1f;
static const int32_t data_290C8 = 2;
static const uint32_t data_290CC[2] = {0xC13FDDF7U, 0xC1D24001U};
static const float data_290D4 = 0.1f;
static const int32_t data_290D8 = 3;
static const float data_290DC[3] = {-12.6812f, -26.243f, 0.1f};
static const int32_t data_290E8 = 196608;
static const uint32_t data_290EC[6] = {0xC137472BU, 0, 0xC1BBAF1CU, 0xC1B37A6BU, 0, 0xC1B37A6BU};
static const Vector3 data_29104 = {.x = -14.8742f, .y = 0.0f, .z = -26.414801f};
static const Vector3 data_29110 = {.x = -11.4742f, .y = 0.0f, .z = -24.614799f};
static const float data_2911C[4] = {0.03f, 1.85f, 0.7f, 0.0f};
static const int32_t data_2912C[2] = {16384, 0};
static const float data_29134[2] = {-10.8f, 1.1095f};
static const uint32_t data_2913C = 0xC1C1353FU;
static const Vector3 data_29140 = {.x = -10.163f, .y = 1.0765f, .z = -24.112f};
static const Vector3i data_2914C = {.x = 29191, .y = 50374, .z = 3289};
static const Vector3 data_29158 = {.x = -10.163f, .y = 1.0765f, .z = -22.6f};
static const Vector3i data_29164 = {.x = 29191, .y = 50374, .z = 3289};
static const uint32_t data_29170 = 1;
typedef struct Row_29174 { float w0; float w1; float w2; int32_t w3; } Row_29174;
static const Row_29174 data_29174[2] = {
  {-4.04f, -24.0f, 1.0f, 1},
  {-2.8f, -24.55f, 1.0f, 196608}
};
static const char data_29194[36] = "E1011A023\000\000\000\246\012\366\300\262\235\207?,\324\210\301\000\000\000\000K\234";
static const Vector3 data_291B8 = {.x = 0.0922f, .y = 0.1018f, .z = 0.00806f};
static const Vector3i data_291C4 = {.x = 0, .y = 0, .z = 0};
static const Vector3 data_291D0 = {.x = 0.0922f, .y = 0.1018f, .z = 0.00806f};
static const Vector3i data_291DC = {.x = 0, .y = 0, .z = 0};
static const char data_291E8[10] __attribute__((scn_string_literal)) = "E1011A031";
static const char data_291F2[10] __attribute__((scn_string_literal)) = "E1011A101";
static const char data_291FC[10] __attribute__((scn_string_literal)) = "E1011A031";
static const char data_29206[10] __attribute__((scn_string_literal)) = "E1011A026";
static const char data_29210[10] __attribute__((scn_string_literal)) = "E1011A027";
static const char data_2921A[10] __attribute__((scn_string_literal)) = "E1011A028";
static const char data_29224[10] __attribute__((scn_string_literal)) = "E1011A029";
static const char data_2922E[10] __attribute__((scn_string_literal)) = "E1011A030";
static const char data_29238[10] __attribute__((scn_string_literal)) = "E1011A032";
static const char data_29242[10] __attribute__((scn_string_literal)) = "E1011A033";
static const char data_2924C[10] __attribute__((scn_string_literal)) = "E1011A074";
static const char data_29256[10] __attribute__((scn_string_literal)) = "E1011A077";
static const char data_29260[10] __attribute__((scn_string_literal)) = "E1011A076";
static const char data_2926A[10] __attribute__((scn_string_literal)) = "E1011A017";
static const char data_29274[10] __attribute__((scn_string_literal)) = "E1011A016";
static const char data_2927E[10] __attribute__((scn_string_literal)) = "E1011A018";
static const char data_29288[10] __attribute__((scn_string_literal)) = "E1011A007";
static const char data_29292[10] __attribute__((scn_string_literal)) = "E1011A016";
static const char data_2929C[10] __attribute__((scn_string_literal)) = "E1011A034";
static const char data_292A6[10] __attribute__((scn_string_literal)) = "E1011A035";
static const char data_292B0[10] __attribute__((scn_string_literal)) = "E1011A036";
static const char data_292BA[10] __attribute__((scn_string_literal)) = "E1011A037";
static const char data_292C4[10] __attribute__((scn_string_literal)) = "E1011A034";
static const char data_292CE[10] __attribute__((scn_string_literal)) = "E1011A038";
static const char data_292D8[10] __attribute__((scn_string_literal)) = "E1011A039";
static const char data_292E2[10] __attribute__((scn_string_literal)) = "E1011A040";
static const char data_292EC[10] __attribute__((scn_string_literal)) = "E1011A041";
static const char data_292F6[10] __attribute__((scn_string_literal)) = "E1011A042";
static const char data_29300[10] __attribute__((scn_string_literal)) = "E1011A007";
static const char data_2930A[10] __attribute__((scn_string_literal)) = "E1011A008";
static const char data_29314[10] __attribute__((scn_string_literal)) = "E1011A034";
static const char data_2931E[10] __attribute__((scn_string_literal)) = "E1011A035";
static const char data_29328[10] __attribute__((scn_string_literal)) = "E1011A082";
static const char data_29332[10] __attribute__((scn_string_literal)) = "E1011A045";
static const char data_2933C[10] __attribute__((scn_string_literal)) = "E1011A007";
static const char data_29346[10] __attribute__((scn_string_literal)) = "E1011A046";
static const char data_29350[10] __attribute__((scn_string_literal)) = "E1011A051";
static const char data_2935A[10] __attribute__((scn_string_literal)) = "E1011A052";
static const char data_29364[10] __attribute__((scn_string_literal)) = "E1011A053";
static const char data_2936E[10] __attribute__((scn_string_literal)) = "E1011A054";
static const char data_29378[10] __attribute__((scn_string_literal)) = "E1011A055";
static const char data_29382[10] __attribute__((scn_string_literal)) = "E1011A056";
static const char data_2938C[10] __attribute__((scn_string_literal)) = "E1011A057";
static const char data_29396[10] __attribute__((scn_string_literal)) = "E1011A047";
static const char data_293A0[10] __attribute__((scn_string_literal)) = "E1011A045";
static const char data_293AA[10] __attribute__((scn_string_literal)) = "E1011A007";
static const char data_293B4[10] __attribute__((scn_string_literal)) = "E1011A046";
static const char data_293BE[10] __attribute__((scn_string_literal)) = "E1011A048";
static const char data_293C8[10] __attribute__((scn_string_literal)) = "E1011A049";
static const char data_293D2[10] __attribute__((scn_string_literal)) = "E1011A050";
static const char data_293DC[10] __attribute__((scn_string_literal)) = "E1011A070";
static const char data_293E6[10] __attribute__((scn_string_literal)) = "E1011A071";
static const char data_293F0[10] __attribute__((scn_string_literal)) = "E1011A072";
static const char data_293FA[10] __attribute__((scn_string_literal)) = "E1011A087";
static const char data_29404[10] __attribute__((scn_string_literal)) = "E1011A017";
static const char data_2940E[10] __attribute__((scn_string_literal)) = "E1011A016";
static const char data_29418[10] __attribute__((scn_string_literal)) = "E1011A018";
static const char data_29422[10] __attribute__((scn_string_literal)) = "E1011A065";
static const char data_2942C[10] __attribute__((scn_string_literal)) = "E1011A007";
static const char data_29436[10] __attribute__((scn_string_literal)) = "E1011A063";
static const char data_29440[10] __attribute__((scn_string_literal)) = "E1011A017";
static const char data_2944A[10] __attribute__((scn_string_literal)) = "E1011A016";
static const char data_29454[10] __attribute__((scn_string_literal)) = "E1011A018";
static const char data_2945E[10] __attribute__((scn_string_literal)) = "E1011A065";
static const char data_29468[10] __attribute__((scn_string_literal)) = "E1011A007";
static const char data_29472[10] __attribute__((scn_string_literal)) = "E1011A016";
static const char data_2947C[10] __attribute__((scn_string_literal)) = "E1011A040";
static const char data_29486[10] __attribute__((scn_string_literal)) = "E1011A078";
static const char data_29490[10] __attribute__((scn_string_literal)) = "E1011A089";
static const char data_2949A[10] __attribute__((scn_string_literal)) = "E1011A081";
static const char data_294A4[10] __attribute__((scn_string_literal)) = "E1011A090";
static const char data_294AE[10] __attribute__((scn_string_literal)) = "E1011A040";
static const char data_294B8[10] __attribute__((scn_string_literal)) = "E1011A042";
static const char data_294C2[10] __attribute__((scn_string_literal)) = "E1011A031";
static const char data_294CC[10] __attribute__((scn_string_literal)) = "E1011A091";
static const char data_294D6[10] __attribute__((scn_string_literal)) = "E1011A092";
static const char data_294E0[10] __attribute__((scn_string_literal)) = "E1011A093";
static const char data_294EA[10] __attribute__((scn_string_literal)) = "E1011A042";
static const char data_294F4[10] __attribute__((scn_string_literal)) = "E1011A094";
static const char data_294FE[10] __attribute__((scn_string_literal)) = "E1011A095";
static const char data_29508[10] __attribute__((scn_string_literal)) = "E1011A096";
static const char data_29512[10] __attribute__((scn_string_literal)) = "E1011A097";
static const char data_2951C[10] __attribute__((scn_string_literal)) = "E1011A098";
static const char data_29526[10] __attribute__((scn_string_literal)) = "E1011A031";
static const Vector3 data_29530 = {.x = 0.332f, .y = 0.001f, .z = 0.223f};
static const Vector3i data_2953C = {.x = 0, .y = 182, .z = 10555};
static const char data_29548[10] __attribute__((scn_string_literal)) = "E1011A006";
static const char data_29552[10] __attribute__((scn_string_literal)) = "E1011A102";
static const uint32_t data_2955C = 0x3ECCCCCD;
typedef struct Row_29560 { float w0; float w1; int32_t w2; int32_t w3; int32_t w4; float w5; } Row_29560;
static const Row_29560 data_29560[2] = {
  {0.0f, -22.5f, 16384, 40192, 4, -0.55f},
  {1.0f, -22.5f, 49152, 39176, 3, 100000.0f}
};
static const uint8_t data_29590[8] = {0};
static const float data_29598[2] = {0.6f, 0.0f};
static const int32_t data_295A0 = 1;
static const float data_295A4 = 0.6f;
static const int32_t data_295A8[10] = {1, 0, 0, 2, 1, 1, 2, 0, 2, 3};
static const char data_295D0[16] __attribute__((scn_string_literal)) = "/scene/01/JHD0/";
static const char data_295E0[5] __attribute__((scn_string_literal)) = "HIHY";
static const char data_295E5[11] __attribute__((scn_string_literal)) = "tsm003.snd";
static const char data_295F0[7] __attribute__((scn_string_literal)) = "A0125B";
static const char data_295F7[5] __attribute__((scn_string_literal)) = "HIHY";
static const char data_295FC[1] __attribute__((scn_string_literal)) = "";
static const char data_295FD[6] __attribute__((scn_string_literal)) = "A0125";
static const char data_29603[5] __attribute__((scn_string_literal)) = "HIHY";
static const char data_29608[11] __attribute__((scn_string_literal)) = "bgm009.snd";
static const char data_29613[13] __attribute__((scn_string_literal)) = "f1doujou.snd";
static const char data_29620[7] __attribute__((scn_string_literal)) = "SA1082";
static const char data_29627[11] __attribute__((scn_string_literal)) = "SA1082A001";
static const char data_29632[11] __attribute__((scn_string_literal)) = "SA1082A002";
static const char data_2963D[11] __attribute__((scn_string_literal)) = "SA1082A003";
static const char data_29648[11] __attribute__((scn_string_literal)) = "SA1082A004";
static const char data_29653[11] __attribute__((scn_string_literal)) = "SA1082A005";
static const char data_2965E[7] __attribute__((scn_string_literal)) = "SA1082";
static const char data_29665[13] __attribute__((scn_string_literal)) = "a1_oyaji.snd";
static const char data_29672[11] __attribute__((scn_string_literal)) = "bgm009.snd";
static const char data_2967D[6] __attribute__((scn_string_literal)) = "YKB_M";
static const char data_29683[29] = "YKC_M\000\000\000\000\001\000\000\000\315\314,\300\315\314\262\301\000\000\200?\000\000\003";
static const uint16_t data_296A0[8] = {670, 672, 674, 677, 684, 682, 680, 0};
static uint32_t data_296B0 = 1;
static const int32_t data_296B4[4] = {0, 0, 0, 196608};
static const char data_296C4[11] __attribute__((scn_string_literal)) = "SA1116B001";
static const char data_296CF[11] __attribute__((scn_string_literal)) = "SA1116A001";
static const char data_296DA[11] __attribute__((scn_string_literal)) = "SA1116B002";
static const char data_296E5[11] __attribute__((scn_string_literal)) = "SA1116A002";
static const char data_296F0[11] __attribute__((scn_string_literal)) = "SA1116B003";
static const char data_296FB[11] __attribute__((scn_string_literal)) = "SA1116A003";
static const char data_29706[11] __attribute__((scn_string_literal)) = "SA1116B004";
static const char data_29711[11] __attribute__((scn_string_literal)) = "SA1116A004";
static const char data_2971C[11] __attribute__((scn_string_literal)) = "SA1116A005";
static const char data_29727[11] __attribute__((scn_string_literal)) = "SA1116A006";
static const char data_29732[11] __attribute__((scn_string_literal)) = "SA1116B005";
static const char data_2973D[11] __attribute__((scn_string_literal)) = "bgm202.snd";
static const char data_29748[5] __attribute__((scn_string_literal)) = "MSKA";
static const char data_2974D[1] __attribute__((scn_string_literal)) = "";
static const char data_2974E[5] __attribute__((scn_string_literal)) = "MSKA";
static const char data_29753[11] __attribute__((scn_string_literal)) = "bgm009.snd";
static const char data_2975E[6] __attribute__((scn_string_literal)) = "YKB_M";
static const uint32_t data_29764 = SHEN_NAME('KOI0');
static const shen_name data_29768[4] = {SHEN_NAME('KOI1'), SHEN_NAME('KOI2'), SHEN_NAME('KOI3'), SHEN_NAME('KOI4')};
static const uint32_t data_29778 = SHEN_NAME('HAS0');
static const shen_name data_2977C[20] = {
  SHEN_NAME('HAS1'), SHEN_NAME('HAS2'), SHEN_NAME('HAS3'), SHEN_NAME('HAS4'), SHEN_NAME('HAS5'), SHEN_NAME('HAS6'), SHEN_NAME('HAS7'),
  SHEN_NAME('HAS8'), SHEN_NAME('HAS9'), SHEN_NAME('HASA'), SHEN_NAME('HASB'), SHEN_NAME('HASC'), SHEN_NAME('HASD'), SHEN_NAME('HASE'),
  SHEN_NAME('HASF'), SHEN_NAME('HAN0'), SHEN_NAME('HAN1'), SHEN_NAME('HAN2'), SHEN_NAME('HAN3'), SHEN_NAME('HAN4')
};
static const int32_t data_297CC = -1;
static const uint32_t data_297D0 = 1;
typedef struct Row_297D4 { float w0; float w1; float w2; int32_t w3; } Row_297D4;
static const Row_297D4 data_297D4[7] = {
  {9.2f, 7.2f, 0.4f, 1},
  {8.6f, 6.6f, 0.4f, 1},
  {6.6f, 6.8f, 0.4f, 1},
  {5.4f, 7.6f, 0.4f, 1},
  {5.4f, 8.6f, 0.4f, 1},
  {6.0f, 9.4f, 0.4f, 1},
  {8.2f, 9.2f, 0.4f, 196608}
};
static const uint32_t data_29844 = 1;
typedef struct Row_29848 { float w0; float w1; float w2; int32_t w3; } Row_29848;
static const Row_29848 data_29848[4] = {
  {9.2f, 8.2f, 0.6f, 1},
  {8.3f, 6.9f, 0.6f, 1},
  {7.3f, 7.1f, 0.8f, 1},
  {7.6f, 7.9f, 0.6f, 196608}
};
static const uint32_t data_29888 = 1;
typedef struct Row_2988C { float w0; float w1; float w2; int32_t w3; } Row_2988C;
static const Row_2988C data_2988C[11] = {
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
static const uint32_t data_2993C = 1;
typedef struct Row_29940 { float w0; float w1; float w2; int32_t w3; } Row_29940;
static const Row_29940 data_29940[5] = {
  {6.5f, 9.2f, 0.6f, 1},
  {5.7f, 8.5f, 1.0f, 1},
  {6.3f, 7.4f, 1.0f, 1},
  {7.5f, 7.6f, 1.0f, 1},
  {7.5f, 8.8f, 1.0f, 196608}
};
static const uint32_t data_29990 = 1;
typedef struct Row_29994 { float w0; float w1; float w2; int32_t w3; } Row_29994;
static const Row_29994 data_29994[6] = {
  {6.7f, 7.5f, 1.0f, 1},
  {7.1f, 6.6f, 1.0f, 1},
  {9.0f, 6.4f, 1.0f, 1},
  {10.1f, 7.8f, 1.0f, 1},
  {9.2f, 9.3f, 1.0f, 1},
  {7.7f, 8.2f, 1.0f, 196608}
};
static const Vector3 data_299F4 = {.x = 9.3f, .y = -0.08f, .z = 11.0f};
static const char data_29A00[13] __attribute__((scn_string_literal)) = "model/object";
static const char data_29A0D[12] __attribute__((scn_string_literal)) = "trlake1.pvr";
static const char data_29A19[13] __attribute__((scn_string_literal)) = "model/object";
static const char data_29A26[13] __attribute__((scn_string_literal)) = "trsea025.mt6";
static const char data_29A33[13] __attribute__((scn_string_literal)) = "model/object";
static const char data_29A40[48] = "grid025.bin\000\001\000\000\000\232\231i\300ff\256\301\000\000\200?\002\000\000\000ff\226\300\000\000\244\301\000\000\200?\000\000\003";
static const char data_29A70[7] __attribute__((scn_string_literal)) = "A01124";
static const char data_29A77[6] __attribute__((scn_string_literal)) = "01KAK";
static const char data_29A7D[7] __attribute__((scn_string_literal)) = "SA1077";
static const char data_29A84[6] __attribute__((scn_string_literal)) = "FUK_M";
static const char data_29A8A[16] __attribute__((scn_string_literal)) = "/scene/01/JHD0/";
static const char data_29A9A[5] __attribute__((scn_string_literal)) = "KAKG";
static const char data_29A9F[5] __attribute__((scn_string_literal)) = "KAKG";
static const char data_29AA4[1] __attribute__((scn_string_literal)) = "";
static const char data_29AA5[5] __attribute__((scn_string_literal)) = "KAKG";
static const char data_29AAA[1] __attribute__((scn_string_literal)) = "";
static const char data_29AAB[5] __attribute__((scn_string_literal)) = "KAKG";
static const char data_29AB0[1] __attribute__((scn_string_literal)) = "";
static const char data_29AB1[5] __attribute__((scn_string_literal)) = "KAKG";
static const char data_29AB6[1] __attribute__((scn_string_literal)) = "";
static const char data_29AB7[11] __attribute__((scn_string_literal)) = "bgm012.snd";
static const char data_29AC2[6] __attribute__((scn_string_literal)) = "YKE_M";
static const char data_29AC8[11] __attribute__((scn_string_literal)) = "SA1077A001";
static const char data_29AD3[6] __attribute__((scn_string_literal)) = "YKF_M";
static const char data_29AD9[11] __attribute__((scn_string_literal)) = "SA1077B001";
static const char data_29AE4[11] __attribute__((scn_string_literal)) = "SA1077A002";
static const char data_29AEF[11] __attribute__((scn_string_literal)) = "SA1077B002";
static const char data_29AFA[6] __attribute__((scn_string_literal)) = "YKC_M";
static const char data_29B00[13] __attribute__((scn_string_literal)) = "a1_kakgo.snd";
static const char data_29B0D[11] __attribute__((scn_string_literal)) = "bgm085.snd";
static const char data_29B18[6] __attribute__((scn_string_literal)) = "YKB_M";
static const char data_29B1E[6] __attribute__((scn_string_literal)) = "INE_M";
static const char data_29B24[11] __attribute__((scn_string_literal)) = "bgm049.snd";
static const char data_29B2F[5] __attribute__((scn_string_literal)) = "KAKG";
static const int32_t data_29B34[57] = {
  0, 0, -5461, 0, -2366, -1092, 0, -182, 1092, 0, 182, -3458, 0, 0, -2548, 0, 0, -3458, 0, -182, 1274, 0, 910, -910, 0, 0, -2366, 0, 0, -5097, 0, 0,
  1456, 0, 182, -1820, 0, 0, -3276, 0, 0, -4915, -182, -182, 1820, 0, -728, -2730, 0, -1274, -5461, 0, -910, -3640, 0, 0, 1638
};
static const Vector4 data_29C18 = {.x = 0.3f, .y = 0.3f, .z = 0.3f, .w = 0.3f};
static const char data_29C28[16] __attribute__((scn_string_literal)) = "/scene/01/JHD0/";
static const char data_29C38[5] __attribute__((scn_string_literal)) = "KAKG";
static const char data_29C3D[5] __attribute__((scn_string_literal)) = "KAKG";
static const char data_29C42[1] __attribute__((scn_string_literal)) = "";
static const char data_29C43[5] __attribute__((scn_string_literal)) = "KAKG";
static const char data_29C48[1] __attribute__((scn_string_literal)) = "";
static const char data_29C49[11] __attribute__((scn_string_literal)) = "bgm012.snd";
static const char data_29C54[6] __attribute__((scn_string_literal)) = "YKF_M";
static const char data_29C5A[6] __attribute__((scn_string_literal)) = "FUK_M";
static const char data_29C60[6] __attribute__((scn_string_literal)) = "YKC_M";
static const char data_29C66[10] __attribute__((scn_string_literal)) = "E1031A008";
static const char data_29C70[10] __attribute__((scn_string_literal)) = "E1031A009";
static const char data_29C7A[10] __attribute__((scn_string_literal)) = "E1031A010";
static const char data_29C84[10] __attribute__((scn_string_literal)) = "E1031A011";
static const char data_29C8E[10] __attribute__((scn_string_literal)) = "E1031B014";
static const char data_29C98[10] __attribute__((scn_string_literal)) = "E1031B015";
static const char data_29CA2[5] __attribute__((scn_string_literal)) = "KAKG";
static const char data_29CA7[11] __attribute__((scn_string_literal)) = "bgm009.snd";
static const char data_29CB2[6] __attribute__((scn_string_literal)) = "E1031";
static const char data_29CB8[6] __attribute__((scn_string_literal)) = "YKF_M";
static const char data_29CBE[11] __attribute__((scn_string_literal)) = "M_FGT1.BIN";
static const char data_29CC9[10] __attribute__((scn_string_literal)) = "E1031B009";
static const char data_29CD3[10] __attribute__((scn_string_literal)) = "E1031B007";
static const char data_29CDD[11] __attribute__((scn_string_literal)) = "E1031B008";
static const uint32_t data_29CE8 = 3;
static const float data_29CEC[3] = {-2.99f, -22.55f, 1.0f};
static const int32_t data_29CF8 = 196608;
static const uint16_t data_29CFC[20] = {
  1068, 1043, 1069, 1071, 1074, 1072, 1073, 0, 52953, 16735, 52429, 15820, 11901, 16956, 14995, 16706, 52429, 15820, 7602, 16957
};
static const Vector3 data_29D24 = {.x = -5.382f, .y = 0.9526f, .z = -23.509501f};
static const Vector3 data_29D30 = {.x = -6.8787f, .y = 0.975f, .z = -21.903799f};
static const Vector3 data_29D3C = {.x = -3.9799f, .y = 0.9594f, .z = -22.0175f};
static const uint32_t data_29D48 = 0xC0C3A440;
static const float data_29D4C = 0.9594f;
static const uint32_t data_29D50 = 0xC1A33C6AU;
static const Vector3 data_29D54 = {.x = -6.9167f, .y = 0.975f, .z = -17.833599f};
static const uint32_t data_29D60 = 8198;
typedef struct Row_29D64 { int32_t w0; uint32_t w1; int32_t w2; } Row_29D64;
static const Row_29D64 data_29D64[16] = {
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
static const uint32_t data_29E24 = 8198;
static const uint32_t data_29E28[9] = {0, 0, 0, 0, 0xF000200U, 0x600808FU, 0x80880800U, 0x8000A00U, 0x12008088U};
static const int32_t data_29E4C = 5309440;
static const uint32_t data_29E50[2] = {0x4002200U, 0x42000051U};
static const int32_t data_29E58 = 2228736;
static const uint32_t data_29E5C = 0x2F88300U;
static const int32_t data_29E60[2] = {65538, 262408};
static const uint32_t data_29E68 = 0x1080002U;
static const int32_t data_29E6C[2] = {65540, 262408};
static const uint32_t data_29E74[2] = {0x2F88300U, 0x42000002U};
static const int32_t data_29E7C = 2228736;
static const uint32_t data_29E80[2] = {0x4002200U, 0x12000051U};
static const int32_t data_29E88 = 5309440;
static const uint32_t data_29E8C[4] = {0x8000A00U, 0x6008088U, 0x80880800U, 0xF000200U};
static const int32_t data_29E9C[19] = {32911, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
static const Vector4 data_29EE8 = {.x = 0.3f, .y = 0.3f, .z = 0.3f, .w = 0.3f};
static const char data_29EF8[26] __attribute__((scn_string_literal)) = "<<< WazaDenjyu Start >>>\012";
static const char data_29F12[6] __attribute__((scn_string_literal)) = "YKE_M";
static const char data_29F18[10] __attribute__((scn_string_literal)) = "YGB_B.MT5";
static const char data_29F22[16] __attribute__((scn_string_literal)) = "/scene/01/JHD0/";
static const char data_29F32[5] __attribute__((scn_string_literal)) = "JHW0";
static const char data_29F37[5] __attribute__((scn_string_literal)) = "JHW0";
static const char data_29F3C[5] __attribute__((scn_string_literal)) = "JHW0";
static const char data_29F41[13] __attribute__((scn_string_literal)) = "seqdata1.bin";
static const char data_29F4E[5] __attribute__((scn_string_literal)) = "JHW0";
static const char data_29F53[13] __attribute__((scn_string_literal)) = "seqdata2.bin";
static const char data_29F60[5] __attribute__((scn_string_literal)) = "JHW0";
static const char data_29F65[13] __attribute__((scn_string_literal)) = "seqdata3.bin";
static const char data_29F72[5] __attribute__((scn_string_literal)) = "JHW0";
static const char data_29F77[13] __attribute__((scn_string_literal)) = "seqdata4.bin";
static const char data_29F84[5] __attribute__((scn_string_literal)) = "JHW0";
static const char data_29F89[13] __attribute__((scn_string_literal)) = "seqdata5.bin";
static const char data_29F96[5] __attribute__((scn_string_literal)) = "JHW0";
static const char data_29F9B[13] __attribute__((scn_string_literal)) = "seqdata6.bin";
static const char data_29FA8[5] __attribute__((scn_string_literal)) = "JHW0";
static const char data_29FAD[13] __attribute__((scn_string_literal)) = "seqdata7.bin";
static const char data_29FBA[5] __attribute__((scn_string_literal)) = "JHW0";
static const char data_29FBF[13] __attribute__((scn_string_literal)) = "seqdata8.bin";
static const char data_29FCC[5] __attribute__((scn_string_literal)) = "JHW0";
static const char data_29FD1[6] __attribute__((scn_string_literal)) = "E1017";
static const char data_29FD7[11] __attribute__((scn_string_literal)) = "bgm099.snd";
static const char data_29FE2[12] __attribute__((scn_string_literal)) = "a1_fukw.snd";
static const char data_29FEE[13] __attribute__((scn_string_literal)) = "battle_1.snd";
static const char data_29FFB[12] __attribute__((scn_string_literal)) = "system1.snd";
static const char data_2A007[11] __attribute__((scn_string_literal)) = "bgm009.snd";
static const char data_2A012[13] __attribute__((scn_string_literal)) = "f1doujou.snd";
static const char data_2A01F[13] __attribute__((scn_string_literal)) = "battle_1.snd";
static const char data_2A02C[6] __attribute__((scn_string_literal)) = "E1017";
static const char data_2A032[5] __attribute__((scn_string_literal)) = "JHW0";
static const char data_2A037[6] __attribute__((scn_string_literal)) = "YKB_M";
static const char data_2A03D[10] __attribute__((scn_string_literal)) = "YKA_B.MT5";
static const char data_2A047[17] __attribute__((scn_string_literal)) = "auth %d start\012";
static const Vector3i data_2A058 = {.x = 0, .y = 55159, .z = 0};
static const char data_2A064[38] __attribute__((scn_string_literal)) = "涼「腕に力を入れるのが＆早すぎるな」\012";
static const char data_2A08A[22] __attribute__((scn_string_literal)) = "涼「ああ、分かった」\012";
static const char data_2A0A0[44] __attribute__((scn_string_literal)) = "福原「じゃあ、まず俺のを＆見てくださいよ」\012";
static const char data_2A0CC[10] __attribute__((scn_string_literal)) = "E1017A037";
static const char data_2A0D6[10] __attribute__((scn_string_literal)) = "E1017A038";
static const char data_2A0E0[10] __attribute__((scn_string_literal)) = "E1017A039";
static const char data_2A0EA[10] __attribute__((scn_string_literal)) = "E1017A040";
static const char data_2A0F4[10] __attribute__((scn_string_literal)) = "E1017A038";
static const char data_2A0FE[10] __attribute__((scn_string_literal)) = "E1017A050";
static const char data_2A108[10] __attribute__((scn_string_literal)) = "E1017B021";
static const char data_2A112[10] __attribute__((scn_string_literal)) = "E1017B022";
static const char data_2A11C[10] __attribute__((scn_string_literal)) = "E1017B023";
static const char data_2A126[10] __attribute__((scn_string_literal)) = "E1017B024";
static const char data_2A130[10] __attribute__((scn_string_literal)) = "E1017B025";
static const char data_2A13A[10] __attribute__((scn_string_literal)) = "E1017A015";
static const char data_2A144[10] __attribute__((scn_string_literal)) = "E1017A042";
static const char data_2A14E[10] __attribute__((scn_string_literal)) = "E1017A017";
static const char data_2A158[10] __attribute__((scn_string_literal)) = "E1017B031";
static const char data_2A162[10] __attribute__((scn_string_literal)) = "E1017A044";
static const char data_2A16C[10] __attribute__((scn_string_literal)) = "E1017A023";
static const char data_2A176[10] __attribute__((scn_string_literal)) = "E1017A024";
static const char data_2A180[10] __attribute__((scn_string_literal)) = "E1017A025";
static const char data_2A18A[10] __attribute__((scn_string_literal)) = "E1017A027";
static const char data_2A194[10] __attribute__((scn_string_literal)) = "E1017A028";
static const char data_2A19E[10] __attribute__((scn_string_literal)) = "E1017A045";
static const char data_2A1A8[10] __attribute__((scn_string_literal)) = "E1017A046";
static const char data_2A1B2[10] __attribute__((scn_string_literal)) = "E1017A047";
static const char data_2A1BC[10] __attribute__((scn_string_literal)) = "E1017A048";
static const char data_2A1C6[10] __attribute__((scn_string_literal)) = "E1017B043";
static const char data_2A1D0[10] __attribute__((scn_string_literal)) = "E1017A006";
static const char data_2A1DA[10] __attribute__((scn_string_literal)) = "E1017B053";
static const char data_2A1E4[10] __attribute__((scn_string_literal)) = "E1017B054";
static const char data_2A1EE[10] __attribute__((scn_string_literal)) = "E1017A031";
static const char data_2A1F8[10] __attribute__((scn_string_literal)) = "E1017B038";
static const char data_2A202[10] __attribute__((scn_string_literal)) = "E1017B044";
static const char data_2A20C[10] __attribute__((scn_string_literal)) = "E1017B045";
static const char data_2A216[10] __attribute__((scn_string_literal)) = "E1017A032";
static const char data_2A220[10] __attribute__((scn_string_literal)) = "E1017B038";
static const char data_2A22A[10] __attribute__((scn_string_literal)) = "E1017B046";
static const char data_2A234[10] __attribute__((scn_string_literal)) = "E1017A001";
static const char data_2A23E[10] __attribute__((scn_string_literal)) = "E1017B049";
static const char data_2A248[10] __attribute__((scn_string_literal)) = "E1017A035";
static const char data_2A252[10] __attribute__((scn_string_literal)) = "E1017B013";
static const char data_2A25C[10] __attribute__((scn_string_literal)) = "E1017B051";
static const char data_2A266[10] __attribute__((scn_string_literal)) = "E1017B054";
static const char data_2A270[10] __attribute__((scn_string_literal)) = "E1017B055";
static const char data_2A27A[6] __attribute__((scn_string_literal)) = "E1040";
static const char data_2A280[10] __attribute__((scn_string_literal)) = "E1040A028";
static const char data_2A28A[10] __attribute__((scn_string_literal)) = "E1040A029";
static const char data_2A294[6] __attribute__((scn_string_literal)) = "E1040";
static const char data_2A29A[10] __attribute__((scn_string_literal)) = "E1040A030";
static const char data_2A2A4[10] __attribute__((scn_string_literal)) = "E1040A031";
static const char data_2A2AE[10] __attribute__((scn_string_literal)) = "E1040A032";
static const char data_2A2B8[8] __attribute__((scn_string_literal)) = "E1040";
static const Vector3 data_2A2C0 = {.x = -7.13f, .y = 1.0f, .z = -24.79f};
static const char data_2A2CC[13] __attribute__((scn_string_literal)) = "e1shugyo.snd";
static const char data_2A2D9[10] __attribute__((scn_string_literal)) = "E1040A001";
static const char data_2A2E3[10] __attribute__((scn_string_literal)) = "E1040A003";
static const char data_2A2ED[10] __attribute__((scn_string_literal)) = "E1040A009";
static const char data_2A2F7[10] __attribute__((scn_string_literal)) = "E1040A010";
static const char data_2A301[10] __attribute__((scn_string_literal)) = "E1040A012";
static const char data_2A30B[10] __attribute__((scn_string_literal)) = "E1040A013";
static const char data_2A315[10] __attribute__((scn_string_literal)) = "E1040A014";
static const char data_2A31F[10] __attribute__((scn_string_literal)) = "E1040A018";
static const char data_2A329[10] __attribute__((scn_string_literal)) = "E1040A020";
static const char data_2A333[10] __attribute__((scn_string_literal)) = "E1040A015";
static const char data_2A33D[10] __attribute__((scn_string_literal)) = "E1040A016";
static const char data_2A347[10] __attribute__((scn_string_literal)) = "E1040A017";
static const char data_2A351[6] __attribute__((scn_string_literal)) = "E1040";
static const char data_2A357[9] __attribute__((scn_string_literal)) = "E1040";
static const Vector3 data_2A360 = {.x = -7.15f, .y = 1.0f, .z = -20.68f};
static const char data_2A36C[10] __attribute__((scn_string_literal)) = "YGB_B.MT5";
static const char data_2A376[10] __attribute__((scn_string_literal)) = "YKA_B.MT5";
static const char data_2A380[16] __attribute__((scn_string_literal)) = "battle_1.snd";
static const Vector3 data_2A390 = {.x = -11.1987f, .y = 0.9f, .z = -16.3866f};
static const Vector3 data_2A39C = {.x = -3.5184f, .y = 1.2f, .z = -28.564899f};
static const uint32_t data_2A3A8 = 1;
static const float data_2A3AC[3] = {-8.0f, -22.5f, 1.0f};
static const int32_t data_2A3B8 = 196608;
static const char data_2A3BC[11] __attribute__((scn_string_literal)) = "bgm109.snd";
static const char data_2A3C7[45] = "bgm009.snd\000\000\000\232\231aA\000\000\322B\315\314L=\000\000\000\377\000\000\000\000\000\000\000\000H:4\273H:4\273";
static const uint32_t data_2A3F4 = 380;
static const int32_t data_2A3F8[6] = {1, 390, 0, 1018, 0, -1};
static const uint32_t data_2A410 = 510;
static const int32_t data_2A414[6] = {1, 525, 0, 1020, 0, -1};
static const uint32_t data_2A42C = 150;
static const int32_t data_2A430[6] = {1, 160, 0, 1019, 0, -1};
static const uint32_t data_2A448 = 530;
static const int32_t data_2A44C[6] = {1, 970, 0, 1014, 0, -1};
static const uint32_t data_2A464 = 911;
static const int32_t data_2A468[6] = {1, 155, 0, 1015, 0, -1};
static const char data_2A480[7] __attribute__((scn_string_literal)) = "FREE02";
static const char data_2A487[7] __attribute__((scn_string_literal)) = "FREE03";
static const char data_2A48E[7] __attribute__((scn_string_literal)) = "FREE04";
static const char data_2A495[7] __attribute__((scn_string_literal)) = "FREE01";
static const uint32_t data_2A49C = 0x41C00000;
static const float data_2A4A0[2] = {0.0f, -33.0f};
static const uint32_t data_2A4A8 = 0x41A00000;
static const float data_2A4AC[2] = {0.0f, 18.0f};
static const uint32_t data_2A4B4 = 0xC1B80000;
static const float data_2A4B8[2] = {0.0f, 13.0f};
static const uint32_t data_2A4C0 = 0x40E00000;
static const float data_2A4C4[2] = {0.0f, 12.0f};
static const uint8_t data_2A4CC[8] = {0};
static const float data_2A4D4 = -35.0f;
static const uint8_t data_2A4D8[8] = {0};
static const float data_2A4E0 = -35.0f;
static const uint32_t data_2A4E4 = 0x42300000;
static const float data_2A4E8[2] = {0.0f, -24.0f};
static const uint32_t data_2A4F0 = 0xC1A00000;
static const float data_2A4F4[2] = {0.0f, 20.0f};
static const uint32_t data_2A4FC = 0x3F800000;
static const float data_2A500[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.8f, 0.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint32_t data_2A584 = 0x3F800000;
static const float data_2A588[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.8f, 0.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint32_t data_2A60C = 0x3F800000;
static const float data_2A610[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.8f, 0.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint32_t data_2A694 = 0x3F800000;
static const float data_2A698[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint8_t data_2A71C[80] = {0};
typedef struct Row_2A76C { float w0; float w1; float w2; int32_t w3; } Row_2A76C;
static const Row_2A76C data_2A76C[3] = {
  {0.8f, 0.8f, 0.8f, 0},
  {0.0f, 0.0f, 0.0f, 0},
  {0.0f, 0.0f, 0.0f, 0}
};
static const int32_t data_2A79C[2] = {0, 0};
static const uint8_t data_2A7A4[96] = {0};
static const float data_2A804[10] = {1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f};
static const uint32_t data_2A82C = 0x3F800000;
static const float data_2A830[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint32_t data_2A8B4 = 0x3F800000;
static const float data_2A8B8[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const char data_2A93C[12] __attribute__((scn_string_literal)) = "system1.snd";
static const char data_2A948[11] __attribute__((scn_string_literal)) = "amb036.snd";
static const char data_2A953[11] __attribute__((scn_string_literal)) = "bgm063.snd";
static const char data_2A95E[13] __attribute__((scn_string_literal)) = "f1doujou.snd";
static const char data_2A96B[13] __attribute__((scn_string_literal)) = "battle_1.snd";
static const char data_2A978[12] __attribute__((scn_string_literal)) = "system1.snd";
static const char data_2A984[11] __attribute__((scn_string_literal)) = "amb036.snd";
static const char data_2A98F[11] __attribute__((scn_string_literal)) = "bgm012.snd";
static const char data_2A99A[13] __attribute__((scn_string_literal)) = "f1doujou.snd";
static const char data_2A9A7[13] __attribute__((scn_string_literal)) = "battle_1.snd";
static const char data_2A9B4[12] __attribute__((scn_string_literal)) = "system1.snd";
static const char data_2A9C0[11] __attribute__((scn_string_literal)) = "amb036.snd";
static const char data_2A9CB[11] __attribute__((scn_string_literal)) = "bgm009.snd";
static const char data_2A9D6[13] __attribute__((scn_string_literal)) = "f1doujou.snd";
static const char data_2A9E3[13] __attribute__((scn_string_literal)) = "battle_1.snd";
static const char data_2A9F0[16] __attribute__((scn_string_literal)) = "/scene/01/JHD0/";
static const char data_2AA00[5] __attribute__((scn_string_literal)) = "JHD0";
static const char data_2AA05[13] __attribute__((scn_string_literal)) = "HMOT_YKA.BIN";
static const char data_2AA12[16] __attribute__((scn_string_literal)) = "/scene/01/JHD0/";
static const char data_2AA22[5] __attribute__((scn_string_literal)) = "MPSN";
static const char data_2AA27[16] __attribute__((scn_string_literal)) = "/scene/01/JHD0/";
static const char data_2AA37[5] __attribute__((scn_string_literal)) = "MPSU";
static const char data_2AA3C[16] __attribute__((scn_string_literal)) = "/scene/01/JHD0/";
static const char data_2AA4C[5] __attribute__((scn_string_literal)) = "MSKA";
static const char data_2AA51[16] __attribute__((scn_string_literal)) = "/scene/01/JHD0/";
static const char data_2AA61[5] __attribute__((scn_string_literal)) = "TGMA";
static const char data_2AA66[16] __attribute__((scn_string_literal)) = "/scene/01/JHD0/";
static const char data_2AA76[5] __attribute__((scn_string_literal)) = "KAKG";
static const char data_2AA7B[5] __attribute__((scn_string_literal)) = "KAKG";
static const char data_2AA80[1] __attribute__((scn_string_literal)) = "";
static const char data_2AA81[5] __attribute__((scn_string_literal)) = "KAKG";
static const char data_2AA86[1] __attribute__((scn_string_literal)) = "";
static const char data_2AA87[5] __attribute__((scn_string_literal)) = "KAKG";
static const char data_2AA8C[1] __attribute__((scn_string_literal)) = "";
static const char data_2AA8D[5] __attribute__((scn_string_literal)) = "KAKG";
static const char data_2AA92[1] __attribute__((scn_string_literal)) = "";
static const char data_2AA93[11] __attribute__((scn_string_literal)) = "M_MOBJ.BIN";
static const char data_2AA9E[14] __attribute__((scn_string_literal)) = "scene/01/JOMO";
static const char data_2AAAC[] __attribute__((scn_string_literal)) = "SCRTEST.BIN";
static const Vector3 data_2AAB8 = {.x = -0.78f, .y = 0.0f, .z = -16.476601f};
static const Vector3 data_2AAC4 = {.x = -14.0f, .y = 0.0f, .z = -28.671101f};
static const char data_2AAD0[6] __attribute__((scn_string_literal)) = "YKC_M";
static const char data_2AAD6[6] __attribute__((scn_string_literal)) = "E1011";
static const char data_2AADC[7] __attribute__((scn_string_literal)) = "SA1116";
static const char data_2AAE3[7] __attribute__((scn_string_literal)) = "A0125A";
static const char data_2AAEA[6] __attribute__((scn_string_literal)) = "FUK_M";
static const char data_2AAF0[6] __attribute__((scn_string_literal)) = "A0139";
static const char data_2AAF6[6] __attribute__((scn_string_literal)) = "FUB_M";
static const char data_2AAFC[5] __attribute__((scn_string_literal)) = "TGMA";
static const char data_2AB01[1] __attribute__((scn_string_literal)) = "";
static const char data_2AB02[11] __attribute__((scn_string_literal)) = "bgm009.snd";
static const char data_2AB0D[5] __attribute__((scn_string_literal)) = "TGMA";
static const char data_2AB12[16] __attribute__((scn_string_literal)) = "/scene/01/JHD0/";
static const char data_2AB22[5] __attribute__((scn_string_literal)) = "MPM2";
static const char data_2AB27[16] __attribute__((scn_string_literal)) = "/scene/01/JHD0/";
static const char data_2AB37[5] __attribute__((scn_string_literal)) = "MPM1";
static const char data_2AB3C[5] __attribute__((scn_string_literal)) = "MPM2";
static const char data_2AB41[1] __attribute__((scn_string_literal)) = "";
static const char data_2AB42[5] __attribute__((scn_string_literal)) = "MPM1";
static const char data_2AB47[1] __attribute__((scn_string_literal)) = "";
static const char data_2AB48[5] __attribute__((scn_string_literal)) = "MPM2";
static const char data_2AB4D[7] __attribute__((scn_string_literal)) = "MPM1";
static const Vector4 data_2AB54 = {.x = 1.0f, .y = 0.2f, .z = 0.2f, .w = 0.2f};
static const float data_2AB64[7] = {0.3f, 0.3f, 0.3f, 0.3f, 0.1f, 55.0f, 0.24f};
static const int32_t data_2AB80 = -14077627;
static const Vector4 data_2AB84 = {.x = 1.0f, .y = 0.2f, .z = 0.2f, .w = 0.2f};
static const uint32_t data_2AB94 = 0xC17CCCCD;
static const float data_2AB98[41] = {
  -13.7f, -15.9f, -30.6f, 2.0f, -30.6f, 2.0f, -13.7f, 0.0f, 7.0f, -6.75f, 3.0f, -22.5f, 9.0f, 3.89f, 0.83f, -5.75f, 0.83f, -6.08f, -11.99f, 3.88f,
  -12.0f, 0.0f, 2.75f, -0.75f, 1.4f, -5.0f, 6.0f, 1.02f, 9.46f, 0.91f, 12.5f, -3.0f, 12.5f, -3.0f, 9.5f, 0.0f, 2.5f, -0.85f, 1.25f, 11.0f, 3.0f
};
static const FogColourData data_2AC3C = {.red = 0.1f, .green = 360.0f, .blue = 1.14f, .colour = 0xFF17191BU};
static const Vector4 data_2AC4C = {.x = 1.0f, .y = 0.16f, .z = 0.12f, .w = 0.0f};
static const Vector4 data_2AC5C = {.x = 1.0f, .y = 0.3f, .z = 0.3f, .w = 0.3f};
static const FogColourData data_2AC6C = {.red = 9.0f, .green = 800.0f, .blue = 0.73f, .colour = 0xFFE7C7FFU};
static const uint32_t data_2AC7C[4] = {0, 0, 0x912218D8U, 0x912218D8U};
static const Vector4 data_2AC8C = {.x = 1.0f, .y = 0.3f, .z = 0.3f, .w = 0.3f};
static const char data_2AC9C[6] __attribute__((scn_string_literal)) = "A0125";
static const char data_2ACA2[11] __attribute__((scn_string_literal)) = "M_0125.BIN";
static const char data_2ACAD[6] __attribute__((scn_string_literal)) = "A0125";
static const char data_2ACB3[6] __attribute__((scn_string_literal)) = "01KAK";
static const char data_2ACB9[12] __attribute__((scn_string_literal)) = "M_01KAK.BIN";
static const char data_2ACC5[6] __attribute__((scn_string_literal)) = "YKE_M";
static const char data_2ACCB[12] __attribute__((scn_string_literal)) = "M_01CRY.BIN";
static const char data_2ACD7[11] __attribute__((scn_string_literal)) = "bgm049.snd";
static const char data_2ACE2[13] __attribute__((scn_string_literal)) = "a1_inet1.snd";
static const char data_2ACEF[7] __attribute__((scn_string_literal)) = "A01124";
static const char data_2ACF6[12] __attribute__((scn_string_literal)) = "M_01SOR.BIN";
static const char data_2AD02[6] __attribute__((scn_string_literal)) = "01SOR";
static const char data_2AD08[13] __attribute__((scn_string_literal)) = "a1_fukay.snd";
static const char data_2AD15[6] __attribute__((scn_string_literal)) = "YKE_M";
static const char data_2AD1B[12] __attribute__((scn_string_literal)) = "M_01DIS.BIN";
static const char data_2AD27[13] __attribute__((scn_string_literal)) = "seqdataD.bin";
static const char data_2AD34[14] __attribute__((scn_string_literal)) = "scene/01/JHD0";
static const char data_2AD42[14] __attribute__((scn_string_literal)) = "a1_houha.snd";
static const Vector3 data_2AD50 = {.x = 2.8079f, .y = 0.0f, .z = 9.2562f};
static const Vector3 data_2AD5C = {.x = 9.8262f, .y = 0.0f, .z = 2.1434f};
static const Vector3 data_2AD68 = {.x = 12.9789f, .y = 0.0f, .z = -13.1704f};
static const Vector3 data_2AD74 = {.x = -0.4989f, .y = 0.0f, .z = -30.642f};
static const Vector3 data_2AD80 = {.x = -0.78f, .y = 0.0f, .z = -16.476601f};
static const Vector3 data_2AD8C = {.x = -14.0f, .y = 0.0f, .z = -28.671101f};
static const Vector3 data_2AD98 = {.x = 9.25f, .y = 0.0f, .z = -30.33f};
static const Vector3 data_2ADA4 = {.x = 6.5f, .y = 0.0f, .z = -27.0f};
static const char data_2ADB0[16] __attribute__((scn_string_literal)) = "/scene/01/JHD0/";
static const char data_2ADC0[5] __attribute__((scn_string_literal)) = "MPS1";
static const char data_2ADC5[16] __attribute__((scn_string_literal)) = "/scene/01/JHD0/";
static const char data_2ADD5[5] __attribute__((scn_string_literal)) = "MPS2";
static const char data_2ADDA[16] __attribute__((scn_string_literal)) = "/scene/01/JHD0/";
static const char data_2ADEA[5] __attribute__((scn_string_literal)) = "MPS3";
static const char data_2ADEF[16] __attribute__((scn_string_literal)) = "/scene/01/JHD0/";
static const char data_2ADFF[5] __attribute__((scn_string_literal)) = "MPS4";
static const char data_2AE04[16] __attribute__((scn_string_literal)) = "/scene/01/JHD0/";
static const char data_2AE14[5] __attribute__((scn_string_literal)) = "MPS4";
static const char data_2AE19[16] __attribute__((scn_string_literal)) = "/scene/01/JHD0/";
static const char data_2AE29[5] __attribute__((scn_string_literal)) = "MPS5";
static const char data_2AE2E[16] __attribute__((scn_string_literal)) = "/scene/01/JHD0/";
static const char data_2AE3E[5] __attribute__((scn_string_literal)) = "MPS5";
static const char data_2AE43[16] __attribute__((scn_string_literal)) = "/scene/01/JHD0/";
static const char data_2AE53[5] __attribute__((scn_string_literal)) = "MPS6";
static const char data_2AE58[16] __attribute__((scn_string_literal)) = "/scene/01/JHD0/";
static const char data_2AE68[5] __attribute__((scn_string_literal)) = "MPS6";
static const char data_2AE6D[16] __attribute__((scn_string_literal)) = "/scene/01/JHD0/";
static const char data_2AE7D[5] __attribute__((scn_string_literal)) = "MPS6";
static const char data_2AE82[16] __attribute__((scn_string_literal)) = "/scene/01/JHD0/";
static const char data_2AE92[38] = "MPS7\000\000\236\002\240\002\242\002\245\002\254\002\252\002\250\002\000\003\370\000\372\000\365\000\367\000\366\000\373\000\371\000\000\003";
static const char data_2AEB8[61] __attribute__((scn_string_literal)) = "WAZA Warning : handle is not NULL, and create same handle !\012";
static const char data_2AEF5[30] __attribute__((scn_string_literal)) = "POS POS_RYOU_0 = %f, %f, %f.\012";
static const char data_2AF13[30] __attribute__((scn_string_literal)) = "POS POS_KAIS_0 = %f, %f, %f.\012";
static const char data_2AF31[29] __attribute__((scn_string_literal)) = "define ANG_RYOU_0 DEG(%d.0)\012";
static const char data_2AF4E[29] __attribute__((scn_string_literal)) = "define ANG_KAIS_0 DEG(%d.0)\012";
static const char data_2AF6B[4] __attribute__((scn_string_literal)) = "%s\012";
static const char data_2AF6F[37] __attribute__((scn_string_literal)) = "Konnna Error ha Detekonai hazu : %d\012";
static const Vector3 data_2AF94 = {.x = 1.0f, .y = 1.0f, .z = 1.0f};
static const Vector3i data_2AFA0 = {.x = 0, .y = 0, .z = 0};
static const char data_2AFAC[14] __attribute__((scn_string_literal)) = "Entry Model!\012";
static const char data_2AFBA[38] __attribute__((scn_string_literal)) = "WZ ModoriMove Limit Over !!!!!!!!!!!\012";
static const char data_2AFE0[30] __attribute__((scn_string_literal)) = "WazaDen: Load PackFile %s:%s\012";
static const char data_2AFFE[34] __attribute__((scn_string_literal)) = "WazaDen: Now Loading PackFile.\012";

__attribute__((scn_string_bindings(0, data_25048)))
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
    while (*(&task_C + arg5) & 4) {
      SCNF_WaitFrames_0000(1);
    }
    arg2 = Script_KaidanUtils(CAM_STAIR_QUERY_GUIDE_ENTRY, arg0, task_0, (const void *)task_4, (const void *)task_8, task_20);
    if (arg2 >= 0) {
      Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_2);
      Script_ccowUtils(arg0, 9);
      if (*(&task_C + arg5) & 0x800) {
        Script_scenUtils(SCENARIO_CMD_INIT_SCNT);
      }
      (void)((local_CA = 0) & 1);
      local_E4 = arg2 * 6;
      *(int32_t *)&local_F4[1] = Script_ScenDataUtil((void *)task_4, 4, local_E4 + 1);
      *(int32_t *)&local_F4[0] = Script_ScenDataUtil((void *)task_4, 4, local_E4);
      *(int32_t *)&local_F4[2] = Script_ScenDataUtil((void *)task_4, 4, local_E4 + 2);
      local_DC = Script_ScenDataUtil((void *)task_4, 4, local_E4 + 4);
      local_E8[1] = Script_ScenDataUtil((void *)task_4, 4, local_E4 + 3);
      local_E0 = Script_ScenDataUtil((void *)task_4, 4, local_E4 + 5);
      local_D8 = Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, local_E8[1]);
      local_D4 = Script_MathUtils(SCRIPT_MATH_COS_FIXED16, local_E8[1]);
      script_20A3(arg0, arg5, arg2);
      *(&task_18 + arg5) = arg2 + 1;
      *((int8_t *)&task_44 + arg5) = 3;
      script_2D34(arg0, arg5, local_DC);
      Script_CharHandlers(arg0, 2, 0, 16);
      (void)((local_CD = -((local_CB = 0) != 0)) & 1);
      if (local_DC & 0xF00 & 0x400) {
        script_0A25(1, arg2);
        if (local_DC & 0xF000 & 0x1000) {
          (void)((task_4C = -1) & 1);
          while (task_4C & 2) {
            SCNF_WaitFrames_0000(1);
          }
        }
      }
      if (local_DC & 0x100) {
        (void)((local_CA = -1) & 1);
      }
      if (*(&task_C + arg5) & 0x400) {
        Script_MotStepUtil(arg0, 0);
      }
      if ((local_DC & 0xF) <= 4) {
        Script_ccowSetFlag80(arg0, 1);
        if ((*(&task_C + arg5) & 8) == 0) {
          if (local_DC & 0x1000000) {
            Script_scenCreateTask(script_24C5, 2, arg0, 1);
          } else {
            Script_scenCreateTask(script_24C5, 2, arg0, 1);
          }
        }
        if (local_DC & 0x2000) {
          task_49 = 0;
        } else {
          uint8_t unused_C8[2];
          int32_t local_C4;
          local_C4 = *(&task_10 + arg5);
          switch (local_C4) {
            case 0:
              task_49 = 0;
              break;
            case 1:
              task_49 = 1;
              break;
            case 3:
              task_49 = 1;
              break;
            case 2:
              if (Script_ryoControl(PCCTL_QueryIsRunningRequest)) {
                task_49 = 1;
              } else {
                task_49 = 0;
              }
              break;
          }
        }
        if (task_49) {
          local_F4[0] = *local_F4 + local_D8 * -0.1f;
          local_F4[2] = local_F4[2] + local_D4 * -0.1f;
        } else {
          local_F4[0] = *local_F4 + local_D8 * 0.16f;
          local_F4[2] = local_F4[2] + local_D4 * 0.16f;
        }
        local_CF = script_1B07(arg0, local_E8[1], *local_F4, local_F4[2], arg5, local_DC, local_E0);
        if (local_CA & 4) {
          Script_KaidanUtils(CAM_STAIR_APPLY_GUIDE_FACING, arg0, arg2, (const void *)task_4);
        }
        (void)((local_CC = -1) & 1);
        Script_ccowSetFlag80(arg0, 2);
        Script_ccowUnknown(arg0, 1, 3.0f);
      } else {
        if ((local_DC & 0xF) <= 12) {
          if ((*(&task_C + arg5) & 8) == 0) {
            Script_scenCreateTask(script_24C5, 2, arg0, 0);
          }
          if (local_DC & 0x2000) {
            task_49 = 0;
          } else {
            uint8_t unused_C8[2];
            int32_t local_C4;
            local_C4 = *(&task_10 + arg5);
            switch (local_C4) {
              case 0:
                task_49 = 0;
                break;
              case 1:
                task_49 = 1;
                break;
              case 3:
                task_49 = 1;
                break;
              case 2:
                if (Script_ryoControl(PCCTL_QueryIsRunningRequest)) {
                  task_49 = 1;
                } else {
                  task_49 = 0;
                }
                break;
            }
          }
          if (task_49) {
            local_F4[0] = *local_F4 + local_D8 * -0.25f;
            local_F4[2] = local_F4[2] + local_D4 * -0.25f;
          } else {
            local_F4[0] = *local_F4 + local_D8 * 0.15f;
            local_F4[2] = local_F4[2] + local_D4 * 0.15f;
          }
          local_CF = local_E0 - script_1D96(arg0, local_E8[1], *local_F4, local_F4[2], arg5, local_DC, local_E0);
          if (local_CA & 4) {
            Script_KaidanUtils(CAM_STAIR_APPLY_GUIDE_FACING, arg0, arg2, (const void *)task_4);
          }
          Script_ccowSetFlag80(arg0, 2);
          Script_ccowUnknown(arg0, 1, 3.0f);
          (void)((local_CC = 0) & 0x80);
          goto L000525;
        }
      }
      L000525:;
      if ((local_E0 == 1) & ((local_DC & 0xF) <= 4)) {
        if (*((int8_t *)&task_47 + arg5) == 1) {
          Script_motmToggleFlags(arg0, 1, 0x80);
        }
        if (task_49) {
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
          local_CE = task_49;
          if (local_D0) {
            local_D0 = 0;
          } else if (local_DC & 0x2000) {
            task_49 = 0;
          } else {
            uint8_t unused_C8[2];
            int32_t local_C4;
            local_C4 = *(&task_10 + arg5);
            switch (local_C4) {
              case 0:
                task_49 = 0;
                break;
              case 1:
                task_49 = 1;
                break;
              case 3:
                task_49 = 1;
                break;
              case 2:
                if (Script_ryoControl(PCCTL_QueryIsRunningRequest)) {
                  task_49 = 1;
                } else {
                  task_49 = 0;
                }
                break;
            }
          }
          if (local_CB & 8) {
          } else {
            if (*(&task_C + arg5) & 0x100) {
              if (*(&task_C + arg5) & 2) {
                if ((Script_GetControllerInput(0) & 0x2000) > 0) {
                  (void)((local_CD = -1) & 4);
                }
              }
            }
            if (local_CC & 0x10) {
              local_CF = local_CF + script_0E0C(arg0, local_CF, local_E0, arg5, local_DC, arg2, local_CE - task_49);
            } else {
              local_CF = local_CF - script_12FC(arg0, local_CF, arg5, local_DC, arg2, local_CE - task_49);
            }
            if ((local_CF > 0) & (local_CF < local_E0)) {
              if (!(local_CD & 0x20)) goto L000767;
            }
          }
          L000767:;
        }
        if (local_D0) {
          if (((*(&task_C + arg5) & 0x10) == 0) & ((local_DC & 0xF) < 2)) {
            if (*((int8_t *)&task_47 + arg5) == 1) {
              Script_motmToggleFlags(arg0, 1, 0x80);
            }
            if (task_49) {
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
      if ((task_4A == 1) | (task_4A == 2)) {
        task_4A = 3;
      }
      if ((*(&task_C + arg5) & 2) > 0) {
        if (task_49) {
          Script_SetCharaMotionPlaybackRate(arg0, 1.2f);
        } else {
          Script_SetCharaMotionPlaybackRate(arg0, 1.3f);
        }
        if (task_50 & 4) {
          Script_sprintf("Kaidan : ");
          Script_ryoControl(RYO_RestoreControl);
          (void)((task_50 = 0) & 4);
        }
        script_2E09(arg0, arg5, local_DC);
      }
      Script_ccowUnknown(arg0, 2, 0.07f);
      Script_ccowUtils(arg0, 10);
      if (*((int8_t *)&task_45 + arg5) != 2) {
        *((int8_t *)&task_45 + arg5) = 0;
      }
      *((int8_t *)&task_46 + arg5) = 0;
      *((int8_t *)&task_44 + arg5) = 0;
      *(&task_18 + arg5) = -1;
      if (*(&task_C + arg5) & 0x800) {
        Script_scenUtils(SCENARIO_CMD_UPDATE_TIMER);
      }
    } else {
      (void)((task_50 = 0) & 1);
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_0932(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
  int32_t local_FC;
  int32_t local_F8;
  *(&task_C + arg5) = arg4;
  *(&task_14 + arg5) = arg0;
  *(&task_18 + arg5) = -1;
  *(&task_10 + arg5) = arg1;
  *((int8_t *)&task_45 + arg5) = 0;
  *((int8_t *)&task_46 + arg5) = 0;
  Script_sprintf("========== initializing kaidan\012");
  if (arg3) {
    (void)((task_4B = 0) & 1);
    task_20 = 0x4000;
    task_48 = 1;
    task_4 = arg3;
    (void)((task_50 = 0) & 1);
    if (arg2) {
      Script_sprintf("please ENDOFKDATA use can't start shuki kaidan\012");
      task_0 = arg2;
    } else {
      Script_CycleManagerUtils(CYCLE_CMD_SET_GLOBAL_CYCLE_PTR, task_4, &task_18);
      task_0 = local_F8 = 0;
      while ((local_FC = Script_ScenDataUtil((void *)task_4, 4, local_F8)) != 0x47C35000) {
        task_0 = task_0 + 1;
        local_F8 = local_F8 + 6;
      }
    }
  } else {
    task_48 = task_48 + 1;
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
  while ((local_F8 = Script_ScenDataUtil((void *)task_8, 4, local_FC)) != 3) {
    local_F0 = local_F8;
    if (local_F0 == 0) {
      local_FC = local_FC + 3;
    } else if (local_F0 == 1) {
      if (arg0) {
        if (Script_ScenDataUtil((void *)task_8, 4, local_FC + 1) == arg1) {
          local_F4 = Script_ScenDataUtil((void *)task_8, 4, local_FC + 2);
          if (local_F4 < 4) {
            local_F4 = task_24[local_F4];
            break;
          } else {
            Script_PreserveAccumulator_012C(0, "Kaidan Function Error 1\012");
          }
        }
      }
      local_FC = local_FC + 3;
    } else if (local_F0 == 2) {
      if (arg0 == 0) {
        if (Script_ScenDataUtil((void *)task_8, 4, local_FC + 1) == arg1) {
          local_F4 = Script_ScenDataUtil((void *)task_8, 4, local_FC + 2);
          if (local_F4 < 4) {
            local_F4 = task_24[local_F4];
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
    task_49 = 0;
  } else {
    local_F8 = *(&task_10 + arg1);
    switch (local_F8) {
      case 0:
        task_49 = 0;
        break;
      case 1:
        task_49 = 1;
        break;
      case 3:
        task_49 = 1;
        break;
      case 2:
        if (Script_ryoControl(PCCTL_QueryIsRunningRequest)) {
          task_49 = 1;
        } else {
          task_49 = 0;
        }
        break;
    }
  }
  if (task_49) {
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
    task_49 = 0;
  } else {
    local_F8 = *(&task_10 + arg1);
    switch (local_F8) {
      case 0:
        task_49 = 0;
        break;
      case 1:
        task_49 = 1;
        break;
      case 3:
        task_49 = 1;
        break;
      case 2:
        if (Script_ryoControl(PCCTL_QueryIsRunningRequest)) {
          task_49 = 1;
        } else {
          task_49 = 0;
        }
        break;
    }
  }
  if (task_49) {
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
  task_34 = 0;
  if (task_49) {
    local_FF = 4;
    if (arg1 < arg2 - 3) {
      Script_motmToggleFlags(arg0, 2, 0x40);
      if (arg6) {
        if (*((int8_t *)&task_47 + arg3) == 1) {
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
      if ((*(&task_C + arg3) & 8) == 0) {
        task_4A = 2;
      }
      script_2DB9(arg0, arg3, arg4);
      if (arg4 & 0xF00 & 0x800) {
        script_0A25(0, arg5);
        if (arg4 & 0xF000 & 0x1000) {
          (void)((task_4C = -1) & 1);
          while (task_4C & 2) {
            SCNF_WaitFrames_0000(1);
          }
        }
      } else if (arg2 <= arg1 + 2) {
        if (*((int8_t *)&task_47 + arg3) == 0) {
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
          if (*((int8_t *)&task_47 + arg3) == 1) {
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
        if (*((int8_t *)&task_47 + arg3) == 0) {
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
      if ((*(&task_C + arg3) & 8) == 0) {
        task_4A = 3;
      }
    }
  } else {
    local_FF = 2;
    if (arg1 < arg2 - 1) {
      if (arg6) {
        if (*((int8_t *)&task_47 + arg3) == 1) {
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
      if ((*(&task_C + arg3) & 8) == 0) {
        task_4A = 2;
      }
      script_2DB9(arg0, arg3, arg4);
      if (arg6) {
        if (*((int8_t *)&task_47 + arg3) == 1) {
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
          (void)((task_4C = -1) & 2);
          while (task_4C & 2) {
            SCNF_WaitFrames_0000(1);
          }
        }
      } else {
        if (arg2 == arg1) {
          if (*((int8_t *)&task_47 + arg3) == 0) {
            Script_motmToggleFlags(arg0, 1, 0x80);
          }
        } else {
          if (*((int8_t *)&task_47 + arg3) == 1) {
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
      if ((*(&task_C + arg3) & 8) == 0) {
        task_4A = 3;
      }
    }
  }
  return local_FF;
}

static int8_t script_12FC(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
  int8_t local_FF;
  if (task_49) {
    local_FF = 4;
    if (arg1 >= local_FF) {
      if (arg5) {
        if (*((int8_t *)&task_47 + arg2) == 1) {
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
      if (*((int8_t *)&task_45 + arg2) != 2) {
        *((int8_t *)&task_45 + arg2) = 0;
      }
      switch (arg1) {
        case 1:
          if (arg5) {
            if (*((int8_t *)&task_47 + arg2) == 1) {
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
            if (*((int8_t *)&task_47 + arg2) == 1) {
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
            if (*((int8_t *)&task_47 + arg2) == 1) {
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
      if ((*(&task_C + arg2) & 8) == 0) {
        task_4A = 2;
      }
      script_2DB9(arg0, arg2, arg3);
      if (arg3 & 0xF00 & 0x800) {
        script_0A25(0, arg4);
        if (arg3 & 0xF000 & 0x1000) {
          (void)((task_4C = -1) & 1);
          while (task_4C & 2) {
            SCNF_WaitFrames_0000(1);
          }
        }
      } else {
        if (arg1 <= 2) {
          if (*((int8_t *)&task_47 + arg2) == 1) {
            Script_motmToggleFlags(arg0, 1, 0x80);
          }
        } else {
          if (*((int8_t *)&task_47 + arg2) == 0) {
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
      if ((*(&task_C + arg2) & 8) == 0) {
        task_4A = 3;
      }
    }
  } else {
    local_FF = 2;
    if (arg1 >= local_FF) {
      if (arg5) {
        if (*((int8_t *)&task_47 + arg2) == 1) {
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
      if (*((int8_t *)&task_45 + arg2) != 2) {
        *((int8_t *)&task_45 + arg2) = 0;
      }
      if (arg1) {
        if (arg5) {
          if (*((int8_t *)&task_47 + arg2) == 1) {
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
      if ((*(&task_C + arg2) & 8) == 0) {
        task_4A = 2;
      }
      if (arg3 & 0xF00 & 0x800) {
        script_0A25(0, arg4);
        if (arg3 & 0xF000 & 0x1000) {
          (void)((task_4C = -1) & 1);
          while (task_4C & 2) {
            SCNF_WaitFrames_0000(1);
          }
        }
        script_2DB9(arg0, arg2, arg3);
      } else {
        if (arg1) {
          if (*((int8_t *)&task_47 + arg2) == 1) {
            Script_motmToggleFlags(arg0, 1, 0x80);
          }
        } else {
          if (*((int8_t *)&task_47 + arg2) == 0) {
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
      if ((*(&task_C + arg2) & 8) == 0) {
        task_4A = 3;
      }
    }
  }
  return local_FF;
}

static void script_18C8(int32_t arg0, int32_t arg1, float arg2, float arg3, int32_t arg4, int32_t arg5) {
  Vector3 local_F4;
  float local_E8[3];
  if (arg5 & 0x2000) {
    task_49 = 0;
  } else {
    int32_t unused_D4[5];
    int32_t local_D0;
    local_D0 = *(&task_10 + arg4);
    switch (local_D0) {
      case 0:
        task_49 = 0;
        break;
      case 1:
        task_49 = 1;
        break;
      case 3:
        task_49 = 1;
        break;
      case 2:
        if (Script_ryoControl(PCCTL_QueryIsRunningRequest)) {
          task_49 = 1;
        } else {
          task_49 = 0;
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
  if (task_49) {
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
  task_34 = 0;
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
  if (task_49) {
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
      task_34 = -0x80000000;
      *((int8_t *)&task_47 + arg4) = 1;
    } else {
      *((int8_t *)&task_47 + arg4) = 0;
    }
    Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
    if (arg6 > 1) {
      Script_SetCharaMoveTargetEx(arg0, &local_F4, arg1, task_34 | 0x2CB, 0, -1.0f, 2);
      local_B8 = 2;
    } else {
      Script_SetCharaMoveTargetEx(arg0, &local_F4, arg1, task_34 | 0x2CB, 0, -1.0f, 2);
      local_B8 = arg6;
    }
  } else {
    local_F4.x = arg2;
    (&local_F4)->z = arg3;
    Script_EVT_GetPosition(arg0, 26, &local_DC, ENTITY_XFORM_WORLD_SPACE);
    Script_EVT_GetPosition(arg0, 33, &local_D0, ENTITY_XFORM_WORLD_SPACE);
    if (!((&local_DC)->y >= (&local_D0)->y)) {
      task_34 = -0x80000000;
      *((int8_t *)&task_47 + arg4) = 1;
    } else {
      *((int8_t *)&task_47 + arg4) = 0;
    }
    Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
    if (arg6 > 1) {
      Script_SetCharaMoveTargetEx(arg0, &local_F4, arg1, task_34 | 0x2C6, 0, -1.0f, 2);
      local_B8 = 1;
    } else {
      Script_SetCharaMoveTargetEx(arg0, &local_F4, arg1, task_34 | 0x2C6, 0, -1.0f, 2);
      local_B8 = arg6;
    }
  }
  while (Script_SetCharaXMPTMode(arg0, 2) != 0) {
    SCNF_WaitFrames_0000(1);
  }
  return local_B8;
}

__attribute__((scn_string_bindings(0, data_250E7)))
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
  task_34 = 0;
  if (task_49) {
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
      task_34 = -0x80000000;
      *((int8_t *)&task_47 + arg4) = 1;
    } else {
      *((int8_t *)&task_47 + arg4) = 0;
    }
    Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
    if (arg6 > 1) {
      Script_SetCharaMoveTargetEx(arg0, &local_F4, arg1, task_34 | 0x2F9 | 0x30000, 0, -1.0f, 2);
      local_B8 = 0;
    } else {
      Script_SetCharaMoveTargetEx(arg0, &local_F4, arg1, task_34 | 0x2F9 | 0x30000, 0, -1.0f, 2);
      local_B8 = arg6;
    }
  } else {
    local_F4.x = arg2;
    (&local_F4)->z = arg3;
    Script_EVT_GetPosition(arg0, 26, &local_DC, ENTITY_XFORM_WORLD_SPACE);
    Script_EVT_GetPosition(arg0, 33, &local_D0, ENTITY_XFORM_WORLD_SPACE);
    if (!((&local_DC)->y >= (&local_D0)->y)) {
      task_34 = -0x80000000;
      *((int8_t *)&task_47 + arg4) = 1;
    } else {
      *((int8_t *)&task_47 + arg4) = 0;
    }
    Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
    if (arg6 > 1) {
      Script_SetCharaMoveTargetEx(arg0, &local_F4, arg1, task_34 | 0x2C7 | 0x90000, 0, -1.0f, 2);
      local_B8 = 0;
    } else {
      Script_SetCharaMoveTargetEx(arg0, &local_F4, arg1, task_34 | 0x2C7 | 0x90000, 0, -1.0f, 2);
      local_B8 = arg6;
    }
  }
  while (Script_SetCharaXMPTMode(arg0, 2) != 0) {
    SCNF_WaitFrames_0000(1);
  }
  Script_ccowUtils(arg0, 1);
  script_2D34(arg0, arg4, arg5);
  if (*(&task_C + arg4) & 2) {
    if (Script_ryoControl(PCCTL_QueryShouldRun)) {
      Script_sprintf("Kaidan : ");
      Script_ryoControl(PCCTL_DisablePlayerControl);
      (void)((task_50 = -1) & 4);
    }
    Script_ccowUtils(arg0, 6);
  }
  return local_B8;
}

__attribute__((scn_string_bindings(1, data_2510D)))
static void script_20A3(int32_t arg0, int32_t arg1, int32_t arg2) {
  int32_t local_FC;
  int32_t local_F8;
  int8_t local_F7;
  (void)((local_F7 = 0) & 4);
  arg2 = arg2 & -2;
  local_F8 = Script_ScenDataUtil((void *)task_4, 4, arg2 * 6 + 4);
  local_FC = Script_ScenDataUtil((void *)task_4, 4, (arg2 + 1) * 6 + 4);
  local_FC = local_FC | local_F8;
  if (local_FC & -0x80000000) {
    while (local_FC & -0x80000000) {
      if (local_F7 & 0x80) {
        if (task_54) {
        } else {
          Script_sprintf("wait ... shuki shuki shuki\012");
        }
      } else {
        if (*(&task_C + arg1) & 2) {
          if (Script_ryoControl(PCCTL_QueryShouldRun)) {
            Script_sprintf("Kaidan : ");
            Script_ryoControl(PCCTL_DisablePlayerControl);
            (void)((task_50 = -1) & 4);
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
      task_54 = (task_54 + 1) & 0xF;
      local_F8 = Script_ScenDataUtil((void *)task_4, 4, arg2 * 6 + 4);
      local_FC = Script_ScenDataUtil((void *)task_4, 4, (arg2 + 1) * 6 + 4);
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
  if (~task_4B) {
    (void)((task_4B = -1) & 1);
    task_38 = Script_SetCameraSettings(CAM_UTILS_SET_DISTANCE_FROM_PLAYER, -1.0f);
    task_3C = Script_SetCameraSettings(CAM_UTILS_SET_HEIGHT_ABOVE_PLAYER, -1.0f);
    task_40 = Script_SetCameraSettings(CAM_UTILS_SET_MAX_Y_ANGLE, -1.0f);
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
    local_F4 = task_4A;
    if ((local_F4 == 0) | (local_F4 == 2) | (local_F4 == 4)) {
      break;
      goto L0023A3;
    }
    L0023A3:;
    local_F8 = task_40 + (arg0 - task_40) * (float)local_FC / 10.0f;
    Script_SetCameraSettings(CAM_UTILS_SET_MAX_Y_ANGLE, local_F8);
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  {
    int32_t local_F4;
    local_F4 = task_4A;
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
    local_F4 = task_4A;
    if ((local_F4 == 0) | (local_F4 == 2) | (local_F4 == 4)) {
      break;
      goto L002432;
    }
    L002432:;
    local_F8 = task_38 + (arg0 - task_38) * (float)local_FC / 50.0f;
    Script_SetCameraSettings(CAM_UTILS_SET_DISTANCE_FROM_PLAYER, local_F8);
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  {
    int32_t local_F4;
    local_F4 = task_4A;
    if ((local_F4 == 0) | (local_F4 == 2) | (local_F4 == 4)) {
    } else {
      Script_SetCameraSettings(CAM_UTILS_SET_DISTANCE_FROM_PLAYER, arg0);
    }
  }
  return;
}

static void script_248E(void) {
  Script_SetCameraSettings(6, 7);
  Script_SetCameraSettings(CAM_UTILS_SET_DISTANCE_FROM_PLAYER, task_38);
  Script_SetCameraSettings(CAM_UTILS_SET_HEIGHT_ABOVE_PLAYER, task_3C);
  Script_SetCameraSettings(CAM_UTILS_SET_MAX_Y_ANGLE, task_40);
  Script_KaidanUtils(CAM_STAIR_RESET_RETRACTION_LIMIT);
  (void)((task_4B = 0) & 2);
  return;
}

static int32_t script_24C5(int32_t arg0, int32_t arg1) {
  Vector3 local_F4;
  Vector3 local_E8;
  int8_t local_E7;
  int8_t local_E6;
  task_1C = arg0;
  task_4A = 1;
  local_E7 = -1;
  (void)((local_E6 = 0) & 2);
  Script_SetGameStateTo(AUTH_None);
  while (task_1C == arg0) {
    if (Script_GetAuthorState_And_SetCamPos(&local_F4, &local_E8) != 17) {
      if (task_4A != local_E7) {
        uint8_t unused_E4[2];
        int32_t local_E0;
        local_E0 = task_4A;
        if (local_E0 == 1) {
          (void)((local_E6 = -1) & 2);
          script_22D1(arg1);
        } else if (local_E0 == 2) {
          script_248E();
        } else if ((local_E0 == 0) | (local_E0 == 3)) {
          Script_SetCameraSettings(6, 7);
          task_1C = 0;
        } else {
          if (local_E0 == 4) goto L002572;
        }
        L002572:;
        local_E7 = task_4A;
      }
    } else {
      if (task_4A != local_E7) {
        uint8_t unused_E4[2];
        int32_t local_E0;
        local_E0 = task_4A;
        if ((local_E0 == 0) | (local_E0 == 3)) {
          Script_SetCameraSettings(6, 7);
          task_1C = 0;
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
  if (task_4A != 0) {
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
  while (local_FF < task_48) {
    if (arg0 == *(&task_14 + local_FF)) {
      local_F8 = *(&task_18 + local_FF);
    }
    local_FF = local_FF + 1;
  }
  (void)local_F8;
  return;
}

static void script_2643(int32_t arg0) {
  task_8 = arg0;
  return;
}

static void script_264D(int32_t arg0) {
  task_20 = arg0;
  return;
}

static void script_2657(void) {
  (void)task_20;
  return;
}

__attribute__((scn_string_bindings(0, data_25117)))
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

__attribute__((scn_string_bindings(0, data_2512F)))
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

__attribute__((scn_string_bindings(0, data_25147)))
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

__attribute__((scn_string_bindings(0, data_2515F)))
static void script_28EF(int32_t arg0, int32_t arg1) {
  Vector3 local_F4;
  Vector3i local_E8;
  float local_E4;
  float local_E0;
  float local_DC;
  if (*((int8_t *)&task_46 + arg1) == 0) {
    *((int8_t *)&task_46 + arg1) = 1;
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

__attribute__((scn_string_bindings(0, data_25177)))
static void script_29E5(int32_t arg0, int32_t arg1) {
  Vector3 local_F4;
  Vector3i local_E8;
  float local_E4;
  float local_E0;
  float local_DC;
  if (*((int8_t *)&task_46 + arg1) == 0) {
    *((int8_t *)&task_46 + arg1) = 1;
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

__attribute__((scn_string_bindings(0, data_2518F)))
static int32_t script_2ADB(int32_t arg0, int32_t arg1) {
  float local_FC;
  float local_F8;
  if (*((int8_t *)&task_45 + arg1)) {
    Script_sprintf("KAIDAN : Warning setY!\012");
  } else {
    *((int8_t *)&task_45 + arg1) = 1;
    if (task_49) {
      local_FC = local_F8 = -0.001f;
    } else {
      local_FC = local_F8 = 0.0f;
    }
    while (*((int8_t *)&task_45 + arg1) == 1) {
      if (task_49) {
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
    if (*((int8_t *)&task_45 + arg1) != 2) {
      for (;;) {
        if (local_FC >= 0.0f) {
          break;
        }
        if (*((int8_t *)&task_45 + arg1) == 2) {
          *((int8_t *)&task_45 + arg1) = 0;
          Script_KaidanSetY(arg0, 0.0f);
          break;
        }
        local_FC = local_FC + 0.004f;
        Script_KaidanSetY(arg0, local_FC);
        SCNF_WaitFrames_0000(1);
      }
    } else {
      *((int8_t *)&task_45 + arg1) = 0;
      Script_KaidanSetY(arg0, 0.0f);
    }
  }
  end:;
}

__attribute__((scn_string_bindings(0, data_251A7)))
static int32_t script_2C03(int32_t arg0, int32_t arg1) {
  float local_FC;
  float local_F8;
  if (*((int8_t *)&task_45 + arg1)) {
    Script_sprintf("KAIDAN : Warning setY!\012");
  } else {
    *((int8_t *)&task_45 + arg1) = 1;
    if (task_49) {
      local_FC = local_F8 = 0.16f;
    } else {
      local_FC = local_F8 = 0.08f;
    }
    while (*((int8_t *)&task_45 + arg1) == 1) {
      if (task_49) {
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
    if (*((int8_t *)&task_45 + arg1) != 2) {
      for (;;) {
        if (local_FC <= 0.0f) {
          break;
        }
        if (*((int8_t *)&task_45 + arg1) == 2) {
          *((int8_t *)&task_45 + arg1) = 0;
          Script_KaidanSetY(arg0, 0.0f);
          break;
        }
        local_FC = local_FC - 0.004f;
        Script_KaidanSetY(arg0, local_FC);
        SCNF_WaitFrames_0000(1);
      }
    } else {
      *((int8_t *)&task_45 + arg1) = 0;
      Script_KaidanSetY(arg0, 0.0f);
    }
  }
  end:;
}

__attribute__((scn_string_bindings(0, data_251BF)))
static void script_2D34(int32_t arg0, int32_t arg1, int32_t arg2) {
  if ((*(&task_C + arg1) & 2) > 0) {
    if (Script_ryoControl(PCCTL_QueryShouldRun)) {
      Script_sprintf("Kaidan : ");
      Script_ryoControl(PCCTL_DisablePlayerControl);
      (void)((task_50 = -1) & 4);
    }
    Script_ccowUtils(arg0, 6);
  }
  if (*(&task_C + arg1) & 0x20) {
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
  if (*(&task_C + arg1) & 0x20) {
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
  if (*(&task_C + arg1) & 0x20) {
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

static int32_t script_387C(int32_t arg0) {
  if (arg0 < 0) {
    return -arg0;
  } else {
    return arg0;
  }
}

static void script_389A(float arg0) {
  if (!(arg0 >= 0.0f)) {
    (void)((float)0 - arg0);
    return;
  } else {
    (void)arg0;
    return;
  }
}

static int32_t script_38B9(int32_t arg0, int32_t arg1) {
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

static void script_3931(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  int32_t local_FC;
  if (task_1BC != arg0) {
    Script_sprintf("Door Index = %d : call_type = %d : type = %d\012", task_A4[arg2], arg3, arg4);
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

static int32_t script_3EE3(int32_t arg0) {
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

static void script_3FB4(int32_t arg0, int32_t arg1, int32_t arg2) {
  script_CCD7(arg0, arg2, 0);
  return;
}

static void script_3FC4(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  float local_F8;
  float local_F4;
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
    local_FC = script_C46C(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
    if ((local_FC == 1) | (local_FC == 2)) {
      local_FC = 1;
    } else {
      local_FC = 0;
    }
    *(int32_t *)&local_F8 = Script_ScenDataUtil(data_257BC, 5, Script_ScenDataUtil(data_251CC, 1, arg3 * 40 + task_F4[arg2] * 4 + local_FC) * 9 + 5);
    *(int32_t *)&local_F4 = Script_ScenDataUtil(data_257BC, 5, Script_ScenDataUtil(data_251CC, 1, arg3 * 40 + task_F4[arg2] * 4 + local_FC) * 9 + 6);
    local_FC = script_C46C(arg0, arg1, 5, local_F8 / 100.0f, local_F4 / 100.0f, arg3, -1);
    script_7750(arg0, arg1, arg2, arg3, local_FC, 0);
  }
  script_CCD7(arg0, arg2, 0);
  return;
}

static void script_40C8(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  int32_t local_F8;
  float local_F4;
  float local_F0;
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
    local_FC = script_C46C(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
    *(int32_t *)&local_F4 = Script_ScenDataUtil(data_257BC, 5, Script_ScenDataUtil(data_251CC, 1, arg3 * 40 + task_F4[arg2] * 4 + local_FC) * 9 + 5);
    local_F4 = 90.0f - local_F4;
    *(int32_t *)&local_F0 = Script_ScenDataUtil(data_257BC, 5, Script_ScenDataUtil(data_251CC, 1, arg3 * 40 + task_F4[arg2] * 4 + local_FC) * 9 + 6);
    if ((Script_ScenDataUtil(task_5C, 4, task_A4[arg2] * 13 + 1) & 0x40) | (Script_ScenDataUtil(task_5C, 4, task_A4[arg2] * 13 + 1) & 0x80)) {
      if (Script_ScenDataUtil(task_5C, 4, task_A4[arg2] * 13 + 1) & 0x40) {
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
    local_FC = script_C46C(arg0, arg1, local_F8, local_F4 / (float)100, local_F0 / (float)100, arg3, task_A4[arg2]);
    script_7750(arg0, arg1, arg2, arg3, local_FC, 1);
  }
  script_CCD7(arg0, arg2, 0);
  return;
}

static void script_426A(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  float local_F8;
  float local_F4;
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
    local_FC = script_C46C(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
    if ((local_FC == 1) | (local_FC == 2)) {
      local_FC = 1;
    } else {
      local_FC = 0;
    }
    *(int32_t *)&local_F8 = Script_ScenDataUtil(data_257BC, 5, Script_ScenDataUtil(data_251CC, 1, arg3 * 40 + task_F4[arg2] * 4 + local_FC) * 9 + 5);
    *(int32_t *)&local_F4 = Script_ScenDataUtil(data_257BC, 5, Script_ScenDataUtil(data_251CC, 1, arg3 * 40 + task_F4[arg2] * 4 + local_FC) * 9 + 6);
    local_FC = script_C46C(arg0, arg1, 5, local_F8 / 100.0f, local_F4 / 100.0f, arg3, -1);
    if (local_FC == 1) {
      local_FC = 2;
    } else {
      local_FC = 3;
    }
    if ((Script_ScenDataUtil(task_5C, 4, task_A4[arg2] * 13 + 1) & 4) == 0) {
      script_5C5D(arg0, arg1, arg2, arg3, local_FC, 0);
    } else {
      if (arg0 == task_1BC) {
        script_9030(arg0, arg1, arg2, arg3, local_FC, 0);
      }
    }
  }
  script_CCD7(arg0, arg2, 2);
  return;
}

static void script_43CB(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  float local_F8;
  float local_F4;
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
    local_FC = script_C46C(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
    if ((local_FC == 1) | (local_FC == 2)) {
      local_FC = 1;
    } else {
      local_FC = 0;
    }
    *(int32_t *)&local_F8 = Script_ScenDataUtil(data_257BC, 5, Script_ScenDataUtil(data_251CC, 1, arg3 * 40 + task_F4[arg2] * 4 + local_FC) * 9 + 5);
    *(int32_t *)&local_F4 = Script_ScenDataUtil(data_257BC, 5, Script_ScenDataUtil(data_251CC, 1, arg3 * 40 + task_F4[arg2] * 4 + local_FC) * 9 + 6);
    local_FC = script_C46C(arg0, arg1, 5, local_F8 / 100.0f, local_F4 / 100.0f, arg3, -1);
    if (local_FC == 1) {
      local_FC = 1;
    } else {
      local_FC = 0;
    }
    if ((Script_ScenDataUtil(task_5C, 4, task_A4[arg2] * 13 + 1) & 4) == 0) {
      script_5C5D(arg0, arg1, arg2, arg3, local_FC, 0);
    } else {
      if (arg0 == task_1BC) {
        script_9030(arg0, arg1, arg2, arg3, local_FC, 0);
      }
    }
  }
  script_CCD7(arg0, arg2, 2);
  return;
}

static void script_452C(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  int32_t local_F8;
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
    if ((Script_ScenDataUtil(task_5C, 4, task_A4[arg2] * 13 + 1) & 4) == 0) {
      float local_F4;
      float local_F0;
      local_FC = script_C46C(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
      *(int32_t *)&local_F4 = Script_ScenDataUtil(data_257BC, 5, Script_ScenDataUtil(data_251CC, 1, arg3 * 40 + task_F4[arg2] * 4 + local_FC) * 9 + 5);
      *(int32_t *)&local_F0 = Script_ScenDataUtil(data_257BC, 5, Script_ScenDataUtil(data_251CC, 1, arg3 * 40 + task_F4[arg2] * 4 + local_FC) * 9 + 6);
      if ((Script_ScenDataUtil(task_5C, 4, task_A4[arg2] * 13 + 1) & 0x40) | (Script_ScenDataUtil(task_5C, 4, task_A4[arg2] * 13 + 1) & 0x80)) {
        if (Script_ScenDataUtil(task_5C, 4, task_A4[arg2] * 13 + 1) & 0x40) {
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
      local_FC = script_C46C(arg0, arg1, local_F8, local_F4 / 100.0f, local_F0 / 100.0f, arg3, task_A4[arg2]);
      script_5C5D(arg0, arg1, arg2, arg3, local_FC, 1);
    } else {
      if (arg0 == task_1BC) {
        float local_F4;
        float local_F0;
        local_FC = script_C46C(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
        *(int32_t *)&local_F4 = Script_ScenDataUtil(data_257BC, 5, Script_ScenDataUtil(data_251CC, 1, arg3 * 40 + task_F4[arg2] * 4 + local_FC) * 9 + 5);
        *(int32_t *)&local_F0 = Script_ScenDataUtil(data_257BC, 5, Script_ScenDataUtil(data_251CC, 1, arg3 * 40 + task_F4[arg2] * 4 + local_FC) * 9 + 6);
        local_FC = script_C46C(arg0, arg1, 6, local_F4 / 100.0f, local_F0 / 100.0f, arg3, task_A4[arg2]);
        script_9030(arg0, arg1, arg2, arg3, local_FC, 1);
      }
    }
  }
  script_CCD7(arg0, arg2, 2);
  return;
}

static void script_47CA(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  float local_F8;
  float local_F4;
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
    local_FC = script_C46C(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
    *(int32_t *)&local_F8 = Script_ScenDataUtil(data_257BC, 5, Script_ScenDataUtil(data_251CC, 1, arg3 * 40 + task_F4[arg2] * 4 + local_FC) * 9 + 5);
    *(int32_t *)&local_F4 = Script_ScenDataUtil(data_257BC, 5, Script_ScenDataUtil(data_251CC, 1, arg3 * 40 + task_F4[arg2] * 4 + local_FC) * 9 + 6);
    script_C46C(arg0, arg1, 0, local_F8 / 100.0f, local_F4 / 100.0f, arg3, -1);
  }
  script_CCD7(arg0, arg2, 0);
  return;
}

static void script_4898(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
    local_FC = script_C46C(arg0, arg1, 4, 0.5f, 0.5f, arg3, -1);
    if (local_FC == 1) {
      local_FC = 2;
    } else {
      local_FC = 3;
    }
    script_6BDC(arg0, arg1, arg2, arg3, local_FC, 1);
  }
  script_CCD7(arg0, arg2, 1);
  return;
}

static void script_48FF(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
    local_FC = script_C46C(arg0, arg1, 4, 0.5f, 0.5f, arg3, -1);
    if (local_FC == 1) {
      local_FC = 1;
    } else {
      local_FC = 0;
    }
    script_6BDC(arg0, arg1, arg2, arg3, local_FC, 1);
  }
  script_CCD7(arg0, arg2, 1);
  return;
}

static void script_4966(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
    script_C46C(arg0, arg1, 4, 0.0f, 0.5f, arg3, -1);
    script_6BDC(arg0, arg1, arg2, arg3, 4, 1);
  }
  script_CCD7(arg0, arg2, 1);
  return;
}

static void script_49AD(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  float local_F8;
  float local_F4;
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
    local_FC = script_C46C(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
    if ((local_FC == 1) | (local_FC == 2)) {
      local_FC = 1;
    } else {
      local_FC = 0;
    }
    *(int32_t *)&local_F8 = Script_ScenDataUtil(data_257BC, 5, Script_ScenDataUtil(data_251CC, 1, arg3 * 40 + task_F4[arg2] * 4 + local_FC) * 9 + 5);
    *(int32_t *)&local_F4 = Script_ScenDataUtil(data_257BC, 5, Script_ScenDataUtil(data_251CC, 1, arg3 * 40 + task_F4[arg2] * 4 + local_FC) * 9 + 6);
    if (local_FC == 0) {
      local_FC = script_C46C(arg0, arg1, 5, local_F8 / 100.0f, local_F4 / 100.0f, arg3, -1);
    } else {
      local_FC = script_C46C(arg0, arg1, 5, local_F8 / 100.0f, local_F4 / 100.0f, arg3, -1);
    }
    script_7096(arg0, arg1, arg2, arg3, local_FC, 0);
  }
  script_CCD7(arg0, arg2, 2);
  return;
}

static void script_4AE3(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
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
  if (arg0 == task_1BC) {
    if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
      local_FF7F = script_C46C(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
      *(int32_t *)&local_FF78 = Script_ScenDataUtil(data_257BC, 5, Script_ScenDataUtil(data_251CC, 1, arg3 * 40 + task_F4[arg2] * 4 + local_FF7F) * 9 + 5);
      *(int32_t *)&local_FF74 = Script_ScenDataUtil(data_257BC, 5, Script_ScenDataUtil(data_251CC, 1, arg3 * 40 + task_F4[arg2] * 4 + local_FF7F) * 9 + 6);
      local_FF7F = script_C46C(arg0, arg1, 5, local_FF78 / (float)100, local_FF74 / (float)100, arg3, -1);
      script_CB35(arg0, 0);
      *(int32_t *)&local_94 = Script_ScenDataUtil(data_257BC, 5, Script_ScenDataUtil(data_251CC, 1, arg3 * 40 + task_F4[arg2] * 4 + local_FF7F) * 9 + 1);
      *(int32_t *)&local_90 = Script_ScenDataUtil(data_257BC, 5, Script_ScenDataUtil(data_251CC, 1, arg3 * 40 + task_F4[arg2] * 4 + local_FF7F) * 9 + 2);
      local_8C = 0.0f;
      local_FC = Script_ScenDataUtil(data_257BC, 4, Script_ScenDataUtil(data_251CC, 1, arg3 * 40 + task_F4[arg2] * 4 + local_FF7F) * 9);
      local_FC = local_FC | ((Script_FloatUtils(ScriptMathOp_FloatToInt, local_94 - 3.0f) << 16) & 0x7FFF0000);
      local_9C.x = (&task_1C4)->x;
      (&local_9C)->y = task_1C4.y;
      (&local_9C)->z = task_1C4.z;
      Script_charaPosRotUtils(arg1, 12, &local_CC, &local_E4, 2);
      Script_charaPosRotUtils(arg1, 7, &local_C0, &local_D8, 2);
      Script_ryoControl(PCCTL_StartCameraFacingInterpolation, 0);
      if (arg0 == task_1BC) {
        Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
        Script_scenCharaSubmitMoveTargetRequest(arg0, &task_1C4, *(&task_1D0 + 1), local_FC, 0);
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
        Script_SetCharaMoveTarget(arg0, &task_1C4, *(&task_1D0 + 1), local_FC);
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
              local_FF6C = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_A4[arg2], 2);
              local_FF6C = Script_ScenDataUtil(task_64, 4, Script_ScenDataUtil(task_68, 4, local_FF6C * 9 + 1) * 4 + 2);
              if (local_FF6C != -1) {
                if (arg0 == task_1BC) {
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
              local_FF6C = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_A4[arg2], 2);
              local_FF6C = Script_ScenDataUtil(task_64, 4, Script_ScenDataUtil(task_68, 4, local_FF6C * 9 + 1) * 4 + 2);
              if (local_FF6C != -1) {
                if (arg0 == task_1BC) {
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
      script_CB35(arg0, 1);
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
        local_FF6C = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_A4[arg2], 2);
        local_FF6C = Script_ScenDataUtil(task_64, 4, Script_ScenDataUtil(task_68, 4, local_FF6C * 9 + 1) * 4 + 3);
        if (local_FF6C != -1) {
          if (arg0 == task_1BC) {
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
    script_CCD7(arg0, arg2, 0);
  } else {
    script_40C8(arg0, arg1, arg2, arg3);
  }
  return;
}

static void script_53A4(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  float local_F8;
  int32_t local_F4;
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
    local_FC = script_C46C(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
    *(int32_t *)&local_F8 = Script_ScenDataUtil(data_257BC, 5, Script_ScenDataUtil(data_251CC, 1, arg3 * 40 + task_F4[arg2] * 4 + local_FC) * 9 + 5);
    local_F8 = 98.0f - local_F8;
    local_F4 = Script_ScenDataUtil(data_257BC, 5, Script_ScenDataUtil(data_251CC, 1, arg3 * 40 + task_F4[arg2] * 4 + local_FC) * 9 + 6);
    if ((local_FC == 0) | (local_FC == 3)) {
      local_FC = script_C46C(arg0, arg1, 4, 0.34736f, 0.73184f, arg3, -1);
    } else {
      local_FC = script_C46C(arg0, arg1, 4, 0.4f, 0.712f, arg3, -1);
    }
    script_9751(arg0, arg1, arg2, arg3, local_FC, 0);
  }
  script_CCD7(arg0, arg2, 0);
  return;
}

static int32_t script_54C2(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
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
    local_F4 = Script_ScenDataUtil(task_5C, 4, task_A4[arg2] * 13 + 6);
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
        local_FC = script_C46C(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
        local_FC = script_C46C(arg0, arg1, 7, 0.45f, 0.0f, arg3, -1);
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
              goto L00567E;
            }
          }
          L00567E:;
        } else {
          local_D4 = 0.45f;
        }
        Script_EVT_GetPosition(arg1, -1, &local_E4, 0);
        Script_mtwkGetSetRotation(arg1, &local_D8, 0);
        local_E4.x = (&local_E4)->x + local_D4 * Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_D8)->y & 0xFFFF);
        (&local_E4)->z = (&local_E4)->z - local_D4 * Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_D8)->y & 0xFFFF);
        *(&task_1D0 + 1) = Script_FloatUtils(ScriptMathOp_Atan2ReverseVector, (&local_E4)->x - (&task_1C4)->x, (&local_E4)->z - task_1C4.z);
        script_9EDB(arg0, arg1, arg2, arg3, local_FC, local_F0);
      }
    } else if ((local_BC == 3) | (local_BC == 4) | (local_BC == 5) | (local_BC == 6)) {
      *(int32_t *)&local_D0 = Script_ScenDataUtil(&data_261DC, 5, Script_ScenDataUtil(task_5C, 4, task_A4[arg2] * 13 + 6) * 5 + 3);
      *(int32_t *)&local_CC = Script_ScenDataUtil(&data_261DC, 5, Script_ScenDataUtil(task_5C, 4, task_A4[arg2] * 13 + 6) * 5 + 4);
      local_FC = script_C46C(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
      local_BC = arg3;
      if ((local_BC == 13) | (local_BC == 14) | (local_BC == 15) | (local_BC == 16) | (local_BC == 20) | (local_BC == 23) | (local_BC == 28) | (local_BC == 29)) {
        if ((Script_ScenDataUtil(task_5C, 4, task_A4[arg2] * 13 + 1) & 0x40) | (Script_ScenDataUtil(task_5C, 4, task_A4[arg2] * 13 + 1) & 0x80)) {
          if (Script_ScenDataUtil(task_5C, 4, task_A4[arg2] * 13 + 1) & 0x40) {
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
      local_FC = script_C46C(arg0, arg1, local_F8, local_D0 / (float)100, local_CC / (float)100, arg3, task_A4[arg2]);
      script_9FB3(arg0, arg1, arg2, arg3, local_FC, 0);
    } else {
      if (local_BC == 7) {
        local_FC = script_C46C(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
        local_BC = arg3;
        if ((local_BC == 13) | (local_BC == 14) | (local_BC == 15) | (local_BC == 16) | (local_BC == 20) | (local_BC == 23) | (local_BC == 28) | (local_BC == 29)) {
          if ((Script_ScenDataUtil(task_5C, 4, task_A4[arg2] * 13 + 1) & 0x40) | (Script_ScenDataUtil(task_5C, 4, task_A4[arg2] * 13 + 1) & 0x80)) {
            if (Script_ScenDataUtil(task_5C, 4, task_A4[arg2] * 13 + 1) & 0x40) {
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
        if (Script_CycleManagerUtils(CYCLE_CMD_IS_DOOR_OR_MOBJ_TYPE2, Script_ScenDataUtil(task_5C, 4, task_A4[arg2] * 13 + 12)) == 1) {
          *(int32_t *)&local_D0 = Script_ScenDataUtil(&data_261DC, 5, Script_ScenDataUtil(task_5C, 4, task_A4[arg2] * 13 + 6) * 5 + 3);
          *(int32_t *)&local_CC = Script_ScenDataUtil(&data_261DC, 5, Script_ScenDataUtil(task_5C, 4, task_A4[arg2] * 13 + 6) * 5 + 4);
          local_FC = script_C46C(arg0, arg1, local_F8, local_D0 / (float)100, local_CC / (float)100, arg3, task_A4[arg2]);
          Script_scenUtils(SCENARIO_CMD_INIT_SCNT);
          script_A42A(arg0, arg1, arg2, arg3, local_FC, 0);
          Script_scenUtils(SCENARIO_CMD_UPDATE_TIMER);
        } else {
          *(int32_t *)&local_D0 = Script_ScenDataUtil(&data_261DC, 5, Script_ScenDataUtil(task_5C, 4, task_A4[arg2] * 13 + 6) * 5 + 3);
          *(int32_t *)&local_CC = Script_ScenDataUtil(&data_261DC, 5, Script_ScenDataUtil(task_5C, 4, task_A4[arg2] * 13 + 6) * 5 + 4);
          local_FC = script_C46C(arg0, arg1, local_F8, local_D0 / (float)100, local_CC / (float)100, arg3, task_A4[arg2]);
          script_9FB3(arg0, arg1, arg2, arg3, local_FC, 0);
        }
        goto L005B79;
      }
    }
  }
  L005B79:;
  script_CCD7(arg0, arg2, 0);
  end:;
}

static void script_5B87(int32_t arg0, int32_t arg1) {
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
    Script_SetCharaMoveTarget(arg0, &local_F4, (&local_E8)->y, script_3EE3(arg1));
  }
  return;
}

__attribute__((scn_string_bindings(0, data_26CFA)))
static void script_5C5D(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
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
  *(int32_t *)&local_98 = Script_ScenDataUtil(data_257BC, 5, Script_ScenDataUtil(data_251CC, 1, arg3 * 40 + task_F4[arg2] * 4 + arg4) * 9 + 1);
  *(int32_t *)&local_94 = Script_ScenDataUtil(data_257BC, 5, Script_ScenDataUtil(data_251CC, 1, arg3 * 40 + task_F4[arg2] * 4 + arg4) * 9 + 2);
  *(int32_t *)&local_90 = Script_ScenDataUtil(data_257BC, 5, Script_ScenDataUtil(data_251CC, 1, arg3 * 40 + task_F4[arg2] * 4 + arg4) * 9 + 3);
  *(int32_t *)&local_8C = Script_ScenDataUtil(data_257BC, 5, Script_ScenDataUtil(data_251CC, 1, arg3 * 40 + task_F4[arg2] * 4 + arg4) * 9 + 4);
  *(int32_t *)&local_88 = Script_ScenDataUtil(data_257BC, 5, Script_ScenDataUtil(data_251CC, 1, arg3 * 40 + task_F4[arg2] * 4 + arg4) * 9 + 7);
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
      local_FF78 = Script_ScenDataUtil(data_257BC, 4, Script_ScenDataUtil(data_251CC, 1, arg3 * 40 + task_F4[arg2] * 4 + arg4) * 9);
      local_FF7C = 18;
      local_80 = 18;
      local_A4 = 1.0f;
    } else {
      if (arg0 == task_1BC) {
        local_FF78 = Script_ScenDataUtil(data_257BC, 4, Script_ScenDataUtil(data_251CC, 1, arg3 * 40 + task_F4[arg2] * 4 + arg4) * 9) | -0x80000000;
        local_FF7C = 12;
        local_80 = 12;
      } else {
        local_FF78 = Script_ScenDataUtil(data_257BC, 4, Script_ScenDataUtil(data_251CC, 1, arg3 * 40 + task_F4[arg2] * 4 + arg4) * 9);
        local_FF7C = 18;
        local_80 = 18;
      }
      local_A4 = -1.0f;
    }
    script_3931(arg0, local_FF78, arg2, arg3, arg4);
    local_FF78 = local_FF78 | ((Script_FloatUtils(ScriptMathOp_FloatToInt, local_98 - 3.0f) << 16) & 0x7FFF0000);
    script_CB35(arg0, 0);
    local_C0.x = (&task_1C4)->x;
    (&local_C0)->y = task_1C4.y;
    (&local_C0)->z = task_1C4.z;
    Script_ryoControl(PCCTL_StartCameraFacingInterpolation, 0);
    Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
    Script_SetCharaMoveTarget(arg0, &task_1C4, *(&task_1D0 + 1), local_FF78);
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
    if (task_1BC == arg0) {
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
          script_5B87(arg0, task_F4[arg2]);
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
            local_FF64 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_A4[arg2], 2);
            local_FF64 = Script_ScenDataUtil(task_64, 4, Script_ScenDataUtil(task_68, 4, local_FF64 * 9 + 1) * 4 + 2);
            if (local_FF64 != -1) {
              if (arg0 == task_1BC) {
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
                goto L0061E5;
              }
            }
            L0061E5:;
          } else {
            if ((local_FF64 == 1) | (local_FF64 == 3)) {
              if (!(local_B4 > 0.0f)) {
                local_B4 = 0.0f;
              } else {
                if (!(local_B4 < 0.75f)) {
                  local_B4 = 0.75f;
                  goto L006223;
                }
              }
              L006223:;
              goto L006226;
            }
          }
        }
      }
      L006226:;
      if (task_1BC == arg0) {
        if (!(local_B8 <= local_94) & !(local_B8 >= local_90)) {
          local_86 = 0;
        }
      } else {
        if (!(local_B8 <= local_94) & !(local_B8 > local_88)) {
          *(int32_t *)&local_9C = Script_ScenDataUtil(data_257BC, 5, Script_ScenDataUtil(data_251CC, 1, arg3 * 40 + task_F4[arg2] * 4 + arg4) * 9 + 8);
          local_B4 = local_B4 + local_9C * local_A4;
        }
      }
      if (!(local_B8 < local_90) & !(local_B8 >= local_90 + 1.5f)) {
        if (local_84 == 1) {
          int32_t local_FF64;
          local_FF64 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_A4[arg2], 2);
          local_FF64 = Script_ScenDataUtil(task_64, 4, Script_ScenDataUtil(task_68, 4, local_FF64 * 9 + 1) * 4 + 3);
          if (local_FF64 != -1) {
            if (arg0 == task_1BC) {
              Script_executeAicaCommand(local_FF64, 0, 0);
            } else {
              Script_Execute3DSoundCommand_Wrapper(local_FF64, 4, &local_C0, 5.0f, 20.0f, 0);
            }
          }
          local_84 = 2;
        }
        if (task_1BC != arg0) {
          if (local_FF74 == 0) {
            local_AC = local_B4 / (local_8C - local_90);
            local_FF74 = 1;
          }
        }
      }
      if (!(local_B8 < local_90) & !(local_B8 > local_8C)) {
        Script_EVT_GetPosition(arg0, local_FF7C, &local_F0, ENTITY_XFORM_MOTION_SPACE);
        if (task_1BC == arg0) {
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
                goto L0063F2;
              }
            }
            L0063F2:;
          } else {
            if ((local_FF64 == 1) | (local_FF64 == 3)) {
              if (!(local_B4 < 0.75f)) {
                local_B4 = 0.75f;
              } else {
                if (!(local_B4 > 0.0f)) {
                  local_B4 = 0.0f;
                  goto L006430;
                }
              }
              L006430:;
              goto L006433;
            }
          }
          L006433:;
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
              goto L00648C;
            }
          }
        }
      }
      L00648C:;
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
      if (arg0 == task_1BC) {
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
  script_CB35(arg0, 1);
  {
    int32_t local_FF78;
    int32_t unused_FF68[4];
    int32_t local_FF64;
    local_FF78 = task_A4[arg2];
    local_FF64 = arg3;
    if ((local_FF64 == 9) | (local_FF64 == 10) | (local_FF64 == 15)) {
      if (arg5) {
        Script_charaPosRotUtils(arg1, 12, &local_D8, 0, 0);
        Script_charaPosRotUtils(arg1, 7, &local_CC, 0, 0);
      } else {
        Script_EVT_SetPosition(arg1, POS_XYZ, &local_D8);
      }
      if (task_1BC == arg0) {
        local_80 = (Script_ScenDataUtil(task_5C, 4, local_FF78 * 13 + 2) >> 16) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_80, 1);
        local_80 = Script_ScenDataUtil(task_5C, 4, local_FF78 * 13 + 2) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_80, 1);
      }
    } else if ((local_FF64 == 18) | (local_FF64 == 19) | (local_FF64 == 21) | (local_FF64 == 22)) {
      if (task_1BC == arg0) {
        local_80 = (Script_ScenDataUtil(task_5C, 4, local_FF78 * 13 + 2) >> 16) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_80, 1);
        {
          int32_t local_FF60;
          local_FF60 = Script_ScenDataUtil(task_5C, 4, local_FF78 * 13 + 1);
          local_FF60 = local_FF60 | 8;
          Script_ScenDataUtil(task_5C, 14, local_FF78 * 13 + 1, local_FF60);
        }
        {
          int32_t local_FF60;
          local_FF60 = Script_ScenDataUtil(task_5C, 4, local_FF78 * 13 + 1);
          local_FF60 = local_FF60 | 0x10;
          Script_ScenDataUtil(task_5C, 14, local_FF78 * 13 + 1, local_FF60);
        }
        {
          int32_t local_FF60;
          local_FF60 = Script_ScenDataUtil(task_5C, 4, local_FF78 * 13 + 1);
          local_FF60 = local_FF60 | 2;
          Script_ScenDataUtil(task_5C, 14, local_FF78 * 13 + 1, local_FF60);
        }
      } else {
        if (arg5) {
          Script_charaPosRotUtils(arg1, 12, &local_D8, 0, 0);
          Script_charaPosRotUtils(arg1, 7, &local_CC, 0, 0);
        } else {
          Script_EVT_SetPosition(arg1, POS_XYZ, &local_D8);
        }
        local_80 = (Script_ScenDataUtil(task_5C, 4, local_FF78 * 13 + 2) >> 16) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_80, 1);
        local_80 = Script_ScenDataUtil(task_5C, 4, local_FF78 * 13 + 2) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_80, 1);
      }
    } else {
      if ((local_FF64 == 20) | (local_FF64 == 23)) {
        if (task_1BC == arg0) {
          if ((arg4 == 0) | (arg4 == 1)) {
            {
              int32_t local_FF60;
              local_FF60 = Script_ScenDataUtil(task_5C, 4, local_FF78 * 13 + 1);
              local_FF60 = local_FF60 | 0x10;
              Script_ScenDataUtil(task_5C, 14, local_FF78 * 13 + 1, local_FF60);
            }
            {
              int32_t local_FF60;
              local_FF60 = Script_ScenDataUtil(task_5C, 4, local_FF78 * 13 + 1);
              local_FF60 = local_FF60 | 4;
              Script_ScenDataUtil(task_5C, 14, local_FF78 * 13 + 1, local_FF60);
            }
          } else {
            {
              int32_t local_FF60;
              local_FF60 = Script_ScenDataUtil(task_5C, 4, local_FF78 * 13 + 1);
              local_FF60 = local_FF60 | 8;
              Script_ScenDataUtil(task_5C, 14, local_FF78 * 13 + 1, local_FF60);
            }
            {
              int32_t local_FF60;
              local_FF60 = Script_ScenDataUtil(task_5C, 4, local_FF78 * 13 + 1);
              local_FF60 = local_FF60 | 4;
              Script_ScenDataUtil(task_5C, 14, local_FF78 * 13 + 1, local_FF60);
            }
          }
        } else {
          if (arg5) {
            Script_charaPosRotUtils(arg1, 12, &local_D8, 0, 0);
            Script_charaPosRotUtils(arg1, 7, &local_CC, 0, 0);
          } else {
            Script_EVT_SetPosition(arg1, POS_XYZ, &local_D8);
          }
          local_80 = (Script_ScenDataUtil(task_5C, 4, local_FF78 * 13 + 2) >> 16) & 0xFFFF;
          Script_scenToggleCharaFlag(0, local_80, 1);
          local_80 = Script_ScenDataUtil(task_5C, 4, local_FF78 * 13 + 2) & 0xFFFF;
          Script_scenToggleCharaFlag(0, local_80, 1);
        }
        goto L006B6F;
      }
    }
  }
  L006B6F:;
  return;
}

static void script_6B70(int32_t arg0, int32_t arg1, int32_t arg2) {
  int32_t local_FC;
  if (task_1BC == arg0) {
    local_FC = task_A4[arg1];
    {
      int32_t local_F8;
      local_F8 = Script_ScenDataUtil(task_5C, 4, local_FC * 13 + 2) & 0xFFFF;
      Script_scenToggleCharaFlag(0, local_F8, arg2);
    }
    {
      int32_t local_F8;
      local_F8 = (Script_ScenDataUtil(task_5C, 4, local_FC * 13 + 2) >> 16) & 0xFFFF;
      Script_scenToggleCharaFlag(0, local_F8, arg2);
    }
  }
  return;
}

static void script_6BDC(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
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
  local_AC.x = (&task_1C4)->x;
  (&local_AC)->y = task_1C4.y;
  (&local_AC)->z = task_1C4.z;
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
    local_80 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_A4[arg2], 2);
    local_80 = Script_ScenDataUtil(task_64, 4, Script_ScenDataUtil(task_68, 4, local_80 * 9 + 1) * 4 + 2);
    if (local_80 != -1) {
      if (arg0 == task_1BC) {
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
        task_CC[arg2] = arg0;
        local_98 = -local_98;
        local_84 = 0;
      }
      local_8C = 0;
    } else {
      if ((local_93 == 1) & (local_8C == 0)) {
        int32_t local_80;
        local_80 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_A4[arg2], 2);
        local_80 = Script_ScenDataUtil(task_64, 4, Script_ScenDataUtil(task_68, 4, local_80 * 9 + 1) * 4 + 3);
        if (local_80 != -1) {
          if (arg0 == task_1BC) {
            Script_executeAicaCommand(local_80, 0, 0);
          } else {
            Script_Execute3DSoundCommand_Wrapper(local_80, 4, &local_AC, 5.0f, 20.0f, 0);
          }
        }
        local_93 = 0;
        local_8C = 1;
        local_98 = -local_98;
        local_84 = 0;
        task_CC[arg2] = 0;
      }
    }
    local_88 = task_A4[arg2];
    local_88 = Script_ScenDataUtil(task_5C, 4, local_88 * 13 + 2) & 0xFFFF;
    local_88 = local_88 + ((local_9C - 0x4000) >> 12);
    if (local_84 != local_88) {
      Script_scenToggleCharaFlag(0, local_88, local_8C);
      local_84 = local_88;
    }
    local_9C = local_9C - local_98;
    if (local_9C >= 0xC000) {
      local_9C = 0xC000;
      local_88 = task_A4[arg2];
      local_88 = Script_ScenDataUtil(task_5C, 4, local_88 * 13 + 2) & 0xFFFF;
      local_88 = local_88 + 8;
      Script_scenToggleCharaFlag(0, local_88, local_8C);
      break;
    }
    if (local_9C <= 0x4000) {
      local_9C = 0x4000;
      if (local_93 == 0) {
        task_CC[arg2] = 0;
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
          goto L007046;
        }
      }
    }
    L007046:;
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

__attribute__((scn_string_bindings(0, data_26D16)))
static void script_7096(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
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
          local_FC = Script_ScenDataUtil(data_257BC, 4, Script_ScenDataUtil(data_251CC, 1, arg3 * 40 + task_F4[arg2] * 4 + arg4) * 9);
          break;
        case 0:
          local_B7 = 0;
          local_F8 = 12;
          local_F0 = 0x4000;
          local_B8 = 1.0f;
          local_FC = Script_ScenDataUtil(data_257BC, 4, Script_ScenDataUtil(data_251CC, 1, arg3 * 40 + task_F4[arg2] * 4 + arg4) * 9) | -0x80000000;
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
          local_FC = Script_ScenDataUtil(data_257BC, 4, Script_ScenDataUtil(data_251CC, 1, arg3 * 40 + task_F4[arg2] * 4 + arg4) * 9) | -0x80000000;
          break;
        case 0:
          local_B7 = 1;
          local_F8 = 18;
          local_F0 = 0xC000;
          local_B8 = 1.0f;
          local_FC = Script_ScenDataUtil(data_257BC, 4, Script_ScenDataUtil(data_251CC, 1, arg3 * 40 + task_F4[arg2] * 4 + arg4) * 9);
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
          local_FC = Script_ScenDataUtil(data_257BC, 4, Script_ScenDataUtil(data_251CC, 1, arg3 * 40 + task_F4[arg2] * 4 + arg4) * 9) | -0x80000000;
          break;
        case 0:
          local_B7 = 0;
          local_F8 = 18;
          local_F0 = 0xC000;
          local_B8 = -1.0f;
          local_FC = Script_ScenDataUtil(data_257BC, 4, Script_ScenDataUtil(data_251CC, 1, arg3 * 40 + task_F4[arg2] * 4 + arg4) * 9);
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
          local_FC = Script_ScenDataUtil(data_257BC, 4, Script_ScenDataUtil(data_251CC, 1, arg3 * 40 + task_F4[arg2] * 4 + arg4) * 9);
          break;
        case 0:
          local_B7 = 1;
          local_F8 = 12;
          local_F0 = 0x4000;
          local_B8 = -1.0f;
          local_FC = Script_ScenDataUtil(data_257BC, 4, Script_ScenDataUtil(data_251CC, 1, arg3 * 40 + task_F4[arg2] * 4 + arg4) * 9) | -0x80000000;
          break;
      }
      break;
  }
  *(int32_t *)&local_C4 = Script_ScenDataUtil(data_257BC, 5, Script_ScenDataUtil(data_251CC, 1, arg3 * 40 + task_F4[arg2] * 4 + arg4) * 9 + 1);
  *(int32_t *)&local_C0 = Script_ScenDataUtil(data_257BC, 5, Script_ScenDataUtil(data_251CC, 1, arg3 * 40 + task_F4[arg2] * 4 + arg4) * 9 + 2);
  local_FC = local_FC | ((Script_FloatUtils(ScriptMathOp_FloatToInt, local_C4 - 3.0f) << 16) & 0x7FFF0000);
  local_BC = 0.0f;
  script_CB35(arg0, 0);
  local_CC.x = (&task_1C4)->x;
  (&local_CC)->y = task_1C4.y;
  (&local_CC)->z = task_1C4.z;
  Script_ryoControl(PCCTL_StartCameraFacingInterpolation, 0);
  Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
  Script_SetCharaMoveTarget(arg0, &task_1C4, *(&task_1D0 + 1), local_FC);
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
        local_A0 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_A4[arg2], 2);
        local_A0 = Script_ScenDataUtil(task_64, 4, Script_ScenDataUtil(task_68, 4, local_A0 * 9 + 1) * 4 + 2);
        if (local_A0 != -1) {
          if (arg0 == task_1BC) {
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
  script_CB35(arg0, 1);
  local_FC = task_A4[arg2];
  {
    int32_t local_A0;
    local_A0 = Script_ScenDataUtil(task_5C, 4, local_FC * 13 + 1);
    local_A0 = local_A0 | 8;
    Script_ScenDataUtil(task_5C, 14, local_FC * 13 + 1, local_A0);
  }
  {
    int32_t local_A0;
    local_A0 = Script_ScenDataUtil(task_5C, 4, local_FC * 13 + 1);
    local_A0 = local_A0 | 0x10;
    Script_ScenDataUtil(task_5C, 14, local_FC * 13 + 1, local_A0);
  }
  {
    int32_t local_A0;
    local_A0 = Script_ScenDataUtil(task_5C, 4, local_FC * 13 + 1);
    local_A0 = local_A0 | 4;
    Script_ScenDataUtil(task_5C, 14, local_FC * 13 + 1, local_A0);
  }
  return;
}

__attribute__((scn_string_bindings(0, data_26D32)))
static void script_7750(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
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
      if (arg0 == task_1BC) {
        local_FF5C = arg4;
        switch (local_FF5C) {
          case 1:
            local_CF = 1;
            local_A4 = Script_ScenDataUtil(data_257BC, 4, Script_ScenDataUtil(data_251CC, 1, arg3 * 40 + task_F4[arg2] * 4 + arg4) * 9);
            local_C0 = 1.0f;
            local_A0 = 18;
            local_9C = 18;
            local_90 = 15473;
            local_8C = 17294;
            break;
          case 0:
            local_CF = 0;
            local_A4 = Script_ScenDataUtil(data_257BC, 4, Script_ScenDataUtil(data_251CC, 1, arg3 * 40 + task_F4[arg2] * 4 + arg4) * 9) | -0x80000000;
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
            local_A4 = Script_ScenDataUtil(data_257BC, 4, Script_ScenDataUtil(data_251CC, 1, arg3 * 40 + task_F4[arg2] * 4 + arg4) * 9);
            local_C0 = 1.0f;
            local_A0 = 18;
            local_9C = 18;
            local_90 = 13653;
            break;
          case 0:
            local_CF = 0;
            local_A4 = Script_ScenDataUtil(data_257BC, 4, Script_ScenDataUtil(data_251CC, 1, arg3 * 40 + task_F4[arg2] * 4 + arg4) * 9);
            local_C0 = 1.0f;
            local_A0 = 18;
            local_9C = 18;
            local_90 = 19842;
            break;
        }
      }
    } else if ((local_FF5C == 1) | (local_FF5C == 7) | (local_FF5C == 30) | (local_FF5C == 36) | (local_FF5C == 37)) {
      if (arg0 == task_1BC) {
        local_FF5C = arg4;
        switch (local_FF5C) {
          case 1:
            local_CF = 0;
            local_A4 = Script_ScenDataUtil(data_257BC, 4, Script_ScenDataUtil(data_251CC, 1, arg3 * 40 + task_F4[arg2] * 4 + arg4) * 9) | -0x80000000;
            local_C0 = 1.0f;
            local_A0 = 12;
            local_9C = 18;
            local_90 = 50062;
            local_8C = 48241;
            break;
          case 0:
            local_CF = 1;
            local_A4 = Script_ScenDataUtil(data_257BC, 4, Script_ScenDataUtil(data_251CC, 1, arg3 * 40 + task_F4[arg2] * 4 + arg4) * 9);
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
            local_A4 = Script_ScenDataUtil(data_257BC, 4, Script_ScenDataUtil(data_251CC, 1, arg3 * 40 + task_F4[arg2] * 4 + arg4) * 9);
            local_C0 = 1.0f;
            local_A0 = 18;
            local_9C = 18;
            if (task_F4[arg2] == 4) {
              local_90 = 50062;
            } else {
              local_90 = 51882;
            }
            break;
          case 0:
            local_CF = 1;
            local_A4 = Script_ScenDataUtil(data_257BC, 4, Script_ScenDataUtil(data_251CC, 1, arg3 * 40 + task_F4[arg2] * 4 + arg4) * 9);
            local_C0 = 1.0f;
            local_A0 = 18;
            local_9C = 18;
            if (task_F4[arg2] == 4) {
              local_90 = 46421;
            } else {
              local_90 = 45511;
            }
            break;
        }
      }
    } else if ((local_FF5C == 3) | (local_FF5C == 5) | (local_FF5C == 32) | (local_FF5C == 34)) {
      if (arg0 == task_1BC) {
        local_FF5C = arg4;
        switch (local_FF5C) {
          case 1:
            local_CF = 1;
            local_A4 = Script_ScenDataUtil(data_257BC, 4, Script_ScenDataUtil(data_251CC, 1, arg3 * 40 + task_F4[arg2] * 4 + arg4) * 9) | -0x80000000;
            local_C0 = -1.0f;
            local_A0 = 12;
            local_9C = 12;
            local_90 = 50062;
            local_8C = 48241;
            break;
          case 0:
            local_CF = 0;
            local_A4 = Script_ScenDataUtil(data_257BC, 4, Script_ScenDataUtil(data_251CC, 1, arg3 * 40 + task_F4[arg2] * 4 + arg4) * 9);
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
            local_A4 = Script_ScenDataUtil(data_257BC, 4, Script_ScenDataUtil(data_251CC, 1, arg3 * 40 + task_F4[arg2] * 4 + arg4) * 9);
            local_C0 = -1.0f;
            local_A0 = 18;
            local_9C = 18;
            local_90 = 51882;
            break;
          case 0:
            local_CF = 0;
            local_A4 = Script_ScenDataUtil(data_257BC, 4, Script_ScenDataUtil(data_251CC, 1, arg3 * 40 + task_F4[arg2] * 4 + arg4) * 9);
            local_C0 = -1.0f;
            local_A0 = 18;
            local_9C = 18;
            local_90 = 45511;
            break;
        }
      }
    } else if ((local_FF5C == 2) | (local_FF5C == 8) | (local_FF5C == 31)) {
      if (arg0 == task_1BC) {
        local_FF5C = arg4;
        switch (local_FF5C) {
          case 1:
            local_CF = 0;
            local_A4 = Script_ScenDataUtil(data_257BC, 4, Script_ScenDataUtil(data_251CC, 1, arg3 * 40 + task_F4[arg2] * 4 + arg4) * 9);
            local_C0 = -1.0f;
            local_A0 = 18;
            local_9C = 12;
            local_90 = 15473;
            local_8C = 17294;
            break;
          case 0:
            local_CF = 1;
            local_A4 = Script_ScenDataUtil(data_257BC, 4, Script_ScenDataUtil(data_251CC, 1, arg3 * 40 + task_F4[arg2] * 4 + arg4) * 9) | -0x80000000;
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
            local_A4 = Script_ScenDataUtil(data_257BC, 4, Script_ScenDataUtil(data_251CC, 1, arg3 * 40 + task_F4[arg2] * 4 + arg4) * 9);
            local_C0 = -1.0f;
            local_A0 = 18;
            local_9C = 18;
            local_90 = 13653;
            break;
          case 0:
            local_CF = 1;
            local_A4 = Script_ScenDataUtil(data_257BC, 4, Script_ScenDataUtil(data_251CC, 1, arg3 * 40 + task_F4[arg2] * 4 + arg4) * 9);
            local_C0 = -1.0f;
            local_A0 = 18;
            local_9C = 18;
            local_90 = 19842;
            break;
        }
      }
    } else {
      if ((local_FF5C == 13) | (local_FF5C == 14) | (local_FF5C == 28)) {
        if (arg0 == task_1BC) {
          local_FF5C = arg4;
          switch (local_FF5C) {
            case 0:
              local_CF = 0;
              local_A4 = Script_ScenDataUtil(data_257BC, 4, Script_ScenDataUtil(data_251CC, 1, arg3 * 40 + task_F4[arg2] * 4 + arg4) * 9);
              local_C0 = -1.0f;
              local_A0 = 18;
              local_9C = 12;
              local_90 = 48241;
              local_8C = 50062;
              break;
            case 1:
              local_CF = 1;
              local_A4 = Script_ScenDataUtil(data_257BC, 4, Script_ScenDataUtil(data_251CC, 1, arg3 * 40 + task_F4[arg2] * 4 + arg4) * 9) | -0x80000000;
              local_C0 = -1.0f;
              local_A0 = 12;
              local_9C = 12;
              local_90 = 50062;
              local_8C = 48241;
              break;
            case 2:
              local_CF = 1;
              local_A4 = Script_ScenDataUtil(data_257BC, 4, Script_ScenDataUtil(data_251CC, 1, arg3 * 40 + task_F4[arg2] * 4 + arg4) * 9);
              local_C0 = 1.0f;
              local_A0 = 18;
              local_9C = 18;
              local_90 = 15473;
              local_8C = 17294;
              break;
            case 3:
              local_CF = 0;
              local_A4 = Script_ScenDataUtil(data_257BC, 4, Script_ScenDataUtil(data_251CC, 1, arg3 * 40 + task_F4[arg2] * 4 + arg4) * 9) | -0x80000000;
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
              local_A4 = Script_ScenDataUtil(data_257BC, 4, Script_ScenDataUtil(data_251CC, 1, arg3 * 40 + task_F4[arg2] * 4 + arg4) * 9);
              local_C0 = -1.0f;
              local_A0 = 18;
              local_9C = 18;
              local_90 = 45511;
              break;
            case 1:
              local_CF = 1;
              local_A4 = Script_ScenDataUtil(data_257BC, 4, Script_ScenDataUtil(data_251CC, 1, arg3 * 40 + task_F4[arg2] * 4 + arg4) * 9);
              local_C0 = -1.0f;
              local_A0 = 18;
              local_9C = 18;
              local_90 = 51882;
              break;
            case 2:
              local_CF = 1;
              local_A4 = Script_ScenDataUtil(data_257BC, 4, Script_ScenDataUtil(data_251CC, 1, arg3 * 40 + task_F4[arg2] * 4 + arg4) * 9);
              local_C0 = 1.0f;
              local_A0 = 18;
              local_9C = 18;
              if (task_F4[arg2] == 4) {
                local_90 = 14199;
              } else {
                local_90 = 13653;
              }
              break;
            case 3:
              local_CF = 0;
              local_A4 = Script_ScenDataUtil(data_257BC, 4, Script_ScenDataUtil(data_251CC, 1, arg3 * 40 + task_F4[arg2] * 4 + arg4) * 9);
              local_C0 = 1.0f;
              local_A0 = 18;
              local_9C = 18;
              if (task_F4[arg2] == 4) {
                local_90 = 18022;
              } else {
                local_90 = 19842;
              }
              break;
          }
        }
        local_CE = 1;
        goto L008343;
      }
    }
  }
  L008343:;
  (void)local_90;
  *(int32_t *)&local_B8 = Script_ScenDataUtil(data_257BC, 5, Script_ScenDataUtil(data_251CC, 1, arg3 * 40 + task_F4[arg2] * 4 + arg4) * 9 + 1);
  *(int32_t *)&local_B4 = Script_ScenDataUtil(data_257BC, 5, Script_ScenDataUtil(data_251CC, 1, arg3 * 40 + task_F4[arg2] * 4 + arg4) * 9 + 2);
  *(int32_t *)&local_B0 = Script_ScenDataUtil(data_257BC, 5, Script_ScenDataUtil(data_251CC, 1, arg3 * 40 + task_F4[arg2] * 4 + arg4) * 9 + 3);
  *(int32_t *)&local_AC = Script_ScenDataUtil(data_257BC, 5, Script_ScenDataUtil(data_251CC, 1, arg3 * 40 + task_F4[arg2] * 4 + arg4) * 9 + 4);
  *(int32_t *)&local_A8 = Script_ScenDataUtil(data_257BC, 5, Script_ScenDataUtil(data_251CC, 1, arg3 * 40 + task_F4[arg2] * 4 + arg4) * 9 + 7);
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
  script_3931(arg0, local_A4, arg2, arg3, arg4);
  local_A4 = local_A4 | ((Script_FloatUtils(ScriptMathOp_FloatToInt, local_B8 - 3.0f) << 16) & 0x7FFF0000);
  script_CB35(arg0, 0);
  local_D0.x = (&task_1C4)->x;
  (&local_D0)->y = task_1C4.y;
  (&local_D0)->z = task_1C4.z;
  Script_ryoControl(PCCTL_StartCameraFacingInterpolation, 0);
  Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
  Script_SetCharaMoveTarget(arg0, &task_1C4, *(&task_1D0 + 1), local_A4);
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
    if (arg0 == task_1BC) {
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
      if (arg0 == task_1BC) {
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
            if (local_FF50) {
              script_5B87(arg0, task_F4[arg2]);
            }
          }
        }
      }
      if (!(local_C4 > local_B4)) {
        if (!(local_C4 < local_B8) & !(local_C4 > local_B4)) {
          if (arg0 == task_1BC) {
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
          local_FF50 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_A4[arg2], 2);
          local_FF50 = Script_ScenDataUtil(task_64, 4, Script_ScenDataUtil(task_68, 4, local_FF50 * 9 + 1) * 4 + 2);
          if (local_FF50 != -1) {
            if (arg0 == task_1BC) {
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
        local_FF60 = script_38B9((&local_FF64)->y & 0xFFFF, local_88 & 0xFFFF);
        if (local_FF60 > 910) {
          local_88 = (&local_FF64)->y + 910;
        } else {
          if (local_FF60 < -910) {
            local_88 = (&local_FF64)->y - 910;
            goto L008948;
          }
        }
        L008948:;
        if (!(local_C4 >= local_B4)) {
          if (!(local_C0 <= 0.0f)) {
            if (script_38B9(local_FF70, local_88) > 0) {
              local_88 = local_FF70;
            }
          } else {
            if (script_38B9(local_FF70, local_88) < 0) {
              local_88 = local_FF70;
            }
          }
        }
      }
      if (arg0 != task_1BC) {
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
          local_FF74 = (float)script_387C(script_38B9(local_FF70, (&local_FF48)->y) * 360 / 0x10000);
        }
        if (!(local_C4 <= local_B4) & !(local_C4 > local_A8)) {
          *(int32_t *)&local_BC = Script_ScenDataUtil(data_257BC, 5, Script_ScenDataUtil(data_251CC, 1, arg3 * 40 + task_F4[arg2] * 4 + arg4) * 9 + 8);
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
          if (arg0 != task_1BC) {
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
        local_FF50 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_A4[arg2], 2);
        local_FF50 = Script_ScenDataUtil(task_64, 4, Script_ScenDataUtil(task_68, 4, local_FF50 * 9 + 1) * 4 + 3);
        if (local_FF50 != -1) {
          if (arg0 == task_1BC) {
            Script_executeAicaCommand(local_FF50, 0, 0);
          } else {
            Script_Execute3DSoundCommand_Wrapper(local_FF50, 4, &local_D0, 5.0f, 20.0f, 0);
          }
        }
        local_A4 = task_A4[arg2];
        if (arg0 == task_1BC) {
          if (task_1BC == arg0) {
            local_A0 = (Script_ScenDataUtil(task_5C, 4, local_A4 * 13 + 2) >> 16) & 0xFFFF;
            Script_scenToggleCharaFlag(0, local_A0, 1);
            local_A0 = Script_ScenDataUtil(task_5C, 4, local_A4 * 13 + 2) & 0xFFFF;
            Script_scenToggleCharaFlag(0, local_A0, 1);
          }
        }
      }
      if (!(local_AC <= 0.0f)) {
        if (!(local_C4 <= local_AC)) {
          if (arg0 == task_1BC) {
            local_FF5C = arg3;
            if ((local_FF5C == 30) | (local_FF5C == 31) | (local_FF5C == 32) | (local_FF5C == 33) | (local_FF5C == 34) | (local_FF5C == 35)) {
              local_FF78 = local_FF78 - 0.06f;
              if (!(local_FF78 >= 0.3f)) {
                local_FF78 = 0.3f;
              }
              DSWK_SetActorOpacityAndDrawCallback(arg1, local_FF78);
              goto L008DF4;
            }
          }
        }
      }
      L008DF4:;
      if (!(local_C4 < local_B0) & !(local_C4 > local_AC)) {
        if (arg0 == task_1BC) {
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
      if (arg0 != task_1BC) {
        if (!(local_C4 < local_AC)) {
          break;
        }
        local_C4 = local_C4 + 1.0f;
      }
      SCNF_WaitFrames_0000(1);
    }
  }
  script_CB35(arg0, 1);
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
  if (arg0 == task_1BC) {
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
      goto L00902F;
    }
  }
  L00902F:;
  return;
}

__attribute__((scn_string_bindings(0, data_26D4E)))
static void script_9030(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
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
    script_CB35(arg0, 0);
    local_C4.x = (&task_1C4)->x;
    (&local_C4)->y = task_1C4.y;
    (&local_C4)->z = task_1C4.z;
    Script_ryoControl(PCCTL_StartCameraFacingInterpolation, 0);
    Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
    Script_SetCharaMoveTarget(arg0, &task_1C4, *(&task_1D0 + 1), local_94);
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
          local_84 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_A4[arg2], 2);
          local_84 = Script_ScenDataUtil(task_64, 4, Script_ScenDataUtil(task_68, 4, local_84 * 9 + 1) * 4 + 3);
          if (local_84 != -1) {
            if (arg0 == task_1BC) {
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
                goto L009354;
              }
            }
            L009354:;
          } else {
            if ((local_84 == 1) | (local_84 == 3)) {
              if (!(local_B8 > -0.75f)) {
                local_B8 = -0.75f;
              } else {
                if (!(local_B8 < 0.0f)) {
                  local_B8 = 0.0f;
                  goto L009390;
                }
              }
              L009390:;
              goto L009393;
            }
          }
        }
      }
      L009393:;
      if (!(local_BC <= local_A4)) {
        int32_t local_84;
        local_84 = arg4;
        if ((local_84 == 0) | (local_84 == 2)) {
          local_B8 = 0.75f;
        } else {
          if ((local_84 == 1) | (local_84 == 3)) {
            local_B8 = -0.75f;
            goto L0093D8;
          }
        }
      }
      L0093D8:;
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
  script_CB35(arg0, 1);
  {
    int32_t local_94;
    local_94 = task_A4[arg2];
    local_9C = Script_ScenDataUtil(task_5C, 4, local_94 * 13 + 2) & 0xFFFF;
    Script_scenToggleCharaFlag(0, local_9C, 1);
    local_9C = (Script_ScenDataUtil(task_5C, 4, local_94 * 13 + 2) >> 16) & 0xFFFF;
    Script_scenToggleCharaFlag(0, local_9C, 1);
    {
      int32_t unused_88[3];
      int32_t local_84;
      local_84 = Script_ScenDataUtil(task_5C, 4, local_94 * 13 + 1);
      local_84 = local_84 & -9;
      Script_ScenDataUtil(task_5C, 14, local_94 * 13 + 1, local_84);
    }
    {
      int32_t unused_88[3];
      int32_t local_84;
      local_84 = Script_ScenDataUtil(task_5C, 4, local_94 * 13 + 1);
      local_84 = local_84 & -17;
      Script_ScenDataUtil(task_5C, 14, local_94 * 13 + 1, local_84);
    }
    {
      int32_t unused_88[3];
      int32_t local_84;
      local_84 = Script_ScenDataUtil(task_5C, 4, local_94 * 13 + 1);
      local_84 = local_84 & -5;
      Script_ScenDataUtil(task_5C, 14, local_94 * 13 + 1, local_84);
    }
  }
  return;
}

__attribute__((scn_string_bindings(0, data_26D6A)))
static void script_9751(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
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
      local_8C = Script_ScenDataUtil(data_257BC, 4, Script_ScenDataUtil(data_251CC, 1, arg3 * 40 + task_F4[arg2] * 4 + arg4) * 9);
      local_A0 = -1.0f;
      local_88 = 18;
      local_84 = 18;
      local_FF78 = 46421;
      local_FF74 = 48787;
      break;
    case 1:
      local_AF = 1;
      local_8C = Script_ScenDataUtil(data_257BC, 4, Script_ScenDataUtil(data_251CC, 1, arg3 * 40 + task_F4[arg2] * 4 + arg4) * 9) | -0x80000000;
      local_A0 = -1.0f;
      local_88 = 12;
      local_84 = 12;
      local_FF78 = 50426;
      local_FF74 = 48969;
      break;
    case 2:
      local_AF = 1;
      local_8C = Script_ScenDataUtil(data_257BC, 4, Script_ScenDataUtil(data_251CC, 1, arg3 * 40 + task_F4[arg2] * 4 + arg4) * 9);
      local_A0 = 1.0f;
      local_88 = 18;
      local_84 = 18;
      local_FF78 = 15109;
      local_FF74 = 16566;
      break;
    case 3:
      local_AF = 0;
      local_8C = Script_ScenDataUtil(data_257BC, 4, Script_ScenDataUtil(data_251CC, 1, arg3 * 40 + task_F4[arg2] * 4 + arg4) * 9) | -0x80000000;
      local_A0 = 1.0f;
      local_88 = 12;
      local_84 = 12;
      local_FF78 = 19114;
      local_FF74 = 16748;
      break;
  }
  (void)local_FF78;
  *(int32_t *)&local_9C = Script_ScenDataUtil(data_257BC, 5, Script_ScenDataUtil(data_251CC, 1, arg3 * 40 + task_F4[arg2] * 4 + arg4) * 9 + 1);
  *(int32_t *)&local_98 = Script_ScenDataUtil(data_257BC, 5, Script_ScenDataUtil(data_251CC, 1, arg3 * 40 + task_F4[arg2] * 4 + arg4) * 9 + 2);
  *(int32_t *)&local_94 = Script_ScenDataUtil(data_257BC, 5, Script_ScenDataUtil(data_251CC, 1, arg3 * 40 + task_F4[arg2] * 4 + arg4) * 9 + 3);
  *(int32_t *)&local_90 = Script_ScenDataUtil(data_257BC, 5, Script_ScenDataUtil(data_251CC, 1, arg3 * 40 + task_F4[arg2] * 4 + arg4) * 9 + 4);
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
  script_CB35(arg0, 0);
  local_B0.x = (&task_1C4)->x;
  (&local_B0)->y = task_1C4.y;
  (&local_B0)->z = task_1C4.z;
  Script_ryoControl(PCCTL_StartCameraFacingInterpolation, 0);
  Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
  Script_SetCharaMoveTarget(arg0, &task_1C4, *(&task_1D0 + 1), local_8C);
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
        local_FF64 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_A4[arg2], 2);
        local_FF64 = Script_ScenDataUtil(task_64, 4, Script_ScenDataUtil(task_68, 4, local_FF64 * 9 + 1) * 4 + 2);
        if (local_FF64 != -1) {
          if (arg0 == task_1BC) {
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
        local_FF64 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_A4[arg2], 2);
        local_FF64 = Script_ScenDataUtil(task_64, 4, Script_ScenDataUtil(task_68, 4, local_FF64 * 9 + 1) * 4 + 3);
        if (local_FF64 != -1) {
          if (arg0 == task_1BC) {
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
  script_CB35(arg0, 1);
  local_F4.x = 0;
  (&local_F4)->z = 0;
  (&local_F4)->y = local_80;
  Script_charaPosRotUtils(arg1, 12, 0, &local_F4, 0);
  (&local_F4)->y = local_FF7C;
  Script_charaPosRotUtils(arg1, 7, 0, &local_F4, 0);
  return;
}

static void script_9EDB(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
  int32_t local_FC;
  int32_t local_F8;
  Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
  Script_SetCharaMoveTarget(arg0, &task_1C4, *(&task_1D0 + 1), MN_AKI_AKI_BASE);
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
        local_FC = Script_ScenDataUtil(task_5C, 4, task_A4[arg2] * 13 + 7);
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

__attribute__((scn_string_bindings(0, data_26D86)))
static void script_9FB3(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
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
  local_D4 = Script_ScenDataUtil(task_5C, 4, task_A4[arg2] * 13 + 6);
  local_CC = arg3;
  if ((local_CC == 1) | (local_CC == 3) | (local_CC == 5) | (local_CC == 7) | (local_CC == 9) | (local_CC == 18) | (local_CC == 21) | (local_CC == 25) | (local_CC == 27) | (local_CC == 36) | (local_CC == 37)) {
    local_CC = arg4;
    switch (local_CC) {
      case 1:
        local_E0 = Script_ScenDataUtil(&data_261DC, 4, local_D4 * 5) | -0x80000000;
        break;
      case 0:
        local_E0 = Script_ScenDataUtil(&data_261DC, 4, local_D4 * 5);
        break;
    }
  } else if ((local_CC == 2) | (local_CC == 4) | (local_CC == 6) | (local_CC == 8) | (local_CC == 10) | (local_CC == 19) | (local_CC == 22) | (local_CC == 24) | (local_CC == 26) | (local_CC == 28)) {
    local_CC = arg4;
    switch (local_CC) {
      case 1:
        local_E0 = Script_ScenDataUtil(&data_261DC, 4, local_D4 * 5);
        break;
      case 0:
        local_E0 = Script_ScenDataUtil(&data_261DC, 4, local_D4 * 5) | -0x80000000;
        break;
    }
  } else {
    if ((local_CC == 13) | (local_CC == 14) | (local_CC == 15) | (local_CC == 16) | (local_CC == 20) | (local_CC == 23) | (local_CC == 29)) {
      local_CC = arg4;
      switch (local_CC) {
        case 0:
          local_E0 = Script_ScenDataUtil(&data_261DC, 4, local_D4 * 5);
          break;
        case 1:
          local_E0 = Script_ScenDataUtil(&data_261DC, 4, local_D4 * 5) | -0x80000000;
          break;
        case 2:
          local_E0 = Script_ScenDataUtil(&data_261DC, 4, local_D4 * 5);
          break;
        case 3:
          local_E0 = Script_ScenDataUtil(&data_261DC, 4, local_D4 * 5) | -0x80000000;
          break;
      }
      goto L00A1D4;
    }
  }
  L00A1D4:;
  *(int32_t *)&local_EC = Script_ScenDataUtil(&data_261DC, 5, local_D4 * 5 + 1);
  *(int32_t *)&local_E8 = Script_ScenDataUtil(&data_261DC, 5, local_D4 * 5 + 2);
  local_E0 = local_E0 | ((Script_FloatUtils(ScriptMathOp_FloatToInt, local_EC - 3.0f) << 16) & 0x7FFF0000);
  local_F4.x = (&task_1C4)->x;
  (&local_F4)->y = task_1C4.y;
  (&local_F4)->z = task_1C4.z;
  Script_ryoControl(PCCTL_StartCameraFacingInterpolation, 0);
  Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
  Script_SetCharaMoveTarget(arg0, &task_1C4, *(&task_1D0 + 1), local_E0);
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
      local_C8 = Script_ScenDataUtil(task_5C, 4, task_A4[arg2] * 13 + 9);
      if (local_C8 != -1) {
        if (arg0 == task_1BC) {
          Script_executeAicaCommand(local_C8, 0, 0);
        } else {
          Script_Execute3DSoundCommand_Wrapper(local_C8, 4, &local_F4, 5.0f, 20.0f, 0);
        }
      }
    }
    if (!(local_F0 < local_E8) & !(local_F0 >= local_E8 + 1.0f)) {
      if ((local_D4 == 6) | (local_D4 == 7)) {
        int32_t local_C8;
        local_C8 = Script_ScenDataUtil(task_5C, 4, task_A4[arg2] * 13 + 9);
        if (local_C8 != -1) {
          if (arg0 == task_1BC) {
            Script_executeAicaCommand(local_C8, 0, 0);
          } else {
            Script_Execute3DSoundCommand_Wrapper(local_C8, 4, &local_F4, 5.0f, 20.0f, 0);
          }
        }
      }
      if (local_DC == 0) {
        local_DC = 1;
        local_D8 = Script_ScenDataUtil(task_5C, 4, task_A4[arg2] * 13 + 7);
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

__attribute__((scn_string_bindings(0, data_26DA2)))
static void script_A42A(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
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
  task_1E8 = 0;
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
        goto L00A736;
      }
    }
  }
  L00A736:;
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
  local_B8.x = (&task_1C4)->x;
  (&local_B8)->y = task_1C4.y;
  (&local_B8)->z = task_1C4.z;
  Script_ryoControl(PCCTL_StartCameraFacingInterpolation, 0);
  Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
  Script_SetCharaMoveTarget(arg0, &task_1C4, *(&task_1D0 + 1), local_90);
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
        local_FF40 = Script_ScenDataUtil(task_5C, 4, task_A4[arg2] * 13 + 10);
        if (local_FF40 != -1) {
          if (arg0 == task_1BC) {
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
      local_FF40 = Script_ScenDataUtil(task_5C, 4, task_A4[arg2] * 13 + 9);
      if (local_FF40 != -1) {
        if (arg0 == task_1BC) {
          Script_executeAicaCommand(local_FF40, 0, 0);
        } else {
          Script_Execute3DSoundCommand_Wrapper(local_FF40, 4, &local_B8, 5.0f, 20.0f, 0);
        }
      }
    }
    if (!(local_A0 < local_94) & !(local_A0 >= local_94 + 1.0f)) {
      int32_t unused_FF44;
      int32_t local_FF40;
      local_FF40 = Script_ScenDataUtil(task_5C, 4, task_A4[arg2] * 13 + 9);
      if (local_FF40 != -1) {
        if (arg0 == task_1BC) {
          Script_executeAicaCommand(local_FF40, 0, 0);
        } else {
          Script_Execute3DSoundCommand_Wrapper(local_FF40, 4, &local_B8, 5.0f, 20.0f, 0);
        }
      }
      if (local_FF68 == 0) {
        local_FF68 = 1;
        local_FF64 = Script_ScenDataUtil(task_5C, 4, task_A4[arg2] * 13 + 8);
        Script_ConversationUtils(1, local_FF74);
        Script_EnqueueSCNT(1, 0x524F4F44, local_FF64);
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  local_A4 = 0.0f;
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0) != 0) {
    local_FF6C = 0;
    L00B201:;
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
      if (!(local_FF6C >= 20)) goto L00B201;
    }
    {
      int32_t unused_FF44;
      int32_t local_FF40;
      local_FF40 = Script_ScenDataUtil(task_5C, 4, task_A4[arg2] * 13 + 11);
      if (local_FF40 != -1) {
        if (arg0 == task_1BC) {
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

static void script_B3BD(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
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
    if (arg0 != task_1BC) {
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
      Script_SetCharaMoveTarget(arg0, &local_E0, local_BC[1], script_3EE3(arg5));
    }
  }
  return;
}

static void script_B807(int32_t arg0, DSWKDoorEntry *arg1, int32_t *arg2, DoorResourceDescriptor *arg3, MapObjectRecord *arg4, int32_t arg5, int32_t arg6) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t unused_F4;
  task_5C = arg1;
  task_60 = arg2;
  task_64 = arg3;
  task_68 = arg4;
  task_6C.mode = 0;
  task_6C.id = 0;
  task_6C.entry_idx = 0;
  task_1BC = arg0;
  (void)((task_1E0 = -1) & 1);
  task_1DC = 0;
  Script_motiLoad(Script_GetSceneID(), SHEN_NAME('FREE'), "M_MDOR.BIN");
  local_F8 = 0;
  L00B871:;
  local_FC = Script_AddCharacterHandler(1, 0, local_F8 * 0x1000000 + 0x30726F64);
  Script_PreserveAccumulator_012C(local_FC, "This Charactor Work is NULL!!\012");
  DSWK_AllocDoorMapcRenderState(local_FC);
  Script_scenToggleVisibleFor(local_FC, 0);
  task_144[local_F8] = local_FC;
  task_7C[local_F8] = -1;
  task_A4[local_F8] = -1;
  task_CC[local_F8] = 0;
  task_F4[local_F8] = 0;
  task_11C[local_F8] = 0;
  task_16C[local_F8] = 0;
  local_F8 = local_F8 + 1;
  if (!(local_F8 >= 10)) goto L00B871;
  local_F8 = 0;
  for (;;) {
    if (Script_ScenDataUtil(task_5C, 4, local_F8 * 13) == -1) {
      break;
    }
    Script_ScenDataUtil(task_5C, 14, local_F8 * 13 + 1, 0);
    local_F8 = local_F8 + 1;
  }
  Script_EnqueueSCNT(0);
  DSWK_BindTablesAndInitState(arg1, arg2, arg3, arg4, &task_6C, task_7C, task_A4, (shen_name *)task_CC, task_F4, arg0, arg5, data_251CC, data_257BC, (shen_name *)task_144);
  Script_scenCreateTask(script_C0C3, 0);
  return;
}

static void script_B993(DSWKDoorEntry *arg0, int32_t *arg1, DoorResourceDescriptor *arg2, MapObjectRecord *arg3, int32_t arg4) {
  task_5C = arg0;
  task_60 = arg1;
  task_64 = arg2;
  task_68 = arg3;
  DSWK_RebindDoorSystemTables(arg0, arg1, arg2, arg3, arg4);
  task_1DC = 1;
  return;
}

static void script_B9C7(void) {
  int32_t local_FC;
  local_FC = 0;
  L00B9CF:;
  task_7C[local_FC] = -1;
  task_A4[local_FC] = -1;
  task_CC[local_FC] = 0;
  task_F4[local_FC] = 0;
  task_11C[local_FC] = 0;
  task_16C[local_FC] = 0;
  local_FC = local_FC + 1;
  if (!(local_FC >= 10)) goto L00B9CF;
  return;
}

static void script_BA30(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  local_F4 = 0;
  for (;;) {
    Script_sprintf("手動ドア開け番号指定\012");
    if (task_1BC == arg0) {
      Script_EVT_GetPosition(arg0, -1, &task_1F0, 0);
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

static void script_BACD(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  Vector3 local_F0;
  if (task_1BC == arg0) {
    Script_EVT_GetPosition(arg0, -1, &task_1F0, 0);
  }
  Script_EVT_GetPosition(arg0, -1, &local_F0, 0);
  local_FC = DSWK_DoorUtils(DOOR_CMD_FIND_CLOSEST_ELEMENT, arg0, &local_F0, arg1);
  (void)local_FC;
  return;
}

static void script_BB07(void) {
  int32_t local_FC;
  int32_t local_F8;
  local_FC = 0;
  for (;;) {
    if (Script_ScenDataUtil(task_5C, 4, local_FC * 13) == -1) {
      break;
    }
    local_F8 = Script_ScenDataUtil(task_5C, 4, local_FC * 13 + 1);
    local_F8 = local_F8 | 2;
    Script_ScenDataUtil(task_5C, 14, local_FC * 13 + 1, local_F8);
    local_FC = local_FC + 1;
  }
  return;
}

static void script_BB6A(void) {
  int32_t local_FC;
  int32_t local_F8;
  local_FC = 0;
  for (;;) {
    if (Script_ScenDataUtil(task_5C, 4, local_FC * 13) == -1) {
      break;
    }
    local_F8 = Script_ScenDataUtil(task_5C, 4, local_FC * 13 + 1);
    local_F8 = local_F8 & -3;
    Script_ScenDataUtil(task_5C, 14, local_FC * 13 + 1, local_F8);
    local_FC = local_FC + 1;
  }
  return;
}

static void script_BBCD(int32_t arg0) {
  int32_t local_FC;
  local_FC = Script_ScenDataUtil(task_5C, 4, arg0 * 13 + 1);
  local_FC = local_FC | 2;
  Script_ScenDataUtil(task_5C, 14, arg0 * 13 + 1, local_FC);
  return;
}

static void script_BC04(int32_t arg0) {
  int32_t local_FC;
  local_FC = Script_ScenDataUtil(task_5C, 4, arg0 * 13 + 1);
  local_FC = local_FC & -3;
  Script_ScenDataUtil(task_5C, 14, arg0 * 13 + 1, local_FC);
  return;
}

static void script_BC3B(int32_t arg0) {
  Vector3 local_F4;
  int32_t local_F0;
  int32_t local_EC;
  local_F0 = 0;
  for (;;) {
    if (task_A4[local_F0] == arg0) {
      local_EC = task_144[local_F0];
      if (Script_shenGetNode(local_EC, 7)) {
        if (Script_shenGetNode(local_EC, 5) & Script_shenGetNode(local_EC, 10)) {
          if (Script_ScenDataUtil(task_5C, 4, task_A4[local_F0] * 13 + 1) & 0x100) {
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
          if (Script_ScenDataUtil(task_5C, 4, task_A4[local_F0] * 13 + 1) & 0x100) {
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
        if (Script_ScenDataUtil(task_5C, 4, task_A4[local_F0] * 13 + 1) & 0x20) {
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

static void script_BE7D(int32_t arg0) {
  int32_t local_FC;
  local_FC = Script_ScenDataUtil(task_5C, 4, arg0 * 13 + 1);
  local_FC = local_FC | 0x20;
  Script_ScenDataUtil(task_5C, 14, arg0 * 13 + 1, local_FC);
  script_BF85(arg0);
  script_BC3B(arg0);
  return;
}

static void script_BEC6(int32_t arg0) {
  int32_t local_FC;
  local_FC = Script_ScenDataUtil(task_5C, 4, arg0 * 13 + 1);
  local_FC = local_FC & -33;
  Script_ScenDataUtil(task_5C, 14, arg0 * 13 + 1, local_FC);
  script_BF93(arg0);
  script_BC3B(arg0);
  return;
}

static void script_BF0F(int32_t arg0, int32_t arg1) {
  if (arg1) {
    int32_t local_FC;
    local_FC = Script_ScenDataUtil(task_5C, 4, arg0 * 13 + 1);
    local_FC = local_FC & -257;
    Script_ScenDataUtil(task_5C, 14, arg0 * 13 + 1, local_FC);
  } else {
    int32_t local_FC;
    local_FC = Script_ScenDataUtil(task_5C, 4, arg0 * 13 + 1);
    local_FC = local_FC | 0x100;
    Script_ScenDataUtil(task_5C, 14, arg0 * 13 + 1, local_FC);
  }
  return;
}

static void script_BF85(int32_t arg0) {
  script_BF0F(arg0, 0);
  return;
}

static void script_BF93(int32_t arg0) {
  script_BF0F(arg0, 1);
  return;
}

static void script_BFA1(int32_t arg0) {
  int32_t local_FC;
  local_FC = 0;
  for (;;) {
    if (Script_ScenDataUtil(task_5C, 4, local_FC * 13) == -1) {
      break;
    }
    if (arg0) {
      int32_t local_F8;
      local_F8 = Script_ScenDataUtil(task_5C, 4, local_FC * 13 + 1);
      local_F8 = local_F8 & -65;
      local_F8 = local_F8 & -129;
      local_F8 = local_F8 | 0x40;
      Script_ScenDataUtil(task_5C, 14, local_FC * 13 + 1, local_F8);
    } else {
      int32_t local_F8;
      local_F8 = Script_ScenDataUtil(task_5C, 4, local_FC * 13 + 1);
      local_F8 = local_F8 & -65;
      local_F8 = local_F8 & -129;
      local_F8 = local_F8 | 0x80;
      Script_ScenDataUtil(task_5C, 14, local_FC * 13 + 1, local_F8);
    }
    local_FC = local_FC + 1;
  }
  return;
}

static void script_C068(void) {
  script_BFA1(1);
  return;
}

static void script_C074(void) {
  script_BFA1(0);
  return;
}

static void script_C080(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5, int32_t arg6) {
  int32_t local_FC;
  local_FC = 0;
  for (;;) {
    if (Script_ScenDataUtil(task_5C, 4, local_FC * 13) == -1) {
      break;
    }
    DSWK_AssignDoorEntryConfig(local_FC, arg0, arg1, arg2, arg3, arg4, arg5, arg6);
    local_FC = local_FC + 1;
  }
  return;
}

static int32_t script_C0C3(void) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  SCNF_WaitFrames_0000(1);
  SCNF_WaitFrames_0000(1);
  SCNF_WaitFrames_0000(1);
  local_F4 = -1;
  local_F8 = -1;
  for (;;) {
    if (task_1DC == 0) {
      local_F4 = DSWK_DoorUtils(DOOR_CMD_FIND_LOWEST_FLOAT_INDEX, 0x3FD851EA);
      if (-(local_F4 != -1) & -(task_7C[local_F4] == 1) & Script_IsPlayerAllowed(0) & -((&task_6C)->mode == 0)) {
        local_FC = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_A4[local_F4], 2);
        local_F8 = Script_ScenDataUtil(task_64, 4, Script_ScenDataUtil(task_68, 4, local_FC * 9 + 1) * 4 + 1);
        script_CE94(1, local_FC, local_F4, local_F8);
        if (script_D048(local_FC, local_F4, local_F8)) {
          local_F4 = -1;
          local_F8 = -1;
        }
      } else {
        script_CE94(0, local_FC, local_F4, local_F8);
        local_F4 = -1;
        local_F8 = -1;
      }
      script_D25E(local_F4, local_F8);
    } else {
      task_1DC = 0;
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int8_t script_C1D4(int32_t arg0, int32_t arg1, int8_t arg2) {
  Vector3 local_F4;
  Vector3 local_E8;
  Vector3i local_DC;
  int32_t local_D8;
  arg2 = 0;
  Script_EVT_GetPosition(task_1BC, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  Script_mtwkGetSetRotation(task_1BC, &local_DC, 0x42000000);
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

__attribute__((scn_string_bindings(0, data_26E2D, 1, data_26E46)))
static void script_C26C(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  Vector3 local_EC;
  Vector3i local_E0;
  local_F8 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_A4[arg0], 1);
  if (local_F8 != -1) {
    local_FC = Script_AddCharacterHandler(1, 0, arg0 * 0x1000000 + 0x30786F62);
    if (local_FC == 0) {
      Script_PreserveAccumulator_012C(local_FC, "Don't make BOX Buffer!!\012");
      task_194[arg0] = -1;
    } else if (local_FC == arg0 * 0x1000000 + 0x30786F62) {
      task_194[arg0] = local_FC;
      *(int32_t *)&local_EC.x = Script_ScenDataUtil(task_68, 4, local_F8 * 9 + 5);
      *(int32_t *)&(&local_EC)->y = Script_ScenDataUtil(task_68, 4, local_F8 * 9 + 6);
      *(int32_t *)&(&local_EC)->z = Script_ScenDataUtil(task_68, 4, local_F8 * 9 + 7);
      Script_EVT_SetPosition(local_FC, POS_XYZ, &local_EC);
      local_E0.x = 0;
      (&local_E0)->y = Script_ScenDataUtil(task_68, 4, local_F8 * 9 + 8);
      (&local_E0)->z = 0;
      Script_EVT_SetRotation(local_FC, 0x38000000, &local_E0, 0);
      *(int32_t *)&local_EC.x = Script_ScenDataUtil(task_68, 4, local_F8 * 9 + 2);
      *(int32_t *)&(&local_EC)->y = Script_ScenDataUtil(task_68, 4, local_F8 * 9 + 3);
      *(int32_t *)&(&local_EC)->z = Script_ScenDataUtil(task_68, 4, local_F8 * 9 + 4);
      Script_hmdlApplyScale(local_FC, &local_EC);
      Script_CreateNPCImageModule(local_FC, (const char *)Script_ScenDataUtil(task_64, 8, Script_ScenDataUtil(task_68, 4, local_F8 * 9 + 1) * 4), 0);
      Script_charaDisplay(local_FC, CDM_Show);
      Script_charaVisUtils(local_FC, 1);
    } else {
      Script_PreserveAccumulator_012C(local_FC, "Don't make BOX Buffer!!\012");
      task_194[arg0] = -1;
    }
  } else {
    task_194[arg0] = -1;
  }
  return;
}

static void script_C43B(int32_t arg0) {
  if (task_194[arg0] != -1) {
    Script_SetMapControlTaskFlag(task_194[arg0], 1);
    task_194[arg0] = -1;
  }
  return;
}

static int32_t script_C46C(int32_t arg0, int32_t arg1, int32_t arg2, float arg3, float arg4, int32_t arg5, int32_t arg6) {
  Vector3 local_F4;
  Vector3 local_E8;
  float local_DC[3];
  Vector3i local_D0;
  Vector3i local_C4;
  int32_t local_C0;
  int32_t local_BC;
  if (task_1BC == arg0) {
    local_F4.x = (&task_1F0)->x;
    (&local_F4)->y = task_1F0.y;
    (&local_F4)->z = task_1F0.z;
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
        if ((Script_ScenDataUtil(task_5C, 4, arg6 * 13 + 1) & 8) | (Script_ScenDataUtil(task_5C, 4, arg6 * 13 + 1) & 0x40)) {
          local_C0 = local_C0 ^ 3;
        }
      } else {
        if ((Script_ScenDataUtil(task_5C, 4, arg6 * 13 + 1) & 0x10) | (Script_ScenDataUtil(task_5C, 4, arg6 * 13 + 1) & 0x80)) {
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
          if ((local_B4 == 1) | (local_B4 == 2)) goto L00C657;
        }
        L00C657:;
      } else if ((local_B4 == 2) | (local_B4 == 4) | (local_B4 == 6) | (local_B4 == 8) | (local_B4 == 10) | (local_B4 == 12) | (local_B4 == 24) | (local_B4 == 26) | (local_B4 == 31) | (local_B4 == 33) | (local_B4 == 35)) {
        local_B4 = local_C0;
        if ((local_B4 == 2) | (local_B4 == 1)) {
          arg3 = (float)0 - arg3;
        } else {
          if ((local_B4 == 3) | (local_B4 == 0)) {
            arg3 = (float)0 - arg3;
            arg4 = (float)0 - arg4;
            (&local_C4)->y = (&local_C4)->y + 0x8000;
            goto L00C6FA;
          }
        }
        L00C6FA:;
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
            goto L00C8A6;
          }
        }
      }
    }
    L00C8A6:;
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
          task_1C4.x = (&local_F4)->x;
          task_1C4.y = (&local_F4)->y;
          task_1C4.z = (&local_F4)->z;
          task_1D0 = (&local_C4)->x;
          *(&task_1D0 + 1) = (&local_C4)->y;
          *(&task_1D0 + 2) = (&local_C4)->z;
          break;
        case 5:
          task_1C4.x = (&local_F4)->x;
          task_1C4.y = (&local_F4)->y;
          task_1C4.z = (&local_F4)->z;
          task_1D0 = (&local_C4)->x;
          *(&task_1D0 + 1) = (&local_C4)->y;
          *(&task_1D0 + 2) = (&local_C4)->z;
          if ((local_C0 == 1) | (local_C0 == 2)) {
            local_C0 = 1;
          } else {
            local_C0 = 0;
          }
          break;
        case 7:
          Script_EVT_GetPosition(arg0, -1, &task_1C4, 0);
          task_1D0 = (&local_C4)->x;
          *(&task_1D0 + 1) = (&local_C4)->y;
          *(&task_1D0 + 2) = (&local_C4)->z;
          break;
      }
    }
  }
  return local_C0;
}

static void script_CB35(int32_t arg0, int32_t arg1) {
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
    if (task_1BC == arg0) {
      if (arg1) {
        Script_ccowUtils(arg0, 3);
      } else {
        Script_ccowUtils(arg0, 4);
      }
    }
  }
  return;
}

static void script_CB67(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  int32_t local_F8;
  local_F8 = script_C46C(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
  if (task_1BC != arg0) {
    if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
      Script_charaToggleMotEnabled(arg0, 1);
    }
  } else if ((local_F8 == 0) | (local_F8 == 1)) {
    if ((Script_ScenDataUtil(task_5C, 4, arg2 * 13 + 1) & 8) == 0) {
      if ((Script_ScenDataUtil(task_5C, 4, arg2 * 13 + 1) & 0x40) != 0) {
        local_FC = Script_ScenDataUtil(task_5C, 4, arg2 * 13 + 2) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_FC, 0);
      } else {
        local_FC = (Script_ScenDataUtil(task_5C, 4, arg2 * 13 + 2) >> 16) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_FC, 0);
      }
    }
  } else {
    if ((Script_ScenDataUtil(task_5C, 4, arg2 * 13 + 1) & 0x10) == 0) {
      if ((Script_ScenDataUtil(task_5C, 4, arg2 * 13 + 1) & 0x80) != 0) {
        local_FC = (Script_ScenDataUtil(task_5C, 4, arg2 * 13 + 2) >> 16) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_FC, 0);
      } else {
        local_FC = Script_ScenDataUtil(task_5C, 4, arg2 * 13 + 2) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_FC, 0);
      }
    }
  }
  return;
}

__attribute__((scn_string_bindings(0, data_26E5F)))
static void script_CCD7(int32_t arg0, int32_t arg1, int32_t arg2) {
  int32_t local_FC;
  if (task_1BC == arg0) {
    task_6C.mode = 0;
    task_6C.id = 0;
    if ((arg2 == 0) | (arg2 == 2)) {
      Script_PlayCharaMotion(task_1BC, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
      if (task_1E0 & 0x80) {
        Script_sprintf("Door System : ");
        Script_ryoControl(RYO_RestoreControl);
      }
      Script_shenClearFreeState(1);
      if (arg2 != 2) {
        int32_t local_F8;
        local_F8 = task_A4[arg1];
        local_FC = Script_ScenDataUtil(task_5C, 4, local_F8 * 13 + 2) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_FC, 1);
        local_FC = (Script_ScenDataUtil(task_5C, 4, local_F8 * 13 + 2) >> 16) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_FC, 1);
      }
    }
  } else {
    if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
      Script_charaToggleMotEnabled(arg0, 0);
    }
    if ((arg2 == 0) | (arg2 == 2)) {
      int32_t local_F8;
      local_F8 = task_A4[arg1];
      local_FC = Script_ScenDataUtil(task_5C, 4, local_F8 * 13 + 2) & 0xFFFF;
      Script_scenToggleCharaFlag(0, local_FC, 1);
      local_FC = (Script_ScenDataUtil(task_5C, 4, local_F8 * 13 + 2) >> 16) & 0xFFFF;
      Script_scenToggleCharaFlag(0, local_FC, 1);
    }
  }
  task_7C[arg1] = 1;
  task_16C[arg1] = 0;
  task_11C[arg1] = 0;
  task_CC[arg1] = 0;
  if (task_194[arg1] != -1) {
    Script_SetMapControlTaskFlag(task_194[arg1], 1);
    task_194[arg1] = -1;
  }
  return;
}

static void script_CE94(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  Vector3 local_E8;
  local_FC = 0;
  if (arg0 == 1) {
    local_F4 = arg3;
    if ((local_F4 == 17) | (local_F4 == 11) | (local_F4 == 12)) {
      local_FC = 0;
    } else if ((Script_ScenDataUtil(task_5C, 4, task_A4[arg2] * 13 + 1) & 2) == 0) {
      if ((Script_ScenDataUtil(task_5C, 4, task_A4[arg2] * 13 + 1) & 4) == 0) {
        local_FC = script_C1D4(arg2, arg3, local_FC);
      } else {
        local_F4 = arg3;
        if ((local_F4 == 21) | (local_F4 == 22) | (local_F4 == 23)) {
          local_FC = script_C1D4(arg2, arg3, local_FC);
          goto L00CF58;
        }
      }
      L00CF58:;
    } else {
      if ((Script_ScenDataUtil(task_5C, 4, task_A4[arg2] * 13 + 1) & 4) == 0) {
        local_FC = script_C1D4(arg2, arg3, local_FC);
        local_F8 = Script_ScenDataUtil(task_5C, 4, task_A4[arg2] * 13 + 6);
        if (local_F8 == 0) {
          local_FC = 0;
        } else {
          if (local_F8 >= 2) {
            Script_EVT_GetPosition(task_1BC, -1, &local_E8, 0);
            if (Script_CycleManagerUtils(CYCLE_CMD_COUNT_CYCLES_IN_RADIUS, &local_E8, 1.5f)) {
              local_FC = 0;
            }
            goto L00CFF3;
          }
        }
      }
    }
  }
  L00CFF3:;
  if (task_16C[arg2]) {
    local_FC = 0;
  }
  Script_scenUtils(SCENARIO_CMD_SET_HELP_PROMPT_OVERRIDE, local_FC);
  if ((local_FC ^ task_1EC) & local_FC) {
    Script_sicnUtils(0, 4, 1);
  }
  if ((local_FC ^ task_1EC) & task_1EC) {
    Script_sicnUtils(2, 4, 0);
  }
  task_1EC = local_FC;
  return;
}

__attribute__((scn_string_bindings(0, data_26E6E)))
static int32_t script_D048(int32_t arg0, int32_t arg1, int32_t arg2) {
  int32_t local_FC;
  int32_t local_F8;
  Vector3 local_EC;
  local_FC = 1;
  local_F8 = arg2;
  if ((local_F8 == 17) | (local_F8 == 11) | (local_F8 == 12)) {
    if (((Script_ScenDataUtil(task_5C, 4, task_A4[arg1] * 13 + 1) & 2) == 0) & ((Script_ScenDataUtil(task_5C, 4, task_A4[arg1] * 13 + 1) & 4) == 0)) {
      if (task_16C[arg1] == 0) {
        task_7C[arg1] = 4;
        local_FC = 0;
      }
    }
  } else {
    if (Script_GetControllerInput(1) == 0x400) {
      if (-(task_CC[arg1] != task_1BC) & task_1EC) {
        if ((Script_ScenDataUtil(task_5C, 4, task_A4[arg1] * 13 + 1) & 2) == 0) {
          task_7C[arg1] = 3;
          task_CC[arg1] = task_1BC;
          task_F4[arg1] = 0;
          task_6C.mode = 1;
          task_6C.entry_idx = task_A4[arg1];
          local_FC = 0;
        } else {
          if (Script_ScenDataUtil(task_5C, 4, task_A4[arg1] * 13 + 6) >= 2) {
            Script_EVT_GetPosition(task_1BC, -1, &local_EC, 0);
            if (Script_CycleManagerUtils(CYCLE_CMD_COUNT_CYCLES_IN_RADIUS, &local_EC, 1.5f) == 0) {
              task_7C[arg1] = 6;
              task_CC[arg1] = task_1BC;
              task_F4[arg1] = 0;
              task_6C.mode = 0;
              task_6C.entry_idx = task_A4[arg1];
              local_FC = 0;
            }
            goto L00D1FC;
          }
        }
        L00D1FC:;
        if (local_FC == 0) {
          Script_EVT_GetPosition(task_1BC, -1, &task_1F0, 0);
          Script_ryoControl(PCCTL_RequestNewMode, 4);
          Script_sprintf("Door System : ");
          Script_shenClearFreeState(0);
          Script_ryoControl(PCCTL_DisablePlayerControl);
          task_6C.id = task_1BC;
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

static void script_D25E(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  int32_t local_F8;
  local_FC = 0;
  L00D266:;
  local_F8 = task_7C[local_FC];
  switch (local_F8) {
    case 6:
      if (!(arg0 != -1)) goto L00D286;
      L00D286:;
      script_D328(local_FC, arg1);
      break;
    case 5:
      if (!(arg0 != -1)) goto L00D2A6;
      L00D2A6:;
      script_D389(local_FC);
      break;
    case 4:
      if (!(arg0 != -1)) goto L00D2C4;
      L00D2C4:;
      script_D52E(local_FC, arg0, arg1);
      break;
    case 3:
      if (!(arg0 != -1)) goto L00D2E6;
      L00D2E6:;
      script_D5C6(local_FC, arg1);
      break;
    case 2:
      script_D6A8(local_FC);
      break;
    case 1:
      break;
  }
  local_FC = local_FC + 1;
  if (!(local_FC >= 10)) goto L00D266;
  return;
}

static void script_D328(int32_t arg0, int32_t arg1) {
  if (task_16C[arg0] == 0) {
    task_16C[arg0] = Script_scenCreateTask(script_54C2, 4, task_CC[arg0], task_144[arg0], arg0, arg1);
    task_11C[arg0] = task_CC[arg0];
    script_C26C(arg0);
  }
  return;
}

__attribute__((scn_string_bindings(0, data_26E7D, 1, data_26E8F)))
static void script_D389(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, task_CC[arg0])) {
    if (task_16C[arg0] == 0) {
      if (task_CC[arg0] == task_1BC) {
        task_6C.mode = 1;
        task_6C.entry_idx = task_A4[arg0];
        goto L00D3DE;
      }
      L00D3DE:;
      local_FC = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_A4[arg0], 2);
      local_F8 = Script_ScenDataUtil(task_64, 4, Script_ScenDataUtil(task_68, 4, local_FC * 9 + 1) * 4 + 1);
      script_CB67(task_CC[arg0], task_144[arg0], task_A4[arg0], local_F8);
      task_16C[arg0] = Script_scenCreateTask(Script_ScenDataUtil(&data_2627C, 8, local_F8), 4, task_CC[arg0], task_144[arg0], arg0, local_F8);
      task_11C[arg0] = task_CC[arg0];
      if ((Script_ScenDataUtil(task_5C, 4, task_A4[arg0] * 13 + 1) & 0x20000) == 0) {
        Script_sprintf("Door Box is ON!!\012");
        script_C26C(arg0);
      } else {
        Script_sprintf("Door Box is OFF!!\012");
        task_194[arg0] = -1;
      }
    }
  } else {
    if (task_7C[arg0] != 5) {
      task_7C[arg0] = 1;
      task_16C[arg0] = 0;
      task_11C[arg0] = 0;
      task_CC[arg0] = 0;
    }
  }
  return;
}

static void script_D52E(int32_t arg0, int32_t arg1, int32_t arg2) {
  if ((task_16C[arg0] == 0) & (arg2 >= 0)) {
    task_CC[arg0] = task_1BC;
    task_6C.mode = 2;
    task_6C.entry_idx = task_A4[arg0];
    task_16C[arg0] = Script_scenCreateTask(Script_ScenDataUtil(&data_2627C, 8, arg2), 4, task_CC[arg0], task_144[arg0], arg0, arg2);
    task_11C[arg0] = task_CC[arg0];
    script_C26C(arg0);
  }
  return;
}

__attribute__((scn_string_bindings(0, data_26EA2, 1, data_26EB4)))
static void script_D5C6(int32_t arg0, int32_t arg1) {
  if ((task_16C[arg0] == 0) & (arg1 >= 0)) {
    script_CB67(task_CC[arg0], task_144[arg0], task_A4[arg0], arg1);
    task_16C[arg0] = Script_scenCreateTask(Script_ScenDataUtil(&data_2627C, 8, arg1), 4, task_CC[arg0], task_144[arg0], arg0, arg1);
    task_11C[arg0] = task_CC[arg0];
    if ((Script_ScenDataUtil(task_5C, 4, task_A4[arg0] * 13 + 1) & 0x20000) == 0) {
      Script_sprintf("Door Box is ON!!\012");
      script_C26C(arg0);
    } else {
      Script_sprintf("Door Box is OFF!!\012");
      task_194[arg0] = -1;
    }
  }
  return;
}

static void script_D6A8(int32_t arg0) {
  int32_t local_FC;
  task_16C[arg0] = 0;
  task_11C[arg0] = 0;
  task_7C[arg0] = 1;
  local_FC = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_A4[arg0], 2);
  if (local_FC != -1) {
    task_CC[arg0] = 0;
  }
  return;
}

static void script_D6FF(int32_t arg0, float arg1, float arg2, float arg3) {
  Vector3 local_F4;
  local_F4.x = arg1;
  (&local_F4)->y = arg2;
  (&local_F4)->z = arg3;
  Script_EVT_SetPosition(arg0, POS_XYZ, &local_F4);
  return;
}

static void script_D727(int32_t arg0, float arg1, float arg2, float arg3) {
  Vector3i local_F4;
  local_F4.x = (int32_t)(arg1 / 360.0f * (float)0x10000);
  (&local_F4)->y = (int32_t)(arg2 / 360.0f * (float)0x10000);
  (&local_F4)->z = (int32_t)(arg3 / 360.0f * (float)0x10000);
  Script_EVT_SetRotation(arg0, 0x38000000, &local_F4, 0);
  return;
}

static void script_D77B(int32_t arg0, int32_t arg1, int32_t arg2) {
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

static int32_t script_D7CB(int32_t arg0) {
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

static void script_D872(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  local_FC = script_D7CB(arg0);
  if (arg1 > local_FC) {
    (void)0;
  } else {
    (void)-1;
  }
  return;
}

static int32_t script_D895(int32_t arg0) {
  int32_t unused_FC;
  GameDateTime local_F4;
  int32_t local_F0;
  int32_t local_EC;
  Script_ReturnTimeDate(&local_F4);
  local_F0 = (&local_F4)->hours * 60 + (&local_F4)->minutes;
  local_EC = Script_scenUtils(SCENARIO_CMD_GET_STEP_LUT, arg0) * 6;
  return local_F0 - local_EC;
}

static void script_D8C8(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  local_FC = script_D895(arg0);
  if (arg1 > local_FC) {
    (void)0;
  } else {
    (void)-1;
  }
  return;
}

static void script_D8EB(int32_t arg0, int32_t arg1) {
  int8_t local_F4[12];
  int32_t local_F0;
  local_F0 = script_D895(arg0);
  local_F4[4] = local_F0 / 60;
  local_F4[5] = local_F0 % 60;
  if ((local_F4[4] < *(int8_t *)(arg1 + 4)) | ((local_F4[4] == *(int8_t *)(arg1 + 4)) & (local_F4[5] < *(int8_t *)(arg1 + 5)))) {
    (void)0;
  } else {
    (void)-1;
  }
  return;
}

static void script_D94A(int32_t arg0) {
  int32_t local_F8[2];
  local_F8[0] = arg0;
  local_F8[1] = 0;
  (void)local_F8;
  return;
}

__attribute__((scn_string_bindings(0, data_27000)))
static void script_D95F(void) {
  Script_sprintf("TUNED FreeTalk def\012");
  SCNT_Enqueue();
  return;
}

__attribute__((scn_string_bindings(0, data_27014)))
static void script_D96E(void) {
  Script_sprintf("TUNED FreeTalk def\012");
  SCNT_Enqueue();
  return;
}

static void script_D97D(void) {
  return;
}

static void script_D980(void) {
  return;
}

static void script_D983(void) {
  return;
}

static void script_D986(void) {
  return;
}

static void script_D989(void) {
  (void)((task_208 = 0) & 0x20);
  return;
}

static int32_t script_D996(void) {
  return task_1FC = Script_freeTalkGetCurrentChara();
}

static void script_D9A2(void) {
  *(&task_1FC + 1) = Script_GetFreeTalkStatus();
  return;
}

static int32_t script_D9B1(void) {
  return *(&task_1FC + 2) = Script_freeTalkGetAndResetState();
}

static void script_D9C0(void) {
  Script_ryoControl(PCCTL_DisablePlayerControl);
  return;
}

static void script_D9C9(void) {
  Script_ryoControl(RYO_RestoreControl);
  Script_SetGameStateTo(AUTH_None);
  return;
}

static void script_D9D8(int32_t arg0) {
  Script_ccowUtils(arg0, 2);
  if (0) {
    Script_charaDisplay(arg0, CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(arg0, CDM_Show);
      goto L00DA04;
    }
  }
  L00DA04:;
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

static void script_DA65(int32_t arg0) {
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

static void script_DAB5(void) {
  Script_filmCheckOrSetTransitionUtil(80, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_DAE3(int32_t arg0) {
  Script_filmCheckOrSetTransitionUtil(arg0, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_DB11(void) {
  Script_filmCheckOrSetTransitionUtil(80, 255, 255, 255, 255, 0, 255, 255, 255);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_DB45(int32_t arg0) {
  Script_filmCheckOrSetTransitionUtil(arg0, 255, 255, 255, 255, 0, 255, 255, 255);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static int32_t script_DB79(int32_t arg0) {
  return Script_filmCheckOrSetTransitionUtil(arg0, 255, 255, 255, 255, 150, 68, 50, 0);
}

static void script_DB97(int32_t arg0, int32_t arg1, int32_t arg2) {
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

static void script_DBD1(int32_t arg0, int32_t arg1) {
  while (arg1 > 0) {
    Script_SetFightProgramStateBytes(arg0, arg1, 1);
    arg1 = arg1 - 1;
    SCNF_WaitFrames_0000(1);
  }
  Script_GetDataFromFightProgram(arg0, 0);
  return;
}

static int32_t script_DC05(void) {
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
  end:;
}

static void script_DCFD(int32_t arg0, int32_t arg1) {
  Script_FACE_LCG(arg0, 1, arg1, 0);
  SCNF_WaitFrames_0000(arg1 + 1);
  Script_FACE_LCG(arg0, 2, arg1, 0);
  SCNF_WaitFrames_0000(arg1);
  return;
}

static int32_t script_DD2D(void) {
  int16_t local_FE;
  int32_t local_F8;
  local_FE = 0;
  for (;;) {
    local_F8 = local_FE;
    switch (local_F8) {
      case 0:
        Script_FaceEx(CHARA_AKIR, 0, 0, 1);
        break;
      case 150:
        Script_FaceEx(CHARA_FUKU, 0, 0, 1);
        break;
      case 270:
        Script_FaceEx(CHARA_AKIR, 1, 0, 1);
        Script_FaceEx(CHARA_FUKU, 1, 0, 1);
        goto L00DDB4;
        break;
    }
    local_FE = local_FE + 1;
    SCNF_WaitFrames_0000(1);
  }
  L00DDB4:;
  return Script_srchControl(0);
}

static void script_DDBB(void) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  if (-1) {
    Script_charaDisplay(SHEN_NAME('GART'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('GART'), CDM_Show);
      goto L00DDE5;
    }
  }
  L00DDE5:;
  Script_scenToggleVisibleFor(SHEN_NAME('GART'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('SKAB'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('SKAB'), CDM_Show);
      goto L00DE19;
    }
  }
  L00DE19:;
  Script_scenToggleVisibleFor(SHEN_NAME('SKAB'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('KAK1'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('KAK1'), CDM_Show);
      goto L00DE4D;
    }
  }
  L00DE4D:;
  Script_scenToggleVisibleFor(SHEN_NAME('KAK1'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('KAK2'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('KAK2'), CDM_Show);
      goto L00DE81;
    }
  }
  L00DE81:;
  Script_scenToggleVisibleFor(SHEN_NAME('KAK2'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('FIR1'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('FIR1'), CDM_Show);
      goto L00DEB5;
    }
  }
  L00DEB5:;
  Script_scenToggleVisibleFor(SHEN_NAME('FIR1'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('FIR2'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('FIR2'), CDM_Show);
      goto L00DEE9;
    }
  }
  L00DEE9:;
  Script_scenToggleVisibleFor(SHEN_NAME('FIR2'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('FIR3'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('FIR3'), CDM_Show);
      goto L00DF1D;
    }
  }
  L00DF1D:;
  Script_scenToggleVisibleFor(SHEN_NAME('FIR3'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('FIR4'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('FIR4'), CDM_Show);
      goto L00DF51;
    }
  }
  L00DF51:;
  Script_scenToggleVisibleFor(SHEN_NAME('FIR4'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('GRW1'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('GRW1'), CDM_Show);
      goto L00DF85;
    }
  }
  L00DF85:;
  Script_scenToggleVisibleFor(SHEN_NAME('GRW1'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('GRW2'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('GRW2'), CDM_Show);
      goto L00DFB9;
    }
  }
  L00DFB9:;
  Script_scenToggleVisibleFor(SHEN_NAME('GRW2'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('GRW3'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('GRW3'), CDM_Show);
      goto L00DFED;
    }
  }
  L00DFED:;
  Script_scenToggleVisibleFor(SHEN_NAME('GRW3'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('GRW4'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('GRW4'), CDM_Show);
      goto L00E021;
    }
  }
  L00E021:;
  Script_scenToggleVisibleFor(SHEN_NAME('GRW4'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('LKUT'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('LKUT'), CDM_Show);
      goto L00E055;
    }
  }
  L00E055:;
  Script_scenToggleVisibleFor(SHEN_NAME('LKUT'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('RKUT'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('RKUT'), CDM_Show);
      goto L00E089;
    }
  }
  L00E089:;
  Script_scenToggleVisibleFor(SHEN_NAME('RKUT'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('KAWA'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('KAWA'), CDM_Show);
      goto L00E0BD;
    }
  }
  L00E0BD:;
  Script_scenToggleVisibleFor(SHEN_NAME('KAWA'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('KHKS'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('KHKS'), CDM_Show);
      goto L00E0F1;
    }
  }
  L00E0F1:;
  Script_scenToggleVisibleFor(SHEN_NAME('KHKS'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('HAKO'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('HAKO'), CDM_Show);
      goto L00E125;
    }
  }
  L00E125:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAKO'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('MKCS'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('MKCS'), CDM_Show);
      goto L00E159;
    }
  }
  L00E159:;
  Script_scenToggleVisibleFor(SHEN_NAME('MKCS'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('KAGI'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('KAGI'), CDM_Show);
      goto L00E18D;
    }
  }
  L00E18D:;
  Script_scenToggleVisibleFor(SHEN_NAME('KAGI'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('TUBA'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('TUBA'), CDM_Show);
      goto L00E1C1;
    }
  }
  L00E1C1:;
  Script_scenToggleVisibleFor(SHEN_NAME('TUBA'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('PONK'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('PONK'), CDM_Show);
      goto L00E1F5;
    }
  }
  L00E1F5:;
  Script_scenToggleVisibleFor(SHEN_NAME('PONK'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('PONS'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('PONS'), CDM_Show);
      goto L00E229;
    }
  }
  L00E229:;
  Script_scenToggleVisibleFor(SHEN_NAME('PONS'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('TUB2'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('TUB2'), CDM_Show);
      goto L00E25D;
    }
  }
  L00E25D:;
  Script_scenToggleVisibleFor(SHEN_NAME('TUB2'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('POK2'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('POK2'), CDM_Show);
      goto L00E291;
    }
  }
  L00E291:;
  Script_scenToggleVisibleFor(SHEN_NAME('POK2'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('POS2'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('POS2'), CDM_Show);
      goto L00E2C5;
    }
  }
  L00E2C5:;
  Script_scenToggleVisibleFor(SHEN_NAME('POS2'), 0);
  Script_ryoControl(PCCTL_DisablePlayerControl);
  script_FCFB();
  local_F4 = Script_scenCreateTask(script_DD2D, 0);
  Script_FACE(CHARA_FUKU, 10, 0, 0);
  Script_shenSeqUtils(0);
  while (Script_shenSeqUtils(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_srchControl((void *)local_F4);
  Script_FaceEx(CHARA_AKIR, 1, 0, 1);
  Script_FaceEx(CHARA_FUKU, 1, 0, 1);
  local_FC = Script_scenCreateTask(script_E386, 0);
  local_F8 = Script_scenCreateTask(script_FC76, 0);
  Script_shenSeqUtils(1);
  while (Script_shenSeqUtils(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  script_FD2F();
  Script_srchControl((void *)local_FC);
  Script_srchControl((void *)local_F8);
  return;
}

static int32_t script_E386(void) {
  int32_t unused_F4[3];
  int16_t local_F2;
  uint8_t unused_F0[2];
  local_F2 = 0;
  Script_SetMotnFlag(CHARA_AKIR, -1);
  Script_SetMotnFlag(CHARA_FUKU, -1);
  for (;;) {
    if (local_F2 == 0) {
      Script_ResetHandMotion(CHARA_AKIR, 1, 0, 0);
      Script_ResetHandMotion(CHARA_AKIR, 1, 1, 0);
      Script_ResetHandMotion(CHARA_FUKU, 1, 0, 0);
      Script_ResetHandMotion(CHARA_FUKU, 1, 1, 0);
      Script_handSetFloatsFor(CHARA_AKIR, 1, data_27BBC, 0);
      Script_handSetFloatsFor(CHARA_AKIR, 0, data_27BBC, 0);
      Script_handSetFloatsFor(CHARA_FUKU, 1, data_27BBC, 0);
      Script_handSetFloatsFor(CHARA_FUKU, 0, data_27BBC, 0);
    }
    switch (local_F2) {
      case 740:
        Script_FaceEx(CHARA_FUKU, 0, 0, 1);
        Script_ResetHandMotion(CHARA_FUKU, 0, 0, 0);
        Script_ResetHandMotion(CHARA_FUKU, 0, 1, 0);
        break;
      case 1032:
        Script_FaceEx(CHARA_FUKU, 1, 0, 1);
        Script_ResetHandMotion(CHARA_FUKU, 1, 0, 0);
        Script_ResetHandMotion(CHARA_FUKU, 1, 1, 0);
        Script_handSetFloatsFor(CHARA_FUKU, 1, data_27BBC, 0);
        Script_handSetFloatsFor(CHARA_FUKU, 0, data_27BBC, 0);
        break;
    }
    switch (local_F2) {
      case 266:
        Script_FACE_LCG(CHARA_FUKU, 1, 10, 0);
        break;
      case 364:
        Script_FACE_LCG(CHARA_AKIR, 2, 0, 0);
        break;
      case 615:
        Script_FACE_LCG(CHARA_FUKU, 0, 0, 0);
        break;
      case 850:
        Script_FACE_LCG(CHARA_AKIR, 0, 0, 0);
        break;
    }
    if (local_F2 == 1600) {
      Script_executeAicaCommand(2720, 2, 30);
      Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
      while (Script_filmCheckOrSetTransitionUtil(-1)) {
        SCNF_WaitFrames_0000(1);
      }
    }
    local_F2 = local_F2 + 1;
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_E59C(void) {
  int32_t unused_FC;
  script_FCFB();
  Script_shenSeqUtils(0);
  while (Script_shenSeqUtils(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  script_FD2F();
  return;
}

static void script_E5C3(void) {
  int32_t local_FC;
  local_FC = 0;
  for (;;) {
    if (local_FC == 715) {
      Script_filmCheckOrSetTransitionUtil(60, 0, 0, 0, 0, 255, 0, 0, 0);
      while (Script_filmCheckOrSetTransitionUtil(-1)) {
        SCNF_WaitFrames_0000(1);
      }
    }
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_E615(void) {
  int32_t local_FC;
  script_FCFB();
  Script_charaDisplay(SHEN_NAME('TEGS'), CDM_Hide);
  local_FC = Script_scenCreateTask(script_E668, 0);
  Script_shenSeqUtils(0);
  while (Script_shenSeqUtils(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_srchControl((void *)local_FC);
  Script_charaDisplay(SHEN_NAME('TEGS'), CDM_Hide);
  script_FD2F();
  return;
}

static int32_t script_E668(void) {
  int32_t local_FC;
  int32_t unused_F8;
  Vector3 local_EC;
  int32_t local_E0[3];
  Vector3i local_D4;
  Vector3i local_C8;
  Vector3i local_BC;
  int32_t local_B8;
  Script_SetMotnFlag(CHARA_FUKU, 2);
  Script_SetMotnFlag(CHARA_AKIR, -1);
  local_C8.x = 0;
  local_BC.x = 0;
  (&local_C8)->y = 0;
  (&local_BC)->y = 0;
  (&local_C8)->z = 0;
  (&local_BC)->z = 1274;
  Script_charaPosRotUtils(SHEN_NAME('TEGS'), 152, 0, &local_C8, 0);
  Script_charaPosRotUtils(SHEN_NAME('TEGS'), 153, 0, &local_BC, 0);
  local_FC = 0;
  while (Script_shenSeqUtils(-1)) {
    local_B8 = local_FC;
    switch (local_B8) {
      case 1:
        Script_ResetHandMotion(CHARA_AKIR, 1, 1, 0);
        Script_ResetHandMotion(CHARA_AKIR, 1, 0, 0);
        Script_handSetFloatsFor(CHARA_AKIR, 1, data_2782C, 1);
        Script_handSetFloatsFor(CHARA_AKIR, 0, data_27664, 1);
        Script_ResetHandMotion(CHARA_FUKU, 1, 1, 0);
        Script_ResetHandMotion(CHARA_FUKU, 1, 0, 0);
        Script_handSetFloatsFor(CHARA_FUKU, 1, data_27664, 1);
        Script_handSetFloatsFor(CHARA_FUKU, 0, data_27664, 1);
        local_D4.x = -3094;
        (&local_D4)->y = -364;
        (&local_D4)->z = 2730;
        Script_SetHandRootRotation(CHARA_FUKU, 1, 0x15, &local_D4);
        local_D4.x = 0;
        (&local_D4)->y = 0;
        (&local_D4)->z = 2002;
        Script_SetHandRootRotation(CHARA_FUKU, 0, 0x15, &local_D4);
        local_EC.x = 0.123f;
        (&local_EC)->y = 0.016f;
        (&local_EC)->z = -0.052f;
        local_D4.x = 54413;
        (&local_D4)->y = 53684;
        (&local_D4)->z = 22373;
        Script_fixoAttachEx(SHEN_NAME('TEGS'), CHARA_AKIR, 18, &local_EC, &local_D4);
        local_D4.x = -4662;
        (&local_D4)->y = -691;
        (&local_D4)->z = 1742;
        Script_SetHandRootRotation(CHARA_AKIR, 1, 0x15, &local_D4);
        local_E0[0] = 0;
        local_E0[1] = 0;
        local_E0[2] = 0;
        break;
      case 2:
        Script_FACE(CHARA_AKIR, 10, 3, 10);
        break;
      case 90:
        Script_FACE(CHARA_AKIR, 2, 0, 10);
        break;
      case 160:
        Script_FACE(CHARA_FUKU, 1, 4, 10);
        break;
      case 274:
        Script_FACE(CHARA_FUKU, 7, 5, 10);
        break;
      case 320:
        Script_SetCharaLookAtPosition(CHARA_FUKU, -98312, &local_EC, 0);
        break;
      case 350:
        Script_charaDisplay(SHEN_NAME('TEGS'), CDM_Show);
        break;
      case 411:
        Script_FACE(CHARA_FUKU, 5, 5, 10);
        break;
      case 465:
        Script_FACE(CHARA_AKIR, 7, 0, 10);
        break;
      case 475:
        Script_scenToggleActiveFor(CHARA_AKIR, 0);
        Script_scenToggleActiveFor(CHARA_AKIR, 0);
        break;
      case 487:
        Script_FaceLookAtTargetUtils(CHARA_AKIR, 0);
        Script_FACE(CHARA_AKIR, 2, 0, 10);
        break;
      case 511:
        Script_FACE(CHARA_FUKU, 0, 0, 10);
        break;
      case 580:
        Script_FACE(CHARA_FUKU, 4, 5, 10);
        break;
      case 660:
        Script_scenToggleActiveFor(CHARA_AKIR, 1);
        Script_scenToggleActiveFor(CHARA_AKIR, 1);
        break;
      case 715:
        Script_handSetFloatsFor(CHARA_FUKU, 1, data_2749C, 15);
        break;
      case 790:
        Script_FACE(CHARA_AKIR, 7, 0, 10);
        break;
      case 800:
        Script_SetCharaFlags(CHARA_AKIR, 0);
        Script_SetCharaFlags(CHARA_FUKU, 0);
        Script_charaDisplay(SHEN_NAME('TEGS'), CDM_Hide);
        break;
      case 804:
        Script_FaceLookAtTargetUtils(CHARA_AKIR, 0);
        break;
      case 840:
        Script_FACE(CHARA_AKIR, 0, 0, 10);
        break;
      case 900:
        Script_FACE(CHARA_AKIR, 2, 0, 20);
        break;
      case 920:
        Script_executeAicaCommand(2720, 2, 30);
        Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
        break;
    }
    if ((local_FC > 465) & (local_FC < 487)) {
      Script_EVT_GetPosition(CHARA_FUKU, 5, &local_EC, ENTITY_XFORM_WORLD_SPACE);
      Script_FaceLookAtTargetUtils(CHARA_AKIR, 2, &local_EC, 3);
    }
    if ((local_FC > 790) & (local_FC < 804)) {
      Script_EVT_GetPosition(CHARA_FUKU, 5, &local_EC, ENTITY_XFORM_WORLD_SPACE);
      Script_FaceLookAtTargetUtils(CHARA_AKIR, 2, &local_EC, 2);
    }
    if ((local_FC >= 400) & (local_FC <= 423)) {
      (&local_C8)->z = 1274;
      Script_charaPosRotUtils(SHEN_NAME('TEGS'), 152, 0, &local_C8, 1);
    }
    if ((local_FC >= 415) & (local_FC <= 438)) {
      (&local_BC)->z = 1274;
      Script_charaPosRotUtils(SHEN_NAME('TEGS'), 153, 0, &local_BC, 1);
    }
    if ((local_FC >= 760) & (local_FC <= 783)) {
      (&local_BC)->z = -1274;
      Script_charaPosRotUtils(SHEN_NAME('TEGS'), 153, 0, &local_BC, 1);
    }
    if ((local_FC >= 775) & (local_FC <= 798)) {
      (&local_C8)->z = -1274;
      Script_charaPosRotUtils(SHEN_NAME('TEGS'), 152, 0, &local_C8, 1);
    }
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  Script_fixoClearAttachment(SHEN_NAME('HOKI'));
  end:;
}

static void script_EC28(void) {
  int32_t local_FC;
  Script_charaDisplay(SHEN_NAME('MIRR'), CDM_Show);
  Script_charaSetRefboardFlagTo(SHEN_NAME('MIRR'), 1);
  Script_ryoControl(PCCTL_DisablePlayerControl);
  script_FD4A();
  local_FC = Script_scenCreateTask(script_EC8D, 0);
  Script_shenSeqUtils(0);
  while (Script_shenSeqUtils(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  script_FD2F();
  Script_srchControl((void *)local_FC);
  Script_charaDisplay(SHEN_NAME('MIRR'), CDM_Hide);
  return;
}

static int32_t script_EC8D(void) {
  int32_t local_FC;
  Vector3i local_F0;
  local_FC = 0;
  Script_handSetFloatsFor(CHARA_AKIR, 1, data_27CA0, 0);
  local_F0.x = 1274;
  (&local_F0)->y = -546;
  (&local_F0)->z = 728;
  Script_SetHandRootRotation(CHARA_AKIR, 1, 0x15, &local_F0);
  Script_SetMotnFlag(CHARA_AKIR, -1);
  Script_SetMotnFlag(CHARA_FUKU, -1);
  for (;;) {
    if (local_FC == 0) {
      Script_FACE(CHARA_AKIR, 2, 0, 10);
    } else if (local_FC == 121) {
      Script_FACE(CHARA_AKIR, 0, 0, 10);
    } else if (local_FC == 221) {
      Script_FACE(CHARA_AKIR, 2, 0, 10);
    } else if (local_FC == 441) {
      Script_FACE(CHARA_AKIR, 1, 0, 10);
    } else if (local_FC == 531) {
      Script_FACE(CHARA_AKIR, 2, 0, 10);
    } else if (local_FC == 690) {
      Script_FACE(CHARA_AKIR, 7, 0, 10);
    } else if (local_FC == 750) {
      Script_FACE(CHARA_AKIR, 0, 0, 10);
    } else {
      switch (local_FC) {
        case 761:
          Script_FACE(CHARA_AKIR, 2, 0, 10);
          break;
        case 850:
          Script_FACE(CHARA_AKIR, 0, 0, 10);
          break;
        case 951:
          Script_FACE(CHARA_AKIR, 1, 0, 10);
          break;
        case 1150:
          Script_FACE(CHARA_AKIR, 8, 0, 10);
          break;
        case 1190:
          Script_FACE(CHARA_AKIR, 2, 0, 10);
          break;
        case 1321:
          Script_FACE(CHARA_AKIR, 2, 0, 10);
          break;
        case 1426:
          Script_FACE(CHARA_AKIR, 0, 0, 20);
          break;
        case 1710:
          Script_FACE(CHARA_AKIR, 2, 0, 20);
          break;
        case 750:
          Script_FACE(CHARA_AKIR, 2, 0, 10);
          break;
      }
    }
    switch (local_FC) {
      case 0:
        Script_FACE(CHARA_FUKU, 0, 0, 10);
        break;
      case 221:
        Script_FACE(CHARA_FUKU, 2, 0, 10);
        break;
      case 370:
        Script_FACE(CHARA_FUKU, 6, 0, 10);
        break;
      case 441:
        Script_FACE(CHARA_FUKU, 1, 0, 10);
        break;
      case 510:
        Script_FACE(CHARA_FUKU, 7, 5, 10);
        break;
      case 531:
        Script_FACE(CHARA_FUKU, 4, 4, 10);
        break;
      case 640:
        Script_FACE(CHARA_FUKU, 5, 0, 10);
        break;
      case 720:
        Script_FACE(CHARA_FUKU, 0, 0, 10);
        break;
      case 740:
        Script_FACE(CHARA_FUKU, 5, 0, 10);
        break;
      case 761:
        Script_FACE(CHARA_FUKU, 1, 0, 10);
        break;
      case 820:
        Script_FACE(CHARA_FUKU, 7, 5, 10);
        break;
      case 840:
        Script_FACE(CHARA_FUKU, 7, 0, 10);
        break;
      case 951:
        Script_FACE(CHARA_FUKU, 4, 0, 10);
        break;
      case 1080:
        Script_FACE(CHARA_FUKU, 6, 0, 10);
        break;
      case 1190:
        Script_FACE(CHARA_FUKU, 1, 0, 10);
        break;
      case 1250:
        Script_FACE(CHARA_FUKU, 5, 3, 10);
        break;
      case 1321:
        Script_FACE(CHARA_FUKU, 0, 0, 10);
        break;
      case 1401:
        Script_FACE(CHARA_FUKU, 0, 0, 20);
        break;
      case 1440:
        Script_FACE(CHARA_FUKU, 6, 0, 25);
        break;
      case 1470:
        Script_FACE(CHARA_FUKU, 0, 0, 20);
        break;
      case 1500:
        Script_FACE(CHARA_FUKU, 4, 0, 20);
        break;
    }
    if (local_FC == 880) {
      Script_FACE_LCG(CHARA_FUKU, 1, 2, 0);
    }
    if (local_FC == 882) {
      Script_FACE_LCG(CHARA_FUKU, 2, 2, 0);
    }
    if (local_FC == 885) {
      Script_FACE_LCG(CHARA_FUKU, 1, 2, 0);
    }
    if (local_FC == 887) {
      Script_FACE_LCG(CHARA_FUKU, 2, 2, 0);
    }
    if (local_FC == 910) {
      Script_FACE_LCG(CHARA_FUKU, 1, 2, 0);
    }
    if (local_FC == 912) {
      Script_FACE_LCG(CHARA_FUKU, 2, 2, 0);
    }
    if (local_FC == 915) {
      Script_FACE_LCG(CHARA_FUKU, 1, 2, 0);
    }
    if (local_FC == 917) {
      Script_FACE_LCG(CHARA_FUKU, 2, 2, 0);
    }
    if (local_FC == 1200) {
      Script_FACE_LCG(CHARA_FUKU, 1, 2, 0);
    }
    if (local_FC == 1202) {
      Script_FACE_LCG(CHARA_FUKU, 2, 2, 0);
    }
    if (local_FC == 1205) {
      Script_FACE_LCG(CHARA_FUKU, 1, 2, 0);
    }
    if (local_FC == 1207) {
      Script_FACE_LCG(CHARA_FUKU, 2, 2, 0);
    }
    if (local_FC == 1220) {
      Script_FACE_LCG(CHARA_FUKU, 1, 2, 0);
    }
    if (local_FC == 1222) {
      Script_FACE_LCG(CHARA_FUKU, 2, 2, 0);
    }
    if (local_FC == 1225) {
      Script_FACE_LCG(CHARA_FUKU, 1, 2, 0);
    }
    if (local_FC == 1227) {
      Script_FACE_LCG(CHARA_FUKU, 2, 2, 0);
    }
    if (local_FC == 1770) {
      Script_executeAicaCommand(2720, 1, 30);
      Script_executeAicaCommand(2720, 2, 30);
      Script_executeAicaCommand(2720, 3, 30);
      Script_filmCheckOrSetTransitionUtil(40, 0, 0, 0, 0, 255, 0, 0, 0);
      while (Script_filmCheckOrSetTransitionUtil(-1)) {
        SCNF_WaitFrames_0000(1);
      }
    }
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_F323(void) {
  int32_t local_FC;
  Script_charaDisplay(CHARA_AKIR, CDM_Hide);
  script_FDDC(CHARA_JAKR);
  script_FDDC(CHARA_IWAO);
  local_FC = Script_scenCreateTask(script_F3E7, 0);
  Script_shenSeqUtils(0);
  while (Script_shenSeqUtils(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_srchControl((void *)local_FC);
  Script_executeAicaCommand(2720, 2, 60);
  Script_SetMITKFlags(StartFadeOut, 0, 60);
  Script_filmCheckOrSetTransitionUtil(60, 150, 68, 50, 0, 255, 255, 255, 255);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  script_FE98(CHARA_JAKR);
  script_FE98(CHARA_IWAO);
  Script_charaDisplay(CHARA_AKIR, CDM_Show);
  Script_SetGameStateTo(AUTH_None);
  return;
}

static int32_t script_F3E7(void) {
  int32_t local_FC;
  int32_t unused_F8;
  int32_t local_F4;
  int32_t local_F0;
  float local_EC;
  Vector3 local_E0;
  int32_t unused_D8[2];
  int32_t local_D4;
  Script_SetCharaFlags(CHARA_JAKR, 4);
  Script_SetCharaFlags(CHARA_IWAO, 4);
  Script_SetShadowFlags(CHARA_JAKR, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_SetShadowFlags(CHARA_IWAO, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_SetShadowFlags(CHARA_JAKR, SCRIPT_SHADOW_UPDATE_LIGHT_MASK_0, 1);
  Script_SetShadowFlags(CHARA_IWAO, SCRIPT_SHADOW_UPDATE_LIGHT_MASK_0, 1);
  Script_SetShadowFlags(CHARA_JAKR, SCRIPT_SHADOW_UPDATE_LIGHT_MASK_0, 2);
  Script_SetShadowFlags(CHARA_IWAO, SCRIPT_SHADOW_UPDATE_LIGHT_MASK_0, 2);
  Script_SetShadowFlags(CHARA_JAKR, SCRIPT_SHADOW_UPDATE_LIGHT_MASK, 3);
  Script_SetShadowFlags(CHARA_IWAO, SCRIPT_SHADOW_UPDATE_LIGHT_MASK, 3);
  Script_SpriteUtils(Scroll_Read1, (int32_t)"scroll", "SCROLL25.SPR");
  Script_SetMITKFlags(StartFadeIn, 5, 80);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 20.0f);
  Script_SetFogEnabled(0);
  Script_PREP_Related(-1, 0);
  Script_SetupLightFromTable(1);
  Script_SetVector4ByOffset(&data_27D84);
  (void)1;
  Script_charaDisplay(SHEN_NAME('MOON'), CDM_Show);
  Script_motmToggleFlags(CHARA_JAKR, 2, 0x40);
  Script_SetMotnFlag(CHARA_JAKR, 3);
  Script_scenCreateTask(script_DB79, 1, 80);
  Script_filmCheckOrSetTransitionUtil(1, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  local_F4 = Script_scenCreateTask(script_DC05, 0);
  local_FC = 0;
  local_F0 = 100;
  while (Script_shenSeqUtils(-1)) {
    if (local_FC == 0) {
      Script_ResetHandMotion(CHARA_IWAO, 0, 1, 0);
      Script_ResetHandMotion(CHARA_IWAO, 0, 0, 0);
      Script_executeAicaCommand(4008, 0, 0);
      Script_executeAicaCommand(1184, 2, local_F0);
    }
    if (local_FC == 1082) {
      Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 30.0f);
      Script_ResetHandMotion(CHARA_JAKR, 0, 1, 0);
      Script_ResetHandMotion(CHARA_JAKR, 0, 0, 0);
    }
    if (local_FC == 1475) {
      Script_charaDisplay(CHARA_JAKR, CDM_Hide);
    }
    if (local_FC == 1727) {
      Script_charaDisplay(CHARA_JAKR, CDM_Show);
    }
    if (local_FC == 1555) {
      local_E0.x = -10.3f;
      (&local_E0)->y = 1.85f;
      (&local_E0)->z = -19.1f;
      Script_FaceLookAtTargetUtils(CHARA_IWAO, 2, &local_E0, 16);
    }
    local_D4 = local_FC & 3;
    switch (local_D4) {
      case 0:
        local_EC = 1.7f;
        break;
      case 1:
        local_EC = 1.52f;
        break;
      case 2:
        local_EC = 1.36f;
        break;
      case 3:
        local_EC = 1.2f;
        break;
    }
    local_EC = Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 0.75f) + 0.5f;
    Script_LightUtils(1, 4, local_EC);
    Script_LightUtils(2, 4, local_EC);
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
    if (local_FC > 2100) {
      Script_FACE_LCG(CHARA_JAKR, 2, 3, 0);
    }
    if (!(local_FC == 2099)) goto L00F6D1;
    L00F6D1:;
  }
  Script_filmCheckOrSetTransitionUtil(80, 150, 68, 50, 0, 255, 255, 255, 255);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_executeAicaCommand(2720, 2, 64);
  end:;
}

static void script_F70F(void) {
  int32_t local_FC;
  script_FCFB();
  local_FC = Script_scenCreateTask(script_F74C, 0);
  Script_shenSeqUtils(2);
  while (Script_shenSeqUtils(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  script_FD2F();
  Script_srchControl((void *)local_FC);
  return;
}

static int32_t script_F74C(void) {
  int32_t local_FC;
  Vector3 local_F0;
  int32_t local_EC;
  local_FC = 0;
  for (;;) {
    local_EC = local_FC;
    switch (local_EC) {
      case 2:
        Script_FACE(CHARA_AKIR, 1, 0, 10);
        break;
      case 251:
        Script_FACE(CHARA_AKIR, 0, 0, 10);
        break;
      case 521:
        Script_FACE(CHARA_AKIR, 7, 0, 49);
        break;
      case 571:
        Script_FACE(CHARA_AKIR, 2, 0, 129);
        break;
      case 931:
        Script_FACE(CHARA_AKIR, 0, 0, 99);
        break;
      case 1651:
        Script_FACE(CHARA_AKIR, 2, 0, 99);
        break;
      case 1:
        Script_FACE(CHARA_FUKU, 9, 0, 10);
        break;
      case 181:
        Script_FACE(CHARA_FUKU, 0, 0, 10);
        break;
      case 361:
        Script_FACE(CHARA_FUKU, 7, 0, 99);
        break;
      case 461:
        Script_FACE(CHARA_FUKU, 2, 0, 59);
        break;
      case 881:
        Script_FACE(CHARA_FUKU, 3, 0, 10);
        break;
      case 1351:
        Script_FACE(CHARA_FUKU, 7, 0, 49);
        break;
      case 1521:
        Script_FACE(CHARA_FUKU, 6, 0, 49);
        break;
      case 1781:
        Script_FACE(CHARA_FUKU, 7, 0, 79);
        break;
      case 1861:
        Script_FACE(CHARA_FUKU, 3, 0, 119);
        break;
      case 1080:
        Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
        Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
        Script_ResetHandMotion(CHARA_FUKU, 0, 0, 0);
        Script_ResetHandMotion(CHARA_FUKU, 0, 1, 0);
        break;
      case 1321:
        Script_ResetHandMotion(CHARA_AKIR, 1, 0, 0);
        Script_ResetHandMotion(CHARA_AKIR, 1, 1, 0);
        Script_ResetHandMotion(CHARA_FUKU, 1, 0, 0);
        Script_ResetHandMotion(CHARA_FUKU, 1, 1, 0);
        Script_handSetFloatsFor(CHARA_AKIR, 1, data_27BBC, 0);
        Script_handSetFloatsFor(CHARA_AKIR, 0, data_27BBC, 0);
        Script_handSetFloatsFor(CHARA_FUKU, 1, data_27BBC, 0);
        Script_handSetFloatsFor(CHARA_FUKU, 0, data_27BBC, 0);
        break;
    }
    if ((local_FC > 1350) & (local_FC < 1450)) {
      Script_EVT_GetPosition(CHARA_AKIR, 5, &local_F0, ENTITY_XFORM_WORLD_SPACE);
      Script_FaceLookAtTargetUtils(CHARA_FUKU, 2, &local_F0, 10);
    }
    if (local_FC == 1500) {
      Script_FaceLookAtTargetUtils(CHARA_FUKU, 0);
    }
    if (local_FC == 1950) {
      Script_executeAicaCommand(2720, 2, 30);
      Script_filmCheckOrSetTransitionUtil(50, 0, 0, 0, 0, 255, 0, 0, 0);
      while (Script_filmCheckOrSetTransitionUtil(-1)) {
        SCNF_WaitFrames_0000(1);
      }
    }
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_FA8C(void) {
  int32_t local_FC;
  if (-1) {
    Script_charaDisplay(CHARA_FUKU, CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(CHARA_FUKU, CDM_Show);
      goto L00FAB6;
    }
  }
  L00FAB6:;
  Script_scenToggleVisibleFor(CHARA_FUKU, 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('DDO1'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('DDO1'), CDM_Show);
      goto L00FAEA;
    }
  }
  L00FAEA:;
  Script_scenToggleVisibleFor(SHEN_NAME('DDO1'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('DDO2'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('DDO2'), CDM_Show);
      goto L00FB1E;
    }
  }
  L00FB1E:;
  Script_scenToggleVisibleFor(SHEN_NAME('DDO2'), 0);
  Script_ryoControl(PCCTL_DisablePlayerControl);
  script_FD8D();
  Script_mapmSetCamFlags(0, 0);
  Script_filmCheckOrSetTransitionUtil(60, 255, 0, 0, 0, 0, 0, 0, 0);
  local_FC = Script_scenCreateTask(script_FC04, 0);
  Script_shenSeqUtils(3);
  while (Script_shenSeqUtils(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_executeAicaCommand(2720, 1, 30);
  Script_executeAicaCommand(2720, 2, 30);
  Script_executeAicaCommand(2720, 3, 30);
  Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_mapmSetCamFlags(0, 1);
  script_FDC1();
  Script_scenToggleVisibleFor(CHARA_FUKU, 1);
  Script_scenToggleVisibleFor(SHEN_NAME('DDO1'), 1);
  Script_scenToggleVisibleFor(SHEN_NAME('DDO2'), 1);
  Script_srchControl((void *)local_FC);
  return;
}

static int32_t script_FC04(void) {
  int32_t local_FC;
  local_FC = 0;
  Script_charaVisUtils(CHARA_INE_, 1);
  Script_charaDisplay(CHARA_INE_, CDM_Show);
  for (;;) {
    if (local_FC == 561) {
      Script_charaDisplay(CHARA_INE_, CDM_Hide);
      Script_mapmSetCamFlags(0, 1);
      Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 60.0f);
    }
    if (local_FC == 734) {
      Script_charaDisplay(CHARA_INE_, CDM_Show);
    }
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_FC76(void) {
  int32_t local_FC;
  local_FC = 0;
  for (;;) {
    if ((local_FC > 0) & (local_FC < 255)) {
      if (!(Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 1.0f) >= 0.0725f)) {
        Script_FACE_LCG(CHARA_FUKU, 1, 2, 0);
        local_FC = local_FC + 2;
        SCNF_WaitFrames_0000(2);
        Script_FACE_LCG(CHARA_FUKU, 2, 2, 0);
        local_FC = local_FC + 2;
        SCNF_WaitFrames_0000(2);
      }
    }
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_FCFB(void) {
  script_FDDC(CHARA_AKIR);
  script_FDDC(CHARA_FUKU);
  Script_SetMotnFlag(CHARA_FUKU, -1);
  Script_SetVector4ByOffset(&data_27DB8);
  (void)1;
  return;
}

static void script_FD2F(void) {
  script_FE98(CHARA_AKIR);
  script_FE98(CHARA_FUKU);
  return;
}

static void script_FD4A(void) {
  script_FDDC(CHARA_AKIR);
  script_FDDC(CHARA_FUKU);
  Script_SetMotnFlag(CHARA_FUKU, -1);
  Script_SetVector4ByOffset(&data_27DD8);
  (void)1;
  Script_SetCharaRefBoardPos(SHEN_NAME('MIRR'), &data_27DE8);
  return;
}

static void script_FD8D(void) {
  script_FDDC(CHARA_AKIR);
  script_FDDC(CHARA_INE_);
  Script_SetMotnFlag(CHARA_AKIR, -1);
  Script_SetVector4ByOffset(&data_27DF8);
  (void)1;
  return;
}

static void script_FDC1(void) {
  script_FE98(CHARA_AKIR);
  script_FE98(CHARA_INE_);
  return;
}

static void script_FDDC(int32_t arg0) {
  Script_ccowUtils(arg0, 2);
  Script_ccowSetFlag80(arg0, 1);
  Script_motmGetCharaFlags(arg0, 0, 0);
  Script_facemotUtils(arg0, 0, 0);
  Script_facemotUtils(arg0, 1, 0);
  Script_FaceEx(arg0, 1, 0, 1);
  Script_ResetHandMotion(arg0, 1, 1, 0);
  Script_ResetHandMotion(arg0, 1, 0, 0);
  Script_handSetFloatsFor(arg0, 0, data_273B8, 10);
  Script_handSetFloatsFor(arg0, 1, data_273B8, 10);
  Script_SetCPAMFlags(arg0, 0);
  Script_FaceLookAtTargetUtils(arg0, 0);
  Script_charaSetRefboardFlagTo(arg0, 1);
  Script_SetCharaRefBoardPos(arg0, &data_27E08);
  return;
}

static void script_FE78(int32_t arg0) {
  Script_ccowUtils(arg0, 2);
  Script_ccowSetFlag80(arg0, 1);
  Script_CharHandlers(arg0, 2, 8, 16);
  return;
}

static void script_FE98(int32_t arg0) {
  Script_ccowUtils(arg0, 1);
  Script_SetCPAMFlags(arg0, 1);
  Script_FaceLookAtTargetUtils(arg0, 0);
  Script_SetCharaLookAtPosition(arg0, -98310, 0, 0);
  Script_ResetHandMotion(arg0, 0, 1, 0);
  Script_ResetHandMotion(arg0, 0, 0, 0);
  return;
}

static void script_FEDE(void) {
  if (task_238) {
    Script_CycleManagerUtils(CYCLE_CMD_LOAD_OR_RESET_RUNNING_PROGRAM, task_238);
    Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT, 0x554B5546, 0);
    task_240 = 0;
  }
  if (task_23C) {
    Script_CycleManagerUtils(CYCLE_CMD_LOAD_OR_RESET_RUNNING_PROGRAM, task_23C);
    Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT, 0x5F454E49, 0);
    task_244 = 0;
  }
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 1);
  return;
}

static void script_FF36(void) {
  int32_t unused_FC;
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
  Script_CycleManagerUtils(CYCLE_CMD_PROCESS_SND);
  task_238 = 0;
  task_23C = 0;
  if (task_240 == 0) {
    Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT, 0x554B5546, 1);
    task_238 = Script_CycleManagerUtils(CYCLE_CMD_GET_CYCLE_PTR_BY_CHARA_ID, 0x554B5546);
    while (task_238 == 0) {
      Script_sprintf("ふくちゃん読み込み失敗\012");
      SCNF_WaitFrames_0000(1);
      task_238 = Script_CycleManagerUtils(CYCLE_CMD_GET_CYCLE_PTR_BY_CHARA_ID, 0x554B5546);
    }
    while (Script_CycleManagerUtils(CYCLE_CMD_GET_NEXT_TASK_POINTER, task_238) == 0) {
      SCNF_WaitFrames_0000(1);
    }
    Script_CycleManagerUtils(CYCLE_CMD_SET_RUNNING_PROGRAM_FIELD_B8, 0x554B5546, 0);
    task_240 = 1;
    script_D6FF(CHARA_FUKU, -1000.0f, 0.0f, -1000.0f);
  }
  if (task_244 == 0) {
    Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT, 0x5F454E49, 1);
    task_23C = Script_CycleManagerUtils(CYCLE_CMD_GET_CYCLE_PTR_BY_CHARA_ID, 0x5F454E49);
    while (task_23C == 0) {
      Script_sprintf("いねさん読み込み失敗\012");
      SCNF_WaitFrames_0000(1);
      task_23C = Script_CycleManagerUtils(CYCLE_CMD_GET_CYCLE_PTR_BY_CHARA_ID, 0x5F454E49);
    }
    while (Script_CycleManagerUtils(CYCLE_CMD_GET_NEXT_TASK_POINTER, task_23C) == 0) {
      SCNF_WaitFrames_0000(1);
    }
    Script_CycleManagerUtils(CYCLE_CMD_SET_RUNNING_PROGRAM_FIELD_B8, 0x5F454E49, 0);
    task_244 = 1;
    script_D6FF(CHARA_INE_, -1000.0f, 0.0f, -1000.0f);
  }
  return;
}

static void script_1009F(void) {
  if (task_229 == 0) {
    Script_SetGlobalSceneSlotValue34(1, 1);
    Script_SetGlobalSceneSlotValue34(3, 1);
    Script_PREP_Related(-1, 0);
    Script_SetupLightFromTable(0);
    Script_PREP_Related(0, 1);
  }
  return;
}

static void script_100D7(void) {
  if (task_229 == 0) {
    Script_SetGlobalSceneSlotValue34(1, 0);
    Script_SetGlobalSceneSlotValue34(3, 0);
    Script_shenSetLightingMode(1);
    Script_PREP_Related(-1, 0);
  }
  return;
}

static int32_t script_10105(int32_t arg0, int32_t arg1) {
  Vector3 local_F4;
  Vector3 local_E8;
  Vector3i local_DC;
  int32_t local_D8;
  Script_EVT_GetPosition(arg0, -1, &local_F4, 0);
  Script_EVT_GetPosition(arg1, -1, &local_E8, 0);
  local_E8.x = (&local_E8)->x - (&local_F4)->x;
  (&local_E8)->z = (&local_E8)->z - (&local_F4)->z;
  Script_mtwkGetSetRotation(arg0, &local_DC, 0x2000000);
  (&local_DC)->y = (&local_DC)->y - Script_FloatUtils(ScriptMathOp_Atan2ReverseVector, (&local_E8)->x, (&local_E8)->z) + 7281;
  if (((&local_DC)->y < 14563) & ((&local_DC)->y > 0)) {
    local_D8 = 1;
  } else {
    local_D8 = 0;
  }
  end:;
}

static int32_t script_10195(void) {
  int32_t unused_E0[8];
  task_26C = 0;
  Script_EVT_SetPosition(SHEN_NAME('KAK1'), POS_XYZ, &data_27E54);
  Script_EVT_SetPosition(SHEN_NAME('KAK2'), POS_XYZ, &data_27E60);
  Script_charaDisplay(SHEN_NAME('POK2'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('POS2'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('MKCS'), CDM_Hide);
  Script_ccowSetFlag80(SHEN_NAME('DDO1'), 1);
  Script_ccowSetFlag80(SHEN_NAME('DDO2'), 1);
  task_274 = 0;
  task_275 = 0;
  task_276 = 0;
  task_277 = 0;
  task_278 = 0;
  task_279 = 0;
  task_27A = 0;
  task_27B = 0;
  task_27C = 0;
  task_27D = 0;
  task_27E = 0;
  task_27F = 0;
  task_280 = 0;
  Script_scenToggleCharaFlag(0, 200, 0);
  task_282 = 0;
  task_283 = 0;
  task_284 = 0;
  task_285 = 0;
  task_286 = 0;
  task_287 = 0;
  task_288 = 0;
  task_28C = 0;
  Script_scenCreateTask(script_10A78, 0);
  Script_facemotUtils(CHARA_AKIR, 1, 0);
  for (;;) {
    if (task_26C == 1) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_102CA(void) {
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_ROTATING_DOOR_UNLOCKED)) {
    Script_srchUtils(SRCH_SCRIPT_InitEntryTableFromTaskOffset, &data_27ED4);
  } else if (task_22D | (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_BASEMENT_SWORD_FOUND) == 0) | Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_KATANA_INSERTED)) {
    if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_KAK1_HANGING_SCROLL_TAKEN)) {
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_KAK2_HANGING_SCROLL_TAKEN)) {
        Script_srchUtils(SRCH_SCRIPT_InitEntryTableFromTaskOffset, &data_28424);
      } else {
        Script_srchUtils(SRCH_SCRIPT_InitEntryTableFromTaskOffset, &data_2815C);
      }
    } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_KAK2_HANGING_SCROLL_TAKEN)) {
      Script_srchUtils(SRCH_SCRIPT_InitEntryTableFromTaskOffset, &data_282C0);
    } else {
      Script_srchUtils(SRCH_SCRIPT_InitEntryTableFromTaskOffset, &data_27FB8);
    }
  } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_KAK1_HANGING_SCROLL_TAKEN)) {
    if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_KAK2_HANGING_SCROLL_TAKEN)) {
      Script_srchUtils(SRCH_SCRIPT_InitEntryTableFromTaskOffset, &data_28A14);
    } else {
      Script_srchUtils(SRCH_SCRIPT_InitEntryTableFromTaskOffset, &data_2870C);
    }
  } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_KAK2_HANGING_SCROLL_TAKEN)) {
    Script_srchUtils(SRCH_SCRIPT_InitEntryTableFromTaskOffset, &data_28890);
  } else {
    Script_srchUtils(SRCH_SCRIPT_InitEntryTableFromTaskOffset, &data_28548);
  }
  return;
}

static void script_103C5(void) {
  return;
}

static void script_103C8(void) {
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_DOJO_SWORD_DISPLAYED) | (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_BASEMENT_SWORD_FOUND) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_KATANA_INSERTED) == 0))) {
    if (task_22D) {
      Script_charaDisplay(SHEN_NAME('POK2'), CDM_Show);
      Script_charaDisplay(SHEN_NAME('POS2'), CDM_Show);
      script_14F6C();
    } else {
      Vector3 local_F4;
      Vector3i local_E8;
      local_F4.x = -2.3825f;
      (&local_F4)->y = 1.7465f;
      (&local_F4)->z = -24.4261f;
      local_E8.x = 2389;
      (&local_E8)->y = 64763;
      (&local_E8)->z = 62968;
      Script_EVT_SetPosition(SHEN_NAME('POK2'), POS_XYZ, &local_F4);
      Script_EVT_SetRotation(SHEN_NAME('POK2'), 0x38000000, &local_E8, 0);
      Script_EVT_SetPosition(SHEN_NAME('POS2'), POS_XYZ, &local_F4);
      Script_EVT_SetRotation(SHEN_NAME('POS2'), 0x38000000, &local_E8, 0);
      Script_charaDisplay(SHEN_NAME('POK2'), CDM_Show);
      Script_charaDisplay(SHEN_NAME('POS2'), CDM_Show);
    }
  }
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_KAK1_HANGING_SCROLL_TAKEN) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_PROMISED_TERRY_TO_FIGHT_CHEN_GUI_ZHANG) == 0)) {
    Vector3 local_F4;
    Vector3i local_E8;
    local_F4.x = -9.8f;
    (&local_F4)->y = 1.0565f;
    (&local_F4)->z = -24.112f;
    local_E8.x = 29190;
    (&local_E8)->y = 50373;
    (&local_E8)->z = 3288;
    Script_EVT_SetPosition(SHEN_NAME('KAK3'), POS_XYZ, &local_F4);
    Script_EVT_SetRotation(SHEN_NAME('KAK3'), 0x38000000, &local_E8, 0);
    Script_charaDisplay(SHEN_NAME('KAK3'), CDM_Show);
    Script_charaDisplay(SHEN_NAME('KAK1'), CDM_Hide);
  }
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_KAK2_HANGING_SCROLL_TAKEN) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_PROMISED_TERRY_TO_FIGHT_CHEN_GUI_ZHANG) == 0)) {
    Vector3 local_F4;
    Vector3i local_E8;
    local_F4.x = -9.8f;
    (&local_F4)->y = 1.0565f;
    (&local_F4)->z = -22.6f;
    local_E8.x = 29190;
    (&local_E8)->y = 50373;
    (&local_E8)->z = 3288;
    Script_EVT_SetPosition(SHEN_NAME('KAK4'), POS_XYZ, &local_F4);
    Script_EVT_SetRotation(SHEN_NAME('KAK4'), 0x38000000, &local_E8, 0);
    Script_charaDisplay(SHEN_NAME('KAK4'), CDM_Show);
    Script_charaDisplay(SHEN_NAME('KAK2'), CDM_Hide);
  }
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_TSUBA_PLACED) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_PROMISED_TERRY_TO_FIGHT_CHEN_GUI_ZHANG) == 0)) {
    Script_fixoAttachEx(SHEN_NAME('TUB2'), SHEN_NAME('SKAB'), 0, &data_28B70, &data_28B7C);
    Script_charaDisplay(SHEN_NAME('TUB2'), CDM_Show);
  }
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_KATANA_INSERTED) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_PROMISED_TERRY_TO_FIGHT_CHEN_GUI_ZHANG) == 0)) {
    Script_fixoAttachEx(SHEN_NAME('POK2'), SHEN_NAME('SKAB'), 0, &data_28B58, &data_28B64);
    Script_charaDisplay(SHEN_NAME('POK2'), CDM_Show);
    Script_charaDisplay(SHEN_NAME('POS2'), CDM_Show);
  }
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_ROTATING_DOOR_UNLOCKED) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_PROMISED_TERRY_TO_FIGHT_CHEN_GUI_ZHANG) == 0)) {
    Script_EVT_SetRotation(SHEN_NAME('SKAB'), 0x38000000, &data_28B88, 0);
    Script_scenToggleCharaFlag(0, 202, 0);
  }
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_PROMISED_TERRY_TO_FIGHT_CHEN_GUI_ZHANG)) {
    if (Script_GetOwnedItemCount(4, 0) == 0) {
      Script_GetCassetteFlags(4, 1, 0, 0);
    }
  }
  return;
}

static int32_t script_106C8(void) {
  for (;;) {
    if (-(task_228 == 0) & task_230) {
      task_264 = Script_srchUtils(-1);
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_106F1(void) {
  int32_t local_FC;
  SAVE_Script_UpdateForSaveData(0, 0);
  SAVE_Script_UpdateForSaveData(3, 0, 14400);
  for (;;) {
    if (-(task_228 == 0) & Script_GetOwnedItemCount(4, 0) & Script_SetTaskDataValue(CHARA_AKIR, &data_28B94, &data_28BA0, 0)) {
      local_FC = SAVE_Script_UpdateForSaveData(4, 0);
      if ((local_FC == 0) & (Script_srchUtils(SRCH_SCRIPT_GetCurrentTargetEntryIndex) < 0)) {
        if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_BASEMENT_SWORD_FOUND)) {
          if ((Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_KAK1_HANGING_SCROLL_TAKEN) == 0) & (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_KAK2_HANGING_SCROLL_TAKEN) == 0)) {
            Script_ryoControl(PCCTL_DisablePlayerControl);
            Script_shenClearFreeState(0);
            Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
            Script_FaceEx(CHARA_AKIR, 1, 0, 2);
            if (task_281 == 0) {
              task_281 = 1;
              Script_seqEnterEvtCam(1011, CHARA_AKIR, CHARA_AKIR);
              Script_ShowUIMessageFromID("E1011A109");
              while (Script_convGetIsActive(0)) {
                SCNF_WaitFrames_0000(1);
              }
              Script_seqEnterEvtCam(690, 0, AUTH_None);
              SCNF_WaitFrames_0000(30);
              Script_ShowUIMessageFromID("E1011A110");
              while (Script_convGetIsActive(0)) {
                SCNF_WaitFrames_0000(1);
              }
            } else {
              Script_seqEnterEvtCam(1011, CHARA_AKIR, CHARA_AKIR);
              Script_ShowUIMessageFromID("E1011A111");
              while (Script_convGetIsActive(0)) {
                SCNF_WaitFrames_0000(1);
              }
              Script_ShowUIMessageFromID("E1011A112");
              while (Script_convGetIsActive(0)) {
                SCNF_WaitFrames_0000(1);
              }
              Script_seqEnterEvtCam(690, 0, AUTH_None);
              SCNF_WaitFrames_0000(30);
              Script_ShowUIMessageFromID("E1011A113");
              while (Script_convGetIsActive(0)) {
                SCNF_WaitFrames_0000(1);
              }
            }
            Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_HANGING_SCROLL_MEANING, 1);
            Script_FaceEx(CHARA_AKIR, 0, 0, 2);
            Script_convSetCamInactiveFlag(0);
            Script_SetGameStateTo(AUTH_None);
            Script_ryoControl(RYO_RestoreControl);
            SAVE_Script_UpdateForSaveData(3, 0, 14400);
            Script_shenClearFreeState(1);
          }
        } else {
          if (Script_GetOwnedItemCount(39, 0) == 0) {
            Script_seqEnterEvtCam(680, 0, AUTH_None);
            SCNF_WaitFrames_0000(1);
            Script_srchUtils(SRCH_SCRIPT_SetExplicitTargetMapInfoId, 0x534B484B);
            if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_KHKS_HINT_INTRO_SEEN)) {
              Script_ryoControl(PCCTL_DisablePlayerControl);
              Script_shenClearFreeState(0);
              Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
              task_281 = 1;
              SCNF_WaitFrames_0000(30);
              task_228 = 1;
              Script_ShowUIMessageFromID("E1011A107");
              while (Script_convGetIsActive(0)) {
                SCNF_WaitFrames_0000(1);
              }
              Script_ShowUIMessageFromID("E1011A108");
              while (Script_convGetIsActive(0)) {
                SCNF_WaitFrames_0000(1);
              }
              Script_srchUtils(SRCH_SCRIPT_Reset);
              task_281 = 0;
              Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_ANYTHING_IN_DADS_ROOM, 1);
              task_228 = 0;
              Script_convSetCamInactiveFlag(0);
              Script_SetGameStateTo(AUTH_None);
              Script_ryoControl(RYO_RestoreControl);
              Script_shenClearFreeState(1);
            } else {
              SCNF_WaitFrames_0000(30);
            }
            SAVE_Script_UpdateForSaveData(3, 0, 14400);
            goto L010A04;
          }
        }
      }
    }
    L010A04:;
    SCNF_WaitFrames_0000(1);
  }
  return Script_srchControl(0);
}

static int32_t script_10A16(void) {
  int8_t local_FF;
  local_FF = 0;
  for (;;) {
    if (Script_SetTaskDataValue(CHARA_AKIR, &data_28BF4, &data_28C00, 0)) {
      if (local_FF == 0) {
        local_FF = 1;
        Script_SetMaxCharaSearchExtent(1, 0.1f);
      }
    } else {
      if (local_FF) {
        local_FF = 0;
        Script_SetMaxCharaSearchExtent(1, 3.0f);
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_10A78(void) {
  int32_t unused_E8[6];
  int32_t local_E4;
  int32_t local_E0;
  int32_t local_DC;
  Vector3i local_D0;
  int32_t local_CC;
  int32_t local_C8;
  Script_SetMaxCharaSearchExtent(1, 3.0f);
  script_102CA();
  Script_InvestigationUtils(ShenmueUtils_SetupOrSyncAnchors, SHEN_NAME('YKUL'), 0x52554B59);
  script_103C8();
  Script_scenCreateTask(script_10A16, 0);
  Script_scenCreateTask(script_106C8, 0);
  Script_scenCreateTask(script_106F1, 0);
  for (;;) {
    if (-(task_228 == 0) & task_230) {
      local_CC = Script_StepUtils(GET_STEP_HIGHER, 0);
      Script_mtwkGetSetRotation(CHARA_AKIR, &local_D0, 0x2000000);
      if ((task_264 == -1) & (Script_srchUtils(SRCH_SCRIPT_GetCurrentTargetEntryIndex) == -1)) {
        if (Script_GetControllerInput(1) & 0x400) {
          while (Script_IsPlayerAllowed(0) == 0) {
            SCNF_WaitFrames_0000(1);
          }
          if (task_284) {
            script_1140E();
          } else if (task_285) {
            script_114F3();
          } else if (task_287) {
            Script_srchUtils(SRCH_SCRIPT_SetExplicitTargetMapInfoId, 0x534B484B);
          } else if (task_283) {
            Script_srchUtils(SRCH_SCRIPT_SetExplicitTargetMapInfoId, 11);
          } else if (task_288) {
            Script_srchUtils(SRCH_SCRIPT_SetExplicitTargetMapInfoId, 0x324B4F50);
          } else if (task_28C) {
            Script_srchUtils(SRCH_SCRIPT_SetExplicitTargetMapInfoId, 0x4F4B4148);
          } else {
            if (task_22D) {
              script_115FA();
              goto L010BAF;
            }
          }
        }
        L010BAF:;
      } else {
        local_C8 = task_264;
        switch (local_C8) {
          case 0x324B4F50:
            script_14D8D();
            break;
          case 0x534B484B:
            break;
          case 0x54524147:
            break;
          case 0x324B414B:
            script_113C7();
            break;
          case 0x314B414B:
            script_1147C();
            break;
          case 0x31524946:
            break;
          case 0x32524946:
            break;
          case 0x33524946:
            break;
          case 0x34524946:
            break;
          case 0x4F4B4148:
            if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 807) & -(Script_GetOwnedItemCount(36, 0) == 0) & -(Script_GetOwnedItemCount(35, 0) == 0) & -(task_22D == 0)) {
              script_1621A();
            }
            break;
          case 10:
            script_1147C();
            break;
          case 11:
            break;
        }
      }
      if (Script_srchUtils(SRCH_SCRIPT_GetCurrentTargetEntryIndex) == 0) {
        if (-(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_BASEMENT_SWORD_FOUND) == 0) & Script_GetOwnedItemCount(39, 0)) {
          Script_SetOwnedItemMenuFlagIfOwned(1, OwnedItem_UIITEMMYKE, 1);
          if (task_286 == 0) {
            task_286 = 1;
            Script_sicnUtils(0, 8, 1);
            Script_sicnUtils(2, 4, 0);
            task_287 = 0;
          }
        }
      } else {
        Script_SetOwnedItemMenuFlagIfOwned(1, OwnedItem_UIITEMMYKE, 0);
        if (task_286) {
          task_286 = 0;
          Script_sicnUtils(2, 8, 0);
        }
        if (Script_SetTaskDataValue(CHARA_AKIR, &data_28C3C, &data_28C48, 0) & -((&local_D0)->y > 9102) & -((&local_D0)->y < 23665) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_BASEMENT_SWORD_FOUND) == 0) & Script_GetOwnedItemCount(39, 0)) {
          Script_SetOwnedItemMenuFlagIfOwned(1, OwnedItem_UIITEMMYKE, 1);
          if (task_287 == 0) {
            task_287 = 1;
            Script_sicnUtils(0, 12, 1);
          }
        } else {
          Script_SetOwnedItemMenuFlagIfOwned(1, OwnedItem_UIITEMMYKE, 0);
          if (task_287) {
            task_287 = 0;
            Script_sicnUtils(1, 0, 0);
          }
        }
      }
      if (-(Script_srchUtils(SRCH_SCRIPT_GetCurrentTargetEntryIndex) == 0) & -(Script_GetOwnedItemCount(39, 0) == 0) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_BASEMENT_SWORD_FOUND) == 0) & -(task_281 == 0)) {
        script_16147();
      } else {
        task_27D = 0;
      }
      if (Script_srchUtils(SRCH_SCRIPT_GetCurrentTargetEntryIndex) == 7) {
        if (-(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_TSUBA_PLACED) == 0) & Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_KAK2_HANGING_SCROLL_TAKEN)) {
          Script_SetOwnedItemMenuFlagIfOwned(1, OwnedItem_UIITEMSWOR, 1);
          if (task_282 == 0) {
            task_282 = 1;
            Script_sicnUtils(0, 8, 1);
            Script_sicnUtils(2, 4, 0);
            task_283 = 0;
          }
        }
      } else {
        if (task_22D == 0) {
          Script_SetOwnedItemMenuFlagIfOwned(1, OwnedItem_UIITEMSWOR, 0);
        }
        if (task_282) {
          task_282 = 0;
          Script_sicnUtils(2, 8, 0);
        }
        if (Script_SetTaskDataValue(CHARA_AKIR, &data_28C24, &data_28C30, 0) & -(Script_srchUtils(SRCH_SCRIPT_GetCurrentTargetEntryIndex) == -1) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_TSUBA_PLACED) == 0) & Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_KAK2_HANGING_SCROLL_TAKEN)) {
          Script_SetOwnedItemMenuFlagIfOwned(1, OwnedItem_UIITEMSWOR, 1);
          if (task_283 == 0) {
            task_283 = 1;
            Script_sicnUtils(0, 12, 1);
          }
        } else {
          if ((task_22D == 0) | (Script_srchUtils(SRCH_SCRIPT_GetCurrentTargetEntryIndex) != -1)) {
            Script_SetOwnedItemMenuFlagIfOwned(1, OwnedItem_UIITEMSWOR, 0);
          }
          if (task_283) {
            task_283 = 0;
            Script_sicnUtils(1, 0, 0);
          }
        }
      }
      if (-(Script_srchUtils(SRCH_SCRIPT_GetCurrentTargetEntryIndex) == 7) & Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_KAK2_HANGING_SCROLL_TAKEN)) {
        script_15BA7();
      } else {
        task_27B = 0;
      }
      if (-(Script_srchUtils(SRCH_SCRIPT_GetCurrentTargetEntryIndex) == 8) & Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_KAK1_HANGING_SCROLL_TAKEN)) {
        script_15CDB();
      } else {
        task_27C = 0;
      }
      if (Script_srchUtils(SRCH_SCRIPT_GetCurrentTargetEntryIndex) == 1) {
        if ((Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_HIHY_EVENT_DONE) == 0) & (local_CC < 2)) {
          Script_srchUtils(SRCH_SCRIPT_Reset);
          script_16B72();
        } else {
          Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 807, 1);
          script_150B8();
        }
      } else {
        task_27F = 0;
      }
      if (Script_SetTaskDataValue(CHARA_AKIR, &data_28C24, &data_28C30, 0) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_KAK2_HANGING_SCROLL_TAKEN) == 0) & -((&local_D0)->y > 9102) & -((&local_D0)->y < 23665)) {
        if (task_284 == 0) {
          task_284 = 1;
          Script_sicnUtils(0, 4, 1);
        }
      } else {
        if (task_284) {
          task_284 = 0;
          Script_sicnUtils(2, 4, 0);
        }
      }
      if (Script_SetTaskDataValue(CHARA_AKIR, &data_28C0C, &data_28C18, 0) & -((&local_D0)->y > 9102) & -((&local_D0)->y < 23665) & ((-(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_KATANA_INSERTED) == 0) & task_22D) | -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_KAK1_HANGING_SCROLL_TAKEN) == 0))) {
        if (task_285 == 0) {
          task_285 = 1;
          Script_sicnUtils(0, 4, 1);
        }
      } else {
        if (task_285) {
          task_285 = 0;
          Script_sicnUtils(2, 4, 0);
        }
      }
      if (Script_srchUtils(SRCH_SCRIPT_GetCurrentTargetEntryIndex) == -1) {
        if (Script_SetTaskDataValue(CHARA_AKIR, &data_28CE4, &data_28CF0, SHEN_NAME('POK2')) & script_10105(CHARA_AKIR, SHEN_NAME('POK2')) & Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_BASEMENT_SWORD_FOUND) & -(task_22D == 0) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_KATANA_INSERTED) == 0)) {
          if (task_288 == 0) {
            task_288 = 1;
            Script_sicnUtils(0, 4, 1);
          }
        } else {
          if (task_288) {
            task_288 = 0;
            Script_sicnUtils(2, 4, 0);
          }
        }
      } else {
        if (task_288) {
          task_288 = 0;
          Script_sicnUtils(2, 4, 0);
        }
      }
      if ((Script_srchUtils(SRCH_SCRIPT_GetCurrentTargetEntryIndex) == 2) | (Script_srchUtils(SRCH_SCRIPT_GetCurrentTargetEntryIndex) == 3) | (Script_srchUtils(SRCH_SCRIPT_GetCurrentTargetEntryIndex) == 4) | (Script_srchUtils(SRCH_SCRIPT_GetCurrentTargetEntryIndex) == 5)) {
        script_15042();
      } else {
        task_27E = 0;
      }
      if (Script_srchUtils(SRCH_SCRIPT_GetCurrentTargetEntryIndex) == 6) {
        if (task_28C) {
          task_28C = 0;
          Script_sicnUtils(2, 4, 0);
        }
        if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 807) & -(Script_GetOwnedItemCount(36, 0) == 0) & -(Script_GetOwnedItemCount(35, 0) == 0) & -(task_22D == 0)) {
        } else {
          script_161A4();
        }
      } else {
        task_280 = 0;
        if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 807) & -(Script_GetOwnedItemCount(36, 0) == 0) & -(Script_GetOwnedItemCount(35, 0) == 0) & -(task_22D == 0) & Script_SetTaskDataValue(CHARA_AKIR, &data_28CE4, &data_28CF0, SHEN_NAME('HAKO'))) {
          if (task_28C == 0) {
            task_28C = 1;
            Script_sicnUtils(0, 4, 1);
          }
        } else {
          if (task_28C) {
            task_28C = 0;
            Script_sicnUtils(2, 4, 0);
          }
        }
      }
      if (Script_IsPlayerAllowed(0)) {
        local_E4 = Script_cassetteUtils();
        local_E0 = (local_E4 & -0x10000) >> 16;
        local_DC = local_E4 & 0xFFFF;
        if ((local_E0 == 4) & (local_DC == 2)) {
          if ((task_282 == 0) & (task_283 == 0)) {
            script_13D81();
          } else {
            Script_GetCassetteFlags_0(4, 1, 0);
            task_282 = 0;
            task_283 = 0;
            Script_sicnUtils(1, 0, 0);
            script_14221();
            Script_SetOwnedItemMenuFlagIfOwned(1, OwnedItem_UIITEMSWOR, 0);
          }
        }
        if ((local_E0 == 39) & (local_DC == 2)) {
          Script_GetCassetteFlags_0(39, 1, 0);
          task_286 = 0;
          task_287 = 0;
          Script_sicnUtils(1, 0, 0);
          script_12CF6();
        }
      }
      if (Script_SetTaskDataValue(CHARA_AKIR, &data_28CFC, &data_28D08, 0)) {
        if (task_22D) {
          script_14BD6();
        }
      }
      if (Script_SetTaskDataValue(CHARA_AKIR, &data_28C54, &data_28C60, 0)) {
        script_1453C();
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_113C7(void) {
  task_284 = 0;
  Script_sicnUtils(2, 4, 0);
  if (Script_GetOwnedItemCount(4, 0)) {
    task_27B = 1;
    script_13727(CHARA_AKIR);
    script_102CA();
  } else {
    script_13B7A(CHARA_AKIR);
  }
  return;
}

static void script_1140E(void) {
  Script_ryoControl(PCCTL_RequestNewMode, 5);
  Script_ryoControl(PCCTL_DisablePlayerControl);
  while (Script_ryoControl(PCCTL_QueryRequestIdle) == 0) {
    SCNF_WaitFrames_0000(1);
  }
  task_284 = 0;
  Script_sicnUtils(2, 4, 0);
  if (Script_GetOwnedItemCount(4, 0)) {
    task_27B = 1;
    script_13727(CHARA_AKIR);
    script_102CA();
  } else {
    script_13C74(CHARA_AKIR);
  }
  return;
}

static void script_1147C(void) {
  task_285 = 0;
  Script_sicnUtils(2, 4, 0);
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_KATANA_INSERTED) == 0) {
    if (task_22D) {
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_KAK1_HANGING_SCROLL_TAKEN) == 0) {
        script_13424(CHARA_AKIR);
        script_102CA();
      } else {
        script_11EF2();
      }
    } else {
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_KAK1_HANGING_SCROLL_TAKEN) == 0) {
        script_13973(CHARA_AKIR);
      }
    }
  }
  return;
}

static void script_114F3(void) {
  Script_ryoControl(PCCTL_RequestNewMode, 5);
  Script_ryoControl(PCCTL_DisablePlayerControl);
  while (Script_ryoControl(PCCTL_QueryRequestIdle) == 0) {
    SCNF_WaitFrames_0000(1);
  }
  task_285 = 0;
  Script_sicnUtils(2, 4, 0);
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_KATANA_INSERTED) == 0) {
    if (task_22D) {
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_KAK1_HANGING_SCROLL_TAKEN) == 0) {
        script_13424(CHARA_AKIR);
        script_102CA();
      } else {
        script_11EF2();
      }
    } else {
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_KAK1_HANGING_SCROLL_TAKEN) == 0) {
        script_13A6D(CHARA_AKIR);
      }
    }
  }
  return;
}

static int32_t script_11591(int32_t arg0) {
  Vector3 local_F4;
  Vector3i local_E8;
  Script_charaDisplay(arg0, CDM_Hide);
  for (;;) {
    Script_EVT_GetPosition(CHARA_AKIR, -1, &local_F4, 0);
    Script_mtwkGetSetRotation(CHARA_AKIR, &local_E8, 0);
    Script_EVT_SetPosition(arg0, POS_XYZ, &local_F4);
    Script_EVT_SetRotation(arg0, 0x38000000, &local_E8, 0);
    if (task_228 == 0) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_SetMapControlTaskFlag(arg0, 1);
  return Script_srchControl(0);
}

__attribute__((scn_string_bindings(0, data_28D2C)))
static void script_115FA(void) {
  Script_SetGameStateTo(AUTH_None);
  task_228 = 1;
  Script_shenClearFreeState(0);
  Script_ryoControl(PCCTL_RequestNewMode, 5);
  Script_ryoControl(PCCTL_DisablePlayerControl);
  while (Script_ryoControl(PCCTL_QueryRequestIdle) == 0) {
    SCNF_WaitFrames_0000(1);
  }
  if (Script_SetTaskDataValue(CHARA_AKIR, &data_27EA8, &data_27EB4, 0) == 0) {
    Vector3 local_F4;
    local_F4.x = -6.7f;
    (&local_F4)->y = 0.0f;
    (&local_F4)->z = -22.5f;
    SCNF_CHAR_SCNFApplyFacingTowardPoint(CHARA_AKIR, &local_F4, data_27E18, 0.0f, 0, 0);
    Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_WALK_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_LineWalkUtils(LNWK_SCRIPT_BEGIN_PATH_WALK, CHARA_AKIR, &data_27EC0, 0, 0);
    {
      int32_t unused_E8[3];
      int32_t local_E4;
      local_E4 = 0;
      while (local_E4 < 30) {
        Script_LineWalkUtils(LNWK_SCRIPT_SET_MOVE_SCALE_GET_CMD, CHARA_AKIR, 0.05f);
        local_E4 = local_E4 + 1;
        SCNF_WaitFrames_0000(1);
      }
    }
    Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  }
  SCNF_WaitFrames_0000(1);
  Script_charaDisplay(SHEN_NAME('POK2'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('POS2'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('PONK'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('PONS'), CDM_Show);
  {
    Vector3 local_F4;
    Vector3i local_E8;
    Script_EVT_GetPosition(CHARA_AKIR, -1, &local_F4, 0);
    Script_mtwkGetSetRotation(CHARA_AKIR, &local_E8, 0);
    Script_EVT_SetPosition(SHEN_NAME('PONK'), POS_XYZ, &local_F4);
    Script_EVT_SetRotation(SHEN_NAME('PONK'), 0x38000000, &local_E8, 0);
    Script_EVT_SetPosition(SHEN_NAME('PONS'), POS_XYZ, &local_F4);
    Script_EVT_SetRotation(SHEN_NAME('PONS'), 0x38000000, &local_E8, 0);
  }
  Script_FaceEx(CHARA_AKIR, 1, 0, 2);
  Script_scenCreateTask(script_15236, 0);
  Script_PlayCharaMotion(CHARA_AKIR, 7024, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_tmnmUtils(SHEN_NAME('PONK'), 7058, 0.0f, 32767.0f, 0.0f, 1.0f, 0);
  Script_tmnmUtils(SHEN_NAME('PONS'), 7057, 0.0f, 32767.0f, 0.0f, 1.0f, 0);
  Script_AddCharacterHandler(3, "BROOM", SHEN_NAME('DMY0'));
  Script_scenCreateTask(script_11591, 1, SHEN_NAME('DMY0'));
  {
    int32_t unused_E8[6];
    int32_t local_E4;
    local_E4 = 0;
    while (Script_IsCharaMotionFinished(CHARA_AKIR) == 0) {
      int32_t local_E0;
      local_E0 = local_E4;
      switch (local_E0) {
        case 0:
          Script_seqEnterEvtCam(2001, SHEN_NAME('DMY0'), SHEN_NAME('DMY0'));
          break;
        case 50:
          Script_executeAicaCommand(853417, 0, 0);
          break;
        case 120:
          Script_seqEnterEvtCam(2002, SHEN_NAME('DMY0'), SHEN_NAME('DMY0'));
          break;
        case 140:
          Script_executeAicaCommand(0x1F05A9, 0, 0);
          break;
        case 330:
          Script_executeAicaCommand(787881, 0, 0);
          break;
        case 349:
          break;
      }
      local_E4 = local_E4 + 1;
      SCNF_WaitFrames_0000(1);
    }
  }
  Script_charaDisplay(SHEN_NAME('POK2'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('POS2'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('PONK'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('PONS'), CDM_Hide);
  Script_FaceEx(CHARA_AKIR, 0, 0, 2);
  task_228 = 0;
  Script_convSetCamInactiveFlag(0);
  Script_SetGameStateTo(AUTH_None);
  Script_ryoControl(RYO_RestoreControl);
  Script_srchUtils(SRCH_SCRIPT_Reset);
  Script_shenClearFreeState(1);
  return;
}

__attribute__((scn_string_bindings(0, data_28DDC, 1, data_28DE6)))
static void script_1196D(void) {
  Vector3 local_F4;
  Vector3 local_E8;
  Vector3i local_DC;
  int32_t local_D8;
  int32_t unused_CC[3];
  int32_t local_C8;
  local_F4.x = -10.4869f;
  (&local_F4)->y = 2.3514f;
  (&local_F4)->z = -23.1343f;
  task_228 = 1;
  Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &data_28D34);
  Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &data_28D40, 0);
  Script_ccowUtils(CHARA_AKIR, 2);
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_scenCreateTask(script_14A6B, 0);
  Script_fixoAttachEx(SHEN_NAME('PONK'), CHARA_AKIR, 12, &data_28DC4, &data_28DD0);
  Script_fixoAttachEx(SHEN_NAME('PONS'), CHARA_AKIR, 12, &data_28DC4, &data_28DD0);
  Script_PlayCharaMotion(CHARA_AKIR, 7030, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_CharHandlers(CHARA_AKIR, 0, 13, 16);
  for (;;) {
    if (local_D8 == 721) {
      local_D8 = 0;
      break;
    } else {
      local_D8 = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_GetCharaMotionRemainingFramesScaled(CHARA_AKIR));
    }
    local_C8 = local_D8;
    switch (local_C8) {
      case 110:
        Script_ShowUIMessageFromID("E1011A055");
        break;
      case 225:
        Script_executeAicaCommand(787881, 0, 0);
        Script_fixoClearAttachment(SHEN_NAME('PONK'));
        Script_fixoClearAttachment(SHEN_NAME('PONS'));
        Script_fixoAttachEx(SHEN_NAME('PONK'), CHARA_AKIR, 18, &data_28D4C, &data_28D58);
        Script_fixoAttachEx(SHEN_NAME('PONS'), CHARA_AKIR, 18, &data_28D4C, &data_28D58);
        local_E8.x = (&data_28D4C)->x;
        (&local_E8)->y = (&data_28D4C)->y;
        (&local_E8)->z = (&data_28D4C)->z;
        break;
      case 286:
        Script_fixoClearAttachment(SHEN_NAME('PONS'));
        Script_fixoAttachEx(SHEN_NAME('PONS'), CHARA_AKIR, 12, &data_28D64, &data_28D70);
        break;
      case 367:
        Script_executeAicaCommand(0x1705A9, 0, 0);
        Script_fixoClearAttachment(SHEN_NAME('PONS'));
        break;
      case 438:
        local_DC.x = (&data_28D58)->x;
        (&local_DC)->y = (&data_28D58)->y;
        (&local_DC)->z = (&data_28D58)->z;
        local_E8.x = (&data_28D4C)->x;
        (&local_E8)->y = (&data_28D4C)->y;
        (&local_E8)->z = (&data_28D4C)->z;
        break;
      case 448:
        Script_executeAicaCommand(0x1805A9, 0, 0);
        break;
      case 460:
        break;
      case 599:
        Script_executeAicaCommand(0x1905A9, 0, 0);
        Script_fixoClearAttachment(SHEN_NAME('PONK'));
        Script_EVT_GetPosition(CHARA_AKIR, 18, &local_F4, 0);
        (&local_F4)->y = (&local_F4)->y - 0.13f;
        (&local_F4)->z = (&local_F4)->z + 0.03f;
        local_F4.x = (&local_F4)->x - 0.09f;
        Script_EVT_SetPosition(SHEN_NAME('PONK'), POS_XYZ, &local_F4);
        Script_EVT_SetRotation(SHEN_NAME('PONK'), 0x38000000, &data_28D94, 0);
        break;
      case 700:
        Script_ShowUIMessageFromID("E1011A045");
        break;
      case 720:
        Script_fixoClearAttachment(SHEN_NAME('PONK'));
        Script_fixoAttachEx(SHEN_NAME('PONK'), SHEN_NAME('SKAB'), 0, &data_28D7C, &data_28D88);
        break;
    }
    if ((local_D8 > 438) & (local_D8 < 448)) {
      Script_fixoClearAttachment(SHEN_NAME('PONK'));
      (&local_DC)->y = (&local_DC)->y + 3640;
      (&local_E8)->y = (&local_E8)->y - 0.006f;
      Script_fixoAttachEx(SHEN_NAME('PONK'), CHARA_AKIR, 18, &local_E8, &local_DC);
    }
    if ((local_D8 > 599) & (local_D8 < 720)) {
      Script_EVT_GetPosition(CHARA_AKIR, 18, &local_E8, 0);
      local_F4.x = (&local_E8)->x - 0.09f;
      Script_EVT_SetPosition(SHEN_NAME('PONK'), POS_XYZ, &local_F4);
    }
    if (local_D8 > 225) {
      if (local_D8 < 285) {
        local_E8.x = (&local_E8)->x + (0.087f - (&data_28D4C)->x) / (float)40;
        (&local_E8)->y = (&local_E8)->y + (0.514f - (&data_28D4C)->y) / (float)40;
        (&local_E8)->z = (&local_E8)->z + (-0.03f - (&data_28D4C)->z) / (float)40;
        Script_fixoClearAttachment(SHEN_NAME('PONS'));
        Script_fixoAttachEx(SHEN_NAME('PONS'), CHARA_AKIR, 18, &local_E8, &data_28D58);
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_KATANA_INSERTED, 1);
  task_22D = 0;
  Script_ryoSleepUtils(3, 1, 1, 0);
  if (Script_GetOwnedItemCount(4, 0) & -(task_282 == 0) & -(task_283 == 0)) {
    Script_SetOwnedItemMenuFlagIfOwned(1, OwnedItem_UIITEMSWOR, 0);
  }
  Script_CharHandlers(CHARA_AKIR, 0, 3, 16);
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_TSUBA_PLACED)) {
    script_1278B();
  } else {
    Script_motmSetBlendFrames(CHARA_AKIR, MOTM_BLEND_FRAMES_15);
    Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    SCNF_WaitFrames_0000(15);
    Script_motmSetBlendFrames(CHARA_AKIR, MOTM_BLEND_FRAMES_5);
    script_102CA();
  }
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_srchUtils(SRCH_SCRIPT_Reset);
  Script_convSetCamInactiveFlag(0);
  Script_SetGameStateTo(AUTH_None);
  SCNF_WaitFrames_0000(1);
  Script_ccowUtils(CHARA_AKIR, 1);
  Script_ryoControl(RYO_RestoreControl);
  task_228 = 0;
  return;
}

__attribute__((scn_string_bindings(0, data_28E98)))
static void script_11EF2(void) {
  int32_t unused_DC[9];
  int32_t local_D8;
  int32_t unused_CC[3];
  int32_t local_C8;
  Script_srchUtils(SRCH_SCRIPT_Reset);
  task_228 = 1;
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_shenClearFreeState(0);
  Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &data_28DF0);
  Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &data_28DFC, 0);
  Script_charaDisplay(SHEN_NAME('POK2'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('POS2'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('PONK'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('PONS'), CDM_Show);
  Script_EVT_SetPosition(SHEN_NAME('PONK'), POS_XYZ, &data_28DF0);
  Script_EVT_SetRotation(SHEN_NAME('PONK'), 0x38000000, &data_28DFC, 0);
  Script_EVT_SetPosition(SHEN_NAME('PONS'), POS_XYZ, &data_28DF0);
  Script_EVT_SetRotation(SHEN_NAME('PONS'), 0x38000000, &data_28DFC, 0);
  Script_ccowUtils(CHARA_AKIR, 2);
  Script_FaceEx(CHARA_AKIR, 1, 0, 2);
  Script_CharHandlers(CHARA_AKIR, -1, 0, 0);
  Script_motmToggleFlags(CHARA_AKIR, 2, 0x40);
  Script_PlayCharaMotion(CHARA_AKIR, 7039, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_tmnmUtils(SHEN_NAME('PONK'), 7063, 0.0f, 32767.0f, 0.0f, 1.0f, 0);
  Script_tmnmUtils(SHEN_NAME('PONS'), 7068, 0.0f, 32767.0f, 0.0f, 1.0f, 0);
  Script_seqEnterEvtCam(700, 0, AUTH_None);
  for (;;) {
    if (Script_IsCharaMotionFinished(CHARA_AKIR) != 0) {
      local_D8 = 0;
      break;
    } else {
      local_D8 = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_GetCharaMotionRemainingFramesScaled(CHARA_AKIR));
    }
    local_C8 = local_D8;
    switch (local_C8) {
      case 100:
        Script_motmToggleFlags(CHARA_AKIR, 1, 0x40);
        Script_executeAicaCommand(853417, 0, 0);
        break;
      case 190:
        Script_seqEnterEvtCam(701, 0, AUTH_None);
        break;
      case 245:
        Script_executeAicaCommand(0x1705A9, 0, 0);
        break;
      case 290:
        Script_seqEnterEvtCam(702, 0, AUTH_None);
        break;
      case 345:
        Script_executeAicaCommand(0x1805A9, 0, 0);
        break;
      case 350:
        Script_seqEnterEvtCam(703, 0, AUTH_None);
        break;
      case 400:
        Script_CharHandlers(CHARA_AKIR, 1, 14, 16);
        break;
      case 575:
        Script_executeAicaCommand(0x1905A9, 0, 0);
        break;
      case 590:
        Script_charaDisplay(SHEN_NAME('PONK'), CDM_Hide);
        Script_charaDisplay(SHEN_NAME('PONS'), CDM_Hide);
        Script_charaDisplay(SHEN_NAME('POK2'), CDM_Show);
        Script_fixoAttachEx(SHEN_NAME('POK2'), SHEN_NAME('SKAB'), 0, &data_28E38, &data_28E44);
        Script_ShowUIMessageFromID("E1011A045");
        break;
      case 640:
        if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_TSUBA_PLACED)) {
          Script_executeAicaCommand(0x1B05A9, 0, 0);
        }
        break;
      case 650:
        goto L0121F5;
        break;
    }
    SCNF_WaitFrames_0000(1);
  }
  L0121F5:;
  Script_CharHandlers(CHARA_AKIR, -1, 0, 0);
  Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_KATANA_INSERTED, 1);
  task_22D = 0;
  Script_ryoSleepUtils(3, 1, 1, 0);
  if (Script_GetOwnedItemCount(4, 0) & -(task_282 == 0) & -(task_283 == 0)) {
    Script_SetOwnedItemMenuFlagIfOwned(1, OwnedItem_UIITEMSWOR, 0);
  }
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_TSUBA_PLACED)) {
    script_1278B();
  } else {
    Script_motmSetBlendFrames(CHARA_AKIR, MOTM_BLEND_FRAMES_15);
    Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    SCNF_WaitFrames_0000(15);
    Script_motmSetBlendFrames(CHARA_AKIR, MOTM_BLEND_FRAMES_5);
    script_102CA();
    Script_FaceEx(CHARA_AKIR, 0, 0, 2);
  }
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_srchUtils(SRCH_SCRIPT_Reset);
  Script_convSetCamInactiveFlag(0);
  Script_SetGameStateTo(AUTH_None);
  SCNF_WaitFrames_0000(1);
  Script_ccowUtils(CHARA_AKIR, 1);
  Script_ryoControl(RYO_RestoreControl);
  task_228 = 0;
  Script_shenClearFreeState(1);
  return;
}

__attribute__((scn_string_bindings(0, data_28EA2)))
static void script_12311(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t local_E4;
  Script_seqEnterEvtCam(605, 0, AUTH_None);
  local_F4.x = -10.5762f;
  (&local_F4)->y = 1.1095f;
  (&local_F4)->z = -23.0063f;
  local_E8.x = 0;
  (&local_E8)->y = 0x4000;
  (&local_E8)->z = 0;
  Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &local_F4);
  Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &local_E8, 0);
  Script_PlayCharaMotion(CHARA_AKIR, 7030, 720.0f, -1.0f, -1.0f, 1.0f, 0);
  for (;;) {
    if (Script_IsCharaMotionFinished(CHARA_AKIR) != 0) {
      local_E4 = 0;
      break;
    } else {
      local_E4 = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_GetCharaMotionRemainingFramesScaled(CHARA_AKIR));
    }
    {
      Vector3i local_D8;
      int32_t local_D4;
      local_D4 = local_E4;
      switch (local_D4) {
        case 721:
          Script_scenCreateTask(script_157ED, 0);
          Script_executeAicaCommand(0x1A05A9, 0, 0);
          Script_charaDisplay(SHEN_NAME('TUBA'), CDM_Hide);
          break;
        case 731:
          Script_mtwkGetSetRotation(SHEN_NAME('SKAB'), &local_D8, 0);
          (&local_D8)->y = (&local_D8)->y + 27;
          Script_EVT_SetRotation(SHEN_NAME('SKAB'), 0x38000000, &local_D8, 0);
          break;
        case 830:
          Script_executeAicaCommand(0x1C05A9, 0, 0);
          break;
      }
    }
    if (local_E4 > 828) {
      if (local_E4 < 900) {
        Vector3i local_D8;
        Script_mtwkGetSetRotation(SHEN_NAME('SKAB'), &local_D8, 0);
        (&local_D8)->y = (&local_D8)->y + (int32_t)(((float)(local_E4 - 828) * 0.009f + 0.8f) / 360.0f * (float)0x10000);
        Script_EVT_SetRotation(SHEN_NAME('SKAB'), 0x38000000, &local_D8, 0);
        Script_mtwkGetSetRotation(CHARA_AKIR, &task_258, 0);
        task_258.y = task_258.y + 91;
        Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &task_258, 0);
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_ROTATING_DOOR_UNLOCKED, 1);
  script_102CA();
  Script_ProcessSubtitle("JINGURU01", 0);
  Script_scenToggleCharaFlag(0, 202, 0);
  return;
}

__attribute__((scn_string_bindings(0, data_28EAC)))
static void script_12522(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t local_E4;
  Script_seqEnterEvtCam(605, 0, AUTH_None);
  local_F4.x = -10.5762f;
  (&local_F4)->y = 1.1095f;
  (&local_F4)->z = -23.05f;
  local_E8.x = 0;
  (&local_E8)->y = 0x4000;
  (&local_E8)->z = 0;
  Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &local_F4);
  Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &local_E8, 0);
  Script_scenCreateTask(script_157ED, 0);
  Script_charaDisplay(SHEN_NAME('TUBA'), CDM_Hide);
  Script_PlayCharaMotion(CHARA_AKIR, 7030, 820.0f, -1.0f, -1.0f, 1.0f, 0);
  for (;;) {
    if (Script_IsCharaMotionFinished(CHARA_AKIR) != 0) {
      local_E4 = 0;
      break;
    } else {
      local_E4 = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_GetCharaMotionRemainingFramesScaled(CHARA_AKIR));
    }
    {
      Vector3i local_D8;
      int32_t local_D4;
      local_D4 = local_E4;
      switch (local_D4) {
        case 731:
          Script_mtwkGetSetRotation(SHEN_NAME('SKAB'), &local_D8, 0);
          (&local_D8)->y = (&local_D8)->y + 27;
          Script_EVT_SetRotation(SHEN_NAME('SKAB'), 0x38000000, &local_D8, 0);
          break;
        case 830:
          Script_executeAicaCommand(0x1C05A9, 0, 0);
          break;
      }
    }
    if (local_E4 > 828) {
      if (local_E4 < 900) {
        Vector3i local_D8;
        Script_mtwkGetSetRotation(SHEN_NAME('SKAB'), &local_D8, 0);
        (&local_D8)->y = (&local_D8)->y + (int32_t)(((float)(local_E4 - 828) * 0.009f + 0.8f) / 360.0f * (float)0x10000);
        Script_EVT_SetRotation(SHEN_NAME('SKAB'), 0x38000000, &local_D8, 0);
        Script_mtwkGetSetRotation(CHARA_AKIR, &task_258, 0);
        task_258.y = task_258.y + 91;
        Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &task_258, 0);
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_ROTATING_DOOR_UNLOCKED, 1);
  script_102CA();
  Script_ProcessSubtitle("JINGURU01", 0);
  Script_scenToggleCharaFlag(0, 202, 0);
  return;
}

static int32_t script_12719(void) {
  int16_t local_FE;
  int32_t local_F8;
  local_FE = 0;
  for (;;) {
    local_F8 = local_FE;
    switch (local_F8) {
      case 0:
        Script_seqEnterEvtCam(605, 0, AUTH_None);
        break;
      case 125:
        Script_seqEnterEvtCam(606, 0, AUTH_None);
        break;
      case 300:
        Script_seqEnterEvtCam(607, 0, AUTH_None);
        goto L012784;
        break;
    }
    local_FE = local_FE + 1;
    SCNF_WaitFrames_0000(1);
  }
  L012784:;
  return Script_srchControl(0);
}

__attribute__((scn_string_bindings(0, data_28EB6)))
static void script_1278B(void) {
  Vector3 local_F4;
  local_F4.x = -10.7769f;
  (&local_F4)->y = 1.102f;
  (&local_F4)->z = -22.7708f;
  {
    Vector3i local_E8;
    local_E8.x = 0;
    (&local_E8)->y = 0x4000;
    (&local_E8)->z = 0;
    Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &local_F4);
    Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &local_E8, 0);
  }
  Script_scenCreateTask(script_157ED, 0);
  Script_scenCreateTask(script_12719, 0);
  Script_PlayCharaMotion(CHARA_AKIR, 7006, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  for (;;) {
    int32_t unused_E8[3];
    int32_t local_E4;
    if (Script_IsCharaMotionFinished(CHARA_AKIR) != 0) {
      local_E4 = 0;
      break;
    } else {
      local_E4 = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_GetCharaMotionRemainingFramesScaled(CHARA_AKIR));
    }
    {
      Vector3i local_D8;
      int32_t local_D4;
      local_D4 = local_E4;
      switch (local_D4) {
        case 66:
          Script_CharHandlers(CHARA_AKIR, 0, 1, 16);
          break;
        case 83:
          Script_executeAicaCommand(0x1C05A9, 0, 0);
          break;
        case 86:
          Script_mtwkGetSetRotation(SHEN_NAME('SKAB'), &local_D8, 0);
          break;
        case 350:
          goto L012942;
          break;
      }
      if ((local_E4 > 88) & (local_E4 < 198)) {
        Script_mtwkGetSetRotation(SHEN_NAME('SKAB'), &local_D8, 0);
        (&local_D8)->y = (&local_D8)->y + 100;
        Script_EVT_SetRotation(SHEN_NAME('SKAB'), 0x38000000, &local_D8, 0);
      }
    }
    if ((local_E4 > 197) & (local_E4 < 231)) {
      Vector3i local_D8;
      Script_mtwkGetSetRotation(SHEN_NAME('SKAB'), &local_D8, 0);
      (&local_D8)->y = (&local_D8)->y + 200;
      Script_EVT_SetRotation(SHEN_NAME('SKAB'), 0x38000000, &local_D8, 0);
    }
    SCNF_WaitFrames_0000(1);
  }
  L012942:;
  Script_CharHandlers(CHARA_AKIR, -1, 0, 0);
  Script_ProcessSubtitle("JINGURU01", 0);
  SCNF_WaitFrames_0000(90);
  Script_motmToggleFlags(CHARA_AKIR, 2, 0x40);
  {
    Vector3i local_E8;
    local_E8.x = 0;
    (&local_E8)->y = 0;
    (&local_E8)->z = 0;
    Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &local_E8, 0);
  }
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  SCNF_WaitFrames_0000(1);
  Script_motmToggleFlags(CHARA_AKIR, 1, 0x40);
  Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_ROTATING_DOOR_UNLOCKED, 1);
  script_102CA();
  Script_scenToggleCharaFlag(0, 202, 0);
  Script_FaceEx(CHARA_AKIR, 0, 0, 2);
  return;
}

static void script_129FF(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT_INTERNAL_HUMAN, 0x554B5546);
  SCNF_WaitFrames_0000(1);
  Script_CycleManagerUtils(CYCLE_CMD_SET_CYCLE_MANAGER_ENABLED, 0);
  Script_charaDisplay(CHARA_FUKU, CDM_Show);
  local_F4.x = -1.4197f;
  (&local_F4)->y = 0.0f;
  (&local_F4)->z = -22.45f;
  local_E8.x = 0;
  (&local_E8)->y = 0xC000;
  (&local_E8)->z = 0;
  Script_EVT_SetPosition(CHARA_FUKU, POS_XYZ, &local_F4);
  Script_EVT_SetRotation(CHARA_FUKU, 0x38000000, &local_E8, 0);
  Script_PlayCharaMotion(CHARA_FUKU, MN_AKI_AKI_WALK_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_LineWalkUtils(LNWK_SCRIPT_BEGIN_PATH_WALK, CHARA_FUKU, &data_28EC0, 0, 0);
  while (Script_LineWalkUtils(LNWK_SCRIPT_SET_MOVE_SCALE_GET_CMD, CHARA_FUKU, 0.05f) != -1) {
    SCNF_WaitFrames_0000(1);
  }
  Script_SetMapControlTaskFlag(CHARA_FUKU, 1);
  Script_srchControl(0);
  return;
}

static int32_t script_12AF4(void) {
  int32_t unused_E8[6];
  if (task_240) {
    if ((Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_TATQ_FOLLOWUP_COMPLETE) == 0) | Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_SA1077_INE_HANDOFF_DONE)) {
      Script_CycleManagerUtils(CYCLE_CMD_LOAD_OR_RESET_RUNNING_PROGRAM, task_238);
      task_240 = 0;
    }
  } else {
    Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT, 0x554B5546, 1);
    Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
  }
  task_22A = 1;
  return Script_srchControl(0);
}

static void script_12B52(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  int8_t local_E7;
  local_E7 = 0;
  Script_charaPosRotUtils(SHEN_NAME('KHKS'), 152, &local_F4, &local_E8, 2);
  while (local_E7 < 60) {
    local_E8.x = (&local_E8)->x - 364;
    Script_charaPosRotUtils(SHEN_NAME('KHKS'), 152, &local_F4, &local_E8, 0);
    local_E7 = local_E7 + 1;
    SCNF_WaitFrames_0000(1);
  }
  Script_srchControl(0);
  return;
}

static void script_12BAF(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  int8_t local_E7;
  local_E7 = 0;
  Script_charaPosRotUtils(SHEN_NAME('KHKS'), 152, &local_F4, &local_E8, 2);
  while (local_E7 < 80) {
    local_E8.x = (&local_E8)->x + 273;
    Script_charaPosRotUtils(SHEN_NAME('KHKS'), 152, &local_F4, &local_E8, 0);
    local_E7 = local_E7 + 1;
    SCNF_WaitFrames_0000(1);
  }
  Script_srchControl(0);
  return;
}

static int32_t script_12C0C(void) {
  int16_t local_FE;
  int32_t local_F8;
  local_FE = 0;
  for (;;) {
    local_F8 = local_FE;
    switch (local_F8) {
      case 0:
        Script_seqEnterEvtCam(611, 0, AUTH_None);
        break;
      case 90:
        Script_seqEnterEvtCam(612, 0, AUTH_None);
        break;
      case 150:
        Script_seqEnterEvtCam(613, 0, AUTH_None);
        break;
      case 290:
        Script_seqEnterEvtCam(614, 0, AUTH_None);
        break;
      case 410:
        Script_seqEnterEvtCam(615, 0, AUTH_None);
        break;
      case 530:
        Script_seqEnterEvtCam(616, 0, AUTH_None);
        break;
      case 630:
        Script_seqEnterEvtCam(617, 0, AUTH_None);
        break;
      case 750:
        Script_seqEnterEvtCam(610, 0, AUTH_None);
        goto L012CEF;
        break;
    }
    local_FE = local_FE + 1;
    SCNF_WaitFrames_0000(1);
  }
  L012CEF:;
  return Script_srchControl(0);
}

static void script_12CF6(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t local_E4;
  int32_t local_E0;
  task_228 = 1;
  Script_scenCreateTask(script_12AF4, 0);
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_shenClearFreeState(0);
  Script_ccowUtils(CHARA_AKIR, 2);
  Script_motmToggleFlags(CHARA_AKIR, 2, 0x40);
  Script_charaDisplay(SHEN_NAME('PONK'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('PONS'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('POK2'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('POS2'), CDM_Hide);
  local_F4.x = -10.11089f;
  (&local_F4)->y = 0.95036f;
  (&local_F4)->z = -25.216909f;
  local_E8.x = 0;
  (&local_E8)->y = 0x4000;
  (&local_E8)->z = 0;
  Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &local_F4);
  Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &local_E8, 0);
  Script_CharHandlers(CHARA_AKIR, -1, 0, 0);
  Script_FaceEx(CHARA_AKIR, 1, 0, 2);
  Script_scenCreateTask(script_12C0C, 0);
  Script_PlayCharaMotion(CHARA_AKIR, 7004, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_scenCreateTask(script_1591E, 0);
  local_E4 = 0;
  while (Script_IsCharaMotionFinished(CHARA_AKIR) == 0) {
    local_E4 = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_GetCharaMotionRemainingFramesScaled(CHARA_AKIR));
    local_E0 = local_E4;
    switch (local_E0) {
      case 124:
        local_F4.x = -10.46f;
        (&local_F4)->y = 1.0556f;
        (&local_F4)->z = -25.2f;
        local_E8.x = 0;
        (&local_E8)->y = 0x4000;
        (&local_E8)->z = 0;
        Script_EVT_SetPosition(SHEN_NAME('KAGI'), POS_XYZ, &local_F4);
        Script_EVT_SetRotation(SHEN_NAME('KAGI'), 0x38000000, &local_E8, 0);
        Script_tmnmUtils(SHEN_NAME('KAGI'), 7065, 0.0f, 32767.0f, 0.0f, 1.0f, 0);
        break;
      case 120:
        Script_executeAicaCommand(0x2305A9, 0, 0);
        break;
      case 205:
        Script_executeAicaCommand(591273, 0, 0);
        break;
      case 260:
        Script_CharHandlers(CHARA_AKIR, 0, 14, 16);
        Script_CharHandlers(CHARA_AKIR, 1, 14, 16);
        break;
      case 285:
        Script_executeAicaCommand(460201, 0, 0);
        Script_tmnmUtils(SHEN_NAME('KHKS'), 7067, 0.0f, 32767.0f, 0.0f, 1.0f, 0);
        break;
      case 380:
        Script_CharHandlers(CHARA_AKIR, -1, 0, 0);
        break;
      case 436:
        local_F4.x = -10.46f;
        (&local_F4)->y = 1.0556f;
        (&local_F4)->z = -25.2f;
        local_E8.x = 0;
        (&local_E8)->y = 0x4000;
        (&local_E8)->z = 0;
        Script_EVT_SetPosition(SHEN_NAME('PONK'), POS_XYZ, &local_F4);
        Script_EVT_SetRotation(SHEN_NAME('PONK'), 0x38000000, &local_E8, 0);
        Script_EVT_SetPosition(SHEN_NAME('PONS'), POS_XYZ, &local_F4);
        Script_EVT_SetRotation(SHEN_NAME('PONS'), 0x38000000, &local_E8, 0);
        Script_tmnmUtils(SHEN_NAME('PONK'), 7070, 0.0f, 32767.0f, 0.0f, 1.0f, 0);
        Script_tmnmUtils(SHEN_NAME('PONS'), 7069, 0.0f, 32767.0f, 0.0f, 1.0f, 0);
        Script_charaDisplay(SHEN_NAME('PONK'), CDM_Hide);
        Script_charaDisplay(SHEN_NAME('PONS'), CDM_Hide);
        local_F4.x = 0.06721f;
        (&local_F4)->y = 0.22969f;
        (&local_F4)->z = -0.03007f;
        local_E8.x = -34588;
        (&local_E8)->y = -32952;
        (&local_E8)->z = -31832;
        Script_fixoAttachEx(SHEN_NAME('POK2'), CHARA_AKIR, 12, &local_F4, &local_E8);
        Script_fixoAttachEx(SHEN_NAME('POS2'), CHARA_AKIR, 12, &local_F4, &local_E8);
        Script_charaDisplay(SHEN_NAME('POK2'), CDM_Show);
        Script_charaDisplay(SHEN_NAME('POS2'), CDM_Show);
        break;
      case 565:
        Script_CharHandlers(CHARA_AKIR, 1, 14, 16);
        break;
      case 570:
        Script_executeAicaCommand(0x1705A9, 0, 0);
        Script_fixoClearAttachment(SHEN_NAME('POK2'));
        Script_fixoClearAttachment(SHEN_NAME('POS2'));
        break;
      case 600:
        break;
      case 630:
        Script_CharHandlers(CHARA_AKIR, 0, 13, 16);
        Script_CharHandlers(CHARA_AKIR, 1, 13, 16);
        break;
      case 655:
        Script_tmnmUtils(SHEN_NAME('KHKS'), 7066, 0.0f, 32767.0f, 0.0f, 1.0f, 0);
        break;
      case 656:
        Script_tmnmUtils(SHEN_NAME('KAGI'), 7064, 0.0f, 32767.0f, 0.0f, 1.0f, 0);
        break;
      case 745:
        Script_executeAicaCommand(525737, 0, 0);
        break;
      case 750:
        Script_CharHandlers(CHARA_AKIR, -1, 0, 0);
        break;
      case 775:
        Script_charaDisplay(SHEN_NAME('PONK'), CDM_Hide);
        Script_charaDisplay(SHEN_NAME('PONS'), CDM_Hide);
        Script_charaDisplay(SHEN_NAME('POK2'), CDM_Show);
        Script_charaDisplay(SHEN_NAME('POS2'), CDM_Show);
        local_F4.x = 0.0922f;
        (&local_F4)->y = 0.1018f;
        (&local_F4)->z = 0.00806f;
        local_E8.x = 0;
        (&local_E8)->y = 0;
        (&local_E8)->z = 0;
        Script_fixoAttachEx(SHEN_NAME('POK2'), CHARA_AKIR, 12, &local_F4, &local_E8);
        Script_fixoAttachEx(SHEN_NAME('POS2'), CHARA_AKIR, 12, &local_F4, &local_E8);
        break;
      case 800:
        Script_executeAicaCommand(0x3805A9, 0, 0);
        break;
      case 840:
        goto L0132B3;
        break;
    }
    SCNF_WaitFrames_0000(1);
  }
  L0132B3:;
  local_F4.x = -9.6f;
  (&local_F4)->y = 0.959f;
  (&local_F4)->z = -25.2668f;
  local_E8.x = 0;
  (&local_E8)->y = 0x4000;
  (&local_E8)->z = 0;
  Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &local_F4);
  Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &local_E8, 0);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_FaceEx(CHARA_AKIR, 0, 0, 2);
  Script_srchUtils(SRCH_SCRIPT_Reset);
  Script_convSetCamInactiveFlag(1);
  Script_SetGameStateTo(AUTH_None);
  SCNF_WaitFrames_0000(1);
  Script_motmToggleFlags(CHARA_AKIR, 1, 0x40);
  Script_ccowUtils(CHARA_AKIR, 1);
  Script_ryoControl(RYO_RestoreControl);
  Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_BASEMENT_SWORD_FOUND, 1);
  SAVE_Script_UpdateForSaveData(3, 0, 14400);
  task_22D = 1;
  task_228 = 0;
  if (Script_GetOwnedItemCount(4, 0)) {
    Script_SetOwnedItemMenuFlagIfOwned(1, OwnedItem_UIITEMSWOR, 1);
  }
  Script_ryoSleepUtils(3, 1, 1, 1);
  Script_shenClearFreeState(1);
  return;
}

static int32_t script_133C9(void) {
  int16_t local_FE;
  int32_t local_F8;
  local_FE = 0;
  for (;;) {
    local_F8 = local_FE;
    switch (local_F8) {
      case 0:
        Script_seqEnterEvtCam(631, 0, AUTH_None);
        break;
      case 270:
        Script_seqEnterEvtCam(632, 0, AUTH_None);
        goto L01341D;
        break;
    }
    local_FE = local_FE + 1;
    SCNF_WaitFrames_0000(1);
  }
  L01341D:;
  return Script_srchControl(0);
}

__attribute__((scn_string_bindings(0, data_28F84)))
static void script_13424(int32_t arg0) {
  int32_t local_FC;
  Vector3 local_F0;
  int32_t unused_EC;
  int32_t local_E4[2];
  int32_t local_E0;
  local_E4[1] = -0x42333333;
  local_E4[0] = 0x3D75C28F;
  local_FC = 0;
  Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_KAK1_HANGING_SCROLL_TAKEN, 1);
  task_228 = 1;
  Script_ccowUtils(arg0, 2);
  Script_motmToggleFlags(CHARA_AKIR, 2, 0x40);
  Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &data_28F24);
  Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &data_28F30, 0);
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_shenClearFreeState(0);
  Script_PlayCharaMotion(CHARA_AKIR, 7044, 90.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_scenCreateTask(script_133C9, 0);
  Script_CharHandlers(CHARA_AKIR, 0, 13, 16);
  Script_CharHandlers(CHARA_AKIR, 1, 3, 16);
  for (;;) {
    local_FC = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_GetCharaMotionRemainingFramesScaled(CHARA_AKIR));
    if (local_FC > 470) {
      break;
    }
    local_E0 = local_FC;
    switch (local_E0) {
      case 145:
        Script_CharHandlers(CHARA_AKIR, 0, 14, 16);
        break;
      case 149:
        Script_tmnmUtils(SHEN_NAME('KAK1'), 7075, 0.0f, 32767.0f, 0.0f, 1.0f, 0);
        Script_executeAicaCommand(0x1005A9, 0, 0);
        break;
      case 360:
        Script_executeAicaCommand(0x1105A9, 0, 0);
        Script_charaDisplay(SHEN_NAME('KAK1'), CDM_Hide);
        Script_EVT_SetPosition(SHEN_NAME('KAK3'), POS_XYZ, &data_28F3C);
        Script_EVT_SetRotation(SHEN_NAME('KAK3'), 0x38000000, &data_28F48, 0);
        Script_charaDisplay(SHEN_NAME('KAK3'), CDM_Show);
        break;
      case 400:
        Script_ShowUIMessageFromID("E1011A078");
        break;
      case 470:
        Script_ShowUIMessageFromID("E1011A079");
        break;
    }
    SCNF_WaitFrames_0000(1);
  }
  SCNF_WaitFrames_0000(90);
  local_F0.x = -10.62f;
  (&local_F0)->y = 1.1f;
  (&local_F0)->z = -23.29f;
  Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &local_F0);
  Script_CharHandlers(CHARA_AKIR, -1, 0, 0);
  Script_srchUtils(SRCH_SCRIPT_Reset);
  Script_convSetCamInactiveFlag(0);
  Script_SetGameStateTo(AUTH_None);
  SCNF_WaitFrames_0000(1);
  Script_motmToggleFlags(CHARA_AKIR, 1, 0x40);
  Script_ryoControl(RYO_RestoreControl);
  Script_ccowUtils(arg0, 1);
  task_228 = 0;
  Script_shenClearFreeState(1);
  return;
}

static void script_13684(void) {
  Script_seqEnterEvtCam(101, 0, AUTH_None);
  SCNF_WaitFrames_0000(225);
  Script_seqEnterEvtCam(102, 0, AUTH_None);
  SCNF_WaitFrames_0000(77);
  Script_seqEnterEvtCam(103, 0, AUTH_None);
  SCNF_WaitFrames_0000(218);
  return;
}

static int32_t script_136BF(void) {
  int16_t local_FE;
  int32_t local_F8;
  local_FE = 0;
  for (;;) {
    local_F8 = local_FE;
    switch (local_F8) {
      case 0:
        Script_seqEnterEvtCam(621, 0, AUTH_None);
        break;
      case 270:
        Script_seqEnterEvtCam(622, 0, AUTH_None);
        break;
      case 335:
        goto L013720;
        break;
    }
    local_FE = local_FE + 1;
    SCNF_WaitFrames_0000(1);
  }
  L013720:;
  return Script_srchControl(0);
}

static void script_13727(int32_t arg0) {
  int32_t local_FC;
  Vector3 local_F0;
  int32_t unused_E0[4];
  int32_t local_D8[2];
  int32_t local_D4;
  local_D8[1] = -0x42333333;
  local_D8[0] = 0x3D75C28F;
  local_FC = 0;
  Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_KAK2_HANGING_SCROLL_TAKEN, 1);
  task_228 = 1;
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_shenClearFreeState(0);
  Script_ccowUtils(arg0, 2);
  Script_motmToggleFlags(CHARA_AKIR, 2, 0x40);
  Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &data_28F98);
  Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &data_28FA4, 0);
  Script_PlayCharaMotion(CHARA_AKIR, 7044, 90.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_scenCreateTask(script_136BF, 0);
  Script_CharHandlers(CHARA_AKIR, 0, 13, 16);
  Script_CharHandlers(CHARA_AKIR, 1, 3, 16);
  Script_scenCreateTask(script_15B47, 0);
  for (;;) {
    local_FC = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_GetCharaMotionRemainingFramesScaled(CHARA_AKIR));
    if (local_FC > 490) {
      break;
    }
    local_D4 = local_FC;
    switch (local_D4) {
      case 145:
        Script_CharHandlers(CHARA_AKIR, 0, 14, 16);
        break;
      case 149:
        Script_tmnmUtils(SHEN_NAME('KAK2'), 7075, 0.0f, 32767.0f, 0.0f, 1.0f, 0);
        Script_executeAicaCommand(0x1005A9, 0, 0);
        break;
      case 360:
        Script_executeAicaCommand(0x1105A9, 0, 0);
        Script_charaDisplay(SHEN_NAME('KAK2'), CDM_Hide);
        Script_EVT_SetPosition(SHEN_NAME('KAK4'), POS_XYZ, &data_28FB0);
        Script_EVT_SetRotation(SHEN_NAME('KAK4'), 0x38000000, &data_28FBC, 0);
        Script_charaDisplay(SHEN_NAME('KAK4'), CDM_Show);
        break;
      case 400:
        break;
    }
    SCNF_WaitFrames_0000(1);
  }
  SCNF_WaitFrames_0000(50);
  local_F0.x = -10.62f;
  (&local_F0)->y = 1.1f;
  (&local_F0)->z = -21.67f;
  Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &local_F0);
  Script_CharHandlers(CHARA_AKIR, -1, 0, 0);
  Script_srchUtils(SRCH_SCRIPT_Reset);
  Script_convSetCamInactiveFlag(0);
  Script_SetGameStateTo(AUTH_None);
  SCNF_WaitFrames_0000(1);
  Script_motmToggleFlags(CHARA_AKIR, 1, 0x40);
  Script_ryoControl(RYO_RestoreControl);
  Script_ccowUtils(arg0, 1);
  task_228 = 0;
  Script_shenClearFreeState(1);
  return;
}

static void script_13973(int32_t arg0) {
  task_228 = 1;
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_shenClearFreeState(0);
  Script_InvestigationUtils(ShenmueUtils_BeginInteractionMode, SHEN_NAME('KAK1'), 40);
  {
    int16_t local_FE;
    local_FE = 0;
    for (;;) {
      if (local_FE == 40) {
        Script_executeAicaCommand(0x1005A9, 0, 0);
      }
      if (Script_InvestigationUtils(ShenmueUtils_PollInteractionModeDone) != 0) {
        break;
      }
      local_FE = local_FE + 1;
      SCNF_WaitFrames_0000(1);
    }
  }
  script_15A74();
  Script_InvestigationUtils(ShenmueUtils_RequestInteractionModeExit);
  {
    int16_t local_FE;
    local_FE = 0;
    for (;;) {
      if (local_FE == 40) {
        Script_executeAicaCommand(0x1105A9, 0, 0);
      }
      if (Script_InvestigationUtils(ShenmueUtils_PollInteractionModeDone) != 0) {
        break;
      }
      local_FE = local_FE + 1;
      SCNF_WaitFrames_0000(1);
    }
  }
  Script_srchUtils(SRCH_SCRIPT_Reset);
  Script_convSetCamInactiveFlag(0);
  Script_SetGameStateTo(AUTH_None);
  SCNF_WaitFrames_0000(1);
  Script_motmToggleFlags(CHARA_AKIR, 1, 0x40);
  Script_ryoControl(RYO_RestoreControl);
  Script_ccowUtils(arg0, 1);
  task_228 = 0;
  Script_shenClearFreeState(1);
  return;
}

static void script_13A6D(int32_t arg0) {
  task_228 = 1;
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_shenClearFreeState(0);
  Script_seqEnterEvtCam(2051, 0, AUTH_None);
  SCNF_WaitFrames_0000(16);
  Script_InvestigationUtils(ShenmueUtils_BeginInteractionMode, SHEN_NAME('KAK1'), 40);
  {
    int16_t local_FE;
    local_FE = 0;
    for (;;) {
      if (local_FE == 40) {
        Script_executeAicaCommand(0x1005A9, 0, 0);
      }
      if (Script_InvestigationUtils(ShenmueUtils_PollInteractionModeDone) != 0) {
        break;
      }
      local_FE = local_FE + 1;
      SCNF_WaitFrames_0000(1);
    }
  }
  script_15A74();
  Script_InvestigationUtils(ShenmueUtils_RequestInteractionModeExit);
  {
    int16_t local_FE;
    local_FE = 0;
    for (;;) {
      if (local_FE == 40) {
        Script_executeAicaCommand(0x1105A9, 0, 0);
      }
      if (Script_InvestigationUtils(ShenmueUtils_PollInteractionModeDone) != 0) {
        break;
      }
      local_FE = local_FE + 1;
      SCNF_WaitFrames_0000(1);
    }
  }
  Script_srchUtils(SRCH_SCRIPT_Reset);
  Script_convSetCamInactiveFlag(0);
  Script_SetGameStateTo(AUTH_None);
  SCNF_WaitFrames_0000(1);
  Script_motmToggleFlags(CHARA_AKIR, 1, 0x40);
  Script_ryoControl(RYO_RestoreControl);
  Script_ccowUtils(arg0, 1);
  task_228 = 0;
  Script_shenClearFreeState(1);
  return;
}

static void script_13B7A(int32_t arg0) {
  task_228 = 1;
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_shenClearFreeState(0);
  Script_InvestigationUtils(ShenmueUtils_BeginInteractionMode, SHEN_NAME('KAK2'), 40);
  {
    int16_t local_FE;
    local_FE = 0;
    for (;;) {
      if (local_FE == 40) {
        Script_executeAicaCommand(0x1005A9, 0, 0);
      }
      if (Script_InvestigationUtils(ShenmueUtils_PollInteractionModeDone) != 0) {
        break;
      }
      local_FE = local_FE + 1;
      SCNF_WaitFrames_0000(1);
    }
  }
  script_159A1();
  Script_InvestigationUtils(ShenmueUtils_RequestInteractionModeExit);
  {
    int16_t local_FE;
    local_FE = 0;
    for (;;) {
      if (local_FE == 40) {
        Script_executeAicaCommand(0x1105A9, 0, 0);
      }
      if (Script_InvestigationUtils(ShenmueUtils_PollInteractionModeDone) != 0) {
        break;
      }
      local_FE = local_FE + 1;
      SCNF_WaitFrames_0000(1);
    }
  }
  Script_srchUtils(SRCH_SCRIPT_Reset);
  Script_convSetCamInactiveFlag(0);
  Script_SetGameStateTo(AUTH_None);
  SCNF_WaitFrames_0000(1);
  Script_motmToggleFlags(CHARA_AKIR, 1, 0x40);
  Script_ryoControl(RYO_RestoreControl);
  Script_ccowUtils(arg0, 1);
  task_228 = 0;
  Script_shenClearFreeState(1);
  return;
}

static void script_13C74(int32_t arg0) {
  task_228 = 1;
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_shenClearFreeState(0);
  Script_seqEnterEvtCam(2050, 0, AUTH_None);
  SCNF_WaitFrames_0000(16);
  Script_InvestigationUtils(ShenmueUtils_BeginInteractionMode, SHEN_NAME('KAK2'), 40);
  {
    int16_t local_FE;
    local_FE = 0;
    for (;;) {
      if (local_FE == 40) {
        Script_executeAicaCommand(0x1005A9, 0, 0);
      }
      if (Script_InvestigationUtils(ShenmueUtils_PollInteractionModeDone) != 0) {
        break;
      }
      local_FE = local_FE + 1;
      SCNF_WaitFrames_0000(1);
    }
  }
  script_159A1();
  Script_InvestigationUtils(ShenmueUtils_RequestInteractionModeExit);
  {
    int16_t local_FE;
    local_FE = 0;
    for (;;) {
      if (local_FE == 40) {
        Script_executeAicaCommand(0x1105A9, 0, 0);
      }
      if (Script_InvestigationUtils(ShenmueUtils_PollInteractionModeDone) != 0) {
        break;
      }
      local_FE = local_FE + 1;
      SCNF_WaitFrames_0000(1);
    }
  }
  Script_srchUtils(SRCH_SCRIPT_Reset);
  Script_convSetCamInactiveFlag(0);
  Script_SetGameStateTo(AUTH_None);
  SCNF_WaitFrames_0000(1);
  Script_motmToggleFlags(CHARA_AKIR, 1, 0x40);
  Script_ryoControl(RYO_RestoreControl);
  Script_ccowUtils(arg0, 1);
  task_228 = 0;
  Script_shenClearFreeState(1);
  return;
}

__attribute__((scn_string_bindings(0, data_28FF8)))
static void script_13D81(void) {
  task_228 = 1;
  Script_shenClearFreeState(0);
  Script_ryoControl(PCCTL_RequestNewMode, 5);
  Script_ryoControl(PCCTL_DisablePlayerControl);
  while (Script_ryoControl(PCCTL_QueryRequestIdle) == 0) {
    SCNF_WaitFrames_0000(1);
  }
  if (Script_SetTaskDataValue(CHARA_AKIR, &data_27EA8, &data_27EB4, 0) == 0) {
    Vector3 local_F4;
    local_F4.x = -6.7f;
    (&local_F4)->y = 0.0f;
    (&local_F4)->z = -22.5f;
    SCNF_CHAR_SCNFApplyFacingTowardPoint(CHARA_AKIR, &local_F4, data_27E18, 0.0f, 0, 0);
    Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_WALK_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_LineWalkUtils(LNWK_SCRIPT_BEGIN_PATH_WALK, CHARA_AKIR, &data_27EC0, 0, 0);
    {
      int32_t unused_E8[3];
      int32_t local_E4;
      local_E4 = 0;
      while (local_E4 < 30) {
        Script_LineWalkUtils(LNWK_SCRIPT_SET_MOVE_SCALE_GET_CMD, CHARA_AKIR, 0.05f);
        local_E4 = local_E4 + 1;
        SCNF_WaitFrames_0000(1);
      }
    }
    Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  }
  SCNF_WaitFrames_0000(1);
  Script_charaDisplay(SHEN_NAME('POK2'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('POS2'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('PONK'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('PONS'), CDM_Show);
  {
    Vector3 local_F4;
    Vector3i local_E8;
    Script_EVT_GetPosition(CHARA_AKIR, -1, &local_F4, 0);
    Script_mtwkGetSetRotation(CHARA_AKIR, &local_E8, 0);
    Script_EVT_SetPosition(SHEN_NAME('PONK'), POS_XYZ, &local_F4);
    Script_EVT_SetRotation(SHEN_NAME('PONK'), 0x38000000, &local_E8, 0);
    Script_EVT_SetPosition(SHEN_NAME('PONS'), POS_XYZ, &local_F4);
    Script_EVT_SetRotation(SHEN_NAME('PONS'), 0x38000000, &local_E8, 0);
    Script_EVT_SetPosition(SHEN_NAME('TUBA'), POS_XYZ, &local_F4);
    Script_EVT_SetRotation(SHEN_NAME('TUBA'), 0x38000000, &local_E8, 0);
  }
  Script_PlayCharaMotion(CHARA_AKIR, 7042, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_AddCharacterHandler(3, "BROOM", SHEN_NAME('DMY0'));
  Script_scenCreateTask(script_11591, 1, SHEN_NAME('DMY0'));
  {
    int32_t unused_E8[6];
    int32_t local_E4;
    local_E4 = 0;
    while (Script_IsCharaMotionFinished(CHARA_AKIR) == 0) {
      int32_t local_E0;
      local_E4 = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_GetCharaMotionRemainingFramesScaled(CHARA_AKIR));
      local_E0 = local_E4;
      switch (local_E0) {
        case 1:
          Script_seqEnterEvtCam(720, SHEN_NAME('DMY0'), SHEN_NAME('DMY0'));
          Script_tmnmUtils(SHEN_NAME('PONK'), 7073, 0.0f, 32767.0f, 0.0f, 1.0f, 0);
          Script_tmnmUtils(SHEN_NAME('PONS'), 7071, 0.0f, 32767.0f, 0.0f, 1.0f, 0);
          break;
        case 50:
          Script_executeAicaCommand(853417, 0, 0);
          break;
        case 120:
          Script_seqEnterEvtCam(721, SHEN_NAME('DMY0'), SHEN_NAME('DMY0'));
          break;
        case 230:
          Script_executeAicaCommand(0x1705A9, 0, 0);
          break;
        case 240:
          Script_seqEnterEvtCam(722, SHEN_NAME('DMY0'), SHEN_NAME('DMY0'));
          break;
        case 333:
          Script_charaDisplay(SHEN_NAME('TUBA'), CDM_Show);
          Script_tmnmUtils(SHEN_NAME('TUBA'), 7074, 0.0f, 32767.0f, 0.0f, 1.0f, 0);
          break;
        case 335:
          if (task_27A == 0) {
            Script_ShowUIMessageFromID("E1011A103");
          }
          break;
        case 490:
          if (task_27A == 0) {
            Script_ShowUIMessageFromID("E1011A104");
            task_27A = 1;
          } else {
            Script_ShowUIMessageFromID("E1011A105");
          }
          break;
        case 540:
          Script_seqEnterEvtCam(723, SHEN_NAME('DMY0'), SHEN_NAME('DMY0'));
          break;
        case 634:
          Script_charaDisplay(SHEN_NAME('TUBA'), CDM_Hide);
          break;
        case 723:
          Script_tmnmUtils(SHEN_NAME('PONS'), 7072, 0.0f, 32767.0f, 0.0f, 1.0f, 0);
          break;
        case 820:
          Script_executeAicaCommand(787881, 0, 0);
          break;
      }
      SCNF_WaitFrames_0000(1);
    }
  }
  Script_charaDisplay(SHEN_NAME('POK2'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('POS2'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('PONK'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('PONS'), CDM_Hide);
  Script_ryoControl(RYO_RestoreControl);
  task_228 = 0;
  Script_SetGameStateTo(AUTH_None);
  Script_shenClearFreeState(1);
  return;
}

static int32_t script_141DE(void) {
  int16_t local_FE;
  int32_t local_F8;
  local_FE = 0;
  for (;;) {
    local_F8 = local_FE;
    if (local_F8 == 0) {
      Script_seqEnterEvtCam(640, 0, AUTH_None);
      break;
      goto L014206;
    }
    L014206:;
    local_FE = local_FE + 1;
    SCNF_WaitFrames_0000(1);
  }
  return Script_srchControl(0);
}

__attribute__((scn_string_bindings(0, data_29070, 1, data_2907A)))
static void script_14221(void) {
  int32_t local_FC;
  Vector3 local_F0;
  int32_t local_EC;
  task_228 = 1;
  Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_TSUBA_PLACED, 1);
  task_258.y = 0x4000;
  task_24C.x = -10.745f;
  task_24C.y = 1.12075f;
  task_24C.z = -21.875f;
  local_FC = 0;
  Script_ccowUtils(CHARA_AKIR, 2);
  Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &task_24C);
  Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &task_258, 0);
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_shenClearFreeState(0);
  Script_scenCreateTask(script_141DE, 0);
  Script_PlayCharaMotion(CHARA_AKIR, 7010, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_charaDisplay(SHEN_NAME('TUB2'), CDM_Show);
  Script_CharHandlers(CHARA_AKIR, 0, 13, 16);
  Script_CharHandlers(CHARA_AKIR, 1, 3, 16);
  Script_FaceEx(CHARA_AKIR, 1, 0, 2);
  for (;;) {
    if (local_FC > 330) {
      local_FC = 0;
      break;
    } else {
      local_FC = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_GetCharaMotionRemainingFramesScaled(CHARA_AKIR));
    }
    local_EC = local_FC;
    switch (local_EC) {
      case 91:
        Script_fixoAttachEx(SHEN_NAME('TUB2'), CHARA_AKIR, 18, &data_2901C, &data_29028);
        Script_facemotUtils(CHARA_AKIR, 1, 0);
        Script_ResetHandMotion(CHARA_AKIR, 1, 1, 43);
        break;
      case 111:
        Script_ShowUIMessageFromID("E1011A082");
        break;
      case 185:
        break;
      case 280:
        Script_executeAicaCommand(0x1205A9, 0, 0);
        Script_fixoClearAttachment(SHEN_NAME('TUBA'));
        Script_fixoAttachEx(SHEN_NAME('TUB2'), SHEN_NAME('SKAB'), 0, &data_29058, &data_29064);
        break;
      case 291:
        Script_ShowUIMessageFromID("E1011A045");
        break;
      case 320:
        if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_KATANA_INSERTED)) {
          Script_executeAicaCommand(0x1B05A9, 0, 0);
        }
        break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_KATANA_INSERTED)) {
    script_1278B();
  } else {
    local_F0.x = -10.44f;
    (&local_F0)->y = 1.1f;
    (&local_F0)->z = -21.6f;
    Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &local_F0);
    Script_FaceEx(CHARA_AKIR, 0, 0, 2);
  }
  SCNF_WaitFrames_0000(1);
  Script_srchUtils(SRCH_SCRIPT_Reset);
  Script_convSetCamInactiveFlag(0);
  Script_SetGameStateTo(AUTH_None);
  Script_ryoControl(RYO_RestoreControl);
  Script_ccowUtils(CHARA_AKIR, 1);
  task_228 = 0;
  Script_shenClearFreeState(1);
  return;
}

static int32_t script_144C9(void) {
  int32_t unused_FC;
  Script_seqEnterEvtCam(501, 0, AUTH_None);
  SCNF_WaitFrames_0000(70);
  Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
  SCNF_WaitFrames_0000(125);
  Script_executeAicaCommand(2720, 1, 30);
  Script_executeAicaCommand(2720, 2, 30);
  Script_executeAicaCommand(2720, 3, 30);
  Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
  return Script_srchControl(0);
}

static int32_t script_1453C(void) {
  int32_t local_FC;
  task_228 = 1;
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_shenClearFreeState(0);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_NOZOMI_KIDNAPPED) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOZOMI_RESCUE_RUSH_ACTIVE) == 0)) {
    int32_t local_F8;
    local_F8 = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f);
    switch (local_F8) {
      case 0:
        Script_ShowUIMessageFromID("E1011A116");
        break;
      case 1:
        Script_ShowUIMessageFromID("E1011A117");
        break;
      case 2:
        Script_ShowUIMessageFromID("E1011A118");
        break;
    }
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_WALK_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_LineWalkUtils(LNWK_SCRIPT_BEGIN_PATH_WALK, CHARA_AKIR, &data_29084, 0, 0);
    for (;;) {
      if (Script_LineWalkUtils(LNWK_SCRIPT_SET_MOVE_SCALE_GET_CMD, CHARA_AKIR, 0.05f) == -1) {
        break;
      }
      SCNF_WaitFrames_0000(1);
    }
    Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_SetGameStateTo(AUTH_None);
    Script_convSetCamInactiveFlag(1);
    Script_ryoControl(RYO_RestoreControl);
    task_228 = 0;
    Script_shenClearFreeState(1);
  } else {
    Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
    SCNF_WaitFrames_0000(30);
    Script_charaToggleMotEnabled(CHARA_AKIR, 0);
    Script_ccowUtils(CHARA_AKIR, 2);
    Script_scenCreateTask(script_144C9, 0);
    Script_motmToggleFlags(CHARA_AKIR, 2, 0x40);
    task_258.x = 0;
    task_258.y = 0x8000;
    task_258.z = 0;
    task_24C.x = -12.6812f;
    task_24C.y = 1.06f;
    task_24C.z = -26.243f;
    Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &task_24C);
    Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &task_258, 0);
    Script_PlayCharaMotion(CHARA_AKIR, 7020, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    for (;;) {
      if (Script_IsCharaMotionFinished(CHARA_AKIR)) {
        break;
      }
      Script_RotateNodeUtils(CHARA_AKIR, &data_2501C, 0);
      SCNF_WaitFrames_0000(1);
    }
    task_258.y = 65353;
    task_24C.x = -12.7149f;
    task_24C.y = 0.0f;
    task_24C.z = -25.1996f;
    {
      Vector3i local_F0;
      (&local_F0)->y = task_258.y;
      Script_EVT_SetRotation(CHARA_AKIR, MATRIX_XFORM_Y, &local_F0, 0);
    }
    Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &task_24C);
    Script_motmToggleFlags(CHARA_AKIR, 1, 0x40);
    Script_motmSetBlendFrames(CHARA_AKIR, MOTM_BLEND_FRAMES_8);
    Script_charaToggleMotEnabled(CHARA_AKIR, 1);
    Script_PlayCharaMotion(CHARA_AKIR, 7019, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    for (;;) {
      if (Script_IsCharaMotionFinished(CHARA_AKIR)) {
        break;
      }
      Script_RotateNodeUtils(CHARA_AKIR, &data_2501C, 0);
      SCNF_WaitFrames_0000(1);
    }
    task_258.y = 65353;
    task_24C.x = -12.7713f;
    task_24C.y = -1.309f;
    task_24C.z = -25.15f;
    Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &task_24C);
    {
      Vector3i local_F0;
      (&local_F0)->y = task_258.y;
      Script_EVT_SetRotation(CHARA_AKIR, MATRIX_XFORM_Y, &local_F0, 0);
    }
    Script_PlayCharaMotion(CHARA_AKIR, 7019, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    for (;;) {
      if (Script_IsCharaMotionFinished(CHARA_AKIR)) {
        break;
      }
      Script_RotateNodeUtils(CHARA_AKIR, &data_2501C, 0);
      SCNF_WaitFrames_0000(1);
    }
    task_228 = 0;
    local_FC = Script_StepUtils(GET_STEP_HIGHER, 0);
    if (local_FC < 20) {
      Script_shenRequestGameModuleTransition(2, SHEN_NAME('YDB1'), 0);
      SCNF_WaitFrames_0000(1);
    } else {
      Script_shenRequestGameModuleTransition(3, SHEN_NAME('YDB1'), 0);
      SCNF_WaitFrames_0000(1);
    }
  }
  end:;
}

static void script_14964(void) {
  task_228 = 1;
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_ccowUtils(CHARA_AKIR, 2);
  Script_seqEnterEvtCam(501, 0, AUTH_None);
  Script_PlayCharaMotion(CHARA_AKIR, MN_A_WALK_L_02, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_charaToggleMotEnabled(CHARA_AKIR, 0);
  Script_LineWalkUtils(LNWK_SCRIPT_BEGIN_PATH_WALK, CHARA_AKIR, &data_290B8, 0, 0);
  for (;;) {
    if (Script_LineWalkUtils(LNWK_SCRIPT_SET_MOVE_SCALE_GET_CMD, CHARA_AKIR, 0.05f) == -1) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  script_1453C();
  task_26C = 1;
  return;
}

static void script_14A03(void) {
  Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 255, 255, 255);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_SetCurrentScrollEnabled(0);
  Script_SetFogFlags(2, 0);
  return;
}

static void script_14A42(void) {
  Script_seqEnterEvtCam(104, 0, AUTH_None);
  SCNF_WaitFrames_0000(184);
  Script_seqEnterEvtCam(105, 0, AUTH_None);
  SCNF_WaitFrames_0000(140);
  return;
}

static int32_t script_14A6B(void) {
  Script_seqEnterEvtCam(700, CHARA_AKIR, CHARA_AKIR);
  SCNF_WaitFrames_0000(190);
  Script_seqEnterEvtCam(701, CHARA_AKIR, CHARA_AKIR);
  SCNF_WaitFrames_0000(90);
  Script_seqEnterEvtCam(702, CHARA_AKIR, CHARA_AKIR);
  SCNF_WaitFrames_0000(135);
  Script_seqEnterEvtCam(703, CHARA_AKIR, CHARA_AKIR);
  SCNF_WaitFrames_0000(45);
  Script_seqEnterEvtCam(605, 0, AUTH_None);
  end:;
}

static void script_14ADF(void) {
  int32_t unused_E8[6];
  Vector3i local_DC;
  int32_t unused_C4[6];
  Script_mtwkGetSetRotation(SHEN_NAME('SKAB'), &local_DC, 0);
  (&local_DC)->y = (&local_DC)->y + 0x4000;
  Script_EVT_SetRotation(SHEN_NAME('SKAB'), 0x38000000, &local_DC, 0);
  Script_EVT_SetPosition(SHEN_NAME('KAK3'), POS_XYZ, &data_29140);
  Script_EVT_SetRotation(SHEN_NAME('KAK3'), 0x38000000, &data_2914C, 0);
  Script_charaDisplay(SHEN_NAME('KAK3'), CDM_Show);
  Script_EVT_SetPosition(SHEN_NAME('KAK4'), POS_XYZ, &data_29158);
  Script_EVT_SetRotation(SHEN_NAME('KAK4'), 0x38000000, &data_29164, 0);
  Script_charaDisplay(SHEN_NAME('KAK4'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('KAK1'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('KAK2'), CDM_Hide);
  Script_scenToggleCharaFlag(0, 202, 0);
  for (;;) {
    if (Script_GetControllerInput(1) & 0x400) {
      if (Script_SetTaskDataValue(CHARA_AKIR, &data_29104, &data_29110, 0)) {
        Script_scenCreateTask(script_1453C, 0);
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_14BD6(void) {
  int32_t unused_F4[3];
  Vector3i local_E8;
  int32_t local_E4;
  int32_t local_E0;
  Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_DOJO_SWORD_DISPLAYED, 1);
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_shenClearFreeState(0);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_ShowUIMessageFromID(data_29194);
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_WALK_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_LineWalkUtils(LNWK_SCRIPT_BEGIN_PATH_WALK, CHARA_AKIR, &data_29170, 0, 0);
  while (Script_LineWalkUtils(LNWK_SCRIPT_SET_MOVE_SCALE_GET_CMD, CHARA_AKIR, 0.05f) != -1) {
    SCNF_WaitFrames_0000(1);
  }
  local_E8.x = 0;
  (&local_E8)->y = 0xC000;
  (&local_E8)->z = 0;
  Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &local_E8, 0);
  Script_PlayCharaMotion(CHARA_AKIR, 7035, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  for (;;) {
    if (local_E4 == 90) {
      local_E4 = 0;
      break;
    } else {
      local_E4 = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_GetCharaMotionRemainingFramesScaled(CHARA_AKIR));
    }
    local_E0 = local_E4;
    if (local_E0 == 70) {
      Script_fixoClearAttachment(SHEN_NAME('POK2'));
      Script_fixoClearAttachment(SHEN_NAME('POS2'));
      script_14FA5();
      goto L014D30;
    }
    L014D30:;
    SCNF_WaitFrames_0000(1);
  }
  task_22D = 0;
  if (Script_GetOwnedItemCount(4, 0) & -(task_282 == 0) & -(task_283 == 0)) {
    Script_SetOwnedItemMenuFlagIfOwned(1, OwnedItem_UIITEMSWOR, 0);
  }
  Script_ryoSleepUtils(3, 1, 1, 0);
  script_102CA();
  Script_ryoControl(RYO_RestoreControl);
  Script_scenUtils(SCENARIO_CMD_UPDATE_TIMER);
  Script_shenClearFreeState(1);
  return;
}

static void script_14D8D(void) {
  int32_t local_FC;
  Vector3 local_F0;
  Vector3i local_E4;
  int32_t local_E0;
  task_228 = 1;
  Script_shenClearFreeState(0);
  task_288 = 0;
  Script_sicnUtils(2, 4, 0);
  Script_scenUtils(SCENARIO_CMD_INIT_SCNT);
  task_22D = 1;
  Script_scenCreateTask(script_12AF4, 0);
  Script_motmToggleFlags(CHARA_AKIR, 2, 0x40);
  local_F0.x = -3.1f;
  (&local_F0)->y = 1.0595f;
  (&local_F0)->z = -24.5f;
  local_E4.x = 0;
  (&local_E4)->y = 0xC000;
  (&local_E4)->z = 0;
  Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &local_F0);
  Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &local_E4, 0);
  Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_DOJO_SWORD_DISPLAYED, 0);
  Script_ccowUtils(CHARA_AKIR, 2);
  Script_SetRyoMotionDetails(CHARA_AKIR);
  Script_SetGameStateTo(AUTH_None);
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_PlayCharaMotion(CHARA_AKIR, 7048, 60.0f, -1.0f, -1.0f, 1.0f, 0);
  for (;;) {
    if (local_FC == 165) {
      local_FC = 0;
      break;
    } else {
      local_FC = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_GetCharaMotionRemainingFramesScaled(CHARA_AKIR));
    }
    local_E0 = local_FC;
    if (local_E0 == 120) {
      Script_fixoAttachEx(SHEN_NAME('POK2'), CHARA_AKIR, 12, &data_291B8, &data_291C4);
      Script_fixoAttachEx(SHEN_NAME('POS2'), CHARA_AKIR, 12, &data_291B8, &data_291C4);
      goto L014EF3;
    }
    L014EF3:;
    SCNF_WaitFrames_0000(1);
  }
  Script_motmToggleFlags(CHARA_AKIR, 1, 0x40);
  Script_srchUtils(SRCH_SCRIPT_Reset);
  Script_convSetCamInactiveFlag(0);
  Script_SetGameStateTo(AUTH_None);
  script_102CA();
  Script_ryoControl(RYO_RestoreControl);
  Script_ccowUtils(CHARA_AKIR, 1);
  if (Script_GetOwnedItemCount(4, 0)) {
    Script_SetOwnedItemMenuFlagIfOwned(1, OwnedItem_UIITEMSWOR, 1);
  }
  Script_ryoSleepUtils(3, 1, 1, 1);
  task_228 = 0;
  Script_scenUtils(SCENARIO_CMD_UPDATE_TIMER);
  Script_shenClearFreeState(1);
  return;
}

static void script_14F6C(void) {
  Script_fixoAttachEx(SHEN_NAME('POK2'), CHARA_AKIR, 12, &data_291D0, &data_291DC);
  Script_fixoAttachEx(SHEN_NAME('POS2'), CHARA_AKIR, 12, &data_291D0, &data_291DC);
  return;
}

static void script_14FA5(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  local_F4.x = -2.3825f;
  (&local_F4)->y = 1.7465f;
  (&local_F4)->z = -24.4261f;
  local_E8.x = 2389;
  (&local_E8)->y = 64763;
  (&local_E8)->z = 62968;
  Script_EVT_SetPosition(SHEN_NAME('POK2'), POS_XYZ, &local_F4);
  Script_EVT_SetRotation(SHEN_NAME('POK2'), 0x38000000, &local_E8, 0);
  Script_EVT_SetPosition(SHEN_NAME('POS2'), POS_XYZ, &local_F4);
  Script_EVT_SetRotation(SHEN_NAME('POS2'), 0x38000000, &local_E8, 0);
  Script_charaDisplay(SHEN_NAME('POK2'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('POS2'), CDM_Show);
  return;
}

__attribute__((scn_string_bindings(0, data_291E8)))
static void script_15042(void) {
  if (task_27E == 0) {
    Script_ryoControl(PCCTL_DisablePlayerControl);
    Script_shenClearFreeState(0);
    task_27E = 1;
    SCNF_WaitFrames_0000(30);
    task_228 = 1;
    Script_ShowUIMessageFromID("E1011A031");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_srchUtils(SRCH_SCRIPT_Reset);
    Script_convSetCamInactiveFlag(0);
    Script_SetGameStateTo(AUTH_None);
    Script_ryoControl(RYO_RestoreControl);
    task_228 = 0;
    Script_shenClearFreeState(1);
  }
  return;
}

__attribute__((scn_string_bindings(1, data_291FC)))
static void script_150B8(void) {
  if (task_27F == 0) {
    Script_ryoControl(PCCTL_DisablePlayerControl);
    Script_shenClearFreeState(0);
    task_27F = 1;
    SCNF_WaitFrames_0000(30);
    task_228 = 1;
    if ((Script_GetOwnedItemCount(36, 0) == 0) & (Script_GetOwnedItemCount(35, 0) == 0)) {
      Script_ShowUIMessageFromID("E1011A101");
      while (Script_convGetIsActive(0)) {
        SCNF_WaitFrames_0000(1);
      }
    } else {
      Script_ShowUIMessageFromID("E1011A031");
    }
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_srchUtils(SRCH_SCRIPT_Reset);
    Script_convSetCamInactiveFlag(0);
    Script_SetGameStateTo(AUTH_None);
    Script_ryoControl(RYO_RestoreControl);
    task_228 = 0;
    Script_shenClearFreeState(1);
  }
  return;
}

static void script_1516C(void) {
  int32_t local_FC;
  local_FC = task_274;
  switch (local_FC) {
    case 0:
      Script_ShowUIMessageFromID("E1011A026");
      while (Script_convGetIsActive(0)) {
        SCNF_WaitFrames_0000(1);
      }
      Script_ShowUIMessageFromID("E1011A027");
      while (Script_convGetIsActive(0)) {
        SCNF_WaitFrames_0000(1);
      }
      task_274 = 1;
      break;
    case 1:
      local_FC = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f);
      switch (local_FC) {
        case 0:
          Script_ShowUIMessageFromID("E1011A028");
          break;
        case 1:
          Script_ShowUIMessageFromID("E1011A029");
          break;
        case 2:
          Script_ShowUIMessageFromID("E1011A030");
          break;
      }
      while (Script_convGetIsActive(0)) {
        SCNF_WaitFrames_0000(1);
      }
      break;
  }
  return;
}

static int32_t script_15236(void) {
  SCNF_WaitFrames_0000(135);
  {
    int32_t local_FC;
    local_FC = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f);
    switch (local_FC) {
      case 0:
        Script_ShowUIMessageFromID("E1011A032");
        break;
      case 1:
        Script_ShowUIMessageFromID("E1011A033");
        break;
    }
  }
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  SCNF_WaitFrames_0000(9);
  {
    int32_t local_FC;
    local_FC = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f);
    switch (local_FC) {
      case 0:
        Script_ShowUIMessageFromID("E1011A074");
        break;
      case 1:
        Script_ShowUIMessageFromID("E1011A077");
        break;
      case 2:
        Script_ShowUIMessageFromID("E1011A076");
        break;
    }
  }
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  return Script_srchControl(0);
}

__attribute__((scn_string_bindings(0, data_2926A, 1, data_29274, 2, data_2927E, 3, data_29288, 4, data_29292)))
static void script_15305(void) {
  int32_t local_FC;
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_FIRST_SEARCH_HINT_SEEN)) {
    local_FC = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f);
    switch (local_FC) {
      case 0:
        SCNF_WaitFrames_0000(20);
        Script_ShowUIMessageFromID("E1011A017");
        break;
      case 1:
        SCNF_WaitFrames_0000(30);
        Script_ShowUIMessageFromID("E1011A016");
        break;
      case 2:
        SCNF_WaitFrames_0000(15);
        Script_ShowUIMessageFromID("E1011A018");
        break;
    }
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
  } else {
    Script_ShowUIMessageFromID("E1011A007");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_ShowUIMessageFromID("E1011A016");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_FIRST_SEARCH_HINT_SEEN, 1);
  }
  Script_srchControl(0);
  return;
}

static void script_153E1(void) {
  Script_ryoControl(PCCTL_DisablePlayerControl);
  task_2A8 = 1;
  Script_scenCreateTask(script_15428, 0);
  while (task_2A8) {
    SCNF_WaitFrames_0000(1);
  }
  Script_srchUtils(SRCH_SCRIPT_Reset);
  Script_convSetCamInactiveFlag(0);
  Script_SetGameStateTo(AUTH_None);
  Script_ryoControl(RYO_RestoreControl);
  return;
}

__attribute__((scn_string_bindings(0, data_2929C, 1, data_292A6, 4, data_292C4, 7, data_292E2, 9, data_292F6, 10, data_29300, 12, data_29314, 13, data_2931E)))
static int32_t script_15428(void) {
  int32_t local_FC;
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_FIRST_SEARCH_HINT_SEEN)) {
    if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_KAK1_HANGING_SCROLL_TAKEN)) {
      task_275 = 1;
    }
    local_FC = task_275;
    switch (local_FC) {
      case 0:
        SCNF_WaitFrames_0000(430);
        Script_ShowUIMessageFromID("E1011A034");
        while (Script_convGetIsActive(0)) {
          SCNF_WaitFrames_0000(1);
        }
        Script_ShowUIMessageFromID("E1011A035");
        while (Script_convGetIsActive(0)) {
          SCNF_WaitFrames_0000(1);
        }
        task_275 = 1;
        break;
      case 1:
        local_FC = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f);
        switch (local_FC) {
          case 0:
            Script_ShowUIMessageFromID("E1011A036");
            break;
          case 1:
            Script_ShowUIMessageFromID("E1011A037");
            break;
          case 2:
            Script_ShowUIMessageFromID("E1011A034");
            break;
        }
        while (Script_convGetIsActive(0)) {
          SCNF_WaitFrames_0000(1);
        }
        local_FC = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f);
        switch (local_FC) {
          case 0:
            Script_ShowUIMessageFromID("E1011A038");
            break;
          case 1:
            Script_ShowUIMessageFromID("E1011A039");
            break;
        }
        while (Script_convGetIsActive(0)) {
          SCNF_WaitFrames_0000(1);
        }
        local_FC = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f);
        switch (local_FC) {
          case 0:
            Script_ShowUIMessageFromID("E1011A040");
            break;
          case 1:
            Script_ShowUIMessageFromID("E1011A041");
            break;
          case 2:
            Script_ShowUIMessageFromID("E1011A042");
            break;
        }
        while (Script_convGetIsActive(0)) {
          SCNF_WaitFrames_0000(1);
        }
        break;
    }
  } else {
    SCNF_WaitFrames_0000(225);
    Script_ShowUIMessageFromID("E1011A007");
    SCNF_WaitFrames_0000(175);
    Script_ShowUIMessageFromID("E1011A008");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_ShowUIMessageFromID("E1011A034");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_ShowUIMessageFromID("E1011A035");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_FIRST_SEARCH_HINT_SEEN, 1);
    task_275 = 1;
  }
  task_2A8 = 0;
  return Script_srchControl(0);
}

__attribute__((scn_string_bindings(0, data_29328, 1, data_29332, 2, data_2933C)))
static void script_15666(void) {
  SCNF_WaitFrames_0000(150);
  Script_ShowUIMessageFromID("E1011A082");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_ShowUIMessageFromID("E1011A045");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_ShowUIMessageFromID("E1011A007");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_srchControl(0);
  return;
}

__attribute__((scn_string_bindings(0, data_29346)))
static void script_156D2(void) {
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_ShowUIMessageFromID("E1011A046");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_srchUtils(SRCH_SCRIPT_Reset);
  Script_convSetCamInactiveFlag(0);
  Script_SetGameStateTo(AUTH_None);
  Script_ryoControl(RYO_RestoreControl);
  return;
}

static void script_15713(void) {
  int32_t local_FC;
  SCNF_WaitFrames_0000(600);
  local_FC = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f);
  switch (local_FC) {
    case 0:
      Script_ShowUIMessageFromID("E1011A051");
      break;
    case 1:
      Script_ShowUIMessageFromID("E1011A052");
      break;
    case 2:
      Script_ShowUIMessageFromID("E1011A053");
      break;
  }
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_srchControl(0);
  return;
}

__attribute__((scn_string_bindings(1, data_29378)))
static void script_1578A(void) {
  SCNF_WaitFrames_0000(630);
  Script_executeAicaCommand(0x2105A9, 0, 0);
  Script_ShowUIMessageFromID("E1011A054");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  SCNF_WaitFrames_0000(30);
  Script_ShowUIMessageFromID("E1011A055");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_srchControl(0);
  return;
}

static int32_t script_157ED(void) {
  SCNF_WaitFrames_0000(90);
  Script_ShowUIMessageFromID("E1011A056");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  SCNF_WaitFrames_0000(90);
  Script_ShowUIMessageFromID("E1011A057");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  return Script_srchControl(0);
}

__attribute__((scn_string_bindings(1, data_293A0, 2, data_293AA, 3, data_293B4)))
static void script_15842(void) {
  int32_t local_FC;
  if (task_276 == 0) {
    SCNF_WaitFrames_0000(102);
    Script_ShowUIMessageFromID("E1011A047");
    SCNF_WaitFrames_0000(180);
    Script_ShowUIMessageFromID("E1011A045");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_ShowUIMessageFromID("E1011A007");
    Script_seqEnterEvtCam(2006, 0, AUTH_None);
    SCNF_WaitFrames_0000(30);
    Script_ShowUIMessageFromID("E1011A046");
    task_276 = 1;
  } else {
    SCNF_WaitFrames_0000(300);
    Script_ShowUIMessageFromID("E1011A048");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    local_FC = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f);
    switch (local_FC) {
      case 0:
        Script_ShowUIMessageFromID("E1011A049");
        break;
      case 1:
        Script_ShowUIMessageFromID("E1011A050");
        break;
    }
  }
  Script_srchControl(0);
  return;
}

static int32_t script_1591E(void) {
  int32_t local_FC;
  int32_t local_F8;
  local_FC = 0;
  for (;;) {
    local_F8 = local_FC;
    switch (local_F8) {
      case 90:
        Script_ShowUIMessageFromID("E1011A070");
        break;
      case 210:
        Script_ShowUIMessageFromID("E1011A071");
        break;
      case 390:
        Script_ShowUIMessageFromID("E1011A072");
        break;
      case 450:
        Script_ShowUIMessageFromID("E1011A087");
        goto L01599A;
        break;
    }
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  L01599A:;
  return Script_srchControl(0);
}

__attribute__((scn_string_bindings(0, data_29404, 1, data_2940E, 2, data_29418, 3, data_29422, 4, data_2942C)))
static void script_159A1(void) {
  int32_t local_FC;
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_KAK2_FIRST_SEARCH_REACTION_SEEN)) {
    local_FC = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 4.0f);
    switch (local_FC) {
      case 0:
        Script_ShowUIMessageFromID("E1011A017");
        break;
      case 1:
        Script_ShowUIMessageFromID("E1011A016");
        break;
      case 2:
        Script_ShowUIMessageFromID("E1011A018");
        break;
      case 3:
        Script_ShowUIMessageFromID("E1011A065");
        break;
    }
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
  } else {
    Script_ShowUIMessageFromID("E1011A007");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_ShowUIMessageFromID("E1011A063");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_KAK2_FIRST_SEARCH_REACTION_SEEN, 1);
  }
  return;
}

__attribute__((scn_string_bindings(0, data_29440, 1, data_2944A, 2, data_29454, 3, data_2945E, 4, data_29468, 5, data_29472)))
static void script_15A74(void) {
  int32_t local_FC;
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_KAK1_FIRST_SEARCH_REACTION_SEEN)) {
    local_FC = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 4.0f);
    switch (local_FC) {
      case 0:
        Script_ShowUIMessageFromID("E1011A017");
        break;
      case 1:
        Script_ShowUIMessageFromID("E1011A016");
        break;
      case 2:
        Script_ShowUIMessageFromID("E1011A018");
        break;
      case 3:
        Script_ShowUIMessageFromID("E1011A065");
        break;
    }
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
  } else {
    Script_ShowUIMessageFromID("E1011A007");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_ShowUIMessageFromID("E1011A016");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_KAK1_FIRST_SEARCH_REACTION_SEEN, 1);
  }
  return;
}

__attribute__((scn_string_bindings(0, data_2947C, 1, data_29486)))
static int32_t script_15B47(void) {
  int32_t local_FC;
  int32_t local_F8;
  local_FC = 0;
  for (;;) {
    local_F8 = local_FC;
    if (local_F8 == 300) {
      Script_ShowUIMessageFromID("E1011A040");
      while (Script_convGetIsActive(0)) {
        SCNF_WaitFrames_0000(1);
      }
      Script_ShowUIMessageFromID("E1011A078");
      break;
      goto L015B8C;
    }
    L015B8C:;
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  return Script_srchControl(0);
}

__attribute__((scn_string_bindings(3, data_294AE, 4, data_294B8)))
static void script_15BA7(void) {
  if (task_27B == 0) {
    Script_ryoControl(PCCTL_DisablePlayerControl);
    Script_shenClearFreeState(0);
    task_27B = 1;
    SCNF_WaitFrames_0000(30);
    task_228 = 1;
    if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_TSUBA_PLACED)) {
    } else {
      if (Script_GetOwnedItemCount(4, 0)) {
        Script_sicnUtils(2, 8, 0);
      }
      {
        int32_t local_FC;
        local_FC = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f);
        switch (local_FC) {
          case 0:
            Script_ShowUIMessageFromID("E1011A089");
            break;
          case 1:
            Script_ShowUIMessageFromID("E1011A081");
            break;
          case 2:
            Script_ShowUIMessageFromID("E1011A090");
            break;
        }
      }
      while (Script_convGetIsActive(0)) {
        SCNF_WaitFrames_0000(1);
      }
      {
        int32_t local_FC;
        local_FC = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f);
        switch (local_FC) {
          case 0:
            Script_ShowUIMessageFromID("E1011A040");
            break;
          case 1:
            Script_ShowUIMessageFromID("E1011A042");
            break;
        }
      }
      while (Script_convGetIsActive(0)) {
        SCNF_WaitFrames_0000(1);
      }
      if (Script_GetOwnedItemCount(4, 0)) {
        Script_sicnUtils(0, 8, 1);
      }
    }
    Script_ryoControl(RYO_RestoreControl);
    task_228 = 0;
    Script_shenClearFreeState(1);
  }
  return;
}

__attribute__((scn_string_bindings(0, data_294C2, 4, data_294EA)))
static void script_15CDB(void) {
  if (task_27C == 0) {
    Script_ryoControl(PCCTL_DisablePlayerControl);
    Script_shenClearFreeState(0);
    task_27C = 1;
    SCNF_WaitFrames_0000(30);
    task_228 = 1;
    if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_KATANA_INSERTED)) {
      Script_ShowUIMessageFromID("E1011A031");
      while (Script_convGetIsActive(0)) {
        SCNF_WaitFrames_0000(1);
      }
    } else {
      {
        int32_t local_FC;
        local_FC = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f);
        switch (local_FC) {
          case 0:
            Script_ShowUIMessageFromID("E1011A091");
            break;
          case 1:
            Script_ShowUIMessageFromID("E1011A092");
            break;
        }
      }
      while (Script_convGetIsActive(0)) {
        SCNF_WaitFrames_0000(1);
      }
      {
        int32_t local_FC;
        local_FC = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f);
        switch (local_FC) {
          case 0:
            Script_ShowUIMessageFromID("E1011A093");
            break;
          case 1:
            Script_ShowUIMessageFromID("E1011A042");
            break;
        }
      }
      while (Script_convGetIsActive(0)) {
        SCNF_WaitFrames_0000(1);
      }
    }
    Script_ryoControl(RYO_RestoreControl);
    task_228 = 0;
    Script_shenClearFreeState(1);
  }
  return;
}

static int32_t script_15DEC(void) {
  int32_t local_FC;
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_KHKS_HINT_INTRO_SEEN) == 0) {
    Script_ShowUIMessageFromID("E1011A094");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_ShowUIMessageFromID("E1011A095");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_KHKS_HINT_INTRO_SEEN, 1);
    if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JOMO_KAGI_KEY_TAKEN) == 0) {
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 193, 1);
    }
  } else {
    Script_ShowUIMessageFromID("E1011A096");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    local_FC = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f);
    switch (local_FC) {
      case 0:
        Script_ShowUIMessageFromID("E1011A097");
        break;
      case 1:
        Script_ShowUIMessageFromID("E1011A098");
        break;
    }
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
  }
  return Script_srchControl(0);
}

static int32_t script_15EDA(void) {
  int16_t local_FE;
  int32_t local_F8;
  local_FE = 90;
  for (;;) {
    local_F8 = local_FE;
    switch (local_F8) {
      case 0:
        Script_seqEnterEvtCam(611, 0, AUTH_None);
        break;
      case 90:
        Script_seqEnterEvtCam(612, 0, AUTH_None);
        break;
      case 150:
        Script_seqEnterEvtCam(613, 0, AUTH_None);
        break;
      case 215:
        Script_scenCreateTask(script_15DEC, 0);
        break;
      case 290:
        Script_seqEnterEvtCam(614, 0, AUTH_None);
        goto L015FD5;
        break;
      case 410:
        Script_seqEnterEvtCam(615, 0, AUTH_None);
        break;
      case 530:
        Script_seqEnterEvtCam(616, 0, AUTH_None);
        break;
      case 630:
        Script_seqEnterEvtCam(617, 0, AUTH_None);
        break;
      case 750:
        Script_seqEnterEvtCam(610, 0, AUTH_None);
        break;
    }
    local_FE = local_FE + 1;
    SCNF_WaitFrames_0000(1);
  }
  L015FD5:;
  return Script_srchControl(0);
}

static void script_15FDC(void) {
  Script_ccowUtils(CHARA_AKIR, 2);
  {
    Vector3 local_F4;
    Vector3i local_E8;
    local_F4.x = -10.03f;
    (&local_F4)->y = 0.959f;
    (&local_F4)->z = -25.2f;
    local_E8.x = 0;
    (&local_E8)->y = 0x4000;
    (&local_E8)->z = 0;
    Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &local_F4);
    Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &local_E8, 0);
  }
  Script_FaceEx(CHARA_AKIR, 1, 0, 2);
  Script_PlayCharaMotion(CHARA_AKIR, 7001, 90.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_scenCreateTask(script_15EDA, 0);
  for (;;) {
    int32_t unused_E8[6];
    int32_t local_E4;
    if (local_E4 == 340) {
      break;
    }
    local_E4 = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_GetCharaMotionRemainingFramesScaled(CHARA_AKIR));
    SCNF_WaitFrames_0000(1);
  }
  {
    Vector3 local_F4;
    Vector3i local_E8;
    local_F4.x = -9.6f;
    (&local_F4)->y = 0.959f;
    (&local_F4)->z = -25.2668f;
    local_E8.x = 0;
    (&local_E8)->y = 0x4000;
    (&local_E8)->z = 0;
    Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &local_F4);
    Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &local_E8, 0);
  }
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_ccowUtils(CHARA_AKIR, 1);
  Script_FaceEx(CHARA_AKIR, 0, 0, 2);
  return;
}

static void script_16147(void) {
  if (task_27D == 0) {
    Script_ryoControl(PCCTL_DisablePlayerControl);
    Script_shenClearFreeState(0);
    task_27D = 1;
    SCNF_WaitFrames_0000(30);
    task_228 = 1;
    script_15FDC();
    task_228 = 0;
    Script_srchUtils(SRCH_SCRIPT_Reset);
    Script_convSetCamInactiveFlag(0);
    Script_SetGameStateTo(AUTH_None);
    Script_ryoControl(RYO_RestoreControl);
    Script_shenClearFreeState(1);
  }
  return;
}

__attribute__((scn_string_bindings(0, data_29526)))
static void script_161A4(void) {
  if (task_280 == 0) {
    Script_ryoControl(PCCTL_DisablePlayerControl);
    Script_shenClearFreeState(0);
    task_280 = 1;
    SCNF_WaitFrames_0000(30);
    task_228 = 1;
    Script_ShowUIMessageFromID("E1011A031");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    task_228 = 0;
    Script_srchUtils(SRCH_SCRIPT_Reset);
    Script_convSetCamInactiveFlag(0);
    Script_SetGameStateTo(AUTH_None);
    Script_ryoControl(RYO_RestoreControl);
    Script_shenClearFreeState(1);
  }
  return;
}

static void script_1621A(void) {
  if (task_280 == 0) {
    Script_ryoControl(PCCTL_DisablePlayerControl);
    task_280 = 1;
    task_228 = 1;
    Script_shenClearFreeState(0);
    Script_GetCassetteFlags(36, 1, 0, 0);
    script_162A7();
    Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
    SCNF_WaitFrames_0000(30);
    Script_scenCreateTask(script_12AF4, 0);
    Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
    script_16413();
    task_228 = 0;
    Script_shenClearFreeState(1);
  }
  return;
}

static void script_162A7(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t local_E4;
  int32_t local_E0;
  Script_srchUtils(SRCH_SCRIPT_Reset);
  local_F4.x = -9.1394f;
  (&local_F4)->y = 0.9594f;
  (&local_F4)->z = -17.967699f;
  local_E8.x = 0;
  (&local_E8)->y = 21222;
  (&local_E8)->z = 0;
  Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &local_F4);
  Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &local_E8, 0);
  Script_SetGameStateTo(AUTH_None);
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_ccowUtils(SHEN_NAME('HAKO'), 2);
  Script_EVT_GetPosition(SHEN_NAME('HAKO'), -1, &local_F4, 0);
  Script_PlayCharaMotion(CHARA_AKIR, 7025, 60.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_CharHandlers(CHARA_AKIR, 0, 6, 16);
  Script_CharHandlers(CHARA_AKIR, 1, 6, 16);
  for (;;) {
    if (local_E4 == 120) {
      local_E4 = 0;
      break;
    } else {
      local_E4 = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_GetCharaMotionRemainingFramesScaled(CHARA_AKIR));
    }
    SCNF_WaitFrames_0000(1);
    local_E0 = local_E4;
    if (local_E0 == 94) {
      Script_fixoAttachEx(SHEN_NAME('HAKO'), CHARA_AKIR, 12, &data_29530, &data_2953C);
      goto L0163F5;
    }
    L0163F5:;
  }
  Script_fixoClearAttachment(SHEN_NAME('HAKO'));
  return;
}

static int32_t script_16402(void) {
  SCNF_StartIconSystemMenu(36);
  return Script_srchControl(0);
}

static void script_16413(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t local_E4;
  int32_t local_E0;
  Script_motmToggleFlags(CHARA_AKIR, 2, 0x40);
  local_F4.x = -6.7998f;
  (&local_F4)->y = 0.96f;
  (&local_F4)->z = -17.5439f;
  local_E8.x = 0;
  (&local_E8)->y = 0x8000;
  (&local_E8)->z = 0;
  Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &local_F4);
  Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &local_E8, 0);
  local_F4.x = -6.7998f;
  (&local_F4)->y = 1.06f;
  (&local_F4)->z = -16.9373f;
  local_E8.x = 0;
  (&local_E8)->y = 0;
  (&local_E8)->z = 0;
  Script_EVT_SetPosition(SHEN_NAME('HAKO'), POS_XYZ, &local_F4);
  Script_EVT_SetRotation(SHEN_NAME('HAKO'), 0x38000000, &local_E8, 0);
  Script_FaceEx(CHARA_AKIR, 1, 0, 2);
  Script_ccowUtils(CHARA_AKIR, 2);
  Script_SetGameStateTo(AUTH_None);
  Script_ryoControl(PCCTL_DisablePlayerControl);
  task_228 = 1;
  Script_PlayCharaMotion(CHARA_AKIR, 7051, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  for (;;) {
    if (local_E4 > 760) {
      local_E4 = 0;
      break;
    } else {
      local_E4 = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_GetCharaMotionRemainingFramesScaled(CHARA_AKIR));
    }
    local_E0 = local_E4;
    switch (local_E0) {
      case 1:
        Script_seqEnterEvtCam(711, 0, AUTH_None);
        break;
      case 90:
        local_F4.x = -6.7998f;
        (&local_F4)->y = 3.484f;
        (&local_F4)->z = -16.58f;
        Script_EVT_SetPosition(SHEN_NAME('GART'), POS_XYZ, &local_F4);
        Script_seqEnterEvtCam(712, 0, AUTH_None);
        break;
      case 100:
        Script_CharHandlers(CHARA_AKIR, 0, 5, 16);
        Script_CharHandlers(CHARA_AKIR, 1, 5, 16);
        break;
      case 120:
        Script_tmnmUtils(SHEN_NAME('GART'), 7055, 0.0f, 32767.0f, 0.0f, 1.0f, 0);
        Script_executeAicaCommand(0x1D05A9, 0, 0);
        break;
      case 130:
        Script_CharHandlers(CHARA_AKIR, 0, 14, 16);
        Script_CharHandlers(CHARA_AKIR, 1, 14, 16);
        break;
      case 170:
        Script_charaDisplay(SHEN_NAME('MKCS'), CDM_Show);
        break;
      case 180:
        Script_seqEnterEvtCam(713, 0, AUTH_None);
        Script_ShowUIMessageFromID("E1011A006");
        break;
      case 240:
        Script_seqEnterEvtCam(714, 0, AUTH_None);
        break;
      case 265:
        Script_executeAicaCommand(0x3205A9, 0, 0);
        break;
      case 300:
        Script_CharHandlers(CHARA_AKIR, 0, 6, 16);
        break;
      case 350:
        local_F4.x = 0.10222f;
        (&local_F4)->y = 0.01707f;
        (&local_F4)->z = -0.00826f;
        local_E8.x = 21407;
        (&local_E8)->y = 253;
        (&local_E8)->z = 16797;
        Script_fixoAttachEx(SHEN_NAME('MKCS'), CHARA_AKIR, 18, &local_F4, &local_E8);
        break;
      case 360:
        Script_seqEnterEvtCam(715, 0, AUTH_None);
        Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 548, 1);
        Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 903, 1);
        Script_scenCreateTask(script_16402, 0);
        break;
      case 390:
        Script_ShowUIMessageFromID("E1011A102");
        break;
      case 480:
        Script_fixoClearAttachment(SHEN_NAME('MKCS'));
        Script_charaDisplay(SHEN_NAME('MKCS'), CDM_Hide);
        break;
      case 500:
        Script_CharHandlers(CHARA_AKIR, 0, 14, 16);
        Script_CharHandlers(CHARA_AKIR, 1, 14, 16);
        break;
      case 520:
        Script_seqEnterEvtCam(716, 0, AUTH_None);
        break;
      case 550:
        Script_CharHandlers(CHARA_AKIR, 0, 5, 16);
        break;
      case 560:
        Script_CharHandlers(CHARA_AKIR, 0, 14, 16);
        Script_tmnmUtils(SHEN_NAME('GART'), 7056, 0.0f, 32767.0f, 0.0f, 1.0f, 0);
        break;
      case 645:
        Script_executeAicaCommand(0x1E05A9, 0, 0);
        Script_CharHandlers(CHARA_AKIR, 0, 6, 16);
        Script_CharHandlers(CHARA_AKIR, 1, 6, 16);
        Script_tmnmReset(SHEN_NAME('GART'));
        break;
      case 646:
        local_F4.x = -6.7998f;
        (&local_F4)->y = 3.484f;
        (&local_F4)->z = -16.5422f;
        local_E8.x = 0;
        (&local_E8)->y = 0x8000;
        (&local_E8)->z = 0;
        Script_EVT_SetPosition(SHEN_NAME('GART'), POS_XYZ, &local_F4);
        Script_EVT_SetRotation(SHEN_NAME('GART'), 0x38000000, &local_E8, 0);
        break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_FaceEx(CHARA_AKIR, 0, 0, 2);
  Script_SetGameStateTo(AUTH_None);
  Script_convSetCamInactiveFlag(1);
  SCNF_WaitFrames_0000(1);
  Script_motmToggleFlags(CHARA_AKIR, 1, 0x40);
  Script_ryoControl(RYO_RestoreControl);
  Script_ccowUtils(SHEN_NAME('HAKO'), 1);
  Script_ccowUtils(CHARA_AKIR, 1);
  Script_CharHandlers(CHARA_AKIR, -1, 0, 0);
  task_228 = 0;
  return;
}

static int32_t script_1697C(void) {
  Script_scenCreateTask(script_0030, 6, CHARA_AKIR, 2, 0, &data_2955C, 2, 0);
  script_2643((int32_t)data_29590);
  task_24[0] = (int32_t)script_179FB;
  task_24[1] = (int32_t)script_1765C;
  task_24[2] = (int32_t)script_17D07;
  return Script_srchControl(0);
}

static void script_169D2(void) {
  Script_scenCreateTask(script_174D0, 0);
  Script_scenCreateTask(script_1697C, 0);
  return;
}

static int32_t script_169EB(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  local_FC = arg3;
  if (local_FC == 1) goto L0169FF;
  L0169FF:;
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_shenRequestGameModuleTransition(arg0, arg1, arg2);
  end:;
}

static int32_t script_16A3B(void) {
  int8_t local_FF;
  Vector3i local_F0;
  task_290.x = 0.0f;
  task_290.z = 13.4303f;
  task_29C.x = -2.0f;
  task_29C.z = 9.56f;
  Script_mtwkGetSetRotation(CHARA_AKIR, &local_F0, 0);
  if (Script_SetTaskDataValue(CHARA_AKIR, &task_290, &task_29C, 0) & -((&local_F0)->y > 10922) & -((&local_F0)->y < 54613)) {
    if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_SA1116_DOJO_TALK_DONE) == 0) {
      if (Script_IsPlayerAllowed(0)) {
        if (task_232 == 0) {
          task_232 = 1;
          Script_sicnUtils(0, 4, 1);
        }
      }
    }
    local_FF = 1;
  } else {
    if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_SA1116_DOJO_TALK_DONE) == 0) {
      if (Script_IsPlayerAllowed(0)) {
        if (task_232) {
          task_232 = 0;
          Script_sicnUtils(2, 4, 0);
        }
      }
    }
    local_FF = 0;
  }
  end:;
}

static void script_16B1E(void) {
  int8_t local_FF;
  task_290.x = -4.25f;
  task_290.z = -0.24f;
  task_29C.x = -1.95f;
  task_29C.z = 1.05f;
  if (Script_SetTaskDataValue(CHARA_AKIR, &task_290, &task_29C, 0)) {
    local_FF = 2;
  } else {
    local_FF = 0;
  }
  return;
}

__attribute__((scn_string_bindings(0, data_295D0, 1, data_295E0, 4, data_295F7, 5, data_295FC, 6, data_295FD, 7, data_29603, 8, data_29608, 9, data_29613)))
static void script_16B72(void) {
  Vector3 local_F4;
  int32_t unused_F0;
  int32_t local_EC;
  int32_t local_E8;
  Vector3i local_DC;
  local_E8 = Script_StepUtils(GET_STEP_HIGHER, 0);
  local_EC = 0;
  while (Script_IsPlayerAllowed(0) == 0) {
    SCNF_WaitFrames_0000(1);
  }
  Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_HIHY_EVENT_DONE, 1);
  task_228 = 1;
  Script_scenUtils(SCENARIO_CMD_INIT_SCNT);
  Script_GetSetWeatherSettings(WEATHER_SETTINGS_SET_DISABLE_WEATHER, 0);
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  local_F4.x = -6.7523f;
  (&local_F4)->y = 3.3737f;
  (&local_F4)->z = -16.330299f;
  Script_SetCharaLookAtPosition(CHARA_AKIR, 3, &local_F4, 2);
  Script_seqEnterEvtCam(801, 0, AUTH_None);
  SCNF_WaitFrames_0000(140);
  Script_SetCharaLookAtPosition(CHARA_AKIR, -1, 0, 0);
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, "/scene/01/JHD0/", "HIHY") == 0) {
    SCNF_WaitFrames_0000(1);
  }
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
  Script_filmCheckOrSetTransitionUtil(30, 0, 255, 255, 255, 255, 255, 255, 255);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_ryoSetChara(CHARA_JAKR);
  (&local_DC)->y = 30219;
  Script_EVT_SetRotation(CHARA_AKIR, MATRIX_XFORM_Y, &local_DC, 0);
  Script_LoadSND_Wrapper(0, 0, "tsm003.snd", 0, 0, 0, 0, 0);
  Script_LoadVoiceFile(1, "A0125B");
  Script_charaDisplay(SHEN_NAME('POK2'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('POS2'), CDM_Hide);
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, 0x554B5546)) {
    if (-1) {
      Script_charaDisplay(CHARA_FUKU, CDM_Hide);
    } else {
      if (0) {
        Script_charaDisplay(CHARA_FUKU, CDM_Show);
        goto L016D4D;
      }
    }
    L016D4D:;
    Script_scenToggleVisibleFor(CHARA_FUKU, 0);
  }
  script_231E5();
  Script_AuthorUtils(0, 0, "HIHY", "");
  script_F323();
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
  script_2334B();
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, 0x554B5546)) {
    if (0) {
      Script_charaDisplay(CHARA_FUKU, CDM_Hide);
    } else {
      if (-1) {
        Script_charaDisplay(CHARA_FUKU, CDM_Show);
        goto L016DBD;
      }
    }
    L016DBD:;
    Script_scenToggleVisibleFor(CHARA_FUKU, 1);
  }
  Script_charaDisplay(SHEN_NAME('POK2'), CDM_Show);
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_KATANA_INSERTED) == 0) {
    Script_charaDisplay(SHEN_NAME('POS2'), CDM_Show);
  }
  Script_LoadVoiceFile(0, "A0125");
  Script_AuthorUtils(AUTH_FREE, 0);
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "HIHY");
  Script_LoadSND_Wrapper(0, 0, "bgm009.snd", 0, "f1doujou.snd", 0, 0, 0);
  Script_executeAicaCommand(16296, 0, 0);
  Script_executeAicaCommand(1184, 2, 100);
  Script_charaDisplay(SHEN_NAME('MOON'), CDM_Hide);
  Script_ryoSetChara(CHARA_AKIR);
  Script_filmCheckOrSetTransitionUtil(30, 255, 255, 255, 255, 0, 255, 255, 255);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  task_228 = 0;
  Script_scenUtils(SCENARIO_CMD_UPDATE_TIMER);
  Script_GetSetWeatherSettings(WEATHER_SETTINGS_SET_DISABLE_WEATHER, 1);
  Script_ryoControl(RYO_RestoreControl);
  return;
}

static void script_16EA6(void) {
  Vector3 local_F4;
  int32_t local_F0;
  local_F0 = Script_StepUtils(GET_STEP_HIGHER, 0);
  if ((Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_YD01_LOOKOUT_TRANSITION_TRIGGERED) == 0) & (local_F0 < 6)) {
    task_290.x = 9.25f;
    task_290.z = -30.33f;
    task_29C.x = 6.5f;
    task_29C.z = -27.0f;
    if (Script_SetTaskDataValue(CHARA_AKIR, &task_290, &task_29C, 0)) {
      while (Script_IsPlayerAllowed(0) == 0) {
        SCNF_WaitFrames_0000(1);
      }
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_YD01_LOOKOUT_TRANSITION_TRIGGERED, 1);
      Script_ryoControl(PCCTL_DisablePlayerControl);
      Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
      local_F4.x = 6.6279f;
      (&local_F4)->y = 4.798f;
      (&local_F4)->z = -28.2931f;
      Script_SetCharaLookAtPosition(CHARA_AKIR, 3, &local_F4, 2);
      Script_seqEnterEvtCam(802, 0, AUTH_None);
      SCNF_WaitFrames_0000(140);
      Script_SetCharaLookAtPosition(CHARA_AKIR, -1, 0, 0);
      Script_executeAicaCommand(2720, 1, 30);
      Script_executeAicaCommand(2720, 2, 30);
      Script_executeAicaCommand(2720, 3, 30);
      Script_ConstructColourPair(30, 1, 0, 255, 255, 255, 255, 255, 255, 255);
      while (Script_ConstructColourPair(-1)) {
        SCNF_WaitFrames_0000(1);
      }
      Script_cdttUtil(Mode2);
      Script_cdttUtil(Mode1);
      Script_shenRequestGameModuleTransition(1, SHEN_NAME('YD01'), 0);
      SCNF_WaitFrames_0000(1);
    }
  }
  return;
}

static void script_17029(void) {
  int8_t local_FF;
  if (-(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JD00_E1049_ROUTE_UNLOCKED) == 0) & Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JOMO_A01124_INE_EVENT_DONE)) {
    task_290.x = -1.78f;
    task_290.z = 0.07f;
    task_29C.x = -0.78f;
    task_29C.z = 1.07f;
    if (Script_SetTaskDataValue(CHARA_AKIR, &task_290, &task_29C, 0)) {
      local_FF = 5;
    } else {
      local_FF = 0;
    }
  } else {
    local_FF = 0;
  }
  return;
}

__attribute__((scn_string_bindings(0, data_29620, 6, data_2965E, 8, data_29672)))
static void script_170A2(int32_t arg0) {
  int32_t unused_F4[3];
  Vector3 local_E8;
  Vector3i local_DC;
  int32_t local_D8;
  Vector3i local_CC;
  local_D8 = arg0;
  switch (local_D8) {
    case 1:
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_SA1116_DOJO_TALK_DONE) == 0) {
        while (Script_IsPlayerAllowed(0) == 0) {
          SCNF_WaitFrames_0000(1);
        }
        Script_ryoControl(PCCTL_DisablePlayerControl);
        Script_sicnUtils(2, 4, 0);
        task_228 = 1;
        Script_LoadVoiceFile(1, "SA1082");
        Script_seqEnterEvtCam(task_22C + 2, 0, AUTH_None);
        task_22C = task_22C + 1;
        if (task_22C > 2) {
          task_22C = 0;
        }
        Script_FaceEx(CHARA_AKIR, 1, 0, 2);
        Script_motmToggleFlags(CHARA_AKIR, 2, 0x40);
        local_E8.x = -1.4307f;
        (&local_E8)->y = 0.1f;
        (&local_E8)->z = 10.289f;
        local_DC.x = 0;
        (&local_DC)->y = 0x8000;
        (&local_DC)->z = 0;
        Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &local_E8);
        Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &local_DC, 0);
        Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        local_D8 = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f);
        switch (local_D8) {
          case 0:
            Script_ShowUIMessageFromID("SA1082A001");
            break;
          case 1:
            Script_ShowUIMessageFromID("SA1082A002");
            break;
        }
        while (Script_convGetIsActive(0)) {
          SCNF_WaitFrames_0000(1);
        }
        Script_motmToggleFlags(CHARA_AKIR, 1, 0x40);
        SCNF_WaitFrames_0000(30);
        local_D8 = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f);
        switch (local_D8) {
          case 0:
            Script_ShowUIMessageFromID("SA1082A003");
            break;
          case 1:
            Script_ShowUIMessageFromID("SA1082A004");
            break;
          case 2:
            Script_ShowUIMessageFromID("SA1082A005");
            break;
        }
        while (Script_convGetIsActive(0)) {
          SCNF_WaitFrames_0000(1);
        }
        Script_charaToggleMotEnabled(CHARA_AKIR, 0);
        Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TURN180_WAD_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        for (;;) {
          if (Script_IsCharaMotionFinished(CHARA_AKIR)) {
            break;
          }
          Script_RotateNodeUtils(CHARA_AKIR, &data_2501C, 0);
          SCNF_WaitFrames_0000(1);
        }
        Script_charaToggleMotEnabled(CHARA_AKIR, 1);
        (&local_CC)->y = 0;
        Script_EVT_SetRotation(CHARA_AKIR, MATRIX_XFORM_Y, &local_CC, 0);
        Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_WALK_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        SCNF_WaitFrames_0000(40);
        Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        Script_SetGameStateTo(AUTH_None);
        Script_convSetCamInactiveFlag(1);
        Script_ryoControl(RYO_RestoreControl);
        Script_FaceEx(CHARA_AKIR, 0, 0, 2);
        Script_LoadVoiceFile(0, "SA1082");
        task_228 = 0;
      }
      break;
    case 2:
      script_17F71();
      script_169EB(1, SHEN_NAME('JOMO'), 0, 0);
      SCNF_WaitFrames_0000(1);
      break;
    case 5:
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_JD00_E1049_ROUTE_UNLOCKED, 1);
      task_228 = 1;
      Script_GetCassetteFlags(6, 1, 0, 0);
      Script_LoadSND_Wrapper(0, 0, "a1_oyaji.snd", 0, 0, 0, 0, 0);
      script_E615();
      Script_LoadSND(0, 2, "bgm009.snd", 0);
      Script_executeAicaCommand(16296, 0, 0);
      Script_executeAicaCommand(1184, 2, 100);
      Script_fixoClearAttachment(SHEN_NAME('HOKI'));
      if (-1) {
        Script_charaDisplay(SHEN_NAME('HOKI'), CDM_Hide);
      } else {
        if (0) {
          Script_charaDisplay(SHEN_NAME('HOKI'), CDM_Show);
          goto L017458;
        }
      }
      L017458:;
      Script_scenToggleVisibleFor(SHEN_NAME('HOKI'), 0);
      Script_ryoControl(RYO_RestoreControl);
      Script_SetGameStateTo(AUTH_None);
      task_228 = 0;
      break;
  }
  return;
}

static int32_t script_1747B(int32_t arg0, int32_t arg1) {
  if (arg0) {
    (void)arg0;
  } else {
    (void)arg1;
  }
  end:;
}

static int32_t script_1748D(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  local_FC = script_1747B(arg0, arg1);
  switch (local_FC) {
    case 1:
      arg0 = script_16A3B();
      break;
    case 4:
      arg0 = 0;
      script_16EA6();
      break;
  }
  return arg0;
}

static int32_t script_174D0(void) {
  int8_t local_FF;
  int8_t local_FE;
  int32_t local_F8;
  local_FE = 1;
  local_FF = 0;
  local_FF = script_1748D(local_FF, local_FE);
  for (;;) {
    if (task_228 == 0) {
      local_F8 = local_FF;
      local_FF = script_1748D(local_FF, local_FE);
      if (local_FE == 6) {
        local_FE = 1;
      } else {
        local_FE = local_FE + 1;
      }
      if (local_FF) {
        if (Script_GetControllerInput(1) & 0x400) {
          script_170A2(local_FF);
        }
      }
      if ((&task_6C)->mode) {
        local_F8 = task_6C.entry_idx;
        switch (local_F8) {
          case 0:
            Script_ryoControl(PCCTL_DisablePlayerControl);
            SCNF_WaitFrames_0000(90);
            Script_executeAicaCommand(2720, 1, 30);
            Script_executeAicaCommand(2720, 2, 30);
            Script_executeAicaCommand(2720, 3, 30);
            Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
            while (Script_filmCheckOrSetTransitionUtil(-1)) {
              SCNF_WaitFrames_0000(1);
            }
            Script_shenRequestGameModuleTransition(1, SHEN_NAME('JU00'), 1);
            SCNF_WaitFrames_0000(1);
            break;
          case 1:
            Script_ryoControl(PCCTL_DisablePlayerControl);
            SCNF_WaitFrames_0000(90);
            Script_executeAicaCommand(2720, 1, 30);
            Script_executeAicaCommand(2720, 2, 30);
            Script_executeAicaCommand(2720, 3, 30);
            Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
            while (Script_filmCheckOrSetTransitionUtil(-1)) {
              SCNF_WaitFrames_0000(1);
            }
            Script_shenRequestGameModuleTransition(1, SHEN_NAME('JOMO'), 0);
            SCNF_WaitFrames_0000(1);
            break;
        }
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

__attribute__((scn_string_bindings(0, data_2967D)))
static int32_t script_1765C(void) {
  Vector3 local_F4;
  int32_t unused_E8[3];
  Vector3 local_DC;
  Vector3i local_D0;
  int32_t unused_CC;
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_seqEnterEvtCam(307, 0, AUTH_None);
  Script_ccowUtils(CHARA_AKIR, 2);
  Script_EVT_GetPosition(CHARA_AKIR, -1, &local_F4, 0);
  (&local_F4)->y = (&local_F4)->y - 0.2f;
  Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &local_F4);
  Script_mtwkGetSetRotation(CHARA_AKIR, &local_D0, 0);
  (&local_D0)->y = 0xC000;
  Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &local_D0, 0);
  Script_MoveObjectToObject(SHEN_NAME('LKUT'), 0x52494B41);
  local_DC.x = -0.2f;
  (&local_DC)->y = -0.19f;
  (&local_DC)->z = -0.3f;
  Script_EVT_SetPosition(SHEN_NAME('LKUT'), POS_XYZ, &local_DC);
  Script_EVT_GetPosition(SHEN_NAME('LKUT'), -1, &local_DC, ENTITY_XFORM_WORLD_SPACE);
  local_D0.x = 0;
  (&local_D0)->y = 0;
  (&local_D0)->z = 0;
  Script_EVT_SetRotation(SHEN_NAME('LKUT'), 0x38000000, &local_D0, 0);
  Script_mtwkGetSetRotation(SHEN_NAME('LKUT'), &local_D0, 0x40000000);
  Script_MoveObjectToObject(SHEN_NAME('LKUT'), -1);
  Script_EVT_SetPosition(SHEN_NAME('LKUT'), POS_XYZ, &local_DC);
  Script_EVT_SetRotation(SHEN_NAME('LKUT'), 0x38000000, &local_D0, 0);
  Script_MoveObjectToObject(SHEN_NAME('RKUT'), 0x52494B41);
  local_DC.x = 0.0f;
  (&local_DC)->y = -0.19f;
  (&local_DC)->z = -0.3f;
  Script_EVT_SetPosition(SHEN_NAME('RKUT'), POS_XYZ, &local_DC);
  Script_EVT_GetPosition(SHEN_NAME('RKUT'), -1, &local_DC, ENTITY_XFORM_WORLD_SPACE);
  local_D0.x = 0;
  (&local_D0)->y = 0;
  (&local_D0)->z = 0;
  Script_EVT_SetRotation(SHEN_NAME('RKUT'), 0x38000000, &local_D0, 0);
  Script_mtwkGetSetRotation(SHEN_NAME('RKUT'), &local_D0, 0x40000000);
  Script_MoveObjectToObject(SHEN_NAME('RKUT'), -1);
  Script_EVT_SetPosition(SHEN_NAME('RKUT'), POS_XYZ, &local_DC);
  Script_EVT_SetRotation(SHEN_NAME('RKUT'), 0x38000000, &local_D0, 0);
  Script_CreateNPCImageModule(CHARA_AKIR, 0, 0);
  Script_CreateNPCImageModule(CHARA_AKIR, "YKB_M", 0);
  Script_CharHandlers(CHARA_AKIR, -1, 0, 0);
  Script_charaToggleMotEnabled(CHARA_AKIR, 1);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_HAKU_KUTU_225D_F, 20.0f, -1.0f, -1.0f, 1.0f, 0);
  SCNF_WaitFrames_0000(36);
  Script_charaDisplay(SHEN_NAME('LKUT'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('RKUT'), CDM_Hide);
  Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  script_100D7();
  Script_ccowUtils(CHARA_AKIR, 1);
  local_DC.x = 1.1237f;
  (&local_DC)->y = 0.1f;
  (&local_DC)->z = -22.45f;
  local_D0.x = 0;
  (&local_D0)->y = 0xC000;
  (&local_D0)->z = 0;
  Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &local_DC);
  Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &local_D0, 0);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  SCNF_WaitFrames_0000(2);
  Script_SetCameraSettings(5, 6, 1, 0);
  Script_convSetCamInactiveFlag(1);
  Script_SetGameStateTo(AUTH_None);
  Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  (void)((task_4C = 0) & 1);
  Script_cpctStateUtils(1, 0);
  end:;
}

static int32_t script_179FB(void) {
  (void)((task_234 = 0) & 1);
  if (1) {
    task_C = *&task_C | 8;
  } else {
    task_C = *&task_C & -9;
  }
  Script_seqEnterEvtCam(305, 0, AUTH_None);
  Script_ryoControl(PCCTL_DisablePlayerControl);
  if (task_240 == 0) {
    Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT, 0x554B5546, 1);
    Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
  }
  {
    int32_t unused_F4[3];
    Vector3i local_E8;
    Script_mtwkGetSetRotation(CHARA_AKIR, &local_E8, 0);
    (&local_E8)->y = 0x4000;
    Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &local_E8, 0);
  }
  Script_motmToggleFlags(CHARA_AKIR, 2, 0x40);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_motmToggleFlags(CHARA_AKIR, 1, 0x40);
  Script_charaDisplay(SHEN_NAME('LKUT'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('RKUT'), CDM_Show);
  Script_MoveObjectToObject(SHEN_NAME('LKUT'), 0x52494B41);
  {
    Vector3 local_F4;
    local_F4.x = -0.1f;
    (&local_F4)->y = 0.0f;
    (&local_F4)->z = 0.06f;
    Script_EVT_SetPosition(SHEN_NAME('LKUT'), POS_XYZ, &local_F4);
  }
  {
    Vector3 local_F4;
    Script_EVT_GetPosition(SHEN_NAME('LKUT'), -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
    {
      Vector3i local_E8;
      local_E8.x = 0;
      (&local_E8)->y = 0;
      (&local_E8)->z = 0;
      Script_EVT_SetRotation(SHEN_NAME('LKUT'), 0x38000000, &local_E8, 0);
    }
    {
      Vector3i local_E8;
      Script_mtwkGetSetRotation(SHEN_NAME('LKUT'), &local_E8, 0x40000000);
      Script_MoveObjectToObject(SHEN_NAME('LKUT'), -1);
      Script_EVT_SetPosition(SHEN_NAME('LKUT'), POS_XYZ, &local_F4);
      Script_EVT_SetRotation(SHEN_NAME('LKUT'), 0x38000000, &local_E8, 0);
    }
  }
  Script_MoveObjectToObject(SHEN_NAME('RKUT'), 0x52494B41);
  {
    Vector3 local_F4;
    local_F4.x = 0.1f;
    (&local_F4)->y = 0.0f;
    (&local_F4)->z = 0.06f;
    Script_EVT_SetPosition(SHEN_NAME('RKUT'), POS_XYZ, &local_F4);
  }
  {
    Vector3 local_F4;
    Script_EVT_GetPosition(SHEN_NAME('RKUT'), -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
    {
      Vector3i local_E8;
      local_E8.x = 0;
      (&local_E8)->y = 0;
      (&local_E8)->z = 0;
      Script_EVT_SetRotation(SHEN_NAME('RKUT'), 0x38000000, &local_E8, 0);
    }
    {
      Vector3i local_E8;
      Script_mtwkGetSetRotation(SHEN_NAME('RKUT'), &local_E8, 0x40000000);
      Script_MoveObjectToObject(SHEN_NAME('RKUT'), -1);
      Script_EVT_SetPosition(SHEN_NAME('RKUT'), POS_XYZ, &local_F4);
      Script_EVT_SetRotation(SHEN_NAME('RKUT'), 0x38000000, &local_E8, 0);
    }
  }
  Script_CreateNPCImageModule(CHARA_AKIR, data_29683, 0);
  Script_CharHandlers(CHARA_AKIR, -1, 0, 0);
  Script_charaToggleMotEnabled(CHARA_AKIR, 1);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_NUGU_KUTU_225D_F, 20.0f, -1.0f, -1.0f, 1.0f, 0);
  while ((int32_t)Script_GetCharaMotionRemainingFramesScaled(CHARA_AKIR) < 40) {
    SCNF_WaitFrames_0000(1);
  }
  (void)((task_4C = 0) & 2);
  Script_cpctStateUtils(1, 2);
  end:;
}

static int32_t script_17CBE(void) {
  SCNF_WaitFrames_0000(45);
  Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
  SCNF_WaitFrames_0000(30);
  Script_SetCameraSettings(5, 6, 1, 0);
  Script_convSetCamInactiveFlag(1);
  Script_SetGameStateTo(AUTH_None);
  return Script_srchControl(0);
}

static int32_t script_17D07(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  local_F4.x = -1.7591f;
  (&local_F4)->y = 0.9997f;
  (&local_F4)->z = -22.4f;
  Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &local_F4);
  local_E8.x = 0;
  (&local_E8)->y = 0x4000;
  (&local_E8)->z = 0;
  Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &local_E8, 0);
  Script_seqEnterEvtCam(306, 0, AUTH_None);
  Script_scenCreateTask(script_17CBE, 0);
  Script_motmToggleFlags(CHARA_AKIR, 2, 0x40);
  Script_charaToggleMotEnabled(CHARA_AKIR, 1);
  Script_CharHandlers(CHARA_AKIR, 0, 2, 16);
  Script_PlayCharaMotion(CHARA_AKIR, 7021, -1.0f, -1.0f, 32.0f, 2.0f, 0);
  while (Script_IsCharaMotionFinished(CHARA_AKIR) == 0) {
    SCNF_WaitFrames_0000(1);
  }
  Script_CharHandlers(CHARA_AKIR, 0, 0, 16);
  Script_motmToggleFlags(CHARA_AKIR, 1, 0x40);
  script_1009F();
  Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  if (0) {
    task_C = *&task_C | 8;
  } else {
    task_C = *&task_C & -9;
  }
  (void)((task_4C = 0) & 0x40);
  if (task_240 == 0) {
    Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT, 0x554B5546, 0);
    Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 1);
  }
  end:;
}

static void script_17E7B(void) {
  Vector3 local_F4;
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_ccowUtils(CHARA_AKIR, 2);
  local_F4.x = -1.45f;
  (&local_F4)->y = 0.0f;
  (&local_F4)->z = 10.25f;
  while (Script_scenCharaSubmitMoveTargetRequest(CHARA_AKIR, &local_F4, 0x8000, MN_AKI_AKI_PUSH_DOOR, 0) != 0) {
    SCNF_WaitFrames_0000(1);
  }
  Script_scenCreateTask(script_17EFE, 0);
  SCNF_WaitFrames_0000(40);
  Script_scenCreateTask(script_169EB, 4, 1, SHEN_NAME('JU00'), 1, 0);
  return;
}

static int32_t script_17EFE(void) {
  SCNF_WaitFrames_0000(40);
  for (;;) {
    {
      Vector3i local_F4;
      Script_mtwkGetSetRotation(SHEN_NAME('MONB'), &local_F4, 0);
      (&local_F4)->y = (&local_F4)->y - 546;
      Script_EVT_SetRotation(SHEN_NAME('MONB'), 0x38000000, &local_F4, 0);
    }
    {
      Vector3i local_F4;
      Script_mtwkGetSetRotation(SHEN_NAME('MONB'), &local_F4, 0);
      if (((&local_F4)->y >= 46421) & ((&local_F4)->y <= 47331)) {
        break;
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_17F71(void) {
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_ccowSetFlag80(CHARA_AKIR, 1);
  {
    Vector3 local_F4;
    local_F4.x = -3.48f;
    (&local_F4)->z = 0.263f;
    Script_charaToggleMotEnabled(CHARA_AKIR, 0);
    SCNF_CHAR_SCNFApplyFacingTowardPoint(CHARA_AKIR, &local_F4, data_296A0, 0.0f, 0, 0);
  }
  Script_LineWalkUtils(LNWK_SCRIPT_PATCH_PATH_NODE, &data_296B0, 0, -0x3FA147AE, 0x3E86A7F0, -45993, -45993);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_WALK_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_LineWalkUtils(LNWK_SCRIPT_BEGIN_PATH_WALK, CHARA_AKIR, &data_296B0, 0, 0);
  for (;;) {
    SCNF_WaitFrames_0000(1);
    if (Script_LineWalkUtils(LNWK_SCRIPT_SET_MOVE_SCALE_GET_CMD, CHARA_AKIR, 0.06f) == -1) {
      Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_STAND_DOWN_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
      break;
    }
  }
  {
    Vector3 local_F4;
    local_F4.x = -3.48f;
    (&local_F4)->z = -0.76f;
    SCNF_CHAR_SCNFApplyFacingTowardPoint(CHARA_AKIR, &local_F4, data_296A0, 0.0f, 0, 0);
  }
  Script_charaToggleMotEnabled(CHARA_AKIR, 1);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_OPEN_HUSUMA_R, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  return;
}

static int32_t script_180BD(void) {
  int32_t local_FC;
  for (;;) {
    local_FC = script_D9B1();
    if (local_FC == 130) {
      if (script_D996() == 0x554B5546) {
        script_D989();
      }
    } else if (local_FC == 134) {
      if (script_D996() == 0x554B5546) {
        Script_scenCreateTask(script_19AB8, 0);
        script_D989();
      }
    } else {
      script_D989();
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_18129(void) {
  Script_scenCreateTask(script_180BD, 0);
  return;
}

__attribute__((scn_string_bindings(12, data_29748, 13, data_2974D, 14, data_2974E, 15, data_29753, 16, data_2975E)))
static int32_t script_18137(void) {
  Vector3 local_F4;
  Vector3 local_E8;
  Vector3 local_DC;
  Vector3i local_D0;
  int32_t local_CC;
  int32_t local_C8;
  local_F4.x = -2.78f;
  (&local_F4)->y = 1.05f;
  (&local_F4)->z = -24.35f;
  local_E8.x = -4.95f;
  (&local_E8)->y = 1.05f;
  (&local_E8)->z = -20.35f;
  for (;;) {
    if (Script_SetTaskDataValue(CHARA_AKIR, &local_F4, &local_E8, 0)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  while (Script_IsPlayerAllowed(0) == 0) {
    SCNF_WaitFrames_0000(1);
  }
  task_228 = 1;
  Script_ryoControl(PCCTL_DisablePlayerControl);
  task_22A = 1;
  Script_shenClearFreeState(0);
  Script_ccowUtils(CHARA_FUKU, 2);
  Script_ccowUtils(CHARA_AKIR, 2);
  local_DC.x = -3.3f;
  (&local_DC)->y = 0.9594f;
  (&local_DC)->z = -22.25f;
  local_D0.x = 0;
  (&local_D0)->y = 0x5000;
  (&local_D0)->z = 0;
  Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &local_DC);
  Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &local_D0, 0);
  Script_FaceEx(CHARA_AKIR, 1, 0, 0);
  Script_PlayCharaMotion(CHARA_AKIR, 7038, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_EVT_GetPosition(CHARA_AKIR, 5, &local_DC, 0);
  Script_SetCharaLookAtPosition(CHARA_FUKU, 6, &local_DC, 2);
  local_CC = 0;
  for (;;) {
    local_C8 = local_CC;
    switch (local_C8) {
      case 0:
        Script_seqEnterEvtCam(1102, 0, AUTH_None);
        break;
      case 30:
        Script_ShowUIMessageFromID("SA1116B001");
        break;
      case 110:
        Script_ShowUIMessageFromID("SA1116A001");
        break;
      case 270:
        Script_ShowUIMessageFromID("SA1116B002");
        break;
      case 390:
        Script_seqEnterEvtCam(1104, 0, AUTH_None);
        Script_ShowUIMessageFromID("SA1116A002");
        break;
      case 510:
        Script_seqEnterEvtCam(1105, 0, AUTH_None);
        Script_ShowUIMessageFromID("SA1116B003");
        break;
      case 690:
        Script_ShowUIMessageFromID("SA1116A003");
        break;
      case 795:
        Script_seqEnterEvtCam(1106, 0, AUTH_None);
        Script_ShowUIMessageFromID("SA1116B004");
        break;
      case 915:
        Script_seqEnterEvtCam(1107, 0, AUTH_None);
        Script_ShowUIMessageFromID("SA1116A004");
        break;
      case 1035:
        Script_ShowUIMessageFromID("SA1116A005");
        break;
      case 1100:
        Script_ShowUIMessageFromID("SA1116A006");
        break;
      case 1160:
        Script_seqEnterEvtCam(1108, 0, AUTH_None);
        Script_ShowUIMessageFromID("SA1116B005");
        break;
      case 1245:
        goto L0183FF;
        break;
    }
    local_CC = local_CC + 1;
    SCNF_WaitFrames_0000(1);
  }
  L0183FF:;
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_SetCharaLookAtPosition(CHARA_FUKU, -6, 0, 0);
  Script_LoadSND(0, 2, "bgm202.snd", 0);
  Script_executeAicaCommand(16296, 0, 0);
  Script_executeAicaCommand(1184, 2, 90);
  Script_motmToggleFlags(CHARA_FUKU, 2, 8);
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
  Script_AuthorUtils(0, 0, "MSKA", "");
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
  script_E59C();
  Script_executeAicaCommand(2720, 2, 30);
  Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_shenSeqUtils(-6);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_PlayCharaMotion(CHARA_FUKU, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  SCNF_WaitFrames_0000(1);
  Script_AuthorUtils(AUTH_FREE, 0);
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "MSKA");
  Script_LoadSND(0, 2, "bgm009.snd", 0);
  Script_executeAicaCommand(16296, 0, 0);
  Script_executeAicaCommand(1184, 2, 100);
  Script_ccowUtils(CHARA_FUKU, 1);
  Script_ccowUtils(CHARA_AKIR, 1);
  local_D0.x = 0;
  (&local_D0)->y = 53703;
  (&local_D0)->z = 0;
  Script_EVT_SetRotation(CHARA_FUKU, 0x38000000, &local_D0, 0);
  Script_PlayCharaMotion(CHARA_FUKU, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_motmToggleFlags(CHARA_AKIR, 2, 0x40);
  local_DC.x = 3.6714f;
  (&local_DC)->y = 0.0f;
  (&local_DC)->z = -22.493f;
  local_D0.x = 0;
  (&local_D0)->y = 0xC000;
  (&local_D0)->z = 0;
  Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &local_DC);
  Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &local_D0, 0);
  Script_CreateNPCImageModule(CHARA_AKIR, "YKB_M", 0);
  Script_charaDisplay(SHEN_NAME('LKUT'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('RKUT'), CDM_Hide);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_CharHandlers(CHARA_AKIR, -1, 0, 0);
  Script_FaceEx(CHARA_AKIR, 0, 0, 0);
  Script_InitializeOsag(CHARA_AKIR);
  Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_PROGRESS_STEP05_SA1116_DOJO_DONE, 1);
  Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_SA1116_DOJO_TALK_DONE, 1);
  script_BC04(0);
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 0);
  script_100D7();
  SCNF_WaitFrames_0000(1);
  Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
  Script_SetGameStateTo(AUTH_None);
  Script_convSetCamInactiveFlag(1);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_ryoControl(RYO_RestoreControl);
  Script_motmToggleFlags(CHARA_AKIR, 1, 0x40);
  task_228 = 0;
  Script_shenClearFreeState(1);
  script_FEDE();
  end:;
}

static void script_1872C(void) {
  int32_t unused_FC;
  GameDateTime local_F4;
  Script_scenToggleActiveFor(SHEN_NAME('WIS1'), 0);
  Script_scenToggleActiveFor(SHEN_NAME('WIS2'), 0);
  Script_scenToggleActiveFor(SHEN_NAME('WIS3'), 0);
  Script_scenToggleActiveFor(SHEN_NAME('WIS4'), 0);
  Script_scenToggleActiveFor(SHEN_NAME('WIS5'), 0);
  Script_scenToggleActiveFor(SHEN_NAME('WIS6'), 0);
  while (1) {
    if (task_228 == 0) {
      Script_ReturnTimeDate(&local_F4);
      if (((&local_F4)->hours < 9) | ((&local_F4)->hours >= 19)) {
        Script_charaDisplay(SHEN_NAME('WIS1'), CDM_Hide);
        Script_charaDisplay(SHEN_NAME('WIS2'), CDM_Hide);
        Script_charaDisplay(SHEN_NAME('WIS3'), CDM_Hide);
        Script_charaDisplay(SHEN_NAME('WIS4'), CDM_Hide);
        Script_charaDisplay(SHEN_NAME('WIS5'), CDM_Hide);
        Script_charaDisplay(SHEN_NAME('WIS6'), CDM_Hide);
      } else {
        Script_charaDisplay(SHEN_NAME('WIS1'), CDM_Show);
        Script_charaDisplay(SHEN_NAME('WIS2'), CDM_Show);
        Script_charaDisplay(SHEN_NAME('WIS3'), CDM_Show);
        Script_charaDisplay(SHEN_NAME('WIS4'), CDM_Show);
        Script_charaDisplay(SHEN_NAME('WIS5'), CDM_Show);
        Script_charaDisplay(SHEN_NAME('WIS6'), CDM_Show);
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  return;
}

__attribute__((scn_string_bindings(0, data_29A00, 2, data_29A19, 4, data_29A33)))
static void script_18836(void) {
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_PROMISED_TERRY_TO_FIGHT_CHEN_GUI_ZHANG) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 935) == 0)) {
  } else {
    task_31C = Script_MT5Loader_TRWV_TRWAVE(0, &data_299F4, "model/object", "trlake1.pvr", "model/object", "trsea025.mt6", 0.25f, 0x1000);
    Script_MT5Loader_TRWV_TRWAVE(17, task_31C, "model/object", data_29A40);
    Script_MT5Loader_TRWV_TRWAVE(13, task_31C, 0.0196078438f, 0.494117647f);
    Script_MT5Loader_TRWV_TRWAVE(14, task_31C, -0.0196078438f, -0.0431372561f);
    Script_MT5Loader_TRWV_TRWAVE(16, task_31C, 0, 0);
    Script_MT5Loader_TRWV_TRWAVE(21, task_31C, 2, 2);
    Script_MT5Loader_TRWV_TRWAVE(24, task_31C, 255, 169, 190, 125);
    Script_MT5Loader_TRWV_TRWAVE(10, task_31C, 4.0f, 4.0f, 0, 1, 0, 1);
    Script_scenCreateTask(script_18B05, 0);
  }
  script_18926((int32_t)&data_29764);
  return;
}

static void script_18926(int32_t arg0) {
  {
    Vector3 local_F4;
    local_F4.x = 2.0f;
    (&local_F4)->y = 2.0f;
    (&local_F4)->z = 2.0f;
    Script_hmdlApplyScale(*(int32_t *)arg0, &local_F4);
  }
  Script_scenCreateTask(script_18A8B, 3, *(int32_t *)arg0, &data_297D0, 0x3C03126F);
  {
    Vector3 local_F4;
    local_F4.x = 2.0f;
    (&local_F4)->y = 2.0f;
    (&local_F4)->z = 2.0f;
    Script_hmdlApplyScale(*(int32_t *)(arg0 + 4), &local_F4);
  }
  Script_scenCreateTask(script_18A8B, 3, *(int32_t *)(arg0 + 4), &data_29844, 0x3BA3D70A);
  {
    Vector3 local_F4;
    local_F4.x = 2.3f;
    (&local_F4)->y = 2.3f;
    (&local_F4)->z = 2.3f;
    Script_hmdlApplyScale(*(int32_t *)(arg0 + 8), &local_F4);
  }
  Script_scenCreateTask(script_18A8B, 3, *(int32_t *)(arg0 + 8), &data_29888, 0x3BE56042);
  {
    Vector3 local_F4;
    local_F4.x = 2.0f;
    (&local_F4)->y = 2.0f;
    (&local_F4)->z = 2.0f;
    Script_hmdlApplyScale(*(int32_t *)(arg0 + 12), &local_F4);
  }
  Script_scenCreateTask(script_18A8B, 3, *(int32_t *)(arg0 + 12), &data_2993C, 0x3BC49BA6);
  {
    Vector3 local_F4;
    local_F4.x = 1.5f;
    (&local_F4)->y = 1.5f;
    (&local_F4)->z = 1.5f;
    Script_hmdlApplyScale(*(int32_t *)(arg0 + 16), &local_F4);
  }
  Script_scenCreateTask(script_18A8B, 3, *(int32_t *)(arg0 + 16), &data_29990, 0x3C23D70A);
  return;
}

static int32_t script_18A8B(int32_t arg0, const void *arg1, float arg2) {
  Script_tmnmUtils(arg0, 60426, 0.0f, 32767.0f, 0.0f, 1.0f, 0);
  Script_tmnmUtils(arg0, 0, -1.0f, -1.0f, -1.0f, 32767.0f, 4);
  Script_LineWalkUtils(LNWK_SCRIPT_BEGIN_PATH_WALK, arg0, arg1, 0x10000, 72);
  while (1) {
    if (task_228 == 0) {
      Script_LineWalkUtils(LNWK_SCRIPT_SET_MOVE_SCALE_GET_CMD, arg0, arg2);
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_18B05(void) {
  script_18B37((int32_t)&data_29778);
  while (1) {
    script_18CCF((int32_t)&data_29778);
    SCNF_WaitFrames_0000(1);
  }
  return Script_srchControl(0);
}

static void script_18B37(int32_t arg0) {
  {
    Vector3 local_F4;
    local_F4.x = 1.2f;
    (&local_F4)->y = 1.0f;
    (&local_F4)->z = 1.2f;
    Script_hmdlApplyScale(*(int32_t *)(arg0 + 12), &local_F4);
  }
  {
    Vector3 local_F4;
    local_F4.x = 0.8f;
    (&local_F4)->y = 1.0f;
    (&local_F4)->z = 0.8f;
    Script_hmdlApplyScale(*(int32_t *)(arg0 + 20), &local_F4);
  }
  {
    Vector3 local_F4;
    local_F4.x = 1.2f;
    (&local_F4)->y = 1.0f;
    (&local_F4)->z = 1.2f;
    Script_hmdlApplyScale(*(int32_t *)(arg0 + 24), &local_F4);
  }
  {
    Vector3 local_F4;
    local_F4.x = 1.6f;
    (&local_F4)->y = 1.0f;
    (&local_F4)->z = 1.6f;
    Script_hmdlApplyScale(*(int32_t *)(arg0 + 28), &local_F4);
  }
  {
    Vector3 local_F4;
    local_F4.x = 0.6f;
    (&local_F4)->y = 1.0f;
    (&local_F4)->z = 0.6f;
    Script_hmdlApplyScale(*(int32_t *)(arg0 + 32), &local_F4);
  }
  {
    Vector3 local_F4;
    local_F4.x = 1.3f;
    (&local_F4)->y = 1.0f;
    (&local_F4)->z = 1.3f;
    Script_hmdlApplyScale(*(int32_t *)(arg0 + 36), &local_F4);
  }
  {
    Vector3 local_F4;
    local_F4.x = 0.7f;
    (&local_F4)->y = 1.0f;
    (&local_F4)->z = 0.7f;
    Script_hmdlApplyScale(*(int32_t *)(arg0 + 40), &local_F4);
  }
  {
    Vector3 local_F4;
    local_F4.x = 0.9f;
    (&local_F4)->y = 1.0f;
    (&local_F4)->z = 0.9f;
    Script_hmdlApplyScale(*(int32_t *)(arg0 + 48), &local_F4);
  }
  {
    Vector3 local_F4;
    local_F4.x = 1.2f;
    (&local_F4)->y = 1.0f;
    (&local_F4)->z = 1.2f;
    Script_hmdlApplyScale(*(int32_t *)(arg0 + 52), &local_F4);
  }
  return;
}

static void script_18CCF(int32_t arg0) {
  int32_t local_FC;
  Vector3 local_F0;
  Vector3 local_E4;
  local_FC = 0;
  while (*(int32_t *)(arg0 + (local_FC << 2)) != -1) {
    Script_EVT_GetPosition(*(int32_t *)(arg0 + (local_FC << 2)), -1, &local_F0, 0);
    local_E4.x = (&local_F0)->x - (&data_299F4)->x;
    (&local_E4)->y = (&local_F0)->y;
    (&local_E4)->z = (&local_F0)->z - (&data_299F4)->z;
    *(int32_t *)&(&local_F0)->y = Script_MT5Loader_TRWV_TRWAVE(18, task_31C, &local_E4);
    Script_EVT_SetPosition(*(int32_t *)(arg0 + (local_FC << 2)), POS_XYZ, &local_F0);
    local_FC = local_FC + 1;
  }
  return;
}

static int32_t script_18D61(void) {
  Vector3i local_F4;
  Vector3 local_E8;
  int32_t local_E4;
  task_320 = 0;
  task_322 = 0;
  Script_EVT_GetPosition(SHEN_NAME('SSOD'), -1, &local_E8, 0);
  for (;;) {
    Script_mtwkGetSetRotation(SHEN_NAME('SSOD'), &local_F4, 0);
    local_E4 = task_320;
    switch (local_E4) {
      case 0:
        if (task_322 == 540) {
          task_322 = 0;
          task_320 = 1;
        }
        break;
      case 1:
        (&local_F4)->z = (&local_F4)->z - 273;
        if (task_322 == 19) {
          Script_Execute3DSoundCommand_Wrapper(263593, 4, &local_E8, 20.0f, 100.0f, 0);
        }
        if (task_322 == 39) {
          task_322 = 0;
          task_320 = 2;
        }
        break;
      case 2:
        (&local_F4)->z = (&local_F4)->z + 546;
        if (task_322 == 18) {
          Script_Execute3DSoundCommand_Wrapper(132521, 4, &local_E8, 20.0f, 100.0f, 0);
        }
        if (task_322 == 19) {
          task_322 = 0;
          task_320 = 0;
          (&local_F4)->z = 0;
        }
        break;
    }
    task_322 = task_322 + 1;
    Script_EVT_SetRotation(SHEN_NAME('SSOD'), 0x38000000, &local_F4, 0);
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_18E9D(void) {
  int32_t local_FC;
  int32_t local_F8;
  local_FC = 0;
  for (;;) {
    local_F8 = local_FC;
    switch (local_F8) {
      case 0:
        Script_seqEnterEvtCam(3000, 0, AUTH_None);
        break;
      case 60:
        Script_seqEnterEvtCam(3001, 0, AUTH_None);
        break;
      case 400:
        Script_executeAicaCommand(0x3805A9, 0, 0);
        break;
      case 463:
        Script_filmCheckOrSetTransitionUtil(25, 0, 0, 0, 0, 255, 0, 0, 0);
        break;
      case 493:
        Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
        goto L018F5F;
        break;
      case 696:
        break;
    }
    SCNF_WaitFrames_0000(1);
    local_FC = local_FC + 1;
  }
  L018F5F:;
  return Script_srchControl(0);
}

static int32_t script_18F66(void) {
  int32_t local_FC;
  int32_t local_F8;
  local_FC = 0;
  for (;;) {
    local_F8 = local_FC;
    switch (local_F8) {
      case 0:
        Script_seqEnterEvtCam(3002, 0, AUTH_None);
        break;
      case 100:
        Script_seqEnterEvtCam(3003, 0, AUTH_None);
        goto L018FB9;
        break;
    }
    SCNF_WaitFrames_0000(1);
    local_FC = local_FC + 1;
  }
  L018FB9:;
  return Script_srchControl(0);
}

__attribute__((scn_string_bindings(0, data_29A70, 1, data_29A77, 3, data_29A84)))
static void script_18FC0(void) {
  int32_t unused_F4[3];
  task_22A = 1;
  Script_LoadVoiceFile(1, "A01124");
  Script_LoadVoiceFile(1, "01KAK");
  Script_LoadVoiceFile(1, "SA1077");
  script_FF36();
  Script_CreateNPCImageModule(CHARA_FUKU, "FUK_M", 0);
  Script_charaDisplay(CHARA_FUKU, CDM_Hide);
  Script_charaDisplay(CHARA_INE_, CDM_Hide);
  return;
}

__attribute__((scn_string_bindings(0, data_29A8A, 1, data_29A9A, 2, data_29A9F, 3, data_29AA4, 4, data_29AA5, 5, data_29AAA, 6, data_29AAB, 7, data_29AB0, 8, data_29AB1, 9, data_29AB6, 10, data_29AB7, 11, data_29AC2, 13, data_29AD3, 17, data_29AFA, 20, data_29B18, 22, data_29B24, 23, data_29B2F)))
static int32_t script_1901A(void) {
  Vector3 local_F4;
  Vector3 local_E8;
  Vector3 local_DC;
  Vector3i local_D0;
  int32_t unused_C8[2];
  GameDateTime local_C0;
  int32_t local_BC;
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_KAKG_ROUTE_ACTIVE) == 0) {
    Script_ryoControl(RYO_RestoreControl);
    Script_shenClearFreeState(1);
    local_E8.x = -1.5f;
    (&local_E8)->y = 0.0f;
    (&local_E8)->z = -16.0f;
    local_DC.x = 12.0f;
    (&local_DC)->y = 0.0f;
    (&local_DC)->z = -30.0f;
    for (;;) {
      Script_ReturnTimeDate(&local_C0);
      local_BC = (&local_C0)->hours * 60 + (&local_C0)->minutes;
      if (Script_SetTaskDataValue(CHARA_AKIR, &local_E8, &local_DC, 0) & -(local_BC > 1140) & -(local_BC < 1410)) {
        break;
      }
      SCNF_WaitFrames_0000(1);
    }
    while (Script_IsPlayerAllowed(0) == 0) {
      SCNF_WaitFrames_0000(1);
    }
    Script_SetShouldSleepFlag(0);
    Script_ryoControl(PCCTL_DisablePlayerControl);
    Script_shenClearFreeState(0);
    Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
    while (Script_filmCheckOrSetTransitionUtil(-1)) {
      SCNF_WaitFrames_0000(1);
    }
    while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, "/scene/01/JHD0/", "KAKG") == 0) {
      SCNF_WaitFrames_0000(1);
    }
    while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
    Script_AuthorUtils(0, 0, "KAKG", "");
    Script_AuthorUtils(0, 1, "KAKG", "");
    Script_AuthorUtils(0, 2, "KAKG", "");
    Script_AuthorUtils(0, 3, "KAKG", "");
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
    Script_LoadSND(0, 2, "bgm012.snd", 0);
    Script_executeAicaCommand(9384, 0, 0);
    Script_executeAicaCommand(1184, 2, 127);
    Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
  }
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_shenClearFreeState(0);
  script_1009F();
  Script_ccowUtils(CHARA_AKIR, 2);
  Script_scenUtils(SCENARIO_CMD_INIT_SCNT);
  Script_scenCreateTask(script_18E9D, 0);
  task_228 = 1;
  Script_FaceLookAtTargetUtils(CHARA_AKIR, 0);
  Script_CreateNPCImageModule(CHARA_AKIR, "YKE_M", 0);
  Script_FaceEx(CHARA_AKIR, 1, 0, 0);
  Script_FACE_LCG(CHARA_AKIR, 1, 1, 0);
  Script_charaDisplay(CHARA_AKIR, CDM_Hide);
  local_F4.x = -6.8f;
  (&local_F4)->y = 0.96f;
  (&local_F4)->z = -19.5f;
  local_D0.x = 0;
  (&local_D0)->y = 0x8000;
  (&local_D0)->z = 0;
  Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &local_F4);
  Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &local_D0, 0);
  Script_motmToggleFlags(CHARA_AKIR, 2, 0x40);
  Script_PlayCharaMotion(CHARA_AKIR, 7008, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_SetCPAMFlags(CHARA_FUKU, 0);
  local_F4.x = 100.0f;
  (&local_F4)->y = 0.0f;
  (&local_F4)->z = 0.0f;
  Script_EVT_SetPosition(CHARA_FUKU, POS_XYZ, &local_F4);
  Script_PlayCharaMotion(CHARA_FUKU, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_charaDisplay(CHARA_AKIR, CDM_Show);
  SCNF_WaitFrames_0000(60);
  SCNF_WaitFrames_0000(120);
  SCNF_WaitFrames_0000(60);
  SCNF_WaitFrames_0000(20);
  Script_FACE_LCG(CHARA_AKIR, 0, 0, 0);
  SCNF_WaitFrames_0000(10);
  Script_ShowUIMessageFromID("SA1077A001");
  SCNF_WaitFrames_0000(80);
  Script_PlayCharaMotion(CHARA_AKIR, 7009, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  while (Script_IsCharaMotionFinished(CHARA_AKIR) == 0) {
    SCNF_WaitFrames_0000(1);
  }
  script_1A8F1();
  Script_CreateNPCImageModule(CHARA_AKIR, "YKF_M", 0);
  script_198BE();
  Script_scenCreateTask(script_18F66, 0);
  local_F4.x = -5.5f;
  (&local_F4)->y = 0.96f;
  (&local_F4)->z = -18.7f;
  local_D0.x = 0;
  (&local_D0)->y = 0;
  (&local_D0)->z = 0;
  Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &local_F4);
  Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &local_D0, 0);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  local_F4.x = -4.2f;
  (&local_F4)->y = 0.96f;
  (&local_F4)->z = -21.25f;
  local_D0.x = 0;
  (&local_D0)->y = 26396;
  (&local_D0)->z = 0;
  Script_EVT_SetPosition(CHARA_FUKU, POS_XYZ, &local_F4);
  Script_EVT_SetRotation(CHARA_FUKU, 0x38000000, &local_D0, 0);
  Script_charaDisplay(CHARA_FUKU, CDM_Show);
  Script_scenToggleActiveFor(CHARA_FUKU, 1);
  Script_SetCharaFlags(CHARA_FUKU, 4);
  Script_FaceEx(CHARA_AKIR, 0, 0, 0);
  Script_ShowUIMessageFromID("SA1077B001");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  SCNF_WaitFrames_0000(30);
  Script_ShowUIMessageFromID("SA1077A002");
  SCNF_CHAR_SCNFApplyFacingTowardPoint(CHARA_AKIR, &local_F4, data_27E18, 0.0f, 0, 0);
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_ShowUIMessageFromID("SA1077B002");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  SCNF_WaitFrames_0000(15);
  Script_scenToggleCharaFlag(0, 200, 1);
  script_1A163();
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 50.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 15.0f);
  script_1B175();
  Script_scenToggleCharaFlag(0, 200, 0);
  Script_CreateNPCImageModule(CHARA_AKIR, "YKC_M", 0);
  Script_charaDisplay(SHEN_NAME('KAWA'), CDM_Hide);
  Script_FaceEx(CHARA_AKIR, 1, 0, 0);
  Script_SetCharaFlags(CHARA_FUKU, 4);
  Script_LoadSND(0, 4, "a1_kakgo.snd", 0);
  Script_LoadSND(0, 2, "bgm085.snd", 0);
  Script_executeAicaCommand(16296, 0, 0);
  Script_executeAicaCommand(1184, 2, 100);
  Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
  Script_SetMotnFlag(CHARA_FUKU, -1);
  script_F70F();
  Script_SetCPAMFlags(CHARA_FUKU, 0);
  if (Script_shenSeqUtils(-7)) {
    Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
    while (Script_filmCheckOrSetTransitionUtil(-1)) {
      SCNF_WaitFrames_0000(1);
    }
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
  }
  Script_ccowUtils(CHARA_AKIR, 1);
  local_D0.x = 0;
  (&local_D0)->y = 0xC000;
  (&local_D0)->z = 0;
  Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &local_D0, 0);
  Script_charaDisplay(CHARA_INE_, CDM_Show);
  local_F4.x = -0.52f;
  (&local_F4)->y = 0.0f;
  (&local_F4)->z = 2.23f;
  local_D0.x = 0;
  (&local_D0)->y = 0xC000;
  (&local_D0)->z = 0;
  Script_EVT_SetPosition(CHARA_INE_, POS_XYZ, &local_F4);
  Script_EVT_SetRotation(CHARA_INE_, 0x38000000, &local_D0, 0);
  Script_PlayCharaMotion(CHARA_INE_, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_SA1077_INE_HANDOFF_DONE, 1);
  Script_CreateNPCImageModule(CHARA_AKIR, "YKB_M", 0);
  Script_CreateNPCImageModule(CHARA_INE_, "INE_M", 0);
  script_100D7();
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 1);
  Script_scenToggleActiveFor(CHARA_INE_, 1);
  Script_SetCharaFlags(CHARA_INE_, 4);
  Script_LoadSND(0, 2, "bgm049.snd", 0);
  Script_executeAicaCommand(17064, 0, 0);
  Script_executeAicaCommand(1184, 2, 88);
  script_FA8C();
  Script_GetCassetteFlags(2, 1, 0, 0);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_PlayCharaMotion(CHARA_FUKU, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_PlayCharaMotion(CHARA_INE_, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  SCNF_WaitFrames_0000(1);
  Script_AuthorUtils(AUTH_FREE, 0);
  Script_AuthorUtils(AUTH_FREE, 1);
  Script_AuthorUtils(AUTH_FREE, 2);
  Script_AuthorUtils(AUTH_FREE, 3);
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "KAKG");
  Script_shenRequestGameModuleTransition(1, SHEN_NAME('JOMO'), 17);
  SCNF_WaitFrames_0000(1);
  end:;
}

static void script_198BE(void) {
  int32_t local_FC;
  script_199DA();
  local_FC = Script_scenCreateTask(script_19900, 0);
  Script_shenSeqUtils(0);
  while (Script_shenSeqUtils(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_srchControl((void *)local_FC);
  Script_tmnmReset(SHEN_NAME('KAWA'));
  return;
}

static int32_t script_19900(void) {
  int32_t local_FC;
  int32_t unused_F0[3];
  Vector3i local_E4;
  int32_t local_E0;
  local_FC = 0;
  Script_charaDisplay(SHEN_NAME('KAWA'), CDM_Hide);
  Script_ccowSetFlag80(SHEN_NAME('KAWA'), 1);
  local_E4.x = 0;
  (&local_E4)->y = 4187;
  (&local_E4)->z = 0;
  Script_SetHandRootRotation(CHARA_AKIR, 0, 0x15, &local_E4);
  for (;;) {
    local_E0 = local_FC;
    switch (local_E0) {
      case 60:
        Script_tmnmUtils(SHEN_NAME('KAWA'), 4119, 0.0f, 100.0f, 0.0f, 1.0f, 0);
        break;
      case 61:
        Script_charaDisplay(SHEN_NAME('KAWA'), CDM_Show);
        break;
      case 110:
        Script_handSetFloatsFor(CHARA_AKIR, 0, data_29B34, 10);
        break;
      case 157:
        Script_executeAicaCommand(0x3705A9, 0, 0);
        break;
    }
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_199DA(void) {
  script_19A11(CHARA_AKIR);
  Script_SetVector4ByOffset(&data_29C18);
  (void)1;
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 1);
  Script_SetCharaRefBoardPos(CHARA_AKIR, &data_29C18);
  return;
}

static void script_19A11(int32_t arg0) {
  Script_ccowUtils(arg0, 2);
  Script_ccowSetFlag80(arg0, 1);
  Script_motmGetCharaFlags(arg0, 0, 0);
  Script_facemotUtils(arg0, 0, 0);
  Script_facemotUtils(arg0, 1, 0);
  Script_FaceEx(arg0, 1, 0, 1);
  Script_ResetHandMotion(arg0, 1, 1, 0);
  Script_ResetHandMotion(arg0, 1, 0, 0);
  Script_handSetFloatsFor(arg0, 0, data_273B8, 10);
  Script_handSetFloatsFor(arg0, 1, data_273B8, 10);
  Script_SetCPAMFlags(arg0, 0);
  Script_FaceLookAtTargetUtils(arg0, 0);
  return;
}

static void script_19A98(int32_t arg0) {
  Script_ccowUtils(arg0, 2);
  Script_ccowSetFlag80(arg0, 1);
  Script_CharHandlers(arg0, 2, 8, 16);
  return;
}

__attribute__((scn_string_bindings(0, data_29C28, 1, data_29C38, 2, data_29C3D, 3, data_29C42, 4, data_29C43, 5, data_29C48, 6, data_29C49, 7, data_29C54, 8, data_29C5A, 9, data_29C60, 16, data_29CA2, 17, data_29CA7)))
static int32_t script_19AB8(void) {
  while (Script_IsPlayerAllowed(0) == 0) {
    SCNF_WaitFrames_0000(1);
  }
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_shenClearFreeState(0);
  Script_PlayerStatsUtils(PS_SetStatU8_2, Script_PlayerStatsUtils(PS_GetStatU8_2) + 1);
  task_228 = 1;
  Script_scenToggleActiveFor(CHARA_FUKU, 1);
  Script_SetCPAMFlags(CHARA_FUKU, 0);
  Script_filmCheckOrSetTransitionUtil(15, 0, 0, 0, 0, 255, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  script_1009F();
  Script_scenToggleActiveFor(CHARA_FUKU, 1);
  Script_SetCharaFlags(CHARA_FUKU, 4);
  {
    Vector3 local_F4;
    Vector3i local_E8;
    local_F4.x = 0.85f;
    (&local_F4)->y = 0.1f;
    (&local_F4)->z = -22.4f;
    local_E8.x = 0;
    (&local_E8)->y = 0x4000;
    (&local_E8)->z = 0;
    Script_EVT_SetPosition(SHEN_NAME('LKUT'), POS_XYZ, &local_F4);
    Script_EVT_SetRotation(SHEN_NAME('LKUT'), 0x38000000, &local_E8, 0);
  }
  {
    Vector3 local_F4;
    Vector3i local_E8;
    local_F4.x = 0.85f;
    (&local_F4)->y = 0.1f;
    (&local_F4)->z = -22.6f;
    local_E8.x = 0;
    (&local_E8)->y = 0x4000;
    (&local_E8)->z = 0;
    Script_EVT_SetPosition(SHEN_NAME('RKUT'), POS_XYZ, &local_F4);
    Script_EVT_SetRotation(SHEN_NAME('RKUT'), 0x38000000, &local_E8, 0);
  }
  Script_charaDisplay(SHEN_NAME('LKUT'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('RKUT'), CDM_Show);
  script_FF36();
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, "/scene/01/JHD0/", "KAKG") == 0) {
    SCNF_WaitFrames_0000(1);
  }
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
  Script_AuthorUtils(0, 0, "KAKG", "");
  Script_AuthorUtils(0, 1, "KAKG", "");
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
  Script_LoadSND(0, 2, "bgm012.snd", 0);
  Script_executeAicaCommand(9384, 0, 0);
  Script_executeAicaCommand(1184, 2, 127);
  Script_CreateNPCImageModule(CHARA_AKIR, "YKF_M", 0);
  Script_CreateNPCImageModule(CHARA_FUKU, "FUK_M", 0);
  SCNF_WaitFrames_0000(1);
  Script_motmToggleFlags(CHARA_AKIR, 2, 0x40);
  Script_filmCheckOrSetTransitionUtil(15, 255, 0, 0, 0, 0, 0, 0, 0);
  script_198BE();
  Script_scenToggleCharaFlag(0, 200, 1);
  script_1A8F1();
  script_1A163();
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 50.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 15.0f);
  script_1B175();
  Script_scenToggleCharaFlag(0, 200, 0);
  Script_shenClearFreeState(0);
  Script_motmToggleFlags(CHARA_AKIR, 2, 0x40);
  Script_SetCharaLookAtPosition(CHARA_AKIR, -20, 0, 0);
  Script_FaceEx(CHARA_AKIR, 1, 0, 0);
  {
    Vector3 local_F4;
    Vector3i local_E8;
    local_F4.x = -7.0f;
    (&local_F4)->y = 1.0f;
    (&local_F4)->z = -20.4f;
    local_E8.x = 0;
    (&local_E8)->y = 0;
    (&local_E8)->z = 0;
    Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &local_F4);
    Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &local_E8, 0);
  }
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  {
    Vector3 local_F4;
    Vector3i local_E8;
    local_F4.x = -7.0f;
    (&local_F4)->y = 1.0f;
    (&local_F4)->z = -21.6f;
    local_E8.x = 0;
    (&local_E8)->y = 0x8000;
    (&local_E8)->z = 0;
    Script_EVT_SetPosition(CHARA_FUKU, POS_XYZ, &local_F4);
    Script_EVT_SetRotation(CHARA_FUKU, 0x38000000, &local_E8, 0);
  }
  Script_PlayCharaMotion(CHARA_FUKU, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_CreateNPCImageModule(CHARA_AKIR, "YKC_M", 0);
  Script_FACE_LCG(CHARA_AKIR, 2, 0, 0);
  Script_FACE_LCG(CHARA_FUKU, 2, 0, 0);
  Script_seqEnterEvtCam(1011, CHARA_AKIR, CHARA_AKIR);
  Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_motmToggleFlags(CHARA_AKIR, 1, 0x40);
  {
    int32_t unused_E8[6];
    int32_t local_E4;
    local_E4 = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f);
    switch (local_E4) {
      case 0:
        Script_ShowUIMessageFromID("E1031A008");
        break;
      case 1:
        Script_ShowUIMessageFromID("E1031A009");
        break;
    }
  }
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  {
    int32_t unused_E8[6];
    int32_t local_E4;
    local_E4 = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f);
    switch (local_E4) {
      case 0:
        Script_ShowUIMessageFromID("E1031A010");
        break;
      case 1:
        Script_ShowUIMessageFromID("E1031A011");
        break;
    }
  }
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_SetCPAMFlags(CHARA_FUKU, 0);
  Script_seqEnterEvtCam(1012, CHARA_FUKU, CHARA_FUKU);
  Script_executeAicaCommand(2720, 2, 30);
  {
    int32_t unused_E8[6];
    int32_t local_E4;
    local_E4 = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f);
    switch (local_E4) {
      case 0:
        Script_ShowUIMessageFromID("E1031B014");
        break;
      case 1:
        Script_ShowUIMessageFromID("E1031B015");
        break;
    }
  }
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_charaDisplay(SHEN_NAME('KAWA'), CDM_Hide);
  Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_PlayCharaMotion(CHARA_FUKU, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_FACE_LCG(CHARA_AKIR, 0, 0, 0);
  Script_FACE_LCG(CHARA_FUKU, 0, 0, 0);
  SCNF_WaitFrames_0000(1);
  Script_AuthorUtils(AUTH_FREE, 0);
  Script_AuthorUtils(AUTH_FREE, 1);
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "KAKG");
  Script_LoadSND(0, 2, "bgm009.snd", 0);
  Script_executeAicaCommand(16296, 0, 0);
  Script_executeAicaCommand(1184, 2, 100);
  Script_ccowUtils(CHARA_AKIR, 1);
  Script_ccowUtils(CHARA_FUKU, 1);
  Script_convSetCamInactiveFlag(1);
  Script_SetGameStateTo(AUTH_None);
  Script_SetCPAMFlags(CHARA_FUKU, 1);
  Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
  Script_ryoControl(RYO_RestoreControl);
  Script_shenClearFreeState(1);
  task_228 = 0;
  script_FEDE();
  end:;
}

__attribute__((scn_string_bindings(1, data_29CB8)))
static void script_1A163(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t local_E4;
  int32_t local_E0;
  int32_t local_DC;
  local_E0 = 0;
  Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  local_E8.x = 0;
  (&local_E8)->y = 0;
  (&local_E8)->z = 0;
  Script_EVT_SetRotation(SHEN_NAME('DDO1'), 0x38000000, &local_E8, 0);
  local_E8.x = 0;
  (&local_E8)->y = 0;
  (&local_E8)->z = 0;
  Script_EVT_SetRotation(SHEN_NAME('DDO2'), 0x38000000, &local_E8, 0);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 15.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 12.0f);
  Script_FaceEx(CHARA_AKIR, 0, 0, 0);
  Script_LoadVoiceFile(1, "E1031");
  Script_motmToggleFlags(CHARA_FUKU, 2, 0x40);
  Script_PlayCharaMotion(CHARA_FUKU, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_motmToggleFlags(CHARA_AKIR, 2, 0x40);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_CreateNPCImageModule(CHARA_AKIR, "YKF_M", 0);
  local_E4 = Script_motiLoad(1, SHEN_NAME('JHD0'), "M_FGT1.BIN");
  Script_vfSetup(0, 20, 20, 0, 0);
  Script_vfStageObjectUtils(0, 0, 0);
  Script_vfStageObjectUtils(2, 0, 2, 2, 0x3F000000, 0x3F000000, -0x40800000, 0, 0, -0x40800000);
  Script_vfStageObjectUtils(7, 0, 0x4F4B4148, 0);
  Script_vfStageObjectUtils(0, 1, 0);
  Script_vfStageObjectUtils(2, 1, 2, 2, 0x3F800000, 0x3E800000, -0x40000000, 0, 0, -0x41000000);
  Script_vfStageObjectUtils(7, 1, 0x534B484B, 1);
  Script_vfStageObjectUtils(13);
  task_2AC = 0x52494B41;
  *(&task_2AC + 1) = 0;
  *(&task_2AC + 2) = 0;
  *(&task_2AC + 3) = 0;
  *(&task_2AC + 4) = 400;
  *(&task_2AC + 5) = 0;
  *(&task_2AC + 6) = 0;
  *(&task_2AC + 7) = 0;
  *(&task_2AC + 8) = 0;
  *(&task_2AC + 9) = 0;
  *(&task_2AC + 10) = 0;
  Script_vfUtils(&task_2AC);
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_TATQ_FOLLOWUP_COMPLETE) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_SA1077_INE_HANDOFF_DONE) == 0)) {
    task_2AC = 0x554B5546;
    *(&task_2AC + 1) = 0;
    *(&task_2AC + 2) = 1;
    *(&task_2AC + 3) = 1;
    *(&task_2AC + 4) = 400;
    *(&task_2AC + 5) = 12;
    *(&task_2AC + 6) = 0;
    *(&task_2AC + 7) = 0;
    *(&task_2AC + 8) = 0;
    *(&task_2AC + 9) = 0;
    *(&task_2AC + 10) = 0;
    Script_vfUtils(&task_2AC);
    Script_vfConfigureFormationGroup(CHARA_FUKU, VF_FORMATION_APPLY_FLAGS, 0x200000, 0x800);
  } else {
    task_2AC = 0x554B5546;
    *(&task_2AC + 1) = 0;
    *(&task_2AC + 2) = 1;
    *(&task_2AC + 3) = 1;
    *(&task_2AC + 4) = 400;
    *(&task_2AC + 5) = 57;
    *(&task_2AC + 6) = 0;
    *(&task_2AC + 7) = 0;
    *(&task_2AC + 8) = 0;
    *(&task_2AC + 9) = 0;
    *(&task_2AC + 10) = 0;
    Script_vfUtils(&task_2AC);
    Script_vfConfigureFormationGroup(CHARA_FUKU, VF_FORMATION_APPLY_FLAGS, 0x200000, 0x800);
  }
  SCNF_WaitFrames_0000(1);
  local_F4.x = -7.0f;
  (&local_F4)->y = 1.0f;
  (&local_F4)->z = -20.4f;
  local_E8.x = 0;
  (&local_E8)->y = 0;
  (&local_E8)->z = 0;
  Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &local_F4);
  Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &local_E8, 0);
  local_F4.x = -7.0f;
  (&local_F4)->y = 1.0f;
  (&local_F4)->z = -21.6f;
  local_E8.x = 0;
  (&local_E8)->y = 0x8000;
  (&local_E8)->z = 0;
  Script_EVT_SetPosition(CHARA_FUKU, POS_XYZ, &local_F4);
  Script_EVT_SetRotation(CHARA_FUKU, 0x38000000, &local_E8, 0);
  Script_seqEnterEvtCam(1023, 0, AUTH_None);
  Script_filmCheckOrSetTransitionUtil(15, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_motmToggleFlags(CHARA_FUKU, 1, 0x40);
  Script_motmToggleFlags(CHARA_AKIR, 1, 0x40);
  Script_seqEnterEvtCam(1010, 0, AUTH_None);
  SCNF_WaitFrames_0000(1);
  Script_charaToggleMotEnabled(CHARA_AKIR, 1);
  Script_charaToggleMotEnabled(CHARA_FUKU, 1);
  Script_PlayCharaMotion(CHARA_FUKU, MN_AKI_AKI_REI_KAMAE_JYU, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  SCNF_WaitFrames_0000(6);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_REI_KAMAE_JYU, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  for (;;) {
    if (Script_IsCharaMotionFinished(CHARA_AKIR)) {
      break;
    }
    local_E0 = local_E0 + 1;
    if (local_E0 == 15) {
      local_DC = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f);
      switch (local_DC) {
        case 0:
          Script_ShowUIMessageFromID("E1031B009");
          break;
        case 1:
          Script_ShowUIMessageFromID("E1031B007");
          break;
        case 2:
          Script_ShowUIMessageFromID("E1031B008");
          break;
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  for (;;) {
    if (Script_IsInEventCam() == 0) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_vfConfigureFormationGroup(CHARA_AKIR, VF_FORMATION_APPLY_FLAGS, VF_SfxEventPending, 100);
  Script_vfConfigureFormationGroup(CHARA_FUKU, VF_FORMATION_APPLY_FLAGS, VF_SfxEventPending, 100);
  SCN_Enqueue_VFGI(2, 99, 1.0f, 2, 240);
  while (SCN_GetIsFighting() != 2) {
    if (Script_CheckTaskStatus(CHARA_AKIR) & 1) {
      SCNF_WaitFrames_0000(1);
      Script_vfToggleBattleState(scnPostResultsSetup);
      break;
    }
    if (Script_CheckTaskStatus(CHARA_FUKU) & 1) {
      SCNF_WaitFrames_0000(1);
      Script_vfToggleBattleState(scnPostResultsSetup);
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
  Script_executeAicaCommand(2720, 2, 30);
  while (SCN_GetIsFighting() != 2) {
    SCNF_WaitFrames_0000(1);
  }
  Script_ReallocateUNFO(local_E4);
  Script_motmToggleFlags(CHARA_AKIR, 2, 0x80);
  Script_motmToggleFlags(CHARA_FUKU, 2, 0x80);
  Script_charaToggleMotEnabled(CHARA_AKIR, 1);
  Script_charaToggleMotEnabled(CHARA_FUKU, 1);
  Script_GetFightingStats(0x52494B41, 0);
  Script_CharHandlers(CHARA_AKIR, -1, 0, 0);
  local_E8.x = 0;
  (&local_E8)->y = 17560;
  (&local_E8)->z = 0;
  Script_EVT_SetRotation(SHEN_NAME('DDO1'), 0x38000000, &local_E8, 0);
  local_E8.x = 0;
  (&local_E8)->y = 46514;
  (&local_E8)->z = 0;
  Script_EVT_SetRotation(SHEN_NAME('DDO2'), 0x38000000, &local_E8, 0);
  return;
}

static void script_1A86E(void) {
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_WALK_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_LineWalkUtils(LNWK_SCRIPT_BEGIN_PATH_WALK, CHARA_AKIR, &data_29CE8, 0, 0);
  while (Script_LineWalkUtils(LNWK_SCRIPT_SET_MOVE_SCALE_GET_CMD, CHARA_AKIR, 0.05f) != -1) {
    SCNF_WaitFrames_0000(1);
  }
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  return;
}

static void script_1A8F1(void) {
  if (-1) {
    Script_charaDisplay(SHEN_NAME('KOI0'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('KOI0'), CDM_Show);
      goto L01A91B;
    }
  }
  L01A91B:;
  Script_scenToggleVisibleFor(SHEN_NAME('KOI0'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('KOI1'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('KOI1'), CDM_Show);
      goto L01A94F;
    }
  }
  L01A94F:;
  Script_scenToggleVisibleFor(SHEN_NAME('KOI1'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('KOI2'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('KOI2'), CDM_Show);
      goto L01A983;
    }
  }
  L01A983:;
  Script_scenToggleVisibleFor(SHEN_NAME('KOI2'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('KOI3'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('KOI3'), CDM_Show);
      goto L01A9B7;
    }
  }
  L01A9B7:;
  Script_scenToggleVisibleFor(SHEN_NAME('KOI3'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('KOI4'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('KOI4'), CDM_Show);
      goto L01A9EB;
    }
  }
  L01A9EB:;
  Script_scenToggleVisibleFor(SHEN_NAME('KOI4'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('HAS0'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('HAS0'), CDM_Show);
      goto L01AA1F;
    }
  }
  L01AA1F:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAS0'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('HAS1'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('HAS1'), CDM_Show);
      goto L01AA53;
    }
  }
  L01AA53:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAS1'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('HAS2'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('HAS2'), CDM_Show);
      goto L01AA87;
    }
  }
  L01AA87:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAS2'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('HAS3'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('HAS3'), CDM_Show);
      goto L01AABB;
    }
  }
  L01AABB:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAS3'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('HAS4'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('HAS4'), CDM_Show);
      goto L01AAEF;
    }
  }
  L01AAEF:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAS4'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('HAS5'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('HAS5'), CDM_Show);
      goto L01AB23;
    }
  }
  L01AB23:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAS5'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('HAS6'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('HAS6'), CDM_Show);
      goto L01AB57;
    }
  }
  L01AB57:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAS6'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('HAS7'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('HAS7'), CDM_Show);
      goto L01AB8B;
    }
  }
  L01AB8B:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAS7'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('HAS8'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('HAS8'), CDM_Show);
      goto L01ABBF;
    }
  }
  L01ABBF:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAS8'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('HAS9'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('HAS9'), CDM_Show);
      goto L01ABF3;
    }
  }
  L01ABF3:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAS9'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('HASA'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('HASA'), CDM_Show);
      goto L01AC27;
    }
  }
  L01AC27:;
  Script_scenToggleVisibleFor(SHEN_NAME('HASA'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('HASB'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('HASB'), CDM_Show);
      goto L01AC5B;
    }
  }
  L01AC5B:;
  Script_scenToggleVisibleFor(SHEN_NAME('HASB'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('HASC'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('HASC'), CDM_Show);
      goto L01AC8F;
    }
  }
  L01AC8F:;
  Script_scenToggleVisibleFor(SHEN_NAME('HASC'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('HASD'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('HASD'), CDM_Show);
      goto L01ACC3;
    }
  }
  L01ACC3:;
  Script_scenToggleVisibleFor(SHEN_NAME('HASD'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('HASE'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('HASE'), CDM_Show);
      goto L01ACF7;
    }
  }
  L01ACF7:;
  Script_scenToggleVisibleFor(SHEN_NAME('HASE'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('HASF'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('HASF'), CDM_Show);
      goto L01AD2B;
    }
  }
  L01AD2B:;
  Script_scenToggleVisibleFor(SHEN_NAME('HASF'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('HAN0'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('HAN0'), CDM_Show);
      goto L01AD5F;
    }
  }
  L01AD5F:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAN0'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('HAN1'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('HAN1'), CDM_Show);
      goto L01AD93;
    }
  }
  L01AD93:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAN1'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('HAN2'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('HAN2'), CDM_Show);
      goto L01ADC7;
    }
  }
  L01ADC7:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAN2'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('HAN3'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('HAN3'), CDM_Show);
      goto L01ADFB;
    }
  }
  L01ADFB:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAN3'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('HAN4'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('HAN4'), CDM_Show);
      goto L01AE2F;
    }
  }
  L01AE2F:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAN4'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('SSOD'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('SSOD'), CDM_Show);
      goto L01AE63;
    }
  }
  L01AE63:;
  Script_scenToggleVisibleFor(SHEN_NAME('SSOD'), 0);
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, 0x5F454E49)) {
    if (-1) {
      Script_charaDisplay(CHARA_INE_, CDM_Hide);
    } else {
      if (0) {
        Script_charaDisplay(CHARA_INE_, CDM_Show);
        goto L01AEA6;
      }
    }
    L01AEA6:;
    Script_scenToggleVisibleFor(CHARA_INE_, 0);
  }
  if (-1) {
    Script_charaDisplay(SHEN_NAME('TUBA'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('TUBA'), CDM_Show);
      goto L01AEDA;
    }
  }
  L01AEDA:;
  Script_scenToggleVisibleFor(SHEN_NAME('TUBA'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('KAK3'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('KAK3'), CDM_Show);
      goto L01AF0E;
    }
  }
  L01AF0E:;
  Script_scenToggleVisibleFor(SHEN_NAME('KAK3'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('KAK4'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('KAK4'), CDM_Show);
      goto L01AF42;
    }
  }
  L01AF42:;
  Script_scenToggleVisibleFor(SHEN_NAME('KAK4'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('HOKI'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('HOKI'), CDM_Show);
      goto L01AF76;
    }
  }
  L01AF76:;
  Script_scenToggleVisibleFor(SHEN_NAME('HOKI'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('MOON'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('MOON'), CDM_Show);
      goto L01AFAA;
    }
  }
  L01AFAA:;
  Script_scenToggleVisibleFor(SHEN_NAME('MOON'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('GRW1'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('GRW1'), CDM_Show);
      goto L01AFDE;
    }
  }
  L01AFDE:;
  Script_scenToggleVisibleFor(SHEN_NAME('GRW1'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('GRW2'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('GRW2'), CDM_Show);
      goto L01B012;
    }
  }
  L01B012:;
  Script_scenToggleVisibleFor(SHEN_NAME('GRW2'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('GRW3'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('GRW3'), CDM_Show);
      goto L01B046;
    }
  }
  L01B046:;
  Script_scenToggleVisibleFor(SHEN_NAME('GRW3'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('GRW4'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('GRW4'), CDM_Show);
      goto L01B07A;
    }
  }
  L01B07A:;
  Script_scenToggleVisibleFor(SHEN_NAME('GRW4'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('PONK'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('PONK'), CDM_Show);
      goto L01B0AE;
    }
  }
  L01B0AE:;
  Script_scenToggleVisibleFor(SHEN_NAME('PONK'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('PONS'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('PONS'), CDM_Show);
      goto L01B0E2;
    }
  }
  L01B0E2:;
  Script_scenToggleVisibleFor(SHEN_NAME('PONS'), 0);
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, 0x52554B59)) {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('YKUR'), CDM_Hide);
    } else {
      if (0) {
        Script_charaDisplay(SHEN_NAME('YKUR'), CDM_Show);
        goto L01B125;
      }
    }
    L01B125:;
    Script_scenToggleVisibleFor(SHEN_NAME('YKUR'), 0);
  }
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, 0x4C554B59)) {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('YKUL'), CDM_Hide);
    } else {
      if (0) {
        Script_charaDisplay(SHEN_NAME('YKUL'), CDM_Show);
        goto L01B168;
      }
    }
    L01B168:;
    Script_scenToggleVisibleFor(SHEN_NAME('YKUL'), 0);
  }
  return;
}

static void script_1B175(void) {
  if (0) {
    Script_charaDisplay(SHEN_NAME('KOI0'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('KOI0'), CDM_Show);
      goto L01B19F;
    }
  }
  L01B19F:;
  Script_scenToggleVisibleFor(SHEN_NAME('KOI0'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('KOI1'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('KOI1'), CDM_Show);
      goto L01B1D3;
    }
  }
  L01B1D3:;
  Script_scenToggleVisibleFor(SHEN_NAME('KOI1'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('KOI2'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('KOI2'), CDM_Show);
      goto L01B207;
    }
  }
  L01B207:;
  Script_scenToggleVisibleFor(SHEN_NAME('KOI2'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('KOI3'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('KOI3'), CDM_Show);
      goto L01B23B;
    }
  }
  L01B23B:;
  Script_scenToggleVisibleFor(SHEN_NAME('KOI3'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('KOI4'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('KOI4'), CDM_Show);
      goto L01B26F;
    }
  }
  L01B26F:;
  Script_scenToggleVisibleFor(SHEN_NAME('KOI4'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('HAS0'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('HAS0'), CDM_Show);
      goto L01B2A3;
    }
  }
  L01B2A3:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAS0'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('HAS1'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('HAS1'), CDM_Show);
      goto L01B2D7;
    }
  }
  L01B2D7:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAS1'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('HAS2'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('HAS2'), CDM_Show);
      goto L01B30B;
    }
  }
  L01B30B:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAS2'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('HAS3'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('HAS3'), CDM_Show);
      goto L01B33F;
    }
  }
  L01B33F:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAS3'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('HAS4'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('HAS4'), CDM_Show);
      goto L01B373;
    }
  }
  L01B373:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAS4'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('HAS5'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('HAS5'), CDM_Show);
      goto L01B3A7;
    }
  }
  L01B3A7:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAS5'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('HAS6'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('HAS6'), CDM_Show);
      goto L01B3DB;
    }
  }
  L01B3DB:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAS6'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('HAS7'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('HAS7'), CDM_Show);
      goto L01B40F;
    }
  }
  L01B40F:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAS7'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('HAS8'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('HAS8'), CDM_Show);
      goto L01B443;
    }
  }
  L01B443:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAS8'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('HAS9'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('HAS9'), CDM_Show);
      goto L01B477;
    }
  }
  L01B477:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAS9'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('HASA'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('HASA'), CDM_Show);
      goto L01B4AB;
    }
  }
  L01B4AB:;
  Script_scenToggleVisibleFor(SHEN_NAME('HASA'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('HASB'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('HASB'), CDM_Show);
      goto L01B4DF;
    }
  }
  L01B4DF:;
  Script_scenToggleVisibleFor(SHEN_NAME('HASB'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('HASC'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('HASC'), CDM_Show);
      goto L01B513;
    }
  }
  L01B513:;
  Script_scenToggleVisibleFor(SHEN_NAME('HASC'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('HASD'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('HASD'), CDM_Show);
      goto L01B547;
    }
  }
  L01B547:;
  Script_scenToggleVisibleFor(SHEN_NAME('HASD'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('HASE'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('HASE'), CDM_Show);
      goto L01B57B;
    }
  }
  L01B57B:;
  Script_scenToggleVisibleFor(SHEN_NAME('HASE'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('HASF'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('HASF'), CDM_Show);
      goto L01B5AF;
    }
  }
  L01B5AF:;
  Script_scenToggleVisibleFor(SHEN_NAME('HASF'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('HAN0'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('HAN0'), CDM_Show);
      goto L01B5E3;
    }
  }
  L01B5E3:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAN0'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('HAN1'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('HAN1'), CDM_Show);
      goto L01B617;
    }
  }
  L01B617:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAN1'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('HAN2'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('HAN2'), CDM_Show);
      goto L01B64B;
    }
  }
  L01B64B:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAN2'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('HAN3'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('HAN3'), CDM_Show);
      goto L01B67F;
    }
  }
  L01B67F:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAN3'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('HAN4'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('HAN4'), CDM_Show);
      goto L01B6B3;
    }
  }
  L01B6B3:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAN4'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('SSOD'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('SSOD'), CDM_Show);
      goto L01B6E7;
    }
  }
  L01B6E7:;
  Script_scenToggleVisibleFor(SHEN_NAME('SSOD'), 1);
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, 0x5F454E49)) {
    Script_scenToggleVisibleFor(CHARA_INE_, 1);
  }
  Script_scenToggleVisibleFor(SHEN_NAME('TUBA'), 1);
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_KAK1_HANGING_SCROLL_TAKEN)) {
    if (0) {
      Script_charaDisplay(SHEN_NAME('KAK3'), CDM_Hide);
    } else {
      if (-1) {
        Script_charaDisplay(SHEN_NAME('KAK3'), CDM_Show);
        goto L01B74E;
      }
    }
    L01B74E:;
    Script_scenToggleVisibleFor(SHEN_NAME('KAK3'), 1);
  } else {
    Script_scenToggleVisibleFor(SHEN_NAME('KAK3'), 1);
  }
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_KAK2_HANGING_SCROLL_TAKEN)) {
    if (0) {
      Script_charaDisplay(SHEN_NAME('KAK4'), CDM_Hide);
    } else {
      if (-1) {
        Script_charaDisplay(SHEN_NAME('KAK4'), CDM_Show);
        goto L01B79D;
      }
    }
    L01B79D:;
    Script_scenToggleVisibleFor(SHEN_NAME('KAK4'), 1);
  } else {
    Script_scenToggleVisibleFor(SHEN_NAME('KAK4'), 1);
  }
  Script_scenToggleVisibleFor(SHEN_NAME('HOKI'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('GRW1'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('GRW1'), CDM_Show);
      goto L01B7EC;
    }
  }
  L01B7EC:;
  Script_scenToggleVisibleFor(SHEN_NAME('GRW1'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('GRW2'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('GRW2'), CDM_Show);
      goto L01B820;
    }
  }
  L01B820:;
  Script_scenToggleVisibleFor(SHEN_NAME('GRW2'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('GRW3'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('GRW3'), CDM_Show);
      goto L01B854;
    }
  }
  L01B854:;
  Script_scenToggleVisibleFor(SHEN_NAME('GRW3'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('GRW4'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('GRW4'), CDM_Show);
      goto L01B888;
    }
  }
  L01B888:;
  Script_scenToggleVisibleFor(SHEN_NAME('GRW4'), 1);
  Script_scenToggleVisibleFor(SHEN_NAME('PONK'), 1);
  Script_scenToggleVisibleFor(SHEN_NAME('PONS'), 1);
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, 0x52554B59)) {
    Script_scenToggleVisibleFor(SHEN_NAME('YKUR'), 1);
  }
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, 0x4C554B59)) {
    Script_scenToggleVisibleFor(SHEN_NAME('YKUL'), 1);
  }
  return;
}

static void script_1B8E3(int32_t arg0, int32_t arg1) {
  if (task_3B8 != arg1) {
    script_24486(arg1, arg0, arg0);
  }
  SCNF_WaitFrames_0000(2);
  for (;;) {
    script_244B4();
    script_24486(arg1, arg0, arg0);
    SCNF_WaitFrames_0000(2);
  }
  return;
}

static int32_t script_1B922(int32_t arg0, int32_t arg1, int32_t arg2) {
  Vector3 local_F4;
  if (arg2) {
    script_24548();
  }
  Script_EVT_GetPosition(arg1, 5, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  Script_SetCharaMotionPlaybackRate(arg0, 0.5f);
  SCNF_CHAR_SCNFApplyFacingTowardPoint(arg0, &local_F4, data_29CFC, 0.0f, 0, 0x200000);
  script_2449A(arg0);
  Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
  if (arg2 == 0) {
    script_24548();
  }
  Script_PlayCharaMotion(arg0, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  script_2449A(arg0);
  return Script_srchControl(0);
}

static void script_1B9B2(int32_t arg0, int32_t arg1) {
  if (arg1) {
    Script_ResetHandMotion(arg0, 1, 1, 0);
    Script_ResetHandMotion(arg0, 1, 0, 0);
  } else {
    Script_ResetHandMotion(arg0, 0, 1, 0);
    Script_ResetHandMotion(arg0, 0, 0, 0);
  }
  return;
}

static int32_t script_1B9F2(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Vector3 local_F4;
  if (arg2) {
    script_24548();
  }
  Script_SetCharaLookAtPosition(arg0, -1, 0, 0);
  script_2449A(arg0);
  Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_15);
  Script_PlayCharaMotion(arg0, MN_AKI_SIKKARI_HENJI_IIE, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  script_2449A(arg0);
  Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_30);
  Script_PlayCharaMotion(arg0, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  script_2449A(arg0);
  if (arg3) {
    Script_EVT_GetPosition(arg1, 5, &local_F4, ENTITY_XFORM_WORLD_SPACE);
    Script_SetLookPointDetails(arg0, 1, &local_F4, 0);
    script_2449A(arg0);
  }
  Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_5);
  if (arg2 == 0) {
    script_24548();
  }
  return Script_srchControl(0);
}

static void script_1BABF(int32_t arg0, int32_t arg1) {
  Vector3 local_F4;
  Script_SetCharaMotionPlaybackRate(arg0, 2.0f);
  Script_SetCharaMotionPlaybackRate(arg1, 2.0f);
  Script_EVT_GetPosition(arg0, 5, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  SCNF_CHAR_SCNFApplyFacingTowardPoint(arg1, &local_F4, 0, 0.0f, 0, 0);
  script_2449A(arg1);
  Script_EVT_GetPosition(arg1, 5, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  SCNF_CHAR_SCNFApplyFacingTowardPoint(arg0, &local_F4, 0, 0.0f, 0, 0);
  script_2449A(arg0);
  Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
  Script_SetCharaMotionPlaybackRate(arg1, 1.0f);
  script_24548();
  Script_srchControl(0);
  return;
}

static int32_t script_1BB4D(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  Vector3 local_F4;
  if (arg3) {
    script_24548();
  }
  Script_EVT_GetPosition(arg1, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  Script_SetLookPointDetails(arg0, 12, &local_F4, 0);
  SCNF_WaitFrames_0000(arg2);
  if (arg4) {
    Script_EVT_GetPosition(arg1, 5, &local_F4, ENTITY_XFORM_WORLD_SPACE);
    Script_SetLookPointDetails(arg0, 11, &local_F4, 0);
  }
  if (arg3 == 0) {
    script_24548();
  }
  return Script_srchControl(0);
}

static int32_t script_1BBB5(int32_t arg0, int32_t arg1, int32_t arg2) {
  Vector3 local_F4;
  if (arg2) {
    script_24548();
  }
  Script_EVT_GetPosition(arg1, 5, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  Script_SetLookPointDetails(arg0, 2, &local_F4, 0);
  script_2449A(arg0);
  if (arg2 == 0) {
    script_24548();
  }
  return Script_srchControl(0);
}

static void script_1BBFC(int32_t arg0, int32_t arg1, int32_t arg2) {
  Script_SetCharaLookAtPosition(arg0, -1, 0, 0);
  if (arg2) {
    script_24548();
  }
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_OJIGI_LIGHT_ST, -1.0f, 15.0f, -1.0f, 0.4f, 0);
  script_2449A(arg0);
  SCNF_WaitFrames_0000(10);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_OJIGI_LIGHT_EN, -1.0f, 29.0f, 17.0f, 0.3f, 0);
  script_2449A(arg0);
  if (arg2 == 0) {
    script_24548();
  }
  Script_srchControl(0);
  return;
}

static int32_t script_1BC88(int32_t arg0, int32_t arg1, int32_t arg2) {
  Vector3 local_F4;
  int32_t local_F0;
  task_324 = 1;
  local_F0 = 0;
  for (;;) {
    Script_EVT_GetPosition(arg1, 5, &local_F4, ENTITY_XFORM_WORLD_SPACE);
    Script_SetLookPointDetails(arg0, 11, &local_F4, 0);
    if (arg2 != -1) {
      local_F0 = local_F0 + 1;
      if (local_F0 > arg2) {
        break;
      }
    }
    if (task_324 == 0) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_1BCEA(int32_t arg0) {
  int32_t local_FC;
  script_D9C0();
  script_1C54E();
  Script_shenSeqUtils(arg0);
  local_FC = arg0;
  switch (local_FC) {
    case 9:
      script_1BD8B();
      break;
    case 10:
      script_1BEAD();
      break;
    case 11:
      script_1C036();
      break;
    case 12:
      script_1C0A0();
      break;
    case 15:
      script_1C165();
      break;
    case 13:
      script_1C1F7();
      break;
    case 16:
      script_1C2CB();
      break;
    case 14:
      script_1C3F3();
      break;
  }
  return;
}

static void script_1BD8B(void) {
  int32_t local_FC;
  int32_t unused_F8;
  int32_t local_F4;
  local_FC = 0;
  Script_handSetFloatsFor(CHARA_FUKU, 0, data_2710C, 0);
  Script_handSetFloatsFor(CHARA_FUKU, 1, data_2710C, 8);
  while (Script_shenSeqUtils(-1)) {
    local_F4 = local_FC;
    switch (local_F4) {
      case 170:
        Script_handSetFloatsFor(CHARA_FUKU, 0, data_27664, 16);
        Script_handSetFloatsFor(CHARA_FUKU, 1, data_27664, 16);
        break;
      case 220:
        Script_handSetFloatsFor(CHARA_FUKU, 0, data_2710C, 16);
        break;
      case 300:
        Script_handSetFloatsFor(CHARA_FUKU, 1, data_2710C, 8);
        break;
      case 450:
        Script_handSetFloatsFor(CHARA_FUKU, 0, data_27664, 16);
        Script_handSetFloatsFor(CHARA_FUKU, 1, data_27664, 16);
        break;
      case 460:
        script_1C679(CHARA_FUKU);
        break;
      case 580:
        script_1C63E(CHARA_FUKU);
        break;
    }
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_1BEAD(void) {
  int32_t local_FC;
  int32_t unused_F8;
  int32_t local_F4;
  local_FC = 0;
  while (Script_shenSeqUtils(-1)) {
    local_F4 = local_FC;
    switch (local_F4) {
      case 100:
        Script_FACE(CHARA_AKIR, 1, 0, 16);
        break;
      case 150:
        Script_FACE(CHARA_AKIR, 0, 0, 16);
        break;
      case 410:
        Script_handSetFloatsFor(CHARA_FUKU, 0, data_2710C, 16);
        break;
      case 476:
        Script_handSetFloatsFor(CHARA_FUKU, 1, data_2710C, 8);
        break;
      case 607:
        Script_handSetFloatsFor(CHARA_FUKU, 0, data_27664, 8);
        Script_handSetFloatsFor(CHARA_FUKU, 1, data_27664, 8);
        break;
      case 630:
        Script_FACE(CHARA_AKIR, 8, 1, 16);
        Script_FACE(CHARA_FUKU, 4, 0, 16);
        break;
      case 635:
        Script_FACE(CHARA_AKIR, 8, 0, 16);
        break;
      case 660:
        Script_FACE(CHARA_AKIR, 0, 0, 16);
        break;
      case 750:
        Script_FACE(CHARA_FUKU, 0, 0, 16);
        break;
      case 1020:
        Script_handSetFloatsFor(CHARA_AKIR, 0, data_2710C, 16);
        Script_handSetFloatsFor(CHARA_AKIR, 1, data_2710C, 16);
        break;
    }
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_1C036(void) {
  int32_t local_FC;
  int32_t unused_F8;
  int32_t local_F4;
  local_FC = 0;
  while (Script_shenSeqUtils(-1)) {
    local_F4 = local_FC;
    switch (local_F4) {
      case 530:
        Script_handSetFloatsFor(CHARA_FUKU, 0, data_2710C, 16);
        break;
      case 590:
        Script_handSetFloatsFor(CHARA_FUKU, 1, data_2710C, 8);
        break;
    }
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_1C0A0(void) {
  int32_t local_FC;
  int32_t unused_F8;
  Vector3 local_EC;
  int32_t local_E8;
  local_FC = 0;
  Script_handSetFloatsFor(CHARA_AKIR, 1, data_2710C, 0);
  Script_handSetFloatsFor(CHARA_AKIR, 0, data_2710C, 0);
  while (Script_shenSeqUtils(-1)) {
    local_E8 = local_FC;
    if (local_E8 == 90) {
      Script_handSetFloatsFor(CHARA_AKIR, 1, data_27664, 32);
      Script_handSetFloatsFor(CHARA_AKIR, 0, data_27664, 32);
      goto L01C10B;
    }
    L01C10B:;
    if (!(-(local_FC >= 130) & -(local_FC <= 225))) goto L01C11C;
    L01C11C:;
    Script_EVT_GetPosition(CHARA_AKIR, 5, &local_EC, 0);
    Script_SetLookPointDetails(CHARA_FUKU, 98312, &local_EC, 0);
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  Script_SetCharaLookAtPosition(CHARA_FUKU, -98312, 0, 0);
  return;
}

static void script_1C165(void) {
  int32_t local_FC;
  int32_t unused_EC[4];
  int32_t local_E8;
  local_FC = 0;
  Script_handSetFloatsFor(CHARA_AKIR, 1, data_2710C, 0);
  Script_handSetFloatsFor(CHARA_AKIR, 0, data_2710C, 0);
  while (Script_shenSeqUtils(-1)) {
    local_E8 = local_FC;
    if (local_E8 == 90) {
      Script_handSetFloatsFor(CHARA_AKIR, 1, data_27664, 32);
      Script_handSetFloatsFor(CHARA_AKIR, 0, data_27664, 32);
      goto L01C1D0;
    }
    L01C1D0:;
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  Script_SetCharaLookAtPosition(CHARA_FUKU, -98312, 0, 0);
  return;
}

static void script_1C1F7(void) {
  int32_t local_FC;
  int32_t unused_F8;
  int32_t local_F4;
  local_FC = 0;
  while (Script_shenSeqUtils(-1)) {
    local_F4 = local_FC;
    switch (local_F4) {
      case 100:
        Script_FACE(CHARA_FUKU, 3, 0, 16);
        break;
      case 200:
        Script_FACE(CHARA_FUKU, 0, 0, 32);
        break;
      case 260:
        Script_handSetFloatsFor(CHARA_AKIR, 0, data_2710C, 16);
        break;
      case 380:
        Script_handSetFloatsFor(CHARA_AKIR, 0, data_27664, 32);
        break;
      case 700:
        Script_handSetFloatsFor(CHARA_FUKU, 0, data_2710C, 16);
        Script_handSetFloatsFor(CHARA_FUKU, 1, data_2710C, 16);
        break;
    }
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_1C2CB(void) {
  int32_t local_FC;
  int32_t unused_EC[4];
  int32_t local_E8;
  local_FC = 0;
  Script_handSetFloatsFor(CHARA_AKIR, 0, data_2710C, 0);
  Script_handSetFloatsFor(CHARA_AKIR, 1, data_2710C, 0);
  while (Script_shenSeqUtils(-1)) {
    local_E8 = local_FC;
    switch (local_E8) {
      case 60:
        Script_handSetFloatsFor(CHARA_AKIR, 0, data_27664, 16);
        Script_handSetFloatsFor(CHARA_AKIR, 1, data_27664, 16);
        break;
      case 100:
        Script_FACE(CHARA_FUKU, 3, 0, 16);
        break;
      case 200:
        Script_FACE(CHARA_FUKU, 0, 0, 32);
        break;
      case 260:
        Script_handSetFloatsFor(CHARA_AKIR, 0, data_2710C, 16);
        break;
      case 380:
        Script_handSetFloatsFor(CHARA_AKIR, 0, data_27664, 32);
        break;
      case 720:
        Script_handSetFloatsFor(CHARA_FUKU, 0, data_2710C, 16);
        Script_handSetFloatsFor(CHARA_FUKU, 1, data_2710C, 8);
        break;
    }
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_1C3F3(void) {
  int32_t local_FC;
  int32_t unused_F8;
  Vector3i local_EC;
  int32_t local_E8;
  local_FC = 0;
  Script_handSetFloatsFor(CHARA_AKIR, 0, data_2710C, 0);
  Script_handSetFloatsFor(CHARA_AKIR, 1, data_2710C, 0);
  while (Script_shenSeqUtils(-1)) {
    local_E8 = local_FC;
    switch (local_E8) {
      case 60:
        Script_handSetFloatsFor(CHARA_AKIR, 0, data_27664, 16);
        Script_handSetFloatsFor(CHARA_AKIR, 1, data_27664, 16);
        break;
      case 148:
        local_EC.x = -3822;
        (&local_EC)->y = 5643;
        (&local_EC)->z = 0;
        Script_SetHandRootRotation(CHARA_AKIR, 1, 0x15, &local_EC);
        break;
      case 280:
        Script_handSetFloatsFor(CHARA_AKIR, 1, data_2710C, 8);
        break;
      case 314:
        Script_handSetFloatsFor(CHARA_AKIR, 1, data_27664, 16);
        break;
      case 329:
        local_EC.x = 0;
        (&local_EC)->y = 0;
        (&local_EC)->z = 0;
        Script_SetHandRootRotation(CHARA_AKIR, 1, 0x15, &local_EC);
        break;
      case 470:
        Script_handSetFloatsFor(CHARA_AKIR, 1, data_2710C, 16);
        Script_handSetFloatsFor(CHARA_AKIR, 0, data_2710C, 16);
        break;
    }
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_1C54E(void) {
  script_D9D8(CHARA_AKIR);
  script_D9D8(CHARA_FUKU);
  Script_facemotUtils(CHARA_AKIR, 1, 0);
  Script_facemotUtils(CHARA_FUKU, 1, 0);
  script_1C63E(CHARA_AKIR);
  script_1C63E(CHARA_FUKU);
  Script_SetCharaFlags(CHARA_AKIR, 4);
  Script_SetCharaFlags(CHARA_FUKU, 4);
  Script_SetMotnFlag(CHARA_AKIR, -1);
  Script_SetMotnFlag(CHARA_FUKU, -1);
  return;
}

static void script_1C5CB(void) {
  Script_SetMotnFlag(CHARA_AKIR, 0);
  Script_SetMotnFlag(CHARA_FUKU, 0);
  script_1C679(CHARA_AKIR);
  script_1C679(CHARA_FUKU);
  script_DA65(CHARA_AKIR);
  script_DA65(CHARA_FUKU);
  Script_facemotUtils(CHARA_AKIR, 0, 0);
  Script_FaceEx(CHARA_AKIR, 1, 0, 2);
  Script_SetCharaFlags(CHARA_AKIR, 4);
  return;
}

static void script_1C63E(int32_t arg0) {
  Script_ResetHandMotion(arg0, 1, 1, 0);
  Script_ResetHandMotion(arg0, 1, 0, 0);
  Script_handSetFloatsFor(arg0, 1, data_27664, 8);
  Script_handSetFloatsFor(arg0, 0, data_27664, 8);
  return;
}

static void script_1C679(int32_t arg0) {
  Script_ResetHandMotion(arg0, 0, 1, 0);
  Script_ResetHandMotion(arg0, 0, 0, 0);
  Script_CharHandlers(arg0, 2, 8, 16);
  return;
}

__attribute__((scn_string_bindings(1, data_29F12, 2, data_29F18, 3, data_29F22, 4, data_29F32, 5, data_29F37, 6, data_29F3C, 8, data_29F4E, 10, data_29F60, 12, data_29F72, 14, data_29F84, 16, data_29F96, 18, data_29FA8, 20, data_29FBA, 22, data_29FCC, 23, data_29FD1, 26, data_29FEE)))
static void script_1C6A3(void) {
  int32_t unused_FC;
  GameDateTime local_F4;
  Vector3 local_E8;
  Vector3 local_DC;
  int32_t unused_D8;
  GameDateTime local_D0;
  int8_t local_CF;
  int8_t local_CE;
  uint8_t unused_CC[2];
  local_E8.x = -1.5f;
  (&local_E8)->y = 0.0f;
  (&local_E8)->z = -16.0f;
  local_DC.x = 12.0f;
  (&local_DC)->y = 0.0f;
  (&local_DC)->z = -30.0f;
  for (;;) {
    Script_ReturnTimeDate(&local_F4);
    if ((&local_F4)->hours < 16) {
      if (Script_SetTaskDataValue(CHARA_AKIR, &local_E8, &local_DC, 0)) {
        if (task_22B == 0) {
          task_22B = 1;
          break;
        }
      } else {
        if (task_22B) {
          task_22B = 0;
        }
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  while (Script_IsPlayerAllowed(0) == 0) {
    SCNF_WaitFrames_0000(1);
  }
  task_228 = 1;
  Script_sprintf("<<< WazaDenjyu Start >>>\012");
  Script_scenUtils(SCENARIO_CMD_INIT_SCNT);
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_shenClearFreeState(0);
  Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  SCNF_WaitFrames_0000(10);
  Script_ReturnTimeDate(&local_D0);
  if ((&local_D0)->month == 12) {
    local_CE = 0;
  } else {
    local_CE = (&local_D0)->month;
  }
  local_CF = (local_CE << 5) | (&local_D0)->day;
  Script_scenUtils(SCENARIO_CMD_SET_STEP_LUT, 28, local_CF);
  Script_CreateNPCImageModule(CHARA_AKIR, "YKE_M", 0);
  script_1A8F1();
  Script_GetBlurModel(CHARA_AKIR, "YGB_B.MT5");
  Script_MT5Loader_TRWV_TRWAVE(22, task_31C, 3, 1);
  Script_shenClearFreeState(0);
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, "/scene/01/JHD0/", "JHW0") == 0) {
    SCNF_WaitFrames_0000(1);
  }
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
  if (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 3, "JHW0") == 0) {
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 2, "JHW0");
  }
  Script_AuthorUtils(0, 9, "seqdata1.bin", "JHW0");
  Script_AuthorUtils(0, 10, "seqdata2.bin", "JHW0");
  Script_AuthorUtils(0, 11, "seqdata3.bin", "JHW0");
  Script_AuthorUtils(0, 12, "seqdata4.bin", "JHW0");
  Script_AuthorUtils(0, 13, "seqdata5.bin", "JHW0");
  Script_AuthorUtils(0, 14, "seqdata6.bin", "JHW0");
  Script_AuthorUtils(0, 15, "seqdata7.bin", "JHW0");
  Script_AuthorUtils(0, 16, "seqdata8.bin", "JHW0");
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
  Script_executeAicaCommand(131232, 0, 0);
  Script_ryoSetChara(CHARA_AKIR);
  task_368 = Script_GetCurrentEntryPoint();
  Script_LoadVoiceFile(1, "E1017");
  Script_charaDisplay(CHARA_AKIR, CDM_Show);
  Script_LoadSND(0, 2, "bgm099.snd", 0);
  Script_LoadSND(0, 4, "a1_fukw.snd", 0);
  Script_LoadSND(0, 5, "battle_1.snd", 0);
  Script_uiUtils(0, 14, 0);
  Script_SetActionSelectorHUDElementVisibility(1);
  Script_SetupLightFromTable(0);
  Script_SetCharaFlags(CHARA_AKIR, 0);
  Script_SetCharaFlags(CHARA_FUKU, 0);
  Script_PREP_Related(0, 1);
  Script_PREP_Related(1, 0);
  Script_PREP_Related(2, 0);
  Script_PREP_Related(3, 0);
  task_338 = Script_GetCurrentDrawMode(0, 1);
  task_33C = Script_GetCurrentDrawMode(1, 1);
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 3);
  Script_charaSetRefboardFlagTo(CHARA_FUKU, 3);
  Script_SetVector4ByOffset(&data_29EE8);
  Script_SetCharaRefBoardPos(CHARA_AKIR, &data_29EE8);
  Script_SetCharaRefBoardPos(CHARA_FUKU, &data_29EE8);
  (void)1;
  Script_SetShadowFlags(0, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 4);
  Script_ccowSetFlag80(CHARA_AKIR, 1);
  Script_ccowSetFlag80(CHARA_FUKU, 1);
  Script_scenToggleActiveFor(CHARA_AKIR, 1);
  Script_scenToggleActiveFor(CHARA_FUKU, 1);
  Script_motmGetCharaFlags(CHARA_AKIR, 0, 0);
  Script_motmGetCharaFlags(CHARA_FUKU, 0, 0);
  Script_SetCharaMotionPlaybackRate(CHARA_AKIR, 1.0f);
  Script_SetCharaMotionPlaybackRate(CHARA_FUKU, 1.0f);
  Script_motmToggleFlags(CHARA_AKIR, 2, 0x40);
  Script_motmToggleFlags(CHARA_FUKU, 2, 0x40);
  Script_SetCPAMFlags(CHARA_AKIR, 0);
  Script_SetCPAMFlags(CHARA_FUKU, 0);
  Script_motmToggleFlags(CHARA_AKIR, 1, 0x40);
  Script_motmToggleFlags(CHARA_FUKU, 1, 0x40);
  Script_ccowUtils(CHARA_AKIR, 2);
  Script_facemotUtils(CHARA_AKIR, 0, 0);
  Script_facemotUtils(CHARA_AKIR, 1, 0);
  Script_charaDisplay(CHARA_AKIR, CDM_Show);
  Script_FaceEx(CHARA_AKIR, 1, 0, 2);
  Script_InitializeOsag(CHARA_AKIR);
  Script_charaToggleMotEnabled(CHARA_AKIR, 1);
  Script_ccowUtils(CHARA_FUKU, 2);
  Script_charaDisplay(CHARA_FUKU, CDM_Show);
  Script_facemotUtils(CHARA_FUKU, 0, 0);
  Script_facemotUtils(CHARA_FUKU, 1, 0);
  Script_FaceEx(CHARA_FUKU, 1, 0, 2);
  Script_charaDisplay(CHARA_FUKU, CDM_Show);
  Script_InitializeOsag(CHARA_FUKU);
  Script_charaToggleMotEnabled(CHARA_FUKU, 1);
  Script_SetCameraFlags(0);
  Script_CycleManagerUtils(CYCLE_CMD_SET_CYCLE_MANAGER_ENABLED, 0);
  return;
}

__attribute__((scn_string_bindings(0, data_29FFB, 1, data_2A007, 2, data_2A012, 3, data_2A01F, 4, data_2A02C, 5, data_2A032, 6, data_2A037, 7, data_2A03D)))
static void script_1CBC9(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  Script_CycleManagerUtils(CYCLE_CMD_SET_CYCLE_MANAGER_ENABLED, 1);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_PlayCharaMotion(CHARA_FUKU, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_SetCharaLookAtPosition(CHARA_AKIR, -20, 0, 0);
  Script_SetCharaLookAtPosition(CHARA_FUKU, -20, 0, 0);
  Script_executeAicaCommand(131232, 0, 0);
  Script_LoadSND_Wrapper("system1.snd", 0, "bgm009.snd", 0, "f1doujou.snd", "battle_1.snd", 0, 0);
  Script_executeAicaCommand(16296, 0, 0);
  Script_executeAicaCommand(1184, 2, 100);
  Script_PREP_Related(-1, 0);
  Script_GetCurrentDrawMode(0, task_338);
  Script_GetCurrentDrawMode(1, task_33C);
  Script_SetShadowFlags(0, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  script_100D7();
  Script_SetCPAMFlags(CHARA_AKIR, 1);
  Script_SetCPAMFlags(CHARA_FUKU, 1);
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 2);
  Script_charaSetRefboardFlagTo(CHARA_FUKU, 2);
  Script_SetCharaFlags(CHARA_AKIR, 4);
  Script_SetCharaFlags(CHARA_FUKU, 4);
  Script_scenToggleActiveFor(CHARA_AKIR, 1);
  Script_scenToggleActiveFor(CHARA_FUKU, 1);
  Script_AuthorUtils(AUTH_FREE, 9);
  Script_AuthorUtils(AUTH_FREE, 10);
  Script_AuthorUtils(AUTH_FREE, 11);
  Script_AuthorUtils(AUTH_FREE, 12);
  Script_AuthorUtils(AUTH_FREE, 13);
  Script_AuthorUtils(AUTH_FREE, 14);
  Script_AuthorUtils(AUTH_FREE, 15);
  Script_AuthorUtils(AUTH_FREE, 16);
  script_24F70(0);
  Script_LoadVoiceFile(0, "E1017");
  Script_ccowUtils(CHARA_AKIR, 1);
  Script_ccowUtils(CHARA_FUKU, 1);
  Script_ccowSetFlag80(CHARA_AKIR, 2);
  Script_ccowSetFlag80(CHARA_FUKU, 2);
  local_F4.x = 4.75f;
  (&local_F4)->y = 0.0f;
  (&local_F4)->z = -22.5f;
  local_E8.x = 0;
  (&local_E8)->y = 0xC000;
  (&local_E8)->z = 0;
  Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &local_F4);
  Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &local_E8, 0);
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "JHW0");
  script_1B175();
  Script_CreateNPCImageModule(CHARA_AKIR, "YKB_M", 0);
  task_22B = 0;
  Script_GetBlurModel(CHARA_AKIR, "YKA_B.MT5");
  Script_convSetCamInactiveFlag(1);
  Script_SetGameStateTo(AUTH_None);
  Script_SetCameraFlags(1);
  script_FEDE();
  task_22A = 1;
  Script_MT5Loader_TRWV_TRWAVE(22, task_31C, 3, 2);
  task_228 = 0;
  Script_SetActionSelectorHUDElementVisibility(0);
  Script_uiUtils(0, 0, 0);
  Script_shenClearFreeState(1);
  Script_scenUtils(SCENARIO_CMD_UPDATE_TIMER);
  Script_ryoControl(RYO_RestoreControl);
  return;
}

static int32_t script_1CE80(void) {
  int8_t local_FF;
  script_1C6A3();
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_JHW0_EVENT_SEEN)) {
    local_FF = 1;
  } else {
    local_FF = 0;
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_JHW0_EVENT_SEEN, 1);
  }
  script_1E98F(local_FF);
  script_1CBC9();
  Script_filmCheckOrSetTransitionUtil(60, 255, 0, 0, 0, 0, 0, 0, 0);
  SCNF_WaitFrames_0000(2);
  return Script_srchControl(0);
}

static void script_1CEE1(int32_t arg0) {
  if (arg0 == 1) {
    Script_SetFightProgramStateBytes(CHARA_AKIR, 2, 0);
    Script_GetDataFromFightProgram(CHARA_AKIR, 2);
    Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_KONGO_TYUKEN_Y_MID, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    script_2449A(CHARA_AKIR);
    Script_GetDataFromFightProgram(CHARA_AKIR, 0);
    Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_IDO_BOX_B, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    script_2449A(CHARA_AKIR);
  } else if (arg0 == 2) {
    Script_SetFightProgramStateBytes(CHARA_AKIR, 2, 0);
    Script_GetDataFromFightProgram(CHARA_AKIR, 2);
    Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_KONGO_TYUKEN_Y_MID, -1.0f, 22.0f, -1.0f, 1.0f, 0);
    script_2449A(CHARA_AKIR);
    Script_GetDataFromFightProgram(CHARA_AKIR, 0);
  } else if (arg0 == 3) {
    Script_SetFightProgramStateBytes(CHARA_AKIR, 2, 0);
    Script_GetDataFromFightProgram(CHARA_AKIR, 2);
    Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_KONGO_TYUKEN_Y_MID, -1.0f, 22.0f, -1.0f, 0.5f, 0);
    script_2449A(CHARA_AKIR);
    Script_GetDataFromFightProgram(CHARA_AKIR, 0);
  } else {
    Script_SetFightProgramStateBytes(CHARA_AKIR, 2, 0);
    Script_GetDataFromFightProgram(CHARA_AKIR, 2);
    Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_KONGO_TYUKEN_Y_MID, -1.0f, 22.0f, -1.0f, 0.5f, 0);
    script_2449A(CHARA_AKIR);
    Script_GetDataFromFightProgram(CHARA_AKIR, 0);
  }
  return;
}

static void script_1D086(int32_t arg0, int32_t arg1) {
  Script_shenClearFreeState(1);
  Script_sicnUtils(0, 1, 1);
  Script_SetCharaLookAtPosition(CHARA_AKIR, -20, 0, 0);
  SCNF_WaitFrames_0000(1);
  Script_ReinitFightState(0, CHARA_AKIR, CHARA_FUKU, arg0, arg1, &task_3E0);
  for (;;) {
    Script_fightingStyle();
    if (*(int8_t *)&task_3E0 == 2) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_shenClearFreeState(0);
  Script_sicnUtils(2, 1, 0);
  return;
}

static int32_t script_1D0F8(void) {
  int32_t unused_F4[3];
  Vector3i local_E8;
  Script_charaDisplay(CHARA_FUKU, CDM_Hide);
  Script_FACE(CHARA_AKIR, 5, 0, 0);
  script_1B9B2(CHARA_AKIR, 1);
  Script_handSetFloatsFor(CHARA_AKIR, 1, data_2710C, 0);
  Script_handSetFloatsFor(CHARA_AKIR, 0, data_2710C, 0);
  Script_EVT_SetPosition(CHARA_AKIR, 0x78000000, &data_29D24);
  (&local_E8)->y = 31857;
  Script_EVT_SetRotation(CHARA_AKIR, MATRIX_XFORM_Y, &local_E8, 0);
  Script_InitializeOsag(CHARA_AKIR);
  Script_motmToggleFlags(CHARA_AKIR, 2, 0x40);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_SEKKIN_KAMAE_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  SCNF_WaitFrames_0000(3);
  Script_motmToggleFlags(CHARA_AKIR, 1, 0x40);
  Script_charaToggleMotEnabled(CHARA_AKIR, 1);
  script_1CEE1(3);
  script_24548();
  end:;
}

static int32_t script_1D1DE(void) {
  Vector3i local_F4;
  Script_EVT_SetPosition(CHARA_AKIR, 0x78000000, &data_29D24);
  (&local_F4)->y = 31857;
  Script_EVT_SetRotation(CHARA_AKIR, MATRIX_XFORM_Y, &local_F4, 0);
  Script_InitializeOsag(CHARA_AKIR);
  Script_motmToggleFlags(CHARA_AKIR, 2, 0x40);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_SEKKIN_KAMAE_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  SCNF_WaitFrames_0000(3);
  Script_motmToggleFlags(CHARA_AKIR, 1, 0x40);
  Script_charaToggleMotEnabled(CHARA_AKIR, 1);
  script_1CEE1(0);
  Script_FACE(CHARA_AKIR, 0, -1, 16);
  script_24548();
  SCNF_WaitFrames_0000(2);
  return Script_charaDisplay(CHARA_FUKU, CDM_Show);
}

static void script_1D29A(void) {
  Vector3 local_F4;
  script_24548();
  script_1B9B2(CHARA_AKIR, 0);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_charaToggleMotEnabled(CHARA_AKIR, 1);
  Script_EVT_GetPosition(CHARA_FUKU, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  SCNF_CHAR_SCNFApplyFacingTowardPoint(CHARA_AKIR, &local_F4, 0, 0.0f, 0, 0x200000);
  Script_EVT_GetPosition(CHARA_FUKU, 5, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  Script_SetLookPointDetails(CHARA_AKIR, 10, &local_F4, 0);
  Script_EVT_GetPosition(CHARA_AKIR, 5, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  Script_SetLookPointDetails(CHARA_FUKU, 10, &local_F4, 0);
  return;
}

static void script_1D34B(int32_t arg0) {
  Script_sprintf("auth %d start\012", arg0);
  arg0 = arg0 + 9;
  Script_SetMotnFlag(CHARA_AKIR, -1);
  Script_SetMotnFlag(CHARA_FUKU, -1);
  Script_motmToggleFlags(CHARA_AKIR, 2, 0x40);
  Script_motmToggleFlags(CHARA_FUKU, 2, 0x40);
  SCNF_WaitFrames_0000(1);
  Script_shenSeqUtils(-9, arg0, 0);
  script_1BCEA(arg0);
  Script_shenSeqUtils(-6);
  return;
}

static void script_1D3B5(void) {
  Script_motmSetBlendFrames(CHARA_AKIR, MOTM_BLEND_FRAMES_2);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_motmSetBlendFrames(CHARA_AKIR, MOTM_BLEND_FRAMES_5);
  return;
}

static int32_t script_1D3F2(int32_t arg0) {
  Vector3i local_F4;
  Vector3i local_E8;
  if (arg0) {
    script_24548();
  }
  Script_motmToggleFlags(CHARA_FUKU, 1, 0x40);
  Script_mtwkGetSetRotation(CHARA_FUKU, &local_F4, 0x2000000);
  (&local_E8)->y = (&local_F4)->y;
  Script_EVT_SetRotation(CHARA_FUKU, MATRIX_XFORM_Y, &local_E8, 0);
  Script_motmSetBlendFrames(CHARA_FUKU, MOTM_BLEND_FRAMES_30);
  Script_PlayCharaMotion(CHARA_FUKU, 20996, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  script_2449A(CHARA_FUKU);
  if (arg0 == 0) {
    script_24548();
  }
  end:;
}

static void script_1D484(void) {
  Vector3i local_F4;
  Vector3i local_E8;
  Script_motmToggleFlags(CHARA_AKIR, 1, 0x40);
  Script_mtwkGetSetRotation(CHARA_AKIR, &local_F4, 0x2000000);
  (&local_E8)->y = (&local_F4)->y;
  Script_EVT_SetRotation(CHARA_AKIR, MATRIX_XFORM_Y, &local_E8, 0);
  Script_motmSetBlendFrames(CHARA_AKIR, MOTM_BLEND_FRAMES_30);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  script_2449A(CHARA_AKIR);
  Script_motmSetBlendFrames(CHARA_AKIR, MOTM_BLEND_FRAMES_5);
  return;
}

static void script_1D509(void) {
  Script_ccowSetFlag80(CHARA_AKIR, 1);
  Script_ccowSetFlag80(CHARA_FUKU, 1);
  Script_SetCPAMFlags(CHARA_AKIR, 0);
  Script_SetCPAMFlags(CHARA_FUKU, 0);
  return;
}

static void script_1D53A(int32_t arg0) {
  int32_t local_FC;
  local_FC = arg0;
  switch (local_FC) {
    case 0:
      script_24F70((int32_t)&data_29D60);
      break;
    case 1:
      script_24F70((int32_t)&data_29E24);
      break;
  }
  return;
}

static void script_1D573(void) {
  Vector3i local_F4;
  Script_motmToggleFlags(CHARA_AKIR, 2, 0x40);
  Script_motmToggleFlags(CHARA_FUKU, 2, 0x40);
  Script_SetCharaLookAtPosition(CHARA_AKIR, -20, 0, 0);
  script_1B9B2(CHARA_AKIR, 0);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_SEKKIN_KAMAE_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_PlayCharaMotion(CHARA_FUKU, 20996, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_motmToggleFlags(CHARA_AKIR, 2, 0x40);
  Script_motmToggleFlags(CHARA_FUKU, 2, 0x40);
  Script_InitializeOsag(CHARA_FUKU);
  Script_EVT_SetPosition(CHARA_FUKU, 0x78000000, &data_29D30);
  Script_EVT_SetRotation(CHARA_FUKU, 0x78000000, &data_2A058, 0);
  Script_InitializeOsag(CHARA_AKIR);
  Script_EVT_SetPosition(CHARA_AKIR, 0x78000000, &data_29D24);
  (&local_F4)->y = 31857;
  Script_EVT_SetRotation(CHARA_AKIR, MATRIX_XFORM_Y, &local_F4, 0);
  SCNF_WaitFrames_0000(1);
  Script_motmToggleFlags(CHARA_AKIR, 1, 0x40);
  Script_motmToggleFlags(CHARA_FUKU, 1, 0x40);
  return;
}

static void script_1D698(void) {
  Vector3i local_F4;
  Vector3i local_E8;
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TURN045_WAD_R, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  script_2449A(CHARA_AKIR);
  Script_mtwkGetSetRotation(CHARA_AKIR, &local_F4, 0x2000000);
  (&local_E8)->y = (&local_F4)->y;
  Script_EVT_SetRotation(CHARA_AKIR, MATRIX_XFORM_Y, &local_E8, 0);
  Script_motmSetBlendFrames(CHARA_AKIR, MOTM_BLEND_FRAMES_15);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  script_2449A(CHARA_AKIR);
  Script_motmSetBlendFrames(CHARA_AKIR, MOTM_BLEND_FRAMES_5);
  return;
}

static int32_t script_1D73E(void) {
  Vector3 local_F4;
  script_24548();
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_WALK_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  for (;;) {
    Script_EVT_GetPosition(CHARA_AKIR, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
    if (!((&local_F4)->x > *(float *)&data_29D48)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  end:;
}

static int32_t script_1D7B7(void) {
  int32_t unused_F4[3];
  Vector3i local_E8;
  Vector3i local_DC;
  script_24548();
  Script_motmSetBlendFrames(CHARA_FUKU, MOTM_BLEND_FRAMES_20);
  Script_PlayCharaMotion(CHARA_FUKU, MN_AKI_AKI_TURN045_WAD_R, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  script_2449A(CHARA_FUKU);
  Script_mtwkGetSetRotation(CHARA_FUKU, &local_E8, 0x2000000);
  (&local_DC)->y = (&local_E8)->y;
  Script_EVT_SetRotation(CHARA_FUKU, MATRIX_XFORM_Y, &local_DC, 0);
  Script_PlayCharaMotion(CHARA_FUKU, 20996, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  script_2449A(CHARA_FUKU);
  Script_motmSetBlendFrames(CHARA_FUKU, MOTM_BLEND_FRAMES_5);
  end:;
}

static void script_1D862(void) {
  return;
}

static void script_1D865(void) {
  return;
}

static int32_t script_1D868(void) {
  end:;
}

static int32_t script_1D86B(void) {
  end:;
}

static int32_t script_1D86E(void) {
  end:;
}

static int32_t script_1D871(void) {
  end:;
}

static int32_t script_1D874(void) {
  end:;
}

static int32_t script_1D877(void) {
  end:;
}

static int32_t script_1D87A(void) {
  end:;
}

static int32_t script_1D87D(void) {
  end:;
}

static int32_t script_1D880(void) {
  end:;
}

static int32_t script_1D883(void) {
  end:;
}

static int32_t script_1D886(void) {
  end:;
}

static int32_t script_1D889(void) {
  end:;
}

static int32_t script_1D88C(void) {
  end:;
}

static int32_t script_1D88F(void) {
  end:;
}

static int32_t script_1D892(void) {
  end:;
}

static int32_t script_1D895(void) {
  end:;
}

static int32_t script_1D898(void) {
  end:;
}

static void script_1D89B(void) {
  SCNT_ShowSubtitle("涼「腕に力を入れるのが＆早すぎるな」\012");
  return;
}

static int32_t script_1D8A3(void) {
  end:;
}

static int32_t script_1D8A6(void) {
  end:;
}

static int32_t script_1D8A9(void) {
  end:;
}

static void script_1D8AC(void) {
  return;
}

static int32_t script_1D8AF(void) {
  end:;
}

static int32_t script_1D8B2(void) {
  end:;
}

static int32_t script_1D8B5(void) {
  end:;
}

static int32_t script_1D8B8(void) {
  end:;
}

static int32_t script_1D8BB(void) {
  end:;
}

static int32_t script_1D8BE(void) {
  end:;
}

static int32_t script_1D8C1(void) {
  end:;
}

static int32_t script_1D8C4(void) {
  end:;
}

static int32_t script_1D8C7(void) {
  end:;
}

static int32_t script_1D8CA(void) {
  end:;
}

static int32_t script_1D8CD(void) {
  end:;
}

static int32_t script_1D8D0(void) {
  end:;
}

static int32_t script_1D8D3(void) {
  end:;
}

static int32_t script_1D8D6(void) {
  end:;
}

static int32_t script_1D8D9(void) {
  end:;
}

static void script_1D8DC(void) {
  SCNT_ShowSubtitle("涼「ああ、分かった」\012");
  return;
}

static void script_1D8E4(void) {
  SCNT_ShowSubtitle("福原「じゃあ、まず俺のを＆見てくださいよ」\012");
  return;
}

static int32_t script_1D8EC(void) {
  end:;
}

static int32_t script_1D8EF(void) {
  end:;
}

static int32_t script_1D8F2(void) {
  end:;
}

static int32_t script_1D8F5(void) {
  end:;
}

static int32_t script_1D8F8(void) {
  end:;
}

static void script_1D8FB(void) {
  Script_ShowUIMessageFromID("E1017A037");
  return;
}

__attribute__((scn_string_bindings(0, data_2A0D6)))
static void script_1D907(void) {
  Script_ShowUIMessageFromID("E1017A038");
  return;
}

static int32_t script_1D913(void) {
  return Script_ShowUIMessageFromID("E1017A039");
}

static int32_t script_1D91F(void) {
  return Script_ShowUIMessageFromID("E1017A040");
}

__attribute__((scn_string_bindings(0, data_2A0F4)))
static int32_t script_1D92B(void) {
  return Script_ShowUIMessageFromID("E1017A038");
}

static int32_t script_1D937(void) {
  return Script_ShowUIMessageFromID("E1017A050");
}

static int32_t script_1D943(void) {
  return Script_ShowUIMessageFromID("E1017B021");
}

static int32_t script_1D94F(void) {
  return Script_ShowUIMessageFromID("E1017B022");
}

static int32_t script_1D95B(void) {
  return Script_ShowUIMessageFromID("E1017B023");
}

static int32_t script_1D967(void) {
  return Script_ShowUIMessageFromID("E1017B024");
}

static int32_t script_1D973(void) {
  return Script_ShowUIMessageFromID("E1017B025");
}

static int32_t script_1D97F(void) {
  return Script_ShowUIMessageFromID("E1017A015");
}

static int32_t script_1D98B(void) {
  return Script_ShowUIMessageFromID("E1017A042");
}

static int32_t script_1D997(void) {
  return Script_ShowUIMessageFromID("E1017A017");
}

static int32_t script_1D9A3(void) {
  return Script_ShowUIMessageFromID("E1017B031");
}

static int32_t script_1D9AF(void) {
  return Script_ShowUIMessageFromID("E1017A044");
}

static int32_t script_1D9BB(void) {
  return Script_ShowUIMessageFromID("E1017A023");
}

static int32_t script_1D9C7(void) {
  return Script_ShowUIMessageFromID("E1017A024");
}

static int32_t script_1D9D3(void) {
  return Script_ShowUIMessageFromID("E1017A025");
}

static void script_1D9DF(void) {
  return;
}

static int32_t script_1D9E2(void) {
  return Script_ShowUIMessageFromID("E1017A027");
}

static int32_t script_1D9EE(void) {
  return Script_ShowUIMessageFromID("E1017A028");
}

static int32_t script_1D9FA(void) {
  return Script_ShowUIMessageFromID("E1017A045");
}

static void script_1DA06(void) {
  Script_ShowUIMessageFromID("E1017A046");
  return;
}

static int32_t script_1DA12(void) {
  return Script_ShowUIMessageFromID("E1017A047");
}

static int32_t script_1DA1E(void) {
  return Script_ShowUIMessageFromID("E1017A048");
}

static int32_t script_1DA2A(void) {
  return Script_ShowUIMessageFromID("E1017B043");
}

static int32_t script_1DA36(void) {
  return Script_ShowUIMessageFromID("E1017A006");
}

static int32_t script_1DA42(void) {
  return Script_ShowUIMessageFromID("E1017B053");
}

__attribute__((scn_string_bindings(0, data_2A1E4)))
static int32_t script_1DA4E(void) {
  return Script_ShowUIMessageFromID("E1017B054");
}

static int32_t script_1DA5A(void) {
  return Script_ShowUIMessageFromID("E1017A031");
}

__attribute__((scn_string_bindings(0, data_2A1F8)))
static int32_t script_1DA66(void) {
  return Script_ShowUIMessageFromID("E1017B038");
}

static int32_t script_1DA72(void) {
  return Script_ShowUIMessageFromID("E1017B044");
}

static int32_t script_1DA7E(void) {
  return Script_ShowUIMessageFromID("E1017B045");
}

static int32_t script_1DA8A(void) {
  return Script_ShowUIMessageFromID("E1017A032");
}

__attribute__((scn_string_bindings(0, data_2A220)))
static int32_t script_1DA96(void) {
  return Script_ShowUIMessageFromID("E1017B038");
}

static int32_t script_1DAA2(void) {
  return Script_ShowUIMessageFromID("E1017B046");
}

static int32_t script_1DAAE(void) {
  return Script_ShowUIMessageFromID("E1017A001");
}

static int32_t script_1DABA(void) {
  return Script_ShowUIMessageFromID("E1017B049");
}

static void script_1DAC6(void) {
  return;
}

static void script_1DAC9(void) {
  return;
}

static int32_t script_1DACC(void) {
  return Script_ShowUIMessageFromID("E1017A035");
}

static int32_t script_1DAD8(void) {
  return Script_ShowUIMessageFromID("E1017B013");
}

static int32_t script_1DAE4(void) {
  return Script_ShowUIMessageFromID("E1017B051");
}

__attribute__((scn_string_bindings(0, data_2A266)))
static int32_t script_1DAF0(void) {
  return Script_ShowUIMessageFromID("E1017B054");
}

static int32_t script_1DAFC(void) {
  return Script_ShowUIMessageFromID("E1017B055");
}

static void script_1DB08(void) {
  Vector3i local_F4;
  script_2440A(9109);
  Script_charaDisplay(CHARA_FUKU, CDM_Show);
  script_1D3B5();
  Script_EVT_SetPosition(CHARA_AKIR, 0x78000000, &data_29D24);
  (&local_F4)->y = 22755;
  Script_EVT_SetRotation(CHARA_AKIR, MATRIX_XFORM_Y, &local_F4, 0);
  Script_FACE(CHARA_FUKU, 7, -1, 16);
  Script_scenCreateTask(script_1DA2A, 0);
  Script_scenCreateTask(script_1D8B5, 0);
  script_24552(90);
  Script_FACE(CHARA_FUKU, 0, -1, 16);
  script_2440A(9110);
  script_24523();
  Script_scenCreateTask(script_1BBB5, 3, CHARA_FUKU, CHARA_AKIR, 0);
  script_2452D();
  Script_FACE(CHARA_AKIR, 4, -1, 16);
  script_24523();
  Script_scenCreateTask(script_1BB4D, 5, CHARA_AKIR, CHARA_FUKU, 30, 0, 1);
  script_2452D();
  Script_scenCreateTask(script_1DA36, 0);
  Script_scenCreateTask(script_1D8B8, 0);
  Script_FACE(CHARA_AKIR, 0, -1, 16);
  script_24552(30);
  script_2440A(9111);
  Script_scenCreateTask(script_1DA42, 0);
  Script_scenCreateTask(script_1D8BB, 0);
  script_24552(90);
  Script_scenCreateTask(script_1DA4E, 0);
  Script_scenCreateTask(script_1D8BE, 0);
  script_24552(90);
  return;
}

static int32_t script_1DC6A(int32_t arg0, int32_t arg1, int32_t arg2) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  if (task_34C >= 2) {
    local_F8 = 0xFFFF;
  } else {
    local_F8 = SCNF_RequestActionSelectorFromParams_0008(0xFFFF, 37, 34, 0xFFFF, 450, 0xFFFF, ACTION_SELECTOR_MODE_STANDARD);
  }
  local_F4 = local_F8;
  if (local_F4 == 37) {
    if (arg2 == 1) {
      script_24486(9114, CHARA_AKIR, CHARA_AKIR);
    } else {
      script_24486(9107, CHARA_AKIR, CHARA_AKIR);
    }
    task_34C = task_34C + 1;
    script_24523();
    Script_scenCreateTask(script_1BB4D, 5, CHARA_AKIR, CHARA_FUKU, 30, 0, 1);
    script_2452D();
    Script_scenCreateTask(script_1DA5A, 0);
    Script_scenCreateTask(script_1D8C1, 0);
    script_24552(90);
    script_24486(9108, CHARA_FUKU, CHARA_FUKU);
    Script_FACE(CHARA_FUKU, 1, -1, 16);
    script_24523();
    Script_scenCreateTask(script_1BB4D, 5, CHARA_FUKU, CHARA_AKIR, 30, 0, 1);
    script_2452D();
    Script_scenCreateTask(script_1DA66, 0);
    Script_scenCreateTask(script_1D8C4, 0);
    script_24552(45);
    Script_scenCreateTask(script_1DA72, 0);
    Script_scenCreateTask(script_1D8C7, 0);
    Script_FACE(CHARA_FUKU, 0, -1, 16);
    script_24552(90);
    local_FC = arg0;
  } else {
    task_348 = 1;
    script_1D34B(4);
    script_1D509();
    local_FC = arg1;
  }
  return local_FC;
}

static int32_t script_1DDF6(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  task_34C = 0;
  script_2440A(9113);
  script_24523();
  Script_scenCreateTask(script_1D0F8, 0);
  script_2452D();
  script_2440A(9112);
  script_24523();
  Script_scenCreateTask(script_1D1DE, 0);
  script_2452D();
  Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_JHW0_EVENT_COMPLETE, 1);
  if (task_350 == 0) {
    script_1D34B(3);
    script_24523();
    Script_scenCreateTask(script_1D3F2, 1, 1);
    script_2452D();
    script_1D509();
    script_24523();
    Script_scenCreateTask(script_1BBB5, 3, CHARA_FUKU, CHARA_AKIR, 1);
    script_2452D();
  } else {
    script_1DB08();
  }
  local_FC = script_1DC6A(arg0, arg1, 1);
  return local_FC;
}

static int32_t script_1DEB6(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  if (task_3DE) {
    task_350 = 1;
    script_24486(9103, CHARA_FUKU, CHARA_FUKU);
    if (arg0 == 1) {
      script_24523();
      Script_scenCreateTask(script_1B922, 3, CHARA_AKIR, CHARA_FUKU, 0);
      script_2452D();
    }
    Script_FACE(CHARA_FUKU, 8, -1, 16);
    Script_scenCreateTask(script_1DA7E, 0);
    Script_scenCreateTask(script_1D8CA, 0);
    script_24552(90);
    Script_FACE(CHARA_FUKU, 0, -1, 16);
    if (task_34C >= 2) {
      local_F8 = 0xFFFF;
    } else {
      local_F8 = SCNF_RequestActionSelectorFromParams_0008(0xFFFF, 37, 34, 0xFFFF, 450, 0xFFFF, ACTION_SELECTOR_MODE_STANDARD);
    }
    local_F4 = local_F8;
    if (local_F4 == 37) {
      task_34C = task_34C + 1;
      script_2440A(9104);
      script_24523();
      Script_scenCreateTask(script_1B9F2, 4, CHARA_AKIR, CHARA_FUKU, 1, 1);
      script_2452D();
      Script_scenCreateTask(script_1DA8A, 0);
      Script_scenCreateTask(script_1D8CD, 0);
      script_24552(90);
      script_24523();
      Script_scenCreateTask(script_1BB4D, 5, CHARA_FUKU, CHARA_AKIR, 30, 1, 1);
      script_2452D();
      Script_scenCreateTask(script_1DA96, 0);
      Script_scenCreateTask(script_1D8D0, 0);
      script_24552(90);
      Script_scenCreateTask(script_1DAA2, 0);
      Script_scenCreateTask(script_1D8D3, 0);
      script_24552(90);
      local_FC = arg1;
    } else {
      script_1D34B(7);
      script_1D509();
      local_FC = arg3;
    }
    task_3DE = 0;
    task_3C8 = 0;
  } else {
    local_FC = arg1;
  }
  return local_FC;
}

static void script_1E07D(int32_t arg0, int32_t arg1) {
  script_1D34B(5);
  script_24523();
  Script_scenCreateTask(script_1D3F2, 1, 1);
  script_2452D();
  script_1D509();
  return;
}

static int32_t script_1E0A5(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  task_34C = 0;
  script_2440A(9113);
  script_24523();
  Script_scenCreateTask(script_1D0F8, 0);
  script_2452D();
  script_2440A(9112);
  script_24523();
  Script_scenCreateTask(script_1D1DE, 0);
  script_2452D();
  Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_JHW0_EVENT_COMPLETE, 1);
  if (task_350 == 0) {
    script_1D34B(6);
    script_24523();
    Script_scenCreateTask(script_1BBB5, 3, CHARA_FUKU, CHARA_AKIR, 1);
    script_2452D();
    script_24523();
    Script_scenCreateTask(script_1D3F2, 1, 1);
    script_2452D();
    script_1D509();
  } else {
    script_1DB08();
  }
  local_FC = script_1DC6A(arg2, arg3, 1);
  return local_FC;
}

static int32_t script_1E165(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  script_2440A(9106);
  script_1D698();
  Script_scenCreateTask(script_1DAF0, 0);
  Script_scenCreateTask(script_1D8F5, 0);
  script_24552(90);
  local_FC = script_1DC6A(arg2, arg3, 0);
  return local_FC;
}

static int32_t script_1E1AB(int32_t arg0, int32_t arg1, int32_t arg2) {
  int32_t local_FC;
  if (arg0 == 1) {
    script_24523();
    Script_scenCreateTask(script_1BBB5, 3, CHARA_FUKU, CHARA_AKIR, 0);
    script_2452D();
  }
  local_FC = arg1;
  if (local_FC == 0xBC614E) {
  } else if (local_FC == 20) {
    local_FC = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f);
    if (local_FC == 0) {
      Script_scenCreateTask(script_1D943, 0);
      Script_scenCreateTask(script_1D874, 0);
      script_24552(90);
    } else {
      Script_scenCreateTask(script_1D94F, 0);
      Script_scenCreateTask(script_1D877, 0);
      script_24552(90);
    }
    local_FC = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f);
    if (local_FC == 0) {
      Script_scenCreateTask(script_1D95B, 0);
      Script_scenCreateTask(script_1D87A, 0);
      script_24552(90);
    } else if (local_FC == 1) {
      Script_scenCreateTask(script_1D967, 0);
      Script_scenCreateTask(script_1D87D, 0);
      script_24552(90);
    } else {
      Script_scenCreateTask(script_1D973, 0);
      Script_scenCreateTask(script_1D880, 0);
      script_24552(90);
    }
  } else {
    local_FC = *(int8_t *)&task_3EC[arg2][3];
    if (local_FC == 1) {
      local_FC = *(int8_t *)&task_3EC[arg2][2];
      if (local_FC == 1) {
        Script_scenCreateTask(script_1D97F, 0);
        Script_scenCreateTask(script_1D883, 0);
        script_24552(30);
        local_FC = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f);
        if (local_FC == 0) {
          Script_scenCreateTask(script_1D98B, 0);
          Script_scenCreateTask(script_1D886, 0);
          script_24552(90);
        } else {
          Script_scenCreateTask(script_1D997, 0);
          Script_scenCreateTask(script_1D889, 0);
          script_24552(90);
        }
      } else {
        if ((local_FC == 2) | (local_FC == 8) | (local_FC == 9) | (local_FC == 10) | (local_FC == 11)) {
          Script_scenCreateTask(script_1D9A3, 0);
          Script_scenCreateTask(script_1D88C, 0);
          script_24552(90);
          goto L01E3CD;
        }
      }
      L01E3CD:;
      goto L01E3D0;
    }
  }
  L01E3D0:;
  return task_354 = 0;
}

static int32_t script_1E3D8(int32_t arg0, int32_t arg1, int32_t arg2) {
  int32_t local_FC;
  if (!(arg0 == 1)) goto L01E3E3;
  L01E3E3:;
  local_FC = arg1;
  if (local_FC == 0xBC614E) {
  } else if (local_FC == 20) {
    local_FC = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f);
    if (local_FC == 0) {
      Script_scenCreateTask(script_1D943, 0);
      Script_scenCreateTask(script_1D874, 0);
      script_24552(90);
    } else {
      Script_scenCreateTask(script_1D94F, 0);
      Script_scenCreateTask(script_1D877, 0);
      script_24552(90);
    }
    local_FC = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f);
    if (local_FC == 0) {
      Script_scenCreateTask(script_1D95B, 0);
      Script_scenCreateTask(script_1D87A, 0);
      script_24552(90);
    } else if (local_FC == 1) {
      Script_scenCreateTask(script_1D967, 0);
      Script_scenCreateTask(script_1D87D, 0);
      script_24552(90);
    } else {
      Script_scenCreateTask(script_1D973, 0);
      Script_scenCreateTask(script_1D880, 0);
      script_24552(90);
    }
  } else {
    local_FC = *(int8_t *)&task_3EC[arg2][3];
    switch (local_FC) {
      case 1:
        local_FC = *(int8_t *)&task_3EC[arg2][2];
        if ((local_FC == 8) | (local_FC == 7)) {
          local_FC = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f);
          if (local_FC == 0) {
            Script_scenCreateTask(script_1D9AF, 0);
            Script_scenCreateTask(script_1D88F, 0);
            script_24552(90);
          } else {
            Script_scenCreateTask(script_1D9BB, 0);
            Script_scenCreateTask(script_1D892, 0);
            script_24552(90);
          }
        } else {
          switch (local_FC) {
            case 9:
              Script_scenCreateTask(script_1D91F, 0);
              Script_scenCreateTask(script_1D86B, 0);
              script_24552(90);
              break;
            case 10:
              Script_scenCreateTask(script_1D92B, 0);
              Script_scenCreateTask(script_1D86E, 0);
              script_24552(90);
              break;
            case 11:
              Script_scenCreateTask(script_1D937, 0);
              Script_scenCreateTask(script_1D871, 0);
              script_24552(90);
              break;
            case 2:
              Script_scenCreateTask(script_1D92B, 0);
              Script_scenCreateTask(script_1D86E, 0);
              script_24552(90);
              break;
            case 1:
              local_FC = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f);
              if (local_FC == 0) {
                Script_scenCreateTask(script_1D9FA, 0);
                Script_scenCreateTask(script_1D8A9, 0);
                script_24552(90);
              } else {
                Script_scenCreateTask(script_1D92B, 0);
                Script_scenCreateTask(script_1D86E, 0);
                script_24552(90);
              }
              local_FC = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f);
              if (local_FC == 0) {
                Script_scenCreateTask(script_1DA12, 0);
                Script_scenCreateTask(script_1D8AF, 0);
                script_24552(90);
              } else {
                Script_scenCreateTask(script_1DA1E, 0);
                Script_scenCreateTask(script_1D8B2, 0);
                script_24552(90);
              }
              break;
          }
        }
        break;
      case 3:
        local_FC = *(int8_t *)&task_3EC[arg2][2];
        switch (local_FC) {
          case 2:
            Script_scenCreateTask(script_1D92B, 0);
            Script_scenCreateTask(script_1D86E, 0);
            script_24552(90);
            break;
          case 8:
            Script_scenCreateTask(script_1D913, 0);
            Script_scenCreateTask(script_1D868, 0);
            script_24552(90);
            break;
          case 9:
            Script_scenCreateTask(script_1D91F, 0);
            Script_scenCreateTask(script_1D86B, 0);
            script_24552(90);
            break;
          case 10:
            Script_scenCreateTask(script_1D92B, 0);
            Script_scenCreateTask(script_1D86E, 0);
            script_24552(90);
            break;
          case 11:
            Script_scenCreateTask(script_1D937, 0);
            Script_scenCreateTask(script_1D871, 0);
            script_24552(90);
            break;
          case 3:
            Script_scenCreateTask(script_1D9C7, 0);
            Script_scenCreateTask(script_1D895, 0);
            script_24552(90);
            break;
          case 14:
            Script_scenCreateTask(script_1D9D3, 0);
            Script_scenCreateTask(script_1D898, 0);
            script_24552(90);
            break;
          case 15:
            Script_scenCreateTask(script_1D9E2, 0);
            Script_scenCreateTask(script_1D8A3, 0);
            script_24552(90);
            break;
          case 16:
            Script_scenCreateTask(script_1D9EE, 0);
            Script_scenCreateTask(script_1D8A6, 0);
            script_24552(90);
            break;
          case 12:
            Script_scenCreateTask(script_1DA12, 0);
            Script_scenCreateTask(script_1D8AF, 0);
            script_24552(90);
            break;
          case 17:
            break;
        }
        break;
    }
  }
  return task_354 = 0;
}

static int32_t script_1E8CD(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  int32_t local_FC;
  script_2440A(9123);
  script_24F70((int32_t)&data_29E24);
  script_1D698();
  script_24523();
  Script_scenCreateTask(script_1BBB5, 3, CHARA_FUKU, CHARA_AKIR, 0);
  script_2452D();
  script_1E3D8(arg0, arg1, arg2);
  script_24523();
  Script_scenCreateTask(script_1BBB5, 3, CHARA_FUKU, CHARA_AKIR, 1);
  script_2452D();
  script_24523();
  Script_scenCreateTask(script_1BBB5, 3, CHARA_AKIR, CHARA_FUKU, 1);
  script_2452D();
  Script_scenCreateTask(script_1DAFC, 0);
  Script_scenCreateTask(script_1D8F8, 0);
  script_24552(90);
  local_FC = script_1DC6A(arg3, arg4, 0);
  return local_FC;
}

static void script_1E98F(int32_t arg0) {
  int16_t local_FE;
  uint8_t unused_FC[2];
  int32_t local_F8;
  int32_t local_F4;
  int32_t local_F0;
  int32_t local_EC;
  local_FE = 0;
  local_F8 = 0;
  for (;;) {
    local_EC = local_FE;
    switch (local_EC) {
      case -1:
        SCNF_WaitFrames_0000(2);
        break;
      case 0:
        {
          *(int8_t *)task_35C = 0;
          *((int8_t *)task_35C + 1) = 0;
          *((int8_t *)task_35C + 9) = 0;
          task_350 = 0;
          task_334 = 0;
          task_3C8 = 0;
          task_34C = 0;
          if (local_F8 == 0) {
            local_F8 = Script_scenCreateTask(script_24F1B, 0);
          }
          Script_executeAicaCommand(16296, 0, 0);
          Script_executeAicaCommand(1184, 2, 105);
          Script_set_val_to_1(1, 64);
          Script_filmCheckOrSetTransitionUtil(60, 255, 0, 0, 0, 0, 0, 0, 0);
          SCNF_WaitFrames_0000(2);
          script_24C5E();
          if (arg0) {
            {
              Vector3i local_E0;
              (&local_E0)->y = 910;
              Script_EVT_SetRotation(CHARA_FUKU, MATRIX_XFORM_Y, &local_E0, 0);
            }
            Script_EVT_SetPosition(CHARA_FUKU, 0x78000000, &data_29D54);
            Script_EVT_SetPosition(CHARA_AKIR, 0x78000000, &data_29D3C);
            {
              Vector3i local_E0;
              (&local_E0)->y = 22209;
              Script_EVT_SetRotation(CHARA_AKIR, MATRIX_XFORM_Y, &local_E0, 0);
            }
            Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
            Script_PlayCharaMotion(CHARA_FUKU, 20997, -1.0f, -1.0f, -1.0f, 1.0f, 0);
          }
          Script_SetCharaLookAtPosition(CHARA_AKIR, -20, 0, 0);
          Script_SetCharaLookAtPosition(CHARA_FUKU, -20, 0, 0);
          if (arg0 == 0) {
            local_FE = 10;
          } else {
            local_FE = 30;
          }
          task_348 = 0;
          Script_motmToggleFlags(CHARA_FUKU, 2, 8);
          break;
        }
      case 10:
        script_1D34B(0);
        script_1D509();
        script_24523();
        Script_scenCreateTask(script_1BBB5, 3, CHARA_AKIR, CHARA_FUKU, 1);
        script_2452D();
        local_F4 = SCNF_RequestActionSelectorFromParams_0008(0xFFFF, 63, 60, 0xFFFF, 450, 0xFFFF, ACTION_SELECTOR_MODE_STANDARD);
        local_EC = local_F4;
        if (local_EC == 63) {
          local_FE = 40;
        } else {
          local_FE = 20;
        }
        break;
      case 20:
        script_1D34B(2);
        script_1D509();
        task_334 = -1;
        local_FE = 100;
        break;
      case 30:
        {
          Vector3i local_E0;
          Script_charaDisplay(CHARA_AKIR, CDM_Hide);
          script_2440A(9117);
          SCNF_WaitFrames_0000(290);
          Script_charaDisplay(CHARA_AKIR, CDM_Show);
          script_24523();
          Script_scenCreateTask(script_1D73E, 0);
          script_2452D();
          SCNF_WaitFrames_0000(30);
          Script_CharHandlers(CHARA_FUKU, 2, 8, 16);
          script_24523();
          Script_scenCreateTask(script_1D7B7, 0);
          script_2452D();
          Script_scenCreateTask(script_24B94, 2, CHARA_FUKU, -1);
          script_24523();
          Script_scenCreateTask(script_1BBB5, 3, CHARA_AKIR, CHARA_FUKU, 1);
          script_2452D();
          Script_FaceLookAtTargetUtils(CHARA_FUKU, 0);
          Script_scenCreateTask(script_1DAAE, 0);
          Script_scenCreateTask(script_1D8D6, 0);
          script_24552(30);
          script_2440A(9119);
          (&local_E0)->y = 57161;
          Script_EVT_SetRotation(CHARA_FUKU, MATRIX_XFORM_Y, &local_E0, 0);
          Script_scenCreateTask(script_1DABA, 0);
          Script_scenCreateTask(script_1D8D9, 0);
          script_24552(90);
          task_8B4 = 0;
          task_8B3 = 0;
          local_F4 = SCNF_RequestActionSelectorFromParams_0008(0xFFFF, 63, 60, 0xFFFF, 450, 0xFFFF, ACTION_SELECTOR_MODE_STANDARD);
          local_EC = local_F4;
          if (local_EC == 63) {
            local_FE = 40;
          } else {
            local_FE = 50;
          }
          break;
        }
      case 40:
        script_1D34B(1);
        script_1D509();
        local_FE = 70;
        break;
      case 50:
        script_24486(9120, CHARA_AKIR, CHARA_AKIR);
        script_24523();
        Script_scenCreateTask(script_1B9F2, 4, CHARA_AKIR, CHARA_FUKU, 1, 1);
        script_2452D();
        Script_scenCreateTask(script_1DACC, 0);
        Script_scenCreateTask(script_1D8EC, 0);
        script_24552(90);
        script_2440A(9121);
        Script_FACE(CHARA_FUKU, 6, -1, 16);
        script_24523();
        Script_scenCreateTask(script_1BB4D, 5, CHARA_FUKU, CHARA_AKIR, 30, 1, 1);
        script_2452D();
        Script_scenCreateTask(script_1DAD8, 0);
        Script_scenCreateTask(script_1D8EF, 0);
        script_24552(90);
        Script_FACE(CHARA_FUKU, 0, -1, 16);
        Script_scenCreateTask(script_1DAE4, 0);
        Script_scenCreateTask(script_1D8F2, 0);
        script_24552(90);
        local_FE = 100;
        break;
      case 70:
        local_FE = script_1F0C2(0, 60, 61, 80, 90, 70, -1, 100);
        break;
      case 80:
        local_FE = script_1F0C2(1, 61, -1, 90, 90, 80, -1, 100);
        break;
      case 90:
        local_FE = script_1F0C2(9, 61, -1, 90, 90, 90, -1, 100);
        break;
      case 100:
        script_1D484();
        Script_executeAicaCommand(2720, 2, 30);
        Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
        while (Script_filmCheckOrSetTransitionUtil(-1)) {
          SCNF_WaitFrames_0000(1);
        }
        Script_SetCharaLookAtPosition(CHARA_AKIR, -98312, 0, 0);
        Script_SetCharaLookAtPosition(CHARA_FUKU, -98312, 0, 0);
        local_FE = 200;
        break;
      case 200:
        if (local_F8) {
          Script_srchControl((void *)local_F8);
          local_F8 = 0;
        }
        if (task_334 == 0x800000) {
          Script_wazaUtils(MOVE_UI_CMD_SET_MOVE_SKILL_VALUE, 1, 13, 12);
          Script_wazaUtils(MOVE_UI_CMD_SHOW_PENDING_MASTERED_POPUP, 0, 0, 0);
          local_F0 = 150;
          for (;;) {
            local_F0 = local_F0 - 1;
            if ((Script_GetControllerInput(1) == 0x200) | (local_F0 == 0)) {
              Script_wazaUtils(MOVE_UI_CMD_CLOSE_MOVE_POPUP, 0, 0, 0);
              break;
            }
            SCNF_WaitFrames_0000(1);
          }
        }
        goto L01EFE8;
        break;
    }
    SCNF_WaitFrames_0000(1);
  }
  L01EFE8:;
  return;
}

static void script_1EFE9(void) {
  if (task_340 == 0) {
    task_340 = Script_scenCreateTask(script_244CF, 1, 9101);
    script_1B9B2(CHARA_AKIR, 1);
    Script_handSetFloatsFor(CHARA_AKIR, 1, data_2710C, 0);
    Script_handSetFloatsFor(CHARA_AKIR, 0, data_2710C, 0);
    Script_shenClearFreeState(1);
  }
  if (task_344 == 0) {
    task_344 = Script_scenCreateTask(script_1BC88, 3, CHARA_FUKU, CHARA_AKIR, -1);
  }
  return;
}

static void script_1F06A(void) {
  if (task_340) {
    if (task_340) {
      Script_srchControl((void *)task_340);
      task_340 = 0;
    }
    script_1B9B2(CHARA_AKIR, 0);
  }
  if (task_344) {
    if (task_344) {
      Script_srchControl((void *)task_344);
      task_344 = 0;
    }
    Script_SetCharaLookAtPosition(CHARA_AKIR, -20, 0, 0);
  }
  return;
}

static int32_t script_1F0C2(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5, int32_t arg6, int32_t arg7) {
  int32_t unused_FC;
  int32_t local_F8;
  int32_t local_F4;
  int32_t local_F0;
  int32_t local_EC;
  int32_t local_E8;
  int32_t unused_E4;
  int8_t local_E3;
  int8_t local_E2;
  script_24C9A();
  task_3DC = 0;
  task_3DD = 0;
  task_3DE = 0;
  task_3BC = 0;
  task_3C0 = 0;
  task_3C4 = 0;
  local_EC = 0;
  local_E8 = 0;
  task_354 = 0;
  task_358 = 0;
  script_1EFE9();
  script_1D573();
  for (;;) {
    Script_set_val_to_1(2, 0);
    task_3CC = 0;
    local_EC = Script_scenCreateTask(script_249C4, 0);
    local_E8 = Script_scenCreateTask(script_24AB9, 1, arg0);
    task_3D4 = 0;
    local_E3 = 0;
    for (;;) {
      local_E2 = 0;
      script_1D086(arg1, arg2);
      script_249E8(task_3D4);
      if (*((int8_t *)&task_3E0 + 2) != 20) {
        task_358 = task_358 + 1;
      }
      if (*((int8_t *)&task_3E0 + 1) == 2) {
        script_24701(CHARA_AKIR, 0, (int32_t)&data_29D24);
        task_3BC = task_3BC + 1;
        task_3C0 = task_3C0 + 1;
        SCNF_WaitFrames_0000(1);
        if (task_3DC) {
          Script_StartOrFreeSEQ(0);
          if (task_354) {
            if (task_354) {
              Script_srchControl((void *)task_354);
              task_354 = 0;
            }
          }
          if (local_EC) {
            Script_srchControl((void *)local_EC);
            local_EC = 0;
          }
          if (local_E8) {
            Script_srchControl((void *)local_E8);
            local_E8 = 0;
          }
          Script_set_val_to_1(3, 0);
          local_F4 = script_24AAE(task_3D4);
          local_F0 = script_24AA8(task_3D4, local_F4);
          if (task_3DE | task_3DD) {
            if (task_3DE & -(task_358 >= 9)) {
              uint8_t unused_E0[2];
              int32_t local_DC;
              local_DC = arg0;
              switch (local_DC) {
                case 0:
                  script_24F70((int32_t)&data_29D60);
                  break;
                case 1:
                  script_24F70((int32_t)&data_29E24);
                  break;
              }
            }
            script_248C7(0);
            script_1F06A();
            {
              uint8_t unused_E0[2];
              int32_t local_DC;
              local_DC = arg0;
              switch (local_DC) {
                case 0:
                  script_1E1AB(0, local_F4, local_F0);
                  local_F8 = script_1DEB6(1, arg5, arg6, arg7);
                  if (local_F8 == arg5) {
                    script_1EFE9();
                    script_1D573();
                  }
                  break;
                case 1:
                  script_1E3D8(0, local_F4, local_F0);
                  local_F8 = script_1DEB6(1, arg5, arg6, arg7);
                  if (local_F8 == arg5) {
                    script_1EFE9();
                    script_1D573();
                  }
                  break;
                case 9:
                  script_248C7(0);
                  script_1F06A();
                  local_F8 = script_1E8CD(1, local_F4, local_F0, arg3, arg7);
                  if (task_348 == 0) {
                    script_1EFE9();
                    script_1D573();
                  } else {
                    script_1F06A();
                  }
                  break;
              }
            }
          } else {
            if (task_3DF & -(task_358 >= 9)) {
              uint8_t unused_E0[2];
              int32_t local_DC;
              local_DC = arg0;
              switch (local_DC) {
                case 0:
                  script_24F70((int32_t)&data_29D60);
                  break;
                case 1:
                  script_24F70((int32_t)&data_29E24);
                  break;
              }
            }
            {
              uint8_t unused_E0[2];
              int32_t local_DC;
              local_DC = arg0;
              switch (local_DC) {
                case 0:
                  task_354 = Script_scenCreateTask(script_1E1AB, 3, 0, local_F4, local_F0);
                  local_F8 = arg5;
                  break;
                case 1:
                  task_354 = Script_scenCreateTask(script_1E3D8, 3, 0, local_F4, local_F0);
                  local_F8 = arg5;
                  break;
                case 9:
                  task_354 = Script_scenCreateTask(script_1E3D8, 3, 0, local_F4, local_F0);
                  local_F8 = arg5;
                  break;
              }
            }
          }
          task_3D4 = 0;
          task_3CC = 0;
          task_3DC = 0;
          task_3DD = 0;
          task_3DE = 0;
          task_3C0 = 0;
          task_3C4 = 0;
          if (local_F8 != arg5) {
            (void)local_F8;
            goto L01F6C2;
          }
          script_248C7(0);
          break;
        }
        script_248C7(0);
      } else {
        uint8_t unused_E0[2];
        int32_t local_DC;
        Script_StartOrFreeSEQ(0);
        if (local_EC) {
          Script_srchControl((void *)local_EC);
          local_EC = 0;
        }
        if (local_E8) {
          Script_srchControl((void *)local_E8);
          local_E8 = 0;
        }
        Script_set_val_to_1(3, 0);
        SCNF_WaitFrames_0000(1);
        if (*((int8_t *)task_35C + arg0) == 0) {
          *((int8_t *)task_35C + arg0) = 1;
        }
        local_DC = *((int8_t *)&task_3E0 + 2);
        if (local_DC == 22) {
          script_1CEE1(2);
          script_1F06A();
          script_24F70((int32_t)&data_29E24);
          Script_PreserveAccumulator_00CD(0, 0x800000);
          task_334 = 0x800000;
          local_F8 = script_1DDF6(arg4, arg7);
        } else {
          local_F4 = script_24AAE(task_3D4 + 1);
          local_F0 = script_24AA8(task_3D4 + 1, local_F4);
          local_DC = arg0;
          switch (local_DC) {
            case 0:
              script_1F06A();
              script_24F70((int32_t)&data_29D60);
              script_1E07D(local_F4, local_F0);
              local_F8 = arg3;
              break;
            case 1:
              script_1CEE1(2);
              script_1F06A();
              script_24F70((int32_t)&data_29E24);
              Script_PreserveAccumulator_00CD(0, 0x800000);
              task_334 = 0x800000;
              local_F8 = script_1E0A5(local_F4, local_F0, arg3, arg7);
              break;
            case 9:
              script_1CEE1(1);
              script_24F70((int32_t)&data_29E24);
              local_E3 = local_E3 + 1;
              if ((local_E3 == 5) | task_3DD) {
                script_1F06A();
                local_F8 = script_1E165(local_F4, local_F0, arg3, arg7);
              } else {
                script_24701(CHARA_AKIR, 0, (int32_t)&data_29D24);
                script_248C7(0);
                local_F8 = arg5;
                local_E2 = 1;
              }
              break;
          }
        }
        if (local_E2 == 0) {
          task_3C8 = 0;
          (void)local_F8;
          goto L01F6C2;
        } else {
          local_EC = Script_scenCreateTask(script_249C4, 0);
          local_E8 = Script_scenCreateTask(script_24AB9, 1, arg0);
          SCNF_WaitFrames_0000(2);
        }
      }
      if (task_3D4 < 100) {
        task_3D4 = task_3D4 + 1;
        goto L01F6B7;
      }
      L01F6B7:;
    }
  }
  script_1F06A();
  L01F6C2:;
}

static int32_t script_1F6C3(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  local_FC = 0;
  while (SCN_GetIsFighting() != 2) {
    SCNF_WaitFrames_0000(1);
    if (Script_GetFightingStats(-1, 2) != 1) {
      local_FC = local_FC + 1;
    }
  }
  local_F8 = Script_PlayerStatsUtils(PS_GetShugyoCourseActiveFramesSlot, 0);
  local_F8 = local_F8 + local_FC;
  return Script_PlayerStatsUtils(PS_SetShugyoCourseActiveFramesSlot, 0, local_F8);
}

__attribute__((scn_string_bindings(0, data_2A27A, 3, data_2A294, 7, data_2A2B8)))
static int32_t script_1F71E(void) {
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
      goto L01F81E;
    }
  }
  L01F81E:;
  if (local_F4) {
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_LoadVoiceFile(0, "E1040");
  }
  if ((local_FC >= 1501) & (local_FC <= 1601)) {
    local_F4 = 1;
  }
  if ((local_F8 >= 2200) | (local_F8 <= 829)) {
    local_F4 = 1;
  }
  return local_F4;
}

static int32_t script_1F877(void) {
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

static void script_1F91D(void) {
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

static void script_1F976(void) {
  Script_seqEnterEvtCam(2900, CHARA_AKIR, CHARA_AKIR);
  Script_shenClearFreeState(0);
  for (;;) {
    SCNF_WaitFrames_0000(1);
    if (Script_IsInEventCam() == 0) {
      break;
    }
  }
  Script_shenClearFreeState(1);
  return;
}

static int32_t script_1F9B0(int32_t arg0) {
  Script_EVT_SetPosition(arg0, 0x78000000, &data_2A2C0);
  Script_charaDisplay(arg0, CDM_Hide);
  return Script_ccowUtils(arg0, 2);
}

static void script_1F9D3(void) {
  SCNF_WaitFrames_0000(2);
  while (Script_IsInEventCam() != 0) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static int32_t script_1F9F3(void) {
  Script_LoadSND(3, 0, 0, 0);
  Script_LoadSND(1, 5, "e1shugyo.snd", 0x8000);
  while (Script_LoadSND(2, 0, 0, 0)) {
    SCNF_WaitFrames_0000(1);
  }
  return Script_LoadSND(4, 0, 0, 0);
}

static void script_1FA3E(void) {
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

static void script_1FA85(void) {
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

static void script_1FAD8(void) {
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

static void script_1FB34(void) {
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

static void script_1FB7B(void) {
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

__attribute__((scn_string_bindings(0, data_2A351)))
static void script_1FBD7(void) {
  Script_SetCPAMFlags(CHARA_AKIR, 0);
  Script_ryoControl(PCCTL_RequestNewMode, 5);
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_LoadVoiceFile(1, "E1040");
  while (Script_ryoControl(PCCTL_QueryRequestIdle) == 0) {
    SCNF_WaitFrames_0000(1);
  }
  Script_scenCreateTask(script_1F9B0, 1, SHEN_NAME('LUV2'));
  Script_FaceEx(CHARA_AKIR, 1, 0, 1);
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 1);
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
  Script_CycleManagerUtils(CYCLE_CMD_PROCESS_SND_EXTRA);
  Script_scenCreateTask(script_1F9F3, 0);
  return;
}

__attribute__((scn_string_bindings(0, data_2A357)))
static void script_1FC5F(void) {
  Script_vfToggleBattleState(scnPostResultsSetup);
  while (SCN_GetIsFighting() != 2) {
    SCNF_WaitFrames_0000(1);
  }
  Script_LoadVoiceFile(0, "E1040");
  SCNF_WaitFrames_0000(1);
  while (Script_GetConversationDetails(CONV_DETAILS_MODE_GET_GAME_STATE, 0) != 7) {
    SCNF_WaitFrames_0000(1);
  }
  Script_motmToggleFlags(CHARA_AKIR, 1, 0x40);
  Script_FaceEx(CHARA_AKIR, 0, 0, 1);
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 2);
  return;
}

static void script_1FCD7(void) {
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

static void script_1FD14(void) {
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

static int32_t script_1FD51(int32_t arg0) {
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

static void script_1FDC2(int32_t arg0) {
  Vector3i local_F4;
  Script_uiUtils(0, 15, 0);
  Script_vfSetup(0, 20, 20, 0, 0);
  Script_vfStageObjectUtils(0, 0, 0);
  Script_vfStageObjectUtils(2, 0, 2, 2, 0x3F000000, 0x3F000000, -0x40800000, 0, 0, -0x40800000);
  Script_vfStageObjectUtils(7, 0, 0x4F4B4148, 0);
  Script_vfStageObjectUtils(0, 1, 0);
  Script_vfStageObjectUtils(2, 1, 2, 2, 0x3F800000, 0x3E800000, -0x40000000, 0, 0, -0x41000000);
  Script_vfStageObjectUtils(7, 1, 0x534B484B, 1);
  Script_vfStageObjectUtils(13);
  task_2AC = 0x52494B41;
  *(&task_2AC + 1) = 0;
  *(&task_2AC + 2) = 0;
  *(&task_2AC + 3) = 0;
  *(&task_2AC + 4) = 5;
  *(&task_2AC + 5) = 0;
  *(&task_2AC + 6) = 0;
  *(&task_2AC + 7) = 0;
  *(&task_2AC + 8) = 0;
  *(&task_2AC + 9) = 0;
  *(&task_2AC + 10) = 0;
  Script_vfUtils(&task_2AC);
  task_2AC = 0x3256554C;
  *(&task_2AC + 1) = 0;
  *(&task_2AC + 2) = 1;
  *(&task_2AC + 3) = 1;
  *(&task_2AC + 4) = 5;
  *(&task_2AC + 5) = 21;
  *(&task_2AC + 6) = 0;
  *(&task_2AC + 7) = 0;
  *(&task_2AC + 8) = 0;
  *(&task_2AC + 9) = 0;
  *(&task_2AC + 10) = 0;
  Script_vfUtils(&task_2AC);
  Script_vfConfigureFormationGroup(SHEN_NAME('LUV2'), VF_FORMATION_APPLY_FLAGS, 0x200000, 0x800);
  Script_shenClearFreeState(0);
  Script_filmCheckOrSetTransitionUtil(15, 0, 0, 0, 0, 255, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_FaceEx(CHARA_AKIR, 0, 0, 1);
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 2);
  Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &data_2A360);
  (&local_F4)->y = 0;
  Script_EVT_SetRotation(CHARA_AKIR, MATRIX_XFORM_Y, &local_F4, 0);
  SCNF_WaitFrames_0000(1);
  Script_seqEnterEvtCam(2903, CHARA_AKIR, CHARA_AKIR);
  SCNF_WaitFrames_0000(1);
  script_1F9D3();
  SCNF_WaitFrames_0000(1);
  SCN_Enqueue_VFGI(2, 99, 1.0f, 4, -1);
  Script_filmCheckOrSetTransitionUtil(15, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_shenClearFreeState(1);
  Script_scenCreateTask(script_1F6C3, 1, arg0);
  return;
}

static int32_t script_2005C(int32_t arg0) {
  int32_t local_FC;
  local_FC = 0;
  if (Script_GetFightingStats(-1, 2) == 1) {
    local_FC = 1;
  }
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_EV_STOP_ACTIVE) == 1) {
    local_FC = 2;
  }
  if (script_1F877()) {
    local_FC = 2;
  }
  return local_FC;
}

static void script_200A4(void) {
  script_1FB34();
  return;
}

static void script_200AC(void) {
  SCN_Enqueue_VFGI(2, 99, 1.0f, 99, -1);
  return;
}

static void script_200C0(int8_t arg0) {
  int8_t local_FF;
  int8_t local_FE;
  int32_t local_F8;
  local_FE = 0;
  local_FF = 1;
  if (~Script_IsPlayerAllowed(0)) {
    local_FE = 1;
  }
  if (Script_CycleManagerUtils(CYCLE_CMD_GET_NEAR_NPC_COUNT_CLAMPED) > 0) {
    local_FE = 1;
  }
  if (script_1F71E()) {
    local_FE = 1;
  }
  if (local_FE == 0) {
    script_1FBD7();
    local_FF = 1;
  }
  for (;;) {
    script_1FCD7();
    script_1FD14();
    if (local_FE == 1) {
      break;
      local_FF = 14;
    }
    SCNF_WaitFrames_0000(1);
    local_F8 = local_FF;
    if (local_F8 == 0) {
      local_FF = 1;
    } else if (local_F8 == 1) {
      script_1FA85();
      local_FF = 4;
    } else if ((local_F8 == 3) | (local_F8 == 13) | (local_F8 == 14)) {
      break;
    } else if ((local_F8 == 4) | (local_F8 == 12)) {
      local_FF = local_FF + script_1FD51(local_FF);
    } else if (local_F8 == 5) {
      local_FE = 2;
      script_1FAD8();
      script_1FDC2(0);
      local_FF = 6;
    } else if (local_F8 == 6) {
      local_FF = local_FF + script_2005C(arg0);
    } else if ((local_F8 == 7) | (local_F8 == 8)) {
      local_FF = local_FF - 7 + 9;
    } else {
      switch (local_F8) {
        case 9:
          if (Script_GetFightingStats(-1, 2) == 1) {
            script_200A4();
            local_FF = 12;
          }
          break;
        case 10:
          if (Script_GetFightingStats(-1, 2) == 1) {
            script_200A4();
            local_FF = 14;
          }
          break;
        case 11:
          script_200AC();
          local_FF = 6;
          break;
      }
    }
  }
  if (local_FE != 1) {
    script_1FC5F();
  }
  if (local_FE == 2) {
    script_1F91D();
    Script_PlayerStatsUtils(PS_SetShugyoCourseClearCountSlot, 0, Script_PlayerStatsUtils(PS_GetShugyoCourseClearCountSlot, 0) + 1);
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

__attribute__((scn_string_bindings(0, data_2A36C, 1, data_2A376, 2, data_2A380)))
static int32_t script_20323(void) {
  int32_t local_F4[3];
  int32_t local_E8[3];
  local_F4[0] = -0x3ECCD220;
  local_F4[1] = 0x3F666666;
  local_F4[2] = -0x3E7CE83E;
  local_E8[0] = -0x3F9ED289;
  local_E8[1] = 0x3F99999A;
  local_E8[2] = -0x3E1B7B16;
  for (;;) {
    if ((task_22D == 0) & (script_2045D(0) == 1)) {
    } else {
      SCNF_WaitFrames_0000(1);
      continue;
    }
    Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
    while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_CycleManagerUtils(CYCLE_CMD_PROCESS_SND_EXTRA);
    task_228 = 1;
    Script_scenToggleCharaFlag(0, 200, 1);
    if (Script_scenToggleVisibleFor(SHEN_NAME('LUV2'), -1) == 0) {
      Script_scenToggleVisibleFor(SHEN_NAME('LUV2'), 1);
    }
    Script_GetBlurModel(CHARA_AKIR, "YGB_B.MT5");
    script_200C0(0);
    Script_GetBlurModel(CHARA_AKIR, "YKA_B.MT5");
    if (Script_scenToggleVisibleFor(SHEN_NAME('LUV2'), -1)) {
      Script_scenToggleVisibleFor(SHEN_NAME('LUV2'), 0);
    }
    Script_scenToggleCharaFlag(0, 200, 0);
    task_228 = 0;
    Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 1);
    Script_LoadSND(0, 5, "battle_1.snd", 0);
  }
  return Script_srchControl(0);
}

static int8_t script_2045D(int8_t arg0) {
  int8_t local_FF;
  float local_F8;
  int32_t local_F4;
  int8_t local_F3;
  int32_t local_EC;
  local_FF = 0;
  Script_EVT_GetPosition(CHARA_AKIR, -1, &task_29C, ENTITY_XFORM_WORLD_SPACE);
  local_EC = arg0;
  if (local_EC == 0) {
    task_290.x = -7.5f;
    task_290.y = 0.0f;
    task_290.z = -22.5f;
    local_F8 = 2.0f;
    local_F4 = 1;
    goto L0204BA;
  }
  L0204BA:;
  local_F3 = 0;
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, 0x554B5546)) {
    if (Script_SetTaskDataValue(CHARA_FUKU, &data_2A390, &data_2A39C, 0)) {
      local_F3 = 1;
    }
  }
  if (!(Script_MathUtils(SCRIPT_MATH_TASK_VEC3_DISTANCE, &task_290, &task_29C) >= local_F8) & (local_F3 == 0)) {
    if (task_234 == 0) {
      (void)((task_234 = -1) & 1);
      local_FF = local_F4;
    }
  } else {
    if (!(task_234 == -1)) goto L02052E;
  }
  L02052E:;
  return local_FF;
}

static void script_20532(void) {
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_WALK_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_LineWalkUtils(LNWK_SCRIPT_BEGIN_PATH_WALK, CHARA_AKIR, &data_2A3A8, 0, 0);
  while (Script_LineWalkUtils(LNWK_SCRIPT_SET_MOVE_SCALE_GET_CMD, CHARA_AKIR, 0.05f) != -1) {
    SCNF_WaitFrames_0000(1);
  }
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  return;
}

static void script_205B5(int32_t arg0) {
  int32_t local_FC;
  int32_t unused_F8;
  int32_t local_F4;
  int32_t local_F0;
  int32_t local_EC;
  int32_t local_E8;
  int32_t local_E4;
  float local_E0;
  float local_DC;
  float local_D8;
  float local_D4;
  float local_D0;
  float local_CC;
  float local_C4[2];
  int32_t unused_B8[3];
  int32_t local_B4;
  Script_executeAicaCommand(2720, 2, 30);
  Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_LoadSND(0, 2, "bgm109.snd", 0);
  Script_executeAicaCommand(16296, 0, 0);
  Script_executeAicaCommand(1184, 2, 93);
  script_20BE9();
  script_D9C0();
  script_2098F();
  Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
  Script_lightSetSlotEnabled(1, 0);
  Script_lightSetSlotEnabled(8, 0);
  Script_lightSetSlotEnabled(10, 0);
  Script_lightSetSlotEnabled(5, 0);
  script_20A62();
  local_C4[0] = 0.0f;
  local_C4[1] = 0.0f;
  Script_LightUtils(3, 6, (const float *)local_C4);
  Script_LightUtils(2, 6, (const float *)local_C4);
  Script_LightUtils(6, 6, (const float *)local_C4);
  Script_LightUtils(7, 6, (const float *)local_C4);
  Script_LightUtils(8, 6, (const float *)local_C4);
  Script_shenSeqUtils(0);
  local_F4 = 0;
  local_F0 = 0;
  local_EC = 0;
  local_E8 = 0;
  local_E4 = 0;
  local_E0 = 0.0f;
  local_DC = 0.0f;
  local_D8 = 0.1725f;
  local_D4 = 0.331818193f;
  local_D4 = 0.0f;
  local_D0 = 0.0f;
  local_FC = 0;
  while (Script_shenSeqUtils(-1)) {
    local_B4 = local_FC;
    if (local_B4 == 100) {
    } else if (local_B4 == 0) {
      local_F4 = 1;
      local_E8 = 1;
    } else {
      switch (local_B4) {
        case 0:
          local_E8 = 1;
          break;
        case 76:
          local_F0 = 1;
          break;
        case 80:
          local_E4 = 1;
          break;
        case 165:
          local_EC = 1;
          break;
        case 130:
          script_21992();
          break;
        case 220:
          script_2103D();
          break;
        case 300:
          Script_executeAicaCommand(2720, 2, 30);
          Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
          break;
      }
    }
    SCNF_WaitFrames_0000(1);
    local_FC = local_FC + 1;
    if (!(local_D8 >= 0.69f) & (local_F4 == 1)) {
      local_D8 = local_D8 + 0.00575f;
      local_C4[0] = local_D8;
      local_C4[1] = local_D8;
      Script_LightUtils(2, 6, (const float *)local_C4);
    }
    if (!(local_D4 >= 0.4f) & (local_E8 == 1)) {
      local_D4 = local_D4 + 0.00363636366f;
      local_C4[0] = local_D4;
      local_C4[1] = local_D4;
      Script_LightUtils(7, 6, (const float *)local_C4);
    }
    if (!(local_DC >= 0.48f) & (local_F0 == 1)) {
      local_DC = local_DC + 0.00399999972f;
      local_C4[0] = local_DC;
      local_C4[1] = local_DC;
      Script_LightUtils(6, 6, (const float *)local_C4);
    } else if (!(local_DC >= 0.6f) & (local_F0 == 1)) {
      local_DC = local_DC + 0.00500000035f;
      local_C4[0] = local_DC;
      local_C4[1] = local_DC;
      Script_LightUtils(6, 6, (const float *)local_C4);
    } else {
      if (local_F0 == 1) {
        local_CC = Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 0.1f);
        local_DC = 4.8f;
        local_DC = local_DC - local_CC;
        Script_LightUtils(6, 5, local_DC);
        goto L0208ED;
      }
    }
    L0208ED:;
    if (!(local_D0 >= 0.58f) & (local_E4 == 1)) {
      local_D0 = local_D0 + 0.00483333319f;
      local_C4[0] = local_D0;
      local_C4[1] = local_D0;
      Script_LightUtils(8, 6, (const float *)local_C4);
    }
    if (!(local_E0 >= 0.71f) & (local_EC == 1)) {
      local_E0 = local_E0 + 0.00709999958f;
      local_C4[0] = local_E0;
      local_C4[1] = local_E0;
      Script_LightUtils(3, 6, (const float *)local_C4);
    }
  }
  script_209DA();
  script_21588();
  Script_LoadSND(0, 2, data_2A3C7, 0);
  Script_executeAicaCommand(16296, 0, 0);
  Script_executeAicaCommand(1184, 2, 100);
  return;
}

static void script_2098F(void) {
  int32_t unused_E8[6];
  Script_LODSnapshotUtils(DISP_CMD_SaveDistancesAndCaptureLodState);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 50.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 50.0f);
  Script_SetGlobalSceneSlotValue34(1, 0);
  Script_SetGlobalSceneSlotValue34(3, 0);
  Script_PREP_Related(-1, 0);
  Script_SetupLightFromTable(8);
  Script_mapmSetCamFlags(0, 0);
  return;
}

static void script_209DA(void) {
  script_DA65(CHARA_AKIR);
  Script_SetGameStateTo(AUTH_None);
  Script_LODSnapshotUtils(DISP_CMD_RestoreDistancesAndApplyLodState);
  return;
}

static void script_209F6(int32_t arg0, int32_t arg1) {
  float local_FC;
  int16_t local_FA;
  local_FA = 0;
  while (1) {
    if (local_FA <= arg1) {
      local_FC = Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 1.2f);
      if (!(local_FC >= 1.0f)) {
        Script_lightSetSlotEnabled(arg0, 0);
      } else {
        Script_lightSetSlotEnabled(arg0, 1);
      }
    } else {
      Script_lightSetSlotEnabled(arg0, 1);
      Script_srchControl(0);
    }
    local_FA = local_FA + 1;
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_20A62(void) {
  Vector4 local_F0;
  Vector3 local_E4;
  int32_t local_DC[2];
  local_E4.x = 8.05f;
  (&local_E4)->y = 1.42f;
  (&local_E4)->z = 11.56f;
  Script_lightEnablePointLightAt(6, &local_E4);
  local_F0.x = 1.0f;
  (&local_F0)->y = 0.83f;
  (&local_F0)->z = 0.53f;
  (&local_F0)->w = 0.39f;
  Script_LightUtils(6, 2, &local_F0);
  Script_LightUtils(6, 4, 0.8f);
  Script_LightUtils(6, 5, 4.8f);
  local_DC[0] = 0x3F3AE148;
  local_DC[1] = 0x3F3AE148;
  Script_LightUtils(6, 6, (const float *)local_DC);
  Script_lightSetSlotEnabled(6, 1);
  return;
}

static void script_20B06(void) {
  float local_FC;
  float local_F8;
  float local_F4;
  float local_F0;
  float local_EC;
  Vector3i local_E0;
  local_F8 = 0.0f;
  local_F4 = 0.0f;
  local_F0 = 0.0f;
  local_EC = 0.0f;
  local_FC = 357.0f;
  while (1) {
    if (!(local_F8 > 2.0f)) {
      local_F4 = local_F4 + 0.003f;
      local_F8 = local_F8 + local_F4;
    }
    if (!(local_FC > 358.0f)) {
      if (!(local_F8 <= local_F0)) {
        local_EC = local_EC + 0.001f;
      } else {
        local_EC = local_EC + 0.008f;
      }
      local_F0 = local_F0 + local_EC;
      local_FC = local_FC - local_F8;
      local_FC = local_FC + local_F0;
      local_E0.x = 0;
      (&local_E0)->y = 6006;
      (&local_E0)->z = (int32_t)(local_FC / 360.0f * (float)0x10000);
      Script_EVT_SetRotation(SHEN_NAME('SSOD'), 0x38000000, &local_E0, 0);
    } else {
      Script_srchControl(0);
    }
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_20BE9(void) {
  if (-1) {
    Script_charaDisplay(CHARA_AKIR, CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(CHARA_AKIR, CDM_Show);
      goto L020C13;
    }
  }
  L020C13:;
  Script_scenToggleVisibleFor(CHARA_AKIR, 0);
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, 0x554B5546)) {
    if (-1) {
      Script_charaDisplay(CHARA_FUKU, CDM_Hide);
    } else {
      if (0) {
        Script_charaDisplay(CHARA_FUKU, CDM_Show);
        goto L020C56;
      }
    }
    L020C56:;
    Script_scenToggleVisibleFor(CHARA_FUKU, 0);
  }
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, 0x5F454E49)) {
    if (-1) {
      Script_charaDisplay(CHARA_INE_, CDM_Hide);
    } else {
      if (0) {
        Script_charaDisplay(CHARA_INE_, CDM_Show);
        goto L020C99;
      }
    }
    L020C99:;
    Script_scenToggleVisibleFor(CHARA_INE_, 0);
  }
  if (-1) {
    Script_charaDisplay(SHEN_NAME('TUBA'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('TUBA'), CDM_Show);
      goto L020CCD;
    }
  }
  L020CCD:;
  Script_scenToggleVisibleFor(SHEN_NAME('TUBA'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('GART'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('GART'), CDM_Show);
      goto L020D01;
    }
  }
  L020D01:;
  Script_scenToggleVisibleFor(SHEN_NAME('GART'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('KAK3'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('KAK3'), CDM_Show);
      goto L020D35;
    }
  }
  L020D35:;
  Script_scenToggleVisibleFor(SHEN_NAME('KAK3'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('KAK4'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('KAK4'), CDM_Show);
      goto L020D69;
    }
  }
  L020D69:;
  Script_scenToggleVisibleFor(SHEN_NAME('KAK4'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('HOKI'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('HOKI'), CDM_Show);
      goto L020D9D;
    }
  }
  L020D9D:;
  Script_scenToggleVisibleFor(SHEN_NAME('HOKI'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('MOON'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('MOON'), CDM_Show);
      goto L020DD1;
    }
  }
  L020DD1:;
  Script_scenToggleVisibleFor(SHEN_NAME('MOON'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('GRW1'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('GRW1'), CDM_Show);
      goto L020E05;
    }
  }
  L020E05:;
  Script_scenToggleVisibleFor(SHEN_NAME('GRW1'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('GRW2'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('GRW2'), CDM_Show);
      goto L020E39;
    }
  }
  L020E39:;
  Script_scenToggleVisibleFor(SHEN_NAME('GRW2'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('GRW3'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('GRW3'), CDM_Show);
      goto L020E6D;
    }
  }
  L020E6D:;
  Script_scenToggleVisibleFor(SHEN_NAME('GRW3'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('GRW4'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('GRW4'), CDM_Show);
      goto L020EA1;
    }
  }
  L020EA1:;
  Script_scenToggleVisibleFor(SHEN_NAME('GRW4'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('HAKO'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('HAKO'), CDM_Show);
      goto L020ED5;
    }
  }
  L020ED5:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAKO'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('PONK'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('PONK'), CDM_Show);
      goto L020F09;
    }
  }
  L020F09:;
  Script_scenToggleVisibleFor(SHEN_NAME('PONK'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('PONS'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('PONS'), CDM_Show);
      goto L020F3D;
    }
  }
  L020F3D:;
  Script_scenToggleVisibleFor(SHEN_NAME('PONS'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('POK2'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('POK2'), CDM_Show);
      goto L020F71;
    }
  }
  L020F71:;
  Script_scenToggleVisibleFor(SHEN_NAME('POK2'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('POS2'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('POS2'), CDM_Show);
      goto L020FA5;
    }
  }
  L020FA5:;
  Script_scenToggleVisibleFor(SHEN_NAME('POS2'), 0);
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, 0x52554B59)) {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('YKUR'), CDM_Hide);
    } else {
      if (0) {
        Script_charaDisplay(SHEN_NAME('YKUR'), CDM_Show);
        goto L020FE8;
      }
    }
    L020FE8:;
    Script_scenToggleVisibleFor(SHEN_NAME('YKUR'), 0);
  }
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, 0x4C554B59)) {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('YKUL'), CDM_Hide);
    } else {
      if (0) {
        Script_charaDisplay(SHEN_NAME('YKUL'), CDM_Show);
        goto L02102B;
      }
    }
    L02102B:;
    Script_scenToggleVisibleFor(SHEN_NAME('YKUL'), 0);
  }
  script_2103D();
  return;
}

static void script_2103D(void) {
  if (-1) {
    Script_charaDisplay(SHEN_NAME('KOI0'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('KOI0'), CDM_Show);
      goto L021067;
    }
  }
  L021067:;
  Script_scenToggleVisibleFor(SHEN_NAME('KOI0'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('KOI1'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('KOI1'), CDM_Show);
      goto L02109B;
    }
  }
  L02109B:;
  Script_scenToggleVisibleFor(SHEN_NAME('KOI1'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('KOI2'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('KOI2'), CDM_Show);
      goto L0210CF;
    }
  }
  L0210CF:;
  Script_scenToggleVisibleFor(SHEN_NAME('KOI2'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('KOI3'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('KOI3'), CDM_Show);
      goto L021103;
    }
  }
  L021103:;
  Script_scenToggleVisibleFor(SHEN_NAME('KOI3'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('KOI4'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('KOI4'), CDM_Show);
      goto L021137;
    }
  }
  L021137:;
  Script_scenToggleVisibleFor(SHEN_NAME('KOI4'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('HAS0'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('HAS0'), CDM_Show);
      goto L02116B;
    }
  }
  L02116B:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAS0'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('HAS1'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('HAS1'), CDM_Show);
      goto L02119F;
    }
  }
  L02119F:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAS1'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('HAS2'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('HAS2'), CDM_Show);
      goto L0211D3;
    }
  }
  L0211D3:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAS2'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('HAS3'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('HAS3'), CDM_Show);
      goto L021207;
    }
  }
  L021207:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAS3'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('HAS4'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('HAS4'), CDM_Show);
      goto L02123B;
    }
  }
  L02123B:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAS4'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('HAS5'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('HAS5'), CDM_Show);
      goto L02126F;
    }
  }
  L02126F:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAS5'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('HAS6'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('HAS6'), CDM_Show);
      goto L0212A3;
    }
  }
  L0212A3:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAS6'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('HAS7'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('HAS7'), CDM_Show);
      goto L0212D7;
    }
  }
  L0212D7:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAS7'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('HAS8'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('HAS8'), CDM_Show);
      goto L02130B;
    }
  }
  L02130B:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAS8'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('HAS9'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('HAS9'), CDM_Show);
      goto L02133F;
    }
  }
  L02133F:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAS9'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('HASA'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('HASA'), CDM_Show);
      goto L021373;
    }
  }
  L021373:;
  Script_scenToggleVisibleFor(SHEN_NAME('HASA'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('HASB'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('HASB'), CDM_Show);
      goto L0213A7;
    }
  }
  L0213A7:;
  Script_scenToggleVisibleFor(SHEN_NAME('HASB'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('HASC'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('HASC'), CDM_Show);
      goto L0213DB;
    }
  }
  L0213DB:;
  Script_scenToggleVisibleFor(SHEN_NAME('HASC'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('HASD'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('HASD'), CDM_Show);
      goto L02140F;
    }
  }
  L02140F:;
  Script_scenToggleVisibleFor(SHEN_NAME('HASD'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('HASE'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('HASE'), CDM_Show);
      goto L021443;
    }
  }
  L021443:;
  Script_scenToggleVisibleFor(SHEN_NAME('HASE'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('HASF'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('HASF'), CDM_Show);
      goto L021477;
    }
  }
  L021477:;
  Script_scenToggleVisibleFor(SHEN_NAME('HASF'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('HAN0'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('HAN0'), CDM_Show);
      goto L0214AB;
    }
  }
  L0214AB:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAN0'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('HAN1'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('HAN1'), CDM_Show);
      goto L0214DF;
    }
  }
  L0214DF:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAN1'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('HAN2'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('HAN2'), CDM_Show);
      goto L021513;
    }
  }
  L021513:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAN2'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('HAN3'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('HAN3'), CDM_Show);
      goto L021547;
    }
  }
  L021547:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAN3'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('HAN4'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('HAN4'), CDM_Show);
      goto L02157B;
    }
  }
  L02157B:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAN4'), 0);
  return;
}

static void script_21588(void) {
  if (0) {
    Script_charaDisplay(CHARA_AKIR, CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(CHARA_AKIR, CDM_Show);
      goto L0215B2;
    }
  }
  L0215B2:;
  Script_scenToggleVisibleFor(CHARA_AKIR, 1);
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, 0x554B5546)) {
    if (0) {
      Script_charaDisplay(CHARA_FUKU, CDM_Hide);
    } else {
      if (-1) {
        Script_charaDisplay(CHARA_FUKU, CDM_Show);
        goto L0215F5;
      }
    }
    L0215F5:;
    Script_scenToggleVisibleFor(CHARA_FUKU, 1);
  }
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, 0x5F454E49)) {
    if (-1) {
      Script_charaDisplay(CHARA_INE_, CDM_Hide);
    } else {
      if (0) {
        Script_charaDisplay(CHARA_INE_, CDM_Show);
        goto L021638;
      }
    }
    L021638:;
    Script_scenToggleVisibleFor(CHARA_INE_, 0);
  }
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, 0x5F454E49)) {
    Script_scenToggleVisibleFor(CHARA_INE_, 1);
  }
  Script_scenToggleVisibleFor(SHEN_NAME('TUBA'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('GART'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('GART'), CDM_Show);
      goto L021693;
    }
  }
  L021693:;
  Script_scenToggleVisibleFor(SHEN_NAME('GART'), 1);
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_KAK1_HANGING_SCROLL_TAKEN) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_PROMISED_TERRY_TO_FIGHT_CHEN_GUI_ZHANG) == 0)) {
    if (0) {
      Script_charaDisplay(SHEN_NAME('KAK3'), CDM_Hide);
    } else {
      if (-1) {
        Script_charaDisplay(SHEN_NAME('KAK3'), CDM_Show);
        goto L0216E3;
      }
    }
    L0216E3:;
    Script_scenToggleVisibleFor(SHEN_NAME('KAK3'), 1);
  } else {
    Script_scenToggleVisibleFor(SHEN_NAME('KAK3'), 1);
  }
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_KAK2_HANGING_SCROLL_TAKEN) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_PROMISED_TERRY_TO_FIGHT_CHEN_GUI_ZHANG) == 0)) {
    if (0) {
      Script_charaDisplay(SHEN_NAME('KAK4'), CDM_Hide);
    } else {
      if (-1) {
        Script_charaDisplay(SHEN_NAME('KAK4'), CDM_Show);
        goto L021742;
      }
    }
    L021742:;
    Script_scenToggleVisibleFor(SHEN_NAME('KAK4'), 1);
  } else {
    Script_scenToggleVisibleFor(SHEN_NAME('KAK4'), 1);
  }
  Script_scenToggleVisibleFor(SHEN_NAME('HOKI'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('GRW1'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('GRW1'), CDM_Show);
      goto L021791;
    }
  }
  L021791:;
  Script_scenToggleVisibleFor(SHEN_NAME('GRW1'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('GRW2'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('GRW2'), CDM_Show);
      goto L0217C5;
    }
  }
  L0217C5:;
  Script_scenToggleVisibleFor(SHEN_NAME('GRW2'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('GRW3'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('GRW3'), CDM_Show);
      goto L0217F9;
    }
  }
  L0217F9:;
  Script_scenToggleVisibleFor(SHEN_NAME('GRW3'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('GRW4'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('GRW4'), CDM_Show);
      goto L02182D;
    }
  }
  L02182D:;
  Script_scenToggleVisibleFor(SHEN_NAME('GRW4'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('HAKO'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('HAKO'), CDM_Show);
      goto L021861;
    }
  }
  L021861:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAKO'), 1);
  Script_scenToggleVisibleFor(SHEN_NAME('PONK'), 1);
  Script_scenToggleVisibleFor(SHEN_NAME('PONS'), 1);
  if ((Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_KATANA_INSERTED) | Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_DOJO_SWORD_DISPLAYED) | task_22D) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_PROMISED_TERRY_TO_FIGHT_CHEN_GUI_ZHANG) == 0)) {
    if (0) {
      Script_charaDisplay(SHEN_NAME('POK2'), CDM_Hide);
    } else {
      if (-1) {
        Script_charaDisplay(SHEN_NAME('POK2'), CDM_Show);
        goto L0218D8;
      }
    }
    L0218D8:;
    Script_scenToggleVisibleFor(SHEN_NAME('POK2'), 1);
  } else {
    Script_scenToggleVisibleFor(SHEN_NAME('POK2'), 1);
  }
  if ((Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_DOJO_SWORD_DISPLAYED) | task_22D) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_PROMISED_TERRY_TO_FIGHT_CHEN_GUI_ZHANG) == 0)) {
    if (0) {
      Script_charaDisplay(SHEN_NAME('POS2'), CDM_Hide);
    } else {
      if (-1) {
        Script_charaDisplay(SHEN_NAME('POS2'), CDM_Show);
        goto L02193B;
      }
    }
    L02193B:;
    Script_scenToggleVisibleFor(SHEN_NAME('POS2'), 1);
  } else {
    Script_scenToggleVisibleFor(SHEN_NAME('POS2'), 1);
  }
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, 0x52554B59)) {
    Script_scenToggleVisibleFor(SHEN_NAME('YKUR'), 1);
  }
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, 0x4C554B59)) {
    Script_scenToggleVisibleFor(SHEN_NAME('YKUL'), 1);
  }
  script_21992();
  return;
}

static void script_21992(void) {
  if (0) {
    Script_charaDisplay(SHEN_NAME('KOI0'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('KOI0'), CDM_Show);
      goto L0219BC;
    }
  }
  L0219BC:;
  Script_scenToggleVisibleFor(SHEN_NAME('KOI0'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('KOI1'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('KOI1'), CDM_Show);
      goto L0219F0;
    }
  }
  L0219F0:;
  Script_scenToggleVisibleFor(SHEN_NAME('KOI1'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('KOI2'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('KOI2'), CDM_Show);
      goto L021A24;
    }
  }
  L021A24:;
  Script_scenToggleVisibleFor(SHEN_NAME('KOI2'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('KOI3'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('KOI3'), CDM_Show);
      goto L021A58;
    }
  }
  L021A58:;
  Script_scenToggleVisibleFor(SHEN_NAME('KOI3'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('KOI4'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('KOI4'), CDM_Show);
      goto L021A8C;
    }
  }
  L021A8C:;
  Script_scenToggleVisibleFor(SHEN_NAME('KOI4'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('HAS0'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('HAS0'), CDM_Show);
      goto L021AC0;
    }
  }
  L021AC0:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAS0'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('HAS1'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('HAS1'), CDM_Show);
      goto L021AF4;
    }
  }
  L021AF4:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAS1'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('HAS2'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('HAS2'), CDM_Show);
      goto L021B28;
    }
  }
  L021B28:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAS2'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('HAS3'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('HAS3'), CDM_Show);
      goto L021B5C;
    }
  }
  L021B5C:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAS3'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('HAS4'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('HAS4'), CDM_Show);
      goto L021B90;
    }
  }
  L021B90:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAS4'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('HAS5'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('HAS5'), CDM_Show);
      goto L021BC4;
    }
  }
  L021BC4:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAS5'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('HAS6'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('HAS6'), CDM_Show);
      goto L021BF8;
    }
  }
  L021BF8:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAS6'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('HAS7'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('HAS7'), CDM_Show);
      goto L021C2C;
    }
  }
  L021C2C:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAS7'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('HAS8'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('HAS8'), CDM_Show);
      goto L021C60;
    }
  }
  L021C60:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAS8'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('HAS9'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('HAS9'), CDM_Show);
      goto L021C94;
    }
  }
  L021C94:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAS9'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('HASA'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('HASA'), CDM_Show);
      goto L021CC8;
    }
  }
  L021CC8:;
  Script_scenToggleVisibleFor(SHEN_NAME('HASA'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('HASB'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('HASB'), CDM_Show);
      goto L021CFC;
    }
  }
  L021CFC:;
  Script_scenToggleVisibleFor(SHEN_NAME('HASB'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('HASC'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('HASC'), CDM_Show);
      goto L021D30;
    }
  }
  L021D30:;
  Script_scenToggleVisibleFor(SHEN_NAME('HASC'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('HASD'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('HASD'), CDM_Show);
      goto L021D64;
    }
  }
  L021D64:;
  Script_scenToggleVisibleFor(SHEN_NAME('HASD'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('HASE'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('HASE'), CDM_Show);
      goto L021D98;
    }
  }
  L021D98:;
  Script_scenToggleVisibleFor(SHEN_NAME('HASE'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('HASF'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('HASF'), CDM_Show);
      goto L021DCC;
    }
  }
  L021DCC:;
  Script_scenToggleVisibleFor(SHEN_NAME('HASF'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('HAN0'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('HAN0'), CDM_Show);
      goto L021E00;
    }
  }
  L021E00:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAN0'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('HAN1'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('HAN1'), CDM_Show);
      goto L021E34;
    }
  }
  L021E34:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAN1'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('HAN2'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('HAN2'), CDM_Show);
      goto L021E68;
    }
  }
  L021E68:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAN2'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('HAN3'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('HAN3'), CDM_Show);
      goto L021E9C;
    }
  }
  L021E9C:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAN3'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('HAN4'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('HAN4'), CDM_Show);
      goto L021ED0;
    }
  }
  L021ED0:;
  Script_scenToggleVisibleFor(SHEN_NAME('HAN4'), 1);
  return;
}

static void script_21EDD(void) {
  int32_t local_FC;
  local_FC = Script_GetSceneID();
  switch (local_FC) {
    case 1:
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_ENTRY2_TIMED_EVENT_DONE) == 0) {
        Script_TimedTransitionUtils(TIMED_TRANSITION_REGISTER, 1, 0x3044484A, 2, &data_2A464, 0xD5A6C3, TimedTransitionUpdateMemoItem150);
      }
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JD00_ENTRY20_TIMED_EVENT_DONE) == 0) {
        Script_TimedTransitionUtils(TIMED_TRANSITION_REGISTER, 1, 0x3030444A, 20, &data_2A42C, 0xDA3AA3, 0);
      }
      break;
    case 2:
      if ((Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_GMCT_ROUTE_STARTED) == 0) | (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_GMCT_ROUTE_ADVANCED) == 0)) {
        Script_TimedTransitionUtils(TIMED_TRANSITION_REGISTER, 2, 0x54434D47, 1, &data_2A3F4, 0xB7420F, 0);
      }
      break;
    case 3:
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOZOMI_RESCUE_RUSH_ACTIVE) == 0) {
        Script_TimedTransitionUtils(TIMED_TRANSITION_REGISTER, 3, 0x4F4D4F4A, 20, &data_2A410, 0x2DC9FE, TimedTransitionRollbackToPreviousDay2330);
      }
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_FIRED_FROM_JOB) & ~Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 970)) {
        if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_MFSY_ENTRY42_TIMED_EVENT_DONE) == 0) {
          Script_TimedTransitionUtils(TIMED_TRANSITION_REGISTER, 3, 0x5953464D, 42, &data_2A448, 0xBBB817, 0);
        }
      }
      break;
  }
  return;
}

static void script_22015(int32_t arg0) {
  script_2202D(arg0);
  return;
}

static void script_22021(int32_t arg0) {
  script_2202D(arg0);
  return;
}

static void script_2202D(int32_t arg0) {
  if (Script_StepUtils(GET_STEP_HIGHER, 0) == 22) {
    int32_t local_FC;
    local_FC = Script_StepUtils(GET_STEP_LOWER, 0);
    if ((local_FC == 2) | (local_FC == 3)) {
      Script_SetShouldSleepFlag(0);
      goto L022066;
    }
  }
  L022066:;
  Script_ryoSetChara(arg0);
  Script_GetSetWeatherSettings(WEATHER_SETTINGS_SET_DISABLE_WEATHER, 1);
  Script_WeatherUtils_0(WeatherDataUtil_ApplyCurrentWeatherOrEnableDateTimeRead, 0);
  Script_WeatherTaskUtil(InitTaskData);
  Script_facemotUtils(arg0, 1, 0);
  Script_facemotUtils(arg0, 0, 0);
  Script_FaceEx(arg0, 0, 0, 2);
  Script_CharHandlers(arg0, -1, 0, 0);
  script_21EDD();
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

static void script_22139(int32_t arg0) {
  script_22021(arg0);
  for (;;) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_22150(int32_t arg0) {
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

static void script_22191(int32_t arg0) {
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

static void script_221D5(int32_t arg0) {
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

static void script_22253(void) {
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_PROGRESS_STEP_28_REACHED)) {
    script_221D5(40);
  } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_PROGRESS_STEP_1E_REACHED)) {
    script_221D5(30);
  } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_PROGRESS_STEP_14_REACHED)) {
    script_221D5(20);
  } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JU00_CATA1_NBO_BRANCH_CHOSEN)) {
    script_221D5(10);
  } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_PROGRESS_STEP05_SA1116_DOJO_DONE)) {
    script_221D5(5);
  } else {
    script_221D5(0);
  }
  return;
}

static void script_222D2(void) {
  Script_ambSoundUtils(1, 0, &data_2A49C, 15.0f, 50.0f, 0.8f, 2, &data_2A4FC);
  Script_ambSoundUtils(2, 0, &data_2A4A8, 18.0f, 30.0f, 0.8f, 2, &data_2A584);
  Script_ambSoundUtils(3, 0, &data_2A4B4, 20.0f, 30.0f, 0.8f, 2, &data_2A60C);
  Script_ambSoundUtils(4, 0, &data_2A4C0, 8.0f, 100.0f, 0.8f, 0, &data_2A694);
  Script_ambSoundUtils(5, 1, data_2A4CC, 50.0f, 40.0f, 1.0f, 0, data_2A71C);
  Script_ambSoundUtils(6, 1, data_2A4D8, 50.0f, 40.0f, 0.8f, 0, data_2A7A4);
  Script_ambSoundUtils(14, 1, &data_2A4E4, 40.0f, 15.0f, 1.0f, 3, &data_2A82C);
  Script_ambSoundUtils(15, 1, &data_2A4F0, 32.0f, 15.0f, 1.0f, 3, &data_2A8B4);
  Script_Ambient1SoundUtils(0, 23976);
  return;
}

__attribute__((scn_string_bindings(0, data_2A93C, 1, data_2A948, 3, data_2A95E, 4, data_2A96B, 5, data_2A978, 6, data_2A984, 7, data_2A98F, 8, data_2A99A, 9, data_2A9A7, 10, data_2A9B4, 11, data_2A9C0, 12, data_2A9CB, 13, data_2A9D6, 14, data_2A9E3, 15, data_2A9F0, 18, data_2AA12, 20, data_2AA27, 22, data_2AA3C, 23, data_2AA4C, 24, data_2AA51, 25, data_2AA61, 26, data_2AA66, 27, data_2AA76, 28, data_2AA7B, 29, data_2AA80, 30, data_2AA81, 31, data_2AA86, 32, data_2AA87, 33, data_2AA8C, 34, data_2AA8D, 35, data_2AA92)))
void script_223FF(void) {
  int32_t unused_E8[6];
  task_248 = Script_ryoSleepUtils(0);
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JOMO_A01124_INE_EVENT_DONE) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JD00_E1049_ROUTE_UNLOCKED) == 0)) {
    Script_LoadSND_Wrapper("system1.snd", "amb036.snd", "bgm063.snd", 0, "f1doujou.snd", "battle_1.snd", 0, 0);
    Script_executeAicaCommand(18088, 0, 0);
    Script_executeAicaCommand(1184, 2, 105);
  } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_TATQ_FOLLOWUP_COMPLETE) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_SA1077_INE_HANDOFF_DONE) == 0) & Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_KAKG_ROUTE_ACTIVE)) {
    Script_LoadSND_Wrapper("system1.snd", "amb036.snd", "bgm012.snd", 0, "f1doujou.snd", "battle_1.snd", 0, 0);
    Script_executeAicaCommand(9384, 0, 0);
    Script_executeAicaCommand(1184, 2, 127);
  } else {
    Script_LoadSND_Wrapper("system1.snd", "amb036.snd", "bgm009.snd", 0, "f1doujou.snd", "battle_1.snd", 0, 0);
    Script_executeAicaCommand(16296, 0, 0);
    Script_executeAicaCommand(1184, 2, 100);
  }
  if ((Script_GetCurrentEntryPoint() == 5) & (task_248 == 0)) {
    Script_ConstructColourPair(0, 1, 255, 255, 255, 255, 255, 255, 255, 255);
  } else {
    Script_filmCheckOrSetTransitionUtil(0, 255, 0, 0, 0, 255, 0, 0, 0);
  }
  script_222D2();
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 10, -1);
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, "/scene/01/JHD0/", "JHD0") == 0) {
    SCNF_WaitFrames_0000(1);
  }
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
  Script_LoadHandMot(1, SHEN_NAME('JHD0'), "HMOT_YKA.BIN");
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
  task_224 = Script_WeatherUtils_0(WeatherDataUtil_QueryCurrentFCASFlag, 0);
  if (task_224 & 2) {
    while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, "/scene/01/JHD0/", "MPSN") == 0) {
      SCNF_WaitFrames_0000(1);
    }
  } else {
    while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, "/scene/01/JHD0/", "MPSU") == 0) {
      SCNF_WaitFrames_0000(1);
    }
  }
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2)) {
    SCNF_WaitFrames_0000(1);
  }
  script_239F9();
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_SA1116_DOJO_TALK_DONE) == 0) {
    while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, "/scene/01/JHD0/", "MSKA") == 0) {
      SCNF_WaitFrames_0000(1);
    }
    while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2)) {
      SCNF_WaitFrames_0000(1);
    }
  }
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JOMO_A01124_INE_EVENT_DONE) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JD00_E1049_ROUTE_UNLOCKED) == 0)) {
    while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, "/scene/01/JHD0/", "TGMA") == 0) {
      SCNF_WaitFrames_0000(1);
    }
    while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2)) {
      SCNF_WaitFrames_0000(1);
    }
  }
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_TATQ_FOLLOWUP_COMPLETE) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_SA1077_INE_HANDOFF_DONE) == 0)) {
    if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_KAKG_ROUTE_ACTIVE)) {
      while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, "/scene/01/JHD0/", "KAKG") == 0) {
        SCNF_WaitFrames_0000(1);
      }
      while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2)) {
        SCNF_WaitFrames_0000(1);
      }
      Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
      Script_AuthorUtils(0, 0, "KAKG", "");
      Script_AuthorUtils(0, 1, "KAKG", "");
      Script_AuthorUtils(0, 2, "KAKG", "");
      Script_AuthorUtils(0, 3, "KAKG", "");
      Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
    }
  }
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 10, 0);
  script_18836();
  script_B807(CHARA_AKIR, (DSWKDoorEntry *)data_26EC8, (int32_t *)data_26F64, (DoorResourceDescriptor *)&data_26F88, (MapObjectRecord *)data_26FA8, 3, 0);
  SCNF_WaitFrames_0000(1);
  SCNF_WaitFrames_0000(1);
  DSWK_DoorUtils(DOOR_CMD_SET_STATE_1);
  SCNF_WaitFrames_0000(1);
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_SA1116_DOJO_TALK_DONE) == 0) {
    script_BBCD(0);
  }
  Script_motiLoad(1, SHEN_NAME('FREE'), "M_MOBJ.BIN");
  Script_CycleManagerUtils(CYCLE_CMD_SETUP, "scene/01/JOMO", (int32_t)"SCRTEST.BIN", 0x3044484A);
  script_2325E();
  Script_shenSetLightingMode(1);
  Script_PREP_Related(-1, 0);
  task_228 = 0;
  task_22B = 0;
  task_22C = 0;
  task_232 = 0;
  task_22D = 0;
  Script_scenToggleCharaFlag(0, 200, 0);
  Script_motmToggleFlags(CHARA_AKIR, 2, 0x40);
  SCNF_WaitFrames_0000(1);
  Script_ryoSleepUtils(2, 1);
  if (task_248 != 0) {
    Script_ryoSleepUtils(1);
    task_22D = Script_ryoSleepUtils(3, 0, 1);
  } else {
    Script_ryoSleepUtils(3, 1, 1, 0);
  }
  SCNF_WaitFrames_0000(1);
  Script_motmToggleFlags(CHARA_AKIR, 1, 0x40);
  script_22015(CHARA_AKIR);
  Script_scenCreateTask(script_22929, 1, 0x52494B41);
  script_169D2();
  Script_scenCreateTask(script_18D61, 0);
  script_18129();
  script_D95F();
  return;
}

__attribute__((scn_string_bindings(0, data_2AAD0, 4, data_2AAEA, 7, data_2AAFC, 8, data_2AB01, 9, data_2AB02, 10, data_2AB0D)))
static int32_t script_22929(int32_t arg0) {
  Vector3 local_F4;
  int32_t unused_DC[6];
  Vector3i local_D0;
  int32_t unused_CC;
  GameDateTime local_C4;
  int32_t local_C0;
  int32_t local_BC;
  int32_t local_B8;
  int32_t unused_A8[4];
  int32_t local_A4;
  script_233AE();
  task_229 = 0;
  task_22A = 0;
  Script_ReturnTimeDate(&local_C4);
  local_C0 = (&local_C4)->hours * 60 + (&local_C4)->minutes;
  local_BC = Script_StepUtils(GET_STEP_HIGHER, 0);
  local_B8 = local_BC * 10 + Script_StepUtils(GET_STEP_LOWER, 0);
  Script_SetRyoMotionDetails(CHARA_AKIR);
  Script_scenCreateTask(script_23876, 0);
  Script_cpctStateUtils(1, 0);
  task_233 = 1;
  Script_scenCreateTask(script_22F34, 0);
  if (Script_SetTaskDataValue(CHARA_AKIR, &data_2AAB8, &data_2AAC4, 0)) {
    Script_cpctStateUtils(1, 2);
    Script_CreateNPCImageModule(CHARA_AKIR, "YKC_M", 0);
    local_F4.x = 0.85f;
    (&local_F4)->y = 0.1f;
    (&local_F4)->z = -22.4f;
    local_D0.x = 0;
    (&local_D0)->y = 0x4000;
    (&local_D0)->z = 0;
    Script_EVT_SetPosition(SHEN_NAME('LKUT'), POS_XYZ, &local_F4);
    Script_EVT_SetRotation(SHEN_NAME('LKUT'), 0x38000000, &local_D0, 0);
    local_F4.x = 0.85f;
    (&local_F4)->y = 0.1f;
    (&local_F4)->z = -22.6f;
    local_D0.x = 0;
    (&local_D0)->y = 0x4000;
    (&local_D0)->z = 0;
    Script_EVT_SetPosition(SHEN_NAME('RKUT'), POS_XYZ, &local_F4);
    Script_EVT_SetRotation(SHEN_NAME('RKUT'), 0x38000000, &local_D0, 0);
    Script_charaDisplay(SHEN_NAME('LKUT'), CDM_Show);
    Script_charaDisplay(SHEN_NAME('RKUT'), CDM_Show);
    task_22B = 1;
    script_1009F();
  }
  local_A4 = Script_GetCurrentEntryPoint();
  if (local_A4 == 5) {
    task_22B = 1;
    goto L022AD0;
  }
  L022AD0:;
  task_10 = 0;
  Script_LoadVoiceFile(1, "E1011");
  Script_scenCreateTask(script_10195, 0);
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_SA1116_DOJO_TALK_DONE) == 0) {
    Script_LoadVoiceFile(1, "SA1116");
    Script_LoadVoiceFile(1, "A0125A");
    script_FF36();
    Script_CreateNPCImageModule(CHARA_FUKU, "FUK_M", 0);
    Script_scenToggleActiveFor(CHARA_FUKU, 1);
    Script_SetCharaFlags(CHARA_FUKU, 4);
    Script_motmGetCharaFlags(CHARA_FUKU, 0, 0);
    local_F4.x = -6.85f;
    (&local_F4)->y = 0.95f;
    (&local_F4)->z = -20.44f;
    local_D0.x = 0;
    (&local_D0)->y = 0x8000;
    (&local_D0)->z = 0;
    Script_EVT_SetPosition(CHARA_FUKU, POS_XYZ, &local_F4);
    Script_EVT_SetRotation(CHARA_FUKU, 0x38000000, &local_D0, 0);
    Script_PlayCharaMotion(CHARA_FUKU, 7077, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_motmToggleFlags(CHARA_FUKU, 1, 8);
    Script_scenCreateTask(script_18137, 0);
    Script_ryoControl(RYO_RestoreControl);
    Script_shenClearFreeState(1);
  } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JOMO_A01124_INE_EVENT_DONE) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JD00_E1049_ROUTE_UNLOCKED) == 0)) {
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_ENTRY2_TIMED_EVENT_DONE, 1);
    Script_LoadVoiceFile(1, "A0139");
    script_FF36();
    Script_CreateNPCImageModule(CHARA_FUKU, "FUB_M", 0);
    Script_scenToggleActiveFor(CHARA_FUKU, 1);
    Script_SetCharaFlags(CHARA_FUKU, 4);
    Script_scenUtils(SCENARIO_CMD_INIT_SCNT);
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
    Script_AuthorUtils(0, 0, "TGMA", "");
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
    task_228 = 1;
    Script_cdttLoadScrUtils(1, 0);
    Script_ConstructColourPair(30, 2, 255, 0, 0, 0, 0, 0, 0, 0);
    script_E615();
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_JD00_E1049_ROUTE_UNLOCKED, 1);
    Script_charaSetRefboardFlagTo(CHARA_AKIR, 0);
    SCNF_WaitFrames_0000(1);
    Script_LoadSND(0, 2, "bgm009.snd", 0);
    Script_executeAicaCommand(16296, 0, 0);
    Script_executeAicaCommand(1184, 2, 100);
    Script_fixoClearAttachment(SHEN_NAME('HOKI'));
    Script_fixoClearAttachment(SHEN_NAME('TEGS'));
    if (-1) {
      Script_charaDisplay(SHEN_NAME('HOKI'), CDM_Hide);
    } else {
      if (0) {
        Script_charaDisplay(SHEN_NAME('HOKI'), CDM_Show);
        goto L022D38;
      }
    }
    L022D38:;
    Script_scenToggleVisibleFor(SHEN_NAME('HOKI'), 0);
    Script_PlayCharaMotion(CHARA_FUKU, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    SCNF_WaitFrames_0000(1);
    Script_AuthorUtils(AUTH_FREE, 0);
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "TGMA");
    Script_ryoControl(RYO_RestoreControl);
    Script_SetGameStateTo(AUTH_None);
    task_228 = 0;
    Script_scenUtils(SCENARIO_CMD_UPDATE_TIMER);
    Script_shenClearFreeState(1);
    script_FEDE();
    task_22A = 1;
  } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_TATQ_FOLLOWUP_COMPLETE) & -(Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_SA1077_INE_HANDOFF_DONE) == 0)) {
    script_18FC0();
    Script_scenCreateTask(script_1901A, 0);
  } else if ((Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_PROGRESS_STEP05_SA1116_DOJO_DONE) == 0) | (-(local_C0 >= 510) & -(local_C0 < 960) & -(local_BC < 6))) {
    if ((local_B8 >= 12) & (local_B8 < 60)) {
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_JHW0_EVENT_COMPLETE) == 0) {
        if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_JHW0_EVENT_SEEN) == 1) {
          if (script_D7CB(28) > 0) {
            script_FF36();
            Script_scenCreateTask(script_1CE80, 0);
          }
        } else {
          script_FF36();
          Script_scenCreateTask(script_1CE80, 0);
        }
      }
    }
    if (task_22D) {
      Script_CycleManagerUtils(CYCLE_CMD_SET_CYCLE_MANAGER_ENABLED, 0);
      goto L022EA1;
    }
    L022EA1:;
    Script_ryoControl(RYO_RestoreControl);
    Script_shenClearFreeState(1);
  } else if (local_BC >= 6) {
    if (task_22D) {
      Script_CycleManagerUtils(CYCLE_CMD_SET_CYCLE_MANAGER_ENABLED, 0);
    }
    Script_scenCreateTask(script_20323, 0);
    Script_ryoControl(RYO_RestoreControl);
    Script_shenClearFreeState(1);
    task_22A = 1;
  } else {
    if (task_22D) {
      Script_CycleManagerUtils(CYCLE_CMD_SET_CYCLE_MANAGER_ENABLED, 0);
    }
    Script_ryoControl(RYO_RestoreControl);
    Script_shenClearFreeState(1);
  }
  Script_cdttLoadScrUtils(1, 0);
  Script_ConstructColourPair(30, 2, 255, 0, 0, 0, 0, 0, 0, 0);
  task_233 = 0;
}

__attribute__((scn_string_bindings(0, data_2AB12, 1, data_2AB22, 2, data_2AB27, 3, data_2AB37, 4, data_2AB3C, 5, data_2AB41, 6, data_2AB42, 7, data_2AB47, 8, data_2AB48, 9, data_2AB4D)))
static int32_t script_22F34(void) {
  int32_t unused_FC;
  GameDateTime local_F4;
  int32_t local_F0;
  while (task_233) {
    SCNF_WaitFrames_0000(1);
  }
  for (;;) {
    Script_ReturnTimeDate(&local_F4);
    if (task_229 == 0) {
      if ((&local_F4)->hours > 18) {
        local_F0 = (&local_F4)->hours * 60 + (&local_F4)->minutes;
        if (local_F0 == 1140) {
          while ((Script_IsPlayerAllowed(0) == 0) | (task_228 != 0)) {
            SCNF_WaitFrames_0000(1);
          }
          Script_ryoControl(PCCTL_DisablePlayerControl);
          Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
          while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2)) {
            SCNF_WaitFrames_0000(1);
          }
          Script_CycleManagerUtils(CYCLE_CMD_PROCESS_SND_EXTRA);
          Script_EVT_GetPosition(CHARA_AKIR, -1, &task_20C, 0);
          Script_mtwkGetSetRotation(CHARA_AKIR, &task_218, 0);
          if (task_224 & 2) {
            while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, "/scene/01/JHD0/", "MPM2") == 0) {
              SCNF_WaitFrames_0000(1);
            }
          } else {
            while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, "/scene/01/JHD0/", "MPM1") == 0) {
              SCNF_WaitFrames_0000(1);
            }
          }
          while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2)) {
            SCNF_WaitFrames_0000(1);
          }
          Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
          if (task_224 & 2) {
            Script_AuthorUtils(0, 0, "MPM2", "");
          } else {
            Script_AuthorUtils(0, 0, "MPM1", "");
          }
          Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
          script_205B5(1);
          Script_AuthorUtils(AUTH_FREE, 0);
          if (task_224 & 2) {
            Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "MPM2");
          } else {
            Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "MPM1");
          }
          Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &task_20C);
          Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &task_218, 0);
          Script_InitializeOsag(CHARA_AKIR);
          Script_SetupLightFromTable(4);
          Script_PREP_Related(-1, 1);
          Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
          Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 1);
          Script_convSetCamInactiveFlag(1);
          Script_SetGameStateTo(AUTH_None);
          Script_ryoControl(RYO_RestoreControl);
        } else {
          Script_SetGlobalSceneSlotValue34(1, 0);
          Script_SetGlobalSceneSlotValue34(3, 0);
          Script_PREP_Related(-1, 0);
          Script_SetupLightFromTable(4);
          Script_PREP_Related(-1, 1);
        }
        task_229 = 1;
      }
    }
    if (-(task_22A == 0) & -(task_228 == 0) & -(Script_scenUtils(SCENARIO_CMD_GET_SCEN_FLAG100) == 0)) {
      if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, 0x554B5546)) {
        if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_SA1116_DOJO_TALK_DONE)) {
          if ((&local_F4)->hours > 15) {
            script_FEDE();
            task_22A = 1;
          }
        }
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_231E5(void) {
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 50.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 30.0f);
  SCNF_WaitFrames_0000(1);
  Script_WeatherTaskUtil(FreeTaskData);
  Script_GetSetWeatherSettings(WEATHER_SETTINGS_SET_DISABLE_WEATHER, 0);
  Script_ProcessFogFromTable(0);
  Script_SetupLightFromTable(3);
  (void)1;
  Script_SetVector4ByOffset(&data_2AB54);
  return;
}

static void script_23231(void) {
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 50.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 50.0f);
  Script_SetupLightFromTable(1);
  Script_SetVector4ByOffset(&data_2AB84);
  (void)1;
  return;
}

static void script_2325E(void) {
  int32_t unused_F8[2];
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 50.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 15.0f);
  SCNF_WaitFrames_0000(1);
  Script_SetFogColour(&data_2AC3C);
  Script_SetVector4ByOffset(&data_2AC4C);
  (void)1;
  Script_SetMapFlags(0, 1);
  Script_WeatherUtils(WeatherScript_LoadWccpClipSet0FromTaskOffset, &data_2AB94, 3);
  return;
}

static void script_232AC(void) {
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 50.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 0.0f);
  SCNF_WaitFrames_0000(1);
  Script_WeatherTaskUtil(FreeTaskData);
  Script_GetSetWeatherSettings(WEATHER_SETTINGS_SET_DISABLE_WEATHER, 0);
  Script_SetupLightFromTable(5);
  Script_SetFogColour(&data_2AC6C);
  Script_SetFogFlags(0, data_2AC7C);
  (void)1;
  Script_SetVector4ByOffset(&data_2AC5C);
  return;
}

static void script_23302(void) {
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 30.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 0.0f);
  SCNF_WaitFrames_0000(1);
  Script_WeatherTaskUtil(FreeTaskData);
  Script_GetSetWeatherSettings(WEATHER_SETTINGS_SET_DISABLE_WEATHER, 0);
  Script_ProcessFogFromTable(0);
  Script_SetupLightFromTable(6);
  (void)1;
  Script_SetVector4ByOffset(&data_2AC8C);
  return;
}

static void script_2334B(void) {
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 50.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 15.0f);
  Script_GetSetWeatherSettings(WEATHER_SETTINGS_SET_DISABLE_WEATHER, 1);
  Script_WeatherTaskUtil(InitTaskData);
  if (task_229) {
    Script_SetGlobalSceneSlotValue34(1, 0);
    Script_SetGlobalSceneSlotValue34(3, 0);
    Script_PREP_Related(-1, 0);
    Script_SetupLightFromTable(4);
    Script_PREP_Related(-1, 1);
  } else {
    script_1009F();
  }
  return;
}

__attribute__((scn_string_bindings(0, data_2AC9C, 2, data_2ACAD, 3, data_2ACB3, 5, data_2ACC5, 7, data_2ACD7, 9, data_2ACEF, 13, data_2AD15)))
static void script_233AE(void) {
  int32_t local_FC;
  int32_t local_F8;
  while (1) {
    local_F8 = Script_GetCurrentEntryPoint();
    if (local_F8 == 90) {
      Script_LoadVoiceFile(1, "A0125");
      local_FC = Script_motiLoad(1, SHEN_NAME('JHD0'), "M_0125.BIN");
      script_231E5();
      Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
      script_F323();
      script_2334B();
      Script_ReallocateUNFO(local_FC);
      Script_LoadVoiceFile(0, "A0125");
      Script_charaDisplay(SHEN_NAME('MOON'), CDM_Hide);
      Script_filmCheckOrSetTransitionUtil(30, 255, 255, 255, 255, 0, 255, 255, 255);
      while (Script_filmCheckOrSetTransitionUtil(-1)) {
        SCNF_WaitFrames_0000(1);
      }
    } else if (local_F8 == 91) {
      script_231E5();
      script_237B7(0x554B5546);
      Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
      script_E59C();
      script_2334B();
    } else if (local_F8 == 92) {
      script_237B7(0x554B5546);
      script_231E5();
      Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
      script_E615();
      script_2334B();
    } else if (local_F8 == 93) {
      Script_cdttUtil(Mode2);
      Script_cdttUtil(Mode1);
      script_169EB(1, SHEN_NAME('YD01'), 0, 0);
      SCNF_WaitFrames_0000(1);
    } else if (local_F8 == 94) {
      Script_LoadVoiceFile(1, "01KAK");
      script_237B7(0x554B5546);
      script_237B7(0x5F454E49);
      local_FC = Script_motiLoad(1, SHEN_NAME('JHD0'), "M_01KAK.BIN");
      Script_CreateNPCImageModule(CHARA_AKIR, 0, 0);
      Script_CreateNPCImageModule(CHARA_AKIR, "YKE_M", 0);
      script_231E5();
      Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
      script_F70F();
      script_2334B();
      Script_ReallocateUNFO(local_FC);
    } else if (local_F8 == 95) {
      script_237B7(0x5F454E49);
      Script_charaSetRefboardFlagTo(CHARA_AKIR, 1);
      local_FC = Script_motiLoad(1, SHEN_NAME('JHD0'), "M_01CRY.BIN");
      script_232AC();
      Script_LoadSND(0, 2, "bgm049.snd", 0);
      Script_LoadSND(0, 4, "a1_inet1.snd", 0);
      Script_executeAicaCommand(17064, 0, 0);
      Script_executeAicaCommand(1184, 2, 96);
      Script_LoadVoiceFile(1, "A01124");
      Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
      script_FA8C();
      Script_executeAicaCommand(2720, 2, 64);
      script_2334B();
      Script_ReallocateUNFO(local_FC);
    } else if (local_F8 == 96) {
      script_237B7(0x554B5546);
      Script_charaSetRefboardFlagTo(CHARA_AKIR, 1);
      Script_charaSetRefboardFlagTo(CHARA_FUKU, 1);
      local_FC = Script_motiLoad(1, SHEN_NAME('JHD0'), "M_01SOR.BIN");
      script_23302();
      Script_LoadVoiceFile(1, "01SOR");
      Script_LoadSND(0, 4, "a1_fukay.snd", 0);
      Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
      script_DDBB();
      script_2334B();
      Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 50.0f);
      Script_ReallocateUNFO(local_FC);
    } else if (local_F8 == 97) {
      script_237B7(0x554B5546);
      Script_CreateNPCImageModule(CHARA_AKIR, 0, 0);
      Script_CreateNPCImageModule(CHARA_AKIR, "YKE_M", 0);
      Script_charaSetRefboardFlagTo(CHARA_AKIR, 1);
      Script_charaSetRefboardFlagTo(CHARA_FUKU, 1);
      local_FC = Script_motiLoad(1, SHEN_NAME('JHD0'), "M_01DIS.BIN");
      Script_AuthorUtils(0, 9, "seqdataD.bin", "scene/01/JHD0");
      Script_LoadSND(0, 4, "a1_houha.snd", 0);
      script_231E5();
      Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
      script_EC28();
      script_2334B();
      Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 50.0f);
      Script_ReallocateUNFO(local_FC);
    } else {
      break;
    }
  }
  return;
}

static void script_237B7(int32_t arg0) {
  int32_t local_FC;
  Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT, arg0, 1);
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0) == 0) {
    local_FC = Script_CycleManagerUtils(CYCLE_CMD_GET_CYCLE_PTR_BY_CHARA_ID, arg0);
    while (local_FC == 0) {
      SCNF_WaitFrames_0000(1);
      local_FC = Script_CycleManagerUtils(CYCLE_CMD_GET_CYCLE_PTR_BY_CHARA_ID, arg0);
    }
    while (Script_CycleManagerUtils(CYCLE_CMD_GET_NEXT_TASK_POINTER, local_FC) == 0) {
      SCNF_WaitFrames_0000(1);
    }
    Script_CycleManagerUtils(CYCLE_CMD_SET_RUNNING_PROGRAM_FIELD_B8, arg0, 1);
  }
  return;
}

static void script_2382D(void) {
  int32_t local_FC;
  local_FC = Script_StepUtils(GET_STEP_HIGHER, 0);
  if (local_FC < 6) goto L023849;
  L023849:;
  Script_FaceEx(CHARA_FUKU, 0, 0, 0);
  return;
}

static void script_23859(void) {
  int32_t local_FC;
  local_FC = Script_StepUtils(GET_STEP_HIGHER, 0);
  if (local_FC < 6) goto L023875;
  L023875:;
  return;
}

static int32_t script_23876(void) {
  int8_t local_FF;
  int8_t local_FE;
  int8_t local_FD;
  int8_t local_FC;
  task_22E = 0;
  task_22F = 0;
  task_230 = 0;
  task_231 = 0;
  local_FF = 0;
  local_FE = 0;
  local_FD = 0;
  local_FC = 0;
  SCNF_WaitFrames_0000(1);
  Script_PlayerStatsUtils(PS_IncrementSpotVisitCount, 16);
  for (;;) {
    if (Script_SetTaskDataValue(CHARA_AKIR, &data_2AD50, &data_2AD5C, 0)) {
      if (task_22E == 0) {
        task_22E = 1;
        Script_PlayerStatsUtils(PS_IncrementSpotVisitCount, 11);
        local_FF = local_FF + 1;
      }
    } else {
      if (task_22E) {
        task_22E = 0;
      }
    }
    if (Script_SetTaskDataValue(CHARA_AKIR, &data_2AD68, &data_2AD74, 0)) {
      if (task_22F == 0) {
        task_22F = 1;
        Script_PlayerStatsUtils(PS_IncrementSpotVisitCount, 12);
        local_FE = local_FE + 1;
      }
    } else {
      if (task_22F) {
        task_22F = 0;
      }
    }
    if (Script_SetTaskDataValue(CHARA_AKIR, &data_2AD80, &data_2AD8C, 0)) {
      if (task_230 == 0) {
        task_230 = 1;
        Script_PlayerStatsUtils(PS_IncrementSpotVisitCount, 13);
        local_FD = local_FD + 1;
      }
    } else {
      if (task_230) {
        task_230 = 0;
      }
    }
    if (Script_SetTaskDataValue(CHARA_AKIR, &data_2AD98, &data_2ADA4, 0)) {
      if (task_231 == 0) {
        task_231 = 1;
        Script_PlayerStatsUtils(PS_IncrementSpotVisitCount, 10);
        local_FC = local_FC + 1;
      }
    } else {
      if (task_231) {
        task_231 = 0;
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

__attribute__((scn_string_bindings(0, data_2ADB0, 2, data_2ADC5, 4, data_2ADDA, 6, data_2ADEF, 7, data_2ADFF, 8, data_2AE04, 9, data_2AE14, 10, data_2AE19, 11, data_2AE29, 12, data_2AE2E, 13, data_2AE3E, 14, data_2AE43, 15, data_2AE53, 16, data_2AE58, 17, data_2AE68, 18, data_2AE6D, 19, data_2AE7D, 20, data_2AE82)))
static void script_239F9(void) {
  int32_t unused_FC;
  GameDateTime local_F4;
  Script_ReturnTimeDate(&local_F4);
  if ((&local_F4)->month == 3) {
    if (((&local_F4)->day == 25) | ((&local_F4)->day == 26) | ((&local_F4)->day == 27)) {
      while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, "/scene/01/JHD0/", "MPS1") == 0) {
        SCNF_WaitFrames_0000(1);
      }
    } else {
      if (((&local_F4)->day == 28) | ((&local_F4)->day == 29) | ((&local_F4)->day == 30) | ((&local_F4)->day == 31)) {
        while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, "/scene/01/JHD0/", "MPS2") == 0) {
          SCNF_WaitFrames_0000(1);
        }
        goto L023AAC;
      }
    }
    L023AAC:;
  } else {
    if ((&local_F4)->month == 4) {
      if (((&local_F4)->day == 1) | ((&local_F4)->day == 2) | ((&local_F4)->day == 3) | ((&local_F4)->day == 4) | ((&local_F4)->day == 5) | ((&local_F4)->day == 6) | ((&local_F4)->day == 7)) {
        while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, "/scene/01/JHD0/", "MPS3") == 0) {
          SCNF_WaitFrames_0000(1);
        }
      } else if ((&local_F4)->day == 8) {
        Vector3 local_E8;
        Vector3 local_DC;
        int32_t local_D8;
        Script_sakuraUtils(15);
        local_E8.x = 9.67f;
        (&local_E8)->y = 7.73f;
        (&local_E8)->z = -29.1f;
        local_DC.x = 3.0f;
        (&local_DC)->y = 0.0f;
        (&local_DC)->z = 3.0f;
        local_D8 = Script_sakuraUtils(0, &local_E8, &local_DC, 40, 0.00025f, 0);
        script_24000(local_D8);
        Script_sakuraUtils(1, local_D8);
        while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, "/scene/01/JHD0/", "MPS4") == 0) {
          SCNF_WaitFrames_0000(1);
        }
      } else if ((&local_F4)->day == 9) {
        Vector3 local_E8;
        Vector3 local_DC;
        int32_t local_D8;
        Script_sakuraUtils(15);
        local_E8.x = 9.67f;
        (&local_E8)->y = 7.73f;
        (&local_E8)->z = -29.1f;
        local_DC.x = 3.0f;
        (&local_DC)->y = 0.0f;
        (&local_DC)->z = 3.0f;
        local_D8 = Script_sakuraUtils(0, &local_E8, &local_DC, 800, 0.005f, 0);
        script_24000(local_D8);
        Script_sakuraUtils(1, local_D8);
        while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, "/scene/01/JHD0/", "MPS4") == 0) {
          SCNF_WaitFrames_0000(1);
        }
      } else if ((&local_F4)->day == 10) {
        Vector3 local_E8;
        Vector3 local_DC;
        int32_t local_D8;
        Script_sakuraUtils(15);
        local_E8.x = 9.67f;
        (&local_E8)->y = 7.73f;
        (&local_E8)->z = -29.1f;
        local_DC.x = 3.0f;
        (&local_DC)->y = 0.0f;
        (&local_DC)->z = 3.0f;
        local_D8 = Script_sakuraUtils(0, &local_E8, &local_DC, 480, 0.003f, 0);
        script_24000(local_D8);
        Script_sakuraUtils(1, local_D8);
        while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, "/scene/01/JHD0/", "MPS5") == 0) {
          SCNF_WaitFrames_0000(1);
        }
      } else if ((&local_F4)->day == 11) {
        Vector3 local_E8;
        Vector3 local_DC;
        int32_t local_D8;
        Script_sakuraUtils(15);
        local_E8.x = 9.67f;
        (&local_E8)->y = 7.73f;
        (&local_E8)->z = -29.1f;
        local_DC.x = 3.0f;
        (&local_DC)->y = 0.0f;
        (&local_DC)->z = 3.0f;
        local_D8 = Script_sakuraUtils(0, &local_E8, &local_DC, 400, 0.0025f, 0);
        script_24000(local_D8);
        Script_sakuraUtils(1, local_D8);
        while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, "/scene/01/JHD0/", "MPS5") == 0) {
          SCNF_WaitFrames_0000(1);
        }
      } else if ((&local_F4)->day == 12) {
        Vector3 local_E8;
        Vector3 local_DC;
        int32_t local_D8;
        Script_sakuraUtils(15);
        local_E8.x = 9.67f;
        (&local_E8)->y = 7.73f;
        (&local_E8)->z = -29.1f;
        local_DC.x = 3.0f;
        (&local_DC)->y = 0.0f;
        (&local_DC)->z = 3.0f;
        local_D8 = Script_sakuraUtils(0, &local_E8, &local_DC, 240, 0.002f, 0);
        script_24000(local_D8);
        Script_sakuraUtils(1, local_D8);
        while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, "/scene/01/JHD0/", "MPS6") == 0) {
          SCNF_WaitFrames_0000(1);
        }
      } else if ((&local_F4)->day == 13) {
        Vector3 local_E8;
        Vector3 local_DC;
        int32_t local_D8;
        Script_sakuraUtils(15);
        local_E8.x = 9.67f;
        (&local_E8)->y = 7.73f;
        (&local_E8)->z = -29.1f;
        local_DC.x = 3.0f;
        (&local_DC)->y = 0.0f;
        (&local_DC)->z = 3.0f;
        local_D8 = Script_sakuraUtils(0, &local_E8, &local_DC, 160, 0.0015f, 0);
        script_24000(local_D8);
        Script_sakuraUtils(1, local_D8);
        while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, "/scene/01/JHD0/", "MPS6") == 0) {
          SCNF_WaitFrames_0000(1);
        }
      } else if ((&local_F4)->day == 14) {
        Vector3 local_E8;
        Vector3 local_DC;
        int32_t local_D8;
        Script_sakuraUtils(15);
        local_E8.x = 9.67f;
        (&local_E8)->y = 7.73f;
        (&local_E8)->z = -29.1f;
        local_DC.x = 3.0f;
        (&local_DC)->y = 0.0f;
        (&local_DC)->z = 3.0f;
        local_D8 = Script_sakuraUtils(0, &local_E8, &local_DC, 80, 0.0005f, 0);
        script_24000(local_D8);
        Script_sakuraUtils(1, local_D8);
        while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, "/scene/01/JHD0/", "MPS6") == 0) {
          SCNF_WaitFrames_0000(1);
        }
      } else {
        if ((&local_F4)->day == 15) {
          while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, "/scene/01/JHD0/", data_2AE92) == 0) {
            SCNF_WaitFrames_0000(1);
          }
          goto L023FE5;
        }
      }
      L023FE5:;
      goto L023FE8;
    }
  }
  L023FE8:;
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_24000(int32_t arg0) {
  Vector3 local_F4;
  Vector3 local_E8;
  int32_t unused_E4;
  GameDateTime local_DC;
  int32_t local_D8;
  Script_ReturnTimeDate(&local_DC);
  local_D8 = Script_WeatherUtils_0(WeatherDataUtil_GetWeatherClassForDateTime, (&local_DC)->year, (&local_DC)->month, (&local_DC)->day, (&local_DC)->hours, (&local_DC)->minutes);
  if (local_D8 == 2) {
    local_F4.x = -0.00360784307f;
    (&local_F4)->y = -0.0200784318f;
    (&local_F4)->z = 0.00188235298f;
  } else {
    local_F4.x = -0.00360784307f;
    (&local_F4)->y = -0.0122352941f;
    (&local_F4)->z = 0.00188235298f;
  }
  local_E8.x = -0.00235294108f;
  (&local_E8)->y = -0.00250980398f;
  (&local_E8)->z = -0.00235294108f;
  Script_sakuraUtils(6, arg0, &local_F4, &local_E8);
  return;
}

static void script_240AB(int32_t arg0) {
  if (arg0 < 0) {
    (void)-arg0;
    return;
  } else {
    (void)arg0;
    return;
  }
}

static void script_240C9(float arg0) {
  if (!(arg0 >= 0.0f)) {
    (void)((float)0 - arg0);
    return;
  } else {
    (void)arg0;
    return;
  }
}

static void script_240E8(int32_t arg0) {
  if (arg0 != 0) {
    Script_sprintf("WAZA Warning : handle is not NULL, and create same handle !\012");
  }
  return;
}

static void script_240FD(int32_t arg0, int32_t arg1) {
  for (;;) {
    if (*(&task_394 + 3) & 2) {
      {
        Vector3 local_F4;
        Script_EVT_GetPosition(arg0, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
        Script_sprintf("POS POS_RYOU_0 = %f, %f, %f.\012", (&local_F4)->x, (&local_F4)->y, (&local_F4)->z);
      }
      {
        Vector3 local_F4;
        Script_EVT_GetPosition(arg1, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
        Script_sprintf("POS POS_KAIS_0 = %f, %f, %f.\012", (&local_F4)->x, (&local_F4)->y, (&local_F4)->z);
      }
      {
        int32_t unused_F4[3];
        Vector3i local_E8;
        Script_mtwkGetSetRotation(arg0, &local_E8, 0x40000000);
        Script_sprintf("define ANG_RYOU_0 DEG(%d.0)\012", (&local_E8)->y * 360 / 0x10000);
      }
      {
        int32_t unused_F4[3];
        Vector3i local_E8;
        Script_mtwkGetSetRotation(arg1, &local_E8, 0x40000000);
        Script_sprintf("define ANG_KAIS_0 DEG(%d.0)\012", (&local_E8)->y * 360 / 0x10000);
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static int32_t script_241B3(int32_t arg0) {
  task_8B8 = arg0;
  *(&task_8B8 + 1) = 0;
  return (int32_t)&task_8B8;
}

static void script_241CB(int32_t arg0) {
  Script_sprintf("%s\012", script_241B3(arg0));
  return;
}

static void script_241E1(int32_t arg0) {
  Script_sprintf("Konnna Error ha Detekonai hazu : %d\012", arg0);
  return;
}

static void script_241EF(void) {
  Vector3 local_F4;
  SCNF_WaitFrames_0000(60);
  Script_EVT_GetPosition(CHARA_AKIR, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  (&local_F4)->y = 0.1f;
  for (;;) {
    if (*(&task_384 + 1) & 0x4000) {
      local_F4.x = (&local_F4)->x - 0.1f;
    }
    if (*(&task_384 + 1) & 0x8000) {
      local_F4.x = (&local_F4)->x + 0.1f;
    }
    if (*(&task_384 + 1) & 0x1000) {
      (&local_F4)->z = (&local_F4)->z - 0.1f;
    }
    if (*(&task_384 + 1) & 0x2000) {
      (&local_F4)->z = (&local_F4)->z + 0.1f;
    }
    Script_TranslateRotateScale_Wrapper(1, 0x3F800000, &local_F4, &data_2AFA0, &data_2AF94);
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_242B9(void) {
  return;
}

static void script_242BC(void) {
  task_3B4 = 0;
  while (1) {
    task_3B4 = task_3B4 + 1;
    task_384 = Script_GetInputData(0, 0);
    *(&task_384 + 1) = Script_GetInputData(1, 0);
    *(&task_384 + 2) = Script_GetInputData(2, 0);
    *(&task_384 + 3) = Script_GetInputData(3, 0);
    task_394 = Script_GetInputData(0, 1);
    *(&task_394 + 1) = Script_GetInputData(1, 1);
    *(&task_394 + 2) = Script_GetInputData(2, 1);
    *(&task_394 + 3) = Script_GetInputData(3, 1);
    task_3A4 = Script_GetInputData(0, 2);
    *(&task_3A4 + 1) = Script_GetInputData(1, 2);
    *(&task_3A4 + 2) = Script_GetInputData(2, 2);
    *(&task_3A4 + 3) = Script_GetInputData(3, 2);
    if (*(&task_394 + 1) & 2) {
      Script_facemotUtils(CHARA_AKIR, 0, 0);
      Script_facemotUtils(CHARA_KISY, 0, 0);
      Script_facemotUtils(CHARA_TAIJ, 0, 0);
      Script_facemotUtils(CHARA_AKIR, 1, 0);
      Script_facemotUtils(CHARA_KISY, 1, 0);
      Script_facemotUtils(CHARA_TAIJ, 1, 0);
      Script_sprintf("Entry Model!\012");
    }
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_2440A(int32_t arg0) {
  task_3B8 = arg0;
  Script_seqEnterEvtCam(arg0, 0, AUTH_None);
  return;
}

static int32_t script_2441E(int32_t arg0, int32_t arg1, int32_t arg2) {
  SCNF_WaitFrames_0000(arg2);
  script_2440A(arg0);
  SCNF_WaitFrames_0000(1);
  Script_AllocSpline(arg1);
  end:;
}

static void script_24440(int32_t arg0, int32_t arg1, int32_t arg2) {
  arg2 = arg2 / 2;
  if (arg2) {
    arg2 = arg2 - 1;
    Script_scenCreateTask(script_2441E, 3, arg0, arg1, arg2);
  } else {
    script_2440A(arg0);
    SCNF_WaitFrames_0000(1);
    Script_AllocSpline(arg1);
  }
  return;
}

static void script_24486(int32_t arg0, int32_t arg1, int32_t arg2) {
  task_3B8 = arg0;
  Script_seqEnterEvtCam(arg0, arg1, arg2);
  return;
}

static void script_2449A(int32_t arg0) {
  for (;;) {
    SCNF_WaitFrames_0000(1);
    if (Script_IsCharaMotionFinished(arg0)) {
      break;
    }
  }
  return;
}

static void script_244B4(void) {
  for (;;) {
    SCNF_WaitFrames_0000(1);
    if (Script_IsInEventCam() == 0) {
      break;
    }
  }
  return;
}

static int32_t script_244CF(int32_t arg0) {
  script_2440A(arg0);
  SCNF_WaitFrames_0000(2);
  for (;;) {
    script_244B4();
    script_2440A(arg0);
    SCNF_WaitFrames_0000(2);
  }
  end:;
}

static void script_244FC(int32_t arg0, int32_t arg1) {
  while (arg0) {
    SCNF_WaitFrames_0000(1);
    arg0 = arg0 - 1;
  }
  Script_executeAicaCommand(arg1, 0, 0);
  return;
}

static void script_24523(void) {
  task_8C0 = 0;
  return;
}

static void script_2452D(void) {
  for (;;) {
    if (task_8C0 == 1) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_24548(void) {
  task_8C0 = 1;
  return;
}

static void script_24552(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  local_F8 = arg0;
  local_FC = 0;
  SCNF_WaitFrames_0000(1);
  for (;;) {
    if ((Script_convGetIsActive(0) == 0) & (arg0 == 0)) {
      break;
    }
    if (arg0 > 0) {
      arg0 = arg0 - 1;
    }
    if (Script_convGetIsActive(0) != 0) {
      local_FC = local_FC + 1;
    }
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static int32_t script_245B6(float arg0, float arg1, float arg2, float arg3) {
  int32_t unused_FC;
  int32_t local_F4[2];
  arg0 = arg2 - arg0;
  arg1 = arg3 - arg1;
  return local_F4[1] = Script_FloatUtils(ScriptMathOp_Atan2ReverseVector, arg0, arg1);
}

static int32_t script_245DD(int32_t arg0, int32_t arg1) {
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

static int32_t script_24655(int32_t arg0, int32_t arg1, int32_t arg2, const Vector3 *arg3) {
  int8_t local_FF;
  local_FF = 0;
  for (;;) {
    task_8C4[arg1] = Script_wmptUtils(arg0, arg3, arg2);
    if (task_8C4[arg1] == 0) {
      break;
    }
    if (Script_IsCharaMotionFinished(arg0)) {
      local_FF = local_FF + 1;
      if (local_FF > 10) {
        Script_sprintf("WZ ModoriMove Limit Over !!!!!!!!!!!\012");
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_EVT_SetPosition(arg0, 0x78000000, arg3);
  Script_PlayCharaMotion(arg0, MN_AKI_AKI_SEKKIN_KAMAE_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_motmToggleFlags(arg0, 1, 0x40);
  Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
  Script_charaToggleMotEnabled(arg0, 1);
  end:;
}

static void script_24701(int32_t arg0, int32_t arg1, int32_t arg2) {
  Vector3 local_F4;
  Vector3i local_E8;
  float local_DC[3];
  int32_t local_D8;
  int32_t unused_C8[4];
  int32_t local_C0[2];
  int32_t local_BC;
  int8_t local_BB;
  uint8_t unused_B8[3];
  int32_t local_B4;
  Script_EVT_GetPosition(arg0, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  Script_mtwkGetSetRotation(arg0, &local_E8, 0x40000000);
  if (!(Script_MathUtils(SCRIPT_MATH_TASK_VEC3_DISTANCE, &local_F4, (const Vector3 *)arg2) >= 0.06f)) {
    task_8C4[arg1] = 0;
  } else {
    local_DC[0] = *(float *)arg2 - (&local_F4)->x;
    local_DC[2] = *(float *)(arg2 + 8) - (&local_F4)->z;
    local_C0[1] = script_245B6((&local_F4)->x, (&local_F4)->z, *(float *)arg2, *(float *)(arg2 + 8));
    local_D8 = script_245DD(local_C0[1], (&local_E8)->y);
    local_BB = 0;
    if ((local_D8 < -0x7000) | (local_D8 > 0x7000)) {
      local_BC = 839;
    } else if (local_D8 < -0x5000) {
      local_BC = 842;
    } else if (local_D8 < -0x3000) {
      local_BC = 841;
    } else if (local_D8 < -0x1000) {
      local_BC = 1274;
    } else if (local_D8 < 0x1000) {
      local_BC = 840;
    } else if (local_D8 < 0x3000) {
      local_BC = 863;
    } else if (local_D8 < 0x5000) {
      local_BC = 861;
    } else {
      local_BC = 1273;
    }
    Script_PlayCharaMotion(arg0, local_BC, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    if (local_BB) {
      Script_motmToggleFlags(arg0, 1, 0x80);
    } else {
      Script_motmToggleFlags(arg0, 2, 0x80);
    }
    Script_motmToggleFlags(arg0, 2, 0x40);
    Script_SetCharaMotionPlaybackRate(arg0, 2.0f);
    Script_charaToggleMotEnabled(arg0, 1);
    Script_motmToggleFlags(arg0, 2, 4);
    task_8C4[arg1] = 1000;
    local_B4 = Script_scenCreateTask(script_24655, 4, arg0, arg1, local_BC, arg2);
  }
  return;
}

static void script_248C7(int32_t arg0) {
  for (;;) {
    if (task_8C4[arg0] == 0) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_248E9(int32_t arg0, int32_t arg1, int32_t arg2) {
  Vector3 local_F4;
  Vector3 local_E8;
  Vector3i local_DC;
  Vector3i local_D0;
  Script_EVT_GetPosition(arg0, -1, &local_E8, 0);
  Script_mtwkGetSetRotation(arg0, &local_DC, 0);
  (&local_DC)->y = ((&local_DC)->y + arg2) & 0xFFFF;
  (&local_D0)->y = (&local_DC)->y;
  Script_EVT_SetRotation(arg0, MATRIX_XFORM_Y, &local_D0, 0);
  local_F4.x = *(float *)(arg1 + 8) * Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_DC)->y) + *(float *)arg1 * Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_DC)->y);
  (&local_F4)->z = *(float *)(arg1 + 8) * Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_DC)->y) - *(float *)arg1 * Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_DC)->y);
  local_F4.x = (&local_F4)->x + (&local_E8)->x;
  (&local_F4)->y = (&local_E8)->y;
  (&local_F4)->z = (&local_F4)->z + (&local_E8)->z;
  Script_EVT_SetPosition(arg0, POS_XYZ, &local_F4);
  return;
}

static int32_t script_249C4(void) {
  for (;;) {
    task_3C8 = task_3C8 + 1;
    task_3CC = task_3CC + 1;
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_249E8(int32_t arg0) {
  arg0 = 0;
  *(int8_t *)task_3EC[arg0] = *(int8_t *)&task_3E0;
  *(int8_t *)&task_3EC[arg0][1] = *((int8_t *)&task_3E0 + 1);
  *(int8_t *)&task_3EC[arg0][2] = *((int8_t *)&task_3E0 + 2);
  *(int8_t *)&task_3EC[arg0][3] = *((int8_t *)&task_3E0 + 3);
  *(int8_t *)&task_3EC[arg0][4] = *((int8_t *)&task_3E0 + 4);
  *(int8_t *)&task_3EC[arg0][5] = *((int8_t *)&task_3E0 + 5);
  *(int8_t *)&task_3EC[arg0][6] = *((int8_t *)&task_3E0 + 6);
  *(int8_t *)&task_3EC[arg0][7] = *((int8_t *)&task_3E0 + 7);
  *(int32_t *)&task_3EC[arg0][8] = *(int32_t *)(&task_3E0 + 8);
  return;
}

static int32_t script_24AA8(int32_t arg0, int32_t arg1) {
  return 0;
}

static int32_t script_24AAE(int32_t arg0) {
  return *(int8_t *)&task_3EC[0][2];
}

static int32_t script_24AB9(int32_t arg0) {
  int32_t unused_F8[2];
  for (;;) {
    if (arg0 == 9) {
      if ((task_3C4 != task_3C0) & (task_3C0 % 3 == 0)) {
        task_3DC = 1;
      }
      if ((task_3C8 != 0) & (task_3C8 % 450 == 0)) {
        task_3DC = 1;
        task_3DD = 1;
      }
    } else {
      if (((task_3CC != 0) & (task_3CC % 300 == 0)) | ((task_3C4 != task_3C0) & (task_3C0 % 3 == 0))) {
        task_3DC = 1;
      }
      if ((task_3C8 != 0) & (task_3C8 % 2700 == 0)) {
        task_3DC = 1;
        task_3DF = 1;
      } else {
        if ((task_3C8 != 0) & (task_3C8 % 1800 == 0)) {
          task_3DC = 1;
          task_3DE = 1;
          goto L024B80;
        }
      }
    }
    L024B80:;
    task_3C4 = task_3C0;
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_24B94(int32_t arg0, int32_t arg1) {
  Vector3 local_F4;
  int32_t local_F0;
  task_8B3 = 1;
  local_F0 = 0;
  for (;;) {
    if (task_8B3 == 0) {
      break;
    }
    Script_EVT_GetPosition(CHARA_AKIR, 5, &local_F4, ENTITY_XFORM_WORLD_SPACE);
    Script_SetLookPointDetails(arg0, 12, &local_F4, 0);
    if (arg1 != -1) {
      local_F0 = local_F0 + 1;
      if (local_F0 > arg1) {
        break;
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_24BF9(int32_t arg0, int32_t arg1) {
  Vector3 local_F4;
  int32_t local_F0;
  task_8B4 = 1;
  local_F0 = 0;
  for (;;) {
    if (task_8B4 == 0) {
      break;
    }
    Script_EVT_GetPosition(arg0, 5, &local_F4, ENTITY_XFORM_WORLD_SPACE);
    Script_SetLookPointDetails(CHARA_AKIR, 12, &local_F4, 0);
    if (arg1 != -1) {
      local_F0 = local_F0 + 1;
      if (local_F0 > arg1) {
        break;
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_24C5E(void) {
  int32_t local_FC;
  task_8B2 = 0;
  local_FC = 0;
  for (;;) {
    *((int8_t *)task_89C + local_FC) = 0;
    *((int8_t *)task_8A8 + local_FC) = 0;
    local_FC = local_FC + 1;
    if (local_FC >= 10) {
      break;
    }
  }
  return;
}

static void script_24C9A(void) {
  for (;;) {
    if (task_8B2 == 0) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_24CB5(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Vector3 local_F4;
  SCNF_WaitFrames_0000(arg3);
  Script_EVT_GetPosition(arg1, 5, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  Script_SetLookPointDetails(arg0, 12, &local_F4, 0);
  Script_charaToggleMotEnabled(arg0, 0);
  Script_motmSetBlendFrames(arg0, arg2);
  Script_motmToggleFlags(arg0, 1, 0x40);
  Script_PlayCharaMotion(arg0, MN_AKI_AKI_STAND_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  SCNF_WaitFrames_0000(arg2 + 20);
  Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_5);
  return;
}

static void script_24D29(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t unused_F4[3];
  script_24548();
  task_8B2 = 1;
  script_24CB5(arg0, arg1, arg2, arg3);
  task_8B2 = 0;
  return;
}

static void script_24D4E(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Vector3 local_F4;
  script_24548();
  task_8B2 = 1;
  script_24CB5(arg0, arg1, arg2, arg3);
  Script_EVT_GetPosition(arg1, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  SCNF_CHAR_SCNFApplyFacingTowardPoint(arg0, &local_F4, 0, 0.0f, 0, 0x200000);
  task_8B2 = 0;
  return;
}

static void script_24D97(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Vector3 local_F4;
  int32_t local_F0;
  script_24548();
  task_8B2 = 1;
  script_24CB5(arg0, arg1, arg2, arg3);
  local_F0 = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f);
  if (local_F0 == 0) {
    Script_EVT_GetPosition(arg1, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
    SCNF_CHAR_SCNFApplyFacingTowardPoint(arg0, &local_F4, 0, 0.0f, 0, 0x200000);
    goto L024DF5;
  }
  L024DF5:;
  task_8B2 = 0;
  return;
}

static void script_24DFD(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Vector3 local_F4;
  script_24548();
  task_8B2 = 1;
  SCNF_WaitFrames_0000(arg3);
  Script_charaToggleMotEnabled(arg0, 1);
  Script_motmToggleFlags(arg0, 1, 0x40);
  Script_SetCharaMotionPlaybackRate(arg0, 1.5f);
  Script_PlayCharaMotion(arg0, 20998, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  SCNF_WaitFrames_0000(30);
  Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
  script_24CB5(arg0, arg1, arg2, 0);
  Script_EVT_GetPosition(arg1, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  SCNF_CHAR_SCNFApplyFacingTowardPoint(arg0, &local_F4, 0, 0.0f, 0, 0x200000);
  script_2449A(arg0);
  Script_PlayCharaMotion(arg0, 21000, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  task_8B2 = 0;
  return;
}

static void script_24EC5(int32_t arg0, int32_t arg1, int32_t arg2) {
  Vector3 local_F4;
  task_8B2 = 1;
  Script_EVT_GetPosition(arg1, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  Script_SetLookPointDetails(arg0, 11, &local_F4, 0);
  SCNF_WaitFrames_0000(arg2);
  Script_EVT_GetPosition(arg1, 5, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  Script_SetLookPointDetails(arg0, 11, &local_F4, 0);
  task_8B2 = 0;
  script_24548();
  return;
}

static int32_t script_24F1B(void) {
  task_8EC = 1;
  task_8F0 = 0;
  for (;;) {
    if (task_8EC) {
      task_8EC = task_8EC - 1;
      if (task_8EC == 0) {
        task_8F0 = 0;
        Script_LCDM_Utils(0, (const void *)task_8F0);
        Script_sicnUtils(2, 0x800, 0);
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_24F70(int32_t arg0) {
  task_8F0 = arg0;
  if (task_8F0 == 0) {
    task_8EC = 0;
    Script_LCDM_Utils(1, 0);
    Script_sicnUtils(2, 0x800, 0);
  } else {
    task_8EC = 600;
    Script_LCDM_Utils(1, (const void *)1);
    Script_sicnUtils(0, 0x800, 0);
  }
  Script_LCDM_Utils(0, (const void *)task_8F0);
  return;
}

static void script_24FCA(const char *arg0, const char *arg1) {
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, arg0, arg1) == 0) {
    Script_sprintf("WazaDen: Load PackFile %s:%s\012", arg0, arg1);
    SCNF_WaitFrames_0000(1);
  }
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2)) {
    Script_sprintf("WazaDen: Now Loading PackFile.\012");
    SCNF_WaitFrames_0000(1);
  }
  return;
}
