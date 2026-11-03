#include "act02_mkyu.h"

static int32_t script_0030(int32_t arg0);
static void script_004E(float arg0);
static int32_t script_006D(int32_t arg0, int32_t arg1);
static void script_00E5(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4);
static int32_t script_0697(int32_t arg0);
static void script_0768(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_0778(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_087C(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_0A1E(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_0B7F(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_0CE0(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_0F7E(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_104C(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_10B3(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_111A(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_1161(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_1297(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_1B58(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_1C76(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_233B(int32_t arg0, int32_t arg1);
static void script_2411(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_3324(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_3390(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_384A(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_3F04(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_57E4(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_5F05(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_668F(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_6767(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_6BDE(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_7B71(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static void script_7FBB(int32_t arg0, DSWKDoorEntry *arg1, int32_t *arg2, DoorResourceDescriptor *arg3, MapObjectRecord *arg4, int32_t arg5, int32_t arg6);
static void script_8147(DSWKDoorEntry *arg0, int32_t *arg1, DoorResourceDescriptor *arg2, MapObjectRecord *arg3, int32_t arg4);
static void script_817B(void);
static void script_81E4(int32_t arg0, int32_t arg1);
static void script_8281(int32_t arg0, int32_t arg1);
static void script_82BB(void);
static void script_831E(void);
static void script_8381(int32_t arg0);
static void script_83B8(int32_t arg0);
static void script_83EF(int32_t arg0);
static void script_8631(int32_t arg0);
static void script_867A(int32_t arg0);
static void script_86C3(int32_t arg0, int32_t arg1);
static void script_8739(int32_t arg0);
static void script_8747(int32_t arg0);
static void script_8755(int32_t arg0);
static void script_881C(void);
static void script_8828(void);
static void script_8834(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5, int32_t arg6);
static int32_t script_8877(void);
static int8_t script_8988(int32_t arg0, int32_t arg1, int8_t arg2);
static void script_8A20(int32_t arg0);
static void script_8BEF(int32_t arg0);
static int32_t script_8C20(int32_t arg0, int32_t arg1, int32_t arg2, float arg3, float arg4, int32_t arg5, int32_t arg6);
static void script_92E9(int32_t arg0, int32_t arg1);
static void script_931B(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_948B(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_9647(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_97FB(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_9A11(int32_t arg0, int32_t arg1);
static void script_9ADB(int32_t arg0, int32_t arg1);
static void script_9B3C(int32_t arg0);
static void script_9CE1(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_9D79(int32_t arg0, int32_t arg1);
static void script_9E5B(int32_t arg0);
static void script_9EB2(int32_t arg0, float arg1, float arg2, float arg3);
static void script_9EDA(int32_t arg0, float arg1, float arg2, float arg3);
static void script_9F2E(int32_t arg0, int32_t arg1, int32_t arg2);
static int32_t script_9F7E(int32_t arg0);
static void script_A025(int32_t arg0, int32_t arg1);
static int32_t script_A048(int32_t arg0);
static void script_A07B(int32_t arg0, int32_t arg1);
static void script_A09E(int32_t arg0, int32_t arg1);
static void script_A0FD(int32_t arg0);
static void script_A112(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_A4A8(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_AB36(void);
static void script_AB45(void);
static void script_AB54(void);
static void script_AB57(void);
static void script_AB5A(void);
static void script_AB5D(void);
static void script_AB60(void);
static void script_AB6D(void);
static void script_AB79(void);
static int32_t script_AB88(void);
static void script_AB97(void);
static void script_ACCF(int32_t arg0);
static void script_ACDB(int32_t arg0);
static void script_ACE7(int32_t arg0);
static void script_ADE3(int32_t arg0);
static void script_ADFA(int32_t arg0);
static void script_AE3B(int32_t arg0);
static void script_AE7F(int32_t arg0);
static void script_AEFD(void);
static void script_AF7C(int32_t arg0);
static int32_t script_AFA8(int32_t arg0, int32_t arg1);
static int8_t script_AFDB(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_B11D(int32_t arg0, int32_t arg1);
static void script_B2E6(int32_t arg0, int32_t arg1);
static void script_B4BC(int32_t arg0, int32_t arg1);
static void script_B850(void);
static void script_B917(void);
static void script_B9AC(float arg0, float arg1, float arg2, int32_t arg3);
static void script_B9F6(int32_t arg0);
static int8_t script_BA10(void);
static int8_t script_BA28(void);
static void script_BA57(void);
static void script_BF4B(const char *arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_BFF8(void);
static void script_C178(void);
static void script_C214(void);
static void script_CD49(void);
static void script_CD4C(int32_t arg0);
static void script_CDFE(void);
static void script_CECC(void);
static int32_t script_CFCA(void);
static int32_t script_D0D5(void);
static int32_t script_D1C1(void);
static int32_t script_D21F(void);
static int32_t script_D283(void);
static int32_t script_D46E(void);
static int32_t script_D514(void);
static int32_t script_D5A0(void);
static void script_D698(void);
static void script_D70D(void);
static void script_D71F(void);
static int32_t script_D77C(void);
static int32_t script_D86D(int32_t arg0);
static int32_t script_D8B4(int32_t arg0, const char *arg1);
static void script_D929(const char *arg0);
static int32_t script_D94A(int32_t arg0, const char *arg1, int32_t arg2);
static int32_t script_D98F(int32_t arg0, int32_t arg1);
static void script_D9BD(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_D9D1(int32_t arg0, int32_t arg1);
static int32_t script_D9E3(const char *arg0, int32_t arg1);
static void script_DA14(int32_t arg0, int32_t arg1);
static int32_t script_DA26(int32_t arg0, int32_t arg1, int32_t arg2, const Vector3 *arg3, const Vector3i *arg4, int32_t arg5);
static void script_DA59(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static int32_t script_DA73(int32_t arg0, int32_t arg1);
static void script_DA9B(int32_t arg0, int32_t arg1);
static void script_DAAD(int32_t arg0, int32_t arg1);
static int32_t script_DAEB(int32_t arg0, int32_t arg1, int32_t arg2, float arg3, int32_t arg4, int32_t arg5);
static int32_t script_DB5C(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4);
static void script_DBBE(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4);
static void script_DBD6(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5);
static int32_t script_DBF0(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_DC2E(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_DC44(int32_t arg0, int32_t arg1, const Vector3i *arg2, int32_t arg3);
static void script_DC75(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_DC8B(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4);
static void script_DCBC(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4);
static int32_t script_DCD4(int32_t arg0, int32_t arg1, const Vector3 *arg2, int32_t arg3);
static void script_DD21(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_DD37(int32_t arg0, int32_t arg1, const Vector3i *arg2, int32_t arg3);
static void script_DD86(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_DD9C(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_DDCA(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_DDE0(int32_t arg0);
static void script_DE1F(void);
static void script_DE4B(int32_t arg0, int32_t arg1, int32_t arg2);
static int32_t script_DF71(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_DFE3(int32_t arg0, int32_t arg1, int32_t arg2);
static int32_t script_DFF7(int32_t arg0);
static int32_t script_E01D(int32_t arg0);
static int32_t script_E03B(int32_t arg0, int32_t arg1);
static void script_E069(int32_t arg0, int32_t arg1);
static void script_E07B(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_E1AC(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_E1DE(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_E2AD(void);
static void script_E2E7(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_E444(int32_t arg0);
static void script_E51F(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_E5C0(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_E6C9(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4);
static void script_EA0D(int32_t arg0, int32_t arg1);
static void script_EADB(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_EDCC(int32_t arg0, int32_t arg1, int32_t arg2);
static int32_t script_F0B6(int32_t arg0);
static void script_F51A(int32_t arg0);
static void script_F535(void);
static void script_F546(void);
static void script_F54D(void);
static void script_F55A(int32_t arg0);
static void script_F6E1(int32_t arg0);
static int32_t script_F707(void);
static int32_t script_F86D(void);
static void script_F934(int32_t arg0);
static void script_F952(int32_t arg0);
static void script_FB6C(int32_t arg0);
static void script_FCBA(int32_t arg0);
static void script_FE54(void);
static int32_t script_FE7D(int32_t arg0);
static void script_FFC6(void);
static void script_10001(int32_t arg0);
static int32_t script_10632(void);
static void script_10661(int32_t arg0);
static void script_106B3(int32_t arg0);
static void script_10B16(int32_t arg0);
static int32_t script_10C1E(void);
static void script_10C3B(void);
static void script_10C7A(void);
static void script_10D4D(void);
static void script_10E05(const Vector3 *arg0, const Vector3i *arg1);
static void script_10E41(void);
static void script_10F57(int32_t arg0);
static void script_11016(int32_t arg0);
static void script_110F4(int32_t arg0, int32_t arg1);
static void script_11244(int32_t arg0);
static void script_1131D(int32_t arg0);
static int32_t script_1143B(int32_t arg0, int32_t arg1);
static int32_t script_1161F(int32_t arg0);
static void script_1190C(int32_t arg0);
static void script_11AAC(int32_t arg0, int32_t arg1);
static void script_11C67(int8_t arg0, int8_t arg1, int32_t arg2);
static void script_11D86(int32_t arg0);
static int32_t script_11EB6(int32_t arg0);
static int32_t script_12000(int32_t arg0);
static void script_12017(int32_t arg0);
static void script_1203B(int32_t arg0);
static void script_120C1(int32_t arg0);
static void script_121B2(int32_t arg0, int32_t arg1, const Vector3 *arg2, int32_t arg3, int32_t arg4, const Vector3 *arg5, const Vector3 *arg6);
static void script_1225B(int32_t arg0, int32_t arg1, int32_t arg2);
static void script_122AD(int32_t arg0, ...);
static void script_123AD(const char *arg0);
static void script_123BC(int32_t arg0, const char *arg1, const char *arg2, ...);
static void script_12426(int32_t arg0, int32_t arg1);
static int32_t script_12625(int32_t arg0, int32_t arg1);
static void script_12651(int32_t arg0);
static void script_1271B(int32_t arg0);
static void script_12735(int32_t arg0, const Vector3 *arg1);
static int32_t script_12759(float arg0, float arg1);
static int32_t script_12792(float arg0, float arg1);
static int32_t script_128AC(float arg0, float arg1);
static int32_t script_1295D(void);
static int32_t script_1297D(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static void script_12E53(int32_t arg0, int32_t arg1, const Vector3 *arg2);
static void script_12E73(int32_t arg0, int32_t arg1, Vector3 *arg2);
static void script_12E92(int32_t arg0, int32_t arg1, const Vector3i *arg2);
static void script_12EB4(int32_t arg0, Vector3 *arg1);
static void script_12EF7(int32_t arg0, Vector3 *arg1);
static void script_12F6D(int32_t arg0, float *arg1);
static void script_12FC0(void);
static void script_12FF9(void);
static int32_t script_1303E(int8_t arg0, int8_t arg1, int8_t arg2);
static int32_t script_1307B(int8_t arg0, int8_t arg1, int8_t arg2);
static void script_130D6(void);
static void script_13112(void);
static int32_t script_1315A(void);
static void script_131B8(int32_t arg0);
static int16_t script_13359(int8_t arg0, int8_t arg1);
static void script_13544(int32_t arg0, int32_t arg1);
static int32_t script_13593(int32_t arg0, int32_t arg1);
static void script_135E6(int32_t arg0, int32_t arg1, int32_t arg2, float arg3);
static void script_1381D(void);
static void script_139C6(int32_t arg0);
static int32_t script_13A30(int32_t arg0);
static int32_t script_13C61(void);
static void script_13CCA(void);
static int32_t script_13D6D(void);
static void script_13DB9(void);
static void script_13DD6(int32_t arg0);
static void script_13EA8(void);
static void script_1404F(void);
static void script_14129(void);
static void script_141ED(void);
static void script_1421C(int32_t arg0);
static void script_143A5(int32_t arg0, int32_t arg1);
static int32_t script_14426(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4);
static int32_t script_144E0(void);
static int32_t script_1461B(float arg0);
static int32_t script_146EB(void);
static int32_t script_147B4(void);
static int32_t script_1485E(void);
static int32_t script_148A8(void);
static void script_149A3(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3);
static int32_t script_14B94(void);
static int32_t script_14F30(void);
static int32_t script_15022(int32_t arg0);
static int32_t script_150A0(int32_t arg0);
static void script_151D2(int32_t arg0);
static int32_t script_1543E(void);
static void script_15516(void);
static int32_t script_15519(int32_t arg0);
static int32_t script_1562F(int16_t arg0);
static int32_t script_15758(void);
static void script_15781(void);
static int32_t script_157EB(void);
static int32_t script_158AD(void);
static int32_t script_15BCC(void);
static void script_15CA9(void);
static int32_t script_15CC1(void);
static int32_t script_15CD4(void);
static void script_15DAF(void);
static int32_t script_15E8A(int32_t arg0, int32_t arg1);
static void script_16043(int32_t arg0);
static void script_160D6(int32_t arg0, int32_t arg1);
static void script_1618C(void);
static void script_16206(int32_t arg0);
static int32_t script_1631C(void);
static int32_t script_1687C(int32_t arg0, const char *arg1, const char *arg2, ...);
static void script_168CE(int32_t arg0, Vector3 *arg1);
static void script_16906(int32_t arg0, Vector3i *arg1);
static void script_1693E(int32_t arg0);
static void script_16948(void);
static void script_1696C(void);
static void script_16AA3(int32_t arg0);
static int32_t script_17057(int32_t arg0);
static int8_t script_170B6(int32_t arg0);
static void script_17A7E(void);
static void script_181E3(void);
static void script_18878(void);
static int32_t script_18935(void);
static void script_19154(void);
static int32_t script_19213(void);
static int32_t script_1924C(int32_t arg0);
static void script_19283(int32_t arg0);
static void script_19605(void);
static void script_196F5(void);
static void script_1999E(void);
static void script_19CFF(void);
static void script_19D7A(void);
static void script_19E18(void);
static void script_19E79(void);
static int32_t script_19EF5(void);
static int32_t script_19F7D(int32_t arg0, int32_t arg1);
static void script_1A2A6(int32_t arg0);
static void script_1A577(void);
static void script_1AE3E(void);
static void script_1B239(void);
static void script_1B318(void);
static void script_1B370(void);
void script_1B40D(void);
static int32_t script_1B729(void);
static void script_1B822(void);
static void script_1B8D8(void);
static int32_t script_1B900(void);
static int32_t script_1B931(void);
static int32_t script_1BD6B(void);
static int32_t script_1BDBD(void);
static void script_1BDE6(void);
static void script_1BE93(void);
static void script_1BEAD(const char *arg0, const char *arg1);
static void script_1BED0(const char *arg0, const char *arg1);
static void script_1BEF4(void);

static DSWKDoorEntry *task_0;
static int32_t *task_4;
static DoorResourceDescriptor *task_8;
static MapObjectRecord *task_C;
static DoorSlotState task_10;
static int32_t task_1C;
static int32_t task_20[10];
static int32_t task_48[10];
static int32_t task_70[10];
static int32_t task_98[10];
static int32_t task_C0[10];
static int32_t task_E8[10];
static int32_t task_110[10];
static int32_t task_138[10];
static int32_t task_160;
static int32_t task_164;
static Vector3 task_168;
static int32_t task_174;
static int32_t task_178;
static int32_t task_17C;
static int8_t task_180;
static uint8_t task_181;
static uint16_t task_182;
static int8_t task_184;
static uint8_t task_185;
static uint16_t task_186;
static int32_t task_188;
static int32_t task_18C;
static int8_t task_190;
static uint8_t task_191;
static uint16_t task_192;
static Vector3 task_194;
static int32_t task_1A0;
static int32_t task_1A4;
static int32_t task_1A8;
static int8_t task_1AC;
static int8_t task_1AD;
static int8_t task_1AE;
static int8_t task_1AF;
static GameDateTime task_1B0;
static uint8_t task_1B8;
static int8_t task_1B9;
static int8_t task_1BA;
static int8_t task_1BB;
static int8_t task_1BC;
static int8_t task_1BD;
static int8_t task_1BE;
static int8_t task_1BF;
static int8_t task_1C0;
static uint8_t task_1C1;
static uint16_t task_1C2;
static int32_t task_1C4;
static int32_t task_1C8;
static int32_t task_1CC;
static int32_t task_1D0;
static int32_t task_1D4;
static int32_t task_1D8;
static int32_t task_1DC;
static int32_t task_1E0;
static int8_t task_1E4;
static uint8_t task_1E5;
static uint16_t task_1E6;
static int32_t task_1E8;
static int32_t task_1EC;
static int32_t task_1F0;
static int8_t task_1F4;
static int8_t task_1F5;
static int8_t task_1F6;
static int8_t task_1F7;
static int8_t task_1F8;
static uint8_t task_1F9;
static uint16_t task_1FA;
static int32_t task_1FC;
static char *task_200;
static int32_t task_204;
static int32_t task_208;
static int32_t task_20C;
static int8_t task_210;
static int8_t task_211;
static int8_t task_212;
static uint8_t task_213;
static int32_t task_214;
static int32_t task_218;
static int32_t task_21C;
static int32_t task_220;
static int32_t task_224;
static int32_t task_228;
static int32_t task_22C;
static int32_t task_230;
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
static int16_t task_252;
static int32_t task_254;
static int8_t task_258;
static int8_t task_259;
static int8_t task_25A;
static int8_t task_25B;
static int8_t task_25C;
static uint8_t task_25D;
static uint16_t task_25E;
static int32_t task_260;
static int32_t task_264;
static int8_t task_268;
static int8_t task_269;
static int8_t task_26A;
static int8_t task_26B;
static Vector3 task_26C;
static Vector3i task_278;
static GameDateTime task_284;
static int32_t task_28C;
static GameDateTime task_290;
static int32_t task_298;
static int32_t task_29C;
static int32_t task_2A0;
static int32_t task_2A4;
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
static const Vector3 data_1BFF0 = {.x = 1.0f, .y = 1.0f, .z = 1.0f};
static const int32_t data_1BFFC[4] = {16777215, 16777215, 16777215, 16777215};
static const float data_1C00C[3] = {20.0f, 800.0f, 1.0f};
static const int32_t data_1C018 = -1;
static DoorMotionSelectorBlock data_1C01C[38] = {
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
static ScnDoorMotionDescriptor data_1C60C[72] = {
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
static uint32_t data_1D02C = 0xFFFFFFFF;
typedef struct Row_1D030 { float w0; float w1; float w2; float w3; int32_t w4; } Row_1D030;
static const Row_1D030 data_1D030[7] = {
  {0.0f, 0.0f, 0.0f, 0.0f, -1},
  {0.0f, 0.0f, 0.0f, 0.0f, -1},
  {0.0f, 0.0f, 0.0f, 0.0f, 1534},
  {26.0f, 55.0f, 45.0f, 75.0f, 1555},
  {61.0f, 146.0f, 45.0f, 50.0f, 1554},
  {59.0f, 124.0f, 45.0f, 70.0f, 1543},
  {33.0f, 49.0f, 45.0f, 50.0f, 1543}
};
static const float data_1D0BC[4] = {33.0f, 49.0f, 50.0f, 50.0f};
static void (*data_1D0CC)(int32_t, int32_t, int32_t) = script_0768;
static void (*const data_1D0D0)(int32_t, int32_t, int32_t, int32_t) = script_0778;
static void (*const data_1D0D4)(int32_t, int32_t, int32_t, int32_t) = script_0778;
static void (*const data_1D0D8)(int32_t, int32_t, int32_t, int32_t) = script_0778;
static void (*const data_1D0DC)(int32_t, int32_t, int32_t, int32_t) = script_0778;
static void (*const data_1D0E0)(int32_t, int32_t, int32_t, int32_t) = script_0778;
static void (*const data_1D0E4)(int32_t, int32_t, int32_t, int32_t) = script_0778;
static void (*const data_1D0E8)(int32_t, int32_t, int32_t, int32_t) = script_0778;
static void (*const data_1D0EC)(int32_t, int32_t, int32_t, int32_t) = script_0778;
static void (*const data_1D0F0)(int32_t, int32_t, int32_t, int32_t) = script_0A1E;
static void (*const data_1D0F4)(int32_t, int32_t, int32_t, int32_t) = script_0B7F;
static void (*const data_1D0F8)(int32_t, int32_t, int32_t, int32_t) = script_104C;
static void (*const data_1D0FC)(int32_t, int32_t, int32_t, int32_t) = script_10B3;
static void (*const data_1D100)(int32_t, int32_t, int32_t, int32_t) = script_087C;
static void (*const data_1D104)(int32_t, int32_t, int32_t, int32_t) = script_087C;
static void (*const data_1D108)(int32_t, int32_t, int32_t, int32_t) = script_0CE0;
static void (*const data_1D10C)(int32_t, int32_t, int32_t, int32_t) = script_0F7E;
static void (*const data_1D110)(int32_t, int32_t, int32_t, int32_t) = script_111A;
static void (*const data_1D114)(int32_t, int32_t, int32_t, int32_t) = script_0A1E;
static void (*const data_1D118)(int32_t, int32_t, int32_t, int32_t) = script_0B7F;
static void (*const data_1D11C)(int32_t, int32_t, int32_t, int32_t) = script_0CE0;
static void (*const data_1D120)(int32_t, int32_t, int32_t, int32_t) = script_0A1E;
static void (*const data_1D124)(int32_t, int32_t, int32_t, int32_t) = script_0B7F;
static void (*const data_1D128)(int32_t, int32_t, int32_t, int32_t) = script_0CE0;
static void (*const data_1D12C)(int32_t, int32_t, int32_t, int32_t) = script_1161;
static void (*const data_1D130)(int32_t, int32_t, int32_t, int32_t) = script_1161;
static void (*const data_1D134)(int32_t, int32_t, int32_t, int32_t) = script_1161;
static void (*const data_1D138)(int32_t, int32_t, int32_t, int32_t) = script_1161;
static void (*const data_1D13C)(int32_t, int32_t, int32_t, int32_t) = script_1297;
static void (*const data_1D140)(int32_t, int32_t, int32_t, int32_t) = script_1B58;
static void (*const data_1D144)(int32_t, int32_t, int32_t, int32_t) = script_0778;
static void (*const data_1D148)(int32_t, int32_t, int32_t, int32_t) = script_0778;
static void (*const data_1D14C)(int32_t, int32_t, int32_t, int32_t) = script_0778;
static void (*const data_1D150)(int32_t, int32_t, int32_t, int32_t) = script_0778;
static void (*const data_1D154)(int32_t, int32_t, int32_t, int32_t) = script_0778;
static void (*const data_1D158)(int32_t, int32_t, int32_t, int32_t) = script_0778;
static void (*const data_1D15C)(int32_t, int32_t, int32_t, int32_t) = script_0778;
static void (*const data_1D160)(int32_t, int32_t, int32_t, int32_t) = script_0778;
static const char data_1D164[46] __attribute__((scn_string_literal)) = "Door Index = %d : call_type = %d : type = %d\012";
static const char data_1D192[43] __attribute__((scn_string_literal)) = "Door Open Motion = DEB_AKESIME_FUSUMA_L_F\012";
static const char data_1D1BD[43] __attribute__((scn_string_literal)) = "Door Open Motion = DEB_AKESIME_FUSUMA_R_F\012";
static const char data_1D1E8[42] __attribute__((scn_string_literal)) = "Door Open Motion = DEB_HIKISIME_DOOR_L_F\012";
static const char data_1D212[42] __attribute__((scn_string_literal)) = "Door Open Motion = DEB_HIKISIME_DOOR_R_F\012";
static const char data_1D23C[41] __attribute__((scn_string_literal)) = "Door Open Motion = DEB_OSISIME_DOOR_L_F\012";
static const char data_1D265[41] __attribute__((scn_string_literal)) = "Door Open Motion = DEB_OSISIME_DOOR_R_F\012";
static const char data_1D28E[43] __attribute__((scn_string_literal)) = "Door Open Motion = GAK_AKESIME_FUSUMA_L_F\012";
static const char data_1D2B9[43] __attribute__((scn_string_literal)) = "Door Open Motion = GAK_AKESIME_FUSUMA_R_F\012";
static const char data_1D2E4[42] __attribute__((scn_string_literal)) = "Door Open Motion = GAK_HIKISIME_DOOR_L_F\012";
static const char data_1D30E[42] __attribute__((scn_string_literal)) = "Door Open Motion = GAK_HIKISIME_DOOR_R_F\012";
static const char data_1D338[41] __attribute__((scn_string_literal)) = "Door Open Motion = GAK_OSISIME_DOOR_L_F\012";
static const char data_1D361[41] __attribute__((scn_string_literal)) = "Door Open Motion = GAK_OSISIME_DOOR_R_F\012";
static const char data_1D38A[43] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_AKESIME_FUSUMA_L_F\012";
static const char data_1D3B5[43] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_AKESIME_FUSUMA_R_F\012";
static const char data_1D3E0[42] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_HIKISIME_DOOR_L_F\012";
static const char data_1D40A[42] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_HIKISIME_DOOR_R_F\012";
static const char data_1D434[41] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_OSISIME_DOOR_L_F\012";
static const char data_1D45D[41] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_OSISIME_DOOR_R_F\012";
static const char data_1D486[43] __attribute__((scn_string_literal)) = "Door Open Motion = KOD_AKESIME_FUSUMA_L_F\012";
static const char data_1D4B1[43] __attribute__((scn_string_literal)) = "Door Open Motion = KOD_AKESIME_FUSUMA_R_F\012";
static const char data_1D4DC[42] __attribute__((scn_string_literal)) = "Door Open Motion = KOD_HIKISIME_DOOR_L_F\012";
static const char data_1D506[42] __attribute__((scn_string_literal)) = "Door Open Motion = KOD_HIKISIME_DOOR_R_F\012";
static const char data_1D530[41] __attribute__((scn_string_literal)) = "Door Open Motion = KOD_OSISIME_DOOR_L_F\012";
static const char data_1D559[41] __attribute__((scn_string_literal)) = "Door Open Motion = KOD_OSISIME_DOOR_R_F\012";
static const char data_1D582[43] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_AKESIME_FUSUMA_L_F\012";
static const char data_1D5AD[43] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_AKESIME_FUSUMA_R_F\012";
static const char data_1D5D8[42] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_HIKISIME_DOOR_L_F\012";
static const char data_1D602[42] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_HIKISIME_DOOR_R_F\012";
static const char data_1D62C[41] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_OSISIME_DOOR_L_F\012";
static const char data_1D655[41] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_OSISIME_DOOR_R_F\012";
static const char data_1D67E[43] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_AKESIME_FUSUMA_L_F\012";
static const char data_1D6A9[43] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_AKESIME_FUSUMA_R_F\012";
static const char data_1D6D4[42] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_HIKISIME_DOOR_L_F\012";
static const char data_1D6FE[42] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_HIKISIME_DOOR_R_F\012";
static const char data_1D728[41] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_OSISIME_DOOR_L_F\012";
static const char data_1D751[41] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_OSISIME_DOOR_R_F\012";
static const char data_1D77A[43] __attribute__((scn_string_literal)) = "Door Open Motion = SIN_AKESIME_FUSUMA_L_F\012";
static const char data_1D7A5[43] __attribute__((scn_string_literal)) = "Door Open Motion = SIN_AKESIME_FUSUMA_R_F\012";
static const char data_1D7D0[42] __attribute__((scn_string_literal)) = "Door Open Motion = SIN_HIKISIME_DOOR_L_F\012";
static const char data_1D7FA[42] __attribute__((scn_string_literal)) = "Door Open Motion = SIN_HIKISIME_DOOR_R_F\012";
static const char data_1D824[41] __attribute__((scn_string_literal)) = "Door Open Motion = SIN_OSISIME_DOOR_L_F\012";
static const char data_1D84D[41] __attribute__((scn_string_literal)) = "Door Open Motion = SIN_OSISIME_DOOR_R_F\012";
static const char data_1D876[43] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_AKESIME_FUSUMA_L_F\012";
static const char data_1D8A1[43] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_AKESIME_FUSUMA_R_F\012";
static const char data_1D8CC[42] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_HIKISIME_DOOR_L_F\012";
static const char data_1D8F6[42] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_HIKISIME_DOOR_R_F\012";
static const char data_1D920[41] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_OSISIME_DOOR_L_F\012";
static const char data_1D949[41] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_OSISIME_DOOR_R_F\012";
static const char data_1D972[43] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_AKESIME_FUSUMA_L_F\012";
static const char data_1D99D[43] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_AKESIME_FUSUMA_R_F\012";
static const char data_1D9C8[42] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_HIKISIME_DOOR_L_F\012";
static const char data_1D9F2[42] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_HIKISIME_DOOR_R_F\012";
static const char data_1DA1C[41] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_OSISIME_DOOR_L_F\012";
static const char data_1DA45[41] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_OSISIME_DOOR_R_F\012";
static const char data_1DA6E[44] __attribute__((scn_string_literal)) = "Door Open Motion = JIJ_OSISIME_DOOR_AP_L_F\012";
static const char data_1DA9A[44] __attribute__((scn_string_literal)) = "Door Open Motion = OTH_OSISIME_DOOR_AP_L_F\012";
static const char data_1DAC6[44] __attribute__((scn_string_literal)) = "Door Open Motion = PNW_OSISIME_DOOR_AP_L_F\012";
static const char data_1DAF2[44] __attribute__((scn_string_literal)) = "Door Open Motion = SYP_OSISIME_DOOR_AP_L_F\012";
static const char data_1DB1E[44] __attribute__((scn_string_literal)) = "Door Open Motion = YKI_OSISIME_DOOR_AP_L_F\012";
static const char data_1DB4A[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_1DB66[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_1DB82[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_1DB9E[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_1DBBA[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_1DBD6[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_1DBF2[28] __attribute__((scn_string_literal)) = "This Motion don't Finish!!\012";
static const char data_1DC0E[13] __attribute__((scn_string_literal)) = "open-move!!\012";
static const char data_1DC1B[11] __attribute__((scn_string_literal)) = "M_MDOR.BIN";
static const char data_1DC26[] __attribute__((scn_string_literal)) = "This Charactor Work is NULL!!\n";
static const char data_1DC45[22] __attribute__((scn_string_literal)) = "手動ドア開け番号指定\012";
static const char data_1DC5B[34] __attribute__((scn_string_literal)) = "無限ループになりそうなので抜ける\012";
static const char data_1DC7D[] __attribute__((scn_string_literal)) = "Don't make BOX Buffer!!\n";
static const char data_1DC96[] __attribute__((scn_string_literal)) = "Don't make BOX Buffer!!\n";
static const char data_1DCAF[15] __attribute__((scn_string_literal)) = "Door System : ";
static const char data_1DCBE[15] __attribute__((scn_string_literal)) = "Door System : ";
static const char data_1DCCD[18] __attribute__((scn_string_literal)) = "Door Box is ON!!\012";
static const char data_1DCDF[19] __attribute__((scn_string_literal)) = "Door Box is OFF!!\012";
static const char data_1DCF2[18] __attribute__((scn_string_literal)) = "Door Box is ON!!\012";
static const char data_1DD04[20] __attribute__((scn_string_literal)) = "Door Box is OFF!!\012";
static const DSWKDoorEntry data_1DD18 = {.slot = 0, .flags = 0, .ctrl_mask_pair = 0x640064U, .coarse_metric = 0.0f, .selection_metric = 0.0f, .view_ang_delta = 0, .group = 0, .global1 = 0xFFFFFFFFU, .global2 = 0xFFFFFFFFU, .global3 = 0xFFFFFFFFU, .sndCommandId = 0xFFFFFFFFU, .global5 = 0xFFFFFFFFU, .global6 = 0xFFFFFFFFU};
static const int32_t data_1DD4C[13] = {-1, 0, 6553700, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1};
static const uint8_t data_1DD80[4] = {0};
static const int32_t data_1DD84 = -1;
static char data_1DD88[] = "DR02_001";
static const uint8_t data_1DD91[3] = {0};
static char *const data_1DD94 = data_1DD88;
static const int32_t data_1DD98[3] = {2, 1966763, 2032299};
static const MapObjectRecord data_1DDA4 = {.kind = 2, .resourceIndex = 0, .scale = {1.0f, 1.0f, 1.0f}, .pos = {-83.095467f, 0.0f, 59.85025f}, .yawFixed16 = -8191};
static const uint32_t data_1DDC8[4] = {0x413042CU, 0x42F042DU, 0x4300432U, 0x3870431U};
static const char data_1DDD8[20] __attribute__((scn_string_literal)) = "TUNED FreeTalk def\012";
static const char data_1DDEC[20] __attribute__((scn_string_literal)) = "TUNED FreeTalk def\012";
static const uint32_t data_1DE00 = 380;
static const int32_t data_1DE04[6] = {1, 390, 0, 1018, 0, -1};
static const uint32_t data_1DE1C = 510;
static const int32_t data_1DE20[6] = {1, 525, 0, 1020, 0, -1};
static const uint32_t data_1DE38 = 150;
static const int32_t data_1DE3C[6] = {1, 160, 0, 1019, 0, -1};
static const uint32_t data_1DE54 = 530;
static const int32_t data_1DE58[6] = {1, 970, 0, 1014, 0, -1};
static const uint32_t data_1DE70 = 911;
static const int32_t data_1DE74[6] = {1, 155, 0, 1015, 0, -1};
static const char data_1DE8C[7] __attribute__((scn_string_literal)) = "FREE02";
static const char data_1DE93[7] __attribute__((scn_string_literal)) = "FREE03";
static const char data_1DE9A[7] __attribute__((scn_string_literal)) = "FREE04";
static const char data_1DEA1[7] __attribute__((scn_string_literal)) = "FREE01";
static Vector3 data_1DEA8 = {.x = 0.0f, .y = 0.0f, .z = 0.0f};
static Vector3 data_1DEB4 = {.x = 1.0f, .y = 1.0f, .z = 1.0f};
static const char data_1DEC0[5] __attribute__((scn_string_literal)) = "SY11";
static const char data_1DEC5[9] __attribute__((scn_string_literal)) = "SY11A006";
static const char data_1DECE[9] __attribute__((scn_string_literal)) = "SY11A007";
static const char data_1DED7[9] __attribute__((scn_string_literal)) = "SY11A008";
static const char data_1DEE0[9] __attribute__((scn_string_literal)) = "SY11A002";
static const char data_1DEE9[9] __attribute__((scn_string_literal)) = "SY11A003";
static const char data_1DEF2[9] __attribute__((scn_string_literal)) = "SY11A004";
static const char data_1DEFB[9] __attribute__((scn_string_literal)) = "SY11A005";
static const char data_1DF04[9] __attribute__((scn_string_literal)) = "SY11A001";
static const char data_1DF0D[9] __attribute__((scn_string_literal)) = "SY11A009";
static const char data_1DF16[9] __attribute__((scn_string_literal)) = "SY11A010";
static const char data_1DF1F[9] __attribute__((scn_string_literal)) = "SY11A011";
static const char data_1DF28[9] __attribute__((scn_string_literal)) = "SY11A009";
static const char data_1DF31[5] __attribute__((scn_string_literal)) = "SY11";
static const char data_1DF36[5] __attribute__((scn_string_literal)) = "SY11";
static const char data_1DF3B[9] __attribute__((scn_string_literal)) = "SY11A001";
static const char data_1DF44[5] __attribute__((scn_string_literal)) = "SY11";
static const char data_1DF49[5] __attribute__((scn_string_literal)) = "SY11";
static const char data_1DF4E[9] __attribute__((scn_string_literal)) = "SY11A009";
static const char data_1DF57[9] __attribute__((scn_string_literal)) = "SY11A010";
static const char data_1DF60[9] __attribute__((scn_string_literal)) = "SY11A011";
static const char data_1DF69[5] __attribute__((scn_string_literal)) = "SY11";
static const char data_1DF6E[6] __attribute__((scn_string_literal)) = "E1067";
static const char data_1DF74[16] __attribute__((scn_string_literal)) = "Volume = %3d";
static const Vector4 data_1DF84 = {.x = 0.0f, .y = 0.7f, .z = 0.7f, .w = 0.7f};
static const char data_1DF94[10] __attribute__((scn_string_literal)) = "E1067A011";
static const char data_1DF9E[10] __attribute__((scn_string_literal)) = "E1067A001";
static const char data_1DFA8[10] __attribute__((scn_string_literal)) = "E1067A002";
static const char data_1DFB2[10] __attribute__((scn_string_literal)) = "E1067A003";
static const char data_1DFBC[10] __attribute__((scn_string_literal)) = "E1067A004";
static const char data_1DFC6[10] __attribute__((scn_string_literal)) = "E1067A005";
static const char data_1DFD0[10] __attribute__((scn_string_literal)) = "E1067A006";
static const char data_1DFDA[10] __attribute__((scn_string_literal)) = "E1067A007";
static const char data_1DFE4[10] __attribute__((scn_string_literal)) = "E1067A008";
static const char data_1DFEE[10] __attribute__((scn_string_literal)) = "E1067A009";
static const char data_1DFF8[10] __attribute__((scn_string_literal)) = "E1067A011";
static const char data_1E002[10] __attribute__((scn_string_literal)) = "E1067A009";
static const char data_1E00C[10] __attribute__((scn_string_literal)) = "E1067A010";
static const char data_1E016[10] __attribute__((scn_string_literal)) = "E1067A011";
static const char data_1E020[] __attribute__((scn_string_literal)) = "ykm003.snd";
static const char data_1E02B[] __attribute__((scn_string_literal)) = "ykm002.snd";
static const char data_1E036[] __attribute__((scn_string_literal)) = "bgm045.snd";
static const char data_1E041[] __attribute__((scn_string_literal)) = "tsm002.snd";
static const char data_1E04C[] __attribute__((scn_string_literal)) = "bgm015.snd";
static const char data_1E057[] __attribute__((scn_string_literal)) = "bgm016.snd";
static const char data_1E062[] __attribute__((scn_string_literal)) = "bgm014.snd";
static const char data_1E06D[] __attribute__((scn_string_literal)) = "bgm047.snd";
static const char data_1E078[] __attribute__((scn_string_literal)) = "bgm075.snd";
static const char data_1E083[] __attribute__((scn_string_literal)) = "bgm010.snd";
static const char data_1E08E[] __attribute__((scn_string_literal)) = "ykm003.snd";
static const char data_1E099[] __attribute__((scn_string_literal)) = "ykm002.snd";
static const char data_1E0A4[] __attribute__((scn_string_literal)) = "bgm045.snd";
static const char data_1E0AF[] __attribute__((scn_string_literal)) = "tsm002.snd";
static const char data_1E0BA[] __attribute__((scn_string_literal)) = "bgm015.snd";
static const char data_1E0C5[] __attribute__((scn_string_literal)) = "bgm016.snd";
static const char data_1E0D0[] __attribute__((scn_string_literal)) = "bgm014.snd";
static const char data_1E0DB[] __attribute__((scn_string_literal)) = "bgm047.snd";
static const char data_1E0E6[] __attribute__((scn_string_literal)) = "bgm075.snd";
static const char data_1E0F1[] __attribute__((scn_string_literal)) = "bgm010.snd";
static const char data_1E0FC[] __attribute__((scn_string_literal)) = "bgm061.snd";
static const char data_1E107[] __attribute__((scn_string_literal)) = "bgm058.snd";
static const char data_1E112[] __attribute__((scn_string_literal)) = "bgm057.snd";
static const char data_1E11D[] __attribute__((scn_string_literal)) = "bgm056.snd";
static const char data_1E128[] __attribute__((scn_string_literal)) = "bgm015.snd";
static const char data_1E133[] __attribute__((scn_string_literal)) = "bgm016.snd";
static const char data_1E13E[] __attribute__((scn_string_literal)) = "bgm014.snd";
static const char data_1E149[] __attribute__((scn_string_literal)) = "bgm047.snd";
static const char data_1E154[] __attribute__((scn_string_literal)) = "bgm075.snd";
static const char data_1E15F[] __attribute__((scn_string_literal)) = "bgm010.snd";
static const char data_1E16A[] __attribute__((scn_string_literal)) = "bgm125.snd";
static const char data_1E175[] __attribute__((scn_string_literal)) = "bgm074.snd";
static const char data_1E180[] __attribute__((scn_string_literal)) = "bgm053.snd";
static const char data_1E18B[] __attribute__((scn_string_literal)) = "bgm124.snd";
static const char data_1E196[] __attribute__((scn_string_literal)) = "bgm015.snd";
static const char data_1E1A1[] __attribute__((scn_string_literal)) = "bgm016.snd";
static const char data_1E1AC[] __attribute__((scn_string_literal)) = "bgm014.snd";
static const char data_1E1B7[] __attribute__((scn_string_literal)) = "bgm047.snd";
static const char data_1E1C2[] __attribute__((scn_string_literal)) = "bgm075.snd";
static const char data_1E1CD[] __attribute__((scn_string_literal)) = "bgm010.snd";
static const char data_1E1D8[] __attribute__((scn_string_literal)) = "bgm054.snd";
static const char data_1E1E3[] __attribute__((scn_string_literal)) = "bgm092.snd";
static const char data_1E1EE[] __attribute__((scn_string_literal)) = "bgm086.snd";
static const char data_1E1F9[] __attribute__((scn_string_literal)) = "bgm048.snd";
static const char data_1E204[] __attribute__((scn_string_literal)) = "bgm015.snd";
static const char data_1E20F[] __attribute__((scn_string_literal)) = "bgm016.snd";
static const char data_1E21A[] __attribute__((scn_string_literal)) = "bgm014.snd";
static const char data_1E225[] __attribute__((scn_string_literal)) = "bgm047.snd";
static const char data_1E230[] __attribute__((scn_string_literal)) = "bgm075.snd";
static const char data_1E23B[] __attribute__((scn_string_literal)) = "bgm010.snd";
static const char data_1E246[] __attribute__((scn_string_literal)) = "bgm021.snd";
static const char data_1E251[] __attribute__((scn_string_literal)) = "bgm060.snd";
static const char data_1E25C[] __attribute__((scn_string_literal)) = "bgm090.snd";
static const char data_1E267[] __attribute__((scn_string_literal)) = "bgm072.snd";
static const char data_1E272[] __attribute__((scn_string_literal)) = "bgm015.snd";
static const char data_1E27D[] __attribute__((scn_string_literal)) = "bgm016.snd";
static const char data_1E288[] __attribute__((scn_string_literal)) = "bgm014.snd";
static const char data_1E293[] __attribute__((scn_string_literal)) = "bgm047.snd";
static const char data_1E29E[] __attribute__((scn_string_literal)) = "bgm075.snd";
static const char data_1E2A9[] __attribute__((scn_string_literal)) = "bgm010.snd";
static const uint32_t data_1E2B4 = 0xA300A2;
static const int32_t data_1E2B8[9] = {10813604, 10944678, 11075752, 11206826, 11337900, 11468974, 11600048, 11731122, 11862196};
static const uint32_t data_1E2DC = SHEN_NAME('VM_0');
static const shen_name data_1E2E0[7] = {
  SHEN_NAME('VM_1'), SHEN_NAME('VM_2'), SHEN_NAME('VM_3'), SHEN_NAME('VM_4'), SHEN_NAME('VM_5'), SHEN_NAME('VM_6'), SHEN_NAME('VM_7')
};
static const char data_1E2FC[18] __attribute__((scn_string_literal)) = "/scene/02/stream/";
static const char data_1E30E[5] __attribute__((scn_string_literal)) = "%03d";
static const char data_1E313[] __attribute__((scn_string_literal)) = "ATRK";
static Vector3 data_1E318 = {.x = 0.0f, .y = -0.1f, .z = 0.0f};
static Vector3 data_1E324 = {.x = 0.0f, .y = 0.0f, .z = 0.0f};
static const char data_1E330[] __attribute__((scn_string_literal)) = "VEND";
static const char data_1E335[] __attribute__((scn_string_literal)) = "VEND";
static const char data_1E33A[] __attribute__((scn_string_literal)) = "SPRT";
static const char data_1E33F[7] __attribute__((scn_string_literal)) = "01JUCE";
static const char data_1E346[13] __attribute__((scn_string_literal)) = "HMOT_CAN.BIN";
static const char data_1E353[] __attribute__((scn_string_literal)) = "VEND";
static const char data_1E358[7] __attribute__((scn_string_literal)) = "01JUCE";
static const char data_1E35F[26] __attribute__((scn_string_literal)) = "HandMot read by current!\012";
static const char data_1E379[13] __attribute__((scn_string_literal)) = "HMOT_CAN.BIN";
static const char data_1E386[7] __attribute__((scn_string_literal)) = "01JUCE";
static const char data_1E38D[] __attribute__((scn_string_literal)) = "VEND";
static const uint8_t data_1E392[2] = {0};
static const uint8_t data_1E394[12] = {0};
static const char data_1E3A0[29] __attribute__((scn_string_literal)) = "vendmachine exit is EV_STOP\012";
static const char data_1E3BD[41] __attribute__((scn_string_literal)) = "vendmachine exit is MONEY_LOCK -> %d,%d\012";
static const char data_1E3E6[] __attribute__((scn_string_literal)) = "COKE";
static const char data_1E3EB[] __attribute__((scn_string_literal)) = "FATG";
static const char data_1E3F0[] __attribute__((scn_string_literal)) = "FATO";
static const char data_1E3F5[] __attribute__((scn_string_literal)) = "SPRT";
static const char data_1E3FA[] __attribute__((scn_string_literal)) = "CAFE";
static const uint8_t data_1E3FF[1] = {0};
static const uint32_t data_1E400 = 0x3D204C06;
static const float data_1E404[2] = {0.0f, 0.890018f};
static const uint8_t data_1E40C[12] = {0};
static const uint8_t data_1E418[12] = {0};
static const uint32_t data_1E424 = 0x3DE978D5;
static const float data_1E428[2] = {0.002f, -0.036f};
static const uint32_t data_1E430 = 8192;
static const int32_t data_1E434[2] = {11377, 3640};
static const uint32_t data_1E43C = 0x3D204C06;
static const float data_1E440[2] = {0.0f, 0.890018f};
static const uint8_t data_1E448[12] = {0};
static const uint32_t data_1E454 = 4369;
static const int32_t data_1E458[2] = {0, -2184};
static const char data_1E460[] __attribute__((scn_string_literal)) = "01JUCEA011";
static const char data_1E46B[] __attribute__((scn_string_literal)) = "01JUCEA012";
static const char data_1E476[] __attribute__((scn_string_literal)) = "01JUCEA013";
static const char data_1E481[] __attribute__((scn_string_literal)) = "01JUCEA002";
static const char data_1E48C[] __attribute__((scn_string_literal)) = "01JUCEA003";
static const uint8_t data_1E497[1] = {0};
static const uint32_t data_1E498 = 0x3DE978D5;
static const float data_1E49C[2] = {0.002f, -0.036f};
static const uint32_t data_1E4A4 = 8192;
static const int32_t data_1E4A8[2] = {11377, 3640};
static const uint32_t data_1E4B0 = 0x3DCB04AB;
static const float data_1E4B4[2] = {0.02044f, -0.04322f};
static const uint32_t data_1E4BC = 0xFFFFFE69;
static const int32_t data_1E4C0[2] = {3331, -3464};
static const uint32_t data_1E4C8 = 0x3D204C06;
static const float data_1E4CC[2] = {0.0f, 0.890018f};
static const uint8_t data_1E4D4[12] = {0};
static const uint32_t data_1E4E0 = 4369;
static const int32_t data_1E4E4[2] = {0, -2184};
static const uint8_t data_1E4EC[12] = {0};
static const char data_1E4F8[] __attribute__((scn_string_literal)) = "01JUCEA002";
static const char data_1E503[] __attribute__((scn_string_literal)) = "01JUCEA003";
static const uint8_t pad_1E50E[4] = {0};
static const uint8_t data_1E512[10] = {
  0x80, 0x3F, 0x00, 0x00, 0x80, 0x3F, 0x00, 0x00, 0x80, 0x3F
};
static const uint32_t data_1E51C = 0xC2A0B7DC;
typedef struct Row_1E520 { float w0; uint32_t w1; int32_t w2; uint32_t w3; } Row_1E520;
static const Row_1E520 data_1E520[2] = {
  {0.0017f, 0x42356C22U, 188416, 0x0U},
  {0.0f, 0x0U, 0, 0xC2A21924U}
};
static const float data_1E540[2] = {0.0017f, 44.6656f};
static const int32_t data_1E548 = 253952;
static const uint32_t data_1E54C = SHEN_NAME('DART');
static const shen_name data_1E550[2] = {SHEN_NAME('DAR2'), SHEN_NAME('DAR3')};
static const uint32_t data_1E558 = SHEN_NAME('YA_Y');
static const shen_name data_1E55C[4] = {SHEN_NAME('YA_R'), SHEN_NAME('YA_B'), SHEN_NAME('YA_G'), SHEN_NAME('YA_K')};
static const char data_1E56C[16] __attribute__((scn_string_literal)) = "/scene/02/MKYU/";
static const char data_1E57C[5] __attribute__((scn_string_literal)) = "DART";
static const char data_1E581[13] __attribute__((scn_string_literal)) = "HMOT_DRT.BIN";
static const char data_1E58E[5] __attribute__((scn_string_literal)) = "DART";
static const char data_1E593[6] __attribute__((scn_string_literal)) = "E1025";
static const char data_1E599[6] __attribute__((scn_string_literal)) = "E1025";
static const char data_1E59F[] __attribute__((scn_string_literal)) = "E1025A001";
static const char data_1E5A9[] __attribute__((scn_string_literal)) = "E1025A002";
static const char data_1E5B3[] __attribute__((scn_string_literal)) = "E1025A003";
static const char data_1E5BD[] __attribute__((scn_string_literal)) = "E1025A004";
static const char data_1E5C7[] __attribute__((scn_string_literal)) = "E1025A005";
static const char data_1E5D1[] __attribute__((scn_string_literal)) = "E1025A009";
static const char data_1E5DB[] __attribute__((scn_string_literal)) = "E1025A010";
static const char data_1E5E5[] __attribute__((scn_string_literal)) = "E1025A011";
static const char data_1E5EF[] __attribute__((scn_string_literal)) = "E1025A006";
static const char data_1E5F9[] __attribute__((scn_string_literal)) = "E1025A007";
static const char data_1E603[] __attribute__((scn_string_literal)) = "E1025A008";
static const uint8_t data_1E60D[3] = {0};
static const Vector3 data_1E610 = {.x = 0.0882f, .y = -0.0112f, .z = 0.0f};
static const Vector3i data_1E61C = {.x = 0, .y = 0, .z = 0};
static const uint32_t data_1E628 = 0x3E21CAC1;
static const float data_1E62C[14] = {0.009f, 0.004f, 0.145f, 0.01f, -0.002f, 0.128f, 0.017f, -0.007f, 0.135f, 0.008f, 0.0f, 0.15f, 0.009f, 0.004f};
static const uint8_t data_1E664[4] = {0};
static const int32_t data_1E668[14] = {2001, 15469, 0, 2001, 15469, 0, 2001, 15469, 0, 2001, 15469, 0, 2001, 15469};
static const Vector4 data_1E6A0 = {.x = 0.0f, .y = -0.3f, .z = -0.3f, .w = -0.3f};
static const char data_1E6B0[] __attribute__((scn_string_literal)) = "E1025A022";
static const char data_1E6BA[] __attribute__((scn_string_literal)) = "E1025A023";
static const char data_1E6C4[] __attribute__((scn_string_literal)) = "E1025A024";
static const uint8_t data_1E6CE[2] = {0};
static const Vector4 data_1E6D0 = {.x = 0.0f, .y = 0.7f, .z = 0.7f, .w = 0.7f};
static const char data_1E6E0[] __attribute__((scn_string_literal)) = "E1025A016";
static const char data_1E6EA[] __attribute__((scn_string_literal)) = "E1025A017";
static const char data_1E6F4[] __attribute__((scn_string_literal)) = "E1025A013";
static const char data_1E6FE[] __attribute__((scn_string_literal)) = "E1025A018";
static const char data_1E708[] __attribute__((scn_string_literal)) = "E1025A048";
static const char data_1E712[] __attribute__((scn_string_literal)) = "E1025A018";
static const char data_1E71C[] __attribute__((scn_string_literal)) = "E1025A046";
static const char data_1E726[] __attribute__((scn_string_literal)) = "E1025A012";
static const char data_1E730[] __attribute__((scn_string_literal)) = "E1025A047";
static const char data_1E73A[] __attribute__((scn_string_literal)) = "E1025A014";
static const char data_1E744[] __attribute__((scn_string_literal)) = "E1025A015";
static const char data_1E74E[] __attribute__((scn_string_literal)) = "E1025A050";
static const char data_1E758[] __attribute__((scn_string_literal)) = "E1025A051";
static const char data_1E762[] __attribute__((scn_string_literal)) = "E1025A052";
static const char data_1E76C[] __attribute__((scn_string_literal)) = "E1025A037";
static const char data_1E776[] __attribute__((scn_string_literal)) = "E1025A038";
static const char data_1E780[] __attribute__((scn_string_literal)) = "E1025A039";
static const char data_1E78A[] __attribute__((scn_string_literal)) = "E1025A040";
static const char data_1E794[] __attribute__((scn_string_literal)) = "E1025A041";
static const char data_1E79E[] __attribute__((scn_string_literal)) = "E1025A042";
static const Vector3 data_1E7A8 = {.x = -0.05f, .y = 0.0f, .z = 0.0f};
static const Vector3 data_1E7B4 = {.x = -0.05f, .y = 0.0f, .z = 0.0f};
static const uint32_t data_1E7C0 = 0xBCCCCCCD;
static const uint8_t pad_1E7C4[8] = {0};
static const uint32_t data_1E7CC = 0x3CB78034;
static const float data_1E7D0[2] = {2.155f, -0.04f};
static const uint32_t data_1E7D8 = 0x3E16F007;
static const float data_1E7DC[2] = {2.138f, -0.04f};
static const uint32_t data_1E7E4 = 0x4009EB85;
static const float data_1E7E8[4] = {2.005f, 1.855f, 1.705f, 1.855f};
static const uint32_t data_1E7F8 = 0x4008D4FE;
static const float data_1E7FC[4] = {1.988f, 1.838f, 1.688f, 1.838f};
static const Vector3 data_1E80C = {.x = 0.5f, .y = 0.5f, .z = 0.5f};
static const char data_1E818[] __attribute__((scn_string_literal)) = "E1025A025";
static const char data_1E822[] __attribute__((scn_string_literal)) = "E1025A026";
static const char data_1E82C[] __attribute__((scn_string_literal)) = "E1025A027";
static const char data_1E836[] __attribute__((scn_string_literal)) = "E1025A028";
static const char data_1E840[] __attribute__((scn_string_literal)) = "E1025A029";
static const char data_1E84A[] __attribute__((scn_string_literal)) = "E1025A030";
static const char data_1E854[] __attribute__((scn_string_literal)) = "E1025A011";
static const char data_1E85E[] __attribute__((scn_string_literal)) = "E1025A031";
static const char data_1E868[] __attribute__((scn_string_literal)) = "E1025A032";
static const uint8_t data_1E872[2] = {0};
static const Vector3 data_1E874 = {.x = -0.1061f, .y = 0.0f, .z = 0.0f};
static const Vector3 data_1E880 = {.x = -0.0474f, .y = 0.0f, .z = 0.0f};
static const Vector3 data_1E88C = {.x = 0.1344f, .y = 1.91f, .z = -0.0388f};
static const Vector3 data_1E898 = {.x = -0.0585f, .y = 1.7258f, .z = -0.0388f};
static const Vector3 data_1E8A4 = {.x = 0.1905f, .y = 1.7258f, .z = -0.0388f};
static const Vector3 data_1E8B0 = {.x = 1.0f, .y = 1.0f, .z = 1.0f};
static const Vector3 data_1E8BC = {.x = 1.8f, .y = 1.6f, .z = 1.0f};
static const uint32_t data_1E8C8 = 153;
static const int32_t data_1E8CC[4] = {154, 155, 156, 157};
static const Vector4 data_1E8DC = {.x = 0.0f, .y = 0.7f, .z = 0.7f, .w = 0.7f};
static const int8_t data_1E8EC[20] = {20, 1, 18, 4, 13, 6, 10, 15, 2, 17, 3, 19, 7, 16, 8, 11, 14, 9, 12, 5};
static const char data_1E900[12] __attribute__((scn_string_literal)) = "rim055.snd";
static const int8_t data_1E90C[16] = {12, 17, 22, 10, 19, 18, 11, 8, 4, 4, 17, 7, 6, 5, 4, 4};
static const int8_t data_1E91C[16] = {12, 1, 2, 1, 3, 2, 2, 8, 4, 4, 17, 7, 6, 5, 4, 4};
static const uint32_t data_1E92C = 0xA800A7;
static const int32_t data_1E930[5] = {8847529, 8978568, 15073509, 15204583, 15335657};
static const uint32_t data_1E944 = 0x440043;
static const int32_t data_1E948[8] = {4587589, 4718663, 4849737, 4980811, 5111885, 5242959, 5374033, 85};
static const uint32_t data_1E968 = 0x5C005B;
static const int32_t data_1E96C[10] = {6160477, 6291551, 6422625, 6553699, 6684773, 6815847, 6946921, 7077995, 7209069, 7340143};
static const uint32_t data_1E994 = 0x740073;
static const int32_t data_1E998[4] = {7798902, 7929976, 8061050, 8257660};
static const uint32_t data_1E9A8 = 0x720071;
static const int32_t data_1E9AC[9] = {10223750, 10354845, 10485919, 10616993, 10748067, 10879141, 11206826, 5767255, 180};
static const uint32_t data_1E9D0 = 0x810080;
static const int32_t data_1E9D4[8] = {8585346, 11403437, 11534511, 11665585, 11862195, 11993270, 12452024, 8650879};
static const uint32_t data_1E9F4 = 0xB900BC;
static const int32_t data_1E9F8[5] = {12255418, 13238473, 13500621, 12517579, 200};
static const uint32_t data_1EA0C = 0x5A0059;
static const int32_t data_1EA10[3] = {5636268, 14221445, 5505107};
static const uint32_t data_1EA1C = 0x7D0075;
static const int32_t data_1EA20 = 12386442;
static const uint32_t data_1EA24 = 0xC600C5;
static const int32_t data_1EA28 = 13369543;
static const uint32_t data_1EA2C = 0x8C008B;
static const int32_t data_1EA30[8] = {9306253, 9437327, 9633937, 148, 9961623, 10092697, 9568411, 149};
static const uint32_t data_1EA50 = 0xD700D6;
static const int32_t data_1EA54[3] = {14287064, 14876891, 228};
static const uint32_t data_1EA60 = 0xDC00D3;
static const int32_t data_1EA64[2] = {13893845, 14549213};
static const uint32_t data_1EA6C = 0xC100C0;
static const int32_t data_1EA70[2] = {12779714, 196};
static const uint32_t data_1EA78 = 0xD000CF;
static const int32_t data_1EA7C = 13762769;
static const uint32_t data_1EA80 = 0xE000DF;
static const int32_t data_1EA84 = 14811361;
static const char data_1EA88[169] = "%GACR016G.MT5\000%GACK66BG.MT5\000%GACK66RG.MT5\000%GACR0BNG.MT5\000%GACR0B2G.MT5\000%GACR0B3G.MT5\000%SPBK6REG.MT5\000%SPBK6BRG.MT5\000%SPBK6YEG.MT5\000%SAIK6WHG.MT5\000%SAIK6BKG.MT5\000%SAIK6REG.MT5";
static const char data_1EB31[315] = "%GACIAK1G.MT5\000%GACIAK2G.MT5\000%GACOJK1G.MT5\000%GACOJK2G.MT5\000%GACOSA1G.MT5\000%GACOSA2G.MT5\000%GACOLA1G.MT5\000%GACOLA2G.MT5\000%GACOPA1G.MT5\000%GACOPA2G.MT5\000%GACGWF1G.MT5\000%GACGWF2G.MT5\000%GACGJF1G.MT5\000%GACGJF2G.MT5\000%GACIKA1G.MT5\000%GACIKA2G.MT5\000%GACM401G.MT5\000%GACIAA1G.MT5\000%GACIAA2G.MT5\000D\000D\000D\000D\000D\000D\000D\000D\000D\000D\000%GACGJJ1G.MT5\000%GACGJJ2G.MT5";
static const char data_1EC6C[309] = "%GACIAKAG.MT5\000%GACIAKBG.MT5\000%GACGJKAG.MT5\000%GACGJKBG.MT5\000%GACOSAAG.MT5\000%GACOSABG.MT5\000%GACOLAAG.MT5\000%GACOLABG.MT5\000%GACOPAAG.MT5\000%GACOPABG.MT5\000%GACIWLAG.MT5\000%GACIWLBG.MT5\000%GACGJFAG.MT5\000%GACGJFBG.MT5\000%GACIKAAG.MT5\000%GACIKABG.MT5\000%GACGSNAG.MT5\000%GACGSNBG.MT5\000%GACILIAG.MT5\000%GACILIBG.MT5\000%GACIDUAG.MT5\000%GACIDUBG.MT5";
static const char data_1EDA1[167] = "%GACRSO1G.MT5\000%GACRSO2G.MT5\000%GACRTL1G.MT5\000%GACRNUCG.MT5\000%GACRBAKG.MT5\000%GACRBING.MT5\000%GACRFNGG.MT5\000%GACRAMYG.MT5\000%GACRESPG.MT5\000%GACREGGG.MT5\000D\000D\000D\000D\000D\000D\000%GACRFNNG.MT5";
static const char data_1EE48[365] = "%GACRNT1G.MT5\000%GACRNT2G.MT5\000%GACRLOPG.MT5\000%GACRMYUG.MT5\000%GACRCKNG.MT5\000%GACRALFG.MT5\000%GACRRISG.MT5\000%GACRARXG.MT5\000%GACR0OPG.MT5\000%GACR0O2G.MT5\000%GACRPOPG.MT5\000%GACRCOBG.MT5\000%GACRIDAG.MT5\000%GACRSHPG.MT5\000%GACRMOBG.MT5\000%GACRROBG.MT5\000%GACK6TZG.MT5\000%GACRPANG.MT5\000%GACK6DYG.MT5\000D\000D\000D\000D\000D\000D\000D\000%GACRAXXG.MT5\000D\000D\000D\000D\000D\000D\000D\000%GACRRBBG.MT5\000%GACK6TSG.MT5\000%GACRPPNG.MT5\000%GACK6DSG.MT5";
static const char data_1EFB5[505] = "%GACK6HHG.MT5\000%GACK6HYG.MT5\000%GACK6HBG.MT5\000%GACK6HGG.MT5\000%GACH5BKG.MT5\000%KAB02FSG.MT5\000%GACO1K2G.MT5\000%GACO1K3G.MT5\000%GACO1K7G.MT5\000%GACO1K8G.MT5\000%GACO1K9G.MT5\000%GACO1K1G.MT5\000%GACO1K4G.MT5\000%GACO1K5G.MT5\000%GACO1K6G.MT5\000%GACS5BSG.MT5\000%GACK6HOG.MT5\000%GACK6HKG.MT5\000%GACK6XHG.MT5\000%GACK6XYG.MT5\000%GACK6XBG.MT5\000%GACK6XGG.MT5\000%GACK6BKG.MT5\000%KABK6FSG.MT5\000%GACOKK2G.MT5\000%GACOKK3G.MT5\000%GACOKK7G.MT5\000%GACOKK8G.MT5\000%GACOKK9G.MT5\000%GACOKK1G.MT5\000%GACOKK4G.MT5\000%GACOKK5G.MT5\000%GACOKK6G.MT5\000%GACS5BBG.MT5\000%GACK6XOG.MT5\000%GACK6XKG.MT5";
static const char data_1F1AE[211] = "%GACH5TKG.MT5\000%GACGTRXG.MT5\000%GACO1T1G.MT5\000%GACO1T2G.MT5\000%GACO1KOG.MT5\000%GACO1KBG.MT5\000%GACO1KJG.MT5\000%GACH5UKG.MT5\000%GACH5IKG.MT5\000%GACO1FOG.MT5\000%GACGSHEG.MT5\000%GACH5TTG.MT5\000%GACGTXXG.MT5\000%GACOTT1G.MT5\000%GACOTT2G.MT5";
static const char data_1F281[113] = "%YKW0201G.MT5\000%YWT0201G.MT5\000%GACRBONG.MT5\000%GACRRENG.MT5\000%GACK6SPG.MT5\000%GACG360G.MT5\000%GACRDR1G.MT5\000%GACRDR2G.MT5";
static const char data_1F2F2[77] = "%GACRSSCG.MT5\000%GACRMETG.MT5\000%GACRGBIG.MT5\000%GACH5TRG.MT5\000D\000D\000D\000%GACH5RRG.MT5";
static const char data_1F33F[57] = "%GACS5FRG.MT5\000%GACS5FBG.MT5\000%GACO1KUG.MT5\000%GACO1FEG.MT5";
static const char data_1F378[239] = "%GACODG1G.MT5\000%GACODG2G.MT5\000%GACODG3G.MT5\000%GACOCT2G.MT5\000%GACOCATG.MT5\000%GACO1SHG.MT5\000%GACOKSHG.MT5\000%GACO1IRG.MT5\000%GACOKIRG.MT5\000%GACO1HAG.MT5\000%GACOHA2G.MT5\000%GACOHA3G.MT5\000%GACOHA4G.MT5\000%GACO1YGG.MT5\000%GACO1USG.MT5\000%GACOOSHG.MT5\000%GACOOIRG.MT5";
static const char data_1F467[99] = "%GACM400G.MT5\000%GACGSPHG.MT5\000%GACS5HGG.MT5\000%GACGDT1G.MT5\000%GACS5EQG.MT5\000%SYOGSH0G.MT5\000%SYOGHG0G.MT5";
static const char data_1F4CA[85] = "%GACGCRYG.MT5\000%GACGBIRG.MT5\000%GACH5SLG.MT5\000%GACH5JKG.MT5\000%SYOGMD1G.MT5\000%SYOGMD5G.MT5";
static const char data_1F51F[71] = "%GACS5N1G.MT5\000%GACS5N2G.MT5\000%GACS5N3G.MT5\000%GACS5N4G.MT5\000%GACS5N5G.MT5";
static const char data_1F566[57] = "%GACGXBNG.MT5\000%GACGYBNG.MT5\000%GACGABNG.MT5\000%GACGBBNG.MT5";
static const char data_1F59F[57] = "%GACRMEGG.MT5\000%GACR0GGG.MT5\000%GACRSATG.MT5\000%GACRCASG.MT5";
static const uint32_t data_1F5D8 = SHEN_NAME('GCH0');
static const shen_name data_1F5DC[5] = {SHEN_NAME('GCH1'), SHEN_NAME('GCH2'), SHEN_NAME('GCH3'), SHEN_NAME('GCH4'), SHEN_NAME('GCH5')};
static const uint32_t data_1F5F0 = SHEN_NAME('HGC0');
static const shen_name data_1F5F4[5] = {SHEN_NAME('HGC1'), SHEN_NAME('HGC2'), SHEN_NAME('HGC3'), SHEN_NAME('HGC4'), SHEN_NAME('HGC5')};
static const uint32_t data_1F608 = SHEN_NAME('GBX0');
static const shen_name data_1F60C[5] = {SHEN_NAME('GBX1'), SHEN_NAME('GBX2'), SHEN_NAME('GBX3'), SHEN_NAME('GBX4'), SHEN_NAME('GBX5')};
static const uint32_t data_1F620 = 0xC29E0595;
static const uint32_t data_1F624[2] = {0x3A03126FU, 0x423BD3A9U};
static const int32_t data_1F62C[17] = {-8192, 1028, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
static const uint32_t data_1F670[3] = {0xC29D2C57U, 0x3951B717U, 0x423D8625U};
static const int32_t data_1F67C[2] = {-8192, 1285};
static const float data_1F684 = -78.1623f;
static const uint32_t data_1F688[2] = {0xB8D1B717U, 0x423F3886U};
static const int32_t data_1F690[2] = {-8192, 1542};
static const char data_1F698[16] __attribute__((scn_string_literal)) = "/scene/02/MKYU/";
static const char data_1F6A8[8] __attribute__((scn_string_literal)) = "GACH";
static const Vector3 data_1F6B0 = {.x = 0.29f, .y = 0.0f, .z = 0.088f};
static const Vector3i data_1F6BC = {.x = 0, .y = 0, .z = 0};
static const char data_1F6C8[6] __attribute__((scn_string_literal)) = "E1066";
static const char data_1F6CE[16] __attribute__((scn_string_literal)) = "/scene/02/MKYU/";
static const char data_1F6DE[8] __attribute__((scn_string_literal)) = "HI_GACH";
static const char data_1F6E6[6] __attribute__((scn_string_literal)) = "E1066";
static const char data_1F6EC[8] __attribute__((scn_string_literal)) = "HI_GACH";
static const char data_1F6F4[8] __attribute__((scn_string_literal)) = "HI_GACH";
static const Vector4 data_1F6FC = {.x = 0.0f, .y = 0.7f, .z = 0.7f, .w = 0.7f};
static const uint8_t data_1F70C[4] = {0};
static const int32_t data_1F710[56] = {
  -546, -5870, 0, 2184, -3726, 0, 182, 0, 0, 0, -5184, 0, 71525, -12075, 0, -182, -5279, 0, -364, 910, 0, 1456, -2894, 0, 3458, -13352, 0, -910,
  -10327, 0, 0, 1820, 0, 0, -6863, 0, 1820, -16438, 0, -364, -10150, 0, 236, 291, 0, 0, -12105, 0, 0, -15539, 0, 0, -7249, 0, -819, -364
};
static const uint8_t data_1F7F0[4] = {0};
static const int32_t data_1F7F4[56] = {
  -546, -5870, 0, 2184, -3726, 0, 182, 0, 0, 0, -5184, 0, 71525, -10922, 0, -182, -5279, 0, -364, 910, 0, 1456, -2894, 0, 3458, -13352, 0, -910,
  -10327, 0, 0, 1820, 0, 0, -6863, 0, 1820, -16438, 0, -364, -10150, 0, 236, 291, 0, 0, -12105, 0, 0, -15539, 0, 0, -7249, 0, -819, -364
};
static const uint8_t data_1F8D4[4] = {0};
static const int32_t data_1F8D8[56] = {
  2912, -7507, 0, 6189, -4454, 0, -1456, 0, 0, 546, -8266, 0, 1292, -7099, 0, 910, -11468, 0, 728, 3276, 0, 1092, -11450, 0, 546, -10256, 0, 1456,
  -6504, 0, 546, 1638, 0, -1092, -12142, 0, -364, -16438, 0, -364, -10150, 0, -309, 291, 0, 1638, -11923, 0, -3276, -17359, 0, -2912, -10707, 0, -819,
  -364
};
static const uint8_t data_1F9B8[4] = {0};
static const int32_t data_1F9BC[56] = {
  1638, -7507, 0, 6189, -4454, 0, -1456, 0, 0, 546, -17731, 0, -709, -9466, 0, 728, -15473, 0, 182, 2548, 0, 1092, -13271, 0, -1092, -14625, 0, 1638,
  -11965, 0, 0, 1638, 0, -1092, -12142, 0, -364, -16438, 0, -364, -10150, 0, -309, 291, 0, 1638, -11923, 0, -3276, -17359, 0, -2912, -10707, 0, -819,
  -364
};
static const uint8_t data_1FA9C[4] = {0};
static const int32_t data_1FAA0[56] = {
  1638, -6779, 0, 6189, -4454, 0, -1456, 0, 0, 546, -17731, 0, -709, -9466, 0, 1274, -15473, 0, 546, 2548, 0, 1092, -13271, 0, -1092, -14625, 0, 1638,
  -11965, 0, -910, 1638, 0, -1092, -12142, 0, -364, -16438, 0, -364, -10150, 0, -309, 291, 0, 1638, -12105, 0, -3276, -17359, 0, -2912, -10889, 0,
  -819, -364
};
static const uint8_t data_1FB80[4] = {0};
static const int32_t data_1FB84[56] = {
  -2730, -9510, -3094, 4004, -1359, 0, 4004, -728, 0, 546, -6990, 0, 18, -4004, 2366, 1274, -11468, 0, 182, -182, 0, 1092, -13271, 0, -1092, -14443,
  0, 1638, -11965, 0, -910, 1638, 0, -1092, -12142, 0, -364, -16438, 0, -364, -10150, 0, -309, 291, 0, 1638, -12105, 0, -3276, -17359, 0, -2912,
  -10889, 0, -819, -364
};
static const uint8_t data_1FC64[8] = {0};
static const int32_t data_1FC6C[55] = {
  -6005, 0, 4731, -6188, 0, 0, 0, 0, 0, -2727, 0, 0, -9097, 0, 0, -5457, 0, 0, 0, 0, 0, -8734, 0, -909, -7277, 0, 0, -7823, 0, 0, 0, 0, 1456, -4002,
  0, 0, -9462, 0, 726, -7824, 0, 0, 0, 0, 0, -12011, 0, -1091, -7461, 0, -1636, -9280, 0, 0, 0
};
static const uint8_t data_1FD48[4] = {0};
static const int32_t data_1FD4C[56] = {
  -546, -5870, 0, 2184, -3726, 0, 182, 0, 0, 0, -5184, 0, 71525, -12075, 0, -182, -5279, 0, -364, 910, 0, 1456, -2894, 0, 3458, -13352, 0, -910,
  -10327, 0, 0, 1820, 0, 0, -6863, 0, 1820, -16438, 0, -364, -10150, 0, 236, 291, 0, 0, -12105, 0, 0, -15539, 0, 0, -7249, 0, -819, -364
};
static const uint8_t data_1FE2C[8] = {0};
static const int32_t data_1FE34[55] = {
  -8046, 0, 0, -1820, -13088, 4350, -2894, 0, 0, -9011, 0, 0, -18422, 0, 0, -9812, 0, 0, 0, 0, 0, -7518, 0, 0, -17385, 0, 0, -11559, 0, 0, 0, 0, 0,
  -5734, 0, 0, -18058, 0, 0, -12233, 0, 0, 0, 0, 0, -7827, 0, 0, -16183, 0, 0, -13981, 0, -72, 0
};
static const Vector3 data_1FF10 = {.x = 0.067f, .y = 0.061f, .z = 0.0379f};
static const uint32_t data_1FF1C = 0x3DB851EC;
static const float data_1FF20[2] = {0.075f, 0.008f};
static const Vector3i data_1FF28 = {.x = 13111, .y = 817, .z = 50684};
static const Vector3 data_1FF34 = {.x = 0.091f, .y = -0.087f, .z = 0.0671f};
static const Vector3i data_1FF40 = {.x = 20024, .y = 65353, .z = 11832};
static const Vector3 data_1FF4C = {.x = 0.0f, .y = 0.0f, .z = 0.0f};
static const Vector3i data_1FF58 = {.x = 0, .y = 0, .z = 0};
static const Vector3 data_1FF64 = {.x = -0.042f, .y = 0.1735f, .z = 0.2062f};
static const char data_1FF70[] __attribute__((scn_string_literal)) = "E1066A001";
static const char data_1FF7A[] __attribute__((scn_string_literal)) = "E1066A002";
static const char data_1FF84[] __attribute__((scn_string_literal)) = "E1066A003";
static const char data_1FF8E[] __attribute__((scn_string_literal)) = "E1066A004";
static const char data_1FF98[] __attribute__((scn_string_literal)) = "E1066A005";
static const char data_1FFA2[] __attribute__((scn_string_literal)) = "E1066A006";
static const char data_1FFAC[] __attribute__((scn_string_literal)) = "E1066A007";
static const char data_1FFB6[] __attribute__((scn_string_literal)) = "E1066A008";
static const char data_1FFC0[] __attribute__((scn_string_literal)) = "E1066A009";
static const char data_1FFCA[] __attribute__((scn_string_literal)) = "E1066A010";
static const char data_1FFD4[] __attribute__((scn_string_literal)) = "E1066A011";
static const char data_1FFDE[] __attribute__((scn_string_literal)) = "E1066A026";
static const char data_1FFE8[] __attribute__((scn_string_literal)) = "E1066A027";
static const char data_1FFF2[] __attribute__((scn_string_literal)) = "E1066A019";
static const char data_1FFFC[] __attribute__((scn_string_literal)) = "E1066A020";
static const char data_20006[] __attribute__((scn_string_literal)) = "E1066A021";
static const char data_20010[] __attribute__((scn_string_literal)) = "E1066A014";
static const char data_2001A[] __attribute__((scn_string_literal)) = "E1066A015";
static const char data_20024[] __attribute__((scn_string_literal)) = "E1066A016";
static const char data_2002E[] __attribute__((scn_string_literal)) = "E1066A017";
static const char data_20038[] __attribute__((scn_string_literal)) = "E1066A018";
static const char data_20042[] __attribute__((scn_string_literal)) = "E1066A022";
static const char data_2004C[] __attribute__((scn_string_literal)) = "E1066A025";
static const char data_20056[] __attribute__((scn_string_literal)) = "E1066A024";
static const char data_20060[12] __attribute__((scn_string_literal)) = "E1066A023";
static const Vector3 data_2006C = {.x = 0.337795f, .y = 0.596267f, .z = -0.181433f};
static const Vector3 data_20078 = {.x = 0.091f, .y = -0.087f, .z = 0.0671f};
static const Vector3i data_20084 = {.x = 20024, .y = 65353, .z = 11832};
static const char data_20090[12] __attribute__((scn_string_literal)) = "JINGURU01";
static uint8_t data_2009C[4] = {0};
static const uint32_t data_200A0[3] = {0x78626B64U, 0x5555E000U, 0};
static const float data_200AC[4] = {0.2f, 0.1f, 0.0f, 0.0f};
static const int32_t data_200BC[2] = {-65536, 100};
static const uint32_t data_200C4 = 0x5555E000U;
static const float data_200C8[5] = {-75.26f, 1.57f, 50.89f, 0.0f, 0.0f};
static const int32_t data_200DC[2] = {-65536, 101};
static const uint32_t data_200E4 = 0x5555E000U;
static const float data_200E8[5] = {-75.3f, 1.2f, 50.66f, 0.0f, 0.0f};
static const int32_t data_200FC[2] = {-65536, 102};
static const uint32_t data_20104 = 0x5555E000U;
static const float data_20108[5] = {-75.3f, 0.86f, 50.66f, 0.0f, 0.0f};
static const int32_t data_2011C[2] = {-65536, 103};
static const uint32_t data_20124 = 0x5555E000U;
static const float data_20128[5] = {-74.5f, 0.31f, 51.32f, 0.0f, 0.0f};
static const int32_t data_2013C[2] = {-65536, 104};
static const uint32_t data_20144 = 0x5555E000U;
static const float data_20148[5] = {-74.17f, 0.31f, 51.69f, 0.0f, 0.0f};
static const int32_t data_2015C[2] = {-65536, 200};
static const uint32_t data_20164 = 0x5555A000U;
static const float data_20168[5] = {-74.59f, 1.5f, 52.33f, 0.0f, 0.0f};
static const int32_t data_2017C[2] = {-65536, 201};
static const uint32_t data_20184 = 0x5555A000U;
static const float data_20188[5] = {-74.59f, 1.12f, 52.33f, 0.0f, 0.0f};
static const int32_t data_2019C[2] = {-65536, 202};
static const uint32_t data_201A4 = 0x5555A000U;
static const float data_201A8[5] = {-74.59f, 0.37f, 52.33f, 0.0f, 0.0f};
static const int32_t data_201BC[2] = {-65536, 203};
static const uint32_t data_201C4 = 0x5555A000U;
static const float data_201C8[2] = {-75.2f, 1.46f};
static const shen_name data_201D0[3] = {SHEN_NAME('33TB'), 0, 0};
static const int32_t data_201DC[2] = {-65536, 204};
static const uint32_t data_201E4 = 0x5555A000U;
static const float data_201E8[2] = {-75.2f, 1.14f};
static const shen_name data_201F0[3] = {SHEN_NAME('33TB'), 0, 0};
static const int32_t data_201FC[2] = {-65536, 205};
static const uint32_t data_20204 = 0x5555A000U;
static const float data_20208 = -75.2f;
static const char str_2020C[] __attribute__((scn_string_literal)) = ")\\O?33TB";
static const uint8_t pad_20215[9] = {0};
static const uint8_t data_2021E[16] = {
  0xFF, 0xFF, 0xCE, 0x00, 0x00, 0x00, 0x00, 0xA0, 0x55, 0x55, 0x66, 0x66, 0x96, 0xC2, 0x9A, 0x99
};
static const uint8_t data_2022E[16] = {
  0x99, 0x3E, 0x33, 0x33, 0x54, 0x42, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
static const uint8_t data_2023E[16] = {
  0xFF, 0xFF, 0xCF, 0x00, 0x00, 0x00, 0x00, 0xE0, 0x55, 0x55, 0xF6, 0x28, 0x9D, 0xC2, 0x8F, 0xC2
};
static const uint8_t data_2024E[16] = {
  0xD5, 0x3F, 0x1F, 0x85, 0x4A, 0x42, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
static const uint8_t data_2025E[16] = {
  0xFF, 0xFF, 0xD0, 0x00, 0x00, 0x00, 0x00, 0xE0, 0x55, 0x55, 0xF6, 0x28, 0x9D, 0xC2, 0x66, 0x66
};
static const uint8_t data_2026E[16] = {
  0xA6, 0x3F, 0x1F, 0x85, 0x4A, 0x42, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
static const uint8_t data_2027E[16] = {
  0xFF, 0xFF, 0xD1, 0x00, 0x00, 0x00, 0x00, 0xE0, 0x55, 0x55, 0xF6, 0x28, 0x9D, 0xC2, 0x0A, 0xD7
};
static const uint8_t data_2028E[16] = {
  0x63, 0x3F, 0x1F, 0x85, 0x4A, 0x42, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
static const uint8_t data_2029E[16] = {
  0xFF, 0xFF, 0x2C, 0x01, 0x00, 0x00, 0x05, 0x5B, 0x00, 0x00, 0x0A, 0x57, 0xA8, 0xC2, 0x33, 0x33
};
static const uint8_t data_202AE[16] = {
  0x73, 0x3F, 0xA4, 0x70, 0x5C, 0x42, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
static const uint8_t data_202BE[16] = {
  0xFF, 0xFF, 0x2D, 0x01, 0x00, 0x00, 0x00, 0x20, 0x55, 0x55, 0x85, 0xEB, 0xAA, 0xC2, 0x71, 0x3D
};
static const uint8_t data_202CE[16] = {
  0x0A, 0x40, 0x0A, 0xD7, 0x42, 0x42, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01
};
static const uint8_t data_202DE[16] = {
  0xFF, 0xFF, 0x2E, 0x01, 0x00, 0x00, 0x00, 0xE0, 0x55, 0x55, 0xC3, 0x75, 0x9A, 0xC2, 0xA4, 0x70
};
static const uint8_t data_202EE[16] = {
  0x3D, 0x3F, 0x7B, 0x14, 0x4E, 0x42, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01
};
static const uint8_t data_202FE[16] = {
  0xFF, 0xFF, 0x2F, 0x01, 0x00, 0x00, 0x00, 0xE0, 0xC7, 0x71, 0xB8, 0x9E, 0x9E, 0xC2, 0x5C, 0x8F
};
static const uint8_t data_2030E[16] = {
  0xC2, 0x3F, 0x52, 0xB8, 0x49, 0x42, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
static const uint8_t data_2031E[16] = {
  0x09, 0x00, 0x63, 0x6E, 0x74, 0x6B, 0x00, 0xE0, 0x55, 0x55, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
static const uint8_t pad_2032E[6] = {0};
static const int32_t data_20334[3] = {61, 5, 589824};
static const uint32_t data_20340[5] = {0x706E6E63U, 0x5555E000U, 0, 0, 0};
static const int32_t data_20354[3] = {60, 1, 589824};
static const uint32_t data_20360[5] = {0x706D6E63U, 0x5555E000U, 0, 0, 0};
static const int32_t data_20374[3] = {58, 1, 589824};
static const uint32_t data_20380[5] = {0x6B6D6E63U, 0x5555E000U, 0, 0, 0};
static const int32_t data_20394[3] = {59, 1, 589824};
static const uint32_t data_203A0[3] = {0x31636E6DU, 0x5555E000U, 0};
static const float data_203AC[2] = {0.05f, 0.0f};
static const int32_t data_203B4[3] = {9999, 5, 589824};
static const uint32_t data_203C0[3] = {0x32636E6DU, 0x5555E000U, 0};
static const float data_203CC[2] = {0.05f, 0.0f};
static const int32_t data_203D4[3] = {9999, 5, 589824};
static const uint32_t data_203E0[5] = {0x33636E6DU, 0x5555E000U, 0, 0, 0};
static const int32_t data_203F4[3] = {9999, 1, 589824};
static const uint32_t data_20400[3] = {0x34636E6DU, 0x5555E000U, 0};
static const float data_2040C[2] = {0.05f, 0.0f};
static const int32_t data_20414[3] = {9999, 5, 589824};
static const uint32_t data_20420[3] = {0x35636E6DU, 0x5555E000U, 0};
static const float data_2042C[2] = {0.05f, 0.0f};
static const int32_t data_20434[3] = {9999, 5, 589824};
static const uint32_t data_20440[3] = {0x36636E6DU, 0x5555E000U, 0};
static const float data_2044C[2] = {0.05f, 0.0f};
static const int32_t data_20454[3] = {9999, 5, 589824};
static const uint32_t data_20460[5] = {0x37636E6DU, 0x5555E000U, 0, 0, 0};
static const int32_t data_20474[3] = {9999, 1, 589824};
static const uint32_t data_20480[3] = {0x38636E6DU, 0x5555E000U, 0};
static const float data_2048C[2] = {0.05f, 0.0f};
static const int32_t data_20494[3] = {9999, 5, 589824};
static const uint32_t data_204A0[5] = {0x39636E6DU, 0x5555E000U, 0, 0, 0};
static const int32_t data_204B4[3] = {9999, 1, 589824};
static const uint32_t data_204C0[3] = {0x61636E6DU, 0x5555E000U, 0};
static const float data_204CC[2] = {0.05f, 0.0f};
static const int32_t data_204D4[3] = {9999, 5, 589824};
static const uint32_t data_204E0[3] = {0x62636E6DU, 0x5555E000U, 0};
static const float data_204EC[2] = {0.05f, 0.0f};
static const int32_t data_204F4[3] = {9999, 5, 589824};
static const uint32_t data_20500[3] = {0x63636E6DU, 0x5555E000U, 0};
static const float data_2050C[2] = {0.05f, 0.0f};
static const int32_t data_20514[3] = {9999, 5, 589824};
static const uint32_t data_20520[5] = {0x64636E6DU, 0x5555E000U, 0, 0, 0};
static const int32_t data_20534[3] = {9999, 1, 589824};
static const uint32_t data_20540[3] = {0x65636E6DU, 0x5555E000U, 0};
static const float data_2054C[2] = {0.05f, 0.0f};
static const int32_t data_20554[3] = {9999, 5, 589824};
static const uint32_t data_20560[5] = {0x66636E6DU, 0x5555E000U, 0, 0, 0};
static const int32_t data_20574[3] = {9999, 1, 589824};
static const uint32_t data_20580[3] = {0x67636E6DU, 0x5555E000U, 0};
static const float data_2058C[2] = {0.05f, 0.0f};
static const int32_t data_20594[3] = {9999, 5, 589824};
static const uint32_t data_205A0[3] = {0x68636E6DU, 0x5555E000U, 0};
static const float data_205AC[2] = {0.05f, 0.0f};
static const int32_t data_205B4[3] = {9999, 5, 589824};
static const uint32_t data_205C0[3] = {0x69636E6DU, 0x5555E000U, 0};
static const float data_205CC[2] = {0.05f, 0.0f};
static const int32_t data_205D4[3] = {9999, 5, 589824};
static const uint32_t data_205E0[5] = {0x6A636E6DU, 0x5555E000U, 0, 0, 0};
static const int32_t data_205F4[3] = {9999, 1, 589824};
static const uint32_t data_20600[5] = {0x6B636E6DU, 0x5555E000U, 0, 0, 0};
static const int32_t data_20614[3] = {9999, 1, 589824};
static const uint32_t data_20620[5] = {0x31736E63U, 0x5555E000U, 0, 0, 0};
static const int32_t data_20634[3] = {65, 1, 589824};
static const uint32_t data_20640[5] = {0x63736E63U, 0x5555E000U, 0, 0, 0};
static const int32_t data_20654[3] = {63, 5, 589824};
static const uint32_t data_20660[5] = {0x6B736E63U, 0x5555E000U, 0, 0, 0};
static const int32_t data_20674[3] = {64, 5, 589824};
static const uint32_t data_20680[5] = {0x31626E63U, 0x55552000U, 0, 0, 0};
static const int32_t data_20694[3] = {52, 5, 589824};
static const uint32_t data_206A0[5] = {0x33626E63U, 0x55552000U, 0, 0, 0};
static const int32_t data_206B4[3] = {53, 5, 0};
static const shen_name data_206C0 = SHEN_NAME('GCH0');
static const float data_206C4[7] = {40160.0f, 0.0f, 0.76f, 0.055f, 0.0f, 0.0f, 0.0f};
static const shen_name data_206E0 = SHEN_NAME('GCH1');
static const float data_206E4[7] = {40160.0f, 0.0f, 0.76f, 0.055f, 0.0f, 0.0f, 0.0f};
static const shen_name data_20700 = SHEN_NAME('GCH3');
static const float data_20704[7] = {40160.0f, 0.0f, 0.76f, 0.055f, 0.0f, 0.0f, 0.0f};
static const shen_name data_20720 = SHEN_NAME('GCH2');
static const float data_20724[7] = {40160.0f, 0.0f, 0.76f, 0.055f, 0.0f, 0.0f, 0.0f};
static const shen_name data_20740 = SHEN_NAME('GCH3');
static const float data_20744[7] = {40160.0f, 0.0f, 0.76f, 0.055f, 0.0f, 0.0f, 0.0f};
static const shen_name data_20760 = SHEN_NAME('GCH4');
static const float data_20764[7] = {40160.0f, 0.0f, 0.76f, 0.055f, 0.0f, 0.0f, 0.0f};
static const shen_name data_20780 = SHEN_NAME('GCH5');
static const float data_20784[6] = {40160.0f, 0.0f, 0.76f, 0.055f, 0.0f, 0.0f};
static const int32_t data_2079C = 65535;
static uint8_t data_207A0[12] = {0};
static const int32_t data_207AC = 722089;
static const uint32_t data_207B0[29] = {
  0x70736E63U, 0x6F686E63U, 0x6B746E63U, 0x706D6E63U, 0x6B6D6E63U, 0x31626E63U, 0x33626E63U, 0x31636E6DU, 0x32636E6DU, 0x33636E6DU, 0x34636E6DU,
  0x35636E6DU, 0x36636E6DU, 0x37636E6DU, 0x38636E6DU, 0x39636E6DU, 0x61636E6DU, 0x62636E6DU, 0x63636E6DU, 0x64636E6DU, 0x65636E6DU, 0x66636E6DU,
  0x67636E6DU, 0x68636E6DU, 0x69636E6DU, 0x6A636E6DU, 0x6B636E6DU, 0x63736E63U, 0x6B736E63U
};
static const int32_t data_20824[5] = {-1, 787625, 0, 0, 0};
static const uint32_t data_20838[2] = {0x31736E63U, 0x706E6E63U};
static const int32_t data_20840[2] = {-1, -1};
static uint32_t data_20848 = 0x6B746E63;
typedef struct Row_2084C { int32_t w0; uint32_t w1; int32_t w2; uint32_t w3; } Row_2084C;
static const Row_2084C data_2084C[14] = {
  {180, 0x706E6E63U, 300, 0x706D6E63U},
  {500, 0x6B6D6E63U, 100, 0x31626E63U},
  {150, 0x33626E63U, 150, 0x31636E6DU},
  {300, 0x32636E6DU, 300, 0x33636E6DU},
  {300, 0x34636E6DU, 300, 0x35636E6DU},
  {300, 0x36636E6DU, 300, 0x37636E6DU},
  {300, 0x38636E6DU, 300, 0x39636E6DU},
  {300, 0x61636E6DU, 300, 0x62636E6DU},
  {300, 0x63636E6DU, 300, 0x64636E6DU},
  {300, 0x65636E6DU, 300, 0x66636E6DU},
  {300, 0x67636E6DU, 300, 0x68636E6DU},
  {300, 0x69636E6DU, 300, 0x6A636E6DU},
  {300, 0x6B636E6DU, 300, 0x31736E63U},
  {120, 0x63736E63U, 120, 0x6B736E63U}
};
static const int32_t data_2092C[2] = {120, -1};
static const int32_t data_20934[57] = {
  0, 0, -6005, 0, 4731, -6188, 0, 0, 0, 0, 0, -2727, 0, 0, -9097, 0, 0, -5457, 0, 0, 0, 0, 0, -8734, 0, -909, -7277, 0, 0, -7823, 0, 0, 0, 0, 1456,
  -4002, 0, 0, -9462, 0, 726, -7824, 0, 0, 0, 0, 0, -12011, 0, -1091, -7461, 0, -1636, -9280, 0, 0, 0
};
static const int32_t data_20A18[114] = {
  0, 0, -6003, 0, 0, -908, 0, 0, 0, 0, 0, -3819, 0, 0, -7094, 0, 0, -4547, 0, 0, 0, 0, 0, -2909, 0, 0, -8550, 0, -544, -4001, 0, 0, 0, 0, 0, -4363, 0,
  0, -8004, 0, -908, -2180, 0, 0, 0, 0, -1634, -6366, 0, -2000, -4365, 0, -1818, -2182, 0, 0, 0, 0, 0, -10922, 0, 0, -10558, 0, 1456, 0, 0, 0, -18022,
  0, 0, -17112, 0, 0, -18386, 0, 0, 0, 0, 0, -12196, 0, 0, -18204, 0, 0, -16748, 0, 0, 0, 0, 0, -11832, 0, 0, -18386, 0, 0, -15109, 0, 0, 0, 0, 819,
  -12014, 0, -1274, -16748, 0, -3458, -16201, 0, 0, 0
};
static const int32_t data_20BE0[57] = {
  0, 0, -3093, 0, 0, -1455, 0, 0, 0, 0, 0, -3275, 0, 0, -2001, 0, 0, -1637, 0, 0, 0, 0, 0, -12193, 0, 0, -12921, 0, 0, -4185, 0, 0, -363, 0, 0,
  -14195, 0, 0, -10919, 0, 0, -4550, 0, 0, 181, 0, 0, -15651, 0, 0, -10373, 0, 0, -6005, 0, -909, 181
};
static const Vector3i data_20CC4 = {.x = -4096, .y = 0, .z = 0};
static const Vector3i data_20CD0 = {.x = -11832, .y = 0, .z = 0};
static const char data_20CDC[11] __attribute__((scn_string_literal)) = "SA1114A007";
static const char data_20CE7[11] __attribute__((scn_string_literal)) = "SA1114A008";
static const char data_20CF2[11] __attribute__((scn_string_literal)) = "SA1114A009";
static const char data_20CFD[11] __attribute__((scn_string_literal)) = "SA1114B001";
static const char data_20D08[11] __attribute__((scn_string_literal)) = "SA1114B004";
static const char data_20D13[11] __attribute__((scn_string_literal)) = "SA1114B002";
static const char data_20D1E[38] = "SA1114B003\000\000\000\000\000\340\377\377\000\000\000\000\000\000\000\000\310\321\377\377";
static const char data_20D44[11] __attribute__((scn_string_literal)) = "SA1114B006";
static const char data_20D4F[11] __attribute__((scn_string_literal)) = "SA1114A010";
static const char data_20D5A[14] __attribute__((scn_string_literal)) = "SA1114A011";
static const Vector3 data_20D68 = {.x = -77.559601f, .y = 0.0f, .z = 51.088902f};
static const Vector3i data_20D74 = {.x = -4096, .y = 0, .z = 0};
static const Vector3i data_20D80 = {.x = -11832, .y = 0, .z = 0};
static const char data_20D8C[11] __attribute__((scn_string_literal)) = "SA1114B001";
static const char data_20D97[11] __attribute__((scn_string_literal)) = "SA1114A007";
static const char data_20DA2[11] __attribute__((scn_string_literal)) = "SA1114A009";
static const char data_20DAD[11] __attribute__((scn_string_literal)) = "SA1114B004";
static const char data_20DB8[11] __attribute__((scn_string_literal)) = "SA1114B009";
static const char data_20DC3[11] __attribute__((scn_string_literal)) = "SA1114B010";
static const char data_20DCE[11] __attribute__((scn_string_literal)) = "SA1114B011";
static const char data_20DD9[11] __attribute__((scn_string_literal)) = "SA1114B012";
static const char data_20DE4[11] __attribute__((scn_string_literal)) = "SA1114B013";
static const char data_20DEF[11] __attribute__((scn_string_literal)) = "SA1114A014";
static const char data_20DFA[11] __attribute__((scn_string_literal)) = "SA1114A012";
static const char data_20E05[11] __attribute__((scn_string_literal)) = "SA1114A013";
static const char data_20E10[11] __attribute__((scn_string_literal)) = "SA1114B005";
static const char data_20E1B[11] __attribute__((scn_string_literal)) = "SA1114B007";
static const char data_20E26[11] __attribute__((scn_string_literal)) = "SA1114B008";
static const char data_20E31[19] __attribute__((scn_string_literal)) = "[[[UISRCHMKYU22]]]";
static const char data_20E44[19] __attribute__((scn_string_literal)) = "[[[UISRCHMKYU21]]]";
static const char data_20E57[19] __attribute__((scn_string_literal)) = "[[[UISRCHMKYU20]]]";
static const char data_20E6A[19] __attribute__((scn_string_literal)) = "[[[UISRCHMKYU19]]]";
static const char data_20E7D[19] __attribute__((scn_string_literal)) = "[[[UISRCHMKYU17]]]";
static const char data_20E90[19] __attribute__((scn_string_literal)) = "[[[UISRCHMKYU18]]]";
static const char data_20EA3[19] __attribute__((scn_string_literal)) = "[[[UISRCHMKYU23]]]";
static const char data_20EB6[19] __attribute__((scn_string_literal)) = "[[[UISRCHMKYU24]]]";
static const char data_20EC9[19] __attribute__((scn_string_literal)) = "[[[UISRCHMKYU25]]]";
static const char data_20EDC[19] __attribute__((scn_string_literal)) = "[[[UISRCHMKYU26]]]";
static const char data_20EEF[19] __attribute__((scn_string_literal)) = "[[[UISRCHMKYU27]]]";
static const char data_20F02[19] __attribute__((scn_string_literal)) = "[[[UISRCHMKYU28]]]";
static const char data_20F15[19] __attribute__((scn_string_literal)) = "[[[UISRCHMKYU29]]]";
static const char data_20F28[19] __attribute__((scn_string_literal)) = "[[[UISRCHMKYU30]]]";
static const char data_20F3B[19] __attribute__((scn_string_literal)) = "[[[UISRCHMKYU31]]]";
static const char data_20F4E[19] __attribute__((scn_string_literal)) = "[[[UISRCHMKYU32]]]";
static const char data_20F61[19] __attribute__((scn_string_literal)) = "[[[UISRCHMKYU33]]]";
static const char data_20F74[19] __attribute__((scn_string_literal)) = "[[[UISRCHMKYU34]]]";
static const char data_20F87[19] __attribute__((scn_string_literal)) = "[[[UISRCHMKYU35]]]";
static const char data_20F9A[19] __attribute__((scn_string_literal)) = "[[[UISRCHMKYU36]]]";
static const char data_20FAD[19] __attribute__((scn_string_literal)) = "[[[UISRCHMKYU37]]]";
static const char data_20FC0[19] __attribute__((scn_string_literal)) = "[[[UISRCHMKYU38]]]";
static const char data_20FD3[19] __attribute__((scn_string_literal)) = "[[[UISRCHMKYU39]]]";
static const char data_20FE6[19] __attribute__((scn_string_literal)) = "[[[UISRCHMKYU40]]]";
static const char data_20FF9[19] __attribute__((scn_string_literal)) = "[[[UISRCHMKYU41]]]";
static const char data_2100C[19] __attribute__((scn_string_literal)) = "[[[UISRCHMKYU42]]]";
static const char data_2101F[19] __attribute__((scn_string_literal)) = "[[[UISRCHMKYU14]]]";
static const char data_21032[19] __attribute__((scn_string_literal)) = "[[[UISRCHMKYU16]]]";
static const char data_21045[19] __attribute__((scn_string_literal)) = "[[[UISRCHMKYU15]]]";
static const char data_21058[11] __attribute__((scn_string_literal)) = "SA1114A003";
static const char data_21063[11] __attribute__((scn_string_literal)) = "SA1114A004";
static const char data_2106E[11] __attribute__((scn_string_literal)) = "SA1114A005";
static const char data_21079[11] __attribute__((scn_string_literal)) = "SA1114A006";
static const char data_21084[11] __attribute__((scn_string_literal)) = "SA1114A001";
static const char data_2108F[13] __attribute__((scn_string_literal)) = "SA1114A002";
static const uint32_t data_2109C = 0xC2970000;
static const float data_210A0[2] = {0.0f, 54.1f};
static const uint32_t data_210A8 = 0xC29F999A;
static const float data_210AC[2] = {0.0f, 58.4f};
static const uint32_t data_210B4 = 0xC2A1999A;
static const float data_210B8[2] = {0.0f, 59.4f};
static const uint32_t data_210C0 = 0xC2A80000;
static const float data_210C4[2] = {0.0f, 55.0f};
static const uint32_t data_210CC = 0x3F800000;
static const float data_210D0[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint32_t data_21154 = 0x3F800000;
static const float data_21158[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint32_t data_211DC = 0x3F800000;
static const float data_211E0[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const uint32_t data_21264 = 0x3F800000;
static const float data_21268[33] = {
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
static const char data_212EC[6] __attribute__((scn_string_literal)) = "MPK00";
static const char data_212F2[13] __attribute__((scn_string_literal)) = "HMOT_KYU.BIN";
static const char data_212FF[13] __attribute__((scn_string_literal)) = "HMOT_JBX.BIN";
static const char data_2130C[7] __attribute__((scn_string_literal)) = "SA1114";
static const char data_21313[11] __attribute__((scn_string_literal)) = "M_MOBJ.BIN";
static const char data_2131E[14] __attribute__((scn_string_literal)) = "scene/02/MFSY";
static const char data_2132C[] __attribute__((scn_string_literal)) = "CYCLEMAN.BIN";
static const uint8_t data_21339[3] = {0};
static const Vector3 data_2133C = {.x = -81.97f, .y = 0.0f, .z = 61.54f};
static const Vector3 data_21348 = {.x = -83.99f, .y = 0.0f, .z = 54.97f};
static const Vector3 data_21354 = {.x = -84.356201f, .y = 1.0f, .z = 54.992802f};
static const Vector3 data_21360 = {.x = 0.005f, .y = 0.0f, .z = 0.005f};
static const Vector3 data_2136C = {.x = 0.0f, .y = 0.0f, .z = 0.0f};
static const Vector3 data_21378 = {.x = -75.551598f, .y = 0.0f, .z = 54.132999f};
static const Vector3 data_21384 = {.x = -79.877296f, .y = 0.0f, .z = 58.478699f};
static const Vector3 data_21390 = {.x = -80.867203f, .y = 0.0f, .z = 59.4687f};
static const Vector3 data_2139C = {.x = -81.049103f, .y = 0.0f, .z = 44.6656f};
static const Vector3 data_213A8 = {.x = -80.3591f, .y = 0.0f, .z = 45.355598f};
typedef struct Row_213B4 { uint32_t w0; float w1; uint32_t w2; uint32_t w3; uint32_t w4; } Row_213B4;
static const Row_213B4 data_213B4[2] = {
  {0xC29E0595U, 0.0f, 0x423BD3A9U, 0xC29D2C57U, 0x0U},
  {0x423D8625U, -78.1623f, 0x0U, 0x423F3886U, 0xC29FC1CBU}
};
static const uint32_t data_213DC[2] = {0, 0x42394B92U};
static const char data_213E4[] __attribute__((scn_string_literal)) = "/scene/02/MKYU/";
static const char data_213F4[] __attribute__((scn_string_literal)) = "MKY0";
static const char data_213F9[] __attribute__((scn_string_literal)) = "/scene/02/MKYU/";
static const char data_21409[] __attribute__((scn_string_literal)) = "MKY1";
static const char data_2140E[] __attribute__((scn_string_literal)) = "/scene/02/MKYU/";
static const char data_2141E[] __attribute__((scn_string_literal)) = "MKY2";
static const char data_21423[] __attribute__((scn_string_literal)) = "/scene/02/MKYU/";
static const char data_21433[] __attribute__((scn_string_literal)) = "MKY3";
static const char data_21438[] __attribute__((scn_string_literal)) = "/scene/02/MKYU/";
static const char data_21448[] __attribute__((scn_string_literal)) = "MKY4";
static const uint8_t data_2144D[3] = {0};

static int32_t script_0030(int32_t arg0) {
  if (arg0 < 0) {
    return -arg0;
  } else {
    return arg0;
  }
}

static void script_004E(float arg0) {
  if (!(arg0 >= 0.0f)) {
    (void)((float)0 - arg0);
    return;
  } else {
    (void)arg0;
    return;
  }
}

static int32_t script_006D(int32_t arg0, int32_t arg1) {
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

static void script_00E5(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  int32_t local_FC;
  if (task_160 != arg0) {
    Script_sprintf("Door Index = %d : call_type = %d : type = %d\012", task_48[arg2], arg3, arg4);
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

static int32_t script_0697(int32_t arg0) {
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

static void script_0768(int32_t arg0, int32_t arg1, int32_t arg2) {
  script_948B(arg0, arg2, 0);
  return;
}

static void script_0778(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  float local_F8;
  float local_F4;
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
    local_FC = script_8C20(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
    if ((local_FC == 1) | (local_FC == 2)) {
      local_FC = 1;
    } else {
      local_FC = 0;
    }
    *(int32_t *)&local_F8 = Script_ScenDataUtil(data_1C60C, 5, Script_ScenDataUtil(data_1C01C, 1, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 5);
    *(int32_t *)&local_F4 = Script_ScenDataUtil(data_1C60C, 5, Script_ScenDataUtil(data_1C01C, 1, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 6);
    local_FC = script_8C20(arg0, arg1, 5, local_F8 / 100.0f, local_F4 / 100.0f, arg3, -1);
    script_3F04(arg0, arg1, arg2, arg3, local_FC, 0);
  }
  script_948B(arg0, arg2, 0);
  return;
}

static void script_087C(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  int32_t local_F8;
  float local_F4;
  float local_F0;
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
    local_FC = script_8C20(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
    *(int32_t *)&local_F4 = Script_ScenDataUtil(data_1C60C, 5, Script_ScenDataUtil(data_1C01C, 1, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 5);
    local_F4 = 90.0f - local_F4;
    *(int32_t *)&local_F0 = Script_ScenDataUtil(data_1C60C, 5, Script_ScenDataUtil(data_1C01C, 1, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 6);
    if ((Script_ScenDataUtil(task_0, 4, task_48[arg2] * 13 + 1) & 0x40) | (Script_ScenDataUtil(task_0, 4, task_48[arg2] * 13 + 1) & 0x80)) {
      if (Script_ScenDataUtil(task_0, 4, task_48[arg2] * 13 + 1) & 0x40) {
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
    local_FC = script_8C20(arg0, arg1, local_F8, local_F4 / (float)100, local_F0 / (float)100, arg3, task_48[arg2]);
    script_3F04(arg0, arg1, arg2, arg3, local_FC, 1);
  }
  script_948B(arg0, arg2, 0);
  return;
}

static void script_0A1E(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  float local_F8;
  float local_F4;
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
    local_FC = script_8C20(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
    if ((local_FC == 1) | (local_FC == 2)) {
      local_FC = 1;
    } else {
      local_FC = 0;
    }
    *(int32_t *)&local_F8 = Script_ScenDataUtil(data_1C60C, 5, Script_ScenDataUtil(data_1C01C, 1, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 5);
    *(int32_t *)&local_F4 = Script_ScenDataUtil(data_1C60C, 5, Script_ScenDataUtil(data_1C01C, 1, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 6);
    local_FC = script_8C20(arg0, arg1, 5, local_F8 / 100.0f, local_F4 / 100.0f, arg3, -1);
    if (local_FC == 1) {
      local_FC = 2;
    } else {
      local_FC = 3;
    }
    if ((Script_ScenDataUtil(task_0, 4, task_48[arg2] * 13 + 1) & 4) == 0) {
      script_2411(arg0, arg1, arg2, arg3, local_FC, 0);
    } else {
      if (arg0 == task_160) {
        script_57E4(arg0, arg1, arg2, arg3, local_FC, 0);
      }
    }
  }
  script_948B(arg0, arg2, 2);
  return;
}

static void script_0B7F(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  float local_F8;
  float local_F4;
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
    local_FC = script_8C20(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
    if ((local_FC == 1) | (local_FC == 2)) {
      local_FC = 1;
    } else {
      local_FC = 0;
    }
    *(int32_t *)&local_F8 = Script_ScenDataUtil(data_1C60C, 5, Script_ScenDataUtil(data_1C01C, 1, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 5);
    *(int32_t *)&local_F4 = Script_ScenDataUtil(data_1C60C, 5, Script_ScenDataUtil(data_1C01C, 1, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 6);
    local_FC = script_8C20(arg0, arg1, 5, local_F8 / 100.0f, local_F4 / 100.0f, arg3, -1);
    if (local_FC == 1) {
      local_FC = 1;
    } else {
      local_FC = 0;
    }
    if ((Script_ScenDataUtil(task_0, 4, task_48[arg2] * 13 + 1) & 4) == 0) {
      script_2411(arg0, arg1, arg2, arg3, local_FC, 0);
    } else {
      if (arg0 == task_160) {
        script_57E4(arg0, arg1, arg2, arg3, local_FC, 0);
      }
    }
  }
  script_948B(arg0, arg2, 2);
  return;
}

static void script_0CE0(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  int32_t local_F8;
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
    if ((Script_ScenDataUtil(task_0, 4, task_48[arg2] * 13 + 1) & 4) == 0) {
      float local_F4;
      float local_F0;
      local_FC = script_8C20(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
      *(int32_t *)&local_F4 = Script_ScenDataUtil(data_1C60C, 5, Script_ScenDataUtil(data_1C01C, 1, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 5);
      *(int32_t *)&local_F0 = Script_ScenDataUtil(data_1C60C, 5, Script_ScenDataUtil(data_1C01C, 1, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 6);
      if ((Script_ScenDataUtil(task_0, 4, task_48[arg2] * 13 + 1) & 0x40) | (Script_ScenDataUtil(task_0, 4, task_48[arg2] * 13 + 1) & 0x80)) {
        if (Script_ScenDataUtil(task_0, 4, task_48[arg2] * 13 + 1) & 0x40) {
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
      local_FC = script_8C20(arg0, arg1, local_F8, local_F4 / 100.0f, local_F0 / 100.0f, arg3, task_48[arg2]);
      script_2411(arg0, arg1, arg2, arg3, local_FC, 1);
    } else {
      if (arg0 == task_160) {
        float local_F4;
        float local_F0;
        local_FC = script_8C20(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
        *(int32_t *)&local_F4 = Script_ScenDataUtil(data_1C60C, 5, Script_ScenDataUtil(data_1C01C, 1, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 5);
        *(int32_t *)&local_F0 = Script_ScenDataUtil(data_1C60C, 5, Script_ScenDataUtil(data_1C01C, 1, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 6);
        local_FC = script_8C20(arg0, arg1, 6, local_F4 / 100.0f, local_F0 / 100.0f, arg3, task_48[arg2]);
        script_57E4(arg0, arg1, arg2, arg3, local_FC, 1);
      }
    }
  }
  script_948B(arg0, arg2, 2);
  return;
}

static void script_0F7E(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  float local_F8;
  float local_F4;
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
    local_FC = script_8C20(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
    *(int32_t *)&local_F8 = Script_ScenDataUtil(data_1C60C, 5, Script_ScenDataUtil(data_1C01C, 1, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 5);
    *(int32_t *)&local_F4 = Script_ScenDataUtil(data_1C60C, 5, Script_ScenDataUtil(data_1C01C, 1, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 6);
    script_8C20(arg0, arg1, 0, local_F8 / 100.0f, local_F4 / 100.0f, arg3, -1);
  }
  script_948B(arg0, arg2, 0);
  return;
}

static void script_104C(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
    local_FC = script_8C20(arg0, arg1, 4, 0.5f, 0.5f, arg3, -1);
    if (local_FC == 1) {
      local_FC = 2;
    } else {
      local_FC = 3;
    }
    script_3390(arg0, arg1, arg2, arg3, local_FC, 1);
  }
  script_948B(arg0, arg2, 1);
  return;
}

static void script_10B3(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
    local_FC = script_8C20(arg0, arg1, 4, 0.5f, 0.5f, arg3, -1);
    if (local_FC == 1) {
      local_FC = 1;
    } else {
      local_FC = 0;
    }
    script_3390(arg0, arg1, arg2, arg3, local_FC, 1);
  }
  script_948B(arg0, arg2, 1);
  return;
}

static void script_111A(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
    script_8C20(arg0, arg1, 4, 0.0f, 0.5f, arg3, -1);
    script_3390(arg0, arg1, arg2, arg3, 4, 1);
  }
  script_948B(arg0, arg2, 1);
  return;
}

static void script_1161(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  float local_F8;
  float local_F4;
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
    local_FC = script_8C20(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
    if ((local_FC == 1) | (local_FC == 2)) {
      local_FC = 1;
    } else {
      local_FC = 0;
    }
    *(int32_t *)&local_F8 = Script_ScenDataUtil(data_1C60C, 5, Script_ScenDataUtil(data_1C01C, 1, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 5);
    *(int32_t *)&local_F4 = Script_ScenDataUtil(data_1C60C, 5, Script_ScenDataUtil(data_1C01C, 1, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 6);
    if (local_FC == 0) {
      local_FC = script_8C20(arg0, arg1, 5, local_F8 / 100.0f, local_F4 / 100.0f, arg3, -1);
    } else {
      local_FC = script_8C20(arg0, arg1, 5, local_F8 / 100.0f, local_F4 / 100.0f, arg3, -1);
    }
    script_384A(arg0, arg1, arg2, arg3, local_FC, 0);
  }
  script_948B(arg0, arg2, 2);
  return;
}

static void script_1297(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
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
  if (arg0 == task_160) {
    if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
      local_FF7F = script_8C20(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
      *(int32_t *)&local_FF78 = Script_ScenDataUtil(data_1C60C, 5, Script_ScenDataUtil(data_1C01C, 1, arg3 * 40 + task_98[arg2] * 4 + local_FF7F) * 9 + 5);
      *(int32_t *)&local_FF74 = Script_ScenDataUtil(data_1C60C, 5, Script_ScenDataUtil(data_1C01C, 1, arg3 * 40 + task_98[arg2] * 4 + local_FF7F) * 9 + 6);
      local_FF7F = script_8C20(arg0, arg1, 5, local_FF78 / (float)100, local_FF74 / (float)100, arg3, -1);
      script_92E9(arg0, 0);
      *(int32_t *)&local_94 = Script_ScenDataUtil(data_1C60C, 5, Script_ScenDataUtil(data_1C01C, 1, arg3 * 40 + task_98[arg2] * 4 + local_FF7F) * 9 + 1);
      *(int32_t *)&local_90 = Script_ScenDataUtil(data_1C60C, 5, Script_ScenDataUtil(data_1C01C, 1, arg3 * 40 + task_98[arg2] * 4 + local_FF7F) * 9 + 2);
      local_8C = 0.0f;
      local_FC = Script_ScenDataUtil(data_1C60C, 4, Script_ScenDataUtil(data_1C01C, 1, arg3 * 40 + task_98[arg2] * 4 + local_FF7F) * 9);
      local_FC = local_FC | ((Script_FloatUtils(ScriptMathOp_FloatToInt, local_94 - 3.0f) << 16) & 0x7FFF0000);
      local_9C.x = (&task_168)->x;
      (&local_9C)->y = task_168.y;
      (&local_9C)->z = task_168.z;
      Script_charaPosRotUtils(arg1, 12, &local_CC, &local_E4, 2);
      Script_charaPosRotUtils(arg1, 7, &local_C0, &local_D8, 2);
      Script_ryoControl(PCCTL_StartCameraFacingInterpolation, 0);
      if (arg0 == task_160) {
        Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
        Script_scenCharaSubmitMoveTargetRequest(arg0, &task_168, *(&task_174 + 1), local_FC, 0);
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
        Script_SetCharaMoveTarget(arg0, &task_168, *(&task_174 + 1), local_FC);
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
              local_FF6C = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_48[arg2], 2);
              local_FF6C = Script_ScenDataUtil(task_8, 4, Script_ScenDataUtil(task_C, 4, local_FF6C * 9 + 1) * 4 + 2);
              if (local_FF6C != -1) {
                if (arg0 == task_160) {
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
              local_FF6C = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_48[arg2], 2);
              local_FF6C = Script_ScenDataUtil(task_8, 4, Script_ScenDataUtil(task_C, 4, local_FF6C * 9 + 1) * 4 + 2);
              if (local_FF6C != -1) {
                if (arg0 == task_160) {
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
      script_92E9(arg0, 1);
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
        local_FF6C = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_48[arg2], 2);
        local_FF6C = Script_ScenDataUtil(task_8, 4, Script_ScenDataUtil(task_C, 4, local_FF6C * 9 + 1) * 4 + 3);
        if (local_FF6C != -1) {
          if (arg0 == task_160) {
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
    script_948B(arg0, arg2, 0);
  } else {
    script_087C(arg0, arg1, arg2, arg3);
  }
  return;
}

static void script_1B58(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  float local_F8;
  int32_t local_F4;
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
    local_FC = script_8C20(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
    *(int32_t *)&local_F8 = Script_ScenDataUtil(data_1C60C, 5, Script_ScenDataUtil(data_1C01C, 1, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 5);
    local_F8 = 98.0f - local_F8;
    local_F4 = Script_ScenDataUtil(data_1C60C, 5, Script_ScenDataUtil(data_1C01C, 1, arg3 * 40 + task_98[arg2] * 4 + local_FC) * 9 + 6);
    if ((local_FC == 0) | (local_FC == 3)) {
      local_FC = script_8C20(arg0, arg1, 4, 0.34736f, 0.73184f, arg3, -1);
    } else {
      local_FC = script_8C20(arg0, arg1, 4, 0.4f, 0.712f, arg3, -1);
    }
    script_5F05(arg0, arg1, arg2, arg3, local_FC, 0);
  }
  script_948B(arg0, arg2, 0);
  return;
}

static int32_t script_1C76(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
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
    local_F4 = Script_ScenDataUtil(task_0, 4, task_48[arg2] * 13 + 6);
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
        local_FC = script_8C20(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
        local_FC = script_8C20(arg0, arg1, 7, 0.45f, 0.0f, arg3, -1);
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
              goto L001E32;
            }
          }
          L001E32:;
        } else {
          local_D4 = 0.45f;
        }
        Script_EVT_GetPosition(arg1, -1, &local_E4, 0);
        Script_mtwkGetSetRotation(arg1, &local_D8, 0);
        local_E4.x = (&local_E4)->x + local_D4 * Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_D8)->y & 0xFFFF);
        (&local_E4)->z = (&local_E4)->z - local_D4 * Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_D8)->y & 0xFFFF);
        *(&task_174 + 1) = Script_FloatUtils(ScriptMathOp_Atan2ReverseVector, (&local_E4)->x - (&task_168)->x, (&local_E4)->z - task_168.z);
        script_668F(arg0, arg1, arg2, arg3, local_FC, local_F0);
      }
    } else if ((local_BC == 3) | (local_BC == 4) | (local_BC == 5) | (local_BC == 6)) {
      *(int32_t *)&local_D0 = Script_ScenDataUtil(&data_1D02C, 5, Script_ScenDataUtil(task_0, 4, task_48[arg2] * 13 + 6) * 5 + 3);
      *(int32_t *)&local_CC = Script_ScenDataUtil(&data_1D02C, 5, Script_ScenDataUtil(task_0, 4, task_48[arg2] * 13 + 6) * 5 + 4);
      local_FC = script_8C20(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
      local_BC = arg3;
      if ((local_BC == 13) | (local_BC == 14) | (local_BC == 15) | (local_BC == 16) | (local_BC == 20) | (local_BC == 23) | (local_BC == 28) | (local_BC == 29)) {
        if ((Script_ScenDataUtil(task_0, 4, task_48[arg2] * 13 + 1) & 0x40) | (Script_ScenDataUtil(task_0, 4, task_48[arg2] * 13 + 1) & 0x80)) {
          if (Script_ScenDataUtil(task_0, 4, task_48[arg2] * 13 + 1) & 0x40) {
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
      local_FC = script_8C20(arg0, arg1, local_F8, local_D0 / (float)100, local_CC / (float)100, arg3, task_48[arg2]);
      script_6767(arg0, arg1, arg2, arg3, local_FC, 0);
    } else {
      if (local_BC == 7) {
        local_FC = script_8C20(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
        local_BC = arg3;
        if ((local_BC == 13) | (local_BC == 14) | (local_BC == 15) | (local_BC == 16) | (local_BC == 20) | (local_BC == 23) | (local_BC == 28) | (local_BC == 29)) {
          if ((Script_ScenDataUtil(task_0, 4, task_48[arg2] * 13 + 1) & 0x40) | (Script_ScenDataUtil(task_0, 4, task_48[arg2] * 13 + 1) & 0x80)) {
            if (Script_ScenDataUtil(task_0, 4, task_48[arg2] * 13 + 1) & 0x40) {
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
        if (Script_CycleManagerUtils(CYCLE_CMD_IS_DOOR_OR_MOBJ_TYPE2, Script_ScenDataUtil(task_0, 4, task_48[arg2] * 13 + 12)) == 1) {
          *(int32_t *)&local_D0 = Script_ScenDataUtil(&data_1D02C, 5, Script_ScenDataUtil(task_0, 4, task_48[arg2] * 13 + 6) * 5 + 3);
          *(int32_t *)&local_CC = Script_ScenDataUtil(&data_1D02C, 5, Script_ScenDataUtil(task_0, 4, task_48[arg2] * 13 + 6) * 5 + 4);
          local_FC = script_8C20(arg0, arg1, local_F8, local_D0 / (float)100, local_CC / (float)100, arg3, task_48[arg2]);
          Script_scenUtils(SCENARIO_CMD_INIT_SCNT);
          script_6BDE(arg0, arg1, arg2, arg3, local_FC, 0);
          Script_scenUtils(SCENARIO_CMD_UPDATE_TIMER);
        } else {
          *(int32_t *)&local_D0 = Script_ScenDataUtil(&data_1D02C, 5, Script_ScenDataUtil(task_0, 4, task_48[arg2] * 13 + 6) * 5 + 3);
          *(int32_t *)&local_CC = Script_ScenDataUtil(&data_1D02C, 5, Script_ScenDataUtil(task_0, 4, task_48[arg2] * 13 + 6) * 5 + 4);
          local_FC = script_8C20(arg0, arg1, local_F8, local_D0 / (float)100, local_CC / (float)100, arg3, task_48[arg2]);
          script_6767(arg0, arg1, arg2, arg3, local_FC, 0);
        }
        goto L00232D;
      }
    }
  }
  L00232D:;
  script_948B(arg0, arg2, 0);
  end:;
}

static void script_233B(int32_t arg0, int32_t arg1) {
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
    Script_SetCharaMoveTarget(arg0, &local_F4, (&local_E8)->y, script_0697(arg1));
  }
  return;
}

__attribute__((scn_string_bindings(0, data_1DB4A)))
static void script_2411(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
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
  *(int32_t *)&local_98 = Script_ScenDataUtil(data_1C60C, 5, Script_ScenDataUtil(data_1C01C, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 1);
  *(int32_t *)&local_94 = Script_ScenDataUtil(data_1C60C, 5, Script_ScenDataUtil(data_1C01C, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 2);
  *(int32_t *)&local_90 = Script_ScenDataUtil(data_1C60C, 5, Script_ScenDataUtil(data_1C01C, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 3);
  *(int32_t *)&local_8C = Script_ScenDataUtil(data_1C60C, 5, Script_ScenDataUtil(data_1C01C, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 4);
  *(int32_t *)&local_88 = Script_ScenDataUtil(data_1C60C, 5, Script_ScenDataUtil(data_1C01C, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 7);
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
      local_FF78 = Script_ScenDataUtil(data_1C60C, 4, Script_ScenDataUtil(data_1C01C, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
      local_FF7C = 18;
      local_80 = 18;
      local_A4 = 1.0f;
    } else {
      if (arg0 == task_160) {
        local_FF78 = Script_ScenDataUtil(data_1C60C, 4, Script_ScenDataUtil(data_1C01C, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9) | -0x80000000;
        local_FF7C = 12;
        local_80 = 12;
      } else {
        local_FF78 = Script_ScenDataUtil(data_1C60C, 4, Script_ScenDataUtil(data_1C01C, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
        local_FF7C = 18;
        local_80 = 18;
      }
      local_A4 = -1.0f;
    }
    script_00E5(arg0, local_FF78, arg2, arg3, arg4);
    local_FF78 = local_FF78 | ((Script_FloatUtils(ScriptMathOp_FloatToInt, local_98 - 3.0f) << 16) & 0x7FFF0000);
    script_92E9(arg0, 0);
    local_C0.x = (&task_168)->x;
    (&local_C0)->y = task_168.y;
    (&local_C0)->z = task_168.z;
    Script_ryoControl(PCCTL_StartCameraFacingInterpolation, 0);
    Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
    Script_SetCharaMoveTarget(arg0, &task_168, *(&task_174 + 1), local_FF78);
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
    if (task_160 == arg0) {
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
          script_233B(arg0, task_98[arg2]);
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
            local_FF64 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_48[arg2], 2);
            local_FF64 = Script_ScenDataUtil(task_8, 4, Script_ScenDataUtil(task_C, 4, local_FF64 * 9 + 1) * 4 + 2);
            if (local_FF64 != -1) {
              if (arg0 == task_160) {
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
                goto L002999;
              }
            }
            L002999:;
          } else {
            if ((local_FF64 == 1) | (local_FF64 == 3)) {
              if (!(local_B4 > 0.0f)) {
                local_B4 = 0.0f;
              } else {
                if (!(local_B4 < 0.75f)) {
                  local_B4 = 0.75f;
                  goto L0029D7;
                }
              }
              L0029D7:;
              goto L0029DA;
            }
          }
        }
      }
      L0029DA:;
      if (task_160 == arg0) {
        if (!(local_B8 <= local_94) & !(local_B8 >= local_90)) {
          local_86 = 0;
        }
      } else {
        if (!(local_B8 <= local_94) & !(local_B8 > local_88)) {
          *(int32_t *)&local_9C = Script_ScenDataUtil(data_1C60C, 5, Script_ScenDataUtil(data_1C01C, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 8);
          local_B4 = local_B4 + local_9C * local_A4;
        }
      }
      if (!(local_B8 < local_90) & !(local_B8 >= local_90 + 1.5f)) {
        if (local_84 == 1) {
          int32_t local_FF64;
          local_FF64 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_48[arg2], 2);
          local_FF64 = Script_ScenDataUtil(task_8, 4, Script_ScenDataUtil(task_C, 4, local_FF64 * 9 + 1) * 4 + 3);
          if (local_FF64 != -1) {
            if (arg0 == task_160) {
              Script_executeAicaCommand(local_FF64, 0, 0);
            } else {
              Script_Execute3DSoundCommand_Wrapper(local_FF64, 4, &local_C0, 5.0f, 20.0f, 0);
            }
          }
          local_84 = 2;
        }
        if (task_160 != arg0) {
          if (local_FF74 == 0) {
            local_AC = local_B4 / (local_8C - local_90);
            local_FF74 = 1;
          }
        }
      }
      if (!(local_B8 < local_90) & !(local_B8 > local_8C)) {
        Script_EVT_GetPosition(arg0, local_FF7C, &local_F0, ENTITY_XFORM_MOTION_SPACE);
        if (task_160 == arg0) {
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
                goto L002BA6;
              }
            }
            L002BA6:;
          } else {
            if ((local_FF64 == 1) | (local_FF64 == 3)) {
              if (!(local_B4 < 0.75f)) {
                local_B4 = 0.75f;
              } else {
                if (!(local_B4 > 0.0f)) {
                  local_B4 = 0.0f;
                  goto L002BE4;
                }
              }
              L002BE4:;
              goto L002BE7;
            }
          }
          L002BE7:;
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
              goto L002C40;
            }
          }
        }
      }
      L002C40:;
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
      if (arg0 == task_160) {
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
  script_92E9(arg0, 1);
  {
    int32_t local_FF78;
    int32_t unused_FF68[4];
    int32_t local_FF64;
    local_FF78 = task_48[arg2];
    local_FF64 = arg3;
    if ((local_FF64 == 9) | (local_FF64 == 10) | (local_FF64 == 15)) {
      if (arg5) {
        Script_charaPosRotUtils(arg1, 12, &local_D8, 0, 0);
        Script_charaPosRotUtils(arg1, 7, &local_CC, 0, 0);
      } else {
        Script_EVT_SetPosition(arg1, POS_XYZ, &local_D8);
      }
      if (task_160 == arg0) {
        local_80 = (Script_ScenDataUtil(task_0, 4, local_FF78 * 13 + 2) >> 16) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_80, 1);
        local_80 = Script_ScenDataUtil(task_0, 4, local_FF78 * 13 + 2) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_80, 1);
      }
    } else if ((local_FF64 == 18) | (local_FF64 == 19) | (local_FF64 == 21) | (local_FF64 == 22)) {
      if (task_160 == arg0) {
        local_80 = (Script_ScenDataUtil(task_0, 4, local_FF78 * 13 + 2) >> 16) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_80, 1);
        {
          int32_t local_FF60;
          local_FF60 = Script_ScenDataUtil(task_0, 4, local_FF78 * 13 + 1);
          local_FF60 = local_FF60 | 8;
          Script_ScenDataUtil(task_0, 14, local_FF78 * 13 + 1, local_FF60);
        }
        {
          int32_t local_FF60;
          local_FF60 = Script_ScenDataUtil(task_0, 4, local_FF78 * 13 + 1);
          local_FF60 = local_FF60 | 0x10;
          Script_ScenDataUtil(task_0, 14, local_FF78 * 13 + 1, local_FF60);
        }
        {
          int32_t local_FF60;
          local_FF60 = Script_ScenDataUtil(task_0, 4, local_FF78 * 13 + 1);
          local_FF60 = local_FF60 | 2;
          Script_ScenDataUtil(task_0, 14, local_FF78 * 13 + 1, local_FF60);
        }
      } else {
        if (arg5) {
          Script_charaPosRotUtils(arg1, 12, &local_D8, 0, 0);
          Script_charaPosRotUtils(arg1, 7, &local_CC, 0, 0);
        } else {
          Script_EVT_SetPosition(arg1, POS_XYZ, &local_D8);
        }
        local_80 = (Script_ScenDataUtil(task_0, 4, local_FF78 * 13 + 2) >> 16) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_80, 1);
        local_80 = Script_ScenDataUtil(task_0, 4, local_FF78 * 13 + 2) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_80, 1);
      }
    } else {
      if ((local_FF64 == 20) | (local_FF64 == 23)) {
        if (task_160 == arg0) {
          if ((arg4 == 0) | (arg4 == 1)) {
            {
              int32_t local_FF60;
              local_FF60 = Script_ScenDataUtil(task_0, 4, local_FF78 * 13 + 1);
              local_FF60 = local_FF60 | 0x10;
              Script_ScenDataUtil(task_0, 14, local_FF78 * 13 + 1, local_FF60);
            }
            {
              int32_t local_FF60;
              local_FF60 = Script_ScenDataUtil(task_0, 4, local_FF78 * 13 + 1);
              local_FF60 = local_FF60 | 4;
              Script_ScenDataUtil(task_0, 14, local_FF78 * 13 + 1, local_FF60);
            }
          } else {
            {
              int32_t local_FF60;
              local_FF60 = Script_ScenDataUtil(task_0, 4, local_FF78 * 13 + 1);
              local_FF60 = local_FF60 | 8;
              Script_ScenDataUtil(task_0, 14, local_FF78 * 13 + 1, local_FF60);
            }
            {
              int32_t local_FF60;
              local_FF60 = Script_ScenDataUtil(task_0, 4, local_FF78 * 13 + 1);
              local_FF60 = local_FF60 | 4;
              Script_ScenDataUtil(task_0, 14, local_FF78 * 13 + 1, local_FF60);
            }
          }
        } else {
          if (arg5) {
            Script_charaPosRotUtils(arg1, 12, &local_D8, 0, 0);
            Script_charaPosRotUtils(arg1, 7, &local_CC, 0, 0);
          } else {
            Script_EVT_SetPosition(arg1, POS_XYZ, &local_D8);
          }
          local_80 = (Script_ScenDataUtil(task_0, 4, local_FF78 * 13 + 2) >> 16) & 0xFFFF;
          Script_scenToggleCharaFlag(0, local_80, 1);
          local_80 = Script_ScenDataUtil(task_0, 4, local_FF78 * 13 + 2) & 0xFFFF;
          Script_scenToggleCharaFlag(0, local_80, 1);
        }
        goto L003323;
      }
    }
  }
  L003323:;
  return;
}

static void script_3324(int32_t arg0, int32_t arg1, int32_t arg2) {
  int32_t local_FC;
  if (task_160 == arg0) {
    local_FC = task_48[arg1];
    {
      int32_t local_F8;
      local_F8 = Script_ScenDataUtil(task_0, 4, local_FC * 13 + 2) & 0xFFFF;
      Script_scenToggleCharaFlag(0, local_F8, arg2);
    }
    {
      int32_t local_F8;
      local_F8 = (Script_ScenDataUtil(task_0, 4, local_FC * 13 + 2) >> 16) & 0xFFFF;
      Script_scenToggleCharaFlag(0, local_F8, arg2);
    }
  }
  return;
}

static void script_3390(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
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
  local_AC.x = (&task_168)->x;
  (&local_AC)->y = task_168.y;
  (&local_AC)->z = task_168.z;
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
    local_80 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_48[arg2], 2);
    local_80 = Script_ScenDataUtil(task_8, 4, Script_ScenDataUtil(task_C, 4, local_80 * 9 + 1) * 4 + 2);
    if (local_80 != -1) {
      if (arg0 == task_160) {
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
        task_70[arg2] = arg0;
        local_98 = -local_98;
        local_84 = 0;
      }
      local_8C = 0;
    } else {
      if ((local_93 == 1) & (local_8C == 0)) {
        int32_t local_80;
        local_80 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_48[arg2], 2);
        local_80 = Script_ScenDataUtil(task_8, 4, Script_ScenDataUtil(task_C, 4, local_80 * 9 + 1) * 4 + 3);
        if (local_80 != -1) {
          if (arg0 == task_160) {
            Script_executeAicaCommand(local_80, 0, 0);
          } else {
            Script_Execute3DSoundCommand_Wrapper(local_80, 4, &local_AC, 5.0f, 20.0f, 0);
          }
        }
        local_93 = 0;
        local_8C = 1;
        local_98 = -local_98;
        local_84 = 0;
        task_70[arg2] = 0;
      }
    }
    local_88 = task_48[arg2];
    local_88 = Script_ScenDataUtil(task_0, 4, local_88 * 13 + 2) & 0xFFFF;
    local_88 = local_88 + ((local_9C - 0x4000) >> 12);
    if (local_84 != local_88) {
      Script_scenToggleCharaFlag(0, local_88, local_8C);
      local_84 = local_88;
    }
    local_9C = local_9C - local_98;
    if (local_9C >= 0xC000) {
      local_9C = 0xC000;
      local_88 = task_48[arg2];
      local_88 = Script_ScenDataUtil(task_0, 4, local_88 * 13 + 2) & 0xFFFF;
      local_88 = local_88 + 8;
      Script_scenToggleCharaFlag(0, local_88, local_8C);
      break;
    }
    if (local_9C <= 0x4000) {
      local_9C = 0x4000;
      if (local_93 == 0) {
        task_70[arg2] = 0;
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
          goto L0037FA;
        }
      }
    }
    L0037FA:;
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

__attribute__((scn_string_bindings(0, data_1DB66)))
static void script_384A(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
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
          local_FC = Script_ScenDataUtil(data_1C60C, 4, Script_ScenDataUtil(data_1C01C, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
          break;
        case 0:
          local_B7 = 0;
          local_F8 = 12;
          local_F0 = 0x4000;
          local_B8 = 1.0f;
          local_FC = Script_ScenDataUtil(data_1C60C, 4, Script_ScenDataUtil(data_1C01C, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9) | -0x80000000;
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
          local_FC = Script_ScenDataUtil(data_1C60C, 4, Script_ScenDataUtil(data_1C01C, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9) | -0x80000000;
          break;
        case 0:
          local_B7 = 1;
          local_F8 = 18;
          local_F0 = 0xC000;
          local_B8 = 1.0f;
          local_FC = Script_ScenDataUtil(data_1C60C, 4, Script_ScenDataUtil(data_1C01C, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
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
          local_FC = Script_ScenDataUtil(data_1C60C, 4, Script_ScenDataUtil(data_1C01C, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9) | -0x80000000;
          break;
        case 0:
          local_B7 = 0;
          local_F8 = 18;
          local_F0 = 0xC000;
          local_B8 = -1.0f;
          local_FC = Script_ScenDataUtil(data_1C60C, 4, Script_ScenDataUtil(data_1C01C, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
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
          local_FC = Script_ScenDataUtil(data_1C60C, 4, Script_ScenDataUtil(data_1C01C, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
          break;
        case 0:
          local_B7 = 1;
          local_F8 = 12;
          local_F0 = 0x4000;
          local_B8 = -1.0f;
          local_FC = Script_ScenDataUtil(data_1C60C, 4, Script_ScenDataUtil(data_1C01C, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9) | -0x80000000;
          break;
      }
      break;
  }
  *(int32_t *)&local_C4 = Script_ScenDataUtil(data_1C60C, 5, Script_ScenDataUtil(data_1C01C, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 1);
  *(int32_t *)&local_C0 = Script_ScenDataUtil(data_1C60C, 5, Script_ScenDataUtil(data_1C01C, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 2);
  local_FC = local_FC | ((Script_FloatUtils(ScriptMathOp_FloatToInt, local_C4 - 3.0f) << 16) & 0x7FFF0000);
  local_BC = 0.0f;
  script_92E9(arg0, 0);
  local_CC.x = (&task_168)->x;
  (&local_CC)->y = task_168.y;
  (&local_CC)->z = task_168.z;
  Script_ryoControl(PCCTL_StartCameraFacingInterpolation, 0);
  Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
  Script_SetCharaMoveTarget(arg0, &task_168, *(&task_174 + 1), local_FC);
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
        local_A0 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_48[arg2], 2);
        local_A0 = Script_ScenDataUtil(task_8, 4, Script_ScenDataUtil(task_C, 4, local_A0 * 9 + 1) * 4 + 2);
        if (local_A0 != -1) {
          if (arg0 == task_160) {
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
  script_92E9(arg0, 1);
  local_FC = task_48[arg2];
  {
    int32_t local_A0;
    local_A0 = Script_ScenDataUtil(task_0, 4, local_FC * 13 + 1);
    local_A0 = local_A0 | 8;
    Script_ScenDataUtil(task_0, 14, local_FC * 13 + 1, local_A0);
  }
  {
    int32_t local_A0;
    local_A0 = Script_ScenDataUtil(task_0, 4, local_FC * 13 + 1);
    local_A0 = local_A0 | 0x10;
    Script_ScenDataUtil(task_0, 14, local_FC * 13 + 1, local_A0);
  }
  {
    int32_t local_A0;
    local_A0 = Script_ScenDataUtil(task_0, 4, local_FC * 13 + 1);
    local_A0 = local_A0 | 4;
    Script_ScenDataUtil(task_0, 14, local_FC * 13 + 1, local_A0);
  }
  return;
}

__attribute__((scn_string_bindings(0, data_1DB82)))
static void script_3F04(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
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
      if (arg0 == task_160) {
        local_FF5C = arg4;
        switch (local_FF5C) {
          case 1:
            local_CF = 1;
            local_A4 = Script_ScenDataUtil(data_1C60C, 4, Script_ScenDataUtil(data_1C01C, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
            local_C0 = 1.0f;
            local_A0 = 18;
            local_9C = 18;
            local_90 = 15473;
            local_8C = 17294;
            break;
          case 0:
            local_CF = 0;
            local_A4 = Script_ScenDataUtil(data_1C60C, 4, Script_ScenDataUtil(data_1C01C, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9) | -0x80000000;
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
            local_A4 = Script_ScenDataUtil(data_1C60C, 4, Script_ScenDataUtil(data_1C01C, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
            local_C0 = 1.0f;
            local_A0 = 18;
            local_9C = 18;
            local_90 = 13653;
            break;
          case 0:
            local_CF = 0;
            local_A4 = Script_ScenDataUtil(data_1C60C, 4, Script_ScenDataUtil(data_1C01C, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
            local_C0 = 1.0f;
            local_A0 = 18;
            local_9C = 18;
            local_90 = 19842;
            break;
        }
      }
    } else if ((local_FF5C == 1) | (local_FF5C == 7) | (local_FF5C == 30) | (local_FF5C == 36) | (local_FF5C == 37)) {
      if (arg0 == task_160) {
        local_FF5C = arg4;
        switch (local_FF5C) {
          case 1:
            local_CF = 0;
            local_A4 = Script_ScenDataUtil(data_1C60C, 4, Script_ScenDataUtil(data_1C01C, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9) | -0x80000000;
            local_C0 = 1.0f;
            local_A0 = 12;
            local_9C = 18;
            local_90 = 50062;
            local_8C = 48241;
            break;
          case 0:
            local_CF = 1;
            local_A4 = Script_ScenDataUtil(data_1C60C, 4, Script_ScenDataUtil(data_1C01C, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
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
            local_A4 = Script_ScenDataUtil(data_1C60C, 4, Script_ScenDataUtil(data_1C01C, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
            local_C0 = 1.0f;
            local_A0 = 18;
            local_9C = 18;
            if (task_98[arg2] == 4) {
              local_90 = 50062;
            } else {
              local_90 = 51882;
            }
            break;
          case 0:
            local_CF = 1;
            local_A4 = Script_ScenDataUtil(data_1C60C, 4, Script_ScenDataUtil(data_1C01C, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
            local_C0 = 1.0f;
            local_A0 = 18;
            local_9C = 18;
            if (task_98[arg2] == 4) {
              local_90 = 46421;
            } else {
              local_90 = 45511;
            }
            break;
        }
      }
    } else if ((local_FF5C == 3) | (local_FF5C == 5) | (local_FF5C == 32) | (local_FF5C == 34)) {
      if (arg0 == task_160) {
        local_FF5C = arg4;
        switch (local_FF5C) {
          case 1:
            local_CF = 1;
            local_A4 = Script_ScenDataUtil(data_1C60C, 4, Script_ScenDataUtil(data_1C01C, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9) | -0x80000000;
            local_C0 = -1.0f;
            local_A0 = 12;
            local_9C = 12;
            local_90 = 50062;
            local_8C = 48241;
            break;
          case 0:
            local_CF = 0;
            local_A4 = Script_ScenDataUtil(data_1C60C, 4, Script_ScenDataUtil(data_1C01C, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
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
            local_A4 = Script_ScenDataUtil(data_1C60C, 4, Script_ScenDataUtil(data_1C01C, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
            local_C0 = -1.0f;
            local_A0 = 18;
            local_9C = 18;
            local_90 = 51882;
            break;
          case 0:
            local_CF = 0;
            local_A4 = Script_ScenDataUtil(data_1C60C, 4, Script_ScenDataUtil(data_1C01C, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
            local_C0 = -1.0f;
            local_A0 = 18;
            local_9C = 18;
            local_90 = 45511;
            break;
        }
      }
    } else if ((local_FF5C == 2) | (local_FF5C == 8) | (local_FF5C == 31)) {
      if (arg0 == task_160) {
        local_FF5C = arg4;
        switch (local_FF5C) {
          case 1:
            local_CF = 0;
            local_A4 = Script_ScenDataUtil(data_1C60C, 4, Script_ScenDataUtil(data_1C01C, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
            local_C0 = -1.0f;
            local_A0 = 18;
            local_9C = 12;
            local_90 = 15473;
            local_8C = 17294;
            break;
          case 0:
            local_CF = 1;
            local_A4 = Script_ScenDataUtil(data_1C60C, 4, Script_ScenDataUtil(data_1C01C, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9) | -0x80000000;
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
            local_A4 = Script_ScenDataUtil(data_1C60C, 4, Script_ScenDataUtil(data_1C01C, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
            local_C0 = -1.0f;
            local_A0 = 18;
            local_9C = 18;
            local_90 = 13653;
            break;
          case 0:
            local_CF = 1;
            local_A4 = Script_ScenDataUtil(data_1C60C, 4, Script_ScenDataUtil(data_1C01C, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
            local_C0 = -1.0f;
            local_A0 = 18;
            local_9C = 18;
            local_90 = 19842;
            break;
        }
      }
    } else {
      if ((local_FF5C == 13) | (local_FF5C == 14) | (local_FF5C == 28)) {
        if (arg0 == task_160) {
          local_FF5C = arg4;
          switch (local_FF5C) {
            case 0:
              local_CF = 0;
              local_A4 = Script_ScenDataUtil(data_1C60C, 4, Script_ScenDataUtil(data_1C01C, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
              local_C0 = -1.0f;
              local_A0 = 18;
              local_9C = 12;
              local_90 = 48241;
              local_8C = 50062;
              break;
            case 1:
              local_CF = 1;
              local_A4 = Script_ScenDataUtil(data_1C60C, 4, Script_ScenDataUtil(data_1C01C, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9) | -0x80000000;
              local_C0 = -1.0f;
              local_A0 = 12;
              local_9C = 12;
              local_90 = 50062;
              local_8C = 48241;
              break;
            case 2:
              local_CF = 1;
              local_A4 = Script_ScenDataUtil(data_1C60C, 4, Script_ScenDataUtil(data_1C01C, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
              local_C0 = 1.0f;
              local_A0 = 18;
              local_9C = 18;
              local_90 = 15473;
              local_8C = 17294;
              break;
            case 3:
              local_CF = 0;
              local_A4 = Script_ScenDataUtil(data_1C60C, 4, Script_ScenDataUtil(data_1C01C, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9) | -0x80000000;
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
              local_A4 = Script_ScenDataUtil(data_1C60C, 4, Script_ScenDataUtil(data_1C01C, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
              local_C0 = -1.0f;
              local_A0 = 18;
              local_9C = 18;
              local_90 = 45511;
              break;
            case 1:
              local_CF = 1;
              local_A4 = Script_ScenDataUtil(data_1C60C, 4, Script_ScenDataUtil(data_1C01C, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
              local_C0 = -1.0f;
              local_A0 = 18;
              local_9C = 18;
              local_90 = 51882;
              break;
            case 2:
              local_CF = 1;
              local_A4 = Script_ScenDataUtil(data_1C60C, 4, Script_ScenDataUtil(data_1C01C, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
              local_C0 = 1.0f;
              local_A0 = 18;
              local_9C = 18;
              if (task_98[arg2] == 4) {
                local_90 = 14199;
              } else {
                local_90 = 13653;
              }
              break;
            case 3:
              local_CF = 0;
              local_A4 = Script_ScenDataUtil(data_1C60C, 4, Script_ScenDataUtil(data_1C01C, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
              local_C0 = 1.0f;
              local_A0 = 18;
              local_9C = 18;
              if (task_98[arg2] == 4) {
                local_90 = 18022;
              } else {
                local_90 = 19842;
              }
              break;
          }
        }
        local_CE = 1;
        goto L004AF7;
      }
    }
  }
  L004AF7:;
  (void)local_90;
  *(int32_t *)&local_B8 = Script_ScenDataUtil(data_1C60C, 5, Script_ScenDataUtil(data_1C01C, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 1);
  *(int32_t *)&local_B4 = Script_ScenDataUtil(data_1C60C, 5, Script_ScenDataUtil(data_1C01C, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 2);
  *(int32_t *)&local_B0 = Script_ScenDataUtil(data_1C60C, 5, Script_ScenDataUtil(data_1C01C, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 3);
  *(int32_t *)&local_AC = Script_ScenDataUtil(data_1C60C, 5, Script_ScenDataUtil(data_1C01C, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 4);
  *(int32_t *)&local_A8 = Script_ScenDataUtil(data_1C60C, 5, Script_ScenDataUtil(data_1C01C, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 7);
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
  script_00E5(arg0, local_A4, arg2, arg3, arg4);
  local_A4 = local_A4 | ((Script_FloatUtils(ScriptMathOp_FloatToInt, local_B8 - 3.0f) << 16) & 0x7FFF0000);
  script_92E9(arg0, 0);
  local_D0.x = (&task_168)->x;
  (&local_D0)->y = task_168.y;
  (&local_D0)->z = task_168.z;
  Script_ryoControl(PCCTL_StartCameraFacingInterpolation, 0);
  Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
  Script_SetCharaMoveTarget(arg0, &task_168, *(&task_174 + 1), local_A4);
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
    if (arg0 == task_160) {
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
      if (arg0 == task_160) {
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
              script_233B(arg0, task_98[arg2]);
            }
          }
        }
      }
      if (!(local_C4 > local_B4)) {
        if (!(local_C4 < local_B8) & !(local_C4 > local_B4)) {
          if (arg0 == task_160) {
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
          local_FF50 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_48[arg2], 2);
          local_FF50 = Script_ScenDataUtil(task_8, 4, Script_ScenDataUtil(task_C, 4, local_FF50 * 9 + 1) * 4 + 2);
          if (local_FF50 != -1) {
            if (arg0 == task_160) {
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
        local_FF60 = script_006D((&local_FF64)->y & 0xFFFF, local_88 & 0xFFFF);
        if (local_FF60 > 910) {
          local_88 = (&local_FF64)->y + 910;
        } else {
          if (local_FF60 < -910) {
            local_88 = (&local_FF64)->y - 910;
            goto L0050FC;
          }
        }
        L0050FC:;
        if (!(local_C4 >= local_B4)) {
          if (!(local_C0 <= 0.0f)) {
            if (script_006D(local_FF70, local_88) > 0) {
              local_88 = local_FF70;
            }
          } else {
            if (script_006D(local_FF70, local_88) < 0) {
              local_88 = local_FF70;
            }
          }
        }
      }
      if (arg0 != task_160) {
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
          local_FF74 = (float)script_0030(script_006D(local_FF70, (&local_FF48)->y) * 360 / 0x10000);
        }
        if (!(local_C4 <= local_B4) & !(local_C4 > local_A8)) {
          *(int32_t *)&local_BC = Script_ScenDataUtil(data_1C60C, 5, Script_ScenDataUtil(data_1C01C, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 8);
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
          if (arg0 != task_160) {
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
        local_FF50 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_48[arg2], 2);
        local_FF50 = Script_ScenDataUtil(task_8, 4, Script_ScenDataUtil(task_C, 4, local_FF50 * 9 + 1) * 4 + 3);
        if (local_FF50 != -1) {
          if (arg0 == task_160) {
            Script_executeAicaCommand(local_FF50, 0, 0);
          } else {
            Script_Execute3DSoundCommand_Wrapper(local_FF50, 4, &local_D0, 5.0f, 20.0f, 0);
          }
        }
        local_A4 = task_48[arg2];
        if (arg0 == task_160) {
          if (task_160 == arg0) {
            local_A0 = (Script_ScenDataUtil(task_0, 4, local_A4 * 13 + 2) >> 16) & 0xFFFF;
            Script_scenToggleCharaFlag(0, local_A0, 1);
            local_A0 = Script_ScenDataUtil(task_0, 4, local_A4 * 13 + 2) & 0xFFFF;
            Script_scenToggleCharaFlag(0, local_A0, 1);
          }
        }
      }
      if (!(local_AC <= 0.0f)) {
        if (!(local_C4 <= local_AC)) {
          if (arg0 == task_160) {
            local_FF5C = arg3;
            if ((local_FF5C == 30) | (local_FF5C == 31) | (local_FF5C == 32) | (local_FF5C == 33) | (local_FF5C == 34) | (local_FF5C == 35)) {
              local_FF78 = local_FF78 - 0.06f;
              if (!(local_FF78 >= 0.3f)) {
                local_FF78 = 0.3f;
              }
              DSWK_SetActorOpacityAndDrawCallback(arg1, local_FF78);
              goto L0055A8;
            }
          }
        }
      }
      L0055A8:;
      if (!(local_C4 < local_B0) & !(local_C4 > local_AC)) {
        if (arg0 == task_160) {
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
      if (arg0 != task_160) {
        if (!(local_C4 < local_AC)) {
          break;
        }
        local_C4 = local_C4 + 1.0f;
      }
      SCNF_WaitFrames_0000(1);
    }
  }
  script_92E9(arg0, 1);
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
  if (arg0 == task_160) {
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
      goto L0057E3;
    }
  }
  L0057E3:;
  return;
}

__attribute__((scn_string_bindings(0, data_1DB9E)))
static void script_57E4(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
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
    script_92E9(arg0, 0);
    local_C4.x = (&task_168)->x;
    (&local_C4)->y = task_168.y;
    (&local_C4)->z = task_168.z;
    Script_ryoControl(PCCTL_StartCameraFacingInterpolation, 0);
    Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
    Script_SetCharaMoveTarget(arg0, &task_168, *(&task_174 + 1), local_94);
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
          local_84 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_48[arg2], 2);
          local_84 = Script_ScenDataUtil(task_8, 4, Script_ScenDataUtil(task_C, 4, local_84 * 9 + 1) * 4 + 3);
          if (local_84 != -1) {
            if (arg0 == task_160) {
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
                goto L005B08;
              }
            }
            L005B08:;
          } else {
            if ((local_84 == 1) | (local_84 == 3)) {
              if (!(local_B8 > -0.75f)) {
                local_B8 = -0.75f;
              } else {
                if (!(local_B8 < 0.0f)) {
                  local_B8 = 0.0f;
                  goto L005B44;
                }
              }
              L005B44:;
              goto L005B47;
            }
          }
        }
      }
      L005B47:;
      if (!(local_BC <= local_A4)) {
        int32_t local_84;
        local_84 = arg4;
        if ((local_84 == 0) | (local_84 == 2)) {
          local_B8 = 0.75f;
        } else {
          if ((local_84 == 1) | (local_84 == 3)) {
            local_B8 = -0.75f;
            goto L005B8C;
          }
        }
      }
      L005B8C:;
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
  script_92E9(arg0, 1);
  {
    int32_t local_94;
    local_94 = task_48[arg2];
    local_9C = Script_ScenDataUtil(task_0, 4, local_94 * 13 + 2) & 0xFFFF;
    Script_scenToggleCharaFlag(0, local_9C, 1);
    local_9C = (Script_ScenDataUtil(task_0, 4, local_94 * 13 + 2) >> 16) & 0xFFFF;
    Script_scenToggleCharaFlag(0, local_9C, 1);
    {
      int32_t unused_88[3];
      int32_t local_84;
      local_84 = Script_ScenDataUtil(task_0, 4, local_94 * 13 + 1);
      local_84 = local_84 & -9;
      Script_ScenDataUtil(task_0, 14, local_94 * 13 + 1, local_84);
    }
    {
      int32_t unused_88[3];
      int32_t local_84;
      local_84 = Script_ScenDataUtil(task_0, 4, local_94 * 13 + 1);
      local_84 = local_84 & -17;
      Script_ScenDataUtil(task_0, 14, local_94 * 13 + 1, local_84);
    }
    {
      int32_t unused_88[3];
      int32_t local_84;
      local_84 = Script_ScenDataUtil(task_0, 4, local_94 * 13 + 1);
      local_84 = local_84 & -5;
      Script_ScenDataUtil(task_0, 14, local_94 * 13 + 1, local_84);
    }
  }
  return;
}

__attribute__((scn_string_bindings(0, data_1DBBA)))
static void script_5F05(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
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
      local_8C = Script_ScenDataUtil(data_1C60C, 4, Script_ScenDataUtil(data_1C01C, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
      local_A0 = -1.0f;
      local_88 = 18;
      local_84 = 18;
      local_FF78 = 46421;
      local_FF74 = 48787;
      break;
    case 1:
      local_AF = 1;
      local_8C = Script_ScenDataUtil(data_1C60C, 4, Script_ScenDataUtil(data_1C01C, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9) | -0x80000000;
      local_A0 = -1.0f;
      local_88 = 12;
      local_84 = 12;
      local_FF78 = 50426;
      local_FF74 = 48969;
      break;
    case 2:
      local_AF = 1;
      local_8C = Script_ScenDataUtil(data_1C60C, 4, Script_ScenDataUtil(data_1C01C, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9);
      local_A0 = 1.0f;
      local_88 = 18;
      local_84 = 18;
      local_FF78 = 15109;
      local_FF74 = 16566;
      break;
    case 3:
      local_AF = 0;
      local_8C = Script_ScenDataUtil(data_1C60C, 4, Script_ScenDataUtil(data_1C01C, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9) | -0x80000000;
      local_A0 = 1.0f;
      local_88 = 12;
      local_84 = 12;
      local_FF78 = 19114;
      local_FF74 = 16748;
      break;
  }
  (void)local_FF78;
  *(int32_t *)&local_9C = Script_ScenDataUtil(data_1C60C, 5, Script_ScenDataUtil(data_1C01C, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 1);
  *(int32_t *)&local_98 = Script_ScenDataUtil(data_1C60C, 5, Script_ScenDataUtil(data_1C01C, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 2);
  *(int32_t *)&local_94 = Script_ScenDataUtil(data_1C60C, 5, Script_ScenDataUtil(data_1C01C, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 3);
  *(int32_t *)&local_90 = Script_ScenDataUtil(data_1C60C, 5, Script_ScenDataUtil(data_1C01C, 1, arg3 * 40 + task_98[arg2] * 4 + arg4) * 9 + 4);
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
  script_92E9(arg0, 0);
  local_B0.x = (&task_168)->x;
  (&local_B0)->y = task_168.y;
  (&local_B0)->z = task_168.z;
  Script_ryoControl(PCCTL_StartCameraFacingInterpolation, 0);
  Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
  Script_SetCharaMoveTarget(arg0, &task_168, *(&task_174 + 1), local_8C);
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
        local_FF64 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_48[arg2], 2);
        local_FF64 = Script_ScenDataUtil(task_8, 4, Script_ScenDataUtil(task_C, 4, local_FF64 * 9 + 1) * 4 + 2);
        if (local_FF64 != -1) {
          if (arg0 == task_160) {
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
        local_FF64 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_48[arg2], 2);
        local_FF64 = Script_ScenDataUtil(task_8, 4, Script_ScenDataUtil(task_C, 4, local_FF64 * 9 + 1) * 4 + 3);
        if (local_FF64 != -1) {
          if (arg0 == task_160) {
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
  script_92E9(arg0, 1);
  local_F4.x = 0;
  (&local_F4)->z = 0;
  (&local_F4)->y = local_80;
  Script_charaPosRotUtils(arg1, 12, 0, &local_F4, 0);
  (&local_F4)->y = local_FF7C;
  Script_charaPosRotUtils(arg1, 7, 0, &local_F4, 0);
  return;
}

static void script_668F(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
  int32_t local_FC;
  int32_t local_F8;
  Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
  Script_SetCharaMoveTarget(arg0, &task_168, *(&task_174 + 1), MN_AKI_AKI_BASE);
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
        local_FC = Script_ScenDataUtil(task_0, 4, task_48[arg2] * 13 + 7);
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

__attribute__((scn_string_bindings(0, data_1DBD6)))
static void script_6767(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
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
  local_D4 = Script_ScenDataUtil(task_0, 4, task_48[arg2] * 13 + 6);
  local_CC = arg3;
  if ((local_CC == 1) | (local_CC == 3) | (local_CC == 5) | (local_CC == 7) | (local_CC == 9) | (local_CC == 18) | (local_CC == 21) | (local_CC == 25) | (local_CC == 27) | (local_CC == 36) | (local_CC == 37)) {
    local_CC = arg4;
    switch (local_CC) {
      case 1:
        local_E0 = Script_ScenDataUtil(&data_1D02C, 4, local_D4 * 5) | -0x80000000;
        break;
      case 0:
        local_E0 = Script_ScenDataUtil(&data_1D02C, 4, local_D4 * 5);
        break;
    }
  } else if ((local_CC == 2) | (local_CC == 4) | (local_CC == 6) | (local_CC == 8) | (local_CC == 10) | (local_CC == 19) | (local_CC == 22) | (local_CC == 24) | (local_CC == 26) | (local_CC == 28)) {
    local_CC = arg4;
    switch (local_CC) {
      case 1:
        local_E0 = Script_ScenDataUtil(&data_1D02C, 4, local_D4 * 5);
        break;
      case 0:
        local_E0 = Script_ScenDataUtil(&data_1D02C, 4, local_D4 * 5) | -0x80000000;
        break;
    }
  } else {
    if ((local_CC == 13) | (local_CC == 14) | (local_CC == 15) | (local_CC == 16) | (local_CC == 20) | (local_CC == 23) | (local_CC == 29)) {
      local_CC = arg4;
      switch (local_CC) {
        case 0:
          local_E0 = Script_ScenDataUtil(&data_1D02C, 4, local_D4 * 5);
          break;
        case 1:
          local_E0 = Script_ScenDataUtil(&data_1D02C, 4, local_D4 * 5) | -0x80000000;
          break;
        case 2:
          local_E0 = Script_ScenDataUtil(&data_1D02C, 4, local_D4 * 5);
          break;
        case 3:
          local_E0 = Script_ScenDataUtil(&data_1D02C, 4, local_D4 * 5) | -0x80000000;
          break;
      }
      goto L006988;
    }
  }
  L006988:;
  *(int32_t *)&local_EC = Script_ScenDataUtil(&data_1D02C, 5, local_D4 * 5 + 1);
  *(int32_t *)&local_E8 = Script_ScenDataUtil(&data_1D02C, 5, local_D4 * 5 + 2);
  local_E0 = local_E0 | ((Script_FloatUtils(ScriptMathOp_FloatToInt, local_EC - 3.0f) << 16) & 0x7FFF0000);
  local_F4.x = (&task_168)->x;
  (&local_F4)->y = task_168.y;
  (&local_F4)->z = task_168.z;
  Script_ryoControl(PCCTL_StartCameraFacingInterpolation, 0);
  Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
  Script_SetCharaMoveTarget(arg0, &task_168, *(&task_174 + 1), local_E0);
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
      local_C8 = Script_ScenDataUtil(task_0, 4, task_48[arg2] * 13 + 9);
      if (local_C8 != -1) {
        if (arg0 == task_160) {
          Script_executeAicaCommand(local_C8, 0, 0);
        } else {
          Script_Execute3DSoundCommand_Wrapper(local_C8, 4, &local_F4, 5.0f, 20.0f, 0);
        }
      }
    }
    if (!(local_F0 < local_E8) & !(local_F0 >= local_E8 + 1.0f)) {
      if ((local_D4 == 6) | (local_D4 == 7)) {
        int32_t local_C8;
        local_C8 = Script_ScenDataUtil(task_0, 4, task_48[arg2] * 13 + 9);
        if (local_C8 != -1) {
          if (arg0 == task_160) {
            Script_executeAicaCommand(local_C8, 0, 0);
          } else {
            Script_Execute3DSoundCommand_Wrapper(local_C8, 4, &local_F4, 5.0f, 20.0f, 0);
          }
        }
      }
      if (local_DC == 0) {
        local_DC = 1;
        local_D8 = Script_ScenDataUtil(task_0, 4, task_48[arg2] * 13 + 7);
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

__attribute__((scn_string_bindings(0, data_1DBF2)))
static void script_6BDE(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
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
  task_18C = 0;
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
        goto L006EEA;
      }
    }
  }
  L006EEA:;
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
  local_B8.x = (&task_168)->x;
  (&local_B8)->y = task_168.y;
  (&local_B8)->z = task_168.z;
  Script_ryoControl(PCCTL_StartCameraFacingInterpolation, 0);
  Script_SetCharaMotionPlaybackRate(arg0, 1.0f);
  Script_SetCharaMoveTarget(arg0, &task_168, *(&task_174 + 1), local_90);
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
        local_FF40 = Script_ScenDataUtil(task_0, 4, task_48[arg2] * 13 + 10);
        if (local_FF40 != -1) {
          if (arg0 == task_160) {
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
      local_FF40 = Script_ScenDataUtil(task_0, 4, task_48[arg2] * 13 + 9);
      if (local_FF40 != -1) {
        if (arg0 == task_160) {
          Script_executeAicaCommand(local_FF40, 0, 0);
        } else {
          Script_Execute3DSoundCommand_Wrapper(local_FF40, 4, &local_B8, 5.0f, 20.0f, 0);
        }
      }
    }
    if (!(local_A0 < local_94) & !(local_A0 >= local_94 + 1.0f)) {
      int32_t unused_FF44;
      int32_t local_FF40;
      local_FF40 = Script_ScenDataUtil(task_0, 4, task_48[arg2] * 13 + 9);
      if (local_FF40 != -1) {
        if (arg0 == task_160) {
          Script_executeAicaCommand(local_FF40, 0, 0);
        } else {
          Script_Execute3DSoundCommand_Wrapper(local_FF40, 4, &local_B8, 5.0f, 20.0f, 0);
        }
      }
      if (local_FF68 == 0) {
        local_FF68 = 1;
        local_FF64 = Script_ScenDataUtil(task_0, 4, task_48[arg2] * 13 + 8);
        Script_ConversationUtils(1, local_FF74);
        Script_EnqueueSCNT(1, 0x524F4F44, local_FF64);
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  local_A4 = 0.0f;
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0) != 0) {
    local_FF6C = 0;
    L0079B5:;
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
      if (!(local_FF6C >= 20)) goto L0079B5;
    }
    {
      int32_t unused_FF44;
      int32_t local_FF40;
      local_FF40 = Script_ScenDataUtil(task_0, 4, task_48[arg2] * 13 + 11);
      if (local_FF40 != -1) {
        if (arg0 == task_160) {
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

static void script_7B71(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
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
    if (arg0 != task_160) {
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
      Script_SetCharaMoveTarget(arg0, &local_E0, local_BC[1], script_0697(arg5));
    }
  }
  return;
}

static void script_7FBB(int32_t arg0, DSWKDoorEntry *arg1, int32_t *arg2, DoorResourceDescriptor *arg3, MapObjectRecord *arg4, int32_t arg5, int32_t arg6) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t unused_F4;
  task_0 = arg1;
  task_4 = arg2;
  task_8 = arg3;
  task_C = arg4;
  task_10.mode = 0;
  task_10.id = 0;
  task_10.entry_idx = 0;
  task_160 = arg0;
  (void)((task_184 = -1) & 1);
  task_180 = 0;
  Script_motiLoad(Script_GetSceneID(), SHEN_NAME('FREE'), "M_MDOR.BIN");
  local_F8 = 0;
  L008025:;
  local_FC = Script_AddCharacterHandler(1, 0, local_F8 * 0x1000000 + 0x30726F64);
  Script_PreserveAccumulator_012C(local_FC, "This Charactor Work is NULL!!\012");
  DSWK_AllocDoorMapcRenderState(local_FC);
  Script_scenToggleVisibleFor(local_FC, 0);
  task_E8[local_F8] = local_FC;
  task_20[local_F8] = -1;
  task_48[local_F8] = -1;
  task_70[local_F8] = 0;
  task_98[local_F8] = 0;
  task_C0[local_F8] = 0;
  task_110[local_F8] = 0;
  local_F8 = local_F8 + 1;
  if (!(local_F8 >= 10)) goto L008025;
  local_F8 = 0;
  for (;;) {
    if (Script_ScenDataUtil(task_0, 4, local_F8 * 13) == -1) {
      break;
    }
    Script_ScenDataUtil(task_0, 14, local_F8 * 13 + 1, 0);
    local_F8 = local_F8 + 1;
  }
  Script_EnqueueSCNT(0);
  DSWK_BindTablesAndInitState(arg1, arg2, arg3, arg4, &task_10, task_20, task_48, (shen_name *)task_70, task_98, arg0, arg5, data_1C01C, data_1C60C, (shen_name *)task_E8);
  Script_scenCreateTask(script_8877, 0);
  return;
}

static void script_8147(DSWKDoorEntry *arg0, int32_t *arg1, DoorResourceDescriptor *arg2, MapObjectRecord *arg3, int32_t arg4) {
  task_0 = arg0;
  task_4 = arg1;
  task_8 = arg2;
  task_C = arg3;
  DSWK_RebindDoorSystemTables(arg0, arg1, arg2, arg3, arg4);
  task_180 = 1;
  return;
}

static void script_817B(void) {
  int32_t local_FC;
  local_FC = 0;
  L008183:;
  task_20[local_FC] = -1;
  task_48[local_FC] = -1;
  task_70[local_FC] = 0;
  task_98[local_FC] = 0;
  task_C0[local_FC] = 0;
  task_110[local_FC] = 0;
  local_FC = local_FC + 1;
  if (!(local_FC >= 10)) goto L008183;
  return;
}

static void script_81E4(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  local_F4 = 0;
  for (;;) {
    Script_sprintf("手動ドア開け番号指定\012");
    if (task_160 == arg0) {
      Script_EVT_GetPosition(arg0, -1, &task_194, 0);
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

static void script_8281(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  Vector3 local_F0;
  if (task_160 == arg0) {
    Script_EVT_GetPosition(arg0, -1, &task_194, 0);
  }
  Script_EVT_GetPosition(arg0, -1, &local_F0, 0);
  local_FC = DSWK_DoorUtils(DOOR_CMD_FIND_CLOSEST_ELEMENT, arg0, &local_F0, arg1);
  (void)local_FC;
  return;
}

static void script_82BB(void) {
  int32_t local_FC;
  int32_t local_F8;
  local_FC = 0;
  for (;;) {
    if (Script_ScenDataUtil(task_0, 4, local_FC * 13) == -1) {
      break;
    }
    local_F8 = Script_ScenDataUtil(task_0, 4, local_FC * 13 + 1);
    local_F8 = local_F8 | 2;
    Script_ScenDataUtil(task_0, 14, local_FC * 13 + 1, local_F8);
    local_FC = local_FC + 1;
  }
  return;
}

static void script_831E(void) {
  int32_t local_FC;
  int32_t local_F8;
  local_FC = 0;
  for (;;) {
    if (Script_ScenDataUtil(task_0, 4, local_FC * 13) == -1) {
      break;
    }
    local_F8 = Script_ScenDataUtil(task_0, 4, local_FC * 13 + 1);
    local_F8 = local_F8 & -3;
    Script_ScenDataUtil(task_0, 14, local_FC * 13 + 1, local_F8);
    local_FC = local_FC + 1;
  }
  return;
}

static void script_8381(int32_t arg0) {
  int32_t local_FC;
  local_FC = Script_ScenDataUtil(task_0, 4, arg0 * 13 + 1);
  local_FC = local_FC | 2;
  Script_ScenDataUtil(task_0, 14, arg0 * 13 + 1, local_FC);
  return;
}

static void script_83B8(int32_t arg0) {
  int32_t local_FC;
  local_FC = Script_ScenDataUtil(task_0, 4, arg0 * 13 + 1);
  local_FC = local_FC & -3;
  Script_ScenDataUtil(task_0, 14, arg0 * 13 + 1, local_FC);
  return;
}

static void script_83EF(int32_t arg0) {
  Vector3 local_F4;
  int32_t local_F0;
  int32_t local_EC;
  local_F0 = 0;
  for (;;) {
    if (task_48[local_F0] == arg0) {
      local_EC = task_E8[local_F0];
      if (Script_shenGetNode(local_EC, 7)) {
        if (Script_shenGetNode(local_EC, 5) & Script_shenGetNode(local_EC, 10)) {
          if (Script_ScenDataUtil(task_0, 4, task_48[local_F0] * 13 + 1) & 0x100) {
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
          if (Script_ScenDataUtil(task_0, 4, task_48[local_F0] * 13 + 1) & 0x100) {
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
        if (Script_ScenDataUtil(task_0, 4, task_48[local_F0] * 13 + 1) & 0x20) {
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

static void script_8631(int32_t arg0) {
  int32_t local_FC;
  local_FC = Script_ScenDataUtil(task_0, 4, arg0 * 13 + 1);
  local_FC = local_FC | 0x20;
  Script_ScenDataUtil(task_0, 14, arg0 * 13 + 1, local_FC);
  script_8739(arg0);
  script_83EF(arg0);
  return;
}

static void script_867A(int32_t arg0) {
  int32_t local_FC;
  local_FC = Script_ScenDataUtil(task_0, 4, arg0 * 13 + 1);
  local_FC = local_FC & -33;
  Script_ScenDataUtil(task_0, 14, arg0 * 13 + 1, local_FC);
  script_8747(arg0);
  script_83EF(arg0);
  return;
}

static void script_86C3(int32_t arg0, int32_t arg1) {
  if (arg1) {
    int32_t local_FC;
    local_FC = Script_ScenDataUtil(task_0, 4, arg0 * 13 + 1);
    local_FC = local_FC & -257;
    Script_ScenDataUtil(task_0, 14, arg0 * 13 + 1, local_FC);
  } else {
    int32_t local_FC;
    local_FC = Script_ScenDataUtil(task_0, 4, arg0 * 13 + 1);
    local_FC = local_FC | 0x100;
    Script_ScenDataUtil(task_0, 14, arg0 * 13 + 1, local_FC);
  }
  return;
}

static void script_8739(int32_t arg0) {
  script_86C3(arg0, 0);
  return;
}

static void script_8747(int32_t arg0) {
  script_86C3(arg0, 1);
  return;
}

static void script_8755(int32_t arg0) {
  int32_t local_FC;
  local_FC = 0;
  for (;;) {
    if (Script_ScenDataUtil(task_0, 4, local_FC * 13) == -1) {
      break;
    }
    if (arg0) {
      int32_t local_F8;
      local_F8 = Script_ScenDataUtil(task_0, 4, local_FC * 13 + 1);
      local_F8 = local_F8 & -65;
      local_F8 = local_F8 & -129;
      local_F8 = local_F8 | 0x40;
      Script_ScenDataUtil(task_0, 14, local_FC * 13 + 1, local_F8);
    } else {
      int32_t local_F8;
      local_F8 = Script_ScenDataUtil(task_0, 4, local_FC * 13 + 1);
      local_F8 = local_F8 & -65;
      local_F8 = local_F8 & -129;
      local_F8 = local_F8 | 0x80;
      Script_ScenDataUtil(task_0, 14, local_FC * 13 + 1, local_F8);
    }
    local_FC = local_FC + 1;
  }
  return;
}

static void script_881C(void) {
  script_8755(1);
  return;
}

static void script_8828(void) {
  script_8755(0);
  return;
}

static void script_8834(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5, int32_t arg6) {
  int32_t local_FC;
  local_FC = 0;
  for (;;) {
    if (Script_ScenDataUtil(task_0, 4, local_FC * 13) == -1) {
      break;
    }
    DSWK_AssignDoorEntryConfig(local_FC, arg0, arg1, arg2, arg3, arg4, arg5, arg6);
    local_FC = local_FC + 1;
  }
  return;
}

static int32_t script_8877(void) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  SCNF_WaitFrames_0000(1);
  SCNF_WaitFrames_0000(1);
  SCNF_WaitFrames_0000(1);
  local_F4 = -1;
  local_F8 = -1;
  for (;;) {
    if (task_180 == 0) {
      local_F4 = DSWK_DoorUtils(DOOR_CMD_FIND_LOWEST_FLOAT_INDEX, 0x3FD851EA);
      if (-(local_F4 != -1) & -(task_20[local_F4] == 1) & Script_IsPlayerAllowed(0) & -((&task_10)->mode == 0)) {
        local_FC = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_48[local_F4], 2);
        local_F8 = Script_ScenDataUtil(task_8, 4, Script_ScenDataUtil(task_C, 4, local_FC * 9 + 1) * 4 + 1);
        script_9647(1, local_FC, local_F4, local_F8);
        if (script_97FB(local_FC, local_F4, local_F8)) {
          local_F4 = -1;
          local_F8 = -1;
        }
      } else {
        script_9647(0, local_FC, local_F4, local_F8);
        local_F4 = -1;
        local_F8 = -1;
      }
      script_9A11(local_F4, local_F8);
    } else {
      task_180 = 0;
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int8_t script_8988(int32_t arg0, int32_t arg1, int8_t arg2) {
  Vector3 local_F4;
  Vector3 local_E8;
  Vector3i local_DC;
  int32_t local_D8;
  arg2 = 0;
  Script_EVT_GetPosition(task_160, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  Script_mtwkGetSetRotation(task_160, &local_DC, 0x42000000);
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

__attribute__((scn_string_bindings(0, data_1DC7D, 1, data_1DC96)))
static void script_8A20(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  Vector3 local_EC;
  Vector3i local_E0;
  local_F8 = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_48[arg0], 1);
  if (local_F8 != -1) {
    local_FC = Script_AddCharacterHandler(1, 0, arg0 * 0x1000000 + 0x30786F62);
    if (local_FC == 0) {
      Script_PreserveAccumulator_012C(local_FC, "Don't make BOX Buffer!!\012");
      task_138[arg0] = -1;
    } else if (local_FC == arg0 * 0x1000000 + 0x30786F62) {
      task_138[arg0] = local_FC;
      *(int32_t *)&local_EC.x = Script_ScenDataUtil(task_C, 4, local_F8 * 9 + 5);
      *(int32_t *)&(&local_EC)->y = Script_ScenDataUtil(task_C, 4, local_F8 * 9 + 6);
      *(int32_t *)&(&local_EC)->z = Script_ScenDataUtil(task_C, 4, local_F8 * 9 + 7);
      Script_EVT_SetPosition(local_FC, POS_XYZ, &local_EC);
      local_E0.x = 0;
      (&local_E0)->y = Script_ScenDataUtil(task_C, 4, local_F8 * 9 + 8);
      (&local_E0)->z = 0;
      Script_EVT_SetRotation(local_FC, 0x38000000, &local_E0, 0);
      *(int32_t *)&local_EC.x = Script_ScenDataUtil(task_C, 4, local_F8 * 9 + 2);
      *(int32_t *)&(&local_EC)->y = Script_ScenDataUtil(task_C, 4, local_F8 * 9 + 3);
      *(int32_t *)&(&local_EC)->z = Script_ScenDataUtil(task_C, 4, local_F8 * 9 + 4);
      Script_hmdlApplyScale(local_FC, &local_EC);
      Script_CreateNPCImageModule(local_FC, (const char *)Script_ScenDataUtil(task_8, 8, Script_ScenDataUtil(task_C, 4, local_F8 * 9 + 1) * 4), 0);
      Script_charaDisplay(local_FC, CDM_Show);
      Script_charaVisUtils(local_FC, 1);
    } else {
      Script_PreserveAccumulator_012C(local_FC, "Don't make BOX Buffer!!\012");
      task_138[arg0] = -1;
    }
  } else {
    task_138[arg0] = -1;
  }
  return;
}

static void script_8BEF(int32_t arg0) {
  if (task_138[arg0] != -1) {
    Script_SetMapControlTaskFlag(task_138[arg0], 1);
    task_138[arg0] = -1;
  }
  return;
}

static int32_t script_8C20(int32_t arg0, int32_t arg1, int32_t arg2, float arg3, float arg4, int32_t arg5, int32_t arg6) {
  Vector3 local_F4;
  Vector3 local_E8;
  float local_DC[3];
  Vector3i local_D0;
  Vector3i local_C4;
  int32_t local_C0;
  int32_t local_BC;
  if (task_160 == arg0) {
    local_F4.x = (&task_194)->x;
    (&local_F4)->y = task_194.y;
    (&local_F4)->z = task_194.z;
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
        if ((Script_ScenDataUtil(task_0, 4, arg6 * 13 + 1) & 8) | (Script_ScenDataUtil(task_0, 4, arg6 * 13 + 1) & 0x40)) {
          local_C0 = local_C0 ^ 3;
        }
      } else {
        if ((Script_ScenDataUtil(task_0, 4, arg6 * 13 + 1) & 0x10) | (Script_ScenDataUtil(task_0, 4, arg6 * 13 + 1) & 0x80)) {
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
          if ((local_B4 == 1) | (local_B4 == 2)) goto L008E0B;
        }
        L008E0B:;
      } else if ((local_B4 == 2) | (local_B4 == 4) | (local_B4 == 6) | (local_B4 == 8) | (local_B4 == 10) | (local_B4 == 12) | (local_B4 == 24) | (local_B4 == 26) | (local_B4 == 31) | (local_B4 == 33) | (local_B4 == 35)) {
        local_B4 = local_C0;
        if ((local_B4 == 2) | (local_B4 == 1)) {
          arg3 = (float)0 - arg3;
        } else {
          if ((local_B4 == 3) | (local_B4 == 0)) {
            arg3 = (float)0 - arg3;
            arg4 = (float)0 - arg4;
            (&local_C4)->y = (&local_C4)->y + 0x8000;
            goto L008EAE;
          }
        }
        L008EAE:;
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
            goto L00905A;
          }
        }
      }
    }
    L00905A:;
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
          task_168.x = (&local_F4)->x;
          task_168.y = (&local_F4)->y;
          task_168.z = (&local_F4)->z;
          task_174 = (&local_C4)->x;
          *(&task_174 + 1) = (&local_C4)->y;
          *(&task_174 + 2) = (&local_C4)->z;
          break;
        case 5:
          task_168.x = (&local_F4)->x;
          task_168.y = (&local_F4)->y;
          task_168.z = (&local_F4)->z;
          task_174 = (&local_C4)->x;
          *(&task_174 + 1) = (&local_C4)->y;
          *(&task_174 + 2) = (&local_C4)->z;
          if ((local_C0 == 1) | (local_C0 == 2)) {
            local_C0 = 1;
          } else {
            local_C0 = 0;
          }
          break;
        case 7:
          Script_EVT_GetPosition(arg0, -1, &task_168, 0);
          task_174 = (&local_C4)->x;
          *(&task_174 + 1) = (&local_C4)->y;
          *(&task_174 + 2) = (&local_C4)->z;
          break;
      }
    }
  }
  return local_C0;
}

static void script_92E9(int32_t arg0, int32_t arg1) {
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
    if (task_160 == arg0) {
      if (arg1) {
        Script_ccowUtils(arg0, 3);
      } else {
        Script_ccowUtils(arg0, 4);
      }
    }
  }
  return;
}

static void script_931B(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  int32_t local_F8;
  local_F8 = script_8C20(arg0, arg1, 99, 0.0f, 0.5f, -1, -1);
  if (task_160 != arg0) {
    if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
      Script_charaToggleMotEnabled(arg0, 1);
    }
  } else if ((local_F8 == 0) | (local_F8 == 1)) {
    if ((Script_ScenDataUtil(task_0, 4, arg2 * 13 + 1) & 8) == 0) {
      if ((Script_ScenDataUtil(task_0, 4, arg2 * 13 + 1) & 0x40) != 0) {
        local_FC = Script_ScenDataUtil(task_0, 4, arg2 * 13 + 2) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_FC, 0);
      } else {
        local_FC = (Script_ScenDataUtil(task_0, 4, arg2 * 13 + 2) >> 16) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_FC, 0);
      }
    }
  } else {
    if ((Script_ScenDataUtil(task_0, 4, arg2 * 13 + 1) & 0x10) == 0) {
      if ((Script_ScenDataUtil(task_0, 4, arg2 * 13 + 1) & 0x80) != 0) {
        local_FC = (Script_ScenDataUtil(task_0, 4, arg2 * 13 + 2) >> 16) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_FC, 0);
      } else {
        local_FC = Script_ScenDataUtil(task_0, 4, arg2 * 13 + 2) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_FC, 0);
      }
    }
  }
  return;
}

__attribute__((scn_string_bindings(0, data_1DCAF)))
static void script_948B(int32_t arg0, int32_t arg1, int32_t arg2) {
  int32_t local_FC;
  if (task_160 == arg0) {
    task_10.mode = 0;
    task_10.id = 0;
    if ((arg2 == 0) | (arg2 == 2)) {
      Script_PlayCharaMotion(task_160, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
      if (task_184 & 0x20) {
        Script_sprintf("Door System : ");
        Script_ryoControl(RYO_RestoreControl);
      }
      Script_shenClearFreeState(1);
      if (arg2 != 2) {
        int32_t local_F8;
        local_F8 = task_48[arg1];
        local_FC = Script_ScenDataUtil(task_0, 4, local_F8 * 13 + 2) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_FC, 1);
        local_FC = (Script_ScenDataUtil(task_0, 4, local_F8 * 13 + 2) >> 16) & 0xFFFF;
        Script_scenToggleCharaFlag(0, local_FC, 1);
      }
    }
  } else {
    if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, arg0)) {
      Script_charaToggleMotEnabled(arg0, 0);
    }
    if ((arg2 == 0) | (arg2 == 2)) {
      int32_t local_F8;
      local_F8 = task_48[arg1];
      local_FC = Script_ScenDataUtil(task_0, 4, local_F8 * 13 + 2) & 0xFFFF;
      Script_scenToggleCharaFlag(0, local_FC, 1);
      local_FC = (Script_ScenDataUtil(task_0, 4, local_F8 * 13 + 2) >> 16) & 0xFFFF;
      Script_scenToggleCharaFlag(0, local_FC, 1);
    }
  }
  task_20[arg1] = 1;
  task_110[arg1] = 0;
  task_C0[arg1] = 0;
  task_70[arg1] = 0;
  if (task_138[arg1] != -1) {
    Script_SetMapControlTaskFlag(task_138[arg1], 1);
    task_138[arg1] = -1;
  }
  return;
}

static void script_9647(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  Vector3 local_E8;
  local_FC = 0;
  if (arg0 == 1) {
    local_F4 = arg3;
    if ((local_F4 == 17) | (local_F4 == 11) | (local_F4 == 12)) {
      local_FC = 0;
    } else if ((Script_ScenDataUtil(task_0, 4, task_48[arg2] * 13 + 1) & 2) == 0) {
      if ((Script_ScenDataUtil(task_0, 4, task_48[arg2] * 13 + 1) & 4) == 0) {
        local_FC = script_8988(arg2, arg3, local_FC);
      } else {
        local_F4 = arg3;
        if ((local_F4 == 21) | (local_F4 == 22) | (local_F4 == 23)) {
          local_FC = script_8988(arg2, arg3, local_FC);
          goto L00970B;
        }
      }
      L00970B:;
    } else {
      if ((Script_ScenDataUtil(task_0, 4, task_48[arg2] * 13 + 1) & 4) == 0) {
        local_FC = script_8988(arg2, arg3, local_FC);
        local_F8 = Script_ScenDataUtil(task_0, 4, task_48[arg2] * 13 + 6);
        if (local_F8 == 0) {
          local_FC = 0;
        } else {
          if (local_F8 >= 2) {
            Script_EVT_GetPosition(task_160, -1, &local_E8, 0);
            if (Script_CycleManagerUtils(CYCLE_CMD_COUNT_CYCLES_IN_RADIUS, &local_E8, 1.5f)) {
              local_FC = 0;
            }
            goto L0097A6;
          }
        }
      }
    }
  }
  L0097A6:;
  if (task_110[arg2]) {
    local_FC = 0;
  }
  Script_scenUtils(SCENARIO_CMD_SET_HELP_PROMPT_OVERRIDE, local_FC);
  if ((local_FC ^ task_190) & local_FC) {
    Script_sicnUtils(0, 4, 1);
  }
  if ((local_FC ^ task_190) & task_190) {
    Script_sicnUtils(2, 4, 0);
  }
  task_190 = local_FC;
  return;
}

__attribute__((scn_string_bindings(0, data_1DCBE)))
static int32_t script_97FB(int32_t arg0, int32_t arg1, int32_t arg2) {
  int32_t local_FC;
  int32_t local_F8;
  Vector3 local_EC;
  local_FC = 1;
  local_F8 = arg2;
  if ((local_F8 == 17) | (local_F8 == 11) | (local_F8 == 12)) {
    if (((Script_ScenDataUtil(task_0, 4, task_48[arg1] * 13 + 1) & 2) == 0) & ((Script_ScenDataUtil(task_0, 4, task_48[arg1] * 13 + 1) & 4) == 0)) {
      if (task_110[arg1] == 0) {
        task_20[arg1] = 4;
        local_FC = 0;
      }
    }
  } else {
    if (Script_GetControllerInput(1) == 0x400) {
      if (-(task_70[arg1] != task_160) & task_190) {
        if ((Script_ScenDataUtil(task_0, 4, task_48[arg1] * 13 + 1) & 2) == 0) {
          task_20[arg1] = 3;
          task_70[arg1] = task_160;
          task_98[arg1] = 0;
          task_10.mode = 1;
          task_10.entry_idx = task_48[arg1];
          local_FC = 0;
        } else {
          if (Script_ScenDataUtil(task_0, 4, task_48[arg1] * 13 + 6) >= 2) {
            Script_EVT_GetPosition(task_160, -1, &local_EC, 0);
            if (Script_CycleManagerUtils(CYCLE_CMD_COUNT_CYCLES_IN_RADIUS, &local_EC, 1.5f) == 0) {
              task_20[arg1] = 6;
              task_70[arg1] = task_160;
              task_98[arg1] = 0;
              task_10.mode = 0;
              task_10.entry_idx = task_48[arg1];
              local_FC = 0;
            }
            goto L0099AF;
          }
        }
        L0099AF:;
        if (local_FC == 0) {
          Script_EVT_GetPosition(task_160, -1, &task_194, 0);
          Script_ryoControl(PCCTL_RequestNewMode, 4);
          Script_sprintf("Door System : ");
          Script_shenClearFreeState(0);
          Script_ryoControl(PCCTL_DisablePlayerControl);
          task_10.id = task_160;
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

static void script_9A11(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  int32_t local_F8;
  local_FC = 0;
  L009A19:;
  local_F8 = task_20[local_FC];
  switch (local_F8) {
    case 6:
      if (!(arg0 != -1)) goto L009A39;
      L009A39:;
      script_9ADB(local_FC, arg1);
      break;
    case 5:
      if (!(arg0 != -1)) goto L009A59;
      L009A59:;
      script_9B3C(local_FC);
      break;
    case 4:
      if (!(arg0 != -1)) goto L009A77;
      L009A77:;
      script_9CE1(local_FC, arg0, arg1);
      break;
    case 3:
      if (!(arg0 != -1)) goto L009A99;
      L009A99:;
      script_9D79(local_FC, arg1);
      break;
    case 2:
      script_9E5B(local_FC);
      break;
    case 1:
      break;
  }
  local_FC = local_FC + 1;
  if (!(local_FC >= 10)) goto L009A19;
  return;
}

static void script_9ADB(int32_t arg0, int32_t arg1) {
  if (task_110[arg0] == 0) {
    task_110[arg0] = Script_scenCreateTask(script_1C76, 4, task_70[arg0], task_E8[arg0], arg0, arg1);
    task_C0[arg0] = task_70[arg0];
    script_8A20(arg0);
  }
  return;
}

__attribute__((scn_string_bindings(0, data_1DCCD, 1, data_1DCDF)))
static void script_9B3C(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, task_70[arg0])) {
    if (task_110[arg0] == 0) {
      if (task_70[arg0] == task_160) {
        task_10.mode = 1;
        task_10.entry_idx = task_48[arg0];
        goto L009B91;
      }
      L009B91:;
      local_FC = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_48[arg0], 2);
      local_F8 = Script_ScenDataUtil(task_8, 4, Script_ScenDataUtil(task_C, 4, local_FC * 9 + 1) * 4 + 1);
      script_931B(task_70[arg0], task_E8[arg0], task_48[arg0], local_F8);
      task_110[arg0] = Script_scenCreateTask(Script_ScenDataUtil(&data_1D0CC, 8, local_F8), 4, task_70[arg0], task_E8[arg0], arg0, local_F8);
      task_C0[arg0] = task_70[arg0];
      if ((Script_ScenDataUtil(task_0, 4, task_48[arg0] * 13 + 1) & 0x20000) == 0) {
        Script_sprintf("Door Box is ON!!\012");
        script_8A20(arg0);
      } else {
        Script_sprintf("Door Box is OFF!!\012");
        task_138[arg0] = -1;
      }
    }
  } else {
    if (task_20[arg0] != 5) {
      task_20[arg0] = 1;
      task_110[arg0] = 0;
      task_C0[arg0] = 0;
      task_70[arg0] = 0;
    }
  }
  return;
}

static void script_9CE1(int32_t arg0, int32_t arg1, int32_t arg2) {
  if ((task_110[arg0] == 0) & (arg2 >= 0)) {
    task_70[arg0] = task_160;
    task_10.mode = 2;
    task_10.entry_idx = task_48[arg0];
    task_110[arg0] = Script_scenCreateTask(Script_ScenDataUtil(&data_1D0CC, 8, arg2), 4, task_70[arg0], task_E8[arg0], arg0, arg2);
    task_C0[arg0] = task_70[arg0];
    script_8A20(arg0);
  }
  return;
}

__attribute__((scn_string_bindings(0, data_1DCF2, 1, data_1DD04)))
static void script_9D79(int32_t arg0, int32_t arg1) {
  if ((task_110[arg0] == 0) & (arg1 >= 0)) {
    script_931B(task_70[arg0], task_E8[arg0], task_48[arg0], arg1);
    task_110[arg0] = Script_scenCreateTask(Script_ScenDataUtil(&data_1D0CC, 8, arg1), 4, task_70[arg0], task_E8[arg0], arg0, arg1);
    task_C0[arg0] = task_70[arg0];
    if ((Script_ScenDataUtil(task_0, 4, task_48[arg0] * 13 + 1) & 0x20000) == 0) {
      Script_sprintf("Door Box is ON!!\012");
      script_8A20(arg0);
    } else {
      Script_sprintf("Door Box is OFF!!\012");
      task_138[arg0] = -1;
    }
  }
  return;
}

static void script_9E5B(int32_t arg0) {
  int32_t local_FC;
  task_110[arg0] = 0;
  task_C0[arg0] = 0;
  task_20[arg0] = 1;
  local_FC = DSWK_DoorUtils(DOOR_CMD_FIND_ENTRY_BY_VALUES, task_48[arg0], 2);
  if (local_FC != -1) {
    task_70[arg0] = 0;
  }
  return;
}

static void script_9EB2(int32_t arg0, float arg1, float arg2, float arg3) {
  Vector3 local_F4;
  local_F4.x = arg1;
  (&local_F4)->y = arg2;
  (&local_F4)->z = arg3;
  Script_EVT_SetPosition(arg0, POS_XYZ, &local_F4);
  return;
}

static void script_9EDA(int32_t arg0, float arg1, float arg2, float arg3) {
  Vector3i local_F4;
  local_F4.x = (int32_t)(arg1 / 360.0f * (float)0x10000);
  (&local_F4)->y = (int32_t)(arg2 / 360.0f * (float)0x10000);
  (&local_F4)->z = (int32_t)(arg3 / 360.0f * (float)0x10000);
  Script_EVT_SetRotation(arg0, 0x38000000, &local_F4, 0);
  return;
}

static void script_9F2E(int32_t arg0, int32_t arg1, int32_t arg2) {
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

static int32_t script_9F7E(int32_t arg0) {
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

static void script_A025(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  local_FC = script_9F7E(arg0);
  if (arg1 > local_FC) {
    (void)0;
  } else {
    (void)-1;
  }
  return;
}

static int32_t script_A048(int32_t arg0) {
  int32_t unused_FC;
  GameDateTime local_F4;
  int32_t local_F0;
  int32_t local_EC;
  Script_ReturnTimeDate(&local_F4);
  local_F0 = (&local_F4)->hours * 60 + (&local_F4)->minutes;
  local_EC = Script_scenUtils(SCENARIO_CMD_GET_STEP_LUT, arg0) * 6;
  return local_F0 - local_EC;
}

static void script_A07B(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  local_FC = script_A048(arg0);
  if (arg1 > local_FC) {
    (void)0;
  } else {
    (void)-1;
  }
  return;
}

static void script_A09E(int32_t arg0, int32_t arg1) {
  int8_t local_F4[12];
  int32_t local_F0;
  local_F0 = script_A048(arg0);
  local_F4[4] = local_F0 / 60;
  local_F4[5] = local_F0 % 60;
  if ((local_F4[4] < *(int8_t *)(arg1 + 4)) | ((local_F4[4] == *(int8_t *)(arg1 + 4)) & (local_F4[5] < *(int8_t *)(arg1 + 5)))) {
    (void)0;
  } else {
    (void)-1;
  }
  return;
}

static void script_A0FD(int32_t arg0) {
  int32_t local_F8[2];
  local_F8[0] = arg0;
  local_F8[1] = 0;
  (void)local_F8;
  return;
}

static void script_A112(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
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

static void script_A4A8(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
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

__attribute__((scn_string_bindings(0, data_1DDD8)))
static void script_AB36(void) {
  Script_sprintf("TUNED FreeTalk def\012");
  SCNT_Enqueue();
  return;
}

__attribute__((scn_string_bindings(0, data_1DDEC)))
static void script_AB45(void) {
  Script_sprintf("TUNED FreeTalk def\012");
  SCNT_Enqueue();
  return;
}

static void script_AB54(void) {
  return;
}

static void script_AB57(void) {
  return;
}

static void script_AB5A(void) {
  return;
}

static void script_AB5D(void) {
  return;
}

static void script_AB60(void) {
  (void)((task_1AC = 0) & 0x20);
  return;
}

static void script_AB6D(void) {
  task_1A0 = Script_freeTalkGetCurrentChara();
  return;
}

static void script_AB79(void) {
  *(&task_1A0 + 1) = Script_GetFreeTalkStatus();
  return;
}

static int32_t script_AB88(void) {
  return *(&task_1A0 + 2) = Script_freeTalkGetAndResetState();
}

static void script_AB97(void) {
  int32_t local_FC;
  local_FC = Script_GetSceneID();
  switch (local_FC) {
    case 1:
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JHD0_ENTRY2_TIMED_EVENT_DONE) == 0) {
        Script_TimedTransitionUtils(TIMED_TRANSITION_REGISTER, 1, 0x3044484A, 2, &data_1DE70, 0xD5A6C3, TimedTransitionUpdateMemoItem150);
      }
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JD00_ENTRY20_TIMED_EVENT_DONE) == 0) {
        Script_TimedTransitionUtils(TIMED_TRANSITION_REGISTER, 1, 0x3030444A, 20, &data_1DE38, 0xDA3AA3, 0);
      }
      break;
    case 2:
      if ((Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_GMCT_ROUTE_STARTED) == 0) | (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_GMCT_ROUTE_ADVANCED) == 0)) {
        Script_TimedTransitionUtils(TIMED_TRANSITION_REGISTER, 2, 0x54434D47, 1, &data_1DE00, 0xB7420F, 0);
      }
      break;
    case 3:
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOZOMI_RESCUE_RUSH_ACTIVE) == 0) {
        Script_TimedTransitionUtils(TIMED_TRANSITION_REGISTER, 3, 0x4F4D4F4A, 20, &data_1DE1C, 0x2DC9FE, TimedTransitionRollbackToPreviousDay2330);
      }
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_NOTE_FIRED_FROM_JOB) & ~Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, 970)) {
        if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_MFSY_ENTRY42_TIMED_EVENT_DONE) == 0) {
          Script_TimedTransitionUtils(TIMED_TRANSITION_REGISTER, 3, 0x5953464D, 42, &data_1DE54, 0xBBB817, 0);
        }
      }
      break;
  }
  return;
}

static void script_ACCF(int32_t arg0) {
  script_ACE7(arg0);
  return;
}

static void script_ACDB(int32_t arg0) {
  script_ACE7(arg0);
  return;
}

static void script_ACE7(int32_t arg0) {
  if (Script_StepUtils(GET_STEP_HIGHER, 0) == 22) {
    int32_t local_FC;
    local_FC = Script_StepUtils(GET_STEP_LOWER, 0);
    if ((local_FC == 2) | (local_FC == 3)) {
      Script_SetShouldSleepFlag(0);
      goto L00AD20;
    }
  }
  L00AD20:;
  Script_ryoSetChara(arg0);
  Script_facemotUtils(arg0, 1, 0);
  Script_facemotUtils(arg0, 0, 0);
  Script_FaceEx(arg0, 0, 0, 2);
  Script_CharHandlers(arg0, -1, 0, 0);
  script_AB97();
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

static void script_ADE3(int32_t arg0) {
  script_ACDB(arg0);
  for (;;) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_ADFA(int32_t arg0) {
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

static void script_AE3B(int32_t arg0) {
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

static void script_AE7F(int32_t arg0) {
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

static void script_AEFD(void) {
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_PROGRESS_STEP_28_REACHED)) {
    script_AE7F(40);
  } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_PROGRESS_STEP_1E_REACHED)) {
    script_AE7F(30);
  } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_PROGRESS_STEP_14_REACHED)) {
    script_AE7F(20);
  } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_JU00_CATA1_NBO_BRANCH_CHOSEN)) {
    script_AE7F(10);
  } else if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_PROGRESS_STEP05_SA1116_DOJO_DONE)) {
    script_AE7F(5);
  } else {
    script_AE7F(0);
  }
  return;
}

static void script_AF7C(int32_t arg0) {
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

static int32_t script_AFA8(int32_t arg0, int32_t arg1) {
  int8_t local_FF;
  int32_t local_F8;
  (void)((local_FF = 0) & 4);
  local_F8 = Script_shenGetStats(STATS_CASH);
  if (local_F8 - arg1 >= arg0) {
    (void)((local_FF = -1) & 4);
  }
  return local_FF & 0x80;
}

static int8_t script_AFDB(int32_t arg0, int32_t arg1, int32_t arg2) {
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
      goto L00B049;
    }
  }
  L00B049:;
  local_F8 = local_F8 + local_F4;
  if ((Script_StepUtils(GET_STEP_HIGHER, 0) == 17) & (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_D000_VENDING_LOCK_CLEARED) == 0)) {
    local_F8 = local_F8 + 69000;
    (void)((local_F3 = 0) & 1);
  } else if ((local_EC >= 2202) & (local_EC <= 2204)) {
    (void)((local_F3 = 0) & 1);
    local_F8 = local_F4 = 0;
    local_FF = 4;
  } else {
    (void)((local_F3 = -(script_AFA8(local_F8, arg1) != 0)) & 0x10);
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
      script_B11D(local_FF, arg0);
    }
  }
  return local_FF;
}

__attribute__((scn_string_bindings(0, data_1DEC0, 8, data_1DF04, 9, data_1DF0D, 10, data_1DF16, 11, data_1DF1F, 12, data_1DF28, 13, data_1DF31)))
static void script_B11D(int32_t arg0, int32_t arg1) {
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

__attribute__((scn_string_bindings(0, data_1DF36, 1, data_1DF3B, 2, data_1DF44, 3, data_1DF49, 4, data_1DF4E, 5, data_1DF57, 6, data_1DF60, 7, data_1DF69)))
static void script_B2E6(int32_t arg0, int32_t arg1) {
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
  (void)local_FE;
  return;
}

static void script_B4BC(int32_t arg0, int32_t arg1) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t local_E4;
  task_1E8 = arg1;
  task_1EC = arg0;
  local_E4 = task_1E8;
  switch (local_E4) {
    case 0:
      task_1F4 = 1;
      break;
    case 1:
      task_1F4 = 1;
      break;
    case 2:
      task_1F4 = 0;
      break;
    case 3:
      task_1F4 = 0;
      break;
    case 4:
      task_1F4 = 0;
      break;
    case 5:
      task_1F4 = 0;
      break;
  }
  Script_ccowUtils(SHEN_NAME('JBOX'), 2);
  Script_MoveObjectToObject(SHEN_NAME('JCNT'), 0x584F424A);
  Script_MoveObjectToObject(SHEN_NAME('ASTD'), 0x584F424A);
  SCNF_WaitFrames_0000(1);
  switch (task_1EC) {
    case 0:
      {
        {
          Vector3 local_D8;
          local_D8.x = -0.461f;
          (&local_D8)->z = 0.566f;
          Script_EVT_SetPosition(SHEN_NAME('JCNT'), 0x28000000, &local_D8);
        }
        {
          Vector3 local_D8;
          local_D8.x = -0.225f;
          (&local_D8)->z = 1.138f;
          Script_EVT_SetPosition(SHEN_NAME('ASTD'), 0x28000000, &local_D8);
        }
        {
          Vector3i local_D8;
          (&local_D8)->y = 0;
          Script_EVT_SetRotation(SHEN_NAME('JCNT'), MATRIX_XFORM_Y, &local_D8, 0);
        }
        {
          Vector3i local_D8;
          (&local_D8)->y = 0;
          Script_EVT_SetRotation(SHEN_NAME('ASTD'), MATRIX_XFORM_Y, &local_D8, 0);
        }
        break;
      }
    case 1:
      {
        {
          Vector3 local_D8;
          local_D8.x = -0.461f;
          (&local_D8)->z = 0.566f;
          Script_EVT_SetPosition(SHEN_NAME('JCNT'), 0x28000000, &local_D8);
        }
        {
          Vector3 local_D8;
          local_D8.x = -0.225f;
          (&local_D8)->z = 1.138f;
          Script_EVT_SetPosition(SHEN_NAME('ASTD'), 0x28000000, &local_D8);
        }
        {
          Vector3i local_D8;
          (&local_D8)->y = 0;
          Script_EVT_SetRotation(SHEN_NAME('JCNT'), MATRIX_XFORM_Y, &local_D8, 0);
        }
        {
          Vector3i local_D8;
          (&local_D8)->y = 0;
          Script_EVT_SetRotation(SHEN_NAME('ASTD'), MATRIX_XFORM_Y, &local_D8, 0);
        }
        break;
      }
    case 2:
      {
        {
          Vector3 local_D8;
          local_D8.x = -0.461f;
          (&local_D8)->z = 0.566f;
          Script_EVT_SetPosition(SHEN_NAME('JCNT'), 0x28000000, &local_D8);
        }
        {
          Vector3 local_D8;
          local_D8.x = -0.225f;
          (&local_D8)->z = 1.138f;
          Script_EVT_SetPosition(SHEN_NAME('ASTD'), 0x28000000, &local_D8);
        }
        {
          Vector3i local_D8;
          (&local_D8)->y = 0;
          Script_EVT_SetRotation(SHEN_NAME('JCNT'), MATRIX_XFORM_Y, &local_D8, 0);
        }
        {
          Vector3i local_D8;
          (&local_D8)->y = 0;
          Script_EVT_SetRotation(SHEN_NAME('ASTD'), MATRIX_XFORM_Y, &local_D8, 0);
        }
        break;
      }
  }
  Script_LoadVoiceFile(1, "E1067");
  Script_scenToggleVisibleFor(SHEN_NAME('YKUR'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('D_TE'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('ASTD'), 0);
  Script_facemotUtils(CHARA_AKIR, 1, 0);
  Script_facemotUtils(SHEN_NAME('YKUR'), 1, 0);
  SCNF_WaitFrames_0000(1);
  Script_charaDisplay(SHEN_NAME('YKUR'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('D_TE'), CDM_Hide);
  Script_charaPosRotUtils(SHEN_NAME('JBOX'), 161, &data_1DEA8, 0, 3);
  if (task_1F4 == 1) {
    Script_charaPosRotUtils(SHEN_NAME('JBOX'), 156, &local_F4, &local_E8, 2);
    (&local_E8)->y = (&local_E8)->y - 3094;
    Script_charaPosRotUtils(SHEN_NAME('JBOX'), 156, &local_F4, &local_E8, 0);
    Script_charaPosRotUtils(SHEN_NAME('JBOX'), 156, &local_F4, &local_E8, 2);
    local_E8.x = (&local_E8)->x + 236;
    Script_charaPosRotUtils(SHEN_NAME('JBOX'), 156, &local_F4, &local_E8, 0);
  }
  task_1CC = Script_scenCreateTask(script_D1C1, 0);
  task_1D4 = Script_scenCreateTask(script_D21F, 0);
  if (task_1F4 == 1) {
    task_1BB = 1;
    task_1BD = 1;
  }
  (void)((task_1F6 = 0) & 1);
  return;
}

static void script_B850(void) {
  int8_t local_FF;
  local_FF = 100;
  for (;;) {
    if (Script_GetInputData(1, 0) == 0x1000) {
      local_FF = local_FF + 1;
      if (local_FF >= 126) {
        local_FF = 126;
      }
      Script_executeAicaCommand(1184, 2, local_FF);
    }
    if (Script_GetInputData(1, 0) == 0x2000) {
      local_FF = local_FF - 1;
      if (local_FF <= 0) {
        local_FF = 0;
      }
      Script_executeAicaCommand(1184, 2, local_FF);
    }
    if (Script_GetInputData(1, 0) == 0x8000) {
      Script_executeAicaCommand(171, 0, 0);
    }
    if (Script_GetInputData(1, 0) == 0x4000) {
      Script_executeAicaCommand(AICA_CMD_PLAY, 0, 0);
    }
    Script_PrintDebugString(2, 15, "Volume = %3d", local_FF);
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_B917(void) {
  int8_t local_FF;
  (void)((local_FF = 0) & 2);
  for (;;) {
    if (script_D77C()) {
      if (Script_GetControllerInput(1) == 0x400) {
        (void)((local_FF = 0) & 2);
        script_BA57();
      }
      if (local_FF == 0) {
        (void)((local_FF = -1) & 2);
        Script_sicnUtils(0, 5, 1);
        Script_uiUtils(0, 7, 0);
      }
    } else {
      if (local_FF == -1) {
        (void)((local_FF = 0) & 2);
        Script_sicnUtils(1, 0, 0);
        Script_uiUtils(0, 56, 0);
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_srchControl(0);
  return;
}

static void script_B9AC(float arg0, float arg1, float arg2, int32_t arg3) {
  Vector3 local_F4;
  local_F4.x = arg0;
  (&local_F4)->y = arg1;
  (&local_F4)->z = arg2;
  Script_scenCharaSubmitMoveTargetRequest(CHARA_AKIR, &local_F4, arg3, MN_AKI_AKI_STAND_LP, 0);
  while (Script_IsMoveTargetRequestActive(CHARA_AKIR) != 0) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_B9F6(int32_t arg0) {
  for (;;) {
    if (Script_IsCharaMotionFinished(arg0)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static int8_t script_BA10(void) {
  int8_t local_FF;
  local_FF = script_AFDB(2, 100, 1);
  return local_FF;
}

static int8_t script_BA28(void) {
  int8_t local_FF;
  uint8_t unused_F8[7];
  GameDateTime local_F0;
  local_FF = 0;
  Script_ReturnTimeDate(&local_F0);
  if (((&local_F0)->hours >= 21) & ((&local_F0)->minutes >= 45)) {
    local_FF = 1;
  }
  return local_FF;
}

__attribute__((scn_string_bindings(0, data_1DF94, 9, data_1DFEE, 10, data_1DFF8, 11, data_1E002, 13, data_1E016)))
static void script_BA57(void) {
  int32_t local_FC;
  int32_t local_F8;
  Vector3 local_EC;
  if (script_D86D(0) == 0) {
    if (~task_1F6) {
      (void)((task_1F6 = -1) & 8);
      Script_ShowUIMessageFromID("E1067A011");
      while (Script_convGetIsActive(0)) {
        SCNF_WaitFrames_0000(1);
      }
    }
  } else {
    Script_uiUtils(0, 7, 0);
    script_BFF8();
    if (task_1E8 == 2) {
      Script_SetCharaRefBoardPos(CHARA_AKIR, &data_1DF84);
    }
    Script_charaSetRefboardFlagTo(CHARA_AKIR, 1);
    Script_scenUtils(SCENARIO_CMD_INIT_SCNT);
    Script_EVT_GetPosition(SHEN_NAME('JBOX'), -1, &local_EC, ENTITY_XFORM_WORLD_SPACE);
    Script_SetCharaLookAtPosition(CHARA_AKIR, 1, &local_EC, 1);
    L00BAF9:;
    Script_seqEnterEvtCam(160, SHEN_NAME('JBOX'), SHEN_NAME('JBOX'));
    {
      int32_t unused_E0[3];
      int32_t local_DC;
      int32_t local_D8;
      local_DC = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f));
      local_D8 = local_DC;
      if (local_D8 == 0) {
        Script_ShowUIMessageFromID("E1067A001");
        while (Script_convGetIsActive(0)) {
          SCNF_WaitFrames_0000(1);
        }
      } else {
        Script_ShowUIMessageFromID("E1067A002");
        while (Script_convGetIsActive(0)) {
          SCNF_WaitFrames_0000(1);
        }
      }
    }
    {
      int32_t unused_E0[3];
      int32_t local_DC;
      int32_t local_D8;
      local_DC = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f));
      local_D8 = local_DC;
      if (local_D8 == 0) {
        Script_ShowUIMessageFromID("E1067A003");
      } else if (local_D8 == 1) {
        Script_ShowUIMessageFromID("E1067A004");
      } else {
        Script_ShowUIMessageFromID("E1067A005");
      }
    }
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    local_FC = SCNF_RequestActionSelectorFromParams_0008(0xFFFF, 46, 47, 0xFFFF, 450, 0xFFFF, ACTION_SELECTOR_MODE_STANDARD_TIMEOUT_CANCEL);
    if (local_FC == 46) {
      if (script_BA10() < 2) {
        if (Script_shenGetStats(STATS_CASH) < 100) {
          int32_t unused_DC[4];
          int32_t local_D8;
          local_D8 = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f));
          if (local_D8 == 0) {
            Script_ShowUIMessageFromID("E1067A006");
          } else if (local_D8 == 1) {
            Script_ShowUIMessageFromID("E1067A007");
          } else {
            Script_ShowUIMessageFromID("E1067A008");
          }
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
          goto L00BEB3;
        }
        if (script_BA28()) {
          int32_t unused_E0[3];
          int32_t local_DC;
          int32_t local_D8;
          local_DC = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f));
          local_D8 = local_DC;
          if (local_D8 == 0) {
            Script_ShowUIMessageFromID("E1067A009");
          } else {
            Script_ShowUIMessageFromID("E1067A011");
          }
          while (Script_convGetIsActive(0)) {
            SCNF_WaitFrames_0000(1);
          }
          goto L00BEB3;
        }
        Script_ResetHandMotion(CHARA_AKIR, 1, 1, 0);
        Script_ResetHandMotion(SHEN_NAME('YKUR'), 1, 1, 0);
        Script_ResetHandMotion(CHARA_AKIR, 1, 1, 851);
        Script_shenClearFreeState(0);
        SCNF_WaitFrames_0000(1);
        Script_seqEnterEvtCam(170, SHEN_NAME('JBOX'), SHEN_NAME('JBOX'));
        Script_SetCharaLookAtPosition(CHARA_AKIR, -98312, 0, 0);
        Script_PlayCharaMotion(CHARA_AKIR, 24577, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        SCNF_WaitFrames_0000(60);
        Script_EVT_GetPosition(SHEN_NAME('JBOX'), -1, &local_EC, ENTITY_XFORM_WORLD_SPACE);
        Script_SetCharaLookAtPosition(CHARA_AKIR, 1, &local_EC, 1);
        SCNF_WaitFrames_0000(40);
        Script_executeAicaCommand(263337, 0, 0);
        script_B9F6(CHARA_AKIR);
        Script_EVT_GetPosition(SHEN_NAME('D_TE'), -1, &local_EC, ENTITY_XFORM_WORLD_SPACE);
        Script_SetLookPointDetails(CHARA_AKIR, 11, &local_EC, 1);
        local_F8 = Script_shenGetStats(STATS_CASH);
        SCNF_SetCashValue(-100, 1);
        local_F8 = local_F8 - 100;
        Script_SetStats(STATS_CASH, local_F8);
        Script_shenClearFreeState(1);
        Script_uiUtils(0, 7, 0);
        script_C178();
        task_1F5 = 1;
        goto L00BEB3;
      }
      goto L00BEB3;
    } else {
      int32_t unused_E0[3];
      int32_t local_DC;
      int32_t local_D8;
      local_DC = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f));
      local_D8 = local_DC;
      if (local_D8 == 0) {
        Script_ShowUIMessageFromID("E1067A009");
        while (Script_convGetIsActive(0)) {
          SCNF_WaitFrames_0000(1);
        }
      } else if (local_D8 == 1) {
        Script_ShowUIMessageFromID("E1067A010");
        while (Script_convGetIsActive(0)) {
          SCNF_WaitFrames_0000(1);
        }
      } else {
        Script_ShowUIMessageFromID("E1067A011");
        while (Script_convGetIsActive(0)) {
          SCNF_WaitFrames_0000(1);
        }
      }
      goto L00BEB3;
    }
    SCNF_WaitFrames_0000(1);
    goto L00BAF9;
    L00BEB3:;
    Script_SetCharaLookAtPosition(CHARA_AKIR, -98312, 0, 0);
    Script_MoveObjectToObject(SHEN_NAME('YKUR'), -1);
    Script_MoveObjectToObject(SHEN_NAME('D_TE'), -1);
    Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
    Script_scenToggleVisibleFor(SHEN_NAME('YKUR'), 0);
    Script_scenToggleVisibleFor(SHEN_NAME('D_TE'), 0);
    Script_scenToggleVisibleFor(SHEN_NAME('ASTD'), 0);
    Script_charaSetRefboardFlagTo(CHARA_AKIR, 0);
    Script_SetGameStateTo(AUTH_None);
    Script_shenClearFreeState(1);
    Script_uiUtils(0, 56, 0);
    Script_scenUtils(SCENARIO_CMD_UPDATE_TIMER);
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_EV_STOP_ACTIVE, 0);
    Script_ryoControl(RYO_RestoreControl);
  }
  return;
}

static void script_BF4B(const char *arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Script_LoadSND(3, 0, 0, 0);
  Script_LoadSND(1, arg2, arg0, 0x8000);
  task_1BB = 1;
  task_1BD = 1;
  while (Script_LoadSND(2, 0, 0, 0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_LoadSND(4, 0, 0, 0);
  task_1BC = 4;
  SCNF_WaitFrames_0000(20);
  Script_executeAicaCommand(132265, 0, 0);
  SCNF_WaitFrames_0000(40);
  Script_executeAicaCommand(16296, 0, 0);
  Script_executeAicaCommand(1184, arg2, arg3);
  Script_PlayerStatsUtils(PS_SetIndexedU8Table1Slot, arg1, Script_PlayerStatsUtils(PS_GetIndexedU8Table1Slot, arg1) + 1);
  task_1F4 = 1;
  return;
}

static void script_BFF8(void) {
  Vector3 local_F4;
  int32_t unused_D0[9];
  Vector3i local_C4;
  int32_t unused_BC[2];
  SCNF_WaitFrames_0000(1);
  Script_uiUtils(0, 7, 0);
  Script_sicnUtils(1, 0, 0);
  Script_scenToggleVisibleFor(SHEN_NAME('YKUR'), 1);
  Script_scenToggleVisibleFor(SHEN_NAME('D_TE'), 1);
  Script_scenToggleVisibleFor(SHEN_NAME('ASTD'), 1);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_FaceEx(CHARA_AKIR, 1, 0, 1);
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_MoveObjectToObject(SHEN_NAME('YKUR'), 0x584F424A);
  Script_MoveObjectToObject(SHEN_NAME('D_TE'), 0x52554B59);
  Script_EVT_GetPosition(SHEN_NAME('ASTD'), -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  Script_mtwkGetSetRotation(SHEN_NAME('ASTD'), &local_C4, 0x40000000);
  script_B9AC((&local_F4)->x, (&local_F4)->y, (&local_F4)->z, (&local_C4)->y);
  local_F4.x = -0.419f;
  (&local_F4)->y = 1.058f;
  (&local_F4)->z = 0.803f;
  local_C4.x = 20028;
  (&local_C4)->y = 64635;
  (&local_C4)->z = 30038;
  Script_EVT_SetPosition(SHEN_NAME('YKUR'), POS_XYZ, &local_F4);
  Script_EVT_SetRotation(SHEN_NAME('YKUR'), 0x38000000, &local_C4, 0);
  local_F4.x = 0.0f;
  (&local_F4)->y = 0.0f;
  (&local_F4)->z = 0.0f;
  Script_EVT_SetPosition(SHEN_NAME('D_TE'), POS_XYZ, &local_F4);
  Script_mtwkGetSetRotation(SHEN_NAME('JBOX'), &local_C4, 0x40000000);
  Script_EVT_SetRotation(SHEN_NAME('D_TE'), 0x78000000, &local_C4, 0);
  return;
}

static void script_C178(void) {
  L00C17A:;
  Script_charaDisplay(SHEN_NAME('YKUR'), CDM_Show);
  Script_charaDisplay(CHARA_AKIR, CDM_Hide);
  Script_sicnUtils(0, 37, 1);
  Script_seqEnterEvtCam(152, SHEN_NAME('D_TE'), SHEN_NAME('D_TE'));
  task_1F8 = 1;
  Script_SetNewHandMotion(SHEN_NAME('YKUR'), 1, 852, 0, 0, 0, 0, 1.0f, 0);
  Script_SetCameraFlags(-1);
  script_C214();
  Script_sicnUtils(1, 0, 0);
  Script_shenClearFreeState(0);
  Script_charaDisplay(SHEN_NAME('YKUR'), CDM_Hide);
  Script_charaDisplay(CHARA_AKIR, CDM_Show);
  goto L00C213;
  SCNF_WaitFrames_0000(1);
  goto L00C17A;
  L00C213:;
  return;
}

__attribute__((scn_string_bindings(0, data_1E020, 1, data_1E02B, 2, data_1E036, 3, data_1E041, 4, data_1E04C, 5, data_1E057, 6, data_1E062, 7, data_1E06D, 8, data_1E078, 9, data_1E083, 10, data_1E08E, 11, data_1E099, 12, data_1E0A4, 13, data_1E0AF, 14, data_1E0BA, 15, data_1E0C5, 16, data_1E0D0, 17, data_1E0DB, 18, data_1E0E6, 19, data_1E0F1, 24, data_1E128, 25, data_1E133, 26, data_1E13E, 27, data_1E149, 28, data_1E154, 29, data_1E15F, 34, data_1E196, 35, data_1E1A1, 36, data_1E1AC, 37, data_1E1B7, 38, data_1E1C2, 39, data_1E1CD, 44, data_1E204, 45, data_1E20F, 46, data_1E21A, 47, data_1E225, 48, data_1E230, 49, data_1E23B, 54, data_1E272, 55, data_1E27D, 56, data_1E288, 57, data_1E293, 58, data_1E29E, 59, data_1E2A9)))
static void script_C214(void) {
  Vector3 local_F4;
  int32_t unused_E8[3];
  int8_t local_E7;
  uint8_t unused_E4[3];
  int32_t local_E0;
  switch (task_1EC) {
    case 0:
      task_1F7 = 10;
      break;
    case 1:
      task_1F7 = 4;
      break;
    case 2:
      task_1F7 = 2;
      break;
  }
  for (;;) {
    if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_EV_STOP_ACTIVE)) {
      break;
    }
    if ((Script_GetControllerInput(0) == 0x8000) | (Script_GetControllerInput(0) == 0x2000) | (Script_GetControllerInput(0) == 0xA000)) {
      task_1AD = 0;
      task_1AE = 0;
      task_1AF = 0;
      local_E7 = 0;
      if (task_1EC == 0) {
        if (task_1F7 <= 19) {
          while (local_E7 < 16) {
            Script_EVT_GetPosition(SHEN_NAME('YKUR'), -1, &local_F4, 0);
            local_F4.x = (&local_F4)->x + 0.002f;
            Script_EVT_SetPosition(SHEN_NAME('YKUR'), POS_XYZ, &local_F4);
            local_E7 = local_E7 + 1;
            SCNF_WaitFrames_0000(1);
          }
          task_1F7 = task_1F7 + 1;
          if (task_1F7 >= 20) {
            task_1F7 = 20;
          }
        }
      } else {
        if (task_1F7 <= 9) {
          while (local_E7 < 16) {
            Script_EVT_GetPosition(SHEN_NAME('YKUR'), -1, &local_F4, 0);
            local_F4.x = (&local_F4)->x + 0.002f;
            Script_EVT_SetPosition(SHEN_NAME('YKUR'), POS_XYZ, &local_F4);
            local_E7 = local_E7 + 1;
            SCNF_WaitFrames_0000(1);
          }
          task_1F7 = task_1F7 + 1;
          if (task_1F7 >= 10) {
            task_1F7 = 10;
          }
        }
      }
    }
    if ((Script_GetControllerInput(0) == 0x4000) | (Script_GetControllerInput(0) == 0x1000) | (Script_GetControllerInput(0) == 0x5000)) {
      task_1AD = 0;
      task_1AE = 0;
      task_1AF = 0;
      local_E7 = 0;
      if (task_1F7 >= 2) {
        while (local_E7 < 16) {
          Script_EVT_GetPosition(SHEN_NAME('YKUR'), -1, &local_F4, 0);
          local_F4.x = (&local_F4)->x - 0.002f;
          Script_EVT_SetPosition(SHEN_NAME('YKUR'), POS_XYZ, &local_F4);
          local_E7 = local_E7 + 1;
          SCNF_WaitFrames_0000(1);
        }
        task_1F7 = task_1F7 - 1;
        if (task_1F7 <= 1) {
          task_1F7 = 1;
        }
      }
    }
    if (Script_GetControllerInput(1) == 0x400) {
      task_1AD = 0;
      task_1AE = 0;
      task_1AF = 0;
      if (task_1F7 >= 11) {
        script_CD49();
      } else {
        Script_shenClearFreeState(0);
        Script_sicnUtils(1, 0, 0);
        Script_SetCameraFlags(0);
        Script_seqEnterEvtCam(180, SHEN_NAME('JBOX'), SHEN_NAME('JBOX'));
        local_E7 = 0;
        while (local_E7 < 10) {
          Script_EVT_GetPosition(SHEN_NAME('YKUR'), -1, &local_F4, 0);
          (&local_F4)->y = (&local_F4)->y - 0.0015f;
          Script_EVT_SetPosition(SHEN_NAME('YKUR'), POS_XYZ, &local_F4);
          local_E7 = local_E7 + 1;
          script_CD4C(1);
          SCNF_WaitFrames_0000(1);
        }
        Script_executeAicaCommand(1193, 0, 0);
        local_E7 = 0;
        while (local_E7 < 10) {
          Script_EVT_GetPosition(SHEN_NAME('YKUR'), -1, &local_F4, 0);
          (&local_F4)->y = (&local_F4)->y + 0.0015f;
          Script_EVT_SetPosition(SHEN_NAME('YKUR'), POS_XYZ, &local_F4);
          local_E7 = local_E7 + 1;
          script_CD4C(0);
          SCNF_WaitFrames_0000(1);
        }
        if (task_1F0 != 0) {
          Script_srchControl((void *)task_1F0);
        }
        Script_executeAicaCommand(197801, 0, 0);
        task_1E4 = 0;
        script_CDFE();
        for (;;) {
          if (task_1E4 == 1) {
            break;
          }
          SCNF_WaitFrames_0000(1);
        }
        local_E0 = task_1E8;
        switch (local_E0) {
          case 0:
            local_E0 = task_1F7;
            switch (local_E0) {
              case 1:
                script_BF4B("ykm003.snd", 0, 2, 100);
                break;
              case 2:
                script_BF4B("ykm002.snd", 1, 2, 100);
                break;
              case 3:
                script_BF4B("bgm045.snd", 2, 2, 100);
                break;
              case 4:
                script_BF4B("tsm002.snd", 3, 2, 100);
                break;
              case 5:
                script_BF4B("bgm015.snd", 20, 2, 100);
                break;
              case 6:
                script_BF4B("bgm016.snd", 21, 2, 100);
                break;
              case 7:
                script_BF4B("bgm014.snd", 22, 2, 100);
                break;
              case 8:
                script_BF4B("bgm047.snd", 23, 2, 100);
                break;
              case 9:
                script_BF4B("bgm075.snd", 24, 2, 100);
                break;
              case 10:
                script_BF4B("bgm010.snd", 25, 2, 100);
                break;
            }
            break;
          case 1:
            local_E0 = task_1F7;
            switch (local_E0) {
              case 1:
                script_BF4B("ykm003.snd", 0, 2, 100);
                break;
              case 2:
                script_BF4B("ykm002.snd", 1, 2, 100);
                break;
              case 3:
                script_BF4B("bgm045.snd", 2, 2, 100);
                break;
              case 4:
                script_BF4B("tsm002.snd", 3, 2, 100);
                break;
              case 5:
                script_BF4B("bgm015.snd", 20, 2, 100);
                break;
              case 6:
                script_BF4B("bgm016.snd", 21, 2, 100);
                break;
              case 7:
                script_BF4B("bgm014.snd", 22, 2, 100);
                break;
              case 8:
                script_BF4B("bgm047.snd", 23, 2, 100);
                break;
              case 9:
                script_BF4B("bgm075.snd", 24, 2, 100);
                break;
              case 10:
                script_BF4B("bgm010.snd", 25, 2, 100);
                break;
            }
            break;
          case 2:
            local_E0 = task_1F7;
            switch (local_E0) {
              case 1:
                script_BF4B("bgm061.snd", 4, 2, 100);
                break;
              case 2:
                script_BF4B("bgm058.snd", 5, 2, 100);
                break;
              case 3:
                script_BF4B("bgm057.snd", 6, 2, 100);
                break;
              case 4:
                script_BF4B("bgm056.snd", 7, 2, 100);
                break;
              case 5:
                script_BF4B("bgm015.snd", 20, 2, 100);
                break;
              case 6:
                script_BF4B("bgm016.snd", 21, 2, 100);
                break;
              case 7:
                script_BF4B("bgm014.snd", 22, 2, 100);
                break;
              case 8:
                script_BF4B("bgm047.snd", 23, 2, 100);
                break;
              case 9:
                script_BF4B("bgm075.snd", 24, 2, 100);
                break;
              case 10:
                script_BF4B("bgm010.snd", 25, 2, 100);
                break;
            }
            break;
          case 3:
            local_E0 = task_1F7;
            switch (local_E0) {
              case 1:
                script_BF4B("bgm125.snd", 8, 2, 100);
                break;
              case 2:
                script_BF4B("bgm074.snd", 9, 2, 100);
                break;
              case 3:
                script_BF4B("bgm053.snd", 10, 2, 100);
                break;
              case 4:
                script_BF4B("bgm124.snd", 11, 2, 100);
                break;
              case 5:
                script_BF4B("bgm015.snd", 20, 2, 100);
                break;
              case 6:
                script_BF4B("bgm016.snd", 21, 2, 100);
                break;
              case 7:
                script_BF4B("bgm014.snd", 22, 2, 100);
                break;
              case 8:
                script_BF4B("bgm047.snd", 23, 2, 100);
                break;
              case 9:
                script_BF4B("bgm075.snd", 24, 2, 100);
                break;
              case 10:
                script_BF4B("bgm010.snd", 25, 2, 100);
                break;
            }
            break;
          case 4:
            local_E0 = task_1F7;
            switch (local_E0) {
              case 1:
                script_BF4B("bgm054.snd", 12, 2, 100);
                break;
              case 2:
                script_BF4B("bgm092.snd", 13, 2, 100);
                break;
              case 3:
                script_BF4B("bgm086.snd", 14, 2, 100);
                break;
              case 4:
                script_BF4B("bgm048.snd", 15, 2, 100);
                break;
              case 5:
                script_BF4B("bgm015.snd", 20, 2, 100);
                break;
              case 6:
                script_BF4B("bgm016.snd", 21, 2, 100);
                break;
              case 7:
                script_BF4B("bgm014.snd", 22, 2, 100);
                break;
              case 8:
                script_BF4B("bgm047.snd", 23, 2, 100);
                break;
              case 9:
                script_BF4B("bgm075.snd", 24, 2, 100);
                break;
              case 10:
                script_BF4B("bgm010.snd", 25, 2, 100);
                break;
            }
            break;
          case 5:
            local_E0 = task_1F7;
            switch (local_E0) {
              case 1:
                script_BF4B("bgm021.snd", 16, 2, 100);
                break;
              case 2:
                script_BF4B("bgm060.snd", 17, 2, 100);
                break;
              case 3:
                script_BF4B("bgm090.snd", 18, 2, 100);
                break;
              case 4:
                script_BF4B("bgm072.snd", 19, 2, 100);
                break;
              case 5:
                script_BF4B("bgm015.snd", 20, 2, 100);
                break;
              case 6:
                script_BF4B("bgm016.snd", 21, 2, 100);
                break;
              case 7:
                script_BF4B("bgm014.snd", 22, 2, 100);
                break;
              case 8:
                script_BF4B("bgm047.snd", 23, 2, 100);
                break;
              case 9:
                script_BF4B("bgm075.snd", 24, 2, 100);
                break;
              case 10:
                script_BF4B("bgm010.snd", 25, 2, 100);
                break;
            }
            break;
        }
        SCNF_WaitFrames_0000(40);
        script_CECC();
        break;
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_CD49(void) {
  return;
}

static void script_CD4C(int32_t arg0) {
  Vector3 local_F4;
  Vector3i local_E8;
  float local_E4;
  float local_E0;
  int32_t local_DC;
  local_E4 = -0.0015f;
  local_E0 = -0.00069f;
  local_DC = task_1F7 - 1;
  if ((local_DC > 20) | (local_DC < 0)) {
  } else {
    Script_charaPosRotUtils(SHEN_NAME('JBOX'), *(int16_t *)((int8_t *)&data_1E2B4 + (local_DC << 1)), &local_F4, &local_E8, 2);
    if (arg0) {
      (&local_F4)->y = (&local_F4)->y + local_E4;
      (&local_F4)->z = (&local_F4)->z + local_E0;
    } else {
      (&local_F4)->y = (&local_F4)->y - local_E4;
      (&local_F4)->z = (&local_F4)->z - local_E0;
    }
    Script_charaPosRotUtils(SHEN_NAME('JBOX'), *(int16_t *)((int8_t *)&data_1E2B4 + (local_DC << 1)), &local_F4, &local_E8, 0);
  }
  return;
}

static void script_CDFE(void) {
  Script_executeAicaCommand(0x1200A0, 2, 0);
  task_1B9 = 0;
  task_1BA = 0;
  task_1BB = 0;
  task_1BD = 0;
  task_1C4 = Script_scenCreateTask(script_CFCA, 0);
  task_1C8 = Script_scenCreateTask(script_D0D5, 0);
  task_1D0 = Script_scenCreateTask(script_D283, 0);
  task_1D8 = Script_scenCreateTask(script_D46E, 0);
  task_1DC = Script_scenCreateTask(script_D514, 0);
  task_1E0 = Script_scenCreateTask(script_D5A0, 0);
  if (task_1F4 == 1) {
    task_1BC = 1;
  } else {
    task_1B9 = 1;
    Script_executeAicaCommand(394409, 0, 0);
    task_1BA = 1;
    Script_executeAicaCommand(66729, 0, 0);
  }
  return;
}

static void script_CECC(void) {
  Script_srchControl((void *)task_1C4);
  Script_srchControl((void *)task_1C8);
  Script_srchControl((void *)task_1D0);
  Script_srchControl((void *)task_1D8);
  Script_srchControl((void *)task_1DC);
  Script_srchControl((void *)task_1E0);
  {
    Vector3 local_F4;
    Vector3i local_E8;
    Script_charaPosRotUtils(SHEN_NAME('JBOX'), 159, &local_F4, &local_E8, 2);
    (&local_E8)->z = (&local_E8)->z + 5825;
    Script_charaPosRotUtils(SHEN_NAME('JBOX'), 159, &local_F4, &local_E8, 0);
  }
  {
    Vector3 local_F4;
    Vector3i local_E8;
    Script_charaPosRotUtils(SHEN_NAME('JBOX'), 160, &local_F4, &local_E8, 2);
    (&local_E8)->z = (&local_E8)->z + 5825;
    Script_charaPosRotUtils(SHEN_NAME('JBOX'), 160, &local_F4, &local_E8, 0);
  }
  {
    Vector3 local_F4;
    Vector3i local_E8;
    Script_charaPosRotUtils(SHEN_NAME('JBOX'), 161, &local_F4, &local_E8, 2);
    (&local_E8)->z = (&local_E8)->z + 5825;
    Script_charaPosRotUtils(SHEN_NAME('JBOX'), 161, &local_F4, &local_E8, 0);
  }
  Script_charaPosRotUtils(SHEN_NAME('JBOX'), 161, &data_1DEA8, 0, 3);
  Script_charaPosRotUtils(SHEN_NAME('JBOX'), 160, &data_1DEB4, 0, 3);
  return;
}

static int32_t script_CFCA(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t local_E4;
  int32_t local_E0;
  local_E4 = 0;
  for (;;) {
    local_E0 = task_1B9;
    switch (local_E0) {
      case 1:
        if (local_E4 < 90) {
          Script_charaPosRotUtils(SHEN_NAME('JBOX'), 153, &local_F4, &local_E8, 2);
          local_E8.x = (&local_E8)->x - 364;
          Script_charaPosRotUtils(SHEN_NAME('JBOX'), 153, &local_F4, &local_E8, 0);
          local_E4 = local_E4 + 1;
        } else {
          local_E4 = 0;
          task_1BE = 1;
          Script_executeAicaCommand(525481, 0, 0);
          task_1BF = 1;
          task_1C0 = 1;
          task_1B9 = 0;
        }
        break;
      case 2:
        if (local_E4 < 90) {
          if (local_E4 == 44) {
            task_1BA = 2;
          }
          Script_charaPosRotUtils(SHEN_NAME('JBOX'), 153, &local_F4, &local_E8, 2);
          local_E8.x = (&local_E8)->x + 364;
          Script_charaPosRotUtils(SHEN_NAME('JBOX'), 153, &local_F4, &local_E8, 0);
          local_E4 = local_E4 + 1;
        } else {
          local_E4 = 0;
          task_1B9 = 0;
        }
        break;
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_D0D5(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t local_E4;
  int32_t local_E0;
  local_E4 = 0;
  for (;;) {
    local_E0 = task_1BA;
    switch (local_E0) {
      case 1:
        if (local_E4 < 45) {
          Script_charaPosRotUtils(SHEN_NAME('JBOX'), 154, &local_F4, &local_E8, 2);
          (&local_E8)->z = (&local_E8)->z + 364;
          Script_charaPosRotUtils(SHEN_NAME('JBOX'), 154, &local_F4, &local_E8, 0);
          local_E4 = local_E4 + 1;
        } else {
          local_E4 = 0;
          task_1BA = 0;
        }
        break;
      case 2:
        if (local_E4 < 45) {
          Script_charaPosRotUtils(SHEN_NAME('JBOX'), 154, &local_F4, &local_E8, 2);
          (&local_E8)->z = (&local_E8)->z - 364;
          Script_charaPosRotUtils(SHEN_NAME('JBOX'), 154, &local_F4, &local_E8, 0);
          local_E4 = local_E4 + 1;
        } else {
          local_E4 = 0;
          task_1BC = 3;
          task_1BA = 0;
        }
        break;
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_D1C1(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t unused_E4;
  int32_t local_E0;
  for (;;) {
    local_E0 = task_1BB;
    switch (local_E0) {
      case 1:
        Script_charaPosRotUtils(SHEN_NAME('JBOX'), 155, &local_F4, &local_E8, 2);
        local_E8.x = (&local_E8)->x - 1601;
        Script_charaPosRotUtils(SHEN_NAME('JBOX'), 155, &local_F4, &local_E8, 0);
        break;
      case 2:
        break;
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_D21F(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t unused_E4;
  int32_t local_E0;
  for (;;) {
    local_E0 = task_1BD;
    switch (local_E0) {
      case 1:
        Script_charaPosRotUtils(SHEN_NAME('JBOX'), 157, &local_F4, &local_E8, 2);
        (&local_E8)->y = (&local_E8)->y - 1601;
        Script_charaPosRotUtils(SHEN_NAME('JBOX'), 157, &local_F4, &local_E8, 0);
        break;
      case 2:
        break;
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_D283(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t local_E4;
  int32_t local_E0;
  local_E4 = 0;
  for (;;) {
    local_E0 = task_1BC;
    switch (local_E0) {
      case 1:
        task_1BB = 0;
        task_1BD = 0;
        if (local_E4 < 13) {
          Script_charaPosRotUtils(SHEN_NAME('JBOX'), 156, &local_F4, &local_E8, 2);
          local_E8.x = (&local_E8)->x - 18;
          Script_charaPosRotUtils(SHEN_NAME('JBOX'), 156, &local_F4, &local_E8, 0);
          local_E4 = local_E4 + 1;
        } else {
          local_E4 = 0;
          task_1BC = 2;
        }
        break;
      case 2:
        if (local_E4 < 17) {
          Script_charaPosRotUtils(SHEN_NAME('JBOX'), 156, &local_F4, &local_E8, 2);
          (&local_E8)->y = (&local_E8)->y + 182;
          Script_charaPosRotUtils(SHEN_NAME('JBOX'), 156, &local_F4, &local_E8, 0);
          local_E4 = local_E4 + 1;
        } else {
          local_E4 = 0;
          task_1BC = 0;
          task_1B9 = 1;
          Script_executeAicaCommand(394409, 0, 0);
          task_1BA = 1;
          Script_executeAicaCommand(66729, 0, 0);
        }
        break;
      case 3:
        if (local_E4 < 17) {
          Script_charaPosRotUtils(SHEN_NAME('JBOX'), 156, &local_F4, &local_E8, 2);
          (&local_E8)->y = (&local_E8)->y - 182;
          Script_charaPosRotUtils(SHEN_NAME('JBOX'), 156, &local_F4, &local_E8, 0);
          local_E4 = local_E4 + 1;
        } else {
          local_E4 = 0;
          task_1BC = 0;
          task_1E4 = 1;
        }
        break;
      case 4:
        if (local_E4 < 13) {
          Script_charaPosRotUtils(SHEN_NAME('JBOX'), 156, &local_F4, &local_E8, 2);
          local_E8.x = (&local_E8)->x + 18;
          Script_charaPosRotUtils(SHEN_NAME('JBOX'), 156, &local_F4, &local_E8, 0);
          local_E4 = local_E4 + 1;
        } else {
          local_E4 = 0;
          Script_executeAicaCommand(197801, 0, 0);
          task_1BC = 0;
        }
        break;
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_D46E(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t local_E4;
  int32_t local_E0;
  local_E4 = 0;
  for (;;) {
    local_E0 = task_1BE;
    switch (local_E0) {
      case 1:
        if (local_E4 < 32) {
          Script_charaPosRotUtils(SHEN_NAME('JBOX'), 159, &local_F4, &local_E8, 2);
          (&local_E8)->z = (&local_E8)->z - 182;
          Script_charaPosRotUtils(SHEN_NAME('JBOX'), 159, &local_F4, &local_E8, 0);
          local_E4 = local_E4 + 1;
        } else {
          local_E4 = 0;
          task_1BE = 0;
          Script_executeAicaCommand(591017, 0, 0);
          Script_executeAicaCommand(459945, 0, 0);
        }
        break;
      case 2:
        break;
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_D514(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t local_E4;
  int32_t local_E0;
  local_E4 = 0;
  for (;;) {
    local_E0 = task_1BF;
    switch (local_E0) {
      case 1:
        if (local_E4 < 32) {
          Script_charaPosRotUtils(SHEN_NAME('JBOX'), 160, &local_F4, &local_E8, 2);
          (&local_E8)->z = (&local_E8)->z - 182;
          Script_charaPosRotUtils(SHEN_NAME('JBOX'), 160, &local_F4, &local_E8, 0);
          local_E4 = local_E4 + 1;
        } else {
          local_E4 = 0;
          task_1BF = 0;
        }
        break;
      case 2:
        break;
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_D5A0(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t local_E4;
  int32_t local_E0;
  local_E4 = 0;
  for (;;) {
    local_E0 = task_1C0;
    switch (local_E0) {
      case 1:
        Script_charaPosRotUtils(SHEN_NAME('JBOX'), 161, &data_1DEB4, 0, 3);
        Script_charaPosRotUtils(SHEN_NAME('JBOX'), 155, &data_1DEA8, 0, 3);
        if (local_E4 < 32) {
          Script_charaPosRotUtils(SHEN_NAME('JBOX'), 161, &local_F4, &local_E8, 2);
          (&local_E8)->z = (&local_E8)->z - 182;
          Script_charaPosRotUtils(SHEN_NAME('JBOX'), 161, &local_F4, &local_E8, 0);
          local_E4 = local_E4 + 1;
        } else {
          local_E4 = 0;
          task_1C0 = 0;
          Script_charaPosRotUtils(SHEN_NAME('JBOX'), 160, &data_1DEA8, 0, 3);
          Script_charaPosRotUtils(SHEN_NAME('JBOX'), 155, &data_1DEB4, 0, 3);
          task_1B9 = 2;
          Script_executeAicaCommand(66729, 0, 0);
        }
        break;
      case 2:
        break;
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_D698(void) {
  int32_t local_FC;
  Script_aicaGetOrSetState(8);
  for (;;) {
    local_FC = Script_aicaGetOrSetState(8);
    if (local_FC == 1) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_ReturnTimeDate(&task_1B0);
  if ((task_1B0.hours >= 22) & (task_1B0.minutes >= 0)) {
    Script_executeAicaCommand(0x1200A0, 2, 0);
    while (Script_aicaGetOrSetState(2)) {
      SCNF_WaitFrames_0000(1);
    }
  }
  Script_srchControl(0);
  return;
}

static void script_D70D(void) {
  Script_SetShouldSleepFlag(1);
  Script_TimedTransitionUtils(TIMED_TRANSITION_SET_ENABLED, 1);
  return;
}

static void script_D71F(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  if (task_1F4 == 0) {
  } else {
    Script_charaPosRotUtils(SHEN_NAME('JBOX'), 156, &local_F4, &local_E8, 2);
    local_E8.x = (&local_E8)->y = (&local_E8)->z = 0;
    Script_charaPosRotUtils(SHEN_NAME('JBOX'), 156, &local_F4, &local_E8, 0);
    task_1BB = 0;
    task_1BD = 0;
    task_1F4 = 0;
  }
  return;
}

static int32_t script_D77C(void) {
  int32_t local_FC;
  Vector3 local_F0;
  Vector3i local_E4;
  Vector3 local_D8;
  Vector3i local_CC;
  Vector3 local_C0;
  local_FC = 0;
  if (task_1F6 & 2) {
    (void)0;
  } else {
    if (Script_IsPlayerAllowed(0)) {
      Script_EVT_GetPosition(CHARA_AKIR, -1, &local_F0, ENTITY_XFORM_WORLD_SPACE);
      Script_mtwkGetSetRotation(CHARA_AKIR, &local_E4, 0x42000000);
      Script_EVT_GetPosition(SHEN_NAME('JCNT'), -1, &local_D8, ENTITY_XFORM_WORLD_SPACE);
      Script_mtwkGetSetRotation(SHEN_NAME('JCNT'), &local_CC, 0x40000000);
      local_C0.x = (&local_F0)->x + Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_E4)->y) * 0.3f;
      (&local_C0)->y = (&local_F0)->y;
      (&local_C0)->z = (&local_F0)->z + Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_E4)->y) * 0.3f;
      if (Script_CheckTargetWithinAngleRange(&local_C0, 1.4f, (&local_E4)->y, 7281, &local_D8)) {
        if (Script_CheckTargetWithinAngleRange(&local_D8, 1.5f, (&local_CC)->y + 0x8000, 12743, &local_F0)) {
          local_FC = 1;
        }
      }
    }
    (void)local_FC;
  }
  end:;
}

static int32_t script_D86D(int32_t arg0) {
  int32_t local_FC;
  int32_t unused_F8;
  GameDateTime local_F0;
  int32_t local_EC;
  local_FC = 1;
  Script_ReturnTimeDate(&local_F0);
  local_EC = arg0;
  if (local_EC == 0) {
    if (((&local_F0)->hours >= 21) & ((&local_F0)->minutes >= 45)) {
      local_FC = 0;
    }
  } else {
    local_FC = 0;
  }
  return local_FC;
}

static int32_t script_D8B4(int32_t arg0, const char *arg1) {
  task_212 = 0;
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
  task_212 = 1;
}

static void script_D929(const char *arg0) {
  if (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 5, arg0) != 0) {
    Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, arg0);
  }
  return;
}

static int32_t script_D94A(int32_t arg0, const char *arg1, int32_t arg2) {
  Script_LoadSND(3, 0, 0, 0);
  Script_LoadSND(1, arg0, arg1, arg2);
  while (Script_LoadSND(2, 0, 0, 0)) {
    SCNF_WaitFrames_0000(1);
  }
  return Script_LoadSND(4, 0, 0, 0);
}

static int32_t script_D98F(int32_t arg0, int32_t arg1) {
  if (arg1 > 0) {
    SCNF_WaitFrames_0000(arg1);
  }
  if (task_210 == 0) {
    Script_executeAicaCommand(arg0, 0, 0);
  }
  return Script_srchControl(0);
}

static void script_D9BD(int32_t arg0, int32_t arg1, int32_t arg2) {
  Script_scenCreateTask(script_D94A, 3, arg0, arg1, arg2);
  return;
}

static void script_D9D1(int32_t arg0, int32_t arg1) {
  Script_scenCreateTask(script_D98F, 2, arg0, arg1);
  return;
}

static int32_t script_D9E3(const char *arg0, int32_t arg1) {
  if (arg1 > 0) {
    SCNF_WaitFrames_0000(arg1);
  }
  if (task_210 == 0) {
    Script_StartOrFreeSEQ(0);
    Script_ShowUIMessageFromID(arg0);
  }
  return Script_srchControl(0);
}

static void script_DA14(int32_t arg0, int32_t arg1) {
  Script_scenCreateTask(script_D9E3, 2, arg0, arg1);
  return;
}

static int32_t script_DA26(int32_t arg0, int32_t arg1, int32_t arg2, const Vector3 *arg3, const Vector3i *arg4, int32_t arg5) {
  if (arg5 >= 0) {
    SCNF_WaitFrames_0000(arg5);
  }
  if (task_210 == 0) {
    Script_fixoAttachEx(arg0, arg1, arg2, arg3, arg4);
  }
  return Script_srchControl(0);
}

static void script_DA59(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
  Script_scenCreateTask(script_DA26, 6, arg0, arg1, arg2, arg3, arg4, arg5);
  return;
}

static int32_t script_DA73(int32_t arg0, int32_t arg1) {
  if (arg1 >= 0) {
    SCNF_WaitFrames_0000(arg1);
  }
  Script_fixoClearAttachment(arg0);
  Script_charaDisplay(arg0, CDM_Hide);
  return Script_srchControl(0);
}

static void script_DA9B(int32_t arg0, int32_t arg1) {
  Script_scenCreateTask(script_DA73, 2, arg0, arg1);
  return;
}

static void script_DAAD(int32_t arg0, int32_t arg1) {
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

static int32_t script_DAEB(int32_t arg0, int32_t arg1, int32_t arg2, float arg3, int32_t arg4, int32_t arg5) {
  if (arg5 >= 0) {
    SCNF_WaitFrames_0000(arg5);
  }
  if (task_210 == 0) {
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

static int32_t script_DB5C(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  if (arg4 >= 0) {
    SCNF_WaitFrames_0000(arg4);
  }
  if (task_210 == 0) {
    if (arg3 & -0x80000000) {
      Script_motmToggleFlags(arg0, 2, 0x40);
    } else {
      Script_motmToggleFlags(arg0, 1, 0x40);
    }
    Script_PlayCharaMotion(arg0, arg1, arg2, -1.0f, -1.0f, 1.0f, 0);
  }
  return Script_srchControl(0);
}

static void script_DBBE(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  Script_scenCreateTask(script_DB5C, 5, arg0, arg1, arg2, arg3, arg4);
  return;
}

static void script_DBD6(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4, int32_t arg5) {
  Script_scenCreateTask(script_DAEB, 6, arg0, arg1, arg2, arg3, arg4, arg5);
  return;
}

static int32_t script_DBF0(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  if (arg3 >= 0) {
    SCNF_WaitFrames_0000(arg3);
  }
  if (task_210 == 0) {
    Script_SetNewHandMotion(arg0, arg1, arg2, 0, 0, 0, 0, 1.0f, 0);
  }
  return Script_srchControl(0);
}

static void script_DC2E(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Script_scenCreateTask(script_DBF0, 4, arg0, arg1, arg2, arg3);
  return;
}

static int32_t script_DC44(int32_t arg0, int32_t arg1, const Vector3i *arg2, int32_t arg3) {
  if (arg3 >= 0) {
    SCNF_WaitFrames_0000(arg3);
  }
  if (task_210 == 0) {
    Script_SetHandRootRotation(arg0, arg1, 0x15, arg2);
  }
  return Script_srchControl(0);
}

static void script_DC75(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Script_scenCreateTask(script_DC44, 4, arg0, arg1, arg2, arg3);
  return;
}

static int32_t script_DC8B(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  if (arg4 >= 0) {
    SCNF_WaitFrames_0000(arg4);
  }
  if (task_210 == 0) {
    Script_FACE(arg0, arg1, arg2, arg3);
  }
  return Script_srchControl(0);
}

static void script_DCBC(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
  Script_scenCreateTask(script_DC8B, 5, arg0, arg1, arg2, arg3, arg4);
  return;
}

static int32_t script_DCD4(int32_t arg0, int32_t arg1, const Vector3 *arg2, int32_t arg3) {
  int32_t local_FC;
  if (arg3 >= 0) {
    SCNF_WaitFrames_0000(arg3);
  }
  if (task_210 == 0) {
    local_FC = Script_CEWPRelated(arg0);
    Script_MoveObjectToObject(arg0, arg1);
    Script_EVT_SetPosition(arg0, POS_XYZ, arg2);
    Script_MoveObjectToObject(arg0, local_FC);
  }
  return Script_srchControl(0);
}

static void script_DD21(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Script_scenCreateTask(script_DCD4, 4, arg0, arg1, arg2, arg3);
  return;
}

static int32_t script_DD37(int32_t arg0, int32_t arg1, const Vector3i *arg2, int32_t arg3) {
  int32_t local_FC;
  if (arg3 >= 0) {
    SCNF_WaitFrames_0000(arg3);
  }
  if (task_210 == 0) {
    local_FC = Script_CEWPRelated(arg0);
    Script_MoveObjectToObject(arg0, arg1);
    Script_EVT_SetRotation(arg0, 0x38000000, arg2, 0);
    Script_MoveObjectToObject(arg0, local_FC);
  }
  return Script_srchControl(0);
}

static void script_DD86(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Script_scenCreateTask(script_DD37, 4, arg0, arg1, arg2, arg3);
  return;
}

static int32_t script_DD9C(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  if (arg3 >= 0) {
    SCNF_WaitFrames_0000(arg3);
  }
  if (task_210 == 0) {
    Script_seqEnterEvtCam(arg0, arg1, arg2);
  }
  return Script_srchControl(0);
}

static void script_DDCA(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Script_scenCreateTask(script_DD9C, 4, arg0, arg1, arg2, arg3);
  return;
}

static void script_DDE0(int32_t arg0) {
  Vector4 local_F0;
  local_F0.x = 0.0f;
  (&local_F0)->y = 0.2f;
  (&local_F0)->z = 0.2f;
  (&local_F0)->w = 0.2f;
  Script_SetCharaRefBoardPos(arg0, &local_F0);
  Script_charaSetRefboardFlagTo(arg0, 1);
  return;
}

static void script_DE1F(void) {
  int32_t local_FC;
  local_FC = 0;
  for (;;) {
    Script_PrintDebugString(2, 4, "%03d", local_FC);
    SCNF_WaitFrames_0000(1);
    local_FC = local_FC + 1;
  }
  return;
}

static void script_DE4B(int32_t arg0, int32_t arg1, int32_t arg2) {
  int8_t local_FF;
  int32_t local_F8;
  local_FF = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 100.0f);
  if (~(arg2 & 2)) {
    if (local_FF > 74) {
      task_204 = 5;
      task_200 = (char *)data_1E313;
      arg1 = (int32_t)task_200;
    }
  }
  local_F8 = task_204;
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
  script_D8B4(arg0, (const char *)arg1);
  script_DDE0(arg0);
  return;
}

static int32_t script_DF71(int32_t arg0, int32_t arg1, int32_t arg2) {
  if (arg2 > 0) {
    SCNF_WaitFrames_0000(arg2);
  }
  if (arg1 == 0) {
    Script_charaPosRotUtils(arg0, 152, &data_1E318, 0, 0);
    Script_charaPosRotUtils(arg0, 153, &data_1E324, 0, 0);
  } else {
    Script_charaPosRotUtils(arg0, 152, &data_1E324, 0, 0);
    Script_charaPosRotUtils(arg0, 153, &data_1E318, 0, 0);
  }
  return Script_srchControl(0);
}

static void script_DFE3(int32_t arg0, int32_t arg1, int32_t arg2) {
  Script_scenCreateTask(script_DF71, 3, arg0, arg1, arg2);
  return;
}

static int32_t script_DFF7(int32_t arg0) {
  SCNF_WaitFrames_0000(73);
  SCNF_SetCashValue(-100, 1);
  arg0 = arg0 - 100;
  Script_SetStats(STATS_CASH, arg0);
  end:;
}

static int32_t script_E01D(int32_t arg0) {
  SCNF_SetCashValue(100, 1);
  arg0 = arg0 + 100;
  Script_SetStats(STATS_CASH, arg0);
  end:;
}

static int32_t script_E03B(int32_t arg0, int32_t arg1) {
  if (arg1 > 0) {
    SCNF_WaitFrames_0000(arg1);
  }
  SCNF_StartIconSystemMenu(arg0);
  Script_GetCassetteFlags(arg0, 1, 0, 0);
  return Script_srchControl(0);
}

static void script_E069(int32_t arg0, int32_t arg1) {
  Script_scenCreateTask(script_E03B, 2, arg0, arg1);
  return;
}

__attribute__((scn_string_bindings(0, data_1E330)))
static void script_E07B(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
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
        script_E444(arg0);
        Script_scenCreateTask(script_DFF7, 1, local_F8);
        Script_scenCreateTask(script_D8B4, 2, arg2, "VEND");
        script_EA0D(arg0, arg1);
        script_E51F(arg0, arg1, arg2, arg3);
        local_F4 = script_E6C9(arg0, arg1, arg2, arg3, 1);
        local_E4 = local_F4;
        switch (local_E4) {
          case 13:
            if (task_204 == 5) {
              script_EADB(arg0, arg1, 1);
            } else {
              script_EDCC(arg0, arg1, 1);
            }
            break;
          case 16:
            local_F8 = Script_shenGetStats(STATS_CASH);
            Script_scenCreateTask(script_E01D, 1, local_F8);
            break;
        }
        script_E5C0(arg0, arg1, arg2, arg3);
      } else {
        if (Script_ryoControl(PCCTL_QueryShouldRun)) {
          local_FF = 1;
          Script_ryoControl(PCCTL_DisablePlayerControl);
        }
        script_B11D(2, 0);
        if (local_FF == 1) {
          Script_ryoControl(RYO_RestoreControl);
        }
      }
    }
  }
  return;
}

static void script_E1AC(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  script_E2E7(arg0, arg1, arg2, arg3);
  script_E6C9(arg0, arg1, arg2, arg3, 0);
  script_E5C0(arg0, arg1, arg2, arg3);
  return;
}

__attribute__((scn_string_bindings(0, data_1E335)))
static void script_E1DE(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  local_FC = Script_shenGetStats(STATS_CASH);
  if (Script_scenUtils(SCENARIO_CMD_GET_SCEN_FLAG100) == 0) {
    if (local_FC > 99) {
      script_E444(arg0);
      Script_scenCreateTask(script_DFF7, 1, local_FC);
      Script_scenCreateTask(script_D8B4, 2, arg2, "VEND");
      script_EA0D(arg0, arg1);
      script_E51F(arg0, arg1, arg2, arg3);
      local_F8 = script_E6C9(arg0, arg1, arg2, arg3, 1);
      local_F4 = local_F8;
      switch (local_F4) {
        case 13:
          script_EDCC(arg0, arg1, 1);
          break;
        case 16:
          local_FC = Script_shenGetStats(STATS_CASH);
          Script_scenCreateTask(script_E01D, 1, local_FC);
          break;
      }
      script_E5C0(arg0, arg1, arg2, arg3);
    }
  }
  return;
}

static void script_E2AD(void) {
  task_1FC = 0x314E4143;
  task_204 = 3;
  task_200 = (char *)data_1E33A;
  task_210 = 0;
  task_211 = 0;
  task_212 = 0;
  task_218 = 0;
  return;
}

__attribute__((scn_string_bindings(0, data_1E33F, 1, data_1E346, 2, data_1E353)))
static void script_E2E7(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Vector3i local_F4;
  Vector3 local_E8;
  script_E2AD();
  Script_ryoControl(PCCTL_UpdateRyoNavYaw, arg0);
  Script_charaSetRefboardFlagTo(arg0, 1);
  Script_uiUtils(0, 25, 0);
  Script_FaceEx(arg0, 1, 0, 2);
  task_208 = Script_ryoControl(PCCTL_QueryShouldRun);
  if (task_208) {
    Script_ryoControl(PCCTL_DisablePlayerControl);
    Script_PlayCharaMotion(arg0, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_scenUtils(SCENARIO_CMD_INIT_SCNT);
    Script_ccowUtils(arg0, 2);
    Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
    Script_CycleManagerUtils(CYCLE_CMD_PROCESS_SND);
  }
  Script_LoadVoiceFile(1, "01JUCE");
  task_218 = Script_LoadHandMot(2, SHEN_NAME('VEND'), "HMOT_CAN.BIN");
  Script_scenCreateTask(script_D8B4, 2, arg2, "VEND");
  while (task_212 != 1) {
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
  script_DDE0(arg3);
  Script_InitMOTMGlobalState(arg0, arg2, arg3);
  return;
}

__attribute__((scn_string_bindings(0, data_1E358, 2, data_1E379)))
static void script_E444(int32_t arg0) {
  script_E2AD();
  Script_SetCPAMFlags(arg0, 0);
  Script_FaceEx(arg0, 1, 0, 2);
  Script_charaSetRefboardFlagTo(arg0, 1);
  Script_uiUtils(0, 25, 0);
  task_208 = Script_ryoControl(PCCTL_QueryShouldRun);
  if (task_208) {
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
  Script_sprintf("HandMot read by current!\012");
  task_218 = Script_LoadHandMot(2, SHEN_NAME('MKYU'), "HMOT_CAN.BIN");
  Script_ResetHandMotion(arg0, 1, 1, 0);
  Script_ResetHandMotion(arg0, 1, 0, 0);
  return;
}

static void script_E51F(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Vector3 local_F4;
  Vector3i local_E8;
  while (task_212 != 1) {
    SCNF_WaitFrames_0000(1);
  }
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
  Script_facemotUtils(arg3, 1, 0);
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
  Script_ResetHandMotion(arg3, 1, 1, 0);
  script_DDE0(arg3);
  Script_charaDisplay(arg1, CDM_Hide);
  Script_ccowUtils(arg1, 2);
  Script_EVT_GetPosition(arg1, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  Script_mtwkGetSetRotation(arg1, &local_E8, 0x40000000);
  Script_EVT_SetPosition(arg2, 0x78000000, &local_F4);
  Script_EVT_SetRotation(arg2, 0x78000000, &local_E8, 0);
  Script_InitMOTMGlobalState(arg0, arg2, arg3);
  return;
}

__attribute__((scn_string_bindings(0, data_1E386, 1, data_1E38D)))
static void script_E5C0(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  Vector3i local_F4;
  Script_charaSetRefboardFlagTo(arg3, 0);
  Script_ResetHandMotion(arg3, 0, 1, 0);
  Script_faceUpdateFor(arg3, 1, 0);
  SCNF_WaitFrames_0000(1);
  Script_ResetHandMotion(arg0, 0, 1, 0);
  Script_ResetHandMotion(arg0, 0, 0, 0);
  if (task_218 != 0) {
    Script_FreeTaskData(task_218);
  }
  Script_LoadVoiceFile(0, "01JUCE");
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 1);
  Script_ccowUtils(arg0, 1);
  Script_SetGameStateTo(AUTH_None);
  Script_scenUtils(SCENARIO_CMD_UPDATE_TIMER);
  Script_mtwkGetSetRotation(arg0, &local_F4, 0x2000000);
  Script_EVT_SetRotation(arg0, 0x78000000, &local_F4, 0);
  if (task_208) {
    Script_PlayCharaMotion(arg0, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
    Script_ryoControl(RYO_RestoreControl);
  }
  SCNF_WaitFrames_0000(1);
  script_D929("VEND");
  Script_FaceEx(arg0, 0, 0, 2);
  Script_charaSetRefboardFlagTo(arg0, 2);
  Script_SetCPAMFlags(arg0, 1);
  task_211 = 14;
  Script_shenClearFreeState(1);
  return;
}

static int32_t script_E6C9(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
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
  local_D7 = script_AFDB(2, 100, 0);
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
            script_AFDB(2, 100, 1);
            Script_sprintf("vendmachine exit is MONEY_LOCK -> %d,%d\012", local_D7, local_DC);
            local_E4 = 16;
          }
        }
      }
    }
    if (local_E0 != local_E4) {
      local_E0 = local_E4;
      task_211 = local_E4;
      local_D0 = local_E4;
      if (local_D0 == 1) {
        script_DC75(arg3, 1, (int32_t)data_1E394, -1);
        script_DC2E(arg3, 1, 542, -1);
      } else if (local_D0 == 3) {
        task_211 = 1;
        Script_shenClearFreeState(0);
        Script_sicnUtils(1, 0, 0);
      } else if (local_D0 == 16) {
        Script_sicnUtils(1, 0, 0);
      } else if ((local_D0 == 2) | (local_D0 == 4)) {
        task_211 = 1;
      } else {
        switch (local_D0) {
          case 5:
            script_D9D1(0x1E04A9, -1);
            break;
          case 6:
            script_D9D1(0x1104A9, 5);
            break;
          case 8:
            task_200 = (char *)data_1E3E6;
            task_204 = 0;
            if (arg4 & 1) {
              script_DE4B(task_1FC, (int32_t)task_200, arg4);
            }
            break;
          case 9:
            task_200 = (char *)data_1E3EB;
            task_204 = 1;
            if (arg4 & 1) {
              script_DE4B(task_1FC, (int32_t)task_200, arg4);
            }
            break;
          case 10:
            task_200 = (char *)data_1E3F0;
            task_204 = 2;
            if (arg4 & 1) {
              script_DE4B(task_1FC, (int32_t)task_200, arg4);
            }
            break;
          case 11:
            task_200 = (char *)data_1E3F5;
            task_204 = 3;
            if (arg4 & 1) {
              script_DE4B(task_1FC, (int32_t)task_200, arg4);
            }
            break;
          case 12:
            task_200 = (char *)data_1E3FA;
            task_204 = 4;
            if (arg4 & 1) {
              script_DE4B(task_1FC, (int32_t)task_200, arg4);
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

static void script_EA0D(int32_t arg0, int32_t arg1) {
  Script_shenClearFreeState(0);
  script_DD21(arg0, arg1, (int32_t)&data_1E400, -1);
  script_DD86(arg0, arg1, (int32_t)data_1E40C, -1);
  script_DC75(arg0, 1, (int32_t)data_1E418, -1);
  script_DC75(arg0, 0, (int32_t)data_1E418, -1);
  script_DC2E(arg0, 1, 871, -1);
  script_DBBE(arg0, 712, 0, -0x80000000, -1);
  script_D9D1(0x1004A9, 125);
  script_DDCA(4013, arg1, arg1, -1);
  SCNF_WaitFrames_0000(10);
  script_DAAD(arg0, 0);
  Script_PlayCharaMotion(arg0, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_shenClearFreeState(1);
  return;
}

__attribute__((scn_string_bindings(3, data_1E481, 4, data_1E48C)))
static void script_EADB(int32_t arg0, int32_t arg1, int32_t arg2) {
  int32_t local_FC;
  if (arg2 == 0) {
    script_D8B4(task_1FC, task_200);
  }
  script_DA59(task_1FC, arg0, 18, (int32_t)&data_1E424, (int32_t)&data_1E430, 82);
  script_DFE3(task_1FC, 1, 0);
  script_DD21(arg0, arg1, (int32_t)&data_1E43C, -1);
  script_DD86(arg0, arg1, (int32_t)data_1E448, -1);
  script_DC75(arg0, 1, (int32_t)&data_1E454, -1);
  script_DC2E(arg0, 1, 545, -1);
  script_DC2E(arg0, 0, 546, -1);
  script_DBBE(arg0, 45059, 0, -0x80000000, -1);
  script_D9D1(0x1604A9, 80);
  script_DDCA(4019, arg1, arg1, -1);
  SCNF_WaitFrames_0000(10);
  script_DAAD(arg0, 0);
  local_FC = (int32_t)Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f);
  if (local_FC == 0) {
    script_DA14((int32_t)"01JUCEA011", -1);
  } else if (local_FC == 1) {
    script_DA14((int32_t)"01JUCEA012", -1);
  } else {
    script_DA14((int32_t)"01JUCEA013", -1);
  }
  SCNF_WaitFrames_0000(90);
  script_DCBC(arg0, 10, 5, 16, 180);
  script_DCBC(arg0, 9, 5, 16, 251);
  script_DCBC(arg0, 4, 5, 16, 351);
  script_DCBC(arg0, 1, 4, 16, 391);
  script_DCBC(arg0, 9, 5, 16, 426);
  script_DCBC(arg0, 10, 1, 16, 521);
  script_DCBC(arg0, 1, 2, 16, 545);
  script_DCBC(arg0, 0, 0, 16, 596);
  script_D9D1(0x1204A9, 108);
  script_DFE3(task_1FC, 0, 108);
  script_DBD6(arg0, 45058, 0, 0x441C0000, 0, -1);
  script_D9D1(0x2004A9, 210);
  script_D9D1(0x2104A9, 330);
  script_D9D1(0x2004A9, 470);
  script_D9D1(0x2104A9, 500);
  script_DA14((int32_t)"01JUCEA002", 550);
  script_DA14((int32_t)"01JUCEA003", 400);
  script_DDCA(4015, arg1, arg1, -1);
  script_DDCA(4016, arg1, arg1, 575);
  SCNF_WaitFrames_0000(10);
  script_DAAD(arg0, 1);
  Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_16);
  script_DBBE(arg0, 45061, 0x42100000, 0, -1);
  script_DA9B(task_1FC, 30);
  script_E069(66, 32);
  SCNF_WaitFrames_0000(10);
  script_DAAD(arg0, 0);
  Script_motmSetBlendFrames(arg0, MOTM_BLEND_FRAMES_5);
  script_D929(task_200);
  Script_shenClearFreeState(1);
  return;
}

__attribute__((scn_string_bindings(0, data_1E4F8, 1, data_1E503)))
static void script_EDCC(int32_t arg0, int32_t arg1, int32_t arg2) {
  if (arg2 == 0) {
    script_D8B4(task_1FC, task_200);
  }
  if (task_204 == 4) {
    script_DA59(task_1FC, arg0, 18, (int32_t)&data_1E4B0, (int32_t)&data_1E4BC, 82);
    script_DD21(arg0, arg1, (int32_t)&data_1E4C8, -1);
    script_DD86(arg0, arg1, (int32_t)data_1E4D4, -1);
    script_DC75(arg0, 1, (int32_t)data_1E4EC, -1);
    script_DC2E(arg0, 1, 544, -1);
  } else {
    script_DA59(task_1FC, arg0, 18, (int32_t)&data_1E498, (int32_t)&data_1E4A4, 82);
    script_DD21(arg0, arg1, (int32_t)&data_1E4C8, -1);
    script_DD86(arg0, arg1, (int32_t)data_1E4D4, -1);
    script_DC75(arg0, 1, (int32_t)&data_1E4E0, -1);
    script_DC2E(arg0, 1, 545, -1);
  }
  script_DFE3(task_1FC, 1, 0);
  script_DC2E(arg0, 0, 546, -1);
  script_DBBE(arg0, 45059, 0, -0x80000000, -1);
  script_D9D1(0x1604A9, 80);
  script_D9D1(0x1204A9, 290);
  script_DDCA(4014, arg1, arg1, -1);
  script_DFE3(task_1FC, 0, 290);
  SCNF_WaitFrames_0000(10);
  script_DAAD(arg0, 0);
  script_DCBC(arg0, 10, 5, 16, 180);
  script_DCBC(arg0, 9, 5, 16, 251);
  script_DCBC(arg0, 4, 5, 16, 351);
  script_DCBC(arg0, 1, 4, 16, 391);
  script_DCBC(arg0, 9, 5, 16, 426);
  script_DCBC(arg0, 10, 1, 16, 521);
  script_DCBC(arg0, 1, 2, 16, 545);
  script_DCBC(arg0, 0, 0, 16, 596);
  if (task_204 == 4) {
    script_DBBE(arg0, 45060, 0, 0, -1);
  } else {
    script_DBBE(arg0, 45058, 0, 0, -1);
  }
  script_D9D1(0x2004A9, 210);
  script_D9D1(0x2104A9, 330);
  script_D9D1(0x2004A9, 470);
  script_D9D1(0x2104A9, 500);
  script_D9D1(0x1504A9, 690);
  script_DA14((int32_t)"01JUCEA002", 550);
  script_DA14((int32_t)"01JUCEA003", 400);
  script_DDCA(4015, arg1, arg1, -1);
  script_DDCA(4016, arg1, arg1, 575);
  script_DA9B(task_1FC, 690);
  SCNF_WaitFrames_0000(10);
  script_DAAD(arg0, 0);
  script_D929(task_200);
  Script_shenClearFreeState(1);
  return;
}

static int32_t script_F0B6(int32_t arg0) {
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
    if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, *(&data_1E2DC + local_F4))) {
      Script_EVT_GetPosition(*(&data_1E2DC + local_F4), -1, (Vector3 *)local_90[local_F4], ENTITY_XFORM_WORLD_SPACE);
      Script_mtwkGetSetRotation(*(&data_1E2DC + local_F4), (Vector3i *)local_FEC4[local_F4], 0x40000000);
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
      script_E1DE(arg0, *(&data_1E2DC + local_F9), SHEN_NAME('JIHN'), SHEN_NAME('HAND'));
      (void)((local_FB = 0) & 0x80);
      (void)((local_FC = 0) & 0x80);
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_F51A(int32_t arg0) {
  script_E2AD();
  task_20C = Script_scenCreateTask(script_F0B6, 1, arg0);
  return;
}

static void script_F535(void) {
  Script_srchControl((void *)task_20C);
  task_20C = 0;
  return;
}

static void script_F546(void) {
  (void)task_211;
  return;
}

static void script_F54D(void) {
  Script_ExitCharaSelector();
  task_210 = 1;
  return;
}

__attribute__((scn_string_bindings(0, data_1E56C, 1, data_1E57C)))
static void script_F55A(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  int32_t local_F0;
  Vector3 local_E4;
  Vector3i local_D8;
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, "/scene/02/MKYU/", "DART") == 0) {
    SCNF_WaitFrames_0000(1);
  }
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
  Script_LoadHandMot(2, SHEN_NAME('MKYU'), "HMOT_DRT.BIN");
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
  script_FE54();
  local_FC = 0;
  while (local_FC < 3) {
    local_F8 = 5;
    local_F4 = 1 << local_FC;
    local_F0 = *(&data_1E54C + local_FC);
    if (local_F8 & local_F4) {
      local_E4.x = *(float *)((const char *)&data_1E51C + local_FC * 16);
      (&local_E4)->y = *(float *)((const char *)&data_1E51C + (local_FC * 16 + 4));
      (&local_E4)->z = *(float *)((const char *)&data_1E51C + (local_FC * 16 + 8));
      local_D8.x = 0;
      (&local_D8)->y = *(int16_t *)((int8_t *)&data_1E51C + (local_FC * 16 + 12));
      (&local_D8)->z = 0;
      Script_EVT_SetPosition(local_F0, POS_XYZ, &local_E4);
      Script_EVT_SetRotation(local_F0, 0x38000000, &local_D8, 0);
      Script_scenToggleVisibleFor(local_F0, 1);
      Script_charaDisplay(local_F0, CDM_Hide);
      task_21C = script_1297D(local_F0, 0, 2, 0);
      task_220 = script_1297D(local_F0, 0, 3, 0);
      script_12017(local_F0);
    }
    local_FC = local_FC + 1;
  }
  if (arg0) {
    task_22C = Script_scenCreateTask(script_F86D, 0);
  }
  return;
}

__attribute__((scn_string_bindings(0, data_1E58E)))
static void script_F6E1(int32_t arg0) {
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "DART");
  Script_FreeTaskData(task_228);
  if (arg0) {
    Script_srchControl((void *)task_22C);
  }
  return;
}

static int32_t script_F707(void) {
  Vector3 local_F4;
  Vector3 local_E8;
  Vector3i local_DC;
  int32_t local_D8;
  int32_t local_D4;
  int32_t local_D0;
  int32_t local_CC;
  Vector3 local_C0;
  Vector3i local_B4;
  float local_B0;
  Script_EVT_GetPosition(CHARA_AKIR, -1, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  Script_mtwkGetSetRotation(CHARA_AKIR, &local_DC, 0x42000000);
  local_E8.x = (&local_F4)->x + Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_DC)->y) * 0.3f;
  (&local_E8)->y = (&local_F4)->y;
  (&local_E8)->z = (&local_F4)->z + Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_DC)->y) * 0.3f;
  local_D4 = 0;
  local_D8 = 0;
  while (local_D8 < 3) {
    local_D0 = 5;
    local_CC = 1 << local_D8;
    if (local_D0 & local_CC) {
      Script_EVT_GetPosition(*(&data_1E54C + local_D8), -1, &local_C0, ENTITY_XFORM_WORLD_SPACE);
      local_B0 = Script_MathUtils(SCRIPT_MATH_TASK_VEC3_DISTANCE, &local_F4, &local_C0);
      if (!(local_B0 >= 3.0f)) {
        Script_scenToggleVisibleFor(*(&data_1E54C + local_D8), 1);
        if (Script_CheckTargetWithinAngleRange(&local_E8, 1.4f, (&local_DC)->y, 5461, &local_C0)) {
          Script_mtwkGetSetRotation(*(&data_1E54C + local_D8), &local_B4, 0x40000000);
          if (Script_CheckTargetWithinAngleRange(&local_C0, 1.5f, (&local_B4)->y + 0x8000, 9102, &local_F4)) {
            local_D4 = *(&data_1E54C + local_D8);
          }
        }
      } else {
        Script_scenToggleVisibleFor(*(&data_1E54C + local_D8), 0);
      }
    }
    local_D8 = local_D8 + 1;
  }
  return local_D4;
}

static int32_t script_F86D(void) {
  int8_t local_FF;
  int32_t local_F8;
  int8_t local_F7;
  (void)((local_FF = 0) & 2);
  for (;;) {
    (void)((local_F7 = 0) & 2);
    if (Script_IsPlayerAllowed(0)) {
      local_F8 = script_F707();
      if (local_F8) {
        (void)((local_F7 = -1) & 2);
        if (Script_GetControllerInput(1) == 0x400) {
          if (local_FF == -1) {
            (void)((local_FF = 0) & 2);
            SCNF_RequestIconNotification(0, 0, 0, 2);
          }
          script_F934(local_F8);
        }
      }
    }
    if ((local_FF == 0) & (local_F7 == -1)) {
      (void)((local_FF = -1) & 2);
      Script_sicnUtils(0, 5, 1);
    } else {
      if ((local_FF == -1) & (local_F7 == 0)) {
        (void)((local_FF = 0) & 2);
        Script_sicnUtils(1, 0, 0);
        goto L00F928;
      }
    }
    L00F928:;
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_F934(int32_t arg0) {
  script_F952(arg0);
  script_FB6C(arg0);
  script_FCBA(arg0);
  return;
}

__attribute__((scn_string_bindings(0, data_1E593)))
static void script_F952(int32_t arg0) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t local_E4;
  int32_t unused_E0;
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_motmToggleFlags(CHARA_AKIR, 2, 4);
  Script_motmToggleFlags(CHARA_AKIR, 2, 0x40);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 1);
  Script_scenToggleActiveFor(CHARA_AKIR, 0);
  local_F4.x = 0.0f;
  (&local_F4)->y = 0.0f;
  (&local_F4)->z = 0.6f;
  local_E8.x = 0;
  (&local_E8)->y = 0;
  (&local_E8)->z = 0;
  script_12E53(CHARA_AKIR, arg0, &local_F4);
  script_12E92(CHARA_AKIR, arg0, &local_E8);
  script_122AD(1, arg0);
  Script_InitializeOsag(CHARA_AKIR);
  script_12651(arg0);
  script_12426(2, arg0);
  SCNF_WaitFrames_0000(5);
  script_130D6();
  if ((arg0 == 0x54524144) | (arg0 == 0x32524144)) {
    (void)((task_259 = -1) & 1);
  } else {
    if (arg0 == 0x33524144) {
      (void)((task_259 = 0) & 1);
      goto L00FA72;
    }
  }
  L00FA72:;
  task_21C = script_1297D(arg0, 0, 2, 0);
  task_220 = script_1297D(arg0, 0, 3, 0);
  (void)((task_25A = 0) & 1);
  task_25B = 15;
  Script_ccowUtils(CHARA_AKIR, 2);
  if (task_259 & 8) {
    Script_uiUtils(0, 47, 0);
  } else {
    Script_uiUtils(0, 3, 0);
  }
  Script_sicnUtils(0, 1, 1);
  Script_LoadVoiceFile(1, "E1025");
  Script_ResetHandMotion(CHARA_AKIR, 1, 1, 0);
  Script_ResetHandMotion(CHARA_AKIR, 1, 0, 0);
  Script_motmToggleFlags(CHARA_AKIR, 1, 0x40);
  Script_motmSetBlendFrames(CHARA_AKIR, MOTM_BLEND_FRAMES_30);
  local_E4 = 0;
  while (local_E4 < 5) {
    Script_scenToggleVisibleFor(*(&data_1E558 + local_E4), 1);
    Script_charaDisplay(*(&data_1E558 + local_E4), CDM_Hide);
    local_E4 = local_E4 + 1;
  }
  return;
}

static void script_FB6C(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  if (script_FE7D(arg0)) {
    script_12FC0();
    local_F4 = Script_scenCreateTask(script_1295D, 0);
    for (;;) {
      script_1297D(arg0, 1, 0, script_1297D(arg0, 0, 0, 0) + 1);
      script_10001(arg0);
      local_FC = Script_scenCreateTask(script_12000, 1, arg0);
      script_10661(arg0);
      local_F8 = Script_scenCreateTask(script_10C1E, 0);
      if (task_259 & 8) {
        Script_executeAicaCommand(131496, 0, 0);
      } else {
        Script_executeAicaCommand(197032, 0, 0);
      }
      L00FC0A:;
      script_106B3(arg0);
      if (task_25A & 0x10) {
        task_238 = 0;
        task_254 = 0;
        SCNF_WaitFrames_0000(1);
      } else {
        if (!(script_1161F(arg0) == 0)) goto L00FC0A;
      }
      Script_executeAicaCommand(0x1200A0, 4, 0);
      script_10B16(arg0);
      Script_srchControl((void *)local_FC);
      Script_srchControl((void *)local_F8);
      if (task_25A & 0x10) {
        break;
      }
      if (script_11EB6(arg0) == 0) {
        break;
      }
    }
    Script_srchControl((void *)local_F4);
    script_1297D(arg0, 1, 1, script_1297D(arg0, 0, 1, 0) + task_23C);
    script_12FF9();
  }
  return;
}

__attribute__((scn_string_bindings(0, data_1E599)))
static void script_FCBA(int32_t arg0) {
  int32_t local_FC;
  Vector3 local_F0;
  Vector3i local_E4;
  Script_charaDisplay(CHARA_AKIR, CDM_Show);
  script_1297D(arg0, 1, 2, task_21C);
  script_1297D(arg0, 1, 3, task_220);
  local_FC = 0;
  while (local_FC < 5) {
    Script_MoveObjectToObject(*(&data_1E558 + local_FC), -1);
    Script_charaDisplay(*(&data_1E558 + local_FC), CDM_Hide);
    Script_scenToggleVisibleFor(*(&data_1E558 + local_FC), 0);
    script_1225B(arg0, local_FC, 0);
    local_FC = local_FC + 1;
  }
  Script_convSetCamInactiveFlag(1);
  Script_SetMotnFlag(CHARA_AKIR, 0);
  Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
  Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
  script_12426(0, arg0);
  Script_LoadVoiceFile(0, "E1025");
  Script_charaDisplay(CHARA_AKIR, CDM_Show);
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 2);
  Script_scenToggleActiveFor(CHARA_AKIR, 1);
  Script_ccowUtils(CHARA_AKIR, 1);
  local_F0.x = 0.0f;
  (&local_F0)->y = 0.0f;
  (&local_F0)->z = 0.53f;
  local_E4.x = 0;
  (&local_E4)->y = 0;
  (&local_E4)->z = 0;
  script_12E53(CHARA_AKIR, arg0, &local_F0);
  script_12E92(CHARA_AKIR, arg0, &local_E4);
  Script_InitializeOsag(CHARA_AKIR);
  Script_FaceLookAtTargetUtils(CHARA_AKIR, 0);
  Script_SetCharaLookAtPosition(CHARA_AKIR, -1, 0, 0);
  script_13112();
  Script_shenClearFreeState(1);
  Script_ryoControl(RYO_RestoreControl);
  Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_EV_STOP_ACTIVE, 0);
  return;
}

static void script_FE54(void) {
  task_254 = 0;
  task_234 = 0;
  task_238 = 0;
  (void)((task_258 = 0) & 1);
  task_25B = 15;
  return;
}

__attribute__((scn_string_bindings(7, data_1E5E5)))
static int32_t script_FE7D(int32_t arg0) {
  int32_t local_FC;
  int8_t local_FB;
  int32_t local_F4;
  int32_t local_F0;
  SCNF_WaitFrames_0000(1);
  Script_scenCreateTask(script_12625, 2, 9, arg0);
  script_123BC(2, "E1025A001", "E1025A002");
  SCNF_WaitFrames_0000(135);
  Script_motmSetBlendFrames(CHARA_AKIR, MOTM_BLEND_FRAMES_5);
  local_FC = SCNF_RequestActionSelectorFromParams_0008(0xFFFF, 36, 47, 0xFFFF, 0x7FFF, 0xFFFF, ACTION_SELECTOR_MODE_STANDARD_TIMEOUT_CANCEL);
  if (local_FC == 36) {
    local_F4 = script_AFDB(2, 100, 1);
    local_F0 = local_F4;
    if (local_F0 == 0) {
      script_123BC(3, "E1025A003", "E1025A004", "E1025A005");
      while (Script_convGetIsActive((const void *)task_224)) {
        SCNF_WaitFrames_0000(1);
      }
      (void)((local_FB = -1) & 4);
    } else if (local_F0 == 1) {
      script_FFC6();
      (void)((local_FB = 0) & 4);
    } else {
      (void)((local_FB = 0) & 4);
    }
  } else {
    script_12426(7, arg0);
    script_123BC(3, "E1025A009", "E1025A010", "E1025A011");
    while (Script_convGetIsActive((const void *)task_224)) {
      SCNF_WaitFrames_0000(1);
    }
    (void)((local_FB = 0) & 4);
  }
  Script_sicnUtils(2, 1, 0);
  Script_shenClearFreeState(0);
  return local_FB & 8;
}

static void script_FFC6(void) {
  script_123BC(3, "E1025A006", "E1025A007", "E1025A008");
  SCNF_SetCashValue(0, 1);
  while (Script_convGetIsActive((const void *)task_224)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_10001(int32_t arg0) {
  Vector3 local_F4;
  Vector3 local_E8;
  Vector3 local_DC;
  Vector3i local_D0;
  int32_t local_CC;
  int32_t local_C8;
  Script_shenClearFreeState(1);
  Script_sicnUtils(0, 1, 1);
  local_E8.x = 0.0f;
  (&local_E8)->y = 0.0f;
  (&local_E8)->z = 0.6f;
  local_D0.x = 0;
  (&local_D0)->y = 0;
  (&local_D0)->z = 0;
  script_12E53(CHARA_AKIR, arg0, &local_E8);
  script_12E92(CHARA_AKIR, arg0, &local_D0);
  Script_PlayCharaMotion(CHARA_AKIR, 31012, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_InitializeOsag(CHARA_AKIR);
  script_122AD(0);
  local_C8 = 0;
  L0100A3:;
  Script_charaDisplay(*(&data_1E558 + local_C8), CDM_Hide);
  Script_MoveObjectToObject(*(&data_1E558 + local_C8), arg0);
  local_C8 = local_C8 + 1;
  if (!(local_C8 == 5)) goto L0100A3;
  local_F4.x = 1.0f;
  (&local_F4)->y = 0.0f;
  (&local_F4)->z = 0.5f;
  script_12426(5, arg0);
  Script_charaToggleMotEnabled(CHARA_AKIR, 1);
  SCNF_WaitFrames_0000(1);
  Script_scenCreateTask(script_12625, 2, 6, arg0);
  Script_scenCreateTask(script_10632, 0);
  Script_motmToggleFlags(CHARA_AKIR, 1, 0x40);
  for (;;) {
    local_CC = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_GetCharaMotionRemainingFramesScaled(CHARA_AKIR));
    if (Script_IsCharaMotionFinished(CHARA_AKIR)) {
      break;
    } else {
      switch (local_CC) {
        int32_t local_C4;
        case 30:
          Script_executeAicaCommand(0x3304A9, 0, 0);
          Script_EVT_GetPosition(CHARA_AKIR, 18, &local_E8, 0);
          Script_EVT_SetPosition(0x6E696F43, POS_XYZ, &local_E8);
          break;
        case 31:
          Script_charaDisplay(0x6E696F43, CDM_Show);
          local_DC.x = 0.124f;
          (&local_DC)->y = 0.012f;
          (&local_DC)->z = -0.085f;
          local_D0.x = 64990;
          (&local_D0)->y = 64260;
          (&local_D0)->z = 46788;
          Script_fixoAttachEx(0x6E696F43, CHARA_AKIR, 18, &local_DC, &local_D0);
          break;
        case 40:
          Script_SetNewHandMotion(CHARA_AKIR, 1, 861, 0, 0, 0, 0, 1.0f, 10);
          break;
        case 140:
          Script_charaDisplay(arg0, CDM_Show);
          task_238 = 0;
          task_234 = 0;
          task_254 = 100000;
          script_12017(arg0);
          Script_executeAicaCommand(0x3204A9, 0, 0);
          Script_fixoClearAttachment(0x6E696F43);
          Script_charaDisplay(0x6E696F43, CDM_Hide);
          local_C4 = 0;
          while (local_C4 < 5) {
            script_1225B(arg0, local_C4, 1);
            local_C4 = local_C4 + 1;
          }
          break;
      }
    }
    Script_RotateNodeUtils(CHARA_AKIR, &data_1BFF0, 1);
    SCNF_WaitFrames_0000(1);
  }
  script_12426(14, arg0);
  local_E8.x = -0.045f;
  (&local_E8)->y = 0.0f;
  (&local_E8)->z = 0.6f;
  script_12E53(CHARA_AKIR, arg0, &local_E8);
  Script_SetNewHandMotion(CHARA_AKIR, 0, 863, 0, 0, 0, 0, 1.0f, 0);
  Script_SetNewHandMotion(CHARA_AKIR, 1, 864, 0, 0, 0, 0, 1.0f, 30);
  Script_PlayCharaMotion(CHARA_AKIR, 31013, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_executeAicaCommand(0x3604A9, 0, 0);
  SCNF_WaitFrames_0000(1);
  for (;;) {
    local_CC = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_GetCharaMotionRemainingFramesScaled(CHARA_AKIR));
    if (Script_IsCharaMotionFinished(CHARA_AKIR)) {
      break;
    } else {
      switch (local_CC) {
        case 55:
          {
            int32_t local_C4;
            Script_executeAicaCommand(0x3704A9, 0, 0);
            Script_EVT_GetPosition(CHARA_AKIR, 12, &local_E8, 0);
            local_C4 = 0;
            L0103CB:;
            Script_EVT_SetPosition(*(&data_1E558 + local_C4), POS_XYZ, &local_E8);
            local_C4 = local_C4 + 1;
            if (!(local_C4 >= 5)) goto L0103CB;
            break;
          }
        case 56:
          {
            int32_t local_C4;
            local_C4 = 0;
            L010406:;
            Script_charaDisplay(*(&data_1E558 + local_C4), CDM_Show);
            Script_fixoSetAttachmentOffset(*(&data_1E558 + local_C4), 0, &data_1E610, &data_1E61C);
            local_DC.x = *(float *)(&data_1E628 + local_C4 * 3);
            (&local_DC)->y = *(float *)(&data_1E628 + (local_C4 * 3 + 1));
            (&local_DC)->z = *(float *)(&data_1E628 + (local_C4 * 3 + 2));
            local_D0.x = *(int32_t *)(data_1E664 + ((local_C4 * 3) << 2));
            (&local_D0)->y = *(int32_t *)(data_1E664 + ((local_C4 * 3 + 1) << 2));
            (&local_D0)->z = *(int32_t *)(data_1E664 + ((local_C4 * 3 + 2) << 2));
            Script_fixoAttachEx(*(&data_1E558 + local_C4), CHARA_AKIR, 12, &local_DC, &local_D0);
            local_C4 = local_C4 + 1;
            if (!(local_C4 >= 5)) goto L010406;
            break;
          }
        case 125:
          script_12426(10, arg0);
          break;
        case 150:
          Script_SetNewHandMotion(CHARA_AKIR, 1, 861, 0, 0, 0, 0, 1.0f, 20);
          break;
        case 180:
          Script_EVT_GetPosition(CHARA_AKIR, 18, &local_E8, 0);
          Script_EVT_SetPosition(SHEN_NAME('YA_Y'), POS_XYZ, &local_E8);
          break;
        case 181:
          Script_fixoSetAttachmentOffset(SHEN_NAME('YA_Y'), 0, &data_1E610, &data_1E61C);
          local_DC.x = 0.137f;
          (&local_DC)->y = 0.028f;
          (&local_DC)->z = -0.068f;
          local_D0.x = 0;
          (&local_D0)->y = 0;
          (&local_D0)->z = 15834;
          Script_fixoAttachEx(SHEN_NAME('YA_Y'), CHARA_AKIR, 18, &local_DC, &local_D0);
          break;
      }
    }
    Script_RotateNodeUtils(CHARA_AKIR, &local_F4, 1);
    SCNF_WaitFrames_0000(1);
  }
  Script_charaToggleMotEnabled(CHARA_AKIR, 0);
  Script_PlayCharaMotion(CHARA_AKIR, 31014, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  script_1271B(CHARA_AKIR);
  Script_shenClearFreeState(0);
  Script_sicnUtils(2, 1, 0);
  return;
}

static int32_t script_10632(void) {
  int32_t local_FC;
  local_FC = Script_shenGetStats(STATS_CASH);
  SCNF_SetCashValue(-100, 1);
  local_FC = local_FC - 100;
  Script_SetStats(STATS_CASH, local_FC);
  return Script_srchControl(0);
}

static void script_10661(int32_t arg0) {
  task_240 = 5;
  if (task_259 & 8) {
    task_250 = 0;
  } else {
    task_250 = 3;
  }
  Script_SetCharaRefBoardPos(CHARA_AKIR, &data_1E6A0);
  Script_charaSetRenderOpacity(CHARA_AKIR, 0.5f);
  Script_lightSetSlotEnabled(15, 1);
  task_252 = 0;
  return;
}

static void script_106B3(int32_t arg0) {
  int8_t local_FF;
  uint8_t unused_F8[7];
  int32_t local_F4;
  Vector3 local_E8;
  Vector3 local_DC;
  Vector3i local_D0;
  Vector3i local_C4;
  int32_t local_C0;
  int32_t local_BC;
  int32_t unused_B8;
  int32_t local_B4;
  local_FF = task_240 % 5;
  local_C0 = local_FF;
  switch (local_C0) {
    case 0:
      task_230 = 0x595F4159;
      break;
    case 4:
      task_230 = 0x525F4159;
      break;
    case 3:
      task_230 = 0x425F4159;
      break;
    case 2:
      task_230 = 0x475F4159;
      break;
    case 1:
      task_230 = 0x4B5F4159;
      break;
  }
  Script_charaDisplay(task_230, CDM_Show);
  if (task_240 == 1) {
    script_123BC(3, "E1025A022", "E1025A023", "E1025A024");
    while (Script_convGetIsActive((const void *)task_224)) {
      SCNF_WaitFrames_0000(1);
    }
  }
  Script_motmToggleFlags(CHARA_AKIR, 2, 0x40);
  if (task_259 & 8) {
    task_248 = (int32_t)(Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 360.0f) / 360.0f * (float)0x10000);
    task_24C = (int32_t)(Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 360.0f) / 360.0f * (float)0x10000);
    Script_PlayCharaMotion(CHARA_AKIR, 31014, 0.0f, 44.0f, 27.0f, 0.0f, 0);
  } else {
    task_24C = 0;
    Script_PlayCharaMotion(CHARA_AKIR, 31014, 0.0f, 44.0f, 0.0f, 0.0f, 0);
  }
  Script_SetNewHandMotion(CHARA_AKIR, 1, 861, 0, 0, 0, 0, 1.0f, 0);
  local_DC.x = -0.015f;
  (&local_DC)->y = -0.002f;
  (&local_DC)->z = 1.8f;
  local_C4.x = 0;
  (&local_C4)->y = 0;
  (&local_C4)->z = 0;
  script_12E53(CHARA_AKIR, arg0, &local_DC);
  script_12E92(CHARA_AKIR, arg0, &local_C4);
  script_11016(arg0);
  script_12426(3, arg0);
  SCNF_WaitFrames_0000(1);
  Script_motmToggleFlags(CHARA_AKIR, 1, 0x40);
  if (task_259 & 8) {
    Script_fixoClearAttachment(task_230);
    Script_MoveObjectToObject(CHARA_AKIR, task_230);
    Script_EVT_GetPosition(CHARA_AKIR, -1, &local_E8, 0);
    Script_mtwkGetSetRotation(CHARA_AKIR, &local_D0, 0);
    Script_MoveObjectToObject(CHARA_AKIR, -1);
    Script_EVT_GetPosition(task_230, -1, &local_DC, ENTITY_XFORM_WORLD_SPACE);
    Script_EVT_SetPosition(0x6E696F43, 0x78000000, &local_DC);
  } else {
    local_DC.x = (&local_DC)->x + 0.025f;
    script_12E53(CHARA_AKIR, arg0, &local_DC);
    Script_SetCharaMotionPlaybackRate(CHARA_AKIR, 1.0f);
  }
  task_254 = 100000;
  task_252 = 0;
  (void)((task_258 = -1) & 0x10);
  Script_sicnUtils(0, 4, 0);
  for (;;) {
    if (task_259 & 8) {
      script_10D4D();
      script_10C7A();
      script_10E41();
      script_10E05(&local_E8, &local_D0);
    } else {
      script_10F57(arg0);
    }
    SCNF_WaitFrames_0000(1);
    local_F4 = Script_GetControllerInput(1);
    if (local_F4 == 0x400) {
      Script_sicnUtils(1, 0, 0);
      if (task_259 & 8) {
        script_11244((int32_t)&local_B4);
      } else {
        script_1131D((int32_t)&local_B4);
      }
      script_11016(arg0);
      script_110F4(arg0, (int32_t)&local_B4);
      break;
    } else if (local_F4 == 0x200) {
      (void)((task_258 = 0) & 2);
      Script_SetCharaMotionPlaybackRate(CHARA_AKIR, 0.0f);
      Script_charaDisplay(CHARA_AKIR, CDM_Hide);
      Script_charaDisplay(task_230, CDM_Hide);
      local_BC = SCNF_RequestActionSelectorFromParams_0008(0xFFFF, 37, 34, 0xFFFF, 0x7FFF, 0xFFFF, ACTION_SELECTOR_MODE_STANDARD_TIMEOUT_CANCEL);
      if (local_BC != 37) {
        Script_sicnUtils(1, 0, 0);
        (void)((task_25A = -1) & 0x80);
        break;
      } else if (task_259 & 8) {
        task_248 = (int32_t)(Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 360.0f) / 360.0f * (float)0x10000);
        task_24C = (int32_t)(Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 360.0f) / 360.0f * (float)0x10000);
      } else {
        Script_SetCharaMotionPlaybackRate(CHARA_AKIR, 1.0f);
      }
      Script_charaDisplay(CHARA_AKIR, CDM_Show);
      Script_charaDisplay(task_230, CDM_Show);
      (void)((task_258 = -1) & 4);
    } else {
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_EV_STOP_ACTIVE) == 1) {
        Script_sicnUtils(1, 0, 0);
        (void)((task_25A = -1) & 4);
        break;
        goto L010B12;
      }
    }
    L010B12:;
  }
  return;
}

static void script_10B16(int32_t arg0) {
  Vector3 local_F4;
  Vector3i local_E8;
  Script_SetCharaRefBoardPos(CHARA_AKIR, &data_1E6D0);
  Script_lightSetSlotEnabled(15, 0);
  Script_charaSetRenderOpacity(CHARA_AKIR, 1.0f);
  Script_charaToggleMotEnabled(CHARA_AKIR, 1);
  Script_charaDisplay(arg0, CDM_Hide);
  Script_charaDisplay(CHARA_AKIR, CDM_Hide);
  Script_InitializeOsag(CHARA_AKIR);
  Script_motmToggleFlags(CHARA_AKIR, 2, 0x40);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  local_F4.x = 0.0f;
  (&local_F4)->y = 0.0f;
  (&local_F4)->z = 0.6f;
  local_E8.x = 0;
  (&local_E8)->y = 0;
  (&local_E8)->z = 0;
  script_12E53(CHARA_AKIR, arg0, &local_F4);
  script_12E92(CHARA_AKIR, arg0, &local_E8);
  script_122AD(3);
  SCNF_WaitFrames_0000(1);
  Script_SetCharaLookAtPosition(CHARA_AKIR, -20, 0, 0);
  Script_motmToggleFlags(CHARA_AKIR, 1, 0x40);
  SCNF_WaitFrames_0000(1);
  return;
}

static int32_t script_10C1E(void) {
  for (;;) {
    if (task_258 & 4) {
      script_10C3B();
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_10C3B(void) {
  if (~task_259) {
  } else if (task_254 > 0) {
    task_254 = 100000 - task_252 * 166;
    task_252 = task_252 + 1;
  } else {
    task_254 = 0;
  }
  return;
}

static void script_10C7A(void) {
  float local_FC;
  float local_F8;
  float local_F4;
  Vector3 local_E8;
  Vector3 local_DC;
  int32_t local_D8;
  int32_t local_D4;
  int32_t local_D0;
  local_F4 = 300.0f;
  local_D8 = Script_CEWPRelated(task_230);
  local_D4 = task_24C;
  local_D0 = task_24C + (int32_t)(360.0f / local_F4 / 360.0f * (float)0x10000);
  local_FC = Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, local_D4) * 0.04f;
  local_F8 = Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, local_D0) * 0.04f;
  Script_EVT_GetPosition(task_230, -1, &local_E8, 0);
  local_E8.x = local_FC + -0.119f;
  Script_EVT_SetPosition(task_230, POS_XYZ, &local_E8);
  Script_MoveObjectToObject(0x6E696F43, local_D8);
  Script_EVT_GetPosition(0x6E696F43, -1, &local_DC, 0);
  local_DC.x = local_F8 + -0.119f;
  Script_EVT_SetPosition(0x6E696F43, POS_XYZ, &local_DC);
  Script_MoveObjectToObject(0x6E696F43, -1);
  task_24C = local_D0;
  return;
}

static void script_10D4D(void) {
  float local_FC;
  float local_F8;
  int32_t local_F4;
  int32_t local_F0;
  Vector3 local_E4;
  Vector3 local_D8;
  int32_t local_D4;
  local_D4 = Script_CEWPRelated(task_230);
  local_F4 = task_248;
  local_F0 = task_248 + 18;
  script_12F6D(local_F4, &local_FC);
  script_12F6D(local_F0, &local_F8);
  Script_EVT_GetPosition(task_230, -1, &local_E4, 0);
  (&local_E4)->y = local_FC * 0.05f + 1.5891f;
  Script_EVT_SetPosition(task_230, POS_XYZ, &local_E4);
  Script_MoveObjectToObject(0x6E696F43, local_D4);
  Script_EVT_GetPosition(0x6E696F43, -1, &local_D8, 0);
  (&local_D8)->y = local_F8 * 0.05f + 1.5891f;
  Script_EVT_SetPosition(0x6E696F43, POS_XYZ, &local_D8);
  Script_MoveObjectToObject(0x6E696F43, -1);
  task_248 = local_F0;
  return;
}

static void script_10E05(const Vector3 *arg0, const Vector3i *arg1) {
  Script_MoveObjectToObject(CHARA_AKIR, task_230);
  Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, arg0);
  Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, arg1, 0);
  Script_MoveObjectToObject(CHARA_AKIR, -1);
  return;
}

static void script_10E41(void) {
  Vector3 local_F4;
  int32_t local_E8[3];
  Vector3i local_DC;
  float local_D8;
  Script_EVT_GetPosition(task_230, -1, &local_F4, 0);
  local_F4.x = (&local_F4)->x - -0.119f;
  (&local_F4)->y = (&local_F4)->y - 1.5891f;
  (&local_F4)->z = (&local_F4)->z + 0.0378f;
  local_E8[1] = Script_FloatUtils(ScriptMathOp_Atan2ReverseVector, (float)0 - (&local_F4)->x, (float)0 - (&local_F4)->z);
  local_D8 = Script_MathUtils(SCRIPT_MATH_VEC2_LENGTH, (&local_F4)->x, (&local_F4)->z);
  local_E8[0] = Script_FloatUtils(ScriptMathOp_Atan2ReverseVector, (float)0 - (&local_F4)->y, (float)0 - local_D8);
  local_E8[2] = 0;
  local_DC.x = 10279 - *(int32_t *)local_E8;
  (&local_DC)->y = 50706 - local_E8[1];
  (&local_DC)->z = 22478 - local_E8[2];
  if ((&local_DC)->x > 0x10000) {
    local_DC.x = (&local_DC)->x - 0x10000;
  }
  if ((&local_DC)->y > 0x10000) {
    (&local_DC)->y = (&local_DC)->y - 0x10000;
  }
  Script_EVT_SetRotation(task_230, 0x38000000, &local_DC, 0);
  return;
}

static void script_10F57(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  Vector3i local_EC;
  task_24C = task_24C % 90;
  if (task_24C <= 22) {
    local_F8 = task_24C;
    local_FC = local_F8 * 41;
  } else if (task_24C <= 44) {
    local_F8 = task_24C - 22;
    local_FC = 910 - local_F8 * 41;
  } else if (task_24C <= 67) {
    local_F8 = task_24C - 44;
    local_FC = -local_F8 * 39;
  } else {
    local_F8 = task_24C - 67;
    local_FC = local_F8 * 39 + -910;
  }
  local_EC.x = 0;
  (&local_EC)->y = -local_FC;
  (&local_EC)->z = 0;
  script_12E92(CHARA_AKIR, arg0, &local_EC);
  task_24C = task_24C + 1;
  return;
}

static void script_11016(int32_t arg0) {
  {
    Vector3 local_F4;
    Vector3i local_E8;
    local_F4.x = 0.0882f;
    (&local_F4)->y = -0.0112f;
    (&local_F4)->z = 0.0f;
    local_E8.x = 0;
    (&local_E8)->y = 0;
    (&local_E8)->z = 0;
    Script_fixoSetAttachmentOffset(task_230, 0, &local_F4, &local_E8);
  }
  {
    Vector3 local_F4;
    Vector3i local_E8;
    if (task_259 & 8) {
      local_F4.x = 0.136f;
      (&local_F4)->y = 0.024f;
      (&local_F4)->z = -0.073f;
      {
        local_E8.x = 62077;
        (&local_E8)->y = 2000;
        (&local_E8)->z = 0x3B00;
      }
    } else {
      local_F4.x = 0.137f;
      (&local_F4)->y = 0.028f;
      (&local_F4)->z = -0.068f;
      {
        local_E8.x = 0;
        (&local_E8)->y = 0;
        (&local_E8)->z = 15834;
      }
    }
    Script_fixoAttachEx(task_230, CHARA_AKIR, 18, &local_F4, &local_E8);
  }
  return;
}

static void script_110F4(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  Script_PlayCharaMotion(CHARA_AKIR, 31015, 0.0f, 70.0f, 0.0f, 1.0f, 0);
  for (;;) {
    SCNF_WaitFrames_0000(1);
    local_FC = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_GetCharaMotionRemainingFramesScaled(CHARA_AKIR));
    if (local_FC == 26) {
      script_12426(4, arg0);
      Script_executeAicaCommand(0x3804A9, 0, 0);
      Script_SetNewHandMotion(CHARA_AKIR, 1, 862, 0, 0, 0, 0, 1.0f, 0);
      Script_fixoClearAttachment(task_230);
      (void)((task_244 = 0) & 0x10);
      SCNF_WaitFrames_0000(1);
      Script_scenCreateTask(script_12625, 2, 8, arg0);
      Script_scenCreateTask(script_1143B, 2, arg0, arg1);
      break;
    }
  }
  for (;;) {
    if (task_244 == -1) {
      (void)((task_258 = 0) & 0x10);
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  script_1225B(arg0, 5 - task_240, 0);
  task_240 = task_240 - 1;
  if (task_240 > 0) {
    Script_PlayCharaMotion(CHARA_AKIR, 31014, 0.0f, 0.0f, 0.0f, 0.0f, 0);
  } else {
    if (task_240 <= 0) {
      Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
      goto L01123B;
    }
  }
  L01123B:;
  SCNF_WaitFrames_0000(15);
  return;
}

static void script_11244(int32_t arg0) {
  Vector3 local_F4;
  int32_t unused_F0;
  float local_E8[2];
  Vector3 local_DC;
  int32_t local_D8;
  local_D8 = Script_CEWPRelated(task_230);
  local_F4.x = -0.119f;
  (&local_F4)->y = 1.5891f;
  (&local_F4)->z = 1.3261f;
  Script_MoveObjectToObject(0x6E696F43, -1);
  Script_EVT_GetPosition(0x6E696F43, -1, &local_DC, 0);
  Script_ScreenToWorldAssign(&local_DC, (void *)local_E8);
  Script_MoveObjectToObject(0x6E696F43, local_D8);
  Script_EVT_SetPosition(0x6E696F43, POS_XYZ, &local_F4);
  Script_MoveObjectToObject(0x6E696F43, -1);
  Script_EVT_GetPosition(0x6E696F43, -1, &local_F4, 0);
  Script_ScreenToWorldAssign(&local_F4, &local_F4);
  *(float *)arg0 = (*local_E8 - (&local_F4)->x) * 0.166f / 85.5f;
  *(float *)(arg0 + 4) = ((&local_F4)->y - local_E8[1]) * 0.166f / 85.5f;
  Script_EVT_SetPosition(0x6E696F43, POS_XYZ, &local_DC);
  return;
}

static void script_1131D(int32_t arg0) {
  float local_FC;
  local_FC = Script_GetCharaMotionRemainingFramesScaled(CHARA_AKIR);
  if (task_24C <= 22) {
    int32_t local_F8;
    local_F8 = task_24C;
    *(float *)arg0 = (float)local_F8 * 0.0090909088f;
  } else if (task_24C <= 44) {
    int32_t local_F8;
    local_F8 = task_24C - 22;
    *(float *)arg0 = 0.2f - (float)local_F8 * 0.0090909088f;
  } else if (task_24C <= 67) {
    int32_t local_F8;
    local_F8 = task_24C - 44;
    *(float *)arg0 = (float)local_F8 * -0.00869565271f;
  } else {
    int32_t local_F8;
    local_F8 = task_24C - 67;
    *(float *)arg0 = (float)local_F8 * 0.00869565271f + -0.2f;
  }
  if (!(local_FC > 13.0f)) {
    *(float *)(arg0 + 4) = local_FC * -0.2f / 13.0f;
  } else if (!(local_FC > 37.0f)) {
    local_FC = local_FC - 13.0f;
    *(float *)(arg0 + 4) = local_FC * 0.4f / 24.0f + -0.2f;
  } else {
    local_FC = local_FC - 37.0f;
    *(float *)(arg0 + 4) = 0.2f - local_FC * 0.2f / 7.0f;
  }
  return;
}

static int32_t script_1143B(int32_t arg0, int32_t arg1) {
  Vector3 local_F4;
  float local_E8[3];
  float local_DC[3];
  float local_D8;
  float local_D4;
  int8_t local_D3;
  Vector3i local_C4;
  int32_t local_C0;
  local_C4.x = 0;
  (&local_C4)->y = 0xC000;
  (&local_C4)->z = 0;
  Script_EVT_SetRotation(task_230, 0x38000000, &local_C4, 0);
  local_E8[0] = *(float *)arg1;
  local_E8[1] = *(float *)(arg1 + 4);
  local_E8[2] = -0.0682000071f;
  if (task_259 & 8) {
    local_D8 = 0.0f;
    local_D4 = 0.0f;
  } else {
    local_D8 = Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 0.034f) - 0.017f;
    local_D4 = Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 0.034f) - 0.017f;
  }
  Script_EVT_GetPosition(task_230, -1, &local_F4, 0);
  local_DC[0] = (*local_E8 + local_D8 - (&local_F4)->x) / 13.0f;
  local_DC[1] = (local_E8[1] + local_D4 + 1.4174f - (&local_F4)->y) / 13.0f;
  local_DC[2] = (local_E8[2] - (&local_F4)->z) / 13.0f;
  local_D3 = Script_FloatUtils(ScriptMathOp_FloatToInt, 13.0f);
  for (;;) {
    local_F4.x = (&local_F4)->x + *local_DC;
    (&local_F4)->y = (&local_F4)->y + local_DC[1];
    (&local_F4)->z = (&local_F4)->z + local_DC[2];
    Script_EVT_SetPosition(task_230, POS_XYZ, &local_F4);
    local_D3 = local_D3 - 1;
    if (local_D3 == 0) {
      local_C0 = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f));
      switch (local_C0) {
        case 0:
          Script_executeAicaCommand(0x3404A9, 0, 0);
          break;
        case 1:
          Script_executeAicaCommand(0x3504A9, 0, 0);
          break;
      }
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_EVT_GetPosition(task_230, -1, &local_F4, 0);
  task_234 = script_12759((&local_F4)->x, (&local_F4)->y - 1.4174f);
  (void)((task_244 = -1) & 0x10);
  return Script_srchControl(0);
}

__attribute__((scn_string_bindings(3, data_1E6FE, 5, data_1E712)))
static int32_t script_1161F(int32_t arg0) {
  int32_t local_FC;
  int8_t local_FB;
  if (task_259 & 8) {
    local_FC = task_234 / 10000;
  } else {
    local_FC = task_234 / 2;
  }
  SCNF_WaitFrames_0000(1);
  if (local_FC == 0) {
    script_123BC(2, "E1025A016", "E1025A017");
  } else if (local_FC <= 10) {
    script_123AD("E1025A013");
  } else if (local_FC <= 15) {
    script_123BC(2, "E1025A018", "E1025A048");
  } else if (local_FC <= 20) {
    script_123BC(2, "E1025A018", "E1025A046");
  } else if (local_FC <= 30) {
    script_123BC(2, "E1025A012", "E1025A047");
  } else if (local_FC <= 45) {
    script_123AD("E1025A014");
  } else {
    script_123AD("E1025A015");
  }
  while (Script_convGetIsActive((const void *)task_224)) {
    SCNF_WaitFrames_0000(1);
  }
  SCNF_WaitFrames_0000(24);
  local_FC = task_234;
  script_11D86((int32_t)&local_FC);
  SCNF_WaitFrames_0000(36);
  if (-(task_254 > 0) & task_259) {
    task_25B = task_25B | 0x40 | 0x20;
    script_11D86((int32_t)&task_254);
    task_25B = task_25B & -97;
    SCNF_WaitFrames_0000(30);
  }
  if (-(task_240 == 0) & task_259) {
    task_250 = task_250 + 1;
    if ((task_250 <= 2) & (task_238 / 10000 >= task_250 * 120)) {
      uint8_t unused_F8[3];
      int32_t local_F4;
      script_1190C(arg0);
      task_240 = 5;
      local_F4 = 0;
      while (local_F4 < 5) {
        Script_charaDisplay(*(&data_1E558 + local_F4), CDM_Hide);
        script_1225B(arg0, local_F4, 1);
        local_F4 = local_F4 + 1;
      }
      Script_executeAicaCommand(131496, 0, 0);
    }
  }
  if (task_240 == 0) {
    uint8_t unused_F8[3];
    int32_t local_F4;
    local_F4 = 0;
    L01180B:;
    task_25B = 15;
    if (local_F4 % 4 < 2) {
      task_25B = task_25B & -2;
    }
    SCNF_WaitFrames_0000(1);
    local_F4 = local_F4 + 1;
    if (!(local_F4 == 30)) goto L01180B;
    task_25B = 15;
    task_220 = task_238;
    if (task_21C < task_238) {
      int8_t local_F3;
      task_21C = task_238;
      (void)((local_F3 = 0) & 2);
      if (task_259 & 8) {
        if (task_21C > 777777) {
          (void)((local_F3 = -1) & 2);
        }
      } else {
        if (task_21C > 50) {
          (void)((local_F3 = -1) & 2);
        }
      }
      if (local_F3 & 8) {
        Script_executeAicaCommand(65960, 0, 0);
        script_123BC(3, "E1025A050", "E1025A051", "E1025A052");
        task_238 = 0;
        SCNF_WaitFrames_0000(90);
      }
    }
    task_238 = 0;
    SCNF_WaitFrames_0000(15);
    (void)((local_FB = 0) & 2);
  } else {
    (void)((local_FB = -1) & 2);
  }
  return local_FB & 8;
}

static void script_1190C(int32_t arg0) {
  Vector3 local_F4;
  int8_t local_EC[8];
  int32_t local_E8;
  int32_t local_E4;
  int8_t local_E3;
  task_25B = 0;
  Script_executeAicaCommand(424, 0, 0);
  local_F4.x = -0.127600014f;
  (&local_F4)->y = 2.155f;
  (&local_F4)->z = -0.04f;
  local_E3 = task_250 + 1;
  local_EC[0] = 0;
  if (local_E3 == 2) {
    local_EC[1] = 1;
    local_EC[2] = 2;
  } else {
    local_EC[1] = 3;
    local_EC[2] = 2;
  }
  local_EC[3] = 0;
  local_EC[4] = 4;
  local_EC[5] = 5;
  local_EC[6] = 6;
  local_EC[7] = 7;
  if (local_E3 == 2) {
    script_123BC(3, "E1025A037", "E1025A038", "E1025A039");
  }
  if (local_E3 == 3) {
    script_123BC(3, "E1025A040", "E1025A041", "E1025A042");
  }
  local_E4 = 0;
  while (local_E4 < 80) {
    Script_GameUtils(arg0, GC_CommitStagedEntries, 0, 0, 0, 0);
    Script_GameUtils(arg0, GC_ConfigureNumericDisplay, 0x100, &data_1E7A8, 9, 0);
    Script_GameUtils(arg0, GC_StageNumericDisplayValue, local_E3, &local_F4, 1.0f, 0);
    local_E8 = 0;
    L011A3F:;
    if (local_E4 >= local_E8 * 7) {
      script_11C67(local_EC[local_E8], local_E8, arg0);
    }
    local_E8 = local_E8 + 1;
    if (!(local_E8 == 8)) goto L011A3F;
    script_11AAC(arg0, 2);
    if (local_E4 % 4 < 2) {
      script_11AAC(arg0, 3);
    }
    local_E4 = local_E4 + 1;
    SCNF_WaitFrames_0000(1);
  }
  task_25B = 15;
  return;
}

static void script_11AAC(int32_t arg0, int32_t arg1) {
  Vector3 local_F4;
  int32_t local_F0;
  int32_t local_EC;
  int32_t local_E8;
  int32_t local_E4;
  local_E4 = arg1;
  switch (local_E4) {
    case 0:
      local_F0 = task_21C;
      if (local_F0 < 777777) {
        local_F0 = 777777;
      }
      break;
    case 1:
      local_F0 = task_220;
      break;
    case 2:
      local_F0 = task_254;
      break;
    case 3:
      local_F0 = task_238;
      break;
    case 4:
      local_F0 = task_234;
      break;
  }
  local_EC = local_F0 / 10000;
  local_E8 = local_F0 - local_EC * 10000;
  if (-(arg1 == 3) & (task_25B & 0x20)) {
    local_E8 = Script_FloatUtils(ScriptMathOp_RandomIntBelow, 10000);
  }
  if (-(arg1 == 2) & (task_25B & 0x40)) {
    local_E8 = Script_FloatUtils(ScriptMathOp_RandomIntBelow, 10000);
  }
  local_F4.x = *(float *)&data_1E7CC;
  (&local_F4)->y = *(float *)(&data_1E7E4 + arg1);
  (&local_F4)->z = *(float *)(&data_1E7CC + 2);
  Script_GameUtils(arg0, GC_ConfigureNumericDisplay, 0x100, &data_1E7B4, 9999, 0);
  Script_GameUtils(arg0, GC_StageNumericDisplayValue, local_EC, &local_F4, 1.0f, 0);
  local_F4.x = *(float *)&data_1E7C0 * (float)4 + 0.1474f;
  (&local_F4)->y = *(float *)(&data_1E7F8 + arg1);
  (&local_F4)->z = -0.04f;
  Script_GameUtils(arg0, GC_StageNodeData, 336, &local_F4, 1.0f, &data_1E80C);
  local_F4.x = *(float *)&data_1E7D8;
  (&local_F4)->y = *(float *)(&data_1E7F8 + arg1);
  (&local_F4)->z = *(float *)(&data_1E7D8 + 2);
  script_121B2(arg0, 0x100, (const Vector3 *)&data_1E7C0, 4, local_E8, &local_F4, &data_1E80C);
  return;
}

static void script_11C67(int8_t arg0, int8_t arg1, int32_t arg2) {
  int32_t local_FC;
  Vector3 local_F0;
  int32_t local_EC;
  local_EC = arg0;
  if (local_EC == 5) {
    local_FC = 292;
  } else if (local_EC == 2) {
    local_FC = 295;
  } else if (local_EC == 7) {
    local_FC = 296;
  } else if (local_EC == 4) {
    local_FC = 298;
  } else if (local_EC == 6) {
    local_FC = 330;
  } else if (local_EC == 1) {
    local_FC = 331;
  } else if (local_EC == 3) {
    local_FC = 335;
  } else {
    local_FC = 0;
  }
  if (arg1 < 4) {
    local_F0.x = (float)(arg1 - 3) * 0.05f + 0.0224f;
    (&local_F0)->y = 2.155f;
    (&local_F0)->z = -0.04f;
  } else {
    arg1 = arg1 - 4;
    local_F0.x = (float)(arg1 - 3) * 0.05f + 0.0224f;
    (&local_F0)->y = 2.005f;
    (&local_F0)->z = -0.04f;
  }
  if (local_FC) {
    Script_GameUtils(arg2, GC_StageNodeData, local_FC, &local_F0, 1.0f, 0);
  }
  return;
}

static void script_11D86(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  int32_t local_F0;
  if (task_259 & 8) {
    local_F8 = *(int32_t *)arg0 / 10000;
  } else {
    local_F8 = *(int32_t *)arg0;
  }
  if (local_F8 == 0) {
    SCNF_WaitFrames_0000(1);
  } else {
    if (local_F8 < 20) {
      local_FC = 1;
      local_F4 = 2;
    } else if (local_F8 < 40) {
      local_FC = 1;
      local_F4 = 1;
    } else {
      local_FC = 2;
      local_F4 = 1;
    }
    if (task_259 & 8) {
      local_FC = local_FC * 10000;
      local_F8 = local_F8 * 10000;
    }
    local_F0 = local_F4;
    Script_executeAicaCommand(0x3904A9, 0, 0);
    for (;;) {
      if (local_F8 == 0) {
        Script_executeAicaCommand(0x3A04A9, 0, 0);
        break;
      } else if (local_F0 == 0) {
        if (local_F8 < local_FC) {
          local_FC = local_F8;
        }
        local_F8 = local_F8 - local_FC;
        task_238 = task_238 + local_FC;
        *(int32_t *)arg0 = *(int32_t *)arg0 - local_FC;
        local_F0 = local_F4;
      } else {
        if (local_F0 != 0) {
          local_F0 = local_F0 - 1;
          goto L011E98;
        }
      }
      L011E98:;
      SCNF_WaitFrames_0000(1);
    }
  }
  task_238 = task_238 + *(int32_t *)arg0;
  *(int32_t *)arg0 = 0;
  return;
}

__attribute__((scn_string_bindings(6, data_1E854)))
static int32_t script_11EB6(int32_t arg0) {
  int32_t local_FC;
  int8_t local_FB;
  int8_t local_FA;
  (void)((local_FB = 0) & 1);
  Script_charaDisplay(CHARA_AKIR, CDM_Show);
  script_12426(7, arg0);
  script_122AD(2, arg0);
  script_12017(arg0);
  script_123BC(3, "E1025A025", "E1025A026", "E1025A027");
  while (Script_convGetIsActive((const void *)task_224)) {
    SCNF_WaitFrames_0000(1);
  }
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_EV_STOP_ACTIVE) == 1) {
    (void)0;
  } else {
    local_FC = SCNF_RequestActionSelectorFromParams_0008(0xFFFF, 37, 34, 0xFFFF, 0x7FFF, 0xFFFF, ACTION_SELECTOR_MODE_STANDARD_TIMEOUT_CANCEL);
    local_FA = script_AFDB(2, 100, 1);
    if ((local_FC == 37) & (local_FA == 0)) {
      script_123BC(3, "E1025A028", "E1025A029", "E1025A030");
      while (Script_convGetIsActive((const void *)task_224)) {
        SCNF_WaitFrames_0000(1);
      }
      (void)((local_FB = -1) & 4);
    } else {
      switch (local_FA) {
        case 0:
          script_123BC(3, "E1025A011", "E1025A031", "E1025A032");
          while (Script_convGetIsActive((const void *)task_224)) {
            SCNF_WaitFrames_0000(1);
          }
          break;
        case 1:
          script_FFC6();
          break;
      }
    }
    (void)(local_FB & 8);
  }
  end:;
}

static int32_t script_12000(int32_t arg0) {
  for (;;) {
    script_12017(arg0);
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_12017(int32_t arg0) {
  if (arg0 != 0x33524144) {
    script_1203B(arg0);
  } else {
    script_120C1(arg0);
  }
  return;
}

static void script_1203B(int32_t arg0) {
  int8_t local_FF;
  local_FF = task_25B;
  if (task_25B) {
    Script_GameUtils(arg0, GC_CommitStagedEntries, 0, 0, 0, 0);
    if (local_FF & 2) {
      script_11AAC(arg0, 0);
    }
    if (local_FF & 4) {
      script_11AAC(arg0, 1);
    }
    if (local_FF & 8) {
      script_11AAC(arg0, 2);
    }
    if (local_FF & 1) {
      script_11AAC(arg0, 3);
    }
    if (local_FF & 0x10) {
      script_11AAC(arg0, 4);
    }
  }
  return;
}

static void script_120C1(int32_t arg0) {
  int8_t local_FF;
  int32_t local_F8;
  int32_t local_F4;
  int32_t local_F0;
  local_FF = task_25B;
  local_F8 = task_21C;
  local_F4 = task_220;
  local_F0 = task_238;
  if (local_F8 < 50) {
    local_F8 = 50;
  }
  if (task_25B) {
    Script_GameUtils(arg0, GC_CommitStagedEntries, 0, 0, 0, 0);
    if (local_FF & 1) {
      Script_GameUtils(arg0, GC_ConfigureNumericDisplay, 0x100, &data_1E874, 9999, 0);
      Script_GameUtils(arg0, GC_StageNumericDisplayValue, local_F0, &data_1E88C, 1.0f, &data_1E8BC);
    }
    if (local_FF & 4) {
      Script_GameUtils(arg0, GC_ConfigureNumericDisplay, 0x100, &data_1E880, 9999, 0);
      Script_GameUtils(arg0, GC_StageNumericDisplayValue, local_F4, &data_1E898, 1.0f, &data_1E8B0);
    }
    if (local_FF & 2) {
      Script_GameUtils(arg0, GC_ConfigureNumericDisplay, 0x100, &data_1E880, 9999, 0);
      Script_GameUtils(arg0, GC_StageNumericDisplayValue, local_F8, &data_1E8A4, 1.0f, &data_1E8B0);
    }
  }
  return;
}

static void script_121B2(int32_t arg0, int32_t arg1, const Vector3 *arg2, int32_t arg3, int32_t arg4, const Vector3 *arg5, const Vector3 *arg6) {
  int32_t local_FC;
  int32_t local_F8;
  Vector3 local_EC;
  local_EC.x = arg5->x;
  (&local_EC)->y = arg5->y;
  (&local_EC)->z = arg5->z;
  Script_GameUtils(arg0, GC_ConfigureNumericDisplay, arg1, arg2, 9, 0);
  local_FC = 0;
  while (local_FC < arg3) {
    local_F8 = arg4 % 10;
    Script_GameUtils(arg0, GC_StageNumericDisplayValue, local_F8, &local_EC, 1.0f, arg6);
    local_EC.x = (&local_EC)->x + arg2->x;
    (&local_EC)->y = (&local_EC)->y + arg2->y;
    (&local_EC)->z = (&local_EC)->z + arg2->z;
    arg4 = arg4 / 10;
    local_FC = local_FC + 1;
  }
  return;
}

static void script_1225B(int32_t arg0, int32_t arg1, int32_t arg2) {
  Vector3 local_F4;
  Vector3i local_E8;
  local_F4.x = (&local_F4)->y = (&local_F4)->z = 0.0f;
  local_E8.x = (&local_E8)->y = (&local_E8)->z = 0;
  if (arg2) {
    (&local_F4)->z = -10.05f;
  }
  Script_charaPosRotUtils(arg0, *(&data_1E8C8 + arg1), &local_F4, &local_E8, 0);
  return;
}

static void script_122AD(int32_t arg0, ...) {
  Vector3 local_F4;
  int32_t unused_DC[6];
  switch (arg0) {
    case 0:
      Script_SetCharaLookAtPosition(CHARA_AKIR, -1, 0, 0);
      Script_FaceLookAtTargetUtils(CHARA_AKIR, 0);
      break;
    case 1:
      {
        va_list arguments0;
        va_start(arguments0, arg0);
        Script_EVT_GetPosition(va_arg(arguments0, int32_t), -1, &local_F4, 0);
        va_end(arguments0);
      }
      (&local_F4)->y = (&local_F4)->y + 1.4f;
      Script_SetCharaLookAtPosition(CHARA_AKIR, 20, &local_F4, 2);
      Script_FaceLookAtTargetUtils(CHARA_AKIR, 2, &local_F4, 5);
      break;
    case 2:
      {
        va_list arguments1;
        va_start(arguments1, arg0);
        Script_EVT_GetPosition(va_arg(arguments1, int32_t), -1, &local_F4, 0);
        va_end(arguments1);
      }
      (&local_F4)->y = (&local_F4)->y + 1.4f;
      Script_SetCharaLookAtPosition(CHARA_AKIR, 98334, &local_F4, 2);
      Script_FaceLookAtTargetUtils(CHARA_AKIR, 2, &local_F4, 15);
      break;
    case 3:
      Script_SetMotnFlag(CHARA_AKIR, -1);
      Script_SetCharaLookAtPosition(CHARA_AKIR, -20, 0, 0);
      Script_SetMotnFlag(CHARA_AKIR, 0);
      Script_FaceLookAtTargetUtils(CHARA_AKIR, 0);
      break;
  }
  return;
}

static void script_123AD(const char *arg0) {
  task_224 = Script_ShowUIMessageFromID(arg0);
  return;
}

static void script_123BC(int32_t arg0, const char *arg1, const char *arg2, ...) {
  int8_t local_FF;
  int32_t local_F8;
  local_FF = Script_FloatUtils(ScriptMathOp_RandomIntBelow, arg0);
  local_F8 = local_FF;
  if (local_F8 == 1) {
    task_224 = Script_ShowUIMessageFromID(arg2);
  } else if (local_F8 == 2) {
    {
      va_list arguments0;
      va_start(arguments0, arg2);
      task_224 = Script_ShowUIMessageFromID(va_arg(arguments0, const char *));
      va_end(arguments0);
    }
  } else if (local_F8 == 3) {
    {
      va_list arguments1;
      va_start(arguments1, arg2);
      (void)va_arg(arguments1, const char *);
      task_224 = Script_ShowUIMessageFromID(va_arg(arguments1, const char *));
      va_end(arguments1);
    }
  } else {
    task_224 = Script_ShowUIMessageFromID(arg1);
  }
  return;
}

static void script_12426(int32_t arg0, int32_t arg1) {
  int8_t local_FF;
  int32_t local_F8;
  switch (arg1) {
    case 0x54524144:
      (void)((local_FF = -1) & 0x10);
      break;
    case 0x32524144:
      (void)((local_FF = 0) & 0x10);
      break;
  }
  (void)((local_FF = 0) & 0x10);
  local_F8 = arg0;
  switch (local_F8) {
    case 0:
      Script_SetGameStateTo(AUTH_None);
      break;
    case 1:
      Script_SetGameStateTo(AUTH_FirstPerson);
      break;
    case 2:
      Script_seqEnterEvtCam(7200, arg1, arg1);
      break;
    case 3:
      if (task_259 & 8) {
        Script_seqEnterEvtCam(7201, arg1, arg1);
      } else {
        Script_seqEnterEvtCam(7218, arg1, arg1);
      }
      break;
    case 4:
      Script_seqEnterEvtCam(7202, task_230, task_230);
      break;
    case 5:
      Script_seqEnterEvtCam(7203, arg1, arg1);
      break;
    case 6:
      if (local_FF & 0x80) {
        Script_seqEnterEvtCam(7204, arg1, arg1);
      } else {
        Script_seqEnterEvtCam(7216, arg1, arg1);
      }
      break;
    case 7:
      Script_seqEnterEvtCam(7205, CHARA_AKIR, CHARA_AKIR);
      break;
    case 8:
      Script_seqEnterEvtCam(7206, arg1, arg1);
      break;
    case 9:
      Script_seqEnterEvtCam(7207, arg1, arg1);
      break;
    case 10:
      if (local_FF & 0x80) {
        Script_seqEnterEvtCam(7208, arg1, arg1);
      } else {
        Script_seqEnterEvtCam(7217, arg1, arg1);
      }
      break;
    case 11:
      Script_seqEnterEvtCam(7209, CHARA_AKIR, CHARA_AKIR);
      break;
    case 12:
      if (local_FF & 0x80) {
        Script_seqEnterEvtCam(7210, arg1, arg1);
      } else {
        Script_seqEnterEvtCam(7219, arg1, arg1);
      }
      break;
    case 13:
      Script_seqEnterEvtCam(7211, arg1, arg1);
      break;
    case 14:
      Script_seqEnterEvtCam(7212, arg1, arg1);
      break;
  }
  return;
}

static int32_t script_12625(int32_t arg0, int32_t arg1) {
  for (;;) {
    if (Script_IsInEventCam() == 0) {
      script_12426(arg0, arg1);
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  return Script_srchControl(0);
}

static void script_12651(int32_t arg0) {
  Vector4 local_F0;
  int32_t local_E8[2];
  Vector3 local_DC;
  Script_SetCharaRefBoardPos(CHARA_AKIR, &data_1E8DC);
  Script_lightResetSlotToDefaults(15);
  Script_LightUtils(15, 0, 3);
  local_F0.x = 0.0f;
  (&local_F0)->y = 1.0f;
  (&local_F0)->z = 1.0f;
  (&local_F0)->w = 0.93f;
  Script_LightUtils(15, 2, &local_F0);
  Script_LightUtils(15, 4, 0.5f);
  Script_LightUtils(15, 5, 0.74f);
  local_E8[0] = 0x3F3851EC;
  local_E8[1] = 0;
  Script_LightUtils(15, 6, (const float *)local_E8);
  local_DC.x = -0.28f;
  (&local_DC)->y = 1.59f;
  (&local_DC)->z = 1.11f;
  script_12EB4(arg0, &local_DC);
  Script_LightUtils(15, 7, &local_DC);
  Script_lightSetSlotEnabled(15, 0);
  return;
}

static void script_1271B(int32_t arg0) {
  for (;;) {
    SCNF_WaitFrames_0000(1);
    if (Script_IsCharaMotionFinished(arg0)) {
      break;
    }
  }
  return;
}

static void script_12735(int32_t arg0, const Vector3 *arg1) {
  for (;;) {
    if (Script_IsCharaMotionFinished(arg0)) {
      break;
    }
    Script_RotateNodeUtils(arg0, arg1, 1);
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static int32_t script_12759(float arg0, float arg1) {
  int32_t local_FC;
  local_FC = 0;
  if (task_259 & 8) {
    local_FC = script_12792(arg0, arg1);
  } else {
    local_FC = script_128AC(arg0, arg1);
  }
  return local_FC;
}

static int32_t script_12792(float arg0, float arg1) {
  int32_t local_FC;
  float local_F8;
  int32_t local_F4;
  int8_t local_F3;
  int16_t local_F0;
  arg0 = arg0 * 1.0f;
  arg1 = arg1 * 1.00721359f;
  local_F8 = Script_MathUtils(SCRIPT_MATH_VEC2_LENGTH, arg0, arg1);
  if (!(local_F8 > 0.012f)) {
    local_FC = 50;
  } else if (!(local_F8 > 0.024f)) {
    local_FC = 25;
  } else {
    local_F4 = Script_FloatUtils(ScriptMathOp_Atan2ReverseVector, (float)0 - arg0, (float)0 - arg1);
    local_F4 = local_F4 + 1638;
    if (local_F4 >= 0x10000) {
      local_F4 = local_F4 - 0x10000;
    }
    local_F3 = local_F4 / 3276;
    if (local_F3 >= 20) {
      local_F3 = 0;
    }
    local_F0 = data_1E8EC[local_F3];
    if (!(local_F8 > 0.088f)) {
      local_FC = local_F0;
    } else if (!(local_F8 > 0.104f)) {
      local_FC = local_F0 * 3;
    } else if (!(local_F8 > 0.1492f)) {
      local_FC = local_F0;
    } else if (!(local_F8 > 0.166f)) {
      local_FC = local_F0 * 2;
    } else {
      local_FC = 0;
    }
  }
  return local_FC * 10000;
}

static int32_t script_128AC(float arg0, float arg1) {
  int32_t local_FC;
  float local_F8;
  arg0 = arg0 * 1.0f;
  arg1 = arg1 * 1.00721359f;
  local_F8 = Script_MathUtils(SCRIPT_MATH_VEC2_LENGTH, arg0, arg1);
  if (!(local_F8 > 0.016f)) {
    local_FC = 100;
  } else if (!(local_F8 > 0.047f)) {
    local_FC = 50;
  } else if (!(local_F8 > 0.079f)) {
    local_FC = 40;
  } else if (!(local_F8 > 0.11f)) {
    local_FC = 30;
  } else if (!(local_F8 > 0.147f)) {
    local_FC = 20;
  } else if (!(local_F8 > 0.201f)) {
    local_FC = 10;
  } else {
    local_FC = 0;
  }
  return local_FC;
}

static int32_t script_1295D(void) {
  task_23C = 0;
  for (;;) {
    task_23C = task_23C + 1;
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_1297D(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  int32_t local_F0;
  local_FC = 0;
  while (arg0 != *(&data_1E54C + local_FC)) {
    local_FC = local_FC + 1;
  }
  local_F4 = *((int8_t *)&data_1E51C + (local_FC * 16 + 14));
  local_F0 = arg1;
  switch (local_F0) {
    case 0:
      local_F0 = arg2;
      switch (local_F0) {
        case 0:
          local_F0 = local_F4;
          switch (local_F0) {
            case 0:
              local_F8 = Script_PlayerStatsUtils(PS_GetDartsModePlayCountSlot, 4);
              break;
            case 1:
              local_F8 = Script_PlayerStatsUtils(PS_GetDartsModePlayCountSlot, 5);
              break;
            case 2:
              local_F8 = Script_PlayerStatsUtils(PS_GetDartsModePlayCountSlot, 6);
              break;
            case 3:
              local_F8 = Script_PlayerStatsUtils(PS_GetDartsModePlayCountSlot, 7);
              break;
            case 4:
              local_F8 = Script_PlayerStatsUtils(PS_GetDartsModePlayCountSlot, 8);
              break;
          }
          break;
        case 1:
          local_F0 = local_F4;
          switch (local_F0) {
            case 0:
              local_F8 = Script_PlayerStatsUtils(PS_GetDartsModeActiveFramesSlot, 4);
              break;
            case 1:
              local_F8 = Script_PlayerStatsUtils(PS_GetDartsModeActiveFramesSlot, 5);
              break;
            case 2:
              local_F8 = Script_PlayerStatsUtils(PS_GetDartsModeActiveFramesSlot, 6);
              break;
            case 3:
              local_F8 = Script_PlayerStatsUtils(PS_GetDartsModeActiveFramesSlot, 7);
              break;
            case 4:
              local_F8 = Script_PlayerStatsUtils(PS_GetDartsModeActiveFramesSlot, 8);
              break;
          }
          break;
        case 2:
          local_F0 = local_F4;
          switch (local_F0) {
            case 0:
              local_F8 = Script_GetHighScoreValue(Highscore_DART_Primary);
              break;
            case 1:
              local_F8 = Script_GetHighScoreValue(Highscore_DAR2_Primary);
              break;
            case 2:
              local_F8 = Script_GetHighScoreValue(Highscore_DAR3_Primary);
              break;
            case 3:
              local_F8 = Script_GetHighScoreValue(Highscore_DAR4_Primary);
              break;
            case 4:
              local_F8 = Script_GetHighScoreValue(Highscore_DAR5_Primary);
              break;
          }
          break;
        case 3:
          local_F0 = local_F4;
          switch (local_F0) {
            case 0:
              local_F8 = Script_GetHighScoreValue(Highscore_DART_Secondary);
              break;
            case 1:
              local_F8 = Script_GetHighScoreValue(Highscore_DAR2_Secondary);
              break;
            case 2:
              local_F8 = Script_GetHighScoreValue(Highscore_DAR3_Secondary);
              break;
            case 3:
              local_F8 = Script_GetHighScoreValue(Highscore_DAR4_Secondary);
              break;
            case 4:
              local_F8 = Script_GetHighScoreValue(Highscore_DAR5_Secondary);
              break;
          }
          break;
      }
      break;
    case 1:
      local_F0 = arg2;
      switch (local_F0) {
        case 0:
          local_F0 = local_F4;
          switch (local_F0) {
            case 0:
              local_F8 = Script_PlayerStatsUtils(PS_SetDartsModePlayCountSlot, 4, arg3);
              break;
            case 1:
              local_F8 = Script_PlayerStatsUtils(PS_SetDartsModePlayCountSlot, 5, arg3);
              break;
            case 2:
              local_F8 = Script_PlayerStatsUtils(PS_SetDartsModePlayCountSlot, 6, arg3);
              break;
            case 3:
              local_F8 = Script_PlayerStatsUtils(PS_SetDartsModePlayCountSlot, 7, arg3);
              break;
            case 4:
              local_F8 = Script_PlayerStatsUtils(PS_SetDartsModePlayCountSlot, 8, arg3);
              break;
          }
          break;
        case 1:
          local_F0 = local_F4;
          switch (local_F0) {
            case 0:
              local_F8 = Script_PlayerStatsUtils(PS_SetDartsModeActiveFramesSlot, 4, arg3);
              break;
            case 1:
              local_F8 = Script_PlayerStatsUtils(PS_SetDartsModeActiveFramesSlot, 5, arg3);
              break;
            case 2:
              local_F8 = Script_PlayerStatsUtils(PS_SetDartsModeActiveFramesSlot, 6, arg3);
              break;
            case 3:
              local_F8 = Script_PlayerStatsUtils(PS_SetDartsModeActiveFramesSlot, 7, arg3);
              break;
            case 4:
              local_F8 = Script_PlayerStatsUtils(PS_SetDartsModeActiveFramesSlot, 8, arg3);
              break;
          }
          break;
        case 2:
          local_F0 = local_F4;
          switch (local_F0) {
            case 0:
              Script_SetHighScoreValue(Highscore_DART_Primary, arg3);
              break;
            case 1:
              Script_SetHighScoreValue(Highscore_DAR2_Primary, arg3);
              break;
            case 2:
              Script_SetHighScoreValue(Highscore_DAR3_Primary, arg3);
              break;
            case 3:
              Script_SetHighScoreValue(Highscore_DAR4_Primary, arg3);
              break;
            case 4:
              Script_SetHighScoreValue(Highscore_DAR5_Primary, arg3);
              break;
          }
          break;
        case 3:
          local_F0 = local_F4;
          switch (local_F0) {
            case 0:
              Script_SetHighScoreValue(Highscore_DART_Secondary, arg3);
              break;
            case 1:
              Script_SetHighScoreValue(Highscore_DAR2_Secondary, arg3);
              break;
            case 2:
              Script_SetHighScoreValue(Highscore_DAR3_Secondary, arg3);
              break;
            case 3:
              Script_SetHighScoreValue(Highscore_DAR4_Secondary, arg3);
              break;
            case 4:
              Script_SetHighScoreValue(Highscore_DAR5_Secondary, arg3);
              break;
          }
          break;
      }
      break;
  }
  return local_F8;
}

static void script_12E53(int32_t arg0, int32_t arg1, const Vector3 *arg2) {
  Script_MoveObjectToObject(arg0, arg1);
  Script_EVT_SetPosition(arg0, POS_XYZ, arg2);
  Script_MoveObjectToObject(arg0, -1);
  return;
}

static void script_12E73(int32_t arg0, int32_t arg1, Vector3 *arg2) {
  Script_MoveObjectToObject(arg0, arg1);
  Script_EVT_GetPosition(arg0, -1, arg2, 0);
  Script_MoveObjectToObject(arg0, -1);
  return;
}

static void script_12E92(int32_t arg0, int32_t arg1, const Vector3i *arg2) {
  Script_MoveObjectToObject(arg0, arg1);
  Script_EVT_SetRotation(arg0, 0x38000000, arg2, 0);
  Script_MoveObjectToObject(arg0, -1);
  return;
}

static void script_12EB4(int32_t arg0, Vector3 *arg1) {
  Script_charaDisplay(0x6E696F43, CDM_Hide);
  Script_MoveObjectToObject(0x6E696F43, arg0);
  Script_EVT_SetPosition(0x6E696F43, POS_XYZ, arg1);
  Script_MoveObjectToObject(0x6E696F43, -1);
  Script_EVT_GetPosition(0x6E696F43, -1, arg1, 0);
  return;
}

static void script_12EF7(int32_t arg0, Vector3 *arg1) {
  Vector3 local_F4;
  Vector3 local_E8;
  local_F4.x = 0.0f;
  (&local_F4)->y = 0.0f;
  (&local_F4)->z = 0.0f;
  Script_MoveObjectToObject(0x6E696F43, arg0);
  Script_EVT_SetPosition(0x6E696F43, POS_XYZ, arg1);
  Script_EVT_GetPosition(arg0, -1, &local_E8, 0);
  Script_EVT_SetPosition(arg0, POS_XYZ, &local_F4);
  Script_MoveObjectToObject(0x6E696F43, -1);
  Script_EVT_GetPosition(0x6E696F43, -1, arg1, 0);
  Script_EVT_SetPosition(arg0, POS_XYZ, &local_E8);
  return;
}

static void script_12F6D(int32_t arg0, float *arg1) {
  *arg1 = Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, arg0 * 22) * 0.3f + Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, arg0 * 31) * 0.37f + Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, arg0 * 103) * 0.15f + Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, arg0 * 147) * 0.18f;
  return;
}

static void script_12FC0(void) {
  Script_scenCreateTask(script_1303E, 3, 2, 100, 1);
  Script_scenCreateTask(script_1315A, 0);
  Script_executeAicaCommand(0x1200A0, 1, 0);
  Script_executeAicaCommand(0x1200A0, 3, 0);
  return;
}

static void script_12FF9(void) {
  if (task_1F4) {
    script_D71F();
  }
  script_1B370();
  Script_Ambient1SoundUtils(1, 24232);
  Script_executeAicaCommand(16296, 0, 0);
  Script_executeAicaCommand(1184, 2, 0);
  Script_scenCreateTask(script_1307B, 3, 2, 80, 1);
  return;
}

static int32_t script_1303E(int8_t arg0, int8_t arg1, int8_t arg2) {
  while (arg1) {
    arg1 = arg1 - arg2;
    if (arg1 < 0) {
      arg1 = 0;
    }
    Script_executeAicaCommand(1184, arg0, arg1);
    SCNF_WaitFrames_0000(1);
  }
  return Script_srchControl(0);
}

static int32_t script_1307B(int8_t arg0, int8_t arg1, int8_t arg2) {
  int32_t local_FC;
  local_FC = 0;
  while (arg1) {
    if (arg1 - arg2 > 0) {
      arg1 = arg1 - arg2;
      local_FC = local_FC + arg2;
    } else {
      local_FC = local_FC + arg1;
      arg1 = 0;
    }
    Script_executeAicaCommand(1184, arg0, local_FC);
    SCNF_WaitFrames_0000(1);
  }
  return Script_srchControl(0);
}

static void script_130D6(void) {
  Script_scenUtils(SCENARIO_CMD_INIT_SCNT);
  Script_FaceEx(CHARA_AKIR, 1, 0, 2);
  Script_charaSetRenderOpacity(CHARA_AKIR, 1.0f);
  Script_SetCPAMFlags(CHARA_AKIR, 0);
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
  return;
}

static void script_13112(void) {
  Script_CharHandlers(CHARA_AKIR, -1, 0, 0);
  Script_FaceEx(CHARA_AKIR, 0, 0, 2);
  Script_SetCPAMFlags(CHARA_AKIR, 1);
  Script_uiUtils(0, 0, 0);
  Script_scenUtils(SCENARIO_CMD_UPDATE_TIMER);
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 1);
  return;
}

static int32_t script_1315A(void) {
  SCNF_WaitFrames_0000(85);
  if (task_1F4) {
    Script_LoadSND(3, 0, 0, 0);
    Script_LoadSND(1, 2, "rim055.snd", 0x4000);
    while (Script_LoadSND(2, 0, 0, 0)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_LoadSND(4, 0, 0, 0);
  }
  return Script_srchControl(0);
}

static void script_131B8(int32_t arg0) {
  int32_t local_FC;
  if (arg0 != -1) {
    task_25C = arg0;
  }
  local_FC = task_25C;
  if (local_FC == 0) {
    Script_gachaponUtils(0, data_1EA88);
  } else if (local_FC == 1) {
    Script_gachaponUtils(0, data_1EB31);
  } else if (local_FC == 2) {
    Script_gachaponUtils(0, data_1EC6C);
  } else if (local_FC == 3) {
    Script_gachaponUtils(0, data_1EDA1);
  } else if (local_FC == 4) {
    Script_gachaponUtils(0, data_1EE48);
  } else if (local_FC == 5) {
    Script_gachaponUtils(0, data_1EFB5);
  } else if (local_FC == 6) {
    Script_gachaponUtils(0, data_1F1AE);
  } else if (local_FC == 7) {
    Script_gachaponUtils(0, data_1F281);
  } else if (local_FC == 8) {
    Script_gachaponUtils(0, data_1F2F2);
  } else if (local_FC == 9) {
    Script_gachaponUtils(0, data_1F33F);
  } else if (local_FC == 10) {
    Script_gachaponUtils(0, data_1F378);
  } else if (local_FC == 11) {
    Script_gachaponUtils(0, data_1F467);
  } else if (local_FC == 12) {
    Script_gachaponUtils(0, data_1F4CA);
  } else if (local_FC == 13) {
    Script_gachaponUtils(0, data_1F51F);
  } else if (local_FC == 14) {
    Script_gachaponUtils(0, data_1F566);
  } else if (local_FC == 15) {
    Script_gachaponUtils(0, data_1F59F);
  } else {
    task_25C = -1;
  }
  return;
}

static int16_t script_13359(int8_t arg0, int8_t arg1) {
  int16_t local_FE;
  int8_t local_FD;
  int32_t local_F8;
  local_FE = -1;
  if (arg0 == -1) {
    local_FD = task_25C;
  } else {
    local_FD = arg0;
  }
  local_F8 = local_FD;
  switch (local_F8) {
    case 0:
      local_FE = *(int16_t *)((int8_t *)&data_1E92C + (arg1 << 1));
      break;
    case 1:
      local_FE = *(int16_t *)((int8_t *)&data_1E944 + (arg1 << 1));
      break;
    case 2:
      local_FE = *(int16_t *)((int8_t *)&data_1E968 + (arg1 << 1));
      break;
    case 3:
      local_FE = *(int16_t *)((int8_t *)&data_1E994 + (arg1 << 1));
      break;
    case 4:
      local_FE = *(int16_t *)((int8_t *)&data_1E9A8 + (arg1 << 1));
      break;
    case 5:
      local_FE = *(int16_t *)((int8_t *)&data_1E9D0 + (arg1 << 1));
      break;
    case 6:
      local_FE = *(int16_t *)((int8_t *)&data_1E9F4 + (arg1 << 1));
      break;
    case 7:
      local_FE = *(int16_t *)((int8_t *)&data_1EA0C + (arg1 << 1));
      break;
    case 8:
      local_FE = *(int16_t *)((int8_t *)&data_1EA1C + (arg1 << 1));
      break;
    case 9:
      local_FE = *(int16_t *)((int8_t *)&data_1EA24 + (arg1 << 1));
      break;
    case 10:
      local_FE = *(int16_t *)((int8_t *)&data_1EA2C + (arg1 << 1));
      break;
    case 11:
      local_FE = *(int16_t *)((int8_t *)&data_1EA50 + (arg1 << 1));
      break;
    case 12:
      local_FE = *(int16_t *)((int8_t *)&data_1EA60 + (arg1 << 1));
      break;
    case 13:
      local_FE = *(int16_t *)((int8_t *)&data_1EA6C + (arg1 << 1));
      break;
    case 14:
      local_FE = *(int16_t *)((int8_t *)&data_1EA78 + (arg1 << 1));
      break;
    case 15:
      local_FE = *(int16_t *)((int8_t *)&data_1EA80 + (arg1 << 1));
      break;
  }
  return local_FE;
}

static void script_13544(int32_t arg0, int32_t arg1) {
  for (;;) {
    *(int32_t *)arg0 = Script_FloatUtils(ScriptMathOp_RandomIntBelow, 16);
    if (data_1E90C[*(int32_t *)arg0] - data_1E91C[*(int32_t *)arg0]) {
      break;
    }
  }
  *(int32_t *)arg1 = Script_FloatUtils(ScriptMathOp_RandomIntBelow, data_1E90C[*(int32_t *)arg0] - data_1E91C[*(int32_t *)arg0]);
  return;
}

static int32_t script_13593(int32_t arg0, int32_t arg1) {
  if (arg0 == 0) {
    (void)0;
  } else if (arg0 == 14) {
    (void)0;
  } else if (arg1 < data_1E90C[arg0] - data_1E91C[arg0]) {
    (void)1;
  } else if (arg0 == 15) {
    (void)3;
  } else {
    (void)2;
  }
  end:;
}

static void script_135E6(int32_t arg0, int32_t arg1, int32_t arg2, float arg3) {
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
        goto L013657;
      }
      L013657:;
      break;
    case 3:
      local_EC = arg2;
      if (local_EC == 6) {
        (void)((local_F3 = -1) & 1);
        goto L01367E;
      }
      L01367E:;
      break;
    case 4:
      local_EC = arg2;
      if ((local_EC == 7) | (local_EC == 15) | (local_EC == 16) | (local_EC == 17) | (local_EC == 18)) {
        (void)((local_F3 = -1) & 1);
        goto L0136BD;
      }
      L0136BD:;
      break;
    case 5:
      (void)((local_F3 = -1) & 1);
      break;
    case 6:
      local_EC = arg2;
      if ((local_EC == 0) | (local_EC == 1) | (local_EC == 2) | (local_EC == 3)) {
        (void)((local_F3 = -1) & 1);
        goto L01370B;
      }
      L01370B:;
      break;
    case 8:
      local_EC = arg2;
      if (local_EC == 3) {
        (void)((local_F3 = -1) & 1);
        goto L013732;
      }
      L013732:;
      break;
  }
  local_F4.x = (&local_F4)->y = (&local_F4)->z = 1.0f;
  if (local_F3 & 8) {
    arg2 = arg2 + data_1E90C[arg1];
    local_F4.x = (&local_F4)->x * arg3;
    (&local_F4)->y = (&local_F4)->y * arg3;
    (&local_F4)->z = (&local_F4)->z * arg3;
  } else {
    local_F4.x = (&local_F4)->x * arg3 * 0.33333f;
    (&local_F4)->y = (&local_F4)->y * arg3 * 0.33333f;
    (&local_F4)->z = (&local_F4)->z * arg3 * 0.33333f;
  }
  if (local_F2 & 4) {
    script_131B8(arg1);
    Script_gachaponUtils(5, arg0, arg2);
  } else {
    script_131B8(arg1);
    Script_gachaponUtils(1, arg0, arg2);
  }
  if ((((scn3_bits){.f = (&local_F4)->x}).i != 0x3F800000) & (((scn3_bits){.f = (&local_F4)->x}).i != 0)) {
    Script_hmdlApplyScale(arg0, &local_F4);
  }
  return;
}

__attribute__((scn_string_bindings(0, data_1F698)))
static void script_1381D(void) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  Vector3 local_E8;
  Vector3i local_DC;
  int32_t local_D8;
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, "/scene/02/MKYU/", "GACH") == 0) {
    SCNF_WaitFrames_0000(1);
  }
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2)) {
    SCNF_WaitFrames_0000(1);
  }
  local_F8 = 49;
  local_FC = 0;
  while (local_FC < 6) {
    local_F4 = 1 << local_FC;
    if (local_F8 & local_F4) {
      local_D8 = *(&data_1F5D8 + local_FC);
      local_E8.x = *(float *)((const char *)&data_1F620 + local_FC * 20);
      (&local_E8)->y = *(float *)((const char *)&data_1F620 + (local_FC * 20 + 4));
      (&local_E8)->z = *(float *)((const char *)&data_1F620 + (local_FC * 20 + 8));
      local_DC.x = 0;
      (&local_DC)->y = *(const uint32_t *)((const char *)&data_1F620 + (local_FC * 20 + 12));
      (&local_DC)->z = 0;
      Script_EVT_SetPosition(local_D8, POS_XYZ, &local_E8);
      Script_EVT_SetRotation(local_D8, 0x38000000, &local_DC, 0);
      Script_charaDisplay(local_D8, CDM_Show);
      Script_SetOrGetWOCCValue(local_D8, *((int8_t *)&data_1F620 + (local_FC * 20 + 16)));
      Script_MoveObjectToObject(*(&data_1F608 + local_FC), *(&data_1F5D8 + local_FC));
      Script_EVT_SetPosition(*(&data_1F608 + local_FC), POS_XYZ, &data_1F6B0);
      Script_EVT_SetRotation(*(&data_1F608 + local_FC), 0x38000000, &data_1F6BC, 0);
      Script_charaDisplay(*(&data_1F608 + local_FC), CDM_Show);
      goto L013994;
    }
    L013994:;
    local_FC = local_FC + 1;
  }
  task_260 = 0;
  (void)((task_268 = 0) & 2);
  (void)((task_269 = 0) & 2);
  (void)((task_26A = 0) & 2);
  return;
}

static void script_139C6(int32_t arg0) {
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
  if (task_268 & 8) {
    (void)((local_DB = 0) & 0x80);
  }
  (void)(local_DB & 8);
  return;
}

static int32_t script_13A30(int32_t arg0) {
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
    local_D4 = 49;
    if (local_D4 & (1 << local_E0)) {
      int32_t local_D0;
      local_D0 = *(&data_1F5D8 + local_E0);
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
  script_168CE(arg0, &local_F4);
  script_168CE(arg0, &local_E8);
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

static int32_t script_13C61(void) {
  int16_t local_FE;
  int32_t local_F8;
  int32_t local_F4;
  int32_t local_F0;
  local_F8 = 0;
  local_FE = 0;
  while (local_FE < 6) {
    local_F4 = 49;
    if (local_F4 & (1 << local_FE)) {
      local_F0 = *(&data_1F5D8 + local_FE);
      if (script_13A30(local_F0)) {
        local_F8 = local_F0;
        task_260 = local_F0;
      }
    }
    local_FE = local_FE + 1;
  }
  task_260 = local_F8;
  return local_F8;
}

static void script_13CCA(void) {
  int32_t local_FC;
  int8_t local_FB;
  local_FC = 0;
  (void)((local_FB = 0) & 2);
  if (Script_IsPlayerAllowed(0)) {
    if (script_13C61()) {
      (void)((local_FB = -1) & 2);
      if (Script_GetControllerInput(1) == 0x400) {
        local_FC = 1;
      }
    }
  }
  if (local_FB & 8) {
    if (~task_269) {
      (void)((task_269 = -1) & 2);
      Script_sicnUtils(0, 5, 1);
      Script_uiUtils(0, 6, 0);
    }
  } else {
    if (task_269 & 0x10) {
      (void)((task_269 = 0) & 2);
      Script_sicnUtils(1, 0, 0);
      Script_uiUtils(0, 0, 0);
    }
  }
  (void)local_FC;
  return;
}

__attribute__((scn_string_bindings(0, data_1F6C8, 1, data_1F6CE, 2, data_1F6DE)))
static int32_t script_13D6D(void) {
  Script_LoadVoiceFile(1, "E1066");
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, "/scene/02/MKYU/", "HI_GACH") == 0) {
    SCNF_WaitFrames_0000(1);
  }
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2)) {
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

__attribute__((scn_string_bindings(0, data_1F6E6, 1, data_1F6EC)))
static void script_13DB9(void) {
  Script_LoadVoiceFile(0, "E1066");
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 6, "HI_GACH");
  return;
}

__attribute__((scn_string_bindings(0, data_1F6F4)))
static void script_13DD6(int32_t arg0) {
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
  local_F8 = 49;
  local_FC = 0;
  while (local_FC < 6) {
    local_F4 = 1 << local_FC;
    if (local_F8 & local_F4) {
      local_F0 = *(&data_1F5D8 + local_FC);
      local_EC = *(&data_1F5F0 + local_FC);
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

static void script_13EA8(void) {
  Vector3i local_F4;
  (void)((task_268 = -1) & 8);
  Script_scenCreateTask(script_13D6D, 0);
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
  (void)((task_269 = 0) & 8);
  Script_ccowUtils(CHARA_AKIR, 2);
  SCNF_WaitFrames_0000(1);
  Script_uiUtils(0, 6, 0);
  Script_SetCharaRefBoardPos(CHARA_AKIR, &data_1F6FC);
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

static void script_1404F(void) {
  Script_charaSetRefboardFlagTo(CHARA_AKIR, 2);
  Script_motmSetBlendFrames(CHARA_AKIR, MOTM_BLEND_FRAMES_5);
  script_13DB9();
  Script_SetGameStateTo(AUTH_None);
  Script_ccowUtils(CHARA_AKIR, 1);
  Script_ryoControl(RYO_RestoreControl);
  Script_scenUtils(SCENARIO_CMD_UPDATE_TIMER);
  Script_uiUtils(0, 0, 0);
  (void)((task_268 = 0) & 8);
  Script_scenToggleVisibleFor(SHEN_NAME('DAMY'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('GCPG'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('GCPB'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('GCPR'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('GCPY'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('GABS'), 0);
  Script_FaceEx(CHARA_AKIR, 0, 0, 2);
  Script_SetCPAMFlags(CHARA_AKIR, 1);
  Script_shenClearFreeState(1);
  (void)((task_268 = 0) & 8);
  Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_EV_STOP_ACTIVE, 0);
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 1);
  return;
}

static void script_14129(void) {
  int32_t local_F4[3];
  int32_t local_F0;
  int32_t local_EC;
  int32_t local_E8;
  Script_ResetHandMotion(CHARA_AKIR, 1, 1, 0);
  Script_ResetHandMotion(CHARA_AKIR, 1, 0, 0);
  script_149A3(CHARA_AKIR, 1, 8, 1);
  script_149A3(CHARA_AKIR, 0, 8, 1);
  local_F4[0] = local_F4[1] = local_F4[2] = 0;
  local_E8 = 0;
  while (local_E8 < 6) {
    if (task_260 == *(&data_1F5D8 + local_E8)) {
      local_EC = *((int8_t *)&data_1F620 + (local_E8 * 20 + 17));
      break;
    }
    local_E8 = local_E8 + 1;
  }
  local_F0 = Script_PlayerStatsUtils(PS_GetIndexedU16Table3Slot, local_EC);
  Script_PlayerStatsUtils(PS_SetIndexedU16Table3Slot, local_EC, local_F0 + 1);
  Script_InitializeOsag(CHARA_AKIR);
  return;
}

static void script_141ED(void) {
  Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
  Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
  Script_gachaponUtils(2, 0x4849454B);
  return;
}

static void script_1421C(int32_t arg0) {
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
  script_168CE(arg0, &local_F4);
  Script_mtwkGetSetRotation(arg0, &local_D0, 0);
  Script_scenCharaSubmitMoveTargetRequest(CHARA_AKIR, &local_F4, (&local_D0)->y, MN_AKI_AKI_STAND_LP, 0);
  while (Script_IsMoveTargetRequestActive(CHARA_AKIR)) {
    SCNF_WaitFrames_0000(1);
  }
  SCNF_WaitFrames_0000(1);
  local_F4.x = -0.0511f;
  (&local_F4)->y = 0.0f;
  (&local_F4)->z = 0.650771f;
  script_168CE(task_260, &local_F4);
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

static void script_143A5(int32_t arg0, int32_t arg1) {
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
  script_168CE(task_260, &local_F4);
  Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &local_F4);
  Script_mtwkGetSetRotation(task_260, &local_E8, 0);
  Script_EVT_SetRotation(CHARA_AKIR, 0x38000000, &local_E8, 0);
  return;
}

static int32_t script_14426(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4) {
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

static int32_t script_144E0(void) {
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
      Script_scenCreateTask(script_1461B, 1, local_FC - 100.0f);
    }
    if (-!(local_FC < 170.0f) & ~local_FA) {
      (void)((local_FA = -1) & 2);
      Script_scenCreateTask(script_146EB, 0);
    }
    if (-!(local_FC < 215.0f) & ~local_F9) {
      (void)((local_F9 = -1) & 2);
      Script_scenCreateTask(script_147B4, 0);
    }
    if (-!(local_FC < 280.0f) & ~local_F8) {
      (void)((local_F8 = -1) & 2);
      Script_scenCreateTask(script_1485E, 0);
    }
    if (-!(local_FC < 300.0f) & ~local_F7) {
      (void)((local_F7 = -1) & 2);
      Script_scenCreateTask(script_148A8, 0);
    }
    if (-!(local_FC < 310.0f) & ~local_F6) {
      (void)((local_F6 = -1) & 2);
      Script_scenCreateTask(script_14B94, 0);
    }
    if (local_F6 & 0x40) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  return Script_srchControl(0);
}

static int32_t script_1461B(float arg0) {
  int32_t local_F4[3];
  int32_t local_F0;
  if (!(40.0f - arg0 <= 0.0f)) {
    local_F0 = Script_FloatUtils(ScriptMathOp_FloatToInt, 40.0f - arg0);
    script_149A3(CHARA_AKIR, 1, 0, local_F0);
    local_F4[0] = 0;
    local_F4[1] = -5643;
    local_F4[2] = 728;
    script_14426(1, *(int32_t *)local_F4, local_F4[1], local_F4[2], local_F0);
  }
  script_149A3(CHARA_AKIR, 1, 1, 16);
  local_F4[1] = -4915;
  script_14426(1, *(int32_t *)local_F4, local_F4[1], local_F4[2], 8);
  local_F4[1] = -5643;
  script_14426(1, *(int32_t *)local_F4, local_F4[1], local_F4[2], 8);
  Script_scenCreateTask(script_157EB, 0);
  end:;
}

static int32_t script_146EB(void) {
  int32_t local_F4[3];
  script_149A3(CHARA_AKIR, 1, 2, 40);
  local_F4[0] = -7827;
  local_F4[1] = 4187;
  local_F4[2] = 4187;
  script_14426(1, *(int32_t *)local_F4, local_F4[1], local_F4[2], 8);
  local_F4[0] = -7827;
  local_F4[1] = 3458;
  local_F4[2] = 4187;
  script_14426(1, *(int32_t *)local_F4, local_F4[1], local_F4[2], 8);
  Script_scenCreateTask(script_150A0, 1, 20);
  local_F4[0] = 6735;
  local_F4[1] = 2821;
  local_F4[2] = 1456;
  script_14426(1, *(int32_t *)local_F4, local_F4[1], local_F4[2], 16);
  SCNF_WaitFrames_0000(4);
  end:;
}

static int32_t script_147B4(void) {
  int32_t local_F4[3];
  script_149A3(CHARA_AKIR, 1, 3, 40);
  local_F4[0] = -7827;
  local_F4[1] = 3458;
  local_F4[2] = 4187;
  script_14426(1, *(int32_t *)local_F4, local_F4[1], local_F4[2], 24);
  Script_scenCreateTask(script_150A0, 1, 14);
  local_F4[0] = 6735;
  local_F4[1] = 2821;
  local_F4[2] = 1456;
  script_14426(1, *(int32_t *)local_F4, local_F4[1], local_F4[2], 16);
  Script_scenCreateTask(script_158AD, 0);
  SCNF_WaitFrames_0000(4);
  Script_scenCreateTask(script_15BCC, 0);
  end:;
}

static int32_t script_1485E(void) {
  int32_t local_F4[3];
  script_149A3(CHARA_AKIR, 1, 4, 20);
  local_F4[0] = 2548;
  local_F4[1] = -3822;
  local_F4[2] = -2184;
  script_14426(1, *(int32_t *)local_F4, local_F4[1], local_F4[2], 20);
  end:;
}

static int32_t script_148A8(void) {
  int32_t local_F4[3];
  script_149A3(CHARA_AKIR, 1, 5, 20);
  local_F4[0] = -364;
  local_F4[1] = -2730;
  local_F4[2] = -182;
  script_14426(1, *(int32_t *)local_F4, local_F4[1], local_F4[2], 20);
  local_F4[0] = -546;
  local_F4[1] = -1274;
  local_F4[2] = 2730;
  Script_scenCreateTask(script_14426, 5, 1, *(int32_t *)local_F4, local_F4[1], local_F4[2], 40);
  local_F4[0] = -7645;
  local_F4[1] = 3822;
  local_F4[2] = 910;
  local_F4[0] = -2419;
  local_F4[1] = 4176;
  local_F4[2] = 5642;
  Script_scenCreateTask(script_14426, 5, 0, *(int32_t *)local_F4, local_F4[1], local_F4[2], 30);
  script_149A3(CHARA_AKIR, 1, 6, 10);
  script_149A3(CHARA_AKIR, 0, 6, 10);
  end:;
}

static void script_149A3(int32_t arg0, int32_t arg1, int32_t arg2, int32_t arg3) {
  int32_t unused_FF20[56];
  int32_t local_FF1C;
  int32_t local_FF18;
  int32_t local_FF14;
  local_FF18 = -1;
  local_FF14 = arg2;
  switch (local_FF14) {
    case 0:
      local_FF18 = (int32_t)data_1F70C;
      break;
    case 1:
      local_FF18 = (int32_t)data_1F7F0;
      break;
    case 2:
      local_FF18 = (int32_t)data_1F8D4;
      break;
    case 3:
      local_FF18 = (int32_t)data_1F9B8;
      break;
    case 4:
      local_FF18 = (int32_t)data_1FA9C;
      break;
    case 5:
      local_FF18 = (int32_t)data_1FB80;
      break;
    case 6:
      local_FF18 = (int32_t)data_1FC64;
      break;
    case 7:
      local_FF18 = (int32_t)data_1FD48;
      break;
    case 8:
      local_FF18 = (int32_t)data_1FE2C;
      break;
    case 9:
      {
        int32_t local_FF10;
        local_FF10 = 0;
        while (local_FF10 < 57) {
          *(&local_FF1C + local_FF10) = *(int32_t *)(data_1FB80 + (local_FF10 << 2));
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
          *(&local_FF1C + local_FF10) = *(int32_t *)(data_1FB80 + (local_FF10 << 2));
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

static int32_t script_14B94(void) {
  Script_seqEnterEvtCam(7008, task_260, task_260);
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
    Script_fixoAttachEx(task_264, CHARA_AKIR, 18, &local_F4, &local_E8);
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
    Script_fixoAttachEx(SHEN_NAME('KEIH'), task_264, 18, &local_F4, &local_E8);
  }
  Script_charaDisplay(SHEN_NAME('KEIH'), CDM_Show);
  SCNF_WaitFrames_0000(1);
  for (;;) {
    if (Script_GetCharaMotionRemainingFramesScaled(CHARA_AKIR) >= 360.0f) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_executeAicaCommand(0x1A04A9, 0, 0);
  {
    Vector3 local_F4;
    Vector3i local_E8;
    local_F4.x = 0.09f;
    (&local_F4)->y = 0.075f;
    (&local_F4)->z = 0.008f;
    local_E8.x = 13111;
    (&local_E8)->y = 817;
    (&local_E8)->z = 50684;
    Script_fixoAttachEx(task_264, CHARA_AKIR, 12, &local_F4, &local_E8);
  }
  SCNF_WaitFrames_0000(1);
  for (;;) {
    if (Script_GetCharaMotionRemainingFramesScaled(CHARA_AKIR) >= 410.0f) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_fixoClearAttachment(SHEN_NAME('GABS'));
  Script_scenCreateTask(script_15022, 1, SHEN_NAME('GABS'));
  SCNF_WaitFrames_0000(1);
  for (;;) {
    if (Script_GetCharaMotionRemainingFramesScaled(CHARA_AKIR) >= 430.0f) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  script_149A3(CHARA_AKIR, 1, 5, 20);
  {
    int32_t unused_F4[3];
    int32_t local_E8[3];
    local_E8[0] = 909;
    local_E8[1] = -1657;
    local_E8[2] = 1431;
    Script_scenCreateTask(script_14426, 5, 1, *(int32_t *)local_E8, local_E8[1], local_E8[2], 20);
  }
  {
    int32_t unused_F4[3];
    int32_t local_E8[3];
    local_E8[0] = -38;
    local_E8[1] = 476;
    local_E8[2] = 9888;
    Script_scenCreateTask(script_14426, 5, 0, *(int32_t *)local_E8, local_E8[1], local_E8[2], 20);
  }
  Script_scenCreateTask(script_14F30, 0);
  SCNF_WaitFrames_0000(1);
  for (;;) {
    if (Script_GetCharaMotionRemainingFramesScaled(CHARA_AKIR) >= 450.0f) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_seqEnterEvtCam(7009, task_260, task_260);
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
  if (task_26B & 2) {
    int32_t unused_E8[6];
    Vector4 local_D8;
    local_D8.x = (&local_D8)->y = (&local_D8)->z = (&local_D8)->w = 0.5f;
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
  Script_fixoClearAttachment(task_264);
  Script_scenCreateTask(script_15022, 1, task_264);
  SCNF_WaitFrames_0000(1);
  while (Script_IsCharaMotionFinished(CHARA_AKIR) == 0) {
    SCNF_WaitFrames_0000(1);
  }
  Script_fixoClearAttachment(SHEN_NAME('KEIH'));
  end:;
}

static int32_t script_14F30(void) {
  Vector3 local_F4;
  int32_t local_F0;
  local_F4.x = *(float *)&data_1FF1C;
  (&local_F4)->y = *(float *)(&data_1FF1C + 1);
  (&local_F4)->z = *(float *)(&data_1FF1C + 2);
  local_F0 = 20;
  while (local_F0) {
    local_F4.x = (&local_F4)->x + ((&data_1FF10)->x - *(float *)&data_1FF1C) / 20.0f;
    (&local_F4)->y = (&local_F4)->y + ((&data_1FF10)->y - *(float *)(&data_1FF1C + 1)) / 20.0f;
    (&local_F4)->z = (&local_F4)->z + ((&data_1FF10)->z - *(float *)(&data_1FF1C + 2)) / 20.0f;
    Script_fixoClearAttachment(task_264);
    Script_fixoAttachEx(task_264, CHARA_AKIR, 12, &local_F4, &data_1FF28);
    local_F0 = local_F0 - 1;
    SCNF_WaitFrames_0000(1);
  }
  Script_fixoAttachEx(task_264, CHARA_AKIR, 12, &data_1FF10, &data_1FF28);
  return Script_srchControl(0);
}

static int32_t script_15022(int32_t arg0) {
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
  Script_executeAicaCommand(1449, 0, 0);
  Script_charaDisplay(arg0, CDM_Hide);
  Script_scenToggleVisibleFor(arg0, 0);
  return Script_srchControl(0);
}

static int32_t script_150A0(int32_t arg0) {
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
      if (task_260 == *(&data_1F5D8 + local_FC)) {
        local_F8 = *(&data_1F5F0 + local_FC);
        break;
      }
      local_FC = local_FC + 1;
    }
    Script_charaPosRotUtils(local_F8, 153, &local_E0, 0, 2);
    local_DC = (&local_E0)->y;
    local_FC = 0;
    L015120:;
    if (local_FC == arg0 * 6 / 10) {
      Script_executeAicaCommand(0x1904A9, 0, 0);
    }
    Script_charaPosRotUtils(local_F8, 152, 0, &local_EC, 1);
    (&local_E0)->y = (&local_E0)->y + (Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 0.003f) - 0.0015f);
    Script_charaPosRotUtils(local_F8, 153, &local_E0, 0, 0);
    local_FC = local_FC + 1;
    SCNF_WaitFrames_0000(1);
    if (!(local_FC >= arg0)) goto L015120;
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

static void script_151D2(int32_t arg0) {
  int32_t unused_F4[3];
  Vector3i local_E8;
  int32_t local_E4;
  int32_t local_E0;
  int32_t local_DC;
  int32_t unused_D0[3];
  Vector3i local_C4;
  Script_MoveObjectToObject(SHEN_NAME('KEIH'), task_260);
  (&local_E8)->y = (&local_E8)->z = 0;
  local_E8.x = 0;
  Script_EVT_SetRotation(SHEN_NAME('KEIH'), 0x38000000, &local_E8, 0);
  Script_MoveObjectToObject(SHEN_NAME('KEIH'), -1);
  Script_MoveObjectToObject(SHEN_NAME('YKUR'), 0x4849454B);
  Script_EVT_SetPosition(SHEN_NAME('YKUR'), POS_XYZ, &data_1FF34);
  Script_EVT_SetRotation(SHEN_NAME('YKUR'), 0x38000000, &data_1FF40, 0);
  Script_MoveObjectToObject(SHEN_NAME('YKUR'), -1);
  Script_charaDisplay(CHARA_AKIR, CDM_Hide);
  Script_charaDisplay(SHEN_NAME('YKUR'), CDM_Show);
  Script_gachaponUtils(3, 0x4849454B, &data_1FF64);
  SCNF_WaitFrames_0000(1);
  local_E0 = Script_scenCreateTask(script_1543E, 0);
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
        goto L01531F;
      }
    }
    L01531F:;
    Script_SetHandEulerRotationMasked(SHEN_NAME('YKUR'), 1, 0x15, &local_E8);
    Script_fixoClearAttachment(SHEN_NAME('KEIH'));
    Script_fixoSetAttachmentOffset(SHEN_NAME('KEIH'), 0, &data_1FF34, &data_1FF40);
    local_C4.x = 0;
    (&local_C4)->y = -(&local_E8)->x;
    (&local_C4)->z = 0;
    Script_fixoAttachEx(SHEN_NAME('KEIH'), SHEN_NAME('YKUR'), 18, &data_1FF4C, &local_C4);
    SCNF_WaitFrames_0000(1);
    if (local_DC & 0x400) {
      if (~task_26A) {
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
  script_16206(arg0);
  Script_SetHandEulerRotationMasked(SHEN_NAME('YKUR'), 1, 0x15, &data_1FF58);
  Script_charaDisplay(SHEN_NAME('KEIH'), CDM_Hide);
  Script_MoveObjectToObject(SHEN_NAME('YKUR'), -1);
  Script_scenToggleVisibleFor(SHEN_NAME('YKUR'), 0);
  Script_charaDisplay(SHEN_NAME('YKUR'), CDM_Hide);
  Script_ResetHandMotion(SHEN_NAME('YKUR'), 0, 1, 0);
  Script_charaDisplay(CHARA_AKIR, CDM_Show);
  return;
}

static int32_t script_1543E(void) {
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

static void script_15516(void) {
  return;
}

static int32_t script_15519(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  while (task_26A & 1) {
    SCNF_WaitFrames_0000(1);
  }
  (void)((task_26A = -1) & 0x10);
  local_F8 = arg0;
  switch (local_F8) {
    case 0:
      local_FC = script_1687C(3, "E1066A001", "E1066A002", "E1066A003");
      break;
    case 1:
      local_FC = script_1687C(2, "E1066A004", "E1066A005");
      break;
    case 2:
      local_FC = script_1687C(3, "E1066A006", "E1066A007", "E1066A008");
      break;
    case 3:
      local_FC = script_1687C(3, "E1066A009", "E1066A010", "E1066A011");
      break;
    case 4:
      local_FC = script_1687C(2, "E1066A026", "E1066A027");
      break;
  }
  if (local_FC) {
    while (Script_convGetIsActive((const void *)local_FC)) {
      SCNF_WaitFrames_0000(1);
    }
  }
  (void)((task_26A = 0) & 4);
  return Script_srchControl(0);
}

static int32_t script_1562F(int16_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  int32_t local_F0;
  int32_t local_EC;
  int32_t local_E8;
  while (task_26A & 1) {
    SCNF_WaitFrames_0000(1);
  }
  (void)((task_26A = -1) & 4);
  local_F8 = task_25C;
  local_F4 = script_13359(local_F8, arg0);
  local_EC = script_13593(local_F8, arg0);
  local_F0 = Script_GetOwnedItemCount(local_F4, 0);
  local_FC = 0;
  if (local_F0) {
    local_FC = script_1687C(3, "E1066A019", "E1066A020", "E1066A021");
  } else {
    local_E8 = local_EC;
    if (local_E8 == 0) {
      local_FC = script_1687C(5, "E1066A014", "E1066A015", "E1066A016", "E1066A017", "E1066A018");
    } else if (local_E8 == 1) {
      local_FC = script_1687C(3, "E1066A022", "E1066A025", "E1066A024");
    } else {
      if ((local_E8 == 2) | (local_E8 == 3)) {
        local_FC = Script_ShowUIMessageFromID("E1066A023");
        goto L015732;
      }
    }
  }
  L015732:;
  if (local_FC) {
    while (Script_convGetIsActive((const void *)local_FC)) {
      SCNF_WaitFrames_0000(1);
    }
  }
  return (task_26A = 0) & 1;
}

static int32_t script_15758(void) {
  int32_t local_FC;
  local_FC = Script_shenGetStats(STATS_CASH);
  SCNF_SetCashValue(-100, 1);
  Script_SetStats(STATS_CASH, local_FC - 100);
  return Script_srchControl(0);
}

static void script_15781(void) {
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

static int32_t script_157EB(void) {
  Vector3 local_F4;
  int32_t unused_E8[3];
  float local_E4;
  float local_E0;
  Script_fixoClearAttachment(0x6E496F63);
  Script_MoveObjectToObject(0x6E496F63, task_260);
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
  Script_executeAicaCommand(0x1804A9, 0, 0);
  end:;
}

static int32_t script_158AD(void) {
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
  Script_executeAicaCommand(0x1B04A9, 0, 0);
  local_D8 = Script_FloatUtils(ScriptMathOp_RandomIntBelow, 4);
  local_B8 = local_D8;
  if (local_B8 == 0) {
    task_264 = 0x47504347;
  } else if (local_B8 == 1) {
    task_264 = 0x42504347;
  } else if (local_B8 == 2) {
    task_264 = 0x52504347;
  } else {
    task_264 = 0x59504347;
  }
  Script_EVT_GetPosition(task_260, -1, &local_F4, 0);
  Script_mtwkGetSetRotation(task_260, &local_E8, 0);
  local_F4.x = (&local_F4)->x + (Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_E8)->y) * 0.2f + ((float)0 - Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_E8)->y)) * -0.1f);
  (&local_F4)->y = (&local_F4)->y + 0.5f;
  (&local_F4)->z = (&local_F4)->z + (Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_E8)->y) * 0.2f + Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_E8)->y) * -0.1f);
  Script_EVT_SetPosition(task_264, POS_XYZ, &local_F4);
  Script_EVT_SetPosition(SHEN_NAME('GABS'), POS_XYZ, &local_F4);
  Script_charaDisplay(task_264, CDM_Show);
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
    Script_EVT_SetPosition(task_264, POS_XYZ, &local_F4);
    Script_EVT_SetPosition(SHEN_NAME('GABS'), POS_XYZ, &local_F4);
    Script_EVT_SetRotation(task_264, 0x38000000, &local_E8, 0);
    Script_EVT_SetRotation(SHEN_NAME('GABS'), 0x38000000, &local_E8, 0);
    SCNF_WaitFrames_0000(1);
  }
  local_E8.x = 910;
  (&local_E8)->y = 1274;
  (&local_E8)->z = 0x8000;
  Script_mtwkGetSetRotation(task_264, &local_DC, 0);
  *(float *)&local_DC.x = (float)((&local_E8)->x - (&local_DC)->x) / 10.0f;
  *(float *)&(&local_DC)->y = (float)((&local_E8)->y - (&local_DC)->y) / 10.0f;
  *(float *)&(&local_DC)->z = (float)((&local_E8)->z - (&local_DC)->z) / 10.0f;
  local_BC = 10;
  while (local_BC) {
    local_E8.x = (&local_E8)->x + (&local_DC)->x;
    (&local_E8)->y = (&local_E8)->y + (&local_DC)->y;
    (&local_E8)->z = (&local_E8)->z + (&local_DC)->z;
    script_16906(task_260, &local_E8);
    Script_EVT_SetRotation(task_264, 0x38000000, &local_E8, 0);
    Script_EVT_SetRotation(SHEN_NAME('GABS'), 0x38000000, &local_E8, 0);
    local_BC = local_BC - 1;
    SCNF_WaitFrames_0000(1);
  }
  Script_executeAicaCommand(0x1C04A9, 0, 0);
  end:;
}

static int32_t script_15BCC(void) {
  Vector3 local_F4;
  Script_MoveObjectToObject(CHARA_AKIR, task_260);
  Script_EVT_GetPosition(CHARA_AKIR, -1, &local_F4, 0);
  Script_MoveObjectToObject(CHARA_AKIR, -1);
  {
    int32_t local_F0;
    local_F0 = 15;
    while (local_F0) {
      (&local_F4)->z = (&local_F4)->z + 0.002f;
      Script_MoveObjectToObject(CHARA_AKIR, task_260);
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
      Script_MoveObjectToObject(CHARA_AKIR, task_260);
      Script_EVT_SetPosition(CHARA_AKIR, POS_XYZ, &local_F4);
      Script_MoveObjectToObject(CHARA_AKIR, -1);
      local_F0 = local_F0 - 1;
      SCNF_WaitFrames_0000(1);
    }
  }
  end:;
}

static void script_15CA9(void) {
  Script_SetMotnFlags(CHARA_AKIR, 17, &data_2006C, 0x39000000);
  return;
}

static int32_t script_15CC1(void) {
  for (;;) {
    script_15CA9();
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_15CD4(void) {
  Script_scenToggleVisibleFor(SHEN_NAME('YKUR'), 1);
  Script_ResetHandMotion(SHEN_NAME('YKUR'), 1, 1, 0);
  Script_charaDisplay(SHEN_NAME('YKUR'), CDM_Hide);
  Script_SetHandMotionTargetChara(SHEN_NAME('YKUR'), 1, 0);
  script_149A3(SHEN_NAME('YKUR'), 1, 9, 1);
  SCNF_WaitFrames_0000(1);
  Script_SetHandMotionTargetChara(SHEN_NAME('YKUR'), 1, SHEN_NAME('KEIH'));
  script_149A3(SHEN_NAME('YKUR'), 1, 10, 1);
  L015D48:;
  Script_MoveObjectToObject(SHEN_NAME('YKUR'), 0x4849454B);
  Script_EVT_SetPosition(SHEN_NAME('YKUR'), POS_XYZ, &data_20078);
  Script_EVT_SetRotation(SHEN_NAME('YKUR'), 0x38000000, &data_20084, 0);
  Script_MoveObjectToObject(SHEN_NAME('YKUR'), -1);
  SCNF_WaitFrames_0000(1);
  if (!Script_GetHandControlBit3(SHEN_NAME('YKUR'), 1)) goto L015D48;
  Script_SetHandMotionTargetChara(SHEN_NAME('YKUR'), 1, 0);
  end:;
}

static void script_15DAF(void) {
  int32_t unused_F4[3];
  int32_t local_F0;
  int32_t local_EC;
  local_EC = 0;
  local_F0 = task_260;
  script_13EA8();
  Script_sicnUtils(0, 1, 1);
  for (;;) {
    if (script_15E8A(local_F0, local_EC)) {
      Script_shenClearFreeState(0);
      Script_sicnUtils(2, 1, 0);
      script_160D6(local_F0, local_EC);
    } else {
      Script_shenClearFreeState(0);
      Script_sicnUtils(2, 1, 0);
      if (local_EC) {
        script_13DD6(0);
        Script_charaSetRefboardFlagTo(CHARA_AKIR, 2);
        Script_motmSetBlendFrames(CHARA_AKIR, MOTM_BLEND_FRAMES_30);
        Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        Script_SetGameStateTo(AUTH_None);
        SCNF_WaitFrames_0000(30);
      }
      L015E65:;
      SCNF_WaitFrames_0000(1);
      if (!~task_26A) goto L015E65;
      break;
    }
    local_EC = local_EC + 1;
  }
  script_1404F();
  return;
}

static int32_t script_15E8A(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  int32_t local_F8;
  Vector3 local_EC;
  int32_t local_E8;
  int32_t local_E4;
  local_F8 = 0;
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_EV_STOP_ACTIVE) == 1) {
    (void)0;
  } else {
    script_143A5(arg0, arg1);
    Script_EVT_GetPosition(arg0, -1, &local_EC, 0);
    (&local_EC)->y = (&local_EC)->y + 0.6f;
    Script_SetCharaLookAtPosition(CHARA_AKIR, 1, &local_EC, 0);
    if (arg1 == 0) {
      Script_scenCreateTask(script_15519, 1, 0);
      Script_seqEnterEvtCam(7000, arg0, arg0);
    } else {
      local_F8 = Script_scenCreateTask(script_15CC1, 0);
      Script_scenCreateTask(script_15519, 1, 4);
      Script_seqEnterEvtCam(7010, arg0, arg0);
    }
    L015F2E:;
    SCNF_WaitFrames_0000(1);
    if (!~task_26A) goto L015F2E;
    if (arg1 == 0) {
      local_FC = SCNF_RequestActionSelectorFromParams_0008(0xFFFF, 40, 41, 0xFFFF, 0x7FFF, 0xFFFF, ACTION_SELECTOR_MODE_STANDARD_TIMEOUT_CANCEL);
    } else {
      local_FC = SCNF_RequestActionSelectorFromParams_0008(0xFFFF, 4, 34, 0xFFFF, 0x7FFF, 0xFFFF, ACTION_SELECTOR_MODE_STANDARD_TIMEOUT_CANCEL);
    }
    Script_SetCharaLookAtPosition(CHARA_AKIR, -1, 0, 0);
    if ((local_FC == 40) | (local_FC == 4)) {
      local_E8 = script_AFDB(2, 100, 1);
      local_E4 = local_E8;
      if (local_E4 == 0) {
        if (arg1 == 0) {
          Script_scenCreateTask(script_15519, 1, 1);
        }
        local_FC = 1;
      } else if (local_E4 == 1) {
        Script_scenCreateTask(script_15519, 1, 2);
        SCNF_SetCashValue(0, 1);
        local_FC = 0;
      } else {
        local_FC = 0;
      }
    } else {
      Script_scenCreateTask(script_15519, 1, 3);
      local_FC = 0;
    }
    if (local_F8) {
      Script_srchControl((void *)local_F8);
    }
    (void)local_FC;
  }
  end:;
}

static void script_16043(int32_t arg0) {
  int32_t local_FC;
  if (arg0 == 0) {
    local_FC = -0x40800000;
  } else {
    local_FC = 0x42FA0000;
    Script_motmSetBlendFrames(CHARA_AKIR, MOTM_BLEND_FRAMES_10);
  }
  script_149A3(CHARA_AKIR, 1, 7, 1);
  script_149A3(CHARA_AKIR, 0, 8, 1);
  Script_PlayCharaMotion(CHARA_AKIR, 4353, -1.0f, -1.0f, local_FC, 0.0f, 0);
  SCNF_WaitFrames_0000(1);
  Script_SetCharaMotionPlaybackRate(CHARA_AKIR, 1.25f);
  script_15781();
  Script_scenCreateTask(script_144E0, 0);
  return;
}

static void script_160D6(int32_t arg0, int32_t arg1) {
  int32_t local_FC;
  Script_shenClearFreeState(0);
  script_14129();
  local_FC = script_1631C();
  if (arg1 == 0) {
    Script_seqEnterEvtCam(7006, arg0, arg0);
    script_13DD6(1);
  } else {
    Script_seqEnterEvtCam(7011, arg0, arg0);
  }
  script_16043(arg1);
  SCNF_WaitFrames_0000(1);
  Script_scenCreateTask(script_15758, 0);
  Script_scenCreateTask(script_15CD4, 0);
  SCNF_WaitFrames_0000(1);
  while (Script_IsCharaMotionFinished(CHARA_AKIR) == 0) {
    SCNF_WaitFrames_0000(1);
  }
  Script_scenCreateTask(script_1562F, 1, local_FC);
  Script_shenClearFreeState(1);
  script_151D2(local_FC);
  script_141ED();
  script_1618C();
  return;
}

static void script_1618C(void) {
  Vector3 local_F4;
  Script_motmToggleFlags(CHARA_AKIR, 2, 0x40);
  Script_PlayCharaMotion(CHARA_AKIR, 4353, -1.0f, -1.0f, 135.0f, 0.0f, 0);
  script_15CA9();
  Script_EVT_GetPosition(task_260, -1, &local_F4, 0);
  (&local_F4)->y = (&local_F4)->y + 0.6f;
  Script_seqEnterEvtCam(7005, CHARA_AKIR, CHARA_AKIR);
  SCNF_WaitFrames_0000(1);
  Script_motmToggleFlags(CHARA_AKIR, 1, 0x40);
  return;
}

static void script_16206(int32_t arg0) {
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
  local_C8 = script_13359(-1, arg0);
  Script_ProcessSubtitle("JINGURU01", 0);
  Script_GetCassetteFlags(local_C8, 1, 0, 0);
  SCNF_StartIconSystemMenu(local_C8);
  Script_fixoClearAttachment(SHEN_NAME('KEIH'));
  return;
}

static int32_t script_1631C(void) {
  float local_FC;
  float local_F8;
  int32_t local_F4;
  int32_t local_F0;
  int32_t local_EC;
  int32_t unused_E8;
  GameDateTime local_E0;
  local_F0 = Script_SetOrGetWOCCValue(task_260, -1);
  local_FC = Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 1.0f);
  local_EC = 2;
  if (!(local_FC >= 0.333333313f)) {
    local_EC = 0;
  } else {
    local_FC = local_FC - 0.333333313f;
    if (!(local_FC >= 0.05f)) {
      if (data_1E91C[local_F0] != 0) {
        local_EC = 3;
        if (local_F0 == 4) {
          float local_DC;
          local_DC = Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 1.0f);
          if (!(local_DC >= 0.333333343f)) {
            local_EC = 4;
          }
        }
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
  {
    int32_t local_DC;
    local_DC = local_EC;
    switch (local_DC) {
      int32_t local_D8;
      int32_t local_D4;
      case 0:
        local_F0 = 0;
        local_F4 = Script_FloatUtils(ScriptMathOp_RandomIntBelow, data_1E90C[local_F0]);
        break;
      case 1:
        local_F0 = 14;
        local_F4 = Script_FloatUtils(ScriptMathOp_RandomIntBelow, data_1E90C[local_F0]);
        break;
      case 2:
        local_F4 = Script_FloatUtils(ScriptMathOp_RandomIntBelow, data_1E90C[local_F0] - data_1E91C[local_F0]);
        break;
      case 3:
        local_D8 = data_1E90C[local_F0] - data_1E91C[local_F0] - 1;
        local_D4 = 0;
        while (local_D4 < data_1E91C[local_F0]) {
          if (Script_GetOwnedItemCount(script_13359(local_F0, local_D8 + local_D4), 0) == 0) {
            if (!(Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 1.0f) >= 0.666666687f)) {
              break;
            }
          }
          local_D4 = local_D4 + 1;
        }
        if (local_D4 < data_1E91C[local_F0]) {
          local_F4 = data_1E90C[local_F0] - local_D4 - 1;
        } else {
          local_D8 = Script_FloatUtils(ScriptMathOp_RandomIntBelow, data_1E91C[local_F0]) + 1;
          local_F4 = data_1E90C[local_F0] - local_D8;
        }
        break;
      case 4:
        local_F0 = 15;
        local_D8 = 0;
        while (local_D8 < data_1E90C[local_F0]) {
          if (Script_GetOwnedItemCount(script_13359(local_F0, local_D8), 0) == 0) {
            if (!(Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 1.0f) >= 0.333333343f)) {
              break;
            }
          }
          local_D8 = local_D8 + 1;
        }
        if (local_D8 < data_1E90C[local_F0]) {
          local_F4 = local_D8;
        } else {
          local_F4 = Script_FloatUtils(ScriptMathOp_RandomIntBelow, data_1E90C[local_F0]);
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
  }
  local_F8 = 1.0f;
  Script_ReturnTimeDate(&local_E0);
  if ((&local_E0)->hours >= 17) {
    local_F8 = -1.0f;
  } else {
    local_F8 = 1.0f;
  }
  local_F8 = -1.0f;
  if (!(local_F8 >= 0.0f)) {
    (void)((task_26B = -1) & 2);
  } else {
    (void)((task_26B = 0) & 2);
  }
  script_135E6(SHEN_NAME('KEIH'), local_F0, local_F4, local_F8);
  Script_charaDisplay(SHEN_NAME('KEIH'), CDM_Hide);
  return local_F4;
}

static int32_t script_1687C(int32_t arg0, const char *arg1, const char *arg2, ...) {
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

static void script_168CE(int32_t arg0, Vector3 *arg1) {
  Script_MoveObjectToObject(SHEN_NAME('DAMY'), arg0);
  Script_EVT_SetPosition(SHEN_NAME('DAMY'), POS_XYZ, arg1);
  Script_MoveObjectToObject(SHEN_NAME('DAMY'), -1);
  Script_EVT_GetPosition(SHEN_NAME('DAMY'), -1, arg1, 0);
  return;
}

static void script_16906(int32_t arg0, Vector3i *arg1) {
  int32_t unused_F4[3];
  Script_MoveObjectToObject(SHEN_NAME('DAMY'), arg0);
  Script_EVT_SetRotation(SHEN_NAME('DAMY'), 0x38000000, arg1, 0);
  Script_MoveObjectToObject(SHEN_NAME('DAMY'), -1);
  Script_mtwkGetSetRotation(SHEN_NAME('DAMY'), arg1, 0);
  return;
}

static void script_1693E(int32_t arg0) {
  task_2B4 = arg0;
  return;
}

static void script_16948(void) {
  task_2AC = 0;
  Script_srchUtils(SRCH_SCRIPT_InitEntryTableFromTaskOffset, data_2009C);
  task_2B0 = 1;
  task_2B4 = 0;
  return;
}

static void script_1696C(void) {
  int32_t local_FC;
  int16_t local_FA;
  int32_t local_F4;
  task_2A8 = -1;
  task_2A8 = Script_srchUtils(-1);
  if (Script_srchUtils(SRCH_SCRIPT_GetCurrentTargetEntryIndex) >= 0) {
    Script_scenUtils(SCENARIO_CMD_INIT_SCNT);
  } else {
    Script_scenUtils(SCENARIO_CMD_UPDATE_TIMER);
  }
  if (task_2A8 != -1) {
    Script_scenToggleVisibleFor(SHEN_NAME('YKUR'), 1);
    Script_scenToggleVisibleFor(SHEN_NAME('YKUL'), 1);
    Script_ryoControl(PCCTL_DisablePlayerControl);
    local_FC = script_17057(task_2A8);
    local_FA = Script_ScenDataUtil(data_2009C, 2, local_FC * 16 + 1);
    local_F4 = local_FA;
    if ((local_F4 == 0) | (local_F4 == 0) | (local_F4 == -1)) {
      script_19F7D(task_2A8, 0);
    } else if (local_F4 == 9) {
      Script_InvestigationUtils(ShenmueUtils_BeginInteractionMode, 0, 60);
      for (;;) {
        if (Script_InvestigationUtils(ShenmueUtils_PollInteractionModeDone) != 0) {
          break;
        }
        SCNF_WaitFrames_0000(1);
      }
      script_16AA3(local_FC);
      Script_InvestigationUtils(ShenmueUtils_RequestInteractionModeExit);
      for (;;) {
        if (Script_InvestigationUtils(ShenmueUtils_PollInteractionModeDone) != 0) {
          break;
        }
        SCNF_WaitFrames_0000(1);
      }
    } else {
      script_16AA3(local_FC);
    }
    Script_ryoControl(RYO_RestoreControl);
    Script_scenToggleVisibleFor(SHEN_NAME('YKUR'), 0);
    Script_scenToggleVisibleFor(SHEN_NAME('YKUL'), 0);
  }
  return;
}

static void script_16AA3(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  int32_t local_F0;
  int32_t local_EC;
  int32_t local_E8;
  int32_t local_E4;
  int8_t local_E3;
  int32_t local_DC;
  int32_t local_D8;
  int32_t local_D4;
  int32_t local_D0;
  int8_t local_CF;
  local_E3 = 0;
  local_FC = Script_ScenDataUtil(data_2009C, 4, arg0 * 8 + 1);
  local_D8 = 0;
  local_E4 = 0;
  for (;;) {
    if (local_FC == Script_ScenDataUtil(&data_20848, 4, local_E4 * 2)) {
      local_D8 = Script_ScenDataUtil(&data_20848, 4, local_E4 * 2 + 1);
      break;
    }
    if (Script_ScenDataUtil(&data_20848, 4, local_E4 * 2) == -1) {
      break;
    }
    local_E4 = local_E4 + 1;
  }
  if (-1) {
    local_CF = script_AFDB(2, local_D8, 0);
  } else {
    local_CF = script_AFDB(0, local_D8, 0);
  }
  local_FC = Script_ScenDataUtil(data_2009C, 4, arg0 * 8 + 1);
  local_E4 = 0;
  if (Script_ScenDataUtil(data_207A0, 4, local_E4) != -1) {
    local_F4 = Script_ScenDataUtil(data_207A0, 4, local_E4);
    local_F0 = Script_ScenDataUtil(data_207A0, 4, local_E4 + 1);
    local_EC = Script_ScenDataUtil(data_207A0, 4, local_E4 + 2);
    local_E8 = Script_ScenDataUtil(data_207A0, 4, local_E4 + 3);
    local_E4 = 4;
    for (;;) {
      if ((Script_ScenDataUtil(data_207A0, 4, local_E4) == -1) & (Script_ScenDataUtil(data_207A0, 4, local_E4 + 1) == -1)) {
        local_F4 = 0;
        local_F0 = 0;
        local_EC = 0;
        local_E8 = 0;
        break;
      }
      if (Script_ScenDataUtil(data_207A0, 4, local_E4) == -1) {
        local_E4 = local_E4 + 1;
        local_F4 = Script_ScenDataUtil(data_207A0, 4, local_E4);
        local_F0 = Script_ScenDataUtil(data_207A0, 4, local_E4 + 1);
        local_EC = Script_ScenDataUtil(data_207A0, 4, local_E4 + 2);
        local_E8 = Script_ScenDataUtil(data_207A0, 4, local_E4 + 3);
        local_E4 = local_E4 + 4;
      }
      if (local_FC == Script_ScenDataUtil(data_207A0, 4, local_E4)) {
        break;
      }
      local_E4 = local_E4 + 1;
    }
  }
  Script_InvestigationUtils(ShenmueUtils_QueueSndCommand, local_F4, local_F0);
  if (Script_ScenDataUtil(data_2009C, 4, arg0 * 8 + 7) == 0) {
    Script_InvestigationUtils(ShenmueUtils_BeginTargetModeDefault, local_FC);
  } else {
    Script_InvestigationUtils(ShenmueUtils_BeginTargetModeByType, local_FC, Script_ScenDataUtil(data_2009C, 4, arg0 * 8 + 7) - 1);
  }
  for (;;) {
    SCNF_WaitFrames_0000(1);
    if (Script_InvestigationUtils(ShenmueUtils_UpdateStateMachine) != 0) {
      break;
    }
  }
  if (0) {
    Script_scenCreateTask(script_19F7D, 2, task_2A8, 5);
  } else if (local_CF == 0) {
    Script_scenCreateTask(script_19F7D, 2, task_2A8, 5);
  } else {
    Script_scenCreateTask(script_19F7D, 2, task_2A8, 0);
  }
  local_D0 = 0;
  for (;;) {
    if (task_2B4 < local_D0) {
      break;
    }
    local_D0 = local_D0 + 1;
    SCNF_WaitFrames_0000(1);
  }
  local_DC = Script_shenGetStats(STATS_CASH);
  if (local_D8 == 0) {
    Script_InvestigationUtils(ShenmueUtils_QueueSndCommand, local_EC, local_E8);
    Script_InvestigationUtils(ShenmueUtils_RequestTransitionVariantA);
    for (;;) {
      if (Script_InvestigationUtils(ShenmueUtils_UpdateStateMachine) != 0) {
        break;
      }
      SCNF_WaitFrames_0000(1);
    }
  } else {
    local_E4 = script_17057(task_2A8);
    local_F8 = Script_ScenDataUtil(data_2009C, 4, local_E4 * 8 + 6);
    if (local_CF == 1) {
      if (-1) {
        if (-1) {
          script_B11D(local_CF, 2);
        } else {
          script_19F7D(task_2A8, 2);
        }
      }
      Script_InvestigationUtils(ShenmueUtils_QueueSndCommand, local_EC, local_E8);
      Script_InvestigationUtils(ShenmueUtils_RequestTransitionVariantA);
      if (0) {
        script_19F7D(task_2A8, 2);
      } else {
        script_19F7D(task_2A8, 0);
      }
    } else if ((local_CF == 2) | (local_CF == 3) | (local_CF == 4)) {
      if (-1) {
        if (-1) {
          script_B11D(local_CF, 2);
        } else {
          script_B11D(local_CF, 0);
        }
      }
      Script_InvestigationUtils(ShenmueUtils_QueueSndCommand, local_EC, local_E8);
      Script_InvestigationUtils(ShenmueUtils_RequestTransitionVariantA);
      if (0) {
        script_B11D(local_CF, 0);
      } else {
        script_19F7D(task_2A8, 0);
      }
    } else if ((local_F8 == 0) | (task_2B0 == 0)) {
      if (task_2B0 == 0) {
        for (;;) {
          if (Script_GetControllerInput(1) == 0x400) {
            break;
          }
          SCNF_WaitFrames_0000(1);
        }
      }
      Script_InvestigationUtils(ShenmueUtils_QueueSndCommand, local_EC, local_E8);
      Script_InvestigationUtils(ShenmueUtils_RequestTransitionVariantA);
    } else {
      local_D4 = SCNF_RequestActionSelectorFromParams_0008(0xFFFF, 40, 41, 0xFFFF, 0x7FFF, 0xFFFF, ACTION_SELECTOR_MODE_STANDARD_TIMEOUT_CANCEL);
      if (local_D4 == 40) {
        if (local_CF == 0) {
          if (local_F8 != 9999) {
            Script_GetCassetteFlags(local_F8, 1, 0, 0);
          }
          Script_InvestigationUtils(ShenmueUtils_RequestTransitionVariantB);
          local_E3 = 1;
          script_19F7D(task_2A8, 1);
        } else {
          Script_InvestigationUtils(ShenmueUtils_QueueSndCommand, local_EC, local_E8);
          Script_InvestigationUtils(ShenmueUtils_RequestTransitionVariantA);
          script_19F7D(task_2A8, 2);
        }
      } else {
        Script_InvestigationUtils(ShenmueUtils_QueueSndCommand, local_EC, local_E8);
        Script_InvestigationUtils(ShenmueUtils_RequestTransitionVariantA);
        script_19F7D(task_2A8, 3);
      }
    }
    if (local_F8 == 0) {
      script_19F7D(task_2A8, 4);
    }
    for (;;) {
      if (Script_InvestigationUtils(ShenmueUtils_UpdateStateMachine) != 0) {
        break;
      }
      SCNF_WaitFrames_0000(1);
    }
    if (local_E3 == 1) {
      Script_srchUtils(SRCH_SCRIPT_Reset);
      Script_charaSetRenderOpacity(CHARA_AKIR, 0.0f);
      script_1A2A6(local_D8);
      Script_charaSetRenderOpacity(CHARA_AKIR, 1.0f);
      Script_SetGameStateTo(AUTH_None);
      Script_ryoControl(RYO_RestoreControl);
    }
  }
  return;
}

static int32_t script_17057(int32_t arg0) {
  int32_t local_FC;
  int32_t local_F8;
  int16_t local_F6;
  local_F8 = 0;
  for (;;) {
    local_F6 = Script_ScenDataUtil(data_2009C, 4, local_F8 * 8);
    if (local_F6 < 0) {
      break;
    }
    if (Script_ScenDataUtil(data_2009C, 4, local_F8 * 8 + 1) == arg0) {
      local_FC = local_F8;
      break;
    }
    local_F8 = local_F8 + 1;
  }
  return local_FC;
}

__attribute__((scn_string_bindings(0, data_20CDC, 2, data_20CF2, 3, data_20CFD, 4, data_20D08)))
static int8_t script_170B6(int32_t arg0) {
  int32_t local_FC;
  int8_t local_FB;
  uint8_t unused_F8[3];
  Vector3 local_EC;
  int32_t local_E8;
  int32_t local_E4;
  int32_t local_E0;
  int32_t local_DC;
  script_9EB2(CHARA_AKIR, -77.892998f, 0.0f, 51.616001f);
  script_9EDA(CHARA_AKIR, 0.0f, 315.0f, 0.0f);
  Script_InitializeOsag(CHARA_AKIR);
  Script_filmCheckOrSetTransitionUtil(1, 0, 0, 0, 0, 255, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_SetHandRootRotation(CHARA_AKIR, 1, 0x15, &data_20CC4);
  Script_handSetFloatsFor(CHARA_AKIR, 1, data_20934, 0);
  Script_handSetFloatsFor(CHARA_AKIR, 0, data_20934, 0);
  Script_seqEnterEvtCam(9010, 0, AUTH_None);
  Script_filmCheckOrSetTransitionUtil(5, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_EVT_GetPosition(CHARA_AKIR, 4, &local_EC, ENTITY_XFORM_WORLD_SPACE);
  Script_SetLookPointDetails(CHARA_MEYS, 16, &local_EC, 0);
  local_FC = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f));
  {
    int32_t local_D8;
    local_D8 = local_FC;
    switch (local_D8) {
      case 0:
        Script_ShowUIMessageFromID("SA1114A007");
        break;
      case 1:
        Script_ShowUIMessageFromID("SA1114A008");
        break;
      case 2:
        Script_ShowUIMessageFromID("SA1114A009");
        break;
    }
  }
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  SCNF_WaitFrames_0000(20);
  Script_ShowUIMessageFromID("SA1114B001");
  script_1AE3E();
  script_9EB2(CHARA_AKIR, -78.039803f, 0.0f, 51.527f);
  script_9EDA(CHARA_AKIR, 0.0f, 328.0f, 0.0f);
  Script_InitializeOsag(CHARA_AKIR);
  Script_motmToggleFlags(CHARA_AKIR, 2, 0x40);
  Script_motmToggleFlags(CHARA_AKIR, 1, 0x20);
  Script_PlayCharaMotion(CHARA_AKIR, 28673, 10.0f, 29.0f, -1.0f, 1.0f, 0);
  Script_seqEnterEvtCam(9030, 0, AUTH_None);
  SCNF_SetCashValue(-arg0, 1);
  Script_motmToggleFlags(CHARA_AKIR, 2, 0x20);
  Script_motmToggleFlags(CHARA_AKIR, 2, 0x40);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_SetHandRootRotation(CHARA_AKIR, 1, 0x15, &data_20CD0);
  Script_handSetFloatsFor(CHARA_AKIR, 1, data_20A18, 10);
  script_9EB2(CHARA_AKIR, -77.892998f, 0.0f, 51.616001f);
  script_9EDA(CHARA_AKIR, 0.0f, 315.0f, 0.0f);
  Script_InitializeOsag(CHARA_AKIR);
  script_1A577();
  local_E8 = script_17057(task_2A8);
  local_DC = Script_ScenDataUtil(data_2009C, 4, local_E8 * 8 + 1);
  local_E0 = Script_ScenDataUtil(data_2009C, 4, local_E8 * 8 + 6);
  if ((local_DC == 0x31736E63) | (local_DC == 0x63736E63) | (local_DC == 0x6B736E63) | (local_DC == 0x31636E6D) | (local_DC == 0x32636E6D) | (local_DC == 0x33636E6D) | (local_DC == 0x34636E6D) | (local_DC == 0x35636E6D) | (local_DC == 0x36636E6D) | (local_DC == 0x37636E6D) | (local_DC == 0x38636E6D) | (local_DC == 0x39636E6D) | (local_DC == 0x61636E6D) | (local_DC == 0x62636E6D) | (local_DC == 0x63636E6D) | (local_DC == 0x64636E6D) | (local_DC == 0x65636E6D) | (local_DC == 0x66636E6D) | (local_DC == 0x67636E6D) | (local_DC == 0x68636E6D) | (local_DC == 0x69636E6D) | (local_DC == 0x6A636E6D) | (local_DC == 0x6B636E6D)) {
    local_FB = 1;
  } else {
    local_FB = 0;
  }
  if (local_FB == 0) {
    Script_seqEnterEvtCam(9060, CHARA_AKIR, CHARA_AKIR);
  } else {
    Script_seqEnterEvtCam(9065, CHARA_AKIR, CHARA_AKIR);
  }
  if ((local_DC == 0x31636E6D) | (local_DC == 0x32636E6D) | (local_DC == 0x33636E6D) | (local_DC == 0x34636E6D) | (local_DC == 0x35636E6D) | (local_DC == 0x36636E6D) | (local_DC == 0x37636E6D) | (local_DC == 0x38636E6D) | (local_DC == 0x39636E6D) | (local_DC == 0x61636E6D) | (local_DC == 0x62636E6D) | (local_DC == 0x63636E6D) | (local_DC == 0x64636E6D) | (local_DC == 0x65636E6D) | (local_DC == 0x66636E6D) | (local_DC == 0x67636E6D) | (local_DC == 0x68636E6D) | (local_DC == 0x69636E6D) | (local_DC == 0x6A636E6D) | (local_DC == 0x6B636E6D)) {
    int32_t local_D8;
    local_D8 = local_DC;
    if (local_D8 == 0x31636E6D) {
      local_E4 = 2;
    } else if (local_D8 == 0x32636E6D) {
      local_E4 = 3;
    } else if (local_D8 == 0x33636E6D) {
      local_E4 = 19;
    } else if (local_D8 == 0x34636E6D) {
      local_E4 = 20;
    } else if (local_D8 == 0x35636E6D) {
      local_E4 = 24;
    } else if (local_D8 == 0x36636E6D) {
      local_E4 = 8;
    } else if (local_D8 == 0x37636E6D) {
      local_E4 = 14;
    } else if (local_D8 == 0x38636E6D) {
      local_E4 = 15;
    } else if (local_D8 == 0x39636E6D) {
      local_E4 = 25;
    } else if (local_D8 == 0x61636E6D) {
      local_E4 = 6;
    } else if (local_D8 == 0x62636E6D) {
      local_E4 = 5;
    } else if (local_D8 == 0x63636E6D) {
      local_E4 = 16;
    } else if (local_D8 == 0x64636E6D) {
      local_E4 = 18;
    } else if (local_D8 == 0x65636E6D) {
      local_E4 = 1;
    } else if (local_D8 == 0x66636E6D) {
      local_E4 = 7;
    } else if (local_D8 == 0x67636E6D) {
      local_E4 = 21;
    } else if (local_D8 == 0x68636E6D) {
      local_E4 = 22;
    } else if (local_D8 == 0x69636E6D) {
      local_E4 = 23;
    } else if (local_D8 == 0x6A636E6D) {
      local_E4 = 9;
    } else if (local_D8 == 0x6B636E6D) {
      local_E4 = 0;
    } else {
      local_E4 = 2;
    }
    Script_GetCassetteFlags(local_E4, 1, 0, 1);
    SCNF_StartIconSystemMenu(local_E4 + 255);
  } else {
    SCNF_StartIconSystemMenu(local_E0);
  }
  if (local_E0 == 60) {
    Script_GetCassetteFlags(local_E0, 3, 0, 0);
  }
  {
    int32_t local_D8;
    local_D8 = local_DC;
    if (local_D8 == 0x31736E63) {
      local_E4 = 0;
    } else if (local_D8 == 0x63736E63) {
      local_E4 = 1;
    } else if (local_D8 == 0x6B736E63) {
      local_E4 = 2;
    } else if (local_D8 == 0x31626E63) {
      local_E4 = 5;
    } else if (local_D8 == 0x33626E63) {
      local_E4 = 46;
    } else if (local_D8 == 0x6B746E63) {
      local_E4 = 10;
    } else if (local_D8 == 0x706E6E63) {
      local_E4 = 11;
    } else if (local_D8 == 0x706D6E63) {
      local_E4 = 12;
    } else if (local_D8 == 0x6B6D6E63) {
      local_E4 = 13;
    } else if (local_D8 == 0x31636E6D) {
      local_E4 = 15;
    } else if (local_D8 == 0x32636E6D) {
      local_E4 = 17;
    } else if (local_D8 == 0x33636E6D) {
      local_E4 = 26;
    } else if (local_D8 == 0x34636E6D) {
      local_E4 = 31;
    } else if (local_D8 == 0x35636E6D) {
      local_E4 = 22;
    } else if (local_D8 == 0x36636E6D) {
      local_E4 = 37;
    } else if (local_D8 == 0x37636E6D) {
      local_E4 = 23;
    } else if (local_D8 == 0x38636E6D) {
      local_E4 = 38;
    } else if (local_D8 == 0x39636E6D) {
      local_E4 = 14;
    } else if (local_D8 == 0x61636E6D) {
      local_E4 = 34;
    } else if (local_D8 == 0x62636E6D) {
      local_E4 = 35;
    } else if (local_D8 == 0x63636E6D) {
      local_E4 = 33;
    } else if (local_D8 == 0x64636E6D) {
      local_E4 = 24;
    } else if (local_D8 == 0x65636E6D) {
      local_E4 = 30;
    } else if (local_D8 == 0x66636E6D) {
      local_E4 = 26;
    } else if (local_D8 == 0x67636E6D) {
      local_E4 = 27;
    } else if (local_D8 == 0x68636E6D) {
      local_E4 = 28;
    } else if (local_D8 == 0x69636E6D) {
      local_E4 = 32;
    } else if (local_D8 == 0x6A636E6D) {
      local_E4 = 29;
    } else if (local_D8 == 0x6B636E6D) {
      local_E4 = 16;
    } else {
      local_E4 = 0;
    }
  }
  Script_PlayerStatsUtils(PS_SetSparsePairU8Value, 2, local_E4, Script_PlayerStatsUtils(PS_GetSparsePairU8Value, 2, local_E4) + 1);
  if (local_FB == 1) {
    Script_seqEnterEvtCam(9066, CHARA_AKIR, CHARA_AKIR);
    Script_ShowUIMessageFromID("SA1114B004");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
  } else {
    int32_t local_D8;
    local_FC = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f));
    local_D8 = local_FC;
    switch (local_D8) {
      case 0:
        Script_ShowUIMessageFromID("SA1114B002");
        while (Script_convGetIsActive(0)) {
          SCNF_WaitFrames_0000(1);
        }
        break;
      case 1:
        Script_ShowUIMessageFromID(data_20D1E);
        while (Script_convGetIsActive(0)) {
          SCNF_WaitFrames_0000(1);
        }
        break;
    }
  }
  return local_FB;
}

static void script_17A7E(void) {
  int32_t local_FC;
  int32_t unused_F8;
  Vector3 local_EC;
  Script_filmCheckOrSetTransitionUtil(1, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_motmToggleFlags(CHARA_AKIR, 2, 0x40);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_ccowUtils(CHARA_AKIR, 2);
  Script_InitializeOsag(CHARA_AKIR);
  Script_ResetHandMotion(CHARA_AKIR, 1, 1, 0);
  Script_ResetHandMotion(CHARA_AKIR, 1, 0, 0);
  {
    int32_t unused_E0[3];
    Vector3i local_D4;
    Script_SetHandRootRotation(CHARA_AKIR, 1, 0x15, &local_D4);
  }
  Script_handSetFloatsFor(CHARA_AKIR, 1, data_20934, 10);
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, 0x52494B41)) {
    Script_SetCharaLookAtPosition(CHARA_AKIR, -13, 0, 0);
    Script_FaceLookAtTargetUtils(CHARA_AKIR, 0);
  }
  if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, 0x5359454D)) {
    Script_SetCharaLookAtPosition(CHARA_MEYS, -13, 0, 0);
    Script_FaceLookAtTargetUtils(CHARA_MEYS, 0);
  }
  Script_EVT_GetPosition(0x78626B64, -1, &local_EC, 0);
  Script_SetCharaLookAtPosition(CHARA_AKIR, 0x10000, &local_EC, 0);
  Script_FaceLookAtTargetUtils(CHARA_AKIR, 2, &local_EC, 1);
  script_9EB2(0x78626B64, -77.11f, 0.75f, 51.766998f);
  script_9EDA(0x78626B64, 0.0f, 45.0f, 0.0f);
  Script_charaDisplay(0x78626B64, CDM_Show);
  script_9EB2(CHARA_AKIR, -77.49f, 0.0f, 51.983002f);
  script_9EDA(CHARA_AKIR, 0.0f, 315.0f, 0.0f);
  Script_ResetHandMotion(CHARA_AKIR, 1, 0, 0);
  Script_ResetHandMotion(CHARA_AKIR, 1, 1, 0);
  Script_handSetFloatsFor(CHARA_AKIR, 1, data_20934, 10);
  Script_motmToggleFlags(CHARA_AKIR, 2, 0x40);
  Script_PlayCharaMotion(CHARA_AKIR, 28677, 70.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_SetCharaMotionPlaybackRate(CHARA_AKIR, 1.0f);
  Script_seqEnterEvtCam(9100, CHARA_AKIR, CHARA_AKIR);
  L017C9E:;
  local_FC = Script_FloatUtils(ScriptMathOp_RandomIntBelow, 1000);
  if (local_FC <= 9) {
    task_2C0 = 1;
  } else if (local_FC <= 28) {
    task_2C0 = 2;
  } else if (local_FC <= 60) {
    task_2C0 = 3;
  } else if (local_FC <= 121) {
    task_2C0 = 4;
  } else if (local_FC <= 245) {
    task_2C0 = 5;
  } else {
    task_2C0 = 6;
    goto L017D19;
    goto L017C9E;
  }
  L017D19:;
  Script_PlayerStatsUtils(PS_SetMatrix4x8U16Slot, 2, task_2C0, Script_PlayerStatsUtils(PS_GetMatrix4x8U16Slot, 2, task_2C0) + 1);
  Script_handSetFloatsFor(CHARA_AKIR, 1, data_20934, 10);
  Script_handSetFloatsFor(CHARA_AKIR, 0, data_20934, 10);
  {
    int32_t unused_D4[6];
    int32_t local_D0;
    local_D0 = 0;
    for (;;) {
      if (local_D0 == 75) {
        Script_sicnUtils(0, 4, 1);
      }
      if (Script_IsCharaMotionFinished(CHARA_AKIR)) {
        Script_PlayCharaMotion(CHARA_AKIR, 28676, -1.0f, -1.0f, -1.0f, 1.0f, 0);
        Script_executeAicaCommand(918697, 0, 0);
      } else {
        if ((local_D0 > 60) & (Script_GetControllerInput(1) == 0x400)) {
          break;
        }
        local_D0 = local_D0 + 1;
        if (local_D0 == 180) {
          break;
        }
      }
      SCNF_WaitFrames_0000(1);
    }
  }
  Script_sicnUtils(2, 4, 0);
  local_EC.x = -77.307098f;
  (&local_EC)->y = 1.2525f;
  (&local_EC)->z = 51.8316f;
  Script_SetCharaLookAtPosition(CHARA_AKIR, 0x10000, &local_EC, 0);
  Script_FaceLookAtTargetUtils(CHARA_AKIR, 2, &local_EC, 1);
  Script_seqEnterEvtCam(9105, 0, AUTH_None);
  local_EC.x = 0.118f;
  (&local_EC)->y = 0.02f;
  (&local_EC)->z = -0.062f;
  {
    int32_t unused_E0[3];
    Vector3i local_D4;
    local_D4.x = 8008;
    (&local_D4)->y = 9827;
    (&local_D4)->z = 5183;
    Script_fixoAttachEx(0x6C636B64, CHARA_AKIR, 18, &local_EC, &local_D4);
  }
  Script_PlayCharaMotion(CHARA_AKIR, 28675, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_SetCharaMotionPlaybackRate(CHARA_AKIR, 1.2f);
  {
    int32_t unused_E0[3];
    Vector3i local_D4;
    int32_t local_D0;
    int32_t local_CC;
    local_CC = 0;
    for (;;) {
      local_CC = local_CC + 2;
      if (local_CC == 100) {
        Script_executeAicaCommand(853161, 0, 0);
      }
      if (local_CC == 170) {
        Script_fixoClearAttachment(0x6C636B64);
        script_9EB2(0x6C636B64, 0.0f, -1000.0f, 0.0f);
        local_EC.x = 0.137f;
        (&local_EC)->y = 0.046f;
        (&local_EC)->z = -0.05f;
        local_D4.x = 17668;
        (&local_D4)->y = 52596;
        (&local_D4)->z = 39763;
        Script_fixoAttachEx(0x706F6B64, CHARA_AKIR, 18, &local_EC, &local_D4);
        {
          local_D0 = 0;
          for (;;) {
            local_EC.x = 0.0f;
            (&local_EC)->y = 0.0f;
            (&local_EC)->z = -0.004f;
            Script_charaPosRotUtils(0x706F6B64, local_D0 + 152, &local_EC, 0, 0);
            local_D0 = local_D0 + 1;
            if (local_D0 == 9) {
              break;
            }
          }
        }
        if (task_2C0 == 6) {
          local_EC.x = 0.0f;
          (&local_EC)->y = 0.0f;
          (&local_EC)->z = 0.0f;
          Script_charaPosRotUtils(0x706F6B64, 160, &local_EC, 0, 0);
        } else {
          local_EC.x = 0.0f;
          (&local_EC)->y = 0.0f;
          (&local_EC)->z = 0.0f;
          Script_charaPosRotUtils(0x706F6B64, task_2C0 + 152, &local_EC, 0, 0);
        }
        if ((task_2C0 > 0) & (task_2C0 < 6)) {
          local_EC.x = 0.0f;
          (&local_EC)->y = 0.0f;
          (&local_EC)->z = 0.0f;
          if (task_2C0 == 1) {
            Script_charaPosRotUtils(0x706F6B64, 158, &local_EC, 0, 0);
          } else {
            Script_charaPosRotUtils(0x706F6B64, 159, &local_EC, 0, 0);
          }
        }
        SCNF_WaitFrames_0000(1);
        Script_seqEnterEvtCam(9110, CHARA_AKIR, CHARA_AKIR);
      }
      if (local_CC == 220) {
        script_19E18();
      }
      if (Script_IsCharaMotionFinished(CHARA_AKIR)) {
        break;
      }
      SCNF_WaitFrames_0000(1);
    }
  }
  Script_PlayCharaMotion(CHARA_AKIR, 28678, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  for (;;) {
    if (Script_IsCharaMotionFinished(CHARA_AKIR)) {
      break;
    }
    Script_RotateNodeUtils(CHARA_AKIR, &data_1BFF0, 0);
    SCNF_WaitFrames_0000(1);
  }
  if (task_2C0 == 5) {
    task_2BC = script_19213();
  }
  if (task_2C0 == 6) {
    Script_seqEnterEvtCam(9115, CHARA_AKIR, CHARA_AKIR);
    if (task_2C0 == 6) {
      script_19D7A();
    }
  }
  script_19E79();
  {
    int32_t unused_D4[6];
    int32_t local_D0;
    local_D0 = 0;
    for (;;) {
      local_EC.x = 0.0f;
      (&local_EC)->y = 0.0f;
      (&local_EC)->z = -0.004f;
      Script_charaPosRotUtils(0x706F6B64, local_D0 + 152, &local_EC, 0, 0);
      local_D0 = local_D0 + 1;
      if (local_D0 == 9) {
        break;
      }
    }
  }
  if (!(task_2C0 + 1 == 1)) goto L0181A6;
  L0181A6:;
  if (task_2C0 == 6) {
    SCNF_WaitFrames_0000(20);
  }
  Script_fixoClearAttachment(0x706F6B64);
  script_9EB2(0x706F6B64, 0.0f, -1000.0f, 0.0f);
  Script_FaceLookAtTargetUtils(CHARA_AKIR, 0);
  return;
}

static void script_181E3(void) {
  Vector3 local_F4;
  Vector3i local_E8;
  int32_t unused_E4;
  int32_t local_E0;
  script_9EB2(0x78626B64, -76.998001f, 0.75f, 51.813999f);
  script_9EDA(0x78626B64, 0.0f, 45.0f, 0.0f);
  Script_charaDisplay(0x78626B64, CDM_Show);
  Script_SetCharaLookAtPosition(CHARA_AKIR, -20, 0, 0);
  Script_SetCharaLookAtPosition(CHARA_MEYS, -20, 0, 0);
  {
    int32_t local_DC;
    local_DC = task_2C0;
    switch (local_DC) {
      case 0:
        task_2BC = 0x70736E63;
        break;
      case 1:
        task_2BC = script_18935();
        break;
      case 2:
        task_2BC = script_18935();
        break;
      case 3:
        task_2BC = script_18935();
        break;
      case 4:
        task_2BC = script_18935();
        break;
      case 5:
        break;
    }
  }
  {
    int32_t local_DC;
    local_DC = task_2C0;
    switch (local_DC) {
      case 1:
        local_F4.x = (&local_F4)->y = (&local_F4)->z = 0.8f;
        Script_hmdlApplyScale(task_2BC, &local_F4);
        local_F4.x = -77.6259f;
        (&local_F4)->y = 0.791f;
        (&local_F4)->z = 51.058998f;
        local_E8.x = 0;
        (&local_E8)->y = 0xA000;
        (&local_E8)->z = 0;
        Script_EVT_SetPosition(task_2BC, POS_XYZ, &local_F4);
        Script_EVT_SetRotation(task_2BC, 0x38000000, &local_E8, 0);
        Script_charaDisplay(task_2BC, CDM_Show);
        break;
      case 2:
        local_F4.x = -77.6259f;
        (&local_F4)->y = 0.791f;
        (&local_F4)->z = 51.058998f;
        local_E8.x = 0;
        (&local_E8)->y = 0x6000;
        (&local_E8)->z = 0;
        Script_EVT_SetPosition(task_2BC, POS_XYZ, &local_F4);
        Script_EVT_SetRotation(task_2BC, 0x38000000, &local_E8, 0);
        Script_charaDisplay(task_2BC, CDM_Show);
        break;
      case 3:
        local_F4.x = -77.604897f;
        (&local_F4)->y = 0.754f;
        (&local_F4)->z = 51.162998f;
        local_E8.x = 0xC000;
        (&local_E8)->y = 0xE000;
        (&local_E8)->z = 0;
        Script_EVT_SetPosition(task_2BC, POS_XYZ, &local_F4);
        Script_EVT_SetRotation(task_2BC, 0x38000000, &local_E8, 0);
        Script_charaDisplay(task_2BC, CDM_Show);
        break;
      case 4:
        local_F4.x = -77.567001f;
        (&local_F4)->y = 0.772f;
        (&local_F4)->z = 51.1f;
        local_E8.x = 0;
        (&local_E8)->y = 0xE000;
        (&local_E8)->z = 0;
        Script_EVT_SetPosition(task_2BC, POS_XYZ, &local_F4);
        Script_EVT_SetRotation(task_2BC, 0x38000000, &local_E8, 0);
        Script_charaDisplay(task_2BC, CDM_Show);
        break;
      case 5:
        local_F4.x = -77.584f;
        (&local_F4)->y = 0.772f;
        (&local_F4)->z = 51.144001f;
        local_E8.x = 0x8000;
        (&local_E8)->y = 0xC000;
        (&local_E8)->z = 0x8000;
        Script_EVT_SetPosition(0x67676764, POS_XYZ, &local_F4);
        Script_EVT_SetRotation(0x67676764, 0x38000000, &local_E8, 0);
        Script_charaDisplay(0x67676764, CDM_Show);
        Script_EVT_SetPosition(0x66676764, POS_XYZ, &local_F4);
        Script_EVT_SetRotation(0x66676764, 0x38000000, &local_E8, 0);
        Script_charaDisplay(0x66676764, CDM_Show);
        break;
    }
  }
  script_9EB2(CHARA_AKIR, -77.892998f, 0.0f, 51.616001f);
  Script_InitializeOsag(CHARA_AKIR);
  Script_motmToggleFlags(CHARA_MEYS, 2, 4);
  Script_motmToggleFlags(CHARA_MEYS, 2, 0x40);
  Script_motmToggleFlags(CHARA_MEYS, 1, 0x80);
  Script_motmToggleFlags(CHARA_MEYS, 1, 0x20);
  Script_PlayCharaMotion(CHARA_MEYS, 28682, -1.0f, -1.0f, -1, 1.0f, 0);
  Script_InitializeOsag(CHARA_MEYS);
  script_19CFF();
  SCNF_WaitFrames_0000(165);
  Script_PlayCharaMotion(CHARA_MEYS, 28681, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_EVT_GetPosition(CHARA_AKIR, 4, &local_F4, ENTITY_XFORM_WORLD_SPACE);
  Script_SetLookPointDetails(CHARA_MEYS, 16, &local_F4, 0);
  if (task_2C0 == 5) {
    script_19283(task_2BC);
  }
  {
    int32_t local_DC;
    local_DC = task_2C0;
    if ((local_DC == 1) | (local_DC == 2)) {
      script_9EB2(task_2BC, 0.0f, -1000.0f, 0.0f);
      Script_charaDisplay(task_2BC, CDM_Show);
    } else if (local_DC == 3) {
      script_9EB2(task_2BC, 0.0f, -1000.0f, 0.0f);
      Script_charaDisplay(task_2BC, CDM_Show);
    } else {
      if ((local_DC == 4) | (local_DC == 5)) {
        Script_charaDisplay(task_2BC, CDM_Hide);
        Script_gachaponUtils(2, task_2BC);
        goto L0186B5;
      }
    }
  }
  L0186B5:;
  Script_motmToggleFlags(CHARA_AKIR, 2, 0x40);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_seqEnterEvtCam(9118, 0, AUTH_None);
  if (task_2C0 != 1) {
    Script_motmToggleFlags(CHARA_AKIR, 1, 0x20);
    Script_PlayCharaMotion(CHARA_AKIR, 28673, 0.0f, 30.0f, -1.0f, 1.0f, 0);
  }
  {
    int32_t local_DC;
    local_DC = task_2C0;
    if (local_DC == 0) {
    } else if (local_DC == 1) {
      Script_ShowUIMessageFromID("SA1114B006");
      while (Script_convGetIsActive(0)) {
        SCNF_WaitFrames_0000(1);
      }
      Script_ShowUIMessageFromID("SA1114A010");
      while (Script_convGetIsActive(0)) {
        SCNF_WaitFrames_0000(1);
      }
      SCNF_WaitFrames_0000(20);
    } else if (local_DC == 2) {
      if (task_2BC == 0x70736E63) {
        Script_GetCassetteFlags(38, 1, 0, 0);
        SCNF_StartIconSystemMenu(38);
      } else {
        Script_GetCassetteFlags(37, 1, 0, 0);
        SCNF_StartIconSystemMenu(37);
      }
    } else if (local_DC == 3) {
      Script_GetCassetteFlags(task_2B8, 1, 0, 1);
      local_DC = task_2B8;
      switch (local_DC) {
        case 10:
          SCNF_StartIconSystemMenu(265);
          break;
        case 11:
          SCNF_StartIconSystemMenu(266);
          break;
        case 12:
          SCNF_StartIconSystemMenu(267);
          break;
        case 13:
          SCNF_StartIconSystemMenu(268);
          break;
      }
    } else {
      if ((local_DC == 4) | (local_DC == 5)) {
        local_E0 = script_13359(-1, task_2B8);
        Script_GetCassetteFlags(local_E0, 1, 0, 0);
        SCNF_StartIconSystemMenu(local_E0);
        goto L018877;
      }
    }
  }
  L018877:;
  return;
}

static void script_18878(void) {
  int32_t local_FC;
  local_FC = task_2C0;
  switch (local_FC) {
    case 0:
      break;
    case 1:
      Script_seqEnterEvtCam(9121, 0, AUTH_None);
      SCNF_WaitFrames_0000(60);
      break;
    case 2:
      Script_seqEnterEvtCam(9122, 0, AUTH_None);
      SCNF_WaitFrames_0000(60);
      break;
    case 3:
      Script_seqEnterEvtCam(9123, 0, AUTH_None);
      SCNF_WaitFrames_0000(60);
      break;
    case 4:
      Script_seqEnterEvtCam(9124, 0x74687364, 0x74687364);
      SCNF_WaitFrames_0000(100);
      break;
    case 5:
      Script_seqEnterEvtCam(9125, 0x74687364, 0x74687364);
      SCNF_WaitFrames_0000(100);
      break;
  }
  return;
}

static int32_t script_18935(void) {
  Vector3 local_F4;
  Vector3 local_E8;
  Vector3i local_DC;
  float local_D8;
  float local_D4;
  float local_D0;
  float local_CC;
  float local_C8;
  float local_C4;
  int32_t local_C0;
  int32_t local_BC;
  int32_t local_B8;
  int8_t local_B7;
  int8_t local_B6;
  int8_t local_B5;
  int8_t local_B4;
  int32_t local_B0;
  Script_scenToggleVisibleFor(SHEN_NAME('YKUR'), 1);
  Script_scenToggleVisibleFor(SHEN_NAME('YKUL'), 1);
  Script_scenToggleVisibleFor(SHEN_NAME('URDM'), 1);
  Script_charaDisplay(CHARA_AKIR, CDM_Hide);
  Script_charaDisplay(SHEN_NAME('URDM'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('YKUR'), CDM_Show);
  SCNF_WaitFrames_0000(1);
  Script_handSetFloatsFor(SHEN_NAME('YKUR'), 1, data_20BE0, 10);
  local_F4.x = -77.45f;
  (&local_F4)->y = 0.753f;
  (&local_F4)->z = 51.195999f;
  Script_EVT_SetPosition(0x74687364, POS_XYZ, &local_F4);
  Script_charaDisplay(0x74687364, CDM_Show);
  local_DC.x = 17880;
  (&local_DC)->y = 6630;
  (&local_DC)->z = 32210;
  Script_EVT_SetRotation(SHEN_NAME('YKUR'), 0x38000000, &local_DC, 0);
  Script_EVT_SetRotation(SHEN_NAME('URDM'), 0x38000000, &local_DC, 0);
  Script_SetCameraFlags(0);
  script_18878();
  {
    int32_t local_AC;
    local_AC = task_2C0;
    switch (local_AC) {
      case 1:
        local_D8 = -77.518898f;
        local_D4 = 51.1222f;
        local_B5 = 2;
        local_B4 = 9;
        break;
      case 2:
        local_D8 = -77.545097f;
        local_D4 = 51.262001f;
        local_B5 = 1;
        local_B4 = 10;
        break;
      case 3:
        local_D8 = -77.696098f;
        local_D4 = 51.285099f;
        local_B5 = 3;
        local_B4 = 6;
        break;
      case 4:
        local_D8 = -77.79f;
        local_D4 = 51.379002f;
        local_B5 = 3;
        local_B4 = 6;
        break;
    }
  }
  if ((task_2C0 == 1) | (task_2C0 == 3) | (task_2C0 == 4)) {
    local_D8 = local_D8 + (float)(local_B4 - 1) * 0.013f;
    local_D4 = local_D4 + (float)(local_B4 - 1) * 0.013f;
    local_B6 = 1;
  } else {
    local_B6 = 0;
  }
  local_C8 = local_D8 - 0.15f;
  local_C4 = local_D4 + 0.15f;
  for (;;) {
    local_C8 = local_C8 + 0.01f;
    local_C4 = local_C4 - 0.01f;
    local_F4.x = local_C8;
    (&local_F4)->y = 0.8292f;
    (&local_F4)->z = local_C4;
    Script_EVT_SetPosition(SHEN_NAME('YKUR'), POS_XYZ, &local_F4);
    if (!(local_C4 - local_D4 >= 0.0001f)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  local_F4.x = local_D8;
  (&local_F4)->y = 0.8292f;
  (&local_F4)->z = local_D4;
  Script_EVT_SetPosition(SHEN_NAME('URDM'), POS_XYZ, &local_F4);
  local_D0 = 0.0f;
  local_CC = 0.0f;
  Script_seqEnterEvtCam(9120, SHEN_NAME('URDM'), SHEN_NAME('URDM'));
  Script_uiUtils(0, 58, 0);
  Script_shenClearFreeState(1);
  Script_sicnUtils(0, 4, 1);
  local_C0 = 0;
  local_B7 = 0;
  local_B8 = 0;
  for (;;) {
    if (local_B7 == 0) {
      if (Script_GetControllerInput(0) & 0x4000) {
        if (local_B6 > 0) {
          local_B6 = local_B6 - 1;
          local_BC = 0;
          local_B7 = 1;
        }
      }
      if (Script_GetControllerInput(0) & 0x8000) {
        if (local_B6 < local_B5) {
          local_B6 = local_B6 + 1;
          local_BC = 0;
          local_B7 = 2;
        }
      }
      if (Script_GetControllerInput(1) == 0x400) {
        break;
      }
      local_B8 = local_B8 + 1;
      if (local_B8 > 1800) {
        break;
      }
      if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_EV_STOP_ACTIVE)) {
        break;
      }
    } else {
      if (local_B7 == 1) {
        local_F4.x = (&local_F4)->x - 0.013f;
        (&local_F4)->z = (&local_F4)->z - 0.013f;
        local_D0 = local_D0 + 0.004f;
        local_CC = local_CC + 0.004f;
        if (!(local_D0 <= 0.02f)) {
          local_D0 = 0.02f;
        }
        if (!(local_CC <= 0.02f)) {
          local_CC = 0.02f;
        }
      } else {
        local_F4.x = (&local_F4)->x + 0.013f;
        (&local_F4)->z = (&local_F4)->z + 0.013f;
        local_D0 = local_D0 - 0.004f;
        local_CC = local_CC - 0.004f;
        if (!(local_D0 >= -0.02f)) {
          local_D0 = -0.02f;
        }
        if (!(local_CC >= -0.02f)) {
          local_CC = -0.02f;
        }
      }
      local_BC = local_BC + 1;
      if (local_BC == local_B4) {
        local_B7 = 0;
      }
    }
    if (local_C0 < 50) {
      (&local_F4)->y = (&local_F4)->y + 0.00015f;
    } else {
      (&local_F4)->y = (&local_F4)->y - 0.00015f;
    }
    local_C0 = local_C0 + 1;
    if (local_C0 == 100) {
      local_C0 = 0;
    }
    if (!(local_D0 <= 0.001f)) {
      local_D0 = local_D0 - 0.002f;
    } else {
      if (!(local_D0 >= -0.001f)) {
        local_D0 = local_D0 + 0.002f;
        goto L018E0F;
      }
    }
    L018E0F:;
    if (!(local_CC <= 0.001f)) {
      local_CC = local_CC - 0.002f;
    } else {
      if (!(local_CC >= -0.001f)) {
        local_CC = local_CC + 0.002f;
        goto L018E45;
      }
    }
    L018E45:;
    Script_EVT_SetPosition(SHEN_NAME('URDM'), POS_XYZ, &local_F4);
    local_E8.x = (&local_F4)->x + local_D0;
    (&local_E8)->y = (&local_F4)->y;
    (&local_E8)->z = (&local_F4)->z + local_CC;
    Script_EVT_SetPosition(SHEN_NAME('YKUR'), POS_XYZ, &local_E8);
    Script_scenUtils(SCENARIO_CMD_SET_STEP_LUT, 7, task_2C0 * 10 + local_B6);
    SCNF_WaitFrames_0000(1);
  }
  Script_scenUtils(SCENARIO_CMD_SET_STEP_LUT, 7, 0);
  Script_sicnUtils(2, 4, 0);
  Script_shenClearFreeState(0);
  Script_uiUtils(0, 56, 0);
  script_19154();
  {
    int32_t local_AC;
    local_AC = task_2C0;
    switch (local_AC) {
      case 1:
        local_AC = local_B6;
        switch (local_AC) {
          case 0:
            local_B0 = 0x62726E63;
            Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 963, 1);
            Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 971, 0);
            Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 966, 0);
            break;
          case 1:
            local_B0 = 0x72726E63;
            Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 963, 0);
            Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 971, 1);
            Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 966, 0);
            break;
          case 2:
            local_B0 = 0x73726E63;
            Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 963, 0);
            Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 971, 0);
            Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, 966, 1);
            break;
        }
        break;
      case 2:
        local_AC = local_B6;
        switch (local_AC) {
          case 0:
            local_B0 = 0x70736E63;
            break;
          case 1:
            local_B0 = 0x6F686E63;
            break;
        }
        break;
      case 3:
        local_AC = local_B6;
        switch (local_AC) {
          case 0:
            local_B0 = 0x6C636E6D;
            task_2B8 = 10;
            break;
          case 1:
            local_B0 = 0x6E636E6D;
            task_2B8 = 11;
            break;
          case 2:
            local_B0 = 0x6F636E6D;
            task_2B8 = 12;
            break;
          case 3:
            local_B0 = 0x6D636E6D;
            task_2B8 = 13;
            break;
        }
        break;
      case 4:
        local_AC = local_B6;
        switch (local_AC) {
          case 0:
            local_BC = 0;
            break;
          case 1:
            local_BC = 1;
            break;
          case 2:
            local_BC = 3;
            break;
          case 3:
            local_BC = 2;
            break;
        }
        local_B0 = script_1924C(local_BC);
        break;
    }
  }
  Script_ShowUIMessageFromID("SA1114A011");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_SetCameraFlags(1);
  Script_charaDisplay(SHEN_NAME('YKUR'), CDM_Hide);
  Script_charaDisplay(SHEN_NAME('URDM'), CDM_Hide);
  Script_charaDisplay(CHARA_AKIR, CDM_Show);
  Script_scenToggleVisibleFor(SHEN_NAME('YKUR'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('YKUL'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('URDM'), 0);
  local_F4.x = -77.18f;
  (&local_F4)->y = 0.753f;
  (&local_F4)->z = 51.456001f;
  Script_EVT_SetPosition(0x74687364, POS_XYZ, &local_F4);
  return local_B0;
}

static void script_19154(void) {
  int32_t local_FC;
  Vector3i local_F0;
  Script_charaDisplay(SHEN_NAME('URDM'), CDM_Hide);
  local_F0.x = 17880;
  (&local_F0)->y = 6630;
  (&local_F0)->z = 32210;
  local_FC = 0;
  for (;;) {
    local_FC = local_FC + 1;
    local_F0.x = (&local_F0)->x + 200;
    Script_EVT_SetRotation(SHEN_NAME('YKUR'), 0x38000000, &local_F0, 0);
    SCNF_WaitFrames_0000(1);
    Script_charaDisplay(SHEN_NAME('YKUR'), CDM_Show);
    if (local_FC == 6) {
      break;
    }
  }
  for (;;) {
    local_FC = local_FC - 1;
    local_F0.x = (&local_F0)->x - 200;
    Script_EVT_SetRotation(SHEN_NAME('YKUR'), 0x38000000, &local_F0, 0);
    SCNF_WaitFrames_0000(1);
    Script_charaDisplay(SHEN_NAME('YKUR'), CDM_Show);
    if (local_FC == 0) {
      break;
    }
  }
  return;
}

static int32_t script_19213(void) {
  int32_t local_FC;
  int32_t local_F8;
  int32_t local_F4;
  float local_F0;
  script_13544((int32_t)&local_FC, (int32_t)&local_F8);
  local_F0 = 1.0f;
  script_135E6(0x6E68656B, local_FC, local_F8, local_F0);
  task_2B8 = local_F8;
  return local_F4 = 0x6E68656B;
}

static int32_t script_1924C(int32_t arg0) {
  int32_t local_FC;
  float local_F8;
  script_131B8(9);
  local_F8 = 1.0f;
  script_135E6(0x6E68656B, 9, arg0, local_F8);
  task_2B8 = arg0;
  return local_FC = 0x6E68656B;
}

static void script_19283(int32_t arg0) {
  Vector3 local_F4;
  Script_ResetHandMotion(CHARA_AKIR, 1, 0, 0);
  Script_ResetHandMotion(CHARA_AKIR, 1, 1, 0);
  Script_handSetFloatsFor(CHARA_AKIR, 1, data_20934, 10);
  Script_handSetFloatsFor(CHARA_AKIR, 0, data_20934, 10);
  local_F4.x = 0.081f;
  (&local_F4)->y = 0.005f;
  (&local_F4)->z = -0.046f;
  {
    Vector3i local_E8;
    local_E8.x = 25282;
    (&local_E8)->y = 3231;
    (&local_E8)->z = 14226;
    Script_fixoAttachEx(0x66676764, CHARA_AKIR, 18, &local_F4, &local_E8);
  }
  local_F4.x = 0.109f;
  (&local_F4)->y = 0.005f;
  (&local_F4)->z = 0.056f;
  {
    Vector3i local_E8;
    local_E8.x = 52428;
    (&local_E8)->y = 27306;
    (&local_E8)->z = 64807;
    Script_fixoAttachEx(0x67676764, CHARA_AKIR, 12, &local_F4, &local_E8);
  }
  local_F4.x = -77.571899f;
  (&local_F4)->y = 1.3481f;
  (&local_F4)->z = 51.390499f;
  Script_SetCharaLookAtPosition(CHARA_AKIR, 20, &local_F4, 0);
  Script_FaceLookAtTargetUtils(CHARA_AKIR, 2, &local_F4, 1);
  Script_seqEnterEvtCam(9130, 0, AUTH_None);
  Script_PlayCharaMotion(CHARA_AKIR, 28679, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_SetCharaMotionPlaybackRate(CHARA_AKIR, 2.0f);
  {
    Vector3i local_E8;
    int32_t local_E4;
    local_E4 = 0;
    for (;;) {
      int32_t local_E0;
      local_E0 = local_E4;
      switch (local_E0) {
        case 80:
          Script_executeAicaCommand(656553, 0, 0);
          break;
        case 190:
          Script_fixoClearAttachment(0x66676764);
          script_9EB2(0x66676764, 0.0f, -1000.0f, 0.0f);
          local_F4.x = 0.118f;
          (&local_F4)->y = 0.02f;
          (&local_F4)->z = -0.062f;
          local_E8.x = 59708;
          (&local_E8)->y = 43506;
          (&local_E8)->z = 63619;
          Script_fixoAttachEx(arg0, CHARA_AKIR, 18, &local_F4, &local_E8);
          Script_charaDisplay(arg0, CDM_Show);
          break;
        case 230:
          Script_EVT_GetPosition(arg0, -1, &local_F4, 0);
          Script_FaceLookAtTargetUtils(CHARA_AKIR, 2, &local_F4, 10);
          break;
        case 240:
          Script_fixoClearAttachment(0x67676764);
          script_9EB2(0x67676764, 0.0f, -1000.0f, 0.0f);
          break;
      }
      if (Script_IsCharaMotionFinished(CHARA_AKIR)) {
        break;
      }
      SCNF_WaitFrames_0000(1);
      local_E4 = local_E4 + 2;
    }
  }
  {
    int32_t unused_E4[4];
    Vector3 local_D8;
    if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, 0x52494B41) & Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, 0x5359454D)) {
      Script_EVT_GetPosition(CHARA_MEYS, 4, &local_D8, ENTITY_XFORM_WORLD_SPACE);
      Script_SetLookPointDetails(CHARA_AKIR, 20, &local_D8, 2);
      Script_FaceLookAtTargetUtils(CHARA_AKIR, 2, &local_D8, 0);
    }
    if (Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, 0x5359454D) & Script_CycleManagerUtils(CYCLE_CMD_CHECK_SCNF_HAS_CVWK, 0x52494B41)) {
      Script_EVT_GetPosition(CHARA_AKIR, 4, &local_D8, ENTITY_XFORM_WORLD_SPACE);
      Script_SetLookPointDetails(CHARA_MEYS, 20, &local_D8, 2);
      Script_FaceLookAtTargetUtils(CHARA_MEYS, 2, &local_D8, 0);
    }
  }
  Script_seqEnterEvtCam(9140, CHARA_AKIR, CHARA_AKIR);
  SCNF_WaitFrames_0000(60);
  Script_fixoClearAttachment(arg0);
  script_9EB2(arg0, 0.0f, -1000.0f, 0.0f);
  Script_handSetFloatsFor(CHARA_AKIR, 1, data_20A18, 10);
  Script_handSetFloatsFor(CHARA_AKIR, 0, data_20A18, 10);
  return;
}

static void script_19605(void) {
  Vector3 local_F4;
  Vector3 local_E8;
  Vector3i local_DC;
  int32_t local_D8;
  int32_t local_D4;
  int32_t local_D0;
  Script_EVT_GetPosition(CHARA_AKIR, -1, &local_E8, ENTITY_XFORM_WORLD_SPACE);
  Script_mtwkGetSetRotation(CHARA_AKIR, &local_DC, 0x42000000);
  local_F4.x = (&local_E8)->x + Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, (&local_DC)->y) * 0.3f;
  (&local_F4)->y = 0.0f;
  (&local_F4)->z = (&local_E8)->z + Script_MathUtils(SCRIPT_MATH_COS_FIXED16, (&local_DC)->y) * 0.3f;
  if (Script_CheckTargetWithinAngleRange(&data_20D68, 1.8f, 0x6000, 12743, &local_E8) & Script_CheckTargetWithinAngleRange(&local_F4, 1.7f, (&local_DC)->y, 7281, &data_20D68)) {
    Script_SetOwnedItemMenuFlagIfOwned(1, OwnedItem_UIITEMWINN, 1);
  } else {
    Script_SetOwnedItemMenuFlagIfOwned(1, OwnedItem_UIITEMWINN, 0);
  }
  local_D8 = Script_cassetteUtils();
  local_D4 = (local_D8 & -0x10000) >> 16;
  local_D0 = local_D8 & 0xFFFF;
  if (local_D0 == 2) {
    if (local_D4 == 66) {
      script_196F5();
    }
  }
  return;
}

static void script_196F5(void) {
  script_1B239();
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_FaceEx(CHARA_AKIR, 1, 0, 2);
  Script_ResetHandMotion(CHARA_AKIR, -1, 0, 0);
  Script_ResetHandMotion(CHARA_AKIR, -1, 1, 0);
  Script_shenClearFreeState(0);
  Script_SetShadowFlags(CHARA_AKIR, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 0);
  Script_PlayerStatsUtils(PS_SetCharaItemCount, 0x5359454D, Script_PlayerStatsUtils(PS_GetCharaItemCount, 0x5359454D) + 1);
  Script_motmToggleFlags(CHARA_AKIR, 2, 0x40);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_SetCPAMFlags(CHARA_MEYS, 0);
  Script_ccowUtils(CHARA_AKIR, 2);
  Script_InitializeOsag(CHARA_AKIR);
  Script_scenToggleVisibleFor(SHEN_NAME('YKUR'), 1);
  Script_scenToggleVisibleFor(SHEN_NAME('YKUL'), 1);
  Script_facemotUtils(CHARA_AKIR, 1, 0);
  Script_ResetHandMotion(CHARA_AKIR, 1, 1, 0);
  Script_ResetHandMotion(CHARA_AKIR, 1, 0, 0);
  task_2C0 = -1;
  script_1999E();
  script_17A7E();
  if (task_2C0 != 6) {
    script_181E3();
    SCNF_WaitFrames_0000(30);
  } else {
    script_9EB2(0x78626B64, -0.04f, 0.74f, -0.9f);
    script_9EDA(0x78626B64, 0.0f, 0.0f, 0.0f);
    Script_charaDisplay(0x78626B64, CDM_Show);
  }
  Script_SetCharaLookAtPosition(CHARA_AKIR, -20, 0, 0);
  Script_SetCharaLookAtPosition(CHARA_MEYS, -20, 0, 0);
  script_9EB2(CHARA_AKIR, -78.289001f, 0.0f, 51.922901f);
  Script_InitializeOsag(CHARA_AKIR);
  Script_motmToggleFlags(CHARA_AKIR, 2, 0x20);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
  Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
  Script_ccowUtils(CHARA_AKIR, 1);
  Script_SetCPAMFlags(CHARA_MEYS, 1);
  Script_SetCPAMFlags(CHARA_AKIR, 1);
  Script_SetCharaLookAtPosition(CHARA_AKIR, -20, 0, 0);
  Script_SetCharaLookAtPosition(CHARA_MEYS, -20, 0, 0);
  Script_convSetCamInactiveFlag(1);
  script_1B318();
  Script_SetShadowFlags(CHARA_AKIR, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 4);
  Script_srchUtils(SRCH_SCRIPT_Reset);
  Script_SetGameStateTo(AUTH_None);
  SCNF_WaitFrames_0000(18);
  SCNF_WaitFrames_0000(1);
  Script_ryoControl(RYO_RestoreControl);
  Script_FaceEx(CHARA_AKIR, 0, 0, 2);
  Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
  Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
  Script_shenClearFreeState(1);
  return;
}

__attribute__((scn_string_bindings(0, data_20D8C, 1, data_20D97, 2, data_20DA2, 3, data_20DAD)))
static void script_1999E(void) {
  int32_t local_FC;
  int32_t unused_F0[3];
  Vector3 local_E4;
  int32_t local_E0;
  Script_filmCheckOrSetTransitionUtil(2, 0, 0, 0, 0, 255, 0, 0, 0);
  SCNF_WaitFrames_0000(1);
  Script_SetCPAMFlags(CHARA_AKIR, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  script_9EB2(CHARA_MEYS, -77.1f, 0.0f, 50.6f);
  script_9EDA(CHARA_MEYS, 0.0f, 143.0f, 0.0f);
  script_9EB2(CHARA_AKIR, -77.892998f, 0.0f, 51.616001f);
  script_9EDA(CHARA_AKIR, 0.0f, 324.0f, 0.0f);
  Script_InitializeOsag(CHARA_AKIR);
  Script_SetHandRootRotation(CHARA_AKIR, 1, 0x15, &data_20D74);
  Script_handSetFloatsFor(CHARA_AKIR, 1, data_20934, 0);
  Script_handSetFloatsFor(CHARA_AKIR, 0, data_20934, 0);
  Script_seqEnterEvtCam(9010, 0, AUTH_None);
  Script_filmCheckOrSetTransitionUtil(5, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_EVT_GetPosition(CHARA_AKIR, 4, &local_E4, ENTITY_XFORM_WORLD_SPACE);
  Script_SetLookPointDetails(CHARA_MEYS, 16, &local_E4, 0);
  Script_ShowUIMessageFromID("SA1114B001");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  local_FC = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f));
  local_E0 = local_FC;
  switch (local_E0) {
    case 0:
      Script_ShowUIMessageFromID("SA1114A007");
      break;
    case 1:
      Script_ShowUIMessageFromID("SA1114A009");
      break;
  }
  script_9EB2(CHARA_AKIR, -78.039803f, 0.0f, 51.527f);
  script_9EDA(CHARA_AKIR, 0.0f, 328.0f, 0.0f);
  Script_InitializeOsag(CHARA_AKIR);
  Script_motmToggleFlags(CHARA_AKIR, 2, 0x40);
  Script_motmToggleFlags(CHARA_AKIR, 1, 0x20);
  Script_PlayCharaMotion(CHARA_AKIR, 28673, 10.0f, 29.0f, -1.0f, 1.0f, 0);
  script_9EB2(0x6B746164, -77.6f, 0.812f, 51.15f);
  script_9EDA(0x6B746164, 180.0f, 180.0f, 180.0f);
  Script_charaDisplay(0x6B746164, CDM_Show);
  Script_seqEnterEvtCam(9030, 0, AUTH_None);
  SCNF_WaitFrames_0000(60);
  Script_motmToggleFlags(CHARA_AKIR, 2, 0x20);
  Script_motmToggleFlags(CHARA_AKIR, 2, 0x40);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_SetHandRootRotation(CHARA_AKIR, 1, 0x15, &data_20D80);
  Script_handSetFloatsFor(CHARA_AKIR, 1, data_20A18, 10);
  script_9EB2(CHARA_AKIR, -77.892998f, 0.0f, 51.616001f);
  script_9EDA(CHARA_AKIR, 0.0f, 315.0f, 0.0f);
  Script_InitializeOsag(CHARA_AKIR);
  SCNF_WaitFrames_0000(1);
  script_9EB2(0x6B746164, 0.0f, -1000.0f, 0.0f);
  Script_InitializeOsag(CHARA_AKIR);
  Script_seqEnterEvtCam(9066, CHARA_AKIR, CHARA_AKIR);
  Script_ShowUIMessageFromID("SA1114B004");
  while (Script_convGetIsActive(0)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_GetCassetteFlags_0(66, 1, 0);
  SCNF_WaitFrames_0000(10);
  return;
}

static void script_19CFF(void) {
  int32_t unused_FC;
  int32_t local_F8;
  if (task_2C0 == 1) {
    Script_seqEnterEvtCam(9070, CHARA_AKIR, CHARA_AKIR);
  } else {
    Script_seqEnterEvtCam(9075, CHARA_AKIR, CHARA_AKIR);
  }
  local_F8 = task_2C0;
  if ((local_F8 == 0) | (local_F8 == 1) | (local_F8 == 2) | (local_F8 == 3) | (local_F8 == 4)) {
    Script_ShowUIMessageFromID("SA1114B009");
  } else {
    if (local_F8 == 5) {
      Script_ShowUIMessageFromID("SA1114B010");
      goto L019D79;
    }
  }
  L019D79:;
  return;
}

static void script_19D7A(void) {
  int32_t local_FC;
  int32_t local_F8;
  local_FC = Script_FloatUtils(ScriptMathOp_FloatToInt, Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 3.0f));
  local_F8 = local_FC;
  switch (local_F8) {
    case 0:
      Script_ShowUIMessageFromID("SA1114B011");
      while (Script_convGetIsActive(0)) {
        SCNF_WaitFrames_0000(1);
      }
      break;
    case 1:
      Script_ShowUIMessageFromID("SA1114B012");
      while (Script_convGetIsActive(0)) {
        SCNF_WaitFrames_0000(1);
      }
      break;
    case 2:
      Script_ShowUIMessageFromID("SA1114B013");
      while (Script_convGetIsActive(0)) {
        SCNF_WaitFrames_0000(1);
      }
      break;
  }
  return;
}

static void script_19E18(void) {
  int32_t unused_FC;
  int32_t local_F8;
  local_F8 = task_2C0;
  if ((local_F8 == 0) | (local_F8 == 1) | (local_F8 == 2) | (local_F8 == 3) | (local_F8 == 4)) {
    Script_ShowUIMessageFromID("SA1114A014");
  } else {
    switch (local_F8) {
      case 5:
        Script_ShowUIMessageFromID("SA1114A012");
        break;
      case 6:
        Script_ShowUIMessageFromID("SA1114A013");
        break;
    }
  }
  return;
}

static void script_19E79(void) {
  int32_t unused_FC;
  int32_t local_F8;
  local_F8 = task_2C0;
  if ((local_F8 == 0) | (local_F8 == 1)) {
    Script_ShowUIMessageFromID("SA1114B005");
  } else if ((local_F8 == 2) | (local_F8 == 3) | (local_F8 == 4)) {
    Script_ShowUIMessageFromID("SA1114B007");
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
    Script_ShowUIMessageFromID("SA1114B008");
  } else {
    switch (local_F8) {
      case 5:
        break;
      case 6:
        break;
    }
  }
  return;
}

static int32_t script_19EF5(void) {
  int32_t local_FC;
  Script_InvestigationUtils(ShenmueUtils_SetupOrSyncAnchors, SHEN_NAME('YKUL'), 0x52554B59);
  script_16948();
  for (;;) {
    script_1696C();
    local_FC = task_2A8;
    if ((local_FC == 0x31484347) | (local_FC == 0x34484347) | (local_FC == 0x35484347) | (local_FC == 0x30484347) | (local_FC == 0x32484347) | (local_FC == 0x33484347)) {
      task_260 = task_2A8;
      script_15DAF();
      goto L019F62;
    }
    L019F62:;
    if (Script_IsPlayerAllowed(0)) {
      script_19605();
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_19F7D(int32_t arg0, int32_t arg1) {
  if (arg1 == 5) {
    int32_t unused_FC;
    int32_t local_F8;
    script_1BEF4();
    local_F8 = arg0;
    switch (local_F8) {
      case 0x6B746E63:
        SCNT_ShowSubtitle("[[[UISRCHMKYU22]]]");
        break;
      case 0x706E6E63:
        SCNT_ShowSubtitle("[[[UISRCHMKYU21]]]");
        break;
      case 0x706D6E63:
        SCNT_ShowSubtitle("[[[UISRCHMKYU20]]]");
        break;
      case 0x6B6D6E63:
        SCNT_ShowSubtitle("[[[UISRCHMKYU19]]]");
        break;
      case 0x31626E63:
        SCNT_ShowSubtitle("[[[UISRCHMKYU17]]]");
        break;
      case 0x33626E63:
        SCNT_ShowSubtitle("[[[UISRCHMKYU18]]]");
        break;
      case 0x31636E6D:
        SCNT_ShowSubtitle("[[[UISRCHMKYU23]]]");
        break;
      case 0x32636E6D:
        SCNT_ShowSubtitle("[[[UISRCHMKYU24]]]");
        break;
      case 0x33636E6D:
        SCNT_ShowSubtitle("[[[UISRCHMKYU25]]]");
        break;
      case 0x34636E6D:
        SCNT_ShowSubtitle("[[[UISRCHMKYU26]]]");
        break;
      case 0x35636E6D:
        SCNT_ShowSubtitle("[[[UISRCHMKYU27]]]");
        break;
      case 0x36636E6D:
        SCNT_ShowSubtitle("[[[UISRCHMKYU28]]]");
        break;
      case 0x37636E6D:
        SCNT_ShowSubtitle("[[[UISRCHMKYU29]]]");
        break;
      case 0x38636E6D:
        SCNT_ShowSubtitle("[[[UISRCHMKYU30]]]");
        break;
      case 0x39636E6D:
        SCNT_ShowSubtitle("[[[UISRCHMKYU31]]]");
        break;
      case 0x61636E6D:
        SCNT_ShowSubtitle("[[[UISRCHMKYU32]]]");
        break;
      case 0x62636E6D:
        SCNT_ShowSubtitle("[[[UISRCHMKYU33]]]");
        break;
      case 0x63636E6D:
        SCNT_ShowSubtitle("[[[UISRCHMKYU34]]]");
        break;
      case 0x64636E6D:
        SCNT_ShowSubtitle("[[[UISRCHMKYU35]]]");
        break;
      case 0x65636E6D:
        SCNT_ShowSubtitle("[[[UISRCHMKYU36]]]");
        break;
      case 0x66636E6D:
        SCNT_ShowSubtitle("[[[UISRCHMKYU37]]]");
        break;
      case 0x67636E6D:
        SCNT_ShowSubtitle("[[[UISRCHMKYU38]]]");
        break;
      case 0x68636E6D:
        SCNT_ShowSubtitle("[[[UISRCHMKYU39]]]");
        break;
      case 0x69636E6D:
        SCNT_ShowSubtitle("[[[UISRCHMKYU40]]]");
        break;
      case 0x6A636E6D:
        SCNT_ShowSubtitle("[[[UISRCHMKYU41]]]");
        break;
      case 0x6B636E6D:
        SCNT_ShowSubtitle("[[[UISRCHMKYU42]]]");
        break;
      case 0x31736E63:
        SCNT_ShowSubtitle("[[[UISRCHMKYU14]]]");
        break;
      case 0x63736E63:
        SCNT_ShowSubtitle("[[[UISRCHMKYU16]]]");
        break;
      case 0x6B736E63:
        SCNT_ShowSubtitle("[[[UISRCHMKYU15]]]");
        break;
    }
  }
  if (arg1 == 3) {
    float local_FC;
    local_FC = Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 4.0f);
    if (!(local_FC >= 1.0f)) {
      Script_ShowUIMessageFromID("SA1114A003");
    } else if (!(local_FC >= 2.0f)) {
      Script_ShowUIMessageFromID("SA1114A004");
    } else if (!(local_FC >= 3.0f)) {
      Script_ShowUIMessageFromID("SA1114A005");
    } else {
      Script_ShowUIMessageFromID("SA1114A006");
    }
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
  }
  if (arg1 == 2) {
    float local_FC;
    local_FC = Script_MathUtils(SCRIPT_MATH_RANDOM_FLOAT_RANGE, 2.0f);
    if (!(local_FC >= 1.0f)) {
      Script_ShowUIMessageFromID("SA1114A001");
    } else {
      Script_ShowUIMessageFromID("SA1114A002");
    }
    while (Script_convGetIsActive(0)) {
      SCNF_WaitFrames_0000(1);
    }
  }
  end:;
}

static void script_1A2A6(int32_t arg0) {
  int8_t local_FF;
  uint8_t unused_F8[7];
  script_1B239();
  Script_SetShadowFlags(CHARA_AKIR, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 0);
  Script_ryoControl(PCCTL_DisablePlayerControl);
  Script_FaceEx(CHARA_AKIR, 1, 0, 2);
  Script_ResetHandMotion(CHARA_AKIR, -1, 0, 0);
  Script_ResetHandMotion(CHARA_AKIR, -1, 1, 0);
  Script_shenClearFreeState(0);
  Script_PlayerStatsUtils(PS_SetCharaItemCount, 0x5359454D, Script_PlayerStatsUtils(PS_GetCharaItemCount, 0x5359454D) + 1);
  Script_motmToggleFlags(CHARA_AKIR, 2, 0x40);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_SetCPAMFlags(CHARA_MEYS, 0);
  Script_ccowUtils(CHARA_AKIR, 2);
  Script_InitializeOsag(CHARA_AKIR);
  Script_scenToggleVisibleFor(SHEN_NAME('YKUR'), 1);
  Script_scenToggleVisibleFor(SHEN_NAME('YKUL'), 1);
  Script_facemotUtils(CHARA_AKIR, 1, 0);
  Script_ResetHandMotion(CHARA_AKIR, 1, 1, 0);
  Script_ResetHandMotion(CHARA_AKIR, 1, 0, 0);
  local_FF = script_170B6(arg0);
  if (local_FF == 1) {
    script_17A7E();
    if (task_2C0 != 6) {
      script_181E3();
      SCNF_WaitFrames_0000(30);
    } else {
      script_9EB2(0x78626B64, -76.998001f, 0.75f, 51.813999f);
      script_9EDA(0x78626B64, 0.0f, 315.0f, 0.0f);
      Script_charaDisplay(0x78626B64, CDM_Show);
    }
  }
  script_9EB2(CHARA_AKIR, -78.289001f, 0.0f, 51.922901f);
  Script_InitializeOsag(CHARA_AKIR);
  Script_motmToggleFlags(CHARA_MEYS, 2, 0x20);
  Script_motmToggleFlags(CHARA_MEYS, 2, 0x40);
  Script_motmToggleFlags(CHARA_MEYS, 2, 0x80);
  Script_motmToggleFlags(CHARA_AKIR, 2, 0x20);
  Script_PlayCharaMotion(CHARA_AKIR, MN_AKI_AKI_TATI_IWA_L, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
  Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
  Script_ccowUtils(CHARA_AKIR, 1);
  Script_SetCPAMFlags(CHARA_MEYS, 1);
  Script_SetCharaLookAtPosition(CHARA_AKIR, -20, 0, 0);
  Script_SetCharaLookAtPosition(CHARA_MEYS, -20, 0, 0);
  Script_FaceLookAtTargetUtils(CHARA_AKIR, 0);
  Script_convSetCamInactiveFlag(1);
  script_1B318();
  Script_SetShadowFlags(CHARA_AKIR, SCRIPT_SHADOW_SET_LIGHT_MASK_BIT_COUNT, 4);
  Script_SetGameStateTo(AUTH_None);
  SCNF_WaitFrames_0000(18);
  SCNF_WaitFrames_0000(1);
  if (Script_scenUtils(SCENARIO_CMD_GET_SHENMUE_FLAG, SHENMUE_FLAG_EV_STOP_ACTIVE)) {
    Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_EV_STOP_ACTIVE, 0);
  }
  Script_ryoControl(RYO_RestoreControl);
  Script_FaceEx(CHARA_AKIR, 0, 0, 2);
  Script_ResetHandMotion(CHARA_AKIR, 0, 0, 0);
  Script_ResetHandMotion(CHARA_AKIR, 0, 1, 0);
  Script_shenClearFreeState(1);
  return;
}

static void script_1A577(void) {
  int32_t unused_FC;
  script_9EB2(0x6B746E63, -74.3f, 0.645f, 51.665001f);
  script_9EDA(0x6B746E63, 0.0f, 317.0f, 0.0f);
  Script_charaDisplay(0x6B746E63, CDM_Show);
  script_9EB2(0x706E6E63, -74.626999f, 0.73f, 51.345001f);
  script_9EDA(0x706E6E63, 340.0f, 315.0f, 0.0f);
  Script_charaDisplay(0x706E6E63, CDM_Show);
  script_9EB2(0x706D6E63, -75.05f, 1.1f, 50.94f);
  script_9EDA(0x706D6E63, 340.0f, 315.0f, 0.0f);
  Script_charaDisplay(0x706D6E63, CDM_Show);
  script_9EB2(0x6B6D6E63, -75.08f, 0.8f, 51.0f);
  script_9EDA(0x6B6D6E63, 0.0f, 315.0f, 0.0f);
  Script_charaDisplay(0x6B6D6E63, CDM_Show);
  script_9EB2(0x31626E63, -76.5f, 0.817f, 51.1f);
  script_9EDA(0x31626E63, 270.0f, 45.0f, 0.0f);
  Script_charaDisplay(0x31626E63, CDM_Show);
  script_9EB2(0x33626E63, -76.72f, 0.805f, 51.277f);
  script_9EDA(0x33626E63, 270.0f, 45.0f, 0.0f);
  Script_charaDisplay(0x33626E63, CDM_Show);
  script_9EB2(0x31736E63, -74.65f, 1.14f, 51.35f);
  script_9EDA(0x31736E63, 180.0f, 225.0f, 180.0f);
  Script_charaDisplay(0x31736E63, CDM_Show);
  script_9EB2(0x63736E63, -74.34f, 1.11f, 51.55f);
  script_9EDA(0x63736E63, 90.0f, 225.0f, 180.0f);
  Script_charaDisplay(0x63736E63, CDM_Show);
  script_9EB2(0x6B736E63, -74.15f, 1.115f, 51.862f);
  script_9EDA(0x6B736E63, 270.0f, 315.0f, 0.0f);
  Script_charaDisplay(0x6B736E63, CDM_Show);
  script_9EB2(0x31636E6D, 0.0f, 0.0f, -1000.0f);
  script_9EB2(0x32636E6D, 0.0f, 0.0f, -1000.0f);
  script_9EB2(0x33636E6D, 0.0f, 0.0f, -1000.0f);
  script_9EB2(0x34636E6D, 0.0f, 0.0f, -1000.0f);
  script_9EB2(0x35636E6D, 0.0f, 0.0f, -1000.0f);
  script_9EB2(0x36636E6D, 0.0f, 0.0f, -1000.0f);
  script_9EB2(0x37636E6D, 0.0f, 0.0f, -1000.0f);
  script_9EB2(0x38636E6D, 0.0f, 0.0f, -1000.0f);
  script_9EB2(0x39636E6D, 0.0f, 0.0f, -1000.0f);
  script_9EB2(0x61636E6D, 0.0f, 0.0f, -1000.0f);
  script_9EB2(0x62636E6D, 0.0f, 0.0f, -1000.0f);
  script_9EB2(0x63636E6D, 0.0f, 0.0f, -1000.0f);
  script_9EB2(0x64636E6D, 0.0f, 0.0f, -1000.0f);
  script_9EB2(0x65636E6D, 0.0f, 0.0f, -1000.0f);
  script_9EB2(0x66636E6D, 0.0f, 0.0f, -1000.0f);
  script_9EB2(0x67636E6D, 0.0f, 0.0f, -1000.0f);
  script_9EB2(0x68636E6D, 0.0f, 0.0f, -1000.0f);
  script_9EB2(0x69636E6D, 0.0f, 0.0f, -1000.0f);
  script_9EB2(0x6A636E6D, 0.0f, 0.0f, -1000.0f);
  script_9EB2(0x6B636E6D, 0.0f, 0.0f, -1000.0f);
  switch (task_2D0) {
    case 0:
      script_9EB2(0x31636E6D, -74.7f, 1.503f, 51.34f);
      script_9EDA(0x31636E6D, 0.0f, 315.0f, 0.0f);
      Script_charaDisplay(0x31636E6D, CDM_Show);
      script_9EB2(0x32636E6D, -74.52f, 1.503f, 51.53f);
      script_9EDA(0x32636E6D, 0.0f, 315.0f, 0.0f);
      Script_charaDisplay(0x32636E6D, CDM_Show);
      script_9EB2(0x33636E6D, -74.36f, 1.554f, 51.69f);
      script_9EDA(0x33636E6D, 45.0f, 0.0f, 270.0f);
      Script_charaDisplay(0x33636E6D, CDM_Show);
      script_9EB2(0x34636E6D, -74.21f, 1.503f, 51.84f);
      script_9EDA(0x34636E6D, 0.0f, 315.0f, 0.0f);
      Script_charaDisplay(0x34636E6D, CDM_Show);
      break;
    case 1:
      script_9EB2(0x35636E6D, -74.7f, 1.503f, 51.34f);
      script_9EDA(0x35636E6D, 0.0f, 315.0f, 0.0f);
      Script_charaDisplay(0x35636E6D, CDM_Show);
      script_9EB2(0x36636E6D, -74.52f, 1.503f, 51.53f);
      script_9EDA(0x36636E6D, 0.0f, 315.0f, 0.0f);
      Script_charaDisplay(0x36636E6D, CDM_Show);
      script_9EB2(0x37636E6D, -74.36f, 1.554f, 51.69f);
      script_9EDA(0x37636E6D, 45.0f, 0.0f, 270.0f);
      Script_charaDisplay(0x37636E6D, CDM_Show);
      script_9EB2(0x38636E6D, -74.21f, 1.503f, 51.84f);
      script_9EDA(0x38636E6D, 0.0f, 315.0f, 0.0f);
      Script_charaDisplay(0x38636E6D, CDM_Show);
      break;
    case 2:
      script_9EB2(0x39636E6D, -74.7f, 1.554f, 51.34f);
      script_9EDA(0x39636E6D, 45.0f, 0.0f, 270.0f);
      Script_charaDisplay(0x39636E6D, CDM_Show);
      script_9EB2(0x61636E6D, -74.52f, 1.503f, 51.53f);
      script_9EDA(0x61636E6D, 0.0f, 315.0f, 0.0f);
      Script_charaDisplay(0x61636E6D, CDM_Show);
      script_9EB2(0x62636E6D, -74.36f, 1.503f, 51.69f);
      script_9EDA(0x62636E6D, 0.0f, 315.0f, 0.0f);
      Script_charaDisplay(0x62636E6D, CDM_Show);
      script_9EB2(0x63636E6D, -74.21f, 1.503f, 51.84f);
      script_9EDA(0x63636E6D, 0.0f, 315.0f, 0.0f);
      Script_charaDisplay(0x63636E6D, CDM_Show);
      break;
    case 3:
      script_9EB2(0x64636E6D, -74.7f, 1.554f, 51.34f);
      script_9EDA(0x64636E6D, 45.0f, 0.0f, 270.0f);
      Script_charaDisplay(0x64636E6D, CDM_Show);
      script_9EB2(0x65636E6D, -74.52f, 1.503f, 51.53f);
      script_9EDA(0x65636E6D, 0.0f, 315.0f, 0.0f);
      Script_charaDisplay(0x65636E6D, CDM_Show);
      script_9EB2(0x66636E6D, -74.36f, 1.554f, 51.69f);
      script_9EDA(0x66636E6D, 45.0f, 0.0f, 270.0f);
      Script_charaDisplay(0x66636E6D, CDM_Show);
      script_9EB2(0x67636E6D, -74.21f, 1.503f, 51.84f);
      script_9EDA(0x67636E6D, 0.0f, 315.0f, 0.0f);
      Script_charaDisplay(0x67636E6D, CDM_Show);
      break;
    case 4:
      script_9EB2(0x68636E6D, -74.7f, 1.503f, 51.34f);
      script_9EDA(0x68636E6D, 0.0f, 315.0f, 0.0f);
      Script_charaDisplay(0x68636E6D, CDM_Show);
      script_9EB2(0x69636E6D, -74.52f, 1.503f, 51.54f);
      script_9EDA(0x69636E6D, 0.0f, 315.0f, 0.0f);
      Script_charaDisplay(0x69636E6D, CDM_Show);
      script_9EB2(0x6A636E6D, -74.36f, 1.554f, 51.69f);
      script_9EDA(0x6A636E6D, 45.0f, 0.0f, 270.0f);
      Script_charaDisplay(0x6A636E6D, CDM_Show);
      script_9EB2(0x6B636E6D, -74.21f, 1.554f, 51.84f);
      script_9EDA(0x6B636E6D, 45.0f, 0.0f, 270.0f);
      Script_charaDisplay(0x6B636E6D, CDM_Show);
      break;
  }
  return;
}

static void script_1AE3E(void) {
  int32_t unused_FC;
  int32_t local_F8;
  local_F8 = task_2A8;
  if ((local_F8 == 0x70736E63) | (local_F8 == 0x6F686E63)) {
    script_9EB2(0x6F686E63, -77.6f, 0.773f, 51.15f);
    script_9EDA(0x6F686E63, 0.0f, 0.0f, 0.0f);
    Script_charaDisplay(0x6F686E63, CDM_Show);
  } else if (local_F8 == 0x6B746E63) {
    script_9EB2(0x6B746E63, -77.6f, 0.773f, 51.15f);
    script_9EDA(0x6B746E63, 0.0f, 315.0f, 0.0f);
    Script_charaDisplay(0x6B746E63, CDM_Show);
  } else if (local_F8 == 0x706E6E63) {
    script_9EB2(0x706E6E63, -77.6f, 0.77f, 51.15f);
    script_9EDA(0x706E6E63, 270.0f, 315.0f, 0.0f);
    Script_charaDisplay(0x706E6E63, CDM_Show);
  } else if (local_F8 == 0x706D6E63) {
    script_9EB2(0x706D6E63, -77.6f, 0.763f, 51.15f);
    script_9EDA(0x706D6E63, 270.0f, 315.0f, 0.0f);
    Script_charaDisplay(0x706D6E63, CDM_Show);
  } else if (local_F8 == 0x6B6D6E63) {
    script_9EB2(0x6B6D6E63, -77.6f, 0.832f, 51.15f);
    script_9EDA(0x6B6D6E63, 0.0f, 315.0f, 0.0f);
    Script_charaDisplay(0x6B6D6E63, CDM_Show);
  } else if (local_F8 == 0x31626E63) {
    script_9EB2(0x31626E63, -77.6f, 0.77f, 51.15f);
    script_9EDA(0x31626E63, 270.0f, 45.0f, 0.0f);
    Script_charaDisplay(0x31626E63, CDM_Show);
  } else if (local_F8 == 0x33626E63) {
    script_9EB2(0x33626E63, -77.6f, 0.77f, 51.15f);
    script_9EDA(0x33626E63, 270.0f, 45.0f, 0.0f);
    Script_charaDisplay(0x33626E63, CDM_Show);
  } else if ((local_F8 == 0x31636E6D) | (local_F8 == 0x32636E6D) | (local_F8 == 0x33636E6D) | (local_F8 == 0x34636E6D) | (local_F8 == 0x35636E6D) | (local_F8 == 0x36636E6D) | (local_F8 == 0x37636E6D) | (local_F8 == 0x38636E6D) | (local_F8 == 0x39636E6D) | (local_F8 == 0x61636E6D) | (local_F8 == 0x62636E6D) | (local_F8 == 0x63636E6D) | (local_F8 == 0x64636E6D) | (local_F8 == 0x65636E6D) | (local_F8 == 0x66636E6D) | (local_F8 == 0x67636E6D) | (local_F8 == 0x68636E6D) | (local_F8 == 0x69636E6D) | (local_F8 == 0x6A636E6D) | (local_F8 == 0x6B636E6D)) {
    script_9EB2(task_2A8, -77.6f, 0.757f, 51.15f);
    script_9EDA(task_2A8, 270.0f, 135.0f, 0.0f);
    Script_charaDisplay(task_2A8, CDM_Show);
  } else {
    switch (local_F8) {
      case 0x31736E63:
        script_9EB2(0x31736E63, -77.584f, 0.773f, 51.144001f);
        script_9EDA(0x31736E63, 270.0f, 315.0f, 0.0f);
        Script_charaDisplay(0x31736E63, CDM_Show);
        break;
      case 0x63736E63:
        script_9EB2(0x63736E63, -77.6f, 0.773f, 51.15f);
        script_9EDA(0x63736E63, 270.0f, 315.0f, 0.0f);
        Script_charaDisplay(0x63736E63, CDM_Show);
        break;
      case 0x6B736E63:
        script_9EB2(0x6B736E63, -77.6f, 0.773f, 51.15f);
        script_9EDA(0x6B736E63, 270.0f, 45.0f, 0.0f);
        Script_charaDisplay(0x6B736E63, CDM_Show);
        break;
    }
  }
  return;
}

static void script_1B239(void) {
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
  Script_CycleManagerUtils(CYCLE_CMD_PROCESS_SND_EXTRA);
  Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT, 0x5359454D, 1);
  task_2D4 = Script_CycleManagerUtils(CYCLE_CMD_GET_CYCLE_PTR_BY_CHARA_ID, 0x5359454D);
  while (task_2D4 == 0) {
    SCNF_WaitFrames_0000(1);
    task_2D4 = Script_CycleManagerUtils(CYCLE_CMD_GET_CYCLE_PTR_BY_CHARA_ID, 0x5359454D);
  }
  while (Script_CycleManagerUtils(CYCLE_CMD_GET_NEXT_TASK_POINTER, task_2D4) == 0) {
    SCNF_WaitFrames_0000(1);
  }
  Script_CycleManagerUtils(CYCLE_CMD_SET_RUNNING_PROGRAM_FIELD_B8, 0x5359454D, 0);
  script_9EB2(CHARA_MEYS, -77.1f, 0.0f, 50.6f);
  script_9EDA(CHARA_MEYS, 0.0f, 135.0f, 0.0f);
  Script_PlayCharaMotion(CHARA_MEYS, 28681, -1.0f, -1.0f, -1.0f, 1.0f, 0);
  Script_charaDisplay(CHARA_MEYS, CDM_Show);
  return;
}

static void script_1B318(void) {
  if (-1) {
    Script_charaDisplay(CHARA_MEYS, CDM_Hide);
  } else {
    if (0) {
      Script_charaDisplay(CHARA_MEYS, CDM_Show);
      goto L01B342;
    }
  }
  L01B342:;
  Script_scenToggleVisibleFor(CHARA_MEYS, 0);
  Script_CycleManagerUtils(CYCLE_CMD_LOAD_OR_RESET_RUNNING_PROGRAM, task_2D4);
  Script_CycleManagerUtils(CYCLE_CMD_PARSE_FOR_SCRIPT, 0x5359454D, 0);
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 1);
  return;
}

static void script_1B370(void) {
  Script_ambSoundUtils(3, 0, &data_2109C, 0.7f, 3.0f, 1.0f, 0, &data_210CC);
  Script_ambSoundUtils(4, 0, &data_210A8, 0.7f, 3.0f, 1.0f, 0, &data_21154);
  Script_ambSoundUtils(5, 0, &data_210B4, 0.7f, 3.0f, 1.0f, 0, &data_211DC);
  Script_ambSoundUtils(6, 0, &data_210C0, 1.0f, 10.0f, 1.0f, 0, &data_21264);
  Script_Ambient1SoundUtils(0, 23976);
  return;
}

void script_1B40D(void) {
  Script_scenUtils(SCENARIO_CMD_SET_STEP_LUT, 7, 0);
  task_29C = Script_ryoSleepUtils(0);
  Script_SetMapFlags(0, 0);
  Script_filmCheckOrSetTransitionUtil(0, 255, 0, 0, 0, 0, 0, 0, 0);
  Script_GetSetWeatherSettings(WEATHER_SETTINGS_SET_DISABLE_WEATHER, 1);
  Script_WeatherUtils_0(WeatherDataUtil_ApplyCurrentWeatherOrEnableDateTimeRead, 0);
  Script_WeatherTaskUtil(InitTaskData);
  Script_WeatherUtils(WeatherScript_ApplyWccpClipSet1, -72.03f, 52.29f, -81.73f, 62.03f, -92.19f, 51.56f, -82.49f, 41.83f, -1.0f, 4.0f);
  Script_WeatherTaskUtil(ToggleFogTask, 0);
  Script_WeatherTaskUtil(ToggleSceneLightingFlags, 0);
  Script_SetGlobalSceneSlotValue34(2, 1);
  Script_SetupLightFromTable(0);
  Script_ProcessFogFromTable(0);
  Script_PREP_Related(0, 1);
  Script_PREP_Related(1, 1);
  Script_scenCreateTask(script_1BDBD, 0);
  script_1B370();
  Script_Ambient1SoundUtils(1, 24232);
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 2, "MPK00");
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 1);
  Script_LoadHandMot(2, SHEN_NAME('MKYU'), "HMOT_KYU.BIN");
  Script_LoadHandMot(2, SHEN_NAME('MKYU'), "HMOT_JBX.BIN");
  Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_TASK, 1, 0);
  script_1BEF4();
  script_1BDE6();
  Script_LoadVoiceFile(1, "SA1114");
  Script_facemotUtils(CHARA_AKIR, 1, 0);
  script_B4BC(2, 3);
  script_1381D();
  script_F55A(0);
  script_ACDB(CHARA_AKIR);
  script_7FBB(CHARA_AKIR, (DSWKDoorEntry *)&data_1DD18, (int32_t *)data_1DD80, (DoorResourceDescriptor *)&data_1DD94, (MapObjectRecord *)&data_1DDA4, 10, 0);
  SCNF_WaitFrames_0000(2);
  DSWK_DoorUtils(DOOR_CMD_SET_STATE_1);
  task_2A0 = -1;
  Script_EVT_GetPosition(CHARA_AKIR, -1, &task_26C, ENTITY_XFORM_WORLD_SPACE);
  Script_mtwkGetSetRotation(CHARA_AKIR, &task_278, 0x42000000);
  Script_ReturnTimeDate(&task_284);
  SCNF_WaitFrames_0000(1);
  script_1B822();
  Script_scenCreateTask(script_1B729, 0);
  Script_scenCreateTask(script_1B931, 0);
  Script_TimedTransitionUtils(TIMED_TRANSITION_REGISTER, 2, 0x5953464D, 19, (const void *)-1, 0x14FBAE0, 6);
  Script_scenCreateTask(script_1BD6B, 0);
  Script_scenToggleVisibleFor(SHEN_NAME('JIH0'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('JIH1'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('JIH2'), 0);
  Script_scenToggleVisibleFor(SHEN_NAME('JBOX'), 0);
  Script_motiLoad(2, SHEN_NAME('FREE'), "M_MOBJ.BIN");
  Script_CycleManagerUtils(CYCLE_CMD_SETUP, "scene/02/MFSY", (int32_t)"CYCLEMAN.BIN", 0x55594B4D);
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
  Script_scenCreateTask(script_1B900, 0);
  Script_scenCreateTask(script_19EF5, 0);
  script_1A577();
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 1);
  Script_uiUtils(0, 56, 0);
  if (task_29C) {
    Script_ryoSleepUtils(1);
  }
  Script_ryoSleepUtils(2, 1);
  Script_CycleManagerUtils(CYCLE_CMD_RESET_LOAD_SETTLE);
  for (;;) {
    if (Script_CycleManagerUtils(CYCLE_CMD_TICK_LOAD_SETTLE_COUNT) == 0) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_filmCheckOrSetTransitionUtil(30, 255, 0, 0, 0, 0, 0, 0, 0);
  while (Script_filmCheckOrSetTransitionUtil(-1)) {
    SCNF_WaitFrames_0000(1);
  }
  Script_shenClearFreeState(1);
  Script_ryoControl(RYO_RestoreControl);
  return;
}

static int32_t script_1B729(void) {
  int32_t local_FC;
  for (;;) {
    Script_EVT_GetPosition(CHARA_AKIR, -1, &task_26C, ENTITY_XFORM_WORLD_SPACE);
    Script_mtwkGetSetRotation(CHARA_AKIR, &task_278, 0x42000000);
    Script_ReturnTimeDate(&task_284);
    if ((&task_10)->mode) {
      local_FC = task_10.entry_idx;
      if (local_FC == 0) {
        Script_scenUtils(SCENARIO_CMD_INIT_SCNT);
        SCNF_WaitFrames_0000(90);
        Script_executeAicaCommand(2720, 1, 30);
        Script_executeAicaCommand(2720, 2, 30);
        Script_executeAicaCommand(2720, 3, 30);
        Script_filmCheckOrSetTransitionUtil(30, 0, 0, 0, 0, 255, 0, 0, 0);
        while (Script_filmCheckOrSetTransitionUtil(-1)) {
          SCNF_WaitFrames_0000(1);
        }
        script_1B8D8();
        Script_shenRequestGameModuleTransition(2, SHEN_NAME('MFSY'), 19);
        SCNF_WaitFrames_0000(1);
        goto L01B7E9;
      }
    }
    L01B7E9:;
    if (Script_SetTaskDataValue(CHARA_AKIR, &data_2133C, &data_21348, 0)) {
      Script_mapmSetRenderFlag(1, 0);
    } else {
      Script_mapmSetRenderFlag(1, 1);
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static void script_1B822(void) {
  Vector3 local_F4;
  local_F4.x = (&local_F4)->z = 0.0f;
  (&local_F4)->y = 0.00980392192f;
  task_2A0 = Script_EffectUtils(0, &data_21354, &data_21360, 50, 0.08f, 0, 0);
  Script_EffectUtils(12, task_2A0, 8);
  Script_EffectUtils(11, task_2A0, 255);
  Script_EffectUtils(9, task_2A0, 193, 193, 191);
  Script_EffectUtils(10, task_2A0, 0.356862754f);
  Script_EffectUtils(8, task_2A0, 0.392156869f, 0.431372553f);
  Script_EffectUtils(7, task_2A0, 0.052941177f);
  Script_EffectUtils(6, task_2A0, &local_F4, &data_2136C);
  Script_EffectUtils(1, task_2A0);
  return;
}

static void script_1B8D8(void) {
  if (task_2A0 != -1) {
    Script_EffectUtils(2, task_2A0);
    Script_EffectUtils(3, task_2A0);
    task_2A0 = -1;
  }
  return;
}

static int32_t script_1B900(void) {
  int8_t local_FF;
  int32_t local_F8;
  script_AB36();
  for (;;) {
    local_FF = script_AB88();
    local_F8 = local_FF;
    if (local_FF != -1) {
      script_AB60();
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_1B931(void) {
  Vector3 local_F4;
  int32_t local_F0;
  int8_t local_EF;
  int32_t local_E8;
  int32_t local_E4;
  (void)((local_EF = 0) & 4);
  for (;;) {
    if (Script_IsPlayerAllowed(0)) {
      local_F4.x = (&task_26C)->x + Script_MathUtils(SCRIPT_MATH_SIN_FIXED16, task_278.y) * 0.3f;
      (&local_F4)->y = 0.0f;
      (&local_F4)->z = task_26C.z + Script_MathUtils(SCRIPT_MATH_COS_FIXED16, task_278.y) * 0.3f;
      local_F0 = 0;
      local_E8 = Script_GetControllerInput(1);
      if (Script_shenGetStats(STATS_CASH) > 260) {
        if (Script_CheckTargetWithinAngleRange(&data_21378, 1.5f, 0x2000, 12743, &task_26C) & Script_CheckTargetWithinAngleRange(&local_F4, 1.4f, task_278.y, 7281, &data_21378)) {
          local_F0 = 0x3048494A;
        } else if (Script_CheckTargetWithinAngleRange(&data_21384, 1.5f, 0x2000, 12743, &task_26C) & Script_CheckTargetWithinAngleRange(&local_F4, 1.4f, task_278.y, 7281, &data_21384)) {
          local_F0 = 0x3148494A;
        } else {
          if (Script_CheckTargetWithinAngleRange(&data_21390, 1.5f, 0x2000, 12743, &task_26C) & Script_CheckTargetWithinAngleRange(&local_F4, 1.4f, task_278.y, 7281, &data_21390)) {
            local_F0 = 0x3248494A;
            goto L01BA7B;
          }
        }
      }
      L01BA7B:;
      if (Script_CheckTargetWithinAngleRange(&data_2139C, 1.5f, 0x6000, 5461, &task_26C) & Script_CheckTargetWithinAngleRange(&local_F4, 1.4f, task_278.y, 7281, &data_2139C)) {
        local_F0 = 0x33524144;
      } else {
        if (Script_CheckTargetWithinAngleRange(&data_213A8, 1.5f, 0x6000, 5461, &task_26C) & Script_CheckTargetWithinAngleRange(&local_F4, 1.4f, task_278.y, 7281, &data_213A8)) {
          local_F0 = 0x54524144;
          goto L01BB03;
        }
      }
      L01BB03:;
      if (script_13C61()) {
        local_F0 = 0x48434147;
      }
      if (script_D77C()) {
        local_F0 = 0x584F424A;
      }
      if (local_F0) {
        if (~local_EF) {
          Script_sicnUtils(0, 5, 1);
          (void)((local_EF = -1) & 2);
        }
        local_E4 = local_F0;
        if ((local_E4 == 0x3048494A) | (local_E4 == 0x3148494A) | (local_E4 == 0x3248494A)) {
          Script_uiUtils(0, 25, 0);
          if (local_E8 == 0x400) {
            if (local_EF & 0x80) {
              Script_sicnUtils(1, 0, 0);
              (void)((local_EF = 0) & 2);
            }
            Script_scenToggleActiveFor(CHARA_AKIR, 0);
            script_E07B(CHARA_AKIR, local_F0, SHEN_NAME('JIHN'), SHEN_NAME('HAND'));
            Script_scenToggleActiveFor(CHARA_AKIR, 1);
          }
        } else {
          switch (local_E4) {
            case 0x54524144:
              Script_uiUtils(0, 47, 0);
              if (local_E8 == 0x400) {
                if (local_EF & 0x80) {
                  Script_sicnUtils(1, 0, 0);
                  (void)((local_EF = 0) & 2);
                }
                script_F934(SHEN_NAME('DART'));
              }
              break;
            case 0x33524144:
              Script_uiUtils(0, 3, 0);
              if (local_E8 == 0x400) {
                if (local_EF & 0x80) {
                  Script_sicnUtils(1, 0, 0);
                  (void)((local_EF = 0) & 2);
                }
                script_F934(SHEN_NAME('DAR3'));
              }
              break;
            case 0x48434147:
              Script_uiUtils(0, 6, 0);
              if (local_E8 == 0x400) {
                if (local_EF & 0x80) {
                  Script_sicnUtils(1, 0, 0);
                  (void)((local_EF = 0) & 2);
                }
                script_15DAF();
              }
              break;
            case 0x584F424A:
              Script_uiUtils(0, 7, 0);
              if (local_E8 == 0x400) {
                if (local_EF & 0x80) {
                  Script_sicnUtils(1, 0, 0);
                  (void)((local_EF = 0) & 2);
                }
                Script_scenToggleVisibleFor(SHEN_NAME('JBOX'), 1);
                script_BA57();
                Script_scenToggleVisibleFor(SHEN_NAME('JBOX'), 0);
              }
              break;
          }
        }
      } else {
        if (local_EF & 0x80) {
          Script_uiUtils(0, 56, 0);
          Script_sicnUtils(1, 0, 0);
          (void)((local_EF = 0) & 2);
        }
      }
    } else {
      if (local_EF & 0x80) {
        (void)((local_EF = 0) & 2);
        Script_sicnUtils(1, 0, 0);
      }
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_1BD6B(void) {
  while (1) {
    if (task_284.hours >= 22) {
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_EV_STOP_ACTIVE, 1);
      while (~Script_ryoControl(PCCTL_QueryShouldRun)) {
        SCNF_WaitFrames_0000(1);
      }
      Script_scenUtils(SCENARIO_CMD_SET_SHENMUE_FLAG, SHENMUE_FLAG_EV_STOP_ACTIVE, 0);
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  end:;
}

static int32_t script_1BDBD(void) {
  while (1) {
    if (Script_filmCheckOrSetTransitionUtil(-1)) {
      break;
    }
    SCNF_WaitFrames_0000(1);
  }
  Script_cdttLoadScrUtils(1, 0);
  end:;
}

__attribute__((scn_string_bindings(0, data_213E4, 2, data_213F9, 4, data_2140E, 6, data_21423, 8, data_21438)))
static void script_1BDE6(void) {
  int32_t local_FC;
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 0);
  local_FC = task_2D0;
  switch (local_FC) {
    case 0:
      script_1BED0("/scene/02/MKYU/", "MKY0");
      break;
    case 1:
      script_1BED0("/scene/02/MKYU/", "MKY1");
      break;
    case 2:
      script_1BED0("/scene/02/MKYU/", "MKY2");
      break;
    case 3:
      script_1BED0("/scene/02/MKYU/", "MKY3");
      break;
    case 4:
      script_1BED0("/scene/02/MKYU/", "MKY4");
      break;
  }
  Script_CycleManagerUtils(CYCLE_CMD_LINK_CYCLES_IN_STACK, 1);
  return;
}

static void script_1BE93(void) {
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 2)) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_1BEAD(const char *arg0, const char *arg1) {
  while (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 0, arg0, arg1) == 0) {
    SCNF_WaitFrames_0000(1);
  }
  return;
}

static void script_1BED0(const char *arg0, const char *arg1) {
  if (Script_EV_PAK_AFS_Utils(EVPAK_MODE_PACK_FILE, 5, arg1)) {
  } else {
    script_1BEAD(arg0, arg1);
    script_1BE93();
  }
  return;
}

static void script_1BEF4(void) {
  int32_t local_FC;
  Script_ReturnTimeDate(&task_290);
  local_FC = task_290.dayOfWeek;
  switch (local_FC) {
    case 0:
      break;
    case 1:
      break;
    case 2:
      break;
    case 3:
      break;
    case 4:
      break;
    case 5:
      break;
    case 6:
      break;
  }
  if (task_290.month == 12) {
    task_2D0 = 0;
  } else if (task_290.month == 1) {
    task_2D0 = 31;
  } else if (task_290.month == 2) {
    task_2D0 = 62;
  } else if (task_290.month == 3) {
    task_2D0 = 90;
  } else {
    task_2D0 = 121;
  }
  task_2D0 = task_2D0 + task_290.day + 1;
  task_2D0 = task_2D0 / 7;
  task_2D0 = task_2D0 % 5;
  return;
}
