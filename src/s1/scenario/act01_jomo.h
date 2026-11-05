#pragma once

#include "scenario.h"
#include "motion/motion.h"

static void script_0030(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_03C6(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_0A54(void);
static void script_0A5D(void);
static void script_0A6C(int32_t arg0);
static void script_0AF9(int32_t arg0);
static void script_0B49(void);
static void script_0B77(int32_t arg0);
static void script_0BA5(void);
static void script_0BD9(int32_t arg0);
static void script_0C0D(int32_t arg0);
static void script_0C2B(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_0C65(int32_t arg0, int32_t arg1);
static void script_0C99(void);
static void script_0D91(int32_t arg0, int32_t arg1);
static int32_t script_0DC1(int32_t arg0);
static void script_0DDF(float arg0);
static int32_t script_0DFE(int32_t arg0, int32_t arg1);
static void script_0E76(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4);
static int32_t script_1428(int32_t arg0);
static void script_14F9(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_1509(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_160D(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_17AF(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_1910(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_1A71(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_1D0F(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_1DDD(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_1E44(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_1EAB(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_1EF2(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_2028(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_28E9(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_2A07(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_30CC(int32_t arg0, int32_t arg1);
static void script_31A2(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_40B5(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_4121(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_45DB(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_4C95(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_6575(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_6C96(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_7420(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_74F8(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_796F(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_8902(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_8D4C(int32_t arg0, DSWKDoorEntry *arg1, int32_t *arg2, DoorResourceDescriptor *arg3, MapObjectRecord *arg4, int32_t arg5, int32_t arg6);
static void script_8ED8(DSWKDoorEntry *arg0, int32_t *arg1, DoorResourceDescriptor *arg2, MapObjectRecord *arg3, int32_t arg4);
static void script_8F0C(void);
static void script_8F75(int32_t arg0, int32_t arg1);
static void script_9012(int32_t arg0, int32_t arg1);
static int32_t script_904C(void);
static void script_90AF(void);
static void script_9112(int32_t arg0);
static void script_9149(int32_t arg0);
static void script_9180(int32_t arg0);
static void script_93C2(int32_t arg0);
static void script_940B(int32_t arg0);
static void script_9454(int32_t arg0, int32_t arg1);
static void script_94CA(int32_t arg0);
static void script_94D8(int32_t arg0);
static void script_94E6(int32_t arg0);
static void script_95AD(void);
static void script_95B9(void);
static void script_95C5(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5, int32_t arg6);
static int32_t script_9608(void);
static int8_t script_9719(int32_t arg0, int32_t arg1, int8_t arg2);
static void script_97B1(int32_t arg0);
static void script_9980(int32_t arg0);
static int32_t script_99B1(int32_t arg0, int32_t arg1, int32_t arg2, float arg3, float arg4, int32_t arg5, int32_t arg6);
static void script_A07A(int32_t arg0, int32_t arg1);
static void script_A0AC(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_A21C(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_A3D8(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_A58C(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_A7A2(int32_t arg0, int32_t arg1);
static void script_A86C(int32_t arg0, int32_t arg1);
static void script_A8CD(int32_t arg0);
static void script_AA72(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_AB0A(int32_t arg0, int32_t arg1);
static void script_ABEC(int32_t arg0);
static void script_AC43(int32_t arg0, float arg1, float arg2, float arg3);
static void script_AC6B(int32_t arg0, float arg1, float arg2, float arg3);
static void script_ACBF(int32_t arg0, int32_t arg1, int32_t arg2);
static int32_t script_AD0F(int32_t arg0);
static void script_ADB6(int32_t arg0, int32_t arg1);
static int32_t script_ADD9(int32_t arg0);
static void script_AE0C(int32_t arg0, int32_t arg1);
static void script_AE2F(int32_t arg0, int32_t arg1);
static void script_AE8E(int32_t arg0);
static void script_AEA3(void);
static void script_AEB2(void);
static void script_AEC1(void);
static void script_AEC4(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_AEC7(void);
static void script_AECA(void);
static void script_AECD(void);
static void script_AEDA(void);
static void script_AEE6(void);
static int32_t script_AEF5(void);
static void script_AF04(void);
static void script_B03C(int32_t arg0);
static void script_B048(int32_t arg0);
static void script_B054(int32_t arg0);
static void script_B130(int32_t arg0);
static int32_t script_B147(int32_t arg0);
static int32_t script_B188(int32_t arg0);
static void script_B1CC(int32_t arg0);
static void script_B24A(void);
static void script_B2C9(int32_t arg0);
static void script_B3C6(int32_t arg0);
static int32_t script_B573(void);
static int32_t script_B5AE(void);
static int32_t script_B5F2(float arg0, int32_t arg1);
static void script_B616(int32_t arg0);
static void script_B68A(int32_t arg0);
static void script_B6FA(void);
static void script_B704(void);
static void script_B70E(void);
static void script_B718(void);
static void script_B722(void);
static void script_B81E(void);
static void script_B9B1(int32_t arg0);
static int32_t script_BA11(void);
static void script_BA38(void);
static int32_t script_BA69(int32_t arg0);
static void script_BAED(int32_t arg0, int32_t arg1);
static void script_BB71(int32_t arg0);
static void script_BBA0(int32_t arg0, int32_t arg1);
static void script_BBFF(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static int32_t script_BC26(float arg0, float arg1, float arg2, float arg3);
static int32_t script_BDC9(void);
static void script_BE90(const char *arg0, int32_t arg1);
static void script_BEDD(void);
static int32_t script_C014(int32_t arg0);
static void script_C08D(void);
static void script_C0EE(int32_t arg0);
static void script_C119(void);
static int32_t script_C140(int32_t arg0);
static int8_t script_C182(int32_t arg0);
static void script_C25A(void);
static void script_C267(void);
static void script_C5F5(void);
static void script_D13C(void);
static void script_D1FA(void);
static void script_D21F(void);
static void script_D966(void);
static int8_t script_F9ED(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int8_t script_FAA6(void);
static void script_FC5C(int32_t arg0);
static int8_t script_10DCD(void);
static void script_1131C(int32_t arg0);
static void script_151DA(void);
static int8_t script_15CA2(void);
static int8_t script_15CCA(void);
static void script_15DE5(int32_t arg0);
static int32_t script_15ECE(int32_t arg0, int32_t arg1);
static int32_t script_15F37(void);
static void script_15F52(int32_t arg0);
static void script_1603B(int32_t arg0);
static void script_16229(void);
static void script_16260(int32_t arg0);
static int8_t script_16328(void);
static int32_t script_16FF7(void);
static void script_170DD(void);
static void script_17176(void);
static void script_1720C(void);
static void script_1738F(void);
static void script_173A3(void);
static void script_173EA(void);
static int32_t script_17568(void);
static int32_t script_175EB(void);
static void script_17CC6(void);
static void script_17D28(void);
static void script_17D3A(void);
static void script_17D3D(void);
static void script_17D64(void);
static void script_17D88(void *arg0, int32_t arg1);
static void script_17DC1(void);
static void script_1812E(int32_t arg0);
static int32_t script_182B8(void);
static int8_t script_188BC(void);
static int8_t script_189A3(void);
static int8_t script_18AA2(void);
static int8_t script_18B85(void);
static int8_t script_18C19(void);
static int8_t script_18CF1(void);
static int8_t script_18E5B(void);
static int32_t script_18FD7(void);
static int32_t script_18FEF(void);
static void script_18FF7(int32_t arg0);
static int32_t script_19673(int32_t arg0, int32_t arg1);
static int32_t script_196C0(int32_t arg0, int32_t arg1);
static int32_t script_19770(void);
static void script_19F1C(void);
static void script_1A794(int32_t arg0);
static void script_1A941(void);
static void script_1A96C(void);
static void script_1A98D(void);
static void script_1A9D0(void);
static void script_1AA0A(void);
static void script_1AA4B(void);
static void script_1AA6D(void);
static void script_1AAD5(void);
static void script_1AAF0(void);
static void script_1AB63(void);
static void script_1AB8A(void);
static void script_1ABDD(void);
static void script_1ABF8(int32_t arg0);
static void script_1AC4D(int32_t arg0);
static void script_1ACF0(int32_t arg0);
static void script_1ACFB(int32_t arg0);
static int32_t script_1AD52(void);
static int32_t script_1AF87(void);
static int32_t script_1B3C4(void);
static int32_t script_1B719(void);
static int32_t script_1B874(void);
static int32_t script_1B953(void);
static void script_1BA32(void);
static void script_1BA92(void);
static int32_t script_1BB86(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_1BBF2(int32_t arg0, int32_t arg1);
static int32_t script_1BC5E(int32_t arg0, int32_t arg1, float arg2);
static int32_t script_1BCCD(int32_t arg0, int32_t arg1, int32_t arg2);
static int32_t script_1BD3C(int32_t arg0, int32_t arg1);
static int32_t script_1BDAB(int32_t arg0, int32_t arg1, float arg2);
static int32_t script_1BE14(int32_t arg0);
static void script_1C598(int32_t arg0);
static void script_1CBA2(void);
static void script_1CBC6(void);
static void script_1CBEA(void);
static void script_1CC0E(void);
static void script_1CC32(void);
static void script_1CC56(void);
static void script_1CC7A(int32_t arg0);
static void script_1CD24(int32_t arg0);
static void script_1CE23(void);
static void script_1CE30(void);
static int32_t script_1CE56(void);
static int32_t script_1CEE4(void);
static int32_t script_1CFA6(void);
static void script_1D050(void);
static void script_1D071(void);
static void script_1D088(int32_t arg0);
static void script_1D2D4(void);
static void script_1D304(void);
static int32_t script_1D334(void);
static int32_t script_1D3C3(int32_t arg0, int32_t arg1);
static void script_1DBD6(int32_t arg0);
static void script_1DCD5(void);
static void script_1DD40(void);
static void script_1E3BD(int32_t arg0);
static void script_1E715(int32_t arg0);
static void script_1EC85(void);
static void script_1ECDE(void);
static void script_1EE2C(int32_t arg0);
static void script_1EE2F(void);
static void script_1EE4A(int32_t arg0);
static void script_1EE66(int32_t arg0);
static void script_1EEC0(int32_t arg0);
static void script_1EF9D(int32_t arg0);
static void script_1F037(void);
static void script_1F046(int32_t arg0);
static void script_1F051(int32_t arg0);
static void script_1F0A3(int32_t arg0);
static int32_t script_1F139(void);
static int32_t script_1F17E(void);
static int32_t script_1F21E(void);
static void script_1F277(void);
static void script_1FA95(int32_t arg0);
static void script_1FB80(int32_t arg0);
static void script_1FCEB(void);
static int32_t script_1FD2E(void);
static void script_1FD9A(int32_t arg0);
static void script_1FF06(int32_t arg0);
static int32_t script_209F5(int32_t arg0, int32_t arg1, int32_t arg2);
static int32_t script_20CE4(int32_t arg0);
static void script_20EAE(int32_t arg0, int32_t arg1, float arg2);
static void script_20F3E(int32_t arg0, int32_t arg1);
static int32_t script_2112E(int32_t arg0, int32_t arg1);
static void script_21310(void);
static int32_t script_21730(void);
static int32_t script_219AF(int32_t arg0, int32_t arg1);
static void script_219C4(void);
static int32_t script_21A4B(void);
static void script_21A9F(void);
static int32_t script_21F1E(void);
static void script_21F36(void);
static int32_t script_22219(void);
static void script_22231(void);
static void script_22519(void);
static void script_2271E(int32_t arg0, int32_t arg1);
static void script_2299A(void);
static void script_22A9A(void);
static int32_t script_22B3F(void);
static int32_t script_22D2D(void);
static void script_22E62(void);
static int32_t script_22FA4(int32_t arg0);
static int32_t script_23194(void);
static int32_t script_233A1(void);
static int32_t script_23604(void);
static int32_t script_2394A(void);
static int32_t script_23BB7(void);
static int32_t script_23F0D(void);
static void script_24170(void);
static int32_t script_24389(void);
static int32_t script_24647(void);
static int32_t script_249B2(void);
static int32_t script_24B21(void);
static void script_24BA2(void);
static void script_24E66(int32_t arg0);
static void script_24ED9(void);
static void script_24EE2(void);
static void script_25309(float arg0, float arg1, float arg2, float arg3, float arg4, float arg5, int32_t arg6);
static void script_253B3(float arg0, float arg1, float arg2, float arg3, float arg4, float arg5, int32_t arg6);
static void script_254D2(void);
static void script_255CA(void);
static void script_25699(void);
static int32_t script_257A9(void);
static int32_t script_25814(void);
static int32_t script_2587F(void);
static int32_t script_2593A(void);
static int32_t script_259E8(void);
static void script_25A71(void);
static int32_t script_25BEF(void);
static void script_25D61(void);
static void script_25EAB(void);
static void script_26DDA(void);
static int32_t script_26EE3(void);
static void script_27213(void);
static void script_2733E(void);
static void script_2735C(void);
static int32_t script_2736B(void);
static void script_27530(void);
static void script_2759E(void);
static void script_2760C(void);
static int32_t script_278BB(void);
static void script_27FEE(void);
static void script_280C8(void);
static void script_281A2(void);
static void script_28257(void);
static void script_28316(void);
static void script_283D2(void);
static int32_t script_2848F(void);
static void script_28781(void);
static void script_28A2F(void);
static void script_28EB5(void);
static int32_t script_29408(void);
static int32_t script_2954E(void);
static int32_t script_29622(void);
static int32_t script_29860(void);
static void script_2997B(void);
static void script_299B4(void);
static void script_299CF(void);
static void script_29A6B(void);
static void script_29B73(void);
static void script_29E12(void);
static void script_29EEB(void);
static int32_t script_29F42(void);
static void script_2A30F(void);
static int32_t script_2AB47(void);
static void script_2AC62(void);
static void script_2AD77(void);
static void script_2ADAD(void);
static int32_t script_2ADD4(void);
static void script_2AF61(void);
static void script_2B20C(int32_t arg0);
static void script_2B3CB(void);
static void script_2B439(void);
static int32_t script_2B46C(void);
static void script_2B85E(void);
static void script_2B965(void);
static void script_2B98F(void);
static int32_t script_2B99E(void);
static void script_2BC8E(void);
static void script_2BE53(void);
static void script_2BE9E(void);
static void script_2BF4E(void);
static int32_t script_2C1DF(void);
static void script_2CB82(void);
static int32_t script_2CD6D(void);
static void script_2CE2C(void);
static void script_2CFDD(void);
static void script_2D007(void);
static int32_t script_2D022(void);
static void script_2D320(void);
static void script_2D555(void);
static void script_2D5BD(void);
static int32_t script_2D5FC(void);
static void script_2DB21(void);
static void script_2DC64(void);
static void script_2DCED(void);
static int32_t script_2DD20(void);
static void script_2DDCF(void);
static int32_t script_2DFC5(void);
static void script_2E531(void);
static void script_2E641(void);
static void script_2E6A3(void);
static int32_t script_2E716(void);
static void script_2E95A(void);
static void script_2EB4B(void);
static void script_2EB90(void);
static int32_t script_2EBBD(void);
static void script_2EF75(void);
static int32_t script_300EF(void);
static void script_30547(void);
static int32_t script_316EF(void);
static void script_31844(int32_t arg0);
static int32_t script_31A2A(void);
static int32_t script_31C29(void);
static void script_31E16(void);
static void script_31FC2(void);
static void script_32231(void);
static void script_3245D(void);
static int32_t script_32689(int32_t arg0);
static int32_t script_32736(int32_t arg0);
static int32_t script_32792(void);
static void script_327BD(void);
static int32_t script_32A20(void);
static int32_t script_32EF6(void);
static int32_t script_32FD6(void);
static void script_32FE7(void);
static int32_t script_33238(void);
static int32_t script_334FF(void);
static void script_335ED(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_3378A(void);
static void script_338AA(void);
static void script_33B04(int32_t arg0);
static void script_33C60(int32_t arg0, int32_t arg1);
static void script_346C3(int32_t arg0);
static int32_t script_34A98(int32_t arg0);
static void script_34AFD(int32_t arg0);
static void script_34C74(void);
static void script_34C7E(void);
static void script_34C88(void);
static void script_34C92(int32_t arg0);
static void script_34C9C(int32_t arg0);
static int32_t script_34E42(void);
static void script_34EBA(void);
static void script_34F68(int32_t arg0);
static int32_t script_34FBA(void);
static void script_35092(int32_t arg0);
static void script_36153(int32_t arg0, int32_t arg1, float arg2, float arg3, float arg4);
static void script_361BB(void);
static void script_3626D(void);
static void script_36877(void);
static void script_368F3(void);
static void script_3696F(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_36AD9(void);
static int32_t script_36AF8(void);
static void script_36BD8(int32_t arg0);
static int32_t script_36C9E(void);
void script_36CFA(void);
static void script_37213(void);
static int32_t script_37474(void);
static void script_374A5(void);
static void script_374FE(void);
static void script_37564(void);
static void script_375CA(void);
static void script_37630(void);
static void script_37691(void);
static int8_t script_376ED(void);
static int8_t script_377A0(void);
static int8_t script_3782B(void);
static void script_378CC(void);
static int8_t script_37929(void);
static void script_379BD(void);
static int8_t script_37A7D(void);
static int8_t script_37BC3(void);
static int8_t script_37C8F(void);
static int8_t script_37D71(void);
static int8_t script_37E44(void);
static void script_37F0C(int32_t arg0);
static int32_t script_381AE(void);
static void script_39C36(void);

static int32_t task_0;
static int32_t task_4;
static int32_t task_8;
static int8_t task_C;
static int8_t task_D;
static int8_t task_E;
static int8_t task_F;
static int8_t task_10;
static uint8_t task_11;
static uint16_t task_12;
static int8_t task_14;
static uint8_t task_15;
static uint16_t task_16;
static int32_t task_18;
static int8_t task_1C;
static int8_t task_1D;
static int8_t task_1E;
static uint8_t task_1F;
static int32_t task_20;
static int8_t task_24;
static int8_t task_25;
static int8_t task_26;
static int8_t task_27;
static int8_t task_28;
static int8_t task_29;
static int8_t task_2A;
static int8_t task_2B;
static int8_t task_2C;
static uint8_t task_2D;
static uint16_t task_2E;
static int8_t task_30;
static uint8_t task_31;
static uint16_t task_32;
static int32_t task_34;
static GameDateTime task_38;
static int32_t task_40;
static int32_t task_44;
static int8_t task_48[5][24];
static DSWKDoorEntry *task_C0;
static int32_t *task_C4;
static DoorResourceDescriptor *task_C8;
static MapObjectRecord *task_CC;
static DoorSlotState task_D0;
static int32_t task_DC;
static int32_t task_E0[10];
static int32_t task_108[10];
static int32_t task_130[10];
static int32_t task_158[10];
static int32_t task_180[10];
static int32_t task_1A8[10];
static int32_t task_1D0[10];
static int32_t task_1F8[10];
static int32_t task_220;
static int32_t task_224;
static Vector3 task_228;
static int32_t task_234;
static int32_t task_238;
static int32_t task_23C;
static int8_t task_240;
static uint8_t task_241;
static uint16_t task_242;
static int8_t task_244;
static uint8_t task_245;
static uint16_t task_246;
static int32_t task_248;
static int32_t task_24C;
static int8_t task_250;
static uint8_t task_251;
static uint16_t task_252;
static Vector3 task_254;
static int32_t task_260;
static int32_t task_264;
static int32_t task_268;
static int8_t task_26C;
static uint8_t task_26D;
static uint16_t task_26E;
static int32_t task_270;
static int32_t task_274;
static int32_t task_278;
static int32_t task_27C;
static int32_t task_280;
static int32_t task_284;
static int32_t task_288;
static int32_t task_28C;
static int32_t task_290;
static int32_t task_294;
static int32_t task_298;
static int32_t task_29C;
static int8_t task_2A0;
static int8_t task_2A1;
static int8_t task_2A2;
static int8_t task_2A3;
static int8_t task_2A4;
static uint8_t task_2A5;
static uint16_t task_2A6;
static int8_t task_2A8;
static uint8_t task_2A9;
static uint16_t task_2AA;
static int32_t task_2AC[23];
static int8_t task_308;
static int8_t task_309;
static int8_t task_30A;
static int8_t task_30B;
static int32_t task_30C[4];
static int32_t task_31C;
static int32_t task_320;
static int32_t task_324;
static int32_t task_328;
static int8_t task_32C;
static uint8_t task_32D;
static uint16_t task_32E;
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
static int8_t task_364;
static uint8_t task_365;
static uint16_t task_366;
static int8_t task_368;
static uint8_t task_369;
static uint16_t task_36A;
static int32_t task_36C;
static int32_t task_370;
static int8_t task_374;
static uint8_t task_375;
static uint16_t task_376;
static Vector3 task_378;
static int32_t task_384;
static int32_t task_388;
static int32_t task_38C;
static Vector3 task_390;
static int8_t task_39C;
static uint8_t task_39D;
static uint16_t task_39E;
static int8_t task_3A0;
static uint8_t task_3A1;
static uint16_t task_3A2;
static Vector3 task_3A4;
static Vector3i task_3B0;
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
static int8_t task_3E8;
static uint8_t task_3E9;
static uint16_t task_3EA;
static int8_t task_3EC;
static uint8_t task_3ED;
static uint16_t task_3EE;
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
static int32_t task_440[100];
static int32_t task_5D0;
static int32_t task_5D4;
static int32_t task_5D8;
static int32_t task_5DC;
static int32_t task_5E0;
static int32_t task_5E4;
static int32_t task_5E8;
static int32_t task_5EC;
static int32_t task_5F0;
static int32_t task_5F4;
static int32_t task_5F8;
static int32_t task_5FC;
static int32_t task_600;
static int32_t task_604;
static int32_t task_608;
static const float data_39C3C[3] = {1.0f, 1.0f, 1.0f};
static const int32_t data_39C48[4] = {16777215, 16777215, 16777215, 16777215};
static const float data_39C58[3] = {20.0f, 800.0f, 1.0f};
static const int32_t data_39C64[742] = {
  -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -10922, 0, 0, -10558, 0, 1456, 0, 0, 0, -18022, 0, 0, -17112, 0, 0, -18386, 0, 0, 0, 0, 0, -12196, 0, 0, -18204, 0,
  0, -16748, 0, 0, 0, 0, 0, -11832, 0, 0, -18386, 0, 0, -15109, 0, 0, 0, 0, 910, -12014, 0, -2184, -16748, 0, -2184, -10012, 0, 0, 0, 0, -364, -11650,
  -2912, -364, -4369, 1638, 910, 182, 0, 0, -16019, 0, 182, -23119, 0, 546, -18932, 0, 0, -1092, 0, 0, -16384, 0, 0, -21845, 0, 0, -18750, 0, 0, -546,
  0, 0, -21481, 0, 0, -21299, 0, 0, -18568, 0, 0, -728, 0, 910, -18750, 0, 182, -18750, 0, -4004, -18204, 0, 0, -728, 0, -364, -14017, -2912, -364,
  -6735, 1638, 910, 2548, 0, 0, -18568, 0, 182, -25668, 0, 546, -21481, 0, 0, -3640, 0, 0, -18932, 0, 0, -24393, 0, 0, -21299, 0, 0, -3094, 0, 0,
  -24029, 0, 0, -23847, 0, 0, -21117, 0, 0, -3276, 0, 910, -21299, 0, 182, -21299, 0, -4004, -20753, 0, 0, -3276, 0, 0, -5461, 0, -2366, -1092, 0,
  -182, 1092, 0, -546, -6735, 0, 0, -4004, 0, 0, -2912, 0, -182, 1274, 0, 0, -13289, 0, 0, -5279, 0, 0, -8009, 0, 0, 1456, 0, 182, -8738, 0, 0, -6007,
  0, 0, -8738, 0, 182, 1638, 0, -728, -5279, 0, -1274, -6735, 0, -910, -8738, 0, 0, 1638, 0, 0, -5461, 0, -2366, -1092, 0, -182, 1092, 0, -546,
  -13289, 0, 0, -5643, 0, 0, -8009, 0, -182, 1274, 0, 0, -11832, 0, 0, -5279, 0, 0, -8009, 0, 0, 1456, 0, 182, -8738, 0, 0, -6007, 0, 0, -8738, 0,
  182, 1638, 0, -728, -5279, 0, -1274, -6735, 0, -910, -8738, 0, 0, 1092, 0, 0, -5461, 0, -2366, -1092, 0, -182, 1092, 0, -546, -3640, 0, 0, -2366, 0,
  0, -1456, 0, -182, 1274, 0, 0, -3822, 0, 0, -2730, 0, 0, -1456, 0, 0, 1456, 0, 182, -3822, 0, 0, -2912, 0, 0, -1638, 0, 182, 1638, 0, -728, -3640,
  0, -910, -2730, 0, -546, -1638, 0, 0, 1092, 0, 0, -5461, 0, -2366, -1092, 0, -182, 1092, 0, -546, -6735, 0, 0, -4004, 0, 0, -6553, 0, -182, 1274, 0,
  0, -13289, 0, 0, -5279, 0, 0, -8009, 0, 0, 1456, 0, 182, -8738, 0, 0, -6007, 0, 0, -8738, 0, 182, 1638, 0, -728, -5279, 0, -1274, -6735, 0, -910,
  -8738, 0, 0, 1638, 0, 0, -9102, 0, -2366, -2002, 0, -182, 910, 0, -546, -2366, 0, 0, -2002, 0, 0, -2912, 0, -182, 1274, 0, 0, -14199, 0, 0, -11468,
  0, 0, -13653, 0, 0, 1456, 0, 182, -11286, 0, 0, -12014, 0, 0, -12743, 0, 182, 1638, 0, -728, -10194, 0, -1274, -12196, 0, -910, -10922, -728, 546,
  1638, 0, 0, -1820, 0, -2366, -728, 0, -364, -1092, 0, -546, -11832, 0, 910, -6735, 0, 0, -6553, 0, -182, 1274, 0, 0, -13289, 0, 0, -6007, 0, 0,
  -10012, 0, 0, 1092, 0, 182, -8738, 0, 0, -6735, 0, 0, -9830, 0, 182, 1638, 0, -728, -6553, 0, -1274, -7281, 0, -910, -10740, 0, 0, 1638, 0, 0,
  -5461, -2912, -2366, -364, -5279, 2548, -910, 0, -546, -8374, 0, 0, -9466, 0, 910, -4915, 0, -182, 546, 0, 0, -13289, 0, 0, -5279, 0, 364, -7645, 0,
  -182, 1456, 0, 182, -8738, 0, 0, -7281, 0, 182, -6917, 0, -182, 1638, 0, -728, -5279, 0, -1274, -6735, 0, -182, -8009, 0, 0, 1638, -4733, 0, -8556,
  0, -2366, -1092, -6189, 2912, 0, 0, -546, -10922, 0, 1820, -16384, 0, 0, -11468, 0, 182, 546, 0, 0, -10376, 0, 0, -15837, 0, 0, -12743, 0, -182,
  728, 0, 182, -13835, 0, 0, -16201, 0, 364, -10922, 0, -182, 728, 0, -728, -9648, 0, -3458, -12379, 0, -728, -7463, 0, 0, 1092, 0, 0, -5461, 0,
  -2366, -1638, 0, -910, 1092, 0, -546, -3640, 0, 0, -2366, 0, 0, -1456, 0, -182, 1638, 0, 0, -910, 0, 0, -2730, 0, 0, -1274, 0, 0, 1820, 0, 182,
  -2730, 0, 0, -1820, 0, 0, -1274, 0, 182, 2184, 0, -728, -3094, 0, -910, -2548, 0, 910, -1456, -546, 910, 2366
};
static DoorMotionSelectorBlock data_3A7FC[38] = {
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
static ScnDoorMotionDescriptor data_3ADEC[72] = {
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
static uint32_t data_3B80C = 0xFFFFFFFF;
typedef struct Row_3B810 { float w0; float w1; float w2; float w3; int32_t w4; } Row_3B810;
static const Row_3B810 data_3B810[7] = {
  {0.0f, 0.0f, 0.0f, 0.0f, -1},
  {0.0f, 0.0f, 0.0f, 0.0f, -1},
  {0.0f, 0.0f, 0.0f, 0.0f, 1534},
  {26.0f, 55.0f, 45.0f, 75.0f, 1555},
  {61.0f, 146.0f, 45.0f, 50.0f, 1554},
  {59.0f, 124.0f, 45.0f, 70.0f, 1543},
  {33.0f, 49.0f, 45.0f, 50.0f, 1543}
};
static const float data_3B89C[4] = {33.0f, 49.0f, 50.0f, 50.0f};
static void (*data_3B8AC)(int32_t, int32_t, int32_t) = script_14F9;
static void (*const data_3B8B0)(int32_t, int32_t, int32_t, int32_t) = script_1509;
static void (*const data_3B8B4)(int32_t, int32_t, int32_t, int32_t) = script_1509;
static void (*const data_3B8B8)(int32_t, int32_t, int32_t, int32_t) = script_1509;
static void (*const data_3B8BC)(int32_t, int32_t, int32_t, int32_t) = script_1509;
static void (*const data_3B8C0)(int32_t, int32_t, int32_t, int32_t) = script_1509;
static void (*const data_3B8C4)(int32_t, int32_t, int32_t, int32_t) = script_1509;
static void (*const data_3B8C8)(int32_t, int32_t, int32_t, int32_t) = script_1509;
static void (*const data_3B8CC)(int32_t, int32_t, int32_t, int32_t) = script_1509;
static void (*const data_3B8D0)(int32_t, int32_t, int32_t, int32_t) = script_17AF;
static void (*const data_3B8D4)(int32_t, int32_t, int32_t, int32_t) = script_1910;
static void (*const data_3B8D8)(int32_t, int32_t, int32_t, int32_t) = script_1DDD;
static void (*const data_3B8DC)(int32_t, int32_t, int32_t, int32_t) = script_1E44;
static void (*const data_3B8E0)(int32_t, int32_t, int32_t, int32_t) = script_160D;
static void (*const data_3B8E4)(int32_t, int32_t, int32_t, int32_t) = script_160D;
static void (*const data_3B8E8)(int32_t, int32_t, int32_t, int32_t) = script_1A71;
static void (*const data_3B8EC)(int32_t, int32_t, int32_t, int32_t) = script_1D0F;
static void (*const data_3B8F0)(int32_t, int32_t, int32_t, int32_t) = script_1EAB;
static void (*const data_3B8F4)(int32_t, int32_t, int32_t, int32_t) = script_17AF;
static void (*const data_3B8F8)(int32_t, int32_t, int32_t, int32_t) = script_1910;
static void (*const data_3B8FC)(int32_t, int32_t, int32_t, int32_t) = script_1A71;
static void (*const data_3B900)(int32_t, int32_t, int32_t, int32_t) = script_17AF;
static void (*const data_3B904)(int32_t, int32_t, int32_t, int32_t) = script_1910;
static void (*const data_3B908)(int32_t, int32_t, int32_t, int32_t) = script_1A71;
static void (*const data_3B90C)(int32_t, int32_t, int32_t, int32_t) = script_1EF2;
static void (*const data_3B910)(int32_t, int32_t, int32_t, int32_t) = script_1EF2;
static void (*const data_3B914)(int32_t, int32_t, int32_t, int32_t) = script_1EF2;
static void (*const data_3B918)(int32_t, int32_t, int32_t, int32_t) = script_1EF2;
static void (*const data_3B91C)(int32_t, int32_t, int32_t, int32_t) = script_2028;
static void (*const data_3B920)(int32_t, int32_t, int32_t, int32_t) = script_28E9;
static void (*const data_3B924)(int32_t, int32_t, int32_t, int32_t) = script_1509;
static void (*const data_3B928)(int32_t, int32_t, int32_t, int32_t) = script_1509;
static void (*const data_3B92C)(int32_t, int32_t, int32_t, int32_t) = script_1509;
static void (*const data_3B930)(int32_t, int32_t, int32_t, int32_t) = script_1509;
static void (*const data_3B934)(int32_t, int32_t, int32_t, int32_t) = script_1509;
static void (*const data_3B938)(int32_t, int32_t, int32_t, int32_t) = script_1509;
static void (*const data_3B93C)(int32_t, int32_t, int32_t, int32_t) = script_1509;
static void (*const data_3B940)(int32_t, int32_t, int32_t, int32_t) = script_1509;
static const char data_3B944[46] __attribute__((scn_string_literal)) = "Door Index = %d : call_type = %d : type = %d\012";
static const char data_3B972[43] __attribute__((scn_string_literal)) = "Door Open Motion = DEB_AKESIME_FUSUMA_L_F\012";
static const char data_3B99D[43] __attribute__((scn_string_literal)) = "Door Open Motion = DEB_AKESIME_FUSUMA_R_F\012";
static const char data_3B9C8[42] __attribute__((scn_string_literal)) = "Door Open Motion = DEB_HIKISIME_DOOR_L_F\012";
static const char data_3B9F2[42] __attribute__((scn_string_literal)) = "Door Open Motion = DEB_HIKISIME_DOOR_R_F\012";
static const char data_3BA1C[41] __attribute__((scn_string_literal)) = "Door Open Motion = DEB_OSISIME_DOOR_L_F\012";
static const char data_3BA45[41] __attribute__((scn_string_literal)) = "Door Open Motion = DEB_OSISIME_DOOR_R_F\012";
static const char data_3BA6E[43] __attribute__((scn_string_literal)) = "Door Open Motion = GAK_AKESIME_FUSUMA_L_F\012";
static const char data_3BA99[43] __attribute__((scn_string_literal)) = "Door Open Motion = GAK_AKESIME_FUSUMA_R_F\012";
static const char data_3BAC4[42] __attribute__((scn_string_literal)) = "Door Open Motion = GAK_HIKISIME_DOOR_L_F\012";
static const char data_3BAEE[42] __attribute__((scn_string_literal)) = "Door Open Motion = GAK_HIKISIME_DOOR_R_F\012";
static const char data_3BB18[41] __attribute__((scn_string_literal)) = "Door Open Motion = GAK_OSISIME_DOOR_L_F\012";
static const char data_3BB41[41] __attribute__((scn_string_literal)) = "Door Open Motion = GAK_OSISIME_DOOR_R_F\012";
static const char data_3BB6A[43] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_AKESIME_FUSUMA_L_F\012";
static const char data_3BB95[43] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_AKESIME_FUSUMA_R_F\012";
static const char data_3BBC0[42] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_HIKISIME_DOOR_L_F\012";
static const char data_3BBEA[42] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_HIKISIME_DOOR_R_F\012";
static const char data_3BC14[41] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_OSISIME_DOOR_L_F\012";
static const char data_3BC3D[41] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_OSISIME_DOOR_R_F\012";
static const char data_3BC66[43] __attribute__((scn_string_literal)) = "Door Open Motion = KOD_AKESIME_FUSUMA_L_F\012";
static const char data_3BC91[43] __attribute__((scn_string_literal)) = "Door Open Motion = KOD_AKESIME_FUSUMA_R_F\012";
static const char data_3BCBC[42] __attribute__((scn_string_literal)) = "Door Open Motion = KOD_HIKISIME_DOOR_L_F\012";
static const char data_3BCE6[42] __attribute__((scn_string_literal)) = "Door Open Motion = KOD_HIKISIME_DOOR_R_F\012";
static const char data_3BD10[41] __attribute__((scn_string_literal)) = "Door Open Motion = KOD_OSISIME_DOOR_L_F\012";
static const char data_3BD39[41] __attribute__((scn_string_literal)) = "Door Open Motion = KOD_OSISIME_DOOR_R_F\012";
static const char data_3BD62[43] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_AKESIME_FUSUMA_L_F\012";
static const char data_3BD8D[43] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_AKESIME_FUSUMA_R_F\012";
static const char data_3BDB8[42] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_HIKISIME_DOOR_L_F\012";
static const char data_3BDE2[42] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_HIKISIME_DOOR_R_F\012";
static const char data_3BE0C[41] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_OSISIME_DOOR_L_F\012";
static const char data_3BE35[41] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_OSISIME_DOOR_R_F\012";
static const char data_3BE5E[43] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_AKESIME_FUSUMA_L_F\012";
static const char data_3BE89[43] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_AKESIME_FUSUMA_R_F\012";
static const char data_3BEB4[42] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_HIKISIME_DOOR_L_F\012";
static const char data_3BEDE[42] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_HIKISIME_DOOR_R_F\012";
static const char data_3BF08[41] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_OSISIME_DOOR_L_F\012";
static const char data_3BF31[41] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_OSISIME_DOOR_R_F\012";
static const char data_3BF5A[43] __attribute__((scn_string_literal)) = "Door Open Motion = SIN_AKESIME_FUSUMA_L_F\012";
static const char data_3BF85[43] __attribute__((scn_string_literal)) = "Door Open Motion = SIN_AKESIME_FUSUMA_R_F\012";
static const char data_3BFB0[42] __attribute__((scn_string_literal)) = "Door Open Motion = SIN_HIKISIME_DOOR_L_F\012";
static const char data_3BFDA[42] __attribute__((scn_string_literal)) = "Door Open Motion = SIN_HIKISIME_DOOR_R_F\012";
static const char data_3C004[41] __attribute__((scn_string_literal)) = "Door Open Motion = SIN_OSISIME_DOOR_L_F\012";
static const char data_3C02D[41] __attribute__((scn_string_literal)) = "Door Open Motion = SIN_OSISIME_DOOR_R_F\012";
static const char data_3C056[43] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_AKESIME_FUSUMA_L_F\012";
static const char data_3C081[43] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_AKESIME_FUSUMA_R_F\012";
static const char data_3C0AC[42] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_HIKISIME_DOOR_L_F\012";
static const char data_3C0D6[42] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_HIKISIME_DOOR_R_F\012";
static const char data_3C100[41] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_OSISIME_DOOR_L_F\012";
static const char data_3C129[41] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_OSISIME_DOOR_R_F\012";
static const char data_3C152[43] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_AKESIME_FUSUMA_L_F\012";
static const char data_3C17D[43] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_AKESIME_FUSUMA_R_F\012";
static const char data_3C1A8[42] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_HIKISIME_DOOR_L_F\012";
static const char data_3C1D2[42] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_HIKISIME_DOOR_R_F\012";
static const char data_3C1FC[41] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_OSISIME_DOOR_L_F\012";
static const char data_3C225[41] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_OSISIME_DOOR_R_F\012";
static const char data_3C24E[44] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_OSISIME_DOOR_AP_L_F\012";
static const char data_3C27A[44] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_OSISIME_DOOR_AP_L_F\012";
static const char data_3C2A6[44] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_OSISIME_DOOR_AP_L_F\012";
static const char data_3C2D2[44] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_OSISIME_DOOR_AP_L_F\012";
static const char data_3C2FE[44] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_OSISIME_DOOR_AP_L_F\012";
static const char data_3C32A[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_3C346[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_3C362[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_3C37E[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_3C39A[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_3C3B6[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_3C3D2[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_3C3EE[13] __attribute__((scn_string_literal)) = "open-move!!\012";
static const char data_3C3FB[11] __attribute__((scn_string_literal)) = "M_MDOR.BIN";
static const char data_3C406[] __attribute__((scn_string_literal)) = "This Charactor Work is NULL!!\n";
static const char data_3C425[22] __attribute__((scn_string_literal)) = "手動ドア開け番号指定\012";
static const char data_3C43B[34] __attribute__((scn_string_literal)) = "無限ループになりそうなので抜ける\012";
static const char data_3C45D[] __attribute__((scn_string_literal)) = "Don't make BOX Buffer!!\n";
static const char data_3C476[] __attribute__((scn_string_literal)) = "Don't make BOX Buffer!!\n";
static const char data_3C48F[15] __attribute__((scn_string_literal)) = "Door System : ";
static const char data_3C49E[15] __attribute__((scn_string_literal)) = "Door System : ";
static const char data_3C4AD[18] __attribute__((scn_string_literal)) = "Door Box is ON!!\012";
static const char data_3C4BF[19] __attribute__((scn_string_literal)) = "Door Box is OFF!!\012";
static const char data_3C4D2[18] __attribute__((scn_string_literal)) = "Door Box is ON!!\012";
static const char data_3C4E4[20] __attribute__((scn_string_literal)) = "Door Box is OFF!!\012";
static const DSWKDoorEntry data_3C4F8[17] = {
  {.slot = 0, .flags = 0, .ctrl_mask_pair = 0x660067U, .coarse_metric = 0.0f, .selection_metric = 0.0f, .view_ang_delta = 0, .group = 0, .global1 = 0xFFFFFFFFU, .global2 = 0xFFFFFFFFU, .global3 = 0xFFFFFFFFU, .sndCommandId = 0xFFFFFFFFU, .global5 = 0xFFFFFFFFU, .global6 = 0xFFFFFFFFU},
  {.slot = 4, .flags = 0, .ctrl_mask_pair = 0x680069U, .coarse_metric = 0.0f, .selection_metric = 0.0f, .view_ang_delta = 0, .group = 0, .global1 = 0xFFFFFFFFU, .global2 = 0xFFFFFFFFU, .global3 = 0xFFFFFFFFU, .sndCommandId = 0xFFFFFFFFU, .global5 = 0xFFFFFFFFU, .global6 = 0xFFFFFFFFU},
  {.slot = 6, .flags = 0, .ctrl_mask_pair = 0x760077U, .coarse_metric = 0.0f, .selection_metric = 0.0f, .view_ang_delta = 0, .group = 0, .global1 = 0xFFFFFFFFU, .global2 = 0xFFFFFFFFU, .global3 = 0xFFFFFFFFU, .sndCommandId = 0xFFFFFFFFU, .global5 = 0xFFFFFFFFU, .global6 = 0xFFFFFFFFU},
  {.slot = 8, .flags = 0, .ctrl_mask_pair = 0x6E006FU, .coarse_metric = 0.0f, .selection_metric = 0.0f, .view_ang_delta = 0, .group = 0, .global1 = 0xFFFFFFFFU, .global2 = 0xFFFFFFFFU, .global3 = 0xFFFFFFFFU, .sndCommandId = 0xFFFFFFFFU, .global5 = 0xFFFFFFFFU, .global6 = 0xFFFFFFFFU},
  {.slot = 10, .flags = 0, .ctrl_mask_pair = 0x700071U, .coarse_metric = 0.0f, .selection_metric = 0.0f, .view_ang_delta = 0, .group = 0, .global1 = 0xFFFFFFFFU, .global2 = 0xFFFFFFFFU, .global3 = 0xFFFFFFFFU, .sndCommandId = 0xFFFFFFFFU, .global5 = 0xFFFFFFFFU, .global6 = 0xFFFFFFFFU},
  {.slot = 12, .flags = 0, .ctrl_mask_pair = 0x6C006DU, .coarse_metric = 0.0f, .selection_metric = 0.0f, .view_ang_delta = 0, .group = 0, .global1 = 0xFFFFFFFFU, .global2 = 0xFFFFFFFFU, .global3 = 0xFFFFFFFFU, .sndCommandId = 0xFFFFFFFFU, .global5 = 0xFFFFFFFFU, .global6 = 0xFFFFFFFFU},
  {.slot = 14, .flags = 0, .ctrl_mask_pair = 0x720073U, .coarse_metric = 0.0f, .selection_metric = 0.0f, .view_ang_delta = 0, .group = 0, .global1 = 0xFFFFFFFFU, .global2 = 0xFFFFFFFFU, .global3 = 0xFFFFFFFFU, .sndCommandId = 0xFFFFFFFFU, .global5 = 0xFFFFFFFFU, .global6 = 0xFFFFFFFFU},
  {.slot = 16, .flags = 0, .ctrl_mask_pair = 0x740075U, .coarse_metric = 0.0f, .selection_metric = 0.0f, .view_ang_delta = 0, .group = 0, .global1 = 0xFFFFFFFFU, .global2 = 0xFFFFFFFFU, .global3 = 0xFFFFFFFFU, .sndCommandId = 0xFFFFFFFFU, .global5 = 0xFFFFFFFFU, .global6 = 0xFFFFFFFFU},
  {.slot = 18, .flags = 0, .ctrl_mask_pair = 0x640078U, .coarse_metric = 0.0f, .selection_metric = 0.0f, .view_ang_delta = 0, .group = 0, .global1 = 0xFFFFFFFFU, .global2 = 0xFFFFFFFFU, .global3 = 0xFFFFFFFFU, .sndCommandId = 0xFFFFFFFFU, .global5 = 0xFFFFFFFFU, .global6 = 0xFFFFFFFFU},
  {.slot = 20, .flags = 0, .ctrl_mask_pair = 0x790065U, .coarse_metric = 0.0f, .selection_metric = 0.0f, .view_ang_delta = 0, .group = 0, .global1 = 0xFFFFFFFFU, .global2 = 0xFFFFFFFFU, .global3 = 0xFFFFFFFFU, .sndCommandId = 0xFFFFFFFFU, .global5 = 0xFFFFFFFFU, .global6 = 0xFFFFFFFFU},
  {.slot = 22, .flags = 0, .ctrl_mask_pair = 0x7B007BU, .coarse_metric = 0.0f, .selection_metric = 0.0f, .view_ang_delta = 0, .group = 0, .global1 = 0xFFFFFFFFU, .global2 = 0xFFFFFFFFU, .global3 = 0xFFFFFFFFU, .sndCommandId = 0xFFFFFFFFU, .global5 = 0xFFFFFFFFU, .global6 = 0xFFFFFFFFU},
  {.slot = 24, .flags = 0, .ctrl_mask_pair = 0x7A007AU, .coarse_metric = 0.0f, .selection_metric = 0.0f, .view_ang_delta = 0, .group = 0, .global1 = 0xFFFFFFFFU, .global2 = 0xFFFFFFFFU, .global3 = 0xFFFFFFFFU, .sndCommandId = 0xFFFFFFFFU, .global5 = 0xFFFFFFFFU, .global6 = 0xFFFFFFFFU},
  {.slot = 26, .flags = 0, .ctrl_mask_pair = 0x7C007CU, .coarse_metric = 0.0f, .selection_metric = 0.0f, .view_ang_delta = 0, .group = 0, .global1 = 0xFFFFFFFFU, .global2 = 0xFFFFFFFFU, .global3 = 0xFFFFFFFFU, .sndCommandId = 0xFFFFFFFFU, .global5 = 0xFFFFFFFFU, .global6 = 0xFFFFFFFFU},
  {.slot = 28, .flags = 0, .ctrl_mask_pair = 0x7D007DU, .coarse_metric = 0.0f, .selection_metric = 0.0f, .view_ang_delta = 0, .group = 0, .global1 = 0xFFFFFFFFU, .global2 = 0xFFFFFFFFU, .global3 = 0xFFFFFFFFU, .sndCommandId = 0xFFFFFFFFU, .global5 = 0xFFFFFFFFU, .global6 = 0xFFFFFFFFU},
  {.slot = 30, .flags = 0, .ctrl_mask_pair = 0x830084U, .coarse_metric = 0.0f, .selection_metric = 0.0f, .view_ang_delta = 0, .group = 0, .global1 = 0xFFFFFFFFU, .global2 = 0xFFFFFFFFU, .global3 = 0xFFFFFFFFU, .sndCommandId = 0xFFFFFFFFU, .global5 = 0xFFFFFFFFU, .global6 = 0xFFFFFFFFU},
  {.slot = 32, .flags = 0, .ctrl_mask_pair = 0x810082U, .coarse_metric = 0.0f, .selection_metric = 0.0f, .view_ang_delta = 0, .group = 0, .global1 = 0xFFFFFFFFU, .global2 = 0xFFFFFFFFU, .global3 = 0xFFFFFFFFU, .sndCommandId = 0xFFFFFFFFU, .global5 = 0xFFFFFFFFU, .global6 = 0xFFFFFFFFU},
  {.slot = 34, .flags = 0, .ctrl_mask_pair = 0x7F0080U, .coarse_metric = 0.0f, .selection_metric = 0.0f, .view_ang_delta = 0, .group = 0, .global1 = 0xFFFFFFFFU, .global2 = 0xFFFFFFFFU, .global3 = 0xFFFFFFFFU, .sndCommandId = 0xFFFFFFFFU, .global5 = 0xFFFFFFFFU, .global6 = 0xFFFFFFFFU}
};
static const int32_t data_3C86C[13] = {-1, 0, 6553700, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1};
static const uint32_t data_3C8A0 = 2;
static const int32_t data_3C8A4[35] = {
  -1, 3, -1, 4, -1, 13, -1, 9, -1, 10, -1, 8, -1, 11, -1, 12, -1, 0, -1, 1, -1, 6, -1, 7, -1, 5, -1, 14, -1, 15, -1, 16, -1, 17, -1
};
static char data_3C930[] = "DR15_026";
static char data_3C939[] = "DR15_029";
static char data_3C942[] = "DR01_015";
static char data_3C94B[] = "DR15_028";
static char data_3C954[] = "DR15_016";
static char data_3C95D[] = "DR01_016";
static char data_3C966[] = "DR23_000";
static const uint8_t data_3C96F[1] = {0};
static char *const data_3C970 = data_3C930;
static const int32_t data_3C974[3] = {20, 131755, 197291};
static char *const data_3C980 = data_3C939;
static const int32_t data_3C984[3] = {20, 262827, 328363};
static char *const data_3C990 = data_3C942;
static const int32_t data_3C994[3] = {27, 683, 66219};
static char *const data_3C9A0 = data_3C94B;
static const int32_t data_3C9A4[3] = {20, 393899, 459435};
static char *const data_3C9B0 = data_3C954;
static const int32_t data_3C9B4[3] = {15, 524971, 590507};
static char *const data_3C9C0 = data_3C95D;
static const int32_t data_3C9C4[3] = {30, 683, 66219};
static char *const data_3C9D0 = data_3C966;
static const int32_t data_3C9D4[3] = {23, 131755, 197291};
static const MapObjectRecord data_3C9E0[18] = {
  {.kind = 2, .resourceIndex = 0, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-0.323f, 0.0f, -1.8162f}, .yawFixed16 = 0},
  {.kind = 2, .resourceIndex = 1, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-0.087f, 0.0f, 1.85f}, .yawFixed16 = 32768},
  {.kind = 2, .resourceIndex = 0, .scale = {1.0f, 1.0f, 1.0f}, .pos = {4.515f, 0.0f, -1.816f}, .yawFixed16 = 0},
  {.kind = 2, .resourceIndex = 0, .scale = {1.0f, 1.0f, 1.0f}, .pos = {6.346f, 0.0f, 0.015f}, .yawFixed16 = 16384},
  {.kind = 2, .resourceIndex = 1, .scale = {1.0f, 1.0f, 1.0f}, .pos = {4.515f, 0.0f, 1.8142f}, .yawFixed16 = 32768},
  {.kind = 2, .resourceIndex = 5, .scale = {1.0f, 1.0f, 1.0f}, .pos = {5.4f, 0.0f, -3.6149f}, .yawFixed16 = 0},
  {.kind = 2, .resourceIndex = 2, .scale = {1.0f, 1.0f, 1.0f}, .pos = {21.007f, 0.0f, 1.833f}, .yawFixed16 = 0},
  {.kind = 2, .resourceIndex = 2, .scale = {1.0f, 1.0f, 1.0f}, .pos = {16.302f, 0.0f, 1.833f}, .yawFixed16 = 0},
  {.kind = 2, .resourceIndex = 3, .scale = {1.0f, 1.0f, 1.0f}, .pos = {15.345f, 0.0f, -0.0183f}, .yawFixed16 = 0},
  {.kind = 2, .resourceIndex = 0, .scale = {1.0f, 1.0f, 1.0f}, .pos = {15.372f, 0.0f, -4.555f}, .yawFixed16 = 0},
  {.kind = 2, .resourceIndex = 0, .scale = {1.0f, 1.0f, 1.0f}, .pos = {17.218f, 0.0f, -1.8344f}, .yawFixed16 = 16384},
  {.kind = 2, .resourceIndex = 0, .scale = {1.0f, 1.0f, 1.0f}, .pos = {19.062f, 0.0f, -4.555f}, .yawFixed16 = 0},
  {.kind = 2, .resourceIndex = 0, .scale = {1.0f, 1.0f, 1.0f}, .pos = {19.062f, 0.0f, -0.0141f}, .yawFixed16 = 0},
  {.kind = 2, .resourceIndex = 0, .scale = {1.0f, 1.0f, 1.0f}, .pos = {21.882f, 0.0f, -0.0141f}, .yawFixed16 = 0},
  {.kind = 2, .resourceIndex = 4, .scale = {1.0f, 1.0f, 1.0f}, .pos = {14.886f, -0.219f, 6.3261f}, .yawFixed16 = 0},
  {.kind = 2, .resourceIndex = 6, .scale = {1.0f, 1.0f, 1.0f}, .pos = {0.901f, 0.0f, -0.9f}, .yawFixed16 = 16384},
  {.kind = 2, .resourceIndex = 6, .scale = {1.0f, 1.0f, 1.0f}, .pos = {19.871f, 0.0f, 2.755f}, .yawFixed16 = 16384},
  {.kind = 2, .resourceIndex = 6, .scale = {1.0f, 1.0f, 1.0f}, .pos = {20.910999f, 0.0f, 5.368f}, .yawFixed16 = 16384}
};
static const uint32_t data_3CC68[4] = {0x413042CU, 0x42F042DU, 0x4300432U, 0x3870431U};
static const char data_3CC78[20] __attribute__((scn_string_literal)) = "TUNED FreeTalk def\012";
static const char data_3CC8C[20] __attribute__((scn_string_literal)) = "TUNED FreeTalk def\012";
static const uint32_t data_3CCA0 = 380;
static const int32_t data_3CCA4[6] = {1, 390, 0, 1018, 0, -1};
static const uint32_t data_3CCBC = 510;
static const int32_t data_3CCC0[6] = {1, 525, 0, 1020, 0, -1};
static const uint32_t data_3CCD8 = 150;
static const int32_t data_3CCDC[6] = {1, 160, 0, 1019, 0, -1};
static const uint32_t data_3CCF4 = 530;
static const int32_t data_3CCF8[6] = {1, 970, 0, 1014, 0, -1};
static const uint32_t data_3CD10 = 911;
static const int32_t data_3CD14[6] = {1, 155, 0, 1015, 0, -1};
static const char data_3CD2C[7] __attribute__((scn_string_literal)) = "FREE02";
static const char data_3CD33[7] __attribute__((scn_string_literal)) = "FREE03";
static const char data_3CD3A[7] __attribute__((scn_string_literal)) = "FREE04";
static const char data_3CD41[7] __attribute__((scn_string_literal)) = "FREE01";
static const char data_3CD48[24] __attribute__((scn_string_literal)) = "JOMO SA Auth Ready!!\012";
static const FogColourData data_3CD60 = {.red = 8.0f, .green = 150.0f, .blue = 1.0f, .colour = 0xFF000000U};
static const char data_3CD70[11] __attribute__((scn_string_literal)) = "amb037.snd";
static const char data_3CD7B[11] __attribute__((scn_string_literal)) = "bgm009.snd";
static const char data_3CD86[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_3CD8B[13] __attribute__((scn_string_literal)) = "f1omoyaa.snd";
static const char data_3CD98[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_3CD9D[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_3CDA2[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_3CDA7[17] __attribute__((scn_string_literal)) = "JOMO_AKIRA = %d\012";
static const char data_3CDB8[29] __attribute__((scn_string_literal)) = "JOMO FadeIn Start! 30 frame\012";
static const char data_3CDD5[28] __attribute__((scn_string_literal)) = "ふくちゃんハイモデル解放。\012";
static const char data_3CDF1[26] __attribute__((scn_string_literal)) = "いねさんハイモデル解放。\012";
static const char data_3CE0B[24] __attribute__((scn_string_literal)) = "ふくちゃん読み込み失敗\012";
static const char data_3CE23[22] __attribute__((scn_string_literal)) = "ふくちゃんロック終了\012";
static const char data_3CE39[22] __attribute__((scn_string_literal)) = "いねさん読み込み失敗\012";
static const char data_3CE4F[20] __attribute__((scn_string_literal)) = "いねさんロック終了\012";
static const char data_3CE63[16] __attribute__((scn_string_literal)) = "強制睡眠ＯＦＦ\012";
static const char data_3CE73[14] __attribute__((scn_string_literal)) = "強制睡眠ＯＮ\012";
static const char data_3CE81[19] __attribute__((scn_string_literal)) = "強制睡眠リセット\012";
static const uint32_t data_3CE94 = 0x4144CCCD;
static const float data_3CE98[2] = {0.0f, -2.2f};
static const uint32_t data_3CEA0 = 0x3F800000;
static const float data_3CEA4[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint8_t data_3CF28[8] = {0};
static const int32_t data_3CF30[49] = {
  -1, 139, 0, -1, 141, 142, 143, 144, 146, 147, 148, 0, -1, 149, 0, -1, 151, 152, 153, 154, 156, 157, 158, 0, -1, 159, 0, -1, 161, 0, -1, 0, 0, -1, 0,
  0, -1, 0, 0, -1, 162, 0, -1, 163, 0, -1, 164, 0, -1
};
static const int32_t data_3CFF4[57] = {
  0, 0, -8046, 0, 0, -1820, -13088, 4350, -2894, 0, 0, -9011, 0, 0, -18422, 0, 0, -9812, 0, 0, 0, 0, 0, -7518, 0, 0, -17385, 0, 0, -11559, 0, 0, 0, 0,
  0, -5734, 0, 0, -18058, 0, 0, -12233, 0, 0, 0, 0, 0, -7827, 0, 0, -16183, 0, 0, -13981, 0, -72, 0
};
static const int32_t data_3D0D8[57] = {
  0, 0, -2712, 0, 0, -691, 0, 0, 0, 0, 0, -1092, 0, 0, -1438, 0, 0, -2748, 0, 0, 0, 0, 0, 364, 0, 0, -4132, 0, 0, -3531, 0, 0, 0, 0, 0, -2020, 0, 0,
  -6881, 0, 0, -5333, 0, 109, -200, 0, 0, -3986, 0, 0, -8829, 0, 0, -7245, 0, -764, -837
};
static const uint32_t data_3D1BC = 0x303131;
static const uint32_t data_3D1C0 = 0x393131;
static const uint32_t data_3D1C4 = 0x373731;
static const uint32_t data_3D1C8 = 0x343031;
static const char data_3D1CC[] __attribute__((scn_string_literal)) = "61564";
static const char data_3D1D2[] __attribute__((scn_string_literal)) = "37742";
static const char data_3D1D8[] __attribute__((scn_string_literal)) = "37749";
static const char data_3D1DE[] __attribute__((scn_string_literal)) = "22550";
static const char data_3D1E4[] __attribute__((scn_string_literal)) = "046861564";
static const char data_3D1EE[] __attribute__((scn_string_literal)) = "046837742";
static const char data_3D1F8[] __attribute__((scn_string_literal)) = "046837749";
static const char data_3D202[] __attribute__((scn_string_literal)) = "046822550";
static const char data_3D20C[] __attribute__((scn_string_literal)) = "2?????";
static const char data_3D213[] __attribute__((scn_string_literal)) = "5?????";
static const char data_3D21A[] __attribute__((scn_string_literal)) = "7?????";
static const char data_3D221[] __attribute__((scn_string_literal)) = "8?????";
static const char data_3D228[] __attribute__((scn_string_literal)) = "3?????";
static const char data_3D22F[] __attribute__((scn_string_literal)) = "4?????";
static const char data_3D236[] __attribute__((scn_string_literal)) = "6?????";
static const char data_3D23D[] __attribute__((scn_string_literal)) = "9?????";
static const char data_3D244[] __attribute__((scn_string_literal)) = "0?????????";
static const char data_3D24F[] __attribute__((scn_string_literal)) = "1?????";
static const char data_3D256[12] __attribute__((scn_string_literal)) = "メモ帳消去\012";
static const char data_3D262[6] __attribute__((scn_string_literal)) = "E1032";
static const char data_3D268[10] __attribute__((scn_string_literal)) = "E1032A001";
static const char data_3D272[10] __attribute__((scn_string_literal)) = "E1032A002";
static const char data_3D27C[10] __attribute__((scn_string_literal)) = "E1032A003";
static const char data_3D286[10] __attribute__((scn_string_literal)) = "E1032A004";
static const char data_3D290[10] __attribute__((scn_string_literal)) = "E1032A005";
static const char data_3D29A[6] __attribute__((scn_string_literal)) = "E1032";
static const char data_3D2A0[6] __attribute__((scn_string_literal)) = "E1032";
static const char data_3D2A6[10] __attribute__((scn_string_literal)) = "E1032A061";
static const char data_3D2B0[10] __attribute__((scn_string_literal)) = "E1032A062";
static const char data_3D2BA[10] __attribute__((scn_string_literal)) = "E1032A063";
static const char data_3D2C4[10] __attribute__((scn_string_literal)) = "E1032A064";
static const char data_3D2CE[6] __attribute__((scn_string_literal)) = "E1032";
static const char data_3D2D4[6] __attribute__((scn_string_literal)) = "E1032";
static const char data_3D2DA[6] __attribute__((scn_string_literal)) = "E1032";
static const char data_3D2E0[6] __attribute__((scn_string_literal)) = "E1032";
static const char data_3D2E6[10] __attribute__((scn_string_literal)) = "E1032A031";
static const char data_3D2F0[10] __attribute__((scn_string_literal)) = "E1032A029";
static const char data_3D2FA[10] __attribute__((scn_string_literal)) = "E1032A032";
static const char data_3D304[10] __attribute__((scn_string_literal)) = "E1032A033";
static const char data_3D30E[6] __attribute__((scn_string_literal)) = "E1032";
static const char data_3D314[10] __attribute__((scn_string_literal)) = "E1032A028";
static const char data_3D31E[10] __attribute__((scn_string_literal)) = "E1032A029";
static const char data_3D328[10] __attribute__((scn_string_literal)) = "E1032A030";
static const char data_3D332[6] __attribute__((scn_string_literal)) = "E1032";
static const char data_3D338[10] __attribute__((scn_string_literal)) = "E1032G001";
static const char data_3D342[10] __attribute__((scn_string_literal)) = "E1032G002";
static const char data_3D34C[6] __attribute__((scn_string_literal)) = "E1032";
static const char data_3D352[10] __attribute__((scn_string_literal)) = "E1032I001";
static const char data_3D35C[10] __attribute__((scn_string_literal)) = "E1032A052";
static const char data_3D366[10] __attribute__((scn_string_literal)) = "E1032I007";
static const char data_3D370[10] __attribute__((scn_string_literal)) = "E1032I008";
static const char data_3D37A[10] __attribute__((scn_string_literal)) = "E1032A097";
static const char data_3D384[10] __attribute__((scn_string_literal)) = "E1032I009";
static const char data_3D38E[10] __attribute__((scn_string_literal)) = "E1032I010";
static const char data_3D398[10] __attribute__((scn_string_literal)) = "E1032A054";
static const char data_3D3A2[10] __attribute__((scn_string_literal)) = "E1032I011";
static const char data_3D3AC[10] __attribute__((scn_string_literal)) = "E1032A055";
static const char data_3D3B6[10] __attribute__((scn_string_literal)) = "E1032I012";
static const char data_3D3C0[10] __attribute__((scn_string_literal)) = "E1032A056";
static const char data_3D3CA[10] __attribute__((scn_string_literal)) = "E1032I013";
static const char data_3D3D4[10] __attribute__((scn_string_literal)) = "E1032A057";
static const char data_3D3DE[10] __attribute__((scn_string_literal)) = "E1032I011";
static const char data_3D3E8[10] __attribute__((scn_string_literal)) = "E1032A059";
static const char data_3D3F2[10] __attribute__((scn_string_literal)) = "E1032I009";
static const char data_3D3FC[10] __attribute__((scn_string_literal)) = "E1032I010";
static const char data_3D406[10] __attribute__((scn_string_literal)) = "E1032A054";
static const char data_3D410[10] __attribute__((scn_string_literal)) = "E1032I011";
static const char data_3D41A[10] __attribute__((scn_string_literal)) = "E1032A060";
static const char data_3D424[10] __attribute__((scn_string_literal)) = "E1032I009";
static const char data_3D42E[10] __attribute__((scn_string_literal)) = "E1032I010";
static const char data_3D438[10] __attribute__((scn_string_literal)) = "E1032A054";
static const char data_3D442[10] __attribute__((scn_string_literal)) = "E1032I011";
static const char data_3D44C[10] __attribute__((scn_string_literal)) = "E1032I001";
static const char data_3D456[10] __attribute__((scn_string_literal)) = "E1032A007";
static const char data_3D460[10] __attribute__((scn_string_literal)) = "E1032A006";
static const char data_3D46A[10] __attribute__((scn_string_literal)) = "E1032I002";
static const char data_3D474[10] __attribute__((scn_string_literal)) = "E1032I003";
static const char data_3D47E[10] __attribute__((scn_string_literal)) = "E1032I004";
static const char data_3D488[10] __attribute__((scn_string_literal)) = "E1032I005";
static const char data_3D492[10] __attribute__((scn_string_literal)) = "E1032I006";
static const char data_3D49C[10] __attribute__((scn_string_literal)) = "E1032A008";
static const char data_3D4A6[10] __attribute__((scn_string_literal)) = "E1032A009";
static const char data_3D4B0[10] __attribute__((scn_string_literal)) = "E1032K001";
static const char data_3D4BA[10] __attribute__((scn_string_literal)) = "E1032K002";
static const char data_3D4C4[10] __attribute__((scn_string_literal)) = "E1032K019";
static const char data_3D4CE[10] __attribute__((scn_string_literal)) = "E1032K020";
static const char data_3D4D8[10] __attribute__((scn_string_literal)) = "E1032K022";
static const char data_3D4E2[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_3D4EC[10] __attribute__((scn_string_literal)) = "E1032K016";
static const char data_3D4F6[10] __attribute__((scn_string_literal)) = "E1032K017";
static const char data_3D500[10] __attribute__((scn_string_literal)) = "E1032K018";
static const char data_3D50A[10] __attribute__((scn_string_literal)) = "E1032K003";
static const char data_3D514[10] __attribute__((scn_string_literal)) = "E1032K016";
static const char data_3D51E[10] __attribute__((scn_string_literal)) = "E1032K003";
static const char data_3D528[10] __attribute__((scn_string_literal)) = "E1032K017";
static const char data_3D532[10] __attribute__((scn_string_literal)) = "E1032K003";
static const char data_3D53C[10] __attribute__((scn_string_literal)) = "E1032K018";
static const char data_3D546[10] __attribute__((scn_string_literal)) = "E1032K004";
static const char data_3D550[10] __attribute__((scn_string_literal)) = "E1032K016";
static const char data_3D55A[10] __attribute__((scn_string_literal)) = "E1032K004";
static const char data_3D564[10] __attribute__((scn_string_literal)) = "E1032K017";
static const char data_3D56E[10] __attribute__((scn_string_literal)) = "E1032K004";
static const char data_3D578[10] __attribute__((scn_string_literal)) = "E1032K018";
static const char data_3D582[10] __attribute__((scn_string_literal)) = "E1032K005";
static const char data_3D58C[10] __attribute__((scn_string_literal)) = "E1032K016";
static const char data_3D596[10] __attribute__((scn_string_literal)) = "E1032K005";
static const char data_3D5A0[10] __attribute__((scn_string_literal)) = "E1032K017";
static const char data_3D5AA[10] __attribute__((scn_string_literal)) = "E1032K005";
static const char data_3D5B4[10] __attribute__((scn_string_literal)) = "E1032K018";
static const char data_3D5BE[10] __attribute__((scn_string_literal)) = "E1032K006";
static const char data_3D5C8[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_3D5D2[10] __attribute__((scn_string_literal)) = "E1032K006";
static const char data_3D5DC[10] __attribute__((scn_string_literal)) = "E1032K017";
static const char data_3D5E6[10] __attribute__((scn_string_literal)) = "E1032K006";
static const char data_3D5F0[10] __attribute__((scn_string_literal)) = "E1032K018";
static const char data_3D5FA[10] __attribute__((scn_string_literal)) = "E1032K007";
static const char data_3D604[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_3D60E[10] __attribute__((scn_string_literal)) = "E1032K007";
static const char data_3D618[10] __attribute__((scn_string_literal)) = "E1032K017";
static const char data_3D622[10] __attribute__((scn_string_literal)) = "E1032K007";
static const char data_3D62C[10] __attribute__((scn_string_literal)) = "E1032K018";
static const char data_3D636[10] __attribute__((scn_string_literal)) = "E1032K008";
static const char data_3D640[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_3D64A[10] __attribute__((scn_string_literal)) = "E1032K008";
static const char data_3D654[10] __attribute__((scn_string_literal)) = "E1032K017";
static const char data_3D65E[10] __attribute__((scn_string_literal)) = "E1032K008";
static const char data_3D668[10] __attribute__((scn_string_literal)) = "E1032K018";
static const char data_3D672[10] __attribute__((scn_string_literal)) = "E1032K009";
static const char data_3D67C[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_3D686[10] __attribute__((scn_string_literal)) = "E1032K009";
static const char data_3D690[10] __attribute__((scn_string_literal)) = "E1032K016";
static const char data_3D69A[10] __attribute__((scn_string_literal)) = "E1032K009";
static const char data_3D6A4[10] __attribute__((scn_string_literal)) = "E1032K018";
static const char data_3D6AE[10] __attribute__((scn_string_literal)) = "E1032K010";
static const char data_3D6B8[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_3D6C2[10] __attribute__((scn_string_literal)) = "E1032K010";
static const char data_3D6CC[10] __attribute__((scn_string_literal)) = "E1032K016";
static const char data_3D6D6[10] __attribute__((scn_string_literal)) = "E1032K010";
static const char data_3D6E0[10] __attribute__((scn_string_literal)) = "E1032K018";
static const char data_3D6EA[10] __attribute__((scn_string_literal)) = "E1032K011";
static const char data_3D6F4[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_3D6FE[10] __attribute__((scn_string_literal)) = "E1032K011";
static const char data_3D708[10] __attribute__((scn_string_literal)) = "E1032K016";
static const char data_3D712[10] __attribute__((scn_string_literal)) = "E1032K011";
static const char data_3D71C[10] __attribute__((scn_string_literal)) = "E1032K018";
static const char data_3D726[10] __attribute__((scn_string_literal)) = "E1032K012";
static const char data_3D730[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_3D73A[10] __attribute__((scn_string_literal)) = "E1032K012";
static const char data_3D744[10] __attribute__((scn_string_literal)) = "E1032K016";
static const char data_3D74E[10] __attribute__((scn_string_literal)) = "E1032K012";
static const char data_3D758[10] __attribute__((scn_string_literal)) = "E1032K017";
static const char data_3D762[10] __attribute__((scn_string_literal)) = "E1032K013";
static const char data_3D76C[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_3D776[10] __attribute__((scn_string_literal)) = "E1032K013";
static const char data_3D780[10] __attribute__((scn_string_literal)) = "E1032K016";
static const char data_3D78A[10] __attribute__((scn_string_literal)) = "E1032K013";
static const char data_3D794[10] __attribute__((scn_string_literal)) = "E1032K017";
static const char data_3D79E[10] __attribute__((scn_string_literal)) = "E1032K014";
static const char data_3D7A8[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_3D7B2[10] __attribute__((scn_string_literal)) = "E1032K014";
static const char data_3D7BC[10] __attribute__((scn_string_literal)) = "E1032K016";
static const char data_3D7C6[10] __attribute__((scn_string_literal)) = "E1032K014";
static const char data_3D7D0[10] __attribute__((scn_string_literal)) = "E1032K017";
static const char data_3D7DA[10] __attribute__((scn_string_literal)) = "E1032K004";
static const char data_3D7E4[10] __attribute__((scn_string_literal)) = "E1032K007";
static const char data_3D7EE[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_3D7F8[10] __attribute__((scn_string_literal)) = "E1032K004";
static const char data_3D802[10] __attribute__((scn_string_literal)) = "E1032K007";
static const char data_3D80C[10] __attribute__((scn_string_literal)) = "E1032K017";
static const char data_3D816[10] __attribute__((scn_string_literal)) = "E1032K004";
static const char data_3D820[10] __attribute__((scn_string_literal)) = "E1032K006";
static const char data_3D82A[10] __attribute__((scn_string_literal)) = "E1032K017";
static const char data_3D834[10] __attribute__((scn_string_literal)) = "E1032K004";
static const char data_3D83E[10] __attribute__((scn_string_literal)) = "E1032K013";
static const char data_3D848[10] __attribute__((scn_string_literal)) = "E1032K017";
static const char data_3D852[10] __attribute__((scn_string_literal)) = "E1032K005";
static const char data_3D85C[10] __attribute__((scn_string_literal)) = "E1032K007";
static const char data_3D866[10] __attribute__((scn_string_literal)) = "E1032K018";
static const char data_3D870[10] __attribute__((scn_string_literal)) = "E1032K007";
static const char data_3D87A[10] __attribute__((scn_string_literal)) = "E1032K004";
static const char data_3D884[10] __attribute__((scn_string_literal)) = "E1032K017";
static const char data_3D88E[10] __attribute__((scn_string_literal)) = "E1032K008";
static const char data_3D898[10] __attribute__((scn_string_literal)) = "E1032K010";
static const char data_3D8A2[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_3D8AC[10] __attribute__((scn_string_literal)) = "E1032K008";
static const char data_3D8B6[10] __attribute__((scn_string_literal)) = "E1032K013";
static const char data_3D8C0[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_3D8CA[10] __attribute__((scn_string_literal)) = "E1032K010";
static const char data_3D8D4[10] __attribute__((scn_string_literal)) = "E1032K005";
static const char data_3D8DE[10] __attribute__((scn_string_literal)) = "E1032K016";
static const char data_3D8E8[10] __attribute__((scn_string_literal)) = "E1032K010";
static const char data_3D8F2[10] __attribute__((scn_string_literal)) = "E1032K004";
static const char data_3D8FC[10] __attribute__((scn_string_literal)) = "E1032K018";
static const char data_3D906[10] __attribute__((scn_string_literal)) = "E1032K010";
static const char data_3D910[10] __attribute__((scn_string_literal)) = "E1032K007";
static const char data_3D91A[10] __attribute__((scn_string_literal)) = "E1032K018";
static const char data_3D924[10] __attribute__((scn_string_literal)) = "E1032K011";
static const char data_3D92E[10] __attribute__((scn_string_literal)) = "E1032K007";
static const char data_3D938[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_3D942[10] __attribute__((scn_string_literal)) = "E1032K013";
static const char data_3D94C[10] __attribute__((scn_string_literal)) = "E1032K008";
static const char data_3D956[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_3D960[10] __attribute__((scn_string_literal)) = "E1032K010";
static const char data_3D96A[10] __attribute__((scn_string_literal)) = "E1032K005";
static const char data_3D974[10] __attribute__((scn_string_literal)) = "E1032K016";
static const char data_3D97E[10] __attribute__((scn_string_literal)) = "E1032K004";
static const char data_3D988[10] __attribute__((scn_string_literal)) = "E1032K010";
static const char data_3D992[10] __attribute__((scn_string_literal)) = "E1032K016";
static const char data_3D99C[10] __attribute__((scn_string_literal)) = "E1032K004";
static const char data_3D9A6[10] __attribute__((scn_string_literal)) = "E1032K008";
static const char data_3D9B0[10] __attribute__((scn_string_literal)) = "E1032K018";
static const char data_3D9BA[10] __attribute__((scn_string_literal)) = "E1032K003";
static const char data_3D9C4[10] __attribute__((scn_string_literal)) = "E1032K007";
static const char data_3D9CE[10] __attribute__((scn_string_literal)) = "E1032K018";
static const char data_3D9D8[10] __attribute__((scn_string_literal)) = "E1032K007";
static const char data_3D9E2[10] __attribute__((scn_string_literal)) = "E1032K010";
static const char data_3D9EC[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_3D9F6[10] __attribute__((scn_string_literal)) = "E1032K008";
static const char data_3DA00[10] __attribute__((scn_string_literal)) = "E1032K004";
static const char data_3DA0A[10] __attribute__((scn_string_literal)) = "E1032K018";
static const char data_3DA14[10] __attribute__((scn_string_literal)) = "E1032K007";
static const char data_3DA1E[10] __attribute__((scn_string_literal)) = "E1032K010";
static const char data_3DA28[10] __attribute__((scn_string_literal)) = "E1032K018";
static const char data_3DA32[10] __attribute__((scn_string_literal)) = "E1032K004";
static const char data_3DA3C[10] __attribute__((scn_string_literal)) = "E1032K010";
static const char data_3DA46[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_3DA50[10] __attribute__((scn_string_literal)) = "E1032K004";
static const char data_3DA5A[10] __attribute__((scn_string_literal)) = "E1032K007";
static const char data_3DA64[10] __attribute__((scn_string_literal)) = "E1032K017";
static const char data_3DA6E[10] __attribute__((scn_string_literal)) = "E1032K004";
static const char data_3DA78[10] __attribute__((scn_string_literal)) = "E1032K007";
static const char data_3DA82[10] __attribute__((scn_string_literal)) = "E1032K018";
static const char data_3DA8C[10] __attribute__((scn_string_literal)) = "E1032K010";
static const char data_3DA96[10] __attribute__((scn_string_literal)) = "E1032K008";
static const char data_3DAA0[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_3DAAA[10] __attribute__((scn_string_literal)) = "E1032K013";
static const char data_3DAB4[10] __attribute__((scn_string_literal)) = "E1032K007";
static const char data_3DABE[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_3DAC8[10] __attribute__((scn_string_literal)) = "E1032K004";
static const char data_3DAD2[10] __attribute__((scn_string_literal)) = "E1032K013";
static const char data_3DADC[10] __attribute__((scn_string_literal)) = "E1032K016";
static const char data_3DAE6[10] __attribute__((scn_string_literal)) = "E1032K014";
static const char data_3DAF0[10] __attribute__((scn_string_literal)) = "E1032K007";
static const char data_3DAFA[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_3DB04[10] __attribute__((scn_string_literal)) = "E1032K015";
static const char data_3DB0E[10] __attribute__((scn_string_literal)) = "E1032L001";
static const char data_3DB18[10] __attribute__((scn_string_literal)) = "E1032A098";
static const char data_3DB22[10] __attribute__((scn_string_literal)) = "E1032L002";
static const char data_3DB2C[10] __attribute__((scn_string_literal)) = "E1032A099";
static const char data_3DB36[10] __attribute__((scn_string_literal)) = "E1032L003";
static const char data_3DB40[10] __attribute__((scn_string_literal)) = "E1032L004";
static const char data_3DB4A[10] __attribute__((scn_string_literal)) = "E1032L005";
static const char data_3DB54[10] __attribute__((scn_string_literal)) = "E1032A100";
static const char data_3DB5E[10] __attribute__((scn_string_literal)) = "E1032L001";
static const char data_3DB68[10] __attribute__((scn_string_literal)) = "E1032A101";
static const char data_3DB72[10] __attribute__((scn_string_literal)) = "E1032L006";
static const char data_3DB7C[10] __attribute__((scn_string_literal)) = "E1032L007";
static const char data_3DB86[10] __attribute__((scn_string_literal)) = "E1032L008";
static const char data_3DB90[10] __attribute__((scn_string_literal)) = "E1032L009";
static const char data_3DB9A[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_3DBA4[10] __attribute__((scn_string_literal)) = "E1032A102";
static const char data_3DBAE[10] __attribute__((scn_string_literal)) = "E1032L010";
static const char data_3DBB8[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_3DBC2[10] __attribute__((scn_string_literal)) = "E1032L011";
static const char data_3DBCC[10] __attribute__((scn_string_literal)) = "E1032A101";
static const char data_3DBD6[10] __attribute__((scn_string_literal)) = "E1032L012";
static const char data_3DBE0[10] __attribute__((scn_string_literal)) = "E1032A103";
static const char data_3DBEA[10] __attribute__((scn_string_literal)) = "E1032L013";
static const char data_3DBF4[10] __attribute__((scn_string_literal)) = "E1032L014";
static const char data_3DBFE[10] __attribute__((scn_string_literal)) = "E1032L015";
static const char data_3DC08[10] __attribute__((scn_string_literal)) = "E1032A103";
static const char data_3DC12[10] __attribute__((scn_string_literal)) = "E1032L016";
static const char data_3DC1C[10] __attribute__((scn_string_literal)) = "E1032A104";
static const char data_3DC26[10] __attribute__((scn_string_literal)) = "E1032A105";
static const char data_3DC30[10] __attribute__((scn_string_literal)) = "E1032L017";
static const char data_3DC3A[10] __attribute__((scn_string_literal)) = "E1032A106";
static const char data_3DC44[10] __attribute__((scn_string_literal)) = "E1032L018";
static const char data_3DC4E[10] __attribute__((scn_string_literal)) = "E1032A107";
static const char data_3DC58[10] __attribute__((scn_string_literal)) = "E1032L019";
static const char data_3DC62[10] __attribute__((scn_string_literal)) = "E1032A108";
static const char data_3DC6C[10] __attribute__((scn_string_literal)) = "E1032L020";
static const char data_3DC76[10] __attribute__((scn_string_literal)) = "E1032L021";
static const char data_3DC80[10] __attribute__((scn_string_literal)) = "E1032A109";
static const char data_3DC8A[10] __attribute__((scn_string_literal)) = "E1032L022";
static const char data_3DC94[10] __attribute__((scn_string_literal)) = "E1032A110";
static const char data_3DC9E[10] __attribute__((scn_string_literal)) = "E1032L023";
static const char data_3DCA8[10] __attribute__((scn_string_literal)) = "E1032M001";
static const char data_3DCB2[10] __attribute__((scn_string_literal)) = "E1032A111";
static const char data_3DCBC[10] __attribute__((scn_string_literal)) = "E1032M002";
static const char data_3DCC6[10] __attribute__((scn_string_literal)) = "E1032A112";
static const char data_3DCD0[10] __attribute__((scn_string_literal)) = "E1032M003";
static const char data_3DCDA[10] __attribute__((scn_string_literal)) = "E1032A113";
static const char data_3DCE4[10] __attribute__((scn_string_literal)) = "E1032M004";
static const char data_3DCEE[10] __attribute__((scn_string_literal)) = "E1032A104";
static const char data_3DCF8[10] __attribute__((scn_string_literal)) = "E1032A114";
static const char data_3DD02[10] __attribute__((scn_string_literal)) = "E1032M005";
static const char data_3DD0C[10] __attribute__((scn_string_literal)) = "E1032M006";
static const char data_3DD16[10] __attribute__((scn_string_literal)) = "E1032A111";
static const char data_3DD20[10] __attribute__((scn_string_literal)) = "E1032M002";
static const char data_3DD2A[10] __attribute__((scn_string_literal)) = "E1032A115";
static const char data_3DD34[10] __attribute__((scn_string_literal)) = "E1032M018";
static const char data_3DD3E[10] __attribute__((scn_string_literal)) = "E1032A122";
static const char data_3DD48[10] __attribute__((scn_string_literal)) = "E1032M019";
static const char data_3DD52[10] __attribute__((scn_string_literal)) = "E1032A123";
static const char data_3DD5C[10] __attribute__((scn_string_literal)) = "E1032M019";
static const char data_3DD66[10] __attribute__((scn_string_literal)) = "E1032M020";
static const char data_3DD70[10] __attribute__((scn_string_literal)) = "E1032A104";
static const char data_3DD7A[10] __attribute__((scn_string_literal)) = "E1032A114";
static const char data_3DD84[10] __attribute__((scn_string_literal)) = "E1032M010";
static const char data_3DD8E[10] __attribute__((scn_string_literal)) = "E1032N001";
static const char data_3DD98[10] __attribute__((scn_string_literal)) = "E1032A124";
static const char data_3DDA2[10] __attribute__((scn_string_literal)) = "E1032A125";
static const char data_3DDAC[10] __attribute__((scn_string_literal)) = "E1032N002";
static const char data_3DDB6[10] __attribute__((scn_string_literal)) = "E1032A126";
static const char data_3DDC0[10] __attribute__((scn_string_literal)) = "E1032N003";
static const char data_3DDCA[10] __attribute__((scn_string_literal)) = "E1032A127";
static const char data_3DDD4[10] __attribute__((scn_string_literal)) = "E1032A128";
static const char data_3DDDE[10] __attribute__((scn_string_literal)) = "E1032N004";
static const char data_3DDE8[10] __attribute__((scn_string_literal)) = "E1032A129";
static const char data_3DDF2[10] __attribute__((scn_string_literal)) = "E1032N005";
static const char data_3DDFC[10] __attribute__((scn_string_literal)) = "E1032L011";
static const char data_3DE06[10] __attribute__((scn_string_literal)) = "E1032A101";
static const char data_3DE10[10] __attribute__((scn_string_literal)) = "E1032L019";
static const char data_3DE1A[10] __attribute__((scn_string_literal)) = "E1032A117";
static const char data_3DE24[10] __attribute__((scn_string_literal)) = "E1032L024";
static const char data_3DE2E[10] __attribute__((scn_string_literal)) = "E1032L025";
static const char data_3DE38[10] __attribute__((scn_string_literal)) = "E1032L026";
static const char data_3DE42[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_3DE4C[10] __attribute__((scn_string_literal)) = "E1032A118";
static const char data_3DE56[10] __attribute__((scn_string_literal)) = "E1032L023";
static const char data_3DE60[10] __attribute__((scn_string_literal)) = "E1032M001";
static const char data_3DE6A[10] __attribute__((scn_string_literal)) = "E1032A111";
static const char data_3DE74[10] __attribute__((scn_string_literal)) = "E1032M002";
static const char data_3DE7E[10] __attribute__((scn_string_literal)) = "E1032A112";
static const char data_3DE88[10] __attribute__((scn_string_literal)) = "E1032M015";
static const char data_3DE92[10] __attribute__((scn_string_literal)) = "E1032A119";
static const char data_3DE9C[10] __attribute__((scn_string_literal)) = "E1032M016";
static const char data_3DEA6[10] __attribute__((scn_string_literal)) = "E1032A104";
static const char data_3DEB0[10] __attribute__((scn_string_literal)) = "E1032A118";
static const char data_3DEBA[10] __attribute__((scn_string_literal)) = "E1032M017";
static const char data_3DEC4[10] __attribute__((scn_string_literal)) = "E1032O001";
static const char data_3DECE[10] __attribute__((scn_string_literal)) = "E1032A136";
static const char data_3DED8[10] __attribute__((scn_string_literal)) = "E1032O002";
static const char data_3DEE2[10] __attribute__((scn_string_literal)) = "E1032A137";
static const char data_3DEEC[10] __attribute__((scn_string_literal)) = "E1032O007";
static const char data_3DEF6[10] __attribute__((scn_string_literal)) = "E1032A127";
static const char data_3DF00[10] __attribute__((scn_string_literal)) = "E1032O004";
static const char data_3DF0A[10] __attribute__((scn_string_literal)) = "E1032A138";
static const char data_3DF14[10] __attribute__((scn_string_literal)) = "E1032O005";
static const char data_3DF1E[10] __attribute__((scn_string_literal)) = "E1032A139";
static const char data_3DF28[10] __attribute__((scn_string_literal)) = "E1032O006";
static const char data_3DF32[10] __attribute__((scn_string_literal)) = "E1032O001";
static const char data_3DF3C[10] __attribute__((scn_string_literal)) = "E1032A136";
static const char data_3DF46[10] __attribute__((scn_string_literal)) = "E1032O002";
static const char data_3DF50[10] __attribute__((scn_string_literal)) = "E1032A140";
static const char data_3DF5A[10] __attribute__((scn_string_literal)) = "E1032O007";
static const char data_3DF64[10] __attribute__((scn_string_literal)) = "E1032A127";
static const char data_3DF6E[10] __attribute__((scn_string_literal)) = "E1032O004";
static const char data_3DF78[10] __attribute__((scn_string_literal)) = "E1032A138";
static const char data_3DF82[10] __attribute__((scn_string_literal)) = "E1032O005";
static const char data_3DF8C[10] __attribute__((scn_string_literal)) = "E1032A139";
static const char data_3DF96[10] __attribute__((scn_string_literal)) = "E1032O006";
static const char data_3DFA0[10] __attribute__((scn_string_literal)) = "E1032P001";
static const char data_3DFAA[10] __attribute__((scn_string_literal)) = "E1032A085";
static const char data_3DFB4[10] __attribute__((scn_string_literal)) = "E1032P002";
static const char data_3DFBE[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_3DFC8[10] __attribute__((scn_string_literal)) = "E1032P003";
static const char data_3DFD2[10] __attribute__((scn_string_literal)) = "E1032A141";
static const char data_3DFDC[10] __attribute__((scn_string_literal)) = "E1032P004";
static const char data_3DFE6[10] __attribute__((scn_string_literal)) = "E1032A142";
static const char data_3DFF0[10] __attribute__((scn_string_literal)) = "E1032P005";
static const char data_3DFFA[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_3E004[10] __attribute__((scn_string_literal)) = "E1032P001";
static const char data_3E00E[10] __attribute__((scn_string_literal)) = "E1032A085";
static const char data_3E018[10] __attribute__((scn_string_literal)) = "E1032P006";
static const char data_3E022[10] __attribute__((scn_string_literal)) = "E1032A143";
static const char data_3E02C[10] __attribute__((scn_string_literal)) = "E1032P007";
static const char data_3E036[10] __attribute__((scn_string_literal)) = "E1032A144";
static const char data_3E040[10] __attribute__((scn_string_literal)) = "E1032P004";
static const char data_3E04A[10] __attribute__((scn_string_literal)) = "E1032O008";
static const char data_3E054[10] __attribute__((scn_string_literal)) = "E1032P008";
static const char data_3E05E[10] __attribute__((scn_string_literal)) = "E1032A068";
static const char data_3E068[10] __attribute__((scn_string_literal)) = "E1032P009";
static const char data_3E072[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_3E07C[10] __attribute__((scn_string_literal)) = "E1032A085";
static const char data_3E086[10] __attribute__((scn_string_literal)) = "E1032P010";
static const char data_3E090[10] __attribute__((scn_string_literal)) = "E1032A145";
static const char data_3E09A[10] __attribute__((scn_string_literal)) = "E1032P011";
static const char data_3E0A4[10] __attribute__((scn_string_literal)) = "E1032A146";
static const char data_3E0AE[10] __attribute__((scn_string_literal)) = "E1032P012";
static const char data_3E0B8[10] __attribute__((scn_string_literal)) = "E1032A147";
static const char data_3E0C2[10] __attribute__((scn_string_literal)) = "E1032P013";
static const char data_3E0CC[10] __attribute__((scn_string_literal)) = "E1032A148";
static const char data_3E0D6[10] __attribute__((scn_string_literal)) = "E1032P014";
static const char data_3E0E0[10] __attribute__((scn_string_literal)) = "E1032A149";
static const char data_3E0EA[10] __attribute__((scn_string_literal)) = "E1032P015";
static const char data_3E0F4[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_3E0FE[10] __attribute__((scn_string_literal)) = "E1032P016";
static const char data_3E108[10] __attribute__((scn_string_literal)) = "E1032A068";
static const char data_3E112[10] __attribute__((scn_string_literal)) = "E1032P017";
static const char data_3E11C[10] __attribute__((scn_string_literal)) = "E1032A085";
static const char data_3E126[10] __attribute__((scn_string_literal)) = "E1032P018";
static const char data_3E130[10] __attribute__((scn_string_literal)) = "E1032A150";
static const char data_3E13A[10] __attribute__((scn_string_literal)) = "E1032P019";
static const char data_3E144[10] __attribute__((scn_string_literal)) = "E1032A151";
static const char data_3E14E[10] __attribute__((scn_string_literal)) = "E1032P004";
static const char data_3E158[10] __attribute__((scn_string_literal)) = "E1032P016";
static const char data_3E162[10] __attribute__((scn_string_literal)) = "E1032A068";
static const char data_3E16C[10] __attribute__((scn_string_literal)) = "E1032P017";
static const char data_3E176[10] __attribute__((scn_string_literal)) = "E1032P021";
static const char data_3E180[10] __attribute__((scn_string_literal)) = "E1032A152";
static const char data_3E18A[10] __attribute__((scn_string_literal)) = "E1032P010";
static const char data_3E194[10] __attribute__((scn_string_literal)) = "E1032A153";
static const char data_3E19E[10] __attribute__((scn_string_literal)) = "E1032P022";
static const char data_3E1A8[10] __attribute__((scn_string_literal)) = "E1032A154";
static const char data_3E1B2[10] __attribute__((scn_string_literal)) = "E1032P023";
static const char data_3E1BC[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_3E1C6[10] __attribute__((scn_string_literal)) = "E1032P024";
static const char data_3E1D0[10] __attribute__((scn_string_literal)) = "E1032A068";
static const char data_3E1DA[10] __attribute__((scn_string_literal)) = "E1032P017";
static const char data_3E1E4[10] __attribute__((scn_string_literal)) = "E1032P001";
static const char data_3E1EE[10] __attribute__((scn_string_literal)) = "E1032A085";
static const char data_3E1F8[10] __attribute__((scn_string_literal)) = "E1032P025";
static const char data_3E202[10] __attribute__((scn_string_literal)) = "E1032A155";
static const char data_3E20C[10] __attribute__((scn_string_literal)) = "E1032P004";
static const char data_3E216[10] __attribute__((scn_string_literal)) = "E1032A156";
static const char data_3E220[10] __attribute__((scn_string_literal)) = "E1032O008";
static const char data_3E22A[10] __attribute__((scn_string_literal)) = "E1032P008";
static const char data_3E234[10] __attribute__((scn_string_literal)) = "E1032A154";
static const char data_3E23E[10] __attribute__((scn_string_literal)) = "E1032P027";
static const char data_3E248[10] __attribute__((scn_string_literal)) = "E1032A068";
static const char data_3E252[10] __attribute__((scn_string_literal)) = "E1032P017";
static const char data_3E25C[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_3E266[10] __attribute__((scn_string_literal)) = "E1032P021";
static const char data_3E270[10] __attribute__((scn_string_literal)) = "E1032A152";
static const char data_3E27A[10] __attribute__((scn_string_literal)) = "E1032P028";
static const char data_3E284[10] __attribute__((scn_string_literal)) = "E1032A158";
static const char data_3E28E[10] __attribute__((scn_string_literal)) = "E1032P030";
static const char data_3E298[10] __attribute__((scn_string_literal)) = "E1032A159";
static const char data_3E2A2[10] __attribute__((scn_string_literal)) = "E1032P066";
static const char data_3E2AC[10] __attribute__((scn_string_literal)) = "E1032P016";
static const char data_3E2B6[10] __attribute__((scn_string_literal)) = "E1032A068";
static const char data_3E2C0[10] __attribute__((scn_string_literal)) = "E1032P017";
static const char data_3E2CA[10] __attribute__((scn_string_literal)) = "E1032P021";
static const char data_3E2D4[10] __attribute__((scn_string_literal)) = "E1032A152";
static const char data_3E2DE[10] __attribute__((scn_string_literal)) = "E1032P032";
static const char data_3E2E8[10] __attribute__((scn_string_literal)) = "E1032A160";
static const char data_3E2F2[10] __attribute__((scn_string_literal)) = "E1032P033";
static const char data_3E2FC[10] __attribute__((scn_string_literal)) = "E1032P034";
static const char data_3E306[10] __attribute__((scn_string_literal)) = "E1032P080";
static const char data_3E310[10] __attribute__((scn_string_literal)) = "E1032P081";
static const char data_3E31A[10] __attribute__((scn_string_literal)) = "E1032A068";
static const char data_3E324[10] __attribute__((scn_string_literal)) = "E1032P035";
static const char data_3E32E[10] __attribute__((scn_string_literal)) = "E1032A141";
static const char data_3E338[10] __attribute__((scn_string_literal)) = "E1032P036";
static const char data_3E342[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_3E34C[10] __attribute__((scn_string_literal)) = "E1032P021";
static const char data_3E356[10] __attribute__((scn_string_literal)) = "E1032A152";
static const char data_3E360[10] __attribute__((scn_string_literal)) = "E1032P025";
static const char data_3E36A[10] __attribute__((scn_string_literal)) = "E1032A160";
static const char data_3E374[10] __attribute__((scn_string_literal)) = "E1032P033";
static const char data_3E37E[10] __attribute__((scn_string_literal)) = "E1032P082";
static const char data_3E388[10] __attribute__((scn_string_literal)) = "E1032P083";
static const char data_3E392[10] __attribute__((scn_string_literal)) = "E1032P084";
static const char data_3E39C[10] __attribute__((scn_string_literal)) = "E1032P085";
static const char data_3E3A6[10] __attribute__((scn_string_literal)) = "E1032A068";
static const char data_3E3B0[10] __attribute__((scn_string_literal)) = "E1032P086";
static const char data_3E3BA[10] __attribute__((scn_string_literal)) = "E1032P001";
static const char data_3E3C4[10] __attribute__((scn_string_literal)) = "E1032A164";
static const char data_3E3CE[10] __attribute__((scn_string_literal)) = "E1032P018";
static const char data_3E3D8[10] __attribute__((scn_string_literal)) = "E1032A165";
static const char data_3E3E2[10] __attribute__((scn_string_literal)) = "E1032P037";
static const char data_3E3EC[10] __attribute__((scn_string_literal)) = "E1032A104";
static const char data_3E3F6[10] __attribute__((scn_string_literal)) = "E1032P120";
static const char data_3E400[10] __attribute__((scn_string_literal)) = "E1032A173";
static const char data_3E40A[10] __attribute__((scn_string_literal)) = "E1032O008";
static const char data_3E414[10] __attribute__((scn_string_literal)) = "E1032P008";
static const char data_3E41E[10] __attribute__((scn_string_literal)) = "E1032A154";
static const char data_3E428[10] __attribute__((scn_string_literal)) = "E1032P027";
static const char data_3E432[10] __attribute__((scn_string_literal)) = "E1032A068";
static const char data_3E43C[10] __attribute__((scn_string_literal)) = "E1032P017";
static const char data_3E446[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_3E450[10] __attribute__((scn_string_literal)) = "E1032P001";
static const char data_3E45A[10] __attribute__((scn_string_literal)) = "E1032A164";
static const char data_3E464[10] __attribute__((scn_string_literal)) = "E1032P018";
static const char data_3E46E[10] __attribute__((scn_string_literal)) = "E1032A170";
static const char data_3E478[10] __attribute__((scn_string_literal)) = "E1032P044";
static const char data_3E482[10] __attribute__((scn_string_literal)) = "E1032A200";
static const char data_3E48C[10] __attribute__((scn_string_literal)) = "E1032A172";
static const char data_3E496[10] __attribute__((scn_string_literal)) = "E1032P045";
static const char data_3E4A0[10] __attribute__((scn_string_literal)) = "E1032A173";
static const char data_3E4AA[10] __attribute__((scn_string_literal)) = "E1032P046";
static const char data_3E4B4[10] __attribute__((scn_string_literal)) = "E1032P047";
static const char data_3E4BE[10] __attribute__((scn_string_literal)) = "E1032A201";
static const char data_3E4C8[10] __attribute__((scn_string_literal)) = "E1032P049";
static const char data_3E4D2[10] __attribute__((scn_string_literal)) = "E1032A176";
static const char data_3E4DC[10] __attribute__((scn_string_literal)) = "E1032P050";
static const char data_3E4E6[10] __attribute__((scn_string_literal)) = "E1032A177";
static const char data_3E4F0[10] __attribute__((scn_string_literal)) = "E1032P051";
static const char data_3E4FA[10] __attribute__((scn_string_literal)) = "E1032P001";
static const char data_3E504[10] __attribute__((scn_string_literal)) = "E1032A164";
static const char data_3E50E[10] __attribute__((scn_string_literal)) = "E1032P018";
static const char data_3E518[10] __attribute__((scn_string_literal)) = "E1032A178";
static const char data_3E522[10] __attribute__((scn_string_literal)) = "E1032P004";
static const char data_3E52C[10] __attribute__((scn_string_literal)) = "E1032P052";
static const char data_3E536[10] __attribute__((scn_string_literal)) = "E1032A179";
static const char data_3E540[10] __attribute__((scn_string_literal)) = "E1032A180";
static const char data_3E54A[10] __attribute__((scn_string_literal)) = "E1032A181";
static const char data_3E554[10] __attribute__((scn_string_literal)) = "E1032P053";
static const char data_3E55E[10] __attribute__((scn_string_literal)) = "E1032P087";
static const char data_3E568[10] __attribute__((scn_string_literal)) = "E1032P055";
static const char data_3E572[10] __attribute__((scn_string_literal)) = "E1032A182";
static const char data_3E57C[10] __attribute__((scn_string_literal)) = "E1032P051";
static const char data_3E586[10] __attribute__((scn_string_literal)) = "E1032P001";
static const char data_3E590[10] __attribute__((scn_string_literal)) = "E1032A164";
static const char data_3E59A[10] __attribute__((scn_string_literal)) = "E1032P018";
static const char data_3E5A4[10] __attribute__((scn_string_literal)) = "E1032A183";
static const char data_3E5AE[10] __attribute__((scn_string_literal)) = "E1032P038";
static const char data_3E5B8[10] __attribute__((scn_string_literal)) = "E1032A184";
static const char data_3E5C2[10] __attribute__((scn_string_literal)) = "E1032P056";
static const char data_3E5CC[10] __attribute__((scn_string_literal)) = "E1032P088";
static const char data_3E5D6[10] __attribute__((scn_string_literal)) = "E1032P058";
static const char data_3E5E0[10] __attribute__((scn_string_literal)) = "E1032A220";
static const char data_3E5EA[10] __attribute__((scn_string_literal)) = "E1032A185";
static const char data_3E5F4[10] __attribute__((scn_string_literal)) = "E1032P059";
static const char data_3E5FE[10] __attribute__((scn_string_literal)) = "E1032P001";
static const char data_3E608[10] __attribute__((scn_string_literal)) = "E1032A164";
static const char data_3E612[10] __attribute__((scn_string_literal)) = "E1032P018";
static const char data_3E61C[10] __attribute__((scn_string_literal)) = "E1032P060";
static const char data_3E626[10] __attribute__((scn_string_literal)) = "E1032A186";
static const char data_3E630[10] __attribute__((scn_string_literal)) = "E1032P061";
static const char data_3E63A[10] __attribute__((scn_string_literal)) = "E1032A187";
static const char data_3E644[10] __attribute__((scn_string_literal)) = "E1032P062";
static const char data_3E64E[10] __attribute__((scn_string_literal)) = "E1032A188";
static const char data_3E658[10] __attribute__((scn_string_literal)) = "E1032P063";
static const char data_3E662[10] __attribute__((scn_string_literal)) = "E1032P064";
static const char data_3E66C[10] __attribute__((scn_string_literal)) = "E1032P065";
static const char data_3E676[10] __attribute__((scn_string_literal)) = "E1032A189";
static const char data_3E680[10] __attribute__((scn_string_literal)) = "E1032P066";
static const char data_3E68A[10] __attribute__((scn_string_literal)) = "E1032A190";
static const char data_3E694[10] __attribute__((scn_string_literal)) = "E1032P001";
static const char data_3E69E[10] __attribute__((scn_string_literal)) = "E1032A152";
static const char data_3E6A8[10] __attribute__((scn_string_literal)) = "E1032P067";
static const char data_3E6B2[10] __attribute__((scn_string_literal)) = "E1032A191";
static const char data_3E6BC[10] __attribute__((scn_string_literal)) = "E1032A192";
static const char data_3E6C6[10] __attribute__((scn_string_literal)) = "E1032P068";
static const char data_3E6D0[10] __attribute__((scn_string_literal)) = "E1032P069";
static const char data_3E6DA[10] __attribute__((scn_string_literal)) = "E1032A193";
static const char data_3E6E4[10] __attribute__((scn_string_literal)) = "E1032P001";
static const char data_3E6EE[10] __attribute__((scn_string_literal)) = "E1032A152";
static const char data_3E6F8[10] __attribute__((scn_string_literal)) = "E1032P070";
static const char data_3E702[10] __attribute__((scn_string_literal)) = "E1032A194";
static const char data_3E70C[10] __attribute__((scn_string_literal)) = "E1032P071";
static const char data_3E716[10] __attribute__((scn_string_literal)) = "E1032A195";
static const char data_3E720[10] __attribute__((scn_string_literal)) = "E1032P072";
static const char data_3E72A[10] __attribute__((scn_string_literal)) = "E1032P073";
static const char data_3E734[10] __attribute__((scn_string_literal)) = "E1032A151";
static const char data_3E73E[10] __attribute__((scn_string_literal)) = "E1032P074";
static const char data_3E748[10] __attribute__((scn_string_literal)) = "E1032P075";
static const char data_3E752[10] __attribute__((scn_string_literal)) = "E1032P089";
static const char data_3E75C[10] __attribute__((scn_string_literal)) = "E1032P090";
static const char data_3E766[10] __attribute__((scn_string_literal)) = "E1032A196";
static const char data_3E770[10] __attribute__((scn_string_literal)) = "E1032P079";
static const char data_3E77A[10] __attribute__((scn_string_literal)) = "E1032A197";
static const char data_3E784[10] __attribute__((scn_string_literal)) = "E1032P001";
static const char data_3E78E[10] __attribute__((scn_string_literal)) = "E1032A152";
static const char data_3E798[10] __attribute__((scn_string_literal)) = "E1032P010";
static const char data_3E7A2[10] __attribute__((scn_string_literal)) = "E1032P091";
static const char data_3E7AC[10] __attribute__((scn_string_literal)) = "E1032P092";
static const char data_3E7B6[10] __attribute__((scn_string_literal)) = "E1032A203";
static const char data_3E7C0[10] __attribute__((scn_string_literal)) = "E1032P093";
static const char data_3E7CA[10] __attribute__((scn_string_literal)) = "E1032P094";
static const char data_3E7D4[10] __attribute__((scn_string_literal)) = "E1032A204";
static const char data_3E7DE[10] __attribute__((scn_string_literal)) = "E1032P095";
static const char data_3E7E8[10] __attribute__((scn_string_literal)) = "E1032P021";
static const char data_3E7F2[10] __attribute__((scn_string_literal)) = "E1032A152";
static const char data_3E7FC[10] __attribute__((scn_string_literal)) = "E1032P096";
static const char data_3E806[10] __attribute__((scn_string_literal)) = "E1032A104";
static const char data_3E810[10] __attribute__((scn_string_literal)) = "E1032P097";
static const char data_3E81A[10] __attribute__((scn_string_literal)) = "E1032A103";
static const char data_3E824[10] __attribute__((scn_string_literal)) = "E1032P098";
static const char data_3E82E[10] __attribute__((scn_string_literal)) = "E1032P099";
static const char data_3E838[10] __attribute__((scn_string_literal)) = "E1032P100";
static const char data_3E842[10] __attribute__((scn_string_literal)) = "E1032A206";
static const char data_3E84C[10] __attribute__((scn_string_literal)) = "E1032P021";
static const char data_3E856[10] __attribute__((scn_string_literal)) = "E1032A152";
static const char data_3E860[10] __attribute__((scn_string_literal)) = "E1032P101";
static const char data_3E86A[10] __attribute__((scn_string_literal)) = "E1032A207";
static const char data_3E874[10] __attribute__((scn_string_literal)) = "E1032P102";
static const char data_3E87E[10] __attribute__((scn_string_literal)) = "E1032A208";
static const char data_3E888[10] __attribute__((scn_string_literal)) = "E1032P103";
static const char data_3E892[10] __attribute__((scn_string_literal)) = "E1032A209";
static const char data_3E89C[10] __attribute__((scn_string_literal)) = "E1032P104";
static const char data_3E8A6[10] __attribute__((scn_string_literal)) = "E1032P105";
static const char data_3E8B0[10] __attribute__((scn_string_literal)) = "E1032A210";
static const char data_3E8BA[10] __attribute__((scn_string_literal)) = "E1032P106";
static const char data_3E8C4[10] __attribute__((scn_string_literal)) = "E1032A154";
static const char data_3E8CE[10] __attribute__((scn_string_literal)) = "E1032P107";
static const char data_3E8D8[10] __attribute__((scn_string_literal)) = "E1032A204";
static const char data_3E8E2[10] __attribute__((scn_string_literal)) = "E1032P108";
static const char data_3E8EC[10] __attribute__((scn_string_literal)) = "E1032P109";
static const char data_3E8F6[10] __attribute__((scn_string_literal)) = "E1032A085";
static const char data_3E900[10] __attribute__((scn_string_literal)) = "E1032P110";
static const char data_3E90A[10] __attribute__((scn_string_literal)) = "E1032A211";
static const char data_3E914[10] __attribute__((scn_string_literal)) = "E1032P111";
static const char data_3E91E[10] __attribute__((scn_string_literal)) = "E1032A104";
static const char data_3E928[10] __attribute__((scn_string_literal)) = "E1032P112";
static const char data_3E932[10] __attribute__((scn_string_literal)) = "E1032A212";
static const char data_3E93C[10] __attribute__((scn_string_literal)) = "E1032P113";
static const char data_3E946[10] __attribute__((scn_string_literal)) = "E1032A158";
static const char data_3E950[10] __attribute__((scn_string_literal)) = "E1032P114";
static const char data_3E95A[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_3E964[10] __attribute__((scn_string_literal)) = "E1032P115";
static const char data_3E96E[10] __attribute__((scn_string_literal)) = "E1032A213";
static const char data_3E978[10] __attribute__((scn_string_literal)) = "E1032P116";
static const char data_3E982[10] __attribute__((scn_string_literal)) = "E1032P001";
static const char data_3E98C[10] __attribute__((scn_string_literal)) = "E1032A152";
static const char data_3E996[10] __attribute__((scn_string_literal)) = "E1032P117";
static const char data_3E9A0[10] __attribute__((scn_string_literal)) = "E1032A214";
static const char data_3E9AA[10] __attribute__((scn_string_literal)) = "E1032P118";
static const char data_3E9B4[10] __attribute__((scn_string_literal)) = "E1032A154";
static const char data_3E9BE[10] __attribute__((scn_string_literal)) = "E1032P119";
static const char data_3E9C8[10] __attribute__((scn_string_literal)) = "E1032P120";
static const char data_3E9D2[10] __attribute__((scn_string_literal)) = "E1032A215";
static const char data_3E9DC[10] __attribute__((scn_string_literal)) = "E1032P121";
static const char data_3E9E6[10] __attribute__((scn_string_literal)) = "E1032A163";
static const char data_3E9F0[10] __attribute__((scn_string_literal)) = "E1032P123";
static const char data_3E9FA[10] __attribute__((scn_string_literal)) = "E1032A217";
static const char data_3EA04[10] __attribute__((scn_string_literal)) = "E1032P001";
static const char data_3EA0E[10] __attribute__((scn_string_literal)) = "E1032A152";
static const char data_3EA18[10] __attribute__((scn_string_literal)) = "E1032P010";
static const char data_3EA22[10] __attribute__((scn_string_literal)) = "E1032P124";
static const char data_3EA2C[10] __attribute__((scn_string_literal)) = "E1032P125";
static const char data_3EA36[10] __attribute__((scn_string_literal)) = "E1032A218";
static const char data_3EA40[10] __attribute__((scn_string_literal)) = "E1032P126";
static const char data_3EA4A[10] __attribute__((scn_string_literal)) = "E1032A154";
static const char data_3EA54[10] __attribute__((scn_string_literal)) = "E1032P127";
static const char data_3EA5E[10] __attribute__((scn_string_literal)) = "E1032A213";
static const char data_3EA68[10] __attribute__((scn_string_literal)) = "E1032P128";
static const char data_3EA72[10] __attribute__((scn_string_literal)) = "E1032A204";
static const char data_3EA7C[10] __attribute__((scn_string_literal)) = "E1032P116";
static const char data_3EA86[10] __attribute__((scn_string_literal)) = "E1032P109";
static const char data_3EA90[10] __attribute__((scn_string_literal)) = "E1032A085";
static const char data_3EA9A[10] __attribute__((scn_string_literal)) = "E1032P117";
static const char data_3EAA4[10] __attribute__((scn_string_literal)) = "E1032A214";
static const char data_3EAAE[10] __attribute__((scn_string_literal)) = "E1032P129";
static const char data_3EAB8[10] __attribute__((scn_string_literal)) = "E1032P130";
static const char data_3EAC2[10] __attribute__((scn_string_literal)) = "E1032P131";
static const char data_3EACC[10] __attribute__((scn_string_literal)) = "E1032A213";
static const char data_3EAD6[10] __attribute__((scn_string_literal)) = "E1032A219";
static const char data_3EAE0[10] __attribute__((scn_string_literal)) = "E1032P132";
static const char data_3EAEA[10] __attribute__((scn_string_literal)) = "E1032A153";
static const char data_3EAF4[10] __attribute__((scn_string_literal)) = "E1032P133";
static const char data_3EAFE[10] __attribute__((scn_string_literal)) = "E1032A220";
static const char data_3EB08[10] __attribute__((scn_string_literal)) = "E1032P134";
static const char data_3EB12[10] __attribute__((scn_string_literal)) = "E1032A204";
static const char data_3EB1C[10] __attribute__((scn_string_literal)) = "E1032P116";
static const char data_3EB26[10] __attribute__((scn_string_literal)) = "E1032A085";
static const char data_3EB30[10] __attribute__((scn_string_literal)) = "E1032P010";
static const char data_3EB3A[10] __attribute__((scn_string_literal)) = "E1032A221";
static const char data_3EB44[10] __attribute__((scn_string_literal)) = "E1032P135";
static const char data_3EB4E[10] __attribute__((scn_string_literal)) = "E1032P136";
static const char data_3EB58[10] __attribute__((scn_string_literal)) = "E1032A222";
static const char data_3EB62[10] __attribute__((scn_string_literal)) = "E1032P137";
static const char data_3EB6C[10] __attribute__((scn_string_literal)) = "E1032P138";
static const char data_3EB76[10] __attribute__((scn_string_literal)) = "E1032A223";
static const char data_3EB80[10] __attribute__((scn_string_literal)) = "E1032P139";
static const char data_3EB8A[10] __attribute__((scn_string_literal)) = "E1032A224";
static const char data_3EB94[10] __attribute__((scn_string_literal)) = "E1032P140";
static const char data_3EB9E[10] __attribute__((scn_string_literal)) = "E1032A225";
static const char data_3EBA8[10] __attribute__((scn_string_literal)) = "E1032A226";
static const char data_3EBB2[10] __attribute__((scn_string_literal)) = "E1032P004";
static const char data_3EBBC[10] __attribute__((scn_string_literal)) = "E1032E001";
static const char data_3EBC6[10] __attribute__((scn_string_literal)) = "E1032E002";
static const char data_3EBD0[10] __attribute__((scn_string_literal)) = "E1032E003";
static const char data_3EBDA[10] __attribute__((scn_string_literal)) = "E1032A010";
static const char data_3EBE4[10] __attribute__((scn_string_literal)) = "E1032E004";
static const char data_3EBEE[10] __attribute__((scn_string_literal)) = "E1032A011";
static const char data_3EBF8[10] __attribute__((scn_string_literal)) = "E1032E005";
static const char data_3EC02[10] __attribute__((scn_string_literal)) = "E1032A012";
static const char data_3EC0C[10] __attribute__((scn_string_literal)) = "E1032A013";
static const char data_3EC16[10] __attribute__((scn_string_literal)) = "E1032E006";
static const char data_3EC20[10] __attribute__((scn_string_literal)) = "E1032A014";
static const char data_3EC2A[10] __attribute__((scn_string_literal)) = "E1032A015";
static const char data_3EC34[10] __attribute__((scn_string_literal)) = "E1032E007";
static const char data_3EC3E[10] __attribute__((scn_string_literal)) = "E1032A016";
static const char data_3EC48[10] __attribute__((scn_string_literal)) = "E1032E008";
static const char data_3EC52[10] __attribute__((scn_string_literal)) = "E1032E009";
static const char data_3EC5C[10] __attribute__((scn_string_literal)) = "E1032A017";
static const char data_3EC66[10] __attribute__((scn_string_literal)) = "E1032E010";
static const char data_3EC70[10] __attribute__((scn_string_literal)) = "E1032E011";
static const char data_3EC7A[10] __attribute__((scn_string_literal)) = "E1032E012";
static const char data_3EC84[10] __attribute__((scn_string_literal)) = "E1032E013";
static const char data_3EC8E[10] __attribute__((scn_string_literal)) = "E1032A018";
static const char data_3EC98[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_3ECA2[10] __attribute__((scn_string_literal)) = "E1032A020";
static const char data_3ECAC[10] __attribute__((scn_string_literal)) = "E1032A021";
static const char data_3ECB6[10] __attribute__((scn_string_literal)) = "E1032F001";
static const char data_3ECC0[10] __attribute__((scn_string_literal)) = "E1032F002";
static const char data_3ECCA[10] __attribute__((scn_string_literal)) = "E1032F003";
static const char data_3ECD4[10] __attribute__((scn_string_literal)) = "E1032A022";
static const char data_3ECDE[10] __attribute__((scn_string_literal)) = "E1032F004";
static const char data_3ECE8[10] __attribute__((scn_string_literal)) = "E1032A011";
static const char data_3ECF2[10] __attribute__((scn_string_literal)) = "E1032F005";
static const char data_3ECFC[10] __attribute__((scn_string_literal)) = "E1032A012";
static const char data_3ED06[10] __attribute__((scn_string_literal)) = "E1032A013";
static const char data_3ED10[10] __attribute__((scn_string_literal)) = "E1032F006";
static const char data_3ED1A[10] __attribute__((scn_string_literal)) = "E1032A023";
static const char data_3ED24[10] __attribute__((scn_string_literal)) = "E1032A024";
static const char data_3ED2E[10] __attribute__((scn_string_literal)) = "E1032F007";
static const char data_3ED38[10] __attribute__((scn_string_literal)) = "E1032A025";
static const char data_3ED42[10] __attribute__((scn_string_literal)) = "E1032A026";
static const char data_3ED4C[10] __attribute__((scn_string_literal)) = "E1032F008";
static const char data_3ED56[10] __attribute__((scn_string_literal)) = "E1032F009";
static const char data_3ED60[10] __attribute__((scn_string_literal)) = "E1032F010";
static const char data_3ED6A[10] __attribute__((scn_string_literal)) = "E1032F011";
static const char data_3ED74[10] __attribute__((scn_string_literal)) = "E1032F012";
static const char data_3ED7E[10] __attribute__((scn_string_literal)) = "E1032F013";
static const char data_3ED88[10] __attribute__((scn_string_literal)) = "E1032F014";
static const char data_3ED92[10] __attribute__((scn_string_literal)) = "E1032F015";
static const char data_3ED9C[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_3EDA6[10] __attribute__((scn_string_literal)) = "E1032A018";
static const char data_3EDB0[10] __attribute__((scn_string_literal)) = "E1032A027";
static const char data_3EDBA[10] __attribute__((scn_string_literal)) = "E1032A034";
static const char data_3EDC4[10] __attribute__((scn_string_literal)) = "E1032H005";
static const char data_3EDCE[10] __attribute__((scn_string_literal)) = "E1032H004";
static const char data_3EDD8[10] __attribute__((scn_string_literal)) = "E1032H009";
static const char data_3EDE2[10] __attribute__((scn_string_literal)) = "E1032H006";
static const char data_3EDEC[10] __attribute__((scn_string_literal)) = "E1032A042";
static const char data_3EDF6[10] __attribute__((scn_string_literal)) = "E1032A040";
static const char data_3EE00[10] __attribute__((scn_string_literal)) = "E1032A043";
static const char data_3EE0A[10] __attribute__((scn_string_literal)) = "E1032A041";
static const char data_3EE14[10] __attribute__((scn_string_literal)) = "E1032A045";
static const char data_3EE1E[10] __attribute__((scn_string_literal)) = "E1032A046";
static const char data_3EE28[10] __attribute__((scn_string_literal)) = "E1032A047";
static const char data_3EE32[10] __attribute__((scn_string_literal)) = "E1032A046";
static const char data_3EE3C[10] __attribute__((scn_string_literal)) = "E1032A047";
static const char data_3EE46[10] __attribute__((scn_string_literal)) = "E1032A044";
static const char data_3EE50[10] __attribute__((scn_string_literal)) = "E1032A051";
static const char data_3EE5A[10] __attribute__((scn_string_literal)) = "E1032A039";
static const char data_3EE64[10] __attribute__((scn_string_literal)) = "E1032H007";
static const char data_3EE6E[10] __attribute__((scn_string_literal)) = "E1032A120";
static const char data_3EE78[10] __attribute__((scn_string_literal)) = "E1032H010";
static const char data_3EE82[10] __attribute__((scn_string_literal)) = "E1032A066";
static const char data_3EE8C[10] __attribute__((scn_string_literal)) = "E1032H011";
static const char data_3EE96[10] __attribute__((scn_string_literal)) = "E1032J001";
static const char data_3EEA0[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_3EEAA[10] __attribute__((scn_string_literal)) = "E1032A131";
static const char data_3EEB4[10] __attribute__((scn_string_literal)) = "E1032J010";
static const char data_3EEBE[10] __attribute__((scn_string_literal)) = "E1032J013";
static const char data_3EEC8[10] __attribute__((scn_string_literal)) = "E1032J001";
static const char data_3EED2[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_3EEDC[10] __attribute__((scn_string_literal)) = "E1032A134";
static const char data_3EEE6[10] __attribute__((scn_string_literal)) = "E1032J015";
static const char data_3EEF0[10] __attribute__((scn_string_literal)) = "E1032A135";
static const char data_3EEFA[10] __attribute__((scn_string_literal)) = "E1032J001";
static const char data_3EF04[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_3EF0E[10] __attribute__((scn_string_literal)) = "E1032J018";
static const char data_3EF18[10] __attribute__((scn_string_literal)) = "E1032A163";
static const char data_3EF22[10] __attribute__((scn_string_literal)) = "E1032J019";
static const char data_3EF2C[10] __attribute__((scn_string_literal)) = "E1032J020";
static const char data_3EF36[10] __attribute__((scn_string_literal)) = "E1032A068";
static const char data_3EF40[10] __attribute__((scn_string_literal)) = "E1032J016";
static const char data_3EF4A[10] __attribute__((scn_string_literal)) = "E1032A161";
static const char data_3EF54[10] __attribute__((scn_string_literal)) = "E1032J017";
static const char data_3EF5E[10] __attribute__((scn_string_literal)) = "E1032A162";
static const char data_3EF68[10] __attribute__((scn_string_literal)) = "E1032J001";
static const char data_3EF72[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_3EF7C[10] __attribute__((scn_string_literal)) = "E1032A131";
static const char data_3EF86[10] __attribute__((scn_string_literal)) = "E1032J010";
static const char data_3EF90[10] __attribute__((scn_string_literal)) = "E1032A132";
static const char data_3EF9A[10] __attribute__((scn_string_literal)) = "E1032J011";
static const char data_3EFA4[10] __attribute__((scn_string_literal)) = "E1032J012";
static const char data_3EFAE[10] __attribute__((scn_string_literal)) = "E1032H007";
static const char data_3EFB8[10] __attribute__((scn_string_literal)) = "E1032A120";
static const char data_3EFC2[10] __attribute__((scn_string_literal)) = "E1032H010";
static const char data_3EFCC[10] __attribute__((scn_string_literal)) = "E1032A066";
static const char data_3EFD6[10] __attribute__((scn_string_literal)) = "E1032H011";
static const char data_3EFE0[10] __attribute__((scn_string_literal)) = "E1032J001";
static const char data_3EFEA[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_3EFF4[10] __attribute__((scn_string_literal)) = "E1032J018";
static const char data_3EFFE[10] __attribute__((scn_string_literal)) = "E1032A163";
static const char data_3F008[10] __attribute__((scn_string_literal)) = "E1032J019";
static const char data_3F012[10] __attribute__((scn_string_literal)) = "E1032J020";
static const char data_3F01C[10] __attribute__((scn_string_literal)) = "E1032A068";
static const char data_3F026[10] __attribute__((scn_string_literal)) = "E1032J016";
static const char data_3F030[10] __attribute__((scn_string_literal)) = "E1032A161";
static const char data_3F03A[10] __attribute__((scn_string_literal)) = "E1032J017";
static const char data_3F044[10] __attribute__((scn_string_literal)) = "E1032A162";
static const char data_3F04E[10] __attribute__((scn_string_literal)) = "E1032J001";
static const char data_3F058[10] __attribute__((scn_string_literal)) = "E1032A067";
static const char data_3F062[10] __attribute__((scn_string_literal)) = "E1032J002";
static const char data_3F06C[10] __attribute__((scn_string_literal)) = "E1032A068";
static const char data_3F076[10] __attribute__((scn_string_literal)) = "E1032J001";
static const char data_3F080[10] __attribute__((scn_string_literal)) = "E1032A019";
static const char data_3F08A[10] __attribute__((scn_string_literal)) = "E1032J003";
static const char data_3F094[10] __attribute__((scn_string_literal)) = "E1032A069";
static const char data_3F09E[10] __attribute__((scn_string_literal)) = "E1032J004";
static const char data_3F0A8[10] __attribute__((scn_string_literal)) = "E1032A070";
static const char data_3F0B2[10] __attribute__((scn_string_literal)) = "E1032H007";
static const char data_3F0BC[10] __attribute__((scn_string_literal)) = "E1032A048";
static const char data_3F0C6[10] __attribute__((scn_string_literal)) = "E1032H008";
static const char data_3F0D0[10] __attribute__((scn_string_literal)) = "E1032A049";
static const char data_3F0DA[10] __attribute__((scn_string_literal)) = "E1032A050";
static const char data_3F0E4[20] __attribute__((scn_string_literal)) = "JOMO_AKIRA = %d\012";
static void (*const data_3F0F8)(void) = script_17D3A;
static void (*const data_3F0FC)(void) = script_17D3A;
static void (*const data_3F100)(void) = script_17D3A;
static void (*const data_3F104)(void) = script_17D3A;
static void (*const data_3F108)(void) = script_17D3A;
static void (*const data_3F10C)(void) = script_17D3A;
static void (*const data_3F110)(void) = script_17D3A;
static void (*const data_3F114)(void) = script_17D3A;
static void (*const data_3F118)(void) = script_17D3A;
static void (*const data_3F11C)(void) = script_17D3A;
static void (*const data_3F120)(void) = script_17D3A;
static void (*const data_3F124)(void) = script_17D3A;
static void (*const data_3F128)(void) = script_17D3A;
static void (*const data_3F12C)(void) = script_17D3A;
static void (*const data_3F130)(void) = script_17D3A;
static void (*const data_3F134)(void) = script_17D3A;
static void (*const data_3F138)(void) = script_17D3A;
static void (*const data_3F13C)(void) = script_17D3A;
static void (*const data_3F140)(void) = script_17D3A;
static void (*const data_3F144)(void) = script_17D3A;
static void (*const data_3F148)(void) = script_17D3A;
static void (*const data_3F14C)(void) = script_17D3A;
static void (*const data_3F150)(void) = script_17D3A;
static void (*const data_3F154)(void) = script_17D3A;
static void (*const data_3F158)(void) = script_17D3A;
static const uint32_t data_3F15C = 0xFFFFFFFF;
static const int32_t data_3F160[24] = {1, 9, 9, 9, 4, 9, 6, 8, 5, 8, 3, 8, 0, 2, 8, 7, 13, -1, -1, 10, 10, 12, 11, 11};
static const uint8_t data_3F1C0[4] = {0};
static const int32_t data_3F1C4[17] = {1, 1, 3, 2, 2, 4, 260, 260, 280, 268, 24, 400, 384, 96, 96, 96, 256};
static const uint8_t data_3F208[4] = {0};
static const int32_t data_3F20C[17] = {136, 136, 200, 200, 136, 468, 341, 469, 341, 341, 341, 341, 341, 341, 341, 341, 84};
static const uint8_t data_3F250[4] = {0};
static const float data_3F254[35] = {
  10.0f, 10.0f, 10.0f, 10.0f, 10.0f, 16.0f, 16.0f, 16.0f, 16.0f, 16.0f, 16.0f, 16.0f, 16.0f, 16.0f, 16.0f, 16.0f, 11.0f, 0.0f, 16.0f, 16.0f, 16.0f,
  16.0f, 16.0f, 16.0f, 16.0f, 16.0f, 16.0f, 16.0f, 16.0f, 16.0f, 16.0f, 16.0f, 16.0f, 16.0f, 16.0f
};
static const uint32_t data_3F2E0 = SHEN_NAME('IWKJ');
static const shen_name data_3F2E4[6] = {SHEN_NAME('IGAK'), SHEN_NAME('TRNE'), SHEN_NAME('IWWH'), SHEN_NAME('TEGA'), SHEN_NAME('ITBO'), SHEN_NAME('HIM1')};
static const uint32_t data_3F2FC[6] = {0x52627469U, 0x4C627469U, 0x6E656477U, 0x6E747377U, 0x68736F69U, 0x6F6B6869U};
static const int32_t data_3F314 = -1;
static const uint32_t data_3F318 = SHEN_NAME('AKD2');
static const shen_name data_3F31C[12] = {
  SHEN_NAME('AKD3'), SHEN_NAME('AKD4'), SHEN_NAME('ATS1'), SHEN_NAME('ATS2'), SHEN_NAME('ATS3'), SHEN_NAME('ATS4'), SHEN_NAME('ATS5'),
  SHEN_NAME('ATS6'), SHEN_NAME('ATS7'), SHEN_NAME('AKDG'), SHEN_NAME('FUT2'), SHEN_NAME('TOKE')
};
static const uint32_t data_3F34C[3] = {0x6E656472U, 0x6E747372U, 0x68736F72U};
static const int32_t data_3F358 = -1;
static const uint32_t data_3F35C = SHEN_NAME('BMKJ');
static const shen_name data_3F360[8] = {
  SHEN_NAME('KIKJ'), SHEN_NAME('BMBO'), SHEN_NAME('BMB1'), SHEN_NAME('BMB2'), SHEN_NAME('BMB3'), SHEN_NAME('BMB4'), SHEN_NAME('BMB5'),
  SHEN_NAME('BTBO')
};
static const uint32_t data_3F380 = 0x6E656462U;
static const shen_name data_3F384 = CHARA_HANA;
static const uint32_t data_3F388[3] = {0x52627462U, 0x4C627462U, 0x6E656474U};
static const int32_t data_3F394 = -1;
static const uint32_t data_3F398 = SHEN_NAME('DDZ1');
static const shen_name data_3F39C[13] = {
  SHEN_NAME('D1C1'), SHEN_NAME('D1C2'), SHEN_NAME('D1C3'), SHEN_NAME('D1C4'), SHEN_NAME('DDG1'), SHEN_NAME('DDG2'), SHEN_NAME('DDR1'),
  SHEN_NAME('DDR2'), SHEN_NAME('DND1'), SHEN_NAME('DND2'), SHEN_NAME('DGKI'), SHEN_NAME('DDYK'), SHEN_NAME('DDFP')
};
static const uint32_t data_3F3D0[3] = {0x6E65646BU, 0x6B6C636BU, 0x6369706BU};
static const int32_t data_3F3DC = -1;
static const uint32_t data_3F3E0 = SHEN_NAME('STAN');
static const uint32_t data_3F3E4[2] = {0x68736463U, 0x6E686463U};
static const shen_name data_3F3EC[7] = {
  SHEN_NAME('IMOR'), SHEN_NAME('IMC1'), SHEN_NAME('IMC2'), SHEN_NAME('IMC3'), SHEN_NAME('IMC4'), SHEN_NAME('IYNU'), SHEN_NAME('ITVH')
};
static const uint32_t data_3F408 = 0x6B6C636CU;
static const int32_t data_3F40C = -1;
static const uint32_t data_3F410 = SHEN_NAME('IKD1');
static const shen_name data_3F414[9] = {
  SHEN_NAME('IKD2'), SHEN_NAME('ITS1'), SHEN_NAME('ITS2'), SHEN_NAME('ITS3'), SHEN_NAME('ITS4'), SHEN_NAME('ITS5'), SHEN_NAME('ITS6'),
  SHEN_NAME('ITS7'), SHEN_NAME('HIM4')
};
static const uint32_t data_3F438[2] = {0x6E656469U, 0x75676B69U};
static const int32_t data_3F440 = -1;
static const uint32_t data_3F444 = SHEN_NAME('FGAK');
static const shen_name data_3F448[2] = {SHEN_NAME('FKWH'), SHEN_NAME('HIM3')};
static const uint32_t data_3F450[2] = {0x6E656466U, 0x6E747366U};
static const int32_t data_3F458 = -1;
static const uint32_t data_3F45C = 0x4C62746B;
static const uint32_t data_3F460 = 0x5262746BU;
static const shen_name data_3F464[2] = {SHEN_NAME('KGK1'), SHEN_NAME('KGK2')};
static const uint32_t data_3F46C[3] = {0x6E656467U, 0x68736F67U, 0x6B6C6367U};
static const int32_t data_3F478 = -1;
static const uint32_t data_3F47C = SHEN_NAME('TEL_');
static const shen_name data_3F480[10] = {
  SHEN_NAME('TGAK'), SHEN_NAME('TAN1'), SHEN_NAME('TAN2'), SHEN_NAME('GGB1'), SHEN_NAME('GGB2'), SHEN_NAME('RKUT'), SHEN_NAME('LKUT'),
  SHEN_NAME('TLBK'), SHEN_NAME('ATB1'), SHEN_NAME('ATB2')
};
static const uint32_t data_3F4A8[2] = {0x6E656465U, 0x6C616365U};
static const int32_t data_3F4B0 = -1;
static const uint32_t data_3F4B4[6] = {0x316E646CU, 0x326E646CU, 0x336E646CU, 0x6E656474U, 0x3163705FU, 0x3363705FU};
static const int32_t data_3F4CC = -1;
static const char data_3F4D0[5] __attribute__((scn_string_literal)) = "HANA";
static const char data_3F4D5[6] __attribute__((scn_string_literal)) = "HANA2";
static const char data_3F4DB[5] __attribute__((scn_string_literal)) = "HANA";
static const char data_3F4E0[6] __attribute__((scn_string_literal)) = "HANA2";
static const char data_3F4E6[9] __attribute__((scn_string_literal)) = "RADICA_R";
static const char data_3F4EF[9] __attribute__((scn_string_literal)) = "RADICASE";
static const char data_3F4F8[9] __attribute__((scn_string_literal)) = "RADICA_S";
static const char data_3F501[16] __attribute__((scn_string_literal)) = "AKIRA_ROOM_ %d\012";
static const char data_3F511[17] __attribute__((scn_string_literal)) = "JOMO_AKIRA = %d\012";
static const char data_3F522[17] __attribute__((scn_string_literal)) = "JOMO_AKIRA = %d\012";
static const char data_3F533[16] __attribute__((scn_string_literal)) = "AKIRA_ROOM_ %d\012";
static const char data_3F543[16] __attribute__((scn_string_literal)) = "AKIRA_ROOM_ %d\012";
static const char data_3F553[17] __attribute__((scn_string_literal)) = "JOMO_AKIRA = %d\012";
static const char data_3F564[17] __attribute__((scn_string_literal)) = "JOMO_AKIRA = %d\012";
static const char data_3F575[19] __attribute__((scn_string_literal)) = "[[[UISCRWHEREGO]]]";
static const char data_3F588[2] __attribute__((scn_string_literal)) = " ";
static const char data_3F58A[17] __attribute__((scn_string_literal)) = "JOMO_AKIRA = %d\012";
static const char data_3F59B[7] __attribute__((scn_string_literal)) = "SA1117";
static const char data_3F5A2[11] __attribute__((scn_string_literal)) = "SA1117A001";
static const char data_3F5AD[11] __attribute__((scn_string_literal)) = "SA1117A002";
static const char data_3F5B8[11] __attribute__((scn_string_literal)) = "SA1117A003";
static const char data_3F5C3[7] __attribute__((scn_string_literal)) = "SA1117";
static const char data_3F5CA[18] __attribute__((scn_string_literal)) = "JOMO_AKIRA = %d\012";
static const int8_t data_3F5DC[8] = {0, 1, 2, 1, 2, 0, 2, 1};
static const char data_3F5E4[7] __attribute__((scn_string_literal)) = "SA1090";
static const char data_3F5EB[11] __attribute__((scn_string_literal)) = "SA1090A001";
static const char data_3F5F6[11] __attribute__((scn_string_literal)) = "SA1090A002";
static const char data_3F601[11] __attribute__((scn_string_literal)) = "SA1090A003";
static const char data_3F60C[8] __attribute__((scn_string_literal)) = "SA1090";
static const Vector3 data_3F614 = {.x = 14.886f, .y = -0.219f, .z = 6.3261f};
static const char data_3F620[18] __attribute__((scn_string_literal)) = "検索なしの移動１\012";
static const char data_3F632[20] __attribute__((scn_string_literal)) = "検索結果による移動\012";
static const char data_3F646[82] __attribute__((scn_string_literal)) = "異常移動です。どこでもドアの検索結果が異常です。花岡か、矢島に早急に連絡下さい。\012";
static const char data_3F698[17] __attribute__((scn_string_literal)) = "JOMO_AKIRA = %d\012";
static const char data_3F6A9[17] __attribute__((scn_string_literal)) = "JOMO_AKIRA = %d\012";
static const char data_3F6BA[18] __attribute__((scn_string_literal)) = "JOMO_AKIRA = %d\012";
static const int8_t data_3F6CC[12] = {3, 10, 17, 24, 31, 0, 0, 0, 0, 0, 0, 0};
static const int8_t data_3F6D8[12] = {7, 14, 21, 28, 0, 0, 0, 0, 0, 0, 0, 0};
static const int8_t data_3F6E4[12] = {4, 11, 18, 25, 0, 0, 0, 0, 0, 0, 0, 0};
static const int8_t data_3F6F0[9] = {4, 11, 18, 25, 0, 0, 0, 0, 0};
static const char data_3F6F9[59] __attribute__((scn_string_literal)) = " : %d %d %d %d %d %d : %d %d %d %d : %d %d %d %d : %d %d \012";
static const char data_3F734[12] __attribute__((scn_string_literal)) = "lacky : %d\012";
static const int32_t data_3F740[57] = {
  0, 0, 0, 0, 910, 1456, 0, -1274, 0, 0, 0, 0, 0, 0, 0, 0, 2548, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 910, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, -1638, 0, 0, 0
};
static const int32_t data_3F824[57] = {
  0, 1274, 0, 0, -3094, -3094, 0, 0, 0, 0, 0, -4369, 0, 0, -3276, 0, 0, -728, 0, 0, 0, 0, 0, -3822, 0, -728, -3458, 0, 0, 0, 0, 0, 0, 0, 0, -3276, 0,
  0, -3094, 0, 0, 0, 0, 0, 0, 0, 0, -2912, 0, 0, -2730, 0, 0, 0, 0, 0, 0
};
static const int32_t data_3F908[57] = {
  0, -364, -4004, 0, 546, 728, 0, 0, 0, 0, 0, -2366, 0, 0, -2548, 0, 0, -910, 0, 0, 0, 0, -182, -2548, 0, 0, -2912, 0, 182, -1092, 0, 0, 0, 0, 0,
  -2730, 0, 0, -2730, 0, 728, -1820, 0, 0, 0, 0, 0, -2366, 0, 0, -2002, 0, -182, -1456, 0, 0, 0
};
static const int32_t data_3F9EC[57] = {
  0, 0, 1638, 0, 0, -1456, 0, 0, -728, 0, 0, -2366, 0, 0, -2366, 0, 0, -1456, 0, 0, 0, 0, 0, -8009, 0, 1274, -9102, 0, 728, -364, 0, 0, 0, 0, 0,
  -9102, 0, 2002, -12743, 0, 2366, 728, 0, 0, 0, 0, 0, -9648, 0, -1820, -10012, 0, 2184, -4369, 0, 0, 0
};
static const int32_t data_3FAD0[57] = {
  0, 1456, -7099, 0, -1820, -1456, 0, -910, 0, 0, 0, -3822, 0, 0, -11468, 0, 0, -10194, 0, 0, 0, 0, 0, -4551, 0, -546, -12014, 0, -1092, -10376, 0, 0,
  0, 0, 0, -4369, 0, -910, -12014, 0, -364, -9466, 0, 0, 0, 0, -1638, -8374, 0, -2002, -8192, 0, -3276, -10376, 0, 0, 0
};
static const int32_t data_3FBB4[57] = {
  0, 3094, -7099, -182, -3094, -1092, 0, -2093, 0, 0, 0, -18022, 0, 0, -17112, 0, 0, -18386, 0, 0, 0, 0, 0, -12196, 0, 0, -18204, 0, 0, -16748, 0, 0,
  0, 0, 0, -11832, 0, 0, -18386, 0, 0, -15109, 0, 0, 0, 0, 819, -6007, 0, -1274, -16748, 0, -3458, -16201, 0, 0, 0
};
static const char data_3FC98[] __attribute__((scn_string_literal)) = "sprite";
static const char data_3FC9F[13] __attribute__((scn_string_literal)) = "SCROLL07.SPR";
static const char data_3FCAC[] __attribute__((scn_string_literal)) = "sprite";
static const char data_3FCB3[13] __attribute__((scn_string_literal)) = "SCROLL19.SPR";
static const char data_3FCC0[] __attribute__((scn_string_literal)) = "sprite";
static const char data_3FCC7[13] __attribute__((scn_string_literal)) = "SCROLL19.SPR";
static const Vector3 data_3FCD4 = {.x = 3.0f, .y = 3.0f, .z = 3.0f};
static const char data_3FCE0[] __attribute__((scn_string_literal)) = "sprite";
static const char data_3FCE7[13] __attribute__((scn_string_literal)) = "SCROLL27.SPR";
static const Vector3 data_3FCF4 = {.x = 0.35f, .y = 0.35f, .z = 0.35f};
static const char data_3FD00[] __attribute__((scn_string_literal)) = "sprite";
static const char data_3FD07[13] __attribute__((scn_string_literal)) = "SCROLL27.SPR";
static const char data_3FD14[] __attribute__((scn_string_literal)) = "sprite";
static const char data_3FD1B[13] __attribute__((scn_string_literal)) = "SCROLL19.SPR";
static const Vector4 data_3FD28 = {.x = 1.0f, .y = 0.3f, .z = 0.3f, .w = 0.3f};
static const Vector4 data_3FD38 = {.x = 1.0f, .y = 1.0f, .z = 1.0f, .w = 1.0f};
static const Vector3 data_3FD48 = {.x = 100.0f, .y = 100.0f, .z = 100.0f};
static const Vector3i data_3FD54 = {.x = -8556, .y = -5275, .z = -3817};
static const Vector3i data_3FD60 = {.x = 2548, .y = 3819, .z = -2910};
static const Vector3 data_3FD6C = {.x = 75.0f, .y = 75.0f, .z = 75.0f};
static const uint32_t data_3FD78[4] = {0, 0, 0, 0};
static const char data_3FD88[14] __attribute__((scn_string_literal)) = "scene/01/JOMO";
static const char data_3FD96[5] __attribute__((scn_string_literal)) = "KKYA";
static const char data_3FD9B[13] __attribute__((scn_string_literal)) = "seqdata0.bin";
static const char data_3FDA8[5] __attribute__((scn_string_literal)) = "KKYA";
static const char data_3FDAD[14] __attribute__((scn_string_literal)) = "scene/01/JOMO";
static const char data_3FDBB[5] __attribute__((scn_string_literal)) = "KKYB";
static const char data_3FDC0[13] __attribute__((scn_string_literal)) = "seqdata1.bin";
static const char data_3FDCD[5] __attribute__((scn_string_literal)) = "KKYB";
static const char data_3FDD2[14] __attribute__((scn_string_literal)) = "scene/01/JOMO";
static const char data_3FDE0[5] __attribute__((scn_string_literal)) = "KKYC";
static const char data_3FDE5[13] __attribute__((scn_string_literal)) = "seqdata2.bin";
static const char data_3FDF2[5] __attribute__((scn_string_literal)) = "KKYC";
static const char data_3FDF7[14] __attribute__((scn_string_literal)) = "scene/01/JOMO";
static const char data_3FE05[5] __attribute__((scn_string_literal)) = "KKYD";
static const char data_3FE0A[13] __attribute__((scn_string_literal)) = "seqdata3.bin";
static const char data_3FE17[5] __attribute__((scn_string_literal)) = "KKYD";
static const char data_3FE1C[14] __attribute__((scn_string_literal)) = "scene/01/JOMO";
static const char data_3FE2A[5] __attribute__((scn_string_literal)) = "KKYE";
static const char data_3FE2F[13] __attribute__((scn_string_literal)) = "seqdata4.bin";
static const char data_3FE3C[5] __attribute__((scn_string_literal)) = "KKYE";
static const char data_3FE41[14] __attribute__((scn_string_literal)) = "scene/01/JOMO";
static const char data_3FE4F[5] __attribute__((scn_string_literal)) = "KKYF";
static const char data_3FE54[13] __attribute__((scn_string_literal)) = "seqdata5.bin";
static const char data_3FE61[5] __attribute__((scn_string_literal)) = "KKYF";
static const char data_3FE66[11] __attribute__((scn_string_literal)) = "bgm201.snd";
static const char data_3FE71[11] __attribute__((scn_string_literal)) = "bgm079.snd";
static const char data_3FE7C[5] __attribute__((scn_string_literal)) = "KKYA";
static const char data_3FE81[5] __attribute__((scn_string_literal)) = "KKYB";
static const char data_3FE86[5] __attribute__((scn_string_literal)) = "KKYC";
static const char data_3FE8B[5] __attribute__((scn_string_literal)) = "KKYD";
static const char data_3FE90[5] __attribute__((scn_string_literal)) = "KKYE";
static const char data_3FE95[7] __attribute__((scn_string_literal)) = "KKYF";
static const FogColourData data_3FE9C = {.red = 1.0f, .green = 296.0f, .blue = 0.52f, .colour = 0xFF303038U};
static const uint32_t data_3FEAC[4] = {0xFF162000U, 0xFF162000U, 0x7D360839U, 0x7D360839U};
static const FogColourData data_3FEBC = {.red = 20.1f, .green = 800.0f, .blue = 0.31f, .colour = 0xFFFFBDBDU};
static const uint32_t data_3FECC[4] = {0xFF004C4AU, 0xFF004C4AU, 0xFF000000U, 0xFF000000U};
static const FogColourData data_3FEDC = {.red = 0.1f, .green = 60.0f, .blue = 0.26f, .colour = 0xFC3C7C38U};
static const uint32_t data_3FEEC[4] = {0xFF0000ACU, 0xFF0000ACU, 0xFF000000U, 0xFF000000U};
static const FogColourData data_3FEFC = {.red = 50.1f, .green = 600.1f, .blue = 0.3f, .colour = 0xFF2379FDU};
static const uint32_t data_3FF0C[4] = {0xFF000000U, 0xFF000000U, 0x4D000000U, 0x4D000000U};
static const FogColourData data_3FF1C = {.red = 0.1f, .green = 60.0f, .blue = 0.36f, .colour = 0xFF00257BU};
static const uint32_t data_3FF2C[4] = {0xFF000000U, 0xFF000000U, 0xFF00000CU, 0xFF00000CU};
static const FogColourData data_3FF3C = {.red = 5.0f, .green = 800.0f, .blue = 1.0f, .colour = 0xFFFFFFFFU};
static const uint32_t data_3FF4C[61] = {
  0xFF000000U, 0xFF000000U, 0xFF000000U, 0xFF000000U, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};
static const int32_t data_40040[228] = {
  0, 0, -3640, 0, 0, -2366, -4369, 1820, -546, 0, 0, -5279, 0, 0, -6007, 0, 0, -2912, 0, 0, -910, 0, 0, -6553, 0, 0, -6735, 0, 0, -3640, 0, 0, -182,
  0, 0, -5461, 0, 0, -6007, 0, 0, -2366, 0, 0, 0, 0, 0, -5643, 0, 0, -4733, 0, 0, -2366, -1456, 0, 728, 0, 0, -9466, 0, -546, -2912, 0, 3094, -2366,
  0, 0, -18386, 0, 0, -19296, 0, 1092, -14381, 0, 0, 0, 0, 0, -16748, 0, 0, -17658, 0, 0, -15109, 0, 0, 0, 0, 0, -16566, 0, 0, -18204, 0, 0, -14381,
  0, 0, 0, 0, 0, -19478, 0, 0, -18386, 0, -4004, -12925, 0, 0, -546, 0, 0, -3276, 0, 0, 0, 0, 0, 0, 0, 0, -6553, 0, 0, -4369, 0, -546, -3458, 0, 0, 0,
  0, 0, -5461, 0, 0, -5643, 0, 0, -3458, 0, 0, 0, 0, 0, -6007, 0, 0, -4551, 0, 546, -2548, 0, 0, 0, 0, 0, -6189, 0, -1092, -3094, 0, 0, -2548, 0, 0,
  0, 0, 0, -6007, 0, 0, -728, 0, 0, 0, 0, 0, -5097, 0, 0, -2184, 0, 0, -2002, 0, 0, 0, 0, 0, -5825, 0, 0, -2912, 0, 0, -728, 0, 0, 0, 0, 0, -4915, 0,
  0, -4187, 0, 0, -1274, 0, 0, 0, 0, 0, -6553, 0, -364, -4369, 0, -546, -1456, 0, 0, 0
};
static const char data_403D0[5] __attribute__((scn_string_literal)) = "BEBF";
static const char data_403D5[7] __attribute__((scn_string_literal)) = "BEBF";
static const Vector4 data_403DC = {.x = 0.3f, .y = 0.3f, .z = 0.3f, .w = 0.3f};
static const FogColourData data_403EC = {.red = 0.1f, .green = 190.0f, .blue = 1.3f, .colour = 0xFF312B3FU};
static const uint32_t data_403FC[4] = {0xFF202622U, 0xFF202622U, 0xA7787C74U, 0xA7787C74U};
static const Vector4 data_4040C = {.x = 1.0f, .y = 0.04f, .z = 0.02f, .w = 0.08f};
static const FogColourData data_4041C = {.red = 0.1f, .green = 10.1f, .blue = 0.14f, .colour = 0xFFAB8DD3U};
static const uint32_t data_4042C[4] = {0, 0, 0, 0};
static const uint32_t data_4043C[4] = {0, 0, 0, 0};
static const char data_4044C[14] __attribute__((scn_string_literal)) = "scene/01/JOMO";
static const char data_4045A[5] __attribute__((scn_string_literal)) = "BEBF";
static const char data_4045F[13] __attribute__((scn_string_literal)) = "seqdata0.bin";
static const char data_4046C[5] __attribute__((scn_string_literal)) = "BEBF";
static const char data_40471[13] __attribute__((scn_string_literal)) = "seqdata1.bin";
static const char data_4047E[5] __attribute__((scn_string_literal)) = "BEBF";
static const char data_40483[13] __attribute__((scn_string_literal)) = "seqdata2.bin";
static const char data_40490[5] __attribute__((scn_string_literal)) = "BEBF";
static const char data_40495[13] __attribute__((scn_string_literal)) = "seqdata3.bin";
static const char data_404A2[5] __attribute__((scn_string_literal)) = "BEBF";
static const char data_404A7[7] __attribute__((scn_string_literal)) = "01BEDB";
static const char data_404AE[11] __attribute__((scn_string_literal)) = "bgm129.snd";
static const char data_404B9[13] __attribute__((scn_string_literal)) = "a1_badme.snd";
static const char data_404C6[7] __attribute__((scn_string_literal)) = "01BEDB";
static const char data_404CD[5] __attribute__((scn_string_literal)) = "BEBF";
static const char data_404D2[22] __attribute__((scn_string_literal)) = "母屋内強制睡眠始動！\012";
static const char data_404E8[17] __attribute__((scn_string_literal)) = "JOMO_AKIRA = %d\012";
static const char data_404F9[42] __attribute__((scn_string_literal)) = "こらこらーーー。誰だここを呼んだのは！！\012";
static const char data_40523[13] __attribute__((scn_string_literal)) = "a1_kawak.snd";
static const char data_40530[15] __attribute__((scn_string_literal)) = "寝るオーサ %d\012";
static const char data_4053F[13] __attribute__((scn_string_literal)) = "a1_kawat.snd";
static const char data_4054C[11] __attribute__((scn_string_literal)) = "bgm046.snd";
static const char data_40557[39] __attribute__((scn_string_literal)) = "起きるオーサ・アルバイトバージョン %d\012";
static const char data_4057E[17] __attribute__((scn_string_literal)) = "起きるオーサ %d\012";
static const char data_4058F[9] __attribute__((scn_string_literal)) = "YKC_M";
static const FogColourData data_40598 = {.red = 8.0f, .green = 150.0f, .blue = 1.0f, .colour = 0xFF000000U};
static const char data_405A8[10] __attribute__((scn_string_literal)) = "E1018A149";
static const char data_405B2[13] __attribute__((scn_string_literal)) = "a1_kawat.snd";
static const char data_405BF[11] __attribute__((scn_string_literal)) = "bgm046.snd";
static const char data_405CA[6] __attribute__((scn_string_literal)) = "YKC_M";
static const uint32_t data_405D0[4] = {0, 0, 0, 0};
static const char data_405E0[6] __attribute__((scn_string_literal)) = "MPK00";
static const char data_405E6[14] __attribute__((scn_string_literal)) = "scene/01/JOMO";
static const char data_405F4[6] __attribute__((scn_string_literal)) = "MPK00";
static const char data_405FA[24] __attribute__((scn_string_literal)) = "ふくちゃん読み込み失敗\012";
static const char data_40612[22] __attribute__((scn_string_literal)) = "いねさん読み込み失敗\012";
static const int32_t data_40628[57] = {
  0, 0, -6004, 0, 0, -910, 0, 0, 0, 0, 0, -6004, 0, 1820, -3091, 0, 0, -4548, 0, 0, 0, 0, 0, -6186, 0, 0, -6185, 0, -545, -4002, 0, 0, 0, 0, 0, -2730,
  0, 0, -9648, 0, -909, -2182, 0, 0, 0, 0, -1636, -5274, 0, -2001, -9098, 0, -1819, -727, 0, 0, 0
};
static const int32_t data_4070C[57] = {
  0, 0, -10922, 0, 0, -10558, 0, 1456, 0, 0, 0, -18022, 0, 0, -17112, 0, 0, -18386, 0, 0, 0, 0, 0, -12196, 0, 0, -18204, 0, 0, -16748, 0, 0, 0, 0, 0,
  -11832, 0, 0, -18386, 0, 0, -15109, 0, 0, 0, 0, 910, -12014, 0, -2184, -16748, 0, -2184, -10012, 0, 0, 0
};
static const int32_t data_407F0[114] = {
  0, 0, -5461, 0, -2912, -364, 0, -182, 1092, 0, -546, -10012, 0, 0, -16019, 0, 728, -8374, 0, -182, 1274, 0, 0, -17476, 0, 546, -17840, 0, 0, -8009,
  0, 0, 1456, 0, 182, -910, 0, 0, -16748, 0, 0, -8738, 0, 182, 1638, 0, -728, -16384, 0, -2548, -16566, 0, -1456, -8738, 0, 546, 1638, 0, 182, 0, 0,
  183, -909, 0, 1092, -1092, 0, 0, -4187, 0, -3458, -9284, 5097, -364, -910, 0, 0, 0, 0, 0, -16748, 0, 0, -19478, 0, 0, -2730, 0, 0, 0, 0, -546,
  -13107, 0, -910, -18386, 0, 0, -3276, 0, 0, 0, 0, -1456, -15109, 0, -4733, -20024, 0, 1092, -3094, 0, 0, 0
};
static const int32_t data_409B8[57] = {
  0, 0, -5461, 0, -2366, -1092, 0, -182, 1092, 0, -546, -6735, 0, 0, -4004, 0, 0, -2912, 0, -182, 1274, 0, 0, -13289, 0, 0, -5279, 0, 0, -8009, 0, 0,
  1456, 0, 182, -8738, 0, 0, -6007, 0, 0, -8738, 0, 182, 1638, 0, -728, -5279, 0, -1274, -6735, 0, -910, -8738, 0, 0, 1638
};
static const int32_t data_40A9C[114] = {
  0, 0, -1820, 0, -2366, -728, 0, 1456, -1274, 0, -546, -2366, 0, 910, -6735, 0, 0, -8009, 0, -182, 546, 0, 0, -5279, 0, 0, -4551, 0, 0, -10012, 0, 0,
  1092, 0, 182, -7281, 0, 0, -6735, 0, 0, -9830, 0, 182, 1638, 0, -728, -6553, 0, -1274, -7281, 0, -910, -10740, 0, 0, 1638, 0, 0, -2730, 0, 0, -2366,
  0, 0, 0, 0, 0, 0, 0, -364, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 182, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 546, 0, 0, 0,
  0, 0, 0, 0
};
static const int32_t data_40C64[57] = {
  0, 0, -10922, 0, 0, -10558, 0, 1456, 0, 0, 0, -18022, 0, 0, -17112, 0, 0, -18386, 0, 0, 0, 0, 0, -12196, 0, 0, -18204, 0, 0, -16748, 0, 0, 0, 0, 0,
  -11832, 0, 0, -18386, 0, 0, -15109, 0, 0, 0, 0, 910, -12014, 0, -2184, -16748, 0, -2184, -10012, 0, 0, 0
};
static const int32_t data_40D48[57] = {
  0, 0, -3640, 0, 0, -3640, 0, 0, 0, 0, 0, -3640, 0, 0, -3640, 0, 0, 0, 0, 0, 0, 0, 0, -3640, 0, 0, -3640, 0, 0, 0, 0, 0, 0, 0, 0, -3640, 0, 0, -3640,
  0, 0, 0, 0, 0, 0, 0, 0, -3640, 0, 0, -3640, 0, 0, 0, 0, 0, 0
};
static const int32_t data_40E2C[114] = {
  0, 0, -3640, 0, 2002, 1638, 0, 0, 0, 0, 0, -3640, 0, 0, -3640, 0, 0, 0, 0, 0, 0, 0, 0, -3640, 0, 0, -3640, 0, 0, 0, 0, 0, 0, 0, 0, -3640, 0, 0,
  -3640, 0, 0, 0, 0, 0, 0, 0, 0, -3640, 0, 0, -3640, 0, 0, 0, 0, 0, 0, 0, 0, -10922, 0, 0, -10558, 0, 1456, 0, 0, 0, -18022, 0, 0, -17112, 0, 0,
  -18386, 0, 0, 0, 0, 0, -12196, 0, 0, -18204, 0, 0, -16748, 0, 0, 0, 0, 0, -11832, 0, 0, -18386, 0, 0, -15109, 0, 0, 0, 0, 910, -12014, 0, -2184,
  -16748, 0, -2184, -10012, 0, 0, 0
};
static const int32_t data_40FF4[57] = {
  0, 182, 0, 0, 182, -909, 0, 1092, -1092, 0, 0, -4187, 0, -3458, -9284, 5097, -364, -910, 0, 0, 0, 0, 0, -16748, 0, 0, -19478, 0, 0, -2730, 0, 0, 0,
  0, -546, -13107, 0, -910, -18386, 0, 0, -3276, 0, 0, 0, 0, -1456, -15109, 0, -4733, -20024, 0, 1092, -3094, 0, 0, 0
};
static const int32_t data_410D8[114] = {
  0, -2002, -4369, 0, 6007, -2002, 0, 1092, -1092, 0, 0, -4187, 0, -3458, -9284, 5097, -364, -910, 0, 0, 0, 0, 0, -16748, 0, 0, -19478, 0, 0, -2730,
  0, 0, 0, 0, -546, -13107, 0, -910, -18386, 0, 0, -3276, 0, 0, 0, 0, -1456, -15109, 0, -4733, -20024, 0, 1092, -3094, 0, 0, 0, 0, -3640, -2184, 0,
  -2548, -4551, 0, 0, 0, 0, 0, -728, 0, 0, 728, 0, 0, 0, 0, 0, 0, 0, 0, -1092, 0, 0, -364, 0, 0, -546, 0, 0, 0, 0, 0, -1092, 0, 0, -1456, 0, 0, 0, 0,
  0, 0, 0, 0, -2366, 0, 0, -2002, 0, 0, 0, 0, 0, 0
};
static const int32_t data_412A0[57] = {
  0, -3640, -5643, 0, -910, -1638, 0, 0, -1092, 0, 0, -4733, 0, -546, -4187, 0, 728, -1820, 0, 0, 0, 0, 0, -910, 0, 0, -4551, 0, 0, -3276, 0, 0, 0, 0,
  -546, -2912, 0, -1638, -4733, 0, 546, -3458, 0, 0, 0, 0, -546, -6007, 0, -2002, -5825, 0, 0, -2548, 0, 0, 0
};
static const int32_t data_41384[57] = {
  0, 0, -2730, 0, 0, -2366, 0, 0, 0, 0, 0, 0, 0, -364, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 182, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 546, 0, 0, 0, 0, 0, 0, 0
};
static const int32_t data_41468[57] = {
  0, 0, -10975, 0, 0, -4840, 0, 0, 0, 0, 0, -1436, 0, 0, 380, 0, 0, -961, 0, 0, 0, 0, 0, -12213, 0, 0, -20259, 0, 0, -7188, 0, 0, 0, 0, 0, -11703, 0,
  182, -19149, 0, 0, -8226, 0, 34, -52, 0, 0, -12905, 0, -2166, -17747, 0, -2530, -10793, 0, -91, -344
};
static const int32_t data_4154C[57] = {
  0, 0, -10975, 0, 0, -4840, 0, 0, 0, 0, 0, -1072, 0, 0, -2894, 0, 0, -3693, 0, 0, 0, 0, 0, -12213, 0, 0, -20259, 0, 0, -7188, 0, 0, 0, 0, 0, -11703,
  0, 182, -19149, 0, 0, -8226, 0, 34, -52, 0, 0, -12905, 0, -2166, -17747, 0, -2530, -10793, 0, -91, -344
};
static const int32_t data_41630[57] = {
  0, 0, -7463, 0, 0, -575, 0, 0, 0, 0, 0, -2257, 0, 0, -1368, 0, 0, -1013, 0, 0, 0, 0, 0, -4860, 0, 0, -3495, 0, 0, -1201, 0, 0, 0, 0, 0, -2621, 0,
  364, -5206, 0, 0, -902, 0, 91, -91, 0, -1456, -5270, 0, -546, -3342, 0, 0, -1230, 0, -91, -708
};
static const int32_t data_41714[57] = {
  0, 0, -10977, 0, 0, -4842, 0, 0, 0, 0, 0, -892, 0, 0, 0, 0, 0, -1693, 0, 0, 0, 0, 0, -8574, 0, 0, -16074, 0, 0, -4642, 0, 0, 0, 0, 0, -8610, 0, 182,
  -16966, 0, 0, -6407, 0, 36, -54, 0, 0, -11450, 0, 0, -13562, 0, 0, -8974, 0, -819, -345
};
static const int32_t data_417F8[57] = {
  0, 0, -8046, 0, 2184, -1829, 0, 0, 0, 0, 0, -9011, 0, 910, -18422, 0, 0, -9284, 0, 0, 0, 0, 0, -5697, 0, 0, -17385, 0, 0, -13016, 0, 0, 0, 0, 0,
  -7736, 0, 182, -13871, 0, 0, -14599, 0, 0, 0, 0, -1274, -14199, 0, 0, -6371, 0, -3094, -15983, -27, -109, 0
};
static const int32_t data_418DC[57] = {
  0, 0, -8047, 0, 0, -1831, 0, 0, 0, 0, 0, -9014, 0, 0, -18428, 0, 0, -9829, 0, 0, 0, 0, 0, -7520, 0, 0, -17389, 0, 0, -11564, 0, 0, 0, 0, 0, -5737,
  0, 0, -18062, 0, 0, -12249, 0, 0, 0, 0, 0, -7833, 0, 0, -16185, 0, 0, -13991, -574, -849, 0
};
static const int32_t data_419C0[57] = {
  0, 0, -3275, 0, 0, -1637, 0, 0, 0, 0, 0, -4550, 0, 0, -4731, 0, 0, -1455, 0, 0, 0, 0, 0, -4186, 0, 0, -6005, 0, 0, -1819, 0, 0, 0, 0, 0, -5823, 0,
  910, -4731, 0, 0, -2001, 0, 0, 0, 0, -909, -3276, 0, -909, -3639, 0, 0, -3457, 0, 0, 0
};
static const int32_t data_41AA4[57] = {
  0, 0, -6004, 0, 0, -910, 0, 0, 0, 0, 0, -6004, 0, 1820, -3091, 0, 0, -4548, 0, 0, 0, 0, 0, -6186, 0, 0, -6185, 0, -545, -4002, 0, 0, 0, 0, 0, -7823,
  0, 0, -12374, 0, -909, -2182, 0, 0, 0, 0, -1636, -10735, 0, -2001, -10372, 0, -1819, -727, 0, 0, 0
};
static const int32_t data_41B88[57] = {
  0, 0, -6005, 0, 0, -909, 0, 0, 0, 0, 0, -6368, 0, 1820, -8552, 0, 0, -4548, 0, 0, 0, 0, 0, -8552, 0, 0, -10372, 0, -545, -4002, 0, 0, 0, 0, 0,
  -7823, 0, 0, -12374, 0, -909, -2182, 0, 0, 0, 0, -1636, -10735, 0, -2001, -10372, 0, -1819, -727, 0, 0, 0
};
static const int32_t data_41C6C[57] = {
  0, 0, -6003, 0, 0, -908, 0, 0, 0, 0, 0, -3819, 0, 0, -7094, 0, 0, -4547, 0, 0, 0, 0, 0, -2909, 0, 0, -8550, 0, -544, -4001, 0, 0, 0, 0, 0, -4363, 0,
  0, -8004, 0, -908, -2180, 0, 0, 0, 0, -1634, -6366, 0, -2000, -4365, 0, -1818, -2182, 0, 0, 0
};
static const int32_t data_41D50[57] = {
  0, 1092, -5825, 2184, 364, 1720, 0, 0, 0, 0, 1456, -9648, 0, 1092, -4849, 0, 1092, -6861, 0, 0, 0, 0, 2002, -7620, 0, 0, -6351, 0, 364, -7152, 0, 0,
  0, 0, 0, -7988, 0, 1820, -19842, 0, 0, -10758, 0, 43, -49, 0, 0, -7778, 0, -546, -18459, 0, -1092, -13076, 0, -668, -327
};
static const int32_t data_41E34[57] = {
  0, 1092, -5825, 2184, 364, 1720, 0, 0, -281, 0, 1456, -9648, 0, 1092, -7032, 0, 1092, -6861, 0, 0, 0, 0, 2002, -9803, 0, 1274, -20000, 0, 364,
  -7697, 0, 0, 0, 0, 0, -7988, 0, 1820, -19842, 0, 0, -10758, 0, 43, -49, 0, 0, -7778, 0, -546, -18459, 0, -1092, -13076, 0, -668, -327
};
static const int32_t data_41F18[57] = {
  0, 0, -3640, 0, 1092, -2184, 0, 0, 0, 0, 0, -3458, 0, 0, -8374, 0, 0, -4369, 0, 0, 0, 0, 0, -4733, 0, 0, -6917, 0, 0, -3458, 0, 0, 0, 0, 0, -5097,
  0, -364, -5461, 0, 0, -2184, 0, 0, 0, 0, 0, -4369, 0, -2184, -5097, 0, 0, -1456, 0, 0, 0
};
static const int32_t data_41FFC[57] = {
  0, 0, -6005, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -5459, 0, 727, -6005, 0, 0, 0, 0, 0, -3821, 0, -363, -6005, 0, 363, -4367, 0, 0, 0, 0, 0, -3821, 0, 0,
  -6188, 0, 0, -3093, 0, 0, 0, 0, 0, -3821, 0, -1637, -363, 0, -1273, -4731, 0, 0, 0
};
static const int32_t data_420E0[57] = {
  0, 0, -6005, 0, 4731, -6188, 0, 0, 0, 0, 0, -2727, 0, 0, -9097, 0, 0, -5457, 0, 0, 0, 0, 0, -8734, 0, -909, -7277, 0, 0, -7823, 0, 0, 0, 0, 1456,
  -4002, 0, 0, -9462, 0, 726, -7824, 0, 0, 0, 0, 0, -12011, 0, -1091, -7461, 0, -1636, -9280, 0, 0, 0
};
static const int32_t data_421C4[57] = {
  0, 0, -6005, 0, 2547, -3275, 0, 0, 0, 0, 0, -2727, 0, 0, -9097, 0, 0, -5457, 0, 0, 0, 0, 0, -8734, 0, -909, -7277, 0, 0, -7823, 0, 0, 0, 0, 1456,
  -4002, 0, 0, -9462, 0, 726, -7824, 0, 0, 0, 0, 0, -12011, 0, -1091, -7461, 0, -1636, -9280, 0, 0, 0
};
static const char data_422A8[9] __attribute__((scn_string_literal)) = "COMMON01";
static const char data_422B1[9] __attribute__((scn_string_literal)) = "COMMON01";
static const char data_422BA[11] __attribute__((scn_string_literal)) = "SA1062B001";
static const char data_422C5[11] __attribute__((scn_string_literal)) = "SA1062A001";
static const char data_422D0[7] __attribute__((scn_string_literal)) = "SA1063";
static const char data_422D7[7] __attribute__((scn_string_literal)) = "SA1062";
static const char data_422DE[11] __attribute__((scn_string_literal)) = "SA1063B001";
static const char data_422E9[11] __attribute__((scn_string_literal)) = "SA1063A001";
static const char data_422F4[11] __attribute__((scn_string_literal)) = "SA1063B002";
static const char data_422FF[11] __attribute__((scn_string_literal)) = "SA1063A002";
static const char data_4230A[11] __attribute__((scn_string_literal)) = "SA1063B003";
static const char data_42315[11] __attribute__((scn_string_literal)) = "SA1063A003";
static const char data_42320[11] __attribute__((scn_string_literal)) = "SA1062B002";
static const char data_4232B[11] __attribute__((scn_string_literal)) = "SA1062A002";
static const char data_42336[11] __attribute__((scn_string_literal)) = "SA1062B004";
static const char data_42341[11] __attribute__((scn_string_literal)) = "SA1062A003";
static const char data_4234C[7] __attribute__((scn_string_literal)) = "SA1063";
static const char data_42353[9] __attribute__((scn_string_literal)) = "SA1062";
static const uint32_t data_4235C = 27306;
static const float data_42360[4] = {14.118f, 1.281f, -0.058f, 0.0f};
static const int32_t data_42370[4] = {2, 2, 0, 0};
static const float data_42380[3] = {15.38f, 1.85f, -2.2f};
static const int32_t data_4238C[5] = {1, 0, 2, 0, 32768};
static const float data_423A0[4] = {17.792f, 1.281f, -0.058f, 0.0f};
static const int32_t data_423B0[4] = {1, 3, 0, 0};
static const float data_423C0[3] = {19.03f, 1.85f, -2.31f};
static const int32_t data_423CC[5] = {1, 0, 3, 0, 0};
static const uint32_t data_423E0 = 0x418BEF9EU;
static const float data_423E4[3] = {1.281f, 1.856f, 0.0f};
static const int32_t data_423F0[4] = {1, 4, 0, 0};
static const uint32_t data_42400 = 0x41B2353FU;
static const float data_42404[3] = {1.281f, 1.83f, 0.0f};
static const int32_t data_42410[4] = {1, 5, 0, 16384};
static const float data_42420[4] = {20.95f, 1.277f, -0.246f, 0.0f};
static const int32_t data_42430[4] = {5, 6, 1, 0};
static const float data_42440[3] = {22.79f, 1.85f, -2.31f};
static const int32_t data_4244C[5] = {1, 0, 6, 0, 0};
static const float data_42460[4] = {0.772f, 1.283f, -1.79f, 0.0f};
static const int32_t data_42470[4] = {2, 7, 0, 0};
static const float data_42480[3] = {-0.9f, 1.85f, 0.0f};
static const int32_t data_4248C[5] = {1, 0, 7, 0, 0};
static const float data_424A0[4] = {5.75f, 1.281f, -1.794f, 0.0f};
static const int32_t data_424B0[4] = {1, 8, 0, 32768};
static const float data_424C0[4] = {12.912f, 1.281f, -0.056f, 0.0f};
static const int32_t data_424D0[4] = {5, 9, 1, 43690};
static const float data_424E0[4] = {16.198f, 1.361f, 2.063f, 0.0f};
static const int32_t data_424F0[4] = {2, 10, 0, 32768};
static const shen_name data_42500 = SHEN_NAME('33SA');
static const float data_42504[3] = {1.271f, 1.78f, 0.0f};
static const int32_t data_42510[4] = {3, 10, 0, 32768};
static const float data_42520[4] = {6.06f, 1.271f, -1.84f, 0.0f};
static const int32_t data_42530[4] = {4, 13, 0, 16384};
static const float data_42540[3] = {-2.19f, 0.45f, -0.45f};
static const int32_t data_4254C[5] = {2, 7, 14, 0, 27306};
static const float data_42560[3] = {16.38f, 0.75f, 3.81f};
static const int32_t data_4256C[5] = {2, 6, 15, 0, 54613};
static const float data_42580[3] = {22.51f, 0.45f, -4.32f};
static const int32_t data_4258C[5] = {2, 8, 16, 0, 0};
static const float data_425A0[4] = {5.12f, 1.271f, -3.624f, 0.0f};
static const int32_t data_425B0[3] = {5, 1, 1};
static const uint8_t data_425BC[8] = {0};
typedef struct Row_425C4 { uint32_t w0; int32_t w1; float w2; int32_t w3; int32_t w4; int32_t w5; } Row_425C4;
static const Row_425C4 data_425C4[16] = {
  {0x204F4841U, 0, 0.0f, 0, 0, 0},
  {0x6E656474U, 5, 0.0f, 7, 9, 18},
  {0x6E65646CU, 1, 2.3f, 2, 27, 36},
  {0x6E656469U, 0, 2.15f, 2, 45, 0},
  {0x6E656472U, 2, 0.0f, 8, 54, 0},
  {0x6E656467U, 2, 0.0f, 0, 63, 72},
  {0x6E656466U, 0, 2.15f, 0, 81, 90},
  {0x6E656477U, 0, 2.15f, 3, 99, 0},
  {0x6E656462U, 2, 0.0f, 3, 108, 0},
  {0x6E65646BU, 2, 0.0f, 4, 117, 0},
  {0x316E646CU, 3, 0.0f, 6, 126, 0},
  {0x326E646CU, 3, 0.0f, 6, 135, 0},
  {0x6E656465U, 3, 0.0f, 6, 144, 0},
  {0x336E646CU, 3, 0.0f, 7, 153, 0},
  {0x6E747377U, 6, 0.0f, 3, 162, 0},
  {0x6E747372U, 6, 0.0f, 8, 171, 0}
};
static const uint32_t data_42744 = 0x6E747366U;
static const int32_t data_42748[3] = {6, 0, 0};
static const uint32_t data_42754 = 0x3F51EB85;
static const float data_42758[179] = {
  0.68f, 0.87f, 0.9f, 5.95f, 2.26f, -4.68f, 1.22f, 0.65f, 0.93f, 0.75f, 0.56f, 0.9f, 15.67f, 1.8f, -2.17f, 2.92f, 0.68f, 0.75f, 0.38f, 0.11f, 0.9f,
  15.67f, 1.8f, -2.17f, 2.92f, 0.68f, 0.96f, 0.77f, 0.63f, 0.9f, 18.86f, 1.62f, -2.45f, 2.94f, 0.68f, 0.75f, 0.38f, 0.11f, 0.9f, 18.86f, 1.62f,
  -2.45f, 2.94f, 0.68f, 0.79f, 0.71f, 0.61f, 0.9f, 18.06f, 1.81f, 3.98f, 3.21f, 0.68f, 0.79f, 0.67f, 0.55f, 0.9f, 22.48f, 1.9f, 4.36f, 3.11f, 0.68f,
  0.96f, 0.8f, 0.59f, 0.8f, 22.75f, 1.55f, -2.48f, 3.38f, 0.65f, 0.75f, 0.38f, 0.11f, 0.8f, 22.75f, 1.55f, -2.48f, 3.38f, 0.65f, 0.96f, 0.85f, 0.7f,
  0.9f, -0.91f, 1.83f, -0.09f, 2.86f, 0.68f, 0.75f, 0.38f, 0.11f, 0.9f, -0.91f, 1.83f, -0.09f, 2.86f, 0.68f, 0.79f, 0.67f, 0.55f, 0.9f, 4.23f, 1.79f,
  -0.16f, 3.15f, 0.68f, 0.85f, 0.85f, 0.78f, 0.9f, 10.8f, 2.18f, -2.35f, 3.8f, 0.67f, 0.79f, 0.71f, 0.55f, 0.8f, 10.53f, 2.0f, 0.89f, 2.44f, 0.67f,
  0.79f, 0.71f, 0.55f, 0.8f, 18.47f, 1.82f, 1.02f, 1.74f, 0.67f, 0.79f, 0.67f, 0.55f, 0.9f, 14.77f, 1.88f, 4.19f, 3.54f, 0.63f, 0.79f, 0.71f, 0.55f,
  1.0f, 2.28f, 2.15f, -3.18f, 1.83f, 0.67f, 0.93f, 0.95f, 0.93f, 1.3f, -2.14f, 0.56f, -0.33f, 0.94f, 0.69f, 0.93f, 0.95f, 0.93f, 1.3f, 16.59f, 0.77f,
  4.0f, 0.94f, 0.69f, 0.93f, 0.95f, 0.93f, 1.3f, 22.72f, 0.64f, -3.89f, 0.94f, 0.69f
};
static const uint8_t data_42A24[8] = {0};
static const float data_42A2C[2] = {-0.014433f, 0.0f};
static const uint32_t data_42A34 = 0x39B78034U;
static const float data_42A38[25] = {
  -0.014433f, 0.0f, 0.001104f, -0.014433f, 0.0f, 0.001784f, -0.014433f, 0.0f, 0.001919f, -0.014433f, 0.0f, 0.005351f, -0.014433f, 0.001958f,
  0.012284f, -0.014433f, 0.004421f, 0.017139f, -0.016739f, 0.004241f, 0.014464f, -0.017014f, 0.00177f, 0.003749f, -0.015205f
};
static const uint32_t data_42A9C = 0xBA16FEB5U;
static const float data_42AA0[14] = {
  -0.010343f, -0.012764f, -0.002219f, -0.025452f, -0.010269f, -0.002954f, -0.039229f, -0.008123f, -0.002799f, -0.049361f, -0.006577f, -0.00178f,
  -0.053549f, -0.005817f
};
static const uint32_t data_42AD8 = 0xB9EAE18BU;
static const float data_42ADC[2] = {-0.052011f, -0.005856f};
static const uint32_t data_42AE4 = 0x3A62C12BU;
static const float data_42AE8[18] = {
  -0.047061f, -0.006428f, 0.002427f, -0.039593f, -0.007418f, 0.004162f, -0.030609f, -0.008785f, 0.005975f, -0.021107f, -0.010431f, 0.007678f,
  -0.012086f, -0.012205f, 0.009024f, -0.004519f, -0.013838f, 0.009622f
};
static const uint32_t data_42B30 = 0x3A26B93DU;
static const float data_42B34[7] = {-0.014986f, 0.009622f, 0.0f, -0.014986f, 0.009024f, 0.0f, -0.013838f};
static const uint32_t data_42B50 = 0xBF65DCC6;
static const float data_42B54[11] = {1.85f, 0.0f, 15.3865f, 1.85f, -2.1978f, 22.7922f, 1.85f, -2.31f, 19.03f, 1.85f, -2.31f};
static const uint32_t data_42B80 = 0x3DCCCCCD;
static const float data_42B84[43] = {
  0.12f, 0.13f, 0.15f, 0.16f, 0.18f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.15f, 0.0f, 0.0f, 0.6f, 0.0f, 0.0f, 0.0f, 0.6f, 0.0f,
  0.0f, 0.0f, 0.6f, 0.0f, 0.0f, 0.0f, 0.6f, 0.0f, 0.0f, 0.0f, 0.5f, 0.6f, 0.0f, 0.6f, 0.0f, 0.2f, 0.0f, 0.0f, 0.6f, 0.8f
};
static const char str_42C30[] __attribute__((scn_string_literal)) = "fff?33s?";
static const uint8_t data_42C39[11] = {
  0x00, 0x80, 0x3F, 0x00, 0x00, 0x80, 0x3F, 0x00, 0x00, 0x80, 0xBF
};
static const uint32_t data_42C44 = 0x3E99999A;
static const float data_42C48[18] = {
  0.2f, 0.15f, 0.1f, 0.07f, 0.06f, 0.05f, 0.04f, 0.03f, 0.02f, 0.014f, 0.01f, 0.005f, 0.007f, 0.004f, 0.002f, 0.001f, 0.0f, -1.0f
};
static const uint32_t data_42C90 = 0x3E4CCCCD;
static const float data_42C94[4] = {0.4f, 0.6f, 0.7f, 0.8f};
static const char str_42CA4[] __attribute__((scn_string_literal)) = "fff?33s?";
static const uint8_t data_42CAD[7] = {
  0x00, 0x80, 0x3F, 0x00, 0x00, 0x80, 0xBF
};
static const uint32_t data_42CB4 = 0x3F800000;
static const float data_42CB8[47] = {
  0.27f, 0.28f, 0.31f, 1.0f, 0.3f, 0.3f, 0.32f, 1.0f, 0.33f, 0.32f, 0.33f, 1.0f, 0.36f, 0.34f, 0.31f, 1.0f, 0.39f, 0.36f, 0.35f, 1.0f, 0.39f, 0.342f,
  0.308f, 1.0f, 0.39f, 0.324f, 0.266f, 1.0f, 0.39f, 0.306f, 0.224f, 1.0f, 0.4f, 0.315f, 0.245f, 1.0f, 0.4f, 0.27f, 0.14f, 1.0f, 0.32f, 0.25f, 0.18f,
  1.0f, 0.2f, 0.18f, 0.2f
};
static const int8_t data_42D74[44] = {
  11, 11, 11, 11, 11, 0, 0, 0, 1, 2, 3, 4, 4, 4, 4, 3, 2, 1, 0, 11, 11, 11, 11, 11, 0, 2, 1, 1, 2, 2, 1, 1, 2, 2, 2, 0, 0, 0, 2, 2, 2, 0, 0, 0
};
static const int8_t data_42DA0[20] = {0, 2, 3, 3, 3, 2, 3, 1, 3, 3, 3, 0, 0, 0, 2, 2, 2, 0, 0, 0};
static const Vector3 data_42DB4 = {.x = 1.0f, .y = 1.15f, .z = 1.0f};
static const Vector3i data_42DC0 = {.x = 0, .y = 0, .z = 0};
static const char data_42DCC[16] __attribute__((scn_string_literal)) = "NOT FOUND HIMO\012";
static const char data_42DDC[10] __attribute__((scn_string_literal)) = "come on!\012";
static const char data_42DE6[9] __attribute__((scn_string_literal)) = "LGWS201G";
static const char data_42DEF[9] __attribute__((scn_string_literal)) = "LGWS202G";
static const char data_42DF8[9] __attribute__((scn_string_literal)) = "LGWS203G";
static const char data_42E01[9] __attribute__((scn_string_literal)) = "LGWS204G";
static const char data_42E0A[9] __attribute__((scn_string_literal)) = "LGWS205G";
static const char data_42E13[9] __attribute__((scn_string_literal)) = "LGWS206G";
static const char data_42E1C[9] __attribute__((scn_string_literal)) = "LGWS207G";
static const char data_42E25[9] __attribute__((scn_string_literal)) = "LGWS261G";
static const char data_42E2E[9] __attribute__((scn_string_literal)) = "LGWS262G";
static const char data_42E37[9] __attribute__((scn_string_literal)) = "LGWS263G";
static const char data_42E40[9] __attribute__((scn_string_literal)) = "LGWS264G";
static const char data_42E49[9] __attribute__((scn_string_literal)) = "LGWS265G";
static const char data_42E52[9] __attribute__((scn_string_literal)) = "LGWS266G";
static const char data_42E5B[9] __attribute__((scn_string_literal)) = "LGWS267G";
static const Vector4 data_42E64 = {.x = 0.3f, .y = 0.3f, .z = 0.3f, .w = 0.3f};
static const char data_42E74[10] __attribute__((scn_string_literal)) = "E1018A080";
static const char data_42E7E[10] __attribute__((scn_string_literal)) = "E1018A079";
static const char data_42E88[10] __attribute__((scn_string_literal)) = "E1018A078";
static const char data_42E92[10] __attribute__((scn_string_literal)) = "E1018A077";
static const char data_42E9C[10] __attribute__((scn_string_literal)) = "E1018A075";
static const char data_42EA6[238] = "E1018A074\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\034\007\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000V\325\377\377\000\000\000\000\000\000\000\000P\332\377\377\000\000\000\000\000\000\000\000\224\336\377\377\000\000\000\000\"\002\000\000\000\000\000\000\000\000\000\000\000\000\000\000\006\333\377\377\000\000\000\000\000\000\000\000\203\315\377\377\000\000\000\000\000\000\000\000\006\333\377\377\000\000\000\000\"\002\000\000\000\000\000\000\000\000\000\000\000\000\000\000(\275\377\377\000\000\000\000\000\000\000\0004\323\377\377\000\000\000\000\000\000\000\000\344\330\377\377\000\000\000\000\330\002\000\000\000\000\000\000\000\000\000\000\000\000\000\000\232\271\377\377\000\000\000\000\000\000\000\000\240\324\377\377\000\000\000\000\000\000\000\000\240\324\377\377\000\000\000\000\"\002";
static const char data_42F94[11] __attribute__((scn_string_literal)) = "SA1102A001";
static const char data_42F9F[11] __attribute__((scn_string_literal)) = "SA1102A002";
static const char data_42FAA[11] __attribute__((scn_string_literal)) = "SA1102A003";
static const char data_42FB5[11] __attribute__((scn_string_literal)) = "SA1102A004";
static const char data_42FC0[11] __attribute__((scn_string_literal)) = "SA1102A005";
static const char data_42FCB[6] __attribute__((scn_string_literal)) = "YKB_M";
static const char data_42FD1[6] __attribute__((scn_string_literal)) = "YKB_M";
static const char data_42FD7[6] __attribute__((scn_string_literal)) = "YKC_M";
static const char data_42FDD[10] __attribute__((scn_string_literal)) = "E1018A036";
static const char data_42FE7[9] __attribute__((scn_string_literal)) = "01GMO";
static const Vector4 data_42FF0 = {.x = 1.0f, .y = 0.5f, .z = 0.5f, .w = 0.5f};
static const float data_43000[3] = {0.1f, 950.0f, 0.96f};
static const int32_t data_4300C[4] = {-15693310, 5461, -3458, -3640};
static const Vector3i data_4301C = {.x = 5643, .y = 364, .z = -2184};
static Vector3 data_43028 = {.x = 0.01f, .y = 0.0f, .z = 0.0f};
static Vector3 data_43034 = {.x = -0.01f, .y = 0.0f, .z = 0.0f};
static Vector3 data_43040 = {.x = 0.005f, .y = 0.1428f, .z = 0.0f};
static Vector3i data_4304C = {.x = 0, .y = 0, .z = 364};
static Vector3i data_43058 = {.x = 0, .y = 0, .z = -364};
static Vector3i data_43064 = {.x = 0, .y = 0, .z = 182};
static Vector3 data_43070 = {.x = 0.01f, .y = 0.0f, .z = 0.0f};
static Vector3 data_4307C = {.x = -0.01f, .y = 0.0f, .z = 0.0f};
static Vector3 data_43088 = {.x = 0.0f, .y = 0.1428f, .z = 0.0f};
static Vector3i data_43094 = {.x = 0, .y = 0, .z = 364};
static Vector3i data_430A0 = {.x = 0, .y = 0, .z = -364};
static Vector3i data_430AC = {.x = 0, .y = 0, .z = 182};
static Vector3 data_430B8 = {.x = 0.01f, .y = 0.0f, .z = 0.0f};
static Vector3 data_430C4 = {.x = -0.01f, .y = 0.0f, .z = 0.0f};
static Vector3 data_430D0 = {.x = 0.0f, .y = 0.1428f, .z = 0.0f};
static Vector3i data_430DC = {.x = 0, .y = 0, .z = 364};
static Vector3i data_430E8 = {.x = 0, .y = 0, .z = -364};
static Vector3i data_430F4 = {.x = 0, .y = 0, .z = 182};
static const Vector3 data_43100 = {.x = 17.0583f, .y = 0.8762f, .z = 4.3862f};
static const Vector3i data_4310C = {.x = 0, .y = 64160, .z = 0};
static Vector3 data_43118 = {.x = 0.01f, .y = 0.0f, .z = 0.0f};
static Vector3 data_43124 = {.x = -0.01f, .y = 0.0f, .z = 0.0f};
static Vector3 data_43130 = {.x = 0.0f, .y = 0.1428f, .z = 0.0f};
static Vector3i data_4313C = {.x = 0, .y = 0, .z = 364};
static Vector3i data_43148 = {.x = 0, .y = 0, .z = -364};
static Vector3i data_43154 = {.x = 0, .y = 0, .z = 182};
static const Vector3 data_43160 = {.x = 17.0583f, .y = 0.8762f, .z = 4.3862f};
static const Vector3i data_4316C = {.x = 0, .y = 64160, .z = 0};
static Vector3 data_43178 = {.x = 0.01f, .y = 0.0f, .z = 0.0f};
static Vector3 data_43184 = {.x = -0.01f, .y = 0.0f, .z = 0.0f};
static Vector3 data_43190 = {.x = 0.0f, .y = 0.1428f, .z = 0.0f};
static Vector3i data_4319C = {.x = 0, .y = 0, .z = 364};
static Vector3i data_431A8 = {.x = 0, .y = 0, .z = -364};
static Vector3i data_431B4 = {.x = 0, .y = 0, .z = 182};
static Vector3 data_431C0 = {.x = 0.01f, .y = 0.0f, .z = 0.0f};
static Vector3 data_431CC = {.x = -0.01f, .y = 0.0f, .z = 0.0f};
static Vector3 data_431D8 = {.x = 0.0f, .y = 0.1428f, .z = 0.0f};
static Vector3i data_431E4 = {.x = 0, .y = 0, .z = 364};
static Vector3i data_431F0 = {.x = 0, .y = 0, .z = -364};
static Vector3i data_431FC = {.x = 0, .y = 0, .z = 182};
static const int32_t data_43208[9] = {0, 0, 32768, 0, 0, 19660, 5461, -3458, -3640};
static const Vector3i data_4322C = {.x = 5643, .y = 364, .z = -2184};
static Vector3 data_43238 = {.x = 0.01f, .y = 0.0f, .z = 0.0f};
static Vector3 data_43244 = {.x = -0.01f, .y = 0.0f, .z = 0.0f};
static Vector3 data_43250 = {.x = 0.005f, .y = 0.1428f, .z = 0.0f};
static Vector3i data_4325C = {.x = 0, .y = 0, .z = 364};
static Vector3i data_43268 = {.x = 0, .y = 0, .z = -364};
static Vector3i data_43274 = {.x = 0, .y = 0, .z = 182};
static Vector3 data_43280 = {.x = 0.01f, .y = 0.0f, .z = 0.0f};
static Vector3 data_4328C = {.x = -0.01f, .y = 0.0f, .z = 0.0f};
static Vector3 data_43298 = {.x = 0.005f, .y = 0.1428f, .z = 0.0f};
static Vector3i data_432A4 = {.x = 0, .y = 0, .z = 364};
static Vector3i data_432B0 = {.x = 0, .y = 0, .z = -364};
static Vector3i data_432BC = {.x = 0, .y = 0, .z = 182};
static const int32_t data_432C8[6] = {0, 0, 32768, 0, 0, 25486};
static const Vector3i data_432E0 = {.x = -2366, .y = 4369, .z = -5097};
static Vector3 data_432EC = {.x = 0.01f, .y = 0.0f, .z = 0.0f};
static Vector3 data_432F8 = {.x = -0.01f, .y = 0.0f, .z = 0.0f};
static Vector3 data_43304 = {.x = 0.005f, .y = 0.1428f, .z = 0.0f};
static Vector3i data_43310 = {.x = 0, .y = 0, .z = 364};
static Vector3i data_4331C = {.x = 0, .y = 0, .z = -364};
static Vector3i data_43328 = {.x = 0, .y = 0, .z = 182};
static const Vector3 data_43334 = {.x = 17.0583f, .y = 0.8762f, .z = 4.3862f};
static const Vector3i data_43340 = {.x = 0, .y = 64160, .z = 0};
static Vector3 data_4334C = {.x = 0.0f, .y = 0.1541f, .z = 0.0f};
static const int32_t data_43358[57] = {
  0, 0, -3093, 0, 0, -1455, 0, 0, 0, 0, 0, -3275, 0, 0, -2001, 0, 0, -1637, 0, 0, 0, 0, 0, -12193, 0, 0, -12921, 0, 0, -4185, 0, 0, -363, 0, 0,
  -14195, 0, 0, -10919, 0, 0, -4550, 0, 0, 181, 0, 0, -15651, 0, 0, -10373, 0, 0, -6005, 0, -909, 181
};
static const int32_t data_4343C[57] = {
  0, 0, -1490, 0, 0, 2790, 0, 0, 2365, 0, 1092, -6861, 0, 727, -13293, 0, 0, 1275, 0, 727, 1819, 0, 0, 200, 0, 0, -5633, 0, 0, -2293, 0, 0, 1819, 0,
  0, -432, 0, 0, -4641, 0, 0, -3053, 0, 149, 1639, 0, 0, -2156, 0, 0, -8617, 0, 0, -1688, 0, -582, 412
};
static const int32_t data_43520[57] = {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0
};
static const Vector4 data_43604 = {.x = 1.0f, .y = 0.5f, .z = 0.5f, .w = 0.5f};
static const char data_43614[6] __attribute__((scn_string_literal)) = "YKD_M";
static const char data_4361A[11] __attribute__((scn_string_literal)) = "bgm101.snd";
static const char data_43625[13] __attribute__((scn_string_literal)) = "a1_fukan.snd";
static const char data_43632[6] __attribute__((scn_string_literal)) = "A0173";
static const char data_43638[16] __attribute__((scn_string_literal)) = "AKIRA_ROOM_ %d\012";
static const char data_43648[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_4364D[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_43652[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_43657[11] __attribute__((scn_string_literal)) = "bgm070.snd";
static const char data_43662[13] __attribute__((scn_string_literal)) = "a1_okozu.snd";
static const char data_4366F[9] __attribute__((scn_string_literal)) = "01KOZ";
static const Vector4 data_43678 = {.x = 1.0f, .y = 0.5f, .z = 0.5f, .w = 0.5f};
static const float data_43688[4] = {1.0f, 0.2f, 0.2f, 0.2f};
static Vector3 data_43698 = {.x = 0.0f, .y = 0.0f, .z = 0.0f};
static const char data_436A4[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_436A9[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_436AE[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_436B3[11] __attribute__((scn_string_literal)) = "bgm050.snd";
static const char data_436BE[13] __attribute__((scn_string_literal)) = "a1_tmryo.snd";
static const char data_436CB[9] __attribute__((scn_string_literal)) = "01LET";
static const Vector4 data_436D4 = {.x = 1.0f, .y = 0.5f, .z = 0.5f, .w = 0.5f};
static const Vector3i data_436E4 = {.x = -2548, .y = -2184, .z = 546};
static const Vector3i data_436F0 = {.x = 5279, .y = -2730, .z = -910};
static const Vector4 data_436FC = {.x = 1.0f, .y = 0.5f, .z = 0.5f, .w = 0.5f};
static const Vector4 data_4370C = {.x = 1.0f, .y = 0.5f, .z = 0.5f, .w = 0.5f};
static const char data_4371C[14] __attribute__((scn_string_literal)) = "scene/01/JOMO";
static const char data_4372A[7] __attribute__((scn_string_literal)) = "NINJIN";
static const char data_43731[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_43736[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_4373B[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_43740[11] __attribute__((scn_string_literal)) = "tsm003.snd";
static const char data_4374B[13] __attribute__((scn_string_literal)) = "a1_ksnin.snd";
static const char data_43758[6] __attribute__((scn_string_literal)) = "A0122";
static const char data_4375E[10] __attribute__((scn_string_literal)) = "NINJIN";
static const Vector3i data_43768 = {.x = -3457, .y = 4731, .z = 0};
static const Vector3i data_43774 = {.x = -13285, .y = -2365, .z = 0};
static const Vector3i data_43780 = {.x = -3819, .y = -2365, .z = 0};
static const Vector3i data_4378C = {.x = 0, .y = 0, .z = 0};
static const Vector3i data_43798 = {.x = 546, .y = 4914, .z = 1820};
static const int32_t data_437A4[10] = {57344, 0, 0, 3276, 1092, 0, 4004, 3094, 0, 7463};
static const int32_t data_437CC[10] = {60984, 2184, 0, 3640, 61531, 0, 61895, 62441, 0, 62077};
static const int32_t data_437F4[10] = {57344, 0, 0, 0, 1092, 0, 4004, 3094, 0, 7463};
static const Vector3 data_4381C = {.x = 10.9437f, .y = 0.75f, .z = -2.49f};
static const Vector3 data_43828 = {.x = 0.005f, .y = 0.0f, .z = 0.005f};
static const Vector3 data_43834 = {.x = 0.0f, .y = 0.0f, .z = 0.0f};
static const Vector4 data_43840 = {.x = 1.0f, .y = 0.5f, .z = 0.5f, .w = 0.5f};
static const char data_43850[14] __attribute__((scn_string_literal)) = "scene/01/JOMO";
static const char data_4385E[7] __attribute__((scn_string_literal)) = "HOUMON";
static const char data_43865[6] __attribute__((scn_string_literal)) = "YKB_M";
static const char data_4386B[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_43870[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_43875[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_4387A[11] __attribute__((scn_string_literal)) = "bgm085.snd";
static const char data_43885[13] __attribute__((scn_string_literal)) = "a1_gakri.snd";
static const char data_43892[6] __attribute__((scn_string_literal)) = "01GAK";
static const char data_43898[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_4389D[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_438A2[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_438A7[11] __attribute__((scn_string_literal)) = "bgm085.snd";
static const char data_438B2[13] __attribute__((scn_string_literal)) = "a1_gakri.snd";
static const char data_438BF[6] __attribute__((scn_string_literal)) = "01GAK";
static const char data_438C5[6] __attribute__((scn_string_literal)) = "01FRE";
static const char data_438CB[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_438D0[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_438D5[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_438DA[11] __attribute__((scn_string_literal)) = "bgm095.snd";
static const char data_438E5[11] __attribute__((scn_string_literal)) = "a1_ito.snd";
static const char data_438F0[6] __attribute__((scn_string_literal)) = "RFB_L";
static const char data_438F6[6] __attribute__((scn_string_literal)) = "NKK_L";
static const char data_438FC[48] = "HOUMON\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\034\007\000\000\000\000\000\000ﾇ1\000\000\000\000\000\000ﾇ1";
static const char data_4392C[6] __attribute__((scn_string_literal)) = "YKC_M";
static const char data_43932[6] __attribute__((scn_string_literal)) = "YKC_M";
static const Vector4 data_43938 = {.x = 1.0f, .y = 0.5f, .z = 0.5f, .w = 0.5f};
static const Vector4 data_43948 = {.x = 1.0f, .y = 0.5f, .z = 0.5f, .w = 0.5f};
static const Vector3 data_43958 = {.x = 14.8f, .y = 0.0f, .z = 2.4f};
static const char data_43964[30] = "$TTMS246G.MT5\000$MALS501G.MT5";
static const char data_43982[13] __attribute__((scn_string_literal)) = "a1_inet2.snd";
static const char data_4398F[11] __attribute__((scn_string_literal)) = "bgm115.snd";
static const char data_4399A[7] __attribute__((scn_string_literal)) = "A01124";
static const char data_439A1[22] __attribute__((scn_string_literal)) = "母屋内強制睡眠始動！\012";
static const char data_439B7[17] __attribute__((scn_string_literal)) = "JOMO_AKIRA = %d\012";
static const Vector4 data_439C8 = {.x = 1.0f, .y = 0.5f, .z = 0.5f, .w = 0.5f};
static const int32_t data_439D8[57] = {
  0, 0, -6003, 0, 0, -908, 0, 0, 0, 0, 0, -3819, 0, 0, -7094, 0, 0, -4547, 0, 0, 0, 0, 0, -2909, 0, 0, -8550, 0, -544, -4001, 0, 0, 0, 0, 0, -4363, 0,
  0, -8004, 0, -908, -2180, 0, 0, 0, 0, -1634, -6366, 0, -2000, -4365, 0, -1818, -2182, 0, 0, 0
};
static const int32_t data_43ABC[57] = {
  -3820, -1636, -4548, 6370, 4186, 4914, -9646, -364, 2730, 0, 0, -5641, 0, 1273, -9826, 0, 0, -6368, 0, 0, 2911, 0, 0, -1819, 0, -363, -12374, 0, 0,
  -6369, 0, 0, 2184, 0, 0, -4185, 0, 0, -11646, 0, -1273, -4913, 0, 0, 2184, 0, 0, -7825, 0, -3821, -8227, 0, -727, -6733, 0, 0, 2911
};
static const int32_t data_43BA0[57] = {
  0, 0, -6002, 0, 3276, -907, 0, 3458, 546, 0, 0, -3819, 0, 0, -5091, 0, 2184, -8550, 0, -545, 546, 0, 0, -2908, 0, 0, -6911, 0, -543, -8733, 0, 0,
  727, 0, 0, -4362, 0, 0, -6183, 0, -907, -7275, 0, 0, 0, 0, -1633, -9095, 0, -1999, -4000, 0, -2727, -9461, 0, 0, 1637
};
static const int32_t data_43C84[57] = {
  2911, 0, -4910, 2366, -181, -3637, 5641, -363, 910, 0, 0, -1634, 0, 0, -7093, 0, 0, -1088, 0, 0, 0, 0, 0, -2908, 0, 0, -7093, 0, -543, 1095, 0, 0,
  0, 0, 0, -3270, 0, 0, -8004, 0, -907, 2916, 0, 0, 0, 0, -1633, -3818, 0, -1817, -4364, 0, -543, 2367, 0, 0, -181
};
static const char data_43D68[7] __attribute__((scn_string_literal)) = "SA1071";
static const char data_43D6F[11] __attribute__((scn_string_literal)) = "SA1071A001";
static const char data_43D7A[11] __attribute__((scn_string_literal)) = "SA1071A002";
static const char data_43D85[11] __attribute__((scn_string_literal)) = "SA1071A003";
static const char data_43D90[11] __attribute__((scn_string_literal)) = "SA1071A004";
static const char data_43D9B[11] __attribute__((scn_string_literal)) = "amb037.snd";
static const char data_43DA6[11] __attribute__((scn_string_literal)) = "bgm009.snd";
static const char data_43DB1[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_43DB6[13] __attribute__((scn_string_literal)) = "f1omoyaa.snd";
static const char data_43DC3[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_43DC8[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_43DCD[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_43DD2[17] __attribute__((scn_string_literal)) = "JOMO_AKIRA = %d\012";
static const char data_43DE3[7] __attribute__((scn_string_literal)) = "SA1071";
static const char data_43DEA[6] __attribute__((scn_string_literal)) = "01TEL";
static const char data_43DF0[11] __attribute__((scn_string_literal)) = "bgm029.snd";
static const char data_43DFB[13] __attribute__((scn_string_literal)) = "a1_telp.snd";
static const Vector4 data_43E08 = {.x = 1.0f, .y = 0.5f, .z = 0.5f, .w = 0.5f};
static const Vector3 data_43E18 = {.x = 15.74418f, .y = 0.63152f, .z = 2.51917f};
static const Vector3 data_43E24 = {.x = 15.771f, .y = 0.6328f, .z = 2.3943f};
static const Vector3 data_43E30 = {.x = 0.0f, .y = -10.0f, .z = 100.0f};
static const Vector3i data_43E3C = {.x = 0, .y = 16167, .z = 0};
static const Vector3i data_43E48 = {.x = -6735, .y = -2366, .z = 546};
static const Vector3 data_43E54 = {.x = 22.36599f, .y = 0.43175f, .z = 5.79331f};
static const Vector3 data_43E60 = {.x = 0.0f, .y = -10.0f, .z = 100.0f};
static const Vector3i data_43E6C = {.x = 0, .y = 16167, .z = 0};
static const Vector3 data_43E78 = {.x = 22.374001f, .y = 0.4277f, .z = 5.5813f};
static const char data_43E84[16] __attribute__((scn_string_literal)) = "$MMA0201G.MT5";
static const char data_43E94[11] __attribute__((scn_string_literal)) = "tym001.snd";
static const char data_43E9F[13] __attribute__((scn_string_literal)) = "a1_hukum.snd";
static const char data_43EAC[6] __attribute__((scn_string_literal)) = "01MEM";
static const char data_43EB2[18] __attribute__((scn_string_literal)) = "AKIRA_ROOM_ %d\012";
static const Vector4 data_43EC4 = {.x = 1.0f, .y = 0.5f, .z = 0.5f, .w = 0.5f};
static const Vector3 data_43ED4 = {.x = 1.5f, .y = 1.5f, .z = 1.5f};
static const char data_43EE0[6] __attribute__((scn_string_literal)) = "YKB_M";
static const char data_43EE6[6] __attribute__((scn_string_literal)) = "YKC_M";
static const char data_43EEC[11] __attribute__((scn_string_literal)) = "bgm068.snd";
static const char data_43EF7[13] __attribute__((scn_string_literal)) = "a1_nozok.snd";
static const char data_43F04[8] __attribute__((scn_string_literal)) = "01SKIA";
static const Vector4 data_43F0C = {.x = 1.0f, .y = 0.5f, .z = 0.5f, .w = 0.5f};
static const char data_43F1C[16] __attribute__((scn_string_literal)) = "$TUB0210G.MT5";
static const char data_43F2C[11] __attribute__((scn_string_literal)) = "bgm077.snd";
static const char data_43F37[12] __attribute__((scn_string_literal)) = "a1_butu.snd";
static const char data_43F43[25] = "A01136\000\000\000\000\000\200?\315\314L>\315\314L>\315\314L>";
static const Vector4 data_43F5C = {.x = 1.0f, .y = 0.5f, .z = 0.5f, .w = 0.5f};
static const Vector3i data_43F6C = {.x = -3273, .y = -364, .z = -2000};
static const Vector3 data_43F78 = {.x = 4.16f, .y = 0.37f, .z = 0.57f};
static const int32_t data_43F84[3] = {-3273, -364, -2000};
static const Vector4 data_43F90 = {.x = 1.0f, .y = 0.5f, .z = 0.5f, .w = 0.5f};
static const char data_43FA0[11] __attribute__((scn_string_literal)) = "bgm085.snd";
static const char data_43FAB[13] __attribute__((scn_string_literal)) = "a1_hukhe.snd";
static const char data_43FB8[6] __attribute__((scn_string_literal)) = "A0160";
static const char data_43FBE[17] __attribute__((scn_string_literal)) = "JOMO_AKIRA = %d\012";
static const char data_43FCF[16] __attribute__((scn_string_literal)) = "$BNKT2DBG.MT5";
static const char data_43FDF[11] __attribute__((scn_string_literal)) = "bgm029.snd";
static const char data_43FEA[13] __attribute__((scn_string_literal)) = "a1_tumir.snd";
static const char data_43FF7[9] __attribute__((scn_string_literal)) = "01BNK";
static const Vector4 data_44000 = {.x = 1.0f, .y = 0.5f, .z = 0.5f, .w = 0.5f};
static const char data_44010[44] = "$SRP3H01G.MT5\000$TIRS701G.MT5\000$OKAK301G.MT5";
static const char data_4403C[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_44041[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_44046[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_4404B[11] __attribute__((scn_string_literal)) = "bgm101.snd";
static const char data_44056[13] __attribute__((scn_string_literal)) = "a1_chofk.snd";
static const char data_44063[9] __attribute__((scn_string_literal)) = "01MON";
static const Vector4 data_4406C = {.x = 1.0f, .y = 0.5f, .z = 0.5f, .w = 0.5f};
static const char data_4407C[11] __attribute__((scn_string_literal)) = "bgm100.snd";
static const char data_44087[13] __attribute__((scn_string_literal)) = "a1_hongo.snd";
static const char data_44094[8] __attribute__((scn_string_literal)) = "01TAZ";
static const Vector4 data_4409C = {.x = 1.0f, .y = 0.5f, .z = 0.5f, .w = 0.5f};
static const float data_440AC[3] = {22.7349f, 0.0f, -0.0141f};
static const Vector4 data_440B8 = {.x = 1.0f, .y = 0.5f, .z = 0.5f, .w = 0.5f};
static const char data_440C8[11] __attribute__((scn_string_literal)) = "bgm114.snd";
static const char data_440D3[13] __attribute__((scn_string_literal)) = "a1_kikum.snd";
static const char data_440E0[6] __attribute__((scn_string_literal)) = "01EAR";
static const char data_440E6[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_440EB[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_440F0[5] __attribute__((scn_string_literal)) = "FREE";
static const char data_440F5[11] __attribute__((scn_string_literal)) = "bgm070.snd";
static const char data_44100[13] __attribute__((scn_string_literal)) = "a1_nhana.snd";
static const char data_4410D[7] __attribute__((scn_string_literal)) = "01BTU";
static const Vector4 data_44114 = {.x = 1.0f, .y = 0.5f, .z = 0.5f, .w = 0.5f};
static const char data_44124[24] = "HANA\000\000\000\000\000\000\200?\315\314L>\315\314L>\315\314L>";
static const Vector3 data_4413C = {.x = 4.0f, .y = 0.0f, .z = -2.2f};
static Vector3 data_44148 = {.x = 0.75f, .y = 0.0f, .z = 0.0f};
static const Vector3 data_44154 = {.x = 4.0f, .y = 0.0f, .z = -2.2f};
static Vector3 data_44160 = {.x = 0.0f, .y = 0.0f, .z = 0.0f};
static const char data_4416C[6] __attribute__((scn_string_literal)) = "A0169";
static const char data_44172[11] __attribute__((scn_string_literal)) = "bgm063.snd";
static const char data_4417D[15] __attribute__((scn_string_literal)) = "a1_jytel.snd";
static const Vector4 data_4418C = {.x = 1.0f, .y = 0.5f, .z = 0.5f, .w = 0.5f};
static const Vector4 data_4419C = {.x = 1.0f, .y = 0.2f, .z = 0.2f, .w = 0.2f};
static const Vector3 data_441AC = {.x = -0.02f, .y = -0.01f, .z = -0.01f};
static const Vector3i data_441B8 = {.x = 11962, .y = 62353, .z = 60708};
static const Vector3i data_441C4 = {.x = 4915, .y = -546, .z = -182};
static const Vector4 data_441D0 = {.x = 1.0f, .y = 0.5f, .z = 0.5f, .w = 0.5f};
static const char data_441E0[6] __attribute__((scn_string_literal)) = "YKB_M";
static const char data_441E6[11] __attribute__((scn_string_literal)) = "bgm085.snd";
static const char data_441F1[13] __attribute__((scn_string_literal)) = "a1_uresi.snd";
static const char data_441FE[6] __attribute__((scn_string_literal)) = "01OKO";
static const char data_44204[16] __attribute__((scn_string_literal)) = "AKIRA_ROOM_ %d\012";
static const char data_44214[6] __attribute__((scn_string_literal)) = "YKB_M";
static const char data_4421A[6] __attribute__((scn_string_literal)) = "YKC_M";
static const Vector4 data_44220 = {.x = 1.0f, .y = 0.5f, .z = 0.5f, .w = 0.5f};
static const char data_44230[16] __attribute__((scn_string_literal)) = "AKIRA_ROOM_ %d\012";
static const char data_44240[11] __attribute__((scn_string_literal)) = "bgm085.snd";
static const char data_4424B[13] __attribute__((scn_string_literal)) = "a1_jibun.snd";
static const char data_44258[6] __attribute__((scn_string_literal)) = "01JIB";
static const char data_4425E[6] __attribute__((scn_string_literal)) = "YKC_M";
static const char data_44264[16] __attribute__((scn_string_literal)) = "AKIRA_ROOM_ %d\012";
static const Vector4 data_44274 = {.x = 1.0f, .y = 0.5f, .z = 0.5f, .w = 0.5f};
static const char data_44284[11] __attribute__((scn_string_literal)) = "bgm063.snd";
static const char data_4428F[13] __attribute__((scn_string_literal)) = "a1_fukyu.snd";
static const char data_4429C[6] __attribute__((scn_string_literal)) = "01GET";
static const char data_442A2[6] __attribute__((scn_string_literal)) = "YKB_M";
static const Vector3i data_442A8 = {.x = 4186, .y = -1455, .z = 2547};
static const char data_442B4[8] __attribute__((scn_string_literal)) = "YKB_M";
static const Vector3i data_442BC = {.x = 4186, .y = -1455, .z = 2547};
static const char data_442C8[7] __attribute__((scn_string_literal)) = "SA1093";
static const char data_442CF[7] __attribute__((scn_string_literal)) = "SA1093";
static const char data_442D6[11] __attribute__((scn_string_literal)) = "SA1093A001";
static const char data_442E1[11] __attribute__((scn_string_literal)) = "SA1093B001";
static const char data_442EC[11] __attribute__((scn_string_literal)) = "SA1093A002";
static const char data_442F7[11] __attribute__((scn_string_literal)) = "SA1093B002";
static const char data_44302[11] __attribute__((scn_string_literal)) = "SA1093B003";
static const char data_4430D[11] __attribute__((scn_string_literal)) = "SA1093A003";
static const char data_44318[11] __attribute__((scn_string_literal)) = "SA1093B004";
static const char data_44323[11] __attribute__((scn_string_literal)) = "SA1093B005";
static const char data_4432E[11] __attribute__((scn_string_literal)) = "SA1093B006";
static const char data_44339[11] __attribute__((scn_string_literal)) = "SA1093A004";
static const char data_44344[11] __attribute__((scn_string_literal)) = "SA1093B007";
static const char data_4434F[11] __attribute__((scn_string_literal)) = "SA1093A001";
static const char data_4435A[11] __attribute__((scn_string_literal)) = "SA1093B001";
static const char data_44365[11] __attribute__((scn_string_literal)) = "SA1093A005";
static const char data_44370[11] __attribute__((scn_string_literal)) = "SA1093B008";
static const char data_4437B[11] __attribute__((scn_string_literal)) = "SA1093B009";
static const char data_44386[11] __attribute__((scn_string_literal)) = "SA1093A006";
static const char data_44391[11] __attribute__((scn_string_literal)) = "SA1093B006";
static const char data_4439C[11] __attribute__((scn_string_literal)) = "SA1093A004";
static const char data_443A7[11] __attribute__((scn_string_literal)) = "SA1093B007";
static const char data_443B2[11] __attribute__((scn_string_literal)) = "SA1093A001";
static const char data_443BD[11] __attribute__((scn_string_literal)) = "SA1093B001";
static const char data_443C8[11] __attribute__((scn_string_literal)) = "SA1093A005";
static const char data_443D3[11] __attribute__((scn_string_literal)) = "SA1093B008";
static const char data_443DE[11] __attribute__((scn_string_literal)) = "SA1093B011";
static const char data_443E9[11] __attribute__((scn_string_literal)) = "SA1093A007";
static const char data_443F4[11] __attribute__((scn_string_literal)) = "SA1093B010";
static const char data_443FF[11] __attribute__((scn_string_literal)) = "SA1093A004";
static const char data_4440A[14] __attribute__((scn_string_literal)) = "SA1093B012";
static const Vector3 data_44418 = {.x = 0.0f, .y = 0.0f, .z = 0.0f};
static const Vector3i data_44424 = {.x = 0, .y = 0, .z = 0};
static const uint32_t data_44430 = 157;
typedef struct Row_44434 { float w0; int32_t w1; } Row_44434;
static const Row_44434 data_44434[19] = {
  {0.0f, 174},
  {30.0f, 192},
  {68.0f, 199},
  {74.0f, 226},
  {76.0f, 236},
  {65.0f, 248},
  {38.0f, 270},
  {15.0f, 1196},
  {15.0f, 1214},
  {50.0f, 1218},
  {55.0f, 1250},
  {55.0f, 1254},
  {48.0f, 1262},
  {27.0f, 1267},
  {9.0f, 1269},
  {6.0f, 1272},
  {5.0f, 1275},
  {3.5f, 1278},
  {0.0f, -1}
};
static const int32_t data_444CC = 0;
static const char data_444D0[16] __attribute__((scn_string_literal)) = "$SHAS502G.MT5";
static const char data_444E0[7] __attribute__((scn_string_literal)) = "SA1094";
static const char data_444E7[12] __attribute__((scn_string_literal)) = "e1omoya.snd";
static const char data_444F3[7] __attribute__((scn_string_literal)) = "SA1094";
static const char data_444FA[11] __attribute__((scn_string_literal)) = "SA1094B001";
static const char data_44505[11] __attribute__((scn_string_literal)) = "SA1094A001";
static const char data_44510[11] __attribute__((scn_string_literal)) = "SA1094B002";
static const char data_4451B[11] __attribute__((scn_string_literal)) = "SA1094A002";
static const char data_44526[11] __attribute__((scn_string_literal)) = "SA1094B003";
static const char data_44531[11] __attribute__((scn_string_literal)) = "SA1094A003";
static const char data_4453C[11] __attribute__((scn_string_literal)) = "SA1094B004";
static const char data_44547[11] __attribute__((scn_string_literal)) = "SA1094A004";
static const char data_44552[11] __attribute__((scn_string_literal)) = "SA1094B005";
static const char data_4455D[11] __attribute__((scn_string_literal)) = "SA1094A005";
static const char data_44568[11] __attribute__((scn_string_literal)) = "SA1094A006";
static const char data_44573[11] __attribute__((scn_string_literal)) = "SA1094B006";
static const char data_4457E[14] __attribute__((scn_string_literal)) = "SA1094A007";
static const Vector3 data_4458C = {.x = 16.96f, .y = 0.0f, .z = 1.18f};
static const uint32_t data_44598 = 273;
static const int32_t data_4459C[2] = {0, 284};
static const float data_445A4 = 30.0f;
static const int32_t data_445A8 = 302;
static const float data_445AC = 70.0f;
static const int32_t data_445B0 = 309;
static const float data_445B4 = 78.0f;
static const int32_t data_445B8 = 336;
static const float data_445BC = 80.0f;
static const int32_t data_445C0 = 1237;
static const float data_445C4 = 80.0f;
static const int32_t data_445C8[4] = {1238, 0, -1, 0};
static const float data_445D8[3] = {16.31f, 0.0f, 1.83f};
static const char data_445E4[7] __attribute__((scn_string_literal)) = "SA1100";
static const char data_445EB[7] __attribute__((scn_string_literal)) = "SA1100";
static const char data_445F2[11] __attribute__((scn_string_literal)) = "SA1100B001";
static const char data_445FD[11] __attribute__((scn_string_literal)) = "SA1100A001";
static const char data_44608[11] __attribute__((scn_string_literal)) = "SA1100B002";
static const char data_44613[11] __attribute__((scn_string_literal)) = "SA1100A002";
static const char data_4461E[11] __attribute__((scn_string_literal)) = "SA1100B003";
static const char data_44629[11] __attribute__((scn_string_literal)) = "SA1100A003";
static const char data_44634[11] __attribute__((scn_string_literal)) = "SA1100B004";
static const char data_4463F[11] __attribute__((scn_string_literal)) = "SA1100A004";
static const char data_4464A[11] __attribute__((scn_string_literal)) = "SA1100B005";
static const char data_44655[11] __attribute__((scn_string_literal)) = "SA1100A005";
static const char data_44660[11] __attribute__((scn_string_literal)) = "SA1100B006";
static const char data_4466B[13] __attribute__((scn_string_literal)) = "SA1100A006";
static const Vector3 data_44678 = {.x = 16.96f, .y = 0.0f, .z = 1.18f};
static uint32_t data_44684 = 1;
static const shen_name data_44688 = SHEN_NAME('TEGA');
static const uint32_t data_4468C[6] = {0x71C74000U, 0, 0, 0, 0, 0};
static const int32_t data_446A4 = 524289;
static const shen_name data_446A8 = SHEN_NAME('TRNE');
static const uint32_t data_446AC[2] = {0x71C78000U, 0};
static const float data_446B4[4] = {0.3f, 0.0f, 0.0f, 0.0f};
static const int32_t data_446C4 = 131073;
static const shen_name data_446C8 = SHEN_NAME('ITBO');
static const uint32_t data_446CC[2] = {0x71C7C000U, 0};
static const float data_446D4[4] = {0.25f, 0.0f, 0.0f, 0.0f};
static const int32_t data_446E4 = 524289;
static const shen_name data_446E8 = SHEN_NAME('IGAK');
static const uint32_t data_446EC[6] = {0x71C78000U, 0, 0, 0, 0, 0};
static const int32_t data_44704 = 458753;
static const shen_name data_44708 = SHEN_NAME('IWKJ');
static const uint32_t data_4470C[2] = {0x71C7C000U, 0};
static const float data_44714[4] = {0.7f, 0.0f, 0.0f, 0.0f};
static const int32_t data_44724[2] = {-65535, 102};
static const uint32_t data_4472C = 0x71C7C000U;
static const float data_44730[5] = {1.472f, 1.18f, 0.885f, 0.0f, 0.0f};
static const int32_t data_44744 = 1;
static const shen_name data_44748 = SHEN_NAME('IWWH');
static const uint32_t data_4474C[2] = {0x71C74000U, 0};
static const float data_44754[2] = {0.1f, 0.0f};
static const int32_t data_4475C[3] = {4, 15, 1};
static const shen_name data_44768[7] = {SHEN_NAME('HIM1'), 0, 0, 0, 0, 0, 0};
static const int32_t data_44784 = 262145;
static const uint32_t data_44788[2] = {0x4C627469U, 0x78E3C000U};
static const float data_44790[5] = {-0.5f, 0.2f, 0.0f, 0.0f, 0.0f};
static const int32_t data_447A4 = 262145;
static const uint32_t data_447A8[2] = {0x52627469U, 0x78E3C000U};
static const float data_447B0[5] = {0.5f, 0.2f, 0.0f, 0.0f, 0.0f};
static const int32_t data_447C4 = 458756;
static const shen_name data_447C8 = SHEN_NAME('KIKJ');
static const uint32_t data_447CC[2] = {0x6AAA4000U, 0};
static const float data_447D4[4] = {0.7f, 0.0f, 0.0f, 0.0f};
static const int32_t data_447E4[2] = {-65532, 101};
static const uint32_t data_447EC = 0x6AAA4000U;
static const float data_447F0[5] = {1.964f, 1.138f, 0.5f, 0.0f, 0.0f};
static const int32_t data_44804 = 131076;
static const shen_name data_44808 = SHEN_NAME('BTBO');
static const uint32_t data_4480C[6] = {0x6AAA4000U, 0, 0x3EA8F5C2U, 0, 0, 0};
static const int32_t data_44824 = 458756;
static const shen_name data_44828 = SHEN_NAME('BMKJ');
static const uint32_t data_4482C[2] = {0x6AAA4000U, 0};
static const float data_44834[4] = {0.7f, 0.0f, 0.0f, 0.0f};
static const int32_t data_44844[2] = {-65532, 103};
static const uint32_t data_4484C = 0x6AAA4000U;
static const float data_44850[5] = {1.944f, 1.138f, 1.211f, 0.0f, 0.0f};
static const int32_t data_44864 = 196612;
static const shen_name data_44868 = SHEN_NAME('BMB1');
static const uint32_t data_4486C[2] = {0x78E34000U, 0};
static const float data_44874[2] = {0.05f, 0.0f};
static const int32_t data_4487C[3] = {1, 10, 196612};
static const shen_name data_44888 = SHEN_NAME('BMB2');
static const uint32_t data_4488C[2] = {0x78E34000U, 0};
static const float data_44894[2] = {0.05f, 0.0f};
static const int32_t data_4489C[3] = {1, 11, 327684};
static const shen_name data_448A8 = SHEN_NAME('BMB3');
static const uint32_t data_448AC[2] = {0x78E34000U, 0};
static const float data_448B4[3] = {0.25f, 0.0f, 0.0f};
static const int32_t data_448C0[2] = {26, 196612};
static const shen_name data_448C8 = SHEN_NAME('BMB4');
static const uint32_t data_448CC[2] = {0x78E34000U, 0};
static const float data_448D4[2] = {0.1f, 0.0f};
static const int32_t data_448DC[3] = {3, 9, 196612};
static const shen_name data_448E8 = SHEN_NAME('BMB5');
static const uint32_t data_448EC[2] = {0x78E34000U, 0};
static const float data_448F4[2] = {0.1f, 0.0f};
static const int32_t data_448FC[4] = {3, 20, -65532, 100};
static const uint32_t data_4490C = 0x78E34000U;
static const float data_44910[5] = {2.415f, 0.98f, -0.665f, 0.0f, 0.0f};
static const int32_t data_44924 = 131076;
static const shen_name data_44928 = SHEN_NAME('BMMC');
static const uint32_t data_4492C[4] = {0x78E34000U, 0, 0, 0};
static const int32_t data_4493C[3] = {55, 0, 4};
static const shen_name data_44948 = CHARA_HANA;
static const uint32_t data_4494C[2] = {0x78E34000U, 0};
static const float data_44954[4] = {0.18f, 0.0f, 0.0f, 0.0f};
static const int32_t data_44964 = 262148;
static const uint32_t data_44968[2] = {0x4C627462U, 0x78E34000U};
static const float data_44970[5] = {-0.5f, 0.2f, 0.0f, 0.0f, 0.0f};
static const int32_t data_44984 = 262148;
static const uint32_t data_44988[2] = {0x52627462U, 0x78E34000U};
static const float data_44990[5] = {0.5f, 0.2f, 0.0f, 0.0f, 0.0f};
static const int32_t data_449A4 = 6;
static const uint32_t data_449A8[7] = {0x6A6B7973U, 0, 0, 0, 0, 0, 0};
static const int32_t data_449C4 = 131078;
static const shen_name data_449C8 = SHEN_NAME('DDKK');
static const uint32_t data_449CC[6] = {0x78E3D555U, 0, 0, 0, 0, 0};
static const int32_t data_449E4 = 131078;
static const shen_name data_449E8 = SHEN_NAME('DDFP');
static const uint32_t data_449EC[6] = {0x78E30000U, 0, 0, 0, 0, 0};
static const int32_t data_44A04 = 131078;
static const shen_name data_44A08 = SHEN_NAME('DDZ1');
static const uint32_t data_44A0C[2] = {0x78E30000U, 0};
static const float data_44A14[4] = {0.13f, 0.0f, 0.0f, 0.0f};
static const int32_t data_44A24 = 131078;
static const shen_name data_44A28 = SHEN_NAME('DDYK');
static const uint32_t data_44A2C[2] = {0x78E30000U, 0};
static const float data_44A34[4] = {0.1f, 0.0f, 0.0f, 0.0f};
static const int32_t data_44A44 = 196614;
static const shen_name data_44A48 = SHEN_NAME('D1C1');
static const uint32_t data_44A4C[2] = {0x78E3C000U, 0};
static const float data_44A54[2] = {0.2f, 0.0f};
static const int32_t data_44A5C[3] = {1, 13, 196614};
static const shen_name data_44A68 = SHEN_NAME('D1C2');
static const uint32_t data_44A6C[2] = {0x78E3C000U, 0};
static const float data_44A74[2] = {0.2f, 0.0f};
static const int32_t data_44A7C[3] = {1, 16, 262150};
static const shen_name data_44A88 = SHEN_NAME('D1C3');
static const uint32_t data_44A8C = 0x78E3C000U;
static const float data_44A90[4] = {-0.3f, 0.4f, 0.0f, 0.0f};
static const int32_t data_44AA0[2] = {17, 262150};
static const shen_name data_44AA8 = SHEN_NAME('D1C4');
static const uint32_t data_44AAC = 0x78E3C000U;
static const float data_44AB0[4] = {0.3f, 0.4f, 0.0f, 0.0f};
static const int32_t data_44AC0[2] = {17, 327686};
static const shen_name data_44AC8 = SHEN_NAME('DDR1');
static const uint32_t data_44ACC = 0x78E3C000U;
static const float data_44AD0[4] = {-0.4f, 0.0f, 0.25f, 0.0f};
static const int32_t data_44AE0[2] = {23, 327686};
static const shen_name data_44AE8 = SHEN_NAME('DDR2');
static const uint32_t data_44AEC = 0x78E3C000U;
static const float data_44AF0[4] = {-0.4f, 0.3f, 0.25f, 0.0f};
static const int32_t data_44B00[2] = {24, 262150};
static const shen_name data_44B08 = SHEN_NAME('DND1');
static const uint32_t data_44B0C = 0x78E30000U;
static const float data_44B10[4] = {0.5f, 0.1f, 0.0f, 0.0f};
static const int32_t data_44B20[2] = {29, 262150};
static const shen_name data_44B28 = SHEN_NAME('DND2');
static const uint32_t data_44B2C = 0x78E30000U;
static const float data_44B30[4] = {-0.5f, 0.1f, 0.0f, 0.0f};
static const int32_t data_44B40[2] = {29, 327686};
static const shen_name data_44B48 = SHEN_NAME('DDG1');
static const uint32_t data_44B4C[2] = {0x78E30000U, 0};
static const float data_44B54[3] = {0.35f, 0.0f, 0.0f};
static const int32_t data_44B60[2] = {18, 327686};
static const shen_name data_44B68 = SHEN_NAME('DDG2');
static const uint32_t data_44B6C[2] = {0x78E30000U, 0};
static const float data_44B74[3] = {0.46f, 0.0f, 0.0f};
static const int32_t data_44B80[2] = {19, 6};
static const uint32_t data_44B88[7] = {0x6B6C636BU, 0x78E34000U, 0, 0, 0, 0, 0};
static const int32_t data_44BA4 = 131078;
static const uint32_t data_44BA8[5] = {0x62696E6BU, 0x78E30000U, 0, 0, 0};
static const int32_t data_44BBC[3] = {60, 0, 524294};
static const uint32_t data_44BC8[7] = {0x6369706BU, 0x71C74000U, 0, 0, 0, 0, 0};
static const int32_t data_44BE4 = 131077;
static const shen_name data_44BE8[7] = {SHEN_NAME('IMOR'), 0, 0, 0, 0, 0, 0};
static const int32_t data_44C04 = 131077;
static const shen_name data_44C08[3] = {SHEN_NAME('IYNU'), 0, 0};
static const float data_44C14[4] = {0.07f, 0.0f, 0.0f, 0.0f};
static const int32_t data_44C24 = 327685;
static const shen_name data_44C28 = SHEN_NAME('ITVH');
static const uint32_t data_44C2C[2] = {0x6AAA6000U, 0};
static const float data_44C34[4] = {0.25f, 0.0f, 0.0f, 0.0f};
static const int32_t data_44C44 = 261;
static const shen_name data_44C48 = SHEN_NAME('STAN');
static const uint32_t data_44C4C[2] = {0x6AAA6000U, 0};
static const float data_44C54[4] = {0.05f, 0.09f, 0.0f, 0.0f};
static const int32_t data_44C64 = 196613;
static const shen_name data_44C68 = SHEN_NAME('IMC1');
static const uint32_t data_44C6C[2] = {0x55554000U, 0};
static const float data_44C74[2] = {0.15f, 0.0f};
static const int32_t data_44C7C[3] = {1, 14, 196613};
static const shen_name data_44C88 = SHEN_NAME('IMC2');
static const uint32_t data_44C8C[2] = {0x5C714000U, 0};
static const float data_44C94[2] = {0.15f, 0.0f};
static const int32_t data_44C9C[3] = {1, 22, 262149};
static const shen_name data_44CA8 = SHEN_NAME('IMC3');
static const uint32_t data_44CAC = 0x55554000U;
static const float data_44CB0[4] = {-0.33f, 0.35f, 0.0f, 0.0f};
static const int32_t data_44CC0[2] = {21, 262149};
static const shen_name data_44CC8 = SHEN_NAME('IMC4');
static const uint32_t data_44CCC = 0x5C714000U;
static const float data_44CD0[4] = {0.33f, 0.35f, 0.0f, 0.0f};
static const int32_t data_44CE0[2] = {21, 5};
static const shen_name data_44CE8[7] = {SHEN_NAME('HIM2'), 0, 0, 0, 0, 0, 0};
static const int32_t data_44D04 = 5;
static const uint32_t data_44D08[7] = {0x6B6C636CU, 0x71C74000U, 0, 0, 0, 0, 0};
static const int32_t data_44D24 = 3;
static const shen_name data_44D28[7] = {SHEN_NAME('HIM4'), 0, 0, 0, 0, 0, 0};
static const int32_t data_44D44 = 196611;
static const shen_name data_44D48 = SHEN_NAME('ITS1');
static const uint32_t data_44D4C[2] = {0x71C7C000U, 0};
static const float data_44D54[2] = {0.15f, 0.12f};
static const int32_t data_44D5C[3] = {3, 30, 196611};
static const shen_name data_44D68 = SHEN_NAME('ITS2');
static const uint32_t data_44D6C[2] = {0x71C7C000U, 0};
static const float data_44D74[2] = {0.15f, 0.12f};
static const int32_t data_44D7C[3] = {3, 31, 196611};
static const shen_name data_44D88 = SHEN_NAME('ITS3');
static const uint32_t data_44D8C[2] = {0x71C7C000U, 0};
static const float data_44D94[2] = {0.15f, 0.12f};
static const int32_t data_44D9C[3] = {3, 32, 196611};
static const shen_name data_44DA8 = SHEN_NAME('ITS4');
static const uint32_t data_44DAC[2] = {0x71C7C000U, 0};
static const float data_44DB4[2] = {0.15f, 0.12f};
static const int32_t data_44DBC[3] = {3, 33, 196611};
static const shen_name data_44DC8 = SHEN_NAME('ITS5');
static const uint32_t data_44DCC[2] = {0x71C7C000U, 0};
static const float data_44DD4[2] = {0.15f, 0.12f};
static const int32_t data_44DDC[3] = {3, 34, 196611};
static const shen_name data_44DE8 = SHEN_NAME('ITS6');
static const uint32_t data_44DEC[2] = {0x71C7C000U, 0};
static const float data_44DF4[2] = {0.15f, 0.12f};
static const int32_t data_44DFC[3] = {3, 35, 327683};
static const shen_name data_44E08 = SHEN_NAME('ITS7');
static const uint32_t data_44E0C[2] = {0x71C7C000U, 0};
static const float data_44E14[3] = {0.555f, 0.0f, 0.0f};
static const int32_t data_44E20[2] = {36, 196611};
static const shen_name data_44E28 = SHEN_NAME('IKD1');
static const uint32_t data_44E2C[5] = {0x71C7C000U, 0, 0, 0, 0};
static const int32_t data_44E40[2] = {37, 196611};
static const shen_name data_44E48 = SHEN_NAME('IKD2');
static const uint32_t data_44E4C[5] = {0x71C7C000U, 0, 0, 0, 0};
static const int32_t data_44E60[2] = {38, 393219};
static const uint32_t data_44E68[3] = {0x75676B69U, 0x71C7C000U, 0};
static const float data_44E74[3] = {0.1f, 0.0f, 0.0f};
static const int32_t data_44E80[2] = {47, 524290};
static const shen_name data_44E88 = SHEN_NAME('FGAK');
static const uint32_t data_44E8C[6] = {0x71C74000U, 0, 0, 0, 0, 0};
static const int32_t data_44EA4 = 2;
static const shen_name data_44EA8[7] = {SHEN_NAME('HIM3'), 0, 0, 0, 0, 0, 0};
static const int32_t data_44EC4 = 196610;
static const shen_name data_44EC8 = SHEN_NAME('FKWH');
static const uint32_t data_44ECC[2] = {0x71C70000U, 0};
static const float data_44ED4[3] = {0.1f, 0.0f, 0.0f};
static const int32_t data_44EE0[2] = {39, 131074};
static const uint32_t data_44EE8[3] = {0x74756266U, 0x71C70000U, 0};
static const float data_44EF4[4] = {0.07f, 0.0f, 0.0f, 0.0f};
static const int32_t data_44F04 = 131074;
static const uint32_t data_44F08[5] = {0x316B6D69U, 0x71C70000U, 0, 0, 0};
static const int32_t data_44F1C[3] = {19, 6, 524295};
static const shen_name data_44F28 = SHEN_NAME('KGK1');
static const uint32_t data_44F2C[6] = {0x71C74000U, 0, 0, 0, 0, 0};
static const int32_t data_44F44 = 524295;
static const shen_name data_44F48 = SHEN_NAME('KGK2');
static const uint32_t data_44F4C[6] = {0x71C7C000U, 0, 0, 0, 0, 0};
static const int32_t data_44F64 = 7;
static const uint32_t data_44F68[7] = {0x6B6C6367U, 0x71C74000U, 0, 0, 0, 0, 0};
static const int32_t data_44F84 = 262151;
static const uint32_t data_44F88[2] = {0x4C62746BU, 0x78E34000U};
static const float data_44F90[5] = {-0.5f, 0.2f, 0.0f, 0.0f, 0.0f};
static const int32_t data_44FA4 = 262151;
static const uint32_t data_44FA8[2] = {0x5262746BU, 0x78E34000U};
static const float data_44FB0[5] = {0.5f, 0.2f, 0.0f, 0.0f, 0.0f};
static const int32_t data_44FC4 = 458752;
static const shen_name data_44FC8 = SHEN_NAME('AKDG');
static const uint32_t data_44FCC[2] = {0x5C71C000U, 0};
static const float data_44FD4[4] = {-0.3f, 0.0f, 0.0f, 0.0f};
static const int32_t data_44FE4 = 196608;
static const shen_name data_44FE8 = SHEN_NAME('AKD2');
static const uint32_t data_44FEC[2] = {0x71C74000U, 0};
static const float data_44FF4[2] = {0.1f, 0.0f};
static const int32_t data_44FFC[3] = {1, 1, 196608};
static const shen_name data_45008 = SHEN_NAME('AKD3');
static const uint32_t data_4500C[2] = {0x71C74000U, 0};
static const float data_45014[2] = {0.1f, 0.0f};
static const int32_t data_4501C[3] = {1, 2, 196608};
static const shen_name data_45028 = SHEN_NAME('AKD4');
static const uint32_t data_4502C[2] = {0x71C74000U, 0};
static const float data_45034[2] = {0.25f, 0.0f};
static const int32_t data_4503C[3] = {1, 5, 196608};
static const shen_name data_45048 = SHEN_NAME('ATS1');
static const uint32_t data_4504C[2] = {0x71C70000U, 0};
static const float data_45054[2] = {0.12f, 0.0f};
static const int32_t data_4505C[3] = {3, 40, 196608};
static const shen_name data_45068 = SHEN_NAME('ATS2');
static const uint32_t data_4506C[2] = {0x71C70000U, 0};
static const float data_45074[2] = {0.12f, 0.0f};
static const int32_t data_4507C[3] = {3, 41, 196608};
static const shen_name data_45088 = SHEN_NAME('ATS3');
static const uint32_t data_4508C[2] = {0x71C70000U, 0};
static const float data_45094[2] = {0.12f, 0.0f};
static const int32_t data_4509C[3] = {3, 42, 196608};
static const shen_name data_450A8 = SHEN_NAME('ATS4');
static const uint32_t data_450AC[2] = {0x71C70000U, 0};
static const float data_450B4[2] = {0.12f, 0.0f};
static const int32_t data_450BC[3] = {3, 43, 196608};
static const shen_name data_450C8 = SHEN_NAME('ATS5');
static const uint32_t data_450CC[2] = {0x71C70000U, 0};
static const float data_450D4[2] = {0.12f, 0.0f};
static const int32_t data_450DC[3] = {3, 44, 196608};
static const shen_name data_450E8 = SHEN_NAME('ATS6');
static const uint32_t data_450EC[2] = {0x71C70000U, 0};
static const float data_450F4[2] = {0.12f, 0.0f};
static const int32_t data_450FC[3] = {3, 45, 327680};
static const shen_name data_45108 = SHEN_NAME('ATS7');
static const uint32_t data_4510C[2] = {0x71C70000U, 0};
static const float data_45114[3] = {0.555f, 0.0f, 0.0f};
static const int32_t data_45120[2] = {46, 0};
static const uint32_t data_45128[3] = {0x64617272U, 0x40004000U, 0};
static const float data_45134[5] = {0.1f, 0.0f, 0.0f, 0.0f, 0.0f};
static const shen_name data_45148 = SHEN_NAME('TOKE');
static const uint32_t data_4514C[3] = {0x71C78000U, 0, 0x3DF5C290U};
static const float data_45158[3] = {0.1f, 0.0f, 0.0f};
static const int32_t data_45164 = 131072;
static const uint32_t data_45168[7] = {0x6D707470U, 0x71C74000U, 0, 0, 0, 0x800001AU, 0};
static const int32_t data_45184 = 262152;
static const shen_name data_45188 = SHEN_NAME('TAN1');
static const uint32_t data_4518C = 0x71C7C000U;
static const float data_45190 = 0.2f;
static const uint32_t data_45194[3] = {0x3EA3D70BU, 0, 0};
static const int32_t data_451A0[2] = {27, 262152};
static const shen_name data_451A8 = SHEN_NAME('TAN2');
static const uint32_t data_451AC = 0x71C7C000U;
static const float data_451B0 = -0.2f;
static const uint32_t data_451B4[3] = {0x3EA3D70BU, 0, 0};
static const int32_t data_451C0[2] = {27, 262157};
static const shen_name data_451C8 = SHEN_NAME('GGB1');
static const uint32_t data_451CC = 0x71C7C000U;
static const float data_451D0[4] = {-0.3f, 0.45f, 0.0f, 0.0f};
static const int32_t data_451E0[2] = {28, 262157};
static const shen_name data_451E8 = SHEN_NAME('GGB2');
static const uint32_t data_451EC = 0x71C7C000U;
static const float data_451F0[4] = {0.3f, 0.45f, 0.0f, 0.0f};
static const int32_t data_45200[2] = {28, 8};
static const shen_name data_45208 = SHEN_NAME('TEL_');
static const uint32_t data_4520C[2] = {0x71C7C000U, 0};
static const float data_45214[4] = {0.1f, 0.0f, 0.0f, 0.0f};
static const int32_t data_45224 = 524296;
static const shen_name data_45228 = SHEN_NAME('TGAK');
static const uint32_t data_4522C[6] = {0x71C7C000U, 0, 0, 0, 0, 0};
static const int32_t data_45244 = 8;
static const uint32_t data_45248[7] = {0x6C616365U, 0x71C7C000U, 0, 0, 0, 0, 0};
static const int32_t data_45264 = -65531;
static const uint32_t data_45268[2] = {0x54575320U, 0x71C78000U};
static const float data_45270[5] = {14.12f, 1.271f, -0.052f, 0.0f, 0.0f};
static const int32_t data_45284 = -65533;
static const uint32_t data_45288[2] = {0x54575322U, 0x71C78000U};
static const float data_45290[5] = {17.8f, 1.271f, -0.052f, 0.0f, 0.0f};
static const int32_t data_452A4 = -65536;
static const uint32_t data_452A8[2] = {0x54575324U, 0x1555EAAAU};
static const float data_452B0[5] = {17.487f, 1.271f, 1.854f, 0.0f, 0.0f};
static const int32_t data_452C4 = -65529;
static const uint32_t data_452C8[2] = {0x54575325U, 0x71C70000U};
static const float data_452D0[5] = {22.28f, 1.271f, 1.84f, 0.0f, 0.0f};
static const int32_t data_452E4 = -65534;
static const uint32_t data_452E8[3] = {0x54575326U, 0x71C74000U, 0x41A7BC6AU};
static const float data_452F4[4] = {1.271f, -0.255f, 0.0f, 0.0f};
static const int32_t data_45304 = -65535;
static const uint32_t data_45308[2] = {0x54575328U, 0x71C70000U};
static const float data_45310[5] = {0.774f, 1.271f, -1.8f, 0.0f, 0.0f};
static const int32_t data_45324 = -65532;
static const uint32_t data_45328[2] = {0x5457532AU, 0x71C70000U};
static const float data_45330[5] = {5.746f, 1.271f, -1.8f, 0.0f, 0.0f};
static const int32_t data_45344 = -65530;
static const uint32_t data_45348[2] = {0x5457532BU, 0x71C78000U};
static const float data_45350[5] = {12.9f, 1.271f, -0.055f, 0.0f, 0.0f};
static const int32_t data_45364 = -65528;
static const uint32_t data_45368[2] = {0x5457532CU, 0x71C7C000U};
static const float data_45370[5] = {16.198f, 1.271f, 2.063f, 0.0f, 0.0f};
static const int32_t data_45384 = -65528;
static const uint32_t data_45388[2] = {0x5457532DU, 0x71C78000U};
static const shen_name data_45390 = SHEN_NAME('33SA');
static const float data_45394[4] = {1.271f, 1.777f, 0.0f, 0.0f};
static const int32_t data_453A4 = -65527;
static const uint32_t data_453A8[2] = {0x5457532EU, 0x71C78000U};
static const float data_453B0[5] = {6.064f, 1.271f, -1.841f, 0.0f, 0.0f};
static const int32_t data_453C4 = -65535;
static const uint32_t data_453C8[2] = {0x5457532FU, 0x71C74000U};
static const float data_453D0[5] = {-2.183f, 0.55f, -0.442f, 0.0f, 0.0f};
static const int32_t data_453E4 = -65536;
static const shen_name data_453E8 = SHEN_NAME('0SWT');
static const uint32_t data_453EC = 0x71C74000U;
static const float data_453F0[5] = {16.38f, 0.85f, 3.814f, 0.0f, 0.0f};
static const int32_t data_45404 = -65534;
static const shen_name data_45408 = SHEN_NAME('1SWT');
static const uint32_t data_4540C[2] = {0x71C70000U, 0x41B40C4AU};
static const float data_45414[4] = {0.55f, -4.324f, 0.0f, 0.0f};
static const int32_t data_45424 = -65527;
static const shen_name data_45428 = SHEN_NAME('2SWT');
static const uint32_t data_4542C = 0x71C70000U;
static const float data_45430[5] = {5.11f, 1.271f, -3.65f, 0.0f, 0.0f};
static const int32_t data_45444 = -65528;
static const uint32_t data_45448[3] = {0x69726966U, 0x71C70000U, 0x41A74396U};
static const float data_45454[4] = {1.7465f, 0.088f, 0.0f, 0.0f};
static const int32_t data_45464 = -65527;
static const uint32_t data_45468[2] = {0x69726974U, 0x71C70000U};
static const float data_45470[5] = {5.833f, 1.7062f, -3.624f, 0.0f, 0.0f};
static const int32_t data_45484 = 524297;
static const uint32_t data_45488[7] = {0x3163705FU, 0x71C7C000U, 0, 0, 0, 0, 0};
static const int32_t data_454A4 = 524297;
static const uint32_t data_454A8[7] = {0x3363705FU, 0x71C74000U, 0, 0, 0, 0, 0};
static const int32_t data_454C4 = 393226;
static const uint32_t data_454C8[3] = {0x75676B62U, 0x71C7C000U, 0};
static const float data_454D4[3] = {0.1f, 0.0f, 0.0f};
static const int32_t data_454E0[2] = {12, 65535};
static char data_454E8[] = "BMB2brou6";
static const uint8_t data_454F2[16] = {
  0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x49, 0x4D, 0x43, 0x31, 0x49, 0x44
};
static const uint8_t data_45502[16] = {
  0x45, 0x4E, 0x34, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x54, 0x41
};
static const uint8_t data_45512[16] = {
  0x4E, 0x31, 0x54, 0x4C, 0x42, 0x4B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
static const uint8_t data_45522[16] = {
  0x00, 0x00, 0x44, 0x44, 0x52, 0x32, 0x6B, 0x6D, 0x69, 0x6C, 0x3B, 0x00, 0x00, 0x00, 0x01, 0x00
};
static const uint8_t pad_45532[6] = {0};
static const uint32_t data_45538[2] = {0x75676B62U, 0x336B6D69U};
static const int32_t data_45540[3] = {17, 1, 5};
static const shen_name data_4554C = SHEN_NAME('AKD2');
static const uint32_t data_45550 = 0x6D6B7770U;
static const int32_t data_45554[3] = {1, 0, 5};
static const char str_45560[] __attribute__((scn_string_literal)) = "TAN2ekai3";
static const uint8_t data_4556A[16] = {
  0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x41, 0x4B, 0x44, 0x34, 0x70, 0x74
};
static const uint8_t data_4557A[16] = {
  0x70, 0x66, 0x1B, 0x00, 0x00, 0x08, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF
};
static const uint8_t data_4558A[2] = {
  0xFF, 0xFF
};
static uint32_t data_4558C = 722345;
static const int32_t data_45590[3] = {0, 656809, 6555049};
static const shen_name data_4559C[27] = {
  SHEN_NAME('IWWH'), SHEN_NAME('BMB1'), SHEN_NAME('BMB2'), SHEN_NAME('BMB4'), SHEN_NAME('BMB5'), SHEN_NAME('D1C1'), SHEN_NAME('D1C2'),
  SHEN_NAME('IMC1'), SHEN_NAME('IMC2'), SHEN_NAME('AKD2'), SHEN_NAME('AKD3'), SHEN_NAME('AKD4'), SHEN_NAME('ATS1'), SHEN_NAME('ATS2'),
  SHEN_NAME('ATS3'), SHEN_NAME('ATS4'), SHEN_NAME('ATS5'), SHEN_NAME('ATS6'), SHEN_NAME('ITS1'), SHEN_NAME('ITS2'), SHEN_NAME('ITS3'),
  SHEN_NAME('ITS4'), SHEN_NAME('ITS5'), SHEN_NAME('ITS6'), SHEN_NAME('FKWH'), SHEN_NAME('IKD1'), SHEN_NAME('IKD2')
};
static const int32_t data_45608[5] = {-1, 722345, 0, 656809, 6555049};
static const shen_name data_4561C[10] = {
  SHEN_NAME('D1C3'), SHEN_NAME('D1C4'), SHEN_NAME('TAN1'), SHEN_NAME('TAN2'), SHEN_NAME('GGB1'), SHEN_NAME('GGB2'), SHEN_NAME('IMC3'),
  SHEN_NAME('IMC4'), SHEN_NAME('DND1'), SHEN_NAME('DND2')
};
static const int32_t data_45644[5] = {-1, 66985, 0, 0, 1449};
static const shen_name data_45658[2] = {SHEN_NAME('DDR1'), SHEN_NAME('DDR2')};
static const int32_t data_45660[5] = {-1, 329129, 0, 263593, 5572009};
static const shen_name data_45674[6] = {SHEN_NAME('DDG1'), SHEN_NAME('DDG2'), SHEN_NAME('ATS7'), SHEN_NAME('ITVH'), SHEN_NAME('ITS7'), SHEN_NAME('BMB3')};
static const int32_t data_4568C[5] = {-1, 131755, 0, 197291, 0};
static const uint32_t data_456A0[6] = {0x4C627469U, 0x52627469U, 0x4C627462U, 0x52627462U, 0x4C62746BU, 0x5262746BU};
static const int32_t data_456B8[5] = {-1, 853417, 0, 0, 787881};
static const shen_name data_456CC = SHEN_NAME('IWKU');
static const uint32_t data_456D0[2] = {0x75676B62U, 0x75676B69U};
static const int32_t data_456D8[5] = {-1, 2557353, 0, 0, 2557353};
static const shen_name data_456EC[3] = {SHEN_NAME('IGAK'), SHEN_NAME('TRNE'), SHEN_NAME('FGAK')};
static const uint32_t data_456F8[3] = {0x3163705FU, 0x3363705FU, 0x6369706BU};
static const shen_name data_45704[3] = {SHEN_NAME('KGK1'), SHEN_NAME('KGK2'), SHEN_NAME('TGAK')};
static const int32_t data_45710[5] = {-1, 3278249, 0, 0, 3278249};
static const shen_name data_45724 = SHEN_NAME('DDZ1');
static const int32_t data_45728[5] = {-1, 3212713, 0, 0, 3147177};
static const shen_name data_4573C[2] = {SHEN_NAME('BTBO'), SHEN_NAME('ITBO')};
static const int32_t data_45744[5] = {-1, 3081641, 0, 0, 3016105};
static const shen_name data_45758[3] = {SHEN_NAME('IWKJ'), SHEN_NAME('BMKJ'), SHEN_NAME('KIKJ')};
static const int32_t data_45764[5] = {-1, 1115561, 0, 0, 1050025};
static const shen_name data_45778 = SHEN_NAME('DDYK');
static const int32_t data_4577C[5] = {-1, 5768617, 0, 0, 5768617};
static const uint32_t data_45790 = 0x74756266U;
static const int32_t data_45794[5] = {-1, 4064681, 0, 0, 4130217};
static const shen_name data_457A8 = SHEN_NAME('DDFP');
static const int32_t data_457AC[5] = {-1, 3605929, 0, 0, 3540393};
static const shen_name data_457C0 = SHEN_NAME('IYNU');
static const int32_t data_457C4[5] = {-1, 132521, 0, 0, 132521};
static const shen_name data_457D8 = SHEN_NAME('AKDG');
static const int32_t data_457DC[2] = {-1, -1};
static const uint8_t data_457E4[28] = {0};
typedef struct Row_45800 { int32_t w0; float w1; float w2; float w3; int32_t w4; int32_t w5; int32_t w6; } Row_45800;
static const Row_45800 data_45800[95] = {
  {-1, 0.0f, 0.0f, 0.0f, 0, 0, 0},
  {1, 0.0f, 0.0f, 0.0f, 0, 0, 0},
  {-1, 0.0f, 0.0f, 0.0f, 0, 0, 0},
  {2, 0.0f, 0.0f, 0.0f, 0, 0, 0},
  {-1, 0.0f, 0.0f, 0.0f, 0, 0, 0},
  {3, 0.0f, 0.0f, 0.0f, 0, 0, 0},
  {-1, 0.0f, 0.0f, 0.0f, 0, 0, 0},
  {4, 0.0f, 0.0f, 0.0f, 0, 0, 0},
  {-1, 0.0f, 0.0f, 0.0f, 0, 0, 0},
  {5, 0.0f, 0.0f, 0.0f, 0, 0, 0},
  {-1, 0.0f, 0.0f, 0.0f, 0, 0, 0},
  {6, 0.0f, 0.0f, 0.0f, 0, 0, 0},
  {-1, 0.0f, 0.0f, 0.0f, 0, 0, 0},
  {7, -0.2f, -0.65f, 0.0f, 0, 0, 0},
  {-1, 0.0f, 0.0f, 0.0f, 0, 0, 0},
  {8, 0.0f, 0.0f, 0.0f, 0, 0, 0},
  {-1, 0.0f, 0.0f, 0.0f, 0, 0, 0},
  {9, 0.0f, 0.0f, 0.0f, 0, 0, 0},
  {-1, 0.0f, 0.0f, 0.0f, 0, 0, 0},
  {10, 0.0f, 0.0f, 0.0f, 0, 0, 0},
  {-1, 0.0f, 0.0f, 0.0f, 0, 0, 0},
  {11, 0.0f, -0.25f, 0.0f, 0, 0, 0},
  {-1, 0.0f, -0.08f, 0.0f, 0, 0, 0},
  {12, 0.0f, 0.0f, 0.0f, 0, 0, 0},
  {-1, 0.0f, 0.0f, 0.0f, 0, 0, 0},
  {13, 0.0f, 0.0f, 0.0f, 0, 0, 0},
  {-1, 0.0f, 0.0f, 0.0f, 0, 0, 0},
  {14, 0.0f, 0.0f, 0.0f, 0, 0, 0},
  {-1, 0.0f, 0.0f, 0.0f, 0, 0, 0},
  {15, 0.0f, 0.0f, 0.0f, 0, 0, 0},
  {-1, 0.0f, 0.0f, 0.0f, 0, 0, 0},
  {16, 0.0f, -0.05f, -0.06f, 0, 0, 0},
  {-1, 0.0f, 0.0f, 0.0f, 0, 0, 0},
  {17, -0.6f, 0.0f, 0.0f, 0, 0, 0},
  {-1, 0.0f, 0.0f, 0.0f, 0, 0, 0},
  {18, -1.8f, 0.0f, 0.0f, 0, 0, 0},
  {-1, 0.0f, 0.0f, 0.0f, 0, 0, 0},
  {19, 0.0f, 0.0f, 0.0f, 0, 0, 0},
  {-1, 0.0f, 0.0f, 0.0f, 0, 0, 0},
  {20, 0.0f, -0.05f, 0.05f, 0, 0, 0},
  {-1, 0.0f, 0.0f, 0.0f, 0, 0, 0},
  {21, 0.0f, 0.0f, 0.0f, 0, 0, 0},
  {-1, 0.0f, 0.0f, 0.0f, 0, 0, 0},
  {22, -0.4036f, -1.5464f, 0.0f, 0, 0, 0},
  {-1, 0.0f, 0.0f, 0.0f, 0, 0, 0},
  {23, -0.4036f, -0.6164f, 0.0f, 0, 0, 0},
  {-1, 0.0f, 0.0f, 0.0f, 0, 0, 0},
  {24, 0.0f, -0.6164f, 0.0f, 0, 0, 0},
  {-1, 0.0f, 0.0f, 0.0f, 0, 0, 0},
  {25, 0.45f, -0.03f, -0.05f, 0, 0, 0},
  {-1, 0.0f, 0.0f, 0.0f, 0, 0, 0},
  {26, 0.0f, -0.05f, 0.0f, 0, 0, 0},
  {-1, 0.0f, 0.0f, 0.0f, 0, 0, 0},
  {27, 0.0f, -0.2f, -0.2f, 0, 0, 0},
  {-1, 0.0f, 0.0f, 0.0f, 0, 0, 0},
  {28, -0.1f, -0.05f, 0.0f, 0, 0, 0},
  {-1, 0.0f, 0.0f, 0.0f, 0, 0, 0},
  {29, 0.0f, 0.0f, 0.0f, 0, 0, 0},
  {-1, 0.0f, 0.0f, 0.0f, 0, 0, 0},
  {30, 0.0f, 0.0f, 0.0f, 0, 0, 0},
  {-1, 0.0f, 0.0f, 0.0f, 0, 0, 0},
  {31, 0.0f, 0.0f, 0.0f, 0, 0, 0},
  {-1, 0.0f, 0.0f, 0.0f, 0, 0, 0},
  {32, 0.0f, 0.0f, 0.0f, 0, 0, 0},
  {-1, 0.0f, 0.0f, 0.0f, 0, 0, 0},
  {33, 0.0f, 0.0f, 0.0f, 0, 0, 0},
  {-1, 0.0f, 0.0f, 0.0f, 0, 0, 0},
  {34, 0.0f, 0.0f, 0.0f, 0, 0, 0},
  {-1, 0.0f, 0.0f, 0.0f, 0, 0, 0},
  {35, -0.3f, -0.635f, 0.022f, 0, 0, 0},
  {-1, 0.0f, 0.0f, 0.0f, 0, 0, 0},
  {36, 0.0f, 0.0f, 0.0f, 0, 0, 0},
  {-1, 0.0f, 0.0f, 0.0f, 0, 0, 0},
  {37, 0.0f, 0.0f, 0.0f, 0, 0, 0},
  {-1, 0.0f, 0.0f, 0.0f, 0, 0, 0},
  {38, 0.0f, 0.0f, 0.0f, 0, 0, 0},
  {-1, 0.0f, 0.0f, 0.0f, 0, 0, 0},
  {39, 0.0f, 0.0f, 0.0f, 0, 0, 0},
  {-1, 0.0f, 0.0f, 0.0f, 0, 0, 0},
  {40, 0.0f, 0.0f, 0.0f, 0, 0, 0},
  {-1, 0.0f, 0.0f, 0.0f, 0, 0, 0},
  {41, 0.0f, 0.0f, 0.0f, 0, 0, 0},
  {-1, 0.0f, 0.0f, 0.0f, 0, 0, 0},
  {42, 0.0f, 0.0f, 0.0f, 0, 0, 0},
  {-1, 0.0f, 0.0f, 0.0f, 0, 0, 0},
  {43, 0.0f, 0.0f, 0.0f, 0, 0, 0},
  {-1, 0.0f, 0.0f, 0.0f, 0, 0, 0},
  {44, 0.0f, 0.0f, 0.0f, 0, 0, 0},
  {-1, 0.0f, 0.0f, 0.0f, 0, 0, 0},
  {45, -0.3f, -0.635f, 0.022f, 0, 0, 0},
  {-1, 0.0f, 0.0f, 0.0f, 0, 0, 0},
  {46, 0.0f, -0.2f, 0.0f, 0, 0, 0},
  {-1, 0.0f, 0.0f, 0.0f, 0, 0, 0},
  {47, 0.0f, 0.0f, 0.0f, 0, 0, 0},
  {-1, 0.0f, 0.0f, 0.0f, 0, 0, 0}
};
static char data_46264[] = "$DHKK3R1G.MT5";
static const char str_46272[] __attribute__((scn_string_literal)) = "$DHKK3R2G.MT5";
static const char str_46280[] __attribute__((scn_string_literal)) = "$THKK3R4G.MT5";
static const char str_4628E[] __attribute__((scn_string_literal)) = "$THKK3R5G.MT5";
static const char str_4629C[] __attribute__((scn_string_literal)) = "$DHKW1R3G.MT5";
static const char str_462AA[] __attribute__((scn_string_literal)) = "$THKW1R3G.MT5";
static const char str_462B8[] __attribute__((scn_string_literal)) = "$THKW1R6G.MT5";
static const char str_462C6[] __attribute__((scn_string_literal)) = "$TKNS7R1G.MT5";
static const char str_462D4[] __attribute__((scn_string_literal)) = "$BHKI2BLG.MT5";
static const char str_462E2[] __attribute__((scn_string_literal)) = "$BHKK3BLG.MT5";
static const char str_462F0[] __attribute__((scn_string_literal)) = "$BHKK3BRG.MT5";
static const char str_462FE[] __attribute__((scn_string_literal)) = "$KFTH4I1G.MT5";
static const char str_4630C[] __attribute__((scn_string_literal)) = "$SHKI2D1G.MT5";
static const char str_4631A[] __attribute__((scn_string_literal)) = "$SHKK3I1G.MT5";
static const char str_46328[] __attribute__((scn_string_literal)) = "$WHKH4I1G.MT5";
static const char str_46336[] __attribute__((scn_string_literal)) = "$SHKH4DRG.MT5";
static const char str_46344[] __attribute__((scn_string_literal)) = "$SHKI2D2G.MT5";
static const char str_46352[] __attribute__((scn_string_literal)) = "$SKTM4DRG.MT5";
static const char str_46360[] __attribute__((scn_string_literal)) = "$KKTM4DRG.MT5";
static const char str_4636E[] __attribute__((scn_string_literal)) = "$BHKI2B2G.MT5";
static const char str_4637C[] __attribute__((scn_string_literal)) = "$THDH4I1G.MT5";
static const char str_4638A[] __attribute__((scn_string_literal)) = "$THKH4IRG.MT5";
static const char str_46398[] __attribute__((scn_string_literal)) = "$RKAO1DUG.MT5";
static const char str_463A6[] __attribute__((scn_string_literal)) = "$RKAO1DWG.MT5";
static const char str_463B4[] __attribute__((scn_string_literal)) = "$THKK3R1G.MT5";
static const char str_463C2[] __attribute__((scn_string_literal)) = "$BKTK3B1G.MT5";
static const char str_463D0[] __attribute__((scn_string_literal)) = "$THDS7G1G.MT5";
static const char str_463DE[] __attribute__((scn_string_literal)) = "$GHDS7G1G.MT5";
static const char str_463EC[] __attribute__((scn_string_literal)) = "$TKTK3D1G.MT5";
static const char str_463FA[] __attribute__((scn_string_literal)) = "$YHKM4I1G.MT5";
static const char str_46408[] __attribute__((scn_string_literal)) = "$YHKM4I2G.MT5";
static const char str_46416[] __attribute__((scn_string_literal)) = "$YHKK3I3G.MT5";
static const char str_46424[] __attribute__((scn_string_literal)) = "$YHKK3I4G.MT5";
static const char str_46432[] __attribute__((scn_string_literal)) = "$YHKK3I5G.MT5";
static const char str_46440[] __attribute__((scn_string_literal)) = "$YHKK3I6G.MT5";
static const char str_4644E[] __attribute__((scn_string_literal)) = "$YKTM4I1G.MT5";
static const char str_4645C[] __attribute__((scn_string_literal)) = "$KHKK3IRG.MT5";
static const char str_4646A[] __attribute__((scn_string_literal)) = "$KHKK3ILG.MT5";
static const char str_46478[] __attribute__((scn_string_literal)) = "$WHKM4F1G.MT5";
static const char str_46486[] __attribute__((scn_string_literal)) = "$THKK3R1G.MT5";
static const char str_46494[] __attribute__((scn_string_literal)) = "$THKT2R2G.MT5";
static const char str_464A2[] __attribute__((scn_string_literal)) = "$THKW1R3G.MT5";
static const char str_464B0[] __attribute__((scn_string_literal)) = "$THKK3R4G.MT5";
static const char str_464BE[] __attribute__((scn_string_literal)) = "$THKK3R5G.MT5";
static const char str_464CC[] __attribute__((scn_string_literal)) = "$THKW1R6G.MT5";
static const char str_464DA[] __attribute__((scn_string_literal)) = "$TKNH4R1G.MT5";
static const char str_464E8[] __attribute__((scn_string_literal)) = "$TOUK3I1G.MT5";
static const uint8_t data_464F6[2] = {0};
static const Vector4 data_464F8 = {.x = 0.78f, .y = 0.82f, .z = 0.59f, .w = 0.46f};
static const float data_46508[19] = {
  0.89f, 0.89f, 0.13f, 0.39f, 0.28f, 0.14f, 1.1f, 1.84f, 0.93f, 0.58f, 4.1813f, 0.996f, 0.7922f, 1000.0f, 1000.0f, 1000.0f, 1000.0f, 1000.0f, 1000.0f
};
static const char data_46554[28] __attribute__((scn_string_literal)) = "Map Start!!!!!!!!!!!!!!!!!\012";
static const char data_46570[21] __attribute__((scn_string_literal)) = "このアイテムもってる";
static const char data_46585[17] __attribute__((scn_string_literal)) = "tansu naka = %d\012";
static const char data_46596[17] __attribute__((scn_string_literal)) = "JOMO_AKIRA = %d\012";
static const char data_465A7[17] __attribute__((scn_string_literal)) = "JOMO_AKIRA = %d\012";
static const char data_465B8[6] __attribute__((scn_string_literal)) = "01LET";
static const char data_465BE[10] __attribute__((scn_string_literal)) = "01LETB006";
static const char data_465C8[10] __attribute__((scn_string_literal)) = "01LETB001";
static const char data_465D2[10] __attribute__((scn_string_literal)) = "01LETB002";
static const char data_465DC[10] __attribute__((scn_string_literal)) = "01LETB003";
static const char data_465E6[10] __attribute__((scn_string_literal)) = "01LETB004";
static const char data_465F0[10] __attribute__((scn_string_literal)) = "01LETB005";
static const char data_465FA[6] __attribute__((scn_string_literal)) = "01LET";
static const char data_46600[17] __attribute__((scn_string_literal)) = "JOMO_AKIRA = %d\012";
static const char data_46611[17] __attribute__((scn_string_literal)) = "JOMO_AKIRA = %d\012";
static const char data_46622[10] __attribute__((scn_string_literal)) = "E1018A113";
static const char data_4662C[10] __attribute__((scn_string_literal)) = "E1018A114";
static const char data_46636[10] __attribute__((scn_string_literal)) = "E1018A115";
static const char data_46640[17] __attribute__((scn_string_literal)) = "JOMO_AKIRA = %d\012";
static const char data_46651[10] __attribute__((scn_string_literal)) = "E1018A126";
static const char data_4665B[10] __attribute__((scn_string_literal)) = "E1018A127";
static const char data_46665[10] __attribute__((scn_string_literal)) = "E1018A129";
static const char data_4666F[10] __attribute__((scn_string_literal)) = "E1018A128";
static const char data_46679[10] __attribute__((scn_string_literal)) = "E1018A131";
static const char data_46683[10] __attribute__((scn_string_literal)) = "E1018A132";
static const char data_4668D[10] __attribute__((scn_string_literal)) = "E1018A126";
static const char data_46697[10] __attribute__((scn_string_literal)) = "E1018A127";
static const char data_466A1[10] __attribute__((scn_string_literal)) = "E1018A128";
static const char data_466AB[10] __attribute__((scn_string_literal)) = "E1018A129";
static const char data_466B5[10] __attribute__((scn_string_literal)) = "E1018A130";
static const char data_466BF[10] __attribute__((scn_string_literal)) = "E1018A131";
static const char data_466C9[10] __attribute__((scn_string_literal)) = "E1018A132";
static const char data_466D3[10] __attribute__((scn_string_literal)) = "E1018A029";
static const char data_466DD[10] __attribute__((scn_string_literal)) = "E1018A108";
static const char data_466E7[10] __attribute__((scn_string_literal)) = "E1018A109";
static const char data_466F1[10] __attribute__((scn_string_literal)) = "E1018A110";
static const char data_466FB[10] __attribute__((scn_string_literal)) = "E1018A111";
static const char data_46705[10] __attribute__((scn_string_literal)) = "E1018A029";
static const char data_4670F[10] __attribute__((scn_string_literal)) = "E1018A107";
static const char data_46719[10] __attribute__((scn_string_literal)) = "E1018A032";
static const char data_46723[10] __attribute__((scn_string_literal)) = "E1018A116";
static const char data_4672D[10] __attribute__((scn_string_literal)) = "E1018A117";
static const char data_46737[10] __attribute__((scn_string_literal)) = "E1018A118";
static const char data_46741[10] __attribute__((scn_string_literal)) = "E1018A119";
static const char data_4674B[10] __attribute__((scn_string_literal)) = "E1018A120";
static const char data_46755[10] __attribute__((scn_string_literal)) = "E1018A133";
static const char data_4675F[10] __attribute__((scn_string_literal)) = "E1018A134";
static const char data_46769[10] __attribute__((scn_string_literal)) = "E1018A135";
static const char data_46773[10] __attribute__((scn_string_literal)) = "E1018A136";
static const char data_4677D[10] __attribute__((scn_string_literal)) = "E1018A137";
static const char data_46787[10] __attribute__((scn_string_literal)) = "E1018A138";
static const char data_46791[10] __attribute__((scn_string_literal)) = "E1018A142";
static const char data_4679B[10] __attribute__((scn_string_literal)) = "E1018A134";
static const char data_467A5[10] __attribute__((scn_string_literal)) = "E1018A143";
static const char data_467AF[10] __attribute__((scn_string_literal)) = "E1018A136";
static const char data_467B9[14] __attribute__((scn_string_literal)) = "きてるきてる\012";
static const char data_467C7[10] __attribute__((scn_string_literal)) = "E1018A137";
static const char data_467D1[10] __attribute__((scn_string_literal)) = "E1018A138";
static const char data_467DB[15] __attribute__((scn_string_literal)) = "%s = %f %f %f\012";
static const char data_467EA[10] __attribute__((scn_string_literal)) = "E1018A003";
static const char data_467F4[10] __attribute__((scn_string_literal)) = "E1018A012";
static const char data_467FE[10] __attribute__((scn_string_literal)) = "E1018A004";
static const char data_46808[10] __attribute__((scn_string_literal)) = "E1018A005";
static const char data_46812[10] __attribute__((scn_string_literal)) = "E1018A005";
static const char data_4681C[10] __attribute__((scn_string_literal)) = "E1018A006";
static const char data_46826[10] __attribute__((scn_string_literal)) = "JINGURU01";
static const char data_46830[10] __attribute__((scn_string_literal)) = "E1018A009";
static const char data_4683A[10] __attribute__((scn_string_literal)) = "E1018A013";
static const char data_46844[32] = "$SHAS501G.MT5\000$SHAS500G.MT5";
static const Vector3 data_46864 = {.x = 0.0f, .y = 0.0f, .z = 0.0f};
static const Vector3i data_46870 = {.x = 0, .y = 0, .z = 0};
static const Vector3 data_4687C = {.x = 0.0f, .y = -0.1f, .z = 0.0f};
static const Vector3i data_46888 = {.x = 0, .y = 0, .z = 0};
static const Vector3 data_46894 = {.x = 0.0f, .y = 0.14f, .z = -0.13f};
static const Vector3i data_468A0 = {.x = 49152, .y = 0, .z = 0};
static const Vector3 data_468AC = {.x = 0.0f, .y = 0.0f, .z = 0.0f};
static const Vector3i data_468B8 = {.x = 0, .y = 0, .z = 0};
static const char data_468C4[10] __attribute__((scn_string_literal)) = "E1018A151";
static const char data_468CE[10] __attribute__((scn_string_literal)) = "E1018A151";
static const char data_468D8[10] __attribute__((scn_string_literal)) = "E1018A152";
static const char data_468E2[10] __attribute__((scn_string_literal)) = "E1018A153";
static const char data_468EC[11] __attribute__((scn_string_literal)) = "M_MOBJ.BIN";
static const char data_468F7[14] __attribute__((scn_string_literal)) = "scene/01/JOMO";
static const char data_46905[9] __attribute__((scn_string_literal)) = "COMMON01";
static const char data_4690E[13] __attribute__((scn_string_literal)) = "HMOT_OMO.BIN";
static const char data_4691B[14] __attribute__((scn_string_literal)) = "scene/01/JOMO";
static const char data_46929[] __attribute__((scn_string_literal)) = "CYCLEMAN.BIN";
static const char data_46936[6] __attribute__((scn_string_literal)) = "E1018";
static const char data_4693C[14] __attribute__((scn_string_literal)) = "scene/01/JOMO";
static const char data_4694A[6] __attribute__((scn_string_literal)) = "MPK00";
static const char data_46950[17] __attribute__((scn_string_literal)) = "JOMO_AKIRA = %d\012";
static const char data_46961[26] __attribute__((scn_string_literal)) = "母屋のれじゅーむすたーと\012";
static const char data_4697B[36] __attribute__((scn_string_literal)) = "レジューム変だよ。花岡よんでよ！！\012";
static const char data_4699F[17] __attribute__((scn_string_literal)) = "JOMO_AKIRA = %d\012";
static const char data_469B0[17] __attribute__((scn_string_literal)) = "JOMO_AKIRA = %d\012";
static const char data_469C1[17] __attribute__((scn_string_literal)) = "JOMO_AKIRA = %d\012";
static const char data_469D2[17] __attribute__((scn_string_literal)) = "JOMO_AKIRA = %d\012";
static const char data_469E3[17] __attribute__((scn_string_literal)) = "JOMO_AKIRA = %d\012";
static const char data_469F4[17] __attribute__((scn_string_literal)) = "JOMO_AKIRA = %d\012";
static const char data_46A05[17] __attribute__((scn_string_literal)) = "JOMO_AKIRA = %d\012";
static const char data_46A16[17] __attribute__((scn_string_literal)) = "JOMO_AKIRA = %d\012";
static const char data_46A27[17] __attribute__((scn_string_literal)) = "JOMO_AKIRA = %d\012";
static const char data_46A38[17] __attribute__((scn_string_literal)) = "JOMO_AKIRA = %d\012";
static const char data_46A49[19] __attribute__((scn_string_literal)) = "JOMO_AKIRA = %d\012";
static const FogColourData data_46A5C = {.red = 8.0f, .green = 150.0f, .blue = 1.0f, .colour = 0xFF000000U};
static const char data_46A6C[6] __attribute__((scn_string_literal)) = "YKC_M";
static const char data_46A72[6] __attribute__((scn_string_literal)) = "YKB_M";
static const char data_46A78[17] __attribute__((scn_string_literal)) = "JOMO_AKIRA = %d\012";
static const char data_46A89[17] __attribute__((scn_string_literal)) = "JOMO_AKIRA = %d\012";
static const char data_46A9A[16] __attribute__((scn_string_literal)) = "AKIRA_ROOM_ %d\012";
static const char data_46AAA[17] __attribute__((scn_string_literal)) = "JOMO_AKIRA = %d\012";
static const char data_46ABB[16] __attribute__((scn_string_literal)) = "AKIRA_ROOM_ %d\012";
static const char data_46ACB[16] __attribute__((scn_string_literal)) = "AKIRA_ROOM_ %d\012";
static const char data_46ADB[16] __attribute__((scn_string_literal)) = "AKIRA_ROOM_ %d\012";
static const char data_46AEB[16] __attribute__((scn_string_literal)) = "AKIRA_ROOM_ %d\012";
static const char data_46AFB[16] __attribute__((scn_string_literal)) = "AKIRA_ROOM_ %d\012";
static const char data_46B0B[14] __attribute__((scn_string_literal)) = "scene/01/JOMO";
static const char data_46B19[7] __attribute__((scn_string_literal)) = "NINJIN";
static const char data_46B20[11] __attribute__((scn_string_literal)) = "tsm003.snd";
static const char data_46B2B[13] __attribute__((scn_string_literal)) = "a1_ksnin.snd";
static const char data_46B38[7] __attribute__((scn_string_literal)) = "NINJIN";
static const char data_46B3F[11] __attribute__((scn_string_literal)) = "bgm077.snd";
static const char data_46B4A[13] __attribute__((scn_string_literal)) = "a1_houou.snd";
static const char data_46B57[11] __attribute__((scn_string_literal)) = "bgm063.snd";
static const char data_46B62[13] __attribute__((scn_string_literal)) = "a1_jytel.snd";
static const char data_46B6F[11] __attribute__((scn_string_literal)) = "bgm029.snd";
static const char data_46B7A[12] __attribute__((scn_string_literal)) = "a1_telp.snd";
static const char data_46B86[11] __attribute__((scn_string_literal)) = "bgm029.snd";
static const char data_46B91[12] __attribute__((scn_string_literal)) = "a1_telp.snd";
static const char data_46B9D[11] __attribute__((scn_string_literal)) = "bgm050.snd";
static const char data_46BA8[13] __attribute__((scn_string_literal)) = "a1_tmryo.snd";
static const char data_46BB5[13] __attribute__((scn_string_literal)) = "a1_kawak.snd";
static const char data_46BC2[] __attribute__((scn_string_literal)) = "sprite";
static const char data_46BC9[13] __attribute__((scn_string_literal)) = "SCROLL25.SPR";
static const char data_46BD6[13] __attribute__((scn_string_literal)) = "a1_kawak.snd";
static const char data_46BE3[] __attribute__((scn_string_literal)) = "sprite";
static const char data_46BEA[13] __attribute__((scn_string_literal)) = "SCROLL00.SPR";
static const char data_46BF7[] __attribute__((scn_string_literal)) = "sprite";
static const char data_46BFE[13] __attribute__((scn_string_literal)) = "SCROLL25.SPR";
static const char data_46C0B[13] __attribute__((scn_string_literal)) = "a1_kawak.snd";
static const char data_46C18[] __attribute__((scn_string_literal)) = "sprite";
static const char data_46C1F[13] __attribute__((scn_string_literal)) = "SCROLL00.SPR";
static const char data_46C2C[] __attribute__((scn_string_literal)) = "sprite";
static const char data_46C33[13] __attribute__((scn_string_literal)) = "SCROLL25.SPR";
static const char data_46C40[13] __attribute__((scn_string_literal)) = "a1_kawak.snd";
static const char data_46C4D[] __attribute__((scn_string_literal)) = "sprite";
static const char data_46C54[13] __attribute__((scn_string_literal)) = "SCROLL00.SPR";
static const char data_46C61[] __attribute__((scn_string_literal)) = "sprite";
static const char data_46C68[13] __attribute__((scn_string_literal)) = "SCROLL25.SPR";
static const char data_46C75[13] __attribute__((scn_string_literal)) = "a1_kawak.snd";
static const char data_46C82[] __attribute__((scn_string_literal)) = "sprite";
static const char data_46C89[13] __attribute__((scn_string_literal)) = "SCROLL00.SPR";
static const char data_46C96[] __attribute__((scn_string_literal)) = "sprite";
static const char data_46C9D[13] __attribute__((scn_string_literal)) = "SCROLL25.SPR";
static const char data_46CAA[13] __attribute__((scn_string_literal)) = "a1_kawak.snd";
static const char data_46CB7[] __attribute__((scn_string_literal)) = "sprite";
static const char data_46CBE[13] __attribute__((scn_string_literal)) = "SCROLL00.SPR";
static const char data_46CCB[] __attribute__((scn_string_literal)) = "sprite";
static const char data_46CD2[13] __attribute__((scn_string_literal)) = "SCROLL25.SPR";
static const char data_46CDF[13] __attribute__((scn_string_literal)) = "a1_kawak.snd";
static const char data_46CEC[] __attribute__((scn_string_literal)) = "sprite";
static const char data_46CF3[13] __attribute__((scn_string_literal)) = "SCROLL00.SPR";
static const char data_46D00[11] __attribute__((scn_string_literal)) = "bgm046.snd";
static const char data_46D0B[13] __attribute__((scn_string_literal)) = "a1_kawat.snd";
static const char data_46D18[11] __attribute__((scn_string_literal)) = "bgm046.snd";
static const char data_46D23[13] __attribute__((scn_string_literal)) = "a1_kawat.snd";
static const char data_46D30[11] __attribute__((scn_string_literal)) = "bgm046.snd";
static const char data_46D3B[13] __attribute__((scn_string_literal)) = "a1_kawat.snd";
static const char data_46D48[11] __attribute__((scn_string_literal)) = "bgm046.snd";
static const char data_46D53[13] __attribute__((scn_string_literal)) = "a1_kawat.snd";
static const char data_46D60[11] __attribute__((scn_string_literal)) = "bgm046.snd";
static const char data_46D6B[13] __attribute__((scn_string_literal)) = "a1_kawat.snd";
static const char data_46D78[11] __attribute__((scn_string_literal)) = "bgm046.snd";
static const char data_46D83[13] __attribute__((scn_string_literal)) = "a1_kawat.snd";
static const char data_46D90[11] __attribute__((scn_string_literal)) = "bgm063.snd";
static const char data_46D9B[13] __attribute__((scn_string_literal)) = "a1_fukyu.snd";
static const char data_46DA8[6] __attribute__((scn_string_literal)) = "YKB_M";
static const char data_46DAE[11] __attribute__((scn_string_literal)) = "bgm029.snd";
static const char data_46DB9[13] __attribute__((scn_string_literal)) = "a1_tumir.snd";
static const char data_46DC6[13] __attribute__((scn_string_literal)) = "a1_fukay.snd";
static const char data_46DD3[13] __attribute__((scn_string_literal)) = "a1_ineko.snd";
static const char data_46DE0[11] __attribute__((scn_string_literal)) = "bgm101.snd";
static const char data_46DEB[13] __attribute__((scn_string_literal)) = "a1_chofk.snd";
static const char data_46DF8[11] __attribute__((scn_string_literal)) = "bgm077.snd";
static const char data_46E03[12] __attribute__((scn_string_literal)) = "a1_butu.snd";
static const char data_46E0F[6] __attribute__((scn_string_literal)) = "YKD_M";
static const char data_46E15[11] __attribute__((scn_string_literal)) = "bgm101.snd";
static const char data_46E20[13] __attribute__((scn_string_literal)) = "a1_fukan.snd";
static const char data_46E2D[11] __attribute__((scn_string_literal)) = "bgm114.snd";
static const char data_46E38[13] __attribute__((scn_string_literal)) = "a1_kikum.snd";
static const char data_46E45[11] __attribute__((scn_string_literal)) = "bgm115.snd";
static const char data_46E50[13] __attribute__((scn_string_literal)) = "a1_inet2.snd";
static const char data_46E5D[13] __attribute__((scn_string_literal)) = "a1_hukhe.snd";
static const char data_46E6A[14] __attribute__((scn_string_literal)) = "scene/01/JOMO";
static const char data_46E78[7] __attribute__((scn_string_literal)) = "HOUMON";
static const char data_46E7F[11] __attribute__((scn_string_literal)) = "bgm095.snd";
static const char data_46E8A[11] __attribute__((scn_string_literal)) = "a1_ito.snd";
static const char data_46E95[7] __attribute__((scn_string_literal)) = "HOUMON";
static const char data_46E9C[11] __attribute__((scn_string_literal)) = "bgm085.snd";
static const char data_46EA7[13] __attribute__((scn_string_literal)) = "a1_gakri.snd";
static const char data_46EB4[11] __attribute__((scn_string_literal)) = "bgm085.snd";
static const char data_46EBF[13] __attribute__((scn_string_literal)) = "a1_gakri.snd";
static const char data_46ECC[11] __attribute__((scn_string_literal)) = "tym001.snd";
static const char data_46ED7[13] __attribute__((scn_string_literal)) = "a1_hukum.snd";
static const char data_46EE4[11] __attribute__((scn_string_literal)) = "bgm070.snd";
static const char data_46EEF[13] __attribute__((scn_string_literal)) = "a1_okozu.snd";
static const char data_46EFC[11] __attribute__((scn_string_literal)) = "bgm070.snd";
static const char data_46F07[13] __attribute__((scn_string_literal)) = "a1_nhana.snd";
static const char data_46F14[11] __attribute__((scn_string_literal)) = "bgm100.snd";
static const char data_46F1F[13] __attribute__((scn_string_literal)) = "a1_hongo.snd";
static const char data_46F2C[13] __attribute__((scn_string_literal)) = "a1_oresu.snd";
static const char data_46F39[11] __attribute__((scn_string_literal)) = "bgm085.snd";
static const char data_46F44[13] __attribute__((scn_string_literal)) = "a1_uresi.snd";
static const char data_46F51[13] __attribute__((scn_string_literal)) = "a1_jibun.snd";
static const char data_46F5E[] __attribute__((scn_string_literal)) = "sprite";
static const char data_46F65[13] __attribute__((scn_string_literal)) = "SCROLL00.SPR";
static const char data_46F72[11] __attribute__((scn_string_literal)) = "bgm046.snd";
static const char data_46F7D[13] __attribute__((scn_string_literal)) = "a1_kawat.snd";
static const char data_46F8A[] __attribute__((scn_string_literal)) = "sprite";
static const char data_46F91[13] __attribute__((scn_string_literal)) = "SCROLL00.SPR";
static const char data_46F9E[11] __attribute__((scn_string_literal)) = "bgm046.snd";
static const char data_46FA9[13] __attribute__((scn_string_literal)) = "a1_kawat.snd";
static const char data_46FB6[11] __attribute__((scn_string_literal)) = "bgm068.snd";
static const char data_46FC1[13] __attribute__((scn_string_literal)) = "a1_nozok.snd";
static const char data_46FCE[26] __attribute__((scn_string_literal)) = "なんでなんでなんでぇーー\012";
