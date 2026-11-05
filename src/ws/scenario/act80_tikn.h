#pragma once

#include "scenario.h"
#include "motion/motion.h"

static void script_0030(int32_t arg0);
static void script_00CC(void);
static void script_010E(void);
static void script_019A(void);
static void script_0318(int32_t arg0, int32_t arg1);
static int32_t script_03FF(void);
static void script_042F(void);
static void script_0455(void);
static void script_0466(int32_t arg0);
static void script_0500(int32_t arg0);
static int32_t script_0550(void);
static void script_057E(int32_t arg0);
static void script_05AC(void);
static void script_05E0(int32_t arg0);
static void script_0614(int32_t arg0);
static void script_0632(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_066C(int32_t arg0, int32_t arg1);
static void script_06A0(void);
static void script_0798(int32_t arg0, int32_t arg1);
static void script_07C8(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_140D(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_1479(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_1965(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_1F51(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_2A5E(int32_t arg0, DSWKDoorEntry *arg1, int32_t *arg2, DoorResourceDescriptor *arg3, MapObjectRecord *arg4, int32_t arg5);
static void script_2BC6(DSWKDoorEntry *arg0, int32_t *arg1, DoorResourceDescriptor *arg2, MapObjectRecord *arg3, int32_t arg4);
static void script_2BFA(void);
static void script_2C22(void);
static void script_2C25(int32_t arg0);
static void script_2C28(int32_t arg0, int32_t arg1);
static void script_2C35(int32_t arg0, int32_t arg1);
static void script_2C38(int32_t arg0, int32_t arg1);
static void script_2C45(int32_t arg0);
static void script_2C5E(void);
static void script_2CC1(void);
static void script_2D24(int32_t arg0);
static void script_2D5B(int32_t arg0);
static void script_2D92(int32_t arg0);
static void script_2D9C(int32_t arg0);
static void script_2E9E(int32_t arg0);
static void script_2EDE(int32_t arg0);
static int32_t script_2F1E(void);
static void script_39D4(int32_t arg0);
static int32_t script_3B1E(int32_t arg0, int32_t arg1, int32_t arg2, float arg3, float arg4, int32_t arg5);
static void script_4067(int32_t arg0, int32_t arg1);
static void script_408F(int32_t arg0, int32_t arg1, int32_t arg2);
static int32_t script_419D(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_4259(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_4348(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_43CC(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_43DC(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_446E(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_44BB(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_4520(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_4585(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_45D2(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_4607(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_466C(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_46D1(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_4716(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_47AB(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_4E4A(void);
static void script_4E4D(void);
static void script_4E50(int32_t arg0, float arg1, float arg2, float arg3);
static void script_4E78(int32_t arg0, float arg1, float arg2, float arg3);
static int32_t script_4ECC(int32_t arg0, int32_t arg1, int32_t arg2);
static int32_t script_4F1C(int32_t arg0);
static void script_4FC3(int32_t arg0, int32_t arg1);
static int32_t script_4FE6(int32_t arg0);
static void script_5019(int32_t arg0, int32_t arg1);
static void script_503C(int32_t arg0, int32_t arg1);
static int32_t script_509B(int32_t arg0);
static void script_50B0(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_5446(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_5AD4(void);
static void script_5B75(void);
static void script_5B97(void);
static void script_5B9A(int32_t arg0);
static int32_t script_5BBB(int32_t arg0);
static void script_5BE4(int32_t arg0);
static void script_5C66(void);
static void script_5C91(int32_t arg0);
static int8_t script_5D63(int32_t arg0);
static int32_t script_5D98(void);
static void script_5DB1(void);
static void script_5DD0(void);
static int32_t script_5DE3(void);
static void script_65BF(void);
static void script_65CD(void);
static void script_65D5(void);
static int32_t script_65E0(void);
static int32_t script_65EB(void);
static void script_668B(void);
static void script_66AB(void);
static void script_66CE(void);
static void script_66F1(int32_t arg0);
static void script_6866(int32_t arg0);
static void script_6983(int32_t arg0);
static void script_69EE(int32_t arg0, int32_t arg1);
static int32_t script_6ADC(int32_t arg0);
static void script_6B53(int32_t arg0);
static void script_6B9B(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_6EE8(int32_t arg0, int32_t arg1);
static void script_6F60(int32_t arg0, int32_t arg1, const uint16_t *arg2);
static void script_7035(int32_t arg0, int32_t arg1);
static void script_7274(int32_t arg0, float arg1, float arg2, float arg3, const uint16_t *arg4);
static void script_731B(int32_t arg0);
static void script_73A9(void);
static void script_73B5(void);
static int32_t script_73DC(int32_t arg0, float arg1, float arg2, float arg3, int32_t arg4);
static void script_747A(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4);
static int32_t script_7492(int32_t arg0, int32_t arg1, float arg2, float arg3, float arg4, int32_t arg5, int32_t arg6, int32_t arg7);
static void script_7678(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5, int32_t arg6, int32_t arg7);
static void script_769C(int32_t arg0);
static void script_76AF(float arg0, float arg1, float arg2, float arg3);
static void script_76EA(const Vector3 *arg0, float arg1);
static void script_770D(void);
static void script_7717(int32_t arg0, int32_t arg1);
static int32_t script_774A(void);
static void script_78FE(void);
static int32_t script_7912(void);
static void script_7A3A(void);
static int32_t script_7A4E(int32_t arg0);
static int32_t script_7ACF(void);
static void script_7B2A(void);
static void script_7B84(int32_t arg0);
static void script_7BCC(void);
static void script_7BE4(int32_t arg0);
static void script_7C09(int32_t arg0);
static int32_t script_7C2E(int32_t arg0, int32_t arg1);
static void script_7C53(void);
static void script_7C5F(void);
static void script_7C62(void);
static void script_7C6E(void);
static void script_7C7A(void);
static int32_t script_7C86(void);
static int32_t script_7C92(void);
static int32_t script_7C9E(void);
static void script_7CAA(void);
static int32_t script_7CAD(void);
static int32_t script_7CB9(void);
static void script_7CC5(void);
static int32_t script_7CD1(void);
static int32_t script_7CDD(void);
static void script_7CE9(void);
static void script_7CF1(void);
static int32_t script_7CF9(void);
static int32_t script_7D0D(void);
static int32_t script_7D19(void);
static int32_t script_7D25(void);
static void script_7D31(void);
static void script_7D39(void);
static int32_t script_7D45(void);
static int32_t script_7D91(void);
static int32_t script_7D9D(void);
static int32_t script_7DA9(void);
static int32_t script_7DB5(void);
static void script_7DC1(void);
static int32_t script_7DC4(void);
static int32_t script_7DD0(void);
static int32_t script_7DDC(void);
static int32_t script_7DE8(void);
static int32_t script_7DF4(void);
static void script_7E00(void);
static void script_7E0C(void);
static void script_7E18(void);
static int32_t script_7E24(void);
static int32_t script_7E30(void);
static int32_t script_7E3C(void);
static int32_t script_7E48(void);
static int32_t script_7E54(void);
static void script_7E60(void);
static void script_7E6C(void);
static int32_t script_7E78(void);
static int32_t script_7E84(void);
static int32_t script_7E90(void);
static int32_t script_7E9C(void);
static int32_t script_7EA8(void);
static int32_t script_7EB4(void);
static int32_t script_7EC0(void);
static int32_t script_7ECC(void);
static int32_t script_7ED8(void);
static void script_7EE4(void);
static void script_7EF0(void);
static int32_t script_7EFC(void);
static int32_t script_7F08(void);
static int32_t script_7F14(void);
static int32_t script_7F20(void);
static void script_7F23(void);
static void script_7F2F(void);
static void script_7F3B(int32_t arg0);
static void script_7F66(int32_t arg0);
static int32_t script_7F94(int32_t arg0);
static void script_7FB1(float arg0, float arg1, float arg2, float arg3, float arg4, float arg5);
static int32_t script_7FE8(int32_t arg0, float arg1, float arg2, float arg3, float arg4);
static void script_800F(int32_t arg0, float arg1, float arg2, float arg3, float arg4);
static void script_8054(int32_t arg0);
static void script_8070(int32_t arg0, float arg1);
static int32_t script_8094(int32_t arg0, float arg1);
static void script_80AA(int32_t arg0, float arg1);
static void script_80CF(int32_t arg0, float arg1);
static int32_t script_80FC(int32_t arg0, float arg1);
static int32_t script_8115(int32_t arg0, float arg1);
static void script_812B(int32_t arg0, float arg1);
static int32_t script_8150(int32_t arg0, float arg1);
static void script_8169(int32_t arg0, float arg1);
static int32_t script_818E(int32_t arg0, float arg1, float arg2, int32_t arg3, int32_t arg4);
static int32_t script_826F(int32_t arg0, float arg1, float arg2, int32_t arg3, int32_t arg4);
static void script_829A(int32_t arg0, float arg1, float arg2, int32_t arg3, int32_t arg4);
static void script_8332(int32_t arg0, float arg1, float arg2, float arg3, int32_t arg4, int32_t arg5);
static void script_83A9(int32_t arg0, float arg1, float arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_83F5(int32_t arg0);
static void script_8414(int32_t arg0);
static int32_t script_8433(int32_t arg0, int32_t arg1, int32_t arg2);
static int32_t script_8448(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_8517(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_85AF(int32_t arg0, int32_t arg1, float arg2);
static void script_85F6(int32_t arg0);
static void script_8624(int32_t arg0);
static int32_t script_865C(int32_t arg0, int32_t arg1);
static void script_86B5(int32_t arg0, float arg1, float arg2, int32_t arg3, int32_t arg4, int8_t arg5);
static void script_873B(int32_t arg0);
static void script_88F2(void);
static void script_89FD(int32_t arg0);
static void script_8A20(int32_t arg0, int32_t arg1, void *arg2, int32_t arg3, const void *arg4);
static void script_8A99(int32_t arg0, int32_t arg1, void *arg2, int32_t arg3, const void *arg4);
static int32_t script_8B12(int32_t arg0);
static int32_t script_8BB4(int32_t arg0);
static int32_t script_8C56(int32_t arg0);
static int32_t script_8D1A(int32_t arg0);
static int32_t script_8DFA(int32_t arg0);
static int32_t script_8E47(int32_t arg0);
static void script_8EAC(int32_t arg0, int32_t arg1);
static void script_8ECC(int32_t arg0);
static void script_8EE0(int32_t arg0);
static void script_8EF4(int32_t arg0, int32_t arg1, float arg2, float arg3);
static void script_8F66(int32_t arg0, int32_t arg1);
static void script_8FA0(int32_t arg0, int32_t arg1, int32_t arg2);
static int32_t script_8FC6(int32_t arg0);
static int32_t script_92D0(int32_t arg0);
static void script_93BC(int32_t arg0, int32_t arg1);
static int32_t script_947B(void);
static int32_t script_95E8(void);
static int32_t script_9668(void);
static int32_t script_98F8(int32_t arg0);
static void script_99CE(void);
static int32_t script_9A22(void);
static void script_A080(void);
static int32_t script_A0B2(void);
static int32_t script_A0E7(int32_t arg0);
static int32_t script_A39D(int32_t arg0);
static int32_t script_A3DC(int32_t arg0);
static int32_t script_A54A(int32_t arg0);
static int32_t script_ACD7(int32_t arg0);
static int32_t script_AE76(int32_t arg0);
static int32_t script_AF8F(int32_t arg0);
static int32_t script_B0AA(int32_t arg0);
static int32_t script_B1F3(int32_t arg0);
static int32_t script_B2E1(void);
static int32_t script_C50A(void);
static int32_t script_C51F(void);
static int32_t script_C534(float arg0, float arg1, float arg2, float arg3);
static int32_t script_C55B(void);
static int32_t script_C590(void);
static int32_t script_C59C(void);
static int32_t script_C5A8(void);
static int32_t script_C5EB(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_C633(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static int32_t script_C64D(int32_t arg0, float arg1, int32_t arg2);
static void script_C673(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_C687(void);
static void script_C6C6(int32_t arg0);
static int32_t script_C753(void);
static int32_t script_C82B(int32_t arg0);
static int32_t script_C947(int32_t arg0);
static int32_t script_CA4D(int32_t arg0);
static int32_t script_CCD9(int32_t arg0);
static int32_t script_CDD0(int32_t arg0);
static int32_t script_CFB5(int32_t arg0);
static void script_D0B0(int32_t arg0);
static int32_t script_D0EB(int32_t arg0);
static int32_t script_DD59(int32_t arg0);
static int32_t script_DE35(int32_t arg0);
static int32_t script_DED9(int32_t arg0);
static int32_t script_DFD1(void);
static void script_FA25(int32_t arg0, int32_t arg1, float arg2, float arg3, float arg4);
static int32_t script_FA88(void);
static int32_t script_FB56(void);
static int32_t script_FC34(int32_t arg0, float arg1);
static int32_t script_FC6D(void);
static void script_FDE9(void);
static int32_t script_FE5C(int32_t arg0, int32_t arg1, int32_t arg2);
static int32_t script_102C2(void);
static int32_t script_103D9(void);
static int32_t script_1046C(void);
static int32_t script_104FF(void);
static int32_t script_10558(void);
static void script_105C4(int32_t arg0);
static void script_10699(void);
static void script_10812(void);
static int32_t script_10958(int32_t arg0, int32_t arg1);
static int32_t script_1097C(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4);
static int32_t script_10A53(int32_t arg0, int32_t arg1, int32_t arg2, float arg3, int32_t arg4, int32_t arg5);
static int32_t script_10AEA(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4);
static int32_t script_10B4C(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_10BA8(int32_t arg0, float arg1, int32_t arg2);
static void script_10C05(int32_t arg0);
static int32_t script_10C21(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, void *arg4, int32_t arg5);
static void script_10E41(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_10E78(int32_t arg0);
static void script_10E88(void);
static void script_10EA8(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_10EE2(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_10F2E(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4);
static int32_t script_10F77(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_10FA5(int32_t arg0, int32_t arg1);
static int32_t script_11066(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_11154(void);
static void script_11272(int32_t arg0, int32_t arg1);
static void script_112CB(int32_t arg0, int32_t arg1, float arg2, float arg3, float arg4);
static void script_1133B(int32_t arg0, int32_t arg1, const Vector3 *arg2);
static void script_11367(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static int32_t script_11381(int32_t arg0, int32_t arg1, float arg2, float arg3, float arg4, int32_t arg5);
static void script_113AC(int32_t arg0, int32_t arg1, float arg2);
static void script_11400(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_11416(int32_t arg0, int32_t arg1, float arg2, int32_t arg3);
static void script_1143D(int32_t arg0, float arg1);
static void script_11466(int32_t arg0, float arg1, float arg2, float arg3);
static void script_114BA(int32_t arg0, int32_t arg1, int32_t arg2);
static int32_t script_114CE(int32_t arg0, float arg1, int32_t arg2);
static void script_114F3(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4);
static int32_t script_1150B(int32_t arg0, float arg1, float arg2, float arg3, int32_t arg4);
static void script_11534(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4);
static int32_t script_1154C(int32_t arg0, float arg1, float arg2, float arg3, int32_t arg4);
static void script_115A1(int32_t arg0, int32_t arg1);
static void script_11686(int32_t arg0, int32_t arg1);
static int32_t script_1176F(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_11BC0(int32_t arg0, int32_t arg1);
static int32_t script_11BD2(int32_t arg0, int32_t arg1);
static void script_12052(int32_t arg0, int32_t arg1);
static int32_t script_12064(int32_t arg0, int32_t arg1);
static void script_12088(int32_t arg0, int32_t arg1);
static int32_t script_1209A(int32_t arg0, int32_t arg1);
static void script_120BB(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_120D1(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_120F5(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_1210B(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_1214B(int32_t arg0, int32_t arg1);
static void script_12166(int32_t arg0, int32_t arg1, int32_t arg2);
static int32_t script_1217A(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_1219F(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4);
static int32_t script_121B7(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4);
static void script_121DE(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5, int32_t arg6);
static int32_t script_121FA(int32_t arg0, int32_t arg1, int32_t arg2, float arg3, int32_t arg4, int32_t arg5, int32_t arg6);
static void script_1229B(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_122B1(float arg0, float arg1, float arg2, int32_t arg3);
static void script_122D6(int32_t arg0, int32_t arg1);
static void script_122E7(int32_t arg0, int32_t arg1);
static int32_t script_1249F(int32_t arg0, int32_t arg1);
static int32_t script_12892(int32_t arg0, int32_t arg1);
static void script_132DE(int32_t arg0, int32_t arg1);
static void script_13649(int32_t arg0, int32_t arg1);
static void script_13D65(int32_t arg0, int32_t arg1);
static int32_t script_140E1(int32_t arg0, int32_t arg1);
static void script_14137(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4);
static int32_t script_1414F(int32_t arg0, int32_t arg1, int32_t arg2, float arg3, float arg4);
static void script_141A8(int32_t arg0, int32_t arg1);
static void script_141F5(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_1420B(int32_t arg0, int32_t arg1, int32_t arg2, float arg3);
static void script_1426E(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_14295(void);
static void script_14509(void);
static void script_14530(void);
static void script_14700(void);
static int32_t script_1486C(int32_t arg0, int32_t arg1);
static int32_t script_149D6(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_14A99(int32_t arg0);
static void script_14C38(void);
static void script_14CB8(void);
static void script_14CEC(int32_t arg0);
static void script_14D27(int32_t arg0);
static void script_14D51(int32_t arg0);
static void script_14D95(int32_t arg0);
static void script_15366(void);
static void script_153A6(void);
static void script_15401(int32_t arg0);
static void script_1543C(int32_t arg0);
static void script_154CC(void);
static int32_t script_154E0(int32_t arg0);
static void script_1562C(int32_t arg0);
static void script_15649(void);
static int32_t script_15668(int32_t arg0, const char *arg1);
static void script_156DD(const char *arg0);
static int32_t script_156FE(int32_t arg0, const char *arg1, int32_t arg2);
static int32_t script_15743(int32_t arg0, int32_t arg1);
static void script_15771(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_15785(int32_t arg0, int32_t arg1);
static int32_t script_15797(const char *arg0, int32_t arg1);
static void script_157C8(int32_t arg0, int32_t arg1);
static int32_t script_157DA(int32_t arg0, int32_t arg1, int32_t arg2, const Vector3 *arg3, const Vector3i *arg4, int32_t arg5);
static void script_1580D(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static int32_t script_15827(int32_t arg0, int32_t arg1);
static void script_1584F(int32_t arg0, int32_t arg1);
static void script_15861(int32_t arg0);
static int32_t script_1587D(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4);
static void script_158DF(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4);
static int32_t script_158F7(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_15935(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_1594B(int32_t arg0, int32_t arg1, const Vector3i *arg2, int32_t arg3);
static void script_1597C(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_15992(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4);
static void script_159C3(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4);
static int32_t script_159DB(int32_t arg0, int32_t arg1, const Vector3 *arg2, int32_t arg3);
static void script_15A28(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_15A3E(int32_t arg0, int32_t arg1, const Vector3i *arg2, int32_t arg3);
static void script_15A8D(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_15AA3(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_15AD1(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_15AE7(int32_t arg0);
static int32_t script_15B26(int32_t arg0);
static int32_t script_15B4C(int32_t arg0);
static void script_15B6A(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_15C1C(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_15C4E(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_15D1B(void);
static void script_15D4E(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_15E6C(int32_t arg0);
static void script_15EE3(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_15F65(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_1601B(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4);
static void script_1620E(int32_t arg0, int32_t arg1);
static void script_162AF(int32_t arg0, int32_t arg1);
static int32_t script_164CB(int32_t arg0);
static void script_16943(int32_t arg0);
static void script_1695E(void);
static int32_t script_169B0(void);
static void script_169B7(void);
static int32_t script_169C4(void);
static void script_16B95(int32_t arg0);
static void script_16D42(int32_t arg0);
static void script_16E48(int32_t arg0, int32_t arg1, float arg2, float arg3, float arg4, float arg5, int32_t arg6);
void script_171D4(void);
static void script_172BB(void);
static void script_172F0(void);
static void script_173AE(void);
static void script_1740B(int32_t arg0, int32_t arg1);
static int32_t script_17476(int32_t arg0, int32_t arg1);
static int32_t script_174B6(void);
static void script_176B3(void);
static void script_176FC(void);
static void script_17774(void);
static void script_177F6(void);
static void script_1781B(void);
static void script_178AB(void);
static void script_1791E(const char *arg0, const char *arg1);
static void script_17975(void);
static void script_17C45(void);
static int32_t script_17ED7(void);
static int32_t script_17EFC(void);
static void script_17F8A(void);
static int32_t script_1812E(void);
static int32_t script_1821F(void);
static void script_18440(void);
static int32_t script_1845F(int32_t arg0);
static void script_184C6(int32_t arg0, int32_t arg1);

static int8_t task_0;
static int8_t task_1;
static int8_t task_2;
static int8_t task_3;
static DSWKDoorEntry *task_4;
static int32_t *task_8;
static DoorResourceDescriptor *task_C;
static MapObjectRecord *task_10;
static DoorSlotState task_14;
static int32_t task_20[11];
static int32_t task_4C[11];
static int32_t task_78[11];
static int32_t task_A4[11];
static int32_t task_D0[11];
static int32_t task_FC[11];
static int32_t task_128;
static int32_t task_12C;
static Vector3 task_130;
static int32_t task_13C;
static int32_t task_140;
static int32_t task_144;
static int32_t task_148;
static int8_t task_14C;
static uint8_t task_14D;
static uint16_t task_14E;
static int32_t task_150;
static int32_t task_154[8];
static int32_t task_174[8];
static int32_t task_194;
static int32_t task_198;
static int32_t task_19C;
static int32_t task_1A0;
static int32_t task_1A4;
static int32_t task_1A8;
static int32_t task_1AC;
static int32_t task_1B0;
static int32_t task_1B4;
static int32_t task_1B8;
static int32_t task_1BC;
static int32_t task_1C0;
static int32_t task_1C4;
static int32_t task_1C8;
static int32_t task_1CC;
static int32_t task_1D0;
static int32_t task_1D4;
static int8_t task_1D8;
static int8_t task_1D9;
static int8_t task_1DA;
static int8_t task_1DB;
static int8_t task_1DC;
static int8_t task_1DD;
static int8_t task_1DE;
static int8_t task_1DF;
static GameDateTime task_1E0;
static int32_t task_1E8;
static int32_t task_1EC;
static int32_t task_1F0;
static int32_t task_1F4;
static int32_t task_1F8;
static int8_t task_1FC;
static int8_t task_1FD;
static int8_t task_1FE;
static int8_t task_1FF;
static int8_t task_200;
static int8_t task_201;
static int8_t task_202;
static int8_t task_203;
static int32_t task_204;
static int32_t task_208[13];
static int32_t task_23C[3];
static int32_t task_248;
static int32_t task_24C;
static int8_t task_250;
static int8_t task_251;
static int8_t task_252;
static int8_t task_253;
static int8_t task_254;
static int8_t task_255;
static int8_t task_256;
static int8_t task_257;
static int32_t task_258[9];
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
static int32_t task_288;
static int32_t task_28C;
static int32_t task_290;
static int8_t task_294;
static uint8_t task_295;
static uint16_t task_296;
static int8_t task_298;
static uint8_t task_299;
static uint16_t task_29A;
static GameDateTime task_29C;
static int32_t task_2A4;
static int32_t task_2A8;
static int32_t task_2AC;
static int32_t task_2B0;
static int32_t task_2B4[3];
static int32_t task_2C0[3];
static int32_t task_2CC[3];
static int32_t task_2D8[3];
static int32_t task_2E4[3];
static int32_t task_2F0;
static int32_t task_2F4[3];
static int32_t task_300[3];
static int32_t task_30C[3];
static int32_t task_318[3];
static int32_t task_324;
static int32_t task_328;
static int32_t task_32C;
static int32_t task_330;
static int32_t task_334;
static int32_t task_338[3];
static float task_344[3][3];
static float task_368[3];
static float task_374;
static float task_378;
static float task_37C;
static float task_380[3][3];
static int32_t task_3A4[3][3];
static float task_3C8[3];
static int32_t task_3D4[3];
static int8_t task_3E0;
static uint8_t task_3E1;
static uint16_t task_3E2;
static int8_t task_3E4;
static uint8_t task_3E5;
static uint16_t task_3E6;
static int32_t task_3E8;
static int8_t task_3EC;
static uint8_t task_3ED;
static uint16_t task_3EE;
static int8_t task_3F0;
static uint8_t task_3F1;
static uint16_t task_3F2;
static int32_t task_3F4;
static char *task_3F8;
static int32_t task_3FC;
static int32_t task_400;
static int32_t task_404;
static int8_t task_408;
static int8_t task_409;
static int8_t task_40A;
static int8_t task_40B;
static int8_t task_40C;
static uint8_t task_40D;
static uint16_t task_40E;
static Vector3 task_410;
static const Vector3 data_184E0 = {.x = 1.0f, .y = 1.0f, .z = 1.0f};
static const int32_t data_184EC[4] = {16777215, 16777215, 16777215, 16777215};
static const float data_184FC[3] = {20.0f, 800.0f, 1.0f};
static const int32_t data_18508 = -1;
static const char data_1850C[11] __attribute__((scn_string_literal)) = "TS1003A001";
static const char data_18517[11] __attribute__((scn_string_literal)) = "TS1003A002";
static const char data_18522[11] __attribute__((scn_string_literal)) = "TS1003A003";
static const char data_1852D[7] __attribute__((scn_string_literal)) = "save!\012";
static const uint32_t data_18534 = 0xC3D48000;
static const float data_18538[2] = {-1.8f, -210.0f};
static const char data_18540[] __attribute__((scn_string_literal)) = "scene/02/model";
static const char data_1854F[] __attribute__((scn_string_literal)) = "C1305S3G.MT5";
static const char data_1855C[] __attribute__((scn_string_literal)) = "C1305S1G.MT5";
static const uint8_t pad_18569[239] = {0};
static const int32_t data_18658[340] = {
  -10922, 0, 0, -10558, 0, 1456, 0, 0, 0, -18022, 0, 0, -17112, 0, 0, -18386, 0, 0, 0, 0, 0, -12196, 0, 0, -18204, 0, 0, -16748, 0, 0, 0, 0, 0,
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
static const int32_t data_18BA8[57] = {
  0, 0, -5461, 0, -2366, -1092, 0, -182, 1092, 0, -546, -6735, 0, 0, -4004, 0, 0, -6553, 0, -182, 1274, 0, 0, -13289, 0, 0, -5279, 0, 0, -8009, 0, 0,
  1456, 0, 182, -8738, 0, 0, -6007, 0, 0, -8738, 0, 182, 1638, 0, -728, -5279, 0, -1274, -6735, 0, -910, -8738, 0, 0, 1638
};
static const int32_t data_18C8C[285] = {
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
static DoorMotionSelectorBlock data_19100[2] = {
  {.motion_desc_index = {0, 0, 0, 0, 7, 6, 6, 7, 7, 6, 6, 7, 6, 7, 7, 6, 6, 7, 7, 6, 6, 7, 7, 6, 6, 7, 7, 6, 7, 6, 6, 7, 7, 6, 6, 7, 1, 1, 1, 1}},
  {.motion_desc_index = {1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 6, 7, 7, 6, 6, 7, 7, 6, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2}}
};
static const uint32_t data_19150[9] = {0x2020202U, 0x3030303U, 0x3030303U, 0x3030303U, 0x4050504U, 0x4050504U, 0x5040405U, 0x5040405U, 0x8080808U};
static ScnDoorMotionDescriptor data_19174[5] = {
  {.motion_id = -1, .frame_open_sfx_begin = 0.0f, .frame_phase1_end = 0.0f, .frame_close_sfx_begin = 0.0f, .frame_motion_end = 0.0f, .approach_off_x = 8.84219331e-43f, .approach_off_z = 19.0f, .npc_tail_end_frame = 52.0f, .npc_tail_delta_per_frame = 96.0f},
  {.motion_id = 1124335616, .frame_open_sfx_begin = 8.92627122e-43f, .frame_phase1_end = 19.0f, .frame_close_sfx_begin = 53.0f, .frame_motion_end = 0.0f, .approach_off_x = 0.0f, .approach_off_z = 8.91225823e-43f, .npc_tail_end_frame = 30.0f, .npc_tail_delta_per_frame = 64.0f},
  {.motion_id = 0, .frame_open_sfx_begin = 0.0f, .frame_phase1_end = 8.80015436e-43f, .frame_close_sfx_begin = 19.0f, .frame_motion_end = 47.0f, .approach_off_x = 0.0f, .approach_off_z = 0.0f, .npc_tail_end_frame = 7.56701171e-44f, .npc_tail_delta_per_frame = 33.0f},
  {.motion_id = 1116471296, .frame_open_sfx_begin = 0.0f, .frame_phase1_end = 0.0f, .frame_close_sfx_begin = 9.05238808e-43f, .frame_motion_end = 19.0f, .approach_off_x = 45.0f, .approach_off_z = 106.0f, .npc_tail_end_frame = 141.0f, .npc_tail_delta_per_frame = 9.09442703e-43f},
  {.motion_id = 1106247680, .frame_open_sfx_begin = 73.0f, .frame_phase1_end = 108.0f, .frame_close_sfx_begin = 145.0f, .frame_motion_end = 1.9127724e-42f, .approach_off_x = 40.0f, .approach_off_z = 73.0f, .npc_tail_end_frame = 0.0f, .npc_tail_delta_per_frame = 0.0f}
};
static void (*data_19228)(int32_t, int32_t, int32_t) = script_43CC;
static void (*const data_1922C)(int32_t, int32_t, int32_t, int32_t) = script_43DC;
static void (*const data_19230)(int32_t, int32_t, int32_t, int32_t) = script_43DC;
static void (*const data_19234)(int32_t, int32_t, int32_t, int32_t) = script_43DC;
static void (*const data_19238)(int32_t, int32_t, int32_t, int32_t) = script_43DC;
static void (*const data_1923C)(int32_t, int32_t, int32_t, int32_t) = script_43DC;
static void (*const data_19240)(int32_t, int32_t, int32_t, int32_t) = script_43DC;
static void (*const data_19244)(int32_t, int32_t, int32_t, int32_t) = script_43DC;
static void (*const data_19248)(int32_t, int32_t, int32_t, int32_t) = script_43DC;
static void (*const data_1924C)(int32_t, int32_t, int32_t, int32_t) = script_44BB;
static void (*const data_19250)(int32_t, int32_t, int32_t, int32_t) = script_4520;
static void (*const data_19254)(int32_t, int32_t, int32_t, int32_t) = script_4607;
static void (*const data_19258)(int32_t, int32_t, int32_t, int32_t) = script_466C;
static void (*const data_1925C)(int32_t, int32_t, int32_t, int32_t) = script_446E;
static void (*const data_19260)(int32_t, int32_t, int32_t, int32_t) = script_446E;
static void (*const data_19264)(int32_t, int32_t, int32_t, int32_t) = script_4585;
static void (*const data_19268)(int32_t, int32_t, int32_t, int32_t) = script_45D2;
static void (*const data_1926C)(int32_t, int32_t, int32_t, int32_t) = script_46D1;
static void (*const data_19270)(int32_t, int32_t, int32_t, int32_t) = script_44BB;
static void (*const data_19274)(int32_t, int32_t, int32_t, int32_t) = script_4520;
static void (*const data_19278)(int32_t, int32_t, int32_t, int32_t) = script_4585;
static void (*const data_1927C)(int32_t, int32_t, int32_t, int32_t) = script_44BB;
static void (*const data_19280)(int32_t, int32_t, int32_t, int32_t) = script_4520;
static void (*const data_19284)(int32_t, int32_t, int32_t, int32_t) = script_4585;
static void (*const data_19288)(int32_t, int32_t, int32_t, int32_t) = script_4716;
static void (*const data_1928C)(int32_t, int32_t, int32_t, int32_t) = script_4716;
static void (*const data_19290)(int32_t, int32_t, int32_t, int32_t) = script_4716;
static void (*const data_19294)(int32_t, int32_t, int32_t, int32_t) = script_4716;
static void (*const data_19298)(int32_t, int32_t, int32_t, int32_t) = script_47AB;
static const char data_1929C[8] __attribute__((scn_string_literal)) = "%d - %d";
static const char data_192A4[10] __attribute__((scn_string_literal)) = "%04X : %d";
static const char data_192AE[14] __attribute__((scn_string_literal)) = "%f : %f : %f";
static Vector3 data_192BC = {.x = 0.88f, .y = 0.0f, .z = 0.0f};
static Vector3 data_192C8 = {.x = -0.88f, .y = 0.0f, .z = 0.0f};
static const DSWKDoorEntry data_192D4[4] = {
  {.slot = 0, .flags = 0, .ctrl_mask_pair = 0x640064U, .coarse_metric = 0.0f, .selection_metric = 0.0f, .view_ang_delta = 0},
  {.slot = 3, .flags = 0, .ctrl_mask_pair = 0x640064U, .coarse_metric = 0.0f, .selection_metric = 0.0f, .view_ang_delta = 0},
  {.slot = 6, .flags = 0, .ctrl_mask_pair = 0x640064U, .coarse_metric = 0.0f, .selection_metric = 0.0f, .view_ang_delta = 0},
  {.slot = 9, .flags = 0, .ctrl_mask_pair = 0x650065U, .coarse_metric = 0.0f, .selection_metric = 0.0f, .view_ang_delta = 0}
};
static const int32_t data_19334[6] = {-1, 0, 6553700, 0, 0, 0};
static const uint8_t data_1934C[4] = {0};
static const int32_t data_19350[10] = {1, -1, 2, 3, -1, 4, 5, -1, 6, -1};
static char data_19378[] = "DR00_000";
static char data_19381[] = "DR15_001";
static char data_1938A[] = "DR06_002";
static char data_19393[] = "DR17_000";
static char *const data_1939C = data_19378;
static const int32_t data_193A0[3] = {0, -1, -1};
static char *const data_193AC = data_19381;
static const int32_t data_193B0[3] = {15, 1180331, 1245867};
static char *const data_193BC = data_1938A;
static const int32_t data_193C0[3] = {6, 1442475, 1508011};
static char *const data_193CC = data_19393;
static const int32_t data_193D0[3] = {17, 656043, 721579};
static const MapObjectRecord data_193DC[7] = {
  {.kind = 1, .resourceIndex = 0, .scale = {0.75f, 1.0f, 1.0f}, .pos = {20.3689f, 0.082743f, 49.165401f}, .yawFixed16 = -16383},
  {.kind = 2, .resourceIndex = 1, .scale = {1.0f, 1.0f, 1.0f}, .pos = {20.368959f, 0.082742f, 49.165401f}, .yawFixed16 = -16383},
  {.kind = 1, .resourceIndex = 0, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-63.242302f, 0.072381f, 66.408524f}, .yawFixed16 = 49152},
  {.kind = 2, .resourceIndex = 1, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-63.242302f, 0.072381f, 66.408524f}, .yawFixed16 = -16384},
  {.kind = 1, .resourceIndex = 0, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-20.707703f, 0.0724f, 33.0093f}, .yawFixed16 = 7117},
  {.kind = 2, .resourceIndex = 2, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-20.010004f, 0.0724f, 32.440777f}, .yawFixed16 = 7117},
  {.kind = 2, .resourceIndex = 3, .scale = {1.0f, 1.0f, 1.0f}, .pos = {22.3589f, 0.0724f, 32.833599f}, .yawFixed16 = -32767}
};
static const uint16_t data_194D8[8] = {1068, 1043, 1069, 1071, 1074, 1072, 1073, 903};
static const char data_194E8[23] __attribute__((scn_string_literal)) = "FTLK: Get ID list(%s)\012";
static const char data_194FF[30] __attribute__((scn_string_literal)) = "FTLK: Illegal address (cast)\012";
static const char data_1951D[23] __attribute__((scn_string_literal)) = "FTLK: Too many person\012";
static const char data_19534[17] __attribute__((scn_string_literal)) = "FTLK: No person\012";
static const char data_19545[22] __attribute__((scn_string_literal)) = "FTLK: ATURN0%d start\012";
static const char data_1955B[20] __attribute__((scn_string_literal)) = "FTLK: ATURN0%d end\012";
static const char data_1956F[21] __attribute__((scn_string_literal)) = "FTLK: TURN0%d start\012";
static const char data_19584[19] __attribute__((scn_string_literal)) = "FTLK: TURN0%d end\012";
static const char data_19597[23] __attribute__((scn_string_literal)) = "FTLK: RETURN0%d start\012";
static const char data_195AE[21] __attribute__((scn_string_literal)) = "FTLK: RETURN0%d end\012";
static const char data_195C3[22] __attribute__((scn_string_literal)) = "FTLK: POINT0%d start\012";
static const char data_195D9[23] __attribute__((scn_string_literal)) = "FTLK: APOINT0%d start\012";
static const char data_195F0[20] __attribute__((scn_string_literal)) = "FTLK: POINT0%d end\012";
static const char data_19604[21] __attribute__((scn_string_literal)) = "FTLK: APOINT0%d end\012";
static const char data_19619[19] __attribute__((scn_string_literal)) = "FTLK: START start\012";
static const char data_1962C[32] __attribute__((scn_string_literal)) = "FTLK: Wait finish canseled(%d)\012";
static const char data_1964C[19] __attribute__((scn_string_literal)) = "FTLK: Start by BS\012";
static const char data_1965F[20] __attribute__((scn_string_literal)) = "FTLK: AKIRA ->> %s\012";
static const char data_19673[20] __attribute__((scn_string_literal)) = "FTLK: AKIRA ->> %s\012";
static const char data_19687[19] __attribute__((scn_string_literal)) = "FTLK: AKIRA -> %s\012";
static const char data_1969A[17] __attribute__((scn_string_literal)) = "FTLK: START end\012";
static const char data_196AB[17] __attribute__((scn_string_literal)) = "FTLK: END start\012";
static const char data_196BC[15] __attribute__((scn_string_literal)) = "FTLK: END end\012";
static const char data_196CB[20] __attribute__((scn_string_literal)) = "FTLK: FINISH start\012";
static const char data_196DF[24] __attribute__((scn_string_literal)) = "FTLK: FINISH send (%s)\012";
static const char data_196F7[18] __attribute__((scn_string_literal)) = "FTLK: FINISH end\012";
static const char data_19709[22] __attribute__((scn_string_literal)) = "FTLK: POINT_END star\012";
static const char data_1971F[21] __attribute__((scn_string_literal)) = "FTLK: POINT_END end\012";
static const char data_19734[24] __attribute__((scn_string_literal)) = "FTLK: APOINT_END start\012";
static const char data_1974C[22] __attribute__((scn_string_literal)) = "FTLK: APOINT_END end\012";
static const char data_19762[23] __attribute__((scn_string_literal)) = "FTLK: SHOW_MEMO start\012";
static const char data_19779[21] __attribute__((scn_string_literal)) = "FTLK: SHOW_MEMO end\012";
static const char data_1978E[22] __attribute__((scn_string_literal)) = "FTLK: SHOW_END start\012";
static const char data_197A4[20] __attribute__((scn_string_literal)) = "FTLK: SHOW_END end\012";
static const char data_197B8[18] __attribute__((scn_string_literal)) = "FTLK: WAIT start\012";
static const char data_197CA[16] __attribute__((scn_string_literal)) = "FTLK: WAIT end\012";
static const char data_197DA[19] __attribute__((scn_string_literal)) = "FTLK: EXTRA start\012";
static const char data_197ED[17] __attribute__((scn_string_literal)) = "FTLK: EXTRA end\012";
static const char data_197FE[23] __attribute__((scn_string_literal)) = "FTLK: Wait finish end\012";
static const char data_19815[29] __attribute__((scn_string_literal)) = "FTLK: Wait finish start(%d)\012";
static const char data_19832[21] __attribute__((scn_string_literal)) = "FTLK: %s not exists\012";
static const char data_19847[14] __attribute__((scn_string_literal)) = "FTLK: %s ->> ";
static const char data_19855[4] __attribute__((scn_string_literal)) = "%s\012";
static const char data_19859[16] __attribute__((scn_string_literal)) = "自転車乗ってる\012";
static const char data_19869[19] __attribute__((scn_string_literal)) = "FTLK: AKIRA -> %s\012";
static const char data_1987C[19] __attribute__((scn_string_literal)) = "FTLK: AKIRA -X %s\012";
static const char data_1988F[31] __attribute__((scn_string_literal)) = "FTLK: %s 's talk motion is %d\012";
static const char data_198AE[23] __attribute__((scn_string_literal)) = "FTLK: Turn canseled 1\012";
static const char data_198C5[29] __attribute__((scn_string_literal)) = "FTLK: Set motion %d to %s 1\012";
static const char data_198E2[23] __attribute__((scn_string_literal)) = "FTLK: Turn canseled 2\012";
static const char data_198F9[23] __attribute__((scn_string_literal)) = "FTLK: Turn canseled 3\012";
static const char data_19910[29] __attribute__((scn_string_literal)) = "FTLK: Set motion %d to %s 2\012";
static const char data_1992D[23] __attribute__((scn_string_literal)) = "FTLK: Turn canseled 4\012";
static const char data_19944[29] __attribute__((scn_string_literal)) = "FTLK: Set motion %d to %s 3\012";
static const char data_19961[23] __attribute__((scn_string_literal)) = "FTLK: Turn canseled 5\012";
static const char data_19978[30] __attribute__((scn_string_literal)) = "FTLK: Illegal addres (point)\012";
static const char data_19996[29] __attribute__((scn_string_literal)) = "FTLK: POINT TO (%f, %f, %f)\012";
static const char data_199B3[27] __attribute__((scn_string_literal)) = "FTLK: %s finished looking\012";
static const char data_199CE[20] __attribute__((scn_string_literal)) = "FTLK: %s not exist\012";
static const char data_199E2[20] __attribute__((scn_string_literal)) = "FTLK: %s not exist\012";
static const char data_199F6[20] __attribute__((scn_string_literal)) = "FTLK: %s not exist\012";
static const char data_19A0A[16] __attribute__((scn_string_literal)) = "自転車乗ってる\012";
static const char data_19A1A[22] __attribute__((scn_string_literal)) = "FTLK: %s not exist\012";
static const uint32_t data_19A30 = SHEN_NAME('ONO_');
static const shen_name data_19A34[11] = {
  CHARA_AKSK, CHARA_NOMR, CHARA_PAUR, CHARA_MNWA, CHARA_STAK, CHARA_MISM, CHARA_YOSE, CHARA_KOND, CHARA_SUMI, CHARA_NORK, CHARA_YMGC
};
static const int32_t data_19A60 = -1;
static const uint32_t data_19A64 = SHEN_NAME('HANA');
static const shen_name data_19A68 = CHARA_SOBA;
static const int32_t data_19A6C = -1;
static const uint32_t data_19A70 = SHEN_NAME('UDGW');
static const shen_name data_19A74 = CHARA_KYMA;
static const int32_t data_19A78 = -1;
static const char data_19A7C[11] __attribute__((scn_string_literal)) = "E1004AA001";
static const char data_19A87[11] __attribute__((scn_string_literal)) = "E1004AB001";
static const char data_19A92[11] __attribute__((scn_string_literal)) = "E1004AA002";
static const char data_19A9D[11] __attribute__((scn_string_literal)) = "E1004AB002";
static const char data_19AA8[11] __attribute__((scn_string_literal)) = "E1004AA003";
static const char data_19AB3[12] __attribute__((scn_string_literal)) = "TE1004AA003";
static const char data_19ABF[12] __attribute__((scn_string_literal)) = "TE1004AD001";
static const char data_19ACB[12] __attribute__((scn_string_literal)) = "TE1004AA004";
static const char data_19AD7[12] __attribute__((scn_string_literal)) = "TE1004AE001";
static const char data_19AE3[12] __attribute__((scn_string_literal)) = "TE1004AA006";
static const char data_19AEF[12] __attribute__((scn_string_literal)) = "TE1004AA005";
static const char data_19AFB[12] __attribute__((scn_string_literal)) = "TE1004AA026";
static const char data_19B07[13] __attribute__((scn_string_literal)) = "「よっと！」";
static const char data_19B14[15] __attribute__((scn_string_literal)) = "「おわっと！」";
static const char data_19B23[12] __attribute__((scn_string_literal)) = "TE1004AI001";
static const char data_19B2F[12] __attribute__((scn_string_literal)) = "TE1004AE012";
static const char data_19B3B[12] __attribute__((scn_string_literal)) = "TE1004AA010";
static const char data_19B47[12] __attribute__((scn_string_literal)) = "TE1004AA011";
static const char data_19B53[23] __attribute__((scn_string_literal)) = "「クソ、ジミーの野郎」";
static const char data_19B6A[12] __attribute__((scn_string_literal)) = "TE1004AA024";
static const char data_19B76[12] __attribute__((scn_string_literal)) = "TE1004AA012";
static const char data_19B82[12] __attribute__((scn_string_literal)) = "TE1004AA013";
static const char data_19B8E[12] __attribute__((scn_string_literal)) = "TE1004AA014";
static const char data_19B9A[12] __attribute__((scn_string_literal)) = "TE1004AA015";
static const char data_19BA6[12] __attribute__((scn_string_literal)) = "TE1004AJ002";
static const char data_19BB2[12] __attribute__((scn_string_literal)) = "TE1004AJ001";
static const char data_19BBE[12] __attribute__((scn_string_literal)) = "TE1004AE003";
static const char data_19BCA[12] __attribute__((scn_string_literal)) = "TE1004AG002";
static const char data_19BD6[12] __attribute__((scn_string_literal)) = "TE1004AA023";
static const char data_19BE2[12] __attribute__((scn_string_literal)) = "TE1004AL002";
static const char data_19BEE[12] __attribute__((scn_string_literal)) = "TE1004AA017";
static const char data_19BFA[12] __attribute__((scn_string_literal)) = "TE1004AL003";
static const char data_19C06[11] __attribute__((scn_string_literal)) = "E1004AH001";
static const char data_19C11[12] __attribute__((scn_string_literal)) = "TE1004AC001";
static const char data_19C1D[11] __attribute__((scn_string_literal)) = "E1004AH003";
static const char data_19C28[12] __attribute__((scn_string_literal)) = "TE1004AA025";
static const char data_19C34[12] __attribute__((scn_string_literal)) = "TE1004AE004";
static const char data_19C40[12] __attribute__((scn_string_literal)) = "TE1004AA026";
static const char data_19C4C[12] __attribute__((scn_string_literal)) = "TE1004AN002";
static const char data_19C58[12] __attribute__((scn_string_literal)) = "TE1004AN001";
static const char data_19C64[12] __attribute__((scn_string_literal)) = "TE1004AO002";
static const char data_19C70[12] __attribute__((scn_string_literal)) = "TE1004AO004";
static const char data_19C7C[12] __attribute__((scn_string_literal)) = "TE1004AA008";
static const char data_19C88[12] __attribute__((scn_string_literal)) = "TE1004AO002";
static const char data_19C94[12] __attribute__((scn_string_literal)) = "TE1004AA023";
static const char data_19CA0[12] __attribute__((scn_string_literal)) = "TE1004AA016";
static const char data_19CAC[12] __attribute__((scn_string_literal)) = "TE1004AG001";
static const char data_19CB8[12] __attribute__((scn_string_literal)) = "TE1004AA001";
static const char data_19CC4[12] __attribute__((scn_string_literal)) = "TE1004AB002";
static const char data_19CD0[12] __attribute__((scn_string_literal)) = "TE1004AC002";
static const char data_19CDC[12] __attribute__((scn_string_literal)) = "TE1004AC001";
static const char data_19CE8[11] __attribute__((scn_string_literal)) = "E1004AI003";
static const char data_19CF3[11] __attribute__((scn_string_literal)) = "E1004AA022";
static const char data_19CFE[12] __attribute__((scn_string_literal)) = "TE1004AA007";
static const char data_19D0A[11] __attribute__((scn_string_literal)) = "E1004AL003";
static const char data_19D15[12] __attribute__((scn_string_literal)) = "TE1004AF001";
static const char data_19D21[11] __attribute__((scn_string_literal)) = "E1004AA040";
static const char data_19D2C[11] __attribute__((scn_string_literal)) = "E1004AB009";
static const char data_19D37[13] __attribute__((scn_string_literal)) = "QTE [%d]\012";
static const uint8_t data_19D44[4] = {0};
static const int32_t data_19D48[9] = {20, 15, 3, 10, 5, 14, 1, 13, 8};
static const char data_19D6C[] __attribute__((scn_string_literal)) = "BIKE";
static const uint8_t data_19D71[3] = {0};
static const uint32_t data_19D74 = 18;
static const int32_t data_19D78[6] = {0, 17, 1, 8, 12, 7};
static const char data_19D90[] __attribute__((scn_string_literal)) = "KANB";
static const uint8_t data_19D95[3] = {0};
static const uint8_t data_19D98[4] = {0};
static const int32_t data_19D9C[33] = {
  1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33
};
static const char data_19E20[] __attribute__((scn_string_literal)) = "APLE";
static const uint8_t data_19E25[3] = {0};
static const uint32_t data_19E28 = 153;
static const int32_t data_19E2C[16] = {154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 169, 172, 175};
static const char data_19E6C[] __attribute__((scn_string_literal)) = "SOBA";
static const char data_19E71[] __attribute__((scn_string_literal)) = "SOBA";
static const char data_19E76[] __attribute__((scn_string_literal)) = "SOBA";
static const uint8_t data_19E7B[1] = {0};
static const uint32_t data_19E7C = 153;
static const int32_t data_19E80[3] = {154, 155, 156};
static const char data_19E8C[] __attribute__((scn_string_literal)) = "HAKO";
static const uint8_t data_19E91[3] = {0};
static const uint32_t data_19E94 = 0x3A0039;
static const int32_t data_19E98[4] = {3670071, 4063291, 61, 1};
static const float data_19EA8[3] = {-67.88f, 79.77f, 0.0f};
static const int32_t data_19EB4[2] = {196608, 1};
static const float data_19EBC[3] = {-69.94f, 81.09f, 0.0f};
static const int32_t data_19EC8 = 196608;
static const uint32_t data_19ECC = 3;
typedef struct Row_19ED0 { float w0; float w1; float w2; int32_t w3; } Row_19ED0;
static const Row_19ED0 data_19ED0[7] = {
  {-80.03f, 81.41f, 1.0f, 4},
  {-85.53f, 82.61f, 1.0f, 5},
  {-90.83f, 84.61f, 1.0f, 6},
  {-98.78f, 89.06f, 1.0f, 7},
  {-107.03f, 99.61f, 1.0f, 8},
  {-109.43f, 100.61f, 1.0f, 9},
  {-114.5f, 98.9f, 0.0f, 196608}
};
static const uint32_t data_19F40 = 6;
typedef struct Row_19F44 { float w0; float w1; float w2; int32_t w3; } Row_19F44;
static const Row_19F44 data_19F44[4] = {
  {-98.78f, 89.06f, 1.0f, 7},
  {-107.03f, 99.61f, 1.0f, 8},
  {-109.43f, 100.61f, 1.0f, 9},
  {-114.5f, 98.9f, 0.0f, 196608}
};
static const uint32_t data_19F84 = 8;
typedef struct Row_19F88 { float w0; float w1; float w2; int32_t w3; } Row_19F88;
static const Row_19F88 data_19F88[2] = {
  {-109.43f, 100.61f, 1.0f, 9},
  {-114.5f, 98.9f, 0.0f, 196608}
};
static const uint32_t data_19FA8 = 5;
typedef struct Row_19FAC { float w0; float w1; float w2; int32_t w3; } Row_19FAC;
static const Row_19FAC data_19FAC[7] = {
  {-93.0f, 86.71f, 1.0f, 6},
  {-98.0f, 88.91f, 1.0f, 7},
  {-105.6f, 98.61f, 1.0f, 8},
  {-108.1f, 100.01f, 1.0f, 9},
  {-113.4f, 98.91f, 1.0f, 10},
  {-119.3f, 96.41f, 1.0f, 11},
  {-120.8f, 90.91f, 1.0f, 196608}
};
static const uint32_t data_1A01C = 0xC2EB147B;
static const float data_1A020[9] = {97.88f, -110.35f, 101.7f, -108.15f, 101.5f, -106.73f, 100.5f, -104.83f, 97.8f};
static const int32_t data_1A044 = 1;
static const float data_1A048[3] = {-93.3f, 85.65f, 1.0f};
static const int32_t data_1A054 = 2;
static const float data_1A058[3] = {-85.3f, 81.8f, 1.0f};
static const int32_t data_1A064 = 196608;
static const char data_1A068[12] __attribute__((scn_string_literal)) = "TE1004AA028";
static const char data_1A074[12] __attribute__((scn_string_literal)) = "TE1004AA002";
static const char data_1A080[18] __attribute__((scn_string_literal)) = "check count = %d\012";
static const char data_1A092[12] __attribute__((scn_string_literal)) = "TE1004AE005";
static const char data_1A09E[12] __attribute__((scn_string_literal)) = "TE1004AE006";
static const char data_1A0AA[12] __attribute__((scn_string_literal)) = "TE1004AG001";
static const char data_1A0B6[10] __attribute__((scn_string_literal)) = "LEFT[%d]\012";
static const char data_1A0C0[11] __attribute__((scn_string_literal)) = "RIGHT[%d]\012";
static const char data_1A0CB[12] __attribute__((scn_string_literal)) = "TE1004AA029";
static const char data_1A0D7[12] __attribute__((scn_string_literal)) = "TE1004AE007";
static const char data_1A0E3[12] __attribute__((scn_string_literal)) = "TE1004AA030";
static const char data_1A0EF[13] __attribute__((scn_string_literal)) = "TE1004AA033";
static const uint32_t data_1A0FC = 0xC226999A;
static const float data_1A100[4] = {38.85f, -50.85f, 46.15f, -56.35f};
static const shen_name data_1A110 = SHEN_NAME('33JB');
static const char data_1A114[12] __attribute__((scn_string_literal)) = "TE1004AO001";
static const char data_1A120[12] __attribute__((scn_string_literal)) = "TE1004AA005";
static const char data_1A12C[12] __attribute__((scn_string_literal)) = "TE1004AO003";
static const char data_1A138[12] __attribute__((scn_string_literal)) = "TE1004AA021";
static const char data_1A144[12] __attribute__((scn_string_literal)) = "TE1004AA022";
static const char data_1A150[12] __attribute__((scn_string_literal)) = "TE1004AA023";
static const char data_1A15C[12] __attribute__((scn_string_literal)) = "TE1004AO002";
static const char data_1A168[12] __attribute__((scn_string_literal)) = "TE1004AE009";
static const char data_1A174[12] __attribute__((scn_string_literal)) = "TE1004AE011";
static const char data_1A180[12] __attribute__((scn_string_literal)) = "TE1004AA030";
static const char data_1A18C[12] __attribute__((scn_string_literal)) = "UNDEL[%ld]\012";
static const char data_1A198[10] __attribute__((scn_string_literal)) = "DEL[%ld]\012";
static const char data_1A1A2[12] __attribute__((scn_string_literal)) = "UNDEL[%ld]\012";
static const char data_1A1AE[10] __attribute__((scn_string_literal)) = "DEL[%ld]\012";
static const char data_1A1B8[9] __attribute__((scn_string_literal)) = "CJIM_PAK";
static const char data_1A1C1[5] __attribute__((scn_string_literal)) = "CTER";
static const char data_1A1C6[8] __attribute__((scn_string_literal)) = "AUTHOR1";
static const char data_1A1CE[8] __attribute__((scn_string_literal)) = "FACEPAK";
static const char data_1A1D6[9] __attribute__((scn_string_literal)) = "STARTMOT";
static const char data_1A1DF[6] __attribute__((scn_string_literal)) = "SHUKI";
static const char data_1A1E5[7] __attribute__((scn_string_literal)) = "save!\012";
static const char data_1A1EC[] __attribute__((scn_string_literal)) = "/scene/80/TIKN/";
static const char data_1A1FC[] __attribute__((scn_string_literal)) = "CJIM_PAK";
static const char data_1A205[8] __attribute__((scn_string_literal)) = "PX: %f\012";
static const char data_1A20D[8] __attribute__((scn_string_literal)) = "PZ: %f\012";
static const char data_1A215[8] __attribute__((scn_string_literal)) = "AY: %f\012";
static const char data_1A21D[15] __attribute__((scn_string_literal)) = "DPos = %f, %f\012";
static const char data_1A22C[12] __attribute__((scn_string_literal)) = "DAng = %f\012";
static const uint32_t data_1A238 = 64;
static const int32_t data_1A23C[48] = {
  20, 10, 4, 0, -1, -1, -1, 1, -1, 5, 3, 256, 20, 10, 4, 0, -1, -1, -1, 1, -1, 7, 3, 512, 20, 10, 4, 0, -1, -1, -1, 9, -1, 11, 3, 256, 20, 10, 4, 0,
  -1, -1, 18, -1, 13, 15, 17, 1
};
static const float data_1A2FC[3] = {-113.1f, 99.6f, 1.0f};
static const int32_t data_1A308 = 196608;
static const char data_1A30C[9] __attribute__((scn_string_literal)) = "TE02A001";
static const char data_1A315[9] __attribute__((scn_string_literal)) = "TE02A002";
static const char data_1A31E[9] __attribute__((scn_string_literal)) = "TE02A003";
static const char data_1A327[9] __attribute__((scn_string_literal)) = "TE02A004";
static const char data_1A330[9] __attribute__((scn_string_literal)) = "TE02A005";
static const char data_1A339[9] __attribute__((scn_string_literal)) = "TE02A006";
static const char data_1A342[9] __attribute__((scn_string_literal)) = "TE02A007";
static const char data_1A34B[9] __attribute__((scn_string_literal)) = "TE02A008";
static const char data_1A354[9] __attribute__((scn_string_literal)) = "TE02A009";
static const char data_1A35D[9] __attribute__((scn_string_literal)) = "TE02A010";
static const char data_1A366[9] __attribute__((scn_string_literal)) = "TE02A011";
static const char data_1A36F[9] __attribute__((scn_string_literal)) = "TE02A012";
static const char data_1A378[9] __attribute__((scn_string_literal)) = "TE02A014";
static const char data_1A381[9] __attribute__((scn_string_literal)) = "TE02A013";
static const char data_1A38A[9] __attribute__((scn_string_literal)) = "TE02B001";
static const char data_1A393[9] __attribute__((scn_string_literal)) = "TE02B002";
static const char data_1A39C[9] __attribute__((scn_string_literal)) = "TE02B003";
static const char data_1A3A5[9] __attribute__((scn_string_literal)) = "TE02B004";
static const char data_1A3AE[9] __attribute__((scn_string_literal)) = "TE02B005";
static const char data_1A3B7[9] __attribute__((scn_string_literal)) = "TE02B006";
static const char data_1A3C0[9] __attribute__((scn_string_literal)) = "TE02B007";
static const char data_1A3C9[9] __attribute__((scn_string_literal)) = "TE02B008";
static const char data_1A3D2[9] __attribute__((scn_string_literal)) = "TE02B009";
static const char data_1A3DB[9] __attribute__((scn_string_literal)) = "TE02B010";
static const char data_1A3E4[9] __attribute__((scn_string_literal)) = "TE02B011";
static const char data_1A3ED[9] __attribute__((scn_string_literal)) = "TE02B012";
static const char data_1A3F6[9] __attribute__((scn_string_literal)) = "TE02B013";
static const char data_1A3FF[9] __attribute__((scn_string_literal)) = "TE02B014";
static const char data_1A408[9] __attribute__((scn_string_literal)) = "TE02B015";
static const char data_1A411[9] __attribute__((scn_string_literal)) = "TE02B016";
static const char data_1A41A[9] __attribute__((scn_string_literal)) = "TE02B017";
static const char data_1A423[9] __attribute__((scn_string_literal)) = "TE02B018";
static const char data_1A42C[9] __attribute__((scn_string_literal)) = "TE02B019";
static const char data_1A435[9] __attribute__((scn_string_literal)) = "TE02C001";
static const char data_1A43E[9] __attribute__((scn_string_literal)) = "TE02C002";
static const char data_1A447[9] __attribute__((scn_string_literal)) = "TE02C003";
static const char data_1A450[9] __attribute__((scn_string_literal)) = "TE02C004";
static const char data_1A459[9] __attribute__((scn_string_literal)) = "TE02C005";
static const char data_1A462[9] __attribute__((scn_string_literal)) = "TE02C006";
static const char data_1A46B[9] __attribute__((scn_string_literal)) = "TE02C007";
static const char data_1A474[9] __attribute__((scn_string_literal)) = "TE02C008";
static const char data_1A47D[9] __attribute__((scn_string_literal)) = "TE02C009";
static const char data_1A486[9] __attribute__((scn_string_literal)) = "TE02C010";
static const char data_1A48F[9] __attribute__((scn_string_literal)) = "TE02C011";
static const char data_1A498[9] __attribute__((scn_string_literal)) = "TE02C012";
static const char data_1A4A1[9] __attribute__((scn_string_literal)) = "TE02C013";
static const char data_1A4AA[9] __attribute__((scn_string_literal)) = "TE02C014";
static const char data_1A4B3[9] __attribute__((scn_string_literal)) = "TE02C015";
static const char data_1A4BC[9] __attribute__((scn_string_literal)) = "TE02C016";
static const char data_1A4C5[9] __attribute__((scn_string_literal)) = "TE02C017";
static const char data_1A4CE[9] __attribute__((scn_string_literal)) = "TE02C018";
static const char data_1A4D7[9] __attribute__((scn_string_literal)) = "TE02C020";
static const char data_1A4E0[9] __attribute__((scn_string_literal)) = "TE02C019";
static const char data_1A4E9[7] __attribute__((scn_string_literal)) = "開始０";
static const char data_1A4F0[7] __attribute__((scn_string_literal)) = "成功０";
static const char data_1A4F7[7] __attribute__((scn_string_literal)) = "失敗０";
static const char data_1A4FE[7] __attribute__((scn_string_literal)) = "開始１";
static const char data_1A505[7] __attribute__((scn_string_literal)) = "成功１";
static const char data_1A50C[7] __attribute__((scn_string_literal)) = "失敗１";
static const char data_1A513[7] __attribute__((scn_string_literal)) = "開始２";
static const char data_1A51A[7] __attribute__((scn_string_literal)) = "成功２";
static const char data_1A521[7] __attribute__((scn_string_literal)) = "失敗２";
static const char data_1A528[7] __attribute__((scn_string_literal)) = "開始３";
static const char data_1A52F[7] __attribute__((scn_string_literal)) = "成功３";
static const char data_1A536[7] __attribute__((scn_string_literal)) = "失敗３";
static const char data_1A53D[7] __attribute__((scn_string_literal)) = "開始４";
static const char data_1A544[7] __attribute__((scn_string_literal)) = "成功４";
static const char data_1A54B[7] __attribute__((scn_string_literal)) = "失敗４";
static const char data_1A552[7] __attribute__((scn_string_literal)) = "開始５";
static const char data_1A559[7] __attribute__((scn_string_literal)) = "成功５";
static const char data_1A560[7] __attribute__((scn_string_literal)) = "失敗５";
static const char data_1A567[7] __attribute__((scn_string_literal)) = "開始６";
static const char data_1A56E[7] __attribute__((scn_string_literal)) = "成功６";
static const char data_1A575[7] __attribute__((scn_string_literal)) = "失敗６";
static const char data_1A57C[9] __attribute__((scn_string_literal)) = "オート１";
static const char data_1A585[9] __attribute__((scn_string_literal)) = "オート２";
static const char data_1A58E[9] __attribute__((scn_string_literal)) = "オート３";
static const char data_1A597[7] __attribute__((scn_string_literal)) = "save!\012";
static const char data_1A59E[7] __attribute__((scn_string_literal)) = "save!\012";
static const char data_1A5A5[7] __attribute__((scn_string_literal)) = "save!\012";
static const char data_1A5AC[7] __attribute__((scn_string_literal)) = "save!\012";
static const char data_1A5B3[7] __attribute__((scn_string_literal)) = "save!\012";
static const char data_1A5BA[10] __attribute__((scn_string_literal)) = "save!\012";
static const Vector3 data_1A5C4 = {.x = -168.0f, .y = 0.0f, .z = -53.8f};
static const Vector3i data_1A5D0 = {.x = 0, .y = 16384, .z = 0};
static const Vector3 data_1A5DC = {.x = 0.0f, .y = 0.0f, .z = 0.0f};
static const Vector3i data_1A5E8 = {.x = 0, .y = 0, .z = 0};
static const Vector3 data_1A5F4 = {.x = 0.0f, .y = 0.0f, .z = 0.0f};
static const Vector3i data_1A600 = {.x = 0, .y = 0, .z = 0};
typedef struct Row_1A60C { float w0; float w1; float w2; int32_t w3; float w4; } Row_1A60C;
static const Row_1A60C data_1A60C[2] = {
  {0.0f, 1.0f, 0.0f, 0, 0.0f},
  {2.0f, -2.0f, -168.0f, 0, -53.8f}
};
static const int32_t data_1A634[3] = {0, 16384, 0};
static const int32_t data_1A640[57] = {
  0, 0, 0, 0, 0, 0, 0, -1638, 1456, 0, 0, 0, 0, 182, -5097, 0, 0, -3458, 0, 0, 0, 0, 0, 0, 0, 0, -4733, 0, 0, -3640, 0, 0, 0, 0, 0, -910, 0, 0, -6917,
  0, 0, -1820, 0, 0, 0, 0, 0, -1820, 0, 0, -6735, 0, -1456, -2002, 0, 0, 0
};
static const int32_t data_1A724[57] = {
  0, 0, -7281, 0, 0, -4004, 0, 546, 182, 0, 0, -18204, 0, 0, -16930, 0, 0, -12379, 0, 0, 0, 0, 0, -12196, 0, 0, -18204, 0, 0, -13289, 0, 0, 0, 0, 0,
  -11832, 0, 0, -18386, 0, 0, -13835, 0, 0, 0, 0, 910, -12014, 0, -2184, -16748, 0, -2002, -13835, 0, 0, 364
};
static const int32_t data_1A808[57] = {
  0, 0, -4004, 0, 0, -546, 0, 364, -1638, 0, 0, -7827, 0, 0, -11468, 0, 0, -3822, 0, 0, 0, 0, 0, -8192, 0, 0, -18022, 0, 0, -3276, 0, 0, 0, 0, 0,
  -7827, 0, 0, -18022, 0, 0, -5279, 0, 0, 0, 0, 910, -12014, 0, -2184, -16748, 0, -2002, -8738, 0, 0, 364
};
static const int32_t data_1A8EC[57] = {
  0, 0, -4004, 0, 0, 1820, 0, 728, -728, 0, 0, -5097, 0, 0, -3640, 0, 0, -2548, 0, 0, 0, 0, 0, -6371, 0, 0, -8920, 0, 0, -3276, 0, 0, 0, 0, 0, -3822,
  0, 0, -9648, 0, 0, -1274, 0, 0, 0, 0, 910, -8374, 0, -2184, -8192, 0, -2002, -2184, 0, 0, 364
};
static const Vector3i data_1A9D0 = {.x = -1274, .y = 910, .z = -1820};
static const char data_1A9DC[40] = "save!\012\000\000\232\231aA\000\000HD\217\302\365>\377\321\237\177\000\000\000\000\000\000\000\000<f\000\273<f\000\273";
static const uint32_t data_1AA04 = 0x41B3851F;
static const float data_1AA08[293] = {
  12.94f, 24.98f, 8.06f, 29.89f, 10.59f, 27.49f, 15.34f, 0.0f, 3.0f, 26.0f, 1.0f, 11.56f, 2.0f, 10.45f, 31.25f, 15.31f, 31.26f, 15.33f, 36.64f,
  10.43f, 36.64f, 0.0f, 3.0f, 12.33f, 1.0f, 33.2f, 1.5f, 0.1f, 31.77f, 4.17f, 31.76f, 4.19f, 36.45f, 0.1f, 36.45f, 0.0f, 3.0f, 2.11f, 1.0f, 32.82f,
  1.5f, -10.41f, 23.87f, -7.27f, 23.84f, -7.26f, 27.02f, -10.41f, 27.01f, 0.0f, 4.4f, -8.89f, 1.0f, 25.68f, 1.0f, -37.28f, 34.88f, -32.29f, 34.91f,
  -32.3f, 39.72f, -37.28f, 39.72f, 0.0f, 3.0f, -34.93f, 1.0f, 36.73f, 2.5f, -64.45f, 70.33f, -54.16f, 70.41f, -57.28f, 73.43f, -64.49f, 73.58f, 0.0f,
  2.9f, -61.02f, 1.0f, 71.83f, 4.0f, -75.79f, 62.9f, -69.2f, 62.89f, -69.19f, 69.54f, -75.82f, 69.35f, 0.0f, 3.2f, -73.0f, 1.0f, 66.62f, 4.0f,
  -95.31f, 58.48f, -90.31f, 57.11f, -89.08f, 61.8f, -94.06f, 63.18f, 0.0f, 2.6f, -92.26f, 1.0f, 60.25f, 2.0f, -69.57f, 91.23f, -65.26f, 91.25f,
  -65.24f, 96.3f, -69.55f, 96.3f, 0.0f, 2.8f, -67.2f, 1.0f, 92.23f, 1.0f, 37.9f, 7.42f, 41.27f, 10.4f, 40.55f, 12.12f, 36.49f, 10.05f, -0.1f, 2.9f,
  38.0f, 1.0f, 9.76f, 2.0f, -50.56f, 47.3f, -47.61f, 51.37f, -50.33f, 53.17f, -53.09f, 49.25f, 0.0f, 4.4f, -50.88f, 1.0f, 49.69f, 2.0f, 2.32f, 26.39f,
  2.37f, 23.35f, 7.4f, 23.26f, 7.47f, 26.41f, 0.0f, 2.2f, 5.43f, 1.0f, 24.0f, 1.6f, -15.65f, 30.81f, -19.42f, 26.1f, -16.9f, 24.07f, -13.13f, 28.75f,
  -0.1f, 3.66f, -15.63f, 1.0f, 28.25f, 1.5f, -53.27f, 42.68f, -55.56f, 39.25f, -52.04f, 34.13f, -49.67f, 40.12f, -0.1f, 2.79f, -52.59f, 1.0f, 39.68f,
  1.5f, -20.89f, 34.8f, -26.5f, 28.85f, -21.16f, 25.45f, -17.11f, 31.45f, -0.1f, 3.17f, -21.0f, 0.0f, 30.0f, 3.0f, -17.19f, 36.6f, -14.79f, 39.42f,
  -17.25f, 41.42f, -19.54f, 38.59f, -0.1f, 3.17f, -17.16f, 1.0f, 39.31f, 2.0f, -68.54f, 56.35f, -71.82f, 55.19f, -69.77f, 48.96f, -66.44f, 49.97f,
  -0.1f, 3.17f, -69.4f, 0.0f, 52.71f, 4.0f, -63.62f, 63.68f, -58.74f, 63.6f, -56.14f, 68.94f, -63.49f, 68.65f, -0.1f, 3.1f, -62.65f, 0.0f, 66.33f,
  2.0f, -76.0f, 80.23f, -76.2f, 71.72f, -70.0f, 71.7f, -70.1f, 80.0f, -0.1f, 3.1f, -71.9f, 0.0f, 78.59f, 2.8f, 25.51f, 32.46f, 25.84f, 35.16f, 19.29f,
  35.3f, 19.24f, 32.31f, -0.1f, 3.2f, 22.34f, 0.0f, 33.7f, 2.4f, -223.0f, -52.0f, 166.0f, -52.0f, 166.0f, 173.0f, -223.0f, 150.0f, -1000.0f, 1000.0f,
  -26.0f, 0.0f, 53.0f, 100.0f
};
static const uint32_t data_1AE9C = SHEN_NAME('VM_0');
static const shen_name data_1AEA0[7] = {
  SHEN_NAME('VM_1'), SHEN_NAME('VM_2'), SHEN_NAME('VM_3'), SHEN_NAME('VM_4'), SHEN_NAME('VM_5'), SHEN_NAME('VM_6'), SHEN_NAME('VM_7')
};
static const char data_1AEBC[16] __attribute__((scn_string_literal)) = "/scene/80/TIKN/";
static const char data_1AECC[] __attribute__((scn_string_literal)) = "VEND";
static const char data_1AED1[] __attribute__((scn_string_literal)) = "SPRT";
static const char data_1AED6[7] __attribute__((scn_string_literal)) = "01JUCE";
static const char data_1AEDD[] __attribute__((scn_string_literal)) = "VEND";
static const char data_1AEE2[] __attribute__((scn_string_literal)) = "VEND";
static const uint8_t data_1AEE7[1] = {0};
static const uint8_t data_1AEE8[12] = {0};
static const char data_1AEF4[] __attribute__((scn_string_literal)) = "COKE";
static const char data_1AEF9[] __attribute__((scn_string_literal)) = "FATG";
static const char data_1AEFE[] __attribute__((scn_string_literal)) = "FATO";
static const char data_1AF03[] __attribute__((scn_string_literal)) = "SPRT";
static const uint32_t data_1AF08 = 0x3D204C06;
static const float data_1AF0C[2] = {0.0f, 0.890018f};
static const uint8_t data_1AF14[12] = {0};
static const uint8_t data_1AF20[12] = {0};
static const uint32_t data_1AF2C = 0x3DE978D5;
static const float data_1AF30[2] = {0.002f, -0.036f};
static const uint32_t data_1AF38 = 8192;
static const int32_t data_1AF3C[2] = {11377, 3640};
static const uint32_t data_1AF44 = 0x3D204C06;
static const float data_1AF48[2] = {0.0f, 0.890018f};
static const uint8_t data_1AF50[12] = {0};
static const uint32_t data_1AF5C = 4369;
static const int32_t data_1AF60[2] = {0, -2184};
static const char data_1AF68[] __attribute__((scn_string_literal)) = "01JUCEA002";
static const char data_1AF73[] __attribute__((scn_string_literal)) = "01JUCEA003";
static const uint8_t pad_1AF7E[4] = {0};
static const uint8_t data_1AF82[10] = {
  0x80, 0x3F, 0x00, 0x00, 0x80, 0x3F, 0x00, 0x00, 0x80, 0x3F
};
static const char data_1AF8C[10] __attribute__((scn_string_literal)) = "T1260B001";
static const char data_1AF96[10] __attribute__((scn_string_literal)) = "T1260B002";
static const char data_1AFA0[10] __attribute__((scn_string_literal)) = "T1260B003";
static const char data_1AFAA[10] __attribute__((scn_string_literal)) = "T1260B004";
static const char data_1AFB4[10] __attribute__((scn_string_literal)) = "T1246B001";
static const char data_1AFBE[10] __attribute__((scn_string_literal)) = "T1246B002";
static const char data_1AFC8[10] __attribute__((scn_string_literal)) = "T1246B003";
static const char data_1AFD2[10] __attribute__((scn_string_literal)) = "T1246B004";
static const char data_1AFDC[10] __attribute__((scn_string_literal)) = "T1216B001";
static const char data_1AFE6[10] __attribute__((scn_string_literal)) = "T1216B002";
static const char data_1AFF0[10] __attribute__((scn_string_literal)) = "T1216B003";
static const char data_1AFFA[10] __attribute__((scn_string_literal)) = "T1216B004";
static const char data_1B004[10] __attribute__((scn_string_literal)) = "T1073B001";
static const char data_1B00E[10] __attribute__((scn_string_literal)) = "T1073B002";
static const char data_1B018[10] __attribute__((scn_string_literal)) = "T1073B003";
static const char data_1B022[10] __attribute__((scn_string_literal)) = "T1073B004";
static const char data_1B02C[10] __attribute__((scn_string_literal)) = "T1115B001";
static const char data_1B036[10] __attribute__((scn_string_literal)) = "T1115B002";
static const char data_1B040[10] __attribute__((scn_string_literal)) = "T1115B003";
static const char data_1B04A[10] __attribute__((scn_string_literal)) = "T1115B004";
static const char data_1B054[15] __attribute__((scn_string_literal)) = "stopper start\012";
static const char data_1B063[18] __attribute__((scn_string_literal)) = "chara load start\012";
static const char data_1B075[14] __attribute__((scn_string_literal)) = "scene/80/TIKN";
static const char data_1B083[] __attribute__((scn_string_literal)) = "SCRTEST.BIN";
static const char data_1B08F[13] __attribute__((scn_string_literal)) = "seqdata2.bin";
static const char data_1B09C[14] __attribute__((scn_string_literal)) = "scene/80/TIKN";
static const char data_1B0AA[11] __attribute__((scn_string_literal)) = "M_DJUC.BIN";
static const char data_1B0B5[13] __attribute__((scn_string_literal)) = "HMOT_CAN.BIN";
static const char data_1B0C2[9] __attribute__((scn_string_literal)) = "ALLVOICE";
static const char data_1B0CB[13] __attribute__((scn_string_literal)) = "SEQDATA1.BIN";
static const char data_1B0D8[14] __attribute__((scn_string_literal)) = "SCENE/80/TIKN";
static const char data_1B0E6[13] __attribute__((scn_string_literal)) = "SEQDATA1.BIN";
static const char data_1B0F3[17] __attribute__((scn_string_literal)) = "SCENE/80/TIKN";
static const Vector4 data_1B104 = {.x = 1.0f, .y = 0.3f, .z = 0.3f, .w = 0.3f};
static const char data_1B114[7] __attribute__((scn_string_literal)) = "save!\012";
static const char data_1B11B[7] __attribute__((scn_string_literal)) = "save!\012";
static const char data_1B122[7] __attribute__((scn_string_literal)) = "save!\012";
static const char data_1B129[11] __attribute__((scn_string_literal)) = "TS1001A003";
static const char data_1B134[11] __attribute__((scn_string_literal)) = "TS1001A002";
static const char data_1B13F[7] __attribute__((scn_string_literal)) = "save!\012";
static const char data_1B146[14] __attribute__((scn_string_literal)) = "TS1001A001";
static const uint32_t data_1B154 = 0xC2480000;
static const float data_1B158[2] = {0.0f, 10.0f};
static const uint32_t data_1B160 = 0x41A00000;
static const float data_1B164[2] = {0.0f, 110.0f};
static const uint32_t data_1B16C = 0xC2041FF3;
static const float data_1B170[2] = {0.0f, 52.1008f};
static const uint32_t data_1B178 = 0x41B3AA99;
static const uint32_t data_1B17C[2] = {0, 0x420947FDU};
static const uint32_t data_1B184 = 0x42201A1D;
static const float data_1B188[2] = {0.0f, -2.1877f};
static const uint32_t data_1B190 = 0xC2BD9F2E;
static const uint32_t data_1B194[2] = {0, 0x428A24A9U};
static const uint32_t data_1B19C = 0x41D6BFB1;
static const uint32_t data_1B1A0[2] = {0, 0x42CFC5AFU};
static const uint32_t data_1B1A8 = 0xC1A00000;
static const float data_1B1AC[2] = {0.0f, 110.0f};
static const uint32_t data_1B1B4 = 0xC19181A3;
static const uint32_t data_1B1B8[2] = {0, 0x421B20DFU};
static const uint32_t data_1B1C0 = 0xC2C00000;
static const float data_1B1C4[2] = {0.0f, 78.0f};
static const uint32_t data_1B1CC = 0xC1A00000;
static const float data_1B1D0[2] = {0.0f, 110.0f};
static const uint32_t data_1B1D8 = 0x3F4978D5;
static const float data_1B1DC[32] = {
  0.787f, 0.866f, 0.944f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.944f, 0.866f, 0.787f,
  0.708f, 0.629f, 0.551f, 0.511f, 0.472f, 0.433f, 0.393f, 0.393f, 0.393f, 0.393f
};
static const uint8_t data_1B25C[84] = {0};
typedef struct Row_1B2B0 { float w0; float w1; float w2; int32_t w3; } Row_1B2B0;
static const Row_1B2B0 data_1B2B0[3] = {
  {1.0f, 1.0f, 1.0f, 0},
  {0.0f, 0.0f, 0.0f, 0},
  {0.0f, 0.0f, 0.0f, 0}
};
static const uint8_t data_1B2E0[24] = {0};
static const float data_1B2F8[27] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f
};
static const uint8_t data_1B364[24] = {0};
static const float data_1B37C[27] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f
};
static const uint32_t data_1B3E8 = 0x3F4978D5;
static const float data_1B3EC[32] = {
  0.787f, 0.866f, 0.944f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  0.944f, 0.866f, 0.787f, 0.787f, 0.787f, 0.787f, 0.708f, 0.708f, 0.708f
};
static const uint8_t data_1B46C[24] = {0};
static const float data_1B484[27] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  0.0f, 0.0f, 0.0f
};
static const uint32_t data_1B4F0 = 0x3F800000;
static const float data_1B4F4[32] = {
  0.944f, 0.866f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f,
  0.787f, 0.787f, 0.551f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint8_t data_1B574[104] = {0};
static const float data_1B5DC[7] = {1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f};
static const uint8_t data_1B5F8[24] = {0};
static const float data_1B610[27] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f
};
static const uint8_t data_1B67C[88] = {0};
static const float data_1B6D4[11] = {0.708f, 0.787f, 0.866f, 0.944f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f};
static const uint32_t data_1B700 = 0x3F800000;
static const float data_1B704[32] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const char data_1B784[] __attribute__((scn_string_literal)) = "/scene/80/TIKN/";
static const char data_1B794[] __attribute__((scn_string_literal)) = "FACEPAK";
static const char data_1B79C[] __attribute__((scn_string_literal)) = "/scene/80/TIKN/";
static const char data_1B7AC[] __attribute__((scn_string_literal)) = "SHUKI";
static const char data_1B7B2[] __attribute__((scn_string_literal)) = "/scene/80/TIKN/";
static const char data_1B7C2[] __attribute__((scn_string_literal)) = "STARTMOT";
static const char data_1B7CB[] __attribute__((scn_string_literal)) = "/scene/80/TIKN/";
static const char data_1B7DB[] __attribute__((scn_string_literal)) = "AUTHOR1";
static const char data_1B7E3[] __attribute__((scn_string_literal)) = "/scene/80/TIKN/";
static const char data_1B7F3[] __attribute__((scn_string_literal)) = "CTER";
