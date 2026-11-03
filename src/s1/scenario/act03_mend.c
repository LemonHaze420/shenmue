#include "act03_mend.h"

static void script_0030(void);
static void script_0168(int32_t arg0);
static void script_0174(int32_t arg0);
static void script_0180(int32_t arg0);
static void script_027C(int32_t arg0);
static void script_0293(int32_t arg0);
static void script_02D4(int32_t arg0);
static void script_0318(int32_t arg0);
static void script_0396(void);
static void script_0415(int32_t arg0, float arg1, float arg2, float arg3);
static void script_043D(int32_t arg0, float arg1, float arg2, float arg3);
static void script_0491(int32_t arg0, int32_t arg1, int32_t arg2);
static int32_t script_04E1(int32_t arg0);
static void script_0588(int32_t arg0, int32_t arg1);
static int32_t script_05AB(int32_t arg0);
static void script_05DE(int32_t arg0, int32_t arg1);
static void script_0601(int32_t arg0, int32_t arg1);
static void script_0660(int32_t arg0);
static void script_0675(int32_t arg0, int32_t arg1);
static void script_0713(void);
static void script_071E(int32_t arg0, int32_t arg1);
static void script_0798(void);
static void script_0952(void);
static void script_0A7C(void);
static void script_0AAD(void);
static void script_0ADE(void);
static void script_0BE4(void);
static void script_0BEC(void);
static void script_0D61(void);
static void script_0D95(void);
static void script_0DFA(void);
static void script_0E7E(void);
static void script_0F20(int32_t arg0);
static void script_0FB8(int32_t arg0, int32_t arg1);
static void script_0FDE(const char *arg0, const char *arg1);
static void script_1052(int32_t arg0);
static void script_1083(void);
static void script_10CB(void);
static void script_1152(void);
static void script_1190(void);
static void script_120E(void);
static void script_121A(void);
static void script_121D(void);
static void script_1226(void);
static void script_1235(int32_t arg0);
static void script_12C2(int32_t arg0);
static int32_t script_1312(void);
static void script_1340(int32_t arg0);
static int32_t script_136E(void);
static void script_13A2(int32_t arg0);
static void script_13D6(int32_t arg0);
static void script_13F4(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_142E(int32_t arg0, int32_t arg1);
static void script_1462(void);
static void script_155A(int32_t arg0, int32_t arg1);
static int32_t script_158A(void);
static void script_15C5(int32_t arg0);
static void script_165B(int32_t arg0);
static void script_16E8(int32_t arg0);
static void script_1ADF(int32_t arg0);
static void script_26B2(int32_t arg0);
static void script_293B(void);
static void script_2B03(void);
static void script_2B6B(void);
static void script_2CF8(void);
static void script_2D60(void);
static void script_2DF0(void);
static int32_t script_2E57(void);
static int32_t script_2E9C(void);
static int32_t script_31DE(void);
static int32_t script_32DF(void);
static int32_t script_3367(void);
static void script_33FE(int32_t arg0, int32_t arg1);
static int32_t script_3462(void);
static void script_3527(void);
static void script_3600(int32_t arg0);
static void script_360C(int32_t arg0);
static void script_3679(void);
static void script_37B7(void);
static void script_37CF(int32_t arg0);
static void script_3A85(void);
static void script_3DE2(void);
static void script_3EE4(void);
static void script_3F2F(int32_t arg0);
static void script_408F(int32_t arg0);
static void script_427C(void);
static void script_433C(void);
static void script_4387(int32_t arg0);
static void script_4413(void);
static void script_44F4(int32_t arg0);
static void script_4518(int32_t arg0);
static void script_45DB(void);
static void script_45F6(int32_t arg0);
static void script_4611(int32_t arg0);
static int32_t script_468A(void);
static void script_4B5C(void);
static void script_4BEE(void);
static void script_4C7E(void);
static void script_4D19(void);
static void script_4DB4(void);
static void script_4E4F(void);
static void script_4EEA(void);
static void script_4F82(void);
static void script_501A(void);
static void script_5135(void);
static void script_522A(void);
static void script_5315(void);
static void script_53F7(void);
static void script_5498(void);
static void script_54F4(void);
static void script_5591(void);
static void script_55C6(int32_t arg0, int32_t arg1);
static void script_60F1(int32_t arg0, int32_t arg1);
static int32_t script_60F4(int32_t arg0);
static void script_61A3(int32_t arg0);
static void script_6288(int32_t arg0);
static void script_62B9(int32_t arg0, int32_t arg1);
static void script_6628(int32_t arg0);
static int32_t script_6736(void);
static void script_6972(void);
static int32_t script_69AB(void);
static void script_69F1(int32_t arg0);
static void script_6FCA(int32_t arg0, int32_t arg1);
static void script_7028(int32_t arg0, int32_t arg1);
static void script_7090(int32_t arg0, int32_t arg1);
static void script_70EE(int32_t arg0, int32_t arg1);
static void script_714C(int32_t arg0, int32_t arg1);
static void script_71AA(int32_t arg0, int32_t arg1);
static void script_7314(int32_t arg0, int32_t arg1);
static void script_742B(int32_t arg0, int32_t arg1);
static void script_758E(int32_t arg0, int32_t arg1);
static void script_760F(int32_t arg0, int32_t arg1);
static void script_766D(int32_t arg0, int32_t arg1);
static void script_76CB(int32_t arg0, int32_t arg1);
static void script_7899(int32_t arg0, int32_t arg1);
static void script_79A2(int32_t arg0, int32_t arg1);
static void script_7DC4(int32_t arg0, int32_t arg1);
static void script_8152(int32_t arg0, int32_t arg1);
static void script_844C(void);
static void script_856E(void);
static void script_85F8(int32_t arg0);
static void script_8692(int32_t arg0, int32_t arg1);
static void script_86F6(int32_t arg0);
static void script_88F8(void);
static void script_8AEE(void);
static void script_8B8B(int32_t arg0);
static void script_8BC6(int32_t arg0);
static void script_8BF0(int32_t arg0);
static void script_8C19(void);
static void script_8CB4(int32_t arg0);
static void script_8D47(void);
static void script_8E0F(void);
static void script_8EC7(int32_t arg0);
static void script_90E4(void);
static void script_922C(void);
static int32_t script_92DB(int32_t arg0);
static int32_t script_9568(int32_t arg0);
static void script_95BE(void);
static void script_95DB(void);
static void script_967F(int32_t arg0, int32_t arg1);
void script_96E3(void);
static void script_9900(int32_t arg0);
static void script_99F0(void);
static void script_9C08(void);
static void script_9CB7(void);
static void script_9E6D(void);
static void script_9FD2(void);
static void script_A1C6(void);
static void script_A389(void);
static int32_t script_A8BF(void);
static void script_AAAE(void);
static void script_ABBF(void);
static void script_AC62(void);
static void script_AC95(void);
static int32_t script_ACC8(int32_t arg0);
static void script_ACE6(float arg0);
static void script_AD05(int32_t arg0);
static void script_AD1A(int32_t arg0, int32_t arg1);
static int32_t script_ADD0(int32_t arg0);
static void script_ADE8(int32_t arg0);
static void script_ADFE(int32_t arg0);
static void script_AE0C(void);
static void script_AED6(void);
static int32_t script_AED9(void);
static void script_B027(int32_t arg0);
static int32_t script_B03B(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_B05D(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_B0A3(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_B0B7(int32_t arg0);
static void script_B0D1(void);
static void script_B0EC(int32_t arg0);
static void script_B119(int32_t arg0, int32_t arg1);
static void script_B140(void);
static void script_B14A(void);
static void script_B165(void);
static void script_B16F(int32_t arg0);
static int32_t script_B1D3(float arg0, float arg1, float arg2, float arg3);
static int32_t script_B1FA(int32_t arg0, int32_t arg1);
static int32_t script_B272(int32_t arg0, int32_t arg1, int32_t arg2, const Vector3 *arg3);
static void script_B31E(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_B4E4(int32_t arg0);
static void script_B506(int32_t arg0, int32_t arg1, int32_t arg2);
static int32_t script_B5E1(void);
static void script_B605(int32_t arg0);
static int32_t script_B6C5(int32_t arg0, int32_t arg1);
static int32_t script_B6CB(int32_t arg0);
static int32_t script_B6D6(int32_t arg0);
static int32_t script_B7B1(int32_t arg0, int32_t arg1);
static int32_t script_B816(int32_t arg0, int32_t arg1);
static void script_B87B(void);
static void script_B8B7(void);
static void script_B8D2(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_B946(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_B96B(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_B9B4(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_BA1A(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_BAE2(int32_t arg0, int32_t arg1, int32_t arg2);
static int32_t script_BB38(void);
static void script_BB8D(int32_t arg0);
static void script_BBE7(const char *arg0, const char *arg1);
static void script_BC37(void);
static void script_BC50(int32_t arg0);
static void script_BFE2(void);
static void script_C30A(int32_t arg0);
static void script_C375(int32_t arg0, int32_t arg1);
static int32_t script_C558(void);
static void script_CA42(int32_t arg0, int32_t arg1);
static void script_CB01(void);
static void script_CBD8(void);
static void script_CC5D(int32_t arg0);
static void script_CC8F(int32_t arg0);
static void script_CCC1(int32_t arg0);
static void script_CCF3(int32_t arg0);
static void script_CD25(int32_t arg0, int32_t arg1);
static void script_CD57(int32_t arg0);
static void script_CD89(void);
static void script_CDF2(int32_t arg0, int32_t arg1);
static void script_CE6C(void);
static int32_t script_CE74(void);
static void script_CEDB(void);
static void script_CEE3(void);
static void script_D029(int32_t arg0, int32_t arg1);
static void script_D065(void);
static void script_D0F3(int32_t arg0);
static void script_D12A(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_D158(const Vector3 *arg0);
static void script_D173(int32_t arg0);
static void script_D1B9(int32_t arg0);
static void script_D1FF(void);
static void script_D566(void);
static void script_D645(void);
static void script_D907(void);
static void script_DCE2(void);
static void script_DD67(void);
static void script_DE4F(void);
static int32_t script_DE96(void);
static void script_E00D(void);
static int32_t script_E096(void);
static void script_E14C(void);
static void script_E14F(void);
static void script_E152(void);
static void script_E155(void);
static void script_E158(void);
static void script_E15B(void);
static void script_E15E(void);
static void script_E161(void);
static int32_t script_E164(void);
static int32_t script_E167(void);
static int32_t script_E16A(void);
static int32_t script_E16D(void);
static void script_E170(void);
static void script_E173(void);
static void script_E176(void);
static void script_E179(void);
static void script_E17C(void);
static void script_E17F(void);
static void script_E182(void);
static void script_E185(void);
static void script_E188(void);
static void script_E18B(void);
static void script_E18E(void);
static void script_E191(void);
static void script_E194(void);
static void script_E197(void);
static void script_E19A(void);
static int32_t script_E19D(void);
static int32_t script_E1A0(void);
static int32_t script_E1A3(void);
static int32_t script_E1A6(void);
static int32_t script_E1A9(void);
static int32_t script_E1AC(void);
static int32_t script_E1AF(void);
static int32_t script_E1B2(void);
static int32_t script_E1B5(void);
static void script_E1B8(void);
static void script_E1BB(void);
static void script_E1BE(void);
static void script_E1C1(void);
static void script_E1C4(void);
static void script_E1C7(void);
static void script_E1CA(void);
static void script_E1CD(void);
static void script_E1D0(void);
static void script_E1D3(void);
static int32_t script_E1D6(void);
static int32_t script_E1D9(void);
static int32_t script_E1DC(void);
static int32_t script_E1DF(void);
static int32_t script_E1E2(void);
static int32_t script_E1E5(void);
static int32_t script_E1E8(void);
static int32_t script_E1EB(void);
static int32_t script_E1EE(void);
static int32_t script_E1F1(void);
static void script_E1F4(void);
static void script_E1F7(void);
static void script_E1FA(void);
static void script_E1FD(void);
static int32_t script_E200(void);
static int32_t script_E203(void);
static int32_t script_E206(void);
static int32_t script_E209(void);
static int32_t script_E20C(void);
static int32_t script_E20F(void);
static int32_t script_E212(void);
static int32_t script_E215(void);
static int32_t script_E218(void);
static int32_t script_E21B(void);
static int32_t script_E21E(void);
static int32_t script_E221(void);
static int32_t script_E224(void);
static int32_t script_E227(void);
static int32_t script_E22A(void);
static int32_t script_E22D(void);
static int32_t script_E230(void);
static int32_t script_E233(void);
static int32_t script_E236(void);
static int32_t script_E239(void);
static int32_t script_E23C(void);
static int32_t script_E23F(void);
static int32_t script_E242(void);
static int32_t script_E245(void);
static int32_t script_E248(void);
static int32_t script_E24B(void);
static int32_t script_E24E(void);
static int32_t script_E251(void);
static int32_t script_E254(void);
static void script_E257(void);
static void script_E25A(void);
static void script_E25D(void);
static void script_E260(void);
static int32_t script_E263(void);
static int32_t script_E266(void);
static void script_E269(void);
static void script_E26C(void);
static void script_E26F(void);
static void script_E272(void);
static void script_E275(void);
static void script_E278(void);
static void script_E27B(void);
static void script_E27E(void);
static void script_E281(void);
static void script_E284(void);
static void script_E287(void);
static void script_E28A(void);
static void script_E28D(void);
static void script_E290(void);
static void script_E293(void);
static void script_E296(void);
static void script_E299(void);
static void script_E29C(void);
static void script_E29F(void);
static void script_E2A2(void);
static void script_E2A5(void);
static void script_E2A8(void);
static void script_E2AB(void);
static void script_E2AE(void);
static void script_E2B1(void);
static int32_t script_E2B4(void);
static int32_t script_E2B7(void);
static int32_t script_E2BA(void);
static int32_t script_E2BD(void);
static int32_t script_E2C0(void);
static int32_t script_E2C3(void);
static int32_t script_E2C6(void);
static int32_t script_E2C9(void);
static int32_t script_E2CC(void);
static void script_E2CF(void);
static void script_E2D2(void);
static void script_E2D5(void);
static int32_t script_E2D8(void);
static void script_E2DB(void);
static void script_E2DE(void);
static int32_t script_E2E1(void);
static int32_t script_E2E4(void);
static int32_t script_E2E7(void);
static int32_t script_E2EA(void);
static int32_t script_E2ED(void);
static int32_t script_E2F0(void);
static int32_t script_E2F3(void);
static int32_t script_E2F6(void);
static int32_t script_E2F9(void);
static int32_t script_E2FC(void);
static int32_t script_E2FF(void);
static int32_t script_E302(void);
static int32_t script_E305(void);
static int32_t script_E308(void);
static int32_t script_E30B(void);
static int32_t script_E30E(void);
static void script_E311(void);
static void script_E314(void);
static void script_E317(void);
static void script_E31A(void);
static void script_E31D(void);
static void script_E320(void);
static int32_t script_E323(void);
static int32_t script_E326(void);
static int32_t script_E329(void);
static int32_t script_E32C(void);
static void script_E32F(void);
static void script_E332(void);
static void script_E335(void);
static int32_t script_E338(void);
static int32_t script_E33B(void);
static int32_t script_E33E(void);
static int32_t script_E341(void);
static void script_E344(void);
static void script_E350(void);
static void script_E35C(void);
static void script_E368(void);
static void script_E374(void);
static void script_E380(void);
static void script_E38C(void);
static void script_E398(void);
static int32_t script_E3A4(void);
static int32_t script_E3B0(void);
static int32_t script_E3BC(void);
static int32_t script_E3C8(void);
static void script_E3D4(void);
static void script_E3E0(void);
static void script_E3EC(void);
static void script_E3F8(void);
static void script_E404(void);
static void script_E410(void);
static void script_E41C(void);
static void script_E428(void);
static void script_E434(void);
static void script_E440(void);
static void script_E44C(void);
static void script_E458(void);
static void script_E464(void);
static void script_E470(void);
static void script_E47C(void);
static int32_t script_E488(void);
static int32_t script_E494(void);
static int32_t script_E4A0(void);
static int32_t script_E4AC(void);
static int32_t script_E4B8(void);
static int32_t script_E4C4(void);
static int32_t script_E4D0(void);
static int32_t script_E4DC(void);
static int32_t script_E4E8(void);
static void script_E4F4(void);
static void script_E500(void);
static void script_E50C(void);
static void script_E518(void);
static void script_E524(void);
static void script_E530(void);
static void script_E53C(void);
static void script_E548(void);
static void script_E554(void);
static void script_E560(void);
static int32_t script_E56C(void);
static int32_t script_E578(void);
static int32_t script_E584(void);
static int32_t script_E590(void);
static int32_t script_E59C(void);
static int32_t script_E5A8(void);
static int32_t script_E5B4(void);
static int32_t script_E5C0(void);
static int32_t script_E5CC(void);
static int32_t script_E5D8(void);
static void script_E5E4(void);
static void script_E5F0(void);
static void script_E5FC(void);
static void script_E608(void);
static int32_t script_E614(void);
static int32_t script_E620(void);
static int32_t script_E62C(void);
static int32_t script_E638(void);
static int32_t script_E644(void);
static int32_t script_E650(void);
static int32_t script_E65C(void);
static int32_t script_E668(void);
static int32_t script_E674(void);
static int32_t script_E680(void);
static int32_t script_E68C(void);
static int32_t script_E698(void);
static int32_t script_E6A4(void);
static int32_t script_E6B0(void);
static int32_t script_E6BC(void);
static int32_t script_E6C8(void);
static int32_t script_E6D4(void);
static int32_t script_E6E0(void);
static int32_t script_E6EC(void);
static int32_t script_E6F8(void);
static int32_t script_E704(void);
static int32_t script_E710(void);
static int32_t script_E71C(void);
static int32_t script_E728(void);
static int32_t script_E734(void);
static int32_t script_E740(void);
static int32_t script_E74C(void);
static int32_t script_E758(void);
static int32_t script_E764(void);
static void script_E770(void);
static void script_E77C(void);
static void script_E788(void);
static void script_E794(void);
static int32_t script_E7A0(void);
static int32_t script_E7AC(void);
static void script_E7B8(void);
static void script_E7C4(void);
static void script_E7D0(void);
static void script_E7DC(void);
static void script_E7E8(void);
static void script_E7F4(void);
static void script_E800(void);
static void script_E80C(void);
static void script_E818(void);
static void script_E824(void);
static void script_E830(void);
static void script_E83C(void);
static void script_E848(void);
static void script_E854(void);
static void script_E860(void);
static void script_E86C(void);
static void script_E878(void);
static void script_E884(void);
static void script_E890(void);
static void script_E89C(void);
static void script_E8A8(void);
static void script_E8B4(void);
static void script_E8C0(void);
static void script_E8CC(void);
static void script_E8D8(void);
static int32_t script_E8E4(void);
static int32_t script_E8F0(void);
static int32_t script_E8FC(void);
static int32_t script_E908(void);
static int32_t script_E914(void);
static int32_t script_E920(void);
static int32_t script_E92C(void);
static int32_t script_E938(void);
static int32_t script_E944(void);
static void script_E950(void);
static void script_E95C(void);
static void script_E968(void);
static int32_t script_E974(void);
static void script_E980(void);
static void script_E98C(void);
static int32_t script_E998(void);
static int32_t script_E9A4(void);
static int32_t script_E9B0(void);
static int32_t script_E9BC(void);
static int32_t script_E9C8(void);
static int32_t script_E9D4(void);
static int32_t script_E9E0(void);
static int32_t script_E9EC(void);
static int32_t script_E9F8(void);
static int32_t script_EA04(void);
static int32_t script_EA10(void);
static int32_t script_EA1C(void);
static int32_t script_EA28(void);
static int32_t script_EA34(void);
static int32_t script_EA40(void);
static int32_t script_EA4C(void);
static void script_EA58(void);
static void script_EA64(void);
static void script_EA70(void);
static void script_EA7C(void);
static void script_EA88(void);
static void script_EA94(void);
static int32_t script_EAA0(void);
static int32_t script_EAAC(void);
static int32_t script_EAB8(void);
static int32_t script_EAC4(void);
static void script_EAD0(void);
static void script_EADC(void);
static void script_EAE8(void);
static int32_t script_EAF4(void);
static int32_t script_EB00(void);
static int32_t script_EB0C(void);
static int32_t script_EB18(void);
static int32_t script_EB24(int32_t arg0);
static int32_t script_EB48(int32_t arg0, int32_t arg1);
static int32_t script_EE34(int32_t arg0, int32_t arg1);
static int32_t script_EF18(int32_t arg0, int32_t arg1);
static int32_t script_F008(int32_t arg0, int32_t arg1);
static int32_t script_F025(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_F260(int32_t arg0, int32_t arg1);
static void script_F2F2(int32_t arg0, int32_t arg1);
static void script_F3A3(int32_t arg0, int32_t arg1);
static void script_F454(int32_t arg0, int32_t arg1);
static int32_t script_F460(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_F5FC(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_F631(int32_t arg0, int32_t arg1, int32_t arg2);
static int32_t script_F860(int32_t arg0, int32_t arg1, int32_t arg2);
static int32_t script_FDBF(int32_t arg0, int32_t arg1, int32_t arg2);
static int32_t script_108A3(int32_t arg0, int32_t arg1, int32_t arg2);
static int32_t script_108B3(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4);
static void script_10905(void);
static int32_t script_10E46(int32_t arg0);
static void script_10ED7(int32_t arg0);
static void script_10FD9(int32_t arg0);
static void script_11118(int32_t arg0);
static int32_t script_11245(int32_t arg0);
static void script_11385(int32_t arg0);
static void script_113C3(void);
static int32_t script_113DB(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5, int32_t arg6, int32_t arg7);
static void script_11C2A(int32_t arg0);
static void script_11C5C(int32_t arg0);
static void script_11DA4(int32_t arg0);
static void script_11DE9(int32_t arg0);
static void script_11EAF(void);
static int32_t script_11EFF(void);
static int32_t script_120ED(void);
static int32_t script_1220A(void);
static int32_t script_122B3(void);
static float script_12329(void);
static int32_t script_123B4(void);
static int32_t script_12456(void);
static int32_t script_12493(void);
static int32_t script_124D9(void);
static int32_t script_12516(void);
static int32_t script_1255C(void);
static void script_125CC(int32_t arg0);
static int32_t script_12803(int32_t arg0, int32_t arg1);
static int32_t script_12827(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4);
static int32_t script_128FE(int32_t arg0, int32_t arg1, int32_t arg2, float arg3, int32_t arg4, int32_t arg5);
static int32_t script_12995(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4);
static int32_t script_129F7(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_12A53(int32_t arg0, float arg1, int32_t arg2);
static void script_12AB0(int32_t arg0);
static int32_t script_12ACC(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, void *arg4, int32_t arg5);
static void script_12CF3(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_12D2A(int32_t arg0);
static void script_12D3A(void);
static void script_12D5A(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_12D94(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_12DE0(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4);
static void script_12E29(int32_t arg0);
static int32_t script_12E3C(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_12E9F(int32_t arg0, int32_t arg1);
static int32_t script_12F60(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_1304E(void);
static void script_1313C(int32_t arg0, int32_t arg1);
static void script_13165(int32_t arg0, int32_t arg1, float arg2, float arg3, float arg4);
static void script_131F4(int32_t arg0, int32_t arg1, const Vector3 *arg2);
static void script_13227(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static int32_t script_13241(int32_t arg0, int32_t arg1, float arg2, float arg3, float arg4, int32_t arg5);
static void script_1326C(int32_t arg0, int32_t arg1, float arg2, float arg3, float arg4, float arg5);
static void script_132EC(int32_t arg0, int32_t arg1, float arg2);
static void script_13340(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_13356(int32_t arg0, int32_t arg1, float arg2, int32_t arg3);
static void script_1337D(int32_t arg0, float arg1);
static void script_133A6(int32_t arg0, float arg1, float arg2, float arg3);
static void script_133FA(int32_t arg0, int32_t arg1, int32_t arg2);
static int32_t script_1340E(int32_t arg0, float arg1, int32_t arg2);
static void script_13433(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4);
static int32_t script_1344B(int32_t arg0, float arg1, float arg2, float arg3, int32_t arg4);
static void script_13474(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4);
static int32_t script_1348C(int32_t arg0, float arg1, float arg2, float arg3, int32_t arg4);
static void script_134E1(int32_t arg0, int32_t arg1);
static void script_135C6(int32_t arg0, int32_t arg1);
static void script_136AF(int32_t arg0);
static void script_136CD(void);
static void script_13715(void);
static void script_13730(void);
static void script_1374A(const char *arg0, const char *arg1);
static void script_13788(const char *arg0);
static void script_137A9(int32_t arg0);
static void script_137C0(int32_t arg0);
static void script_13846(int32_t arg0, int32_t arg1);
static void script_13869(int32_t arg0);
static int32_t script_13B21(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_142D0(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_142E6(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_1430A(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_14320(float arg0, float arg1, float arg2, int32_t arg3);
static void script_14345(int32_t arg0, int32_t arg1);
static int32_t script_14357(const char *arg0, int32_t arg1);
static void script_1437E(int32_t arg0, int32_t arg1);
static void script_1438F(int32_t arg0, int32_t arg1);
static void script_144D0(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4);
static int32_t script_144E8(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4);
static void script_1450F(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_14525(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_14561(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5, int32_t arg6);
static int32_t script_1457D(int32_t arg0, int32_t arg1, int32_t arg2, float arg3, int32_t arg4, int32_t arg5, int32_t arg6);
static void script_145D3(void);
static int32_t script_145D6(int32_t arg0, int32_t arg1);
static int32_t script_147F7(int32_t arg0, int32_t arg1);
static int32_t script_14D71(int32_t arg0, int32_t arg1);
static void script_14DD1(int32_t arg0);
static void script_14DEC(int32_t arg0);
static void script_14E25(int32_t arg0);
static void script_14ECD(int32_t arg0);
static int32_t script_14F4F(int32_t arg0, int32_t arg1, int32_t arg2);
static int32_t script_150D5(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4);

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
static GameDateTime task_6C;
static int32_t task_74;
static int32_t task_78;
static int32_t task_7C;
static int8_t task_80;
static uint8_t task_81;
static uint16_t task_82;
static int8_t task_84;
static uint8_t task_85;
static uint16_t task_86;
static int32_t task_88;
static int32_t task_8C;
static int32_t task_90;
static int32_t task_94;
static int32_t task_98;
static int32_t task_9C;
static Vector3 task_A0;
static int8_t task_AC;
static uint8_t task_AD;
static uint16_t task_AE;
static int32_t task_B0;
static int32_t task_B4;
static int32_t task_B8;
static int32_t task_BC;
static int32_t task_C0;
static int32_t task_C4;
static int32_t task_C8;
static int32_t task_CC;
static int32_t task_D0;
static int32_t task_D4;
static int32_t task_D8;
static int32_t task_DC;
static int32_t task_E0;
static int32_t task_E4;
static int32_t task_E8;
static int32_t task_EC;
static int32_t task_F0;
static int32_t task_F4;
static int32_t task_F8;
static int32_t task_FC;
static int32_t task_100;
static int32_t task_104;
static int32_t task_108;
static int32_t task_10C;
static int32_t task_110;
static int32_t task_114;
static int32_t task_118;
static int32_t task_11C;
static int8_t task_120;
static int8_t task_121;
static int8_t task_122;
static int8_t task_123;
static int8_t task_124;
static uint8_t task_125;
static uint16_t task_126;
static int32_t task_128;
static int32_t task_12C;
static int8_t task_130[100][12];
static int8_t task_5E0[12];
static int8_t task_5EC[10];
static int8_t task_5F6;
static int8_t task_5F7;
static int8_t task_5F8;
static uint8_t task_5F9;
static uint16_t task_5FA;
static int32_t task_5FC;
static int32_t task_600;
static int8_t task_604;
static uint8_t task_605;
static uint16_t task_606;
static int32_t task_608[10];
static int32_t task_630;
static int32_t task_634;
static int32_t task_638;
static int8_t task_63C;
static uint8_t task_63D;
static uint16_t task_63E;
static int32_t task_640;
static int32_t task_644;
static int32_t task_648;
static int32_t task_64C;
static int32_t task_650;
static int32_t task_654;
static int32_t task_658;
static int32_t task_65C;
static int32_t task_660;
static int32_t task_664;
static int32_t task_668;
static int32_t task_66C;
static int32_t task_670;
static int32_t task_674;
static int32_t task_678;
static int32_t task_67C;
static int8_t task_680;
static uint8_t task_681;
static uint16_t task_682;
static int8_t task_684;
static uint8_t task_685;
static uint16_t task_686;
static int32_t task_688;
static int32_t task_68C;
static int32_t task_690;
static int32_t task_694;
static int32_t task_698;
static int32_t task_69C;
static int32_t task_6A0;
static int32_t task_6A4;
static int32_t task_6A8;
static int32_t task_6AC;
static int32_t task_6B0;
static int32_t task_6B4;
static int32_t task_6B8;
static int32_t task_6BC;
static int32_t task_6C0;
static int32_t task_6C4;
static int32_t task_6C8;
static int32_t task_6CC[2];
static int32_t task_6D4[2];
static int32_t task_6DC[2];
static int32_t task_6E4[2];
static int32_t task_6EC[2];
static int32_t task_6F4;
static int32_t task_6F8[2];
static int32_t task_700[2];
static int32_t task_708[2];
static int32_t task_710[2];
static int32_t task_718;
static int32_t task_71C;
static int32_t task_720;
static int32_t task_724;
static int32_t task_728;
static int32_t task_72C;
static int32_t task_730[2];
static float task_738[2][3];
static float task_750[2];
static float task_758;
static float task_75C;
static float task_760;
static float task_764[2][3];
static int32_t task_77C[2][3];
static float task_794[2];
static int32_t task_79C[2];
static int32_t task_7A4;
static int32_t task_7A8;
static int32_t task_7AC;
static int32_t task_7B0;
static const float data_1525C[3] = {1.0f, 1.0f, 1.0f};
static const int32_t data_15268[4] = {16777215, 16777215, 16777215, 16777215};
static const float data_15278[3] = {20.0f, 800.0f, 1.0f};
static const int32_t data_15284 = -1;
static const uint32_t data_15288 = 380;
static const int32_t data_1528C[6] = {1, 390, 0, 1018, 0, -1};
static const uint32_t data_152A4 = 510;
static const int32_t data_152A8[6] = {1, 525, 0, 1020, 0, -1};
static const uint32_t data_152C0 = 150;
static const int32_t data_152C4[6] = {1, 160, 0, 1019, 0, -1};
static const uint32_t data_152DC = 530;
static const int32_t data_152E0[6] = {1, 970, 0, 1014, 0, -1};
static const uint32_t data_152F8 = 911;
static const int32_t data_152FC[6] = {1, 155, 0, 1015, 0, -1};
static const char data_15314[7] __attribute__((scn_string_literal)) = "FREE02";
static const char data_1531B[7] __attribute__((scn_string_literal)) = "FREE03";
static const char data_15322[7] __attribute__((scn_string_literal)) = "FREE04";
static const char data_15329[7] __attribute__((scn_string_literal)) = "FREE01";
static const uint32_t data_15330 = 0x41A00000;
static const float data_15334[2] = {0.0f, -50.0f};
static const uint32_t data_1533C = 0x41A00000;
static const float data_15340[2] = {0.0f, -50.0f};
static const uint32_t data_15348 = 0xC2200000;
static const float data_1534C[2] = {0.0f, 80.0f};
static const uint32_t data_15354 = 0xC2D60000;
static const float data_15358[2] = {0.0f, 78.0f};
static const uint8_t data_15360[8] = {0};
static const float data_15368 = 90.0f;
static const uint32_t data_1536C = 0xC1A00000;
static const float data_15370[2] = {0.0f, -50.0f};
static const uint32_t data_15378 = 0xC1A00000;
static const float data_1537C[2] = {0.0f, -50.0f};
static const uint32_t data_15384 = 0xC2E60000;
static const float data_15388[2] = {0.0f, 175.0f};
static const uint32_t data_15390 = 0xC2DC0000;
static const float data_15394[2] = {0.0f, 10.0f};
static const uint32_t data_1539C = 0x42C80000;
static const float data_153A0[2] = {0.0f, -40.0f};
static const uint32_t data_153A8 = 0x43020000;
static const float data_153AC[2] = {0.0f, 25.0f};
static const uint8_t data_153B4[12] = {0};
static const uint32_t data_153C0 = 0x3F800000;
static const float data_153C4[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint32_t data_15448 = 0x3F800000;
static const float data_1544C[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint8_t data_154D0[16] = {0};
static const float data_154E0[30] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint32_t data_15558 = 0x3F800000;
static const float data_1555C[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint32_t data_155E0 = 0x3F800000;
static const float data_155E4[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint8_t data_15668[84] = {0};
typedef struct Row_156BC { float w0; float w1; float w2; int32_t w3; } Row_156BC;
static const Row_156BC data_156BC[3] = {
  {0.787f, 0.787f, 0.787f, 0},
  {0.0f, 0.0f, 0.0f, 0},
  {0.0f, 0.0f, 0.0f, 0}
};
static const int32_t data_156EC = 0;
static const uint32_t data_156F0 = 0x3F800000;
static const float data_156F4[33] = {
  0.944f, 0.866f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f,
  0.787f, 0.787f, 0.787f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint32_t data_15778 = 0x3F4978D5;
static const float data_1577C[33] = {
  0.787f, 0.866f, 0.944f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  0.944f, 0.866f, 0.787f, 0.787f, 0.787f, 0.787f, 0.708f, 0.708f, 0.708f, 0.708f
};
static const uint8_t data_15800[12] = {0};
static const float data_1580C[31] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint32_t data_15888 = 0x3F800000;
static const float data_1588C[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint32_t data_15910 = 0x3F800000;
static const float data_15914[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint32_t data_15998 = 0x3F800000;
static const float data_1599C[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint32_t data_15A20 = 0x41A00000;
static const float data_15A24[2] = {0.0f, -50.0f};
static const uint32_t data_15A2C = 0x41A00000;
static const float data_15A30[2] = {0.0f, -50.0f};
static const uint32_t data_15A38 = 0xC2200000;
static const float data_15A3C[2] = {0.0f, 80.0f};
static const uint32_t data_15A44 = 0xC1A00000;
static const float data_15A48[2] = {0.0f, -50.0f};
static const uint32_t data_15A50 = 0xC1A00000;
static const float data_15A54[2] = {0.0f, -50.0f};
static const uint32_t data_15A5C = 0xC2E60000;
static const float data_15A60[2] = {0.0f, 175.0f};
static const uint32_t data_15A68 = 0x42C80000;
static const float data_15A6C[2] = {0.0f, -40.0f};
static const uint8_t data_15A74[12] = {0};
static const uint32_t data_15A80 = 0x3F800000;
static const float data_15A84[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint32_t data_15B08 = 0x3F800000;
static const float data_15B0C[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint8_t data_15B90[16] = {0};
static const float data_15BA0[30] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint8_t data_15C18[84] = {0};
typedef struct Row_15C6C { float w0; float w1; float w2; int32_t w3; } Row_15C6C;
static const Row_15C6C data_15C6C[3] = {
  {0.787f, 0.787f, 0.787f, 0},
  {0.0f, 0.0f, 0.0f, 0},
  {0.0f, 0.0f, 0.0f, 0}
};
static const int32_t data_15C9C = 0;
static const uint32_t data_15CA0 = 0x3F800000;
static const float data_15CA4[33] = {
  0.944f, 0.866f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f,
  0.787f, 0.787f, 0.787f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint32_t data_15D28 = 0x3F4978D5;
static const float data_15D2C[33] = {
  0.787f, 0.866f, 0.944f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  0.944f, 0.866f, 0.787f, 0.787f, 0.787f, 0.787f, 0.708f, 0.708f, 0.708f, 0.708f
};
static const uint32_t data_15DB0 = 0x3F800000;
static const float data_15DB4[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint32_t data_15E38 = 0x3F800000;
static const float data_15E3C[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint32_t data_15EC0 = 0xC2A50000;
static const float data_15EC4[2] = {0.0f, 68.0f};
static const uint32_t data_15ECC = 0x3F800000;
static const float data_15ED0[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint32_t data_15F54 = 0xC1D00000;
static const float data_15F58[2] = {0.0f, 76.5f};
static const uint32_t data_15F60 = 0x3F800000;
static const float data_15F64[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint32_t data_15FE8 = 0x41A00000;
static const float data_15FEC[2] = {0.0f, -50.0f};
static const uint32_t data_15FF4 = 0x41A00000;
static const float data_15FF8[2] = {0.0f, -50.0f};
static const uint32_t data_16000 = 0xC2200000;
static const float data_16004[2] = {0.0f, 80.0f};
static const uint32_t data_1600C = 0xC1A00000;
static const float data_16010[2] = {0.0f, -50.0f};
static const uint32_t data_16018 = 0xC1A00000;
static const float data_1601C[2] = {0.0f, -50.0f};
static const uint32_t data_16024 = 0xC2E60000;
static const float data_16028[5] = {0.0f, 175.0f, 100.0f, 0.0f, -40.0f};
static const uint8_t data_1603C[12] = {0};
static const uint32_t data_16048 = 0x3F800000;
static const float data_1604C[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint32_t data_160D0 = 0x3F800000;
static const float data_160D4[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint8_t data_16158[16] = {0};
static const float data_16168[30] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint8_t data_161E0[84] = {0};
typedef struct Row_16234 { float w0; float w1; float w2; int32_t w3; } Row_16234;
static const Row_16234 data_16234[3] = {
  {0.787f, 0.787f, 0.787f, 0},
  {0.0f, 0.0f, 0.0f, 0},
  {0.0f, 0.0f, 0.0f, 0}
};
static const int32_t data_16264 = 0;
static const uint32_t data_16268 = 0x3F800000;
static const float data_1626C[33] = {
  0.944f, 0.866f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f, 0.787f,
  0.787f, 0.787f, 0.787f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint32_t data_162F0 = 0x3F4978D5;
static const float data_162F4[33] = {
  0.787f, 0.866f, 0.944f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  0.944f, 0.866f, 0.787f, 0.787f, 0.787f, 0.787f, 0.708f, 0.708f, 0.708f, 0.708f
};
static const uint32_t data_16378 = 0x3F800000;
static const float data_1637C[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint32_t data_16400 = 0xC2680000;
static const float data_16404[2] = {0.0f, 85.0f};
static const uint32_t data_1640C = 0xC2180000;
static const float data_16410[2] = {0.0f, 107.0f};
static const uint32_t data_16418 = 0xC23C0000;
static const float data_1641C[2] = {0.0f, 22.0f};
static const uint32_t data_16424 = 0xC1500000;
static const float data_16428[2] = {0.0f, 26.0f};
static const uint32_t data_16430 = 0xC2040000;
static const float data_16434[2] = {0.0f, 34.0f};
static const uint32_t data_1643C = 0xC0A00000;
static const float data_16440[2] = {0.0f, 43.0f};
static const uint32_t data_16448 = 0xC2200000;
static const float data_1644C[2] = {0.0f, 20.0f};
static const uint8_t data_16454[8] = {0};
static const float data_1645C = 100.0f;
static const uint8_t data_16460[8] = {0};
static const float data_16468 = 100.0f;
static const uint8_t data_1646C[8] = {0};
static const float data_16474 = 50.0f;
static const uint32_t data_16478 = 0x3F800000;
static const float data_1647C[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.8f, 0.7f, 0.5f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint32_t data_16500 = 0x3F800000;
static const float data_16504[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.8f, 0.7f, 0.5f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint32_t data_16588 = 0x3F800000;
static const float data_1658C[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.8f, 0.7f, 0.5f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint32_t data_16610 = 0x3F800000;
static const float data_16614[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.8f, 0.7f, 0.5f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint32_t data_16698 = 0x3F800000;
static const float data_1669C[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.8f, 0.7f, 0.5f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint32_t data_16720 = 0x3F800000;
static const float data_16724[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.8f, 0.7f, 0.5f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint8_t data_167A8[80] = {0};
typedef struct Row_167F8 { float w0; float w1; float w2; int32_t w3; } Row_167F8;
static const Row_167F8 data_167F8[3] = {
  {1.0f, 1.0f, 1.0f, 0},
  {0.0f, 0.0f, 0.0f, 0},
  {0.0f, 0.0f, 0.0f, 0}
};
static const int32_t data_16828[2] = {0, 0};
static const uint8_t data_16830[96] = {0};
static const float data_16890[10] = {0.8f, 0.8f, 0.8f, 0.8f, 0.8f, 0.8f, 0.8f, 0.8f, 0.8f, 0.8f};
static const uint32_t data_168B8 = 0x3F800000;
static const float data_168BC[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};
static const uint32_t data_16940 = 0x3F800000;
static const float data_16944[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const char data_169C8[30] __attribute__((scn_string_literal)) = "MEND: Character read failed.\012";
static const char data_169E6[18] __attribute__((scn_string_literal)) = "MEND: Now loading";
static const char data_169F8[2] __attribute__((scn_string_literal)) = ".";
static const char data_169FA[2] __attribute__((scn_string_literal)) = "\012";
static const char data_169FC[25] __attribute__((scn_string_literal)) = "MEND: Pack read failed.\012";
static const char data_16A15[18] __attribute__((scn_string_literal)) = "MEND: Now loading";
static const char data_16A27[2] __attribute__((scn_string_literal)) = ".";
static const char data_16A29[2] __attribute__((scn_string_literal)) = "\012";
static const char data_16A2B[233] __attribute__((scn_string_literal)) = "FREE";
static const int32_t data_16B14[57] = {
  0, 0, -3640, 0, 0, -2366, -4369, 1820, -546, 0, 0, -5279, 0, 0, -6007, 0, 0, -2912, 0, 0, -910, 0, 0, -6553, 0, 0, -6735, 0, 0, -3640, 0, 0, -182,
  0, 0, -5461, 0, 0, -6007, 0, 0, -2366, 0, 0, 0, 0, 0, -5643, 0, 0, -4733, 0, 0, -2366, -1456, 0, 728
};
static const int32_t data_16BF8[57] = {
  0, 0, -9466, 0, -546, -2912, 0, 3094, -2366, 0, 0, -18386, 0, 0, -19296, 0, 1092, -14381, 0, 0, 0, 0, 0, -16748, 0, 0, -17658, 0, 0, -15109, 0, 0,
  0, 0, 0, -16566, 0, 0, -18204, 0, 0, -14381, 0, 0, 0, 0, 0, -19478, 0, 0, -18386, 0, -4004, -12925, 0, 0, -546
};
static const int32_t data_16CDC[57] = {
  0, 0, -3276, 0, 0, 0, 0, 0, 0, 0, 0, -6553, 0, 0, -4369, 0, -546, -3458, 0, 0, 0, 0, 0, -5461, 0, 0, -5643, 0, 0, -3458, 0, 0, 0, 0, 0, -6007, 0, 0,
  -4551, 0, 546, -2548, 0, 0, 0, 0, 0, -6189, 0, -1092, -3094, 0, 0, -2548, 0, 0, 0
};
static const int32_t data_16DC0[399] = {
  0, 0, -6007, 0, 0, -728, 0, 0, 0, 0, 0, -5097, 0, 0, -2184, 0, 0, -2002, 0, 0, 0, 0, 0, -5825, 0, 0, -2912, 0, 0, -728, 0, 0, 0, 0, 0, -4915, 0, 0,
  -4187, 0, 0, -1274, 0, 0, 0, 0, 0, -6553, 0, -364, -4369, 0, -546, -1456, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -10922, 0, 0, -10558, 0, 1456, 0, 0,
  0, -18022, 0, 0, -17112, 0, 0, -18386, 0, 0, 0, 0, 0, -12196, 0, 0, -18204, 0, 0, -16748, 0, 0, 0, 0, 0, -11832, 0, 0, -18386, 0, 0, -15109, 0, 0,
  0, 0, 910, -12014, 0, -2184, -16748, 0, -2184, -10012, 0, 0, 0, 0, -364, -11650, -2912, -364, -4369, 1638, 910, 182, 0, 0, -16019, 0, 182, -23119,
  0, 546, -18932, 0, 0, -1092, 0, 0, -16384, 0, 0, -21845, 0, 0, -18750, 0, 0, -546, 0, 0, -21481, 0, 0, -21299, 0, 0, -18568, 0, 0, -728, 0, 910,
  -18750, 0, 182, -18750, 0, -4004, -18204, 0, 0, -728, 0, -364, -14017, -2912, -364, -6735, 1638, 910, 2548, 0, 0, -18568, 0, 182, -25668, 0, 546,
  -21481, 0, 0, -3640, 0, 0, -18932, 0, 0, -24393, 0, 0, -21299, 0, 0, -3094, 0, 0, -24029, 0, 0, -23847, 0, 0, -21117, 0, 0, -3276, 0, 910, -21299,
  0, 182, -21299, 0, -4004, -20753, 0, 0, -3276, 0, 0, -5461, 0, -2366, -1092, 0, -182, 1092, 0, -546, -6735, 0, 0, -4004, 0, 0, -2912, 0, -182, 1274,
  0, 0, -13289, 0, 0, -5279, 0, 0, -8009, 0, 0, 1456, 0, 182, -8738, 0, 0, -6007, 0, 0, -8738, 0, 182, 1638, 0, -728, -5279, 0, -1274, -6735, 0, -910,
  -8738, 0, 0, 1638, 0, 0, -5461, 0, -2366, -1092, 0, -182, 1092, 0, -546, -13289, 0, 0, -5643, 0, 0, -8009, 0, -182, 1274, 0, 0, -11832, 0, 0, -5279,
  0, 0, -8009, 0, 0, 1456, 0, 182, -8738, 0, 0, -6007, 0, 0, -8738, 0, 182, 1638, 0, -728, -5279, 0, -1274, -6735, 0, -910, -8738, 0, 0, 1092
};
static const int32_t data_173FC[57] = {
  0, 0, -5461, 0, -2366, -1092, 0, -182, 1092, 0, -546, -3640, 0, 0, -2366, 0, 0, -1456, 0, -182, 1274, 0, 0, -3822, 0, 0, -2730, 0, 0, -1456, 0, 0,
  1456, 0, 182, -3822, 0, 0, -2912, 0, 0, -1638, 0, 182, 1638, 0, -728, -3640, 0, -910, -2730, 0, -546, -1638, 0, 0, 1092
};
static const int32_t data_174E0[342] = {
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
static const Vector3 data_17A38 = {.x = -17.5f, .y = -1.8f, .z = -15.0f};
static const char data_17A44[13] __attribute__((scn_string_literal)) = "model/object";
static const char data_17A51[9] __attribute__((scn_string_literal)) = "SEA3.PVR";
static const char data_17A5A[13] __attribute__((scn_string_literal)) = "model/object";
static const char data_17A67[11] __attribute__((scn_string_literal)) = "TRSEA4.MT6";
static const char data_17A72[13] __attribute__((scn_string_literal)) = "model/object";
static const char data_17A7F[13] __attribute__((scn_string_literal)) = "TRWTBL1.BIN";
static const Vector3 data_17A8C = {.x = -150.5f, .y = 0.0f, .z = 177.5f};
static const Vector3 data_17A98 = {.x = 71.0f, .y = 0.0f, .z = 101.6f};
static const uint32_t data_17AA4 = 0x428FCCCD;
static const float data_17AA8[2] = {-1.9f, 15.9f};
static const int32_t data_17AB0[114] = {
  0, 0, -7281, 0, 0, -4004, 0, 546, 182, 0, 0, -18204, 0, 0, -16930, 0, 0, -12379, 0, 0, 0, 0, 0, -12196, 0, 0, -18204, 0, 0, -13289, 0, 0, 0, 0, 0,
  -11832, 0, 0, -18386, 0, 0, -13835, 0, 0, 0, 0, 910, -12014, 0, -2184, -16748, 0, -2002, -13835, 0, 0, 364, 0, 0, -10922, 0, 0, -10558, 0, 1456, 0,
  0, 0, -18022, 0, 0, -17112, 0, 0, -18386, 0, 0, 0, 0, 0, -12196, 0, 0, -18204, 0, 0, -16748, 0, 0, 0, 0, 0, -11832, 0, 0, -18386, 0, 0, -15109, 0,
  0, 0, 0, 819, -12014, 0, -1274, -16748, 0, -3458, -16201, 0, 0, 0
};
static const int32_t data_17C78[57] = {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -3094, 0, 0, -4369, 0, 0, -3822, 0, 0, 0, 0, 0, -1092, 0, 0, -4187, 0, 0, -3276, 0, 0, 0, 0, 0, -1820, 0, 0, -3822,
  0, 0, -2183, 0, 0, 0, 0, -1637, -4004, 0, 0, -4369, 0, 0, -2730, 0, 0, 0
};
static const int32_t data_17D5C[114] = {
  0, 0, 0, 0, 6916, -3275, 0, 0, 0, 0, 0, -4002, 0, 0, -8006, 0, 0, -4548, 0, 0, 0, 0, 0, -2183, 0, 0, -7824, 0, 0, -4730, 0, 0, 0, 0, 0, -4184, 0, 0,
  -4912, 0, 0, -4730, 0, 0, 0, 0, -1636, -5277, 0, -1637, -4730, 0, -363, -1637, 0, 0, 0, 0, 728, -1092, -4915, 2366, 3822, -6735, 2730, -1638, 0, 0,
  -5279, 0, 0, -6007, 0, 0, -2912, 0, 0, -910, 0, 0, -6553, 0, 0, -6735, 0, 0, -3640, 0, 0, -182, 0, 0, -5461, 0, 0, -6007, 0, 0, -2366, 0, 0, 0, 0,
  0, -5643, 0, 0, -4733, 0, 0, -2366, -1456, 0, 728
};
static const int32_t data_17F24[57] = {
  0, 0, -1820, 0, -182, -3094, 0, 182, -182, 0, -546, -8738, 0, 910, -10740, 0, 0, -4369, 0, -182, 1274, 0, 0, -15473, 0, 0, -19114, 0, 0, -11468, 0,
  0, 1092, 0, 182, -15109, 0, 0, -19114, 0, 0, -14927, 0, 182, 1638, 0, -182, -16566, 0, -1274, -20206, 0, -1638, -15291, 0, 0, 1274
};
static const int32_t data_18008[57] = {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -3076, 0, 0, -7809, 0, 0, -5261, 0, 0, 0, 0, 0, -3440, 0, 0, -8355, 0, 0, -6535, 0, 0, 0, 0, 0, -3276, 0, 0, -9466,
  0, 0, -6735, 0, 0, 0, 0, -1638, -6735, 0, -2002, -7099, 0, -1456, -5097, 0, 0, 0
};
static const Vector3i data_180EC = {.x = 0, .y = 0, .z = 910};
static const Vector3i data_180F8 = {.x = 0, .y = 2547, .z = -5641};
static const Vector3i data_18104 = {.x = -7281, .y = 2730, .z = -2184};
static const int32_t data_18110[3] = {544, -4367, -2184};
static const Vector3i data_1811C = {.x = -6549, .y = -907, .z = 1637};
static const Vector3i data_18128 = {.x = 363, .y = 2730, .z = -2183};
static const int32_t data_18134[3] = {-5824, -5095, -10738};
static const Vector3i data_18140 = {.x = -2912, .y = -2911, .z = 544};
static const int32_t data_1814C[6] = {-5824, -5095, -10738, -5824, -5095, -10738};
static const Vector3i data_18164 = {.x = 2002, .y = -2911, .z = 0};
static const Vector3i data_18170 = {.x = -6548, .y = -4000, .z = 1637};
static const int32_t data_1817C[3] = {0, 0, 2547};
static const Vector3i data_18188 = {.x = -3094, .y = 4915, .z = 2547};
static const Vector3i data_18194 = {.x = 182, .y = -546, .z = 2366};
static const int32_t data_181A0[10] = {1638, 62441, 0, 3640, 4915, 0, 0, 5097, 0, 0};
static const int32_t data_181C8[10] = {182, 62623, 63715, 3640, 4915, 0, 0, 5097, 0, 0};
static const int32_t data_181F0[10] = {57344, 546, 0, 0, 61895, 0, 0, 59164, 0, 0};
static const int32_t data_18218[57] = {
  0, -546, -3458, 0, 1638, -2548, 0, 5825, -546, 0, 728, -10194, 0, 0, -11832, 0, 0, -15837, 0, 364, -3458, 0, 0, -8920, 0, 0, -15109, 0, -182,
  -14745, 0, 0, -2912, 0, 0, -13835, 0, 0, -11286, 0, 0, -14927, 0, 0, -2002, 0, 0, -12379, 0, 0, -8556, 0, -1820, -12925, -2002, -546, -1456
};
static const int32_t data_182FC[57] = {
  0, 0, -3640, 0, 0, -2366, -4369, 1820, -546, 0, 0, -5279, 0, 0, -6007, 0, 0, -2912, 0, 0, -910, 0, 0, -6553, 0, 0, -6735, 0, 0, -3640, 0, 0, -182,
  0, 0, -5461, 0, 0, -6007, 0, 0, -2366, 0, 0, 0, 0, 0, -5643, 0, 0, -4733, 0, 0, -2366, -1456, 0, 728
};
static const Vector3i data_183E0 = {.x = 2366, .y = 1637, .z = -2182};
static const Vector3i data_183EC = {.x = -5097, .y = 546, .z = 0};
static const int32_t data_183F8[2] = {-16777216, -16777216};
static const float data_18400[5] = {444086.7f, 444086.7f, 5.1f, 84.1f, 0.33f};
static const int32_t data_18414 = -1441793;
static const Vector3 data_18418 = {.x = 68.601196f, .y = 0.0344f, .z = 28.364799f};
static const Vector3 data_18424 = {.x = 0.3f, .y = 0.01f, .z = 0.3f};
static const Vector3 data_18430 = {.x = 67.561501f, .y = 0.0f, .z = 29.7297f};
static const Vector3 data_1843C = {.x = 0.3f, .y = 0.01f, .z = 0.3f};
static const Vector3 data_18448 = {.x = 0.0f, .y = 0.0f, .z = 0.0f};
static const Vector3 data_18454 = {.x = 0.000392156857f, .y = 0.0f, .z = 0.00588235306f};
static const Vector3 data_18460 = {.x = 0.034f, .y = 0.355f, .z = -0.168f};
static const Vector3i data_1846C = {.x = 12833, .y = 50432, .z = 38218};
static const Vector3 data_18478 = {.x = 71.9114f, .y = -1.9f, .z = 16.773199f};
static const char data_18484[13] __attribute__((scn_string_literal)) = "model/object";
static const char data_18491[9] __attribute__((scn_string_literal)) = "SEA3.PVR";
static const char data_1849A[13] __attribute__((scn_string_literal)) = "model/object";
static const char data_184A7[11] __attribute__((scn_string_literal)) = "TRSEA4.MT6";
static const char data_184B2[13] __attribute__((scn_string_literal)) = "model/object";
static const char data_184BF[13] __attribute__((scn_string_literal)) = "TRWTBL1.BIN";
static const Vector3 data_184CC = {.x = 71.9114f, .y = -1.9f, .z = 16.773199f};
static const char data_184D8[13] __attribute__((scn_string_literal)) = "model/object";
static const char data_184E5[9] __attribute__((scn_string_literal)) = "SEA2.PVR";
static const char data_184EE[13] __attribute__((scn_string_literal)) = "model/object";
static const char data_184FB[11] __attribute__((scn_string_literal)) = "TRSEA4.MT6";
static const char data_18506[13] __attribute__((scn_string_literal)) = "model/object";
static const char data_18513[13] __attribute__((scn_string_literal)) = "TRWTBL1.BIN";
static const Vector3 data_18520 = {.x = -150.5f, .y = 0.0f, .z = 177.5f};
static const Vector3 data_1852C = {.x = 71.0f, .y = 0.0f, .z = 101.6f};
static const char data_18538[13] __attribute__((scn_string_literal)) = "model/object";
static const char data_18545[13] __attribute__((scn_string_literal)) = "SIRANAMI.PVR";
static const char data_18552[13] __attribute__((scn_string_literal)) = "model/object";
static const char data_1855F[13] __attribute__((scn_string_literal)) = "TRSEA4.MT6";
static const int32_t data_1856C[57] = {
  0, 0, -7281, 0, 0, -4004, 0, 546, 182, 0, 0, -18204, 0, 0, -16930, 0, 0, -12379, 0, 0, 0, 0, 0, -12196, 0, 0, -18204, 0, 0, -13289, 0, 0, 0, 0, 0,
  -11832, 0, 0, -18386, 0, 0, -13835, 0, 0, 0, 0, 910, -12014, 0, -2184, -16748, 0, -2002, -13835, 0, 0, 364
};
static const int32_t data_18650[57] = {
  0, 0, -1274, 0, 3276, 546, 0, -1274, 1274, 0, -546, -6735, 0, 0, -6553, 0, 2002, -546, 0, -182, 1274, 0, 0, -5643, 0, 0, -9102, 0, 0, -1092, 0, 0,
  1456, 0, 182, -6553, 0, 0, -8374, 0, -1820, -2730, 0, 0, 1638, 0, -728, -5279, 0, -1274, -3640, 0, -2548, -4004, 0, 0, 1638
};
static const int32_t data_18734[57] = {
  0, 0, -1274, 0, 3276, 546, 0, -1274, 1274, 0, -546, -6735, 0, 0, -6553, 0, 2002, -364, 0, -182, 1274, 0, 0, -5643, 0, 0, -9102, 0, 0, -1274, 0, 0,
  1456, 0, 182, -6553, 0, 0, -7827, 0, -1820, -2548, 0, 0, 1638, 0, -728, -5279, 0, -1274, -3640, 0, -2548, -3822, 0, 0, 1638
};
static const int32_t data_18818[57] = {
  0, 0, -7281, 0, 0, -4004, 0, 546, 182, 0, 0, -18204, 0, 0, -16930, 0, 0, -12379, 0, 0, 0, 0, 0, -12196, 0, 0, -18204, 0, 0, -13289, 0, 0, 0, 0, 0,
  -11832, 0, 0, -18386, 0, 0, -13835, 0, 0, 0, 0, 910, -12014, 0, -2184, -16748, 0, -2002, -13835, 0, 0, 364
};
static const int32_t data_188FC[57] = {
  0, 0, -1274, 0, 3276, 546, 0, -1274, 1274, 0, -546, -6735, 0, 0, -6553, 0, 2002, -546, 0, -182, 1274, 0, 0, -5643, 0, 0, -9102, 0, 0, -1092, 0, 0,
  1456, 0, 182, -6553, 0, 0, -8374, 0, -1820, -2730, 0, 0, 1638, 0, -728, -5279, 0, -1274, -3640, 0, -2548, -4004, 0, 0, 1638
};
static const int32_t data_189E0[57] = {
  0, 0, -5461, 0, -2366, -1638, 0, -910, 1092, 0, -546, -3640, 0, 0, -2366, 0, 0, -1456, 0, -182, 1638, 0, 0, -910, 0, 0, -2730, 0, 0, -1274, 0, 0,
  1820, 0, 182, -2730, 0, 0, -1820, 0, 0, -1274, 0, 182, 2184, 0, -728, -3094, 0, -910, -2548, 0, 910, -1456, -546, 910, 2366
};
static const int32_t data_18AC4[57] = {
  0, 0, 0, 0, 0, 0, 0, -2548, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0
};
static const int32_t data_18BA8[57] = {
  0, 0, 0, 0, -2002, -1274, 0, -1820, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};
static const int32_t data_18C8C[57] = {
  0, 0, -5461, 0, -3094, -4551, 0, -182, 1092, 0, 728, -11650, 0, 0, -11104, 0, 0, -4733, 0, -182, 1274, 0, 0, -13289, 0, 364, -18750, 0, 0, -8009, 0,
  0, 1456, 0, -546, -11832, 0, 0, -17476, 0, 0, -9648, 0, 182, 1638, 0, -728, -13653, 0, -1820, -15837, 0, -2366, -9648, 0, 728, 1638
};
static const int32_t data_18D70[57] = {
  0, 0, -3640, 0, 0, -2366, -4369, 1820, -546, 0, 0, -5279, 0, 0, -6007, 0, 0, -2912, 0, 0, -910, 0, 0, -6553, 0, 0, -6735, 0, 0, -3640, 0, 0, -182,
  0, 0, -5461, 0, 0, -6007, 0, 0, -2366, 0, 0, 0, 0, 0, -5643, 0, 0, -4733, 0, 0, -2366, -1456, 0, 728
};
static const Vector4 data_18E54 = {.x = 1.0f, .y = 0.4f, .z = 0.4f, .w = 0.4f};
static const FogColourData data_18E64 = {.red = 0.1f, .green = 1400.0f, .blue = 0.4f, .colour = 0xFF200000U};
static const Vector4 data_18E74 = {.x = 0.3f, .y = 0.4f, .z = 0.4f, .w = 0.4f};
static const Vector4 data_18E84 = {.x = 1.0f, .y = 0.9f, .z = 0.75f, .w = 0.7f};
static const Vector3 data_18E94 = {.x = -0.53f, .y = -0.4f, .z = -0.6f};
static const uint32_t data_18EA0[4] = {0xFF260000U, 0xFF260000U, 0x541C1C1CU, 0x541C1C1CU};
static const FogColourData data_18EB0 = {.red = 0.1f, .green = 1000.0f, .blue = 0.23f, .colour = 0xFF000000U};
static const uint32_t data_18EC0[4] = {0x40A0A1A0U, 0x40A0A1A0U, 0xFFFFFFU, 0xFFFFFFU};
static const Vector4 data_18ED0 = {.x = 0.0f, .y = 0.3f, .z = 0.3f, .w = 0.3f};
static const Vector4 data_18EE0 = {.x = 1.0f, .y = 1.0f, .z = 0.85f, .w = 1.0f};
static const Vector3 data_18EF0 = {.x = 0.0f, .y = -0.3f, .z = 0.0f};
static const Vector4 data_18EFC = {.x = 0.43f, .y = 0.3f, .z = 0.3f, .w = 0.3f};
static const Vector4 data_18F0C = {.x = 1.0f, .y = 0.7f, .z = 0.7f, .w = 0.77f};
static const Vector3 data_18F1C = {.x = -0.4f, .y = -0.76f, .z = -0.41f};
static const FogColourData data_18F28 = {.red = 0.1f, .green = 1000.0f, .blue = 0.5f, .colour = 0xFF000020U};
static const uint32_t data_18F38[4] = {0, 0, 0, 0};
static const Vector4 data_18F48 = {.x = 0.26f, .y = 0.3f, .z = 0.3f, .w = 0.3f};
static const Vector4 data_18F58 = {.x = 1.0f, .y = 0.69f, .z = 0.7f, .w = 0.9f};
static const Vector3 data_18F68 = {.x = 0.37f, .y = -0.22f, .z = 0.86f};
static const FogColourData data_18F74 = {.red = 30.1f, .green = 1000.0f, .blue = 1.5f, .colour = 0xFF000000U};
static const uint32_t data_18F84[4] = {0, 0, 0, 0};
static const FogColourData data_18F94 = {.red = 0.1f, .green = 1400.0f, .blue = 1.3f, .colour = 0xFFA16A50U};
static const uint32_t data_18FA4[4] = {0xFF000000U, 0xFF000000U, 0xFF7060U, 0xFF7060U};
static const Vector4 data_18FB4 = {.x = 0.94f, .y = 0.3f, .z = 0.3f, .w = 0.3f};
static const Vector4 data_18FC4 = {.x = 1.0f, .y = 0.8f, .z = 0.7f, .w = 0.7f};
static const Vector3 data_18FD4 = {.x = -1.0f, .y = -1.0f, .z = -0.64f};
static const Vector4 data_18FE0 = {.x = 1.0f, .y = 0.3f, .z = 0.3f, .w = 0.3f};
static const Vector3 data_18FF0 = {.x = 0.0f, .y = 0.0f, .z = 26.0f};
static const char data_18FFC[13] __attribute__((scn_string_literal)) = "model/object";
static const char data_19009[13] __attribute__((scn_string_literal)) = "siranami.pvr";
static const char data_19016[13] __attribute__((scn_string_literal)) = "model/object";
static const char data_19023[11] __attribute__((scn_string_literal)) = "trsea4.mt6";
static const char data_1902E[13] __attribute__((scn_string_literal)) = "model/object";
static const char data_1903B[13] __attribute__((scn_string_literal)) = "ALPHTBL2.BIN";
static const char data_19048[13] __attribute__((scn_string_literal)) = "model/object";
static const char data_19055[15] __attribute__((scn_string_literal)) = "griddata.bin";
static const Vector3 data_19064 = {.x = 0.3f, .y = 0.0f, .z = -7.2f};
static const char data_19070[13] __attribute__((scn_string_literal)) = "model/object";
static const char data_1907D[11] __attribute__((scn_string_literal)) = "shibu.pvr";
static const Vector3i data_19088 = {.x = 2366, .y = 1637, .z = -2182};
static const Vector3i data_19094 = {.x = -5097, .y = 546, .z = 0};
static const int32_t data_190A0[57] = {
  0, 0, -7281, 0, 2730, -7281, 0, 1456, 0, 0, 0, -18022, 0, 0, -15291, 0, 1092, -17658, 0, 0, 0, 0, 0, -12196, 0, 0, -15109, 0, 0, -16384, 0, 0, 0, 0,
  0, -9466, 0, 728, -12379, 0, 0, -14563, 0, 0, 0, 0, 819, -12014, 0, -1274, -9102, 0, -2366, -14927, 0, 0, 0
};
static const int32_t data_19184[57] = {
  0, -546, -3458, 0, 1638, -2548, 0, 5825, -546, 0, 728, -10194, 0, 0, -11832, 0, 0, -15837, 0, 364, -3458, 0, 0, -8920, 0, 0, -15109, 0, -182,
  -14745, 0, 0, -2912, 0, 0, -13835, 0, 0, -11286, 0, 0, -14927, 0, 0, -2002, 0, 0, -12379, 0, 0, -8556, 0, -1820, -12925, -2002, -546, -1456
};
static const int32_t data_19268[57] = {
  0, 0, -3640, 0, 0, -2366, -4369, 1820, -546, 0, 0, -5279, 0, 0, -6007, 0, 0, -2912, 0, 0, -910, 0, 0, -6553, 0, 0, -6735, 0, 0, -3640, 0, 0, -182,
  0, 0, -5461, 0, 0, -6007, 0, 0, -2366, 0, 0, 0, 0, 0, -5643, 0, 0, -4733, 0, 0, -2366, -1456, 0, 728
};
static const Vector3 data_1934C = {.x = 104.7f, .y = 12.6f, .z = 7.1f};
static const Vector3 data_19358 = {.x = 0.0196078438f, .y = 0.0647058859f, .z = 0.0196078438f};
static const char data_19364[13] __attribute__((scn_string_literal)) = "model/object";
static const char data_19371[9] __attribute__((scn_string_literal)) = "sea2.pvr";
static const char data_1937A[13] __attribute__((scn_string_literal)) = "model/object";
static const char data_19387[11] __attribute__((scn_string_literal)) = "trsea4.mt6";
static const char data_19392[13] __attribute__((scn_string_literal)) = "model/object";
static const char data_1939F[12] __attribute__((scn_string_literal)) = "TRWTBL1.BIN";
static const char data_193AB[11] __attribute__((scn_string_literal)) = "M_0180.BIN";
static const char data_193B6[12] __attribute__((scn_string_literal)) = "M_01156.BIN";
static const char data_193C2[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_193C7[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_193CC[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_193D1[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_193D6[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_193DB[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_193E0[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_193E5[11] __attribute__((scn_string_literal)) = "a1_end.snd";
static const char data_193F0[] __attribute__((scn_string_literal)) = "sprite";
static const char data_193F7[13] __attribute__((scn_string_literal)) = "SCROLL27.SPR";
static const char data_19404[] __attribute__((scn_string_literal)) = "sprite";
static const char data_1940B[13] __attribute__((scn_string_literal)) = "SCROLL05.SPR";
static const char data_19418[4] __attribute__((scn_string_literal)) = "END";
static const char data_1941C[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_19421[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_19426[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_1942B[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_19430[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_19435[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_1943A[6] __attribute__((scn_string_literal)) = "FREE";
static const Vector4 data_19440 = {.x = 1.0f, .y = 0.4f, .z = 0.4f, .w = 0.4f};
static const char data_19450[8] __attribute__((scn_string_literal)) = "ENDMAP";
static const Vector4 data_19458 = {.x = 1.0f, .y = 0.3f, .z = 0.3f, .w = 0.3f};
static const Vector4 data_19468 = {.x = 0.26f, .y = 0.3f, .z = 0.3f, .w = 0.3f};
static const Vector4 data_19478 = {.x = 1.0f, .y = 1.0f, .z = 1.0f, .w = 1.0f};
static const Vector3 data_19488 = {.x = 0.4f, .y = 0.8f, .z = 0.45f};
static const Vector4 data_19494 = {.x = 1.0f, .y = 0.4f, .z = 0.4f, .w = 0.4f};
static const Vector4 data_194A4 = {.x = 0.3f, .y = 0.3f, .z = 0.3f, .w = 0.3f};
static const Vector4 data_194B4 = {.x = 1.0f, .y = 0.6f, .z = 0.49f, .w = 0.7f};
static const Vector3 data_194C4 = {.x = 0.84f, .y = -0.02f, .z = -0.27f};
static const Vector4 data_194D0 = {.x = 0.3f, .y = 0.3f, .z = 0.3f, .w = 0.3f};
static const Vector4 data_194E0 = {.x = 1.0f, .y = 0.8f, .z = 0.83f, .w = 0.68f};
static const Vector3 data_194F0 = {.x = 0.84f, .y = -0.02f, .z = -0.27f};
static const Vector4 data_194FC = {.x = 1.0f, .y = 0.3f, .z = 0.3f, .w = 0.3f};
static const int32_t data_1950C[3] = {1456, 2366, -9102};
static Vector3i data_19518 = {.x = 0, .y = 0, .z = 0};
static const Vector3 data_19524 = {.x = 0.5f, .y = 0.5f, .z = 0.5f};
static const Vector3 data_19530 = {.x = 0.034f, .y = 0.355f, .z = -0.168f};
static const Vector3i data_1953C = {.x = 12833, .y = 50432, .z = 38218};
static const Vector3 data_19548 = {.x = -17.5f, .y = -1.8f, .z = -15.5f};
static const char data_19554[13] __attribute__((scn_string_literal)) = "model/object";
static const char data_19561[9] __attribute__((scn_string_literal)) = "SEA2.PVR";
static const char data_1956A[13] __attribute__((scn_string_literal)) = "model/object";
static const char data_19577[13] __attribute__((scn_string_literal)) = "TRSEA4.MT6";
static const Vector4 data_19584 = {.x = 1.0f, .y = 0.3f, .z = 0.3f, .w = 0.3f};
static const FogColourData data_19594 = {.red = 9.1f, .green = 625.0f, .blue = 0.96f, .colour = 0xFF1D1A30U};
static const uint32_t data_195A4[4] = {0xFF000000U, 0xFF000000U, 0x3016006AU, 0x3016006AU};
static const char data_195B4[] __attribute__((scn_string_literal)) = "sprite";
static const char data_195BB[25] = "SCROLL69.SPR\000\000\000乎ff豼\000\000xﾁ";
static const FogColourData data_195D4 = {.red = 0.1f, .green = 800.0f, .blue = 1.0f, .colour = 0xFF2B105FU};
static const uint32_t data_195E4[4] = {0xFF000000U, 0xFF000000U, 0xE6169C42U, 0xE6169C42U};
static const char data_195F4[] __attribute__((scn_string_literal)) = "sprite";
static const char data_195FB[13] __attribute__((scn_string_literal)) = "SCROLL19.SPR";
static const int32_t data_19608[57] = {
  0, -546, -3458, 0, 1638, -2548, 0, 5825, -546, 0, 728, -10194, 0, 0, -11832, 0, 0, -15837, 0, 364, -3458, 0, 0, -8920, 0, 0, -15109, 0, -182,
  -14745, 0, 0, -2912, 0, 0, -13835, 0, 0, -11286, 0, 0, -14927, 0, 0, -2002, 0, 0, -12379, 0, 0, -8556, 0, -1820, -12925, -2002, -546, -1456
};
static const int32_t data_196EC[57] = {
  0, 0, -5461, 0, -2366, -1092, 0, 2548, 546, 0, -546, -3822, 0, 1274, -2366, 0, 0, -1456, 0, 6007, 546, 0, 0, -3822, 0, 0, -2730, 0, 0, -5279, 0,
  2184, -182, 0, 182, -3822, 0, 0, -2912, 0, -728, -3458, 0, 728, -546, 0, -728, -3640, 0, -910, -2730, 0, 0, -2730, 364, -546, -910
};
static const char data_197D0[11] __attribute__((scn_string_literal)) = "amb013.snd";
static const char data_197DB[11] __attribute__((scn_string_literal)) = "bgm111.snd";
static const char data_197E6[13] __attribute__((scn_string_literal)) = "a1_busno.snd";
static const char data_197F3[] __attribute__((scn_string_literal)) = "scene/03/MEND";
static const char data_19801[] __attribute__((scn_string_literal)) = "BUSAUTH";
static const char data_19809[13] __attribute__((scn_string_literal)) = "seqdataB.bin";
static const char data_19816[8] __attribute__((scn_string_literal)) = "BUSAUTH";
static const char data_1981E[10] __attribute__((scn_string_literal)) = "BUSAUTH";
static Vector3i data_19828 = {.x = 0, .y = 14108, .z = 0};
static Vector3i data_19834 = {.x = 0, .y = -30474, .z = 0};
static Vector3i data_19840 = {.x = 0, .y = -14108, .z = 0};
static Vector3i data_1984C = {.x = 0, .y = 30474, .z = 0};
static const int32_t data_19858[3] = {1456, 2366, -9102};
static const Vector3i data_19864 = {.x = 2366, .y = 1637, .z = -2182};
static const int32_t data_19870[3] = {-5097, 546, 0};
static const Vector4 data_1987C = {.x = 1.0f, .y = 0.3f, .z = 0.3f, .w = 0.3f};
static Vector3i data_1988C = {.x = 2730, .y = 0, .z = 0};
static Vector3i data_19898 = {.x = -2730, .y = 0, .z = 0};
static const Vector3 data_198A4 = {.x = 0.034f, .y = 0.355f, .z = -0.168f};
static const Vector3i data_198B0 = {.x = 12833, .y = 50432, .z = 38218};
static const char data_198BC[11] __attribute__((scn_string_literal)) = "amb013.snd";
static const char data_198C7[] __attribute__((scn_string_literal)) = "scene/03/MEND";
static const uint8_t data_198D5[4] = {
  0x4D, 0x41, 0x50, 0x00
};
static const char data_198D9[] __attribute__((scn_string_literal)) = "scene/03/MEND";
static const char data_198E7[] __attribute__((scn_string_literal)) = "MODELS";
static const char data_198EE[] __attribute__((scn_string_literal)) = "scene/03/MEND";
static const char data_198FC[] __attribute__((scn_string_literal)) = "CHAIAUTH";
static const char data_19905[9] __attribute__((scn_string_literal)) = "CHAIAUTH";
static const char data_1990E[7] __attribute__((scn_string_literal)) = "MODELS";
static const char data_19915[7] __attribute__((scn_string_literal)) = "MAP";
static const Vector3 data_1991C = {.x = 1000.0f, .y = -50.0f, .z = 1000.0f};
static const char data_19928[32] = "TEKKOTU\000\365JzB\000\000\200>\266\004\005B\220\301\000\000\234\\\000\000D\177";
static const char data_19948[11] __attribute__((scn_string_literal)) = "tsm001.snd";
static const char data_19953[12] __attribute__((scn_string_literal)) = "a1_saib.snd";
static const char data_1995F[13] __attribute__((scn_string_literal)) = "battle_1.snd";
static const char data_1996C[13] __attribute__((scn_string_literal)) = "e1batchi.snd";
static const char data_19979[13] __attribute__((scn_string_literal)) = "seqdata3.bin";
static const char data_19986[9] __attribute__((scn_string_literal)) = "CHAIAUTH";
static const char data_1998F[12] __attribute__((scn_string_literal)) = "fre2400.snd";
static const char data_1999B[13] __attribute__((scn_string_literal)) = "a1_momor.snd";
static const char data_199A8[13] __attribute__((scn_string_literal)) = "seqdata0.bin";
static const char data_199B5[9] __attribute__((scn_string_literal)) = "CHAIAUTH";
static const char data_199BE[13] __attribute__((scn_string_literal)) = "seqdata1.bin";
static const char data_199CB[9] __attribute__((scn_string_literal)) = "CHAIAUTH";
static const char data_199D4[13] __attribute__((scn_string_literal)) = "seqdata5.bin";
static const char data_199E1[9] __attribute__((scn_string_literal)) = "CHAIAUTH";
static const char data_199EA[13] __attribute__((scn_string_literal)) = "seqdata6.bin";
static const char data_199F7[9] __attribute__((scn_string_literal)) = "CHAIAUTH";
static const char data_19A00[13] __attribute__((scn_string_literal)) = "seqdata4.bin";
static const char data_19A0D[9] __attribute__((scn_string_literal)) = "CHAIAUTH";
static const char data_19A16[11] __attribute__((scn_string_literal)) = "bgm067.snd";
static const char data_19A21[11] __attribute__((scn_string_literal)) = "bgm032.snd";
static const char data_19A2C[13] __attribute__((scn_string_literal)) = "a1_momor.snd";
static const char data_19A39[] __attribute__((scn_string_literal)) = "scene/03/MEND";
static const char data_19A47[] __attribute__((scn_string_literal)) = "TRAPAUTH";
static const char data_19A50[13] __attribute__((scn_string_literal)) = "seqdata1.bin";
static const char data_19A5D[9] __attribute__((scn_string_literal)) = "TRAPAUTH";
static const char data_19A66[11] __attribute__((scn_string_literal)) = "bgm095.snd";
static const char data_19A71[13] __attribute__((scn_string_literal)) = "a1_shuko.snd";
static const char data_19A7E[10] __attribute__((scn_string_literal)) = "TRAPAUTH";
static const FogColourData data_19A88 = {.red = 9.0f, .green = 545.0f, .blue = 0.38f, .colour = 0xFF180A68U};
static const uint32_t data_19A98[4] = {0xFF008CC0U, 0xFF008CC0U, 0x5C1919U, 0x5C1919U};
static const Vector4 data_19AA8 = {.x = 1.0f, .y = 0.4f, .z = 0.4f, .w = 0.4f};
static const char data_19AB8[11] __attribute__((scn_string_literal)) = "amb018.snd";
static const char data_19AC3[11] __attribute__((scn_string_literal)) = "bgm070.snd";
static const char data_19ACE[13] __attribute__((scn_string_literal)) = "a1_nooma.snd";
static const char data_19ADB[6] __attribute__((scn_string_literal)) = "01JIN";
static const char data_19AE1[] __attribute__((scn_string_literal)) = "scene/03/MEND";
static const char data_19AEF[] __attribute__((scn_string_literal)) = "NZOM";
static const char data_19AF4[13] __attribute__((scn_string_literal)) = "seqdata0.bin";
static const char data_19B01[5] __attribute__((scn_string_literal)) = "NZOM";
static const char data_19B06[] __attribute__((scn_string_literal)) = "sprite";
static const char data_19B0D[13] __attribute__((scn_string_literal)) = "SCROLL27.SPR";
static const char data_19B1A[6] __attribute__((scn_string_literal)) = "01JIN";
static const char data_19B20[5] __attribute__((scn_string_literal)) = "NZOM";
static const char data_19B25[] __attribute__((scn_string_literal)) = "scene/03/MEND";
static const char data_19B33[] __attribute__((scn_string_literal)) = "ENDAUTH";
static const char data_19B3B[13] __attribute__((scn_string_literal)) = "seqdata1.bin";
static const char data_19B48[8] __attribute__((scn_string_literal)) = "ENDAUTH";
static const char data_19B50[13] __attribute__((scn_string_literal)) = "seqdata2.bin";
static const char data_19B5D[8] __attribute__((scn_string_literal)) = "ENDAUTH";
static const char data_19B65[13] __attribute__((scn_string_literal)) = "seqdata3.bin";
static const char data_19B72[8] __attribute__((scn_string_literal)) = "ENDAUTH";
static const char data_19B7A[13] __attribute__((scn_string_literal)) = "seqdata4.bin";
static const char data_19B87[8] __attribute__((scn_string_literal)) = "ENDAUTH";
static const char data_19B8F[13] __attribute__((scn_string_literal)) = "seqdata5.bin";
static const char data_19B9C[8] __attribute__((scn_string_literal)) = "ENDAUTH";
static const char data_19BA4[13] __attribute__((scn_string_literal)) = "seqdata6.bin";
static const char data_19BB1[8] __attribute__((scn_string_literal)) = "ENDAUTH";
static const char data_19BB9[13] __attribute__((scn_string_literal)) = "seqdata7.bin";
static const char data_19BC6[8] __attribute__((scn_string_literal)) = "ENDAUTH";
static const char data_19BCE[13] __attribute__((scn_string_literal)) = "seqdata8.bin";
static const char data_19BDB[8] __attribute__((scn_string_literal)) = "ENDAUTH";
static const char data_19BE3[13] __attribute__((scn_string_literal)) = "seqdata9.bin";
static const char data_19BF0[8] __attribute__((scn_string_literal)) = "ENDAUTH";
static const char data_19BF8[13] __attribute__((scn_string_literal)) = "seqdataA.bin";
static const char data_19C05[8] __attribute__((scn_string_literal)) = "ENDAUTH";
static const char data_19C0D[13] __attribute__((scn_string_literal)) = "seqdataB.bin";
static const char data_19C1A[8] __attribute__((scn_string_literal)) = "ENDAUTH";
static const char data_19C22[13] __attribute__((scn_string_literal)) = "seqdataC.bin";
static const char data_19C2F[8] __attribute__((scn_string_literal)) = "ENDAUTH";
static const char data_19C37[13] __attribute__((scn_string_literal)) = "seqdataD.bin";
static const char data_19C44[8] __attribute__((scn_string_literal)) = "ENDAUTH";
static const char data_19C4C[13] __attribute__((scn_string_literal)) = "seqdataE.bin";
static const char data_19C59[8] __attribute__((scn_string_literal)) = "ENDAUTH";
static const char data_19C61[13] __attribute__((scn_string_literal)) = "seqdataF.bin";
static const char data_19C6E[8] __attribute__((scn_string_literal)) = "ENDAUTH";
static const char data_19C76[13] __attribute__((scn_string_literal)) = "seqdataT.bin";
static const char data_19C83[8] __attribute__((scn_string_literal)) = "ENDAUTH";
static const char data_19C8B[11] __attribute__((scn_string_literal)) = "amb013.snd";
static const char data_19C96[13] __attribute__((scn_string_literal)) = "a1_shuko.snd";
static const char data_19CA3[] __attribute__((scn_string_literal)) = "scene/03/MEND";
static const char data_19CB1[] __attribute__((scn_string_literal)) = "ENDMAP";
static const char data_19CB8[11] __attribute__((scn_string_literal)) = "bgm063.snd";
static const char data_19CC3[13] __attribute__((scn_string_literal)) = "a1_ranbo.snd";
static const char data_19CD0[] __attribute__((scn_string_literal)) = "scene/03/MEND";
static const char data_19CDE[] __attribute__((scn_string_literal)) = "YARN";
static const char data_19CE3[13] __attribute__((scn_string_literal)) = "seqdata0.bin";
static const char data_19CF0[5] __attribute__((scn_string_literal)) = "YARN";
static const char data_19CF5[5] __attribute__((scn_string_literal)) = "YARN";
static const char data_19CFA[11] __attribute__((scn_string_literal)) = "M_FGT4.BIN";
static const char data_19D05[13] __attribute__((scn_string_literal)) = "battle_1.snd";
static const char data_19D12[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_19D17[6] __attribute__((scn_string_literal)) = "A0184";
static const char data_19D1D[7] __attribute__((scn_string_literal)) = "A0184B";
static const char data_19D24[7] __attribute__((scn_string_literal)) = "A0184C";
static const char data_19D2B[7] __attribute__((scn_string_literal)) = "A0184D";
static const char data_19D32[6] __attribute__((scn_string_literal)) = "E1062";
static const char data_19D38[7] __attribute__((scn_string_literal)) = "A0184D";
static const char data_19D3F[7] __attribute__((scn_string_literal)) = "A0184C";
static const char data_19D46[42] = "A0184B\000\000\000\000\236\002\240\002\242\002\245\002\254\002\252\002\250\002\000\003\370\000\372\000\365\000\367\000\366\000\373\000\371\000\000\003";
static const char data_19D70[61] __attribute__((scn_string_literal)) = "WAZA Warning : handle is not NULL, and create same handle !\012";
static const char data_19DAD[30] __attribute__((scn_string_literal)) = "POS POS_RYOU_0 = %f, %f, %f.\012";
static const char data_19DCB[30] __attribute__((scn_string_literal)) = "POS POS_KAIS_0 = %f, %f, %f.\012";
static const char data_19DE9[29] __attribute__((scn_string_literal)) = "define ANG_RYOU_0 DEG(%d.0)\012";
static const char data_19E06[29] __attribute__((scn_string_literal)) = "define ANG_KAIS_0 DEG(%d.0)\012";
static const char data_19E23[4] __attribute__((scn_string_literal)) = "%s\012";
static const char data_19E27[37] __attribute__((scn_string_literal)) = "Konnna Error ha Detekonai hazu : %d\012";
static const Vector3 data_19E4C = {.x = 1.0f, .y = 1.0f, .z = 1.0f};
static const Vector3i data_19E58 = {.x = 0, .y = 0, .z = 0};
static const char data_19E64[14] __attribute__((scn_string_literal)) = "Entry Model!\012";
static const char data_19E72[38] __attribute__((scn_string_literal)) = "WZ ModoriMove Limit Over !!!!!!!!!!!\012";
static const char data_19E98[30] __attribute__((scn_string_literal)) = "WazaDen: Load PackFile %s:%s\012";
static const char data_19EB6[34] __attribute__((scn_string_literal)) = "WazaDen: Now Loading PackFile.\012";
static const Vector3 data_19ED8 = {.x = 65.1f, .y = 0.0f, .z = 31.0f};
static const Vector3 data_19EE4 = {.x = 64.0f, .y = 0.0f, .z = 29.8f};
static const Vector3 data_19EF0 = {.x = 65.0f, .y = 0.0f, .z = 29.9f};
static const Vector3 data_19EFC = {.x = 63.7f, .y = 0.0f, .z = 31.0f};
static const Vector3 data_19F08 = {.x = 65.0f, .y = 0.0f, .z = 29.8f};
static const Vector3 data_19F14 = {.x = 66.2f, .y = 0.0f, .z = 30.7f};
static const float data_19F20[3] = {62.7f, 0.4f, 30.5f};
static const int32_t data_19F2C[3] = {32567, 2730, 32768};
static const Vector3 data_19F38 = {.x = 66.839699f, .y = 0.0f, .z = 30.524401f};
static const Vector3 data_19F44 = {.x = 65.146301f, .y = 0.0f, .z = 31.350401f};
static const Vector3 data_19F50 = {.x = 66.864304f, .y = 0.0f, .z = 32.7365f};
static const Vector3 data_19F5C = {.x = 67.111099f, .y = 0.0f, .z = 30.417101f};
static const Vector3 data_19F68 = {.x = 66.007797f, .y = 0.0f, .z = 31.278999f};
static const uint32_t data_19F74[3] = {0x42829803U, 0, 0x42032F35U};
static const Vector3 data_19F80 = {.x = 66.996902f, .y = 0.0f, .z = 28.496099f};
static const Vector3 data_19F8C = {.x = 65.1f, .y = 0.0f, .z = 31.0f};
static const Vector3 data_19F98 = {.x = 64.1f, .y = 0.0f, .z = 29.8f};
static const float data_19FA4[3] = {66.5f, 0.0f, 29.8f};
static const Vector3 data_19FB0 = {.x = 65.45f, .y = 0.0f, .z = 30.38f};
static const Vector3 data_19FBC = {.x = 64.0f, .y = 0.0f, .z = 29.8f};
static const float data_19FC8[3] = {65.0f, 0.0f, 29.9f};
static const Vector3 data_19FD4 = {.x = 65.45f, .y = 0.0f, .z = 30.38f};
static const Vector3 data_19FE0 = {.x = 65.77f, .y = 0.0f, .z = 31.95f};
static const uint32_t data_19FEC = 0x3DC2363B;
static const float data_19FF0[2] = {0.0f, 1.350878f};
static const uint32_t data_19FF8 = 0xBDC2363B;
static const float data_19FFC[2] = {0.0f, -1.350878f};
static const uint32_t data_1A004 = 0x3F261FF3;
static const float data_1A008[2] = {0.0f, 0.542532f};
static const uint32_t data_1A010 = 0xBF261FF3;
static const float data_1A014[2] = {0.0f, -0.542532f};
static const uint32_t data_1A01C = 0x3CC623D1;
static const float data_1A020[2] = {0.0f, 0.689467f};
static const uint32_t data_1A028 = 0xBCC623D1;
static const float data_1A02C[2] = {0.0f, -0.689467f};
static const char data_1A034[26] __attribute__((scn_string_literal)) = "<<< WazaDenjyu Start >>>\012";
static const char data_1A04E[11] __attribute__((scn_string_literal)) = "bgm064.snd";
static const char data_1A059[13] __attribute__((scn_string_literal)) = "a1_chinw.snd";
static const char data_1A066[13] __attribute__((scn_string_literal)) = "battle_1.snd";
static const char data_1A073[16] __attribute__((scn_string_literal)) = "/scene/03/MEND/";
static const char data_1A083[5] __attribute__((scn_string_literal)) = "M0W0";
static const char data_1A088[5] __attribute__((scn_string_literal)) = "M0W0";
static const char data_1A08D[5] __attribute__((scn_string_literal)) = "M0W0";
static const char data_1A092[5] __attribute__((scn_string_literal)) = "M0W0";
static const char data_1A097[1] __attribute__((scn_string_literal)) = "";
static const char data_1A098[5] __attribute__((scn_string_literal)) = "M0W0";
static const char data_1A09D[1] __attribute__((scn_string_literal)) = "";
static const char data_1A09E[5] __attribute__((scn_string_literal)) = "M0W0";
static const char data_1A0A3[1] __attribute__((scn_string_literal)) = "";
static const char data_1A0A4[5] __attribute__((scn_string_literal)) = "M0W0";
static const char data_1A0A9[1] __attribute__((scn_string_literal)) = "";
static const char data_1A0AA[5] __attribute__((scn_string_literal)) = "M0W0";
static const char data_1A0AF[1] __attribute__((scn_string_literal)) = "";
static const char data_1A0B0[5] __attribute__((scn_string_literal)) = "M0W0";
static const char data_1A0B5[1] __attribute__((scn_string_literal)) = "";
static const char data_1A0B6[5] __attribute__((scn_string_literal)) = "M0W0";
static const char data_1A0BB[1] __attribute__((scn_string_literal)) = "";
static const char data_1A0BC[5] __attribute__((scn_string_literal)) = "M0W0";
static const char data_1A0C1[1] __attribute__((scn_string_literal)) = "";
static const char data_1A0C2[5] __attribute__((scn_string_literal)) = "M0W0";
static const char data_1A0C7[1] __attribute__((scn_string_literal)) = "";
static const char data_1A0C8[5] __attribute__((scn_string_literal)) = "M0W0";
static const char data_1A0CD[1] __attribute__((scn_string_literal)) = "";
static const char data_1A0CE[5] __attribute__((scn_string_literal)) = "M0W0";
static const char data_1A0D3[1] __attribute__((scn_string_literal)) = "";
static const char data_1A0D4[13] __attribute__((scn_string_literal)) = "HMOTM0W0.BIN";
static const char data_1A0E1[6] __attribute__((scn_string_literal)) = "E1016";
static const char data_1A0E7[6] __attribute__((scn_string_literal)) = "E1016";
static const char data_1A0ED[5] __attribute__((scn_string_literal)) = "M0W0";
static const char data_1A0F2[37] __attribute__((scn_string_literal)) = " stat %d/resutl %d/cause %d/line %d\012";
static const char data_1A117[329] = " frame %d/step %d/pad %x\012\000\000\000\000\000\000\000\000\000\000\000\000\302\266\377\377\000\000\000\000\000\000\000\000\267\340\377\377\000\000\000\000`\013\000\000\000\000\000\000\000\000\000\000\000\000\000\000\344\270\377\377\000\000\000\000\000\000\000\000\365\251\377\377\000\000\000\000\330\002\000\000\331\302\377\377\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\344\270\377\377\000\000\000\000\000\000\000\000\310\261\377\377\000\000\000\000\000\000\000\000\224\276\377\377\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\344\270\377\377\000\000\000\000\000\000\000\000\310\261\377\377\000\000\000\000\000\000\000\000J\277\377\377\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000x\267\377\377\000\000\000\000\224\376\377\377\310\261\377\377\000\000\000\000\000\000\000\000\267\300\377\377\336\375\377\377\006\373\377\377\000\000\000\000\004\3473\301\300[\024@\250\306\270\301;\3373\301\341z\024@-\262\256\301\315\314,\301\327\243\220?q=\270\301\000\000\000\000\000@\000\000\000\000\000\000d\314(\301\345\320\262?\005\305\261\301\000\300\000\000\000\000\000\000\000\300";
static const Vector3i data_1A260 = {.x = -4733, .y = 3094, .z = 0};
static const Vector4 data_1A26C = {.x = 1.0f, .y = 0.3f, .z = 0.3f, .w = 0.3f};
static const char data_1A27C[36] __attribute__((scn_string_literal)) = "pos x y z : %8.4f %8.4f %8.4f : %f\012";
static const char data_1A2A0[24] __attribute__((scn_string_literal)) = "%8.4f %8.4f %8.4f : %f\012";
static const char data_1A2B8[32] __attribute__((scn_string_literal)) = "pos x y z : %8.4f %8.4f %8.4f \012";
static const char data_1A2D8[22] __attribute__((scn_string_literal)) = "ang x y z : %d %d %d\012";
static const char data_1A2EE[22] __attribute__((scn_string_literal)) = "ang x y z : %d %d %d\012";
static const char data_1A304[10] __attribute__((scn_string_literal)) = "E1016B001";
static const char data_1A30E[10] __attribute__((scn_string_literal)) = "E1016A001";
static const char data_1A318[10] __attribute__((scn_string_literal)) = "E1016B002";
static const char data_1A322[10] __attribute__((scn_string_literal)) = "E1016A002";
static const char data_1A32C[10] __attribute__((scn_string_literal)) = "E1016B003";
static const char data_1A336[10] __attribute__((scn_string_literal)) = "E1016C001";
static const char data_1A340[10] __attribute__((scn_string_literal)) = "E1016B004";
static const char data_1A34A[10] __attribute__((scn_string_literal)) = "E1016B005";
static const char data_1A354[10] __attribute__((scn_string_literal)) = "E1016A003";
static const char data_1A35E[10] __attribute__((scn_string_literal)) = "E1016B006";
static const char data_1A368[10] __attribute__((scn_string_literal)) = "E1016B007";
static const char data_1A372[10] __attribute__((scn_string_literal)) = "E1016A004";
static const char data_1A37C[10] __attribute__((scn_string_literal)) = "E1016A005";
static const char data_1A386[10] __attribute__((scn_string_literal)) = "E1016B008";
static const char data_1A390[10] __attribute__((scn_string_literal)) = "E1016B009";
static const char data_1A39A[10] __attribute__((scn_string_literal)) = "E1016B115";
static const char data_1A3A4[10] __attribute__((scn_string_literal)) = "E1016C002";
static const char data_1A3AE[10] __attribute__((scn_string_literal)) = "E1016B011";
static const char data_1A3B8[10] __attribute__((scn_string_literal)) = "E1016B012";
static const char data_1A3C2[10] __attribute__((scn_string_literal)) = "E1016B013";
static const char data_1A3CC[10] __attribute__((scn_string_literal)) = "E1016B014";
static const char data_1A3D6[10] __attribute__((scn_string_literal)) = "E1016B015";
static const char data_1A3E0[10] __attribute__((scn_string_literal)) = "E1016B016";
static const char data_1A3EA[10] __attribute__((scn_string_literal)) = "E1016B017";
static const char data_1A3F4[10] __attribute__((scn_string_literal)) = "E1016B018";
static const char data_1A3FE[10] __attribute__((scn_string_literal)) = "E1016B019";
static const char data_1A408[10] __attribute__((scn_string_literal)) = "E1016B020";
static const char data_1A412[10] __attribute__((scn_string_literal)) = "E1016B021";
static const char data_1A41C[10] __attribute__((scn_string_literal)) = "E1016B022";
static const char data_1A426[10] __attribute__((scn_string_literal)) = "E1016B023";
static const char data_1A430[10] __attribute__((scn_string_literal)) = "E1016B020";
static const char data_1A43A[10] __attribute__((scn_string_literal)) = "E1016B024";
static const char data_1A444[10] __attribute__((scn_string_literal)) = "E1016B025";
static const char data_1A44E[10] __attribute__((scn_string_literal)) = "E1016B026";
static const char data_1A458[10] __attribute__((scn_string_literal)) = "E1016B027";
static const char data_1A462[10] __attribute__((scn_string_literal)) = "E1016B028";
static const char data_1A46C[10] __attribute__((scn_string_literal)) = "E1016B029";
static const char data_1A476[10] __attribute__((scn_string_literal)) = "E1016B030";
static const char data_1A480[10] __attribute__((scn_string_literal)) = "E1016B031";
static const char data_1A48A[10] __attribute__((scn_string_literal)) = "E1016B032";
static const char data_1A494[10] __attribute__((scn_string_literal)) = "E1016B033";
static const char data_1A49E[10] __attribute__((scn_string_literal)) = "E1016C003";
static const char data_1A4A8[10] __attribute__((scn_string_literal)) = "E1016B034";
static const char data_1A4B2[10] __attribute__((scn_string_literal)) = "E1016B035";
static const char data_1A4BC[10] __attribute__((scn_string_literal)) = "E1016C004";
static const char data_1A4C6[10] __attribute__((scn_string_literal)) = "E1016B036";
static const char data_1A4D0[10] __attribute__((scn_string_literal)) = "E1016B037";
static const char data_1A4DA[10] __attribute__((scn_string_literal)) = "E1016B038";
static const char data_1A4E4[10] __attribute__((scn_string_literal)) = "E1016B039";
static const char data_1A4EE[10] __attribute__((scn_string_literal)) = "E1016B040";
static const char data_1A4F8[10] __attribute__((scn_string_literal)) = "E1016B041";
static const char data_1A502[10] __attribute__((scn_string_literal)) = "E1016B042";
static const char data_1A50C[10] __attribute__((scn_string_literal)) = "E1016B027";
static const char data_1A516[10] __attribute__((scn_string_literal)) = "E1016B043";
static const char data_1A520[10] __attribute__((scn_string_literal)) = "E1016B044";
static const char data_1A52A[10] __attribute__((scn_string_literal)) = "E1016B053";
static const char data_1A534[10] __attribute__((scn_string_literal)) = "E1016B054";
static const char data_1A53E[10] __attribute__((scn_string_literal)) = "E1016B055";
static const char data_1A548[10] __attribute__((scn_string_literal)) = "E1016B056";
static const char data_1A552[10] __attribute__((scn_string_literal)) = "E1016B059";
static const char data_1A55C[10] __attribute__((scn_string_literal)) = "E1016B045";
static const char data_1A566[10] __attribute__((scn_string_literal)) = "E1016B046";
static const char data_1A570[10] __attribute__((scn_string_literal)) = "E1016B047";
static const char data_1A57A[10] __attribute__((scn_string_literal)) = "E1016B048";
static const char data_1A584[10] __attribute__((scn_string_literal)) = "E1016B049";
static const char data_1A58E[10] __attribute__((scn_string_literal)) = "E1016B048";
static const char data_1A598[10] __attribute__((scn_string_literal)) = "E1016B057";
static const char data_1A5A2[10] __attribute__((scn_string_literal)) = "E1016B058";
static const char data_1A5AC[10] __attribute__((scn_string_literal)) = "E1016B049";
static const char data_1A5B6[10] __attribute__((scn_string_literal)) = "E1016B057";
static const char data_1A5C0[10] __attribute__((scn_string_literal)) = "E1016B060";
static const char data_1A5CA[10] __attribute__((scn_string_literal)) = "E1016B103";
static const char data_1A5D4[10] __attribute__((scn_string_literal)) = "E1016B104";
static const char data_1A5DE[10] __attribute__((scn_string_literal)) = "E1016B105";
static const char data_1A5E8[10] __attribute__((scn_string_literal)) = "E1016B038";
static const char data_1A5F2[10] __attribute__((scn_string_literal)) = "E1016B061";
static const char data_1A5FC[10] __attribute__((scn_string_literal)) = "E1016B062";
static const char data_1A606[10] __attribute__((scn_string_literal)) = "E1016B063";
static const char data_1A610[10] __attribute__((scn_string_literal)) = "E1016B064";
static const char data_1A61A[10] __attribute__((scn_string_literal)) = "E1016B065";
static const char data_1A624[10] __attribute__((scn_string_literal)) = "E1016B086";
static const char data_1A62E[10] __attribute__((scn_string_literal)) = "E1016B087";
static const char data_1A638[10] __attribute__((scn_string_literal)) = "E1016B088";
static const char data_1A642[10] __attribute__((scn_string_literal)) = "E1016B089";
static const char data_1A64C[10] __attribute__((scn_string_literal)) = "E1016B090";
static const char data_1A656[10] __attribute__((scn_string_literal)) = "E1016B060";
static const char data_1A660[10] __attribute__((scn_string_literal)) = "E1016B091";
static const char data_1A66A[10] __attribute__((scn_string_literal)) = "E1016B066";
static const char data_1A674[10] __attribute__((scn_string_literal)) = "E1016B067";
static const char data_1A67E[10] __attribute__((scn_string_literal)) = "E1016A006";
static const char data_1A688[10] __attribute__((scn_string_literal)) = "E1016B068";
static const char data_1A692[10] __attribute__((scn_string_literal)) = "E1016B115";
static const char data_1A69C[10] __attribute__((scn_string_literal)) = "E1016C002";
static const char data_1A6A6[10] __attribute__((scn_string_literal)) = "E1016A007";
static const char data_1A6B0[10] __attribute__((scn_string_literal)) = "E1016B069";
static const char data_1A6BA[10] __attribute__((scn_string_literal)) = "E1016B070";
static const char data_1A6C4[10] __attribute__((scn_string_literal)) = "E1016B031";
static const char data_1A6CE[10] __attribute__((scn_string_literal)) = "E1016B032";
static const char data_1A6D8[10] __attribute__((scn_string_literal)) = "E1016B033";
static const char data_1A6E2[10] __attribute__((scn_string_literal)) = "E1016C003";
static const char data_1A6EC[10] __attribute__((scn_string_literal)) = "E1016B034";
static const char data_1A6F6[10] __attribute__((scn_string_literal)) = "E1016B035";
static const char data_1A700[10] __attribute__((scn_string_literal)) = "E1016C004";
static const char data_1A70A[10] __attribute__((scn_string_literal)) = "E1016B036";
static const char data_1A714[10] __attribute__((scn_string_literal)) = "E1016A008";
static const char data_1A71E[10] __attribute__((scn_string_literal)) = "E1016A009";
static const char data_1A728[10] __attribute__((scn_string_literal)) = "E1016B025";
static const char data_1A732[10] __attribute__((scn_string_literal)) = "E1016B024";
static const char data_1A73C[10] __attribute__((scn_string_literal)) = "E1016B071";
static const char data_1A746[10] __attribute__((scn_string_literal)) = "E1016B072";
static const char data_1A750[10] __attribute__((scn_string_literal)) = "E1016B073";
static const char data_1A75A[10] __attribute__((scn_string_literal)) = "E1016B092";
static const char data_1A764[10] __attribute__((scn_string_literal)) = "E1016B093";
static const char data_1A76E[10] __attribute__((scn_string_literal)) = "E1016A010";
static const char data_1A778[10] __attribute__((scn_string_literal)) = "E1016B094";
static const char data_1A782[10] __attribute__((scn_string_literal)) = "E1016B095";
static const char data_1A78C[10] __attribute__((scn_string_literal)) = "E1016B097";
static const char data_1A796[10] __attribute__((scn_string_literal)) = "E1016A014";
static const char data_1A7A0[10] __attribute__((scn_string_literal)) = "E1016B094";
static const char data_1A7AA[10] __attribute__((scn_string_literal)) = "E1016B095";
static const char data_1A7B4[10] __attribute__((scn_string_literal)) = "E1016A015";
static const char data_1A7BE[10] __attribute__((scn_string_literal)) = "E1016B098";
static const char data_1A7C8[10] __attribute__((scn_string_literal)) = "E1016C002";
static const char data_1A7D2[10] __attribute__((scn_string_literal)) = "E1016B100";
static const char data_1A7DC[10] __attribute__((scn_string_literal)) = "E1016A016";
static const char data_1A7E6[10] __attribute__((scn_string_literal)) = "E1016B098";
static const char data_1A7F0[10] __attribute__((scn_string_literal)) = "E1016C002";
static const char data_1A7FA[10] __attribute__((scn_string_literal)) = "E1016A017";
static const char data_1A804[10] __attribute__((scn_string_literal)) = "E1016B101";
static const char data_1A80E[10] __attribute__((scn_string_literal)) = "E1016A018";
static const char data_1A818[10] __attribute__((scn_string_literal)) = "E1016B102";
static const char data_1A822[10] __attribute__((scn_string_literal)) = "E1016A013";
static const char data_1A82C[10] __attribute__((scn_string_literal)) = "E1016A016";
static const char data_1A836[10] __attribute__((scn_string_literal)) = "E1016B098";
static const char data_1A840[10] __attribute__((scn_string_literal)) = "E1016C002";
static const char data_1A84A[10] __attribute__((scn_string_literal)) = "E1016B074";
static const char data_1A854[10] __attribute__((scn_string_literal)) = "E1016B075";
static const char data_1A85E[10] __attribute__((scn_string_literal)) = "E1016B076";
static const char data_1A868[10] __attribute__((scn_string_literal)) = "E1016B077";
static const char data_1A872[10] __attribute__((scn_string_literal)) = "E1016B078";
static const char data_1A87C[10] __attribute__((scn_string_literal)) = "E1016B079";
static const char data_1A886[10] __attribute__((scn_string_literal)) = "E1016B080";
static const char data_1A890[10] __attribute__((scn_string_literal)) = "E1016B081";
static const char data_1A89A[10] __attribute__((scn_string_literal)) = "E1016B082";
static const char data_1A8A4[10] __attribute__((scn_string_literal)) = "E1016B083";
static const char data_1A8AE[10] __attribute__((scn_string_literal)) = "E1016B084";
static const char data_1A8B8[10] __attribute__((scn_string_literal)) = "E1016B085";
static const char data_1A8C2[10] __attribute__((scn_string_literal)) = "E1016B099";
static const char data_1A8CC[10] __attribute__((scn_string_literal)) = "E1016A015";
static const char data_1A8D6[10] __attribute__((scn_string_literal)) = "E1016B098";
static const char data_1A8E0[10] __attribute__((scn_string_literal)) = "E1016C002";
static const char data_1A8EA[10] __attribute__((scn_string_literal)) = "E1016A011";
static const char data_1A8F4[10] __attribute__((scn_string_literal)) = "E1016B096";
static const char data_1A8FE[10] __attribute__((scn_string_literal)) = "E1016C006";
static const char data_1A908[10] __attribute__((scn_string_literal)) = "E1016A012";
static const char data_1A912[10] __attribute__((scn_string_literal)) = "E1016C007";
static const char data_1A91C[10] __attribute__((scn_string_literal)) = "E1016A013";
static const char data_1A926[10] __attribute__((scn_string_literal)) = "E1016B050";
static const char data_1A930[10] __attribute__((scn_string_literal)) = "E1016B135";
static const char data_1A93A[10] __attribute__((scn_string_literal)) = "E1016B052";
static const char data_1A944[10] __attribute__((scn_string_literal)) = "E1016B027";
static const char data_1A94E[10] __attribute__((scn_string_literal)) = "E1016C005";
static const char data_1A958[10] __attribute__((scn_string_literal)) = "E1016B132";
static const char data_1A962[10] __attribute__((scn_string_literal)) = "E1016B133";
static const char data_1A96C[10] __attribute__((scn_string_literal)) = "E1016B130";
static const char data_1A976[10] __attribute__((scn_string_literal)) = "E1016B134";
static const char data_1A980[10] __attribute__((scn_string_literal)) = "E1016B117";
static const char data_1A98A[10] __attribute__((scn_string_literal)) = "E1016B027";
static const uint32_t data_1A994 = 8198;
static const int32_t data_1A998[7] = {0, 0, 0, 0, 131072, 0, 3};
static const uint32_t data_1A9B4[2] = {0x80020000U, 0};
static const int32_t data_1A9BC = 16386;
static const uint32_t data_1A9C0[2] = {0x20020000U, 0};
static const int32_t data_1A9C8 = 4098;
static const uint32_t data_1A9CC[2] = {0x8FE0000U, 0};
static const int32_t data_1A9D4 = 1152;
static const uint32_t data_1A9D8[2] = {0x2800000U, 0};
static const int32_t data_1A9E0 = 1152;
static const uint32_t data_1A9E4[2] = {0x8FE0000U, 0};
static const int32_t data_1A9EC = 4098;
static const uint32_t data_1A9F0[2] = {0x20020000U, 0};
static const int32_t data_1A9F8 = 16386;
static const uint32_t data_1A9FC[2] = {0x80020000U, 0};
static const int32_t data_1AA04[21] = {3, 131072, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
static const uint32_t data_1AA58 = 8198;
static const uint32_t data_1AA5C[15] = {
  0, 0, 0, 0, 0, 0, 0, 0xE0018000U, 0xC0007038U, 0x90242001U, 0x2001A000U, 0x90009024U, 0x20132001U, 0x3C0F8807U, 0x4042013U
};
static const float data_1AA98 = 2.125246f;
static const uint32_t data_1AA9C[2] = {0x4080204U, 0x4044008U};
static const float data_1AAA4 = 2.125246f;
static const uint32_t data_1AAA8[7] = {0x3C0F8807U, 0x90002013U, 0x20132001U, 0x2001A000U, 0xC0009024U, 0x90242001U, 0xE0018000U};
static const int32_t data_1AAC4[22] = {28728, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
static const uint32_t data_1AB1C = 8198;
static const uint8_t pad_1AB20[28] = {0};
static const uint32_t data_1AB3C = 0x70780004;
static const uint32_t data_1AB40 = 454883;
static const uint32_t data_1AB44 = 0x10224948;
static const uint32_t data_1AB48 = 0x49480005;
static const uint32_t data_1AB4C = 0x80040824;
static const uint32_t data_1AB50 = 0xC8442648;
static const uint32_t data_1AB54 = 0x26CF433C;
static const uint32_t data_1AB58 = 0x22202449;
static const uint32_t data_1AB5C = 0x24891001;
static const uint32_t data_1AB60 = 0x10011220;
static const uint32_t data_1AB64 = 0x2220E489;
static const uint32_t data_1AB68 = 0x4881001;
static const uint32_t data_1AB6C = 0x26CF433C;
static const uint32_t data_1AB70 = 0x80040448;
static const uint32_t data_1AB74 = 0xE4492648;
static const uint32_t data_1AB78 = 0x49480005;
static const uint32_t data_1AB7C = 402473;
static const uint32_t data_1AB80 = 0x24294948;
static const uint32_t data_1AB84 = 0x70780004;
static const uint32_t data_1AB88 = 15599;
static const uint8_t pad_1AB8C[82] = {0};
static const char data_1ABDE[16] __attribute__((scn_string_literal)) = "cause err : %d\012";
static const char data_1ABEE[46] __attribute__((scn_string_literal)) = "cause err : %d(%d) : 3tumeno dousaha naihazu\012";
static const char data_1AC1C[16] __attribute__((scn_string_literal)) = "cause err : %d\012";
static const char data_1AC2C[16] __attribute__((scn_string_literal)) = "cause err : %d\012";
static const char data_1AC3C[45] __attribute__((scn_string_literal)) = "cause err : %d(%d) : ３つめの動作はないはず\012";
static const char data_1AC69[23] __attribute__((scn_string_literal)) = "WorstMiss %d w_idx %d\012";
static const char data_1AC80[23] __attribute__((scn_string_literal)) = "POS 0.0 = %f, %f, %f.\012";
static const char data_1AC97[28] __attribute__((scn_string_literal)) = "POS 0.000001 = %f, %f, %f.\012";
static const char data_1ACB3[29] __attribute__((scn_string_literal)) = "POS 0.0000001 = %f, %f, %f.\012";
static const char data_1ACD0[30] __attribute__((scn_string_literal)) = "POS 0.00000001 = %f, %f, %f.\012";
static const char data_1ACEE[10] __attribute__((scn_string_literal)) = "seq : %d\012";
static const char data_1ACF8[13] __attribute__((scn_string_literal)) = "chgcam : %d\012";
static const char data_1AD05[27] __attribute__((scn_string_literal)) = "DIFF ANG OVER 0: %d %d %d\012";
static const char data_1AD20[27] __attribute__((scn_string_literal)) = "DIFF ANG OVER 1: %d %d %d\012";
static const char data_1AD3B[4] __attribute__((scn_string_literal)) = "1:\012";
static const char data_1AD3F[4] __attribute__((scn_string_literal)) = "2:\012";
static const char data_1AD43[4] __attribute__((scn_string_literal)) = "3:\012";
static const char data_1AD47[4] __attribute__((scn_string_literal)) = "4:\012";
static const char data_1AD4B[36] __attribute__((scn_string_literal)) = "Advice Start %d : Miss %d : Idx %d\012";
static const char data_1AD6F[253] = "WD Buffer Over : step1\012\000\000\000\000\000\000\000\000\000\0004\363\377\377\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000g\346\377\377\000\000\000\000\000\000\000\000\357\356\377\377\000\000\000\000\336\375\377\377~\362\377\377\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\253\352\377\377\000\000\000\000\000\000\000\000\365\351\377\377\000\000\000\000\000\000\000\000~\362\377\377\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\211\350\377\377\000\000\000\000\000\000\000\0009\356\377\377\000\000\000\000\"\002\000\000\014\366\377\377\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\323\347\377\377\000\000\000\000\274\373\377\377\352\363\377\377\000\000\000\000\000\000\000\000\014\366\377\377";
static const int32_t data_1AE6C[57] = {
  0, 0, -9648, 0, -1092, -2366, 0, 5825, -910, 0, 0, -8192, 0, 0, -8920, 0, 364, -12014, 0, 0, 0, 0, 0, -7645, 0, 0, -9466, 0, 0, -13289, 0, 0, 0, 0,
  0, -7827, 0, 0, -8738, 0, 0, -12196, 0, 0, 0, 0, 0, -5461, 0, 0, -10558, 0, -3640, -9284, 0, 0, 0
};
static const Vector3i data_1AF50 = {.x = -6189, .y = -4733, .z = 9830};
static const int32_t data_1AF5C[57] = {
  0, 0, -12379, 0, -3276, -7281, 0, 0, -2002, 0, 0, 0, 0, 0, 0, 0, -728, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 546, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1092, 0, 0, 0, 0
};
static const Vector3i data_1B040 = {.x = 0, .y = -728, .z = 0};
static const Vector3i data_1B04C = {.x = 0, .y = 0, .z = 0};
static const Vector3i data_1B058 = {.x = 0, .y = -4551, .z = 4004};
static const Vector3i data_1B064 = {.x = 0, .y = -1274, .z = 0};
static const Vector3i data_1B070 = {.x = 0, .y = -2912, .z = 0};
static const Vector3i data_1B07C = {.x = 0, .y = -3458, .z = 0};
static const Vector3i data_1B088 = {.x = 0, .y = -3458, .z = 0};
static const Vector3i data_1B094 = {.x = 0, .y = -2730, .z = 0};
static const Vector3i data_1B0A0 = {.x = 0, .y = -3094, .z = 0};
static const Vector3i data_1B0AC = {.x = 0, .y = -4187, .z = 0};
static const char data_1B0B8[19] __attribute__((scn_string_literal)) = "Author Start : %d\012";
static const char data_1B0CB[21] __attribute__((scn_string_literal)) = "Author End : %d %d\012";
static const int32_t data_1B0E0[57] = {
  0, 0, -5461, 0, -2366, -1092, 0, -182, 1092, 0, -546, -6735, 0, 0, -4004, 0, 0, -6553, 0, -182, 1274, 0, 0, -13289, 0, 0, -5279, 0, 0, -8009, 0, 0,
  1456, 0, 182, -8738, 0, 0, -6007, 0, 0, -8738, 0, 182, 1638, 0, -728, -5279, 0, -1274, -6735, 0, -910, -8738, 0, 0, 1638
};
static const int32_t data_1B1C4[57] = {
  0, 0, -7281, 0, 0, -4004, 0, 546, 182, 0, 0, -18204, 0, 0, -16930, 0, 0, -12379, 0, 0, 0, 0, 0, -12196, 0, 0, -18204, 0, 0, -13289, 0, 0, 0, 0, 0,
  -11832, 0, 0, -18386, 0, 0, -13835, 0, 0, 0, 0, 910, -12014, 0, -2184, -16748, 0, -2002, -13835, 0, 0, 364
};
static const int32_t data_1B2A8[57] = {
  0, 0, -1274, 0, 3276, 546, 0, -1274, 1274, 0, -546, -6735, 0, 0, -6553, 0, 2002, -546, 0, -182, 1274, 0, 0, -5643, 0, 0, -9102, 0, 0, -1092, 0, 0,
  1456, 0, 182, -6553, 0, 0, -8374, 0, -1820, -2730, 0, 0, 1638, 0, -728, -5279, 0, -1274, -3640, 0, -2548, -4004, 0, 0, 1638
};
static const int32_t data_1B38C[57] = {
  0, 0, -1274, 0, 3276, 546, 0, -1274, 1274, 0, -546, -6735, 0, 0, -6553, 0, 2002, -364, 0, -182, 1274, 0, 0, -5643, 0, 0, -9102, 0, 0, -1274, 0, 0,
  1456, 0, 182, -6553, 0, 0, -7827, 0, -1820, -2548, 0, 0, 1638, 0, -728, -5279, 0, -1274, -3640, 0, -2548, -3822, 0, 0, 1638
};
static const uint32_t data_1B470 = 16;
static const int32_t data_1B474[47] = {
  24, 14, 0, 0, -1, -1, -1, -1, -1, 1, 3, 256, 24, 14, 0, 0, -1, -1, -1, -1, -1, 5, 7, 256, 24, 14, 0, 0, -1, -1, -1, -1, -1, 9, 11, 512, 24, 14, 0,
  0, -1, -1, -1, -1, -1, 13, 15
};
static const char data_1B530[9] __attribute__((scn_string_literal)) = "オート０";
static const char data_1B539[7] __attribute__((scn_string_literal)) = "開始０";
static const char data_1B540[7] __attribute__((scn_string_literal)) = "成功０";
static const char data_1B547[7] __attribute__((scn_string_literal)) = "失敗０";
static const char data_1B54E[7] __attribute__((scn_string_literal)) = "死亡０";
static const char data_1B555[7] __attribute__((scn_string_literal)) = "開始１";
static const char data_1B55C[7] __attribute__((scn_string_literal)) = "成功１";
static const char data_1B563[7] __attribute__((scn_string_literal)) = "失敗１";
static const char data_1B56A[7] __attribute__((scn_string_literal)) = "開始２";
static const char data_1B571[7] __attribute__((scn_string_literal)) = "成功２";
static const char data_1B578[7] __attribute__((scn_string_literal)) = "失敗２";
static const char data_1B57F[7] __attribute__((scn_string_literal)) = "開始３";
static const char data_1B586[7] __attribute__((scn_string_literal)) = "成功３";
static const char data_1B58D[7] __attribute__((scn_string_literal)) = "失敗３";
static const char data_1B594[7] __attribute__((scn_string_literal)) = "開始４";
static const char data_1B59B[7] __attribute__((scn_string_literal)) = "成功４";
static const char data_1B5A2[7] __attribute__((scn_string_literal)) = "失敗４";
static const char data_1B5A9[] __attribute__((scn_string_literal)) = "E1062B003";
static const char data_1B5B3[] __attribute__((scn_string_literal)) = "E1062B009";
static const char data_1B5BD[] __attribute__((scn_string_literal)) = "E1062B003";
static const char data_1B5C7[] __attribute__((scn_string_literal)) = "E1062B005";
static const char data_1B5D1[] __attribute__((scn_string_literal)) = "E1062B008";
static const char data_1B5DB[] __attribute__((scn_string_literal)) = "E1062B005";
static const char data_1B5E5[11] __attribute__((scn_string_literal)) = "M_MC5Q.BIN";
static const char data_1B5F0[7] __attribute__((scn_string_literal)) = "A0184D";
static const char data_1B5F7[6] __attribute__((scn_string_literal)) = "E1062";
static const char data_1B5FD[12] __attribute__((scn_string_literal)) = "system1.snd";
static const char data_1B609[11] __attribute__((scn_string_literal)) = "qtebat.snd";
static const char data_1B614[11] __attribute__((scn_string_literal)) = "bgm067.snd";
static const char data_1B61F[7] __attribute__((scn_string_literal)) = "A0184D";
static const char data_1B626[6] __attribute__((scn_string_literal)) = "E1062";
static const Vector3 data_1B62C = {.x = 0.0f, .y = 0.0f, .z = 0.0f};
static const Vector3i data_1B638 = {.x = 0, .y = 0, .z = 0};
static const Vector3 data_1B644 = {.x = 0.0f, .y = 0.0f, .z = 0.0f};
static const Vector3i data_1B650 = {.x = 0, .y = 0, .z = 0};

static void script_0030(void) {
  int32_t local_FC;
  local_FC = Script_GetSceneID();
  switch (local_FC) {
    case 1:
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_ENTRY2_TIMED_EVENT_DONE) == 0) {
        Script_TimedTransitionUtils(TIMED_TRANSITION_REGISTER, 1, 0x3044484A, 2, &data_152F8, 0xD5A6C3, TimedTransitionUpdateMemoItem150);
      }
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JD00_ENTRY20_TIMED_EVENT_DONE) == 0) {
        Script_TimedTransitionUtils(TIMED_TRANSITION_REGISTER, 1, 0x3030444A, 20, &data_152C0, 0xDA3AA3, 0);
      }
      break;
    case 2:
      if ((Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_GMCT_ROUTE_STARTED) == 0) | (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_GMCT_ROUTE_ADVANCED) == 0)) {
        Script_TimedTransitionUtils(TIMED_TRANSITION_REGISTER, 2, 0x54434D47, 1, &data_15288, 0xB7420F, 0);
      }
      break;
    case 3:
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOZOMI_RESCUE_RUSH_ACTIVE) == 0) {
        Script_TimedTransitionUtils(TIMED_TRANSITION_REGISTER, 3, 0x4F4D4F4A, 20, &data_152A4, 0x2DC9FE, TimedTransitionRollbackToPreviousDay2330);
      }
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_FIRED_FROM_JOB) & ~Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 970)) {
        if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_MFSY_ENTRY42_TIMED_EVENT_DONE) == 0) {
          Script_TimedTransitionUtils(TIMED_TRANSITION_REGISTER, 3, 0x5953464D, 42, &data_152DC, 0xBBB817, 0);
        }
      }
      break;
  }
  return;
}

static void script_0168(int32_t arg0) {
  script_0180(arg0);
  return;
}

static void script_0174(int32_t arg0) {
  script_0180(arg0);
  return;
}

static void script_0180(int32_t arg0) {
  if (Script_StepUtils(GET_STEP_HIGHER, 0) == 22) {
    int32_t local_FC;
    local_FC = Script_StepUtils(GET_STEP_LOWER, 0);
    if ((local_FC == 2) | (local_FC == 3)) {
      Script_SetShouldSleepFlag(0);
      goto L0001B9;
    }
  }
  L0001B9:;
  Script_ryoSetChara(arg0);
  Script_facemotUtils(arg0, 1, 0);
  Script_facemotUtils(arg0, 0, 0);
  Script_FaceEx(arg0, 0, 0, 2);
  Script_CharHandlers(arg0, -1, 0, 0);
  script_0030();
  Script_cpctStateUtils(1, 2);
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

static void script_027C(int32_t arg0) {
  script_0174(arg0);
  for (;;) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_0293(int32_t arg0) {
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

static void script_02D4(int32_t arg0) {
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

static void script_0318(int32_t arg0) {
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

static void script_0396(void) {
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_PROGRESS_STEP_28_REACHED)) {
    script_0318(40);
  } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_PROGRESS_STEP_1E_REACHED)) {
    script_0318(30);
  } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_PROGRESS_STEP_14_REACHED)) {
    script_0318(20);
  } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JU00_CATA1_NBO_BRANCH_CHOSEN)) {
    script_0318(10);
  } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_PROGRESS_STEP05_SA1116_DOJO_DONE)) {
    script_0318(5);
  } else {
    script_0318(0);
  }
  return;
}

static void script_0415(int32_t arg0, float arg1, float arg2, float arg3) {
  Vector3 local_F4;
  local_F4.x = arg1;
  (&local_F4)->y = arg2;
  (&local_F4)->z = arg3;
  Script_EVT_SetPosition(arg0, POS_XYZ, &local_F4);
  return;
}

static void script_043D(int32_t arg0, float arg1, float arg2, float arg3) {
  Vector3i local_F4;
  local_F4.x = (int32_t)(arg1 / 360.0f * (float)0x10000);
  (&local_F4)->y = (int32_t)(arg2 / 360.0f * (float)0x10000);
  (&local_F4)->z = (int32_t)(arg3 / 360.0f * (float)0x10000);
  Script_EVT_SetRotation(arg0, 0x38000000, &local_F4, 0);
  return;
}

static void script_0491(int32_t arg0, int32_t arg1, int32_t arg2) {
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

static int32_t script_04E1(int32_t arg0) {
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

static void script_0588(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  local_FC = script_04E1(arg0);
  if (arg1 > local_FC) {
    (void)0;
  } else {
    (void)-1;
  }
  return;
}

static int32_t script_05AB(int32_t arg0) {
  int32_t unused_FC;
  GameDateTime local_F4;
  int32_t local_F0;
  int32_t local_EC;
  Script_ReturnTimeDate(&local_F4);
  local_F0 = (&local_F4)->hours * 60 + (&local_F4)->minutes;
  local_EC = Script_scenUtils(SCENARIO_CMD_GET_STEP_LUT, arg0) * 6;
  return local_F0 - local_EC;
}

static void script_05DE(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  local_FC = script_05AB(arg0);
  if (arg1 > local_FC) {
    (void)0;
  } else {
    (void)-1;
  }
  return;
}

static void script_0601(int32_t arg0, int32_t arg1) {
  int8_t local_F4[12];
  int32_t local_F0;
  local_F0 = script_05AB(arg0);
  local_F4[4] = local_F0 / 60;
  local_F4[5] = local_F0 % 60;
  if ((local_F4[4] < *(int8_t *)(arg1 + 4)) | ((local_F4[4] == *(int8_t *)(arg1 + 4)) & (local_F4[5] < *(int8_t *)(arg1 + 5)))) {
    (void)0;
  } else {
    (void)-1;
  }
  return;
}

static void script_0660(int32_t arg0) {
  int32_t local_F8[2];
  local_F8[0] = arg0;
  local_F8[1] = 0;
  (void)local_F8;
  return;
}

static void script_0675(int32_t arg0, int32_t arg1) {
  Vector3 local_F4;
  Vector3i local_E8;
  Script_ryoControl(PCCTL_SetCPCTFlag400, 0);
  Script_ccowUtils(arg1, 2);
  Script_motmToggleFlags(arg0, 2, 0x40);
  Script_RequestFightAnimation(arg0, 657, 0.0f, 14, 1);
  SCNF_WaitFrames_0000(1);
  local_F4.x = 0.08f;
  (&local_F4)->y = 0.0f;
  (&local_F4)->z = -0.025f;
  local_E8.x = 10467;
  (&local_E8)->y = 14563;
  (&local_E8)->z = 0;
  Script_fixoAttachEx(arg1, arg0, 18, &local_F4, &local_E8);
  Script_CharHandlers(arg0, 0, 0, 16);
  Script_CharHandlers(arg0, 1, 8, 16);
  Script_charaDisplay(arg1, CDM_Show);
  return;
}

static void script_0713(void) {
  Script_ryoControl(PCCTL_SetCPCTFlag400, 1);
  return;
}

static void script_071E(int32_t arg0, int32_t arg1) {
  Vector3 local_F4;
  Vector3i local_E8;
  if (Script_charaDisplay(arg1, CDM_Query)) {
    Script_fixoClearAttachment(arg1);
    Script_SetRyoMotionDetails(arg0);
    Script_CharHandlers(arg0, -1, 0, 0);
    Script_EVT_GetPosition(arg1, -1, &local_F4, 0);
    (&local_F4)->y = -0.3f;
    local_E8.x = 47136;
    (&local_E8)->y = 0;
    (&local_E8)->z = 0x8000;
    Script_EVT_SetPosition(arg1, POS_XYZ, &local_F4);
    Script_EVT_SetRotation(arg1, 0x38000000, &local_E8, 0);
  }
  return;
}

static void script_0798(void) {
  Script_ambSoundUtils(1, 1, &data_15330, 135.0f, 280.0f, 1.0f, 0, &data_153C0);
  Script_ambSoundUtils(2, 1, &data_1533C, 135.0f, 60.0f, 1.0f, 0, &data_15448);
  Script_ambSoundUtils(3, 0, &data_15348, 50.0f, 170.0f, 0.8f, 0, data_154D0);
  Script_ambSoundUtils(4, 0, &data_15354, 10.0f, 5.0f, 1.0f, 0, &data_15558);
  Script_ambSoundUtils(5, 0, data_15360, 8.0f, 5.0f, 1.0f, 0, &data_155E0);
  Script_ambSoundUtils(6, 1, &data_1536C, 170.0f, 100.0f, 1.0f, 2, data_15668);
  Script_ambSoundUtils(7, 1, &data_15378, 170.0f, 100.0f, 1.0f, 0, &data_156F0);
  Script_ambSoundUtils(8, 1, &data_15384, 40.0f, 70.0f, 0.8f, 0, &data_15778);
  Script_ambSoundUtils(9, 0, &data_15390, 3.0f, 60.0f, 0.8f, 0, data_15800);
  Script_ambSoundUtils(10, 0, &data_1539C, 60.0f, 35.0f, 0.7f, 0, &data_15888);
  Script_ambSoundUtils(13, 0, &data_153A8, 8.0f, 5.0f, 1.0f, 0, &data_15910);
  Script_ambSoundUtils(15, 1, data_153B4, 300.0f, 0.0f, 1.0f, 3, &data_15998);
  Script_Ambient1SoundUtils(0, 23976);
  return;
}

static void script_0952(void) {
  Script_ambSoundUtils(1, 1, &data_15A20, 135.0f, 280.0f, 1.0f, 0, &data_15A80);
  Script_ambSoundUtils(2, 1, &data_15A2C, 135.0f, 60.0f, 1.0f, 0, &data_15B08);
  Script_ambSoundUtils(3, 0, &data_15A38, 50.0f, 170.0f, 0.8f, 0, data_15B90);
  Script_ambSoundUtils(6, 1, &data_15A44, 170.0f, 100.0f, 1.0f, 2, data_15C18);
  Script_ambSoundUtils(7, 1, &data_15A50, 170.0f, 100.0f, 1.0f, 0, &data_15CA0);
  Script_ambSoundUtils(8, 1, &data_15A5C, 40.0f, 70.0f, 0.8f, 0, &data_15D28);
  Script_ambSoundUtils(10, 0, &data_15A68, 60.0f, 35.0f, 0.7f, 0, &data_15DB0);
  Script_ambSoundUtils(15, 1, data_15A74, 300.0f, 0.0f, 1.0f, 3, &data_15E38);
  Script_Ambient1SoundUtils(0, 23976);
  return;
}

static void script_0A7C(void) {
  Script_ambSoundUtils(1, 0, &data_15EC0, 1.0f, 10.0f, 1.0f, 0, &data_15ECC);
  Script_Ambient1SoundUtils(0, 23976);
  return;
}

static void script_0AAD(void) {
  Script_ambSoundUtils(1, 0, &data_15F54, 1.0f, 10.0f, 1.0f, 0, &data_15F60);
  Script_Ambient1SoundUtils(0, 23976);
  return;
}

static void script_0ADE(void) {
  Script_ambSoundUtils(1, 1, &data_15FE8, 135.0f, 280.0f, 1.0f, 0, &data_16048);
  Script_ambSoundUtils(2, 1, &data_15FF4, 135.0f, 60.0f, 1.0f, 0, &data_160D0);
  Script_ambSoundUtils(3, 0, &data_16000, 50.0f, 170.0f, 0.8f, 0, data_16158);
  Script_ambSoundUtils(6, 1, &data_1600C, 170.0f, 100.0f, 1.0f, 2, data_161E0);
  Script_ambSoundUtils(7, 1, &data_16018, 170.0f, 100.0f, 1.0f, 0, &data_16268);
  Script_ambSoundUtils(8, 1, &data_16024, 40.0f, 70.0f, 0.8f, 0, &data_162F0);
  Script_ambSoundUtils(15, 1, data_1603C, 300.0f, 0.0f, 1.0f, 3, &data_16378);
  Script_Ambient1SoundUtils(0, 23976);
  return;
}

static void script_0BE4(void) {
  script_0798();
  return;
}

static void script_0BEC(void) {
  Script_ambSoundUtils(1, 0, &data_16400, 10.0f, 30.0f, 0.8f, 2, &data_16478);
  Script_ambSoundUtils(2, 0, &data_1640C, 10.0f, 30.0f, 0.8f, 2, &data_16500);
  Script_ambSoundUtils(3, 0, &data_16418, 20.0f, 30.0f, 0.8f, 2, &data_16588);
  Script_ambSoundUtils(4, 0, &data_16424, 15.0f, 30.0f, 0.8f, 2, &data_16610);
  Script_ambSoundUtils(5, 0, &data_16430, 10.0f, 30.0f, 0.8f, 2, &data_16698);
  Script_ambSoundUtils(6, 0, &data_1643C, 15.0f, 30.0f, 0.8f, 2, &data_16720);
  Script_ambSoundUtils(7, 1, &data_16448, 50.0f, 30.0f, 0.8f, 0, data_167A8);
  Script_ambSoundUtils(8, 1, data_16454, 50.0f, 30.0f, 0.8f, 0, data_16830);
  Script_ambSoundUtils(9, 1, data_16460, 50.0f, 30.0f, 0.8f, 0, &data_168B8);
  Script_ambSoundUtils(15, 1, data_1646C, 200.0f, 50.0f, 1.0f, 3, &data_16940);
  Script_Ambient1SoundUtils(0, 23976);
  return;
}

static void script_0D61(void) {
  Script_shenSetLightingMode(1);
  Script_LightUtils(0, 0, 2);
  Script_lightSetSlotEnabled(0, 1);
  Script_GetSetWeatherSettings(WEATHER_SETTINGS_SET_DISABLE_WEATHER, 1);
  Script_WeatherUtils_0(WeatherDataUtil_ApplyCurrentWeatherOrEnableDateTimeRead, 0);
  Script_WeatherTaskUtil(InitTaskData);
  return;
}

static void script_0D95(void) {
  Script_WeatherUtils_ForFIXC(13, 0);
  Script_WeatherUtils_ForFIXC(14, 0);
  Script_WeatherUtils_0(WeatherDataUtil_SetInterpolatedWeatherOverride, 0, 0, 1, 0);
  Script_WeatherUtils_0(WeatherDataUtil_SetInterpolatedWeatherOverride, 1, 0, 1, 0);
  Script_WeatherUtils_0(WeatherDataUtil_SetInterpolatedWeatherOverride, 2, 0, 1, 0);
  Script_WeatherTaskUtil(ToggleFogTask, 0);
  Script_WeatherTaskUtil(ToggleCloudXYFlags, 0);
  Script_WeatherTaskUtil(ToggleSceneLightingFlags, 0);
  SCNF_WaitFrames_0000(3);
  return;
}

static void script_0DFA(void) {
  Script_shenSetLightingMode(1);
  Script_LightUtils(0, 0, 2);
  Script_lightSetSlotEnabled(0, 1);
  Script_WeatherUtils_0(WeatherDataUtil_SetInterpolatedWeatherOverride, 0, 1, 1, 0);
  Script_WeatherUtils_0(WeatherDataUtil_SetInterpolatedWeatherOverride, 1, 1, 1, 0);
  Script_WeatherUtils_0(WeatherDataUtil_SetInterpolatedWeatherOverride, 2, 1, 1, 0);
  Script_WeatherTaskUtil(ToggleFogTask, 1);
  Script_WeatherTaskUtil(ToggleCloudXYFlags, 1);
  Script_WeatherTaskUtil(ToggleSceneLightingFlags, 1);
  SCNF_WaitFrames_0000(3);
  Script_WeatherTaskUtil(LoadSPR);
  Script_WeatherUtils_ForFIXC(13, 1);
  Script_WeatherUtils_ForFIXC(14, 1);
  return;
}

static void script_0E7E(void) {
  Script_shenSetLightingMode(1);
  Script_LightUtils(0, 0, 2);
  Script_lightSetSlotEnabled(0, 1);
  Script_WeatherUtils_0(WeatherDataUtil_SetInterpolatedWeatherOverride, 0, 1, 1, 0);
  Script_WeatherUtils_0(WeatherDataUtil_SetInterpolatedWeatherOverride, 1, 1, 1, 0);
  Script_WeatherUtils_0(WeatherDataUtil_SetInterpolatedWeatherOverride, 2, 1, 1, 0);
  Script_WeatherTaskUtil(ToggleFogTask, 1);
  Script_WeatherTaskUtil(ToggleCloudXYFlags, 1);
  Script_WeatherTaskUtil(ToggleSceneLightingFlags, 1);
  SCNF_WaitFrames_0000(3);
  Script_WeatherTaskUtil(LoadSPR);
  Script_WeatherUtils_ForFIXC(13, 1);
  Script_WeatherUtils_ForFIXC(14, 1);
  Script_WeatherUtils_0(WeatherDataUtil_SetInterpolatedWeatherOverride, 1, 0, 1, 0);
  Script_WeatherUtils_0(WeatherDataUtil_SetInterpolatedWeatherOverride, 2, 0, 1, 0);
  return;
}

__attribute__((scn_string_bindings(1, data_169E6, 2, data_169F8, 3, data_169FA)))
static void script_0F20(int32_t arg0) {
  int32_t local_FC;
  Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT, arg0, 1);
  local_FC = 0;
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0) == 0) {
    local_FC = Script_CycleManagerUtils(CYCLE_CMD_GET_CYCLE_PTR_BY_CHARA_ID, arg0);
    while (local_FC == 0) {
      Script_sprintf("MEND: Character read failed.\012");
      SCNF_WaitFrames_0000(1);
      local_FC = Script_CycleManagerUtils(CYCLE_CMD_GET_CYCLE_PTR_BY_CHARA_ID, arg0);
    }
    Script_sprintf("MEND: Now loading");
    while (Script_CycleManagerUtils(CYCLE_CMD_GET_NEXT_TASK_POINTER, local_FC) == 0) {
      Script_sprintf(".");
      SCNF_WaitFrames_0000(1);
    }
    Script_sprintf("\012");
  }
  (void)local_FC;
  return;
}

static void script_0FB8(int32_t arg0, int32_t arg1) {
  if (arg1 != 0) {
    Script_CycleManagerUtils(CYCLE_CMD_LOAD_OR_RESET_RUNNING_PROGRAM, arg1);
  }
  Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT, arg0, 0);
  arg1 = 0;
  return;
}

__attribute__((scn_string_bindings(1, data_16A15, 2, data_16A27, 3, data_16A29)))
static void script_0FDE(const char *arg0, const char *arg1) {
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 10, -1);
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, arg0, arg1) == 0) {
    Script_sprintf("MEND: Pack read failed.\012");
    SCNF_WaitFrames_0000(1);
  }
  Script_sprintf("MEND: Now loading");
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2)) {
    Script_sprintf(".");
    SCNF_WaitFrames_0000(1);
  }
  Script_sprintf("\012");
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 10, 0);
  return;
}

__attribute__((scn_string_bindings(0, data_16A2B)))
static void script_1052(int32_t arg0) {
  int8_t local_FF;
  local_FF = 6;
  for (;;) {
    Script_LoadSND(0, local_FF, "FREE", 0);
    if (local_FF <= arg0) {
      break;
    }
    local_FF = local_FF - 1;
  }
  return;
}

static void script_1083(void) {
  if (Script_WeatherUtils_0(WeatherDataUtil_QueryCurrentFCASFlag, 3)) {
    Script_mapmSetRenderFlag(2, 0);
    Script_mapmSetRenderFlag(3, 1);
    Script_mapmSetRenderFlag(7, 1);
  } else {
    Script_mapmSetRenderFlag(2, 1);
    Script_mapmSetRenderFlag(3, 0);
    Script_mapmSetRenderFlag(7, 0);
  }
  return;
}

static void script_10CB(void) {
  if (Script_WeatherUtils_0(WeatherDataUtil_QueryCurrentFCASFlag, 3)) {
    Script_mapmSetRenderFlag(9, 0);
    Script_mapmSetRenderFlag(10, 1);
  } else {
    Script_mapmSetRenderFlag(9, 1);
    Script_mapmSetRenderFlag(10, 0);
  }
  Script_mapmSetRenderFlag(0, 0);
  Script_mapmSetRenderFlag(1, 0);
  Script_mapmSetRenderFlag(2, 0);
  Script_mapmSetRenderFlag(3, 0);
  Script_mapmSetRenderFlag(4, 0);
  Script_mapmSetRenderFlag(5, 0);
  Script_mapmSetRenderFlag(6, 0);
  Script_mapmSetRenderFlag(7, 0);
  Script_mapmSetRenderFlag(8, 0);
  return;
}

static void script_1152(void) {
  script_121A();
  Script_mapmSetRenderFlag(0, 1);
  Script_mapmSetRenderFlag(1, 0);
  Script_mapmSetRenderFlag(4, 1);
  Script_mapmSetRenderFlag(5, 1);
  Script_mapmSetRenderFlag(6, 1);
  Script_mapmSetRenderFlag(8, 1);
  return;
}

static void script_1190(void) {
  if (Script_WeatherUtils_0(WeatherDataUtil_QueryCurrentFCASFlag, 3)) {
    Script_mapmSetRenderFlag(2, 0);
    Script_mapmSetRenderFlag(3, 1);
    Script_mapmSetRenderFlag(6, 1);
  } else {
    Script_mapmSetRenderFlag(2, 1);
    Script_mapmSetRenderFlag(3, 0);
    Script_mapmSetRenderFlag(6, 0);
  }
  Script_mapmSetRenderFlag(0, 1);
  Script_mapmSetRenderFlag(1, 1);
  Script_mapmSetRenderFlag(4, 1);
  Script_mapmSetRenderFlag(5, 1);
  Script_mapmSetRenderFlag(6, 0);
  Script_mapmSetRenderFlag(7, 1);
  return;
}

static void script_120E(void) {
  Script_mapmSetRenderFlag(0, 1);
  return;
}

static void script_121A(void) {
  return;
}

static void script_121D(void) {
  Script_ryoControl(PCCTL_DisablePlayerControl);
  return;
}

static void script_1226(void) {
  Script_ryoControl(RYO_RestoreControl);
  Script_SetGameStateTo(AUTH_None);
  return;
}

static void script_1235(int32_t arg0) {
  Script_ccowUtils(arg0, 2);
  if (0) {
    Script_charaDisplay(arg0, CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(arg0, CDM_Show);
      goto L001261;
    }
  }
  L001261:;
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

static void script_12C2(int32_t arg0) {
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

static int32_t script_1312(void) {
  Script_filmCheckOrSetTransitionUtil(80, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_1340(int32_t arg0) {
  Script_filmCheckOrSetTransitionUtil(arg0, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static int32_t script_136E(void) {
  Script_filmCheckOrSetTransitionUtil(80, 255, 255, 255, 255, 0, 255, 255, 255);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_13A2(int32_t arg0) {
  Script_filmCheckOrSetTransitionUtil(arg0, 255, 255, 255, 255, 0, 255, 255, 255);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_13D6(int32_t arg0) {
  Script_filmCheckOrSetTransitionUtil(arg0, 255, 255, 255, 255, 150, 68, 50, 0);
  return;
}

static void script_13F4(int32_t arg0, int32_t arg1, int32_t arg2) {
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

static void script_142E(int32_t arg0, int32_t arg1) {
  while (arg1 > 0) {
    Script_SetFightProgramStateBytes(arg0, arg1, 1);
    arg1 = arg1 - 1;
    SCNF_WaitFrames_0000(1);
  }
  Script_GetDataFromFightProgram(arg0, 0);
  return;
}

static void script_1462(void) {
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

static void script_155A(int32_t arg0, int32_t arg1) {
  Script_FACE_LCG(arg0, 1, arg1, 0);
  SCNF_WaitFrames_0000(arg1 + 1);
  Script_FACE_LCG(arg0, 2, arg1, 0);
  SCNF_WaitFrames_0000(arg1);
  return;
}

__attribute__((scn_string_bindings(0, data_17A44, 1, data_17A51, 2, data_17A5A, 3, data_17A67)))
static int32_t script_158A(void) {
  int32_t local_FC;
  local_FC = Script_MT5Loader_TRWV_TRWAVE(0, &data_17A38, "model/object", "SEA3.PVR", "model/object", "TRSEA4.MT6", 4.0f, 0);
  script_15C5(local_FC);
  return local_FC;
}

__attribute__((scn_string_bindings(0, data_17A72, 1, data_17A7F)))
static void script_15C5(int32_t arg0) {
  Script_MT5Loader_TRWV_TRWAVE(21, arg0, 8, 8);
  Script_MT5Loader_TRWV_TRWAVE(16, arg0, -1, 0);
  Script_MT5Loader_TRWV_TRWAVE(13, arg0, 0.0901960805f, 0.196078435f);
  Script_MT5Loader_TRWV_TRWAVE(15, arg0, "model/object", "TRWTBL1.BIN");
  Script_MT5Loader_TRWV_TRWAVE(12, arg0, 0x4000);
  Script_MT5Loader_TRWV_TRWAVE(10, arg0, 64.0f, 64.0f, 1, 1, 3, 1);
  Script_MT5Loader_TRWV_TRWAVE(24, arg0, 0, 203, 255, 143);
  Script_MT5Loader_TRWV_TRWAVE(11, arg0, -0.51f, -0.68f, 0.52f);
  return;
}

static void script_165B(int32_t arg0) {
  (void)((task_80 = -1) & 8);
  (void)((task_84 = -1) & 8);
  for (;;) {
    if (~task_84) {
      SCNF_WaitFrames_0000(1);
      continue;
    }
    if (Script_SetTaskDataValue(CHARA_AKIR, &data_17A8C, &data_17A98, 0) == 0) {
      if (~task_80) {
        Script_MT5Loader_TRWV_TRWAVE(1, arg0);
        (void)((task_80 = -1) & 8);
      }
    } else {
      if (task_80 & 4) {
        Script_MT5Loader_TRWV_TRWAVE(2, arg0);
        (void)((task_80 = 0) & 8);
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_16E8(int32_t arg0) {
  int32_t local_FC;
  int32_t unused_F8;
  int32_t local_F4;
  int32_t unused_D0[9];
  Vector3i local_C4;
  int32_t local_C0;
  script_293B();
  if (arg0) {
    Script_scenCreateTask(script_1312, 0);
  }
  Script_shenSeqUtils(0);
  local_F4 = 0;
  local_FC = 0;
  while (Script_shenSeqUtils(-1)) {
    local_C0 = local_FC;
    if (local_C0 == 1) {
      Script_SetMaxInterpSpeed(0.0f);
      Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
      Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
      Script_scenToggleActiveFor(CHARA_CHAI, 0);
      Script_ResetHandMotion(CHARA_TAIJ, 1, 0, 0);
      Script_ResetHandMotion(CHARA_TAIJ, 1, 1, 0);
      Script_handSetFloatsFor(CHARA_TAIJ, 1, data_17C78, 1);
      Script_handSetFloatsFor(CHARA_TAIJ, 0, data_17C78, 1);
      Script_charaSetClipAngle(SHEN_NAME('TKOT'), 10.0f);
      Script_SetHandRootRotation(CHARA_TAIJ, 1, 0x15, &data_18164);
      goto L0017C5;
    }
    L0017C5:;
    switch (local_FC) {
      case 181:
        Script_scenToggleActiveFor(CHARA_CHAI, 1);
        break;
      case 207:
        Script_scenCreateTask(script_32DF, 0);
        Script_handSetFloatsFor(CHARA_AKIR, 1, data_182FC, 0);
        break;
      case 277:
        Script_FaceEx(CHARA_KISY, 1, 0, 1);
        Script_FACE(CHARA_KISY, 0, 0, 1);
        break;
      case 278:
        Script_FACE(CHARA_KISY, 4, 0, 10);
        break;
      case 308:
        Script_FACE(CHARA_KISY, 4, 0, 10);
        break;
      case 356:
        Script_FaceEx(CHARA_KISY, 0, 0, 1);
        break;
      case 416:
        Script_scenCreateTask(script_3367, 0);
        break;
      case 628:
        Script_handSetFloatsFor(CHARA_AKIR, 0, data_17AB0, 20);
        break;
    }
    switch (local_FC) {
      case 141:
        Script_FaceEx(CHARA_KISY, 1, 0, 1);
        break;
      case 161:
        Script_FACE(CHARA_KISY, 7, 1, 10);
        break;
    }
    if (local_FC == 61) {
      Script_executeAicaCommand(3496, 0, 0);
      Script_executeAicaCommand(1184, 2, 98);
      local_F4 = 1;
    }
    if (local_FC == 216) {
      Script_handSetFloatsFor(CHARA_AKIR, 1, data_182FC, 0);
      local_C4.x = 0;
      (&local_C4)->y = 0;
      (&local_C4)->z = 0;
      Script_SetHandRootRotation(CHARA_AKIR, 1, 0x15, &local_C4);
    }
    switch (local_FC) {
      case 1:
        Script_mapmSetCamFlags(0, 1);
        break;
      case 381:
        Script_mapmSetCamFlags(0, 1);
        break;
    }
    switch (local_FC) {
      case 105:
        Script_SetFightProgramStateBytes(CHARA_CHAI, 2, 0);
        Script_GetDataFromFightProgram(CHARA_CHAI, 1);
        break;
      case 106:
        Script_SetFightProgramStateBytes(CHARA_CHAI, 3, 1);
        Script_GetDataFromFightProgram(CHARA_CHAI, 1);
        break;
      case 391:
        Script_GetDataFromFightProgram(CHARA_CHAI, 0);
        break;
    }
    if (local_FC == 1) {
      Script_FaceEx(CHARA_AKIR, 0, 0, 1);
      Script_FaceEx(CHARA_TAIJ, 0, 0, 1);
      Script_FaceEx(CHARA_KISY, 0, 0, 1);
    }
    if (local_FC == 129) {
      Script_handSetFloatsFor(CHARA_AKIR, 0, data_182FC, 0);
      Script_handSetFloatsFor(CHARA_AKIR, 1, data_18218, 0);
      local_C4.x = 3457;
      (&local_C4)->y = -3093;
      (&local_C4)->z = -10372;
      Script_SetHandRootRotation(CHARA_AKIR, 1, 0x15, &local_C4);
    }
    if (local_FC == 551) {
      Script_ResetHandMotion(CHARA_AKIR, 1, 0, 0);
    }
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  script_2B03();
  Script_SetMaxInterpSpeed(0.5f);
  if (local_F4 == 0) {
    Script_executeAicaCommand(3496, 0, 0);
    Script_executeAicaCommand(1184, 2, 98);
  }
  return;
}

static void script_1ADF(int32_t arg0) {
  int32_t local_FC;
  int32_t unused_F8;
  Vector3 local_EC;
  Vector3 local_E0;
  Vector3i local_D4;
  int32_t local_C8[3];
  Vector4 local_B8;
  int32_t local_B4;
  local_B8.x = 0.2f;
  (&local_B8)->y = 0.2f;
  (&local_B8)->z = 0.2f;
  (&local_B8)->w = 0.2f;
  script_2B6B();
  Script_shenSeqUtils(4);
  local_FC = 0;
  while (Script_shenSeqUtils(-1)) {
    local_B4 = local_FC;
    switch (local_B4) {
      case 0:
        Script_charaDisplay(CHARA_AKIR, CDM_Hide);
        Script_charaDisplay(CHARA_TAIJ, CDM_Hide);
        Script_charaDisplay(CHARA_KISY, CDM_Hide);
        break;
      case 17:
        if (-1) {
          Script_charaDisplay(CHARA_CHAI, CDM_Hide);
        } else {
          if (0) {
            Script_charaDisplay(CHARA_CHAI, CDM_Show);
            goto L001B8C;
          }
        }
        L001B8C:;
        Script_scenToggleVisibleFor(CHARA_CHAI, 0);
        break;
      case 39:
        Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 60.0f);
        Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 50.0f);
        Script_charaDisplay(CHARA_AKIR, CDM_Show);
        Script_MT5Loader_TRWV_TRWAVE(2, task_88);
        break;
      case 121:
        Script_charaDisplay(CHARA_TAIJ, CDM_Show);
        Script_charaDisplay(CHARA_KISY, CDM_Show);
        Script_SetMotnFlag(CHARA_TAIJ, 0);
        Script_MT5Loader_TRWV_TRWAVE(1, task_88);
        break;
      case 900:
        Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 20.0f);
        Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 50.0f);
        break;
      case 1500:
        Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 60.0f);
        Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 50.0f);
        break;
      case 2000:
        Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 160.0f);
        Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 70.0f);
        Script_LODUtils(LOD_UTILS_SET_NPC_DISPLAY_RADIUS, 160.0f);
        break;
      case 1:
        Script_ReadModelFormat7(CHARA_TAIJ, 0, data_181C8, 10);
        Script_ReadModelFormat7(CHARA_TAIJ, 1, data_181F0, 10);
        Script_SetCharaRefBoardPos(CHARA_AKIR, &local_B8);
        Script_SetCharaRefBoardPos(CHARA_KISY, &local_B8);
        Script_SetCharaRefBoardPos(CHARA_TAIJ, &local_B8);
        Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 4.0f);
        Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 4.0f);
        Script_mapmSetCamFlags(0, 0);
        Script_FaceEx(CHARA_AKIR, 1, 0, 1);
        Script_FaceEx(CHARA_TAIJ, 0, 0, 1);
        Script_FaceEx(CHARA_KISY, 0, 0, 1);
        Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
        Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
        Script_ResetHandMotion(CHARA_KISY, 0, 0, 0);
        Script_ResetHandMotion(CHARA_KISY, 0, 1, 0);
        Script_ResetHandMotion(CHARA_TAIJ, 0, 0, 0);
        Script_ResetHandMotion(CHARA_TAIJ, 0, 1, 0);
        Script_handSetFloatsFor(CHARA_AKIR, 1, data_17AB0, 0);
        Script_handSetFloatsFor(CHARA_AKIR, 0, data_17AB0, 0);
        Script_handSetFloatsFor(CHARA_KISY, 1, data_182FC, 0);
        Script_handSetFloatsFor(CHARA_KISY, 0, data_182FC, 0);
        Script_handSetFloatsFor(CHARA_TAIJ, 1, data_17D5C, 0);
        Script_handSetFloatsFor(CHARA_TAIJ, 0, data_17C78, 0);
        Script_SetHandRootRotation(CHARA_AKIR, 1, 0x15, &data_18194);
        Script_clmdSetFlags(CHARA_CHAI, 2, 1);
        Script_ccowSetFlag80(CHARA_CHAI, 0);
        Script_charaDisplay(SHEN_NAME('BAG0'), CDM_Show);
        Script_FACE_LCG(CHARA_AKIR, 2, 1, 0);
        local_E0.x = 73.186401f;
        (&local_E0)->y = 0.9298f;
        (&local_E0)->z = 14.9317f;
        Script_FaceLookAtTargetUtils(CHARA_AKIR, 2, &local_E0, 1);
        Script_FACE(CHARA_AKIR, 2, 0, 1);
        break;
    }
    if (local_FC == 30) {
      Script_FACE_LCG(CHARA_AKIR, 0, 6, 0);
    }
    if (local_FC == 110) {
      Script_FACE(CHARA_AKIR, 0, 0, 20);
      Script_FaceLookAtTargetUtils(CHARA_AKIR, 0);
    }
    if (!(local_FC == 131)) goto L001EBB;
    L001EBB:;
    if (local_FC == 132) {
      Script_MT5Loader_TRWV_TRWAVE(10, task_88, 64.0f, 64.0f, 3, 1, 1, 1);
    }
    if (!(local_FC == 2178)) goto L001EEB;
    L001EEB:;
    switch (local_FC) {
      case 46:
        break;
      case 45:
        Script_handSetFloatsFor(CHARA_AKIR, 1, data_182FC, 0);
        Script_handSetFloatsFor(CHARA_AKIR, 0, data_182FC, 0);
        break;
      case 118:
        Script_FaceEx(CHARA_TAIJ, 0, 0, 1);
        Script_FaceEx(CHARA_KISY, 1, 0, 1);
        Script_FACE(CHARA_AKIR, 2, 0, 15);
        Script_FACE(CHARA_KISY, 4, 0, 15);
        Script_ResetHandMotion(CHARA_AKIR, 1, 0, 0);
        Script_ResetHandMotion(CHARA_AKIR, 1, 1, 0);
        Script_handSetFloatsFor(CHARA_AKIR, 1, data_182FC, 0);
        Script_handSetFloatsFor(CHARA_AKIR, 0, data_182FC, 0);
        break;
    }
    if (local_FC == 583) {
      Script_FaceEx(CHARA_TAIJ, 1, 0, 1);
      Script_SetHandRootRotation(CHARA_TAIJ, 0, 0x15, &data_18104);
      Script_SetMotnFlag(CHARA_TAIJ, 0);
    }
    if (local_FC == 708) {
      Script_SetHandRootRotation(CHARA_AKIR, 1, 0x15, &data_18188);
    }
    if (local_FC == 808) {
      Script_FaceEx(CHARA_KISY, 0, 0, 1);
      Script_FaceEx(CHARA_TAIJ, 1, 0, 1);
      Script_SetHandRootRotation(CHARA_TAIJ, 1, 0x15, &data_180EC);
      Script_SetHandRootRotation(CHARA_TAIJ, 0, 0x15, &data_180F8);
      Script_ResetHandMotion(CHARA_TAIJ, 1, 0, 0);
      Script_ResetHandMotion(CHARA_TAIJ, 1, 1, 0);
      Script_handSetFloatsFor(CHARA_TAIJ, 1, data_17D5C, 0);
      Script_handSetFloatsFor(CHARA_TAIJ, 0, data_17C78, 0);
    }
    if (local_FC == 878) {
      Script_charaDisplay(SHEN_NAME('FUTO'), CDM_Show);
      Script_EVT_GetPosition(CHARA_TAIJ, 18, &local_EC, 0);
      local_D4.x = 0;
      (&local_D4)->y = 0;
      (&local_D4)->z = 0;
      Script_EVT_SetPosition(SHEN_NAME('FUTO'), POS_XYZ, &local_EC);
      Script_EVT_SetRotation(SHEN_NAME('FUTO'), 0x38000000, &local_D4, 0);
      local_EC.x = 0.204f;
      (&local_EC)->y = 0.033f;
      (&local_EC)->z = -0.03f;
      local_D4.x = 180;
      (&local_D4)->y = 24038;
      (&local_D4)->z = 17471;
      Script_fixoAttachEx(SHEN_NAME('FUTO'), CHARA_TAIJ, 18, &local_EC, &local_D4);
      Script_SetHandRootRotation(CHARA_TAIJ, 1, 0x15, &data_1811C);
      Script_SetHandRootRotation(CHARA_TAIJ, 0, 0x15, &data_18128);
    }
    switch (local_FC) {
      case 1188:
        Script_handSetFloatsFor(CHARA_AKIR, 0, data_17AB0, 40);
        break;
      case 838:
        Script_handSetFloatsFor(CHARA_TAIJ, 1, data_17F24, 40);
        break;
      case 1198:
        Script_handSetFloatsFor(CHARA_AKIR, 1, data_17F24, 90);
        break;
      case 1228:
        Script_handSetFloatsFor(CHARA_TAIJ, 1, data_17C78, 40);
        Script_EVT_GetPosition(CHARA_AKIR, 18, &local_EC, 0);
        local_D4.x = 0;
        (&local_D4)->y = 0;
        (&local_D4)->z = 0;
        Script_EVT_SetPosition(SHEN_NAME('FUTO'), POS_XYZ, &local_EC);
        Script_EVT_SetRotation(SHEN_NAME('FUTO'), 0x38000000, &local_D4, 0);
        local_EC.x = 0.189f;
        (&local_EC)->y = 0.078f;
        (&local_EC)->z = -0.02f;
        local_D4.x = 40777;
        (&local_D4)->y = 50427;
        (&local_D4)->z = 9920;
        Script_fixoAttachEx(SHEN_NAME('FUTO'), CHARA_AKIR, 18, &local_EC, &local_D4);
        Script_FACE(CHARA_AKIR, 0, 0, 1);
        break;
      case 1758:
        Script_fixoClearAttachment(SHEN_NAME('FUTO'));
        local_EC.x = 0.771f;
        (&local_EC)->y = 0.0f;
        (&local_EC)->z = -0.043f;
        Script_EVT_SetPosition(SHEN_NAME('FUTO'), POS_XYZ, &local_EC);
        break;
      case 978:
        Script_ResetHandMotion(CHARA_AKIR, 1, 0, 0);
        Script_ResetHandMotion(CHARA_AKIR, 1, 1, 0);
        Script_handSetFloatsFor(CHARA_AKIR, 1, data_17C78, 1);
        Script_handSetFloatsFor(CHARA_AKIR, 0, data_17D5C, 0);
        Script_SetHandRootRotation(CHARA_AKIR, 1, 0x15, &data_18194);
        local_C8[0] = 0;
        local_C8[1] = 0;
        local_C8[2] = 2547;
        break;
      case 1448:
        Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 50.0f);
        Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 50.0f);
        break;
      case 1771:
        Script_handSetFloatsFor(CHARA_AKIR, 1, data_17AB0, 40);
        Script_handSetFloatsFor(CHARA_AKIR, 0, data_17AB0, 40);
        Script_ReadModelFormat7(CHARA_TAIJ, 0, data_181A0, 10);
        break;
      case 1898:
        Script_mapmSetRenderFlag(10, 0);
        Script_handSetFloatsFor(CHARA_AKIR, 1, data_17C78, 40);
        break;
      case 2068:
        Script_handSetFloatsFor(CHARA_AKIR, 1, data_17AB0, 40);
        break;
      case 1778:
        Script_handSetFloatsFor(CHARA_TAIJ, 0, data_18008, 40);
        Script_mapmSetRenderFlag(10, 1);
        break;
      case 1857:
        Script_FaceEx(CHARA_AKIR, 0, 0, 1);
        Script_FaceEx(CHARA_TAIJ, 0, 0, 1);
        break;
      case 1647:
        Script_InitializeOsag(CHARA_KISY);
        Script_SetHandRootRotation(CHARA_TAIJ, 1, 0x15, &data_18140);
        break;
      case 2272:
        Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
        Script_CharHandlers(CHARA_AKIR, 0, 0, 16);
        break;
      case 2151:
        Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
        Script_CharHandlers(CHARA_AKIR, 1, 0, 16);
        break;
    }
    if (local_FC == 1300) {
      Script_SetHandRootRotation(CHARA_TAIJ, 1, 0x15, &data_18170);
    }
    switch (local_FC) {
      case 1:
        Script_scenCreateTask(script_2E57, 0);
        break;
      case 188:
        break;
    }
    switch (local_FC) {
      case 1878:
        Script_FACE_LCG(CHARA_AKIR, 1, 6, 0);
        break;
      case 1978:
        Script_FACE_LCG(CHARA_AKIR, 0, 6, 0);
        break;
    }
    if (local_FC == 121) {
      Script_executeAicaCommand(12200, 0, 0);
      Script_executeAicaCommand(1184, 3, 104);
    }
    switch (local_FC) {
      case 1:
        Script_SetFightProgramStateBytes(CHARA_AKIR, 2, 1);
        Script_GetDataFromFightProgram(CHARA_AKIR, 6);
        break;
      case 33:
        Script_SetFightProgramStateBytes(CHARA_AKIR, 2, 0);
        Script_GetDataFromFightProgram(CHARA_AKIR, 6);
        break;
      case 42:
        Script_GetDataFromFightProgram(CHARA_AKIR, 0);
        break;
    }
    switch (local_FC) {
      case 0:
        Script_SetFightProgramStateBytes(CHARA_CHAI, 2, 0);
        Script_GetDataFromFightProgram(CHARA_CHAI, 1);
        break;
      case 16:
        Script_GetDataFromFightProgram(CHARA_CHAI, 0);
        break;
    }
    if (local_FC == 2578) {
      break;
    }
    if (local_FC == 200) {
      Script_clmdSetFlags(CHARA_TAIJ, 4, 1);
    }
    if (local_FC == 600) {
      Script_clmdSetFlags(CHARA_TAIJ, 4, 0);
    }
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  Script_executeAicaCommand(2720, 3, 40);
  Script_filmCheckOrSetTransitionUtil(40, 0, 0, 0, 0, 255, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  script_2CF8();
  Script_SetMaxInterpSpeed(0.5f);
  return;
}

static void script_26B2(int32_t arg0) {
  int32_t local_FC;
  int32_t unused_F8;
  Vector3 local_EC;
  Vector3i local_E0;
  int32_t local_DC;
  script_2D60();
  Script_filmCheckOrSetTransitionUtil(0, 0, 0, 0, 0, 255, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_shenSeqUtils(2);
  local_FC = 0;
  while (Script_shenSeqUtils(-1)) {
    local_DC = local_FC;
    if (local_DC == 0) {
      Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
      while (Script_filmCheckOrSetTransitionUtil(-1)) {
        SCNF_WaitFrames_0000(1);
      }
      Script_executeAicaCommand(18344, 0, 0);
      Script_executeAicaCommand(1184, 2, 96);
      Script_SetMaxInterpSpeed(0.0f);
      Script_FaceEx(CHARA_AKIR, 1, 0, 1);
      Script_ResetHandMotion(CHARA_AKIR, 1, 0, 0);
      Script_ResetHandMotion(CHARA_AKIR, 1, 1, 0);
      Script_handSetFloatsFor(CHARA_AKIR, 0, data_174E0, 1);
      Script_handSetFloatsFor(CHARA_AKIR, 1, data_174E0, 1);
      Script_FACE_LCG(CHARA_AKIR, 2, 1, 0);
      Script_FACE(CHARA_AKIR, 0, 0, 10);
      script_33FE(CHARA_AKIR, SHEN_NAME('BAG0'));
      goto L0027D7;
    }
    L0027D7:;
    if (local_FC == 1) {
      Script_ResetHandMotion(CHARA_AKIR, 1, 1, 0);
      Script_SetHandRootRotation(CHARA_AKIR, 1, 0x15, &data_183E0);
      Script_SetHandRootRotation(CHARA_AKIR, 0, 0x15, &data_183EC);
      Script_handSetFloatsFor(CHARA_AKIR, 1, data_18218, 0);
      Script_handSetFloatsFor(CHARA_AKIR, 0, data_182FC, 0);
      local_EC.x = -0.216f;
      (&local_EC)->y = 0.339f;
      (&local_EC)->z = -0.072f;
      local_E0.x = 3183;
      (&local_E0)->y = 53887;
      (&local_E0)->z = 48590;
      Script_fixoAttachEx(SHEN_NAME('BAG0'), CHARA_AKIR, 18, &local_EC, &local_E0);
      Script_charaDisplay(SHEN_NAME('BAG0'), CDM_Show);
    }
    if (local_FC == 130) {
      Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
    }
    if (local_FC == 380) {
      Script_FACE_LCG(CHARA_AKIR, 1, 15, 0);
    }
    if (local_FC == 400) {
      break;
    }
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  Script_executeAicaCommand(2720, 2, 40);
  Script_filmCheckOrSetTransitionUtil(40, 0, 255, 255, 255, 255, 255, 255, 255);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  script_2DF0();
  Script_SetMaxInterpSpeed(0.5f);
  return;
}

static void script_293B(void) {
  Vector4 local_F0;
  local_F0.x = 0.4f;
  (&local_F0)->y = 0.4f;
  (&local_F0)->z = 0.4f;
  (&local_F0)->w = 0.4f;
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 90.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 90.0f);
  script_1235(CHARA_AKIR);
  Script_facemotUtils(CHARA_AKIR, 1, 0);
  script_1235(CHARA_TAIJ);
  Script_facemotUtils(CHARA_TAIJ, 1, 0);
  script_1235(CHARA_KISY);
  Script_facemotUtils(CHARA_KISY, 1, 0);
  script_1235(CHARA_CHAI);
  Script_charaSetRefboardFlagTo(CHARA_TAIJ, 1);
  Script_SetCharaRefBoardPos(CHARA_TAIJ, &local_F0);
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 1);
  Script_SetCharaRefBoardPos(CHARA_AKIR, &local_F0);
  Script_charaSetRefboardFlagTo(CHARA_CHAI, 1);
  Script_SetCharaRefBoardPos(CHARA_CHAI, &local_F0);
  Script_charaSetRefboardFlagTo(CHARA_KISY, 1);
  Script_SetCharaRefBoardPos(CHARA_KISY, &local_F0);
  Script_SetCharaFlags(CHARA_AKIR, 4);
  Script_SetCharaFlags(CHARA_TAIJ, 4);
  Script_SetCharaFlags(CHARA_CHAI, 4);
  Script_SetCharaFlags(CHARA_KISY, 4);
  Script_SetMotnFlag(CHARA_AKIR, -1);
  Script_SetMotnFlag(CHARA_KISY, -1);
  Script_SetMotnFlag(CHARA_TAIJ, -1);
  Script_SetMotnFlag(CHARA_CHAI, -1);
  Script_clmdSetFlags(CHARA_AKIR, 2, 0);
  Script_clmdSetFlags(CHARA_KISY, 2, 0);
  Script_clmdSetFlags(CHARA_TAIJ, 2, 0);
  task_9C = Script_EffectUtils(0, &data_18430, &data_1843C, 20, 0.052941177f, 0, 0);
  task_98 = Script_EffectUtils(0, &data_18418, &data_18424, 60, 0.064f, 0, 0);
  return;
}

static void script_2B03(void) {
  int32_t unused_F4[3];
  script_12C2(CHARA_AKIR);
  Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
  Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
  Script_FaceEx(CHARA_AKIR, 0, 0, 1);
  script_12C2(CHARA_CHAI);
  script_12C2(CHARA_TAIJ);
  script_12C2(CHARA_KISY);
  Script_SetGameStateTo(AUTH_None);
  return;
}

static void script_2B6B(void) {
  int32_t unused_C4[15];
  Vector4 local_B4;
  local_B4.x = 1.0f;
  (&local_B4)->y = 0.4f;
  (&local_B4)->z = 0.4f;
  (&local_B4)->w = 0.4f;
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 90.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 90.0f);
  Script_SetShadowRange(120.0f);
  Script_facemotUtils(CHARA_AKIR, 1, 0);
  script_1235(CHARA_TAIJ);
  Script_facemotUtils(CHARA_TAIJ, 1, 0);
  script_1235(CHARA_KISY);
  Script_facemotUtils(CHARA_KISY, 1, 0);
  script_1235(CHARA_CHAI);
  Script_charaSetRefboardFlagTo(CHARA_TAIJ, 1);
  Script_SetCharaRefBoardPos(CHARA_TAIJ, &local_B4);
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 1);
  Script_SetCharaRefBoardPos(CHARA_AKIR, &local_B4);
  Script_charaSetRefboardFlagTo(CHARA_CHAI, 1);
  Script_SetCharaRefBoardPos(CHARA_CHAI, &local_B4);
  Script_charaSetRefboardFlagTo(CHARA_KISY, 1);
  Script_SetCharaRefBoardPos(CHARA_KISY, &local_B4);
  Script_SetCharaFlags(CHARA_AKIR, 4);
  Script_SetCharaFlags(CHARA_TAIJ, 4);
  Script_SetCharaFlags(CHARA_CHAI, 4);
  Script_SetCharaFlags(CHARA_KISY, 4);
  Script_charaDisplay(CHARA_CHAI, CDM_Show);
  Script_SetMotnFlag(CHARA_AKIR, -1);
  Script_SetMotnFlag(CHARA_KISY, -1);
  Script_SetMotnFlag(CHARA_TAIJ, -1);
  Script_SetMotnFlag(CHARA_CHAI, -1);
  Script_clmdSetFlags(CHARA_AKIR, 2, 0);
  Script_clmdSetFlags(CHARA_KISY, 2, 0);
  Script_clmdSetFlags(CHARA_TAIJ, 2, 0);
  return;
}

static void script_2CF8(void) {
  int32_t unused_F4[3];
  script_12C2(CHARA_AKIR);
  Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
  Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
  Script_FaceEx(CHARA_AKIR, 0, 0, 1);
  script_12C2(CHARA_CHAI);
  script_12C2(CHARA_TAIJ);
  script_12C2(CHARA_KISY);
  Script_SetGameStateTo(AUTH_None);
  return;
}

static void script_2D60(void) {
  int32_t unused_D0[12];
  Vector4 local_C0;
  local_C0.x = 1.0f;
  (&local_C0)->y = 0.4f;
  (&local_C0)->z = 0.4f;
  (&local_C0)->w = 0.4f;
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 90.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 90.0f);
  script_1235(CHARA_AKIR);
  Script_facemotUtils(CHARA_AKIR, 1, 0);
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 1);
  Script_SetCharaRefBoardPos(CHARA_AKIR, &local_C0);
  Script_SetCharaFlags(CHARA_AKIR, 4);
  Script_clmdSetFlags(CHARA_AKIR, 2, 0);
  return;
}

static void script_2DF0(void) {
  int32_t unused_F4[3];
  Script_fixoClearAttachment(SHEN_NAME('BAG0'));
  Script_SetRyoMotionDetails(CHARA_AKIR);
  Script_FACE_LCG(CHARA_AKIR, 0, 1, 0);
  script_12C2(CHARA_AKIR);
  Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
  Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
  Script_FaceEx(CHARA_AKIR, 0, 0, 1);
  Script_SetGameStateTo(AUTH_None);
  return;
}

static int32_t script_2E57(void) {
  SCNF_WaitFrames_0000(4);
  Script_scenCreateTask(script_2E9C, 0);
  SCNF_WaitFrames_0000(8);
  Script_MT5Loader_TRWV_TRWAVE(401, task_88);
  SCNF_WaitFrames_0000(10);
  Script_scenCreateTask(script_31DE, 0);
  SCNF_WaitFrames_0000(180);
  end:;
}

static int32_t script_2E9C(void) {
  int32_t local_F4[3];
  float local_E8[3];
  int32_t local_DC[3];
  Vector3 local_D0;
  Vector3 local_C4;
  int32_t local_C0;
  int32_t local_BC;
  int32_t local_B8;
  int32_t local_B4;
  local_E8[0] = *(float *)&data_17AA4;
  local_E8[1] = *(float *)(&data_17AA4 + 1) - 0.3f;
  local_E8[2] = *(float *)(&data_17AA4 + 2) + 0.2f;
  local_C0 = 0x3ED91687;
  local_BC = 0x3E99999A;
  local_B8 = 0x3D4CCCCD;
  local_B4 = 0x3D0F5C29;
  *(float *)&local_F4[0] = *local_E8;
  *(float *)&local_F4[1] = local_E8[1];
  *(float *)&local_F4[2] = local_E8[2];
  local_DC[0] = 0x3DCCCCCD;
  local_DC[1] = 0x3D4CCCCD;
  local_DC[2] = 0x3DCCCCCD;
  local_C4.x = 0.03f;
  (&local_C4)->y = 0.02f;
  (&local_C4)->z = 0.04f;
  Script_SetEFPT(11, task_90, 81, 168, 224, 0xB0);
  Script_SetEFPT(8, task_90, 0.037647061f);
  Script_SetEFPT(1, task_90);
  local_D0.x = 0.1f;
  (&local_D0)->y = 0.1f;
  (&local_D0)->z = 0.12f;
  Script_SetEFPT(6, task_90, &local_D0, &local_C4);
  Script_SetEFPT(7, task_90, 0.125f);
  SCNF_WaitFrames_0000(1);
  local_D0.x = 0.25f;
  (&local_D0)->y = 0.1f;
  (&local_D0)->z = 0.1f;
  Script_SetEFPT(6, task_90, &local_D0, &local_C4);
  Script_SetEFPT(7, task_90, 0.143f);
  SCNF_WaitFrames_0000(1);
  local_D0.x = 0.3f;
  (&local_D0)->y = 0.11f;
  (&local_D0)->z = 0.08f;
  Script_SetEFPT(6, task_90, &local_D0, &local_C4);
  Script_SetEFPT(7, task_90, 0.167f);
  SCNF_WaitFrames_0000(1);
  local_D0.x = 0.35f;
  (&local_D0)->y = 0.12f;
  (&local_D0)->z = 0.06f;
  Script_SetEFPT(6, task_90, &local_D0, &local_C4);
  Script_SetEFPT(7, task_90, 0.2f);
  SCNF_WaitFrames_0000(1);
  local_D0.x = 0.4f;
  (&local_D0)->y = 0.11f;
  (&local_D0)->z = 0.05f;
  Script_SetEFPT(6, task_90, &local_D0, &local_C4);
  Script_SetEFPT(7, task_90, 0.25f);
  SCNF_WaitFrames_0000(1);
  local_D0.x = 0.45f;
  (&local_D0)->y = 0.1f;
  (&local_D0)->z = 0.04f;
  Script_SetEFPT(6, task_90, &local_D0, &local_C4);
  Script_SetEFPT(7, task_90, 0.33f);
  SCNF_WaitFrames_0000(1);
  local_D0.x = 0.35f;
  (&local_D0)->y = 0.12f;
  (&local_D0)->z = 0.03f;
  Script_SetEFPT(6, task_90, &local_D0, &local_C4);
  Script_SetEFPT(7, task_90, 0.5f);
  SCNF_WaitFrames_0000(1);
  local_D0.x = 0.2f;
  (&local_D0)->y = 0.14f;
  (&local_D0)->z = 0.02f;
  Script_SetEFPT(6, task_90, &local_D0, &local_C4);
  Script_SetEFPT(7, task_90, 1.0f);
  SCNF_WaitFrames_0000(1);
  Script_SetEFPT(7, task_90, 0.0f);
  SCNF_WaitFrames_0000(50);
  Script_SetEFPT(3, task_90);
  return Script_srchControl(0);
}

static int32_t script_31DE(void) {
  float local_F4[3];
  int32_t local_E8[3];
  Vector3 local_DC;
  Vector3 local_D0;
  local_F4[0] = *(float *)&data_17AA4;
  local_F4[1] = *(float *)(&data_17AA4 + 1) - 0.2f;
  local_F4[2] = *(float *)(&data_17AA4 + 2) + 0.2f;
  local_E8[0] = 0x3E19999A;
  local_E8[1] = 0;
  local_E8[2] = 0x3E19999A;
  local_DC.x = 0.000392153859f;
  (&local_DC)->y = 0.0662745163f;
  (&local_DC)->z = 0.000392153859f;
  local_D0.x = 0.0176470578f;
  (&local_D0)->y = 0.0427451059f;
  (&local_D0)->z = 0.0176470578f;
  Script_SetEFPT(11, task_94, 69, 179, 218, 0xB7);
  Script_SetEFPT(6, task_94, &local_DC, &local_D0);
  Script_SetEFPT(8, task_94, 0.0431372561f);
  Script_SetEFPT(1, task_94);
  SCNF_WaitFrames_0000(8);
  Script_SetEFPT(7, task_94, 0.0f);
  SCNF_WaitFrames_0000(50);
  Script_SetEFPT(3, task_94);
  return Script_srchControl(0);
}

static int32_t script_32DF(void) {
  Script_EffectUtils(9, task_98, 91, 88, 89);
  Script_EffectUtils(8, task_98, 3.0f, 3.0f);
  Script_EffectUtils(11, task_98, 114);
  Script_EffectUtils(12, task_98, 17);
  Script_EffectUtils(1, task_98);
  Script_EffectUtils(7, task_98, 0.024f);
  SCNF_WaitFrames_0000(30);
  Script_EffectUtils(7, task_98, 0.0f);
  SCNF_WaitFrames_0000(52);
  Script_EffectUtils(2, task_98);
  return Script_EffectUtils(3, task_98);
}

static int32_t script_3367(void) {
  Script_EffectUtils(6, task_9C, &data_18448, &data_18454);
  Script_EffectUtils(9, task_9C, 152, 145, 152);
  Script_EffectUtils(8, task_9C, 1.21568632f, 1.09803927f);
  Script_EffectUtils(11, task_9C, 122);
  Script_EffectUtils(12, task_9C, 38);
  Script_EffectUtils(10, task_9C, 0.482352942f);
  Script_EffectUtils(1, task_9C);
  SCNF_WaitFrames_0000(30);
  Script_EffectUtils(7, task_9C, 0.0f);
  Script_EffectUtils(2, task_9C);
  return Script_EffectUtils(3, task_9C);
}

static void script_33FE(int32_t arg0, int32_t arg1) {
  Script_ccowUtils(arg1, 2);
  Script_motmToggleFlags(arg0, 2, 0x40);
  Script_RequestFightAnimation(arg0, 657, 0.0f, 14, 1);
  SCNF_WaitFrames_0000(1);
  Script_fixoAttachEx(arg1, arg0, 18, &data_18460, &data_1846C);
  Script_CharHandlers(arg0, 0, 0, 16);
  Script_CharHandlers(arg0, 1, 8, 16);
  Script_charaDisplay(arg1, CDM_Show);
  return;
}

__attribute__((scn_string_bindings(0, data_18484, 1, data_18491, 2, data_1849A, 3, data_184A7, 4, data_184B2, 5, data_184BF)))
static int32_t script_3462(void) {
  int32_t local_FC;
  int32_t unused_F8;
  local_FC = Script_MT5Loader_TRWV_TRWAVE(0, &data_18478, "model/object", "SEA3.PVR", "model/object", "TRSEA4.MT6", 4.0f, 0);
  Script_MT5Loader_TRWV_TRWAVE(21, local_FC, 8, 8);
  Script_MT5Loader_TRWV_TRWAVE(16, local_FC, -1, 0);
  Script_MT5Loader_TRWV_TRWAVE(13, local_FC, 0.0901960805f, 0.196078435f);
  Script_MT5Loader_TRWV_TRWAVE(15, local_FC, "model/object", "TRWTBL1.BIN");
  Script_MT5Loader_TRWV_TRWAVE(12, local_FC, 0x4000);
  Script_MT5Loader_TRWV_TRWAVE(10, local_FC, 64.0f, 64.0f, 0, 0, 0, 0);
  Script_MT5Loader_TRWV_TRWAVE(24, local_FC, 0, 203, 255, 143);
  Script_MT5Loader_TRWV_TRWAVE(11, local_FC, -0.51f, -0.68f, 0.52f);
  return local_FC;
}

__attribute__((scn_string_bindings(0, data_184D8, 1, data_184E5, 2, data_184EE, 3, data_184FB, 4, data_18506, 5, data_18513)))
static void script_3527(void) {
  int32_t local_FC;
  int32_t unused_F8;
  local_FC = Script_MT5Loader_TRWV_TRWAVE(0, &data_184CC, "model/object", "SEA2.PVR", "model/object", "TRSEA4.MT6", 4.0f, 0);
  Script_MT5Loader_TRWV_TRWAVE(22, local_FC, 2, 1);
  Script_MT5Loader_TRWV_TRWAVE(21, local_FC, 8, 8);
  Script_MT5Loader_TRWV_TRWAVE(16, local_FC, -1, 0);
  Script_MT5Loader_TRWV_TRWAVE(13, local_FC, 0.0901960805f, 0.196078435f);
  Script_MT5Loader_TRWV_TRWAVE(15, local_FC, "model/object", "TRWTBL1.BIN");
  Script_MT5Loader_TRWV_TRWAVE(12, local_FC, 0x4000);
  Script_MT5Loader_TRWV_TRWAVE(10, local_FC, 64.0f, 64.0f, 1, 1, 3, 1);
  Script_MT5Loader_TRWV_TRWAVE(24, local_FC, 0, 117, 127, 135);
  Script_MT5Loader_TRWV_TRWAVE(11, local_FC, -0.51f, -0.68f, 0.52f);
  Script_MT5Loader_TRWV_TRWAVE(2, local_FC);
  (void)local_FC;
  return;
}

static void script_3600(int32_t arg0) {
  Script_MT5Loader_TRWV_TRWAVE(3, arg0);
  return;
}

static void script_360C(int32_t arg0) {
  int8_t local_FF;
  (void)((local_FF = -1) & 8);
  for (;;) {
    if (Script_SetTaskDataValue(CHARA_AKIR, &data_18520, &data_1852C, 0) == 0) {
      if (local_FF == 0) {
        Script_MT5Loader_TRWV_TRWAVE(1, arg0);
        (void)((local_FF = -1) & 8);
      }
    } else {
      if (local_FF == -1) {
        Script_MT5Loader_TRWV_TRWAVE(2, arg0);
        (void)((local_FF = 0) & 8);
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  return;
}

__attribute__((scn_string_bindings(0, data_18538, 2, data_18552, 3, data_1855F)))
static void script_3679(void) {
  Vector3 local_F4;
  Vector3 local_E8;
  task_88 = script_3462();
  Script_MT5Loader_TRWV_TRWAVE(10, task_88, 64.0f, 64.0f, 3, 1, 1, 1);
  Script_MT5Loader_TRWV_TRWAVE(400, task_88, "model/object", "SIRANAMI.PVR", "model/object", "TRSEA4.MT6");
  local_F4.x = *(float *)&data_17AA4;
  (&local_F4)->y = *(float *)(&data_17AA4 + 1) - 0.3f;
  (&local_F4)->z = *(float *)(&data_17AA4 + 2) + 0.2f;
  local_E8.x = 0.1f;
  (&local_E8)->y = 0.05f;
  (&local_E8)->z = 0.1f;
  task_90 = Script_SetEFPT(10, &local_F4, &local_E8, 2000, 0.25f, 0x200, -2.5f, 0, 0);
  local_F4.x = *(float *)&data_17AA4;
  (&local_F4)->y = *(float *)(&data_17AA4 + 1) - 0.2f;
  (&local_F4)->z = *(float *)(&data_17AA4 + 2) + 0.2f;
  local_E8.x = 0.15f;
  (&local_E8)->y = 0.0f;
  (&local_E8)->z = 0.15f;
  task_94 = Script_SetEFPT(10, &local_F4, &local_E8, 900, 0.25f, 0x300, -2.5f, 0, 0);
  return;
}

static void script_37B7(void) {
  Script_MT5Loader_TRWV_TRWAVE(403, task_88);
  Script_MT5Loader_TRWV_TRWAVE(3, task_88);
  return;
}

static void script_37CF(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  int32_t local_F0;
  int32_t local_EC;
  script_3DE2();
  local_F8 = Script_shenSeqUtils(-8, 0);
  Script_shenSeqUtils(-9, 0, 0);
  Script_executeAicaCommand(16296, 0, 0);
  Script_executeAicaCommand(1184, 2, 100);
  if (arg0) {
    Script_scenCreateTask(script_1312, 0);
  }
  Script_shenSeqUtils(0);
  local_F0 = 0;
  local_FC = 0;
  while (Script_shenSeqUtils(-1)) {
    local_EC = local_FC;
    if (local_EC == 1) {
      Script_SetMaxInterpSpeed(0.0f);
      Script_SetMotnFlag(CHARA_AKIR, -1);
      Script_SetMotnFlag(CHARA_CHAI, -1);
      Script_FaceEx(CHARA_AKIR, 1, 0, 1);
      Script_ResetHandMotion(CHARA_AKIR, 1, 0, 0);
      Script_ResetHandMotion(CHARA_AKIR, 1, 1, 0);
      Script_handSetFloatsFor(CHARA_AKIR, 0, data_1856C, 1);
      Script_handSetFloatsFor(CHARA_AKIR, 1, data_174E0, 1);
      Script_handSetFloatsFor(CHARA_AKIR, 1, data_1856C, 1);
      Script_FaceEx(CHARA_CHAI, 1, 0, 1);
      Script_ResetHandMotion(CHARA_CHAI, 1, 0, 0);
      Script_ResetHandMotion(CHARA_CHAI, 1, 1, 0);
      Script_handSetFloatsFor(CHARA_CHAI, 0, data_18650, 1);
      Script_handSetFloatsFor(CHARA_CHAI, 1, data_18650, 1);
      Script_FACE(CHARA_CHAI, 0, 0, 10);
      goto L003928;
    }
    L003928:;
    if (local_FC == 440) {
      Script_FACE_LCG(CHARA_CHAI, 1, 30, 0);
    }
    if (-(local_FC > 540) == -(local_FC < 660)) {
      if (local_F0 == 0) {
        if (local_F4 == 0) {
          Script_handSetFloatsFor(CHARA_CHAI, 0, data_18734, 2);
          local_F4 = 1;
          local_F0 = Script_FloatUtils(ScriptMathOp_RandomIntBelow, 5) + 10;
        } else if (local_F4 == 1) {
          Script_handSetFloatsFor(CHARA_CHAI, 0, data_18650, 2);
          local_F4 = 2;
          local_F0 = 0;
        } else if (local_F4 == 2) {
          Script_handSetFloatsFor(CHARA_CHAI, 0, data_18734, 2);
          local_F4 = 3;
          local_F0 = 3;
        } else {
          Script_handSetFloatsFor(CHARA_CHAI, 0, data_18650, 2);
          local_F4 = 0;
          local_F0 = 1;
        }
      } else {
        local_F0 = local_F0 - 1;
      }
    }
    if (local_FC == 560) {
      break;
    }
    if (local_FC == 500) {
      Script_executeAicaCommand(2720, 2, 80);
    }
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  Script_filmCheckOrSetTransitionUtil(10, 0, 255, 255, 255, 255, 255, 255, 255);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  script_3EE4();
  Script_shenSeqUtils(-9, 0, local_F8);
  return;
}

static void script_3A85(void) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  int32_t local_F0;
  Vector3 local_E4;
  int32_t local_E0;
  script_3DE2();
  local_F8 = Script_shenSeqUtils(-8, 1);
  Script_shenSeqUtils(-9, 1, 0);
  Script_filmCheckOrSetTransitionUtil(1, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_executeAicaCommand(25768, 0, 0);
  Script_executeAicaCommand(1184, 2, 120);
  Script_shenSeqUtils(1);
  local_F0 = 0;
  local_FC = 0;
  while (Script_shenSeqUtils(-1)) {
    local_E0 = local_FC;
    if (local_E0 == 1) {
      Script_SetMaxInterpSpeed(0.0f);
      Script_SetMotnFlag(CHARA_AKIR, -1);
      Script_SetMotnFlag(CHARA_CHAI, -1);
      Script_FaceEx(CHARA_AKIR, 1, 0, 1);
      Script_ResetHandMotion(CHARA_AKIR, 1, 0, 0);
      Script_ResetHandMotion(CHARA_AKIR, 1, 1, 0);
      Script_handSetFloatsFor(CHARA_AKIR, 0, data_1856C, 1);
      Script_handSetFloatsFor(CHARA_AKIR, 1, data_174E0, 1);
      Script_handSetFloatsFor(CHARA_AKIR, 1, data_1856C, 1);
      Script_FaceEx(CHARA_CHAI, 1, 0, 1);
      Script_ResetHandMotion(CHARA_CHAI, 1, 0, 0);
      Script_ResetHandMotion(CHARA_CHAI, 1, 1, 0);
      Script_handSetFloatsFor(CHARA_CHAI, 0, data_18650, 1);
      Script_handSetFloatsFor(CHARA_CHAI, 1, data_18650, 1);
      Script_FACE(CHARA_CHAI, 0, 0, 10);
      Script_FACE_LCG(CHARA_CHAI, 1, 0, 0);
      goto L003C08;
    }
    L003C08:;
    if (local_FC < 131) {
      if (local_F0 == 0) {
        if (local_F4 == 0) {
          Script_handSetFloatsFor(CHARA_CHAI, 0, data_18734, 2);
          local_F4 = 1;
          local_F0 = Script_FloatUtils(ScriptMathOp_RandomIntBelow, 5) + 10;
        } else if (local_F4 == 1) {
          Script_handSetFloatsFor(CHARA_CHAI, 0, data_18650, 2);
          local_F4 = 2;
          local_F0 = 0;
        } else if (local_F4 == 2) {
          Script_handSetFloatsFor(CHARA_CHAI, 0, data_18734, 2);
          local_F4 = 3;
          local_F0 = 3;
        } else {
          Script_handSetFloatsFor(CHARA_CHAI, 0, data_18650, 2);
          local_F4 = 0;
          local_F0 = 1;
        }
      } else {
        local_F0 = local_F0 - 1;
      }
    }
    if (local_FC == 56) {
      Script_FACE_LCG(CHARA_CHAI, 2, 2, 0);
    }
    if (local_FC == 59) {
      Script_FACE(CHARA_CHAI, 5, 0, 10);
    }
    if (local_FC == 83) {
      Script_FACE(CHARA_AKIR, 5, 0, 7);
    }
    if (local_FC == 121) {
      Script_handSetFloatsFor(CHARA_CHAI, 0, data_174E0, 20);
      Script_handSetFloatsFor(CHARA_CHAI, 1, data_174E0, 20);
    }
    switch (local_FC) {
      case 74:
        Script_SetFightProgramStateBytes(CHARA_CHAI, 2, 1);
        Script_GetDataFromFightProgram(CHARA_CHAI, 1);
        break;
      case 141:
        Script_GetDataFromFightProgram(CHARA_CHAI, 0);
        break;
    }
    if (local_FC == 75) {
      local_E4.x = 70.702499f;
      (&local_E4)->y = 0.8512f;
      (&local_E4)->z = 24.5648f;
      Script_FaceLookAtTargetUtils(CHARA_AKIR, 2, &local_E4, 5);
    }
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  script_3EE4();
  Script_shenSeqUtils(-9, 1, local_F8);
  return;
}

static void script_3DE2(void) {
  Vector4 local_F0;
  local_F0.x = 0.4f;
  (&local_F0)->y = 0.4f;
  (&local_F0)->z = 0.4f;
  (&local_F0)->w = 0.4f;
  Script_executeAicaCommand(25768, 0, 0);
  Script_executeAicaCommand(1184, 2, 120);
  Script_executeAicaCommand(16296, 0, 0);
  Script_executeAicaCommand(1184, 2, 100);
  Script_executeAicaCommand(1184, 4, 127);
  Script_executeAicaCommand(1184, 1, 96);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 90.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 90.0f);
  script_1235(CHARA_AKIR);
  Script_facemotUtils(CHARA_AKIR, 1, 0);
  script_1235(CHARA_CHAI);
  Script_facemotUtils(CHARA_CHAI, 1, 0);
  Script_SetCharaFlags(CHARA_AKIR, 4);
  Script_SetCharaFlags(CHARA_CHAI, 4);
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 1);
  Script_charaSetRefboardFlagTo(CHARA_CHAI, 1);
  Script_SetCharaRefBoardPos(CHARA_AKIR, &local_F0);
  Script_SetCharaRefBoardPos(CHARA_CHAI, &local_F0);
  return;
}

static void script_3EE4(void) {
  int32_t unused_F4[3];
  script_12C2(CHARA_AKIR);
  Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
  Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
  script_12C2(CHARA_CHAI);
  Script_SetGameStateTo(AUTH_None);
  Script_SetMaxInterpSpeed(0.5f);
  return;
}

static void script_3F2F(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  script_427C();
  local_F8 = Script_shenSeqUtils(-8, 2);
  Script_shenSeqUtils(-9, 2, 0);
  Script_shenSeqUtils(2);
  local_FC = 0;
  while (Script_shenSeqUtils(-1)) {
    local_F4 = local_FC;
    if (local_F4 == 0) {
      Script_SetMaxInterpSpeed(0.0f);
      Script_SetMotnFlag(CHARA_AKIR, -1);
      Script_SetMotnFlag(CHARA_CHAI, -1);
      Script_FaceEx(CHARA_AKIR, 1, 0, 1);
      Script_ResetHandMotion(CHARA_AKIR, 1, 0, 0);
      Script_ResetHandMotion(CHARA_AKIR, 1, 1, 0);
      Script_handSetFloatsFor(CHARA_AKIR, 0, data_18818, 0);
      Script_handSetFloatsFor(CHARA_AKIR, 1, data_174E0, 0);
      Script_handSetFloatsFor(CHARA_AKIR, 1, data_18818, 0);
      Script_FACE(CHARA_AKIR, 2, 0, 0);
      Script_FaceEx(CHARA_CHAI, 1, 0, 1);
      Script_ResetHandMotion(CHARA_CHAI, 1, 0, 0);
      Script_ResetHandMotion(CHARA_CHAI, 1, 1, 0);
      Script_handSetFloatsFor(CHARA_CHAI, 0, data_188FC, 0);
      Script_handSetFloatsFor(CHARA_CHAI, 1, data_188FC, 0);
      Script_FACE(CHARA_CHAI, 0, 0, 10);
      goto L00406B;
    }
    L00406B:;
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  script_433C();
  Script_shenSeqUtils(-9, 2, local_F8);
  return;
}

static void script_408F(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  script_427C();
  local_F8 = Script_shenSeqUtils(-8, 3);
  Script_shenSeqUtils(-9, 3, 0);
  Script_shenSeqUtils(3);
  local_FC = 0;
  while (Script_shenSeqUtils(-1)) {
    local_F4 = local_FC;
    if (local_F4 == 0) {
      Script_SetMaxInterpSpeed(0.0f);
      Script_SetMotnFlag(CHARA_AKIR, -1);
      Script_SetMotnFlag(CHARA_CHAI, -1);
      Script_FaceEx(CHARA_AKIR, 1, 0, 1);
      Script_ResetHandMotion(CHARA_AKIR, 1, 0, 0);
      Script_ResetHandMotion(CHARA_AKIR, 1, 1, 0);
      Script_handSetFloatsFor(CHARA_AKIR, 0, data_18818, 0);
      Script_handSetFloatsFor(CHARA_AKIR, 1, data_174E0, 0);
      Script_handSetFloatsFor(CHARA_AKIR, 1, data_18818, 0);
      Script_FaceEx(CHARA_CHAI, 1, 0, 1);
      Script_ResetHandMotion(CHARA_CHAI, 1, 0, 0);
      Script_ResetHandMotion(CHARA_CHAI, 1, 1, 0);
      Script_handSetFloatsFor(CHARA_CHAI, 0, data_188FC, 0);
      Script_handSetFloatsFor(CHARA_CHAI, 1, data_188FC, 0);
      Script_FACE(CHARA_CHAI, 0, 0, 10);
      Script_FACE(CHARA_AKIR, 0, 0, 1);
      goto L0041CB;
    }
    L0041CB:;
    if (local_FC == 45) {
      Script_SetFightProgramStateBytes(CHARA_CHAI, 2, 1);
      Script_GetDataFromFightProgram(CHARA_CHAI, 1);
    }
    if (local_FC == 146) {
      Script_GetDataFromFightProgram(CHARA_CHAI, 0);
    }
    switch (local_FC) {
      case 75:
        Script_SetFightProgramStateBytes(CHARA_AKIR, 2, 1);
        Script_GetDataFromFightProgram(CHARA_AKIR, 1);
        break;
      case 90:
        Script_GetDataFromFightProgram(CHARA_AKIR, 0);
        break;
    }
    if (local_FC == 120) {
      Script_clmdSetFlags(CHARA_CHAI, 2, 1);
    }
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  script_433C();
  Script_shenSeqUtils(-9, 3, local_F8);
  return;
}

static void script_427C(void) {
  Vector4 local_F0;
  local_F0.x = 0.4f;
  (&local_F0)->y = 0.4f;
  (&local_F0)->z = 0.4f;
  (&local_F0)->w = 0.4f;
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 90.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 90.0f);
  script_1235(CHARA_AKIR);
  Script_facemotUtils(CHARA_AKIR, 1, 0);
  script_1235(CHARA_CHAI);
  Script_facemotUtils(CHARA_CHAI, 1, 0);
  Script_SetCharaFlags(CHARA_AKIR, 4);
  Script_SetCharaFlags(CHARA_CHAI, 4);
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 1);
  Script_charaSetRefboardFlagTo(CHARA_CHAI, 1);
  Script_SetCharaRefBoardPos(CHARA_AKIR, &local_F0);
  Script_SetCharaRefBoardPos(CHARA_CHAI, &local_F0);
  return;
}

static void script_433C(void) {
  int32_t unused_F4[3];
  script_12C2(CHARA_AKIR);
  Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
  Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
  script_12C2(CHARA_CHAI);
  Script_SetGameStateTo(AUTH_None);
  Script_SetMaxInterpSpeed(0.5f);
  return;
}

static void script_4387(int32_t arg0) {
  int8_t local_FF;
  int32_t local_F8;
  int32_t local_F4;
  Script_ryoControl(PCCTL_DisablePlayerControl);
  local_F4 = arg0;
  if (local_F4 == 0) {
    local_F8 = Script_scenCreateTask(script_468A, 0);
    Script_shenSeqUtils(arg0);
    while (Script_shenSeqUtils(-1)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_srchControl((void *)local_F8);
    local_FF = Script_shenSeqUtils(-7);
    if (local_FF == 1) {
      Script_executeAicaCommand(2720, 2, 30);
      Script_filmCheckOrSetTransitionUtil(0, 255, 255, 255, 255, 0, 0, 0, 0);
      SCNF_WaitFrames_0000(30);
    }
    goto L004412;
  }
  L004412:;
  return;
}

static void script_4413(void) {
  script_4518(CHARA_AKIR);
  script_4518(CHARA_HRSK);
  script_44F4(SHEN_NAME('OMAM'));
  script_44F4(SHEN_NAME('TBR1'));
  script_44F4(SHEN_NAME('TBR2'));
  script_44F4(SHEN_NAME('TBR3'));
  script_44F4(SHEN_NAME('TBR4'));
  Script_facemotUtils(CHARA_AKIR, 1, 0);
  Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
  Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 1);
  Script_SetCharaRefBoardPos(CHARA_AKIR, &data_18E54);
  Script_charaSetRefboardFlagTo(SHEN_NAME('OMAM'), 1);
  Script_SetCharaRefBoardPos(SHEN_NAME('OMAM'), &data_18E54);
  Script_charaSetRefboardFlagTo(CHARA_HRSK, 1);
  Script_SetCharaRefBoardPos(CHARA_HRSK, &data_18E54);
  Script_SetMotnFlag(CHARA_AKIR, -1);
  return;
}

static void script_44F4(int32_t arg0) {
  Script_charaDisplay(arg0, CDM_Show);
  Script_ccowSetFlag80(arg0, 1);
  Script_ccowUtils(arg0, 2);
  Script_scenToggleActiveFor(arg0, 0);
  return;
}

static void script_4518(int32_t arg0) {
  Script_charaDisplay(arg0, CDM_Show);
  Script_motmToggleFlags(arg0, 2, 0x40);
  Script_ccowSetFlag80(arg0, 1);
  Script_facemotUtils(arg0, 1, 0);
  Script_facemotUtils(arg0, 0, 0);
  Script_FaceEx(arg0, 1, 0, 2);
  Script_FACE_LCG(arg0, 0, 6, 0);
  Script_FaceLookAtTargetUtils(arg0, 0);
  Script_SetCharaFlags(arg0, 4);
  Script_clmdSetFlags(arg0, 2, 0);
  Script_SetCPAMFlags(arg0, 0);
  Script_ccowUtils(arg0, 2);
  Script_SetMotnFlag(arg0, -1);
  Script_ResetHandMotion(arg0, 1, 0, 0);
  Script_ResetHandMotion(arg0, 1, 1, 0);
  Script_handSetFloatsFor(arg0, 1, data_18D70, 0);
  Script_handSetFloatsFor(arg0, 0, data_18D70, 0);
  Script_SetShadowFlags(arg0, SCRIPT_SHADOW_UPDATE_LIGHT_MASK, 10);
  return;
}

static void script_45DB(void) {
  script_4611(CHARA_AKIR);
  script_4611(CHARA_HRSK);
  return;
}

static void script_45F6(int32_t arg0) {
  Script_charaDisplay(arg0, CDM_Hide);
  Script_ccowUtils(arg0, 1);
  Script_ccowSetFlag80(arg0, 2);
  return;
}

static void script_4611(int32_t arg0) {
  Script_FaceLookAtTargetUtils(arg0, 0);
  Script_SetCharaLookAtPosition(arg0, -98310, 0, 0);
  Script_ccowUtils(arg0, 1);
  Script_ccowSetFlag80(arg0, 2);
  Script_FaceEx(arg0, 0, 0, 2);
  Script_FaceLookAtTargetUtils(arg0, 0);
  Script_CharHandlers(arg0, -1, 0, 0);
  Script_SetCPAMFlags(arg0, 1);
  Script_ResetHandMotion(arg0, 0, 0, 0);
  Script_ResetHandMotion(arg0, 0, 1, 0);
  Script_SetMotnFlag(arg0, 1);
  return;
}

static int32_t script_468A(void) {
  int16_t local_FE;
  Vector3i local_F0;
  Vector3i local_E4;
  Vector3i local_D8;
  Vector3i local_CC;
  Vector3 local_C0;
  Vector3 local_B4;
  local_E4.x = 0;
  (&local_E4)->y = 0;
  (&local_E4)->z = 0;
  local_D8.x = 0;
  (&local_D8)->y = 0;
  (&local_D8)->z = 0;
  local_CC.x = 0;
  (&local_CC)->y = 0;
  (&local_CC)->z = 0;
  local_FE = 0;
  while (1) {
    if (Script_shenSeqUtils(-1)) {
      switch (local_FE) {
        case 0:
          Script_charaDisplay(SHEN_NAME('OMAM'), CDM_Hide);
          local_D8.x = 0;
          (&local_D8)->y = -1820;
          (&local_D8)->z = -182;
          local_E4.x = 1820;
          (&local_E4)->y = -546;
          (&local_E4)->z = 182;
          Script_handSetFloatsFor(CHARA_HRSK, 1, data_18BA8, 0);
          Script_handSetFloatsFor(CHARA_HRSK, 0, data_18AC4, 0);
          Script_SetHandRootRotation(CHARA_HRSK, 0, 0x15, &local_E4);
          Script_SetHandRootRotation(CHARA_HRSK, 1, 0x15, &local_D8);
          Script_FACE_LCG(CHARA_HRSK, 1, 0, 0);
          Script_ResetHandMotion(CHARA_AKIR, 1, 0, 0);
          Script_ResetHandMotion(CHARA_AKIR, 1, 1, 0);
          Script_handSetFloatsFor(CHARA_AKIR, 1, data_18D70, 0);
          Script_handSetFloatsFor(CHARA_AKIR, 0, data_18D70, 0);
          break;
        case 543:
          Script_FACE_LCG(CHARA_HRSK, 2, 7, 0);
          break;
        case 640:
          local_E4.x = 0;
          (&local_E4)->y = 0;
          (&local_E4)->z = 0;
          local_D8.x = 0;
          (&local_D8)->y = 0;
          (&local_D8)->z = 0;
          Script_FACE_LCG(CHARA_HRSK, 0, 6, 0);
          Script_handSetFloatsFor(CHARA_HRSK, 1, data_18D70, 0);
          Script_handSetFloatsFor(CHARA_HRSK, 0, data_18D70, 0);
          Script_SetHandRootRotation(CHARA_HRSK, 0, 0x15, &local_E4);
          Script_SetHandRootRotation(CHARA_HRSK, 1, 0x15, &local_D8);
          break;
        case 1540:
          Script_charaDisplay(SHEN_NAME('OMAM'), CDM_Show);
          local_C0.x = 0.068f;
          (&local_C0)->y = -0.032f;
          (&local_C0)->z = 0.006f;
          local_F0.x = 34213;
          (&local_F0)->y = 6003;
          (&local_F0)->z = 18743;
          Script_fixoAttachEx(SHEN_NAME('OMAM'), CHARA_HRSK, 12, &local_C0, &local_F0);
          local_E4.x = -8374;
          (&local_E4)->y = 728;
          (&local_E4)->z = -1456;
          local_D8.x = 12379;
          (&local_D8)->y = 1820;
          (&local_D8)->z = 728;
          Script_handSetFloatsFor(CHARA_HRSK, 1, data_189E0, 0);
          Script_handSetFloatsFor(CHARA_HRSK, 0, data_189E0, 0);
          Script_SetHandRootRotation(CHARA_HRSK, 0, 0x15, &local_E4);
          Script_SetHandRootRotation(CHARA_HRSK, 1, 0x15, &local_D8);
          break;
        case 2089:
          local_CC.x = -5643;
          (&local_CC)->y = 2912;
          (&local_CC)->z = -6371;
          Script_SetHandRootRotation(CHARA_AKIR, 1, 0x15, &local_CC);
          local_B4.x = -38.12f;
          (&local_B4)->y = 4.7f;
          (&local_B4)->z = 40.299f;
          Script_FaceLookAtTargetUtils(CHARA_AKIR, 2, &local_B4, 0);
          break;
        case 2144:
          Script_FaceLookAtTargetUtils(CHARA_AKIR, 0);
          break;
        case 2132:
          Script_handSetFloatsFor(CHARA_AKIR, 1, data_18C8C, 20);
          break;
        case 2139:
          local_CC.x = 0;
          (&local_CC)->y = 0;
          (&local_CC)->z = 0;
          Script_SetHandRootRotation(CHARA_AKIR, 1, 0x15, &local_CC);
          Script_fixoClearAttachment(SHEN_NAME('OMAM'));
          local_C0.x = 0.097f;
          (&local_C0)->y = 0.027f;
          (&local_C0)->z = -0.018f;
          local_F0.x = 34395;
          (&local_F0)->y = 50792;
          (&local_F0)->z = 17649;
          Script_fixoAttachEx(SHEN_NAME('OMAM'), CHARA_AKIR, 18, &local_C0, &local_F0);
          break;
        case 2250:
          Script_handSetFloatsFor(CHARA_HRSK, 1, data_18D70, 20);
          Script_handSetFloatsFor(CHARA_HRSK, 0, data_18D70, 20);
          local_E4.x = 0;
          (&local_E4)->y = 0;
          (&local_E4)->z = 0;
          local_D8.x = 0;
          (&local_D8)->y = 0;
          (&local_D8)->z = 0;
          Script_SetHandRootRotation(CHARA_HRSK, 0, 0x15, &local_E4);
          Script_SetHandRootRotation(CHARA_HRSK, 1, 0x15, &local_D8);
          break;
        case 4147:
          Script_executeAicaCommand(2720, 2, 40);
          Script_filmCheckOrSetTransitionUtil(40, 0, 0, 0, 0, 255, 255, 255, 255);
          break;
      }
      local_FE = local_FE + 1;
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_4B5C(void) {
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 30.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 30.0f);
  Script_charaDisplay(CHARA_AKIR, CDM_Hide);
  Script_charaDisplay(CHARA_TAIJ, CDM_Hide);
  Script_charaDisplay(CHARA_SINF, CDM_Hide);
  Script_charaDisplay(SHEN_NAME('SHIP'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('KAM1'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('EXT1'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('EXT2'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('EXT3'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('CVGF'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('BAG0'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('STR_'), CDM_Hide);
  return;
}

static void script_4BEE(void) {
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 150.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 60.0f);
  Script_mapmSetCamFlags(0, 0);
  Script_charaDisplay(CHARA_AKIR, CDM_Hide);
  Script_charaDisplay(CHARA_TAIJ, CDM_Hide);
  Script_charaDisplay(CHARA_SINF, CDM_Hide);
  Script_charaDisplay(SHEN_NAME('SHIP'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('KAM1'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('EXT1'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('EXT2'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('EXT3'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('CVGF'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('STR_'), CDM_Hide);
  return;
}

static void script_4C7E(void) {
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 240.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 100.0f);
  Script_mapmSetCamFlags(0, 0);
  Script_charaDisplay(CHARA_AKIR, CDM_Hide);
  Script_charaDisplay(CHARA_TAIJ, CDM_Hide);
  Script_charaDisplay(CHARA_SINF, CDM_Hide);
  Script_charaDisplay(SHEN_NAME('SHIP'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('KAM1'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('EXT1'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('EXT2'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('EXT3'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('CVGF'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('BAG0'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('STR_'), CDM_Hide);
  return;
}

static void script_4D19(void) {
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 240.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 100.0f);
  Script_mapmSetCamFlags(0, 0);
  Script_charaDisplay(CHARA_AKIR, CDM_Hide);
  Script_charaDisplay(CHARA_TAIJ, CDM_Hide);
  Script_charaDisplay(CHARA_SINF, CDM_Hide);
  Script_charaDisplay(SHEN_NAME('SHIP'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('KAM1'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('EXT1'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('EXT2'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('EXT3'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('CVGF'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('BAG0'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('STR_'), CDM_Hide);
  return;
}

static void script_4DB4(void) {
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 100.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 40.0f);
  Script_mapmSetCamFlags(0, 0);
  Script_charaDisplay(CHARA_AKIR, CDM_Hide);
  Script_charaDisplay(CHARA_TAIJ, CDM_Hide);
  Script_charaDisplay(CHARA_SINF, CDM_Hide);
  Script_charaDisplay(SHEN_NAME('SHIP'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('KAM1'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('EXT1'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('EXT2'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('EXT3'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('CVGF'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('BAG0'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('STR_'), CDM_Hide);
  return;
}

static void script_4E4F(void) {
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 240.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 100.0f);
  Script_mapmSetCamFlags(0, 0);
  Script_charaDisplay(CHARA_AKIR, CDM_Hide);
  Script_charaDisplay(CHARA_TAIJ, CDM_Hide);
  Script_charaDisplay(CHARA_SINF, CDM_Hide);
  Script_charaDisplay(SHEN_NAME('SHIP'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('KAM1'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('EXT1'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('EXT2'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('EXT3'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('CVGF'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('BAG0'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('STR_'), CDM_Hide);
  return;
}

static void script_4EEA(void) {
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 280.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 0.0f);
  Script_mapmSetCamFlags(0, 0);
  Script_charaDisplay(CHARA_AKIR, CDM_Hide);
  Script_charaDisplay(CHARA_TAIJ, CDM_Hide);
  Script_charaDisplay(CHARA_SINF, CDM_Hide);
  Script_charaDisplay(SHEN_NAME('SHIP'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('KAM1'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('EXT1'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('EXT2'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('EXT3'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('CVGF'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('BAG0'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('STR_'), CDM_Hide);
  return;
}

static void script_4F82(void) {
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 270.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 0.0f);
  Script_mapmSetCamFlags(0, 0);
  Script_charaDisplay(CHARA_AKIR, CDM_Show);
  Script_charaDisplay(CHARA_TAIJ, CDM_Show);
  Script_charaDisplay(CHARA_SINF, CDM_Hide);
  Script_charaDisplay(SHEN_NAME('SHIP'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('KAM1'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('EXT1'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('EXT2'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('EXT3'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('CVGF'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('BAG0'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('STR_'), CDM_Hide);
  return;
}

static void script_501A(void) {
  Script_WeatherUtils_0(WeatherDataUtil_SetInterpolatedWeatherOverride, 0, 0, 0, 0);
  Script_WeatherTaskUtil(ToggleFogTask, 0);
  Script_WeatherTaskUtil(ToggleCloudXYFlags, 0);
  Script_WeatherTaskUtil(ToggleSceneLightingFlags, 0);
  Script_WeatherTaskUtil(FreeTaskData);
  Script_SetFogFlags(0, data_18EA0);
  Script_shenSetLightingMode(1);
  Script_lightSetGlobalSceneLighting(&data_18E74);
  Script_lightDirLightEnable(0, &data_18E94);
  Script_LightUtils(0, 2, &data_18E84);
  Script_SetFogColour(&data_18E64);
  Script_lightSetSlotEnabled(0, 1);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 100.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 10.0f);
  Script_SpriteUtils(Scroll_Read3, 1, 2);
  Script_SpriteUtils(ScrollHorizontalOffset, 9808);
  Script_charaDisplay(CHARA_AKIR, CDM_Hide);
  Script_charaDisplay(CHARA_TAIJ, CDM_Hide);
  Script_charaDisplay(CHARA_SINF, CDM_Hide);
  Script_charaDisplay(SHEN_NAME('SHIP'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('KAM1'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('EXT1'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('EXT2'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('EXT3'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('CVGF'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('BAG0'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('STR_'), CDM_Hide);
  return;
}

static void script_5135(void) {
  Script_SetFogFlags(0, data_18EC0);
  Script_shenSetLightingMode(1);
  Script_lightSetGlobalSceneLighting(&data_18ED0);
  Script_lightDirLightEnable(0, &data_18EF0);
  Script_LightUtils(0, 2, &data_18EE0);
  Script_LightUtils(0, 4, 0.6f);
  Script_SetFogColour(&data_18EB0);
  Script_lightSetSlotEnabled(0, 1);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 250.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 0.0f);
  Script_SpriteUtils(ModifySky, 1, 1, 1);
  Script_mapmSetCamFlags(0, 0);
  Script_charaDisplay(CHARA_AKIR, CDM_Hide);
  Script_charaDisplay(CHARA_TAIJ, CDM_Hide);
  Script_charaDisplay(CHARA_SINF, CDM_Hide);
  Script_charaDisplay(SHEN_NAME('SHIP'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('KAM1'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('EXT1'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('EXT2'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('EXT3'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('CVGF'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('BAG0'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('STR_'), CDM_Hide);
  return;
}

static void script_522A(void) {
  Script_SetFogFlags(0, data_18F38);
  Script_shenSetLightingMode(1);
  Script_lightSetGlobalSceneLighting(&data_18EFC);
  Script_lightDirLightEnable(0, &data_18F1C);
  Script_LightUtils(0, 2, &data_18F0C);
  Script_LightUtils(0, 4, 0.6f);
  Script_SetFogColour(&data_18F28);
  Script_lightSetSlotEnabled(0, 1);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 240.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 100.0f);
  Script_mapmSetCamFlags(0, 0);
  Script_charaDisplay(CHARA_AKIR, CDM_Hide);
  Script_charaDisplay(CHARA_TAIJ, CDM_Hide);
  Script_charaDisplay(CHARA_SINF, CDM_Hide);
  Script_charaDisplay(SHEN_NAME('SHIP'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('KAM1'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('EXT1'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('EXT2'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('EXT3'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('CVGF'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('BAG0'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('STR_'), CDM_Hide);
  return;
}

static void script_5315(void) {
  Script_SetFogFlags(0, data_18F84);
  Script_shenSetLightingMode(1);
  Script_lightSetGlobalSceneLighting(&data_18F48);
  Script_lightDirLightEnable(0, &data_18F68);
  Script_LightUtils(0, 2, &data_18F58);
  Script_LightUtils(0, 4, 0.8f);
  Script_SetFogColour(&data_18F74);
  Script_lightSetSlotEnabled(0, 1);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 440.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 140.0f);
  Script_charaDisplay(CHARA_AKIR, CDM_Hide);
  Script_charaDisplay(CHARA_TAIJ, CDM_Hide);
  Script_charaDisplay(CHARA_SINF, CDM_Hide);
  Script_charaDisplay(SHEN_NAME('SHIP'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('KAM1'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('EXT1'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('EXT2'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('EXT3'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('CVGF'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('BAG0'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('STR_'), CDM_Show);
  return;
}

static void script_53F7(void) {
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 200.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 0.0f);
  Script_mapmSetCamFlags(0, 1);
  Script_charaDisplay(CHARA_AKIR, CDM_Show);
  Script_charaDisplay(CHARA_TAIJ, CDM_Hide);
  Script_charaDisplay(CHARA_SINF, CDM_Hide);
  Script_charaDisplay(SHEN_NAME('SHIP'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('KAM1'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('EXT1'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('EXT2'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('EXT3'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('CVGF'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('BAG0'), CDM_Hide);
  Script_SetDynamicMapStaticSceneMode(12, 0);
  Script_charaDisplay(SHEN_NAME('STR_'), CDM_Show);
  return;
}

static void script_5498(void) {
  Script_SetFogFlags(0, data_18FA4);
  Script_shenSetLightingMode(1);
  Script_lightSetGlobalSceneLighting(&data_18FB4);
  Script_lightDirLightEnable(0, &data_18FD4);
  Script_LightUtils(0, 4, 0.17f);
  Script_LightUtils(0, 2, &data_18FC4);
  Script_lightSetSlotEnabled(0, 1);
  Script_SetFogColour(&data_18F94);
  Script_SpriteUtils(ScrollHorizontalOffset, 0);
  return;
}

static void script_54F4(void) {
  Script_SetVector4ByOffset(&data_18FE0);
  (void)1;
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 200.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 100.0f);
  Script_mapmSetCamFlags(0, 0);
  Script_charaDisplay(CHARA_AKIR, CDM_Show);
  Script_charaDisplay(CHARA_TAIJ, CDM_Hide);
  Script_charaDisplay(CHARA_SINF, CDM_Hide);
  Script_charaDisplay(SHEN_NAME('SHIP'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('KAM1'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('EXT1'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('EXT2'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('EXT3'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('CVGF'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('BAG0'), CDM_Show);
  return;
}

static void script_5591(void) {
  Script_WeatherUtils(WeatherScript_InitSnowSystems, 1.0f);
  Script_WeatherUtils(WeatherScript_UpdateRainOffset, -0x5E746C64);
  Script_WeatherUtils(WeatherScript_SetSnowScalar0, 0.1f);
  Script_WeatherUtils(WeatherScript_ApplySnowGameData, 0.1f, 120);
  return;
}

static void script_55C6(int32_t arg0, int32_t arg1) {
  Vector3 local_F4;
  int32_t local_F0;
  local_F0 = arg0;
  switch (local_F0) {
    case 0:
      local_F4.x = 0.0f;
      (&local_F4)->y = 0.0827450976f;
      (&local_F4)->z = 0.0313725509f;
      task_A0.x = 0.0298039205f;
      task_A0.y = 0.00431372551f;
      task_A0.z = 0.0235294122f;
      Script_EffectUtils(6, arg1, &local_F4, &task_A0);
      Script_EffectUtils(7, arg1, 0.00980392192f);
      Script_EffectUtils(9, arg1, 23, 23, 23);
      Script_EffectUtils(8, arg1, 10.0f, 7.29411745f);
      Script_EffectUtils(16, arg1, -0.541176498f);
      Script_EffectUtils(11, arg1, 111);
      Script_EffectUtils(12, arg1, 19);
      Script_EffectUtils(10, arg1, 0.358823538f);
      break;
    case 1:
      local_F4.x = 0.0f;
      (&local_F4)->y = 0.0827450976f;
      (&local_F4)->z = 0.0313725509f;
      task_A0.x = 0.0298039205f;
      task_A0.y = 0.00431372551f;
      task_A0.z = 0.0235294122f;
      Script_EffectUtils(6, arg1, &local_F4, &task_A0);
      Script_EffectUtils(7, arg1, 0.00980392192f);
      Script_EffectUtils(9, arg1, 47, 47, 47);
      Script_EffectUtils(8, arg1, 10.0f, 7.29411745f);
      Script_EffectUtils(16, arg1, -0.541176498f);
      Script_EffectUtils(11, arg1, 111);
      Script_EffectUtils(12, arg1, 19);
      Script_EffectUtils(10, arg1, 0.358823538f);
      break;
    case 2:
      local_F4.x = 0.0f;
      (&local_F4)->y = 0.0827450976f;
      (&local_F4)->z = 0.0313725509f;
      task_A0.x = 0.0494117662f;
      task_A0.y = 0.00431372551f;
      task_A0.z = 0.0231372546f;
      Script_EffectUtils(6, arg1, &local_F4, &task_A0);
      Script_EffectUtils(7, arg1, 0.0101960786f);
      Script_EffectUtils(9, arg1, 56, 56, 56);
      Script_EffectUtils(8, arg1, 10.0f, 7.29411745f);
      Script_EffectUtils(16, arg1, -0.541176498f);
      Script_EffectUtils(11, arg1, 107);
      Script_EffectUtils(12, arg1, 19);
      Script_EffectUtils(10, arg1, 0.358823538f);
      break;
    case 3:
      local_F4.x = 0.0f;
      (&local_F4)->y = 0.0827450976f;
      (&local_F4)->z = 0.0313725509f;
      task_A0.x = 0.0494117662f;
      task_A0.y = 0.00431372551f;
      task_A0.z = 0.0231372546f;
      Script_EffectUtils(6, arg1, &local_F4, &task_A0);
      Script_EffectUtils(7, arg1, 0.0101960786f);
      Script_EffectUtils(9, arg1, 56, 56, 56);
      Script_EffectUtils(8, arg1, 10.0f, 7.29411745f);
      Script_EffectUtils(16, arg1, -0.541176498f);
      Script_EffectUtils(11, arg1, 107);
      Script_EffectUtils(12, arg1, 19);
      Script_EffectUtils(10, arg1, 0.358823538f);
      break;
    case 4:
      local_F4.x = 0.0f;
      (&local_F4)->y = 0.0827450976f;
      (&local_F4)->z = 0.0313725509f;
      task_A0.x = 0.0298039205f;
      task_A0.y = 0.00431372551f;
      task_A0.z = 0.0235294122f;
      Script_EffectUtils(6, arg1, &local_F4, &task_A0);
      Script_EffectUtils(7, arg1, 0.00980392192f);
      Script_EffectUtils(9, arg1, 33, 33, 33);
      Script_EffectUtils(8, arg1, 10.0f, 7.29411745f);
      Script_EffectUtils(16, arg1, -0.541176498f);
      Script_EffectUtils(11, arg1, 111);
      Script_EffectUtils(12, arg1, 19);
      Script_EffectUtils(10, arg1, 0.358823538f);
      break;
    case 5:
      local_F4.x = 0.0f;
      (&local_F4)->y = 0.0827450976f;
      (&local_F4)->z = 0.0313725509f;
      task_A0.x = 0.0298039205f;
      task_A0.y = 0.00431372551f;
      task_A0.z = 0.0235294122f;
      Script_EffectUtils(6, arg1, &local_F4, &task_A0);
      Script_EffectUtils(7, arg1, 0.00980392192f);
      Script_EffectUtils(9, arg1, 12, 12, 12);
      Script_EffectUtils(8, arg1, 10.0f, 7.29411745f);
      Script_EffectUtils(16, arg1, -0.541176498f);
      Script_EffectUtils(11, arg1, 111);
      Script_EffectUtils(12, arg1, 19);
      Script_EffectUtils(10, arg1, 0.358823538f);
      break;
    case 6:
      local_F4.x = 0.0f;
      (&local_F4)->y = 0.0827450976f;
      (&local_F4)->z = 0.0313725509f;
      task_A0.x = 0.0298039205f;
      task_A0.y = 0.00431372551f;
      task_A0.z = 0.0235294122f;
      Script_EffectUtils(6, arg1, &local_F4, &task_A0);
      Script_EffectUtils(7, arg1, 0.00980392192f);
      Script_EffectUtils(9, arg1, 12, 12, 12);
      Script_EffectUtils(8, arg1, 10.0f, 7.29411745f);
      Script_EffectUtils(16, arg1, -0.541176498f);
      Script_EffectUtils(11, arg1, 111);
      Script_EffectUtils(12, arg1, 19);
      Script_EffectUtils(10, arg1, 0.358823538f);
      break;
    case 7:
      local_F4.x = 0.0f;
      (&local_F4)->y = 0.0827450976f;
      (&local_F4)->z = 0.0313725509f;
      task_A0.x = 0.0541176461f;
      task_A0.y = 0.00431372551f;
      task_A0.z = 0.0227450989f;
      Script_EffectUtils(6, arg1, &local_F4, &task_A0);
      Script_EffectUtils(7, arg1, 0.00980392192f);
      Script_EffectUtils(9, arg1, 8, 8, 8);
      Script_EffectUtils(8, arg1, 10.0f, 7.29411745f);
      Script_EffectUtils(16, arg1, -0.541176498f);
      Script_EffectUtils(11, arg1, 111);
      Script_EffectUtils(12, arg1, 19);
      Script_EffectUtils(10, arg1, 0.358823538f);
      break;
    case 8:
      local_F4.x = 0.0f;
      (&local_F4)->y = 0.0827450976f;
      (&local_F4)->z = 0.1f;
      task_A0.x = 0.097647056f;
      task_A0.y = 0.0552941188f;
      task_A0.z = 0.0203921571f;
      Script_EffectUtils(6, arg1, &local_F4, &task_A0);
      Script_EffectUtils(7, arg1, 0.00980392192f);
      Script_EffectUtils(9, arg1, 50, 50, 50);
      Script_EffectUtils(8, arg1, 10.0f, 8.313725f);
      Script_EffectUtils(10, arg1, 0.433333337f);
      Script_EffectUtils(11, arg1, 119);
      Script_EffectUtils(12, arg1, 25);
      Script_EffectUtils(16, arg1, -0.376470596f);
      break;
    case 9:
      local_F4.x = 0.0f;
      (&local_F4)->y = 0.0827450976f;
      (&local_F4)->z = 0.0313725509f;
      task_A0.x = 0.0196078438f;
      task_A0.y = 0.0647058859f;
      task_A0.z = 0.0196078438f;
      Script_EffectUtils(6, arg1, &local_F4, &task_A0);
      Script_EffectUtils(7, arg1, 0.0250980388f);
      Script_EffectUtils(9, arg1, 38, 38, 38);
      Script_EffectUtils(8, arg1, 10.0f, 8.274509f);
      Script_EffectUtils(10, arg1, 0.433333337f);
      Script_EffectUtils(11, arg1, 127);
      Script_EffectUtils(12, arg1, 20);
      Script_EffectUtils(16, arg1, -0.376470596f);
      break;
    case 10:
      local_F4.x = 0.0f;
      (&local_F4)->y = 0.0827450976f;
      (&local_F4)->z = 0.1f;
      task_A0.x = 0.0196078438f;
      task_A0.y = 0.0549019612f;
      task_A0.z = 0.0196078438f;
      Script_EffectUtils(6, arg1, &local_F4, &task_A0);
      Script_EffectUtils(7, arg1, 0.0250980388f);
      Script_EffectUtils(9, arg1, 21, 21, 21);
      Script_EffectUtils(8, arg1, 10.0f, 8.313725f);
      Script_EffectUtils(10, arg1, 0.433333337f);
      Script_EffectUtils(11, arg1, 119);
      Script_EffectUtils(12, arg1, 12);
      Script_EffectUtils(16, arg1, -0.376470596f);
      break;
    case 11:
      local_F4.x = 0.0f;
      (&local_F4)->y = 0.0827450976f;
      (&local_F4)->z = 0.1f;
      task_A0.x = 0.0196078438f;
      task_A0.y = 0.0549019612f;
      task_A0.z = 0.0196078438f;
      Script_EffectUtils(6, arg1, &local_F4, &task_A0);
      Script_EffectUtils(7, arg1, 0.0250980388f);
      Script_EffectUtils(9, arg1, 21, 21, 21);
      Script_EffectUtils(8, arg1, 10.0f, 8.313725f);
      Script_EffectUtils(10, arg1, 0.433333337f);
      Script_EffectUtils(11, arg1, 119);
      Script_EffectUtils(12, arg1, 12);
      Script_EffectUtils(16, arg1, -0.376470596f);
      break;
    case 12:
      local_F4.x = 0.0f;
      (&local_F4)->y = 0.0827450976f;
      (&local_F4)->z = 0.0313725509f;
      task_A0.x = 0.0494117662f;
      task_A0.y = 0.00431372551f;
      task_A0.z = 0.0231372546f;
      Script_EffectUtils(6, arg1, &local_F4, &task_A0);
      Script_EffectUtils(7, arg1, 0.0101960786f);
      Script_EffectUtils(9, arg1, 56, 56, 56);
      Script_EffectUtils(8, arg1, 10.0f, 7.29411745f);
      Script_EffectUtils(16, arg1, -0.541176498f);
      Script_EffectUtils(11, arg1, 107);
      Script_EffectUtils(12, arg1, 19);
      Script_EffectUtils(10, arg1, 0.358823538f);
      break;
    case 13:
      local_F4.x = 0.0f;
      (&local_F4)->y = 0.0827450976f;
      (&local_F4)->z = 0.1f;
      task_A0.x = 0.097647056f;
      task_A0.y = 0.0552941188f;
      task_A0.z = 0.0203921571f;
      Script_EffectUtils(6, arg1, &local_F4, &task_A0);
      Script_EffectUtils(7, arg1, 0.00980392192f);
      Script_EffectUtils(9, arg1, 0, 0, 0);
      Script_EffectUtils(8, arg1, 10.0f, 8.313725f);
      Script_EffectUtils(10, arg1, 0.433333337f);
      Script_EffectUtils(11, arg1, 119);
      Script_EffectUtils(12, arg1, 25);
      Script_EffectUtils(16, arg1, -0.376470596f);
      break;
    case 20:
      local_F4.x = 0.0f;
      (&local_F4)->y = 0.0827450976f;
      (&local_F4)->z = 0.0313725509f;
      task_A0.x = 0.0298039205f;
      task_A0.y = 0.00431372551f;
      task_A0.z = 0.0235294122f;
      Script_EffectUtils(6, arg1, &local_F4, &task_A0);
      Script_EffectUtils(7, arg1, 0.00980392192f);
      Script_EffectUtils(9, arg1, 23, 23, 23);
      Script_EffectUtils(8, arg1, 10.0f, 7.29411745f);
      Script_EffectUtils(16, arg1, -0.541176498f);
      Script_EffectUtils(11, arg1, 111);
      Script_EffectUtils(12, arg1, 19);
      Script_EffectUtils(10, arg1, 0.358823538f);
      break;
  }
  return;
}

static void script_60F1(int32_t arg0, int32_t arg1) {
  int32_t unused_F4[3];
  return;
}

static int32_t script_60F4(int32_t arg0) {
  while (1) {
    {
      Vector3 local_F4;
      Script_EVT_GetPosition(SHEN_NAME('SHIP'), -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
      local_F4.x = (&local_F4)->x - 4.0f;
      (&local_F4)->y = (&local_F4)->y + 16.5f;
      Script_EffectUtils(5, arg0, &local_F4, &task_A0);
    }
    SCNF_WaitFrames_0000(1);
    {
      Vector3 local_F4;
      Script_EVT_GetPosition(SHEN_NAME('SHIP'), -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
      local_F4.x = (&local_F4)->x + 5.7f;
      (&local_F4)->y = (&local_F4)->y + 16.2f;
      (&local_F4)->z = (&local_F4)->z - 0.7f;
      Script_EffectUtils(5, arg0, &local_F4, &task_A0);
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

__attribute__((scn_string_bindings(0, data_18FFC, 2, data_19016, 3, data_19023, 4, data_1902E, 6, data_19048)))
static void script_61A3(int32_t arg0) {
  Script_MT5Loader_TRWV_TRWAVE(9, arg0, 0.117647059f, 0.0901960805f, 0.470588237f);
  Script_MT5Loader_TRWV_TRWAVE(300, arg0, "model/object", "siranami.pvr", "model/object", "trsea4.mt6");
  Script_MT5Loader_TRWV_TRWAVE(310, arg0, "model/object", "ALPHTBL2.BIN");
  Script_MT5Loader_TRWV_TRWAVE(312, arg0, 4, 4);
  Script_MT5Loader_TRWV_TRWAVE(10, arg0, 64.0f, 64.0f, 1, 1, 1, 1);
  Script_MT5Loader_TRWV_TRWAVE(17, arg0, "model/object", "griddata.bin");
  Script_MT5Loader_TRWV_TRWAVE(23, arg0, &data_18FF0);
  Script_MT5Loader_TRWV_TRWAVE(13, arg0, 0.137254909f, 0.282352954f);
  Script_MT5Loader_TRWV_TRWAVE(14, arg0, -0.0137254894f, 0.119607866f);
  Script_MT5Loader_TRWV_TRWAVE(16, arg0, 0, -8);
  Script_MT5Loader_TRWV_TRWAVE(311, arg0, 0, -3);
  Script_MT5Loader_TRWV_TRWAVE(21, arg0, 8, 8);
  return;
}

__attribute__((scn_string_bindings(0, data_19070)))
static void script_6288(int32_t arg0) {
  Script_MT5Loader_TRWV_TRWAVE(200, arg0, &data_19064, "model/object", "shibu.pvr");
  Script_MT5Loader_TRWV_TRWAVE(210, arg0, 62, 255, 255, 255);
  return;
}

static void script_62B9(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  local_FC = arg0;
  switch (local_FC) {
    case 0:
      Script_MT5Loader_TRWV_TRWAVE(11, arg1, -0.19f, -0.51f, -0.84f);
      Script_MT5Loader_TRWV_TRWAVE(10, arg1, 64.0f, 64.0f, 1, 1, 1, 1);
      break;
    case 1:
      Script_MT5Loader_TRWV_TRWAVE(11, arg1, -0.75f, -0.06f, -0.66f);
      Script_MT5Loader_TRWV_TRWAVE(10, arg1, 64.0f, 64.0f, 3, 1, 3, 2);
      break;
    case 2:
      Script_MT5Loader_TRWV_TRWAVE(11, arg1, 0.85f, -0.32f, -0.43f);
      Script_MT5Loader_TRWV_TRWAVE(10, arg1, 64.0f, 64.0f, 1, 1, 1, 1);
      break;
    case 3:
      Script_MT5Loader_TRWV_TRWAVE(11, arg1, -0.94f, 0.0f, -1.0f);
      Script_MT5Loader_TRWV_TRWAVE(10, arg1, 64.0f, 64.0f, 1, 1, 1, 1);
      break;
    case 4:
      Script_MT5Loader_TRWV_TRWAVE(11, arg1, -0.75f, 0.08f, -0.66f);
      Script_MT5Loader_TRWV_TRWAVE(10, arg1, 64.0f, 64.0f, 1, 1, 1, 1);
      break;
    case 5:
      Script_MT5Loader_TRWV_TRWAVE(11, arg1, 0.61f, 0.04f, -0.79f);
      Script_MT5Loader_TRWV_TRWAVE(10, arg1, 64.0f, 64.0f, 1, 1, 1, 1);
      break;
    case 6:
      Script_MT5Loader_TRWV_TRWAVE(11, arg1, -0.94f, 0.0f, -1.0f);
      Script_MT5Loader_TRWV_TRWAVE(10, arg1, 64.0f, 64.0f, 1, 0, 0, 2);
      break;
    case 7:
      Script_MT5Loader_TRWV_TRWAVE(11, arg1, -0.94f, 0.16f, 0.3f);
      Script_MT5Loader_TRWV_TRWAVE(10, arg1, 64.0f, 64.0f, 2, 1, 2, 1);
      break;
    case 8:
      Script_MT5Loader_TRWV_TRWAVE(11, arg1, 0.6f, 0.18f, 0.74f);
      Script_MT5Loader_TRWV_TRWAVE(10, arg1, 64.0f, 64.0f, 1, 4, 4, 1);
      break;
    case 9:
      Script_MT5Loader_TRWV_TRWAVE(11, arg1, -1.0f, 0.2f, 0.65f);
      Script_MT5Loader_TRWV_TRWAVE(10, arg1, 64.0f, 64.0f, 0, 6, 2, 2);
      break;
    case 10:
      Script_MT5Loader_TRWV_TRWAVE(11, arg1, -0.53f, 0.19f, 0.68f);
      Script_MT5Loader_TRWV_TRWAVE(10, arg1, 64.0f, 64.0f, 2, 1, 2, 2);
      break;
    case 11:
      Script_MT5Loader_TRWV_TRWAVE(11, arg1, -1.0f, 0.14f, 0.43f);
      Script_MT5Loader_TRWV_TRWAVE(10, arg1, 64.0f, 64.0f, 1, 1, 2, 5);
      break;
    case 12:
      Script_MT5Loader_TRWV_TRWAVE(11, arg1, -0.94f, 0.0f, -1.0f);
      Script_MT5Loader_TRWV_TRWAVE(10, arg1, 64.0f, 64.0f, 3, 0, 0, 4);
      break;
    case 20:
      Script_MT5Loader_TRWV_TRWAVE(11, arg1, -0.94f, 0.0f, -1.0f);
      Script_MT5Loader_TRWV_TRWAVE(10, arg1, 64.0f, 64.0f, 2, 0, 1, 3);
      break;
  }
  return;
}

static void script_6628(int32_t arg0) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t local_E4;
  local_E4 = task_7C;
  switch (local_E4) {
    case 0:
      Script_EVT_GetPosition(SHEN_NAME('SHIP'), -1, &local_F4, 0);
      (&local_F4)->y = -1.8f;
      Script_MT5Loader_TRWV_TRWAVE(5, arg0, &local_F4);
      Script_mtwkGetSetRotation(SHEN_NAME('SHIP'), &local_E8, 0);
      Script_MT5Loader_TRWV_TRWAVE(12, arg0, (&local_E8)->y);
      break;
    case 1:
      local_F4.x = 555.11261f;
      (&local_F4)->y = -1.8f;
      (&local_F4)->z = -4.789f;
      Script_MT5Loader_TRWV_TRWAVE(5, arg0, &local_F4);
      Script_mtwkGetSetRotation(SHEN_NAME('SHIP'), &local_E8, 0);
      Script_MT5Loader_TRWV_TRWAVE(12, arg0, (&local_E8)->y);
      task_7C = 0;
      break;
    case 2:
      local_F4.x = 482.363007f;
      (&local_F4)->y = -1.8f;
      (&local_F4)->z = -4.789f;
      Script_MT5Loader_TRWV_TRWAVE(5, arg0, &local_F4);
      Script_mtwkGetSetRotation(SHEN_NAME('SHIP'), &local_E8, 0);
      Script_MT5Loader_TRWV_TRWAVE(12, arg0, (&local_E8)->y);
      task_7C = 0;
      break;
  }
  return;
}

static int32_t script_6736(void) {
  Vector3i local_F4;
  {
    Vector3 local_E8;
    int32_t local_E4;
    int32_t local_E0;
    local_E4 = 1;
    local_F4.x = 0;
    (&local_F4)->y = 36408;
    (&local_F4)->z = 0;
    Script_charaPosRotUtils(SHEN_NAME('STR_'), 152, 0, &local_F4, 0);
    local_F4.x = 0;
    (&local_F4)->y = 161;
    (&local_F4)->z = 0;
    script_6972();
    local_E8.x = 0.0f;
    (&local_E8)->y = 0.0f;
    (&local_E8)->z = 0.0f;
    Script_charaPosRotUtils(SHEN_NAME('STR_'), 152, &local_E8, 0, 3);
    local_E0 = 0x43480000;
    while (local_E4 <= 180) {
      if (local_E4 <= 15) {
        local_E8.x = (&local_E8)->x + 0.113333337f;
        (&local_E8)->y = (&local_E8)->y + 0.113333337f;
        (&local_E8)->z = (&local_E8)->z + 0.113333337f;
        Script_charaPosRotUtils(SHEN_NAME('STR_'), 152, &local_E8, 0, 3);
      } else if (local_E4 <= 25) {
      } else if (local_E4 <= 40) {
        local_E8.x = (&local_E8)->x - 0.0300000031f;
        (&local_E8)->y = (&local_E8)->y - 0.0300000031f;
        (&local_E8)->z = (&local_E8)->z - 0.0300000031f;
        Script_charaPosRotUtils(SHEN_NAME('STR_'), 152, &local_E8, 0, 3);
      } else {
        if (local_E4 <= 60) {
          local_E8.x = (&local_E8)->x + 0.0875f;
          (&local_E8)->y = (&local_E8)->y + 0.0875f;
          (&local_E8)->z = (&local_E8)->z + 0.0875f;
          Script_charaPosRotUtils(SHEN_NAME('STR_'), 152, &local_E8, 0, 3);
          goto L0068CA;
        }
      }
      L0068CA:;
      if (local_E4 >= 120) {
        local_E8.x = (&local_E8)->x - 0.05f;
        (&local_E8)->y = (&local_E8)->y - 0.05f;
        (&local_E8)->z = (&local_E8)->z - 0.05f;
        Script_charaPosRotUtils(SHEN_NAME('STR_'), 152, &local_E8, 0, 3);
      }
      Script_charaPosRotUtils(SHEN_NAME('STR_'), 152, 0, &local_F4, 1);
      local_E4 = local_E4 + 1;
      SCNF_WaitFrames_0000(1);
    }
  }
  {
    Vector3 local_E8;
    local_E8.x = 0.0f;
    (&local_E8)->y = 0.0f;
    (&local_E8)->z = 0.0f;
    Script_charaPosRotUtils(SHEN_NAME('STR_'), 152, &local_E8, 0, 3);
  }
  return Script_srchControl(0);
}

static void script_6972(void) {
  int32_t unused_F4[3];
  Vector3 local_E8;
  local_E8.x = 0.0f;
  (&local_E8)->y = 0.0f;
  (&local_E8)->z = 0.0f;
  Script_charaPosRotUtils(SHEN_NAME('STR_'), 152, &local_E8, 0, 3);
  Script_charaDisplay(SHEN_NAME('STR_'), CDM_Show);
  return;
}

static int32_t script_69AB(void) {
  float local_FC;
  float local_F8;
  local_FC = 0.0f;
  local_F8 = 1.0f;
  for (;;) {
    if (local_FC >= 2200.0f) {
      break;
    }
    Script_EDTKUtils(3, local_FC, 253.0f);
    local_FC = local_FC + local_F8;
    SCNF_WaitFrames_0000(1);
  }
  return Script_srchControl(0);
}

__attribute__((scn_string_bindings(0, data_19364, 2, data_1937A, 3, data_19387, 4, data_19392, 5, data_1939F, 8, data_193C2, 9, data_193C7, 10, data_193CC, 11, data_193D1, 12, data_193D6, 13, data_193DB, 14, data_193E0, 16, data_193F0, 17, data_193F7, 18, data_19404, 21, data_1941C, 22, data_19421, 23, data_19426, 24, data_1942B, 25, data_19430, 26, data_19435, 27, data_1943A)))
static void script_69F1(int32_t arg0) {
  int32_t local_FC;
  int32_t unused_F8;
  int32_t local_F4;
  int32_t local_F0;
  int32_t unused_EC;
  int32_t local_E8;
  Vector3 local_DC;
  int32_t local_D8;
  script_121D();
  script_844C();
  Script_EVT_GetPosition(SHEN_NAME('SHIP'), -1, &local_DC, 0);
  local_E8 = Script_MT5Loader_TRWV_TRWAVE(0, &local_DC, "model/object", "sea2.pvr", "model/object", "trsea4.mt6", 4.0f, 0);
  Script_MT5Loader_TRWV_TRWAVE(21, local_E8, 4, 4);
  Script_MT5Loader_TRWV_TRWAVE(16, local_E8, -1, 0);
  Script_MT5Loader_TRWV_TRWAVE(13, local_E8, 0.0901960805f, 0.196078435f);
  Script_MT5Loader_TRWV_TRWAVE(15, local_E8, "model/object", "TRWTBL1.BIN");
  Script_MT5Loader_TRWV_TRWAVE(12, local_E8, 0x4000);
  Script_MT5Loader_TRWV_TRWAVE(11, local_E8, -0.94f, 0.0f, -1.0f);
  Script_MT5Loader_TRWV_TRWAVE(10, local_E8, 64.0f, 64.0f, 2, 0, 1, 3);
  Script_MT5Loader_TRWV_TRWAVE(1, local_E8);
  local_FC = Script_EffectUtils(0, &data_1934C, &data_19358, 180, 0.0184313729f, 0, 0);
  Script_EffectUtils(1, local_FC);
  script_55C6(0, local_FC);
  local_F4 = Script_scenCreateTask(script_60F4, 1, local_FC);
  Script_EDTKUtils(0, 0, 0);
  Script_EDTKUtils(1, 0, 0);
  local_D8 = arg0;
  if (local_D8 == 0) {
    script_6FCA(local_E8, local_FC);
  } else if (local_D8 == 1) {
    script_7028(local_E8, local_FC);
  } else if (local_D8 == 2) {
    script_7090(local_E8, local_FC);
  } else if (local_D8 == 3) {
    script_70EE(local_E8, local_FC);
  } else if (local_D8 == 4) {
    script_714C(local_E8, local_FC);
  } else if (local_D8 == 5) {
    script_71AA(local_E8, local_FC);
  } else if (local_D8 == 6) {
    script_7314(local_E8, local_FC);
  } else if (local_D8 == 7) {
    script_742B(local_E8, local_FC);
  } else if (local_D8 == 8) {
    script_758E(local_E8, local_FC);
  } else if (local_D8 == 9) {
    script_760F(local_E8, local_FC);
  } else if (local_D8 == 10) {
    script_766D(local_E8, local_FC);
  } else if (local_D8 == 11) {
    script_76CB(local_E8, local_FC);
  } else if (local_D8 == 12) {
    script_7899(local_E8, local_FC);
  } else if (local_D8 == 14) {
    script_79A2(local_E8, local_FC);
  } else if (local_D8 == 15) {
    script_7DC4(local_E8, local_FC);
  } else if (local_D8 == 16) {
    script_8152(local_E8, local_FC);
  } else {
    if ((local_D8 == 13) | (local_D8 == 17)) {
      L006C9D:;
      if (1) {
        local_F0 = Script_motiLoad(3, SHEN_NAME('MEND'), "M_0180.BIN");
        script_8152(local_E8, local_FC);
        Script_ReallocateUNFO(local_F0);
        local_F0 = Script_motiLoad(3, SHEN_NAME('MEND'), "M_01156.BIN");
        Script_LoadSND(0, 7, "FREE", 0);
        Script_LoadSND(0, 6, "FREE", 0);
        Script_LoadSND(0, 5, "FREE", 0);
        Script_LoadSND(0, 4, "FREE", 0);
        Script_LoadSND(0, 3, "FREE", 0);
        Script_LoadSND(0, 2, "FREE", 0);
        Script_LoadSND(0, 1, "FREE", 0);
        Script_LoadSND(0, 4, "a1_end.snd", 0);
        script_61A3(local_E8);
        script_6288(local_E8);
        Script_WeatherUtils_ForFIXC(13, 0);
        Script_WeatherUtils_ForFIXC(14, 0);
        Script_SpriteUtils(Scroll_Read2, (int32_t)"sprite", "SCROLL27.SPR", 1);
        Script_SpriteUtils(Scroll_Read2, (int32_t)"sprite", "SCROLL05.SPR", 2);
        Script_ProcessSubtitle("END", 0);
        Script_scenCreateTask(script_1312, 0);
        script_6FCA(local_E8, local_FC);
        if (Script_shenSeqUtils(-7)) {
        } else {
          script_7028(local_E8, local_FC);
          if (Script_shenSeqUtils(-7)) {
          } else {
            script_70EE(local_E8, local_FC);
            if (Script_shenSeqUtils(-7)) {
            } else {
              script_7090(local_E8, local_FC);
              if (Script_shenSeqUtils(-7)) {
              } else {
                script_714C(local_E8, local_FC);
                if (Script_shenSeqUtils(-7)) {
                } else {
                  script_71AA(local_E8, local_FC);
                  if (Script_shenSeqUtils(-7)) {
                  } else {
                    script_7314(local_E8, local_FC);
                    if (Script_shenSeqUtils(-7)) {
                    } else {
                      script_7899(local_E8, local_FC);
                      if (Script_shenSeqUtils(-7)) {
                      } else {
                        script_742B(local_E8, local_FC);
                        if (Script_shenSeqUtils(-7)) {
                        } else {
                          Script_scenCreateTask(script_69AB, 0);
                          script_758E(local_E8, local_FC);
                          if (Script_shenSeqUtils(-7)) {
                          } else {
                            script_760F(local_E8, local_FC);
                            if (Script_shenSeqUtils(-7)) {
                            } else {
                              task_7C = 1;
                              script_766D(local_E8, local_FC);
                              if (Script_shenSeqUtils(-7)) {
                              } else {
                                task_7C = 2;
                                script_79A2(local_E8, local_FC);
                                goto L006F00;
                                goto L006C9D;
                              }
                            }
                          }
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
      L006F00:;
      goto L006F03;
    }
  }
  L006F03:;
  if (Script_shenSeqUtils(-7)) {
    Script_filmCheckOrSetTransitionUtil(1, 0, 0, 0, 0, 255, 0, 0, 0);
    while (Script_filmCheckOrSetTransitionUtil(-1)) {
      SCNF_WaitFrames_0000(1);
    }
  } else {
    Script_filmCheckOrSetTransitionUtil(60, 0, 0, 0, 0, 255, 0, 0, 0);
    while (Script_filmCheckOrSetTransitionUtil(-1)) {
      SCNF_WaitFrames_0000(1);
    }
  }
  Script_shenSeqUtils(-6);
  Script_LoadSND_Wrapper(0, "FREE", "FREE", "FREE", "FREE", "FREE", "FREE", "FREE");
  Script_MT5Loader_TRWV_TRWAVE(203, local_E8);
  Script_MT5Loader_TRWV_TRWAVE(3, local_E8);
  Script_EDTKUtils(2, 0, 0);
  Script_EffectUtils(3, local_FC);
  Script_ReallocateUNFO(local_F0);
  Script_srchControl((void *)local_F4);
  return;
}

static void script_6FCA(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  script_4B5C();
  script_62B9(0, arg0);
  script_55C6(0, arg1);
  local_FC = 0;
  Script_shenSeqUtils(0);
  while (Script_shenSeqUtils(-1)) {
    local_FC = local_FC + 1;
    script_60F1(arg1, local_FC % 2);
    script_6628(arg0);
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_7028(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  script_4BEE();
  script_62B9(1, arg0);
  script_55C6(1, arg1);
  local_FC = 0;
  Script_shenSeqUtils(1);
  while (Script_shenSeqUtils(-1)) {
    if (!(local_FC == 170)) goto L007064;
    L007064:;
    local_FC = local_FC + 1;
    script_60F1(arg1, local_FC % 2);
    script_6628(arg0);
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_7090(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  script_4C7E();
  script_62B9(2, arg0);
  script_55C6(2, arg1);
  local_FC = 0;
  Script_shenSeqUtils(2);
  while (Script_shenSeqUtils(-1)) {
    local_FC = local_FC + 1;
    script_60F1(arg1, local_FC % 2);
    script_6628(arg0);
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_70EE(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  script_4D19();
  script_62B9(3, arg0);
  script_55C6(3, arg1);
  local_FC = 0;
  Script_shenSeqUtils(3);
  while (Script_shenSeqUtils(-1)) {
    script_60F1(arg1, local_FC % 2);
    local_FC = local_FC + 1;
    script_6628(arg0);
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_714C(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  script_4DB4();
  script_62B9(4, arg0);
  script_55C6(4, arg1);
  local_FC = 0;
  Script_shenSeqUtils(4);
  while (Script_shenSeqUtils(-1)) {
    script_60F1(arg1, local_FC % 2);
    local_FC = local_FC + 1;
    script_6628(arg0);
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_71AA(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  Vector3 local_F0;
  Vector3 local_E4;
  local_E4.x = -200.0f;
  (&local_E4)->y = 0.0f;
  (&local_E4)->z = 0.0f;
  Script_EffectUtils(50, arg1, &local_E4);
  local_F0.x = -78.5f;
  (&local_F0)->y = 0.0f;
  (&local_F0)->z = 0.4f;
  Script_SetDynamicMapTransform(0, &local_F0, 0);
  Script_SetDynamicMapTransform(1, &local_F0, 0);
  Script_SetDynamicMapTransform(2, &local_F0, 0);
  Script_SetDynamicMapTransform(3, &local_F0, 0);
  Script_SetDynamicMapTransform(4, &local_F0, 0);
  Script_SetDynamicMapTransform(5, &local_F0, 0);
  Script_SetDynamicMapTransform(6, &local_F0, 0);
  Script_SetDynamicMapTransform(7, &local_F0, 0);
  script_4E4F();
  script_62B9(5, arg0);
  script_55C6(5, arg1);
  local_FC = 0;
  Script_shenSeqUtils(5);
  while (Script_shenSeqUtils(-1)) {
    script_60F1(arg1, local_FC % 2);
    local_FC = local_FC + 1;
    script_6628(arg0);
    SCNF_WaitFrames_0000(1);
  }
  local_F0.x = 0.0f;
  (&local_F0)->y = 0.0f;
  (&local_F0)->z = 0.0f;
  Script_SetDynamicMapTransform(0, &local_F0, 0);
  Script_SetDynamicMapTransform(1, &local_F0, 0);
  Script_SetDynamicMapTransform(2, &local_F0, 0);
  Script_SetDynamicMapTransform(3, &local_F0, 0);
  Script_SetDynamicMapTransform(4, &local_F0, 0);
  Script_SetDynamicMapTransform(5, &local_F0, 0);
  Script_SetDynamicMapTransform(6, &local_F0, 0);
  Script_SetDynamicMapTransform(7, &local_F0, 0);
  return;
}

static void script_7314(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  int32_t local_F8;
  script_4EEA();
  script_62B9(6, arg0);
  script_55C6(6, arg1);
  local_FC = 0;
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 50.0f);
  Script_shenSeqUtils(6);
  Script_tmnmUtils(SHEN_NAME('KAM1'), 4113, 0.0f, 32767.0f, 0.0f, 1.0f, 0);
  Script_tmnmUtils(SHEN_NAME('KAM1'), 0, -1.0f, -1.0f, -1.0f, 32767.0f, 4);
  while (Script_shenSeqUtils(-1)) {
    local_F8 = local_FC;
    switch (local_F8) {
      case 350:
        Script_charaDisplay(CHARA_AKIR, CDM_Show);
        Script_MT5Loader_TRWV_TRWAVE(10, arg0, 64.0f, 64.0f, 1, 1, 1, 2);
        Script_tmnmReset(SHEN_NAME('KAM1'));
        break;
      case 689:
        Script_FaceEx(CHARA_AKIR, 1, 2, 1);
        Script_FACE_LCG(CHARA_AKIR, 2, 1, 0);
        break;
    }
    local_FC = local_FC + 1;
    script_6628(arg0);
    script_60F1(arg1, local_FC % 2);
    SCNF_WaitFrames_0000(1);
  }
  return;
}

__attribute__((scn_string_bindings(0, data_19450)))
static void script_742B(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  int32_t local_F8;
  int8_t local_F7;
  (void)((local_F7 = 0) & 0x20);
  (void)1;
  Script_SetCharaRefBoardPos(CHARA_TAIJ, &data_19440);
  Script_charaSetRefboardFlagTo(CHARA_TAIJ, 1);
  script_4F82();
  script_62B9(7, arg0);
  script_55C6(7, arg1);
  local_F8 = 60;
  local_FC = 0;
  Script_shenSeqUtils(7);
  Script_FaceEx(CHARA_TAIJ, 1, 0, 1);
  Script_SetDynamicMapStaticSceneMode(12, 0);
  while (Script_shenSeqUtils(-1)) {
    if (local_FC == 160) {
      Script_WeatherUtils_0(WeatherDataUtil_SetInterpolatedWeatherOverride, 1, 0, 0x200, 0);
      Script_WeatherUtils_0(WeatherDataUtil_SetInterpolatedWeatherOverride, 2, 0, 0x200, 0);
    }
    if (local_FC == 170) {
      Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 150.0f);
      Script_MT5Loader_TRWV_TRWAVE(10, arg0, 64.0f, 64.0f, 1, 1, 2, 3);
    }
    if (local_FC == 262) {
      Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "ENDMAP");
      Script_mapmSetRenderFlag(12, 0);
      (void)((local_F7 = -1) & 0x20);
    }
    if (local_FC == 300) {
      Script_SpriteUtils(ModifySky, 0, 2, 800);
    }
    script_60F1(arg1, local_FC % 2);
    local_FC = local_FC + 1;
    script_6628(arg0);
    SCNF_WaitFrames_0000(1);
  }
  Script_charaSetRefboardFlagTo(CHARA_TAIJ, 0);
  Script_SetDynamicMapStaticSceneMode(12, 1);
  Script_FaceEx(CHARA_TAIJ, 0, 0, 1);
  return;
}

static void script_758E(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  int32_t local_F8;
  script_501A();
  script_62B9(8, arg0);
  script_55C6(8, arg1);
  local_FC = 0;
  Script_shenSeqUtils(8);
  while (Script_shenSeqUtils(-1)) {
    local_F8 = local_FC;
    if (local_F8 == 651) {
      script_5498();
      script_55C6(13, arg1);
      goto L0075E3;
    }
    L0075E3:;
    script_60F1(arg1, local_FC % 2);
    local_FC = local_FC + 1;
    script_6628(arg0);
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_760F(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  script_5135();
  script_62B9(9, arg0);
  script_55C6(9, arg1);
  local_FC = 0;
  Script_shenSeqUtils(9);
  while (Script_shenSeqUtils(-1)) {
    script_60F1(arg1, local_FC % 2);
    local_FC = local_FC + 1;
    script_6628(arg0);
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_766D(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  script_522A();
  script_62B9(10, arg0);
  script_55C6(10, arg1);
  Script_shenSeqUtils(10);
  local_FC = 0;
  while (Script_shenSeqUtils(-1)) {
    script_60F1(arg1, local_FC % 2);
    local_FC = local_FC + 1;
    script_6628(arg0);
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_76CB(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  float local_F8;
  script_5315();
  script_62B9(11, arg0);
  script_55C6(11, arg1);
  Script_SetCharaRefBoardPos(CHARA_SINF, &data_19458);
  local_F8 = 1.0f;
  Script_charaSetRenderOpacity(SHEN_NAME('CVGF'), local_F8);
  Script_InitializeOsag(CHARA_SINF);
  Script_charaDisplay(SHEN_NAME('CVGF'), CDM_Show);
  local_FC = 0;
  Script_shenSeqUtils(11);
  while (Script_shenSeqUtils(-1)) {
    if (!(local_FC == 136)) goto L007740;
    L007740:;
    if (local_FC == 780) {
      Script_charaDisplay(CHARA_SINF, CDM_Show);
      Script_SetMITKFlags(StartFadeIn, 9, 1);
      Script_InitializeOsag(CHARA_SINF);
      Script_lightSetGlobalSceneLighting(&data_19468);
      Script_lightDirLightEnable(0, &data_19488);
      Script_LightUtils(0, 2, &data_19478);
      Script_LightUtils(0, 4, 1.0f);
      Script_lightSetSlotEnabled(0, 1);
    }
    if ((local_FC >= 781) & (local_FC <= 820)) {
      local_F8 = local_F8 - 0.025f;
      Script_charaSetRenderOpacity(SHEN_NAME('CVGF'), local_F8);
    }
    if (local_FC == 780) {
      Script_FACE(CHARA_SINF, 9, 0, 16);
    }
    if (local_FC == 790) {
      Script_FACE(CHARA_SINF, 1, 0, 16);
    }
    if (local_FC == 800) {
      Script_FACE(CHARA_SINF, 0, 0, 16);
    }
    if ((local_FC >= 891) & (local_FC <= 930)) {
      local_F8 = local_F8 + 0.025f;
      Script_charaSetRenderOpacity(SHEN_NAME('CVGF'), local_F8);
    }
    script_60F1(arg1, local_FC % 2);
    local_FC = local_FC + 1;
    script_6628(arg0);
    SCNF_WaitFrames_0000(1);
  }
  Script_SetMITKFlags(StartFadeOut, 0, 1);
  Script_charaDisplay(SHEN_NAME('CVGF'), CDM_Hide);
  Script_charaDisplay(CHARA_SINF, CDM_Hide);
  Script_charaSetRefboardFlagTo(CHARA_SINF, 0);
  return;
}

static void script_7899(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  {
    Vector3 local_F0;
    local_F0.x = 20.0f;
    (&local_F0)->y = 0.0f;
    (&local_F0)->z = -23.0f;
    Script_SetDynamicMapTransform(7, &local_F0, 6553);
  }
  script_53F7();
  script_62B9(12, arg0);
  script_55C6(12, arg1);
  (void)1;
  Script_SetCharaRefBoardPos(CHARA_AKIR, &data_19494);
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 1);
  local_FC = 0;
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 50.0f);
  Script_shenSeqUtils(12);
  while (Script_shenSeqUtils(-1)) {
    script_60F1(arg1, local_FC % 2);
    local_FC = local_FC + 1;
    script_6628(arg0);
    SCNF_WaitFrames_0000(1);
  }
  {
    Vector3 local_F0;
    local_F0.x = 0.0f;
    (&local_F0)->y = 0.0f;
    (&local_F0)->z = 0.0f;
    Script_SetDynamicMapTransform(7, &local_F0, 0);
  }
  Script_SetDynamicMapStaticSceneMode(7, 1);
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 0);
  (void)0;
  Script_FaceEx(CHARA_AKIR, 0, 0, 1);
  Script_faceUpdateFor(CHARA_AKIR, 0, 0);
  return;
}

static void script_79A2(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  int32_t local_F8;
  float local_F4;
  Vector3 local_E8;
  float local_E4;
  int32_t local_E0;
  script_5315();
  script_62B9(11, arg0);
  script_55C6(11, arg1);
  (void)0;
  Script_SpriteUtils(ScrollHorizontalOffset, 2592);
  Script_SpriteUtils(ScrollVerticalOffset, 39);
  Script_charaDisplay(CHARA_SINF, CDM_Hide);
  Script_charaDisplay(SHEN_NAME('STR_'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('CVGF'), CDM_Show);
  local_FC = 0;
  local_F4 = 0.0f;
  local_E4 = 0.0f;
  local_E8.x = 521.6f;
  (&local_E8)->y = 11.4711f;
  (&local_E8)->z = 7.129f;
  Script_EVT_SetPosition(SHEN_NAME('STR_'), 0x78000000, &local_E8);
  Script_shenSeqUtils(13);
  local_F8 = 64;
  Script_charaSetRenderOpacity(SHEN_NAME('CVGF'), 0.0f);
  Script_charaSetRenderOpacity(SHEN_NAME('SCRT'), 0.0f);
  Script_charaDisplay(SHEN_NAME('SCRT'), CDM_Hide);
  while (Script_shenSeqUtils(-1)) {
    local_E0 = local_FC;
    switch (local_E0) {
      case 410:
        Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 40.0f);
        Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 0.0f);
        Script_MT5Loader_TRWV_TRWAVE(10, arg0, 64.0f, 64.0f, 0, 0, 0, 0);
        break;
      case 416:
        Script_FaceEx(CHARA_SINF, 1, 0, 1);
        Script_lightSetGlobalSceneLighting(&data_194A4);
        Script_lightDirLightEnable(0, &data_194C4);
        Script_LightUtils(0, 2, &data_194B4);
        Script_LightUtils(0, 4, 1.0f);
        break;
      case 420:
        Script_charaDisplay(SHEN_NAME('SCRT'), CDM_Show);
        break;
      case 435:
        Script_charaDisplay(CHARA_SINF, CDM_Show);
        Script_InitializeOsag(CHARA_SINF);
        Script_FACE(CHARA_SINF, 0, 0, 16);
        local_F4 = 1.0f;
        Script_charaSetRenderOpacity(SHEN_NAME('CVGF'), 1.0f);
        Script_charaSetRenderOpacity(SHEN_NAME('SCRT'), 1.0f);
        Script_SetOsageFlags(2, 0.0008f);
        break;
      case 600:
        Script_FACE(CHARA_SINF, 1, 0, 50);
        break;
      case 760:
        Script_charaSetRenderOpacity(SHEN_NAME('CVGF'), 1.0f);
        Script_charaSetRenderOpacity(SHEN_NAME('SCRT'), 1.0f);
        Script_scenCreateTask(script_6736, 0);
        break;
      case 950:
        goto L007DA9;
        break;
    }
    if (local_FC % 5 == 1) {
      local_F8 = local_F8 - 1;
      if (!((float)local_F8 < 0.1f)) {
        Script_EffectUtils(7, arg1, (float)local_F8 * 0.1f / 255.0f);
      }
    }
    if (local_FC < 660) {
      local_E8.x = 507.189606f;
      (&local_E8)->y = 0.558f;
      (&local_E8)->z = 1.78f;
      Script_FaceLookAtTargetUtils(CHARA_SINF, 2, &local_E8, 1);
    } else {
      local_E8.x = 507.189606f;
      (&local_E8)->y = 0.558f;
      (&local_E8)->z = 1.8f;
      Script_FaceLookAtTargetUtils(CHARA_SINF, 2, &local_E8, 6);
    }
    if ((local_FC >= 490) & (local_FC <= 610)) {
      local_E4 = local_E4 + 0.0009f;
      Script_SetOsageFlags(2, local_E4);
    }
    if ((local_FC >= 420) & (local_FC < 435)) {
      local_F4 = local_F4 + 0.0666666701f;
      Script_charaSetRenderOpacity(SHEN_NAME('CVGF'), local_F4);
      Script_charaSetRenderOpacity(SHEN_NAME('SCRT'), local_F4);
    }
    if ((local_FC > 436) & (local_FC <= 585)) {
      if (!(local_F4 <= 0.0f)) {
        local_F4 = local_F4 - 0.00666666683f;
        Script_charaSetRenderOpacity(SHEN_NAME('CVGF'), local_F4);
      } else {
        local_F4 = 0.0f;
        Script_charaSetRenderOpacity(SHEN_NAME('CVGF'), local_F4);
      }
    }
    if ((local_FC > 645) & (local_FC <= 759)) {
      if (!(local_F4 >= 1.0f)) {
        local_F4 = local_F4 + 0.00877192989f;
        Script_charaSetRenderOpacity(SHEN_NAME('CVGF'), local_F4);
      } else {
        local_F4 = 1.0f;
        Script_charaSetRenderOpacity(SHEN_NAME('CVGF'), local_F4);
      }
    }
    script_60F1(arg1, local_FC % 2);
    if (local_FC <= 410) {
      script_6628(arg0);
    }
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  L007DA9:;
  Script_FaceEx(CHARA_SINF, 0, 0, 1);
  (void)0;
  Script_lightSetSlotEnabled(0, 1);
  return;
}

static void script_7DC4(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  float local_F8;
  Vector3 local_EC;
  float local_E8;
  int32_t local_E4;
  script_5315();
  script_62B9(11, arg0);
  script_55C6(11, arg1);
  (void)0;
  Script_SpriteUtils(ScrollHorizontalOffset, 2592);
  Script_SpriteUtils(ScrollVerticalOffset, 39);
  Script_charaDisplay(CHARA_SINF, CDM_Hide);
  Script_charaDisplay(SHEN_NAME('STR_'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('CVGF'), CDM_Show);
  (void)1;
  Script_SetCharaRefBoardPos(CHARA_AKIR, &data_194FC);
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 1);
  local_FC = 0;
  local_F8 = 0.0f;
  local_E8 = 0.0f;
  local_EC.x = 521.6f;
  (&local_EC)->y = 11.4711f;
  (&local_EC)->z = 7.129f;
  Script_EVT_SetPosition(SHEN_NAME('STR_'), 0x78000000, &local_EC);
  Script_shenSeqUtils(14);
  Script_charaSetRenderOpacity(SHEN_NAME('CVGF'), 0.0f);
  Script_charaDisplay(SHEN_NAME('SCRT'), CDM_Hide);
  while (Script_shenSeqUtils(-1)) {
    local_E4 = local_FC;
    switch (local_E4) {
      case 410:
        Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 40.0f);
        Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 0.0f);
        Script_MT5Loader_TRWV_TRWAVE(10, arg0, 64.0f, 64.0f, 0, 0, 0, 0);
        break;
      case 416:
        Script_FaceEx(CHARA_SINF, 1, 0, 1);
        Script_lightSetGlobalSceneLighting(&data_194D0);
        Script_lightDirLightEnable(0, &data_194F0);
        Script_LightUtils(0, 2, &data_194E0);
        Script_LightUtils(0, 4, 1.0f);
        break;
      case 420:
        break;
      case 435:
        Script_charaDisplay(CHARA_SINF, CDM_Show);
        Script_charaDisplay(SHEN_NAME('SCRT'), CDM_Show);
        Script_InitializeOsag(CHARA_SINF);
        Script_FACE(CHARA_SINF, 0, 0, 16);
        local_F8 = 1.0f;
        Script_charaSetRenderOpacity(SHEN_NAME('CVGF'), 1.0f);
        Script_SetOsageFlags(2, 0.0008f);
        break;
      case 600:
        Script_FACE(CHARA_SINF, 1, 0, 50);
        break;
      case 760:
        Script_charaDisplay(SHEN_NAME('SCRT'), CDM_Hide);
        Script_charaDisplay(CHARA_SINF, CDM_Hide);
        Script_charaDisplay(SHEN_NAME('CVGF'), CDM_Hide);
        Script_scenCreateTask(script_6736, 0);
        break;
    }
    local_EC.x = 507.233704f;
    (&local_EC)->y = 0.87f;
    (&local_EC)->z = 1.7f;
    Script_FaceLookAtTargetUtils(CHARA_SINF, 2, &local_EC, 8);
    if ((local_FC >= 490) & (local_FC <= 610)) {
      local_E8 = local_E8 + 0.0009f;
      Script_SetOsageFlags(2, local_E8);
    }
    if ((local_FC >= 420) & (local_FC < 435)) {
      local_F8 = local_F8 + 0.0666666701f;
      Script_charaSetRenderOpacity(SHEN_NAME('CVGF'), local_F8);
    }
    if ((local_FC > 436) & (local_FC <= 585)) {
      if (!(local_F8 <= 0.0f)) {
        local_F8 = local_F8 - 0.00666666683f;
        Script_charaSetRenderOpacity(SHEN_NAME('CVGF'), local_F8);
      } else {
        local_F8 = 0.0f;
        Script_charaSetRenderOpacity(SHEN_NAME('CVGF'), local_F8);
      }
    }
    if ((local_FC > 645) & (local_FC <= 795)) {
      if (!(local_F8 >= 1.0f)) {
        local_F8 = local_F8 + 0.00833333377f;
        Script_charaSetRenderOpacity(SHEN_NAME('CVGF'), local_F8);
      } else {
        local_F8 = 1.0f;
        Script_charaSetRenderOpacity(SHEN_NAME('CVGF'), local_F8);
      }
    }
    script_60F1(arg1, local_FC % 2);
    if (local_FC <= 410) {
      script_6628(arg0);
    }
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  Script_FaceEx(CHARA_SINF, 0, 0, 1);
  (void)0;
  Script_lightSetSlotEnabled(0, 1);
  return;
}

static void script_8152(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  script_54F4();
  script_62B9(20, arg0);
  script_55C6(20, arg1);
  Script_facemotUtils(CHARA_AKIR, 0, 0);
  Script_facemotUtils(CHARA_AKIR, 1, 0);
  Script_FaceEx(CHARA_AKIR, 1, 0, 2);
  Script_ResetHandMotion(CHARA_AKIR, 1, 1, 0);
  Script_ResetHandMotion(CHARA_AKIR, 1, 0, 0);
  local_FC = 0;
  {
    int32_t unused_F0[3];
    Vector3i local_E4;
    local_E4.x = 0;
    (&local_E4)->y = 18331;
    (&local_E4)->z = 0;
    Script_charaPosRotUtils(SHEN_NAME('SHIP'), 153, 0, &local_E4, 0);
  }
  Script_FaceEx(CHARA_AKIR, 1, 0, 1);
  {
    Vector3 local_F0;
    Vector3i local_E4;
    local_F0.x = 99.333199f;
    (&local_F0)->y = -1.8f;
    (&local_F0)->z = 7.1017f;
    local_E4.x = 0;
    (&local_E4)->y = 16382;
    (&local_E4)->z = 0;
    Script_EVT_SetPosition(SHEN_NAME('SHIP'), 0x78000000, &local_F0);
    Script_EVT_SetRotation(SHEN_NAME('SHIP'), 0x78000000, &local_E4, 0);
  }
  Script_charaDisplay(SHEN_NAME('BAG0'), CDM_Show);
  Script_scenCreateTask(script_136E, 0);
  Script_FACE_LCG(CHARA_AKIR, 1, 1, 0);
  script_8692(CHARA_AKIR, SHEN_NAME('BAG0'));
  Script_SetHandRootRotation(CHARA_AKIR, 1, 0x15, &data_19088);
  Script_SetHandRootRotation(CHARA_AKIR, 0, 0x15, &data_19094);
  Script_handSetFloatsFor(CHARA_AKIR, 1, data_19184, 0);
  Script_handSetFloatsFor(CHARA_AKIR, 0, data_19268, 0);
  {
    Vector3 local_F0;
    Vector3i local_E4;
    local_F0.x = -0.216f;
    (&local_F0)->y = 0.339f;
    (&local_F0)->z = -0.072f;
    local_E4.x = 3183;
    (&local_E4)->y = 53887;
    (&local_E4)->z = 48590;
    Script_fixoAttachEx(SHEN_NAME('BAG0'), CHARA_AKIR, 18, &local_F0, &local_E4);
  }
  Script_shenSeqUtils(15);
  while (Script_shenSeqUtils(-1)) {
    if (local_FC == 45) {
      Script_FACE_LCG(CHARA_AKIR, 2, 5, 0);
    }
    script_60F1(arg1, local_FC % 2);
    script_6628(arg0);
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  if (Script_shenSeqUtils(-7)) {
    Script_filmCheckOrSetTransitionUtil(1, 0, 0, 0, 0, 255, 0, 0, 0);
    while (Script_filmCheckOrSetTransitionUtil(-1)) {
      SCNF_WaitFrames_0000(1);
    }
  } else {
    Script_filmCheckOrSetTransitionUtil(60, 0, 0, 0, 0, 255, 0, 0, 0);
    while (Script_filmCheckOrSetTransitionUtil(-1)) {
      SCNF_WaitFrames_0000(1);
    }
  }
  Script_FaceEx(CHARA_AKIR, 0, 0, 1);
  {
    int32_t unused_F0[3];
    Vector3i local_E4;
    local_E4.x = 0;
    (&local_E4)->y = 0;
    (&local_E4)->z = 0;
    Script_charaPosRotUtils(SHEN_NAME('SHIP'), 153, 0, &local_E4, 0);
  }
  Script_fixoClearAttachment(SHEN_NAME('BAG0'));
  Script_SetRyoMotionDetails(CHARA_AKIR);
  Script_faceUpdateFor(CHARA_AKIR, 1, 0);
  Script_FACE_LCG(CHARA_AKIR, 2, 5, 0);
  return;
}

static void script_844C(void) {
  Script_hmdlApplyScale(SHEN_NAME('SCRT'), &data_19524);
  Script_hmdlApplyScale(SHEN_NAME('CVGF'), &data_19524);
  script_85F8(CHARA_AKIR);
  script_85F8(CHARA_TAIJ);
  script_85F8(CHARA_SINF);
  script_85F8(SHEN_NAME('EXT1'));
  script_85F8(SHEN_NAME('EXT2'));
  script_85F8(SHEN_NAME('EXT3'));
  Script_FaceEx(CHARA_SINF, 0, 0, 1);
  Script_FaceEx(CHARA_TAIJ, 0, 0, 1);
  Script_FaceEx(CHARA_AKIR, 0, 0, 1);
  Script_scenToggleActiveFor(CHARA_AKIR, 0);
  Script_scenToggleActiveFor(CHARA_SINF, 0);
  Script_scenToggleActiveFor(CHARA_TAIJ, 0);
  Script_scenToggleActiveFor(SHEN_NAME('EXT1'), 0);
  Script_scenToggleActiveFor(SHEN_NAME('EXT2'), 0);
  Script_scenToggleActiveFor(SHEN_NAME('EXT3'), 0);
  Script_scenToggleActiveFor(SHEN_NAME('KAM1'), 0);
  Script_charaPosRotUtils(SHEN_NAME('SHIP'), 153, 0, &data_19518, 0);
  Script_SetMotnFlag(CHARA_AKIR, -1);
  Script_SetMotnFlag(CHARA_TAIJ, -1);
  Script_SetMotnFlag(CHARA_SINF, -1);
  return;
}

static void script_856E(void) {
  script_12C2(CHARA_AKIR);
  script_12C2(CHARA_TAIJ);
  script_12C2(CHARA_SINF);
  Script_charaDisplay(CHARA_AKIR, CDM_Show);
  Script_charaDisplay(CHARA_TAIJ, CDM_Show);
  Script_charaDisplay(CHARA_SINF, CDM_Show);
  Script_charaDisplay(SHEN_NAME('SHIP'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('KAM1'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('EXT1'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('EXT2'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('EXT3'), CDM_Show);
  Script_charaDisplay(SHEN_NAME('CVGF'), CDM_Show);
  return;
}

static void script_85F8(int32_t arg0) {
  Script_ccowUtils(arg0, 2);
  if (0) {
    Script_charaDisplay(arg0, CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(arg0, CDM_Show);
      goto L008624;
    }
  }
  L008624:;
  Script_scenToggleVisibleFor(arg0, 1);
  Script_ccowSetFlag80(arg0, 1);
  Script_motmToggleFlags(arg0, 2, 0x40);
  Script_charaToggleMotEnabled(arg0, 0);
  Script_facemotUtils(arg0, 0, 0);
  Script_FaceEx(arg0, 1, 0, 2);
  Script_FaceLookAtTargetUtils(arg0, 0);
  Script_FACE_LCG(arg0, 0, 5, 0);
  Script_CharHandlers(arg0, 2, 8, 16);
  Script_SetCPAMFlags(arg0, 0);
  Script_charaDisplay(arg0, CDM_Show);
  return;
}

static void script_8692(int32_t arg0, int32_t arg1) {
  Script_ccowUtils(arg1, 2);
  Script_motmToggleFlags(arg0, 2, 0x40);
  Script_RequestFightAnimation(arg0, 657, 0.0f, 14, 1);
  SCNF_WaitFrames_0000(1);
  Script_fixoAttachEx(arg1, arg0, 18, &data_19530, &data_1953C);
  Script_CharHandlers(arg0, 0, 0, 16);
  Script_CharHandlers(arg0, 1, 8, 16);
  Script_charaDisplay(arg1, CDM_Show);
  return;
}

__attribute__((scn_string_bindings(0, data_19554, 1, data_19561, 2, data_1956A, 3, data_19577)))
static void script_86F6(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  local_F8 = Script_MT5Loader_TRWV_TRWAVE(0, &data_19548, "model/object", "SEA2.PVR", "model/object", "TRSEA4.MT6", 4.0f, 0);
  script_8CB4(local_F8);
  script_121D();
  Script_MT5Loader_TRWV_TRWAVE(10, local_F8, 64.0f, 64.0f, 1, 1, 1, 2);
  script_8BC6(CHARA_SORY);
  Script_charaDisplay(CHARA_KURB, CDM_Hide);
  Script_executeAicaCommand(18088, 0, 0);
  Script_executeAicaCommand(1184, 2, 110);
  Script_scenCreateTask(script_136E, 0);
  Script_SetupClothData(CHARA_SORY, 9);
  Script_shenSeqUtils(arg0);
  local_FC = 0;
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 100.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 60.0f);
  Script_LODUtils(LOD_UTILS_SET_NPC_DISPLAY_RADIUS, 100.0f);
  while (Script_shenSeqUtils(-1)) {
    local_F4 = local_FC;
    if (local_F4 == 600) {
      Script_charaDisplay(CHARA_KURB, CDM_Show);
      script_8B8B(CHARA_SORY);
      Script_handSetFloatsFor(CHARA_SORY, 0, data_173FC, 1);
      Script_MT5Loader_TRWV_TRWAVE(10, local_F8, 64.0f, 64.0f, 4, 4, 3, 3);
      goto L00881E;
    }
    L00881E:;
    if (local_FC < 750) {
      Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 230.0f);
      Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 100.0f);
      Script_LODUtils(LOD_UTILS_SET_NPC_DISPLAY_RADIUS, 230.0f);
      script_8BF0(local_F8);
    }
    if (!(local_FC == 1440)) goto L00885C;
    L00885C:;
    script_8C19();
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  Script_executeAicaCommand(2720, 2, 60);
  if (Script_shenSeqUtils(-7)) {
    Script_filmCheckOrSetTransitionUtil(1, 0, 255, 255, 255, 255, 255, 255, 255);
    while (Script_filmCheckOrSetTransitionUtil(-1)) {
      SCNF_WaitFrames_0000(1);
    }
  } else {
    Script_filmCheckOrSetTransitionUtil(60, 0, 255, 255, 255, 255, 255, 255, 255);
    while (Script_filmCheckOrSetTransitionUtil(-1)) {
      SCNF_WaitFrames_0000(1);
    }
  }
  Script_MT5Loader_TRWV_TRWAVE(3, local_F8);
  return;
}

__attribute__((scn_string_bindings(0, data_195B4)))
static void script_88F8(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  Script_SetFogColour(&data_19594);
  Script_SetFogFlags(0, data_195A4);
  Script_SetupLightFromTable(0);
  Script_PREP_Related(-1, 1);
  Script_PREP_Related(8, 0);
  Script_lightSetSlotEnabled(10, 0);
  Script_SpriteUtils(Scroll_Read1, (int32_t)"sprite", data_195BB);
  local_F4.x = 99.333199f;
  (&local_F4)->y = -1.8f;
  (&local_F4)->z = 7.1017f;
  local_E8.x = 0;
  (&local_E8)->y = 16382;
  (&local_E8)->z = 0;
  Script_EVT_SetPosition(SHEN_NAME('SHIP'), 0x78000000, &local_F4);
  Script_EVT_SetRotation(SHEN_NAME('SHIP'), 0x78000000, &local_E8, 0);
  Script_SetVector4ByOffset(&data_19584);
  (void)1;
  script_1235(CHARA_AKIR);
  script_1235(CHARA_SORY);
  script_1235(CHARA_KURB);
  Script_SetShadowFlags(CHARA_AKIR, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_SetShadowFlags(CHARA_SORY, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_SetShadowFlags(CHARA_KURB, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_SetMotnFlag(CHARA_AKIR, -1);
  Script_SetMotnFlag(CHARA_SORY, -1);
  Script_SetMotnFlag(CHARA_KURB, -1);
  Script_SetMaxInterpSpeed(0.4f);
  Script_facemotUtils(CHARA_AKIR, 1, 0);
  Script_facemotUtils(CHARA_SORY, 1, 0);
  script_8B8B(CHARA_AKIR);
  script_8B8B(CHARA_SORY);
  Script_SetCharaRefBoardPos(CHARA_AKIR, &data_19584);
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 1);
  Script_SetCharaRefBoardPos(CHARA_SORY, &data_19584);
  Script_charaSetRefboardFlagTo(CHARA_SORY, 1);
  Script_SetCharaRefBoardPos(CHARA_KURB, &data_19584);
  Script_charaSetRefboardFlagTo(CHARA_KURB, 1);
  Script_SetShadowRange(30.0f);
  Script_SetCharaFlags(CHARA_AKIR, 4);
  Script_scenToggleActiveFor(CHARA_SORY, 0);
  Script_scenToggleActiveFor(CHARA_KURB, 0);
  Script_charaVisUtils(SHEN_NAME('BOT2'), 1);
  Script_charaDisplay(CHARA_AKIR, CDM_Hide);
  return;
}

static void script_8AEE(void) {
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 0);
  Script_charaSetRefboardFlagTo(CHARA_SORY, 0);
  Script_charaSetRefboardFlagTo(CHARA_KURB, 0);
  script_8BC6(CHARA_AKIR);
  script_8BC6(CHARA_SORY);
  Script_SetMotnFlag(CHARA_AKIR, 0);
  Script_SetMotnFlag(CHARA_SORY, 0);
  Script_SetMotnFlag(CHARA_KURB, 0);
  script_12C2(CHARA_AKIR);
  script_12C2(CHARA_SORY);
  script_12C2(CHARA_KURB);
  Script_SetCharaFlags(CHARA_AKIR, 4);
  Script_SetShadowRange(20.0f);
  return;
}

static void script_8B8B(int32_t arg0) {
  Script_ResetHandMotion(arg0, 1, 1, 0);
  Script_ResetHandMotion(arg0, 1, 0, 0);
  Script_handSetFloatsFor(arg0, 1, data_174E0, 8);
  Script_handSetFloatsFor(arg0, 0, data_174E0, 8);
  return;
}

static void script_8BC6(int32_t arg0) {
  Script_ResetHandMotion(arg0, 0, 1, 0);
  Script_ResetHandMotion(arg0, 0, 0, 0);
  Script_CharHandlers(arg0, 2, 8, 16);
  return;
}

static void script_8BF0(int32_t arg0) {
  Vector3 local_F4;
  int32_t unused_E8[3];
  Script_EVT_GetPosition(SHEN_NAME('BOT1'), -1, &local_F4, 0);
  (&local_F4)->y = -2.0f;
  Script_MT5Loader_TRWV_TRWAVE(5, arg0, &local_F4);
  return;
}

static void script_8C19(void) {
  {
    Vector3 local_F4;
    Vector3 local_E8;
    Script_EVT_GetPosition(CHARA_SORY, -1, &local_E8, 0);
    local_F4.x = (&local_E8)->x + 0.1723f;
    (&local_F4)->y = (&local_E8)->y + 1.6383f;
    (&local_F4)->z = (&local_E8)->z + 0.1052f;
    Script_LightUtils(8, 7, &local_F4);
  }
  {
    Vector3 local_F4;
    int32_t unused_E8[3];
    Vector3 local_DC;
    Script_EVT_GetPosition(CHARA_SORY, -1, &local_DC, 0);
    local_F4.x = (&local_DC)->x - 6.0477f;
    (&local_F4)->y = (&local_DC)->y - 0.6217f;
    (&local_F4)->z = (&local_DC)->z - 0.6648f;
    Script_LightUtils(10, 7, &local_F4);
  }
  return;
}

static void script_8CB4(int32_t arg0) {
  Script_MT5Loader_TRWV_TRWAVE(21, arg0, 8, 8);
  Script_MT5Loader_TRWV_TRWAVE(16, arg0, -1, 1);
  Script_MT5Loader_TRWV_TRWAVE(13, arg0, 0.0980392173f, 0.156862751f);
  Script_MT5Loader_TRWV_TRWAVE(14, arg0, 0.0745098069f, -0.00392156886f);
  Script_MT5Loader_TRWV_TRWAVE(10, arg0, 64.0f, 64.0f, 3, 1, 1, 1);
  Script_MT5Loader_TRWV_TRWAVE(24, arg0, 0, 151, 218, 192);
  Script_MT5Loader_TRWV_TRWAVE(11, arg0, -0.61f, -0.02f, 0.79f);
  Script_MT5Loader_TRWV_TRWAVE(1, arg0);
  return;
}

__attribute__((scn_string_bindings(0, data_195F4)))
static void script_8D47(void) {
  Script_SetFogColour(&data_195D4);
  Script_SetFogFlags(0, data_195E4);
  Script_SetupLightFromTable(0);
  Script_PREP_Related(-1, 1);
  Script_PREP_Related(8, 0);
  Script_PREP_Related(10, 0);
  Script_SpriteUtils(Scroll_Read1, (int32_t)"sprite", "SCROLL19.SPR");
  Script_mapmSetRenderFlag(0, 0);
  Script_mapmSetRenderFlag(1, 1);
  Script_mapmSetRenderFlag(2, 1);
  Script_mapmSetRenderFlag(5, 1);
  Script_mapmSetRenderFlag(6, 0);
  Script_mapmSetRenderFlag(7, 1);
  Script_mapmSetRenderFlag(8, 0);
  Script_mapmSetRenderFlag(9, 1);
  Script_mapmSetRenderFlag(12, 0);
  Script_mapmSetRenderFlag(13, 0);
  Script_mapmSetRenderFlag(3, 1);
  Script_mapmSetRenderFlag(4, 0);
  Script_mapmSetRenderFlag(11, 0);
  Script_mapmSetRenderFlag(10, 1);
  return;
}

__attribute__((scn_string_bindings(0, data_197D0, 3, data_197F3, 4, data_19801, 5, data_19809, 6, data_19816, 7, data_1981E)))
static void script_8E0F(void) {
  script_1052(1);
  Script_LoadSND(0, 1, "amb013.snd", 0);
  Script_LoadSND(0, 2, "bgm111.snd", 0);
  Script_LoadSND(0, 4, "a1_busno.snd", 0);
  Script_executeAicaCommand(16296, 0, 0);
  Script_executeAicaCommand(1184, 2, 100);
  script_0BE4();
  script_0FDE("scene/03/MEND", "BUSAUTH");
  script_10CB();
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
  Script_AuthorUtils(0, 0, "seqdataB.bin", "BUSAUTH");
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
  Script_scenCreateTask(script_1312, 0);
  script_8EC7(0);
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "BUSAUTH");
  return;
}

static void script_8EC7(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  Vector3 local_EC;
  Vector3i local_E0;
  int32_t local_DC;
  script_121D();
  script_90E4();
  Script_charaPosRotUtils(SHEN_NAME('BUS_'), 157, 0, &data_19828, 0);
  Script_charaPosRotUtils(SHEN_NAME('BUS_'), 158, 0, &data_19834, 0);
  Script_charaPosRotUtils(SHEN_NAME('BUS_'), 155, 0, &data_19840, 0);
  Script_charaPosRotUtils(SHEN_NAME('BUS_'), 156, 0, &data_1984C, 0);
  local_FC = 0;
  script_967F(CHARA_AKIR, SHEN_NAME('BAG0'));
  Script_executeAicaCommand(0x1B04A9, 0, 0);
  Script_SetHandRootRotation(CHARA_AKIR, 1, 0x15, &data_19864);
  Script_handSetFloatsFor(CHARA_AKIR, 1, data_19608, 0);
  Script_handSetFloatsFor(CHARA_AKIR, 0, data_196EC, 0);
  local_EC.x = -0.216f;
  (&local_EC)->y = 0.339f;
  (&local_EC)->z = -0.072f;
  local_E0.x = 3183;
  (&local_E0)->y = 53887;
  (&local_E0)->z = 48590;
  Script_fixoAttachEx(SHEN_NAME('BAG0'), CHARA_AKIR, 18, &local_EC, &local_E0);
  Script_shenSeqUtils(arg0);
  while (Script_shenSeqUtils(-1)) {
    local_DC = local_FC;
    switch (local_DC) {
      case 145:
        Script_scenCreateTask(script_92DB, 1, 2);
        break;
      case 200:
        Script_executeAicaCommand(0x1C04A9, 0, 0);
        local_F8 = Script_scenCreateTask(script_9568, 1, SHEN_NAME('BUS_'));
        break;
      case 250:
        goto L009062;
        break;
    }
    local_FC = local_FC + 1;
    script_95DB();
    SCNF_WaitFrames_0000(1);
  }
  L009062:;
  Script_executeAicaCommand(2720, 2, 60);
  if (Script_shenSeqUtils(-7)) {
    Script_filmCheckOrSetTransitionUtil(1, 0, 0, 0, 0, 255, 0, 0, 0);
    while (Script_filmCheckOrSetTransitionUtil(-1)) {
      SCNF_WaitFrames_0000(1);
    }
  } else {
    Script_filmCheckOrSetTransitionUtil(60, 0, 0, 0, 0, 255, 0, 0, 0);
    while (Script_filmCheckOrSetTransitionUtil(-1)) {
      SCNF_WaitFrames_0000(1);
    }
  }
  Script_shenSeqUtils(-6);
  Script_AuthorUtils(AUTH_FREE, arg0);
  script_922C();
  return;
}

static void script_90E4(void) {
  Vector3i local_F4;
  Vector3 local_E8;
  int32_t unused_D8[4];
  GameDateTime local_D0;
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 1);
  Script_SetCharaRefBoardPos(CHARA_AKIR, &data_1987C);
  script_1235(CHARA_AKIR);
  Script_facemotUtils(CHARA_AKIR, 1, 0);
  Script_ResetHandMotion(CHARA_AKIR, 1, 1, 0);
  Script_ResetHandMotion(CHARA_AKIR, 1, 0, 0);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 100.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 100.0f);
  local_F4.x = 0;
  (&local_F4)->y = 0;
  (&local_F4)->z = 0;
  Script_charaPosRotUtils(SHEN_NAME('BUS_'), 153, 0, &local_F4, 0);
  Script_charaPosRotUtils(SHEN_NAME('BUS_'), 154, 0, &local_F4, 0);
  Script_charaPosRotUtils(SHEN_NAME('BUS_'), 155, 0, &local_F4, 0);
  Script_charaPosRotUtils(SHEN_NAME('BUS_'), 156, 0, &local_F4, 0);
  Script_charaPosRotUtils(SHEN_NAME('BUS_'), 157, 0, &local_F4, 0);
  Script_charaPosRotUtils(SHEN_NAME('BUS_'), 158, 0, &local_F4, 0);
  Script_SetMaxNPCsNear(32);
  Script_mapmSetCamFlags(0, 0);
  Script_ReturnTimeDate(&local_D0);
  if ((&local_D0)->hours < 19) {
    local_E8.x = 0.0f;
    (&local_E8)->y = -100.0f;
    (&local_E8)->z = 0.0f;
    Script_charaPosRotUtils(SHEN_NAME('BUS_'), 196, &local_E8, 0, 0);
  }
  return;
}

static void script_922C(void) {
  Vector3 local_F4;
  int32_t unused_DC[6];
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 2);
  local_F4.x = 0.0f;
  (&local_F4)->y = 0.0f;
  (&local_F4)->z = 0.0f;
  Script_charaPosRotUtils(SHEN_NAME('BUS_'), 196, &local_F4, 0, 0);
  script_12C2(CHARA_AKIR);
  Script_SetShadowFlags(0, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 4);
  Script_SetShadowFlags(CHARA_AKIR, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 4);
  Script_EVT_GetPosition(CHARA_AKIR, -1, &local_F4, 0);
  (&local_F4)->y = 0.0f;
  Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &local_F4);
  Script_SetCharaFlags(CHARA_AKIR, 4);
  Script_mapmSetCamFlags(0, 1);
  Script_fixoClearAttachment(SHEN_NAME('BAG0'));
  Script_SetRyoMotionDetails(CHARA_AKIR);
  return;
}

static int32_t script_92DB(int32_t arg0) {
  int8_t local_FF;
  uint8_t unused_FC[3];
  Vector3i local_F0;
  Vector3i local_E4;
  Vector3i local_D8;
  Vector3i local_CC;
  Vector3i local_C0;
  Vector3i local_B4;
  local_F0.x = 0;
  (&local_F0)->y = 0;
  (&local_F0)->z = 0;
  local_E4.x = 0;
  (&local_E4)->y = 0;
  (&local_E4)->z = 0;
  local_D8.x = 0;
  (&local_D8)->y = 0;
  (&local_D8)->z = 0;
  local_CC.x = 0;
  (&local_CC)->y = 0;
  (&local_CC)->z = 0;
  local_C0.x = 0;
  (&local_C0)->y = 0;
  (&local_C0)->z = 0;
  local_B4.x = 0;
  (&local_B4)->y = 0;
  (&local_B4)->z = 0;
  {
    int32_t local_B0;
    local_B0 = arg0;
    switch (local_B0) {
      case 0:
        (&local_F0)->y = 14654;
        (&local_E4)->y = -30474;
        break;
      case 2:
        (&local_D8)->y = 14654;
        (&local_CC)->y = -30474;
        (&local_C0)->y = -14654;
        (&local_B4)->y = 30474;
        break;
    }
  }
  local_FF = 0;
  while (local_FF <= 30) {
    if (Script_shenSeqUtils(-1)) {
      int32_t local_B0;
      local_B0 = arg0;
      switch (local_B0) {
        case 0:
          (&local_F0)->y = (&local_F0)->y - 455;
          (&local_E4)->y = (&local_E4)->y + 983;
          break;
        case 1:
          (&local_F0)->y = (&local_F0)->y + 455;
          (&local_E4)->y = (&local_E4)->y - 983;
          break;
        case 2:
          (&local_D8)->y = (&local_D8)->y - 455;
          (&local_CC)->y = (&local_CC)->y + 983;
          (&local_C0)->y = (&local_C0)->y + 455;
          (&local_B4)->y = (&local_B4)->y - 983;
          break;
        case 3:
          (&local_D8)->y = (&local_D8)->y + 455;
          (&local_CC)->y = (&local_CC)->y - 983;
          (&local_C0)->y = (&local_C0)->y - 455;
          (&local_B4)->y = (&local_B4)->y + 983;
          break;
      }
      Script_charaPosRotUtils(SHEN_NAME('BUS_'), 153, 0, &local_F0, 0);
      Script_charaPosRotUtils(SHEN_NAME('BUS_'), 154, 0, &local_E4, 0);
      Script_charaPosRotUtils(SHEN_NAME('BUS_'), 157, 0, &local_D8, 0);
      Script_charaPosRotUtils(SHEN_NAME('BUS_'), 158, 0, &local_CC, 0);
      Script_charaPosRotUtils(SHEN_NAME('BUS_'), 155, 0, &local_C0, 0);
      Script_charaPosRotUtils(SHEN_NAME('BUS_'), 156, 0, &local_B4, 0);
    }
    local_FF = local_FF + 1;
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_9568(int32_t arg0) {
  while (Script_shenSeqUtils(-1)) {
    Script_charaPosRotUtils(arg0, 163, 0, &data_1988C, 1);
    Script_charaPosRotUtils(arg0, 173, 0, &data_1988C, 1);
    Script_charaPosRotUtils(arg0, 182, 0, &data_19898, 1);
    SCNF_WaitFrames_0000(1);
  }
  return Script_srchControl(0);
}

static void script_95BE(void) {
  SCNF_SetCashValue(-160, 1);
  SCNF_WaitFrames_0000(200);
  Script_srchControl(0);
  return;
}

static void script_95DB(void) {
  Vector3 local_F4;
  float local_F0;
  float local_EC;
  float local_E8;
  float local_E4;
  float local_E0;
  float local_DC;
  float local_D8;
  float local_D4;
  Script_EVT_GetPosition(SHEN_NAME('BUS_'), -1, &local_F4, 0);
  local_F0 = (&local_F4)->x - 4.0f;
  local_EC = (&local_F4)->z - 4.0f;
  local_E8 = (&local_F4)->x + 4.0f;
  local_E4 = (&local_F4)->z - 4.0f;
  local_E0 = (&local_F4)->x + 4.0f;
  local_DC = (&local_F4)->z + 4.0f;
  local_D8 = (&local_F4)->x - 4.0f;
  local_D4 = (&local_F4)->z + 4.0f;
  Script_WeatherUtils(WeatherScript_ApplyWccpClipSet1, local_F0, local_EC, local_E8, local_E4, local_E0, local_DC, local_D8, local_D4, 0.0f, 3.5f);
  return;
}

static void script_967F(int32_t arg0, int32_t arg1) {
  Script_ccowUtils(arg1, 2);
  Script_motmToggleFlags(arg0, 2, 0x40);
  Script_RequestFightAnimation(arg0, 657, 0.0f, 14, 1);
  SCNF_WaitFrames_0000(1);
  Script_fixoAttachEx(arg1, arg0, 18, &data_198A4, &data_198B0);
  Script_CharHandlers(arg0, 0, 0, 16);
  Script_CharHandlers(arg0, 1, 8, 16);
  Script_charaDisplay(arg1, CDM_Show);
  return;
}

__attribute__((scn_string_bindings(0, data_198BC, 1, data_198C7, 2, data_198D9, 3, data_198E7, 4, data_198EE, 5, data_198FC, 6, data_19905, 7, data_1990E)))
void script_96E3(void) {
  int32_t unused_F4[3];
  int32_t local_F0;
  Script_filmCheckOrSetTransitionUtil(0, 255, 0, 0, 0, 0, 0, 0, 0);
  SCNF_WaitFrames_0000(30);
  if (Script_GetCurrentEntryPoint() == 99) {
    script_0D61();
    script_A1C6();
    Script_shenRequestGameModuleTransition(3, SHEN_NAME('MEND'), 99);
    SCNF_WaitFrames_0000(1);
  }
  script_1052(1);
  Script_LoadSND(0, 1, "amb013.snd", 0);
  script_0BE4();
  script_0FDE("scene/03/MEND", (const char *)data_198D5);
  script_0FDE("scene/03/MEND", "MODELS");
  script_0D61();
  local_F0 = script_158A();
  Script_MT5Loader_TRWV_TRWAVE(1, local_F0);
  if (-1) {
    Script_charaDisplay(CHARA_CHAI, CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(CHARA_CHAI, CDM_Show);
      goto L0097AD;
    }
  }
  L0097AD:;
  Script_scenToggleVisibleFor(CHARA_CHAI, 0);
  script_0174(CHARA_AKIR);
  Script_SetShouldSleepFlag(0);
  script_8E0F();
  script_1083();
  script_1152();
  script_1052(2);
  Script_MT5Loader_TRWV_TRWAVE(22, local_F0, 3, 1);
  script_9900(local_F0);
  Script_MT5Loader_TRWV_TRWAVE(22, local_F0, 3, 2);
  Script_filmCheckOrSetTransitionUtil(0, 255, 0, 0, 0, 0, 0, 0, 0);
  if (0) {
    Script_charaDisplay(CHARA_CHAI, CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(CHARA_CHAI, CDM_Show);
      goto L009845;
    }
  }
  L009845:;
  Script_scenToggleVisibleFor(CHARA_CHAI, 1);
  task_78 = 0;
  script_AC62();
  script_0FDE("scene/03/MEND", "CHAIAUTH");
  script_99F0();
  Script_SetActionSelectorHUDElementVisibility(1);
  script_9C08();
  Script_MT5Loader_TRWV_TRWAVE(3, local_F0);
  script_A389();
  script_9CB7();
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "CHAIAUTH");
  script_9E6D();
  Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_LETTER_FOR_TAO_LI_SHAO, 1);
  script_AC95();
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "MODELS");
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "MAP");
  script_9FD2();
  script_A1C6();
  Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 979, 1);
  Script_GetCassetteFlags(4, 1, 0, 0);
  MODU_Script_shenRequestGameModule(Credits, 0, 0, 0);
  return;
}

static void script_9900(int32_t arg0) {
  script_1152();
  Script_AddCharacterHandler(3, data_19928, SHEN_NAME('TETU'));
  if (0) {
    Script_charaDisplay(SHEN_NAME('BAG_'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('BAG_'), CDM_Show);
      goto L00993F;
    }
  }
  L00993F:;
  Script_scenToggleVisibleFor(SHEN_NAME('BAG_'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('TETU'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('TETU'), CDM_Show);
      goto L009973;
    }
  }
  L009973:;
  Script_scenToggleVisibleFor(SHEN_NAME('TETU'), 1);
  script_BC50(arg0);
  Script_SetMapControlTaskFlag(SHEN_NAME('TETU'), 1);
  Script_SetCharaLookAtPosition(CHARA_TAIJ, -98312, 0, 0);
  Script_SetCharaLookAtPosition(CHARA_AKIR, -98312, 0, 0);
  Script_SetCharaLookAtPosition(CHARA_KISY, -98312, 0, 0);
  Script_EVT_SetPosition(SHEN_NAME('BAG_'), POS_XYZ, &data_1991C);
  Script_EVT_SetPosition(SHEN_NAME('BAG0'), POS_XYZ, &data_1991C);
  return;
}

__attribute__((scn_string_bindings(2, data_1995F, 4, data_19979, 5, data_19986)))
static void script_99F0(void) {
  int32_t unused_E8[6];
  script_1152();
  script_1052(2);
  Script_LoadSND(0, 2, "tsm001.snd", 0);
  Script_LoadSND(0, 4, "a1_saib.snd", 0);
  Script_LoadSND(0, 5, "battle_1.snd", 0);
  Script_LoadSND(0, 6, "e1batchi.snd", 0);
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
  Script_AuthorUtils(0, 0, "seqdata3.bin", "CHAIAUTH");
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
  if (0) {
    Script_charaDisplay(SHEN_NAME('TKOT'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('TKOT'), CDM_Show);
      goto L009A91;
    }
  }
  L009A91:;
  Script_scenToggleVisibleFor(SHEN_NAME('TKOT'), 1);
  Script_WeatherUtils_0(WeatherDataUtil_SetInterpolatedWeatherOverride, 1, 0, 400, 0);
  Script_WeatherUtils_0(WeatherDataUtil_SetInterpolatedWeatherOverride, 2, 0, 400, 0);
  Script_ReturnTimeDate(&task_6C);
  for (;;) {
    Script_charaDisplay(CHARA_TAIJ, CDM_Show);
    Script_charaDisplay(CHARA_KISY, CDM_Show);
    Script_shenSeqUtils(-9, 0, 0);
    Script_FaceEx(CHARA_KISY, 1, 0, 1);
    Script_FACE(CHARA_KISY, 0, 0, 1);
    script_16E8(1);
    Script_shenSeqUtils(-9, 0, 1);
    Script_charaDisplay(CHARA_TAIJ, CDM_Hide);
    Script_charaDisplay(CHARA_KISY, CDM_Hide);
    Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 120.0f);
    if (script_A8BF() == 3) {
      break;
    }
    script_AAAE();
  }
  Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_AuthorUtils(AUTH_FREE, 0);
  Script_motmToggleFlags(CHARA_AKIR, 2, 4);
  Script_motmToggleFlags(CHARA_AKIR, 2, 0x20);
  Script_motmToggleFlags(CHARA_AKIR, 2, 0x40);
  Script_motmToggleFlags(CHARA_AKIR, 2, 0x80);
  Script_motmToggleFlags(CHARA_AKIR, 2, 8);
  Script_motmToggleFlags(CHARA_CHAI, 2, 4);
  Script_motmToggleFlags(CHARA_CHAI, 2, 0x20);
  Script_motmToggleFlags(CHARA_CHAI, 2, 0x40);
  Script_motmToggleFlags(CHARA_CHAI, 2, 0x80);
  Script_motmToggleFlags(CHARA_CHAI, 2, 8);
  return;
}

__attribute__((scn_string_bindings(1, data_1999B, 2, data_199A8, 3, data_199B5, 4, data_199BE, 5, data_199CB, 6, data_199D4, 7, data_199E1, 8, data_199EA, 9, data_199F7, 10, data_19A00, 11, data_19A0D)))
static void script_9C08(void) {
  int32_t unused_FC;
  script_1152();
  script_1052(2);
  Script_LoadSND(0, 2, "fre2400.snd", 0);
  Script_LoadSND(0, 4, "a1_momor.snd", 0);
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
  Script_AuthorUtils(0, 0, "seqdata0.bin", "CHAIAUTH");
  Script_AuthorUtils(0, 1, "seqdata1.bin", "CHAIAUTH");
  Script_AuthorUtils(0, 2, "seqdata5.bin", "CHAIAUTH");
  Script_AuthorUtils(0, 3, "seqdata6.bin", "CHAIAUTH");
  Script_AuthorUtils(0, 4, "seqdata4.bin", "CHAIAUTH");
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
  script_37CF(1);
  return;
}

__attribute__((scn_string_bindings(0, data_19A16, 2, data_19A2C)))
static void script_9CB7(void) {
  script_3679();
  script_1052(2);
  Script_LoadSND(0, 2, "bgm067.snd", 0);
  Script_LoadSND(0, 3, "bgm032.snd", 0);
  Script_LoadSND(0, 4, "a1_momor.snd", 0);
  if (-1) {
    Script_charaDisplay(CHARA_TAIJ, CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(CHARA_TAIJ, CDM_Show);
      goto L009D1F;
    }
  }
  L009D1F:;
  Script_scenToggleVisibleFor(CHARA_TAIJ, 0);
  if (-1) {
    Script_charaDisplay(CHARA_KISY, CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(CHARA_KISY, CDM_Show);
      goto L009D53;
    }
  }
  L009D53:;
  Script_scenToggleVisibleFor(CHARA_KISY, 0);
  script_AC95();
  Script_shenSeqUtils(-9, 1, 0);
  script_14DEC(0);
  script_14E25(0);
  Script_shenClearFreeState(0);
  Script_shenSeqUtils(-9, 1, 1);
  script_AC62();
  script_3F2F(1);
  script_408F(1);
  if (0) {
    Script_charaDisplay(CHARA_TAIJ, CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(CHARA_TAIJ, CDM_Show);
      goto L009DD0;
    }
  }
  L009DD0:;
  Script_scenToggleVisibleFor(CHARA_TAIJ, 1);
  if (0) {
    Script_charaDisplay(CHARA_KISY, CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(CHARA_KISY, CDM_Show);
      goto L009E04;
    }
  }
  L009E04:;
  Script_scenToggleVisibleFor(CHARA_KISY, 1);
  Script_WeatherUtils_0(WeatherDataUtil_SetInterpolatedWeatherOverride, 2, 1, 400, 0);
  Script_WeatherUtils_0(WeatherDataUtil_SetInterpolatedWeatherOverride, 1, 1, 400, 0);
  script_1ADF(1);
  Script_shenSeqUtils(-6);
  Script_AuthorUtils(AUTH_FREE, 4);
  Script_AuthorUtils(AUTH_FREE, 3);
  Script_AuthorUtils(AUTH_FREE, 2);
  Script_AuthorUtils(AUTH_FREE, 1);
  Script_AuthorUtils(AUTH_FREE, 0);
  return;
}

__attribute__((scn_string_bindings(0, data_19A39, 1, data_19A47, 2, data_19A50, 3, data_19A5D, 5, data_19A71, 6, data_19A7E)))
static void script_9E6D(void) {
  if (-1) {
    Script_charaDisplay(CHARA_CHAI, CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(CHARA_CHAI, CDM_Show);
      goto L009E97;
    }
  }
  L009E97:;
  Script_scenToggleVisibleFor(CHARA_CHAI, 0);
  if (-1) {
    Script_charaDisplay(CHARA_TAIJ, CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(CHARA_TAIJ, CDM_Show);
      goto L009ECB;
    }
  }
  L009ECB:;
  Script_scenToggleVisibleFor(CHARA_TAIJ, 0);
  if (-1) {
    Script_charaDisplay(CHARA_KISY, CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(CHARA_KISY, CDM_Show);
      goto L009EFF;
    }
  }
  L009EFF:;
  Script_scenToggleVisibleFor(CHARA_KISY, 0);
  script_0FDE("scene/03/MEND", "TRAPAUTH");
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
  Script_AuthorUtils(0, 2, "seqdata1.bin", "TRAPAUTH");
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
  script_1052(2);
  Script_LoadSND(0, 2, "bgm095.snd", 0);
  Script_LoadSND(0, 4, "a1_shuko.snd", 0);
  script_26B2(1);
  Script_AuthorUtils(AUTH_FREE, 2);
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "TRAPAUTH");
  Script_SetRyoMotionDetails(CHARA_AKIR);
  script_37B7();
  if (0) {
    Script_charaDisplay(CHARA_AKIR, CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(CHARA_AKIR, CDM_Show);
      goto L009FC5;
    }
  }
  L009FC5:;
  Script_scenToggleVisibleFor(CHARA_AKIR, 1);
  return;
}

__attribute__((scn_string_bindings(3, data_19ADB, 4, data_19AE1, 5, data_19AEF, 6, data_19AF4, 7, data_19B01, 8, data_19B06, 9, data_19B0D, 10, data_19B1A, 11, data_19B20)))
static void script_9FD2(void) {
  int32_t unused_FC;
  GameDateTime local_F4;
  int32_t unused_F0;
  GameDateTime local_E8;
  Script_ReturnTimeDate(&local_F4);
  Script_ReturnTimeDate(&local_E8);
  (&local_F4)->hours = 19;
  (&local_F4)->minutes = 30;
  Script_SetNewDateTime(&local_F4);
  script_1052(1);
  Script_LoadSND(0, 1, "amb018.snd", 0);
  Script_LoadSND(0, 2, "bgm070.snd", 0);
  Script_LoadSND(0, 4, "a1_nooma.snd", 0);
  script_0BEC();
  Script_executeAicaCommand(18088, 0, 0);
  Script_executeAicaCommand(1184, 2, 96);
  Script_LoadVoiceFile(1, "01JIN");
  script_0FDE("scene/03/MEND", "NZOM");
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
  Script_AuthorUtils(0, 0, "seqdata0.bin", "NZOM");
  script_4413();
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
  script_120E();
  script_0D95();
  Script_SetFogFlags(0, data_19A98);
  Script_SetFogColour(&data_19A88);
  Script_SetupLightFromTable(0);
  Script_SpriteUtils(Scroll_Read1, (int32_t)"sprite", "SCROLL27.SPR");
  (void)1;
  Script_SetVector4ByOffset(&data_19AA8);
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 1);
  Script_SetCharaRefBoardPos(CHARA_AKIR, &data_19AA8);
  Script_charaSetRefboardFlagTo(CHARA_HRSK, 1);
  Script_SetCharaRefBoardPos(CHARA_HRSK, &data_19AA8);
  Script_charaSetRefboardFlagTo(SHEN_NAME('OMAM'), 1);
  Script_SetCharaRefBoardPos(SHEN_NAME('OMAM'), &data_19AA8);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_scenCreateTask(script_136E, 0);
  script_4387(0);
  Script_shenSeqUtils(-6);
  Script_AuthorUtils(AUTH_FREE, 0);
  Script_executeAicaCommand(2720, 2, 60);
  script_45DB();
  Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_NOZOMIS_AMULET, 1);
  Script_LoadVoiceFile(0, "01JIN");
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "NZOM");
  script_0DFA();
  script_1052(1);
  Script_SetNewDateTime(&local_E8);
  Script_GetCassetteFlags(7, 1, 0, 0);
  return;
}

__attribute__((scn_string_bindings(0, data_19B25, 1, data_19B33, 2, data_19B3B, 3, data_19B48, 5, data_19B5D, 6, data_19B65, 7, data_19B72, 8, data_19B7A, 9, data_19B87, 10, data_19B8F, 11, data_19B9C, 12, data_19BA4, 13, data_19BB1, 15, data_19BC6, 17, data_19BDB, 19, data_19BF0, 21, data_19C05, 22, data_19C0D, 23, data_19C1A, 25, data_19C2F, 27, data_19C44, 29, data_19C59, 31, data_19C6E, 33, data_19C83, 34, data_19C8B, 35, data_19C96, 36, data_19CA3, 37, data_19CB1)))
static void script_A1C6(void) {
  script_0FDE("scene/03/MEND", "ENDAUTH");
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
  Script_AuthorUtils(0, 0, "seqdata1.bin", "ENDAUTH");
  Script_AuthorUtils(0, 1, "seqdata2.bin", "ENDAUTH");
  Script_AuthorUtils(0, 2, "seqdata3.bin", "ENDAUTH");
  Script_AuthorUtils(0, 3, "seqdata4.bin", "ENDAUTH");
  Script_AuthorUtils(0, 4, "seqdata5.bin", "ENDAUTH");
  Script_AuthorUtils(0, 5, "seqdata6.bin", "ENDAUTH");
  Script_AuthorUtils(0, 6, "seqdata7.bin", "ENDAUTH");
  Script_AuthorUtils(0, 7, "seqdata8.bin", "ENDAUTH");
  Script_AuthorUtils(0, 8, "seqdata9.bin", "ENDAUTH");
  Script_AuthorUtils(0, 9, "seqdataA.bin", "ENDAUTH");
  Script_AuthorUtils(0, 10, "seqdataB.bin", "ENDAUTH");
  Script_AuthorUtils(0, 11, "seqdataC.bin", "ENDAUTH");
  Script_AuthorUtils(0, 12, "seqdataD.bin", "ENDAUTH");
  Script_AuthorUtils(0, 13, "seqdataE.bin", "ENDAUTH");
  Script_AuthorUtils(0, 14, "seqdataF.bin", "ENDAUTH");
  Script_AuthorUtils(0, 15, "seqdataT.bin", "ENDAUTH");
  Script_FaceEx(CHARA_AKIR, 1, 0, 2);
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
  script_1052(1);
  Script_LoadSND(0, 1, "amb013.snd", 0);
  Script_LoadSND(0, 4, "a1_shuko.snd", 0);
  script_0FDE("scene/03/MEND", "ENDMAP");
  script_1190();
  script_0BE4();
  script_69F1(17);
  SCNF_WaitFrames_0000(30);
  script_856E();
  return;
}

__attribute__((scn_string_bindings(2, data_19CD0, 3, data_19CDE, 4, data_19CE3, 5, data_19CF0, 6, data_19CF5)))
static void script_A389(void) {
  if (-1) {
    Script_charaDisplay(CHARA_AKIR, CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(CHARA_AKIR, CDM_Show);
      goto L00A3B3;
    }
  }
  L00A3B3:;
  Script_scenToggleVisibleFor(CHARA_AKIR, 0);
  if (-1) {
    Script_charaDisplay(CHARA_CHAI, CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(CHARA_CHAI, CDM_Show);
      goto L00A3E7;
    }
  }
  L00A3E7:;
  Script_scenToggleVisibleFor(CHARA_CHAI, 0);
  if (-1) {
    Script_charaDisplay(CHARA_TAIJ, CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(CHARA_TAIJ, CDM_Show);
      goto L00A41B;
    }
  }
  L00A41B:;
  Script_scenToggleVisibleFor(CHARA_TAIJ, 0);
  if (-1) {
    Script_charaDisplay(CHARA_KISY, CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(CHARA_KISY, CDM_Show);
      goto L00A44F;
    }
  }
  L00A44F:;
  Script_scenToggleVisibleFor(CHARA_KISY, 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('BAG_'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('BAG_'), CDM_Show);
      goto L00A483;
    }
  }
  L00A483:;
  Script_scenToggleVisibleFor(SHEN_NAME('BAG_'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('BAG0'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('BAG0'), CDM_Show);
      goto L00A4B7;
    }
  }
  L00A4B7:;
  Script_scenToggleVisibleFor(SHEN_NAME('BAG0'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('TKOT'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('TKOT'), CDM_Show);
      goto L00A4EB;
    }
  }
  L00A4EB:;
  Script_scenToggleVisibleFor(SHEN_NAME('TKOT'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('TRAP'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('TRAP'), CDM_Show);
      goto L00A51F;
    }
  }
  L00A51F:;
  Script_scenToggleVisibleFor(SHEN_NAME('TRAP'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('DR00'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('DR00'), CDM_Show);
      goto L00A553;
    }
  }
  L00A553:;
  Script_scenToggleVisibleFor(SHEN_NAME('DR00'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('DR01'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('DR01'), CDM_Show);
      goto L00A587;
    }
  }
  L00A587:;
  Script_scenToggleVisibleFor(SHEN_NAME('DR01'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('DR02'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('DR02'), CDM_Show);
      goto L00A5BB;
    }
  }
  L00A5BB:;
  Script_scenToggleVisibleFor(SHEN_NAME('DR02'), 0);
  script_0D95();
  Script_SetupLightFromTable(1);
  script_1052(2);
  Script_LoadSND(0, 2, "bgm063.snd", 0);
  Script_LoadSND(0, 4, "a1_ranbo.snd", 0);
  script_0FDE("scene/03/MEND", "YARN");
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
  Script_AuthorUtils(0, 10, "seqdata0.bin", "YARN");
  script_88F8();
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
  Script_mapmSetRenderFlag(1, 1);
  script_86F6(10);
  Script_shenSeqUtils(-4);
  Script_AuthorUtils(AUTH_FREE, 10);
  Script_mapmSetRenderFlag(1, 0);
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "YARN");
  script_0E7E();
  script_1083();
  script_1152();
  if (0) {
    Script_charaDisplay(CHARA_AKIR, CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(CHARA_AKIR, CDM_Show);
      goto L00A6AA;
    }
  }
  L00A6AA:;
  Script_scenToggleVisibleFor(CHARA_AKIR, 1);
  if (0) {
    Script_charaDisplay(CHARA_CHAI, CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(CHARA_CHAI, CDM_Show);
      goto L00A6DE;
    }
  }
  L00A6DE:;
  Script_scenToggleVisibleFor(CHARA_CHAI, 1);
  if (0) {
    Script_charaDisplay(CHARA_TAIJ, CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(CHARA_TAIJ, CDM_Show);
      goto L00A712;
    }
  }
  L00A712:;
  Script_scenToggleVisibleFor(CHARA_TAIJ, 1);
  if (0) {
    Script_charaDisplay(CHARA_KISY, CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(CHARA_KISY, CDM_Show);
      goto L00A746;
    }
  }
  L00A746:;
  Script_scenToggleVisibleFor(CHARA_KISY, 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('BAG_'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('BAG_'), CDM_Show);
      goto L00A77A;
    }
  }
  L00A77A:;
  Script_scenToggleVisibleFor(SHEN_NAME('BAG_'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('BAG0'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('BAG0'), CDM_Show);
      goto L00A7AE;
    }
  }
  L00A7AE:;
  Script_scenToggleVisibleFor(SHEN_NAME('BAG0'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('TKOT'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('TKOT'), CDM_Show);
      goto L00A7E2;
    }
  }
  L00A7E2:;
  Script_scenToggleVisibleFor(SHEN_NAME('TKOT'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('TRAP'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('TRAP'), CDM_Show);
      goto L00A816;
    }
  }
  L00A816:;
  Script_scenToggleVisibleFor(SHEN_NAME('TRAP'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('DR00'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('DR00'), CDM_Show);
      goto L00A84A;
    }
  }
  L00A84A:;
  Script_scenToggleVisibleFor(SHEN_NAME('DR00'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('DR01'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('DR01'), CDM_Show);
      goto L00A87E;
    }
  }
  L00A87E:;
  Script_scenToggleVisibleFor(SHEN_NAME('DR01'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('DR02'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('DR02'), CDM_Show);
      goto L00A8B2;
    }
  }
  L00A8B2:;
  Script_scenToggleVisibleFor(SHEN_NAME('DR02'), 1);
  return;
}

__attribute__((scn_string_bindings(1, data_19D05)))
static int32_t script_A8BF(void) {
  int32_t unused_E8[6];
  Vector3 local_DC;
  if (task_78 == 0) {
    task_78 = Script_motiLoad(3, SHEN_NAME('MEND'), "M_FGT4.BIN");
    Script_LoadSND(0, 5, "battle_1.snd", 0);
  }
  Script_vfSetup(0, 20, 20, 0, 0);
  Script_vfStageObjectUtils(0, 0, 0);
  Script_vfStageObjectUtils(2, 0, 2, 2, 0x40600000, 0x4094CCCD, -0x402A3D71, -0x3EE00000, -0x3FC00000, 0x3F000000);
  Script_vfStageObjectUtils(7, 0, 0x544F4B54, 0);
  Script_vfStageObjectUtils(13);
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
  task_0 = 0x49414843;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 1;
  *(&task_0 + 3) = 1;
  *(&task_0 + 4) = 600;
  *(&task_0 + 5) = 27;
  *(&task_0 + 6) = 0;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0;
  Script_vfUtils(&task_0);
  Script_vfConfigureFormationGroup(CHARA_CHAI, VF_FORMATION_APPLY_FLAGS, 0x200000, 0x800);
  Script_EVT_GetPosition(CHARA_AKIR, -1, &local_DC, 0);
  Script_vfStageAssignRingBoundary(&local_DC, 20.0f);
  SCN_Enqueue_VFGI(2, 99, 1.0f, 0, -1);
  Script_SetGameStateTo(AUTH_Combat);
  while (SCN_GetIsFighting() != 2) {
    SCNF_WaitFrames_0000(1);
  }
  Script_SetMotnFlag(CHARA_AKIR, 3);
  return Script_GetFightingStats(0x52494B41, 0);
}

static void script_AAAE(void) {
  Script_executeAicaCommand(2720, 2, 60);
  Script_filmCheckOrSetTransitionUtil(60, 0, 0, 0, 0, 255, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_uiUtils(0, 20, 0);
  Script_shenClearFreeState(1);
  Script_sicnUtils(0, 1, 1);
  SCNF_WaitFrames_0000(90);
  Script_SetNewDateTime(&task_6C);
  Script_WeatherTaskUtil(FreeTaskData);
  Script_WeatherTaskUtil(InitTaskData);
  Script_sicnUtils(2, 1, 0);
  Script_shenClearFreeState(0);
  Script_motmToggleFlags(CHARA_AKIR, 2, 4);
  Script_motmToggleFlags(CHARA_AKIR, 2, 0x20);
  Script_motmToggleFlags(CHARA_AKIR, 2, 0x40);
  Script_motmToggleFlags(CHARA_AKIR, 2, 0x80);
  Script_motmToggleFlags(CHARA_AKIR, 2, 8);
  Script_motmToggleFlags(CHARA_CHAI, 2, 4);
  Script_motmToggleFlags(CHARA_CHAI, 2, 0x20);
  Script_motmToggleFlags(CHARA_CHAI, 2, 0x40);
  Script_motmToggleFlags(CHARA_CHAI, 2, 0x80);
  Script_motmToggleFlags(CHARA_CHAI, 2, 8);
  SCNF_WaitFrames_0000(1);
  return;
}

__attribute__((scn_string_bindings(0, data_19D12)))
static void script_ABBF(void) {
  Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 975, 1);
  task_0 = -6;
  *(&task_0 + 1) = 0;
  *(&task_0 + 2) = 0;
  *(&task_0 + 3) = 0;
  *(&task_0 + 4) = 0;
  *(&task_0 + 5) = 15;
  *(&task_0 + 6) = 0;
  *(&task_0 + 7) = 0;
  *(&task_0 + 8) = 0;
  *(&task_0 + 9) = 0;
  *(&task_0 + 10) = 0;
  Script_vfUtils(&task_0);
  Script_uiUtils(0, 0, 0);
  Script_ReallocateUNFO(task_78);
  Script_LoadSND(0, 5, "FREE", 0);
  return;
}

__attribute__((scn_string_bindings(2, data_19D24, 3, data_19D2B)))
static void script_AC62(void) {
  Script_LoadVoiceFile(1, "A0184");
  Script_LoadVoiceFile(1, "A0184B");
  Script_LoadVoiceFile(1, "A0184C");
  Script_LoadVoiceFile(1, "A0184D");
  return;
}

__attribute__((scn_string_bindings(0, data_19D32, 1, data_19D38, 2, data_19D3F)))
static void script_AC95(void) {
  Script_LoadVoiceFile(0, "E1062");
  Script_LoadVoiceFile(0, "A0184D");
  Script_LoadVoiceFile(0, "A0184C");
  Script_LoadVoiceFile(0, data_19D46);
  return;
}

static int32_t script_ACC8(int32_t arg0) {
  if (arg0 < 0) {
    return -arg0;
  } else {
    return arg0;
  }
}

static void script_ACE6(float arg0) {
  if (!(arg0 >= 0.0f)) {
    (void)((float)0 - arg0);
    return;
  } else {
    (void)arg0;
    return;
  }
}

static void script_AD05(int32_t arg0) {
  if (arg0 != 0) {
    Script_sprintf("WAZA Warning : handle is not NULL, and create same handle !\012");
  }
  return;
}

static void script_AD1A(int32_t arg0, int32_t arg1) {
  for (;;) {
    if (*(&task_D8 + 3) & 2) {
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

static int32_t script_ADD0(int32_t arg0) {
  task_5FC = arg0;
  *(&task_5FC + 1) = 0;
  return (int32_t)&task_5FC;
}

static void script_ADE8(int32_t arg0) {
  Script_sprintf("%s\012", script_ADD0(arg0));
  return;
}

static void script_ADFE(int32_t arg0) {
  Script_sprintf("Konnna Error ha Detekonai hazu : %d\012", arg0);
  return;
}

static void script_AE0C(void) {
  Vector3 local_F4;
  SCNF_WaitFrames_0000(60);
  Script_EVT_GetPosition(CHARA_AKIR, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  (&local_F4)->y = 0.1f;
  for (;;) {
    if (*(&task_C8 + 1) & 0x4000) {
      local_F4.x = (&local_F4)->x - 0.1f;
    }
    if (*(&task_C8 + 1) & 0x8000) {
      local_F4.x = (&local_F4)->x + 0.1f;
    }
    if (*(&task_C8 + 1) & 0x1000) {
      (&local_F4)->z = (&local_F4)->z - 0.1f;
    }
    if (*(&task_C8 + 1) & 0x2000) {
      (&local_F4)->z = (&local_F4)->z + 0.1f;
    }
    Script_TranslateRotateScale_Wrapper(1, 0x3F800000, &local_F4, &data_19E58, &data_19E4C);
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_AED6(void) {
  return;
}

static int32_t script_AED9(void) {
  task_F8 = 0;
  while (1) {
    task_F8 = task_F8 + 1;
    task_C8 = Script_GetInputData(0, 0);
    *(&task_C8 + 1) = Script_GetInputData(1, 0);
    *(&task_C8 + 2) = Script_GetInputData(2, 0);
    *(&task_C8 + 3) = Script_GetInputData(3, 0);
    task_D8 = Script_GetInputData(0, 1);
    *(&task_D8 + 1) = Script_GetInputData(1, 1);
    *(&task_D8 + 2) = Script_GetInputData(2, 1);
    *(&task_D8 + 3) = Script_GetInputData(3, 1);
    task_E8 = Script_GetInputData(0, 2);
    *(&task_E8 + 1) = Script_GetInputData(1, 2);
    *(&task_E8 + 2) = Script_GetInputData(2, 2);
    *(&task_E8 + 3) = Script_GetInputData(3, 2);
    if (*(&task_D8 + 1) & 2) {
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
  end:;
}

static void script_B027(int32_t arg0) {
  task_FC = arg0;
  Script_seqEnterEvtCam(arg0, 0, AUTH_None);
  return;
}

static int32_t script_B03B(int32_t arg0, int32_t arg1, int32_t arg2) {
  SCNF_WaitFrames_0000(arg2);
  script_B027(arg0);
  SCNF_WaitFrames_0000(1);
  Script_AllocSpline(arg1);
  end:;
}

static void script_B05D(int32_t arg0, int32_t arg1, int32_t arg2) {
  arg2 = arg2 / 2;
  if (arg2) {
    arg2 = arg2 - 1;
    Script_scenCreateTask(script_B03B, 3, arg0, arg1, arg2);
  } else {
    script_B027(arg0);
    SCNF_WaitFrames_0000(1);
    Script_AllocSpline(arg1);
  }
  return;
}

static void script_B0A3(int32_t arg0, int32_t arg1, int32_t arg2) {
  task_FC = arg0;
  Script_seqEnterEvtCam(arg0, arg1, arg2);
  return;
}

static void script_B0B7(int32_t arg0) {
  for (;;) {
    SCNF_WaitFrames_0000(1);
    if (Script_IsCharaMotionFinished(arg0)) {
      break;
    }
  }
  return;
}

static void script_B0D1(void) {
  for (;;) {
    SCNF_WaitFrames_0000(1);
    if (Script_IsInEventCam() == 0) {
      break;
    }
  }
  return;
}

static void script_B0EC(int32_t arg0) {
  script_B027(arg0);
  SCNF_WaitFrames_0000(2);
  for (;;) {
    script_B0D1();
    script_B027(arg0);
    SCNF_WaitFrames_0000(2);
  }
  return;
}

static void script_B119(int32_t arg0, int32_t arg1) {
  while (arg0) {
    SCNF_WaitFrames_0000(1);
    arg0 = arg0 - 1;
  }
  Script_executeAicaCommand(arg1, 0, 0);
  return;
}

static void script_B140(void) {
  task_604 = 0;
  return;
}

static void script_B14A(void) {
  for (;;) {
    if (task_604 == 1) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_B165(void) {
  task_604 = 1;
  return;
}

static void script_B16F(int32_t arg0) {
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

static int32_t script_B1D3(float arg0, float arg1, float arg2, float arg3) {
  int32_t unused_FC;
  int32_t local_F4[2];
  arg0 = arg2 - arg0;
  arg1 = arg3 - arg1;
  return local_F4[1] = Script_FloatUtils(ScriptMathOp_Atan2ReverseVector, arg0, arg1);
}

static int32_t script_B1FA(int32_t arg0, int32_t arg1) {
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

static int32_t script_B272(int32_t arg0, int32_t arg1, int32_t arg2, const Vector3 *arg3) {
  int8_t local_FF;
  local_FF = 0;
  for (;;) {
    task_608[arg1] = Script_wmptUtils(arg0, arg3, arg2);
    if (task_608[arg1] == 0) {
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

static void script_B31E(int32_t arg0, int32_t arg1, int32_t arg2) {
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
    task_608[arg1] = 0;
  } else {
    local_DC[0] = *(float *)arg2 - (&local_F4)->x;
    local_DC[2] = *(float *)(arg2 + 8) - (&local_F4)->z;
    local_C0[1] = script_B1D3((&local_F4)->x, (&local_F4)->z, *(float *)arg2, *(float *)(arg2 + 8));
    local_D8 = script_B1FA(local_C0[1], (&local_E8)->y);
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
    task_608[arg1] = 1000;
    local_B4 = Script_scenCreateTask(script_B272, 4, arg0, arg1, local_BC, arg2);
  }
  return;
}

static void script_B4E4(int32_t arg0) {
  for (;;) {
    if (task_608[arg0] == 0) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_B506(int32_t arg0, int32_t arg1, int32_t arg2) {
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

static int32_t script_B5E1(void) {
  for (;;) {
    task_10C = task_10C + 1;
    task_110 = task_110 + 1;
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_B605(int32_t arg0) {
  arg0 = 0;
  *(int8_t *)task_130[arg0] = *(int8_t *)&task_124;
  *(int8_t *)&task_130[arg0][1] = *((int8_t *)&task_124 + 1);
  *(int8_t *)&task_130[arg0][2] = *((int8_t *)&task_124 + 2);
  *(int8_t *)&task_130[arg0][3] = *((int8_t *)&task_124 + 3);
  *(int8_t *)&task_130[arg0][4] = *((int8_t *)&task_124 + 4);
  *(int8_t *)&task_130[arg0][5] = *((int8_t *)&task_124 + 5);
  *(int8_t *)&task_130[arg0][6] = *((int8_t *)&task_124 + 6);
  *(int8_t *)&task_130[arg0][7] = *((int8_t *)&task_124 + 7);
  *(int32_t *)&task_130[arg0][8] = *(int32_t *)(&task_124 + 8);
  return;
}

static int32_t script_B6C5(int32_t arg0, int32_t arg1) {
  return 0;
}

static int32_t script_B6CB(int32_t arg0) {
  return *(int8_t *)&task_130[0][2];
}

static int32_t script_B6D6(int32_t arg0) {
  int32_t unused_F8[2];
  for (;;) {
    if (arg0 == 9) {
      if ((task_108 != task_104) & (task_104 % 3 == 0)) {
        task_120 = 1;
      }
      if ((task_10C != 0) & (task_10C % 450 == 0)) {
        task_120 = 1;
        task_121 = 1;
      }
    } else {
      if (((task_110 != 0) & (task_110 % 300 == 0)) | ((task_108 != task_104) & (task_104 % 3 == 0))) {
        task_120 = 1;
      }
      if ((task_10C != 0) & (task_10C % 2700 == 0)) {
        task_120 = 1;
        task_123 = 1;
      } else {
        if ((task_10C != 0) & (task_10C % 1800 == 0)) {
          task_120 = 1;
          task_122 = 1;
          goto L00B79D;
        }
      }
    }
    L00B79D:;
    task_108 = task_104;
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_B7B1(int32_t arg0, int32_t arg1) {
  Vector3 local_F4;
  int32_t local_F0;
  task_5F7 = 1;
  local_F0 = 0;
  for (;;) {
    if (task_5F7 == 0) {
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

static int32_t script_B816(int32_t arg0, int32_t arg1) {
  Vector3 local_F4;
  int32_t local_F0;
  task_5F8 = 1;
  local_F0 = 0;
  for (;;) {
    if (task_5F8 == 0) {
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
  end:;
}

static void script_B87B(void) {
  int32_t local_FC;
  task_5F6 = 0;
  local_FC = 0;
  for (;;) {
    *((int8_t *)task_5E0 + local_FC) = 0;
    *((int8_t *)task_5EC + local_FC) = 0;
    local_FC = local_FC + 1;
    if (local_FC >= 10) {
      break;
    }
  }
  return;
}

static void script_B8B7(void) {
  for (;;) {
    if (task_5F6 == 0) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_B8D2(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
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

static void script_B946(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t unused_F4[3];
  script_B165();
  task_5F6 = 1;
  script_B8D2(arg0, arg1, arg2, arg3);
  task_5F6 = 0;
  return;
}

static void script_B96B(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Vector3 local_F4;
  script_B165();
  task_5F6 = 1;
  script_B8D2(arg0, arg1, arg2, arg3);
  Script_EVT_GetPosition(arg1, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  SCNF_CHAR_SCNFApplyFacingTowardPoint(arg0, &local_F4, 0, 0.0f, 0, 0x200000);
  task_5F6 = 0;
  return;
}

static int32_t script_B9B4(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Vector3 local_F4;
  int32_t local_F0;
  script_B165();
  task_5F6 = 1;
  script_B8D2(arg0, arg1, arg2, arg3);
  local_F0 = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f);
  if (local_F0 == 0) {
    Script_EVT_GetPosition(arg1, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
    SCNF_CHAR_SCNFApplyFacingTowardPoint(arg0, &local_F4, 0, 0.0f, 0, 0x200000);
    goto L00BA12;
  }
  L00BA12:;
  task_5F6 = 0;
}

static int32_t script_BA1A(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Vector3 local_F4;
  script_B165();
  task_5F6 = 1;
  SCNF_WaitFrames_0000(arg3);
  Script_charaToggleMotEnabled(arg0, 1);
  Script_motmToggleFlags(arg0, 1, 0x40);
  Script_SetCharaMotionPlaybackRate(arg0, 1.5f);
  Script_PlayCharaMotion(arg0, 20998, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  SCNF_WaitFrames_0000(30);
  Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
  script_B8D2(arg0, arg1, arg2, 0);
  Script_EVT_GetPosition(arg1, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  SCNF_CHAR_SCNFApplyFacingTowardPoint(arg0, &local_F4, 0, 0.0f, 0, 0x200000);
  script_B0B7(arg0);
  Script_PlayCharaMotion(arg0, 21000, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  task_5F6 = 0;
}

static void script_BAE2(int32_t arg0, int32_t arg1, int32_t arg2) {
  Vector3 local_F4;
  task_5F6 = 1;
  Script_EVT_GetPosition(arg1, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  Script_SetLookPointDetails(arg0, 11, &local_F4, 0);
  SCNF_WaitFrames_0000(arg2);
  Script_EVT_GetPosition(arg1, 5, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  Script_SetLookPointDetails(arg0, 11, &local_F4, 0);
  task_5F6 = 0;
  script_B165();
  return;
}

static int32_t script_BB38(void) {
  task_630 = 1;
  task_634 = 0;
  for (;;) {
    if (task_630) {
      task_630 = task_630 - 1;
      if (task_630 == 0) {
        task_634 = 0;
        Script_LCDM_Utils(0, (const void *)task_634);
        Script_sicnUtils(2, 0x800, 0);
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_BB8D(int32_t arg0) {
  task_634 = arg0;
  if (task_634 == 0) {
    task_630 = 0;
    Script_LCDM_Utils(1, 0);
    Script_sicnUtils(2, 0x800, 0);
  } else {
    task_630 = 600;
    Script_LCDM_Utils(1, (const void *)1);
    Script_sicnUtils(0, 0x800, 0);
  }
  Script_LCDM_Utils(0, (const void *)task_634);
  return;
}

static void script_BBE7(const char *arg0, const char *arg1) {
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

static void script_BC37(void) {
  for (;;) {
    Script_PreserveAccumulator_0182(1, 10, 10);
    SCNF_WaitFrames_0000(1);
  }
  return;
}

__attribute__((scn_string_bindings(3, data_1A066, 5, data_1A083, 6, data_1A088, 7, data_1A08D, 8, data_1A092, 9, data_1A097, 10, data_1A098, 11, data_1A09D, 12, data_1A09E, 13, data_1A0A3, 14, data_1A0A4, 15, data_1A0A9, 16, data_1A0AA, 17, data_1A0AF, 18, data_1A0B0, 19, data_1A0B5, 20, data_1A0B6, 21, data_1A0BB, 22, data_1A0BC, 23, data_1A0C1, 24, data_1A0C2, 25, data_1A0C7, 26, data_1A0C8, 27, data_1A0CD, 28, data_1A0CE, 29, data_1A0D3, 31, data_1A0E1)))
static void script_BC50(int32_t arg0) {
  task_678 = arg0;
  task_64C = 0;
  task_650 = 0;
  task_654 = 0;
  task_658 = 0;
  task_65C = 0;
  task_660 = 0;
  task_664 = 0;
  task_668 = 0;
  task_66C = 0;
  Script_sprintf("<<< WazaDenjyu Start >>>\012");
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_scenUtils(SCENARIO_CMD_INIT_SCNT);
  Script_shenClearFreeState(0);
  Script_LoadSND(0, 2, "bgm064.snd", 0);
  Script_LoadSND(0, 4, "a1_chinw.snd", 0);
  Script_LoadSND(0, 5, "battle_1.snd", 0);
  task_AC = 0;
  Script_PreserveAccumulator_0182(0, 0, 0);
  task_63C = task_AC;
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
  Script_CycleManagerUtils(CYCLE_CMD_PROCESS_SND_EXTRA);
  SCNF_WaitFrames_0000(10);
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, "/scene/03/MEND/", "M0W0") == 0) {
    SCNF_WaitFrames_0000(1);
  }
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
  if (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 3, "M0W0") == 0) {
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 2, "M0W0");
  }
  Script_AuthorUtils(0, 0, "M0W0", "");
  Script_AuthorUtils(0, 1, "M0W0", "");
  Script_AuthorUtils(0, 2, "M0W0", "");
  Script_AuthorUtils(0, 3, "M0W0", "");
  Script_AuthorUtils(0, 4, "M0W0", "");
  Script_AuthorUtils(0, 5, "M0W0", "");
  Script_AuthorUtils(0, 6, "M0W0", "");
  Script_AuthorUtils(0, 7, "M0W0", "");
  Script_AuthorUtils(0, 8, "M0W0", "");
  Script_AuthorUtils(0, 9, "M0W0", "");
  Script_AuthorUtils(0, 10, "M0W0", "");
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
  task_648 = Script_LoadHandMot(3, SHEN_NAME('M0W0'), "HMOTM0W0.BIN");
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
  Script_LoadVoiceFile(1, "E1016");
  Script_uiUtils(0, 14, 0);
  Script_SetActionSelectorHUDElementVisibility(1);
  if (0) {
    Script_charaDisplay(CHARA_KISY, CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(CHARA_KISY, CDM_Show);
      goto L00BED1;
    }
  }
  L00BED1:;
  Script_scenToggleVisibleFor(CHARA_KISY, 1);
  if (0) {
    Script_charaDisplay(CHARA_TAIJ, CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(CHARA_TAIJ, CDM_Show);
      goto L00BF05;
    }
  }
  L00BF05:;
  Script_scenToggleVisibleFor(CHARA_TAIJ, 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('BAG_'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('BAG_'), CDM_Show);
      goto L00BF39;
    }
  }
  L00BF39:;
  Script_scenToggleVisibleFor(SHEN_NAME('BAG_'), 1);
  if (0) {
    Script_charaDisplay(SHEN_NAME('BAG0'), CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(SHEN_NAME('BAG0'), CDM_Show);
      goto L00BF6D;
    }
  }
  L00BF6D:;
  Script_scenToggleVisibleFor(SHEN_NAME('BAG0'), 1);
  Script_facemotUtils(CHARA_KISY, 0, 0);
  Script_facemotUtils(CHARA_TAIJ, 0, 0);
  Script_facemotUtils(CHARA_AKIR, 1, 0);
  Script_facemotUtils(CHARA_KISY, 1, 0);
  Script_facemotUtils(CHARA_TAIJ, 1, 0);
  Script_SetCameraFlags(0);
  script_AD05(task_654);
  task_654 = Script_scenCreateTask(script_AED9, 0);
  script_10905();
  return;
}

__attribute__((scn_string_bindings(0, data_1A0E7, 1, data_1A0ED)))
static void script_BFE2(void) {
  if (task_638 != -1) {
    Script_WeatherUtils(WeatherScript_ClearSnowTaskData);
    Script_WeatherUtils_ForFIXC(7, (void *)task_638);
    task_638 = -1;
  }
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 2);
  Script_charaSetRefboardFlagTo(CHARA_KISY, 2);
  Script_charaSetRefboardFlagTo(CHARA_TAIJ, 2);
  Script_motDetailFlagUtil(CHARA_TAIJ, 0, 1);
  Script_motDetailFlagUtil(CHARA_KISY, 0, 1);
  Script_FaceEx(CHARA_AKIR, 0, 0, 0);
  Script_FaceEx(CHARA_KISY, 0, 0, 0);
  Script_FaceEx(CHARA_TAIJ, 0, 0, 0);
  Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
  Script_ResetHandMotion(CHARA_KISY, 0, 0, 0);
  Script_ResetHandMotion(CHARA_TAIJ, 0, 0, 0);
  Script_faceUpdateFor(CHARA_KISY, 0, 0);
  Script_faceUpdateFor(CHARA_TAIJ, 0, 0);
  Script_faceUpdateFor(CHARA_KISY, 1, 0);
  Script_faceUpdateFor(CHARA_TAIJ, 1, 0);
  if (task_654) {
    Script_srchControl((void *)task_654);
    task_654 = 0;
  }
  if (task_658) {
    Script_srchControl((void *)task_658);
    task_658 = 0;
  }
  script_BB8D(0);
  Script_AuthorUtils(AUTH_FREE, 0);
  Script_AuthorUtils(AUTH_FREE, 1);
  Script_AuthorUtils(AUTH_FREE, 2);
  Script_AuthorUtils(AUTH_FREE, 3);
  Script_AuthorUtils(AUTH_FREE, 4);
  Script_AuthorUtils(AUTH_FREE, 5);
  Script_AuthorUtils(AUTH_FREE, 6);
  Script_AuthorUtils(AUTH_FREE, 7);
  Script_AuthorUtils(AUTH_FREE, 8);
  Script_AuthorUtils(AUTH_FREE, 9);
  Script_AuthorUtils(AUTH_FREE, 10);
  Script_FreeTaskData(*&task_648);
  Script_LoadVoiceFile(0, "E1016");
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "M0W0");
  Script_SetMotnFlag(CHARA_AKIR, 0);
  Script_SetMotnFlag(CHARA_KISY, 0);
  Script_SetMotnFlag(CHARA_TAIJ, 0);
  if (-1) {
    Script_charaDisplay(CHARA_KISY, CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(CHARA_KISY, CDM_Show);
      goto L00C1E0;
    }
  }
  L00C1E0:;
  Script_scenToggleVisibleFor(CHARA_KISY, 0);
  if (-1) {
    Script_charaDisplay(CHARA_TAIJ, CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(CHARA_TAIJ, CDM_Show);
      goto L00C214;
    }
  }
  L00C214:;
  Script_scenToggleVisibleFor(CHARA_TAIJ, 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('BAG_'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('BAG_'), CDM_Show);
      goto L00C248;
    }
  }
  L00C248:;
  Script_scenToggleVisibleFor(SHEN_NAME('BAG_'), 0);
  if (-1) {
    Script_charaDisplay(SHEN_NAME('BAG0'), CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(SHEN_NAME('BAG0'), CDM_Show);
      goto L00C27C;
    }
  }
  L00C27C:;
  Script_scenToggleVisibleFor(SHEN_NAME('BAG0'), 0);
  if (task_64C) {
    Script_CycleManagerUtils(CYCLE_CMD_LOAD_OR_RESET_RUNNING_PROGRAM, task_64C);
  }
  if (task_650) {
    Script_CycleManagerUtils(CYCLE_CMD_LOAD_OR_RESET_RUNNING_PROGRAM, task_650);
  }
  Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT, 0x5953494B, 0);
  Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT, 0x4A494154, 0);
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 1);
  Script_SetActionSelectorHUDElementVisibility(0);
  Script_uiUtils(0, 0, 0);
  Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 974, 1);
  Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 974, 1);
  Script_SetCameraFlags(1);
  Script_PreserveAccumulator_0182(0, 0, 0);
  return;
}

static void script_C30A(int32_t arg0) {
  int32_t unused_F0[4];
  if (task_638 != -1) {
    Script_WeatherUtils_ForFIXC(10);
    Script_WeatherUtils(WeatherScript_ClearSnowTaskData);
    Script_WeatherUtils_ForFIXC(7, (void *)task_638);
    task_638 = -1;
  }
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 60.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 50.0f);
  Script_SetMapFlags(0, 1);
  Script_WeatherTaskUtil(InitTaskData);
  Script_GetSetWeatherSettings(WEATHER_SETTINGS_SET_DISABLE_WEATHER, 1);
  Script_WeatherUtils_0(WeatherDataUtil_ApplyCurrentWeatherOrEnableDateTimeRead, 0);
  Script_timeSetUpdateEnabled(1);
  return;
}

static void script_C375(int32_t arg0, int32_t arg1) {
  Vector3 local_F4;
  int32_t local_F0;
  int32_t local_EC;
  task_5F8 = 0;
  Script_shenClearFreeState(1);
  Script_sicnUtils(0, 1, 1);
  Script_MT5Loader_TRWV_TRWAVE(22, task_678, 3, 2);
  SCNF_WaitFrames_0000(1);
  Script_SetCharaLookAtPosition(CHARA_AKIR, -20, 0, 0);
  {
    int32_t local_E8;
    local_E8 = task_670;
    if ((local_E8 == 0) | (local_E8 == 1) | (local_E8 == 2)) {
      local_F0 = Script_scenCreateTask(script_B7B1, 2, CHARA_KISY, -1);
      local_EC = Script_scenCreateTask(script_B7B1, 2, CHARA_TAIJ, -1);
    } else {
      if ((local_E8 == 3) | (local_E8 == 9)) {
        Script_SetCharaLookAtPosition(CHARA_KISY, -20, 0, 0);
        local_EC = Script_scenCreateTask(script_B7B1, 2, CHARA_TAIJ, -1);
        goto L00C43A;
      }
    }
  }
  L00C43A:;
  Script_ReinitFightState(0, CHARA_AKIR, CHARA_KISY, arg0, arg1, &task_124);
  for (;;) {
    Script_fightingStyle();
    if (*(int8_t *)&task_124 == 2) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_shenClearFreeState(0);
  Script_sicnUtils(2, 1, 0);
  {
    int32_t local_E8;
    local_E8 = task_670;
    if ((local_E8 == 0) | (local_E8 == 1) | (local_E8 == 2)) {
      if (local_F0) {
        Script_srchControl((void *)local_F0);
        local_F0 = 0;
      }
      if (local_EC) {
        Script_srchControl((void *)local_EC);
        local_EC = 0;
      }
    } else {
      if ((local_E8 == 3) | (local_E8 == 9)) {
        if (local_EC) {
          Script_srchControl((void *)local_EC);
          local_EC = 0;
        }
        goto L00C4E7;
      }
    }
  }
  L00C4E7:;
  Script_EVT_GetPosition(CHARA_AKIR, 5, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  Script_SetLookPointDetails(CHARA_TAIJ, 12, &local_F4, 0);
  Script_MT5Loader_TRWV_TRWAVE(22, task_678, 3, 1);
  Script_sprintf(" stat %d/resutl %d/cause %d/line %d\012", *(int8_t *)&task_124, *((int8_t *)&task_124 + 1), *((int8_t *)&task_124 + 2), *((int8_t *)&task_124 + 3));
  Script_sprintf(data_1A117, *((int8_t *)&task_124 + 5), *((int8_t *)&task_124 + 6), *(int32_t *)(&task_124 + 8));
  return;
}

static int32_t script_C558(void) {
  if (0) {
    Script_charaDisplay(CHARA_AKIR, CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(CHARA_AKIR, CDM_Show);
      goto L00C582;
    }
  }
  L00C582:;
  Script_scenToggleVisibleFor(CHARA_AKIR, 1);
  if (0) {
    Script_charaDisplay(CHARA_KISY, CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(CHARA_KISY, CDM_Show);
      goto L00C5B6;
    }
  }
  L00C5B6:;
  Script_scenToggleVisibleFor(CHARA_KISY, 1);
  if (0) {
    Script_charaDisplay(CHARA_TAIJ, CDM_Hide);
  } else {
    if (-1) {
      Script_charaDisplay(CHARA_TAIJ, CDM_Show);
      goto L00C5EA;
    }
  }
  L00C5EA:;
  Script_scenToggleVisibleFor(CHARA_TAIJ, 1);
  Script_charaDisplay(CHARA_AKIR, CDM_Show);
  Script_charaDisplay(CHARA_KISY, CDM_Show);
  Script_charaDisplay(CHARA_TAIJ, CDM_Show);
  Script_SetCPAMFlags(CHARA_AKIR, 0);
  Script_SetCPAMFlags(CHARA_TAIJ, 0);
  Script_SetCPAMFlags(CHARA_KISY, 0);
  Script_FaceEx(CHARA_AKIR, 1, 0, 2);
  Script_FaceEx(CHARA_KISY, 1, 0, 2);
  Script_FaceEx(CHARA_TAIJ, 1, 0, 2);
  Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
  Script_ResetHandMotion(CHARA_AKIR, 1, 0, 0);
  Script_ResetHandMotion(CHARA_KISY, 0, 1, 0);
  Script_ResetHandMotion(CHARA_KISY, 0, 0, 0);
  Script_ResetHandMotion(CHARA_TAIJ, 0, 1, 0);
  Script_ResetHandMotion(CHARA_TAIJ, 0, 0, 0);
  Script_ccowUtils(CHARA_AKIR, 2);
  Script_ccowUtils(CHARA_KISY, 2);
  Script_ccowUtils(CHARA_TAIJ, 2);
  Script_EVT_SetPosition(CHARA_AKIR, 0x78000000, &data_19ED8);
  {
    int32_t unused_F4[3];
    Vector3i local_E8;
    (&local_E8)->y = 6371;
    Script_EVT_SetRotation(CHARA_AKIR, MATRIX_XFORM_Y, &local_E8, 0);
  }
  Script_EVT_SetPosition(CHARA_KISY, 0x78000000, &data_19EE4);
  {
    int32_t unused_F4[3];
    Vector3i local_E8;
    (&local_E8)->y = 38229;
    Script_EVT_SetRotation(CHARA_KISY, MATRIX_XFORM_Y, &local_E8, 0);
  }
  Script_EVT_SetPosition(CHARA_TAIJ, 0x78000000, &data_19EF0);
  {
    int32_t unused_F4[3];
    Vector3i local_E8;
    (&local_E8)->y = 36408;
    Script_EVT_SetRotation(CHARA_TAIJ, MATRIX_XFORM_Y, &local_E8, 0);
  }
  Script_InitializeOsag(CHARA_AKIR);
  Script_InitializeOsag(CHARA_KISY);
  Script_InitializeOsag(CHARA_TAIJ);
  Script_clmdSetFlags(CHARA_AKIR, 8, 1);
  Script_clmdSetFlags(CHARA_KISY, 8, 1);
  Script_clmdSetFlags(CHARA_TAIJ, 8, 1);
  Script_ccowSetFlag80(CHARA_AKIR, 2);
  Script_ccowSetFlag80(CHARA_TAIJ, 2);
  Script_ccowSetFlag80(CHARA_KISY, 2);
  Script_scenToggleActiveFor(CHARA_AKIR, 1);
  Script_scenToggleActiveFor(CHARA_KISY, 1);
  Script_scenToggleActiveFor(CHARA_TAIJ, 1);
  Script_SetCharaFlags(CHARA_AKIR, 4);
  Script_SetCharaFlags(CHARA_KISY, 4);
  Script_SetCharaFlags(CHARA_TAIJ, 4);
  Script_motmGetCharaFlags(CHARA_AKIR, 0, 0);
  Script_motmGetCharaFlags(CHARA_KISY, 0, 0);
  Script_motmGetCharaFlags(CHARA_TAIJ, 0, 0);
  Script_charaToggleMotEnabled(CHARA_AKIR, 1);
  Script_charaToggleMotEnabled(CHARA_KISY, 0);
  Script_charaToggleMotEnabled(CHARA_TAIJ, 0);
  Script_SetCharaMotionPlaybackRate(CHARA_AKIR, 1.0f);
  Script_SetCharaMotionPlaybackRate(CHARA_KISY, 1.0f);
  Script_SetCharaMotionPlaybackRate(CHARA_TAIJ, 1.0f);
  Script_motmToggleFlags(CHARA_AKIR, 2, 0x40);
  Script_motmToggleFlags(CHARA_KISY, 2, 0x40);
  Script_motmToggleFlags(CHARA_TAIJ, 2, 0x40);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_STAND_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_PlayCharaMotion(CHARA_TAIJ, 12300, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_PlayCharaMotion(CHARA_KISY, 12290, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_charaToggleMotEnabled(CHARA_KISY, 1);
  Script_charaToggleMotEnabled(CHARA_TAIJ, 1);
  Script_SetNewHandMotion(CHARA_AKIR, 1, 701, 0, 0, 0, 0, 1.0f, 0);
  Script_SetNewHandMotion(CHARA_AKIR, 0, 701, 0, 0, 0, 0, 1.0f, 0);
  Script_SetHandRootRotation(CHARA_AKIR, 0, 0x15, &data_1A260);
  SCNF_WaitFrames_0000(1);
  Script_motmToggleFlags(CHARA_AKIR, 1, 0x40);
  Script_motmToggleFlags(CHARA_KISY, 1, 0x40);
  Script_motmToggleFlags(CHARA_TAIJ, 1, 0x40);
  Script_FaceLookAtTargetUtils(CHARA_AKIR, 0);
  Script_FaceLookAtTargetUtils(CHARA_KISY, 0);
  Script_FaceLookAtTargetUtils(CHARA_TAIJ, 0);
  Script_SetVector4ByOffset(&data_1A26C);
  Script_SetCharaRefBoardPos(CHARA_AKIR, &data_1A26C);
  Script_SetCharaRefBoardPos(CHARA_TAIJ, &data_1A26C);
  Script_SetCharaRefBoardPos(CHARA_KISY, &data_1A26C);
  (void)1;
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 1);
  Script_charaSetRefboardFlagTo(CHARA_TAIJ, 1);
  Script_charaSetRefboardFlagTo(CHARA_KISY, 1);
  script_B165();
  end:;
}

static void script_CA42(int32_t arg0, int32_t arg1) {
  Vector3 local_F4;
  Vector3i local_E8;
  Script_ryoControl(PCCTL_SetCPCTFlag400, 0);
  Script_ccowUtils(arg1, 2);
  Script_motmToggleFlags(arg0, 2, 0x40);
  Script_RequestFightAnimation(arg0, 657, 0.0f, 14, 1);
  SCNF_WaitFrames_0000(1);
  local_F4.x = 0.028f;
  (&local_F4)->y = 0.359f;
  (&local_F4)->z = -0.018f;
  local_E8.x = 0xC000;
  (&local_E8)->y = 47204;
  (&local_E8)->z = 0;
  Script_fixoAttachEx(arg1, arg0, 18, &local_F4, &local_E8);
  Script_CharHandlers(arg0, 0, 0, 16);
  Script_CharHandlers(arg0, 1, 8, 16);
  Script_charaDisplay(arg1, CDM_Show);
  Script_tmnmUtils(arg1, 1333, 0.0f, 32767.0f, 0.0f, 1.0f, 0);
  return;
}

static void script_CB01(void) {
  task_680 = 0;
  script_B165();
  script_B0B7(CHARA_AKIR);
  SCNF_WaitFrames_0000(10);
  {
    Vector3 local_F4;
    Script_EVT_GetPosition(CHARA_KISY, 5, &local_F4, ENTITY_XFORM_WORLD_SPACE);
    Script_SetLookPointDetails(CHARA_AKIR, 12, &local_F4, 0);
  }
  SCNF_WaitFrames_0000(90);
  Script_charaToggleMotEnabled(CHARA_AKIR, 1);
  Script_SetCharaMotionPlaybackRate(CHARA_AKIR, 1.0f);
  Script_SetCharaLookAtPosition(CHARA_AKIR, -98312, 0, 0);
  {
    Vector3 local_F4;
    Script_EVT_GetPosition(CHARA_KISY, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
    SCNF_CHAR_SCNFApplyFacingTowardPoint(CHARA_AKIR, &local_F4, 0, 0.0f, 0, 0x200000);
    script_B0B7(CHARA_AKIR);
    Script_EVT_GetPosition(CHARA_KISY, 5, &local_F4, ENTITY_XFORM_WORLD_SPACE);
    Script_SetLookPointDetails(CHARA_AKIR, 12, &local_F4, 1);
  }
  task_680 = 1;
  return;
}

static void script_CBD8(void) {
  Vector3 local_F4;
  script_B165();
  task_5F8 = 0;
  SCNF_WaitFrames_0000(30);
  Script_EVT_GetPosition(CHARA_TAIJ, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  Script_SetLookPointDetails(CHARA_AKIR, 12, &local_F4, 0);
  SCNF_WaitFrames_0000(30);
  Script_EVT_GetPosition(CHARA_TAIJ, 5, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  Script_SetLookPointDetails(CHARA_AKIR, 12, &local_F4, 0);
  Script_PlayCharaMotion(CHARA_KISY, 12293, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  return;
}

static void script_CC5D(int32_t arg0) {
  Vector3 local_F4;
  script_B165();
  SCNF_WaitFrames_0000(arg0);
  Script_EVT_GetPosition(CHARA_TAIJ, 5, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  Script_SetLookPointDetails(CHARA_KISY, 12, &local_F4, 0);
  return;
}

static void script_CC8F(int32_t arg0) {
  Vector3 local_F4;
  script_B165();
  SCNF_WaitFrames_0000(arg0);
  Script_EVT_GetPosition(CHARA_AKIR, 5, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  Script_SetLookPointDetails(CHARA_KISY, 12, &local_F4, 0);
  return;
}

static void script_CCC1(int32_t arg0) {
  Vector3 local_F4;
  script_B165();
  SCNF_WaitFrames_0000(arg0);
  Script_EVT_GetPosition(CHARA_KISY, 5, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  Script_SetLookPointDetails(CHARA_TAIJ, 12, &local_F4, 0);
  return;
}

static void script_CCF3(int32_t arg0) {
  Vector3 local_F4;
  script_B165();
  SCNF_WaitFrames_0000(arg0);
  Script_EVT_GetPosition(CHARA_AKIR, 5, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  Script_SetLookPointDetails(CHARA_TAIJ, 12, &local_F4, 0);
  return;
}

static void script_CD25(int32_t arg0, int32_t arg1) {
  Vector3 local_F4;
  script_B165();
  SCNF_WaitFrames_0000(arg1);
  Script_EVT_GetPosition(CHARA_KISY, 5, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  Script_SetLookPointDetails(CHARA_AKIR, arg0, &local_F4, 0);
  return;
}

static void script_CD57(int32_t arg0) {
  Vector3 local_F4;
  script_B165();
  SCNF_WaitFrames_0000(arg0);
  Script_EVT_GetPosition(CHARA_TAIJ, 5, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  Script_SetLookPointDetails(CHARA_AKIR, 12, &local_F4, 0);
  return;
}

static void script_CD89(void) {
  Vector3 local_F4;
  script_B165();
  Script_SetCharaLookAtPosition(CHARA_AKIR, -98312, 0, 0);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_ESHAKU, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  script_B0B7(CHARA_AKIR);
  Script_EVT_GetPosition(CHARA_TAIJ, 5, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  Script_SetLookPointDetails(CHARA_AKIR, 12, &local_F4, 0);
  return;
}

static void script_CDF2(int32_t arg0, int32_t arg1) {
  Vector3 local_F4;
  Vector3i local_E8;
  if (Script_charaDisplay(arg1, CDM_Query)) {
    Script_fixoClearAttachment(arg1);
    Script_SetRyoMotionDetails(arg0);
    Script_CharHandlers(arg0, -1, 0, 0);
    Script_EVT_GetPosition(arg1, -1, &local_F4, 0);
    (&local_F4)->y = -0.3f;
    local_E8.x = 47136;
    (&local_E8)->y = 0;
    (&local_E8)->z = 0x8000;
    Script_EVT_SetPosition(arg1, POS_XYZ, &local_F4);
    Script_EVT_SetRotation(arg1, 0x38000000, &local_E8, 0);
  }
  return;
}

static void script_CE6C(void) {
  int32_t unused_F4[3];
  script_B165();
  return;
}

static int32_t script_CE74(void) {
  Vector3 local_F4;
  script_B165();
  Script_SetCharaLookAtPosition(CHARA_AKIR, -98312, 0, 0);
  Script_PlayCharaMotion(CHARA_AKIR, 12289, 0.0f, 16.0f, -1.0f, 0.5f, 0);
  script_B0B7(CHARA_AKIR);
  Script_EVT_GetPosition(CHARA_TAIJ, 5, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  Script_SetLookPointDetails(CHARA_AKIR, 12, &local_F4, 0);
  end:;
}

static void script_CEDB(void) {
  int32_t unused_F4[3];
  script_B165();
  return;
}

static void script_CEE3(void) {
  Vector3 local_F4;
  script_B165();
  Script_EVT_SetPosition(CHARA_AKIR, 0x78000000, &data_19EFC);
  {
    Vector3i local_E8;
    (&local_E8)->y = 54613;
    Script_EVT_SetRotation(CHARA_AKIR, MATRIX_XFORM_Y, &local_E8, 0);
  }
  Script_EVT_SetPosition(CHARA_KISY, 0x78000000, &data_19F08);
  {
    Vector3i local_E8;
    (&local_E8)->y = 0xA000;
    Script_EVT_SetRotation(CHARA_KISY, MATRIX_XFORM_Y, &local_E8, 0);
  }
  Script_EVT_SetPosition(CHARA_TAIJ, 0x78000000, &data_19F14);
  {
    Vector3i local_E8;
    (&local_E8)->y = 12743;
    Script_EVT_SetRotation(CHARA_TAIJ, MATRIX_XFORM_Y, &local_E8, 0);
  }
  Script_InitializeOsag(CHARA_AKIR);
  Script_InitializeOsag(CHARA_KISY);
  Script_InitializeOsag(CHARA_TAIJ);
  SCNF_WaitFrames_0000(1);
  Script_EVT_GetPosition(CHARA_AKIR, 5, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  Script_SetLookPointDetails(CHARA_TAIJ, 12, &local_F4, 1);
  Script_EVT_GetPosition(CHARA_TAIJ, 5, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  Script_SetLookPointDetails(CHARA_AKIR, 12, &local_F4, 1);
  Script_EVT_GetPosition(CHARA_AKIR, 5, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  Script_SetLookPointDetails(CHARA_KISY, 12, &local_F4, 1);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_STAND_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  return;
}

static void script_D029(int32_t arg0, int32_t arg1) {
  Script_motmSetBlendFrames(arg0, arg1);
  Script_PlayCharaMotion(arg0, MN_AKI_AKI_STAND_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  script_B0B7(arg0);
  Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_5);
  return;
}

static void script_D065(void) {
  int32_t unused_F4[3];
  script_B165();
  Script_SetCharaLookAtPosition(CHARA_KISY, -98312, 0, 0);
  Script_motmSetBlendFrames(CHARA_KISY, MOTM_BLEND_FRAMES_10);
  Script_PlayCharaMotion(CHARA_KISY, MN_AKI_AKI_SEKKIN_KAMAE_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_motmSetBlendFrames(CHARA_TAIJ, MOTM_BLEND_FRAMES_5);
  Script_PlayCharaMotion(CHARA_TAIJ, 12299, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  script_B0B7(CHARA_TAIJ);
  Script_motmSetBlendFrames(CHARA_TAIJ, MOTM_BLEND_FRAMES_5);
  return;
}

static void script_D0F3(int32_t arg0) {
  Vector3 local_F4;
  float local_F0;
  Script_EVT_GetPosition(arg0, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  local_F0 = Script_GetCharaMotionRemainingFramesScaled(arg0);
  Script_sprintf("pos x y z : %8.4f %8.4f %8.4f : %f\012", (&local_F4)->x, (&local_F4)->y, (&local_F4)->z, local_F0);
  return;
}

static void script_D12A(int32_t arg0, int32_t arg1, int32_t arg2) {
  Vector3 local_F4;
  int32_t unused_F0;
  Script_EVT_GetPosition(arg0, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  Script_PrintDebugString(arg1, arg2, "%8.4f %8.4f %8.4f : %f\012", (&local_F4)->x, (&local_F4)->y, (&local_F4)->z);
  return;
}

static void script_D158(const Vector3 *arg0) {
  Script_sprintf("pos x y z : %8.4f %8.4f %8.4f \012", arg0->x, arg0->y, arg0->z);
  return;
}

__attribute__((scn_string_bindings(0, data_1A2D8)))
static void script_D173(int32_t arg0) {
  Vector3i local_F4;
  int32_t unused_F0;
  Script_mtwkGetSetRotation(arg0, &local_F4, 0x40000000);
  Script_sprintf("ang x y z : %d %d %d\012", (&local_F4)->x * 360 / 0x10000, (&local_F4)->y * 360 / 0x10000, (&local_F4)->z * 360 / 0x10000);
  return;
}

__attribute__((scn_string_bindings(0, data_1A2EE)))
static void script_D1B9(int32_t arg0) {
  Vector3i local_F4;
  Script_mtwkGetSetRotation(arg0, &local_F4, 0x2000000);
  Script_sprintf("ang x y z : %d %d %d\012", (&local_F4)->x * 360 / 0x10000, (&local_F4)->y * 360 / 0x10000, (&local_F4)->z * 360 / 0x10000);
  return;
}

static void script_D1FF(void) {
  Vector3 local_F4;
  int32_t local_F0;
  int32_t unused_E4[3];
  Vector3i local_D8;
  script_B165();
  task_684 = 0;
  local_F0 = Script_scenCreateTask(script_B816, 2, CHARA_TAIJ, -1);
  Script_SetCharaLookAtPosition(CHARA_TAIJ, -0x18000, 0, 0);
  Script_charaToggleMotEnabled(CHARA_TAIJ, 1);
  Script_motmToggleFlags(CHARA_TAIJ, 1, 0x40);
  Script_charaToggleMotEnabled(CHARA_KISY, 1);
  Script_motmToggleFlags(CHARA_KISY, 1, 0x40);
  SCNF_WaitFrames_0000(30);
  Script_PlayCharaMotion(CHARA_KISY, MN_AKI_AKI_BATTLE_SEIKEN_MID, -1.0f, -1.0f, -1.0f, 0.5f, 0);
  SCNF_WaitFrames_0000(15);
  Script_PlayCharaMotion(CHARA_TAIJ, 12303, -1.0f, 6.0f, -1.0f, 0.5f, 0);
  Script_PlayCharaMotion(CHARA_KISY, 12304, -1.0f, 15.0f, -1.0f, 0.5f, 0);
  script_B506(CHARA_KISY, (int32_t)&data_19FEC, -0x8000);
  SCNF_WaitFrames_0000(1);
  script_B506(CHARA_KISY, (int32_t)&data_19FF8, 0);
  script_B0B7(CHARA_TAIJ);
  Script_PlayCharaMotion(CHARA_TAIJ, MN_AKI_AKI_NGS_ENSEI, -1.0f, -1.0f, -1.0f, 0.5f, 0);
  Script_PlayCharaMotion(CHARA_KISY, MN_YKI_AKI_NGR_ENSEI, -1.0f, -1.0f, -1.0f, 0.5f, 0);
  script_B506(CHARA_TAIJ, (int32_t)&data_1A004, 0);
  script_B506(CHARA_KISY, (int32_t)&data_1A01C, 0);
  SCNF_WaitFrames_0000(1);
  script_B506(CHARA_TAIJ, (int32_t)&data_1A010, 0);
  script_B506(CHARA_KISY, (int32_t)&data_1A028, 0);
  script_B0B7(CHARA_TAIJ);
  SCNF_WaitFrames_0000(30);
  script_B027(9227);
  (&local_D8)->y = 38229;
  Script_EVT_SetRotation(CHARA_TAIJ, -0x70000000, &local_D8, 0);
  Script_motmToggleFlags(CHARA_TAIJ, 2, 4);
  Script_PlayCharaMotion(CHARA_TAIJ, MN_AK_SOUKA_HOU_NO_SE, -1.0f, 8.6f, -1.0f, 0.25f, 0);
  script_B0B7(CHARA_TAIJ);
  SCNF_WaitFrames_0000(6);
  Script_executeAicaCommand(0x1A76AB, 0, 0);
  SCNF_WaitFrames_0000(54);
  Script_PlayCharaMotion(CHARA_TAIJ, MN_AK_SOUKA_HOU_NO_SE, -1.0f, -1.0f, 13.0f, 0.25f, 0);
  script_B0B7(CHARA_TAIJ);
  script_B027(9228);
  Script_charaToggleMotEnabled(CHARA_TAIJ, 0);
  Script_motmSetBlendFrames(CHARA_TAIJ, MOTM_BLEND_FRAMES_60);
  Script_PlayCharaMotion(CHARA_TAIJ, MN_AKI_AKI_STAND_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  SCNF_WaitFrames_0000(30);
  Script_PlayCharaMotion(CHARA_KISY, 12296, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  script_B0B7(CHARA_TAIJ);
  Script_motmSetBlendFrames(CHARA_TAIJ, MOTM_BLEND_FRAMES_5);
  script_B0B7(CHARA_KISY);
  Script_PlayCharaMotion(CHARA_KISY, MN_AKI_AKI_STAND_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  if (local_F0) {
    Script_srchControl((void *)local_F0);
    local_F0 = 0;
  }
  task_684 = 1;
  Script_EVT_GetPosition(CHARA_AKIR, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  SCNF_CHAR_SCNFApplyFacingTowardPoint(CHARA_KISY, &local_F4, 0, 0.0f, 0, 0x200000);
  return;
}

static void script_D566(void) {
  Vector3 local_F4;
  Script_SetCharaMotionPlaybackRate(CHARA_TAIJ, 1.0f);
  Script_charaToggleMotEnabled(CHARA_TAIJ, 0);
  Script_EVT_GetPosition(CHARA_AKIR, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  SCNF_CHAR_SCNFApplyFacingTowardPoint(CHARA_TAIJ, &local_F4, 0, 0.0f, 0, 0x200000);
  script_B0B7(CHARA_TAIJ);
  Script_PlayCharaMotion(CHARA_TAIJ, 12302, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_EVT_GetPosition(CHARA_TAIJ, 5, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  Script_SetLookPointDetails(CHARA_AKIR, 12, &local_F4, 1);
  Script_EVT_GetPosition(CHARA_AKIR, 5, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  Script_SetLookPointDetails(CHARA_TAIJ, 12, &local_F4, 1);
  Script_EVT_GetPosition(CHARA_AKIR, 5, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  Script_SetLookPointDetails(CHARA_KISY, 12, &local_F4, 1);
  script_B165();
  return;
}

static void script_D645(void) {
  Script_EVT_SetPosition(CHARA_AKIR, 0x78000000, &data_19F8C);
  Script_EVT_SetPosition(CHARA_KISY, 0x78000000, &data_19F98);
  {
    int32_t unused_FC;
    Vector3i local_F0;
    (&local_F0)->y = 6371;
    Script_EVT_SetRotation(CHARA_AKIR, MATRIX_XFORM_Y, &local_F0, 0);
  }
  {
    int32_t unused_FC;
    Vector3i local_F0;
    (&local_F0)->y = 39139;
    Script_EVT_SetRotation(CHARA_KISY, MATRIX_XFORM_Y, &local_F0, 0);
  }
  Script_charaDisplay(CHARA_TAIJ, CDM_Hide);
  Script_motmToggleFlags(CHARA_AKIR, 2, 0x40);
  Script_motmToggleFlags(CHARA_KISY, 2, 0x40);
  Script_charaToggleMotEnabled(CHARA_AKIR, 1);
  Script_charaToggleMotEnabled(CHARA_KISY, 1);
  Script_PlayCharaMotion(CHARA_KISY, MN_AKI_AKI_SEKKIN_KAMAE_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_SEKKIN_KAMAE_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  SCNF_WaitFrames_0000(1);
  Script_PlayCharaMotion(CHARA_KISY, MN_AKI_AKI_BATTLE_SEIKEN_MID, -1.0f, -1.0f, -1.0f, 0.5f, 0);
  SCNF_WaitFrames_0000(15);
  Script_PlayCharaMotion(CHARA_AKIR, 12303, -1.0f, 6.0f, -1.0f, 0.5f, 0);
  Script_PlayCharaMotion(CHARA_KISY, 12304, -1.0f, 15.0f, -1.0f, 0.5f, 0);
  script_B506(CHARA_KISY, (int32_t)&data_19FEC, -0x8000);
  SCNF_WaitFrames_0000(1);
  script_B506(CHARA_KISY, (int32_t)&data_19FF8, 0);
  script_B0B7(CHARA_AKIR);
  script_D0F3(CHARA_AKIR);
  script_D173(CHARA_AKIR);
  Script_motmToggleFlags(CHARA_AKIR, 1, 0x40);
  Script_motmToggleFlags(CHARA_KISY, 1, 0x40);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_NGS_ENSEI, -1.0f, 26.0f, -1.0f, 0.5f, 0);
  Script_PlayCharaMotion(CHARA_KISY, MN_YKI_AKI_NGR_ENSEI, -1.0f, -1.0f, -1.0f, 0.5f, 0);
  script_B506(CHARA_AKIR, (int32_t)&data_1A004, 0);
  script_B506(CHARA_KISY, (int32_t)&data_1A01C, 0);
  SCNF_WaitFrames_0000(1);
  script_B506(CHARA_AKIR, (int32_t)&data_1A010, 0);
  script_B506(CHARA_KISY, (int32_t)&data_1A028, 0);
  script_B0B7(CHARA_AKIR);
  script_D0F3(CHARA_AKIR);
  script_D173(CHARA_AKIR);
  SCNF_WaitFrames_0000(1);
  script_D0F3(CHARA_AKIR);
  script_D173(CHARA_AKIR);
  Script_SetCharaMotionPlaybackRate(CHARA_AKIR, 1.0f);
  Script_SetCharaMotionPlaybackRate(CHARA_KISY, 1.0f);
  script_B165();
  return;
}

static void script_D907(void) {
  Vector3 local_F4;
  Script_EVT_SetPosition(CHARA_AKIR, 0x78000000, &data_19F8C);
  Script_EVT_SetPosition(CHARA_KISY, 0x78000000, &data_19F98);
  {
    int32_t unused_F0;
    Vector3i local_E4;
    (&local_E4)->y = 6371;
    Script_EVT_SetRotation(CHARA_AKIR, MATRIX_XFORM_Y, &local_E4, 0);
  }
  {
    int32_t unused_F0;
    Vector3i local_E4;
    (&local_E4)->y = 39139;
    Script_EVT_SetRotation(CHARA_KISY, MATRIX_XFORM_Y, &local_E4, 0);
  }
  Script_charaDisplay(CHARA_TAIJ, CDM_Hide);
  Script_motmToggleFlags(CHARA_AKIR, 2, 0x40);
  Script_motmToggleFlags(CHARA_KISY, 2, 0x40);
  Script_charaToggleMotEnabled(CHARA_AKIR, 1);
  Script_charaToggleMotEnabled(CHARA_KISY, 1);
  Script_PlayCharaMotion(CHARA_KISY, MN_AKI_AKI_SEKKIN_KAMAE_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_SEKKIN_KAMAE_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  SCNF_WaitFrames_0000(1);
  Script_PlayCharaMotion(CHARA_KISY, MN_AKI_AKI_BATTLE_SEIKEN_MID, -1.0f, -1.0f, -1.0f, 0.5f, 0);
  SCNF_WaitFrames_0000(15);
  Script_PlayCharaMotion(CHARA_AKIR, 12303, -1.0f, 6.0f, -1.0f, 0.5f, 0);
  Script_PlayCharaMotion(CHARA_KISY, 12304, -1.0f, 15.0f, -1.0f, 0.5f, 0);
  script_B506(CHARA_KISY, (int32_t)&data_19FEC, -0x8000);
  SCNF_WaitFrames_0000(1);
  script_B506(CHARA_KISY, (int32_t)&data_19FF8, 0);
  script_B0B7(CHARA_AKIR);
  Script_motmToggleFlags(CHARA_AKIR, 1, 0x40);
  Script_motmToggleFlags(CHARA_KISY, 1, 0x40);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_NGS_ENSEI, -1.0f, 26.0f, -1.0f, 0.5f, 0);
  Script_PlayCharaMotion(CHARA_KISY, MN_YKI_AKI_NGR_ENSEI, -1.0f, -1.0f, -1.0f, 0.5f, 0);
  script_B506(CHARA_AKIR, (int32_t)&data_1A004, 0);
  script_B506(CHARA_KISY, (int32_t)&data_1A01C, 0);
  SCNF_WaitFrames_0000(1);
  script_B506(CHARA_AKIR, (int32_t)&data_1A010, 0);
  script_B506(CHARA_KISY, (int32_t)&data_1A028, 0);
  script_B0B7(CHARA_AKIR);
  Script_EVT_GetPosition(CHARA_KISY, 5, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  Script_SetLookPointDetails(CHARA_AKIR, 1, &local_F4, 0);
  script_D158(&local_F4);
  {
    int32_t unused_F0;
    Vector3i local_E4;
    (&local_E4)->y = 38229;
    Script_EVT_SetRotation(CHARA_AKIR, -0x70000000, &local_E4, 0);
  }
  Script_PlayCharaMotion(CHARA_AKIR, MN_AK_SOUKA_HOU_NO_SE, -1.0f, 8.65f, -1.0f, 0.25f, 0);
  SCNF_WaitFrames_0000(12);
  Script_executeAicaCommand(30635, 0, 0);
  SCNF_WaitFrames_0000(10);
  Script_executeAicaCommand(0x1A76AB, 0, 0);
  SCNF_WaitFrames_0000(3);
  Script_PlayCharaMotion(CHARA_KISY, 12297, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  script_B0B7(CHARA_AKIR);
  SCNF_WaitFrames_0000(120);
  script_B027(9254);
  Script_FACE_LCG(CHARA_KISY, 1, 2, 0);
  SCNF_WaitFrames_0000(2);
  Script_FACE_LCG(CHARA_KISY, 2, 15, 0);
  SCNF_WaitFrames_0000(15);
  Script_FACE_LCG(CHARA_KISY, 0, 0, 0);
  SCNF_WaitFrames_0000(20);
  script_B027(9257);
  SCNF_WaitFrames_0000(90);
  script_B027(9258);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AK_SOUKA_HOU_NO_SE, -1.0f, -1.0f, 13.0f, 0.25f, 0);
  SCNF_WaitFrames_0000(15);
  Script_SetCharaMotionPlaybackRate(CHARA_AKIR, 1.0f);
  Script_SetCharaMotionPlaybackRate(CHARA_KISY, 1.0f);
  script_B165();
  return;
}

static void script_DCE2(void) {
  Script_motmSetBlendFrames(CHARA_AKIR, MOTM_BLEND_FRAMES_30);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_STAND_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_PlayCharaMotion(CHARA_KISY, 12296, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  script_B0B7(CHARA_KISY);
  Script_PlayCharaMotion(CHARA_KISY, 12293, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  script_B165();
  return;
}

static void script_DD67(void) {
  script_B165();
  Script_motmSetBlendFrames(CHARA_AKIR, MOTM_BLEND_FRAMES_30);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_STAND_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  script_B0B7(CHARA_AKIR);
  Script_motmSetBlendFrames(CHARA_AKIR, MOTM_BLEND_FRAMES_5);
  {
    Vector3 local_F4;
    Script_EVT_GetPosition(CHARA_AKIR, 5, &local_F4, ENTITY_XFORM_WORLD_SPACE);
    Script_SetLookPointDetails(CHARA_KISY, 10, &local_F4, 0);
    Script_EVT_GetPosition(CHARA_AKIR, 5, &local_F4, ENTITY_XFORM_WORLD_SPACE);
    Script_SetLookPointDetails(CHARA_TAIJ, 10, &local_F4, 0);
    Script_EVT_GetPosition(CHARA_TAIJ, 5, &local_F4, ENTITY_XFORM_WORLD_SPACE);
    Script_SetLookPointDetails(CHARA_AKIR, 10, &local_F4, 0);
  }
  Script_charaToggleMotEnabled(CHARA_AKIR, 1);
  {
    Vector3 local_F4;
    Script_EVT_GetPosition(CHARA_TAIJ, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
    SCNF_CHAR_SCNFApplyFacingTowardPoint(CHARA_AKIR, &local_F4, 0, 0.0f, 0, 0x200000);
  }
  return;
}

static void script_DE4F(void) {
  Vector3 local_F4;
  Script_EVT_GetPosition(CHARA_AKIR, 5, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  Script_SetLookPointDetails(CHARA_KISY, 10, &local_F4, 0);
  Script_EVT_GetPosition(CHARA_AKIR, 5, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  Script_SetLookPointDetails(CHARA_TAIJ, 10, &local_F4, 0);
  return;
}

static int32_t script_DE96(void) {
  script_B165();
  Script_EVT_SetPosition(CHARA_AKIR, 0x78000000, &data_19FD4);
  Script_EVT_SetPosition(CHARA_KISY, 0x78000000, &data_19FE0);
  {
    int32_t unused_F4[3];
    Vector3i local_E8;
    (&local_E8)->y = 52246;
    Script_EVT_SetRotation(CHARA_AKIR, MATRIX_XFORM_Y, &local_E8, 0);
  }
  {
    int32_t unused_F4[3];
    Vector3i local_E8;
    (&local_E8)->y = 64079;
    Script_EVT_SetRotation(CHARA_KISY, MATRIX_XFORM_Y, &local_E8, 0);
  }
  Script_motmSetBlendFrames(CHARA_AKIR, MOTM_BLEND_FRAMES_30);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_STAND_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  script_B0B7(CHARA_KISY);
  Script_PlayCharaMotion(CHARA_KISY, MN_AKI_AKI_STAND_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  script_B0B7(CHARA_AKIR);
  Script_motmSetBlendFrames(CHARA_AKIR, MOTM_BLEND_FRAMES_5);
  {
    Vector3 local_F4;
    Script_EVT_GetPosition(CHARA_AKIR, 5, &local_F4, ENTITY_XFORM_WORLD_SPACE);
    Script_SetLookPointDetails(CHARA_KISY, 10, &local_F4, 0);
    Script_EVT_GetPosition(CHARA_AKIR, 5, &local_F4, ENTITY_XFORM_WORLD_SPACE);
    Script_SetLookPointDetails(CHARA_TAIJ, 10, &local_F4, 0);
    Script_EVT_GetPosition(CHARA_TAIJ, 5, &local_F4, ENTITY_XFORM_WORLD_SPACE);
    Script_SetLookPointDetails(CHARA_AKIR, 10, &local_F4, 0);
  }
  Script_charaToggleMotEnabled(CHARA_AKIR, 1);
  {
    Vector3 local_F4;
    Script_EVT_GetPosition(CHARA_TAIJ, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
    SCNF_CHAR_SCNFApplyFacingTowardPoint(CHARA_AKIR, &local_F4, 0, 0.0f, 0, 0x200000);
  }
  end:;
}

static void script_E00D(void) {
  int32_t unused_F4[3];
  script_B165();
  script_B96B(CHARA_AKIR, CHARA_TAIJ, 5, 10);
  Script_SetCharaLookAtPosition(CHARA_AKIR, -98312, 0, 0);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_OJIGI_ST, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  script_B0B7(CHARA_AKIR);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_OJIGI_EN, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  script_B0B7(CHARA_AKIR);
  return;
}

static int32_t script_E096(void) {
  Vector3 local_F4;
  script_B165();
  Script_EVT_SetPosition(CHARA_AKIR, 0x78000000, &data_19FB0);
  Script_EVT_SetPosition(CHARA_KISY, 0x78000000, &data_19FBC);
  {
    Vector3i local_E8;
    (&local_E8)->y = 34042;
    Script_EVT_SetRotation(CHARA_AKIR, MATRIX_XFORM_Y, &local_E8, 0);
  }
  {
    Vector3i local_E8;
    (&local_E8)->y = 38229;
    Script_EVT_SetRotation(CHARA_KISY, MATRIX_XFORM_Y, &local_E8, 0);
  }
  SCNF_WaitFrames_0000(1);
  Script_EVT_GetPosition(CHARA_AKIR, 5, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  Script_SetLookPointDetails(CHARA_KISY, 10, &local_F4, 0);
  Script_EVT_GetPosition(CHARA_TAIJ, 5, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  Script_SetLookPointDetails(CHARA_AKIR, 10, &local_F4, 0);
  end:;
}

static void script_E14C(void) {
  return;
}

static void script_E14F(void) {
  return;
}

static void script_E152(void) {
  return;
}

static void script_E155(void) {
  return;
}

static void script_E158(void) {
  return;
}

static void script_E15B(void) {
  return;
}

static void script_E15E(void) {
  return;
}

static void script_E161(void) {
  return;
}

static int32_t script_E164(void) {
  end:;
}

static int32_t script_E167(void) {
  end:;
}

static int32_t script_E16A(void) {
  end:;
}

static int32_t script_E16D(void) {
  end:;
}

static void script_E170(void) {
  return;
}

static void script_E173(void) {
  return;
}

static void script_E176(void) {
  return;
}

static void script_E179(void) {
  return;
}

static void script_E17C(void) {
  return;
}

static void script_E17F(void) {
  return;
}

static void script_E182(void) {
  return;
}

static void script_E185(void) {
  return;
}

static void script_E188(void) {
  return;
}

static void script_E18B(void) {
  return;
}

static void script_E18E(void) {
  return;
}

static void script_E191(void) {
  return;
}

static void script_E194(void) {
  return;
}

static void script_E197(void) {
  return;
}

static void script_E19A(void) {
  return;
}

static int32_t script_E19D(void) {
  end:;
}

static int32_t script_E1A0(void) {
  end:;
}

static int32_t script_E1A3(void) {
  end:;
}

static int32_t script_E1A6(void) {
  end:;
}

static int32_t script_E1A9(void) {
  end:;
}

static int32_t script_E1AC(void) {
  end:;
}

static int32_t script_E1AF(void) {
  end:;
}

static int32_t script_E1B2(void) {
  end:;
}

static int32_t script_E1B5(void) {
  end:;
}

static void script_E1B8(void) {
  return;
}

static void script_E1BB(void) {
  return;
}

static void script_E1BE(void) {
  return;
}

static void script_E1C1(void) {
  return;
}

static void script_E1C4(void) {
  return;
}

static void script_E1C7(void) {
  return;
}

static void script_E1CA(void) {
  return;
}

static void script_E1CD(void) {
  return;
}

static void script_E1D0(void) {
  return;
}

static void script_E1D3(void) {
  return;
}

static int32_t script_E1D6(void) {
  end:;
}

static int32_t script_E1D9(void) {
  end:;
}

static int32_t script_E1DC(void) {
  end:;
}

static int32_t script_E1DF(void) {
  end:;
}

static int32_t script_E1E2(void) {
  end:;
}

static int32_t script_E1E5(void) {
  end:;
}

static int32_t script_E1E8(void) {
  end:;
}

static int32_t script_E1EB(void) {
  end:;
}

static int32_t script_E1EE(void) {
  end:;
}

static int32_t script_E1F1(void) {
  end:;
}

static void script_E1F4(void) {
  return;
}

static void script_E1F7(void) {
  return;
}

static void script_E1FA(void) {
  return;
}

static void script_E1FD(void) {
  return;
}

static int32_t script_E200(void) {
  end:;
}

static int32_t script_E203(void) {
  end:;
}

static int32_t script_E206(void) {
  end:;
}

static int32_t script_E209(void) {
  end:;
}

static int32_t script_E20C(void) {
  end:;
}

static int32_t script_E20F(void) {
  end:;
}

static int32_t script_E212(void) {
  end:;
}

static int32_t script_E215(void) {
  end:;
}

static int32_t script_E218(void) {
  end:;
}

static int32_t script_E21B(void) {
  end:;
}

static int32_t script_E21E(void) {
  end:;
}

static int32_t script_E221(void) {
  end:;
}

static int32_t script_E224(void) {
  end:;
}

static int32_t script_E227(void) {
  end:;
}

static int32_t script_E22A(void) {
  end:;
}

static int32_t script_E22D(void) {
  end:;
}

static int32_t script_E230(void) {
  end:;
}

static int32_t script_E233(void) {
  end:;
}

static int32_t script_E236(void) {
  end:;
}

static int32_t script_E239(void) {
  end:;
}

static int32_t script_E23C(void) {
  end:;
}

static int32_t script_E23F(void) {
  end:;
}

static int32_t script_E242(void) {
  end:;
}

static int32_t script_E245(void) {
  end:;
}

static int32_t script_E248(void) {
  end:;
}

static int32_t script_E24B(void) {
  end:;
}

static int32_t script_E24E(void) {
  end:;
}

static int32_t script_E251(void) {
  end:;
}

static int32_t script_E254(void) {
  end:;
}

static void script_E257(void) {
  return;
}

static void script_E25A(void) {
  return;
}

static void script_E25D(void) {
  return;
}

static void script_E260(void) {
  return;
}

static int32_t script_E263(void) {
  end:;
}

static int32_t script_E266(void) {
  end:;
}

static void script_E269(void) {
  return;
}

static void script_E26C(void) {
  return;
}

static void script_E26F(void) {
  return;
}

static void script_E272(void) {
  return;
}

static void script_E275(void) {
  return;
}

static void script_E278(void) {
  return;
}

static void script_E27B(void) {
  return;
}

static void script_E27E(void) {
  return;
}

static void script_E281(void) {
  return;
}

static void script_E284(void) {
  return;
}

static void script_E287(void) {
  return;
}

static void script_E28A(void) {
  return;
}

static void script_E28D(void) {
  return;
}

static void script_E290(void) {
  return;
}

static void script_E293(void) {
  return;
}

static void script_E296(void) {
  return;
}

static void script_E299(void) {
  return;
}

static void script_E29C(void) {
  return;
}

static void script_E29F(void) {
  return;
}

static void script_E2A2(void) {
  return;
}

static void script_E2A5(void) {
  return;
}

static void script_E2A8(void) {
  return;
}

static void script_E2AB(void) {
  return;
}

static void script_E2AE(void) {
  return;
}

static void script_E2B1(void) {
  return;
}

static int32_t script_E2B4(void) {
  end:;
}

static int32_t script_E2B7(void) {
  end:;
}

static int32_t script_E2BA(void) {
  end:;
}

static int32_t script_E2BD(void) {
  end:;
}

static int32_t script_E2C0(void) {
  end:;
}

static int32_t script_E2C3(void) {
  end:;
}

static int32_t script_E2C6(void) {
  end:;
}

static int32_t script_E2C9(void) {
  end:;
}

static int32_t script_E2CC(void) {
  end:;
}

static void script_E2CF(void) {
  return;
}

static void script_E2D2(void) {
  return;
}

static void script_E2D5(void) {
  return;
}

static int32_t script_E2D8(void) {
  end:;
}

static void script_E2DB(void) {
  return;
}

static void script_E2DE(void) {
  return;
}

static int32_t script_E2E1(void) {
  end:;
}

static int32_t script_E2E4(void) {
  end:;
}

static int32_t script_E2E7(void) {
  end:;
}

static int32_t script_E2EA(void) {
  end:;
}

static int32_t script_E2ED(void) {
  end:;
}

static int32_t script_E2F0(void) {
  end:;
}

static int32_t script_E2F3(void) {
  end:;
}

static int32_t script_E2F6(void) {
  end:;
}

static int32_t script_E2F9(void) {
  end:;
}

static int32_t script_E2FC(void) {
  end:;
}

static int32_t script_E2FF(void) {
  end:;
}

static int32_t script_E302(void) {
  end:;
}

static int32_t script_E305(void) {
  end:;
}

static int32_t script_E308(void) {
  end:;
}

static int32_t script_E30B(void) {
  end:;
}

static int32_t script_E30E(void) {
  end:;
}

static void script_E311(void) {
  return;
}

static void script_E314(void) {
  return;
}

static void script_E317(void) {
  return;
}

static void script_E31A(void) {
  return;
}

static void script_E31D(void) {
  return;
}

static void script_E320(void) {
  return;
}

static int32_t script_E323(void) {
  end:;
}

static int32_t script_E326(void) {
  end:;
}

static int32_t script_E329(void) {
  end:;
}

static int32_t script_E32C(void) {
  end:;
}

static void script_E32F(void) {
  return;
}

static void script_E332(void) {
  return;
}

static void script_E335(void) {
  return;
}

static int32_t script_E338(void) {
  end:;
}

static int32_t script_E33B(void) {
  end:;
}

static int32_t script_E33E(void) {
  end:;
}

static int32_t script_E341(void) {
  end:;
}

static void script_E344(void) {
  Script_ShowUIMessageFromID("E1016B001");
  return;
}

static void script_E350(void) {
  Script_ShowUIMessageFromID("E1016A001");
  return;
}

static void script_E35C(void) {
  Script_ShowUIMessageFromID("E1016B002");
  return;
}

static void script_E368(void) {
  Script_ShowUIMessageFromID("E1016A002");
  return;
}

static void script_E374(void) {
  Script_ShowUIMessageFromID("E1016B003");
  return;
}

static void script_E380(void) {
  Script_ShowUIMessageFromID("E1016C001");
  return;
}

static void script_E38C(void) {
  Script_ShowUIMessageFromID("E1016B004");
  return;
}

static void script_E398(void) {
  Script_ShowUIMessageFromID("E1016B005");
  return;
}

static int32_t script_E3A4(void) {
  return Script_ShowUIMessageFromID("E1016A003");
}

static int32_t script_E3B0(void) {
  return Script_ShowUIMessageFromID("E1016B006");
}

static int32_t script_E3BC(void) {
  return Script_ShowUIMessageFromID("E1016B007");
}

static int32_t script_E3C8(void) {
  return Script_ShowUIMessageFromID("E1016A004");
}

static void script_E3D4(void) {
  Script_ShowUIMessageFromID("E1016A005");
  return;
}

static void script_E3E0(void) {
  Script_ShowUIMessageFromID("E1016B008");
  return;
}

static void script_E3EC(void) {
  Script_ShowUIMessageFromID("E1016B009");
  return;
}

__attribute__((scn_string_bindings(0, data_1A39A)))
static void script_E3F8(void) {
  Script_ShowUIMessageFromID("E1016B115");
  return;
}

__attribute__((scn_string_bindings(0, data_1A3A4)))
static void script_E404(void) {
  Script_ShowUIMessageFromID("E1016C002");
  return;
}

static void script_E410(void) {
  Script_ShowUIMessageFromID("E1016B011");
  return;
}

static void script_E41C(void) {
  Script_ShowUIMessageFromID("E1016B012");
  return;
}

static void script_E428(void) {
  Script_ShowUIMessageFromID("E1016B013");
  return;
}

static void script_E434(void) {
  Script_ShowUIMessageFromID("E1016B014");
  return;
}

static void script_E440(void) {
  Script_ShowUIMessageFromID("E1016B015");
  return;
}

static void script_E44C(void) {
  Script_ShowUIMessageFromID("E1016B016");
  return;
}

static void script_E458(void) {
  Script_ShowUIMessageFromID("E1016B017");
  return;
}

static void script_E464(void) {
  Script_ShowUIMessageFromID("E1016B018");
  return;
}

static void script_E470(void) {
  Script_ShowUIMessageFromID("E1016B019");
  return;
}

__attribute__((scn_string_bindings(0, data_1A408)))
static void script_E47C(void) {
  Script_ShowUIMessageFromID("E1016B020");
  return;
}

static int32_t script_E488(void) {
  return Script_ShowUIMessageFromID("E1016B021");
}

static int32_t script_E494(void) {
  return Script_ShowUIMessageFromID("E1016B022");
}

static int32_t script_E4A0(void) {
  return Script_ShowUIMessageFromID("E1016B023");
}

__attribute__((scn_string_bindings(0, data_1A430)))
static int32_t script_E4AC(void) {
  return Script_ShowUIMessageFromID("E1016B020");
}

__attribute__((scn_string_bindings(0, data_1A43A)))
static int32_t script_E4B8(void) {
  return Script_ShowUIMessageFromID("E1016B024");
}

__attribute__((scn_string_bindings(0, data_1A444)))
static int32_t script_E4C4(void) {
  return Script_ShowUIMessageFromID("E1016B025");
}

static int32_t script_E4D0(void) {
  return Script_ShowUIMessageFromID("E1016B026");
}

__attribute__((scn_string_bindings(0, data_1A458)))
static int32_t script_E4DC(void) {
  return Script_ShowUIMessageFromID("E1016B027");
}

static int32_t script_E4E8(void) {
  return Script_ShowUIMessageFromID("E1016B028");
}

static void script_E4F4(void) {
  Script_ShowUIMessageFromID("E1016B029");
  return;
}

static void script_E500(void) {
  Script_ShowUIMessageFromID("E1016B030");
  return;
}

__attribute__((scn_string_bindings(0, data_1A480)))
static void script_E50C(void) {
  Script_ShowUIMessageFromID("E1016B031");
  return;
}

__attribute__((scn_string_bindings(0, data_1A48A)))
static void script_E518(void) {
  Script_ShowUIMessageFromID("E1016B032");
  return;
}

__attribute__((scn_string_bindings(0, data_1A494)))
static void script_E524(void) {
  Script_ShowUIMessageFromID("E1016B033");
  return;
}

__attribute__((scn_string_bindings(0, data_1A49E)))
static void script_E530(void) {
  Script_ShowUIMessageFromID("E1016C003");
  return;
}

__attribute__((scn_string_bindings(0, data_1A4A8)))
static void script_E53C(void) {
  Script_ShowUIMessageFromID("E1016B034");
  return;
}

__attribute__((scn_string_bindings(0, data_1A4B2)))
static void script_E548(void) {
  Script_ShowUIMessageFromID("E1016B035");
  return;
}

__attribute__((scn_string_bindings(0, data_1A4BC)))
static void script_E554(void) {
  Script_ShowUIMessageFromID("E1016C004");
  return;
}

__attribute__((scn_string_bindings(0, data_1A4C6)))
static void script_E560(void) {
  Script_ShowUIMessageFromID("E1016B036");
  return;
}

static int32_t script_E56C(void) {
  return Script_ShowUIMessageFromID("E1016B037");
}

__attribute__((scn_string_bindings(0, data_1A4DA)))
static int32_t script_E578(void) {
  return Script_ShowUIMessageFromID("E1016B038");
}

static int32_t script_E584(void) {
  return Script_ShowUIMessageFromID("E1016B039");
}

static int32_t script_E590(void) {
  return Script_ShowUIMessageFromID("E1016B040");
}

static int32_t script_E59C(void) {
  return Script_ShowUIMessageFromID("E1016B041");
}

static int32_t script_E5A8(void) {
  return Script_ShowUIMessageFromID("E1016B042");
}

__attribute__((scn_string_bindings(0, data_1A50C)))
static int32_t script_E5B4(void) {
  return Script_ShowUIMessageFromID("E1016B027");
}

static int32_t script_E5C0(void) {
  return Script_ShowUIMessageFromID("E1016B043");
}

static int32_t script_E5CC(void) {
  return Script_ShowUIMessageFromID("E1016B044");
}

static int32_t script_E5D8(void) {
  return Script_ShowUIMessageFromID("E1016B053");
}

static void script_E5E4(void) {
  Script_ShowUIMessageFromID("E1016B054");
  return;
}

static void script_E5F0(void) {
  Script_ShowUIMessageFromID("E1016B055");
  return;
}

static void script_E5FC(void) {
  Script_ShowUIMessageFromID("E1016B056");
  return;
}

static void script_E608(void) {
  Script_ShowUIMessageFromID("E1016B059");
  return;
}

static int32_t script_E614(void) {
  return Script_ShowUIMessageFromID("E1016B045");
}

static int32_t script_E620(void) {
  return Script_ShowUIMessageFromID("E1016B046");
}

static int32_t script_E62C(void) {
  return Script_ShowUIMessageFromID("E1016B047");
}

__attribute__((scn_string_bindings(0, data_1A57A)))
static int32_t script_E638(void) {
  return Script_ShowUIMessageFromID("E1016B048");
}

__attribute__((scn_string_bindings(0, data_1A584)))
static int32_t script_E644(void) {
  return Script_ShowUIMessageFromID("E1016B049");
}

__attribute__((scn_string_bindings(0, data_1A58E)))
static int32_t script_E650(void) {
  return Script_ShowUIMessageFromID("E1016B048");
}

__attribute__((scn_string_bindings(0, data_1A598)))
static int32_t script_E65C(void) {
  return Script_ShowUIMessageFromID("E1016B057");
}

static int32_t script_E668(void) {
  return Script_ShowUIMessageFromID("E1016B058");
}

__attribute__((scn_string_bindings(0, data_1A5AC)))
static int32_t script_E674(void) {
  return Script_ShowUIMessageFromID("E1016B049");
}

__attribute__((scn_string_bindings(0, data_1A5B6)))
static int32_t script_E680(void) {
  return Script_ShowUIMessageFromID("E1016B057");
}

__attribute__((scn_string_bindings(0, data_1A5C0)))
static int32_t script_E68C(void) {
  return Script_ShowUIMessageFromID("E1016B060");
}

static int32_t script_E698(void) {
  return Script_ShowUIMessageFromID("E1016B103");
}

static int32_t script_E6A4(void) {
  return Script_ShowUIMessageFromID("E1016B104");
}

static int32_t script_E6B0(void) {
  return Script_ShowUIMessageFromID("E1016B105");
}

__attribute__((scn_string_bindings(0, data_1A5E8)))
static int32_t script_E6BC(void) {
  return Script_ShowUIMessageFromID("E1016B038");
}

static int32_t script_E6C8(void) {
  return Script_ShowUIMessageFromID("E1016B061");
}

static int32_t script_E6D4(void) {
  return Script_ShowUIMessageFromID("E1016B062");
}

static int32_t script_E6E0(void) {
  return Script_ShowUIMessageFromID("E1016B063");
}

static int32_t script_E6EC(void) {
  return Script_ShowUIMessageFromID("E1016B064");
}

static int32_t script_E6F8(void) {
  return Script_ShowUIMessageFromID("E1016B065");
}

static int32_t script_E704(void) {
  return Script_ShowUIMessageFromID("E1016B086");
}

static int32_t script_E710(void) {
  return Script_ShowUIMessageFromID("E1016B087");
}

static int32_t script_E71C(void) {
  return Script_ShowUIMessageFromID("E1016B088");
}

static int32_t script_E728(void) {
  return Script_ShowUIMessageFromID("E1016B089");
}

static int32_t script_E734(void) {
  return Script_ShowUIMessageFromID("E1016B090");
}

__attribute__((scn_string_bindings(0, data_1A656)))
static int32_t script_E740(void) {
  return Script_ShowUIMessageFromID("E1016B060");
}

static int32_t script_E74C(void) {
  return Script_ShowUIMessageFromID("E1016B091");
}

static int32_t script_E758(void) {
  return Script_ShowUIMessageFromID("E1016B066");
}

static int32_t script_E764(void) {
  return Script_ShowUIMessageFromID("E1016B067");
}

static void script_E770(void) {
  Script_ShowUIMessageFromID("E1016A006");
  return;
}

static void script_E77C(void) {
  Script_ShowUIMessageFromID("E1016B068");
  return;
}

__attribute__((scn_string_bindings(0, data_1A692)))
static void script_E788(void) {
  Script_ShowUIMessageFromID("E1016B115");
  return;
}

__attribute__((scn_string_bindings(0, data_1A69C)))
static void script_E794(void) {
  Script_ShowUIMessageFromID("E1016C002");
  return;
}

static int32_t script_E7A0(void) {
  return Script_ShowUIMessageFromID("E1016A007");
}

static int32_t script_E7AC(void) {
  return Script_ShowUIMessageFromID("E1016B069");
}

static void script_E7B8(void) {
  Script_ShowUIMessageFromID("E1016B070");
  return;
}

__attribute__((scn_string_bindings(0, data_1A6C4)))
static void script_E7C4(void) {
  Script_ShowUIMessageFromID("E1016B031");
  return;
}

__attribute__((scn_string_bindings(0, data_1A6CE)))
static void script_E7D0(void) {
  Script_ShowUIMessageFromID("E1016B032");
  return;
}

__attribute__((scn_string_bindings(0, data_1A6D8)))
static void script_E7DC(void) {
  Script_ShowUIMessageFromID("E1016B033");
  return;
}

__attribute__((scn_string_bindings(0, data_1A6E2)))
static void script_E7E8(void) {
  Script_ShowUIMessageFromID("E1016C003");
  return;
}

__attribute__((scn_string_bindings(0, data_1A6EC)))
static void script_E7F4(void) {
  Script_ShowUIMessageFromID("E1016B034");
  return;
}

__attribute__((scn_string_bindings(0, data_1A6F6)))
static void script_E800(void) {
  Script_ShowUIMessageFromID("E1016B035");
  return;
}

__attribute__((scn_string_bindings(0, data_1A700)))
static void script_E80C(void) {
  Script_ShowUIMessageFromID("E1016C004");
  return;
}

__attribute__((scn_string_bindings(0, data_1A70A)))
static void script_E818(void) {
  Script_ShowUIMessageFromID("E1016B036");
  return;
}

static void script_E824(void) {
  Script_ShowUIMessageFromID("E1016A008");
  return;
}

static void script_E830(void) {
  Script_ShowUIMessageFromID("E1016A009");
  return;
}

__attribute__((scn_string_bindings(0, data_1A728)))
static void script_E83C(void) {
  Script_ShowUIMessageFromID("E1016B025");
  return;
}

__attribute__((scn_string_bindings(0, data_1A732)))
static void script_E848(void) {
  Script_ShowUIMessageFromID("E1016B024");
  return;
}

static void script_E854(void) {
  Script_ShowUIMessageFromID("E1016B071");
  return;
}

static void script_E860(void) {
  Script_ShowUIMessageFromID("E1016B072");
  return;
}

static void script_E86C(void) {
  Script_ShowUIMessageFromID("E1016B073");
  return;
}

static void script_E878(void) {
  Script_ShowUIMessageFromID("E1016B092");
  return;
}

static void script_E884(void) {
  Script_ShowUIMessageFromID("E1016B093");
  return;
}

static void script_E890(void) {
  Script_ShowUIMessageFromID("E1016A010");
  return;
}

__attribute__((scn_string_bindings(0, data_1A778)))
static void script_E89C(void) {
  Script_ShowUIMessageFromID("E1016B094");
  return;
}

__attribute__((scn_string_bindings(0, data_1A782)))
static void script_E8A8(void) {
  Script_ShowUIMessageFromID("E1016B095");
  return;
}

static void script_E8B4(void) {
  Script_ShowUIMessageFromID("E1016B097");
  return;
}

static void script_E8C0(void) {
  Script_ShowUIMessageFromID("E1016A014");
  return;
}

__attribute__((scn_string_bindings(0, data_1A7A0)))
static void script_E8CC(void) {
  Script_ShowUIMessageFromID("E1016B094");
  return;
}

__attribute__((scn_string_bindings(0, data_1A7AA)))
static void script_E8D8(void) {
  Script_ShowUIMessageFromID("E1016B095");
  return;
}

__attribute__((scn_string_bindings(0, data_1A7B4)))
static int32_t script_E8E4(void) {
  return Script_ShowUIMessageFromID("E1016A015");
}

__attribute__((scn_string_bindings(0, data_1A7BE)))
static int32_t script_E8F0(void) {
  return Script_ShowUIMessageFromID("E1016B098");
}

__attribute__((scn_string_bindings(0, data_1A7C8)))
static int32_t script_E8FC(void) {
  return Script_ShowUIMessageFromID("E1016C002");
}

static int32_t script_E908(void) {
  return Script_ShowUIMessageFromID("E1016B100");
}

__attribute__((scn_string_bindings(0, data_1A7DC)))
static int32_t script_E914(void) {
  return Script_ShowUIMessageFromID("E1016A016");
}

__attribute__((scn_string_bindings(0, data_1A7E6)))
static int32_t script_E920(void) {
  return Script_ShowUIMessageFromID("E1016B098");
}

__attribute__((scn_string_bindings(0, data_1A7F0)))
static int32_t script_E92C(void) {
  return Script_ShowUIMessageFromID("E1016C002");
}

static int32_t script_E938(void) {
  return Script_ShowUIMessageFromID("E1016A017");
}

static int32_t script_E944(void) {
  return Script_ShowUIMessageFromID("E1016B101");
}

static void script_E950(void) {
  Script_ShowUIMessageFromID("E1016A018");
  return;
}

static void script_E95C(void) {
  Script_ShowUIMessageFromID("E1016B102");
  return;
}

__attribute__((scn_string_bindings(0, data_1A822)))
static void script_E968(void) {
  Script_ShowUIMessageFromID("E1016A013");
  return;
}

__attribute__((scn_string_bindings(0, data_1A82C)))
static int32_t script_E974(void) {
  return Script_ShowUIMessageFromID("E1016A016");
}

__attribute__((scn_string_bindings(0, data_1A836)))
static void script_E980(void) {
  Script_ShowUIMessageFromID("E1016B098");
  return;
}

__attribute__((scn_string_bindings(0, data_1A840)))
static void script_E98C(void) {
  Script_ShowUIMessageFromID("E1016C002");
  return;
}

static int32_t script_E998(void) {
  return Script_ShowUIMessageFromID("E1016B074");
}

static int32_t script_E9A4(void) {
  return Script_ShowUIMessageFromID("E1016B075");
}

static int32_t script_E9B0(void) {
  return Script_ShowUIMessageFromID("E1016B076");
}

static int32_t script_E9BC(void) {
  return Script_ShowUIMessageFromID("E1016B077");
}

static int32_t script_E9C8(void) {
  return Script_ShowUIMessageFromID("E1016B078");
}

static int32_t script_E9D4(void) {
  return Script_ShowUIMessageFromID("E1016B079");
}

static int32_t script_E9E0(void) {
  return Script_ShowUIMessageFromID("E1016B080");
}

static int32_t script_E9EC(void) {
  return Script_ShowUIMessageFromID("E1016B081");
}

static int32_t script_E9F8(void) {
  return Script_ShowUIMessageFromID("E1016B082");
}

static int32_t script_EA04(void) {
  return Script_ShowUIMessageFromID("E1016B083");
}

static int32_t script_EA10(void) {
  return Script_ShowUIMessageFromID("E1016B084");
}

static int32_t script_EA1C(void) {
  return Script_ShowUIMessageFromID("E1016B085");
}

static int32_t script_EA28(void) {
  return Script_ShowUIMessageFromID("E1016B099");
}

__attribute__((scn_string_bindings(0, data_1A8CC)))
static int32_t script_EA34(void) {
  return Script_ShowUIMessageFromID("E1016A015");
}

__attribute__((scn_string_bindings(0, data_1A8D6)))
static int32_t script_EA40(void) {
  return Script_ShowUIMessageFromID("E1016B098");
}

__attribute__((scn_string_bindings(0, data_1A8E0)))
static int32_t script_EA4C(void) {
  return Script_ShowUIMessageFromID("E1016C002");
}

static void script_EA58(void) {
  Script_ShowUIMessageFromID("E1016A011");
  return;
}

static void script_EA64(void) {
  Script_ShowUIMessageFromID("E1016B096");
  return;
}

static void script_EA70(void) {
  Script_ShowUIMessageFromID("E1016C006");
  return;
}

static void script_EA7C(void) {
  Script_ShowUIMessageFromID("E1016A012");
  return;
}

static void script_EA88(void) {
  Script_ShowUIMessageFromID("E1016C007");
  return;
}

__attribute__((scn_string_bindings(0, data_1A91C)))
static void script_EA94(void) {
  Script_ShowUIMessageFromID("E1016A013");
  return;
}

static int32_t script_EAA0(void) {
  return Script_ShowUIMessageFromID("E1016B050");
}

static int32_t script_EAAC(void) {
  return Script_ShowUIMessageFromID("E1016B135");
}

static int32_t script_EAB8(void) {
  return Script_ShowUIMessageFromID("E1016B052");
}

__attribute__((scn_string_bindings(0, data_1A944)))
static int32_t script_EAC4(void) {
  return Script_ShowUIMessageFromID("E1016B027");
}

static void script_EAD0(void) {
  Script_ShowUIMessageFromID("E1016C005");
  return;
}

static void script_EADC(void) {
  Script_ShowUIMessageFromID("E1016B132");
  return;
}

static void script_EAE8(void) {
  Script_ShowUIMessageFromID("E1016B133");
  return;
}

static int32_t script_EAF4(void) {
  return Script_ShowUIMessageFromID("E1016B130");
}

static int32_t script_EB00(void) {
  return Script_ShowUIMessageFromID("E1016B134");
}

static int32_t script_EB0C(void) {
  return Script_ShowUIMessageFromID("E1016B117");
}

__attribute__((scn_string_bindings(0, data_1A98A)))
static int32_t script_EB18(void) {
  return Script_ShowUIMessageFromID("E1016B027");
}

static int32_t script_EB24(int32_t arg0) {
  SCNF_WaitFrames_0000(2);
  for (;;) {
    script_B0D1();
    script_B027(arg0);
    SCNF_WaitFrames_0000(2);
  }
  end:;
}

static int32_t script_EB48(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  int32_t local_F8;
  Script_scenCreateTask(script_E908, 0);
  Script_scenCreateTask(script_E2BD, 0);
  script_B16F(30);
  if (task_68C >= 2) {
    task_688 = 0xFFFF;
  } else {
    task_688 = SCNF_RequestActionSelectorFromParams_0008(0xFFFF, 37, 34, 0xFFFF, 450, 0xFFFF, ACTION_SELECTOR_MODE_STANDARD);
  }
  local_F8 = task_688;
  if ((local_F8 == 34) | (local_F8 == 0xFFFF)) {
    if (task_68C < 2) {
      local_F8 = task_670;
      if ((local_F8 == 0) | (local_F8 == 1) | (local_F8 == 2)) {
        script_B027(9290);
      } else {
        if ((local_F8 == 3) | (local_F8 == 9)) goto L00EC02;
      }
      L00EC02:;
      script_B140();
      Script_scenCreateTask(script_E096, 0);
      script_B14A();
      Script_scenCreateTask(script_E938, 0);
      Script_scenCreateTask(script_E2C9, 0);
      script_B16F(30);
      Script_scenCreateTask(script_E944, 0);
      Script_scenCreateTask(script_E2CC, 0);
      script_B16F(30);
      task_688 = SCNF_RequestActionSelectorFromParams_0008(0xFFFF, 37, 34, 0xFFFF, 450, 0xFFFF, ACTION_SELECTOR_MODE_STANDARD);
    } else {
      task_688 = 0xFFFF;
    }
    local_F8 = task_688;
    if ((local_F8 == 34) | (local_F8 == 0xFFFF)) {
      script_125CC(9);
      local_FC = arg1;
    } else {
      task_68C = task_68C + 1;
      Script_scenCreateTask(script_E974, 0);
      Script_scenCreateTask(script_E2D8, 0);
      script_B16F(30);
      local_F8 = task_670;
      if ((local_F8 == 0) | (local_F8 == 1) | (local_F8 == 2)) {
        Script_scenCreateTask(script_E7AC, 0);
        Script_scenCreateTask(script_E266, 0);
        script_B16F(30);
      } else {
        if ((local_F8 == 3) | (local_F8 == 9)) {
          Script_scenCreateTask(script_E920, 0);
          Script_scenCreateTask(script_E2C3, 0);
          script_B16F(30);
          Script_scenCreateTask(script_E92C, 0);
          Script_scenCreateTask(script_E2C6, 0);
          script_B16F(30);
          goto L00ED69;
        }
      }
      L00ED69:;
      local_FC = arg0;
    }
  } else {
    task_68C = task_68C + 1;
    Script_scenCreateTask(script_E914, 0);
    Script_scenCreateTask(script_E2C0, 0);
    script_B16F(30);
    local_F8 = task_670;
    if ((local_F8 == 0) | (local_F8 == 1) | (local_F8 == 2)) {
      Script_scenCreateTask(script_E7AC, 0);
      Script_scenCreateTask(script_E266, 0);
      script_B16F(30);
    } else {
      if ((local_F8 == 3) | (local_F8 == 9)) {
        Script_scenCreateTask(script_E920, 0);
        Script_scenCreateTask(script_E2C3, 0);
        script_B16F(30);
        Script_scenCreateTask(script_E92C, 0);
        Script_scenCreateTask(script_E2C6, 0);
        script_B16F(30);
        goto L00EE2A;
      }
    }
    L00EE2A:;
    local_FC = arg0;
  }
  return local_FC;
}

static int32_t script_EE34(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  int32_t local_F8;
  script_125CC(7);
  script_B027(9291);
  if (task_68C >= 2) {
    task_688 = 0xFFFF;
  } else {
    task_688 = SCNF_RequestActionSelectorFromParams_0008(0xFFFF, 37, 34, 0xFFFF, 450, 0xFFFF, ACTION_SELECTOR_MODE_STANDARD);
  }
  local_F8 = task_688;
  if (local_F8 == 37) {
    task_68C = task_68C + 1;
    Script_scenCreateTask(script_E8E4, 0);
    Script_scenCreateTask(script_E2B4, 0);
    script_B16F(30);
    Script_scenCreateTask(script_E8F0, 0);
    Script_scenCreateTask(script_E2B7, 0);
    script_B16F(30);
    Script_scenCreateTask(script_E8FC, 0);
    Script_scenCreateTask(script_E2BA, 0);
    script_B16F(30);
    local_FC = arg0;
  } else {
    script_125CC(8);
    local_FC = arg1;
  }
  return local_FC;
}

static int32_t script_EF18(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  int32_t local_F8;
  Script_scenCreateTask(script_EA28, 0);
  Script_scenCreateTask(script_E305, 0);
  script_B16F(30);
  if (task_68C >= 2) {
    task_688 = 0xFFFF;
  } else {
    task_688 = SCNF_RequestActionSelectorFromParams_0008(0xFFFF, 37, 34, 0xFFFF, 450, 0xFFFF, ACTION_SELECTOR_MODE_STANDARD);
  }
  local_F8 = task_688;
  if (local_F8 == 37) {
    task_68C = task_68C + 1;
    Script_scenCreateTask(script_EA34, 0);
    Script_scenCreateTask(script_E308, 0);
    script_B16F(30);
    Script_scenCreateTask(script_EA40, 0);
    Script_scenCreateTask(script_E30B, 0);
    script_B16F(30);
    Script_scenCreateTask(script_EA4C, 0);
    Script_scenCreateTask(script_E30E, 0);
    script_B16F(30);
    local_FC = arg0;
  } else {
    script_125CC(8);
    local_FC = arg1;
  }
  return local_FC;
}

static int32_t script_F008(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  task_114 = task_10C;
  script_125CC(2);
  local_FC = arg0;
  return local_FC;
}

static int32_t script_F025(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  if (task_123) {
    int32_t local_F8;
    task_694 = 1;
    local_F8 = task_670;
    if ((local_F8 == 0) | (local_F8 == 1) | (local_F8 == 2)) {
      script_B027(9287);
      script_10ED7(task_670);
    } else {
      if ((local_F8 == 3) | (local_F8 == 9)) {
        script_B027(9285);
        goto L00F084;
      }
    }
    L00F084:;
    if (arg0 == 1) {
      script_B140();
      Script_scenCreateTask(script_BA1A, 4, CHARA_AKIR, CHARA_TAIJ, 15, 0);
      script_B14A();
    }
    local_FC = script_EB48(arg1, arg3);
    task_123 = 0;
  } else if (task_122) {
    task_694 = 1;
    {
      int32_t local_F8;
      local_F8 = task_670;
      if ((local_F8 == 0) | (local_F8 == 1) | (local_F8 == 2)) {
        script_B027(9287);
        script_10ED7(task_670);
      } else {
        if ((local_F8 == 3) | (local_F8 == 9)) {
          script_B027(9285);
          goto L00F127;
        }
      }
    }
    L00F127:;
    if (arg0 == 1) {
      script_B140();
      Script_scenCreateTask(script_BA1A, 4, CHARA_AKIR, CHARA_TAIJ, 15, 0);
      script_B14A();
    }
    Script_scenCreateTask(script_E758, 0);
    Script_scenCreateTask(script_E251, 0);
    script_B16F(30);
    Script_scenCreateTask(script_E764, 0);
    Script_scenCreateTask(script_E254, 0);
    script_B16F(30);
    task_688 = SCNF_RequestActionSelectorFromParams_0008(0xFFFF, 62, 60, 0xFFFF, 450, 0xFFFF, ACTION_SELECTOR_MODE_STANDARD);
    {
      int32_t local_F8;
      local_F8 = task_670;
      if ((local_F8 == 0) | (local_F8 == 1) | (local_F8 == 2)) {
      } else {
        if ((local_F8 == 3) | (local_F8 == 9)) goto L00F1E6;
      }
    }
    L00F1E6:;
    {
      int32_t local_F8;
      local_F8 = task_688;
      if (local_F8 == 62) {
        script_125CC(3);
        local_FC = arg2;
      } else {
        Script_scenCreateTask(script_E7A0, 0);
        Script_scenCreateTask(script_E263, 0);
        script_B16F(30);
        Script_scenCreateTask(script_E7AC, 0);
        Script_scenCreateTask(script_E266, 0);
        script_B16F(30);
        local_FC = arg1;
      }
    }
    task_122 = 0;
  } else {
    local_FC = arg1;
  }
  return local_FC;
}

static void script_F260(int32_t arg0, int32_t arg1) {
  script_B027(9232);
  script_DE4F();
  script_B140();
  Script_scenCreateTask(script_BA1A, 4, CHARA_AKIR, CHARA_TAIJ, 15, 0);
  script_B14A();
  Script_scenCreateTask(script_E59C, 0);
  Script_scenCreateTask(script_E1E2, 0);
  script_B16F(30);
  Script_scenCreateTask(script_E5A8, 0);
  Script_scenCreateTask(script_E1E5, 0);
  script_B16F(30);
  Script_scenCreateTask(script_E5B4, 0);
  Script_scenCreateTask(script_E1E8, 0);
  script_B16F(30);
  return;
}

static void script_F2F2(int32_t arg0, int32_t arg1) {
  script_B027(9233);
  script_DE4F();
  script_B140();
  Script_scenCreateTask(script_BA1A, 4, CHARA_AKIR, CHARA_TAIJ, 15, 0);
  script_B14A();
  Script_scenCreateTask(script_EAF4, 0);
  Script_scenCreateTask(script_E338, 0);
  script_B16F(30);
  Script_scenCreateTask(script_EB00, 0);
  Script_scenCreateTask(script_E33B, 0);
  script_B16F(30);
  Script_scenCreateTask(script_EB0C, 0);
  Script_scenCreateTask(script_E33E, 0);
  script_B16F(30);
  Script_scenCreateTask(script_EB18, 0);
  Script_scenCreateTask(script_E341, 0);
  script_B16F(30);
  return;
}

static void script_F3A3(int32_t arg0, int32_t arg1) {
  script_B027(9233);
  script_B140();
  Script_scenCreateTask(script_BA1A, 4, CHARA_AKIR, CHARA_TAIJ, 15, 0);
  script_B14A();
  script_DE4F();
  Script_scenCreateTask(script_EAA0, 0);
  Script_scenCreateTask(script_E323, 0);
  script_B16F(30);
  Script_scenCreateTask(script_EAAC, 0);
  Script_scenCreateTask(script_E326, 0);
  script_B16F(30);
  Script_scenCreateTask(script_EAB8, 0);
  Script_scenCreateTask(script_E329, 0);
  script_B16F(30);
  Script_scenCreateTask(script_EAC4, 0);
  Script_scenCreateTask(script_E32C, 0);
  script_B16F(30);
  return;
}

static void script_F454(int32_t arg0, int32_t arg1) {
  script_125CC(4);
  return;
}

static int32_t script_F460(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  task_114 = task_10C;
  script_125CC(5);
  if (task_694 == 0) {
    int32_t local_F8;
    script_125CC(6);
    script_B027(9291);
    task_688 = SCNF_RequestActionSelectorFromParams_0008(0xFFFF, 37, 34, 0xFFFF, 450, 0xFFFF, ACTION_SELECTOR_MODE_STANDARD);
    local_F8 = task_688;
    if (local_F8 == 37) {
      Script_scenCreateTask(script_E8E4, 0);
      Script_scenCreateTask(script_E2B4, 0);
      script_B16F(30);
      Script_scenCreateTask(script_E8F0, 0);
      Script_scenCreateTask(script_E2B7, 0);
      script_B16F(30);
      Script_scenCreateTask(script_E8FC, 0);
      Script_scenCreateTask(script_E2BA, 0);
      script_B16F(30);
      local_FC = arg2;
    } else {
      script_125CC(8);
      local_FC = arg3;
    }
  } else {
    int32_t local_F8;
    script_125CC(7);
    script_B027(9291);
    task_688 = SCNF_RequestActionSelectorFromParams_0008(0xFFFF, 37, 34, 0xFFFF, 450, 0xFFFF, ACTION_SELECTOR_MODE_STANDARD);
    local_F8 = task_688;
    if (local_F8 == 37) {
      Script_scenCreateTask(script_E8E4, 0);
      Script_scenCreateTask(script_E2B4, 0);
      script_B16F(30);
      Script_scenCreateTask(script_E8F0, 0);
      Script_scenCreateTask(script_E2B7, 0);
      script_B16F(30);
      Script_scenCreateTask(script_E8FC, 0);
      Script_scenCreateTask(script_E2BA, 0);
      script_B16F(30);
      local_FC = arg2;
    } else {
      script_125CC(8);
      local_FC = arg3;
    }
  }
  return local_FC;
}

static int32_t script_F5FC(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  script_B027(9261);
  script_B140();
  Script_scenCreateTask(script_DE96, 0);
  script_B14A();
  local_FC = script_EE34(arg2, arg3);
  return local_FC;
}

static int32_t script_F631(int32_t arg0, int32_t arg1, int32_t arg2) {
  int32_t local_FC;
  if (arg0 == 1) {
    script_B140();
    Script_scenCreateTask(script_B9B4, 4, CHARA_AKIR, CHARA_TAIJ, 15, 10);
    script_B14A();
  }
  local_FC = arg1;
  if (local_FC == 0xBC614E) {
  } else if (local_FC == 20) {
    local_FC = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f);
    if (local_FC == 0) {
      Script_scenCreateTask(script_E4B8, 0);
      Script_scenCreateTask(script_E1A9, 0);
      script_B16F(30);
    } else {
      Script_scenCreateTask(script_E4C4, 0);
      Script_scenCreateTask(script_E1AC, 0);
      script_B16F(30);
    }
    local_FC = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f);
    if (local_FC == 0) {
      Script_scenCreateTask(script_E4D0, 0);
      Script_scenCreateTask(script_E1AF, 0);
      script_B16F(30);
    } else if (local_FC == 1) {
      Script_scenCreateTask(script_E4DC, 0);
      Script_scenCreateTask(script_E1B2, 0);
      script_B16F(30);
    } else {
      Script_scenCreateTask(script_E4E8, 0);
      Script_scenCreateTask(script_E1B5, 0);
      script_B16F(30);
    }
  } else {
    local_FC = *(int8_t *)&task_130[arg2][3];
    if (local_FC == 1) {
      local_FC = *(int8_t *)&task_130[arg2][2];
      if ((local_FC == 2) | (local_FC == 8) | (local_FC == 9) | (local_FC == 11) | (local_FC == 10)) {
        Script_scenCreateTask(script_E590, 0);
        Script_scenCreateTask(script_E1DF, 0);
        script_B16F(30);
      } else {
        local_FC = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 1.0f);
        if (local_FC == 0) {
          Script_scenCreateTask(script_E56C, 0);
          Script_scenCreateTask(script_E1D6, 0);
          script_B16F(30);
        } else if (local_FC == 0) {
          Script_scenCreateTask(script_E578, 0);
          Script_scenCreateTask(script_E1D9, 0);
          script_B16F(30);
        } else {
          Script_scenCreateTask(script_E584, 0);
          Script_scenCreateTask(script_E1DC, 0);
          script_B16F(30);
        }
      }
      goto L00F858;
    }
  }
  L00F858:;
  return task_65C = 0;
}

__attribute__((scn_string_bindings(0, data_1ABDE)))
static int32_t script_F860(int32_t arg0, int32_t arg1, int32_t arg2) {
  int32_t local_FC;
  if (arg0 == 1) {
    script_B140();
    Script_scenCreateTask(script_B9B4, 4, CHARA_AKIR, CHARA_TAIJ, 15, 10);
    script_B14A();
  }
  local_FC = arg1;
  if (local_FC == 0xBC614E) {
  } else if (local_FC == 20) {
    local_FC = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f);
    if (local_FC == 0) {
      Script_scenCreateTask(script_E4B8, 0);
      Script_scenCreateTask(script_E1A9, 0);
      script_B16F(30);
    } else {
      Script_scenCreateTask(script_E4C4, 0);
      Script_scenCreateTask(script_E1AC, 0);
      script_B16F(30);
    }
    local_FC = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f);
    if (local_FC == 0) {
      Script_scenCreateTask(script_E4D0, 0);
      Script_scenCreateTask(script_E1AF, 0);
      script_B16F(30);
    } else if (local_FC == 1) {
      Script_scenCreateTask(script_E4DC, 0);
      Script_scenCreateTask(script_E1B2, 0);
      script_B16F(30);
    } else {
      Script_scenCreateTask(script_E4E8, 0);
      Script_scenCreateTask(script_E1B5, 0);
      script_B16F(30);
    }
  } else if (local_FC == 17) {
    local_FC = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f);
    if (local_FC == 0) {
      Script_scenCreateTask(script_E728, 0);
      Script_scenCreateTask(script_E245, 0);
      script_B16F(30);
    } else {
      Script_scenCreateTask(script_E734, 0);
      Script_scenCreateTask(script_E248, 0);
      script_B16F(30);
    }
    local_FC = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f);
    if (local_FC == 0) {
      Script_scenCreateTask(script_E740, 0);
      Script_scenCreateTask(script_E24B, 0);
      script_B16F(30);
    } else {
      Script_scenCreateTask(script_E74C, 0);
      Script_scenCreateTask(script_E24E, 0);
      script_B16F(30);
    }
  } else {
    local_FC = *(int8_t *)&task_130[arg2][3];
    if (local_FC == 1) {
      local_FC = *(int8_t *)&task_130[arg2][2];
      if (local_FC == 1) {
        local_FC = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f);
        if (local_FC == 0) {
          Script_scenCreateTask(script_E6D4, 0);
          Script_scenCreateTask(script_E230, 0);
          script_B16F(30);
        } else {
          Script_scenCreateTask(script_E6E0, 0);
          Script_scenCreateTask(script_E233, 0);
          script_B16F(30);
        }
        local_FC = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f);
        if (local_FC == 0) {
          Script_scenCreateTask(script_E6EC, 0);
          Script_scenCreateTask(script_E236, 0);
          script_B16F(30);
        } else {
          Script_scenCreateTask(script_E6F8, 0);
          Script_scenCreateTask(script_E239, 0);
          script_B16F(30);
        }
      } else if (local_FC == 3) {
        Script_scenCreateTask(script_E614, 0);
        Script_scenCreateTask(script_E200, 0);
        script_B16F(30);
      } else if (local_FC == 7) {
        local_FC = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 4.0f);
        if (local_FC == 0) {
          Script_scenCreateTask(script_E5C0, 0);
          Script_scenCreateTask(script_E1EB, 0);
          script_B16F(30);
        } else if (local_FC == 1) {
          Script_scenCreateTask(script_E5CC, 0);
          Script_scenCreateTask(script_E1EE, 0);
          script_B16F(30);
        } else if (local_FC == 2) {
          Script_scenCreateTask(script_E6BC, 0);
          Script_scenCreateTask(script_E22A, 0);
          script_B16F(30);
        } else {
          Script_scenCreateTask(script_E6C8, 0);
          Script_scenCreateTask(script_E22D, 0);
          script_B16F(30);
        }
      } else if (local_FC == 9) {
        Script_scenCreateTask(script_E494, 0);
        Script_scenCreateTask(script_E1A0, 0);
        script_B16F(30);
      } else if (local_FC == 11) {
        Script_scenCreateTask(script_E4AC, 0);
        Script_scenCreateTask(script_E1A6, 0);
        script_B16F(30);
      } else if (local_FC == 10) {
        Script_scenCreateTask(script_E4A0, 0);
        Script_scenCreateTask(script_E1A3, 0);
        script_B16F(30);
      } else if (local_FC == 15) {
        if (*(int32_t *)&task_130[arg2][8] & 1) {
          if (*(int8_t *)&task_130[arg2][5] < 4) {
            Script_scenCreateTask(script_E638, 0);
            Script_scenCreateTask(script_E209, 0);
            script_B16F(30);
          } else {
            Script_scenCreateTask(script_E644, 0);
            Script_scenCreateTask(script_E20C, 0);
            script_B16F(30);
          }
        } else {
          local_FC = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f);
          if (local_FC == 0) {
            Script_scenCreateTask(script_E620, 0);
            Script_scenCreateTask(script_E203, 0);
            script_B16F(30);
          } else {
            Script_scenCreateTask(script_E62C, 0);
            Script_scenCreateTask(script_E206, 0);
            script_B16F(30);
          }
        }
      } else {
        Script_sprintf("cause err : %d\012", *(int8_t *)&task_130[arg2][2]);
      }
    } else {
      Script_sprintf("cause err : %d(%d) : 3tumeno dousaha naihazu\012", *(int8_t *)&task_130[arg2][2], *(int8_t *)&task_130[arg2][3]);
    }
  }
  return task_65C = 0;
}

__attribute__((scn_string_bindings(0, data_1AC1C, 1, data_1AC2C)))
static int32_t script_FDBF(int32_t arg0, int32_t arg1, int32_t arg2) {
  int32_t local_FC;
  if (arg0 == 1) {
    script_B140();
    Script_scenCreateTask(script_B9B4, 4, CHARA_AKIR, CHARA_TAIJ, 15, 10);
    script_B14A();
  }
  local_FC = arg1;
  if (local_FC == 0xBC614E) {
  } else if (local_FC == 20) {
    local_FC = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f);
    if (local_FC == 0) {
      Script_scenCreateTask(script_E4B8, 0);
      Script_scenCreateTask(script_E1A9, 0);
      script_B16F(30);
    } else {
      Script_scenCreateTask(script_E4C4, 0);
      Script_scenCreateTask(script_E1AC, 0);
      script_B16F(30);
    }
    local_FC = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f);
    if (local_FC == 0) {
      Script_scenCreateTask(script_E4D0, 0);
      Script_scenCreateTask(script_E1AF, 0);
      script_B16F(30);
    } else if (local_FC == 1) {
      Script_scenCreateTask(script_E4DC, 0);
      Script_scenCreateTask(script_E1B2, 0);
      script_B16F(30);
    } else {
      Script_scenCreateTask(script_E4E8, 0);
      Script_scenCreateTask(script_E1B5, 0);
      script_B16F(30);
    }
  } else if (local_FC == 18) {
    if (*(int8_t *)&task_130[arg2][5] < 6) {
      local_FC = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f);
      if (local_FC == 0) {
        Script_scenCreateTask(script_E998, 0);
        Script_scenCreateTask(script_E2E1, 0);
        script_B16F(30);
      } else if (local_FC == 1) {
        Script_scenCreateTask(script_E9A4, 0);
        Script_scenCreateTask(script_E2E4, 0);
        script_B16F(30);
      } else {
        Script_scenCreateTask(script_E9B0, 0);
        Script_scenCreateTask(script_E2E7, 0);
        script_B16F(30);
      }
    } else {
      local_FC = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f);
      if (local_FC == 0) {
        Script_scenCreateTask(script_E9BC, 0);
        Script_scenCreateTask(script_E2EA, 0);
        script_B16F(30);
      } else if (local_FC == 1) {
        Script_scenCreateTask(script_E9C8, 0);
        Script_scenCreateTask(script_E2ED, 0);
        script_B16F(30);
      } else {
        Script_scenCreateTask(script_E9D4, 0);
        Script_scenCreateTask(script_E2F0, 0);
        script_B16F(30);
      }
    }
  } else if (local_FC == 19) {
    if (*(int8_t *)&task_130[arg2][5] < 6) {
      local_FC = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f);
      if (local_FC == 0) {
        Script_scenCreateTask(script_E9E0, 0);
        Script_scenCreateTask(script_E2F3, 0);
        script_B16F(30);
      } else if (local_FC == 1) {
        Script_scenCreateTask(script_E9EC, 0);
        Script_scenCreateTask(script_E2F6, 0);
        script_B16F(30);
      } else {
        Script_scenCreateTask(script_E9F8, 0);
        Script_scenCreateTask(script_E2F9, 0);
        script_B16F(30);
      }
    } else {
      local_FC = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f);
      if (local_FC == 0) {
        Script_scenCreateTask(script_EA04, 0);
        Script_scenCreateTask(script_E2FC, 0);
        script_B16F(30);
      } else if (local_FC == 1) {
        Script_scenCreateTask(script_EA10, 0);
        Script_scenCreateTask(script_E2FF, 0);
        script_B16F(30);
      } else {
        Script_scenCreateTask(script_EA1C, 0);
        Script_scenCreateTask(script_E302, 0);
        script_B16F(30);
      }
    }
  } else if (local_FC == 16) {
    if (*(int8_t *)&task_130[arg2][5] < 6) {
      Script_scenCreateTask(script_E650, 0);
      Script_scenCreateTask(script_E20F, 0);
      script_B16F(30);
      local_FC = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f);
      if (local_FC == 0) {
        Script_scenCreateTask(script_E65C, 0);
        Script_scenCreateTask(script_E212, 0);
        script_B16F(30);
      } else {
        Script_scenCreateTask(script_E668, 0);
        Script_scenCreateTask(script_E215, 0);
        script_B16F(30);
      }
    } else {
      Script_scenCreateTask(script_E674, 0);
      Script_scenCreateTask(script_E218, 0);
      script_B16F(30);
      local_FC = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f);
      if (local_FC == 0) {
        Script_scenCreateTask(script_E680, 0);
        Script_scenCreateTask(script_E21B, 0);
        script_B16F(30);
      } else {
        Script_scenCreateTask(script_E68C, 0);
        Script_scenCreateTask(script_E21E, 0);
        script_B16F(30);
      }
    }
  } else if (local_FC == 17) {
    local_FC = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f);
    if (local_FC == 0) {
      Script_scenCreateTask(script_E728, 0);
      Script_scenCreateTask(script_E245, 0);
      script_B16F(30);
    } else {
      Script_scenCreateTask(script_E734, 0);
      Script_scenCreateTask(script_E248, 0);
      script_B16F(30);
    }
    local_FC = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f);
    if (local_FC == 0) {
      Script_scenCreateTask(script_E740, 0);
      Script_scenCreateTask(script_E24B, 0);
      script_B16F(30);
    } else {
      Script_scenCreateTask(script_E74C, 0);
      Script_scenCreateTask(script_E24E, 0);
      script_B16F(30);
    }
  } else {
    local_FC = *(int8_t *)&task_130[arg2][3];
    if (local_FC == 1) {
      local_FC = *(int8_t *)&task_130[arg2][2];
      if (local_FC == 14) {
        local_FC = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f);
        if (local_FC == 0) {
          Script_scenCreateTask(script_E698, 0);
          Script_scenCreateTask(script_E221, 0);
          script_B16F(30);
        } else if (local_FC == 1) {
          Script_scenCreateTask(script_E6A4, 0);
          Script_scenCreateTask(script_E224, 0);
          script_B16F(30);
        } else {
          Script_scenCreateTask(script_E6B0, 0);
          Script_scenCreateTask(script_E227, 0);
          script_B16F(30);
        }
      } else if (local_FC == 1) {
        local_FC = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f);
        if (local_FC == 0) {
          Script_scenCreateTask(script_E6D4, 0);
          Script_scenCreateTask(script_E230, 0);
          script_B16F(30);
        } else {
          Script_scenCreateTask(script_E6E0, 0);
          Script_scenCreateTask(script_E233, 0);
          script_B16F(30);
        }
        local_FC = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f);
        if (local_FC == 0) {
          Script_scenCreateTask(script_E6EC, 0);
          Script_scenCreateTask(script_E236, 0);
          script_B16F(30);
        } else {
          Script_scenCreateTask(script_E6F8, 0);
          Script_scenCreateTask(script_E239, 0);
          script_B16F(30);
        }
      } else if (local_FC == 3) {
        Script_scenCreateTask(script_E614, 0);
        Script_scenCreateTask(script_E200, 0);
        script_B16F(30);
      } else if (local_FC == 7) {
        local_FC = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 4.0f);
        if (local_FC == 0) {
          Script_scenCreateTask(script_E5C0, 0);
          Script_scenCreateTask(script_E1EB, 0);
          script_B16F(30);
        } else if (local_FC == 1) {
          Script_scenCreateTask(script_E5CC, 0);
          Script_scenCreateTask(script_E1EE, 0);
          script_B16F(30);
        } else if (local_FC == 2) {
          Script_scenCreateTask(script_E6BC, 0);
          Script_scenCreateTask(script_E22A, 0);
          script_B16F(30);
        } else {
          Script_scenCreateTask(script_E6C8, 0);
          Script_scenCreateTask(script_E22D, 0);
          script_B16F(30);
        }
      } else if (local_FC == 9) {
        Script_scenCreateTask(script_E494, 0);
        Script_scenCreateTask(script_E1A0, 0);
        script_B16F(30);
      } else if (local_FC == 11) {
        Script_scenCreateTask(script_E4AC, 0);
        Script_scenCreateTask(script_E1A6, 0);
        script_B16F(30);
      } else if (local_FC == 10) {
        Script_scenCreateTask(script_E4A0, 0);
        Script_scenCreateTask(script_E1A3, 0);
        script_B16F(30);
      } else if (local_FC == 15) {
        if (*(int32_t *)&task_130[arg2][8] & 1) {
          if (*(int8_t *)&task_130[arg2][5] < 4) {
            Script_scenCreateTask(script_E638, 0);
            Script_scenCreateTask(script_E209, 0);
            script_B16F(30);
          } else {
            Script_scenCreateTask(script_E644, 0);
            Script_scenCreateTask(script_E20C, 0);
            script_B16F(30);
          }
        } else {
          local_FC = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f);
          if (local_FC == 0) {
            Script_scenCreateTask(script_E620, 0);
            Script_scenCreateTask(script_E203, 0);
            script_B16F(30);
          } else {
            Script_scenCreateTask(script_E62C, 0);
            Script_scenCreateTask(script_E206, 0);
            script_B16F(30);
          }
        }
      } else {
        Script_sprintf("cause err : %d\012", *(int8_t *)&task_130[arg2][2]);
      }
    } else if (local_FC == 3) {
      local_FC = *(int8_t *)&task_130[arg2][2];
      if (local_FC == 4) {
        Script_scenCreateTask(script_E5D8, 0);
        Script_scenCreateTask(script_E1F1, 0);
        script_B16F(30);
      } else if (local_FC == 8) {
        Script_scenCreateTask(script_E488, 0);
        Script_scenCreateTask(script_E19D, 0);
        script_B16F(30);
      } else if (local_FC == 11) {
        Script_scenCreateTask(script_E4AC, 0);
        Script_scenCreateTask(script_E1A6, 0);
        script_B16F(30);
      } else if (local_FC == 10) {
        Script_scenCreateTask(script_E4A0, 0);
        Script_scenCreateTask(script_E1A3, 0);
        script_B16F(30);
      } else if (local_FC == 12) {
        local_FC = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f);
        if (local_FC == 0) {
          Script_scenCreateTask(script_E704, 0);
          Script_scenCreateTask(script_E23C, 0);
          script_B16F(30);
        } else if (local_FC == 1) {
          Script_scenCreateTask(script_E710, 0);
          Script_scenCreateTask(script_E23F, 0);
          script_B16F(30);
        } else {
          Script_scenCreateTask(script_E71C, 0);
          Script_scenCreateTask(script_E242, 0);
          script_B16F(30);
        }
      } else {
        Script_sprintf("cause err : %d\012", *(int8_t *)&task_130[arg2][2]);
      }
    } else {
      Script_sprintf("cause err : %d(%d) : ３つめの動作はないはず\012", *(int8_t *)&task_130[arg2][2], *(int8_t *)&task_130[arg2][3]);
      Script_sprintf("WorstMiss %d w_idx %d\012", arg1, arg2);
    }
  }
  return task_65C = 0;
}

static int32_t script_108A3(int32_t arg0, int32_t arg1, int32_t arg2) {
  script_FDBF(arg0, arg1, arg2);
  end:;
}

static int32_t script_108B3(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  int32_t local_FC;
  script_B140();
  Script_scenCreateTask(script_BA1A, 4, CHARA_KISY, CHARA_AKIR, 15, 30);
  script_B14A();
  script_108A3(arg0, arg1, arg2);
  script_BB8D((int32_t)&data_1AB1C);
  local_FC = script_EF18(arg3, arg4);
  return local_FC;
}

static void script_10905(void) {
  int16_t local_FE;
  uint8_t unused_EC[18];
  int32_t local_E8;
  int32_t local_E4;
  int32_t local_E0;
  local_FE = 0;
  task_658 = 0;
  task_638 = -1;
  for (;;) {
    local_E0 = local_FE;
    switch (local_E0) {
      case -1:
        SCNF_WaitFrames_0000(2);
        break;
      case 0:
        task_68C = 0;
        task_674 = 0;
        task_10C = 0;
        local_E4 = 0;
        task_694 = 0;
        script_C30A(0);
        Script_EVT_SetPosition(CHARA_AKIR, 0x78000000, &data_19ED8);
        Script_EVT_SetPosition(CHARA_KISY, 0x78000000, &data_19EE4);
        Script_EVT_SetPosition(CHARA_TAIJ, 0x78000000, &data_19EF0);
        {
          Vector3i local_D4;
          (&local_D4)->y = 6371;
          Script_EVT_SetRotation(CHARA_AKIR, MATRIX_XFORM_Y, &local_D4, 0);
        }
        {
          Vector3i local_D4;
          (&local_D4)->y = 38229;
          Script_EVT_SetRotation(CHARA_KISY, MATRIX_XFORM_Y, &local_D4, 0);
        }
        {
          Vector3i local_D4;
          (&local_D4)->y = 36408;
          Script_EVT_SetRotation(CHARA_TAIJ, MATRIX_XFORM_Y, &local_D4, 0);
        }
        if (task_658 == 0) {
          task_658 = Script_scenCreateTask(script_BB38, 0);
        }
        {
          Vector3 local_D4;
          Script_ReadMOTNPositionAtFrame(CHARA_AKIR, MN_YKI_AKI_NGR_INASI, 0.0f, &local_D4);
          Script_sprintf("POS 0.0 = %f, %f, %f.\012", (&local_D4)->x, (&local_D4)->y, (&local_D4)->z);
        }
        {
          Vector3 local_D4;
          Script_ReadMOTNPositionAtFrame(CHARA_AKIR, MN_YKI_AKI_NGR_INASI, 9.99999997e-07f, &local_D4);
          Script_sprintf("POS 0.000001 = %f, %f, %f.\012", (&local_D4)->x, (&local_D4)->y, (&local_D4)->z);
        }
        {
          Vector3 local_D4;
          Script_ReadMOTNPositionAtFrame(CHARA_AKIR, MN_YKI_AKI_NGR_INASI, 1.00000001e-07f, &local_D4);
          Script_sprintf("POS 0.0000001 = %f, %f, %f.\012", (&local_D4)->x, (&local_D4)->y, (&local_D4)->z);
        }
        {
          Vector3 local_D4;
          Script_ReadMOTNPositionAtFrame(CHARA_AKIR, MN_YKI_AKI_NGR_INASI, 9.99999994e-09f, &local_D4);
          Script_sprintf("POS 0.00000001 = %f, %f, %f.\012", (&local_D4)->x, (&local_D4)->y, (&local_D4)->z);
        }
        script_B87B();
        script_B140();
        Script_scenCreateTask(script_C558, 0);
        script_B14A();
        local_FE = 10;
        break;
      case 10:
        Script_filmCheckOrSetTransitionUtil(60, 255, 0, 0, 0, 0, 0, 0, 0);
        script_125CC(0);
        local_FE = 30;
        break;
      case 20:
        script_B027(9221);
        script_B140();
        Script_scenCreateTask(script_CE74, 0);
        script_B14A();
        Script_scenCreateTask(script_E3A4, 0);
        Script_scenCreateTask(script_E164, 0);
        script_B16F(30);
        Script_scenCreateTask(script_E3B0, 0);
        Script_scenCreateTask(script_E167, 0);
        script_B16F(30);
        Script_scenCreateTask(script_E3BC, 0);
        Script_scenCreateTask(script_E16A, 0);
        script_B16F(30);
        Script_scenCreateTask(script_E3C8, 0);
        Script_scenCreateTask(script_E16D, 0);
        script_B16F(30);
        task_674 = -1;
        local_FE = 100;
        break;
      case 30:
        local_FE = 40;
        break;
      case 40:
        if (local_E4 == 0) {
          script_125CC(1);
          local_E4 = 1;
        } else {
          script_125CC(10);
        }
        if (task_670 == 3) {
          local_FE = 80;
        } else {
          local_FE = 50;
        }
        break;
      case 50:
        local_FE = script_113DB(0, 62, 64, 60, 80, 50, 40, 100);
        break;
      case 60:
        local_FE = script_113DB(1, 63, 64, 70, 80, 60, 40, 100);
        break;
      case 70:
        local_FE = script_113DB(2, 64, -1, 80, 80, 70, 40, 100);
        break;
      case 80:
        local_FE = script_113DB(3, 67, -1, 90, 90, 80, 40, 100);
        break;
      case 90:
        local_FE = script_113DB(9, 67, -1, 90, 90, 90, 40, 100);
        break;
      case 100:
        local_FE = 200;
        break;
      case 200:
        while (Script_filmCheckOrSetTransitionUtil(-1)) {
          SCNF_WaitFrames_0000(1);
        }
        task_5F8 = 0;
        task_5F7 = 0;
        Script_SetCharaLookAtPosition(CHARA_AKIR, -20, 0, 0);
        Script_SetCharaLookAtPosition(CHARA_KISY, -20, 0, 0);
        Script_SetCharaLookAtPosition(CHARA_TAIJ, -20, 0, 0);
        if (task_674 != -1) {
          switch (task_674) {
            case 0x40400000:
              Script_wazaUtils(MOVE_UI_CMD_SET_MOVE_SKILL_VALUE, 1, 15, 12);
              Script_wazaUtils(MOVE_UI_CMD_SHOW_PENDING_MASTERED_POPUP, 0, 0, 0);
              break;
            case 0:
              Script_wazaUtils(MOVE_UI_CMD_SET_MOVE_SKILL_VALUE, 1, 15, 1);
              Script_wazaUtils(MOVE_UI_CMD_SHOW_PENDING_LEARNED_POPUP, 0, 0, 0);
              break;
          }
          local_E8 = 150;
          for (;;) {
            local_E8 = local_E8 - 1;
            if ((Script_GetControllerInput(1) == 0x200) | (local_E8 == 0)) {
              Script_wazaUtils(MOVE_UI_CMD_CLOSE_MOVE_POPUP, 0, 0, 0);
              break;
            }
            SCNF_WaitFrames_0000(1);
          }
        }
        task_63C = task_63C + 1;
        if (task_63C > 2) {
          task_63C = 0;
        }
        script_BFE2();
        SCNF_WaitFrames_0000(1);
        Script_PreserveAccumulator_0182(0, 0, 0);
        goto L010E45;
        break;
    }
    Script_sprintf("seq : %d\012", local_FE);
    SCNF_WaitFrames_0000(1);
  }
  L010E45:;
  return;
}

static int32_t script_10E46(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  if (task_69C == 0) {
    (void)task_698;
  } else {
    local_F8 = arg0;
    if ((local_F8 == 0) | (local_F8 == 1) | (local_F8 == 2)) {
      if (task_698 == 9280) {
        local_FC = 9281;
      } else {
        local_FC = 9280;
      }
    } else {
      if ((local_F8 == 3) | (local_F8 == 9)) {
        if (task_698 == 9282) {
          local_FC = 9283;
        } else {
          local_FC = 9282;
        }
        goto L010EC1;
      }
    }
    L010EC1:;
    task_69C = 0;
    Script_sprintf("chgcam : %d\012", local_FC);
    (void)local_FC;
  }
  end:;
}

static void script_10ED7(int32_t arg0) {
  int32_t local_FC;
  local_FC = arg0;
  if ((local_FC == 0) | (local_FC == 1) | (local_FC == 2)) {
    Script_EVT_SetPosition(CHARA_AKIR, 0x78000000, &data_19F38);
    Script_EVT_SetPosition(CHARA_KISY, 0x78000000, &data_19F44);
    Script_EVT_SetPosition(CHARA_TAIJ, 0x78000000, &data_19F50);
  } else {
    switch (local_FC) {
      case 3:
        Script_EVT_SetPosition(CHARA_AKIR, 0x78000000, &data_19F5C);
        Script_EVT_SetPosition(CHARA_KISY, 0x78000000, &data_19F68);
        Script_EVT_SetPosition(CHARA_TAIJ, 0x78000000, &data_19F80);
        break;
      case 9:
        Script_EVT_SetPosition(CHARA_AKIR, 0x78000000, &data_19F5C);
        Script_EVT_SetPosition(CHARA_KISY, 0x78000000, &data_19F68);
        Script_EVT_SetPosition(CHARA_TAIJ, 0x78000000, &data_19F80);
        break;
    }
  }
  Script_InitializeOsag(CHARA_AKIR);
  Script_InitializeOsag(CHARA_KISY);
  Script_InitializeOsag(CHARA_TAIJ);
  return;
}

static void script_10FD9(int32_t arg0) {
  int32_t local_FC;
  local_FC = arg0;
  if ((local_FC == 0) | (local_FC == 1) | (local_FC == 2)) {
    {
      Vector3i local_F0;
      (&local_F0)->y = 27488;
      Script_EVT_SetRotation(CHARA_AKIR, MATRIX_XFORM_Y, &local_F0, 0);
    }
    {
      Vector3i local_F0;
      (&local_F0)->y = 0xA000;
      Script_EVT_SetRotation(CHARA_KISY, MATRIX_XFORM_Y, &local_F0, 0);
    }
    {
      Vector3i local_F0;
      (&local_F0)->y = 10194;
      Script_EVT_SetRotation(CHARA_TAIJ, MATRIX_XFORM_Y, &local_F0, 0);
    }
  } else {
    switch (local_FC) {
      case 3:
        {
          {
            Vector3i local_F0;
            (&local_F0)->y = 23301;
            Script_EVT_SetRotation(CHARA_AKIR, MATRIX_XFORM_Y, &local_F0, 0);
          }
          {
            Vector3i local_F0;
            (&local_F0)->y = 56069;
            Script_EVT_SetRotation(CHARA_KISY, MATRIX_XFORM_Y, &local_F0, 0);
          }
          {
            Vector3i local_F0;
            (&local_F0)->y = 33860;
            Script_EVT_SetRotation(CHARA_TAIJ, MATRIX_XFORM_Y, &local_F0, 0);
          }
          break;
        }
      case 9:
        {
          {
            Vector3i local_F0;
            (&local_F0)->y = 23301;
            Script_EVT_SetRotation(CHARA_AKIR, MATRIX_XFORM_Y, &local_F0, 0);
          }
          {
            Vector3i local_F0;
            (&local_F0)->y = 56069;
            Script_EVT_SetRotation(CHARA_KISY, MATRIX_XFORM_Y, &local_F0, 0);
          }
          {
            Vector3i local_F0;
            (&local_F0)->y = 33860;
            Script_EVT_SetRotation(CHARA_TAIJ, MATRIX_XFORM_Y, &local_F0, 0);
          }
          break;
        }
    }
  }
  return;
}

static void script_11118(int32_t arg0) {
  int32_t local_FC;
  if (Script_scenCharaGetMotAnimId(CHARA_AKIR) != 850) {
    Script_motmToggleFlags(CHARA_AKIR, 2, 0x40);
    Script_SetCharaLookAtPosition(CHARA_AKIR, -20, 0, 0);
    Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_SEKKIN_KAMAE_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_motmToggleFlags(CHARA_AKIR, 2, 0x40);
  }
  local_FC = arg0;
  if ((local_FC == 3) | (local_FC == 9)) {
    Script_motmToggleFlags(CHARA_KISY, 2, 0x40);
    Script_SetCharaLookAtPosition(CHARA_KISY, -20, 0, 0);
    Script_PlayCharaMotion(CHARA_KISY, MN_AKI_AKI_SEKKIN_KAMAE_LP, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_motmToggleFlags(CHARA_KISY, 2, 0x40);
    Script_motmToggleFlags(CHARA_TAIJ, 2, 0x40);
    Script_PlayCharaMotion(CHARA_TAIJ, 12300, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_motmToggleFlags(CHARA_TAIJ, 2, 0x40);
    Script_SetCharaLookAtPosition(CHARA_TAIJ, -20, 0, 0);
    goto L011226;
  }
  L011226:;
  Script_InitializeOsag(CHARA_AKIR);
  Script_InitializeOsag(CHARA_KISY);
  Script_InitializeOsag(CHARA_TAIJ);
  return;
}

static int32_t script_11245(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  Vector3i local_EC;
  int32_t local_E8;
  int32_t local_E4;
  local_E4 = arg0;
  if ((local_E4 == 0) | (local_E4 == 1) | (local_E4 == 2)) {
    local_FC = 27488;
    local_F8 = 0xA000;
  } else {
    if ((local_E4 == 3) | (local_E4 == 9)) {
      local_FC = 23301;
      local_F8 = 56069;
      goto L011297;
    }
  }
  L011297:;
  Script_mtwkGetSetRotation(CHARA_AKIR, &local_EC, 0x40000000);
  local_E8 = script_B1FA(local_FC, (&local_EC)->y);
  if (script_ACC8(local_E8) > 0x2000) {
    Script_sprintf("DIFF ANG OVER 0: %d %d %d\012", script_ACC8(local_E8) * 360 / 0x10000, local_FC * 360 / 0x10000, (&local_EC)->y * 360 / 0x10000);
    (void)1;
  } else {
    Script_mtwkGetSetRotation(CHARA_KISY, &local_EC, 0x40000000);
    local_E8 = script_B1FA(local_F8, (&local_EC)->y);
    if (script_ACC8(local_E8) > 0x2000) {
      Script_sprintf("DIFF ANG OVER 1: %d %d %d\012", script_ACC8(local_E8) * 360 / 0x10000, local_F8 * 360 / 0x10000, (&local_EC)->y * 360 / 0x10000);
      (void)1;
    } else {
      (void)0;
    }
  }
  end:;
}

static void script_11385(int32_t arg0) {
  if (task_FC != arg0) {
    script_B027(arg0);
  }
  if (task_664) {
    Script_srchControl((void *)task_664);
    task_664 = 0;
  }
  task_664 = Script_scenCreateTask(script_EB24, 1, arg0);
  return;
}

static void script_113C3(void) {
  if (task_664) {
    Script_srchControl((void *)task_664);
    task_664 = 0;
  }
  return;
}

static int32_t script_113DB(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5, int32_t arg6, int32_t arg7) {
  int32_t unused_FC;
  int32_t local_F8;
  int32_t local_F4;
  int32_t local_F0;
  int32_t unused_EC;
  int8_t local_EB;
  int8_t local_EA;
  uint8_t unused_E4[6];
  int8_t local_E3;
  Script_sprintf("1:\012");
  script_B8B7();
  task_120 = 0;
  task_121 = 0;
  task_122 = 0;
  task_123 = 0;
  task_100 = 0;
  task_104 = 0;
  task_108 = 0;
  task_664 = 0;
  task_668 = 0;
  task_66C = 0;
  task_65C = 0;
  task_69C = 1;
  task_670 = arg0;
  task_690 = 0;
  script_10ED7(arg0);
  script_10FD9(arg0);
  script_11118(arg0);
  Script_sprintf("2:\012");
  script_BB8D(0);
  Script_sprintf("3:\012");
  local_E3 = 1;
  for (;;) {
    task_698 = script_10E46(arg0);
    script_11385(task_698);
    task_110 = 0;
    script_AD05(task_668);
    task_668 = Script_scenCreateTask(script_B5E1, 0);
    script_AD05(task_66C);
    task_66C = Script_scenCreateTask(script_B6D6, 1, arg0);
    task_118 = 0;
    local_EB = 0;
    Script_sprintf("4:\012");
    for (;;) {
      local_EA = 0;
      if (local_E3) {
        task_698 = script_10E46(arg0);
        script_11385(task_698);
        script_10ED7(arg0);
        script_11118(arg0);
      }
      script_10FD9(arg0);
      script_C375(arg1, arg2);
      script_B605(task_118);
      task_69C = 1;
      if (*((int8_t *)&task_124 + 2) != 20) {
        task_690 = task_690 + 1;
      }
      if (script_11245(arg0)) {
        local_E3 = 1;
      } else {
        local_E3 = 0;
      }
      if (*((int8_t *)&task_124 + 1) == 2) {
        if (local_E3 == 0) {
          script_B31E(CHARA_AKIR, 0, (int32_t)&data_19F5C);
          if ((arg0 == 3) | (arg0 == 9)) {
            script_B31E(CHARA_KISY, 1, (int32_t)&data_19F68);
          }
        }
        task_100 = task_100 + 1;
        task_104 = task_104 + 1;
        SCNF_WaitFrames_0000(1);
        if (task_120) {
          Script_StartOrFreeSEQ(0);
          if (task_65C) {
            if (task_65C) {
              Script_srchControl((void *)task_65C);
              task_65C = 0;
            }
          }
          script_113C3();
          if (task_668) {
            Script_srchControl((void *)task_668);
            task_668 = 0;
          }
          if (task_66C) {
            Script_srchControl((void *)task_66C);
            task_66C = 0;
          }
          local_F4 = script_B6CB(task_118);
          local_F0 = script_B6C5(task_118, local_F4);
          Script_sprintf("Advice Start %d : Miss %d : Idx %d\012", task_110, local_F4, local_F0);
          if (task_122 | task_123 | task_121) {
            Script_SetMotnFlag(CHARA_AKIR, 0);
            if (local_E3 == 0) {
              script_B4E4(0);
              if ((arg0 == 3) | (arg0 == 9)) {
                script_B4E4(1);
              }
            }
            if (task_123 & -(task_690 >= 9)) {
              uint8_t unused_E0[3];
              int32_t local_DC;
              local_DC = arg0;
              if (local_DC == 0) {
                script_BB8D((int32_t)&data_1A994);
              } else if (local_DC == 1) {
                script_BB8D((int32_t)&data_1AA58);
              } else {
                if ((local_DC == 2) | (local_DC == 3)) {
                  script_BB8D((int32_t)&data_1AB1C);
                  goto L011721;
                }
              }
            }
            L011721:;
            {
              uint8_t unused_E0[3];
              int32_t local_DC;
              local_DC = arg0;
              switch (local_DC) {
                case 0:
                  script_F631(0, local_F4, local_F0);
                  local_F8 = script_F025(1, arg5, arg6, arg7);
                  break;
                case 1:
                  script_F860(0, local_F4, local_F0);
                  local_F8 = script_F025(1, arg5, arg6, arg7);
                  break;
                case 2:
                  script_FDBF(0, local_F4, local_F0);
                  local_F8 = script_F025(1, arg5, arg6, arg7);
                  break;
                case 3:
                  script_108A3(0, local_F4, local_F0);
                  local_F8 = script_F025(1, arg5, arg6, arg7);
                  break;
                case 9:
                  if (local_E3 == 0) {
                    script_B4E4(0);
                    script_B4E4(1);
                  }
                  local_F8 = script_108B3(1, local_F4, local_F0, arg3, arg7);
                  break;
              }
            }
          } else {
            uint8_t unused_E0[3];
            int32_t local_DC;
            local_DC = arg0;
            switch (local_DC) {
              case 0:
                task_65C = Script_scenCreateTask(script_F631, 3, 0, local_F4, local_F0);
                local_F8 = arg5;
                break;
              case 1:
                task_65C = Script_scenCreateTask(script_F860, 3, 0, local_F4, local_F0);
                local_F8 = arg5;
                break;
              case 2:
                task_65C = Script_scenCreateTask(script_FDBF, 3, 0, local_F4, local_F0);
                local_F8 = arg5;
                break;
              case 3:
                task_65C = Script_scenCreateTask(script_108A3, 3, 0, local_F4, local_F0);
                local_F8 = arg5;
                break;
              case 9:
                task_65C = Script_scenCreateTask(script_108A3, 3, 0, local_F4, local_F0);
                local_F8 = arg5;
                break;
            }
          }
          task_118 = 0;
          task_110 = 0;
          task_120 = 0;
          task_121 = 0;
          task_122 = 0;
          task_123 = 0;
          task_104 = 0;
          task_108 = 0;
          if (local_F8 != arg5) {
            (void)local_F8;
            goto L011C29;
          }
          if (local_E3 == 0) {
            script_B4E4(0);
            if ((arg0 == 3) | (arg0 == 9)) {
              script_B4E4(1);
            }
          }
          break;
        }
        if (local_E3 == 0) {
          script_B4E4(0);
          if ((arg0 == 3) | (arg0 == 9)) {
            script_B4E4(1);
          }
        }
      } else {
        uint8_t unused_E0[3];
        int32_t local_DC;
        Script_StartOrFreeSEQ(0);
        if (task_65C) {
          Script_srchControl((void *)task_65C);
          task_65C = 0;
        }
        script_113C3();
        if (task_668) {
          Script_srchControl((void *)task_668);
          task_668 = 0;
        }
        if (task_66C) {
          Script_srchControl((void *)task_66C);
          task_66C = 0;
        }
        Script_SetMotnFlag(CHARA_AKIR, 0);
        SCNF_WaitFrames_0000(1);
        if (*((int8_t *)task_5E0 + arg0) == 0) {
          *((int8_t *)task_5EC + arg0) = *((int8_t *)task_5E0 + arg0);
          *((int8_t *)task_5E0 + arg0) = 1;
        }
        local_DC = *((int8_t *)&task_124 + 2);
        if (local_DC == 22) {
          script_BB8D((int32_t)&data_1AB1C);
          local_F8 = script_F008(arg4, arg7);
        } else {
          local_F4 = script_B6CB(task_118 + 1);
          local_F0 = script_B6C5(task_118 + 1, local_F4);
          local_DC = arg0;
          switch (local_DC) {
            case 0:
              if (*((int8_t *)&task_124 + 2) == 23) {
                script_BB8D((int32_t)&data_1AA58);
                script_F2F2(local_F4, local_F0);
                local_F8 = 70;
              } else {
                script_BB8D((int32_t)&data_1A994);
                script_F260(local_F4, local_F0);
                local_F8 = arg3;
              }
              break;
            case 1:
              script_BB8D((int32_t)&data_1AA58);
              script_F3A3(local_F4, local_F0);
              local_F8 = arg3;
              break;
            case 2:
              script_BB8D((int32_t)&data_1AB1C);
              script_F454(local_F4, local_F0);
              local_F8 = arg3;
              break;
            case 3:
              script_BB8D((int32_t)&data_1AB1C);
              Script_PreserveAccumulator_00CD(0, 0x40400000);
              task_674 = 0x40400000;
              task_68C = 0;
              local_F8 = script_F460(local_F4, local_F0, arg3, arg7);
              break;
            case 9:
              script_BB8D((int32_t)&data_1AB1C);
              local_EB = local_EB + 1;
              if ((local_EB == 5) | task_121) {
                local_F8 = script_F5FC(local_F4, local_F0, arg3, arg7);
              } else {
                local_F8 = arg5;
                local_EA = 1;
              }
              break;
          }
        }
        if (local_EA == 0) {
          task_10C = 0;
          (void)local_F8;
          goto L011C29;
        } else {
          local_E3 = 1;
          task_668 = Script_scenCreateTask(script_B5E1, 0);
          task_66C = Script_scenCreateTask(script_B6D6, 1, arg0);
          SCNF_WaitFrames_0000(2);
        }
      }
      if (task_118 < 100) {
        task_118 = task_118 + 1;
      } else {
        Script_sprintf(data_1AD6F);
      }
    }
  }
  L011C29:;
}

static void script_11C2A(int32_t arg0) {
  script_11EAF();
  script_11C5C(arg0);
  Script_shenSeqUtils(arg0);
  while (Script_shenSeqUtils(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_srchControl((void *)task_6A4);
  return;
}

static void script_11C5C(int32_t arg0) {
  int32_t local_FC;
  local_FC = arg0;
  switch (local_FC) {
    case 0:
      task_6A4 = Script_scenCreateTask(script_11EFF, 0);
      break;
    case 1:
      task_6A4 = Script_scenCreateTask(script_120ED, 0);
      break;
    case 2:
      task_6A4 = Script_scenCreateTask(script_1220A, 0);
      break;
    case 3:
      task_6A4 = Script_scenCreateTask(script_122B3, 0);
      break;
    case 4:
      task_6A4 = Script_scenCreateTask(script_12329, 0);
      break;
    case 5:
      task_6A4 = Script_scenCreateTask(script_123B4, 0);
      break;
    case 6:
      task_6A4 = Script_scenCreateTask(script_12456, 0);
      break;
    case 7:
      task_6A4 = Script_scenCreateTask(script_12493, 0);
      break;
    case 8:
      task_6A4 = Script_scenCreateTask(script_124D9, 0);
      break;
    case 9:
      task_6A4 = Script_scenCreateTask(script_12516, 0);
      break;
    case 10:
      task_6A4 = Script_scenCreateTask(script_120ED, 0);
      break;
  }
  return;
}

static void script_11DA4(int32_t arg0) {
  Script_ccowUtils(arg0, 2);
  Script_ccowSetFlag80(arg0, 1);
  Script_motmToggleFlags(arg0, 2, 0x40);
  Script_SetMotnFlag(arg0, -1);
  Script_SetCPAMFlags(arg0, 0);
  Script_SetCharaFlags(arg0, 4);
  Script_CharHandlers(arg0, 2, 8, 16);
  return;
}

static void script_11DE9(int32_t arg0) {
  Script_ccowUtils(arg0, 2);
  Script_ccowSetFlag80(arg0, 1);
  Script_motmToggleFlags(arg0, 2, 0x40);
  Script_SetMotnFlag(arg0, -1);
  Script_SetCPAMFlags(arg0, 0);
  Script_SetCharaFlags(arg0, 4);
  Script_motmGetCharaFlags(arg0, 0, 0);
  Script_SetShadowFlags(arg0, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_SetShadowFlags(arg0, SCRIPT_SHADOW_UPDATE_LIGHT_MASK, 1);
  Script_facemotUtils(arg0, 0, 0);
  Script_facemotUtils(arg0, 1, 0);
  Script_FaceEx(arg0, 1, 0, 1);
  Script_ResetHandMotion(arg0, 1, 1, 0);
  Script_ResetHandMotion(arg0, 1, 0, 0);
  Script_handSetFloatsFor(arg0, 1, data_16B14, 1);
  Script_handSetFloatsFor(arg0, 0, data_16B14, 1);
  Script_FACE_LCG(arg0, 0, 6, 0);
  Script_FaceLookAtTargetUtils(arg0, 0);
  return;
}

static void script_11EAF(void) {
  script_11DE9(CHARA_AKIR);
  script_11DE9(CHARA_KISY);
  script_11DE9(CHARA_TAIJ);
  Script_InitializeOsag(CHARA_AKIR);
  Script_InitializeOsag(CHARA_KISY);
  Script_InitializeOsag(CHARA_TAIJ);
  Script_ccowSetFlag80(SHEN_NAME('TETU'), 1);
  return;
}

static int32_t script_11EFF(void) {
  int32_t unused_E8[6];
  int32_t local_E4;
  task_6A0 = 0;
  Script_executeAicaCommand(18344, 0, 0);
  Script_executeAicaCommand(1184, 2, 103);
  for (;;) {
    local_E4 = task_6A0;
    switch (local_E4) {
      case 0:
        Script_SetHandRootRotation(CHARA_TAIJ, 1, 0x15, &data_1B040);
        Script_charaDisplay(SHEN_NAME('BAG0'), CDM_Show);
        Script_charaDisplay(SHEN_NAME('BAG_'), CDM_Hide);
        Script_clmdSetFlags(CHARA_KISY, 2, 1);
        Script_clmdSetFlags(CHARA_TAIJ, 2, 1);
        Script_SetHandRootRotation(CHARA_AKIR, 1, 0x15, &data_1AF50);
        Script_handSetFloatsFor(CHARA_AKIR, 1, data_1AE6C, 0);
        break;
      case 695:
        Script_clmdSetFlags(CHARA_KISY, 2, 0);
        Script_clmdSetFlags(CHARA_TAIJ, 2, 0);
        Script_SetHandRootRotation(CHARA_AKIR, 1, 0x15, &data_1B04C);
        Script_handSetFloatsFor(CHARA_AKIR, 1, data_16B14, 0);
        break;
      case 696:
        Script_charaDisplay(SHEN_NAME('BAG0'), CDM_Hide);
        Script_charaDisplay(SHEN_NAME('BAG_'), CDM_Show);
        break;
      case 797:
        Script_ResetHandMotion(CHARA_TAIJ, 0, 1, 0);
        Script_ResetHandMotion(CHARA_TAIJ, 0, 0, 0);
        break;
      case 2756:
        Script_ResetHandMotion(CHARA_TAIJ, 1, 1, 0);
        Script_ResetHandMotion(CHARA_TAIJ, 1, 0, 0);
        Script_handSetFloatsFor(CHARA_TAIJ, 1, data_16B14, 0);
        Script_handSetFloatsFor(CHARA_TAIJ, 0, data_16B14, 0);
        break;
      case 2799:
        Script_handSetFloatsFor(CHARA_KISY, 1, data_16BF8, 0);
        Script_handSetFloatsFor(CHARA_KISY, 0, data_16CDC, 0);
        break;
      case 2870:
        Script_handSetFloatsFor(CHARA_KISY, 1, data_16DC0, 0);
        break;
    }
    task_6A0 = task_6A0 + 1;
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_120ED(void) {
  int32_t local_FC;
  task_6A0 = 0;
  for (;;) {
    local_FC = task_6A0;
    switch (local_FC) {
      case 0:
        Script_SetHandRootRotation(CHARA_TAIJ, 1, 0x15, &data_1B058);
        Script_handSetFloatsFor(CHARA_KISY, 1, data_16DC0, 0);
        Script_handSetFloatsFor(CHARA_KISY, 0, data_16CDC, 0);
        Script_handSetFloatsFor(CHARA_TAIJ, 1, data_16DC0, 0);
        Script_handSetFloatsFor(CHARA_TAIJ, 0, data_16CDC, 0);
        break;
      case 250:
        Script_SetHandRootRotation(CHARA_TAIJ, 1, 0x15, &data_1B064);
        break;
      case 337:
        Script_handSetFloatsFor(CHARA_KISY, 1, data_16B14, 0);
        Script_handSetFloatsFor(CHARA_KISY, 0, data_16B14, 0);
        break;
      case 537:
        Script_handSetFloatsFor(CHARA_TAIJ, 1, data_1AF5C, 15);
        break;
      case 681:
        Script_handSetFloatsFor(CHARA_TAIJ, 1, data_16DC0, 0);
        break;
    }
    task_6A0 = task_6A0 + 1;
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_1220A(void) {
  int32_t local_FC;
  task_6A0 = 0;
  for (;;) {
    local_FC = task_6A0;
    switch (local_FC) {
      case 0:
        Script_SetHandRootRotation(CHARA_TAIJ, 1, 0x15, &data_1B070);
        Script_GetDataFromFightProgram(CHARA_AKIR, 1);
        Script_SetFightProgramStateBytes(CHARA_AKIR, 3, 1);
        break;
      case 40:
        Script_GetDataFromFightProgram(CHARA_AKIR, 0);
        break;
      case 0x400:
        Script_handSetFloatsFor(CHARA_KISY, 1, data_16DC0, 0);
        Script_handSetFloatsFor(CHARA_KISY, 0, data_16CDC, 0);
        break;
    }
    task_6A0 = task_6A0 + 1;
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_122B3(void) {
  int32_t local_FC;
  task_6A0 = 0;
  for (;;) {
    local_FC = task_6A0;
    switch (local_FC) {
      case 0:
        Script_handSetFloatsFor(CHARA_KISY, 1, data_16BF8, 0);
        Script_handSetFloatsFor(CHARA_KISY, 0, data_16CDC, 0);
        break;
      case 345:
        Script_handSetFloatsFor(CHARA_KISY, 1, data_16DC0, 0);
        break;
    }
    task_6A0 = task_6A0 + 1;
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static float script_12329(void) {
  int32_t local_FC;
  task_6A0 = 0;
  Script_SetShadowRange(200.0f);
  for (;;) {
    local_FC = task_6A0;
    switch (local_FC) {
      case 0:
        Script_SetHandRootRotation(CHARA_TAIJ, 1, 0x15, &data_1B07C);
        break;
      case 825:
        Script_handSetFloatsFor(CHARA_KISY, 1, data_16DC0, 0);
        Script_handSetFloatsFor(CHARA_KISY, 0, data_16CDC, 0);
        break;
    }
    task_6A0 = task_6A0 + 1;
    SCNF_WaitFrames_0000(1);
  }
  return Script_SetShadowRange(20.0f);
}

static int32_t script_123B4(void) {
  int32_t local_FC;
  task_6A0 = 0;
  for (;;) {
    local_FC = task_6A0;
    switch (local_FC) {
      case 0:
        Script_handSetFloatsFor(CHARA_AKIR, 1, data_16BF8, 0);
        break;
      case 11:
        Script_GetDataFromFightProgram(CHARA_KISY, 1);
        Script_SetFightProgramStateBytes(CHARA_KISY, 3, 1);
        break;
      case 35:
        Script_GetDataFromFightProgram(CHARA_KISY, 0);
        break;
      case 183:
        Script_handSetFloatsFor(CHARA_AKIR, 1, data_16B14, 0);
        break;
    }
    task_6A0 = task_6A0 + 1;
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_12456(void) {
  task_6A0 = 0;
  for (;;) {
    if (task_6A0 == 0) {
      Script_SetHandRootRotation(CHARA_TAIJ, 1, 0x15, &data_1B088);
    }
    task_6A0 = task_6A0 + 1;
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_12493(void) {
  int32_t local_FC;
  task_6A0 = 0;
  for (;;) {
    local_FC = task_6A0;
    if (local_FC == 0) {
      Script_SetHandRootRotation(CHARA_TAIJ, 1, 0x15, &data_1B094);
      goto L0124C2;
    }
    L0124C2:;
    task_6A0 = task_6A0 + 1;
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_124D9(void) {
  task_6A0 = 0;
  for (;;) {
    if (task_6A0 == 0) {
      Script_SetHandRootRotation(CHARA_TAIJ, 1, 0x15, &data_1B0A0);
    }
    task_6A0 = task_6A0 + 1;
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_12516(void) {
  int32_t local_FC;
  task_6A0 = 0;
  for (;;) {
    local_FC = task_6A0;
    if (local_FC == 0) {
      Script_SetHandRootRotation(CHARA_TAIJ, 1, 0x15, &data_1B0AC);
      goto L012545;
    }
    L012545:;
    task_6A0 = task_6A0 + 1;
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_1255C(void) {
  task_6A8 = 0;
  for (;;) {
    task_6A8 = task_6A8 + 1;
    if (task_6AC != 0) {
      if (task_6A8 > task_6AC) {
        Script_executeAicaCommand(2720, 2, 30);
        Script_filmCheckOrSetTransitionUtil(32, 0, 0, 0, 0, 255, 0, 0, 0);
        break;
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  for (;;) {
    SCNF_WaitFrames_0000(1);
    task_6A8 = task_6A8 + 1;
  }
  end:;
}

static void script_125CC(int32_t arg0) {
  int32_t unused_FC;
  int32_t local_F8;
  Script_SetMotnFlag(CHARA_AKIR, -1);
  Script_SetMotnFlag(CHARA_KISY, -1);
  Script_SetMotnFlag(CHARA_TAIJ, -1);
  Script_motmToggleFlags(CHARA_AKIR, 2, 0x40);
  Script_motmToggleFlags(CHARA_TAIJ, 2, 0x40);
  Script_motmToggleFlags(CHARA_KISY, 2, 0x40);
  SCNF_WaitFrames_0000(1);
  Script_shenSeqUtils(-9, arg0, 0);
  local_F8 = Script_scenCreateTask(script_1255C, 0);
  task_6AC = 0;
  Script_sprintf("Author Start : %d\012", arg0);
  Script_clmdSetFlags(CHARA_TAIJ, 8, 1);
  {
    int32_t local_F4;
    local_F4 = arg0;
    switch (local_F4) {
      case 1:
        Script_charaDisplay(SHEN_NAME('BAG_'), CDM_Hide);
        Script_charaDisplay(SHEN_NAME('BAG0'), CDM_Hide);
        break;
      case 5:
        Script_charaDisplay(SHEN_NAME('BAG_'), CDM_Hide);
        Script_charaDisplay(SHEN_NAME('BAG0'), CDM_Hide);
        break;
      case 8:
        task_6AC = 560;
        break;
      case 9:
        task_6AC = 690;
        break;
      case 10:
        break;
    }
  }
  script_11C2A(arg0);
  {
    int32_t local_F4;
    local_F4 = arg0;
    switch (local_F4) {
      case 1:
        break;
      case 8:
        Script_motDetailFlagUtil(CHARA_TAIJ, 0, 0);
        Script_motDetailFlagUtil(CHARA_KISY, 0, 0);
        break;
      case 9:
        Script_motDetailFlagUtil(CHARA_TAIJ, 0, 0);
        Script_motDetailFlagUtil(CHARA_KISY, 0, 0);
        break;
      case 10:
        break;
    }
  }
  Script_shenSeqUtils(-6);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_0, 100.0f);
  Script_LODUtils(LOD_UTILS_SET_DISTANCE_2, 50.0f);
  Script_ccowUtils(CHARA_AKIR, 2);
  Script_ccowUtils(CHARA_KISY, 2);
  Script_ccowUtils(CHARA_TAIJ, 2);
  Script_SetCPAMFlags(CHARA_AKIR, 0);
  Script_SetCPAMFlags(CHARA_KISY, 0);
  Script_SetCPAMFlags(CHARA_TAIJ, 0);
  Script_scenToggleActiveFor(CHARA_AKIR, 1);
  Script_scenToggleActiveFor(CHARA_KISY, 1);
  Script_scenToggleActiveFor(CHARA_TAIJ, 1);
  Script_sprintf("Author End : %d %d\012", arg0, task_6A8);
  task_6AC = 0;
  if (local_F8) {
    Script_srchControl((void *)local_F8);
    local_F8 = 0;
  }
  task_FC = -1;
  return;
}

static int32_t script_12803(int32_t arg0, int32_t arg1) {
  if (arg1 > 0) {
    SCNF_WaitFrames_0000(arg1);
  }
  Script_executeAicaCommand(arg0, 0, 0);
  return Script_srchControl(0);
}

static int32_t script_12827(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  task_6CC[arg1] = 0;
  task_6D4[arg1] = 0;
  task_6DC[arg1] = 0;
  task_6E4[arg1] = arg2;
  task_6EC[arg1] = arg3;
  task_6F4 = 0;
  Script_CharHandlers(arg0, 0, task_6E4[arg1], 16);
  Script_CharHandlers(arg0, 1, task_6EC[arg1], 16);
  for (;;) {
    task_6CC[arg1] = 0;
    while (task_6CC[arg1] == 0) {
      SCNF_WaitFrames_0000(1);
    }
    task_6F4 = task_6DC[arg1] | 0x40000000;
    Script_scenCreateTask(arg4, 3, arg0, arg1, task_6CC[arg1] + task_6D4[arg1]);
  }
  end:;
}

static int32_t script_128FE(int32_t arg0, int32_t arg1, int32_t arg2, float arg3, int32_t arg4, int32_t arg5) {
  int32_t local_FC;
  local_FC = task_6F4;
  if (arg1 > 0) {
    SCNF_WaitFrames_0000(arg1);
  }
  Script_motmGetCharaFlags(arg4, 0, 0);
  Script_motmToggleFlags(arg4, 1, 0x40);
  Script_PlayCharaMotion(arg4, arg0, arg2, -1.0f, -1.0f, 1.0f, 0);
  Script_SetCharaMotionPlaybackRate(arg4, arg3);
  if (local_FC & -0x80000000) {
    Script_motmToggleFlags(arg4, 1, 0x80);
  }
  if (local_FC & 0x40000000) {
    Script_charaToggleMotEnabled(arg4, 1);
  }
  if (local_FC & 0x20000000) {
    Script_charaToggleMotEnabled(arg4, 0);
  }
  return Script_srchControl(0);
}

static int32_t script_12995(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  if (arg3 > 0) {
    SCNF_WaitFrames_0000(arg3);
  }
  Script_CharHandlers(arg0, 2, arg2, 16);
  if (arg4 > 0) {
    SCNF_WaitFrames_0000(arg4);
  }
  Script_CharHandlers(arg0, 0, task_6E4[arg1], 16);
  Script_CharHandlers(arg0, 1, task_6EC[arg1], 16);
  return Script_srchControl(0);
}

static int32_t script_129F7(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  local_FC = task_6F4;
  if (arg2 > 0) {
    SCNF_WaitFrames_0000(arg2);
  }
  if (local_FC & 0x10000000) {
    Script_GetDataFromFightProgram(arg0, arg1);
  }
  if (arg3 > 0) {
    SCNF_WaitFrames_0000(arg3);
  }
  if (local_FC & 0x10000000) {
    Script_GetDataFromFightProgram(arg0, 0);
  }
  return Script_srchControl(0);
}

static void script_12A53(int32_t arg0, float arg1, int32_t arg2) {
  int32_t local_FC;
  Vector3i local_F0;
  local_FC = task_6F4;
  if (arg2 > 0) {
    SCNF_WaitFrames_0000(arg2);
  }
  if (local_FC & -0x80000000) {
    arg1 = (float)0 - arg1;
  }
  (&local_F0)->y = (int32_t)(arg1 / 360.0f * (float)0x10000);
  Script_EVT_SetRotation(arg0, -0x70000000, &local_F0, 0);
  Script_srchControl(0);
  return;
}

static void script_12AB0(int32_t arg0) {
  while (Script_IsCharaMotionFinished(arg0) == 0) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static int32_t script_12ACC(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, void *arg4, int32_t arg5) {
  int32_t local_FC;
  task_710[arg1] = Script_scenCreateTask(script_12827, 5, arg0, arg1, arg2, arg3, arg5);
  task_6F8[arg1] = arg0;
  task_700[arg1] = 0;
  task_708[arg1] = 0;
  task_718 = 4;
  for (;;) {
    SCNF_WaitFrames_0000(1);
    if (task_700[arg1]) {
      local_FC = task_708[arg1] * 12;
      {
        int32_t local_F8;
        local_F8 = Script_ScenDataUtil(arg4, 4, local_FC + 5);
        if (local_F8 > 0) {
          SCNF_WaitFrames_0000(local_F8);
        }
      }
      {
        int32_t local_F8;
        local_F8 = Script_ScenDataUtil(arg4, 4, local_FC + 8);
        if (local_F8 > 0) {
          task_6CC[arg1] = local_F8;
        }
      }
      {
        int32_t local_F8;
        local_F8 = Script_ScenDataUtil(arg4, 4, local_FC + 6);
        if (local_F8 > 0) {
          SCNF_WaitFrames_0000(local_F8);
        }
      }
      if (task_6D4[arg1] == 0) {
        Script_scenCreateTask(script_12E3C, 4, Script_ScenDataUtil(arg4, 4, local_FC), Script_ScenDataUtil(arg4, 4, local_FC + 1), Script_ScenDataUtil(arg4, 4, local_FC + 2), Script_ScenDataUtil(arg4, 4, local_FC + 3));
      }
      {
        int32_t local_F8;
        local_F8 = Script_ScenDataUtil(arg4, 4, local_FC + 7);
        if (local_F8 > 0) {
          SCNF_WaitFrames_0000(local_F8);
        }
      }
      {
        int32_t local_F8;
        local_F8 = Script_ScenDataUtil(arg4, 4, local_FC + 9);
        if (local_F8 > 0) {
          task_6CC[arg1] = local_F8;
        }
      }
      script_12D3A();
      {
        int32_t local_F8;
        if (task_724 == Script_ScenDataUtil(arg4, 4, local_FC)) {
          task_720 = 1;
          local_F8 = Script_ScenDataUtil(arg4, 4, local_FC + 10);
        } else {
          if (task_6D4[arg1] == 0) {
            task_72C = task_72C + Script_ScenDataUtil(arg4, 4, local_FC + 4);
          }
          task_720 = 0;
          local_F8 = Script_ScenDataUtil(arg4, 4, local_FC + 11);
        }
        if (local_F8 > 0) {
          task_6CC[arg1] = local_F8;
        }
      }
      script_12D2A(arg1);
    }
  }
  end:;
}

static void script_12CF3(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  task_700[arg0] = 1;
  task_708[arg0] = arg1;
  task_6D4[arg0] = arg2;
  task_6DC[arg0] = arg3;
  return;
}

static void script_12D2A(int32_t arg0) {
  task_700[arg0] = 0;
  return;
}

static void script_12D3A(void) {
  SCNF_WaitFrames_0000(2);
  while (task_724 == 0) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_12D5A(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  script_12CF3(arg0, arg1, 0, arg3);
  script_12CF3(arg2, arg1, 1, arg3);
  while (task_700[arg0]) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_12D94(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  script_12CF3(arg0, arg1, 0, arg3);
  script_12CF3(arg2, arg1, 1, arg3);
  script_12CF3(arg2 + 1, arg1, 2, arg3);
  while (task_700[arg0]) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_12DE0(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  script_12CF3(arg0, arg1, 0, arg4);
  script_12CF3(arg2, arg1, 1, arg4);
  script_12CF3(arg3, arg1, 2, arg4);
  while (task_700[arg0]) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_12E29(int32_t arg0) {
  task_718 = arg0;
  Script_SetSfxOverlayTimeoutTuning(arg0, SfxMode_SetBoostStepFrames);
  return;
}

static int32_t script_12E3C(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  int32_t local_F8;
  local_FC = arg1 + arg3;
  local_F8 = arg2 + arg3;
  Script_scenCreateTask(script_12E9F, 2, arg1, arg3);
  Script_SetSfxOverlayTimeoutTuning(local_FC, SfxMode_SetMaxFrames);
  if (arg0 == 68) {
    SCNF_UIUtils(0x44, local_F8, 5, 0, 1, 0);
  } else {
    SCNF_UIUtils(arg0, local_F8, 2, 0, 1, 0);
  }
  return Script_srchControl(0);
}

static int32_t script_12E9F(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  int32_t local_F8;
  task_728 = arg0;
  task_724 = 0;
  task_71C = 0;
  local_F8 = 0xFFFF;
  for (;;) {
    local_FC = SCNF_UIUtils(0, 0, 0, 0, 0, -1);
    task_71C = task_71C + 1;
    if (local_FC != 0xFFFF) {
      if (local_FC != 0xA000) {
        local_F8 = local_FC;
      }
    }
    if (arg0 == -1) {
      if (local_F8 != 0xFFFF) {
        break;
      }
    } else {
      if (task_71C >= arg0 + arg1 + 4) {
        break;
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  if ((local_F8 & 0xFFF) == 0) {
    local_F8 = 0xFFFF;
  }
  task_724 = local_F8 & 0xFFF;
  return Script_srchControl(0);
}

static int32_t script_12F60(int32_t arg0, int32_t arg1, int32_t arg2) {
  {
    Vector3 local_F4;
    Script_EVT_GetPosition(arg0, -1, &local_F4, 0);
    task_730[arg1] = arg0;
    task_738[arg1][0] = (&local_F4)->x;
    task_738[arg1][1] = (&local_F4)->y;
    task_738[arg1][2] = (&local_F4)->z;
  }
  task_750[arg1] = 0.0f;
  for (;;) {
    Vector3 local_F4;
    SCNF_WaitFrames_0000(1);
    Script_EVT_GetPosition(arg0, -1, &local_F4, 0);
    task_738[arg1][0] = (&local_F4)->x;
    task_738[arg1][1] = (&local_F4)->y;
    task_738[arg1][2] = (&local_F4)->z;
    task_750[arg1] = Script_MathUtils(SCRIPT_MATH_VEC2_LENGTH, task_738[arg1][0] - task_738[arg2][0], task_738[arg1][2] - task_738[arg2][2]);
  }
  end:;
}

static void script_1304E(void) {
  task_758 = 0.0f;
  task_75C = 0.0f;
  task_760 = 0.0f;
  for (;;) {
    SCNF_WaitFrames_0000(1);
    if (Script_GetInputData(2, 0) & 4) {
      if (Script_GetInputData(0, 2) & 0x4000) {
        task_758 = task_758 - 0.01f;
      }
      if (Script_GetInputData(0, 2) & 0x8000) {
        task_758 = task_758 + 0.01f;
      }
      if (Script_GetInputData(0, 2) & 0x1000) {
        task_75C = task_75C - 0.01f;
      }
      if (Script_GetInputData(0, 2) & 0x2000) {
        task_75C = task_75C + 0.01f;
      }
      if (Script_GetInputData(0, 2) & 0x40) {
        task_760 = task_760 - 1.0f;
      }
      if (Script_GetInputData(0, 2) & 0x80) {
        task_760 = task_760 + 1.0f;
      }
    }
  }
  return;
}

static void script_1313C(int32_t arg0, int32_t arg1) {
  Vector3 local_F4;
  Vector3i local_E8;
  Script_MoveObjectToObject(arg0, arg1);
  Script_EVT_GetPosition(arg0, -1, &local_F4, 0);
  Script_mtwkGetSetRotation(arg0, &local_E8, 0);
  Script_MoveObjectToObject(arg0, -1);
  return;
}

static void script_13165(int32_t arg0, int32_t arg1, float arg2, float arg3, float arg4) {
  float local_F4[3];
  Vector3i local_E8;
  int32_t local_E4;
  Vector3 local_D8;
  local_E4 = Script_CEWPRelated(arg0);
  Script_MoveObjectToObject(arg0, arg1);
  local_F4[0] = arg2;
  local_F4[1] = 0.0f;
  local_F4[2] = arg3;
  local_D8.x = arg2;
  (&local_D8)->z = arg3;
  Script_EVT_SetPosition(arg0, 0x28000000, &local_D8);
  local_E8.x = 0;
  (&local_E8)->y = (int32_t)(arg4 / 360.0f * (float)0x10000);
  (&local_E8)->z = 0;
  Script_EVT_SetRotation(arg0, 0x38000000, &local_E8, 0);
  Script_MoveObjectToObject(arg0, local_E4);
  Script_InitializeOsag(arg0);
  return;
}

static void script_131F4(int32_t arg0, int32_t arg1, const Vector3 *arg2) {
  int32_t local_FC;
  local_FC = Script_CEWPRelated(arg0);
  Script_MoveObjectToObject(arg0, arg1);
  Script_EVT_SetPosition(arg0, POS_XYZ, arg2);
  Script_MoveObjectToObject(arg0, local_FC);
  Script_InitializeOsag(arg0);
  return;
}

static void script_13227(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
  Script_scenCreateTask(script_13241, 6, arg0, arg1, arg2, arg3, arg4, arg5);
  return;
}

static int32_t script_13241(int32_t arg0, int32_t arg1, float arg2, float arg3, float arg4, int32_t arg5) {
  if (arg5 >= 0) {
    SCNF_WaitFrames_0000(arg5);
  }
  script_13165(arg0, arg1, arg2, arg3, arg4);
  return Script_srchControl(0);
}

static void script_1326C(int32_t arg0, int32_t arg1, float arg2, float arg3, float arg4, float arg5) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t local_E4;
  local_E4 = Script_CEWPRelated(arg0);
  Script_MoveObjectToObject(arg0, arg1);
  local_F4.x = arg2;
  (&local_F4)->y = arg3;
  (&local_F4)->z = arg4;
  Script_EVT_SetPosition(arg0, POS_XYZ, &local_F4);
  local_E8.x = 0;
  (&local_E8)->y = (int32_t)(arg5 / 360.0f * (float)0x10000);
  (&local_E8)->z = 0;
  Script_EVT_SetRotation(arg0, 0x38000000, &local_E8, 0);
  Script_MoveObjectToObject(arg0, local_E4);
  Script_InitializeOsag(arg0);
  return;
}

static void script_132EC(int32_t arg0, int32_t arg1, float arg2) {
  int32_t unused_F4[3];
  Vector3i local_E8;
  int32_t local_E4;
  local_E4 = Script_CEWPRelated(arg0);
  Script_MoveObjectToObject(arg0, arg1);
  local_E8.x = 0;
  (&local_E8)->y = (int32_t)(arg2 / 360.0f * (float)0x10000);
  (&local_E8)->z = 0;
  Script_EVT_SetRotation(arg0, 0x38000000, &local_E8, 0);
  Script_MoveObjectToObject(arg0, local_E4);
  return;
}

static void script_13340(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Script_scenCreateTask(script_13356, 4, arg0, arg1, arg2, arg3);
  return;
}

static int32_t script_13356(int32_t arg0, int32_t arg1, float arg2, int32_t arg3) {
  if (arg3 >= 0) {
    SCNF_WaitFrames_0000(arg3);
  }
  script_132EC(arg0, arg1, arg2);
  return Script_srchControl(0);
}

static void script_1337D(int32_t arg0, float arg1) {
  Vector3i local_F4;
  (&local_F4)->y = (int32_t)(arg1 / 360.0f * (float)0x10000);
  Script_EVT_SetRotation(arg0, -0x70000000, &local_F4, 0);
  return;
}

static void script_133A6(int32_t arg0, float arg1, float arg2, float arg3) {
  Vector3i local_F4;
  local_F4.x = (int32_t)(arg1 / 360.0f * (float)0x10000);
  (&local_F4)->y = (int32_t)(arg2 / 360.0f * (float)0x10000);
  (&local_F4)->z = (int32_t)(arg3 / 360.0f * (float)0x10000);
  Script_EVT_SetRotation(arg0, -0x48000000, &local_F4, 0);
  return;
}

static void script_133FA(int32_t arg0, int32_t arg1, int32_t arg2) {
  Script_scenCreateTask(script_1340E, 3, arg0, arg1, arg2);
  return;
}

static int32_t script_1340E(int32_t arg0, float arg1, int32_t arg2) {
  if (arg2 >= 0) {
    SCNF_WaitFrames_0000(arg2);
  }
  script_1337D(arg0, arg1);
  return Script_srchControl(0);
}

static void script_13433(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  Script_scenCreateTask(script_1344B, 5, arg0, arg1, arg2, arg3, arg4);
  return;
}

static int32_t script_1344B(int32_t arg0, float arg1, float arg2, float arg3, int32_t arg4) {
  if (arg4 >= 0) {
    SCNF_WaitFrames_0000(arg4);
  }
  script_133A6(arg0, arg1, arg2, arg3);
  return Script_srchControl(0);
}

static void script_13474(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  Script_scenCreateTask(script_1348C, 5, arg0, arg1, arg2, arg3, arg4);
  return;
}

static int32_t script_1348C(int32_t arg0, float arg1, float arg2, float arg3, int32_t arg4) {
  float local_FC;
  if (arg4 >= 0) {
    SCNF_WaitFrames_0000(arg4);
  }
  local_FC = (arg2 - arg1) / arg3;
  for (;;) {
    if (arg3 <= 0.0f) {
      break;
    }
    script_133A6(arg0, 0.0f, 0.0f, local_FC);
    arg3 = arg3 - 1.0f;
    SCNF_WaitFrames_0000(1);
  }
  return Script_srchControl(0);
}

static void script_134E1(int32_t arg0, int32_t arg1) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t local_E4;
  local_E4 = arg0;
  while (local_E4 <= arg1) {
    Script_EVT_GetPosition(task_6F8[local_E4], -1, &local_F4, 0);
    Script_mtwkGetSetRotation(task_6F8[local_E4], &local_E8, 0);
    task_764[local_E4][0] = (&local_F4)->x;
    task_764[local_E4][1] = (&local_F4)->y;
    task_764[local_E4][2] = (&local_F4)->z;
    task_77C[local_E4][0] = (&local_E8)->x;
    task_77C[local_E4][1] = (&local_E8)->y;
    task_77C[local_E4][2] = (&local_E8)->z;
    task_794[local_E4] = Script_GetCharaMotionRemainingFramesScaled(task_6F8[local_E4]);
    task_79C[local_E4] = Script_scenCharaGetMotAnimId(task_6F8[local_E4]);
    local_E4 = local_E4 + 1;
  }
  return;
}

static void script_135C6(int32_t arg0, int32_t arg1) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t local_E4;
  local_E4 = arg0;
  while (local_E4 <= arg1) {
    local_F4.x = task_764[local_E4][0];
    (&local_F4)->y = task_764[local_E4][1];
    (&local_F4)->z = task_764[local_E4][2];
    local_E8.x = task_77C[local_E4][0];
    (&local_E8)->y = task_77C[local_E4][1];
    (&local_E8)->z = task_77C[local_E4][2];
    Script_EVT_SetPosition(task_6F8[local_E4], POS_XYZ, &local_F4);
    Script_EVT_SetRotation(task_6F8[local_E4], 0x38000000, &local_E8, 0);
    Script_PlayCharaMotion(task_6F8[local_E4], task_79C[local_E4], task_794[local_E4], -1.0f, -1.0f, 1.0f, 0);
    local_E4 = local_E4 + 1;
  }
  return;
}

static void script_136AF(int32_t arg0) {
  task_7A8 = arg0;
  task_7A4 = 1;
  Script_PlayerStatsUtils(PS_Set70MBTimeCapSlot, task_7A8, task_7A4);
  return;
}

static void script_136CD(void) {
  int32_t local_FC;
  if (task_7A4 == 1) {
    local_FC = Script_PlayerStatsUtils(PS_Get70MBCounter1);
    Script_PlayerStatsUtils(PS_Set70MBCounter1, local_FC + 1);
  }
  if (task_7A4 < 255) {
    task_7A4 = task_7A4 + 1;
  }
  Script_PlayerStatsUtils(PS_Set70MBTimeCapSlot, task_7A8, task_7A4);
  return;
}

static void script_13715(void) {
  int32_t local_FC;
  local_FC = Script_PlayerStatsUtils(PS_Get70MBCounter0);
  Script_PlayerStatsUtils(PS_Set70MBCounter0, local_FC + 1);
  return;
}

static void script_13730(void) {
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_1374A(const char *arg0, const char *arg1) {
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 5, arg1) == 0) {
    while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, arg0, arg1) == 0) {
      SCNF_WaitFrames_0000(1);
    }
    script_13730();
  }
  return;
}

static void script_13788(const char *arg0) {
  if (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 5, arg0) != 0) {
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, arg0);
  }
  return;
}

static void script_137A9(int32_t arg0) {
  L0137AB:;
  SCNF_WaitFrames_0000(1);
  if (!Script_CycleManagerUtils(CYCLE_CMD_GET_NEXT_TASK_POINTER, arg0)) goto L0137AB;
  return;
}

static void script_137C0(int32_t arg0) {
  int32_t local_FC;
  local_FC = 0;
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0) != 0) {
    local_FC = Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT_INTERNAL_HUMAN, arg0);
  } else {
    Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT, arg0, 1);
    while (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0) == 0) {
      local_FC = Script_CycleManagerUtils(CYCLE_CMD_GET_CYCLE_PTR_BY_CHARA_ID, arg0);
      while (local_FC == 0) {
        SCNF_WaitFrames_0000(1);
        local_FC = Script_CycleManagerUtils(CYCLE_CMD_GET_CYCLE_PTR_BY_CHARA_ID, arg0);
      }
      script_137A9(local_FC);
    }
  }
  (void)local_FC;
  return;
}

static void script_13846(int32_t arg0, int32_t arg1) {
  if (arg1) {
    Script_CycleManagerUtils(CYCLE_CMD_LOAD_OR_RESET_RUNNING_PROGRAM, arg1);
  }
  Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT, arg0, 0);
  arg1 = 0;
  return;
}

static void script_13869(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  Vector3 local_E8;
  int32_t local_E4;
  Script_shenSeqUtils(1);
  task_7AC = 0;
  local_F4 = 0;
  local_FC = 0;
  while (Script_shenSeqUtils(-1)) {
    local_E4 = local_FC;
    switch (local_E4) {
      case 1:
        Script_SetMaxInterpSpeed(0.0f);
        Script_SetMotnFlag(CHARA_AKIR, -1);
        Script_SetMotnFlag(CHARA_CHAI, -1);
        Script_handSetFloatsFor(CHARA_AKIR, 0, data_1B1C4, 1);
        Script_handSetFloatsFor(CHARA_AKIR, 1, data_1B0E0, 1);
        Script_handSetFloatsFor(CHARA_AKIR, 1, data_1B1C4, 1);
        Script_handSetFloatsFor(CHARA_CHAI, 0, data_1B2A8, 1);
        Script_handSetFloatsFor(CHARA_CHAI, 1, data_1B2A8, 1);
        Script_FACE(CHARA_CHAI, 0, 0, 10);
        Script_FACE_LCG(CHARA_CHAI, 1, 0, 0);
        break;
      case 131:
        task_7AC = 1;
        break;
    }
    if (local_FC < 131) {
      if (local_F4 == 0) {
        if (local_F8 == 0) {
          Script_handSetFloatsFor(CHARA_CHAI, 0, data_1B38C, 2);
          local_F8 = 1;
          local_F4 = Script_FloatUtils(ScriptMathOp_RandomIntBelow, 5) + 10;
        } else if (local_F8 == 1) {
          Script_handSetFloatsFor(CHARA_CHAI, 0, data_1B2A8, 2);
          local_F8 = 2;
          local_F4 = 0;
        } else if (local_F8 == 2) {
          Script_handSetFloatsFor(CHARA_CHAI, 0, data_1B38C, 2);
          local_F8 = 3;
          local_F4 = 3;
        } else {
          Script_handSetFloatsFor(CHARA_CHAI, 0, data_1B2A8, 2);
          local_F8 = 0;
          local_F4 = 1;
        }
      } else {
        local_F4 = local_F4 - 1;
      }
    }
    if (local_FC == 56) {
      Script_FACE_LCG(CHARA_CHAI, 2, 2, 0);
    }
    if (local_FC == 59) {
      Script_FACE(CHARA_CHAI, 5, 0, 10);
    }
    if (local_FC == 83) {
      Script_FACE(CHARA_AKIR, 5, 0, 7);
    }
    if (local_FC == 121) {
      Script_handSetFloatsFor(CHARA_CHAI, 0, data_1B0E0, 20);
      Script_handSetFloatsFor(CHARA_CHAI, 1, data_1B0E0, 20);
    }
    switch (local_FC) {
      case 74:
        Script_SetFightProgramStateBytes(CHARA_CHAI, 2, 1);
        Script_GetDataFromFightProgram(CHARA_CHAI, 1);
        break;
      case 141:
        Script_GetDataFromFightProgram(CHARA_CHAI, 0);
        break;
    }
    if (local_FC == 75) {
      local_E8.x = 70.702499f;
      (&local_E8)->y = 0.8512f;
      (&local_E8)->z = 24.5648f;
      Script_FaceLookAtTargetUtils(CHARA_AKIR, 2, &local_E8, 5);
    }
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
  }
  Script_SetMaxInterpSpeed(0.5f);
  return;
}

static int32_t script_13B21(int32_t arg0, int32_t arg1, int32_t arg2) {
  int32_t local_FC;
  SCNF_WaitFrames_0000(1);
  local_FC = arg2;
  switch (local_FC) {
    case 1:
      Script_scenCreateTask(script_128FE, 6, 23301, -1, 0, 0x3F99999A, arg0, arg1);
      Script_scenCreateTask(script_12995, 5, arg0, arg1, 0, -1, 30);
      break;
    case 2:
      Script_scenCreateTask(script_128FE, 6, 23311, -1, 0, 0x3F99999A, arg0, arg1);
      Script_scenCreateTask(script_12803, 2, 0x2176AB, 15);
      Script_scenCreateTask(script_12803, 2, 0x2976AB, 30);
      Script_scenCreateTask(script_129F7, 4, arg0, 1, 18, 10);
      Script_scenCreateTask(script_12995, 5, arg0, arg1, 0, -1, 30);
      break;
    case 3:
      Script_scenCreateTask(script_128FE, 6, 23297, -1, 0, 0x3F99999A, arg0, arg1);
      Script_scenCreateTask(script_128FE, 6, 23317, 60, 0x42D20000, 0x3F99999A, arg0, arg1);
      Script_scenCreateTask(script_12803, 2, 161451, 23);
      Script_scenCreateTask(script_12803, 2, 0x2876AB, 38);
      Script_scenCreateTask(script_12803, 2, 161451, 115);
      break;
    case 4:
      Script_scenCreateTask(script_128FE, 6, 23306, -1, 0, 0x3F99999A, arg0, arg1);
      Script_scenCreateTask(script_128FE, 6, 23320, 60, 0x42D20000, 0x3F99999A, arg0, arg1);
      Script_scenCreateTask(script_12803, 2, 0x2176AB, 15);
      Script_scenCreateTask(script_12803, 2, 0x2976AB, 30);
      Script_scenCreateTask(script_12803, 2, 0x2176AB, 95);
      Script_scenCreateTask(script_12803, 2, 0x2976AB, 110);
      Script_scenCreateTask(script_12803, 2, 0x2176AB, 148);
      Script_scenCreateTask(script_12803, 2, 0x2976AB, 165);
      Script_scenCreateTask(script_129F7, 4, arg0, 1, 18, 10);
      Script_scenCreateTask(script_129F7, 4, arg0, 1, 95, 15);
      Script_scenCreateTask(script_129F7, 4, arg0, 1, 148, 15);
      Script_scenCreateTask(script_12995, 5, arg0, arg1, 0, -1, 30);
      break;
    case 5:
      Script_scenCreateTask(script_128FE, 6, 23302, -1, 0, 0x3F99999A, arg0, arg1);
      Script_scenCreateTask(script_12803, 2, 816811, 26);
      Script_scenCreateTask(script_129F7, 4, arg0, 6, 24, 8);
      break;
    case 6:
      Script_scenCreateTask(script_128FE, 6, 23312, -1, 0, 0x3F99999A, arg0, arg1);
      Script_scenCreateTask(script_12803, 2, 0x1976AB, 20);
      Script_scenCreateTask(script_12803, 2, 161451, 30);
      Script_scenCreateTask(script_12803, 2, 0x2176AB, 62);
      Script_scenCreateTask(script_129F7, 4, arg0, 1, 63, 10);
      break;
    case 7:
      Script_scenCreateTask(script_128FE, 6, 23298, -1, 0, 0x3F99999A, arg0, arg1);
      Script_scenCreateTask(script_128FE, 6, 23300, 141, 0x42CC0000, 0x3F99999A, arg0, arg1);
      Script_scenCreateTask(script_12803, 2, 161451, 25);
      Script_scenCreateTask(script_12803, 2, 0x2876AB, 60);
      Script_scenCreateTask(script_12803, 2, 30379, 169);
      Script_scenCreateTask(script_12803, 2, 161451, 181);
      Script_scenCreateTask(script_12803, 2, 30379, 195);
      break;
    case 8:
      Script_scenCreateTask(script_128FE, 6, 23308, -1, 0, 0x3F99999A, arg0, arg1);
      Script_scenCreateTask(script_128FE, 6, 23310, 81, 0x41F00000, 0x3F99999A, arg0, arg1);
      Script_scenCreateTask(script_12803, 2, 0x1976AB, 20);
      Script_scenCreateTask(script_12803, 2, 0x2176AB, 119);
      Script_scenCreateTask(script_12803, 2, 0x2176AB, 232);
      Script_scenCreateTask(script_12803, 2, 0x2976AB, 253);
      Script_scenCreateTask(script_129F7, 4, arg0, 6, 25, 5);
      Script_scenCreateTask(script_129F7, 4, arg0, 1, 119, 12);
      Script_scenCreateTask(script_129F7, 4, arg0, 1, 236, 12);
      break;
    case 9:
      Script_scenCreateTask(script_128FE, 6, 23303, -1, 0, 0x3F99999A, arg0, arg1);
      Script_scenCreateTask(script_12803, 2, 816811, 30);
      Script_scenCreateTask(script_12803, 2, 0x1976AB, 35);
      Script_scenCreateTask(script_12803, 2, 161451, 39);
      Script_scenCreateTask(script_12803, 2, 816811, 54);
      Script_scenCreateTask(script_129F7, 4, arg0, 6, 37, 7);
      Script_scenCreateTask(script_129F7, 4, arg0, 7, 54, 8);
      break;
    case 10:
      Script_scenCreateTask(script_128FE, 6, 23313, -1, 0, 0x3F99999A, arg0, arg1);
      Script_scenCreateTask(script_12803, 2, 0x1976AB, 26);
      Script_scenCreateTask(script_129F7, 4, arg0, 6, 27, 7);
      break;
    case 11:
      Script_scenCreateTask(script_128FE, 6, 23299, -1, 0, 0x3F99999A, arg0, arg1);
      Script_scenCreateTask(script_128FE, 6, 23318, 75, 0x425C0000, 0x3F99999A, arg0, arg1);
      Script_scenCreateTask(script_12803, 2, 161451, 21);
      Script_scenCreateTask(script_12803, 2, 0x2876AB, 37);
      Script_scenCreateTask(script_12803, 2, 161451, 120);
      break;
    case 12:
      Script_scenCreateTask(script_128FE, 6, 23309, -1, 0, 0x3F99999A, arg0, arg1);
      Script_scenCreateTask(script_128FE, 6, 23321, 75, 0x425C0000, 0x3F99999A, arg0, arg1);
      Script_scenCreateTask(script_12803, 2, 0x1976AB, 18);
      Script_scenCreateTask(script_12803, 2, 0x2176AB, 105);
      Script_scenCreateTask(script_12803, 2, 0x2176AB, 148);
      Script_scenCreateTask(script_12803, 2, 0x2976AB, 160);
      Script_scenCreateTask(script_129F7, 4, arg0, 6, 18, 10);
      Script_scenCreateTask(script_129F7, 4, arg0, 1, 105, 10);
      Script_scenCreateTask(script_129F7, 4, arg0, 3, 136, 10);
      Script_scenCreateTask(script_129F7, 4, arg0, 1, 148, 10);
      break;
    case 13:
      break;
    case 14:
      break;
    case 15:
      Script_scenCreateTask(script_128FE, 6, 23300, -1, 0x41F00000, 0x3F99999A, arg0, arg1);
      Script_scenCreateTask(script_12803, 2, 0x2876AB, 60);
      Script_scenCreateTask(script_12803, 2, 30379, 88);
      Script_scenCreateTask(script_12803, 2, 161451, 100);
      Script_scenCreateTask(script_12803, 2, 30379, 114);
      break;
    case 16:
      Script_scenCreateTask(script_128FE, 6, 23310, -1, 0x41F00000, 0x3F99999A, arg0, arg1);
      Script_scenCreateTask(script_12803, 2, 0x2176AB, 38);
      Script_scenCreateTask(script_12803, 2, 0x2176AB, 151);
      Script_scenCreateTask(script_12803, 2, 0x2976AB, 172);
      Script_scenCreateTask(script_129F7, 4, arg0, 1, 38, 12);
      Script_scenCreateTask(script_129F7, 4, arg0, 1, 155, 12);
      break;
  }
  return Script_srchControl(0);
}

static void script_142D0(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Script_scenCreateTask(script_142E6, 4, arg0, arg1, arg2, arg3);
  return;
}

static int32_t script_142E6(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  if (arg3 >= 0) {
    SCNF_WaitFrames_0000(arg3);
  }
  Script_seqEnterEvtCam(arg0, arg1, arg2);
  return Script_srchControl(0);
}

static void script_1430A(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Script_scenCreateTask(script_14320, 4, arg0, arg1, arg2, arg3);
  return;
}

static int32_t script_14320(float arg0, float arg1, float arg2, int32_t arg3) {
  if (arg3 >= 0) {
    SCNF_WaitFrames_0000(arg3);
  }
  Script_SetCamShake(arg0, arg1, arg2);
  return Script_srchControl(0);
}

static void script_14345(int32_t arg0, int32_t arg1) {
  Script_scenCreateTask(script_14357, 2, arg0, arg1);
  return;
}

static int32_t script_14357(const char *arg0, int32_t arg1) {
  if (arg1 >= 0) {
    SCNF_WaitFrames_0000(arg1);
  }
  Script_StartOrFreeSEQ(0);
  Script_ShowUIMessageFromID(arg0);
  return Script_srchControl(0);
}

static void script_1437E(int32_t arg0, int32_t arg1) {
  task_7B0 = arg0;
  task_6B0 = 0;
  return;
}

static void script_1438F(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  if (arg1 >= 0) {
    SCNF_WaitFrames_0000(arg1);
  }
  local_FC = arg0;
  switch (local_FC) {
    case 0:
      SCNT_ShowSubtitle("オート０");
      break;
    case 2:
      SCNT_ShowSubtitle("開始０");
      break;
    case 3:
      SCNT_ShowSubtitle("成功０");
      break;
    case 4:
      SCNT_ShowSubtitle("失敗０");
      break;
    case 5:
      SCNT_ShowSubtitle("死亡０");
      break;
    case 6:
      SCNT_ShowSubtitle("開始１");
      break;
    case 7:
      SCNT_ShowSubtitle("成功１");
      break;
    case 8:
      SCNT_ShowSubtitle("失敗１");
      break;
    case 9:
      SCNT_ShowSubtitle("開始２");
      break;
    case 10:
      SCNT_ShowSubtitle("成功２");
      break;
    case 11:
      SCNT_ShowSubtitle("失敗２");
      break;
    case 12:
      SCNT_ShowSubtitle("開始３");
      break;
    case 13:
      SCNT_ShowSubtitle("成功３");
      break;
    case 14:
      SCNT_ShowSubtitle("失敗３");
      break;
    case 15:
      SCNT_ShowSubtitle("開始４");
      break;
    case 16:
      SCNT_ShowSubtitle("成功４");
      break;
    case 17:
      SCNT_ShowSubtitle("失敗４");
      break;
  }
  Script_srchControl(0);
  return;
}

static void script_144D0(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  Script_scenCreateTask(script_144E8, 5, arg0, arg1, arg2, arg3, arg4);
  return;
}

static int32_t script_144E8(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  if (arg4 >= 0) {
    SCNF_WaitFrames_0000(arg4);
  }
  Script_FACE(arg0, arg1, arg2, arg3);
  return Script_srchControl(0);
}

static void script_1450F(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Script_scenCreateTask(script_14525, 4, arg0, arg1, arg2, arg3);
  return;
}

static int32_t script_14525(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  if (arg3 > 0) {
    SCNF_WaitFrames_0000(arg3);
  }
  if (arg1) {
    Script_charaDisplay(arg0, CDM_Show);
  } else {
    Script_charaDisplay(arg0, CDM_Hide);
  }
  Script_scenToggleActiveFor(arg0, arg2);
  return Script_srchControl(0);
}

static void script_14561(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5, int32_t arg6) {
  Script_scenCreateTask(script_1457D, 7, arg2, arg6, arg3, arg4, arg0, arg1, arg5);
  return;
}

static int32_t script_1457D(int32_t arg0, int32_t arg1, int32_t arg2, float arg3, int32_t arg4, int32_t arg5, int32_t arg6) {
  if (arg1 > 0) {
    SCNF_WaitFrames_0000(arg1);
  }
  Script_charaToggleMotEnabled(arg4, 1);
  Script_PlayCharaMotion(arg4, arg0, arg2, -1.0f, -1.0f, 1.0f, 0);
  Script_SetCharaMotionPlaybackRate(arg4, arg3);
  if (arg6) {
    Script_motmToggleFlags(arg4, 1, 0x80);
  }
  return Script_srchControl(0);
}

static void script_145D3(void) {
  return;
}

static int32_t script_145D6(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  int32_t unused_F4[2];
  GameDateTime local_EC;
  int32_t local_E8;
  SCNF_WaitFrames_0000(5);
  Script_SetSfxOverlayTimeoutTuning(0, SfxMode_SessionAction);
  Script_uiUtils(0, 21, 0);
  script_136AF(18);
  Script_filmCheckOrSetTransitionUtil(60, 255, 255, 255, 255, 0, 0, 0, 0);
  Script_ReturnTimeDate(&local_EC);
  for (;;) {
    Script_SetNewDateTime(&local_EC);
    Script_WeatherTaskUtil(FreeTaskData);
    Script_WeatherTaskUtil(InitTaskData);
    script_145D3();
    Script_scenCreateTask(script_14D71, 2, arg0, arg1);
    Script_executeAicaCommand(25768, 0, 0);
    Script_executeAicaCommand(1184, 2, 127);
    Script_executeAicaCommand(1184, 1, 96);
    for (;;) {
      local_FC = Script_scenCreateTask(script_147F7, 2, arg0, arg1);
      task_72C = 0;
      for (;;) {
        if (task_72C >= 10) {
          Script_srchControl((void *)local_FC);
          local_E8 = task_7B0;
          if (local_E8 == 17) {
            script_1437E(5, -1);
            SCNF_WaitFrames_0000(200);
          } else {
            script_1437E(5, -1);
            SCNF_WaitFrames_0000(250);
          }
          break;
        } else {
          if (task_72C == -1) {
            Script_srchControl((void *)local_FC);
            break;
            goto L0146E8;
          }
        }
        L0146E8:;
        SCNF_WaitFrames_0000(1);
      }
      Script_executeAicaCommand(2720, 2, 30);
      Script_executeAicaCommand(1184, 1, 127);
      if (task_72C == -1) {
        Script_SetSfxOverlayTimeoutTuning(2, SfxMode_SessionAction);
        script_13715();
        if (1) {
          Script_uiUtils(0, 0, 0);
          script_14ECD(0);
          SCNF_WaitFrames_0000(1);
        } else {
          Script_filmCheckOrSetTransitionUtil(60, 0, 0, 0, 0, 255, 0, 0, 0);
          while (Script_filmCheckOrSetTransitionUtil(-1)) {
            SCNF_WaitFrames_0000(1);
          }
          break;
        }
      } else {
        Script_filmCheckOrSetTransitionUtil(60, 0, 0, 0, 0, 255, 0, 0, 0);
        while (Script_filmCheckOrSetTransitionUtil(-1)) {
          SCNF_WaitFrames_0000(1);
        }
        Script_sicnUtils(0, 1, 1);
        SCNF_WaitFrames_0000(90);
        Script_sicnUtils(2, 1, 0);
        Script_SetSfxOverlayTimeoutTuning(1, SfxMode_SessionAction);
        script_136CD();
        Script_filmCheckOrSetTransitionUtil(60, 255, 0, 0, 0, 0, 0, 0, 0);
        break;
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

__attribute__((scn_string_bindings(0, data_1B5A9, 2, data_1B5BD, 3, data_1B5C7, 5, data_1B5DB)))
static int32_t script_147F7(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  int32_t local_F8;
  local_FC = task_6F8[1];
  local_F8 = 1;
  SCNF_WaitFrames_0000(10);
  while (task_7AC == 0) {
    SCNF_WaitFrames_0000(1);
  }
  Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_IMMEDIATE);
  Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_IMMEDIATE);
  script_144D0(arg0, 2, 0, 1, 1);
  script_144D0(arg0, 1, 0, 1, 1);
  script_1437E(2, -1);
  script_12D5A(arg1, 0, 1, 0x10000000);
  if (task_720 == 0) {
    script_1437E(4, -1);
    script_14345((int32_t)"E1062B003", 100);
    script_144D0(arg0, 4, 0, 8, 23);
    script_142D0(8702, SHEN_NAME('DUMA'), SHEN_NAME('DUMA'), -1);
    script_142D0(8707, SHEN_NAME('DUMC'), SHEN_NAME('DUMC'), 30);
    script_142D0(8711, SHEN_NAME('DUMA'), SHEN_NAME('DUMA'), 60);
    script_1430A(0x3E4CCCCD, 0x3E4CCCCD, 0x3F333333, 115);
    script_13227(local_FC, arg0, 0x3E113C68, -0x3FB6CAF7, 0x43340000, -1);
    script_13227(local_FC, arg0, 0x3FF5D495, -0x41167382, 0x42B40000, 60);
    task_72C = 10;
    SCNF_WaitFrames_0000(1);
  } else {
    script_1437E(3, -1);
    script_142D0(8700, SHEN_NAME('DUMA'), SHEN_NAME('DUMA'), -1);
    script_13227(local_FC, arg0, 0x3E113C68, -0x3FB6CAF7, 0x43340000, -1);
  }
  SCNF_WaitFrames_0000(33);
  script_1437E(6, -1);
  script_142D0(8701, SHEN_NAME('DUMA'), SHEN_NAME('DUMA'), -1);
  script_12D5A(arg1, 1, 1, 0x10000000);
  if (task_720 == 0) {
    script_1437E(8, -1);
    script_14345((int32_t)"E1062B009", 150);
    script_144D0(arg0, 4, 0, 8, 28);
    script_142D0(8708, SHEN_NAME('DUMC'), SHEN_NAME('DUMC'), 30);
    script_142D0(0x2200, SHEN_NAME('DUMC'), SHEN_NAME('DUMC'), 81);
    script_142D0(8713, SHEN_NAME('DUMA'), SHEN_NAME('DUMA'), 141);
    script_13227(local_FC, arg0, -0x3FD76998, -0x411BDA51, 0x42B40000, -1);
    script_13227(local_FC, arg0, -0x427DAA50, -0x3FD6BEB6, 0x433E8ADB, 81);
    script_133FA(arg0, 0x42B40000, -1);
    task_72C = 10;
    SCNF_WaitFrames_0000(1);
  } else {
    script_1437E(7, -1);
    script_14345((int32_t)"E1062B003", -1);
    script_142D0(8701, SHEN_NAME('DUMA'), SHEN_NAME('DUMA'), -1);
    script_13227(local_FC, arg0, -0x3FE3714C, 0x3C14C448, 0x42B40000, -1);
    script_133FA(arg0, 0x42B40000, -1);
    SCNF_WaitFrames_0000(14);
  }
  SCNF_WaitFrames_0000(33);
  script_1437E(9, -1);
  script_142D0(8705, SHEN_NAME('DUMA'), SHEN_NAME('DUMA'), -1);
  script_12D5A(arg1, 2, 1, 0x10000000);
  if (task_720 == 0) {
    script_1437E(11, -1);
    script_14345((int32_t)"E1062B005", 180);
    script_144D0(arg0, 4, 0, 8, 40);
    script_142D0(8706, SHEN_NAME('DUMA'), SHEN_NAME('DUMA'), -1);
    script_142D0(8711, SHEN_NAME('DUMA'), SHEN_NAME('DUMA'), 75);
    script_1430A(0x3E4CCCCD, 0x3E4CCCCD, 0x3F333333, 120);
    script_13227(local_FC, arg0, 0x3EB6EEB7, -0x3FF147AE, 0x43340000, -1);
    script_13227(local_FC, arg0, 0x3FF4AFF7, 0x3F0DCC64, 0x42B40000, 75);
    script_133FA(arg0, 0x42B40000, -1);
    script_133FA(arg0, 0x42B40000, 75);
    task_72C = 10;
    SCNF_WaitFrames_0000(1);
  } else {
    script_1437E(10, -1);
    script_14345((int32_t)"E1062B008", -1);
    script_142D0(8706, SHEN_NAME('DUMA'), SHEN_NAME('DUMA'), -1);
    script_13227(local_FC, arg0, 0x3E669AD4, -0x3FFF736D, 0x4339B333, -1);
    script_133FA(arg0, 0x42B40000, -1);
    SCNF_WaitFrames_0000(13);
  }
  SCNF_WaitFrames_0000(35);
  script_1437E(15, -1);
  script_142D0(8709, SHEN_NAME('DUMA'), SHEN_NAME('DUMA'), -1);
  script_12D5A(arg1, 3, 1, 0x10000000);
  if (task_720 == 0) {
    script_1437E(17, -1);
    script_14345((int32_t)"E1062B005", 180);
    script_144D0(arg0, 7, 0, 16, 15);
    script_144D0(arg0, 4, 0, 8, 60);
    script_142D0(0x2200, SHEN_NAME('DUMC'), SHEN_NAME('DUMC'), -1);
    script_142D0(8713, SHEN_NAME('DUMA'), SHEN_NAME('DUMA'), 60);
    script_13227(local_FC, arg0, -0x427DAA50, -0x3FD6BEB6, 0x433E8ADB, -1);
    SCNF_WaitFrames_0000(60);
    task_72C = 10;
    SCNF_WaitFrames_0000(1);
    goto L014D61;
  }
  L014D61:;
  task_72C = -1;
  SCNF_WaitFrames_0000(1);
  end:;
}

static int32_t script_14D71(int32_t arg0, int32_t arg1) {
  Script_charaToggleMotEnabled(CHARA_AKIR, 0);
  Script_charaToggleMotEnabled(CHARA_CHAI, 0);
  script_144D0(arg0, 2, 0, 1, 1);
  script_144D0(arg0, 4, 0, 1, 1);
  script_13869(1);
  Script_charaToggleMotEnabled(CHARA_AKIR, 1);
  Script_charaToggleMotEnabled(CHARA_CHAI, 1);
  return Script_srchControl(0);
}

static void script_14DD1(int32_t arg0) {
  for (;;) {
    if (task_6BC == 2) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  return;
}

__attribute__((scn_string_bindings(1, data_1B5F0, 2, data_1B5F7)))
static void script_14DEC(int32_t arg0) {
  task_6BC = 0;
  task_6C0 = Script_motiLoad(3, SHEN_NAME('MC5Q'), "M_MC5Q.BIN");
  Script_LoadVoiceFile(1, "A0184D");
  Script_LoadVoiceFile(1, "E1062");
  return;
}

__attribute__((scn_string_bindings(2, data_1B614)))
static void script_14E25(int32_t arg0) {
  task_6BC = 1;
  Script_scenCreateTask(script_150D5, 5, CHARA_CHAI, 1, SHEN_NAME('DUMC'), 0, 0);
  Script_scenCreateTask(script_14F4F, 3, CHARA_AKIR, 0, SHEN_NAME('DUMA'));
  task_6B8 = Script_scenCreateTask(script_145D6, 2, CHARA_AKIR, 0);
  Script_LoadSND(0, 0, "system1.snd", 0);
  Script_LoadSND(0, 5, "qtebat.snd", 0);
  Script_LoadSND(0, 2, "bgm067.snd", 0);
  Script_SetCameraFlags(0);
  Script_camSetRotationStepSize(0);
  task_6B4 = Script_SetMaxNPCsNear(32);
  script_14DD1(arg0);
  return;
}

__attribute__((scn_string_bindings(0, data_1B61F, 1, data_1B626)))
static void script_14ECD(int32_t arg0) {
  task_6BC = 2;
  Script_SetMaxNPCsNear(task_6B4);
  Script_LoadVoiceFile(0, "A0184D");
  Script_LoadVoiceFile(0, "E1062");
  Script_PlayCharaMotion(CHARA_AKIR, 0xFFFF, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_PlayCharaMotion(CHARA_CHAI, 0xFFFF, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_ReallocateUNFO(*&task_6C0);
  Script_srchControl((void *)task_6B8);
  return;
}

static int32_t script_14F4F(int32_t arg0, int32_t arg1, int32_t arg2) {
  int32_t local_FC;
  int32_t local_F8;
  local_FC = Script_scenCreateTask(script_12F60, 3, arg0, arg1, 0);
  local_F8 = Script_scenCreateTask(script_12ACC, 6, arg0, arg1, 0, 2, &data_1B470, script_13B21);
  Script_FaceEx(arg0, 1, 2, 4);
  Script_ResetHandMotion(arg0, 1, 0, 0);
  Script_ResetHandMotion(arg0, 1, 1, 0);
  Script_FACE(arg0, 2, 0, 4);
  Script_FACE_LCG(arg0, 2, 4, 0);
  Script_FaceLookAtTargetUtils(arg0, 0);
  Script_ryoSetChara(arg0);
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_ccowUtils(arg0, 2);
  Script_motmGetCharaFlags(arg0, 0, 0);
  Script_charaToggleMotEnabled(arg0, 1);
  Script_motmToggleFlags(arg0, 1, 0x40);
  Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_7);
  Script_SetFightProgramStateBytes(arg0, 2, 1);
  Script_SetCPAMFlags(arg0, 0);
  Script_MoveObjectToObject(arg0, 0x304D5544);
  Script_MoveObjectToObject(arg2, arg0);
  Script_EVT_SetPosition(arg2, POS_XYZ, &data_1B62C);
  Script_EVT_SetRotation(arg2, 0x38000000, &data_1B638, 0);
  Script_SetShadowFlags(arg0, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_scenToggleActiveFor(arg0, 1);
  Script_SetCharaFlags(arg0, 4);
  Script_charaDisplay(arg0, CDM_Show);
  for (;;) {
    if (task_6BC == 2) {
      Script_ResetHandMotion(arg0, 0, 0, 0);
      Script_ResetHandMotion(arg0, 0, 1, 0);
      Script_FaceEx(arg0, 0, 0, 0);
      Script_srchControl((void *)task_710[arg1]);
      Script_srchControl((void *)local_F8);
      Script_srchControl((void *)local_FC);
      Script_srchControl(0);
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_150D5(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  int32_t local_FC;
  int32_t local_F8;
  local_FC = Script_scenCreateTask(script_12F60, 3, arg0, arg1, 0);
  local_F8 = Script_scenCreateTask(script_12ACC, 6, arg0, arg1, arg3, arg4, &data_1B470, script_13B21);
  Script_FaceEx(arg0, 1, 1, 4);
  Script_ResetHandMotion(arg0, 1, 0, 0);
  Script_ResetHandMotion(arg0, 1, 1, 0);
  Script_FACE(arg0, 1, 0, 4);
  Script_FACE_LCG(arg0, 2, 4, 0);
  Script_FaceLookAtTargetUtils(arg0, 0);
  Script_ccowUtils(arg0, 2);
  Script_motmGetCharaFlags(arg0, 0, 0);
  Script_charaToggleMotEnabled(arg0, 1);
  Script_motmToggleFlags(arg0, 1, 0x40);
  Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_7);
  Script_SetFightProgramStateBytes(arg0, 2, 1);
  Script_SetCPAMFlags(arg0, 0);
  Script_MoveObjectToObject(arg0, 0x304D5544);
  Script_MoveObjectToObject(arg2, arg0);
  Script_EVT_SetPosition(arg2, POS_XYZ, &data_1B644);
  Script_EVT_SetRotation(arg2, 0x38000000, &data_1B650, 0);
  Script_SetShadowFlags(arg0, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 1);
  Script_SetShadowFlags(arg0, SCRIPT_SHADOW_UPDATE_LIGHT_MASK, 5);
  Script_scenToggleActiveFor(arg0, 1);
  Script_SetCharaFlags(arg0, 4);
  Script_charaDisplay(arg0, CDM_Show);
  for (;;) {
    if (task_6BC == 2) {
      Script_ResetHandMotion(arg0, 0, 0, 0);
      Script_ResetHandMotion(arg0, 0, 1, 0);
      Script_FaceEx(arg0, 0, 0, 0);
      Script_srchControl((void *)task_710[arg1]);
      Script_srchControl((void *)local_F8);
      Script_srchControl((void *)local_FC);
      Script_srchControl(0);
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}
